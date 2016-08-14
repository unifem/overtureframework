! This file automatically generated from bcOptMaxwell4.bf with bpp.
        subroutine bcOptMaxwell2dOrder4( nd, nd1a,nd1b,nd2a,nd2b,nd3a,
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
     & js3a,ks1a,ks2a,ks3a,forcingOption,useChargeDensity,fieldOption,
     & boundaryForcingOption
        real dr(0:2), dx(0:2), t, uv(0:5), uvm(0:5), uv0(0:5), uvp(0:5)
     & , uvm2(0:5), uvp2(0:5), ubv(0:5)
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
     & twilightZoneForcing, gaussianChargeSource, 
     & userDefinedForcingOption
      integer noBoundaryForcing,planeWaveBoundaryForcing,
     & chirpedPlaneWaveBoundaryForcing
      parameter(noForcing                =0,
     & magneticSinusoidalPointSource =1,gaussianSource                
     & =2,twilightZoneForcing           =3,    gaussianChargeSource   
     &        =4,userDefinedForcingOption      =5 )
      ! boundary forcing options when solved directly for the scattered field:
      parameter( noBoundaryForcing              =0,   
     & planeWaveBoundaryForcing       =1,
     & chirpedPlaneWaveBoundaryForcing=2 )
        integer i1,i2,i3,j1,j2,j3,axisp1,axisp2,en1,et1,et2,hn1,ht1,
     & ht2,numberOfGhostPoints
        integer extra,extra1a,extra1b,extra2a,extra2b,extra3a,extra3b
        integer nn1a,nn1b,nn2a,nn2b,nn3a,nn3b
        integer nextra1a,nextra1b,nextra2a,nextra2b,nextra3a,nextra3b
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
        ! variables for the chirped-plane-wave (cpw)
        real xi,xi0,phi,phip,phipp,chirp,cpwTa,cpwTb,cpwBeta,cpwAlpha,
     & cpwAmp,cpwX0,cpwY0,cpwZ0,cpwTau,cpwxi
        real amp,ampp,amppp, sinp,cosp, tanha,tanhap,tanhapp, tanhb,
     & tanhbp,tanhbpp
        real an1,an2,an3, aNormSqInverse,nDotE,epsX
        integer numberOfTimeDerivatives
        real dteps,utDiff
        real t1,t2,t3,t4,t5,t6,t7,t8,t9
        real t10,t11,t12,t13,t14,t15,t16,t17,t18,t19
        real t20,t21,t22,t23,t24,t25,t26,t27,t28,t29
        real t30,t31,t32,t33,t34,t35,t36,t37,t38,t39
        real t40,t41,t42,t43,t44,t45,t46,t47,t48,t49
        real t50,t51,t52,t53,t54,t55,t56,t57,t58,t59
        real t60,t61,t62,t63,t64,t65,t66,t67,t68,t69
        real t70,t71,t72,t73,t74,t75,t76,t77,t78,t79
        real t80,t81,t82,t83,t84,t85,t86,t87,t88,t89
        real t90,t91,t92,t93,t94,t95,t96,t97,t98,t99
        real t100,t101,t102,t103,t104,t105,t106,t107,t108,t109
        real t110,t111,t112,t113,t114,t115,t116,t117,t118,t119
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
        boundaryForcingOption=ipar(32)  ! option when solving for scattered field directly
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
        ! variables for the chirped-plane-wave (cpw)
        cpwTa                =rpar(29)   ! turn on chirp
        cpwTb                =rpar(30)   ! turn off chirp
        cpwAlpha             =rpar(31)   ! chirp-rate
        cpwBeta              =rpar(32)   ! exponent in tanh
        cpwAmp               =rpar(33)   ! amplitude
        cpwX0                =rpar(34)   ! x0
        cpwY0                =rpar(35)   ! y0
        cpwZ0                =rpar(36)   ! z0
        if( abs(pwc(0))+abs(pwc(1))+abs(pwc(2)) .eq. 0. )then
          ! sanity check
          stop 12345
        end if
        dxa=dx(0)
        dya=dx(1)
        dza=dx(2)
        epsX = 1.e-30  ! epsilon used to avoid division by zero in the normal computation -- should be REAL_MIN*100 ??
          ! In parallel the dimension may not be the same as the bounds nd1a,nd1b,...
        md1a=dimension(0,0)
        md1b=dimension(1,0)
        md2a=dimension(0,1)
        md2b=dimension(1,1)
        md3a=dimension(0,2)
        md3b=dimension(1,2)
        twoPi=8.*atan2(1.,1.)
        cc= c*sqrt( kx*kx+ky*ky+kz*kz )
        ! write(*,'("initializeBoundaryForcing slowStartInterval=",e10.2)') slowStartInterval
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
              ! assign values on boundary when there are boundary forcings
             ! **  assignBoundaryForcingBoundaryValues(2)
              if( boundaryForcingOption.ne.noBoundaryForcing )then
               ! For boundaryForcing we need to implement forced BCs
               !    v = g(y,t)
               !    v_xx = (1/c^2) ( g_tt ) - g_yy
               ! etc. 
               write(*,'(" bcOptMX:ERROR: boundaryForcingOption not 
     & implemented for rectangular grids")')
               stop 7734
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
                  u(i1-2*is1,i2-2*is2,i3-2*is3,en1)= u(i1+2*is1,i2+2*
     & is2,i3+2*is3,en1)
                  u(i1-2*is1,i2-2*is2,i3-2*is3,et1)=2.*u(i1,i2,i3,et1)-
     & u(i1+2*is1,i2+2*is2,i3+2*is3,et1)
                    u(i1-2*is1,i2-2*is2,i3-2*is3,hz)=u(i1+2*is1,i2+2*
     & is2,i3+2*is3,hz)
               end if ! mask
              end do
              end do
              end do
           else
              if( debug.gt.1 )then
                write(*,'(" bc4r: **START** grid=",i4," side,axis=",
     & 2i2)') grid,side,axis
              end if
              ! assign values on boundary when there are boundary forcings
             ! **  assignBoundaryForcingBoundaryValues(2)
              if( boundaryForcingOption.ne.noBoundaryForcing )then
               ! For boundaryForcing we need to implement forced BCs
               !    v = g(y,t)
               !    v_xx = (1/c^2) ( g_tt ) - g_yy
               ! etc. 
               write(*,'(" bcOptMX:ERROR: boundaryForcingOption not 
     & implemented for rectangular grids")')
               stop 7734
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
                  u(i1-2*is1,i2-2*is2,i3-2*is3,en1)= u(i1+2*is1,i2+2*
     & is2,i3+2*is3,en1)
                  u(i1-2*is1,i2-2*is2,i3-2*is3,et1)=2.*u(i1,i2,i3,et1)-
     & u(i1+2*is1,i2+2*is2,i3+2*is3,et1)
                    u(i1-2*is1,i2-2*is2,i3-2*is3,hz)=u(i1+2*is1,i2+2*
     & is2,i3+2*is3,hz)
                     call ogf2dfo(ep,fieldOption,xy(i1-2*is1,i2-2*is2,
     & i3,0),xy(i1-2*is1,i2-2*is2,i3,1),t,uvm(ex),uvm(ey),uvm(hz))
                     call ogf2dfo(ep,fieldOption,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),t,uv0(ex),uv0(ey),uv0(hz))
                     call ogf2dfo(ep,fieldOption,xy(i1+2*is1,i2+2*is2,
     & i3,0),xy(i1+2*is1,i2+2*is2,i3,1),t,uvp(ex),uvp(ey),uvp(hz))
                    u(i1-2*is1,i2-2*is2,i3,en1)=u(i1-2*is1,i2-2*is2,i3,
     & en1) + uvm(en1) - uvp(en1)
                    u(i1-2*is1,i2-2*is2,i3,et1)=u(i1-2*is1,i2-2*is2,i3,
     & et1) + uvm(et1) -2.*uv0(et1) + uvp(et1)
                    u(i1-2*is1,i2-2*is2,i3,hz )=u(i1-2*is1,i2-2*is2,i3,
     & hz ) + uvm(hz)-uvp(hz)
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
                  ! assign values on boundary when there are boundary forcings
                  !! assignBoundaryForcingBoundaryValuesCurvilinear(2)
                  ! Since is1 is +1 or -1 we need to flip the sign of dr in the derivative approximations
                  dra = dr(axis)*(1-2*side)
                  dsa = dr(axisp1)*(1-2*side)
                  drb = dr(axis  )
                  dsb = dr(axisp1)
                  if( debug .gt.0 )then
                   write(*,'(" ******* Start: grid=",i2," side,axis=",
     & 2i2)') grid,side,axis
                  end if
                  do i3=n3a,n3b
                  do i2=n2a,n2b
                  do i1=n1a,n1b
                  if( mask(i1,i2,i3).gt.0 )then
                   jacm1=1./(rx(i1-is1,i2-is2,i3)*sy(i1-is1,i2-is2,i3)-
     & ry(i1-is1,i2-is2,i3)*sx(i1-is1,i2-is2,i3))
                   a11m1 =rsxy(i1-is1,i2-is2,i3,axis  ,0)*jacm1
                   a12m1 =rsxy(i1-is1,i2-is2,i3,axis  ,1)*jacm1
                   jac=1./(rx(i1,i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)*sx(
     & i1,i2,i3))
                   a11 =rsxy(i1,i2,i3,axis  ,0)*jac
                   a12 =rsxy(i1,i2,i3,axis  ,1)*jac
                   a21 =rsxy(i1,i2,i3,axisp1,0)*jac
                   a22 =rsxy(i1,i2,i3,axisp1,1)*jac
                   jacp1=1./(rx(i1+is1,i2+is2,i3)*sy(i1+is1,i2+is2,i3)-
     & ry(i1+is1,i2+is2,i3)*sx(i1+is1,i2+is2,i3))
                   a11p1=rsxy(i1+is1,i2+is2,i3,axis,0)*jacp1
                   a12p1=rsxy(i1+is1,i2+is2,i3,axis,1)*jacp1
                   jacm2=1./(rx(i1-2*is1,i2-2*is2,i3)*sy(i1-2*is1,i2-2*
     & is2,i3)-ry(i1-2*is1,i2-2*is2,i3)*sx(i1-2*is1,i2-2*is2,i3))
                   a11m2=rsxy(i1-2*is1,i2-2*is2,i3,axis,0)*jacm2
                   a12m2=rsxy(i1-2*is1,i2-2*is2,i3,axis,1)*jacm2
                   jacp2=1./(rx(i1+2*is1,i2+2*is2,i3)*sy(i1+2*is1,i2+2*
     & is2,i3)-ry(i1+2*is1,i2+2*is2,i3)*sx(i1+2*is1,i2+2*is2,i3))
                   a11p2=rsxy(i1+2*is1,i2+2*is2,i3,axis,0)*jacp2
                   a12p2=rsxy(i1+2*is1,i2+2*is2,i3,axis,1)*jacp2
                  a11r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axis,0)/(rx(
     & i1+is1,i2+is2,i3+is3)*sy(i1+is1,i2+is2,i3+is3)-ry(i1+is1,i2+
     & is2,i3+is3)*sx(i1+is1,i2+is2,i3+is3)))-(rsxy(i1-is1,i2-is2,i3-
     & is3,axis,0)/(rx(i1-is1,i2-is2,i3-is3)*sy(i1-is1,i2-is2,i3-is3)-
     & ry(i1-is1,i2-is2,i3-is3)*sx(i1-is1,i2-is2,i3-is3))))-((rsxy(i1+
     & 2*is1,i2+2*is2,i3+2*is3,axis,0)/(rx(i1+2*is1,i2+2*is2,i3+2*is3)
     & *sy(i1+2*is1,i2+2*is2,i3+2*is3)-ry(i1+2*is1,i2+2*is2,i3+2*is3)*
     & sx(i1+2*is1,i2+2*is2,i3+2*is3)))-(rsxy(i1-2*is1,i2-2*is2,i3-2*
     & is3,axis,0)/(rx(i1-2*is1,i2-2*is2,i3-2*is3)*sy(i1-2*is1,i2-2*
     & is2,i3-2*is3)-ry(i1-2*is1,i2-2*is2,i3-2*is3)*sx(i1-2*is1,i2-2*
     & is2,i3-2*is3)))))/(12.*dra)
                  a12r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axis,1)/(rx(
     & i1+is1,i2+is2,i3+is3)*sy(i1+is1,i2+is2,i3+is3)-ry(i1+is1,i2+
     & is2,i3+is3)*sx(i1+is1,i2+is2,i3+is3)))-(rsxy(i1-is1,i2-is2,i3-
     & is3,axis,1)/(rx(i1-is1,i2-is2,i3-is3)*sy(i1-is1,i2-is2,i3-is3)-
     & ry(i1-is1,i2-is2,i3-is3)*sx(i1-is1,i2-is2,i3-is3))))-((rsxy(i1+
     & 2*is1,i2+2*is2,i3+2*is3,axis,1)/(rx(i1+2*is1,i2+2*is2,i3+2*is3)
     & *sy(i1+2*is1,i2+2*is2,i3+2*is3)-ry(i1+2*is1,i2+2*is2,i3+2*is3)*
     & sx(i1+2*is1,i2+2*is2,i3+2*is3)))-(rsxy(i1-2*is1,i2-2*is2,i3-2*
     & is3,axis,1)/(rx(i1-2*is1,i2-2*is2,i3-2*is3)*sy(i1-2*is1,i2-2*
     & is2,i3-2*is3)-ry(i1-2*is1,i2-2*is2,i3-2*is3)*sx(i1-2*is1,i2-2*
     & is2,i3-2*is3)))))/(12.*dra)
                  a21r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axisp1,0)/(rx(
     & i1+is1,i2+is2,i3+is3)*sy(i1+is1,i2+is2,i3+is3)-ry(i1+is1,i2+
     & is2,i3+is3)*sx(i1+is1,i2+is2,i3+is3)))-(rsxy(i1-is1,i2-is2,i3-
     & is3,axisp1,0)/(rx(i1-is1,i2-is2,i3-is3)*sy(i1-is1,i2-is2,i3-
     & is3)-ry(i1-is1,i2-is2,i3-is3)*sx(i1-is1,i2-is2,i3-is3))))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp1,0)/(rx(i1+2*is1,i2+2*
     & is2,i3+2*is3)*sy(i1+2*is1,i2+2*is2,i3+2*is3)-ry(i1+2*is1,i2+2*
     & is2,i3+2*is3)*sx(i1+2*is1,i2+2*is2,i3+2*is3)))-(rsxy(i1-2*is1,
     & i2-2*is2,i3-2*is3,axisp1,0)/(rx(i1-2*is1,i2-2*is2,i3-2*is3)*sy(
     & i1-2*is1,i2-2*is2,i3-2*is3)-ry(i1-2*is1,i2-2*is2,i3-2*is3)*sx(
     & i1-2*is1,i2-2*is2,i3-2*is3)))))/(12.*dra)
                  a22r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axisp1,1)/(rx(
     & i1+is1,i2+is2,i3+is3)*sy(i1+is1,i2+is2,i3+is3)-ry(i1+is1,i2+
     & is2,i3+is3)*sx(i1+is1,i2+is2,i3+is3)))-(rsxy(i1-is1,i2-is2,i3-
     & is3,axisp1,1)/(rx(i1-is1,i2-is2,i3-is3)*sy(i1-is1,i2-is2,i3-
     & is3)-ry(i1-is1,i2-is2,i3-is3)*sx(i1-is1,i2-is2,i3-is3))))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp1,1)/(rx(i1+2*is1,i2+2*
     & is2,i3+2*is3)*sy(i1+2*is1,i2+2*is2,i3+2*is3)-ry(i1+2*is1,i2+2*
     & is2,i3+2*is3)*sx(i1+2*is1,i2+2*is2,i3+2*is3)))-(rsxy(i1-2*is1,
     & i2-2*is2,i3-2*is3,axisp1,1)/(rx(i1-2*is1,i2-2*is2,i3-2*is3)*sy(
     & i1-2*is1,i2-2*is2,i3-2*is3)-ry(i1-2*is1,i2-2*is2,i3-2*is3)*sx(
     & i1-2*is1,i2-2*is2,i3-2*is3)))))/(12.*dra)
                  a11s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axis,0)/(rx(
     & i1+js1,i2+js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+
     & js2,i3+js3)*sx(i1+js1,i2+js2,i3+js3)))-(rsxy(i1-js1,i2-js2,i3-
     & js3,axis,0)/(rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-
     & ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3))))-((rsxy(i1+
     & 2*js1,i2+2*js2,i3+2*js3,axis,0)/(rx(i1+2*js1,i2+2*js2,i3+2*js3)
     & *sy(i1+2*js1,i2+2*js2,i3+2*js3)-ry(i1+2*js1,i2+2*js2,i3+2*js3)*
     & sx(i1+2*js1,i2+2*js2,i3+2*js3)))-(rsxy(i1-2*js1,i2-2*js2,i3-2*
     & js3,axis,0)/(rx(i1-2*js1,i2-2*js2,i3-2*js3)*sy(i1-2*js1,i2-2*
     & js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,i3-2*js3)*sx(i1-2*js1,i2-2*
     & js2,i3-2*js3)))))/(12.*dsa)
                  a12s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axis,1)/(rx(
     & i1+js1,i2+js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+
     & js2,i3+js3)*sx(i1+js1,i2+js2,i3+js3)))-(rsxy(i1-js1,i2-js2,i3-
     & js3,axis,1)/(rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-
     & ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3))))-((rsxy(i1+
     & 2*js1,i2+2*js2,i3+2*js3,axis,1)/(rx(i1+2*js1,i2+2*js2,i3+2*js3)
     & *sy(i1+2*js1,i2+2*js2,i3+2*js3)-ry(i1+2*js1,i2+2*js2,i3+2*js3)*
     & sx(i1+2*js1,i2+2*js2,i3+2*js3)))-(rsxy(i1-2*js1,i2-2*js2,i3-2*
     & js3,axis,1)/(rx(i1-2*js1,i2-2*js2,i3-2*js3)*sy(i1-2*js1,i2-2*
     & js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,i3-2*js3)*sx(i1-2*js1,i2-2*
     & js2,i3-2*js3)))))/(12.*dsa)
                  a21s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axisp1,0)/(rx(
     & i1+js1,i2+js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+
     & js2,i3+js3)*sx(i1+js1,i2+js2,i3+js3)))-(rsxy(i1-js1,i2-js2,i3-
     & js3,axisp1,0)/(rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-
     & js3)-ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3))))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,0)/(rx(i1+2*js1,i2+2*
     & js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,i3+2*js3)-ry(i1+2*js1,i2+2*
     & js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,i3+2*js3)))-(rsxy(i1-2*js1,
     & i2-2*js2,i3-2*js3,axisp1,0)/(rx(i1-2*js1,i2-2*js2,i3-2*js3)*sy(
     & i1-2*js1,i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,i3-2*js3)*sx(
     & i1-2*js1,i2-2*js2,i3-2*js3)))))/(12.*dsa)
                  a22s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axisp1,1)/(rx(
     & i1+js1,i2+js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+
     & js2,i3+js3)*sx(i1+js1,i2+js2,i3+js3)))-(rsxy(i1-js1,i2-js2,i3-
     & js3,axisp1,1)/(rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-
     & js3)-ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3))))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,1)/(rx(i1+2*js1,i2+2*
     & js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,i3+2*js3)-ry(i1+2*js1,i2+2*
     & js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,i3+2*js3)))-(rsxy(i1-2*js1,
     & i2-2*js2,i3-2*js3,axisp1,1)/(rx(i1-2*js1,i2-2*js2,i3-2*js3)*sy(
     & i1-2*js1,i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,i3-2*js3)*sx(
     & i1-2*js1,i2-2*js2,i3-2*js3)))))/(12.*dsa)
                  a11rs = (8.*((8.*((rsxy(i1+1,i2+1,i3,axis,0)/(rx(i1+
     & 1,i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+1,i3)))
     & -(rsxy(i1-1,i2+1,i3,axis,0)/(rx(i1-1,i2+1,i3)*sy(i1-1,i2+1,i3)-
     & ry(i1-1,i2+1,i3)*sx(i1-1,i2+1,i3))))-((rsxy(i1+2,i2+1,i3,axis,
     & 0)/(rx(i1+2,i2+1,i3)*sy(i1+2,i2+1,i3)-ry(i1+2,i2+1,i3)*sx(i1+2,
     & i2+1,i3)))-(rsxy(i1-2,i2+1,i3,axis,0)/(rx(i1-2,i2+1,i3)*sy(i1-
     & 2,i2+1,i3)-ry(i1-2,i2+1,i3)*sx(i1-2,i2+1,i3)))))/(12.*dr(0))-(
     & 8.*((rsxy(i1+1,i2-1,i3,axis,0)/(rx(i1+1,i2-1,i3)*sy(i1+1,i2-1,
     & i3)-ry(i1+1,i2-1,i3)*sx(i1+1,i2-1,i3)))-(rsxy(i1-1,i2-1,i3,
     & axis,0)/(rx(i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-ry(i1-1,i2-1,i3)*sx(
     & i1-1,i2-1,i3))))-((rsxy(i1+2,i2-1,i3,axis,0)/(rx(i1+2,i2-1,i3)*
     & sy(i1+2,i2-1,i3)-ry(i1+2,i2-1,i3)*sx(i1+2,i2-1,i3)))-(rsxy(i1-
     & 2,i2-1,i3,axis,0)/(rx(i1-2,i2-1,i3)*sy(i1-2,i2-1,i3)-ry(i1-2,
     & i2-1,i3)*sx(i1-2,i2-1,i3)))))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2+
     & 2,i3,axis,0)/(rx(i1+1,i2+2,i3)*sy(i1+1,i2+2,i3)-ry(i1+1,i2+2,
     & i3)*sx(i1+1,i2+2,i3)))-(rsxy(i1-1,i2+2,i3,axis,0)/(rx(i1-1,i2+
     & 2,i3)*sy(i1-1,i2+2,i3)-ry(i1-1,i2+2,i3)*sx(i1-1,i2+2,i3))))-((
     & rsxy(i1+2,i2+2,i3,axis,0)/(rx(i1+2,i2+2,i3)*sy(i1+2,i2+2,i3)-
     & ry(i1+2,i2+2,i3)*sx(i1+2,i2+2,i3)))-(rsxy(i1-2,i2+2,i3,axis,0)
     & /(rx(i1-2,i2+2,i3)*sy(i1-2,i2+2,i3)-ry(i1-2,i2+2,i3)*sx(i1-2,
     & i2+2,i3)))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,i3,axis,0)/(rx(
     & i1+1,i2-2,i3)*sy(i1+1,i2-2,i3)-ry(i1+1,i2-2,i3)*sx(i1+1,i2-2,
     & i3)))-(rsxy(i1-1,i2-2,i3,axis,0)/(rx(i1-1,i2-2,i3)*sy(i1-1,i2-
     & 2,i3)-ry(i1-1,i2-2,i3)*sx(i1-1,i2-2,i3))))-((rsxy(i1+2,i2-2,i3,
     & axis,0)/(rx(i1+2,i2-2,i3)*sy(i1+2,i2-2,i3)-ry(i1+2,i2-2,i3)*sx(
     & i1+2,i2-2,i3)))-(rsxy(i1-2,i2-2,i3,axis,0)/(rx(i1-2,i2-2,i3)*
     & sy(i1-2,i2-2,i3)-ry(i1-2,i2-2,i3)*sx(i1-2,i2-2,i3)))))/(12.*dr(
     & 0))))/(12.*dr(1))
                  a12rs = (8.*((8.*((rsxy(i1+1,i2+1,i3,axis,1)/(rx(i1+
     & 1,i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+1,i3)))
     & -(rsxy(i1-1,i2+1,i3,axis,1)/(rx(i1-1,i2+1,i3)*sy(i1-1,i2+1,i3)-
     & ry(i1-1,i2+1,i3)*sx(i1-1,i2+1,i3))))-((rsxy(i1+2,i2+1,i3,axis,
     & 1)/(rx(i1+2,i2+1,i3)*sy(i1+2,i2+1,i3)-ry(i1+2,i2+1,i3)*sx(i1+2,
     & i2+1,i3)))-(rsxy(i1-2,i2+1,i3,axis,1)/(rx(i1-2,i2+1,i3)*sy(i1-
     & 2,i2+1,i3)-ry(i1-2,i2+1,i3)*sx(i1-2,i2+1,i3)))))/(12.*dr(0))-(
     & 8.*((rsxy(i1+1,i2-1,i3,axis,1)/(rx(i1+1,i2-1,i3)*sy(i1+1,i2-1,
     & i3)-ry(i1+1,i2-1,i3)*sx(i1+1,i2-1,i3)))-(rsxy(i1-1,i2-1,i3,
     & axis,1)/(rx(i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-ry(i1-1,i2-1,i3)*sx(
     & i1-1,i2-1,i3))))-((rsxy(i1+2,i2-1,i3,axis,1)/(rx(i1+2,i2-1,i3)*
     & sy(i1+2,i2-1,i3)-ry(i1+2,i2-1,i3)*sx(i1+2,i2-1,i3)))-(rsxy(i1-
     & 2,i2-1,i3,axis,1)/(rx(i1-2,i2-1,i3)*sy(i1-2,i2-1,i3)-ry(i1-2,
     & i2-1,i3)*sx(i1-2,i2-1,i3)))))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2+
     & 2,i3,axis,1)/(rx(i1+1,i2+2,i3)*sy(i1+1,i2+2,i3)-ry(i1+1,i2+2,
     & i3)*sx(i1+1,i2+2,i3)))-(rsxy(i1-1,i2+2,i3,axis,1)/(rx(i1-1,i2+
     & 2,i3)*sy(i1-1,i2+2,i3)-ry(i1-1,i2+2,i3)*sx(i1-1,i2+2,i3))))-((
     & rsxy(i1+2,i2+2,i3,axis,1)/(rx(i1+2,i2+2,i3)*sy(i1+2,i2+2,i3)-
     & ry(i1+2,i2+2,i3)*sx(i1+2,i2+2,i3)))-(rsxy(i1-2,i2+2,i3,axis,1)
     & /(rx(i1-2,i2+2,i3)*sy(i1-2,i2+2,i3)-ry(i1-2,i2+2,i3)*sx(i1-2,
     & i2+2,i3)))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,i3,axis,1)/(rx(
     & i1+1,i2-2,i3)*sy(i1+1,i2-2,i3)-ry(i1+1,i2-2,i3)*sx(i1+1,i2-2,
     & i3)))-(rsxy(i1-1,i2-2,i3,axis,1)/(rx(i1-1,i2-2,i3)*sy(i1-1,i2-
     & 2,i3)-ry(i1-1,i2-2,i3)*sx(i1-1,i2-2,i3))))-((rsxy(i1+2,i2-2,i3,
     & axis,1)/(rx(i1+2,i2-2,i3)*sy(i1+2,i2-2,i3)-ry(i1+2,i2-2,i3)*sx(
     & i1+2,i2-2,i3)))-(rsxy(i1-2,i2-2,i3,axis,1)/(rx(i1-2,i2-2,i3)*
     & sy(i1-2,i2-2,i3)-ry(i1-2,i2-2,i3)*sx(i1-2,i2-2,i3)))))/(12.*dr(
     & 0))))/(12.*dr(1))
                  a21rs = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp1,0)/(rx(
     & i1+1,i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+1,
     & i3)))-(rsxy(i1-1,i2+1,i3,axisp1,0)/(rx(i1-1,i2+1,i3)*sy(i1-1,
     & i2+1,i3)-ry(i1-1,i2+1,i3)*sx(i1-1,i2+1,i3))))-((rsxy(i1+2,i2+1,
     & i3,axisp1,0)/(rx(i1+2,i2+1,i3)*sy(i1+2,i2+1,i3)-ry(i1+2,i2+1,
     & i3)*sx(i1+2,i2+1,i3)))-(rsxy(i1-2,i2+1,i3,axisp1,0)/(rx(i1-2,
     & i2+1,i3)*sy(i1-2,i2+1,i3)-ry(i1-2,i2+1,i3)*sx(i1-2,i2+1,i3)))))
     & /(12.*dr(0))-(8.*((rsxy(i1+1,i2-1,i3,axisp1,0)/(rx(i1+1,i2-1,
     & i3)*sy(i1+1,i2-1,i3)-ry(i1+1,i2-1,i3)*sx(i1+1,i2-1,i3)))-(rsxy(
     & i1-1,i2-1,i3,axisp1,0)/(rx(i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-ry(
     & i1-1,i2-1,i3)*sx(i1-1,i2-1,i3))))-((rsxy(i1+2,i2-1,i3,axisp1,0)
     & /(rx(i1+2,i2-1,i3)*sy(i1+2,i2-1,i3)-ry(i1+2,i2-1,i3)*sx(i1+2,
     & i2-1,i3)))-(rsxy(i1-2,i2-1,i3,axisp1,0)/(rx(i1-2,i2-1,i3)*sy(
     & i1-2,i2-1,i3)-ry(i1-2,i2-1,i3)*sx(i1-2,i2-1,i3)))))/(12.*dr(0))
     & )-((8.*((rsxy(i1+1,i2+2,i3,axisp1,0)/(rx(i1+1,i2+2,i3)*sy(i1+1,
     & i2+2,i3)-ry(i1+1,i2+2,i3)*sx(i1+1,i2+2,i3)))-(rsxy(i1-1,i2+2,
     & i3,axisp1,0)/(rx(i1-1,i2+2,i3)*sy(i1-1,i2+2,i3)-ry(i1-1,i2+2,
     & i3)*sx(i1-1,i2+2,i3))))-((rsxy(i1+2,i2+2,i3,axisp1,0)/(rx(i1+2,
     & i2+2,i3)*sy(i1+2,i2+2,i3)-ry(i1+2,i2+2,i3)*sx(i1+2,i2+2,i3)))-(
     & rsxy(i1-2,i2+2,i3,axisp1,0)/(rx(i1-2,i2+2,i3)*sy(i1-2,i2+2,i3)-
     & ry(i1-2,i2+2,i3)*sx(i1-2,i2+2,i3)))))/(12.*dr(0))-(8.*((rsxy(
     & i1+1,i2-2,i3,axisp1,0)/(rx(i1+1,i2-2,i3)*sy(i1+1,i2-2,i3)-ry(
     & i1+1,i2-2,i3)*sx(i1+1,i2-2,i3)))-(rsxy(i1-1,i2-2,i3,axisp1,0)/(
     & rx(i1-1,i2-2,i3)*sy(i1-1,i2-2,i3)-ry(i1-1,i2-2,i3)*sx(i1-1,i2-
     & 2,i3))))-((rsxy(i1+2,i2-2,i3,axisp1,0)/(rx(i1+2,i2-2,i3)*sy(i1+
     & 2,i2-2,i3)-ry(i1+2,i2-2,i3)*sx(i1+2,i2-2,i3)))-(rsxy(i1-2,i2-2,
     & i3,axisp1,0)/(rx(i1-2,i2-2,i3)*sy(i1-2,i2-2,i3)-ry(i1-2,i2-2,
     & i3)*sx(i1-2,i2-2,i3)))))/(12.*dr(0))))/(12.*dr(1))
                  a22rs = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp1,1)/(rx(
     & i1+1,i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+1,
     & i3)))-(rsxy(i1-1,i2+1,i3,axisp1,1)/(rx(i1-1,i2+1,i3)*sy(i1-1,
     & i2+1,i3)-ry(i1-1,i2+1,i3)*sx(i1-1,i2+1,i3))))-((rsxy(i1+2,i2+1,
     & i3,axisp1,1)/(rx(i1+2,i2+1,i3)*sy(i1+2,i2+1,i3)-ry(i1+2,i2+1,
     & i3)*sx(i1+2,i2+1,i3)))-(rsxy(i1-2,i2+1,i3,axisp1,1)/(rx(i1-2,
     & i2+1,i3)*sy(i1-2,i2+1,i3)-ry(i1-2,i2+1,i3)*sx(i1-2,i2+1,i3)))))
     & /(12.*dr(0))-(8.*((rsxy(i1+1,i2-1,i3,axisp1,1)/(rx(i1+1,i2-1,
     & i3)*sy(i1+1,i2-1,i3)-ry(i1+1,i2-1,i3)*sx(i1+1,i2-1,i3)))-(rsxy(
     & i1-1,i2-1,i3,axisp1,1)/(rx(i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-ry(
     & i1-1,i2-1,i3)*sx(i1-1,i2-1,i3))))-((rsxy(i1+2,i2-1,i3,axisp1,1)
     & /(rx(i1+2,i2-1,i3)*sy(i1+2,i2-1,i3)-ry(i1+2,i2-1,i3)*sx(i1+2,
     & i2-1,i3)))-(rsxy(i1-2,i2-1,i3,axisp1,1)/(rx(i1-2,i2-1,i3)*sy(
     & i1-2,i2-1,i3)-ry(i1-2,i2-1,i3)*sx(i1-2,i2-1,i3)))))/(12.*dr(0))
     & )-((8.*((rsxy(i1+1,i2+2,i3,axisp1,1)/(rx(i1+1,i2+2,i3)*sy(i1+1,
     & i2+2,i3)-ry(i1+1,i2+2,i3)*sx(i1+1,i2+2,i3)))-(rsxy(i1-1,i2+2,
     & i3,axisp1,1)/(rx(i1-1,i2+2,i3)*sy(i1-1,i2+2,i3)-ry(i1-1,i2+2,
     & i3)*sx(i1-1,i2+2,i3))))-((rsxy(i1+2,i2+2,i3,axisp1,1)/(rx(i1+2,
     & i2+2,i3)*sy(i1+2,i2+2,i3)-ry(i1+2,i2+2,i3)*sx(i1+2,i2+2,i3)))-(
     & rsxy(i1-2,i2+2,i3,axisp1,1)/(rx(i1-2,i2+2,i3)*sy(i1-2,i2+2,i3)-
     & ry(i1-2,i2+2,i3)*sx(i1-2,i2+2,i3)))))/(12.*dr(0))-(8.*((rsxy(
     & i1+1,i2-2,i3,axisp1,1)/(rx(i1+1,i2-2,i3)*sy(i1+1,i2-2,i3)-ry(
     & i1+1,i2-2,i3)*sx(i1+1,i2-2,i3)))-(rsxy(i1-1,i2-2,i3,axisp1,1)/(
     & rx(i1-1,i2-2,i3)*sy(i1-1,i2-2,i3)-ry(i1-1,i2-2,i3)*sx(i1-1,i2-
     & 2,i3))))-((rsxy(i1+2,i2-2,i3,axisp1,1)/(rx(i1+2,i2-2,i3)*sy(i1+
     & 2,i2-2,i3)-ry(i1+2,i2-2,i3)*sx(i1+2,i2-2,i3)))-(rsxy(i1-2,i2-2,
     & i3,axisp1,1)/(rx(i1-2,i2-2,i3)*sy(i1-2,i2-2,i3)-ry(i1-2,i2-2,
     & i3)*sx(i1-2,i2-2,i3)))))/(12.*dr(0))))/(12.*dr(1))
                  a11rr = ((-30.*(rsxy(i1,i2,i3,axis,0)/(rx(i1,i2,i3)*
     & sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+16.*((rsxy(i1+is1,i2+
     & is2,i3+is3,axis,0)/(rx(i1+is1,i2+is2,i3+is3)*sy(i1+is1,i2+is2,
     & i3+is3)-ry(i1+is1,i2+is2,i3+is3)*sx(i1+is1,i2+is2,i3+is3)))+(
     & rsxy(i1-is1,i2-is2,i3-is3,axis,0)/(rx(i1-is1,i2-is2,i3-is3)*sy(
     & i1-is1,i2-is2,i3-is3)-ry(i1-is1,i2-is2,i3-is3)*sx(i1-is1,i2-
     & is2,i3-is3))))-((rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,0)/(rx(
     & i1+2*is1,i2+2*is2,i3+2*is3)*sy(i1+2*is1,i2+2*is2,i3+2*is3)-ry(
     & i1+2*is1,i2+2*is2,i3+2*is3)*sx(i1+2*is1,i2+2*is2,i3+2*is3)))+(
     & rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,0)/(rx(i1-2*is1,i2-2*is2,
     & i3-2*is3)*sy(i1-2*is1,i2-2*is2,i3-2*is3)-ry(i1-2*is1,i2-2*is2,
     & i3-2*is3)*sx(i1-2*is1,i2-2*is2,i3-2*is3)))))/(12.*dra**2))
                  a12rr = ((-30.*(rsxy(i1,i2,i3,axis,1)/(rx(i1,i2,i3)*
     & sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+16.*((rsxy(i1+is1,i2+
     & is2,i3+is3,axis,1)/(rx(i1+is1,i2+is2,i3+is3)*sy(i1+is1,i2+is2,
     & i3+is3)-ry(i1+is1,i2+is2,i3+is3)*sx(i1+is1,i2+is2,i3+is3)))+(
     & rsxy(i1-is1,i2-is2,i3-is3,axis,1)/(rx(i1-is1,i2-is2,i3-is3)*sy(
     & i1-is1,i2-is2,i3-is3)-ry(i1-is1,i2-is2,i3-is3)*sx(i1-is1,i2-
     & is2,i3-is3))))-((rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,1)/(rx(
     & i1+2*is1,i2+2*is2,i3+2*is3)*sy(i1+2*is1,i2+2*is2,i3+2*is3)-ry(
     & i1+2*is1,i2+2*is2,i3+2*is3)*sx(i1+2*is1,i2+2*is2,i3+2*is3)))+(
     & rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,1)/(rx(i1-2*is1,i2-2*is2,
     & i3-2*is3)*sy(i1-2*is1,i2-2*is2,i3-2*is3)-ry(i1-2*is1,i2-2*is2,
     & i3-2*is3)*sx(i1-2*is1,i2-2*is2,i3-2*is3)))))/(12.*dra**2))
                  a21rr = ((-30.*(rsxy(i1,i2,i3,axisp1,0)/(rx(i1,i2,i3)
     & *sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+16.*((rsxy(i1+is1,i2+
     & is2,i3+is3,axisp1,0)/(rx(i1+is1,i2+is2,i3+is3)*sy(i1+is1,i2+
     & is2,i3+is3)-ry(i1+is1,i2+is2,i3+is3)*sx(i1+is1,i2+is2,i3+is3)))
     & +(rsxy(i1-is1,i2-is2,i3-is3,axisp1,0)/(rx(i1-is1,i2-is2,i3-is3)
     & *sy(i1-is1,i2-is2,i3-is3)-ry(i1-is1,i2-is2,i3-is3)*sx(i1-is1,
     & i2-is2,i3-is3))))-((rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp1,0)/(
     & rx(i1+2*is1,i2+2*is2,i3+2*is3)*sy(i1+2*is1,i2+2*is2,i3+2*is3)-
     & ry(i1+2*is1,i2+2*is2,i3+2*is3)*sx(i1+2*is1,i2+2*is2,i3+2*is3)))
     & +(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axisp1,0)/(rx(i1-2*is1,i2-2*
     & is2,i3-2*is3)*sy(i1-2*is1,i2-2*is2,i3-2*is3)-ry(i1-2*is1,i2-2*
     & is2,i3-2*is3)*sx(i1-2*is1,i2-2*is2,i3-2*is3)))))/(12.*dra**2))
                  a22rr = ((-30.*(rsxy(i1,i2,i3,axisp1,1)/(rx(i1,i2,i3)
     & *sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+16.*((rsxy(i1+is1,i2+
     & is2,i3+is3,axisp1,1)/(rx(i1+is1,i2+is2,i3+is3)*sy(i1+is1,i2+
     & is2,i3+is3)-ry(i1+is1,i2+is2,i3+is3)*sx(i1+is1,i2+is2,i3+is3)))
     & +(rsxy(i1-is1,i2-is2,i3-is3,axisp1,1)/(rx(i1-is1,i2-is2,i3-is3)
     & *sy(i1-is1,i2-is2,i3-is3)-ry(i1-is1,i2-is2,i3-is3)*sx(i1-is1,
     & i2-is2,i3-is3))))-((rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp1,1)/(
     & rx(i1+2*is1,i2+2*is2,i3+2*is3)*sy(i1+2*is1,i2+2*is2,i3+2*is3)-
     & ry(i1+2*is1,i2+2*is2,i3+2*is3)*sx(i1+2*is1,i2+2*is2,i3+2*is3)))
     & +(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axisp1,1)/(rx(i1-2*is1,i2-2*
     & is2,i3-2*is3)*sy(i1-2*is1,i2-2*is2,i3-2*is3)-ry(i1-2*is1,i2-2*
     & is2,i3-2*is3)*sx(i1-2*is1,i2-2*is2,i3-2*is3)))))/(12.*dra**2))
                  a11ss = ((-30.*(rsxy(i1,i2,i3,axis,0)/(rx(i1,i2,i3)*
     & sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+16.*((rsxy(i1+js1,i2+
     & js2,i3+js3,axis,0)/(rx(i1+js1,i2+js2,i3+js3)*sy(i1+js1,i2+js2,
     & i3+js3)-ry(i1+js1,i2+js2,i3+js3)*sx(i1+js1,i2+js2,i3+js3)))+(
     & rsxy(i1-js1,i2-js2,i3-js3,axis,0)/(rx(i1-js1,i2-js2,i3-js3)*sy(
     & i1-js1,i2-js2,i3-js3)-ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-
     & js2,i3-js3))))-((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axis,0)/(rx(
     & i1+2*js1,i2+2*js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,i3+2*js3)-ry(
     & i1+2*js1,i2+2*js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,i3+2*js3)))+(
     & rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axis,0)/(rx(i1-2*js1,i2-2*js2,
     & i3-2*js3)*sy(i1-2*js1,i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,
     & i3-2*js3)*sx(i1-2*js1,i2-2*js2,i3-2*js3)))))/(12.*dsa**2))
                  a12ss = ((-30.*(rsxy(i1,i2,i3,axis,1)/(rx(i1,i2,i3)*
     & sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+16.*((rsxy(i1+js1,i2+
     & js2,i3+js3,axis,1)/(rx(i1+js1,i2+js2,i3+js3)*sy(i1+js1,i2+js2,
     & i3+js3)-ry(i1+js1,i2+js2,i3+js3)*sx(i1+js1,i2+js2,i3+js3)))+(
     & rsxy(i1-js1,i2-js2,i3-js3,axis,1)/(rx(i1-js1,i2-js2,i3-js3)*sy(
     & i1-js1,i2-js2,i3-js3)-ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-
     & js2,i3-js3))))-((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axis,1)/(rx(
     & i1+2*js1,i2+2*js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,i3+2*js3)-ry(
     & i1+2*js1,i2+2*js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,i3+2*js3)))+(
     & rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axis,1)/(rx(i1-2*js1,i2-2*js2,
     & i3-2*js3)*sy(i1-2*js1,i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,
     & i3-2*js3)*sx(i1-2*js1,i2-2*js2,i3-2*js3)))))/(12.*dsa**2))
                  a21ss = ((-30.*(rsxy(i1,i2,i3,axisp1,0)/(rx(i1,i2,i3)
     & *sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+16.*((rsxy(i1+js1,i2+
     & js2,i3+js3,axisp1,0)/(rx(i1+js1,i2+js2,i3+js3)*sy(i1+js1,i2+
     & js2,i3+js3)-ry(i1+js1,i2+js2,i3+js3)*sx(i1+js1,i2+js2,i3+js3)))
     & +(rsxy(i1-js1,i2-js2,i3-js3,axisp1,0)/(rx(i1-js1,i2-js2,i3-js3)
     & *sy(i1-js1,i2-js2,i3-js3)-ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,
     & i2-js2,i3-js3))))-((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,0)/(
     & rx(i1+2*js1,i2+2*js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,i3+2*js3)-
     & ry(i1+2*js1,i2+2*js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,i3+2*js3)))
     & +(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axisp1,0)/(rx(i1-2*js1,i2-2*
     & js2,i3-2*js3)*sy(i1-2*js1,i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*
     & js2,i3-2*js3)*sx(i1-2*js1,i2-2*js2,i3-2*js3)))))/(12.*dsa**2))
                  a22ss = ((-30.*(rsxy(i1,i2,i3,axisp1,1)/(rx(i1,i2,i3)
     & *sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+16.*((rsxy(i1+js1,i2+
     & js2,i3+js3,axisp1,1)/(rx(i1+js1,i2+js2,i3+js3)*sy(i1+js1,i2+
     & js2,i3+js3)-ry(i1+js1,i2+js2,i3+js3)*sx(i1+js1,i2+js2,i3+js3)))
     & +(rsxy(i1-js1,i2-js2,i3-js3,axisp1,1)/(rx(i1-js1,i2-js2,i3-js3)
     & *sy(i1-js1,i2-js2,i3-js3)-ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,
     & i2-js2,i3-js3))))-((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,1)/(
     & rx(i1+2*js1,i2+2*js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,i3+2*js3)-
     & ry(i1+2*js1,i2+2*js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,i3+2*js3)))
     & +(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axisp1,1)/(rx(i1-2*js1,i2-2*
     & js2,i3-2*js3)*sy(i1-2*js1,i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*
     & js2,i3-2*js3)*sx(i1-2*js1,i2-2*js2,i3-2*js3)))))/(12.*dsa**2))
                  if( .true. )then
                    a11sss = ((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axis,0)
     & /(rx(i1+2*js1,i2+2*js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,i3+2*js3)
     & -ry(i1+2*js1,i2+2*js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,i3+2*js3))
     & )-2.*(rsxy(i1+js1,i2+js2,i3+js3,axis,0)/(rx(i1+js1,i2+js2,i3+
     & js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+js2,i3+js3)*sx(i1+
     & js1,i2+js2,i3+js3)))+2.*(rsxy(i1-js1,i2-js2,i3-js3,axis,0)/(rx(
     & i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-ry(i1-js1,i2-
     & js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3)))-(rsxy(i1-2*js1,i2-2*js2,
     & i3-2*js3,axis,0)/(rx(i1-2*js1,i2-2*js2,i3-2*js3)*sy(i1-2*js1,
     & i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,i3-2*js3)*sx(i1-2*js1,
     & i2-2*js2,i3-2*js3))))/(2.*dsa**3)
                    a12sss = ((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axis,1)
     & /(rx(i1+2*js1,i2+2*js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,i3+2*js3)
     & -ry(i1+2*js1,i2+2*js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,i3+2*js3))
     & )-2.*(rsxy(i1+js1,i2+js2,i3+js3,axis,1)/(rx(i1+js1,i2+js2,i3+
     & js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+js2,i3+js3)*sx(i1+
     & js1,i2+js2,i3+js3)))+2.*(rsxy(i1-js1,i2-js2,i3-js3,axis,1)/(rx(
     & i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-ry(i1-js1,i2-
     & js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3)))-(rsxy(i1-2*js1,i2-2*js2,
     & i3-2*js3,axis,1)/(rx(i1-2*js1,i2-2*js2,i3-2*js3)*sy(i1-2*js1,
     & i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,i3-2*js3)*sx(i1-2*js1,
     & i2-2*js2,i3-2*js3))))/(2.*dsa**3)
                    a21sss = ((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,
     & 0)/(rx(i1+2*js1,i2+2*js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,i3+2*
     & js3)-ry(i1+2*js1,i2+2*js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,i3+2*
     & js3)))-2.*(rsxy(i1+js1,i2+js2,i3+js3,axisp1,0)/(rx(i1+js1,i2+
     & js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+js2,i3+js3)*
     & sx(i1+js1,i2+js2,i3+js3)))+2.*(rsxy(i1-js1,i2-js2,i3-js3,
     & axisp1,0)/(rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-
     & ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3)))-(rsxy(i1-2*
     & js1,i2-2*js2,i3-2*js3,axisp1,0)/(rx(i1-2*js1,i2-2*js2,i3-2*js3)
     & *sy(i1-2*js1,i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,i3-2*js3)*
     & sx(i1-2*js1,i2-2*js2,i3-2*js3))))/(2.*dsa**3)
                    a22sss = ((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,
     & 1)/(rx(i1+2*js1,i2+2*js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,i3+2*
     & js3)-ry(i1+2*js1,i2+2*js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,i3+2*
     & js3)))-2.*(rsxy(i1+js1,i2+js2,i3+js3,axisp1,1)/(rx(i1+js1,i2+
     & js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+js2,i3+js3)*
     & sx(i1+js1,i2+js2,i3+js3)))+2.*(rsxy(i1-js1,i2-js2,i3-js3,
     & axisp1,1)/(rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-
     & ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3)))-(rsxy(i1-2*
     & js1,i2-2*js2,i3-2*js3,axisp1,1)/(rx(i1-2*js1,i2-2*js2,i3-2*js3)
     & *sy(i1-2*js1,i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,i3-2*js3)*
     & sx(i1-2*js1,i2-2*js2,i3-2*js3))))/(2.*dsa**3)
                  else
                    ! not enough ghost points for the periodic or interp case for: (since we solve at i1=0)
                    a11sss = (8*(rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axis,
     & 0)/(rx(i1+2*js1,i2+2*js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,i3+2*
     & js3)-ry(i1+2*js1,i2+2*js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,i3+2*
     & js3)))-13*(rsxy(i1+js1,i2+js2,i3+js3,axis,0)/(rx(i1+js1,i2+js2,
     & i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+js2,i3+js3)*sx(
     & i1+js1,i2+js2,i3+js3)))+13*(rsxy(i1-js1,i2-js2,i3-js3,axis,0)/(
     & rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-ry(i1-js1,i2-
     & js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3)))-8*(rsxy(i1-2*js1,i2-2*
     & js2,i3-2*js3,axis,0)/(rx(i1-2*js1,i2-2*js2,i3-2*js3)*sy(i1-2*
     & js1,i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,i3-2*js3)*sx(i1-2*
     & js1,i2-2*js2,i3-2*js3)))-(rsxy(i1+3*js1,i2+3*js2,i3+3*js3,axis,
     & 0)/(rx(i1+3*js1,i2+3*js2,i3+3*js3)*sy(i1+3*js1,i2+3*js2,i3+3*
     & js3)-ry(i1+3*js1,i2+3*js2,i3+3*js3)*sx(i1+3*js1,i2+3*js2,i3+3*
     & js3)))+(rsxy(i1-3*js1,i2-3*js2,i3-3*js3,axis,0)/(rx(i1-3*js1,
     & i2-3*js2,i3-3*js3)*sy(i1-3*js1,i2-3*js2,i3-3*js3)-ry(i1-3*js1,
     & i2-3*js2,i3-3*js3)*sx(i1-3*js1,i2-3*js2,i3-3*js3))))/(8.*dsa**
     & 3)
                    a12sss = (8*(rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axis,
     & 1)/(rx(i1+2*js1,i2+2*js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,i3+2*
     & js3)-ry(i1+2*js1,i2+2*js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,i3+2*
     & js3)))-13*(rsxy(i1+js1,i2+js2,i3+js3,axis,1)/(rx(i1+js1,i2+js2,
     & i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+js2,i3+js3)*sx(
     & i1+js1,i2+js2,i3+js3)))+13*(rsxy(i1-js1,i2-js2,i3-js3,axis,1)/(
     & rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-ry(i1-js1,i2-
     & js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3)))-8*(rsxy(i1-2*js1,i2-2*
     & js2,i3-2*js3,axis,1)/(rx(i1-2*js1,i2-2*js2,i3-2*js3)*sy(i1-2*
     & js1,i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,i3-2*js3)*sx(i1-2*
     & js1,i2-2*js2,i3-2*js3)))-(rsxy(i1+3*js1,i2+3*js2,i3+3*js3,axis,
     & 1)/(rx(i1+3*js1,i2+3*js2,i3+3*js3)*sy(i1+3*js1,i2+3*js2,i3+3*
     & js3)-ry(i1+3*js1,i2+3*js2,i3+3*js3)*sx(i1+3*js1,i2+3*js2,i3+3*
     & js3)))+(rsxy(i1-3*js1,i2-3*js2,i3-3*js3,axis,1)/(rx(i1-3*js1,
     & i2-3*js2,i3-3*js3)*sy(i1-3*js1,i2-3*js2,i3-3*js3)-ry(i1-3*js1,
     & i2-3*js2,i3-3*js3)*sx(i1-3*js1,i2-3*js2,i3-3*js3))))/(8.*dsa**
     & 3)
                    a21sss = (8*(rsxy(i1+2*js1,i2+2*js2,i3+2*js3,
     & axisp1,0)/(rx(i1+2*js1,i2+2*js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,
     & i3+2*js3)-ry(i1+2*js1,i2+2*js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,
     & i3+2*js3)))-13*(rsxy(i1+js1,i2+js2,i3+js3,axisp1,0)/(rx(i1+js1,
     & i2+js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+js2,i3+
     & js3)*sx(i1+js1,i2+js2,i3+js3)))+13*(rsxy(i1-js1,i2-js2,i3-js3,
     & axisp1,0)/(rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-
     & ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3)))-8*(rsxy(i1-
     & 2*js1,i2-2*js2,i3-2*js3,axisp1,0)/(rx(i1-2*js1,i2-2*js2,i3-2*
     & js3)*sy(i1-2*js1,i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,i3-2*
     & js3)*sx(i1-2*js1,i2-2*js2,i3-2*js3)))-(rsxy(i1+3*js1,i2+3*js2,
     & i3+3*js3,axisp1,0)/(rx(i1+3*js1,i2+3*js2,i3+3*js3)*sy(i1+3*js1,
     & i2+3*js2,i3+3*js3)-ry(i1+3*js1,i2+3*js2,i3+3*js3)*sx(i1+3*js1,
     & i2+3*js2,i3+3*js3)))+(rsxy(i1-3*js1,i2-3*js2,i3-3*js3,axisp1,0)
     & /(rx(i1-3*js1,i2-3*js2,i3-3*js3)*sy(i1-3*js1,i2-3*js2,i3-3*js3)
     & -ry(i1-3*js1,i2-3*js2,i3-3*js3)*sx(i1-3*js1,i2-3*js2,i3-3*js3))
     & ))/(8.*dsa**3)
                    a22sss = (8*(rsxy(i1+2*js1,i2+2*js2,i3+2*js3,
     & axisp1,1)/(rx(i1+2*js1,i2+2*js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,
     & i3+2*js3)-ry(i1+2*js1,i2+2*js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,
     & i3+2*js3)))-13*(rsxy(i1+js1,i2+js2,i3+js3,axisp1,1)/(rx(i1+js1,
     & i2+js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+js2,i3+
     & js3)*sx(i1+js1,i2+js2,i3+js3)))+13*(rsxy(i1-js1,i2-js2,i3-js3,
     & axisp1,1)/(rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-
     & ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3)))-8*(rsxy(i1-
     & 2*js1,i2-2*js2,i3-2*js3,axisp1,1)/(rx(i1-2*js1,i2-2*js2,i3-2*
     & js3)*sy(i1-2*js1,i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,i3-2*
     & js3)*sx(i1-2*js1,i2-2*js2,i3-2*js3)))-(rsxy(i1+3*js1,i2+3*js2,
     & i3+3*js3,axisp1,1)/(rx(i1+3*js1,i2+3*js2,i3+3*js3)*sy(i1+3*js1,
     & i2+3*js2,i3+3*js3)-ry(i1+3*js1,i2+3*js2,i3+3*js3)*sx(i1+3*js1,
     & i2+3*js2,i3+3*js3)))+(rsxy(i1-3*js1,i2-3*js2,i3-3*js3,axisp1,1)
     & /(rx(i1-3*js1,i2-3*js2,i3-3*js3)*sy(i1-3*js1,i2-3*js2,i3-3*js3)
     & -ry(i1-3*js1,i2-3*js2,i3-3*js3)*sx(i1-3*js1,i2-3*js2,i3-3*js3))
     & ))/(8.*dsa**3)
                  end if
                  if( axis.eq.0 )then
                    a11rss = (128*(rsxy(i1+1,i2+1,i3,axis,0)/(rx(i1+1,
     & i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+1,i3)))-
     & 128*(rsxy(i1-1,i2+1,i3,axis,0)/(rx(i1-1,i2+1,i3)*sy(i1-1,i2+1,
     & i3)-ry(i1-1,i2+1,i3)*sx(i1-1,i2+1,i3)))-16*(rsxy(i1+2,i2+1,i3,
     & axis,0)/(rx(i1+2,i2+1,i3)*sy(i1+2,i2+1,i3)-ry(i1+2,i2+1,i3)*sx(
     & i1+2,i2+1,i3)))+16*(rsxy(i1-2,i2+1,i3,axis,0)/(rx(i1-2,i2+1,i3)
     & *sy(i1-2,i2+1,i3)-ry(i1-2,i2+1,i3)*sx(i1-2,i2+1,i3)))+128*(
     & rsxy(i1+1,i2-1,i3,axis,0)/(rx(i1+1,i2-1,i3)*sy(i1+1,i2-1,i3)-
     & ry(i1+1,i2-1,i3)*sx(i1+1,i2-1,i3)))-128*(rsxy(i1-1,i2-1,i3,
     & axis,0)/(rx(i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-ry(i1-1,i2-1,i3)*sx(
     & i1-1,i2-1,i3)))-16*(rsxy(i1+2,i2-1,i3,axis,0)/(rx(i1+2,i2-1,i3)
     & *sy(i1+2,i2-1,i3)-ry(i1+2,i2-1,i3)*sx(i1+2,i2-1,i3)))+16*(rsxy(
     & i1-2,i2-1,i3,axis,0)/(rx(i1-2,i2-1,i3)*sy(i1-2,i2-1,i3)-ry(i1-
     & 2,i2-1,i3)*sx(i1-2,i2-1,i3)))+30*(rsxy(i1+2,i2,i3,axis,0)/(rx(
     & i1+2,i2,i3)*sy(i1+2,i2,i3)-ry(i1+2,i2,i3)*sx(i1+2,i2,i3)))+8*(
     & rsxy(i1-1,i2+2,i3,axis,0)/(rx(i1-1,i2+2,i3)*sy(i1-1,i2+2,i3)-
     & ry(i1-1,i2+2,i3)*sx(i1-1,i2+2,i3)))+(rsxy(i1+2,i2+2,i3,axis,0)
     & /(rx(i1+2,i2+2,i3)*sy(i1+2,i2+2,i3)-ry(i1+2,i2+2,i3)*sx(i1+2,
     & i2+2,i3)))-(rsxy(i1-2,i2+2,i3,axis,0)/(rx(i1-2,i2+2,i3)*sy(i1-
     & 2,i2+2,i3)-ry(i1-2,i2+2,i3)*sx(i1-2,i2+2,i3)))-8*(rsxy(i1+1,i2-
     & 2,i3,axis,0)/(rx(i1+1,i2-2,i3)*sy(i1+1,i2-2,i3)-ry(i1+1,i2-2,
     & i3)*sx(i1+1,i2-2,i3)))+8*(rsxy(i1-1,i2-2,i3,axis,0)/(rx(i1-1,
     & i2-2,i3)*sy(i1-1,i2-2,i3)-ry(i1-1,i2-2,i3)*sx(i1-1,i2-2,i3)))+(
     & rsxy(i1+2,i2-2,i3,axis,0)/(rx(i1+2,i2-2,i3)*sy(i1+2,i2-2,i3)-
     & ry(i1+2,i2-2,i3)*sx(i1+2,i2-2,i3)))-(rsxy(i1-2,i2-2,i3,axis,0)
     & /(rx(i1-2,i2-2,i3)*sy(i1-2,i2-2,i3)-ry(i1-2,i2-2,i3)*sx(i1-2,
     & i2-2,i3)))-8*(rsxy(i1+1,i2+2,i3,axis,0)/(rx(i1+1,i2+2,i3)*sy(
     & i1+1,i2+2,i3)-ry(i1+1,i2+2,i3)*sx(i1+1,i2+2,i3)))-30*(rsxy(i1-
     & 2,i2,i3,axis,0)/(rx(i1-2,i2,i3)*sy(i1-2,i2,i3)-ry(i1-2,i2,i3)*
     & sx(i1-2,i2,i3)))-240*(rsxy(i1+1,i2,i3,axis,0)/(rx(i1+1,i2,i3)*
     & sy(i1+1,i2,i3)-ry(i1+1,i2,i3)*sx(i1+1,i2,i3)))+240*(rsxy(i1-1,
     & i2,i3,axis,0)/(rx(i1-1,i2,i3)*sy(i1-1,i2,i3)-ry(i1-1,i2,i3)*sx(
     & i1-1,i2,i3))))/(144.*dr(1)**2*dr(0))
                    a12rss = (128*(rsxy(i1+1,i2+1,i3,axis,1)/(rx(i1+1,
     & i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+1,i3)))-
     & 128*(rsxy(i1-1,i2+1,i3,axis,1)/(rx(i1-1,i2+1,i3)*sy(i1-1,i2+1,
     & i3)-ry(i1-1,i2+1,i3)*sx(i1-1,i2+1,i3)))-16*(rsxy(i1+2,i2+1,i3,
     & axis,1)/(rx(i1+2,i2+1,i3)*sy(i1+2,i2+1,i3)-ry(i1+2,i2+1,i3)*sx(
     & i1+2,i2+1,i3)))+16*(rsxy(i1-2,i2+1,i3,axis,1)/(rx(i1-2,i2+1,i3)
     & *sy(i1-2,i2+1,i3)-ry(i1-2,i2+1,i3)*sx(i1-2,i2+1,i3)))+128*(
     & rsxy(i1+1,i2-1,i3,axis,1)/(rx(i1+1,i2-1,i3)*sy(i1+1,i2-1,i3)-
     & ry(i1+1,i2-1,i3)*sx(i1+1,i2-1,i3)))-128*(rsxy(i1-1,i2-1,i3,
     & axis,1)/(rx(i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-ry(i1-1,i2-1,i3)*sx(
     & i1-1,i2-1,i3)))-16*(rsxy(i1+2,i2-1,i3,axis,1)/(rx(i1+2,i2-1,i3)
     & *sy(i1+2,i2-1,i3)-ry(i1+2,i2-1,i3)*sx(i1+2,i2-1,i3)))+16*(rsxy(
     & i1-2,i2-1,i3,axis,1)/(rx(i1-2,i2-1,i3)*sy(i1-2,i2-1,i3)-ry(i1-
     & 2,i2-1,i3)*sx(i1-2,i2-1,i3)))+30*(rsxy(i1+2,i2,i3,axis,1)/(rx(
     & i1+2,i2,i3)*sy(i1+2,i2,i3)-ry(i1+2,i2,i3)*sx(i1+2,i2,i3)))+8*(
     & rsxy(i1-1,i2+2,i3,axis,1)/(rx(i1-1,i2+2,i3)*sy(i1-1,i2+2,i3)-
     & ry(i1-1,i2+2,i3)*sx(i1-1,i2+2,i3)))+(rsxy(i1+2,i2+2,i3,axis,1)
     & /(rx(i1+2,i2+2,i3)*sy(i1+2,i2+2,i3)-ry(i1+2,i2+2,i3)*sx(i1+2,
     & i2+2,i3)))-(rsxy(i1-2,i2+2,i3,axis,1)/(rx(i1-2,i2+2,i3)*sy(i1-
     & 2,i2+2,i3)-ry(i1-2,i2+2,i3)*sx(i1-2,i2+2,i3)))-8*(rsxy(i1+1,i2-
     & 2,i3,axis,1)/(rx(i1+1,i2-2,i3)*sy(i1+1,i2-2,i3)-ry(i1+1,i2-2,
     & i3)*sx(i1+1,i2-2,i3)))+8*(rsxy(i1-1,i2-2,i3,axis,1)/(rx(i1-1,
     & i2-2,i3)*sy(i1-1,i2-2,i3)-ry(i1-1,i2-2,i3)*sx(i1-1,i2-2,i3)))+(
     & rsxy(i1+2,i2-2,i3,axis,1)/(rx(i1+2,i2-2,i3)*sy(i1+2,i2-2,i3)-
     & ry(i1+2,i2-2,i3)*sx(i1+2,i2-2,i3)))-(rsxy(i1-2,i2-2,i3,axis,1)
     & /(rx(i1-2,i2-2,i3)*sy(i1-2,i2-2,i3)-ry(i1-2,i2-2,i3)*sx(i1-2,
     & i2-2,i3)))-8*(rsxy(i1+1,i2+2,i3,axis,1)/(rx(i1+1,i2+2,i3)*sy(
     & i1+1,i2+2,i3)-ry(i1+1,i2+2,i3)*sx(i1+1,i2+2,i3)))-30*(rsxy(i1-
     & 2,i2,i3,axis,1)/(rx(i1-2,i2,i3)*sy(i1-2,i2,i3)-ry(i1-2,i2,i3)*
     & sx(i1-2,i2,i3)))-240*(rsxy(i1+1,i2,i3,axis,1)/(rx(i1+1,i2,i3)*
     & sy(i1+1,i2,i3)-ry(i1+1,i2,i3)*sx(i1+1,i2,i3)))+240*(rsxy(i1-1,
     & i2,i3,axis,1)/(rx(i1-1,i2,i3)*sy(i1-1,i2,i3)-ry(i1-1,i2,i3)*sx(
     & i1-1,i2,i3))))/(144.*dr(1)**2*dr(0))
                    a21rss = (128*(rsxy(i1+1,i2+1,i3,axisp1,0)/(rx(i1+
     & 1,i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+1,i3)))
     & -128*(rsxy(i1-1,i2+1,i3,axisp1,0)/(rx(i1-1,i2+1,i3)*sy(i1-1,i2+
     & 1,i3)-ry(i1-1,i2+1,i3)*sx(i1-1,i2+1,i3)))-16*(rsxy(i1+2,i2+1,
     & i3,axisp1,0)/(rx(i1+2,i2+1,i3)*sy(i1+2,i2+1,i3)-ry(i1+2,i2+1,
     & i3)*sx(i1+2,i2+1,i3)))+16*(rsxy(i1-2,i2+1,i3,axisp1,0)/(rx(i1-
     & 2,i2+1,i3)*sy(i1-2,i2+1,i3)-ry(i1-2,i2+1,i3)*sx(i1-2,i2+1,i3)))
     & +128*(rsxy(i1+1,i2-1,i3,axisp1,0)/(rx(i1+1,i2-1,i3)*sy(i1+1,i2-
     & 1,i3)-ry(i1+1,i2-1,i3)*sx(i1+1,i2-1,i3)))-128*(rsxy(i1-1,i2-1,
     & i3,axisp1,0)/(rx(i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-ry(i1-1,i2-1,
     & i3)*sx(i1-1,i2-1,i3)))-16*(rsxy(i1+2,i2-1,i3,axisp1,0)/(rx(i1+
     & 2,i2-1,i3)*sy(i1+2,i2-1,i3)-ry(i1+2,i2-1,i3)*sx(i1+2,i2-1,i3)))
     & +16*(rsxy(i1-2,i2-1,i3,axisp1,0)/(rx(i1-2,i2-1,i3)*sy(i1-2,i2-
     & 1,i3)-ry(i1-2,i2-1,i3)*sx(i1-2,i2-1,i3)))+30*(rsxy(i1+2,i2,i3,
     & axisp1,0)/(rx(i1+2,i2,i3)*sy(i1+2,i2,i3)-ry(i1+2,i2,i3)*sx(i1+
     & 2,i2,i3)))+8*(rsxy(i1-1,i2+2,i3,axisp1,0)/(rx(i1-1,i2+2,i3)*sy(
     & i1-1,i2+2,i3)-ry(i1-1,i2+2,i3)*sx(i1-1,i2+2,i3)))+(rsxy(i1+2,
     & i2+2,i3,axisp1,0)/(rx(i1+2,i2+2,i3)*sy(i1+2,i2+2,i3)-ry(i1+2,
     & i2+2,i3)*sx(i1+2,i2+2,i3)))-(rsxy(i1-2,i2+2,i3,axisp1,0)/(rx(
     & i1-2,i2+2,i3)*sy(i1-2,i2+2,i3)-ry(i1-2,i2+2,i3)*sx(i1-2,i2+2,
     & i3)))-8*(rsxy(i1+1,i2-2,i3,axisp1,0)/(rx(i1+1,i2-2,i3)*sy(i1+1,
     & i2-2,i3)-ry(i1+1,i2-2,i3)*sx(i1+1,i2-2,i3)))+8*(rsxy(i1-1,i2-2,
     & i3,axisp1,0)/(rx(i1-1,i2-2,i3)*sy(i1-1,i2-2,i3)-ry(i1-1,i2-2,
     & i3)*sx(i1-1,i2-2,i3)))+(rsxy(i1+2,i2-2,i3,axisp1,0)/(rx(i1+2,
     & i2-2,i3)*sy(i1+2,i2-2,i3)-ry(i1+2,i2-2,i3)*sx(i1+2,i2-2,i3)))-(
     & rsxy(i1-2,i2-2,i3,axisp1,0)/(rx(i1-2,i2-2,i3)*sy(i1-2,i2-2,i3)-
     & ry(i1-2,i2-2,i3)*sx(i1-2,i2-2,i3)))-8*(rsxy(i1+1,i2+2,i3,
     & axisp1,0)/(rx(i1+1,i2+2,i3)*sy(i1+1,i2+2,i3)-ry(i1+1,i2+2,i3)*
     & sx(i1+1,i2+2,i3)))-30*(rsxy(i1-2,i2,i3,axisp1,0)/(rx(i1-2,i2,
     & i3)*sy(i1-2,i2,i3)-ry(i1-2,i2,i3)*sx(i1-2,i2,i3)))-240*(rsxy(
     & i1+1,i2,i3,axisp1,0)/(rx(i1+1,i2,i3)*sy(i1+1,i2,i3)-ry(i1+1,i2,
     & i3)*sx(i1+1,i2,i3)))+240*(rsxy(i1-1,i2,i3,axisp1,0)/(rx(i1-1,
     & i2,i3)*sy(i1-1,i2,i3)-ry(i1-1,i2,i3)*sx(i1-1,i2,i3))))/(144.*
     & dr(1)**2*dr(0))
                    a22rss = (128*(rsxy(i1+1,i2+1,i3,axisp1,1)/(rx(i1+
     & 1,i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+1,i3)))
     & -128*(rsxy(i1-1,i2+1,i3,axisp1,1)/(rx(i1-1,i2+1,i3)*sy(i1-1,i2+
     & 1,i3)-ry(i1-1,i2+1,i3)*sx(i1-1,i2+1,i3)))-16*(rsxy(i1+2,i2+1,
     & i3,axisp1,1)/(rx(i1+2,i2+1,i3)*sy(i1+2,i2+1,i3)-ry(i1+2,i2+1,
     & i3)*sx(i1+2,i2+1,i3)))+16*(rsxy(i1-2,i2+1,i3,axisp1,1)/(rx(i1-
     & 2,i2+1,i3)*sy(i1-2,i2+1,i3)-ry(i1-2,i2+1,i3)*sx(i1-2,i2+1,i3)))
     & +128*(rsxy(i1+1,i2-1,i3,axisp1,1)/(rx(i1+1,i2-1,i3)*sy(i1+1,i2-
     & 1,i3)-ry(i1+1,i2-1,i3)*sx(i1+1,i2-1,i3)))-128*(rsxy(i1-1,i2-1,
     & i3,axisp1,1)/(rx(i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-ry(i1-1,i2-1,
     & i3)*sx(i1-1,i2-1,i3)))-16*(rsxy(i1+2,i2-1,i3,axisp1,1)/(rx(i1+
     & 2,i2-1,i3)*sy(i1+2,i2-1,i3)-ry(i1+2,i2-1,i3)*sx(i1+2,i2-1,i3)))
     & +16*(rsxy(i1-2,i2-1,i3,axisp1,1)/(rx(i1-2,i2-1,i3)*sy(i1-2,i2-
     & 1,i3)-ry(i1-2,i2-1,i3)*sx(i1-2,i2-1,i3)))+30*(rsxy(i1+2,i2,i3,
     & axisp1,1)/(rx(i1+2,i2,i3)*sy(i1+2,i2,i3)-ry(i1+2,i2,i3)*sx(i1+
     & 2,i2,i3)))+8*(rsxy(i1-1,i2+2,i3,axisp1,1)/(rx(i1-1,i2+2,i3)*sy(
     & i1-1,i2+2,i3)-ry(i1-1,i2+2,i3)*sx(i1-1,i2+2,i3)))+(rsxy(i1+2,
     & i2+2,i3,axisp1,1)/(rx(i1+2,i2+2,i3)*sy(i1+2,i2+2,i3)-ry(i1+2,
     & i2+2,i3)*sx(i1+2,i2+2,i3)))-(rsxy(i1-2,i2+2,i3,axisp1,1)/(rx(
     & i1-2,i2+2,i3)*sy(i1-2,i2+2,i3)-ry(i1-2,i2+2,i3)*sx(i1-2,i2+2,
     & i3)))-8*(rsxy(i1+1,i2-2,i3,axisp1,1)/(rx(i1+1,i2-2,i3)*sy(i1+1,
     & i2-2,i3)-ry(i1+1,i2-2,i3)*sx(i1+1,i2-2,i3)))+8*(rsxy(i1-1,i2-2,
     & i3,axisp1,1)/(rx(i1-1,i2-2,i3)*sy(i1-1,i2-2,i3)-ry(i1-1,i2-2,
     & i3)*sx(i1-1,i2-2,i3)))+(rsxy(i1+2,i2-2,i3,axisp1,1)/(rx(i1+2,
     & i2-2,i3)*sy(i1+2,i2-2,i3)-ry(i1+2,i2-2,i3)*sx(i1+2,i2-2,i3)))-(
     & rsxy(i1-2,i2-2,i3,axisp1,1)/(rx(i1-2,i2-2,i3)*sy(i1-2,i2-2,i3)-
     & ry(i1-2,i2-2,i3)*sx(i1-2,i2-2,i3)))-8*(rsxy(i1+1,i2+2,i3,
     & axisp1,1)/(rx(i1+1,i2+2,i3)*sy(i1+1,i2+2,i3)-ry(i1+1,i2+2,i3)*
     & sx(i1+1,i2+2,i3)))-30*(rsxy(i1-2,i2,i3,axisp1,1)/(rx(i1-2,i2,
     & i3)*sy(i1-2,i2,i3)-ry(i1-2,i2,i3)*sx(i1-2,i2,i3)))-240*(rsxy(
     & i1+1,i2,i3,axisp1,1)/(rx(i1+1,i2,i3)*sy(i1+1,i2,i3)-ry(i1+1,i2,
     & i3)*sx(i1+1,i2,i3)))+240*(rsxy(i1-1,i2,i3,axisp1,1)/(rx(i1-1,
     & i2,i3)*sy(i1-1,i2,i3)-ry(i1-1,i2,i3)*sx(i1-1,i2,i3))))/(144.*
     & dr(1)**2*dr(0))
                  else
                    a11rss = (128*(rsxy(i1+1,i2+1,i3,axis,0)/(rx(i1+1,
     & i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+1,i3)))-
     & 240*(rsxy(i1,i2+1,i3,axis,0)/(rx(i1,i2+1,i3)*sy(i1,i2+1,i3)-ry(
     & i1,i2+1,i3)*sx(i1,i2+1,i3)))+128*(rsxy(i1-1,i2+1,i3,axis,0)/(
     & rx(i1-1,i2+1,i3)*sy(i1-1,i2+1,i3)-ry(i1-1,i2+1,i3)*sx(i1-1,i2+
     & 1,i3)))-8*(rsxy(i1+2,i2+1,i3,axis,0)/(rx(i1+2,i2+1,i3)*sy(i1+2,
     & i2+1,i3)-ry(i1+2,i2+1,i3)*sx(i1+2,i2+1,i3)))-8*(rsxy(i1-2,i2+1,
     & i3,axis,0)/(rx(i1-2,i2+1,i3)*sy(i1-2,i2+1,i3)-ry(i1-2,i2+1,i3)*
     & sx(i1-2,i2+1,i3)))-128*(rsxy(i1+1,i2-1,i3,axis,0)/(rx(i1+1,i2-
     & 1,i3)*sy(i1+1,i2-1,i3)-ry(i1+1,i2-1,i3)*sx(i1+1,i2-1,i3)))+240*
     & (rsxy(i1,i2-1,i3,axis,0)/(rx(i1,i2-1,i3)*sy(i1,i2-1,i3)-ry(i1,
     & i2-1,i3)*sx(i1,i2-1,i3)))-128*(rsxy(i1-1,i2-1,i3,axis,0)/(rx(
     & i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-ry(i1-1,i2-1,i3)*sx(i1-1,i2-1,
     & i3)))+8*(rsxy(i1+2,i2-1,i3,axis,0)/(rx(i1+2,i2-1,i3)*sy(i1+2,
     & i2-1,i3)-ry(i1+2,i2-1,i3)*sx(i1+2,i2-1,i3)))+8*(rsxy(i1-2,i2-1,
     & i3,axis,0)/(rx(i1-2,i2-1,i3)*sy(i1-2,i2-1,i3)-ry(i1-2,i2-1,i3)*
     & sx(i1-2,i2-1,i3)))+30*(rsxy(i1,i2+2,i3,axis,0)/(rx(i1,i2+2,i3)*
     & sy(i1,i2+2,i3)-ry(i1,i2+2,i3)*sx(i1,i2+2,i3)))-16*(rsxy(i1-1,
     & i2+2,i3,axis,0)/(rx(i1-1,i2+2,i3)*sy(i1-1,i2+2,i3)-ry(i1-1,i2+
     & 2,i3)*sx(i1-1,i2+2,i3)))+(rsxy(i1+2,i2+2,i3,axis,0)/(rx(i1+2,
     & i2+2,i3)*sy(i1+2,i2+2,i3)-ry(i1+2,i2+2,i3)*sx(i1+2,i2+2,i3)))+(
     & rsxy(i1-2,i2+2,i3,axis,0)/(rx(i1-2,i2+2,i3)*sy(i1-2,i2+2,i3)-
     & ry(i1-2,i2+2,i3)*sx(i1-2,i2+2,i3)))+16*(rsxy(i1+1,i2-2,i3,axis,
     & 0)/(rx(i1+1,i2-2,i3)*sy(i1+1,i2-2,i3)-ry(i1+1,i2-2,i3)*sx(i1+1,
     & i2-2,i3)))-30*(rsxy(i1,i2-2,i3,axis,0)/(rx(i1,i2-2,i3)*sy(i1,
     & i2-2,i3)-ry(i1,i2-2,i3)*sx(i1,i2-2,i3)))+16*(rsxy(i1-1,i2-2,i3,
     & axis,0)/(rx(i1-1,i2-2,i3)*sy(i1-1,i2-2,i3)-ry(i1-1,i2-2,i3)*sx(
     & i1-1,i2-2,i3)))-(rsxy(i1+2,i2-2,i3,axis,0)/(rx(i1+2,i2-2,i3)*
     & sy(i1+2,i2-2,i3)-ry(i1+2,i2-2,i3)*sx(i1+2,i2-2,i3)))-(rsxy(i1-
     & 2,i2-2,i3,axis,0)/(rx(i1-2,i2-2,i3)*sy(i1-2,i2-2,i3)-ry(i1-2,
     & i2-2,i3)*sx(i1-2,i2-2,i3)))-16*(rsxy(i1+1,i2+2,i3,axis,0)/(rx(
     & i1+1,i2+2,i3)*sy(i1+1,i2+2,i3)-ry(i1+1,i2+2,i3)*sx(i1+1,i2+2,
     & i3))))/(144.*dr(0)**2*dr(1))
                    a12rss = (128*(rsxy(i1+1,i2+1,i3,axis,1)/(rx(i1+1,
     & i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+1,i3)))-
     & 240*(rsxy(i1,i2+1,i3,axis,1)/(rx(i1,i2+1,i3)*sy(i1,i2+1,i3)-ry(
     & i1,i2+1,i3)*sx(i1,i2+1,i3)))+128*(rsxy(i1-1,i2+1,i3,axis,1)/(
     & rx(i1-1,i2+1,i3)*sy(i1-1,i2+1,i3)-ry(i1-1,i2+1,i3)*sx(i1-1,i2+
     & 1,i3)))-8*(rsxy(i1+2,i2+1,i3,axis,1)/(rx(i1+2,i2+1,i3)*sy(i1+2,
     & i2+1,i3)-ry(i1+2,i2+1,i3)*sx(i1+2,i2+1,i3)))-8*(rsxy(i1-2,i2+1,
     & i3,axis,1)/(rx(i1-2,i2+1,i3)*sy(i1-2,i2+1,i3)-ry(i1-2,i2+1,i3)*
     & sx(i1-2,i2+1,i3)))-128*(rsxy(i1+1,i2-1,i3,axis,1)/(rx(i1+1,i2-
     & 1,i3)*sy(i1+1,i2-1,i3)-ry(i1+1,i2-1,i3)*sx(i1+1,i2-1,i3)))+240*
     & (rsxy(i1,i2-1,i3,axis,1)/(rx(i1,i2-1,i3)*sy(i1,i2-1,i3)-ry(i1,
     & i2-1,i3)*sx(i1,i2-1,i3)))-128*(rsxy(i1-1,i2-1,i3,axis,1)/(rx(
     & i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-ry(i1-1,i2-1,i3)*sx(i1-1,i2-1,
     & i3)))+8*(rsxy(i1+2,i2-1,i3,axis,1)/(rx(i1+2,i2-1,i3)*sy(i1+2,
     & i2-1,i3)-ry(i1+2,i2-1,i3)*sx(i1+2,i2-1,i3)))+8*(rsxy(i1-2,i2-1,
     & i3,axis,1)/(rx(i1-2,i2-1,i3)*sy(i1-2,i2-1,i3)-ry(i1-2,i2-1,i3)*
     & sx(i1-2,i2-1,i3)))+30*(rsxy(i1,i2+2,i3,axis,1)/(rx(i1,i2+2,i3)*
     & sy(i1,i2+2,i3)-ry(i1,i2+2,i3)*sx(i1,i2+2,i3)))-16*(rsxy(i1-1,
     & i2+2,i3,axis,1)/(rx(i1-1,i2+2,i3)*sy(i1-1,i2+2,i3)-ry(i1-1,i2+
     & 2,i3)*sx(i1-1,i2+2,i3)))+(rsxy(i1+2,i2+2,i3,axis,1)/(rx(i1+2,
     & i2+2,i3)*sy(i1+2,i2+2,i3)-ry(i1+2,i2+2,i3)*sx(i1+2,i2+2,i3)))+(
     & rsxy(i1-2,i2+2,i3,axis,1)/(rx(i1-2,i2+2,i3)*sy(i1-2,i2+2,i3)-
     & ry(i1-2,i2+2,i3)*sx(i1-2,i2+2,i3)))+16*(rsxy(i1+1,i2-2,i3,axis,
     & 1)/(rx(i1+1,i2-2,i3)*sy(i1+1,i2-2,i3)-ry(i1+1,i2-2,i3)*sx(i1+1,
     & i2-2,i3)))-30*(rsxy(i1,i2-2,i3,axis,1)/(rx(i1,i2-2,i3)*sy(i1,
     & i2-2,i3)-ry(i1,i2-2,i3)*sx(i1,i2-2,i3)))+16*(rsxy(i1-1,i2-2,i3,
     & axis,1)/(rx(i1-1,i2-2,i3)*sy(i1-1,i2-2,i3)-ry(i1-1,i2-2,i3)*sx(
     & i1-1,i2-2,i3)))-(rsxy(i1+2,i2-2,i3,axis,1)/(rx(i1+2,i2-2,i3)*
     & sy(i1+2,i2-2,i3)-ry(i1+2,i2-2,i3)*sx(i1+2,i2-2,i3)))-(rsxy(i1-
     & 2,i2-2,i3,axis,1)/(rx(i1-2,i2-2,i3)*sy(i1-2,i2-2,i3)-ry(i1-2,
     & i2-2,i3)*sx(i1-2,i2-2,i3)))-16*(rsxy(i1+1,i2+2,i3,axis,1)/(rx(
     & i1+1,i2+2,i3)*sy(i1+1,i2+2,i3)-ry(i1+1,i2+2,i3)*sx(i1+1,i2+2,
     & i3))))/(144.*dr(0)**2*dr(1))
                    a21rss = (128*(rsxy(i1+1,i2+1,i3,axisp1,0)/(rx(i1+
     & 1,i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+1,i3)))
     & -240*(rsxy(i1,i2+1,i3,axisp1,0)/(rx(i1,i2+1,i3)*sy(i1,i2+1,i3)-
     & ry(i1,i2+1,i3)*sx(i1,i2+1,i3)))+128*(rsxy(i1-1,i2+1,i3,axisp1,
     & 0)/(rx(i1-1,i2+1,i3)*sy(i1-1,i2+1,i3)-ry(i1-1,i2+1,i3)*sx(i1-1,
     & i2+1,i3)))-8*(rsxy(i1+2,i2+1,i3,axisp1,0)/(rx(i1+2,i2+1,i3)*sy(
     & i1+2,i2+1,i3)-ry(i1+2,i2+1,i3)*sx(i1+2,i2+1,i3)))-8*(rsxy(i1-2,
     & i2+1,i3,axisp1,0)/(rx(i1-2,i2+1,i3)*sy(i1-2,i2+1,i3)-ry(i1-2,
     & i2+1,i3)*sx(i1-2,i2+1,i3)))-128*(rsxy(i1+1,i2-1,i3,axisp1,0)/(
     & rx(i1+1,i2-1,i3)*sy(i1+1,i2-1,i3)-ry(i1+1,i2-1,i3)*sx(i1+1,i2-
     & 1,i3)))+240*(rsxy(i1,i2-1,i3,axisp1,0)/(rx(i1,i2-1,i3)*sy(i1,
     & i2-1,i3)-ry(i1,i2-1,i3)*sx(i1,i2-1,i3)))-128*(rsxy(i1-1,i2-1,
     & i3,axisp1,0)/(rx(i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-ry(i1-1,i2-1,
     & i3)*sx(i1-1,i2-1,i3)))+8*(rsxy(i1+2,i2-1,i3,axisp1,0)/(rx(i1+2,
     & i2-1,i3)*sy(i1+2,i2-1,i3)-ry(i1+2,i2-1,i3)*sx(i1+2,i2-1,i3)))+
     & 8*(rsxy(i1-2,i2-1,i3,axisp1,0)/(rx(i1-2,i2-1,i3)*sy(i1-2,i2-1,
     & i3)-ry(i1-2,i2-1,i3)*sx(i1-2,i2-1,i3)))+30*(rsxy(i1,i2+2,i3,
     & axisp1,0)/(rx(i1,i2+2,i3)*sy(i1,i2+2,i3)-ry(i1,i2+2,i3)*sx(i1,
     & i2+2,i3)))-16*(rsxy(i1-1,i2+2,i3,axisp1,0)/(rx(i1-1,i2+2,i3)*
     & sy(i1-1,i2+2,i3)-ry(i1-1,i2+2,i3)*sx(i1-1,i2+2,i3)))+(rsxy(i1+
     & 2,i2+2,i3,axisp1,0)/(rx(i1+2,i2+2,i3)*sy(i1+2,i2+2,i3)-ry(i1+2,
     & i2+2,i3)*sx(i1+2,i2+2,i3)))+(rsxy(i1-2,i2+2,i3,axisp1,0)/(rx(
     & i1-2,i2+2,i3)*sy(i1-2,i2+2,i3)-ry(i1-2,i2+2,i3)*sx(i1-2,i2+2,
     & i3)))+16*(rsxy(i1+1,i2-2,i3,axisp1,0)/(rx(i1+1,i2-2,i3)*sy(i1+
     & 1,i2-2,i3)-ry(i1+1,i2-2,i3)*sx(i1+1,i2-2,i3)))-30*(rsxy(i1,i2-
     & 2,i3,axisp1,0)/(rx(i1,i2-2,i3)*sy(i1,i2-2,i3)-ry(i1,i2-2,i3)*
     & sx(i1,i2-2,i3)))+16*(rsxy(i1-1,i2-2,i3,axisp1,0)/(rx(i1-1,i2-2,
     & i3)*sy(i1-1,i2-2,i3)-ry(i1-1,i2-2,i3)*sx(i1-1,i2-2,i3)))-(rsxy(
     & i1+2,i2-2,i3,axisp1,0)/(rx(i1+2,i2-2,i3)*sy(i1+2,i2-2,i3)-ry(
     & i1+2,i2-2,i3)*sx(i1+2,i2-2,i3)))-(rsxy(i1-2,i2-2,i3,axisp1,0)/(
     & rx(i1-2,i2-2,i3)*sy(i1-2,i2-2,i3)-ry(i1-2,i2-2,i3)*sx(i1-2,i2-
     & 2,i3)))-16*(rsxy(i1+1,i2+2,i3,axisp1,0)/(rx(i1+1,i2+2,i3)*sy(
     & i1+1,i2+2,i3)-ry(i1+1,i2+2,i3)*sx(i1+1,i2+2,i3))))/(144.*dr(0)*
     & *2*dr(1))
                    a22rss = (128*(rsxy(i1+1,i2+1,i3,axisp1,1)/(rx(i1+
     & 1,i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+1,i3)))
     & -240*(rsxy(i1,i2+1,i3,axisp1,1)/(rx(i1,i2+1,i3)*sy(i1,i2+1,i3)-
     & ry(i1,i2+1,i3)*sx(i1,i2+1,i3)))+128*(rsxy(i1-1,i2+1,i3,axisp1,
     & 1)/(rx(i1-1,i2+1,i3)*sy(i1-1,i2+1,i3)-ry(i1-1,i2+1,i3)*sx(i1-1,
     & i2+1,i3)))-8*(rsxy(i1+2,i2+1,i3,axisp1,1)/(rx(i1+2,i2+1,i3)*sy(
     & i1+2,i2+1,i3)-ry(i1+2,i2+1,i3)*sx(i1+2,i2+1,i3)))-8*(rsxy(i1-2,
     & i2+1,i3,axisp1,1)/(rx(i1-2,i2+1,i3)*sy(i1-2,i2+1,i3)-ry(i1-2,
     & i2+1,i3)*sx(i1-2,i2+1,i3)))-128*(rsxy(i1+1,i2-1,i3,axisp1,1)/(
     & rx(i1+1,i2-1,i3)*sy(i1+1,i2-1,i3)-ry(i1+1,i2-1,i3)*sx(i1+1,i2-
     & 1,i3)))+240*(rsxy(i1,i2-1,i3,axisp1,1)/(rx(i1,i2-1,i3)*sy(i1,
     & i2-1,i3)-ry(i1,i2-1,i3)*sx(i1,i2-1,i3)))-128*(rsxy(i1-1,i2-1,
     & i3,axisp1,1)/(rx(i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-ry(i1-1,i2-1,
     & i3)*sx(i1-1,i2-1,i3)))+8*(rsxy(i1+2,i2-1,i3,axisp1,1)/(rx(i1+2,
     & i2-1,i3)*sy(i1+2,i2-1,i3)-ry(i1+2,i2-1,i3)*sx(i1+2,i2-1,i3)))+
     & 8*(rsxy(i1-2,i2-1,i3,axisp1,1)/(rx(i1-2,i2-1,i3)*sy(i1-2,i2-1,
     & i3)-ry(i1-2,i2-1,i3)*sx(i1-2,i2-1,i3)))+30*(rsxy(i1,i2+2,i3,
     & axisp1,1)/(rx(i1,i2+2,i3)*sy(i1,i2+2,i3)-ry(i1,i2+2,i3)*sx(i1,
     & i2+2,i3)))-16*(rsxy(i1-1,i2+2,i3,axisp1,1)/(rx(i1-1,i2+2,i3)*
     & sy(i1-1,i2+2,i3)-ry(i1-1,i2+2,i3)*sx(i1-1,i2+2,i3)))+(rsxy(i1+
     & 2,i2+2,i3,axisp1,1)/(rx(i1+2,i2+2,i3)*sy(i1+2,i2+2,i3)-ry(i1+2,
     & i2+2,i3)*sx(i1+2,i2+2,i3)))+(rsxy(i1-2,i2+2,i3,axisp1,1)/(rx(
     & i1-2,i2+2,i3)*sy(i1-2,i2+2,i3)-ry(i1-2,i2+2,i3)*sx(i1-2,i2+2,
     & i3)))+16*(rsxy(i1+1,i2-2,i3,axisp1,1)/(rx(i1+1,i2-2,i3)*sy(i1+
     & 1,i2-2,i3)-ry(i1+1,i2-2,i3)*sx(i1+1,i2-2,i3)))-30*(rsxy(i1,i2-
     & 2,i3,axisp1,1)/(rx(i1,i2-2,i3)*sy(i1,i2-2,i3)-ry(i1,i2-2,i3)*
     & sx(i1,i2-2,i3)))+16*(rsxy(i1-1,i2-2,i3,axisp1,1)/(rx(i1-1,i2-2,
     & i3)*sy(i1-1,i2-2,i3)-ry(i1-1,i2-2,i3)*sx(i1-1,i2-2,i3)))-(rsxy(
     & i1+2,i2-2,i3,axisp1,1)/(rx(i1+2,i2-2,i3)*sy(i1+2,i2-2,i3)-ry(
     & i1+2,i2-2,i3)*sx(i1+2,i2-2,i3)))-(rsxy(i1-2,i2-2,i3,axisp1,1)/(
     & rx(i1-2,i2-2,i3)*sy(i1-2,i2-2,i3)-ry(i1-2,i2-2,i3)*sx(i1-2,i2-
     & 2,i3)))-16*(rsxy(i1+1,i2+2,i3,axisp1,1)/(rx(i1+1,i2+2,i3)*sy(
     & i1+1,i2+2,i3)-ry(i1+1,i2+2,i3)*sx(i1+1,i2+2,i3))))/(144.*dr(0)*
     & *2*dr(1))
                  end if
                    c11 = (rsxy(i1,i2,i3,axis,0)**2+rsxy(i1,i2,i3,axis,
     & 1)**2)
                    c22 = (rsxy(i1,i2,i3,axisp1,0)**2+rsxy(i1,i2,i3,
     & axisp1,1)**2)
                    c1 = (rsxyx42(i1,i2,i3,axis,0)+rsxyy42(i1,i2,i3,
     & axis,1))
                    c2 = (rsxyx42(i1,i2,i3,axisp1,0)+rsxyy42(i1,i2,i3,
     & axisp1,1))
                    ! *** we require only one s derivative of c11,c22,c1,c2: ****
                    ! 2nd order:
                    ! c11s = (C11(i1+js1,i2+js2,i3)-C11(i1-js1,i2-js2,i3))/(2.*dsa) 
                    ! c22s = (C22(i1+js1,i2+js2,i3)-C22(i1-js1,i2-js2,i3))/(2.*dsa) 
                    ! c1s =   (C1Order2(i1+js1,i2+js2,i3)- C1Order2(i1-js1,i2-js2,i3))/(2.*dsa) 
                    ! c2s =   (C2Order2(i1+js1,i2+js2,i3)- C2Order2(i1-js1,i2-js2,i3))/(2.*dsa) 
                    ! fourth-order:
                 !$$$   c11s = (8.*(C11(i1+  js1,i2+  js2,i3)-C11(i1-  js1,i2-  js2,i3))   !$$$             -(C11(i1+2*js1,i2+2*js2,i3)-C11(i1-2*js1,i2-2*js2,i3))   )/(12.*dsa)
                 !$$$   c22s = (8.*(C22(i1+  js1,i2+  js2,i3)-C22(i1-  js1,i2-  js2,i3))   !$$$             -(C22(i1+2*js1,i2+2*js2,i3)-C22(i1-2*js1,i2-2*js2,i3))   )/(12.*dsa)
                    c11r = (8.*((rsxy(i1+is1,i2+is2,i3,axis,0)**2+rsxy(
     & i1+is1,i2+is2,i3,axis,1)**2)-(rsxy(i1-is1,i2-is2,i3,axis,0)**2+
     & rsxy(i1-is1,i2-is2,i3,axis,1)**2))   -((rsxy(i1+2*is1,i2+2*is2,
     & i3,axis,0)**2+rsxy(i1+2*is1,i2+2*is2,i3,axis,1)**2)-(rsxy(i1-2*
     & is1,i2-2*is2,i3,axis,0)**2+rsxy(i1-2*is1,i2-2*is2,i3,axis,1)**
     & 2))   )/(12.*dra)
                    c22r = (8.*((rsxy(i1+is1,i2+is2,i3,axisp1,0)**2+
     & rsxy(i1+is1,i2+is2,i3,axisp1,1)**2)-(rsxy(i1-is1,i2-is2,i3,
     & axisp1,0)**2+rsxy(i1-is1,i2-is2,i3,axisp1,1)**2))   -((rsxy(i1+
     & 2*is1,i2+2*is2,i3,axisp1,0)**2+rsxy(i1+2*is1,i2+2*is2,i3,
     & axisp1,1)**2)-(rsxy(i1-2*is1,i2-2*is2,i3,axisp1,0)**2+rsxy(i1-
     & 2*is1,i2-2*is2,i3,axisp1,1)**2))   )/(12.*dra)
                    if( axis.eq.0 )then
                      c1r = (rsxyxr42(i1,i2,i3,axis,0)+rsxyyr42(i1,i2,
     & i3,axis,1))
                      c2r = (rsxyxr42(i1,i2,i3,axisp1,0)+rsxyyr42(i1,
     & i2,i3,axisp1,1))
                    else
                      c1r = (rsxyxs42(i1,i2,i3,axis,0)+rsxyys42(i1,i2,
     & i3,axis,1))
                      c2r = (rsxyxs42(i1,i2,i3,axisp1,0)+rsxyys42(i1,
     & i2,i3,axisp1,1))
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
                 !   ws=US2(hz)
                 !   wss=USS2(hz)
                    ws=(8.*(u(i1+js1,i2+js2,i3+js3,hz)-u(i1-js1,i2-js2,
     & i3-js3,hz))-(u(i1+2*js1,i2+2*js2,i3+2*js3,hz)-u(i1-2*js1,i2-2*
     & js2,i3-2*js3,hz)))/(12.*dsa)
                    wss=(-30.*u(i1,i2,i3,hz)+16.*(u(i1+js1,i2+js2,i3+
     & js3,hz)+u(i1-js1,i2-js2,i3-js3,hz))-(u(i1+2*js1,i2+2*js2,i3+2*
     & js3,hz)+u(i1-2*js1,i2-2*js2,i3-2*js3,hz)))/(12.*dsa**2)
                    tau1=rsxy(i1,i2,i3,axisp1,0)
                    tau2=rsxy(i1,i2,i3,axisp1,1)
                    uex=u(i1,i2,i3,ex)
                    uey=u(i1,i2,i3,ey)
                   ! Dr( a1.Delta\uv ) = (b3u,b3v).uvrrr + (b2u,b2v).uvrr + (b1u,b1v).uv + bf = 0 
                   ! For TZ choose bf = bf - Dr( a1.Delta uvExact )
                   ! see bcdiv.maple for:
      b3u=c11*a11
      b3v=a12*c11
      b2u=(2*c22*a12s*a11**2+c1*a11**2*a22+c11r*a11**2*a22-2*c22*a11s*
     & a12*a11-c1*a11*a21*a12-c11r*a11*a21*a12-a11r*c11*a21*a12+a11r*
     & c11*a11*a22)/(-a21*a12+a11*a22)
      b2v=(-2*c22*a11s*a12**2+2*c22*a12s*a11*a12+a12*c1*a11*a22+a12*
     & c11r*a11*a22-a12r*c11*a21*a12+a12r*c11*a11*a22-c11r*a21*a12**2-
     & c1*a21*a12**2)/(-a21*a12+a11*a22)
      b1u=(-4*c22*a11s*a12*a11r-c22*a11ss*a11*a22+c1r*a11**2*a22+4*c22*
     & a12s*a11*a11r-c1r*a11*a21*a12+2*c22*a11s**2*a22-2*c22*a12s*a21*
     & a11s-2*c22*a11s*a12*a21s+2*c22*a12s*a11*a21s-c2*a11*a11s*a22+
     & c22*a11ss*a21*a12+a12*c2*a21*a11s-a11r*c1*a21*a12+a11r*c1*a11*
     & a22)/(-a21*a12+a11*a22)
      b1v=(-2*c22*a21*a12s**2+2*c22*a11s*a12s*a22+2*c22*a12s*a11*a22s-
     & 2*c22*a11s*a12*a22s+c22*a12ss*a21*a12+4*c22*a12s*a11*a12r-c22*
     & a12ss*a11*a22-4*c22*a11s*a12*a12r+a12*c1r*a11*a22+a12*c2*a21*
     & a12s-a12r*c1*a21*a12+a12r*c1*a11*a22-c1r*a21*a12**2-c2*a11*
     & a12s*a22)/(-a21*a12+a11*a22)
      bf =-(-2*c22*a12s*a11*a21rs*uex+2*c22*a12s*a21*a21ss*uex+3*c22*
     & a22s*vss*a11*a22-c22r*uss*a11**2*a22-2*c22*a11s*a22**2*vss-c22*
     & a21**2*usss*a12+c22*a22**2*vsss*a11+2*c22*a12s*a21**2*uss-c2r*
     & us*a11**2*a22+c2*a11*a22**2*vss-3*c22*a21ss*us*a21*a12-2*c22*
     & a12s*a11*a11rr*uex+3*c22*a21ss*us*a11*a22-3*c22*a22ss*vs*a21*
     & a12+2*c22*a11s*a12*a22r*vs+3*c22*a22ss*vs*a11*a22+c2r*us*a11*
     & a21*a12+2*c2*a11*a22s*vs*a22-c22*a22*vsss*a21*a12-3*c22*a21s*
     & uss*a21*a12+3*c22*a21s*uss*a11*a22-c22*a12rss*uey*a21*a12+c22*
     & a12rss*uey*a11*a22-c22*a11rss*uex*a21*a12+2*c22*a12s*a21*a22ss*
     & uey+2*c22*a12s*a21*a22*vss+4*c22*a12s*a21*a21s*us+c22*a11rss*
     & uex*a11*a22-2*c22*a12rs*vs*a21*a12+2*c22*a12rs*vs*a11*a22-3*
     & c22*a22s*vss*a21*a12-c22*a21sss*uex*a21*a12+c22*a21sss*uex*a11*
     & a22-2*c22*a11rs*us*a21*a12+2*c22*a12s*a21*a11rs*uex+2*c22*a12s*
     & a21*a11r*us+2*c22*a11s*a12*a21r*us+2*c22*a11s*a12*a21rs*uex-2*
     & c22*a12s*a11*a21r*us-2*c22*a12s*a11*a22rs*uey-2*c22*a12s*a11*
     & a22r*vs+2*c22*a12s*a21*a12r*vs+2*c22*a11s*a12*a22rs*uey+2*c22*
     & a11rs*us*a11*a22+c2*a11*a21*uss*a22+c2*a11*a11rs*uex*a22-2*c22*
     & a11s*a12rs*uey*a22+c22*a22sss*uey*a11*a22-2*c22*a12s*a11*a12rr*
     & uey+4*c22*a12s*a21*a22s*vs+2*c22*a12s*a21*a12rs*uey-2*c22*a11s*
     & a21ss*uex*a22-2*c22*a11s*a22ss*uey*a22+2*c22*a11s*a12*a11rr*
     & uex-4*c22*a11s*a22s*vs*a22-2*c22*a11s*a11rs*uex*a22-2*c22*a11s*
     & a21*uss*a22-4*c22*a11s*a21s*us*a22-2*c22*a11s*a11r*us*a22+2*
     & c22*a11s*a12*a12rr*uey+c22*a21*usss*a11*a22-c22*a22sss*uey*a21*
     & a12-2*c22*a11s*a12r*vs*a22+c22r*uss*a11*a21*a12-a12*c22r*vss*
     & a11*a22-a12*c2r*vs*a11*a22-a12*c2*a21*a12rs*uey-2*a12*c2*a21*
     & a22s*vs-a12*c2*a21*a11rs*uex-a12*c2*a21*a22*vss-2*a12*c2*a21*
     & a21s*us-a12*c2*a21*a21ss*uex-a12*c2*a21*a22ss*uey-a12*c2*a21**
     & 2*uss+c2r*vs*a21*a12**2+c22r*vss*a21*a12**2+c2*a11*a12rs*uey*
     & a22+c2*a11*a22ss*uey*a22+c2*a11*a21ss*uex*a22+2*c2*a11*a21s*us*
     & a22)/(-a21*a12+a11*a22)

! -- Here are the approximations for urs, vrs from the divergence
!  ursm =-(2*a22s*vs*a22-a12*a11*urr-a12*a22r*vs-a12*a22s*vr-a12*a22rs*uey-a12*a21r*us-a12*a21s*ur-a12*a21rs*uex-2*a12*a11r*ur-a12*a11rr*uex+a12rs*uey*a22+a12r*vs*a22+a12s*vr*a22+a22ss*uey*a22+a21ss*uex*a22+2*a21s*us*a22+a21*uss*a22+a22**2*vss-a12**2*vrr+a11rs*uex*a22+a11r*us*a22-2*a12*a12r*vr+a11s*ur*a22-a12*a12rr*uey)/(-a21*a12+a11*a22)
!  vrsm =(-a11**2*urr-2*a11*a12r*vr-a11*a12rr*uey+a21*a12r*vs+a21*a12rs*uey+2*a21*a22s*vs+a21*a11s*ur+a21*a11r*us+a21*a11rs*uex+a21*a12s*vr-a11*a22r*vs-a11*a22s*vr-a11*a22rs*uey-a11*a21r*us-a11*a21s*ur-a11*a21rs*uex-a11*a12*vrr+a21*a22*vss+2*a21*a21s*us+a21*a21ss*uex+a21*a22ss*uey-2*a11*a11r*ur+a21**2*uss-a11*a11rr*uex)/(-a21*a12+a11*a22)
                 ! ************ Answer *******************
                  ctlrr=1.
                  ctlr=1.
                  ! forcing terms for TZ are stored in 
                  cgI=1.
                  gIf=0.
                  gIVf=0.
                  tau1DotUtt=0.
                  Da1DotU=0.
                  ! for Hz (w)
                  fw1=0.
                  fw2=0.
                  if( boundaryForcingOption.ne.noBoundaryForcing )then
                    ! ------------ BOUNDARY none 2D --------------
                    ! In the boundary forcing we subtract out a plane wave incident field
                    ! This causes the BC to be 
                    !           tau.u = - tau.uI
                    !   and     tau.utt = -tau.uI.tt
                    ! *** set RHS for (a1.u).r =  - Ds( a2.uv )
                    Da1DotU = -(  a21s*uex+a22s*uey + a21*us+a22*vs )
                    ! Note minus sign since we are subtracting out the incident field
                    x0=xy(i1,i2,i3,0)
                    y0=xy(i1,i2,i3,1)
                    if( .true. )then ! *new way*
                      numberOfTimeDerivatives=1+fieldOption
                        if( boundaryForcingOption.eq.noBoundaryForcing 
     & )then
                        else if( 
     & boundaryForcingOption.eq.planeWaveBoundaryForcing )then
                            if( numberOfTimeDerivatives==0 )then
                              ubv(ex) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==1 )then
                              ubv(ex) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(0)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(1)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(5)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==2 )then
                              ubv(ex) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==3 )then
                              ubv(ex) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==4 )then
                              ubv(ex) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))
                            else
                              stop 1738
                            end if
                        else if(  
     & boundaryForcingOption.eq.chirpedPlaneWaveBoundaryForcing )then
                           xi0 = .5*(cpwTa+cpwTb)
                           xi = t - (kx*(x0-cpwX0)+ky*(y0-cpwY0))/cc -
     & xi0
                           cpwTau=cpwTb-cpwTa  ! tau = tb -ta
                           ! include files generated by the maple code mx/codes/chirpedPlaneWave.maple 
                           if( numberOfTimeDerivatives.eq.0 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 0-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = .5*cpwTau
      t4 = tanh(cpwBeta*(xi+t1))
      t7 = tanh(cpwBeta*(xi-t1))
      t11 = xi ** 2
      t16 = sin(twoPi*(cc*xi+t11*cpwAlpha))
      chirp = cpwAmp*(t4/2.-t7/2.)*t16

                           else if(  numberOfTimeDerivatives.eq.1 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 1-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = .5*cpwTau
      t4 = tanh(cpwBeta*(xi+t1))
      t5 = t4 ** 2
      t10 = tanh(cpwBeta*(xi-t1))
      t11 = t10 ** 2
      t17 = xi ** 2
      t21 = twoPi*(cc*xi+t17*cpwAlpha)
      t22 = sin(t21)
      t31 = cos(t21)
      chirp = cpwAmp*(cpwBeta*(1.-t5)/2.-cpwBeta*(1.-t11)/2.)*t22+
     & cpwAmp*(t4/2.-t10/2.)*twoPi*(2.*xi*cpwAlpha+cc)*t31

                           else if(  numberOfTimeDerivatives.eq.2 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 2-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+2*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+2*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = .5*cpwTau
      t5 = tanh(cpwBeta*(xi+t2))
      t7 = t5 ** 2
      t8 = 1.-t7
      t12 = tanh(cpwBeta*(xi-t2))
      t14 = t12 ** 2
      t15 = 1.-t14
      t19 = xi ** 2
      t23 = twoPi*(cc*xi+t19*cpwAlpha)
      t24 = sin(t23)
      t33 = 2.*xi*cpwAlpha+cc
      t35 = cos(t23)
      t41 = cpwAmp*(t5/2.-t12/2.)
      t46 = twoPi ** 2
      t47 = t33 ** 2
      chirp = cpwAmp*(t1*t12*t15-t1*t5*t8)*t24+2.*cpwAmp*(-cpwBeta*
     & t15/2.+cpwBeta*t8/2.)*twoPi*t33*t35+2.*t41*twoPi*cpwAlpha*t35-
     & t41*t46*t47*t24

                           else if(  numberOfTimeDerivatives.eq.3 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 3-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(-cpwBeta^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2+2*cpwBeta^3*tanh(cpwBeta*(xi+.5*cpwTau))^2*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2-2*cpwBeta^3*tanh(cpwBeta*(xi-.5*cpwTau))^2*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+3*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+6*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-3*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-6*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*cpwAlpha*(2*xi*cpwAlpha+cc)*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^3*(2*xi*cpwAlpha+cc)^3*cos(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = t1*cpwBeta
      t3 = .5*cpwTau
      t6 = tanh(cpwBeta*(xi+t3))
      t7 = t6 ** 2
      t8 = 1.-t7
      t9 = t8 ** 2
      t16 = tanh(cpwBeta*(xi-t3))
      t17 = t16 ** 2
      t18 = 1.-t17
      t19 = t18 ** 2
      t26 = xi ** 2
      t30 = twoPi*(cc*xi+t26*cpwAlpha)
      t31 = sin(t30)
      t41 = 2.*xi*cpwAlpha+cc
      t43 = cos(t30)
      t51 = cpwAmp*(-cpwBeta*t18/2.+cpwBeta*t8/2.)
      t56 = twoPi ** 2
      t57 = t41 ** 2
      t64 = cpwAmp*(t6/2.-t16/2.)
      chirp = cpwAmp*(-2.*t2*t17*t18+2.*t2*t7*t8+t2*t19-t2*t9)*t31+
     & 0.3E1*cpwAmp*(t1*t16*t18-t1*t6*t8)*twoPi*t41*t43+6.*t51*twoPi*
     & cpwAlpha*t43-0.3E1*t51*t56*t57*t31-6.*t64*t56*cpwAlpha*t41*t31-
     & t64*t56*twoPi*t57*t41*t43

                           else if(  numberOfTimeDerivatives.eq.4 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 4-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(8*cpwBeta^4*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2*tanh(cpwBeta*(xi+.5*cpwTau))-4*cpwBeta^4*tanh(cpwBeta*(xi+.5*cpwTau))^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-8*cpwBeta^4*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2*tanh(cpwBeta*(xi-.5*cpwTau))+4*cpwBeta^4*tanh(cpwBeta*(xi-.5*cpwTau))^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+4*cpwAmp*(-cpwBeta^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2+2*cpwBeta^3*tanh(cpwBeta*(xi+.5*cpwTau))^2*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2-2*cpwBeta^3*tanh(cpwBeta*(xi-.5*cpwTau))^2*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+12*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-6*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-24*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*cpwAlpha*(2*xi*cpwAlpha+cc)*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-4*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^3*(2*xi*cpwAlpha+cc)^3*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-12*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*cpwAlpha^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-12*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^3*cpwAlpha*(2*xi*cpwAlpha+cc)^2*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^4*(2*xi*cpwAlpha+cc)^4*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = t1 ** 2
      t3 = .5*cpwTau
      t6 = tanh(cpwBeta*(xi+t3))
      t7 = t6 ** 2
      t8 = 1.-t7
      t9 = t8 ** 2
      t19 = tanh(cpwBeta*(xi-t3))
      t20 = t19 ** 2
      t21 = 1.-t20
      t22 = t21 ** 2
      t32 = xi ** 2
      t36 = twoPi*(cc*xi+t32*cpwAlpha)
      t37 = sin(t36)
      t39 = t1*cpwBeta
      t52 = 2.*xi*cpwAlpha+cc
      t54 = cos(t36)
      t63 = cpwAmp*(t1*t19*t21-t1*t6*t8)
      t68 = twoPi ** 2
      t69 = t52 ** 2
      t78 = cpwAmp*(-cpwBeta*t21/2.+cpwBeta*t8/2.)
      t84 = t68*twoPi
      t92 = cpwAmp*(t6/2.-t19/2.)
      t93 = cpwAlpha ** 2
      t103 = t68 ** 2
      t104 = t69 ** 2
      chirp = cpwAmp*(4.*t2*t20*t19*t21-4.*t2*t7*t6*t8-8.*t2*t22*t19+
     & 8.*t2*t9*t6)*t37+4.*cpwAmp*(-2.*t39*t20*t21+2.*t39*t7*t8+t39*
     & t22-t39*t9)*twoPi*t52*t54+12.*t63*twoPi*cpwAlpha*t54-6.*t63*
     & t68*t69*t37-0.24E2*t78*t68*cpwAlpha*t52*t37-4.*t78*t84*t69*t52*
     & t54-12.*t92*t68*t93*t37-12.*t92*t84*cpwAlpha*t69*t54+t92*t103*
     & t104*t37

                           else
                             write(*,'(" getChirp2D:ERROR: too many 
     & derivatives requested")')
                             stop 4927
                           end if
                           ubv(ex) = chirp*pwc(0)
                           ubv(ey) = chirp*pwc(1)
                           ubv(hz) = chirp*pwc(5)
                        else
                          write(*,'("getBndryForcing2D: Unknown 
     & boundary forcing")')
                        end if
                      ut0 = -ubv(ex)
                      vt0 = -ubv(ey)
                      numberOfTimeDerivatives=2+fieldOption
                        if( boundaryForcingOption.eq.noBoundaryForcing 
     & )then
                        else if( 
     & boundaryForcingOption.eq.planeWaveBoundaryForcing )then
                            if( numberOfTimeDerivatives==0 )then
                              ubv(ex) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==1 )then
                              ubv(ex) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(0)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(1)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(5)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==2 )then
                              ubv(ex) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==3 )then
                              ubv(ex) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==4 )then
                              ubv(ex) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))
                            else
                              stop 1738
                            end if
                        else if(  
     & boundaryForcingOption.eq.chirpedPlaneWaveBoundaryForcing )then
                           xi0 = .5*(cpwTa+cpwTb)
                           xi = t - (kx*(x0-cpwX0)+ky*(y0-cpwY0))/cc -
     & xi0
                           cpwTau=cpwTb-cpwTa  ! tau = tb -ta
                           ! include files generated by the maple code mx/codes/chirpedPlaneWave.maple 
                           if( numberOfTimeDerivatives.eq.0 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 0-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = .5*cpwTau
      t4 = tanh(cpwBeta*(xi+t1))
      t7 = tanh(cpwBeta*(xi-t1))
      t11 = xi ** 2
      t16 = sin(twoPi*(cc*xi+t11*cpwAlpha))
      chirp = cpwAmp*(t4/2.-t7/2.)*t16

                           else if(  numberOfTimeDerivatives.eq.1 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 1-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = .5*cpwTau
      t4 = tanh(cpwBeta*(xi+t1))
      t5 = t4 ** 2
      t10 = tanh(cpwBeta*(xi-t1))
      t11 = t10 ** 2
      t17 = xi ** 2
      t21 = twoPi*(cc*xi+t17*cpwAlpha)
      t22 = sin(t21)
      t31 = cos(t21)
      chirp = cpwAmp*(cpwBeta*(1.-t5)/2.-cpwBeta*(1.-t11)/2.)*t22+
     & cpwAmp*(t4/2.-t10/2.)*twoPi*(2.*xi*cpwAlpha+cc)*t31

                           else if(  numberOfTimeDerivatives.eq.2 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 2-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+2*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+2*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = .5*cpwTau
      t5 = tanh(cpwBeta*(xi+t2))
      t7 = t5 ** 2
      t8 = 1.-t7
      t12 = tanh(cpwBeta*(xi-t2))
      t14 = t12 ** 2
      t15 = 1.-t14
      t19 = xi ** 2
      t23 = twoPi*(cc*xi+t19*cpwAlpha)
      t24 = sin(t23)
      t33 = 2.*xi*cpwAlpha+cc
      t35 = cos(t23)
      t41 = cpwAmp*(t5/2.-t12/2.)
      t46 = twoPi ** 2
      t47 = t33 ** 2
      chirp = cpwAmp*(t1*t12*t15-t1*t5*t8)*t24+2.*cpwAmp*(-cpwBeta*
     & t15/2.+cpwBeta*t8/2.)*twoPi*t33*t35+2.*t41*twoPi*cpwAlpha*t35-
     & t41*t46*t47*t24

                           else if(  numberOfTimeDerivatives.eq.3 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 3-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(-cpwBeta^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2+2*cpwBeta^3*tanh(cpwBeta*(xi+.5*cpwTau))^2*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2-2*cpwBeta^3*tanh(cpwBeta*(xi-.5*cpwTau))^2*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+3*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+6*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-3*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-6*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*cpwAlpha*(2*xi*cpwAlpha+cc)*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^3*(2*xi*cpwAlpha+cc)^3*cos(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = t1*cpwBeta
      t3 = .5*cpwTau
      t6 = tanh(cpwBeta*(xi+t3))
      t7 = t6 ** 2
      t8 = 1.-t7
      t9 = t8 ** 2
      t16 = tanh(cpwBeta*(xi-t3))
      t17 = t16 ** 2
      t18 = 1.-t17
      t19 = t18 ** 2
      t26 = xi ** 2
      t30 = twoPi*(cc*xi+t26*cpwAlpha)
      t31 = sin(t30)
      t41 = 2.*xi*cpwAlpha+cc
      t43 = cos(t30)
      t51 = cpwAmp*(-cpwBeta*t18/2.+cpwBeta*t8/2.)
      t56 = twoPi ** 2
      t57 = t41 ** 2
      t64 = cpwAmp*(t6/2.-t16/2.)
      chirp = cpwAmp*(-2.*t2*t17*t18+2.*t2*t7*t8+t2*t19-t2*t9)*t31+
     & 0.3E1*cpwAmp*(t1*t16*t18-t1*t6*t8)*twoPi*t41*t43+6.*t51*twoPi*
     & cpwAlpha*t43-0.3E1*t51*t56*t57*t31-6.*t64*t56*cpwAlpha*t41*t31-
     & t64*t56*twoPi*t57*t41*t43

                           else if(  numberOfTimeDerivatives.eq.4 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 4-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(8*cpwBeta^4*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2*tanh(cpwBeta*(xi+.5*cpwTau))-4*cpwBeta^4*tanh(cpwBeta*(xi+.5*cpwTau))^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-8*cpwBeta^4*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2*tanh(cpwBeta*(xi-.5*cpwTau))+4*cpwBeta^4*tanh(cpwBeta*(xi-.5*cpwTau))^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+4*cpwAmp*(-cpwBeta^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2+2*cpwBeta^3*tanh(cpwBeta*(xi+.5*cpwTau))^2*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2-2*cpwBeta^3*tanh(cpwBeta*(xi-.5*cpwTau))^2*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+12*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-6*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-24*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*cpwAlpha*(2*xi*cpwAlpha+cc)*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-4*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^3*(2*xi*cpwAlpha+cc)^3*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-12*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*cpwAlpha^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-12*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^3*cpwAlpha*(2*xi*cpwAlpha+cc)^2*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^4*(2*xi*cpwAlpha+cc)^4*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = t1 ** 2
      t3 = .5*cpwTau
      t6 = tanh(cpwBeta*(xi+t3))
      t7 = t6 ** 2
      t8 = 1.-t7
      t9 = t8 ** 2
      t19 = tanh(cpwBeta*(xi-t3))
      t20 = t19 ** 2
      t21 = 1.-t20
      t22 = t21 ** 2
      t32 = xi ** 2
      t36 = twoPi*(cc*xi+t32*cpwAlpha)
      t37 = sin(t36)
      t39 = t1*cpwBeta
      t52 = 2.*xi*cpwAlpha+cc
      t54 = cos(t36)
      t63 = cpwAmp*(t1*t19*t21-t1*t6*t8)
      t68 = twoPi ** 2
      t69 = t52 ** 2
      t78 = cpwAmp*(-cpwBeta*t21/2.+cpwBeta*t8/2.)
      t84 = t68*twoPi
      t92 = cpwAmp*(t6/2.-t19/2.)
      t93 = cpwAlpha ** 2
      t103 = t68 ** 2
      t104 = t69 ** 2
      chirp = cpwAmp*(4.*t2*t20*t19*t21-4.*t2*t7*t6*t8-8.*t2*t22*t19+
     & 8.*t2*t9*t6)*t37+4.*cpwAmp*(-2.*t39*t20*t21+2.*t39*t7*t8+t39*
     & t22-t39*t9)*twoPi*t52*t54+12.*t63*twoPi*cpwAlpha*t54-6.*t63*
     & t68*t69*t37-0.24E2*t78*t68*cpwAlpha*t52*t37-4.*t78*t84*t69*t52*
     & t54-12.*t92*t68*t93*t37-12.*t92*t84*cpwAlpha*t69*t54+t92*t103*
     & t104*t37

                           else
                             write(*,'(" getChirp2D:ERROR: too many 
     & derivatives requested")')
                             stop 4927
                           end if
                           ubv(ex) = chirp*pwc(0)
                           ubv(ey) = chirp*pwc(1)
                           ubv(hz) = chirp*pwc(5)
                        else
                          write(*,'("getBndryForcing2D: Unknown 
     & boundary forcing")')
                        end if
                      utt00 = -ubv(ex)
                      vtt00 = -ubv(ey)
                      numberOfTimeDerivatives=3+fieldOption
                        if( boundaryForcingOption.eq.noBoundaryForcing 
     & )then
                        else if( 
     & boundaryForcingOption.eq.planeWaveBoundaryForcing )then
                            if( numberOfTimeDerivatives==0 )then
                              ubv(ex) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==1 )then
                              ubv(ex) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(0)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(1)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(5)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==2 )then
                              ubv(ex) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==3 )then
                              ubv(ex) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==4 )then
                              ubv(ex) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))
                            else
                              stop 1738
                            end if
                        else if(  
     & boundaryForcingOption.eq.chirpedPlaneWaveBoundaryForcing )then
                           xi0 = .5*(cpwTa+cpwTb)
                           xi = t - (kx*(x0-cpwX0)+ky*(y0-cpwY0))/cc -
     & xi0
                           cpwTau=cpwTb-cpwTa  ! tau = tb -ta
                           ! include files generated by the maple code mx/codes/chirpedPlaneWave.maple 
                           if( numberOfTimeDerivatives.eq.0 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 0-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = .5*cpwTau
      t4 = tanh(cpwBeta*(xi+t1))
      t7 = tanh(cpwBeta*(xi-t1))
      t11 = xi ** 2
      t16 = sin(twoPi*(cc*xi+t11*cpwAlpha))
      chirp = cpwAmp*(t4/2.-t7/2.)*t16

                           else if(  numberOfTimeDerivatives.eq.1 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 1-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = .5*cpwTau
      t4 = tanh(cpwBeta*(xi+t1))
      t5 = t4 ** 2
      t10 = tanh(cpwBeta*(xi-t1))
      t11 = t10 ** 2
      t17 = xi ** 2
      t21 = twoPi*(cc*xi+t17*cpwAlpha)
      t22 = sin(t21)
      t31 = cos(t21)
      chirp = cpwAmp*(cpwBeta*(1.-t5)/2.-cpwBeta*(1.-t11)/2.)*t22+
     & cpwAmp*(t4/2.-t10/2.)*twoPi*(2.*xi*cpwAlpha+cc)*t31

                           else if(  numberOfTimeDerivatives.eq.2 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 2-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+2*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+2*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = .5*cpwTau
      t5 = tanh(cpwBeta*(xi+t2))
      t7 = t5 ** 2
      t8 = 1.-t7
      t12 = tanh(cpwBeta*(xi-t2))
      t14 = t12 ** 2
      t15 = 1.-t14
      t19 = xi ** 2
      t23 = twoPi*(cc*xi+t19*cpwAlpha)
      t24 = sin(t23)
      t33 = 2.*xi*cpwAlpha+cc
      t35 = cos(t23)
      t41 = cpwAmp*(t5/2.-t12/2.)
      t46 = twoPi ** 2
      t47 = t33 ** 2
      chirp = cpwAmp*(t1*t12*t15-t1*t5*t8)*t24+2.*cpwAmp*(-cpwBeta*
     & t15/2.+cpwBeta*t8/2.)*twoPi*t33*t35+2.*t41*twoPi*cpwAlpha*t35-
     & t41*t46*t47*t24

                           else if(  numberOfTimeDerivatives.eq.3 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 3-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(-cpwBeta^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2+2*cpwBeta^3*tanh(cpwBeta*(xi+.5*cpwTau))^2*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2-2*cpwBeta^3*tanh(cpwBeta*(xi-.5*cpwTau))^2*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+3*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+6*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-3*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-6*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*cpwAlpha*(2*xi*cpwAlpha+cc)*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^3*(2*xi*cpwAlpha+cc)^3*cos(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = t1*cpwBeta
      t3 = .5*cpwTau
      t6 = tanh(cpwBeta*(xi+t3))
      t7 = t6 ** 2
      t8 = 1.-t7
      t9 = t8 ** 2
      t16 = tanh(cpwBeta*(xi-t3))
      t17 = t16 ** 2
      t18 = 1.-t17
      t19 = t18 ** 2
      t26 = xi ** 2
      t30 = twoPi*(cc*xi+t26*cpwAlpha)
      t31 = sin(t30)
      t41 = 2.*xi*cpwAlpha+cc
      t43 = cos(t30)
      t51 = cpwAmp*(-cpwBeta*t18/2.+cpwBeta*t8/2.)
      t56 = twoPi ** 2
      t57 = t41 ** 2
      t64 = cpwAmp*(t6/2.-t16/2.)
      chirp = cpwAmp*(-2.*t2*t17*t18+2.*t2*t7*t8+t2*t19-t2*t9)*t31+
     & 0.3E1*cpwAmp*(t1*t16*t18-t1*t6*t8)*twoPi*t41*t43+6.*t51*twoPi*
     & cpwAlpha*t43-0.3E1*t51*t56*t57*t31-6.*t64*t56*cpwAlpha*t41*t31-
     & t64*t56*twoPi*t57*t41*t43

                           else if(  numberOfTimeDerivatives.eq.4 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 4-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(8*cpwBeta^4*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2*tanh(cpwBeta*(xi+.5*cpwTau))-4*cpwBeta^4*tanh(cpwBeta*(xi+.5*cpwTau))^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-8*cpwBeta^4*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2*tanh(cpwBeta*(xi-.5*cpwTau))+4*cpwBeta^4*tanh(cpwBeta*(xi-.5*cpwTau))^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+4*cpwAmp*(-cpwBeta^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2+2*cpwBeta^3*tanh(cpwBeta*(xi+.5*cpwTau))^2*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2-2*cpwBeta^3*tanh(cpwBeta*(xi-.5*cpwTau))^2*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+12*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-6*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-24*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*cpwAlpha*(2*xi*cpwAlpha+cc)*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-4*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^3*(2*xi*cpwAlpha+cc)^3*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-12*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*cpwAlpha^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-12*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^3*cpwAlpha*(2*xi*cpwAlpha+cc)^2*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^4*(2*xi*cpwAlpha+cc)^4*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = t1 ** 2
      t3 = .5*cpwTau
      t6 = tanh(cpwBeta*(xi+t3))
      t7 = t6 ** 2
      t8 = 1.-t7
      t9 = t8 ** 2
      t19 = tanh(cpwBeta*(xi-t3))
      t20 = t19 ** 2
      t21 = 1.-t20
      t22 = t21 ** 2
      t32 = xi ** 2
      t36 = twoPi*(cc*xi+t32*cpwAlpha)
      t37 = sin(t36)
      t39 = t1*cpwBeta
      t52 = 2.*xi*cpwAlpha+cc
      t54 = cos(t36)
      t63 = cpwAmp*(t1*t19*t21-t1*t6*t8)
      t68 = twoPi ** 2
      t69 = t52 ** 2
      t78 = cpwAmp*(-cpwBeta*t21/2.+cpwBeta*t8/2.)
      t84 = t68*twoPi
      t92 = cpwAmp*(t6/2.-t19/2.)
      t93 = cpwAlpha ** 2
      t103 = t68 ** 2
      t104 = t69 ** 2
      chirp = cpwAmp*(4.*t2*t20*t19*t21-4.*t2*t7*t6*t8-8.*t2*t22*t19+
     & 8.*t2*t9*t6)*t37+4.*cpwAmp*(-2.*t39*t20*t21+2.*t39*t7*t8+t39*
     & t22-t39*t9)*twoPi*t52*t54+12.*t63*twoPi*cpwAlpha*t54-6.*t63*
     & t68*t69*t37-0.24E2*t78*t68*cpwAlpha*t52*t37-4.*t78*t84*t69*t52*
     & t54-12.*t92*t68*t93*t37-12.*t92*t84*cpwAlpha*t69*t54+t92*t103*
     & t104*t37

                           else
                             write(*,'(" getChirp2D:ERROR: too many 
     & derivatives requested")')
                             stop 4927
                           end if
                           ubv(ex) = chirp*pwc(0)
                           ubv(ey) = chirp*pwc(1)
                           ubv(hz) = chirp*pwc(5)
                        else
                          write(*,'("getBndryForcing2D: Unknown 
     & boundary forcing")')
                        end if
                      uttt0 = -ubv(ex)
                      vttt0 = -ubv(ey)
                    else if( fieldOption.eq.0 )then ! *old* way
                      utt00=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x0)+
     & ky*(y0)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(0)+ssftt*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(
     & t)))*pwc(0))
                      vtt00=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x0)+
     & ky*(y0)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(1)+ssftt*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(
     & t)))*pwc(1))
                      ut0  =-(ssf*(-twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(
     & y0)-cc*(t)))*pwc(0)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*
     & pwc(0))
                      vt0  =-(ssf*(-twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(
     & y0)-cc*(t)))*pwc(1)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*
     & pwc(1))
                      uttt0=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x0)+ky*
     & (y0)-cc*(t)))*pwc(0))+3.*ssft*(-(twoPi*cc)**2*sin(twoPi*(kx*(
     & x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssftt*(-twoPi*cc)*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssfttt*sin(twoPi*(kx*(x0)+ky*(
     & y0)-cc*(t)))*pwc(0))
                      vttt0=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x0)+ky*
     & (y0)-cc*(t)))*pwc(1))+3.*ssft*(-(twoPi*cc)**2*sin(twoPi*(kx*(
     & x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssftt*(-twoPi*cc)*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssfttt*sin(twoPi*(kx*(x0)+ky*(
     & y0)-cc*(t)))*pwc(1))
                    else
                      ! we are assigning time derivatives (sosup)
                      utt00=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x0)+ky*
     & (y0)-cc*(t)))*pwc(0))+3.*ssft*(-(twoPi*cc)**2*sin(twoPi*(kx*(
     & x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssftt*(-twoPi*cc)*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssfttt*sin(twoPi*(kx*(x0)+ky*(
     & y0)-cc*(t)))*pwc(0))
                      vtt00=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x0)+ky*
     & (y0)-cc*(t)))*pwc(1))+3.*ssft*(-(twoPi*cc)**2*sin(twoPi*(kx*(
     & x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssftt*(-twoPi*cc)*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssfttt*sin(twoPi*(kx*(x0)+ky*(
     & y0)-cc*(t)))*pwc(1))
                      ut0  =-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x0)+
     & ky*(y0)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(0)+ssftt*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(
     & t)))*pwc(0))
                      vt0  =-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x0)+
     & ky*(y0)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(1)+ssftt*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(
     & t)))*pwc(1))
                      uttt0=-(ssf*((twoPi*cc)**4*sin(twoPi*(kx*(x0)+ky*
     & (y0)-cc*(t)))*pwc(0))+4.*ssft*((twoPi*cc)**3*cos(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(0))+6.*ssftt*(-(twoPi*cc)**2*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+4.*ssfttt*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssftttt*sin(twoPi*(kx*(
     & x0)+ky*(y0)-cc*(t)))*pwc(0))
                      vttt0=-(ssf*((twoPi*cc)**4*sin(twoPi*(kx*(x0)+ky*
     & (y0)-cc*(t)))*pwc(1))+4.*ssft*((twoPi*cc)**3*cos(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(1))+6.*ssftt*(-(twoPi*cc)**2*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+4.*ssfttt*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssftttt*sin(twoPi*(kx*(
     & x0)+ky*(y0)-cc*(t)))*pwc(1))
                    end if
                    tau1DotUtt = tau1*utt00+tau2*vtt00
                    ! (a1.Delta u).r = - (a2.utt).s
                    ! (a1.Delta u).r + bf = 0
                    ! bf = bf + ( (a21zp1*uttzp1+a22zp1*vttzp1)-(a21zm1*uttzm1+a22zm1*vttzm1) )/(2.(dsa)
                    x0=xy(i1+js1,i2+js2,i3,0)
                    y0=xy(i1+js1,i2+js2,i3,1)
                    if( .true. )then ! *new way*
                      numberOfTimeDerivatives=1+fieldOption
                        if( boundaryForcingOption.eq.noBoundaryForcing 
     & )then
                        else if( 
     & boundaryForcingOption.eq.planeWaveBoundaryForcing )then
                            if( numberOfTimeDerivatives==0 )then
                              ubv(ex) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==1 )then
                              ubv(ex) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(0)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(1)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(5)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==2 )then
                              ubv(ex) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==3 )then
                              ubv(ex) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==4 )then
                              ubv(ex) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))
                            else
                              stop 1738
                            end if
                        else if(  
     & boundaryForcingOption.eq.chirpedPlaneWaveBoundaryForcing )then
                           xi0 = .5*(cpwTa+cpwTb)
                           xi = t - (kx*(x0-cpwX0)+ky*(y0-cpwY0))/cc -
     & xi0
                           cpwTau=cpwTb-cpwTa  ! tau = tb -ta
                           ! include files generated by the maple code mx/codes/chirpedPlaneWave.maple 
                           if( numberOfTimeDerivatives.eq.0 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 0-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = .5*cpwTau
      t4 = tanh(cpwBeta*(xi+t1))
      t7 = tanh(cpwBeta*(xi-t1))
      t11 = xi ** 2
      t16 = sin(twoPi*(cc*xi+t11*cpwAlpha))
      chirp = cpwAmp*(t4/2.-t7/2.)*t16

                           else if(  numberOfTimeDerivatives.eq.1 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 1-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = .5*cpwTau
      t4 = tanh(cpwBeta*(xi+t1))
      t5 = t4 ** 2
      t10 = tanh(cpwBeta*(xi-t1))
      t11 = t10 ** 2
      t17 = xi ** 2
      t21 = twoPi*(cc*xi+t17*cpwAlpha)
      t22 = sin(t21)
      t31 = cos(t21)
      chirp = cpwAmp*(cpwBeta*(1.-t5)/2.-cpwBeta*(1.-t11)/2.)*t22+
     & cpwAmp*(t4/2.-t10/2.)*twoPi*(2.*xi*cpwAlpha+cc)*t31

                           else if(  numberOfTimeDerivatives.eq.2 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 2-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+2*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+2*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = .5*cpwTau
      t5 = tanh(cpwBeta*(xi+t2))
      t7 = t5 ** 2
      t8 = 1.-t7
      t12 = tanh(cpwBeta*(xi-t2))
      t14 = t12 ** 2
      t15 = 1.-t14
      t19 = xi ** 2
      t23 = twoPi*(cc*xi+t19*cpwAlpha)
      t24 = sin(t23)
      t33 = 2.*xi*cpwAlpha+cc
      t35 = cos(t23)
      t41 = cpwAmp*(t5/2.-t12/2.)
      t46 = twoPi ** 2
      t47 = t33 ** 2
      chirp = cpwAmp*(t1*t12*t15-t1*t5*t8)*t24+2.*cpwAmp*(-cpwBeta*
     & t15/2.+cpwBeta*t8/2.)*twoPi*t33*t35+2.*t41*twoPi*cpwAlpha*t35-
     & t41*t46*t47*t24

                           else if(  numberOfTimeDerivatives.eq.3 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 3-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(-cpwBeta^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2+2*cpwBeta^3*tanh(cpwBeta*(xi+.5*cpwTau))^2*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2-2*cpwBeta^3*tanh(cpwBeta*(xi-.5*cpwTau))^2*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+3*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+6*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-3*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-6*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*cpwAlpha*(2*xi*cpwAlpha+cc)*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^3*(2*xi*cpwAlpha+cc)^3*cos(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = t1*cpwBeta
      t3 = .5*cpwTau
      t6 = tanh(cpwBeta*(xi+t3))
      t7 = t6 ** 2
      t8 = 1.-t7
      t9 = t8 ** 2
      t16 = tanh(cpwBeta*(xi-t3))
      t17 = t16 ** 2
      t18 = 1.-t17
      t19 = t18 ** 2
      t26 = xi ** 2
      t30 = twoPi*(cc*xi+t26*cpwAlpha)
      t31 = sin(t30)
      t41 = 2.*xi*cpwAlpha+cc
      t43 = cos(t30)
      t51 = cpwAmp*(-cpwBeta*t18/2.+cpwBeta*t8/2.)
      t56 = twoPi ** 2
      t57 = t41 ** 2
      t64 = cpwAmp*(t6/2.-t16/2.)
      chirp = cpwAmp*(-2.*t2*t17*t18+2.*t2*t7*t8+t2*t19-t2*t9)*t31+
     & 0.3E1*cpwAmp*(t1*t16*t18-t1*t6*t8)*twoPi*t41*t43+6.*t51*twoPi*
     & cpwAlpha*t43-0.3E1*t51*t56*t57*t31-6.*t64*t56*cpwAlpha*t41*t31-
     & t64*t56*twoPi*t57*t41*t43

                           else if(  numberOfTimeDerivatives.eq.4 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 4-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(8*cpwBeta^4*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2*tanh(cpwBeta*(xi+.5*cpwTau))-4*cpwBeta^4*tanh(cpwBeta*(xi+.5*cpwTau))^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-8*cpwBeta^4*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2*tanh(cpwBeta*(xi-.5*cpwTau))+4*cpwBeta^4*tanh(cpwBeta*(xi-.5*cpwTau))^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+4*cpwAmp*(-cpwBeta^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2+2*cpwBeta^3*tanh(cpwBeta*(xi+.5*cpwTau))^2*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2-2*cpwBeta^3*tanh(cpwBeta*(xi-.5*cpwTau))^2*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+12*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-6*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-24*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*cpwAlpha*(2*xi*cpwAlpha+cc)*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-4*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^3*(2*xi*cpwAlpha+cc)^3*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-12*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*cpwAlpha^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-12*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^3*cpwAlpha*(2*xi*cpwAlpha+cc)^2*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^4*(2*xi*cpwAlpha+cc)^4*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = t1 ** 2
      t3 = .5*cpwTau
      t6 = tanh(cpwBeta*(xi+t3))
      t7 = t6 ** 2
      t8 = 1.-t7
      t9 = t8 ** 2
      t19 = tanh(cpwBeta*(xi-t3))
      t20 = t19 ** 2
      t21 = 1.-t20
      t22 = t21 ** 2
      t32 = xi ** 2
      t36 = twoPi*(cc*xi+t32*cpwAlpha)
      t37 = sin(t36)
      t39 = t1*cpwBeta
      t52 = 2.*xi*cpwAlpha+cc
      t54 = cos(t36)
      t63 = cpwAmp*(t1*t19*t21-t1*t6*t8)
      t68 = twoPi ** 2
      t69 = t52 ** 2
      t78 = cpwAmp*(-cpwBeta*t21/2.+cpwBeta*t8/2.)
      t84 = t68*twoPi
      t92 = cpwAmp*(t6/2.-t19/2.)
      t93 = cpwAlpha ** 2
      t103 = t68 ** 2
      t104 = t69 ** 2
      chirp = cpwAmp*(4.*t2*t20*t19*t21-4.*t2*t7*t6*t8-8.*t2*t22*t19+
     & 8.*t2*t9*t6)*t37+4.*cpwAmp*(-2.*t39*t20*t21+2.*t39*t7*t8+t39*
     & t22-t39*t9)*twoPi*t52*t54+12.*t63*twoPi*cpwAlpha*t54-6.*t63*
     & t68*t69*t37-0.24E2*t78*t68*cpwAlpha*t52*t37-4.*t78*t84*t69*t52*
     & t54-12.*t92*t68*t93*t37-12.*t92*t84*cpwAlpha*t69*t54+t92*t103*
     & t104*t37

                           else
                             write(*,'(" getChirp2D:ERROR: too many 
     & derivatives requested")')
                             stop 4927
                           end if
                           ubv(ex) = chirp*pwc(0)
                           ubv(ey) = chirp*pwc(1)
                           ubv(hz) = chirp*pwc(5)
                        else
                          write(*,'("getBndryForcing2D: Unknown 
     & boundary forcing")')
                        end if
                      utp1 = -ubv(ex)
                      vtp1 = -ubv(ey)
                      numberOfTimeDerivatives=2+fieldOption
                        if( boundaryForcingOption.eq.noBoundaryForcing 
     & )then
                        else if( 
     & boundaryForcingOption.eq.planeWaveBoundaryForcing )then
                            if( numberOfTimeDerivatives==0 )then
                              ubv(ex) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==1 )then
                              ubv(ex) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(0)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(1)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(5)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==2 )then
                              ubv(ex) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==3 )then
                              ubv(ex) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==4 )then
                              ubv(ex) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))
                            else
                              stop 1738
                            end if
                        else if(  
     & boundaryForcingOption.eq.chirpedPlaneWaveBoundaryForcing )then
                           xi0 = .5*(cpwTa+cpwTb)
                           xi = t - (kx*(x0-cpwX0)+ky*(y0-cpwY0))/cc -
     & xi0
                           cpwTau=cpwTb-cpwTa  ! tau = tb -ta
                           ! include files generated by the maple code mx/codes/chirpedPlaneWave.maple 
                           if( numberOfTimeDerivatives.eq.0 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 0-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = .5*cpwTau
      t4 = tanh(cpwBeta*(xi+t1))
      t7 = tanh(cpwBeta*(xi-t1))
      t11 = xi ** 2
      t16 = sin(twoPi*(cc*xi+t11*cpwAlpha))
      chirp = cpwAmp*(t4/2.-t7/2.)*t16

                           else if(  numberOfTimeDerivatives.eq.1 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 1-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = .5*cpwTau
      t4 = tanh(cpwBeta*(xi+t1))
      t5 = t4 ** 2
      t10 = tanh(cpwBeta*(xi-t1))
      t11 = t10 ** 2
      t17 = xi ** 2
      t21 = twoPi*(cc*xi+t17*cpwAlpha)
      t22 = sin(t21)
      t31 = cos(t21)
      chirp = cpwAmp*(cpwBeta*(1.-t5)/2.-cpwBeta*(1.-t11)/2.)*t22+
     & cpwAmp*(t4/2.-t10/2.)*twoPi*(2.*xi*cpwAlpha+cc)*t31

                           else if(  numberOfTimeDerivatives.eq.2 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 2-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+2*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+2*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = .5*cpwTau
      t5 = tanh(cpwBeta*(xi+t2))
      t7 = t5 ** 2
      t8 = 1.-t7
      t12 = tanh(cpwBeta*(xi-t2))
      t14 = t12 ** 2
      t15 = 1.-t14
      t19 = xi ** 2
      t23 = twoPi*(cc*xi+t19*cpwAlpha)
      t24 = sin(t23)
      t33 = 2.*xi*cpwAlpha+cc
      t35 = cos(t23)
      t41 = cpwAmp*(t5/2.-t12/2.)
      t46 = twoPi ** 2
      t47 = t33 ** 2
      chirp = cpwAmp*(t1*t12*t15-t1*t5*t8)*t24+2.*cpwAmp*(-cpwBeta*
     & t15/2.+cpwBeta*t8/2.)*twoPi*t33*t35+2.*t41*twoPi*cpwAlpha*t35-
     & t41*t46*t47*t24

                           else if(  numberOfTimeDerivatives.eq.3 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 3-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(-cpwBeta^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2+2*cpwBeta^3*tanh(cpwBeta*(xi+.5*cpwTau))^2*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2-2*cpwBeta^3*tanh(cpwBeta*(xi-.5*cpwTau))^2*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+3*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+6*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-3*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-6*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*cpwAlpha*(2*xi*cpwAlpha+cc)*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^3*(2*xi*cpwAlpha+cc)^3*cos(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = t1*cpwBeta
      t3 = .5*cpwTau
      t6 = tanh(cpwBeta*(xi+t3))
      t7 = t6 ** 2
      t8 = 1.-t7
      t9 = t8 ** 2
      t16 = tanh(cpwBeta*(xi-t3))
      t17 = t16 ** 2
      t18 = 1.-t17
      t19 = t18 ** 2
      t26 = xi ** 2
      t30 = twoPi*(cc*xi+t26*cpwAlpha)
      t31 = sin(t30)
      t41 = 2.*xi*cpwAlpha+cc
      t43 = cos(t30)
      t51 = cpwAmp*(-cpwBeta*t18/2.+cpwBeta*t8/2.)
      t56 = twoPi ** 2
      t57 = t41 ** 2
      t64 = cpwAmp*(t6/2.-t16/2.)
      chirp = cpwAmp*(-2.*t2*t17*t18+2.*t2*t7*t8+t2*t19-t2*t9)*t31+
     & 0.3E1*cpwAmp*(t1*t16*t18-t1*t6*t8)*twoPi*t41*t43+6.*t51*twoPi*
     & cpwAlpha*t43-0.3E1*t51*t56*t57*t31-6.*t64*t56*cpwAlpha*t41*t31-
     & t64*t56*twoPi*t57*t41*t43

                           else if(  numberOfTimeDerivatives.eq.4 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 4-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(8*cpwBeta^4*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2*tanh(cpwBeta*(xi+.5*cpwTau))-4*cpwBeta^4*tanh(cpwBeta*(xi+.5*cpwTau))^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-8*cpwBeta^4*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2*tanh(cpwBeta*(xi-.5*cpwTau))+4*cpwBeta^4*tanh(cpwBeta*(xi-.5*cpwTau))^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+4*cpwAmp*(-cpwBeta^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2+2*cpwBeta^3*tanh(cpwBeta*(xi+.5*cpwTau))^2*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2-2*cpwBeta^3*tanh(cpwBeta*(xi-.5*cpwTau))^2*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+12*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-6*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-24*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*cpwAlpha*(2*xi*cpwAlpha+cc)*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-4*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^3*(2*xi*cpwAlpha+cc)^3*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-12*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*cpwAlpha^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-12*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^3*cpwAlpha*(2*xi*cpwAlpha+cc)^2*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^4*(2*xi*cpwAlpha+cc)^4*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = t1 ** 2
      t3 = .5*cpwTau
      t6 = tanh(cpwBeta*(xi+t3))
      t7 = t6 ** 2
      t8 = 1.-t7
      t9 = t8 ** 2
      t19 = tanh(cpwBeta*(xi-t3))
      t20 = t19 ** 2
      t21 = 1.-t20
      t22 = t21 ** 2
      t32 = xi ** 2
      t36 = twoPi*(cc*xi+t32*cpwAlpha)
      t37 = sin(t36)
      t39 = t1*cpwBeta
      t52 = 2.*xi*cpwAlpha+cc
      t54 = cos(t36)
      t63 = cpwAmp*(t1*t19*t21-t1*t6*t8)
      t68 = twoPi ** 2
      t69 = t52 ** 2
      t78 = cpwAmp*(-cpwBeta*t21/2.+cpwBeta*t8/2.)
      t84 = t68*twoPi
      t92 = cpwAmp*(t6/2.-t19/2.)
      t93 = cpwAlpha ** 2
      t103 = t68 ** 2
      t104 = t69 ** 2
      chirp = cpwAmp*(4.*t2*t20*t19*t21-4.*t2*t7*t6*t8-8.*t2*t22*t19+
     & 8.*t2*t9*t6)*t37+4.*cpwAmp*(-2.*t39*t20*t21+2.*t39*t7*t8+t39*
     & t22-t39*t9)*twoPi*t52*t54+12.*t63*twoPi*cpwAlpha*t54-6.*t63*
     & t68*t69*t37-0.24E2*t78*t68*cpwAlpha*t52*t37-4.*t78*t84*t69*t52*
     & t54-12.*t92*t68*t93*t37-12.*t92*t84*cpwAlpha*t69*t54+t92*t103*
     & t104*t37

                           else
                             write(*,'(" getChirp2D:ERROR: too many 
     & derivatives requested")')
                             stop 4927
                           end if
                           ubv(ex) = chirp*pwc(0)
                           ubv(ey) = chirp*pwc(1)
                           ubv(hz) = chirp*pwc(5)
                        else
                          write(*,'("getBndryForcing2D: Unknown 
     & boundary forcing")')
                        end if
                      uttp1 = -ubv(ex)
                      vttp1 = -ubv(ey)
                    else if( fieldOption.eq.0 )then ! *old way*
                      uttp1=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x0)+
     & ky*(y0)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(0)+ssftt*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(
     & t)))*pwc(0))
                      vttp1=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x0)+
     & ky*(y0)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(1)+ssftt*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(
     & t)))*pwc(1))
                      utp1 =-(ssf*(-twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(
     & y0)-cc*(t)))*pwc(0)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*
     & pwc(0))
                      vtp1 =-(ssf*(-twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(
     & y0)-cc*(t)))*pwc(1)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*
     & pwc(1))
                    else
                      ! we are assigning time derivatives (sosup)
                      uttp1=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x0)+ky*
     & (y0)-cc*(t)))*pwc(0))+3.*ssft*(-(twoPi*cc)**2*sin(twoPi*(kx*(
     & x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssftt*(-twoPi*cc)*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssfttt*sin(twoPi*(kx*(x0)+ky*(
     & y0)-cc*(t)))*pwc(0))
                      vttp1=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x0)+ky*
     & (y0)-cc*(t)))*pwc(1))+3.*ssft*(-(twoPi*cc)**2*sin(twoPi*(kx*(
     & x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssftt*(-twoPi*cc)*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssfttt*sin(twoPi*(kx*(x0)+ky*(
     & y0)-cc*(t)))*pwc(1))
                      utp1 =-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x0)+
     & ky*(y0)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(0)+ssftt*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(
     & t)))*pwc(0))
                      vtp1 =-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x0)+
     & ky*(y0)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(1)+ssftt*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(
     & t)))*pwc(1))
                    end if
                    x0=xy(i1-js1,i2-js2,i3,0)
                    y0=xy(i1-js1,i2-js2,i3,1)
                    if( .true. )then ! *new way*
                      numberOfTimeDerivatives=1+fieldOption
                        if( boundaryForcingOption.eq.noBoundaryForcing 
     & )then
                        else if( 
     & boundaryForcingOption.eq.planeWaveBoundaryForcing )then
                            if( numberOfTimeDerivatives==0 )then
                              ubv(ex) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==1 )then
                              ubv(ex) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(0)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(1)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(5)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==2 )then
                              ubv(ex) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==3 )then
                              ubv(ex) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==4 )then
                              ubv(ex) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))
                            else
                              stop 1738
                            end if
                        else if(  
     & boundaryForcingOption.eq.chirpedPlaneWaveBoundaryForcing )then
                           xi0 = .5*(cpwTa+cpwTb)
                           xi = t - (kx*(x0-cpwX0)+ky*(y0-cpwY0))/cc -
     & xi0
                           cpwTau=cpwTb-cpwTa  ! tau = tb -ta
                           ! include files generated by the maple code mx/codes/chirpedPlaneWave.maple 
                           if( numberOfTimeDerivatives.eq.0 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 0-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = .5*cpwTau
      t4 = tanh(cpwBeta*(xi+t1))
      t7 = tanh(cpwBeta*(xi-t1))
      t11 = xi ** 2
      t16 = sin(twoPi*(cc*xi+t11*cpwAlpha))
      chirp = cpwAmp*(t4/2.-t7/2.)*t16

                           else if(  numberOfTimeDerivatives.eq.1 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 1-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = .5*cpwTau
      t4 = tanh(cpwBeta*(xi+t1))
      t5 = t4 ** 2
      t10 = tanh(cpwBeta*(xi-t1))
      t11 = t10 ** 2
      t17 = xi ** 2
      t21 = twoPi*(cc*xi+t17*cpwAlpha)
      t22 = sin(t21)
      t31 = cos(t21)
      chirp = cpwAmp*(cpwBeta*(1.-t5)/2.-cpwBeta*(1.-t11)/2.)*t22+
     & cpwAmp*(t4/2.-t10/2.)*twoPi*(2.*xi*cpwAlpha+cc)*t31

                           else if(  numberOfTimeDerivatives.eq.2 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 2-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+2*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+2*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = .5*cpwTau
      t5 = tanh(cpwBeta*(xi+t2))
      t7 = t5 ** 2
      t8 = 1.-t7
      t12 = tanh(cpwBeta*(xi-t2))
      t14 = t12 ** 2
      t15 = 1.-t14
      t19 = xi ** 2
      t23 = twoPi*(cc*xi+t19*cpwAlpha)
      t24 = sin(t23)
      t33 = 2.*xi*cpwAlpha+cc
      t35 = cos(t23)
      t41 = cpwAmp*(t5/2.-t12/2.)
      t46 = twoPi ** 2
      t47 = t33 ** 2
      chirp = cpwAmp*(t1*t12*t15-t1*t5*t8)*t24+2.*cpwAmp*(-cpwBeta*
     & t15/2.+cpwBeta*t8/2.)*twoPi*t33*t35+2.*t41*twoPi*cpwAlpha*t35-
     & t41*t46*t47*t24

                           else if(  numberOfTimeDerivatives.eq.3 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 3-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(-cpwBeta^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2+2*cpwBeta^3*tanh(cpwBeta*(xi+.5*cpwTau))^2*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2-2*cpwBeta^3*tanh(cpwBeta*(xi-.5*cpwTau))^2*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+3*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+6*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-3*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-6*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*cpwAlpha*(2*xi*cpwAlpha+cc)*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^3*(2*xi*cpwAlpha+cc)^3*cos(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = t1*cpwBeta
      t3 = .5*cpwTau
      t6 = tanh(cpwBeta*(xi+t3))
      t7 = t6 ** 2
      t8 = 1.-t7
      t9 = t8 ** 2
      t16 = tanh(cpwBeta*(xi-t3))
      t17 = t16 ** 2
      t18 = 1.-t17
      t19 = t18 ** 2
      t26 = xi ** 2
      t30 = twoPi*(cc*xi+t26*cpwAlpha)
      t31 = sin(t30)
      t41 = 2.*xi*cpwAlpha+cc
      t43 = cos(t30)
      t51 = cpwAmp*(-cpwBeta*t18/2.+cpwBeta*t8/2.)
      t56 = twoPi ** 2
      t57 = t41 ** 2
      t64 = cpwAmp*(t6/2.-t16/2.)
      chirp = cpwAmp*(-2.*t2*t17*t18+2.*t2*t7*t8+t2*t19-t2*t9)*t31+
     & 0.3E1*cpwAmp*(t1*t16*t18-t1*t6*t8)*twoPi*t41*t43+6.*t51*twoPi*
     & cpwAlpha*t43-0.3E1*t51*t56*t57*t31-6.*t64*t56*cpwAlpha*t41*t31-
     & t64*t56*twoPi*t57*t41*t43

                           else if(  numberOfTimeDerivatives.eq.4 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 4-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(8*cpwBeta^4*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2*tanh(cpwBeta*(xi+.5*cpwTau))-4*cpwBeta^4*tanh(cpwBeta*(xi+.5*cpwTau))^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-8*cpwBeta^4*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2*tanh(cpwBeta*(xi-.5*cpwTau))+4*cpwBeta^4*tanh(cpwBeta*(xi-.5*cpwTau))^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+4*cpwAmp*(-cpwBeta^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2+2*cpwBeta^3*tanh(cpwBeta*(xi+.5*cpwTau))^2*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2-2*cpwBeta^3*tanh(cpwBeta*(xi-.5*cpwTau))^2*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+12*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-6*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-24*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*cpwAlpha*(2*xi*cpwAlpha+cc)*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-4*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^3*(2*xi*cpwAlpha+cc)^3*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-12*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*cpwAlpha^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-12*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^3*cpwAlpha*(2*xi*cpwAlpha+cc)^2*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^4*(2*xi*cpwAlpha+cc)^4*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = t1 ** 2
      t3 = .5*cpwTau
      t6 = tanh(cpwBeta*(xi+t3))
      t7 = t6 ** 2
      t8 = 1.-t7
      t9 = t8 ** 2
      t19 = tanh(cpwBeta*(xi-t3))
      t20 = t19 ** 2
      t21 = 1.-t20
      t22 = t21 ** 2
      t32 = xi ** 2
      t36 = twoPi*(cc*xi+t32*cpwAlpha)
      t37 = sin(t36)
      t39 = t1*cpwBeta
      t52 = 2.*xi*cpwAlpha+cc
      t54 = cos(t36)
      t63 = cpwAmp*(t1*t19*t21-t1*t6*t8)
      t68 = twoPi ** 2
      t69 = t52 ** 2
      t78 = cpwAmp*(-cpwBeta*t21/2.+cpwBeta*t8/2.)
      t84 = t68*twoPi
      t92 = cpwAmp*(t6/2.-t19/2.)
      t93 = cpwAlpha ** 2
      t103 = t68 ** 2
      t104 = t69 ** 2
      chirp = cpwAmp*(4.*t2*t20*t19*t21-4.*t2*t7*t6*t8-8.*t2*t22*t19+
     & 8.*t2*t9*t6)*t37+4.*cpwAmp*(-2.*t39*t20*t21+2.*t39*t7*t8+t39*
     & t22-t39*t9)*twoPi*t52*t54+12.*t63*twoPi*cpwAlpha*t54-6.*t63*
     & t68*t69*t37-0.24E2*t78*t68*cpwAlpha*t52*t37-4.*t78*t84*t69*t52*
     & t54-12.*t92*t68*t93*t37-12.*t92*t84*cpwAlpha*t69*t54+t92*t103*
     & t104*t37

                           else
                             write(*,'(" getChirp2D:ERROR: too many 
     & derivatives requested")')
                             stop 4927
                           end if
                           ubv(ex) = chirp*pwc(0)
                           ubv(ey) = chirp*pwc(1)
                           ubv(hz) = chirp*pwc(5)
                        else
                          write(*,'("getBndryForcing2D: Unknown 
     & boundary forcing")')
                        end if
                      utm1 = -ubv(ex)
                      vtm1 = -ubv(ey)
                      numberOfTimeDerivatives=2+fieldOption
                        if( boundaryForcingOption.eq.noBoundaryForcing 
     & )then
                        else if( 
     & boundaryForcingOption.eq.planeWaveBoundaryForcing )then
                            if( numberOfTimeDerivatives==0 )then
                              ubv(ex) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==1 )then
                              ubv(ex) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(0)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(1)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(5)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==2 )then
                              ubv(ex) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==3 )then
                              ubv(ex) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==4 )then
                              ubv(ex) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))
                            else
                              stop 1738
                            end if
                        else if(  
     & boundaryForcingOption.eq.chirpedPlaneWaveBoundaryForcing )then
                           xi0 = .5*(cpwTa+cpwTb)
                           xi = t - (kx*(x0-cpwX0)+ky*(y0-cpwY0))/cc -
     & xi0
                           cpwTau=cpwTb-cpwTa  ! tau = tb -ta
                           ! include files generated by the maple code mx/codes/chirpedPlaneWave.maple 
                           if( numberOfTimeDerivatives.eq.0 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 0-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = .5*cpwTau
      t4 = tanh(cpwBeta*(xi+t1))
      t7 = tanh(cpwBeta*(xi-t1))
      t11 = xi ** 2
      t16 = sin(twoPi*(cc*xi+t11*cpwAlpha))
      chirp = cpwAmp*(t4/2.-t7/2.)*t16

                           else if(  numberOfTimeDerivatives.eq.1 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 1-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = .5*cpwTau
      t4 = tanh(cpwBeta*(xi+t1))
      t5 = t4 ** 2
      t10 = tanh(cpwBeta*(xi-t1))
      t11 = t10 ** 2
      t17 = xi ** 2
      t21 = twoPi*(cc*xi+t17*cpwAlpha)
      t22 = sin(t21)
      t31 = cos(t21)
      chirp = cpwAmp*(cpwBeta*(1.-t5)/2.-cpwBeta*(1.-t11)/2.)*t22+
     & cpwAmp*(t4/2.-t10/2.)*twoPi*(2.*xi*cpwAlpha+cc)*t31

                           else if(  numberOfTimeDerivatives.eq.2 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 2-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+2*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+2*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = .5*cpwTau
      t5 = tanh(cpwBeta*(xi+t2))
      t7 = t5 ** 2
      t8 = 1.-t7
      t12 = tanh(cpwBeta*(xi-t2))
      t14 = t12 ** 2
      t15 = 1.-t14
      t19 = xi ** 2
      t23 = twoPi*(cc*xi+t19*cpwAlpha)
      t24 = sin(t23)
      t33 = 2.*xi*cpwAlpha+cc
      t35 = cos(t23)
      t41 = cpwAmp*(t5/2.-t12/2.)
      t46 = twoPi ** 2
      t47 = t33 ** 2
      chirp = cpwAmp*(t1*t12*t15-t1*t5*t8)*t24+2.*cpwAmp*(-cpwBeta*
     & t15/2.+cpwBeta*t8/2.)*twoPi*t33*t35+2.*t41*twoPi*cpwAlpha*t35-
     & t41*t46*t47*t24

                           else if(  numberOfTimeDerivatives.eq.3 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 3-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(-cpwBeta^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2+2*cpwBeta^3*tanh(cpwBeta*(xi+.5*cpwTau))^2*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2-2*cpwBeta^3*tanh(cpwBeta*(xi-.5*cpwTau))^2*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+3*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+6*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-3*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-6*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*cpwAlpha*(2*xi*cpwAlpha+cc)*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^3*(2*xi*cpwAlpha+cc)^3*cos(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = t1*cpwBeta
      t3 = .5*cpwTau
      t6 = tanh(cpwBeta*(xi+t3))
      t7 = t6 ** 2
      t8 = 1.-t7
      t9 = t8 ** 2
      t16 = tanh(cpwBeta*(xi-t3))
      t17 = t16 ** 2
      t18 = 1.-t17
      t19 = t18 ** 2
      t26 = xi ** 2
      t30 = twoPi*(cc*xi+t26*cpwAlpha)
      t31 = sin(t30)
      t41 = 2.*xi*cpwAlpha+cc
      t43 = cos(t30)
      t51 = cpwAmp*(-cpwBeta*t18/2.+cpwBeta*t8/2.)
      t56 = twoPi ** 2
      t57 = t41 ** 2
      t64 = cpwAmp*(t6/2.-t16/2.)
      chirp = cpwAmp*(-2.*t2*t17*t18+2.*t2*t7*t8+t2*t19-t2*t9)*t31+
     & 0.3E1*cpwAmp*(t1*t16*t18-t1*t6*t8)*twoPi*t41*t43+6.*t51*twoPi*
     & cpwAlpha*t43-0.3E1*t51*t56*t57*t31-6.*t64*t56*cpwAlpha*t41*t31-
     & t64*t56*twoPi*t57*t41*t43

                           else if(  numberOfTimeDerivatives.eq.4 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 4-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(8*cpwBeta^4*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2*tanh(cpwBeta*(xi+.5*cpwTau))-4*cpwBeta^4*tanh(cpwBeta*(xi+.5*cpwTau))^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-8*cpwBeta^4*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2*tanh(cpwBeta*(xi-.5*cpwTau))+4*cpwBeta^4*tanh(cpwBeta*(xi-.5*cpwTau))^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+4*cpwAmp*(-cpwBeta^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2+2*cpwBeta^3*tanh(cpwBeta*(xi+.5*cpwTau))^2*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2-2*cpwBeta^3*tanh(cpwBeta*(xi-.5*cpwTau))^2*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+12*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-6*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-24*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*cpwAlpha*(2*xi*cpwAlpha+cc)*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-4*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^3*(2*xi*cpwAlpha+cc)^3*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-12*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*cpwAlpha^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-12*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^3*cpwAlpha*(2*xi*cpwAlpha+cc)^2*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^4*(2*xi*cpwAlpha+cc)^4*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = t1 ** 2
      t3 = .5*cpwTau
      t6 = tanh(cpwBeta*(xi+t3))
      t7 = t6 ** 2
      t8 = 1.-t7
      t9 = t8 ** 2
      t19 = tanh(cpwBeta*(xi-t3))
      t20 = t19 ** 2
      t21 = 1.-t20
      t22 = t21 ** 2
      t32 = xi ** 2
      t36 = twoPi*(cc*xi+t32*cpwAlpha)
      t37 = sin(t36)
      t39 = t1*cpwBeta
      t52 = 2.*xi*cpwAlpha+cc
      t54 = cos(t36)
      t63 = cpwAmp*(t1*t19*t21-t1*t6*t8)
      t68 = twoPi ** 2
      t69 = t52 ** 2
      t78 = cpwAmp*(-cpwBeta*t21/2.+cpwBeta*t8/2.)
      t84 = t68*twoPi
      t92 = cpwAmp*(t6/2.-t19/2.)
      t93 = cpwAlpha ** 2
      t103 = t68 ** 2
      t104 = t69 ** 2
      chirp = cpwAmp*(4.*t2*t20*t19*t21-4.*t2*t7*t6*t8-8.*t2*t22*t19+
     & 8.*t2*t9*t6)*t37+4.*cpwAmp*(-2.*t39*t20*t21+2.*t39*t7*t8+t39*
     & t22-t39*t9)*twoPi*t52*t54+12.*t63*twoPi*cpwAlpha*t54-6.*t63*
     & t68*t69*t37-0.24E2*t78*t68*cpwAlpha*t52*t37-4.*t78*t84*t69*t52*
     & t54-12.*t92*t68*t93*t37-12.*t92*t84*cpwAlpha*t69*t54+t92*t103*
     & t104*t37

                           else
                             write(*,'(" getChirp2D:ERROR: too many 
     & derivatives requested")')
                             stop 4927
                           end if
                           ubv(ex) = chirp*pwc(0)
                           ubv(ey) = chirp*pwc(1)
                           ubv(hz) = chirp*pwc(5)
                        else
                          write(*,'("getBndryForcing2D: Unknown 
     & boundary forcing")')
                        end if
                      uttm1 = -ubv(ex)
                      vttm1 = -ubv(ey)
                    else if( fieldOption.eq.0 )then ! *old way*
                      uttm1=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x0)+
     & ky*(y0)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(0)+ssftt*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(
     & t)))*pwc(0))
                      vttm1=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x0)+
     & ky*(y0)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(1)+ssftt*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(
     & t)))*pwc(1))
                      utm1 =-(ssf*(-twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(
     & y0)-cc*(t)))*pwc(0)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*
     & pwc(0))
                      vtm1 =-(ssf*(-twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(
     & y0)-cc*(t)))*pwc(1)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*
     & pwc(1))
                    else
                      ! we are assigning time derivatives (sosup)
                      uttm1=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x0)+ky*
     & (y0)-cc*(t)))*pwc(0))+3.*ssft*(-(twoPi*cc)**2*sin(twoPi*(kx*(
     & x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssftt*(-twoPi*cc)*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssfttt*sin(twoPi*(kx*(x0)+ky*(
     & y0)-cc*(t)))*pwc(0))
                      vttm1=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x0)+ky*
     & (y0)-cc*(t)))*pwc(1))+3.*ssft*(-(twoPi*cc)**2*sin(twoPi*(kx*(
     & x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssftt*(-twoPi*cc)*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssfttt*sin(twoPi*(kx*(x0)+ky*(
     & y0)-cc*(t)))*pwc(1))
                      utm1 =-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x0)+
     & ky*(y0)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(0)+ssftt*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(
     & t)))*pwc(0))
                      vtm1 =-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x0)+
     & ky*(y0)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(1)+ssftt*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(
     & t)))*pwc(1))
                    end if
                    x0=xy(i1+2*js1,i2+2*js2,i3,0)
                    y0=xy(i1+2*js1,i2+2*js2,i3,1)
                    if( .true. )then ! *new way*
                      numberOfTimeDerivatives=2+fieldOption
                        if( boundaryForcingOption.eq.noBoundaryForcing 
     & )then
                        else if( 
     & boundaryForcingOption.eq.planeWaveBoundaryForcing )then
                            if( numberOfTimeDerivatives==0 )then
                              ubv(ex) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==1 )then
                              ubv(ex) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(0)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(1)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(5)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==2 )then
                              ubv(ex) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==3 )then
                              ubv(ex) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==4 )then
                              ubv(ex) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))
                            else
                              stop 1738
                            end if
                        else if(  
     & boundaryForcingOption.eq.chirpedPlaneWaveBoundaryForcing )then
                           xi0 = .5*(cpwTa+cpwTb)
                           xi = t - (kx*(x0-cpwX0)+ky*(y0-cpwY0))/cc -
     & xi0
                           cpwTau=cpwTb-cpwTa  ! tau = tb -ta
                           ! include files generated by the maple code mx/codes/chirpedPlaneWave.maple 
                           if( numberOfTimeDerivatives.eq.0 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 0-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = .5*cpwTau
      t4 = tanh(cpwBeta*(xi+t1))
      t7 = tanh(cpwBeta*(xi-t1))
      t11 = xi ** 2
      t16 = sin(twoPi*(cc*xi+t11*cpwAlpha))
      chirp = cpwAmp*(t4/2.-t7/2.)*t16

                           else if(  numberOfTimeDerivatives.eq.1 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 1-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = .5*cpwTau
      t4 = tanh(cpwBeta*(xi+t1))
      t5 = t4 ** 2
      t10 = tanh(cpwBeta*(xi-t1))
      t11 = t10 ** 2
      t17 = xi ** 2
      t21 = twoPi*(cc*xi+t17*cpwAlpha)
      t22 = sin(t21)
      t31 = cos(t21)
      chirp = cpwAmp*(cpwBeta*(1.-t5)/2.-cpwBeta*(1.-t11)/2.)*t22+
     & cpwAmp*(t4/2.-t10/2.)*twoPi*(2.*xi*cpwAlpha+cc)*t31

                           else if(  numberOfTimeDerivatives.eq.2 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 2-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+2*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+2*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = .5*cpwTau
      t5 = tanh(cpwBeta*(xi+t2))
      t7 = t5 ** 2
      t8 = 1.-t7
      t12 = tanh(cpwBeta*(xi-t2))
      t14 = t12 ** 2
      t15 = 1.-t14
      t19 = xi ** 2
      t23 = twoPi*(cc*xi+t19*cpwAlpha)
      t24 = sin(t23)
      t33 = 2.*xi*cpwAlpha+cc
      t35 = cos(t23)
      t41 = cpwAmp*(t5/2.-t12/2.)
      t46 = twoPi ** 2
      t47 = t33 ** 2
      chirp = cpwAmp*(t1*t12*t15-t1*t5*t8)*t24+2.*cpwAmp*(-cpwBeta*
     & t15/2.+cpwBeta*t8/2.)*twoPi*t33*t35+2.*t41*twoPi*cpwAlpha*t35-
     & t41*t46*t47*t24

                           else if(  numberOfTimeDerivatives.eq.3 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 3-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(-cpwBeta^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2+2*cpwBeta^3*tanh(cpwBeta*(xi+.5*cpwTau))^2*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2-2*cpwBeta^3*tanh(cpwBeta*(xi-.5*cpwTau))^2*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+3*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+6*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-3*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-6*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*cpwAlpha*(2*xi*cpwAlpha+cc)*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^3*(2*xi*cpwAlpha+cc)^3*cos(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = t1*cpwBeta
      t3 = .5*cpwTau
      t6 = tanh(cpwBeta*(xi+t3))
      t7 = t6 ** 2
      t8 = 1.-t7
      t9 = t8 ** 2
      t16 = tanh(cpwBeta*(xi-t3))
      t17 = t16 ** 2
      t18 = 1.-t17
      t19 = t18 ** 2
      t26 = xi ** 2
      t30 = twoPi*(cc*xi+t26*cpwAlpha)
      t31 = sin(t30)
      t41 = 2.*xi*cpwAlpha+cc
      t43 = cos(t30)
      t51 = cpwAmp*(-cpwBeta*t18/2.+cpwBeta*t8/2.)
      t56 = twoPi ** 2
      t57 = t41 ** 2
      t64 = cpwAmp*(t6/2.-t16/2.)
      chirp = cpwAmp*(-2.*t2*t17*t18+2.*t2*t7*t8+t2*t19-t2*t9)*t31+
     & 0.3E1*cpwAmp*(t1*t16*t18-t1*t6*t8)*twoPi*t41*t43+6.*t51*twoPi*
     & cpwAlpha*t43-0.3E1*t51*t56*t57*t31-6.*t64*t56*cpwAlpha*t41*t31-
     & t64*t56*twoPi*t57*t41*t43

                           else if(  numberOfTimeDerivatives.eq.4 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 4-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(8*cpwBeta^4*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2*tanh(cpwBeta*(xi+.5*cpwTau))-4*cpwBeta^4*tanh(cpwBeta*(xi+.5*cpwTau))^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-8*cpwBeta^4*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2*tanh(cpwBeta*(xi-.5*cpwTau))+4*cpwBeta^4*tanh(cpwBeta*(xi-.5*cpwTau))^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+4*cpwAmp*(-cpwBeta^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2+2*cpwBeta^3*tanh(cpwBeta*(xi+.5*cpwTau))^2*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2-2*cpwBeta^3*tanh(cpwBeta*(xi-.5*cpwTau))^2*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+12*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-6*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-24*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*cpwAlpha*(2*xi*cpwAlpha+cc)*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-4*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^3*(2*xi*cpwAlpha+cc)^3*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-12*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*cpwAlpha^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-12*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^3*cpwAlpha*(2*xi*cpwAlpha+cc)^2*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^4*(2*xi*cpwAlpha+cc)^4*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = t1 ** 2
      t3 = .5*cpwTau
      t6 = tanh(cpwBeta*(xi+t3))
      t7 = t6 ** 2
      t8 = 1.-t7
      t9 = t8 ** 2
      t19 = tanh(cpwBeta*(xi-t3))
      t20 = t19 ** 2
      t21 = 1.-t20
      t22 = t21 ** 2
      t32 = xi ** 2
      t36 = twoPi*(cc*xi+t32*cpwAlpha)
      t37 = sin(t36)
      t39 = t1*cpwBeta
      t52 = 2.*xi*cpwAlpha+cc
      t54 = cos(t36)
      t63 = cpwAmp*(t1*t19*t21-t1*t6*t8)
      t68 = twoPi ** 2
      t69 = t52 ** 2
      t78 = cpwAmp*(-cpwBeta*t21/2.+cpwBeta*t8/2.)
      t84 = t68*twoPi
      t92 = cpwAmp*(t6/2.-t19/2.)
      t93 = cpwAlpha ** 2
      t103 = t68 ** 2
      t104 = t69 ** 2
      chirp = cpwAmp*(4.*t2*t20*t19*t21-4.*t2*t7*t6*t8-8.*t2*t22*t19+
     & 8.*t2*t9*t6)*t37+4.*cpwAmp*(-2.*t39*t20*t21+2.*t39*t7*t8+t39*
     & t22-t39*t9)*twoPi*t52*t54+12.*t63*twoPi*cpwAlpha*t54-6.*t63*
     & t68*t69*t37-0.24E2*t78*t68*cpwAlpha*t52*t37-4.*t78*t84*t69*t52*
     & t54-12.*t92*t68*t93*t37-12.*t92*t84*cpwAlpha*t69*t54+t92*t103*
     & t104*t37

                           else
                             write(*,'(" getChirp2D:ERROR: too many 
     & derivatives requested")')
                             stop 4927
                           end if
                           ubv(ex) = chirp*pwc(0)
                           ubv(ey) = chirp*pwc(1)
                           ubv(hz) = chirp*pwc(5)
                        else
                          write(*,'("getBndryForcing2D: Unknown 
     & boundary forcing")')
                        end if
                      uttp2 = -ubv(ex)
                      vttp2 = -ubv(ey)
                    else if( fieldOption.eq.0 )then ! *old way*
                      uttp2=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x0)+
     & ky*(y0)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(0)+ssftt*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(
     & t)))*pwc(0))
                      vttp2=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x0)+
     & ky*(y0)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(1)+ssftt*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(
     & t)))*pwc(1))
                    else
                      ! we are assigning time derivatives (sosup)
                      uttp2=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x0)+ky*
     & (y0)-cc*(t)))*pwc(0))+3.*ssft*(-(twoPi*cc)**2*sin(twoPi*(kx*(
     & x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssftt*(-twoPi*cc)*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssfttt*sin(twoPi*(kx*(x0)+ky*(
     & y0)-cc*(t)))*pwc(0))
                      vttp2=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x0)+ky*
     & (y0)-cc*(t)))*pwc(1))+3.*ssft*(-(twoPi*cc)**2*sin(twoPi*(kx*(
     & x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssftt*(-twoPi*cc)*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssfttt*sin(twoPi*(kx*(x0)+ky*(
     & y0)-cc*(t)))*pwc(1))
                    end if
                    x0=xy(i1-2*js1,i2-2*js2,i3,0)
                    y0=xy(i1-2*js1,i2-2*js2,i3,1)
                    if( .true. )then ! *new way*
                      numberOfTimeDerivatives=2+fieldOption
                        if( boundaryForcingOption.eq.noBoundaryForcing 
     & )then
                        else if( 
     & boundaryForcingOption.eq.planeWaveBoundaryForcing )then
                            if( numberOfTimeDerivatives==0 )then
                              ubv(ex) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==1 )then
                              ubv(ex) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(0)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(1)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(5)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==2 )then
                              ubv(ex) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==3 )then
                              ubv(ex) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==4 )then
                              ubv(ex) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))
                            else
                              stop 1738
                            end if
                        else if(  
     & boundaryForcingOption.eq.chirpedPlaneWaveBoundaryForcing )then
                           xi0 = .5*(cpwTa+cpwTb)
                           xi = t - (kx*(x0-cpwX0)+ky*(y0-cpwY0))/cc -
     & xi0
                           cpwTau=cpwTb-cpwTa  ! tau = tb -ta
                           ! include files generated by the maple code mx/codes/chirpedPlaneWave.maple 
                           if( numberOfTimeDerivatives.eq.0 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 0-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = .5*cpwTau
      t4 = tanh(cpwBeta*(xi+t1))
      t7 = tanh(cpwBeta*(xi-t1))
      t11 = xi ** 2
      t16 = sin(twoPi*(cc*xi+t11*cpwAlpha))
      chirp = cpwAmp*(t4/2.-t7/2.)*t16

                           else if(  numberOfTimeDerivatives.eq.1 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 1-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = .5*cpwTau
      t4 = tanh(cpwBeta*(xi+t1))
      t5 = t4 ** 2
      t10 = tanh(cpwBeta*(xi-t1))
      t11 = t10 ** 2
      t17 = xi ** 2
      t21 = twoPi*(cc*xi+t17*cpwAlpha)
      t22 = sin(t21)
      t31 = cos(t21)
      chirp = cpwAmp*(cpwBeta*(1.-t5)/2.-cpwBeta*(1.-t11)/2.)*t22+
     & cpwAmp*(t4/2.-t10/2.)*twoPi*(2.*xi*cpwAlpha+cc)*t31

                           else if(  numberOfTimeDerivatives.eq.2 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 2-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+2*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+2*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = .5*cpwTau
      t5 = tanh(cpwBeta*(xi+t2))
      t7 = t5 ** 2
      t8 = 1.-t7
      t12 = tanh(cpwBeta*(xi-t2))
      t14 = t12 ** 2
      t15 = 1.-t14
      t19 = xi ** 2
      t23 = twoPi*(cc*xi+t19*cpwAlpha)
      t24 = sin(t23)
      t33 = 2.*xi*cpwAlpha+cc
      t35 = cos(t23)
      t41 = cpwAmp*(t5/2.-t12/2.)
      t46 = twoPi ** 2
      t47 = t33 ** 2
      chirp = cpwAmp*(t1*t12*t15-t1*t5*t8)*t24+2.*cpwAmp*(-cpwBeta*
     & t15/2.+cpwBeta*t8/2.)*twoPi*t33*t35+2.*t41*twoPi*cpwAlpha*t35-
     & t41*t46*t47*t24

                           else if(  numberOfTimeDerivatives.eq.3 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 3-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(-cpwBeta^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2+2*cpwBeta^3*tanh(cpwBeta*(xi+.5*cpwTau))^2*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2-2*cpwBeta^3*tanh(cpwBeta*(xi-.5*cpwTau))^2*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+3*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+6*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-3*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-6*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*cpwAlpha*(2*xi*cpwAlpha+cc)*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^3*(2*xi*cpwAlpha+cc)^3*cos(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = t1*cpwBeta
      t3 = .5*cpwTau
      t6 = tanh(cpwBeta*(xi+t3))
      t7 = t6 ** 2
      t8 = 1.-t7
      t9 = t8 ** 2
      t16 = tanh(cpwBeta*(xi-t3))
      t17 = t16 ** 2
      t18 = 1.-t17
      t19 = t18 ** 2
      t26 = xi ** 2
      t30 = twoPi*(cc*xi+t26*cpwAlpha)
      t31 = sin(t30)
      t41 = 2.*xi*cpwAlpha+cc
      t43 = cos(t30)
      t51 = cpwAmp*(-cpwBeta*t18/2.+cpwBeta*t8/2.)
      t56 = twoPi ** 2
      t57 = t41 ** 2
      t64 = cpwAmp*(t6/2.-t16/2.)
      chirp = cpwAmp*(-2.*t2*t17*t18+2.*t2*t7*t8+t2*t19-t2*t9)*t31+
     & 0.3E1*cpwAmp*(t1*t16*t18-t1*t6*t8)*twoPi*t41*t43+6.*t51*twoPi*
     & cpwAlpha*t43-0.3E1*t51*t56*t57*t31-6.*t64*t56*cpwAlpha*t41*t31-
     & t64*t56*twoPi*t57*t41*t43

                           else if(  numberOfTimeDerivatives.eq.4 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 4-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(8*cpwBeta^4*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2*tanh(cpwBeta*(xi+.5*cpwTau))-4*cpwBeta^4*tanh(cpwBeta*(xi+.5*cpwTau))^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-8*cpwBeta^4*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2*tanh(cpwBeta*(xi-.5*cpwTau))+4*cpwBeta^4*tanh(cpwBeta*(xi-.5*cpwTau))^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+4*cpwAmp*(-cpwBeta^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2+2*cpwBeta^3*tanh(cpwBeta*(xi+.5*cpwTau))^2*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2-2*cpwBeta^3*tanh(cpwBeta*(xi-.5*cpwTau))^2*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+12*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-6*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-24*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*cpwAlpha*(2*xi*cpwAlpha+cc)*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-4*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^3*(2*xi*cpwAlpha+cc)^3*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-12*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*cpwAlpha^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-12*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^3*cpwAlpha*(2*xi*cpwAlpha+cc)^2*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^4*(2*xi*cpwAlpha+cc)^4*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = t1 ** 2
      t3 = .5*cpwTau
      t6 = tanh(cpwBeta*(xi+t3))
      t7 = t6 ** 2
      t8 = 1.-t7
      t9 = t8 ** 2
      t19 = tanh(cpwBeta*(xi-t3))
      t20 = t19 ** 2
      t21 = 1.-t20
      t22 = t21 ** 2
      t32 = xi ** 2
      t36 = twoPi*(cc*xi+t32*cpwAlpha)
      t37 = sin(t36)
      t39 = t1*cpwBeta
      t52 = 2.*xi*cpwAlpha+cc
      t54 = cos(t36)
      t63 = cpwAmp*(t1*t19*t21-t1*t6*t8)
      t68 = twoPi ** 2
      t69 = t52 ** 2
      t78 = cpwAmp*(-cpwBeta*t21/2.+cpwBeta*t8/2.)
      t84 = t68*twoPi
      t92 = cpwAmp*(t6/2.-t19/2.)
      t93 = cpwAlpha ** 2
      t103 = t68 ** 2
      t104 = t69 ** 2
      chirp = cpwAmp*(4.*t2*t20*t19*t21-4.*t2*t7*t6*t8-8.*t2*t22*t19+
     & 8.*t2*t9*t6)*t37+4.*cpwAmp*(-2.*t39*t20*t21+2.*t39*t7*t8+t39*
     & t22-t39*t9)*twoPi*t52*t54+12.*t63*twoPi*cpwAlpha*t54-6.*t63*
     & t68*t69*t37-0.24E2*t78*t68*cpwAlpha*t52*t37-4.*t78*t84*t69*t52*
     & t54-12.*t92*t68*t93*t37-12.*t92*t84*cpwAlpha*t69*t54+t92*t103*
     & t104*t37

                           else
                             write(*,'(" getChirp2D:ERROR: too many 
     & derivatives requested")')
                             stop 4927
                           end if
                           ubv(ex) = chirp*pwc(0)
                           ubv(ey) = chirp*pwc(1)
                           ubv(hz) = chirp*pwc(5)
                        else
                          write(*,'("getBndryForcing2D: Unknown 
     & boundary forcing")')
                        end if
                      uttm2 = -ubv(ex)
                      vttm2 = -ubv(ey)
                    else if( fieldOption.eq.0 )then ! *old way*
                      uttm2=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x0)+
     & ky*(y0)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(0)+ssftt*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(
     & t)))*pwc(0))
                      vttm2=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x0)+
     & ky*(y0)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(1)+ssftt*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(
     & t)))*pwc(1))
                    else
                      ! we are assigning time derivatives (sosup)
                      uttm2=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x0)+ky*
     & (y0)-cc*(t)))*pwc(0))+3.*ssft*(-(twoPi*cc)**2*sin(twoPi*(kx*(
     & x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssftt*(-twoPi*cc)*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssfttt*sin(twoPi*(kx*(x0)+ky*(
     & y0)-cc*(t)))*pwc(0))
                      vttm2=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x0)+ky*
     & (y0)-cc*(t)))*pwc(1))+3.*ssft*(-(twoPi*cc)**2*sin(twoPi*(kx*(
     & x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssftt*(-twoPi*cc)*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssfttt*sin(twoPi*(kx*(x0)+ky*(
     & y0)-cc*(t)))*pwc(1))
                    end if
                    utts = (8.*(uttp1-uttm1)-(uttp2-uttm2) )/(12.*dsa)
                    vtts = (8.*(vttp1-vttm1)-(vttp2-vttm2) )/(12.*dsa)
                    bf = bf + a21s*utt00+a22s*vtt00 + a21*utts + a22*
     & vtts
                    ! ***** Forcing for Hz ******
                    ! (w).r = fw1                              (w.n = 0 )
                    ! (c11*w.rr + c22*w.ss + ... ).r = fw2     ( (Delta w).n = 0 )
                    ! *** for planeWaveBoundaryForcing we need to use: u.t=w.y and v.t=-w.x =>
                    ! *****  (n1,n2).(w.x,w.y) = -n1*v.t + n2*u.t
                    !  OR    (rx,ry).(w.x,w.y) = -rx*v.t + ry*u.t
                    !   (rx**2+ry**2) w.r + (rx*sx+ry*sy)*ws = -rx*vt + ry*ut 
                    ! Note: the first term here (rx*sx+ry*sy) will be zero on an orthogonal grid
                     fw1=(-(rsxy(i1,i2,i3,axis,0)*rsxy(i1,i2,i3,axisp1,
     & 0)+rsxy(i1,i2,i3,axis,1)*rsxy(i1,i2,i3,axisp1,1))*ws-rsxy(i1,
     & i2,i3,axis,0)*vt0+rsxy(i1,i2,i3,axis,1)*ut0)/(rsxy(i1,i2,i3,
     & axis,0)**2+rsxy(i1,i2,i3,axis,1)**2)
                 !$$$   fw1=( -(rsxy(i1,i2,i3,axis,0)*rsxy(i1,i2,i3,axisp1,0)+rsxy(i1,i2,i3,axis,1)*rsxy(i1,i2,i3,axisp1,1))*ws !$$$        - rsxy(i1,i2,i3,axis,0)*vt0 + rsxy(i1,i2,i3,axis,1)*ut0 !$$$       )/(rsxy(i1,i2,i3,axis,0)**2+rsxy(i1,i2,i3,axis,1)**2)
                    ! fw2 = fw1.tt -[ c22*wrss + c2 wrs ]
                    ! where
                    !     w.r = fw1 = (-rx*vt + ry*ut - (rx*sx+ry*sy)*ws )/(rx**2+ry**2) 
                    ! Compute wrs and wrss by differencing fw1
                    wsm1 = (u(i1,i2,i3,hz)-u(i1-2*js1,i2-2*js2,i3,hz))
     & /(2.*dsa)    ! ws(i1-js1,i2-js2,i3)
                    wsp1 = (u(i1+2*js1,i2+2*js2,i3,hz)-u(i1,i2,i3,hz))
     & /(2.*dsa)    ! ws(i1+js1,i2+js2,i3)
                    fw1m1=(-(rsxy(i1-js1,i2-js2,i3,axis,0)*rsxy(i1-js1,
     & i2-js2,i3,axisp1,0)+rsxy(i1-js1,i2-js2,i3,axis,1)*rsxy(i1-js1,
     & i2-js2,i3,axisp1,1))*wsm1-rsxy(i1-js1,i2-js2,i3,axis,0)*vtm1+
     & rsxy(i1-js1,i2-js2,i3,axis,1)*utm1)/(rsxy(i1-js1,i2-js2,i3,
     & axis,0)**2+rsxy(i1-js1,i2-js2,i3,axis,1)**2)
                    fw1p1=(-(rsxy(i1+js1,i2+js2,i3,axis,0)*rsxy(i1+js1,
     & i2+js2,i3,axisp1,0)+rsxy(i1+js1,i2+js2,i3,axis,1)*rsxy(i1+js1,
     & i2+js2,i3,axisp1,1))*wsp1-rsxy(i1+js1,i2+js2,i3,axis,0)*vtp1+
     & rsxy(i1+js1,i2+js2,i3,axis,1)*utp1)/(rsxy(i1+js1,i2+js2,i3,
     & axis,0)**2+rsxy(i1+js1,i2+js2,i3,axis,1)**2)
                    ! NOTE: the term involving wtts is left off -- the coeff is zero for orthogonal grids
                    fw2 = (-rsxy(i1,i2,i3,axis,0)*vttt0 + rsxy(i1,i2,
     & i3,axis,1)*uttt0 )/(rsxy(i1,i2,i3,axis,0)**2+rsxy(i1,i2,i3,
     & axis,1)**2)- c22*( fw1p1-2.*fw1+fw1m1 )/(dsa**2) -c2*(fw1p1-
     & fw1m1 )/(2.*dsa)
                 !   fw2 = (-rsxy(i1,i2,i3,axis,0)*vttt0 + rsxy(i1,i2,i3,axis,1)*uttt0 )/!                               (rsxy(i1,i2,i3,axis,0)**2+rsxy(i1,i2,i3,axis,1)**2)!         - c22*( (-rsxy(i1+js1,i2+js2,i3,axis,0)*vtp1 + rsxy(i1+js1,i2+js2,i3,axis,1)*utp1)!             -2.*(-rsxy(i1    ,i2    ,i3,axis,0)*vt0  + rsxy(i1    ,i2    ,i3,axis,1)*ut0 ) !                +(-rsxy(i1-js1,i2-js2,i3,axis,0)*vtm1 + rsxy(i1-js1,i2-js2,i3,axis,1)*utm1) )/(dsa**2) !         -  c2*( (-rsxy(i1+js1,i2+js2,i3,axis,0)*vtp1 + rsxy(i1+js1,i2+js2,i3,axis,1)*utp1)!                -(-rsxy(i1-js1,i2-js2,i3,axis,0)*vtm1 + rsxy(i1-js1,i2-js2,i3,axis,1)*utm1) )/(2.*dsa)
                  end if
                 ! Now assign ex and ey at the ghost points:
                 ! #Include "bc4Maxwell.h"
                 ! Use 5th-order extrap: 8wdh* 2015/07/03
! ************ Results from mx/codes/bc4.maple *******************
      gIII=-tau1*(c2*us+c22*uss)-tau2*(c2*vs+c22*vss)

      tauU=tau1*u(i1,i2,i3,ex)+tau2*u(i1,i2,i3,ey)

      tauUp1=tau1*u(i1+is1,i2+is2,i3+is3,ex)+tau2*u(i1+is1,i2+is2,i3+
     & is3,ey)

      tauUp2=tau1*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)+tau2*u(i1+2*is1,i2+
     & 2*is2,i3+2*is3,ey)

      tauUp3=tau1*u(i1+3*is1,i2+3*is2,i3+3*is3,ex)+tau2*u(i1+3*is1,i2+
     & 3*is2,i3+3*is3,ey)

      gIV=-10*tauU+10*tauUp1-5*tauUp2+tauUp3 +gIVf

      ttu1=-1/(3*c1*ctlr*dra-6*c1*dra-c11*ctlrr+12*c11)*(c1*ctlr*dra*
     & gIV+2*c1*ctlr*dra*tauUp1-c1*ctlr*dra*tauUp2+6*c1*dra*tauUp1-
     & c11*ctlrr*gIV-6*c11*ctlrr*tauU+4*c11*ctlrr*tauUp1-c11*ctlrr*
     & tauUp2-12*dra**2*gIII-12*dra**2*tau1DotUtt-24*c11*tauU+12*c11*
     & tauUp1)
      ttu2=-(2*c1*ctlr*dra*gIV+10*c1*ctlr*dra*tauUp1-5*c1*ctlr*dra*
     & tauUp2+6*c1*dra*gIV+30*c1*dra*tauUp1-4*c11*ctlrr*gIV-30*c11*
     & ctlrr*tauU+20*c11*ctlrr*tauUp1-5*c11*ctlrr*tauUp2-60*dra**2*
     & gIII-60*dra**2*tau1DotUtt-12*c11*gIV-120*c11*tauU+60*c11*
     & tauUp1)/(3*c1*ctlr*dra-6*c1*dra-c11*ctlrr+12*c11)

      f1um2=-1/2.*b3u/dra**3-1/12.*b2u/dra**2+1/12.*b1u/dra
      f1um1=b3u/dra**3+4/3.*b2u/dra**2-2/3.*b1u/dra
      f1vm2=-1/2.*b3v/dra**3-1/12.*b2v/dra**2+1/12.*b1v/dra
      f1vm1=b3v/dra**3+4/3.*b2v/dra**2-2/3.*b1v/dra
      f1f  =-1/12.*(b1u*dra**2*u(i1+2*is1,i2+2*is2,i3,ex)-8*b1u*dra**2*
     & u(i1+is1,i2+is2,i3,ex)+b1v*dra**2*u(i1+2*is1,i2+2*is2,i3,ey)-8*
     & b1v*dra**2*u(i1+is1,i2+is2,i3,ey)-12*bf*dra**3+b2u*dra*u(i1+2*
     & is1,i2+2*is2,i3,ex)-16*b2u*dra*u(i1+is1,i2+is2,i3,ex)+b2v*dra*
     & u(i1+2*is1,i2+2*is2,i3,ey)-16*b2v*dra*u(i1+is1,i2+is2,i3,ey)+
     & 30*b2u*dra*u(i1,i2,i3,ex)+30*b2v*dra*u(i1,i2,i3,ey)-6*b3u*u(i1+
     & 2*is1,i2+2*is2,i3,ex)+12*b3u*u(i1+is1,i2+is2,i3,ex)-6*b3v*u(i1+
     & 2*is1,i2+2*is2,i3,ey)+12*b3v*u(i1+is1,i2+is2,i3,ey))/dra**3

      f2um2=1/12.*a11m2
      f2um1=-2/3.*a11m1
      f2vm2=1/12.*a12m2
      f2vm1=-2/3.*a12m1
      f2f  =2/3.*a11p1*u(i1+is1,i2+is2,i3,ex)+2/3.*a12p1*u(i1+is1,i2+
     & is2,i3,ey)-1/12.*a11p2*u(i1+2*is1,i2+2*is2,i3,ex)-1/12.*a12p2*
     & u(i1+2*is1,i2+2*is2,i3,ey)-Da1DotU*dra

      u(i1-2*is1,i2-2*is2,i3,ex) = (f1f*f2um1*tau2**2-f1f*f2vm1*tau1*
     & tau2-f1um1*f2f*tau2**2-f1um1*f2vm1*tau2*ttu1-f1um1*f2vm2*tau2*
     & ttu2+f1vm1*f2f*tau1*tau2+f1vm1*f2um1*tau2*ttu1+f1vm1*f2vm2*
     & tau1*ttu2+f1vm2*f2um1*tau2*ttu2-f1vm2*f2vm1*tau1*ttu2)/(f1um1*
     & f2um2*tau2**2-f1um1*f2vm2*tau1*tau2-f1um2*f2um1*tau2**2+f1um2*
     & f2vm1*tau1*tau2-f1vm1*f2um2*tau1*tau2+f1vm1*f2vm2*tau1**2+
     & f1vm2*f2um1*tau1*tau2-f1vm2*f2vm1*tau1**2)

      u(i1-is1,i2-is2,i3,ex) = -(f1f*f2um2*tau2**2-f1f*f2vm2*tau1*tau2-
     & f1um2*f2f*tau2**2-f1um2*f2vm1*tau2*ttu1-f1um2*f2vm2*tau2*ttu2+
     & f1vm1*f2um2*tau2*ttu1-f1vm1*f2vm2*tau1*ttu1+f1vm2*f2f*tau1*
     & tau2+f1vm2*f2um2*tau2*ttu2+f1vm2*f2vm1*tau1*ttu1)/(f1um1*f2um2*
     & tau2**2-f1um1*f2vm2*tau1*tau2-f1um2*f2um1*tau2**2+f1um2*f2vm1*
     & tau1*tau2-f1vm1*f2um2*tau1*tau2+f1vm1*f2vm2*tau1**2+f1vm2*
     & f2um1*tau1*tau2-f1vm2*f2vm1*tau1**2)

      u(i1-2*is1,i2-2*is2,i3,ey) = -(f1f*f2um1*tau1*tau2-f1f*f2vm1*
     & tau1**2-f1um1*f2f*tau1*tau2-f1um1*f2um2*tau2*ttu2-f1um1*f2vm1*
     & tau1*ttu1+f1um2*f2um1*tau2*ttu2-f1um2*f2vm1*tau1*ttu2+f1vm1*
     & f2f*tau1**2+f1vm1*f2um1*tau1*ttu1+f1vm1*f2um2*tau1*ttu2)/(
     & f1um1*f2um2*tau2**2-f1um1*f2vm2*tau1*tau2-f1um2*f2um1*tau2**2+
     & f1um2*f2vm1*tau1*tau2-f1vm1*f2um2*tau1*tau2+f1vm1*f2vm2*tau1**
     & 2+f1vm2*f2um1*tau1*tau2-f1vm2*f2vm1*tau1**2)

      u(i1-is1,i2-is2,i3,ey) = (f1f*f2um2*tau1*tau2-f1f*f2vm2*tau1**2+
     & f1um1*f2um2*tau2*ttu1-f1um1*f2vm2*tau1*ttu1-f1um2*f2f*tau1*
     & tau2-f1um2*f2um1*tau2*ttu1-f1um2*f2vm2*tau1*ttu2+f1vm2*f2f*
     & tau1**2+f1vm2*f2um1*tau1*ttu1+f1vm2*f2um2*tau1*ttu2)/(f1um1*
     & f2um2*tau2**2-f1um1*f2vm2*tau1*tau2-f1um2*f2um1*tau2**2+f1um2*
     & f2vm1*tau1*tau2-f1vm1*f2um2*tau1*tau2+f1vm1*f2vm2*tau1**2+
     & f1vm2*f2um1*tau1*tau2-f1vm2*f2vm1*tau1**2)


 ! *********** done *********************
                 ! extrapolate normal component:
                 ! #Include "bc4eMaxwell.h"
                 ! Now assign Hz at the ghost points
                 ! u(i1-  is1,i2-  is2,i3-  is3,hz) = u(i1+  is1,i2+  is2,i3+  is3,hz)
                 ! u(i1-2*is1,i2-2*is2,i3-2*is3,hz) = u(i1+2*is1,i2+2*is2,i3+2*is3,hz)


! ************ Hz Answer *******************
      cw2=c1+c11r
      cw1=c1r
      bfw2=c22r*wss+c2r*ws-fw2

      u(i1-is1,i2-is2,i3,hz) = 1/2.*(-18*c11*u(i1+is1,i2+is2,i3,hz)+36*
     & c11*fw1*dra-12*cw2*dra*u(i1+is1,i2+is2,i3,hz)+15*cw2*dra*u(i1,
     & i2,i3,hz)+cw2*dra*u(i1+2*is1,i2+2*is2,i3,hz)+6*cw2*dra**2*fw1-
     & 6*cw1*dra**3*fw1-6*bfw2*dra**3)/(-9*c11+2*cw2*dra)

      u(i1-2*is1,i2-2*is2,i3,hz) = (-64*cw2*dra*u(i1+is1,i2+is2,i3,hz)+
     & 36*c11*fw1*dra+60*cw2*dra*u(i1,i2,i3,hz)+6*cw2*dra*u(i1+2*is1,
     & i2+2*is2,i3,hz)+48*cw2*dra**2*fw1-24*cw1*dra**3*fw1-24*bfw2*
     & dra**3-9*c11*u(i1+2*is1,i2+2*is2,i3,hz))/(-9*c11+2*cw2*dra)


 ! *********** Hz done *********************
                 !  **********************************************************************************************
                 else if( mask(i1,i2,i3).lt.0 )then
                  ! we need to assign ghost points that lie outside of interpolation points
                  ! This case is similar to above except that we extrapolate the 2nd-ghost line values for a1.u
                  jac=1./(rx(i1,i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,
     & i2,i3))
                  a11 =rsxy(i1,i2,i3,axis  ,0)*jac
                  a12 =rsxy(i1,i2,i3,axis  ,1)*jac
                  a21 =rsxy(i1,i2,i3,axisp1,0)*jac
                  a22 =rsxy(i1,i2,i3,axisp1,1)*jac
                  jacm1=1./(rx(i1-is1,i2-is2,i3)*sy(i1-is1,i2-is2,i3)-
     & ry(i1-is1,i2-is2,i3)*sx(i1-is1,i2-is2,i3))
                  a11m1 =rsxy(i1-is1,i2-is2,i3,axis  ,0)*jacm1
                  a12m1 =rsxy(i1-is1,i2-is2,i3,axis  ,1)*jacm1
                  jacp1=1./(rx(i1+is1,i2+is2,i3)*sy(i1+is1,i2+is2,i3)-
     & ry(i1+is1,i2+is2,i3)*sx(i1+is1,i2+is2,i3))
                  a11p1=rsxy(i1+is1,i2+is2,i3,axis,0)*jacp1
                  a12p1=rsxy(i1+is1,i2+is2,i3,axis,1)*jacp1
                  jacm2=1./(rx(i1-2*is1,i2-2*is2,i3)*sy(i1-2*is1,i2-2*
     & is2,i3)-ry(i1-2*is1,i2-2*is2,i3)*sx(i1-2*is1,i2-2*is2,i3))
                  a11m2=rsxy(i1-2*is1,i2-2*is2,i3,axis,0)*jacm2
                  a12m2=rsxy(i1-2*is1,i2-2*is2,i3,axis,1)*jacm2
                  jacp2=1./(rx(i1+2*is1,i2+2*is2,i3)*sy(i1+2*is1,i2+2*
     & is2,i3)-ry(i1+2*is1,i2+2*is2,i3)*sx(i1+2*is1,i2+2*is2,i3))
                  a11p2=rsxy(i1+2*is1,i2+2*is2,i3,axis,0)*jacp2
                  a12p2=rsxy(i1+2*is1,i2+2*is2,i3,axis,1)*jacp2
                  a11r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axis,0)/(rx(
     & i1+is1,i2+is2,i3+is3)*sy(i1+is1,i2+is2,i3+is3)-ry(i1+is1,i2+
     & is2,i3+is3)*sx(i1+is1,i2+is2,i3+is3)))-(rsxy(i1-is1,i2-is2,i3-
     & is3,axis,0)/(rx(i1-is1,i2-is2,i3-is3)*sy(i1-is1,i2-is2,i3-is3)-
     & ry(i1-is1,i2-is2,i3-is3)*sx(i1-is1,i2-is2,i3-is3))))-((rsxy(i1+
     & 2*is1,i2+2*is2,i3+2*is3,axis,0)/(rx(i1+2*is1,i2+2*is2,i3+2*is3)
     & *sy(i1+2*is1,i2+2*is2,i3+2*is3)-ry(i1+2*is1,i2+2*is2,i3+2*is3)*
     & sx(i1+2*is1,i2+2*is2,i3+2*is3)))-(rsxy(i1-2*is1,i2-2*is2,i3-2*
     & is3,axis,0)/(rx(i1-2*is1,i2-2*is2,i3-2*is3)*sy(i1-2*is1,i2-2*
     & is2,i3-2*is3)-ry(i1-2*is1,i2-2*is2,i3-2*is3)*sx(i1-2*is1,i2-2*
     & is2,i3-2*is3)))))/(12.*dra)
                  a12r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axis,1)/(rx(
     & i1+is1,i2+is2,i3+is3)*sy(i1+is1,i2+is2,i3+is3)-ry(i1+is1,i2+
     & is2,i3+is3)*sx(i1+is1,i2+is2,i3+is3)))-(rsxy(i1-is1,i2-is2,i3-
     & is3,axis,1)/(rx(i1-is1,i2-is2,i3-is3)*sy(i1-is1,i2-is2,i3-is3)-
     & ry(i1-is1,i2-is2,i3-is3)*sx(i1-is1,i2-is2,i3-is3))))-((rsxy(i1+
     & 2*is1,i2+2*is2,i3+2*is3,axis,1)/(rx(i1+2*is1,i2+2*is2,i3+2*is3)
     & *sy(i1+2*is1,i2+2*is2,i3+2*is3)-ry(i1+2*is1,i2+2*is2,i3+2*is3)*
     & sx(i1+2*is1,i2+2*is2,i3+2*is3)))-(rsxy(i1-2*is1,i2-2*is2,i3-2*
     & is3,axis,1)/(rx(i1-2*is1,i2-2*is2,i3-2*is3)*sy(i1-2*is1,i2-2*
     & is2,i3-2*is3)-ry(i1-2*is1,i2-2*is2,i3-2*is3)*sx(i1-2*is1,i2-2*
     & is2,i3-2*is3)))))/(12.*dra)
                  a21r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axisp1,0)/(rx(
     & i1+is1,i2+is2,i3+is3)*sy(i1+is1,i2+is2,i3+is3)-ry(i1+is1,i2+
     & is2,i3+is3)*sx(i1+is1,i2+is2,i3+is3)))-(rsxy(i1-is1,i2-is2,i3-
     & is3,axisp1,0)/(rx(i1-is1,i2-is2,i3-is3)*sy(i1-is1,i2-is2,i3-
     & is3)-ry(i1-is1,i2-is2,i3-is3)*sx(i1-is1,i2-is2,i3-is3))))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp1,0)/(rx(i1+2*is1,i2+2*
     & is2,i3+2*is3)*sy(i1+2*is1,i2+2*is2,i3+2*is3)-ry(i1+2*is1,i2+2*
     & is2,i3+2*is3)*sx(i1+2*is1,i2+2*is2,i3+2*is3)))-(rsxy(i1-2*is1,
     & i2-2*is2,i3-2*is3,axisp1,0)/(rx(i1-2*is1,i2-2*is2,i3-2*is3)*sy(
     & i1-2*is1,i2-2*is2,i3-2*is3)-ry(i1-2*is1,i2-2*is2,i3-2*is3)*sx(
     & i1-2*is1,i2-2*is2,i3-2*is3)))))/(12.*dra)
                  a22r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axisp1,1)/(rx(
     & i1+is1,i2+is2,i3+is3)*sy(i1+is1,i2+is2,i3+is3)-ry(i1+is1,i2+
     & is2,i3+is3)*sx(i1+is1,i2+is2,i3+is3)))-(rsxy(i1-is1,i2-is2,i3-
     & is3,axisp1,1)/(rx(i1-is1,i2-is2,i3-is3)*sy(i1-is1,i2-is2,i3-
     & is3)-ry(i1-is1,i2-is2,i3-is3)*sx(i1-is1,i2-is2,i3-is3))))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp1,1)/(rx(i1+2*is1,i2+2*
     & is2,i3+2*is3)*sy(i1+2*is1,i2+2*is2,i3+2*is3)-ry(i1+2*is1,i2+2*
     & is2,i3+2*is3)*sx(i1+2*is1,i2+2*is2,i3+2*is3)))-(rsxy(i1-2*is1,
     & i2-2*is2,i3-2*is3,axisp1,1)/(rx(i1-2*is1,i2-2*is2,i3-2*is3)*sy(
     & i1-2*is1,i2-2*is2,i3-2*is3)-ry(i1-2*is1,i2-2*is2,i3-2*is3)*sx(
     & i1-2*is1,i2-2*is2,i3-2*is3)))))/(12.*dra)
                 ! a11s = DS4($A11)
                 ! a12s = DS4($A12)
                 ! a21s = DS4($A21)
                 ! a22s = DS4($A22)
                  c11 = (rsxy(i1,i2,i3,axis,0)**2+rsxy(i1,i2,i3,axis,1)
     & **2)
                  c22 = (rsxy(i1,i2,i3,axisp1,0)**2+rsxy(i1,i2,i3,
     & axisp1,1)**2)
                 ! *  c1 = C1Order4(i1,i2,i3)
                 ! *  c2 = C2Order4(i1,i2,i3)
                  ! These next r derivatives are needed for Hz
                  c11r = (8.*((rsxy(i1+is1,i2+is2,i3,axis,0)**2+rsxy(
     & i1+is1,i2+is2,i3,axis,1)**2)-(rsxy(i1-is1,i2-is2,i3,axis,0)**2+
     & rsxy(i1-is1,i2-is2,i3,axis,1)**2))   -((rsxy(i1+2*is1,i2+2*is2,
     & i3,axis,0)**2+rsxy(i1+2*is1,i2+2*is2,i3,axis,1)**2)-(rsxy(i1-2*
     & is1,i2-2*is2,i3,axis,0)**2+rsxy(i1-2*is1,i2-2*is2,i3,axis,1)**
     & 2))   )/(12.*dra)
                  c22r = (8.*((rsxy(i1+is1,i2+is2,i3,axisp1,0)**2+rsxy(
     & i1+is1,i2+is2,i3,axisp1,1)**2)-(rsxy(i1-is1,i2-is2,i3,axisp1,0)
     & **2+rsxy(i1-is1,i2-is2,i3,axisp1,1)**2))   -((rsxy(i1+2*is1,i2+
     & 2*is2,i3,axisp1,0)**2+rsxy(i1+2*is1,i2+2*is2,i3,axisp1,1)**2)-(
     & rsxy(i1-2*is1,i2-2*is2,i3,axisp1,0)**2+rsxy(i1-2*is1,i2-2*is2,
     & i3,axisp1,1)**2))   )/(12.*dra)
                 ! *  if( axis.eq.0 )then
                 ! *    c1r = C1r4(i1,i2,i3)
                 ! *    c2r = C2r4(i1,i2,i3)
                 ! *  else
                 ! *    c1r = C1s4(i1,i2,i3)
                 ! *    c2r = C2s4(i1,i2,i3)
                 ! *  end if
                 ! ************** OLD **************
                 ! *  ! Use one sided approximations as needed 
                 ! *  js1a=abs(js1)
                 ! *  js2a=abs(js2)
                 ! *  if( (i1-2*js1a).ge.md1a .and. (i2-2*js2a).ge.md2a .and. (i1+2*js1a).le.md1b .and. (i2+2*js2a).le.md2b )then
                 ! *    a11s = DS4($A11)
                 ! *    a12s = DS4($A12)
                 ! *    a21s = DS4($A21)
                 ! *    a22s = DS4($A22)
                 ! *  else if( (i1-js1a).ge.md1a .and. (i2-js2a).ge.md2a .and. (i1+js1a).le.md1b .and. (i2+js2a).le.md2b )then
                 ! *    a11s = DS($A11)
                 ! *    a12s = DS($A12)
                 ! *    a21s = DS($A21)
                 ! *    a22s = DS($A22)
                 ! *  else if( (i1-js1).ge.md1a .and. (i1-js1).le.md1b .and. (i2-js2).ge.md2a .and. (i2-js2).le.md2b )then
                 ! *   ! 2nd-order:
                 ! *   a11s =-(-3.*A11(i1,i2,i3)+4.*A11(i1-js1,i2-js2,i3)-A11(i1-2*js1,i2-2*js2,i3))/(2.*dsa)
                 ! *   a12s =-(-3.*A12(i1,i2,i3)+4.*A12(i1-js1,i2-js2,i3)-A12(i1-2*js1,i2-2*js2,i3))/(2.*dsa)
                 ! *   a21s =-(-3.*A21(i1,i2,i3)+4.*A21(i1-js1,i2-js2,i3)-A21(i1-2*js1,i2-2*js2,i3))/(2.*dsa)
                 ! *   a22s =-(-3.*A22(i1,i2,i3)+4.*A22(i1-js1,i2-js2,i3)-A22(i1-2*js1,i2-2*js2,i3))/(2.*dsa)
                 ! *  else
                 ! *   a11s = (-3.*A11(i1,i2,i3)+4.*A11(i1+js1,i2+js2,i3)-A11(i1+2*js1,i2+2*js2,i3))/(2.*dsa)
                 ! *   a12s = (-3.*A12(i1,i2,i3)+4.*A12(i1+js1,i2+js2,i3)-A12(i1+2*js1,i2+2*js2,i3))/(2.*dsa)
                 ! *   a21s = (-3.*A21(i1,i2,i3)+4.*A21(i1+js1,i2+js2,i3)-A21(i1+2*js1,i2+2*js2,i3))/(2.*dsa)
                 ! *   a22s = (-3.*A22(i1,i2,i3)+4.*A22(i1+js1,i2+js2,i3)-A22(i1+2*js1,i2+2*js2,i3))/(2.*dsa)
                 ! *  end if
                 ! * 
                 ! * 
                 ! *  ! warning -- the compiler could still try to evaluate the mask at an invalid point
                 ! *  if( (i1-js1a).ge.md1a .and. (i2-js2a).ge.md2a .and. mask(i1-js1,i2-js2,i3).ne.0 .and. ! *      (i1+js1a).le.md1b .and. (i2+js2a).le.md2b .and. mask(i1+js1,i2+js2,i3).ne.0 )then
                 ! *    us=US2(ex)
                 ! *    vs=US2(ey)
                 ! *    ws=US2(hz)
                 ! * 
                 ! *    uss=USS2(ex)
                 ! *    vss=USS2(ey)
                 ! *    wss=USS2(hz)
                 ! *   !  write(*,'(" **ghost-interp: use central difference: us,uss=",2e10.2)') us,uss
                 ! * 
                 ! *  else if( (i1-2*js1).ge.md1a .and. (i1-2*js1).le.md1b .and. ! *           (i2-2*js2).ge.md2a .and. (i2-2*js2).le.md2b .and. ! *            mask(i1-js1,i2-js2,i3).ne.0 .and. mask(i1-2*js1,i2-2*js2,i3).ne.0 )then
                 ! *    
                 ! *   ! these are just first order but this is probably good enough since these values
                 ! *   ! may not even appear in any other equations
                 ! * !  us = (u(i1,i2,i3,ex)-u(i1-js1,i2-js2,i3,ex))/dsa
                 ! * !  vs = (u(i1,i2,i3,ey)-u(i1-js1,i2-js2,i3,ey))/dsa
                 ! * !  ws = (u(i1,i2,i3,hz)-u(i1-js1,i2-js2,i3,hz))/dsa
                 ! * !
                 ! * !  uss = (u(i1,i2,i3,ex)-2.*u(i1-js1,i2-js2,i3,ex)+u(i1-2*js1,i2-2*js2,i3,ex))/(dsa**2)
                 ! * !  vss = (u(i1,i2,i3,ey)-2.*u(i1-js1,i2-js2,i3,ey)+u(i1-2*js1,i2-2*js2,i3,ey))/(dsa**2)
                 ! * !  wss = (u(i1,i2,i3,hz)-2.*u(i1-js1,i2-js2,i3,hz)+u(i1-2*js1,i2-2*js2,i3,hz))/(dsa**2)
                 ! * 
                 ! *   ! 2nd-order:
                 ! * 
                 ! *   us = -(-3.*u(i1,i2,i3,ex)+4.*u(i1-js1,i2-js2,i3,ex)-u(i1-2*js1,i2-2*js2,i3,ex))/(2.*dsa)
                 ! *   vs = -(-3.*u(i1,i2,i3,ey)+4.*u(i1-js1,i2-js2,i3,ey)-u(i1-2*js1,i2-2*js2,i3,ey))/(2.*dsa)
                 ! *   ws = -(-3.*u(i1,i2,i3,hz)+4.*u(i1-js1,i2-js2,i3,hz)-u(i1-2*js1,i2-2*js2,i3,hz))/(2.*dsa)
                 ! * 
                 ! *   uss = (2.*u(i1,i2,i3,ex)-5.*u(i1-js1,i2-js2,i3,ex)+4.*u(i1-2*js1,i2-2*js2,i3,ex)-u(i1-3*js1,i2-3*js2,i3,ex))/(dsa**2)
                 ! *   vss = (2.*u(i1,i2,i3,ey)-5.*u(i1-js1,i2-js2,i3,ey)+4.*u(i1-2*js1,i2-2*js2,i3,ey)-u(i1-3*js1,i2-3*js2,i3,ey))/(dsa**2)
                 ! *   wss = (2.*u(i1,i2,i3,hz)-5.*u(i1-js1,i2-js2,i3,hz)+4.*u(i1-2*js1,i2-2*js2,i3,hz)-u(i1-3*js1,i2-3*js2,i3,hz))/(dsa**2)
                 ! * 
                 ! * !  write(*,'(" **ghost-interp: use left-difference: us,uss=",2e10.2," us1=",e10.2," js1,js2=",2i2)') us,uss,! * !            (u(i1,i2,i3,ex)-u(i1-js1,i2-js2,i3,ex))/dsa,js1,js2
                 ! * 
                 ! *  else if( (i1+2*js1).ge.md1a .and. (i1+2*js1).le.md1b .and. ! *           (i2+2*js2).ge.md2a .and. (i2+2*js2).le.md2b .and.  ! *           mask(i1+js1,i2+js2,i3).ne.0 .and. mask(i1+2*js1,i2+2*js2,i3).ne.0 )then
                 ! * 
                 ! * !  us = (u(i1+js1,i2+js2,i3,ex)-u(i1,i2,i3,ex))/dsa
                 ! * !  vs = (u(i1+js1,i2+js2,i3,ey)-u(i1,i2,i3,ey))/dsa
                 ! * !  ws = (u(i1+js1,i2+js2,i3,hz)-u(i1,i2,i3,hz))/dsa
                 ! * !
                 ! * !  uss = (u(i1,i2,i3,ex)-2.*u(i1+js1,i2+js2,i3,ex)+u(i1+2*js1,i2+2*js2,i3,ex))/(dsa**2)
                 ! * !  vss = (u(i1,i2,i3,ey)-2.*u(i1+js1,i2+js2,i3,ey)+u(i1+2*js1,i2+2*js2,i3,ey))/(dsa**2)
                 ! * !  wss = (u(i1,i2,i3,hz)-2.*u(i1+js1,i2+js2,i3,hz)+u(i1+2*js1,i2+2*js2,i3,hz))/(dsa**2)
                 ! * 
                 ! *   ! 2nd-order:
                 ! *  us = (-3.*u(i1,i2,i3,ex)+4.*u(i1+js1,i2+js2,i3,ex)-u(i1+2*js1,i2+2*js2,i3,ex))/(2.*dsa)
                 ! *  vs = (-3.*u(i1,i2,i3,ey)+4.*u(i1+js1,i2+js2,i3,ey)-u(i1+2*js1,i2+2*js2,i3,ey))/(2.*dsa)
                 ! *  ws = (-3.*u(i1,i2,i3,hz)+4.*u(i1+js1,i2+js2,i3,hz)-u(i1+2*js1,i2+2*js2,i3,hz))/(2.*dsa)
                 ! *  uss = (2.*u(i1,i2,i3,ex)-5.*u(i1+js1,i2+js2,i3,ex)+4.*u(i1+2*js1,i2+2*js2,i3,ex)-u(i1+3*js1,i2+3*js2,i3,ex))/(dsa**2)
                 ! *  vss = (2.*u(i1,i2,i3,ey)-5.*u(i1+js1,i2+js2,i3,ey)+4.*u(i1+2*js1,i2+2*js2,i3,ey)-u(i1+3*js1,i2+3*js2,i3,ey))/(dsa**2)
                 ! *  wss = (2.*u(i1,i2,i3,hz)-5.*u(i1+js1,i2+js2,i3,hz)+4.*u(i1+2*js1,i2+2*js2,i3,hz)-u(i1+3*js1,i2+3*js2,i3,hz))/(dsa**2)
                 ! * 
                 ! *  ! write(*,'(" **ghost-interp: use right-difference: us,uss=",2e10.2)') us,uss
                 ! * 
                 ! *  else 
                 ! *    ! this case shouldn't matter
                 ! *    us=0.
                 ! *    vs=0.
                 ! *    ws=0.
                 ! *    uss=0.
                 ! *    vss=0.
                 ! *    wss=0.
                 ! *  end if
                 ! *********************** NEW ************************
                  ! ***************************************************************************************
                  ! Use one sided approximations as needed for expressions needing tangential derivatives
                  ! ***************************************************************************************
                  js1a=abs(js1)
                  js2a=abs(js2)
                  ! *** first do metric derivatives -- no need to worry about the mask value ****
                  if( (i1-2*js1a).ge.md1a .and. (i1+2*js1a).le.md1b 
     & .and. (i2-2*js2a).ge.md2a .and. (i2+2*js2a).le.md2b )then
                   ! centered approximation is ok
                   c1 = (rsxyx42(i1,i2,i3,axis,0)+rsxyy42(i1,i2,i3,
     & axis,1))
                   c2 = (rsxyx42(i1,i2,i3,axisp1,0)+rsxyy42(i1,i2,i3,
     & axisp1,1))
                   if( axis.eq.0 )then
                     c1r = (rsxyxr42(i1,i2,i3,axis,0)+rsxyyr42(i1,i2,
     & i3,axis,1))
                     c2r = (rsxyxr42(i1,i2,i3,axisp1,0)+rsxyyr42(i1,i2,
     & i3,axisp1,1))
                   else
                     c1r = (rsxyxs42(i1,i2,i3,axis,0)+rsxyys42(i1,i2,
     & i3,axis,1))
                     c2r = (rsxyxs42(i1,i2,i3,axisp1,0)+rsxyys42(i1,i2,
     & i3,axisp1,1))
                   end if
                   a11s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axis,0)/(rx(
     & i1+js1,i2+js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+
     & js2,i3+js3)*sx(i1+js1,i2+js2,i3+js3)))-(rsxy(i1-js1,i2-js2,i3-
     & js3,axis,0)/(rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-
     & ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3))))-((rsxy(i1+
     & 2*js1,i2+2*js2,i3+2*js3,axis,0)/(rx(i1+2*js1,i2+2*js2,i3+2*js3)
     & *sy(i1+2*js1,i2+2*js2,i3+2*js3)-ry(i1+2*js1,i2+2*js2,i3+2*js3)*
     & sx(i1+2*js1,i2+2*js2,i3+2*js3)))-(rsxy(i1-2*js1,i2-2*js2,i3-2*
     & js3,axis,0)/(rx(i1-2*js1,i2-2*js2,i3-2*js3)*sy(i1-2*js1,i2-2*
     & js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,i3-2*js3)*sx(i1-2*js1,i2-2*
     & js2,i3-2*js3)))))/(12.*dsa)
                   a12s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axis,1)/(rx(
     & i1+js1,i2+js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+
     & js2,i3+js3)*sx(i1+js1,i2+js2,i3+js3)))-(rsxy(i1-js1,i2-js2,i3-
     & js3,axis,1)/(rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-
     & ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3))))-((rsxy(i1+
     & 2*js1,i2+2*js2,i3+2*js3,axis,1)/(rx(i1+2*js1,i2+2*js2,i3+2*js3)
     & *sy(i1+2*js1,i2+2*js2,i3+2*js3)-ry(i1+2*js1,i2+2*js2,i3+2*js3)*
     & sx(i1+2*js1,i2+2*js2,i3+2*js3)))-(rsxy(i1-2*js1,i2-2*js2,i3-2*
     & js3,axis,1)/(rx(i1-2*js1,i2-2*js2,i3-2*js3)*sy(i1-2*js1,i2-2*
     & js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,i3-2*js3)*sx(i1-2*js1,i2-2*
     & js2,i3-2*js3)))))/(12.*dsa)
                   a21s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axisp1,0)/(
     & rx(i1+js1,i2+js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+
     & js2,i3+js3)*sx(i1+js1,i2+js2,i3+js3)))-(rsxy(i1-js1,i2-js2,i3-
     & js3,axisp1,0)/(rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-
     & js3)-ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3))))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,0)/(rx(i1+2*js1,i2+2*
     & js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,i3+2*js3)-ry(i1+2*js1,i2+2*
     & js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,i3+2*js3)))-(rsxy(i1-2*js1,
     & i2-2*js2,i3-2*js3,axisp1,0)/(rx(i1-2*js1,i2-2*js2,i3-2*js3)*sy(
     & i1-2*js1,i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,i3-2*js3)*sx(
     & i1-2*js1,i2-2*js2,i3-2*js3)))))/(12.*dsa)
                   a22s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axisp1,1)/(
     & rx(i1+js1,i2+js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+
     & js2,i3+js3)*sx(i1+js1,i2+js2,i3+js3)))-(rsxy(i1-js1,i2-js2,i3-
     & js3,axisp1,1)/(rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-
     & js3)-ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3))))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,1)/(rx(i1+2*js1,i2+2*
     & js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,i3+2*js3)-ry(i1+2*js1,i2+2*
     & js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,i3+2*js3)))-(rsxy(i1-2*js1,
     & i2-2*js2,i3-2*js3,axisp1,1)/(rx(i1-2*js1,i2-2*js2,i3-2*js3)*sy(
     & i1-2*js1,i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,i3-2*js3)*sx(
     & i1-2*js1,i2-2*js2,i3-2*js3)))))/(12.*dsa)
                  else if( (i1-js1a).ge.md1a .and. (i1+js1a).le.md1b 
     & .and. (i2-js2a).ge.md2a .and. (i2+js2a).le.md2b )then
                   ! use 2nd-order centered approximation
                   c1 = (rsxyx22(i1,i2,i3,axis,0)+rsxyy22(i1,i2,i3,
     & axis,1))
                   c2 = (rsxyx22(i1,i2,i3,axisp1,0)+rsxyy22(i1,i2,i3,
     & axisp1,1))
                   if( axis.eq.0 )then
                     c1r = (rsxyxr22(i1,i2,i3,axis,0)+rsxyyr22(i1,i2,
     & i3,axis,1))
                     c2r = (rsxyxr22(i1,i2,i3,axisp1,0)+rsxyyr22(i1,i2,
     & i3,axisp1,1))
                   else
                     c1r = (rsxyxs22(i1,i2,i3,axis,0)+rsxyys22(i1,i2,
     & i3,axis,1))
                     c2r = (rsxyxs22(i1,i2,i3,axisp1,0)+rsxyys22(i1,i2,
     & i3,axisp1,1))
                   end if
                   a11s = ((rsxy(i1+js1,i2+js2,i3+js3,axis,0)/(rx(i1+
     & js1,i2+js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+js2,
     & i3+js3)*sx(i1+js1,i2+js2,i3+js3)))-(rsxy(i1-js1,i2-js2,i3-js3,
     & axis,0)/(rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-ry(
     & i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3))))/(2.*dsa)
                   a12s = ((rsxy(i1+js1,i2+js2,i3+js3,axis,1)/(rx(i1+
     & js1,i2+js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+js2,
     & i3+js3)*sx(i1+js1,i2+js2,i3+js3)))-(rsxy(i1-js1,i2-js2,i3-js3,
     & axis,1)/(rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-ry(
     & i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3))))/(2.*dsa)
                   a21s = ((rsxy(i1+js1,i2+js2,i3+js3,axisp1,0)/(rx(i1+
     & js1,i2+js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+js2,
     & i3+js3)*sx(i1+js1,i2+js2,i3+js3)))-(rsxy(i1-js1,i2-js2,i3-js3,
     & axisp1,0)/(rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-
     & ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3))))/(2.*dsa)
                   a22s = ((rsxy(i1+js1,i2+js2,i3+js3,axisp1,1)/(rx(i1+
     & js1,i2+js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+js2,
     & i3+js3)*sx(i1+js1,i2+js2,i3+js3)))-(rsxy(i1-js1,i2-js2,i3-js3,
     & axisp1,1)/(rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-
     & ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3))))/(2.*dsa)
                  else if( (i1-3*js1a).ge.md1a .and. (i2-3*js2a)
     & .ge.md2a )then
                   ! one sided  2nd-order:
                   c1 = 2.*(rsxyx22(i1-js1a,i2-js2a,i3,axis,0)+rsxyy22(
     & i1-js1a,i2-js2a,i3,axis,1))-(rsxyx22(i1-2*js1a,i2-2*js2a,i3,
     & axis,0)+rsxyy22(i1-2*js1a,i2-2*js2a,i3,axis,1))
                   c2 = 2.*(rsxyx22(i1-js1a,i2-js2a,i3,axisp1,0)+
     & rsxyy22(i1-js1a,i2-js2a,i3,axisp1,1))-(rsxyx22(i1-2*js1a,i2-2*
     & js2a,i3,axisp1,0)+rsxyy22(i1-2*js1a,i2-2*js2a,i3,axisp1,1))
                   if( axis.eq.0 )then
                     c1r = 2.*(rsxyxr22(i1-js1a,i2-js2a,i3,axis,0)+
     & rsxyyr22(i1-js1a,i2-js2a,i3,axis,1))-(rsxyxr22(i1-2*js1a,i2-2*
     & js2a,i3,axis,0)+rsxyyr22(i1-2*js1a,i2-2*js2a,i3,axis,1))
                     c2r = 2.*(rsxyxr22(i1-js1a,i2-js2a,i3,axisp1,0)+
     & rsxyyr22(i1-js1a,i2-js2a,i3,axisp1,1))-(rsxyxr22(i1-2*js1a,i2-
     & 2*js2a,i3,axisp1,0)+rsxyyr22(i1-2*js1a,i2-2*js2a,i3,axisp1,1))
                   else
                     c1r = 2.*(rsxyxs22(i1-js1a,i2-js2a,i3,axis,0)+
     & rsxyys22(i1-js1a,i2-js2a,i3,axis,1))-(rsxyxs22(i1-2*js1a,i2-2*
     & js2a,i3,axis,0)+rsxyys22(i1-2*js1a,i2-2*js2a,i3,axis,1))
                     c2r = 2.*(rsxyxs22(i1-js1a,i2-js2a,i3,axisp1,0)+
     & rsxyys22(i1-js1a,i2-js2a,i3,axisp1,1))-(rsxyxs22(i1-2*js1a,i2-
     & 2*js2a,i3,axisp1,0)+rsxyys22(i1-2*js1a,i2-2*js2a,i3,axisp1,1))
                   end if
                   a11s =-(-3.*(rsxy(i1,i2,i3,axis,0)/(rx(i1,i2,i3)*sy(
     & i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+4.*(rsxy(i1-js1a,i2-js2a,
     & i3,axis,0)/(rx(i1-js1a,i2-js2a,i3)*sy(i1-js1a,i2-js2a,i3)-ry(
     & i1-js1a,i2-js2a,i3)*sx(i1-js1a,i2-js2a,i3)))-(rsxy(i1-2*js1a,
     & i2-2*js2a,i3,axis,0)/(rx(i1-2*js1a,i2-2*js2a,i3)*sy(i1-2*js1a,
     & i2-2*js2a,i3)-ry(i1-2*js1a,i2-2*js2a,i3)*sx(i1-2*js1a,i2-2*
     & js2a,i3))))/(2.*dsb) ! NOTE: use ds not dsa
                   a12s =-(-3.*(rsxy(i1,i2,i3,axis,1)/(rx(i1,i2,i3)*sy(
     & i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+4.*(rsxy(i1-js1a,i2-js2a,
     & i3,axis,1)/(rx(i1-js1a,i2-js2a,i3)*sy(i1-js1a,i2-js2a,i3)-ry(
     & i1-js1a,i2-js2a,i3)*sx(i1-js1a,i2-js2a,i3)))-(rsxy(i1-2*js1a,
     & i2-2*js2a,i3,axis,1)/(rx(i1-2*js1a,i2-2*js2a,i3)*sy(i1-2*js1a,
     & i2-2*js2a,i3)-ry(i1-2*js1a,i2-2*js2a,i3)*sx(i1-2*js1a,i2-2*
     & js2a,i3))))/(2.*dsb)
                   a21s =-(-3.*(rsxy(i1,i2,i3,axisp1,0)/(rx(i1,i2,i3)*
     & sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+4.*(rsxy(i1-js1a,i2-
     & js2a,i3,axisp1,0)/(rx(i1-js1a,i2-js2a,i3)*sy(i1-js1a,i2-js2a,
     & i3)-ry(i1-js1a,i2-js2a,i3)*sx(i1-js1a,i2-js2a,i3)))-(rsxy(i1-2*
     & js1a,i2-2*js2a,i3,axisp1,0)/(rx(i1-2*js1a,i2-2*js2a,i3)*sy(i1-
     & 2*js1a,i2-2*js2a,i3)-ry(i1-2*js1a,i2-2*js2a,i3)*sx(i1-2*js1a,
     & i2-2*js2a,i3))))/(2.*dsb)
                   a22s =-(-3.*(rsxy(i1,i2,i3,axisp1,1)/(rx(i1,i2,i3)*
     & sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+4.*(rsxy(i1-js1a,i2-
     & js2a,i3,axisp1,1)/(rx(i1-js1a,i2-js2a,i3)*sy(i1-js1a,i2-js2a,
     & i3)-ry(i1-js1a,i2-js2a,i3)*sx(i1-js1a,i2-js2a,i3)))-(rsxy(i1-2*
     & js1a,i2-2*js2a,i3,axisp1,1)/(rx(i1-2*js1a,i2-2*js2a,i3)*sy(i1-
     & 2*js1a,i2-2*js2a,i3)-ry(i1-2*js1a,i2-2*js2a,i3)*sx(i1-2*js1a,
     & i2-2*js2a,i3))))/(2.*dsb)
                 ! if( debug.gt.0 )then
                 !   write(*,'(" ghost-interp:left-shift i=",3i3," js1a,js2a=",2i3," c2r,c2s2(-1),c2s2(-2)=",10e10.2)')!      i1,i2,i3,js1a,js2a,c2r,C2s2(i1-js1a,i2-js2a,i3),C2s2(i1-2*js1a,i2-2*js2a,i3)
                 ! end if
                  else if( (i1+3*js1a).le.md1b .and. (i2+3*js2a)
     & .le.md2b )then
                   ! one sided  2nd-order:
                   c1 = 2.*(rsxyx22(i1+js1a,i2+js2a,i3,axis,0)+rsxyy22(
     & i1+js1a,i2+js2a,i3,axis,1))-(rsxyx22(i1+2*js1a,i2+2*js2a,i3,
     & axis,0)+rsxyy22(i1+2*js1a,i2+2*js2a,i3,axis,1))
                   c2 = 2.*(rsxyx22(i1+js1a,i2+js2a,i3,axisp1,0)+
     & rsxyy22(i1+js1a,i2+js2a,i3,axisp1,1))-(rsxyx22(i1+2*js1a,i2+2*
     & js2a,i3,axisp1,0)+rsxyy22(i1+2*js1a,i2+2*js2a,i3,axisp1,1))
                   if( axis.eq.0 )then
                     c1r = 2.*(rsxyxr22(i1+js1a,i2+js2a,i3,axis,0)+
     & rsxyyr22(i1+js1a,i2+js2a,i3,axis,1))-(rsxyxr22(i1+2*js1a,i2+2*
     & js2a,i3,axis,0)+rsxyyr22(i1+2*js1a,i2+2*js2a,i3,axis,1))
                     c2r = 2.*(rsxyxr22(i1+js1a,i2+js2a,i3,axisp1,0)+
     & rsxyyr22(i1+js1a,i2+js2a,i3,axisp1,1))-(rsxyxr22(i1+2*js1a,i2+
     & 2*js2a,i3,axisp1,0)+rsxyyr22(i1+2*js1a,i2+2*js2a,i3,axisp1,1))
                   else
                     c1r = 2.*(rsxyxs22(i1+js1a,i2+js2a,i3,axis,0)+
     & rsxyys22(i1+js1a,i2+js2a,i3,axis,1))-(rsxyxs22(i1+2*js1a,i2+2*
     & js2a,i3,axis,0)+rsxyys22(i1+2*js1a,i2+2*js2a,i3,axis,1))
                     c2r = 2.*(rsxyxs22(i1+js1a,i2+js2a,i3,axisp1,0)+
     & rsxyys22(i1+js1a,i2+js2a,i3,axisp1,1))-(rsxyxs22(i1+2*js1a,i2+
     & 2*js2a,i3,axisp1,0)+rsxyys22(i1+2*js1a,i2+2*js2a,i3,axisp1,1))
                   end if
                 ! if( debug.gt.0 )then
                 !   write(*,'(" ghost-interp:right-shift i=",3i3," js1a,js2a=",2i3," c2r,c2s2(+1),c2s2(+2)=",10e10.2)')!      i1,i2,i3,js1a,js2a,c2r,C2s2(i1+js1a,i2+js2a,i3),C2s2(i1+2*js1a,i2+2*js2a,i3)
                 ! end if
                   a11s = (-3.*(rsxy(i1,i2,i3,axis,0)/(rx(i1,i2,i3)*sy(
     & i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+4.*(rsxy(i1+js1a,i2+js2a,
     & i3,axis,0)/(rx(i1+js1a,i2+js2a,i3)*sy(i1+js1a,i2+js2a,i3)-ry(
     & i1+js1a,i2+js2a,i3)*sx(i1+js1a,i2+js2a,i3)))-(rsxy(i1+2*js1a,
     & i2+2*js2a,i3,axis,0)/(rx(i1+2*js1a,i2+2*js2a,i3)*sy(i1+2*js1a,
     & i2+2*js2a,i3)-ry(i1+2*js1a,i2+2*js2a,i3)*sx(i1+2*js1a,i2+2*
     & js2a,i3))))/(2.*dsb)
                   a12s = (-3.*(rsxy(i1,i2,i3,axis,1)/(rx(i1,i2,i3)*sy(
     & i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+4.*(rsxy(i1+js1a,i2+js2a,
     & i3,axis,1)/(rx(i1+js1a,i2+js2a,i3)*sy(i1+js1a,i2+js2a,i3)-ry(
     & i1+js1a,i2+js2a,i3)*sx(i1+js1a,i2+js2a,i3)))-(rsxy(i1+2*js1a,
     & i2+2*js2a,i3,axis,1)/(rx(i1+2*js1a,i2+2*js2a,i3)*sy(i1+2*js1a,
     & i2+2*js2a,i3)-ry(i1+2*js1a,i2+2*js2a,i3)*sx(i1+2*js1a,i2+2*
     & js2a,i3))))/(2.*dsb)
                   a21s = (-3.*(rsxy(i1,i2,i3,axisp1,0)/(rx(i1,i2,i3)*
     & sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+4.*(rsxy(i1+js1a,i2+
     & js2a,i3,axisp1,0)/(rx(i1+js1a,i2+js2a,i3)*sy(i1+js1a,i2+js2a,
     & i3)-ry(i1+js1a,i2+js2a,i3)*sx(i1+js1a,i2+js2a,i3)))-(rsxy(i1+2*
     & js1a,i2+2*js2a,i3,axisp1,0)/(rx(i1+2*js1a,i2+2*js2a,i3)*sy(i1+
     & 2*js1a,i2+2*js2a,i3)-ry(i1+2*js1a,i2+2*js2a,i3)*sx(i1+2*js1a,
     & i2+2*js2a,i3))))/(2.*dsb)
                   a22s = (-3.*(rsxy(i1,i2,i3,axisp1,1)/(rx(i1,i2,i3)*
     & sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+4.*(rsxy(i1+js1a,i2+
     & js2a,i3,axisp1,1)/(rx(i1+js1a,i2+js2a,i3)*sy(i1+js1a,i2+js2a,
     & i3)-ry(i1+js1a,i2+js2a,i3)*sx(i1+js1a,i2+js2a,i3)))-(rsxy(i1+2*
     & js1a,i2+2*js2a,i3,axisp1,1)/(rx(i1+2*js1a,i2+2*js2a,i3)*sy(i1+
     & 2*js1a,i2+2*js2a,i3)-ry(i1+2*js1a,i2+2*js2a,i3)*sx(i1+2*js1a,
     & i2+2*js2a,i3))))/(2.*dsb)
                  else
                   ! this case should not happen
                   stop 44066
                  end if
                  ! ***** Now do "s"-derivatives *****
                  ! warning -- the compiler could still try to evaluate the mask at an invalid point
                  if( (i1-js1a).ge.md1a .and. (i2-js2a).ge.md2a .and. 
     & mask(i1-js1a,i2-js2a,i3).ne.0 .and. (i1+js1a).le.md1b .and. (
     & i2+js2a).le.md2b .and. mask(i1+js1a,i2+js2a,i3).ne.0 )then
                    us=(u(i1+js1,i2+js2,i3+js3,ex)-u(i1-js1,i2-js2,i3-
     & js3,ex))/(2.*dsa)
                    vs=(u(i1+js1,i2+js2,i3+js3,ey)-u(i1-js1,i2-js2,i3-
     & js3,ey))/(2.*dsa)
                    ws=(u(i1+js1,i2+js2,i3+js3,hz)-u(i1-js1,i2-js2,i3-
     & js3,hz))/(2.*dsa)
                    uss=(u(i1+js1,i2+js2,i3+js3,ex)-2.*u(i1,i2,i3,ex)+
     & u(i1-js1,i2-js2,i3-js3,ex))/(dsa**2)
                    vss=(u(i1+js1,i2+js2,i3+js3,ey)-2.*u(i1,i2,i3,ey)+
     & u(i1-js1,i2-js2,i3-js3,ey))/(dsa**2)
                    wss=(u(i1+js1,i2+js2,i3+js3,hz)-2.*u(i1,i2,i3,hz)+
     & u(i1-js1,i2-js2,i3-js3,hz))/(dsa**2)
                  else if( (i1-2*js1a).ge.md1a .and. (i2-2*js2a)
     & .ge.md2a .and. mask(i1-js1a,i2-js2a,i3).ne.0 .and. mask(i1-2*
     & js1a,i2-2*js2a,i3).ne.0 )then
                   ! 2nd-order one-sided: ** note ** use ds not dsa
                   us = (-(-3.*u(i1,i2,i3,ex)+4.*u(i1-js1a,i2-js2a,i3-
     & 0,ex)-u(i1-2*js1a,i2-2*js2a,i3-2*0,ex))/(2.*dsb))
                   vs = (-(-3.*u(i1,i2,i3,ey)+4.*u(i1-js1a,i2-js2a,i3-
     & 0,ey)-u(i1-2*js1a,i2-2*js2a,i3-2*0,ey))/(2.*dsb))
                   ws = (-(-3.*u(i1,i2,i3,hz)+4.*u(i1-js1a,i2-js2a,i3-
     & 0,hz)-u(i1-2*js1a,i2-2*js2a,i3-2*0,hz))/(2.*dsb))
                   uss = ((2.*u(i1,i2,i3,ex)-5.*u(i1-js1a,i2-js2a,i3-0,
     & ex)+4.*u(i1-2*js1a,i2-2*js2a,i3-2*0,ex)-u(i1-3*js1a,i2-3*js2a,
     & i3-3*0,ex))/(dsb**2))
                   vss = ((2.*u(i1,i2,i3,ey)-5.*u(i1-js1a,i2-js2a,i3-0,
     & ey)+4.*u(i1-2*js1a,i2-2*js2a,i3-2*0,ey)-u(i1-3*js1a,i2-3*js2a,
     & i3-3*0,ey))/(dsb**2))
                   wss = ((2.*u(i1,i2,i3,hz)-5.*u(i1-js1a,i2-js2a,i3-0,
     & hz)+4.*u(i1-2*js1a,i2-2*js2a,i3-2*0,hz)-u(i1-3*js1a,i2-3*js2a,
     & i3-3*0,hz))/(dsb**2))
                  else if( (i1+2*js1a).le.md1b .and. (i2+2*js2a)
     & .le.md2b .and.  mask(i1+js1a,i2+js2a,i3).ne.0 .and. mask(i1+2*
     & js1a,i2+2*js2a,i3).ne.0 )then
                   ! 2nd-order one-sided:
                   us = ((-3.*u(i1,i2,i3,ex)+4.*u(i1+js1a,i2+js2a,i3+0,
     & ex)-u(i1+2*js1a,i2+2*js2a,i3+2*0,ex))/(2.*dsb))
                   vs = ((-3.*u(i1,i2,i3,ey)+4.*u(i1+js1a,i2+js2a,i3+0,
     & ey)-u(i1+2*js1a,i2+2*js2a,i3+2*0,ey))/(2.*dsb))
                   ws = ((-3.*u(i1,i2,i3,hz)+4.*u(i1+js1a,i2+js2a,i3+0,
     & hz)-u(i1+2*js1a,i2+2*js2a,i3+2*0,hz))/(2.*dsb))
                   uss = ((2.*u(i1,i2,i3,ex)-5.*u(i1+js1a,i2+js2a,i3+0,
     & ex)+4.*u(i1+2*js1a,i2+2*js2a,i3+2*0,ex)-u(i1+3*js1a,i2+3*js2a,
     & i3+3*0,ex))/(dsb**2))
                   vss = ((2.*u(i1,i2,i3,ey)-5.*u(i1+js1a,i2+js2a,i3+0,
     & ey)+4.*u(i1+2*js1a,i2+2*js2a,i3+2*0,ey)-u(i1+3*js1a,i2+3*js2a,
     & i3+3*0,ey))/(dsb**2))
                   wss = ((2.*u(i1,i2,i3,hz)-5.*u(i1+js1a,i2+js2a,i3+0,
     & hz)+4.*u(i1+2*js1a,i2+2*js2a,i3+2*0,hz)-u(i1+3*js1a,i2+3*js2a,
     & i3+3*0,hz))/(dsb**2))
                  else
                    ! this case shouldn't matter
                    us=0.
                    vs=0.
                    ws=0.
                    uss=0.
                    vss=0.
                    wss=0.
                  end if
                 ! ******************************* end NEW ************************
                  tau1=rsxy(i1,i2,i3,axisp1,0)
                  tau2=rsxy(i1,i2,i3,axisp1,1)
                  uex=u(i1,i2,i3,ex)
                  uey=u(i1,i2,i3,ey)
                  ! forcing terms for TZ are stored in 
                  gIVf=0.            ! forcing for extrap tau.u
                  tau1DotUtt=0.      ! forcing for tau.Lu=0
                  Da1DotU=0.         ! forcing for div(u)=0
                  ! for Hz (w)
                  fw1=0.
                  fw2=0.
                 ! assign values using extrapolation of the normal component:


! ************ Answer *******************
      gIII=-tau1*(c22*uss+c2*us)-tau2*(c22*vss+c2*vs)

      tauU=tau1*u(i1,i2,i3,ex)+tau2*u(i1,i2,i3,ey)

      tauUp1=tau1*u(i1+is1,i2+is2,i3+is3,ex)+tau2*u(i1+is1,i2+is2,i3+
     & is3,ey)

      tauUp2=tau1*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)+tau2*u(i1+2*is1,i2+
     & 2*is2,i3+2*is3,ey)

      gIV=-6*tauU+4*tauUp1-tauUp2 +gIVf

      ttu1=1/4.*(16*c11*tauUp1-30*c11*tauU-c11*tauUp2-c11*gIV+8*c1*dra*
     & tauUp1-c1*dra*tauUp2+c1*dra*gIV-12*gIII*dra**2-12*tau1DotUtt*
     & dra**2)/(-3*c11+c1*dra)
      ttu2=(16*c11*tauUp1-30*c11*tauU-c11*tauUp2-4*c11*gIV+8*c1*dra*
     & tauUp1-c1*dra*tauUp2+2*c1*dra*gIV-12*gIII*dra**2-12*tau1DotUtt*
     & dra**2)/(-3*c11+c1*dra)

      u(i1-is1,i2-is2,i3-is3,ex) = -1.*(-12.*a12*tau1*tau2*Da1DotU*dra+
     & 12.*tau2**2*a11*Da1DotU*dra-1.*tau2**2*a11*u(i1+3*is1,i2+3*is2,
     & i3+3*is3,ex)*a11m2+tau2*a12*u(i1+3*is1,i2+3*is2,i3+3*is3,ey)*
     & a12m2*tau1-1.*tau2**2*a12*u(i1+3*is1,i2+3*is2,i3+3*is3,ey)*
     & a11m2+tau2*a11*u(i1+3*is1,i2+3*is2,i3+3*is3,ex)*a12m2*tau1-5.*
     & tau2*a12*u(i1+2*is1,i2+2*is2,i3+2*is3,ey)*a12m2*tau1-1.*a12*
     & tau1*tau2*a12p2*u(i1+2*is1,i2+2*is2,i3+2*is3,ey)+5.*tau2**2*
     & a12*u(i1+2*is1,i2+2*is2,i3+2*is3,ey)*a11m2+tau2**2*a11*a12p2*u(
     & i1+2*is1,i2+2*is2,i3+2*is3,ey)-5.*tau2*a11*u(i1+2*is1,i2+2*is2,
     & i3+2*is3,ex)*a12m2*tau1+5.*tau2**2*a11*u(i1+2*is1,i2+2*is2,i3+
     & 2*is3,ex)*a11m2-1.*a12*tau1*tau2*a11p2*u(i1+2*is1,i2+2*is2,i3+
     & 2*is3,ex)+tau2**2*a11*a11p2*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)-
     & 8.*a12*tau1*a12m1*ttu1+8.*tau2*a11*a12m1*ttu1-1.*tau2*a11*
     & a12m2*ttu2+5.*a12*ttu1*a12m2*tau1-5.*a12*ttu1*tau2*a11m2+a12*
     & ttu2*tau2*a11m2-10.*tau2*a12*u(i1,i2,i3,ey)*a12m2*tau1+10.*
     & tau2**2*a12*u(i1,i2,i3,ey)*a11m2+10.*tau2**2*a11*u(i1,i2,i3,ex)
     & *a11m2-10.*tau2*a11*u(i1,i2,i3,ex)*a12m2*tau1+8.*a12*tau1*tau2*
     & a11p1*u(i1+is1,i2+is2,i3+is3,ex)-8.*tau2**2*a11*a11p1*u(i1+is1,
     & i2+is2,i3+is3,ex)+10.*tau2*a11*u(i1+is1,i2+is2,i3+is3,ex)*
     & a12m2*tau1-10.*tau2**2*a11*u(i1+is1,i2+is2,i3+is3,ex)*a11m2+8.*
     & a12*tau1*tau2*a12p1*u(i1+is1,i2+is2,i3+is3,ey)-10.*tau2**2*a12*
     & u(i1+is1,i2+is2,i3+is3,ey)*a11m2-8.*tau2**2*a11*a12p1*u(i1+is1,
     & i2+is2,i3+is3,ey)+10.*tau2*a12*u(i1+is1,i2+is2,i3+is3,ey)*
     & a12m2*tau1)/(5.*tau2*a11*a12m2*tau1-5.*tau2**2*a11*a11m2-5.*
     & a12*tau1**2*a12m2+5.*a12*tau1*tau2*a11m2-8.*tau2*a11*a12m1*
     & tau1-8.*a12*tau1*tau2*a11m1+8.*tau2**2*a11*a11m1+8.*a12*tau1**
     & 2*a12m1)

      u(i1-is1,i2-is2,i3-is3,ey) = -1.*(8.*tau1*a11m1*a12*ttu1-8.*ttu1*
     & tau2*a11*a11m1+5.*ttu1*tau2*a11*a11m2-1.*tau1*a12*ttu2*a11m2+
     & tau1*a11*a12m2*ttu2-5.*ttu1*a11*a12m2*tau1+10.*a11*u(i1,i2,i3,
     & ex)*a12m2*tau1**2-10.*tau1*tau2*a11*u(i1,i2,i3,ex)*a11m2-10.*
     & a12*u(i1+is1,i2+is2,i3+is3,ey)*a12m2*tau1**2-8.*a12*tau1**2*
     & a12p1*u(i1+is1,i2+is2,i3+is3,ey)+8.*tau1*tau2*a11*a12p1*u(i1+
     & is1,i2+is2,i3+is3,ey)+10.*tau1*tau2*a12*u(i1+is1,i2+is2,i3+is3,
     & ey)*a11m2-10.*a11*u(i1+is1,i2+is2,i3+is3,ex)*a12m2*tau1**2+10.*
     & tau1*tau2*a11*u(i1+is1,i2+is2,i3+is3,ex)*a11m2+8.*tau1*tau2*
     & a11*a11p1*u(i1+is1,i2+is2,i3+is3,ex)-8.*a12*tau1**2*a11p1*u(i1+
     & is1,i2+is2,i3+is3,ex)-10.*tau1*tau2*a12*u(i1,i2,i3,ey)*a11m2+
     & 10.*a12*u(i1,i2,i3,ey)*a12m2*tau1**2-5.*tau1*tau2*a11*u(i1+2*
     & is1,i2+2*is2,i3+2*is3,ex)*a11m2-1.*tau1*tau2*a11*a11p2*u(i1+2*
     & is1,i2+2*is2,i3+2*is3,ex)+a12*tau1**2*a11p2*u(i1+2*is1,i2+2*
     & is2,i3+2*is3,ex)+5.*a11*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)*a12m2*
     & tau1**2+a12*tau1**2*a12p2*u(i1+2*is1,i2+2*is2,i3+2*is3,ey)+5.*
     & a12*u(i1+2*is1,i2+2*is2,i3+2*is3,ey)*a12m2*tau1**2-1.*tau1*
     & tau2*a11*a12p2*u(i1+2*is1,i2+2*is2,i3+2*is3,ey)-5.*tau1*tau2*
     & a12*u(i1+2*is1,i2+2*is2,i3+2*is3,ey)*a11m2-1.*a11*u(i1+3*is1,
     & i2+3*is2,i3+3*is3,ex)*a12m2*tau1**2+tau1*tau2*a11*u(i1+3*is1,
     & i2+3*is2,i3+3*is3,ex)*a11m2+tau1*tau2*a12*u(i1+3*is1,i2+3*is2,
     & i3+3*is3,ey)*a11m2-1.*a12*u(i1+3*is1,i2+3*is2,i3+3*is3,ey)*
     & a12m2*tau1**2+12.*a12*tau1**2*Da1DotU*dra-12.*tau1*tau2*a11*
     & Da1DotU*dra)/(5.*tau2*a11*a12m2*tau1-5.*tau2**2*a11*a11m2-5.*
     & a12*tau1**2*a12m2+5.*a12*tau1*tau2*a11m2-8.*tau2*a11*a12m1*
     & tau1-8.*a12*tau1*tau2*a11m1+8.*tau2**2*a11*a11m1+8.*a12*tau1**
     & 2*a12m1)

      u(i1-2*is1,i2-2*is2,i3-2*is3,ex) = -1.*(-40.*tau2*a12m1*tau1*a12*
     & u(i1+2*is1,i2+2*is2,i3+2*is3,ey)-40.*tau2*a11m1*a12*ttu1-80.*
     & tau2**2*a11m1*a12*u(i1+is1,i2+is2,i3+is3,ey)-5.*a12*tau1*tau2*
     & a12p2*u(i1+2*is1,i2+2*is2,i3+2*is3,ey)+5.*tau2**2*a11*a12p2*u(
     & i1+2*is1,i2+2*is2,i3+2*is3,ey)-60.*a12*tau1*tau2*Da1DotU*dra+
     & 60.*tau2**2*a11*Da1DotU*dra-8.*tau2**2*a11m1*a11*u(i1+3*is1,i2+
     & 3*is2,i3+3*is3,ex)-8.*tau2**2*a11m1*a12*u(i1+3*is1,i2+3*is2,i3+
     & 3*is3,ey)+8.*tau2*a12m1*tau1*a12*u(i1+3*is1,i2+3*is2,i3+3*is3,
     & ey)+8.*tau2*a12m1*tau1*a11*u(i1+3*is1,i2+3*is2,i3+3*is3,ex)+
     & 40.*tau2**2*a11m1*a11*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)+5.*tau2*
     & *2*a11*a11p2*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)+40.*tau2**2*
     & a11m1*a12*u(i1+2*is1,i2+2*is2,i3+2*is3,ey)-40.*tau2*a12m1*tau1*
     & a11*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)-5.*a12*tau1*tau2*a11p2*u(
     & i1+2*is1,i2+2*is2,i3+2*is3,ex)-8.*a12m1*tau1*a12*ttu2+40.*tau2*
     & a11*a12m1*ttu1+8.*tau2*a11m1*a12*ttu2-5.*tau2*a11*a12m2*ttu2+
     & 5.*a12*ttu2*a12m2*tau1+80.*tau2**2*a11m1*a12*u(i1,i2,i3,ey)-
     & 80.*tau2*a12m1*tau1*a11*u(i1,i2,i3,ex)+80.*tau2**2*a11m1*a11*u(
     & i1,i2,i3,ex)-80.*tau2**2*a11m1*a11*u(i1+is1,i2+is2,i3+is3,ex)-
     & 80.*tau2*a12m1*tau1*a12*u(i1,i2,i3,ey)+40.*a12*tau1*tau2*a12p1*
     & u(i1+is1,i2+is2,i3+is3,ey)+80.*tau2*a12m1*tau1*a12*u(i1+is1,i2+
     & is2,i3+is3,ey)-40.*tau2**2*a11*a12p1*u(i1+is1,i2+is2,i3+is3,ey)
     & +80.*tau2*a12m1*tau1*a11*u(i1+is1,i2+is2,i3+is3,ex)+40.*a12*
     & tau1*tau2*a11p1*u(i1+is1,i2+is2,i3+is3,ex)-40.*tau2**2*a11*
     & a11p1*u(i1+is1,i2+is2,i3+is3,ex))/(5.*tau2*a11*a12m2*tau1-5.*
     & tau2**2*a11*a11m2-5.*a12*tau1**2*a12m2+5.*a12*tau1*tau2*a11m2-
     & 8.*tau2*a11*a12m1*tau1-8.*a12*tau1*tau2*a11m1+8.*tau2**2*a11*
     & a11m1+8.*a12*tau1**2*a12m1)

      u(i1-2*is1,i2-2*is2,i3-2*is3,ey) = (40.*tau1*a11*a12m1*ttu1-8.*
     & ttu2*a11*a12m1*tau1-40.*tau1*a11m1*a12*ttu1+8.*ttu2*tau2*a11*
     & a11m1+5.*tau1*a12*ttu2*a11m2-5.*ttu2*tau2*a11*a11m2-40.*tau1*
     & tau2*a11*a12p1*u(i1+is1,i2+is2,i3+is3,ey)+40.*a12*tau1**2*
     & a12p1*u(i1+is1,i2+is2,i3+is3,ey)-80.*tau1*tau2*a11m1*a12*u(i1+
     & is1,i2+is2,i3+is3,ey)-80.*tau1*tau2*a11m1*a11*u(i1+is1,i2+is2,
     & i3+is3,ex)-40.*tau1*tau2*a11*a11p1*u(i1+is1,i2+is2,i3+is3,ex)+
     & 80.*a12m1*tau1**2*a12*u(i1+is1,i2+is2,i3+is3,ey)+80.*a12m1*
     & tau1**2*a11*u(i1+is1,i2+is2,i3+is3,ex)+40.*a12*tau1**2*a11p1*u(
     & i1+is1,i2+is2,i3+is3,ex)+80.*tau1*tau2*a11m1*a12*u(i1,i2,i3,ey)
     & -80.*a12m1*tau1**2*a12*u(i1,i2,i3,ey)+80.*tau1*tau2*a11m1*a11*
     & u(i1,i2,i3,ex)-80.*a12m1*tau1**2*a11*u(i1,i2,i3,ex)+5.*tau1*
     & tau2*a11*a11p2*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)-5.*a12*tau1**2*
     & a11p2*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)+40.*tau1*tau2*a11m1*a11*
     & u(i1+2*is1,i2+2*is2,i3+2*is3,ex)-40.*a12m1*tau1**2*a11*u(i1+2*
     & is1,i2+2*is2,i3+2*is3,ex)-60.*a12*tau1**2*Da1DotU*dra+60.*tau1*
     & tau2*a11*Da1DotU*dra+40.*tau1*tau2*a11m1*a12*u(i1+2*is1,i2+2*
     & is2,i3+2*is3,ey)+5.*tau1*tau2*a11*a12p2*u(i1+2*is1,i2+2*is2,i3+
     & 2*is3,ey)-5.*a12*tau1**2*a12p2*u(i1+2*is1,i2+2*is2,i3+2*is3,ey)
     & -40.*a12m1*tau1**2*a12*u(i1+2*is1,i2+2*is2,i3+2*is3,ey)-8.*
     & tau1*tau2*a11m1*a12*u(i1+3*is1,i2+3*is2,i3+3*is3,ey)+8.*a12m1*
     & tau1**2*a12*u(i1+3*is1,i2+3*is2,i3+3*is3,ey)-8.*tau1*tau2*
     & a11m1*a11*u(i1+3*is1,i2+3*is2,i3+3*is3,ex)+8.*a12m1*tau1**2*
     & a11*u(i1+3*is1,i2+3*is2,i3+3*is3,ex))/(5.*tau2*a11*a12m2*tau1-
     & 5.*tau2**2*a11*a11m2-5.*a12*tau1**2*a12m2+5.*a12*tau1*tau2*
     & a11m2-8.*tau2*a11*a12m1*tau1-8.*a12*tau1*tau2*a11m1+8.*tau2**2*
     & a11*a11m1+8.*a12*tau1**2*a12m1)


 ! *********** done *********************
                 ! Now assign Hz at the ghost points


! ************ Hz Answer *******************
      cw2=c1+c11r
      cw1=c1r
      bfw2=c22r*wss+c2r*ws-fw2

      u(i1-is1,i2-is2,i3,hz) = 1/2.*(-18*c11*u(i1+is1,i2+is2,i3,hz)+36*
     & c11*fw1*dra-12*cw2*dra*u(i1+is1,i2+is2,i3,hz)+15*cw2*dra*u(i1,
     & i2,i3,hz)+cw2*dra*u(i1+2*is1,i2+2*is2,i3,hz)+6*cw2*dra**2*fw1-
     & 6*cw1*dra**3*fw1-6*bfw2*dra**3)/(-9*c11+2*cw2*dra)

      u(i1-2*is1,i2-2*is2,i3,hz) = (-64*cw2*dra*u(i1+is1,i2+is2,i3,hz)+
     & 36*c11*fw1*dra+60*cw2*dra*u(i1,i2,i3,hz)+6*cw2*dra*u(i1+2*is1,
     & i2+2*is2,i3,hz)+48*cw2*dra**2*fw1-24*cw1*dra**3*fw1-24*bfw2*
     & dra**3-9*c11*u(i1+2*is1,i2+2*is2,i3,hz))/(-9*c11+2*cw2*dra)


 ! *********** Hz done *********************
                  if( debug.gt.0 )then
                   write(*,'(" ghost-interp: i=",3i3," ex=",e10.2," 
     & assign i=",3i3," ex=",e10.2," i=",3i3," ex=",e10.2)')i1,i2,i3,
     & u(i1,i2,i3,ex),i1-is1,i2-is2,i3-is3,u(i1-is1,i2-is2,i3-is3,ex),
     & i1-2*is1,i2-2*is2,i3-2*is3,u(i1-2*is1,i2-2*is2,i3-2*is3,ex)
                   det = (5.*tau2*a11*a12m2*tau1-5.*tau2**2*a11*a11m2-
     & 5.*a12*tau1**2*a12m2+5.*a12*tau1*tau2*a11m2-8.*tau2*a11*a12m1*
     & tau1-8.*a12*tau1*tau2*a11m1+8.*tau2**2*a11*a11m1+8.*a12*tau1**
     & 2*a12m1)
                   write(*,'(" ghost-interp: det=",e10.2," tau1,tau2,
     & a11,a11m1,a11m2,a12,a12m1,a12m2=",10f8.4)') det,tau1,tau2,a11,
     & a11m1,a11m2,a12,a12m1,a12m2
                   write(*,'(" ghost-interp: gIII,tauU,tauUp1,tauUp2,
     & gIV,ttu1,ttu2,c11,c1,dra=",10f8.3)') gIII,tauU,tauUp1,tauUp2,
     & gIV,ttu1,ttu2,c11,c1,dra
                   write(*,'(" ghost-interp: c1r,c2r,c22,uss,c2,us,vss,
     & vs=",10e10.2)') c1r,c2r,c22,uss,c2,us,vss,vs
                    call ogf2d(ep,xy(i1-is1,i2-is2,i3,0),xy(i1-is1,i2-
     & is2,i3,1),t,uvm(0),uvm(1),uvm(2))
                    call ogf2d(ep,xy(i1-2*is1,i2-2*is2,i3,0),xy(i1-2*
     & is1,i2-2*is2,i3,1),t,uvm2(0),uvm2(1),uvm2(2))
                   write(*,'(" .............tau1DotUtt,Da1DotU,us,
     & uss=",4e11.3)') tau1DotUtt,Da1DotU,us,uss
                   write(*,'(" .............err: ex(-1,-2)=",2e10.3,", 
     & ey(-1,-2)=",2e10.2,", hz(-1,-2)=",2e10.2)') u(i1-is1,i2-is2,i3-
     & is3,ex)-uvm(0),u(i1-2*is1,i2-2*is2,i3-2*is3,ex)-uvm2(0),u(i1-
     & is1,i2-is2,i3-is3,ey)-uvm(1),u(i1-2*is1,i2-2*is2,i3-2*is3,ey)-
     & uvm2(1),u(i1-is1,i2-is2,i3-is3,hz)-uvm(2),u(i1-2*is1,i2-2*is2,
     & i3-2*is3,hz)-uvm2(2)
                  end if
                  ! ** NO NEED TO DO ALL THE ABOVE IF WE DO THIS:
                  extrapInterpGhost=.true.
                  if( extrapInterpGhost )then
                    ! extrapolate ghost points next to boundary interpolation points  *wdh* 2015/05/30 
                    write(*,'(" extrap ghost next to interp")')
                    u(i1-is1,i2-is2,i3-is3,ex) = (5.*u(i1,i2,i3,ex)-
     & 10.*u(i1+is1,i2+is2,i3+is3,ex)+10.*u(i1+2*is1,i2+2*is2,i3+2*
     & is3,ex)-5.*u(i1+3*is1,i2+3*is2,i3+3*is3,ex)+u(i1+4*is1,i2+4*
     & is2,i3+4*is3,ex))
                    u(i1-is1,i2-is2,i3-is3,ey) = (5.*u(i1,i2,i3,ey)-
     & 10.*u(i1+is1,i2+is2,i3+is3,ey)+10.*u(i1+2*is1,i2+2*is2,i3+2*
     & is3,ey)-5.*u(i1+3*is1,i2+3*is2,i3+3*is3,ey)+u(i1+4*is1,i2+4*
     & is2,i3+4*is3,ey))
                    u(i1-is1,i2-is2,i3-is3,hz) = (5.*u(i1,i2,i3,hz)-
     & 10.*u(i1+is1,i2+is2,i3+is3,hz)+10.*u(i1+2*is1,i2+2*is2,i3+2*
     & is3,hz)-5.*u(i1+3*is1,i2+3*is2,i3+3*is3,hz)+u(i1+4*is1,i2+4*
     & is2,i3+4*is3,hz))
                    u(i1-2*is1,i2-2*is2,i3-2*is3,ex) = (5.*u(i1-is1,i2-
     & is2,i3-is3,ex)-10.*u(i1-is1+is1,i2-is2+is2,i3-is3+is3,ex)+10.*
     & u(i1-is1+2*is1,i2-is2+2*is2,i3-is3+2*is3,ex)-5.*u(i1-is1+3*is1,
     & i2-is2+3*is2,i3-is3+3*is3,ex)+u(i1-is1+4*is1,i2-is2+4*is2,i3-
     & is3+4*is3,ex))
                    u(i1-2*is1,i2-2*is2,i3-2*is3,ey) = (5.*u(i1-is1,i2-
     & is2,i3-is3,ey)-10.*u(i1-is1+is1,i2-is2+is2,i3-is3+is3,ey)+10.*
     & u(i1-is1+2*is1,i2-is2+2*is2,i3-is3+2*is3,ey)-5.*u(i1-is1+3*is1,
     & i2-is2+3*is2,i3-is3+3*is3,ey)+u(i1-is1+4*is1,i2-is2+4*is2,i3-
     & is3+4*is3,ey))
                    u(i1-2*is1,i2-2*is2,i3-2*is3,hz) = (5.*u(i1-is1,i2-
     & is2,i3-is3,hz)-10.*u(i1-is1+is1,i2-is2+is2,i3-is3+is3,hz)+10.*
     & u(i1-is1+2*is1,i2-is2+2*is2,i3-is3+2*is3,hz)-5.*u(i1-is1+3*is1,
     & i2-is2+3*is2,i3-is3+3*is3,hz)+u(i1-is1+4*is1,i2-is2+4*is2,i3-
     & is3+4*is3,hz))
                  end if
                 end if ! mask>0
                 end do
                 end do
                 end do
                 if( debug.gt.0 )then
                 ! ============================DEBUG=======================================================
                 ! ============================END DEBUG=======================================================
                 end if
               else
                  ! assign values on boundary when there are boundary forcings
                  !! assignBoundaryForcingBoundaryValuesCurvilinear(2)
                  ! Since is1 is +1 or -1 we need to flip the sign of dr in the derivative approximations
                  dra = dr(axis)*(1-2*side)
                  dsa = dr(axisp1)*(1-2*side)
                  drb = dr(axis  )
                  dsb = dr(axisp1)
                  if( debug .gt.0 )then
                   write(*,'(" ******* Start: grid=",i2," side,axis=",
     & 2i2)') grid,side,axis
                  end if
                  do i3=n3a,n3b
                  do i2=n2a,n2b
                  do i1=n1a,n1b
                  if( mask(i1,i2,i3).gt.0 )then
                   jacm1=1./(rx(i1-is1,i2-is2,i3)*sy(i1-is1,i2-is2,i3)-
     & ry(i1-is1,i2-is2,i3)*sx(i1-is1,i2-is2,i3))
                   a11m1 =rsxy(i1-is1,i2-is2,i3,axis  ,0)*jacm1
                   a12m1 =rsxy(i1-is1,i2-is2,i3,axis  ,1)*jacm1
                   jac=1./(rx(i1,i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)*sx(
     & i1,i2,i3))
                   a11 =rsxy(i1,i2,i3,axis  ,0)*jac
                   a12 =rsxy(i1,i2,i3,axis  ,1)*jac
                   a21 =rsxy(i1,i2,i3,axisp1,0)*jac
                   a22 =rsxy(i1,i2,i3,axisp1,1)*jac
                   jacp1=1./(rx(i1+is1,i2+is2,i3)*sy(i1+is1,i2+is2,i3)-
     & ry(i1+is1,i2+is2,i3)*sx(i1+is1,i2+is2,i3))
                   a11p1=rsxy(i1+is1,i2+is2,i3,axis,0)*jacp1
                   a12p1=rsxy(i1+is1,i2+is2,i3,axis,1)*jacp1
                   jacm2=1./(rx(i1-2*is1,i2-2*is2,i3)*sy(i1-2*is1,i2-2*
     & is2,i3)-ry(i1-2*is1,i2-2*is2,i3)*sx(i1-2*is1,i2-2*is2,i3))
                   a11m2=rsxy(i1-2*is1,i2-2*is2,i3,axis,0)*jacm2
                   a12m2=rsxy(i1-2*is1,i2-2*is2,i3,axis,1)*jacm2
                   jacp2=1./(rx(i1+2*is1,i2+2*is2,i3)*sy(i1+2*is1,i2+2*
     & is2,i3)-ry(i1+2*is1,i2+2*is2,i3)*sx(i1+2*is1,i2+2*is2,i3))
                   a11p2=rsxy(i1+2*is1,i2+2*is2,i3,axis,0)*jacp2
                   a12p2=rsxy(i1+2*is1,i2+2*is2,i3,axis,1)*jacp2
                  a11r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axis,0)/(rx(
     & i1+is1,i2+is2,i3+is3)*sy(i1+is1,i2+is2,i3+is3)-ry(i1+is1,i2+
     & is2,i3+is3)*sx(i1+is1,i2+is2,i3+is3)))-(rsxy(i1-is1,i2-is2,i3-
     & is3,axis,0)/(rx(i1-is1,i2-is2,i3-is3)*sy(i1-is1,i2-is2,i3-is3)-
     & ry(i1-is1,i2-is2,i3-is3)*sx(i1-is1,i2-is2,i3-is3))))-((rsxy(i1+
     & 2*is1,i2+2*is2,i3+2*is3,axis,0)/(rx(i1+2*is1,i2+2*is2,i3+2*is3)
     & *sy(i1+2*is1,i2+2*is2,i3+2*is3)-ry(i1+2*is1,i2+2*is2,i3+2*is3)*
     & sx(i1+2*is1,i2+2*is2,i3+2*is3)))-(rsxy(i1-2*is1,i2-2*is2,i3-2*
     & is3,axis,0)/(rx(i1-2*is1,i2-2*is2,i3-2*is3)*sy(i1-2*is1,i2-2*
     & is2,i3-2*is3)-ry(i1-2*is1,i2-2*is2,i3-2*is3)*sx(i1-2*is1,i2-2*
     & is2,i3-2*is3)))))/(12.*dra)
                  a12r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axis,1)/(rx(
     & i1+is1,i2+is2,i3+is3)*sy(i1+is1,i2+is2,i3+is3)-ry(i1+is1,i2+
     & is2,i3+is3)*sx(i1+is1,i2+is2,i3+is3)))-(rsxy(i1-is1,i2-is2,i3-
     & is3,axis,1)/(rx(i1-is1,i2-is2,i3-is3)*sy(i1-is1,i2-is2,i3-is3)-
     & ry(i1-is1,i2-is2,i3-is3)*sx(i1-is1,i2-is2,i3-is3))))-((rsxy(i1+
     & 2*is1,i2+2*is2,i3+2*is3,axis,1)/(rx(i1+2*is1,i2+2*is2,i3+2*is3)
     & *sy(i1+2*is1,i2+2*is2,i3+2*is3)-ry(i1+2*is1,i2+2*is2,i3+2*is3)*
     & sx(i1+2*is1,i2+2*is2,i3+2*is3)))-(rsxy(i1-2*is1,i2-2*is2,i3-2*
     & is3,axis,1)/(rx(i1-2*is1,i2-2*is2,i3-2*is3)*sy(i1-2*is1,i2-2*
     & is2,i3-2*is3)-ry(i1-2*is1,i2-2*is2,i3-2*is3)*sx(i1-2*is1,i2-2*
     & is2,i3-2*is3)))))/(12.*dra)
                  a21r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axisp1,0)/(rx(
     & i1+is1,i2+is2,i3+is3)*sy(i1+is1,i2+is2,i3+is3)-ry(i1+is1,i2+
     & is2,i3+is3)*sx(i1+is1,i2+is2,i3+is3)))-(rsxy(i1-is1,i2-is2,i3-
     & is3,axisp1,0)/(rx(i1-is1,i2-is2,i3-is3)*sy(i1-is1,i2-is2,i3-
     & is3)-ry(i1-is1,i2-is2,i3-is3)*sx(i1-is1,i2-is2,i3-is3))))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp1,0)/(rx(i1+2*is1,i2+2*
     & is2,i3+2*is3)*sy(i1+2*is1,i2+2*is2,i3+2*is3)-ry(i1+2*is1,i2+2*
     & is2,i3+2*is3)*sx(i1+2*is1,i2+2*is2,i3+2*is3)))-(rsxy(i1-2*is1,
     & i2-2*is2,i3-2*is3,axisp1,0)/(rx(i1-2*is1,i2-2*is2,i3-2*is3)*sy(
     & i1-2*is1,i2-2*is2,i3-2*is3)-ry(i1-2*is1,i2-2*is2,i3-2*is3)*sx(
     & i1-2*is1,i2-2*is2,i3-2*is3)))))/(12.*dra)
                  a22r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axisp1,1)/(rx(
     & i1+is1,i2+is2,i3+is3)*sy(i1+is1,i2+is2,i3+is3)-ry(i1+is1,i2+
     & is2,i3+is3)*sx(i1+is1,i2+is2,i3+is3)))-(rsxy(i1-is1,i2-is2,i3-
     & is3,axisp1,1)/(rx(i1-is1,i2-is2,i3-is3)*sy(i1-is1,i2-is2,i3-
     & is3)-ry(i1-is1,i2-is2,i3-is3)*sx(i1-is1,i2-is2,i3-is3))))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp1,1)/(rx(i1+2*is1,i2+2*
     & is2,i3+2*is3)*sy(i1+2*is1,i2+2*is2,i3+2*is3)-ry(i1+2*is1,i2+2*
     & is2,i3+2*is3)*sx(i1+2*is1,i2+2*is2,i3+2*is3)))-(rsxy(i1-2*is1,
     & i2-2*is2,i3-2*is3,axisp1,1)/(rx(i1-2*is1,i2-2*is2,i3-2*is3)*sy(
     & i1-2*is1,i2-2*is2,i3-2*is3)-ry(i1-2*is1,i2-2*is2,i3-2*is3)*sx(
     & i1-2*is1,i2-2*is2,i3-2*is3)))))/(12.*dra)
                  a11s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axis,0)/(rx(
     & i1+js1,i2+js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+
     & js2,i3+js3)*sx(i1+js1,i2+js2,i3+js3)))-(rsxy(i1-js1,i2-js2,i3-
     & js3,axis,0)/(rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-
     & ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3))))-((rsxy(i1+
     & 2*js1,i2+2*js2,i3+2*js3,axis,0)/(rx(i1+2*js1,i2+2*js2,i3+2*js3)
     & *sy(i1+2*js1,i2+2*js2,i3+2*js3)-ry(i1+2*js1,i2+2*js2,i3+2*js3)*
     & sx(i1+2*js1,i2+2*js2,i3+2*js3)))-(rsxy(i1-2*js1,i2-2*js2,i3-2*
     & js3,axis,0)/(rx(i1-2*js1,i2-2*js2,i3-2*js3)*sy(i1-2*js1,i2-2*
     & js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,i3-2*js3)*sx(i1-2*js1,i2-2*
     & js2,i3-2*js3)))))/(12.*dsa)
                  a12s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axis,1)/(rx(
     & i1+js1,i2+js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+
     & js2,i3+js3)*sx(i1+js1,i2+js2,i3+js3)))-(rsxy(i1-js1,i2-js2,i3-
     & js3,axis,1)/(rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-
     & ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3))))-((rsxy(i1+
     & 2*js1,i2+2*js2,i3+2*js3,axis,1)/(rx(i1+2*js1,i2+2*js2,i3+2*js3)
     & *sy(i1+2*js1,i2+2*js2,i3+2*js3)-ry(i1+2*js1,i2+2*js2,i3+2*js3)*
     & sx(i1+2*js1,i2+2*js2,i3+2*js3)))-(rsxy(i1-2*js1,i2-2*js2,i3-2*
     & js3,axis,1)/(rx(i1-2*js1,i2-2*js2,i3-2*js3)*sy(i1-2*js1,i2-2*
     & js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,i3-2*js3)*sx(i1-2*js1,i2-2*
     & js2,i3-2*js3)))))/(12.*dsa)
                  a21s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axisp1,0)/(rx(
     & i1+js1,i2+js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+
     & js2,i3+js3)*sx(i1+js1,i2+js2,i3+js3)))-(rsxy(i1-js1,i2-js2,i3-
     & js3,axisp1,0)/(rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-
     & js3)-ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3))))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,0)/(rx(i1+2*js1,i2+2*
     & js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,i3+2*js3)-ry(i1+2*js1,i2+2*
     & js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,i3+2*js3)))-(rsxy(i1-2*js1,
     & i2-2*js2,i3-2*js3,axisp1,0)/(rx(i1-2*js1,i2-2*js2,i3-2*js3)*sy(
     & i1-2*js1,i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,i3-2*js3)*sx(
     & i1-2*js1,i2-2*js2,i3-2*js3)))))/(12.*dsa)
                  a22s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axisp1,1)/(rx(
     & i1+js1,i2+js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+
     & js2,i3+js3)*sx(i1+js1,i2+js2,i3+js3)))-(rsxy(i1-js1,i2-js2,i3-
     & js3,axisp1,1)/(rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-
     & js3)-ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3))))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,1)/(rx(i1+2*js1,i2+2*
     & js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,i3+2*js3)-ry(i1+2*js1,i2+2*
     & js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,i3+2*js3)))-(rsxy(i1-2*js1,
     & i2-2*js2,i3-2*js3,axisp1,1)/(rx(i1-2*js1,i2-2*js2,i3-2*js3)*sy(
     & i1-2*js1,i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,i3-2*js3)*sx(
     & i1-2*js1,i2-2*js2,i3-2*js3)))))/(12.*dsa)
                  a11rs = (8.*((8.*((rsxy(i1+1,i2+1,i3,axis,0)/(rx(i1+
     & 1,i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+1,i3)))
     & -(rsxy(i1-1,i2+1,i3,axis,0)/(rx(i1-1,i2+1,i3)*sy(i1-1,i2+1,i3)-
     & ry(i1-1,i2+1,i3)*sx(i1-1,i2+1,i3))))-((rsxy(i1+2,i2+1,i3,axis,
     & 0)/(rx(i1+2,i2+1,i3)*sy(i1+2,i2+1,i3)-ry(i1+2,i2+1,i3)*sx(i1+2,
     & i2+1,i3)))-(rsxy(i1-2,i2+1,i3,axis,0)/(rx(i1-2,i2+1,i3)*sy(i1-
     & 2,i2+1,i3)-ry(i1-2,i2+1,i3)*sx(i1-2,i2+1,i3)))))/(12.*dr(0))-(
     & 8.*((rsxy(i1+1,i2-1,i3,axis,0)/(rx(i1+1,i2-1,i3)*sy(i1+1,i2-1,
     & i3)-ry(i1+1,i2-1,i3)*sx(i1+1,i2-1,i3)))-(rsxy(i1-1,i2-1,i3,
     & axis,0)/(rx(i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-ry(i1-1,i2-1,i3)*sx(
     & i1-1,i2-1,i3))))-((rsxy(i1+2,i2-1,i3,axis,0)/(rx(i1+2,i2-1,i3)*
     & sy(i1+2,i2-1,i3)-ry(i1+2,i2-1,i3)*sx(i1+2,i2-1,i3)))-(rsxy(i1-
     & 2,i2-1,i3,axis,0)/(rx(i1-2,i2-1,i3)*sy(i1-2,i2-1,i3)-ry(i1-2,
     & i2-1,i3)*sx(i1-2,i2-1,i3)))))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2+
     & 2,i3,axis,0)/(rx(i1+1,i2+2,i3)*sy(i1+1,i2+2,i3)-ry(i1+1,i2+2,
     & i3)*sx(i1+1,i2+2,i3)))-(rsxy(i1-1,i2+2,i3,axis,0)/(rx(i1-1,i2+
     & 2,i3)*sy(i1-1,i2+2,i3)-ry(i1-1,i2+2,i3)*sx(i1-1,i2+2,i3))))-((
     & rsxy(i1+2,i2+2,i3,axis,0)/(rx(i1+2,i2+2,i3)*sy(i1+2,i2+2,i3)-
     & ry(i1+2,i2+2,i3)*sx(i1+2,i2+2,i3)))-(rsxy(i1-2,i2+2,i3,axis,0)
     & /(rx(i1-2,i2+2,i3)*sy(i1-2,i2+2,i3)-ry(i1-2,i2+2,i3)*sx(i1-2,
     & i2+2,i3)))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,i3,axis,0)/(rx(
     & i1+1,i2-2,i3)*sy(i1+1,i2-2,i3)-ry(i1+1,i2-2,i3)*sx(i1+1,i2-2,
     & i3)))-(rsxy(i1-1,i2-2,i3,axis,0)/(rx(i1-1,i2-2,i3)*sy(i1-1,i2-
     & 2,i3)-ry(i1-1,i2-2,i3)*sx(i1-1,i2-2,i3))))-((rsxy(i1+2,i2-2,i3,
     & axis,0)/(rx(i1+2,i2-2,i3)*sy(i1+2,i2-2,i3)-ry(i1+2,i2-2,i3)*sx(
     & i1+2,i2-2,i3)))-(rsxy(i1-2,i2-2,i3,axis,0)/(rx(i1-2,i2-2,i3)*
     & sy(i1-2,i2-2,i3)-ry(i1-2,i2-2,i3)*sx(i1-2,i2-2,i3)))))/(12.*dr(
     & 0))))/(12.*dr(1))
                  a12rs = (8.*((8.*((rsxy(i1+1,i2+1,i3,axis,1)/(rx(i1+
     & 1,i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+1,i3)))
     & -(rsxy(i1-1,i2+1,i3,axis,1)/(rx(i1-1,i2+1,i3)*sy(i1-1,i2+1,i3)-
     & ry(i1-1,i2+1,i3)*sx(i1-1,i2+1,i3))))-((rsxy(i1+2,i2+1,i3,axis,
     & 1)/(rx(i1+2,i2+1,i3)*sy(i1+2,i2+1,i3)-ry(i1+2,i2+1,i3)*sx(i1+2,
     & i2+1,i3)))-(rsxy(i1-2,i2+1,i3,axis,1)/(rx(i1-2,i2+1,i3)*sy(i1-
     & 2,i2+1,i3)-ry(i1-2,i2+1,i3)*sx(i1-2,i2+1,i3)))))/(12.*dr(0))-(
     & 8.*((rsxy(i1+1,i2-1,i3,axis,1)/(rx(i1+1,i2-1,i3)*sy(i1+1,i2-1,
     & i3)-ry(i1+1,i2-1,i3)*sx(i1+1,i2-1,i3)))-(rsxy(i1-1,i2-1,i3,
     & axis,1)/(rx(i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-ry(i1-1,i2-1,i3)*sx(
     & i1-1,i2-1,i3))))-((rsxy(i1+2,i2-1,i3,axis,1)/(rx(i1+2,i2-1,i3)*
     & sy(i1+2,i2-1,i3)-ry(i1+2,i2-1,i3)*sx(i1+2,i2-1,i3)))-(rsxy(i1-
     & 2,i2-1,i3,axis,1)/(rx(i1-2,i2-1,i3)*sy(i1-2,i2-1,i3)-ry(i1-2,
     & i2-1,i3)*sx(i1-2,i2-1,i3)))))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2+
     & 2,i3,axis,1)/(rx(i1+1,i2+2,i3)*sy(i1+1,i2+2,i3)-ry(i1+1,i2+2,
     & i3)*sx(i1+1,i2+2,i3)))-(rsxy(i1-1,i2+2,i3,axis,1)/(rx(i1-1,i2+
     & 2,i3)*sy(i1-1,i2+2,i3)-ry(i1-1,i2+2,i3)*sx(i1-1,i2+2,i3))))-((
     & rsxy(i1+2,i2+2,i3,axis,1)/(rx(i1+2,i2+2,i3)*sy(i1+2,i2+2,i3)-
     & ry(i1+2,i2+2,i3)*sx(i1+2,i2+2,i3)))-(rsxy(i1-2,i2+2,i3,axis,1)
     & /(rx(i1-2,i2+2,i3)*sy(i1-2,i2+2,i3)-ry(i1-2,i2+2,i3)*sx(i1-2,
     & i2+2,i3)))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,i3,axis,1)/(rx(
     & i1+1,i2-2,i3)*sy(i1+1,i2-2,i3)-ry(i1+1,i2-2,i3)*sx(i1+1,i2-2,
     & i3)))-(rsxy(i1-1,i2-2,i3,axis,1)/(rx(i1-1,i2-2,i3)*sy(i1-1,i2-
     & 2,i3)-ry(i1-1,i2-2,i3)*sx(i1-1,i2-2,i3))))-((rsxy(i1+2,i2-2,i3,
     & axis,1)/(rx(i1+2,i2-2,i3)*sy(i1+2,i2-2,i3)-ry(i1+2,i2-2,i3)*sx(
     & i1+2,i2-2,i3)))-(rsxy(i1-2,i2-2,i3,axis,1)/(rx(i1-2,i2-2,i3)*
     & sy(i1-2,i2-2,i3)-ry(i1-2,i2-2,i3)*sx(i1-2,i2-2,i3)))))/(12.*dr(
     & 0))))/(12.*dr(1))
                  a21rs = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp1,0)/(rx(
     & i1+1,i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+1,
     & i3)))-(rsxy(i1-1,i2+1,i3,axisp1,0)/(rx(i1-1,i2+1,i3)*sy(i1-1,
     & i2+1,i3)-ry(i1-1,i2+1,i3)*sx(i1-1,i2+1,i3))))-((rsxy(i1+2,i2+1,
     & i3,axisp1,0)/(rx(i1+2,i2+1,i3)*sy(i1+2,i2+1,i3)-ry(i1+2,i2+1,
     & i3)*sx(i1+2,i2+1,i3)))-(rsxy(i1-2,i2+1,i3,axisp1,0)/(rx(i1-2,
     & i2+1,i3)*sy(i1-2,i2+1,i3)-ry(i1-2,i2+1,i3)*sx(i1-2,i2+1,i3)))))
     & /(12.*dr(0))-(8.*((rsxy(i1+1,i2-1,i3,axisp1,0)/(rx(i1+1,i2-1,
     & i3)*sy(i1+1,i2-1,i3)-ry(i1+1,i2-1,i3)*sx(i1+1,i2-1,i3)))-(rsxy(
     & i1-1,i2-1,i3,axisp1,0)/(rx(i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-ry(
     & i1-1,i2-1,i3)*sx(i1-1,i2-1,i3))))-((rsxy(i1+2,i2-1,i3,axisp1,0)
     & /(rx(i1+2,i2-1,i3)*sy(i1+2,i2-1,i3)-ry(i1+2,i2-1,i3)*sx(i1+2,
     & i2-1,i3)))-(rsxy(i1-2,i2-1,i3,axisp1,0)/(rx(i1-2,i2-1,i3)*sy(
     & i1-2,i2-1,i3)-ry(i1-2,i2-1,i3)*sx(i1-2,i2-1,i3)))))/(12.*dr(0))
     & )-((8.*((rsxy(i1+1,i2+2,i3,axisp1,0)/(rx(i1+1,i2+2,i3)*sy(i1+1,
     & i2+2,i3)-ry(i1+1,i2+2,i3)*sx(i1+1,i2+2,i3)))-(rsxy(i1-1,i2+2,
     & i3,axisp1,0)/(rx(i1-1,i2+2,i3)*sy(i1-1,i2+2,i3)-ry(i1-1,i2+2,
     & i3)*sx(i1-1,i2+2,i3))))-((rsxy(i1+2,i2+2,i3,axisp1,0)/(rx(i1+2,
     & i2+2,i3)*sy(i1+2,i2+2,i3)-ry(i1+2,i2+2,i3)*sx(i1+2,i2+2,i3)))-(
     & rsxy(i1-2,i2+2,i3,axisp1,0)/(rx(i1-2,i2+2,i3)*sy(i1-2,i2+2,i3)-
     & ry(i1-2,i2+2,i3)*sx(i1-2,i2+2,i3)))))/(12.*dr(0))-(8.*((rsxy(
     & i1+1,i2-2,i3,axisp1,0)/(rx(i1+1,i2-2,i3)*sy(i1+1,i2-2,i3)-ry(
     & i1+1,i2-2,i3)*sx(i1+1,i2-2,i3)))-(rsxy(i1-1,i2-2,i3,axisp1,0)/(
     & rx(i1-1,i2-2,i3)*sy(i1-1,i2-2,i3)-ry(i1-1,i2-2,i3)*sx(i1-1,i2-
     & 2,i3))))-((rsxy(i1+2,i2-2,i3,axisp1,0)/(rx(i1+2,i2-2,i3)*sy(i1+
     & 2,i2-2,i3)-ry(i1+2,i2-2,i3)*sx(i1+2,i2-2,i3)))-(rsxy(i1-2,i2-2,
     & i3,axisp1,0)/(rx(i1-2,i2-2,i3)*sy(i1-2,i2-2,i3)-ry(i1-2,i2-2,
     & i3)*sx(i1-2,i2-2,i3)))))/(12.*dr(0))))/(12.*dr(1))
                  a22rs = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp1,1)/(rx(
     & i1+1,i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+1,
     & i3)))-(rsxy(i1-1,i2+1,i3,axisp1,1)/(rx(i1-1,i2+1,i3)*sy(i1-1,
     & i2+1,i3)-ry(i1-1,i2+1,i3)*sx(i1-1,i2+1,i3))))-((rsxy(i1+2,i2+1,
     & i3,axisp1,1)/(rx(i1+2,i2+1,i3)*sy(i1+2,i2+1,i3)-ry(i1+2,i2+1,
     & i3)*sx(i1+2,i2+1,i3)))-(rsxy(i1-2,i2+1,i3,axisp1,1)/(rx(i1-2,
     & i2+1,i3)*sy(i1-2,i2+1,i3)-ry(i1-2,i2+1,i3)*sx(i1-2,i2+1,i3)))))
     & /(12.*dr(0))-(8.*((rsxy(i1+1,i2-1,i3,axisp1,1)/(rx(i1+1,i2-1,
     & i3)*sy(i1+1,i2-1,i3)-ry(i1+1,i2-1,i3)*sx(i1+1,i2-1,i3)))-(rsxy(
     & i1-1,i2-1,i3,axisp1,1)/(rx(i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-ry(
     & i1-1,i2-1,i3)*sx(i1-1,i2-1,i3))))-((rsxy(i1+2,i2-1,i3,axisp1,1)
     & /(rx(i1+2,i2-1,i3)*sy(i1+2,i2-1,i3)-ry(i1+2,i2-1,i3)*sx(i1+2,
     & i2-1,i3)))-(rsxy(i1-2,i2-1,i3,axisp1,1)/(rx(i1-2,i2-1,i3)*sy(
     & i1-2,i2-1,i3)-ry(i1-2,i2-1,i3)*sx(i1-2,i2-1,i3)))))/(12.*dr(0))
     & )-((8.*((rsxy(i1+1,i2+2,i3,axisp1,1)/(rx(i1+1,i2+2,i3)*sy(i1+1,
     & i2+2,i3)-ry(i1+1,i2+2,i3)*sx(i1+1,i2+2,i3)))-(rsxy(i1-1,i2+2,
     & i3,axisp1,1)/(rx(i1-1,i2+2,i3)*sy(i1-1,i2+2,i3)-ry(i1-1,i2+2,
     & i3)*sx(i1-1,i2+2,i3))))-((rsxy(i1+2,i2+2,i3,axisp1,1)/(rx(i1+2,
     & i2+2,i3)*sy(i1+2,i2+2,i3)-ry(i1+2,i2+2,i3)*sx(i1+2,i2+2,i3)))-(
     & rsxy(i1-2,i2+2,i3,axisp1,1)/(rx(i1-2,i2+2,i3)*sy(i1-2,i2+2,i3)-
     & ry(i1-2,i2+2,i3)*sx(i1-2,i2+2,i3)))))/(12.*dr(0))-(8.*((rsxy(
     & i1+1,i2-2,i3,axisp1,1)/(rx(i1+1,i2-2,i3)*sy(i1+1,i2-2,i3)-ry(
     & i1+1,i2-2,i3)*sx(i1+1,i2-2,i3)))-(rsxy(i1-1,i2-2,i3,axisp1,1)/(
     & rx(i1-1,i2-2,i3)*sy(i1-1,i2-2,i3)-ry(i1-1,i2-2,i3)*sx(i1-1,i2-
     & 2,i3))))-((rsxy(i1+2,i2-2,i3,axisp1,1)/(rx(i1+2,i2-2,i3)*sy(i1+
     & 2,i2-2,i3)-ry(i1+2,i2-2,i3)*sx(i1+2,i2-2,i3)))-(rsxy(i1-2,i2-2,
     & i3,axisp1,1)/(rx(i1-2,i2-2,i3)*sy(i1-2,i2-2,i3)-ry(i1-2,i2-2,
     & i3)*sx(i1-2,i2-2,i3)))))/(12.*dr(0))))/(12.*dr(1))
                  a11rr = ((-30.*(rsxy(i1,i2,i3,axis,0)/(rx(i1,i2,i3)*
     & sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+16.*((rsxy(i1+is1,i2+
     & is2,i3+is3,axis,0)/(rx(i1+is1,i2+is2,i3+is3)*sy(i1+is1,i2+is2,
     & i3+is3)-ry(i1+is1,i2+is2,i3+is3)*sx(i1+is1,i2+is2,i3+is3)))+(
     & rsxy(i1-is1,i2-is2,i3-is3,axis,0)/(rx(i1-is1,i2-is2,i3-is3)*sy(
     & i1-is1,i2-is2,i3-is3)-ry(i1-is1,i2-is2,i3-is3)*sx(i1-is1,i2-
     & is2,i3-is3))))-((rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,0)/(rx(
     & i1+2*is1,i2+2*is2,i3+2*is3)*sy(i1+2*is1,i2+2*is2,i3+2*is3)-ry(
     & i1+2*is1,i2+2*is2,i3+2*is3)*sx(i1+2*is1,i2+2*is2,i3+2*is3)))+(
     & rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,0)/(rx(i1-2*is1,i2-2*is2,
     & i3-2*is3)*sy(i1-2*is1,i2-2*is2,i3-2*is3)-ry(i1-2*is1,i2-2*is2,
     & i3-2*is3)*sx(i1-2*is1,i2-2*is2,i3-2*is3)))))/(12.*dra**2))
                  a12rr = ((-30.*(rsxy(i1,i2,i3,axis,1)/(rx(i1,i2,i3)*
     & sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+16.*((rsxy(i1+is1,i2+
     & is2,i3+is3,axis,1)/(rx(i1+is1,i2+is2,i3+is3)*sy(i1+is1,i2+is2,
     & i3+is3)-ry(i1+is1,i2+is2,i3+is3)*sx(i1+is1,i2+is2,i3+is3)))+(
     & rsxy(i1-is1,i2-is2,i3-is3,axis,1)/(rx(i1-is1,i2-is2,i3-is3)*sy(
     & i1-is1,i2-is2,i3-is3)-ry(i1-is1,i2-is2,i3-is3)*sx(i1-is1,i2-
     & is2,i3-is3))))-((rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,1)/(rx(
     & i1+2*is1,i2+2*is2,i3+2*is3)*sy(i1+2*is1,i2+2*is2,i3+2*is3)-ry(
     & i1+2*is1,i2+2*is2,i3+2*is3)*sx(i1+2*is1,i2+2*is2,i3+2*is3)))+(
     & rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,1)/(rx(i1-2*is1,i2-2*is2,
     & i3-2*is3)*sy(i1-2*is1,i2-2*is2,i3-2*is3)-ry(i1-2*is1,i2-2*is2,
     & i3-2*is3)*sx(i1-2*is1,i2-2*is2,i3-2*is3)))))/(12.*dra**2))
                  a21rr = ((-30.*(rsxy(i1,i2,i3,axisp1,0)/(rx(i1,i2,i3)
     & *sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+16.*((rsxy(i1+is1,i2+
     & is2,i3+is3,axisp1,0)/(rx(i1+is1,i2+is2,i3+is3)*sy(i1+is1,i2+
     & is2,i3+is3)-ry(i1+is1,i2+is2,i3+is3)*sx(i1+is1,i2+is2,i3+is3)))
     & +(rsxy(i1-is1,i2-is2,i3-is3,axisp1,0)/(rx(i1-is1,i2-is2,i3-is3)
     & *sy(i1-is1,i2-is2,i3-is3)-ry(i1-is1,i2-is2,i3-is3)*sx(i1-is1,
     & i2-is2,i3-is3))))-((rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp1,0)/(
     & rx(i1+2*is1,i2+2*is2,i3+2*is3)*sy(i1+2*is1,i2+2*is2,i3+2*is3)-
     & ry(i1+2*is1,i2+2*is2,i3+2*is3)*sx(i1+2*is1,i2+2*is2,i3+2*is3)))
     & +(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axisp1,0)/(rx(i1-2*is1,i2-2*
     & is2,i3-2*is3)*sy(i1-2*is1,i2-2*is2,i3-2*is3)-ry(i1-2*is1,i2-2*
     & is2,i3-2*is3)*sx(i1-2*is1,i2-2*is2,i3-2*is3)))))/(12.*dra**2))
                  a22rr = ((-30.*(rsxy(i1,i2,i3,axisp1,1)/(rx(i1,i2,i3)
     & *sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+16.*((rsxy(i1+is1,i2+
     & is2,i3+is3,axisp1,1)/(rx(i1+is1,i2+is2,i3+is3)*sy(i1+is1,i2+
     & is2,i3+is3)-ry(i1+is1,i2+is2,i3+is3)*sx(i1+is1,i2+is2,i3+is3)))
     & +(rsxy(i1-is1,i2-is2,i3-is3,axisp1,1)/(rx(i1-is1,i2-is2,i3-is3)
     & *sy(i1-is1,i2-is2,i3-is3)-ry(i1-is1,i2-is2,i3-is3)*sx(i1-is1,
     & i2-is2,i3-is3))))-((rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp1,1)/(
     & rx(i1+2*is1,i2+2*is2,i3+2*is3)*sy(i1+2*is1,i2+2*is2,i3+2*is3)-
     & ry(i1+2*is1,i2+2*is2,i3+2*is3)*sx(i1+2*is1,i2+2*is2,i3+2*is3)))
     & +(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axisp1,1)/(rx(i1-2*is1,i2-2*
     & is2,i3-2*is3)*sy(i1-2*is1,i2-2*is2,i3-2*is3)-ry(i1-2*is1,i2-2*
     & is2,i3-2*is3)*sx(i1-2*is1,i2-2*is2,i3-2*is3)))))/(12.*dra**2))
                  a11ss = ((-30.*(rsxy(i1,i2,i3,axis,0)/(rx(i1,i2,i3)*
     & sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+16.*((rsxy(i1+js1,i2+
     & js2,i3+js3,axis,0)/(rx(i1+js1,i2+js2,i3+js3)*sy(i1+js1,i2+js2,
     & i3+js3)-ry(i1+js1,i2+js2,i3+js3)*sx(i1+js1,i2+js2,i3+js3)))+(
     & rsxy(i1-js1,i2-js2,i3-js3,axis,0)/(rx(i1-js1,i2-js2,i3-js3)*sy(
     & i1-js1,i2-js2,i3-js3)-ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-
     & js2,i3-js3))))-((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axis,0)/(rx(
     & i1+2*js1,i2+2*js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,i3+2*js3)-ry(
     & i1+2*js1,i2+2*js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,i3+2*js3)))+(
     & rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axis,0)/(rx(i1-2*js1,i2-2*js2,
     & i3-2*js3)*sy(i1-2*js1,i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,
     & i3-2*js3)*sx(i1-2*js1,i2-2*js2,i3-2*js3)))))/(12.*dsa**2))
                  a12ss = ((-30.*(rsxy(i1,i2,i3,axis,1)/(rx(i1,i2,i3)*
     & sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+16.*((rsxy(i1+js1,i2+
     & js2,i3+js3,axis,1)/(rx(i1+js1,i2+js2,i3+js3)*sy(i1+js1,i2+js2,
     & i3+js3)-ry(i1+js1,i2+js2,i3+js3)*sx(i1+js1,i2+js2,i3+js3)))+(
     & rsxy(i1-js1,i2-js2,i3-js3,axis,1)/(rx(i1-js1,i2-js2,i3-js3)*sy(
     & i1-js1,i2-js2,i3-js3)-ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-
     & js2,i3-js3))))-((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axis,1)/(rx(
     & i1+2*js1,i2+2*js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,i3+2*js3)-ry(
     & i1+2*js1,i2+2*js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,i3+2*js3)))+(
     & rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axis,1)/(rx(i1-2*js1,i2-2*js2,
     & i3-2*js3)*sy(i1-2*js1,i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,
     & i3-2*js3)*sx(i1-2*js1,i2-2*js2,i3-2*js3)))))/(12.*dsa**2))
                  a21ss = ((-30.*(rsxy(i1,i2,i3,axisp1,0)/(rx(i1,i2,i3)
     & *sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+16.*((rsxy(i1+js1,i2+
     & js2,i3+js3,axisp1,0)/(rx(i1+js1,i2+js2,i3+js3)*sy(i1+js1,i2+
     & js2,i3+js3)-ry(i1+js1,i2+js2,i3+js3)*sx(i1+js1,i2+js2,i3+js3)))
     & +(rsxy(i1-js1,i2-js2,i3-js3,axisp1,0)/(rx(i1-js1,i2-js2,i3-js3)
     & *sy(i1-js1,i2-js2,i3-js3)-ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,
     & i2-js2,i3-js3))))-((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,0)/(
     & rx(i1+2*js1,i2+2*js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,i3+2*js3)-
     & ry(i1+2*js1,i2+2*js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,i3+2*js3)))
     & +(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axisp1,0)/(rx(i1-2*js1,i2-2*
     & js2,i3-2*js3)*sy(i1-2*js1,i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*
     & js2,i3-2*js3)*sx(i1-2*js1,i2-2*js2,i3-2*js3)))))/(12.*dsa**2))
                  a22ss = ((-30.*(rsxy(i1,i2,i3,axisp1,1)/(rx(i1,i2,i3)
     & *sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+16.*((rsxy(i1+js1,i2+
     & js2,i3+js3,axisp1,1)/(rx(i1+js1,i2+js2,i3+js3)*sy(i1+js1,i2+
     & js2,i3+js3)-ry(i1+js1,i2+js2,i3+js3)*sx(i1+js1,i2+js2,i3+js3)))
     & +(rsxy(i1-js1,i2-js2,i3-js3,axisp1,1)/(rx(i1-js1,i2-js2,i3-js3)
     & *sy(i1-js1,i2-js2,i3-js3)-ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,
     & i2-js2,i3-js3))))-((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,1)/(
     & rx(i1+2*js1,i2+2*js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,i3+2*js3)-
     & ry(i1+2*js1,i2+2*js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,i3+2*js3)))
     & +(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axisp1,1)/(rx(i1-2*js1,i2-2*
     & js2,i3-2*js3)*sy(i1-2*js1,i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*
     & js2,i3-2*js3)*sx(i1-2*js1,i2-2*js2,i3-2*js3)))))/(12.*dsa**2))
                  if( .true. )then
                    a11sss = ((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axis,0)
     & /(rx(i1+2*js1,i2+2*js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,i3+2*js3)
     & -ry(i1+2*js1,i2+2*js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,i3+2*js3))
     & )-2.*(rsxy(i1+js1,i2+js2,i3+js3,axis,0)/(rx(i1+js1,i2+js2,i3+
     & js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+js2,i3+js3)*sx(i1+
     & js1,i2+js2,i3+js3)))+2.*(rsxy(i1-js1,i2-js2,i3-js3,axis,0)/(rx(
     & i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-ry(i1-js1,i2-
     & js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3)))-(rsxy(i1-2*js1,i2-2*js2,
     & i3-2*js3,axis,0)/(rx(i1-2*js1,i2-2*js2,i3-2*js3)*sy(i1-2*js1,
     & i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,i3-2*js3)*sx(i1-2*js1,
     & i2-2*js2,i3-2*js3))))/(2.*dsa**3)
                    a12sss = ((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axis,1)
     & /(rx(i1+2*js1,i2+2*js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,i3+2*js3)
     & -ry(i1+2*js1,i2+2*js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,i3+2*js3))
     & )-2.*(rsxy(i1+js1,i2+js2,i3+js3,axis,1)/(rx(i1+js1,i2+js2,i3+
     & js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+js2,i3+js3)*sx(i1+
     & js1,i2+js2,i3+js3)))+2.*(rsxy(i1-js1,i2-js2,i3-js3,axis,1)/(rx(
     & i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-ry(i1-js1,i2-
     & js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3)))-(rsxy(i1-2*js1,i2-2*js2,
     & i3-2*js3,axis,1)/(rx(i1-2*js1,i2-2*js2,i3-2*js3)*sy(i1-2*js1,
     & i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,i3-2*js3)*sx(i1-2*js1,
     & i2-2*js2,i3-2*js3))))/(2.*dsa**3)
                    a21sss = ((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,
     & 0)/(rx(i1+2*js1,i2+2*js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,i3+2*
     & js3)-ry(i1+2*js1,i2+2*js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,i3+2*
     & js3)))-2.*(rsxy(i1+js1,i2+js2,i3+js3,axisp1,0)/(rx(i1+js1,i2+
     & js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+js2,i3+js3)*
     & sx(i1+js1,i2+js2,i3+js3)))+2.*(rsxy(i1-js1,i2-js2,i3-js3,
     & axisp1,0)/(rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-
     & ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3)))-(rsxy(i1-2*
     & js1,i2-2*js2,i3-2*js3,axisp1,0)/(rx(i1-2*js1,i2-2*js2,i3-2*js3)
     & *sy(i1-2*js1,i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,i3-2*js3)*
     & sx(i1-2*js1,i2-2*js2,i3-2*js3))))/(2.*dsa**3)
                    a22sss = ((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,
     & 1)/(rx(i1+2*js1,i2+2*js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,i3+2*
     & js3)-ry(i1+2*js1,i2+2*js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,i3+2*
     & js3)))-2.*(rsxy(i1+js1,i2+js2,i3+js3,axisp1,1)/(rx(i1+js1,i2+
     & js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+js2,i3+js3)*
     & sx(i1+js1,i2+js2,i3+js3)))+2.*(rsxy(i1-js1,i2-js2,i3-js3,
     & axisp1,1)/(rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-
     & ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3)))-(rsxy(i1-2*
     & js1,i2-2*js2,i3-2*js3,axisp1,1)/(rx(i1-2*js1,i2-2*js2,i3-2*js3)
     & *sy(i1-2*js1,i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,i3-2*js3)*
     & sx(i1-2*js1,i2-2*js2,i3-2*js3))))/(2.*dsa**3)
                  else
                    ! not enough ghost points for the periodic or interp case for: (since we solve at i1=0)
                    a11sss = (8*(rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axis,
     & 0)/(rx(i1+2*js1,i2+2*js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,i3+2*
     & js3)-ry(i1+2*js1,i2+2*js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,i3+2*
     & js3)))-13*(rsxy(i1+js1,i2+js2,i3+js3,axis,0)/(rx(i1+js1,i2+js2,
     & i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+js2,i3+js3)*sx(
     & i1+js1,i2+js2,i3+js3)))+13*(rsxy(i1-js1,i2-js2,i3-js3,axis,0)/(
     & rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-ry(i1-js1,i2-
     & js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3)))-8*(rsxy(i1-2*js1,i2-2*
     & js2,i3-2*js3,axis,0)/(rx(i1-2*js1,i2-2*js2,i3-2*js3)*sy(i1-2*
     & js1,i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,i3-2*js3)*sx(i1-2*
     & js1,i2-2*js2,i3-2*js3)))-(rsxy(i1+3*js1,i2+3*js2,i3+3*js3,axis,
     & 0)/(rx(i1+3*js1,i2+3*js2,i3+3*js3)*sy(i1+3*js1,i2+3*js2,i3+3*
     & js3)-ry(i1+3*js1,i2+3*js2,i3+3*js3)*sx(i1+3*js1,i2+3*js2,i3+3*
     & js3)))+(rsxy(i1-3*js1,i2-3*js2,i3-3*js3,axis,0)/(rx(i1-3*js1,
     & i2-3*js2,i3-3*js3)*sy(i1-3*js1,i2-3*js2,i3-3*js3)-ry(i1-3*js1,
     & i2-3*js2,i3-3*js3)*sx(i1-3*js1,i2-3*js2,i3-3*js3))))/(8.*dsa**
     & 3)
                    a12sss = (8*(rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axis,
     & 1)/(rx(i1+2*js1,i2+2*js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,i3+2*
     & js3)-ry(i1+2*js1,i2+2*js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,i3+2*
     & js3)))-13*(rsxy(i1+js1,i2+js2,i3+js3,axis,1)/(rx(i1+js1,i2+js2,
     & i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+js2,i3+js3)*sx(
     & i1+js1,i2+js2,i3+js3)))+13*(rsxy(i1-js1,i2-js2,i3-js3,axis,1)/(
     & rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-ry(i1-js1,i2-
     & js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3)))-8*(rsxy(i1-2*js1,i2-2*
     & js2,i3-2*js3,axis,1)/(rx(i1-2*js1,i2-2*js2,i3-2*js3)*sy(i1-2*
     & js1,i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,i3-2*js3)*sx(i1-2*
     & js1,i2-2*js2,i3-2*js3)))-(rsxy(i1+3*js1,i2+3*js2,i3+3*js3,axis,
     & 1)/(rx(i1+3*js1,i2+3*js2,i3+3*js3)*sy(i1+3*js1,i2+3*js2,i3+3*
     & js3)-ry(i1+3*js1,i2+3*js2,i3+3*js3)*sx(i1+3*js1,i2+3*js2,i3+3*
     & js3)))+(rsxy(i1-3*js1,i2-3*js2,i3-3*js3,axis,1)/(rx(i1-3*js1,
     & i2-3*js2,i3-3*js3)*sy(i1-3*js1,i2-3*js2,i3-3*js3)-ry(i1-3*js1,
     & i2-3*js2,i3-3*js3)*sx(i1-3*js1,i2-3*js2,i3-3*js3))))/(8.*dsa**
     & 3)
                    a21sss = (8*(rsxy(i1+2*js1,i2+2*js2,i3+2*js3,
     & axisp1,0)/(rx(i1+2*js1,i2+2*js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,
     & i3+2*js3)-ry(i1+2*js1,i2+2*js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,
     & i3+2*js3)))-13*(rsxy(i1+js1,i2+js2,i3+js3,axisp1,0)/(rx(i1+js1,
     & i2+js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+js2,i3+
     & js3)*sx(i1+js1,i2+js2,i3+js3)))+13*(rsxy(i1-js1,i2-js2,i3-js3,
     & axisp1,0)/(rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-
     & ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3)))-8*(rsxy(i1-
     & 2*js1,i2-2*js2,i3-2*js3,axisp1,0)/(rx(i1-2*js1,i2-2*js2,i3-2*
     & js3)*sy(i1-2*js1,i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,i3-2*
     & js3)*sx(i1-2*js1,i2-2*js2,i3-2*js3)))-(rsxy(i1+3*js1,i2+3*js2,
     & i3+3*js3,axisp1,0)/(rx(i1+3*js1,i2+3*js2,i3+3*js3)*sy(i1+3*js1,
     & i2+3*js2,i3+3*js3)-ry(i1+3*js1,i2+3*js2,i3+3*js3)*sx(i1+3*js1,
     & i2+3*js2,i3+3*js3)))+(rsxy(i1-3*js1,i2-3*js2,i3-3*js3,axisp1,0)
     & /(rx(i1-3*js1,i2-3*js2,i3-3*js3)*sy(i1-3*js1,i2-3*js2,i3-3*js3)
     & -ry(i1-3*js1,i2-3*js2,i3-3*js3)*sx(i1-3*js1,i2-3*js2,i3-3*js3))
     & ))/(8.*dsa**3)
                    a22sss = (8*(rsxy(i1+2*js1,i2+2*js2,i3+2*js3,
     & axisp1,1)/(rx(i1+2*js1,i2+2*js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,
     & i3+2*js3)-ry(i1+2*js1,i2+2*js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,
     & i3+2*js3)))-13*(rsxy(i1+js1,i2+js2,i3+js3,axisp1,1)/(rx(i1+js1,
     & i2+js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+js2,i3+
     & js3)*sx(i1+js1,i2+js2,i3+js3)))+13*(rsxy(i1-js1,i2-js2,i3-js3,
     & axisp1,1)/(rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-
     & ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3)))-8*(rsxy(i1-
     & 2*js1,i2-2*js2,i3-2*js3,axisp1,1)/(rx(i1-2*js1,i2-2*js2,i3-2*
     & js3)*sy(i1-2*js1,i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,i3-2*
     & js3)*sx(i1-2*js1,i2-2*js2,i3-2*js3)))-(rsxy(i1+3*js1,i2+3*js2,
     & i3+3*js3,axisp1,1)/(rx(i1+3*js1,i2+3*js2,i3+3*js3)*sy(i1+3*js1,
     & i2+3*js2,i3+3*js3)-ry(i1+3*js1,i2+3*js2,i3+3*js3)*sx(i1+3*js1,
     & i2+3*js2,i3+3*js3)))+(rsxy(i1-3*js1,i2-3*js2,i3-3*js3,axisp1,1)
     & /(rx(i1-3*js1,i2-3*js2,i3-3*js3)*sy(i1-3*js1,i2-3*js2,i3-3*js3)
     & -ry(i1-3*js1,i2-3*js2,i3-3*js3)*sx(i1-3*js1,i2-3*js2,i3-3*js3))
     & ))/(8.*dsa**3)
                  end if
                  if( axis.eq.0 )then
                    a11rss = (128*(rsxy(i1+1,i2+1,i3,axis,0)/(rx(i1+1,
     & i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+1,i3)))-
     & 128*(rsxy(i1-1,i2+1,i3,axis,0)/(rx(i1-1,i2+1,i3)*sy(i1-1,i2+1,
     & i3)-ry(i1-1,i2+1,i3)*sx(i1-1,i2+1,i3)))-16*(rsxy(i1+2,i2+1,i3,
     & axis,0)/(rx(i1+2,i2+1,i3)*sy(i1+2,i2+1,i3)-ry(i1+2,i2+1,i3)*sx(
     & i1+2,i2+1,i3)))+16*(rsxy(i1-2,i2+1,i3,axis,0)/(rx(i1-2,i2+1,i3)
     & *sy(i1-2,i2+1,i3)-ry(i1-2,i2+1,i3)*sx(i1-2,i2+1,i3)))+128*(
     & rsxy(i1+1,i2-1,i3,axis,0)/(rx(i1+1,i2-1,i3)*sy(i1+1,i2-1,i3)-
     & ry(i1+1,i2-1,i3)*sx(i1+1,i2-1,i3)))-128*(rsxy(i1-1,i2-1,i3,
     & axis,0)/(rx(i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-ry(i1-1,i2-1,i3)*sx(
     & i1-1,i2-1,i3)))-16*(rsxy(i1+2,i2-1,i3,axis,0)/(rx(i1+2,i2-1,i3)
     & *sy(i1+2,i2-1,i3)-ry(i1+2,i2-1,i3)*sx(i1+2,i2-1,i3)))+16*(rsxy(
     & i1-2,i2-1,i3,axis,0)/(rx(i1-2,i2-1,i3)*sy(i1-2,i2-1,i3)-ry(i1-
     & 2,i2-1,i3)*sx(i1-2,i2-1,i3)))+30*(rsxy(i1+2,i2,i3,axis,0)/(rx(
     & i1+2,i2,i3)*sy(i1+2,i2,i3)-ry(i1+2,i2,i3)*sx(i1+2,i2,i3)))+8*(
     & rsxy(i1-1,i2+2,i3,axis,0)/(rx(i1-1,i2+2,i3)*sy(i1-1,i2+2,i3)-
     & ry(i1-1,i2+2,i3)*sx(i1-1,i2+2,i3)))+(rsxy(i1+2,i2+2,i3,axis,0)
     & /(rx(i1+2,i2+2,i3)*sy(i1+2,i2+2,i3)-ry(i1+2,i2+2,i3)*sx(i1+2,
     & i2+2,i3)))-(rsxy(i1-2,i2+2,i3,axis,0)/(rx(i1-2,i2+2,i3)*sy(i1-
     & 2,i2+2,i3)-ry(i1-2,i2+2,i3)*sx(i1-2,i2+2,i3)))-8*(rsxy(i1+1,i2-
     & 2,i3,axis,0)/(rx(i1+1,i2-2,i3)*sy(i1+1,i2-2,i3)-ry(i1+1,i2-2,
     & i3)*sx(i1+1,i2-2,i3)))+8*(rsxy(i1-1,i2-2,i3,axis,0)/(rx(i1-1,
     & i2-2,i3)*sy(i1-1,i2-2,i3)-ry(i1-1,i2-2,i3)*sx(i1-1,i2-2,i3)))+(
     & rsxy(i1+2,i2-2,i3,axis,0)/(rx(i1+2,i2-2,i3)*sy(i1+2,i2-2,i3)-
     & ry(i1+2,i2-2,i3)*sx(i1+2,i2-2,i3)))-(rsxy(i1-2,i2-2,i3,axis,0)
     & /(rx(i1-2,i2-2,i3)*sy(i1-2,i2-2,i3)-ry(i1-2,i2-2,i3)*sx(i1-2,
     & i2-2,i3)))-8*(rsxy(i1+1,i2+2,i3,axis,0)/(rx(i1+1,i2+2,i3)*sy(
     & i1+1,i2+2,i3)-ry(i1+1,i2+2,i3)*sx(i1+1,i2+2,i3)))-30*(rsxy(i1-
     & 2,i2,i3,axis,0)/(rx(i1-2,i2,i3)*sy(i1-2,i2,i3)-ry(i1-2,i2,i3)*
     & sx(i1-2,i2,i3)))-240*(rsxy(i1+1,i2,i3,axis,0)/(rx(i1+1,i2,i3)*
     & sy(i1+1,i2,i3)-ry(i1+1,i2,i3)*sx(i1+1,i2,i3)))+240*(rsxy(i1-1,
     & i2,i3,axis,0)/(rx(i1-1,i2,i3)*sy(i1-1,i2,i3)-ry(i1-1,i2,i3)*sx(
     & i1-1,i2,i3))))/(144.*dr(1)**2*dr(0))
                    a12rss = (128*(rsxy(i1+1,i2+1,i3,axis,1)/(rx(i1+1,
     & i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+1,i3)))-
     & 128*(rsxy(i1-1,i2+1,i3,axis,1)/(rx(i1-1,i2+1,i3)*sy(i1-1,i2+1,
     & i3)-ry(i1-1,i2+1,i3)*sx(i1-1,i2+1,i3)))-16*(rsxy(i1+2,i2+1,i3,
     & axis,1)/(rx(i1+2,i2+1,i3)*sy(i1+2,i2+1,i3)-ry(i1+2,i2+1,i3)*sx(
     & i1+2,i2+1,i3)))+16*(rsxy(i1-2,i2+1,i3,axis,1)/(rx(i1-2,i2+1,i3)
     & *sy(i1-2,i2+1,i3)-ry(i1-2,i2+1,i3)*sx(i1-2,i2+1,i3)))+128*(
     & rsxy(i1+1,i2-1,i3,axis,1)/(rx(i1+1,i2-1,i3)*sy(i1+1,i2-1,i3)-
     & ry(i1+1,i2-1,i3)*sx(i1+1,i2-1,i3)))-128*(rsxy(i1-1,i2-1,i3,
     & axis,1)/(rx(i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-ry(i1-1,i2-1,i3)*sx(
     & i1-1,i2-1,i3)))-16*(rsxy(i1+2,i2-1,i3,axis,1)/(rx(i1+2,i2-1,i3)
     & *sy(i1+2,i2-1,i3)-ry(i1+2,i2-1,i3)*sx(i1+2,i2-1,i3)))+16*(rsxy(
     & i1-2,i2-1,i3,axis,1)/(rx(i1-2,i2-1,i3)*sy(i1-2,i2-1,i3)-ry(i1-
     & 2,i2-1,i3)*sx(i1-2,i2-1,i3)))+30*(rsxy(i1+2,i2,i3,axis,1)/(rx(
     & i1+2,i2,i3)*sy(i1+2,i2,i3)-ry(i1+2,i2,i3)*sx(i1+2,i2,i3)))+8*(
     & rsxy(i1-1,i2+2,i3,axis,1)/(rx(i1-1,i2+2,i3)*sy(i1-1,i2+2,i3)-
     & ry(i1-1,i2+2,i3)*sx(i1-1,i2+2,i3)))+(rsxy(i1+2,i2+2,i3,axis,1)
     & /(rx(i1+2,i2+2,i3)*sy(i1+2,i2+2,i3)-ry(i1+2,i2+2,i3)*sx(i1+2,
     & i2+2,i3)))-(rsxy(i1-2,i2+2,i3,axis,1)/(rx(i1-2,i2+2,i3)*sy(i1-
     & 2,i2+2,i3)-ry(i1-2,i2+2,i3)*sx(i1-2,i2+2,i3)))-8*(rsxy(i1+1,i2-
     & 2,i3,axis,1)/(rx(i1+1,i2-2,i3)*sy(i1+1,i2-2,i3)-ry(i1+1,i2-2,
     & i3)*sx(i1+1,i2-2,i3)))+8*(rsxy(i1-1,i2-2,i3,axis,1)/(rx(i1-1,
     & i2-2,i3)*sy(i1-1,i2-2,i3)-ry(i1-1,i2-2,i3)*sx(i1-1,i2-2,i3)))+(
     & rsxy(i1+2,i2-2,i3,axis,1)/(rx(i1+2,i2-2,i3)*sy(i1+2,i2-2,i3)-
     & ry(i1+2,i2-2,i3)*sx(i1+2,i2-2,i3)))-(rsxy(i1-2,i2-2,i3,axis,1)
     & /(rx(i1-2,i2-2,i3)*sy(i1-2,i2-2,i3)-ry(i1-2,i2-2,i3)*sx(i1-2,
     & i2-2,i3)))-8*(rsxy(i1+1,i2+2,i3,axis,1)/(rx(i1+1,i2+2,i3)*sy(
     & i1+1,i2+2,i3)-ry(i1+1,i2+2,i3)*sx(i1+1,i2+2,i3)))-30*(rsxy(i1-
     & 2,i2,i3,axis,1)/(rx(i1-2,i2,i3)*sy(i1-2,i2,i3)-ry(i1-2,i2,i3)*
     & sx(i1-2,i2,i3)))-240*(rsxy(i1+1,i2,i3,axis,1)/(rx(i1+1,i2,i3)*
     & sy(i1+1,i2,i3)-ry(i1+1,i2,i3)*sx(i1+1,i2,i3)))+240*(rsxy(i1-1,
     & i2,i3,axis,1)/(rx(i1-1,i2,i3)*sy(i1-1,i2,i3)-ry(i1-1,i2,i3)*sx(
     & i1-1,i2,i3))))/(144.*dr(1)**2*dr(0))
                    a21rss = (128*(rsxy(i1+1,i2+1,i3,axisp1,0)/(rx(i1+
     & 1,i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+1,i3)))
     & -128*(rsxy(i1-1,i2+1,i3,axisp1,0)/(rx(i1-1,i2+1,i3)*sy(i1-1,i2+
     & 1,i3)-ry(i1-1,i2+1,i3)*sx(i1-1,i2+1,i3)))-16*(rsxy(i1+2,i2+1,
     & i3,axisp1,0)/(rx(i1+2,i2+1,i3)*sy(i1+2,i2+1,i3)-ry(i1+2,i2+1,
     & i3)*sx(i1+2,i2+1,i3)))+16*(rsxy(i1-2,i2+1,i3,axisp1,0)/(rx(i1-
     & 2,i2+1,i3)*sy(i1-2,i2+1,i3)-ry(i1-2,i2+1,i3)*sx(i1-2,i2+1,i3)))
     & +128*(rsxy(i1+1,i2-1,i3,axisp1,0)/(rx(i1+1,i2-1,i3)*sy(i1+1,i2-
     & 1,i3)-ry(i1+1,i2-1,i3)*sx(i1+1,i2-1,i3)))-128*(rsxy(i1-1,i2-1,
     & i3,axisp1,0)/(rx(i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-ry(i1-1,i2-1,
     & i3)*sx(i1-1,i2-1,i3)))-16*(rsxy(i1+2,i2-1,i3,axisp1,0)/(rx(i1+
     & 2,i2-1,i3)*sy(i1+2,i2-1,i3)-ry(i1+2,i2-1,i3)*sx(i1+2,i2-1,i3)))
     & +16*(rsxy(i1-2,i2-1,i3,axisp1,0)/(rx(i1-2,i2-1,i3)*sy(i1-2,i2-
     & 1,i3)-ry(i1-2,i2-1,i3)*sx(i1-2,i2-1,i3)))+30*(rsxy(i1+2,i2,i3,
     & axisp1,0)/(rx(i1+2,i2,i3)*sy(i1+2,i2,i3)-ry(i1+2,i2,i3)*sx(i1+
     & 2,i2,i3)))+8*(rsxy(i1-1,i2+2,i3,axisp1,0)/(rx(i1-1,i2+2,i3)*sy(
     & i1-1,i2+2,i3)-ry(i1-1,i2+2,i3)*sx(i1-1,i2+2,i3)))+(rsxy(i1+2,
     & i2+2,i3,axisp1,0)/(rx(i1+2,i2+2,i3)*sy(i1+2,i2+2,i3)-ry(i1+2,
     & i2+2,i3)*sx(i1+2,i2+2,i3)))-(rsxy(i1-2,i2+2,i3,axisp1,0)/(rx(
     & i1-2,i2+2,i3)*sy(i1-2,i2+2,i3)-ry(i1-2,i2+2,i3)*sx(i1-2,i2+2,
     & i3)))-8*(rsxy(i1+1,i2-2,i3,axisp1,0)/(rx(i1+1,i2-2,i3)*sy(i1+1,
     & i2-2,i3)-ry(i1+1,i2-2,i3)*sx(i1+1,i2-2,i3)))+8*(rsxy(i1-1,i2-2,
     & i3,axisp1,0)/(rx(i1-1,i2-2,i3)*sy(i1-1,i2-2,i3)-ry(i1-1,i2-2,
     & i3)*sx(i1-1,i2-2,i3)))+(rsxy(i1+2,i2-2,i3,axisp1,0)/(rx(i1+2,
     & i2-2,i3)*sy(i1+2,i2-2,i3)-ry(i1+2,i2-2,i3)*sx(i1+2,i2-2,i3)))-(
     & rsxy(i1-2,i2-2,i3,axisp1,0)/(rx(i1-2,i2-2,i3)*sy(i1-2,i2-2,i3)-
     & ry(i1-2,i2-2,i3)*sx(i1-2,i2-2,i3)))-8*(rsxy(i1+1,i2+2,i3,
     & axisp1,0)/(rx(i1+1,i2+2,i3)*sy(i1+1,i2+2,i3)-ry(i1+1,i2+2,i3)*
     & sx(i1+1,i2+2,i3)))-30*(rsxy(i1-2,i2,i3,axisp1,0)/(rx(i1-2,i2,
     & i3)*sy(i1-2,i2,i3)-ry(i1-2,i2,i3)*sx(i1-2,i2,i3)))-240*(rsxy(
     & i1+1,i2,i3,axisp1,0)/(rx(i1+1,i2,i3)*sy(i1+1,i2,i3)-ry(i1+1,i2,
     & i3)*sx(i1+1,i2,i3)))+240*(rsxy(i1-1,i2,i3,axisp1,0)/(rx(i1-1,
     & i2,i3)*sy(i1-1,i2,i3)-ry(i1-1,i2,i3)*sx(i1-1,i2,i3))))/(144.*
     & dr(1)**2*dr(0))
                    a22rss = (128*(rsxy(i1+1,i2+1,i3,axisp1,1)/(rx(i1+
     & 1,i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+1,i3)))
     & -128*(rsxy(i1-1,i2+1,i3,axisp1,1)/(rx(i1-1,i2+1,i3)*sy(i1-1,i2+
     & 1,i3)-ry(i1-1,i2+1,i3)*sx(i1-1,i2+1,i3)))-16*(rsxy(i1+2,i2+1,
     & i3,axisp1,1)/(rx(i1+2,i2+1,i3)*sy(i1+2,i2+1,i3)-ry(i1+2,i2+1,
     & i3)*sx(i1+2,i2+1,i3)))+16*(rsxy(i1-2,i2+1,i3,axisp1,1)/(rx(i1-
     & 2,i2+1,i3)*sy(i1-2,i2+1,i3)-ry(i1-2,i2+1,i3)*sx(i1-2,i2+1,i3)))
     & +128*(rsxy(i1+1,i2-1,i3,axisp1,1)/(rx(i1+1,i2-1,i3)*sy(i1+1,i2-
     & 1,i3)-ry(i1+1,i2-1,i3)*sx(i1+1,i2-1,i3)))-128*(rsxy(i1-1,i2-1,
     & i3,axisp1,1)/(rx(i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-ry(i1-1,i2-1,
     & i3)*sx(i1-1,i2-1,i3)))-16*(rsxy(i1+2,i2-1,i3,axisp1,1)/(rx(i1+
     & 2,i2-1,i3)*sy(i1+2,i2-1,i3)-ry(i1+2,i2-1,i3)*sx(i1+2,i2-1,i3)))
     & +16*(rsxy(i1-2,i2-1,i3,axisp1,1)/(rx(i1-2,i2-1,i3)*sy(i1-2,i2-
     & 1,i3)-ry(i1-2,i2-1,i3)*sx(i1-2,i2-1,i3)))+30*(rsxy(i1+2,i2,i3,
     & axisp1,1)/(rx(i1+2,i2,i3)*sy(i1+2,i2,i3)-ry(i1+2,i2,i3)*sx(i1+
     & 2,i2,i3)))+8*(rsxy(i1-1,i2+2,i3,axisp1,1)/(rx(i1-1,i2+2,i3)*sy(
     & i1-1,i2+2,i3)-ry(i1-1,i2+2,i3)*sx(i1-1,i2+2,i3)))+(rsxy(i1+2,
     & i2+2,i3,axisp1,1)/(rx(i1+2,i2+2,i3)*sy(i1+2,i2+2,i3)-ry(i1+2,
     & i2+2,i3)*sx(i1+2,i2+2,i3)))-(rsxy(i1-2,i2+2,i3,axisp1,1)/(rx(
     & i1-2,i2+2,i3)*sy(i1-2,i2+2,i3)-ry(i1-2,i2+2,i3)*sx(i1-2,i2+2,
     & i3)))-8*(rsxy(i1+1,i2-2,i3,axisp1,1)/(rx(i1+1,i2-2,i3)*sy(i1+1,
     & i2-2,i3)-ry(i1+1,i2-2,i3)*sx(i1+1,i2-2,i3)))+8*(rsxy(i1-1,i2-2,
     & i3,axisp1,1)/(rx(i1-1,i2-2,i3)*sy(i1-1,i2-2,i3)-ry(i1-1,i2-2,
     & i3)*sx(i1-1,i2-2,i3)))+(rsxy(i1+2,i2-2,i3,axisp1,1)/(rx(i1+2,
     & i2-2,i3)*sy(i1+2,i2-2,i3)-ry(i1+2,i2-2,i3)*sx(i1+2,i2-2,i3)))-(
     & rsxy(i1-2,i2-2,i3,axisp1,1)/(rx(i1-2,i2-2,i3)*sy(i1-2,i2-2,i3)-
     & ry(i1-2,i2-2,i3)*sx(i1-2,i2-2,i3)))-8*(rsxy(i1+1,i2+2,i3,
     & axisp1,1)/(rx(i1+1,i2+2,i3)*sy(i1+1,i2+2,i3)-ry(i1+1,i2+2,i3)*
     & sx(i1+1,i2+2,i3)))-30*(rsxy(i1-2,i2,i3,axisp1,1)/(rx(i1-2,i2,
     & i3)*sy(i1-2,i2,i3)-ry(i1-2,i2,i3)*sx(i1-2,i2,i3)))-240*(rsxy(
     & i1+1,i2,i3,axisp1,1)/(rx(i1+1,i2,i3)*sy(i1+1,i2,i3)-ry(i1+1,i2,
     & i3)*sx(i1+1,i2,i3)))+240*(rsxy(i1-1,i2,i3,axisp1,1)/(rx(i1-1,
     & i2,i3)*sy(i1-1,i2,i3)-ry(i1-1,i2,i3)*sx(i1-1,i2,i3))))/(144.*
     & dr(1)**2*dr(0))
                  else
                    a11rss = (128*(rsxy(i1+1,i2+1,i3,axis,0)/(rx(i1+1,
     & i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+1,i3)))-
     & 240*(rsxy(i1,i2+1,i3,axis,0)/(rx(i1,i2+1,i3)*sy(i1,i2+1,i3)-ry(
     & i1,i2+1,i3)*sx(i1,i2+1,i3)))+128*(rsxy(i1-1,i2+1,i3,axis,0)/(
     & rx(i1-1,i2+1,i3)*sy(i1-1,i2+1,i3)-ry(i1-1,i2+1,i3)*sx(i1-1,i2+
     & 1,i3)))-8*(rsxy(i1+2,i2+1,i3,axis,0)/(rx(i1+2,i2+1,i3)*sy(i1+2,
     & i2+1,i3)-ry(i1+2,i2+1,i3)*sx(i1+2,i2+1,i3)))-8*(rsxy(i1-2,i2+1,
     & i3,axis,0)/(rx(i1-2,i2+1,i3)*sy(i1-2,i2+1,i3)-ry(i1-2,i2+1,i3)*
     & sx(i1-2,i2+1,i3)))-128*(rsxy(i1+1,i2-1,i3,axis,0)/(rx(i1+1,i2-
     & 1,i3)*sy(i1+1,i2-1,i3)-ry(i1+1,i2-1,i3)*sx(i1+1,i2-1,i3)))+240*
     & (rsxy(i1,i2-1,i3,axis,0)/(rx(i1,i2-1,i3)*sy(i1,i2-1,i3)-ry(i1,
     & i2-1,i3)*sx(i1,i2-1,i3)))-128*(rsxy(i1-1,i2-1,i3,axis,0)/(rx(
     & i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-ry(i1-1,i2-1,i3)*sx(i1-1,i2-1,
     & i3)))+8*(rsxy(i1+2,i2-1,i3,axis,0)/(rx(i1+2,i2-1,i3)*sy(i1+2,
     & i2-1,i3)-ry(i1+2,i2-1,i3)*sx(i1+2,i2-1,i3)))+8*(rsxy(i1-2,i2-1,
     & i3,axis,0)/(rx(i1-2,i2-1,i3)*sy(i1-2,i2-1,i3)-ry(i1-2,i2-1,i3)*
     & sx(i1-2,i2-1,i3)))+30*(rsxy(i1,i2+2,i3,axis,0)/(rx(i1,i2+2,i3)*
     & sy(i1,i2+2,i3)-ry(i1,i2+2,i3)*sx(i1,i2+2,i3)))-16*(rsxy(i1-1,
     & i2+2,i3,axis,0)/(rx(i1-1,i2+2,i3)*sy(i1-1,i2+2,i3)-ry(i1-1,i2+
     & 2,i3)*sx(i1-1,i2+2,i3)))+(rsxy(i1+2,i2+2,i3,axis,0)/(rx(i1+2,
     & i2+2,i3)*sy(i1+2,i2+2,i3)-ry(i1+2,i2+2,i3)*sx(i1+2,i2+2,i3)))+(
     & rsxy(i1-2,i2+2,i3,axis,0)/(rx(i1-2,i2+2,i3)*sy(i1-2,i2+2,i3)-
     & ry(i1-2,i2+2,i3)*sx(i1-2,i2+2,i3)))+16*(rsxy(i1+1,i2-2,i3,axis,
     & 0)/(rx(i1+1,i2-2,i3)*sy(i1+1,i2-2,i3)-ry(i1+1,i2-2,i3)*sx(i1+1,
     & i2-2,i3)))-30*(rsxy(i1,i2-2,i3,axis,0)/(rx(i1,i2-2,i3)*sy(i1,
     & i2-2,i3)-ry(i1,i2-2,i3)*sx(i1,i2-2,i3)))+16*(rsxy(i1-1,i2-2,i3,
     & axis,0)/(rx(i1-1,i2-2,i3)*sy(i1-1,i2-2,i3)-ry(i1-1,i2-2,i3)*sx(
     & i1-1,i2-2,i3)))-(rsxy(i1+2,i2-2,i3,axis,0)/(rx(i1+2,i2-2,i3)*
     & sy(i1+2,i2-2,i3)-ry(i1+2,i2-2,i3)*sx(i1+2,i2-2,i3)))-(rsxy(i1-
     & 2,i2-2,i3,axis,0)/(rx(i1-2,i2-2,i3)*sy(i1-2,i2-2,i3)-ry(i1-2,
     & i2-2,i3)*sx(i1-2,i2-2,i3)))-16*(rsxy(i1+1,i2+2,i3,axis,0)/(rx(
     & i1+1,i2+2,i3)*sy(i1+1,i2+2,i3)-ry(i1+1,i2+2,i3)*sx(i1+1,i2+2,
     & i3))))/(144.*dr(0)**2*dr(1))
                    a12rss = (128*(rsxy(i1+1,i2+1,i3,axis,1)/(rx(i1+1,
     & i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+1,i3)))-
     & 240*(rsxy(i1,i2+1,i3,axis,1)/(rx(i1,i2+1,i3)*sy(i1,i2+1,i3)-ry(
     & i1,i2+1,i3)*sx(i1,i2+1,i3)))+128*(rsxy(i1-1,i2+1,i3,axis,1)/(
     & rx(i1-1,i2+1,i3)*sy(i1-1,i2+1,i3)-ry(i1-1,i2+1,i3)*sx(i1-1,i2+
     & 1,i3)))-8*(rsxy(i1+2,i2+1,i3,axis,1)/(rx(i1+2,i2+1,i3)*sy(i1+2,
     & i2+1,i3)-ry(i1+2,i2+1,i3)*sx(i1+2,i2+1,i3)))-8*(rsxy(i1-2,i2+1,
     & i3,axis,1)/(rx(i1-2,i2+1,i3)*sy(i1-2,i2+1,i3)-ry(i1-2,i2+1,i3)*
     & sx(i1-2,i2+1,i3)))-128*(rsxy(i1+1,i2-1,i3,axis,1)/(rx(i1+1,i2-
     & 1,i3)*sy(i1+1,i2-1,i3)-ry(i1+1,i2-1,i3)*sx(i1+1,i2-1,i3)))+240*
     & (rsxy(i1,i2-1,i3,axis,1)/(rx(i1,i2-1,i3)*sy(i1,i2-1,i3)-ry(i1,
     & i2-1,i3)*sx(i1,i2-1,i3)))-128*(rsxy(i1-1,i2-1,i3,axis,1)/(rx(
     & i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-ry(i1-1,i2-1,i3)*sx(i1-1,i2-1,
     & i3)))+8*(rsxy(i1+2,i2-1,i3,axis,1)/(rx(i1+2,i2-1,i3)*sy(i1+2,
     & i2-1,i3)-ry(i1+2,i2-1,i3)*sx(i1+2,i2-1,i3)))+8*(rsxy(i1-2,i2-1,
     & i3,axis,1)/(rx(i1-2,i2-1,i3)*sy(i1-2,i2-1,i3)-ry(i1-2,i2-1,i3)*
     & sx(i1-2,i2-1,i3)))+30*(rsxy(i1,i2+2,i3,axis,1)/(rx(i1,i2+2,i3)*
     & sy(i1,i2+2,i3)-ry(i1,i2+2,i3)*sx(i1,i2+2,i3)))-16*(rsxy(i1-1,
     & i2+2,i3,axis,1)/(rx(i1-1,i2+2,i3)*sy(i1-1,i2+2,i3)-ry(i1-1,i2+
     & 2,i3)*sx(i1-1,i2+2,i3)))+(rsxy(i1+2,i2+2,i3,axis,1)/(rx(i1+2,
     & i2+2,i3)*sy(i1+2,i2+2,i3)-ry(i1+2,i2+2,i3)*sx(i1+2,i2+2,i3)))+(
     & rsxy(i1-2,i2+2,i3,axis,1)/(rx(i1-2,i2+2,i3)*sy(i1-2,i2+2,i3)-
     & ry(i1-2,i2+2,i3)*sx(i1-2,i2+2,i3)))+16*(rsxy(i1+1,i2-2,i3,axis,
     & 1)/(rx(i1+1,i2-2,i3)*sy(i1+1,i2-2,i3)-ry(i1+1,i2-2,i3)*sx(i1+1,
     & i2-2,i3)))-30*(rsxy(i1,i2-2,i3,axis,1)/(rx(i1,i2-2,i3)*sy(i1,
     & i2-2,i3)-ry(i1,i2-2,i3)*sx(i1,i2-2,i3)))+16*(rsxy(i1-1,i2-2,i3,
     & axis,1)/(rx(i1-1,i2-2,i3)*sy(i1-1,i2-2,i3)-ry(i1-1,i2-2,i3)*sx(
     & i1-1,i2-2,i3)))-(rsxy(i1+2,i2-2,i3,axis,1)/(rx(i1+2,i2-2,i3)*
     & sy(i1+2,i2-2,i3)-ry(i1+2,i2-2,i3)*sx(i1+2,i2-2,i3)))-(rsxy(i1-
     & 2,i2-2,i3,axis,1)/(rx(i1-2,i2-2,i3)*sy(i1-2,i2-2,i3)-ry(i1-2,
     & i2-2,i3)*sx(i1-2,i2-2,i3)))-16*(rsxy(i1+1,i2+2,i3,axis,1)/(rx(
     & i1+1,i2+2,i3)*sy(i1+1,i2+2,i3)-ry(i1+1,i2+2,i3)*sx(i1+1,i2+2,
     & i3))))/(144.*dr(0)**2*dr(1))
                    a21rss = (128*(rsxy(i1+1,i2+1,i3,axisp1,0)/(rx(i1+
     & 1,i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+1,i3)))
     & -240*(rsxy(i1,i2+1,i3,axisp1,0)/(rx(i1,i2+1,i3)*sy(i1,i2+1,i3)-
     & ry(i1,i2+1,i3)*sx(i1,i2+1,i3)))+128*(rsxy(i1-1,i2+1,i3,axisp1,
     & 0)/(rx(i1-1,i2+1,i3)*sy(i1-1,i2+1,i3)-ry(i1-1,i2+1,i3)*sx(i1-1,
     & i2+1,i3)))-8*(rsxy(i1+2,i2+1,i3,axisp1,0)/(rx(i1+2,i2+1,i3)*sy(
     & i1+2,i2+1,i3)-ry(i1+2,i2+1,i3)*sx(i1+2,i2+1,i3)))-8*(rsxy(i1-2,
     & i2+1,i3,axisp1,0)/(rx(i1-2,i2+1,i3)*sy(i1-2,i2+1,i3)-ry(i1-2,
     & i2+1,i3)*sx(i1-2,i2+1,i3)))-128*(rsxy(i1+1,i2-1,i3,axisp1,0)/(
     & rx(i1+1,i2-1,i3)*sy(i1+1,i2-1,i3)-ry(i1+1,i2-1,i3)*sx(i1+1,i2-
     & 1,i3)))+240*(rsxy(i1,i2-1,i3,axisp1,0)/(rx(i1,i2-1,i3)*sy(i1,
     & i2-1,i3)-ry(i1,i2-1,i3)*sx(i1,i2-1,i3)))-128*(rsxy(i1-1,i2-1,
     & i3,axisp1,0)/(rx(i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-ry(i1-1,i2-1,
     & i3)*sx(i1-1,i2-1,i3)))+8*(rsxy(i1+2,i2-1,i3,axisp1,0)/(rx(i1+2,
     & i2-1,i3)*sy(i1+2,i2-1,i3)-ry(i1+2,i2-1,i3)*sx(i1+2,i2-1,i3)))+
     & 8*(rsxy(i1-2,i2-1,i3,axisp1,0)/(rx(i1-2,i2-1,i3)*sy(i1-2,i2-1,
     & i3)-ry(i1-2,i2-1,i3)*sx(i1-2,i2-1,i3)))+30*(rsxy(i1,i2+2,i3,
     & axisp1,0)/(rx(i1,i2+2,i3)*sy(i1,i2+2,i3)-ry(i1,i2+2,i3)*sx(i1,
     & i2+2,i3)))-16*(rsxy(i1-1,i2+2,i3,axisp1,0)/(rx(i1-1,i2+2,i3)*
     & sy(i1-1,i2+2,i3)-ry(i1-1,i2+2,i3)*sx(i1-1,i2+2,i3)))+(rsxy(i1+
     & 2,i2+2,i3,axisp1,0)/(rx(i1+2,i2+2,i3)*sy(i1+2,i2+2,i3)-ry(i1+2,
     & i2+2,i3)*sx(i1+2,i2+2,i3)))+(rsxy(i1-2,i2+2,i3,axisp1,0)/(rx(
     & i1-2,i2+2,i3)*sy(i1-2,i2+2,i3)-ry(i1-2,i2+2,i3)*sx(i1-2,i2+2,
     & i3)))+16*(rsxy(i1+1,i2-2,i3,axisp1,0)/(rx(i1+1,i2-2,i3)*sy(i1+
     & 1,i2-2,i3)-ry(i1+1,i2-2,i3)*sx(i1+1,i2-2,i3)))-30*(rsxy(i1,i2-
     & 2,i3,axisp1,0)/(rx(i1,i2-2,i3)*sy(i1,i2-2,i3)-ry(i1,i2-2,i3)*
     & sx(i1,i2-2,i3)))+16*(rsxy(i1-1,i2-2,i3,axisp1,0)/(rx(i1-1,i2-2,
     & i3)*sy(i1-1,i2-2,i3)-ry(i1-1,i2-2,i3)*sx(i1-1,i2-2,i3)))-(rsxy(
     & i1+2,i2-2,i3,axisp1,0)/(rx(i1+2,i2-2,i3)*sy(i1+2,i2-2,i3)-ry(
     & i1+2,i2-2,i3)*sx(i1+2,i2-2,i3)))-(rsxy(i1-2,i2-2,i3,axisp1,0)/(
     & rx(i1-2,i2-2,i3)*sy(i1-2,i2-2,i3)-ry(i1-2,i2-2,i3)*sx(i1-2,i2-
     & 2,i3)))-16*(rsxy(i1+1,i2+2,i3,axisp1,0)/(rx(i1+1,i2+2,i3)*sy(
     & i1+1,i2+2,i3)-ry(i1+1,i2+2,i3)*sx(i1+1,i2+2,i3))))/(144.*dr(0)*
     & *2*dr(1))
                    a22rss = (128*(rsxy(i1+1,i2+1,i3,axisp1,1)/(rx(i1+
     & 1,i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+1,i3)))
     & -240*(rsxy(i1,i2+1,i3,axisp1,1)/(rx(i1,i2+1,i3)*sy(i1,i2+1,i3)-
     & ry(i1,i2+1,i3)*sx(i1,i2+1,i3)))+128*(rsxy(i1-1,i2+1,i3,axisp1,
     & 1)/(rx(i1-1,i2+1,i3)*sy(i1-1,i2+1,i3)-ry(i1-1,i2+1,i3)*sx(i1-1,
     & i2+1,i3)))-8*(rsxy(i1+2,i2+1,i3,axisp1,1)/(rx(i1+2,i2+1,i3)*sy(
     & i1+2,i2+1,i3)-ry(i1+2,i2+1,i3)*sx(i1+2,i2+1,i3)))-8*(rsxy(i1-2,
     & i2+1,i3,axisp1,1)/(rx(i1-2,i2+1,i3)*sy(i1-2,i2+1,i3)-ry(i1-2,
     & i2+1,i3)*sx(i1-2,i2+1,i3)))-128*(rsxy(i1+1,i2-1,i3,axisp1,1)/(
     & rx(i1+1,i2-1,i3)*sy(i1+1,i2-1,i3)-ry(i1+1,i2-1,i3)*sx(i1+1,i2-
     & 1,i3)))+240*(rsxy(i1,i2-1,i3,axisp1,1)/(rx(i1,i2-1,i3)*sy(i1,
     & i2-1,i3)-ry(i1,i2-1,i3)*sx(i1,i2-1,i3)))-128*(rsxy(i1-1,i2-1,
     & i3,axisp1,1)/(rx(i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-ry(i1-1,i2-1,
     & i3)*sx(i1-1,i2-1,i3)))+8*(rsxy(i1+2,i2-1,i3,axisp1,1)/(rx(i1+2,
     & i2-1,i3)*sy(i1+2,i2-1,i3)-ry(i1+2,i2-1,i3)*sx(i1+2,i2-1,i3)))+
     & 8*(rsxy(i1-2,i2-1,i3,axisp1,1)/(rx(i1-2,i2-1,i3)*sy(i1-2,i2-1,
     & i3)-ry(i1-2,i2-1,i3)*sx(i1-2,i2-1,i3)))+30*(rsxy(i1,i2+2,i3,
     & axisp1,1)/(rx(i1,i2+2,i3)*sy(i1,i2+2,i3)-ry(i1,i2+2,i3)*sx(i1,
     & i2+2,i3)))-16*(rsxy(i1-1,i2+2,i3,axisp1,1)/(rx(i1-1,i2+2,i3)*
     & sy(i1-1,i2+2,i3)-ry(i1-1,i2+2,i3)*sx(i1-1,i2+2,i3)))+(rsxy(i1+
     & 2,i2+2,i3,axisp1,1)/(rx(i1+2,i2+2,i3)*sy(i1+2,i2+2,i3)-ry(i1+2,
     & i2+2,i3)*sx(i1+2,i2+2,i3)))+(rsxy(i1-2,i2+2,i3,axisp1,1)/(rx(
     & i1-2,i2+2,i3)*sy(i1-2,i2+2,i3)-ry(i1-2,i2+2,i3)*sx(i1-2,i2+2,
     & i3)))+16*(rsxy(i1+1,i2-2,i3,axisp1,1)/(rx(i1+1,i2-2,i3)*sy(i1+
     & 1,i2-2,i3)-ry(i1+1,i2-2,i3)*sx(i1+1,i2-2,i3)))-30*(rsxy(i1,i2-
     & 2,i3,axisp1,1)/(rx(i1,i2-2,i3)*sy(i1,i2-2,i3)-ry(i1,i2-2,i3)*
     & sx(i1,i2-2,i3)))+16*(rsxy(i1-1,i2-2,i3,axisp1,1)/(rx(i1-1,i2-2,
     & i3)*sy(i1-1,i2-2,i3)-ry(i1-1,i2-2,i3)*sx(i1-1,i2-2,i3)))-(rsxy(
     & i1+2,i2-2,i3,axisp1,1)/(rx(i1+2,i2-2,i3)*sy(i1+2,i2-2,i3)-ry(
     & i1+2,i2-2,i3)*sx(i1+2,i2-2,i3)))-(rsxy(i1-2,i2-2,i3,axisp1,1)/(
     & rx(i1-2,i2-2,i3)*sy(i1-2,i2-2,i3)-ry(i1-2,i2-2,i3)*sx(i1-2,i2-
     & 2,i3)))-16*(rsxy(i1+1,i2+2,i3,axisp1,1)/(rx(i1+1,i2+2,i3)*sy(
     & i1+1,i2+2,i3)-ry(i1+1,i2+2,i3)*sx(i1+1,i2+2,i3))))/(144.*dr(0)*
     & *2*dr(1))
                  end if
                    c11 = (rsxy(i1,i2,i3,axis,0)**2+rsxy(i1,i2,i3,axis,
     & 1)**2)
                    c22 = (rsxy(i1,i2,i3,axisp1,0)**2+rsxy(i1,i2,i3,
     & axisp1,1)**2)
                    c1 = (rsxyx42(i1,i2,i3,axis,0)+rsxyy42(i1,i2,i3,
     & axis,1))
                    c2 = (rsxyx42(i1,i2,i3,axisp1,0)+rsxyy42(i1,i2,i3,
     & axisp1,1))
                    ! *** we require only one s derivative of c11,c22,c1,c2: ****
                    ! 2nd order:
                    ! c11s = (C11(i1+js1,i2+js2,i3)-C11(i1-js1,i2-js2,i3))/(2.*dsa) 
                    ! c22s = (C22(i1+js1,i2+js2,i3)-C22(i1-js1,i2-js2,i3))/(2.*dsa) 
                    ! c1s =   (C1Order2(i1+js1,i2+js2,i3)- C1Order2(i1-js1,i2-js2,i3))/(2.*dsa) 
                    ! c2s =   (C2Order2(i1+js1,i2+js2,i3)- C2Order2(i1-js1,i2-js2,i3))/(2.*dsa) 
                    ! fourth-order:
                 !$$$   c11s = (8.*(C11(i1+  js1,i2+  js2,i3)-C11(i1-  js1,i2-  js2,i3))   !$$$             -(C11(i1+2*js1,i2+2*js2,i3)-C11(i1-2*js1,i2-2*js2,i3))   )/(12.*dsa)
                 !$$$   c22s = (8.*(C22(i1+  js1,i2+  js2,i3)-C22(i1-  js1,i2-  js2,i3))   !$$$             -(C22(i1+2*js1,i2+2*js2,i3)-C22(i1-2*js1,i2-2*js2,i3))   )/(12.*dsa)
                    c11r = (8.*((rsxy(i1+is1,i2+is2,i3,axis,0)**2+rsxy(
     & i1+is1,i2+is2,i3,axis,1)**2)-(rsxy(i1-is1,i2-is2,i3,axis,0)**2+
     & rsxy(i1-is1,i2-is2,i3,axis,1)**2))   -((rsxy(i1+2*is1,i2+2*is2,
     & i3,axis,0)**2+rsxy(i1+2*is1,i2+2*is2,i3,axis,1)**2)-(rsxy(i1-2*
     & is1,i2-2*is2,i3,axis,0)**2+rsxy(i1-2*is1,i2-2*is2,i3,axis,1)**
     & 2))   )/(12.*dra)
                    c22r = (8.*((rsxy(i1+is1,i2+is2,i3,axisp1,0)**2+
     & rsxy(i1+is1,i2+is2,i3,axisp1,1)**2)-(rsxy(i1-is1,i2-is2,i3,
     & axisp1,0)**2+rsxy(i1-is1,i2-is2,i3,axisp1,1)**2))   -((rsxy(i1+
     & 2*is1,i2+2*is2,i3,axisp1,0)**2+rsxy(i1+2*is1,i2+2*is2,i3,
     & axisp1,1)**2)-(rsxy(i1-2*is1,i2-2*is2,i3,axisp1,0)**2+rsxy(i1-
     & 2*is1,i2-2*is2,i3,axisp1,1)**2))   )/(12.*dra)
                    if( axis.eq.0 )then
                      c1r = (rsxyxr42(i1,i2,i3,axis,0)+rsxyyr42(i1,i2,
     & i3,axis,1))
                      c2r = (rsxyxr42(i1,i2,i3,axisp1,0)+rsxyyr42(i1,
     & i2,i3,axisp1,1))
                    else
                      c1r = (rsxyxs42(i1,i2,i3,axis,0)+rsxyys42(i1,i2,
     & i3,axis,1))
                      c2r = (rsxyxs42(i1,i2,i3,axisp1,0)+rsxyys42(i1,
     & i2,i3,axisp1,1))
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
                 !   ws=US2(hz)
                 !   wss=USS2(hz)
                    ws=(8.*(u(i1+js1,i2+js2,i3+js3,hz)-u(i1-js1,i2-js2,
     & i3-js3,hz))-(u(i1+2*js1,i2+2*js2,i3+2*js3,hz)-u(i1-2*js1,i2-2*
     & js2,i3-2*js3,hz)))/(12.*dsa)
                    wss=(-30.*u(i1,i2,i3,hz)+16.*(u(i1+js1,i2+js2,i3+
     & js3,hz)+u(i1-js1,i2-js2,i3-js3,hz))-(u(i1+2*js1,i2+2*js2,i3+2*
     & js3,hz)+u(i1-2*js1,i2-2*js2,i3-2*js3,hz)))/(12.*dsa**2)
                    tau1=rsxy(i1,i2,i3,axisp1,0)
                    tau2=rsxy(i1,i2,i3,axisp1,1)
                    uex=u(i1,i2,i3,ex)
                    uey=u(i1,i2,i3,ey)
                   ! Dr( a1.Delta\uv ) = (b3u,b3v).uvrrr + (b2u,b2v).uvrr + (b1u,b1v).uv + bf = 0 
                   ! For TZ choose bf = bf - Dr( a1.Delta uvExact )
                   ! see bcdiv.maple for:
      b3u=c11*a11
      b3v=a12*c11
      b2u=(2*c22*a12s*a11**2+c1*a11**2*a22+c11r*a11**2*a22-2*c22*a11s*
     & a12*a11-c1*a11*a21*a12-c11r*a11*a21*a12-a11r*c11*a21*a12+a11r*
     & c11*a11*a22)/(-a21*a12+a11*a22)
      b2v=(-2*c22*a11s*a12**2+2*c22*a12s*a11*a12+a12*c1*a11*a22+a12*
     & c11r*a11*a22-a12r*c11*a21*a12+a12r*c11*a11*a22-c11r*a21*a12**2-
     & c1*a21*a12**2)/(-a21*a12+a11*a22)
      b1u=(-4*c22*a11s*a12*a11r-c22*a11ss*a11*a22+c1r*a11**2*a22+4*c22*
     & a12s*a11*a11r-c1r*a11*a21*a12+2*c22*a11s**2*a22-2*c22*a12s*a21*
     & a11s-2*c22*a11s*a12*a21s+2*c22*a12s*a11*a21s-c2*a11*a11s*a22+
     & c22*a11ss*a21*a12+a12*c2*a21*a11s-a11r*c1*a21*a12+a11r*c1*a11*
     & a22)/(-a21*a12+a11*a22)
      b1v=(-2*c22*a21*a12s**2+2*c22*a11s*a12s*a22+2*c22*a12s*a11*a22s-
     & 2*c22*a11s*a12*a22s+c22*a12ss*a21*a12+4*c22*a12s*a11*a12r-c22*
     & a12ss*a11*a22-4*c22*a11s*a12*a12r+a12*c1r*a11*a22+a12*c2*a21*
     & a12s-a12r*c1*a21*a12+a12r*c1*a11*a22-c1r*a21*a12**2-c2*a11*
     & a12s*a22)/(-a21*a12+a11*a22)
      bf =-(-2*c22*a12s*a11*a21rs*uex+2*c22*a12s*a21*a21ss*uex+3*c22*
     & a22s*vss*a11*a22-c22r*uss*a11**2*a22-2*c22*a11s*a22**2*vss-c22*
     & a21**2*usss*a12+c22*a22**2*vsss*a11+2*c22*a12s*a21**2*uss-c2r*
     & us*a11**2*a22+c2*a11*a22**2*vss-3*c22*a21ss*us*a21*a12-2*c22*
     & a12s*a11*a11rr*uex+3*c22*a21ss*us*a11*a22-3*c22*a22ss*vs*a21*
     & a12+2*c22*a11s*a12*a22r*vs+3*c22*a22ss*vs*a11*a22+c2r*us*a11*
     & a21*a12+2*c2*a11*a22s*vs*a22-c22*a22*vsss*a21*a12-3*c22*a21s*
     & uss*a21*a12+3*c22*a21s*uss*a11*a22-c22*a12rss*uey*a21*a12+c22*
     & a12rss*uey*a11*a22-c22*a11rss*uex*a21*a12+2*c22*a12s*a21*a22ss*
     & uey+2*c22*a12s*a21*a22*vss+4*c22*a12s*a21*a21s*us+c22*a11rss*
     & uex*a11*a22-2*c22*a12rs*vs*a21*a12+2*c22*a12rs*vs*a11*a22-3*
     & c22*a22s*vss*a21*a12-c22*a21sss*uex*a21*a12+c22*a21sss*uex*a11*
     & a22-2*c22*a11rs*us*a21*a12+2*c22*a12s*a21*a11rs*uex+2*c22*a12s*
     & a21*a11r*us+2*c22*a11s*a12*a21r*us+2*c22*a11s*a12*a21rs*uex-2*
     & c22*a12s*a11*a21r*us-2*c22*a12s*a11*a22rs*uey-2*c22*a12s*a11*
     & a22r*vs+2*c22*a12s*a21*a12r*vs+2*c22*a11s*a12*a22rs*uey+2*c22*
     & a11rs*us*a11*a22+c2*a11*a21*uss*a22+c2*a11*a11rs*uex*a22-2*c22*
     & a11s*a12rs*uey*a22+c22*a22sss*uey*a11*a22-2*c22*a12s*a11*a12rr*
     & uey+4*c22*a12s*a21*a22s*vs+2*c22*a12s*a21*a12rs*uey-2*c22*a11s*
     & a21ss*uex*a22-2*c22*a11s*a22ss*uey*a22+2*c22*a11s*a12*a11rr*
     & uex-4*c22*a11s*a22s*vs*a22-2*c22*a11s*a11rs*uex*a22-2*c22*a11s*
     & a21*uss*a22-4*c22*a11s*a21s*us*a22-2*c22*a11s*a11r*us*a22+2*
     & c22*a11s*a12*a12rr*uey+c22*a21*usss*a11*a22-c22*a22sss*uey*a21*
     & a12-2*c22*a11s*a12r*vs*a22+c22r*uss*a11*a21*a12-a12*c22r*vss*
     & a11*a22-a12*c2r*vs*a11*a22-a12*c2*a21*a12rs*uey-2*a12*c2*a21*
     & a22s*vs-a12*c2*a21*a11rs*uex-a12*c2*a21*a22*vss-2*a12*c2*a21*
     & a21s*us-a12*c2*a21*a21ss*uex-a12*c2*a21*a22ss*uey-a12*c2*a21**
     & 2*uss+c2r*vs*a21*a12**2+c22r*vss*a21*a12**2+c2*a11*a12rs*uey*
     & a22+c2*a11*a22ss*uey*a22+c2*a11*a21ss*uex*a22+2*c2*a11*a21s*us*
     & a22)/(-a21*a12+a11*a22)

! -- Here are the approximations for urs, vrs from the divergence
!  ursm =-(2*a22s*vs*a22-a12*a11*urr-a12*a22r*vs-a12*a22s*vr-a12*a22rs*uey-a12*a21r*us-a12*a21s*ur-a12*a21rs*uex-2*a12*a11r*ur-a12*a11rr*uex+a12rs*uey*a22+a12r*vs*a22+a12s*vr*a22+a22ss*uey*a22+a21ss*uex*a22+2*a21s*us*a22+a21*uss*a22+a22**2*vss-a12**2*vrr+a11rs*uex*a22+a11r*us*a22-2*a12*a12r*vr+a11s*ur*a22-a12*a12rr*uey)/(-a21*a12+a11*a22)
!  vrsm =(-a11**2*urr-2*a11*a12r*vr-a11*a12rr*uey+a21*a12r*vs+a21*a12rs*uey+2*a21*a22s*vs+a21*a11s*ur+a21*a11r*us+a21*a11rs*uex+a21*a12s*vr-a11*a22r*vs-a11*a22s*vr-a11*a22rs*uey-a11*a21r*us-a11*a21s*ur-a11*a21rs*uex-a11*a12*vrr+a21*a22*vss+2*a21*a21s*us+a21*a21ss*uex+a21*a22ss*uey-2*a11*a11r*ur+a21**2*uss-a11*a11rr*uex)/(-a21*a12+a11*a22)
                 ! ************ Answer *******************
                  ctlrr=1.
                  ctlr=1.
                  ! forcing terms for TZ are stored in 
                  cgI=1.
                  gIf=0.
                  gIVf=0.
                  tau1DotUtt=0.
                  Da1DotU=0.
                  ! for Hz (w)
                  fw1=0.
                  fw2=0.
                  if( boundaryForcingOption.ne.noBoundaryForcing )then
                    ! ------------ BOUNDARY twilightZone 2D --------------
                    ! In the boundary forcing we subtract out a plane wave incident field
                    ! This causes the BC to be 
                    !           tau.u = - tau.uI
                    !   and     tau.utt = -tau.uI.tt
                    ! *** set RHS for (a1.u).r =  - Ds( a2.uv )
                    Da1DotU = -(  a21s*uex+a22s*uey + a21*us+a22*vs )
                    ! Note minus sign since we are subtracting out the incident field
                    x0=xy(i1,i2,i3,0)
                    y0=xy(i1,i2,i3,1)
                    if( .true. )then ! *new way*
                      numberOfTimeDerivatives=1+fieldOption
                        if( boundaryForcingOption.eq.noBoundaryForcing 
     & )then
                        else if( 
     & boundaryForcingOption.eq.planeWaveBoundaryForcing )then
                            if( numberOfTimeDerivatives==0 )then
                              ubv(ex) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==1 )then
                              ubv(ex) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(0)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(1)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(5)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==2 )then
                              ubv(ex) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==3 )then
                              ubv(ex) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==4 )then
                              ubv(ex) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))
                            else
                              stop 1738
                            end if
                        else if(  
     & boundaryForcingOption.eq.chirpedPlaneWaveBoundaryForcing )then
                           xi0 = .5*(cpwTa+cpwTb)
                           xi = t - (kx*(x0-cpwX0)+ky*(y0-cpwY0))/cc -
     & xi0
                           cpwTau=cpwTb-cpwTa  ! tau = tb -ta
                           ! include files generated by the maple code mx/codes/chirpedPlaneWave.maple 
                           if( numberOfTimeDerivatives.eq.0 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 0-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = .5*cpwTau
      t4 = tanh(cpwBeta*(xi+t1))
      t7 = tanh(cpwBeta*(xi-t1))
      t11 = xi ** 2
      t16 = sin(twoPi*(cc*xi+t11*cpwAlpha))
      chirp = cpwAmp*(t4/2.-t7/2.)*t16

                           else if(  numberOfTimeDerivatives.eq.1 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 1-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = .5*cpwTau
      t4 = tanh(cpwBeta*(xi+t1))
      t5 = t4 ** 2
      t10 = tanh(cpwBeta*(xi-t1))
      t11 = t10 ** 2
      t17 = xi ** 2
      t21 = twoPi*(cc*xi+t17*cpwAlpha)
      t22 = sin(t21)
      t31 = cos(t21)
      chirp = cpwAmp*(cpwBeta*(1.-t5)/2.-cpwBeta*(1.-t11)/2.)*t22+
     & cpwAmp*(t4/2.-t10/2.)*twoPi*(2.*xi*cpwAlpha+cc)*t31

                           else if(  numberOfTimeDerivatives.eq.2 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 2-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+2*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+2*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = .5*cpwTau
      t5 = tanh(cpwBeta*(xi+t2))
      t7 = t5 ** 2
      t8 = 1.-t7
      t12 = tanh(cpwBeta*(xi-t2))
      t14 = t12 ** 2
      t15 = 1.-t14
      t19 = xi ** 2
      t23 = twoPi*(cc*xi+t19*cpwAlpha)
      t24 = sin(t23)
      t33 = 2.*xi*cpwAlpha+cc
      t35 = cos(t23)
      t41 = cpwAmp*(t5/2.-t12/2.)
      t46 = twoPi ** 2
      t47 = t33 ** 2
      chirp = cpwAmp*(t1*t12*t15-t1*t5*t8)*t24+2.*cpwAmp*(-cpwBeta*
     & t15/2.+cpwBeta*t8/2.)*twoPi*t33*t35+2.*t41*twoPi*cpwAlpha*t35-
     & t41*t46*t47*t24

                           else if(  numberOfTimeDerivatives.eq.3 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 3-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(-cpwBeta^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2+2*cpwBeta^3*tanh(cpwBeta*(xi+.5*cpwTau))^2*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2-2*cpwBeta^3*tanh(cpwBeta*(xi-.5*cpwTau))^2*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+3*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+6*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-3*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-6*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*cpwAlpha*(2*xi*cpwAlpha+cc)*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^3*(2*xi*cpwAlpha+cc)^3*cos(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = t1*cpwBeta
      t3 = .5*cpwTau
      t6 = tanh(cpwBeta*(xi+t3))
      t7 = t6 ** 2
      t8 = 1.-t7
      t9 = t8 ** 2
      t16 = tanh(cpwBeta*(xi-t3))
      t17 = t16 ** 2
      t18 = 1.-t17
      t19 = t18 ** 2
      t26 = xi ** 2
      t30 = twoPi*(cc*xi+t26*cpwAlpha)
      t31 = sin(t30)
      t41 = 2.*xi*cpwAlpha+cc
      t43 = cos(t30)
      t51 = cpwAmp*(-cpwBeta*t18/2.+cpwBeta*t8/2.)
      t56 = twoPi ** 2
      t57 = t41 ** 2
      t64 = cpwAmp*(t6/2.-t16/2.)
      chirp = cpwAmp*(-2.*t2*t17*t18+2.*t2*t7*t8+t2*t19-t2*t9)*t31+
     & 0.3E1*cpwAmp*(t1*t16*t18-t1*t6*t8)*twoPi*t41*t43+6.*t51*twoPi*
     & cpwAlpha*t43-0.3E1*t51*t56*t57*t31-6.*t64*t56*cpwAlpha*t41*t31-
     & t64*t56*twoPi*t57*t41*t43

                           else if(  numberOfTimeDerivatives.eq.4 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 4-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(8*cpwBeta^4*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2*tanh(cpwBeta*(xi+.5*cpwTau))-4*cpwBeta^4*tanh(cpwBeta*(xi+.5*cpwTau))^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-8*cpwBeta^4*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2*tanh(cpwBeta*(xi-.5*cpwTau))+4*cpwBeta^4*tanh(cpwBeta*(xi-.5*cpwTau))^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+4*cpwAmp*(-cpwBeta^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2+2*cpwBeta^3*tanh(cpwBeta*(xi+.5*cpwTau))^2*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2-2*cpwBeta^3*tanh(cpwBeta*(xi-.5*cpwTau))^2*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+12*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-6*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-24*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*cpwAlpha*(2*xi*cpwAlpha+cc)*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-4*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^3*(2*xi*cpwAlpha+cc)^3*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-12*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*cpwAlpha^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-12*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^3*cpwAlpha*(2*xi*cpwAlpha+cc)^2*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^4*(2*xi*cpwAlpha+cc)^4*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = t1 ** 2
      t3 = .5*cpwTau
      t6 = tanh(cpwBeta*(xi+t3))
      t7 = t6 ** 2
      t8 = 1.-t7
      t9 = t8 ** 2
      t19 = tanh(cpwBeta*(xi-t3))
      t20 = t19 ** 2
      t21 = 1.-t20
      t22 = t21 ** 2
      t32 = xi ** 2
      t36 = twoPi*(cc*xi+t32*cpwAlpha)
      t37 = sin(t36)
      t39 = t1*cpwBeta
      t52 = 2.*xi*cpwAlpha+cc
      t54 = cos(t36)
      t63 = cpwAmp*(t1*t19*t21-t1*t6*t8)
      t68 = twoPi ** 2
      t69 = t52 ** 2
      t78 = cpwAmp*(-cpwBeta*t21/2.+cpwBeta*t8/2.)
      t84 = t68*twoPi
      t92 = cpwAmp*(t6/2.-t19/2.)
      t93 = cpwAlpha ** 2
      t103 = t68 ** 2
      t104 = t69 ** 2
      chirp = cpwAmp*(4.*t2*t20*t19*t21-4.*t2*t7*t6*t8-8.*t2*t22*t19+
     & 8.*t2*t9*t6)*t37+4.*cpwAmp*(-2.*t39*t20*t21+2.*t39*t7*t8+t39*
     & t22-t39*t9)*twoPi*t52*t54+12.*t63*twoPi*cpwAlpha*t54-6.*t63*
     & t68*t69*t37-0.24E2*t78*t68*cpwAlpha*t52*t37-4.*t78*t84*t69*t52*
     & t54-12.*t92*t68*t93*t37-12.*t92*t84*cpwAlpha*t69*t54+t92*t103*
     & t104*t37

                           else
                             write(*,'(" getChirp2D:ERROR: too many 
     & derivatives requested")')
                             stop 4927
                           end if
                           ubv(ex) = chirp*pwc(0)
                           ubv(ey) = chirp*pwc(1)
                           ubv(hz) = chirp*pwc(5)
                        else
                          write(*,'("getBndryForcing2D: Unknown 
     & boundary forcing")')
                        end if
                      ut0 = -ubv(ex)
                      vt0 = -ubv(ey)
                      numberOfTimeDerivatives=2+fieldOption
                        if( boundaryForcingOption.eq.noBoundaryForcing 
     & )then
                        else if( 
     & boundaryForcingOption.eq.planeWaveBoundaryForcing )then
                            if( numberOfTimeDerivatives==0 )then
                              ubv(ex) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==1 )then
                              ubv(ex) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(0)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(1)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(5)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==2 )then
                              ubv(ex) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==3 )then
                              ubv(ex) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==4 )then
                              ubv(ex) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))
                            else
                              stop 1738
                            end if
                        else if(  
     & boundaryForcingOption.eq.chirpedPlaneWaveBoundaryForcing )then
                           xi0 = .5*(cpwTa+cpwTb)
                           xi = t - (kx*(x0-cpwX0)+ky*(y0-cpwY0))/cc -
     & xi0
                           cpwTau=cpwTb-cpwTa  ! tau = tb -ta
                           ! include files generated by the maple code mx/codes/chirpedPlaneWave.maple 
                           if( numberOfTimeDerivatives.eq.0 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 0-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = .5*cpwTau
      t4 = tanh(cpwBeta*(xi+t1))
      t7 = tanh(cpwBeta*(xi-t1))
      t11 = xi ** 2
      t16 = sin(twoPi*(cc*xi+t11*cpwAlpha))
      chirp = cpwAmp*(t4/2.-t7/2.)*t16

                           else if(  numberOfTimeDerivatives.eq.1 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 1-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = .5*cpwTau
      t4 = tanh(cpwBeta*(xi+t1))
      t5 = t4 ** 2
      t10 = tanh(cpwBeta*(xi-t1))
      t11 = t10 ** 2
      t17 = xi ** 2
      t21 = twoPi*(cc*xi+t17*cpwAlpha)
      t22 = sin(t21)
      t31 = cos(t21)
      chirp = cpwAmp*(cpwBeta*(1.-t5)/2.-cpwBeta*(1.-t11)/2.)*t22+
     & cpwAmp*(t4/2.-t10/2.)*twoPi*(2.*xi*cpwAlpha+cc)*t31

                           else if(  numberOfTimeDerivatives.eq.2 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 2-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+2*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+2*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = .5*cpwTau
      t5 = tanh(cpwBeta*(xi+t2))
      t7 = t5 ** 2
      t8 = 1.-t7
      t12 = tanh(cpwBeta*(xi-t2))
      t14 = t12 ** 2
      t15 = 1.-t14
      t19 = xi ** 2
      t23 = twoPi*(cc*xi+t19*cpwAlpha)
      t24 = sin(t23)
      t33 = 2.*xi*cpwAlpha+cc
      t35 = cos(t23)
      t41 = cpwAmp*(t5/2.-t12/2.)
      t46 = twoPi ** 2
      t47 = t33 ** 2
      chirp = cpwAmp*(t1*t12*t15-t1*t5*t8)*t24+2.*cpwAmp*(-cpwBeta*
     & t15/2.+cpwBeta*t8/2.)*twoPi*t33*t35+2.*t41*twoPi*cpwAlpha*t35-
     & t41*t46*t47*t24

                           else if(  numberOfTimeDerivatives.eq.3 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 3-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(-cpwBeta^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2+2*cpwBeta^3*tanh(cpwBeta*(xi+.5*cpwTau))^2*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2-2*cpwBeta^3*tanh(cpwBeta*(xi-.5*cpwTau))^2*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+3*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+6*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-3*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-6*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*cpwAlpha*(2*xi*cpwAlpha+cc)*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^3*(2*xi*cpwAlpha+cc)^3*cos(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = t1*cpwBeta
      t3 = .5*cpwTau
      t6 = tanh(cpwBeta*(xi+t3))
      t7 = t6 ** 2
      t8 = 1.-t7
      t9 = t8 ** 2
      t16 = tanh(cpwBeta*(xi-t3))
      t17 = t16 ** 2
      t18 = 1.-t17
      t19 = t18 ** 2
      t26 = xi ** 2
      t30 = twoPi*(cc*xi+t26*cpwAlpha)
      t31 = sin(t30)
      t41 = 2.*xi*cpwAlpha+cc
      t43 = cos(t30)
      t51 = cpwAmp*(-cpwBeta*t18/2.+cpwBeta*t8/2.)
      t56 = twoPi ** 2
      t57 = t41 ** 2
      t64 = cpwAmp*(t6/2.-t16/2.)
      chirp = cpwAmp*(-2.*t2*t17*t18+2.*t2*t7*t8+t2*t19-t2*t9)*t31+
     & 0.3E1*cpwAmp*(t1*t16*t18-t1*t6*t8)*twoPi*t41*t43+6.*t51*twoPi*
     & cpwAlpha*t43-0.3E1*t51*t56*t57*t31-6.*t64*t56*cpwAlpha*t41*t31-
     & t64*t56*twoPi*t57*t41*t43

                           else if(  numberOfTimeDerivatives.eq.4 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 4-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(8*cpwBeta^4*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2*tanh(cpwBeta*(xi+.5*cpwTau))-4*cpwBeta^4*tanh(cpwBeta*(xi+.5*cpwTau))^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-8*cpwBeta^4*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2*tanh(cpwBeta*(xi-.5*cpwTau))+4*cpwBeta^4*tanh(cpwBeta*(xi-.5*cpwTau))^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+4*cpwAmp*(-cpwBeta^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2+2*cpwBeta^3*tanh(cpwBeta*(xi+.5*cpwTau))^2*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2-2*cpwBeta^3*tanh(cpwBeta*(xi-.5*cpwTau))^2*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+12*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-6*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-24*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*cpwAlpha*(2*xi*cpwAlpha+cc)*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-4*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^3*(2*xi*cpwAlpha+cc)^3*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-12*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*cpwAlpha^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-12*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^3*cpwAlpha*(2*xi*cpwAlpha+cc)^2*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^4*(2*xi*cpwAlpha+cc)^4*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = t1 ** 2
      t3 = .5*cpwTau
      t6 = tanh(cpwBeta*(xi+t3))
      t7 = t6 ** 2
      t8 = 1.-t7
      t9 = t8 ** 2
      t19 = tanh(cpwBeta*(xi-t3))
      t20 = t19 ** 2
      t21 = 1.-t20
      t22 = t21 ** 2
      t32 = xi ** 2
      t36 = twoPi*(cc*xi+t32*cpwAlpha)
      t37 = sin(t36)
      t39 = t1*cpwBeta
      t52 = 2.*xi*cpwAlpha+cc
      t54 = cos(t36)
      t63 = cpwAmp*(t1*t19*t21-t1*t6*t8)
      t68 = twoPi ** 2
      t69 = t52 ** 2
      t78 = cpwAmp*(-cpwBeta*t21/2.+cpwBeta*t8/2.)
      t84 = t68*twoPi
      t92 = cpwAmp*(t6/2.-t19/2.)
      t93 = cpwAlpha ** 2
      t103 = t68 ** 2
      t104 = t69 ** 2
      chirp = cpwAmp*(4.*t2*t20*t19*t21-4.*t2*t7*t6*t8-8.*t2*t22*t19+
     & 8.*t2*t9*t6)*t37+4.*cpwAmp*(-2.*t39*t20*t21+2.*t39*t7*t8+t39*
     & t22-t39*t9)*twoPi*t52*t54+12.*t63*twoPi*cpwAlpha*t54-6.*t63*
     & t68*t69*t37-0.24E2*t78*t68*cpwAlpha*t52*t37-4.*t78*t84*t69*t52*
     & t54-12.*t92*t68*t93*t37-12.*t92*t84*cpwAlpha*t69*t54+t92*t103*
     & t104*t37

                           else
                             write(*,'(" getChirp2D:ERROR: too many 
     & derivatives requested")')
                             stop 4927
                           end if
                           ubv(ex) = chirp*pwc(0)
                           ubv(ey) = chirp*pwc(1)
                           ubv(hz) = chirp*pwc(5)
                        else
                          write(*,'("getBndryForcing2D: Unknown 
     & boundary forcing")')
                        end if
                      utt00 = -ubv(ex)
                      vtt00 = -ubv(ey)
                      numberOfTimeDerivatives=3+fieldOption
                        if( boundaryForcingOption.eq.noBoundaryForcing 
     & )then
                        else if( 
     & boundaryForcingOption.eq.planeWaveBoundaryForcing )then
                            if( numberOfTimeDerivatives==0 )then
                              ubv(ex) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==1 )then
                              ubv(ex) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(0)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(1)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(5)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==2 )then
                              ubv(ex) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==3 )then
                              ubv(ex) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==4 )then
                              ubv(ex) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))
                            else
                              stop 1738
                            end if
                        else if(  
     & boundaryForcingOption.eq.chirpedPlaneWaveBoundaryForcing )then
                           xi0 = .5*(cpwTa+cpwTb)
                           xi = t - (kx*(x0-cpwX0)+ky*(y0-cpwY0))/cc -
     & xi0
                           cpwTau=cpwTb-cpwTa  ! tau = tb -ta
                           ! include files generated by the maple code mx/codes/chirpedPlaneWave.maple 
                           if( numberOfTimeDerivatives.eq.0 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 0-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = .5*cpwTau
      t4 = tanh(cpwBeta*(xi+t1))
      t7 = tanh(cpwBeta*(xi-t1))
      t11 = xi ** 2
      t16 = sin(twoPi*(cc*xi+t11*cpwAlpha))
      chirp = cpwAmp*(t4/2.-t7/2.)*t16

                           else if(  numberOfTimeDerivatives.eq.1 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 1-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = .5*cpwTau
      t4 = tanh(cpwBeta*(xi+t1))
      t5 = t4 ** 2
      t10 = tanh(cpwBeta*(xi-t1))
      t11 = t10 ** 2
      t17 = xi ** 2
      t21 = twoPi*(cc*xi+t17*cpwAlpha)
      t22 = sin(t21)
      t31 = cos(t21)
      chirp = cpwAmp*(cpwBeta*(1.-t5)/2.-cpwBeta*(1.-t11)/2.)*t22+
     & cpwAmp*(t4/2.-t10/2.)*twoPi*(2.*xi*cpwAlpha+cc)*t31

                           else if(  numberOfTimeDerivatives.eq.2 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 2-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+2*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+2*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = .5*cpwTau
      t5 = tanh(cpwBeta*(xi+t2))
      t7 = t5 ** 2
      t8 = 1.-t7
      t12 = tanh(cpwBeta*(xi-t2))
      t14 = t12 ** 2
      t15 = 1.-t14
      t19 = xi ** 2
      t23 = twoPi*(cc*xi+t19*cpwAlpha)
      t24 = sin(t23)
      t33 = 2.*xi*cpwAlpha+cc
      t35 = cos(t23)
      t41 = cpwAmp*(t5/2.-t12/2.)
      t46 = twoPi ** 2
      t47 = t33 ** 2
      chirp = cpwAmp*(t1*t12*t15-t1*t5*t8)*t24+2.*cpwAmp*(-cpwBeta*
     & t15/2.+cpwBeta*t8/2.)*twoPi*t33*t35+2.*t41*twoPi*cpwAlpha*t35-
     & t41*t46*t47*t24

                           else if(  numberOfTimeDerivatives.eq.3 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 3-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(-cpwBeta^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2+2*cpwBeta^3*tanh(cpwBeta*(xi+.5*cpwTau))^2*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2-2*cpwBeta^3*tanh(cpwBeta*(xi-.5*cpwTau))^2*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+3*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+6*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-3*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-6*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*cpwAlpha*(2*xi*cpwAlpha+cc)*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^3*(2*xi*cpwAlpha+cc)^3*cos(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = t1*cpwBeta
      t3 = .5*cpwTau
      t6 = tanh(cpwBeta*(xi+t3))
      t7 = t6 ** 2
      t8 = 1.-t7
      t9 = t8 ** 2
      t16 = tanh(cpwBeta*(xi-t3))
      t17 = t16 ** 2
      t18 = 1.-t17
      t19 = t18 ** 2
      t26 = xi ** 2
      t30 = twoPi*(cc*xi+t26*cpwAlpha)
      t31 = sin(t30)
      t41 = 2.*xi*cpwAlpha+cc
      t43 = cos(t30)
      t51 = cpwAmp*(-cpwBeta*t18/2.+cpwBeta*t8/2.)
      t56 = twoPi ** 2
      t57 = t41 ** 2
      t64 = cpwAmp*(t6/2.-t16/2.)
      chirp = cpwAmp*(-2.*t2*t17*t18+2.*t2*t7*t8+t2*t19-t2*t9)*t31+
     & 0.3E1*cpwAmp*(t1*t16*t18-t1*t6*t8)*twoPi*t41*t43+6.*t51*twoPi*
     & cpwAlpha*t43-0.3E1*t51*t56*t57*t31-6.*t64*t56*cpwAlpha*t41*t31-
     & t64*t56*twoPi*t57*t41*t43

                           else if(  numberOfTimeDerivatives.eq.4 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 4-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(8*cpwBeta^4*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2*tanh(cpwBeta*(xi+.5*cpwTau))-4*cpwBeta^4*tanh(cpwBeta*(xi+.5*cpwTau))^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-8*cpwBeta^4*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2*tanh(cpwBeta*(xi-.5*cpwTau))+4*cpwBeta^4*tanh(cpwBeta*(xi-.5*cpwTau))^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+4*cpwAmp*(-cpwBeta^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2+2*cpwBeta^3*tanh(cpwBeta*(xi+.5*cpwTau))^2*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2-2*cpwBeta^3*tanh(cpwBeta*(xi-.5*cpwTau))^2*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+12*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-6*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-24*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*cpwAlpha*(2*xi*cpwAlpha+cc)*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-4*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^3*(2*xi*cpwAlpha+cc)^3*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-12*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*cpwAlpha^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-12*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^3*cpwAlpha*(2*xi*cpwAlpha+cc)^2*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^4*(2*xi*cpwAlpha+cc)^4*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = t1 ** 2
      t3 = .5*cpwTau
      t6 = tanh(cpwBeta*(xi+t3))
      t7 = t6 ** 2
      t8 = 1.-t7
      t9 = t8 ** 2
      t19 = tanh(cpwBeta*(xi-t3))
      t20 = t19 ** 2
      t21 = 1.-t20
      t22 = t21 ** 2
      t32 = xi ** 2
      t36 = twoPi*(cc*xi+t32*cpwAlpha)
      t37 = sin(t36)
      t39 = t1*cpwBeta
      t52 = 2.*xi*cpwAlpha+cc
      t54 = cos(t36)
      t63 = cpwAmp*(t1*t19*t21-t1*t6*t8)
      t68 = twoPi ** 2
      t69 = t52 ** 2
      t78 = cpwAmp*(-cpwBeta*t21/2.+cpwBeta*t8/2.)
      t84 = t68*twoPi
      t92 = cpwAmp*(t6/2.-t19/2.)
      t93 = cpwAlpha ** 2
      t103 = t68 ** 2
      t104 = t69 ** 2
      chirp = cpwAmp*(4.*t2*t20*t19*t21-4.*t2*t7*t6*t8-8.*t2*t22*t19+
     & 8.*t2*t9*t6)*t37+4.*cpwAmp*(-2.*t39*t20*t21+2.*t39*t7*t8+t39*
     & t22-t39*t9)*twoPi*t52*t54+12.*t63*twoPi*cpwAlpha*t54-6.*t63*
     & t68*t69*t37-0.24E2*t78*t68*cpwAlpha*t52*t37-4.*t78*t84*t69*t52*
     & t54-12.*t92*t68*t93*t37-12.*t92*t84*cpwAlpha*t69*t54+t92*t103*
     & t104*t37

                           else
                             write(*,'(" getChirp2D:ERROR: too many 
     & derivatives requested")')
                             stop 4927
                           end if
                           ubv(ex) = chirp*pwc(0)
                           ubv(ey) = chirp*pwc(1)
                           ubv(hz) = chirp*pwc(5)
                        else
                          write(*,'("getBndryForcing2D: Unknown 
     & boundary forcing")')
                        end if
                      uttt0 = -ubv(ex)
                      vttt0 = -ubv(ey)
                    else if( fieldOption.eq.0 )then ! *old* way
                      utt00=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x0)+
     & ky*(y0)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(0)+ssftt*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(
     & t)))*pwc(0))
                      vtt00=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x0)+
     & ky*(y0)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(1)+ssftt*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(
     & t)))*pwc(1))
                      ut0  =-(ssf*(-twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(
     & y0)-cc*(t)))*pwc(0)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*
     & pwc(0))
                      vt0  =-(ssf*(-twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(
     & y0)-cc*(t)))*pwc(1)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*
     & pwc(1))
                      uttt0=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x0)+ky*
     & (y0)-cc*(t)))*pwc(0))+3.*ssft*(-(twoPi*cc)**2*sin(twoPi*(kx*(
     & x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssftt*(-twoPi*cc)*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssfttt*sin(twoPi*(kx*(x0)+ky*(
     & y0)-cc*(t)))*pwc(0))
                      vttt0=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x0)+ky*
     & (y0)-cc*(t)))*pwc(1))+3.*ssft*(-(twoPi*cc)**2*sin(twoPi*(kx*(
     & x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssftt*(-twoPi*cc)*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssfttt*sin(twoPi*(kx*(x0)+ky*(
     & y0)-cc*(t)))*pwc(1))
                    else
                      ! we are assigning time derivatives (sosup)
                      utt00=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x0)+ky*
     & (y0)-cc*(t)))*pwc(0))+3.*ssft*(-(twoPi*cc)**2*sin(twoPi*(kx*(
     & x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssftt*(-twoPi*cc)*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssfttt*sin(twoPi*(kx*(x0)+ky*(
     & y0)-cc*(t)))*pwc(0))
                      vtt00=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x0)+ky*
     & (y0)-cc*(t)))*pwc(1))+3.*ssft*(-(twoPi*cc)**2*sin(twoPi*(kx*(
     & x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssftt*(-twoPi*cc)*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssfttt*sin(twoPi*(kx*(x0)+ky*(
     & y0)-cc*(t)))*pwc(1))
                      ut0  =-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x0)+
     & ky*(y0)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(0)+ssftt*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(
     & t)))*pwc(0))
                      vt0  =-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x0)+
     & ky*(y0)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(1)+ssftt*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(
     & t)))*pwc(1))
                      uttt0=-(ssf*((twoPi*cc)**4*sin(twoPi*(kx*(x0)+ky*
     & (y0)-cc*(t)))*pwc(0))+4.*ssft*((twoPi*cc)**3*cos(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(0))+6.*ssftt*(-(twoPi*cc)**2*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+4.*ssfttt*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssftttt*sin(twoPi*(kx*(
     & x0)+ky*(y0)-cc*(t)))*pwc(0))
                      vttt0=-(ssf*((twoPi*cc)**4*sin(twoPi*(kx*(x0)+ky*
     & (y0)-cc*(t)))*pwc(1))+4.*ssft*((twoPi*cc)**3*cos(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(1))+6.*ssftt*(-(twoPi*cc)**2*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+4.*ssfttt*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssftttt*sin(twoPi*(kx*(
     & x0)+ky*(y0)-cc*(t)))*pwc(1))
                    end if
                    tau1DotUtt = tau1*utt00+tau2*vtt00
                    ! (a1.Delta u).r = - (a2.utt).s
                    ! (a1.Delta u).r + bf = 0
                    ! bf = bf + ( (a21zp1*uttzp1+a22zp1*vttzp1)-(a21zm1*uttzm1+a22zm1*vttzm1) )/(2.(dsa)
                    x0=xy(i1+js1,i2+js2,i3,0)
                    y0=xy(i1+js1,i2+js2,i3,1)
                    if( .true. )then ! *new way*
                      numberOfTimeDerivatives=1+fieldOption
                        if( boundaryForcingOption.eq.noBoundaryForcing 
     & )then
                        else if( 
     & boundaryForcingOption.eq.planeWaveBoundaryForcing )then
                            if( numberOfTimeDerivatives==0 )then
                              ubv(ex) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==1 )then
                              ubv(ex) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(0)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(1)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(5)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==2 )then
                              ubv(ex) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==3 )then
                              ubv(ex) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==4 )then
                              ubv(ex) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))
                            else
                              stop 1738
                            end if
                        else if(  
     & boundaryForcingOption.eq.chirpedPlaneWaveBoundaryForcing )then
                           xi0 = .5*(cpwTa+cpwTb)
                           xi = t - (kx*(x0-cpwX0)+ky*(y0-cpwY0))/cc -
     & xi0
                           cpwTau=cpwTb-cpwTa  ! tau = tb -ta
                           ! include files generated by the maple code mx/codes/chirpedPlaneWave.maple 
                           if( numberOfTimeDerivatives.eq.0 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 0-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = .5*cpwTau
      t4 = tanh(cpwBeta*(xi+t1))
      t7 = tanh(cpwBeta*(xi-t1))
      t11 = xi ** 2
      t16 = sin(twoPi*(cc*xi+t11*cpwAlpha))
      chirp = cpwAmp*(t4/2.-t7/2.)*t16

                           else if(  numberOfTimeDerivatives.eq.1 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 1-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = .5*cpwTau
      t4 = tanh(cpwBeta*(xi+t1))
      t5 = t4 ** 2
      t10 = tanh(cpwBeta*(xi-t1))
      t11 = t10 ** 2
      t17 = xi ** 2
      t21 = twoPi*(cc*xi+t17*cpwAlpha)
      t22 = sin(t21)
      t31 = cos(t21)
      chirp = cpwAmp*(cpwBeta*(1.-t5)/2.-cpwBeta*(1.-t11)/2.)*t22+
     & cpwAmp*(t4/2.-t10/2.)*twoPi*(2.*xi*cpwAlpha+cc)*t31

                           else if(  numberOfTimeDerivatives.eq.2 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 2-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+2*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+2*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = .5*cpwTau
      t5 = tanh(cpwBeta*(xi+t2))
      t7 = t5 ** 2
      t8 = 1.-t7
      t12 = tanh(cpwBeta*(xi-t2))
      t14 = t12 ** 2
      t15 = 1.-t14
      t19 = xi ** 2
      t23 = twoPi*(cc*xi+t19*cpwAlpha)
      t24 = sin(t23)
      t33 = 2.*xi*cpwAlpha+cc
      t35 = cos(t23)
      t41 = cpwAmp*(t5/2.-t12/2.)
      t46 = twoPi ** 2
      t47 = t33 ** 2
      chirp = cpwAmp*(t1*t12*t15-t1*t5*t8)*t24+2.*cpwAmp*(-cpwBeta*
     & t15/2.+cpwBeta*t8/2.)*twoPi*t33*t35+2.*t41*twoPi*cpwAlpha*t35-
     & t41*t46*t47*t24

                           else if(  numberOfTimeDerivatives.eq.3 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 3-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(-cpwBeta^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2+2*cpwBeta^3*tanh(cpwBeta*(xi+.5*cpwTau))^2*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2-2*cpwBeta^3*tanh(cpwBeta*(xi-.5*cpwTau))^2*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+3*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+6*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-3*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-6*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*cpwAlpha*(2*xi*cpwAlpha+cc)*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^3*(2*xi*cpwAlpha+cc)^3*cos(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = t1*cpwBeta
      t3 = .5*cpwTau
      t6 = tanh(cpwBeta*(xi+t3))
      t7 = t6 ** 2
      t8 = 1.-t7
      t9 = t8 ** 2
      t16 = tanh(cpwBeta*(xi-t3))
      t17 = t16 ** 2
      t18 = 1.-t17
      t19 = t18 ** 2
      t26 = xi ** 2
      t30 = twoPi*(cc*xi+t26*cpwAlpha)
      t31 = sin(t30)
      t41 = 2.*xi*cpwAlpha+cc
      t43 = cos(t30)
      t51 = cpwAmp*(-cpwBeta*t18/2.+cpwBeta*t8/2.)
      t56 = twoPi ** 2
      t57 = t41 ** 2
      t64 = cpwAmp*(t6/2.-t16/2.)
      chirp = cpwAmp*(-2.*t2*t17*t18+2.*t2*t7*t8+t2*t19-t2*t9)*t31+
     & 0.3E1*cpwAmp*(t1*t16*t18-t1*t6*t8)*twoPi*t41*t43+6.*t51*twoPi*
     & cpwAlpha*t43-0.3E1*t51*t56*t57*t31-6.*t64*t56*cpwAlpha*t41*t31-
     & t64*t56*twoPi*t57*t41*t43

                           else if(  numberOfTimeDerivatives.eq.4 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 4-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(8*cpwBeta^4*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2*tanh(cpwBeta*(xi+.5*cpwTau))-4*cpwBeta^4*tanh(cpwBeta*(xi+.5*cpwTau))^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-8*cpwBeta^4*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2*tanh(cpwBeta*(xi-.5*cpwTau))+4*cpwBeta^4*tanh(cpwBeta*(xi-.5*cpwTau))^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+4*cpwAmp*(-cpwBeta^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2+2*cpwBeta^3*tanh(cpwBeta*(xi+.5*cpwTau))^2*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2-2*cpwBeta^3*tanh(cpwBeta*(xi-.5*cpwTau))^2*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+12*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-6*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-24*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*cpwAlpha*(2*xi*cpwAlpha+cc)*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-4*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^3*(2*xi*cpwAlpha+cc)^3*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-12*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*cpwAlpha^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-12*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^3*cpwAlpha*(2*xi*cpwAlpha+cc)^2*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^4*(2*xi*cpwAlpha+cc)^4*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = t1 ** 2
      t3 = .5*cpwTau
      t6 = tanh(cpwBeta*(xi+t3))
      t7 = t6 ** 2
      t8 = 1.-t7
      t9 = t8 ** 2
      t19 = tanh(cpwBeta*(xi-t3))
      t20 = t19 ** 2
      t21 = 1.-t20
      t22 = t21 ** 2
      t32 = xi ** 2
      t36 = twoPi*(cc*xi+t32*cpwAlpha)
      t37 = sin(t36)
      t39 = t1*cpwBeta
      t52 = 2.*xi*cpwAlpha+cc
      t54 = cos(t36)
      t63 = cpwAmp*(t1*t19*t21-t1*t6*t8)
      t68 = twoPi ** 2
      t69 = t52 ** 2
      t78 = cpwAmp*(-cpwBeta*t21/2.+cpwBeta*t8/2.)
      t84 = t68*twoPi
      t92 = cpwAmp*(t6/2.-t19/2.)
      t93 = cpwAlpha ** 2
      t103 = t68 ** 2
      t104 = t69 ** 2
      chirp = cpwAmp*(4.*t2*t20*t19*t21-4.*t2*t7*t6*t8-8.*t2*t22*t19+
     & 8.*t2*t9*t6)*t37+4.*cpwAmp*(-2.*t39*t20*t21+2.*t39*t7*t8+t39*
     & t22-t39*t9)*twoPi*t52*t54+12.*t63*twoPi*cpwAlpha*t54-6.*t63*
     & t68*t69*t37-0.24E2*t78*t68*cpwAlpha*t52*t37-4.*t78*t84*t69*t52*
     & t54-12.*t92*t68*t93*t37-12.*t92*t84*cpwAlpha*t69*t54+t92*t103*
     & t104*t37

                           else
                             write(*,'(" getChirp2D:ERROR: too many 
     & derivatives requested")')
                             stop 4927
                           end if
                           ubv(ex) = chirp*pwc(0)
                           ubv(ey) = chirp*pwc(1)
                           ubv(hz) = chirp*pwc(5)
                        else
                          write(*,'("getBndryForcing2D: Unknown 
     & boundary forcing")')
                        end if
                      utp1 = -ubv(ex)
                      vtp1 = -ubv(ey)
                      numberOfTimeDerivatives=2+fieldOption
                        if( boundaryForcingOption.eq.noBoundaryForcing 
     & )then
                        else if( 
     & boundaryForcingOption.eq.planeWaveBoundaryForcing )then
                            if( numberOfTimeDerivatives==0 )then
                              ubv(ex) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==1 )then
                              ubv(ex) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(0)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(1)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(5)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==2 )then
                              ubv(ex) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==3 )then
                              ubv(ex) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==4 )then
                              ubv(ex) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))
                            else
                              stop 1738
                            end if
                        else if(  
     & boundaryForcingOption.eq.chirpedPlaneWaveBoundaryForcing )then
                           xi0 = .5*(cpwTa+cpwTb)
                           xi = t - (kx*(x0-cpwX0)+ky*(y0-cpwY0))/cc -
     & xi0
                           cpwTau=cpwTb-cpwTa  ! tau = tb -ta
                           ! include files generated by the maple code mx/codes/chirpedPlaneWave.maple 
                           if( numberOfTimeDerivatives.eq.0 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 0-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = .5*cpwTau
      t4 = tanh(cpwBeta*(xi+t1))
      t7 = tanh(cpwBeta*(xi-t1))
      t11 = xi ** 2
      t16 = sin(twoPi*(cc*xi+t11*cpwAlpha))
      chirp = cpwAmp*(t4/2.-t7/2.)*t16

                           else if(  numberOfTimeDerivatives.eq.1 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 1-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = .5*cpwTau
      t4 = tanh(cpwBeta*(xi+t1))
      t5 = t4 ** 2
      t10 = tanh(cpwBeta*(xi-t1))
      t11 = t10 ** 2
      t17 = xi ** 2
      t21 = twoPi*(cc*xi+t17*cpwAlpha)
      t22 = sin(t21)
      t31 = cos(t21)
      chirp = cpwAmp*(cpwBeta*(1.-t5)/2.-cpwBeta*(1.-t11)/2.)*t22+
     & cpwAmp*(t4/2.-t10/2.)*twoPi*(2.*xi*cpwAlpha+cc)*t31

                           else if(  numberOfTimeDerivatives.eq.2 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 2-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+2*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+2*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = .5*cpwTau
      t5 = tanh(cpwBeta*(xi+t2))
      t7 = t5 ** 2
      t8 = 1.-t7
      t12 = tanh(cpwBeta*(xi-t2))
      t14 = t12 ** 2
      t15 = 1.-t14
      t19 = xi ** 2
      t23 = twoPi*(cc*xi+t19*cpwAlpha)
      t24 = sin(t23)
      t33 = 2.*xi*cpwAlpha+cc
      t35 = cos(t23)
      t41 = cpwAmp*(t5/2.-t12/2.)
      t46 = twoPi ** 2
      t47 = t33 ** 2
      chirp = cpwAmp*(t1*t12*t15-t1*t5*t8)*t24+2.*cpwAmp*(-cpwBeta*
     & t15/2.+cpwBeta*t8/2.)*twoPi*t33*t35+2.*t41*twoPi*cpwAlpha*t35-
     & t41*t46*t47*t24

                           else if(  numberOfTimeDerivatives.eq.3 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 3-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(-cpwBeta^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2+2*cpwBeta^3*tanh(cpwBeta*(xi+.5*cpwTau))^2*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2-2*cpwBeta^3*tanh(cpwBeta*(xi-.5*cpwTau))^2*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+3*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+6*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-3*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-6*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*cpwAlpha*(2*xi*cpwAlpha+cc)*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^3*(2*xi*cpwAlpha+cc)^3*cos(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = t1*cpwBeta
      t3 = .5*cpwTau
      t6 = tanh(cpwBeta*(xi+t3))
      t7 = t6 ** 2
      t8 = 1.-t7
      t9 = t8 ** 2
      t16 = tanh(cpwBeta*(xi-t3))
      t17 = t16 ** 2
      t18 = 1.-t17
      t19 = t18 ** 2
      t26 = xi ** 2
      t30 = twoPi*(cc*xi+t26*cpwAlpha)
      t31 = sin(t30)
      t41 = 2.*xi*cpwAlpha+cc
      t43 = cos(t30)
      t51 = cpwAmp*(-cpwBeta*t18/2.+cpwBeta*t8/2.)
      t56 = twoPi ** 2
      t57 = t41 ** 2
      t64 = cpwAmp*(t6/2.-t16/2.)
      chirp = cpwAmp*(-2.*t2*t17*t18+2.*t2*t7*t8+t2*t19-t2*t9)*t31+
     & 0.3E1*cpwAmp*(t1*t16*t18-t1*t6*t8)*twoPi*t41*t43+6.*t51*twoPi*
     & cpwAlpha*t43-0.3E1*t51*t56*t57*t31-6.*t64*t56*cpwAlpha*t41*t31-
     & t64*t56*twoPi*t57*t41*t43

                           else if(  numberOfTimeDerivatives.eq.4 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 4-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(8*cpwBeta^4*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2*tanh(cpwBeta*(xi+.5*cpwTau))-4*cpwBeta^4*tanh(cpwBeta*(xi+.5*cpwTau))^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-8*cpwBeta^4*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2*tanh(cpwBeta*(xi-.5*cpwTau))+4*cpwBeta^4*tanh(cpwBeta*(xi-.5*cpwTau))^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+4*cpwAmp*(-cpwBeta^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2+2*cpwBeta^3*tanh(cpwBeta*(xi+.5*cpwTau))^2*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2-2*cpwBeta^3*tanh(cpwBeta*(xi-.5*cpwTau))^2*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+12*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-6*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-24*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*cpwAlpha*(2*xi*cpwAlpha+cc)*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-4*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^3*(2*xi*cpwAlpha+cc)^3*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-12*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*cpwAlpha^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-12*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^3*cpwAlpha*(2*xi*cpwAlpha+cc)^2*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^4*(2*xi*cpwAlpha+cc)^4*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = t1 ** 2
      t3 = .5*cpwTau
      t6 = tanh(cpwBeta*(xi+t3))
      t7 = t6 ** 2
      t8 = 1.-t7
      t9 = t8 ** 2
      t19 = tanh(cpwBeta*(xi-t3))
      t20 = t19 ** 2
      t21 = 1.-t20
      t22 = t21 ** 2
      t32 = xi ** 2
      t36 = twoPi*(cc*xi+t32*cpwAlpha)
      t37 = sin(t36)
      t39 = t1*cpwBeta
      t52 = 2.*xi*cpwAlpha+cc
      t54 = cos(t36)
      t63 = cpwAmp*(t1*t19*t21-t1*t6*t8)
      t68 = twoPi ** 2
      t69 = t52 ** 2
      t78 = cpwAmp*(-cpwBeta*t21/2.+cpwBeta*t8/2.)
      t84 = t68*twoPi
      t92 = cpwAmp*(t6/2.-t19/2.)
      t93 = cpwAlpha ** 2
      t103 = t68 ** 2
      t104 = t69 ** 2
      chirp = cpwAmp*(4.*t2*t20*t19*t21-4.*t2*t7*t6*t8-8.*t2*t22*t19+
     & 8.*t2*t9*t6)*t37+4.*cpwAmp*(-2.*t39*t20*t21+2.*t39*t7*t8+t39*
     & t22-t39*t9)*twoPi*t52*t54+12.*t63*twoPi*cpwAlpha*t54-6.*t63*
     & t68*t69*t37-0.24E2*t78*t68*cpwAlpha*t52*t37-4.*t78*t84*t69*t52*
     & t54-12.*t92*t68*t93*t37-12.*t92*t84*cpwAlpha*t69*t54+t92*t103*
     & t104*t37

                           else
                             write(*,'(" getChirp2D:ERROR: too many 
     & derivatives requested")')
                             stop 4927
                           end if
                           ubv(ex) = chirp*pwc(0)
                           ubv(ey) = chirp*pwc(1)
                           ubv(hz) = chirp*pwc(5)
                        else
                          write(*,'("getBndryForcing2D: Unknown 
     & boundary forcing")')
                        end if
                      uttp1 = -ubv(ex)
                      vttp1 = -ubv(ey)
                    else if( fieldOption.eq.0 )then ! *old way*
                      uttp1=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x0)+
     & ky*(y0)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(0)+ssftt*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(
     & t)))*pwc(0))
                      vttp1=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x0)+
     & ky*(y0)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(1)+ssftt*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(
     & t)))*pwc(1))
                      utp1 =-(ssf*(-twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(
     & y0)-cc*(t)))*pwc(0)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*
     & pwc(0))
                      vtp1 =-(ssf*(-twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(
     & y0)-cc*(t)))*pwc(1)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*
     & pwc(1))
                    else
                      ! we are assigning time derivatives (sosup)
                      uttp1=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x0)+ky*
     & (y0)-cc*(t)))*pwc(0))+3.*ssft*(-(twoPi*cc)**2*sin(twoPi*(kx*(
     & x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssftt*(-twoPi*cc)*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssfttt*sin(twoPi*(kx*(x0)+ky*(
     & y0)-cc*(t)))*pwc(0))
                      vttp1=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x0)+ky*
     & (y0)-cc*(t)))*pwc(1))+3.*ssft*(-(twoPi*cc)**2*sin(twoPi*(kx*(
     & x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssftt*(-twoPi*cc)*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssfttt*sin(twoPi*(kx*(x0)+ky*(
     & y0)-cc*(t)))*pwc(1))
                      utp1 =-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x0)+
     & ky*(y0)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(0)+ssftt*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(
     & t)))*pwc(0))
                      vtp1 =-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x0)+
     & ky*(y0)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(1)+ssftt*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(
     & t)))*pwc(1))
                    end if
                    x0=xy(i1-js1,i2-js2,i3,0)
                    y0=xy(i1-js1,i2-js2,i3,1)
                    if( .true. )then ! *new way*
                      numberOfTimeDerivatives=1+fieldOption
                        if( boundaryForcingOption.eq.noBoundaryForcing 
     & )then
                        else if( 
     & boundaryForcingOption.eq.planeWaveBoundaryForcing )then
                            if( numberOfTimeDerivatives==0 )then
                              ubv(ex) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==1 )then
                              ubv(ex) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(0)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(1)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(5)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==2 )then
                              ubv(ex) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==3 )then
                              ubv(ex) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==4 )then
                              ubv(ex) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))
                            else
                              stop 1738
                            end if
                        else if(  
     & boundaryForcingOption.eq.chirpedPlaneWaveBoundaryForcing )then
                           xi0 = .5*(cpwTa+cpwTb)
                           xi = t - (kx*(x0-cpwX0)+ky*(y0-cpwY0))/cc -
     & xi0
                           cpwTau=cpwTb-cpwTa  ! tau = tb -ta
                           ! include files generated by the maple code mx/codes/chirpedPlaneWave.maple 
                           if( numberOfTimeDerivatives.eq.0 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 0-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = .5*cpwTau
      t4 = tanh(cpwBeta*(xi+t1))
      t7 = tanh(cpwBeta*(xi-t1))
      t11 = xi ** 2
      t16 = sin(twoPi*(cc*xi+t11*cpwAlpha))
      chirp = cpwAmp*(t4/2.-t7/2.)*t16

                           else if(  numberOfTimeDerivatives.eq.1 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 1-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = .5*cpwTau
      t4 = tanh(cpwBeta*(xi+t1))
      t5 = t4 ** 2
      t10 = tanh(cpwBeta*(xi-t1))
      t11 = t10 ** 2
      t17 = xi ** 2
      t21 = twoPi*(cc*xi+t17*cpwAlpha)
      t22 = sin(t21)
      t31 = cos(t21)
      chirp = cpwAmp*(cpwBeta*(1.-t5)/2.-cpwBeta*(1.-t11)/2.)*t22+
     & cpwAmp*(t4/2.-t10/2.)*twoPi*(2.*xi*cpwAlpha+cc)*t31

                           else if(  numberOfTimeDerivatives.eq.2 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 2-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+2*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+2*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = .5*cpwTau
      t5 = tanh(cpwBeta*(xi+t2))
      t7 = t5 ** 2
      t8 = 1.-t7
      t12 = tanh(cpwBeta*(xi-t2))
      t14 = t12 ** 2
      t15 = 1.-t14
      t19 = xi ** 2
      t23 = twoPi*(cc*xi+t19*cpwAlpha)
      t24 = sin(t23)
      t33 = 2.*xi*cpwAlpha+cc
      t35 = cos(t23)
      t41 = cpwAmp*(t5/2.-t12/2.)
      t46 = twoPi ** 2
      t47 = t33 ** 2
      chirp = cpwAmp*(t1*t12*t15-t1*t5*t8)*t24+2.*cpwAmp*(-cpwBeta*
     & t15/2.+cpwBeta*t8/2.)*twoPi*t33*t35+2.*t41*twoPi*cpwAlpha*t35-
     & t41*t46*t47*t24

                           else if(  numberOfTimeDerivatives.eq.3 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 3-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(-cpwBeta^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2+2*cpwBeta^3*tanh(cpwBeta*(xi+.5*cpwTau))^2*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2-2*cpwBeta^3*tanh(cpwBeta*(xi-.5*cpwTau))^2*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+3*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+6*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-3*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-6*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*cpwAlpha*(2*xi*cpwAlpha+cc)*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^3*(2*xi*cpwAlpha+cc)^3*cos(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = t1*cpwBeta
      t3 = .5*cpwTau
      t6 = tanh(cpwBeta*(xi+t3))
      t7 = t6 ** 2
      t8 = 1.-t7
      t9 = t8 ** 2
      t16 = tanh(cpwBeta*(xi-t3))
      t17 = t16 ** 2
      t18 = 1.-t17
      t19 = t18 ** 2
      t26 = xi ** 2
      t30 = twoPi*(cc*xi+t26*cpwAlpha)
      t31 = sin(t30)
      t41 = 2.*xi*cpwAlpha+cc
      t43 = cos(t30)
      t51 = cpwAmp*(-cpwBeta*t18/2.+cpwBeta*t8/2.)
      t56 = twoPi ** 2
      t57 = t41 ** 2
      t64 = cpwAmp*(t6/2.-t16/2.)
      chirp = cpwAmp*(-2.*t2*t17*t18+2.*t2*t7*t8+t2*t19-t2*t9)*t31+
     & 0.3E1*cpwAmp*(t1*t16*t18-t1*t6*t8)*twoPi*t41*t43+6.*t51*twoPi*
     & cpwAlpha*t43-0.3E1*t51*t56*t57*t31-6.*t64*t56*cpwAlpha*t41*t31-
     & t64*t56*twoPi*t57*t41*t43

                           else if(  numberOfTimeDerivatives.eq.4 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 4-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(8*cpwBeta^4*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2*tanh(cpwBeta*(xi+.5*cpwTau))-4*cpwBeta^4*tanh(cpwBeta*(xi+.5*cpwTau))^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-8*cpwBeta^4*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2*tanh(cpwBeta*(xi-.5*cpwTau))+4*cpwBeta^4*tanh(cpwBeta*(xi-.5*cpwTau))^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+4*cpwAmp*(-cpwBeta^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2+2*cpwBeta^3*tanh(cpwBeta*(xi+.5*cpwTau))^2*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2-2*cpwBeta^3*tanh(cpwBeta*(xi-.5*cpwTau))^2*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+12*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-6*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-24*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*cpwAlpha*(2*xi*cpwAlpha+cc)*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-4*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^3*(2*xi*cpwAlpha+cc)^3*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-12*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*cpwAlpha^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-12*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^3*cpwAlpha*(2*xi*cpwAlpha+cc)^2*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^4*(2*xi*cpwAlpha+cc)^4*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = t1 ** 2
      t3 = .5*cpwTau
      t6 = tanh(cpwBeta*(xi+t3))
      t7 = t6 ** 2
      t8 = 1.-t7
      t9 = t8 ** 2
      t19 = tanh(cpwBeta*(xi-t3))
      t20 = t19 ** 2
      t21 = 1.-t20
      t22 = t21 ** 2
      t32 = xi ** 2
      t36 = twoPi*(cc*xi+t32*cpwAlpha)
      t37 = sin(t36)
      t39 = t1*cpwBeta
      t52 = 2.*xi*cpwAlpha+cc
      t54 = cos(t36)
      t63 = cpwAmp*(t1*t19*t21-t1*t6*t8)
      t68 = twoPi ** 2
      t69 = t52 ** 2
      t78 = cpwAmp*(-cpwBeta*t21/2.+cpwBeta*t8/2.)
      t84 = t68*twoPi
      t92 = cpwAmp*(t6/2.-t19/2.)
      t93 = cpwAlpha ** 2
      t103 = t68 ** 2
      t104 = t69 ** 2
      chirp = cpwAmp*(4.*t2*t20*t19*t21-4.*t2*t7*t6*t8-8.*t2*t22*t19+
     & 8.*t2*t9*t6)*t37+4.*cpwAmp*(-2.*t39*t20*t21+2.*t39*t7*t8+t39*
     & t22-t39*t9)*twoPi*t52*t54+12.*t63*twoPi*cpwAlpha*t54-6.*t63*
     & t68*t69*t37-0.24E2*t78*t68*cpwAlpha*t52*t37-4.*t78*t84*t69*t52*
     & t54-12.*t92*t68*t93*t37-12.*t92*t84*cpwAlpha*t69*t54+t92*t103*
     & t104*t37

                           else
                             write(*,'(" getChirp2D:ERROR: too many 
     & derivatives requested")')
                             stop 4927
                           end if
                           ubv(ex) = chirp*pwc(0)
                           ubv(ey) = chirp*pwc(1)
                           ubv(hz) = chirp*pwc(5)
                        else
                          write(*,'("getBndryForcing2D: Unknown 
     & boundary forcing")')
                        end if
                      utm1 = -ubv(ex)
                      vtm1 = -ubv(ey)
                      numberOfTimeDerivatives=2+fieldOption
                        if( boundaryForcingOption.eq.noBoundaryForcing 
     & )then
                        else if( 
     & boundaryForcingOption.eq.planeWaveBoundaryForcing )then
                            if( numberOfTimeDerivatives==0 )then
                              ubv(ex) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==1 )then
                              ubv(ex) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(0)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(1)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(5)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==2 )then
                              ubv(ex) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==3 )then
                              ubv(ex) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==4 )then
                              ubv(ex) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))
                            else
                              stop 1738
                            end if
                        else if(  
     & boundaryForcingOption.eq.chirpedPlaneWaveBoundaryForcing )then
                           xi0 = .5*(cpwTa+cpwTb)
                           xi = t - (kx*(x0-cpwX0)+ky*(y0-cpwY0))/cc -
     & xi0
                           cpwTau=cpwTb-cpwTa  ! tau = tb -ta
                           ! include files generated by the maple code mx/codes/chirpedPlaneWave.maple 
                           if( numberOfTimeDerivatives.eq.0 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 0-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = .5*cpwTau
      t4 = tanh(cpwBeta*(xi+t1))
      t7 = tanh(cpwBeta*(xi-t1))
      t11 = xi ** 2
      t16 = sin(twoPi*(cc*xi+t11*cpwAlpha))
      chirp = cpwAmp*(t4/2.-t7/2.)*t16

                           else if(  numberOfTimeDerivatives.eq.1 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 1-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = .5*cpwTau
      t4 = tanh(cpwBeta*(xi+t1))
      t5 = t4 ** 2
      t10 = tanh(cpwBeta*(xi-t1))
      t11 = t10 ** 2
      t17 = xi ** 2
      t21 = twoPi*(cc*xi+t17*cpwAlpha)
      t22 = sin(t21)
      t31 = cos(t21)
      chirp = cpwAmp*(cpwBeta*(1.-t5)/2.-cpwBeta*(1.-t11)/2.)*t22+
     & cpwAmp*(t4/2.-t10/2.)*twoPi*(2.*xi*cpwAlpha+cc)*t31

                           else if(  numberOfTimeDerivatives.eq.2 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 2-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+2*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+2*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = .5*cpwTau
      t5 = tanh(cpwBeta*(xi+t2))
      t7 = t5 ** 2
      t8 = 1.-t7
      t12 = tanh(cpwBeta*(xi-t2))
      t14 = t12 ** 2
      t15 = 1.-t14
      t19 = xi ** 2
      t23 = twoPi*(cc*xi+t19*cpwAlpha)
      t24 = sin(t23)
      t33 = 2.*xi*cpwAlpha+cc
      t35 = cos(t23)
      t41 = cpwAmp*(t5/2.-t12/2.)
      t46 = twoPi ** 2
      t47 = t33 ** 2
      chirp = cpwAmp*(t1*t12*t15-t1*t5*t8)*t24+2.*cpwAmp*(-cpwBeta*
     & t15/2.+cpwBeta*t8/2.)*twoPi*t33*t35+2.*t41*twoPi*cpwAlpha*t35-
     & t41*t46*t47*t24

                           else if(  numberOfTimeDerivatives.eq.3 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 3-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(-cpwBeta^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2+2*cpwBeta^3*tanh(cpwBeta*(xi+.5*cpwTau))^2*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2-2*cpwBeta^3*tanh(cpwBeta*(xi-.5*cpwTau))^2*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+3*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+6*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-3*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-6*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*cpwAlpha*(2*xi*cpwAlpha+cc)*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^3*(2*xi*cpwAlpha+cc)^3*cos(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = t1*cpwBeta
      t3 = .5*cpwTau
      t6 = tanh(cpwBeta*(xi+t3))
      t7 = t6 ** 2
      t8 = 1.-t7
      t9 = t8 ** 2
      t16 = tanh(cpwBeta*(xi-t3))
      t17 = t16 ** 2
      t18 = 1.-t17
      t19 = t18 ** 2
      t26 = xi ** 2
      t30 = twoPi*(cc*xi+t26*cpwAlpha)
      t31 = sin(t30)
      t41 = 2.*xi*cpwAlpha+cc
      t43 = cos(t30)
      t51 = cpwAmp*(-cpwBeta*t18/2.+cpwBeta*t8/2.)
      t56 = twoPi ** 2
      t57 = t41 ** 2
      t64 = cpwAmp*(t6/2.-t16/2.)
      chirp = cpwAmp*(-2.*t2*t17*t18+2.*t2*t7*t8+t2*t19-t2*t9)*t31+
     & 0.3E1*cpwAmp*(t1*t16*t18-t1*t6*t8)*twoPi*t41*t43+6.*t51*twoPi*
     & cpwAlpha*t43-0.3E1*t51*t56*t57*t31-6.*t64*t56*cpwAlpha*t41*t31-
     & t64*t56*twoPi*t57*t41*t43

                           else if(  numberOfTimeDerivatives.eq.4 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 4-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(8*cpwBeta^4*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2*tanh(cpwBeta*(xi+.5*cpwTau))-4*cpwBeta^4*tanh(cpwBeta*(xi+.5*cpwTau))^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-8*cpwBeta^4*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2*tanh(cpwBeta*(xi-.5*cpwTau))+4*cpwBeta^4*tanh(cpwBeta*(xi-.5*cpwTau))^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+4*cpwAmp*(-cpwBeta^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2+2*cpwBeta^3*tanh(cpwBeta*(xi+.5*cpwTau))^2*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2-2*cpwBeta^3*tanh(cpwBeta*(xi-.5*cpwTau))^2*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+12*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-6*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-24*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*cpwAlpha*(2*xi*cpwAlpha+cc)*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-4*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^3*(2*xi*cpwAlpha+cc)^3*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-12*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*cpwAlpha^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-12*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^3*cpwAlpha*(2*xi*cpwAlpha+cc)^2*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^4*(2*xi*cpwAlpha+cc)^4*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = t1 ** 2
      t3 = .5*cpwTau
      t6 = tanh(cpwBeta*(xi+t3))
      t7 = t6 ** 2
      t8 = 1.-t7
      t9 = t8 ** 2
      t19 = tanh(cpwBeta*(xi-t3))
      t20 = t19 ** 2
      t21 = 1.-t20
      t22 = t21 ** 2
      t32 = xi ** 2
      t36 = twoPi*(cc*xi+t32*cpwAlpha)
      t37 = sin(t36)
      t39 = t1*cpwBeta
      t52 = 2.*xi*cpwAlpha+cc
      t54 = cos(t36)
      t63 = cpwAmp*(t1*t19*t21-t1*t6*t8)
      t68 = twoPi ** 2
      t69 = t52 ** 2
      t78 = cpwAmp*(-cpwBeta*t21/2.+cpwBeta*t8/2.)
      t84 = t68*twoPi
      t92 = cpwAmp*(t6/2.-t19/2.)
      t93 = cpwAlpha ** 2
      t103 = t68 ** 2
      t104 = t69 ** 2
      chirp = cpwAmp*(4.*t2*t20*t19*t21-4.*t2*t7*t6*t8-8.*t2*t22*t19+
     & 8.*t2*t9*t6)*t37+4.*cpwAmp*(-2.*t39*t20*t21+2.*t39*t7*t8+t39*
     & t22-t39*t9)*twoPi*t52*t54+12.*t63*twoPi*cpwAlpha*t54-6.*t63*
     & t68*t69*t37-0.24E2*t78*t68*cpwAlpha*t52*t37-4.*t78*t84*t69*t52*
     & t54-12.*t92*t68*t93*t37-12.*t92*t84*cpwAlpha*t69*t54+t92*t103*
     & t104*t37

                           else
                             write(*,'(" getChirp2D:ERROR: too many 
     & derivatives requested")')
                             stop 4927
                           end if
                           ubv(ex) = chirp*pwc(0)
                           ubv(ey) = chirp*pwc(1)
                           ubv(hz) = chirp*pwc(5)
                        else
                          write(*,'("getBndryForcing2D: Unknown 
     & boundary forcing")')
                        end if
                      uttm1 = -ubv(ex)
                      vttm1 = -ubv(ey)
                    else if( fieldOption.eq.0 )then ! *old way*
                      uttm1=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x0)+
     & ky*(y0)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(0)+ssftt*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(
     & t)))*pwc(0))
                      vttm1=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x0)+
     & ky*(y0)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(1)+ssftt*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(
     & t)))*pwc(1))
                      utm1 =-(ssf*(-twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(
     & y0)-cc*(t)))*pwc(0)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*
     & pwc(0))
                      vtm1 =-(ssf*(-twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(
     & y0)-cc*(t)))*pwc(1)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*
     & pwc(1))
                    else
                      ! we are assigning time derivatives (sosup)
                      uttm1=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x0)+ky*
     & (y0)-cc*(t)))*pwc(0))+3.*ssft*(-(twoPi*cc)**2*sin(twoPi*(kx*(
     & x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssftt*(-twoPi*cc)*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssfttt*sin(twoPi*(kx*(x0)+ky*(
     & y0)-cc*(t)))*pwc(0))
                      vttm1=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x0)+ky*
     & (y0)-cc*(t)))*pwc(1))+3.*ssft*(-(twoPi*cc)**2*sin(twoPi*(kx*(
     & x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssftt*(-twoPi*cc)*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssfttt*sin(twoPi*(kx*(x0)+ky*(
     & y0)-cc*(t)))*pwc(1))
                      utm1 =-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x0)+
     & ky*(y0)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(0)+ssftt*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(
     & t)))*pwc(0))
                      vtm1 =-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x0)+
     & ky*(y0)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(1)+ssftt*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(
     & t)))*pwc(1))
                    end if
                    x0=xy(i1+2*js1,i2+2*js2,i3,0)
                    y0=xy(i1+2*js1,i2+2*js2,i3,1)
                    if( .true. )then ! *new way*
                      numberOfTimeDerivatives=2+fieldOption
                        if( boundaryForcingOption.eq.noBoundaryForcing 
     & )then
                        else if( 
     & boundaryForcingOption.eq.planeWaveBoundaryForcing )then
                            if( numberOfTimeDerivatives==0 )then
                              ubv(ex) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==1 )then
                              ubv(ex) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(0)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(1)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(5)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==2 )then
                              ubv(ex) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==3 )then
                              ubv(ex) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==4 )then
                              ubv(ex) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))
                            else
                              stop 1738
                            end if
                        else if(  
     & boundaryForcingOption.eq.chirpedPlaneWaveBoundaryForcing )then
                           xi0 = .5*(cpwTa+cpwTb)
                           xi = t - (kx*(x0-cpwX0)+ky*(y0-cpwY0))/cc -
     & xi0
                           cpwTau=cpwTb-cpwTa  ! tau = tb -ta
                           ! include files generated by the maple code mx/codes/chirpedPlaneWave.maple 
                           if( numberOfTimeDerivatives.eq.0 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 0-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = .5*cpwTau
      t4 = tanh(cpwBeta*(xi+t1))
      t7 = tanh(cpwBeta*(xi-t1))
      t11 = xi ** 2
      t16 = sin(twoPi*(cc*xi+t11*cpwAlpha))
      chirp = cpwAmp*(t4/2.-t7/2.)*t16

                           else if(  numberOfTimeDerivatives.eq.1 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 1-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = .5*cpwTau
      t4 = tanh(cpwBeta*(xi+t1))
      t5 = t4 ** 2
      t10 = tanh(cpwBeta*(xi-t1))
      t11 = t10 ** 2
      t17 = xi ** 2
      t21 = twoPi*(cc*xi+t17*cpwAlpha)
      t22 = sin(t21)
      t31 = cos(t21)
      chirp = cpwAmp*(cpwBeta*(1.-t5)/2.-cpwBeta*(1.-t11)/2.)*t22+
     & cpwAmp*(t4/2.-t10/2.)*twoPi*(2.*xi*cpwAlpha+cc)*t31

                           else if(  numberOfTimeDerivatives.eq.2 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 2-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+2*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+2*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = .5*cpwTau
      t5 = tanh(cpwBeta*(xi+t2))
      t7 = t5 ** 2
      t8 = 1.-t7
      t12 = tanh(cpwBeta*(xi-t2))
      t14 = t12 ** 2
      t15 = 1.-t14
      t19 = xi ** 2
      t23 = twoPi*(cc*xi+t19*cpwAlpha)
      t24 = sin(t23)
      t33 = 2.*xi*cpwAlpha+cc
      t35 = cos(t23)
      t41 = cpwAmp*(t5/2.-t12/2.)
      t46 = twoPi ** 2
      t47 = t33 ** 2
      chirp = cpwAmp*(t1*t12*t15-t1*t5*t8)*t24+2.*cpwAmp*(-cpwBeta*
     & t15/2.+cpwBeta*t8/2.)*twoPi*t33*t35+2.*t41*twoPi*cpwAlpha*t35-
     & t41*t46*t47*t24

                           else if(  numberOfTimeDerivatives.eq.3 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 3-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(-cpwBeta^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2+2*cpwBeta^3*tanh(cpwBeta*(xi+.5*cpwTau))^2*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2-2*cpwBeta^3*tanh(cpwBeta*(xi-.5*cpwTau))^2*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+3*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+6*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-3*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-6*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*cpwAlpha*(2*xi*cpwAlpha+cc)*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^3*(2*xi*cpwAlpha+cc)^3*cos(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = t1*cpwBeta
      t3 = .5*cpwTau
      t6 = tanh(cpwBeta*(xi+t3))
      t7 = t6 ** 2
      t8 = 1.-t7
      t9 = t8 ** 2
      t16 = tanh(cpwBeta*(xi-t3))
      t17 = t16 ** 2
      t18 = 1.-t17
      t19 = t18 ** 2
      t26 = xi ** 2
      t30 = twoPi*(cc*xi+t26*cpwAlpha)
      t31 = sin(t30)
      t41 = 2.*xi*cpwAlpha+cc
      t43 = cos(t30)
      t51 = cpwAmp*(-cpwBeta*t18/2.+cpwBeta*t8/2.)
      t56 = twoPi ** 2
      t57 = t41 ** 2
      t64 = cpwAmp*(t6/2.-t16/2.)
      chirp = cpwAmp*(-2.*t2*t17*t18+2.*t2*t7*t8+t2*t19-t2*t9)*t31+
     & 0.3E1*cpwAmp*(t1*t16*t18-t1*t6*t8)*twoPi*t41*t43+6.*t51*twoPi*
     & cpwAlpha*t43-0.3E1*t51*t56*t57*t31-6.*t64*t56*cpwAlpha*t41*t31-
     & t64*t56*twoPi*t57*t41*t43

                           else if(  numberOfTimeDerivatives.eq.4 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 4-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(8*cpwBeta^4*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2*tanh(cpwBeta*(xi+.5*cpwTau))-4*cpwBeta^4*tanh(cpwBeta*(xi+.5*cpwTau))^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-8*cpwBeta^4*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2*tanh(cpwBeta*(xi-.5*cpwTau))+4*cpwBeta^4*tanh(cpwBeta*(xi-.5*cpwTau))^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+4*cpwAmp*(-cpwBeta^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2+2*cpwBeta^3*tanh(cpwBeta*(xi+.5*cpwTau))^2*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2-2*cpwBeta^3*tanh(cpwBeta*(xi-.5*cpwTau))^2*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+12*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-6*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-24*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*cpwAlpha*(2*xi*cpwAlpha+cc)*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-4*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^3*(2*xi*cpwAlpha+cc)^3*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-12*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*cpwAlpha^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-12*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^3*cpwAlpha*(2*xi*cpwAlpha+cc)^2*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^4*(2*xi*cpwAlpha+cc)^4*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = t1 ** 2
      t3 = .5*cpwTau
      t6 = tanh(cpwBeta*(xi+t3))
      t7 = t6 ** 2
      t8 = 1.-t7
      t9 = t8 ** 2
      t19 = tanh(cpwBeta*(xi-t3))
      t20 = t19 ** 2
      t21 = 1.-t20
      t22 = t21 ** 2
      t32 = xi ** 2
      t36 = twoPi*(cc*xi+t32*cpwAlpha)
      t37 = sin(t36)
      t39 = t1*cpwBeta
      t52 = 2.*xi*cpwAlpha+cc
      t54 = cos(t36)
      t63 = cpwAmp*(t1*t19*t21-t1*t6*t8)
      t68 = twoPi ** 2
      t69 = t52 ** 2
      t78 = cpwAmp*(-cpwBeta*t21/2.+cpwBeta*t8/2.)
      t84 = t68*twoPi
      t92 = cpwAmp*(t6/2.-t19/2.)
      t93 = cpwAlpha ** 2
      t103 = t68 ** 2
      t104 = t69 ** 2
      chirp = cpwAmp*(4.*t2*t20*t19*t21-4.*t2*t7*t6*t8-8.*t2*t22*t19+
     & 8.*t2*t9*t6)*t37+4.*cpwAmp*(-2.*t39*t20*t21+2.*t39*t7*t8+t39*
     & t22-t39*t9)*twoPi*t52*t54+12.*t63*twoPi*cpwAlpha*t54-6.*t63*
     & t68*t69*t37-0.24E2*t78*t68*cpwAlpha*t52*t37-4.*t78*t84*t69*t52*
     & t54-12.*t92*t68*t93*t37-12.*t92*t84*cpwAlpha*t69*t54+t92*t103*
     & t104*t37

                           else
                             write(*,'(" getChirp2D:ERROR: too many 
     & derivatives requested")')
                             stop 4927
                           end if
                           ubv(ex) = chirp*pwc(0)
                           ubv(ey) = chirp*pwc(1)
                           ubv(hz) = chirp*pwc(5)
                        else
                          write(*,'("getBndryForcing2D: Unknown 
     & boundary forcing")')
                        end if
                      uttp2 = -ubv(ex)
                      vttp2 = -ubv(ey)
                    else if( fieldOption.eq.0 )then ! *old way*
                      uttp2=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x0)+
     & ky*(y0)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(0)+ssftt*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(
     & t)))*pwc(0))
                      vttp2=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x0)+
     & ky*(y0)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(1)+ssftt*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(
     & t)))*pwc(1))
                    else
                      ! we are assigning time derivatives (sosup)
                      uttp2=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x0)+ky*
     & (y0)-cc*(t)))*pwc(0))+3.*ssft*(-(twoPi*cc)**2*sin(twoPi*(kx*(
     & x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssftt*(-twoPi*cc)*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssfttt*sin(twoPi*(kx*(x0)+ky*(
     & y0)-cc*(t)))*pwc(0))
                      vttp2=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x0)+ky*
     & (y0)-cc*(t)))*pwc(1))+3.*ssft*(-(twoPi*cc)**2*sin(twoPi*(kx*(
     & x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssftt*(-twoPi*cc)*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssfttt*sin(twoPi*(kx*(x0)+ky*(
     & y0)-cc*(t)))*pwc(1))
                    end if
                    x0=xy(i1-2*js1,i2-2*js2,i3,0)
                    y0=xy(i1-2*js1,i2-2*js2,i3,1)
                    if( .true. )then ! *new way*
                      numberOfTimeDerivatives=2+fieldOption
                        if( boundaryForcingOption.eq.noBoundaryForcing 
     & )then
                        else if( 
     & boundaryForcingOption.eq.planeWaveBoundaryForcing )then
                            if( numberOfTimeDerivatives==0 )then
                              ubv(ex) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*sin(twoPi*(kx*(x0)+ky*(y0)
     & -cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==1 )then
                              ubv(ex) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(0)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(1)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*(-twoPi*cc)*cos(twoPi*(kx*
     & (x0)+ky*(y0)-cc*(t)))*pwc(5)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==2 )then
                              ubv(ex) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*(-(twoPi*cc)**2*sin(twoPi*
     & (kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+2.*ssft*(-twoPi*cc)*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssftt*sin(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==3 )then
                              ubv(ex) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*((twoPi*cc)**3*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+3.*ssft*(-(twoPi*cc)**2*sin(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+3.*ssftt*(-twoPi*cc)*
     & cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssfttt*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))
                            else if( numberOfTimeDerivatives==4 )then
                              ubv(ex) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0))
                              ubv(ey) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1))
                              ubv(hz) = (ssf*((twoPi*cc)**4*sin(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+4.*ssft*((twoPi*cc)**3*cos(
     & twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+6.*ssftt*(-(twoPi*cc)**
     & 2*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))+4.*ssfttt*(-
     & twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5)+ssftttt*
     & sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(5))
                            else
                              stop 1738
                            end if
                        else if(  
     & boundaryForcingOption.eq.chirpedPlaneWaveBoundaryForcing )then
                           xi0 = .5*(cpwTa+cpwTb)
                           xi = t - (kx*(x0-cpwX0)+ky*(y0-cpwY0))/cc -
     & xi0
                           cpwTau=cpwTb-cpwTa  ! tau = tb -ta
                           ! include files generated by the maple code mx/codes/chirpedPlaneWave.maple 
                           if( numberOfTimeDerivatives.eq.0 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 0-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = .5*cpwTau
      t4 = tanh(cpwBeta*(xi+t1))
      t7 = tanh(cpwBeta*(xi-t1))
      t11 = xi ** 2
      t16 = sin(twoPi*(cc*xi+t11*cpwAlpha))
      chirp = cpwAmp*(t4/2.-t7/2.)*t16

                           else if(  numberOfTimeDerivatives.eq.1 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 1-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = .5*cpwTau
      t4 = tanh(cpwBeta*(xi+t1))
      t5 = t4 ** 2
      t10 = tanh(cpwBeta*(xi-t1))
      t11 = t10 ** 2
      t17 = xi ** 2
      t21 = twoPi*(cc*xi+t17*cpwAlpha)
      t22 = sin(t21)
      t31 = cos(t21)
      chirp = cpwAmp*(cpwBeta*(1.-t5)/2.-cpwBeta*(1.-t11)/2.)*t22+
     & cpwAmp*(t4/2.-t10/2.)*twoPi*(2.*xi*cpwAlpha+cc)*t31

                           else if(  numberOfTimeDerivatives.eq.2 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 2-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+2*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+2*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = .5*cpwTau
      t5 = tanh(cpwBeta*(xi+t2))
      t7 = t5 ** 2
      t8 = 1.-t7
      t12 = tanh(cpwBeta*(xi-t2))
      t14 = t12 ** 2
      t15 = 1.-t14
      t19 = xi ** 2
      t23 = twoPi*(cc*xi+t19*cpwAlpha)
      t24 = sin(t23)
      t33 = 2.*xi*cpwAlpha+cc
      t35 = cos(t23)
      t41 = cpwAmp*(t5/2.-t12/2.)
      t46 = twoPi ** 2
      t47 = t33 ** 2
      chirp = cpwAmp*(t1*t12*t15-t1*t5*t8)*t24+2.*cpwAmp*(-cpwBeta*
     & t15/2.+cpwBeta*t8/2.)*twoPi*t33*t35+2.*t41*twoPi*cpwAlpha*t35-
     & t41*t46*t47*t24

                           else if(  numberOfTimeDerivatives.eq.3 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 3-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(-cpwBeta^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2+2*cpwBeta^3*tanh(cpwBeta*(xi+.5*cpwTau))^2*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2-2*cpwBeta^3*tanh(cpwBeta*(xi-.5*cpwTau))^2*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+3*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+6*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-3*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-6*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*cpwAlpha*(2*xi*cpwAlpha+cc)*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^3*(2*xi*cpwAlpha+cc)^3*cos(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = t1*cpwBeta
      t3 = .5*cpwTau
      t6 = tanh(cpwBeta*(xi+t3))
      t7 = t6 ** 2
      t8 = 1.-t7
      t9 = t8 ** 2
      t16 = tanh(cpwBeta*(xi-t3))
      t17 = t16 ** 2
      t18 = 1.-t17
      t19 = t18 ** 2
      t26 = xi ** 2
      t30 = twoPi*(cc*xi+t26*cpwAlpha)
      t31 = sin(t30)
      t41 = 2.*xi*cpwAlpha+cc
      t43 = cos(t30)
      t51 = cpwAmp*(-cpwBeta*t18/2.+cpwBeta*t8/2.)
      t56 = twoPi ** 2
      t57 = t41 ** 2
      t64 = cpwAmp*(t6/2.-t16/2.)
      chirp = cpwAmp*(-2.*t2*t17*t18+2.*t2*t7*t8+t2*t19-t2*t9)*t31+
     & 0.3E1*cpwAmp*(t1*t16*t18-t1*t6*t8)*twoPi*t41*t43+6.*t51*twoPi*
     & cpwAlpha*t43-0.3E1*t51*t56*t57*t31-6.*t64*t56*cpwAlpha*t41*t31-
     & t64*t56*twoPi*t57*t41*t43

                           else if(  numberOfTimeDerivatives.eq.4 )then
! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 4-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(8*cpwBeta^4*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2*tanh(cpwBeta*(xi+.5*cpwTau))-4*cpwBeta^4*tanh(cpwBeta*(xi+.5*cpwTau))^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-8*cpwBeta^4*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2*tanh(cpwBeta*(xi-.5*cpwTau))+4*cpwBeta^4*tanh(cpwBeta*(xi-.5*cpwTau))^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+4*cpwAmp*(-cpwBeta^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2+2*cpwBeta^3*tanh(cpwBeta*(xi+.5*cpwTau))^2*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2-2*cpwBeta^3*tanh(cpwBeta*(xi-.5*cpwTau))^2*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+12*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-6*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-24*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*cpwAlpha*(2*xi*cpwAlpha+cc)*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-4*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^3*(2*xi*cpwAlpha+cc)^3*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-12*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*cpwAlpha^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-12*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^3*cpwAlpha*(2*xi*cpwAlpha+cc)^2*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^4*(2*xi*cpwAlpha+cc)^4*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = t1 ** 2
      t3 = .5*cpwTau
      t6 = tanh(cpwBeta*(xi+t3))
      t7 = t6 ** 2
      t8 = 1.-t7
      t9 = t8 ** 2
      t19 = tanh(cpwBeta*(xi-t3))
      t20 = t19 ** 2
      t21 = 1.-t20
      t22 = t21 ** 2
      t32 = xi ** 2
      t36 = twoPi*(cc*xi+t32*cpwAlpha)
      t37 = sin(t36)
      t39 = t1*cpwBeta
      t52 = 2.*xi*cpwAlpha+cc
      t54 = cos(t36)
      t63 = cpwAmp*(t1*t19*t21-t1*t6*t8)
      t68 = twoPi ** 2
      t69 = t52 ** 2
      t78 = cpwAmp*(-cpwBeta*t21/2.+cpwBeta*t8/2.)
      t84 = t68*twoPi
      t92 = cpwAmp*(t6/2.-t19/2.)
      t93 = cpwAlpha ** 2
      t103 = t68 ** 2
      t104 = t69 ** 2
      chirp = cpwAmp*(4.*t2*t20*t19*t21-4.*t2*t7*t6*t8-8.*t2*t22*t19+
     & 8.*t2*t9*t6)*t37+4.*cpwAmp*(-2.*t39*t20*t21+2.*t39*t7*t8+t39*
     & t22-t39*t9)*twoPi*t52*t54+12.*t63*twoPi*cpwAlpha*t54-6.*t63*
     & t68*t69*t37-0.24E2*t78*t68*cpwAlpha*t52*t37-4.*t78*t84*t69*t52*
     & t54-12.*t92*t68*t93*t37-12.*t92*t84*cpwAlpha*t69*t54+t92*t103*
     & t104*t37

                           else
                             write(*,'(" getChirp2D:ERROR: too many 
     & derivatives requested")')
                             stop 4927
                           end if
                           ubv(ex) = chirp*pwc(0)
                           ubv(ey) = chirp*pwc(1)
                           ubv(hz) = chirp*pwc(5)
                        else
                          write(*,'("getBndryForcing2D: Unknown 
     & boundary forcing")')
                        end if
                      uttm2 = -ubv(ex)
                      vttm2 = -ubv(ey)
                    else if( fieldOption.eq.0 )then ! *old way*
                      uttm2=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x0)+
     & ky*(y0)-cc*(t)))*pwc(0))+2.*ssft*(-twoPi*cc)*cos(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(0)+ssftt*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(
     & t)))*pwc(0))
                      vttm2=-(ssf*(-(twoPi*cc)**2*sin(twoPi*(kx*(x0)+
     & ky*(y0)-cc*(t)))*pwc(1))+2.*ssft*(-twoPi*cc)*cos(twoPi*(kx*(x0)
     & +ky*(y0)-cc*(t)))*pwc(1)+ssftt*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(
     & t)))*pwc(1))
                    else
                      ! we are assigning time derivatives (sosup)
                      uttm2=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x0)+ky*
     & (y0)-cc*(t)))*pwc(0))+3.*ssft*(-(twoPi*cc)**2*sin(twoPi*(kx*(
     & x0)+ky*(y0)-cc*(t)))*pwc(0))+3.*ssftt*(-twoPi*cc)*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)+ssfttt*sin(twoPi*(kx*(x0)+ky*(
     & y0)-cc*(t)))*pwc(0))
                      vttm2=-(ssf*((twoPi*cc)**3*cos(twoPi*(kx*(x0)+ky*
     & (y0)-cc*(t)))*pwc(1))+3.*ssft*(-(twoPi*cc)**2*sin(twoPi*(kx*(
     & x0)+ky*(y0)-cc*(t)))*pwc(1))+3.*ssftt*(-twoPi*cc)*cos(twoPi*(
     & kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)+ssfttt*sin(twoPi*(kx*(x0)+ky*(
     & y0)-cc*(t)))*pwc(1))
                    end if
                    utts = (8.*(uttp1-uttm1)-(uttp2-uttm2) )/(12.*dsa)
                    vtts = (8.*(vttp1-vttm1)-(vttp2-vttm2) )/(12.*dsa)
                    bf = bf + a21s*utt00+a22s*vtt00 + a21*utts + a22*
     & vtts
                    ! ***** Forcing for Hz ******
                    ! (w).r = fw1                              (w.n = 0 )
                    ! (c11*w.rr + c22*w.ss + ... ).r = fw2     ( (Delta w).n = 0 )
                    ! *** for planeWaveBoundaryForcing we need to use: u.t=w.y and v.t=-w.x =>
                    ! *****  (n1,n2).(w.x,w.y) = -n1*v.t + n2*u.t
                    !  OR    (rx,ry).(w.x,w.y) = -rx*v.t + ry*u.t
                    !   (rx**2+ry**2) w.r + (rx*sx+ry*sy)*ws = -rx*vt + ry*ut 
                    ! Note: the first term here (rx*sx+ry*sy) will be zero on an orthogonal grid
                     fw1=(-(rsxy(i1,i2,i3,axis,0)*rsxy(i1,i2,i3,axisp1,
     & 0)+rsxy(i1,i2,i3,axis,1)*rsxy(i1,i2,i3,axisp1,1))*ws-rsxy(i1,
     & i2,i3,axis,0)*vt0+rsxy(i1,i2,i3,axis,1)*ut0)/(rsxy(i1,i2,i3,
     & axis,0)**2+rsxy(i1,i2,i3,axis,1)**2)
                 !$$$   fw1=( -(rsxy(i1,i2,i3,axis,0)*rsxy(i1,i2,i3,axisp1,0)+rsxy(i1,i2,i3,axis,1)*rsxy(i1,i2,i3,axisp1,1))*ws !$$$        - rsxy(i1,i2,i3,axis,0)*vt0 + rsxy(i1,i2,i3,axis,1)*ut0 !$$$       )/(rsxy(i1,i2,i3,axis,0)**2+rsxy(i1,i2,i3,axis,1)**2)
                    ! fw2 = fw1.tt -[ c22*wrss + c2 wrs ]
                    ! where
                    !     w.r = fw1 = (-rx*vt + ry*ut - (rx*sx+ry*sy)*ws )/(rx**2+ry**2) 
                    ! Compute wrs and wrss by differencing fw1
                    wsm1 = (u(i1,i2,i3,hz)-u(i1-2*js1,i2-2*js2,i3,hz))
     & /(2.*dsa)    ! ws(i1-js1,i2-js2,i3)
                    wsp1 = (u(i1+2*js1,i2+2*js2,i3,hz)-u(i1,i2,i3,hz))
     & /(2.*dsa)    ! ws(i1+js1,i2+js2,i3)
                    fw1m1=(-(rsxy(i1-js1,i2-js2,i3,axis,0)*rsxy(i1-js1,
     & i2-js2,i3,axisp1,0)+rsxy(i1-js1,i2-js2,i3,axis,1)*rsxy(i1-js1,
     & i2-js2,i3,axisp1,1))*wsm1-rsxy(i1-js1,i2-js2,i3,axis,0)*vtm1+
     & rsxy(i1-js1,i2-js2,i3,axis,1)*utm1)/(rsxy(i1-js1,i2-js2,i3,
     & axis,0)**2+rsxy(i1-js1,i2-js2,i3,axis,1)**2)
                    fw1p1=(-(rsxy(i1+js1,i2+js2,i3,axis,0)*rsxy(i1+js1,
     & i2+js2,i3,axisp1,0)+rsxy(i1+js1,i2+js2,i3,axis,1)*rsxy(i1+js1,
     & i2+js2,i3,axisp1,1))*wsp1-rsxy(i1+js1,i2+js2,i3,axis,0)*vtp1+
     & rsxy(i1+js1,i2+js2,i3,axis,1)*utp1)/(rsxy(i1+js1,i2+js2,i3,
     & axis,0)**2+rsxy(i1+js1,i2+js2,i3,axis,1)**2)
                    ! NOTE: the term involving wtts is left off -- the coeff is zero for orthogonal grids
                    fw2 = (-rsxy(i1,i2,i3,axis,0)*vttt0 + rsxy(i1,i2,
     & i3,axis,1)*uttt0 )/(rsxy(i1,i2,i3,axis,0)**2+rsxy(i1,i2,i3,
     & axis,1)**2)- c22*( fw1p1-2.*fw1+fw1m1 )/(dsa**2) -c2*(fw1p1-
     & fw1m1 )/(2.*dsa)
                 !   fw2 = (-rsxy(i1,i2,i3,axis,0)*vttt0 + rsxy(i1,i2,i3,axis,1)*uttt0 )/!                               (rsxy(i1,i2,i3,axis,0)**2+rsxy(i1,i2,i3,axis,1)**2)!         - c22*( (-rsxy(i1+js1,i2+js2,i3,axis,0)*vtp1 + rsxy(i1+js1,i2+js2,i3,axis,1)*utp1)!             -2.*(-rsxy(i1    ,i2    ,i3,axis,0)*vt0  + rsxy(i1    ,i2    ,i3,axis,1)*ut0 ) !                +(-rsxy(i1-js1,i2-js2,i3,axis,0)*vtm1 + rsxy(i1-js1,i2-js2,i3,axis,1)*utm1) )/(dsa**2) !         -  c2*( (-rsxy(i1+js1,i2+js2,i3,axis,0)*vtp1 + rsxy(i1+js1,i2+js2,i3,axis,1)*utp1)!                -(-rsxy(i1-js1,i2-js2,i3,axis,0)*vtm1 + rsxy(i1-js1,i2-js2,i3,axis,1)*utm1) )/(2.*dsa)
                  end if
                   ! ********** For now do this: should work for quadratics *******************
                   cgI=0.
                   gIf=0.
                   ! ***********************************************
                    ! For TZ: utt0 = utt - ett + Lap(e)
                    call ogDeriv(ep, 0, 2,0,0, xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.,t,ex, uxx)
                    call ogDeriv(ep, 0, 0,2,0, xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.,t,ex, uyy)
                    utt00=uxx+uyy
                    call ogDeriv(ep, 0, 2,0,0, xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.,t,ey, vxx)
                    call ogDeriv(ep, 0, 0,2,0, xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.,t,ey, vyy)
                    vtt00=vxx+vyy
                   tau1DotUtt = tau1*utt00+tau2*vtt00
                   call ogDeriv(ep, 0, 2,0,0, xy(i1-is1,i2-is2,i3,0),
     & xy(i1-is1,i2-is2,i3,1),0.,t,ex, uxxm1)
                   call ogDeriv(ep, 0, 0,2,0, xy(i1-is1,i2-is2,i3,0),
     & xy(i1-is1,i2-is2,i3,1),0.,t,ex, uyym1)
                   call ogDeriv(ep, 0, 2,0,0, xy(i1+is1,i2+is2,i3,0),
     & xy(i1+is1,i2+is2,i3,1),0.,t,ex, uxxp1)
                   call ogDeriv(ep, 0, 0,2,0, xy(i1+is1,i2+is2,i3,0),
     & xy(i1+is1,i2+is2,i3,1),0.,t,ex, uyyp1)
                   call ogDeriv(ep, 0, 2,0,0, xy(i1-is1,i2-is2,i3,0),
     & xy(i1-is1,i2-is2,i3,1),0.,t,ey, vxxm1)
                   call ogDeriv(ep, 0, 0,2,0, xy(i1-is1,i2-is2,i3,0),
     & xy(i1-is1,i2-is2,i3,1),0.,t,ey, vyym1)
                   call ogDeriv(ep, 0, 2,0,0, xy(i1+is1,i2+is2,i3,0),
     & xy(i1+is1,i2+is2,i3,1),0.,t,ey, vxxp1)
                   call ogDeriv(ep, 0, 0,2,0, xy(i1+is1,i2+is2,i3,0),
     & xy(i1+is1,i2+is2,i3,1),0.,t,ey, vyyp1)
                   call ogDeriv(ep, 0, 2,0,0, xy(i1-2*is1,i2-2*is2,i3,
     & 0),xy(i1-2*is1,i2-2*is2,i3,1),0.,t,ex, uxxm2)
                   call ogDeriv(ep, 0, 0,2,0, xy(i1-2*is1,i2-2*is2,i3,
     & 0),xy(i1-2*is1,i2-2*is2,i3,1),0.,t,ex, uyym2)
                   call ogDeriv(ep, 0, 2,0,0, xy(i1+2*is1,i2+2*is2,i3,
     & 0),xy(i1+2*is1,i2+2*is2,i3,1),0.,t,ex, uxxp2)
                   call ogDeriv(ep, 0, 0,2,0, xy(i1+2*is1,i2+2*is2,i3,
     & 0),xy(i1+2*is1,i2+2*is2,i3,1),0.,t,ex, uyyp2)
                   call ogDeriv(ep, 0, 2,0,0, xy(i1-2*is1,i2-2*is2,i3,
     & 0),xy(i1-2*is1,i2-2*is2,i3,1),0.,t,ey, vxxm2)
                   call ogDeriv(ep, 0, 0,2,0, xy(i1-2*is1,i2-2*is2,i3,
     & 0),xy(i1-2*is1,i2-2*is2,i3,1),0.,t,ey, vyym2)
                   call ogDeriv(ep, 0, 2,0,0, xy(i1+2*is1,i2+2*is2,i3,
     & 0),xy(i1+2*is1,i2+2*is2,i3,1),0.,t,ey, vxxp2)
                   call ogDeriv(ep, 0, 0,2,0, xy(i1+2*is1,i2+2*is2,i3,
     & 0),xy(i1+2*is1,i2+2*is2,i3,1),0.,t,ey, vyyp2)
                   ! For TZ choose bf = bf - Dr( a1.Delta uvExact )
                   bf = bf - a11r*utt00 - a12r*vtt00 -a11*( 8.*((uxxp1+
     & uyyp1)-(uxxm1+uyym1))-((uxxp2+uyyp2)-(uxxm2+uyym2)) )/(12.*dra)
     &  -a12*( 8.*((vxxp1+vyyp1)-(vxxm1+vyym1))-((vxxp2+vyyp2)-(vxxm2+
     & vyym2)) )/(12.*dra)
                   ! write(*,'("  bc4:i1,i2=",2i3,"  b3u,b3v,b2u,b2v,b1u,b1v,bf,a11r,a12r=",9e12.4)') i1,i2,b3u,b3v,b2u,b2v,b1u,b1v,bf,a11r,a12r
                    call ogf2d(ep,xy(i1,i2,i3,0),xy(i1,i2,i3,1),t,uv0(
     & 0),uv0(1),uv0(2))
                    call ogf2d(ep,xy(i1-is1,i2-is2,i3,0),xy(i1-is1,i2-
     & is2,i3,1),t,uvm(0),uvm(1),uvm(2))
                    call ogf2d(ep,xy(i1+is1,i2+is2,i3,0),xy(i1+is1,i2+
     & is2,i3,1),t,uvp(0),uvp(1),uvp(2))
                    call ogf2d(ep,xy(i1-2*is1,i2-2*is2,i3,0),xy(i1-2*
     & is1,i2-2*is2,i3,1),t,uvm2(0),uvm2(1),uvm2(2))
                    call ogf2d(ep,xy(i1+2*is1,i2+2*is2,i3,0),xy(i1+2*
     & is1,i2+2*is2,i3,1),t,uvp2(0),uvp2(1),uvp2(2))
                   ! Da1DotU = (a1.uv).r to 4th order
                   Da1DotU = (8.*( (a11p1*uvp(0)+a12p1*uvp(1)) - (
     & a11m1*uvm(0)+a12m1*uvm(1)) )- ( (a11p2*uvp2(0)+a12p2*uvp2(1)) -
     &  (a11m2*uvm2(0)+a12m2*uvm2(1)) ) )/(12.*dra)
                   ! For testing: *************************************************************
                 !$$$  urrr=(uvp2(0)-2.*(uvp(0)-uvm(0))-uvm2(0))/(2.*dra**3)
                 !$$$  vrrr=(uvp2(1)-2.*(uvp(1)-uvm(1))-uvm2(1))/(2.*dra**3)
                 !$$$
                 !$$$  urr=(-30.*uv0(0)+16.*(uvp(0)+uvm(0))-(uvp2(0)+uvm2(0)) )/(12.*dra**2)
                 !$$$  vrr=(-30.*uv0(1)+16.*(uvp(1)+uvm(1))-(uvp2(1)+uvm2(1)) )/(12.*dra**2)
                 !$$$
                 !$$$  ur=(8.*(uvp(0)-uvm(0))-(uvp2(0)-uvm2(0)))/(12.*dra)
                 !$$$  vr=(8.*(uvp(1)-uvm(1))-(uvp2(1)-uvm2(1)))/(12.*dra)
                 !$$$
                 !$$$  bf = -( b3u*urrr+b3v*vrrr+b2u*urr+b2v*vrr+b1u*ur+b1v*vr )
                   ! *************************************************************************
                  ! for now remove the error in the extrapolation ************
                  ! gIVf = tau1*( uvm2(0)-4.*uvm(0)+6.*uv0(0)-4.*uvp(0)+uvp2(0)) +!        tau2*( uvm2(1)-4.*uvm(1)+6.*uv0(1)-4.*uvp(1)+uvp2(1))
                  gIVf=0.
                   a21zp1= (rsxy(i1+js1,i2+js2,i3,axisp1,0)/(rx(i1+js1,
     & i2+js2,i3)*sy(i1+js1,i2+js2,i3)-ry(i1+js1,i2+js2,i3)*sx(i1+js1,
     & i2+js2,i3)))
                   a21zm1= (rsxy(i1-js1,i2-js2,i3,axisp1,0)/(rx(i1-js1,
     & i2-js2,i3)*sy(i1-js1,i2-js2,i3)-ry(i1-js1,i2-js2,i3)*sx(i1-js1,
     & i2-js2,i3)))
                   a21zp2= (rsxy(i1+2*js1,i2+2*js2,i3,axisp1,0)/(rx(i1+
     & 2*js1,i2+2*js2,i3)*sy(i1+2*js1,i2+2*js2,i3)-ry(i1+2*js1,i2+2*
     & js2,i3)*sx(i1+2*js1,i2+2*js2,i3)))
                   a21zm2= (rsxy(i1-2*js1,i2-2*js2,i3,axisp1,0)/(rx(i1-
     & 2*js1,i2-2*js2,i3)*sy(i1-2*js1,i2-2*js2,i3)-ry(i1-2*js1,i2-2*
     & js2,i3)*sx(i1-2*js1,i2-2*js2,i3)))
                   a22zp1= (rsxy(i1+js1,i2+js2,i3,axisp1,1)/(rx(i1+js1,
     & i2+js2,i3)*sy(i1+js1,i2+js2,i3)-ry(i1+js1,i2+js2,i3)*sx(i1+js1,
     & i2+js2,i3)))
                   a22zm1= (rsxy(i1-js1,i2-js2,i3,axisp1,1)/(rx(i1-js1,
     & i2-js2,i3)*sy(i1-js1,i2-js2,i3)-ry(i1-js1,i2-js2,i3)*sx(i1-js1,
     & i2-js2,i3)))
                   a22zp2= (rsxy(i1+2*js1,i2+2*js2,i3,axisp1,1)/(rx(i1+
     & 2*js1,i2+2*js2,i3)*sy(i1+2*js1,i2+2*js2,i3)-ry(i1+2*js1,i2+2*
     & js2,i3)*sx(i1+2*js1,i2+2*js2,i3)))
                   a22zm2= (rsxy(i1-2*js1,i2-2*js2,i3,axisp1,1)/(rx(i1-
     & 2*js1,i2-2*js2,i3)*sy(i1-2*js1,i2-2*js2,i3)-ry(i1-2*js1,i2-2*
     & js2,i3)*sx(i1-2*js1,i2-2*js2,i3)))
                   ! *** set to - Ds( a2.uv )
                   Da1DotU = -(  ( 8.*(a21zp1*u(i1+  js1,i2+  js2,i3,
     & ex)-a21zm1*u(i1-  js1,i2-  js2,i3,ex)) -(a21zp2*u(i1+2*js1,i2+
     & 2*js2,i3,ex)-a21zm2*u(i1-2*js1,i2-2*js2,i3,ex)) )/(12.*dsa) +( 
     & 8.*(a22zp1*u(i1+  js1,i2+  js2,i3,ey)-a22zm1*u(i1-  js1,i2-  
     & js2,i3,ey)) -(a22zp2*u(i1+2*js1,i2+2*js2,i3,ey)-a22zm2*u(i1-2*
     & js1,i2-2*js2,i3,ey)) )/(12.*dsa)  )
                  ! ***** Forcing for Hz ******
                  ! (w).r = fw1                              (w.n = 0 )
                  ! (c11*w.rr + c22*w.ss + ... ).r = fw2     ( (Delta w).n = 0 )
                  ! u(i1-is1,i2-is2,i3,hz) = u(i1-is1,i2-is2,i3,hz) + uvm(2)-uvp(2)
                  ! u(i1-2*is1,i2-2*is2,i3,hz) = u(i1+2*is1,i2+2*is2,i3,hz)+ uvm2(2)-uvp2(2)
                  fw1= (8.*(uvp(2)-uvm(2))-(uvp2(2)-uvm2(2)))/(12.*dra)
                  wrrr=(uvp2(2)-2.*(uvp(2)-uvm(2))-uvm2(2))/(2.*dra**3)
                  ! wrr=(uvp(2)-2.*uv0(2)+uvm(2))/(dra**2)
                  wrr=(-30.*uv0(2)+16.*(uvp(2)+uvm(2))-(uvp2(2)+uvm2(2)
     & ) )/(12.*dra**2)
                  ! wr=(uvp(2)-uvm(2))/(2.*dra)
                  wr=(8.*(uvp(2)-uvm(2))-(uvp2(2)-uvm2(2)))/(12.*dra)
                  fw2= c11*wrrr + (c1+c11r)*wrr + c1r*wr
                  ! for tangential derivatives:
                   call ogf2d(ep,xy(i1-js1,i2-js2,i3,0),xy(i1-js1,i2-
     & js2,i3,1),t,uvm(0),uvm(1),uvm(2))
                   call ogf2d(ep,xy(i1+js1,i2+js2,i3,0),xy(i1+js1,i2+
     & js2,i3,1),t,uvp(0),uvp(1),uvp(2))
                   call ogf2d(ep,xy(i1-2*js1,i2-2*js2,i3,0),xy(i1-2*
     & js1,i2-2*js2,i3,1),t,uvm2(0),uvm2(1),uvm2(2))
                   call ogf2d(ep,xy(i1+2*js1,i2+2*js2,i3,0),xy(i1+2*
     & js1,i2+2*js2,i3,1),t,uvp2(0),uvp2(1),uvp2(2))
                  ! These approximations should be consistent with the approximations for ws and wss above
                  ! fw2=fw2 + c22r*(uvp(2)-2.*uv0(2)+uvm(2))/(dsa**2)+c2r*(uvp(2)-uvm(2))/(2.*dsa)
                  fw2=fw2 + c22r*(-30.*uv0(2)+16.*(uvp(2)+uvm(2))-(
     & uvp2(2)+uvm2(2)) )/(12.*dsa**2)+ c2r*(8.*(uvp(2)-uvm(2))-(uvp2(
     & 2)-uvm2(2)))/(12.*dsa)
                 ! Now assign ex and ey at the ghost points:
                 ! #Include "bc4Maxwell.h"
                 ! Use 5th-order extrap: 8wdh* 2015/07/03
! ************ Results from mx/codes/bc4.maple *******************
      gIII=-tau1*(c2*us+c22*uss)-tau2*(c2*vs+c22*vss)

      tauU=tau1*u(i1,i2,i3,ex)+tau2*u(i1,i2,i3,ey)

      tauUp1=tau1*u(i1+is1,i2+is2,i3+is3,ex)+tau2*u(i1+is1,i2+is2,i3+
     & is3,ey)

      tauUp2=tau1*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)+tau2*u(i1+2*is1,i2+
     & 2*is2,i3+2*is3,ey)

      tauUp3=tau1*u(i1+3*is1,i2+3*is2,i3+3*is3,ex)+tau2*u(i1+3*is1,i2+
     & 3*is2,i3+3*is3,ey)

      gIV=-10*tauU+10*tauUp1-5*tauUp2+tauUp3 +gIVf

      ttu1=-1/(3*c1*ctlr*dra-6*c1*dra-c11*ctlrr+12*c11)*(c1*ctlr*dra*
     & gIV+2*c1*ctlr*dra*tauUp1-c1*ctlr*dra*tauUp2+6*c1*dra*tauUp1-
     & c11*ctlrr*gIV-6*c11*ctlrr*tauU+4*c11*ctlrr*tauUp1-c11*ctlrr*
     & tauUp2-12*dra**2*gIII-12*dra**2*tau1DotUtt-24*c11*tauU+12*c11*
     & tauUp1)
      ttu2=-(2*c1*ctlr*dra*gIV+10*c1*ctlr*dra*tauUp1-5*c1*ctlr*dra*
     & tauUp2+6*c1*dra*gIV+30*c1*dra*tauUp1-4*c11*ctlrr*gIV-30*c11*
     & ctlrr*tauU+20*c11*ctlrr*tauUp1-5*c11*ctlrr*tauUp2-60*dra**2*
     & gIII-60*dra**2*tau1DotUtt-12*c11*gIV-120*c11*tauU+60*c11*
     & tauUp1)/(3*c1*ctlr*dra-6*c1*dra-c11*ctlrr+12*c11)

      f1um2=-1/2.*b3u/dra**3-1/12.*b2u/dra**2+1/12.*b1u/dra
      f1um1=b3u/dra**3+4/3.*b2u/dra**2-2/3.*b1u/dra
      f1vm2=-1/2.*b3v/dra**3-1/12.*b2v/dra**2+1/12.*b1v/dra
      f1vm1=b3v/dra**3+4/3.*b2v/dra**2-2/3.*b1v/dra
      f1f  =-1/12.*(b1u*dra**2*u(i1+2*is1,i2+2*is2,i3,ex)-8*b1u*dra**2*
     & u(i1+is1,i2+is2,i3,ex)+b1v*dra**2*u(i1+2*is1,i2+2*is2,i3,ey)-8*
     & b1v*dra**2*u(i1+is1,i2+is2,i3,ey)-12*bf*dra**3+b2u*dra*u(i1+2*
     & is1,i2+2*is2,i3,ex)-16*b2u*dra*u(i1+is1,i2+is2,i3,ex)+b2v*dra*
     & u(i1+2*is1,i2+2*is2,i3,ey)-16*b2v*dra*u(i1+is1,i2+is2,i3,ey)+
     & 30*b2u*dra*u(i1,i2,i3,ex)+30*b2v*dra*u(i1,i2,i3,ey)-6*b3u*u(i1+
     & 2*is1,i2+2*is2,i3,ex)+12*b3u*u(i1+is1,i2+is2,i3,ex)-6*b3v*u(i1+
     & 2*is1,i2+2*is2,i3,ey)+12*b3v*u(i1+is1,i2+is2,i3,ey))/dra**3

      f2um2=1/12.*a11m2
      f2um1=-2/3.*a11m1
      f2vm2=1/12.*a12m2
      f2vm1=-2/3.*a12m1
      f2f  =2/3.*a11p1*u(i1+is1,i2+is2,i3,ex)+2/3.*a12p1*u(i1+is1,i2+
     & is2,i3,ey)-1/12.*a11p2*u(i1+2*is1,i2+2*is2,i3,ex)-1/12.*a12p2*
     & u(i1+2*is1,i2+2*is2,i3,ey)-Da1DotU*dra

      u(i1-2*is1,i2-2*is2,i3,ex) = (f1f*f2um1*tau2**2-f1f*f2vm1*tau1*
     & tau2-f1um1*f2f*tau2**2-f1um1*f2vm1*tau2*ttu1-f1um1*f2vm2*tau2*
     & ttu2+f1vm1*f2f*tau1*tau2+f1vm1*f2um1*tau2*ttu1+f1vm1*f2vm2*
     & tau1*ttu2+f1vm2*f2um1*tau2*ttu2-f1vm2*f2vm1*tau1*ttu2)/(f1um1*
     & f2um2*tau2**2-f1um1*f2vm2*tau1*tau2-f1um2*f2um1*tau2**2+f1um2*
     & f2vm1*tau1*tau2-f1vm1*f2um2*tau1*tau2+f1vm1*f2vm2*tau1**2+
     & f1vm2*f2um1*tau1*tau2-f1vm2*f2vm1*tau1**2)

      u(i1-is1,i2-is2,i3,ex) = -(f1f*f2um2*tau2**2-f1f*f2vm2*tau1*tau2-
     & f1um2*f2f*tau2**2-f1um2*f2vm1*tau2*ttu1-f1um2*f2vm2*tau2*ttu2+
     & f1vm1*f2um2*tau2*ttu1-f1vm1*f2vm2*tau1*ttu1+f1vm2*f2f*tau1*
     & tau2+f1vm2*f2um2*tau2*ttu2+f1vm2*f2vm1*tau1*ttu1)/(f1um1*f2um2*
     & tau2**2-f1um1*f2vm2*tau1*tau2-f1um2*f2um1*tau2**2+f1um2*f2vm1*
     & tau1*tau2-f1vm1*f2um2*tau1*tau2+f1vm1*f2vm2*tau1**2+f1vm2*
     & f2um1*tau1*tau2-f1vm2*f2vm1*tau1**2)

      u(i1-2*is1,i2-2*is2,i3,ey) = -(f1f*f2um1*tau1*tau2-f1f*f2vm1*
     & tau1**2-f1um1*f2f*tau1*tau2-f1um1*f2um2*tau2*ttu2-f1um1*f2vm1*
     & tau1*ttu1+f1um2*f2um1*tau2*ttu2-f1um2*f2vm1*tau1*ttu2+f1vm1*
     & f2f*tau1**2+f1vm1*f2um1*tau1*ttu1+f1vm1*f2um2*tau1*ttu2)/(
     & f1um1*f2um2*tau2**2-f1um1*f2vm2*tau1*tau2-f1um2*f2um1*tau2**2+
     & f1um2*f2vm1*tau1*tau2-f1vm1*f2um2*tau1*tau2+f1vm1*f2vm2*tau1**
     & 2+f1vm2*f2um1*tau1*tau2-f1vm2*f2vm1*tau1**2)

      u(i1-is1,i2-is2,i3,ey) = (f1f*f2um2*tau1*tau2-f1f*f2vm2*tau1**2+
     & f1um1*f2um2*tau2*ttu1-f1um1*f2vm2*tau1*ttu1-f1um2*f2f*tau1*
     & tau2-f1um2*f2um1*tau2*ttu1-f1um2*f2vm2*tau1*ttu2+f1vm2*f2f*
     & tau1**2+f1vm2*f2um1*tau1*ttu1+f1vm2*f2um2*tau1*ttu2)/(f1um1*
     & f2um2*tau2**2-f1um1*f2vm2*tau1*tau2-f1um2*f2um1*tau2**2+f1um2*
     & f2vm1*tau1*tau2-f1vm1*f2um2*tau1*tau2+f1vm1*f2vm2*tau1**2+
     & f1vm2*f2um1*tau1*tau2-f1vm2*f2vm1*tau1**2)


 ! *********** done *********************
                 ! extrapolate normal component:
                 ! #Include "bc4eMaxwell.h"
                 ! Now assign Hz at the ghost points
                 ! u(i1-  is1,i2-  is2,i3-  is3,hz) = u(i1+  is1,i2+  is2,i3+  is3,hz)
                 ! u(i1-2*is1,i2-2*is2,i3-2*is3,hz) = u(i1+2*is1,i2+2*is2,i3+2*is3,hz)


! ************ Hz Answer *******************
      cw2=c1+c11r
      cw1=c1r
      bfw2=c22r*wss+c2r*ws-fw2

      u(i1-is1,i2-is2,i3,hz) = 1/2.*(-18*c11*u(i1+is1,i2+is2,i3,hz)+36*
     & c11*fw1*dra-12*cw2*dra*u(i1+is1,i2+is2,i3,hz)+15*cw2*dra*u(i1,
     & i2,i3,hz)+cw2*dra*u(i1+2*is1,i2+2*is2,i3,hz)+6*cw2*dra**2*fw1-
     & 6*cw1*dra**3*fw1-6*bfw2*dra**3)/(-9*c11+2*cw2*dra)

      u(i1-2*is1,i2-2*is2,i3,hz) = (-64*cw2*dra*u(i1+is1,i2+is2,i3,hz)+
     & 36*c11*fw1*dra+60*cw2*dra*u(i1,i2,i3,hz)+6*cw2*dra*u(i1+2*is1,
     & i2+2*is2,i3,hz)+48*cw2*dra**2*fw1-24*cw1*dra**3*fw1-24*bfw2*
     & dra**3-9*c11*u(i1+2*is1,i2+2*is2,i3,hz))/(-9*c11+2*cw2*dra)


 ! *********** Hz done *********************
                 !  **********************************************************************************************
                 else if( mask(i1,i2,i3).lt.0 )then
                  ! we need to assign ghost points that lie outside of interpolation points
                  ! This case is similar to above except that we extrapolate the 2nd-ghost line values for a1.u
                  jac=1./(rx(i1,i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,
     & i2,i3))
                  a11 =rsxy(i1,i2,i3,axis  ,0)*jac
                  a12 =rsxy(i1,i2,i3,axis  ,1)*jac
                  a21 =rsxy(i1,i2,i3,axisp1,0)*jac
                  a22 =rsxy(i1,i2,i3,axisp1,1)*jac
                  jacm1=1./(rx(i1-is1,i2-is2,i3)*sy(i1-is1,i2-is2,i3)-
     & ry(i1-is1,i2-is2,i3)*sx(i1-is1,i2-is2,i3))
                  a11m1 =rsxy(i1-is1,i2-is2,i3,axis  ,0)*jacm1
                  a12m1 =rsxy(i1-is1,i2-is2,i3,axis  ,1)*jacm1
                  jacp1=1./(rx(i1+is1,i2+is2,i3)*sy(i1+is1,i2+is2,i3)-
     & ry(i1+is1,i2+is2,i3)*sx(i1+is1,i2+is2,i3))
                  a11p1=rsxy(i1+is1,i2+is2,i3,axis,0)*jacp1
                  a12p1=rsxy(i1+is1,i2+is2,i3,axis,1)*jacp1
                  jacm2=1./(rx(i1-2*is1,i2-2*is2,i3)*sy(i1-2*is1,i2-2*
     & is2,i3)-ry(i1-2*is1,i2-2*is2,i3)*sx(i1-2*is1,i2-2*is2,i3))
                  a11m2=rsxy(i1-2*is1,i2-2*is2,i3,axis,0)*jacm2
                  a12m2=rsxy(i1-2*is1,i2-2*is2,i3,axis,1)*jacm2
                  jacp2=1./(rx(i1+2*is1,i2+2*is2,i3)*sy(i1+2*is1,i2+2*
     & is2,i3)-ry(i1+2*is1,i2+2*is2,i3)*sx(i1+2*is1,i2+2*is2,i3))
                  a11p2=rsxy(i1+2*is1,i2+2*is2,i3,axis,0)*jacp2
                  a12p2=rsxy(i1+2*is1,i2+2*is2,i3,axis,1)*jacp2
                  a11r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axis,0)/(rx(
     & i1+is1,i2+is2,i3+is3)*sy(i1+is1,i2+is2,i3+is3)-ry(i1+is1,i2+
     & is2,i3+is3)*sx(i1+is1,i2+is2,i3+is3)))-(rsxy(i1-is1,i2-is2,i3-
     & is3,axis,0)/(rx(i1-is1,i2-is2,i3-is3)*sy(i1-is1,i2-is2,i3-is3)-
     & ry(i1-is1,i2-is2,i3-is3)*sx(i1-is1,i2-is2,i3-is3))))-((rsxy(i1+
     & 2*is1,i2+2*is2,i3+2*is3,axis,0)/(rx(i1+2*is1,i2+2*is2,i3+2*is3)
     & *sy(i1+2*is1,i2+2*is2,i3+2*is3)-ry(i1+2*is1,i2+2*is2,i3+2*is3)*
     & sx(i1+2*is1,i2+2*is2,i3+2*is3)))-(rsxy(i1-2*is1,i2-2*is2,i3-2*
     & is3,axis,0)/(rx(i1-2*is1,i2-2*is2,i3-2*is3)*sy(i1-2*is1,i2-2*
     & is2,i3-2*is3)-ry(i1-2*is1,i2-2*is2,i3-2*is3)*sx(i1-2*is1,i2-2*
     & is2,i3-2*is3)))))/(12.*dra)
                  a12r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axis,1)/(rx(
     & i1+is1,i2+is2,i3+is3)*sy(i1+is1,i2+is2,i3+is3)-ry(i1+is1,i2+
     & is2,i3+is3)*sx(i1+is1,i2+is2,i3+is3)))-(rsxy(i1-is1,i2-is2,i3-
     & is3,axis,1)/(rx(i1-is1,i2-is2,i3-is3)*sy(i1-is1,i2-is2,i3-is3)-
     & ry(i1-is1,i2-is2,i3-is3)*sx(i1-is1,i2-is2,i3-is3))))-((rsxy(i1+
     & 2*is1,i2+2*is2,i3+2*is3,axis,1)/(rx(i1+2*is1,i2+2*is2,i3+2*is3)
     & *sy(i1+2*is1,i2+2*is2,i3+2*is3)-ry(i1+2*is1,i2+2*is2,i3+2*is3)*
     & sx(i1+2*is1,i2+2*is2,i3+2*is3)))-(rsxy(i1-2*is1,i2-2*is2,i3-2*
     & is3,axis,1)/(rx(i1-2*is1,i2-2*is2,i3-2*is3)*sy(i1-2*is1,i2-2*
     & is2,i3-2*is3)-ry(i1-2*is1,i2-2*is2,i3-2*is3)*sx(i1-2*is1,i2-2*
     & is2,i3-2*is3)))))/(12.*dra)
                  a21r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axisp1,0)/(rx(
     & i1+is1,i2+is2,i3+is3)*sy(i1+is1,i2+is2,i3+is3)-ry(i1+is1,i2+
     & is2,i3+is3)*sx(i1+is1,i2+is2,i3+is3)))-(rsxy(i1-is1,i2-is2,i3-
     & is3,axisp1,0)/(rx(i1-is1,i2-is2,i3-is3)*sy(i1-is1,i2-is2,i3-
     & is3)-ry(i1-is1,i2-is2,i3-is3)*sx(i1-is1,i2-is2,i3-is3))))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp1,0)/(rx(i1+2*is1,i2+2*
     & is2,i3+2*is3)*sy(i1+2*is1,i2+2*is2,i3+2*is3)-ry(i1+2*is1,i2+2*
     & is2,i3+2*is3)*sx(i1+2*is1,i2+2*is2,i3+2*is3)))-(rsxy(i1-2*is1,
     & i2-2*is2,i3-2*is3,axisp1,0)/(rx(i1-2*is1,i2-2*is2,i3-2*is3)*sy(
     & i1-2*is1,i2-2*is2,i3-2*is3)-ry(i1-2*is1,i2-2*is2,i3-2*is3)*sx(
     & i1-2*is1,i2-2*is2,i3-2*is3)))))/(12.*dra)
                  a22r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axisp1,1)/(rx(
     & i1+is1,i2+is2,i3+is3)*sy(i1+is1,i2+is2,i3+is3)-ry(i1+is1,i2+
     & is2,i3+is3)*sx(i1+is1,i2+is2,i3+is3)))-(rsxy(i1-is1,i2-is2,i3-
     & is3,axisp1,1)/(rx(i1-is1,i2-is2,i3-is3)*sy(i1-is1,i2-is2,i3-
     & is3)-ry(i1-is1,i2-is2,i3-is3)*sx(i1-is1,i2-is2,i3-is3))))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp1,1)/(rx(i1+2*is1,i2+2*
     & is2,i3+2*is3)*sy(i1+2*is1,i2+2*is2,i3+2*is3)-ry(i1+2*is1,i2+2*
     & is2,i3+2*is3)*sx(i1+2*is1,i2+2*is2,i3+2*is3)))-(rsxy(i1-2*is1,
     & i2-2*is2,i3-2*is3,axisp1,1)/(rx(i1-2*is1,i2-2*is2,i3-2*is3)*sy(
     & i1-2*is1,i2-2*is2,i3-2*is3)-ry(i1-2*is1,i2-2*is2,i3-2*is3)*sx(
     & i1-2*is1,i2-2*is2,i3-2*is3)))))/(12.*dra)
                 ! a11s = DS4($A11)
                 ! a12s = DS4($A12)
                 ! a21s = DS4($A21)
                 ! a22s = DS4($A22)
                  c11 = (rsxy(i1,i2,i3,axis,0)**2+rsxy(i1,i2,i3,axis,1)
     & **2)
                  c22 = (rsxy(i1,i2,i3,axisp1,0)**2+rsxy(i1,i2,i3,
     & axisp1,1)**2)
                 ! *  c1 = C1Order4(i1,i2,i3)
                 ! *  c2 = C2Order4(i1,i2,i3)
                  ! These next r derivatives are needed for Hz
                  c11r = (8.*((rsxy(i1+is1,i2+is2,i3,axis,0)**2+rsxy(
     & i1+is1,i2+is2,i3,axis,1)**2)-(rsxy(i1-is1,i2-is2,i3,axis,0)**2+
     & rsxy(i1-is1,i2-is2,i3,axis,1)**2))   -((rsxy(i1+2*is1,i2+2*is2,
     & i3,axis,0)**2+rsxy(i1+2*is1,i2+2*is2,i3,axis,1)**2)-(rsxy(i1-2*
     & is1,i2-2*is2,i3,axis,0)**2+rsxy(i1-2*is1,i2-2*is2,i3,axis,1)**
     & 2))   )/(12.*dra)
                  c22r = (8.*((rsxy(i1+is1,i2+is2,i3,axisp1,0)**2+rsxy(
     & i1+is1,i2+is2,i3,axisp1,1)**2)-(rsxy(i1-is1,i2-is2,i3,axisp1,0)
     & **2+rsxy(i1-is1,i2-is2,i3,axisp1,1)**2))   -((rsxy(i1+2*is1,i2+
     & 2*is2,i3,axisp1,0)**2+rsxy(i1+2*is1,i2+2*is2,i3,axisp1,1)**2)-(
     & rsxy(i1-2*is1,i2-2*is2,i3,axisp1,0)**2+rsxy(i1-2*is1,i2-2*is2,
     & i3,axisp1,1)**2))   )/(12.*dra)
                 ! *  if( axis.eq.0 )then
                 ! *    c1r = C1r4(i1,i2,i3)
                 ! *    c2r = C2r4(i1,i2,i3)
                 ! *  else
                 ! *    c1r = C1s4(i1,i2,i3)
                 ! *    c2r = C2s4(i1,i2,i3)
                 ! *  end if
                 ! ************** OLD **************
                 ! *  ! Use one sided approximations as needed 
                 ! *  js1a=abs(js1)
                 ! *  js2a=abs(js2)
                 ! *  if( (i1-2*js1a).ge.md1a .and. (i2-2*js2a).ge.md2a .and. (i1+2*js1a).le.md1b .and. (i2+2*js2a).le.md2b )then
                 ! *    a11s = DS4($A11)
                 ! *    a12s = DS4($A12)
                 ! *    a21s = DS4($A21)
                 ! *    a22s = DS4($A22)
                 ! *  else if( (i1-js1a).ge.md1a .and. (i2-js2a).ge.md2a .and. (i1+js1a).le.md1b .and. (i2+js2a).le.md2b )then
                 ! *    a11s = DS($A11)
                 ! *    a12s = DS($A12)
                 ! *    a21s = DS($A21)
                 ! *    a22s = DS($A22)
                 ! *  else if( (i1-js1).ge.md1a .and. (i1-js1).le.md1b .and. (i2-js2).ge.md2a .and. (i2-js2).le.md2b )then
                 ! *   ! 2nd-order:
                 ! *   a11s =-(-3.*A11(i1,i2,i3)+4.*A11(i1-js1,i2-js2,i3)-A11(i1-2*js1,i2-2*js2,i3))/(2.*dsa)
                 ! *   a12s =-(-3.*A12(i1,i2,i3)+4.*A12(i1-js1,i2-js2,i3)-A12(i1-2*js1,i2-2*js2,i3))/(2.*dsa)
                 ! *   a21s =-(-3.*A21(i1,i2,i3)+4.*A21(i1-js1,i2-js2,i3)-A21(i1-2*js1,i2-2*js2,i3))/(2.*dsa)
                 ! *   a22s =-(-3.*A22(i1,i2,i3)+4.*A22(i1-js1,i2-js2,i3)-A22(i1-2*js1,i2-2*js2,i3))/(2.*dsa)
                 ! *  else
                 ! *   a11s = (-3.*A11(i1,i2,i3)+4.*A11(i1+js1,i2+js2,i3)-A11(i1+2*js1,i2+2*js2,i3))/(2.*dsa)
                 ! *   a12s = (-3.*A12(i1,i2,i3)+4.*A12(i1+js1,i2+js2,i3)-A12(i1+2*js1,i2+2*js2,i3))/(2.*dsa)
                 ! *   a21s = (-3.*A21(i1,i2,i3)+4.*A21(i1+js1,i2+js2,i3)-A21(i1+2*js1,i2+2*js2,i3))/(2.*dsa)
                 ! *   a22s = (-3.*A22(i1,i2,i3)+4.*A22(i1+js1,i2+js2,i3)-A22(i1+2*js1,i2+2*js2,i3))/(2.*dsa)
                 ! *  end if
                 ! * 
                 ! * 
                 ! *  ! warning -- the compiler could still try to evaluate the mask at an invalid point
                 ! *  if( (i1-js1a).ge.md1a .and. (i2-js2a).ge.md2a .and. mask(i1-js1,i2-js2,i3).ne.0 .and. ! *      (i1+js1a).le.md1b .and. (i2+js2a).le.md2b .and. mask(i1+js1,i2+js2,i3).ne.0 )then
                 ! *    us=US2(ex)
                 ! *    vs=US2(ey)
                 ! *    ws=US2(hz)
                 ! * 
                 ! *    uss=USS2(ex)
                 ! *    vss=USS2(ey)
                 ! *    wss=USS2(hz)
                 ! *   !  write(*,'(" **ghost-interp: use central difference: us,uss=",2e10.2)') us,uss
                 ! * 
                 ! *  else if( (i1-2*js1).ge.md1a .and. (i1-2*js1).le.md1b .and. ! *           (i2-2*js2).ge.md2a .and. (i2-2*js2).le.md2b .and. ! *            mask(i1-js1,i2-js2,i3).ne.0 .and. mask(i1-2*js1,i2-2*js2,i3).ne.0 )then
                 ! *    
                 ! *   ! these are just first order but this is probably good enough since these values
                 ! *   ! may not even appear in any other equations
                 ! * !  us = (u(i1,i2,i3,ex)-u(i1-js1,i2-js2,i3,ex))/dsa
                 ! * !  vs = (u(i1,i2,i3,ey)-u(i1-js1,i2-js2,i3,ey))/dsa
                 ! * !  ws = (u(i1,i2,i3,hz)-u(i1-js1,i2-js2,i3,hz))/dsa
                 ! * !
                 ! * !  uss = (u(i1,i2,i3,ex)-2.*u(i1-js1,i2-js2,i3,ex)+u(i1-2*js1,i2-2*js2,i3,ex))/(dsa**2)
                 ! * !  vss = (u(i1,i2,i3,ey)-2.*u(i1-js1,i2-js2,i3,ey)+u(i1-2*js1,i2-2*js2,i3,ey))/(dsa**2)
                 ! * !  wss = (u(i1,i2,i3,hz)-2.*u(i1-js1,i2-js2,i3,hz)+u(i1-2*js1,i2-2*js2,i3,hz))/(dsa**2)
                 ! * 
                 ! *   ! 2nd-order:
                 ! * 
                 ! *   us = -(-3.*u(i1,i2,i3,ex)+4.*u(i1-js1,i2-js2,i3,ex)-u(i1-2*js1,i2-2*js2,i3,ex))/(2.*dsa)
                 ! *   vs = -(-3.*u(i1,i2,i3,ey)+4.*u(i1-js1,i2-js2,i3,ey)-u(i1-2*js1,i2-2*js2,i3,ey))/(2.*dsa)
                 ! *   ws = -(-3.*u(i1,i2,i3,hz)+4.*u(i1-js1,i2-js2,i3,hz)-u(i1-2*js1,i2-2*js2,i3,hz))/(2.*dsa)
                 ! * 
                 ! *   uss = (2.*u(i1,i2,i3,ex)-5.*u(i1-js1,i2-js2,i3,ex)+4.*u(i1-2*js1,i2-2*js2,i3,ex)-u(i1-3*js1,i2-3*js2,i3,ex))/(dsa**2)
                 ! *   vss = (2.*u(i1,i2,i3,ey)-5.*u(i1-js1,i2-js2,i3,ey)+4.*u(i1-2*js1,i2-2*js2,i3,ey)-u(i1-3*js1,i2-3*js2,i3,ey))/(dsa**2)
                 ! *   wss = (2.*u(i1,i2,i3,hz)-5.*u(i1-js1,i2-js2,i3,hz)+4.*u(i1-2*js1,i2-2*js2,i3,hz)-u(i1-3*js1,i2-3*js2,i3,hz))/(dsa**2)
                 ! * 
                 ! * !  write(*,'(" **ghost-interp: use left-difference: us,uss=",2e10.2," us1=",e10.2," js1,js2=",2i2)') us,uss,! * !            (u(i1,i2,i3,ex)-u(i1-js1,i2-js2,i3,ex))/dsa,js1,js2
                 ! * 
                 ! *  else if( (i1+2*js1).ge.md1a .and. (i1+2*js1).le.md1b .and. ! *           (i2+2*js2).ge.md2a .and. (i2+2*js2).le.md2b .and.  ! *           mask(i1+js1,i2+js2,i3).ne.0 .and. mask(i1+2*js1,i2+2*js2,i3).ne.0 )then
                 ! * 
                 ! * !  us = (u(i1+js1,i2+js2,i3,ex)-u(i1,i2,i3,ex))/dsa
                 ! * !  vs = (u(i1+js1,i2+js2,i3,ey)-u(i1,i2,i3,ey))/dsa
                 ! * !  ws = (u(i1+js1,i2+js2,i3,hz)-u(i1,i2,i3,hz))/dsa
                 ! * !
                 ! * !  uss = (u(i1,i2,i3,ex)-2.*u(i1+js1,i2+js2,i3,ex)+u(i1+2*js1,i2+2*js2,i3,ex))/(dsa**2)
                 ! * !  vss = (u(i1,i2,i3,ey)-2.*u(i1+js1,i2+js2,i3,ey)+u(i1+2*js1,i2+2*js2,i3,ey))/(dsa**2)
                 ! * !  wss = (u(i1,i2,i3,hz)-2.*u(i1+js1,i2+js2,i3,hz)+u(i1+2*js1,i2+2*js2,i3,hz))/(dsa**2)
                 ! * 
                 ! *   ! 2nd-order:
                 ! *  us = (-3.*u(i1,i2,i3,ex)+4.*u(i1+js1,i2+js2,i3,ex)-u(i1+2*js1,i2+2*js2,i3,ex))/(2.*dsa)
                 ! *  vs = (-3.*u(i1,i2,i3,ey)+4.*u(i1+js1,i2+js2,i3,ey)-u(i1+2*js1,i2+2*js2,i3,ey))/(2.*dsa)
                 ! *  ws = (-3.*u(i1,i2,i3,hz)+4.*u(i1+js1,i2+js2,i3,hz)-u(i1+2*js1,i2+2*js2,i3,hz))/(2.*dsa)
                 ! *  uss = (2.*u(i1,i2,i3,ex)-5.*u(i1+js1,i2+js2,i3,ex)+4.*u(i1+2*js1,i2+2*js2,i3,ex)-u(i1+3*js1,i2+3*js2,i3,ex))/(dsa**2)
                 ! *  vss = (2.*u(i1,i2,i3,ey)-5.*u(i1+js1,i2+js2,i3,ey)+4.*u(i1+2*js1,i2+2*js2,i3,ey)-u(i1+3*js1,i2+3*js2,i3,ey))/(dsa**2)
                 ! *  wss = (2.*u(i1,i2,i3,hz)-5.*u(i1+js1,i2+js2,i3,hz)+4.*u(i1+2*js1,i2+2*js2,i3,hz)-u(i1+3*js1,i2+3*js2,i3,hz))/(dsa**2)
                 ! * 
                 ! *  ! write(*,'(" **ghost-interp: use right-difference: us,uss=",2e10.2)') us,uss
                 ! * 
                 ! *  else 
                 ! *    ! this case shouldn't matter
                 ! *    us=0.
                 ! *    vs=0.
                 ! *    ws=0.
                 ! *    uss=0.
                 ! *    vss=0.
                 ! *    wss=0.
                 ! *  end if
                 ! *********************** NEW ************************
                  ! ***************************************************************************************
                  ! Use one sided approximations as needed for expressions needing tangential derivatives
                  ! ***************************************************************************************
                  js1a=abs(js1)
                  js2a=abs(js2)
                  ! *** first do metric derivatives -- no need to worry about the mask value ****
                  if( (i1-2*js1a).ge.md1a .and. (i1+2*js1a).le.md1b 
     & .and. (i2-2*js2a).ge.md2a .and. (i2+2*js2a).le.md2b )then
                   ! centered approximation is ok
                   c1 = (rsxyx42(i1,i2,i3,axis,0)+rsxyy42(i1,i2,i3,
     & axis,1))
                   c2 = (rsxyx42(i1,i2,i3,axisp1,0)+rsxyy42(i1,i2,i3,
     & axisp1,1))
                   if( axis.eq.0 )then
                     c1r = (rsxyxr42(i1,i2,i3,axis,0)+rsxyyr42(i1,i2,
     & i3,axis,1))
                     c2r = (rsxyxr42(i1,i2,i3,axisp1,0)+rsxyyr42(i1,i2,
     & i3,axisp1,1))
                   else
                     c1r = (rsxyxs42(i1,i2,i3,axis,0)+rsxyys42(i1,i2,
     & i3,axis,1))
                     c2r = (rsxyxs42(i1,i2,i3,axisp1,0)+rsxyys42(i1,i2,
     & i3,axisp1,1))
                   end if
                   a11s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axis,0)/(rx(
     & i1+js1,i2+js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+
     & js2,i3+js3)*sx(i1+js1,i2+js2,i3+js3)))-(rsxy(i1-js1,i2-js2,i3-
     & js3,axis,0)/(rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-
     & ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3))))-((rsxy(i1+
     & 2*js1,i2+2*js2,i3+2*js3,axis,0)/(rx(i1+2*js1,i2+2*js2,i3+2*js3)
     & *sy(i1+2*js1,i2+2*js2,i3+2*js3)-ry(i1+2*js1,i2+2*js2,i3+2*js3)*
     & sx(i1+2*js1,i2+2*js2,i3+2*js3)))-(rsxy(i1-2*js1,i2-2*js2,i3-2*
     & js3,axis,0)/(rx(i1-2*js1,i2-2*js2,i3-2*js3)*sy(i1-2*js1,i2-2*
     & js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,i3-2*js3)*sx(i1-2*js1,i2-2*
     & js2,i3-2*js3)))))/(12.*dsa)
                   a12s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axis,1)/(rx(
     & i1+js1,i2+js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+
     & js2,i3+js3)*sx(i1+js1,i2+js2,i3+js3)))-(rsxy(i1-js1,i2-js2,i3-
     & js3,axis,1)/(rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-
     & ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3))))-((rsxy(i1+
     & 2*js1,i2+2*js2,i3+2*js3,axis,1)/(rx(i1+2*js1,i2+2*js2,i3+2*js3)
     & *sy(i1+2*js1,i2+2*js2,i3+2*js3)-ry(i1+2*js1,i2+2*js2,i3+2*js3)*
     & sx(i1+2*js1,i2+2*js2,i3+2*js3)))-(rsxy(i1-2*js1,i2-2*js2,i3-2*
     & js3,axis,1)/(rx(i1-2*js1,i2-2*js2,i3-2*js3)*sy(i1-2*js1,i2-2*
     & js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,i3-2*js3)*sx(i1-2*js1,i2-2*
     & js2,i3-2*js3)))))/(12.*dsa)
                   a21s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axisp1,0)/(
     & rx(i1+js1,i2+js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+
     & js2,i3+js3)*sx(i1+js1,i2+js2,i3+js3)))-(rsxy(i1-js1,i2-js2,i3-
     & js3,axisp1,0)/(rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-
     & js3)-ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3))))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,0)/(rx(i1+2*js1,i2+2*
     & js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,i3+2*js3)-ry(i1+2*js1,i2+2*
     & js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,i3+2*js3)))-(rsxy(i1-2*js1,
     & i2-2*js2,i3-2*js3,axisp1,0)/(rx(i1-2*js1,i2-2*js2,i3-2*js3)*sy(
     & i1-2*js1,i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,i3-2*js3)*sx(
     & i1-2*js1,i2-2*js2,i3-2*js3)))))/(12.*dsa)
                   a22s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axisp1,1)/(
     & rx(i1+js1,i2+js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+
     & js2,i3+js3)*sx(i1+js1,i2+js2,i3+js3)))-(rsxy(i1-js1,i2-js2,i3-
     & js3,axisp1,1)/(rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-
     & js3)-ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3))))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,1)/(rx(i1+2*js1,i2+2*
     & js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,i3+2*js3)-ry(i1+2*js1,i2+2*
     & js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,i3+2*js3)))-(rsxy(i1-2*js1,
     & i2-2*js2,i3-2*js3,axisp1,1)/(rx(i1-2*js1,i2-2*js2,i3-2*js3)*sy(
     & i1-2*js1,i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,i3-2*js3)*sx(
     & i1-2*js1,i2-2*js2,i3-2*js3)))))/(12.*dsa)
                  else if( (i1-js1a).ge.md1a .and. (i1+js1a).le.md1b 
     & .and. (i2-js2a).ge.md2a .and. (i2+js2a).le.md2b )then
                   ! use 2nd-order centered approximation
                   c1 = (rsxyx22(i1,i2,i3,axis,0)+rsxyy22(i1,i2,i3,
     & axis,1))
                   c2 = (rsxyx22(i1,i2,i3,axisp1,0)+rsxyy22(i1,i2,i3,
     & axisp1,1))
                   if( axis.eq.0 )then
                     c1r = (rsxyxr22(i1,i2,i3,axis,0)+rsxyyr22(i1,i2,
     & i3,axis,1))
                     c2r = (rsxyxr22(i1,i2,i3,axisp1,0)+rsxyyr22(i1,i2,
     & i3,axisp1,1))
                   else
                     c1r = (rsxyxs22(i1,i2,i3,axis,0)+rsxyys22(i1,i2,
     & i3,axis,1))
                     c2r = (rsxyxs22(i1,i2,i3,axisp1,0)+rsxyys22(i1,i2,
     & i3,axisp1,1))
                   end if
                   a11s = ((rsxy(i1+js1,i2+js2,i3+js3,axis,0)/(rx(i1+
     & js1,i2+js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+js2,
     & i3+js3)*sx(i1+js1,i2+js2,i3+js3)))-(rsxy(i1-js1,i2-js2,i3-js3,
     & axis,0)/(rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-ry(
     & i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3))))/(2.*dsa)
                   a12s = ((rsxy(i1+js1,i2+js2,i3+js3,axis,1)/(rx(i1+
     & js1,i2+js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+js2,
     & i3+js3)*sx(i1+js1,i2+js2,i3+js3)))-(rsxy(i1-js1,i2-js2,i3-js3,
     & axis,1)/(rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-ry(
     & i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3))))/(2.*dsa)
                   a21s = ((rsxy(i1+js1,i2+js2,i3+js3,axisp1,0)/(rx(i1+
     & js1,i2+js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+js2,
     & i3+js3)*sx(i1+js1,i2+js2,i3+js3)))-(rsxy(i1-js1,i2-js2,i3-js3,
     & axisp1,0)/(rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-
     & ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3))))/(2.*dsa)
                   a22s = ((rsxy(i1+js1,i2+js2,i3+js3,axisp1,1)/(rx(i1+
     & js1,i2+js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+js2,
     & i3+js3)*sx(i1+js1,i2+js2,i3+js3)))-(rsxy(i1-js1,i2-js2,i3-js3,
     & axisp1,1)/(rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-
     & ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3))))/(2.*dsa)
                  else if( (i1-3*js1a).ge.md1a .and. (i2-3*js2a)
     & .ge.md2a )then
                   ! one sided  2nd-order:
                   c1 = 2.*(rsxyx22(i1-js1a,i2-js2a,i3,axis,0)+rsxyy22(
     & i1-js1a,i2-js2a,i3,axis,1))-(rsxyx22(i1-2*js1a,i2-2*js2a,i3,
     & axis,0)+rsxyy22(i1-2*js1a,i2-2*js2a,i3,axis,1))
                   c2 = 2.*(rsxyx22(i1-js1a,i2-js2a,i3,axisp1,0)+
     & rsxyy22(i1-js1a,i2-js2a,i3,axisp1,1))-(rsxyx22(i1-2*js1a,i2-2*
     & js2a,i3,axisp1,0)+rsxyy22(i1-2*js1a,i2-2*js2a,i3,axisp1,1))
                   if( axis.eq.0 )then
                     c1r = 2.*(rsxyxr22(i1-js1a,i2-js2a,i3,axis,0)+
     & rsxyyr22(i1-js1a,i2-js2a,i3,axis,1))-(rsxyxr22(i1-2*js1a,i2-2*
     & js2a,i3,axis,0)+rsxyyr22(i1-2*js1a,i2-2*js2a,i3,axis,1))
                     c2r = 2.*(rsxyxr22(i1-js1a,i2-js2a,i3,axisp1,0)+
     & rsxyyr22(i1-js1a,i2-js2a,i3,axisp1,1))-(rsxyxr22(i1-2*js1a,i2-
     & 2*js2a,i3,axisp1,0)+rsxyyr22(i1-2*js1a,i2-2*js2a,i3,axisp1,1))
                   else
                     c1r = 2.*(rsxyxs22(i1-js1a,i2-js2a,i3,axis,0)+
     & rsxyys22(i1-js1a,i2-js2a,i3,axis,1))-(rsxyxs22(i1-2*js1a,i2-2*
     & js2a,i3,axis,0)+rsxyys22(i1-2*js1a,i2-2*js2a,i3,axis,1))
                     c2r = 2.*(rsxyxs22(i1-js1a,i2-js2a,i3,axisp1,0)+
     & rsxyys22(i1-js1a,i2-js2a,i3,axisp1,1))-(rsxyxs22(i1-2*js1a,i2-
     & 2*js2a,i3,axisp1,0)+rsxyys22(i1-2*js1a,i2-2*js2a,i3,axisp1,1))
                   end if
                   a11s =-(-3.*(rsxy(i1,i2,i3,axis,0)/(rx(i1,i2,i3)*sy(
     & i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+4.*(rsxy(i1-js1a,i2-js2a,
     & i3,axis,0)/(rx(i1-js1a,i2-js2a,i3)*sy(i1-js1a,i2-js2a,i3)-ry(
     & i1-js1a,i2-js2a,i3)*sx(i1-js1a,i2-js2a,i3)))-(rsxy(i1-2*js1a,
     & i2-2*js2a,i3,axis,0)/(rx(i1-2*js1a,i2-2*js2a,i3)*sy(i1-2*js1a,
     & i2-2*js2a,i3)-ry(i1-2*js1a,i2-2*js2a,i3)*sx(i1-2*js1a,i2-2*
     & js2a,i3))))/(2.*dsb) ! NOTE: use ds not dsa
                   a12s =-(-3.*(rsxy(i1,i2,i3,axis,1)/(rx(i1,i2,i3)*sy(
     & i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+4.*(rsxy(i1-js1a,i2-js2a,
     & i3,axis,1)/(rx(i1-js1a,i2-js2a,i3)*sy(i1-js1a,i2-js2a,i3)-ry(
     & i1-js1a,i2-js2a,i3)*sx(i1-js1a,i2-js2a,i3)))-(rsxy(i1-2*js1a,
     & i2-2*js2a,i3,axis,1)/(rx(i1-2*js1a,i2-2*js2a,i3)*sy(i1-2*js1a,
     & i2-2*js2a,i3)-ry(i1-2*js1a,i2-2*js2a,i3)*sx(i1-2*js1a,i2-2*
     & js2a,i3))))/(2.*dsb)
                   a21s =-(-3.*(rsxy(i1,i2,i3,axisp1,0)/(rx(i1,i2,i3)*
     & sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+4.*(rsxy(i1-js1a,i2-
     & js2a,i3,axisp1,0)/(rx(i1-js1a,i2-js2a,i3)*sy(i1-js1a,i2-js2a,
     & i3)-ry(i1-js1a,i2-js2a,i3)*sx(i1-js1a,i2-js2a,i3)))-(rsxy(i1-2*
     & js1a,i2-2*js2a,i3,axisp1,0)/(rx(i1-2*js1a,i2-2*js2a,i3)*sy(i1-
     & 2*js1a,i2-2*js2a,i3)-ry(i1-2*js1a,i2-2*js2a,i3)*sx(i1-2*js1a,
     & i2-2*js2a,i3))))/(2.*dsb)
                   a22s =-(-3.*(rsxy(i1,i2,i3,axisp1,1)/(rx(i1,i2,i3)*
     & sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+4.*(rsxy(i1-js1a,i2-
     & js2a,i3,axisp1,1)/(rx(i1-js1a,i2-js2a,i3)*sy(i1-js1a,i2-js2a,
     & i3)-ry(i1-js1a,i2-js2a,i3)*sx(i1-js1a,i2-js2a,i3)))-(rsxy(i1-2*
     & js1a,i2-2*js2a,i3,axisp1,1)/(rx(i1-2*js1a,i2-2*js2a,i3)*sy(i1-
     & 2*js1a,i2-2*js2a,i3)-ry(i1-2*js1a,i2-2*js2a,i3)*sx(i1-2*js1a,
     & i2-2*js2a,i3))))/(2.*dsb)
                 ! if( debug.gt.0 )then
                 !   write(*,'(" ghost-interp:left-shift i=",3i3," js1a,js2a=",2i3," c2r,c2s2(-1),c2s2(-2)=",10e10.2)')!      i1,i2,i3,js1a,js2a,c2r,C2s2(i1-js1a,i2-js2a,i3),C2s2(i1-2*js1a,i2-2*js2a,i3)
                 ! end if
                  else if( (i1+3*js1a).le.md1b .and. (i2+3*js2a)
     & .le.md2b )then
                   ! one sided  2nd-order:
                   c1 = 2.*(rsxyx22(i1+js1a,i2+js2a,i3,axis,0)+rsxyy22(
     & i1+js1a,i2+js2a,i3,axis,1))-(rsxyx22(i1+2*js1a,i2+2*js2a,i3,
     & axis,0)+rsxyy22(i1+2*js1a,i2+2*js2a,i3,axis,1))
                   c2 = 2.*(rsxyx22(i1+js1a,i2+js2a,i3,axisp1,0)+
     & rsxyy22(i1+js1a,i2+js2a,i3,axisp1,1))-(rsxyx22(i1+2*js1a,i2+2*
     & js2a,i3,axisp1,0)+rsxyy22(i1+2*js1a,i2+2*js2a,i3,axisp1,1))
                   if( axis.eq.0 )then
                     c1r = 2.*(rsxyxr22(i1+js1a,i2+js2a,i3,axis,0)+
     & rsxyyr22(i1+js1a,i2+js2a,i3,axis,1))-(rsxyxr22(i1+2*js1a,i2+2*
     & js2a,i3,axis,0)+rsxyyr22(i1+2*js1a,i2+2*js2a,i3,axis,1))
                     c2r = 2.*(rsxyxr22(i1+js1a,i2+js2a,i3,axisp1,0)+
     & rsxyyr22(i1+js1a,i2+js2a,i3,axisp1,1))-(rsxyxr22(i1+2*js1a,i2+
     & 2*js2a,i3,axisp1,0)+rsxyyr22(i1+2*js1a,i2+2*js2a,i3,axisp1,1))
                   else
                     c1r = 2.*(rsxyxs22(i1+js1a,i2+js2a,i3,axis,0)+
     & rsxyys22(i1+js1a,i2+js2a,i3,axis,1))-(rsxyxs22(i1+2*js1a,i2+2*
     & js2a,i3,axis,0)+rsxyys22(i1+2*js1a,i2+2*js2a,i3,axis,1))
                     c2r = 2.*(rsxyxs22(i1+js1a,i2+js2a,i3,axisp1,0)+
     & rsxyys22(i1+js1a,i2+js2a,i3,axisp1,1))-(rsxyxs22(i1+2*js1a,i2+
     & 2*js2a,i3,axisp1,0)+rsxyys22(i1+2*js1a,i2+2*js2a,i3,axisp1,1))
                   end if
                 ! if( debug.gt.0 )then
                 !   write(*,'(" ghost-interp:right-shift i=",3i3," js1a,js2a=",2i3," c2r,c2s2(+1),c2s2(+2)=",10e10.2)')!      i1,i2,i3,js1a,js2a,c2r,C2s2(i1+js1a,i2+js2a,i3),C2s2(i1+2*js1a,i2+2*js2a,i3)
                 ! end if
                   a11s = (-3.*(rsxy(i1,i2,i3,axis,0)/(rx(i1,i2,i3)*sy(
     & i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+4.*(rsxy(i1+js1a,i2+js2a,
     & i3,axis,0)/(rx(i1+js1a,i2+js2a,i3)*sy(i1+js1a,i2+js2a,i3)-ry(
     & i1+js1a,i2+js2a,i3)*sx(i1+js1a,i2+js2a,i3)))-(rsxy(i1+2*js1a,
     & i2+2*js2a,i3,axis,0)/(rx(i1+2*js1a,i2+2*js2a,i3)*sy(i1+2*js1a,
     & i2+2*js2a,i3)-ry(i1+2*js1a,i2+2*js2a,i3)*sx(i1+2*js1a,i2+2*
     & js2a,i3))))/(2.*dsb)
                   a12s = (-3.*(rsxy(i1,i2,i3,axis,1)/(rx(i1,i2,i3)*sy(
     & i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+4.*(rsxy(i1+js1a,i2+js2a,
     & i3,axis,1)/(rx(i1+js1a,i2+js2a,i3)*sy(i1+js1a,i2+js2a,i3)-ry(
     & i1+js1a,i2+js2a,i3)*sx(i1+js1a,i2+js2a,i3)))-(rsxy(i1+2*js1a,
     & i2+2*js2a,i3,axis,1)/(rx(i1+2*js1a,i2+2*js2a,i3)*sy(i1+2*js1a,
     & i2+2*js2a,i3)-ry(i1+2*js1a,i2+2*js2a,i3)*sx(i1+2*js1a,i2+2*
     & js2a,i3))))/(2.*dsb)
                   a21s = (-3.*(rsxy(i1,i2,i3,axisp1,0)/(rx(i1,i2,i3)*
     & sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+4.*(rsxy(i1+js1a,i2+
     & js2a,i3,axisp1,0)/(rx(i1+js1a,i2+js2a,i3)*sy(i1+js1a,i2+js2a,
     & i3)-ry(i1+js1a,i2+js2a,i3)*sx(i1+js1a,i2+js2a,i3)))-(rsxy(i1+2*
     & js1a,i2+2*js2a,i3,axisp1,0)/(rx(i1+2*js1a,i2+2*js2a,i3)*sy(i1+
     & 2*js1a,i2+2*js2a,i3)-ry(i1+2*js1a,i2+2*js2a,i3)*sx(i1+2*js1a,
     & i2+2*js2a,i3))))/(2.*dsb)
                   a22s = (-3.*(rsxy(i1,i2,i3,axisp1,1)/(rx(i1,i2,i3)*
     & sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+4.*(rsxy(i1+js1a,i2+
     & js2a,i3,axisp1,1)/(rx(i1+js1a,i2+js2a,i3)*sy(i1+js1a,i2+js2a,
     & i3)-ry(i1+js1a,i2+js2a,i3)*sx(i1+js1a,i2+js2a,i3)))-(rsxy(i1+2*
     & js1a,i2+2*js2a,i3,axisp1,1)/(rx(i1+2*js1a,i2+2*js2a,i3)*sy(i1+
     & 2*js1a,i2+2*js2a,i3)-ry(i1+2*js1a,i2+2*js2a,i3)*sx(i1+2*js1a,
     & i2+2*js2a,i3))))/(2.*dsb)
                  else
                   ! this case should not happen
                   stop 44066
                  end if
                  ! ***** Now do "s"-derivatives *****
                  ! warning -- the compiler could still try to evaluate the mask at an invalid point
                  if( (i1-js1a).ge.md1a .and. (i2-js2a).ge.md2a .and. 
     & mask(i1-js1a,i2-js2a,i3).ne.0 .and. (i1+js1a).le.md1b .and. (
     & i2+js2a).le.md2b .and. mask(i1+js1a,i2+js2a,i3).ne.0 )then
                    us=(u(i1+js1,i2+js2,i3+js3,ex)-u(i1-js1,i2-js2,i3-
     & js3,ex))/(2.*dsa)
                    vs=(u(i1+js1,i2+js2,i3+js3,ey)-u(i1-js1,i2-js2,i3-
     & js3,ey))/(2.*dsa)
                    ws=(u(i1+js1,i2+js2,i3+js3,hz)-u(i1-js1,i2-js2,i3-
     & js3,hz))/(2.*dsa)
                    uss=(u(i1+js1,i2+js2,i3+js3,ex)-2.*u(i1,i2,i3,ex)+
     & u(i1-js1,i2-js2,i3-js3,ex))/(dsa**2)
                    vss=(u(i1+js1,i2+js2,i3+js3,ey)-2.*u(i1,i2,i3,ey)+
     & u(i1-js1,i2-js2,i3-js3,ey))/(dsa**2)
                    wss=(u(i1+js1,i2+js2,i3+js3,hz)-2.*u(i1,i2,i3,hz)+
     & u(i1-js1,i2-js2,i3-js3,hz))/(dsa**2)
                  else if( (i1-2*js1a).ge.md1a .and. (i2-2*js2a)
     & .ge.md2a .and. mask(i1-js1a,i2-js2a,i3).ne.0 .and. mask(i1-2*
     & js1a,i2-2*js2a,i3).ne.0 )then
                   ! 2nd-order one-sided: ** note ** use ds not dsa
                   us = (-(-3.*u(i1,i2,i3,ex)+4.*u(i1-js1a,i2-js2a,i3-
     & 0,ex)-u(i1-2*js1a,i2-2*js2a,i3-2*0,ex))/(2.*dsb))
                   vs = (-(-3.*u(i1,i2,i3,ey)+4.*u(i1-js1a,i2-js2a,i3-
     & 0,ey)-u(i1-2*js1a,i2-2*js2a,i3-2*0,ey))/(2.*dsb))
                   ws = (-(-3.*u(i1,i2,i3,hz)+4.*u(i1-js1a,i2-js2a,i3-
     & 0,hz)-u(i1-2*js1a,i2-2*js2a,i3-2*0,hz))/(2.*dsb))
                   uss = ((2.*u(i1,i2,i3,ex)-5.*u(i1-js1a,i2-js2a,i3-0,
     & ex)+4.*u(i1-2*js1a,i2-2*js2a,i3-2*0,ex)-u(i1-3*js1a,i2-3*js2a,
     & i3-3*0,ex))/(dsb**2))
                   vss = ((2.*u(i1,i2,i3,ey)-5.*u(i1-js1a,i2-js2a,i3-0,
     & ey)+4.*u(i1-2*js1a,i2-2*js2a,i3-2*0,ey)-u(i1-3*js1a,i2-3*js2a,
     & i3-3*0,ey))/(dsb**2))
                   wss = ((2.*u(i1,i2,i3,hz)-5.*u(i1-js1a,i2-js2a,i3-0,
     & hz)+4.*u(i1-2*js1a,i2-2*js2a,i3-2*0,hz)-u(i1-3*js1a,i2-3*js2a,
     & i3-3*0,hz))/(dsb**2))
                  else if( (i1+2*js1a).le.md1b .and. (i2+2*js2a)
     & .le.md2b .and.  mask(i1+js1a,i2+js2a,i3).ne.0 .and. mask(i1+2*
     & js1a,i2+2*js2a,i3).ne.0 )then
                   ! 2nd-order one-sided:
                   us = ((-3.*u(i1,i2,i3,ex)+4.*u(i1+js1a,i2+js2a,i3+0,
     & ex)-u(i1+2*js1a,i2+2*js2a,i3+2*0,ex))/(2.*dsb))
                   vs = ((-3.*u(i1,i2,i3,ey)+4.*u(i1+js1a,i2+js2a,i3+0,
     & ey)-u(i1+2*js1a,i2+2*js2a,i3+2*0,ey))/(2.*dsb))
                   ws = ((-3.*u(i1,i2,i3,hz)+4.*u(i1+js1a,i2+js2a,i3+0,
     & hz)-u(i1+2*js1a,i2+2*js2a,i3+2*0,hz))/(2.*dsb))
                   uss = ((2.*u(i1,i2,i3,ex)-5.*u(i1+js1a,i2+js2a,i3+0,
     & ex)+4.*u(i1+2*js1a,i2+2*js2a,i3+2*0,ex)-u(i1+3*js1a,i2+3*js2a,
     & i3+3*0,ex))/(dsb**2))
                   vss = ((2.*u(i1,i2,i3,ey)-5.*u(i1+js1a,i2+js2a,i3+0,
     & ey)+4.*u(i1+2*js1a,i2+2*js2a,i3+2*0,ey)-u(i1+3*js1a,i2+3*js2a,
     & i3+3*0,ey))/(dsb**2))
                   wss = ((2.*u(i1,i2,i3,hz)-5.*u(i1+js1a,i2+js2a,i3+0,
     & hz)+4.*u(i1+2*js1a,i2+2*js2a,i3+2*0,hz)-u(i1+3*js1a,i2+3*js2a,
     & i3+3*0,hz))/(dsb**2))
                  else
                    ! this case shouldn't matter
                    us=0.
                    vs=0.
                    ws=0.
                    uss=0.
                    vss=0.
                    wss=0.
                  end if
                 ! ******************************* end NEW ************************
                  tau1=rsxy(i1,i2,i3,axisp1,0)
                  tau2=rsxy(i1,i2,i3,axisp1,1)
                  uex=u(i1,i2,i3,ex)
                  uey=u(i1,i2,i3,ey)
                  ! forcing terms for TZ are stored in 
                  gIVf=0.            ! forcing for extrap tau.u
                  tau1DotUtt=0.      ! forcing for tau.Lu=0
                  Da1DotU=0.         ! forcing for div(u)=0
                  ! for Hz (w)
                  fw1=0.
                  fw2=0.
                    ! For TZ: utt0 = utt - ett + Lap(e)
                    call ogDeriv(ep, 0, 2,0,0, xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.,t,ex, uxx)
                    call ogDeriv(ep, 0, 0,2,0, xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.,t,ex, uyy)
                    utt00=uxx+uyy
                    call ogDeriv(ep, 0, 2,0,0, xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.,t,ey, vxx)
                    call ogDeriv(ep, 0, 0,2,0, xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.,t,ey, vyy)
                    vtt00=vxx+vyy
                   tau1DotUtt = tau1*utt00+tau2*vtt00
                    call ogf2d(ep,xy(i1,i2,i3,0),xy(i1,i2,i3,1),t,uv0(
     & 0),uv0(1),uv0(2))
                    call ogf2d(ep,xy(i1-is1,i2-is2,i3,0),xy(i1-is1,i2-
     & is2,i3,1),t,uvm(0),uvm(1),uvm(2))
                    call ogf2d(ep,xy(i1+is1,i2+is2,i3,0),xy(i1+is1,i2+
     & is2,i3,1),t,uvp(0),uvp(1),uvp(2))
                    call ogf2d(ep,xy(i1-2*is1,i2-2*is2,i3,0),xy(i1-2*
     & is1,i2-2*is2,i3,1),t,uvm2(0),uvm2(1),uvm2(2))
                    call ogf2d(ep,xy(i1+2*is1,i2+2*is2,i3,0),xy(i1+2*
     & is1,i2+2*is2,i3,1),t,uvp2(0),uvp2(1),uvp2(2))
                   ! Da1DotU = (a1.uv).r to 4th order
                   Da1DotU = (8.*( (a11p1*uvp(0)+a12p1*uvp(1)) - (
     & a11m1*uvm(0)+a12m1*uvm(1)) )- ( (a11p2*uvp2(0)+a12p2*uvp2(1)) -
     &  (a11m2*uvm2(0)+a12m2*uvm2(1)) ) )/(12.*dra)
                  ! for now remove the error in the extrapolation ************
                  ! gIVf = tau1*( uvm2(0)-4.*uvm(0)+6.*uv0(0)-4.*uvp(0)+uvp2(0)) +!        tau2*( uvm2(1)-4.*uvm(1)+6.*uv0(1)-4.*uvp(1)+uvp2(1))
                  gIVf=0.
                 !  a21zp1= A21(i1+js1,i2+js2,i3) 
                 !  a21zm1= A21(i1-js1,i2-js2,i3) 
                 !  a21zp2= A21(i1+2*js1,i2+2*js2,i3) 
                 !  a21zm2= A21(i1-2*js1,i2-2*js2,i3) 
                 !
                 !  a22zp1= A22(i1+js1,i2+js2,i3) 
                 !  a22zm1= A22(i1-js1,i2-js2,i3) 
                 !  a22zp2= A22(i1+2*js1,i2+2*js2,i3) 
                 !  a22zm2= A22(i1-2*js1,i2-2*js2,i3) 
                 !
                 !  ! *** set to - Ds( a2.uv )
                 !  Da1DotU = -(  !       ( 8.*(a21zp1*u(i1+  js1,i2+  js2,i3,ex)-a21zm1*u(i1-  js1,i2-  js2,i3,ex)) !           -(a21zp2*u(i1+2*js1,i2+2*js2,i3,ex)-a21zm2*u(i1-2*js1,i2-2*js2,i3,ex)) )/(12.*dsa) !      +( 8.*(a22zp1*u(i1+  js1,i2+  js2,i3,ey)-a22zm1*u(i1-  js1,i2-  js2,i3,ey)) !           -(a22zp2*u(i1+2*js1,i2+2*js2,i3,ey)-a22zm2*u(i1-2*js1,i2-2*js2,i3,ey)) )/(12.*dsa)  )
                  ! ***** Forcing for Hz ******
                  ! (w).r = fw1                              (w.n = 0 )
                  ! (c11*w.rr + c22*w.ss + ... ).r = fw2     ( (Delta w).n = 0 )
                  fw1= (8.*(uvp(2)-uvm(2))-(uvp2(2)-uvm2(2)))/(12.*dra)
                  wrrr=(uvp2(2)-2.*(uvp(2)-uvm(2))-uvm2(2))/(2.*dra**3)
                  ! wrr=(uvp(2)-2.*uv0(2)+uvm(2))/(dra**2)
                  wrr=(-30.*uv0(2)+16.*(uvp(2)+uvm(2))-(uvp2(2)+uvm2(2)
     & ) )/(12.*dra**2)
                  ! wr=(uvp(2)-uvm(2))/(2.*dra)
                  wr=(8.*(uvp(2)-uvm(2))-(uvp2(2)-uvm2(2)))/(12.*dra)
                  fw2= c11*wrrr + (c1+c11r)*wrr + c1r*wr
                  ! for tangential derivatives:
                   call ogf2d(ep,xy(i1-js1,i2-js2,i3,0),xy(i1-js1,i2-
     & js2,i3,1),t,uvm(0),uvm(1),uvm(2))
                   call ogf2d(ep,xy(i1+js1,i2+js2,i3,0),xy(i1+js1,i2+
     & js2,i3,1),t,uvp(0),uvp(1),uvp(2))
                   call ogf2d(ep,xy(i1-2*js1,i2-2*js2,i3,0),xy(i1-2*
     & js1,i2-2*js2,i3,1),t,uvm2(0),uvm2(1),uvm2(2))
                   call ogf2d(ep,xy(i1+2*js1,i2+2*js2,i3,0),xy(i1+2*
     & js1,i2+2*js2,i3,1),t,uvp2(0),uvp2(1),uvp2(2))
                  ! These approximations should be consistent with the approximations for ws and wss above
                  ! fw2=fw2 + c22r*(uvp(2)-2.*uv0(2)+uvm(2))/(dsa**2)+c2r*(uvp(2)-uvm(2))/(2.*dsa)
                  fw2=fw2 + c22r*(-30.*uv0(2)+16.*(uvp(2)+uvm(2))-(
     & uvp2(2)+uvm2(2)) )/(12.*dsa**2)+ c2r*(8.*(uvp(2)-uvm(2))-(uvp2(
     & 2)-uvm2(2)))/(12.*dsa)
                 ! assign values using extrapolation of the normal component:


! ************ Answer *******************
      gIII=-tau1*(c22*uss+c2*us)-tau2*(c22*vss+c2*vs)

      tauU=tau1*u(i1,i2,i3,ex)+tau2*u(i1,i2,i3,ey)

      tauUp1=tau1*u(i1+is1,i2+is2,i3+is3,ex)+tau2*u(i1+is1,i2+is2,i3+
     & is3,ey)

      tauUp2=tau1*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)+tau2*u(i1+2*is1,i2+
     & 2*is2,i3+2*is3,ey)

      gIV=-6*tauU+4*tauUp1-tauUp2 +gIVf

      ttu1=1/4.*(16*c11*tauUp1-30*c11*tauU-c11*tauUp2-c11*gIV+8*c1*dra*
     & tauUp1-c1*dra*tauUp2+c1*dra*gIV-12*gIII*dra**2-12*tau1DotUtt*
     & dra**2)/(-3*c11+c1*dra)
      ttu2=(16*c11*tauUp1-30*c11*tauU-c11*tauUp2-4*c11*gIV+8*c1*dra*
     & tauUp1-c1*dra*tauUp2+2*c1*dra*gIV-12*gIII*dra**2-12*tau1DotUtt*
     & dra**2)/(-3*c11+c1*dra)

      u(i1-is1,i2-is2,i3-is3,ex) = -1.*(-12.*a12*tau1*tau2*Da1DotU*dra+
     & 12.*tau2**2*a11*Da1DotU*dra-1.*tau2**2*a11*u(i1+3*is1,i2+3*is2,
     & i3+3*is3,ex)*a11m2+tau2*a12*u(i1+3*is1,i2+3*is2,i3+3*is3,ey)*
     & a12m2*tau1-1.*tau2**2*a12*u(i1+3*is1,i2+3*is2,i3+3*is3,ey)*
     & a11m2+tau2*a11*u(i1+3*is1,i2+3*is2,i3+3*is3,ex)*a12m2*tau1-5.*
     & tau2*a12*u(i1+2*is1,i2+2*is2,i3+2*is3,ey)*a12m2*tau1-1.*a12*
     & tau1*tau2*a12p2*u(i1+2*is1,i2+2*is2,i3+2*is3,ey)+5.*tau2**2*
     & a12*u(i1+2*is1,i2+2*is2,i3+2*is3,ey)*a11m2+tau2**2*a11*a12p2*u(
     & i1+2*is1,i2+2*is2,i3+2*is3,ey)-5.*tau2*a11*u(i1+2*is1,i2+2*is2,
     & i3+2*is3,ex)*a12m2*tau1+5.*tau2**2*a11*u(i1+2*is1,i2+2*is2,i3+
     & 2*is3,ex)*a11m2-1.*a12*tau1*tau2*a11p2*u(i1+2*is1,i2+2*is2,i3+
     & 2*is3,ex)+tau2**2*a11*a11p2*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)-
     & 8.*a12*tau1*a12m1*ttu1+8.*tau2*a11*a12m1*ttu1-1.*tau2*a11*
     & a12m2*ttu2+5.*a12*ttu1*a12m2*tau1-5.*a12*ttu1*tau2*a11m2+a12*
     & ttu2*tau2*a11m2-10.*tau2*a12*u(i1,i2,i3,ey)*a12m2*tau1+10.*
     & tau2**2*a12*u(i1,i2,i3,ey)*a11m2+10.*tau2**2*a11*u(i1,i2,i3,ex)
     & *a11m2-10.*tau2*a11*u(i1,i2,i3,ex)*a12m2*tau1+8.*a12*tau1*tau2*
     & a11p1*u(i1+is1,i2+is2,i3+is3,ex)-8.*tau2**2*a11*a11p1*u(i1+is1,
     & i2+is2,i3+is3,ex)+10.*tau2*a11*u(i1+is1,i2+is2,i3+is3,ex)*
     & a12m2*tau1-10.*tau2**2*a11*u(i1+is1,i2+is2,i3+is3,ex)*a11m2+8.*
     & a12*tau1*tau2*a12p1*u(i1+is1,i2+is2,i3+is3,ey)-10.*tau2**2*a12*
     & u(i1+is1,i2+is2,i3+is3,ey)*a11m2-8.*tau2**2*a11*a12p1*u(i1+is1,
     & i2+is2,i3+is3,ey)+10.*tau2*a12*u(i1+is1,i2+is2,i3+is3,ey)*
     & a12m2*tau1)/(5.*tau2*a11*a12m2*tau1-5.*tau2**2*a11*a11m2-5.*
     & a12*tau1**2*a12m2+5.*a12*tau1*tau2*a11m2-8.*tau2*a11*a12m1*
     & tau1-8.*a12*tau1*tau2*a11m1+8.*tau2**2*a11*a11m1+8.*a12*tau1**
     & 2*a12m1)

      u(i1-is1,i2-is2,i3-is3,ey) = -1.*(8.*tau1*a11m1*a12*ttu1-8.*ttu1*
     & tau2*a11*a11m1+5.*ttu1*tau2*a11*a11m2-1.*tau1*a12*ttu2*a11m2+
     & tau1*a11*a12m2*ttu2-5.*ttu1*a11*a12m2*tau1+10.*a11*u(i1,i2,i3,
     & ex)*a12m2*tau1**2-10.*tau1*tau2*a11*u(i1,i2,i3,ex)*a11m2-10.*
     & a12*u(i1+is1,i2+is2,i3+is3,ey)*a12m2*tau1**2-8.*a12*tau1**2*
     & a12p1*u(i1+is1,i2+is2,i3+is3,ey)+8.*tau1*tau2*a11*a12p1*u(i1+
     & is1,i2+is2,i3+is3,ey)+10.*tau1*tau2*a12*u(i1+is1,i2+is2,i3+is3,
     & ey)*a11m2-10.*a11*u(i1+is1,i2+is2,i3+is3,ex)*a12m2*tau1**2+10.*
     & tau1*tau2*a11*u(i1+is1,i2+is2,i3+is3,ex)*a11m2+8.*tau1*tau2*
     & a11*a11p1*u(i1+is1,i2+is2,i3+is3,ex)-8.*a12*tau1**2*a11p1*u(i1+
     & is1,i2+is2,i3+is3,ex)-10.*tau1*tau2*a12*u(i1,i2,i3,ey)*a11m2+
     & 10.*a12*u(i1,i2,i3,ey)*a12m2*tau1**2-5.*tau1*tau2*a11*u(i1+2*
     & is1,i2+2*is2,i3+2*is3,ex)*a11m2-1.*tau1*tau2*a11*a11p2*u(i1+2*
     & is1,i2+2*is2,i3+2*is3,ex)+a12*tau1**2*a11p2*u(i1+2*is1,i2+2*
     & is2,i3+2*is3,ex)+5.*a11*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)*a12m2*
     & tau1**2+a12*tau1**2*a12p2*u(i1+2*is1,i2+2*is2,i3+2*is3,ey)+5.*
     & a12*u(i1+2*is1,i2+2*is2,i3+2*is3,ey)*a12m2*tau1**2-1.*tau1*
     & tau2*a11*a12p2*u(i1+2*is1,i2+2*is2,i3+2*is3,ey)-5.*tau1*tau2*
     & a12*u(i1+2*is1,i2+2*is2,i3+2*is3,ey)*a11m2-1.*a11*u(i1+3*is1,
     & i2+3*is2,i3+3*is3,ex)*a12m2*tau1**2+tau1*tau2*a11*u(i1+3*is1,
     & i2+3*is2,i3+3*is3,ex)*a11m2+tau1*tau2*a12*u(i1+3*is1,i2+3*is2,
     & i3+3*is3,ey)*a11m2-1.*a12*u(i1+3*is1,i2+3*is2,i3+3*is3,ey)*
     & a12m2*tau1**2+12.*a12*tau1**2*Da1DotU*dra-12.*tau1*tau2*a11*
     & Da1DotU*dra)/(5.*tau2*a11*a12m2*tau1-5.*tau2**2*a11*a11m2-5.*
     & a12*tau1**2*a12m2+5.*a12*tau1*tau2*a11m2-8.*tau2*a11*a12m1*
     & tau1-8.*a12*tau1*tau2*a11m1+8.*tau2**2*a11*a11m1+8.*a12*tau1**
     & 2*a12m1)

      u(i1-2*is1,i2-2*is2,i3-2*is3,ex) = -1.*(-40.*tau2*a12m1*tau1*a12*
     & u(i1+2*is1,i2+2*is2,i3+2*is3,ey)-40.*tau2*a11m1*a12*ttu1-80.*
     & tau2**2*a11m1*a12*u(i1+is1,i2+is2,i3+is3,ey)-5.*a12*tau1*tau2*
     & a12p2*u(i1+2*is1,i2+2*is2,i3+2*is3,ey)+5.*tau2**2*a11*a12p2*u(
     & i1+2*is1,i2+2*is2,i3+2*is3,ey)-60.*a12*tau1*tau2*Da1DotU*dra+
     & 60.*tau2**2*a11*Da1DotU*dra-8.*tau2**2*a11m1*a11*u(i1+3*is1,i2+
     & 3*is2,i3+3*is3,ex)-8.*tau2**2*a11m1*a12*u(i1+3*is1,i2+3*is2,i3+
     & 3*is3,ey)+8.*tau2*a12m1*tau1*a12*u(i1+3*is1,i2+3*is2,i3+3*is3,
     & ey)+8.*tau2*a12m1*tau1*a11*u(i1+3*is1,i2+3*is2,i3+3*is3,ex)+
     & 40.*tau2**2*a11m1*a11*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)+5.*tau2*
     & *2*a11*a11p2*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)+40.*tau2**2*
     & a11m1*a12*u(i1+2*is1,i2+2*is2,i3+2*is3,ey)-40.*tau2*a12m1*tau1*
     & a11*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)-5.*a12*tau1*tau2*a11p2*u(
     & i1+2*is1,i2+2*is2,i3+2*is3,ex)-8.*a12m1*tau1*a12*ttu2+40.*tau2*
     & a11*a12m1*ttu1+8.*tau2*a11m1*a12*ttu2-5.*tau2*a11*a12m2*ttu2+
     & 5.*a12*ttu2*a12m2*tau1+80.*tau2**2*a11m1*a12*u(i1,i2,i3,ey)-
     & 80.*tau2*a12m1*tau1*a11*u(i1,i2,i3,ex)+80.*tau2**2*a11m1*a11*u(
     & i1,i2,i3,ex)-80.*tau2**2*a11m1*a11*u(i1+is1,i2+is2,i3+is3,ex)-
     & 80.*tau2*a12m1*tau1*a12*u(i1,i2,i3,ey)+40.*a12*tau1*tau2*a12p1*
     & u(i1+is1,i2+is2,i3+is3,ey)+80.*tau2*a12m1*tau1*a12*u(i1+is1,i2+
     & is2,i3+is3,ey)-40.*tau2**2*a11*a12p1*u(i1+is1,i2+is2,i3+is3,ey)
     & +80.*tau2*a12m1*tau1*a11*u(i1+is1,i2+is2,i3+is3,ex)+40.*a12*
     & tau1*tau2*a11p1*u(i1+is1,i2+is2,i3+is3,ex)-40.*tau2**2*a11*
     & a11p1*u(i1+is1,i2+is2,i3+is3,ex))/(5.*tau2*a11*a12m2*tau1-5.*
     & tau2**2*a11*a11m2-5.*a12*tau1**2*a12m2+5.*a12*tau1*tau2*a11m2-
     & 8.*tau2*a11*a12m1*tau1-8.*a12*tau1*tau2*a11m1+8.*tau2**2*a11*
     & a11m1+8.*a12*tau1**2*a12m1)

      u(i1-2*is1,i2-2*is2,i3-2*is3,ey) = (40.*tau1*a11*a12m1*ttu1-8.*
     & ttu2*a11*a12m1*tau1-40.*tau1*a11m1*a12*ttu1+8.*ttu2*tau2*a11*
     & a11m1+5.*tau1*a12*ttu2*a11m2-5.*ttu2*tau2*a11*a11m2-40.*tau1*
     & tau2*a11*a12p1*u(i1+is1,i2+is2,i3+is3,ey)+40.*a12*tau1**2*
     & a12p1*u(i1+is1,i2+is2,i3+is3,ey)-80.*tau1*tau2*a11m1*a12*u(i1+
     & is1,i2+is2,i3+is3,ey)-80.*tau1*tau2*a11m1*a11*u(i1+is1,i2+is2,
     & i3+is3,ex)-40.*tau1*tau2*a11*a11p1*u(i1+is1,i2+is2,i3+is3,ex)+
     & 80.*a12m1*tau1**2*a12*u(i1+is1,i2+is2,i3+is3,ey)+80.*a12m1*
     & tau1**2*a11*u(i1+is1,i2+is2,i3+is3,ex)+40.*a12*tau1**2*a11p1*u(
     & i1+is1,i2+is2,i3+is3,ex)+80.*tau1*tau2*a11m1*a12*u(i1,i2,i3,ey)
     & -80.*a12m1*tau1**2*a12*u(i1,i2,i3,ey)+80.*tau1*tau2*a11m1*a11*
     & u(i1,i2,i3,ex)-80.*a12m1*tau1**2*a11*u(i1,i2,i3,ex)+5.*tau1*
     & tau2*a11*a11p2*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)-5.*a12*tau1**2*
     & a11p2*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)+40.*tau1*tau2*a11m1*a11*
     & u(i1+2*is1,i2+2*is2,i3+2*is3,ex)-40.*a12m1*tau1**2*a11*u(i1+2*
     & is1,i2+2*is2,i3+2*is3,ex)-60.*a12*tau1**2*Da1DotU*dra+60.*tau1*
     & tau2*a11*Da1DotU*dra+40.*tau1*tau2*a11m1*a12*u(i1+2*is1,i2+2*
     & is2,i3+2*is3,ey)+5.*tau1*tau2*a11*a12p2*u(i1+2*is1,i2+2*is2,i3+
     & 2*is3,ey)-5.*a12*tau1**2*a12p2*u(i1+2*is1,i2+2*is2,i3+2*is3,ey)
     & -40.*a12m1*tau1**2*a12*u(i1+2*is1,i2+2*is2,i3+2*is3,ey)-8.*
     & tau1*tau2*a11m1*a12*u(i1+3*is1,i2+3*is2,i3+3*is3,ey)+8.*a12m1*
     & tau1**2*a12*u(i1+3*is1,i2+3*is2,i3+3*is3,ey)-8.*tau1*tau2*
     & a11m1*a11*u(i1+3*is1,i2+3*is2,i3+3*is3,ex)+8.*a12m1*tau1**2*
     & a11*u(i1+3*is1,i2+3*is2,i3+3*is3,ex))/(5.*tau2*a11*a12m2*tau1-
     & 5.*tau2**2*a11*a11m2-5.*a12*tau1**2*a12m2+5.*a12*tau1*tau2*
     & a11m2-8.*tau2*a11*a12m1*tau1-8.*a12*tau1*tau2*a11m1+8.*tau2**2*
     & a11*a11m1+8.*a12*tau1**2*a12m1)


 ! *********** done *********************
                 ! Now assign Hz at the ghost points


! ************ Hz Answer *******************
      cw2=c1+c11r
      cw1=c1r
      bfw2=c22r*wss+c2r*ws-fw2

      u(i1-is1,i2-is2,i3,hz) = 1/2.*(-18*c11*u(i1+is1,i2+is2,i3,hz)+36*
     & c11*fw1*dra-12*cw2*dra*u(i1+is1,i2+is2,i3,hz)+15*cw2*dra*u(i1,
     & i2,i3,hz)+cw2*dra*u(i1+2*is1,i2+2*is2,i3,hz)+6*cw2*dra**2*fw1-
     & 6*cw1*dra**3*fw1-6*bfw2*dra**3)/(-9*c11+2*cw2*dra)

      u(i1-2*is1,i2-2*is2,i3,hz) = (-64*cw2*dra*u(i1+is1,i2+is2,i3,hz)+
     & 36*c11*fw1*dra+60*cw2*dra*u(i1,i2,i3,hz)+6*cw2*dra*u(i1+2*is1,
     & i2+2*is2,i3,hz)+48*cw2*dra**2*fw1-24*cw1*dra**3*fw1-24*bfw2*
     & dra**3-9*c11*u(i1+2*is1,i2+2*is2,i3,hz))/(-9*c11+2*cw2*dra)


 ! *********** Hz done *********************
                  if( debug.gt.0 )then
                   write(*,'(" ghost-interp: i=",3i3," ex=",e10.2," 
     & assign i=",3i3," ex=",e10.2," i=",3i3," ex=",e10.2)')i1,i2,i3,
     & u(i1,i2,i3,ex),i1-is1,i2-is2,i3-is3,u(i1-is1,i2-is2,i3-is3,ex),
     & i1-2*is1,i2-2*is2,i3-2*is3,u(i1-2*is1,i2-2*is2,i3-2*is3,ex)
                   det = (5.*tau2*a11*a12m2*tau1-5.*tau2**2*a11*a11m2-
     & 5.*a12*tau1**2*a12m2+5.*a12*tau1*tau2*a11m2-8.*tau2*a11*a12m1*
     & tau1-8.*a12*tau1*tau2*a11m1+8.*tau2**2*a11*a11m1+8.*a12*tau1**
     & 2*a12m1)
                   write(*,'(" ghost-interp: det=",e10.2," tau1,tau2,
     & a11,a11m1,a11m2,a12,a12m1,a12m2=",10f8.4)') det,tau1,tau2,a11,
     & a11m1,a11m2,a12,a12m1,a12m2
                   write(*,'(" ghost-interp: gIII,tauU,tauUp1,tauUp2,
     & gIV,ttu1,ttu2,c11,c1,dra=",10f8.3)') gIII,tauU,tauUp1,tauUp2,
     & gIV,ttu1,ttu2,c11,c1,dra
                   write(*,'(" ghost-interp: c1r,c2r,c22,uss,c2,us,vss,
     & vs=",10e10.2)') c1r,c2r,c22,uss,c2,us,vss,vs
                    call ogf2d(ep,xy(i1-is1,i2-is2,i3,0),xy(i1-is1,i2-
     & is2,i3,1),t,uvm(0),uvm(1),uvm(2))
                    call ogf2d(ep,xy(i1-2*is1,i2-2*is2,i3,0),xy(i1-2*
     & is1,i2-2*is2,i3,1),t,uvm2(0),uvm2(1),uvm2(2))
                   write(*,'(" .............tau1DotUtt,Da1DotU,us,
     & uss=",4e11.3)') tau1DotUtt,Da1DotU,us,uss
                   write(*,'(" .............err: ex(-1,-2)=",2e10.3,", 
     & ey(-1,-2)=",2e10.2,", hz(-1,-2)=",2e10.2)') u(i1-is1,i2-is2,i3-
     & is3,ex)-uvm(0),u(i1-2*is1,i2-2*is2,i3-2*is3,ex)-uvm2(0),u(i1-
     & is1,i2-is2,i3-is3,ey)-uvm(1),u(i1-2*is1,i2-2*is2,i3-2*is3,ey)-
     & uvm2(1),u(i1-is1,i2-is2,i3-is3,hz)-uvm(2),u(i1-2*is1,i2-2*is2,
     & i3-2*is3,hz)-uvm2(2)
                  end if
                  ! ** NO NEED TO DO ALL THE ABOVE IF WE DO THIS:
                  extrapInterpGhost=.true.
                  if( extrapInterpGhost )then
                    ! extrapolate ghost points next to boundary interpolation points  *wdh* 2015/05/30 
                    write(*,'(" extrap ghost next to interp")')
                    u(i1-is1,i2-is2,i3-is3,ex) = (5.*u(i1,i2,i3,ex)-
     & 10.*u(i1+is1,i2+is2,i3+is3,ex)+10.*u(i1+2*is1,i2+2*is2,i3+2*
     & is3,ex)-5.*u(i1+3*is1,i2+3*is2,i3+3*is3,ex)+u(i1+4*is1,i2+4*
     & is2,i3+4*is3,ex))
                    u(i1-is1,i2-is2,i3-is3,ey) = (5.*u(i1,i2,i3,ey)-
     & 10.*u(i1+is1,i2+is2,i3+is3,ey)+10.*u(i1+2*is1,i2+2*is2,i3+2*
     & is3,ey)-5.*u(i1+3*is1,i2+3*is2,i3+3*is3,ey)+u(i1+4*is1,i2+4*
     & is2,i3+4*is3,ey))
                    u(i1-is1,i2-is2,i3-is3,hz) = (5.*u(i1,i2,i3,hz)-
     & 10.*u(i1+is1,i2+is2,i3+is3,hz)+10.*u(i1+2*is1,i2+2*is2,i3+2*
     & is3,hz)-5.*u(i1+3*is1,i2+3*is2,i3+3*is3,hz)+u(i1+4*is1,i2+4*
     & is2,i3+4*is3,hz))
                    u(i1-2*is1,i2-2*is2,i3-2*is3,ex) = (5.*u(i1-is1,i2-
     & is2,i3-is3,ex)-10.*u(i1-is1+is1,i2-is2+is2,i3-is3+is3,ex)+10.*
     & u(i1-is1+2*is1,i2-is2+2*is2,i3-is3+2*is3,ex)-5.*u(i1-is1+3*is1,
     & i2-is2+3*is2,i3-is3+3*is3,ex)+u(i1-is1+4*is1,i2-is2+4*is2,i3-
     & is3+4*is3,ex))
                    u(i1-2*is1,i2-2*is2,i3-2*is3,ey) = (5.*u(i1-is1,i2-
     & is2,i3-is3,ey)-10.*u(i1-is1+is1,i2-is2+is2,i3-is3+is3,ey)+10.*
     & u(i1-is1+2*is1,i2-is2+2*is2,i3-is3+2*is3,ey)-5.*u(i1-is1+3*is1,
     & i2-is2+3*is2,i3-is3+3*is3,ey)+u(i1-is1+4*is1,i2-is2+4*is2,i3-
     & is3+4*is3,ey))
                    u(i1-2*is1,i2-2*is2,i3-2*is3,hz) = (5.*u(i1-is1,i2-
     & is2,i3-is3,hz)-10.*u(i1-is1+is1,i2-is2+is2,i3-is3+is3,hz)+10.*
     & u(i1-is1+2*is1,i2-is2+2*is2,i3-is3+2*is3,hz)-5.*u(i1-is1+3*is1,
     & i2-is2+3*is2,i3-is3+3*is3,hz)+u(i1-is1+4*is1,i2-is2+4*is2,i3-
     & is3+4*is3,hz))
                  end if
                 end if ! mask>0
                 end do
                 end do
                 end do
                 if( debug.gt.0 )then
                 ! ============================DEBUG=======================================================
                 ! **** check that we satisfy all the equations ****
                 maxDivc=0.
                 maxTauDotLapu=0.
                 maxExtrap=0.
                 maxDr3aDotU=0.
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                 if( mask(i1,i2,i3).gt.0 )then
                  tau1=rsxy(i1,i2,i3,axisp1,0)
                  tau2=rsxy(i1,i2,i3,axisp1,1)
                  tauU=tau1*u(i1,i2,i3,ex)+tau2*u(i1,i2,i3,ey)
                  div = ux42(i1,i2,i3,ex)+uy42(i1,i2,i3,ey)
                  a11= (rsxy(i1,i2,i3,axis,0)/(rx(i1,i2,i3)*sy(i1,i2,
     & i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))
                  a12= (rsxy(i1,i2,i3,axis,1)/(rx(i1,i2,i3)*sy(i1,i2,
     & i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))
                  jac=1./(rx(i1,i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,
     & i2,i3))
                  a21 =rsxy(i1,i2,i3,axisp1,0)*jac
                  a22 =rsxy(i1,i2,i3,axisp1,1)*jac
                  a11p1= (rsxy(i1+is1,i2+is2,i3,axis,0)/(rx(i1+is1,i2+
     & is2,i3)*sy(i1+is1,i2+is2,i3)-ry(i1+is1,i2+is2,i3)*sx(i1+is1,i2+
     & is2,i3)))
                  a11m1= (rsxy(i1-is1,i2-is2,i3,axis,0)/(rx(i1-is1,i2-
     & is2,i3)*sy(i1-is1,i2-is2,i3)-ry(i1-is1,i2-is2,i3)*sx(i1-is1,i2-
     & is2,i3)))
                  a11p2= (rsxy(i1+2*is1,i2+2*is2,i3,axis,0)/(rx(i1+2*
     & is1,i2+2*is2,i3)*sy(i1+2*is1,i2+2*is2,i3)-ry(i1+2*is1,i2+2*is2,
     & i3)*sx(i1+2*is1,i2+2*is2,i3)))
                  a11m2= (rsxy(i1-2*is1,i2-2*is2,i3,axis,0)/(rx(i1-2*
     & is1,i2-2*is2,i3)*sy(i1-2*is1,i2-2*is2,i3)-ry(i1-2*is1,i2-2*is2,
     & i3)*sx(i1-2*is1,i2-2*is2,i3)))
                  a12p1= (rsxy(i1+is1,i2+is2,i3,axis,1)/(rx(i1+is1,i2+
     & is2,i3)*sy(i1+is1,i2+is2,i3)-ry(i1+is1,i2+is2,i3)*sx(i1+is1,i2+
     & is2,i3)))
                  a12m1= (rsxy(i1-is1,i2-is2,i3,axis,1)/(rx(i1-is1,i2-
     & is2,i3)*sy(i1-is1,i2-is2,i3)-ry(i1-is1,i2-is2,i3)*sx(i1-is1,i2-
     & is2,i3)))
                  a12p2= (rsxy(i1+2*is1,i2+2*is2,i3,axis,1)/(rx(i1+2*
     & is1,i2+2*is2,i3)*sy(i1+2*is1,i2+2*is2,i3)-ry(i1+2*is1,i2+2*is2,
     & i3)*sx(i1+2*is1,i2+2*is2,i3)))
                  a12m2= (rsxy(i1-2*is1,i2-2*is2,i3,axis,1)/(rx(i1-2*
     & is1,i2-2*is2,i3)*sy(i1-2*is1,i2-2*is2,i3)-ry(i1-2*is1,i2-2*is2,
     & i3)*sx(i1-2*is1,i2-2*is2,i3)))
                  a21zp1= (rsxy(i1+js1,i2+js2,i3,axisp1,0)/(rx(i1+js1,
     & i2+js2,i3)*sy(i1+js1,i2+js2,i3)-ry(i1+js1,i2+js2,i3)*sx(i1+js1,
     & i2+js2,i3)))
                  a21zm1= (rsxy(i1-js1,i2-js2,i3,axisp1,0)/(rx(i1-js1,
     & i2-js2,i3)*sy(i1-js1,i2-js2,i3)-ry(i1-js1,i2-js2,i3)*sx(i1-js1,
     & i2-js2,i3)))
                  a21zp2= (rsxy(i1+2*js1,i2+2*js2,i3,axisp1,0)/(rx(i1+
     & 2*js1,i2+2*js2,i3)*sy(i1+2*js1,i2+2*js2,i3)-ry(i1+2*js1,i2+2*
     & js2,i3)*sx(i1+2*js1,i2+2*js2,i3)))
                  a21zm2= (rsxy(i1-2*js1,i2-2*js2,i3,axisp1,0)/(rx(i1-
     & 2*js1,i2-2*js2,i3)*sy(i1-2*js1,i2-2*js2,i3)-ry(i1-2*js1,i2-2*
     & js2,i3)*sx(i1-2*js1,i2-2*js2,i3)))
                  a22zp1= (rsxy(i1+js1,i2+js2,i3,axisp1,1)/(rx(i1+js1,
     & i2+js2,i3)*sy(i1+js1,i2+js2,i3)-ry(i1+js1,i2+js2,i3)*sx(i1+js1,
     & i2+js2,i3)))
                  a22zm1= (rsxy(i1-js1,i2-js2,i3,axisp1,1)/(rx(i1-js1,
     & i2-js2,i3)*sy(i1-js1,i2-js2,i3)-ry(i1-js1,i2-js2,i3)*sx(i1-js1,
     & i2-js2,i3)))
                  a22zp2= (rsxy(i1+2*js1,i2+2*js2,i3,axisp1,1)/(rx(i1+
     & 2*js1,i2+2*js2,i3)*sy(i1+2*js1,i2+2*js2,i3)-ry(i1+2*js1,i2+2*
     & js2,i3)*sx(i1+2*js1,i2+2*js2,i3)))
                  a22zm2= (rsxy(i1-2*js1,i2-2*js2,i3,axisp1,1)/(rx(i1-
     & 2*js1,i2-2*js2,i3)*sy(i1-2*js1,i2-2*js2,i3)-ry(i1-2*js1,i2-2*
     & js2,i3)*sx(i1-2*js1,i2-2*js2,i3)))
                  divc= ( 8.*(a11p1*u(i1+  is1,i2+  is2,i3,ex)-a11m1*u(
     & i1-  is1,i2-  is2,i3,ex)) -(a11p2*u(i1+2*is1,i2+2*is2,i3,ex)-
     & a11m2*u(i1-2*is1,i2-2*is2,i3,ex)) )/(12.*dra) +( 8.*(a12p1*u(
     & i1+  is1,i2+  is2,i3,ey)-a12m1*u(i1-  is1,i2-  is2,i3,ey)) -(
     & a12p2*u(i1+2*is1,i2+2*is2,i3,ey)-a12m2*u(i1-2*is1,i2-2*is2,i3,
     & ey)) )/(12.*dra) +( 8.*(a21zp1*u(i1+  js1,i2+  js2,i3,ex)-
     & a21zm1*u(i1-  js1,i2-  js2,i3,ex)) -(a21zp2*u(i1+2*js1,i2+2*
     & js2,i3,ex)-a21zm2*u(i1-2*js1,i2-2*js2,i3,ex)) )/(12.*dsa) +( 
     & 8.*(a22zp1*u(i1+  js1,i2+  js2,i3,ey)-a22zm1*u(i1-  js1,i2-  
     & js2,i3,ey)) -(a22zp2*u(i1+2*js1,i2+2*js2,i3,ey)-a22zm2*u(i1-2*
     & js1,i2-2*js2,i3,ey)) )/(12.*dsa)
                  divc=divc*(rx(i1,i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)*sx(
     & i1,i2,i3))
                  divc2= (a11p1*u(i1+  is1,i2+  is2,i3,ex)-a11m1*u(i1- 
     &  is1,i2-  is2,i3,ex))/(2.*dra) +(a12p1*u(i1+  is1,i2+  is2,i3,
     & ey)-a12m1*u(i1-  is1,i2-  is2,i3,ey))/(2.*dra) +(a21zp1*u(i1+  
     & js1,i2+  js2,i3,ex)-a21zm1*u(i1-  js1,i2-  js2,i3,ex))/(2.*dsa)
     &  +(a22zp1*u(i1+  js1,i2+  js2,i3,ey)-a22zm1*u(i1-  js1,i2-  
     & js2,i3,ey))/(2.*dsa)
                  divc2=divc2*(rx(i1,i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)*
     & sx(i1,i2,i3))
                  tauUp1=tau1*(u(i1-2*is1,i2-2*is2,i3,ex)-4.*u(i1-  
     & is1,i2-  is2,i3,ex)+6.*u(i1,i2,i3,ex)-4.*u(i1+  is1,i2+  is2,
     & i3,ex)+u(i1+2*is1,i2+2*is2,i3,ex))+tau2*(u(i1-2*is1,i2-2*is2,
     & i3,ey)-4.*u(i1-  is1,i2-  is2,i3,ey)+6.*u(i1,i2,i3,ey)-4.*u(i1+
     &   is1,i2+  is2,i3,ey)+u(i1+2*is1,i2+2*is2,i3,ey))
                  tauDotLap= tau1*ulaplacian42(i1,i2,i3,ex)+tau2*
     & ulaplacian42(i1,i2,i3,ey)
                  c11=(rsxy(i1,i2,i3,axis,0)**2+rsxy(i1,i2,i3,axis,1)**
     & 2)
                  c22=(rsxy(i1,i2,i3,axisp1,0)**2+rsxy(i1,i2,i3,axisp1,
     & 1)**2)
                  c1=(rsxyx42(i1,i2,i3,axis,0)+rsxyy42(i1,i2,i3,axis,1)
     & )
                  c2=(rsxyx42(i1,i2,i3,axisp1,0)+rsxyy42(i1,i2,i3,
     & axisp1,1))
                  errLapex=(c11*(-30.*u(i1,i2,i3,ex)+16.*(u(i1+is1,i2+
     & is2,i3+is3,ex)+u(i1-is1,i2-is2,i3-is3,ex))-(u(i1+2*is1,i2+2*
     & is2,i3+2*is3,ex)+u(i1-2*is1,i2-2*is2,i3-2*is3,ex)))/(12.*dra**
     & 2)+c22*(-30.*u(i1,i2,i3,ex)+16.*(u(i1+js1,i2+js2,i3+js3,ex)+u(
     & i1-js1,i2-js2,i3-js3,ex))-(u(i1+2*js1,i2+2*js2,i3+2*js3,ex)+u(
     & i1-2*js1,i2-2*js2,i3-2*js3,ex)))/(12.*dsa**2)+c1*(8.*(u(i1+is1,
     & i2+is2,i3+is3,ex)-u(i1-is1,i2-is2,i3-is3,ex))-(u(i1+2*is1,i2+2*
     & is2,i3+2*is3,ex)-u(i1-2*is1,i2-2*is2,i3-2*is3,ex)))/(12.*dra)+
     & c2*(8.*(u(i1+js1,i2+js2,i3+js3,ex)-u(i1-js1,i2-js2,i3-js3,ex))-
     & (u(i1+2*js1,i2+2*js2,i3+2*js3,ex)-u(i1-2*js1,i2-2*js2,i3-2*js3,
     & ex)))/(12.*dsa))-ulaplacian42(i1,i2,i3,ex)
                  errLapey=(c11*(-30.*u(i1,i2,i3,ey)+16.*(u(i1+is1,i2+
     & is2,i3+is3,ey)+u(i1-is1,i2-is2,i3-is3,ey))-(u(i1+2*is1,i2+2*
     & is2,i3+2*is3,ey)+u(i1-2*is1,i2-2*is2,i3-2*is3,ey)))/(12.*dra**
     & 2)+c22*(-30.*u(i1,i2,i3,ey)+16.*(u(i1+js1,i2+js2,i3+js3,ey)+u(
     & i1-js1,i2-js2,i3-js3,ey))-(u(i1+2*js1,i2+2*js2,i3+2*js3,ey)+u(
     & i1-2*js1,i2-2*js2,i3-2*js3,ey)))/(12.*dsa**2)+c1*(8.*(u(i1+is1,
     & i2+is2,i3+is3,ey)-u(i1-is1,i2-is2,i3-is3,ey))-(u(i1+2*is1,i2+2*
     & is2,i3+2*is3,ey)-u(i1-2*is1,i2-2*is2,i3-2*is3,ey)))/(12.*dra)+
     & c2*(8.*(u(i1+js1,i2+js2,i3+js3,ey)-u(i1-js1,i2-js2,i3-js3,ey))-
     & (u(i1+2*js1,i2+2*js2,i3+2*js3,ey)-u(i1-2*js1,i2-2*js2,i3-2*js3,
     & ey)))/(12.*dsa))-ulaplacian42(i1,i2,i3,ey)
                  ! f1 := Dzr(Dpr(Dmr( a11*u + a12*v )))(i1,i2,i3)/dra^3 - cur*Dzr(u)(i1,i2,i3)/dra - cvr*Dzr(v)(i1,i2,i3)/dra - gI:
                  ! 
                  uex=u(i1,i2,i3,ex)
                  uey=u(i1,i2,i3,ey)
                  ur=(8.*(u(i1+is1,i2+is2,i3+is3,ex)-u(i1-is1,i2-is2,
     & i3-is3,ex))-(u(i1+2*is1,i2+2*is2,i3+2*is3,ex)-u(i1-2*is1,i2-2*
     & is2,i3-2*is3,ex)))/(12.*dra)
                  vr=(8.*(u(i1+is1,i2+is2,i3+is3,ey)-u(i1-is1,i2-is2,
     & i3-is3,ey))-(u(i1+2*is1,i2+2*is2,i3+2*is3,ey)-u(i1-2*is1,i2-2*
     & is2,i3-2*is3,ey)))/(12.*dra)
                  us=(8.*(u(i1+js1,i2+js2,i3+js3,ex)-u(i1-js1,i2-js2,
     & i3-js3,ex))-(u(i1+2*js1,i2+2*js2,i3+2*js3,ex)-u(i1-2*js1,i2-2*
     & js2,i3-2*js3,ex)))/(12.*dsa)
                  vs=(8.*(u(i1+js1,i2+js2,i3+js3,ey)-u(i1-js1,i2-js2,
     & i3-js3,ey))-(u(i1+2*js1,i2+2*js2,i3+2*js3,ey)-u(i1-2*js1,i2-2*
     & js2,i3-2*js3,ey)))/(12.*dsa)
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
                  uss=(-30.*u(i1,i2,i3,ex)+16.*(u(i1+js1,i2+js2,i3+js3,
     & ex)+u(i1-js1,i2-js2,i3-js3,ex))-(u(i1+2*js1,i2+2*js2,i3+2*js3,
     & ex)+u(i1-2*js1,i2-2*js2,i3-2*js3,ex)))/(12.*dsa**2)
                  vss=(-30.*u(i1,i2,i3,ey)+16.*(u(i1+js1,i2+js2,i3+js3,
     & ey)+u(i1-js1,i2-js2,i3-js3,ey))-(u(i1+2*js1,i2+2*js2,i3+2*js3,
     & ey)+u(i1-2*js1,i2-2*js2,i3-2*js3,ey)))/(12.*dsa**2)
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
                  usss=(u(i1+2*js1,i2+2*js2,i3+2*js3,ex)-2.*u(i1+js1,
     & i2+js2,i3+js3,ex)+2.*u(i1-js1,i2-js2,i3-js3,ex)-u(i1-2*js1,i2-
     & 2*js2,i3-2*js3,ex))/(2.*dsa**3)
                  vsss=(u(i1+2*js1,i2+2*js2,i3+2*js3,ey)-2.*u(i1+js1,
     & i2+js2,i3+js3,ey)+2.*u(i1-js1,i2-js2,i3-js3,ey)-u(i1-2*js1,i2-
     & 2*js2,i3-2*js3,ey))/(2.*dsa**3)
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
                 !       ursm=-(a12**2*c2*vs+a12**2*c1*vr+a12**2*c22*vss+2*a22*a21s*us*c11-a21r*us*a12*c11-a22r*vs*a12*c11-a22s*vr*a12*c11+a22*a12s*vr*c11+a22*a12r*vs*c11+a22*a12rs*uey*c11+a22*a11s*ur*c11+a22*a11r*us*c11+a22*a11rs*uex*c11-a12rr*uey*a12*c11+a22*a21ss*uex*c11-2*a12r*vr*a12*c11-a21s*ur*a12*c11-a21rs*uex*a12*c11+a22**2*vss*c11+a11*a12*c1*ur+a11*a12*c2*us+a11*a12*c22*uss-2*a11r*ur*a12*c11-a11rr*uex*a12*c11+a22*a21*uss*c11-a22rs*uey*a12*c11+a22*a22ss*uey*c11+2*a22*a22s*vs*c11)/c11/(-a21*a12+a11*a22)
                 ! vrsm =(a21*a11s*ur+a21*a22*vss-a11*a21r*us-a11*a21s*ur-a11*a21rs*uex-a11*a12*vrr-2*a11*a12r*vr-a11*a12rr*uey-a11*a22r*vs-a11*a22s*vr-a11*a22rs*uey+a21*a11r*us+2*a21*a21s*us+a21*a12rs*uey+a21*a12r*vs-a11*a11rr*uex+a21*a12s*vr+a21*a11rs*uex-a11**2*urr+a21*a21ss*uex-2*a11*a11r*ur+a21**2*uss+a21*a22ss*uey+2*a21*a22s*vs)/(a11*a22-a21*a12)
                  a11r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axis,0)/(rx(
     & i1+is1,i2+is2,i3+is3)*sy(i1+is1,i2+is2,i3+is3)-ry(i1+is1,i2+
     & is2,i3+is3)*sx(i1+is1,i2+is2,i3+is3)))-(rsxy(i1-is1,i2-is2,i3-
     & is3,axis,0)/(rx(i1-is1,i2-is2,i3-is3)*sy(i1-is1,i2-is2,i3-is3)-
     & ry(i1-is1,i2-is2,i3-is3)*sx(i1-is1,i2-is2,i3-is3))))-((rsxy(i1+
     & 2*is1,i2+2*is2,i3+2*is3,axis,0)/(rx(i1+2*is1,i2+2*is2,i3+2*is3)
     & *sy(i1+2*is1,i2+2*is2,i3+2*is3)-ry(i1+2*is1,i2+2*is2,i3+2*is3)*
     & sx(i1+2*is1,i2+2*is2,i3+2*is3)))-(rsxy(i1-2*is1,i2-2*is2,i3-2*
     & is3,axis,0)/(rx(i1-2*is1,i2-2*is2,i3-2*is3)*sy(i1-2*is1,i2-2*
     & is2,i3-2*is3)-ry(i1-2*is1,i2-2*is2,i3-2*is3)*sx(i1-2*is1,i2-2*
     & is2,i3-2*is3)))))/(12.*dra)
                  a12r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axis,1)/(rx(
     & i1+is1,i2+is2,i3+is3)*sy(i1+is1,i2+is2,i3+is3)-ry(i1+is1,i2+
     & is2,i3+is3)*sx(i1+is1,i2+is2,i3+is3)))-(rsxy(i1-is1,i2-is2,i3-
     & is3,axis,1)/(rx(i1-is1,i2-is2,i3-is3)*sy(i1-is1,i2-is2,i3-is3)-
     & ry(i1-is1,i2-is2,i3-is3)*sx(i1-is1,i2-is2,i3-is3))))-((rsxy(i1+
     & 2*is1,i2+2*is2,i3+2*is3,axis,1)/(rx(i1+2*is1,i2+2*is2,i3+2*is3)
     & *sy(i1+2*is1,i2+2*is2,i3+2*is3)-ry(i1+2*is1,i2+2*is2,i3+2*is3)*
     & sx(i1+2*is1,i2+2*is2,i3+2*is3)))-(rsxy(i1-2*is1,i2-2*is2,i3-2*
     & is3,axis,1)/(rx(i1-2*is1,i2-2*is2,i3-2*is3)*sy(i1-2*is1,i2-2*
     & is2,i3-2*is3)-ry(i1-2*is1,i2-2*is2,i3-2*is3)*sx(i1-2*is1,i2-2*
     & is2,i3-2*is3)))))/(12.*dra)
                  a21r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axisp1,0)/(rx(
     & i1+is1,i2+is2,i3+is3)*sy(i1+is1,i2+is2,i3+is3)-ry(i1+is1,i2+
     & is2,i3+is3)*sx(i1+is1,i2+is2,i3+is3)))-(rsxy(i1-is1,i2-is2,i3-
     & is3,axisp1,0)/(rx(i1-is1,i2-is2,i3-is3)*sy(i1-is1,i2-is2,i3-
     & is3)-ry(i1-is1,i2-is2,i3-is3)*sx(i1-is1,i2-is2,i3-is3))))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp1,0)/(rx(i1+2*is1,i2+2*
     & is2,i3+2*is3)*sy(i1+2*is1,i2+2*is2,i3+2*is3)-ry(i1+2*is1,i2+2*
     & is2,i3+2*is3)*sx(i1+2*is1,i2+2*is2,i3+2*is3)))-(rsxy(i1-2*is1,
     & i2-2*is2,i3-2*is3,axisp1,0)/(rx(i1-2*is1,i2-2*is2,i3-2*is3)*sy(
     & i1-2*is1,i2-2*is2,i3-2*is3)-ry(i1-2*is1,i2-2*is2,i3-2*is3)*sx(
     & i1-2*is1,i2-2*is2,i3-2*is3)))))/(12.*dra)
                  a22r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axisp1,1)/(rx(
     & i1+is1,i2+is2,i3+is3)*sy(i1+is1,i2+is2,i3+is3)-ry(i1+is1,i2+
     & is2,i3+is3)*sx(i1+is1,i2+is2,i3+is3)))-(rsxy(i1-is1,i2-is2,i3-
     & is3,axisp1,1)/(rx(i1-is1,i2-is2,i3-is3)*sy(i1-is1,i2-is2,i3-
     & is3)-ry(i1-is1,i2-is2,i3-is3)*sx(i1-is1,i2-is2,i3-is3))))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp1,1)/(rx(i1+2*is1,i2+2*
     & is2,i3+2*is3)*sy(i1+2*is1,i2+2*is2,i3+2*is3)-ry(i1+2*is1,i2+2*
     & is2,i3+2*is3)*sx(i1+2*is1,i2+2*is2,i3+2*is3)))-(rsxy(i1-2*is1,
     & i2-2*is2,i3-2*is3,axisp1,1)/(rx(i1-2*is1,i2-2*is2,i3-2*is3)*sy(
     & i1-2*is1,i2-2*is2,i3-2*is3)-ry(i1-2*is1,i2-2*is2,i3-2*is3)*sx(
     & i1-2*is1,i2-2*is2,i3-2*is3)))))/(12.*dra)
                  a11s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axis,0)/(rx(
     & i1+js1,i2+js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+
     & js2,i3+js3)*sx(i1+js1,i2+js2,i3+js3)))-(rsxy(i1-js1,i2-js2,i3-
     & js3,axis,0)/(rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-
     & ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3))))-((rsxy(i1+
     & 2*js1,i2+2*js2,i3+2*js3,axis,0)/(rx(i1+2*js1,i2+2*js2,i3+2*js3)
     & *sy(i1+2*js1,i2+2*js2,i3+2*js3)-ry(i1+2*js1,i2+2*js2,i3+2*js3)*
     & sx(i1+2*js1,i2+2*js2,i3+2*js3)))-(rsxy(i1-2*js1,i2-2*js2,i3-2*
     & js3,axis,0)/(rx(i1-2*js1,i2-2*js2,i3-2*js3)*sy(i1-2*js1,i2-2*
     & js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,i3-2*js3)*sx(i1-2*js1,i2-2*
     & js2,i3-2*js3)))))/(12.*dsa)
                  a12s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axis,1)/(rx(
     & i1+js1,i2+js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+
     & js2,i3+js3)*sx(i1+js1,i2+js2,i3+js3)))-(rsxy(i1-js1,i2-js2,i3-
     & js3,axis,1)/(rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-
     & ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3))))-((rsxy(i1+
     & 2*js1,i2+2*js2,i3+2*js3,axis,1)/(rx(i1+2*js1,i2+2*js2,i3+2*js3)
     & *sy(i1+2*js1,i2+2*js2,i3+2*js3)-ry(i1+2*js1,i2+2*js2,i3+2*js3)*
     & sx(i1+2*js1,i2+2*js2,i3+2*js3)))-(rsxy(i1-2*js1,i2-2*js2,i3-2*
     & js3,axis,1)/(rx(i1-2*js1,i2-2*js2,i3-2*js3)*sy(i1-2*js1,i2-2*
     & js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,i3-2*js3)*sx(i1-2*js1,i2-2*
     & js2,i3-2*js3)))))/(12.*dsa)
                  a21s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axisp1,0)/(rx(
     & i1+js1,i2+js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+
     & js2,i3+js3)*sx(i1+js1,i2+js2,i3+js3)))-(rsxy(i1-js1,i2-js2,i3-
     & js3,axisp1,0)/(rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-
     & js3)-ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3))))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,0)/(rx(i1+2*js1,i2+2*
     & js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,i3+2*js3)-ry(i1+2*js1,i2+2*
     & js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,i3+2*js3)))-(rsxy(i1-2*js1,
     & i2-2*js2,i3-2*js3,axisp1,0)/(rx(i1-2*js1,i2-2*js2,i3-2*js3)*sy(
     & i1-2*js1,i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,i3-2*js3)*sx(
     & i1-2*js1,i2-2*js2,i3-2*js3)))))/(12.*dsa)
                  a22s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axisp1,1)/(rx(
     & i1+js1,i2+js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+
     & js2,i3+js3)*sx(i1+js1,i2+js2,i3+js3)))-(rsxy(i1-js1,i2-js2,i3-
     & js3,axisp1,1)/(rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-
     & js3)-ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3))))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,1)/(rx(i1+2*js1,i2+2*
     & js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,i3+2*js3)-ry(i1+2*js1,i2+2*
     & js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,i3+2*js3)))-(rsxy(i1-2*js1,
     & i2-2*js2,i3-2*js3,axisp1,1)/(rx(i1-2*js1,i2-2*js2,i3-2*js3)*sy(
     & i1-2*js1,i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,i3-2*js3)*sx(
     & i1-2*js1,i2-2*js2,i3-2*js3)))))/(12.*dsa)
                  a11rs = (8.*((8.*((rsxy(i1+1,i2+1,i3,axis,0)/(rx(i1+
     & 1,i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+1,i3)))
     & -(rsxy(i1-1,i2+1,i3,axis,0)/(rx(i1-1,i2+1,i3)*sy(i1-1,i2+1,i3)-
     & ry(i1-1,i2+1,i3)*sx(i1-1,i2+1,i3))))-((rsxy(i1+2,i2+1,i3,axis,
     & 0)/(rx(i1+2,i2+1,i3)*sy(i1+2,i2+1,i3)-ry(i1+2,i2+1,i3)*sx(i1+2,
     & i2+1,i3)))-(rsxy(i1-2,i2+1,i3,axis,0)/(rx(i1-2,i2+1,i3)*sy(i1-
     & 2,i2+1,i3)-ry(i1-2,i2+1,i3)*sx(i1-2,i2+1,i3)))))/(12.*dr(0))-(
     & 8.*((rsxy(i1+1,i2-1,i3,axis,0)/(rx(i1+1,i2-1,i3)*sy(i1+1,i2-1,
     & i3)-ry(i1+1,i2-1,i3)*sx(i1+1,i2-1,i3)))-(rsxy(i1-1,i2-1,i3,
     & axis,0)/(rx(i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-ry(i1-1,i2-1,i3)*sx(
     & i1-1,i2-1,i3))))-((rsxy(i1+2,i2-1,i3,axis,0)/(rx(i1+2,i2-1,i3)*
     & sy(i1+2,i2-1,i3)-ry(i1+2,i2-1,i3)*sx(i1+2,i2-1,i3)))-(rsxy(i1-
     & 2,i2-1,i3,axis,0)/(rx(i1-2,i2-1,i3)*sy(i1-2,i2-1,i3)-ry(i1-2,
     & i2-1,i3)*sx(i1-2,i2-1,i3)))))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2+
     & 2,i3,axis,0)/(rx(i1+1,i2+2,i3)*sy(i1+1,i2+2,i3)-ry(i1+1,i2+2,
     & i3)*sx(i1+1,i2+2,i3)))-(rsxy(i1-1,i2+2,i3,axis,0)/(rx(i1-1,i2+
     & 2,i3)*sy(i1-1,i2+2,i3)-ry(i1-1,i2+2,i3)*sx(i1-1,i2+2,i3))))-((
     & rsxy(i1+2,i2+2,i3,axis,0)/(rx(i1+2,i2+2,i3)*sy(i1+2,i2+2,i3)-
     & ry(i1+2,i2+2,i3)*sx(i1+2,i2+2,i3)))-(rsxy(i1-2,i2+2,i3,axis,0)
     & /(rx(i1-2,i2+2,i3)*sy(i1-2,i2+2,i3)-ry(i1-2,i2+2,i3)*sx(i1-2,
     & i2+2,i3)))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,i3,axis,0)/(rx(
     & i1+1,i2-2,i3)*sy(i1+1,i2-2,i3)-ry(i1+1,i2-2,i3)*sx(i1+1,i2-2,
     & i3)))-(rsxy(i1-1,i2-2,i3,axis,0)/(rx(i1-1,i2-2,i3)*sy(i1-1,i2-
     & 2,i3)-ry(i1-1,i2-2,i3)*sx(i1-1,i2-2,i3))))-((rsxy(i1+2,i2-2,i3,
     & axis,0)/(rx(i1+2,i2-2,i3)*sy(i1+2,i2-2,i3)-ry(i1+2,i2-2,i3)*sx(
     & i1+2,i2-2,i3)))-(rsxy(i1-2,i2-2,i3,axis,0)/(rx(i1-2,i2-2,i3)*
     & sy(i1-2,i2-2,i3)-ry(i1-2,i2-2,i3)*sx(i1-2,i2-2,i3)))))/(12.*dr(
     & 0))))/(12.*dr(1))
                  a12rs = (8.*((8.*((rsxy(i1+1,i2+1,i3,axis,1)/(rx(i1+
     & 1,i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+1,i3)))
     & -(rsxy(i1-1,i2+1,i3,axis,1)/(rx(i1-1,i2+1,i3)*sy(i1-1,i2+1,i3)-
     & ry(i1-1,i2+1,i3)*sx(i1-1,i2+1,i3))))-((rsxy(i1+2,i2+1,i3,axis,
     & 1)/(rx(i1+2,i2+1,i3)*sy(i1+2,i2+1,i3)-ry(i1+2,i2+1,i3)*sx(i1+2,
     & i2+1,i3)))-(rsxy(i1-2,i2+1,i3,axis,1)/(rx(i1-2,i2+1,i3)*sy(i1-
     & 2,i2+1,i3)-ry(i1-2,i2+1,i3)*sx(i1-2,i2+1,i3)))))/(12.*dr(0))-(
     & 8.*((rsxy(i1+1,i2-1,i3,axis,1)/(rx(i1+1,i2-1,i3)*sy(i1+1,i2-1,
     & i3)-ry(i1+1,i2-1,i3)*sx(i1+1,i2-1,i3)))-(rsxy(i1-1,i2-1,i3,
     & axis,1)/(rx(i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-ry(i1-1,i2-1,i3)*sx(
     & i1-1,i2-1,i3))))-((rsxy(i1+2,i2-1,i3,axis,1)/(rx(i1+2,i2-1,i3)*
     & sy(i1+2,i2-1,i3)-ry(i1+2,i2-1,i3)*sx(i1+2,i2-1,i3)))-(rsxy(i1-
     & 2,i2-1,i3,axis,1)/(rx(i1-2,i2-1,i3)*sy(i1-2,i2-1,i3)-ry(i1-2,
     & i2-1,i3)*sx(i1-2,i2-1,i3)))))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2+
     & 2,i3,axis,1)/(rx(i1+1,i2+2,i3)*sy(i1+1,i2+2,i3)-ry(i1+1,i2+2,
     & i3)*sx(i1+1,i2+2,i3)))-(rsxy(i1-1,i2+2,i3,axis,1)/(rx(i1-1,i2+
     & 2,i3)*sy(i1-1,i2+2,i3)-ry(i1-1,i2+2,i3)*sx(i1-1,i2+2,i3))))-((
     & rsxy(i1+2,i2+2,i3,axis,1)/(rx(i1+2,i2+2,i3)*sy(i1+2,i2+2,i3)-
     & ry(i1+2,i2+2,i3)*sx(i1+2,i2+2,i3)))-(rsxy(i1-2,i2+2,i3,axis,1)
     & /(rx(i1-2,i2+2,i3)*sy(i1-2,i2+2,i3)-ry(i1-2,i2+2,i3)*sx(i1-2,
     & i2+2,i3)))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,i3,axis,1)/(rx(
     & i1+1,i2-2,i3)*sy(i1+1,i2-2,i3)-ry(i1+1,i2-2,i3)*sx(i1+1,i2-2,
     & i3)))-(rsxy(i1-1,i2-2,i3,axis,1)/(rx(i1-1,i2-2,i3)*sy(i1-1,i2-
     & 2,i3)-ry(i1-1,i2-2,i3)*sx(i1-1,i2-2,i3))))-((rsxy(i1+2,i2-2,i3,
     & axis,1)/(rx(i1+2,i2-2,i3)*sy(i1+2,i2-2,i3)-ry(i1+2,i2-2,i3)*sx(
     & i1+2,i2-2,i3)))-(rsxy(i1-2,i2-2,i3,axis,1)/(rx(i1-2,i2-2,i3)*
     & sy(i1-2,i2-2,i3)-ry(i1-2,i2-2,i3)*sx(i1-2,i2-2,i3)))))/(12.*dr(
     & 0))))/(12.*dr(1))
                  a21rs = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp1,0)/(rx(
     & i1+1,i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+1,
     & i3)))-(rsxy(i1-1,i2+1,i3,axisp1,0)/(rx(i1-1,i2+1,i3)*sy(i1-1,
     & i2+1,i3)-ry(i1-1,i2+1,i3)*sx(i1-1,i2+1,i3))))-((rsxy(i1+2,i2+1,
     & i3,axisp1,0)/(rx(i1+2,i2+1,i3)*sy(i1+2,i2+1,i3)-ry(i1+2,i2+1,
     & i3)*sx(i1+2,i2+1,i3)))-(rsxy(i1-2,i2+1,i3,axisp1,0)/(rx(i1-2,
     & i2+1,i3)*sy(i1-2,i2+1,i3)-ry(i1-2,i2+1,i3)*sx(i1-2,i2+1,i3)))))
     & /(12.*dr(0))-(8.*((rsxy(i1+1,i2-1,i3,axisp1,0)/(rx(i1+1,i2-1,
     & i3)*sy(i1+1,i2-1,i3)-ry(i1+1,i2-1,i3)*sx(i1+1,i2-1,i3)))-(rsxy(
     & i1-1,i2-1,i3,axisp1,0)/(rx(i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-ry(
     & i1-1,i2-1,i3)*sx(i1-1,i2-1,i3))))-((rsxy(i1+2,i2-1,i3,axisp1,0)
     & /(rx(i1+2,i2-1,i3)*sy(i1+2,i2-1,i3)-ry(i1+2,i2-1,i3)*sx(i1+2,
     & i2-1,i3)))-(rsxy(i1-2,i2-1,i3,axisp1,0)/(rx(i1-2,i2-1,i3)*sy(
     & i1-2,i2-1,i3)-ry(i1-2,i2-1,i3)*sx(i1-2,i2-1,i3)))))/(12.*dr(0))
     & )-((8.*((rsxy(i1+1,i2+2,i3,axisp1,0)/(rx(i1+1,i2+2,i3)*sy(i1+1,
     & i2+2,i3)-ry(i1+1,i2+2,i3)*sx(i1+1,i2+2,i3)))-(rsxy(i1-1,i2+2,
     & i3,axisp1,0)/(rx(i1-1,i2+2,i3)*sy(i1-1,i2+2,i3)-ry(i1-1,i2+2,
     & i3)*sx(i1-1,i2+2,i3))))-((rsxy(i1+2,i2+2,i3,axisp1,0)/(rx(i1+2,
     & i2+2,i3)*sy(i1+2,i2+2,i3)-ry(i1+2,i2+2,i3)*sx(i1+2,i2+2,i3)))-(
     & rsxy(i1-2,i2+2,i3,axisp1,0)/(rx(i1-2,i2+2,i3)*sy(i1-2,i2+2,i3)-
     & ry(i1-2,i2+2,i3)*sx(i1-2,i2+2,i3)))))/(12.*dr(0))-(8.*((rsxy(
     & i1+1,i2-2,i3,axisp1,0)/(rx(i1+1,i2-2,i3)*sy(i1+1,i2-2,i3)-ry(
     & i1+1,i2-2,i3)*sx(i1+1,i2-2,i3)))-(rsxy(i1-1,i2-2,i3,axisp1,0)/(
     & rx(i1-1,i2-2,i3)*sy(i1-1,i2-2,i3)-ry(i1-1,i2-2,i3)*sx(i1-1,i2-
     & 2,i3))))-((rsxy(i1+2,i2-2,i3,axisp1,0)/(rx(i1+2,i2-2,i3)*sy(i1+
     & 2,i2-2,i3)-ry(i1+2,i2-2,i3)*sx(i1+2,i2-2,i3)))-(rsxy(i1-2,i2-2,
     & i3,axisp1,0)/(rx(i1-2,i2-2,i3)*sy(i1-2,i2-2,i3)-ry(i1-2,i2-2,
     & i3)*sx(i1-2,i2-2,i3)))))/(12.*dr(0))))/(12.*dr(1))
                  a22rs = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp1,1)/(rx(
     & i1+1,i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+1,
     & i3)))-(rsxy(i1-1,i2+1,i3,axisp1,1)/(rx(i1-1,i2+1,i3)*sy(i1-1,
     & i2+1,i3)-ry(i1-1,i2+1,i3)*sx(i1-1,i2+1,i3))))-((rsxy(i1+2,i2+1,
     & i3,axisp1,1)/(rx(i1+2,i2+1,i3)*sy(i1+2,i2+1,i3)-ry(i1+2,i2+1,
     & i3)*sx(i1+2,i2+1,i3)))-(rsxy(i1-2,i2+1,i3,axisp1,1)/(rx(i1-2,
     & i2+1,i3)*sy(i1-2,i2+1,i3)-ry(i1-2,i2+1,i3)*sx(i1-2,i2+1,i3)))))
     & /(12.*dr(0))-(8.*((rsxy(i1+1,i2-1,i3,axisp1,1)/(rx(i1+1,i2-1,
     & i3)*sy(i1+1,i2-1,i3)-ry(i1+1,i2-1,i3)*sx(i1+1,i2-1,i3)))-(rsxy(
     & i1-1,i2-1,i3,axisp1,1)/(rx(i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-ry(
     & i1-1,i2-1,i3)*sx(i1-1,i2-1,i3))))-((rsxy(i1+2,i2-1,i3,axisp1,1)
     & /(rx(i1+2,i2-1,i3)*sy(i1+2,i2-1,i3)-ry(i1+2,i2-1,i3)*sx(i1+2,
     & i2-1,i3)))-(rsxy(i1-2,i2-1,i3,axisp1,1)/(rx(i1-2,i2-1,i3)*sy(
     & i1-2,i2-1,i3)-ry(i1-2,i2-1,i3)*sx(i1-2,i2-1,i3)))))/(12.*dr(0))
     & )-((8.*((rsxy(i1+1,i2+2,i3,axisp1,1)/(rx(i1+1,i2+2,i3)*sy(i1+1,
     & i2+2,i3)-ry(i1+1,i2+2,i3)*sx(i1+1,i2+2,i3)))-(rsxy(i1-1,i2+2,
     & i3,axisp1,1)/(rx(i1-1,i2+2,i3)*sy(i1-1,i2+2,i3)-ry(i1-1,i2+2,
     & i3)*sx(i1-1,i2+2,i3))))-((rsxy(i1+2,i2+2,i3,axisp1,1)/(rx(i1+2,
     & i2+2,i3)*sy(i1+2,i2+2,i3)-ry(i1+2,i2+2,i3)*sx(i1+2,i2+2,i3)))-(
     & rsxy(i1-2,i2+2,i3,axisp1,1)/(rx(i1-2,i2+2,i3)*sy(i1-2,i2+2,i3)-
     & ry(i1-2,i2+2,i3)*sx(i1-2,i2+2,i3)))))/(12.*dr(0))-(8.*((rsxy(
     & i1+1,i2-2,i3,axisp1,1)/(rx(i1+1,i2-2,i3)*sy(i1+1,i2-2,i3)-ry(
     & i1+1,i2-2,i3)*sx(i1+1,i2-2,i3)))-(rsxy(i1-1,i2-2,i3,axisp1,1)/(
     & rx(i1-1,i2-2,i3)*sy(i1-1,i2-2,i3)-ry(i1-1,i2-2,i3)*sx(i1-1,i2-
     & 2,i3))))-((rsxy(i1+2,i2-2,i3,axisp1,1)/(rx(i1+2,i2-2,i3)*sy(i1+
     & 2,i2-2,i3)-ry(i1+2,i2-2,i3)*sx(i1+2,i2-2,i3)))-(rsxy(i1-2,i2-2,
     & i3,axisp1,1)/(rx(i1-2,i2-2,i3)*sy(i1-2,i2-2,i3)-ry(i1-2,i2-2,
     & i3)*sx(i1-2,i2-2,i3)))))/(12.*dr(0))))/(12.*dr(1))
                  a11rr = ((-30.*(rsxy(i1,i2,i3,axis,0)/(rx(i1,i2,i3)*
     & sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+16.*((rsxy(i1+is1,i2+
     & is2,i3+is3,axis,0)/(rx(i1+is1,i2+is2,i3+is3)*sy(i1+is1,i2+is2,
     & i3+is3)-ry(i1+is1,i2+is2,i3+is3)*sx(i1+is1,i2+is2,i3+is3)))+(
     & rsxy(i1-is1,i2-is2,i3-is3,axis,0)/(rx(i1-is1,i2-is2,i3-is3)*sy(
     & i1-is1,i2-is2,i3-is3)-ry(i1-is1,i2-is2,i3-is3)*sx(i1-is1,i2-
     & is2,i3-is3))))-((rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,0)/(rx(
     & i1+2*is1,i2+2*is2,i3+2*is3)*sy(i1+2*is1,i2+2*is2,i3+2*is3)-ry(
     & i1+2*is1,i2+2*is2,i3+2*is3)*sx(i1+2*is1,i2+2*is2,i3+2*is3)))+(
     & rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,0)/(rx(i1-2*is1,i2-2*is2,
     & i3-2*is3)*sy(i1-2*is1,i2-2*is2,i3-2*is3)-ry(i1-2*is1,i2-2*is2,
     & i3-2*is3)*sx(i1-2*is1,i2-2*is2,i3-2*is3)))))/(12.*dra**2))
                  a12rr = ((-30.*(rsxy(i1,i2,i3,axis,1)/(rx(i1,i2,i3)*
     & sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+16.*((rsxy(i1+is1,i2+
     & is2,i3+is3,axis,1)/(rx(i1+is1,i2+is2,i3+is3)*sy(i1+is1,i2+is2,
     & i3+is3)-ry(i1+is1,i2+is2,i3+is3)*sx(i1+is1,i2+is2,i3+is3)))+(
     & rsxy(i1-is1,i2-is2,i3-is3,axis,1)/(rx(i1-is1,i2-is2,i3-is3)*sy(
     & i1-is1,i2-is2,i3-is3)-ry(i1-is1,i2-is2,i3-is3)*sx(i1-is1,i2-
     & is2,i3-is3))))-((rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,1)/(rx(
     & i1+2*is1,i2+2*is2,i3+2*is3)*sy(i1+2*is1,i2+2*is2,i3+2*is3)-ry(
     & i1+2*is1,i2+2*is2,i3+2*is3)*sx(i1+2*is1,i2+2*is2,i3+2*is3)))+(
     & rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,1)/(rx(i1-2*is1,i2-2*is2,
     & i3-2*is3)*sy(i1-2*is1,i2-2*is2,i3-2*is3)-ry(i1-2*is1,i2-2*is2,
     & i3-2*is3)*sx(i1-2*is1,i2-2*is2,i3-2*is3)))))/(12.*dra**2))
                  a21rr = ((-30.*(rsxy(i1,i2,i3,axisp1,0)/(rx(i1,i2,i3)
     & *sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+16.*((rsxy(i1+is1,i2+
     & is2,i3+is3,axisp1,0)/(rx(i1+is1,i2+is2,i3+is3)*sy(i1+is1,i2+
     & is2,i3+is3)-ry(i1+is1,i2+is2,i3+is3)*sx(i1+is1,i2+is2,i3+is3)))
     & +(rsxy(i1-is1,i2-is2,i3-is3,axisp1,0)/(rx(i1-is1,i2-is2,i3-is3)
     & *sy(i1-is1,i2-is2,i3-is3)-ry(i1-is1,i2-is2,i3-is3)*sx(i1-is1,
     & i2-is2,i3-is3))))-((rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp1,0)/(
     & rx(i1+2*is1,i2+2*is2,i3+2*is3)*sy(i1+2*is1,i2+2*is2,i3+2*is3)-
     & ry(i1+2*is1,i2+2*is2,i3+2*is3)*sx(i1+2*is1,i2+2*is2,i3+2*is3)))
     & +(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axisp1,0)/(rx(i1-2*is1,i2-2*
     & is2,i3-2*is3)*sy(i1-2*is1,i2-2*is2,i3-2*is3)-ry(i1-2*is1,i2-2*
     & is2,i3-2*is3)*sx(i1-2*is1,i2-2*is2,i3-2*is3)))))/(12.*dra**2))
                  a22rr = ((-30.*(rsxy(i1,i2,i3,axisp1,1)/(rx(i1,i2,i3)
     & *sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+16.*((rsxy(i1+is1,i2+
     & is2,i3+is3,axisp1,1)/(rx(i1+is1,i2+is2,i3+is3)*sy(i1+is1,i2+
     & is2,i3+is3)-ry(i1+is1,i2+is2,i3+is3)*sx(i1+is1,i2+is2,i3+is3)))
     & +(rsxy(i1-is1,i2-is2,i3-is3,axisp1,1)/(rx(i1-is1,i2-is2,i3-is3)
     & *sy(i1-is1,i2-is2,i3-is3)-ry(i1-is1,i2-is2,i3-is3)*sx(i1-is1,
     & i2-is2,i3-is3))))-((rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp1,1)/(
     & rx(i1+2*is1,i2+2*is2,i3+2*is3)*sy(i1+2*is1,i2+2*is2,i3+2*is3)-
     & ry(i1+2*is1,i2+2*is2,i3+2*is3)*sx(i1+2*is1,i2+2*is2,i3+2*is3)))
     & +(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axisp1,1)/(rx(i1-2*is1,i2-2*
     & is2,i3-2*is3)*sy(i1-2*is1,i2-2*is2,i3-2*is3)-ry(i1-2*is1,i2-2*
     & is2,i3-2*is3)*sx(i1-2*is1,i2-2*is2,i3-2*is3)))))/(12.*dra**2))
                  a11ss = ((-30.*(rsxy(i1,i2,i3,axis,0)/(rx(i1,i2,i3)*
     & sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+16.*((rsxy(i1+js1,i2+
     & js2,i3+js3,axis,0)/(rx(i1+js1,i2+js2,i3+js3)*sy(i1+js1,i2+js2,
     & i3+js3)-ry(i1+js1,i2+js2,i3+js3)*sx(i1+js1,i2+js2,i3+js3)))+(
     & rsxy(i1-js1,i2-js2,i3-js3,axis,0)/(rx(i1-js1,i2-js2,i3-js3)*sy(
     & i1-js1,i2-js2,i3-js3)-ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-
     & js2,i3-js3))))-((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axis,0)/(rx(
     & i1+2*js1,i2+2*js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,i3+2*js3)-ry(
     & i1+2*js1,i2+2*js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,i3+2*js3)))+(
     & rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axis,0)/(rx(i1-2*js1,i2-2*js2,
     & i3-2*js3)*sy(i1-2*js1,i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,
     & i3-2*js3)*sx(i1-2*js1,i2-2*js2,i3-2*js3)))))/(12.*dsa**2))
                  a12ss = ((-30.*(rsxy(i1,i2,i3,axis,1)/(rx(i1,i2,i3)*
     & sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+16.*((rsxy(i1+js1,i2+
     & js2,i3+js3,axis,1)/(rx(i1+js1,i2+js2,i3+js3)*sy(i1+js1,i2+js2,
     & i3+js3)-ry(i1+js1,i2+js2,i3+js3)*sx(i1+js1,i2+js2,i3+js3)))+(
     & rsxy(i1-js1,i2-js2,i3-js3,axis,1)/(rx(i1-js1,i2-js2,i3-js3)*sy(
     & i1-js1,i2-js2,i3-js3)-ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-
     & js2,i3-js3))))-((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axis,1)/(rx(
     & i1+2*js1,i2+2*js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,i3+2*js3)-ry(
     & i1+2*js1,i2+2*js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,i3+2*js3)))+(
     & rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axis,1)/(rx(i1-2*js1,i2-2*js2,
     & i3-2*js3)*sy(i1-2*js1,i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,
     & i3-2*js3)*sx(i1-2*js1,i2-2*js2,i3-2*js3)))))/(12.*dsa**2))
                  a21ss = ((-30.*(rsxy(i1,i2,i3,axisp1,0)/(rx(i1,i2,i3)
     & *sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+16.*((rsxy(i1+js1,i2+
     & js2,i3+js3,axisp1,0)/(rx(i1+js1,i2+js2,i3+js3)*sy(i1+js1,i2+
     & js2,i3+js3)-ry(i1+js1,i2+js2,i3+js3)*sx(i1+js1,i2+js2,i3+js3)))
     & +(rsxy(i1-js1,i2-js2,i3-js3,axisp1,0)/(rx(i1-js1,i2-js2,i3-js3)
     & *sy(i1-js1,i2-js2,i3-js3)-ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,
     & i2-js2,i3-js3))))-((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,0)/(
     & rx(i1+2*js1,i2+2*js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,i3+2*js3)-
     & ry(i1+2*js1,i2+2*js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,i3+2*js3)))
     & +(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axisp1,0)/(rx(i1-2*js1,i2-2*
     & js2,i3-2*js3)*sy(i1-2*js1,i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*
     & js2,i3-2*js3)*sx(i1-2*js1,i2-2*js2,i3-2*js3)))))/(12.*dsa**2))
                  a22ss = ((-30.*(rsxy(i1,i2,i3,axisp1,1)/(rx(i1,i2,i3)
     & *sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+16.*((rsxy(i1+js1,i2+
     & js2,i3+js3,axisp1,1)/(rx(i1+js1,i2+js2,i3+js3)*sy(i1+js1,i2+
     & js2,i3+js3)-ry(i1+js1,i2+js2,i3+js3)*sx(i1+js1,i2+js2,i3+js3)))
     & +(rsxy(i1-js1,i2-js2,i3-js3,axisp1,1)/(rx(i1-js1,i2-js2,i3-js3)
     & *sy(i1-js1,i2-js2,i3-js3)-ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,
     & i2-js2,i3-js3))))-((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,1)/(
     & rx(i1+2*js1,i2+2*js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,i3+2*js3)-
     & ry(i1+2*js1,i2+2*js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,i3+2*js3)))
     & +(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axisp1,1)/(rx(i1-2*js1,i2-2*
     & js2,i3-2*js3)*sy(i1-2*js1,i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*
     & js2,i3-2*js3)*sx(i1-2*js1,i2-2*js2,i3-2*js3)))))/(12.*dsa**2))
                  if( .true. )then
                    a11sss = ((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axis,0)
     & /(rx(i1+2*js1,i2+2*js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,i3+2*js3)
     & -ry(i1+2*js1,i2+2*js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,i3+2*js3))
     & )-2.*(rsxy(i1+js1,i2+js2,i3+js3,axis,0)/(rx(i1+js1,i2+js2,i3+
     & js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+js2,i3+js3)*sx(i1+
     & js1,i2+js2,i3+js3)))+2.*(rsxy(i1-js1,i2-js2,i3-js3,axis,0)/(rx(
     & i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-ry(i1-js1,i2-
     & js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3)))-(rsxy(i1-2*js1,i2-2*js2,
     & i3-2*js3,axis,0)/(rx(i1-2*js1,i2-2*js2,i3-2*js3)*sy(i1-2*js1,
     & i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,i3-2*js3)*sx(i1-2*js1,
     & i2-2*js2,i3-2*js3))))/(2.*dsa**3)
                    a12sss = ((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axis,1)
     & /(rx(i1+2*js1,i2+2*js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,i3+2*js3)
     & -ry(i1+2*js1,i2+2*js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,i3+2*js3))
     & )-2.*(rsxy(i1+js1,i2+js2,i3+js3,axis,1)/(rx(i1+js1,i2+js2,i3+
     & js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+js2,i3+js3)*sx(i1+
     & js1,i2+js2,i3+js3)))+2.*(rsxy(i1-js1,i2-js2,i3-js3,axis,1)/(rx(
     & i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-ry(i1-js1,i2-
     & js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3)))-(rsxy(i1-2*js1,i2-2*js2,
     & i3-2*js3,axis,1)/(rx(i1-2*js1,i2-2*js2,i3-2*js3)*sy(i1-2*js1,
     & i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,i3-2*js3)*sx(i1-2*js1,
     & i2-2*js2,i3-2*js3))))/(2.*dsa**3)
                    a21sss = ((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,
     & 0)/(rx(i1+2*js1,i2+2*js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,i3+2*
     & js3)-ry(i1+2*js1,i2+2*js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,i3+2*
     & js3)))-2.*(rsxy(i1+js1,i2+js2,i3+js3,axisp1,0)/(rx(i1+js1,i2+
     & js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+js2,i3+js3)*
     & sx(i1+js1,i2+js2,i3+js3)))+2.*(rsxy(i1-js1,i2-js2,i3-js3,
     & axisp1,0)/(rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-
     & ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3)))-(rsxy(i1-2*
     & js1,i2-2*js2,i3-2*js3,axisp1,0)/(rx(i1-2*js1,i2-2*js2,i3-2*js3)
     & *sy(i1-2*js1,i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,i3-2*js3)*
     & sx(i1-2*js1,i2-2*js2,i3-2*js3))))/(2.*dsa**3)
                    a22sss = ((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,
     & 1)/(rx(i1+2*js1,i2+2*js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,i3+2*
     & js3)-ry(i1+2*js1,i2+2*js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,i3+2*
     & js3)))-2.*(rsxy(i1+js1,i2+js2,i3+js3,axisp1,1)/(rx(i1+js1,i2+
     & js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+js2,i3+js3)*
     & sx(i1+js1,i2+js2,i3+js3)))+2.*(rsxy(i1-js1,i2-js2,i3-js3,
     & axisp1,1)/(rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-
     & ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3)))-(rsxy(i1-2*
     & js1,i2-2*js2,i3-2*js3,axisp1,1)/(rx(i1-2*js1,i2-2*js2,i3-2*js3)
     & *sy(i1-2*js1,i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,i3-2*js3)*
     & sx(i1-2*js1,i2-2*js2,i3-2*js3))))/(2.*dsa**3)
                  else ! there are not enough ghost points in general to use:
                    a11sss = (8*(rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axis,
     & 0)/(rx(i1+2*js1,i2+2*js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,i3+2*
     & js3)-ry(i1+2*js1,i2+2*js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,i3+2*
     & js3)))-13*(rsxy(i1+js1,i2+js2,i3+js3,axis,0)/(rx(i1+js1,i2+js2,
     & i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+js2,i3+js3)*sx(
     & i1+js1,i2+js2,i3+js3)))+13*(rsxy(i1-js1,i2-js2,i3-js3,axis,0)/(
     & rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-ry(i1-js1,i2-
     & js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3)))-8*(rsxy(i1-2*js1,i2-2*
     & js2,i3-2*js3,axis,0)/(rx(i1-2*js1,i2-2*js2,i3-2*js3)*sy(i1-2*
     & js1,i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,i3-2*js3)*sx(i1-2*
     & js1,i2-2*js2,i3-2*js3)))-(rsxy(i1+3*js1,i2+3*js2,i3+3*js3,axis,
     & 0)/(rx(i1+3*js1,i2+3*js2,i3+3*js3)*sy(i1+3*js1,i2+3*js2,i3+3*
     & js3)-ry(i1+3*js1,i2+3*js2,i3+3*js3)*sx(i1+3*js1,i2+3*js2,i3+3*
     & js3)))+(rsxy(i1-3*js1,i2-3*js2,i3-3*js3,axis,0)/(rx(i1-3*js1,
     & i2-3*js2,i3-3*js3)*sy(i1-3*js1,i2-3*js2,i3-3*js3)-ry(i1-3*js1,
     & i2-3*js2,i3-3*js3)*sx(i1-3*js1,i2-3*js2,i3-3*js3))))/(8.*dsa**
     & 3)
                    a12sss = (8*(rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axis,
     & 1)/(rx(i1+2*js1,i2+2*js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,i3+2*
     & js3)-ry(i1+2*js1,i2+2*js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,i3+2*
     & js3)))-13*(rsxy(i1+js1,i2+js2,i3+js3,axis,1)/(rx(i1+js1,i2+js2,
     & i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+js2,i3+js3)*sx(
     & i1+js1,i2+js2,i3+js3)))+13*(rsxy(i1-js1,i2-js2,i3-js3,axis,1)/(
     & rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-ry(i1-js1,i2-
     & js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3)))-8*(rsxy(i1-2*js1,i2-2*
     & js2,i3-2*js3,axis,1)/(rx(i1-2*js1,i2-2*js2,i3-2*js3)*sy(i1-2*
     & js1,i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,i3-2*js3)*sx(i1-2*
     & js1,i2-2*js2,i3-2*js3)))-(rsxy(i1+3*js1,i2+3*js2,i3+3*js3,axis,
     & 1)/(rx(i1+3*js1,i2+3*js2,i3+3*js3)*sy(i1+3*js1,i2+3*js2,i3+3*
     & js3)-ry(i1+3*js1,i2+3*js2,i3+3*js3)*sx(i1+3*js1,i2+3*js2,i3+3*
     & js3)))+(rsxy(i1-3*js1,i2-3*js2,i3-3*js3,axis,1)/(rx(i1-3*js1,
     & i2-3*js2,i3-3*js3)*sy(i1-3*js1,i2-3*js2,i3-3*js3)-ry(i1-3*js1,
     & i2-3*js2,i3-3*js3)*sx(i1-3*js1,i2-3*js2,i3-3*js3))))/(8.*dsa**
     & 3)
                    a21sss = (8*(rsxy(i1+2*js1,i2+2*js2,i3+2*js3,
     & axisp1,0)/(rx(i1+2*js1,i2+2*js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,
     & i3+2*js3)-ry(i1+2*js1,i2+2*js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,
     & i3+2*js3)))-13*(rsxy(i1+js1,i2+js2,i3+js3,axisp1,0)/(rx(i1+js1,
     & i2+js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+js2,i3+
     & js3)*sx(i1+js1,i2+js2,i3+js3)))+13*(rsxy(i1-js1,i2-js2,i3-js3,
     & axisp1,0)/(rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-
     & ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3)))-8*(rsxy(i1-
     & 2*js1,i2-2*js2,i3-2*js3,axisp1,0)/(rx(i1-2*js1,i2-2*js2,i3-2*
     & js3)*sy(i1-2*js1,i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,i3-2*
     & js3)*sx(i1-2*js1,i2-2*js2,i3-2*js3)))-(rsxy(i1+3*js1,i2+3*js2,
     & i3+3*js3,axisp1,0)/(rx(i1+3*js1,i2+3*js2,i3+3*js3)*sy(i1+3*js1,
     & i2+3*js2,i3+3*js3)-ry(i1+3*js1,i2+3*js2,i3+3*js3)*sx(i1+3*js1,
     & i2+3*js2,i3+3*js3)))+(rsxy(i1-3*js1,i2-3*js2,i3-3*js3,axisp1,0)
     & /(rx(i1-3*js1,i2-3*js2,i3-3*js3)*sy(i1-3*js1,i2-3*js2,i3-3*js3)
     & -ry(i1-3*js1,i2-3*js2,i3-3*js3)*sx(i1-3*js1,i2-3*js2,i3-3*js3))
     & ))/(8.*dsa**3)
                    a22sss = (8*(rsxy(i1+2*js1,i2+2*js2,i3+2*js3,
     & axisp1,1)/(rx(i1+2*js1,i2+2*js2,i3+2*js3)*sy(i1+2*js1,i2+2*js2,
     & i3+2*js3)-ry(i1+2*js1,i2+2*js2,i3+2*js3)*sx(i1+2*js1,i2+2*js2,
     & i3+2*js3)))-13*(rsxy(i1+js1,i2+js2,i3+js3,axisp1,1)/(rx(i1+js1,
     & i2+js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+js2,i3+
     & js3)*sx(i1+js1,i2+js2,i3+js3)))+13*(rsxy(i1-js1,i2-js2,i3-js3,
     & axisp1,1)/(rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-
     & ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3)))-8*(rsxy(i1-
     & 2*js1,i2-2*js2,i3-2*js3,axisp1,1)/(rx(i1-2*js1,i2-2*js2,i3-2*
     & js3)*sy(i1-2*js1,i2-2*js2,i3-2*js3)-ry(i1-2*js1,i2-2*js2,i3-2*
     & js3)*sx(i1-2*js1,i2-2*js2,i3-2*js3)))-(rsxy(i1+3*js1,i2+3*js2,
     & i3+3*js3,axisp1,1)/(rx(i1+3*js1,i2+3*js2,i3+3*js3)*sy(i1+3*js1,
     & i2+3*js2,i3+3*js3)-ry(i1+3*js1,i2+3*js2,i3+3*js3)*sx(i1+3*js1,
     & i2+3*js2,i3+3*js3)))+(rsxy(i1-3*js1,i2-3*js2,i3-3*js3,axisp1,1)
     & /(rx(i1-3*js1,i2-3*js2,i3-3*js3)*sy(i1-3*js1,i2-3*js2,i3-3*js3)
     & -ry(i1-3*js1,i2-3*js2,i3-3*js3)*sx(i1-3*js1,i2-3*js2,i3-3*js3))
     & ))/(8.*dsa**3)
                  end if
                  if( axis.eq.0 )then
                    a11rss = (128*(rsxy(i1+1,i2+1,i3,axis,0)/(rx(i1+1,
     & i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+1,i3)))-
     & 128*(rsxy(i1-1,i2+1,i3,axis,0)/(rx(i1-1,i2+1,i3)*sy(i1-1,i2+1,
     & i3)-ry(i1-1,i2+1,i3)*sx(i1-1,i2+1,i3)))-16*(rsxy(i1+2,i2+1,i3,
     & axis,0)/(rx(i1+2,i2+1,i3)*sy(i1+2,i2+1,i3)-ry(i1+2,i2+1,i3)*sx(
     & i1+2,i2+1,i3)))+16*(rsxy(i1-2,i2+1,i3,axis,0)/(rx(i1-2,i2+1,i3)
     & *sy(i1-2,i2+1,i3)-ry(i1-2,i2+1,i3)*sx(i1-2,i2+1,i3)))+128*(
     & rsxy(i1+1,i2-1,i3,axis,0)/(rx(i1+1,i2-1,i3)*sy(i1+1,i2-1,i3)-
     & ry(i1+1,i2-1,i3)*sx(i1+1,i2-1,i3)))-128*(rsxy(i1-1,i2-1,i3,
     & axis,0)/(rx(i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-ry(i1-1,i2-1,i3)*sx(
     & i1-1,i2-1,i3)))-16*(rsxy(i1+2,i2-1,i3,axis,0)/(rx(i1+2,i2-1,i3)
     & *sy(i1+2,i2-1,i3)-ry(i1+2,i2-1,i3)*sx(i1+2,i2-1,i3)))+16*(rsxy(
     & i1-2,i2-1,i3,axis,0)/(rx(i1-2,i2-1,i3)*sy(i1-2,i2-1,i3)-ry(i1-
     & 2,i2-1,i3)*sx(i1-2,i2-1,i3)))+30*(rsxy(i1+2,i2,i3,axis,0)/(rx(
     & i1+2,i2,i3)*sy(i1+2,i2,i3)-ry(i1+2,i2,i3)*sx(i1+2,i2,i3)))+8*(
     & rsxy(i1-1,i2+2,i3,axis,0)/(rx(i1-1,i2+2,i3)*sy(i1-1,i2+2,i3)-
     & ry(i1-1,i2+2,i3)*sx(i1-1,i2+2,i3)))+(rsxy(i1+2,i2+2,i3,axis,0)
     & /(rx(i1+2,i2+2,i3)*sy(i1+2,i2+2,i3)-ry(i1+2,i2+2,i3)*sx(i1+2,
     & i2+2,i3)))-(rsxy(i1-2,i2+2,i3,axis,0)/(rx(i1-2,i2+2,i3)*sy(i1-
     & 2,i2+2,i3)-ry(i1-2,i2+2,i3)*sx(i1-2,i2+2,i3)))-8*(rsxy(i1+1,i2-
     & 2,i3,axis,0)/(rx(i1+1,i2-2,i3)*sy(i1+1,i2-2,i3)-ry(i1+1,i2-2,
     & i3)*sx(i1+1,i2-2,i3)))+8*(rsxy(i1-1,i2-2,i3,axis,0)/(rx(i1-1,
     & i2-2,i3)*sy(i1-1,i2-2,i3)-ry(i1-1,i2-2,i3)*sx(i1-1,i2-2,i3)))+(
     & rsxy(i1+2,i2-2,i3,axis,0)/(rx(i1+2,i2-2,i3)*sy(i1+2,i2-2,i3)-
     & ry(i1+2,i2-2,i3)*sx(i1+2,i2-2,i3)))-(rsxy(i1-2,i2-2,i3,axis,0)
     & /(rx(i1-2,i2-2,i3)*sy(i1-2,i2-2,i3)-ry(i1-2,i2-2,i3)*sx(i1-2,
     & i2-2,i3)))-8*(rsxy(i1+1,i2+2,i3,axis,0)/(rx(i1+1,i2+2,i3)*sy(
     & i1+1,i2+2,i3)-ry(i1+1,i2+2,i3)*sx(i1+1,i2+2,i3)))-30*(rsxy(i1-
     & 2,i2,i3,axis,0)/(rx(i1-2,i2,i3)*sy(i1-2,i2,i3)-ry(i1-2,i2,i3)*
     & sx(i1-2,i2,i3)))-240*(rsxy(i1+1,i2,i3,axis,0)/(rx(i1+1,i2,i3)*
     & sy(i1+1,i2,i3)-ry(i1+1,i2,i3)*sx(i1+1,i2,i3)))+240*(rsxy(i1-1,
     & i2,i3,axis,0)/(rx(i1-1,i2,i3)*sy(i1-1,i2,i3)-ry(i1-1,i2,i3)*sx(
     & i1-1,i2,i3))))/(144.*dr(1)**2*dr(0))
                    a12rss = (128*(rsxy(i1+1,i2+1,i3,axis,1)/(rx(i1+1,
     & i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+1,i3)))-
     & 128*(rsxy(i1-1,i2+1,i3,axis,1)/(rx(i1-1,i2+1,i3)*sy(i1-1,i2+1,
     & i3)-ry(i1-1,i2+1,i3)*sx(i1-1,i2+1,i3)))-16*(rsxy(i1+2,i2+1,i3,
     & axis,1)/(rx(i1+2,i2+1,i3)*sy(i1+2,i2+1,i3)-ry(i1+2,i2+1,i3)*sx(
     & i1+2,i2+1,i3)))+16*(rsxy(i1-2,i2+1,i3,axis,1)/(rx(i1-2,i2+1,i3)
     & *sy(i1-2,i2+1,i3)-ry(i1-2,i2+1,i3)*sx(i1-2,i2+1,i3)))+128*(
     & rsxy(i1+1,i2-1,i3,axis,1)/(rx(i1+1,i2-1,i3)*sy(i1+1,i2-1,i3)-
     & ry(i1+1,i2-1,i3)*sx(i1+1,i2-1,i3)))-128*(rsxy(i1-1,i2-1,i3,
     & axis,1)/(rx(i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-ry(i1-1,i2-1,i3)*sx(
     & i1-1,i2-1,i3)))-16*(rsxy(i1+2,i2-1,i3,axis,1)/(rx(i1+2,i2-1,i3)
     & *sy(i1+2,i2-1,i3)-ry(i1+2,i2-1,i3)*sx(i1+2,i2-1,i3)))+16*(rsxy(
     & i1-2,i2-1,i3,axis,1)/(rx(i1-2,i2-1,i3)*sy(i1-2,i2-1,i3)-ry(i1-
     & 2,i2-1,i3)*sx(i1-2,i2-1,i3)))+30*(rsxy(i1+2,i2,i3,axis,1)/(rx(
     & i1+2,i2,i3)*sy(i1+2,i2,i3)-ry(i1+2,i2,i3)*sx(i1+2,i2,i3)))+8*(
     & rsxy(i1-1,i2+2,i3,axis,1)/(rx(i1-1,i2+2,i3)*sy(i1-1,i2+2,i3)-
     & ry(i1-1,i2+2,i3)*sx(i1-1,i2+2,i3)))+(rsxy(i1+2,i2+2,i3,axis,1)
     & /(rx(i1+2,i2+2,i3)*sy(i1+2,i2+2,i3)-ry(i1+2,i2+2,i3)*sx(i1+2,
     & i2+2,i3)))-(rsxy(i1-2,i2+2,i3,axis,1)/(rx(i1-2,i2+2,i3)*sy(i1-
     & 2,i2+2,i3)-ry(i1-2,i2+2,i3)*sx(i1-2,i2+2,i3)))-8*(rsxy(i1+1,i2-
     & 2,i3,axis,1)/(rx(i1+1,i2-2,i3)*sy(i1+1,i2-2,i3)-ry(i1+1,i2-2,
     & i3)*sx(i1+1,i2-2,i3)))+8*(rsxy(i1-1,i2-2,i3,axis,1)/(rx(i1-1,
     & i2-2,i3)*sy(i1-1,i2-2,i3)-ry(i1-1,i2-2,i3)*sx(i1-1,i2-2,i3)))+(
     & rsxy(i1+2,i2-2,i3,axis,1)/(rx(i1+2,i2-2,i3)*sy(i1+2,i2-2,i3)-
     & ry(i1+2,i2-2,i3)*sx(i1+2,i2-2,i3)))-(rsxy(i1-2,i2-2,i3,axis,1)
     & /(rx(i1-2,i2-2,i3)*sy(i1-2,i2-2,i3)-ry(i1-2,i2-2,i3)*sx(i1-2,
     & i2-2,i3)))-8*(rsxy(i1+1,i2+2,i3,axis,1)/(rx(i1+1,i2+2,i3)*sy(
     & i1+1,i2+2,i3)-ry(i1+1,i2+2,i3)*sx(i1+1,i2+2,i3)))-30*(rsxy(i1-
     & 2,i2,i3,axis,1)/(rx(i1-2,i2,i3)*sy(i1-2,i2,i3)-ry(i1-2,i2,i3)*
     & sx(i1-2,i2,i3)))-240*(rsxy(i1+1,i2,i3,axis,1)/(rx(i1+1,i2,i3)*
     & sy(i1+1,i2,i3)-ry(i1+1,i2,i3)*sx(i1+1,i2,i3)))+240*(rsxy(i1-1,
     & i2,i3,axis,1)/(rx(i1-1,i2,i3)*sy(i1-1,i2,i3)-ry(i1-1,i2,i3)*sx(
     & i1-1,i2,i3))))/(144.*dr(1)**2*dr(0))
                    a21rss = (128*(rsxy(i1+1,i2+1,i3,axisp1,0)/(rx(i1+
     & 1,i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+1,i3)))
     & -128*(rsxy(i1-1,i2+1,i3,axisp1,0)/(rx(i1-1,i2+1,i3)*sy(i1-1,i2+
     & 1,i3)-ry(i1-1,i2+1,i3)*sx(i1-1,i2+1,i3)))-16*(rsxy(i1+2,i2+1,
     & i3,axisp1,0)/(rx(i1+2,i2+1,i3)*sy(i1+2,i2+1,i3)-ry(i1+2,i2+1,
     & i3)*sx(i1+2,i2+1,i3)))+16*(rsxy(i1-2,i2+1,i3,axisp1,0)/(rx(i1-
     & 2,i2+1,i3)*sy(i1-2,i2+1,i3)-ry(i1-2,i2+1,i3)*sx(i1-2,i2+1,i3)))
     & +128*(rsxy(i1+1,i2-1,i3,axisp1,0)/(rx(i1+1,i2-1,i3)*sy(i1+1,i2-
     & 1,i3)-ry(i1+1,i2-1,i3)*sx(i1+1,i2-1,i3)))-128*(rsxy(i1-1,i2-1,
     & i3,axisp1,0)/(rx(i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-ry(i1-1,i2-1,
     & i3)*sx(i1-1,i2-1,i3)))-16*(rsxy(i1+2,i2-1,i3,axisp1,0)/(rx(i1+
     & 2,i2-1,i3)*sy(i1+2,i2-1,i3)-ry(i1+2,i2-1,i3)*sx(i1+2,i2-1,i3)))
     & +16*(rsxy(i1-2,i2-1,i3,axisp1,0)/(rx(i1-2,i2-1,i3)*sy(i1-2,i2-
     & 1,i3)-ry(i1-2,i2-1,i3)*sx(i1-2,i2-1,i3)))+30*(rsxy(i1+2,i2,i3,
     & axisp1,0)/(rx(i1+2,i2,i3)*sy(i1+2,i2,i3)-ry(i1+2,i2,i3)*sx(i1+
     & 2,i2,i3)))+8*(rsxy(i1-1,i2+2,i3,axisp1,0)/(rx(i1-1,i2+2,i3)*sy(
     & i1-1,i2+2,i3)-ry(i1-1,i2+2,i3)*sx(i1-1,i2+2,i3)))+(rsxy(i1+2,
     & i2+2,i3,axisp1,0)/(rx(i1+2,i2+2,i3)*sy(i1+2,i2+2,i3)-ry(i1+2,
     & i2+2,i3)*sx(i1+2,i2+2,i3)))-(rsxy(i1-2,i2+2,i3,axisp1,0)/(rx(
     & i1-2,i2+2,i3)*sy(i1-2,i2+2,i3)-ry(i1-2,i2+2,i3)*sx(i1-2,i2+2,
     & i3)))-8*(rsxy(i1+1,i2-2,i3,axisp1,0)/(rx(i1+1,i2-2,i3)*sy(i1+1,
     & i2-2,i3)-ry(i1+1,i2-2,i3)*sx(i1+1,i2-2,i3)))+8*(rsxy(i1-1,i2-2,
     & i3,axisp1,0)/(rx(i1-1,i2-2,i3)*sy(i1-1,i2-2,i3)-ry(i1-1,i2-2,
     & i3)*sx(i1-1,i2-2,i3)))+(rsxy(i1+2,i2-2,i3,axisp1,0)/(rx(i1+2,
     & i2-2,i3)*sy(i1+2,i2-2,i3)-ry(i1+2,i2-2,i3)*sx(i1+2,i2-2,i3)))-(
     & rsxy(i1-2,i2-2,i3,axisp1,0)/(rx(i1-2,i2-2,i3)*sy(i1-2,i2-2,i3)-
     & ry(i1-2,i2-2,i3)*sx(i1-2,i2-2,i3)))-8*(rsxy(i1+1,i2+2,i3,
     & axisp1,0)/(rx(i1+1,i2+2,i3)*sy(i1+1,i2+2,i3)-ry(i1+1,i2+2,i3)*
     & sx(i1+1,i2+2,i3)))-30*(rsxy(i1-2,i2,i3,axisp1,0)/(rx(i1-2,i2,
     & i3)*sy(i1-2,i2,i3)-ry(i1-2,i2,i3)*sx(i1-2,i2,i3)))-240*(rsxy(
     & i1+1,i2,i3,axisp1,0)/(rx(i1+1,i2,i3)*sy(i1+1,i2,i3)-ry(i1+1,i2,
     & i3)*sx(i1+1,i2,i3)))+240*(rsxy(i1-1,i2,i3,axisp1,0)/(rx(i1-1,
     & i2,i3)*sy(i1-1,i2,i3)-ry(i1-1,i2,i3)*sx(i1-1,i2,i3))))/(144.*
     & dr(1)**2*dr(0))
                    a22rss = (128*(rsxy(i1+1,i2+1,i3,axisp1,1)/(rx(i1+
     & 1,i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+1,i3)))
     & -128*(rsxy(i1-1,i2+1,i3,axisp1,1)/(rx(i1-1,i2+1,i3)*sy(i1-1,i2+
     & 1,i3)-ry(i1-1,i2+1,i3)*sx(i1-1,i2+1,i3)))-16*(rsxy(i1+2,i2+1,
     & i3,axisp1,1)/(rx(i1+2,i2+1,i3)*sy(i1+2,i2+1,i3)-ry(i1+2,i2+1,
     & i3)*sx(i1+2,i2+1,i3)))+16*(rsxy(i1-2,i2+1,i3,axisp1,1)/(rx(i1-
     & 2,i2+1,i3)*sy(i1-2,i2+1,i3)-ry(i1-2,i2+1,i3)*sx(i1-2,i2+1,i3)))
     & +128*(rsxy(i1+1,i2-1,i3,axisp1,1)/(rx(i1+1,i2-1,i3)*sy(i1+1,i2-
     & 1,i3)-ry(i1+1,i2-1,i3)*sx(i1+1,i2-1,i3)))-128*(rsxy(i1-1,i2-1,
     & i3,axisp1,1)/(rx(i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-ry(i1-1,i2-1,
     & i3)*sx(i1-1,i2-1,i3)))-16*(rsxy(i1+2,i2-1,i3,axisp1,1)/(rx(i1+
     & 2,i2-1,i3)*sy(i1+2,i2-1,i3)-ry(i1+2,i2-1,i3)*sx(i1+2,i2-1,i3)))
     & +16*(rsxy(i1-2,i2-1,i3,axisp1,1)/(rx(i1-2,i2-1,i3)*sy(i1-2,i2-
     & 1,i3)-ry(i1-2,i2-1,i3)*sx(i1-2,i2-1,i3)))+30*(rsxy(i1+2,i2,i3,
     & axisp1,1)/(rx(i1+2,i2,i3)*sy(i1+2,i2,i3)-ry(i1+2,i2,i3)*sx(i1+
     & 2,i2,i3)))+8*(rsxy(i1-1,i2+2,i3,axisp1,1)/(rx(i1-1,i2+2,i3)*sy(
     & i1-1,i2+2,i3)-ry(i1-1,i2+2,i3)*sx(i1-1,i2+2,i3)))+(rsxy(i1+2,
     & i2+2,i3,axisp1,1)/(rx(i1+2,i2+2,i3)*sy(i1+2,i2+2,i3)-ry(i1+2,
     & i2+2,i3)*sx(i1+2,i2+2,i3)))-(rsxy(i1-2,i2+2,i3,axisp1,1)/(rx(
     & i1-2,i2+2,i3)*sy(i1-2,i2+2,i3)-ry(i1-2,i2+2,i3)*sx(i1-2,i2+2,
     & i3)))-8*(rsxy(i1+1,i2-2,i3,axisp1,1)/(rx(i1+1,i2-2,i3)*sy(i1+1,
     & i2-2,i3)-ry(i1+1,i2-2,i3)*sx(i1+1,i2-2,i3)))+8*(rsxy(i1-1,i2-2,
     & i3,axisp1,1)/(rx(i1-1,i2-2,i3)*sy(i1-1,i2-2,i3)-ry(i1-1,i2-2,
     & i3)*sx(i1-1,i2-2,i3)))+(rsxy(i1+2,i2-2,i3,axisp1,1)/(rx(i1+2,
     & i2-2,i3)*sy(i1+2,i2-2,i3)-ry(i1+2,i2-2,i3)*sx(i1+2,i2-2,i3)))-(
     & rsxy(i1-2,i2-2,i3,axisp1,1)/(rx(i1-2,i2-2,i3)*sy(i1-2,i2-2,i3)-
     & ry(i1-2,i2-2,i3)*sx(i1-2,i2-2,i3)))-8*(rsxy(i1+1,i2+2,i3,
     & axisp1,1)/(rx(i1+1,i2+2,i3)*sy(i1+1,i2+2,i3)-ry(i1+1,i2+2,i3)*
     & sx(i1+1,i2+2,i3)))-30*(rsxy(i1-2,i2,i3,axisp1,1)/(rx(i1-2,i2,
     & i3)*sy(i1-2,i2,i3)-ry(i1-2,i2,i3)*sx(i1-2,i2,i3)))-240*(rsxy(
     & i1+1,i2,i3,axisp1,1)/(rx(i1+1,i2,i3)*sy(i1+1,i2,i3)-ry(i1+1,i2,
     & i3)*sx(i1+1,i2,i3)))+240*(rsxy(i1-1,i2,i3,axisp1,1)/(rx(i1-1,
     & i2,i3)*sy(i1-1,i2,i3)-ry(i1-1,i2,i3)*sx(i1-1,i2,i3))))/(144.*
     & dr(1)**2*dr(0))
                  else
                    a11rss = (128*(rsxy(i1+1,i2+1,i3,axis,0)/(rx(i1+1,
     & i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+1,i3)))-
     & 240*(rsxy(i1,i2+1,i3,axis,0)/(rx(i1,i2+1,i3)*sy(i1,i2+1,i3)-ry(
     & i1,i2+1,i3)*sx(i1,i2+1,i3)))+128*(rsxy(i1-1,i2+1,i3,axis,0)/(
     & rx(i1-1,i2+1,i3)*sy(i1-1,i2+1,i3)-ry(i1-1,i2+1,i3)*sx(i1-1,i2+
     & 1,i3)))-8*(rsxy(i1+2,i2+1,i3,axis,0)/(rx(i1+2,i2+1,i3)*sy(i1+2,
     & i2+1,i3)-ry(i1+2,i2+1,i3)*sx(i1+2,i2+1,i3)))-8*(rsxy(i1-2,i2+1,
     & i3,axis,0)/(rx(i1-2,i2+1,i3)*sy(i1-2,i2+1,i3)-ry(i1-2,i2+1,i3)*
     & sx(i1-2,i2+1,i3)))-128*(rsxy(i1+1,i2-1,i3,axis,0)/(rx(i1+1,i2-
     & 1,i3)*sy(i1+1,i2-1,i3)-ry(i1+1,i2-1,i3)*sx(i1+1,i2-1,i3)))+240*
     & (rsxy(i1,i2-1,i3,axis,0)/(rx(i1,i2-1,i3)*sy(i1,i2-1,i3)-ry(i1,
     & i2-1,i3)*sx(i1,i2-1,i3)))-128*(rsxy(i1-1,i2-1,i3,axis,0)/(rx(
     & i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-ry(i1-1,i2-1,i3)*sx(i1-1,i2-1,
     & i3)))+8*(rsxy(i1+2,i2-1,i3,axis,0)/(rx(i1+2,i2-1,i3)*sy(i1+2,
     & i2-1,i3)-ry(i1+2,i2-1,i3)*sx(i1+2,i2-1,i3)))+8*(rsxy(i1-2,i2-1,
     & i3,axis,0)/(rx(i1-2,i2-1,i3)*sy(i1-2,i2-1,i3)-ry(i1-2,i2-1,i3)*
     & sx(i1-2,i2-1,i3)))+30*(rsxy(i1,i2+2,i3,axis,0)/(rx(i1,i2+2,i3)*
     & sy(i1,i2+2,i3)-ry(i1,i2+2,i3)*sx(i1,i2+2,i3)))-16*(rsxy(i1-1,
     & i2+2,i3,axis,0)/(rx(i1-1,i2+2,i3)*sy(i1-1,i2+2,i3)-ry(i1-1,i2+
     & 2,i3)*sx(i1-1,i2+2,i3)))+(rsxy(i1+2,i2+2,i3,axis,0)/(rx(i1+2,
     & i2+2,i3)*sy(i1+2,i2+2,i3)-ry(i1+2,i2+2,i3)*sx(i1+2,i2+2,i3)))+(
     & rsxy(i1-2,i2+2,i3,axis,0)/(rx(i1-2,i2+2,i3)*sy(i1-2,i2+2,i3)-
     & ry(i1-2,i2+2,i3)*sx(i1-2,i2+2,i3)))+16*(rsxy(i1+1,i2-2,i3,axis,
     & 0)/(rx(i1+1,i2-2,i3)*sy(i1+1,i2-2,i3)-ry(i1+1,i2-2,i3)*sx(i1+1,
     & i2-2,i3)))-30*(rsxy(i1,i2-2,i3,axis,0)/(rx(i1,i2-2,i3)*sy(i1,
     & i2-2,i3)-ry(i1,i2-2,i3)*sx(i1,i2-2,i3)))+16*(rsxy(i1-1,i2-2,i3,
     & axis,0)/(rx(i1-1,i2-2,i3)*sy(i1-1,i2-2,i3)-ry(i1-1,i2-2,i3)*sx(
     & i1-1,i2-2,i3)))-(rsxy(i1+2,i2-2,i3,axis,0)/(rx(i1+2,i2-2,i3)*
     & sy(i1+2,i2-2,i3)-ry(i1+2,i2-2,i3)*sx(i1+2,i2-2,i3)))-(rsxy(i1-
     & 2,i2-2,i3,axis,0)/(rx(i1-2,i2-2,i3)*sy(i1-2,i2-2,i3)-ry(i1-2,
     & i2-2,i3)*sx(i1-2,i2-2,i3)))-16*(rsxy(i1+1,i2+2,i3,axis,0)/(rx(
     & i1+1,i2+2,i3)*sy(i1+1,i2+2,i3)-ry(i1+1,i2+2,i3)*sx(i1+1,i2+2,
     & i3))))/(144.*dr(0)**2*dr(1))
                    a12rss = (128*(rsxy(i1+1,i2+1,i3,axis,1)/(rx(i1+1,
     & i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+1,i3)))-
     & 240*(rsxy(i1,i2+1,i3,axis,1)/(rx(i1,i2+1,i3)*sy(i1,i2+1,i3)-ry(
     & i1,i2+1,i3)*sx(i1,i2+1,i3)))+128*(rsxy(i1-1,i2+1,i3,axis,1)/(
     & rx(i1-1,i2+1,i3)*sy(i1-1,i2+1,i3)-ry(i1-1,i2+1,i3)*sx(i1-1,i2+
     & 1,i3)))-8*(rsxy(i1+2,i2+1,i3,axis,1)/(rx(i1+2,i2+1,i3)*sy(i1+2,
     & i2+1,i3)-ry(i1+2,i2+1,i3)*sx(i1+2,i2+1,i3)))-8*(rsxy(i1-2,i2+1,
     & i3,axis,1)/(rx(i1-2,i2+1,i3)*sy(i1-2,i2+1,i3)-ry(i1-2,i2+1,i3)*
     & sx(i1-2,i2+1,i3)))-128*(rsxy(i1+1,i2-1,i3,axis,1)/(rx(i1+1,i2-
     & 1,i3)*sy(i1+1,i2-1,i3)-ry(i1+1,i2-1,i3)*sx(i1+1,i2-1,i3)))+240*
     & (rsxy(i1,i2-1,i3,axis,1)/(rx(i1,i2-1,i3)*sy(i1,i2-1,i3)-ry(i1,
     & i2-1,i3)*sx(i1,i2-1,i3)))-128*(rsxy(i1-1,i2-1,i3,axis,1)/(rx(
     & i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-ry(i1-1,i2-1,i3)*sx(i1-1,i2-1,
     & i3)))+8*(rsxy(i1+2,i2-1,i3,axis,1)/(rx(i1+2,i2-1,i3)*sy(i1+2,
     & i2-1,i3)-ry(i1+2,i2-1,i3)*sx(i1+2,i2-1,i3)))+8*(rsxy(i1-2,i2-1,
     & i3,axis,1)/(rx(i1-2,i2-1,i3)*sy(i1-2,i2-1,i3)-ry(i1-2,i2-1,i3)*
     & sx(i1-2,i2-1,i3)))+30*(rsxy(i1,i2+2,i3,axis,1)/(rx(i1,i2+2,i3)*
     & sy(i1,i2+2,i3)-ry(i1,i2+2,i3)*sx(i1,i2+2,i3)))-16*(rsxy(i1-1,
     & i2+2,i3,axis,1)/(rx(i1-1,i2+2,i3)*sy(i1-1,i2+2,i3)-ry(i1-1,i2+
     & 2,i3)*sx(i1-1,i2+2,i3)))+(rsxy(i1+2,i2+2,i3,axis,1)/(rx(i1+2,
     & i2+2,i3)*sy(i1+2,i2+2,i3)-ry(i1+2,i2+2,i3)*sx(i1+2,i2+2,i3)))+(
     & rsxy(i1-2,i2+2,i3,axis,1)/(rx(i1-2,i2+2,i3)*sy(i1-2,i2+2,i3)-
     & ry(i1-2,i2+2,i3)*sx(i1-2,i2+2,i3)))+16*(rsxy(i1+1,i2-2,i3,axis,
     & 1)/(rx(i1+1,i2-2,i3)*sy(i1+1,i2-2,i3)-ry(i1+1,i2-2,i3)*sx(i1+1,
     & i2-2,i3)))-30*(rsxy(i1,i2-2,i3,axis,1)/(rx(i1,i2-2,i3)*sy(i1,
     & i2-2,i3)-ry(i1,i2-2,i3)*sx(i1,i2-2,i3)))+16*(rsxy(i1-1,i2-2,i3,
     & axis,1)/(rx(i1-1,i2-2,i3)*sy(i1-1,i2-2,i3)-ry(i1-1,i2-2,i3)*sx(
     & i1-1,i2-2,i3)))-(rsxy(i1+2,i2-2,i3,axis,1)/(rx(i1+2,i2-2,i3)*
     & sy(i1+2,i2-2,i3)-ry(i1+2,i2-2,i3)*sx(i1+2,i2-2,i3)))-(rsxy(i1-
     & 2,i2-2,i3,axis,1)/(rx(i1-2,i2-2,i3)*sy(i1-2,i2-2,i3)-ry(i1-2,
     & i2-2,i3)*sx(i1-2,i2-2,i3)))-16*(rsxy(i1+1,i2+2,i3,axis,1)/(rx(
     & i1+1,i2+2,i3)*sy(i1+1,i2+2,i3)-ry(i1+1,i2+2,i3)*sx(i1+1,i2+2,
     & i3))))/(144.*dr(0)**2*dr(1))
                    a21rss = (128*(rsxy(i1+1,i2+1,i3,axisp1,0)/(rx(i1+
     & 1,i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+1,i3)))
     & -240*(rsxy(i1,i2+1,i3,axisp1,0)/(rx(i1,i2+1,i3)*sy(i1,i2+1,i3)-
     & ry(i1,i2+1,i3)*sx(i1,i2+1,i3)))+128*(rsxy(i1-1,i2+1,i3,axisp1,
     & 0)/(rx(i1-1,i2+1,i3)*sy(i1-1,i2+1,i3)-ry(i1-1,i2+1,i3)*sx(i1-1,
     & i2+1,i3)))-8*(rsxy(i1+2,i2+1,i3,axisp1,0)/(rx(i1+2,i2+1,i3)*sy(
     & i1+2,i2+1,i3)-ry(i1+2,i2+1,i3)*sx(i1+2,i2+1,i3)))-8*(rsxy(i1-2,
     & i2+1,i3,axisp1,0)/(rx(i1-2,i2+1,i3)*sy(i1-2,i2+1,i3)-ry(i1-2,
     & i2+1,i3)*sx(i1-2,i2+1,i3)))-128*(rsxy(i1+1,i2-1,i3,axisp1,0)/(
     & rx(i1+1,i2-1,i3)*sy(i1+1,i2-1,i3)-ry(i1+1,i2-1,i3)*sx(i1+1,i2-
     & 1,i3)))+240*(rsxy(i1,i2-1,i3,axisp1,0)/(rx(i1,i2-1,i3)*sy(i1,
     & i2-1,i3)-ry(i1,i2-1,i3)*sx(i1,i2-1,i3)))-128*(rsxy(i1-1,i2-1,
     & i3,axisp1,0)/(rx(i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-ry(i1-1,i2-1,
     & i3)*sx(i1-1,i2-1,i3)))+8*(rsxy(i1+2,i2-1,i3,axisp1,0)/(rx(i1+2,
     & i2-1,i3)*sy(i1+2,i2-1,i3)-ry(i1+2,i2-1,i3)*sx(i1+2,i2-1,i3)))+
     & 8*(rsxy(i1-2,i2-1,i3,axisp1,0)/(rx(i1-2,i2-1,i3)*sy(i1-2,i2-1,
     & i3)-ry(i1-2,i2-1,i3)*sx(i1-2,i2-1,i3)))+30*(rsxy(i1,i2+2,i3,
     & axisp1,0)/(rx(i1,i2+2,i3)*sy(i1,i2+2,i3)-ry(i1,i2+2,i3)*sx(i1,
     & i2+2,i3)))-16*(rsxy(i1-1,i2+2,i3,axisp1,0)/(rx(i1-1,i2+2,i3)*
     & sy(i1-1,i2+2,i3)-ry(i1-1,i2+2,i3)*sx(i1-1,i2+2,i3)))+(rsxy(i1+
     & 2,i2+2,i3,axisp1,0)/(rx(i1+2,i2+2,i3)*sy(i1+2,i2+2,i3)-ry(i1+2,
     & i2+2,i3)*sx(i1+2,i2+2,i3)))+(rsxy(i1-2,i2+2,i3,axisp1,0)/(rx(
     & i1-2,i2+2,i3)*sy(i1-2,i2+2,i3)-ry(i1-2,i2+2,i3)*sx(i1-2,i2+2,
     & i3)))+16*(rsxy(i1+1,i2-2,i3,axisp1,0)/(rx(i1+1,i2-2,i3)*sy(i1+
     & 1,i2-2,i3)-ry(i1+1,i2-2,i3)*sx(i1+1,i2-2,i3)))-30*(rsxy(i1,i2-
     & 2,i3,axisp1,0)/(rx(i1,i2-2,i3)*sy(i1,i2-2,i3)-ry(i1,i2-2,i3)*
     & sx(i1,i2-2,i3)))+16*(rsxy(i1-1,i2-2,i3,axisp1,0)/(rx(i1-1,i2-2,
     & i3)*sy(i1-1,i2-2,i3)-ry(i1-1,i2-2,i3)*sx(i1-1,i2-2,i3)))-(rsxy(
     & i1+2,i2-2,i3,axisp1,0)/(rx(i1+2,i2-2,i3)*sy(i1+2,i2-2,i3)-ry(
     & i1+2,i2-2,i3)*sx(i1+2,i2-2,i3)))-(rsxy(i1-2,i2-2,i3,axisp1,0)/(
     & rx(i1-2,i2-2,i3)*sy(i1-2,i2-2,i3)-ry(i1-2,i2-2,i3)*sx(i1-2,i2-
     & 2,i3)))-16*(rsxy(i1+1,i2+2,i3,axisp1,0)/(rx(i1+1,i2+2,i3)*sy(
     & i1+1,i2+2,i3)-ry(i1+1,i2+2,i3)*sx(i1+1,i2+2,i3))))/(144.*dr(0)*
     & *2*dr(1))
                    a22rss = (128*(rsxy(i1+1,i2+1,i3,axisp1,1)/(rx(i1+
     & 1,i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+1,i3)))
     & -240*(rsxy(i1,i2+1,i3,axisp1,1)/(rx(i1,i2+1,i3)*sy(i1,i2+1,i3)-
     & ry(i1,i2+1,i3)*sx(i1,i2+1,i3)))+128*(rsxy(i1-1,i2+1,i3,axisp1,
     & 1)/(rx(i1-1,i2+1,i3)*sy(i1-1,i2+1,i3)-ry(i1-1,i2+1,i3)*sx(i1-1,
     & i2+1,i3)))-8*(rsxy(i1+2,i2+1,i3,axisp1,1)/(rx(i1+2,i2+1,i3)*sy(
     & i1+2,i2+1,i3)-ry(i1+2,i2+1,i3)*sx(i1+2,i2+1,i3)))-8*(rsxy(i1-2,
     & i2+1,i3,axisp1,1)/(rx(i1-2,i2+1,i3)*sy(i1-2,i2+1,i3)-ry(i1-2,
     & i2+1,i3)*sx(i1-2,i2+1,i3)))-128*(rsxy(i1+1,i2-1,i3,axisp1,1)/(
     & rx(i1+1,i2-1,i3)*sy(i1+1,i2-1,i3)-ry(i1+1,i2-1,i3)*sx(i1+1,i2-
     & 1,i3)))+240*(rsxy(i1,i2-1,i3,axisp1,1)/(rx(i1,i2-1,i3)*sy(i1,
     & i2-1,i3)-ry(i1,i2-1,i3)*sx(i1,i2-1,i3)))-128*(rsxy(i1-1,i2-1,
     & i3,axisp1,1)/(rx(i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-ry(i1-1,i2-1,
     & i3)*sx(i1-1,i2-1,i3)))+8*(rsxy(i1+2,i2-1,i3,axisp1,1)/(rx(i1+2,
     & i2-1,i3)*sy(i1+2,i2-1,i3)-ry(i1+2,i2-1,i3)*sx(i1+2,i2-1,i3)))+
     & 8*(rsxy(i1-2,i2-1,i3,axisp1,1)/(rx(i1-2,i2-1,i3)*sy(i1-2,i2-1,
     & i3)-ry(i1-2,i2-1,i3)*sx(i1-2,i2-1,i3)))+30*(rsxy(i1,i2+2,i3,
     & axisp1,1)/(rx(i1,i2+2,i3)*sy(i1,i2+2,i3)-ry(i1,i2+2,i3)*sx(i1,
     & i2+2,i3)))-16*(rsxy(i1-1,i2+2,i3,axisp1,1)/(rx(i1-1,i2+2,i3)*
     & sy(i1-1,i2+2,i3)-ry(i1-1,i2+2,i3)*sx(i1-1,i2+2,i3)))+(rsxy(i1+
     & 2,i2+2,i3,axisp1,1)/(rx(i1+2,i2+2,i3)*sy(i1+2,i2+2,i3)-ry(i1+2,
     & i2+2,i3)*sx(i1+2,i2+2,i3)))+(rsxy(i1-2,i2+2,i3,axisp1,1)/(rx(
     & i1-2,i2+2,i3)*sy(i1-2,i2+2,i3)-ry(i1-2,i2+2,i3)*sx(i1-2,i2+2,
     & i3)))+16*(rsxy(i1+1,i2-2,i3,axisp1,1)/(rx(i1+1,i2-2,i3)*sy(i1+
     & 1,i2-2,i3)-ry(i1+1,i2-2,i3)*sx(i1+1,i2-2,i3)))-30*(rsxy(i1,i2-
     & 2,i3,axisp1,1)/(rx(i1,i2-2,i3)*sy(i1,i2-2,i3)-ry(i1,i2-2,i3)*
     & sx(i1,i2-2,i3)))+16*(rsxy(i1-1,i2-2,i3,axisp1,1)/(rx(i1-1,i2-2,
     & i3)*sy(i1-1,i2-2,i3)-ry(i1-1,i2-2,i3)*sx(i1-1,i2-2,i3)))-(rsxy(
     & i1+2,i2-2,i3,axisp1,1)/(rx(i1+2,i2-2,i3)*sy(i1+2,i2-2,i3)-ry(
     & i1+2,i2-2,i3)*sx(i1+2,i2-2,i3)))-(rsxy(i1-2,i2-2,i3,axisp1,1)/(
     & rx(i1-2,i2-2,i3)*sy(i1-2,i2-2,i3)-ry(i1-2,i2-2,i3)*sx(i1-2,i2-
     & 2,i3)))-16*(rsxy(i1+1,i2+2,i3,axisp1,1)/(rx(i1+1,i2+2,i3)*sy(
     & i1+1,i2+2,i3)-ry(i1+1,i2+2,i3)*sx(i1+1,i2+2,i3))))/(144.*dr(0)*
     & *2*dr(1))
                  end if
                  ! fourth-order:
                  c11s = (8.*((rsxy(i1+js1,i2+js2,i3,axis,0)**2+rsxy(
     & i1+js1,i2+js2,i3,axis,1)**2)-(rsxy(i1-js1,i2-js2,i3,axis,0)**2+
     & rsxy(i1-js1,i2-js2,i3,axis,1)**2))   -((rsxy(i1+2*js1,i2+2*js2,
     & i3,axis,0)**2+rsxy(i1+2*js1,i2+2*js2,i3,axis,1)**2)-(rsxy(i1-2*
     & js1,i2-2*js2,i3,axis,0)**2+rsxy(i1-2*js1,i2-2*js2,i3,axis,1)**
     & 2))   )/(12.*dsa)
                  c22s = (8.*((rsxy(i1+js1,i2+js2,i3,axisp1,0)**2+rsxy(
     & i1+js1,i2+js2,i3,axisp1,1)**2)-(rsxy(i1-js1,i2-js2,i3,axisp1,0)
     & **2+rsxy(i1-js1,i2-js2,i3,axisp1,1)**2))   -((rsxy(i1+2*js1,i2+
     & 2*js2,i3,axisp1,0)**2+rsxy(i1+2*js1,i2+2*js2,i3,axisp1,1)**2)-(
     & rsxy(i1-2*js1,i2-2*js2,i3,axisp1,0)**2+rsxy(i1-2*js1,i2-2*js2,
     & i3,axisp1,1)**2))   )/(12.*dsa)
                  if( axis.eq.0 )then
                    c1s = (rsxyxs42(i1,i2,i3,axis,0)+rsxyys42(i1,i2,i3,
     & axis,1))
                    c2s = (rsxyxs42(i1,i2,i3,axisp1,0)+rsxyys42(i1,i2,
     & i3,axisp1,1))
                  else
                    c1s = (rsxyxr42(i1,i2,i3,axis,0)+rsxyyr42(i1,i2,i3,
     & axis,1))
                    c2s = (rsxyxr42(i1,i2,i3,axisp1,0)+rsxyyr42(i1,i2,
     & i3,axisp1,1))
                  end if
                  c11r = (8.*((rsxy(i1+is1,i2+is2,i3,axis,0)**2+rsxy(
     & i1+is1,i2+is2,i3,axis,1)**2)-(rsxy(i1-is1,i2-is2,i3,axis,0)**2+
     & rsxy(i1-is1,i2-is2,i3,axis,1)**2))   -((rsxy(i1+2*is1,i2+2*is2,
     & i3,axis,0)**2+rsxy(i1+2*is1,i2+2*is2,i3,axis,1)**2)-(rsxy(i1-2*
     & is1,i2-2*is2,i3,axis,0)**2+rsxy(i1-2*is1,i2-2*is2,i3,axis,1)**
     & 2))   )/(12.*dra)
                  c22r = (8.*((rsxy(i1+is1,i2+is2,i3,axisp1,0)**2+rsxy(
     & i1+is1,i2+is2,i3,axisp1,1)**2)-(rsxy(i1-is1,i2-is2,i3,axisp1,0)
     & **2+rsxy(i1-is1,i2-is2,i3,axisp1,1)**2))   -((rsxy(i1+2*is1,i2+
     & 2*is2,i3,axisp1,0)**2+rsxy(i1+2*is1,i2+2*is2,i3,axisp1,1)**2)-(
     & rsxy(i1-2*is1,i2-2*is2,i3,axisp1,0)**2+rsxy(i1-2*is1,i2-2*is2,
     & i3,axisp1,1)**2))   )/(12.*dra)
                  if( axis.eq.0 )then
                    c1r = (rsxyxr42(i1,i2,i3,axis,0)+rsxyyr42(i1,i2,i3,
     & axis,1))
                    c2r = (rsxyxr42(i1,i2,i3,axisp1,0)+rsxyyr42(i1,i2,
     & i3,axisp1,1))
                  else
                    c1r = (rsxyxs42(i1,i2,i3,axis,0)+rsxyys42(i1,i2,i3,
     & axis,1))
                    c2r = (rsxyxs42(i1,i2,i3,axisp1,0)+rsxyys42(i1,i2,
     & i3,axisp1,1))
                  end if
                  g1a = a21rrs*uex+a22rrs*uey + a21rr*us+a22rr*vs +2.*(
     &  a21rs*ur+a22rs*vr +a21r*urs+a22r*vrs ) +a21s*urr+a22s*vrr + 
     & a21*urrs+a22*vrrs
                  g2a=dr3aDotU+g1a
      b3u=c11*a11
      b3v=a12*c11
      b2u=(2*c22*a12s*a11**2+c1*a11**2*a22+c11r*a11**2*a22-2*c22*a11s*
     & a12*a11-c1*a11*a21*a12-c11r*a11*a21*a12-a11r*c11*a21*a12+a11r*
     & c11*a11*a22)/(-a21*a12+a11*a22)
      b2v=(-2*c22*a11s*a12**2+2*c22*a12s*a11*a12+a12*c1*a11*a22+a12*
     & c11r*a11*a22-a12r*c11*a21*a12+a12r*c11*a11*a22-c11r*a21*a12**2-
     & c1*a21*a12**2)/(-a21*a12+a11*a22)
      b1u=(-4*c22*a11s*a12*a11r-c22*a11ss*a11*a22+c1r*a11**2*a22+4*c22*
     & a12s*a11*a11r-c1r*a11*a21*a12+2*c22*a11s**2*a22-2*c22*a12s*a21*
     & a11s-2*c22*a11s*a12*a21s+2*c22*a12s*a11*a21s-c2*a11*a11s*a22+
     & c22*a11ss*a21*a12+a12*c2*a21*a11s-a11r*c1*a21*a12+a11r*c1*a11*
     & a22)/(-a21*a12+a11*a22)
      b1v=(-2*c22*a21*a12s**2+2*c22*a11s*a12s*a22+2*c22*a12s*a11*a22s-
     & 2*c22*a11s*a12*a22s+c22*a12ss*a21*a12+4*c22*a12s*a11*a12r-c22*
     & a12ss*a11*a22-4*c22*a11s*a12*a12r+a12*c1r*a11*a22+a12*c2*a21*
     & a12s-a12r*c1*a21*a12+a12r*c1*a11*a22-c1r*a21*a12**2-c2*a11*
     & a12s*a22)/(-a21*a12+a11*a22)
      bf =-(-2*c22*a12s*a11*a21rs*uex+2*c22*a12s*a21*a21ss*uex+3*c22*
     & a22s*vss*a11*a22-c22r*uss*a11**2*a22-2*c22*a11s*a22**2*vss-c22*
     & a21**2*usss*a12+c22*a22**2*vsss*a11+2*c22*a12s*a21**2*uss-c2r*
     & us*a11**2*a22+c2*a11*a22**2*vss-3*c22*a21ss*us*a21*a12-2*c22*
     & a12s*a11*a11rr*uex+3*c22*a21ss*us*a11*a22-3*c22*a22ss*vs*a21*
     & a12+2*c22*a11s*a12*a22r*vs+3*c22*a22ss*vs*a11*a22+c2r*us*a11*
     & a21*a12+2*c2*a11*a22s*vs*a22-c22*a22*vsss*a21*a12-3*c22*a21s*
     & uss*a21*a12+3*c22*a21s*uss*a11*a22-c22*a12rss*uey*a21*a12+c22*
     & a12rss*uey*a11*a22-c22*a11rss*uex*a21*a12+2*c22*a12s*a21*a22ss*
     & uey+2*c22*a12s*a21*a22*vss+4*c22*a12s*a21*a21s*us+c22*a11rss*
     & uex*a11*a22-2*c22*a12rs*vs*a21*a12+2*c22*a12rs*vs*a11*a22-3*
     & c22*a22s*vss*a21*a12-c22*a21sss*uex*a21*a12+c22*a21sss*uex*a11*
     & a22-2*c22*a11rs*us*a21*a12+2*c22*a12s*a21*a11rs*uex+2*c22*a12s*
     & a21*a11r*us+2*c22*a11s*a12*a21r*us+2*c22*a11s*a12*a21rs*uex-2*
     & c22*a12s*a11*a21r*us-2*c22*a12s*a11*a22rs*uey-2*c22*a12s*a11*
     & a22r*vs+2*c22*a12s*a21*a12r*vs+2*c22*a11s*a12*a22rs*uey+2*c22*
     & a11rs*us*a11*a22+c2*a11*a21*uss*a22+c2*a11*a11rs*uex*a22-2*c22*
     & a11s*a12rs*uey*a22+c22*a22sss*uey*a11*a22-2*c22*a12s*a11*a12rr*
     & uey+4*c22*a12s*a21*a22s*vs+2*c22*a12s*a21*a12rs*uey-2*c22*a11s*
     & a21ss*uex*a22-2*c22*a11s*a22ss*uey*a22+2*c22*a11s*a12*a11rr*
     & uex-4*c22*a11s*a22s*vs*a22-2*c22*a11s*a11rs*uex*a22-2*c22*a11s*
     & a21*uss*a22-4*c22*a11s*a21s*us*a22-2*c22*a11s*a11r*us*a22+2*
     & c22*a11s*a12*a12rr*uey+c22*a21*usss*a11*a22-c22*a22sss*uey*a21*
     & a12-2*c22*a11s*a12r*vs*a22+c22r*uss*a11*a21*a12-a12*c22r*vss*
     & a11*a22-a12*c2r*vs*a11*a22-a12*c2*a21*a12rs*uey-2*a12*c2*a21*
     & a22s*vs-a12*c2*a21*a11rs*uex-a12*c2*a21*a22*vss-2*a12*c2*a21*
     & a21s*us-a12*c2*a21*a21ss*uex-a12*c2*a21*a22ss*uey-a12*c2*a21**
     & 2*uss+c2r*vs*a21*a12**2+c22r*vss*a21*a12**2+c2*a11*a12rs*uey*
     & a22+c2*a11*a22ss*uey*a22+c2*a11*a21ss*uex*a22+2*c2*a11*a21s*us*
     & a22)/(-a21*a12+a11*a22)

! -- Here are the approximations for urs, vrs from the divergence
!  ursm =-(2*a22s*vs*a22-a12*a11*urr-a12*a22r*vs-a12*a22s*vr-a12*a22rs*uey-a12*a21r*us-a12*a21s*ur-a12*a21rs*uex-2*a12*a11r*ur-a12*a11rr*uex+a12rs*uey*a22+a12r*vs*a22+a12s*vr*a22+a22ss*uey*a22+a21ss*uex*a22+2*a21s*us*a22+a21*uss*a22+a22**2*vss-a12**2*vrr+a11rs*uex*a22+a11r*us*a22-2*a12*a12r*vr+a11s*ur*a22-a12*a12rr*uey)/(-a21*a12+a11*a22)
!  vrsm =(-a11**2*urr-2*a11*a12r*vr-a11*a12rr*uey+a21*a12r*vs+a21*a12rs*uey+2*a21*a22s*vs+a21*a11s*ur+a21*a11r*us+a21*a11rs*uex+a21*a12s*vr-a11*a22r*vs-a11*a22s*vr-a11*a22rs*uey-a11*a21r*us-a11*a21s*ur-a11*a21rs*uex-a11*a12*vrr+a21*a22*vss+2*a21*a21s*us+a21*a21ss*uex+a21*a22ss*uey-2*a11*a11r*ur+a21**2*uss-a11*a11rr*uex)/(-a21*a12+a11*a22)
                  ! forcing terms for TZ are stored in 
                  cgI=1.
                  gIf=0.
                  tau1DotUtt=0.
                  Da1DotU=0.
                   ! ********** For now do this: should work for quadratics *******************
                   cgI=0.
                   gIf=0.
                   ! ***********************************************
                    ! For TZ: utt0 = utt - ett + Lap(e)
                    call ogDeriv(ep, 0, 2,0,0, xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.,t,ex, uxx)
                    call ogDeriv(ep, 0, 0,2,0, xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.,t,ex, uyy)
                    utt00=uxx+uyy
                    call ogDeriv(ep, 0, 2,0,0, xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.,t,ey, vxx)
                    call ogDeriv(ep, 0, 0,2,0, xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.,t,ey, vyy)
                    vtt00=vxx+vyy
                   tau1DotUtt = tau1*utt00+tau2*vtt00
                   ! ***
                   tauDotLap = tauDotLap - tau1DotUtt
                    call ogf2d(ep,xy(i1,i2,i3,0),xy(i1,i2,i3,1),t,uv0(
     & 0),uv0(1),uv0(2))
                    call ogf2d(ep,xy(i1-is1,i2-is2,i3,0),xy(i1-is1,i2-
     & is2,i3,1),t,uvm(0),uvm(1),uvm(2))
                    call ogf2d(ep,xy(i1+is1,i2+is2,i3,0),xy(i1+is1,i2+
     & is2,i3,1),t,uvp(0),uvp(1),uvp(2))
                    call ogf2d(ep,xy(i1-2*is1,i2-2*is2,i3,0),xy(i1-2*
     & is1,i2-2*is2,i3,1),t,uvm2(0),uvm2(1),uvm2(2))
                    call ogf2d(ep,xy(i1+2*is1,i2+2*is2,i3,0),xy(i1+2*
     & is1,i2+2*is2,i3,1),t,uvp2(0),uvp2(1),uvp2(2))
                   ! Da1DotU = (a1.uv).r to 4th order
                   Da1DotU = (8.*( (a11p1*uvp(0)+a12p1*uvp(1)) - (
     & a11m1*uvm(0)+a12m1*uvm(1)) )- ( (a11p2*uvp2(0)+a12p2*uvp2(1)) -
     &  (a11m2*uvm2(0)+a12m2*uvm2(1)) ) )/(12.*dra)
                  ! for now remove the error in the extrapolation ************
                  !  gIVf = tau1*( uvm2(0)-4.*uvm(0)+6.*uv0(0)-4.*uvp(0)+uvp2(0)) +!         tau2*( uvm2(1)-4.*uvm(1)+6.*uv0(1)-4.*uvp(1)+uvp2(1))
                  gIVf=0.
                   ! *** set to - Ds( a2.uv )
                   Da1DotU = -(  ( 8.*(a21zp1*u(i1+  js1,i2+  js2,i3,
     & ex)-a21zm1*u(i1-  js1,i2-  js2,i3,ex)) -(a21zp2*u(i1+2*js1,i2+
     & 2*js2,i3,ex)-a21zm2*u(i1-2*js1,i2-2*js2,i3,ex)) )/(12.*dsa) +( 
     & 8.*(a22zp1*u(i1+  js1,i2+  js2,i3,ey)-a22zm1*u(i1-  js1,i2-  
     & js2,i3,ey)) -(a22zp2*u(i1+2*js1,i2+2*js2,i3,ey)-a22zm2*u(i1-2*
     & js1,i2-2*js2,i3,ey)) )/(12.*dsa)  )
                   tauU= tauU -( tau1*uv0(0)+tau2*uv0(1) )
                   call ogf2d(ep,xy(i1,i2,i3,0),xy(i1,i2,i3,1),t,uv0(0)
     & ,uv0(1),uv0(2))
                   call ogf2d(ep,xy(i1-is1,i2-is2,i3,0),xy(i1-is1,i2-
     & is2,i3,1),t,uvm(0),uvm(1),uvm(2))
                   call ogf2d(ep,xy(i1+is1,i2+is2,i3,0),xy(i1+is1,i2+
     & is2,i3,1),t,uvp(0),uvp(1),uvp(2))
                   call ogf2d(ep,xy(i1-2*is1,i2-2*is2,i3,0),xy(i1-2*
     & is1,i2-2*is2,i3,1),t,uvm2(0),uvm2(1),uvm2(2))
                  write(*,'(/,"  bc4: (i1,i2,i3)=(",i6,",",i6,",",i6,")
     &  (side,axis)=(",i2,",",i2,")")') i1,i2,i3,side,axis
                  write(*,'("  bc4: u(-1),err, v(-1),err",4e12.3)') u(
     & i1-is1,i2-is2,i3,ex),u(i1-is1,i2-is2,i3,ex)-uvm(0),u(i1-is1,i2-
     & is2,i3,ey),u(i1-is1,i2-is2,i3,ey)-uvm(1)
                  write(*,'("  bc4: u(-2),err, v(-2),err",4e12.3)') u(
     & i1-2*is1,i2-2*is2,i3,ex),u(i1-2*is1,i2-2*is2,i3,ex)-uvm2(0),u(
     & i1-2*is1,i2-2*is2,i3,ey),u(i1-2*is1,i2-2*is2,i3,ey)-uvm2(1)
                  write(*,'("  bc4: err(tau.u)=",e9.2," div4(u)=",e9.2,
     & " divc(u)=",e9.2,", divc2=",e9.2,", tauD+4u=",e9.2)') tauU,div,
     & divc,divc2,tauUp1
                  ! write(*,'("  bc4: a11m2,a11m1,a11,a11p1,a11p2=",5e14.6)') a11m2,a11m1,a11,a11p1,a11p2
                  ! write(*,'("  bc4: a12m2,a12m1,a12,a12p1,a12p2=",5e14.6)') a12m2,a12m1,a12,a12p1,a12p2
                  ! write(*,'("  bc4: a21zm2,a21zm1,a21,a21zp1,a21zp2=",5e14.6)') a21zm2,a21zm1,a21,a21zp1,a21zp2
                  ! write(*,'("  bc4: a22zm2,a22zm1,a22,a22zp1,a22zp2=",5e14.6)') a22zm2,a22zm1,a22,a22zp1,a22zp2
                  g1a= ( 8.*(a11p1*u(i1+  is1,i2+  is2,i3,ex)-a11m1*u(
     & i1-  is1,i2-  is2,i3,ex)) -(a11p2*u(i1+2*is1,i2+2*is2,i3,ex)-
     & a11m2*u(i1-2*is1,i2-2*is2,i3,ex)) )/(12.*dra) +( 8.*(a12p1*u(
     & i1+  is1,i2+  is2,i3,ey)-a12m1*u(i1-  is1,i2-  is2,i3,ey)) -(
     & a12p2*u(i1+2*is1,i2+2*is2,i3,ey)-a12m2*u(i1-2*is1,i2-2*is2,i3,
     & ey)) )/(12.*dra)
                  write(*,'("  bc4: uex,err,uey,err=",4e10.2)') uex,
     & uex-uv0(0),uey,uey-uv0(1)
                  write(*,'("  bc4: d(a1.uv),Da1DotU,err=",5e10.2)') 
     & g1a,Da1DotU,g1a-Da1DotU
                  call ogDeriv(ep, 0, 2,0,0, xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.,t,ex, uxx)
                  call ogDeriv(ep, 0, 0,2,0, xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.,t,ex, uyy)
                  utt00=uxx+uyy
                  call ogDeriv(ep, 0, 2,0,0, xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.,t,ey, vxx)
                  call ogDeriv(ep, 0, 0,2,0, xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.,t,ey, vyy)
                  vtt00=vxx+vyy
                  uLap=uLaplacian42(i1,i2,i3,ex)
                  vLap=uLaplacian42(i1,i2,i3,ey)
                  write(*,'("  bc4: Lu-utt=",e10.2," Lv-vtt=",e10.2," 
     & tau.(L\uv-\uvtt)=",e10.2)') uLap-utt00,vLap-vtt00,tau1*(uLap-
     & utt00)+tau2*(vLap-vtt00)
                   call ogDeriv(ep, 0, 2,0,0, xy(i1-is1,i2-is2,i3,0),
     & xy(i1-is1,i2-is2,i3,1),0.,t,ex, uxxm1)
                   call ogDeriv(ep, 0, 0,2,0, xy(i1-is1,i2-is2,i3,0),
     & xy(i1-is1,i2-is2,i3,1),0.,t,ex, uyym1)
                   call ogDeriv(ep, 0, 2,0,0, xy(i1+is1,i2+is2,i3,0),
     & xy(i1+is1,i2+is2,i3,1),0.,t,ex, uxxp1)
                   call ogDeriv(ep, 0, 0,2,0, xy(i1+is1,i2+is2,i3,0),
     & xy(i1+is1,i2+is2,i3,1),0.,t,ex, uyyp1)
                   call ogDeriv(ep, 0, 2,0,0, xy(i1-is1,i2-is2,i3,0),
     & xy(i1-is1,i2-is2,i3,1),0.,t,ey, vxxm1)
                   call ogDeriv(ep, 0, 0,2,0, xy(i1-is1,i2-is2,i3,0),
     & xy(i1-is1,i2-is2,i3,1),0.,t,ey, vyym1)
                   call ogDeriv(ep, 0, 2,0,0, xy(i1+is1,i2+is2,i3,0),
     & xy(i1+is1,i2+is2,i3,1),0.,t,ey, vxxp1)
                   call ogDeriv(ep, 0, 0,2,0, xy(i1+is1,i2+is2,i3,0),
     & xy(i1+is1,i2+is2,i3,1),0.,t,ey, vyyp1)
                   call ogDeriv(ep, 0, 2,0,0, xy(i1-2*is1,i2-2*is2,i3,
     & 0),xy(i1-2*is1,i2-2*is2,i3,1),0.,t,ex, uxxm2)
                   call ogDeriv(ep, 0, 0,2,0, xy(i1-2*is1,i2-2*is2,i3,
     & 0),xy(i1-2*is1,i2-2*is2,i3,1),0.,t,ex, uyym2)
                   call ogDeriv(ep, 0, 2,0,0, xy(i1+2*is1,i2+2*is2,i3,
     & 0),xy(i1+2*is1,i2+2*is2,i3,1),0.,t,ex, uxxp2)
                   call ogDeriv(ep, 0, 0,2,0, xy(i1+2*is1,i2+2*is2,i3,
     & 0),xy(i1+2*is1,i2+2*is2,i3,1),0.,t,ex, uyyp2)
                   call ogDeriv(ep, 0, 2,0,0, xy(i1-2*is1,i2-2*is2,i3,
     & 0),xy(i1-2*is1,i2-2*is2,i3,1),0.,t,ey, vxxm2)
                   call ogDeriv(ep, 0, 0,2,0, xy(i1-2*is1,i2-2*is2,i3,
     & 0),xy(i1-2*is1,i2-2*is2,i3,1),0.,t,ey, vyym2)
                   call ogDeriv(ep, 0, 2,0,0, xy(i1+2*is1,i2+2*is2,i3,
     & 0),xy(i1+2*is1,i2+2*is2,i3,1),0.,t,ey, vxxp2)
                   call ogDeriv(ep, 0, 0,2,0, xy(i1+2*is1,i2+2*is2,i3,
     & 0),xy(i1+2*is1,i2+2*is2,i3,1),0.,t,ey, vyyp2)
                   ! For TZ choose bf = bf - Dr( a1.Delta uvExact )
                   bf = bf - a11r*utt00 - a12r*vtt00 -a11*( 8.*((uxxp1+
     & uyyp1)-(uxxm1+uyym1))-((uxxp2+uyyp2)-(uxxm2+uyym2)) )/(12.*dra)
     &  -a12*( 8.*((vxxp1+vyyp1)-(vxxm1+vyym1))-((vxxp2+vyyp2)-(vxxm2+
     & vyym2)) )/(12.*dra)
                   divtt=b3u*(u(i1+2*is1,i2+2*is2,i3+2*is3,ex)-2.*u(i1+
     & is1,i2+is2,i3+is3,ex)+2.*u(i1-is1,i2-is2,i3-is3,ex)-u(i1-2*is1,
     & i2-2*is2,i3-2*is3,ex))/(2.*dra**3)+b3v*(u(i1+2*is1,i2+2*is2,i3+
     & 2*is3,ey)-2.*u(i1+is1,i2+is2,i3+is3,ey)+2.*u(i1-is1,i2-is2,i3-
     & is3,ey)-u(i1-2*is1,i2-2*is2,i3-2*is3,ey))/(2.*dra**3)+b2u*(-
     & 30.*u(i1,i2,i3,ex)+16.*(u(i1+is1,i2+is2,i3+is3,ex)+u(i1-is1,i2-
     & is2,i3-is3,ex))-(u(i1+2*is1,i2+2*is2,i3+2*is3,ex)+u(i1-2*is1,
     & i2-2*is2,i3-2*is3,ex)))/(12.*dra**2)+b2v*(-30.*u(i1,i2,i3,ey)+
     & 16.*(u(i1+is1,i2+is2,i3+is3,ey)+u(i1-is1,i2-is2,i3-is3,ey))-(u(
     & i1+2*is1,i2+2*is2,i3+2*is3,ey)+u(i1-2*is1,i2-2*is2,i3-2*is3,ey)
     & ))/(12.*dra**2)+b1u*(8.*(u(i1+is1,i2+is2,i3+is3,ex)-u(i1-is1,
     & i2-is2,i3-is3,ex))-(u(i1+2*is1,i2+2*is2,i3+2*is3,ex)-u(i1-2*
     & is1,i2-2*is2,i3-2*is3,ex)))/(12.*dra)+b1v*(8.*(u(i1+is1,i2+is2,
     & i3+is3,ey)-u(i1-is1,i2-is2,i3-is3,ey))-(u(i1+2*is1,i2+2*is2,i3+
     & 2*is3,ey)-u(i1-2*is1,i2-2*is2,i3-2*is3,ey)))/(12.*dra)+bf
                   write(*,'("  bc4: divtt=b3u*urrr2(ex)+b3v*urrr2(ey)+
     & ...+bf",e10.2)') divtt
                   ! write(*,'("  bc4:b3u,b3v,b2u,b2v,b1u,b1v,bf,a11r,a12r=",9e10.2)') b3u,b3v,b2u,b2v,b1u,b1v,bf,a11r,a12r
                   divtt=a11 *( c11*urrr+c22*urss+c1*urr+c2*urs + c11r*
     & urr+c22r*uss+c1r*ur+c2r*us ) +a11r*( c11*urr+c22*uss+c1*ur+c2*
     & us ) +a12 *( c11*vrrr+c22*vrss+c1*vrr+c2*vrs + c11r*vrr+c22r*
     & vss+c1r*vr+c2r*vs ) +a12r*( c11*vrr+c22*vss+c1*vr+c2*vs )
                   bf =  - a11r*utt00 - a12r*vtt00 -a11*( 8.*((uxxp1+
     & uyyp1)-(uxxm1+uyym1))-((uxxp2+uyyp2)-(uxxm2+uyym2)) )/(12.*dra)
     &  -a12*( 8.*((vxxp1+vyyp1)-(vxxm1+vyym1))-((vxxp2+vyyp2)-(vxxm2+
     & vyym2)) )/(12.*dra)
                   write(*,'("  bc4: (a.Lu).r - rhs=",e10.2," (a.Lu)
     & .r=",e10.2," rhs=",e10.2)') divtt+bf,divtt,bf
                   ! write(*,'("  bc4: urrr,vrrr,urss,vrss,urr,vrr,urs,vrs,uss,vss,ur,vr,us,vs=",15e10.2)') urrr,vrrr,urss,vrss,urr,vrr,urs,vrs,uss,vss,ur,vr,us,vs
                  ! gIa+cur*ur+cvr*vr = ( a2.uv).rrs
                  ! write(*,'("  bc4: cur,cvr=",2e10.2)') cur,cvr
                  ! write(*,'("  bc4: dr3aDotU=",e10.2," gIf=",e10.2," ,err=",e12.4)') dr3aDotU,gIf,dr3aDotU-gIf
                  tauDotExtrap=tau1*( u(i1-2*is1,i2-2*is2,i3,ex)-4.*u(
     & i1-is1,i2-is2,i3,ex)+6.*u(i1,i2,i3,ex) -4.*u(i1+is1,i2+is2,i3,
     & ex)+u(i1+2*is1,i2+2*is2,i3,ex)) +tau2*( u(i1-2*is1,i2-2*is2,i3,
     & ey)-4.*u(i1-is1,i2-is2,i3,ey)+6.*u(i1,i2,i3,ey) -4.*u(i1+is1,
     & i2+is2,i3,ey)+u(i1+2*is1,i2+2*is2,i3,ey))
                  write(*,'("  bc4: tauDotD+4(uv)-gIVf=",2e10.2)') 
     & tauDotExtrap-gIVf
                  tauDotExtrap=tau1*( u(i1-2*is1,i2-2*is2,i3,ex)-5.*u(
     & i1-is1,i2-is2,i3,ex)+10.*u(i1,i2,i3,ex) -10.*u(i1+is1,i2+is2,
     & i3,ex)+5.*u(i1+2*is1,i2+2*is2,i3,ex)-u(i1+3*is1,i2+3*is2,i3,ex)
     & ) +tau2*( u(i1-2*is1,i2-2*is2,i3,ey)-5.*u(i1-is1,i2-is2,i3,ey)+
     & 10.*u(i1,i2,i3,ey) -10.*u(i1+is1,i2+is2,i3,ey)+5.*u(i1+2*is1,
     & i2+2*is2,i3,ey)-u(i1+3*is1,i2+3*is2,i3,ey))
                  write(*,'("  bc4: tauDotD+5(uv)-gIVf=",2e10.2)') 
     & tauDotExtrap-gIVf
                  write(*,'("  bc4: tau.Lap=",e9.2,", err(lap)=",2e9.1,
     & " dr3aDotU-cur*Du-cvr*Dv,g1a,sum=",3e10.2)') tauDotLap,
     & errLapex,errLapey,dr3aDotU,g1a,g2a
                 !write(*,'("  bc4: gIa=",e10.2') gIa
                  write(*,'("  bc4: err(tau1.u(-1,-2))=",2e10.2)')tau1*
     & (u(i1-is1,i2-is2,i3-is3,ex)-uvm(0))+tau2*(u(i1-is1,i2-is2,i3-
     & is3,ey)-uvm(1)),tau1*(u(i1-2*is1,i2-2*is2,i3-2*is3,ex)-uvm2(0))
     & +tau2*(u(i1-2*is1,i2-2*is2,i3-2*is3,ey)-uvm2(1))
                   ! ****** compute the error in Dr( a1.Delta u ) + bf = 0
                   if( axis.eq.0 )then
                     urrr=urrr2(i1,i2,i3,ex)
                     urr = urr4(i1,i2,i3,ex)
                     ur  =  ur4(i1,i2,i3,ex)
                     urs = urs2(i1,i2,i3,ex)  ! don't use a wide stencil since pts missing near boundaries
                     urss=urss2(i1,i2,i3,ex)  ! we should maybe use the expression computed from bcdiv3d
                     vrrr=urrr2(i1,i2,i3,ey)
                     vrr = urr4(i1,i2,i3,ey)
                     vr  =  ur4(i1,i2,i3,ey)
                     vrs = urs2(i1,i2,i3,ey)
                     vrss=urss2(i1,i2,i3,ey)
                   else if( axis.eq.1 )then
                     urrr=usss2(i1,i2,i3,ex)
                     urr = uss4(i1,i2,i3,ex)
                     ur  =  us4(i1,i2,i3,ex)
                     urs = urs2(i1,i2,i3,ex)
                     urss=urrs2(i1,i2,i3,ex)  ! we should maybe use the expression computed from bcdiv3d
                     vrrr=usss2(i1,i2,i3,ey)
                     vrr = uss4(i1,i2,i3,ey)
                     vr  =  us4(i1,i2,i3,ey)
                     vrs = urs2(i1,i2,i3,ey)
                     vrss=urrs2(i1,i2,i3,ey)
                   end if
                   drA1DotDeltaU = a11*( c11*urrr+ c22*urss + c1*urr + 
     & c2*urs +c11r*urr+c22r*uss+c1r*ur+c2r*us) + a11r*uLap +a12*( 
     & c11*vrrr+ c22*vrss + c1*vrr + c2*vrs +c11r*vrr+c22r*vss+c1r*vr+
     & c2r*vs) + a12r*vLap +bf
                   write(*,'(" error in (a1.Delta u).r + bf = ",e11.3)
     & ') drA1DotDeltaU
                   call ogf2d(ep,xy(i1,i2,i3,0),xy(i1,i2,i3,1),t,uv0(0)
     & ,uv0(1),uv0(2))
                   call ogf2d(ep,xy(i1-is1,i2-is2,i3,0),xy(i1-is1,i2-
     & is2,i3,1),t,uvm(0),uvm(1),uvm(2))
                   call ogf2d(ep,xy(i1+is1,i2+is2,i3,0),xy(i1+is1,i2+
     & is2,i3,1),t,uvp(0),uvp(1),uvp(2))
                   call ogf2d(ep,xy(i1-2*is1,i2-2*is2,i3,0),xy(i1-2*
     & is1,i2-2*is2,i3,1),t,uvm2(0),uvm2(1),uvm2(2))
                   call ogf2d(ep,xy(i1+2*is1,i2+2*is2,i3,0),xy(i1+2*
     & is1,i2+2*is2,i3,1),t,uvp2(0),uvp2(1),uvp2(2))
                  ! compute exact ur,urr for testing ursm formula
                  uex=uv0(0)
                  uey=uv0(1)
                  urr=(-30.*uv0(0)+16.*(uvp(0)+uvm(0))-(uvp2(0)+uvm2(0)
     & ) )/(12.*dra**2)
                  vrr=(-30.*uv0(1)+16.*(uvp(1)+uvm(1))-(uvp2(1)+uvm2(1)
     & ) )/(12.*dra**2)
                  ur=(8.*(uvp(0)-uvm(0))-(uvp2(0)-uvm2(0)))/(12.*dra)
                  vr=(8.*(uvp(1)-uvm(1))-(uvp2(1)-uvm2(1)))/(12.*dra)
                   call ogf2d(ep,xy(i1,i2,i3,0),xy(i1,i2,i3,1),t,uv0(0)
     & ,uv0(1),uv0(2))
                   call ogf2d(ep,xy(i1-js1,i2-js2,i3,0),xy(i1-js1,i2-
     & js2,i3,1),t,uvm(0),uvm(1),uvm(2))
                   call ogf2d(ep,xy(i1+js1,i2+js2,i3,0),xy(i1+js1,i2+
     & js2,i3,1),t,uvp(0),uvp(1),uvp(2))
                   call ogf2d(ep,xy(i1-2*js1,i2-2*js2,i3,0),xy(i1-2*
     & js1,i2-2*js2,i3,1),t,uvm2(0),uvm2(1),uvm2(2))
                   call ogf2d(ep,xy(i1+2*js1,i2+2*js2,i3,0),xy(i1+2*
     & js1,i2+2*js2,i3,1),t,uvp2(0),uvp2(1),uvp2(2))
                  uss=(-30.*uv0(0)+16.*(uvp(0)+uvm(0))-(uvp2(0)+uvm2(0)
     & ) )/(12.*dsa**2)
                  vss=(-30.*uv0(1)+16.*(uvp(1)+uvm(1))-(uvp2(1)+uvm2(1)
     & ) )/(12.*dsa**2)
                  us=(8.*(uvp(0)-uvm(0))-(uvp2(0)-uvm2(0)))/(12.*dsa)
                  vs=(8.*(uvp(1)-uvm(1))-(uvp2(1)-uvm2(1)))/(12.*dsa)
                 ! These are from bcdiv.maple (includes urr,vrr)
                  ursm =-(-a22rs*uey*a12-a22s*vr*a12-a22r*vs*a12+a22*
     & a22ss*uey+2*a22*a22s*vs+2*a22*a21s*us+a22*a12rs*uey+a22*a12r*
     & vs+a22*a12s*vr+a22*a11rs*uex+a22*a21ss*uex+a22*a11r*us-a21r*us*
     & a12+a22*a11s*ur-a21s*ur*a12-a12**2*vrr-2*a12r*vr*a12-a21rs*uex*
     & a12+a22*a21*uss+a22**2*vss-a11rr*uex*a12-2*a11r*ur*a12-a11*urr*
     & a12-a12rr*uey*a12)/(a11*a22-a21*a12)
                  vrsm =(a21*a11s*ur+a21*a22*vss-a11*a21r*us-a11*a21s*
     & ur-a11*a21rs*uex-a11*a12*vrr-2*a11*a12r*vr-a11*a12rr*uey-a11*
     & a22r*vs-a11*a22s*vr-a11*a22rs*uey+a21*a11r*us+2*a21*a21s*us+
     & a21*a12rs*uey+a21*a12r*vs-a11*a11rr*uex+a21*a12s*vr+a21*a11rs*
     & uex-a11**2*urr+a21*a21ss*uex-2*a11*a11r*ur+a21**2*uss+a21*
     & a22ss*uey+2*a21*a22s*vs)/(a11*a22-a21*a12)
                 !      urrm=-(c22*uss+c1*ur+c2*us)/c11
                 !
                 !      vrrm=-(c22*vss+c1*vr+c2*vs)/c11
                 !
                  write(*,'("  bc4: dra,dsa=",2e10.2)') dra,dsa
                  write(*,'("  bc4: ursm,urs=",2e10.2," error=",e9.2)')
     &  ursm,urs,ursm-urs
                  write(*,'("  bc4: vrsm,vrs=",2e10.2," error=",e9.2," 
     & vrs2=",e10.2)') vrsm,vrs,vrsm-vrs,((u(i1+1,i2+1,i3,ey)-u(i1-1,
     & i2+1,i3,ey))-(u(i1+1,i2-1,i3,ey)-u(i1-1,i2-1,i3,ey)))/(4.*dra*
     & dsa)
                  a1Dotur=-( a11r*uex +a12r*uey + a21*us + a22*vs + 
     & a21s*uex + a22s*uey )
                  write(*,'("  bc4: a1.ur= -( a1r.u + a2.us + a2s.u) 
     & =",e10.2," a1.ur=",e10.2," error=",e10.2)') a1Dotur,(a11*ur+
     & a12*vr),a1Dotur-(a11*ur+a12*vr)
                  a1Dotur=-( a11r*us +a12r*vs + a21*uss + a22*vss + 
     & a21s*us + a22s*vs  +a11rs*uex+a12rs*uey + a21s*us + a22s*vs + 
     & a21ss*uex + a22ss*uey )
                  write(*,'("  bc4: a1.urs= -( a1r.u...)_s =",e10.2," 
     & a1.urs=",e10.2," error=",e10.2)') a1Dotur,(a11*urs+a12*vrs),
     & a1Dotur-(a11*urs+a12*vrs)
                  write(*,'("  bc4: a1.ursm=",e10.2," error=",e10.2)') 
     & a11*ursm+a12*vrsm,a11*ursm+a12*vrsm-(a11*urs+a12*vrs)
                  write(*,'("  bc4: (a1.1).r + (a2.1).s = ",e10.2)') 
     & a11r+a12r+a21s+a22s
                  write(*,'("  bc4: (a1.1).rr + (a2.1).rs = ",e10.2)') 
     & a11rr+a12rr+a21rs+a22rs
                  write(*,'("  bc4: (a1.1).rs + (a2.1).ss = ",e10.2)') 
     & a11rs+a12rs+a21ss+a22ss
                  write(*,'("  bc4: (a1.1).rss + (a2.1).sss = ",e10.2)
     & ') a11rss+a12rss+a21sss+a22sss
                  write(*,'("  bc4: a11,a12,a21,a22,a11r,a12r,a21r,
     & a22r=",8e11.3)') a11,a12,a21,a22
                  write(*,'("  bc4: a11r,a12r,a21r,a22r=",8e11.3)') 
     & a11r,a12r,a21r,a22r
                  write(*,'("  bc4: a11s,a12s,a21s,a22s=",8e11.3)') 
     & a11s,a12s,a21s,a22s
                  write(*,'("  bc4: a11rs,a11ss,a12rs,a12ss=",4e11.3)')
     &  a11rs,a11ss,a12rs,a12ss
                  write(*,'("  bc4: a21rs,a21ss,a22rs,a22ss=",4e11.3)')
     &  a21rs,a21ss,a22rs,a22ss
                  write(*,'("  bc4:a11r,a11r2=",2e11.3," a12r,a12r2=",
     & 2e11.3)') a11r,((rsxy(i1+is1,i2+is2,i3+is3,axis,0)/(rx(i1+is1,
     & i2+is2,i3+is3)*sy(i1+is1,i2+is2,i3+is3)-ry(i1+is1,i2+is2,i3+
     & is3)*sx(i1+is1,i2+is2,i3+is3)))-(rsxy(i1-is1,i2-is2,i3-is3,
     & axis,0)/(rx(i1-is1,i2-is2,i3-is3)*sy(i1-is1,i2-is2,i3-is3)-ry(
     & i1-is1,i2-is2,i3-is3)*sx(i1-is1,i2-is2,i3-is3))))/(2.*dra),
     & a12r,((rsxy(i1+is1,i2+is2,i3+is3,axis,1)/(rx(i1+is1,i2+is2,i3+
     & is3)*sy(i1+is1,i2+is2,i3+is3)-ry(i1+is1,i2+is2,i3+is3)*sx(i1+
     & is1,i2+is2,i3+is3)))-(rsxy(i1-is1,i2-is2,i3-is3,axis,1)/(rx(i1-
     & is1,i2-is2,i3-is3)*sy(i1-is1,i2-is2,i3-is3)-ry(i1-is1,i2-is2,
     & i3-is3)*sx(i1-is1,i2-is2,i3-is3))))/(2.*dra)
                  write(*,'("  bc4:a21r,a21r2=",2e11.3," a22r,a22r2=",
     & 2e11.3)') a21r,((rsxy(i1+is1,i2+is2,i3+is3,axisp1,0)/(rx(i1+
     & is1,i2+is2,i3+is3)*sy(i1+is1,i2+is2,i3+is3)-ry(i1+is1,i2+is2,
     & i3+is3)*sx(i1+is1,i2+is2,i3+is3)))-(rsxy(i1-is1,i2-is2,i3-is3,
     & axisp1,0)/(rx(i1-is1,i2-is2,i3-is3)*sy(i1-is1,i2-is2,i3-is3)-
     & ry(i1-is1,i2-is2,i3-is3)*sx(i1-is1,i2-is2,i3-is3))))/(2.*dra),
     & a22r,((rsxy(i1+is1,i2+is2,i3+is3,axisp1,1)/(rx(i1+is1,i2+is2,
     & i3+is3)*sy(i1+is1,i2+is2,i3+is3)-ry(i1+is1,i2+is2,i3+is3)*sx(
     & i1+is1,i2+is2,i3+is3)))-(rsxy(i1-is1,i2-is2,i3-is3,axisp1,1)/(
     & rx(i1-is1,i2-is2,i3-is3)*sy(i1-is1,i2-is2,i3-is3)-ry(i1-is1,i2-
     & is2,i3-is3)*sx(i1-is1,i2-is2,i3-is3))))/(2.*dra)
                  write(*,'("  bc4:a11s,a11s2=",2e11.3," a12s,a12s2=",
     & 2e11.3)') a11s,((rsxy(i1+js1,i2+js2,i3+js3,axis,0)/(rx(i1+js1,
     & i2+js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+js2,i3+
     & js3)*sx(i1+js1,i2+js2,i3+js3)))-(rsxy(i1-js1,i2-js2,i3-js3,
     & axis,0)/(rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-ry(
     & i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3))))/(2.*dsa),
     & a12s,((rsxy(i1+js1,i2+js2,i3+js3,axis,1)/(rx(i1+js1,i2+js2,i3+
     & js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+js2,i3+js3)*sx(i1+
     & js1,i2+js2,i3+js3)))-(rsxy(i1-js1,i2-js2,i3-js3,axis,1)/(rx(i1-
     & js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-ry(i1-js1,i2-js2,
     & i3-js3)*sx(i1-js1,i2-js2,i3-js3))))/(2.*dsa)
                  write(*,'("  bc4:a21s,a21s2=",2e11.3," a22s,a22s2=",
     & 2e11.3)') a21s,((rsxy(i1+js1,i2+js2,i3+js3,axisp1,0)/(rx(i1+
     & js1,i2+js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+js2,
     & i3+js3)*sx(i1+js1,i2+js2,i3+js3)))-(rsxy(i1-js1,i2-js2,i3-js3,
     & axisp1,0)/(rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-
     & ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3))))/(2.*dsa),
     & a22s,((rsxy(i1+js1,i2+js2,i3+js3,axisp1,1)/(rx(i1+js1,i2+js2,
     & i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+js2,i3+js3)*sx(
     & i1+js1,i2+js2,i3+js3)))-(rsxy(i1-js1,i2-js2,i3-js3,axisp1,1)/(
     & rx(i1-js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-ry(i1-js1,i2-
     & js2,i3-js3)*sx(i1-js1,i2-js2,i3-js3))))/(2.*dsa)
                  write(*,'("  bc4:a11rs,a11rs2=",2e11.3," a12rs,
     & a12rs2=",2e11.3)') a11rs,(((rsxy(i1+1,i2+1,i3,axis,0)/(rx(i1+1,
     & i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+1,i3)))-(
     & rsxy(i1-1,i2+1,i3,axis,0)/(rx(i1-1,i2+1,i3)*sy(i1-1,i2+1,i3)-
     & ry(i1-1,i2+1,i3)*sx(i1-1,i2+1,i3))))-((rsxy(i1+1,i2-1,i3,axis,
     & 0)/(rx(i1+1,i2-1,i3)*sy(i1+1,i2-1,i3)-ry(i1+1,i2-1,i3)*sx(i1+1,
     & i2-1,i3)))-(rsxy(i1-1,i2-1,i3,axis,0)/(rx(i1-1,i2-1,i3)*sy(i1-
     & 1,i2-1,i3)-ry(i1-1,i2-1,i3)*sx(i1-1,i2-1,i3)))))/(4.*dra*dsa),
     & a12rs,(((rsxy(i1+1,i2+1,i3,axis,1)/(rx(i1+1,i2+1,i3)*sy(i1+1,
     & i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+1,i3)))-(rsxy(i1-1,i2+1,
     & i3,axis,1)/(rx(i1-1,i2+1,i3)*sy(i1-1,i2+1,i3)-ry(i1-1,i2+1,i3)*
     & sx(i1-1,i2+1,i3))))-((rsxy(i1+1,i2-1,i3,axis,1)/(rx(i1+1,i2-1,
     & i3)*sy(i1+1,i2-1,i3)-ry(i1+1,i2-1,i3)*sx(i1+1,i2-1,i3)))-(rsxy(
     & i1-1,i2-1,i3,axis,1)/(rx(i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-ry(i1-
     & 1,i2-1,i3)*sx(i1-1,i2-1,i3)))))/(4.*dra*dsa)
                  write(*,'("  bc4:a21rs,a21rs2=",2e11.3," a22rs,
     & a22rs2=",2e11.3)') a21rs,(((rsxy(i1+1,i2+1,i3,axisp1,0)/(rx(i1+
     & 1,i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+1,i3)))
     & -(rsxy(i1-1,i2+1,i3,axisp1,0)/(rx(i1-1,i2+1,i3)*sy(i1-1,i2+1,
     & i3)-ry(i1-1,i2+1,i3)*sx(i1-1,i2+1,i3))))-((rsxy(i1+1,i2-1,i3,
     & axisp1,0)/(rx(i1+1,i2-1,i3)*sy(i1+1,i2-1,i3)-ry(i1+1,i2-1,i3)*
     & sx(i1+1,i2-1,i3)))-(rsxy(i1-1,i2-1,i3,axisp1,0)/(rx(i1-1,i2-1,
     & i3)*sy(i1-1,i2-1,i3)-ry(i1-1,i2-1,i3)*sx(i1-1,i2-1,i3)))))/(4.*
     & dra*dsa),a22rs,(((rsxy(i1+1,i2+1,i3,axisp1,1)/(rx(i1+1,i2+1,i3)
     & *sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+1,i3)))-(rsxy(i1-
     & 1,i2+1,i3,axisp1,1)/(rx(i1-1,i2+1,i3)*sy(i1-1,i2+1,i3)-ry(i1-1,
     & i2+1,i3)*sx(i1-1,i2+1,i3))))-((rsxy(i1+1,i2-1,i3,axisp1,1)/(rx(
     & i1+1,i2-1,i3)*sy(i1+1,i2-1,i3)-ry(i1+1,i2-1,i3)*sx(i1+1,i2-1,
     & i3)))-(rsxy(i1-1,i2-1,i3,axisp1,1)/(rx(i1-1,i2-1,i3)*sy(i1-1,
     & i2-1,i3)-ry(i1-1,i2-1,i3)*sx(i1-1,i2-1,i3)))))/(4.*dra*dsa)
                  write(*,'("  bc4:a11rr,a11rr2=",2e11.3," a12rr,
     & a12rr2=",2e11.3)') a11rr,((rsxy(i1+is1,i2+is2,i3+is3,axis,0)/(
     & rx(i1+is1,i2+is2,i3+is3)*sy(i1+is1,i2+is2,i3+is3)-ry(i1+is1,i2+
     & is2,i3+is3)*sx(i1+is1,i2+is2,i3+is3)))-2.*(rsxy(i1,i2,i3,axis,
     & 0)/(rx(i1,i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+(
     & rsxy(i1-is1,i2-is2,i3-is3,axis,0)/(rx(i1-is1,i2-is2,i3-is3)*sy(
     & i1-is1,i2-is2,i3-is3)-ry(i1-is1,i2-is2,i3-is3)*sx(i1-is1,i2-
     & is2,i3-is3))))/(dra**2),a12rr,((rsxy(i1+is1,i2+is2,i3+is3,axis,
     & 1)/(rx(i1+is1,i2+is2,i3+is3)*sy(i1+is1,i2+is2,i3+is3)-ry(i1+
     & is1,i2+is2,i3+is3)*sx(i1+is1,i2+is2,i3+is3)))-2.*(rsxy(i1,i2,
     & i3,axis,1)/(rx(i1,i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)
     & ))+(rsxy(i1-is1,i2-is2,i3-is3,axis,1)/(rx(i1-is1,i2-is2,i3-is3)
     & *sy(i1-is1,i2-is2,i3-is3)-ry(i1-is1,i2-is2,i3-is3)*sx(i1-is1,
     & i2-is2,i3-is3))))/(dra**2)
                  write(*,'("  bc4:a21rr,a21rr2=",2e11.3," a22rr,
     & a22rr2=",2e11.3)') a21rr,((rsxy(i1+is1,i2+is2,i3+is3,axisp1,0)
     & /(rx(i1+is1,i2+is2,i3+is3)*sy(i1+is1,i2+is2,i3+is3)-ry(i1+is1,
     & i2+is2,i3+is3)*sx(i1+is1,i2+is2,i3+is3)))-2.*(rsxy(i1,i2,i3,
     & axisp1,0)/(rx(i1,i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3))
     & )+(rsxy(i1-is1,i2-is2,i3-is3,axisp1,0)/(rx(i1-is1,i2-is2,i3-
     & is3)*sy(i1-is1,i2-is2,i3-is3)-ry(i1-is1,i2-is2,i3-is3)*sx(i1-
     & is1,i2-is2,i3-is3))))/(dra**2),a22rr,((rsxy(i1+is1,i2+is2,i3+
     & is3,axisp1,1)/(rx(i1+is1,i2+is2,i3+is3)*sy(i1+is1,i2+is2,i3+
     & is3)-ry(i1+is1,i2+is2,i3+is3)*sx(i1+is1,i2+is2,i3+is3)))-2.*(
     & rsxy(i1,i2,i3,axisp1,1)/(rx(i1,i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)
     & *sx(i1,i2,i3)))+(rsxy(i1-is1,i2-is2,i3-is3,axisp1,1)/(rx(i1-
     & is1,i2-is2,i3-is3)*sy(i1-is1,i2-is2,i3-is3)-ry(i1-is1,i2-is2,
     & i3-is3)*sx(i1-is1,i2-is2,i3-is3))))/(dra**2)
                  write(*,'("  bc4:a11ss,a11ss2=",2e11.3," a12ss,
     & a12ss2=",2e11.3)') a11ss,((rsxy(i1+js1,i2+js2,i3+js3,axis,0)/(
     & rx(i1+js1,i2+js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,i2+
     & js2,i3+js3)*sx(i1+js1,i2+js2,i3+js3)))-2.*(rsxy(i1,i2,i3,axis,
     & 0)/(rx(i1,i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+(
     & rsxy(i1-js1,i2-js2,i3-js3,axis,0)/(rx(i1-js1,i2-js2,i3-js3)*sy(
     & i1-js1,i2-js2,i3-js3)-ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,i2-
     & js2,i3-js3))))/(dsa**2),a12ss,((rsxy(i1+js1,i2+js2,i3+js3,axis,
     & 1)/(rx(i1+js1,i2+js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+
     & js1,i2+js2,i3+js3)*sx(i1+js1,i2+js2,i3+js3)))-2.*(rsxy(i1,i2,
     & i3,axis,1)/(rx(i1,i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)
     & ))+(rsxy(i1-js1,i2-js2,i3-js3,axis,1)/(rx(i1-js1,i2-js2,i3-js3)
     & *sy(i1-js1,i2-js2,i3-js3)-ry(i1-js1,i2-js2,i3-js3)*sx(i1-js1,
     & i2-js2,i3-js3))))/(dsa**2)
                  write(*,'("  bc4:a21ss,a21ss2=",2e11.3," a22ss,
     & a22ss2=",2e11.3)') a21ss,((rsxy(i1+js1,i2+js2,i3+js3,axisp1,0)
     & /(rx(i1+js1,i2+js2,i3+js3)*sy(i1+js1,i2+js2,i3+js3)-ry(i1+js1,
     & i2+js2,i3+js3)*sx(i1+js1,i2+js2,i3+js3)))-2.*(rsxy(i1,i2,i3,
     & axisp1,0)/(rx(i1,i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3))
     & )+(rsxy(i1-js1,i2-js2,i3-js3,axisp1,0)/(rx(i1-js1,i2-js2,i3-
     & js3)*sy(i1-js1,i2-js2,i3-js3)-ry(i1-js1,i2-js2,i3-js3)*sx(i1-
     & js1,i2-js2,i3-js3))))/(dsa**2),a22ss,((rsxy(i1+js1,i2+js2,i3+
     & js3,axisp1,1)/(rx(i1+js1,i2+js2,i3+js3)*sy(i1+js1,i2+js2,i3+
     & js3)-ry(i1+js1,i2+js2,i3+js3)*sx(i1+js1,i2+js2,i3+js3)))-2.*(
     & rsxy(i1,i2,i3,axisp1,1)/(rx(i1,i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)
     & *sx(i1,i2,i3)))+(rsxy(i1-js1,i2-js2,i3-js3,axisp1,1)/(rx(i1-
     & js1,i2-js2,i3-js3)*sy(i1-js1,i2-js2,i3-js3)-ry(i1-js1,i2-js2,
     & i3-js3)*sx(i1-js1,i2-js2,i3-js3))))/(dsa**2)
                  if( axis.eq.1 )then
                    write(*,'("  bc4: a11rss,a12rss=",2e11.3," 2nd-
     & order=",2e11.3)') a11rss,a12rss,(((rsxy(i1+1,i2+1,i3,axis,0)/(
     & rx(i1+1,i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+
     & 1,i3)))-2.*(rsxy(i1,i2+1,i3,axis,0)/(rx(i1,i2+1,i3)*sy(i1,i2+1,
     & i3)-ry(i1,i2+1,i3)*sx(i1,i2+1,i3)))+(rsxy(i1-1,i2+1,i3,axis,0)
     & /(rx(i1-1,i2+1,i3)*sy(i1-1,i2+1,i3)-ry(i1-1,i2+1,i3)*sx(i1-1,
     & i2+1,i3))))-((rsxy(i1+1,i2-1,i3,axis,0)/(rx(i1+1,i2-1,i3)*sy(
     & i1+1,i2-1,i3)-ry(i1+1,i2-1,i3)*sx(i1+1,i2-1,i3)))-2.*(rsxy(i1,
     & i2-1,i3,axis,0)/(rx(i1,i2-1,i3)*sy(i1,i2-1,i3)-ry(i1,i2-1,i3)*
     & sx(i1,i2-1,i3)))+(rsxy(i1-1,i2-1,i3,axis,0)/(rx(i1-1,i2-1,i3)*
     & sy(i1-1,i2-1,i3)-ry(i1-1,i2-1,i3)*sx(i1-1,i2-1,i3)))))/(2.*dr(
     & 1)*dr(0)**2),(((rsxy(i1+1,i2+1,i3,axis,1)/(rx(i1+1,i2+1,i3)*sy(
     & i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+1,i3)))-2.*(rsxy(i1,
     & i2+1,i3,axis,1)/(rx(i1,i2+1,i3)*sy(i1,i2+1,i3)-ry(i1,i2+1,i3)*
     & sx(i1,i2+1,i3)))+(rsxy(i1-1,i2+1,i3,axis,1)/(rx(i1-1,i2+1,i3)*
     & sy(i1-1,i2+1,i3)-ry(i1-1,i2+1,i3)*sx(i1-1,i2+1,i3))))-((rsxy(
     & i1+1,i2-1,i3,axis,1)/(rx(i1+1,i2-1,i3)*sy(i1+1,i2-1,i3)-ry(i1+
     & 1,i2-1,i3)*sx(i1+1,i2-1,i3)))-2.*(rsxy(i1,i2-1,i3,axis,1)/(rx(
     & i1,i2-1,i3)*sy(i1,i2-1,i3)-ry(i1,i2-1,i3)*sx(i1,i2-1,i3)))+(
     & rsxy(i1-1,i2-1,i3,axis,1)/(rx(i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-
     & ry(i1-1,i2-1,i3)*sx(i1-1,i2-1,i3)))))/(2.*dr(1)*dr(0)**2)
                  end if
                 !
                  a1Doturss =-a11r*uss-a11rss*uex-2*a11rs*us-a11ss*ur-
     & 2*a11s*urs-a22*vsss-a12rss*uex-2*a12rs*vs-a12r*vss-2*a12s*vrs-
     & a12ss*vr-a21sss*uex-3*a21ss*us-3*a21s*uss-a21*usss-3*a22ss*vs-
     & a22sss*uex-3*a22s*vss
                 !
                   write(*,'("  bc4:  a1.urss=-a11r*uss-...",e10.2,", 
     & a1.urs=",e10.2," error=",e10.2)') a1Doturss,(a11*urss+a12*vrss)
     & ,a1Doturss-(a11*urss+a12*vrss)
                 !
                 !
                 ! write(*,'("  bc4: urrm,urr=",2e10.2," error=",e9.2)') urrm,URR4(ex),urrm-URR4(ex)
                 ! write(*,'("  bc4: vrrm,vrr=",2e10.2," error=",e9.2)') vrrm,URR4(ey),vrrm-URR4(ey)
                 !
                  ! write(*,'("  bc4: a21rrs,a22rrs,a21rr,a22rr,a21rs=",12f7.2)') !                    a21rrs,a22rrs,a21rr,a22rr,a21rs
                  ! write(*,'("  bc4: a22rs,a21r,a22r,a21s,a22s,a21,a22=",12f7.2)') !                    a22rs,a21r,a22r,a21s,a22s,a21,a22
                  ! write(*,'("  bc4: uex,uey,us,vs,ur,vr,urs,vrs=",12f7.2)')!                    uex,uey,us,vs,ur,vr,urs,vrs 
                  ! write(*,'("  bc4: urr,vrr,urrs,vrrs, vrrs2=",12f7.2)')!                    urr,vrr,urrs,vrrs,(urr2(i1,i2+1,i3,ey)-urr2(i1,i2-1,i3,ey))/(2.*dsa)
                  ! ***** Check Hz *******
                   call ogf2d(ep,xy(i1,i2,i3,0),xy(i1,i2,i3,1),t,uv0(0)
     & ,uv0(1),uv0(2))
                   call ogf2d(ep,xy(i1-is1,i2-is2,i3,0),xy(i1-is1,i2-
     & is2,i3,1),t,uvm(0),uvm(1),uvm(2))
                   call ogf2d(ep,xy(i1+is1,i2+is2,i3,0),xy(i1+is1,i2+
     & is2,i3,1),t,uvp(0),uvp(1),uvp(2))
                   call ogf2d(ep,xy(i1-2*is1,i2-2*is2,i3,0),xy(i1-2*
     & is1,i2-2*is2,i3,1),t,uvm2(0),uvm2(1),uvm2(2))
                   call ogf2d(ep,xy(i1+2*is1,i2+2*is2,i3,0),xy(i1+2*
     & is1,i2+2*is2,i3,1),t,uvp2(0),uvp2(1),uvp2(2))
                  fw1= (8.*(uvp(2)-uvm(2))-(uvp2(2)-uvm2(2)))/(12.*dra)
                  wrrr=(uvp2(2)-2.*(uvp(2)-uvm(2))-uvm2(2))/(2.*dra**3)
                  ! wrr=(uvp(2)-2.*uv0(2)+uvm(2))/(dra**2)
                  wrr=(-30.*uv0(2)+16.*(uvp(2)+uvm(2))-(uvp2(2)+uvm2(2)
     & ) )/(12.*dra**2)
                  ! wr=(uvp(2)-uvm(2))/(2.*dra)
                  wr=(8.*(uvp(2)-uvm(2))-(uvp2(2)-uvm2(2)))/(12.*dra)
                  fw2= c11*wrrr + (c1+c11r)*wrr + c1r*wr
                  ! for tangential derivatives:
                   call ogf2d(ep,xy(i1-js1,i2-js2,i3,0),xy(i1-js1,i2-
     & js2,i3,1),t,uvm(0),uvm(1),uvm(2))
                   call ogf2d(ep,xy(i1+js1,i2+js2,i3,0),xy(i1+js1,i2+
     & js2,i3,1),t,uvp(0),uvp(1),uvp(2))
                   call ogf2d(ep,xy(i1-2*js1,i2-2*js2,i3,0),xy(i1-2*
     & js1,i2-2*js2,i3,1),t,uvm2(0),uvm2(1),uvm2(2))
                   call ogf2d(ep,xy(i1+2*js1,i2+2*js2,i3,0),xy(i1+2*
     & js1,i2+2*js2,i3,1),t,uvp2(0),uvp2(1),uvp2(2))
                  ! These approximations should be consistent with the approximations for ws and wss above
                  ! fw2=fw2 + c22r*(uvp(2)-2.*uv0(2)+uvm(2))/(dsa**2)+c2r*(uvp(2)-uvm(2))/(2.*dsa)
                  fw2=fw2 + c22r*(-30.*uv0(2)+16.*(uvp(2)+uvm(2))-(
     & uvp2(2)+uvm2(2)) )/(12.*dsa**2)+ c2r*(8.*(uvp(2)-uvm(2))-(uvp2(
     & 2)-uvm2(2)))/(12.*dsa)
                  write(*,'("  bc4: Hz: error in wr-fw1=",e10.2)') (8.*
     & (u(i1+is1,i2+is2,i3+is3,hz)-u(i1-is1,i2-is2,i3-is3,hz))-(u(i1+
     & 2*is1,i2+2*is2,i3+2*is3,hz)-u(i1-2*is1,i2-2*is2,i3-2*is3,hz)))
     & /(12.*dra)-fw1
                 ! write(*,'("  bc4: Hz: error in (Lw).r-fw2=",e10.2)') !                c11*URRR2(hz)+(c1+c11r)*URR2(hz)+c1r*UR2(hz)+c22r*USS2(hz)+c2r*US2(hz) - fw2
                  write(*,'("  bc4: Hz: error in (Lw).r-fw2=",e10.2)') 
     & c11*(u(i1+2*is1,i2+2*is2,i3+2*is3,hz)-2.*u(i1+is1,i2+is2,i3+
     & is3,hz)+2.*u(i1-is1,i2-is2,i3-is3,hz)-u(i1-2*is1,i2-2*is2,i3-2*
     & is3,hz))/(2.*dra**3)+(c1+c11r)*(-30.*u(i1,i2,i3,hz)+16.*(u(i1+
     & is1,i2+is2,i3+is3,hz)+u(i1-is1,i2-is2,i3-is3,hz))-(u(i1+2*is1,
     & i2+2*is2,i3+2*is3,hz)+u(i1-2*is1,i2-2*is2,i3-2*is3,hz)))/(12.*
     & dra**2)+c1r*(8.*(u(i1+is1,i2+is2,i3+is3,hz)-u(i1-is1,i2-is2,i3-
     & is3,hz))-(u(i1+2*is1,i2+2*is2,i3+2*is3,hz)-u(i1-2*is1,i2-2*is2,
     & i3-2*is3,hz)))/(12.*dra)+c22r*(-30.*u(i1,i2,i3,hz)+16.*(u(i1+
     & js1,i2+js2,i3+js3,hz)+u(i1-js1,i2-js2,i3-js3,hz))-(u(i1+2*js1,
     & i2+2*js2,i3+2*js3,hz)+u(i1-2*js1,i2-2*js2,i3-2*js3,hz)))/(12.*
     & dsa**2)+c2r*(8.*(u(i1+js1,i2+js2,i3+js3,hz)-u(i1-js1,i2-js2,i3-
     & js3,hz))-(u(i1+2*js1,i2+2*js2,i3+2*js3,hz)-u(i1-2*js1,i2-2*js2,
     & i3-2*js3,hz)))/(12.*dsa) - fw2
                  maxDivc=max(maxDivc,divc)
                  maxTauDotLapu=max(maxTauDotLapu,tauDotLap)
                  maxExtrap=max(maxExtrap,tauUp1)
                  maxDr3aDotU=max(maxDr3aDotU,g2a)
                 end if ! mask>0
                 end do
                 end do
                 end do
                  write(*,'(" *** side,axis=",2i3," maxDivc=",e8.1,", 
     & maxTauDotLapu=",e8.1,", maxExtrap=",e8.1,", maxDr3aDotU=",e8.1,
     & " ***** ")') side,axis,maxDivc,maxTauDotLapu,maxExtrap,
     & maxDr3aDotU
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
