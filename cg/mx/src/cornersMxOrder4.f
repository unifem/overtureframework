! This file automatically generated from bcMaxwellCorners.bf with bpp.
        subroutine cornersMxOrder4( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     & ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,gridIndexRange,dimension,u,
     & f,mask,rsxy, xy,bc, boundaryCondition, ipar, rpar, ierr )
       ! ===================================================================================
       !  Optimised Boundary conditions for Maxwell's Equations.
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
        real ep ! holds the pointer to the TZ function
        integer is1,is2,is3,js1,js2,js3,ks1,ks2,ks3,ls1,ls2,ls3,
     & orderOfAccuracy,gridType,debug,grid,side,axis,useForcing,ex,ey,
     & ez,hx,hy,hz,useWhereMask,side1,side2,side3,m1,m2,m3,bc1,bc2,
     & forcingOption,fieldOption
        real dt,kx,ky,kz,eps,mu,c,cc,twoPi,slowStartInterval,ssf,ssft,
     & ssftt,ssfttt,ssftttt,tt
        real dr(0:2), dx(0:2), t, uv(0:5), uvm(0:5), uv0(0:5), uvp(0:5)
     & , uvm2(0:5), uvp2(0:5)
        real uvmm(0:2),uvzm(0:2),uvpm(0:2)
        real uvmz(0:2),uvzz(0:2),uvpz(0:2)
        real uvmp(0:2),uvzp(0:2),uvpp(0:2)
        integer i10,i20,i30
        real jac3di(-2:2,-2:2,-2:2)
        integer orderOfExtrapolation
        logical setCornersToExact
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
        real det,dra,dsa,dta,dxa,dya,dza
        real tau1,tau2,tau11,tau12,tau13, tau21,tau22,tau23
        real tau11s,tau12s,tau13s, tau21s,tau22s,tau23s
        real tau11t,tau12t,tau13t, tau21t,tau22t,tau23t
        real tau1u,tau2u,tau1Up1,tau1Up2,tau1Up3,tau2Up1,tau2Up2,
     & tau2Up3
        real tau1Dotu,tau2Dotu,tauU,tauUp1,tauUp2,tauUp3,ttu1,ttu2
        real ttu11,ttu12,ttu13, ttu21,ttu22,ttu23
        real DtTau1DotUvr,DtTau2DotUvr,DsTau1DotUvr,DsTau2DotUvr,
     & tau1DotUtt,tau2DotUtt,Da1DotU,a1DotU
        real drA1DotDeltaU
       ! real tau1DotUvrs, tau2DotUvrs, tau1DotUvrt, tau2DotUvrt
        real gx1,gx2,g1a,g2a
        real g1,g2,g3
        real tauDotExtrap
        real jac,jacm1,jacp1,jacp2,jacm2,detnt
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
        real b3u,b3v,b3w, b2u,b2v,b2w, b1u,b1v,b1w, bf,divtt
        real cw1,cw2,bfw2,fw1,fw2,fw3,fw4
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
        real xm,ym,x0,y0,z0,xp,yp,um,vm,wm,u0,v0,w0,up,vp,wp
        real an(0:2), anNorm, nDotE, nDotE0, epsX
        real tdu10,tdu01,tdu20,tdu02,gLu,gLv,utt00,vtt00,wtt00
        real cu10,cu01,cu20,cu02,cv10,cv01,cv20,cv02
        real maxDivc,maxTauDotLapu,maxExtrap,maxDr3aDotU,dr3aDotU,
     & a1Doturss
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
       ! real uxxx22r,uyyy22r,uxxx42r,uyyy42r,uxxxx22r,uyyyy22r, urrrr2,ussss2
        real urrrr2,ussss2
        real urrs4,urrt4,usst4,urss4,ustt4,urtt4
        real urrs2,urrt2,usst2,urss2,ustt2,urtt2
        real deltaFu,deltaFv,deltaFw,g1f,g2f
        real a1Dotu1,a3Dotu1, a1Dotu2,a3Dotu2, a2Dotu3,a3Dotu3, 
     & a2Dotu4,a3Dotu4
        real a11c,a12c,a13c,a21c,a22c,a23c,a31c,a32c,a33c
        real a1a1,a1a2,a1a3,a2a2,a2a3,a3a3
        real b11,b12,b13, g11,g12,g13
        real b21,b22,b23, g21,g22,g23
        real b31,b32,b33, g31,g32,g33
        real b41,b42,b43, g41,g42,g43
        real cc11a,cc12a,cc13a,cc14a,cc15a,cc16a,cc11b,cc12b,cc13b,
     & cc14b,cc15b,cc16b
        real cc21a,cc22a,cc23a,cc24a,cc25a,cc26a,cc21b,cc22b,cc23b,
     & cc24b,cc25b,cc26b
        real dd11,dd12,dd13,dd14,dd21,dd22,dd23,dd24,dd31,dd32,dd33,
     & dd34,dd41,dd42,dd43,dd44
        real f1x,f2x,f3x,f4x
        real deltaU,deltaV,deltaW
        real a1DotLu,a2DotLu
        real f1,f2,f3,f4, x1,x2,x3,x4
        integer edgeDirection,sidea,sideb,ms1,ms2,ms3,ns1,ns2,ns3
        real a1Dotu0,a2Dotu0,a1Doturr,a1Dotuss,a2Doturr,a2Dotuss,
     & a3Doturrr,a3Dotusss,a3Doturss,a3Doturrs
        real a1Doturs,a2Doturs,a3Doturs, a2Dotu, a3Dotu, a3Dotur, 
     & a3Dotus
        real uLapr,vLapr,wLapr,uLaps,vLaps,wLaps
        real drb,dsb,dtb
        real ur0,us0,urr0,uss0,  urs0,vrs0,wrs0,urrs0,vrrs0,wrrs0,
     & urss0,vrss0,wrss0
       !     --- start statement function ----
        integer kd,m,n
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

       ! rsxyr2(i1,i2,i3,m,n)=(rsxy(i1+1,i2,i3,m,n)-rsxy(i1-1,i2,i3,m,n))*d12(0)
       ! rsxys2(i1,i2,i3,m,n)=(rsxy(i1,i2+1,i3,m,n)-rsxy(i1,i2-1,i3,m,n))*d12(1)
       !
       ! rsxyx22(i1,i2,i3,m,n)= rx(i1,i2,i3)*rsxyr2(i1,i2,i3,m,n)+sx(i1,i2,i3)*rsxys2(i1,i2,i3,m,n)
       ! rsxyy22(i1,i2,i3,m,n)= ry(i1,i2,i3)*rsxyr2(i1,i2,i3,m,n)+sy(i1,i2,i3)*rsxys2(i1,i2,i3,m,n)
       !
       ! rsxyr4(i1,i2,i3,m,n)=(8.*(rsxy(i1+1,i2,i3,m,n)-rsxy(i1-1,i2,i3,m,n))!                         -(rsxy(i1+2,i2,i3,m,n)-rsxy(i1-2,i2,i3,m,n)))*d14(0)
       ! rsxys4(i1,i2,i3,m,n)=(8.*(rsxy(i1,i2+1,i3,m,n)-rsxy(i1,i2-1,i3,m,n))!                         -(rsxy(i1,i2+2,i3,m,n)-rsxy(i1,i2-2,i3,m,n)))*d14(1)
       ! rsxyt4(i1,i2,i3,m,n)=(8.*(rsxy(i1,i2,i3+1,m,n)-rsxy(i1,i2,i3-1,m,n))!                         -(rsxy(i1,i2,i3+2,m,n)-rsxy(i1,i2,i3-2,m,n)))*d14(2)
       !
       ! rsxyrr4(i1,i2,i3,m,n)=(-30.*rsxy(i1,i2,i3,m,n)+16.*(rsxy(i1+1,i2,i3,m,n)+rsxy(i1-1,i2,i3,m,n))!                           -(rsxy(i1+2,i2,i3,m,n)+rsxy(i1-2,i2,i3,m,n)) )*d24(0)
       !
       ! rsxyss4(i1,i2,i3,m,n)=(-30.*rsxy(i1,i2,i3,m,n)+16.*(rsxy(i1,i2+1,i3,m,n)+rsxy(i1,i2-1,i3,m,n))!                           -(rsxy(i1,i2+2,i3,m,n)+rsxy(i1,i2-2,i3,m,n)) )*d24(1)
       !
       ! rsxytt4(i1,i2,i3,m,n)=(-30.*rsxy(i1,i2,i3,m,n)+16.*(rsxy(i1,i2,i3+1,m,n)+rsxy(i1,i2,i3-1,m,n))!                           -(rsxy(i1,i2,i3+2,m,n)+rsxy(i1,i2,i3-2,m,n)) )*d24(2)
       !
       ! rsxyrs4(i1,i2,i3,m,n)=(8.*(rsxyr4(i1,i2+1,i3,m,n)-rsxyr4(i1,i2-1,i3,m,n))!                          -(rsxyr4(i1,i2+2,i3,m,n)-rsxyr4(i1,i2-2,i3,m,n)))*d14(1)
       !
       ! rsxyrt4(i1,i2,i3,m,n)=(8.*(rsxyr4(i1,i2,i3+1,m,n)-rsxyr4(i1,i2,i3-1,m,n))!                          -(rsxyr4(i1,i2,i3+2,m,n)-rsxyr4(i1,i2,i3-2,m,n)))*d14(2)
       !
       ! rsxyst4(i1,i2,i3,m,n)=(8.*(rsxys4(i1,i2,i3+1,m,n)-rsxys4(i1,i2,i3-1,m,n))!                          -(rsxys4(i1,i2,i3+2,m,n)-rsxys4(i1,i2,i3-2,m,n)))*d14(2)
       !
       ! rsxyx42(i1,i2,i3,m,n)= rx(i1,i2,i3)*rsxyr4(i1,i2,i3,m,n)+sx(i1,i2,i3)*rsxys4(i1,i2,i3,m,n)
       ! rsxyy42(i1,i2,i3,m,n)= ry(i1,i2,i3)*rsxyr4(i1,i2,i3,m,n)+sy(i1,i2,i3)*rsxys4(i1,i2,i3,m,n)
       !
       !
       ! ! check these again:
       ! rsxyxr42(i1,i2,i3,m,n)= rsxyr4(i1,i2,i3,0,0)*rsxyr4(i1,i2,i3,m,n) + rx(i1,i2,i3)*rsxyrr4(i1,i2,i3,m,n)!                        +rsxyr4(i1,i2,i3,1,0)*rsxys4(i1,i2,i3,m,n) + sx(i1,i2,i3)*rsxyrs4(i1,i2,i3,m,n)
       ! rsxyxs42(i1,i2,i3,m,n)= rsxys4(i1,i2,i3,0,0)*rsxyr4(i1,i2,i3,m,n) + rx(i1,i2,i3)*rsxyrs4(i1,i2,i3,m,n)!                        +rsxys4(i1,i2,i3,1,0)*rsxys4(i1,i2,i3,m,n) + sx(i1,i2,i3)*rsxyss4(i1,i2,i3,m,n)
       !
       ! rsxyyr42(i1,i2,i3,m,n)= rsxyr4(i1,i2,i3,0,1)*rsxyr4(i1,i2,i3,m,n) + ry(i1,i2,i3)*rsxyrr4(i1,i2,i3,m,n)!                        +rsxyr4(i1,i2,i3,1,1)*rsxys4(i1,i2,i3,m,n) + sy(i1,i2,i3)*rsxyrs4(i1,i2,i3,m,n)
       ! rsxyys42(i1,i2,i3,m,n)= rsxys4(i1,i2,i3,0,1)*rsxyr4(i1,i2,i3,m,n) + ry(i1,i2,i3)*rsxyrs4(i1,i2,i3,m,n)!                        +rsxys4(i1,i2,i3,1,1)*rsxys4(i1,i2,i3,m,n) + sy(i1,i2,i3)*rsxyss4(i1,i2,i3,m,n)
       !
       ! ! 3d versions -- check these again
       ! rsxyx43(i1,i2,i3,m,n)= rx(i1,i2,i3)*rsxyr4(i1,i2,i3,m,n)+sx(i1,i2,i3)*rsxys4(i1,i2,i3,m,n)!                       +tx(i1,i2,i3)*rsxyt4(i1,i2,i3,m,n)
       ! rsxyy43(i1,i2,i3,m,n)= ry(i1,i2,i3)*rsxyr4(i1,i2,i3,m,n)+sy(i1,i2,i3)*rsxys4(i1,i2,i3,m,n)!                       +ty(i1,i2,i3)*rsxyt4(i1,i2,i3,m,n)
       ! rsxyz43(i1,i2,i3,m,n)= rz(i1,i2,i3)*rsxyr4(i1,i2,i3,m,n)+sz(i1,i2,i3)*rsxys4(i1,i2,i3,m,n)!                       +tz(i1,i2,i3)*rsxyt4(i1,i2,i3,m,n)
       !
       ! rsxyxr43(i1,i2,i3,m,n)= rsxyr4(i1,i2,i3,0,0)*rsxyr4(i1,i2,i3,m,n) + rx(i1,i2,i3)*rsxyrr4(i1,i2,i3,m,n)!                        +rsxyr4(i1,i2,i3,1,0)*rsxys4(i1,i2,i3,m,n) + sx(i1,i2,i3)*rsxyrs4(i1,i2,i3,m,n)!                        +rsxyr4(i1,i2,i3,2,0)*rsxyt4(i1,i2,i3,m,n) + tx(i1,i2,i3)*rsxyrt4(i1,i2,i3,m,n)
       !
       ! rsxyxs43(i1,i2,i3,m,n)= rsxys4(i1,i2,i3,0,0)*rsxyr4(i1,i2,i3,m,n) + rx(i1,i2,i3)*rsxyrs4(i1,i2,i3,m,n)!                        +rsxys4(i1,i2,i3,1,0)*rsxys4(i1,i2,i3,m,n) + sx(i1,i2,i3)*rsxyss4(i1,i2,i3,m,n)!                        +rsxys4(i1,i2,i3,2,0)*rsxyt4(i1,i2,i3,m,n) + tx(i1,i2,i3)*rsxyst4(i1,i2,i3,m,n)
       !
       ! rsxyxt43(i1,i2,i3,m,n)= rsxyt4(i1,i2,i3,0,0)*rsxyr4(i1,i2,i3,m,n) + rx(i1,i2,i3)*rsxyrt4(i1,i2,i3,m,n)!                        +rsxyt4(i1,i2,i3,1,0)*rsxys4(i1,i2,i3,m,n) + sx(i1,i2,i3)*rsxyst4(i1,i2,i3,m,n)!                        +rsxyt4(i1,i2,i3,2,0)*rsxyt4(i1,i2,i3,m,n) + tx(i1,i2,i3)*rsxytt4(i1,i2,i3,m,n)
       !
       ! rsxyyr43(i1,i2,i3,m,n)= rsxyr4(i1,i2,i3,0,1)*rsxyr4(i1,i2,i3,m,n) + ry(i1,i2,i3)*rsxyrr4(i1,i2,i3,m,n)!                        +rsxyr4(i1,i2,i3,1,1)*rsxys4(i1,i2,i3,m,n) + sy(i1,i2,i3)*rsxyrs4(i1,i2,i3,m,n)!                        +rsxyr4(i1,i2,i3,2,1)*rsxyt4(i1,i2,i3,m,n) + ty(i1,i2,i3)*rsxyrt4(i1,i2,i3,m,n)
       !
       ! rsxyys43(i1,i2,i3,m,n)= rsxys4(i1,i2,i3,0,1)*rsxyr4(i1,i2,i3,m,n) + ry(i1,i2,i3)*rsxyrs4(i1,i2,i3,m,n)!                        +rsxys4(i1,i2,i3,1,1)*rsxys4(i1,i2,i3,m,n) + sy(i1,i2,i3)*rsxyss4(i1,i2,i3,m,n)!                        +rsxys4(i1,i2,i3,2,1)*rsxyt4(i1,i2,i3,m,n) + ty(i1,i2,i3)*rsxyst4(i1,i2,i3,m,n)
       !
       ! rsxyyt43(i1,i2,i3,m,n)= rsxyt4(i1,i2,i3,0,1)*rsxyr4(i1,i2,i3,m,n) + ry(i1,i2,i3)*rsxyrt4(i1,i2,i3,m,n)!                        +rsxyt4(i1,i2,i3,1,1)*rsxys4(i1,i2,i3,m,n) + sy(i1,i2,i3)*rsxyst4(i1,i2,i3,m,n)!                        +rsxyt4(i1,i2,i3,2,1)*rsxyt4(i1,i2,i3,m,n) + ty(i1,i2,i3)*rsxytt4(i1,i2,i3,m,n)
       !
       ! rsxyzr43(i1,i2,i3,m,n)= rsxyr4(i1,i2,i3,0,2)*rsxyr4(i1,i2,i3,m,n) + rz(i1,i2,i3)*rsxyrr4(i1,i2,i3,m,n)!                        +rsxyr4(i1,i2,i3,1,2)*rsxys4(i1,i2,i3,m,n) + sz(i1,i2,i3)*rsxyrs4(i1,i2,i3,m,n)!                        +rsxyr4(i1,i2,i3,2,2)*rsxyt4(i1,i2,i3,m,n) + tz(i1,i2,i3)*rsxyrt4(i1,i2,i3,m,n)
       !
       ! rsxyzs43(i1,i2,i3,m,n)= rsxys4(i1,i2,i3,0,2)*rsxyr4(i1,i2,i3,m,n) + rz(i1,i2,i3)*rsxyrs4(i1,i2,i3,m,n)!                        +rsxys4(i1,i2,i3,1,2)*rsxys4(i1,i2,i3,m,n) + sz(i1,i2,i3)*rsxyss4(i1,i2,i3,m,n)!                        +rsxys4(i1,i2,i3,2,2)*rsxyt4(i1,i2,i3,m,n) + tz(i1,i2,i3)*rsxyst4(i1,i2,i3,m,n)
       !
       ! rsxyzt43(i1,i2,i3,m,n)= rsxyt4(i1,i2,i3,0,2)*rsxyr4(i1,i2,i3,m,n) + rz(i1,i2,i3)*rsxyrt4(i1,i2,i3,m,n)!                        +rsxyt4(i1,i2,i3,1,2)*rsxys4(i1,i2,i3,m,n) + sz(i1,i2,i3)*rsxyst4(i1,i2,i3,m,n)!                        +rsxyt4(i1,i2,i3,2,2)*rsxyt4(i1,i2,i3,m,n) + tz(i1,i2,i3)*rsxytt4(i1,i2,i3,m,n)
       !
       !$$$ uxxx22r(i1,i2,i3,kd)=(-2.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))+(u(i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
       !$$$ uyyy22r(i1,i2,i3,kd)=(-2.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))+(u(i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
       !$$$
       !$$$ uxxxx22r(i1,i2,i3,kd)=(6.*u(i1,i2,i3,kd)-4.*(u(i1+1,i2,i3,kd)+u(i1-1,i2,i3,kd))!$$$                         +(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)) )/(dx(0)**4)
       !$$$
       !$$$ uyyyy22r(i1,i2,i3,kd)=(6.*u(i1,i2,i3,kd)-4.*(u(i1,i2+1,i3,kd)+u(i1,i2-1,i3,kd))!$$$                         +(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )/(dx(1)**4)
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
        pwc(0)               =rpar(20)  ! coeffs. for plane wave
        pwc(1)               =rpar(21)
        pwc(2)               =rpar(22)
        pwc(3)               =rpar(23)
        pwc(4)               =rpar(24)
        pwc(5)               =rpar(25)
        dxa=dx(0)
        dya=dx(1)
        dza=dx(2)
        if( abs(pwc(0))+abs(pwc(1))+abs(pwc(2)) .eq. 0. )then
          ! sanity check
          stop 12345
        end if
       epsX = 1.e-30  ! epsilon used to avoid division by zero in the normal computation -- should be REAL_MIN*100 ??
       !       We first assign the boundary values for the tangential
       !       components and then assign the corner values      
        twoPi=8.*atan2(1.,1.)
        cc= c*sqrt( kx*kx+ky*ky+kz*kz )
        ! write(*,'(" ***assign corners: forcingOption=",i4," twoPi=",f18.14," cc=",f10.7)') forcingOption,twoPi,cc
        ! initialize parameters used in slow starts (e.g. for plane waves)
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
        numberOfGhostPoints=orderOfAccuracy/2
        extra=orderOfAccuracy/2  ! assign the extended boundary
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
          if( nd.eq.2 )then
            if( forcingOption.eq.planeWaveBoundaryForcing )then
              ! write(*,'(" ***assign corners:planeWaveBoundaryForcing: twoPi=",f18.14," cc=",f10.7)') twoPi,cc
               ! Set the tangential component to zero
               if( gridType.eq.curvilinear )then
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                   tau1=rsxy(i1,i2,i3,axisp1,0)
                   tau2=rsxy(i1,i2,i3,axisp1,1)
                   tau1DotU=(tau1*u(i1,i2,i3,ex)+tau2*u(i1,i2,i3,ey))/(
     & tau1**2+tau2**2)
                     x0=xy(i1,i2,i3,0)
                     y0=xy(i1,i2,i3,1)
                     if( fieldOption.eq.0 )then
                       u0=-(ssf*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*
     & pwc(0))
                       v0=-(ssf*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*
     & pwc(1))
                     else
                       ! we are assigning time derivatives (sosup)
                       u0=-(ssf*(-twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(0)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(0)
     & )
                       v0=-(ssf*(-twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)-
     & cc*(t)))*pwc(1)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*pwc(1)
     & )
                     end if
                     tau1DotU = tau1DotU - ( tau1*u0 + tau2*v0 )/(tau1*
     & *2+tau2**2)
                   u(i1,i2,i3,ex)=u(i1,i2,i3,ex)-tau1DotU*tau1
                   u(i1,i2,i3,ey)=u(i1,i2,i3,ey)-tau1DotU*tau2
                  ! if( .true. )then
                  !   write(*,'(" assignBndry: i=",3i3," u=",2f12.8," u0,v0=",2f12.8," x0,y0,t=",3f8.5," ,ssf,sfft=",5f8.5)')!            i1,i2,i3,u(i1,i2,i3,ex),u(i1,i2,i3,ey),u0,v0,x0,y0,t,ssf,ssft,ssftt
                  !   write(*,'(" assignBndry: tau1,tau2=",2e10.2," err tau.u=",e10.2)') tau1,tau2,tau1*u(i1,i2,i3,ex)+tau2*u(i1,i2,i3,ey) - (tau1*u0 + tau2*v0)
                  !   ! write(*,'(" assignBndry: tau*uv - tau*uv0 = ",e10.2)') tau1*u(i1,i2,i3,ex)+tau2*u(i1,i2,i3,ey) - (tau1*u0 + tau2*v0)
                  ! end if
                 end do
                 end do
                 end do
               else
                 if( axis.eq.0 )then
                   et1=ey
                   et2=ez
                 else if( axis.eq.1 )then
                   et1=ex
                   et2=ez
                 else
                   et1=ex
                   et2=ey
                 end if
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                     x0=xy(i1,i2,i3,0)
                     y0=xy(i1,i2,i3,1)
                     if( fieldOption.eq.0 )then
                       uv(ex)=(ssf*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*
     & pwc(0))
                       uv(ey)=(ssf*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*
     & pwc(1))
                     else
                      ! we are assigning time derivatives (sosup)
                       uv(ex)=(ssf*(-twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(
     & y0)-cc*(t)))*pwc(0)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*
     & pwc(0))
                       uv(ey)=(ssf*(-twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(
     & y0)-cc*(t)))*pwc(1)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)-cc*(t)))*
     & pwc(1))
                     end if
                     u(i1,i2,i3,et1)=uv(et1)
                 end do
                 end do
                 end do
               end if
            else if( useForcing.eq.0 )then
               ! Set the tangential component to zero
               if( gridType.eq.curvilinear )then
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                   tau1=rsxy(i1,i2,i3,axisp1,0)
                   tau2=rsxy(i1,i2,i3,axisp1,1)
                   tau1DotU=(tau1*u(i1,i2,i3,ex)+tau2*u(i1,i2,i3,ey))/(
     & tau1**2+tau2**2)
                   u(i1,i2,i3,ex)=u(i1,i2,i3,ex)-tau1DotU*tau1
                   u(i1,i2,i3,ey)=u(i1,i2,i3,ey)-tau1DotU*tau2
                  ! if( .true. )then
                  !   write(*,'(" assignBndry: i=",3i3," u=",2f12.8," u0,v0=",2f12.8," x0,y0,t=",3f8.5," ,ssf,sfft=",5f8.5)')!            i1,i2,i3,u(i1,i2,i3,ex),u(i1,i2,i3,ey),u0,v0,x0,y0,t,ssf,ssft,ssftt
                  !   write(*,'(" assignBndry: tau1,tau2=",2e10.2," err tau.u=",e10.2)') tau1,tau2,tau1*u(i1,i2,i3,ex)+tau2*u(i1,i2,i3,ey) - (tau1*u0 + tau2*v0)
                  !   ! write(*,'(" assignBndry: tau*uv - tau*uv0 = ",e10.2)') tau1*u(i1,i2,i3,ex)+tau2*u(i1,i2,i3,ey) - (tau1*u0 + tau2*v0)
                  ! end if
                 end do
                 end do
                 end do
               else
                 if( axis.eq.0 )then
                   et1=ey
                   et2=ez
                 else if( axis.eq.1 )then
                   et1=ex
                   et2=ez
                 else
                   et1=ex
                   et2=ey
                 end if
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                     u(i1,i2,i3,et1)=0.
                 end do
                 end do
                 end do
               end if
            else
               ! Set the tangential component to zero
               if( gridType.eq.curvilinear )then
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                   tau1=rsxy(i1,i2,i3,axisp1,0)
                   tau2=rsxy(i1,i2,i3,axisp1,1)
                   tau1DotU=(tau1*u(i1,i2,i3,ex)+tau2*u(i1,i2,i3,ey))/(
     & tau1**2+tau2**2)
                     call ogf2dfo(ep,fieldOption,xy(i1    ,i2    ,i3,0)
     & ,xy(i1    ,i2    ,i3,1),t, u0,v0,w0)
                     tau1DotU = tau1DotU - ( tau1*u0 + tau2*v0 )/(tau1*
     & *2+tau2**2)
                   u(i1,i2,i3,ex)=u(i1,i2,i3,ex)-tau1DotU*tau1
                   u(i1,i2,i3,ey)=u(i1,i2,i3,ey)-tau1DotU*tau2
                  ! if( .true. )then
                  !   write(*,'(" assignBndry: i=",3i3," u=",2f12.8," u0,v0=",2f12.8," x0,y0,t=",3f8.5," ,ssf,sfft=",5f8.5)')!            i1,i2,i3,u(i1,i2,i3,ex),u(i1,i2,i3,ey),u0,v0,x0,y0,t,ssf,ssft,ssftt
                  !   write(*,'(" assignBndry: tau1,tau2=",2e10.2," err tau.u=",e10.2)') tau1,tau2,tau1*u(i1,i2,i3,ex)+tau2*u(i1,i2,i3,ey) - (tau1*u0 + tau2*v0)
                  !   ! write(*,'(" assignBndry: tau*uv - tau*uv0 = ",e10.2)') tau1*u(i1,i2,i3,ex)+tau2*u(i1,i2,i3,ey) - (tau1*u0 + tau2*v0)
                  ! end if
                 end do
                 end do
                 end do
               else
                 if( axis.eq.0 )then
                   et1=ey
                   et2=ez
                 else if( axis.eq.1 )then
                   et1=ex
                   et2=ez
                 else
                   et1=ex
                   et2=ey
                 end if
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                     call ogf2dfo(ep,fieldOption,xy(i1    ,i2    ,i3,0)
     & ,xy(i1    ,i2    ,i3,1),t, u0,v0,w0)
                     uv(ex)=u0
                     uv(ey)=v0
                     u(i1,i2,i3,et1)=uv(et1)
                 end do
                 end do
                 end do
               end if
            end if
          else
            if( forcingOption.eq.planeWaveBoundaryForcing )then
              ! write(*,'(" ***assign corners:planeWaveBoundaryForcing: twoPi=",f18.14," cc=",f10.7)') twoPi,cc
               ! Set the tangential components to zero
               if( gridType.eq.curvilinear )then
                if( .true. )then ! *new way* *wdh* 2015/07/29
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                   ! if( mask(i1,i2,i3).ne.0 )then
                         ! get the outward normal for curvilinear grids
                         an(0)=rsxy(i1,i2,i3,axis,0)
                         an(1)=rsxy(i1,i2,i3,axis,1)
                           an(2)=rsxy(i1,i2,i3,axis,2)
                           anNorm = (2*side-1)/max( epsX, sqrt( an(0)**
     & 2 + an(1)**2 + an(2)**2 ) )
                           an(0)=an(0)*anNorm
                           an(1)=an(1)*anNorm
                           an(2)=an(2)*anNorm
                     ! set tangential components to zero by eliminating all but the normal component
                     !  E(new) = n.E(old) n 
                     nDotE = an(0)*u(i1,i2,i3,ex) + an(1)*u(i1,i2,i3,
     & ey) + an(2)*u(i1,i2,i3,ez)
                     u(i1,i2,i3,ex) = nDotE*an(0)
                     u(i1,i2,i3,ey) = nDotE*an(1)
                     u(i1,i2,i3,ez) = nDotE*an(2)
                     ! set tangential components to a non zero value: 
                     x0=xy(i1,i2,i3,0)
                     y0=xy(i1,i2,i3,1)
                     z0=xy(i1,i2,i3,2)
                     if( fieldOption.eq.0 )then
                       u0=-(ssf*sin(twoPi*(kx*(x0)+ky*(y0)+kz*(z0)-cc*(
     & t)))*pwc(0))
                       v0=-(ssf*sin(twoPi*(kx*(x0)+ky*(y0)+kz*(z0)-cc*(
     & t)))*pwc(1))
                       w0=-(ssf*sin(twoPi*(kx*(x0)+ky*(y0)+kz*(z0)-cc*(
     & t)))*pwc(2))
                     else
                      ! we are assigning time derivatives (sosup)
                       u0=-(ssf*(-twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)+
     & kz*(z0)-cc*(t)))*pwc(0)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)+kz*(z0)
     & -cc*(t)))*pwc(0))
                       v0=-(ssf*(-twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)+
     & kz*(z0)-cc*(t)))*pwc(1)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)+kz*(z0)
     & -cc*(t)))*pwc(1))
                       w0=-(ssf*(-twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)+
     & kz*(z0)-cc*(t)))*pwc(2)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)+kz*(z0)
     & -cc*(t)))*pwc(2))
                     end if
                     nDotE0 = an(0)*u0 + an(1)*v0 + an(2)*w0
                     u(i1,i2,i3,ex) = u(i1,i2,i3,ex) + u0 - nDotE0*an(
     & 0)
                     u(i1,i2,i3,ey) = u(i1,i2,i3,ey) + v0 - nDotE0*an(
     & 1)
                     u(i1,i2,i3,ez) = u(i1,i2,i3,ez) + w0 - nDotE0*an(
     & 2)
                   ! end if
                 end do
                 end do
                 end do
                else
                   ! ***** OLD WAY *****
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                   tau11=rsxy(i1,i2,i3,axisp1,0)
                   tau12=rsxy(i1,i2,i3,axisp1,1)
                   tau13=rsxy(i1,i2,i3,axisp1,2)
                   tau1DotU=(tau11*u(i1,i2,i3,ex)+tau12*u(i1,i2,i3,ey)+
     & tau13*u(i1,i2,i3,ez))/(tau11**2+tau12**2+tau13**2)
                   tau21=rsxy(i1,i2,i3,axisp2,0)
                   tau22=rsxy(i1,i2,i3,axisp2,1)
                   tau23=rsxy(i1,i2,i3,axisp2,2)
                   tau2DotU=(tau21*u(i1,i2,i3,ex)+tau22*u(i1,i2,i3,ey)+
     & tau23*u(i1,i2,i3,ez))/(tau21**2+tau22**2+tau23**2)
                     x0=xy(i1,i2,i3,0)
                     y0=xy(i1,i2,i3,1)
                     z0=xy(i1,i2,i3,2)
                     if( fieldOption.eq.0 )then
                       u0=-(ssf*sin(twoPi*(kx*(x0)+ky*(y0)+kz*(z0)-cc*(
     & t)))*pwc(0))
                       v0=-(ssf*sin(twoPi*(kx*(x0)+ky*(y0)+kz*(z0)-cc*(
     & t)))*pwc(1))
                       w0=-(ssf*sin(twoPi*(kx*(x0)+ky*(y0)+kz*(z0)-cc*(
     & t)))*pwc(2))
                     else
                      ! we are assigning time derivatives (sosup)
                       u0=-(ssf*(-twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)+
     & kz*(z0)-cc*(t)))*pwc(0)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)+kz*(z0)
     & -cc*(t)))*pwc(0))
                       v0=-(ssf*(-twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)+
     & kz*(z0)-cc*(t)))*pwc(1)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)+kz*(z0)
     & -cc*(t)))*pwc(1))
                       w0=-(ssf*(-twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(y0)+
     & kz*(z0)-cc*(t)))*pwc(2)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)+kz*(z0)
     & -cc*(t)))*pwc(2))
                     end if
                     tau1DotU = tau1DotU - ( tau11*u0 + tau12*v0 + 
     & tau13*w0 )/(tau11**2+tau12**2+tau13**2)
                     tau2DotU = tau2DotU - ( tau21*u0 + tau22*v0 + 
     & tau23*w0 )/(tau21**2+tau22**2+tau23**2)
                   ! ** this assumes tau1 and tau2 are orthogonal **
                   u(i1,i2,i3,ex)=u(i1,i2,i3,ex)-tau1DotU*tau11-
     & tau2DotU*tau21
                   u(i1,i2,i3,ey)=u(i1,i2,i3,ey)-tau1DotU*tau12-
     & tau2DotU*tau22
                   u(i1,i2,i3,ez)=u(i1,i2,i3,ez)-tau1DotU*tau13-
     & tau2DotU*tau23
               ! write(*,'("assignBoundary3d: i1,i2,i3=",3i3," x=",3f5.2," u0=",3f5.2," u=",3f5.2," tau1=",3f5.2," tau2=",3f5.2)')!   i1,i2,i3, xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2), u0,v0,w0,u(i1,i2,i3,ex),u(i1,i2,i3,ey),u(i1,i2,i3,ez),!   tau11,tau12,tau13,tau21,tau22,tau23
                 end do
                 end do
                 end do
                end if ! **** END OLD WAY
               else
                 if( axis.eq.0 )then
                   et1=ey
                   et2=ez
                 else if( axis.eq.1 )then
                   et1=ex
                   et2=ez
                 else
                   et1=ex
                   et2=ey
                 end if
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                     x0=xy(i1,i2,i3,0)
                     y0=xy(i1,i2,i3,1)
                     z0=xy(i1,i2,i3,2)
                     if( fieldOption.eq.0 )then
                       uv(ex)=-(ssf*sin(twoPi*(kx*(x0)+ky*(y0)+kz*(z0)-
     & cc*(t)))*pwc(0))
                       uv(ey)=-(ssf*sin(twoPi*(kx*(x0)+ky*(y0)+kz*(z0)-
     & cc*(t)))*pwc(1))
                       uv(ez)=-(ssf*sin(twoPi*(kx*(x0)+ky*(y0)+kz*(z0)-
     & cc*(t)))*pwc(2))
                     else
                      ! we are assigning time derivatives (sosup)
                       uv(ex)=-(ssf*(-twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(
     & y0)+kz*(z0)-cc*(t)))*pwc(0)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)+kz*
     & (z0)-cc*(t)))*pwc(0))
                       uv(ey)=-(ssf*(-twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(
     & y0)+kz*(z0)-cc*(t)))*pwc(1)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)+kz*
     & (z0)-cc*(t)))*pwc(1))
                       uv(ez)=-(ssf*(-twoPi*cc)*cos(twoPi*(kx*(x0)+ky*(
     & y0)+kz*(z0)-cc*(t)))*pwc(2)+ssft*sin(twoPi*(kx*(x0)+ky*(y0)+kz*
     & (z0)-cc*(t)))*pwc(2))
                     end if
                     u(i1,i2,i3,et1)=uv(et1)
                     u(i1,i2,i3,et2)=uv(et2)
                 end do
                 end do
                 end do
               end if
            else if( useForcing.eq.0 )then
               ! Set the tangential components to zero
               if( gridType.eq.curvilinear )then
                if( .true. )then ! *new way* *wdh* 2015/07/29
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                   ! if( mask(i1,i2,i3).ne.0 )then
                         ! get the outward normal for curvilinear grids
                         an(0)=rsxy(i1,i2,i3,axis,0)
                         an(1)=rsxy(i1,i2,i3,axis,1)
                           an(2)=rsxy(i1,i2,i3,axis,2)
                           anNorm = (2*side-1)/max( epsX, sqrt( an(0)**
     & 2 + an(1)**2 + an(2)**2 ) )
                           an(0)=an(0)*anNorm
                           an(1)=an(1)*anNorm
                           an(2)=an(2)*anNorm
                     ! set tangential components to zero by eliminating all but the normal component
                     !  E(new) = n.E(old) n 
                     nDotE = an(0)*u(i1,i2,i3,ex) + an(1)*u(i1,i2,i3,
     & ey) + an(2)*u(i1,i2,i3,ez)
                     u(i1,i2,i3,ex) = nDotE*an(0)
                     u(i1,i2,i3,ey) = nDotE*an(1)
                     u(i1,i2,i3,ez) = nDotE*an(2)
                   ! end if
                 end do
                 end do
                 end do
                else
                   ! ***** OLD WAY *****
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                   tau11=rsxy(i1,i2,i3,axisp1,0)
                   tau12=rsxy(i1,i2,i3,axisp1,1)
                   tau13=rsxy(i1,i2,i3,axisp1,2)
                   tau1DotU=(tau11*u(i1,i2,i3,ex)+tau12*u(i1,i2,i3,ey)+
     & tau13*u(i1,i2,i3,ez))/(tau11**2+tau12**2+tau13**2)
                   tau21=rsxy(i1,i2,i3,axisp2,0)
                   tau22=rsxy(i1,i2,i3,axisp2,1)
                   tau23=rsxy(i1,i2,i3,axisp2,2)
                   tau2DotU=(tau21*u(i1,i2,i3,ex)+tau22*u(i1,i2,i3,ey)+
     & tau23*u(i1,i2,i3,ez))/(tau21**2+tau22**2+tau23**2)
                   ! ** this assumes tau1 and tau2 are orthogonal **
                   u(i1,i2,i3,ex)=u(i1,i2,i3,ex)-tau1DotU*tau11-
     & tau2DotU*tau21
                   u(i1,i2,i3,ey)=u(i1,i2,i3,ey)-tau1DotU*tau12-
     & tau2DotU*tau22
                   u(i1,i2,i3,ez)=u(i1,i2,i3,ez)-tau1DotU*tau13-
     & tau2DotU*tau23
               ! write(*,'("assignBoundary3d: i1,i2,i3=",3i3," x=",3f5.2," u0=",3f5.2," u=",3f5.2," tau1=",3f5.2," tau2=",3f5.2)')!   i1,i2,i3, xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2), u0,v0,w0,u(i1,i2,i3,ex),u(i1,i2,i3,ey),u(i1,i2,i3,ez),!   tau11,tau12,tau13,tau21,tau22,tau23
                 end do
                 end do
                 end do
                end if ! **** END OLD WAY
               else
                 if( axis.eq.0 )then
                   et1=ey
                   et2=ez
                 else if( axis.eq.1 )then
                   et1=ex
                   et2=ez
                 else
                   et1=ex
                   et2=ey
                 end if
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                     u(i1,i2,i3,et1)=0.
                     u(i1,i2,i3,et2)=0.
                 end do
                 end do
                 end do
               end if
            else
               ! Set the tangential components to zero
               if( gridType.eq.curvilinear )then
                if( .true. )then ! *new way* *wdh* 2015/07/29
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                   ! if( mask(i1,i2,i3).ne.0 )then
                         ! get the outward normal for curvilinear grids
                         an(0)=rsxy(i1,i2,i3,axis,0)
                         an(1)=rsxy(i1,i2,i3,axis,1)
                           an(2)=rsxy(i1,i2,i3,axis,2)
                           anNorm = (2*side-1)/max( epsX, sqrt( an(0)**
     & 2 + an(1)**2 + an(2)**2 ) )
                           an(0)=an(0)*anNorm
                           an(1)=an(1)*anNorm
                           an(2)=an(2)*anNorm
                     ! set tangential components to zero by eliminating all but the normal component
                     !  E(new) = n.E(old) n 
                     nDotE = an(0)*u(i1,i2,i3,ex) + an(1)*u(i1,i2,i3,
     & ey) + an(2)*u(i1,i2,i3,ez)
                     u(i1,i2,i3,ex) = nDotE*an(0)
                     u(i1,i2,i3,ey) = nDotE*an(1)
                     u(i1,i2,i3,ez) = nDotE*an(2)
                     ! set tangential components to a non zero value: 
                     ! If we want   
                     !       tn . E = tn . E0
                     ! then set
                     !     E(new) = E(old) + E0 - (n.E0) n 
                     call ogf3dfo(ep,fieldOption,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),xy(i1,i2,i3,2),t, u0,v0,w0)
                     nDotE0 = an(0)*u0 + an(1)*v0 + an(2)*w0
                     u(i1,i2,i3,ex) = u(i1,i2,i3,ex) + u0 - nDotE0*an(
     & 0)
                     u(i1,i2,i3,ey) = u(i1,i2,i3,ey) + v0 - nDotE0*an(
     & 1)
                     u(i1,i2,i3,ez) = u(i1,i2,i3,ez) + w0 - nDotE0*an(
     & 2)
                   ! end if
                 end do
                 end do
                 end do
                else
                   ! ***** OLD WAY *****
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                   tau11=rsxy(i1,i2,i3,axisp1,0)
                   tau12=rsxy(i1,i2,i3,axisp1,1)
                   tau13=rsxy(i1,i2,i3,axisp1,2)
                   tau1DotU=(tau11*u(i1,i2,i3,ex)+tau12*u(i1,i2,i3,ey)+
     & tau13*u(i1,i2,i3,ez))/(tau11**2+tau12**2+tau13**2)
                   tau21=rsxy(i1,i2,i3,axisp2,0)
                   tau22=rsxy(i1,i2,i3,axisp2,1)
                   tau23=rsxy(i1,i2,i3,axisp2,2)
                   tau2DotU=(tau21*u(i1,i2,i3,ex)+tau22*u(i1,i2,i3,ey)+
     & tau23*u(i1,i2,i3,ez))/(tau21**2+tau22**2+tau23**2)
                     call ogf3dfo(ep,fieldOption,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),xy(i1,i2,i3,2),t, u0,v0,w0)
                     tau1DotU = tau1DotU - ( tau11*u0 + tau12*v0 + 
     & tau13*w0 )/(tau11**2+tau12**2+tau13**2)
                     tau2DotU = tau2DotU - ( tau21*u0 + tau22*v0 + 
     & tau23*w0 )/(tau21**2+tau22**2+tau23**2)
                   ! ** this assumes tau1 and tau2 are orthogonal **
                   u(i1,i2,i3,ex)=u(i1,i2,i3,ex)-tau1DotU*tau11-
     & tau2DotU*tau21
                   u(i1,i2,i3,ey)=u(i1,i2,i3,ey)-tau1DotU*tau12-
     & tau2DotU*tau22
                   u(i1,i2,i3,ez)=u(i1,i2,i3,ez)-tau1DotU*tau13-
     & tau2DotU*tau23
               ! write(*,'("assignBoundary3d: i1,i2,i3=",3i3," x=",3f5.2," u0=",3f5.2," u=",3f5.2," tau1=",3f5.2," tau2=",3f5.2)')!   i1,i2,i3, xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2), u0,v0,w0,u(i1,i2,i3,ex),u(i1,i2,i3,ey),u(i1,i2,i3,ez),!   tau11,tau12,tau13,tau21,tau22,tau23
                 end do
                 end do
                 end do
                end if ! **** END OLD WAY
               else
                 if( axis.eq.0 )then
                   et1=ey
                   et2=ez
                 else if( axis.eq.1 )then
                   et1=ex
                   et2=ez
                 else
                   et1=ex
                   et2=ey
                 end if
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                     call ogf3dfo(ep,fieldOption,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),xy(i1,i2,i3,2),t, u0,v0,w0)
                     uv(ex)=u0
                     uv(ey)=v0
                     uv(ez)=w0
                     u(i1,i2,i3,et1)=uv(et1)
                     u(i1,i2,i3,et2)=uv(et2)
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
        if( nd.eq.2 )then
          if( gridType.eq.rectangular )then
            if( useForcing.eq.0 )then
                axis=0
                axisp1=1
                i3=gridIndexRange(0,2)
                numberOfGhostPoints=orderOfAccuracy/2
                do side1=0,1
                do side2=0,1
                if( boundaryCondition(side1,0)
     & .eq.perfectElectricalConductor .and.boundaryCondition(side2,1)
     & .eq.perfectElectricalConductor )then
                  i1=gridIndexRange(side1,0) ! (i1,i2,i3)=corner point
                  i2=gridIndexRange(side2,1)
                  ! write(*,'("bcOpt: assign corner side1,side2,i1,i2,i3=",2i2,3i5)') side1,side2,i1,i2,i3
                  is1=1-2*side1
                  is2=1-2*side2
                  dra=dr(0)*is1
                  dsa=dr(1)*is2
                  g2a=0.
                  ! For now assign second ghost line by symmetry 
                  do m=1,numberOfGhostPoints
                    js1=is1*m  ! shift to ghost point "m"
                    js2=is2*m
                       u(i1,i2-js2,i3,ex)=2.*u(i1,i2,i3,ex)-u(i1,i2+
     & js2,i3,ex)
                       u(i1,i2-js2,i3,ey)=u(i1,i2+js2,i3,ey)
                       u(i1-js1,i2,i3,ex)=u(i1+js1,i2,i3,ex)
                       u(i1-js1,i2,i3,ey)=2.*u(i1,i2,i3,ey)-u(i1+js1,
     & i2,i3,ey)
                       u(i1,i2-js2,i3,hz)=u(i1,i2+js2,i3,hz)  ! Hz is even symmetry
                       u(i1-js1,i2,i3,hz)=u(i1+js1,i2,i3,hz)  ! Hz is even symmetry
                  end do
                  ! assign u(i1-is1,i2,i3,ev) and u(i1,i2-is2,i3,ev)
                    ! Now do corner (C) points
                    u(i1-  is1,i2-  is2,i3,ex)=-u(i1+  is1,i2+  is2,i3,
     & ex)
                    u(i1-  is1,i2-  is2,i3,ey)=-u(i1+  is1,i2+  is2,i3,
     & ey)
                    u(i1-  is1,i2-  is2,i3,hz)= u(i1+  is1,i2+  is2,i3,
     & hz)  ! Hz is even symmetry
                      u(i1-2*is1,i2-  is2,i3,ex)=-u(i1+2*is1,i2+  is2,
     & i3,ex)
                      u(i1-  is1,i2-2*is2,i3,ex)=-u(i1+  is1,i2+2*is2,
     & i3,ex)
                      u(i1-2*is1,i2-2*is2,i3,ex)=-u(i1+2*is1,i2+2*is2,
     & i3,ex)
                      u(i1-2*is1,i2-  is2,i3,ey)=-u(i1+2*is1,i2+  is2,
     & i3,ey)
                      u(i1-  is1,i2-2*is2,i3,ey)=-u(i1+  is1,i2+2*is2,
     & i3,ey)
                      u(i1-2*is1,i2-2*is2,i3,ey)=-u(i1+2*is1,i2+2*is2,
     & i3,ey)
                      u(i1-2*is1,i2-  is2,i3,hz)= u(i1+2*is1,i2+  is2,
     & i3,hz)
                      u(i1-  is1,i2-2*is2,i3,hz)= u(i1+  is1,i2+2*is2,
     & i3,hz)
                      u(i1-2*is1,i2-2*is2,i3,hz)= u(i1+2*is1,i2+2*is2,
     & i3,hz)
                else if( boundaryCondition(side1,0).ge.abcEM2 .and. 
     & boundaryCondition(side1,0).le.lastBC .and. boundaryCondition(
     & side2,1).ge.abcEM2 .and. boundaryCondition(side2,1).le.lastBC )
     & then
                  ! **** do nothing *** this is done in abcMaxwell
                end if
                end do
                end do
            else
                axis=0
                axisp1=1
                i3=gridIndexRange(0,2)
                numberOfGhostPoints=orderOfAccuracy/2
                do side1=0,1
                do side2=0,1
                if( boundaryCondition(side1,0)
     & .eq.perfectElectricalConductor .and.boundaryCondition(side2,1)
     & .eq.perfectElectricalConductor )then
                  i1=gridIndexRange(side1,0) ! (i1,i2,i3)=corner point
                  i2=gridIndexRange(side2,1)
                  ! write(*,'("bcOpt: assign corner side1,side2,i1,i2,i3=",2i2,3i5)') side1,side2,i1,i2,i3
                  is1=1-2*side1
                  is2=1-2*side2
                  dra=dr(0)*is1
                  dsa=dr(1)*is2
                  g2a=0.
                  ! For now assign second ghost line by symmetry 
                  do m=1,numberOfGhostPoints
                    js1=is1*m  ! shift to ghost point "m"
                    js2=is2*m
                       call ogf2dfo(ep,fieldOption,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),t, u0,v0,w0)
                       call ogf2dfo(ep,fieldOption,xy(i1,i2-js2,i3,0),
     & xy(i1,i2-js2,i3,1),t, um,vm,wm)
                       call ogf2dfo(ep,fieldOption,xy(i1,i2+js2,i3,0),
     & xy(i1,i2+js2,i3,1),t, up,vp,wp)
                       g1=um-2.*u0+up
                       g2=vm-vp
                       g3=wm-wp
                       u(i1,i2-js2,i3,ex)=2.*u(i1,i2,i3,ex)-u(i1,i2+
     & js2,i3,ex) +g1
                       u(i1,i2-js2,i3,ey)=u(i1,i2+js2,i3,ey)+g2
                       u(i1,i2-js2,i3,hz)=u(i1,i2+js2,i3,hz)+g3
                       call ogf2dfo(ep,fieldOption,xy(i1-js1,i2,i3,0),
     & xy(i1-js1,i2,i3,1),t, um,vm,wm)
                       call ogf2dfo(ep,fieldOption,xy(i1+js1,i2,i3,0),
     & xy(i1+js1,i2,i3,1),t, up,vp,wp)
                       g1=um-up
                       g2=vm-2.*v0+vp
                       g3=wm-wp
                       u(i1-js1,i2,i3,ex)=u(i1+js1,i2,i3,ex) +g1
                       u(i1-js1,i2,i3,ey)=2.*u(i1,i2,i3,ey)-u(i1+js1,
     & i2,i3,ey) +g2
                       u(i1-js1,i2,i3,hz)=u(i1+js1,i2,i3,hz)+g3
                  end do
                  ! assign u(i1-is1,i2,i3,ev) and u(i1,i2-is2,i3,ev)
                    ! dra=dr(0)  ! ** reset *** is this correct?
                    ! dsa=dr(1)
                     axis=0
                     axisp1=1
                        urr=uxx42r(i1,i2,i3,ex)  ! note: this is uxx
                        vrr=uxx42r(i1,i2,i3,ey)
                        uss=uyy42r(i1,i2,i3,ex)
                        vss=uyy42r(i1,i2,i3,ey)
                        urs=-vss  ! uxy=-vyy
                        vrs=-urr
                        urrrr=uxxxx22r(i1,i2,i3,ex)
                        ussss=uyyyy22r(i1,i2,i3,ex)
                        vrrrr=uxxxx22r(i1,i2,i3,ey)
                        vssss=uyyyy22r(i1,i2,i3,ey)
                        urrss=0.  ! from equation   uxxxx + uxxyy = uttxx  [ u(x,0)=0 => uxxxx(x,0)=0 uxxtt(x,0)=0 ]
                        vrrss=0.  ! from equation
                        urrrs=-vrrss  ! from div
                        ursss=-vssss  ! from div
                        vrrrs=-urrrr  ! from div
                        vrsss=-urrss  ! from div
                          u(i1-is1,i2-is2,i3,ex)= 2.*u(i1,i2,i3,ex)-u(
     & i1+is1,i2+is2,i3,ex) + ( (is1*dxa)**2*urr+2.*(is1*dxa)*(is2*
     & dya)*urs+(is2*dya)**2*uss )+ (1./12.)*( (is1*dxa)**4*urrrr + 
     & 4.*(is1*dxa)**3*(is2*dya)*urrrs + 6.*(is1*dxa)**2*(is2*dya)**2*
     & urrss + 4.*(is1*dxa)*(is2*dya)**3*ursss + (is2*dya)**4*ussss )
                          u(i1-is1,i2-is2,i3,ey)= 2.*u(i1,i2,i3,ey)-u(
     & i1+is1,i2+is2,i3,ey) + ( (is1*dxa)**2*vrr+2.*(is1*dxa)*(is2*
     & dya)*vrs+(is2*dya)**2*vss )+ (1./12.)*( (is1*dxa)**4*vrrrr + 
     & 4.*(is1*dxa)**3*(is2*dya)*vrrrs + 6.*(is1*dxa)**2*(is2*dya)**2*
     & vrrss + 4.*(is1*dxa)*(is2*dya)**3*vrsss + (is2*dya)**4*vssss )
                          u(i1-2*is1,i2-is2,i3,ex)= 2.*u(i1,i2,i3,ex)-
     & u(i1+2*is1,i2+is2,i3,ex) + ( (2.*is1*dxa)**2*urr+2.*(2.*is1*
     & dxa)*(is2*dya)*urs+(is2*dya)**2*uss )+ (1./12.)*( (2.*is1*dxa)*
     & *4*urrrr + 4.*(2.*is1*dxa)**3*(is2*dya)*urrrs + 6.*(2.*is1*dxa)
     & **2*(is2*dya)**2*urrss + 4.*(2.*is1*dxa)*(is2*dya)**3*ursss + (
     & is2*dya)**4*ussss )
                          u(i1-2*is1,i2-is2,i3,ey)= 2.*u(i1,i2,i3,ey)-
     & u(i1+2*is1,i2+is2,i3,ey) + ( (2.*is1*dxa)**2*vrr+2.*(2.*is1*
     & dxa)*(is2*dya)*vrs+(is2*dya)**2*vss )+ (1./12.)*( (2.*is1*dxa)*
     & *4*vrrrr + 4.*(2.*is1*dxa)**3*(is2*dya)*vrrrs + 6.*(2.*is1*dxa)
     & **2*(is2*dya)**2*vrrss + 4.*(2.*is1*dxa)*(is2*dya)**3*vrsss + (
     & is2*dya)**4*vssss )
                          u(i1-is1,i2-2*is2,i3,ex)= 2.*u(i1,i2,i3,ex)-
     & u(i1+is1,i2+2*is2,i3,ex) + ( (is1*dxa)**2*urr+2.*(is1*dxa)*(2.*
     & is2*dya)*urs+(2.*is2*dya)**2*uss )+ (1./12.)*( (is1*dxa)**4*
     & urrrr + 4.*(is1*dxa)**3*(2.*is2*dya)*urrrs + 6.*(is1*dxa)**2*(
     & 2.*is2*dya)**2*urrss + 4.*(is1*dxa)*(2.*is2*dya)**3*ursss + (
     & 2.*is2*dya)**4*ussss )
                          u(i1-is1,i2-2*is2,i3,ey)= 2.*u(i1,i2,i3,ey)-
     & u(i1+is1,i2+2*is2,i3,ey) + ( (is1*dxa)**2*vrr+2.*(is1*dxa)*(2.*
     & is2*dya)*vrs+(2.*is2*dya)**2*vss )+ (1./12.)*( (is1*dxa)**4*
     & vrrrr + 4.*(is1*dxa)**3*(2.*is2*dya)*vrrrs + 6.*(is1*dxa)**2*(
     & 2.*is2*dya)**2*vrrss + 4.*(is1*dxa)*(2.*is2*dya)**3*vrsss + (
     & 2.*is2*dya)**4*vssss )
                          u(i1-2*is1,i2-2*is2,i3,ex)= 2.*u(i1,i2,i3,ex)
     & -u(i1+2*is1,i2+2*is2,i3,ex) + ( (2.*is1*dxa)**2*urr+2.*(2.*is1*
     & dxa)*(2.*is2*dya)*urs+(2.*is2*dya)**2*uss )+ (1./12.)*( (2.*
     & is1*dxa)**4*urrrr + 4.*(2.*is1*dxa)**3*(2.*is2*dya)*urrrs + 6.*
     & (2.*is1*dxa)**2*(2.*is2*dya)**2*urrss + 4.*(2.*is1*dxa)*(2.*
     & is2*dya)**3*ursss + (2.*is2*dya)**4*ussss )
                          u(i1-2*is1,i2-2*is2,i3,ey)= 2.*u(i1,i2,i3,ey)
     & -u(i1+2*is1,i2+2*is2,i3,ey) + ( (2.*is1*dxa)**2*vrr+2.*(2.*is1*
     & dxa)*(2.*is2*dya)*vrs+(2.*is2*dya)**2*vss )+ (1./12.)*( (2.*
     & is1*dxa)**4*vrrrr + 4.*(2.*is1*dxa)**3*(2.*is2*dya)*vrrrs + 6.*
     & (2.*is1*dxa)**2*(2.*is2*dya)**2*vrrss + 4.*(2.*is1*dxa)*(2.*
     & is2*dya)**3*vrsss + (2.*is2*dya)**4*vssss )
                        ! Now do Hz
                        ur = ux42r(i1,i2,i3,hz)
                        us = uy42r(i1,i2,i3,hz)
                        urrr=uxxx22r(i1,i2,i3,hz)   ! 2nd order should be good enough
                        usss=uyyy22r(i1,i2,i3,hz)
                        urrs=0. !  (from ux(0,s)=0 and uy(r,0)=0)   ! ****************** fix for TZ
                        urss=0. !  (from ux(0,s)=0 and uy(r,0)=0)
                          ! just set uxxy and uxyy to the exact values.
                          call ogDeriv(ep, 0, 2,1,0, xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.,t,hz, urrs)
                          call ogDeriv(ep, 0, 1,2,0, xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.,t,hz, urss)
                            u(i1-is1,i2-is2,i3,hz)=u(i1+is1,i2+is2,i3,
     & hz) - 2.*((is1*dxa)*ur+(is2*dya)*us) - (1./3.)*((is1*dxa)**3*
     & urrr+3.*(is1*dxa)**2*(is2*dya)*urrs+3.*(is1*dxa)*(is2*dya)**2*
     & urss+(is2*dya)**3*usss)
                            u(i1-2*is1,i2-is2,i3,hz)=u(i1+2*is1,i2+is2,
     & i3,hz) - 2.*((2.*is1*dxa)*ur+(is2*dya)*us) - (1./3.)*((2.*is1*
     & dxa)**3*urrr+3.*(2.*is1*dxa)**2*(is2*dya)*urrs+3.*(2.*is1*dxa)*
     & (is2*dya)**2*urss+(is2*dya)**3*usss)
                            u(i1-is1,i2-2*is2,i3,hz)=u(i1+is1,i2+2*is2,
     & i3,hz) - 2.*((is1*dxa)*ur+(2.*is2*dya)*us) - (1./3.)*((is1*dxa)
     & **3*urrr+3.*(is1*dxa)**2*(2.*is2*dya)*urrs+3.*(is1*dxa)*(2.*
     & is2*dya)**2*urss+(2.*is2*dya)**3*usss)
                            u(i1-2*is1,i2-2*is2,i3,hz)=u(i1+2*is1,i2+2*
     & is2,i3,hz) - 2.*((2.*is1*dxa)*ur+(2.*is2*dya)*us) - (1./3.)*((
     & 2.*is1*dxa)**3*urrr+3.*(2.*is1*dxa)**2*(2.*is2*dya)*urrs+3.*(
     & 2.*is1*dxa)*(2.*is2*dya)**2*urss+(2.*is2*dya)**3*usss)
                else if( boundaryCondition(side1,0).ge.abcEM2 .and. 
     & boundaryCondition(side1,0).le.lastBC .and. boundaryCondition(
     & side2,1).ge.abcEM2 .and. boundaryCondition(side2,1).le.lastBC )
     & then
                  ! **** do nothing *** this is done in abcMaxwell
                end if
                end do
                end do
            end if
          else
            if( useForcing.eq.0 )then
                axis=0
                axisp1=1
                i3=gridIndexRange(0,2)
                numberOfGhostPoints=orderOfAccuracy/2
                do side1=0,1
                do side2=0,1
                if( boundaryCondition(side1,0)
     & .eq.perfectElectricalConductor .and.boundaryCondition(side2,1)
     & .eq.perfectElectricalConductor )then
                  i1=gridIndexRange(side1,0) ! (i1,i2,i3)=corner point
                  i2=gridIndexRange(side2,1)
                  ! write(*,'("bcOpt: assign corner side1,side2,i1,i2,i3=",2i2,3i5)') side1,side2,i1,i2,i3
                  is1=1-2*side1
                  is2=1-2*side2
                  dra=dr(0)*is1
                  dsa=dr(1)*is2
                  g2a=0.
                  ! For now assign second ghost line by symmetry 
                  do m=1,numberOfGhostPoints
                    js1=is1*m  ! shift to ghost point "m"
                    js2=is2*m
                     ! *** there is no need to do this for orderOfAccuracy.eq.4 -- these are done below
                  end do
                  ! assign u(i1-is1,i2,i3,ev) and u(i1,i2-is2,i3,ev)
                      ! write(*,'("assign extended-curvilinear-order4 grid,side,axis=",i5,2i3)') grid,side,axis
                      axis=0   ! for c11, c22, ...
                      axisp1=1
                       a11m2 =rsxy(i1-2*is1,i2    ,i3,0,0)
                       a12m2 =rsxy(i1-2*is1,i2    ,i3,0,1)
                       a11m1 =rsxy(i1-is1,i2    ,i3,0,0)
                       a12m1 =rsxy(i1-is1,i2    ,i3,0,1)
                       a11   =rsxy(i1    ,i2    ,i3,0,0)
                       a12   =rsxy(i1    ,i2    ,i3,0,1)
                       a21   =rsxy(i1    ,i2    ,i3,1,0)
                       a22   =rsxy(i1    ,i2    ,i3,1,1)
                       a21zm1 =rsxy(i1   ,i2-is2,i3,1,0)
                       a22zm1 =rsxy(i1   ,i2-is2,i3,1,1)
                       a21zm2 =rsxy(i1   ,i2-2*is2,i3,1,0)
                       a22zm2 =rsxy(i1   ,i2-2*is2,i3,1,1)
                       c11 = (rsxy(i1,i2,i3,axis,0)**2+rsxy(i1,i2,i3,
     & axis,1)**2)
                       c22 = (rsxy(i1,i2,i3,axisp1,0)**2+rsxy(i1,i2,i3,
     & axisp1,1)**2)
                       c1  = (rsxyx42(i1,i2,i3,axis,0)+rsxyy42(i1,i2,
     & i3,axis,1))
                       c2  = (rsxyx42(i1,i2,i3,axisp1,0)+rsxyy42(i1,i2,
     & i3,axisp1,1))
                       c11r = (8.*((rsxy(i1+is1,i2,i3,axis,0)**2+rsxy(
     & i1+is1,i2,i3,axis,1)**2)-(rsxy(i1-is1,i2,i3,axis,0)**2+rsxy(i1-
     & is1,i2,i3,axis,1)**2))   -((rsxy(i1+2*is1,i2,i3,axis,0)**2+
     & rsxy(i1+2*is1,i2,i3,axis,1)**2)-(rsxy(i1-2*is1,i2,i3,axis,0)**
     & 2+rsxy(i1-2*is1,i2,i3,axis,1)**2))   )/(12.*dra)
                       c22r = (8.*((rsxy(i1+is1,i2,i3,axisp1,0)**2+
     & rsxy(i1+is1,i2,i3,axisp1,1)**2)-(rsxy(i1-is1,i2,i3,axisp1,0)**
     & 2+rsxy(i1-is1,i2,i3,axisp1,1)**2))   -((rsxy(i1+2*is1,i2,i3,
     & axisp1,0)**2+rsxy(i1+2*is1,i2,i3,axisp1,1)**2)-(rsxy(i1-2*is1,
     & i2,i3,axisp1,0)**2+rsxy(i1-2*is1,i2,i3,axisp1,1)**2))   )/(12.*
     & dra)
                       c11s = (8.*((rsxy(i1,i2+is2,i3,axis,0)**2+rsxy(
     & i1,i2+is2,i3,axis,1)**2)-(rsxy(i1,i2-is2,i3,axis,0)**2+rsxy(i1,
     & i2-is2,i3,axis,1)**2))   -((rsxy(i1,i2+2*is2,i3,axis,0)**2+
     & rsxy(i1,i2+2*is2,i3,axis,1)**2)-(rsxy(i1,i2-2*is2,i3,axis,0)**
     & 2+rsxy(i1,i2-2*is2,i3,axis,1)**2))   )/(12.*dsa)
                       c22s = (8.*((rsxy(i1,i2+is2,i3,axisp1,0)**2+
     & rsxy(i1,i2+is2,i3,axisp1,1)**2)-(rsxy(i1,i2-is2,i3,axisp1,0)**
     & 2+rsxy(i1,i2-is2,i3,axisp1,1)**2))   -((rsxy(i1,i2+2*is2,i3,
     & axisp1,0)**2+rsxy(i1,i2+2*is2,i3,axisp1,1)**2)-(rsxy(i1,i2-2*
     & is2,i3,axisp1,0)**2+rsxy(i1,i2-2*is2,i3,axisp1,1)**2))   )/(
     & 12.*dsa)
                       !  Solve for Hz on extended boundaries from:  
                       !          wr=fw1  
                       !          ws=fw2  
                       !          c11*wrrr+(c1+c11r)*wrr + c22r*wss=fw3, (i.e. (Lw).r=0 )
                       !          c22*wsss+(c2+c22s)*wss + c11s*wrr=fw4, (i.e. (Lw).s=0 )
                       fw1=0.
                       fw2=0.
                       fw3=0.
                       fw4=0.
                         tdu10=0.  ! e1 := (a11*u+a12*v)(i1-1,i2,i3) - tdu10:
                         tdu01=0.  ! e2 := (a21*u+a22*v)(i1,i2-1,i3) - tdu01:
                         tdu20=0.
                         tdu02=0.
                         ! Lua := cu20*u(i1-2,i2,i3) + cu10*u(i1-1,i2,i3) + cu02*u(i1,i2-2,i3) + cu01*u(i1,i2-1,i3) + gLu - utt00:
                         utt00=0.  ! u.tt
                         vtt00=0.  ! u.tt
                        ! uLap = uLaplacian42(i1,i2,i3,ex)
                        ! vLap = uLaplacian42(i1,i2,i3,ey)
                        ! Drop the cross term for now -- this should be fixed for non-orthogonal grids ---
                        uLap = c11*urr4(i1,i2,i3,ex)+c22*uss4(i1,i2,i3,
     & ex)+c1*ur4(i1,i2,i3,ex)+c2*us4(i1,i2,i3,ex)
                        vLap = c11*urr4(i1,i2,i3,ey)+c22*uss4(i1,i2,i3,
     & ey)+c1*ur4(i1,i2,i3,ey)+c2*us4(i1,i2,i3,ey)
                      ! The next file is from bc4c.maple


! ****************** Fourth-order ********************
      g1a = a21*(6*u(i1,i2,i3,ex)-4*u(i1+is1,i2,i3,ex)+u(i1+2*is1,i2,
     & i3,ex))+a22*(6*u(i1,i2,i3,ey)-4*u(i1+is1,i2,i3,ey)+u(i1+2*is1,
     & i2,i3,ey))

      g2a = a11*(6*u(i1,i2,i3,ex)-4*u(i1,i2+is2,i3,ex)+u(i1,i2+2*is2,
     & i3,ex))+a12*(6*u(i1,i2,i3,ey)-4*u(i1,i2+is2,i3,ey)+u(i1,i2+2*
     & is2,i3,ey))

      cu20=-1/12.*c11/dra**2+1/12.*c1/dra

      cu02=-1/12.*c22/dsa**2+1/12.*c2/dsa

      cu10=4/3.*c11/dra**2-2/3.*c1/dra

      cu01=4/3.*c22/dsa**2-2/3.*c2/dsa

      cv20=-1/12.*c11/dra**2+1/12.*c1/dra

      cv02=-1/12.*c22/dsa**2+1/12.*c2/dsa

      cv10=4/3.*c11/dra**2-2/3.*c1/dra

      cv01=4/3.*c22/dsa**2-2/3.*c2/dsa

      gLu= uLap-(cu20*u(i1-2*is1,i2,i3,ex) + cu10*u(i1-is1,i2,i3,ex) + 
     & cu02*u(i1,i2-2*is2,i3,ex) + cu01*u(i1,i2-is2,i3,ex)) -utt00

      gLv= vLap-(cv20*u(i1-2*is1,i2,i3,ey) + cv10*u(i1-is1,i2,i3,ey) + 
     & cv02*u(i1,i2-2*is2,i3,ey) + cv01*u(i1,i2-is2,i3,ey)) -vtt00

      det=4*a11m2*a22*cv02*a21zm2*a12*a21zm1*cu10*a12m1+4*a11m1*a22*
     & cu20*a12m2*cv01*a21zm1*a21zm2*a12-16*a21*a12m1*a21zm2*cu20*
     & a12m2*cv02*a12*a21zm1-4*a21*a12m1*cu20*a12m2*cv01*a21zm1*
     & a21zm2*a12-a21*a12m2*cv01*a21zm1*a21zm2*a12*cu10*a12m1+16*
     & a11m1*a22*a21zm2*cu20*a12m2*cv02*a12*a21zm1-4*a21*a12m2*cv02*
     & a21zm2*a12*a21zm1*cu10*a12m1+16*a11m2*cv20*a21*a12m1*cu02*
     & a22zm2*a12*a21zm1-16*a11m2*a11m1*cv20*a22*cu02*a22zm2*a12*
     & a21zm1-a11m1*a21*a12m2*cv10*a22zm1*cu01*a11*a22zm2-4*a11m1*a21*
     & a12m2*cv10*a22zm1*cu02*a22zm2*a11+4*a11m2*a11m1*cv20*a22*
     & a22zm1*cu01*a11*a22zm2+16*a11m2*a11m1*cv20*a22*a22zm1*cu02*
     & a22zm2*a11-4*a11m2*a11m1*a22*cv10*cu02*a22zm2*a12*a21zm1+4*
     & a11m2*a11m1*a22*cv10*a22zm1*cu02*a22zm2*a11-a11m2*a22*cv01*
     & a21zm1*a11*a22zm2*cu10*a12m1-4*a11m1*a22*cu20*a12m2*cv01*
     & a21zm1*a11*a22zm2+4*a21*a12m1*cu20*a12m2*cv01*a21zm1*a11*
     & a22zm2+4*a11m1*a21*a12m2*cv10*cu02*a22zm2*a12*a21zm1+a21*a12m2*
     & cv01*a21zm1*a11*a22zm2*cu10*a12m1-4*a11m2*cv20*a21*a12m1*
     & a22zm1*cu01*a11*a22zm2-16*a11m2*cv20*a21*a12m1*a22zm1*cu02*
     & a22zm2*a11+a11m2*a11m1*a22*cv10*a22zm1*cu01*a11*a22zm2-a11m2*
     & a11m1*a22*cv10*a22zm1*cu01*a21zm2*a12+16*a21*a12m1*a22zm1*
     & a21zm2*cu20*a12m2*cv02*a11+4*a22zm1*a21*a12m2*cv02*a21zm2*a11*
     & cu10*a12m1+4*a11m2*cv20*a21*a12m1*a22zm1*cu01*a21zm2*a12+a11m2*
     & a22*cv01*a21zm1*a21zm2*a12*cu10*a12m1+a11m1*a21*a12m2*cv10*
     & a22zm1*cu01*a21zm2*a12-16*a11m1*a22*a22zm1*a21zm2*cu20*a12m2*
     & cv02*a11-4*a11m2*a11m1*cv20*a22*a22zm1*cu01*a21zm2*a12-4*a11m2*
     & a22zm1*a22*cv02*a21zm2*a11*cu10*a12m1

      u(i1-is1,i2,i3,ex)=(a12m1*cu01*a21zm2*a12*a21*a12m2*cv01*tdu01+
     & a12m1*a22zm1*a11m2*cv20*cu01*a21zm2*a12*g1a+a12m1*a22zm1*cu01*
     & a21zm2*a12*a21*tdu20*cv20+4*a12m1*a22zm1*a11*a21zm2*cu20*tdu20*
     & a22*cv02+4*a12m1*a22zm1*a11*a21zm2*cu20*a12m2*cv02*g1a+a12m1*
     & a22zm1*cu01*a21zm2*a12*a21*a12m2*gLv-4*a12m1*a22zm1*a11*a21zm2*
     & gLu*a21*a12m2*cv02-a12m1*a22zm1*cu01*a21zm2*g2a*a21*a12m2*cv02-
     & 4*tdu10*a11m2*cv20*a22*a22zm1*cu01*a21zm2*a12-4*a12m1*a11*cu02*
     & a22zm2*a21*a12m2*cv01*tdu01-4*a12m1*a21zm2*cu20*tdu20*a12*
     & a21zm1*a22*cv02+a12m1*a22zm1*a11m2*cu01*a21zm2*g2a*a22*cv02-
     & a12m1*a22zm1*a11m2*cu01*a21zm2*a12*a22*gLv+4*a12m1*a22zm1*
     & a11m2*a11*a21zm2*gLu*a22*cv02+tdu10*a21*a12m2*cv10*a22zm1*cu01*
     & a21zm2*a12-16*tdu10*a22*a22zm1*a21zm2*cu20*a12m2*cv02*a11-
     & tdu10*a11m2*a22*cv10*a22zm1*cu01*a21zm2*a12-4*a12m1*a21zm2*
     & cu20*a12m2*cv02*a12*a21zm1*g1a-a12m1*cu20*a12m2*cv01*a21zm1*
     & a21zm2*a12*g1a+4*a12m1*a21zm2*gLu*a12*a21zm1*a21*a12m2*cv02+
     & a12m1*a21zm2*gLu*a21*a12m2*cv01*a21zm1*a12-a12m1*a21zm2*cu20*
     & tdu20*a22*cv01*a21zm1*a12+4*a12m1*cu01*a21zm2*a12*tdu01*a21*
     & a12m2*cv02-a12m1*a11m2*cu01*a21zm2*a12*a22*cv01*tdu01-4*a12m1*
     & a11m2*cu01*a21zm2*a12*tdu01*a22*cv02-a12m1*a11m2*a21zm2*gLu*
     & a22*cv01*a21zm1*a12-4*a12m1*a11m2*a21zm2*gLu*a12*a21zm1*a22*
     & cv02+4*tdu10*a22*cu20*a12m2*cv01*a21zm1*a21zm2*a12+16*tdu10*
     & a22*a21zm2*cu20*a12m2*cv02*a12*a21zm1-a12m1*a22zm1*a11*cv20*
     & cu01*a22zm2*a21*tdu20-a12m1*a22zm1*a11m2*a11*cv20*cu01*a22zm2*
     & g1a+4*a12m1*a11m2*a11*cu02*a22zm2*a22*cv01*tdu01+a12m1*a11m2*
     & a11*cu01*a22zm2*a22*cv01*tdu01-a12m1*a11*cu01*a22zm2*a21*a12m2*
     & cv01*tdu01+4*tdu10*a11m2*cv20*a22*a22zm1*cu01*a11*a22zm2+4*
     & tdu10*a21*a12m2*cv10*cu02*a22zm2*a12*a21zm1+a12m1*a11*cu20*
     & a12m2*cv01*a21zm1*a22zm2*g1a+4*a12m1*cu02*a22zm2*a12*a21zm1*
     & a21*a12m2*gLv+4*a12m1*cu02*a22zm2*a12*a21zm1*a21*tdu20*cv20+
     & a12m1*a11*cu20*tdu20*a22*cv01*a21zm1*a22zm2+4*a12m1*a11m2*cv20*
     & cu02*a22zm2*a12*a21zm1*g1a-4*tdu10*a21*a12m2*cv10*a22zm1*cu02*
     & a22zm2*a11-tdu10*a21*a12m2*cv10*a22zm1*cu01*a11*a22zm2-a12m1*
     & a11m2*cu02*a22zm2*a22*cv01*a21zm1*g2a-4*a12m1*a11m2*cu02*
     & a22zm2*a12*a21zm1*a22*gLv+a12m1*a11m2*a11*gLu*a22*cv01*a21zm1*
     & a22zm2-4*tdu10*a11m2*a22*cv10*cu02*a22zm2*a12*a21zm1-16*tdu10*
     & a11m2*cv20*a22*cu02*a22zm2*a12*a21zm1+a12m1*cu02*a22zm2*a21*
     & a12m2*cv01*a21zm1*g2a-a12m1*a11*gLu*a21*a12m2*cv01*a21zm1*
     & a22zm2+tdu10*a11m2*a22*cv10*a22zm1*cu01*a11*a22zm2+a12m1*
     & a22zm1*a11m2*a11*cu01*a22zm2*a22*gLv+16*tdu10*a11m2*cv20*a22*
     & a22zm1*cu02*a22zm2*a11+4*tdu10*a11m2*a22*cv10*a22zm1*cu02*
     & a22zm2*a11-a12m1*a22zm1*a11*cu01*a22zm2*a21*a12m2*gLv-4*a12m1*
     & a22zm1*a11*cu02*a22zm2*a21*a12m2*gLv-4*a12m1*a22zm1*a11m2*a11*
     & cv20*cu02*a22zm2*g1a+4*a12m1*a22zm1*a11m2*a11*cu02*a22zm2*a22*
     & gLv-4*a12m1*a22zm1*a11*cv20*cu02*a22zm2*a21*tdu20-4*a12m1*
     & a11m2*cu02*tdu02*a12*a21zm1*a22*cv02-a12m1*a11m2*cu02*tdu02*
     & a22*cv01*a21zm1*a12+4*a12m1*cu02*tdu02*a12*a21zm1*a21*a12m2*
     & cv02+a12m1*cu02*tdu02*a21*a12m2*cv01*a21zm1*a12-4*tdu10*a22*
     & cu20*a12m2*cv01*a21zm1*a11*a22zm2+4*a12m1*a22zm1*a11m2*a11*
     & cu02*tdu02*a22*cv02+a12m1*a22zm1*a11m2*a11*cu01*tdu02*a22*cv02-
     & 4*a12m1*a22zm1*a11*cu02*tdu02*a21*a12m2*cv02-a12m1*a22zm1*a11*
     & cu01*tdu02*a21*a12m2*cv02)/det

      u(i1-is1,i2,i3,ey)=(-4*a22zm1*a11m2*a11*a11m1*cu02*tdu02*a22*
     & cv02-a22zm1*a11m2*a11*a11m1*cu01*tdu02*a22*cv02+4*a22zm1*a11*
     & a11m1*cu02*tdu02*a21*a12m2*cv02+a22zm1*a11*a11m1*cu01*tdu02*
     & a21*a12m2*cv02+4*a11m2*a11m1*cu02*tdu02*a12*a21zm1*a22*cv02-4*
     & a11m1*cu02*tdu02*a12*a21zm1*a21*a12m2*cv02+a11m2*a11m1*cu02*
     & tdu02*a22*cv01*a21zm1*a12-a11m1*cu02*tdu02*a21*a12m2*cv01*
     & a21zm1*a12+a11*a11m1*cu01*a22zm2*a21*a12m2*cv01*tdu01+4*a11*
     & a11m1*cu02*a22zm2*a21*a12m2*cv01*tdu01+4*a22zm1*a11*cv20*a11m1*
     & cu02*a22zm2*a21*tdu20-4*a22zm1*a11m2*a11*a11m1*cu02*a22zm2*a22*
     & gLv+a22zm1*a11m2*a11*cv20*a11m1*cu01*a22zm2*g1a-4*a22zm1*a11m2*
     & a11*cv20*tdu10*a21*cu01*a22zm2-16*a22zm1*a11m2*a11*cv20*tdu10*
     & a21*cu02*a22zm2-a22zm1*a11m2*a11*a11m1*cu01*a22zm2*a22*gLv+4*
     & a22zm1*a11m2*a11*cv20*a11m1*cu02*a22zm2*g1a+4*a22zm1*a11*a11m1*
     & cu02*a22zm2*a21*a12m2*gLv+a22zm1*a11*a11m1*cu01*a22zm2*a21*
     & a12m2*gLv+a11*a11m1*gLu*a21*a12m2*cv01*a21zm1*a22zm2-4*a11m1*
     & cu02*a22zm2*a12*a21zm1*a21*a12m2*gLv-a11m2*a11*tdu10*a22*cv01*
     & a21zm1*a22zm2*cu10+16*a11m2*cv20*tdu10*a12*a21zm1*a21*cu02*
     & a22zm2-a11*a11m1*cu20*a12m2*cv01*a21zm1*a22zm2*g1a+a11*tdu10*
     & a21*a12m2*cv01*a21zm1*a22zm2*cu10+4*a11*tdu10*a21*cu20*a12m2*
     & cv01*a21zm1*a22zm2-a11m1*cu02*a22zm2*a21*a12m2*cv01*a21zm1*g2a-
     & a11m2*a11*a11m1*gLu*a22*cv01*a21zm1*a22zm2+4*a11m2*a11m1*cu02*
     & a22zm2*a12*a21zm1*a22*gLv-a11*a11m1*cu20*tdu20*a22*cv01*a21zm1*
     & a22zm2-4*a11m1*cu02*a22zm2*a12*a21zm1*a21*tdu20*cv20+a11m2*
     & a11m1*cu02*a22zm2*a22*cv01*a21zm1*g2a-4*a11m2*cv20*a11m1*cu02*
     & a22zm2*a12*a21zm1*g1a-4*a11m1*a21zm2*gLu*a12*a21zm1*a21*a12m2*
     & cv02-a11m1*a21zm2*gLu*a21*a12m2*cv01*a21zm1*a12+4*a11m1*a21zm2*
     & cu20*a12m2*cv02*a12*a21zm1*g1a+4*a11m1*a21zm2*cu20*tdu20*a12*
     & a21zm1*a22*cv02+a11m1*a21zm2*cu20*tdu20*a22*cv01*a21zm1*a12+
     & a11m2*a11m1*a21zm2*gLu*a22*cv01*a21zm1*a12+a22zm1*a11*cv20*
     & a11m1*cu01*a22zm2*a21*tdu20-a11m2*a11*a11m1*cu01*a22zm2*a22*
     & cv01*tdu01-4*a11m2*a11*a11m1*cu02*a22zm2*a22*cv01*tdu01+4*
     & a22zm1*a11*a21zm2*tdu10*a21*a12m2*cv02*cu10+16*a22zm1*a11*
     & a21zm2*tdu10*a21*cu20*a12m2*cv02+4*a22zm1*a11*a11m1*a21zm2*gLu*
     & a21*a12m2*cv02-a22zm1*a11m1*cu01*a21zm2*a12*a21*a12m2*gLv-4*
     & a22zm1*a11*a11m1*a21zm2*cu20*a12m2*cv02*g1a+a22zm1*a11m1*cu01*
     & a21zm2*g2a*a21*a12m2*cv02-4*a22zm1*a11m2*a11*a21zm2*tdu10*a22*
     & cv02*cu10+4*a22zm1*a11m2*cv20*a21zm2*tdu10*a21*cu01*a12+a11m1*
     & cu20*a12m2*cv01*a21zm1*a21zm2*a12*g1a+4*a11m2*cu10*a21zm2*
     & tdu10*a12*a21zm1*a22*cv02+4*a11m2*a11m1*a21zm2*gLu*a12*a21zm1*
     & a22*cv02-cu10*a21zm2*tdu10*a21*a12m2*cv01*a21zm1*a12-4*a21zm2*
     & tdu10*a21*cu20*a12m2*cv01*a21zm1*a12-4*cu10*a21zm2*tdu10*a12*
     & a21zm1*a21*a12m2*cv02+4*a11m2*a11m1*cu01*a21zm2*a12*tdu01*a22*
     & cv02+a11m2*a11m1*cu01*a21zm2*a12*a22*cv01*tdu01-4*a11m1*cu01*
     & a21zm2*a12*tdu01*a21*a12m2*cv02-a11m1*cu01*a21zm2*a12*a21*
     & a12m2*cv01*tdu01+a11m2*cu10*a21zm2*tdu10*a22*cv01*a21zm1*a12-
     & 16*a21zm2*tdu10*a12*a21zm1*a21*cu20*a12m2*cv02-a22zm1*a11m1*
     & cu01*a21zm2*a12*a21*tdu20*cv20-a22zm1*a11m2*a11m1*cu01*a21zm2*
     & g2a*a22*cv02-a22zm1*a11m2*cv20*a11m1*cu01*a21zm2*a12*g1a-4*
     & a22zm1*a11m2*a11*a11m1*a21zm2*gLu*a22*cv02+a22zm1*a11m2*a11m1*
     & cu01*a21zm2*a12*a22*gLv-4*a22zm1*a11*a11m1*a21zm2*cu20*tdu20*
     & a22*cv02)/det

      u(i1,i2-is2,i3,ex)=(a22zm1*a11*a22zm2*cv20*a21*tdu20*cu10*a12m1-
     & a22zm1*a11*a22zm2*a11m1*a22*cv10*cu20*tdu20+4*a22zm1*a11*
     & a22zm2*a21*a12m1*cu20*a12m2*gLv+a22zm1*a11*a22zm2*a21*a12m2*
     & gLv*cu10*a12m1-4*a22zm1*g2a*a11m2*cv20*a21*a12m1*cu02*a22zm2-
     & a22zm1*a11*a22zm2*a11m2*a22*gLv*cu10*a12m1+4*a22zm1*a11*a22zm2*
     & a11m2*cv20*a21*a12m1*gLu-4*a22zm1*a11*a22zm2*a11m2*a11m1*cv20*
     & a22*gLu+a22zm1*a11*a22zm2*a11m2*cv20*g1a*cu10*a12m1-a22zm1*a11*
     & a22zm2*a11m2*a11m1*a22*cv10*gLu-4*a22zm1*a11*a22zm2*a11m1*a22*
     & cu20*a12m2*gLv-4*a22zm1*a11*a22zm2*a11m1*cv20*a22*cu20*tdu20+
     & a22zm1*a11*a22zm2*a11m1*a21*a12m2*cv10*gLu-a22zm1*a11*a22zm2*
     & a11m1*g1a*cu20*a12m2*cv10-a22zm1*a11*a22zm2*a11m2*a22*cv10*
     & cu10*tdu10-4*a22zm1*a11*a22zm2*a11m2*cv20*a22*cu10*tdu10+4*
     & a22zm1*a11*a22zm2*cv20*a21*a12m1*cu20*tdu20-16*a21zm2*a12*
     & tdu01*a21*a12m1*cu20*a12m2*cv02+16*a21zm2*a12*tdu01*a11m1*a22*
     & cu20*a12m2*cv02+4*a21zm2*a12*a11m1*a22*cu20*a12m2*cv01*tdu01-
     & a21zm2*a12*a21*a12m2*cv01*tdu01*cu10*a12m1-4*a21zm2*a12*a21*
     & a12m1*cu20*a12m2*cv01*tdu01-4*a21zm2*a12*tdu01*a21*a12m2*cv02*
     & cu10*a12m1+a22zm1*a21zm2*a12*a11m1*g1a*cu20*a12m2*cv10+a22zm1*
     & a21zm2*a12*a11m1*a22*cv10*cu20*tdu20+4*a22zm1*a21zm2*a12*a11m1*
     & cv20*a22*cu20*tdu20+a21zm2*a12*a11m2*a22*cv01*tdu01*cu10*a12m1+
     & 4*a21zm2*a12*tdu01*a11m2*a22*cv02*cu10*a12m1-a22zm1*a21zm2*g2a*
     & a11m2*a22*cv02*cu10*a12m1+4*a22zm1*a21zm2*a12*a11m2*a11m1*cv20*
     & a22*gLu-4*a22zm1*a21zm2*g2a*a11m1*a22*cu20*a12m2*cv02-4*a22zm1*
     & a21zm2*a12*a11m2*cv20*a21*a12m1*gLu+a22zm1*a21zm2*a12*a11m2*
     & a11m1*a22*cv10*gLu-a22zm1*a21zm2*a12*a21*a12m2*cv10*cu10*tdu10-
     & a22zm1*a21zm2*a12*a11m2*cv20*g1a*cu10*a12m1-4*a22zm1*a21zm2*
     & a12*a21*a12m1*cu20*a12m2*gLv+4*a22zm1*a21zm2*a12*a11m2*cv20*
     & a22*cu10*tdu10+a22zm1*a21zm2*a12*a11m2*a22*cv10*cu10*tdu10+4*
     & a22zm1*a21zm2*a12*a11m1*a22*cu20*a12m2*gLv-4*a22zm1*a21zm2*a12*
     & a21*tdu10*cu20*a12m2*cv10+a22zm1*a21zm2*a12*a11m2*a22*gLv*cu10*
     & a12m1-a22zm1*a21zm2*a12*cv20*a21*tdu20*cu10*a12m1-4*a22zm1*
     & a21zm2*a12*cv20*a21*a12m1*cu20*tdu20+4*a22zm1*a21zm2*g2a*a21*
     & a12m1*cu20*a12m2*cv02-a22zm1*a21zm2*a12*a21*a12m2*gLv*cu10*
     & a12m1+a22zm1*a21zm2*g2a*a21*a12m2*cv02*cu10*a12m1-a22zm1*
     & a21zm2*a12*a11m1*a21*a12m2*cv10*gLu+a11*a22zm2*a21*a12m2*cv01*
     & tdu01*cu10*a12m1-4*a11*a22zm2*a11m1*a22*cu20*a12m2*cv01*tdu01-
     & 16*a12*tdu01*a11m2*a11m1*cv20*a22*cu02*a22zm2+16*a12*tdu01*
     & a11m2*cv20*a21*a12m1*cu02*a22zm2-4*a12*tdu01*a11m2*a11m1*a22*
     & cv10*cu02*a22zm2+4*a12*tdu01*a11m1*a21*a12m2*cv10*cu02*a22zm2+
     & 4*a11*a22zm2*a21*a12m1*cu20*a12m2*cv01*tdu01-a22zm1*g2a*a11m1*
     & a21*a12m2*cv10*cu02*a22zm2-a11*a22zm2*a11m2*a22*cv01*tdu01*
     & cu10*a12m1+a22zm1*a11*a22zm2*a21*a12m2*cv10*cu10*tdu10+a22zm1*
     & g2a*a11m2*a11m1*a22*cv10*cu02*a22zm2+4*a22zm1*a11*a22zm2*a21*
     & tdu10*cu20*a12m2*cv10-a22zm1*a12*a11m1*a21*a12m2*cv10*cu02*
     & tdu02+a22zm1*a12*a11m2*a11m1*a22*cv10*cu02*tdu02+4*a22zm1*a12*
     & a11m2*a11m1*cv20*a22*cu02*tdu02-a22zm1*a11*tdu02*a11m2*a22*
     & cv02*cu10*a12m1-4*a22zm1*a12*a11m2*cv20*a21*a12m1*cu02*tdu02+4*
     & a22zm1*a11*tdu02*a21*a12m1*cu20*a12m2*cv02-4*a22zm1*a11*tdu02*
     & a11m1*a22*cu20*a12m2*cv02+a22zm1*a11*tdu02*a21*a12m2*cv02*cu10*
     & a12m1+4*a22zm1*g2a*a11m2*a11m1*cv20*a22*cu02*a22zm2)/det

      u(i1,i2-is2,i3,ey)=(-4*a21zm1*a11*a22zm2*a21*a12m1*cu20*a12m2*
     & gLv+a21zm1*g2a*a11m1*a21*a12m2*cv10*cu02*a22zm2+4*tdu01*a21*
     & a12m2*cv02*a21zm2*a11*cu10*a12m1-a21zm1*a21zm2*g2a*a21*a12m2*
     & cv02*cu10*a12m1-a21zm1*a11*tdu02*a21*a12m2*cv02*cu10*a12m1+4*
     & a21zm1*a11*tdu02*a11m1*a22*cu20*a12m2*cv02+a11m1*a21*a12m2*
     & cv10*cu02*tdu02*a12*a21zm1-4*a21zm1*a11*tdu02*a21*a12m1*cu20*
     & a12m2*cv02+a21zm1*a11*tdu02*a11m2*a22*cv02*cu10*a12m1+4*a11m2*
     & cv20*a21*a12m1*cu02*tdu02*a12*a21zm1-a11m2*a11m1*a22*cv10*cu02*
     & tdu02*a12*a21zm1-4*a11m2*a11m1*cv20*a22*cu02*tdu02*a12*a21zm1-
     & 16*tdu01*a11m2*cv20*a21*a12m1*cu02*a22zm2*a11-a21zm1*a11*
     & a22zm2*a21*a12m2*cv10*cu10*tdu10+tdu01*a11m2*a11m1*a22*cv10*
     & cu01*a11*a22zm2-4*tdu01*a11m2*cv20*a21*a12m1*cu01*a11*a22zm2+4*
     & tdu01*a11m2*a11m1*a22*cv10*cu02*a22zm2*a11-4*a21zm1*a11*a22zm2*
     & cv20*a21*a12m1*cu20*tdu20+a21zm1*a11*a22zm2*a11m1*a22*cv10*
     & cu20*tdu20+4*a21zm1*a11*a22zm2*a11m1*cv20*a22*cu20*tdu20-
     & a21zm1*a11*a22zm2*cv20*a21*tdu20*cu10*a12m1-4*a21zm1*a11*
     & a22zm2*a21*tdu10*cu20*a12m2*cv10+16*tdu01*a11m2*a11m1*cv20*a22*
     & cu02*a22zm2*a11+4*tdu01*a11m2*a11m1*cv20*a22*cu01*a11*a22zm2+
     & a21zm1*a11*a22zm2*a11m2*a22*gLv*cu10*a12m1+4*a21zm1*a11*a22zm2*
     & a11m2*a11m1*cv20*a22*gLu-a21zm1*g2a*a11m2*a11m1*a22*cv10*cu02*
     & a22zm2-4*a21zm1*a11*a22zm2*a11m2*cv20*a21*a12m1*gLu-4*a21zm1*
     & g2a*a11m2*a11m1*cv20*a22*cu02*a22zm2+a21zm1*a11*a22zm2*a11m2*
     & a11m1*a22*cv10*gLu+4*a21zm1*g2a*a11m2*cv20*a21*a12m1*cu02*
     & a22zm2+4*a21zm1*a11*a22zm2*a11m1*a22*cu20*a12m2*gLv+a21zm1*a11*
     & a22zm2*a11m1*g1a*cu20*a12m2*cv10+16*tdu01*a21*a12m1*a21zm2*
     & cu20*a12m2*cv02*a11+a11m1*a21*a12m2*cv10*cu01*a21zm2*a12*tdu01-
     & a21zm1*a11*a22zm2*a21*a12m2*gLv*cu10*a12m1-4*tdu01*a11m2*a22*
     & cv02*a21zm2*a11*cu10*a12m1+4*a11m2*cv20*a21*a12m1*cu01*a21zm2*
     & a12*tdu01-16*tdu01*a11m1*a22*a21zm2*cu20*a12m2*cv02*a11+a21zm1*
     & a11*a22zm2*a11m2*a22*cv10*cu10*tdu10-a21zm1*a11*a22zm2*a11m1*
     & a21*a12m2*cv10*gLu+4*a21zm1*a11*a22zm2*a11m2*cv20*a22*cu10*
     & tdu10-a21zm1*a11*a22zm2*a11m2*cv20*g1a*cu10*a12m1+a21*a12m2*
     & gLv*a21zm2*a12*a21zm1*cu10*a12m1-4*a11m2*a11m1*cv20*a22*cu01*
     & a21zm2*a12*tdu01-a11m2*a11m1*a22*cv10*cu01*a21zm2*a12*tdu01-
     & a11m1*a22*cv10*a21zm2*cu20*tdu20*a12*a21zm1+4*a21*tdu10*a21zm2*
     & a12*a21zm1*cu20*a12m2*cv10+a21*a12m2*cv10*a21zm2*cu10*tdu10*
     & a12*a21zm1-a11m2*a11m1*a22*cv10*a21zm2*gLu*a12*a21zm1-4*a11m2*
     & a11m1*cv20*a22*a21zm2*gLu*a12*a21zm1+cv20*a21*tdu20*a21zm2*a12*
     & a21zm1*cu10*a12m1+4*cv20*a21*a12m1*a21zm2*cu20*tdu20*a12*
     & a21zm1-4*a11m1*cv20*a22*a21zm2*cu20*tdu20*a12*a21zm1+4*a21zm1*
     & a21zm2*g2a*a11m1*a22*cu20*a12m2*cv02+4*a11m2*cv20*a21*a12m1*
     & a21zm2*gLu*a12*a21zm1-a11m2*a22*gLv*a21zm2*a12*a21zm1*cu10*
     & a12m1-4*a11m1*a22*a21zm2*cu20*a12m2*gLv*a12*a21zm1-4*a11m2*
     & cv20*a22*a21zm2*cu10*tdu10*a12*a21zm1+a11m2*cv20*g1a*a21zm2*
     & a12*a21zm1*cu10*a12m1+a21zm1*a21zm2*g2a*a11m2*a22*cv02*cu10*
     & a12m1-a11m1*g1a*a21zm2*a12*a21zm1*cu20*a12m2*cv10-a11m2*a22*
     & cv10*a21zm2*cu10*tdu10*a12*a21zm1+4*a21*a12m1*a21zm2*cu20*
     & a12m2*gLv*a12*a21zm1-4*a21zm1*a21zm2*g2a*a21*a12m1*cu20*a12m2*
     & cv02+a11m1*a21*a12m2*cv10*a21zm2*gLu*a12*a21zm1-4*tdu01*a11m1*
     & a21*a12m2*cv10*cu02*a22zm2*a11-tdu01*a11m1*a21*a12m2*cv10*cu01*
     & a11*a22zm2)/det

      u(i1-2*is1,i2,i3,ex)=(-4*a12m2*a11m1*cu02*tdu02*a22*cv01*a21zm1*
     & a12-16*a12m2*a11m1*cu02*tdu02*a12*a21zm1*a22*cv02+16*a12m1*
     & cu02*tdu02*a12*a21zm1*a21*a12m2*cv02+4*a12m1*cu02*tdu02*a21*
     & a12m2*cv01*a21zm1*a12+4*a12m2*a22zm1*a11*a11m1*cu01*tdu02*a22*
     & cv02+16*a12m2*a22zm1*a11*a11m1*cu02*tdu02*a22*cv02-16*a12m1*
     & a22zm1*a11*cu02*tdu02*a21*a12m2*cv02-4*a12m1*a22zm1*a11*cu01*
     & tdu02*a21*a12m2*cv02+4*a12m1*cu01*a21zm2*a12*a21*a12m2*cv01*
     & tdu01+16*a12m1*cu01*a21zm2*a12*tdu01*a21*a12m2*cv02-4*a12m2*
     & a11m1*a21zm2*gLu*a22*cv01*a21zm1*a12-16*a12m2*a11m1*a21zm2*gLu*
     & a12*a21zm1*a22*cv02+4*a12m2*cv02*g1a*a21zm2*a12*a21zm1*cu10*
     & a12m1-4*a12m2*cu10*a21zm2*tdu10*a22*cv01*a21zm1*a12+a12m2*cv01*
     & a21zm1*a21zm2*a12*g1a*cu10*a12m1+16*a12m2*a22zm1*a11*a21zm2*
     & tdu10*a22*cv02*cu10-4*a12m2*a22zm1*a11m1*cu01*a21zm2*a12*a22*
     & gLv-4*a12m2*a22zm1*cv02*g1a*a21zm2*a11*cu10*a12m1-a12m2*a22zm1*
     & cv10*a11m1*cu01*a21zm2*a12*g1a+16*a12m2*a22zm1*a11*a11m1*
     & a21zm2*gLu*a22*cv02-16*tdu10*a21*a12m2*cv10*a22zm1*cu02*a22zm2*
     & a11-4*a12m1*a22zm1*a11*cu01*a22zm2*a21*a12m2*gLv-16*a12m1*
     & a22zm1*a11*cu02*a22zm2*a21*a12m2*gLv+16*tdu20*a11m1*cv20*a22*
     & a22zm1*cu02*a22zm2*a11+16*a12m1*cu02*a22zm2*a12*a21zm1*a21*
     & tdu20*cv20-4*a12m1*a11*gLu*a21*a12m2*cv01*a21zm1*a22zm2-16*
     & a12m2*a11m1*cu02*a22zm2*a12*a21zm1*a22*gLv-4*a12m2*a11m1*cu02*
     & a22zm2*a22*cv01*a21zm1*g2a+4*a12m2*a11*a11m1*gLu*a22*cv01*
     & a21zm1*a22zm2-4*a12m2*cv10*a11m1*cu02*a22zm2*a12*a21zm1*g1a+4*
     & a12m1*cu02*a22zm2*a21*a12m2*cv01*a21zm1*g2a-tdu20*a22*cv01*
     & a21zm1*a11*a22zm2*cu10*a12m1-a12m2*cv01*a21zm1*a11*a22zm2*g1a*
     & cu10*a12m1-16*tdu20*a11m1*cv20*a22*cu02*a22zm2*a12*a21zm1-16*
     & a12m1*a11*cu02*a22zm2*a21*a12m2*cv01*tdu01+4*a12m2*a11*a11m1*
     & cu01*a22zm2*a22*cv01*tdu01-4*tdu20*a22zm1*a22*cv02*a21zm2*a11*
     & cu10*a12m1+4*a12m2*a22zm1*a11m1*cu01*a21zm2*g2a*a22*cv02-16*
     & a12m2*a11m1*cu01*a21zm2*a12*tdu01*a22*cv02-4*a12m2*a11m1*cu01*
     & a21zm2*a12*a22*cv01*tdu01+4*a12m1*a22zm1*cu01*a21zm2*a12*a21*
     & a12m2*gLv-16*a12m1*a22zm1*a11*a21zm2*gLu*a21*a12m2*cv02+4*
     & a12m1*a22zm1*cu01*a21zm2*a12*a21*tdu20*cv20-4*a12m1*a22zm1*
     & cu01*a21zm2*g2a*a21*a12m2*cv02-4*tdu20*a11m1*cv20*a22*a22zm1*
     & cu01*a21zm2*a12-tdu20*a11m1*a22*cv10*a22zm1*cu01*a21zm2*a12+4*
     & tdu10*a21*a12m2*cv10*a22zm1*cu01*a21zm2*a12-4*tdu20*a11m1*a22*
     & cv10*cu02*a22zm2*a12*a21zm1-4*a12m1*a11*cu01*a22zm2*a21*a12m2*
     & cv01*tdu01+16*a12m2*a11*a11m1*cu02*a22zm2*a22*cv01*tdu01+4*
     & a12m2*a11*tdu10*a22*cv01*a21zm1*a22zm2*cu10+16*a12m1*cu02*
     & a22zm2*a12*a21zm1*a21*a12m2*gLv+16*tdu10*a21*a12m2*cv10*cu02*
     & a22zm2*a12*a21zm1-4*tdu10*a21*a12m2*cv10*a22zm1*cu01*a11*
     & a22zm2+16*a12m2*a22zm1*a11*a11m1*cu02*a22zm2*a22*gLv+4*a12m2*
     & a22zm1*cv10*a11*a11m1*cu02*a22zm2*g1a+4*a12m2*a22zm1*a11*a11m1*
     & cu01*a22zm2*a22*gLv+a12m2*a22zm1*cv10*a11*a11m1*cu01*a22zm2*
     & g1a-4*a12m1*a22zm1*a11*cv20*cu01*a22zm2*a21*tdu20-16*a12m1*
     & a22zm1*a11*cv20*cu02*a22zm2*a21*tdu20+4*tdu20*a11m1*cv20*a22*
     & a22zm1*cu01*a11*a22zm2+tdu20*a11m1*a22*cv10*a22zm1*cu01*a11*
     & a22zm2+4*tdu20*a11m1*a22*cv10*a22zm1*cu02*a22zm2*a11-16*a12m2*
     & cu10*a21zm2*tdu10*a12*a21zm1*a22*cv02+tdu20*a22*cv01*a21zm1*
     & a21zm2*a12*cu10*a12m1+16*a12m1*a21zm2*gLu*a12*a21zm1*a21*a12m2*
     & cv02+4*a12m1*a21zm2*gLu*a21*a12m2*cv01*a21zm1*a12+4*tdu20*a22*
     & cv02*a21zm2*a12*a21zm1*cu10*a12m1)/det

      u(i1-2*is1,i2,i3,ey)=(4*a22zm1*cv02*a11m2*a21*a12m1*cu01*a21zm2*
     & g2a+16*a22zm1*cv02*a11m2*a21*a12m1*cu02*tdu02*a11+4*a22zm1*
     & cv02*a11m2*a21*a12m1*cu01*a11*tdu02-16*a22zm1*a11m2*a11*a11m1*
     & cu02*tdu02*a22*cv02-4*cv01*a21zm1*a12*a11m2*a21*a12m1*cu02*
     & tdu02-16*cv02*a11m2*a21*a12m1*cu02*tdu02*a12*a21zm1-4*a22zm1*
     & a11m2*a11*a11m1*cu01*tdu02*a22*cv02+16*a11m2*a11m1*cu02*tdu02*
     & a12*a21zm1*a22*cv02+4*a11m2*a11m1*cu02*tdu02*a22*cv01*a21zm1*
     & a12+16*a11m2*a11m1*a21zm2*gLu*a12*a21zm1*a22*cv02+16*a11m2*
     & cu10*a21zm2*tdu10*a12*a21zm1*a22*cv02-4*cv02*a11m2*g1a*a21zm2*
     & a12*a21zm1*cu10*a12m1+16*a11m1*a21zm2*cu20*tdu20*a12*a21zm1*
     & a22*cv02-4*cv01*a21zm1*a21zm2*a12*a21*a12m1*cu20*tdu20+16*cv01*
     & tdu01*a11m2*a21*a12m1*cu02*a22zm2*a11+4*cv01*tdu01*a11m2*a21*
     & a12m1*cu01*a11*a22zm2-16*a11m2*a11*a11m1*cu02*a22zm2*a22*cv01*
     & tdu01-4*a11m2*a11*a11m1*cu01*a22zm2*a22*cv01*tdu01+4*a22zm1*
     & cv02*a11m2*g1a*a21zm2*a11*cu10*a12m1-4*a22zm1*gLv*a11m2*a21*
     & a12m1*cu01*a21zm2*a12+16*a22zm1*cv02*a11m2*a21*a12m1*a21zm2*
     & gLu*a11-cv01*a21zm1*a21zm2*a12*a21*tdu20*cu10*a12m1+4*a11m1*
     & a21zm2*cu20*tdu20*a22*cv01*a21zm1*a12-4*cv02*a21*tdu20*a21zm2*
     & a12*a21zm1*cu10*a12m1-16*cv02*a21*a12m1*a21zm2*cu20*tdu20*a12*
     & a21zm1+16*a22zm1*cv02*a21*a12m1*a21zm2*cu20*tdu20*a11+4*a11m2*
     & cu10*a21zm2*tdu10*a22*cv01*a21zm1*a12-16*a22zm1*a11*a11m1*
     & a21zm2*cu20*tdu20*a22*cv02+a22zm1*cv10*a11m1*cu01*a21zm2*a12*
     & a21*tdu20+4*a11m2*a11m1*a21zm2*gLu*a22*cv01*a21zm1*a12-16*cv02*
     & a11m2*a21*a12m1*a21zm2*gLu*a12*a21zm1-4*cv01*a21zm1*a21zm2*a12*
     & a11m2*a21*a12m1*gLu-cv01*a21zm1*a21zm2*a12*a11m2*g1a*cu10*
     & a12m1+4*a22zm1*cv02*a21*tdu20*a21zm2*a11*cu10*a12m1-16*a22zm1*
     & a11m2*a11*a21zm2*tdu10*a22*cv02*cu10+16*a11m2*a11m1*cu01*
     & a21zm2*a12*tdu01*a22*cv02+4*a22zm1*a11m2*a11m1*cu01*a21zm2*a12*
     & a22*gLv-16*a22zm1*a11m2*a11*a11m1*a21zm2*gLu*a22*cv02-4*a22zm1*
     & a11m2*a11m1*cu01*a21zm2*g2a*a22*cv02+a22zm1*cv10*a11m2*a11m1*
     & cu01*a21zm2*a12*g1a-4*a22zm1*cv10*a11m2*a21zm2*tdu10*a21*cu01*
     & a12-16*cv02*a11m2*a21*a12m1*cu01*a21zm2*a12*tdu01-4*cv01*tdu01*
     & a11m2*a21*a12m1*cu01*a21zm2*a12+4*a11m2*a11m1*cu01*a21zm2*a12*
     & a22*cv01*tdu01-4*a11m2*a11*tdu10*a22*cv01*a21zm1*a22zm2*cu10+4*
     & cv01*a21zm1*a11*a22zm2*a21*a12m1*cu20*tdu20+4*cv10*a11m1*cu02*
     & a22zm2*a12*a21zm1*a21*tdu20-4*a11*a11m1*cu20*tdu20*a22*cv01*
     & a21zm1*a22zm2+cv01*a21zm1*a11*a22zm2*a21*tdu20*cu10*a12m1-16*
     & a22zm1*a11m2*a11*a11m1*cu02*a22zm2*a22*gLv-4*a22zm1*cv10*a11m2*
     & a11*a11m1*cu02*a22zm2*g1a-a22zm1*cv10*a11m2*a11*a11m1*cu01*
     & a22zm2*g1a+16*a22zm1*cv10*a11m2*a11*tdu10*a21*cu02*a22zm2+4*
     & a22zm1*cv10*a11m2*a11*tdu10*a21*cu01*a22zm2+4*a22zm1*gLv*a11m2*
     & a21*a12m1*cu01*a11*a22zm2+4*cv10*a11m2*a11m1*cu02*a22zm2*a12*
     & a21zm1*g1a+4*a11m2*a11m1*cu02*a22zm2*a22*cv01*a21zm1*g2a-16*
     & cv10*a11m2*tdu10*a12*a21zm1*a21*cu02*a22zm2-4*a22zm1*cv10*a11*
     & a11m1*cu02*a22zm2*a21*tdu20-a22zm1*cv10*a11*a11m1*cu01*a22zm2*
     & a21*tdu20+4*cv01*a21zm1*a11*a22zm2*a11m2*a21*a12m1*gLu+cv01*
     & a21zm1*a11*a22zm2*a11m2*g1a*cu10*a12m1-4*cv01*a21zm1*g2a*a11m2*
     & a21*a12m1*cu02*a22zm2-16*gLv*a11m2*a21*a12m1*cu02*a22zm2*a12*
     & a21zm1-4*a22zm1*a11m2*a11*a11m1*cu01*a22zm2*a22*gLv+16*a22zm1*
     & gLv*a11m2*a21*a12m1*cu02*a22zm2*a11+16*a11m2*a11m1*cu02*a22zm2*
     & a12*a21zm1*a22*gLv-4*a11m2*a11*a11m1*gLu*a22*cv01*a21zm1*
     & a22zm2)/det

      u(i1,i2-2*is2,i3,ex)=(-16*a22zm1*a11*a22zm2*a11m2*cv20*a22*cu10*
     & tdu10-4*a22zm1*a11*a22zm2*a11m2*a22*cv10*cu10*tdu10+a22zm2*
     & a11m1*a21*a12m2*cv10*a22zm1*cu01*g2a-4*a22zm1*a11*a22zm2*a11m2*
     & a11m1*a22*cv10*gLu+4*a22zm2*a11m1*g1a*a12*a21zm1*cu20*a12m2*
     & cv10-4*a22zm2*a11m1*a21*a12m2*cv10*gLu*a12*a21zm1+16*a22zm2*
     & a11m2*a11m1*cv20*a22*cu01*a12*tdu01+4*a22zm2*a11m2*a11m1*a22*
     & cv10*cu01*a12*tdu01-16*a22zm2*a11m2*cv20*a21*a12m1*cu01*a12*
     & tdu01-4*a11*a22zm2*a11m2*a22*cv01*tdu01*cu10*a12m1-16*a11*
     & a22zm2*a11m1*a22*cu20*a12m2*cv01*tdu01+16*a11*a22zm2*a21*a12m1*
     & cu20*a12m2*cv01*tdu01+4*a11*a22zm2*a21*a12m2*cv01*tdu01*cu10*
     & a12m1-4*a22zm2*a11m1*a21*a12m2*cv10*cu01*a12*tdu01-4*a22zm1*
     & a11*a22zm2*a11m1*a22*cv10*cu20*tdu20-16*a22zm2*a11m2*cv20*a21*
     & a12m1*gLu*a12*a21zm1+16*a22zm2*a11m1*a22*cu20*a12m2*gLv*a12*
     & a21zm1-16*a22zm1*a11*a22zm2*a11m2*a11m1*cv20*a22*gLu+4*a22zm1*
     & a11*a22zm2*a11m1*a21*a12m2*cv10*gLu+16*a22zm1*a11*a22zm2*a21*
     & tdu10*cu20*a12m2*cv10+4*a22zm1*a11*a22zm2*a21*a12m2*cv10*cu10*
     & tdu10+4*a22zm1*a11*a22zm2*a21*a12m2*gLv*cu10*a12m1+16*a22zm1*
     & a11*a22zm2*a21*a12m1*cu20*a12m2*gLv-16*a22zm1*a11*a22zm2*a11m1*
     & a22*cu20*a12m2*gLv-4*a22zm1*a11*a22zm2*a11m1*g1a*cu20*a12m2*
     & cv10+16*a22zm1*a11*a22zm2*a11m2*cv20*a21*a12m1*gLu+4*a22zm1*
     & a11*a22zm2*a11m2*cv20*g1a*cu10*a12m1-4*a22zm1*a11*a22zm2*a11m2*
     & a22*gLv*cu10*a12m1-a22zm2*a11m2*a11m1*a22*cv10*a22zm1*cu01*g2a-
     & 4*a22zm2*a11m2*a11m1*cv20*a22*a22zm1*cu01*g2a+4*a22zm2*a11m2*
     & cv20*a21*a12m1*a22zm1*cu01*g2a+4*a22zm2*a11m1*a22*cv10*cu20*
     & tdu20*a12*a21zm1+16*a22zm2*a11m1*cv20*a22*cu20*tdu20*a12*
     & a21zm1-4*a22zm2*cv20*a21*tdu20*a12*a21zm1*cu10*a12m1-16*a22zm2*
     & cv20*a21*a12m1*cu20*tdu20*a12*a21zm1-4*a22zm2*a21*a12m2*gLv*
     & a12*a21zm1*cu10*a12m1-16*a22zm2*a21*tdu10*a12*a21zm1*cu20*
     & a12m2*cv10+4*a22zm2*a11m1*a22*cu20*a12m2*cv01*a21zm1*g2a-4*
     & a22zm2*a21*a12m1*cu20*a12m2*cv01*a21zm1*g2a-16*a22zm2*a21*
     & a12m1*cu20*a12m2*gLv*a12*a21zm1-4*a22zm2*a21*a12m2*cv10*cu10*
     & tdu10*a12*a21zm1+4*a22zm2*a11m2*a22*cv10*cu10*tdu10*a12*a21zm1+
     & 16*a22zm2*a11m2*cv20*a22*cu10*tdu10*a12*a21zm1-a22zm2*a21*
     & a12m2*cv01*a21zm1*g2a*cu10*a12m1+16*a22zm1*a11*a22zm2*cv20*a21*
     & a12m1*cu20*tdu20+4*a22zm1*a11*a22zm2*cv20*a21*tdu20*cu10*a12m1-
     & 16*a22zm1*a11*a22zm2*a11m1*cv20*a22*cu20*tdu20+4*a22zm2*a11m2*
     & a11m1*a22*cv10*gLu*a12*a21zm1-4*a22zm2*a11m2*cv20*g1a*a12*
     & a21zm1*cu10*a12m1+16*a22zm2*a11m2*a11m1*cv20*a22*gLu*a12*
     & a21zm1+a22zm2*a11m2*a22*cv01*a21zm1*g2a*cu10*a12m1+4*a22zm2*
     & a11m2*a22*gLv*a12*a21zm1*cu10*a12m1-4*tdu02*a11m2*a11m1*cv20*
     & a22*a22zm1*cu01*a12+4*tdu02*a11m2*cv20*a21*a12m1*a22zm1*cu01*
     & a12-tdu02*a11m2*a11m1*a22*cv10*a22zm1*cu01*a12-4*a22zm1*a11*
     & tdu02*a11m2*a22*cv02*cu10*a12m1+tdu02*a11m1*a21*a12m2*cv10*
     & a22zm1*cu01*a12-16*a22zm1*a11*tdu02*a11m1*a22*cu20*a12m2*cv02+
     & 4*a22zm1*a11*tdu02*a21*a12m2*cv02*cu10*a12m1+16*a22zm1*a11*
     & tdu02*a21*a12m1*cu20*a12m2*cv02+4*tdu02*a11m2*a22*cv02*a12*
     & a21zm1*cu10*a12m1+tdu02*a11m2*a22*cv01*a21zm1*a12*cu10*a12m1+
     & 16*tdu02*a11m1*a22*cu20*a12m2*cv02*a12*a21zm1+4*tdu02*a11m1*
     & a22*cu20*a12m2*cv01*a21zm1*a12-4*tdu02*a21*a12m2*cv02*a12*
     & a21zm1*cu10*a12m1-tdu02*a21*a12m2*cv01*a21zm1*a12*cu10*a12m1-4*
     & tdu02*a21*a12m1*cu20*a12m2*cv01*a21zm1*a12-16*tdu02*a21*a12m1*
     & cu20*a12m2*cv02*a12*a21zm1)/det

      u(i1,i2-2*is2,i3,ey)=(-16*a11m2*a11m1*cv20*a22*cu01*a21zm2*a12*
     & tdu01+4*a11m2*a22*cv01*tdu01*a21zm2*a11*cu10*a12m1-4*a21*a12m2*
     & cv01*tdu01*a21zm2*a11*cu10*a12m1-a11m1*a21*a12m2*cv10*a22zm1*
     & cu01*a11*tdu02-4*a11m1*a21*a12m2*cv10*a22zm1*cu02*tdu02*a11-16*
     & a11m2*cv20*a21*a12m1*a22zm1*cu02*tdu02*a11+a11m2*a11m1*a22*
     & cv10*a22zm1*cu01*a11*tdu02+16*a11m2*a11m1*cv20*a22*a22zm1*cu02*
     & tdu02*a11+4*a11m2*a11m1*a22*cv10*a22zm1*cu02*tdu02*a11+4*a11m2*
     & a11m1*cv20*a22*a22zm1*cu01*a11*tdu02-4*a11m2*cv20*a21*a12m1*
     & a22zm1*cu01*a11*tdu02-4*a11m1*a22*cu20*a12m2*cv01*a21zm1*a11*
     & tdu02+4*a11m1*a21*a12m2*cv10*cu02*tdu02*a12*a21zm1+a21*a12m2*
     & cv01*a21zm1*a11*tdu02*cu10*a12m1+4*a21*a12m1*cu20*a12m2*cv01*
     & a21zm1*a11*tdu02+16*a11m2*cv20*a21*a12m1*cu02*tdu02*a12*a21zm1-
     & 4*a11m2*a11m1*a22*cv10*cu02*tdu02*a12*a21zm1-a11m2*a22*cv01*
     & a21zm1*a11*tdu02*cu10*a12m1-16*a11m2*a11m1*cv20*a22*cu02*tdu02*
     & a12*a21zm1+4*a21*a12m2*cv10*a21zm2*cu10*tdu10*a12*a21zm1+16*
     & a21*tdu10*a21zm2*a12*a21zm1*cu20*a12m2*cv10-a11m2*a22*cv01*
     & a21zm1*a21zm2*g2a*cu10*a12m1-16*a11m1*a22*a21zm2*cu20*a12m2*
     & gLv*a12*a21zm1+4*a11m1*a21*a12m2*cv10*a21zm2*gLu*a12*a21zm1-4*
     & a11m1*g1a*a21zm2*a12*a21zm1*cu20*a12m2*cv10-4*a11m1*a22*cu20*
     & a12m2*cv01*a21zm1*a21zm2*g2a+16*a21*a12m1*a21zm2*cu20*a12m2*
     & gLv*a12*a21zm1+4*a21*a12m2*gLv*a21zm2*a12*a21zm1*cu10*a12m1+16*
     & cv20*a21*a12m1*a21zm2*cu20*tdu20*a12*a21zm1+a21*a12m2*cv01*
     & a21zm1*a21zm2*g2a*cu10*a12m1+4*a21*a12m1*cu20*a12m2*cv01*
     & a21zm1*a21zm2*g2a-4*a11m1*a22*cv10*a21zm2*cu20*tdu20*a12*
     & a21zm1-16*a11m1*cv20*a22*a21zm2*cu20*tdu20*a12*a21zm1+4*cv20*
     & a21*tdu20*a21zm2*a12*a21zm1*cu10*a12m1+4*a11m2*a22zm1*a22*gLv*
     & a21zm2*a11*cu10*a12m1+a11m2*a11m1*a22*cv10*a22zm1*cu01*a21zm2*
     & g2a+16*a11m2*cv20*a22*a22zm1*a21zm2*cu10*tdu10*a11-4*a11m2*
     & cv20*a22zm1*g1a*a21zm2*a11*cu10*a12m1-4*a11m2*cv20*a21*a12m1*
     & a22zm1*cu01*a21zm2*g2a-4*a21*a12m2*cv10*a22zm1*a21zm2*cu10*
     & tdu10*a11-16*a22zm1*a21*tdu10*a21zm2*a11*cu20*a12m2*cv10+16*
     & a11m2*a11m1*cv20*a22*a22zm1*a21zm2*gLu*a11-4*a22zm1*a21*a12m2*
     & gLv*a21zm2*a11*cu10*a12m1+4*a11m2*a11m1*cv20*a22*a22zm1*cu01*
     & a21zm2*g2a+4*a11m2*a22*cv10*a22zm1*a21zm2*cu10*tdu10*a11+4*
     & a11m2*a11m1*a22*cv10*a22zm1*a21zm2*gLu*a11-a11m1*a21*a12m2*
     & cv10*a22zm1*cu01*a21zm2*g2a+4*a11m1*a22zm1*g1a*a21zm2*a11*cu20*
     & a12m2*cv10-4*a11m1*a21*a12m2*cv10*a22zm1*a21zm2*gLu*a11-16*
     & a11m2*cv20*a22*a21zm2*cu10*tdu10*a12*a21zm1+16*a11m2*cv20*a21*
     & a12m1*a21zm2*gLu*a12*a21zm1+4*a11m1*a22*cv10*a22zm1*a21zm2*
     & cu20*tdu20*a11+16*a11m1*cv20*a22*a22zm1*a21zm2*cu20*tdu20*a11-
     & 4*cv20*a22zm1*a21*tdu20*a21zm2*a11*cu10*a12m1+4*a11m2*cv20*g1a*
     & a21zm2*a12*a21zm1*cu10*a12m1-16*cv20*a21*a12m1*a22zm1*a21zm2*
     & cu20*tdu20*a11+16*a11m1*a22*a22zm1*a21zm2*cu20*a12m2*gLv*a11-
     & 16*a21*a12m1*a22zm1*a21zm2*cu20*a12m2*gLv*a11-16*a11m2*cv20*
     & a21*a12m1*a22zm1*a21zm2*gLu*a11-16*a21*a12m1*a21zm2*cu20*a12m2*
     & cv01*tdu01*a11+16*a11m1*a22*a21zm2*cu20*a12m2*cv01*tdu01*a11+4*
     & a11m1*a21*a12m2*cv10*cu01*a21zm2*a12*tdu01-4*a11m2*a11m1*a22*
     & cv10*a21zm2*gLu*a12*a21zm1-16*a11m2*a11m1*cv20*a22*a21zm2*gLu*
     & a12*a21zm1-4*a11m2*a22*cv10*a21zm2*cu10*tdu10*a12*a21zm1-4*
     & a11m2*a22*gLv*a21zm2*a12*a21zm1*cu10*a12m1+16*a11m2*cv20*a21*
     & a12m1*cu01*a21zm2*a12*tdu01-4*a11m2*a11m1*a22*cv10*cu01*a21zm2*
     & a12*tdu01)/det



! ****************** done fourth-order ********************
                      ! The next file is from bc4c.maple
! ****************** Start Hz extended fourth-order ********************
!  Solve:  wr=fw1  
!          ws=fw2  
!          c11*wrrr+(c1+c11r)*wrr + c22r*wss=fw3, (i.e. (Lw).r=0 )
!          c22*wsss+(c2+c22s)*wss + c11s*wrr=fw4, (i.e. (Lw).s=0 )

      u(i1,i2-2*is2,i3,hz) = (12*fw2*dsa**2*dra**3*c11r*c2+12*fw2*dsa**
     & 2*dra**3*c11r*c22s+12*fw2*dsa**2*dra**3*c1*c2+12*fw2*dsa**2*
     & dra**3*c1*c22s-12*fw2*dsa**2*dra**3*c11s*c22r-36*fw2*dsa**2*
     & dra**2*c11*c22s-36*fw2*dsa**2*dra**2*c11*c2-3*u(i1,i2+2*is2,i3,
     & hz)*dra**2*c11*dsa*c22s+9*u(i1,i2+2*is2,i3,hz)*dra**2*c11*c22-
     & 3*u(i1,i2+2*is2,i3,hz)*dra**3*c11r*c22-3*u(i1,i2+2*is2,i3,hz)*
     & dra**3*c1*c22+u(i1,i2+2*is2,i3,hz)*dra**3*c11r*dsa*c2+u(i1,i2+
     & 2*is2,i3,hz)*dra**3*c11r*dsa*c22s+u(i1,i2+2*is2,i3,hz)*dra**3*
     & c1*dsa*c2+u(i1,i2+2*is2,i3,hz)*dra**3*c1*dsa*c22s-u(i1,i2+2*
     & is2,i3,hz)*dra**3*c11s*dsa*c22r-3*u(i1,i2+2*is2,i3,hz)*dra**2*
     & c11*dsa*c2-48*c11s*dsa**3*c11*fw1*dra-8*c11s*dsa**3*fw3*dra**3-
     & 36*c11*c22*dra**2*fw2*dsa-24*c11*fw4*dsa**3*dra**2+12*dra**3*
     & c11r*c22*fw2*dsa+12*dra**3*c1*c22*fw2*dsa+8*dra**3*c11r*fw4*
     & dsa**3+8*dra**3*c1*fw4*dsa**3-48*c11*dsa*dra**2*c2*u(i1,i2,i3,
     & hz)-48*c11*dsa*dra**2*c22s*u(i1,i2,i3,hz)+16*dra**3*c11r*dsa*
     & c2*u(i1,i2,i3,hz)+16*dra**3*c11r*dsa*c22s*u(i1,i2,i3,hz)+16*
     & dra**3*c1*dsa*c22s*u(i1,i2,i3,hz)+16*dra**3*c1*dsa*c2*u(i1,i2,
     & i3,hz)-16*c11s*dsa*c22r*dra**3*u(i1,i2,i3,hz)-48*c11*c11s*dsa**
     & 3*u(i1,i2,i3,hz)+48*c11s*dsa**3*c11*u(i1+is1,i2,i3,hz)-16*dra**
     & 3*c1*dsa*c2*u(i1,i2+is2,i3,hz)+16*c11s*dsa*c22r*dra**3*u(i1,i2+
     & is2,i3,hz)-16*dra**3*c1*dsa*c22s*u(i1,i2+is2,i3,hz)-16*dra**3*
     & c11r*dsa*c2*u(i1,i2+is2,i3,hz)-16*dra**3*c11r*dsa*c22s*u(i1,i2+
     & is2,i3,hz)+48*c11*dsa*dra**2*c2*u(i1,i2+is2,i3,hz)+48*c11*dsa*
     & dra**2*c22s*u(i1,i2+is2,i3,hz))/dra**2/(-3*c11*dsa*c22s-3*c11*
     & dsa*c2+dra*c11r*dsa*c2+dra*c11r*dsa*c22s+9*c11*c22-3*dra*c11r*
     & c22+dra*c1*dsa*c2+dra*c1*dsa*c22s-c11s*dsa*c22r*dra-3*dra*c1*
     & c22)

      u(i1-is1,i2,i3,hz) = -1/dsa**2*(6*dsa**2*u(i1,i2,i3,hz)*dra*c1*
     & c22-2*dsa**3*u(i1,i2,i3,hz)*dra*c11r*c2-2*dsa**3*u(i1,i2,i3,hz)
     & *dra*c11r*c22s+2*c11s*dsa**3*u(i1,i2,i3,hz)*c22r*dra+6*dsa**2*
     & u(i1,i2,i3,hz)*dra*c11r*c22-2*dsa**3*u(i1,i2,i3,hz)*dra*c1*c2-
     & 2*dsa**3*u(i1,i2,i3,hz)*dra*c1*c22s+6*c22*c22r*dra**3*u(i1,i2,
     & i3,hz)+3*dsa**3*c22s*c11*u(i1+is1,i2,i3,hz)-3*dsa**2*u(i1+is1,
     & i2,i3,hz)*dra*c11r*c22-3*dsa**2*u(i1+is1,i2,i3,hz)*dra*c1*c22-
     & 9*c22*dsa**2*c11*u(i1+is1,i2,i3,hz)+dsa**3*u(i1+is1,i2,i3,hz)*
     & dra*c11r*c22s-c11s*dsa**3*u(i1+is1,i2,i3,hz)*c22r*dra+dsa**3*u(
     & i1+is1,i2,i3,hz)*dra*c1*c2+dsa**3*u(i1+is1,i2,i3,hz)*dra*c1*
     & c22s+dsa**3*u(i1+is1,i2,i3,hz)*dra*c11r*c2+3*c2*dsa**3*c11*u(
     & i1+is1,i2,i3,hz)-6*c22*c22r*dra**3*u(i1,i2+is2,i3,hz)-dsa**3*
     & c22s*fw3*dra**3+18*c22*dsa**2*c11*fw1*dra+3*c22*dsa**2*fw3*dra*
     & *3-6*dsa**3*c22s*c11*fw1*dra+fw4*dsa**3*dra**3*c22r-6*c2*dsa**
     & 3*c11*fw1*dra-c2*dsa**3*fw3*dra**3+6*c22*dra**3*fw2*dsa*c22r)/(
     & -3*c11*dsa*c22s-3*c11*dsa*c2+dra*c11r*dsa*c2+dra*c11r*dsa*c22s+
     & 9*c11*c22-3*dra*c11r*c22+dra*c1*dsa*c2+dra*c1*dsa*c22s-c11s*
     & dsa*c22r*dra-3*dra*c1*c22)

      u(i1,i2-is2,i3,hz) = -(6*c11*c11s*dsa**3*u(i1,i2,i3,hz)-2*dra**3*
     & c1*dsa*c22s*u(i1,i2,i3,hz)-2*dra**3*c1*dsa*c2*u(i1,i2,i3,hz)+2*
     & c11s*dsa*c22r*dra**3*u(i1,i2,i3,hz)-2*dra**3*c11r*dsa*c22s*u(
     & i1,i2,i3,hz)+6*c11*dsa*dra**2*c2*u(i1,i2,i3,hz)+6*c11*dsa*dra**
     & 2*c22s*u(i1,i2,i3,hz)-2*dra**3*c11r*dsa*c2*u(i1,i2,i3,hz)-6*
     & c11s*dsa**3*c11*u(i1+is1,i2,i3,hz)-c11s*dsa*c22r*dra**3*u(i1,
     & i2+is2,i3,hz)+dra**3*c1*dsa*c22s*u(i1,i2+is2,i3,hz)+dra**3*
     & c11r*dsa*c2*u(i1,i2+is2,i3,hz)+dra**3*c11r*dsa*c22s*u(i1,i2+
     & is2,i3,hz)+dra**3*c1*dsa*c2*u(i1,i2+is2,i3,hz)-3*c11*dsa*dra**
     & 2*c2*u(i1,i2+is2,i3,hz)-3*c11*dsa*dra**2*c22s*u(i1,i2+is2,i3,
     & hz)+3*dra**3*c11r*c22*u(i1,i2+is2,i3,hz)-9*c11*c22*dra**2*u(i1,
     & i2+is2,i3,hz)+3*dra**3*c1*c22*u(i1,i2+is2,i3,hz)+6*c11s*dsa**3*
     & c11*fw1*dra+c11s*dsa**3*fw3*dra**3+18*c11*c22*dra**2*fw2*dsa+3*
     & c11*fw4*dsa**3*dra**2-6*dra**3*c11r*c22*fw2*dsa-6*dra**3*c1*
     & c22*fw2*dsa-dra**3*c11r*fw4*dsa**3-dra**3*c1*fw4*dsa**3)/dra**
     & 2/(-3*c11*dsa*c22s-3*c11*dsa*c2+dra*c11r*dsa*c2+dra*c11r*dsa*
     & c22s+9*c11*c22-3*dra*c11r*c22+dra*c1*dsa*c2+dra*c1*dsa*c22s-
     & c11s*dsa*c22r*dra-3*dra*c1*c22)

      u(i1-2*is1,i2,i3,hz) = (-36*fw1*dra**2*dsa**2*c11r*c22+12*fw1*
     & dra**2*dsa**3*c1*c2+12*fw1*dra**2*dsa**3*c1*c22s-12*fw1*dra**2*
     & dsa**3*c11s*c22r-36*fw1*dra**2*dsa**2*c1*c22+12*fw1*dra**2*dsa*
     & *3*c11r*c2+12*fw1*dra**2*dsa**3*c11r*c22s+48*dsa**2*u(i1+is1,
     & i2,i3,hz)*dra*c11r*c22+48*dsa**2*u(i1+is1,i2,i3,hz)*dra*c1*c22-
     & 48*c22*c22r*dra**3*u(i1,i2,i3,hz)+16*dsa**3*u(i1,i2,i3,hz)*dra*
     & c1*c22s-16*c11s*dsa**3*u(i1,i2,i3,hz)*c22r*dra-48*dsa**2*u(i1,
     & i2,i3,hz)*dra*c1*c22+16*dsa**3*u(i1,i2,i3,hz)*dra*c11r*c2+16*
     & dsa**3*u(i1,i2,i3,hz)*dra*c11r*c22s-48*dsa**2*u(i1,i2,i3,hz)*
     & dra*c11r*c22+16*dsa**3*u(i1,i2,i3,hz)*dra*c1*c2-16*dsa**3*u(i1+
     & is1,i2,i3,hz)*dra*c1*c2-16*dsa**3*u(i1+is1,i2,i3,hz)*dra*c1*
     & c22s-16*dsa**3*u(i1+is1,i2,i3,hz)*dra*c11r*c2-16*dsa**3*u(i1+
     & is1,i2,i3,hz)*dra*c11r*c22s+16*c11s*dsa**3*u(i1+is1,i2,i3,hz)*
     & c22r*dra+48*c22*c22r*dra**3*u(i1,i2+is2,i3,hz)+u(i1+2*is1,i2,
     & i3,hz)*dsa**3*dra*c11r*c2-3*u(i1+2*is1,i2,i3,hz)*dsa**2*dra*c1*
     & c22+u(i1+2*is1,i2,i3,hz)*dsa**3*dra*c11r*c22s-3*u(i1+2*is1,i2,
     & i3,hz)*dsa**2*dra*c11r*c22+u(i1+2*is1,i2,i3,hz)*dsa**3*dra*c1*
     & c2+u(i1+2*is1,i2,i3,hz)*dsa**3*dra*c1*c22s-u(i1+2*is1,i2,i3,hz)
     & *dsa**3*c11s*c22r*dra-3*u(i1+2*is1,i2,i3,hz)*dsa**3*c11*c2+9*u(
     & i1+2*is1,i2,i3,hz)*dsa**2*c11*c22-3*u(i1+2*is1,i2,i3,hz)*dsa**
     & 3*c11*c22s+8*dsa**3*c22s*fw3*dra**3-36*c22*dsa**2*c11*fw1*dra-
     & 24*c22*dsa**2*fw3*dra**3+12*dsa**3*c22s*c11*fw1*dra-8*fw4*dsa**
     & 3*dra**3*c22r+12*c2*dsa**3*c11*fw1*dra+8*c2*dsa**3*fw3*dra**3-
     & 48*c22*dra**3*fw2*dsa*c22r)/dsa**2/(-3*c11*dsa*c22s-3*c11*dsa*
     & c2+dra*c11r*dsa*c2+dra*c11r*dsa*c22s+9*c11*c22-3*dra*c11r*c22+
     & dra*c1*dsa*c2+dra*c1*dsa*c22s-c11s*dsa*c22r*dra-3*dra*c1*c22)



! ****************** done Hz extended fourth-order ********************
                    ! dra=dr(0)  ! ** reset *** is this correct?
                    ! dsa=dr(1)
                     axis=0
                     axisp1=1
                      ! evaluate non-mixed derivatives at the corner
                        ! ***** finish this *****
                        ur=ur4(i1,i2,i3,ex)
                        vr=ur4(i1,i2,i3,ey)
                        us=us4(i1,i2,i3,ex)
                        vs=us4(i1,i2,i3,ey)
                        urr=urr4(i1,i2,i3,ex)
                        vrr=urr4(i1,i2,i3,ey)
                        uss=uss4(i1,i2,i3,ex)
                        vss=uss4(i1,i2,i3,ey)
                        jac=1./(rx(i1,i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)*
     & sx(i1,i2,i3))
                        a11 =rsxy(i1,i2,i3,0,0)*jac
                        a12 =rsxy(i1,i2,i3,0,1)*jac
                        a21 =rsxy(i1,i2,i3,1,0)*jac
                        a22 =rsxy(i1,i2,i3,1,1)*jac
                        a11r = (8.*((rsxy(i1+1,i2,i3,axis,0)/(rx(i1+1,
     & i2,i3)*sy(i1+1,i2,i3)-ry(i1+1,i2,i3)*sx(i1+1,i2,i3)))-(rsxy(i1-
     & 1,i2,i3,axis,0)/(rx(i1-1,i2,i3)*sy(i1-1,i2,i3)-ry(i1-1,i2,i3)*
     & sx(i1-1,i2,i3))))-((rsxy(i1+2,i2,i3,axis,0)/(rx(i1+2,i2,i3)*sy(
     & i1+2,i2,i3)-ry(i1+2,i2,i3)*sx(i1+2,i2,i3)))-(rsxy(i1-2,i2,i3,
     & axis,0)/(rx(i1-2,i2,i3)*sy(i1-2,i2,i3)-ry(i1-2,i2,i3)*sx(i1-2,
     & i2,i3)))))/(12.*dr(0))
                        a12r = (8.*((rsxy(i1+1,i2,i3,axis,1)/(rx(i1+1,
     & i2,i3)*sy(i1+1,i2,i3)-ry(i1+1,i2,i3)*sx(i1+1,i2,i3)))-(rsxy(i1-
     & 1,i2,i3,axis,1)/(rx(i1-1,i2,i3)*sy(i1-1,i2,i3)-ry(i1-1,i2,i3)*
     & sx(i1-1,i2,i3))))-((rsxy(i1+2,i2,i3,axis,1)/(rx(i1+2,i2,i3)*sy(
     & i1+2,i2,i3)-ry(i1+2,i2,i3)*sx(i1+2,i2,i3)))-(rsxy(i1-2,i2,i3,
     & axis,1)/(rx(i1-2,i2,i3)*sy(i1-2,i2,i3)-ry(i1-2,i2,i3)*sx(i1-2,
     & i2,i3)))))/(12.*dr(0))
                        a21r = (8.*((rsxy(i1+1,i2,i3,axisp1,0)/(rx(i1+
     & 1,i2,i3)*sy(i1+1,i2,i3)-ry(i1+1,i2,i3)*sx(i1+1,i2,i3)))-(rsxy(
     & i1-1,i2,i3,axisp1,0)/(rx(i1-1,i2,i3)*sy(i1-1,i2,i3)-ry(i1-1,i2,
     & i3)*sx(i1-1,i2,i3))))-((rsxy(i1+2,i2,i3,axisp1,0)/(rx(i1+2,i2,
     & i3)*sy(i1+2,i2,i3)-ry(i1+2,i2,i3)*sx(i1+2,i2,i3)))-(rsxy(i1-2,
     & i2,i3,axisp1,0)/(rx(i1-2,i2,i3)*sy(i1-2,i2,i3)-ry(i1-2,i2,i3)*
     & sx(i1-2,i2,i3)))))/(12.*dr(0))
                        a22r = (8.*((rsxy(i1+1,i2,i3,axisp1,1)/(rx(i1+
     & 1,i2,i3)*sy(i1+1,i2,i3)-ry(i1+1,i2,i3)*sx(i1+1,i2,i3)))-(rsxy(
     & i1-1,i2,i3,axisp1,1)/(rx(i1-1,i2,i3)*sy(i1-1,i2,i3)-ry(i1-1,i2,
     & i3)*sx(i1-1,i2,i3))))-((rsxy(i1+2,i2,i3,axisp1,1)/(rx(i1+2,i2,
     & i3)*sy(i1+2,i2,i3)-ry(i1+2,i2,i3)*sx(i1+2,i2,i3)))-(rsxy(i1-2,
     & i2,i3,axisp1,1)/(rx(i1-2,i2,i3)*sy(i1-2,i2,i3)-ry(i1-2,i2,i3)*
     & sx(i1-2,i2,i3)))))/(12.*dr(0))
                        a11s = (8.*((rsxy(i1,i2+1,i3,axis,0)/(rx(i1,i2+
     & 1,i3)*sy(i1,i2+1,i3)-ry(i1,i2+1,i3)*sx(i1,i2+1,i3)))-(rsxy(i1,
     & i2-1,i3,axis,0)/(rx(i1,i2-1,i3)*sy(i1,i2-1,i3)-ry(i1,i2-1,i3)*
     & sx(i1,i2-1,i3))))-((rsxy(i1,i2+2,i3,axis,0)/(rx(i1,i2+2,i3)*sy(
     & i1,i2+2,i3)-ry(i1,i2+2,i3)*sx(i1,i2+2,i3)))-(rsxy(i1,i2-2,i3,
     & axis,0)/(rx(i1,i2-2,i3)*sy(i1,i2-2,i3)-ry(i1,i2-2,i3)*sx(i1,i2-
     & 2,i3)))))/(12.*dr(1))
                        a12s = (8.*((rsxy(i1,i2+1,i3,axis,1)/(rx(i1,i2+
     & 1,i3)*sy(i1,i2+1,i3)-ry(i1,i2+1,i3)*sx(i1,i2+1,i3)))-(rsxy(i1,
     & i2-1,i3,axis,1)/(rx(i1,i2-1,i3)*sy(i1,i2-1,i3)-ry(i1,i2-1,i3)*
     & sx(i1,i2-1,i3))))-((rsxy(i1,i2+2,i3,axis,1)/(rx(i1,i2+2,i3)*sy(
     & i1,i2+2,i3)-ry(i1,i2+2,i3)*sx(i1,i2+2,i3)))-(rsxy(i1,i2-2,i3,
     & axis,1)/(rx(i1,i2-2,i3)*sy(i1,i2-2,i3)-ry(i1,i2-2,i3)*sx(i1,i2-
     & 2,i3)))))/(12.*dr(1))
                        a21s = (8.*((rsxy(i1,i2+1,i3,axisp1,0)/(rx(i1,
     & i2+1,i3)*sy(i1,i2+1,i3)-ry(i1,i2+1,i3)*sx(i1,i2+1,i3)))-(rsxy(
     & i1,i2-1,i3,axisp1,0)/(rx(i1,i2-1,i3)*sy(i1,i2-1,i3)-ry(i1,i2-1,
     & i3)*sx(i1,i2-1,i3))))-((rsxy(i1,i2+2,i3,axisp1,0)/(rx(i1,i2+2,
     & i3)*sy(i1,i2+2,i3)-ry(i1,i2+2,i3)*sx(i1,i2+2,i3)))-(rsxy(i1,i2-
     & 2,i3,axisp1,0)/(rx(i1,i2-2,i3)*sy(i1,i2-2,i3)-ry(i1,i2-2,i3)*
     & sx(i1,i2-2,i3)))))/(12.*dr(1))
                        a22s = (8.*((rsxy(i1,i2+1,i3,axisp1,1)/(rx(i1,
     & i2+1,i3)*sy(i1,i2+1,i3)-ry(i1,i2+1,i3)*sx(i1,i2+1,i3)))-(rsxy(
     & i1,i2-1,i3,axisp1,1)/(rx(i1,i2-1,i3)*sy(i1,i2-1,i3)-ry(i1,i2-1,
     & i3)*sx(i1,i2-1,i3))))-((rsxy(i1,i2+2,i3,axisp1,1)/(rx(i1,i2+2,
     & i3)*sy(i1,i2+2,i3)-ry(i1,i2+2,i3)*sx(i1,i2+2,i3)))-(rsxy(i1,i2-
     & 2,i3,axisp1,1)/(rx(i1,i2-2,i3)*sy(i1,i2-2,i3)-ry(i1,i2-2,i3)*
     & sx(i1,i2-2,i3)))))/(12.*dr(1))
                        a11rs = (8.*((8.*((rsxy(i1+1,i2+1,i3,axis,0)/(
     & rx(i1+1,i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+
     & 1,i3)))-(rsxy(i1-1,i2+1,i3,axis,0)/(rx(i1-1,i2+1,i3)*sy(i1-1,
     & i2+1,i3)-ry(i1-1,i2+1,i3)*sx(i1-1,i2+1,i3))))-((rsxy(i1+2,i2+1,
     & i3,axis,0)/(rx(i1+2,i2+1,i3)*sy(i1+2,i2+1,i3)-ry(i1+2,i2+1,i3)*
     & sx(i1+2,i2+1,i3)))-(rsxy(i1-2,i2+1,i3,axis,0)/(rx(i1-2,i2+1,i3)
     & *sy(i1-2,i2+1,i3)-ry(i1-2,i2+1,i3)*sx(i1-2,i2+1,i3)))))/(12.*
     & dr(0))-(8.*((rsxy(i1+1,i2-1,i3,axis,0)/(rx(i1+1,i2-1,i3)*sy(i1+
     & 1,i2-1,i3)-ry(i1+1,i2-1,i3)*sx(i1+1,i2-1,i3)))-(rsxy(i1-1,i2-1,
     & i3,axis,0)/(rx(i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-ry(i1-1,i2-1,i3)*
     & sx(i1-1,i2-1,i3))))-((rsxy(i1+2,i2-1,i3,axis,0)/(rx(i1+2,i2-1,
     & i3)*sy(i1+2,i2-1,i3)-ry(i1+2,i2-1,i3)*sx(i1+2,i2-1,i3)))-(rsxy(
     & i1-2,i2-1,i3,axis,0)/(rx(i1-2,i2-1,i3)*sy(i1-2,i2-1,i3)-ry(i1-
     & 2,i2-1,i3)*sx(i1-2,i2-1,i3)))))/(12.*dr(0)))-((8.*((rsxy(i1+1,
     & i2+2,i3,axis,0)/(rx(i1+1,i2+2,i3)*sy(i1+1,i2+2,i3)-ry(i1+1,i2+
     & 2,i3)*sx(i1+1,i2+2,i3)))-(rsxy(i1-1,i2+2,i3,axis,0)/(rx(i1-1,
     & i2+2,i3)*sy(i1-1,i2+2,i3)-ry(i1-1,i2+2,i3)*sx(i1-1,i2+2,i3))))-
     & ((rsxy(i1+2,i2+2,i3,axis,0)/(rx(i1+2,i2+2,i3)*sy(i1+2,i2+2,i3)-
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
                        a12rs = (8.*((8.*((rsxy(i1+1,i2+1,i3,axis,1)/(
     & rx(i1+1,i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+
     & 1,i3)))-(rsxy(i1-1,i2+1,i3,axis,1)/(rx(i1-1,i2+1,i3)*sy(i1-1,
     & i2+1,i3)-ry(i1-1,i2+1,i3)*sx(i1-1,i2+1,i3))))-((rsxy(i1+2,i2+1,
     & i3,axis,1)/(rx(i1+2,i2+1,i3)*sy(i1+2,i2+1,i3)-ry(i1+2,i2+1,i3)*
     & sx(i1+2,i2+1,i3)))-(rsxy(i1-2,i2+1,i3,axis,1)/(rx(i1-2,i2+1,i3)
     & *sy(i1-2,i2+1,i3)-ry(i1-2,i2+1,i3)*sx(i1-2,i2+1,i3)))))/(12.*
     & dr(0))-(8.*((rsxy(i1+1,i2-1,i3,axis,1)/(rx(i1+1,i2-1,i3)*sy(i1+
     & 1,i2-1,i3)-ry(i1+1,i2-1,i3)*sx(i1+1,i2-1,i3)))-(rsxy(i1-1,i2-1,
     & i3,axis,1)/(rx(i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-ry(i1-1,i2-1,i3)*
     & sx(i1-1,i2-1,i3))))-((rsxy(i1+2,i2-1,i3,axis,1)/(rx(i1+2,i2-1,
     & i3)*sy(i1+2,i2-1,i3)-ry(i1+2,i2-1,i3)*sx(i1+2,i2-1,i3)))-(rsxy(
     & i1-2,i2-1,i3,axis,1)/(rx(i1-2,i2-1,i3)*sy(i1-2,i2-1,i3)-ry(i1-
     & 2,i2-1,i3)*sx(i1-2,i2-1,i3)))))/(12.*dr(0)))-((8.*((rsxy(i1+1,
     & i2+2,i3,axis,1)/(rx(i1+1,i2+2,i3)*sy(i1+1,i2+2,i3)-ry(i1+1,i2+
     & 2,i3)*sx(i1+1,i2+2,i3)))-(rsxy(i1-1,i2+2,i3,axis,1)/(rx(i1-1,
     & i2+2,i3)*sy(i1-1,i2+2,i3)-ry(i1-1,i2+2,i3)*sx(i1-1,i2+2,i3))))-
     & ((rsxy(i1+2,i2+2,i3,axis,1)/(rx(i1+2,i2+2,i3)*sy(i1+2,i2+2,i3)-
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
                        a21rs = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp1,0)
     & /(rx(i1+1,i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,
     & i2+1,i3)))-(rsxy(i1-1,i2+1,i3,axisp1,0)/(rx(i1-1,i2+1,i3)*sy(
     & i1-1,i2+1,i3)-ry(i1-1,i2+1,i3)*sx(i1-1,i2+1,i3))))-((rsxy(i1+2,
     & i2+1,i3,axisp1,0)/(rx(i1+2,i2+1,i3)*sy(i1+2,i2+1,i3)-ry(i1+2,
     & i2+1,i3)*sx(i1+2,i2+1,i3)))-(rsxy(i1-2,i2+1,i3,axisp1,0)/(rx(
     & i1-2,i2+1,i3)*sy(i1-2,i2+1,i3)-ry(i1-2,i2+1,i3)*sx(i1-2,i2+1,
     & i3)))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-1,i3,axisp1,0)/(rx(i1+1,
     & i2-1,i3)*sy(i1+1,i2-1,i3)-ry(i1+1,i2-1,i3)*sx(i1+1,i2-1,i3)))-(
     & rsxy(i1-1,i2-1,i3,axisp1,0)/(rx(i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-
     & ry(i1-1,i2-1,i3)*sx(i1-1,i2-1,i3))))-((rsxy(i1+2,i2-1,i3,
     & axisp1,0)/(rx(i1+2,i2-1,i3)*sy(i1+2,i2-1,i3)-ry(i1+2,i2-1,i3)*
     & sx(i1+2,i2-1,i3)))-(rsxy(i1-2,i2-1,i3,axisp1,0)/(rx(i1-2,i2-1,
     & i3)*sy(i1-2,i2-1,i3)-ry(i1-2,i2-1,i3)*sx(i1-2,i2-1,i3)))))/(
     & 12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp1,0)/(rx(i1+1,i2+2,
     & i3)*sy(i1+1,i2+2,i3)-ry(i1+1,i2+2,i3)*sx(i1+1,i2+2,i3)))-(rsxy(
     & i1-1,i2+2,i3,axisp1,0)/(rx(i1-1,i2+2,i3)*sy(i1-1,i2+2,i3)-ry(
     & i1-1,i2+2,i3)*sx(i1-1,i2+2,i3))))-((rsxy(i1+2,i2+2,i3,axisp1,0)
     & /(rx(i1+2,i2+2,i3)*sy(i1+2,i2+2,i3)-ry(i1+2,i2+2,i3)*sx(i1+2,
     & i2+2,i3)))-(rsxy(i1-2,i2+2,i3,axisp1,0)/(rx(i1-2,i2+2,i3)*sy(
     & i1-2,i2+2,i3)-ry(i1-2,i2+2,i3)*sx(i1-2,i2+2,i3)))))/(12.*dr(0))
     & -(8.*((rsxy(i1+1,i2-2,i3,axisp1,0)/(rx(i1+1,i2-2,i3)*sy(i1+1,
     & i2-2,i3)-ry(i1+1,i2-2,i3)*sx(i1+1,i2-2,i3)))-(rsxy(i1-1,i2-2,
     & i3,axisp1,0)/(rx(i1-1,i2-2,i3)*sy(i1-1,i2-2,i3)-ry(i1-1,i2-2,
     & i3)*sx(i1-1,i2-2,i3))))-((rsxy(i1+2,i2-2,i3,axisp1,0)/(rx(i1+2,
     & i2-2,i3)*sy(i1+2,i2-2,i3)-ry(i1+2,i2-2,i3)*sx(i1+2,i2-2,i3)))-(
     & rsxy(i1-2,i2-2,i3,axisp1,0)/(rx(i1-2,i2-2,i3)*sy(i1-2,i2-2,i3)-
     & ry(i1-2,i2-2,i3)*sx(i1-2,i2-2,i3)))))/(12.*dr(0))))/(12.*dr(1))
                        a22rs = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp1,1)
     & /(rx(i1+1,i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,
     & i2+1,i3)))-(rsxy(i1-1,i2+1,i3,axisp1,1)/(rx(i1-1,i2+1,i3)*sy(
     & i1-1,i2+1,i3)-ry(i1-1,i2+1,i3)*sx(i1-1,i2+1,i3))))-((rsxy(i1+2,
     & i2+1,i3,axisp1,1)/(rx(i1+2,i2+1,i3)*sy(i1+2,i2+1,i3)-ry(i1+2,
     & i2+1,i3)*sx(i1+2,i2+1,i3)))-(rsxy(i1-2,i2+1,i3,axisp1,1)/(rx(
     & i1-2,i2+1,i3)*sy(i1-2,i2+1,i3)-ry(i1-2,i2+1,i3)*sx(i1-2,i2+1,
     & i3)))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-1,i3,axisp1,1)/(rx(i1+1,
     & i2-1,i3)*sy(i1+1,i2-1,i3)-ry(i1+1,i2-1,i3)*sx(i1+1,i2-1,i3)))-(
     & rsxy(i1-1,i2-1,i3,axisp1,1)/(rx(i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-
     & ry(i1-1,i2-1,i3)*sx(i1-1,i2-1,i3))))-((rsxy(i1+2,i2-1,i3,
     & axisp1,1)/(rx(i1+2,i2-1,i3)*sy(i1+2,i2-1,i3)-ry(i1+2,i2-1,i3)*
     & sx(i1+2,i2-1,i3)))-(rsxy(i1-2,i2-1,i3,axisp1,1)/(rx(i1-2,i2-1,
     & i3)*sy(i1-2,i2-1,i3)-ry(i1-2,i2-1,i3)*sx(i1-2,i2-1,i3)))))/(
     & 12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp1,1)/(rx(i1+1,i2+2,
     & i3)*sy(i1+1,i2+2,i3)-ry(i1+1,i2+2,i3)*sx(i1+1,i2+2,i3)))-(rsxy(
     & i1-1,i2+2,i3,axisp1,1)/(rx(i1-1,i2+2,i3)*sy(i1-1,i2+2,i3)-ry(
     & i1-1,i2+2,i3)*sx(i1-1,i2+2,i3))))-((rsxy(i1+2,i2+2,i3,axisp1,1)
     & /(rx(i1+2,i2+2,i3)*sy(i1+2,i2+2,i3)-ry(i1+2,i2+2,i3)*sx(i1+2,
     & i2+2,i3)))-(rsxy(i1-2,i2+2,i3,axisp1,1)/(rx(i1-2,i2+2,i3)*sy(
     & i1-2,i2+2,i3)-ry(i1-2,i2+2,i3)*sx(i1-2,i2+2,i3)))))/(12.*dr(0))
     & -(8.*((rsxy(i1+1,i2-2,i3,axisp1,1)/(rx(i1+1,i2-2,i3)*sy(i1+1,
     & i2-2,i3)-ry(i1+1,i2-2,i3)*sx(i1+1,i2-2,i3)))-(rsxy(i1-1,i2-2,
     & i3,axisp1,1)/(rx(i1-1,i2-2,i3)*sy(i1-1,i2-2,i3)-ry(i1-1,i2-2,
     & i3)*sx(i1-1,i2-2,i3))))-((rsxy(i1+2,i2-2,i3,axisp1,1)/(rx(i1+2,
     & i2-2,i3)*sy(i1+2,i2-2,i3)-ry(i1+2,i2-2,i3)*sx(i1+2,i2-2,i3)))-(
     & rsxy(i1-2,i2-2,i3,axisp1,1)/(rx(i1-2,i2-2,i3)*sy(i1-2,i2-2,i3)-
     & ry(i1-2,i2-2,i3)*sx(i1-2,i2-2,i3)))))/(12.*dr(0))))/(12.*dr(1))
                        a11rr = ((-30.*(rsxy(i1,i2,i3,axis,0)/(rx(i1,
     & i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+16.*((rsxy(i1+
     & 1,i2,i3,axis,0)/(rx(i1+1,i2,i3)*sy(i1+1,i2,i3)-ry(i1+1,i2,i3)*
     & sx(i1+1,i2,i3)))+(rsxy(i1-1,i2,i3,axis,0)/(rx(i1-1,i2,i3)*sy(
     & i1-1,i2,i3)-ry(i1-1,i2,i3)*sx(i1-1,i2,i3))))-((rsxy(i1+2,i2,i3,
     & axis,0)/(rx(i1+2,i2,i3)*sy(i1+2,i2,i3)-ry(i1+2,i2,i3)*sx(i1+2,
     & i2,i3)))+(rsxy(i1-2,i2,i3,axis,0)/(rx(i1-2,i2,i3)*sy(i1-2,i2,
     & i3)-ry(i1-2,i2,i3)*sx(i1-2,i2,i3)))))/(12.*dr(0)**2))
                        a12rr = ((-30.*(rsxy(i1,i2,i3,axis,1)/(rx(i1,
     & i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+16.*((rsxy(i1+
     & 1,i2,i3,axis,1)/(rx(i1+1,i2,i3)*sy(i1+1,i2,i3)-ry(i1+1,i2,i3)*
     & sx(i1+1,i2,i3)))+(rsxy(i1-1,i2,i3,axis,1)/(rx(i1-1,i2,i3)*sy(
     & i1-1,i2,i3)-ry(i1-1,i2,i3)*sx(i1-1,i2,i3))))-((rsxy(i1+2,i2,i3,
     & axis,1)/(rx(i1+2,i2,i3)*sy(i1+2,i2,i3)-ry(i1+2,i2,i3)*sx(i1+2,
     & i2,i3)))+(rsxy(i1-2,i2,i3,axis,1)/(rx(i1-2,i2,i3)*sy(i1-2,i2,
     & i3)-ry(i1-2,i2,i3)*sx(i1-2,i2,i3)))))/(12.*dr(0)**2))
                        a21rr = ((-30.*(rsxy(i1,i2,i3,axisp1,0)/(rx(i1,
     & i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+16.*((rsxy(i1+
     & 1,i2,i3,axisp1,0)/(rx(i1+1,i2,i3)*sy(i1+1,i2,i3)-ry(i1+1,i2,i3)
     & *sx(i1+1,i2,i3)))+(rsxy(i1-1,i2,i3,axisp1,0)/(rx(i1-1,i2,i3)*
     & sy(i1-1,i2,i3)-ry(i1-1,i2,i3)*sx(i1-1,i2,i3))))-((rsxy(i1+2,i2,
     & i3,axisp1,0)/(rx(i1+2,i2,i3)*sy(i1+2,i2,i3)-ry(i1+2,i2,i3)*sx(
     & i1+2,i2,i3)))+(rsxy(i1-2,i2,i3,axisp1,0)/(rx(i1-2,i2,i3)*sy(i1-
     & 2,i2,i3)-ry(i1-2,i2,i3)*sx(i1-2,i2,i3)))))/(12.*dr(0)**2))
                        a22rr = ((-30.*(rsxy(i1,i2,i3,axisp1,1)/(rx(i1,
     & i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+16.*((rsxy(i1+
     & 1,i2,i3,axisp1,1)/(rx(i1+1,i2,i3)*sy(i1+1,i2,i3)-ry(i1+1,i2,i3)
     & *sx(i1+1,i2,i3)))+(rsxy(i1-1,i2,i3,axisp1,1)/(rx(i1-1,i2,i3)*
     & sy(i1-1,i2,i3)-ry(i1-1,i2,i3)*sx(i1-1,i2,i3))))-((rsxy(i1+2,i2,
     & i3,axisp1,1)/(rx(i1+2,i2,i3)*sy(i1+2,i2,i3)-ry(i1+2,i2,i3)*sx(
     & i1+2,i2,i3)))+(rsxy(i1-2,i2,i3,axisp1,1)/(rx(i1-2,i2,i3)*sy(i1-
     & 2,i2,i3)-ry(i1-2,i2,i3)*sx(i1-2,i2,i3)))))/(12.*dr(0)**2))
                        a21ss = ((-30.*(rsxy(i1,i2,i3,axisp1,0)/(rx(i1,
     & i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+16.*((rsxy(i1,
     & i2+1,i3,axisp1,0)/(rx(i1,i2+1,i3)*sy(i1,i2+1,i3)-ry(i1,i2+1,i3)
     & *sx(i1,i2+1,i3)))+(rsxy(i1,i2-1,i3,axisp1,0)/(rx(i1,i2-1,i3)*
     & sy(i1,i2-1,i3)-ry(i1,i2-1,i3)*sx(i1,i2-1,i3))))-((rsxy(i1,i2+2,
     & i3,axisp1,0)/(rx(i1,i2+2,i3)*sy(i1,i2+2,i3)-ry(i1,i2+2,i3)*sx(
     & i1,i2+2,i3)))+(rsxy(i1,i2-2,i3,axisp1,0)/(rx(i1,i2-2,i3)*sy(i1,
     & i2-2,i3)-ry(i1,i2-2,i3)*sx(i1,i2-2,i3)))))/(12.*dr(1)**2))
                        a22ss = ((-30.*(rsxy(i1,i2,i3,axisp1,1)/(rx(i1,
     & i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+16.*((rsxy(i1,
     & i2+1,i3,axisp1,1)/(rx(i1,i2+1,i3)*sy(i1,i2+1,i3)-ry(i1,i2+1,i3)
     & *sx(i1,i2+1,i3)))+(rsxy(i1,i2-1,i3,axisp1,1)/(rx(i1,i2-1,i3)*
     & sy(i1,i2-1,i3)-ry(i1,i2-1,i3)*sx(i1,i2-1,i3))))-((rsxy(i1,i2+2,
     & i3,axisp1,1)/(rx(i1,i2+2,i3)*sy(i1,i2+2,i3)-ry(i1,i2+2,i3)*sx(
     & i1,i2+2,i3)))+(rsxy(i1,i2-2,i3,axisp1,1)/(rx(i1,i2-2,i3)*sy(i1,
     & i2-2,i3)-ry(i1,i2-2,i3)*sx(i1,i2-2,i3)))))/(12.*dr(1)**2))
                        urs=-(a12**2*vrr-2*a21s*us*a22-a21ss*u(i1,i2,
     & i3,ex)*a22-a12s*vr*a22-a12r*vs*a22-a22**2*vss-2*a22s*vs*a22-
     & a21*uss*a22-a11rs*u(i1,i2,i3,ex)*a22-a11s*ur*a22-a11r*us*a22+2*
     & a12*a12r*vr+a12*a21rs*u(i1,i2,i3,ex)+a12*a12rr*u(i1,i2,i3,ey)+
     & a12*a11*urr+2*a12*a11r*ur-a12rs*u(i1,i2,i3,ey)*a22+a12*a22r*vs+
     & a12*a11rr*u(i1,i2,i3,ex)+a12*a22s*vr+a12*a22rs*u(i1,i2,i3,ey)+
     & a12*a21s*ur+a12*a21r*us-a22ss*u(i1,i2,i3,ey)*a22)/(-a11*a22+
     & a21*a12)
                        vrs=(a11*a21rs*u(i1,i2,i3,ex)+a11*a12*vrr+2*
     & a11*a12r*vr+a11*a12rr*u(i1,i2,i3,ey)+2*a11*a11r*ur+a11*a11rr*u(
     & i1,i2,i3,ex)-a21*a22*vss+a11*a22r*vs+a11*a22s*vr+a11*a22rs*u(
     & i1,i2,i3,ey)+a11*a21r*us+a11*a21s*ur-a21*a12s*vr-a21*a12r*vs-
     & a21*a12rs*u(i1,i2,i3,ey)-a21*a11s*ur-a21*a11r*us-a21*a11rs*u(
     & i1,i2,i3,ex)-a21*a21ss*u(i1,i2,i3,ex)-a21*a22ss*u(i1,i2,i3,ey)-
     & 2*a21*a22s*vs-2*a21*a21s*us+a11**2*urr-a21**2*uss)/(-a11*a22+
     & a21*a12)
                        urrrr=urrrr2(i1,i2,i3,ex)
                        ussss=ussss2(i1,i2,i3,ex)
                        vrrrr=urrrr2(i1,i2,i3,ey)
                        vssss=ussss2(i1,i2,i3,ey)
                        ! **** finish these ****
                        urrss=0.  ! from equation   uxxxx + uxxyy = uttxx  [ u(x,0)=0 => uxxxx(x,0)=0 uxxtt(x,0)=0 ]
                        vrrss=0.  ! from equation
                        urrrs=-vrrss  ! from div
                        ursss=-vssss  ! from div
                        vrrrs=-urrrr  ! from div
                        vrsss=-urrss  ! from div
                          u(i1-is1,i2-is2,i3,ex)= 2.*u(i1,i2,i3,ex)-u(
     & i1+is1,i2+is2,i3,ex) + ( (dra)**2*urr+2.*(dra)*(dsa)*urs+(dsa)*
     & *2*uss )+ (1./12.)*( (dra)**4*urrrr + 4.*(dra)**3*(dsa)*urrrs +
     &  6.*(dra)**2*(dsa)**2*urrss + 4.*(dra)*(dsa)**3*ursss + (dsa)**
     & 4*ussss )
                          u(i1-is1,i2-is2,i3,ey)= 2.*u(i1,i2,i3,ey)-u(
     & i1+is1,i2+is2,i3,ey) + ( (dra)**2*vrr+2.*(dra)*(dsa)*vrs+(dsa)*
     & *2*vss )+ (1./12.)*( (dra)**4*vrrrr + 4.*(dra)**3*(dsa)*vrrrs +
     &  6.*(dra)**2*(dsa)**2*vrrss + 4.*(dra)*(dsa)**3*vrsss + (dsa)**
     & 4*vssss )
                          u(i1-2*is1,i2-is2,i3,ex)= 2.*u(i1,i2,i3,ex)-
     & u(i1+2*is1,i2+is2,i3,ex) + ( (2.*dra)**2*urr+2.*(2.*dra)*(dsa)*
     & urs+(dsa)**2*uss )+ (1./12.)*( (2.*dra)**4*urrrr + 4.*(2.*dra)*
     & *3*(dsa)*urrrs + 6.*(2.*dra)**2*(dsa)**2*urrss + 4.*(2.*dra)*(
     & dsa)**3*ursss + (dsa)**4*ussss )
                          u(i1-2*is1,i2-is2,i3,ey)= 2.*u(i1,i2,i3,ey)-
     & u(i1+2*is1,i2+is2,i3,ey) + ( (2.*dra)**2*vrr+2.*(2.*dra)*(dsa)*
     & vrs+(dsa)**2*vss )+ (1./12.)*( (2.*dra)**4*vrrrr + 4.*(2.*dra)*
     & *3*(dsa)*vrrrs + 6.*(2.*dra)**2*(dsa)**2*vrrss + 4.*(2.*dra)*(
     & dsa)**3*vrsss + (dsa)**4*vssss )
                          u(i1-is1,i2-2*is2,i3,ex)= 2.*u(i1,i2,i3,ex)-
     & u(i1+is1,i2+2*is2,i3,ex) + ( (dra)**2*urr+2.*(dra)*(2.*dsa)*
     & urs+(2.*dsa)**2*uss )+ (1./12.)*( (dra)**4*urrrr + 4.*(dra)**3*
     & (2.*dsa)*urrrs + 6.*(dra)**2*(2.*dsa)**2*urrss + 4.*(dra)*(2.*
     & dsa)**3*ursss + (2.*dsa)**4*ussss )
                          u(i1-is1,i2-2*is2,i3,ey)= 2.*u(i1,i2,i3,ey)-
     & u(i1+is1,i2+2*is2,i3,ey) + ( (dra)**2*vrr+2.*(dra)*(2.*dsa)*
     & vrs+(2.*dsa)**2*vss )+ (1./12.)*( (dra)**4*vrrrr + 4.*(dra)**3*
     & (2.*dsa)*vrrrs + 6.*(dra)**2*(2.*dsa)**2*vrrss + 4.*(dra)*(2.*
     & dsa)**3*vrsss + (2.*dsa)**4*vssss )
                          u(i1-2*is1,i2-2*is2,i3,ex)= 2.*u(i1,i2,i3,ex)
     & -u(i1+2*is1,i2+2*is2,i3,ex) + ( (2.*dra)**2*urr+2.*(2.*dra)*(
     & 2.*dsa)*urs+(2.*dsa)**2*uss )+ (1./12.)*( (2.*dra)**4*urrrr + 
     & 4.*(2.*dra)**3*(2.*dsa)*urrrs + 6.*(2.*dra)**2*(2.*dsa)**2*
     & urrss + 4.*(2.*dra)*(2.*dsa)**3*ursss + (2.*dsa)**4*ussss )
                          u(i1-2*is1,i2-2*is2,i3,ey)= 2.*u(i1,i2,i3,ey)
     & -u(i1+2*is1,i2+2*is2,i3,ey) + ( (2.*dra)**2*vrr+2.*(2.*dra)*(
     & 2.*dsa)*vrs+(2.*dsa)**2*vss )+ (1./12.)*( (2.*dra)**4*vrrrr + 
     & 4.*(2.*dra)**3*(2.*dsa)*vrrrs + 6.*(2.*dra)**2*(2.*dsa)**2*
     & vrrss + 4.*(2.*dra)*(2.*dsa)**3*vrsss + (2.*dsa)**4*vssss )
                        setCornersToExact=.false.
                        ! check errors
                        ! --- Now do Hz ---
                        ur = ur4(i1,i2,i3,hz)
                        us = us4(i1,i2,i3,hz)
                        urrr=urrr2(i1,i2,i3,hz)
                        usss=usss2(i1,i2,i3,hz)
                        urrs=0. !  (from ur(0,s)=0 and us(r,0)=0)  ! ****************** fix for TZ
                        urss=0. !  (from ur(0,s)=0 and us(r,0)=0)
                    !   write(*,'(" ghostValuesOutsideCorners2d: i1,i2,is1,is2=",4i4," dra,dsa=",2e10.2," urrr,usss,urrs,urss=",4e10.2)')i1,i2,is1,is2,dra,dsa,urrr,usss,urrs,urss
                    !    urrr=0.
                    !    usss=0.
                    !    urrs=0.
                    !   urss=0.
                            u(i1-is1,i2-is2,i3,hz)=u(i1+is1,i2+is2,i3,
     & hz) - 2.*((dra)*ur+(dsa)*us) - (1./3.)*((dra)**3*urrr+3.*(dra)*
     & *2*(dsa)*urrs+3.*(dra)*(dsa)**2*urss+(dsa)**3*usss)
                            u(i1-2*is1,i2-is2,i3,hz)=u(i1+2*is1,i2+is2,
     & i3,hz) - 2.*((2.*dra)*ur+(dsa)*us) - (1./3.)*((2.*dra)**3*urrr+
     & 3.*(2.*dra)**2*(dsa)*urrs+3.*(2.*dra)*(dsa)**2*urss+(dsa)**3*
     & usss)
                            u(i1-is1,i2-2*is2,i3,hz)=u(i1+is1,i2+2*is2,
     & i3,hz) - 2.*((dra)*ur+(2.*dsa)*us) - (1./3.)*((dra)**3*urrr+3.*
     & (dra)**2*(2.*dsa)*urrs+3.*(dra)*(2.*dsa)**2*urss+(2.*dsa)**3*
     & usss)
                            u(i1-2*is1,i2-2*is2,i3,hz)=u(i1+2*is1,i2+2*
     & is2,i3,hz) - 2.*((2.*dra)*ur+(2.*dsa)*us) - (1./3.)*((2.*dra)**
     & 3*urrr+3.*(2.*dra)**2*(2.*dsa)*urrs+3.*(2.*dra)*(2.*dsa)**2*
     & urss+(2.*dsa)**3*usss)
                else if( boundaryCondition(side1,0).ge.abcEM2 .and. 
     & boundaryCondition(side1,0).le.lastBC .and. boundaryCondition(
     & side2,1).ge.abcEM2 .and. boundaryCondition(side2,1).le.lastBC )
     & then
                  ! **** do nothing *** this is done in abcMaxwell
                end if
                end do
                end do
            else
                axis=0
                axisp1=1
                i3=gridIndexRange(0,2)
                numberOfGhostPoints=orderOfAccuracy/2
                do side1=0,1
                do side2=0,1
                if( boundaryCondition(side1,0)
     & .eq.perfectElectricalConductor .and.boundaryCondition(side2,1)
     & .eq.perfectElectricalConductor )then
                  i1=gridIndexRange(side1,0) ! (i1,i2,i3)=corner point
                  i2=gridIndexRange(side2,1)
                  ! write(*,'("bcOpt: assign corner side1,side2,i1,i2,i3=",2i2,3i5)') side1,side2,i1,i2,i3
                  is1=1-2*side1
                  is2=1-2*side2
                  dra=dr(0)*is1
                  dsa=dr(1)*is2
                  g2a=0.
                  ! For now assign second ghost line by symmetry 
                  do m=1,numberOfGhostPoints
                    js1=is1*m  ! shift to ghost point "m"
                    js2=is2*m
                     ! *** there is no need to do this for orderOfAccuracy.eq.4 -- these are done below
                  end do
                  ! assign u(i1-is1,i2,i3,ev) and u(i1,i2-is2,i3,ev)
                      ! write(*,'("assign extended-curvilinear-order4 grid,side,axis=",i5,2i3)') grid,side,axis
                      axis=0   ! for c11, c22, ...
                      axisp1=1
                       a11m2 =rsxy(i1-2*is1,i2    ,i3,0,0)
                       a12m2 =rsxy(i1-2*is1,i2    ,i3,0,1)
                       a11m1 =rsxy(i1-is1,i2    ,i3,0,0)
                       a12m1 =rsxy(i1-is1,i2    ,i3,0,1)
                       a11   =rsxy(i1    ,i2    ,i3,0,0)
                       a12   =rsxy(i1    ,i2    ,i3,0,1)
                       a21   =rsxy(i1    ,i2    ,i3,1,0)
                       a22   =rsxy(i1    ,i2    ,i3,1,1)
                       a21zm1 =rsxy(i1   ,i2-is2,i3,1,0)
                       a22zm1 =rsxy(i1   ,i2-is2,i3,1,1)
                       a21zm2 =rsxy(i1   ,i2-2*is2,i3,1,0)
                       a22zm2 =rsxy(i1   ,i2-2*is2,i3,1,1)
                       c11 = (rsxy(i1,i2,i3,axis,0)**2+rsxy(i1,i2,i3,
     & axis,1)**2)
                       c22 = (rsxy(i1,i2,i3,axisp1,0)**2+rsxy(i1,i2,i3,
     & axisp1,1)**2)
                       c1  = (rsxyx42(i1,i2,i3,axis,0)+rsxyy42(i1,i2,
     & i3,axis,1))
                       c2  = (rsxyx42(i1,i2,i3,axisp1,0)+rsxyy42(i1,i2,
     & i3,axisp1,1))
                       c11r = (8.*((rsxy(i1+is1,i2,i3,axis,0)**2+rsxy(
     & i1+is1,i2,i3,axis,1)**2)-(rsxy(i1-is1,i2,i3,axis,0)**2+rsxy(i1-
     & is1,i2,i3,axis,1)**2))   -((rsxy(i1+2*is1,i2,i3,axis,0)**2+
     & rsxy(i1+2*is1,i2,i3,axis,1)**2)-(rsxy(i1-2*is1,i2,i3,axis,0)**
     & 2+rsxy(i1-2*is1,i2,i3,axis,1)**2))   )/(12.*dra)
                       c22r = (8.*((rsxy(i1+is1,i2,i3,axisp1,0)**2+
     & rsxy(i1+is1,i2,i3,axisp1,1)**2)-(rsxy(i1-is1,i2,i3,axisp1,0)**
     & 2+rsxy(i1-is1,i2,i3,axisp1,1)**2))   -((rsxy(i1+2*is1,i2,i3,
     & axisp1,0)**2+rsxy(i1+2*is1,i2,i3,axisp1,1)**2)-(rsxy(i1-2*is1,
     & i2,i3,axisp1,0)**2+rsxy(i1-2*is1,i2,i3,axisp1,1)**2))   )/(12.*
     & dra)
                       c11s = (8.*((rsxy(i1,i2+is2,i3,axis,0)**2+rsxy(
     & i1,i2+is2,i3,axis,1)**2)-(rsxy(i1,i2-is2,i3,axis,0)**2+rsxy(i1,
     & i2-is2,i3,axis,1)**2))   -((rsxy(i1,i2+2*is2,i3,axis,0)**2+
     & rsxy(i1,i2+2*is2,i3,axis,1)**2)-(rsxy(i1,i2-2*is2,i3,axis,0)**
     & 2+rsxy(i1,i2-2*is2,i3,axis,1)**2))   )/(12.*dsa)
                       c22s = (8.*((rsxy(i1,i2+is2,i3,axisp1,0)**2+
     & rsxy(i1,i2+is2,i3,axisp1,1)**2)-(rsxy(i1,i2-is2,i3,axisp1,0)**
     & 2+rsxy(i1,i2-is2,i3,axisp1,1)**2))   -((rsxy(i1,i2+2*is2,i3,
     & axisp1,0)**2+rsxy(i1,i2+2*is2,i3,axisp1,1)**2)-(rsxy(i1,i2-2*
     & is2,i3,axisp1,0)**2+rsxy(i1,i2-2*is2,i3,axisp1,1)**2))   )/(
     & 12.*dsa)
                       !  Solve for Hz on extended boundaries from:  
                       !          wr=fw1  
                       !          ws=fw2  
                       !          c11*wrrr+(c1+c11r)*wrr + c22r*wss=fw3, (i.e. (Lw).r=0 )
                       !          c22*wsss+(c2+c22s)*wss + c11s*wrr=fw4, (i.e. (Lw).s=0 )
                       fw1=0.
                       fw2=0.
                       fw3=0.
                       fw4=0.
                          call ogf2dfo(ep,fieldOption,xy(i1-is1,i2,i3,
     & 0),xy(i1-is1,i2,i3,1),t,uv0(0),uv0(1),uv0(2))
                         tdu10=a11m1*uv0(0)+a12m1*uv0(1)
                          call ogf2dfo(ep,fieldOption,xy(i1,i2-is2,i3,
     & 0),xy(i1,i2-is2,i3,1),t,uv0(0),uv0(1),uv0(2))
                         tdu01=a21zm1*uv0(0)+a22zm1*uv0(1)
                          call ogf2dfo(ep,fieldOption,xy(i1-2*is1,i2,
     & i3,0),xy(i1-2*is1,i2,i3,1),t,uv0(0),uv0(1),uv0(2))
                         tdu20=a11m2*uv0(0)+a12m2*uv0(1)
                          call ogf2dfo(ep,fieldOption,xy(i1,i2-2*is2,
     & i3,0),xy(i1,i2-2*is2,i3,1),t,uv0(0),uv0(1),uv0(2))
                         tdu02=a21zm2*uv0(0)+a22zm2*uv0(1)
                         ! For TZ: utt0 = utt - ett + Lap(e)
                         call ogDeriv(ep, 0, 2,0,0, xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.,t,ex, urr)
                         call ogDeriv(ep, 0, 0,2,0, xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.,t,ex, uss)
                         utt00=urr+uss
                         call ogDeriv(ep, 0, 2,0,0, xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.,t,ey, vrr)
                         call ogDeriv(ep, 0, 0,2,0, xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.,t,ey, vss)
                         vtt00=vrr+vss
                         ! Now compute forcing for Hz
                          call ogf2dfo(ep,fieldOption,xy(i1,i2,i3,0),
     & xy(i1,i2,i3,1),t,uv0(0),uv0(1),uv0(2))
                          call ogf2dfo(ep,fieldOption,xy(i1-is1,i2,i3,
     & 0),xy(i1-is1,i2,i3,1),t,uvm(0),uvm(1),uvm(2))
                          call ogf2dfo(ep,fieldOption,xy(i1+is1,i2,i3,
     & 0),xy(i1+is1,i2,i3,1),t,uvp(0),uvp(1),uvp(2))
                          call ogf2dfo(ep,fieldOption,xy(i1-2*is1,i2,
     & i3,0),xy(i1-2*is1,i2,i3,1),t,uvm2(0),uvm2(1),uvm2(2))
                          call ogf2dfo(ep,fieldOption,xy(i1+2*is1,i2,
     & i3,0),xy(i1+2*is1,i2,i3,1),t,uvp2(0),uvp2(1),uvp2(2))
                         wr = (8.*(uvp(2)-uvm(2))-(uvp2(2)-uvm2(2)))/(
     & 12.*dra)
                         wrr=(uvp(2)-2.*uv0(2)+uvm(2))/(dra**2)
                         wrrr=(uvp2(2)-2.*(uvp(2)-uvm(2))-uvm2(2))/(2.*
     & dra**3)
                          call ogf2dfo(ep,fieldOption,xy(i1,i2,i3,0),
     & xy(i1,i2,i3,1),t,uv0(0),uv0(1),uv0(2))
                          call ogf2dfo(ep,fieldOption,xy(i1,i2-is2,i3,
     & 0),xy(i1,i2-is2,i3,1),t,uvm(0),uvm(1),uvm(2))
                          call ogf2dfo(ep,fieldOption,xy(i1,i2+is2,i3,
     & 0),xy(i1,i2+is2,i3,1),t,uvp(0),uvp(1),uvp(2))
                          call ogf2dfo(ep,fieldOption,xy(i1,i2-2*is2,
     & i3,0),xy(i1,i2-2*is2,i3,1),t,uvm2(0),uvm2(1),uvm2(2))
                          call ogf2dfo(ep,fieldOption,xy(i1,i2+2*is2,
     & i3,0),xy(i1,i2+2*is2,i3,1),t,uvp2(0),uvp2(1),uvp2(2))
                         ws = (8.*(uvp(2)-uvm(2))-(uvp2(2)-uvm2(2)))/(
     & 12.*dsa)
                         wss=(uvp(2)-2.*uv0(2)+uvm(2))/(dsa**2)
                         wsss=(uvp2(2)-2.*(uvp(2)-uvm(2))-uvm2(2))/(2.*
     & dsa**3)
                         fw1=wr
                         fw2=ws
                         fw3= c11*wrrr+(c1+c11r)*wrr +c22r*wss
                         fw4= c22*wsss+(c2+c22s)*wss +c11s*wrr
                        ! uLap = uLaplacian42(i1,i2,i3,ex)
                        ! vLap = uLaplacian42(i1,i2,i3,ey)
                        ! Drop the cross term for now -- this should be fixed for non-orthogonal grids ---
                        uLap = c11*urr4(i1,i2,i3,ex)+c22*uss4(i1,i2,i3,
     & ex)+c1*ur4(i1,i2,i3,ex)+c2*us4(i1,i2,i3,ex)
                        vLap = c11*urr4(i1,i2,i3,ey)+c22*uss4(i1,i2,i3,
     & ey)+c1*ur4(i1,i2,i3,ey)+c2*us4(i1,i2,i3,ey)
                      ! The next file is from bc4c.maple


! ****************** Fourth-order ********************
      g1a = a21*(6*u(i1,i2,i3,ex)-4*u(i1+is1,i2,i3,ex)+u(i1+2*is1,i2,
     & i3,ex))+a22*(6*u(i1,i2,i3,ey)-4*u(i1+is1,i2,i3,ey)+u(i1+2*is1,
     & i2,i3,ey))

      g2a = a11*(6*u(i1,i2,i3,ex)-4*u(i1,i2+is2,i3,ex)+u(i1,i2+2*is2,
     & i3,ex))+a12*(6*u(i1,i2,i3,ey)-4*u(i1,i2+is2,i3,ey)+u(i1,i2+2*
     & is2,i3,ey))

      cu20=-1/12.*c11/dra**2+1/12.*c1/dra

      cu02=-1/12.*c22/dsa**2+1/12.*c2/dsa

      cu10=4/3.*c11/dra**2-2/3.*c1/dra

      cu01=4/3.*c22/dsa**2-2/3.*c2/dsa

      cv20=-1/12.*c11/dra**2+1/12.*c1/dra

      cv02=-1/12.*c22/dsa**2+1/12.*c2/dsa

      cv10=4/3.*c11/dra**2-2/3.*c1/dra

      cv01=4/3.*c22/dsa**2-2/3.*c2/dsa

      gLu= uLap-(cu20*u(i1-2*is1,i2,i3,ex) + cu10*u(i1-is1,i2,i3,ex) + 
     & cu02*u(i1,i2-2*is2,i3,ex) + cu01*u(i1,i2-is2,i3,ex)) -utt00

      gLv= vLap-(cv20*u(i1-2*is1,i2,i3,ey) + cv10*u(i1-is1,i2,i3,ey) + 
     & cv02*u(i1,i2-2*is2,i3,ey) + cv01*u(i1,i2-is2,i3,ey)) -vtt00

      det=4*a11m2*a22*cv02*a21zm2*a12*a21zm1*cu10*a12m1+4*a11m1*a22*
     & cu20*a12m2*cv01*a21zm1*a21zm2*a12-16*a21*a12m1*a21zm2*cu20*
     & a12m2*cv02*a12*a21zm1-4*a21*a12m1*cu20*a12m2*cv01*a21zm1*
     & a21zm2*a12-a21*a12m2*cv01*a21zm1*a21zm2*a12*cu10*a12m1+16*
     & a11m1*a22*a21zm2*cu20*a12m2*cv02*a12*a21zm1-4*a21*a12m2*cv02*
     & a21zm2*a12*a21zm1*cu10*a12m1+16*a11m2*cv20*a21*a12m1*cu02*
     & a22zm2*a12*a21zm1-16*a11m2*a11m1*cv20*a22*cu02*a22zm2*a12*
     & a21zm1-a11m1*a21*a12m2*cv10*a22zm1*cu01*a11*a22zm2-4*a11m1*a21*
     & a12m2*cv10*a22zm1*cu02*a22zm2*a11+4*a11m2*a11m1*cv20*a22*
     & a22zm1*cu01*a11*a22zm2+16*a11m2*a11m1*cv20*a22*a22zm1*cu02*
     & a22zm2*a11-4*a11m2*a11m1*a22*cv10*cu02*a22zm2*a12*a21zm1+4*
     & a11m2*a11m1*a22*cv10*a22zm1*cu02*a22zm2*a11-a11m2*a22*cv01*
     & a21zm1*a11*a22zm2*cu10*a12m1-4*a11m1*a22*cu20*a12m2*cv01*
     & a21zm1*a11*a22zm2+4*a21*a12m1*cu20*a12m2*cv01*a21zm1*a11*
     & a22zm2+4*a11m1*a21*a12m2*cv10*cu02*a22zm2*a12*a21zm1+a21*a12m2*
     & cv01*a21zm1*a11*a22zm2*cu10*a12m1-4*a11m2*cv20*a21*a12m1*
     & a22zm1*cu01*a11*a22zm2-16*a11m2*cv20*a21*a12m1*a22zm1*cu02*
     & a22zm2*a11+a11m2*a11m1*a22*cv10*a22zm1*cu01*a11*a22zm2-a11m2*
     & a11m1*a22*cv10*a22zm1*cu01*a21zm2*a12+16*a21*a12m1*a22zm1*
     & a21zm2*cu20*a12m2*cv02*a11+4*a22zm1*a21*a12m2*cv02*a21zm2*a11*
     & cu10*a12m1+4*a11m2*cv20*a21*a12m1*a22zm1*cu01*a21zm2*a12+a11m2*
     & a22*cv01*a21zm1*a21zm2*a12*cu10*a12m1+a11m1*a21*a12m2*cv10*
     & a22zm1*cu01*a21zm2*a12-16*a11m1*a22*a22zm1*a21zm2*cu20*a12m2*
     & cv02*a11-4*a11m2*a11m1*cv20*a22*a22zm1*cu01*a21zm2*a12-4*a11m2*
     & a22zm1*a22*cv02*a21zm2*a11*cu10*a12m1

      u(i1-is1,i2,i3,ex)=(a12m1*cu01*a21zm2*a12*a21*a12m2*cv01*tdu01+
     & a12m1*a22zm1*a11m2*cv20*cu01*a21zm2*a12*g1a+a12m1*a22zm1*cu01*
     & a21zm2*a12*a21*tdu20*cv20+4*a12m1*a22zm1*a11*a21zm2*cu20*tdu20*
     & a22*cv02+4*a12m1*a22zm1*a11*a21zm2*cu20*a12m2*cv02*g1a+a12m1*
     & a22zm1*cu01*a21zm2*a12*a21*a12m2*gLv-4*a12m1*a22zm1*a11*a21zm2*
     & gLu*a21*a12m2*cv02-a12m1*a22zm1*cu01*a21zm2*g2a*a21*a12m2*cv02-
     & 4*tdu10*a11m2*cv20*a22*a22zm1*cu01*a21zm2*a12-4*a12m1*a11*cu02*
     & a22zm2*a21*a12m2*cv01*tdu01-4*a12m1*a21zm2*cu20*tdu20*a12*
     & a21zm1*a22*cv02+a12m1*a22zm1*a11m2*cu01*a21zm2*g2a*a22*cv02-
     & a12m1*a22zm1*a11m2*cu01*a21zm2*a12*a22*gLv+4*a12m1*a22zm1*
     & a11m2*a11*a21zm2*gLu*a22*cv02+tdu10*a21*a12m2*cv10*a22zm1*cu01*
     & a21zm2*a12-16*tdu10*a22*a22zm1*a21zm2*cu20*a12m2*cv02*a11-
     & tdu10*a11m2*a22*cv10*a22zm1*cu01*a21zm2*a12-4*a12m1*a21zm2*
     & cu20*a12m2*cv02*a12*a21zm1*g1a-a12m1*cu20*a12m2*cv01*a21zm1*
     & a21zm2*a12*g1a+4*a12m1*a21zm2*gLu*a12*a21zm1*a21*a12m2*cv02+
     & a12m1*a21zm2*gLu*a21*a12m2*cv01*a21zm1*a12-a12m1*a21zm2*cu20*
     & tdu20*a22*cv01*a21zm1*a12+4*a12m1*cu01*a21zm2*a12*tdu01*a21*
     & a12m2*cv02-a12m1*a11m2*cu01*a21zm2*a12*a22*cv01*tdu01-4*a12m1*
     & a11m2*cu01*a21zm2*a12*tdu01*a22*cv02-a12m1*a11m2*a21zm2*gLu*
     & a22*cv01*a21zm1*a12-4*a12m1*a11m2*a21zm2*gLu*a12*a21zm1*a22*
     & cv02+4*tdu10*a22*cu20*a12m2*cv01*a21zm1*a21zm2*a12+16*tdu10*
     & a22*a21zm2*cu20*a12m2*cv02*a12*a21zm1-a12m1*a22zm1*a11*cv20*
     & cu01*a22zm2*a21*tdu20-a12m1*a22zm1*a11m2*a11*cv20*cu01*a22zm2*
     & g1a+4*a12m1*a11m2*a11*cu02*a22zm2*a22*cv01*tdu01+a12m1*a11m2*
     & a11*cu01*a22zm2*a22*cv01*tdu01-a12m1*a11*cu01*a22zm2*a21*a12m2*
     & cv01*tdu01+4*tdu10*a11m2*cv20*a22*a22zm1*cu01*a11*a22zm2+4*
     & tdu10*a21*a12m2*cv10*cu02*a22zm2*a12*a21zm1+a12m1*a11*cu20*
     & a12m2*cv01*a21zm1*a22zm2*g1a+4*a12m1*cu02*a22zm2*a12*a21zm1*
     & a21*a12m2*gLv+4*a12m1*cu02*a22zm2*a12*a21zm1*a21*tdu20*cv20+
     & a12m1*a11*cu20*tdu20*a22*cv01*a21zm1*a22zm2+4*a12m1*a11m2*cv20*
     & cu02*a22zm2*a12*a21zm1*g1a-4*tdu10*a21*a12m2*cv10*a22zm1*cu02*
     & a22zm2*a11-tdu10*a21*a12m2*cv10*a22zm1*cu01*a11*a22zm2-a12m1*
     & a11m2*cu02*a22zm2*a22*cv01*a21zm1*g2a-4*a12m1*a11m2*cu02*
     & a22zm2*a12*a21zm1*a22*gLv+a12m1*a11m2*a11*gLu*a22*cv01*a21zm1*
     & a22zm2-4*tdu10*a11m2*a22*cv10*cu02*a22zm2*a12*a21zm1-16*tdu10*
     & a11m2*cv20*a22*cu02*a22zm2*a12*a21zm1+a12m1*cu02*a22zm2*a21*
     & a12m2*cv01*a21zm1*g2a-a12m1*a11*gLu*a21*a12m2*cv01*a21zm1*
     & a22zm2+tdu10*a11m2*a22*cv10*a22zm1*cu01*a11*a22zm2+a12m1*
     & a22zm1*a11m2*a11*cu01*a22zm2*a22*gLv+16*tdu10*a11m2*cv20*a22*
     & a22zm1*cu02*a22zm2*a11+4*tdu10*a11m2*a22*cv10*a22zm1*cu02*
     & a22zm2*a11-a12m1*a22zm1*a11*cu01*a22zm2*a21*a12m2*gLv-4*a12m1*
     & a22zm1*a11*cu02*a22zm2*a21*a12m2*gLv-4*a12m1*a22zm1*a11m2*a11*
     & cv20*cu02*a22zm2*g1a+4*a12m1*a22zm1*a11m2*a11*cu02*a22zm2*a22*
     & gLv-4*a12m1*a22zm1*a11*cv20*cu02*a22zm2*a21*tdu20-4*a12m1*
     & a11m2*cu02*tdu02*a12*a21zm1*a22*cv02-a12m1*a11m2*cu02*tdu02*
     & a22*cv01*a21zm1*a12+4*a12m1*cu02*tdu02*a12*a21zm1*a21*a12m2*
     & cv02+a12m1*cu02*tdu02*a21*a12m2*cv01*a21zm1*a12-4*tdu10*a22*
     & cu20*a12m2*cv01*a21zm1*a11*a22zm2+4*a12m1*a22zm1*a11m2*a11*
     & cu02*tdu02*a22*cv02+a12m1*a22zm1*a11m2*a11*cu01*tdu02*a22*cv02-
     & 4*a12m1*a22zm1*a11*cu02*tdu02*a21*a12m2*cv02-a12m1*a22zm1*a11*
     & cu01*tdu02*a21*a12m2*cv02)/det

      u(i1-is1,i2,i3,ey)=(-4*a22zm1*a11m2*a11*a11m1*cu02*tdu02*a22*
     & cv02-a22zm1*a11m2*a11*a11m1*cu01*tdu02*a22*cv02+4*a22zm1*a11*
     & a11m1*cu02*tdu02*a21*a12m2*cv02+a22zm1*a11*a11m1*cu01*tdu02*
     & a21*a12m2*cv02+4*a11m2*a11m1*cu02*tdu02*a12*a21zm1*a22*cv02-4*
     & a11m1*cu02*tdu02*a12*a21zm1*a21*a12m2*cv02+a11m2*a11m1*cu02*
     & tdu02*a22*cv01*a21zm1*a12-a11m1*cu02*tdu02*a21*a12m2*cv01*
     & a21zm1*a12+a11*a11m1*cu01*a22zm2*a21*a12m2*cv01*tdu01+4*a11*
     & a11m1*cu02*a22zm2*a21*a12m2*cv01*tdu01+4*a22zm1*a11*cv20*a11m1*
     & cu02*a22zm2*a21*tdu20-4*a22zm1*a11m2*a11*a11m1*cu02*a22zm2*a22*
     & gLv+a22zm1*a11m2*a11*cv20*a11m1*cu01*a22zm2*g1a-4*a22zm1*a11m2*
     & a11*cv20*tdu10*a21*cu01*a22zm2-16*a22zm1*a11m2*a11*cv20*tdu10*
     & a21*cu02*a22zm2-a22zm1*a11m2*a11*a11m1*cu01*a22zm2*a22*gLv+4*
     & a22zm1*a11m2*a11*cv20*a11m1*cu02*a22zm2*g1a+4*a22zm1*a11*a11m1*
     & cu02*a22zm2*a21*a12m2*gLv+a22zm1*a11*a11m1*cu01*a22zm2*a21*
     & a12m2*gLv+a11*a11m1*gLu*a21*a12m2*cv01*a21zm1*a22zm2-4*a11m1*
     & cu02*a22zm2*a12*a21zm1*a21*a12m2*gLv-a11m2*a11*tdu10*a22*cv01*
     & a21zm1*a22zm2*cu10+16*a11m2*cv20*tdu10*a12*a21zm1*a21*cu02*
     & a22zm2-a11*a11m1*cu20*a12m2*cv01*a21zm1*a22zm2*g1a+a11*tdu10*
     & a21*a12m2*cv01*a21zm1*a22zm2*cu10+4*a11*tdu10*a21*cu20*a12m2*
     & cv01*a21zm1*a22zm2-a11m1*cu02*a22zm2*a21*a12m2*cv01*a21zm1*g2a-
     & a11m2*a11*a11m1*gLu*a22*cv01*a21zm1*a22zm2+4*a11m2*a11m1*cu02*
     & a22zm2*a12*a21zm1*a22*gLv-a11*a11m1*cu20*tdu20*a22*cv01*a21zm1*
     & a22zm2-4*a11m1*cu02*a22zm2*a12*a21zm1*a21*tdu20*cv20+a11m2*
     & a11m1*cu02*a22zm2*a22*cv01*a21zm1*g2a-4*a11m2*cv20*a11m1*cu02*
     & a22zm2*a12*a21zm1*g1a-4*a11m1*a21zm2*gLu*a12*a21zm1*a21*a12m2*
     & cv02-a11m1*a21zm2*gLu*a21*a12m2*cv01*a21zm1*a12+4*a11m1*a21zm2*
     & cu20*a12m2*cv02*a12*a21zm1*g1a+4*a11m1*a21zm2*cu20*tdu20*a12*
     & a21zm1*a22*cv02+a11m1*a21zm2*cu20*tdu20*a22*cv01*a21zm1*a12+
     & a11m2*a11m1*a21zm2*gLu*a22*cv01*a21zm1*a12+a22zm1*a11*cv20*
     & a11m1*cu01*a22zm2*a21*tdu20-a11m2*a11*a11m1*cu01*a22zm2*a22*
     & cv01*tdu01-4*a11m2*a11*a11m1*cu02*a22zm2*a22*cv01*tdu01+4*
     & a22zm1*a11*a21zm2*tdu10*a21*a12m2*cv02*cu10+16*a22zm1*a11*
     & a21zm2*tdu10*a21*cu20*a12m2*cv02+4*a22zm1*a11*a11m1*a21zm2*gLu*
     & a21*a12m2*cv02-a22zm1*a11m1*cu01*a21zm2*a12*a21*a12m2*gLv-4*
     & a22zm1*a11*a11m1*a21zm2*cu20*a12m2*cv02*g1a+a22zm1*a11m1*cu01*
     & a21zm2*g2a*a21*a12m2*cv02-4*a22zm1*a11m2*a11*a21zm2*tdu10*a22*
     & cv02*cu10+4*a22zm1*a11m2*cv20*a21zm2*tdu10*a21*cu01*a12+a11m1*
     & cu20*a12m2*cv01*a21zm1*a21zm2*a12*g1a+4*a11m2*cu10*a21zm2*
     & tdu10*a12*a21zm1*a22*cv02+4*a11m2*a11m1*a21zm2*gLu*a12*a21zm1*
     & a22*cv02-cu10*a21zm2*tdu10*a21*a12m2*cv01*a21zm1*a12-4*a21zm2*
     & tdu10*a21*cu20*a12m2*cv01*a21zm1*a12-4*cu10*a21zm2*tdu10*a12*
     & a21zm1*a21*a12m2*cv02+4*a11m2*a11m1*cu01*a21zm2*a12*tdu01*a22*
     & cv02+a11m2*a11m1*cu01*a21zm2*a12*a22*cv01*tdu01-4*a11m1*cu01*
     & a21zm2*a12*tdu01*a21*a12m2*cv02-a11m1*cu01*a21zm2*a12*a21*
     & a12m2*cv01*tdu01+a11m2*cu10*a21zm2*tdu10*a22*cv01*a21zm1*a12-
     & 16*a21zm2*tdu10*a12*a21zm1*a21*cu20*a12m2*cv02-a22zm1*a11m1*
     & cu01*a21zm2*a12*a21*tdu20*cv20-a22zm1*a11m2*a11m1*cu01*a21zm2*
     & g2a*a22*cv02-a22zm1*a11m2*cv20*a11m1*cu01*a21zm2*a12*g1a-4*
     & a22zm1*a11m2*a11*a11m1*a21zm2*gLu*a22*cv02+a22zm1*a11m2*a11m1*
     & cu01*a21zm2*a12*a22*gLv-4*a22zm1*a11*a11m1*a21zm2*cu20*tdu20*
     & a22*cv02)/det

      u(i1,i2-is2,i3,ex)=(a22zm1*a11*a22zm2*cv20*a21*tdu20*cu10*a12m1-
     & a22zm1*a11*a22zm2*a11m1*a22*cv10*cu20*tdu20+4*a22zm1*a11*
     & a22zm2*a21*a12m1*cu20*a12m2*gLv+a22zm1*a11*a22zm2*a21*a12m2*
     & gLv*cu10*a12m1-4*a22zm1*g2a*a11m2*cv20*a21*a12m1*cu02*a22zm2-
     & a22zm1*a11*a22zm2*a11m2*a22*gLv*cu10*a12m1+4*a22zm1*a11*a22zm2*
     & a11m2*cv20*a21*a12m1*gLu-4*a22zm1*a11*a22zm2*a11m2*a11m1*cv20*
     & a22*gLu+a22zm1*a11*a22zm2*a11m2*cv20*g1a*cu10*a12m1-a22zm1*a11*
     & a22zm2*a11m2*a11m1*a22*cv10*gLu-4*a22zm1*a11*a22zm2*a11m1*a22*
     & cu20*a12m2*gLv-4*a22zm1*a11*a22zm2*a11m1*cv20*a22*cu20*tdu20+
     & a22zm1*a11*a22zm2*a11m1*a21*a12m2*cv10*gLu-a22zm1*a11*a22zm2*
     & a11m1*g1a*cu20*a12m2*cv10-a22zm1*a11*a22zm2*a11m2*a22*cv10*
     & cu10*tdu10-4*a22zm1*a11*a22zm2*a11m2*cv20*a22*cu10*tdu10+4*
     & a22zm1*a11*a22zm2*cv20*a21*a12m1*cu20*tdu20-16*a21zm2*a12*
     & tdu01*a21*a12m1*cu20*a12m2*cv02+16*a21zm2*a12*tdu01*a11m1*a22*
     & cu20*a12m2*cv02+4*a21zm2*a12*a11m1*a22*cu20*a12m2*cv01*tdu01-
     & a21zm2*a12*a21*a12m2*cv01*tdu01*cu10*a12m1-4*a21zm2*a12*a21*
     & a12m1*cu20*a12m2*cv01*tdu01-4*a21zm2*a12*tdu01*a21*a12m2*cv02*
     & cu10*a12m1+a22zm1*a21zm2*a12*a11m1*g1a*cu20*a12m2*cv10+a22zm1*
     & a21zm2*a12*a11m1*a22*cv10*cu20*tdu20+4*a22zm1*a21zm2*a12*a11m1*
     & cv20*a22*cu20*tdu20+a21zm2*a12*a11m2*a22*cv01*tdu01*cu10*a12m1+
     & 4*a21zm2*a12*tdu01*a11m2*a22*cv02*cu10*a12m1-a22zm1*a21zm2*g2a*
     & a11m2*a22*cv02*cu10*a12m1+4*a22zm1*a21zm2*a12*a11m2*a11m1*cv20*
     & a22*gLu-4*a22zm1*a21zm2*g2a*a11m1*a22*cu20*a12m2*cv02-4*a22zm1*
     & a21zm2*a12*a11m2*cv20*a21*a12m1*gLu+a22zm1*a21zm2*a12*a11m2*
     & a11m1*a22*cv10*gLu-a22zm1*a21zm2*a12*a21*a12m2*cv10*cu10*tdu10-
     & a22zm1*a21zm2*a12*a11m2*cv20*g1a*cu10*a12m1-4*a22zm1*a21zm2*
     & a12*a21*a12m1*cu20*a12m2*gLv+4*a22zm1*a21zm2*a12*a11m2*cv20*
     & a22*cu10*tdu10+a22zm1*a21zm2*a12*a11m2*a22*cv10*cu10*tdu10+4*
     & a22zm1*a21zm2*a12*a11m1*a22*cu20*a12m2*gLv-4*a22zm1*a21zm2*a12*
     & a21*tdu10*cu20*a12m2*cv10+a22zm1*a21zm2*a12*a11m2*a22*gLv*cu10*
     & a12m1-a22zm1*a21zm2*a12*cv20*a21*tdu20*cu10*a12m1-4*a22zm1*
     & a21zm2*a12*cv20*a21*a12m1*cu20*tdu20+4*a22zm1*a21zm2*g2a*a21*
     & a12m1*cu20*a12m2*cv02-a22zm1*a21zm2*a12*a21*a12m2*gLv*cu10*
     & a12m1+a22zm1*a21zm2*g2a*a21*a12m2*cv02*cu10*a12m1-a22zm1*
     & a21zm2*a12*a11m1*a21*a12m2*cv10*gLu+a11*a22zm2*a21*a12m2*cv01*
     & tdu01*cu10*a12m1-4*a11*a22zm2*a11m1*a22*cu20*a12m2*cv01*tdu01-
     & 16*a12*tdu01*a11m2*a11m1*cv20*a22*cu02*a22zm2+16*a12*tdu01*
     & a11m2*cv20*a21*a12m1*cu02*a22zm2-4*a12*tdu01*a11m2*a11m1*a22*
     & cv10*cu02*a22zm2+4*a12*tdu01*a11m1*a21*a12m2*cv10*cu02*a22zm2+
     & 4*a11*a22zm2*a21*a12m1*cu20*a12m2*cv01*tdu01-a22zm1*g2a*a11m1*
     & a21*a12m2*cv10*cu02*a22zm2-a11*a22zm2*a11m2*a22*cv01*tdu01*
     & cu10*a12m1+a22zm1*a11*a22zm2*a21*a12m2*cv10*cu10*tdu10+a22zm1*
     & g2a*a11m2*a11m1*a22*cv10*cu02*a22zm2+4*a22zm1*a11*a22zm2*a21*
     & tdu10*cu20*a12m2*cv10-a22zm1*a12*a11m1*a21*a12m2*cv10*cu02*
     & tdu02+a22zm1*a12*a11m2*a11m1*a22*cv10*cu02*tdu02+4*a22zm1*a12*
     & a11m2*a11m1*cv20*a22*cu02*tdu02-a22zm1*a11*tdu02*a11m2*a22*
     & cv02*cu10*a12m1-4*a22zm1*a12*a11m2*cv20*a21*a12m1*cu02*tdu02+4*
     & a22zm1*a11*tdu02*a21*a12m1*cu20*a12m2*cv02-4*a22zm1*a11*tdu02*
     & a11m1*a22*cu20*a12m2*cv02+a22zm1*a11*tdu02*a21*a12m2*cv02*cu10*
     & a12m1+4*a22zm1*g2a*a11m2*a11m1*cv20*a22*cu02*a22zm2)/det

      u(i1,i2-is2,i3,ey)=(-4*a21zm1*a11*a22zm2*a21*a12m1*cu20*a12m2*
     & gLv+a21zm1*g2a*a11m1*a21*a12m2*cv10*cu02*a22zm2+4*tdu01*a21*
     & a12m2*cv02*a21zm2*a11*cu10*a12m1-a21zm1*a21zm2*g2a*a21*a12m2*
     & cv02*cu10*a12m1-a21zm1*a11*tdu02*a21*a12m2*cv02*cu10*a12m1+4*
     & a21zm1*a11*tdu02*a11m1*a22*cu20*a12m2*cv02+a11m1*a21*a12m2*
     & cv10*cu02*tdu02*a12*a21zm1-4*a21zm1*a11*tdu02*a21*a12m1*cu20*
     & a12m2*cv02+a21zm1*a11*tdu02*a11m2*a22*cv02*cu10*a12m1+4*a11m2*
     & cv20*a21*a12m1*cu02*tdu02*a12*a21zm1-a11m2*a11m1*a22*cv10*cu02*
     & tdu02*a12*a21zm1-4*a11m2*a11m1*cv20*a22*cu02*tdu02*a12*a21zm1-
     & 16*tdu01*a11m2*cv20*a21*a12m1*cu02*a22zm2*a11-a21zm1*a11*
     & a22zm2*a21*a12m2*cv10*cu10*tdu10+tdu01*a11m2*a11m1*a22*cv10*
     & cu01*a11*a22zm2-4*tdu01*a11m2*cv20*a21*a12m1*cu01*a11*a22zm2+4*
     & tdu01*a11m2*a11m1*a22*cv10*cu02*a22zm2*a11-4*a21zm1*a11*a22zm2*
     & cv20*a21*a12m1*cu20*tdu20+a21zm1*a11*a22zm2*a11m1*a22*cv10*
     & cu20*tdu20+4*a21zm1*a11*a22zm2*a11m1*cv20*a22*cu20*tdu20-
     & a21zm1*a11*a22zm2*cv20*a21*tdu20*cu10*a12m1-4*a21zm1*a11*
     & a22zm2*a21*tdu10*cu20*a12m2*cv10+16*tdu01*a11m2*a11m1*cv20*a22*
     & cu02*a22zm2*a11+4*tdu01*a11m2*a11m1*cv20*a22*cu01*a11*a22zm2+
     & a21zm1*a11*a22zm2*a11m2*a22*gLv*cu10*a12m1+4*a21zm1*a11*a22zm2*
     & a11m2*a11m1*cv20*a22*gLu-a21zm1*g2a*a11m2*a11m1*a22*cv10*cu02*
     & a22zm2-4*a21zm1*a11*a22zm2*a11m2*cv20*a21*a12m1*gLu-4*a21zm1*
     & g2a*a11m2*a11m1*cv20*a22*cu02*a22zm2+a21zm1*a11*a22zm2*a11m2*
     & a11m1*a22*cv10*gLu+4*a21zm1*g2a*a11m2*cv20*a21*a12m1*cu02*
     & a22zm2+4*a21zm1*a11*a22zm2*a11m1*a22*cu20*a12m2*gLv+a21zm1*a11*
     & a22zm2*a11m1*g1a*cu20*a12m2*cv10+16*tdu01*a21*a12m1*a21zm2*
     & cu20*a12m2*cv02*a11+a11m1*a21*a12m2*cv10*cu01*a21zm2*a12*tdu01-
     & a21zm1*a11*a22zm2*a21*a12m2*gLv*cu10*a12m1-4*tdu01*a11m2*a22*
     & cv02*a21zm2*a11*cu10*a12m1+4*a11m2*cv20*a21*a12m1*cu01*a21zm2*
     & a12*tdu01-16*tdu01*a11m1*a22*a21zm2*cu20*a12m2*cv02*a11+a21zm1*
     & a11*a22zm2*a11m2*a22*cv10*cu10*tdu10-a21zm1*a11*a22zm2*a11m1*
     & a21*a12m2*cv10*gLu+4*a21zm1*a11*a22zm2*a11m2*cv20*a22*cu10*
     & tdu10-a21zm1*a11*a22zm2*a11m2*cv20*g1a*cu10*a12m1+a21*a12m2*
     & gLv*a21zm2*a12*a21zm1*cu10*a12m1-4*a11m2*a11m1*cv20*a22*cu01*
     & a21zm2*a12*tdu01-a11m2*a11m1*a22*cv10*cu01*a21zm2*a12*tdu01-
     & a11m1*a22*cv10*a21zm2*cu20*tdu20*a12*a21zm1+4*a21*tdu10*a21zm2*
     & a12*a21zm1*cu20*a12m2*cv10+a21*a12m2*cv10*a21zm2*cu10*tdu10*
     & a12*a21zm1-a11m2*a11m1*a22*cv10*a21zm2*gLu*a12*a21zm1-4*a11m2*
     & a11m1*cv20*a22*a21zm2*gLu*a12*a21zm1+cv20*a21*tdu20*a21zm2*a12*
     & a21zm1*cu10*a12m1+4*cv20*a21*a12m1*a21zm2*cu20*tdu20*a12*
     & a21zm1-4*a11m1*cv20*a22*a21zm2*cu20*tdu20*a12*a21zm1+4*a21zm1*
     & a21zm2*g2a*a11m1*a22*cu20*a12m2*cv02+4*a11m2*cv20*a21*a12m1*
     & a21zm2*gLu*a12*a21zm1-a11m2*a22*gLv*a21zm2*a12*a21zm1*cu10*
     & a12m1-4*a11m1*a22*a21zm2*cu20*a12m2*gLv*a12*a21zm1-4*a11m2*
     & cv20*a22*a21zm2*cu10*tdu10*a12*a21zm1+a11m2*cv20*g1a*a21zm2*
     & a12*a21zm1*cu10*a12m1+a21zm1*a21zm2*g2a*a11m2*a22*cv02*cu10*
     & a12m1-a11m1*g1a*a21zm2*a12*a21zm1*cu20*a12m2*cv10-a11m2*a22*
     & cv10*a21zm2*cu10*tdu10*a12*a21zm1+4*a21*a12m1*a21zm2*cu20*
     & a12m2*gLv*a12*a21zm1-4*a21zm1*a21zm2*g2a*a21*a12m1*cu20*a12m2*
     & cv02+a11m1*a21*a12m2*cv10*a21zm2*gLu*a12*a21zm1-4*tdu01*a11m1*
     & a21*a12m2*cv10*cu02*a22zm2*a11-tdu01*a11m1*a21*a12m2*cv10*cu01*
     & a11*a22zm2)/det

      u(i1-2*is1,i2,i3,ex)=(-4*a12m2*a11m1*cu02*tdu02*a22*cv01*a21zm1*
     & a12-16*a12m2*a11m1*cu02*tdu02*a12*a21zm1*a22*cv02+16*a12m1*
     & cu02*tdu02*a12*a21zm1*a21*a12m2*cv02+4*a12m1*cu02*tdu02*a21*
     & a12m2*cv01*a21zm1*a12+4*a12m2*a22zm1*a11*a11m1*cu01*tdu02*a22*
     & cv02+16*a12m2*a22zm1*a11*a11m1*cu02*tdu02*a22*cv02-16*a12m1*
     & a22zm1*a11*cu02*tdu02*a21*a12m2*cv02-4*a12m1*a22zm1*a11*cu01*
     & tdu02*a21*a12m2*cv02+4*a12m1*cu01*a21zm2*a12*a21*a12m2*cv01*
     & tdu01+16*a12m1*cu01*a21zm2*a12*tdu01*a21*a12m2*cv02-4*a12m2*
     & a11m1*a21zm2*gLu*a22*cv01*a21zm1*a12-16*a12m2*a11m1*a21zm2*gLu*
     & a12*a21zm1*a22*cv02+4*a12m2*cv02*g1a*a21zm2*a12*a21zm1*cu10*
     & a12m1-4*a12m2*cu10*a21zm2*tdu10*a22*cv01*a21zm1*a12+a12m2*cv01*
     & a21zm1*a21zm2*a12*g1a*cu10*a12m1+16*a12m2*a22zm1*a11*a21zm2*
     & tdu10*a22*cv02*cu10-4*a12m2*a22zm1*a11m1*cu01*a21zm2*a12*a22*
     & gLv-4*a12m2*a22zm1*cv02*g1a*a21zm2*a11*cu10*a12m1-a12m2*a22zm1*
     & cv10*a11m1*cu01*a21zm2*a12*g1a+16*a12m2*a22zm1*a11*a11m1*
     & a21zm2*gLu*a22*cv02-16*tdu10*a21*a12m2*cv10*a22zm1*cu02*a22zm2*
     & a11-4*a12m1*a22zm1*a11*cu01*a22zm2*a21*a12m2*gLv-16*a12m1*
     & a22zm1*a11*cu02*a22zm2*a21*a12m2*gLv+16*tdu20*a11m1*cv20*a22*
     & a22zm1*cu02*a22zm2*a11+16*a12m1*cu02*a22zm2*a12*a21zm1*a21*
     & tdu20*cv20-4*a12m1*a11*gLu*a21*a12m2*cv01*a21zm1*a22zm2-16*
     & a12m2*a11m1*cu02*a22zm2*a12*a21zm1*a22*gLv-4*a12m2*a11m1*cu02*
     & a22zm2*a22*cv01*a21zm1*g2a+4*a12m2*a11*a11m1*gLu*a22*cv01*
     & a21zm1*a22zm2-4*a12m2*cv10*a11m1*cu02*a22zm2*a12*a21zm1*g1a+4*
     & a12m1*cu02*a22zm2*a21*a12m2*cv01*a21zm1*g2a-tdu20*a22*cv01*
     & a21zm1*a11*a22zm2*cu10*a12m1-a12m2*cv01*a21zm1*a11*a22zm2*g1a*
     & cu10*a12m1-16*tdu20*a11m1*cv20*a22*cu02*a22zm2*a12*a21zm1-16*
     & a12m1*a11*cu02*a22zm2*a21*a12m2*cv01*tdu01+4*a12m2*a11*a11m1*
     & cu01*a22zm2*a22*cv01*tdu01-4*tdu20*a22zm1*a22*cv02*a21zm2*a11*
     & cu10*a12m1+4*a12m2*a22zm1*a11m1*cu01*a21zm2*g2a*a22*cv02-16*
     & a12m2*a11m1*cu01*a21zm2*a12*tdu01*a22*cv02-4*a12m2*a11m1*cu01*
     & a21zm2*a12*a22*cv01*tdu01+4*a12m1*a22zm1*cu01*a21zm2*a12*a21*
     & a12m2*gLv-16*a12m1*a22zm1*a11*a21zm2*gLu*a21*a12m2*cv02+4*
     & a12m1*a22zm1*cu01*a21zm2*a12*a21*tdu20*cv20-4*a12m1*a22zm1*
     & cu01*a21zm2*g2a*a21*a12m2*cv02-4*tdu20*a11m1*cv20*a22*a22zm1*
     & cu01*a21zm2*a12-tdu20*a11m1*a22*cv10*a22zm1*cu01*a21zm2*a12+4*
     & tdu10*a21*a12m2*cv10*a22zm1*cu01*a21zm2*a12-4*tdu20*a11m1*a22*
     & cv10*cu02*a22zm2*a12*a21zm1-4*a12m1*a11*cu01*a22zm2*a21*a12m2*
     & cv01*tdu01+16*a12m2*a11*a11m1*cu02*a22zm2*a22*cv01*tdu01+4*
     & a12m2*a11*tdu10*a22*cv01*a21zm1*a22zm2*cu10+16*a12m1*cu02*
     & a22zm2*a12*a21zm1*a21*a12m2*gLv+16*tdu10*a21*a12m2*cv10*cu02*
     & a22zm2*a12*a21zm1-4*tdu10*a21*a12m2*cv10*a22zm1*cu01*a11*
     & a22zm2+16*a12m2*a22zm1*a11*a11m1*cu02*a22zm2*a22*gLv+4*a12m2*
     & a22zm1*cv10*a11*a11m1*cu02*a22zm2*g1a+4*a12m2*a22zm1*a11*a11m1*
     & cu01*a22zm2*a22*gLv+a12m2*a22zm1*cv10*a11*a11m1*cu01*a22zm2*
     & g1a-4*a12m1*a22zm1*a11*cv20*cu01*a22zm2*a21*tdu20-16*a12m1*
     & a22zm1*a11*cv20*cu02*a22zm2*a21*tdu20+4*tdu20*a11m1*cv20*a22*
     & a22zm1*cu01*a11*a22zm2+tdu20*a11m1*a22*cv10*a22zm1*cu01*a11*
     & a22zm2+4*tdu20*a11m1*a22*cv10*a22zm1*cu02*a22zm2*a11-16*a12m2*
     & cu10*a21zm2*tdu10*a12*a21zm1*a22*cv02+tdu20*a22*cv01*a21zm1*
     & a21zm2*a12*cu10*a12m1+16*a12m1*a21zm2*gLu*a12*a21zm1*a21*a12m2*
     & cv02+4*a12m1*a21zm2*gLu*a21*a12m2*cv01*a21zm1*a12+4*tdu20*a22*
     & cv02*a21zm2*a12*a21zm1*cu10*a12m1)/det

      u(i1-2*is1,i2,i3,ey)=(4*a22zm1*cv02*a11m2*a21*a12m1*cu01*a21zm2*
     & g2a+16*a22zm1*cv02*a11m2*a21*a12m1*cu02*tdu02*a11+4*a22zm1*
     & cv02*a11m2*a21*a12m1*cu01*a11*tdu02-16*a22zm1*a11m2*a11*a11m1*
     & cu02*tdu02*a22*cv02-4*cv01*a21zm1*a12*a11m2*a21*a12m1*cu02*
     & tdu02-16*cv02*a11m2*a21*a12m1*cu02*tdu02*a12*a21zm1-4*a22zm1*
     & a11m2*a11*a11m1*cu01*tdu02*a22*cv02+16*a11m2*a11m1*cu02*tdu02*
     & a12*a21zm1*a22*cv02+4*a11m2*a11m1*cu02*tdu02*a22*cv01*a21zm1*
     & a12+16*a11m2*a11m1*a21zm2*gLu*a12*a21zm1*a22*cv02+16*a11m2*
     & cu10*a21zm2*tdu10*a12*a21zm1*a22*cv02-4*cv02*a11m2*g1a*a21zm2*
     & a12*a21zm1*cu10*a12m1+16*a11m1*a21zm2*cu20*tdu20*a12*a21zm1*
     & a22*cv02-4*cv01*a21zm1*a21zm2*a12*a21*a12m1*cu20*tdu20+16*cv01*
     & tdu01*a11m2*a21*a12m1*cu02*a22zm2*a11+4*cv01*tdu01*a11m2*a21*
     & a12m1*cu01*a11*a22zm2-16*a11m2*a11*a11m1*cu02*a22zm2*a22*cv01*
     & tdu01-4*a11m2*a11*a11m1*cu01*a22zm2*a22*cv01*tdu01+4*a22zm1*
     & cv02*a11m2*g1a*a21zm2*a11*cu10*a12m1-4*a22zm1*gLv*a11m2*a21*
     & a12m1*cu01*a21zm2*a12+16*a22zm1*cv02*a11m2*a21*a12m1*a21zm2*
     & gLu*a11-cv01*a21zm1*a21zm2*a12*a21*tdu20*cu10*a12m1+4*a11m1*
     & a21zm2*cu20*tdu20*a22*cv01*a21zm1*a12-4*cv02*a21*tdu20*a21zm2*
     & a12*a21zm1*cu10*a12m1-16*cv02*a21*a12m1*a21zm2*cu20*tdu20*a12*
     & a21zm1+16*a22zm1*cv02*a21*a12m1*a21zm2*cu20*tdu20*a11+4*a11m2*
     & cu10*a21zm2*tdu10*a22*cv01*a21zm1*a12-16*a22zm1*a11*a11m1*
     & a21zm2*cu20*tdu20*a22*cv02+a22zm1*cv10*a11m1*cu01*a21zm2*a12*
     & a21*tdu20+4*a11m2*a11m1*a21zm2*gLu*a22*cv01*a21zm1*a12-16*cv02*
     & a11m2*a21*a12m1*a21zm2*gLu*a12*a21zm1-4*cv01*a21zm1*a21zm2*a12*
     & a11m2*a21*a12m1*gLu-cv01*a21zm1*a21zm2*a12*a11m2*g1a*cu10*
     & a12m1+4*a22zm1*cv02*a21*tdu20*a21zm2*a11*cu10*a12m1-16*a22zm1*
     & a11m2*a11*a21zm2*tdu10*a22*cv02*cu10+16*a11m2*a11m1*cu01*
     & a21zm2*a12*tdu01*a22*cv02+4*a22zm1*a11m2*a11m1*cu01*a21zm2*a12*
     & a22*gLv-16*a22zm1*a11m2*a11*a11m1*a21zm2*gLu*a22*cv02-4*a22zm1*
     & a11m2*a11m1*cu01*a21zm2*g2a*a22*cv02+a22zm1*cv10*a11m2*a11m1*
     & cu01*a21zm2*a12*g1a-4*a22zm1*cv10*a11m2*a21zm2*tdu10*a21*cu01*
     & a12-16*cv02*a11m2*a21*a12m1*cu01*a21zm2*a12*tdu01-4*cv01*tdu01*
     & a11m2*a21*a12m1*cu01*a21zm2*a12+4*a11m2*a11m1*cu01*a21zm2*a12*
     & a22*cv01*tdu01-4*a11m2*a11*tdu10*a22*cv01*a21zm1*a22zm2*cu10+4*
     & cv01*a21zm1*a11*a22zm2*a21*a12m1*cu20*tdu20+4*cv10*a11m1*cu02*
     & a22zm2*a12*a21zm1*a21*tdu20-4*a11*a11m1*cu20*tdu20*a22*cv01*
     & a21zm1*a22zm2+cv01*a21zm1*a11*a22zm2*a21*tdu20*cu10*a12m1-16*
     & a22zm1*a11m2*a11*a11m1*cu02*a22zm2*a22*gLv-4*a22zm1*cv10*a11m2*
     & a11*a11m1*cu02*a22zm2*g1a-a22zm1*cv10*a11m2*a11*a11m1*cu01*
     & a22zm2*g1a+16*a22zm1*cv10*a11m2*a11*tdu10*a21*cu02*a22zm2+4*
     & a22zm1*cv10*a11m2*a11*tdu10*a21*cu01*a22zm2+4*a22zm1*gLv*a11m2*
     & a21*a12m1*cu01*a11*a22zm2+4*cv10*a11m2*a11m1*cu02*a22zm2*a12*
     & a21zm1*g1a+4*a11m2*a11m1*cu02*a22zm2*a22*cv01*a21zm1*g2a-16*
     & cv10*a11m2*tdu10*a12*a21zm1*a21*cu02*a22zm2-4*a22zm1*cv10*a11*
     & a11m1*cu02*a22zm2*a21*tdu20-a22zm1*cv10*a11*a11m1*cu01*a22zm2*
     & a21*tdu20+4*cv01*a21zm1*a11*a22zm2*a11m2*a21*a12m1*gLu+cv01*
     & a21zm1*a11*a22zm2*a11m2*g1a*cu10*a12m1-4*cv01*a21zm1*g2a*a11m2*
     & a21*a12m1*cu02*a22zm2-16*gLv*a11m2*a21*a12m1*cu02*a22zm2*a12*
     & a21zm1-4*a22zm1*a11m2*a11*a11m1*cu01*a22zm2*a22*gLv+16*a22zm1*
     & gLv*a11m2*a21*a12m1*cu02*a22zm2*a11+16*a11m2*a11m1*cu02*a22zm2*
     & a12*a21zm1*a22*gLv-4*a11m2*a11*a11m1*gLu*a22*cv01*a21zm1*
     & a22zm2)/det

      u(i1,i2-2*is2,i3,ex)=(-16*a22zm1*a11*a22zm2*a11m2*cv20*a22*cu10*
     & tdu10-4*a22zm1*a11*a22zm2*a11m2*a22*cv10*cu10*tdu10+a22zm2*
     & a11m1*a21*a12m2*cv10*a22zm1*cu01*g2a-4*a22zm1*a11*a22zm2*a11m2*
     & a11m1*a22*cv10*gLu+4*a22zm2*a11m1*g1a*a12*a21zm1*cu20*a12m2*
     & cv10-4*a22zm2*a11m1*a21*a12m2*cv10*gLu*a12*a21zm1+16*a22zm2*
     & a11m2*a11m1*cv20*a22*cu01*a12*tdu01+4*a22zm2*a11m2*a11m1*a22*
     & cv10*cu01*a12*tdu01-16*a22zm2*a11m2*cv20*a21*a12m1*cu01*a12*
     & tdu01-4*a11*a22zm2*a11m2*a22*cv01*tdu01*cu10*a12m1-16*a11*
     & a22zm2*a11m1*a22*cu20*a12m2*cv01*tdu01+16*a11*a22zm2*a21*a12m1*
     & cu20*a12m2*cv01*tdu01+4*a11*a22zm2*a21*a12m2*cv01*tdu01*cu10*
     & a12m1-4*a22zm2*a11m1*a21*a12m2*cv10*cu01*a12*tdu01-4*a22zm1*
     & a11*a22zm2*a11m1*a22*cv10*cu20*tdu20-16*a22zm2*a11m2*cv20*a21*
     & a12m1*gLu*a12*a21zm1+16*a22zm2*a11m1*a22*cu20*a12m2*gLv*a12*
     & a21zm1-16*a22zm1*a11*a22zm2*a11m2*a11m1*cv20*a22*gLu+4*a22zm1*
     & a11*a22zm2*a11m1*a21*a12m2*cv10*gLu+16*a22zm1*a11*a22zm2*a21*
     & tdu10*cu20*a12m2*cv10+4*a22zm1*a11*a22zm2*a21*a12m2*cv10*cu10*
     & tdu10+4*a22zm1*a11*a22zm2*a21*a12m2*gLv*cu10*a12m1+16*a22zm1*
     & a11*a22zm2*a21*a12m1*cu20*a12m2*gLv-16*a22zm1*a11*a22zm2*a11m1*
     & a22*cu20*a12m2*gLv-4*a22zm1*a11*a22zm2*a11m1*g1a*cu20*a12m2*
     & cv10+16*a22zm1*a11*a22zm2*a11m2*cv20*a21*a12m1*gLu+4*a22zm1*
     & a11*a22zm2*a11m2*cv20*g1a*cu10*a12m1-4*a22zm1*a11*a22zm2*a11m2*
     & a22*gLv*cu10*a12m1-a22zm2*a11m2*a11m1*a22*cv10*a22zm1*cu01*g2a-
     & 4*a22zm2*a11m2*a11m1*cv20*a22*a22zm1*cu01*g2a+4*a22zm2*a11m2*
     & cv20*a21*a12m1*a22zm1*cu01*g2a+4*a22zm2*a11m1*a22*cv10*cu20*
     & tdu20*a12*a21zm1+16*a22zm2*a11m1*cv20*a22*cu20*tdu20*a12*
     & a21zm1-4*a22zm2*cv20*a21*tdu20*a12*a21zm1*cu10*a12m1-16*a22zm2*
     & cv20*a21*a12m1*cu20*tdu20*a12*a21zm1-4*a22zm2*a21*a12m2*gLv*
     & a12*a21zm1*cu10*a12m1-16*a22zm2*a21*tdu10*a12*a21zm1*cu20*
     & a12m2*cv10+4*a22zm2*a11m1*a22*cu20*a12m2*cv01*a21zm1*g2a-4*
     & a22zm2*a21*a12m1*cu20*a12m2*cv01*a21zm1*g2a-16*a22zm2*a21*
     & a12m1*cu20*a12m2*gLv*a12*a21zm1-4*a22zm2*a21*a12m2*cv10*cu10*
     & tdu10*a12*a21zm1+4*a22zm2*a11m2*a22*cv10*cu10*tdu10*a12*a21zm1+
     & 16*a22zm2*a11m2*cv20*a22*cu10*tdu10*a12*a21zm1-a22zm2*a21*
     & a12m2*cv01*a21zm1*g2a*cu10*a12m1+16*a22zm1*a11*a22zm2*cv20*a21*
     & a12m1*cu20*tdu20+4*a22zm1*a11*a22zm2*cv20*a21*tdu20*cu10*a12m1-
     & 16*a22zm1*a11*a22zm2*a11m1*cv20*a22*cu20*tdu20+4*a22zm2*a11m2*
     & a11m1*a22*cv10*gLu*a12*a21zm1-4*a22zm2*a11m2*cv20*g1a*a12*
     & a21zm1*cu10*a12m1+16*a22zm2*a11m2*a11m1*cv20*a22*gLu*a12*
     & a21zm1+a22zm2*a11m2*a22*cv01*a21zm1*g2a*cu10*a12m1+4*a22zm2*
     & a11m2*a22*gLv*a12*a21zm1*cu10*a12m1-4*tdu02*a11m2*a11m1*cv20*
     & a22*a22zm1*cu01*a12+4*tdu02*a11m2*cv20*a21*a12m1*a22zm1*cu01*
     & a12-tdu02*a11m2*a11m1*a22*cv10*a22zm1*cu01*a12-4*a22zm1*a11*
     & tdu02*a11m2*a22*cv02*cu10*a12m1+tdu02*a11m1*a21*a12m2*cv10*
     & a22zm1*cu01*a12-16*a22zm1*a11*tdu02*a11m1*a22*cu20*a12m2*cv02+
     & 4*a22zm1*a11*tdu02*a21*a12m2*cv02*cu10*a12m1+16*a22zm1*a11*
     & tdu02*a21*a12m1*cu20*a12m2*cv02+4*tdu02*a11m2*a22*cv02*a12*
     & a21zm1*cu10*a12m1+tdu02*a11m2*a22*cv01*a21zm1*a12*cu10*a12m1+
     & 16*tdu02*a11m1*a22*cu20*a12m2*cv02*a12*a21zm1+4*tdu02*a11m1*
     & a22*cu20*a12m2*cv01*a21zm1*a12-4*tdu02*a21*a12m2*cv02*a12*
     & a21zm1*cu10*a12m1-tdu02*a21*a12m2*cv01*a21zm1*a12*cu10*a12m1-4*
     & tdu02*a21*a12m1*cu20*a12m2*cv01*a21zm1*a12-16*tdu02*a21*a12m1*
     & cu20*a12m2*cv02*a12*a21zm1)/det

      u(i1,i2-2*is2,i3,ey)=(-16*a11m2*a11m1*cv20*a22*cu01*a21zm2*a12*
     & tdu01+4*a11m2*a22*cv01*tdu01*a21zm2*a11*cu10*a12m1-4*a21*a12m2*
     & cv01*tdu01*a21zm2*a11*cu10*a12m1-a11m1*a21*a12m2*cv10*a22zm1*
     & cu01*a11*tdu02-4*a11m1*a21*a12m2*cv10*a22zm1*cu02*tdu02*a11-16*
     & a11m2*cv20*a21*a12m1*a22zm1*cu02*tdu02*a11+a11m2*a11m1*a22*
     & cv10*a22zm1*cu01*a11*tdu02+16*a11m2*a11m1*cv20*a22*a22zm1*cu02*
     & tdu02*a11+4*a11m2*a11m1*a22*cv10*a22zm1*cu02*tdu02*a11+4*a11m2*
     & a11m1*cv20*a22*a22zm1*cu01*a11*tdu02-4*a11m2*cv20*a21*a12m1*
     & a22zm1*cu01*a11*tdu02-4*a11m1*a22*cu20*a12m2*cv01*a21zm1*a11*
     & tdu02+4*a11m1*a21*a12m2*cv10*cu02*tdu02*a12*a21zm1+a21*a12m2*
     & cv01*a21zm1*a11*tdu02*cu10*a12m1+4*a21*a12m1*cu20*a12m2*cv01*
     & a21zm1*a11*tdu02+16*a11m2*cv20*a21*a12m1*cu02*tdu02*a12*a21zm1-
     & 4*a11m2*a11m1*a22*cv10*cu02*tdu02*a12*a21zm1-a11m2*a22*cv01*
     & a21zm1*a11*tdu02*cu10*a12m1-16*a11m2*a11m1*cv20*a22*cu02*tdu02*
     & a12*a21zm1+4*a21*a12m2*cv10*a21zm2*cu10*tdu10*a12*a21zm1+16*
     & a21*tdu10*a21zm2*a12*a21zm1*cu20*a12m2*cv10-a11m2*a22*cv01*
     & a21zm1*a21zm2*g2a*cu10*a12m1-16*a11m1*a22*a21zm2*cu20*a12m2*
     & gLv*a12*a21zm1+4*a11m1*a21*a12m2*cv10*a21zm2*gLu*a12*a21zm1-4*
     & a11m1*g1a*a21zm2*a12*a21zm1*cu20*a12m2*cv10-4*a11m1*a22*cu20*
     & a12m2*cv01*a21zm1*a21zm2*g2a+16*a21*a12m1*a21zm2*cu20*a12m2*
     & gLv*a12*a21zm1+4*a21*a12m2*gLv*a21zm2*a12*a21zm1*cu10*a12m1+16*
     & cv20*a21*a12m1*a21zm2*cu20*tdu20*a12*a21zm1+a21*a12m2*cv01*
     & a21zm1*a21zm2*g2a*cu10*a12m1+4*a21*a12m1*cu20*a12m2*cv01*
     & a21zm1*a21zm2*g2a-4*a11m1*a22*cv10*a21zm2*cu20*tdu20*a12*
     & a21zm1-16*a11m1*cv20*a22*a21zm2*cu20*tdu20*a12*a21zm1+4*cv20*
     & a21*tdu20*a21zm2*a12*a21zm1*cu10*a12m1+4*a11m2*a22zm1*a22*gLv*
     & a21zm2*a11*cu10*a12m1+a11m2*a11m1*a22*cv10*a22zm1*cu01*a21zm2*
     & g2a+16*a11m2*cv20*a22*a22zm1*a21zm2*cu10*tdu10*a11-4*a11m2*
     & cv20*a22zm1*g1a*a21zm2*a11*cu10*a12m1-4*a11m2*cv20*a21*a12m1*
     & a22zm1*cu01*a21zm2*g2a-4*a21*a12m2*cv10*a22zm1*a21zm2*cu10*
     & tdu10*a11-16*a22zm1*a21*tdu10*a21zm2*a11*cu20*a12m2*cv10+16*
     & a11m2*a11m1*cv20*a22*a22zm1*a21zm2*gLu*a11-4*a22zm1*a21*a12m2*
     & gLv*a21zm2*a11*cu10*a12m1+4*a11m2*a11m1*cv20*a22*a22zm1*cu01*
     & a21zm2*g2a+4*a11m2*a22*cv10*a22zm1*a21zm2*cu10*tdu10*a11+4*
     & a11m2*a11m1*a22*cv10*a22zm1*a21zm2*gLu*a11-a11m1*a21*a12m2*
     & cv10*a22zm1*cu01*a21zm2*g2a+4*a11m1*a22zm1*g1a*a21zm2*a11*cu20*
     & a12m2*cv10-4*a11m1*a21*a12m2*cv10*a22zm1*a21zm2*gLu*a11-16*
     & a11m2*cv20*a22*a21zm2*cu10*tdu10*a12*a21zm1+16*a11m2*cv20*a21*
     & a12m1*a21zm2*gLu*a12*a21zm1+4*a11m1*a22*cv10*a22zm1*a21zm2*
     & cu20*tdu20*a11+16*a11m1*cv20*a22*a22zm1*a21zm2*cu20*tdu20*a11-
     & 4*cv20*a22zm1*a21*tdu20*a21zm2*a11*cu10*a12m1+4*a11m2*cv20*g1a*
     & a21zm2*a12*a21zm1*cu10*a12m1-16*cv20*a21*a12m1*a22zm1*a21zm2*
     & cu20*tdu20*a11+16*a11m1*a22*a22zm1*a21zm2*cu20*a12m2*gLv*a11-
     & 16*a21*a12m1*a22zm1*a21zm2*cu20*a12m2*gLv*a11-16*a11m2*cv20*
     & a21*a12m1*a22zm1*a21zm2*gLu*a11-16*a21*a12m1*a21zm2*cu20*a12m2*
     & cv01*tdu01*a11+16*a11m1*a22*a21zm2*cu20*a12m2*cv01*tdu01*a11+4*
     & a11m1*a21*a12m2*cv10*cu01*a21zm2*a12*tdu01-4*a11m2*a11m1*a22*
     & cv10*a21zm2*gLu*a12*a21zm1-16*a11m2*a11m1*cv20*a22*a21zm2*gLu*
     & a12*a21zm1-4*a11m2*a22*cv10*a21zm2*cu10*tdu10*a12*a21zm1-4*
     & a11m2*a22*gLv*a21zm2*a12*a21zm1*cu10*a12m1+16*a11m2*cv20*a21*
     & a12m1*cu01*a21zm2*a12*tdu01-4*a11m2*a11m1*a22*cv10*cu01*a21zm2*
     & a12*tdu01)/det



! ****************** done fourth-order ********************
                      ! The next file is from bc4c.maple
! ****************** Start Hz extended fourth-order ********************
!  Solve:  wr=fw1  
!          ws=fw2  
!          c11*wrrr+(c1+c11r)*wrr + c22r*wss=fw3, (i.e. (Lw).r=0 )
!          c22*wsss+(c2+c22s)*wss + c11s*wrr=fw4, (i.e. (Lw).s=0 )

      u(i1,i2-2*is2,i3,hz) = (12*fw2*dsa**2*dra**3*c11r*c2+12*fw2*dsa**
     & 2*dra**3*c11r*c22s+12*fw2*dsa**2*dra**3*c1*c2+12*fw2*dsa**2*
     & dra**3*c1*c22s-12*fw2*dsa**2*dra**3*c11s*c22r-36*fw2*dsa**2*
     & dra**2*c11*c22s-36*fw2*dsa**2*dra**2*c11*c2-3*u(i1,i2+2*is2,i3,
     & hz)*dra**2*c11*dsa*c22s+9*u(i1,i2+2*is2,i3,hz)*dra**2*c11*c22-
     & 3*u(i1,i2+2*is2,i3,hz)*dra**3*c11r*c22-3*u(i1,i2+2*is2,i3,hz)*
     & dra**3*c1*c22+u(i1,i2+2*is2,i3,hz)*dra**3*c11r*dsa*c2+u(i1,i2+
     & 2*is2,i3,hz)*dra**3*c11r*dsa*c22s+u(i1,i2+2*is2,i3,hz)*dra**3*
     & c1*dsa*c2+u(i1,i2+2*is2,i3,hz)*dra**3*c1*dsa*c22s-u(i1,i2+2*
     & is2,i3,hz)*dra**3*c11s*dsa*c22r-3*u(i1,i2+2*is2,i3,hz)*dra**2*
     & c11*dsa*c2-48*c11s*dsa**3*c11*fw1*dra-8*c11s*dsa**3*fw3*dra**3-
     & 36*c11*c22*dra**2*fw2*dsa-24*c11*fw4*dsa**3*dra**2+12*dra**3*
     & c11r*c22*fw2*dsa+12*dra**3*c1*c22*fw2*dsa+8*dra**3*c11r*fw4*
     & dsa**3+8*dra**3*c1*fw4*dsa**3-48*c11*dsa*dra**2*c2*u(i1,i2,i3,
     & hz)-48*c11*dsa*dra**2*c22s*u(i1,i2,i3,hz)+16*dra**3*c11r*dsa*
     & c2*u(i1,i2,i3,hz)+16*dra**3*c11r*dsa*c22s*u(i1,i2,i3,hz)+16*
     & dra**3*c1*dsa*c22s*u(i1,i2,i3,hz)+16*dra**3*c1*dsa*c2*u(i1,i2,
     & i3,hz)-16*c11s*dsa*c22r*dra**3*u(i1,i2,i3,hz)-48*c11*c11s*dsa**
     & 3*u(i1,i2,i3,hz)+48*c11s*dsa**3*c11*u(i1+is1,i2,i3,hz)-16*dra**
     & 3*c1*dsa*c2*u(i1,i2+is2,i3,hz)+16*c11s*dsa*c22r*dra**3*u(i1,i2+
     & is2,i3,hz)-16*dra**3*c1*dsa*c22s*u(i1,i2+is2,i3,hz)-16*dra**3*
     & c11r*dsa*c2*u(i1,i2+is2,i3,hz)-16*dra**3*c11r*dsa*c22s*u(i1,i2+
     & is2,i3,hz)+48*c11*dsa*dra**2*c2*u(i1,i2+is2,i3,hz)+48*c11*dsa*
     & dra**2*c22s*u(i1,i2+is2,i3,hz))/dra**2/(-3*c11*dsa*c22s-3*c11*
     & dsa*c2+dra*c11r*dsa*c2+dra*c11r*dsa*c22s+9*c11*c22-3*dra*c11r*
     & c22+dra*c1*dsa*c2+dra*c1*dsa*c22s-c11s*dsa*c22r*dra-3*dra*c1*
     & c22)

      u(i1-is1,i2,i3,hz) = -1/dsa**2*(6*dsa**2*u(i1,i2,i3,hz)*dra*c1*
     & c22-2*dsa**3*u(i1,i2,i3,hz)*dra*c11r*c2-2*dsa**3*u(i1,i2,i3,hz)
     & *dra*c11r*c22s+2*c11s*dsa**3*u(i1,i2,i3,hz)*c22r*dra+6*dsa**2*
     & u(i1,i2,i3,hz)*dra*c11r*c22-2*dsa**3*u(i1,i2,i3,hz)*dra*c1*c2-
     & 2*dsa**3*u(i1,i2,i3,hz)*dra*c1*c22s+6*c22*c22r*dra**3*u(i1,i2,
     & i3,hz)+3*dsa**3*c22s*c11*u(i1+is1,i2,i3,hz)-3*dsa**2*u(i1+is1,
     & i2,i3,hz)*dra*c11r*c22-3*dsa**2*u(i1+is1,i2,i3,hz)*dra*c1*c22-
     & 9*c22*dsa**2*c11*u(i1+is1,i2,i3,hz)+dsa**3*u(i1+is1,i2,i3,hz)*
     & dra*c11r*c22s-c11s*dsa**3*u(i1+is1,i2,i3,hz)*c22r*dra+dsa**3*u(
     & i1+is1,i2,i3,hz)*dra*c1*c2+dsa**3*u(i1+is1,i2,i3,hz)*dra*c1*
     & c22s+dsa**3*u(i1+is1,i2,i3,hz)*dra*c11r*c2+3*c2*dsa**3*c11*u(
     & i1+is1,i2,i3,hz)-6*c22*c22r*dra**3*u(i1,i2+is2,i3,hz)-dsa**3*
     & c22s*fw3*dra**3+18*c22*dsa**2*c11*fw1*dra+3*c22*dsa**2*fw3*dra*
     & *3-6*dsa**3*c22s*c11*fw1*dra+fw4*dsa**3*dra**3*c22r-6*c2*dsa**
     & 3*c11*fw1*dra-c2*dsa**3*fw3*dra**3+6*c22*dra**3*fw2*dsa*c22r)/(
     & -3*c11*dsa*c22s-3*c11*dsa*c2+dra*c11r*dsa*c2+dra*c11r*dsa*c22s+
     & 9*c11*c22-3*dra*c11r*c22+dra*c1*dsa*c2+dra*c1*dsa*c22s-c11s*
     & dsa*c22r*dra-3*dra*c1*c22)

      u(i1,i2-is2,i3,hz) = -(6*c11*c11s*dsa**3*u(i1,i2,i3,hz)-2*dra**3*
     & c1*dsa*c22s*u(i1,i2,i3,hz)-2*dra**3*c1*dsa*c2*u(i1,i2,i3,hz)+2*
     & c11s*dsa*c22r*dra**3*u(i1,i2,i3,hz)-2*dra**3*c11r*dsa*c22s*u(
     & i1,i2,i3,hz)+6*c11*dsa*dra**2*c2*u(i1,i2,i3,hz)+6*c11*dsa*dra**
     & 2*c22s*u(i1,i2,i3,hz)-2*dra**3*c11r*dsa*c2*u(i1,i2,i3,hz)-6*
     & c11s*dsa**3*c11*u(i1+is1,i2,i3,hz)-c11s*dsa*c22r*dra**3*u(i1,
     & i2+is2,i3,hz)+dra**3*c1*dsa*c22s*u(i1,i2+is2,i3,hz)+dra**3*
     & c11r*dsa*c2*u(i1,i2+is2,i3,hz)+dra**3*c11r*dsa*c22s*u(i1,i2+
     & is2,i3,hz)+dra**3*c1*dsa*c2*u(i1,i2+is2,i3,hz)-3*c11*dsa*dra**
     & 2*c2*u(i1,i2+is2,i3,hz)-3*c11*dsa*dra**2*c22s*u(i1,i2+is2,i3,
     & hz)+3*dra**3*c11r*c22*u(i1,i2+is2,i3,hz)-9*c11*c22*dra**2*u(i1,
     & i2+is2,i3,hz)+3*dra**3*c1*c22*u(i1,i2+is2,i3,hz)+6*c11s*dsa**3*
     & c11*fw1*dra+c11s*dsa**3*fw3*dra**3+18*c11*c22*dra**2*fw2*dsa+3*
     & c11*fw4*dsa**3*dra**2-6*dra**3*c11r*c22*fw2*dsa-6*dra**3*c1*
     & c22*fw2*dsa-dra**3*c11r*fw4*dsa**3-dra**3*c1*fw4*dsa**3)/dra**
     & 2/(-3*c11*dsa*c22s-3*c11*dsa*c2+dra*c11r*dsa*c2+dra*c11r*dsa*
     & c22s+9*c11*c22-3*dra*c11r*c22+dra*c1*dsa*c2+dra*c1*dsa*c22s-
     & c11s*dsa*c22r*dra-3*dra*c1*c22)

      u(i1-2*is1,i2,i3,hz) = (-36*fw1*dra**2*dsa**2*c11r*c22+12*fw1*
     & dra**2*dsa**3*c1*c2+12*fw1*dra**2*dsa**3*c1*c22s-12*fw1*dra**2*
     & dsa**3*c11s*c22r-36*fw1*dra**2*dsa**2*c1*c22+12*fw1*dra**2*dsa*
     & *3*c11r*c2+12*fw1*dra**2*dsa**3*c11r*c22s+48*dsa**2*u(i1+is1,
     & i2,i3,hz)*dra*c11r*c22+48*dsa**2*u(i1+is1,i2,i3,hz)*dra*c1*c22-
     & 48*c22*c22r*dra**3*u(i1,i2,i3,hz)+16*dsa**3*u(i1,i2,i3,hz)*dra*
     & c1*c22s-16*c11s*dsa**3*u(i1,i2,i3,hz)*c22r*dra-48*dsa**2*u(i1,
     & i2,i3,hz)*dra*c1*c22+16*dsa**3*u(i1,i2,i3,hz)*dra*c11r*c2+16*
     & dsa**3*u(i1,i2,i3,hz)*dra*c11r*c22s-48*dsa**2*u(i1,i2,i3,hz)*
     & dra*c11r*c22+16*dsa**3*u(i1,i2,i3,hz)*dra*c1*c2-16*dsa**3*u(i1+
     & is1,i2,i3,hz)*dra*c1*c2-16*dsa**3*u(i1+is1,i2,i3,hz)*dra*c1*
     & c22s-16*dsa**3*u(i1+is1,i2,i3,hz)*dra*c11r*c2-16*dsa**3*u(i1+
     & is1,i2,i3,hz)*dra*c11r*c22s+16*c11s*dsa**3*u(i1+is1,i2,i3,hz)*
     & c22r*dra+48*c22*c22r*dra**3*u(i1,i2+is2,i3,hz)+u(i1+2*is1,i2,
     & i3,hz)*dsa**3*dra*c11r*c2-3*u(i1+2*is1,i2,i3,hz)*dsa**2*dra*c1*
     & c22+u(i1+2*is1,i2,i3,hz)*dsa**3*dra*c11r*c22s-3*u(i1+2*is1,i2,
     & i3,hz)*dsa**2*dra*c11r*c22+u(i1+2*is1,i2,i3,hz)*dsa**3*dra*c1*
     & c2+u(i1+2*is1,i2,i3,hz)*dsa**3*dra*c1*c22s-u(i1+2*is1,i2,i3,hz)
     & *dsa**3*c11s*c22r*dra-3*u(i1+2*is1,i2,i3,hz)*dsa**3*c11*c2+9*u(
     & i1+2*is1,i2,i3,hz)*dsa**2*c11*c22-3*u(i1+2*is1,i2,i3,hz)*dsa**
     & 3*c11*c22s+8*dsa**3*c22s*fw3*dra**3-36*c22*dsa**2*c11*fw1*dra-
     & 24*c22*dsa**2*fw3*dra**3+12*dsa**3*c22s*c11*fw1*dra-8*fw4*dsa**
     & 3*dra**3*c22r+12*c2*dsa**3*c11*fw1*dra+8*c2*dsa**3*fw3*dra**3-
     & 48*c22*dra**3*fw2*dsa*c22r)/dsa**2/(-3*c11*dsa*c22s-3*c11*dsa*
     & c2+dra*c11r*dsa*c2+dra*c11r*dsa*c22s+9*c11*c22-3*dra*c11r*c22+
     & dra*c1*dsa*c2+dra*c1*dsa*c22s-c11s*dsa*c22r*dra-3*dra*c1*c22)



! ****************** done Hz extended fourth-order ********************
                       if( debug.gt.0 )then
                        write(*,'(/,"-------------")')
                        write(*,'(" bcOpt: extended4 i1,i2=",2i4," is1,
     & is2=",2i3," x,y=",2f8.4,"dra,dsa=",2e8.2)') i1,i2,is1,is2,xy(
     & i1,i2,i3,0),xy(i1,i2,i3,1),dra,dsa
                        write(*,'(" bcOpt: extended4 det,c11,c22,c1,
     & c2=",5e10.2, ", c1Order2=",e10.2)') det,c11,c22,c1,c2,(rsxyx22(
     & i1,i2,i3,axis,0)+rsxyy22(i1,i2,i3,axis,1))
                        write(*,'("      : Lu-utt=",e10.2," Lv-vtt=",
     & e10.2)') uLaplacian42(i1,i2,i3,ex)-utt00,uLaplacian42(i1,i2,i3,
     & ey)-vtt00
                        ! write(*,'("   g1a,g2a,cu20,cu02,cu10,cu01=",6e16.8)') g1a,g2a,cu20,cu02,cu10,cu01
                        ! write(*,'("   cv20,cv02,cv10,cv01=",6e18.10)') cv20,cv02,cv10,cv01
                        ! write(*,'("   gLu,gLv,uLaplacian42(ex,ey)=",6e16.8)') gLu,gLv,uLaplacian42(i1,i2,i3,ex),uLaplacian42(i1,i2,i3,ey)
                         call ogf2dfo(ep,fieldOption,xy(i1-is1,i2,i3,0)
     & ,xy(i1-is1,i2,i3,1),t,uv0(0),uv0(1),uv0(2))
                        write(*,'(" bcOpt: extended4 i1-is1,i2=",2i4," 
     & ex,err,ey,err=",4e10.2)') i1-is1,i2,u(i1-is1,i2,i3,ex),u(i1-
     & is1,i2,i3,ex)-uv0(0),u(i1-is1,i2,i3,ey),u(i1-is1,i2,i3,ey)-uv0(
     & 1)
                         call ogf2dfo(ep,fieldOption,xy(i1,i2-is2,i3,0)
     & ,xy(i1,i2-is2,i3,1),t,uv0(0),uv0(1),uv0(2))
                        write(*,'(" bcOpt: extended4 i1,i2-is2=",2i4," 
     & ex,err,ey,err=",4e10.2)') i1,i2-is2,u(i1,i2-is2,i3,ex),u(i1,i2-
     & is2,i3,ex)-uv0(0),u(i1,i2-is2,i3,ey),u(i1,i2-is2,i3,ey)-uv0(1)
                         call ogf2dfo(ep,fieldOption,xy(i1-2*is1,i2,i3,
     & 0),xy(i1-2*is1,i2,i3,1),t,uv0(0),uv0(1),uv0(2))
                        write(*,'(" bcOpt: extended4 i1-2*is1,i2=",2i4,
     & " ex,err,ey,err=",4e10.2)') i1-2*is1,i2,u(i1-2*is1,i2,i3,ex),u(
     & i1-2*is1,i2,i3,ex)-uv0(0),u(i1-2*is1,i2,i3,ey),u(i1-2*is1,i2,
     & i3,ey)-uv0(1)
                         call ogf2dfo(ep,fieldOption,xy(i1,i2-2*is2,i3,
     & 0),xy(i1,i2-2*is2,i3,1),t,uv0(0),uv0(1),uv0(2))
                        write(*,'(" bcOpt: extended4 i1,i2-2*is2=",2i4,
     & " ex,err,ey,err=",4e10.2)') i1,i2-2*is2,u(i1,i2-2*is2,i3,ex),u(
     & i1,i2-2*is2,i3,ex)-uv0(0),u(i1,i2-2*is2,i3,ey),u(i1,i2-2*is2,
     & i3,ey)-uv0(1)
                        write(*,'("-------------",/)')
                       end if
                    ! dra=dr(0)  ! ** reset *** is this correct?
                    ! dsa=dr(1)
                     axis=0
                     axisp1=1
                      ! evaluate non-mixed derivatives at the corner
                        ! ***** finish this *****
                        ur=ur4(i1,i2,i3,ex)
                        vr=ur4(i1,i2,i3,ey)
                        us=us4(i1,i2,i3,ex)
                        vs=us4(i1,i2,i3,ey)
                        urr=urr4(i1,i2,i3,ex)
                        vrr=urr4(i1,i2,i3,ey)
                        uss=uss4(i1,i2,i3,ex)
                        vss=uss4(i1,i2,i3,ey)
                        jac=1./(rx(i1,i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)*
     & sx(i1,i2,i3))
                        a11 =rsxy(i1,i2,i3,0,0)*jac
                        a12 =rsxy(i1,i2,i3,0,1)*jac
                        a21 =rsxy(i1,i2,i3,1,0)*jac
                        a22 =rsxy(i1,i2,i3,1,1)*jac
                        a11r = (8.*((rsxy(i1+1,i2,i3,axis,0)/(rx(i1+1,
     & i2,i3)*sy(i1+1,i2,i3)-ry(i1+1,i2,i3)*sx(i1+1,i2,i3)))-(rsxy(i1-
     & 1,i2,i3,axis,0)/(rx(i1-1,i2,i3)*sy(i1-1,i2,i3)-ry(i1-1,i2,i3)*
     & sx(i1-1,i2,i3))))-((rsxy(i1+2,i2,i3,axis,0)/(rx(i1+2,i2,i3)*sy(
     & i1+2,i2,i3)-ry(i1+2,i2,i3)*sx(i1+2,i2,i3)))-(rsxy(i1-2,i2,i3,
     & axis,0)/(rx(i1-2,i2,i3)*sy(i1-2,i2,i3)-ry(i1-2,i2,i3)*sx(i1-2,
     & i2,i3)))))/(12.*dr(0))
                        a12r = (8.*((rsxy(i1+1,i2,i3,axis,1)/(rx(i1+1,
     & i2,i3)*sy(i1+1,i2,i3)-ry(i1+1,i2,i3)*sx(i1+1,i2,i3)))-(rsxy(i1-
     & 1,i2,i3,axis,1)/(rx(i1-1,i2,i3)*sy(i1-1,i2,i3)-ry(i1-1,i2,i3)*
     & sx(i1-1,i2,i3))))-((rsxy(i1+2,i2,i3,axis,1)/(rx(i1+2,i2,i3)*sy(
     & i1+2,i2,i3)-ry(i1+2,i2,i3)*sx(i1+2,i2,i3)))-(rsxy(i1-2,i2,i3,
     & axis,1)/(rx(i1-2,i2,i3)*sy(i1-2,i2,i3)-ry(i1-2,i2,i3)*sx(i1-2,
     & i2,i3)))))/(12.*dr(0))
                        a21r = (8.*((rsxy(i1+1,i2,i3,axisp1,0)/(rx(i1+
     & 1,i2,i3)*sy(i1+1,i2,i3)-ry(i1+1,i2,i3)*sx(i1+1,i2,i3)))-(rsxy(
     & i1-1,i2,i3,axisp1,0)/(rx(i1-1,i2,i3)*sy(i1-1,i2,i3)-ry(i1-1,i2,
     & i3)*sx(i1-1,i2,i3))))-((rsxy(i1+2,i2,i3,axisp1,0)/(rx(i1+2,i2,
     & i3)*sy(i1+2,i2,i3)-ry(i1+2,i2,i3)*sx(i1+2,i2,i3)))-(rsxy(i1-2,
     & i2,i3,axisp1,0)/(rx(i1-2,i2,i3)*sy(i1-2,i2,i3)-ry(i1-2,i2,i3)*
     & sx(i1-2,i2,i3)))))/(12.*dr(0))
                        a22r = (8.*((rsxy(i1+1,i2,i3,axisp1,1)/(rx(i1+
     & 1,i2,i3)*sy(i1+1,i2,i3)-ry(i1+1,i2,i3)*sx(i1+1,i2,i3)))-(rsxy(
     & i1-1,i2,i3,axisp1,1)/(rx(i1-1,i2,i3)*sy(i1-1,i2,i3)-ry(i1-1,i2,
     & i3)*sx(i1-1,i2,i3))))-((rsxy(i1+2,i2,i3,axisp1,1)/(rx(i1+2,i2,
     & i3)*sy(i1+2,i2,i3)-ry(i1+2,i2,i3)*sx(i1+2,i2,i3)))-(rsxy(i1-2,
     & i2,i3,axisp1,1)/(rx(i1-2,i2,i3)*sy(i1-2,i2,i3)-ry(i1-2,i2,i3)*
     & sx(i1-2,i2,i3)))))/(12.*dr(0))
                        a11s = (8.*((rsxy(i1,i2+1,i3,axis,0)/(rx(i1,i2+
     & 1,i3)*sy(i1,i2+1,i3)-ry(i1,i2+1,i3)*sx(i1,i2+1,i3)))-(rsxy(i1,
     & i2-1,i3,axis,0)/(rx(i1,i2-1,i3)*sy(i1,i2-1,i3)-ry(i1,i2-1,i3)*
     & sx(i1,i2-1,i3))))-((rsxy(i1,i2+2,i3,axis,0)/(rx(i1,i2+2,i3)*sy(
     & i1,i2+2,i3)-ry(i1,i2+2,i3)*sx(i1,i2+2,i3)))-(rsxy(i1,i2-2,i3,
     & axis,0)/(rx(i1,i2-2,i3)*sy(i1,i2-2,i3)-ry(i1,i2-2,i3)*sx(i1,i2-
     & 2,i3)))))/(12.*dr(1))
                        a12s = (8.*((rsxy(i1,i2+1,i3,axis,1)/(rx(i1,i2+
     & 1,i3)*sy(i1,i2+1,i3)-ry(i1,i2+1,i3)*sx(i1,i2+1,i3)))-(rsxy(i1,
     & i2-1,i3,axis,1)/(rx(i1,i2-1,i3)*sy(i1,i2-1,i3)-ry(i1,i2-1,i3)*
     & sx(i1,i2-1,i3))))-((rsxy(i1,i2+2,i3,axis,1)/(rx(i1,i2+2,i3)*sy(
     & i1,i2+2,i3)-ry(i1,i2+2,i3)*sx(i1,i2+2,i3)))-(rsxy(i1,i2-2,i3,
     & axis,1)/(rx(i1,i2-2,i3)*sy(i1,i2-2,i3)-ry(i1,i2-2,i3)*sx(i1,i2-
     & 2,i3)))))/(12.*dr(1))
                        a21s = (8.*((rsxy(i1,i2+1,i3,axisp1,0)/(rx(i1,
     & i2+1,i3)*sy(i1,i2+1,i3)-ry(i1,i2+1,i3)*sx(i1,i2+1,i3)))-(rsxy(
     & i1,i2-1,i3,axisp1,0)/(rx(i1,i2-1,i3)*sy(i1,i2-1,i3)-ry(i1,i2-1,
     & i3)*sx(i1,i2-1,i3))))-((rsxy(i1,i2+2,i3,axisp1,0)/(rx(i1,i2+2,
     & i3)*sy(i1,i2+2,i3)-ry(i1,i2+2,i3)*sx(i1,i2+2,i3)))-(rsxy(i1,i2-
     & 2,i3,axisp1,0)/(rx(i1,i2-2,i3)*sy(i1,i2-2,i3)-ry(i1,i2-2,i3)*
     & sx(i1,i2-2,i3)))))/(12.*dr(1))
                        a22s = (8.*((rsxy(i1,i2+1,i3,axisp1,1)/(rx(i1,
     & i2+1,i3)*sy(i1,i2+1,i3)-ry(i1,i2+1,i3)*sx(i1,i2+1,i3)))-(rsxy(
     & i1,i2-1,i3,axisp1,1)/(rx(i1,i2-1,i3)*sy(i1,i2-1,i3)-ry(i1,i2-1,
     & i3)*sx(i1,i2-1,i3))))-((rsxy(i1,i2+2,i3,axisp1,1)/(rx(i1,i2+2,
     & i3)*sy(i1,i2+2,i3)-ry(i1,i2+2,i3)*sx(i1,i2+2,i3)))-(rsxy(i1,i2-
     & 2,i3,axisp1,1)/(rx(i1,i2-2,i3)*sy(i1,i2-2,i3)-ry(i1,i2-2,i3)*
     & sx(i1,i2-2,i3)))))/(12.*dr(1))
                        a11rs = (8.*((8.*((rsxy(i1+1,i2+1,i3,axis,0)/(
     & rx(i1+1,i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+
     & 1,i3)))-(rsxy(i1-1,i2+1,i3,axis,0)/(rx(i1-1,i2+1,i3)*sy(i1-1,
     & i2+1,i3)-ry(i1-1,i2+1,i3)*sx(i1-1,i2+1,i3))))-((rsxy(i1+2,i2+1,
     & i3,axis,0)/(rx(i1+2,i2+1,i3)*sy(i1+2,i2+1,i3)-ry(i1+2,i2+1,i3)*
     & sx(i1+2,i2+1,i3)))-(rsxy(i1-2,i2+1,i3,axis,0)/(rx(i1-2,i2+1,i3)
     & *sy(i1-2,i2+1,i3)-ry(i1-2,i2+1,i3)*sx(i1-2,i2+1,i3)))))/(12.*
     & dr(0))-(8.*((rsxy(i1+1,i2-1,i3,axis,0)/(rx(i1+1,i2-1,i3)*sy(i1+
     & 1,i2-1,i3)-ry(i1+1,i2-1,i3)*sx(i1+1,i2-1,i3)))-(rsxy(i1-1,i2-1,
     & i3,axis,0)/(rx(i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-ry(i1-1,i2-1,i3)*
     & sx(i1-1,i2-1,i3))))-((rsxy(i1+2,i2-1,i3,axis,0)/(rx(i1+2,i2-1,
     & i3)*sy(i1+2,i2-1,i3)-ry(i1+2,i2-1,i3)*sx(i1+2,i2-1,i3)))-(rsxy(
     & i1-2,i2-1,i3,axis,0)/(rx(i1-2,i2-1,i3)*sy(i1-2,i2-1,i3)-ry(i1-
     & 2,i2-1,i3)*sx(i1-2,i2-1,i3)))))/(12.*dr(0)))-((8.*((rsxy(i1+1,
     & i2+2,i3,axis,0)/(rx(i1+1,i2+2,i3)*sy(i1+1,i2+2,i3)-ry(i1+1,i2+
     & 2,i3)*sx(i1+1,i2+2,i3)))-(rsxy(i1-1,i2+2,i3,axis,0)/(rx(i1-1,
     & i2+2,i3)*sy(i1-1,i2+2,i3)-ry(i1-1,i2+2,i3)*sx(i1-1,i2+2,i3))))-
     & ((rsxy(i1+2,i2+2,i3,axis,0)/(rx(i1+2,i2+2,i3)*sy(i1+2,i2+2,i3)-
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
                        a12rs = (8.*((8.*((rsxy(i1+1,i2+1,i3,axis,1)/(
     & rx(i1+1,i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,i2+
     & 1,i3)))-(rsxy(i1-1,i2+1,i3,axis,1)/(rx(i1-1,i2+1,i3)*sy(i1-1,
     & i2+1,i3)-ry(i1-1,i2+1,i3)*sx(i1-1,i2+1,i3))))-((rsxy(i1+2,i2+1,
     & i3,axis,1)/(rx(i1+2,i2+1,i3)*sy(i1+2,i2+1,i3)-ry(i1+2,i2+1,i3)*
     & sx(i1+2,i2+1,i3)))-(rsxy(i1-2,i2+1,i3,axis,1)/(rx(i1-2,i2+1,i3)
     & *sy(i1-2,i2+1,i3)-ry(i1-2,i2+1,i3)*sx(i1-2,i2+1,i3)))))/(12.*
     & dr(0))-(8.*((rsxy(i1+1,i2-1,i3,axis,1)/(rx(i1+1,i2-1,i3)*sy(i1+
     & 1,i2-1,i3)-ry(i1+1,i2-1,i3)*sx(i1+1,i2-1,i3)))-(rsxy(i1-1,i2-1,
     & i3,axis,1)/(rx(i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-ry(i1-1,i2-1,i3)*
     & sx(i1-1,i2-1,i3))))-((rsxy(i1+2,i2-1,i3,axis,1)/(rx(i1+2,i2-1,
     & i3)*sy(i1+2,i2-1,i3)-ry(i1+2,i2-1,i3)*sx(i1+2,i2-1,i3)))-(rsxy(
     & i1-2,i2-1,i3,axis,1)/(rx(i1-2,i2-1,i3)*sy(i1-2,i2-1,i3)-ry(i1-
     & 2,i2-1,i3)*sx(i1-2,i2-1,i3)))))/(12.*dr(0)))-((8.*((rsxy(i1+1,
     & i2+2,i3,axis,1)/(rx(i1+1,i2+2,i3)*sy(i1+1,i2+2,i3)-ry(i1+1,i2+
     & 2,i3)*sx(i1+1,i2+2,i3)))-(rsxy(i1-1,i2+2,i3,axis,1)/(rx(i1-1,
     & i2+2,i3)*sy(i1-1,i2+2,i3)-ry(i1-1,i2+2,i3)*sx(i1-1,i2+2,i3))))-
     & ((rsxy(i1+2,i2+2,i3,axis,1)/(rx(i1+2,i2+2,i3)*sy(i1+2,i2+2,i3)-
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
                        a21rs = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp1,0)
     & /(rx(i1+1,i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,
     & i2+1,i3)))-(rsxy(i1-1,i2+1,i3,axisp1,0)/(rx(i1-1,i2+1,i3)*sy(
     & i1-1,i2+1,i3)-ry(i1-1,i2+1,i3)*sx(i1-1,i2+1,i3))))-((rsxy(i1+2,
     & i2+1,i3,axisp1,0)/(rx(i1+2,i2+1,i3)*sy(i1+2,i2+1,i3)-ry(i1+2,
     & i2+1,i3)*sx(i1+2,i2+1,i3)))-(rsxy(i1-2,i2+1,i3,axisp1,0)/(rx(
     & i1-2,i2+1,i3)*sy(i1-2,i2+1,i3)-ry(i1-2,i2+1,i3)*sx(i1-2,i2+1,
     & i3)))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-1,i3,axisp1,0)/(rx(i1+1,
     & i2-1,i3)*sy(i1+1,i2-1,i3)-ry(i1+1,i2-1,i3)*sx(i1+1,i2-1,i3)))-(
     & rsxy(i1-1,i2-1,i3,axisp1,0)/(rx(i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-
     & ry(i1-1,i2-1,i3)*sx(i1-1,i2-1,i3))))-((rsxy(i1+2,i2-1,i3,
     & axisp1,0)/(rx(i1+2,i2-1,i3)*sy(i1+2,i2-1,i3)-ry(i1+2,i2-1,i3)*
     & sx(i1+2,i2-1,i3)))-(rsxy(i1-2,i2-1,i3,axisp1,0)/(rx(i1-2,i2-1,
     & i3)*sy(i1-2,i2-1,i3)-ry(i1-2,i2-1,i3)*sx(i1-2,i2-1,i3)))))/(
     & 12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp1,0)/(rx(i1+1,i2+2,
     & i3)*sy(i1+1,i2+2,i3)-ry(i1+1,i2+2,i3)*sx(i1+1,i2+2,i3)))-(rsxy(
     & i1-1,i2+2,i3,axisp1,0)/(rx(i1-1,i2+2,i3)*sy(i1-1,i2+2,i3)-ry(
     & i1-1,i2+2,i3)*sx(i1-1,i2+2,i3))))-((rsxy(i1+2,i2+2,i3,axisp1,0)
     & /(rx(i1+2,i2+2,i3)*sy(i1+2,i2+2,i3)-ry(i1+2,i2+2,i3)*sx(i1+2,
     & i2+2,i3)))-(rsxy(i1-2,i2+2,i3,axisp1,0)/(rx(i1-2,i2+2,i3)*sy(
     & i1-2,i2+2,i3)-ry(i1-2,i2+2,i3)*sx(i1-2,i2+2,i3)))))/(12.*dr(0))
     & -(8.*((rsxy(i1+1,i2-2,i3,axisp1,0)/(rx(i1+1,i2-2,i3)*sy(i1+1,
     & i2-2,i3)-ry(i1+1,i2-2,i3)*sx(i1+1,i2-2,i3)))-(rsxy(i1-1,i2-2,
     & i3,axisp1,0)/(rx(i1-1,i2-2,i3)*sy(i1-1,i2-2,i3)-ry(i1-1,i2-2,
     & i3)*sx(i1-1,i2-2,i3))))-((rsxy(i1+2,i2-2,i3,axisp1,0)/(rx(i1+2,
     & i2-2,i3)*sy(i1+2,i2-2,i3)-ry(i1+2,i2-2,i3)*sx(i1+2,i2-2,i3)))-(
     & rsxy(i1-2,i2-2,i3,axisp1,0)/(rx(i1-2,i2-2,i3)*sy(i1-2,i2-2,i3)-
     & ry(i1-2,i2-2,i3)*sx(i1-2,i2-2,i3)))))/(12.*dr(0))))/(12.*dr(1))
                        a22rs = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp1,1)
     & /(rx(i1+1,i2+1,i3)*sy(i1+1,i2+1,i3)-ry(i1+1,i2+1,i3)*sx(i1+1,
     & i2+1,i3)))-(rsxy(i1-1,i2+1,i3,axisp1,1)/(rx(i1-1,i2+1,i3)*sy(
     & i1-1,i2+1,i3)-ry(i1-1,i2+1,i3)*sx(i1-1,i2+1,i3))))-((rsxy(i1+2,
     & i2+1,i3,axisp1,1)/(rx(i1+2,i2+1,i3)*sy(i1+2,i2+1,i3)-ry(i1+2,
     & i2+1,i3)*sx(i1+2,i2+1,i3)))-(rsxy(i1-2,i2+1,i3,axisp1,1)/(rx(
     & i1-2,i2+1,i3)*sy(i1-2,i2+1,i3)-ry(i1-2,i2+1,i3)*sx(i1-2,i2+1,
     & i3)))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-1,i3,axisp1,1)/(rx(i1+1,
     & i2-1,i3)*sy(i1+1,i2-1,i3)-ry(i1+1,i2-1,i3)*sx(i1+1,i2-1,i3)))-(
     & rsxy(i1-1,i2-1,i3,axisp1,1)/(rx(i1-1,i2-1,i3)*sy(i1-1,i2-1,i3)-
     & ry(i1-1,i2-1,i3)*sx(i1-1,i2-1,i3))))-((rsxy(i1+2,i2-1,i3,
     & axisp1,1)/(rx(i1+2,i2-1,i3)*sy(i1+2,i2-1,i3)-ry(i1+2,i2-1,i3)*
     & sx(i1+2,i2-1,i3)))-(rsxy(i1-2,i2-1,i3,axisp1,1)/(rx(i1-2,i2-1,
     & i3)*sy(i1-2,i2-1,i3)-ry(i1-2,i2-1,i3)*sx(i1-2,i2-1,i3)))))/(
     & 12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp1,1)/(rx(i1+1,i2+2,
     & i3)*sy(i1+1,i2+2,i3)-ry(i1+1,i2+2,i3)*sx(i1+1,i2+2,i3)))-(rsxy(
     & i1-1,i2+2,i3,axisp1,1)/(rx(i1-1,i2+2,i3)*sy(i1-1,i2+2,i3)-ry(
     & i1-1,i2+2,i3)*sx(i1-1,i2+2,i3))))-((rsxy(i1+2,i2+2,i3,axisp1,1)
     & /(rx(i1+2,i2+2,i3)*sy(i1+2,i2+2,i3)-ry(i1+2,i2+2,i3)*sx(i1+2,
     & i2+2,i3)))-(rsxy(i1-2,i2+2,i3,axisp1,1)/(rx(i1-2,i2+2,i3)*sy(
     & i1-2,i2+2,i3)-ry(i1-2,i2+2,i3)*sx(i1-2,i2+2,i3)))))/(12.*dr(0))
     & -(8.*((rsxy(i1+1,i2-2,i3,axisp1,1)/(rx(i1+1,i2-2,i3)*sy(i1+1,
     & i2-2,i3)-ry(i1+1,i2-2,i3)*sx(i1+1,i2-2,i3)))-(rsxy(i1-1,i2-2,
     & i3,axisp1,1)/(rx(i1-1,i2-2,i3)*sy(i1-1,i2-2,i3)-ry(i1-1,i2-2,
     & i3)*sx(i1-1,i2-2,i3))))-((rsxy(i1+2,i2-2,i3,axisp1,1)/(rx(i1+2,
     & i2-2,i3)*sy(i1+2,i2-2,i3)-ry(i1+2,i2-2,i3)*sx(i1+2,i2-2,i3)))-(
     & rsxy(i1-2,i2-2,i3,axisp1,1)/(rx(i1-2,i2-2,i3)*sy(i1-2,i2-2,i3)-
     & ry(i1-2,i2-2,i3)*sx(i1-2,i2-2,i3)))))/(12.*dr(0))))/(12.*dr(1))
                        a11rr = ((-30.*(rsxy(i1,i2,i3,axis,0)/(rx(i1,
     & i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+16.*((rsxy(i1+
     & 1,i2,i3,axis,0)/(rx(i1+1,i2,i3)*sy(i1+1,i2,i3)-ry(i1+1,i2,i3)*
     & sx(i1+1,i2,i3)))+(rsxy(i1-1,i2,i3,axis,0)/(rx(i1-1,i2,i3)*sy(
     & i1-1,i2,i3)-ry(i1-1,i2,i3)*sx(i1-1,i2,i3))))-((rsxy(i1+2,i2,i3,
     & axis,0)/(rx(i1+2,i2,i3)*sy(i1+2,i2,i3)-ry(i1+2,i2,i3)*sx(i1+2,
     & i2,i3)))+(rsxy(i1-2,i2,i3,axis,0)/(rx(i1-2,i2,i3)*sy(i1-2,i2,
     & i3)-ry(i1-2,i2,i3)*sx(i1-2,i2,i3)))))/(12.*dr(0)**2))
                        a12rr = ((-30.*(rsxy(i1,i2,i3,axis,1)/(rx(i1,
     & i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+16.*((rsxy(i1+
     & 1,i2,i3,axis,1)/(rx(i1+1,i2,i3)*sy(i1+1,i2,i3)-ry(i1+1,i2,i3)*
     & sx(i1+1,i2,i3)))+(rsxy(i1-1,i2,i3,axis,1)/(rx(i1-1,i2,i3)*sy(
     & i1-1,i2,i3)-ry(i1-1,i2,i3)*sx(i1-1,i2,i3))))-((rsxy(i1+2,i2,i3,
     & axis,1)/(rx(i1+2,i2,i3)*sy(i1+2,i2,i3)-ry(i1+2,i2,i3)*sx(i1+2,
     & i2,i3)))+(rsxy(i1-2,i2,i3,axis,1)/(rx(i1-2,i2,i3)*sy(i1-2,i2,
     & i3)-ry(i1-2,i2,i3)*sx(i1-2,i2,i3)))))/(12.*dr(0)**2))
                        a21rr = ((-30.*(rsxy(i1,i2,i3,axisp1,0)/(rx(i1,
     & i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+16.*((rsxy(i1+
     & 1,i2,i3,axisp1,0)/(rx(i1+1,i2,i3)*sy(i1+1,i2,i3)-ry(i1+1,i2,i3)
     & *sx(i1+1,i2,i3)))+(rsxy(i1-1,i2,i3,axisp1,0)/(rx(i1-1,i2,i3)*
     & sy(i1-1,i2,i3)-ry(i1-1,i2,i3)*sx(i1-1,i2,i3))))-((rsxy(i1+2,i2,
     & i3,axisp1,0)/(rx(i1+2,i2,i3)*sy(i1+2,i2,i3)-ry(i1+2,i2,i3)*sx(
     & i1+2,i2,i3)))+(rsxy(i1-2,i2,i3,axisp1,0)/(rx(i1-2,i2,i3)*sy(i1-
     & 2,i2,i3)-ry(i1-2,i2,i3)*sx(i1-2,i2,i3)))))/(12.*dr(0)**2))
                        a22rr = ((-30.*(rsxy(i1,i2,i3,axisp1,1)/(rx(i1,
     & i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+16.*((rsxy(i1+
     & 1,i2,i3,axisp1,1)/(rx(i1+1,i2,i3)*sy(i1+1,i2,i3)-ry(i1+1,i2,i3)
     & *sx(i1+1,i2,i3)))+(rsxy(i1-1,i2,i3,axisp1,1)/(rx(i1-1,i2,i3)*
     & sy(i1-1,i2,i3)-ry(i1-1,i2,i3)*sx(i1-1,i2,i3))))-((rsxy(i1+2,i2,
     & i3,axisp1,1)/(rx(i1+2,i2,i3)*sy(i1+2,i2,i3)-ry(i1+2,i2,i3)*sx(
     & i1+2,i2,i3)))+(rsxy(i1-2,i2,i3,axisp1,1)/(rx(i1-2,i2,i3)*sy(i1-
     & 2,i2,i3)-ry(i1-2,i2,i3)*sx(i1-2,i2,i3)))))/(12.*dr(0)**2))
                        a21ss = ((-30.*(rsxy(i1,i2,i3,axisp1,0)/(rx(i1,
     & i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+16.*((rsxy(i1,
     & i2+1,i3,axisp1,0)/(rx(i1,i2+1,i3)*sy(i1,i2+1,i3)-ry(i1,i2+1,i3)
     & *sx(i1,i2+1,i3)))+(rsxy(i1,i2-1,i3,axisp1,0)/(rx(i1,i2-1,i3)*
     & sy(i1,i2-1,i3)-ry(i1,i2-1,i3)*sx(i1,i2-1,i3))))-((rsxy(i1,i2+2,
     & i3,axisp1,0)/(rx(i1,i2+2,i3)*sy(i1,i2+2,i3)-ry(i1,i2+2,i3)*sx(
     & i1,i2+2,i3)))+(rsxy(i1,i2-2,i3,axisp1,0)/(rx(i1,i2-2,i3)*sy(i1,
     & i2-2,i3)-ry(i1,i2-2,i3)*sx(i1,i2-2,i3)))))/(12.*dr(1)**2))
                        a22ss = ((-30.*(rsxy(i1,i2,i3,axisp1,1)/(rx(i1,
     & i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)))+16.*((rsxy(i1,
     & i2+1,i3,axisp1,1)/(rx(i1,i2+1,i3)*sy(i1,i2+1,i3)-ry(i1,i2+1,i3)
     & *sx(i1,i2+1,i3)))+(rsxy(i1,i2-1,i3,axisp1,1)/(rx(i1,i2-1,i3)*
     & sy(i1,i2-1,i3)-ry(i1,i2-1,i3)*sx(i1,i2-1,i3))))-((rsxy(i1,i2+2,
     & i3,axisp1,1)/(rx(i1,i2+2,i3)*sy(i1,i2+2,i3)-ry(i1,i2+2,i3)*sx(
     & i1,i2+2,i3)))+(rsxy(i1,i2-2,i3,axisp1,1)/(rx(i1,i2-2,i3)*sy(i1,
     & i2-2,i3)-ry(i1,i2-2,i3)*sx(i1,i2-2,i3)))))/(12.*dr(1)**2))
                        urs=-(a12**2*vrr-2*a21s*us*a22-a21ss*u(i1,i2,
     & i3,ex)*a22-a12s*vr*a22-a12r*vs*a22-a22**2*vss-2*a22s*vs*a22-
     & a21*uss*a22-a11rs*u(i1,i2,i3,ex)*a22-a11s*ur*a22-a11r*us*a22+2*
     & a12*a12r*vr+a12*a21rs*u(i1,i2,i3,ex)+a12*a12rr*u(i1,i2,i3,ey)+
     & a12*a11*urr+2*a12*a11r*ur-a12rs*u(i1,i2,i3,ey)*a22+a12*a22r*vs+
     & a12*a11rr*u(i1,i2,i3,ex)+a12*a22s*vr+a12*a22rs*u(i1,i2,i3,ey)+
     & a12*a21s*ur+a12*a21r*us-a22ss*u(i1,i2,i3,ey)*a22)/(-a11*a22+
     & a21*a12)
                        vrs=(a11*a21rs*u(i1,i2,i3,ex)+a11*a12*vrr+2*
     & a11*a12r*vr+a11*a12rr*u(i1,i2,i3,ey)+2*a11*a11r*ur+a11*a11rr*u(
     & i1,i2,i3,ex)-a21*a22*vss+a11*a22r*vs+a11*a22s*vr+a11*a22rs*u(
     & i1,i2,i3,ey)+a11*a21r*us+a11*a21s*ur-a21*a12s*vr-a21*a12r*vs-
     & a21*a12rs*u(i1,i2,i3,ey)-a21*a11s*ur-a21*a11r*us-a21*a11rs*u(
     & i1,i2,i3,ex)-a21*a21ss*u(i1,i2,i3,ex)-a21*a22ss*u(i1,i2,i3,ey)-
     & 2*a21*a22s*vs-2*a21*a21s*us+a11**2*urr-a21**2*uss)/(-a11*a22+
     & a21*a12)
                        urrrr=urrrr2(i1,i2,i3,ex)
                        ussss=ussss2(i1,i2,i3,ex)
                        vrrrr=urrrr2(i1,i2,i3,ey)
                        vssss=ussss2(i1,i2,i3,ey)
                         if( debug.gt.0 )then
                         write(*,'("ghostValuesOutsideCorners2d: i1,i2,
     & i3=",3i3," urs,-vss=",2f9.3," vrs,-urr=",2f9.3," dra,dsa=",
     & 2e10.2)') i1,i2,i3,urs,-vss,vrs,-urr,dra,dsa
                         write(*,'("ghostValuesOutsideCorners2d:  urrr,
     & usss=",2e10.2,", urrrr,ussss=",4e10.2)') urrr2(i1,i2,i3,ex),
     & usss2(i1,i2,i3,ex),urrrr,ussss,vrrrr,vssss
                          ! "
                         end if
                        ! **** finish these ****
                        urrss=0.  ! from equation   uxxxx + uxxyy = uttxx  [ u(x,0)=0 => uxxxx(x,0)=0 uxxtt(x,0)=0 ]
                        vrrss=0.  ! from equation
                        urrrs=-vrrss  ! from div
                        ursss=-vssss  ! from div
                        vrrrs=-urrrr  ! from div
                        vrsss=-urrss  ! from div
                          u(i1-is1,i2-is2,i3,ex)= 2.*u(i1,i2,i3,ex)-u(
     & i1+is1,i2+is2,i3,ex) + ( (dra)**2*urr+2.*(dra)*(dsa)*urs+(dsa)*
     & *2*uss )+ (1./12.)*( (dra)**4*urrrr + 4.*(dra)**3*(dsa)*urrrs +
     &  6.*(dra)**2*(dsa)**2*urrss + 4.*(dra)*(dsa)**3*ursss + (dsa)**
     & 4*ussss )
                          u(i1-is1,i2-is2,i3,ey)= 2.*u(i1,i2,i3,ey)-u(
     & i1+is1,i2+is2,i3,ey) + ( (dra)**2*vrr+2.*(dra)*(dsa)*vrs+(dsa)*
     & *2*vss )+ (1./12.)*( (dra)**4*vrrrr + 4.*(dra)**3*(dsa)*vrrrs +
     &  6.*(dra)**2*(dsa)**2*vrrss + 4.*(dra)*(dsa)**3*vrsss + (dsa)**
     & 4*vssss )
                          u(i1-2*is1,i2-is2,i3,ex)= 2.*u(i1,i2,i3,ex)-
     & u(i1+2*is1,i2+is2,i3,ex) + ( (2.*dra)**2*urr+2.*(2.*dra)*(dsa)*
     & urs+(dsa)**2*uss )+ (1./12.)*( (2.*dra)**4*urrrr + 4.*(2.*dra)*
     & *3*(dsa)*urrrs + 6.*(2.*dra)**2*(dsa)**2*urrss + 4.*(2.*dra)*(
     & dsa)**3*ursss + (dsa)**4*ussss )
                          u(i1-2*is1,i2-is2,i3,ey)= 2.*u(i1,i2,i3,ey)-
     & u(i1+2*is1,i2+is2,i3,ey) + ( (2.*dra)**2*vrr+2.*(2.*dra)*(dsa)*
     & vrs+(dsa)**2*vss )+ (1./12.)*( (2.*dra)**4*vrrrr + 4.*(2.*dra)*
     & *3*(dsa)*vrrrs + 6.*(2.*dra)**2*(dsa)**2*vrrss + 4.*(2.*dra)*(
     & dsa)**3*vrsss + (dsa)**4*vssss )
                          u(i1-is1,i2-2*is2,i3,ex)= 2.*u(i1,i2,i3,ex)-
     & u(i1+is1,i2+2*is2,i3,ex) + ( (dra)**2*urr+2.*(dra)*(2.*dsa)*
     & urs+(2.*dsa)**2*uss )+ (1./12.)*( (dra)**4*urrrr + 4.*(dra)**3*
     & (2.*dsa)*urrrs + 6.*(dra)**2*(2.*dsa)**2*urrss + 4.*(dra)*(2.*
     & dsa)**3*ursss + (2.*dsa)**4*ussss )
                          u(i1-is1,i2-2*is2,i3,ey)= 2.*u(i1,i2,i3,ey)-
     & u(i1+is1,i2+2*is2,i3,ey) + ( (dra)**2*vrr+2.*(dra)*(2.*dsa)*
     & vrs+(2.*dsa)**2*vss )+ (1./12.)*( (dra)**4*vrrrr + 4.*(dra)**3*
     & (2.*dsa)*vrrrs + 6.*(dra)**2*(2.*dsa)**2*vrrss + 4.*(dra)*(2.*
     & dsa)**3*vrsss + (2.*dsa)**4*vssss )
                          u(i1-2*is1,i2-2*is2,i3,ex)= 2.*u(i1,i2,i3,ex)
     & -u(i1+2*is1,i2+2*is2,i3,ex) + ( (2.*dra)**2*urr+2.*(2.*dra)*(
     & 2.*dsa)*urs+(2.*dsa)**2*uss )+ (1./12.)*( (2.*dra)**4*urrrr + 
     & 4.*(2.*dra)**3*(2.*dsa)*urrrs + 6.*(2.*dra)**2*(2.*dsa)**2*
     & urrss + 4.*(2.*dra)*(2.*dsa)**3*ursss + (2.*dsa)**4*ussss )
                          u(i1-2*is1,i2-2*is2,i3,ey)= 2.*u(i1,i2,i3,ey)
     & -u(i1+2*is1,i2+2*is2,i3,ey) + ( (2.*dra)**2*vrr+2.*(2.*dra)*(
     & 2.*dsa)*vrs+(2.*dsa)**2*vss )+ (1./12.)*( (2.*dra)**4*vrrrr + 
     & 4.*(2.*dra)**3*(2.*dsa)*vrrrs + 6.*(2.*dra)**2*(2.*dsa)**2*
     & vrrss + 4.*(2.*dra)*(2.*dsa)**3*vrsss + (2.*dsa)**4*vssss )
                        setCornersToExact=.false.
                        ! check errors
                           call ogf2dfo(ep,fieldOption,xy(i1-is1,i2-
     & is2,i3,0),xy(i1-is1,i2-is2,i3,1),t,uv0(0),uv0(1),uv0(2))
                          if( debug.gt.0 ) write(*,'(" 
     & ghostValuesOutsideCorners2d: i1-is1,i2-is2=",2i4," ex,err,ey,
     & err=",4e10.2)') i1-is1,i2-is2,u(i1-is1,i2-is2,i3,ex),u(i1-is1,
     & i2-is2,i3,ex)-uv0(0),u(i1-is1,i2-is2,i3,ey),u(i1-is1,i2-is2,i3,
     & ey)-uv0(1)
                          ! '
                          if( setCornersToExact )then
                            u(i1-is1,i2-is2,i3,ex)=uv0(0)
                            u(i1-is1,i2-is2,i3,ey)=uv0(1)
                          end if
                           call ogf2dfo(ep,fieldOption,xy(i1-2*is1,i2-
     & is2,i3,0),xy(i1-2*is1,i2-is2,i3,1),t,uv0(0),uv0(1),uv0(2))
                          if( debug.gt.0 ) write(*,'(" 
     & ghostValuesOutsideCorners2d: i1-2*is1,i2-is2=",2i4," ex,err,ey,
     & err=",4e10.2)') i1-2*is1,i2-is2,u(i1-2*is1,i2-is2,i3,ex),u(i1-
     & 2*is1,i2-is2,i3,ex)-uv0(0),u(i1-2*is1,i2-is2,i3,ey),u(i1-2*is1,
     & i2-is2,i3,ey)-uv0(1)
                          ! '
                          if( setCornersToExact )then
                            u(i1-2*is1,i2-is2,i3,ex)=uv0(0)
                            u(i1-2*is1,i2-is2,i3,ey)=uv0(1)
                          end if
                           call ogf2dfo(ep,fieldOption,xy(i1-is1,i2-2*
     & is2,i3,0),xy(i1-is1,i2-2*is2,i3,1),t,uv0(0),uv0(1),uv0(2))
                          if( debug.gt.0 ) write(*,'(" 
     & ghostValuesOutsideCorners2d: i1-is1,i2-2*is2=",2i4," ex,err,ey,
     & err=",4e10.2)') i1-is1,i2-2*is2,u(i1-is1,i2-2*is2,i3,ex),u(i1-
     & is1,i2-2*is2,i3,ex)-uv0(0),u(i1-is1,i2-2*is2,i3,ey),u(i1-is1,
     & i2-2*is2,i3,ey)-uv0(1)
                          ! '
                          if( setCornersToExact )then
                            u(i1-is1,i2-2*is2,i3,ex)=uv0(0)
                            u(i1-is1,i2-2*is2,i3,ey)=uv0(1)
                          end if
                           call ogf2dfo(ep,fieldOption,xy(i1-2*is1,i2-
     & 2*is2,i3,0),xy(i1-2*is1,i2-2*is2,i3,1),t,uv0(0),uv0(1),uv0(2))
                          if( debug.gt.0 ) write(*,'(" 
     & ghostValuesOutsideCorners2d: i1-2*is1,i2-2*is2=",2i4," ex,err,
     & ey,err=",4e10.2)') i1-2*is1,i2-2*is2,u(i1-2*is1,i2-2*is2,i3,ex)
     & ,u(i1-2*is1,i2-2*is2,i3,ex)-uv0(0),u(i1-2*is1,i2-2*is2,i3,ey),
     & u(i1-2*is1,i2-2*is2,i3,ey)-uv0(1)
                          ! '
                          if( setCornersToExact )then
                            u(i1-2*is1,i2-2*is2,i3,ex)=uv0(0)
                            u(i1-2*is1,i2-2*is2,i3,ey)=uv0(1)
                          end if
                        ! --- Now do Hz ---
                        ur = ur4(i1,i2,i3,hz)
                        us = us4(i1,i2,i3,hz)
                        urrr=urrr2(i1,i2,i3,hz)
                        usss=usss2(i1,i2,i3,hz)
                        urrs=0. !  (from ur(0,s)=0 and us(r,0)=0)  ! ****************** fix for TZ
                        urss=0. !  (from ur(0,s)=0 and us(r,0)=0)
                           call ogf2dfo(ep,fieldOption,xy(i1-1,i2-1,i3,
     & 0),xy(i1-1,i2-1,i3,1),t,uvmm(0),uvmm(1),uvmm(2))
                           call ogf2dfo(ep,fieldOption,xy(i1,i2-1,i3,0)
     & ,xy(i1,i2-1,i3,1),t,uvzm(0),uvzm(1),uvzm(2))
                           call ogf2dfo(ep,fieldOption,xy(i1+1,i2-1,i3,
     & 0),xy(i1+1,i2-1,i3,1),t,uvpm(0),uvpm(1),uvpm(2))
                           call ogf2dfo(ep,fieldOption,xy(i1-1,i2,i3,0)
     & ,xy(i1-1,i2,i3,1),t,uvmz(0),uvmz(1),uvmz(2))
                           call ogf2dfo(ep,fieldOption,xy(i1,i2,i3,0),
     & xy(i1,i2,i3,1),t,uvzz(0),uvzz(1),uvzz(2))
                           call ogf2dfo(ep,fieldOption,xy(i1+1,i2,i3,0)
     & ,xy(i1+1,i2,i3,1),t,uvpz(0),uvpz(1),uvpz(2))
                           call ogf2dfo(ep,fieldOption,xy(i1-1,i2+1,i3,
     & 0),xy(i1-1,i2+1,i3,1),t,uvmp(0),uvmp(1),uvmp(2))
                           call ogf2dfo(ep,fieldOption,xy(i1,i2+1,i3,0)
     & ,xy(i1,i2+1,i3,1),t,uvzp(0),uvzp(1),uvzp(2))
                           call ogf2dfo(ep,fieldOption,xy(i1+1,i2+1,i3,
     & 0),xy(i1+1,i2+1,i3,1),t,uvpp(0),uvpp(1),uvpp(2))
                          urrs=( (uvpp(2)-2.*uvzp(2)+uvmp(2))-(uvpm(2)-
     & 2.*uvzm(2)+uvmm(2)) )/(2.*dr(1)*dra**2)
                          urss=( (uvpp(2)-2.*uvpz(2)+uvpm(2))-(uvmp(2)-
     & 2.*uvmz(2)+uvmm(2)) )/(2.*dr(0)*dsa**2)
                          ! stop 6666
                    !   write(*,'(" ghostValuesOutsideCorners2d: i1,i2,is1,is2=",4i4," dra,dsa=",2e10.2," urrr,usss,urrs,urss=",4e10.2)')i1,i2,is1,is2,dra,dsa,urrr,usss,urrs,urss
                    !    urrr=0.
                    !    usss=0.
                    !    urrs=0.
                    !   urss=0.
                            u(i1-is1,i2-is2,i3,hz)=u(i1+is1,i2+is2,i3,
     & hz) - 2.*((dra)*ur+(dsa)*us) - (1./3.)*((dra)**3*urrr+3.*(dra)*
     & *2*(dsa)*urrs+3.*(dra)*(dsa)**2*urss+(dsa)**3*usss)
                            u(i1-2*is1,i2-is2,i3,hz)=u(i1+2*is1,i2+is2,
     & i3,hz) - 2.*((2.*dra)*ur+(dsa)*us) - (1./3.)*((2.*dra)**3*urrr+
     & 3.*(2.*dra)**2*(dsa)*urrs+3.*(2.*dra)*(dsa)**2*urss+(dsa)**3*
     & usss)
                            u(i1-is1,i2-2*is2,i3,hz)=u(i1+is1,i2+2*is2,
     & i3,hz) - 2.*((dra)*ur+(2.*dsa)*us) - (1./3.)*((dra)**3*urrr+3.*
     & (dra)**2*(2.*dsa)*urrs+3.*(dra)*(2.*dsa)**2*urss+(2.*dsa)**3*
     & usss)
                            u(i1-2*is1,i2-2*is2,i3,hz)=u(i1+2*is1,i2+2*
     & is2,i3,hz) - 2.*((2.*dra)*ur+(2.*dsa)*us) - (1./3.)*((2.*dra)**
     & 3*urrr+3.*(2.*dra)**2*(2.*dsa)*urrs+3.*(2.*dra)*(2.*dsa)**2*
     & urss+(2.*dsa)**3*usss)
                    !      setCornersToExact=.true.
                    !      if( setCornersToExact )then
                    !        OGF2DFO(i1-is1,i2-is2,i3,t, uv0(0),uv0(1),uv0(2))
                    !        write(*,'(" ghostValuesOutsideCorners2d: i1-is1,i2-is2=",2i4," hz,err=",4e10.2)') i1-is1,i2-is2,!                 u(i1-is1,i2-is2,i3,hz),u(i1-is1,i2-is2,i3,hz)-uv0(2)
                    !        u(i1-is1,i2-is2,i3,hz)=uv0(2)
                    !        OGF2DFO(i1-2*is1,i2-is2,i3,t, uv0(0),uv0(1),uv0(2))
                    !        u(i1-2*is1,i2-is2,i3,hz)=uv0(2)
                    !        write(*,'(" ghostValuesOutsideCorners2d: i1-2*is1,i2-is2=",2i4," hz,err=",4e10.2)') i1-2*is1,i2-is2,!                 u(i1-2*is1,i2-is2,i3,hz),u(i1-2*is1,i2-is2,i3,hz)-uv0(2)
                    !        OGF2DFO(i1-is1,i2-2*is2,i3,t, uv0(0),uv0(1),uv0(2))
                    !        write(*,'(" ghostValuesOutsideCorners2d: i1-is1,i2-2*is2=",2i4," hz,err=",4e10.2)') i1-is1,i2-2*is2,!                 u(i1-is1,i2-2*is2,i3,hz),u(i1-is1,i2-2*is2,i3,hz)-uv0(2)
                    !        u(i1-is1,i2-2*is2,i3,hz)=uv0(2)
                    !        OGF2DFO(i1-2*is1,i2-2*is2,i3,t, uv0(0),uv0(1),uv0(2))
                    !        write(*,'(" ghostValuesOutsideCorners2d: i1-2*is1,i2-2*is2=",2i4," hz,err=",4e10.2)') i1-2*is1,i2-2*is2,!                 u(i1-2*is1,i2-2*is2,i3,hz),u(i1-2*is1,i2-2*is2,i3,hz)-uv0(2)
                    !        u(i1-2*is1,i2-2*is2,i3,hz)=uv0(2)
                    !     end if
                else if( boundaryCondition(side1,0).ge.abcEM2 .and. 
     & boundaryCondition(side1,0).le.lastBC .and. boundaryCondition(
     & side2,1).ge.abcEM2 .and. boundaryCondition(side2,1).le.lastBC )
     & then
                  ! **** do nothing *** this is done in abcMaxwell
                end if
                end do
                end do
            end if
          end if
        else
          if( gridType.eq.rectangular )then
            if( useForcing.eq.0 )then
                numberOfGhostPoints=orderOfAccuracy/2
                ! Assign the edges
                 do edgeDirection=0,2 ! direction parallel to the edge
                ! do edgeDirection=0,0 ! direction parallel to the edge
                 do sidea=0,1
                 do sideb=0,1
                   if( edgeDirection.eq.0 )then
                     side1=0
                     side2=sidea
                     side3=sideb
                   else if( edgeDirection.eq.1 )then
                     side1=sideb
                     side2=0
                     side3=sidea
                   else
                     side1=sidea
                     side2=sideb
                     side3=0
                   end if
                 is1=1-2*(side1)
                 is2=1-2*(side2)
                 is3=1-2*(side3)
                 if( edgeDirection.eq.2 )then
                  is3=0
                  n1a=gridIndexRange(side1,0)
                  n1b=gridIndexRange(side1,0)
                  n2a=gridIndexRange(side2,1)
                  n2b=gridIndexRange(side2,1)
                  n3a=gridIndexRange(0,2)
                  n3b=gridIndexRange(1,2)
                  bc1=boundaryCondition(side1,0)
                  bc2=boundaryCondition(side2,1)
                 else if( edgeDirection.eq.1 )then
                  is2=0
                  n1a=gridIndexRange(side1,0)
                  n1b=gridIndexRange(side1,0)
                  n2a=gridIndexRange(    0,1)
                  n2b=gridIndexRange(    1,1)
                  n3a=gridIndexRange(side3,2)
                  n3b=gridIndexRange(side3,2)
                  bc1=boundaryCondition(side1,0)
                  bc2=boundaryCondition(side3,2)
                 else
                  is1=0
                  n1a=gridIndexRange(    0,0)
                  n1b=gridIndexRange(    1,0)
                  n2a=gridIndexRange(side2,1)
                  n2b=gridIndexRange(side2,1)
                  n3a=gridIndexRange(side3,2)
                  n3b=gridIndexRange(side3,2)
                  bc1=boundaryCondition(side2,1)
                  bc2=boundaryCondition(side3,2)
                 end if
                 g1=0.
                 g2=0.
                 g3=0.
                 ! ********************************************************************
                 ! ***************Assign Extended boundary points**********************
                 ! ************** on an edge between two faces   ********************** 
                 ! ********************************************************************
                  ! ************** CARTESIAN -- EXTENDED NEAR TWO FACES ************
                  do m=1,numberOfGhostPoints
                   js1=is1*m  ! shift to ghost point "m"
                   js2=is2*m
                   js3=is3*m
                   if( bc1.eq.perfectElectricalConductor 
     & .and.bc2.eq.perfectElectricalConductor )then
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                      if( edgeDirection.ne.0 )then
                        u(i1-js1,i2,i3,ex)=                  u(i1+js1,
     & i2,i3,ex) +g1
                        u(i1-js1,i2,i3,ey)=2.*u(i1,i2,i3,ey)-u(i1+js1,
     & i2,i3,ey) +g2
                        u(i1-js1,i2,i3,ez)=2.*u(i1,i2,i3,ez)-u(i1+js1,
     & i2,i3,ez) +g3
                      end if
                      if( edgeDirection.ne.1 )then
                        u(i1,i2-js2,i3,ex)=2.*u(i1,i2,i3,ex)-u(i1,i2+
     & js2,i3,ex) +g1
                        u(i1,i2-js2,i3,ey)=                  u(i1,i2+
     & js2,i3,ey)+g2
                        u(i1,i2-js2,i3,ez)=2.*u(i1,i2,i3,ez)-u(i1,i2+
     & js2,i3,ez) +g3
                      end if
                      if( edgeDirection.ne.2 )then
                        u(i1,i2,i3-js3,ex)=2.*u(i1,i2,i3,ex)-u(i1,i2,
     & i3+js3,ex) +g1
                        u(i1,i2,i3-js3,ey)=2.*u(i1,i2,i3,ey)-u(i1,i2,
     & i3+js3,ey) +g2
                        u(i1,i2,i3-js3,ez)=                   u(i1,i2,
     & i3+js3,ez)+g3
                      end if
                     end do ! end do i1
                     end do ! end do i2
                     end do ! end do i3
                   else if( bc1.eq.dirichlet .or. bc2.eq.dirichlet )
     & then
                     ! *wdh* 081124 -- do nothing here ---
                ! *     do i3=n3a,n3b
                ! *     do i2=n2a,n2b
                ! *     do i1=n1a,n1b
                ! *
                ! *      if( edgeDirection.ne.0 )then
                ! *        #If "none" == "twilightZone"
                ! *          OGF3DFO(i1-js1,i2,i3,t,g1,g2,g3)
                ! *        #End
                ! *        u(i1-js1,i2,i3,ex)=g1
                ! *        u(i1-js1,i2,i3,ey)=g2
                ! *        u(i1-js1,i2,i3,ez)=g3
                ! *      end if
                ! *
                ! *      if( edgeDirection.ne.1 )then
                ! *        #If "none" == "twilightZone"
                ! *          OGF3DFO(i1,i2-js2,i3,t,g1,g2,g3)
                ! *        #End
                ! *        u(i1,i2-js2,i3,ex)=g1
                ! *        u(i1,i2-js2,i3,ey)=g2
                ! *        u(i1,i2-js2,i3,ez)=g3
                ! *      end if
                ! *
                ! *      if( edgeDirection.ne.2 )then
                ! *        #If "none" == "twilightZone"
                ! *          OGF3DFO(i1,i2,i3-js3,t,g1,g2,g3)
                ! *        #End
                ! *        u(i1,i2,i3-js3,ex)=g1
                ! *        u(i1,i2,i3-js3,ey)=g2
                ! *        u(i1,i2,i3-js3,ez)=g3
                ! *      end if
                ! *
                ! *     end do ! end do i1
                ! *     end do ! end do i2
                ! *     end do ! end do i3
                   else if( bc1.le.0 .or. bc2.le.0 )then
                    ! periodic or interpolation -- nothing to do
                   else if( bc1.eq.planeWaveBoundaryCondition 
     & .or.bc2.eq.planeWaveBoundaryCondition .or. 
     & bc1.eq.symmetryBoundaryCondition .or. 
     & bc2.eq.symmetryBoundaryCondition .or. (bc1.ge.abcEM2 .and. 
     & bc1.le.lastBC) .or. (bc2.ge.abcEM2 .and. bc2.le.lastBC))then
                     ! do nothing
                   else
                     write(*,'("ERROR: unknown boundary conditions bc1,
     & bc2=",2i3)') bc1,bc2
                     ! unknown boundary conditions
                      stop 8866
                   end if
                  end do ! end do m
                 end do
                 end do
                 end do ! edge direction
                 ! *****************************************************************************
                 ! ************ assign corner GHOST points outside edges ***********************
                 ! ****************************************************************************
                  do edgeDirection=0,2 ! direction parallel to the edge
                 ! do edgeDirection=2,2 ! direction parallel to the edge
                  do sidea=0,1
                  do sideb=0,1
                   if( edgeDirection.eq.0 )then
                     side1=0
                     side2=sidea
                     side3=sideb
                   else if( edgeDirection.eq.1 )then
                     side1=sideb
                     side2=0
                     side3=sidea
                   else
                     side1=sidea
                     side2=sideb
                     side3=0
                   end if
                  extra=numberOfGhostPoints  ! assign the extended boundary *wdh* 2015/06/23
                  is1=1-2*(side1)
                  is2=1-2*(side2)
                  is3=1-2*(side3)
                  if( edgeDirection.eq.2 )then
                   is3=0
                   n1a=gridIndexRange(side1,0)
                   n1b=gridIndexRange(side1,0)
                   n2a=gridIndexRange(side2,1)
                   n2b=gridIndexRange(side2,1)
                   n3a=gridIndexRange(0,2)-extra
                   n3b=gridIndexRange(1,2)+extra
                   bc1=boundaryCondition(side1,0)
                   bc2=boundaryCondition(side2,1)
                  else if( edgeDirection.eq.1 )then
                   is2=0
                   n1a=gridIndexRange(side1,0)
                   n1b=gridIndexRange(side1,0)
                   n2a=gridIndexRange(    0,1)-extra
                   n2b=gridIndexRange(    1,1)+extra
                   n3a=gridIndexRange(side3,2)
                   n3b=gridIndexRange(side3,2)
                   bc1=boundaryCondition(side1,0)
                   bc2=boundaryCondition(side3,2)
                  else
                   is1=0
                   n1a=gridIndexRange(    0,0)-extra
                   n1b=gridIndexRange(    1,0)+extra
                   n2a=gridIndexRange(side2,1)
                   n2b=gridIndexRange(side2,1)
                   n3a=gridIndexRange(side3,2)
                   n3b=gridIndexRange(side3,2)
                   bc1=boundaryCondition(side2,1)
                   bc2=boundaryCondition(side3,2)
                  end if
                  ! *********************************************************
                  ! ************* Assign Ghost near two faces ***************
                  ! *************       CARTESIAN              **************
                  ! *********************************************************
                  do m1=1,numberOfGhostPoints
                  do m2=1,numberOfGhostPoints
                   ! shift to ghost point "(m1,m2)"
                   if( edgeDirection.eq.2 )then
                     js1=is1*m1
                     js2=is2*m2
                     js3=0
                   else if( edgeDirection.eq.1 )then
                     js1=is1*m1
                     js2=0
                     js3=is3*m2
                   else
                     js1=0
                     js2=is2*m1
                     js3=is3*m2
                   end if
                   if( bc1.eq.perfectElectricalConductor .and. 
     & bc2.eq.perfectElectricalConductor )then
                    ! *********************************************************
                    ! ************* PEC EDGE BC (CARTESIAN) *******************
                    ! *********************************************************
                    ! bug fixed *wdh* 2015/07/12 -- one component is even along an edge
                    if( edgeDirection.eq.0 )then
                     ! --- edge parallel to the x-axis ----
                     !  Ey and Ez are odd, Ex is even 
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                       ! We could check the mask ***
                      u(i1-js1,i2-js2,i3-js3,ex)=                  u(
     & i1+js1,i2+js2,i3+js3,ex) +g1
                      u(i1-js1,i2-js2,i3-js3,ey)=2.*u(i1,i2,i3,ey)-u(
     & i1+js1,i2+js2,i3+js3,ey) +g2
                      u(i1-js1,i2-js2,i3-js3,ez)=2.*u(i1,i2,i3,ez)-u(
     & i1+js1,i2+js2,i3+js3,ez) +g3
                     end do ! end do i1
                     end do ! end do i2
                     end do ! end do i3
                    else if( edgeDirection.eq.1 )then
                     ! --- edge parallel to the y-axis ----
                     !  Ex and Ez are odd, Ey is even 
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                       ! We could check the mask ***
                      u(i1-js1,i2-js2,i3-js3,ex)=2.*u(i1,i2,i3,ex)-u(
     & i1+js1,i2+js2,i3+js3,ex) +g1
                      u(i1-js1,i2-js2,i3-js3,ey)=                  u(
     & i1+js1,i2+js2,i3+js3,ey) +g2
                      u(i1-js1,i2-js2,i3-js3,ez)=2.*u(i1,i2,i3,ez)-u(
     & i1+js1,i2+js2,i3+js3,ez) +g3
                     end do ! end do i1
                     end do ! end do i2
                     end do ! end do i3
                    else
                     ! --- edge parallel to the z-axis ----
                     !  Ex and Ey are odd, Ez is even 
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                       ! We could check the mask ***
                      u(i1-js1,i2-js2,i3-js3,ex)=2.*u(i1,i2,i3,ex)-u(
     & i1+js1,i2+js2,i3+js3,ex) +g1
                      u(i1-js1,i2-js2,i3-js3,ey)=2.*u(i1,i2,i3,ey)-u(
     & i1+js1,i2+js2,i3+js3,ey) +g2
                      u(i1-js1,i2-js2,i3-js3,ez)=                  u(
     & i1+js1,i2+js2,i3+js3,ez) +g3
                     end do ! end do i1
                     end do ! end do i2
                     end do ! end do i3
                    end if
                   else if( bc1.eq.perfectElectricalConductor .or. 
     & bc2.eq.perfectElectricalConductor )then
                    ! *********************************************************************
                    ! ******* PEC FACE ADJACENT to NON-PEC FACE (CARTESIAN) ***************
                    ! *********************************************************************
                    ! -- assign edge ghost where a PEC face meets an interp face (e.g. twoBox)
                    !     *wdh* 2015/07/11 
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                       ! We could check the mask ***
                        u(i1-js1,i2-js2,i3-js3,ex)=(5.*u(i1,i2,i3,ex)-
     & 10.*u(i1+js1,i2+js2,i3+js3,ex)+10.*u(i1+2*js1,i2+2*js2,i3+2*
     & js3,ex)-5.*u(i1+3*js1,i2+3*js2,i3+3*js3,ex)+u(i1+4*js1,i2+4*
     & js2,i3+4*js3,ex))
                        u(i1-js1,i2-js2,i3-js3,ey)=(5.*u(i1,i2,i3,ey)-
     & 10.*u(i1+js1,i2+js2,i3+js3,ey)+10.*u(i1+2*js1,i2+2*js2,i3+2*
     & js3,ey)-5.*u(i1+3*js1,i2+3*js2,i3+3*js3,ey)+u(i1+4*js1,i2+4*
     & js2,i3+4*js3,ey))
                        u(i1-js1,i2-js2,i3-js3,ez)=(5.*u(i1,i2,i3,ez)-
     & 10.*u(i1+js1,i2+js2,i3+js3,ez)+10.*u(i1+2*js1,i2+2*js2,i3+2*
     & js3,ez)-5.*u(i1+3*js1,i2+3*js2,i3+3*js3,ez)+u(i1+4*js1,i2+4*
     & js2,i3+4*js3,ez))
                     end do ! end do i1
                     end do ! end do i2
                     end do ! end do i3
                   else if( bc1.eq.dirichlet .or. bc2.eq.dirichlet )
     & then
                     ! *wdh* 081124 -- do nothing here ---
                     ! This is a dirichlet BC 
                 ! *     do i3=n3a,n3b
                 ! *     do i2=n2a,n2b
                 ! *     do i1=n1a,n1b
                 ! * 
                 ! *      #If "none" == "twilightZone"
                 ! *        OGF3DFO(i1-js1,i2-js2,i3-js3,t, g1,g2,g3)
                 ! *      #End
                 ! *      u(i1-js1,i2-js2,i3-js3,ex)=g1
                 ! *      u(i1-js1,i2-js2,i3-js3,ey)=g2
                 ! *      u(i1-js1,i2-js2,i3-js3,ez)=g3
                 ! * 
                 ! *     end do ! end do i1
                 ! *     end do ! end do i2
                 ! *     end do ! end do i3
                   else if( bc1.le.0 .or. bc2.le.0 )then
                     ! periodic or interpolation -- nothing to do
                   else if( bc1.eq.planeWaveBoundaryCondition 
     & .or.bc2.eq.planeWaveBoundaryCondition .or. 
     & bc1.eq.symmetryBoundaryCondition .or. 
     & bc2.eq.symmetryBoundaryCondition .or. (bc1.ge.abcEM2 .and. 
     & bc1.le.lastBC) .or. (bc2.ge.abcEM2 .and. bc2.le.lastBC) )then
                      ! do nothing
                   else
                     write(*,'("ERROR: unknown boundary conditions bc1,
     & bc2=",2i3)') bc1,bc2
                     ! unknown boundary conditions
                     stop 8866
                   end if
                  end do ! end do m1
                  end do ! end do m2
                  end do
                  end do
                  end do  ! edge direction
                ! Finally assign points outside the vertices of the unit cube
                g1=0.
                g2=0.
                g3=0.
                do side3=0,1
                do side2=0,1
                do side1=0,1
                 ! assign ghost values outside the corner (vertex)
                 i1=gridIndexRange(side1,0)
                 i2=gridIndexRange(side2,1)
                 i3=gridIndexRange(side3,2)
                 is1=1-2*side1
                 is2=1-2*side2
                 is3=1-2*side3
                 if( boundaryCondition(side1,0)
     & .eq.perfectElectricalConductor .and.boundaryCondition(side2,1)
     & .eq.perfectElectricalConductor .and.boundaryCondition(side3,2)
     & .eq.perfectElectricalConductor )then
                   ! ------------------------------------------------------------
                   ! -------------- VERTEX adjacent to 3 PEC faces --------------
                   ! ------------------------------------------------------------
                  do m3=1,numberOfGhostPoints
                  do m2=1,numberOfGhostPoints
                  do m1=1,numberOfGhostPoints
                    js1=is1*m1  ! shift to ghost point "m"
                    js2=is2*m2
                    js3=is3*m3
                    dra=dr(0)*js1
                    dsa=dr(1)*js2
                    dta=dr(2)*js3
                     ! *wdh* 2015/07/12 -- I think this is wrong: *fix me*
                     !   For  PEC corner:  E(-dx,-dy,-dz) = E(dx,dy,dz) 
                     u(i1-js1,i2-js2,i3-js3,ex)=u(i1+js1,i2+js2,i3+js3,
     & ex)+g1
                     u(i1-js1,i2-js2,i3-js3,ey)=u(i1+js1,i2+js2,i3+js3,
     & ey)+g2
                     u(i1-js1,i2-js2,i3-js3,ez)=u(i1+js1,i2+js2,i3+js3,
     & ez)+g3
                     ! u(i1-js1,i2-js2,i3-js3,ex)=2.*u(i1,i2,i3,ex)-u(i1+js1,i2+js2,i3+js3,ex)+g1
                     ! u(i1-js1,i2-js2,i3-js3,ey)=2.*u(i1,i2,i3,ey)-u(i1+js1,i2+js2,i3+js3,ey)+g2
                     ! u(i1-js1,i2-js2,i3-js3,ez)=2.*u(i1,i2,i3,ez)-u(i1+js1,i2+js2,i3+js3,ez)+g3
                  end do
                  end do
                  end do
                 else if( .true. .and. (boundaryCondition(side1,0)
     & .eq.perfectElectricalConductor .or.boundaryCondition(side2,1)
     & .eq.perfectElectricalConductor .or.boundaryCondition(side3,2)
     & .eq.perfectElectricalConductor) )then
                   ! *new* *wdh* 2015/07/12 
                   ! -----------------------------------------------------------------
                   ! -------------- VERTEX adjacent to 1 or 2 PEC faces --------------
                   ! -----------------------------------------------------------------
                  do m3=1,numberOfGhostPoints
                  do m2=1,numberOfGhostPoints
                  do m1=1,numberOfGhostPoints
                    js1=is1*m1  ! shift to ghost point "m"
                    js2=is2*m2
                    js3=is3*m3
                     u(i1-js1,i2-js2,i3-js3,ex)=(5.*u(i1,i2,i3,ex)-10.*
     & u(i1+js1,i2+js2,i3+js3,ex)+10.*u(i1+2*js1,i2+2*js2,i3+2*js3,ex)
     & -5.*u(i1+3*js1,i2+3*js2,i3+3*js3,ex)+u(i1+4*js1,i2+4*js2,i3+4*
     & js3,ex))
                     u(i1-js1,i2-js2,i3-js3,ey)=(5.*u(i1,i2,i3,ey)-10.*
     & u(i1+js1,i2+js2,i3+js3,ey)+10.*u(i1+2*js1,i2+2*js2,i3+2*js3,ey)
     & -5.*u(i1+3*js1,i2+3*js2,i3+3*js3,ey)+u(i1+4*js1,i2+4*js2,i3+4*
     & js3,ey))
                     u(i1-js1,i2-js2,i3-js3,ez)=(5.*u(i1,i2,i3,ez)-10.*
     & u(i1+js1,i2+js2,i3+js3,ez)+10.*u(i1+2*js1,i2+2*js2,i3+2*js3,ez)
     & -5.*u(i1+3*js1,i2+3*js2,i3+3*js3,ez)+u(i1+4*js1,i2+4*js2,i3+4*
     & js3,ez))
                  end do ! end do m1
                  end do ! end do m2
                  end do ! end do m3
                 else if( boundaryCondition(side1,0).eq.dirichlet 
     & .or.boundaryCondition(side2,1).eq.dirichlet 
     & .or.boundaryCondition(side3,2).eq.dirichlet )then
                  ! *wdh* 081124 -- do nothing here ---
              ! *    do m3=1,numberOfGhostPoints
              ! *    do m2=1,numberOfGhostPoints
              ! *    do m1=1,numberOfGhostPoints
              ! *
              ! *      js1=is1*m1  ! shift to ghost point "m"
              ! *      js2=is2*m2
              ! *      js3=is3*m3
              ! *
              ! *      #If "none" == "twilightZone" 
              ! *        OGF3DFO(i1-js1,i2-js2,i3-js3,t, g1,g2,g3)
              ! *      #End
              ! *      u(i1-js1,i2-js2,i3-js3,ex)=g1
              ! *      u(i1-js1,i2-js2,i3-js3,ey)=g2
              ! *      u(i1-js1,i2-js2,i3-js3,ez)=g3
              ! *
              ! *    end do
              ! *    end do
              ! *    end do
                 else if( boundaryCondition(side1,0).le.0 
     & .or.boundaryCondition(side2,1).le.0 .or.boundaryCondition(
     & side3,2).le.0 )then
                    ! one or more boundaries are periodic or interpolation -- nothing to do
                 else if( boundaryCondition(side1,0)
     & .eq.planeWaveBoundaryCondition .or.boundaryCondition(side2,1)
     & .eq.planeWaveBoundaryCondition .or.boundaryCondition(side3,2)
     & .eq.planeWaveBoundaryCondition  .or. boundaryCondition(side1,0)
     & .eq.symmetryBoundaryCondition .or. boundaryCondition(side2,1)
     & .eq.symmetryBoundaryCondition .or. boundaryCondition(side3,2)
     & .eq.symmetryBoundaryCondition .or. (boundaryCondition(side1,0)
     & .ge.abcEM2 .and. boundaryCondition(side1,0).le.lastBC) .or. (
     & boundaryCondition(side2,1).ge.abcEM2 .and. boundaryCondition(
     & side2,1).le.lastBC) .or. (boundaryCondition(side3,2).ge.abcEM2 
     & .and. boundaryCondition(side3,2).le.lastBC)  )then
                   ! do nothing
                 else
                   write(*,'("ERROR: unknown boundary conditions at a 
     & 3D corner bc1,bc2,bc3=",2i3)') boundaryCondition(side1,0),
     & boundaryCondition(side2,1),boundaryCondition(side3,2)
                   ! '
                   ! unknown boundary conditions
                   stop 3399
                 end if
                end do
                end do
                end do
            else
                numberOfGhostPoints=orderOfAccuracy/2
                ! Assign the edges
                 do edgeDirection=0,2 ! direction parallel to the edge
                ! do edgeDirection=0,0 ! direction parallel to the edge
                 do sidea=0,1
                 do sideb=0,1
                   if( edgeDirection.eq.0 )then
                     side1=0
                     side2=sidea
                     side3=sideb
                   else if( edgeDirection.eq.1 )then
                     side1=sideb
                     side2=0
                     side3=sidea
                   else
                     side1=sidea
                     side2=sideb
                     side3=0
                   end if
                 is1=1-2*(side1)
                 is2=1-2*(side2)
                 is3=1-2*(side3)
                 if( edgeDirection.eq.2 )then
                  is3=0
                  n1a=gridIndexRange(side1,0)
                  n1b=gridIndexRange(side1,0)
                  n2a=gridIndexRange(side2,1)
                  n2b=gridIndexRange(side2,1)
                  n3a=gridIndexRange(0,2)
                  n3b=gridIndexRange(1,2)
                  bc1=boundaryCondition(side1,0)
                  bc2=boundaryCondition(side2,1)
                 else if( edgeDirection.eq.1 )then
                  is2=0
                  n1a=gridIndexRange(side1,0)
                  n1b=gridIndexRange(side1,0)
                  n2a=gridIndexRange(    0,1)
                  n2b=gridIndexRange(    1,1)
                  n3a=gridIndexRange(side3,2)
                  n3b=gridIndexRange(side3,2)
                  bc1=boundaryCondition(side1,0)
                  bc2=boundaryCondition(side3,2)
                 else
                  is1=0
                  n1a=gridIndexRange(    0,0)
                  n1b=gridIndexRange(    1,0)
                  n2a=gridIndexRange(side2,1)
                  n2b=gridIndexRange(side2,1)
                  n3a=gridIndexRange(side3,2)
                  n3b=gridIndexRange(side3,2)
                  bc1=boundaryCondition(side2,1)
                  bc2=boundaryCondition(side3,2)
                 end if
                 g1=0.
                 g2=0.
                 g3=0.
                 ! ********************************************************************
                 ! ***************Assign Extended boundary points**********************
                 ! ************** on an edge between two faces   ********************** 
                 ! ********************************************************************
                  ! ************** CARTESIAN -- EXTENDED NEAR TWO FACES ************
                  do m=1,numberOfGhostPoints
                   js1=is1*m  ! shift to ghost point "m"
                   js2=is2*m
                   js3=is3*m
                   if( bc1.eq.perfectElectricalConductor 
     & .and.bc2.eq.perfectElectricalConductor )then
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                          call ogf3dfo(ep,fieldOption,xy(i1,i2,i3,0),
     & xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,u0,v0,w0)
                      if( edgeDirection.ne.0 )then
                            call ogf3dfo(ep,fieldOption,xy(i1-js1,i2,
     & i3,0),xy(i1-js1,i2,i3,1),xy(i1-js1,i2,i3,2),t,um,vm,wm)
                            call ogf3dfo(ep,fieldOption,xy(i1+js1,i2,
     & i3,0),xy(i1+js1,i2,i3,1),xy(i1+js1,i2,i3,2),t,up,vp,wp)
                          g1=um-up
                          g2=vm-2.*v0+vp
                          g3=wm-2.*w0+wp
                        u(i1-js1,i2,i3,ex)=                  u(i1+js1,
     & i2,i3,ex) +g1
                        u(i1-js1,i2,i3,ey)=2.*u(i1,i2,i3,ey)-u(i1+js1,
     & i2,i3,ey) +g2
                        u(i1-js1,i2,i3,ez)=2.*u(i1,i2,i3,ez)-u(i1+js1,
     & i2,i3,ez) +g3
                      end if
                      if( edgeDirection.ne.1 )then
                            call ogf3dfo(ep,fieldOption,xy(i1,i2-js2,
     & i3,0),xy(i1,i2-js2,i3,1),xy(i1,i2-js2,i3,2),t,um,vm,wm)
                            call ogf3dfo(ep,fieldOption,xy(i1,i2+js2,
     & i3,0),xy(i1,i2+js2,i3,1),xy(i1,i2+js2,i3,2),t,up,vp,wp)
                          g1=um-2.*u0+up
                          g2=vm-vp
                          g3=wm-2.*w0+wp
                        u(i1,i2-js2,i3,ex)=2.*u(i1,i2,i3,ex)-u(i1,i2+
     & js2,i3,ex) +g1
                        u(i1,i2-js2,i3,ey)=                  u(i1,i2+
     & js2,i3,ey)+g2
                        u(i1,i2-js2,i3,ez)=2.*u(i1,i2,i3,ez)-u(i1,i2+
     & js2,i3,ez) +g3
                      end if
                      if( edgeDirection.ne.2 )then
                            call ogf3dfo(ep,fieldOption,xy(i1,i2,i3-
     & js3,0),xy(i1,i2,i3-js3,1),xy(i1,i2,i3-js3,2),t,um,vm,wm)
                            call ogf3dfo(ep,fieldOption,xy(i1,i2,i3+
     & js3,0),xy(i1,i2,i3+js3,1),xy(i1,i2,i3+js3,2),t,up,vp,wp)
                          g1=um-2.*u0+up
                          g2=vm-2.*v0+vp
                          g3=wm-wp
                        u(i1,i2,i3-js3,ex)=2.*u(i1,i2,i3,ex)-u(i1,i2,
     & i3+js3,ex) +g1
                        u(i1,i2,i3-js3,ey)=2.*u(i1,i2,i3,ey)-u(i1,i2,
     & i3+js3,ey) +g2
                        u(i1,i2,i3-js3,ez)=                   u(i1,i2,
     & i3+js3,ez)+g3
                      end if
                     end do ! end do i1
                     end do ! end do i2
                     end do ! end do i3
                   else if( bc1.eq.dirichlet .or. bc2.eq.dirichlet )
     & then
                     ! *wdh* 081124 -- do nothing here ---
                ! *     do i3=n3a,n3b
                ! *     do i2=n2a,n2b
                ! *     do i1=n1a,n1b
                ! *
                ! *      if( edgeDirection.ne.0 )then
                ! *        #If "twilightZone" == "twilightZone"
                ! *          OGF3DFO(i1-js1,i2,i3,t,g1,g2,g3)
                ! *        #End
                ! *        u(i1-js1,i2,i3,ex)=g1
                ! *        u(i1-js1,i2,i3,ey)=g2
                ! *        u(i1-js1,i2,i3,ez)=g3
                ! *      end if
                ! *
                ! *      if( edgeDirection.ne.1 )then
                ! *        #If "twilightZone" == "twilightZone"
                ! *          OGF3DFO(i1,i2-js2,i3,t,g1,g2,g3)
                ! *        #End
                ! *        u(i1,i2-js2,i3,ex)=g1
                ! *        u(i1,i2-js2,i3,ey)=g2
                ! *        u(i1,i2-js2,i3,ez)=g3
                ! *      end if
                ! *
                ! *      if( edgeDirection.ne.2 )then
                ! *        #If "twilightZone" == "twilightZone"
                ! *          OGF3DFO(i1,i2,i3-js3,t,g1,g2,g3)
                ! *        #End
                ! *        u(i1,i2,i3-js3,ex)=g1
                ! *        u(i1,i2,i3-js3,ey)=g2
                ! *        u(i1,i2,i3-js3,ez)=g3
                ! *      end if
                ! *
                ! *     end do ! end do i1
                ! *     end do ! end do i2
                ! *     end do ! end do i3
                   else if( bc1.le.0 .or. bc2.le.0 )then
                    ! periodic or interpolation -- nothing to do
                   else if( bc1.eq.planeWaveBoundaryCondition 
     & .or.bc2.eq.planeWaveBoundaryCondition .or. 
     & bc1.eq.symmetryBoundaryCondition .or. 
     & bc2.eq.symmetryBoundaryCondition .or. (bc1.ge.abcEM2 .and. 
     & bc1.le.lastBC) .or. (bc2.ge.abcEM2 .and. bc2.le.lastBC))then
                     ! do nothing
                   else
                     write(*,'("ERROR: unknown boundary conditions bc1,
     & bc2=",2i3)') bc1,bc2
                     ! unknown boundary conditions
                      stop 8866
                   end if
                  end do ! end do m
                 end do
                 end do
                 end do ! edge direction
                 ! *****************************************************************************
                 ! ************ assign corner GHOST points outside edges ***********************
                 ! ****************************************************************************
                  do edgeDirection=0,2 ! direction parallel to the edge
                 ! do edgeDirection=2,2 ! direction parallel to the edge
                  do sidea=0,1
                  do sideb=0,1
                   if( edgeDirection.eq.0 )then
                     side1=0
                     side2=sidea
                     side3=sideb
                   else if( edgeDirection.eq.1 )then
                     side1=sideb
                     side2=0
                     side3=sidea
                   else
                     side1=sidea
                     side2=sideb
                     side3=0
                   end if
                  extra=numberOfGhostPoints  ! assign the extended boundary *wdh* 2015/06/23
                  is1=1-2*(side1)
                  is2=1-2*(side2)
                  is3=1-2*(side3)
                  if( edgeDirection.eq.2 )then
                   is3=0
                   n1a=gridIndexRange(side1,0)
                   n1b=gridIndexRange(side1,0)
                   n2a=gridIndexRange(side2,1)
                   n2b=gridIndexRange(side2,1)
                   n3a=gridIndexRange(0,2)-extra
                   n3b=gridIndexRange(1,2)+extra
                   bc1=boundaryCondition(side1,0)
                   bc2=boundaryCondition(side2,1)
                  else if( edgeDirection.eq.1 )then
                   is2=0
                   n1a=gridIndexRange(side1,0)
                   n1b=gridIndexRange(side1,0)
                   n2a=gridIndexRange(    0,1)-extra
                   n2b=gridIndexRange(    1,1)+extra
                   n3a=gridIndexRange(side3,2)
                   n3b=gridIndexRange(side3,2)
                   bc1=boundaryCondition(side1,0)
                   bc2=boundaryCondition(side3,2)
                  else
                   is1=0
                   n1a=gridIndexRange(    0,0)-extra
                   n1b=gridIndexRange(    1,0)+extra
                   n2a=gridIndexRange(side2,1)
                   n2b=gridIndexRange(side2,1)
                   n3a=gridIndexRange(side3,2)
                   n3b=gridIndexRange(side3,2)
                   bc1=boundaryCondition(side2,1)
                   bc2=boundaryCondition(side3,2)
                  end if
                  ! *********************************************************
                  ! ************* Assign Ghost near two faces ***************
                  ! *************       CARTESIAN              **************
                  ! *********************************************************
                  do m1=1,numberOfGhostPoints
                  do m2=1,numberOfGhostPoints
                   ! shift to ghost point "(m1,m2)"
                   if( edgeDirection.eq.2 )then
                     js1=is1*m1
                     js2=is2*m2
                     js3=0
                   else if( edgeDirection.eq.1 )then
                     js1=is1*m1
                     js2=0
                     js3=is3*m2
                   else
                     js1=0
                     js2=is2*m1
                     js3=is3*m2
                   end if
                   if( bc1.eq.perfectElectricalConductor .and. 
     & bc2.eq.perfectElectricalConductor )then
                    ! *********************************************************
                    ! ************* PEC EDGE BC (CARTESIAN) *******************
                    ! *********************************************************
                    ! bug fixed *wdh* 2015/07/12 -- one component is even along an edge
                    if( edgeDirection.eq.0 )then
                     ! --- edge parallel to the x-axis ----
                     !  Ey and Ez are odd, Ex is even 
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                       ! We could check the mask ***
                          call ogf3dfo(ep,fieldOption,xy(i1,i2,i3,0),
     & xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,u0,v0,w0)
                          call ogf3dfo(ep,fieldOption,xy(i1-js1,i2-js2,
     & i3-js3,0),xy(i1-js1,i2-js2,i3-js3,1),xy(i1-js1,i2-js2,i3-js3,2)
     & ,t,um,vm,wm)
                          call ogf3dfo(ep,fieldOption,xy(i1+js1,i2+js2,
     & i3+js3,0),xy(i1+js1,i2+js2,i3+js3,1),xy(i1+js1,i2+js2,i3+js3,2)
     & ,t,up,vp,wp)
                        g1=um      -up
                        g2=vm-2.*v0+vp
                        g3=wm-2.*w0+wp
                      u(i1-js1,i2-js2,i3-js3,ex)=                  u(
     & i1+js1,i2+js2,i3+js3,ex) +g1
                      u(i1-js1,i2-js2,i3-js3,ey)=2.*u(i1,i2,i3,ey)-u(
     & i1+js1,i2+js2,i3+js3,ey) +g2
                      u(i1-js1,i2-js2,i3-js3,ez)=2.*u(i1,i2,i3,ez)-u(
     & i1+js1,i2+js2,i3+js3,ez) +g3
                     end do ! end do i1
                     end do ! end do i2
                     end do ! end do i3
                    else if( edgeDirection.eq.1 )then
                     ! --- edge parallel to the y-axis ----
                     !  Ex and Ez are odd, Ey is even 
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                       ! We could check the mask ***
                          call ogf3dfo(ep,fieldOption,xy(i1,i2,i3,0),
     & xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,u0,v0,w0)
                          call ogf3dfo(ep,fieldOption,xy(i1-js1,i2-js2,
     & i3-js3,0),xy(i1-js1,i2-js2,i3-js3,1),xy(i1-js1,i2-js2,i3-js3,2)
     & ,t,um,vm,wm)
                          call ogf3dfo(ep,fieldOption,xy(i1+js1,i2+js2,
     & i3+js3,0),xy(i1+js1,i2+js2,i3+js3,1),xy(i1+js1,i2+js2,i3+js3,2)
     & ,t,up,vp,wp)
                        g1=um-2.*u0+up
                        g2=vm      -vp
                        g3=wm-2.*w0+wp
                      u(i1-js1,i2-js2,i3-js3,ex)=2.*u(i1,i2,i3,ex)-u(
     & i1+js1,i2+js2,i3+js3,ex) +g1
                      u(i1-js1,i2-js2,i3-js3,ey)=                  u(
     & i1+js1,i2+js2,i3+js3,ey) +g2
                      u(i1-js1,i2-js2,i3-js3,ez)=2.*u(i1,i2,i3,ez)-u(
     & i1+js1,i2+js2,i3+js3,ez) +g3
                     end do ! end do i1
                     end do ! end do i2
                     end do ! end do i3
                    else
                     ! --- edge parallel to the z-axis ----
                     !  Ex and Ey are odd, Ez is even 
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                       ! We could check the mask ***
                          call ogf3dfo(ep,fieldOption,xy(i1,i2,i3,0),
     & xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,u0,v0,w0)
                          call ogf3dfo(ep,fieldOption,xy(i1-js1,i2-js2,
     & i3-js3,0),xy(i1-js1,i2-js2,i3-js3,1),xy(i1-js1,i2-js2,i3-js3,2)
     & ,t,um,vm,wm)
                          call ogf3dfo(ep,fieldOption,xy(i1+js1,i2+js2,
     & i3+js3,0),xy(i1+js1,i2+js2,i3+js3,1),xy(i1+js1,i2+js2,i3+js3,2)
     & ,t,up,vp,wp)
                        g1=um-2.*u0+up
                        g2=vm-2.*v0+vp
                        g3=wm      -wp
                      u(i1-js1,i2-js2,i3-js3,ex)=2.*u(i1,i2,i3,ex)-u(
     & i1+js1,i2+js2,i3+js3,ex) +g1
                      u(i1-js1,i2-js2,i3-js3,ey)=2.*u(i1,i2,i3,ey)-u(
     & i1+js1,i2+js2,i3+js3,ey) +g2
                      u(i1-js1,i2-js2,i3-js3,ez)=                  u(
     & i1+js1,i2+js2,i3+js3,ez) +g3
                     end do ! end do i1
                     end do ! end do i2
                     end do ! end do i3
                    end if
                   else if( bc1.eq.perfectElectricalConductor .or. 
     & bc2.eq.perfectElectricalConductor )then
                    ! *********************************************************************
                    ! ******* PEC FACE ADJACENT to NON-PEC FACE (CARTESIAN) ***************
                    ! *********************************************************************
                    ! -- assign edge ghost where a PEC face meets an interp face (e.g. twoBox)
                    !     *wdh* 2015/07/11 
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                       ! We could check the mask ***
                        u(i1-js1,i2-js2,i3-js3,ex)=(5.*u(i1,i2,i3,ex)-
     & 10.*u(i1+js1,i2+js2,i3+js3,ex)+10.*u(i1+2*js1,i2+2*js2,i3+2*
     & js3,ex)-5.*u(i1+3*js1,i2+3*js2,i3+3*js3,ex)+u(i1+4*js1,i2+4*
     & js2,i3+4*js3,ex))
                        u(i1-js1,i2-js2,i3-js3,ey)=(5.*u(i1,i2,i3,ey)-
     & 10.*u(i1+js1,i2+js2,i3+js3,ey)+10.*u(i1+2*js1,i2+2*js2,i3+2*
     & js3,ey)-5.*u(i1+3*js1,i2+3*js2,i3+3*js3,ey)+u(i1+4*js1,i2+4*
     & js2,i3+4*js3,ey))
                        u(i1-js1,i2-js2,i3-js3,ez)=(5.*u(i1,i2,i3,ez)-
     & 10.*u(i1+js1,i2+js2,i3+js3,ez)+10.*u(i1+2*js1,i2+2*js2,i3+2*
     & js3,ez)-5.*u(i1+3*js1,i2+3*js2,i3+3*js3,ez)+u(i1+4*js1,i2+4*
     & js2,i3+4*js3,ez))
                     end do ! end do i1
                     end do ! end do i2
                     end do ! end do i3
                   else if( bc1.eq.dirichlet .or. bc2.eq.dirichlet )
     & then
                     ! *wdh* 081124 -- do nothing here ---
                     ! This is a dirichlet BC 
                 ! *     do i3=n3a,n3b
                 ! *     do i2=n2a,n2b
                 ! *     do i1=n1a,n1b
                 ! * 
                 ! *      #If "twilightZone" == "twilightZone"
                 ! *        OGF3DFO(i1-js1,i2-js2,i3-js3,t, g1,g2,g3)
                 ! *      #End
                 ! *      u(i1-js1,i2-js2,i3-js3,ex)=g1
                 ! *      u(i1-js1,i2-js2,i3-js3,ey)=g2
                 ! *      u(i1-js1,i2-js2,i3-js3,ez)=g3
                 ! * 
                 ! *     end do ! end do i1
                 ! *     end do ! end do i2
                 ! *     end do ! end do i3
                   else if( bc1.le.0 .or. bc2.le.0 )then
                     ! periodic or interpolation -- nothing to do
                   else if( bc1.eq.planeWaveBoundaryCondition 
     & .or.bc2.eq.planeWaveBoundaryCondition .or. 
     & bc1.eq.symmetryBoundaryCondition .or. 
     & bc2.eq.symmetryBoundaryCondition .or. (bc1.ge.abcEM2 .and. 
     & bc1.le.lastBC) .or. (bc2.ge.abcEM2 .and. bc2.le.lastBC) )then
                      ! do nothing
                   else
                     write(*,'("ERROR: unknown boundary conditions bc1,
     & bc2=",2i3)') bc1,bc2
                     ! unknown boundary conditions
                     stop 8866
                   end if
                  end do ! end do m1
                  end do ! end do m2
                  end do
                  end do
                  end do  ! edge direction
                ! Finally assign points outside the vertices of the unit cube
                g1=0.
                g2=0.
                g3=0.
                do side3=0,1
                do side2=0,1
                do side1=0,1
                 ! assign ghost values outside the corner (vertex)
                 i1=gridIndexRange(side1,0)
                 i2=gridIndexRange(side2,1)
                 i3=gridIndexRange(side3,2)
                 is1=1-2*side1
                 is2=1-2*side2
                 is3=1-2*side3
                 if( boundaryCondition(side1,0)
     & .eq.perfectElectricalConductor .and.boundaryCondition(side2,1)
     & .eq.perfectElectricalConductor .and.boundaryCondition(side3,2)
     & .eq.perfectElectricalConductor )then
                   ! ------------------------------------------------------------
                   ! -------------- VERTEX adjacent to 3 PEC faces --------------
                   ! ------------------------------------------------------------
                  do m3=1,numberOfGhostPoints
                  do m2=1,numberOfGhostPoints
                  do m1=1,numberOfGhostPoints
                    js1=is1*m1  ! shift to ghost point "m"
                    js2=is2*m2
                    js3=is3*m3
                    dra=dr(0)*js1
                    dsa=dr(1)*js2
                    dta=dr(2)*js3
                     ! *wdh* 2015/07/12 -- I think this is wrong: *fix me*
                     !   For  PEC corner:  E(-dx,-dy,-dz) = E(dx,dy,dz) 
                         call ogf3dfo(ep,fieldOption,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),xy(i1,i2,i3,2),t,u0,v0,w0)
                         call ogf3dfo(ep,fieldOption,xy(i1-js1,i2-js2,
     & i3-js3,0),xy(i1-js1,i2-js2,i3-js3,1),xy(i1-js1,i2-js2,i3-js3,2)
     & ,t,um,vm,wm)
                         call ogf3dfo(ep,fieldOption,xy(i1+js1,i2+js2,
     & i3+js3,0),xy(i1+js1,i2+js2,i3+js3,1),xy(i1+js1,i2+js2,i3+js3,2)
     & ,t,up,vp,wp)
                       g1=um-up
                       g2=vm-vp
                       g3=wm-wp
                     u(i1-js1,i2-js2,i3-js3,ex)=u(i1+js1,i2+js2,i3+js3,
     & ex)+g1
                     u(i1-js1,i2-js2,i3-js3,ey)=u(i1+js1,i2+js2,i3+js3,
     & ey)+g2
                     u(i1-js1,i2-js2,i3-js3,ez)=u(i1+js1,i2+js2,i3+js3,
     & ez)+g3
                     ! u(i1-js1,i2-js2,i3-js3,ex)=2.*u(i1,i2,i3,ex)-u(i1+js1,i2+js2,i3+js3,ex)+g1
                     ! u(i1-js1,i2-js2,i3-js3,ey)=2.*u(i1,i2,i3,ey)-u(i1+js1,i2+js2,i3+js3,ey)+g2
                     ! u(i1-js1,i2-js2,i3-js3,ez)=2.*u(i1,i2,i3,ez)-u(i1+js1,i2+js2,i3+js3,ez)+g3
                  end do
                  end do
                  end do
                 else if( .true. .and. (boundaryCondition(side1,0)
     & .eq.perfectElectricalConductor .or.boundaryCondition(side2,1)
     & .eq.perfectElectricalConductor .or.boundaryCondition(side3,2)
     & .eq.perfectElectricalConductor) )then
                   ! *new* *wdh* 2015/07/12 
                   ! -----------------------------------------------------------------
                   ! -------------- VERTEX adjacent to 1 or 2 PEC faces --------------
                   ! -----------------------------------------------------------------
                  do m3=1,numberOfGhostPoints
                  do m2=1,numberOfGhostPoints
                  do m1=1,numberOfGhostPoints
                    js1=is1*m1  ! shift to ghost point "m"
                    js2=is2*m2
                    js3=is3*m3
                     u(i1-js1,i2-js2,i3-js3,ex)=(5.*u(i1,i2,i3,ex)-10.*
     & u(i1+js1,i2+js2,i3+js3,ex)+10.*u(i1+2*js1,i2+2*js2,i3+2*js3,ex)
     & -5.*u(i1+3*js1,i2+3*js2,i3+3*js3,ex)+u(i1+4*js1,i2+4*js2,i3+4*
     & js3,ex))
                     u(i1-js1,i2-js2,i3-js3,ey)=(5.*u(i1,i2,i3,ey)-10.*
     & u(i1+js1,i2+js2,i3+js3,ey)+10.*u(i1+2*js1,i2+2*js2,i3+2*js3,ey)
     & -5.*u(i1+3*js1,i2+3*js2,i3+3*js3,ey)+u(i1+4*js1,i2+4*js2,i3+4*
     & js3,ey))
                     u(i1-js1,i2-js2,i3-js3,ez)=(5.*u(i1,i2,i3,ez)-10.*
     & u(i1+js1,i2+js2,i3+js3,ez)+10.*u(i1+2*js1,i2+2*js2,i3+2*js3,ez)
     & -5.*u(i1+3*js1,i2+3*js2,i3+3*js3,ez)+u(i1+4*js1,i2+4*js2,i3+4*
     & js3,ez))
                  end do ! end do m1
                  end do ! end do m2
                  end do ! end do m3
                 else if( boundaryCondition(side1,0).eq.dirichlet 
     & .or.boundaryCondition(side2,1).eq.dirichlet 
     & .or.boundaryCondition(side3,2).eq.dirichlet )then
                  ! *wdh* 081124 -- do nothing here ---
              ! *    do m3=1,numberOfGhostPoints
              ! *    do m2=1,numberOfGhostPoints
              ! *    do m1=1,numberOfGhostPoints
              ! *
              ! *      js1=is1*m1  ! shift to ghost point "m"
              ! *      js2=is2*m2
              ! *      js3=is3*m3
              ! *
              ! *      #If "twilightZone" == "twilightZone" 
              ! *        OGF3DFO(i1-js1,i2-js2,i3-js3,t, g1,g2,g3)
              ! *      #End
              ! *      u(i1-js1,i2-js2,i3-js3,ex)=g1
              ! *      u(i1-js1,i2-js2,i3-js3,ey)=g2
              ! *      u(i1-js1,i2-js2,i3-js3,ez)=g3
              ! *
              ! *    end do
              ! *    end do
              ! *    end do
                 else if( boundaryCondition(side1,0).le.0 
     & .or.boundaryCondition(side2,1).le.0 .or.boundaryCondition(
     & side3,2).le.0 )then
                    ! one or more boundaries are periodic or interpolation -- nothing to do
                 else if( boundaryCondition(side1,0)
     & .eq.planeWaveBoundaryCondition .or.boundaryCondition(side2,1)
     & .eq.planeWaveBoundaryCondition .or.boundaryCondition(side3,2)
     & .eq.planeWaveBoundaryCondition  .or. boundaryCondition(side1,0)
     & .eq.symmetryBoundaryCondition .or. boundaryCondition(side2,1)
     & .eq.symmetryBoundaryCondition .or. boundaryCondition(side3,2)
     & .eq.symmetryBoundaryCondition .or. (boundaryCondition(side1,0)
     & .ge.abcEM2 .and. boundaryCondition(side1,0).le.lastBC) .or. (
     & boundaryCondition(side2,1).ge.abcEM2 .and. boundaryCondition(
     & side2,1).le.lastBC) .or. (boundaryCondition(side3,2).ge.abcEM2 
     & .and. boundaryCondition(side3,2).le.lastBC)  )then
                   ! do nothing
                 else
                   write(*,'("ERROR: unknown boundary conditions at a 
     & 3D corner bc1,bc2,bc3=",2i3)') boundaryCondition(side1,0),
     & boundaryCondition(side2,1),boundaryCondition(side3,2)
                   ! '
                   ! unknown boundary conditions
                   stop 3399
                 end if
                end do
                end do
                end do
            end if
          else
            if( useForcing.eq.0 )then
                numberOfGhostPoints=orderOfAccuracy/2
                ! Assign the edges
                 do edgeDirection=0,2 ! direction parallel to the edge
                ! do edgeDirection=0,0 ! direction parallel to the edge
                 do sidea=0,1
                 do sideb=0,1
                   if( edgeDirection.eq.0 )then
                     side1=0
                     side2=sidea
                     side3=sideb
                   else if( edgeDirection.eq.1 )then
                     side1=sideb
                     side2=0
                     side3=sidea
                   else
                     side1=sidea
                     side2=sideb
                     side3=0
                   end if
                 is1=1-2*(side1)
                 is2=1-2*(side2)
                 is3=1-2*(side3)
                 if( edgeDirection.eq.2 )then
                  is3=0
                  n1a=gridIndexRange(side1,0)
                  n1b=gridIndexRange(side1,0)
                  n2a=gridIndexRange(side2,1)
                  n2b=gridIndexRange(side2,1)
                  n3a=gridIndexRange(0,2)
                  n3b=gridIndexRange(1,2)
                  bc1=boundaryCondition(side1,0)
                  bc2=boundaryCondition(side2,1)
                 else if( edgeDirection.eq.1 )then
                  is2=0
                  n1a=gridIndexRange(side1,0)
                  n1b=gridIndexRange(side1,0)
                  n2a=gridIndexRange(    0,1)
                  n2b=gridIndexRange(    1,1)
                  n3a=gridIndexRange(side3,2)
                  n3b=gridIndexRange(side3,2)
                  bc1=boundaryCondition(side1,0)
                  bc2=boundaryCondition(side3,2)
                 else
                  is1=0
                  n1a=gridIndexRange(    0,0)
                  n1b=gridIndexRange(    1,0)
                  n2a=gridIndexRange(side2,1)
                  n2b=gridIndexRange(side2,1)
                  n3a=gridIndexRange(side3,2)
                  n3b=gridIndexRange(side3,2)
                  bc1=boundaryCondition(side2,1)
                  bc2=boundaryCondition(side3,2)
                 end if
                 g1=0.
                 g2=0.
                 g3=0.
                 ! ********************************************************************
                 ! ***************Assign Extended boundary points**********************
                 ! ************** on an edge between two faces   ********************** 
                 ! ********************************************************************
                  ! ************** CURVILINEAR -- EXTENDED NEAR TWO FACES ************
                  is1=0
                  is2=0
                  is3=0
                  js1=0
                  js2=0
                  js3=0
                  ks1=0
                  ks2=0
                  ks3=0
                  if( edgeDirection.eq.0 )then
                    axis=1
                    axisp1=2
                    axisp2=0
                    is2=1-2*(side2)
                    js3=1-2*(side3)
                    ks1=1
                    dra=dr(axis  )*(1-2*(side2))
                    dsa=dr(axisp1)*(1-2*(side3))
                    dta=dr(axisp2)*(1          )
                  else if( edgeDirection.eq.1 )then
                    axis=2
                    axisp1=0
                    axisp2=1
                    is3=1-2*(side3)
                    js1=1-2*(side1)
                    ks2=1
                    dra=dr(axis  )*(1-2*(side3))
                    dsa=dr(axisp1)*(1-2*(side1))
                    dta=dr(axisp2)*(1          )
                  else
                    axis=0
                    axisp1=1
                    axisp2=2
                    is1=1-2*(side1)
                    js2=1-2*(side2)
                    ks3=1
                    dra=dr(axis  )*(1-2*(side1))
                    dsa=dr(axisp1)*(1-2*(side2))
                    dta=dr(axisp2)*(1          )
                  end if
                  if( debug.gt.2 )then
                    write(*,'(" cornersMxOrderORDER: **** Start: 
     & edgeDirection=",i1," ,side1,side2,side3 = ",3i2," axis,axisp1,
     & axisp2=",3i2,/,"      dra,dsa,dta=",3e10.2,"****")') 
     & edgeDirection,side1,side2,side3,axis,axisp1,axisp2,dra,dsa,dta
                    ! '
                  end if
                ! if( orderOfAccuracy.eq.4 )then
                  ! ************** CURVILINEAR -- EXTENDED NEAR TWO FACES ************
                  ! **************              4 4                   ************
                   if( bc1.eq.perfectElectricalConductor 
     & .and.bc2.eq.perfectElectricalConductor )then
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                     if( mask(i1,i2,i3).gt.0 )then ! *wdh* 2015/06/24
                       c11 = (rsxy(i1,i2,i3,axis,0)**2+rsxy(i1,i2,i3,
     & axis,1)**2+rsxy(i1,i2,i3,axis,2)**2)
                       c22 = (rsxy(i1,i2,i3,axisp1,0)**2+rsxy(i1,i2,i3,
     & axisp1,1)**2+rsxy(i1,i2,i3,axisp1,2)**2)
                       c33 = (rsxy(i1,i2,i3,axisp2,0)**2+rsxy(i1,i2,i3,
     & axisp2,1)**2+rsxy(i1,i2,i3,axisp2,2)**2)
                       c1 = (rsxyx43(i1,i2,i3,axis,0)+rsxyy43(i1,i2,i3,
     & axis,1)+rsxyz43(i1,i2,i3,axis,2))
                       c2 = (rsxyx43(i1,i2,i3,axisp1,0)+rsxyy43(i1,i2,
     & i3,axisp1,1)+rsxyz43(i1,i2,i3,axisp1,2))
                       c3 = (rsxyx43(i1,i2,i3,axisp2,0)+rsxyy43(i1,i2,
     & i3,axisp2,1)+rsxyz43(i1,i2,i3,axisp2,2))
                       ! urr=URR ,uss,utt,ur,us,ut (also for v and w)
                       urr=(-30.*u(i1,i2,i3,ex)+16.*(u(i1+is1,i2+is2,
     & i3+is3,ex)+u(i1-is1,i2-is2,i3-is3,ex))-(u(i1+2*is1,i2+2*is2,i3+
     & 2*is3,ex)+u(i1-2*is1,i2-2*is2,i3-2*is3,ex)))/(12.*dra**2)
                       uss=(-30.*u(i1,i2,i3,ex)+16.*(u(i1+js1,i2+js2,
     & i3+js3,ex)+u(i1-js1,i2-js2,i3-js3,ex))-(u(i1+2*js1,i2+2*js2,i3+
     & 2*js3,ex)+u(i1-2*js1,i2-2*js2,i3-2*js3,ex)))/(12.*dsa**2)
                       utt=(-30.*u(i1,i2,i3,ex)+16.*(u(i1+ks1,i2+ks2,
     & i3+ks3,ex)+u(i1-ks1,i2-ks2,i3-ks3,ex))-(u(i1+2*ks1,i2+2*ks2,i3+
     & 2*ks3,ex)+u(i1-2*ks1,i2-2*ks2,i3-2*ks3,ex)))/(12.*dta**2)
                       ur = (8.*(u(i1+is1,i2+is2,i3+is3,ex)-u(i1-is1,
     & i2-is2,i3-is3,ex))-(u(i1+2*is1,i2+2*is2,i3+2*is3,ex)-u(i1-2*
     & is1,i2-2*is2,i3-2*is3,ex)))/(12.*dra)
                       us = (8.*(u(i1+js1,i2+js2,i3+js3,ex)-u(i1-js1,
     & i2-js2,i3-js3,ex))-(u(i1+2*js1,i2+2*js2,i3+2*js3,ex)-u(i1-2*
     & js1,i2-2*js2,i3-2*js3,ex)))/(12.*dsa)
                       ut = (8.*(u(i1+ks1,i2+ks2,i3+ks3,ex)-u(i1-ks1,
     & i2-ks2,i3-ks3,ex))-(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ex)-u(i1-2*
     & ks1,i2-2*ks2,i3-2*ks3,ex)))/(12.*dta)
                       vrr=(-30.*u(i1,i2,i3,ey)+16.*(u(i1+is1,i2+is2,
     & i3+is3,ey)+u(i1-is1,i2-is2,i3-is3,ey))-(u(i1+2*is1,i2+2*is2,i3+
     & 2*is3,ey)+u(i1-2*is1,i2-2*is2,i3-2*is3,ey)))/(12.*dra**2)
                       vss=(-30.*u(i1,i2,i3,ey)+16.*(u(i1+js1,i2+js2,
     & i3+js3,ey)+u(i1-js1,i2-js2,i3-js3,ey))-(u(i1+2*js1,i2+2*js2,i3+
     & 2*js3,ey)+u(i1-2*js1,i2-2*js2,i3-2*js3,ey)))/(12.*dsa**2)
                       vtt=(-30.*u(i1,i2,i3,ey)+16.*(u(i1+ks1,i2+ks2,
     & i3+ks3,ey)+u(i1-ks1,i2-ks2,i3-ks3,ey))-(u(i1+2*ks1,i2+2*ks2,i3+
     & 2*ks3,ey)+u(i1-2*ks1,i2-2*ks2,i3-2*ks3,ey)))/(12.*dta**2)
                       vr = (8.*(u(i1+is1,i2+is2,i3+is3,ey)-u(i1-is1,
     & i2-is2,i3-is3,ey))-(u(i1+2*is1,i2+2*is2,i3+2*is3,ey)-u(i1-2*
     & is1,i2-2*is2,i3-2*is3,ey)))/(12.*dra)
                       vs = (8.*(u(i1+js1,i2+js2,i3+js3,ey)-u(i1-js1,
     & i2-js2,i3-js3,ey))-(u(i1+2*js1,i2+2*js2,i3+2*js3,ey)-u(i1-2*
     & js1,i2-2*js2,i3-2*js3,ey)))/(12.*dsa)
                       vt = (8.*(u(i1+ks1,i2+ks2,i3+ks3,ey)-u(i1-ks1,
     & i2-ks2,i3-ks3,ey))-(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ey)-u(i1-2*
     & ks1,i2-2*ks2,i3-2*ks3,ey)))/(12.*dta)
                       wrr=(-30.*u(i1,i2,i3,ez)+16.*(u(i1+is1,i2+is2,
     & i3+is3,ez)+u(i1-is1,i2-is2,i3-is3,ez))-(u(i1+2*is1,i2+2*is2,i3+
     & 2*is3,ez)+u(i1-2*is1,i2-2*is2,i3-2*is3,ez)))/(12.*dra**2)
                       wss=(-30.*u(i1,i2,i3,ez)+16.*(u(i1+js1,i2+js2,
     & i3+js3,ez)+u(i1-js1,i2-js2,i3-js3,ez))-(u(i1+2*js1,i2+2*js2,i3+
     & 2*js3,ez)+u(i1-2*js1,i2-2*js2,i3-2*js3,ez)))/(12.*dsa**2)
                       wtt=(-30.*u(i1,i2,i3,ez)+16.*(u(i1+ks1,i2+ks2,
     & i3+ks3,ez)+u(i1-ks1,i2-ks2,i3-ks3,ez))-(u(i1+2*ks1,i2+2*ks2,i3+
     & 2*ks3,ez)+u(i1-2*ks1,i2-2*ks2,i3-2*ks3,ez)))/(12.*dta**2)
                       wr = (8.*(u(i1+is1,i2+is2,i3+is3,ez)-u(i1-is1,
     & i2-is2,i3-is3,ez))-(u(i1+2*is1,i2+2*is2,i3+2*is3,ez)-u(i1-2*
     & is1,i2-2*is2,i3-2*is3,ez)))/(12.*dra)
                       ws = (8.*(u(i1+js1,i2+js2,i3+js3,ez)-u(i1-js1,
     & i2-js2,i3-js3,ez))-(u(i1+2*js1,i2+2*js2,i3+2*js3,ez)-u(i1-2*
     & js1,i2-2*js2,i3-2*js3,ez)))/(12.*dsa)
                       wt = (8.*(u(i1+ks1,i2+ks2,i3+ks3,ez)-u(i1-ks1,
     & i2-ks2,i3-ks3,ez))-(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ez)-u(i1-2*
     & ks1,i2-2*ks2,i3-2*ks3,ez)))/(12.*dta)
                       !    deltaFu,deltaFv,deltaFw = RHS for Delta(u,v,w)
                       deltaFu=0.
                       deltaFv=0.
                       deltaFw=0.
                       !    g1f,g2f = RHS for extrapolation, a1.D+2^4u(i1,i2-2)=g1f, a2.D+2^4u(i1-2,i2)=g2f,    
                       g1f=0.
                       g2f=0.
                !        if( debug.gt.1 )then
                !         write(*,'(" bce4: before: u(-1,0),(-2,0)=",6f7.2)') !           u(i1-  is1,i2-  is2,i3-  is3,ex),!           u(i1-  is1,i2-  is2,i3-  is3,ey),!           u(i1-  is1,i2-  is2,i3-  is3,ez),!           u(i1-2*is1,i2-2*is2,i3-2*is3,ex),!           u(i1-2*is1,i2-2*is2,i3-2*is3,ey),!           u(i1-2*is1,i2-2*is2,i3-2*is3,ez)
                !         write(*,'(" bce4: before u(0,2),u(0,1),u(0,0),u(0,-1),(0,-2)=",/,(4x,3f7.2))') !           u(i1+2*js1,i2+2*js2,i3+2*js3,ex),!           u(i1+2*js1,i2+2*js2,i3+2*js3,ey),!           u(i1+2*js1,i2+2*js2,i3+2*js3,ez),!           u(i1+  js1,i2+  js2,i3+  js3,ex),!           u(i1+  js1,i2+  js2,i3+  js3,ey),!           u(i1+  js1,i2+  js2,i3+  js3,ez),!           u(i1,i2,i3,ex),!           u(i1,i2,i3,ey),!           u(i1,i2,i3,ez),!           u(i1-  js1,i2-  js2,i3-  js3,ex),!           u(i1-  js1,i2-  js2,i3-  js3,ey),!           u(i1-  js1,i2-  js2,i3-  js3,ez),!           u(i1-2*js1,i2-2*js2,i3-2*js3,ex),!           u(i1-2*js1,i2-2*js2,i3-2*js3,ey),!           u(i1-2*js1,i2-2*js2,i3-2*js3,ez)
                !          write(*,'(" bce4: urr,uss,utt=",9f7.2)') urr,vrr,wrr,uss,vss,wss,utt,vtt,wtt
                !        end if
                ! this next file is generated by bce.maple
 ! results from bcExtended3d4.maple
 ! Assign values on the extended boundary next to two PEC boundaries
 !                                                                  
 ! Here we assume the following are defined                               
 !    c11,c22,c33,c1,c2,c3                                          
 !    urr,uss,utt,ur,us,ut (also for v and w)                       
 !    deltaFu,deltaFv,deltaFw = RHS for Delta(u,v,w)                
 !    g1f,g2f = RHS for extrapolation, a1.D+2^4u(i1,i2-2)=g1f, a2.D+2^4u(i1-2,i2)=g2f,    
 !                                                                  
      DeltaU = c11*urr+c22*uss+c33*utt+c1*ur+c2*us+c3*ut - deltaFu
      DeltaV = c11*vrr+c22*vss+c33*vtt+c1*vr+c2*vs+c3*vt - deltaFv
      DeltaW = c11*wrr+c22*wss+c33*wtt+c1*wr+c2*ws+c3*wt - deltaFw

! ** decompose point u(i1-is1,i2-is2,i3-is3) into components along a1,a2,a3 **
      a11c=(rsxy(i1-is1,i2-is2,i3-is3,axis,0)/(rx(i1-is1,i2-is2,i3-is3)
     & *(sy(i1-is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3)-sz(i1-is1,
     & i2-is2,i3-is3)*ty(i1-is1,i2-is2,i3-is3))+ry(i1-is1,i2-is2,i3-
     & is3)*(sz(i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-is3)-sx(i1-
     & is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3))+rz(i1-is1,i2-is2,
     & i3-is3)*(sx(i1-is1,i2-is2,i3-is3)*ty(i1-is1,i2-is2,i3-is3)-sy(
     & i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-is3))))
      a12c=(rsxy(i1-is1,i2-is2,i3-is3,axis,1)/(rx(i1-is1,i2-is2,i3-is3)
     & *(sy(i1-is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3)-sz(i1-is1,
     & i2-is2,i3-is3)*ty(i1-is1,i2-is2,i3-is3))+ry(i1-is1,i2-is2,i3-
     & is3)*(sz(i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-is3)-sx(i1-
     & is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3))+rz(i1-is1,i2-is2,
     & i3-is3)*(sx(i1-is1,i2-is2,i3-is3)*ty(i1-is1,i2-is2,i3-is3)-sy(
     & i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-is3))))
      a13c=(rsxy(i1-is1,i2-is2,i3-is3,axis,2)/(rx(i1-is1,i2-is2,i3-is3)
     & *(sy(i1-is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3)-sz(i1-is1,
     & i2-is2,i3-is3)*ty(i1-is1,i2-is2,i3-is3))+ry(i1-is1,i2-is2,i3-
     & is3)*(sz(i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-is3)-sx(i1-
     & is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3))+rz(i1-is1,i2-is2,
     & i3-is3)*(sx(i1-is1,i2-is2,i3-is3)*ty(i1-is1,i2-is2,i3-is3)-sy(
     & i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-is3))))
      a21c=(rsxy(i1-is1,i2-is2,i3-is3,axisp1,0)/(rx(i1-is1,i2-is2,i3-
     & is3)*(sy(i1-is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3)-sz(i1-
     & is1,i2-is2,i3-is3)*ty(i1-is1,i2-is2,i3-is3))+ry(i1-is1,i2-is2,
     & i3-is3)*(sz(i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-is3)-sx(
     & i1-is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3))+rz(i1-is1,i2-
     & is2,i3-is3)*(sx(i1-is1,i2-is2,i3-is3)*ty(i1-is1,i2-is2,i3-is3)-
     & sy(i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-is3))))
      a22c=(rsxy(i1-is1,i2-is2,i3-is3,axisp1,1)/(rx(i1-is1,i2-is2,i3-
     & is3)*(sy(i1-is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3)-sz(i1-
     & is1,i2-is2,i3-is3)*ty(i1-is1,i2-is2,i3-is3))+ry(i1-is1,i2-is2,
     & i3-is3)*(sz(i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-is3)-sx(
     & i1-is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3))+rz(i1-is1,i2-
     & is2,i3-is3)*(sx(i1-is1,i2-is2,i3-is3)*ty(i1-is1,i2-is2,i3-is3)-
     & sy(i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-is3))))
      a23c=(rsxy(i1-is1,i2-is2,i3-is3,axisp1,2)/(rx(i1-is1,i2-is2,i3-
     & is3)*(sy(i1-is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3)-sz(i1-
     & is1,i2-is2,i3-is3)*ty(i1-is1,i2-is2,i3-is3))+ry(i1-is1,i2-is2,
     & i3-is3)*(sz(i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-is3)-sx(
     & i1-is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3))+rz(i1-is1,i2-
     & is2,i3-is3)*(sx(i1-is1,i2-is2,i3-is3)*ty(i1-is1,i2-is2,i3-is3)-
     & sy(i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-is3))))
      a31c=(rsxy(i1-is1,i2-is2,i3-is3,axisp2,0)/(rx(i1-is1,i2-is2,i3-
     & is3)*(sy(i1-is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3)-sz(i1-
     & is1,i2-is2,i3-is3)*ty(i1-is1,i2-is2,i3-is3))+ry(i1-is1,i2-is2,
     & i3-is3)*(sz(i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-is3)-sx(
     & i1-is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3))+rz(i1-is1,i2-
     & is2,i3-is3)*(sx(i1-is1,i2-is2,i3-is3)*ty(i1-is1,i2-is2,i3-is3)-
     & sy(i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-is3))))
      a32c=(rsxy(i1-is1,i2-is2,i3-is3,axisp2,1)/(rx(i1-is1,i2-is2,i3-
     & is3)*(sy(i1-is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3)-sz(i1-
     & is1,i2-is2,i3-is3)*ty(i1-is1,i2-is2,i3-is3))+ry(i1-is1,i2-is2,
     & i3-is3)*(sz(i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-is3)-sx(
     & i1-is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3))+rz(i1-is1,i2-
     & is2,i3-is3)*(sx(i1-is1,i2-is2,i3-is3)*ty(i1-is1,i2-is2,i3-is3)-
     & sy(i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-is3))))
      a33c=(rsxy(i1-is1,i2-is2,i3-is3,axisp2,2)/(rx(i1-is1,i2-is2,i3-
     & is3)*(sy(i1-is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3)-sz(i1-
     & is1,i2-is2,i3-is3)*ty(i1-is1,i2-is2,i3-is3))+ry(i1-is1,i2-is2,
     & i3-is3)*(sz(i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-is3)-sx(
     & i1-is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3))+rz(i1-is1,i2-
     & is2,i3-is3)*(sx(i1-is1,i2-is2,i3-is3)*ty(i1-is1,i2-is2,i3-is3)-
     & sy(i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-is3))))

      a1a1=a11c*a11c+a12c*a12c+a13c*a13c
      a1a2=a11c*a21c+a12c*a22c+a13c*a23c
      a1a3=a11c*a31c+a12c*a32c+a13c*a33c
      a2a3=a21c*a31c+a22c*a32c+a23c*a33c
      a3a3=a31c*a31c+a32c*a32c+a33c*a33c
! The tangential component is assumed valid:
      a1Dotu1=a11c*u(i1-is1,i2-is2,i3-is3,ex)+a12c*u(i1-is1,i2-is2,i3-
     & is3,ey)+a13c*u(i1-is1,i2-is2,i3-is3,ez)
      a3Dotu1=a31c*u(i1-is1,i2-is2,i3-is3,ex)+a32c*u(i1-is1,i2-is2,i3-
     & is3,ey)+a33c*u(i1-is1,i2-is2,i3-is3,ez)
 ! u(i1-is1,i2-is2,i3-is3,k) = b1[k]*x1 +g1[k]
      b11 =-a11c*(-a1a2*a3a3+a1a3*a2a3)/(-a1a1*a3a3+a1a3**2)+a21c-a31c*
     & (-a1a1*a2a3+a1a3*a1a2)/(-a1a1*a3a3+a1a3**2)
      b12 =-a12c*(-a1a2*a3a3+a1a3*a2a3)/(-a1a1*a3a3+a1a3**2)+a22c-a32c*
     & (-a1a1*a2a3+a1a3*a1a2)/(-a1a1*a3a3+a1a3**2)
      b13 =-a13c*(-a1a2*a3a3+a1a3*a2a3)/(-a1a1*a3a3+a1a3**2)+a23c-a33c*
     & (-a1a1*a2a3+a1a3*a1a2)/(-a1a1*a3a3+a1a3**2)
      g11 =-(-a11c*a1a3*a3Dotu1+a11c*a1Dotu1*a3a3-a31c*a1a3*a1Dotu1+
     & a31c*a1a1*a3Dotu1)/(-a1a1*a3a3+a1a3**2)
      g12 =-(-a12c*a1a3*a3Dotu1+a12c*a1Dotu1*a3a3-a32c*a1a3*a1Dotu1+
     & a32c*a1a1*a3Dotu1)/(-a1a1*a3a3+a1a3**2)
      g13 =(a13c*a1a3*a3Dotu1-a13c*a1Dotu1*a3a3+a33c*a1a3*a1Dotu1-a33c*
     & a1a1*a3Dotu1)/(-a1a1*a3a3+a1a3**2)

! ** decompose point u(i1-2*is1,i2-2*is2,i3-2*is3) into components along a1,a2,a3 **
      a11c=(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,0)/(rx(i1-2*is1,i2-2*
     & is2,i3-2*is3)*(sy(i1-2*is1,i2-2*is2,i3-2*is3)*tz(i1-2*is1,i2-2*
     & is2,i3-2*is3)-sz(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-2*is1,i2-2*
     & is2,i3-2*is3))+ry(i1-2*is1,i2-2*is2,i3-2*is3)*(sz(i1-2*is1,i2-
     & 2*is2,i3-2*is3)*tx(i1-2*is1,i2-2*is2,i3-2*is3)-sx(i1-2*is1,i2-
     & 2*is2,i3-2*is3)*tz(i1-2*is1,i2-2*is2,i3-2*is3))+rz(i1-2*is1,i2-
     & 2*is2,i3-2*is3)*(sx(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-2*is1,i2-
     & 2*is2,i3-2*is3)-sy(i1-2*is1,i2-2*is2,i3-2*is3)*tx(i1-2*is1,i2-
     & 2*is2,i3-2*is3))))
      a12c=(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,1)/(rx(i1-2*is1,i2-2*
     & is2,i3-2*is3)*(sy(i1-2*is1,i2-2*is2,i3-2*is3)*tz(i1-2*is1,i2-2*
     & is2,i3-2*is3)-sz(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-2*is1,i2-2*
     & is2,i3-2*is3))+ry(i1-2*is1,i2-2*is2,i3-2*is3)*(sz(i1-2*is1,i2-
     & 2*is2,i3-2*is3)*tx(i1-2*is1,i2-2*is2,i3-2*is3)-sx(i1-2*is1,i2-
     & 2*is2,i3-2*is3)*tz(i1-2*is1,i2-2*is2,i3-2*is3))+rz(i1-2*is1,i2-
     & 2*is2,i3-2*is3)*(sx(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-2*is1,i2-
     & 2*is2,i3-2*is3)-sy(i1-2*is1,i2-2*is2,i3-2*is3)*tx(i1-2*is1,i2-
     & 2*is2,i3-2*is3))))
      a13c=(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,2)/(rx(i1-2*is1,i2-2*
     & is2,i3-2*is3)*(sy(i1-2*is1,i2-2*is2,i3-2*is3)*tz(i1-2*is1,i2-2*
     & is2,i3-2*is3)-sz(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-2*is1,i2-2*
     & is2,i3-2*is3))+ry(i1-2*is1,i2-2*is2,i3-2*is3)*(sz(i1-2*is1,i2-
     & 2*is2,i3-2*is3)*tx(i1-2*is1,i2-2*is2,i3-2*is3)-sx(i1-2*is1,i2-
     & 2*is2,i3-2*is3)*tz(i1-2*is1,i2-2*is2,i3-2*is3))+rz(i1-2*is1,i2-
     & 2*is2,i3-2*is3)*(sx(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-2*is1,i2-
     & 2*is2,i3-2*is3)-sy(i1-2*is1,i2-2*is2,i3-2*is3)*tx(i1-2*is1,i2-
     & 2*is2,i3-2*is3))))
      a21c=(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axisp1,0)/(rx(i1-2*is1,i2-
     & 2*is2,i3-2*is3)*(sy(i1-2*is1,i2-2*is2,i3-2*is3)*tz(i1-2*is1,i2-
     & 2*is2,i3-2*is3)-sz(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-2*is1,i2-
     & 2*is2,i3-2*is3))+ry(i1-2*is1,i2-2*is2,i3-2*is3)*(sz(i1-2*is1,
     & i2-2*is2,i3-2*is3)*tx(i1-2*is1,i2-2*is2,i3-2*is3)-sx(i1-2*is1,
     & i2-2*is2,i3-2*is3)*tz(i1-2*is1,i2-2*is2,i3-2*is3))+rz(i1-2*is1,
     & i2-2*is2,i3-2*is3)*(sx(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-2*is1,
     & i2-2*is2,i3-2*is3)-sy(i1-2*is1,i2-2*is2,i3-2*is3)*tx(i1-2*is1,
     & i2-2*is2,i3-2*is3))))
      a22c=(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axisp1,1)/(rx(i1-2*is1,i2-
     & 2*is2,i3-2*is3)*(sy(i1-2*is1,i2-2*is2,i3-2*is3)*tz(i1-2*is1,i2-
     & 2*is2,i3-2*is3)-sz(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-2*is1,i2-
     & 2*is2,i3-2*is3))+ry(i1-2*is1,i2-2*is2,i3-2*is3)*(sz(i1-2*is1,
     & i2-2*is2,i3-2*is3)*tx(i1-2*is1,i2-2*is2,i3-2*is3)-sx(i1-2*is1,
     & i2-2*is2,i3-2*is3)*tz(i1-2*is1,i2-2*is2,i3-2*is3))+rz(i1-2*is1,
     & i2-2*is2,i3-2*is3)*(sx(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-2*is1,
     & i2-2*is2,i3-2*is3)-sy(i1-2*is1,i2-2*is2,i3-2*is3)*tx(i1-2*is1,
     & i2-2*is2,i3-2*is3))))
      a23c=(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axisp1,2)/(rx(i1-2*is1,i2-
     & 2*is2,i3-2*is3)*(sy(i1-2*is1,i2-2*is2,i3-2*is3)*tz(i1-2*is1,i2-
     & 2*is2,i3-2*is3)-sz(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-2*is1,i2-
     & 2*is2,i3-2*is3))+ry(i1-2*is1,i2-2*is2,i3-2*is3)*(sz(i1-2*is1,
     & i2-2*is2,i3-2*is3)*tx(i1-2*is1,i2-2*is2,i3-2*is3)-sx(i1-2*is1,
     & i2-2*is2,i3-2*is3)*tz(i1-2*is1,i2-2*is2,i3-2*is3))+rz(i1-2*is1,
     & i2-2*is2,i3-2*is3)*(sx(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-2*is1,
     & i2-2*is2,i3-2*is3)-sy(i1-2*is1,i2-2*is2,i3-2*is3)*tx(i1-2*is1,
     & i2-2*is2,i3-2*is3))))
      a31c=(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axisp2,0)/(rx(i1-2*is1,i2-
     & 2*is2,i3-2*is3)*(sy(i1-2*is1,i2-2*is2,i3-2*is3)*tz(i1-2*is1,i2-
     & 2*is2,i3-2*is3)-sz(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-2*is1,i2-
     & 2*is2,i3-2*is3))+ry(i1-2*is1,i2-2*is2,i3-2*is3)*(sz(i1-2*is1,
     & i2-2*is2,i3-2*is3)*tx(i1-2*is1,i2-2*is2,i3-2*is3)-sx(i1-2*is1,
     & i2-2*is2,i3-2*is3)*tz(i1-2*is1,i2-2*is2,i3-2*is3))+rz(i1-2*is1,
     & i2-2*is2,i3-2*is3)*(sx(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-2*is1,
     & i2-2*is2,i3-2*is3)-sy(i1-2*is1,i2-2*is2,i3-2*is3)*tx(i1-2*is1,
     & i2-2*is2,i3-2*is3))))
      a32c=(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axisp2,1)/(rx(i1-2*is1,i2-
     & 2*is2,i3-2*is3)*(sy(i1-2*is1,i2-2*is2,i3-2*is3)*tz(i1-2*is1,i2-
     & 2*is2,i3-2*is3)-sz(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-2*is1,i2-
     & 2*is2,i3-2*is3))+ry(i1-2*is1,i2-2*is2,i3-2*is3)*(sz(i1-2*is1,
     & i2-2*is2,i3-2*is3)*tx(i1-2*is1,i2-2*is2,i3-2*is3)-sx(i1-2*is1,
     & i2-2*is2,i3-2*is3)*tz(i1-2*is1,i2-2*is2,i3-2*is3))+rz(i1-2*is1,
     & i2-2*is2,i3-2*is3)*(sx(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-2*is1,
     & i2-2*is2,i3-2*is3)-sy(i1-2*is1,i2-2*is2,i3-2*is3)*tx(i1-2*is1,
     & i2-2*is2,i3-2*is3))))
      a33c=(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axisp2,2)/(rx(i1-2*is1,i2-
     & 2*is2,i3-2*is3)*(sy(i1-2*is1,i2-2*is2,i3-2*is3)*tz(i1-2*is1,i2-
     & 2*is2,i3-2*is3)-sz(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-2*is1,i2-
     & 2*is2,i3-2*is3))+ry(i1-2*is1,i2-2*is2,i3-2*is3)*(sz(i1-2*is1,
     & i2-2*is2,i3-2*is3)*tx(i1-2*is1,i2-2*is2,i3-2*is3)-sx(i1-2*is1,
     & i2-2*is2,i3-2*is3)*tz(i1-2*is1,i2-2*is2,i3-2*is3))+rz(i1-2*is1,
     & i2-2*is2,i3-2*is3)*(sx(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-2*is1,
     & i2-2*is2,i3-2*is3)-sy(i1-2*is1,i2-2*is2,i3-2*is3)*tx(i1-2*is1,
     & i2-2*is2,i3-2*is3))))

      a1a1=a11c*a11c+a12c*a12c+a13c*a13c
      a1a2=a11c*a21c+a12c*a22c+a13c*a23c
      a1a3=a11c*a31c+a12c*a32c+a13c*a33c
      a2a3=a21c*a31c+a22c*a32c+a23c*a33c
      a3a3=a31c*a31c+a32c*a32c+a33c*a33c
! The tangential component is assumed valid:
      a1Dotu2=a11c*u(i1-2*is1,i2-2*is2,i3-2*is3,ex)+a12c*u(i1-2*is1,i2-
     & 2*is2,i3-2*is3,ey)+a13c*u(i1-2*is1,i2-2*is2,i3-2*is3,ez)
      a3Dotu2=a31c*u(i1-2*is1,i2-2*is2,i3-2*is3,ex)+a32c*u(i1-2*is1,i2-
     & 2*is2,i3-2*is3,ey)+a33c*u(i1-2*is1,i2-2*is2,i3-2*is3,ez)
 ! u(i1-2*is1,i2-2*is2,i3-2*is3,k) = b2[k]*x2 +g2[k]
      b21 =-a11c*(-a1a2*a3a3+a1a3*a2a3)/(-a1a1*a3a3+a1a3**2)+a21c-a31c*
     & (-a1a1*a2a3+a1a3*a1a2)/(-a1a1*a3a3+a1a3**2)
      b22 =-a12c*(-a1a2*a3a3+a1a3*a2a3)/(-a1a1*a3a3+a1a3**2)+a22c-a32c*
     & (-a1a1*a2a3+a1a3*a1a2)/(-a1a1*a3a3+a1a3**2)
      b23 =-a13c*(-a1a2*a3a3+a1a3*a2a3)/(-a1a1*a3a3+a1a3**2)+a23c-a33c*
     & (-a1a1*a2a3+a1a3*a1a2)/(-a1a1*a3a3+a1a3**2)
      g21 =(a11c*a1a3*a3Dotu2-a11c*a1Dotu2*a3a3+a31c*a1a3*a1Dotu2-a31c*
     & a1a1*a3Dotu2)/(-a1a1*a3a3+a1a3**2)
      g22 =(a12c*a1a3*a3Dotu2-a12c*a1Dotu2*a3a3+a32c*a1a3*a1Dotu2-a32c*
     & a1a1*a3Dotu2)/(-a1a1*a3a3+a1a3**2)
      g23 =(a13c*a1a3*a3Dotu2-a13c*a1Dotu2*a3a3+a33c*a1a3*a1Dotu2-a33c*
     & a1a1*a3Dotu2)/(-a1a1*a3a3+a1a3**2)

! ** decompose point u(i1-js1,i2-js2,i3-js3) into components along a1,a2,a3 **
      a11c=(rsxy(i1-js1,i2-js2,i3-js3,axis,0)/(rx(i1-js1,i2-js2,i3-js3)
     & *(sy(i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3)-sz(i1-js1,
     & i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3))+ry(i1-js1,i2-js2,i3-
     & js3)*(sz(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3)-sx(i1-
     & js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3))+rz(i1-js1,i2-js2,
     & i3-js3)*(sx(i1-js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3)-sy(
     & i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3))))
      a12c=(rsxy(i1-js1,i2-js2,i3-js3,axis,1)/(rx(i1-js1,i2-js2,i3-js3)
     & *(sy(i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3)-sz(i1-js1,
     & i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3))+ry(i1-js1,i2-js2,i3-
     & js3)*(sz(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3)-sx(i1-
     & js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3))+rz(i1-js1,i2-js2,
     & i3-js3)*(sx(i1-js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3)-sy(
     & i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3))))
      a13c=(rsxy(i1-js1,i2-js2,i3-js3,axis,2)/(rx(i1-js1,i2-js2,i3-js3)
     & *(sy(i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3)-sz(i1-js1,
     & i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3))+ry(i1-js1,i2-js2,i3-
     & js3)*(sz(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3)-sx(i1-
     & js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3))+rz(i1-js1,i2-js2,
     & i3-js3)*(sx(i1-js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3)-sy(
     & i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3))))
      a21c=(rsxy(i1-js1,i2-js2,i3-js3,axisp1,0)/(rx(i1-js1,i2-js2,i3-
     & js3)*(sy(i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3)-sz(i1-
     & js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3))+ry(i1-js1,i2-js2,
     & i3-js3)*(sz(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3)-sx(
     & i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3))+rz(i1-js1,i2-
     & js2,i3-js3)*(sx(i1-js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3)-
     & sy(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3))))
      a22c=(rsxy(i1-js1,i2-js2,i3-js3,axisp1,1)/(rx(i1-js1,i2-js2,i3-
     & js3)*(sy(i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3)-sz(i1-
     & js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3))+ry(i1-js1,i2-js2,
     & i3-js3)*(sz(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3)-sx(
     & i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3))+rz(i1-js1,i2-
     & js2,i3-js3)*(sx(i1-js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3)-
     & sy(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3))))
      a23c=(rsxy(i1-js1,i2-js2,i3-js3,axisp1,2)/(rx(i1-js1,i2-js2,i3-
     & js3)*(sy(i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3)-sz(i1-
     & js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3))+ry(i1-js1,i2-js2,
     & i3-js3)*(sz(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3)-sx(
     & i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3))+rz(i1-js1,i2-
     & js2,i3-js3)*(sx(i1-js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3)-
     & sy(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3))))
      a31c=(rsxy(i1-js1,i2-js2,i3-js3,axisp2,0)/(rx(i1-js1,i2-js2,i3-
     & js3)*(sy(i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3)-sz(i1-
     & js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3))+ry(i1-js1,i2-js2,
     & i3-js3)*(sz(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3)-sx(
     & i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3))+rz(i1-js1,i2-
     & js2,i3-js3)*(sx(i1-js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3)-
     & sy(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3))))
      a32c=(rsxy(i1-js1,i2-js2,i3-js3,axisp2,1)/(rx(i1-js1,i2-js2,i3-
     & js3)*(sy(i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3)-sz(i1-
     & js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3))+ry(i1-js1,i2-js2,
     & i3-js3)*(sz(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3)-sx(
     & i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3))+rz(i1-js1,i2-
     & js2,i3-js3)*(sx(i1-js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3)-
     & sy(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3))))
      a33c=(rsxy(i1-js1,i2-js2,i3-js3,axisp2,2)/(rx(i1-js1,i2-js2,i3-
     & js3)*(sy(i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3)-sz(i1-
     & js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3))+ry(i1-js1,i2-js2,
     & i3-js3)*(sz(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3)-sx(
     & i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3))+rz(i1-js1,i2-
     & js2,i3-js3)*(sx(i1-js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3)-
     & sy(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3))))

      a2a2=a21c*a21c+a22c*a22c+a23c*a23c
      a1a2=a11c*a21c+a12c*a22c+a13c*a23c
      a1a3=a11c*a31c+a12c*a32c+a13c*a33c
      a2a3=a21c*a31c+a22c*a32c+a23c*a33c
      a3a3=a31c*a31c+a32c*a32c+a33c*a33c
! The tangential component is assumed valid:
      a2Dotu3=a21c*u(i1-js1,i2-js2,i3-js3,ex)+a22c*u(i1-js1,i2-js2,i3-
     & js3,ey)+a23c*u(i1-js1,i2-js2,i3-js3,ez)
      a3Dotu3=a31c*u(i1-js1,i2-js2,i3-js3,ex)+a32c*u(i1-js1,i2-js2,i3-
     & js3,ey)+a33c*u(i1-js1,i2-js2,i3-js3,ez)
 ! u(i1-js1,i2-js2,i3-js3,k) = b3[k]*x3 +g3[k]
      b31 =a11c-a21c/(a2a3**2-a3a3*a2a2)*(-a1a2*a3a3+a1a3*a2a3)+a31c*(-
     & a1a2*a2a3+a2a2*a1a3)/(a2a3**2-a3a3*a2a2)
      b32 =a12c-a22c/(a2a3**2-a3a3*a2a2)*(-a1a2*a3a3+a1a3*a2a3)+a32c*(-
     & a1a2*a2a3+a2a2*a1a3)/(a2a3**2-a3a3*a2a2)
      b33 =a13c-a23c/(a2a3**2-a3a3*a2a2)*(-a1a2*a3a3+a1a3*a2a3)+a33c*(-
     & a1a2*a2a3+a2a2*a1a3)/(a2a3**2-a3a3*a2a2)
      g31 =(-a21c*a3a3*a2Dotu3+a21c*a2a3*a3Dotu3-a31c*a2a2*a3Dotu3+
     & a31c*a2Dotu3*a2a3)/(a2a3**2-a3a3*a2a2)
      g32 =(-a22c*a3a3*a2Dotu3+a22c*a2a3*a3Dotu3-a32c*a2a2*a3Dotu3+
     & a32c*a2Dotu3*a2a3)/(a2a3**2-a3a3*a2a2)
      g33 =(-a23c*a3a3*a2Dotu3+a23c*a2a3*a3Dotu3-a33c*a2a2*a3Dotu3+
     & a33c*a2Dotu3*a2a3)/(a2a3**2-a3a3*a2a2)

! ** decompose point u(i1-2*js1,i2-2*js2,i3-2*js3) into components along a1,a2,a3 **
      a11c=(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axis,0)/(rx(i1-2*js1,i2-2*
     & js2,i3-2*js3)*(sy(i1-2*js1,i2-2*js2,i3-2*js3)*tz(i1-2*js1,i2-2*
     & js2,i3-2*js3)-sz(i1-2*js1,i2-2*js2,i3-2*js3)*ty(i1-2*js1,i2-2*
     & js2,i3-2*js3))+ry(i1-2*js1,i2-2*js2,i3-2*js3)*(sz(i1-2*js1,i2-
     & 2*js2,i3-2*js3)*tx(i1-2*js1,i2-2*js2,i3-2*js3)-sx(i1-2*js1,i2-
     & 2*js2,i3-2*js3)*tz(i1-2*js1,i2-2*js2,i3-2*js3))+rz(i1-2*js1,i2-
     & 2*js2,i3-2*js3)*(sx(i1-2*js1,i2-2*js2,i3-2*js3)*ty(i1-2*js1,i2-
     & 2*js2,i3-2*js3)-sy(i1-2*js1,i2-2*js2,i3-2*js3)*tx(i1-2*js1,i2-
     & 2*js2,i3-2*js3))))
      a12c=(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axis,1)/(rx(i1-2*js1,i2-2*
     & js2,i3-2*js3)*(sy(i1-2*js1,i2-2*js2,i3-2*js3)*tz(i1-2*js1,i2-2*
     & js2,i3-2*js3)-sz(i1-2*js1,i2-2*js2,i3-2*js3)*ty(i1-2*js1,i2-2*
     & js2,i3-2*js3))+ry(i1-2*js1,i2-2*js2,i3-2*js3)*(sz(i1-2*js1,i2-
     & 2*js2,i3-2*js3)*tx(i1-2*js1,i2-2*js2,i3-2*js3)-sx(i1-2*js1,i2-
     & 2*js2,i3-2*js3)*tz(i1-2*js1,i2-2*js2,i3-2*js3))+rz(i1-2*js1,i2-
     & 2*js2,i3-2*js3)*(sx(i1-2*js1,i2-2*js2,i3-2*js3)*ty(i1-2*js1,i2-
     & 2*js2,i3-2*js3)-sy(i1-2*js1,i2-2*js2,i3-2*js3)*tx(i1-2*js1,i2-
     & 2*js2,i3-2*js3))))
      a13c=(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axis,2)/(rx(i1-2*js1,i2-2*
     & js2,i3-2*js3)*(sy(i1-2*js1,i2-2*js2,i3-2*js3)*tz(i1-2*js1,i2-2*
     & js2,i3-2*js3)-sz(i1-2*js1,i2-2*js2,i3-2*js3)*ty(i1-2*js1,i2-2*
     & js2,i3-2*js3))+ry(i1-2*js1,i2-2*js2,i3-2*js3)*(sz(i1-2*js1,i2-
     & 2*js2,i3-2*js3)*tx(i1-2*js1,i2-2*js2,i3-2*js3)-sx(i1-2*js1,i2-
     & 2*js2,i3-2*js3)*tz(i1-2*js1,i2-2*js2,i3-2*js3))+rz(i1-2*js1,i2-
     & 2*js2,i3-2*js3)*(sx(i1-2*js1,i2-2*js2,i3-2*js3)*ty(i1-2*js1,i2-
     & 2*js2,i3-2*js3)-sy(i1-2*js1,i2-2*js2,i3-2*js3)*tx(i1-2*js1,i2-
     & 2*js2,i3-2*js3))))
      a21c=(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axisp1,0)/(rx(i1-2*js1,i2-
     & 2*js2,i3-2*js3)*(sy(i1-2*js1,i2-2*js2,i3-2*js3)*tz(i1-2*js1,i2-
     & 2*js2,i3-2*js3)-sz(i1-2*js1,i2-2*js2,i3-2*js3)*ty(i1-2*js1,i2-
     & 2*js2,i3-2*js3))+ry(i1-2*js1,i2-2*js2,i3-2*js3)*(sz(i1-2*js1,
     & i2-2*js2,i3-2*js3)*tx(i1-2*js1,i2-2*js2,i3-2*js3)-sx(i1-2*js1,
     & i2-2*js2,i3-2*js3)*tz(i1-2*js1,i2-2*js2,i3-2*js3))+rz(i1-2*js1,
     & i2-2*js2,i3-2*js3)*(sx(i1-2*js1,i2-2*js2,i3-2*js3)*ty(i1-2*js1,
     & i2-2*js2,i3-2*js3)-sy(i1-2*js1,i2-2*js2,i3-2*js3)*tx(i1-2*js1,
     & i2-2*js2,i3-2*js3))))
      a22c=(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axisp1,1)/(rx(i1-2*js1,i2-
     & 2*js2,i3-2*js3)*(sy(i1-2*js1,i2-2*js2,i3-2*js3)*tz(i1-2*js1,i2-
     & 2*js2,i3-2*js3)-sz(i1-2*js1,i2-2*js2,i3-2*js3)*ty(i1-2*js1,i2-
     & 2*js2,i3-2*js3))+ry(i1-2*js1,i2-2*js2,i3-2*js3)*(sz(i1-2*js1,
     & i2-2*js2,i3-2*js3)*tx(i1-2*js1,i2-2*js2,i3-2*js3)-sx(i1-2*js1,
     & i2-2*js2,i3-2*js3)*tz(i1-2*js1,i2-2*js2,i3-2*js3))+rz(i1-2*js1,
     & i2-2*js2,i3-2*js3)*(sx(i1-2*js1,i2-2*js2,i3-2*js3)*ty(i1-2*js1,
     & i2-2*js2,i3-2*js3)-sy(i1-2*js1,i2-2*js2,i3-2*js3)*tx(i1-2*js1,
     & i2-2*js2,i3-2*js3))))
      a23c=(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axisp1,2)/(rx(i1-2*js1,i2-
     & 2*js2,i3-2*js3)*(sy(i1-2*js1,i2-2*js2,i3-2*js3)*tz(i1-2*js1,i2-
     & 2*js2,i3-2*js3)-sz(i1-2*js1,i2-2*js2,i3-2*js3)*ty(i1-2*js1,i2-
     & 2*js2,i3-2*js3))+ry(i1-2*js1,i2-2*js2,i3-2*js3)*(sz(i1-2*js1,
     & i2-2*js2,i3-2*js3)*tx(i1-2*js1,i2-2*js2,i3-2*js3)-sx(i1-2*js1,
     & i2-2*js2,i3-2*js3)*tz(i1-2*js1,i2-2*js2,i3-2*js3))+rz(i1-2*js1,
     & i2-2*js2,i3-2*js3)*(sx(i1-2*js1,i2-2*js2,i3-2*js3)*ty(i1-2*js1,
     & i2-2*js2,i3-2*js3)-sy(i1-2*js1,i2-2*js2,i3-2*js3)*tx(i1-2*js1,
     & i2-2*js2,i3-2*js3))))
      a31c=(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axisp2,0)/(rx(i1-2*js1,i2-
     & 2*js2,i3-2*js3)*(sy(i1-2*js1,i2-2*js2,i3-2*js3)*tz(i1-2*js1,i2-
     & 2*js2,i3-2*js3)-sz(i1-2*js1,i2-2*js2,i3-2*js3)*ty(i1-2*js1,i2-
     & 2*js2,i3-2*js3))+ry(i1-2*js1,i2-2*js2,i3-2*js3)*(sz(i1-2*js1,
     & i2-2*js2,i3-2*js3)*tx(i1-2*js1,i2-2*js2,i3-2*js3)-sx(i1-2*js1,
     & i2-2*js2,i3-2*js3)*tz(i1-2*js1,i2-2*js2,i3-2*js3))+rz(i1-2*js1,
     & i2-2*js2,i3-2*js3)*(sx(i1-2*js1,i2-2*js2,i3-2*js3)*ty(i1-2*js1,
     & i2-2*js2,i3-2*js3)-sy(i1-2*js1,i2-2*js2,i3-2*js3)*tx(i1-2*js1,
     & i2-2*js2,i3-2*js3))))
      a32c=(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axisp2,1)/(rx(i1-2*js1,i2-
     & 2*js2,i3-2*js3)*(sy(i1-2*js1,i2-2*js2,i3-2*js3)*tz(i1-2*js1,i2-
     & 2*js2,i3-2*js3)-sz(i1-2*js1,i2-2*js2,i3-2*js3)*ty(i1-2*js1,i2-
     & 2*js2,i3-2*js3))+ry(i1-2*js1,i2-2*js2,i3-2*js3)*(sz(i1-2*js1,
     & i2-2*js2,i3-2*js3)*tx(i1-2*js1,i2-2*js2,i3-2*js3)-sx(i1-2*js1,
     & i2-2*js2,i3-2*js3)*tz(i1-2*js1,i2-2*js2,i3-2*js3))+rz(i1-2*js1,
     & i2-2*js2,i3-2*js3)*(sx(i1-2*js1,i2-2*js2,i3-2*js3)*ty(i1-2*js1,
     & i2-2*js2,i3-2*js3)-sy(i1-2*js1,i2-2*js2,i3-2*js3)*tx(i1-2*js1,
     & i2-2*js2,i3-2*js3))))
      a33c=(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axisp2,2)/(rx(i1-2*js1,i2-
     & 2*js2,i3-2*js3)*(sy(i1-2*js1,i2-2*js2,i3-2*js3)*tz(i1-2*js1,i2-
     & 2*js2,i3-2*js3)-sz(i1-2*js1,i2-2*js2,i3-2*js3)*ty(i1-2*js1,i2-
     & 2*js2,i3-2*js3))+ry(i1-2*js1,i2-2*js2,i3-2*js3)*(sz(i1-2*js1,
     & i2-2*js2,i3-2*js3)*tx(i1-2*js1,i2-2*js2,i3-2*js3)-sx(i1-2*js1,
     & i2-2*js2,i3-2*js3)*tz(i1-2*js1,i2-2*js2,i3-2*js3))+rz(i1-2*js1,
     & i2-2*js2,i3-2*js3)*(sx(i1-2*js1,i2-2*js2,i3-2*js3)*ty(i1-2*js1,
     & i2-2*js2,i3-2*js3)-sy(i1-2*js1,i2-2*js2,i3-2*js3)*tx(i1-2*js1,
     & i2-2*js2,i3-2*js3))))

      a2a2=a21c*a21c+a22c*a22c+a23c*a23c
      a1a2=a11c*a21c+a12c*a22c+a13c*a23c
      a1a3=a11c*a31c+a12c*a32c+a13c*a33c
      a2a3=a21c*a31c+a22c*a32c+a23c*a33c
      a3a3=a31c*a31c+a32c*a32c+a33c*a33c
! The tangential component is assumed valid:
      a2Dotu4=a21c*u(i1-2*js1,i2-2*js2,i3-2*js3,ex)+a22c*u(i1-2*js1,i2-
     & 2*js2,i3-2*js3,ey)+a23c*u(i1-2*js1,i2-2*js2,i3-2*js3,ez)
      a3Dotu4=a31c*u(i1-2*js1,i2-2*js2,i3-2*js3,ex)+a32c*u(i1-2*js1,i2-
     & 2*js2,i3-2*js3,ey)+a33c*u(i1-2*js1,i2-2*js2,i3-2*js3,ez)
 ! u(i1-2*js1,i2-2*js2,i3-2*js3,k) = b4[k]*x4 +g4[k]
      b41 =a11c-a21c/(a2a3**2-a3a3*a2a2)*(-a1a2*a3a3+a1a3*a2a3)+a31c*(-
     & a1a2*a2a3+a2a2*a1a3)/(a2a3**2-a3a3*a2a2)
      b42 =a12c-a22c/(a2a3**2-a3a3*a2a2)*(-a1a2*a3a3+a1a3*a2a3)+a32c*(-
     & a1a2*a2a3+a2a2*a1a3)/(a2a3**2-a3a3*a2a2)
      b43 =a13c-a23c/(a2a3**2-a3a3*a2a2)*(-a1a2*a3a3+a1a3*a2a3)+a33c*(-
     & a1a2*a2a3+a2a2*a1a3)/(a2a3**2-a3a3*a2a2)
      g41 =-(a21c*a3a3*a2Dotu4-a21c*a3Dotu4*a2a3+a31c*a2a2*a3Dotu4-
     & a31c*a2a3*a2Dotu4)/(a2a3**2-a3a3*a2a2)
      g42 =-(a22c*a3a3*a2Dotu4-a22c*a3Dotu4*a2a3+a32c*a2a2*a3Dotu4-
     & a32c*a2a3*a2Dotu4)/(a2a3**2-a3a3*a2a2)
      g43 =(-a23c*a3a3*a2Dotu4+a23c*a3Dotu4*a2a3-a33c*a2a2*a3Dotu4+
     & a33c*a2a3*a2Dotu4)/(a2a3**2-a3a3*a2a2)

 ! Evaluate a1, a2 and a3 at the corner
      a11=(rsxy(i1,i2,i3,axis,0)/(rx(i1,i2,i3)*(sy(i1,i2,i3)*tz(i1,i2,
     & i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,i2,i3)*tx(
     & i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(sx(i1,i2,i3)
     & *ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))
      a12=(rsxy(i1,i2,i3,axis,1)/(rx(i1,i2,i3)*(sy(i1,i2,i3)*tz(i1,i2,
     & i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,i2,i3)*tx(
     & i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(sx(i1,i2,i3)
     & *ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))
      a13=(rsxy(i1,i2,i3,axis,2)/(rx(i1,i2,i3)*(sy(i1,i2,i3)*tz(i1,i2,
     & i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,i2,i3)*tx(
     & i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(sx(i1,i2,i3)
     & *ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))
      a21=(rsxy(i1,i2,i3,axisp1,0)/(rx(i1,i2,i3)*(sy(i1,i2,i3)*tz(i1,
     & i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,i2,i3)*
     & tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(sx(i1,i2,
     & i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))
      a22=(rsxy(i1,i2,i3,axisp1,1)/(rx(i1,i2,i3)*(sy(i1,i2,i3)*tz(i1,
     & i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,i2,i3)*
     & tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(sx(i1,i2,
     & i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))
      a23=(rsxy(i1,i2,i3,axisp1,2)/(rx(i1,i2,i3)*(sy(i1,i2,i3)*tz(i1,
     & i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,i2,i3)*
     & tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(sx(i1,i2,
     & i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))
      a31=(rsxy(i1,i2,i3,axisp2,0)/(rx(i1,i2,i3)*(sy(i1,i2,i3)*tz(i1,
     & i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,i2,i3)*
     & tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(sx(i1,i2,
     & i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))
      a32=(rsxy(i1,i2,i3,axisp2,1)/(rx(i1,i2,i3)*(sy(i1,i2,i3)*tz(i1,
     & i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,i2,i3)*
     & tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(sx(i1,i2,
     & i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))
      a33=(rsxy(i1,i2,i3,axisp2,2)/(rx(i1,i2,i3)*(sy(i1,i2,i3)*tz(i1,
     & i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,i2,i3)*
     & tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(sx(i1,i2,
     & i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))

      a1DotLu=a11*DeltaU+a12*DeltaV+a13*DeltaW
      a2DotLu=a21*DeltaU+a22*DeltaV+a23*DeltaW

!   a1.Lu = 0 
! e1 := cc11a*u(i1-2,i2,i3)+cc12a*v(i1-2,i2,i3)+cc13a*w(i1-2,i2,i3)
!     + cc14a*u(i1-1,i2,i3)+cc15a*v(i1-1,i2,i3)+cc16a*w(i1-1,i2,i3) 
!     + cc11b*u(i1,i2-2,i3)+cc12b*v(i1,i2-2,i3)+cc13b*w(i1,i2-2,i3)
!     + cc14b*u(i1,i2-1,i3)+cc15b*v(i1,i2-1,i3)+cc16b*w(i1,i2-1,i3) - f1:
!  a2.Lu = 0 :
! e2 := cc21a*u(i1-2,i2,i3)+cc22a*v(i1-2,i2,i3)+cc23a*w(i1-2,i2,i3)
!     + cc24a*u(i1-1,i2,i3)+cc25a*v(i1-1,i2,i3)+cc26a*w(i1-1,i2,i3) 
!     + cc21b*u(i1,i2-2,i3)+cc22b*v(i1,i2-2,i3)+cc23b*w(i1,i2-2,i3) 
!     + cc24b*u(i1,i2-1,i3)+cc25b*v(i1,i2-1,i3)+cc26b*w(i1,i2-1,i3) - f2:
      cc11a=(-1/12.*c11/dra**2+1/12.*c1/dra)*a11
      cc12a=(-1/12.*c11/dra**2+1/12.*c1/dra)*a12
      cc13a=(-1/12.*c11/dra**2+1/12.*c1/dra)*a13
      cc14a=(4/3.*c11/dra**2-2/3.*c1/dra)*a11
      cc15a=(4/3.*c11/dra**2-2/3.*c1/dra)*a12
      cc16a=(4/3.*c11/dra**2-2/3.*c1/dra)*a13
      cc11b=(-1/12.*c22/dsa**2+1/12.*c2/dsa)*a11
      cc12b=(-1/12.*c22/dsa**2+1/12.*c2/dsa)*a12
      cc13b=(-1/12.*c22/dsa**2+1/12.*c2/dsa)*a13
      cc14b=(4/3.*c22/dsa**2-2/3.*c2/dsa)*a11
      cc15b=(4/3.*c22/dsa**2-2/3.*c2/dsa)*a12
      cc16b=(4/3.*c22/dsa**2-2/3.*c2/dsa)*a13
      cc21a=(-1/12.*c11/dra**2+1/12.*c1/dra)*a21
      cc22a=(-1/12.*c11/dra**2+1/12.*c1/dra)*a22
      cc23a=(-1/12.*c11/dra**2+1/12.*c1/dra)*a23
      cc24a=(4/3.*c11/dra**2-2/3.*c1/dra)*a21
      cc25a=(4/3.*c11/dra**2-2/3.*c1/dra)*a22
      cc26a=(4/3.*c11/dra**2-2/3.*c1/dra)*a23
      cc21b=(-1/12.*c22/dsa**2+1/12.*c2/dsa)*a21
      cc22b=(-1/12.*c22/dsa**2+1/12.*c2/dsa)*a22
      cc23b=(-1/12.*c22/dsa**2+1/12.*c2/dsa)*a23
      cc24b=(4/3.*c22/dsa**2-2/3.*c2/dsa)*a21
      cc25b=(4/3.*c22/dsa**2-2/3.*c2/dsa)*a22
      cc26b=(4/3.*c22/dsa**2-2/3.*c2/dsa)*a23

      f1=a1DotLu-cc11a*u(i1-2*is1,i2-2*is2,i3-2*is3,ex)-cc12a*u(i1-2*
     & is1,i2-2*is2,i3-2*is3,ey)-cc13a*u(i1-2*is1,i2-2*is2,i3-2*is3,
     & ez)-cc14a*u(i1-is1,i2-is2,i3-is3,ex)-cc15a*u(i1-is1,i2-is2,i3-
     & is3,ey)-cc16a*u(i1-is1,i2-is2,i3-is3,ez)-cc11b*u(i1-2*js1,i2-2*
     & js2,i3-2*js3,ex)-cc12b*u(i1-2*js1,i2-2*js2,i3-2*js3,ey)-cc13b*
     & u(i1-2*js1,i2-2*js2,i3-2*js3,ez)-cc14b*u(i1-js1,i2-js2,i3-js3,
     & ex)-cc15b*u(i1-js1,i2-js2,i3-js3,ey)-cc16b*u(i1-js1,i2-js2,i3-
     & js3,ez)
      f2=a2DotLu-cc21a*u(i1-2*is1,i2-2*is2,i3-2*is3,ex)-cc22a*u(i1-2*
     & is1,i2-2*is2,i3-2*is3,ey)-cc23a*u(i1-2*is1,i2-2*is2,i3-2*is3,
     & ez)-cc24a*u(i1-is1,i2-is2,i3-is3,ex)-cc25a*u(i1-is1,i2-is2,i3-
     & is3,ey)-cc26a*u(i1-is1,i2-is2,i3-is3,ez)-cc21b*u(i1-2*js1,i2-2*
     & js2,i3-2*js3,ex)-cc22b*u(i1-2*js1,i2-2*js2,i3-2*js3,ey)-cc23b*
     & u(i1-2*js1,i2-2*js2,i3-2*js3,ez)-cc24b*u(i1-js1,i2-js2,i3-js3,
     & ex)-cc25b*u(i1-js1,i2-js2,i3-js3,ey)-cc26b*u(i1-js1,i2-js2,i3-
     & js3,ez)
      f3=6*a21*u(i1,i2,i3,ex)+6*a22*u(i1,i2,i3,ey)+6*a23*u(i1,i2,i3,ez)
     & -4*a21*u(i1+is1,i2+is2,i3+is3,ex)-4*a22*u(i1+is1,i2+is2,i3+is3,
     & ey)-4*a23*u(i1+is1,i2+is2,i3+is3,ez)+a21*u(i1+2*is1,i2+2*is2,
     & i3+2*is3,ex)+a22*u(i1+2*is1,i2+2*is2,i3+2*is3,ey)+a23*u(i1+2*
     & is1,i2+2*is2,i3+2*is3,ez)-g2f
      f4=6*a11*u(i1,i2,i3,ex)+6*a12*u(i1,i2,i3,ey)+6*a13*u(i1,i2,i3,ez)
     & -4*a11*u(i1+js1,i2+js2,i3+js3,ex)-4*a12*u(i1+js1,i2+js2,i3+js3,
     & ey)-4*a13*u(i1+js1,i2+js2,i3+js3,ez)+a11*u(i1+2*js1,i2+2*js2,
     & i3+2*js3,ex)+a12*u(i1+2*js1,i2+2*js2,i3+2*js3,ey)+a13*u(i1+2*
     & js1,i2+2*js2,i3+2*js3,ez)-g1f

 ! Simplfied forms for the 4 equations a1.Lu, a2.Lu, a2.D+r4 u = g2f  a1.D+s4 u = g1f
 ! e1x := dd11*x1+dd12*x2+dd13*x3+dd14*x4+ f1x
 ! e2x := dd21*x1+dd22*x2+dd23*x3+dd24*x4+ f2x
 ! e3x := dd31*x1+dd32*x2+dd33*x3+dd34*x4+ f3x
 ! e4x := dd41*x1+dd42*x2+dd43*x3+dd44*x4+ f4x
      dd11=cc14a*b11+cc15a*b12+cc16a*b13
      dd12=cc11a*b21+cc12a*b22+cc13a*b23
      dd13=cc14b*b31+cc15b*b32+cc16b*b33
      dd14=cc11b*b41+cc12b*b42+cc13b*b43
      dd21=cc24a*b11+cc25a*b12+cc26a*b13
      dd22=cc21a*b21+cc22a*b22+cc23a*b23
      dd23=cc24b*b31+cc25b*b32+cc26b*b33
      dd24=cc21b*b41+cc22b*b42+cc23b*b43
      dd31=-4*a21*b11-4*a22*b12-4*a23*b13
      dd32=a21*b21+a22*b22+a23*b23
      dd33=0
      dd34=0
      dd41=0
      dd42=0
      dd43=-4*a11*b31-4*a12*b32-4*a13*b33
      dd44=a11*b41+a12*b42+a13*b43

      f1x=cc11a*g21+cc12a*g22+cc13a*g23+cc14a*g11+cc15a*g12+cc16a*g13+
     & cc11b*g41+cc12b*g42+cc13b*g43+cc14b*g31+cc15b*g32+cc16b*g33+f1
      f2x=cc21a*g21+cc22a*g22+cc23a*g23+cc24a*g11+cc25a*g12+cc26a*g13+
     & cc21b*g41+cc22b*g42+cc23b*g43+cc24b*g31+cc25b*g32+cc26b*g33+f2
      f3x=a21*g21+a22*g22+a23*g23-4*a21*g11-4*a22*g12-4*a23*g13+f3
      f4x=a11*g41+a12*g42+a13*g43-4*a11*g31-4*a12*g32-4*a13*g33+f4

!  solution x1,x2,x3,x4: 
      det=-dd32*dd43*dd14*dd21-dd32*dd11*dd23*dd44+dd32*dd11*dd43*dd24+
     & dd43*dd14*dd22*dd31-dd13*dd44*dd22*dd31+dd12*dd31*dd23*dd44+
     & dd32*dd13*dd44*dd21-dd12*dd31*dd43*dd24
      x1=(-dd32*f2x*dd13*dd44-dd32*dd43*dd24*f1x+dd32*dd23*dd44*f1x+
     & dd32*dd43*f2x*dd14-dd32*dd23*f4x*dd14+dd32*dd24*dd13*f4x-dd23*
     & dd44*dd12*f3x+dd22*f3x*dd13*dd44-dd43*dd22*f3x*dd14+dd43*dd24*
     & dd12*f3x)/det
      x2=(dd31*f2x*dd13*dd44+dd31*dd43*dd24*f1x-dd31*dd23*dd44*f1x-
     & dd31*dd43*f2x*dd14+dd31*dd23*f4x*dd14-dd31*dd24*dd13*f4x+f3x*
     & dd43*dd14*dd21+f3x*dd11*dd23*dd44-f3x*dd11*dd43*dd24-f3x*dd13*
     & dd44*dd21)/det
      x3=(dd44*dd32*dd11*f2x-dd44*dd12*dd31*f2x+dd44*dd12*f3x*dd21-
     & dd44*dd32*f1x*dd21-dd44*dd11*dd22*f3x+dd44*f1x*dd22*dd31+f4x*
     & dd32*dd14*dd21-f4x*dd32*dd11*dd24-f4x*dd14*dd22*dd31+f4x*dd12*
     & dd31*dd24)/det
      x4=(-dd32*dd13*f4x*dd21-dd32*dd11*dd43*f2x+dd12*dd31*dd43*f2x-
     & dd12*dd31*dd23*f4x-dd43*dd12*f3x*dd21+dd32*dd43*f1x*dd21+dd32*
     & dd11*dd23*f4x+dd13*f4x*dd22*dd31+dd11*dd43*dd22*f3x-dd43*f1x*
     & dd22*dd31)/det

! **** Now assign the extended boundary points **** 
      u(i1-  is1,i2-  is2,i3-  is3,ex) = b11*x1+ g11
      u(i1-  is1,i2-  is2,i3-  is3,ey) = b12*x1+ g12
      u(i1-  is1,i2-  is2,i3-  is3,ez) = b13*x1+ g13
      u(i1-2*is1,i2-2*is2,i3-2*is3,ex) = b21*x2+ g21
      u(i1-2*is1,i2-2*is2,i3-2*is3,ey) = b22*x2+ g22
      u(i1-2*is1,i2-2*is2,i3-2*is3,ez) = b23*x2+ g23
      u(i1-  js1,i2-  js2,i3-  js3,ex) = b31*x3+ g31
      u(i1-  js1,i2-  js2,i3-  js3,ey) = b32*x3+ g32
      u(i1-  js1,i2-  js2,i3-  js3,ez) = b33*x3+ g33
      u(i1-2*js1,i2-2*js2,i3-2*js3,ex) = b41*x4+ g41
      u(i1-2*js1,i2-2*js2,i3-2*js3,ey) = b42*x4+ g42
      u(i1-2*js1,i2-2*js2,i3-2*js3,ez) = b43*x4+ g43
                     else
                       ! -----------------------------------------------------------------------------
                       ! --------------- fill in extended face values by extrapolation ---------------
                       ! ---------------         curvilinear, order 4                  ---------------
                       ! -----------------------------------------------------------------------------
                       ! *wdh* 2015/06/24 **WRONG**
                       ! -- for fourth-order scheme:
                       ! u(i1-js1,i2-js2,i3-js3,ex)=extrapolate5(ex,i1-js1,i2-js2,i3-js3,is1,is2,is3)
                       ! u(i1-js1,i2-js2,i3-js3,ey)=extrapolate5(ey,i1-js1,i2-js2,i3-js3,is1,is2,is3)
                       ! u(i1-js1,i2-js2,i3-js3,ez)=extrapolate5(ez,i1-js1,i2-js2,i3-js3,is1,is2,is3)
                      ! *wdh* 2015/07/13  
                      u(i1-  is1,i2-  is2,i3-  is3,ex) = (5.*u(i1-is1+
     & is1,i2-is2+is2,i3-is3+is3,ex)-10.*u(i1-is1+2*is1,i2-is2+2*is2,
     & i3-is3+2*is3,ex)+10.*u(i1-is1+3*is1,i2-is2+3*is2,i3-is3+3*is3,
     & ex)-5.*u(i1-is1+4*is1,i2-is2+4*is2,i3-is3+4*is3,ex)+u(i1-is1+5*
     & is1,i2-is2+5*is2,i3-is3+5*is3,ex))
                      u(i1-  is1,i2-  is2,i3-  is3,ey) = (5.*u(i1-is1+
     & is1,i2-is2+is2,i3-is3+is3,ey)-10.*u(i1-is1+2*is1,i2-is2+2*is2,
     & i3-is3+2*is3,ey)+10.*u(i1-is1+3*is1,i2-is2+3*is2,i3-is3+3*is3,
     & ey)-5.*u(i1-is1+4*is1,i2-is2+4*is2,i3-is3+4*is3,ey)+u(i1-is1+5*
     & is1,i2-is2+5*is2,i3-is3+5*is3,ey))
                      u(i1-  is1,i2-  is2,i3-  is3,ez) = (5.*u(i1-is1+
     & is1,i2-is2+is2,i3-is3+is3,ez)-10.*u(i1-is1+2*is1,i2-is2+2*is2,
     & i3-is3+2*is3,ez)+10.*u(i1-is1+3*is1,i2-is2+3*is2,i3-is3+3*is3,
     & ez)-5.*u(i1-is1+4*is1,i2-is2+4*is2,i3-is3+4*is3,ez)+u(i1-is1+5*
     & is1,i2-is2+5*is2,i3-is3+5*is3,ez))
                      u(i1-2*is1,i2-2*is2,i3-2*is3,ex) = (5.*u(i1-2*
     & is1+is1,i2-2*is2+is2,i3-2*is3+is3,ex)-10.*u(i1-2*is1+2*is1,i2-
     & 2*is2+2*is2,i3-2*is3+2*is3,ex)+10.*u(i1-2*is1+3*is1,i2-2*is2+3*
     & is2,i3-2*is3+3*is3,ex)-5.*u(i1-2*is1+4*is1,i2-2*is2+4*is2,i3-2*
     & is3+4*is3,ex)+u(i1-2*is1+5*is1,i2-2*is2+5*is2,i3-2*is3+5*is3,
     & ex))
                      u(i1-2*is1,i2-2*is2,i3-2*is3,ey) = (5.*u(i1-2*
     & is1+is1,i2-2*is2+is2,i3-2*is3+is3,ey)-10.*u(i1-2*is1+2*is1,i2-
     & 2*is2+2*is2,i3-2*is3+2*is3,ey)+10.*u(i1-2*is1+3*is1,i2-2*is2+3*
     & is2,i3-2*is3+3*is3,ey)-5.*u(i1-2*is1+4*is1,i2-2*is2+4*is2,i3-2*
     & is3+4*is3,ey)+u(i1-2*is1+5*is1,i2-2*is2+5*is2,i3-2*is3+5*is3,
     & ey))
                      u(i1-2*is1,i2-2*is2,i3-2*is3,ez) = (5.*u(i1-2*
     & is1+is1,i2-2*is2+is2,i3-2*is3+is3,ez)-10.*u(i1-2*is1+2*is1,i2-
     & 2*is2+2*is2,i3-2*is3+2*is3,ez)+10.*u(i1-2*is1+3*is1,i2-2*is2+3*
     & is2,i3-2*is3+3*is3,ez)-5.*u(i1-2*is1+4*is1,i2-2*is2+4*is2,i3-2*
     & is3+4*is3,ez)+u(i1-2*is1+5*is1,i2-2*is2+5*is2,i3-2*is3+5*is3,
     & ez))
                      u(i1-  js1,i2-  js2,i3-  js3,ex) = (5.*u(i1-js1+
     & js1,i2-js2+js2,i3-js3+js3,ex)-10.*u(i1-js1+2*js1,i2-js2+2*js2,
     & i3-js3+2*js3,ex)+10.*u(i1-js1+3*js1,i2-js2+3*js2,i3-js3+3*js3,
     & ex)-5.*u(i1-js1+4*js1,i2-js2+4*js2,i3-js3+4*js3,ex)+u(i1-js1+5*
     & js1,i2-js2+5*js2,i3-js3+5*js3,ex))
                      u(i1-  js1,i2-  js2,i3-  js3,ey) = (5.*u(i1-js1+
     & js1,i2-js2+js2,i3-js3+js3,ey)-10.*u(i1-js1+2*js1,i2-js2+2*js2,
     & i3-js3+2*js3,ey)+10.*u(i1-js1+3*js1,i2-js2+3*js2,i3-js3+3*js3,
     & ey)-5.*u(i1-js1+4*js1,i2-js2+4*js2,i3-js3+4*js3,ey)+u(i1-js1+5*
     & js1,i2-js2+5*js2,i3-js3+5*js3,ey))
                      u(i1-  js1,i2-  js2,i3-  js3,ez) = (5.*u(i1-js1+
     & js1,i2-js2+js2,i3-js3+js3,ez)-10.*u(i1-js1+2*js1,i2-js2+2*js2,
     & i3-js3+2*js3,ez)+10.*u(i1-js1+3*js1,i2-js2+3*js2,i3-js3+3*js3,
     & ez)-5.*u(i1-js1+4*js1,i2-js2+4*js2,i3-js3+4*js3,ez)+u(i1-js1+5*
     & js1,i2-js2+5*js2,i3-js3+5*js3,ez))
                      u(i1-2*js1,i2-2*js2,i3-2*js3,ex) = (5.*u(i1-2*
     & js1+js1,i2-2*js2+js2,i3-2*js3+js3,ex)-10.*u(i1-2*js1+2*js1,i2-
     & 2*js2+2*js2,i3-2*js3+2*js3,ex)+10.*u(i1-2*js1+3*js1,i2-2*js2+3*
     & js2,i3-2*js3+3*js3,ex)-5.*u(i1-2*js1+4*js1,i2-2*js2+4*js2,i3-2*
     & js3+4*js3,ex)+u(i1-2*js1+5*js1,i2-2*js2+5*js2,i3-2*js3+5*js3,
     & ex))
                      u(i1-2*js1,i2-2*js2,i3-2*js3,ey) = (5.*u(i1-2*
     & js1+js1,i2-2*js2+js2,i3-2*js3+js3,ey)-10.*u(i1-2*js1+2*js1,i2-
     & 2*js2+2*js2,i3-2*js3+2*js3,ey)+10.*u(i1-2*js1+3*js1,i2-2*js2+3*
     & js2,i3-2*js3+3*js3,ey)-5.*u(i1-2*js1+4*js1,i2-2*js2+4*js2,i3-2*
     & js3+4*js3,ey)+u(i1-2*js1+5*js1,i2-2*js2+5*js2,i3-2*js3+5*js3,
     & ey))
                      u(i1-2*js1,i2-2*js2,i3-2*js3,ez) = (5.*u(i1-2*
     & js1+js1,i2-2*js2+js2,i3-2*js3+js3,ez)-10.*u(i1-2*js1+2*js1,i2-
     & 2*js2+2*js2,i3-2*js3+2*js3,ez)+10.*u(i1-2*js1+3*js1,i2-2*js2+3*
     & js2,i3-2*js3+3*js3,ez)-5.*u(i1-2*js1+4*js1,i2-2*js2+4*js2,i3-2*
     & js3+4*js3,ez)+u(i1-2*js1+5*js1,i2-2*js2+5*js2,i3-2*js3+5*js3,
     & ez))
                !       ! Face 1:  (note: only one of (is1,is2,is3) is non-zero)
                !       u(i1-  is1,i2-  is2,i3-  is3,ex) = extrap5(u,i1,i2,i3,ex,is1,is2,is3)
                !       u(i1-  is1,i2-  is2,i3-  is3,ey) = extrap5(u,i1,i2,i3,ey,is1,is2,is3)
                !       u(i1-  is1,i2-  is2,i3-  is3,ez) = extrap5(u,i1,i2,i3,ez,is1,is2,is3)
                ! 
                !       u(i1-2*is1,i2-2*is2,i3-2*is3,ex) = extrap5(u,i1-is1,i2-is2,i3-is3,ex,is1,is2,is3)
                !       u(i1-2*is1,i2-2*is2,i3-2*is3,ey) = extrap5(u,i1-is1,i2-is2,i3-is3,ey,is1,is2,is3)
                !       u(i1-2*is1,i2-2*is2,i3-2*is3,ez) = extrap5(u,i1-is1,i2-is2,i3-is3,ez,is1,is2,is3)
                ! 
                !       ! Face 2 : (note: only one of (js1,js2,js3) is non-zero)
                !       u(i1-  js1,i2-  js2,i3-  js3,ex) = extrap5(u,i1,i2,i3,ex,js1,js2,js3)            
                !       u(i1-  js1,i2-  js2,i3-  js3,ey) = extrap5(u,i1,i2,i3,ey,js1,js2,js3)            
                !       u(i1-  js1,i2-  js2,i3-  js3,ez) = extrap5(u,i1,i2,i3,ez,js1,js2,js3)            
                !                                                                                      
                !       u(i1-2*js1,i2-2*js2,i3-2*js3,ex) = extrap5(u,i1-js1,i2-js2,i3-js3,ex,js1,js2,js3)
                !       u(i1-2*js1,i2-2*js2,i3-2*js3,ey) = extrap5(u,i1-js1,i2-js2,i3-js3,ey,js1,js2,js3)
                !       u(i1-2*js1,i2-2*js2,i3-2*js3,ez) = extrap5(u,i1-js1,i2-js2,i3-js3,ez,js1,js2,js3)
                     end if
                     end do ! end do i1
                     end do ! end do i2
                     end do ! end do i3
                   else if( bc1.eq.dirichlet .or. bc2.eq.dirichlet )
     & then
                ! *wdh* 081124 -- do nothing here ---
                ! *      do i3=n3a,n3b
                ! *      do i2=n2a,n2b
                ! *      do i1=n1a,n1b
                ! * 
                ! *       if( edgeDirection.ne.0 )then
                ! *         #If "none" == "twilightZone"
                ! *           OGF3DFO(i1-js1,i2,i3,t,g1,g2,g3)
                ! *         #End
                ! *         u(i1-js1,i2,i3,ex)=g1
                ! *         u(i1-js1,i2,i3,ey)=g2
                ! *         u(i1-js1,i2,i3,ez)=g3
                ! *       end if
                ! * 
                ! *       if( edgeDirection.ne.1 )then
                ! *         #If "none" == "twilightZone"
                ! *           OGF3DFO(i1,i2-js2,i3,t,g1,g2,g3)
                ! *         #End
                ! *         u(i1,i2-js2,i3,ex)=g1
                ! *         u(i1,i2-js2,i3,ey)=g2
                ! *         u(i1,i2-js2,i3,ez)=g3
                ! *       end if
                ! * 
                ! *       if( edgeDirection.ne.2 )then
                ! *         #If "none" == "twilightZone"
                ! *           OGF3DFO(i1,i2,i3-js3,t,g1,g2,g3)
                ! *         #End
                ! *         u(i1,i2,i3-js3,ex)=g1
                ! *         u(i1,i2,i3-js3,ey)=g2
                ! *         u(i1,i2,i3-js3,ez)=g3
                ! *       end if
                ! * 
                ! *      end do ! end do i1
                ! *      end do ! end do i2
                ! *      end do ! end do i3
                    else if( bc1.le.0 .or. bc2.le.0 )then
                      ! periodic or interpolation -- nothing to do
                    else if( bc1.eq.planeWaveBoundaryCondition 
     & .or.bc2.eq.planeWaveBoundaryCondition .or. 
     & bc1.eq.symmetryBoundaryCondition .or. 
     & bc2.eq.symmetryBoundaryCondition .or. (bc1.ge.abcEM2 .and. 
     & bc1.le.lastBC) .or. (bc2.ge.abcEM2 .and. bc2.le.lastBC))then
                      ! do nothing
                    else
                      write(*,'("ERROR: unknown boundary conditions 
     & bc1,bc2=",2i3)') bc1,bc2
                      ! unknown boundary conditions
                      stop 8866
                   end if
                 ! end orderOfAccuracy==4 
                 end do
                 end do
                 end do ! edge direction
                 ! *****************************************************************************
                 ! ************ assign corner GHOST points outside edges ***********************
                 ! ****************************************************************************
                  do edgeDirection=0,2 ! direction parallel to the edge
                 ! do edgeDirection=2,2 ! direction parallel to the edge
                  do sidea=0,1
                  do sideb=0,1
                   if( edgeDirection.eq.0 )then
                     side1=0
                     side2=sidea
                     side3=sideb
                   else if( edgeDirection.eq.1 )then
                     side1=sideb
                     side2=0
                     side3=sidea
                   else
                     side1=sidea
                     side2=sideb
                     side3=0
                   end if
                  extra=numberOfGhostPoints  ! assign the extended boundary *wdh* 2015/06/23
                  is1=1-2*(side1)
                  is2=1-2*(side2)
                  is3=1-2*(side3)
                  if( edgeDirection.eq.2 )then
                   is3=0
                   n1a=gridIndexRange(side1,0)
                   n1b=gridIndexRange(side1,0)
                   n2a=gridIndexRange(side2,1)
                   n2b=gridIndexRange(side2,1)
                   n3a=gridIndexRange(0,2)-extra
                   n3b=gridIndexRange(1,2)+extra
                   bc1=boundaryCondition(side1,0)
                   bc2=boundaryCondition(side2,1)
                  else if( edgeDirection.eq.1 )then
                   is2=0
                   n1a=gridIndexRange(side1,0)
                   n1b=gridIndexRange(side1,0)
                   n2a=gridIndexRange(    0,1)-extra
                   n2b=gridIndexRange(    1,1)+extra
                   n3a=gridIndexRange(side3,2)
                   n3b=gridIndexRange(side3,2)
                   bc1=boundaryCondition(side1,0)
                   bc2=boundaryCondition(side3,2)
                  else
                   is1=0
                   n1a=gridIndexRange(    0,0)-extra
                   n1b=gridIndexRange(    1,0)+extra
                   n2a=gridIndexRange(side2,1)
                   n2b=gridIndexRange(side2,1)
                   n3a=gridIndexRange(side3,2)
                   n3b=gridIndexRange(side3,2)
                   bc1=boundaryCondition(side2,1)
                   bc2=boundaryCondition(side3,2)
                  end if
                  ! *********************************************************
                  ! ************* Assign Ghost near two faces ***************
                  ! *************       CURVILINEAR            **************
                  ! *********************************************************
                   ls1=is1  ! save for extrapolation
                   ls2=is2
                   ls3=is3
                   is1=0
                   is2=0
                   is3=0
                   js1=0
                   js2=0
                   js3=0
                   ks1=0
                   ks2=0
                   ks3=0
                   if( edgeDirection.eq.0 )then
                     axis=1
                     axisp1=2
                     axisp2=0
                     side1=0
                     side2=sidea
                     side3=sideb
                     is2=1-2*side2  ! normal direction 1
                     js3=1-2*side3  ! normal direction 2
                     ks1=1          ! tangential direction
                   else if( edgeDirection.eq.1 )then
                     axis=2
                     axisp1=0
                     axisp2=1
                     side1=sideb
                     side2=0
                     side3=sidea
                     is3=1-2*side3  ! normal direction 1
                     js1=1-2*side1  ! normal direction 2
                     ks2=1          ! tangential direction
                   else
                     axis=0
                     axisp1=1
                     axisp2=2
                     side1=sidea
                     side2=sideb
                     side3=0
                     is1=1-2*side1  ! normal direction 1
                     js2=1-2*side2  ! normal direction 2
                     ks3=1          ! tangential direction
                   end if
                   dra=dr(axis  )*(1-2*sidea)
                   dsa=dr(axisp1)*(1-2*sideb)
                   dta=dr(axisp2)
                   if( bc1.eq.perfectElectricalConductor .and. 
     & bc2.eq.perfectElectricalConductor )then
                    ! *********************************************************
                    ! ************* PEC EDGE BC (CURVILINEAR) *****************
                    ! *********************************************************
                     if( debug.gt.0 )then
                       write(*,'(/," corner-edge-4:Start edge=",i1," 
     & side1,side2,side3=",3i2," is=",3i3," js=",3i3," ks=",3i3)') 
     & edgeDirection,side1,side2,side3,is1,is2,is3,js1,js2,js3,ks1,
     & ks2,ks3
                       write(*,'("   dra,dsa,dta=",3f8.5)') dra,dsa,dta
                       ! '
                     end if
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                     ! Check the mask:  *wdh* 2015/06/24
                     if( mask(i1,i2,i3).gt.0 .and. 
     & i1.ge.gridIndexRange(0,0) .and. i1.le.gridIndexRange(1,0) 
     & .and. i2.ge.gridIndexRange(0,1) .and. i2.le.gridIndexRange(1,1)
     &  .and. i3.ge.gridIndexRange(0,2) .and. i3.le.gridIndexRange(1,
     & 2) )then
                       ! precompute the inverse of the jacobian, used in macros AmnD3J
                       i10=i1  ! used by jac3di in macros
                       i20=i2
                       i30=i3
                       do m3=-numberOfGhostPoints,numberOfGhostPoints
                       do m2=-numberOfGhostPoints,numberOfGhostPoints
                       do m1=-numberOfGhostPoints,numberOfGhostPoints
                        jac3di(m1,m2,m3)=1./(rx(i1+m1,i2+m2,i3+m3)*(sy(
     & i1+m1,i2+m2,i3+m3)*tz(i1+m1,i2+m2,i3+m3)-sz(i1+m1,i2+m2,i3+m3)*
     & ty(i1+m1,i2+m2,i3+m3))+ry(i1+m1,i2+m2,i3+m3)*(sz(i1+m1,i2+m2,
     & i3+m3)*tx(i1+m1,i2+m2,i3+m3)-sx(i1+m1,i2+m2,i3+m3)*tz(i1+m1,i2+
     & m2,i3+m3))+rz(i1+m1,i2+m2,i3+m3)*(sx(i1+m1,i2+m2,i3+m3)*ty(i1+
     & m1,i2+m2,i3+m3)-sy(i1+m1,i2+m2,i3+m3)*tx(i1+m1,i2+m2,i3+m3)))
                       end do
                       end do
                       end do
                       a11 =(rsxy(i1,i2,i3,axis,0)*jac3di(i1-i10,i2-
     & i20,i3-i30))
                       a12 =(rsxy(i1,i2,i3,axis,1)*jac3di(i1-i10,i2-
     & i20,i3-i30))
                       a13 =(rsxy(i1,i2,i3,axis,2)*jac3di(i1-i10,i2-
     & i20,i3-i30))
                       a21 =(rsxy(i1,i2,i3,axisp1,0)*jac3di(i1-i10,i2-
     & i20,i3-i30))
                       a22 =(rsxy(i1,i2,i3,axisp1,1)*jac3di(i1-i10,i2-
     & i20,i3-i30))
                       a23 =(rsxy(i1,i2,i3,axisp1,2)*jac3di(i1-i10,i2-
     & i20,i3-i30))
                       a31 =(rsxy(i1,i2,i3,axisp2,0)*jac3di(i1-i10,i2-
     & i20,i3-i30))
                       a32 =(rsxy(i1,i2,i3,axisp2,1)*jac3di(i1-i10,i2-
     & i20,i3-i30))
                       a33 =(rsxy(i1,i2,i3,axisp2,2)*jac3di(i1-i10,i2-
     & i20,i3-i30))
                       ! ************ Order 4 ******************
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
                       a21r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axisp1,0)
     & *jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))-(rsxy(i1-is1,i2-is2,
     & i3-is3,axisp1,0)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp1,0)*jac3di(i1+2*is1-i10,
     & i2+2*is2-i20,i3+2*is3-i30))-(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,
     & axisp1,0)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))/(
     & 12.*dra)
                       a22r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axisp1,1)
     & *jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))-(rsxy(i1-is1,i2-is2,
     & i3-is3,axisp1,1)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp1,1)*jac3di(i1+2*is1-i10,
     & i2+2*is2-i20,i3+2*is3-i30))-(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,
     & axisp1,1)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))/(
     & 12.*dra)
                       a23r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axisp1,2)
     & *jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))-(rsxy(i1-is1,i2-is2,
     & i3-is3,axisp1,2)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp1,2)*jac3di(i1+2*is1-i10,
     & i2+2*is2-i20,i3+2*is3-i30))-(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,
     & axisp1,2)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))/(
     & 12.*dra)
                       a31r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axisp2,0)
     & *jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))-(rsxy(i1-is1,i2-is2,
     & i3-is3,axisp2,0)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp2,0)*jac3di(i1+2*is1-i10,
     & i2+2*is2-i20,i3+2*is3-i30))-(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,
     & axisp2,0)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))/(
     & 12.*dra)
                       a32r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axisp2,1)
     & *jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))-(rsxy(i1-is1,i2-is2,
     & i3-is3,axisp2,1)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp2,1)*jac3di(i1+2*is1-i10,
     & i2+2*is2-i20,i3+2*is3-i30))-(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,
     & axisp2,1)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))/(
     & 12.*dra)
                       a33r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axisp2,2)
     & *jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))-(rsxy(i1-is1,i2-is2,
     & i3-is3,axisp2,2)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp2,2)*jac3di(i1+2*is1-i10,
     & i2+2*is2-i20,i3+2*is3-i30))-(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,
     & axisp2,2)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))/(
     & 12.*dra)
                       a11rr = ((-30.*(rsxy(i1,i2,i3,axis,0)*jac3di(i1-
     & i10,i2-i20,i3-i30))+16.*((rsxy(i1+is1,i2+is2,i3+is3,axis,0)*
     & jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))+(rsxy(i1-is1,i2-is2,
     & i3-is3,axis,0)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,0)*jac3di(i1+2*is1-i10,i2+
     & 2*is2-i20,i3+2*is3-i30))+(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,
     & 0)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))/(12.*dra**
     & 2))
                       a12rr = ((-30.*(rsxy(i1,i2,i3,axis,1)*jac3di(i1-
     & i10,i2-i20,i3-i30))+16.*((rsxy(i1+is1,i2+is2,i3+is3,axis,1)*
     & jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))+(rsxy(i1-is1,i2-is2,
     & i3-is3,axis,1)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,1)*jac3di(i1+2*is1-i10,i2+
     & 2*is2-i20,i3+2*is3-i30))+(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,
     & 1)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))/(12.*dra**
     & 2))
                       a13rr = ((-30.*(rsxy(i1,i2,i3,axis,2)*jac3di(i1-
     & i10,i2-i20,i3-i30))+16.*((rsxy(i1+is1,i2+is2,i3+is3,axis,2)*
     & jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))+(rsxy(i1-is1,i2-is2,
     & i3-is3,axis,2)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,2)*jac3di(i1+2*is1-i10,i2+
     & 2*is2-i20,i3+2*is3-i30))+(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,
     & 2)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))/(12.*dra**
     & 2))
                       a21rr = ((-30.*(rsxy(i1,i2,i3,axisp1,0)*jac3di(
     & i1-i10,i2-i20,i3-i30))+16.*((rsxy(i1+is1,i2+is2,i3+is3,axisp1,
     & 0)*jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))+(rsxy(i1-is1,i2-
     & is2,i3-is3,axisp1,0)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))
     & -((rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp1,0)*jac3di(i1+2*is1-
     & i10,i2+2*is2-i20,i3+2*is3-i30))+(rsxy(i1-2*is1,i2-2*is2,i3-2*
     & is3,axisp1,0)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))
     & /(12.*dra**2))
                       a22rr = ((-30.*(rsxy(i1,i2,i3,axisp1,1)*jac3di(
     & i1-i10,i2-i20,i3-i30))+16.*((rsxy(i1+is1,i2+is2,i3+is3,axisp1,
     & 1)*jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))+(rsxy(i1-is1,i2-
     & is2,i3-is3,axisp1,1)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))
     & -((rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp1,1)*jac3di(i1+2*is1-
     & i10,i2+2*is2-i20,i3+2*is3-i30))+(rsxy(i1-2*is1,i2-2*is2,i3-2*
     & is3,axisp1,1)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))
     & /(12.*dra**2))
                       a23rr = ((-30.*(rsxy(i1,i2,i3,axisp1,2)*jac3di(
     & i1-i10,i2-i20,i3-i30))+16.*((rsxy(i1+is1,i2+is2,i3+is3,axisp1,
     & 2)*jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))+(rsxy(i1-is1,i2-
     & is2,i3-is3,axisp1,2)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))
     & -((rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp1,2)*jac3di(i1+2*is1-
     & i10,i2+2*is2-i20,i3+2*is3-i30))+(rsxy(i1-2*is1,i2-2*is2,i3-2*
     & is3,axisp1,2)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))
     & /(12.*dra**2))
                       a31rr = ((-30.*(rsxy(i1,i2,i3,axisp2,0)*jac3di(
     & i1-i10,i2-i20,i3-i30))+16.*((rsxy(i1+is1,i2+is2,i3+is3,axisp2,
     & 0)*jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))+(rsxy(i1-is1,i2-
     & is2,i3-is3,axisp2,0)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))
     & -((rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp2,0)*jac3di(i1+2*is1-
     & i10,i2+2*is2-i20,i3+2*is3-i30))+(rsxy(i1-2*is1,i2-2*is2,i3-2*
     & is3,axisp2,0)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))
     & /(12.*dra**2))
                       a32rr = ((-30.*(rsxy(i1,i2,i3,axisp2,1)*jac3di(
     & i1-i10,i2-i20,i3-i30))+16.*((rsxy(i1+is1,i2+is2,i3+is3,axisp2,
     & 1)*jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))+(rsxy(i1-is1,i2-
     & is2,i3-is3,axisp2,1)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))
     & -((rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp2,1)*jac3di(i1+2*is1-
     & i10,i2+2*is2-i20,i3+2*is3-i30))+(rsxy(i1-2*is1,i2-2*is2,i3-2*
     & is3,axisp2,1)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))
     & /(12.*dra**2))
                       a33rr = ((-30.*(rsxy(i1,i2,i3,axisp2,2)*jac3di(
     & i1-i10,i2-i20,i3-i30))+16.*((rsxy(i1+is1,i2+is2,i3+is3,axisp2,
     & 2)*jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))+(rsxy(i1-is1,i2-
     & is2,i3-is3,axisp2,2)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))
     & -((rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp2,2)*jac3di(i1+2*is1-
     & i10,i2+2*is2-i20,i3+2*is3-i30))+(rsxy(i1-2*is1,i2-2*is2,i3-2*
     & is3,axisp2,2)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))
     & /(12.*dra**2))
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
                       a21s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axisp1,0)
     & *jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))-(rsxy(i1-js1,i2-js2,
     & i3-js3,axisp1,0)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,0)*jac3di(i1+2*js1-i10,
     & i2+2*js2-i20,i3+2*js3-i30))-(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,
     & axisp1,0)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(
     & 12.*dsa)
                       a22s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axisp1,1)
     & *jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))-(rsxy(i1-js1,i2-js2,
     & i3-js3,axisp1,1)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,1)*jac3di(i1+2*js1-i10,
     & i2+2*js2-i20,i3+2*js3-i30))-(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,
     & axisp1,1)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(
     & 12.*dsa)
                       a23s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axisp1,2)
     & *jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))-(rsxy(i1-js1,i2-js2,
     & i3-js3,axisp1,2)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,2)*jac3di(i1+2*js1-i10,
     & i2+2*js2-i20,i3+2*js3-i30))-(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,
     & axisp1,2)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(
     & 12.*dsa)
                       a31s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axisp2,0)
     & *jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))-(rsxy(i1-js1,i2-js2,
     & i3-js3,axisp2,0)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp2,0)*jac3di(i1+2*js1-i10,
     & i2+2*js2-i20,i3+2*js3-i30))-(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,
     & axisp2,0)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(
     & 12.*dsa)
                       a32s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axisp2,1)
     & *jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))-(rsxy(i1-js1,i2-js2,
     & i3-js3,axisp2,1)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp2,1)*jac3di(i1+2*js1-i10,
     & i2+2*js2-i20,i3+2*js3-i30))-(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,
     & axisp2,1)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(
     & 12.*dsa)
                       a33s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axisp2,2)
     & *jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))-(rsxy(i1-js1,i2-js2,
     & i3-js3,axisp2,2)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp2,2)*jac3di(i1+2*js1-i10,
     & i2+2*js2-i20,i3+2*js3-i30))-(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,
     & axisp2,2)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(
     & 12.*dsa)
                       a11ss = ((-30.*(rsxy(i1,i2,i3,axis,0)*jac3di(i1-
     & i10,i2-i20,i3-i30))+16.*((rsxy(i1+js1,i2+js2,i3+js3,axis,0)*
     & jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))+(rsxy(i1-js1,i2-js2,
     & i3-js3,axis,0)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axis,0)*jac3di(i1+2*js1-i10,i2+
     & 2*js2-i20,i3+2*js3-i30))+(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axis,
     & 0)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(12.*dsa**
     & 2))
                       a12ss = ((-30.*(rsxy(i1,i2,i3,axis,1)*jac3di(i1-
     & i10,i2-i20,i3-i30))+16.*((rsxy(i1+js1,i2+js2,i3+js3,axis,1)*
     & jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))+(rsxy(i1-js1,i2-js2,
     & i3-js3,axis,1)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axis,1)*jac3di(i1+2*js1-i10,i2+
     & 2*js2-i20,i3+2*js3-i30))+(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axis,
     & 1)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(12.*dsa**
     & 2))
                       a13ss = ((-30.*(rsxy(i1,i2,i3,axis,2)*jac3di(i1-
     & i10,i2-i20,i3-i30))+16.*((rsxy(i1+js1,i2+js2,i3+js3,axis,2)*
     & jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))+(rsxy(i1-js1,i2-js2,
     & i3-js3,axis,2)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axis,2)*jac3di(i1+2*js1-i10,i2+
     & 2*js2-i20,i3+2*js3-i30))+(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axis,
     & 2)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(12.*dsa**
     & 2))
                       a21ss = ((-30.*(rsxy(i1,i2,i3,axisp1,0)*jac3di(
     & i1-i10,i2-i20,i3-i30))+16.*((rsxy(i1+js1,i2+js2,i3+js3,axisp1,
     & 0)*jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))+(rsxy(i1-js1,i2-
     & js2,i3-js3,axisp1,0)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))
     & -((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,0)*jac3di(i1+2*js1-
     & i10,i2+2*js2-i20,i3+2*js3-i30))+(rsxy(i1-2*js1,i2-2*js2,i3-2*
     & js3,axisp1,0)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))
     & /(12.*dsa**2))
                       a22ss = ((-30.*(rsxy(i1,i2,i3,axisp1,1)*jac3di(
     & i1-i10,i2-i20,i3-i30))+16.*((rsxy(i1+js1,i2+js2,i3+js3,axisp1,
     & 1)*jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))+(rsxy(i1-js1,i2-
     & js2,i3-js3,axisp1,1)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))
     & -((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,1)*jac3di(i1+2*js1-
     & i10,i2+2*js2-i20,i3+2*js3-i30))+(rsxy(i1-2*js1,i2-2*js2,i3-2*
     & js3,axisp1,1)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))
     & /(12.*dsa**2))
                       a23ss = ((-30.*(rsxy(i1,i2,i3,axisp1,2)*jac3di(
     & i1-i10,i2-i20,i3-i30))+16.*((rsxy(i1+js1,i2+js2,i3+js3,axisp1,
     & 2)*jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))+(rsxy(i1-js1,i2-
     & js2,i3-js3,axisp1,2)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))
     & -((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,2)*jac3di(i1+2*js1-
     & i10,i2+2*js2-i20,i3+2*js3-i30))+(rsxy(i1-2*js1,i2-2*js2,i3-2*
     & js3,axisp1,2)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))
     & /(12.*dsa**2))
                       a31ss = ((-30.*(rsxy(i1,i2,i3,axisp2,0)*jac3di(
     & i1-i10,i2-i20,i3-i30))+16.*((rsxy(i1+js1,i2+js2,i3+js3,axisp2,
     & 0)*jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))+(rsxy(i1-js1,i2-
     & js2,i3-js3,axisp2,0)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))
     & -((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp2,0)*jac3di(i1+2*js1-
     & i10,i2+2*js2-i20,i3+2*js3-i30))+(rsxy(i1-2*js1,i2-2*js2,i3-2*
     & js3,axisp2,0)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))
     & /(12.*dsa**2))
                       a32ss = ((-30.*(rsxy(i1,i2,i3,axisp2,1)*jac3di(
     & i1-i10,i2-i20,i3-i30))+16.*((rsxy(i1+js1,i2+js2,i3+js3,axisp2,
     & 1)*jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))+(rsxy(i1-js1,i2-
     & js2,i3-js3,axisp2,1)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))
     & -((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp2,1)*jac3di(i1+2*js1-
     & i10,i2+2*js2-i20,i3+2*js3-i30))+(rsxy(i1-2*js1,i2-2*js2,i3-2*
     & js3,axisp2,1)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))
     & /(12.*dsa**2))
                       a33ss = ((-30.*(rsxy(i1,i2,i3,axisp2,2)*jac3di(
     & i1-i10,i2-i20,i3-i30))+16.*((rsxy(i1+js1,i2+js2,i3+js3,axisp2,
     & 2)*jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))+(rsxy(i1-js1,i2-
     & js2,i3-js3,axisp2,2)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))
     & -((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp2,2)*jac3di(i1+2*js1-
     & i10,i2+2*js2-i20,i3+2*js3-i30))+(rsxy(i1-2*js1,i2-2*js2,i3-2*
     & js3,axisp2,2)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))
     & /(12.*dsa**2))
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
                       a21t = (8.*((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp1,0)
     & *jac3di(i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))-(rsxy(i1-ks1,i2-ks2,
     & i3-ks3,axisp1,0)*jac3di(i1-ks1-i10,i2-ks2-i20,i3-ks3-i30)))-((
     & rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp1,0)*jac3di(i1+2*ks1-i10,
     & i2+2*ks2-i20,i3+2*ks3-i30))-(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,
     & axisp1,0)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))))/(
     & 12.*dta)
                       a22t = (8.*((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp1,1)
     & *jac3di(i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))-(rsxy(i1-ks1,i2-ks2,
     & i3-ks3,axisp1,1)*jac3di(i1-ks1-i10,i2-ks2-i20,i3-ks3-i30)))-((
     & rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp1,1)*jac3di(i1+2*ks1-i10,
     & i2+2*ks2-i20,i3+2*ks3-i30))-(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,
     & axisp1,1)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))))/(
     & 12.*dta)
                       a23t = (8.*((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp1,2)
     & *jac3di(i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))-(rsxy(i1-ks1,i2-ks2,
     & i3-ks3,axisp1,2)*jac3di(i1-ks1-i10,i2-ks2-i20,i3-ks3-i30)))-((
     & rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp1,2)*jac3di(i1+2*ks1-i10,
     & i2+2*ks2-i20,i3+2*ks3-i30))-(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,
     & axisp1,2)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))))/(
     & 12.*dta)
                       a31t = (8.*((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,0)
     & *jac3di(i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))-(rsxy(i1-ks1,i2-ks2,
     & i3-ks3,axisp2,0)*jac3di(i1-ks1-i10,i2-ks2-i20,i3-ks3-i30)))-((
     & rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp2,0)*jac3di(i1+2*ks1-i10,
     & i2+2*ks2-i20,i3+2*ks3-i30))-(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,
     & axisp2,0)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))))/(
     & 12.*dta)
                       a32t = (8.*((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,1)
     & *jac3di(i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))-(rsxy(i1-ks1,i2-ks2,
     & i3-ks3,axisp2,1)*jac3di(i1-ks1-i10,i2-ks2-i20,i3-ks3-i30)))-((
     & rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp2,1)*jac3di(i1+2*ks1-i10,
     & i2+2*ks2-i20,i3+2*ks3-i30))-(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,
     & axisp2,1)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))))/(
     & 12.*dta)
                       a33t = (8.*((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,2)
     & *jac3di(i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))-(rsxy(i1-ks1,i2-ks2,
     & i3-ks3,axisp2,2)*jac3di(i1-ks1-i10,i2-ks2-i20,i3-ks3-i30)))-((
     & rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp2,2)*jac3di(i1+2*ks1-i10,
     & i2+2*ks2-i20,i3+2*ks3-i30))-(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,
     & axisp2,2)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))))/(
     & 12.*dta)
                       c11 = (rsxy(i1,i2,i3,axis,0)**2+rsxy(i1,i2,i3,
     & axis,1)**2+rsxy(i1,i2,i3,axis,2)**2)
                       c22 = (rsxy(i1,i2,i3,axisp1,0)**2+rsxy(i1,i2,i3,
     & axisp1,1)**2+rsxy(i1,i2,i3,axisp1,2)**2)
                       c33 = (rsxy(i1,i2,i3,axisp2,0)**2+rsxy(i1,i2,i3,
     & axisp2,1)**2+rsxy(i1,i2,i3,axisp2,2)**2)
                       c1 = (rsxyx43(i1,i2,i3,axis,0)+rsxyy43(i1,i2,i3,
     & axis,1)+rsxyz43(i1,i2,i3,axis,2))
                       c2 = (rsxyx43(i1,i2,i3,axisp1,0)+rsxyy43(i1,i2,
     & i3,axisp1,1)+rsxyz43(i1,i2,i3,axisp1,2))
                       c3 = (rsxyx43(i1,i2,i3,axisp2,0)+rsxyy43(i1,i2,
     & i3,axisp2,1)+rsxyz43(i1,i2,i3,axisp2,2))
                       c11r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axis,0)**
     & 2+rsxy(i1+is1,i2+is2,i3+is3,axis,1)**2+rsxy(i1+is1,i2+is2,i3+
     & is3,axis,2)**2)-(rsxy(i1-is1,i2-is2,i3-is3,axis,0)**2+rsxy(i1-
     & is1,i2-is2,i3-is3,axis,1)**2+rsxy(i1-is1,i2-is2,i3-is3,axis,2)*
     & *2))   -((rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,0)**2+rsxy(i1+2*
     & is1,i2+2*is2,i3+2*is3,axis,1)**2+rsxy(i1+2*is1,i2+2*is2,i3+2*
     & is3,axis,2)**2)-(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,0)**2+
     & rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,1)**2+rsxy(i1-2*is1,i2-2*
     & is2,i3-2*is3,axis,2)**2))   )/(12.*dra)
                       c22r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axisp1,0)
     & **2+rsxy(i1+is1,i2+is2,i3+is3,axisp1,1)**2+rsxy(i1+is1,i2+is2,
     & i3+is3,axisp1,2)**2)-(rsxy(i1-is1,i2-is2,i3-is3,axisp1,0)**2+
     & rsxy(i1-is1,i2-is2,i3-is3,axisp1,1)**2+rsxy(i1-is1,i2-is2,i3-
     & is3,axisp1,2)**2))   -((rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp1,
     & 0)**2+rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp1,1)**2+rsxy(i1+2*
     & is1,i2+2*is2,i3+2*is3,axisp1,2)**2)-(rsxy(i1-2*is1,i2-2*is2,i3-
     & 2*is3,axisp1,0)**2+rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axisp1,1)**
     & 2+rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axisp1,2)**2))   )/(12.*dra)
                       c33r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axisp2,0)
     & **2+rsxy(i1+is1,i2+is2,i3+is3,axisp2,1)**2+rsxy(i1+is1,i2+is2,
     & i3+is3,axisp2,2)**2)-(rsxy(i1-is1,i2-is2,i3-is3,axisp2,0)**2+
     & rsxy(i1-is1,i2-is2,i3-is3,axisp2,1)**2+rsxy(i1-is1,i2-is2,i3-
     & is3,axisp2,2)**2))   -((rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp2,
     & 0)**2+rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp2,1)**2+rsxy(i1+2*
     & is1,i2+2*is2,i3+2*is3,axisp2,2)**2)-(rsxy(i1-2*is1,i2-2*is2,i3-
     & 2*is3,axisp2,0)**2+rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axisp2,1)**
     & 2+rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axisp2,2)**2))   )/(12.*dra)
                       c11s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axis,0)**
     & 2+rsxy(i1+js1,i2+js2,i3+js3,axis,1)**2+rsxy(i1+js1,i2+js2,i3+
     & js3,axis,2)**2)-(rsxy(i1-js1,i2-js2,i3-js3,axis,0)**2+rsxy(i1-
     & js1,i2-js2,i3-js3,axis,1)**2+rsxy(i1-js1,i2-js2,i3-js3,axis,2)*
     & *2))   -((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axis,0)**2+rsxy(i1+2*
     & js1,i2+2*js2,i3+2*js3,axis,1)**2+rsxy(i1+2*js1,i2+2*js2,i3+2*
     & js3,axis,2)**2)-(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axis,0)**2+
     & rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axis,1)**2+rsxy(i1-2*js1,i2-2*
     & js2,i3-2*js3,axis,2)**2))   )/(12.*dsa)
                       c22s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axisp1,0)
     & **2+rsxy(i1+js1,i2+js2,i3+js3,axisp1,1)**2+rsxy(i1+js1,i2+js2,
     & i3+js3,axisp1,2)**2)-(rsxy(i1-js1,i2-js2,i3-js3,axisp1,0)**2+
     & rsxy(i1-js1,i2-js2,i3-js3,axisp1,1)**2+rsxy(i1-js1,i2-js2,i3-
     & js3,axisp1,2)**2))   -((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,
     & 0)**2+rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,1)**2+rsxy(i1+2*
     & js1,i2+2*js2,i3+2*js3,axisp1,2)**2)-(rsxy(i1-2*js1,i2-2*js2,i3-
     & 2*js3,axisp1,0)**2+rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axisp1,1)**
     & 2+rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axisp1,2)**2))   )/(12.*dsa)
                       c33s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axisp2,0)
     & **2+rsxy(i1+js1,i2+js2,i3+js3,axisp2,1)**2+rsxy(i1+js1,i2+js2,
     & i3+js3,axisp2,2)**2)-(rsxy(i1-js1,i2-js2,i3-js3,axisp2,0)**2+
     & rsxy(i1-js1,i2-js2,i3-js3,axisp2,1)**2+rsxy(i1-js1,i2-js2,i3-
     & js3,axisp2,2)**2))   -((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp2,
     & 0)**2+rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp2,1)**2+rsxy(i1+2*
     & js1,i2+2*js2,i3+2*js3,axisp2,2)**2)-(rsxy(i1-2*js1,i2-2*js2,i3-
     & 2*js3,axisp2,0)**2+rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axisp2,1)**
     & 2+rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axisp2,2)**2))   )/(12.*dsa)
                       if( axis.eq.0 )then
                         c1r = (rsxyxr43(i1,i2,i3,axis,0)+rsxyyr43(i1,
     & i2,i3,axis,1)+rsxyzr43(i1,i2,i3,axis,2))
                         c2r = (rsxyxr43(i1,i2,i3,axisp1,0)+rsxyyr43(
     & i1,i2,i3,axisp1,1)+rsxyzr43(i1,i2,i3,axisp1,2))
                         c3r = (rsxyxr43(i1,i2,i3,axisp2,0)+rsxyyr43(
     & i1,i2,i3,axisp2,1)+rsxyzr43(i1,i2,i3,axisp2,2))
                         c1s = (rsxyxs43(i1,i2,i3,axis,0)+rsxyys43(i1,
     & i2,i3,axis,1)+rsxyzs43(i1,i2,i3,axis,2))
                         c2s = (rsxyxs43(i1,i2,i3,axisp1,0)+rsxyys43(
     & i1,i2,i3,axisp1,1)+rsxyzs43(i1,i2,i3,axisp1,2))
                         c3s = (rsxyxs43(i1,i2,i3,axisp2,0)+rsxyys43(
     & i1,i2,i3,axisp2,1)+rsxyzs43(i1,i2,i3,axisp2,2))
                       else if( axis.eq.1 )then
                         c1r = (rsxyxs43(i1,i2,i3,axis,0)+rsxyys43(i1,
     & i2,i3,axis,1)+rsxyzs43(i1,i2,i3,axis,2))
                         c2r = (rsxyxs43(i1,i2,i3,axisp1,0)+rsxyys43(
     & i1,i2,i3,axisp1,1)+rsxyzs43(i1,i2,i3,axisp1,2))
                         c3r = (rsxyxs43(i1,i2,i3,axisp2,0)+rsxyys43(
     & i1,i2,i3,axisp2,1)+rsxyzs43(i1,i2,i3,axisp2,2))
                         c1s = (rsxyxt43(i1,i2,i3,axis,0)+rsxyyt43(i1,
     & i2,i3,axis,1)+rsxyzt43(i1,i2,i3,axis,2))
                         c2s = (rsxyxt43(i1,i2,i3,axisp1,0)+rsxyyt43(
     & i1,i2,i3,axisp1,1)+rsxyzt43(i1,i2,i3,axisp1,2))
                         c3s = (rsxyxt43(i1,i2,i3,axisp2,0)+rsxyyt43(
     & i1,i2,i3,axisp2,1)+rsxyzt43(i1,i2,i3,axisp2,2))
                       else
                         c1r = (rsxyxt43(i1,i2,i3,axis,0)+rsxyyt43(i1,
     & i2,i3,axis,1)+rsxyzt43(i1,i2,i3,axis,2))
                         c2r = (rsxyxt43(i1,i2,i3,axisp1,0)+rsxyyt43(
     & i1,i2,i3,axisp1,1)+rsxyzt43(i1,i2,i3,axisp1,2))
                         c3r = (rsxyxt43(i1,i2,i3,axisp2,0)+rsxyyt43(
     & i1,i2,i3,axisp2,1)+rsxyzt43(i1,i2,i3,axisp2,2))
                         c1s = (rsxyxr43(i1,i2,i3,axis,0)+rsxyyr43(i1,
     & i2,i3,axis,1)+rsxyzr43(i1,i2,i3,axis,2))
                         c2s = (rsxyxr43(i1,i2,i3,axisp1,0)+rsxyyr43(
     & i1,i2,i3,axisp1,1)+rsxyzr43(i1,i2,i3,axisp1,2))
                         c3s = (rsxyxr43(i1,i2,i3,axisp2,0)+rsxyyr43(
     & i1,i2,i3,axisp2,1)+rsxyzr43(i1,i2,i3,axisp2,2))
                       end if
                       ! ************ Order 4 ******************
                       ur=(8.*(u(i1+is1,i2+is2,i3+is3,ex)-u(i1-is1,i2-
     & is2,i3-is3,ex))-(u(i1+2*is1,i2+2*is2,i3+2*is3,ex)-u(i1-2*is1,
     & i2-2*is2,i3-2*is3,ex)))/(12.*dra)
                       urr=(-30.*u(i1,i2,i3,ex)+16.*(u(i1+is1,i2+is2,
     & i3+is3,ex)+u(i1-is1,i2-is2,i3-is3,ex))-(u(i1+2*is1,i2+2*is2,i3+
     & 2*is3,ex)+u(i1-2*is1,i2-2*is2,i3-2*is3,ex)))/(12.*dra**2)
                       urrr=(u(i1+2*is1,i2+2*is2,i3+2*is3,ex)-2.*u(i1+
     & is1,i2+is2,i3+is3,ex)+2.*u(i1-is1,i2-is2,i3-is3,ex)-u(i1-2*is1,
     & i2-2*is2,i3-2*is3,ex))/(2.*dra**3)
                       vr=(8.*(u(i1+is1,i2+is2,i3+is3,ey)-u(i1-is1,i2-
     & is2,i3-is3,ey))-(u(i1+2*is1,i2+2*is2,i3+2*is3,ey)-u(i1-2*is1,
     & i2-2*is2,i3-2*is3,ey)))/(12.*dra)
                       vrr=(-30.*u(i1,i2,i3,ey)+16.*(u(i1+is1,i2+is2,
     & i3+is3,ey)+u(i1-is1,i2-is2,i3-is3,ey))-(u(i1+2*is1,i2+2*is2,i3+
     & 2*is3,ey)+u(i1-2*is1,i2-2*is2,i3-2*is3,ey)))/(12.*dra**2)
                       vrrr=(u(i1+2*is1,i2+2*is2,i3+2*is3,ey)-2.*u(i1+
     & is1,i2+is2,i3+is3,ey)+2.*u(i1-is1,i2-is2,i3-is3,ey)-u(i1-2*is1,
     & i2-2*is2,i3-2*is3,ey))/(2.*dra**3)
                       wr=(8.*(u(i1+is1,i2+is2,i3+is3,ez)-u(i1-is1,i2-
     & is2,i3-is3,ez))-(u(i1+2*is1,i2+2*is2,i3+2*is3,ez)-u(i1-2*is1,
     & i2-2*is2,i3-2*is3,ez)))/(12.*dra)
                       wrr=(-30.*u(i1,i2,i3,ez)+16.*(u(i1+is1,i2+is2,
     & i3+is3,ez)+u(i1-is1,i2-is2,i3-is3,ez))-(u(i1+2*is1,i2+2*is2,i3+
     & 2*is3,ez)+u(i1-2*is1,i2-2*is2,i3-2*is3,ez)))/(12.*dra**2)
                       wrrr=(u(i1+2*is1,i2+2*is2,i3+2*is3,ez)-2.*u(i1+
     & is1,i2+is2,i3+is3,ez)+2.*u(i1-is1,i2-is2,i3-is3,ez)-u(i1-2*is1,
     & i2-2*is2,i3-2*is3,ez))/(2.*dra**3)
                       us=(8.*(u(i1+js1,i2+js2,i3+js3,ex)-u(i1-js1,i2-
     & js2,i3-js3,ex))-(u(i1+2*js1,i2+2*js2,i3+2*js3,ex)-u(i1-2*js1,
     & i2-2*js2,i3-2*js3,ex)))/(12.*dsa)
                       uss=(-30.*u(i1,i2,i3,ex)+16.*(u(i1+js1,i2+js2,
     & i3+js3,ex)+u(i1-js1,i2-js2,i3-js3,ex))-(u(i1+2*js1,i2+2*js2,i3+
     & 2*js3,ex)+u(i1-2*js1,i2-2*js2,i3-2*js3,ex)))/(12.*dsa**2)
                       usss=(u(i1+2*js1,i2+2*js2,i3+2*js3,ex)-2.*u(i1+
     & js1,i2+js2,i3+js3,ex)+2.*u(i1-js1,i2-js2,i3-js3,ex)-u(i1-2*js1,
     & i2-2*js2,i3-2*js3,ex))/(2.*dsa**3)
                       vs=(8.*(u(i1+js1,i2+js2,i3+js3,ey)-u(i1-js1,i2-
     & js2,i3-js3,ey))-(u(i1+2*js1,i2+2*js2,i3+2*js3,ey)-u(i1-2*js1,
     & i2-2*js2,i3-2*js3,ey)))/(12.*dsa)
                       vss=(-30.*u(i1,i2,i3,ey)+16.*(u(i1+js1,i2+js2,
     & i3+js3,ey)+u(i1-js1,i2-js2,i3-js3,ey))-(u(i1+2*js1,i2+2*js2,i3+
     & 2*js3,ey)+u(i1-2*js1,i2-2*js2,i3-2*js3,ey)))/(12.*dsa**2)
                       vsss=(u(i1+2*js1,i2+2*js2,i3+2*js3,ey)-2.*u(i1+
     & js1,i2+js2,i3+js3,ey)+2.*u(i1-js1,i2-js2,i3-js3,ey)-u(i1-2*js1,
     & i2-2*js2,i3-2*js3,ey))/(2.*dsa**3)
                       ws=(8.*(u(i1+js1,i2+js2,i3+js3,ez)-u(i1-js1,i2-
     & js2,i3-js3,ez))-(u(i1+2*js1,i2+2*js2,i3+2*js3,ez)-u(i1-2*js1,
     & i2-2*js2,i3-2*js3,ez)))/(12.*dsa)
                       wss=(-30.*u(i1,i2,i3,ez)+16.*(u(i1+js1,i2+js2,
     & i3+js3,ez)+u(i1-js1,i2-js2,i3-js3,ez))-(u(i1+2*js1,i2+2*js2,i3+
     & 2*js3,ez)+u(i1-2*js1,i2-2*js2,i3-2*js3,ez)))/(12.*dsa**2)
                       wsss=(u(i1+2*js1,i2+2*js2,i3+2*js3,ez)-2.*u(i1+
     & js1,i2+js2,i3+js3,ez)+2.*u(i1-js1,i2-js2,i3-js3,ez)-u(i1-2*js1,
     & i2-2*js2,i3-2*js3,ez))/(2.*dsa**3)
                       ut=(8.*(u(i1+ks1,i2+ks2,i3+ks3,ex)-u(i1-ks1,i2-
     & ks2,i3-ks3,ex))-(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ex)-u(i1-2*ks1,
     & i2-2*ks2,i3-2*ks3,ex)))/(12.*dta)
                       utt=(-30.*u(i1,i2,i3,ex)+16.*(u(i1+ks1,i2+ks2,
     & i3+ks3,ex)+u(i1-ks1,i2-ks2,i3-ks3,ex))-(u(i1+2*ks1,i2+2*ks2,i3+
     & 2*ks3,ex)+u(i1-2*ks1,i2-2*ks2,i3-2*ks3,ex)))/(12.*dta**2)
                       uttt=(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ex)-2.*u(i1+
     & ks1,i2+ks2,i3+ks3,ex)+2.*u(i1-ks1,i2-ks2,i3-ks3,ex)-u(i1-2*ks1,
     & i2-2*ks2,i3-2*ks3,ex))/(2.*dta**3)
                       vt=(8.*(u(i1+ks1,i2+ks2,i3+ks3,ey)-u(i1-ks1,i2-
     & ks2,i3-ks3,ey))-(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ey)-u(i1-2*ks1,
     & i2-2*ks2,i3-2*ks3,ey)))/(12.*dta)
                       vtt=(-30.*u(i1,i2,i3,ey)+16.*(u(i1+ks1,i2+ks2,
     & i3+ks3,ey)+u(i1-ks1,i2-ks2,i3-ks3,ey))-(u(i1+2*ks1,i2+2*ks2,i3+
     & 2*ks3,ey)+u(i1-2*ks1,i2-2*ks2,i3-2*ks3,ey)))/(12.*dta**2)
                       vttt=(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ey)-2.*u(i1+
     & ks1,i2+ks2,i3+ks3,ey)+2.*u(i1-ks1,i2-ks2,i3-ks3,ey)-u(i1-2*ks1,
     & i2-2*ks2,i3-2*ks3,ey))/(2.*dta**3)
                       wt=(8.*(u(i1+ks1,i2+ks2,i3+ks3,ez)-u(i1-ks1,i2-
     & ks2,i3-ks3,ez))-(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ez)-u(i1-2*ks1,
     & i2-2*ks2,i3-2*ks3,ez)))/(12.*dta)
                       wtt=(-30.*u(i1,i2,i3,ez)+16.*(u(i1+ks1,i2+ks2,
     & i3+ks3,ez)+u(i1-ks1,i2-ks2,i3-ks3,ez))-(u(i1+2*ks1,i2+2*ks2,i3+
     & 2*ks3,ez)+u(i1-2*ks1,i2-2*ks2,i3-2*ks3,ez)))/(12.*dta**2)
                       wttt=(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ez)-2.*u(i1+
     & ks1,i2+ks2,i3+ks3,ez)+2.*u(i1-ks1,i2-ks2,i3-ks3,ez)-u(i1-2*ks1,
     & i2-2*ks2,i3-2*ks3,ez))/(2.*dta**3)
                       if( edgeDirection.eq.0 )then
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
                          a21rs = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp1,
     & 0)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp1,
     & 0)*jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,
     & axisp1,0)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,
     & axisp1,0)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*(
     & (rsxy(i1,i2+1,i3-1,axisp1,0)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-
     & (rsxy(i1,i2-1,i3-1,axisp1,0)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))
     & -((rsxy(i1,i2+2,i3-1,axisp1,0)*jac3di(i1-i10,i2+2-i20,i3-1-i30)
     & )-(rsxy(i1,i2-2,i3-1,axisp1,0)*jac3di(i1-i10,i2-2-i20,i3-1-i30)
     & )))/(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp1,0)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp1,0)*jac3di(i1-
     & i10,i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp1,0)*jac3di(
     & i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp1,0)*jac3di(
     & i1-i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-
     & 2,axisp1,0)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-
     & 2,axisp1,0)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,
     & i3-2,axisp1,0)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp1,0)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1)))
     & )/(12.*dr(2))
                          a22rs = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp1,
     & 1)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp1,
     & 1)*jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,
     & axisp1,1)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,
     & axisp1,1)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*(
     & (rsxy(i1,i2+1,i3-1,axisp1,1)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-
     & (rsxy(i1,i2-1,i3-1,axisp1,1)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))
     & -((rsxy(i1,i2+2,i3-1,axisp1,1)*jac3di(i1-i10,i2+2-i20,i3-1-i30)
     & )-(rsxy(i1,i2-2,i3-1,axisp1,1)*jac3di(i1-i10,i2-2-i20,i3-1-i30)
     & )))/(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp1,1)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp1,1)*jac3di(i1-
     & i10,i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp1,1)*jac3di(
     & i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp1,1)*jac3di(
     & i1-i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-
     & 2,axisp1,1)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-
     & 2,axisp1,1)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,
     & i3-2,axisp1,1)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp1,1)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1)))
     & )/(12.*dr(2))
                          a23rs = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp1,
     & 2)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp1,
     & 2)*jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,
     & axisp1,2)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,
     & axisp1,2)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*(
     & (rsxy(i1,i2+1,i3-1,axisp1,2)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-
     & (rsxy(i1,i2-1,i3-1,axisp1,2)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))
     & -((rsxy(i1,i2+2,i3-1,axisp1,2)*jac3di(i1-i10,i2+2-i20,i3-1-i30)
     & )-(rsxy(i1,i2-2,i3-1,axisp1,2)*jac3di(i1-i10,i2-2-i20,i3-1-i30)
     & )))/(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp1,2)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp1,2)*jac3di(i1-
     & i10,i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp1,2)*jac3di(
     & i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp1,2)*jac3di(
     & i1-i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-
     & 2,axisp1,2)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-
     & 2,axisp1,2)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,
     & i3-2,axisp1,2)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp1,2)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1)))
     & )/(12.*dr(2))
                          a31rs = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp2,
     & 0)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp2,
     & 0)*jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,
     & axisp2,0)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,
     & axisp2,0)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*(
     & (rsxy(i1,i2+1,i3-1,axisp2,0)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-
     & (rsxy(i1,i2-1,i3-1,axisp2,0)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))
     & -((rsxy(i1,i2+2,i3-1,axisp2,0)*jac3di(i1-i10,i2+2-i20,i3-1-i30)
     & )-(rsxy(i1,i2-2,i3-1,axisp2,0)*jac3di(i1-i10,i2-2-i20,i3-1-i30)
     & )))/(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp2,0)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp2,0)*jac3di(i1-
     & i10,i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp2,0)*jac3di(
     & i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp2,0)*jac3di(
     & i1-i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-
     & 2,axisp2,0)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-
     & 2,axisp2,0)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,
     & i3-2,axisp2,0)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp2,0)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1)))
     & )/(12.*dr(2))
                          a32rs = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp2,
     & 1)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp2,
     & 1)*jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,
     & axisp2,1)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,
     & axisp2,1)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*(
     & (rsxy(i1,i2+1,i3-1,axisp2,1)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-
     & (rsxy(i1,i2-1,i3-1,axisp2,1)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))
     & -((rsxy(i1,i2+2,i3-1,axisp2,1)*jac3di(i1-i10,i2+2-i20,i3-1-i30)
     & )-(rsxy(i1,i2-2,i3-1,axisp2,1)*jac3di(i1-i10,i2-2-i20,i3-1-i30)
     & )))/(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp2,1)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp2,1)*jac3di(i1-
     & i10,i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp2,1)*jac3di(
     & i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp2,1)*jac3di(
     & i1-i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-
     & 2,axisp2,1)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-
     & 2,axisp2,1)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,
     & i3-2,axisp2,1)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp2,1)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1)))
     & )/(12.*dr(2))
                          a33rs = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp2,
     & 2)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp2,
     & 2)*jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,
     & axisp2,2)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,
     & axisp2,2)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*(
     & (rsxy(i1,i2+1,i3-1,axisp2,2)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-
     & (rsxy(i1,i2-1,i3-1,axisp2,2)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))
     & -((rsxy(i1,i2+2,i3-1,axisp2,2)*jac3di(i1-i10,i2+2-i20,i3-1-i30)
     & )-(rsxy(i1,i2-2,i3-1,axisp2,2)*jac3di(i1-i10,i2-2-i20,i3-1-i30)
     & )))/(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp2,2)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp2,2)*jac3di(i1-
     & i10,i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp2,2)*jac3di(
     & i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp2,2)*jac3di(
     & i1-i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-
     & 2,axisp2,2)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-
     & 2,axisp2,2)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,
     & i3-2,axisp2,2)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp2,2)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1)))
     & )/(12.*dr(2))
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
                          a21rt = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp1,
     & 0)*jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp1,
     & 0)*jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,
     & axisp1,0)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,
     & axisp1,0)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2-1,i3,axisp1,0)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-
     & (rsxy(i1-1,i2-1,i3,axisp1,0)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))
     & -((rsxy(i1+2,i2-1,i3,axisp1,0)*jac3di(i1+2-i10,i2-1-i20,i3-i30)
     & )-(rsxy(i1-2,i2-1,i3,axisp1,0)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp1,0)*jac3di(i1+
     & 1-i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp1,0)*jac3di(i1-
     & 1-i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp1,0)*jac3di(
     & i1+2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp1,0)*jac3di(
     & i1-2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,
     & i3,axisp1,0)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,
     & i3,axisp1,0)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-
     & 2,i3,axisp1,0)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-
     & 2,i3,axisp1,0)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0)))
     & )/(12.*dr(1))
                          a22rt = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp1,
     & 1)*jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp1,
     & 1)*jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,
     & axisp1,1)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,
     & axisp1,1)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2-1,i3,axisp1,1)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-
     & (rsxy(i1-1,i2-1,i3,axisp1,1)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))
     & -((rsxy(i1+2,i2-1,i3,axisp1,1)*jac3di(i1+2-i10,i2-1-i20,i3-i30)
     & )-(rsxy(i1-2,i2-1,i3,axisp1,1)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp1,1)*jac3di(i1+
     & 1-i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp1,1)*jac3di(i1-
     & 1-i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp1,1)*jac3di(
     & i1+2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp1,1)*jac3di(
     & i1-2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,
     & i3,axisp1,1)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,
     & i3,axisp1,1)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-
     & 2,i3,axisp1,1)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-
     & 2,i3,axisp1,1)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0)))
     & )/(12.*dr(1))
                          a23rt = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp1,
     & 2)*jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp1,
     & 2)*jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,
     & axisp1,2)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,
     & axisp1,2)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2-1,i3,axisp1,2)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-
     & (rsxy(i1-1,i2-1,i3,axisp1,2)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))
     & -((rsxy(i1+2,i2-1,i3,axisp1,2)*jac3di(i1+2-i10,i2-1-i20,i3-i30)
     & )-(rsxy(i1-2,i2-1,i3,axisp1,2)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp1,2)*jac3di(i1+
     & 1-i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp1,2)*jac3di(i1-
     & 1-i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp1,2)*jac3di(
     & i1+2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp1,2)*jac3di(
     & i1-2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,
     & i3,axisp1,2)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,
     & i3,axisp1,2)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-
     & 2,i3,axisp1,2)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-
     & 2,i3,axisp1,2)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0)))
     & )/(12.*dr(1))
                          a31rt = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp2,
     & 0)*jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp2,
     & 0)*jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,
     & axisp2,0)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,
     & axisp2,0)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2-1,i3,axisp2,0)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-
     & (rsxy(i1-1,i2-1,i3,axisp2,0)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))
     & -((rsxy(i1+2,i2-1,i3,axisp2,0)*jac3di(i1+2-i10,i2-1-i20,i3-i30)
     & )-(rsxy(i1-2,i2-1,i3,axisp2,0)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp2,0)*jac3di(i1+
     & 1-i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp2,0)*jac3di(i1-
     & 1-i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp2,0)*jac3di(
     & i1+2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp2,0)*jac3di(
     & i1-2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,
     & i3,axisp2,0)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,
     & i3,axisp2,0)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-
     & 2,i3,axisp2,0)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-
     & 2,i3,axisp2,0)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0)))
     & )/(12.*dr(1))
                          a32rt = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp2,
     & 1)*jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp2,
     & 1)*jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,
     & axisp2,1)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,
     & axisp2,1)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2-1,i3,axisp2,1)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-
     & (rsxy(i1-1,i2-1,i3,axisp2,1)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))
     & -((rsxy(i1+2,i2-1,i3,axisp2,1)*jac3di(i1+2-i10,i2-1-i20,i3-i30)
     & )-(rsxy(i1-2,i2-1,i3,axisp2,1)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp2,1)*jac3di(i1+
     & 1-i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp2,1)*jac3di(i1-
     & 1-i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp2,1)*jac3di(
     & i1+2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp2,1)*jac3di(
     & i1-2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,
     & i3,axisp2,1)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,
     & i3,axisp2,1)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-
     & 2,i3,axisp2,1)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-
     & 2,i3,axisp2,1)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0)))
     & )/(12.*dr(1))
                          a33rt = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp2,
     & 2)*jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp2,
     & 2)*jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,
     & axisp2,2)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,
     & axisp2,2)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2-1,i3,axisp2,2)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-
     & (rsxy(i1-1,i2-1,i3,axisp2,2)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))
     & -((rsxy(i1+2,i2-1,i3,axisp2,2)*jac3di(i1+2-i10,i2-1-i20,i3-i30)
     & )-(rsxy(i1-2,i2-1,i3,axisp2,2)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp2,2)*jac3di(i1+
     & 1-i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp2,2)*jac3di(i1-
     & 1-i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp2,2)*jac3di(
     & i1+2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp2,2)*jac3di(
     & i1-2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,
     & i3,axisp2,2)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,
     & i3,axisp2,2)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-
     & 2,i3,axisp2,2)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-
     & 2,i3,axisp2,2)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0)))
     & )/(12.*dr(1))
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
                          a21st = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp1,
     & 0)*jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp1,
     & 0)*jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,
     & axisp1,0)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,
     & axisp1,0)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2,i3-1,axisp1,0)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-
     & (rsxy(i1-1,i2,i3-1,axisp1,0)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))
     & -((rsxy(i1+2,i2,i3-1,axisp1,0)*jac3di(i1+2-i10,i2-i20,i3-1-i30)
     & )-(rsxy(i1-2,i2,i3-1,axisp1,0)*jac3di(i1-2-i10,i2-i20,i3-1-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp1,0)*jac3di(i1+
     & 1-i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp1,0)*jac3di(i1-
     & 1-i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp1,0)*jac3di(
     & i1+2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp1,0)*jac3di(
     & i1-2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-
     & 2,axisp1,0)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-
     & 2,axisp1,0)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,
     & i3-2,axisp1,0)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,
     & i3-2,axisp1,0)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0)))
     & )/(12.*dr(2))
                          a22st = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp1,
     & 1)*jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp1,
     & 1)*jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,
     & axisp1,1)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,
     & axisp1,1)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2,i3-1,axisp1,1)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-
     & (rsxy(i1-1,i2,i3-1,axisp1,1)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))
     & -((rsxy(i1+2,i2,i3-1,axisp1,1)*jac3di(i1+2-i10,i2-i20,i3-1-i30)
     & )-(rsxy(i1-2,i2,i3-1,axisp1,1)*jac3di(i1-2-i10,i2-i20,i3-1-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp1,1)*jac3di(i1+
     & 1-i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp1,1)*jac3di(i1-
     & 1-i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp1,1)*jac3di(
     & i1+2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp1,1)*jac3di(
     & i1-2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-
     & 2,axisp1,1)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-
     & 2,axisp1,1)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,
     & i3-2,axisp1,1)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,
     & i3-2,axisp1,1)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0)))
     & )/(12.*dr(2))
                          a23st = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp1,
     & 2)*jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp1,
     & 2)*jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,
     & axisp1,2)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,
     & axisp1,2)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2,i3-1,axisp1,2)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-
     & (rsxy(i1-1,i2,i3-1,axisp1,2)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))
     & -((rsxy(i1+2,i2,i3-1,axisp1,2)*jac3di(i1+2-i10,i2-i20,i3-1-i30)
     & )-(rsxy(i1-2,i2,i3-1,axisp1,2)*jac3di(i1-2-i10,i2-i20,i3-1-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp1,2)*jac3di(i1+
     & 1-i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp1,2)*jac3di(i1-
     & 1-i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp1,2)*jac3di(
     & i1+2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp1,2)*jac3di(
     & i1-2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-
     & 2,axisp1,2)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-
     & 2,axisp1,2)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,
     & i3-2,axisp1,2)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,
     & i3-2,axisp1,2)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0)))
     & )/(12.*dr(2))
                          a31st = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp2,
     & 0)*jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp2,
     & 0)*jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,
     & axisp2,0)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,
     & axisp2,0)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2,i3-1,axisp2,0)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-
     & (rsxy(i1-1,i2,i3-1,axisp2,0)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))
     & -((rsxy(i1+2,i2,i3-1,axisp2,0)*jac3di(i1+2-i10,i2-i20,i3-1-i30)
     & )-(rsxy(i1-2,i2,i3-1,axisp2,0)*jac3di(i1-2-i10,i2-i20,i3-1-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp2,0)*jac3di(i1+
     & 1-i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp2,0)*jac3di(i1-
     & 1-i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp2,0)*jac3di(
     & i1+2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp2,0)*jac3di(
     & i1-2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-
     & 2,axisp2,0)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-
     & 2,axisp2,0)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,
     & i3-2,axisp2,0)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,
     & i3-2,axisp2,0)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0)))
     & )/(12.*dr(2))
                          a32st = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp2,
     & 1)*jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp2,
     & 1)*jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,
     & axisp2,1)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,
     & axisp2,1)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2,i3-1,axisp2,1)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-
     & (rsxy(i1-1,i2,i3-1,axisp2,1)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))
     & -((rsxy(i1+2,i2,i3-1,axisp2,1)*jac3di(i1+2-i10,i2-i20,i3-1-i30)
     & )-(rsxy(i1-2,i2,i3-1,axisp2,1)*jac3di(i1-2-i10,i2-i20,i3-1-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp2,1)*jac3di(i1+
     & 1-i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp2,1)*jac3di(i1-
     & 1-i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp2,1)*jac3di(
     & i1+2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp2,1)*jac3di(
     & i1-2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-
     & 2,axisp2,1)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-
     & 2,axisp2,1)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,
     & i3-2,axisp2,1)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,
     & i3-2,axisp2,1)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0)))
     & )/(12.*dr(2))
                          a33st = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp2,
     & 2)*jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp2,
     & 2)*jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,
     & axisp2,2)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,
     & axisp2,2)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2,i3-1,axisp2,2)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-
     & (rsxy(i1-1,i2,i3-1,axisp2,2)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))
     & -((rsxy(i1+2,i2,i3-1,axisp2,2)*jac3di(i1+2-i10,i2-i20,i3-1-i30)
     & )-(rsxy(i1-2,i2,i3-1,axisp2,2)*jac3di(i1-2-i10,i2-i20,i3-1-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp2,2)*jac3di(i1+
     & 1-i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp2,2)*jac3di(i1-
     & 1-i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp2,2)*jac3di(
     & i1+2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp2,2)*jac3di(
     & i1-2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-
     & 2,axisp2,2)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-
     & 2,axisp2,2)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,
     & i3-2,axisp2,2)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,
     & i3-2,axisp2,2)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0)))
     & )/(12.*dr(2))
                          urt=urs4(i1,i2,i3,ex)
                          ust=urt4(i1,i2,i3,ex)
                          urtt=urrs2(i1,i2,i3,ex)
                          ustt=urrt2(i1,i2,i3,ex)
                          vrt  =urs4(i1,i2,i3,ey)
                          vst  =urt4(i1,i2,i3,ey)
                          vrtt=urrs2(i1,i2,i3,ey)
                          vstt=urrt2(i1,i2,i3,ey)
                          wrt  =urs4(i1,i2,i3,ez)
                          wst  =urt4(i1,i2,i3,ez)
                          wrtt=urrs2(i1,i2,i3,ez)
                          wstt=urrt2(i1,i2,i3,ez)
                       else if( edgeDirection.eq.1 )then
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
                          a21rs = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp1,
     & 0)*jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp1,
     & 0)*jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,
     & axisp1,0)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,
     & axisp1,0)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2,i3-1,axisp1,0)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-
     & (rsxy(i1-1,i2,i3-1,axisp1,0)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))
     & -((rsxy(i1+2,i2,i3-1,axisp1,0)*jac3di(i1+2-i10,i2-i20,i3-1-i30)
     & )-(rsxy(i1-2,i2,i3-1,axisp1,0)*jac3di(i1-2-i10,i2-i20,i3-1-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp1,0)*jac3di(i1+
     & 1-i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp1,0)*jac3di(i1-
     & 1-i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp1,0)*jac3di(
     & i1+2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp1,0)*jac3di(
     & i1-2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-
     & 2,axisp1,0)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-
     & 2,axisp1,0)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,
     & i3-2,axisp1,0)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,
     & i3-2,axisp1,0)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0)))
     & )/(12.*dr(2))
                          a22rs = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp1,
     & 1)*jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp1,
     & 1)*jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,
     & axisp1,1)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,
     & axisp1,1)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2,i3-1,axisp1,1)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-
     & (rsxy(i1-1,i2,i3-1,axisp1,1)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))
     & -((rsxy(i1+2,i2,i3-1,axisp1,1)*jac3di(i1+2-i10,i2-i20,i3-1-i30)
     & )-(rsxy(i1-2,i2,i3-1,axisp1,1)*jac3di(i1-2-i10,i2-i20,i3-1-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp1,1)*jac3di(i1+
     & 1-i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp1,1)*jac3di(i1-
     & 1-i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp1,1)*jac3di(
     & i1+2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp1,1)*jac3di(
     & i1-2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-
     & 2,axisp1,1)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-
     & 2,axisp1,1)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,
     & i3-2,axisp1,1)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,
     & i3-2,axisp1,1)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0)))
     & )/(12.*dr(2))
                          a23rs = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp1,
     & 2)*jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp1,
     & 2)*jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,
     & axisp1,2)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,
     & axisp1,2)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2,i3-1,axisp1,2)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-
     & (rsxy(i1-1,i2,i3-1,axisp1,2)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))
     & -((rsxy(i1+2,i2,i3-1,axisp1,2)*jac3di(i1+2-i10,i2-i20,i3-1-i30)
     & )-(rsxy(i1-2,i2,i3-1,axisp1,2)*jac3di(i1-2-i10,i2-i20,i3-1-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp1,2)*jac3di(i1+
     & 1-i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp1,2)*jac3di(i1-
     & 1-i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp1,2)*jac3di(
     & i1+2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp1,2)*jac3di(
     & i1-2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-
     & 2,axisp1,2)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-
     & 2,axisp1,2)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,
     & i3-2,axisp1,2)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,
     & i3-2,axisp1,2)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0)))
     & )/(12.*dr(2))
                          a31rs = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp2,
     & 0)*jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp2,
     & 0)*jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,
     & axisp2,0)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,
     & axisp2,0)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2,i3-1,axisp2,0)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-
     & (rsxy(i1-1,i2,i3-1,axisp2,0)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))
     & -((rsxy(i1+2,i2,i3-1,axisp2,0)*jac3di(i1+2-i10,i2-i20,i3-1-i30)
     & )-(rsxy(i1-2,i2,i3-1,axisp2,0)*jac3di(i1-2-i10,i2-i20,i3-1-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp2,0)*jac3di(i1+
     & 1-i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp2,0)*jac3di(i1-
     & 1-i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp2,0)*jac3di(
     & i1+2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp2,0)*jac3di(
     & i1-2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-
     & 2,axisp2,0)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-
     & 2,axisp2,0)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,
     & i3-2,axisp2,0)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,
     & i3-2,axisp2,0)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0)))
     & )/(12.*dr(2))
                          a32rs = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp2,
     & 1)*jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp2,
     & 1)*jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,
     & axisp2,1)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,
     & axisp2,1)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2,i3-1,axisp2,1)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-
     & (rsxy(i1-1,i2,i3-1,axisp2,1)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))
     & -((rsxy(i1+2,i2,i3-1,axisp2,1)*jac3di(i1+2-i10,i2-i20,i3-1-i30)
     & )-(rsxy(i1-2,i2,i3-1,axisp2,1)*jac3di(i1-2-i10,i2-i20,i3-1-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp2,1)*jac3di(i1+
     & 1-i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp2,1)*jac3di(i1-
     & 1-i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp2,1)*jac3di(
     & i1+2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp2,1)*jac3di(
     & i1-2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-
     & 2,axisp2,1)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-
     & 2,axisp2,1)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,
     & i3-2,axisp2,1)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,
     & i3-2,axisp2,1)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0)))
     & )/(12.*dr(2))
                          a33rs = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp2,
     & 2)*jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp2,
     & 2)*jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,
     & axisp2,2)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,
     & axisp2,2)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2,i3-1,axisp2,2)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-
     & (rsxy(i1-1,i2,i3-1,axisp2,2)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))
     & -((rsxy(i1+2,i2,i3-1,axisp2,2)*jac3di(i1+2-i10,i2-i20,i3-1-i30)
     & )-(rsxy(i1-2,i2,i3-1,axisp2,2)*jac3di(i1-2-i10,i2-i20,i3-1-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp2,2)*jac3di(i1+
     & 1-i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp2,2)*jac3di(i1-
     & 1-i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp2,2)*jac3di(
     & i1+2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp2,2)*jac3di(
     & i1-2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-
     & 2,axisp2,2)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-
     & 2,axisp2,2)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,
     & i3-2,axisp2,2)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,
     & i3-2,axisp2,2)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0)))
     & )/(12.*dr(2))
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
                          a21rt = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp1,
     & 0)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp1,
     & 0)*jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,
     & axisp1,0)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,
     & axisp1,0)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*(
     & (rsxy(i1,i2+1,i3-1,axisp1,0)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-
     & (rsxy(i1,i2-1,i3-1,axisp1,0)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))
     & -((rsxy(i1,i2+2,i3-1,axisp1,0)*jac3di(i1-i10,i2+2-i20,i3-1-i30)
     & )-(rsxy(i1,i2-2,i3-1,axisp1,0)*jac3di(i1-i10,i2-2-i20,i3-1-i30)
     & )))/(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp1,0)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp1,0)*jac3di(i1-
     & i10,i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp1,0)*jac3di(
     & i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp1,0)*jac3di(
     & i1-i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-
     & 2,axisp1,0)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-
     & 2,axisp1,0)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,
     & i3-2,axisp1,0)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp1,0)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1)))
     & )/(12.*dr(2))
                          a22rt = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp1,
     & 1)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp1,
     & 1)*jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,
     & axisp1,1)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,
     & axisp1,1)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*(
     & (rsxy(i1,i2+1,i3-1,axisp1,1)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-
     & (rsxy(i1,i2-1,i3-1,axisp1,1)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))
     & -((rsxy(i1,i2+2,i3-1,axisp1,1)*jac3di(i1-i10,i2+2-i20,i3-1-i30)
     & )-(rsxy(i1,i2-2,i3-1,axisp1,1)*jac3di(i1-i10,i2-2-i20,i3-1-i30)
     & )))/(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp1,1)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp1,1)*jac3di(i1-
     & i10,i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp1,1)*jac3di(
     & i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp1,1)*jac3di(
     & i1-i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-
     & 2,axisp1,1)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-
     & 2,axisp1,1)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,
     & i3-2,axisp1,1)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp1,1)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1)))
     & )/(12.*dr(2))
                          a23rt = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp1,
     & 2)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp1,
     & 2)*jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,
     & axisp1,2)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,
     & axisp1,2)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*(
     & (rsxy(i1,i2+1,i3-1,axisp1,2)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-
     & (rsxy(i1,i2-1,i3-1,axisp1,2)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))
     & -((rsxy(i1,i2+2,i3-1,axisp1,2)*jac3di(i1-i10,i2+2-i20,i3-1-i30)
     & )-(rsxy(i1,i2-2,i3-1,axisp1,2)*jac3di(i1-i10,i2-2-i20,i3-1-i30)
     & )))/(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp1,2)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp1,2)*jac3di(i1-
     & i10,i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp1,2)*jac3di(
     & i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp1,2)*jac3di(
     & i1-i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-
     & 2,axisp1,2)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-
     & 2,axisp1,2)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,
     & i3-2,axisp1,2)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp1,2)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1)))
     & )/(12.*dr(2))
                          a31rt = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp2,
     & 0)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp2,
     & 0)*jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,
     & axisp2,0)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,
     & axisp2,0)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*(
     & (rsxy(i1,i2+1,i3-1,axisp2,0)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-
     & (rsxy(i1,i2-1,i3-1,axisp2,0)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))
     & -((rsxy(i1,i2+2,i3-1,axisp2,0)*jac3di(i1-i10,i2+2-i20,i3-1-i30)
     & )-(rsxy(i1,i2-2,i3-1,axisp2,0)*jac3di(i1-i10,i2-2-i20,i3-1-i30)
     & )))/(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp2,0)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp2,0)*jac3di(i1-
     & i10,i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp2,0)*jac3di(
     & i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp2,0)*jac3di(
     & i1-i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-
     & 2,axisp2,0)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-
     & 2,axisp2,0)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,
     & i3-2,axisp2,0)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp2,0)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1)))
     & )/(12.*dr(2))
                          a32rt = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp2,
     & 1)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp2,
     & 1)*jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,
     & axisp2,1)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,
     & axisp2,1)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*(
     & (rsxy(i1,i2+1,i3-1,axisp2,1)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-
     & (rsxy(i1,i2-1,i3-1,axisp2,1)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))
     & -((rsxy(i1,i2+2,i3-1,axisp2,1)*jac3di(i1-i10,i2+2-i20,i3-1-i30)
     & )-(rsxy(i1,i2-2,i3-1,axisp2,1)*jac3di(i1-i10,i2-2-i20,i3-1-i30)
     & )))/(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp2,1)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp2,1)*jac3di(i1-
     & i10,i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp2,1)*jac3di(
     & i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp2,1)*jac3di(
     & i1-i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-
     & 2,axisp2,1)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-
     & 2,axisp2,1)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,
     & i3-2,axisp2,1)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp2,1)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1)))
     & )/(12.*dr(2))
                          a33rt = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp2,
     & 2)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp2,
     & 2)*jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,
     & axisp2,2)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,
     & axisp2,2)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*(
     & (rsxy(i1,i2+1,i3-1,axisp2,2)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-
     & (rsxy(i1,i2-1,i3-1,axisp2,2)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))
     & -((rsxy(i1,i2+2,i3-1,axisp2,2)*jac3di(i1-i10,i2+2-i20,i3-1-i30)
     & )-(rsxy(i1,i2-2,i3-1,axisp2,2)*jac3di(i1-i10,i2-2-i20,i3-1-i30)
     & )))/(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp2,2)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp2,2)*jac3di(i1-
     & i10,i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp2,2)*jac3di(
     & i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp2,2)*jac3di(
     & i1-i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-
     & 2,axisp2,2)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-
     & 2,axisp2,2)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,
     & i3-2,axisp2,2)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp2,2)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1)))
     & )/(12.*dr(2))
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
                          a21st = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp1,
     & 0)*jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp1,
     & 0)*jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,
     & axisp1,0)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,
     & axisp1,0)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2-1,i3,axisp1,0)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-
     & (rsxy(i1-1,i2-1,i3,axisp1,0)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))
     & -((rsxy(i1+2,i2-1,i3,axisp1,0)*jac3di(i1+2-i10,i2-1-i20,i3-i30)
     & )-(rsxy(i1-2,i2-1,i3,axisp1,0)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp1,0)*jac3di(i1+
     & 1-i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp1,0)*jac3di(i1-
     & 1-i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp1,0)*jac3di(
     & i1+2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp1,0)*jac3di(
     & i1-2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,
     & i3,axisp1,0)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,
     & i3,axisp1,0)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-
     & 2,i3,axisp1,0)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-
     & 2,i3,axisp1,0)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0)))
     & )/(12.*dr(1))
                          a22st = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp1,
     & 1)*jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp1,
     & 1)*jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,
     & axisp1,1)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,
     & axisp1,1)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2-1,i3,axisp1,1)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-
     & (rsxy(i1-1,i2-1,i3,axisp1,1)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))
     & -((rsxy(i1+2,i2-1,i3,axisp1,1)*jac3di(i1+2-i10,i2-1-i20,i3-i30)
     & )-(rsxy(i1-2,i2-1,i3,axisp1,1)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp1,1)*jac3di(i1+
     & 1-i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp1,1)*jac3di(i1-
     & 1-i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp1,1)*jac3di(
     & i1+2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp1,1)*jac3di(
     & i1-2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,
     & i3,axisp1,1)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,
     & i3,axisp1,1)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-
     & 2,i3,axisp1,1)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-
     & 2,i3,axisp1,1)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0)))
     & )/(12.*dr(1))
                          a23st = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp1,
     & 2)*jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp1,
     & 2)*jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,
     & axisp1,2)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,
     & axisp1,2)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2-1,i3,axisp1,2)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-
     & (rsxy(i1-1,i2-1,i3,axisp1,2)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))
     & -((rsxy(i1+2,i2-1,i3,axisp1,2)*jac3di(i1+2-i10,i2-1-i20,i3-i30)
     & )-(rsxy(i1-2,i2-1,i3,axisp1,2)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp1,2)*jac3di(i1+
     & 1-i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp1,2)*jac3di(i1-
     & 1-i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp1,2)*jac3di(
     & i1+2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp1,2)*jac3di(
     & i1-2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,
     & i3,axisp1,2)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,
     & i3,axisp1,2)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-
     & 2,i3,axisp1,2)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-
     & 2,i3,axisp1,2)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0)))
     & )/(12.*dr(1))
                          a31st = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp2,
     & 0)*jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp2,
     & 0)*jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,
     & axisp2,0)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,
     & axisp2,0)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2-1,i3,axisp2,0)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-
     & (rsxy(i1-1,i2-1,i3,axisp2,0)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))
     & -((rsxy(i1+2,i2-1,i3,axisp2,0)*jac3di(i1+2-i10,i2-1-i20,i3-i30)
     & )-(rsxy(i1-2,i2-1,i3,axisp2,0)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp2,0)*jac3di(i1+
     & 1-i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp2,0)*jac3di(i1-
     & 1-i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp2,0)*jac3di(
     & i1+2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp2,0)*jac3di(
     & i1-2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,
     & i3,axisp2,0)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,
     & i3,axisp2,0)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-
     & 2,i3,axisp2,0)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-
     & 2,i3,axisp2,0)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0)))
     & )/(12.*dr(1))
                          a32st = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp2,
     & 1)*jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp2,
     & 1)*jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,
     & axisp2,1)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,
     & axisp2,1)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2-1,i3,axisp2,1)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-
     & (rsxy(i1-1,i2-1,i3,axisp2,1)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))
     & -((rsxy(i1+2,i2-1,i3,axisp2,1)*jac3di(i1+2-i10,i2-1-i20,i3-i30)
     & )-(rsxy(i1-2,i2-1,i3,axisp2,1)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp2,1)*jac3di(i1+
     & 1-i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp2,1)*jac3di(i1-
     & 1-i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp2,1)*jac3di(
     & i1+2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp2,1)*jac3di(
     & i1-2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,
     & i3,axisp2,1)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,
     & i3,axisp2,1)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-
     & 2,i3,axisp2,1)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-
     & 2,i3,axisp2,1)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0)))
     & )/(12.*dr(1))
                          a33st = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp2,
     & 2)*jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp2,
     & 2)*jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,
     & axisp2,2)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,
     & axisp2,2)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2-1,i3,axisp2,2)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-
     & (rsxy(i1-1,i2-1,i3,axisp2,2)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))
     & -((rsxy(i1+2,i2-1,i3,axisp2,2)*jac3di(i1+2-i10,i2-1-i20,i3-i30)
     & )-(rsxy(i1-2,i2-1,i3,axisp2,2)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp2,2)*jac3di(i1+
     & 1-i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp2,2)*jac3di(i1-
     & 1-i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp2,2)*jac3di(
     & i1+2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp2,2)*jac3di(
     & i1-2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,
     & i3,axisp2,2)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,
     & i3,axisp2,2)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-
     & 2,i3,axisp2,2)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-
     & 2,i3,axisp2,2)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0)))
     & )/(12.*dr(1))
                          urt=ust4(i1,i2,i3,ex)
                          ust=urs4(i1,i2,i3,ex)
                          urtt=usst2(i1,i2,i3,ex)
                          ustt=urss2(i1,i2,i3,ex)
                          vrt  =ust4(i1,i2,i3,ey)
                          vst  =urs4(i1,i2,i3,ey)
                          vrtt=usst2(i1,i2,i3,ey)
                          vstt=urss2(i1,i2,i3,ey)
                          wrt  =ust4(i1,i2,i3,ez)
                          wst  =urs4(i1,i2,i3,ez)
                          wrtt=usst2(i1,i2,i3,ez)
                          wstt=urss2(i1,i2,i3,ez)
                       else ! edgeDirection.eq.2
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
                          a21rs = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp1,
     & 0)*jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp1,
     & 0)*jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,
     & axisp1,0)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,
     & axisp1,0)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2-1,i3,axisp1,0)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-
     & (rsxy(i1-1,i2-1,i3,axisp1,0)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))
     & -((rsxy(i1+2,i2-1,i3,axisp1,0)*jac3di(i1+2-i10,i2-1-i20,i3-i30)
     & )-(rsxy(i1-2,i2-1,i3,axisp1,0)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp1,0)*jac3di(i1+
     & 1-i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp1,0)*jac3di(i1-
     & 1-i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp1,0)*jac3di(
     & i1+2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp1,0)*jac3di(
     & i1-2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,
     & i3,axisp1,0)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,
     & i3,axisp1,0)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-
     & 2,i3,axisp1,0)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-
     & 2,i3,axisp1,0)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0)))
     & )/(12.*dr(1))
                          a22rs = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp1,
     & 1)*jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp1,
     & 1)*jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,
     & axisp1,1)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,
     & axisp1,1)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2-1,i3,axisp1,1)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-
     & (rsxy(i1-1,i2-1,i3,axisp1,1)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))
     & -((rsxy(i1+2,i2-1,i3,axisp1,1)*jac3di(i1+2-i10,i2-1-i20,i3-i30)
     & )-(rsxy(i1-2,i2-1,i3,axisp1,1)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp1,1)*jac3di(i1+
     & 1-i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp1,1)*jac3di(i1-
     & 1-i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp1,1)*jac3di(
     & i1+2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp1,1)*jac3di(
     & i1-2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,
     & i3,axisp1,1)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,
     & i3,axisp1,1)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-
     & 2,i3,axisp1,1)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-
     & 2,i3,axisp1,1)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0)))
     & )/(12.*dr(1))
                          a23rs = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp1,
     & 2)*jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp1,
     & 2)*jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,
     & axisp1,2)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,
     & axisp1,2)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2-1,i3,axisp1,2)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-
     & (rsxy(i1-1,i2-1,i3,axisp1,2)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))
     & -((rsxy(i1+2,i2-1,i3,axisp1,2)*jac3di(i1+2-i10,i2-1-i20,i3-i30)
     & )-(rsxy(i1-2,i2-1,i3,axisp1,2)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp1,2)*jac3di(i1+
     & 1-i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp1,2)*jac3di(i1-
     & 1-i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp1,2)*jac3di(
     & i1+2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp1,2)*jac3di(
     & i1-2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,
     & i3,axisp1,2)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,
     & i3,axisp1,2)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-
     & 2,i3,axisp1,2)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-
     & 2,i3,axisp1,2)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0)))
     & )/(12.*dr(1))
                          a31rs = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp2,
     & 0)*jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp2,
     & 0)*jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,
     & axisp2,0)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,
     & axisp2,0)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2-1,i3,axisp2,0)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-
     & (rsxy(i1-1,i2-1,i3,axisp2,0)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))
     & -((rsxy(i1+2,i2-1,i3,axisp2,0)*jac3di(i1+2-i10,i2-1-i20,i3-i30)
     & )-(rsxy(i1-2,i2-1,i3,axisp2,0)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp2,0)*jac3di(i1+
     & 1-i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp2,0)*jac3di(i1-
     & 1-i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp2,0)*jac3di(
     & i1+2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp2,0)*jac3di(
     & i1-2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,
     & i3,axisp2,0)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,
     & i3,axisp2,0)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-
     & 2,i3,axisp2,0)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-
     & 2,i3,axisp2,0)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0)))
     & )/(12.*dr(1))
                          a32rs = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp2,
     & 1)*jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp2,
     & 1)*jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,
     & axisp2,1)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,
     & axisp2,1)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2-1,i3,axisp2,1)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-
     & (rsxy(i1-1,i2-1,i3,axisp2,1)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))
     & -((rsxy(i1+2,i2-1,i3,axisp2,1)*jac3di(i1+2-i10,i2-1-i20,i3-i30)
     & )-(rsxy(i1-2,i2-1,i3,axisp2,1)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp2,1)*jac3di(i1+
     & 1-i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp2,1)*jac3di(i1-
     & 1-i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp2,1)*jac3di(
     & i1+2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp2,1)*jac3di(
     & i1-2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,
     & i3,axisp2,1)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,
     & i3,axisp2,1)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-
     & 2,i3,axisp2,1)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-
     & 2,i3,axisp2,1)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0)))
     & )/(12.*dr(1))
                          a33rs = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp2,
     & 2)*jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp2,
     & 2)*jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,
     & axisp2,2)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,
     & axisp2,2)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2-1,i3,axisp2,2)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-
     & (rsxy(i1-1,i2-1,i3,axisp2,2)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))
     & -((rsxy(i1+2,i2-1,i3,axisp2,2)*jac3di(i1+2-i10,i2-1-i20,i3-i30)
     & )-(rsxy(i1-2,i2-1,i3,axisp2,2)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp2,2)*jac3di(i1+
     & 1-i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp2,2)*jac3di(i1-
     & 1-i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp2,2)*jac3di(
     & i1+2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp2,2)*jac3di(
     & i1-2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,
     & i3,axisp2,2)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,
     & i3,axisp2,2)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-
     & 2,i3,axisp2,2)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-
     & 2,i3,axisp2,2)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0)))
     & )/(12.*dr(1))
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
                          a21rt = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp1,
     & 0)*jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp1,
     & 0)*jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,
     & axisp1,0)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,
     & axisp1,0)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2,i3-1,axisp1,0)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-
     & (rsxy(i1-1,i2,i3-1,axisp1,0)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))
     & -((rsxy(i1+2,i2,i3-1,axisp1,0)*jac3di(i1+2-i10,i2-i20,i3-1-i30)
     & )-(rsxy(i1-2,i2,i3-1,axisp1,0)*jac3di(i1-2-i10,i2-i20,i3-1-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp1,0)*jac3di(i1+
     & 1-i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp1,0)*jac3di(i1-
     & 1-i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp1,0)*jac3di(
     & i1+2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp1,0)*jac3di(
     & i1-2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-
     & 2,axisp1,0)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-
     & 2,axisp1,0)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,
     & i3-2,axisp1,0)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,
     & i3-2,axisp1,0)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0)))
     & )/(12.*dr(2))
                          a22rt = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp1,
     & 1)*jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp1,
     & 1)*jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,
     & axisp1,1)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,
     & axisp1,1)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2,i3-1,axisp1,1)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-
     & (rsxy(i1-1,i2,i3-1,axisp1,1)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))
     & -((rsxy(i1+2,i2,i3-1,axisp1,1)*jac3di(i1+2-i10,i2-i20,i3-1-i30)
     & )-(rsxy(i1-2,i2,i3-1,axisp1,1)*jac3di(i1-2-i10,i2-i20,i3-1-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp1,1)*jac3di(i1+
     & 1-i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp1,1)*jac3di(i1-
     & 1-i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp1,1)*jac3di(
     & i1+2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp1,1)*jac3di(
     & i1-2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-
     & 2,axisp1,1)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-
     & 2,axisp1,1)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,
     & i3-2,axisp1,1)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,
     & i3-2,axisp1,1)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0)))
     & )/(12.*dr(2))
                          a23rt = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp1,
     & 2)*jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp1,
     & 2)*jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,
     & axisp1,2)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,
     & axisp1,2)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2,i3-1,axisp1,2)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-
     & (rsxy(i1-1,i2,i3-1,axisp1,2)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))
     & -((rsxy(i1+2,i2,i3-1,axisp1,2)*jac3di(i1+2-i10,i2-i20,i3-1-i30)
     & )-(rsxy(i1-2,i2,i3-1,axisp1,2)*jac3di(i1-2-i10,i2-i20,i3-1-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp1,2)*jac3di(i1+
     & 1-i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp1,2)*jac3di(i1-
     & 1-i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp1,2)*jac3di(
     & i1+2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp1,2)*jac3di(
     & i1-2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-
     & 2,axisp1,2)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-
     & 2,axisp1,2)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,
     & i3-2,axisp1,2)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,
     & i3-2,axisp1,2)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0)))
     & )/(12.*dr(2))
                          a31rt = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp2,
     & 0)*jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp2,
     & 0)*jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,
     & axisp2,0)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,
     & axisp2,0)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2,i3-1,axisp2,0)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-
     & (rsxy(i1-1,i2,i3-1,axisp2,0)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))
     & -((rsxy(i1+2,i2,i3-1,axisp2,0)*jac3di(i1+2-i10,i2-i20,i3-1-i30)
     & )-(rsxy(i1-2,i2,i3-1,axisp2,0)*jac3di(i1-2-i10,i2-i20,i3-1-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp2,0)*jac3di(i1+
     & 1-i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp2,0)*jac3di(i1-
     & 1-i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp2,0)*jac3di(
     & i1+2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp2,0)*jac3di(
     & i1-2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-
     & 2,axisp2,0)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-
     & 2,axisp2,0)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,
     & i3-2,axisp2,0)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,
     & i3-2,axisp2,0)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0)))
     & )/(12.*dr(2))
                          a32rt = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp2,
     & 1)*jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp2,
     & 1)*jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,
     & axisp2,1)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,
     & axisp2,1)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2,i3-1,axisp2,1)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-
     & (rsxy(i1-1,i2,i3-1,axisp2,1)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))
     & -((rsxy(i1+2,i2,i3-1,axisp2,1)*jac3di(i1+2-i10,i2-i20,i3-1-i30)
     & )-(rsxy(i1-2,i2,i3-1,axisp2,1)*jac3di(i1-2-i10,i2-i20,i3-1-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp2,1)*jac3di(i1+
     & 1-i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp2,1)*jac3di(i1-
     & 1-i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp2,1)*jac3di(
     & i1+2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp2,1)*jac3di(
     & i1-2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-
     & 2,axisp2,1)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-
     & 2,axisp2,1)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,
     & i3-2,axisp2,1)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,
     & i3-2,axisp2,1)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0)))
     & )/(12.*dr(2))
                          a33rt = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp2,
     & 2)*jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp2,
     & 2)*jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,
     & axisp2,2)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,
     & axisp2,2)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2,i3-1,axisp2,2)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-
     & (rsxy(i1-1,i2,i3-1,axisp2,2)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))
     & -((rsxy(i1+2,i2,i3-1,axisp2,2)*jac3di(i1+2-i10,i2-i20,i3-1-i30)
     & )-(rsxy(i1-2,i2,i3-1,axisp2,2)*jac3di(i1-2-i10,i2-i20,i3-1-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp2,2)*jac3di(i1+
     & 1-i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp2,2)*jac3di(i1-
     & 1-i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp2,2)*jac3di(
     & i1+2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp2,2)*jac3di(
     & i1-2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-
     & 2,axisp2,2)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-
     & 2,axisp2,2)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,
     & i3-2,axisp2,2)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,
     & i3-2,axisp2,2)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0)))
     & )/(12.*dr(2))
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
                          a21st = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp1,
     & 0)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp1,
     & 0)*jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,
     & axisp1,0)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,
     & axisp1,0)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*(
     & (rsxy(i1,i2+1,i3-1,axisp1,0)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-
     & (rsxy(i1,i2-1,i3-1,axisp1,0)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))
     & -((rsxy(i1,i2+2,i3-1,axisp1,0)*jac3di(i1-i10,i2+2-i20,i3-1-i30)
     & )-(rsxy(i1,i2-2,i3-1,axisp1,0)*jac3di(i1-i10,i2-2-i20,i3-1-i30)
     & )))/(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp1,0)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp1,0)*jac3di(i1-
     & i10,i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp1,0)*jac3di(
     & i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp1,0)*jac3di(
     & i1-i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-
     & 2,axisp1,0)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-
     & 2,axisp1,0)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,
     & i3-2,axisp1,0)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp1,0)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1)))
     & )/(12.*dr(2))
                          a22st = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp1,
     & 1)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp1,
     & 1)*jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,
     & axisp1,1)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,
     & axisp1,1)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*(
     & (rsxy(i1,i2+1,i3-1,axisp1,1)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-
     & (rsxy(i1,i2-1,i3-1,axisp1,1)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))
     & -((rsxy(i1,i2+2,i3-1,axisp1,1)*jac3di(i1-i10,i2+2-i20,i3-1-i30)
     & )-(rsxy(i1,i2-2,i3-1,axisp1,1)*jac3di(i1-i10,i2-2-i20,i3-1-i30)
     & )))/(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp1,1)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp1,1)*jac3di(i1-
     & i10,i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp1,1)*jac3di(
     & i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp1,1)*jac3di(
     & i1-i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-
     & 2,axisp1,1)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-
     & 2,axisp1,1)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,
     & i3-2,axisp1,1)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp1,1)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1)))
     & )/(12.*dr(2))
                          a23st = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp1,
     & 2)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp1,
     & 2)*jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,
     & axisp1,2)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,
     & axisp1,2)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*(
     & (rsxy(i1,i2+1,i3-1,axisp1,2)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-
     & (rsxy(i1,i2-1,i3-1,axisp1,2)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))
     & -((rsxy(i1,i2+2,i3-1,axisp1,2)*jac3di(i1-i10,i2+2-i20,i3-1-i30)
     & )-(rsxy(i1,i2-2,i3-1,axisp1,2)*jac3di(i1-i10,i2-2-i20,i3-1-i30)
     & )))/(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp1,2)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp1,2)*jac3di(i1-
     & i10,i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp1,2)*jac3di(
     & i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp1,2)*jac3di(
     & i1-i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-
     & 2,axisp1,2)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-
     & 2,axisp1,2)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,
     & i3-2,axisp1,2)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp1,2)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1)))
     & )/(12.*dr(2))
                          a31st = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp2,
     & 0)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp2,
     & 0)*jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,
     & axisp2,0)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,
     & axisp2,0)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*(
     & (rsxy(i1,i2+1,i3-1,axisp2,0)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-
     & (rsxy(i1,i2-1,i3-1,axisp2,0)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))
     & -((rsxy(i1,i2+2,i3-1,axisp2,0)*jac3di(i1-i10,i2+2-i20,i3-1-i30)
     & )-(rsxy(i1,i2-2,i3-1,axisp2,0)*jac3di(i1-i10,i2-2-i20,i3-1-i30)
     & )))/(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp2,0)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp2,0)*jac3di(i1-
     & i10,i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp2,0)*jac3di(
     & i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp2,0)*jac3di(
     & i1-i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-
     & 2,axisp2,0)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-
     & 2,axisp2,0)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,
     & i3-2,axisp2,0)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp2,0)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1)))
     & )/(12.*dr(2))
                          a32st = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp2,
     & 1)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp2,
     & 1)*jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,
     & axisp2,1)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,
     & axisp2,1)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*(
     & (rsxy(i1,i2+1,i3-1,axisp2,1)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-
     & (rsxy(i1,i2-1,i3-1,axisp2,1)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))
     & -((rsxy(i1,i2+2,i3-1,axisp2,1)*jac3di(i1-i10,i2+2-i20,i3-1-i30)
     & )-(rsxy(i1,i2-2,i3-1,axisp2,1)*jac3di(i1-i10,i2-2-i20,i3-1-i30)
     & )))/(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp2,1)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp2,1)*jac3di(i1-
     & i10,i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp2,1)*jac3di(
     & i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp2,1)*jac3di(
     & i1-i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-
     & 2,axisp2,1)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-
     & 2,axisp2,1)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,
     & i3-2,axisp2,1)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp2,1)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1)))
     & )/(12.*dr(2))
                          a33st = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp2,
     & 2)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp2,
     & 2)*jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,
     & axisp2,2)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,
     & axisp2,2)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*(
     & (rsxy(i1,i2+1,i3-1,axisp2,2)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-
     & (rsxy(i1,i2-1,i3-1,axisp2,2)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))
     & -((rsxy(i1,i2+2,i3-1,axisp2,2)*jac3di(i1-i10,i2+2-i20,i3-1-i30)
     & )-(rsxy(i1,i2-2,i3-1,axisp2,2)*jac3di(i1-i10,i2-2-i20,i3-1-i30)
     & )))/(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp2,2)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp2,2)*jac3di(i1-
     & i10,i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp2,2)*jac3di(
     & i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp2,2)*jac3di(
     & i1-i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-
     & 2,axisp2,2)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-
     & 2,axisp2,2)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,
     & i3-2,axisp2,2)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp2,2)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1)))
     & )/(12.*dr(2))
                          urt=urt4(i1,i2,i3,ex)
                          ust=ust4(i1,i2,i3,ex)
                          urtt=urtt2(i1,i2,i3,ex)
                          ustt=ustt2(i1,i2,i3,ex)
                          vrt  =urt4(i1,i2,i3,ey)
                          vst  =ust4(i1,i2,i3,ey)
                          vrtt=urtt2(i1,i2,i3,ey)
                          vstt=ustt2(i1,i2,i3,ey)
                          wrt  =urt4(i1,i2,i3,ez)
                          wst  =ust4(i1,i2,i3,ez)
                          wrtt=urtt2(i1,i2,i3,ez)
                          wstt=ustt2(i1,i2,i3,ez)
                       end if
                      uex=u(i1,i2,i3,ex)
                      uey=u(i1,i2,i3,ey)
                      uez=u(i1,i2,i3,ez)
                      ! We get a1.urs, a2.urs from the divergence:
                      ! a1.ur = -( a1r.u + a2.us + a2s.u + a3.ut + a3t.u )
                      ! a1.urs = -( a1s.ur + a1r.us + a1rs.u + a2.uss + 2*a2s.us +a2ss*u + a3s.ut + a3.ust +  a3t.us + a3st.u)
                      a1Doturs = -( (a11s*ur  +a12s*vr  +a13s*wr  ) +(
     & a11r*us  +a12r*vs  +a13r*ws  ) +(a11rs*uex+a12rs*uey+a13rs*uez)
     &  +(a21*uss  +a22*vss  +a23*wss  ) +2.*(a21s*us  +a22s*vs  +
     & a23s*ws  ) +(a21ss*uex+a22ss*uey+a23ss*uez) +(a31s*ut  +a32s*
     & vt  +a33s*wt  ) +(a31*ust  +a32*vst  +a33*wst  ) +(a31t*us  +
     & a32t*vs  +a33t*ws  ) +(a31st*uex+a32st*uey+a33st*uez) )
                      ! a2.us = -( a1.ur + a1r.u + a2s.u + a3.ut + a3t.u )
                      ! a2.urs = -(  a1.urr+2*a1r*ur + a1rr*u + a2r.us +a2s.ur + a2rs.u+   a3r.ut + a3.urt +  a3t.ur + a3rt.u
                      a2Doturs = -( (a21s*ur  +a22s*vr  +a23s*wr  ) +(
     & a21r*us  +a22r*vs  +a23r*ws  ) +(a21rs*uex+a22rs*uey+a23rs*uez)
     &  +(a11*urr  +a12*vrr  +a13*wrr  ) +2.*(a11r*ur  +a12r*vr  +
     & a13r*wr  ) +(a11rr*uex+a12rr*uey+a13rr*uez) +(a31r*ut  +a32r*
     & vt  +a33r*wt  ) +(a31*urt  +a32*vrt  +a33*wrt  ) +(a31t*ur  +
     & a32t*vr  +a33t*wr  ) +(a31rt*uex+a32rt*uey+a33rt*uez) )
                      ! here is a first order approximation to urs, used in the formula for urss and urrs below
                      ! urs = ( (u(i1+is1+js1,i2+is2+js2,i3+is3+js3,ex)-u(i1+js1,i2+js2,i3+js3,ex)) !       - (u(i1+is1    ,i2+is2    ,i3+is3    ,ex)-u(i1    ,i2    ,i3    ,ex)) )/(dra*dsa)
                      ! vrs = ( (u(i1+is1+js1,i2+is2+js2,i3+is3+js3,ey)-u(i1+js1,i2+js2,i3+js3,ey)) !       - (u(i1+is1    ,i2+is2    ,i3+is3    ,ey)-u(i1    ,i2    ,i3    ,ey)) )/(dra*dsa)
                      ! wrs = ( (u(i1+is1+js1,i2+is2+js2,i3+is3+js3,ez)-u(i1+js1,i2+js2,i3+js3,ez)) !       - (u(i1+is1    ,i2+is2    ,i3+is3    ,ez)-u(i1    ,i2    ,i3    ,ez)) )/(dra*dsa)
                      ! here is a second order approximation to urs from :
                      !  u(r,s)   =u0 + (r*ur+s*us) + (1/2)*( r^2*urr + 2*r*s*urs + s^2*uss ) + (1/6)*( r^3*urrr + ... )
                      !  u(2r,2s) =u0 +2(         ) + (4/2)*(                               ) + (8/6)*(                ) 
                      urs = ( 8.*u(i1+is1+js1,i2+is2+js2,i3+is3+js3,ex)
     & -u(i1+2*is1+2*js1,i2+2*is2+2*js2,i3+2*is3+2*js3,ex)-7.*uex -6.*
     & (dra*ur+dsa*us)-2.*(dra**2*urr+dsa**2*uss) )/(4.*dra*dsa)
                      vrs = ( 8.*u(i1+is1+js1,i2+is2+js2,i3+is3+js3,ey)
     & -u(i1+2*is1+2*js1,i2+2*is2+2*js2,i3+2*is3+2*js3,ey)-7.*uey -6.*
     & (dra*vr+dsa*vs)-2.*(dra**2*vrr+dsa**2*vss) )/(4.*dra*dsa)
                      wrs = ( 8.*u(i1+is1+js1,i2+is2+js2,i3+is3+js3,ez)
     & -u(i1+2*is1+2*js1,i2+2*is2+2*js2,i3+2*is3+2*js3,ez)-7.*uez -6.*
     & (dra*wr+dsa*ws)-2.*(dra**2*wrr+dsa**2*wss) )/(4.*dra*dsa)
                      uLapr=0.
                      vLapr=0.
                      wLapr=0.
                      uLaps=0.
                      vLaps=0.
                      wLaps=0.
                      a1Dotu0=a11*u(i1,i2,i3,ex)+a12*u(i1,i2,i3,ey)+
     & a13*u(i1,i2,i3,ez)
                      a2Dotu0=a21*u(i1,i2,i3,ex)+a22*u(i1,i2,i3,ey)+
     & a23*u(i1,i2,i3,ez)
                      a1Doturr=a11*urr+a12*vrr+a13*wrr
                      a1Dotuss=a11*uss+a12*vss+a13*wss
                      a2Doturr=a21*urr+a22*vrr+a23*wrr
                      a2Dotuss=a21*uss+a22*vss+a23*wss
                      a3Dotur=a31*ur+a32*vr+a33*wr
                      a3Dotus=a31*us+a32*vs+a33*ws
                        ! we get a3.urss and a3.urrs from the equation
                        ! c22*uss = -( c11*urr + c33*utt + c1*ur + c2*us + c3*ut )
                        ! c11*urr = -( c22*uss + c33*utt + c1*ur + c2*us + c3*ut )
                        ! c22*urss = -( c22r*uss + c11*urrr + c11r*urr + c33*urtt + c33r*utt + ... )
                        urss = -(  c22r*uss + c11*urrr + c11r*urr + 
     & c33*urtt + c33r*utt + c1*urr + c1r*ur + c2*urs + c2r*us + c3*
     & urt + c3r*ut - uLapr )/c22
                        urrs = -(  c11s*urr + c22*usss + c22s*uss + 
     & c33*ustt + c33s*utt + c1*urs + c1s*ur + c2*uss + c2s*us + c3*
     & ust + c3s*ut - uLaps )/c11
                        vrss = -(  c22r*vss + c11*vrrr + c11r*vrr + 
     & c33*vrtt + c33r*vtt + c1*vrr + c1r*vr + c2*vrs + c2r*vs + c3*
     & vrt + c3r*vt - vLapr )/c22
                        vrrs = -(  c11s*vrr + c22*vsss + c22s*vss + 
     & c33*vstt + c33s*vtt + c1*vrs + c1s*vr + c2*vss + c2s*vs + c3*
     & vst + c3s*vt - vLaps )/c11
                        wrss = -(  c22r*wss + c11*wrrr + c11r*wrr + 
     & c33*wrtt + c33r*wtt + c1*wrr + c1r*wr + c2*wrs + c2r*ws + c3*
     & wrt + c3r*wt - wLapr )/c22
                        wrrs = -(  c11s*wrr + c22*wsss + c22s*wss + 
     & c33*wstt + c33s*wtt + c1*wrs + c1s*wr + c2*wss + c2s*ws + c3*
     & wst + c3s*wt - wLaps )/c11
                        a3Doturrr=a31*urrr+a32*vrrr+a33*wrrr
                        a3Dotusss=a31*usss+a32*vsss+a33*wsss
                        a3Doturss=a31*urss+a32*vrss+a33*wrss
                        a3Doturrs=a31*urrs+a32*vrrs+a33*wrrs
                      detnt=a33*a11*a22-a33*a12*a21-a13*a31*a22+a31*
     & a23*a12+a13*a32*a21-a32*a23*a11
                      ! loop over different ghost points here -- could make a single loop, 1...4 and use arrays of ms1(m) 
                      do m1=1,numberOfGhostPoints
                      do m2=1,numberOfGhostPoints
                       if( edgeDirection.eq.0 )then
                         ms1=0
                         ms2=(1-2*side2)*m1
                         ms3=(1-2*side3)*m2
                         drb=dr(1)*ms2
                         dsb=dr(2)*ms3
                       else if( edgeDirection.eq.1 )then
                         ms2=0
                         ms3=(1-2*side3)*m1
                         ms1=(1-2*side1)*m2
                         drb=dr(2)*ms3
                         dsb=dr(0)*ms1
                       else
                         ms3=0
                         ms1=(1-2*side1)*m1
                         ms2=(1-2*side2)*m2
                         drb=dr(0)*ms1
                         dsb=dr(1)*ms2
                       end if
                      ! **** this is really for order=4 -- no need to be so accurate for order 2 ******
                      ! Here are a1.u(i1-ms1,i2-ms2,i3-ms3,.) a2.u(...), a3.u(...)
                      ! a1Dotu and a2Dotu -- odd Taylor series
                      ! a3Dotu : even Taylor series
                      a1Dotu = 2.*a1Dotu0 -(a11*u(i1+ms1,i2+ms2,i3+ms3,
     & ex)+a12*u(i1+ms1,i2+ms2,i3+ms3,ey)+a13*u(i1+ms1,i2+ms2,i3+ms3,
     & ez)) + drb**2*(a1Doturr) + 2.*drb*dsb*a1Doturs + dsb**2*(
     & a1Dotuss)
                      a2Dotu = 2.*a2Dotu0 -(a21*u(i1+ms1,i2+ms2,i3+ms3,
     & ex)+a22*u(i1+ms1,i2+ms2,i3+ms3,ey)+a23*u(i1+ms1,i2+ms2,i3+ms3,
     & ez)) + drb**2*(a2Doturr) + 2.*drb*dsb*a2Doturs + dsb**2*(
     & a2Dotuss)
                        a3Dotu = (a31*u(i1+ms1,i2+ms2,i3+ms3,ex)+a32*u(
     & i1+ms1,i2+ms2,i3+ms3,ey)+a33*u(i1+ms1,i2+ms2,i3+ms3,ez))-2.*( 
     & drb*(a3Dotur) + dsb*(a3Dotus) ) -(1./3.)*( drb**3*(a3Doturrr) +
     &  dsb**3*(a3Dotusss) + 3.*drb**2*dsb*(a3Doturrs) + 3.*drb*dsb**
     & 2*(a3Doturss) )
                      ! Now given a1.u(-1), a2.u(-1) a3.u(-1) we solve for u(-1)
                      u(i1-ms1,i2-ms2,i3-ms3,ex)=(a33*a1DotU*a22-a13*
     & a3DotU*a22+a13*a32*a2DotU+a3DotU*a23*a12-a32*a23*a1DotU-a33*
     & a12*a2DotU)/detnt
                      u(i1-ms1,i2-ms2,i3-ms3,ey)=(-a23*a11*a3DotU+a23*
     & a1DotU*a31+a11*a33*a2DotU+a13*a21*a3DotU-a1DotU*a33*a21-a13*
     & a2DotU*a31)/detnt
                      u(i1-ms1,i2-ms2,i3-ms3,ez)=(a11*a3DotU*a22-a11*
     & a32*a2DotU-a12*a21*a3DotU+a12*a2DotU*a31-a1DotU*a31*a22+a1DotU*
     & a32*a21)/detnt
                        ! *** extrap for now ****
                        ! j1=i1-ms1
                        ! j2=i2-ms2
                        ! j3=i3-ms3
                        ! u(j1,j2,j3,ex)=5.*u(j1+ls1,j2+ls2,j3+ls3,ex)-10.*u(j1+2*ls1,j2+2*ls2,j3+2*ls3,ex)+10.*u(j1+3*ls1,j2+3*ls2,j3+3*ls3,ex)!               -5.*u(j1+4*ls1,j2+4*ls2,j3+4*ls3,ex)+u(j1+5*ls1,j2+5*ls2,j3+5*ls3,ex)
                        ! u(j1,j2,j3,ey)=5.*u(j1+ls1,j2+ls2,j3+ls3,ey)-10.*u(j1+2*ls1,j2+2*ls2,j3+2*ls3,ey)+10.*u(j1+3*ls1,j2+3*ls2,j3+3*ls3,ey)!               -5.*u(j1+4*ls1,j2+4*ls2,j3+4*ls3,ey)+u(j1+5*ls1,j2+5*ls2,j3+5*ls3,ey)
                        ! u(j1,j2,j3,ez)=5.*u(j1+ls1,j2+ls2,j3+ls3,ez)-10.*u(j1+2*ls1,j2+2*ls2,j3+2*ls3,ez)+10.*u(j1+3*ls1,j2+3*ls2,j3+3*ls3,ez)!               -5.*u(j1+4*ls1,j2+4*ls2,j3+4*ls3,ez)+u(j1+5*ls1,j2+5*ls2,j3+5*ls3,ez)
                      end do
                      end do ! m1
                     else
                       ! ---------------- fill in ghost by extrapolation  --------------
                       !  *wdh* 2016/06/24 
                       ! loop over different ghost points here -- could make a single loop, 1...4 and use arrays of ms1(m) 
                      do m1=1,numberOfGhostPoints
                      do m2=1,numberOfGhostPoints
                       if( edgeDirection.eq.0 )then
                         ns1=0
                         ns2=(1-2*side2)
                         ns3=(1-2*side3)
                         ms1=0
                         ms2=(1-2*side2)*m1
                         ms3=(1-2*side3)*m2
                       else if( edgeDirection.eq.1 )then
                         ns2=0
                         ns3=(1-2*side3)
                         ns1=(1-2*side1)
                         ms2=0
                         ms3=(1-2*side3)*m1
                         ms1=(1-2*side1)*m2
                       else
                         ns3=0
                         ns1=(1-2*side1)
                         ns2=(1-2*side2)
                         ms3=0
                         ms1=(1-2*side1)*m1
                         ms2=(1-2*side2)*m2
                       end if
                        u(i1-ms1,i2-ms2,i3-ms3,ex)=(5.*u(i1-ms1+ns1,i2-
     & ms2+ns2,i3-ms3+ns3,ex)-10.*u(i1-ms1+2*ns1,i2-ms2+2*ns2,i3-ms3+
     & 2*ns3,ex)+10.*u(i1-ms1+3*ns1,i2-ms2+3*ns2,i3-ms3+3*ns3,ex)-5.*
     & u(i1-ms1+4*ns1,i2-ms2+4*ns2,i3-ms3+4*ns3,ex)+u(i1-ms1+5*ns1,i2-
     & ms2+5*ns2,i3-ms3+5*ns3,ex))
                        u(i1-ms1,i2-ms2,i3-ms3,ey)=(5.*u(i1-ms1+ns1,i2-
     & ms2+ns2,i3-ms3+ns3,ey)-10.*u(i1-ms1+2*ns1,i2-ms2+2*ns2,i3-ms3+
     & 2*ns3,ey)+10.*u(i1-ms1+3*ns1,i2-ms2+3*ns2,i3-ms3+3*ns3,ey)-5.*
     & u(i1-ms1+4*ns1,i2-ms2+4*ns2,i3-ms3+4*ns3,ey)+u(i1-ms1+5*ns1,i2-
     & ms2+5*ns2,i3-ms3+5*ns3,ey))
                        u(i1-ms1,i2-ms2,i3-ms3,ez)=(5.*u(i1-ms1+ns1,i2-
     & ms2+ns2,i3-ms3+ns3,ez)-10.*u(i1-ms1+2*ns1,i2-ms2+2*ns2,i3-ms3+
     & 2*ns3,ez)+10.*u(i1-ms1+3*ns1,i2-ms2+3*ns2,i3-ms3+3*ns3,ez)-5.*
     & u(i1-ms1+4*ns1,i2-ms2+4*ns2,i3-ms3+4*ns3,ez)+u(i1-ms1+5*ns1,i2-
     & ms2+5*ns2,i3-ms3+5*ns3,ez))
                      end do ! m2
                      end do ! m1
                     end if  ! end if mask
                     end do ! end do i1
                     end do ! end do i2
                     end do ! end do i3
                   else if( bc1.eq.perfectElectricalConductor .or. 
     & bc2.eq.perfectElectricalConductor )then
                    ! ***************************************************************************
                    ! ************* PEC FACE ON ONE ADJACENT FACE (CURVILINEAR) *****************
                    ! ***************************************************************************
                     ! *new* *wdh*  2015/07/12 
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                     if( mask(i1,i2,i3).ne.0 )then
                      ! ---------------- fill in ghost by extrapolation  --------------
                      do m1=1,numberOfGhostPoints
                      do m2=1,numberOfGhostPoints
                       if( edgeDirection.eq.0 )then
                         ns1=0
                         ns2=(1-2*side2)
                         ns3=(1-2*side3)
                         ms1=0
                         ms2=(1-2*side2)*m1
                         ms3=(1-2*side3)*m2
                       else if( edgeDirection.eq.1 )then
                         ns2=0
                         ns3=(1-2*side3)
                         ns1=(1-2*side1)
                         ms2=0
                         ms3=(1-2*side3)*m1
                         ms1=(1-2*side1)*m2
                       else
                         ns3=0
                         ns1=(1-2*side1)
                         ns2=(1-2*side2)
                         ms3=0
                         ms1=(1-2*side1)*m1
                         ms2=(1-2*side2)*m2
                       end if
                        u(i1-ms1,i2-ms2,i3-ms3,ex)=(5.*u(i1-ms1+ns1,i2-
     & ms2+ns2,i3-ms3+ns3,ex)-10.*u(i1-ms1+2*ns1,i2-ms2+2*ns2,i3-ms3+
     & 2*ns3,ex)+10.*u(i1-ms1+3*ns1,i2-ms2+3*ns2,i3-ms3+3*ns3,ex)-5.*
     & u(i1-ms1+4*ns1,i2-ms2+4*ns2,i3-ms3+4*ns3,ex)+u(i1-ms1+5*ns1,i2-
     & ms2+5*ns2,i3-ms3+5*ns3,ex))
                        u(i1-ms1,i2-ms2,i3-ms3,ey)=(5.*u(i1-ms1+ns1,i2-
     & ms2+ns2,i3-ms3+ns3,ey)-10.*u(i1-ms1+2*ns1,i2-ms2+2*ns2,i3-ms3+
     & 2*ns3,ey)+10.*u(i1-ms1+3*ns1,i2-ms2+3*ns2,i3-ms3+3*ns3,ey)-5.*
     & u(i1-ms1+4*ns1,i2-ms2+4*ns2,i3-ms3+4*ns3,ey)+u(i1-ms1+5*ns1,i2-
     & ms2+5*ns2,i3-ms3+5*ns3,ey))
                        u(i1-ms1,i2-ms2,i3-ms3,ez)=(5.*u(i1-ms1+ns1,i2-
     & ms2+ns2,i3-ms3+ns3,ez)-10.*u(i1-ms1+2*ns1,i2-ms2+2*ns2,i3-ms3+
     & 2*ns3,ez)+10.*u(i1-ms1+3*ns1,i2-ms2+3*ns2,i3-ms3+3*ns3,ez)-5.*
     & u(i1-ms1+4*ns1,i2-ms2+4*ns2,i3-ms3+4*ns3,ez)+u(i1-ms1+5*ns1,i2-
     & ms2+5*ns2,i3-ms3+5*ns3,ez))
                      end do ! m2
                      end do ! m1
                     end if  ! end if mask
                     end do ! end do i1
                     end do ! end do i2
                     end do ! end do i3
                   else if( bc1.eq.dirichlet .or. bc2.eq.dirichlet )
     & then
                     ! *wdh* 081124 -- do nothing here ---
                     ! This is a dirichlet BC 
                 ! *    do m1=1,numberOfGhostPoints
                 ! *    do m2=1,numberOfGhostPoints
                 ! * 
                 ! *     ! shift to ghost point "(m1,m2)"
                 ! *     if( edgeDirection.eq.2 )then 
                 ! *       js1=is1*m1  
                 ! *       js2=is2*m2
                 ! *       js3=0
                 ! *     else if( edgeDirection.eq.1 )then 
                 ! *       js1=is1*m1  
                 ! *       js2=0
                 ! *       js3=is3*m2
                 ! *     else 
                 ! *       js1=0
                 ! *       js2=is2*m1
                 ! *       js3=is3*m2
                 ! *     end if 
                 ! * 
                 ! *     do i3=n3a,n3b
                 ! *     do i2=n2a,n2b
                 ! *     do i1=n1a,n1b
                 ! *   
                 ! *       #If "none" == "twilightZone"
                 ! *         OGF3DFO(i1-js1,i2-js2,i3-js3,t, g1,g2,g3)
                 ! *       #End
                 ! *       u(i1-js1,i2-js2,i3-js3,ex)=g1
                 ! *       u(i1-js1,i2-js2,i3-js3,ey)=g2
                 ! *       u(i1-js1,i2-js2,i3-js3,ez)=g3
                 ! * 
                 ! *     end do ! end do i1
                 ! *     end do ! end do i2
                 ! *     end do ! end do i3
                 ! * 
                 ! *    end do
                 ! *    end do ! m1
                   else if( bc1.le.0 .or. bc2.le.0 )then
                     ! periodic or interpolation -- nothing to do
                   else if( bc1.eq.planeWaveBoundaryCondition 
     & .or.bc2.eq.planeWaveBoundaryCondition .or. 
     & bc1.eq.symmetryBoundaryCondition .or. 
     & bc2.eq.symmetryBoundaryCondition .or. (bc1.ge.abcEM2 .and. 
     & bc1.le.lastBC) .or. (bc2.ge.abcEM2 .and. bc2.le.lastBC))then
                      ! do nothing
                   else
                     write(*,'("ERROR: unknown boundary conditions bc1,
     & bc2=",2i3)') bc1,bc2
                     ! unknown boundary conditions
                     stop 8866
                   end if
                  end do
                  end do
                  end do  ! edge direction
                ! Finally assign points outside the vertices of the unit cube
                g1=0.
                g2=0.
                g3=0.
                do side3=0,1
                do side2=0,1
                do side1=0,1
                 ! assign ghost values outside the corner (vertex)
                 i1=gridIndexRange(side1,0)
                 i2=gridIndexRange(side2,1)
                 i3=gridIndexRange(side3,2)
                 is1=1-2*side1
                 is2=1-2*side2
                 is3=1-2*side3
                 if( boundaryCondition(side1,0)
     & .eq.perfectElectricalConductor .and.boundaryCondition(side2,1)
     & .eq.perfectElectricalConductor .and.boundaryCondition(side3,2)
     & .eq.perfectElectricalConductor )then
                   ! ------------------------------------------------------------
                   ! -------------- VERTEX adjacent to 3 PEC faces --------------
                   ! ------------------------------------------------------------
                    urr = urr2(i1,i2,i3,ex)
                    uss = uss2(i1,i2,i3,ex)
                    utt = utt2(i1,i2,i3,ex)
                    urs = urs2(i1,i2,i3,ex)
                    urt = urt2(i1,i2,i3,ex)
                    ust = ust2(i1,i2,i3,ex)
                    vrr = urr2(i1,i2,i3,ey)
                    vss = uss2(i1,i2,i3,ey)
                    vtt = utt2(i1,i2,i3,ey)
                    vrs = urs2(i1,i2,i3,ey)
                    vrt = urt2(i1,i2,i3,ey)
                    vst = ust2(i1,i2,i3,ey)
                    wrr = urr2(i1,i2,i3,ez)
                    wss = uss2(i1,i2,i3,ez)
                    wtt = utt2(i1,i2,i3,ez)
                    wrs = urs2(i1,i2,i3,ez)
                    wrt = urt2(i1,i2,i3,ez)
                    wst = ust2(i1,i2,i3,ez)
                  do m3=1,numberOfGhostPoints
                  do m2=1,numberOfGhostPoints
                  do m1=1,numberOfGhostPoints
                    js1=is1*m1  ! shift to ghost point "m"
                    js2=is2*m2
                    js3=is3*m3
                    dra=dr(0)*js1
                    dsa=dr(1)*js2
                    dta=dr(2)*js3
                       ! *new* 2015/07/12 
                       u(i1-js1,i2-js2,i3-js3,ex)=(5.*u(i1,i2,i3,ex)-
     & 10.*u(i1+js1,i2+js2,i3+js3,ex)+10.*u(i1+2*js1,i2+2*js2,i3+2*
     & js3,ex)-5.*u(i1+3*js1,i2+3*js2,i3+3*js3,ex)+u(i1+4*js1,i2+4*
     & js2,i3+4*js3,ex))
                       u(i1-js1,i2-js2,i3-js3,ey)=(5.*u(i1,i2,i3,ey)-
     & 10.*u(i1+js1,i2+js2,i3+js3,ey)+10.*u(i1+2*js1,i2+2*js2,i3+2*
     & js3,ey)-5.*u(i1+3*js1,i2+3*js2,i3+3*js3,ey)+u(i1+4*js1,i2+4*
     & js2,i3+4*js3,ey))
                       u(i1-js1,i2-js2,i3-js3,ez)=(5.*u(i1,i2,i3,ez)-
     & 10.*u(i1+js1,i2+js2,i3+js3,ez)+10.*u(i1+2*js1,i2+2*js2,i3+2*
     & js3,ez)-5.*u(i1+3*js1,i2+3*js2,i3+3*js3,ez)+u(i1+4*js1,i2+4*
     & js2,i3+4*js3,ez))
                     if( debug.gt.2 )then
                       write(*,'("Corner point from taylor: ghost-pt=",
     & 3i4," errors=",3e10.2)') i1-js1,i2-js2,i3-js3,u(i1-js1,i2-js2,
     & i3-js3,ex)-um,u(i1-js1,i2-js2,i3-js3,ey)-vm,u(i1-js1,i2-js2,i3-
     & js3,ez)-wm
                       ! write(*,'(" corner: dra,dsa,dta=",3f6.3," urr,uss,utt,urs,urt,ust=",6f8.3)') dra,dsa,dta,!    urr,uss,utt,urs,urt,ust
                       ! "
                     end if
                  end do
                  end do
                  end do
                 else if( .true. .and. (boundaryCondition(side1,0)
     & .eq.perfectElectricalConductor .or.boundaryCondition(side2,1)
     & .eq.perfectElectricalConductor .or.boundaryCondition(side3,2)
     & .eq.perfectElectricalConductor) )then
                   ! *new* *wdh* 2015/07/12 
                   ! -----------------------------------------------------------------
                   ! -------------- VERTEX adjacent to 1 or 2 PEC faces --------------
                   ! -----------------------------------------------------------------
                  do m3=1,numberOfGhostPoints
                  do m2=1,numberOfGhostPoints
                  do m1=1,numberOfGhostPoints
                    js1=is1*m1  ! shift to ghost point "m"
                    js2=is2*m2
                    js3=is3*m3
                     u(i1-js1,i2-js2,i3-js3,ex)=(5.*u(i1,i2,i3,ex)-10.*
     & u(i1+js1,i2+js2,i3+js3,ex)+10.*u(i1+2*js1,i2+2*js2,i3+2*js3,ex)
     & -5.*u(i1+3*js1,i2+3*js2,i3+3*js3,ex)+u(i1+4*js1,i2+4*js2,i3+4*
     & js3,ex))
                     u(i1-js1,i2-js2,i3-js3,ey)=(5.*u(i1,i2,i3,ey)-10.*
     & u(i1+js1,i2+js2,i3+js3,ey)+10.*u(i1+2*js1,i2+2*js2,i3+2*js3,ey)
     & -5.*u(i1+3*js1,i2+3*js2,i3+3*js3,ey)+u(i1+4*js1,i2+4*js2,i3+4*
     & js3,ey))
                     u(i1-js1,i2-js2,i3-js3,ez)=(5.*u(i1,i2,i3,ez)-10.*
     & u(i1+js1,i2+js2,i3+js3,ez)+10.*u(i1+2*js1,i2+2*js2,i3+2*js3,ez)
     & -5.*u(i1+3*js1,i2+3*js2,i3+3*js3,ez)+u(i1+4*js1,i2+4*js2,i3+4*
     & js3,ez))
                  end do ! end do m1
                  end do ! end do m2
                  end do ! end do m3
                 else if( boundaryCondition(side1,0).eq.dirichlet 
     & .or.boundaryCondition(side2,1).eq.dirichlet 
     & .or.boundaryCondition(side3,2).eq.dirichlet )then
                  ! *wdh* 081124 -- do nothing here ---
              ! *    do m3=1,numberOfGhostPoints
              ! *    do m2=1,numberOfGhostPoints
              ! *    do m1=1,numberOfGhostPoints
              ! *
              ! *      js1=is1*m1  ! shift to ghost point "m"
              ! *      js2=is2*m2
              ! *      js3=is3*m3
              ! *
              ! *      #If "none" == "twilightZone" 
              ! *        OGF3DFO(i1-js1,i2-js2,i3-js3,t, g1,g2,g3)
              ! *      #End
              ! *      u(i1-js1,i2-js2,i3-js3,ex)=g1
              ! *      u(i1-js1,i2-js2,i3-js3,ey)=g2
              ! *      u(i1-js1,i2-js2,i3-js3,ez)=g3
              ! *
              ! *    end do
              ! *    end do
              ! *    end do
                 else if( boundaryCondition(side1,0).le.0 
     & .or.boundaryCondition(side2,1).le.0 .or.boundaryCondition(
     & side3,2).le.0 )then
                    ! one or more boundaries are periodic or interpolation -- nothing to do
                 else if( boundaryCondition(side1,0)
     & .eq.planeWaveBoundaryCondition .or.boundaryCondition(side2,1)
     & .eq.planeWaveBoundaryCondition .or.boundaryCondition(side3,2)
     & .eq.planeWaveBoundaryCondition  .or. boundaryCondition(side1,0)
     & .eq.symmetryBoundaryCondition .or. boundaryCondition(side2,1)
     & .eq.symmetryBoundaryCondition .or. boundaryCondition(side3,2)
     & .eq.symmetryBoundaryCondition .or. (boundaryCondition(side1,0)
     & .ge.abcEM2 .and. boundaryCondition(side1,0).le.lastBC) .or. (
     & boundaryCondition(side2,1).ge.abcEM2 .and. boundaryCondition(
     & side2,1).le.lastBC) .or. (boundaryCondition(side3,2).ge.abcEM2 
     & .and. boundaryCondition(side3,2).le.lastBC)  )then
                   ! do nothing
                 else
                   write(*,'("ERROR: unknown boundary conditions at a 
     & 3D corner bc1,bc2,bc3=",2i3)') boundaryCondition(side1,0),
     & boundaryCondition(side2,1),boundaryCondition(side3,2)
                   ! '
                   ! unknown boundary conditions
                   stop 3399
                 end if
                end do
                end do
                end do
            else
                numberOfGhostPoints=orderOfAccuracy/2
                ! Assign the edges
                 do edgeDirection=0,2 ! direction parallel to the edge
                ! do edgeDirection=0,0 ! direction parallel to the edge
                 do sidea=0,1
                 do sideb=0,1
                   if( edgeDirection.eq.0 )then
                     side1=0
                     side2=sidea
                     side3=sideb
                   else if( edgeDirection.eq.1 )then
                     side1=sideb
                     side2=0
                     side3=sidea
                   else
                     side1=sidea
                     side2=sideb
                     side3=0
                   end if
                 is1=1-2*(side1)
                 is2=1-2*(side2)
                 is3=1-2*(side3)
                 if( edgeDirection.eq.2 )then
                  is3=0
                  n1a=gridIndexRange(side1,0)
                  n1b=gridIndexRange(side1,0)
                  n2a=gridIndexRange(side2,1)
                  n2b=gridIndexRange(side2,1)
                  n3a=gridIndexRange(0,2)
                  n3b=gridIndexRange(1,2)
                  bc1=boundaryCondition(side1,0)
                  bc2=boundaryCondition(side2,1)
                 else if( edgeDirection.eq.1 )then
                  is2=0
                  n1a=gridIndexRange(side1,0)
                  n1b=gridIndexRange(side1,0)
                  n2a=gridIndexRange(    0,1)
                  n2b=gridIndexRange(    1,1)
                  n3a=gridIndexRange(side3,2)
                  n3b=gridIndexRange(side3,2)
                  bc1=boundaryCondition(side1,0)
                  bc2=boundaryCondition(side3,2)
                 else
                  is1=0
                  n1a=gridIndexRange(    0,0)
                  n1b=gridIndexRange(    1,0)
                  n2a=gridIndexRange(side2,1)
                  n2b=gridIndexRange(side2,1)
                  n3a=gridIndexRange(side3,2)
                  n3b=gridIndexRange(side3,2)
                  bc1=boundaryCondition(side2,1)
                  bc2=boundaryCondition(side3,2)
                 end if
                 g1=0.
                 g2=0.
                 g3=0.
                 ! ********************************************************************
                 ! ***************Assign Extended boundary points**********************
                 ! ************** on an edge between two faces   ********************** 
                 ! ********************************************************************
                  ! ************** CURVILINEAR -- EXTENDED NEAR TWO FACES ************
                  is1=0
                  is2=0
                  is3=0
                  js1=0
                  js2=0
                  js3=0
                  ks1=0
                  ks2=0
                  ks3=0
                  if( edgeDirection.eq.0 )then
                    axis=1
                    axisp1=2
                    axisp2=0
                    is2=1-2*(side2)
                    js3=1-2*(side3)
                    ks1=1
                    dra=dr(axis  )*(1-2*(side2))
                    dsa=dr(axisp1)*(1-2*(side3))
                    dta=dr(axisp2)*(1          )
                  else if( edgeDirection.eq.1 )then
                    axis=2
                    axisp1=0
                    axisp2=1
                    is3=1-2*(side3)
                    js1=1-2*(side1)
                    ks2=1
                    dra=dr(axis  )*(1-2*(side3))
                    dsa=dr(axisp1)*(1-2*(side1))
                    dta=dr(axisp2)*(1          )
                  else
                    axis=0
                    axisp1=1
                    axisp2=2
                    is1=1-2*(side1)
                    js2=1-2*(side2)
                    ks3=1
                    dra=dr(axis  )*(1-2*(side1))
                    dsa=dr(axisp1)*(1-2*(side2))
                    dta=dr(axisp2)*(1          )
                  end if
                  if( debug.gt.2 )then
                    write(*,'(" cornersMxOrderORDER: **** Start: 
     & edgeDirection=",i1," ,side1,side2,side3 = ",3i2," axis,axisp1,
     & axisp2=",3i2,/,"      dra,dsa,dta=",3e10.2,"****")') 
     & edgeDirection,side1,side2,side3,axis,axisp1,axisp2,dra,dsa,dta
                    ! '
                  end if
                ! if( orderOfAccuracy.eq.4 )then
                  ! ************** CURVILINEAR -- EXTENDED NEAR TWO FACES ************
                  ! **************              4 4                   ************
                   if( bc1.eq.perfectElectricalConductor 
     & .and.bc2.eq.perfectElectricalConductor )then
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                     if( mask(i1,i2,i3).gt.0 )then ! *wdh* 2015/06/24
                       c11 = (rsxy(i1,i2,i3,axis,0)**2+rsxy(i1,i2,i3,
     & axis,1)**2+rsxy(i1,i2,i3,axis,2)**2)
                       c22 = (rsxy(i1,i2,i3,axisp1,0)**2+rsxy(i1,i2,i3,
     & axisp1,1)**2+rsxy(i1,i2,i3,axisp1,2)**2)
                       c33 = (rsxy(i1,i2,i3,axisp2,0)**2+rsxy(i1,i2,i3,
     & axisp2,1)**2+rsxy(i1,i2,i3,axisp2,2)**2)
                       c1 = (rsxyx43(i1,i2,i3,axis,0)+rsxyy43(i1,i2,i3,
     & axis,1)+rsxyz43(i1,i2,i3,axis,2))
                       c2 = (rsxyx43(i1,i2,i3,axisp1,0)+rsxyy43(i1,i2,
     & i3,axisp1,1)+rsxyz43(i1,i2,i3,axisp1,2))
                       c3 = (rsxyx43(i1,i2,i3,axisp2,0)+rsxyy43(i1,i2,
     & i3,axisp2,1)+rsxyz43(i1,i2,i3,axisp2,2))
                       ! urr=URR ,uss,utt,ur,us,ut (also for v and w)
                       urr=(-30.*u(i1,i2,i3,ex)+16.*(u(i1+is1,i2+is2,
     & i3+is3,ex)+u(i1-is1,i2-is2,i3-is3,ex))-(u(i1+2*is1,i2+2*is2,i3+
     & 2*is3,ex)+u(i1-2*is1,i2-2*is2,i3-2*is3,ex)))/(12.*dra**2)
                       uss=(-30.*u(i1,i2,i3,ex)+16.*(u(i1+js1,i2+js2,
     & i3+js3,ex)+u(i1-js1,i2-js2,i3-js3,ex))-(u(i1+2*js1,i2+2*js2,i3+
     & 2*js3,ex)+u(i1-2*js1,i2-2*js2,i3-2*js3,ex)))/(12.*dsa**2)
                       utt=(-30.*u(i1,i2,i3,ex)+16.*(u(i1+ks1,i2+ks2,
     & i3+ks3,ex)+u(i1-ks1,i2-ks2,i3-ks3,ex))-(u(i1+2*ks1,i2+2*ks2,i3+
     & 2*ks3,ex)+u(i1-2*ks1,i2-2*ks2,i3-2*ks3,ex)))/(12.*dta**2)
                       ur = (8.*(u(i1+is1,i2+is2,i3+is3,ex)-u(i1-is1,
     & i2-is2,i3-is3,ex))-(u(i1+2*is1,i2+2*is2,i3+2*is3,ex)-u(i1-2*
     & is1,i2-2*is2,i3-2*is3,ex)))/(12.*dra)
                       us = (8.*(u(i1+js1,i2+js2,i3+js3,ex)-u(i1-js1,
     & i2-js2,i3-js3,ex))-(u(i1+2*js1,i2+2*js2,i3+2*js3,ex)-u(i1-2*
     & js1,i2-2*js2,i3-2*js3,ex)))/(12.*dsa)
                       ut = (8.*(u(i1+ks1,i2+ks2,i3+ks3,ex)-u(i1-ks1,
     & i2-ks2,i3-ks3,ex))-(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ex)-u(i1-2*
     & ks1,i2-2*ks2,i3-2*ks3,ex)))/(12.*dta)
                       vrr=(-30.*u(i1,i2,i3,ey)+16.*(u(i1+is1,i2+is2,
     & i3+is3,ey)+u(i1-is1,i2-is2,i3-is3,ey))-(u(i1+2*is1,i2+2*is2,i3+
     & 2*is3,ey)+u(i1-2*is1,i2-2*is2,i3-2*is3,ey)))/(12.*dra**2)
                       vss=(-30.*u(i1,i2,i3,ey)+16.*(u(i1+js1,i2+js2,
     & i3+js3,ey)+u(i1-js1,i2-js2,i3-js3,ey))-(u(i1+2*js1,i2+2*js2,i3+
     & 2*js3,ey)+u(i1-2*js1,i2-2*js2,i3-2*js3,ey)))/(12.*dsa**2)
                       vtt=(-30.*u(i1,i2,i3,ey)+16.*(u(i1+ks1,i2+ks2,
     & i3+ks3,ey)+u(i1-ks1,i2-ks2,i3-ks3,ey))-(u(i1+2*ks1,i2+2*ks2,i3+
     & 2*ks3,ey)+u(i1-2*ks1,i2-2*ks2,i3-2*ks3,ey)))/(12.*dta**2)
                       vr = (8.*(u(i1+is1,i2+is2,i3+is3,ey)-u(i1-is1,
     & i2-is2,i3-is3,ey))-(u(i1+2*is1,i2+2*is2,i3+2*is3,ey)-u(i1-2*
     & is1,i2-2*is2,i3-2*is3,ey)))/(12.*dra)
                       vs = (8.*(u(i1+js1,i2+js2,i3+js3,ey)-u(i1-js1,
     & i2-js2,i3-js3,ey))-(u(i1+2*js1,i2+2*js2,i3+2*js3,ey)-u(i1-2*
     & js1,i2-2*js2,i3-2*js3,ey)))/(12.*dsa)
                       vt = (8.*(u(i1+ks1,i2+ks2,i3+ks3,ey)-u(i1-ks1,
     & i2-ks2,i3-ks3,ey))-(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ey)-u(i1-2*
     & ks1,i2-2*ks2,i3-2*ks3,ey)))/(12.*dta)
                       wrr=(-30.*u(i1,i2,i3,ez)+16.*(u(i1+is1,i2+is2,
     & i3+is3,ez)+u(i1-is1,i2-is2,i3-is3,ez))-(u(i1+2*is1,i2+2*is2,i3+
     & 2*is3,ez)+u(i1-2*is1,i2-2*is2,i3-2*is3,ez)))/(12.*dra**2)
                       wss=(-30.*u(i1,i2,i3,ez)+16.*(u(i1+js1,i2+js2,
     & i3+js3,ez)+u(i1-js1,i2-js2,i3-js3,ez))-(u(i1+2*js1,i2+2*js2,i3+
     & 2*js3,ez)+u(i1-2*js1,i2-2*js2,i3-2*js3,ez)))/(12.*dsa**2)
                       wtt=(-30.*u(i1,i2,i3,ez)+16.*(u(i1+ks1,i2+ks2,
     & i3+ks3,ez)+u(i1-ks1,i2-ks2,i3-ks3,ez))-(u(i1+2*ks1,i2+2*ks2,i3+
     & 2*ks3,ez)+u(i1-2*ks1,i2-2*ks2,i3-2*ks3,ez)))/(12.*dta**2)
                       wr = (8.*(u(i1+is1,i2+is2,i3+is3,ez)-u(i1-is1,
     & i2-is2,i3-is3,ez))-(u(i1+2*is1,i2+2*is2,i3+2*is3,ez)-u(i1-2*
     & is1,i2-2*is2,i3-2*is3,ez)))/(12.*dra)
                       ws = (8.*(u(i1+js1,i2+js2,i3+js3,ez)-u(i1-js1,
     & i2-js2,i3-js3,ez))-(u(i1+2*js1,i2+2*js2,i3+2*js3,ez)-u(i1-2*
     & js1,i2-2*js2,i3-2*js3,ez)))/(12.*dsa)
                       wt = (8.*(u(i1+ks1,i2+ks2,i3+ks3,ez)-u(i1-ks1,
     & i2-ks2,i3-ks3,ez))-(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ez)-u(i1-2*
     & ks1,i2-2*ks2,i3-2*ks3,ez)))/(12.*dta)
                       !    deltaFu,deltaFv,deltaFw = RHS for Delta(u,v,w)
                       deltaFu=0.
                       deltaFv=0.
                       deltaFw=0.
                       !    g1f,g2f = RHS for extrapolation, a1.D+2^4u(i1,i2-2)=g1f, a2.D+2^4u(i1-2,i2)=g2f,    
                       g1f=0.
                       g2f=0.
                !        if( debug.gt.1 )then
                !         write(*,'(" bce4: before: u(-1,0),(-2,0)=",6f7.2)') !           u(i1-  is1,i2-  is2,i3-  is3,ex),!           u(i1-  is1,i2-  is2,i3-  is3,ey),!           u(i1-  is1,i2-  is2,i3-  is3,ez),!           u(i1-2*is1,i2-2*is2,i3-2*is3,ex),!           u(i1-2*is1,i2-2*is2,i3-2*is3,ey),!           u(i1-2*is1,i2-2*is2,i3-2*is3,ez)
                !         write(*,'(" bce4: before u(0,2),u(0,1),u(0,0),u(0,-1),(0,-2)=",/,(4x,3f7.2))') !           u(i1+2*js1,i2+2*js2,i3+2*js3,ex),!           u(i1+2*js1,i2+2*js2,i3+2*js3,ey),!           u(i1+2*js1,i2+2*js2,i3+2*js3,ez),!           u(i1+  js1,i2+  js2,i3+  js3,ex),!           u(i1+  js1,i2+  js2,i3+  js3,ey),!           u(i1+  js1,i2+  js2,i3+  js3,ez),!           u(i1,i2,i3,ex),!           u(i1,i2,i3,ey),!           u(i1,i2,i3,ez),!           u(i1-  js1,i2-  js2,i3-  js3,ex),!           u(i1-  js1,i2-  js2,i3-  js3,ey),!           u(i1-  js1,i2-  js2,i3-  js3,ez),!           u(i1-2*js1,i2-2*js2,i3-2*js3,ex),!           u(i1-2*js1,i2-2*js2,i3-2*js3,ey),!           u(i1-2*js1,i2-2*js2,i3-2*js3,ez)
                !          write(*,'(" bce4: urr,uss,utt=",9f7.2)') urr,vrr,wrr,uss,vss,wss,utt,vtt,wtt
                !        end if
                          call ogDeriv3(ep, 0,2,0,0, xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),xy(i1,i2,i3,2),t, ex,uxx, ey,vxx, ez,wxx)
                          call ogDeriv3(ep, 0,0,2,0, xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),xy(i1,i2,i3,2),t, ex,uyy, ey,vyy, ez,wyy)
                          call ogDeriv3(ep, 0,0,0,2, xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),xy(i1,i2,i3,2),t, ex,uzz, ey,vzz, ez,wzz)
                        deltaFu=uxx+uyy+uzz
                        deltaFv=vxx+vyy+vzz
                        deltaFw=wxx+wyy+wzz
                        ! for now remove the error in the extrapolation ************
                          call ogf3dfo(ep,fieldOption,xy(i1,i2,i3,0),
     & xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,uv0(0),uv0(1),uv0(2))
                          call ogf3dfo(ep,fieldOption,xy(i1-js1,i2-js2,
     & i3-js3,0),xy(i1-js1,i2-js2,i3-js3,1),xy(i1-js1,i2-js2,i3-js3,2)
     & ,t,uvm(0),uvm(1),uvm(2))
                          call ogf3dfo(ep,fieldOption,xy(i1+js1,i2+js2,
     & i3+js3,0),xy(i1+js1,i2+js2,i3+js3,1),xy(i1+js1,i2+js2,i3+js3,2)
     & ,t,uvp(0),uvp(1),uvp(2))
                          call ogf3dfo(ep,fieldOption,xy(i1-2*js1,i2-2*
     & js2,i3-2*js3,0),xy(i1-2*js1,i2-2*js2,i3-2*js3,1),xy(i1-2*js1,
     & i2-2*js2,i3-2*js3,2),t,uvm2(0),uvm2(1),uvm2(2))
                          call ogf3dfo(ep,fieldOption,xy(i1+2*js1,i2+2*
     & js2,i3+2*js3,0),xy(i1+2*js1,i2+2*js2,i3+2*js3,1),xy(i1+2*js1,
     & i2+2*js2,i3+2*js3,2),t,uvp2(0),uvp2(1),uvp2(2))
                        m1=i1-2*js1
                        m2=i2-2*js2
                        m3=i3-2*js3
                        g1f   = (rsxy(m1,m2,m3,axis,0)/(rx(m1,m2,m3)*(
     & sy(m1,m2,m3)*tz(m1,m2,m3)-sz(m1,m2,m3)*ty(m1,m2,m3))+ry(m1,m2,
     & m3)*(sz(m1,m2,m3)*tx(m1,m2,m3)-sx(m1,m2,m3)*tz(m1,m2,m3))+rz(
     & m1,m2,m3)*(sx(m1,m2,m3)*ty(m1,m2,m3)-sy(m1,m2,m3)*tx(m1,m2,m3))
     & ))*( uvm2(0)-4.*uvm(0)+6.*uv0(0)-4.*uvp(0)+uvp2(0)) +(rsxy(m1,
     & m2,m3,axis,1)/(rx(m1,m2,m3)*(sy(m1,m2,m3)*tz(m1,m2,m3)-sz(m1,
     & m2,m3)*ty(m1,m2,m3))+ry(m1,m2,m3)*(sz(m1,m2,m3)*tx(m1,m2,m3)-
     & sx(m1,m2,m3)*tz(m1,m2,m3))+rz(m1,m2,m3)*(sx(m1,m2,m3)*ty(m1,m2,
     & m3)-sy(m1,m2,m3)*tx(m1,m2,m3))))*( uvm2(1)-4.*uvm(1)+6.*uv0(1)-
     & 4.*uvp(1)+uvp2(1)) +(rsxy(m1,m2,m3,axis,2)/(rx(m1,m2,m3)*(sy(
     & m1,m2,m3)*tz(m1,m2,m3)-sz(m1,m2,m3)*ty(m1,m2,m3))+ry(m1,m2,m3)*
     & (sz(m1,m2,m3)*tx(m1,m2,m3)-sx(m1,m2,m3)*tz(m1,m2,m3))+rz(m1,m2,
     & m3)*(sx(m1,m2,m3)*ty(m1,m2,m3)-sy(m1,m2,m3)*tx(m1,m2,m3))))*( 
     & uvm2(2)-4.*uvm(2)+6.*uv0(2)-4.*uvp(2)+uvp2(2))
                          call ogf3dfo(ep,fieldOption,xy(i1-is1,i2-is2,
     & i3-is3,0),xy(i1-is1,i2-is2,i3-is3,1),xy(i1-is1,i2-is2,i3-is3,2)
     & ,t,uvm(0),uvm(1),uvm(2))
                          call ogf3dfo(ep,fieldOption,xy(i1+is1,i2+is2,
     & i3+is3,0),xy(i1+is1,i2+is2,i3+is3,1),xy(i1+is1,i2+is2,i3+is3,2)
     & ,t,uvp(0),uvp(1),uvp(2))
                          call ogf3dfo(ep,fieldOption,xy(i1-2*is1,i2-2*
     & is2,i3-2*is3,0),xy(i1-2*is1,i2-2*is2,i3-2*is3,1),xy(i1-2*is1,
     & i2-2*is2,i3-2*is3,2),t,uvm2(0),uvm2(1),uvm2(2))
                          call ogf3dfo(ep,fieldOption,xy(i1+2*is1,i2+2*
     & is2,i3+2*is3,0),xy(i1+2*is1,i2+2*is2,i3+2*is3,1),xy(i1+2*is1,
     & i2+2*is2,i3+2*is3,2),t,uvp2(0),uvp2(1),uvp2(2))
                        m1=i1-2*is1
                        m2=i2-2*is2
                        m3=i3-2*is3
                        g2f = (rsxy(m1,m2,m3,axisp1,0)/(rx(m1,m2,m3)*(
     & sy(m1,m2,m3)*tz(m1,m2,m3)-sz(m1,m2,m3)*ty(m1,m2,m3))+ry(m1,m2,
     & m3)*(sz(m1,m2,m3)*tx(m1,m2,m3)-sx(m1,m2,m3)*tz(m1,m2,m3))+rz(
     & m1,m2,m3)*(sx(m1,m2,m3)*ty(m1,m2,m3)-sy(m1,m2,m3)*tx(m1,m2,m3))
     & ))*( uvm2(0)-4.*uvm(0)+6.*uv0(0)-4.*uvp(0)+uvp2(0)) +(rsxy(m1,
     & m2,m3,axisp1,1)/(rx(m1,m2,m3)*(sy(m1,m2,m3)*tz(m1,m2,m3)-sz(m1,
     & m2,m3)*ty(m1,m2,m3))+ry(m1,m2,m3)*(sz(m1,m2,m3)*tx(m1,m2,m3)-
     & sx(m1,m2,m3)*tz(m1,m2,m3))+rz(m1,m2,m3)*(sx(m1,m2,m3)*ty(m1,m2,
     & m3)-sy(m1,m2,m3)*tx(m1,m2,m3))))*( uvm2(1)-4.*uvm(1)+6.*uv0(1)-
     & 4.*uvp(1)+uvp2(1)) +(rsxy(m1,m2,m3,axisp1,2)/(rx(m1,m2,m3)*(sy(
     & m1,m2,m3)*tz(m1,m2,m3)-sz(m1,m2,m3)*ty(m1,m2,m3))+ry(m1,m2,m3)*
     & (sz(m1,m2,m3)*tx(m1,m2,m3)-sx(m1,m2,m3)*tz(m1,m2,m3))+rz(m1,m2,
     & m3)*(sx(m1,m2,m3)*ty(m1,m2,m3)-sy(m1,m2,m3)*tx(m1,m2,m3))))*( 
     & uvm2(2)-4.*uvm(2)+6.*uv0(2)-4.*uvp(2)+uvp2(2))
                ! this next file is generated by bce.maple
 ! results from bcExtended3d4.maple
 ! Assign values on the extended boundary next to two PEC boundaries
 !                                                                  
 ! Here we assume the following are defined                               
 !    c11,c22,c33,c1,c2,c3                                          
 !    urr,uss,utt,ur,us,ut (also for v and w)                       
 !    deltaFu,deltaFv,deltaFw = RHS for Delta(u,v,w)                
 !    g1f,g2f = RHS for extrapolation, a1.D+2^4u(i1,i2-2)=g1f, a2.D+2^4u(i1-2,i2)=g2f,    
 !                                                                  
      DeltaU = c11*urr+c22*uss+c33*utt+c1*ur+c2*us+c3*ut - deltaFu
      DeltaV = c11*vrr+c22*vss+c33*vtt+c1*vr+c2*vs+c3*vt - deltaFv
      DeltaW = c11*wrr+c22*wss+c33*wtt+c1*wr+c2*ws+c3*wt - deltaFw

! ** decompose point u(i1-is1,i2-is2,i3-is3) into components along a1,a2,a3 **
      a11c=(rsxy(i1-is1,i2-is2,i3-is3,axis,0)/(rx(i1-is1,i2-is2,i3-is3)
     & *(sy(i1-is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3)-sz(i1-is1,
     & i2-is2,i3-is3)*ty(i1-is1,i2-is2,i3-is3))+ry(i1-is1,i2-is2,i3-
     & is3)*(sz(i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-is3)-sx(i1-
     & is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3))+rz(i1-is1,i2-is2,
     & i3-is3)*(sx(i1-is1,i2-is2,i3-is3)*ty(i1-is1,i2-is2,i3-is3)-sy(
     & i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-is3))))
      a12c=(rsxy(i1-is1,i2-is2,i3-is3,axis,1)/(rx(i1-is1,i2-is2,i3-is3)
     & *(sy(i1-is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3)-sz(i1-is1,
     & i2-is2,i3-is3)*ty(i1-is1,i2-is2,i3-is3))+ry(i1-is1,i2-is2,i3-
     & is3)*(sz(i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-is3)-sx(i1-
     & is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3))+rz(i1-is1,i2-is2,
     & i3-is3)*(sx(i1-is1,i2-is2,i3-is3)*ty(i1-is1,i2-is2,i3-is3)-sy(
     & i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-is3))))
      a13c=(rsxy(i1-is1,i2-is2,i3-is3,axis,2)/(rx(i1-is1,i2-is2,i3-is3)
     & *(sy(i1-is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3)-sz(i1-is1,
     & i2-is2,i3-is3)*ty(i1-is1,i2-is2,i3-is3))+ry(i1-is1,i2-is2,i3-
     & is3)*(sz(i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-is3)-sx(i1-
     & is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3))+rz(i1-is1,i2-is2,
     & i3-is3)*(sx(i1-is1,i2-is2,i3-is3)*ty(i1-is1,i2-is2,i3-is3)-sy(
     & i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-is3))))
      a21c=(rsxy(i1-is1,i2-is2,i3-is3,axisp1,0)/(rx(i1-is1,i2-is2,i3-
     & is3)*(sy(i1-is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3)-sz(i1-
     & is1,i2-is2,i3-is3)*ty(i1-is1,i2-is2,i3-is3))+ry(i1-is1,i2-is2,
     & i3-is3)*(sz(i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-is3)-sx(
     & i1-is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3))+rz(i1-is1,i2-
     & is2,i3-is3)*(sx(i1-is1,i2-is2,i3-is3)*ty(i1-is1,i2-is2,i3-is3)-
     & sy(i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-is3))))
      a22c=(rsxy(i1-is1,i2-is2,i3-is3,axisp1,1)/(rx(i1-is1,i2-is2,i3-
     & is3)*(sy(i1-is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3)-sz(i1-
     & is1,i2-is2,i3-is3)*ty(i1-is1,i2-is2,i3-is3))+ry(i1-is1,i2-is2,
     & i3-is3)*(sz(i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-is3)-sx(
     & i1-is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3))+rz(i1-is1,i2-
     & is2,i3-is3)*(sx(i1-is1,i2-is2,i3-is3)*ty(i1-is1,i2-is2,i3-is3)-
     & sy(i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-is3))))
      a23c=(rsxy(i1-is1,i2-is2,i3-is3,axisp1,2)/(rx(i1-is1,i2-is2,i3-
     & is3)*(sy(i1-is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3)-sz(i1-
     & is1,i2-is2,i3-is3)*ty(i1-is1,i2-is2,i3-is3))+ry(i1-is1,i2-is2,
     & i3-is3)*(sz(i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-is3)-sx(
     & i1-is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3))+rz(i1-is1,i2-
     & is2,i3-is3)*(sx(i1-is1,i2-is2,i3-is3)*ty(i1-is1,i2-is2,i3-is3)-
     & sy(i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-is3))))
      a31c=(rsxy(i1-is1,i2-is2,i3-is3,axisp2,0)/(rx(i1-is1,i2-is2,i3-
     & is3)*(sy(i1-is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3)-sz(i1-
     & is1,i2-is2,i3-is3)*ty(i1-is1,i2-is2,i3-is3))+ry(i1-is1,i2-is2,
     & i3-is3)*(sz(i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-is3)-sx(
     & i1-is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3))+rz(i1-is1,i2-
     & is2,i3-is3)*(sx(i1-is1,i2-is2,i3-is3)*ty(i1-is1,i2-is2,i3-is3)-
     & sy(i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-is3))))
      a32c=(rsxy(i1-is1,i2-is2,i3-is3,axisp2,1)/(rx(i1-is1,i2-is2,i3-
     & is3)*(sy(i1-is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3)-sz(i1-
     & is1,i2-is2,i3-is3)*ty(i1-is1,i2-is2,i3-is3))+ry(i1-is1,i2-is2,
     & i3-is3)*(sz(i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-is3)-sx(
     & i1-is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3))+rz(i1-is1,i2-
     & is2,i3-is3)*(sx(i1-is1,i2-is2,i3-is3)*ty(i1-is1,i2-is2,i3-is3)-
     & sy(i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-is3))))
      a33c=(rsxy(i1-is1,i2-is2,i3-is3,axisp2,2)/(rx(i1-is1,i2-is2,i3-
     & is3)*(sy(i1-is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3)-sz(i1-
     & is1,i2-is2,i3-is3)*ty(i1-is1,i2-is2,i3-is3))+ry(i1-is1,i2-is2,
     & i3-is3)*(sz(i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-is3)-sx(
     & i1-is1,i2-is2,i3-is3)*tz(i1-is1,i2-is2,i3-is3))+rz(i1-is1,i2-
     & is2,i3-is3)*(sx(i1-is1,i2-is2,i3-is3)*ty(i1-is1,i2-is2,i3-is3)-
     & sy(i1-is1,i2-is2,i3-is3)*tx(i1-is1,i2-is2,i3-is3))))

      a1a1=a11c*a11c+a12c*a12c+a13c*a13c
      a1a2=a11c*a21c+a12c*a22c+a13c*a23c
      a1a3=a11c*a31c+a12c*a32c+a13c*a33c
      a2a3=a21c*a31c+a22c*a32c+a23c*a33c
      a3a3=a31c*a31c+a32c*a32c+a33c*a33c
! The tangential component is assumed valid:
      a1Dotu1=a11c*u(i1-is1,i2-is2,i3-is3,ex)+a12c*u(i1-is1,i2-is2,i3-
     & is3,ey)+a13c*u(i1-is1,i2-is2,i3-is3,ez)
      a3Dotu1=a31c*u(i1-is1,i2-is2,i3-is3,ex)+a32c*u(i1-is1,i2-is2,i3-
     & is3,ey)+a33c*u(i1-is1,i2-is2,i3-is3,ez)
 ! u(i1-is1,i2-is2,i3-is3,k) = b1[k]*x1 +g1[k]
      b11 =-a11c*(-a1a2*a3a3+a1a3*a2a3)/(-a1a1*a3a3+a1a3**2)+a21c-a31c*
     & (-a1a1*a2a3+a1a3*a1a2)/(-a1a1*a3a3+a1a3**2)
      b12 =-a12c*(-a1a2*a3a3+a1a3*a2a3)/(-a1a1*a3a3+a1a3**2)+a22c-a32c*
     & (-a1a1*a2a3+a1a3*a1a2)/(-a1a1*a3a3+a1a3**2)
      b13 =-a13c*(-a1a2*a3a3+a1a3*a2a3)/(-a1a1*a3a3+a1a3**2)+a23c-a33c*
     & (-a1a1*a2a3+a1a3*a1a2)/(-a1a1*a3a3+a1a3**2)
      g11 =-(-a11c*a1a3*a3Dotu1+a11c*a1Dotu1*a3a3-a31c*a1a3*a1Dotu1+
     & a31c*a1a1*a3Dotu1)/(-a1a1*a3a3+a1a3**2)
      g12 =-(-a12c*a1a3*a3Dotu1+a12c*a1Dotu1*a3a3-a32c*a1a3*a1Dotu1+
     & a32c*a1a1*a3Dotu1)/(-a1a1*a3a3+a1a3**2)
      g13 =(a13c*a1a3*a3Dotu1-a13c*a1Dotu1*a3a3+a33c*a1a3*a1Dotu1-a33c*
     & a1a1*a3Dotu1)/(-a1a1*a3a3+a1a3**2)

! ** decompose point u(i1-2*is1,i2-2*is2,i3-2*is3) into components along a1,a2,a3 **
      a11c=(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,0)/(rx(i1-2*is1,i2-2*
     & is2,i3-2*is3)*(sy(i1-2*is1,i2-2*is2,i3-2*is3)*tz(i1-2*is1,i2-2*
     & is2,i3-2*is3)-sz(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-2*is1,i2-2*
     & is2,i3-2*is3))+ry(i1-2*is1,i2-2*is2,i3-2*is3)*(sz(i1-2*is1,i2-
     & 2*is2,i3-2*is3)*tx(i1-2*is1,i2-2*is2,i3-2*is3)-sx(i1-2*is1,i2-
     & 2*is2,i3-2*is3)*tz(i1-2*is1,i2-2*is2,i3-2*is3))+rz(i1-2*is1,i2-
     & 2*is2,i3-2*is3)*(sx(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-2*is1,i2-
     & 2*is2,i3-2*is3)-sy(i1-2*is1,i2-2*is2,i3-2*is3)*tx(i1-2*is1,i2-
     & 2*is2,i3-2*is3))))
      a12c=(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,1)/(rx(i1-2*is1,i2-2*
     & is2,i3-2*is3)*(sy(i1-2*is1,i2-2*is2,i3-2*is3)*tz(i1-2*is1,i2-2*
     & is2,i3-2*is3)-sz(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-2*is1,i2-2*
     & is2,i3-2*is3))+ry(i1-2*is1,i2-2*is2,i3-2*is3)*(sz(i1-2*is1,i2-
     & 2*is2,i3-2*is3)*tx(i1-2*is1,i2-2*is2,i3-2*is3)-sx(i1-2*is1,i2-
     & 2*is2,i3-2*is3)*tz(i1-2*is1,i2-2*is2,i3-2*is3))+rz(i1-2*is1,i2-
     & 2*is2,i3-2*is3)*(sx(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-2*is1,i2-
     & 2*is2,i3-2*is3)-sy(i1-2*is1,i2-2*is2,i3-2*is3)*tx(i1-2*is1,i2-
     & 2*is2,i3-2*is3))))
      a13c=(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,2)/(rx(i1-2*is1,i2-2*
     & is2,i3-2*is3)*(sy(i1-2*is1,i2-2*is2,i3-2*is3)*tz(i1-2*is1,i2-2*
     & is2,i3-2*is3)-sz(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-2*is1,i2-2*
     & is2,i3-2*is3))+ry(i1-2*is1,i2-2*is2,i3-2*is3)*(sz(i1-2*is1,i2-
     & 2*is2,i3-2*is3)*tx(i1-2*is1,i2-2*is2,i3-2*is3)-sx(i1-2*is1,i2-
     & 2*is2,i3-2*is3)*tz(i1-2*is1,i2-2*is2,i3-2*is3))+rz(i1-2*is1,i2-
     & 2*is2,i3-2*is3)*(sx(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-2*is1,i2-
     & 2*is2,i3-2*is3)-sy(i1-2*is1,i2-2*is2,i3-2*is3)*tx(i1-2*is1,i2-
     & 2*is2,i3-2*is3))))
      a21c=(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axisp1,0)/(rx(i1-2*is1,i2-
     & 2*is2,i3-2*is3)*(sy(i1-2*is1,i2-2*is2,i3-2*is3)*tz(i1-2*is1,i2-
     & 2*is2,i3-2*is3)-sz(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-2*is1,i2-
     & 2*is2,i3-2*is3))+ry(i1-2*is1,i2-2*is2,i3-2*is3)*(sz(i1-2*is1,
     & i2-2*is2,i3-2*is3)*tx(i1-2*is1,i2-2*is2,i3-2*is3)-sx(i1-2*is1,
     & i2-2*is2,i3-2*is3)*tz(i1-2*is1,i2-2*is2,i3-2*is3))+rz(i1-2*is1,
     & i2-2*is2,i3-2*is3)*(sx(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-2*is1,
     & i2-2*is2,i3-2*is3)-sy(i1-2*is1,i2-2*is2,i3-2*is3)*tx(i1-2*is1,
     & i2-2*is2,i3-2*is3))))
      a22c=(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axisp1,1)/(rx(i1-2*is1,i2-
     & 2*is2,i3-2*is3)*(sy(i1-2*is1,i2-2*is2,i3-2*is3)*tz(i1-2*is1,i2-
     & 2*is2,i3-2*is3)-sz(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-2*is1,i2-
     & 2*is2,i3-2*is3))+ry(i1-2*is1,i2-2*is2,i3-2*is3)*(sz(i1-2*is1,
     & i2-2*is2,i3-2*is3)*tx(i1-2*is1,i2-2*is2,i3-2*is3)-sx(i1-2*is1,
     & i2-2*is2,i3-2*is3)*tz(i1-2*is1,i2-2*is2,i3-2*is3))+rz(i1-2*is1,
     & i2-2*is2,i3-2*is3)*(sx(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-2*is1,
     & i2-2*is2,i3-2*is3)-sy(i1-2*is1,i2-2*is2,i3-2*is3)*tx(i1-2*is1,
     & i2-2*is2,i3-2*is3))))
      a23c=(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axisp1,2)/(rx(i1-2*is1,i2-
     & 2*is2,i3-2*is3)*(sy(i1-2*is1,i2-2*is2,i3-2*is3)*tz(i1-2*is1,i2-
     & 2*is2,i3-2*is3)-sz(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-2*is1,i2-
     & 2*is2,i3-2*is3))+ry(i1-2*is1,i2-2*is2,i3-2*is3)*(sz(i1-2*is1,
     & i2-2*is2,i3-2*is3)*tx(i1-2*is1,i2-2*is2,i3-2*is3)-sx(i1-2*is1,
     & i2-2*is2,i3-2*is3)*tz(i1-2*is1,i2-2*is2,i3-2*is3))+rz(i1-2*is1,
     & i2-2*is2,i3-2*is3)*(sx(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-2*is1,
     & i2-2*is2,i3-2*is3)-sy(i1-2*is1,i2-2*is2,i3-2*is3)*tx(i1-2*is1,
     & i2-2*is2,i3-2*is3))))
      a31c=(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axisp2,0)/(rx(i1-2*is1,i2-
     & 2*is2,i3-2*is3)*(sy(i1-2*is1,i2-2*is2,i3-2*is3)*tz(i1-2*is1,i2-
     & 2*is2,i3-2*is3)-sz(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-2*is1,i2-
     & 2*is2,i3-2*is3))+ry(i1-2*is1,i2-2*is2,i3-2*is3)*(sz(i1-2*is1,
     & i2-2*is2,i3-2*is3)*tx(i1-2*is1,i2-2*is2,i3-2*is3)-sx(i1-2*is1,
     & i2-2*is2,i3-2*is3)*tz(i1-2*is1,i2-2*is2,i3-2*is3))+rz(i1-2*is1,
     & i2-2*is2,i3-2*is3)*(sx(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-2*is1,
     & i2-2*is2,i3-2*is3)-sy(i1-2*is1,i2-2*is2,i3-2*is3)*tx(i1-2*is1,
     & i2-2*is2,i3-2*is3))))
      a32c=(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axisp2,1)/(rx(i1-2*is1,i2-
     & 2*is2,i3-2*is3)*(sy(i1-2*is1,i2-2*is2,i3-2*is3)*tz(i1-2*is1,i2-
     & 2*is2,i3-2*is3)-sz(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-2*is1,i2-
     & 2*is2,i3-2*is3))+ry(i1-2*is1,i2-2*is2,i3-2*is3)*(sz(i1-2*is1,
     & i2-2*is2,i3-2*is3)*tx(i1-2*is1,i2-2*is2,i3-2*is3)-sx(i1-2*is1,
     & i2-2*is2,i3-2*is3)*tz(i1-2*is1,i2-2*is2,i3-2*is3))+rz(i1-2*is1,
     & i2-2*is2,i3-2*is3)*(sx(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-2*is1,
     & i2-2*is2,i3-2*is3)-sy(i1-2*is1,i2-2*is2,i3-2*is3)*tx(i1-2*is1,
     & i2-2*is2,i3-2*is3))))
      a33c=(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axisp2,2)/(rx(i1-2*is1,i2-
     & 2*is2,i3-2*is3)*(sy(i1-2*is1,i2-2*is2,i3-2*is3)*tz(i1-2*is1,i2-
     & 2*is2,i3-2*is3)-sz(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-2*is1,i2-
     & 2*is2,i3-2*is3))+ry(i1-2*is1,i2-2*is2,i3-2*is3)*(sz(i1-2*is1,
     & i2-2*is2,i3-2*is3)*tx(i1-2*is1,i2-2*is2,i3-2*is3)-sx(i1-2*is1,
     & i2-2*is2,i3-2*is3)*tz(i1-2*is1,i2-2*is2,i3-2*is3))+rz(i1-2*is1,
     & i2-2*is2,i3-2*is3)*(sx(i1-2*is1,i2-2*is2,i3-2*is3)*ty(i1-2*is1,
     & i2-2*is2,i3-2*is3)-sy(i1-2*is1,i2-2*is2,i3-2*is3)*tx(i1-2*is1,
     & i2-2*is2,i3-2*is3))))

      a1a1=a11c*a11c+a12c*a12c+a13c*a13c
      a1a2=a11c*a21c+a12c*a22c+a13c*a23c
      a1a3=a11c*a31c+a12c*a32c+a13c*a33c
      a2a3=a21c*a31c+a22c*a32c+a23c*a33c
      a3a3=a31c*a31c+a32c*a32c+a33c*a33c
! The tangential component is assumed valid:
      a1Dotu2=a11c*u(i1-2*is1,i2-2*is2,i3-2*is3,ex)+a12c*u(i1-2*is1,i2-
     & 2*is2,i3-2*is3,ey)+a13c*u(i1-2*is1,i2-2*is2,i3-2*is3,ez)
      a3Dotu2=a31c*u(i1-2*is1,i2-2*is2,i3-2*is3,ex)+a32c*u(i1-2*is1,i2-
     & 2*is2,i3-2*is3,ey)+a33c*u(i1-2*is1,i2-2*is2,i3-2*is3,ez)
 ! u(i1-2*is1,i2-2*is2,i3-2*is3,k) = b2[k]*x2 +g2[k]
      b21 =-a11c*(-a1a2*a3a3+a1a3*a2a3)/(-a1a1*a3a3+a1a3**2)+a21c-a31c*
     & (-a1a1*a2a3+a1a3*a1a2)/(-a1a1*a3a3+a1a3**2)
      b22 =-a12c*(-a1a2*a3a3+a1a3*a2a3)/(-a1a1*a3a3+a1a3**2)+a22c-a32c*
     & (-a1a1*a2a3+a1a3*a1a2)/(-a1a1*a3a3+a1a3**2)
      b23 =-a13c*(-a1a2*a3a3+a1a3*a2a3)/(-a1a1*a3a3+a1a3**2)+a23c-a33c*
     & (-a1a1*a2a3+a1a3*a1a2)/(-a1a1*a3a3+a1a3**2)
      g21 =(a11c*a1a3*a3Dotu2-a11c*a1Dotu2*a3a3+a31c*a1a3*a1Dotu2-a31c*
     & a1a1*a3Dotu2)/(-a1a1*a3a3+a1a3**2)
      g22 =(a12c*a1a3*a3Dotu2-a12c*a1Dotu2*a3a3+a32c*a1a3*a1Dotu2-a32c*
     & a1a1*a3Dotu2)/(-a1a1*a3a3+a1a3**2)
      g23 =(a13c*a1a3*a3Dotu2-a13c*a1Dotu2*a3a3+a33c*a1a3*a1Dotu2-a33c*
     & a1a1*a3Dotu2)/(-a1a1*a3a3+a1a3**2)

! ** decompose point u(i1-js1,i2-js2,i3-js3) into components along a1,a2,a3 **
      a11c=(rsxy(i1-js1,i2-js2,i3-js3,axis,0)/(rx(i1-js1,i2-js2,i3-js3)
     & *(sy(i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3)-sz(i1-js1,
     & i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3))+ry(i1-js1,i2-js2,i3-
     & js3)*(sz(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3)-sx(i1-
     & js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3))+rz(i1-js1,i2-js2,
     & i3-js3)*(sx(i1-js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3)-sy(
     & i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3))))
      a12c=(rsxy(i1-js1,i2-js2,i3-js3,axis,1)/(rx(i1-js1,i2-js2,i3-js3)
     & *(sy(i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3)-sz(i1-js1,
     & i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3))+ry(i1-js1,i2-js2,i3-
     & js3)*(sz(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3)-sx(i1-
     & js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3))+rz(i1-js1,i2-js2,
     & i3-js3)*(sx(i1-js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3)-sy(
     & i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3))))
      a13c=(rsxy(i1-js1,i2-js2,i3-js3,axis,2)/(rx(i1-js1,i2-js2,i3-js3)
     & *(sy(i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3)-sz(i1-js1,
     & i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3))+ry(i1-js1,i2-js2,i3-
     & js3)*(sz(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3)-sx(i1-
     & js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3))+rz(i1-js1,i2-js2,
     & i3-js3)*(sx(i1-js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3)-sy(
     & i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3))))
      a21c=(rsxy(i1-js1,i2-js2,i3-js3,axisp1,0)/(rx(i1-js1,i2-js2,i3-
     & js3)*(sy(i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3)-sz(i1-
     & js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3))+ry(i1-js1,i2-js2,
     & i3-js3)*(sz(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3)-sx(
     & i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3))+rz(i1-js1,i2-
     & js2,i3-js3)*(sx(i1-js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3)-
     & sy(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3))))
      a22c=(rsxy(i1-js1,i2-js2,i3-js3,axisp1,1)/(rx(i1-js1,i2-js2,i3-
     & js3)*(sy(i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3)-sz(i1-
     & js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3))+ry(i1-js1,i2-js2,
     & i3-js3)*(sz(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3)-sx(
     & i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3))+rz(i1-js1,i2-
     & js2,i3-js3)*(sx(i1-js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3)-
     & sy(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3))))
      a23c=(rsxy(i1-js1,i2-js2,i3-js3,axisp1,2)/(rx(i1-js1,i2-js2,i3-
     & js3)*(sy(i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3)-sz(i1-
     & js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3))+ry(i1-js1,i2-js2,
     & i3-js3)*(sz(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3)-sx(
     & i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3))+rz(i1-js1,i2-
     & js2,i3-js3)*(sx(i1-js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3)-
     & sy(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3))))
      a31c=(rsxy(i1-js1,i2-js2,i3-js3,axisp2,0)/(rx(i1-js1,i2-js2,i3-
     & js3)*(sy(i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3)-sz(i1-
     & js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3))+ry(i1-js1,i2-js2,
     & i3-js3)*(sz(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3)-sx(
     & i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3))+rz(i1-js1,i2-
     & js2,i3-js3)*(sx(i1-js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3)-
     & sy(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3))))
      a32c=(rsxy(i1-js1,i2-js2,i3-js3,axisp2,1)/(rx(i1-js1,i2-js2,i3-
     & js3)*(sy(i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3)-sz(i1-
     & js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3))+ry(i1-js1,i2-js2,
     & i3-js3)*(sz(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3)-sx(
     & i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3))+rz(i1-js1,i2-
     & js2,i3-js3)*(sx(i1-js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3)-
     & sy(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3))))
      a33c=(rsxy(i1-js1,i2-js2,i3-js3,axisp2,2)/(rx(i1-js1,i2-js2,i3-
     & js3)*(sy(i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3)-sz(i1-
     & js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3))+ry(i1-js1,i2-js2,
     & i3-js3)*(sz(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3)-sx(
     & i1-js1,i2-js2,i3-js3)*tz(i1-js1,i2-js2,i3-js3))+rz(i1-js1,i2-
     & js2,i3-js3)*(sx(i1-js1,i2-js2,i3-js3)*ty(i1-js1,i2-js2,i3-js3)-
     & sy(i1-js1,i2-js2,i3-js3)*tx(i1-js1,i2-js2,i3-js3))))

      a2a2=a21c*a21c+a22c*a22c+a23c*a23c
      a1a2=a11c*a21c+a12c*a22c+a13c*a23c
      a1a3=a11c*a31c+a12c*a32c+a13c*a33c
      a2a3=a21c*a31c+a22c*a32c+a23c*a33c
      a3a3=a31c*a31c+a32c*a32c+a33c*a33c
! The tangential component is assumed valid:
      a2Dotu3=a21c*u(i1-js1,i2-js2,i3-js3,ex)+a22c*u(i1-js1,i2-js2,i3-
     & js3,ey)+a23c*u(i1-js1,i2-js2,i3-js3,ez)
      a3Dotu3=a31c*u(i1-js1,i2-js2,i3-js3,ex)+a32c*u(i1-js1,i2-js2,i3-
     & js3,ey)+a33c*u(i1-js1,i2-js2,i3-js3,ez)
 ! u(i1-js1,i2-js2,i3-js3,k) = b3[k]*x3 +g3[k]
      b31 =a11c-a21c/(a2a3**2-a3a3*a2a2)*(-a1a2*a3a3+a1a3*a2a3)+a31c*(-
     & a1a2*a2a3+a2a2*a1a3)/(a2a3**2-a3a3*a2a2)
      b32 =a12c-a22c/(a2a3**2-a3a3*a2a2)*(-a1a2*a3a3+a1a3*a2a3)+a32c*(-
     & a1a2*a2a3+a2a2*a1a3)/(a2a3**2-a3a3*a2a2)
      b33 =a13c-a23c/(a2a3**2-a3a3*a2a2)*(-a1a2*a3a3+a1a3*a2a3)+a33c*(-
     & a1a2*a2a3+a2a2*a1a3)/(a2a3**2-a3a3*a2a2)
      g31 =(-a21c*a3a3*a2Dotu3+a21c*a2a3*a3Dotu3-a31c*a2a2*a3Dotu3+
     & a31c*a2Dotu3*a2a3)/(a2a3**2-a3a3*a2a2)
      g32 =(-a22c*a3a3*a2Dotu3+a22c*a2a3*a3Dotu3-a32c*a2a2*a3Dotu3+
     & a32c*a2Dotu3*a2a3)/(a2a3**2-a3a3*a2a2)
      g33 =(-a23c*a3a3*a2Dotu3+a23c*a2a3*a3Dotu3-a33c*a2a2*a3Dotu3+
     & a33c*a2Dotu3*a2a3)/(a2a3**2-a3a3*a2a2)

! ** decompose point u(i1-2*js1,i2-2*js2,i3-2*js3) into components along a1,a2,a3 **
      a11c=(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axis,0)/(rx(i1-2*js1,i2-2*
     & js2,i3-2*js3)*(sy(i1-2*js1,i2-2*js2,i3-2*js3)*tz(i1-2*js1,i2-2*
     & js2,i3-2*js3)-sz(i1-2*js1,i2-2*js2,i3-2*js3)*ty(i1-2*js1,i2-2*
     & js2,i3-2*js3))+ry(i1-2*js1,i2-2*js2,i3-2*js3)*(sz(i1-2*js1,i2-
     & 2*js2,i3-2*js3)*tx(i1-2*js1,i2-2*js2,i3-2*js3)-sx(i1-2*js1,i2-
     & 2*js2,i3-2*js3)*tz(i1-2*js1,i2-2*js2,i3-2*js3))+rz(i1-2*js1,i2-
     & 2*js2,i3-2*js3)*(sx(i1-2*js1,i2-2*js2,i3-2*js3)*ty(i1-2*js1,i2-
     & 2*js2,i3-2*js3)-sy(i1-2*js1,i2-2*js2,i3-2*js3)*tx(i1-2*js1,i2-
     & 2*js2,i3-2*js3))))
      a12c=(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axis,1)/(rx(i1-2*js1,i2-2*
     & js2,i3-2*js3)*(sy(i1-2*js1,i2-2*js2,i3-2*js3)*tz(i1-2*js1,i2-2*
     & js2,i3-2*js3)-sz(i1-2*js1,i2-2*js2,i3-2*js3)*ty(i1-2*js1,i2-2*
     & js2,i3-2*js3))+ry(i1-2*js1,i2-2*js2,i3-2*js3)*(sz(i1-2*js1,i2-
     & 2*js2,i3-2*js3)*tx(i1-2*js1,i2-2*js2,i3-2*js3)-sx(i1-2*js1,i2-
     & 2*js2,i3-2*js3)*tz(i1-2*js1,i2-2*js2,i3-2*js3))+rz(i1-2*js1,i2-
     & 2*js2,i3-2*js3)*(sx(i1-2*js1,i2-2*js2,i3-2*js3)*ty(i1-2*js1,i2-
     & 2*js2,i3-2*js3)-sy(i1-2*js1,i2-2*js2,i3-2*js3)*tx(i1-2*js1,i2-
     & 2*js2,i3-2*js3))))
      a13c=(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axis,2)/(rx(i1-2*js1,i2-2*
     & js2,i3-2*js3)*(sy(i1-2*js1,i2-2*js2,i3-2*js3)*tz(i1-2*js1,i2-2*
     & js2,i3-2*js3)-sz(i1-2*js1,i2-2*js2,i3-2*js3)*ty(i1-2*js1,i2-2*
     & js2,i3-2*js3))+ry(i1-2*js1,i2-2*js2,i3-2*js3)*(sz(i1-2*js1,i2-
     & 2*js2,i3-2*js3)*tx(i1-2*js1,i2-2*js2,i3-2*js3)-sx(i1-2*js1,i2-
     & 2*js2,i3-2*js3)*tz(i1-2*js1,i2-2*js2,i3-2*js3))+rz(i1-2*js1,i2-
     & 2*js2,i3-2*js3)*(sx(i1-2*js1,i2-2*js2,i3-2*js3)*ty(i1-2*js1,i2-
     & 2*js2,i3-2*js3)-sy(i1-2*js1,i2-2*js2,i3-2*js3)*tx(i1-2*js1,i2-
     & 2*js2,i3-2*js3))))
      a21c=(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axisp1,0)/(rx(i1-2*js1,i2-
     & 2*js2,i3-2*js3)*(sy(i1-2*js1,i2-2*js2,i3-2*js3)*tz(i1-2*js1,i2-
     & 2*js2,i3-2*js3)-sz(i1-2*js1,i2-2*js2,i3-2*js3)*ty(i1-2*js1,i2-
     & 2*js2,i3-2*js3))+ry(i1-2*js1,i2-2*js2,i3-2*js3)*(sz(i1-2*js1,
     & i2-2*js2,i3-2*js3)*tx(i1-2*js1,i2-2*js2,i3-2*js3)-sx(i1-2*js1,
     & i2-2*js2,i3-2*js3)*tz(i1-2*js1,i2-2*js2,i3-2*js3))+rz(i1-2*js1,
     & i2-2*js2,i3-2*js3)*(sx(i1-2*js1,i2-2*js2,i3-2*js3)*ty(i1-2*js1,
     & i2-2*js2,i3-2*js3)-sy(i1-2*js1,i2-2*js2,i3-2*js3)*tx(i1-2*js1,
     & i2-2*js2,i3-2*js3))))
      a22c=(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axisp1,1)/(rx(i1-2*js1,i2-
     & 2*js2,i3-2*js3)*(sy(i1-2*js1,i2-2*js2,i3-2*js3)*tz(i1-2*js1,i2-
     & 2*js2,i3-2*js3)-sz(i1-2*js1,i2-2*js2,i3-2*js3)*ty(i1-2*js1,i2-
     & 2*js2,i3-2*js3))+ry(i1-2*js1,i2-2*js2,i3-2*js3)*(sz(i1-2*js1,
     & i2-2*js2,i3-2*js3)*tx(i1-2*js1,i2-2*js2,i3-2*js3)-sx(i1-2*js1,
     & i2-2*js2,i3-2*js3)*tz(i1-2*js1,i2-2*js2,i3-2*js3))+rz(i1-2*js1,
     & i2-2*js2,i3-2*js3)*(sx(i1-2*js1,i2-2*js2,i3-2*js3)*ty(i1-2*js1,
     & i2-2*js2,i3-2*js3)-sy(i1-2*js1,i2-2*js2,i3-2*js3)*tx(i1-2*js1,
     & i2-2*js2,i3-2*js3))))
      a23c=(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axisp1,2)/(rx(i1-2*js1,i2-
     & 2*js2,i3-2*js3)*(sy(i1-2*js1,i2-2*js2,i3-2*js3)*tz(i1-2*js1,i2-
     & 2*js2,i3-2*js3)-sz(i1-2*js1,i2-2*js2,i3-2*js3)*ty(i1-2*js1,i2-
     & 2*js2,i3-2*js3))+ry(i1-2*js1,i2-2*js2,i3-2*js3)*(sz(i1-2*js1,
     & i2-2*js2,i3-2*js3)*tx(i1-2*js1,i2-2*js2,i3-2*js3)-sx(i1-2*js1,
     & i2-2*js2,i3-2*js3)*tz(i1-2*js1,i2-2*js2,i3-2*js3))+rz(i1-2*js1,
     & i2-2*js2,i3-2*js3)*(sx(i1-2*js1,i2-2*js2,i3-2*js3)*ty(i1-2*js1,
     & i2-2*js2,i3-2*js3)-sy(i1-2*js1,i2-2*js2,i3-2*js3)*tx(i1-2*js1,
     & i2-2*js2,i3-2*js3))))
      a31c=(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axisp2,0)/(rx(i1-2*js1,i2-
     & 2*js2,i3-2*js3)*(sy(i1-2*js1,i2-2*js2,i3-2*js3)*tz(i1-2*js1,i2-
     & 2*js2,i3-2*js3)-sz(i1-2*js1,i2-2*js2,i3-2*js3)*ty(i1-2*js1,i2-
     & 2*js2,i3-2*js3))+ry(i1-2*js1,i2-2*js2,i3-2*js3)*(sz(i1-2*js1,
     & i2-2*js2,i3-2*js3)*tx(i1-2*js1,i2-2*js2,i3-2*js3)-sx(i1-2*js1,
     & i2-2*js2,i3-2*js3)*tz(i1-2*js1,i2-2*js2,i3-2*js3))+rz(i1-2*js1,
     & i2-2*js2,i3-2*js3)*(sx(i1-2*js1,i2-2*js2,i3-2*js3)*ty(i1-2*js1,
     & i2-2*js2,i3-2*js3)-sy(i1-2*js1,i2-2*js2,i3-2*js3)*tx(i1-2*js1,
     & i2-2*js2,i3-2*js3))))
      a32c=(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axisp2,1)/(rx(i1-2*js1,i2-
     & 2*js2,i3-2*js3)*(sy(i1-2*js1,i2-2*js2,i3-2*js3)*tz(i1-2*js1,i2-
     & 2*js2,i3-2*js3)-sz(i1-2*js1,i2-2*js2,i3-2*js3)*ty(i1-2*js1,i2-
     & 2*js2,i3-2*js3))+ry(i1-2*js1,i2-2*js2,i3-2*js3)*(sz(i1-2*js1,
     & i2-2*js2,i3-2*js3)*tx(i1-2*js1,i2-2*js2,i3-2*js3)-sx(i1-2*js1,
     & i2-2*js2,i3-2*js3)*tz(i1-2*js1,i2-2*js2,i3-2*js3))+rz(i1-2*js1,
     & i2-2*js2,i3-2*js3)*(sx(i1-2*js1,i2-2*js2,i3-2*js3)*ty(i1-2*js1,
     & i2-2*js2,i3-2*js3)-sy(i1-2*js1,i2-2*js2,i3-2*js3)*tx(i1-2*js1,
     & i2-2*js2,i3-2*js3))))
      a33c=(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axisp2,2)/(rx(i1-2*js1,i2-
     & 2*js2,i3-2*js3)*(sy(i1-2*js1,i2-2*js2,i3-2*js3)*tz(i1-2*js1,i2-
     & 2*js2,i3-2*js3)-sz(i1-2*js1,i2-2*js2,i3-2*js3)*ty(i1-2*js1,i2-
     & 2*js2,i3-2*js3))+ry(i1-2*js1,i2-2*js2,i3-2*js3)*(sz(i1-2*js1,
     & i2-2*js2,i3-2*js3)*tx(i1-2*js1,i2-2*js2,i3-2*js3)-sx(i1-2*js1,
     & i2-2*js2,i3-2*js3)*tz(i1-2*js1,i2-2*js2,i3-2*js3))+rz(i1-2*js1,
     & i2-2*js2,i3-2*js3)*(sx(i1-2*js1,i2-2*js2,i3-2*js3)*ty(i1-2*js1,
     & i2-2*js2,i3-2*js3)-sy(i1-2*js1,i2-2*js2,i3-2*js3)*tx(i1-2*js1,
     & i2-2*js2,i3-2*js3))))

      a2a2=a21c*a21c+a22c*a22c+a23c*a23c
      a1a2=a11c*a21c+a12c*a22c+a13c*a23c
      a1a3=a11c*a31c+a12c*a32c+a13c*a33c
      a2a3=a21c*a31c+a22c*a32c+a23c*a33c
      a3a3=a31c*a31c+a32c*a32c+a33c*a33c
! The tangential component is assumed valid:
      a2Dotu4=a21c*u(i1-2*js1,i2-2*js2,i3-2*js3,ex)+a22c*u(i1-2*js1,i2-
     & 2*js2,i3-2*js3,ey)+a23c*u(i1-2*js1,i2-2*js2,i3-2*js3,ez)
      a3Dotu4=a31c*u(i1-2*js1,i2-2*js2,i3-2*js3,ex)+a32c*u(i1-2*js1,i2-
     & 2*js2,i3-2*js3,ey)+a33c*u(i1-2*js1,i2-2*js2,i3-2*js3,ez)
 ! u(i1-2*js1,i2-2*js2,i3-2*js3,k) = b4[k]*x4 +g4[k]
      b41 =a11c-a21c/(a2a3**2-a3a3*a2a2)*(-a1a2*a3a3+a1a3*a2a3)+a31c*(-
     & a1a2*a2a3+a2a2*a1a3)/(a2a3**2-a3a3*a2a2)
      b42 =a12c-a22c/(a2a3**2-a3a3*a2a2)*(-a1a2*a3a3+a1a3*a2a3)+a32c*(-
     & a1a2*a2a3+a2a2*a1a3)/(a2a3**2-a3a3*a2a2)
      b43 =a13c-a23c/(a2a3**2-a3a3*a2a2)*(-a1a2*a3a3+a1a3*a2a3)+a33c*(-
     & a1a2*a2a3+a2a2*a1a3)/(a2a3**2-a3a3*a2a2)
      g41 =-(a21c*a3a3*a2Dotu4-a21c*a3Dotu4*a2a3+a31c*a2a2*a3Dotu4-
     & a31c*a2a3*a2Dotu4)/(a2a3**2-a3a3*a2a2)
      g42 =-(a22c*a3a3*a2Dotu4-a22c*a3Dotu4*a2a3+a32c*a2a2*a3Dotu4-
     & a32c*a2a3*a2Dotu4)/(a2a3**2-a3a3*a2a2)
      g43 =(-a23c*a3a3*a2Dotu4+a23c*a3Dotu4*a2a3-a33c*a2a2*a3Dotu4+
     & a33c*a2a3*a2Dotu4)/(a2a3**2-a3a3*a2a2)

 ! Evaluate a1, a2 and a3 at the corner
      a11=(rsxy(i1,i2,i3,axis,0)/(rx(i1,i2,i3)*(sy(i1,i2,i3)*tz(i1,i2,
     & i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,i2,i3)*tx(
     & i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(sx(i1,i2,i3)
     & *ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))
      a12=(rsxy(i1,i2,i3,axis,1)/(rx(i1,i2,i3)*(sy(i1,i2,i3)*tz(i1,i2,
     & i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,i2,i3)*tx(
     & i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(sx(i1,i2,i3)
     & *ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))
      a13=(rsxy(i1,i2,i3,axis,2)/(rx(i1,i2,i3)*(sy(i1,i2,i3)*tz(i1,i2,
     & i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,i2,i3)*tx(
     & i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(sx(i1,i2,i3)
     & *ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))
      a21=(rsxy(i1,i2,i3,axisp1,0)/(rx(i1,i2,i3)*(sy(i1,i2,i3)*tz(i1,
     & i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,i2,i3)*
     & tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(sx(i1,i2,
     & i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))
      a22=(rsxy(i1,i2,i3,axisp1,1)/(rx(i1,i2,i3)*(sy(i1,i2,i3)*tz(i1,
     & i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,i2,i3)*
     & tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(sx(i1,i2,
     & i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))
      a23=(rsxy(i1,i2,i3,axisp1,2)/(rx(i1,i2,i3)*(sy(i1,i2,i3)*tz(i1,
     & i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,i2,i3)*
     & tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(sx(i1,i2,
     & i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))
      a31=(rsxy(i1,i2,i3,axisp2,0)/(rx(i1,i2,i3)*(sy(i1,i2,i3)*tz(i1,
     & i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,i2,i3)*
     & tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(sx(i1,i2,
     & i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))
      a32=(rsxy(i1,i2,i3,axisp2,1)/(rx(i1,i2,i3)*(sy(i1,i2,i3)*tz(i1,
     & i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,i2,i3)*
     & tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(sx(i1,i2,
     & i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))
      a33=(rsxy(i1,i2,i3,axisp2,2)/(rx(i1,i2,i3)*(sy(i1,i2,i3)*tz(i1,
     & i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))+ry(i1,i2,i3)*(sz(i1,i2,i3)*
     & tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))+rz(i1,i2,i3)*(sx(i1,i2,
     & i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3))))

      a1DotLu=a11*DeltaU+a12*DeltaV+a13*DeltaW
      a2DotLu=a21*DeltaU+a22*DeltaV+a23*DeltaW

!   a1.Lu = 0 
! e1 := cc11a*u(i1-2,i2,i3)+cc12a*v(i1-2,i2,i3)+cc13a*w(i1-2,i2,i3)
!     + cc14a*u(i1-1,i2,i3)+cc15a*v(i1-1,i2,i3)+cc16a*w(i1-1,i2,i3) 
!     + cc11b*u(i1,i2-2,i3)+cc12b*v(i1,i2-2,i3)+cc13b*w(i1,i2-2,i3)
!     + cc14b*u(i1,i2-1,i3)+cc15b*v(i1,i2-1,i3)+cc16b*w(i1,i2-1,i3) - f1:
!  a2.Lu = 0 :
! e2 := cc21a*u(i1-2,i2,i3)+cc22a*v(i1-2,i2,i3)+cc23a*w(i1-2,i2,i3)
!     + cc24a*u(i1-1,i2,i3)+cc25a*v(i1-1,i2,i3)+cc26a*w(i1-1,i2,i3) 
!     + cc21b*u(i1,i2-2,i3)+cc22b*v(i1,i2-2,i3)+cc23b*w(i1,i2-2,i3) 
!     + cc24b*u(i1,i2-1,i3)+cc25b*v(i1,i2-1,i3)+cc26b*w(i1,i2-1,i3) - f2:
      cc11a=(-1/12.*c11/dra**2+1/12.*c1/dra)*a11
      cc12a=(-1/12.*c11/dra**2+1/12.*c1/dra)*a12
      cc13a=(-1/12.*c11/dra**2+1/12.*c1/dra)*a13
      cc14a=(4/3.*c11/dra**2-2/3.*c1/dra)*a11
      cc15a=(4/3.*c11/dra**2-2/3.*c1/dra)*a12
      cc16a=(4/3.*c11/dra**2-2/3.*c1/dra)*a13
      cc11b=(-1/12.*c22/dsa**2+1/12.*c2/dsa)*a11
      cc12b=(-1/12.*c22/dsa**2+1/12.*c2/dsa)*a12
      cc13b=(-1/12.*c22/dsa**2+1/12.*c2/dsa)*a13
      cc14b=(4/3.*c22/dsa**2-2/3.*c2/dsa)*a11
      cc15b=(4/3.*c22/dsa**2-2/3.*c2/dsa)*a12
      cc16b=(4/3.*c22/dsa**2-2/3.*c2/dsa)*a13
      cc21a=(-1/12.*c11/dra**2+1/12.*c1/dra)*a21
      cc22a=(-1/12.*c11/dra**2+1/12.*c1/dra)*a22
      cc23a=(-1/12.*c11/dra**2+1/12.*c1/dra)*a23
      cc24a=(4/3.*c11/dra**2-2/3.*c1/dra)*a21
      cc25a=(4/3.*c11/dra**2-2/3.*c1/dra)*a22
      cc26a=(4/3.*c11/dra**2-2/3.*c1/dra)*a23
      cc21b=(-1/12.*c22/dsa**2+1/12.*c2/dsa)*a21
      cc22b=(-1/12.*c22/dsa**2+1/12.*c2/dsa)*a22
      cc23b=(-1/12.*c22/dsa**2+1/12.*c2/dsa)*a23
      cc24b=(4/3.*c22/dsa**2-2/3.*c2/dsa)*a21
      cc25b=(4/3.*c22/dsa**2-2/3.*c2/dsa)*a22
      cc26b=(4/3.*c22/dsa**2-2/3.*c2/dsa)*a23

      f1=a1DotLu-cc11a*u(i1-2*is1,i2-2*is2,i3-2*is3,ex)-cc12a*u(i1-2*
     & is1,i2-2*is2,i3-2*is3,ey)-cc13a*u(i1-2*is1,i2-2*is2,i3-2*is3,
     & ez)-cc14a*u(i1-is1,i2-is2,i3-is3,ex)-cc15a*u(i1-is1,i2-is2,i3-
     & is3,ey)-cc16a*u(i1-is1,i2-is2,i3-is3,ez)-cc11b*u(i1-2*js1,i2-2*
     & js2,i3-2*js3,ex)-cc12b*u(i1-2*js1,i2-2*js2,i3-2*js3,ey)-cc13b*
     & u(i1-2*js1,i2-2*js2,i3-2*js3,ez)-cc14b*u(i1-js1,i2-js2,i3-js3,
     & ex)-cc15b*u(i1-js1,i2-js2,i3-js3,ey)-cc16b*u(i1-js1,i2-js2,i3-
     & js3,ez)
      f2=a2DotLu-cc21a*u(i1-2*is1,i2-2*is2,i3-2*is3,ex)-cc22a*u(i1-2*
     & is1,i2-2*is2,i3-2*is3,ey)-cc23a*u(i1-2*is1,i2-2*is2,i3-2*is3,
     & ez)-cc24a*u(i1-is1,i2-is2,i3-is3,ex)-cc25a*u(i1-is1,i2-is2,i3-
     & is3,ey)-cc26a*u(i1-is1,i2-is2,i3-is3,ez)-cc21b*u(i1-2*js1,i2-2*
     & js2,i3-2*js3,ex)-cc22b*u(i1-2*js1,i2-2*js2,i3-2*js3,ey)-cc23b*
     & u(i1-2*js1,i2-2*js2,i3-2*js3,ez)-cc24b*u(i1-js1,i2-js2,i3-js3,
     & ex)-cc25b*u(i1-js1,i2-js2,i3-js3,ey)-cc26b*u(i1-js1,i2-js2,i3-
     & js3,ez)
      f3=6*a21*u(i1,i2,i3,ex)+6*a22*u(i1,i2,i3,ey)+6*a23*u(i1,i2,i3,ez)
     & -4*a21*u(i1+is1,i2+is2,i3+is3,ex)-4*a22*u(i1+is1,i2+is2,i3+is3,
     & ey)-4*a23*u(i1+is1,i2+is2,i3+is3,ez)+a21*u(i1+2*is1,i2+2*is2,
     & i3+2*is3,ex)+a22*u(i1+2*is1,i2+2*is2,i3+2*is3,ey)+a23*u(i1+2*
     & is1,i2+2*is2,i3+2*is3,ez)-g2f
      f4=6*a11*u(i1,i2,i3,ex)+6*a12*u(i1,i2,i3,ey)+6*a13*u(i1,i2,i3,ez)
     & -4*a11*u(i1+js1,i2+js2,i3+js3,ex)-4*a12*u(i1+js1,i2+js2,i3+js3,
     & ey)-4*a13*u(i1+js1,i2+js2,i3+js3,ez)+a11*u(i1+2*js1,i2+2*js2,
     & i3+2*js3,ex)+a12*u(i1+2*js1,i2+2*js2,i3+2*js3,ey)+a13*u(i1+2*
     & js1,i2+2*js2,i3+2*js3,ez)-g1f

 ! Simplfied forms for the 4 equations a1.Lu, a2.Lu, a2.D+r4 u = g2f  a1.D+s4 u = g1f
 ! e1x := dd11*x1+dd12*x2+dd13*x3+dd14*x4+ f1x
 ! e2x := dd21*x1+dd22*x2+dd23*x3+dd24*x4+ f2x
 ! e3x := dd31*x1+dd32*x2+dd33*x3+dd34*x4+ f3x
 ! e4x := dd41*x1+dd42*x2+dd43*x3+dd44*x4+ f4x
      dd11=cc14a*b11+cc15a*b12+cc16a*b13
      dd12=cc11a*b21+cc12a*b22+cc13a*b23
      dd13=cc14b*b31+cc15b*b32+cc16b*b33
      dd14=cc11b*b41+cc12b*b42+cc13b*b43
      dd21=cc24a*b11+cc25a*b12+cc26a*b13
      dd22=cc21a*b21+cc22a*b22+cc23a*b23
      dd23=cc24b*b31+cc25b*b32+cc26b*b33
      dd24=cc21b*b41+cc22b*b42+cc23b*b43
      dd31=-4*a21*b11-4*a22*b12-4*a23*b13
      dd32=a21*b21+a22*b22+a23*b23
      dd33=0
      dd34=0
      dd41=0
      dd42=0
      dd43=-4*a11*b31-4*a12*b32-4*a13*b33
      dd44=a11*b41+a12*b42+a13*b43

      f1x=cc11a*g21+cc12a*g22+cc13a*g23+cc14a*g11+cc15a*g12+cc16a*g13+
     & cc11b*g41+cc12b*g42+cc13b*g43+cc14b*g31+cc15b*g32+cc16b*g33+f1
      f2x=cc21a*g21+cc22a*g22+cc23a*g23+cc24a*g11+cc25a*g12+cc26a*g13+
     & cc21b*g41+cc22b*g42+cc23b*g43+cc24b*g31+cc25b*g32+cc26b*g33+f2
      f3x=a21*g21+a22*g22+a23*g23-4*a21*g11-4*a22*g12-4*a23*g13+f3
      f4x=a11*g41+a12*g42+a13*g43-4*a11*g31-4*a12*g32-4*a13*g33+f4

!  solution x1,x2,x3,x4: 
      det=-dd32*dd43*dd14*dd21-dd32*dd11*dd23*dd44+dd32*dd11*dd43*dd24+
     & dd43*dd14*dd22*dd31-dd13*dd44*dd22*dd31+dd12*dd31*dd23*dd44+
     & dd32*dd13*dd44*dd21-dd12*dd31*dd43*dd24
      x1=(-dd32*f2x*dd13*dd44-dd32*dd43*dd24*f1x+dd32*dd23*dd44*f1x+
     & dd32*dd43*f2x*dd14-dd32*dd23*f4x*dd14+dd32*dd24*dd13*f4x-dd23*
     & dd44*dd12*f3x+dd22*f3x*dd13*dd44-dd43*dd22*f3x*dd14+dd43*dd24*
     & dd12*f3x)/det
      x2=(dd31*f2x*dd13*dd44+dd31*dd43*dd24*f1x-dd31*dd23*dd44*f1x-
     & dd31*dd43*f2x*dd14+dd31*dd23*f4x*dd14-dd31*dd24*dd13*f4x+f3x*
     & dd43*dd14*dd21+f3x*dd11*dd23*dd44-f3x*dd11*dd43*dd24-f3x*dd13*
     & dd44*dd21)/det
      x3=(dd44*dd32*dd11*f2x-dd44*dd12*dd31*f2x+dd44*dd12*f3x*dd21-
     & dd44*dd32*f1x*dd21-dd44*dd11*dd22*f3x+dd44*f1x*dd22*dd31+f4x*
     & dd32*dd14*dd21-f4x*dd32*dd11*dd24-f4x*dd14*dd22*dd31+f4x*dd12*
     & dd31*dd24)/det
      x4=(-dd32*dd13*f4x*dd21-dd32*dd11*dd43*f2x+dd12*dd31*dd43*f2x-
     & dd12*dd31*dd23*f4x-dd43*dd12*f3x*dd21+dd32*dd43*f1x*dd21+dd32*
     & dd11*dd23*f4x+dd13*f4x*dd22*dd31+dd11*dd43*dd22*f3x-dd43*f1x*
     & dd22*dd31)/det

! **** Now assign the extended boundary points **** 
      u(i1-  is1,i2-  is2,i3-  is3,ex) = b11*x1+ g11
      u(i1-  is1,i2-  is2,i3-  is3,ey) = b12*x1+ g12
      u(i1-  is1,i2-  is2,i3-  is3,ez) = b13*x1+ g13
      u(i1-2*is1,i2-2*is2,i3-2*is3,ex) = b21*x2+ g21
      u(i1-2*is1,i2-2*is2,i3-2*is3,ey) = b22*x2+ g22
      u(i1-2*is1,i2-2*is2,i3-2*is3,ez) = b23*x2+ g23
      u(i1-  js1,i2-  js2,i3-  js3,ex) = b31*x3+ g31
      u(i1-  js1,i2-  js2,i3-  js3,ey) = b32*x3+ g32
      u(i1-  js1,i2-  js2,i3-  js3,ez) = b33*x3+ g33
      u(i1-2*js1,i2-2*js2,i3-2*js3,ex) = b41*x4+ g41
      u(i1-2*js1,i2-2*js2,i3-2*js3,ey) = b42*x4+ g42
      u(i1-2*js1,i2-2*js2,i3-2*js3,ez) = b43*x4+ g43
                !        if( debug.gt.1 )then
                !         write(*,'(/," bce4: extended:(i1,i2,i3)=",3i5," is=",3i2," js=",3i2," ks=",3i2)') i1,i2,i3,is1,is2,is3,!               js1,js2,js3,ks1,ks2,ks3
                !         write(*,'(" bce4: c11,c22,c33,c1,c2,c3, DeltaU,DeltaV,DeltaW=",9f6.2)') c11,c22,c33,c1,c2,c3, DeltaU,DeltaV,DeltaW
                !        end if
                !        if( debug.gt.0 )then
                !         OGF3DFO(i1-is1,i2-is2,i3-is3,t, uvm(0),uvm(1),uvm(2))
                !         OGF3DFO(i1-2*is1,i2-2*is2,i3-2*is3,t, uvm2(0),uvm2(1),uvm2(2))
                !         write(*,'(" bce4: extended: (i1,i2,i3)=",3i4," err(-1,0),(-2,0)=",6e9.1)') i1,i2,i3,!           u(i1-  is1,i2-  is2,i3-  is3,ex)-uvm(0),!           u(i1-  is1,i2-  is2,i3-  is3,ey)-uvm(1),!           u(i1-  is1,i2-  is2,i3-  is3,ez)-uvm(2),!           u(i1-2*is1,i2-2*is2,i3-2*is3,ex)-uvm2(0),!           u(i1-2*is1,i2-2*is2,i3-2*is3,ey)-uvm2(1),!           u(i1-2*is1,i2-2*is2,i3-2*is3,ez)-uvm2(2)
                !         ! '
                !        end if
                !        if( debug.gt.1 )then
                !         write(*,'(" bce4: true(-1,0),(-2,0)    =",6f7.2)') uvm(0),uvm(1),uvm(2),uvm2(0),uvm2(1), uvm2(2)
                !         write(*,'(" bce4: computed(-1,0),(-2,0)=",6f7.2)') !           u(i1-  is1,i2-  is2,i3-  is3,ex),!           u(i1-  is1,i2-  is2,i3-  is3,ey),!           u(i1-  is1,i2-  is2,i3-  is3,ez),!           u(i1-2*is1,i2-2*is2,i3-2*is3,ex),!           u(i1-2*is1,i2-2*is2,i3-2*is3,ey),!           u(i1-2*is1,i2-2*is2,i3-2*is3,ez)
                !        end if
                !        if( debug.gt.0 )then
                !         OGF3DFO(i1-js1,i2-js2,i3-js3,t, uvm(0),uvm(1),uvm(2))
                !         OGF3DFO(i1-2*js1,i2-2*js2,i3-2*js3,t, uvm2(0),uvm2(1),uvm2(2))
                !         write(*,'(" bce4: extended: (i1,i2,i3)=",3i4," err(0,-1),(0,-2)=",6e9.1)') i1,i2,i3,!           u(i1-  js1,i2-  js2,i3-  js3,ex)-uvm(0),!           u(i1-  js1,i2-  js2,i3-  js3,ey)-uvm(1),!           u(i1-  js1,i2-  js2,i3-  js3,ez)-uvm(2),!           u(i1-2*js1,i2-2*js2,i3-2*js3,ex)-uvm2(0),!           u(i1-2*js1,i2-2*js2,i3-2*js3,ey)-uvm2(1),!           u(i1-2*js1,i2-2*js2,i3-2*js3,ez)-uvm2(2)
                !         ! '
                !        end if
                !        if( debug.gt.1 )then
                !         write(*,'(" bce4: true(0,-1),(0,-2)    =",6f7.2)') uvm(0),uvm(1),uvm(2),uvm2(0),uvm2(1), uvm2(2)
                !         write(*,'(" bce4: computed(0,-1),(0,-2)=",6f7.2)') !           u(i1-  js1,i2-  js2,i3-  js3,ex),!           u(i1-  js1,i2-  js2,i3-  js3,ey),!           u(i1-  js1,i2-  js2,i3-  js3,ez),!           u(i1-2*js1,i2-2*js2,i3-2*js3,ex),!           u(i1-2*js1,i2-2*js2,i3-2*js3,ey),!           u(i1-2*js1,i2-2*js2,i3-2*js3,ez)
                !        end if
                !
                !        if( debug.gt.1 )then
                !         m1=i1-is1
                !         m2=i2-is2
                !         m3=i3-is3
                !         OGF3DFO(m1,m2,m3,t, uvm(0),uvm(1),uvm(2))
                !         write(*,'(" bce4:tan-comp: err(a1.u1,a3.u1)=",2e10.2)') !              A11D3(m1,m2,m3)*(u(m1,m2,m3,ex)-uvm(0))+!              A12D3(m1,m2,m3)*(u(m1,m2,m3,ey)-uvm(1))+!              A13D3(m1,m2,m3)*(u(m1,m2,m3,ez)-uvm(2)), !              A31D3(m1,m2,m3)*(u(m1,m2,m3,ex)-uvm(0))+!              A32D3(m1,m2,m3)*(u(m1,m2,m3,ey)-uvm(1))+!              A33D3(m1,m2,m3)*(u(m1,m2,m3,ez)-uvm(2))
                !
                !         
                !         write(*,'(" bce4:tan: u1[k] = b1[k] + g1[k]")')
                !         a11c=A11D3(m1,m2,m3)
                !         a12c=A12D3(m1,m2,m3)
                !         a13c=A13D3(m1,m2,m3)
                !         a21c=A21D3(m1,m2,m3)
                !         a22c=A22D3(m1,m2,m3)
                !         a23c=A23D3(m1,m2,m3)
                !         a31c=A31D3(m1,m2,m3)
                !         a32c=A32D3(m1,m2,m3)
                !         a33c=A33D3(m1,m2,m3)
                !         write(*,'(" bce4:tan: (a11,a12,a13)=(",3e10.2,")")') a11c,a12c,a13c
                !         write(*,'(" bce4:tan: (a21,a22,a23)=(",3e10.2,")")') a21c,a22c,a23c
                !         write(*,'(" bce4:tan: (a31,a32,a33)=(",3e10.2,")")') a31c,a32c,a33c
                !         write(*,'(" bce4:tan: (b11,b12,b13)=(",3e10.2,") (g11,g12,g12)=(",3e10.2,")")') b11,b12,b13,g11,g12,g13
                !         ! '
                !         write(*,'(" bce4:tan: a1Dotu1-a1.g1 =",e10.2,", a3Dotu1-a3.g1 =",e10.2)') !                      a1Dotu1-(a11c*g11+a12c*g12+a13c*g13),!                      a3Dotu1-(a31c*g11+a32c*g12+a33c*g13)
                !         ! '
                !         m1=i1-2*is1
                !         m2=i2-2*is2
                !         m3=i3-2*is3
                !         OGF3DFO(m1,m2,m3,t, uvm(0),uvm(1),uvm(2))
                !         write(*,'(" bce4:tan-comp: err(a1.u2,a3.u2)=",2e10.2)') !              A11D3(m1,m2,m3)*(u(m1,m2,m3,ex)-uvm(0))+!              A12D3(m1,m2,m3)*(u(m1,m2,m3,ey)-uvm(1))+!              A13D3(m1,m2,m3)*(u(m1,m2,m3,ez)-uvm(2)), !              A31D3(m1,m2,m3)*(u(m1,m2,m3,ex)-uvm(0))+!              A32D3(m1,m2,m3)*(u(m1,m2,m3,ey)-uvm(1))+!              A33D3(m1,m2,m3)*(u(m1,m2,m3,ez)-uvm(2))
                !
                !         write(*,'(" bce4:tan: u2[k] = b2[k] + g2[k]")')
                !         a11c=A11D3(m1,m2,m3)
                !         a12c=A12D3(m1,m2,m3)
                !         a13c=A13D3(m1,m2,m3)
                !         a21c=A21D3(m1,m2,m3)
                !         a22c=A22D3(m1,m2,m3)
                !         a23c=A23D3(m1,m2,m3)
                !         a31c=A31D3(m1,m2,m3)
                !         a32c=A32D3(m1,m2,m3)
                !         a33c=A33D3(m1,m2,m3)
                !         write(*,'(" bce4:tan: (a11,a12,a13)=(",3e10.2,")")') a11c,a12c,a13c
                !         write(*,'(" bce4:tan: (b21,b22,b23)=(",3e10.2,") (g21,g22,g22)=(",3e10.2,")")') b21,b22,b23,g21,g22,g23
                !         ! '
                !         write(*,'(" bce4:tan: a1Dotu2-a1.g2 =",e10.2,", a3Dotu2-a3.g2 =",e10.2)') !                      a1Dotu2-(a11c*g21+a12c*g22+a13c*g23),!                      a3Dotu2-(a31c*g21+a32c*g22+a33c*g23)
                !         ! '
                !
                !         ! error in extrap : a2.D+ u(i1-2) - g2f
                !         write(*,'(" bce4:extrap: err(a2.D+ u(i1-2)-g2f)=",e10.2," g2f=",e10.2)')!              a21c*(u(i1-2*is1,i2-2*is2,i3-2*is3,ex)-4.*u(i1-is1,i2-is2,i3-is3,ex)!                +6.*u(i1,i2,i3,ex)-4.*u(i1+is1,i2+is2,i3+is3,ex)+u(i1+2*is1,i2+2*is2,i3+2*is3,ex)) !            + a22c*(u(i1-2*is1,i2-2*is2,i3-2*is3,ey)-4.*u(i1-is1,i2-is2,i3-is3,ey)!                +6.*u(i1,i2,i3,ey)-4.*u(i1+is1,i2+is2,i3+is3,ey)+u(i1+2*is1,i2+2*is2,i3+2*is3,ey)) !            + a23c*(u(i1-2*is1,i2-2*is2,i3-2*is3,ez)-4.*u(i1-is1,i2-is2,i3-is3,ez)!                +6.*u(i1,i2,i3,ez)-4.*u(i1+is1,i2+is2,i3+is3,ez)+u(i1+2*is1,i2+2*is2,i3+2*is3,ez)) -g2f,g2f
                !         ! '
                !
                !         m1=i1-js1
                !         m2=i2-js2
                !         m3=i3-js3
                !         write(*,'(" bce4:tan: u3[k] = b3[k] + g3[k]")')
                !         a11c=A11D3(m1,m2,m3)
                !         a12c=A12D3(m1,m2,m3)
                !         a13c=A13D3(m1,m2,m3)
                !         a21c=A21D3(m1,m2,m3)
                !         a22c=A22D3(m1,m2,m3)
                !         a23c=A23D3(m1,m2,m3)
                !         a31c=A31D3(m1,m2,m3)
                !         a32c=A32D3(m1,m2,m3)
                !         a33c=A33D3(m1,m2,m3)
                !         write(*,'(" bce4:tan: (a11,a12,a13)=(",3e10.2,")")') a11c,a12c,a13c
                !         write(*,'(" bce4:tan: (b31,b32,b33)=(",3e10.2,") (g31,g32,g32)=(",3e10.2,")")') b31,b32,b33,g31,g32,g33
                !         ! '
                !         write(*,'(" bce4:tan: a2Dotu3-a2.g3 =",e10.2,", a3Dotu3-a3.g3 =",e10.2)') !                      a2Dotu3-(a21c*g31+a22c*g32+a23c*g33),!                      a3Dotu3-(a31c*g31+a32c*g32+a33c*g33)
                !         ! '
                !
                !         OGF3DFO(m1,m2,m3,t, uvm(0),uvm(1),uvm(2))
                !         write(*,'(" bce4:tan-comp: err(a2.u3,a3.u3)=",2e10.2)') !              A21D3(m1,m2,m3)*(u(m1,m2,m3,ex)-uvm(0))+!              A22D3(m1,m2,m3)*(u(m1,m2,m3,ey)-uvm(1))+!              A23D3(m1,m2,m3)*(u(m1,m2,m3,ez)-uvm(2)), !              A31D3(m1,m2,m3)*(u(m1,m2,m3,ex)-uvm(0))+!              A32D3(m1,m2,m3)*(u(m1,m2,m3,ey)-uvm(1))+!              A33D3(m1,m2,m3)*(u(m1,m2,m3,ez)-uvm(2))
                !
                !         m1=i1-2*js1
                !         m2=i2-2*js2
                !         m3=i3-2*js3
                !         a11c=A11D3(m1,m2,m3)
                !         a12c=A12D3(m1,m2,m3)
                !         a13c=A13D3(m1,m2,m3)
                !         a21c=A21D3(m1,m2,m3)
                !         a22c=A22D3(m1,m2,m3)
                !         a23c=A23D3(m1,m2,m3)
                !         a31c=A31D3(m1,m2,m3)
                !         a32c=A32D3(m1,m2,m3)
                !         a33c=A33D3(m1,m2,m3)
                !         write(*,'(" bce4:tan: (a11,a12,a13)=(",3e10.2,")")') a11c,a12c,a13c
                !         write(*,'(" bce4:tan: (b41,b42,b43)=(",3e10.2,") (g41,g42,g42)=(",3e10.2,")")') b41,b42,b43,g41,g42,g43
                !         ! '
                !         write(*,'(" bce4:tan: a2Dotu4-a2.g4 =",e10.2,", a3Dotu4-a3.g4 =",e10.2)') !                      a2Dotu4-(a21c*g41+a22c*g42+a23c*g43),!                      a3Dotu4-(a31c*g41+a32c*g42+a33c*g43)
                !
                !         ! '
                !
                !         OGF3DFO(m1,m2,m3,t, uvm(0),uvm(1),uvm(2))
                !         write(*,'(" bce4:tan-comp: err(a2.u4,a3.u4)=",2e10.2)') !              A21D3(m1,m2,m3)*(u(m1,m2,m3,ex)-uvm(0))+!              A22D3(m1,m2,m3)*(u(m1,m2,m3,ey)-uvm(1))+!              A23D3(m1,m2,m3)*(u(m1,m2,m3,ez)-uvm(2)), !              A31D3(m1,m2,m3)*(u(m1,m2,m3,ex)-uvm(0))+!              A32D3(m1,m2,m3)*(u(m1,m2,m3,ey)-uvm(1))+!              A33D3(m1,m2,m3)*(u(m1,m2,m3,ez)-uvm(2))
                !
                !         a11c=A11D3(m1,m2,m3)
                !         a12c=A12D3(m1,m2,m3)
                !         a13c=A13D3(m1,m2,m3)
                !         a21c=A21D3(m1,m2,m3)
                !         a22c=A22D3(m1,m2,m3)
                !         a23c=A23D3(m1,m2,m3)
                !         a31c=A31D3(m1,m2,m3)
                !         a32c=A32D3(m1,m2,m3)
                !         a33c=A33D3(m1,m2,m3)
                !
                !         ! error in extrap : a1.D+ u(i2-2) - g1f
                !         write(*,'(" bce4:extrap: err(a1.D+ u(i2-2)-g1f)=",e10.2," g1f=",e10.2)')!              a11c*(u(i1-2*js1,i2-2*js2,i3-2*js3,ex)-4.*u(i1-js1,i2-js2,i3-js3,ex)!                +6.*u(i1,i2,i3,ex)-4.*u(i1+js1,i2+js2,i3+js3,ex)+u(i1+2*js1,i2+2*js2,i3+2*js3,ex)) !            + a12c*(u(i1-2*js1,i2-2*js2,i3-2*js3,ey)-4.*u(i1-js1,i2-js2,i3-js3,ey)!                +6.*u(i1,i2,i3,ey)-4.*u(i1+js1,i2+js2,i3+js3,ey)+u(i1+2*js1,i2+2*js2,i3+2*js3,ey)) !            + a13c*(u(i1-2*js1,i2-2*js2,i3-2*js3,ez)-4.*u(i1-js1,i2-js2,i3-js3,ez)!                +6.*u(i1,i2,i3,ez)-4.*u(i1+js1,i2+js2,i3+js3,ez)+u(i1+2*js1,i2+2*js2,i3+2*js3,ez)) -g1f,g1f
                !         ! '
                !
                !         uLap=ulaplacian43(i1,i2,i3,ex)
                !         vLap=ulaplacian43(i1,i2,i3,ey)
                !         wLap=ulaplacian43(i1,i2,i3,ez)
                !
                !         write(*,'(" bce4: err(a1.Delta u)=",e10.2," err(a2.Delta u)=",e10.2)')!           A11D3(i1,i2,i3)*(uLap-deltaFu)+A12D3(i1,i2,i3)*(vLap-deltaFv)+A13D3(i1,i2,i3)*(wLap-deltaFw),!           A21D3(i1,i2,i3)*(uLap-deltaFu)+A22D3(i1,i2,i3)*(vLap-deltaFv)+A23D3(i1,i2,i3)*(wLap-deltaFw)
                !         ! '
                !        end if ! end debug
                !        if( debug.gt.2 )then
                !         write(*,'(" bce4: a1DotLu,a2DotLu=",2e10.2,", deltaFu,deltaFv,deltaFw=",3e10.2)') !                a1DotLu,a2DotLu,deltaFu,deltaFv,deltaFw
                !         ! '
                !         
                !         write(*,'(" bce4: cc1ka : uv(0,-2) uv(0,-1) cc1kb: uv(2,0) uv(1,0)")')
                !         write(*,'(" bce4: 12.*(cc11a,cc12a,cc13a,cc14a,cc15a,cc16a)*dr^2=",6f6.2)')!               12.*cc11a*dra**2,12.*cc12a*dra**2,12.*cc13a*dsa**2,12.*cc14a*dsa**2,12.*cc15a*dsa**2,12.*cc16a*dsa**2
                !         write(*,'(" bce4: 12.*(cc11b,cc12b,cc13b,cc14b,cc15b,cc16b)*dr^2=",6f6.2)')!               12.*cc11b*dra**2,12.*cc12b*dra**2,12.*cc13b*dsa**2,12.*cc14b*dsa**2,12.*cc15b*dsa**2,12.*cc16b*dsa**2
                !         write(*,'(" bce4: 12.*(cc21a,cc22a,cc23a,cc24a,cc25a,cc26a)*dr^2=",6f6.2)')!               12.*cc21a*dra**2,12.*cc22a*dra**2,12.*cc23a*dsa**2,12.*cc24a*dsa**2,12.*cc25a*dsa**2,12.*cc26a*dsa**2
                !         write(*,'(" bce4: 12.*(cc21b,cc22b,cc23b,cc24b,cc25b,cc26b)*dr^2=",6f6.2)')!               12.*cc21b*dra**2,12.*cc22b*dra**2,12.*cc23b*dsa**2,12.*cc24b*dsa**2,12.*cc25b*dsa**2,12.*cc26b*dsa**2
                !         write(*,'(" bce4: 12.*(d11,d12,d13,d14)*dr^2=",4f6.2,", 12.*f1*dr^2,12.*f1x*dr^2=",2f7.2)')!                12.*dd11*dra**2,12.*dd12*dra**2,12.*dd13*dsa**2,12.*dd14*dsa**2,12.*f1*dra**2,12.*f1x*dra**2
                !         write(*,'(" bce4: 12.*(d21,d22,d23,d24)*dr^2=",4f6.2,", 12.*f2,12.*f2x*dr^2=",2f7.2)')!                12.*dd21*dra**2,12.*dd22*dra**2,12.*dd23*dsa**2,12.*dd24*dsa**2,12.*f2*dra**2,12.*f2x*dra**2
                !         ! '
                !        end if ! end debug
                         ! *** for now -- set solution to be exact ---
                         ! OGF3DFO(i1-is1,i2-is2,i3-is3,t, uvm(0),uvm(1),uvm(2))
                         ! OGF3DFO(i1-2*is1,i2-2*is2,i3-2*is3,t, uvm2(0),uvm2(1),uvm2(2))
                         ! u(i1-  is1,i2-  is2,i3-  is3,ex)=uvm(0)
                         ! u(i1-  is1,i2-  is2,i3-  is3,ey)=uvm(1)
                         ! u(i1-  is1,i2-  is2,i3-  is3,ez)=uvm(2)
                         ! u(i1-2*is1,i2-2*is2,i3-2*is3,ex)=uvm2(0)
                         ! u(i1-2*is1,i2-2*is2,i3-2*is3,ey)=uvm2(1)
                         ! u(i1-2*is1,i2-2*is2,i3-2*is3,ez)=uvm2(2)
                         ! OGF3DFO(i1-js1,i2-js2,i3-js3,t, uvm(0),uvm(1),uvm(2))
                         ! OGF3DFO(i1-2*js1,i2-2*js2,i3-2*js3,t, uvm2(0),uvm2(1),uvm2(2))
                         ! u(i1-  js1,i2-  js2,i3-  js3,ex)=uvm(0)
                         ! u(i1-  js1,i2-  js2,i3-  js3,ey)=uvm(1)
                         ! u(i1-  js1,i2-  js2,i3-  js3,ez)=uvm(2)
                         ! u(i1-2*js1,i2-2*js2,i3-2*js3,ex)=uvm2(0)
                         ! u(i1-2*js1,i2-2*js2,i3-2*js3,ey)=uvm2(1)
                         ! u(i1-2*js1,i2-2*js2,i3-2*js3,ez)=uvm2(2)
                     else
                       ! -----------------------------------------------------------------------------
                       ! --------------- fill in extended face values by extrapolation ---------------
                       ! ---------------         curvilinear, order 4                  ---------------
                       ! -----------------------------------------------------------------------------
                       ! *wdh* 2015/06/24 **WRONG**
                       ! -- for fourth-order scheme:
                       ! u(i1-js1,i2-js2,i3-js3,ex)=extrapolate5(ex,i1-js1,i2-js2,i3-js3,is1,is2,is3)
                       ! u(i1-js1,i2-js2,i3-js3,ey)=extrapolate5(ey,i1-js1,i2-js2,i3-js3,is1,is2,is3)
                       ! u(i1-js1,i2-js2,i3-js3,ez)=extrapolate5(ez,i1-js1,i2-js2,i3-js3,is1,is2,is3)
                      ! *wdh* 2015/07/13  
                      u(i1-  is1,i2-  is2,i3-  is3,ex) = (5.*u(i1-is1+
     & is1,i2-is2+is2,i3-is3+is3,ex)-10.*u(i1-is1+2*is1,i2-is2+2*is2,
     & i3-is3+2*is3,ex)+10.*u(i1-is1+3*is1,i2-is2+3*is2,i3-is3+3*is3,
     & ex)-5.*u(i1-is1+4*is1,i2-is2+4*is2,i3-is3+4*is3,ex)+u(i1-is1+5*
     & is1,i2-is2+5*is2,i3-is3+5*is3,ex))
                      u(i1-  is1,i2-  is2,i3-  is3,ey) = (5.*u(i1-is1+
     & is1,i2-is2+is2,i3-is3+is3,ey)-10.*u(i1-is1+2*is1,i2-is2+2*is2,
     & i3-is3+2*is3,ey)+10.*u(i1-is1+3*is1,i2-is2+3*is2,i3-is3+3*is3,
     & ey)-5.*u(i1-is1+4*is1,i2-is2+4*is2,i3-is3+4*is3,ey)+u(i1-is1+5*
     & is1,i2-is2+5*is2,i3-is3+5*is3,ey))
                      u(i1-  is1,i2-  is2,i3-  is3,ez) = (5.*u(i1-is1+
     & is1,i2-is2+is2,i3-is3+is3,ez)-10.*u(i1-is1+2*is1,i2-is2+2*is2,
     & i3-is3+2*is3,ez)+10.*u(i1-is1+3*is1,i2-is2+3*is2,i3-is3+3*is3,
     & ez)-5.*u(i1-is1+4*is1,i2-is2+4*is2,i3-is3+4*is3,ez)+u(i1-is1+5*
     & is1,i2-is2+5*is2,i3-is3+5*is3,ez))
                      u(i1-2*is1,i2-2*is2,i3-2*is3,ex) = (5.*u(i1-2*
     & is1+is1,i2-2*is2+is2,i3-2*is3+is3,ex)-10.*u(i1-2*is1+2*is1,i2-
     & 2*is2+2*is2,i3-2*is3+2*is3,ex)+10.*u(i1-2*is1+3*is1,i2-2*is2+3*
     & is2,i3-2*is3+3*is3,ex)-5.*u(i1-2*is1+4*is1,i2-2*is2+4*is2,i3-2*
     & is3+4*is3,ex)+u(i1-2*is1+5*is1,i2-2*is2+5*is2,i3-2*is3+5*is3,
     & ex))
                      u(i1-2*is1,i2-2*is2,i3-2*is3,ey) = (5.*u(i1-2*
     & is1+is1,i2-2*is2+is2,i3-2*is3+is3,ey)-10.*u(i1-2*is1+2*is1,i2-
     & 2*is2+2*is2,i3-2*is3+2*is3,ey)+10.*u(i1-2*is1+3*is1,i2-2*is2+3*
     & is2,i3-2*is3+3*is3,ey)-5.*u(i1-2*is1+4*is1,i2-2*is2+4*is2,i3-2*
     & is3+4*is3,ey)+u(i1-2*is1+5*is1,i2-2*is2+5*is2,i3-2*is3+5*is3,
     & ey))
                      u(i1-2*is1,i2-2*is2,i3-2*is3,ez) = (5.*u(i1-2*
     & is1+is1,i2-2*is2+is2,i3-2*is3+is3,ez)-10.*u(i1-2*is1+2*is1,i2-
     & 2*is2+2*is2,i3-2*is3+2*is3,ez)+10.*u(i1-2*is1+3*is1,i2-2*is2+3*
     & is2,i3-2*is3+3*is3,ez)-5.*u(i1-2*is1+4*is1,i2-2*is2+4*is2,i3-2*
     & is3+4*is3,ez)+u(i1-2*is1+5*is1,i2-2*is2+5*is2,i3-2*is3+5*is3,
     & ez))
                      u(i1-  js1,i2-  js2,i3-  js3,ex) = (5.*u(i1-js1+
     & js1,i2-js2+js2,i3-js3+js3,ex)-10.*u(i1-js1+2*js1,i2-js2+2*js2,
     & i3-js3+2*js3,ex)+10.*u(i1-js1+3*js1,i2-js2+3*js2,i3-js3+3*js3,
     & ex)-5.*u(i1-js1+4*js1,i2-js2+4*js2,i3-js3+4*js3,ex)+u(i1-js1+5*
     & js1,i2-js2+5*js2,i3-js3+5*js3,ex))
                      u(i1-  js1,i2-  js2,i3-  js3,ey) = (5.*u(i1-js1+
     & js1,i2-js2+js2,i3-js3+js3,ey)-10.*u(i1-js1+2*js1,i2-js2+2*js2,
     & i3-js3+2*js3,ey)+10.*u(i1-js1+3*js1,i2-js2+3*js2,i3-js3+3*js3,
     & ey)-5.*u(i1-js1+4*js1,i2-js2+4*js2,i3-js3+4*js3,ey)+u(i1-js1+5*
     & js1,i2-js2+5*js2,i3-js3+5*js3,ey))
                      u(i1-  js1,i2-  js2,i3-  js3,ez) = (5.*u(i1-js1+
     & js1,i2-js2+js2,i3-js3+js3,ez)-10.*u(i1-js1+2*js1,i2-js2+2*js2,
     & i3-js3+2*js3,ez)+10.*u(i1-js1+3*js1,i2-js2+3*js2,i3-js3+3*js3,
     & ez)-5.*u(i1-js1+4*js1,i2-js2+4*js2,i3-js3+4*js3,ez)+u(i1-js1+5*
     & js1,i2-js2+5*js2,i3-js3+5*js3,ez))
                      u(i1-2*js1,i2-2*js2,i3-2*js3,ex) = (5.*u(i1-2*
     & js1+js1,i2-2*js2+js2,i3-2*js3+js3,ex)-10.*u(i1-2*js1+2*js1,i2-
     & 2*js2+2*js2,i3-2*js3+2*js3,ex)+10.*u(i1-2*js1+3*js1,i2-2*js2+3*
     & js2,i3-2*js3+3*js3,ex)-5.*u(i1-2*js1+4*js1,i2-2*js2+4*js2,i3-2*
     & js3+4*js3,ex)+u(i1-2*js1+5*js1,i2-2*js2+5*js2,i3-2*js3+5*js3,
     & ex))
                      u(i1-2*js1,i2-2*js2,i3-2*js3,ey) = (5.*u(i1-2*
     & js1+js1,i2-2*js2+js2,i3-2*js3+js3,ey)-10.*u(i1-2*js1+2*js1,i2-
     & 2*js2+2*js2,i3-2*js3+2*js3,ey)+10.*u(i1-2*js1+3*js1,i2-2*js2+3*
     & js2,i3-2*js3+3*js3,ey)-5.*u(i1-2*js1+4*js1,i2-2*js2+4*js2,i3-2*
     & js3+4*js3,ey)+u(i1-2*js1+5*js1,i2-2*js2+5*js2,i3-2*js3+5*js3,
     & ey))
                      u(i1-2*js1,i2-2*js2,i3-2*js3,ez) = (5.*u(i1-2*
     & js1+js1,i2-2*js2+js2,i3-2*js3+js3,ez)-10.*u(i1-2*js1+2*js1,i2-
     & 2*js2+2*js2,i3-2*js3+2*js3,ez)+10.*u(i1-2*js1+3*js1,i2-2*js2+3*
     & js2,i3-2*js3+3*js3,ez)-5.*u(i1-2*js1+4*js1,i2-2*js2+4*js2,i3-2*
     & js3+4*js3,ez)+u(i1-2*js1+5*js1,i2-2*js2+5*js2,i3-2*js3+5*js3,
     & ez))
                !       ! Face 1:  (note: only one of (is1,is2,is3) is non-zero)
                !       u(i1-  is1,i2-  is2,i3-  is3,ex) = extrap5(u,i1,i2,i3,ex,is1,is2,is3)
                !       u(i1-  is1,i2-  is2,i3-  is3,ey) = extrap5(u,i1,i2,i3,ey,is1,is2,is3)
                !       u(i1-  is1,i2-  is2,i3-  is3,ez) = extrap5(u,i1,i2,i3,ez,is1,is2,is3)
                ! 
                !       u(i1-2*is1,i2-2*is2,i3-2*is3,ex) = extrap5(u,i1-is1,i2-is2,i3-is3,ex,is1,is2,is3)
                !       u(i1-2*is1,i2-2*is2,i3-2*is3,ey) = extrap5(u,i1-is1,i2-is2,i3-is3,ey,is1,is2,is3)
                !       u(i1-2*is1,i2-2*is2,i3-2*is3,ez) = extrap5(u,i1-is1,i2-is2,i3-is3,ez,is1,is2,is3)
                ! 
                !       ! Face 2 : (note: only one of (js1,js2,js3) is non-zero)
                !       u(i1-  js1,i2-  js2,i3-  js3,ex) = extrap5(u,i1,i2,i3,ex,js1,js2,js3)            
                !       u(i1-  js1,i2-  js2,i3-  js3,ey) = extrap5(u,i1,i2,i3,ey,js1,js2,js3)            
                !       u(i1-  js1,i2-  js2,i3-  js3,ez) = extrap5(u,i1,i2,i3,ez,js1,js2,js3)            
                !                                                                                      
                !       u(i1-2*js1,i2-2*js2,i3-2*js3,ex) = extrap5(u,i1-js1,i2-js2,i3-js3,ex,js1,js2,js3)
                !       u(i1-2*js1,i2-2*js2,i3-2*js3,ey) = extrap5(u,i1-js1,i2-js2,i3-js3,ey,js1,js2,js3)
                !       u(i1-2*js1,i2-2*js2,i3-2*js3,ez) = extrap5(u,i1-js1,i2-js2,i3-js3,ez,js1,js2,js3)
                     end if
                     end do ! end do i1
                     end do ! end do i2
                     end do ! end do i3
                   else if( bc1.eq.dirichlet .or. bc2.eq.dirichlet )
     & then
                ! *wdh* 081124 -- do nothing here ---
                ! *      do i3=n3a,n3b
                ! *      do i2=n2a,n2b
                ! *      do i1=n1a,n1b
                ! * 
                ! *       if( edgeDirection.ne.0 )then
                ! *         #If "twilightZone" == "twilightZone"
                ! *           OGF3DFO(i1-js1,i2,i3,t,g1,g2,g3)
                ! *         #End
                ! *         u(i1-js1,i2,i3,ex)=g1
                ! *         u(i1-js1,i2,i3,ey)=g2
                ! *         u(i1-js1,i2,i3,ez)=g3
                ! *       end if
                ! * 
                ! *       if( edgeDirection.ne.1 )then
                ! *         #If "twilightZone" == "twilightZone"
                ! *           OGF3DFO(i1,i2-js2,i3,t,g1,g2,g3)
                ! *         #End
                ! *         u(i1,i2-js2,i3,ex)=g1
                ! *         u(i1,i2-js2,i3,ey)=g2
                ! *         u(i1,i2-js2,i3,ez)=g3
                ! *       end if
                ! * 
                ! *       if( edgeDirection.ne.2 )then
                ! *         #If "twilightZone" == "twilightZone"
                ! *           OGF3DFO(i1,i2,i3-js3,t,g1,g2,g3)
                ! *         #End
                ! *         u(i1,i2,i3-js3,ex)=g1
                ! *         u(i1,i2,i3-js3,ey)=g2
                ! *         u(i1,i2,i3-js3,ez)=g3
                ! *       end if
                ! * 
                ! *      end do ! end do i1
                ! *      end do ! end do i2
                ! *      end do ! end do i3
                    else if( bc1.le.0 .or. bc2.le.0 )then
                      ! periodic or interpolation -- nothing to do
                    else if( bc1.eq.planeWaveBoundaryCondition 
     & .or.bc2.eq.planeWaveBoundaryCondition .or. 
     & bc1.eq.symmetryBoundaryCondition .or. 
     & bc2.eq.symmetryBoundaryCondition .or. (bc1.ge.abcEM2 .and. 
     & bc1.le.lastBC) .or. (bc2.ge.abcEM2 .and. bc2.le.lastBC))then
                      ! do nothing
                    else
                      write(*,'("ERROR: unknown boundary conditions 
     & bc1,bc2=",2i3)') bc1,bc2
                      ! unknown boundary conditions
                      stop 8866
                   end if
                 ! end orderOfAccuracy==4 
                 end do
                 end do
                 end do ! edge direction
                 ! *****************************************************************************
                 ! ************ assign corner GHOST points outside edges ***********************
                 ! ****************************************************************************
                  do edgeDirection=0,2 ! direction parallel to the edge
                 ! do edgeDirection=2,2 ! direction parallel to the edge
                  do sidea=0,1
                  do sideb=0,1
                   if( edgeDirection.eq.0 )then
                     side1=0
                     side2=sidea
                     side3=sideb
                   else if( edgeDirection.eq.1 )then
                     side1=sideb
                     side2=0
                     side3=sidea
                   else
                     side1=sidea
                     side2=sideb
                     side3=0
                   end if
                  extra=numberOfGhostPoints  ! assign the extended boundary *wdh* 2015/06/23
                  is1=1-2*(side1)
                  is2=1-2*(side2)
                  is3=1-2*(side3)
                  if( edgeDirection.eq.2 )then
                   is3=0
                   n1a=gridIndexRange(side1,0)
                   n1b=gridIndexRange(side1,0)
                   n2a=gridIndexRange(side2,1)
                   n2b=gridIndexRange(side2,1)
                   n3a=gridIndexRange(0,2)-extra
                   n3b=gridIndexRange(1,2)+extra
                   bc1=boundaryCondition(side1,0)
                   bc2=boundaryCondition(side2,1)
                  else if( edgeDirection.eq.1 )then
                   is2=0
                   n1a=gridIndexRange(side1,0)
                   n1b=gridIndexRange(side1,0)
                   n2a=gridIndexRange(    0,1)-extra
                   n2b=gridIndexRange(    1,1)+extra
                   n3a=gridIndexRange(side3,2)
                   n3b=gridIndexRange(side3,2)
                   bc1=boundaryCondition(side1,0)
                   bc2=boundaryCondition(side3,2)
                  else
                   is1=0
                   n1a=gridIndexRange(    0,0)-extra
                   n1b=gridIndexRange(    1,0)+extra
                   n2a=gridIndexRange(side2,1)
                   n2b=gridIndexRange(side2,1)
                   n3a=gridIndexRange(side3,2)
                   n3b=gridIndexRange(side3,2)
                   bc1=boundaryCondition(side2,1)
                   bc2=boundaryCondition(side3,2)
                  end if
                  ! *********************************************************
                  ! ************* Assign Ghost near two faces ***************
                  ! *************       CURVILINEAR            **************
                  ! *********************************************************
                   ls1=is1  ! save for extrapolation
                   ls2=is2
                   ls3=is3
                   is1=0
                   is2=0
                   is3=0
                   js1=0
                   js2=0
                   js3=0
                   ks1=0
                   ks2=0
                   ks3=0
                   if( edgeDirection.eq.0 )then
                     axis=1
                     axisp1=2
                     axisp2=0
                     side1=0
                     side2=sidea
                     side3=sideb
                     is2=1-2*side2  ! normal direction 1
                     js3=1-2*side3  ! normal direction 2
                     ks1=1          ! tangential direction
                   else if( edgeDirection.eq.1 )then
                     axis=2
                     axisp1=0
                     axisp2=1
                     side1=sideb
                     side2=0
                     side3=sidea
                     is3=1-2*side3  ! normal direction 1
                     js1=1-2*side1  ! normal direction 2
                     ks2=1          ! tangential direction
                   else
                     axis=0
                     axisp1=1
                     axisp2=2
                     side1=sidea
                     side2=sideb
                     side3=0
                     is1=1-2*side1  ! normal direction 1
                     js2=1-2*side2  ! normal direction 2
                     ks3=1          ! tangential direction
                   end if
                   dra=dr(axis  )*(1-2*sidea)
                   dsa=dr(axisp1)*(1-2*sideb)
                   dta=dr(axisp2)
                   if( bc1.eq.perfectElectricalConductor .and. 
     & bc2.eq.perfectElectricalConductor )then
                    ! *********************************************************
                    ! ************* PEC EDGE BC (CURVILINEAR) *****************
                    ! *********************************************************
                     if( debug.gt.0 )then
                       write(*,'(/," corner-edge-4:Start edge=",i1," 
     & side1,side2,side3=",3i2," is=",3i3," js=",3i3," ks=",3i3)') 
     & edgeDirection,side1,side2,side3,is1,is2,is3,js1,js2,js3,ks1,
     & ks2,ks3
                       write(*,'("   dra,dsa,dta=",3f8.5)') dra,dsa,dta
                       ! '
                     end if
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                     ! Check the mask:  *wdh* 2015/06/24
                     if( mask(i1,i2,i3).gt.0 .and. 
     & i1.ge.gridIndexRange(0,0) .and. i1.le.gridIndexRange(1,0) 
     & .and. i2.ge.gridIndexRange(0,1) .and. i2.le.gridIndexRange(1,1)
     &  .and. i3.ge.gridIndexRange(0,2) .and. i3.le.gridIndexRange(1,
     & 2) )then
                       ! precompute the inverse of the jacobian, used in macros AmnD3J
                       i10=i1  ! used by jac3di in macros
                       i20=i2
                       i30=i3
                       do m3=-numberOfGhostPoints,numberOfGhostPoints
                       do m2=-numberOfGhostPoints,numberOfGhostPoints
                       do m1=-numberOfGhostPoints,numberOfGhostPoints
                        jac3di(m1,m2,m3)=1./(rx(i1+m1,i2+m2,i3+m3)*(sy(
     & i1+m1,i2+m2,i3+m3)*tz(i1+m1,i2+m2,i3+m3)-sz(i1+m1,i2+m2,i3+m3)*
     & ty(i1+m1,i2+m2,i3+m3))+ry(i1+m1,i2+m2,i3+m3)*(sz(i1+m1,i2+m2,
     & i3+m3)*tx(i1+m1,i2+m2,i3+m3)-sx(i1+m1,i2+m2,i3+m3)*tz(i1+m1,i2+
     & m2,i3+m3))+rz(i1+m1,i2+m2,i3+m3)*(sx(i1+m1,i2+m2,i3+m3)*ty(i1+
     & m1,i2+m2,i3+m3)-sy(i1+m1,i2+m2,i3+m3)*tx(i1+m1,i2+m2,i3+m3)))
                       end do
                       end do
                       end do
                       a11 =(rsxy(i1,i2,i3,axis,0)*jac3di(i1-i10,i2-
     & i20,i3-i30))
                       a12 =(rsxy(i1,i2,i3,axis,1)*jac3di(i1-i10,i2-
     & i20,i3-i30))
                       a13 =(rsxy(i1,i2,i3,axis,2)*jac3di(i1-i10,i2-
     & i20,i3-i30))
                       a21 =(rsxy(i1,i2,i3,axisp1,0)*jac3di(i1-i10,i2-
     & i20,i3-i30))
                       a22 =(rsxy(i1,i2,i3,axisp1,1)*jac3di(i1-i10,i2-
     & i20,i3-i30))
                       a23 =(rsxy(i1,i2,i3,axisp1,2)*jac3di(i1-i10,i2-
     & i20,i3-i30))
                       a31 =(rsxy(i1,i2,i3,axisp2,0)*jac3di(i1-i10,i2-
     & i20,i3-i30))
                       a32 =(rsxy(i1,i2,i3,axisp2,1)*jac3di(i1-i10,i2-
     & i20,i3-i30))
                       a33 =(rsxy(i1,i2,i3,axisp2,2)*jac3di(i1-i10,i2-
     & i20,i3-i30))
                       ! ************ Order 4 ******************
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
                       a21r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axisp1,0)
     & *jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))-(rsxy(i1-is1,i2-is2,
     & i3-is3,axisp1,0)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp1,0)*jac3di(i1+2*is1-i10,
     & i2+2*is2-i20,i3+2*is3-i30))-(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,
     & axisp1,0)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))/(
     & 12.*dra)
                       a22r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axisp1,1)
     & *jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))-(rsxy(i1-is1,i2-is2,
     & i3-is3,axisp1,1)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp1,1)*jac3di(i1+2*is1-i10,
     & i2+2*is2-i20,i3+2*is3-i30))-(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,
     & axisp1,1)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))/(
     & 12.*dra)
                       a23r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axisp1,2)
     & *jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))-(rsxy(i1-is1,i2-is2,
     & i3-is3,axisp1,2)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp1,2)*jac3di(i1+2*is1-i10,
     & i2+2*is2-i20,i3+2*is3-i30))-(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,
     & axisp1,2)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))/(
     & 12.*dra)
                       a31r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axisp2,0)
     & *jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))-(rsxy(i1-is1,i2-is2,
     & i3-is3,axisp2,0)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp2,0)*jac3di(i1+2*is1-i10,
     & i2+2*is2-i20,i3+2*is3-i30))-(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,
     & axisp2,0)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))/(
     & 12.*dra)
                       a32r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axisp2,1)
     & *jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))-(rsxy(i1-is1,i2-is2,
     & i3-is3,axisp2,1)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp2,1)*jac3di(i1+2*is1-i10,
     & i2+2*is2-i20,i3+2*is3-i30))-(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,
     & axisp2,1)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))/(
     & 12.*dra)
                       a33r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axisp2,2)
     & *jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))-(rsxy(i1-is1,i2-is2,
     & i3-is3,axisp2,2)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp2,2)*jac3di(i1+2*is1-i10,
     & i2+2*is2-i20,i3+2*is3-i30))-(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,
     & axisp2,2)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))/(
     & 12.*dra)
                       a11rr = ((-30.*(rsxy(i1,i2,i3,axis,0)*jac3di(i1-
     & i10,i2-i20,i3-i30))+16.*((rsxy(i1+is1,i2+is2,i3+is3,axis,0)*
     & jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))+(rsxy(i1-is1,i2-is2,
     & i3-is3,axis,0)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,0)*jac3di(i1+2*is1-i10,i2+
     & 2*is2-i20,i3+2*is3-i30))+(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,
     & 0)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))/(12.*dra**
     & 2))
                       a12rr = ((-30.*(rsxy(i1,i2,i3,axis,1)*jac3di(i1-
     & i10,i2-i20,i3-i30))+16.*((rsxy(i1+is1,i2+is2,i3+is3,axis,1)*
     & jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))+(rsxy(i1-is1,i2-is2,
     & i3-is3,axis,1)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,1)*jac3di(i1+2*is1-i10,i2+
     & 2*is2-i20,i3+2*is3-i30))+(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,
     & 1)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))/(12.*dra**
     & 2))
                       a13rr = ((-30.*(rsxy(i1,i2,i3,axis,2)*jac3di(i1-
     & i10,i2-i20,i3-i30))+16.*((rsxy(i1+is1,i2+is2,i3+is3,axis,2)*
     & jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))+(rsxy(i1-is1,i2-is2,
     & i3-is3,axis,2)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))-((
     & rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,2)*jac3di(i1+2*is1-i10,i2+
     & 2*is2-i20,i3+2*is3-i30))+(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,
     & 2)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))/(12.*dra**
     & 2))
                       a21rr = ((-30.*(rsxy(i1,i2,i3,axisp1,0)*jac3di(
     & i1-i10,i2-i20,i3-i30))+16.*((rsxy(i1+is1,i2+is2,i3+is3,axisp1,
     & 0)*jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))+(rsxy(i1-is1,i2-
     & is2,i3-is3,axisp1,0)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))
     & -((rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp1,0)*jac3di(i1+2*is1-
     & i10,i2+2*is2-i20,i3+2*is3-i30))+(rsxy(i1-2*is1,i2-2*is2,i3-2*
     & is3,axisp1,0)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))
     & /(12.*dra**2))
                       a22rr = ((-30.*(rsxy(i1,i2,i3,axisp1,1)*jac3di(
     & i1-i10,i2-i20,i3-i30))+16.*((rsxy(i1+is1,i2+is2,i3+is3,axisp1,
     & 1)*jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))+(rsxy(i1-is1,i2-
     & is2,i3-is3,axisp1,1)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))
     & -((rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp1,1)*jac3di(i1+2*is1-
     & i10,i2+2*is2-i20,i3+2*is3-i30))+(rsxy(i1-2*is1,i2-2*is2,i3-2*
     & is3,axisp1,1)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))
     & /(12.*dra**2))
                       a23rr = ((-30.*(rsxy(i1,i2,i3,axisp1,2)*jac3di(
     & i1-i10,i2-i20,i3-i30))+16.*((rsxy(i1+is1,i2+is2,i3+is3,axisp1,
     & 2)*jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))+(rsxy(i1-is1,i2-
     & is2,i3-is3,axisp1,2)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))
     & -((rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp1,2)*jac3di(i1+2*is1-
     & i10,i2+2*is2-i20,i3+2*is3-i30))+(rsxy(i1-2*is1,i2-2*is2,i3-2*
     & is3,axisp1,2)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))
     & /(12.*dra**2))
                       a31rr = ((-30.*(rsxy(i1,i2,i3,axisp2,0)*jac3di(
     & i1-i10,i2-i20,i3-i30))+16.*((rsxy(i1+is1,i2+is2,i3+is3,axisp2,
     & 0)*jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))+(rsxy(i1-is1,i2-
     & is2,i3-is3,axisp2,0)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))
     & -((rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp2,0)*jac3di(i1+2*is1-
     & i10,i2+2*is2-i20,i3+2*is3-i30))+(rsxy(i1-2*is1,i2-2*is2,i3-2*
     & is3,axisp2,0)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))
     & /(12.*dra**2))
                       a32rr = ((-30.*(rsxy(i1,i2,i3,axisp2,1)*jac3di(
     & i1-i10,i2-i20,i3-i30))+16.*((rsxy(i1+is1,i2+is2,i3+is3,axisp2,
     & 1)*jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))+(rsxy(i1-is1,i2-
     & is2,i3-is3,axisp2,1)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))
     & -((rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp2,1)*jac3di(i1+2*is1-
     & i10,i2+2*is2-i20,i3+2*is3-i30))+(rsxy(i1-2*is1,i2-2*is2,i3-2*
     & is3,axisp2,1)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))
     & /(12.*dra**2))
                       a33rr = ((-30.*(rsxy(i1,i2,i3,axisp2,2)*jac3di(
     & i1-i10,i2-i20,i3-i30))+16.*((rsxy(i1+is1,i2+is2,i3+is3,axisp2,
     & 2)*jac3di(i1+is1-i10,i2+is2-i20,i3+is3-i30))+(rsxy(i1-is1,i2-
     & is2,i3-is3,axisp2,2)*jac3di(i1-is1-i10,i2-is2-i20,i3-is3-i30)))
     & -((rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp2,2)*jac3di(i1+2*is1-
     & i10,i2+2*is2-i20,i3+2*is3-i30))+(rsxy(i1-2*is1,i2-2*is2,i3-2*
     & is3,axisp2,2)*jac3di(i1-2*is1-i10,i2-2*is2-i20,i3-2*is3-i30))))
     & /(12.*dra**2))
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
                       a21s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axisp1,0)
     & *jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))-(rsxy(i1-js1,i2-js2,
     & i3-js3,axisp1,0)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,0)*jac3di(i1+2*js1-i10,
     & i2+2*js2-i20,i3+2*js3-i30))-(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,
     & axisp1,0)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(
     & 12.*dsa)
                       a22s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axisp1,1)
     & *jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))-(rsxy(i1-js1,i2-js2,
     & i3-js3,axisp1,1)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,1)*jac3di(i1+2*js1-i10,
     & i2+2*js2-i20,i3+2*js3-i30))-(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,
     & axisp1,1)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(
     & 12.*dsa)
                       a23s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axisp1,2)
     & *jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))-(rsxy(i1-js1,i2-js2,
     & i3-js3,axisp1,2)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,2)*jac3di(i1+2*js1-i10,
     & i2+2*js2-i20,i3+2*js3-i30))-(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,
     & axisp1,2)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(
     & 12.*dsa)
                       a31s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axisp2,0)
     & *jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))-(rsxy(i1-js1,i2-js2,
     & i3-js3,axisp2,0)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp2,0)*jac3di(i1+2*js1-i10,
     & i2+2*js2-i20,i3+2*js3-i30))-(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,
     & axisp2,0)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(
     & 12.*dsa)
                       a32s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axisp2,1)
     & *jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))-(rsxy(i1-js1,i2-js2,
     & i3-js3,axisp2,1)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp2,1)*jac3di(i1+2*js1-i10,
     & i2+2*js2-i20,i3+2*js3-i30))-(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,
     & axisp2,1)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(
     & 12.*dsa)
                       a33s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axisp2,2)
     & *jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))-(rsxy(i1-js1,i2-js2,
     & i3-js3,axisp2,2)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp2,2)*jac3di(i1+2*js1-i10,
     & i2+2*js2-i20,i3+2*js3-i30))-(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,
     & axisp2,2)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(
     & 12.*dsa)
                       a11ss = ((-30.*(rsxy(i1,i2,i3,axis,0)*jac3di(i1-
     & i10,i2-i20,i3-i30))+16.*((rsxy(i1+js1,i2+js2,i3+js3,axis,0)*
     & jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))+(rsxy(i1-js1,i2-js2,
     & i3-js3,axis,0)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axis,0)*jac3di(i1+2*js1-i10,i2+
     & 2*js2-i20,i3+2*js3-i30))+(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axis,
     & 0)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(12.*dsa**
     & 2))
                       a12ss = ((-30.*(rsxy(i1,i2,i3,axis,1)*jac3di(i1-
     & i10,i2-i20,i3-i30))+16.*((rsxy(i1+js1,i2+js2,i3+js3,axis,1)*
     & jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))+(rsxy(i1-js1,i2-js2,
     & i3-js3,axis,1)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axis,1)*jac3di(i1+2*js1-i10,i2+
     & 2*js2-i20,i3+2*js3-i30))+(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axis,
     & 1)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(12.*dsa**
     & 2))
                       a13ss = ((-30.*(rsxy(i1,i2,i3,axis,2)*jac3di(i1-
     & i10,i2-i20,i3-i30))+16.*((rsxy(i1+js1,i2+js2,i3+js3,axis,2)*
     & jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))+(rsxy(i1-js1,i2-js2,
     & i3-js3,axis,2)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))-((
     & rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axis,2)*jac3di(i1+2*js1-i10,i2+
     & 2*js2-i20,i3+2*js3-i30))+(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axis,
     & 2)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))/(12.*dsa**
     & 2))
                       a21ss = ((-30.*(rsxy(i1,i2,i3,axisp1,0)*jac3di(
     & i1-i10,i2-i20,i3-i30))+16.*((rsxy(i1+js1,i2+js2,i3+js3,axisp1,
     & 0)*jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))+(rsxy(i1-js1,i2-
     & js2,i3-js3,axisp1,0)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))
     & -((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,0)*jac3di(i1+2*js1-
     & i10,i2+2*js2-i20,i3+2*js3-i30))+(rsxy(i1-2*js1,i2-2*js2,i3-2*
     & js3,axisp1,0)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))
     & /(12.*dsa**2))
                       a22ss = ((-30.*(rsxy(i1,i2,i3,axisp1,1)*jac3di(
     & i1-i10,i2-i20,i3-i30))+16.*((rsxy(i1+js1,i2+js2,i3+js3,axisp1,
     & 1)*jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))+(rsxy(i1-js1,i2-
     & js2,i3-js3,axisp1,1)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))
     & -((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,1)*jac3di(i1+2*js1-
     & i10,i2+2*js2-i20,i3+2*js3-i30))+(rsxy(i1-2*js1,i2-2*js2,i3-2*
     & js3,axisp1,1)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))
     & /(12.*dsa**2))
                       a23ss = ((-30.*(rsxy(i1,i2,i3,axisp1,2)*jac3di(
     & i1-i10,i2-i20,i3-i30))+16.*((rsxy(i1+js1,i2+js2,i3+js3,axisp1,
     & 2)*jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))+(rsxy(i1-js1,i2-
     & js2,i3-js3,axisp1,2)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))
     & -((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,2)*jac3di(i1+2*js1-
     & i10,i2+2*js2-i20,i3+2*js3-i30))+(rsxy(i1-2*js1,i2-2*js2,i3-2*
     & js3,axisp1,2)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))
     & /(12.*dsa**2))
                       a31ss = ((-30.*(rsxy(i1,i2,i3,axisp2,0)*jac3di(
     & i1-i10,i2-i20,i3-i30))+16.*((rsxy(i1+js1,i2+js2,i3+js3,axisp2,
     & 0)*jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))+(rsxy(i1-js1,i2-
     & js2,i3-js3,axisp2,0)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))
     & -((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp2,0)*jac3di(i1+2*js1-
     & i10,i2+2*js2-i20,i3+2*js3-i30))+(rsxy(i1-2*js1,i2-2*js2,i3-2*
     & js3,axisp2,0)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))
     & /(12.*dsa**2))
                       a32ss = ((-30.*(rsxy(i1,i2,i3,axisp2,1)*jac3di(
     & i1-i10,i2-i20,i3-i30))+16.*((rsxy(i1+js1,i2+js2,i3+js3,axisp2,
     & 1)*jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))+(rsxy(i1-js1,i2-
     & js2,i3-js3,axisp2,1)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))
     & -((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp2,1)*jac3di(i1+2*js1-
     & i10,i2+2*js2-i20,i3+2*js3-i30))+(rsxy(i1-2*js1,i2-2*js2,i3-2*
     & js3,axisp2,1)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))
     & /(12.*dsa**2))
                       a33ss = ((-30.*(rsxy(i1,i2,i3,axisp2,2)*jac3di(
     & i1-i10,i2-i20,i3-i30))+16.*((rsxy(i1+js1,i2+js2,i3+js3,axisp2,
     & 2)*jac3di(i1+js1-i10,i2+js2-i20,i3+js3-i30))+(rsxy(i1-js1,i2-
     & js2,i3-js3,axisp2,2)*jac3di(i1-js1-i10,i2-js2-i20,i3-js3-i30)))
     & -((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp2,2)*jac3di(i1+2*js1-
     & i10,i2+2*js2-i20,i3+2*js3-i30))+(rsxy(i1-2*js1,i2-2*js2,i3-2*
     & js3,axisp2,2)*jac3di(i1-2*js1-i10,i2-2*js2-i20,i3-2*js3-i30))))
     & /(12.*dsa**2))
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
                       a21t = (8.*((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp1,0)
     & *jac3di(i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))-(rsxy(i1-ks1,i2-ks2,
     & i3-ks3,axisp1,0)*jac3di(i1-ks1-i10,i2-ks2-i20,i3-ks3-i30)))-((
     & rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp1,0)*jac3di(i1+2*ks1-i10,
     & i2+2*ks2-i20,i3+2*ks3-i30))-(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,
     & axisp1,0)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))))/(
     & 12.*dta)
                       a22t = (8.*((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp1,1)
     & *jac3di(i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))-(rsxy(i1-ks1,i2-ks2,
     & i3-ks3,axisp1,1)*jac3di(i1-ks1-i10,i2-ks2-i20,i3-ks3-i30)))-((
     & rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp1,1)*jac3di(i1+2*ks1-i10,
     & i2+2*ks2-i20,i3+2*ks3-i30))-(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,
     & axisp1,1)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))))/(
     & 12.*dta)
                       a23t = (8.*((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp1,2)
     & *jac3di(i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))-(rsxy(i1-ks1,i2-ks2,
     & i3-ks3,axisp1,2)*jac3di(i1-ks1-i10,i2-ks2-i20,i3-ks3-i30)))-((
     & rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp1,2)*jac3di(i1+2*ks1-i10,
     & i2+2*ks2-i20,i3+2*ks3-i30))-(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,
     & axisp1,2)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))))/(
     & 12.*dta)
                       a31t = (8.*((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,0)
     & *jac3di(i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))-(rsxy(i1-ks1,i2-ks2,
     & i3-ks3,axisp2,0)*jac3di(i1-ks1-i10,i2-ks2-i20,i3-ks3-i30)))-((
     & rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp2,0)*jac3di(i1+2*ks1-i10,
     & i2+2*ks2-i20,i3+2*ks3-i30))-(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,
     & axisp2,0)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))))/(
     & 12.*dta)
                       a32t = (8.*((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,1)
     & *jac3di(i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))-(rsxy(i1-ks1,i2-ks2,
     & i3-ks3,axisp2,1)*jac3di(i1-ks1-i10,i2-ks2-i20,i3-ks3-i30)))-((
     & rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp2,1)*jac3di(i1+2*ks1-i10,
     & i2+2*ks2-i20,i3+2*ks3-i30))-(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,
     & axisp2,1)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))))/(
     & 12.*dta)
                       a33t = (8.*((rsxy(i1+ks1,i2+ks2,i3+ks3,axisp2,2)
     & *jac3di(i1+ks1-i10,i2+ks2-i20,i3+ks3-i30))-(rsxy(i1-ks1,i2-ks2,
     & i3-ks3,axisp2,2)*jac3di(i1-ks1-i10,i2-ks2-i20,i3-ks3-i30)))-((
     & rsxy(i1+2*ks1,i2+2*ks2,i3+2*ks3,axisp2,2)*jac3di(i1+2*ks1-i10,
     & i2+2*ks2-i20,i3+2*ks3-i30))-(rsxy(i1-2*ks1,i2-2*ks2,i3-2*ks3,
     & axisp2,2)*jac3di(i1-2*ks1-i10,i2-2*ks2-i20,i3-2*ks3-i30))))/(
     & 12.*dta)
                       c11 = (rsxy(i1,i2,i3,axis,0)**2+rsxy(i1,i2,i3,
     & axis,1)**2+rsxy(i1,i2,i3,axis,2)**2)
                       c22 = (rsxy(i1,i2,i3,axisp1,0)**2+rsxy(i1,i2,i3,
     & axisp1,1)**2+rsxy(i1,i2,i3,axisp1,2)**2)
                       c33 = (rsxy(i1,i2,i3,axisp2,0)**2+rsxy(i1,i2,i3,
     & axisp2,1)**2+rsxy(i1,i2,i3,axisp2,2)**2)
                       c1 = (rsxyx43(i1,i2,i3,axis,0)+rsxyy43(i1,i2,i3,
     & axis,1)+rsxyz43(i1,i2,i3,axis,2))
                       c2 = (rsxyx43(i1,i2,i3,axisp1,0)+rsxyy43(i1,i2,
     & i3,axisp1,1)+rsxyz43(i1,i2,i3,axisp1,2))
                       c3 = (rsxyx43(i1,i2,i3,axisp2,0)+rsxyy43(i1,i2,
     & i3,axisp2,1)+rsxyz43(i1,i2,i3,axisp2,2))
                       c11r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axis,0)**
     & 2+rsxy(i1+is1,i2+is2,i3+is3,axis,1)**2+rsxy(i1+is1,i2+is2,i3+
     & is3,axis,2)**2)-(rsxy(i1-is1,i2-is2,i3-is3,axis,0)**2+rsxy(i1-
     & is1,i2-is2,i3-is3,axis,1)**2+rsxy(i1-is1,i2-is2,i3-is3,axis,2)*
     & *2))   -((rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axis,0)**2+rsxy(i1+2*
     & is1,i2+2*is2,i3+2*is3,axis,1)**2+rsxy(i1+2*is1,i2+2*is2,i3+2*
     & is3,axis,2)**2)-(rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,0)**2+
     & rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axis,1)**2+rsxy(i1-2*is1,i2-2*
     & is2,i3-2*is3,axis,2)**2))   )/(12.*dra)
                       c22r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axisp1,0)
     & **2+rsxy(i1+is1,i2+is2,i3+is3,axisp1,1)**2+rsxy(i1+is1,i2+is2,
     & i3+is3,axisp1,2)**2)-(rsxy(i1-is1,i2-is2,i3-is3,axisp1,0)**2+
     & rsxy(i1-is1,i2-is2,i3-is3,axisp1,1)**2+rsxy(i1-is1,i2-is2,i3-
     & is3,axisp1,2)**2))   -((rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp1,
     & 0)**2+rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp1,1)**2+rsxy(i1+2*
     & is1,i2+2*is2,i3+2*is3,axisp1,2)**2)-(rsxy(i1-2*is1,i2-2*is2,i3-
     & 2*is3,axisp1,0)**2+rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axisp1,1)**
     & 2+rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axisp1,2)**2))   )/(12.*dra)
                       c33r = (8.*((rsxy(i1+is1,i2+is2,i3+is3,axisp2,0)
     & **2+rsxy(i1+is1,i2+is2,i3+is3,axisp2,1)**2+rsxy(i1+is1,i2+is2,
     & i3+is3,axisp2,2)**2)-(rsxy(i1-is1,i2-is2,i3-is3,axisp2,0)**2+
     & rsxy(i1-is1,i2-is2,i3-is3,axisp2,1)**2+rsxy(i1-is1,i2-is2,i3-
     & is3,axisp2,2)**2))   -((rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp2,
     & 0)**2+rsxy(i1+2*is1,i2+2*is2,i3+2*is3,axisp2,1)**2+rsxy(i1+2*
     & is1,i2+2*is2,i3+2*is3,axisp2,2)**2)-(rsxy(i1-2*is1,i2-2*is2,i3-
     & 2*is3,axisp2,0)**2+rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axisp2,1)**
     & 2+rsxy(i1-2*is1,i2-2*is2,i3-2*is3,axisp2,2)**2))   )/(12.*dra)
                       c11s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axis,0)**
     & 2+rsxy(i1+js1,i2+js2,i3+js3,axis,1)**2+rsxy(i1+js1,i2+js2,i3+
     & js3,axis,2)**2)-(rsxy(i1-js1,i2-js2,i3-js3,axis,0)**2+rsxy(i1-
     & js1,i2-js2,i3-js3,axis,1)**2+rsxy(i1-js1,i2-js2,i3-js3,axis,2)*
     & *2))   -((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axis,0)**2+rsxy(i1+2*
     & js1,i2+2*js2,i3+2*js3,axis,1)**2+rsxy(i1+2*js1,i2+2*js2,i3+2*
     & js3,axis,2)**2)-(rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axis,0)**2+
     & rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axis,1)**2+rsxy(i1-2*js1,i2-2*
     & js2,i3-2*js3,axis,2)**2))   )/(12.*dsa)
                       c22s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axisp1,0)
     & **2+rsxy(i1+js1,i2+js2,i3+js3,axisp1,1)**2+rsxy(i1+js1,i2+js2,
     & i3+js3,axisp1,2)**2)-(rsxy(i1-js1,i2-js2,i3-js3,axisp1,0)**2+
     & rsxy(i1-js1,i2-js2,i3-js3,axisp1,1)**2+rsxy(i1-js1,i2-js2,i3-
     & js3,axisp1,2)**2))   -((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,
     & 0)**2+rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp1,1)**2+rsxy(i1+2*
     & js1,i2+2*js2,i3+2*js3,axisp1,2)**2)-(rsxy(i1-2*js1,i2-2*js2,i3-
     & 2*js3,axisp1,0)**2+rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axisp1,1)**
     & 2+rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axisp1,2)**2))   )/(12.*dsa)
                       c33s = (8.*((rsxy(i1+js1,i2+js2,i3+js3,axisp2,0)
     & **2+rsxy(i1+js1,i2+js2,i3+js3,axisp2,1)**2+rsxy(i1+js1,i2+js2,
     & i3+js3,axisp2,2)**2)-(rsxy(i1-js1,i2-js2,i3-js3,axisp2,0)**2+
     & rsxy(i1-js1,i2-js2,i3-js3,axisp2,1)**2+rsxy(i1-js1,i2-js2,i3-
     & js3,axisp2,2)**2))   -((rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp2,
     & 0)**2+rsxy(i1+2*js1,i2+2*js2,i3+2*js3,axisp2,1)**2+rsxy(i1+2*
     & js1,i2+2*js2,i3+2*js3,axisp2,2)**2)-(rsxy(i1-2*js1,i2-2*js2,i3-
     & 2*js3,axisp2,0)**2+rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axisp2,1)**
     & 2+rsxy(i1-2*js1,i2-2*js2,i3-2*js3,axisp2,2)**2))   )/(12.*dsa)
                       if( axis.eq.0 )then
                         c1r = (rsxyxr43(i1,i2,i3,axis,0)+rsxyyr43(i1,
     & i2,i3,axis,1)+rsxyzr43(i1,i2,i3,axis,2))
                         c2r = (rsxyxr43(i1,i2,i3,axisp1,0)+rsxyyr43(
     & i1,i2,i3,axisp1,1)+rsxyzr43(i1,i2,i3,axisp1,2))
                         c3r = (rsxyxr43(i1,i2,i3,axisp2,0)+rsxyyr43(
     & i1,i2,i3,axisp2,1)+rsxyzr43(i1,i2,i3,axisp2,2))
                         c1s = (rsxyxs43(i1,i2,i3,axis,0)+rsxyys43(i1,
     & i2,i3,axis,1)+rsxyzs43(i1,i2,i3,axis,2))
                         c2s = (rsxyxs43(i1,i2,i3,axisp1,0)+rsxyys43(
     & i1,i2,i3,axisp1,1)+rsxyzs43(i1,i2,i3,axisp1,2))
                         c3s = (rsxyxs43(i1,i2,i3,axisp2,0)+rsxyys43(
     & i1,i2,i3,axisp2,1)+rsxyzs43(i1,i2,i3,axisp2,2))
                       else if( axis.eq.1 )then
                         c1r = (rsxyxs43(i1,i2,i3,axis,0)+rsxyys43(i1,
     & i2,i3,axis,1)+rsxyzs43(i1,i2,i3,axis,2))
                         c2r = (rsxyxs43(i1,i2,i3,axisp1,0)+rsxyys43(
     & i1,i2,i3,axisp1,1)+rsxyzs43(i1,i2,i3,axisp1,2))
                         c3r = (rsxyxs43(i1,i2,i3,axisp2,0)+rsxyys43(
     & i1,i2,i3,axisp2,1)+rsxyzs43(i1,i2,i3,axisp2,2))
                         c1s = (rsxyxt43(i1,i2,i3,axis,0)+rsxyyt43(i1,
     & i2,i3,axis,1)+rsxyzt43(i1,i2,i3,axis,2))
                         c2s = (rsxyxt43(i1,i2,i3,axisp1,0)+rsxyyt43(
     & i1,i2,i3,axisp1,1)+rsxyzt43(i1,i2,i3,axisp1,2))
                         c3s = (rsxyxt43(i1,i2,i3,axisp2,0)+rsxyyt43(
     & i1,i2,i3,axisp2,1)+rsxyzt43(i1,i2,i3,axisp2,2))
                       else
                         c1r = (rsxyxt43(i1,i2,i3,axis,0)+rsxyyt43(i1,
     & i2,i3,axis,1)+rsxyzt43(i1,i2,i3,axis,2))
                         c2r = (rsxyxt43(i1,i2,i3,axisp1,0)+rsxyyt43(
     & i1,i2,i3,axisp1,1)+rsxyzt43(i1,i2,i3,axisp1,2))
                         c3r = (rsxyxt43(i1,i2,i3,axisp2,0)+rsxyyt43(
     & i1,i2,i3,axisp2,1)+rsxyzt43(i1,i2,i3,axisp2,2))
                         c1s = (rsxyxr43(i1,i2,i3,axis,0)+rsxyyr43(i1,
     & i2,i3,axis,1)+rsxyzr43(i1,i2,i3,axis,2))
                         c2s = (rsxyxr43(i1,i2,i3,axisp1,0)+rsxyyr43(
     & i1,i2,i3,axisp1,1)+rsxyzr43(i1,i2,i3,axisp1,2))
                         c3s = (rsxyxr43(i1,i2,i3,axisp2,0)+rsxyyr43(
     & i1,i2,i3,axisp2,1)+rsxyzr43(i1,i2,i3,axisp2,2))
                       end if
                       ! ************ Order 4 ******************
                       ur=(8.*(u(i1+is1,i2+is2,i3+is3,ex)-u(i1-is1,i2-
     & is2,i3-is3,ex))-(u(i1+2*is1,i2+2*is2,i3+2*is3,ex)-u(i1-2*is1,
     & i2-2*is2,i3-2*is3,ex)))/(12.*dra)
                       urr=(-30.*u(i1,i2,i3,ex)+16.*(u(i1+is1,i2+is2,
     & i3+is3,ex)+u(i1-is1,i2-is2,i3-is3,ex))-(u(i1+2*is1,i2+2*is2,i3+
     & 2*is3,ex)+u(i1-2*is1,i2-2*is2,i3-2*is3,ex)))/(12.*dra**2)
                       urrr=(u(i1+2*is1,i2+2*is2,i3+2*is3,ex)-2.*u(i1+
     & is1,i2+is2,i3+is3,ex)+2.*u(i1-is1,i2-is2,i3-is3,ex)-u(i1-2*is1,
     & i2-2*is2,i3-2*is3,ex))/(2.*dra**3)
                       vr=(8.*(u(i1+is1,i2+is2,i3+is3,ey)-u(i1-is1,i2-
     & is2,i3-is3,ey))-(u(i1+2*is1,i2+2*is2,i3+2*is3,ey)-u(i1-2*is1,
     & i2-2*is2,i3-2*is3,ey)))/(12.*dra)
                       vrr=(-30.*u(i1,i2,i3,ey)+16.*(u(i1+is1,i2+is2,
     & i3+is3,ey)+u(i1-is1,i2-is2,i3-is3,ey))-(u(i1+2*is1,i2+2*is2,i3+
     & 2*is3,ey)+u(i1-2*is1,i2-2*is2,i3-2*is3,ey)))/(12.*dra**2)
                       vrrr=(u(i1+2*is1,i2+2*is2,i3+2*is3,ey)-2.*u(i1+
     & is1,i2+is2,i3+is3,ey)+2.*u(i1-is1,i2-is2,i3-is3,ey)-u(i1-2*is1,
     & i2-2*is2,i3-2*is3,ey))/(2.*dra**3)
                       wr=(8.*(u(i1+is1,i2+is2,i3+is3,ez)-u(i1-is1,i2-
     & is2,i3-is3,ez))-(u(i1+2*is1,i2+2*is2,i3+2*is3,ez)-u(i1-2*is1,
     & i2-2*is2,i3-2*is3,ez)))/(12.*dra)
                       wrr=(-30.*u(i1,i2,i3,ez)+16.*(u(i1+is1,i2+is2,
     & i3+is3,ez)+u(i1-is1,i2-is2,i3-is3,ez))-(u(i1+2*is1,i2+2*is2,i3+
     & 2*is3,ez)+u(i1-2*is1,i2-2*is2,i3-2*is3,ez)))/(12.*dra**2)
                       wrrr=(u(i1+2*is1,i2+2*is2,i3+2*is3,ez)-2.*u(i1+
     & is1,i2+is2,i3+is3,ez)+2.*u(i1-is1,i2-is2,i3-is3,ez)-u(i1-2*is1,
     & i2-2*is2,i3-2*is3,ez))/(2.*dra**3)
                       us=(8.*(u(i1+js1,i2+js2,i3+js3,ex)-u(i1-js1,i2-
     & js2,i3-js3,ex))-(u(i1+2*js1,i2+2*js2,i3+2*js3,ex)-u(i1-2*js1,
     & i2-2*js2,i3-2*js3,ex)))/(12.*dsa)
                       uss=(-30.*u(i1,i2,i3,ex)+16.*(u(i1+js1,i2+js2,
     & i3+js3,ex)+u(i1-js1,i2-js2,i3-js3,ex))-(u(i1+2*js1,i2+2*js2,i3+
     & 2*js3,ex)+u(i1-2*js1,i2-2*js2,i3-2*js3,ex)))/(12.*dsa**2)
                       usss=(u(i1+2*js1,i2+2*js2,i3+2*js3,ex)-2.*u(i1+
     & js1,i2+js2,i3+js3,ex)+2.*u(i1-js1,i2-js2,i3-js3,ex)-u(i1-2*js1,
     & i2-2*js2,i3-2*js3,ex))/(2.*dsa**3)
                       vs=(8.*(u(i1+js1,i2+js2,i3+js3,ey)-u(i1-js1,i2-
     & js2,i3-js3,ey))-(u(i1+2*js1,i2+2*js2,i3+2*js3,ey)-u(i1-2*js1,
     & i2-2*js2,i3-2*js3,ey)))/(12.*dsa)
                       vss=(-30.*u(i1,i2,i3,ey)+16.*(u(i1+js1,i2+js2,
     & i3+js3,ey)+u(i1-js1,i2-js2,i3-js3,ey))-(u(i1+2*js1,i2+2*js2,i3+
     & 2*js3,ey)+u(i1-2*js1,i2-2*js2,i3-2*js3,ey)))/(12.*dsa**2)
                       vsss=(u(i1+2*js1,i2+2*js2,i3+2*js3,ey)-2.*u(i1+
     & js1,i2+js2,i3+js3,ey)+2.*u(i1-js1,i2-js2,i3-js3,ey)-u(i1-2*js1,
     & i2-2*js2,i3-2*js3,ey))/(2.*dsa**3)
                       ws=(8.*(u(i1+js1,i2+js2,i3+js3,ez)-u(i1-js1,i2-
     & js2,i3-js3,ez))-(u(i1+2*js1,i2+2*js2,i3+2*js3,ez)-u(i1-2*js1,
     & i2-2*js2,i3-2*js3,ez)))/(12.*dsa)
                       wss=(-30.*u(i1,i2,i3,ez)+16.*(u(i1+js1,i2+js2,
     & i3+js3,ez)+u(i1-js1,i2-js2,i3-js3,ez))-(u(i1+2*js1,i2+2*js2,i3+
     & 2*js3,ez)+u(i1-2*js1,i2-2*js2,i3-2*js3,ez)))/(12.*dsa**2)
                       wsss=(u(i1+2*js1,i2+2*js2,i3+2*js3,ez)-2.*u(i1+
     & js1,i2+js2,i3+js3,ez)+2.*u(i1-js1,i2-js2,i3-js3,ez)-u(i1-2*js1,
     & i2-2*js2,i3-2*js3,ez))/(2.*dsa**3)
                       ut=(8.*(u(i1+ks1,i2+ks2,i3+ks3,ex)-u(i1-ks1,i2-
     & ks2,i3-ks3,ex))-(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ex)-u(i1-2*ks1,
     & i2-2*ks2,i3-2*ks3,ex)))/(12.*dta)
                       utt=(-30.*u(i1,i2,i3,ex)+16.*(u(i1+ks1,i2+ks2,
     & i3+ks3,ex)+u(i1-ks1,i2-ks2,i3-ks3,ex))-(u(i1+2*ks1,i2+2*ks2,i3+
     & 2*ks3,ex)+u(i1-2*ks1,i2-2*ks2,i3-2*ks3,ex)))/(12.*dta**2)
                       uttt=(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ex)-2.*u(i1+
     & ks1,i2+ks2,i3+ks3,ex)+2.*u(i1-ks1,i2-ks2,i3-ks3,ex)-u(i1-2*ks1,
     & i2-2*ks2,i3-2*ks3,ex))/(2.*dta**3)
                       vt=(8.*(u(i1+ks1,i2+ks2,i3+ks3,ey)-u(i1-ks1,i2-
     & ks2,i3-ks3,ey))-(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ey)-u(i1-2*ks1,
     & i2-2*ks2,i3-2*ks3,ey)))/(12.*dta)
                       vtt=(-30.*u(i1,i2,i3,ey)+16.*(u(i1+ks1,i2+ks2,
     & i3+ks3,ey)+u(i1-ks1,i2-ks2,i3-ks3,ey))-(u(i1+2*ks1,i2+2*ks2,i3+
     & 2*ks3,ey)+u(i1-2*ks1,i2-2*ks2,i3-2*ks3,ey)))/(12.*dta**2)
                       vttt=(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ey)-2.*u(i1+
     & ks1,i2+ks2,i3+ks3,ey)+2.*u(i1-ks1,i2-ks2,i3-ks3,ey)-u(i1-2*ks1,
     & i2-2*ks2,i3-2*ks3,ey))/(2.*dta**3)
                       wt=(8.*(u(i1+ks1,i2+ks2,i3+ks3,ez)-u(i1-ks1,i2-
     & ks2,i3-ks3,ez))-(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ez)-u(i1-2*ks1,
     & i2-2*ks2,i3-2*ks3,ez)))/(12.*dta)
                       wtt=(-30.*u(i1,i2,i3,ez)+16.*(u(i1+ks1,i2+ks2,
     & i3+ks3,ez)+u(i1-ks1,i2-ks2,i3-ks3,ez))-(u(i1+2*ks1,i2+2*ks2,i3+
     & 2*ks3,ez)+u(i1-2*ks1,i2-2*ks2,i3-2*ks3,ez)))/(12.*dta**2)
                       wttt=(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ez)-2.*u(i1+
     & ks1,i2+ks2,i3+ks3,ez)+2.*u(i1-ks1,i2-ks2,i3-ks3,ez)-u(i1-2*ks1,
     & i2-2*ks2,i3-2*ks3,ez))/(2.*dta**3)
                       if( edgeDirection.eq.0 )then
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
                          a21rs = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp1,
     & 0)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp1,
     & 0)*jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,
     & axisp1,0)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,
     & axisp1,0)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*(
     & (rsxy(i1,i2+1,i3-1,axisp1,0)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-
     & (rsxy(i1,i2-1,i3-1,axisp1,0)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))
     & -((rsxy(i1,i2+2,i3-1,axisp1,0)*jac3di(i1-i10,i2+2-i20,i3-1-i30)
     & )-(rsxy(i1,i2-2,i3-1,axisp1,0)*jac3di(i1-i10,i2-2-i20,i3-1-i30)
     & )))/(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp1,0)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp1,0)*jac3di(i1-
     & i10,i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp1,0)*jac3di(
     & i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp1,0)*jac3di(
     & i1-i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-
     & 2,axisp1,0)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-
     & 2,axisp1,0)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,
     & i3-2,axisp1,0)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp1,0)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1)))
     & )/(12.*dr(2))
                          a22rs = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp1,
     & 1)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp1,
     & 1)*jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,
     & axisp1,1)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,
     & axisp1,1)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*(
     & (rsxy(i1,i2+1,i3-1,axisp1,1)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-
     & (rsxy(i1,i2-1,i3-1,axisp1,1)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))
     & -((rsxy(i1,i2+2,i3-1,axisp1,1)*jac3di(i1-i10,i2+2-i20,i3-1-i30)
     & )-(rsxy(i1,i2-2,i3-1,axisp1,1)*jac3di(i1-i10,i2-2-i20,i3-1-i30)
     & )))/(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp1,1)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp1,1)*jac3di(i1-
     & i10,i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp1,1)*jac3di(
     & i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp1,1)*jac3di(
     & i1-i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-
     & 2,axisp1,1)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-
     & 2,axisp1,1)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,
     & i3-2,axisp1,1)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp1,1)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1)))
     & )/(12.*dr(2))
                          a23rs = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp1,
     & 2)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp1,
     & 2)*jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,
     & axisp1,2)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,
     & axisp1,2)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*(
     & (rsxy(i1,i2+1,i3-1,axisp1,2)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-
     & (rsxy(i1,i2-1,i3-1,axisp1,2)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))
     & -((rsxy(i1,i2+2,i3-1,axisp1,2)*jac3di(i1-i10,i2+2-i20,i3-1-i30)
     & )-(rsxy(i1,i2-2,i3-1,axisp1,2)*jac3di(i1-i10,i2-2-i20,i3-1-i30)
     & )))/(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp1,2)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp1,2)*jac3di(i1-
     & i10,i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp1,2)*jac3di(
     & i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp1,2)*jac3di(
     & i1-i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-
     & 2,axisp1,2)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-
     & 2,axisp1,2)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,
     & i3-2,axisp1,2)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp1,2)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1)))
     & )/(12.*dr(2))
                          a31rs = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp2,
     & 0)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp2,
     & 0)*jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,
     & axisp2,0)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,
     & axisp2,0)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*(
     & (rsxy(i1,i2+1,i3-1,axisp2,0)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-
     & (rsxy(i1,i2-1,i3-1,axisp2,0)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))
     & -((rsxy(i1,i2+2,i3-1,axisp2,0)*jac3di(i1-i10,i2+2-i20,i3-1-i30)
     & )-(rsxy(i1,i2-2,i3-1,axisp2,0)*jac3di(i1-i10,i2-2-i20,i3-1-i30)
     & )))/(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp2,0)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp2,0)*jac3di(i1-
     & i10,i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp2,0)*jac3di(
     & i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp2,0)*jac3di(
     & i1-i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-
     & 2,axisp2,0)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-
     & 2,axisp2,0)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,
     & i3-2,axisp2,0)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp2,0)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1)))
     & )/(12.*dr(2))
                          a32rs = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp2,
     & 1)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp2,
     & 1)*jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,
     & axisp2,1)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,
     & axisp2,1)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*(
     & (rsxy(i1,i2+1,i3-1,axisp2,1)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-
     & (rsxy(i1,i2-1,i3-1,axisp2,1)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))
     & -((rsxy(i1,i2+2,i3-1,axisp2,1)*jac3di(i1-i10,i2+2-i20,i3-1-i30)
     & )-(rsxy(i1,i2-2,i3-1,axisp2,1)*jac3di(i1-i10,i2-2-i20,i3-1-i30)
     & )))/(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp2,1)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp2,1)*jac3di(i1-
     & i10,i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp2,1)*jac3di(
     & i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp2,1)*jac3di(
     & i1-i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-
     & 2,axisp2,1)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-
     & 2,axisp2,1)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,
     & i3-2,axisp2,1)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp2,1)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1)))
     & )/(12.*dr(2))
                          a33rs = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp2,
     & 2)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp2,
     & 2)*jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,
     & axisp2,2)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,
     & axisp2,2)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*(
     & (rsxy(i1,i2+1,i3-1,axisp2,2)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-
     & (rsxy(i1,i2-1,i3-1,axisp2,2)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))
     & -((rsxy(i1,i2+2,i3-1,axisp2,2)*jac3di(i1-i10,i2+2-i20,i3-1-i30)
     & )-(rsxy(i1,i2-2,i3-1,axisp2,2)*jac3di(i1-i10,i2-2-i20,i3-1-i30)
     & )))/(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp2,2)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp2,2)*jac3di(i1-
     & i10,i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp2,2)*jac3di(
     & i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp2,2)*jac3di(
     & i1-i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-
     & 2,axisp2,2)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-
     & 2,axisp2,2)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,
     & i3-2,axisp2,2)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp2,2)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1)))
     & )/(12.*dr(2))
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
                          a21rt = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp1,
     & 0)*jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp1,
     & 0)*jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,
     & axisp1,0)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,
     & axisp1,0)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2-1,i3,axisp1,0)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-
     & (rsxy(i1-1,i2-1,i3,axisp1,0)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))
     & -((rsxy(i1+2,i2-1,i3,axisp1,0)*jac3di(i1+2-i10,i2-1-i20,i3-i30)
     & )-(rsxy(i1-2,i2-1,i3,axisp1,0)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp1,0)*jac3di(i1+
     & 1-i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp1,0)*jac3di(i1-
     & 1-i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp1,0)*jac3di(
     & i1+2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp1,0)*jac3di(
     & i1-2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,
     & i3,axisp1,0)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,
     & i3,axisp1,0)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-
     & 2,i3,axisp1,0)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-
     & 2,i3,axisp1,0)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0)))
     & )/(12.*dr(1))
                          a22rt = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp1,
     & 1)*jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp1,
     & 1)*jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,
     & axisp1,1)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,
     & axisp1,1)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2-1,i3,axisp1,1)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-
     & (rsxy(i1-1,i2-1,i3,axisp1,1)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))
     & -((rsxy(i1+2,i2-1,i3,axisp1,1)*jac3di(i1+2-i10,i2-1-i20,i3-i30)
     & )-(rsxy(i1-2,i2-1,i3,axisp1,1)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp1,1)*jac3di(i1+
     & 1-i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp1,1)*jac3di(i1-
     & 1-i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp1,1)*jac3di(
     & i1+2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp1,1)*jac3di(
     & i1-2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,
     & i3,axisp1,1)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,
     & i3,axisp1,1)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-
     & 2,i3,axisp1,1)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-
     & 2,i3,axisp1,1)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0)))
     & )/(12.*dr(1))
                          a23rt = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp1,
     & 2)*jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp1,
     & 2)*jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,
     & axisp1,2)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,
     & axisp1,2)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2-1,i3,axisp1,2)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-
     & (rsxy(i1-1,i2-1,i3,axisp1,2)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))
     & -((rsxy(i1+2,i2-1,i3,axisp1,2)*jac3di(i1+2-i10,i2-1-i20,i3-i30)
     & )-(rsxy(i1-2,i2-1,i3,axisp1,2)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp1,2)*jac3di(i1+
     & 1-i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp1,2)*jac3di(i1-
     & 1-i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp1,2)*jac3di(
     & i1+2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp1,2)*jac3di(
     & i1-2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,
     & i3,axisp1,2)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,
     & i3,axisp1,2)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-
     & 2,i3,axisp1,2)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-
     & 2,i3,axisp1,2)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0)))
     & )/(12.*dr(1))
                          a31rt = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp2,
     & 0)*jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp2,
     & 0)*jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,
     & axisp2,0)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,
     & axisp2,0)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2-1,i3,axisp2,0)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-
     & (rsxy(i1-1,i2-1,i3,axisp2,0)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))
     & -((rsxy(i1+2,i2-1,i3,axisp2,0)*jac3di(i1+2-i10,i2-1-i20,i3-i30)
     & )-(rsxy(i1-2,i2-1,i3,axisp2,0)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp2,0)*jac3di(i1+
     & 1-i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp2,0)*jac3di(i1-
     & 1-i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp2,0)*jac3di(
     & i1+2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp2,0)*jac3di(
     & i1-2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,
     & i3,axisp2,0)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,
     & i3,axisp2,0)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-
     & 2,i3,axisp2,0)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-
     & 2,i3,axisp2,0)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0)))
     & )/(12.*dr(1))
                          a32rt = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp2,
     & 1)*jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp2,
     & 1)*jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,
     & axisp2,1)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,
     & axisp2,1)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2-1,i3,axisp2,1)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-
     & (rsxy(i1-1,i2-1,i3,axisp2,1)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))
     & -((rsxy(i1+2,i2-1,i3,axisp2,1)*jac3di(i1+2-i10,i2-1-i20,i3-i30)
     & )-(rsxy(i1-2,i2-1,i3,axisp2,1)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp2,1)*jac3di(i1+
     & 1-i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp2,1)*jac3di(i1-
     & 1-i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp2,1)*jac3di(
     & i1+2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp2,1)*jac3di(
     & i1-2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,
     & i3,axisp2,1)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,
     & i3,axisp2,1)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-
     & 2,i3,axisp2,1)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-
     & 2,i3,axisp2,1)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0)))
     & )/(12.*dr(1))
                          a33rt = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp2,
     & 2)*jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp2,
     & 2)*jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,
     & axisp2,2)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,
     & axisp2,2)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2-1,i3,axisp2,2)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-
     & (rsxy(i1-1,i2-1,i3,axisp2,2)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))
     & -((rsxy(i1+2,i2-1,i3,axisp2,2)*jac3di(i1+2-i10,i2-1-i20,i3-i30)
     & )-(rsxy(i1-2,i2-1,i3,axisp2,2)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp2,2)*jac3di(i1+
     & 1-i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp2,2)*jac3di(i1-
     & 1-i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp2,2)*jac3di(
     & i1+2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp2,2)*jac3di(
     & i1-2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,
     & i3,axisp2,2)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,
     & i3,axisp2,2)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-
     & 2,i3,axisp2,2)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-
     & 2,i3,axisp2,2)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0)))
     & )/(12.*dr(1))
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
                          a21st = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp1,
     & 0)*jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp1,
     & 0)*jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,
     & axisp1,0)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,
     & axisp1,0)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2,i3-1,axisp1,0)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-
     & (rsxy(i1-1,i2,i3-1,axisp1,0)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))
     & -((rsxy(i1+2,i2,i3-1,axisp1,0)*jac3di(i1+2-i10,i2-i20,i3-1-i30)
     & )-(rsxy(i1-2,i2,i3-1,axisp1,0)*jac3di(i1-2-i10,i2-i20,i3-1-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp1,0)*jac3di(i1+
     & 1-i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp1,0)*jac3di(i1-
     & 1-i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp1,0)*jac3di(
     & i1+2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp1,0)*jac3di(
     & i1-2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-
     & 2,axisp1,0)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-
     & 2,axisp1,0)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,
     & i3-2,axisp1,0)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,
     & i3-2,axisp1,0)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0)))
     & )/(12.*dr(2))
                          a22st = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp1,
     & 1)*jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp1,
     & 1)*jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,
     & axisp1,1)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,
     & axisp1,1)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2,i3-1,axisp1,1)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-
     & (rsxy(i1-1,i2,i3-1,axisp1,1)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))
     & -((rsxy(i1+2,i2,i3-1,axisp1,1)*jac3di(i1+2-i10,i2-i20,i3-1-i30)
     & )-(rsxy(i1-2,i2,i3-1,axisp1,1)*jac3di(i1-2-i10,i2-i20,i3-1-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp1,1)*jac3di(i1+
     & 1-i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp1,1)*jac3di(i1-
     & 1-i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp1,1)*jac3di(
     & i1+2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp1,1)*jac3di(
     & i1-2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-
     & 2,axisp1,1)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-
     & 2,axisp1,1)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,
     & i3-2,axisp1,1)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,
     & i3-2,axisp1,1)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0)))
     & )/(12.*dr(2))
                          a23st = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp1,
     & 2)*jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp1,
     & 2)*jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,
     & axisp1,2)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,
     & axisp1,2)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2,i3-1,axisp1,2)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-
     & (rsxy(i1-1,i2,i3-1,axisp1,2)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))
     & -((rsxy(i1+2,i2,i3-1,axisp1,2)*jac3di(i1+2-i10,i2-i20,i3-1-i30)
     & )-(rsxy(i1-2,i2,i3-1,axisp1,2)*jac3di(i1-2-i10,i2-i20,i3-1-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp1,2)*jac3di(i1+
     & 1-i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp1,2)*jac3di(i1-
     & 1-i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp1,2)*jac3di(
     & i1+2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp1,2)*jac3di(
     & i1-2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-
     & 2,axisp1,2)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-
     & 2,axisp1,2)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,
     & i3-2,axisp1,2)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,
     & i3-2,axisp1,2)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0)))
     & )/(12.*dr(2))
                          a31st = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp2,
     & 0)*jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp2,
     & 0)*jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,
     & axisp2,0)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,
     & axisp2,0)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2,i3-1,axisp2,0)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-
     & (rsxy(i1-1,i2,i3-1,axisp2,0)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))
     & -((rsxy(i1+2,i2,i3-1,axisp2,0)*jac3di(i1+2-i10,i2-i20,i3-1-i30)
     & )-(rsxy(i1-2,i2,i3-1,axisp2,0)*jac3di(i1-2-i10,i2-i20,i3-1-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp2,0)*jac3di(i1+
     & 1-i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp2,0)*jac3di(i1-
     & 1-i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp2,0)*jac3di(
     & i1+2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp2,0)*jac3di(
     & i1-2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-
     & 2,axisp2,0)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-
     & 2,axisp2,0)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,
     & i3-2,axisp2,0)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,
     & i3-2,axisp2,0)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0)))
     & )/(12.*dr(2))
                          a32st = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp2,
     & 1)*jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp2,
     & 1)*jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,
     & axisp2,1)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,
     & axisp2,1)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2,i3-1,axisp2,1)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-
     & (rsxy(i1-1,i2,i3-1,axisp2,1)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))
     & -((rsxy(i1+2,i2,i3-1,axisp2,1)*jac3di(i1+2-i10,i2-i20,i3-1-i30)
     & )-(rsxy(i1-2,i2,i3-1,axisp2,1)*jac3di(i1-2-i10,i2-i20,i3-1-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp2,1)*jac3di(i1+
     & 1-i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp2,1)*jac3di(i1-
     & 1-i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp2,1)*jac3di(
     & i1+2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp2,1)*jac3di(
     & i1-2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-
     & 2,axisp2,1)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-
     & 2,axisp2,1)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,
     & i3-2,axisp2,1)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,
     & i3-2,axisp2,1)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0)))
     & )/(12.*dr(2))
                          a33st = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp2,
     & 2)*jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp2,
     & 2)*jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,
     & axisp2,2)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,
     & axisp2,2)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2,i3-1,axisp2,2)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-
     & (rsxy(i1-1,i2,i3-1,axisp2,2)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))
     & -((rsxy(i1+2,i2,i3-1,axisp2,2)*jac3di(i1+2-i10,i2-i20,i3-1-i30)
     & )-(rsxy(i1-2,i2,i3-1,axisp2,2)*jac3di(i1-2-i10,i2-i20,i3-1-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp2,2)*jac3di(i1+
     & 1-i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp2,2)*jac3di(i1-
     & 1-i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp2,2)*jac3di(
     & i1+2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp2,2)*jac3di(
     & i1-2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-
     & 2,axisp2,2)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-
     & 2,axisp2,2)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,
     & i3-2,axisp2,2)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,
     & i3-2,axisp2,2)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0)))
     & )/(12.*dr(2))
                          urt=urs4(i1,i2,i3,ex)
                          ust=urt4(i1,i2,i3,ex)
                          urtt=urrs2(i1,i2,i3,ex)
                          ustt=urrt2(i1,i2,i3,ex)
                          vrt  =urs4(i1,i2,i3,ey)
                          vst  =urt4(i1,i2,i3,ey)
                          vrtt=urrs2(i1,i2,i3,ey)
                          vstt=urrt2(i1,i2,i3,ey)
                          wrt  =urs4(i1,i2,i3,ez)
                          wst  =urt4(i1,i2,i3,ez)
                          wrtt=urrs2(i1,i2,i3,ez)
                          wstt=urrt2(i1,i2,i3,ez)
                       else if( edgeDirection.eq.1 )then
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
                          a21rs = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp1,
     & 0)*jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp1,
     & 0)*jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,
     & axisp1,0)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,
     & axisp1,0)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2,i3-1,axisp1,0)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-
     & (rsxy(i1-1,i2,i3-1,axisp1,0)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))
     & -((rsxy(i1+2,i2,i3-1,axisp1,0)*jac3di(i1+2-i10,i2-i20,i3-1-i30)
     & )-(rsxy(i1-2,i2,i3-1,axisp1,0)*jac3di(i1-2-i10,i2-i20,i3-1-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp1,0)*jac3di(i1+
     & 1-i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp1,0)*jac3di(i1-
     & 1-i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp1,0)*jac3di(
     & i1+2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp1,0)*jac3di(
     & i1-2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-
     & 2,axisp1,0)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-
     & 2,axisp1,0)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,
     & i3-2,axisp1,0)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,
     & i3-2,axisp1,0)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0)))
     & )/(12.*dr(2))
                          a22rs = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp1,
     & 1)*jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp1,
     & 1)*jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,
     & axisp1,1)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,
     & axisp1,1)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2,i3-1,axisp1,1)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-
     & (rsxy(i1-1,i2,i3-1,axisp1,1)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))
     & -((rsxy(i1+2,i2,i3-1,axisp1,1)*jac3di(i1+2-i10,i2-i20,i3-1-i30)
     & )-(rsxy(i1-2,i2,i3-1,axisp1,1)*jac3di(i1-2-i10,i2-i20,i3-1-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp1,1)*jac3di(i1+
     & 1-i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp1,1)*jac3di(i1-
     & 1-i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp1,1)*jac3di(
     & i1+2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp1,1)*jac3di(
     & i1-2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-
     & 2,axisp1,1)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-
     & 2,axisp1,1)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,
     & i3-2,axisp1,1)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,
     & i3-2,axisp1,1)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0)))
     & )/(12.*dr(2))
                          a23rs = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp1,
     & 2)*jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp1,
     & 2)*jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,
     & axisp1,2)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,
     & axisp1,2)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2,i3-1,axisp1,2)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-
     & (rsxy(i1-1,i2,i3-1,axisp1,2)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))
     & -((rsxy(i1+2,i2,i3-1,axisp1,2)*jac3di(i1+2-i10,i2-i20,i3-1-i30)
     & )-(rsxy(i1-2,i2,i3-1,axisp1,2)*jac3di(i1-2-i10,i2-i20,i3-1-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp1,2)*jac3di(i1+
     & 1-i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp1,2)*jac3di(i1-
     & 1-i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp1,2)*jac3di(
     & i1+2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp1,2)*jac3di(
     & i1-2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-
     & 2,axisp1,2)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-
     & 2,axisp1,2)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,
     & i3-2,axisp1,2)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,
     & i3-2,axisp1,2)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0)))
     & )/(12.*dr(2))
                          a31rs = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp2,
     & 0)*jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp2,
     & 0)*jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,
     & axisp2,0)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,
     & axisp2,0)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2,i3-1,axisp2,0)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-
     & (rsxy(i1-1,i2,i3-1,axisp2,0)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))
     & -((rsxy(i1+2,i2,i3-1,axisp2,0)*jac3di(i1+2-i10,i2-i20,i3-1-i30)
     & )-(rsxy(i1-2,i2,i3-1,axisp2,0)*jac3di(i1-2-i10,i2-i20,i3-1-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp2,0)*jac3di(i1+
     & 1-i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp2,0)*jac3di(i1-
     & 1-i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp2,0)*jac3di(
     & i1+2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp2,0)*jac3di(
     & i1-2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-
     & 2,axisp2,0)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-
     & 2,axisp2,0)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,
     & i3-2,axisp2,0)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,
     & i3-2,axisp2,0)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0)))
     & )/(12.*dr(2))
                          a32rs = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp2,
     & 1)*jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp2,
     & 1)*jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,
     & axisp2,1)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,
     & axisp2,1)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2,i3-1,axisp2,1)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-
     & (rsxy(i1-1,i2,i3-1,axisp2,1)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))
     & -((rsxy(i1+2,i2,i3-1,axisp2,1)*jac3di(i1+2-i10,i2-i20,i3-1-i30)
     & )-(rsxy(i1-2,i2,i3-1,axisp2,1)*jac3di(i1-2-i10,i2-i20,i3-1-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp2,1)*jac3di(i1+
     & 1-i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp2,1)*jac3di(i1-
     & 1-i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp2,1)*jac3di(
     & i1+2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp2,1)*jac3di(
     & i1-2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-
     & 2,axisp2,1)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-
     & 2,axisp2,1)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,
     & i3-2,axisp2,1)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,
     & i3-2,axisp2,1)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0)))
     & )/(12.*dr(2))
                          a33rs = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp2,
     & 2)*jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp2,
     & 2)*jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,
     & axisp2,2)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,
     & axisp2,2)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2,i3-1,axisp2,2)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-
     & (rsxy(i1-1,i2,i3-1,axisp2,2)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))
     & -((rsxy(i1+2,i2,i3-1,axisp2,2)*jac3di(i1+2-i10,i2-i20,i3-1-i30)
     & )-(rsxy(i1-2,i2,i3-1,axisp2,2)*jac3di(i1-2-i10,i2-i20,i3-1-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp2,2)*jac3di(i1+
     & 1-i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp2,2)*jac3di(i1-
     & 1-i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp2,2)*jac3di(
     & i1+2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp2,2)*jac3di(
     & i1-2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-
     & 2,axisp2,2)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-
     & 2,axisp2,2)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,
     & i3-2,axisp2,2)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,
     & i3-2,axisp2,2)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0)))
     & )/(12.*dr(2))
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
                          a21rt = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp1,
     & 0)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp1,
     & 0)*jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,
     & axisp1,0)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,
     & axisp1,0)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*(
     & (rsxy(i1,i2+1,i3-1,axisp1,0)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-
     & (rsxy(i1,i2-1,i3-1,axisp1,0)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))
     & -((rsxy(i1,i2+2,i3-1,axisp1,0)*jac3di(i1-i10,i2+2-i20,i3-1-i30)
     & )-(rsxy(i1,i2-2,i3-1,axisp1,0)*jac3di(i1-i10,i2-2-i20,i3-1-i30)
     & )))/(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp1,0)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp1,0)*jac3di(i1-
     & i10,i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp1,0)*jac3di(
     & i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp1,0)*jac3di(
     & i1-i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-
     & 2,axisp1,0)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-
     & 2,axisp1,0)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,
     & i3-2,axisp1,0)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp1,0)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1)))
     & )/(12.*dr(2))
                          a22rt = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp1,
     & 1)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp1,
     & 1)*jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,
     & axisp1,1)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,
     & axisp1,1)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*(
     & (rsxy(i1,i2+1,i3-1,axisp1,1)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-
     & (rsxy(i1,i2-1,i3-1,axisp1,1)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))
     & -((rsxy(i1,i2+2,i3-1,axisp1,1)*jac3di(i1-i10,i2+2-i20,i3-1-i30)
     & )-(rsxy(i1,i2-2,i3-1,axisp1,1)*jac3di(i1-i10,i2-2-i20,i3-1-i30)
     & )))/(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp1,1)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp1,1)*jac3di(i1-
     & i10,i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp1,1)*jac3di(
     & i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp1,1)*jac3di(
     & i1-i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-
     & 2,axisp1,1)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-
     & 2,axisp1,1)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,
     & i3-2,axisp1,1)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp1,1)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1)))
     & )/(12.*dr(2))
                          a23rt = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp1,
     & 2)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp1,
     & 2)*jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,
     & axisp1,2)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,
     & axisp1,2)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*(
     & (rsxy(i1,i2+1,i3-1,axisp1,2)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-
     & (rsxy(i1,i2-1,i3-1,axisp1,2)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))
     & -((rsxy(i1,i2+2,i3-1,axisp1,2)*jac3di(i1-i10,i2+2-i20,i3-1-i30)
     & )-(rsxy(i1,i2-2,i3-1,axisp1,2)*jac3di(i1-i10,i2-2-i20,i3-1-i30)
     & )))/(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp1,2)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp1,2)*jac3di(i1-
     & i10,i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp1,2)*jac3di(
     & i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp1,2)*jac3di(
     & i1-i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-
     & 2,axisp1,2)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-
     & 2,axisp1,2)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,
     & i3-2,axisp1,2)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp1,2)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1)))
     & )/(12.*dr(2))
                          a31rt = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp2,
     & 0)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp2,
     & 0)*jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,
     & axisp2,0)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,
     & axisp2,0)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*(
     & (rsxy(i1,i2+1,i3-1,axisp2,0)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-
     & (rsxy(i1,i2-1,i3-1,axisp2,0)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))
     & -((rsxy(i1,i2+2,i3-1,axisp2,0)*jac3di(i1-i10,i2+2-i20,i3-1-i30)
     & )-(rsxy(i1,i2-2,i3-1,axisp2,0)*jac3di(i1-i10,i2-2-i20,i3-1-i30)
     & )))/(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp2,0)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp2,0)*jac3di(i1-
     & i10,i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp2,0)*jac3di(
     & i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp2,0)*jac3di(
     & i1-i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-
     & 2,axisp2,0)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-
     & 2,axisp2,0)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,
     & i3-2,axisp2,0)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp2,0)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1)))
     & )/(12.*dr(2))
                          a32rt = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp2,
     & 1)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp2,
     & 1)*jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,
     & axisp2,1)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,
     & axisp2,1)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*(
     & (rsxy(i1,i2+1,i3-1,axisp2,1)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-
     & (rsxy(i1,i2-1,i3-1,axisp2,1)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))
     & -((rsxy(i1,i2+2,i3-1,axisp2,1)*jac3di(i1-i10,i2+2-i20,i3-1-i30)
     & )-(rsxy(i1,i2-2,i3-1,axisp2,1)*jac3di(i1-i10,i2-2-i20,i3-1-i30)
     & )))/(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp2,1)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp2,1)*jac3di(i1-
     & i10,i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp2,1)*jac3di(
     & i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp2,1)*jac3di(
     & i1-i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-
     & 2,axisp2,1)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-
     & 2,axisp2,1)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,
     & i3-2,axisp2,1)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp2,1)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1)))
     & )/(12.*dr(2))
                          a33rt = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp2,
     & 2)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp2,
     & 2)*jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,
     & axisp2,2)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,
     & axisp2,2)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*(
     & (rsxy(i1,i2+1,i3-1,axisp2,2)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-
     & (rsxy(i1,i2-1,i3-1,axisp2,2)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))
     & -((rsxy(i1,i2+2,i3-1,axisp2,2)*jac3di(i1-i10,i2+2-i20,i3-1-i30)
     & )-(rsxy(i1,i2-2,i3-1,axisp2,2)*jac3di(i1-i10,i2-2-i20,i3-1-i30)
     & )))/(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp2,2)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp2,2)*jac3di(i1-
     & i10,i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp2,2)*jac3di(
     & i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp2,2)*jac3di(
     & i1-i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-
     & 2,axisp2,2)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-
     & 2,axisp2,2)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,
     & i3-2,axisp2,2)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp2,2)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1)))
     & )/(12.*dr(2))
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
                          a21st = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp1,
     & 0)*jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp1,
     & 0)*jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,
     & axisp1,0)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,
     & axisp1,0)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2-1,i3,axisp1,0)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-
     & (rsxy(i1-1,i2-1,i3,axisp1,0)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))
     & -((rsxy(i1+2,i2-1,i3,axisp1,0)*jac3di(i1+2-i10,i2-1-i20,i3-i30)
     & )-(rsxy(i1-2,i2-1,i3,axisp1,0)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp1,0)*jac3di(i1+
     & 1-i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp1,0)*jac3di(i1-
     & 1-i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp1,0)*jac3di(
     & i1+2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp1,0)*jac3di(
     & i1-2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,
     & i3,axisp1,0)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,
     & i3,axisp1,0)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-
     & 2,i3,axisp1,0)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-
     & 2,i3,axisp1,0)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0)))
     & )/(12.*dr(1))
                          a22st = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp1,
     & 1)*jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp1,
     & 1)*jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,
     & axisp1,1)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,
     & axisp1,1)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2-1,i3,axisp1,1)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-
     & (rsxy(i1-1,i2-1,i3,axisp1,1)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))
     & -((rsxy(i1+2,i2-1,i3,axisp1,1)*jac3di(i1+2-i10,i2-1-i20,i3-i30)
     & )-(rsxy(i1-2,i2-1,i3,axisp1,1)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp1,1)*jac3di(i1+
     & 1-i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp1,1)*jac3di(i1-
     & 1-i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp1,1)*jac3di(
     & i1+2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp1,1)*jac3di(
     & i1-2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,
     & i3,axisp1,1)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,
     & i3,axisp1,1)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-
     & 2,i3,axisp1,1)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-
     & 2,i3,axisp1,1)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0)))
     & )/(12.*dr(1))
                          a23st = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp1,
     & 2)*jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp1,
     & 2)*jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,
     & axisp1,2)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,
     & axisp1,2)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2-1,i3,axisp1,2)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-
     & (rsxy(i1-1,i2-1,i3,axisp1,2)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))
     & -((rsxy(i1+2,i2-1,i3,axisp1,2)*jac3di(i1+2-i10,i2-1-i20,i3-i30)
     & )-(rsxy(i1-2,i2-1,i3,axisp1,2)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp1,2)*jac3di(i1+
     & 1-i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp1,2)*jac3di(i1-
     & 1-i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp1,2)*jac3di(
     & i1+2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp1,2)*jac3di(
     & i1-2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,
     & i3,axisp1,2)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,
     & i3,axisp1,2)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-
     & 2,i3,axisp1,2)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-
     & 2,i3,axisp1,2)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0)))
     & )/(12.*dr(1))
                          a31st = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp2,
     & 0)*jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp2,
     & 0)*jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,
     & axisp2,0)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,
     & axisp2,0)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2-1,i3,axisp2,0)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-
     & (rsxy(i1-1,i2-1,i3,axisp2,0)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))
     & -((rsxy(i1+2,i2-1,i3,axisp2,0)*jac3di(i1+2-i10,i2-1-i20,i3-i30)
     & )-(rsxy(i1-2,i2-1,i3,axisp2,0)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp2,0)*jac3di(i1+
     & 1-i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp2,0)*jac3di(i1-
     & 1-i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp2,0)*jac3di(
     & i1+2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp2,0)*jac3di(
     & i1-2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,
     & i3,axisp2,0)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,
     & i3,axisp2,0)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-
     & 2,i3,axisp2,0)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-
     & 2,i3,axisp2,0)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0)))
     & )/(12.*dr(1))
                          a32st = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp2,
     & 1)*jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp2,
     & 1)*jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,
     & axisp2,1)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,
     & axisp2,1)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2-1,i3,axisp2,1)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-
     & (rsxy(i1-1,i2-1,i3,axisp2,1)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))
     & -((rsxy(i1+2,i2-1,i3,axisp2,1)*jac3di(i1+2-i10,i2-1-i20,i3-i30)
     & )-(rsxy(i1-2,i2-1,i3,axisp2,1)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp2,1)*jac3di(i1+
     & 1-i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp2,1)*jac3di(i1-
     & 1-i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp2,1)*jac3di(
     & i1+2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp2,1)*jac3di(
     & i1-2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,
     & i3,axisp2,1)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,
     & i3,axisp2,1)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-
     & 2,i3,axisp2,1)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-
     & 2,i3,axisp2,1)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0)))
     & )/(12.*dr(1))
                          a33st = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp2,
     & 2)*jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp2,
     & 2)*jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,
     & axisp2,2)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,
     & axisp2,2)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2-1,i3,axisp2,2)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-
     & (rsxy(i1-1,i2-1,i3,axisp2,2)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))
     & -((rsxy(i1+2,i2-1,i3,axisp2,2)*jac3di(i1+2-i10,i2-1-i20,i3-i30)
     & )-(rsxy(i1-2,i2-1,i3,axisp2,2)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp2,2)*jac3di(i1+
     & 1-i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp2,2)*jac3di(i1-
     & 1-i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp2,2)*jac3di(
     & i1+2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp2,2)*jac3di(
     & i1-2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,
     & i3,axisp2,2)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,
     & i3,axisp2,2)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-
     & 2,i3,axisp2,2)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-
     & 2,i3,axisp2,2)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0)))
     & )/(12.*dr(1))
                          urt=ust4(i1,i2,i3,ex)
                          ust=urs4(i1,i2,i3,ex)
                          urtt=usst2(i1,i2,i3,ex)
                          ustt=urss2(i1,i2,i3,ex)
                          vrt  =ust4(i1,i2,i3,ey)
                          vst  =urs4(i1,i2,i3,ey)
                          vrtt=usst2(i1,i2,i3,ey)
                          vstt=urss2(i1,i2,i3,ey)
                          wrt  =ust4(i1,i2,i3,ez)
                          wst  =urs4(i1,i2,i3,ez)
                          wrtt=usst2(i1,i2,i3,ez)
                          wstt=urss2(i1,i2,i3,ez)
                       else ! edgeDirection.eq.2
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
                          a21rs = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp1,
     & 0)*jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp1,
     & 0)*jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,
     & axisp1,0)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,
     & axisp1,0)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2-1,i3,axisp1,0)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-
     & (rsxy(i1-1,i2-1,i3,axisp1,0)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))
     & -((rsxy(i1+2,i2-1,i3,axisp1,0)*jac3di(i1+2-i10,i2-1-i20,i3-i30)
     & )-(rsxy(i1-2,i2-1,i3,axisp1,0)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp1,0)*jac3di(i1+
     & 1-i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp1,0)*jac3di(i1-
     & 1-i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp1,0)*jac3di(
     & i1+2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp1,0)*jac3di(
     & i1-2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,
     & i3,axisp1,0)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,
     & i3,axisp1,0)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-
     & 2,i3,axisp1,0)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-
     & 2,i3,axisp1,0)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0)))
     & )/(12.*dr(1))
                          a22rs = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp1,
     & 1)*jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp1,
     & 1)*jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,
     & axisp1,1)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,
     & axisp1,1)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2-1,i3,axisp1,1)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-
     & (rsxy(i1-1,i2-1,i3,axisp1,1)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))
     & -((rsxy(i1+2,i2-1,i3,axisp1,1)*jac3di(i1+2-i10,i2-1-i20,i3-i30)
     & )-(rsxy(i1-2,i2-1,i3,axisp1,1)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp1,1)*jac3di(i1+
     & 1-i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp1,1)*jac3di(i1-
     & 1-i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp1,1)*jac3di(
     & i1+2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp1,1)*jac3di(
     & i1-2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,
     & i3,axisp1,1)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,
     & i3,axisp1,1)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-
     & 2,i3,axisp1,1)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-
     & 2,i3,axisp1,1)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0)))
     & )/(12.*dr(1))
                          a23rs = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp1,
     & 2)*jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp1,
     & 2)*jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,
     & axisp1,2)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,
     & axisp1,2)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2-1,i3,axisp1,2)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-
     & (rsxy(i1-1,i2-1,i3,axisp1,2)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))
     & -((rsxy(i1+2,i2-1,i3,axisp1,2)*jac3di(i1+2-i10,i2-1-i20,i3-i30)
     & )-(rsxy(i1-2,i2-1,i3,axisp1,2)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp1,2)*jac3di(i1+
     & 1-i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp1,2)*jac3di(i1-
     & 1-i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp1,2)*jac3di(
     & i1+2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp1,2)*jac3di(
     & i1-2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,
     & i3,axisp1,2)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,
     & i3,axisp1,2)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-
     & 2,i3,axisp1,2)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-
     & 2,i3,axisp1,2)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0)))
     & )/(12.*dr(1))
                          a31rs = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp2,
     & 0)*jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp2,
     & 0)*jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,
     & axisp2,0)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,
     & axisp2,0)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2-1,i3,axisp2,0)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-
     & (rsxy(i1-1,i2-1,i3,axisp2,0)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))
     & -((rsxy(i1+2,i2-1,i3,axisp2,0)*jac3di(i1+2-i10,i2-1-i20,i3-i30)
     & )-(rsxy(i1-2,i2-1,i3,axisp2,0)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp2,0)*jac3di(i1+
     & 1-i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp2,0)*jac3di(i1-
     & 1-i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp2,0)*jac3di(
     & i1+2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp2,0)*jac3di(
     & i1-2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,
     & i3,axisp2,0)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,
     & i3,axisp2,0)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-
     & 2,i3,axisp2,0)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-
     & 2,i3,axisp2,0)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0)))
     & )/(12.*dr(1))
                          a32rs = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp2,
     & 1)*jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp2,
     & 1)*jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,
     & axisp2,1)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,
     & axisp2,1)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2-1,i3,axisp2,1)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-
     & (rsxy(i1-1,i2-1,i3,axisp2,1)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))
     & -((rsxy(i1+2,i2-1,i3,axisp2,1)*jac3di(i1+2-i10,i2-1-i20,i3-i30)
     & )-(rsxy(i1-2,i2-1,i3,axisp2,1)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp2,1)*jac3di(i1+
     & 1-i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp2,1)*jac3di(i1-
     & 1-i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp2,1)*jac3di(
     & i1+2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp2,1)*jac3di(
     & i1-2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,
     & i3,axisp2,1)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,
     & i3,axisp2,1)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-
     & 2,i3,axisp2,1)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-
     & 2,i3,axisp2,1)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0)))
     & )/(12.*dr(1))
                          a33rs = (8.*((8.*((rsxy(i1+1,i2+1,i3,axisp2,
     & 2)*jac3di(i1+1-i10,i2+1-i20,i3-i30))-(rsxy(i1-1,i2+1,i3,axisp2,
     & 2)*jac3di(i1-1-i10,i2+1-i20,i3-i30)))-((rsxy(i1+2,i2+1,i3,
     & axisp2,2)*jac3di(i1+2-i10,i2+1-i20,i3-i30))-(rsxy(i1-2,i2+1,i3,
     & axisp2,2)*jac3di(i1-2-i10,i2+1-i20,i3-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2-1,i3,axisp2,2)*jac3di(i1+1-i10,i2-1-i20,i3-i30))-
     & (rsxy(i1-1,i2-1,i3,axisp2,2)*jac3di(i1-1-i10,i2-1-i20,i3-i30)))
     & -((rsxy(i1+2,i2-1,i3,axisp2,2)*jac3di(i1+2-i10,i2-1-i20,i3-i30)
     & )-(rsxy(i1-2,i2-1,i3,axisp2,2)*jac3di(i1-2-i10,i2-1-i20,i3-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2+2,i3,axisp2,2)*jac3di(i1+
     & 1-i10,i2+2-i20,i3-i30))-(rsxy(i1-1,i2+2,i3,axisp2,2)*jac3di(i1-
     & 1-i10,i2+2-i20,i3-i30)))-((rsxy(i1+2,i2+2,i3,axisp2,2)*jac3di(
     & i1+2-i10,i2+2-i20,i3-i30))-(rsxy(i1-2,i2+2,i3,axisp2,2)*jac3di(
     & i1-2-i10,i2+2-i20,i3-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2-2,
     & i3,axisp2,2)*jac3di(i1+1-i10,i2-2-i20,i3-i30))-(rsxy(i1-1,i2-2,
     & i3,axisp2,2)*jac3di(i1-1-i10,i2-2-i20,i3-i30)))-((rsxy(i1+2,i2-
     & 2,i3,axisp2,2)*jac3di(i1+2-i10,i2-2-i20,i3-i30))-(rsxy(i1-2,i2-
     & 2,i3,axisp2,2)*jac3di(i1-2-i10,i2-2-i20,i3-i30))))/(12.*dr(0)))
     & )/(12.*dr(1))
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
                          a21rt = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp1,
     & 0)*jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp1,
     & 0)*jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,
     & axisp1,0)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,
     & axisp1,0)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2,i3-1,axisp1,0)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-
     & (rsxy(i1-1,i2,i3-1,axisp1,0)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))
     & -((rsxy(i1+2,i2,i3-1,axisp1,0)*jac3di(i1+2-i10,i2-i20,i3-1-i30)
     & )-(rsxy(i1-2,i2,i3-1,axisp1,0)*jac3di(i1-2-i10,i2-i20,i3-1-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp1,0)*jac3di(i1+
     & 1-i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp1,0)*jac3di(i1-
     & 1-i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp1,0)*jac3di(
     & i1+2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp1,0)*jac3di(
     & i1-2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-
     & 2,axisp1,0)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-
     & 2,axisp1,0)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,
     & i3-2,axisp1,0)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,
     & i3-2,axisp1,0)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0)))
     & )/(12.*dr(2))
                          a22rt = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp1,
     & 1)*jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp1,
     & 1)*jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,
     & axisp1,1)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,
     & axisp1,1)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2,i3-1,axisp1,1)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-
     & (rsxy(i1-1,i2,i3-1,axisp1,1)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))
     & -((rsxy(i1+2,i2,i3-1,axisp1,1)*jac3di(i1+2-i10,i2-i20,i3-1-i30)
     & )-(rsxy(i1-2,i2,i3-1,axisp1,1)*jac3di(i1-2-i10,i2-i20,i3-1-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp1,1)*jac3di(i1+
     & 1-i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp1,1)*jac3di(i1-
     & 1-i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp1,1)*jac3di(
     & i1+2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp1,1)*jac3di(
     & i1-2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-
     & 2,axisp1,1)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-
     & 2,axisp1,1)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,
     & i3-2,axisp1,1)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,
     & i3-2,axisp1,1)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0)))
     & )/(12.*dr(2))
                          a23rt = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp1,
     & 2)*jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp1,
     & 2)*jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,
     & axisp1,2)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,
     & axisp1,2)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2,i3-1,axisp1,2)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-
     & (rsxy(i1-1,i2,i3-1,axisp1,2)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))
     & -((rsxy(i1+2,i2,i3-1,axisp1,2)*jac3di(i1+2-i10,i2-i20,i3-1-i30)
     & )-(rsxy(i1-2,i2,i3-1,axisp1,2)*jac3di(i1-2-i10,i2-i20,i3-1-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp1,2)*jac3di(i1+
     & 1-i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp1,2)*jac3di(i1-
     & 1-i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp1,2)*jac3di(
     & i1+2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp1,2)*jac3di(
     & i1-2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-
     & 2,axisp1,2)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-
     & 2,axisp1,2)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,
     & i3-2,axisp1,2)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,
     & i3-2,axisp1,2)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0)))
     & )/(12.*dr(2))
                          a31rt = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp2,
     & 0)*jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp2,
     & 0)*jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,
     & axisp2,0)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,
     & axisp2,0)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2,i3-1,axisp2,0)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-
     & (rsxy(i1-1,i2,i3-1,axisp2,0)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))
     & -((rsxy(i1+2,i2,i3-1,axisp2,0)*jac3di(i1+2-i10,i2-i20,i3-1-i30)
     & )-(rsxy(i1-2,i2,i3-1,axisp2,0)*jac3di(i1-2-i10,i2-i20,i3-1-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp2,0)*jac3di(i1+
     & 1-i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp2,0)*jac3di(i1-
     & 1-i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp2,0)*jac3di(
     & i1+2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp2,0)*jac3di(
     & i1-2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-
     & 2,axisp2,0)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-
     & 2,axisp2,0)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,
     & i3-2,axisp2,0)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,
     & i3-2,axisp2,0)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0)))
     & )/(12.*dr(2))
                          a32rt = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp2,
     & 1)*jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp2,
     & 1)*jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,
     & axisp2,1)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,
     & axisp2,1)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2,i3-1,axisp2,1)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-
     & (rsxy(i1-1,i2,i3-1,axisp2,1)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))
     & -((rsxy(i1+2,i2,i3-1,axisp2,1)*jac3di(i1+2-i10,i2-i20,i3-1-i30)
     & )-(rsxy(i1-2,i2,i3-1,axisp2,1)*jac3di(i1-2-i10,i2-i20,i3-1-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp2,1)*jac3di(i1+
     & 1-i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp2,1)*jac3di(i1-
     & 1-i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp2,1)*jac3di(
     & i1+2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp2,1)*jac3di(
     & i1-2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-
     & 2,axisp2,1)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-
     & 2,axisp2,1)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,
     & i3-2,axisp2,1)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,
     & i3-2,axisp2,1)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0)))
     & )/(12.*dr(2))
                          a33rt = (8.*((8.*((rsxy(i1+1,i2,i3+1,axisp2,
     & 2)*jac3di(i1+1-i10,i2-i20,i3+1-i30))-(rsxy(i1-1,i2,i3+1,axisp2,
     & 2)*jac3di(i1-1-i10,i2-i20,i3+1-i30)))-((rsxy(i1+2,i2,i3+1,
     & axisp2,2)*jac3di(i1+2-i10,i2-i20,i3+1-i30))-(rsxy(i1-2,i2,i3+1,
     & axisp2,2)*jac3di(i1-2-i10,i2-i20,i3+1-i30))))/(12.*dr(0))-(8.*(
     & (rsxy(i1+1,i2,i3-1,axisp2,2)*jac3di(i1+1-i10,i2-i20,i3-1-i30))-
     & (rsxy(i1-1,i2,i3-1,axisp2,2)*jac3di(i1-1-i10,i2-i20,i3-1-i30)))
     & -((rsxy(i1+2,i2,i3-1,axisp2,2)*jac3di(i1+2-i10,i2-i20,i3-1-i30)
     & )-(rsxy(i1-2,i2,i3-1,axisp2,2)*jac3di(i1-2-i10,i2-i20,i3-1-i30)
     & )))/(12.*dr(0)))-((8.*((rsxy(i1+1,i2,i3+2,axisp2,2)*jac3di(i1+
     & 1-i10,i2-i20,i3+2-i30))-(rsxy(i1-1,i2,i3+2,axisp2,2)*jac3di(i1-
     & 1-i10,i2-i20,i3+2-i30)))-((rsxy(i1+2,i2,i3+2,axisp2,2)*jac3di(
     & i1+2-i10,i2-i20,i3+2-i30))-(rsxy(i1-2,i2,i3+2,axisp2,2)*jac3di(
     & i1-2-i10,i2-i20,i3+2-i30))))/(12.*dr(0))-(8.*((rsxy(i1+1,i2,i3-
     & 2,axisp2,2)*jac3di(i1+1-i10,i2-i20,i3-2-i30))-(rsxy(i1-1,i2,i3-
     & 2,axisp2,2)*jac3di(i1-1-i10,i2-i20,i3-2-i30)))-((rsxy(i1+2,i2,
     & i3-2,axisp2,2)*jac3di(i1+2-i10,i2-i20,i3-2-i30))-(rsxy(i1-2,i2,
     & i3-2,axisp2,2)*jac3di(i1-2-i10,i2-i20,i3-2-i30))))/(12.*dr(0)))
     & )/(12.*dr(2))
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
                          a21st = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp1,
     & 0)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp1,
     & 0)*jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,
     & axisp1,0)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,
     & axisp1,0)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*(
     & (rsxy(i1,i2+1,i3-1,axisp1,0)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-
     & (rsxy(i1,i2-1,i3-1,axisp1,0)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))
     & -((rsxy(i1,i2+2,i3-1,axisp1,0)*jac3di(i1-i10,i2+2-i20,i3-1-i30)
     & )-(rsxy(i1,i2-2,i3-1,axisp1,0)*jac3di(i1-i10,i2-2-i20,i3-1-i30)
     & )))/(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp1,0)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp1,0)*jac3di(i1-
     & i10,i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp1,0)*jac3di(
     & i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp1,0)*jac3di(
     & i1-i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-
     & 2,axisp1,0)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-
     & 2,axisp1,0)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,
     & i3-2,axisp1,0)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp1,0)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1)))
     & )/(12.*dr(2))
                          a22st = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp1,
     & 1)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp1,
     & 1)*jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,
     & axisp1,1)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,
     & axisp1,1)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*(
     & (rsxy(i1,i2+1,i3-1,axisp1,1)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-
     & (rsxy(i1,i2-1,i3-1,axisp1,1)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))
     & -((rsxy(i1,i2+2,i3-1,axisp1,1)*jac3di(i1-i10,i2+2-i20,i3-1-i30)
     & )-(rsxy(i1,i2-2,i3-1,axisp1,1)*jac3di(i1-i10,i2-2-i20,i3-1-i30)
     & )))/(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp1,1)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp1,1)*jac3di(i1-
     & i10,i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp1,1)*jac3di(
     & i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp1,1)*jac3di(
     & i1-i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-
     & 2,axisp1,1)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-
     & 2,axisp1,1)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,
     & i3-2,axisp1,1)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp1,1)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1)))
     & )/(12.*dr(2))
                          a23st = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp1,
     & 2)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp1,
     & 2)*jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,
     & axisp1,2)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,
     & axisp1,2)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*(
     & (rsxy(i1,i2+1,i3-1,axisp1,2)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-
     & (rsxy(i1,i2-1,i3-1,axisp1,2)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))
     & -((rsxy(i1,i2+2,i3-1,axisp1,2)*jac3di(i1-i10,i2+2-i20,i3-1-i30)
     & )-(rsxy(i1,i2-2,i3-1,axisp1,2)*jac3di(i1-i10,i2-2-i20,i3-1-i30)
     & )))/(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp1,2)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp1,2)*jac3di(i1-
     & i10,i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp1,2)*jac3di(
     & i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp1,2)*jac3di(
     & i1-i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-
     & 2,axisp1,2)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-
     & 2,axisp1,2)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,
     & i3-2,axisp1,2)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp1,2)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1)))
     & )/(12.*dr(2))
                          a31st = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp2,
     & 0)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp2,
     & 0)*jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,
     & axisp2,0)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,
     & axisp2,0)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*(
     & (rsxy(i1,i2+1,i3-1,axisp2,0)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-
     & (rsxy(i1,i2-1,i3-1,axisp2,0)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))
     & -((rsxy(i1,i2+2,i3-1,axisp2,0)*jac3di(i1-i10,i2+2-i20,i3-1-i30)
     & )-(rsxy(i1,i2-2,i3-1,axisp2,0)*jac3di(i1-i10,i2-2-i20,i3-1-i30)
     & )))/(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp2,0)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp2,0)*jac3di(i1-
     & i10,i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp2,0)*jac3di(
     & i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp2,0)*jac3di(
     & i1-i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-
     & 2,axisp2,0)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-
     & 2,axisp2,0)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,
     & i3-2,axisp2,0)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp2,0)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1)))
     & )/(12.*dr(2))
                          a32st = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp2,
     & 1)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp2,
     & 1)*jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,
     & axisp2,1)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,
     & axisp2,1)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*(
     & (rsxy(i1,i2+1,i3-1,axisp2,1)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-
     & (rsxy(i1,i2-1,i3-1,axisp2,1)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))
     & -((rsxy(i1,i2+2,i3-1,axisp2,1)*jac3di(i1-i10,i2+2-i20,i3-1-i30)
     & )-(rsxy(i1,i2-2,i3-1,axisp2,1)*jac3di(i1-i10,i2-2-i20,i3-1-i30)
     & )))/(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp2,1)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp2,1)*jac3di(i1-
     & i10,i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp2,1)*jac3di(
     & i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp2,1)*jac3di(
     & i1-i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-
     & 2,axisp2,1)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-
     & 2,axisp2,1)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,
     & i3-2,axisp2,1)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp2,1)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1)))
     & )/(12.*dr(2))
                          a33st = (8.*((8.*((rsxy(i1,i2+1,i3+1,axisp2,
     & 2)*jac3di(i1-i10,i2+1-i20,i3+1-i30))-(rsxy(i1,i2-1,i3+1,axisp2,
     & 2)*jac3di(i1-i10,i2-1-i20,i3+1-i30)))-((rsxy(i1,i2+2,i3+1,
     & axisp2,2)*jac3di(i1-i10,i2+2-i20,i3+1-i30))-(rsxy(i1,i2-2,i3+1,
     & axisp2,2)*jac3di(i1-i10,i2-2-i20,i3+1-i30))))/(12.*dr(1))-(8.*(
     & (rsxy(i1,i2+1,i3-1,axisp2,2)*jac3di(i1-i10,i2+1-i20,i3-1-i30))-
     & (rsxy(i1,i2-1,i3-1,axisp2,2)*jac3di(i1-i10,i2-1-i20,i3-1-i30)))
     & -((rsxy(i1,i2+2,i3-1,axisp2,2)*jac3di(i1-i10,i2+2-i20,i3-1-i30)
     & )-(rsxy(i1,i2-2,i3-1,axisp2,2)*jac3di(i1-i10,i2-2-i20,i3-1-i30)
     & )))/(12.*dr(1)))-((8.*((rsxy(i1,i2+1,i3+2,axisp2,2)*jac3di(i1-
     & i10,i2+1-i20,i3+2-i30))-(rsxy(i1,i2-1,i3+2,axisp2,2)*jac3di(i1-
     & i10,i2-1-i20,i3+2-i30)))-((rsxy(i1,i2+2,i3+2,axisp2,2)*jac3di(
     & i1-i10,i2+2-i20,i3+2-i30))-(rsxy(i1,i2-2,i3+2,axisp2,2)*jac3di(
     & i1-i10,i2-2-i20,i3+2-i30))))/(12.*dr(1))-(8.*((rsxy(i1,i2+1,i3-
     & 2,axisp2,2)*jac3di(i1-i10,i2+1-i20,i3-2-i30))-(rsxy(i1,i2-1,i3-
     & 2,axisp2,2)*jac3di(i1-i10,i2-1-i20,i3-2-i30)))-((rsxy(i1,i2+2,
     & i3-2,axisp2,2)*jac3di(i1-i10,i2+2-i20,i3-2-i30))-(rsxy(i1,i2-2,
     & i3-2,axisp2,2)*jac3di(i1-i10,i2-2-i20,i3-2-i30))))/(12.*dr(1)))
     & )/(12.*dr(2))
                          urt=urt4(i1,i2,i3,ex)
                          ust=ust4(i1,i2,i3,ex)
                          urtt=urtt2(i1,i2,i3,ex)
                          ustt=ustt2(i1,i2,i3,ex)
                          vrt  =urt4(i1,i2,i3,ey)
                          vst  =ust4(i1,i2,i3,ey)
                          vrtt=urtt2(i1,i2,i3,ey)
                          vstt=ustt2(i1,i2,i3,ey)
                          wrt  =urt4(i1,i2,i3,ez)
                          wst  =ust4(i1,i2,i3,ez)
                          wrtt=urtt2(i1,i2,i3,ez)
                          wstt=ustt2(i1,i2,i3,ez)
                       end if
                      uex=u(i1,i2,i3,ex)
                      uey=u(i1,i2,i3,ey)
                      uez=u(i1,i2,i3,ez)
                      ! We get a1.urs, a2.urs from the divergence:
                      ! a1.ur = -( a1r.u + a2.us + a2s.u + a3.ut + a3t.u )
                      ! a1.urs = -( a1s.ur + a1r.us + a1rs.u + a2.uss + 2*a2s.us +a2ss*u + a3s.ut + a3.ust +  a3t.us + a3st.u)
                      a1Doturs = -( (a11s*ur  +a12s*vr  +a13s*wr  ) +(
     & a11r*us  +a12r*vs  +a13r*ws  ) +(a11rs*uex+a12rs*uey+a13rs*uez)
     &  +(a21*uss  +a22*vss  +a23*wss  ) +2.*(a21s*us  +a22s*vs  +
     & a23s*ws  ) +(a21ss*uex+a22ss*uey+a23ss*uez) +(a31s*ut  +a32s*
     & vt  +a33s*wt  ) +(a31*ust  +a32*vst  +a33*wst  ) +(a31t*us  +
     & a32t*vs  +a33t*ws  ) +(a31st*uex+a32st*uey+a33st*uez) )
                      ! a2.us = -( a1.ur + a1r.u + a2s.u + a3.ut + a3t.u )
                      ! a2.urs = -(  a1.urr+2*a1r*ur + a1rr*u + a2r.us +a2s.ur + a2rs.u+   a3r.ut + a3.urt +  a3t.ur + a3rt.u
                      a2Doturs = -( (a21s*ur  +a22s*vr  +a23s*wr  ) +(
     & a21r*us  +a22r*vs  +a23r*ws  ) +(a21rs*uex+a22rs*uey+a23rs*uez)
     &  +(a11*urr  +a12*vrr  +a13*wrr  ) +2.*(a11r*ur  +a12r*vr  +
     & a13r*wr  ) +(a11rr*uex+a12rr*uey+a13rr*uez) +(a31r*ut  +a32r*
     & vt  +a33r*wt  ) +(a31*urt  +a32*vrt  +a33*wrt  ) +(a31t*ur  +
     & a32t*vr  +a33t*wr  ) +(a31rt*uex+a32rt*uey+a33rt*uez) )
                      ! here is a first order approximation to urs, used in the formula for urss and urrs below
                      ! urs = ( (u(i1+is1+js1,i2+is2+js2,i3+is3+js3,ex)-u(i1+js1,i2+js2,i3+js3,ex)) !       - (u(i1+is1    ,i2+is2    ,i3+is3    ,ex)-u(i1    ,i2    ,i3    ,ex)) )/(dra*dsa)
                      ! vrs = ( (u(i1+is1+js1,i2+is2+js2,i3+is3+js3,ey)-u(i1+js1,i2+js2,i3+js3,ey)) !       - (u(i1+is1    ,i2+is2    ,i3+is3    ,ey)-u(i1    ,i2    ,i3    ,ey)) )/(dra*dsa)
                      ! wrs = ( (u(i1+is1+js1,i2+is2+js2,i3+is3+js3,ez)-u(i1+js1,i2+js2,i3+js3,ez)) !       - (u(i1+is1    ,i2+is2    ,i3+is3    ,ez)-u(i1    ,i2    ,i3    ,ez)) )/(dra*dsa)
                      ! here is a second order approximation to urs from :
                      !  u(r,s)   =u0 + (r*ur+s*us) + (1/2)*( r^2*urr + 2*r*s*urs + s^2*uss ) + (1/6)*( r^3*urrr + ... )
                      !  u(2r,2s) =u0 +2(         ) + (4/2)*(                               ) + (8/6)*(                ) 
                      urs = ( 8.*u(i1+is1+js1,i2+is2+js2,i3+is3+js3,ex)
     & -u(i1+2*is1+2*js1,i2+2*is2+2*js2,i3+2*is3+2*js3,ex)-7.*uex -6.*
     & (dra*ur+dsa*us)-2.*(dra**2*urr+dsa**2*uss) )/(4.*dra*dsa)
                      vrs = ( 8.*u(i1+is1+js1,i2+is2+js2,i3+is3+js3,ey)
     & -u(i1+2*is1+2*js1,i2+2*is2+2*js2,i3+2*is3+2*js3,ey)-7.*uey -6.*
     & (dra*vr+dsa*vs)-2.*(dra**2*vrr+dsa**2*vss) )/(4.*dra*dsa)
                      wrs = ( 8.*u(i1+is1+js1,i2+is2+js2,i3+is3+js3,ez)
     & -u(i1+2*is1+2*js1,i2+2*is2+2*js2,i3+2*is3+2*js3,ez)-7.*uez -6.*
     & (dra*wr+dsa*ws)-2.*(dra**2*wrr+dsa**2*wss) )/(4.*dra*dsa)
                      uLapr=0.
                      vLapr=0.
                      wLapr=0.
                      uLaps=0.
                      vLaps=0.
                      wLaps=0.
                         ! we need to define uLap, uLaps
                          call ogDeriv3(ep, 0,2,0,0, xy(i1-is1,i2-is2,
     & i3-is3,0),xy(i1-is1,i2-is2,i3-is3,1),xy(i1-is1,i2-is2,i3-is3,2)
     & ,t, ex,uxxm1, ey,vxxm1, ez,wxxm1)
                          call ogDeriv3(ep, 0,0,2,0, xy(i1-is1,i2-is2,
     & i3-is3,0),xy(i1-is1,i2-is2,i3-is3,1),xy(i1-is1,i2-is2,i3-is3,2)
     & ,t, ex,uyym1, ey,vyym1, ez,wyym1)
                          call ogDeriv3(ep, 0,0,0,2, xy(i1-is1,i2-is2,
     & i3-is3,0),xy(i1-is1,i2-is2,i3-is3,1),xy(i1-is1,i2-is2,i3-is3,2)
     & ,t, ex,uzzm1, ey,vzzm1, ez,wzzm1)
                          call ogDeriv3(ep, 0,2,0,0, xy(i1+is1,i2+is2,
     & i3+is3,0),xy(i1+is1,i2+is2,i3+is3,1),xy(i1+is1,i2+is2,i3+is3,2)
     & ,t, ex,uxxp1, ey,vxxp1, ez,wxxp1)
                          call ogDeriv3(ep, 0,0,2,0, xy(i1+is1,i2+is2,
     & i3+is3,0),xy(i1+is1,i2+is2,i3+is3,1),xy(i1+is1,i2+is2,i3+is3,2)
     & ,t, ex,uyyp1, ey,vyyp1, ez,wyyp1)
                          call ogDeriv3(ep, 0,0,0,2, xy(i1+is1,i2+is2,
     & i3+is3,0),xy(i1+is1,i2+is2,i3+is3,1),xy(i1+is1,i2+is2,i3+is3,2)
     & ,t, ex,uzzp1, ey,vzzp1, ez,wzzp1)
                          call ogDeriv3(ep, 0,2,0,0, xy(i1-2*is1,i2-2*
     & is2,i3-2*is3,0),xy(i1-2*is1,i2-2*is2,i3-2*is3,1),xy(i1-2*is1,
     & i2-2*is2,i3-2*is3,2),t, ex,uxxm2, ey,vxxm2, ez,wxxm2)
                          call ogDeriv3(ep, 0,0,2,0, xy(i1-2*is1,i2-2*
     & is2,i3-2*is3,0),xy(i1-2*is1,i2-2*is2,i3-2*is3,1),xy(i1-2*is1,
     & i2-2*is2,i3-2*is3,2),t, ex,uyym2, ey,vyym2, ez,wyym2)
                          call ogDeriv3(ep, 0,0,0,2, xy(i1-2*is1,i2-2*
     & is2,i3-2*is3,0),xy(i1-2*is1,i2-2*is2,i3-2*is3,1),xy(i1-2*is1,
     & i2-2*is2,i3-2*is3,2),t, ex,uzzm2, ey,vzzm2, ez,wzzm2)
                          call ogDeriv3(ep, 0,2,0,0, xy(i1+2*is1,i2+2*
     & is2,i3+2*is3,0),xy(i1+2*is1,i2+2*is2,i3+2*is3,1),xy(i1+2*is1,
     & i2+2*is2,i3+2*is3,2),t, ex,uxxp2, ey,vxxp2, ez,wxxp2)
                          call ogDeriv3(ep, 0,0,2,0, xy(i1+2*is1,i2+2*
     & is2,i3+2*is3,0),xy(i1+2*is1,i2+2*is2,i3+2*is3,1),xy(i1+2*is1,
     & i2+2*is2,i3+2*is3,2),t, ex,uyyp2, ey,vyyp2, ez,wyyp2)
                          call ogDeriv3(ep, 0,0,0,2, xy(i1+2*is1,i2+2*
     & is2,i3+2*is3,0),xy(i1+2*is1,i2+2*is2,i3+2*is3,1),xy(i1+2*is1,
     & i2+2*is2,i3+2*is3,2),t, ex,uzzp2, ey,vzzp2, ez,wzzp2)
                        uLapr=(8.*((uxxp1+uyyp1+uzzp1)-(uxxm1+uyym1+
     & uzzm1))-((uxxp2+uyyp2+uzzp2)-(uxxm2+uyym2+uzzm2)) )/(12.*dra)
                        vLapr=(8.*((vxxp1+vyyp1+vzzp1)-(vxxm1+vyym1+
     & vzzm1))-((vxxp2+vyyp2+vzzp2)-(vxxm2+vyym2+vzzm2)) )/(12.*dra)
                        wLapr=(8.*((wxxp1+wyyp1+wzzp1)-(wxxm1+wyym1+
     & wzzm1))-((wxxp2+wyyp2+wzzp2)-(wxxm2+wyym2+wzzm2)) )/(12.*dra)
                          call ogDeriv3(ep, 0,2,0,0, xy(i1-js1,i2-js2,
     & i3-js3,0),xy(i1-js1,i2-js2,i3-js3,1),xy(i1-js1,i2-js2,i3-js3,2)
     & ,t, ex,uxxm1, ey,vxxm1, ez,wxxm1)
                          call ogDeriv3(ep, 0,0,2,0, xy(i1-js1,i2-js2,
     & i3-js3,0),xy(i1-js1,i2-js2,i3-js3,1),xy(i1-js1,i2-js2,i3-js3,2)
     & ,t, ex,uyym1, ey,vyym1, ez,wyym1)
                          call ogDeriv3(ep, 0,0,0,2, xy(i1-js1,i2-js2,
     & i3-js3,0),xy(i1-js1,i2-js2,i3-js3,1),xy(i1-js1,i2-js2,i3-js3,2)
     & ,t, ex,uzzm1, ey,vzzm1, ez,wzzm1)
                          call ogDeriv3(ep, 0,2,0,0, xy(i1+js1,i2+js2,
     & i3+js3,0),xy(i1+js1,i2+js2,i3+js3,1),xy(i1+js1,i2+js2,i3+js3,2)
     & ,t, ex,uxxp1, ey,vxxp1, ez,wxxp1)
                          call ogDeriv3(ep, 0,0,2,0, xy(i1+js1,i2+js2,
     & i3+js3,0),xy(i1+js1,i2+js2,i3+js3,1),xy(i1+js1,i2+js2,i3+js3,2)
     & ,t, ex,uyyp1, ey,vyyp1, ez,wyyp1)
                          call ogDeriv3(ep, 0,0,0,2, xy(i1+js1,i2+js2,
     & i3+js3,0),xy(i1+js1,i2+js2,i3+js3,1),xy(i1+js1,i2+js2,i3+js3,2)
     & ,t, ex,uzzp1, ey,vzzp1, ez,wzzp1)
                          call ogDeriv3(ep, 0,2,0,0, xy(i1-2*js1,i2-2*
     & js2,i3-2*js3,0),xy(i1-2*js1,i2-2*js2,i3-2*js3,1),xy(i1-2*js1,
     & i2-2*js2,i3-2*js3,2),t, ex,uxxm2, ey,vxxm2, ez,wxxm2)
                          call ogDeriv3(ep, 0,0,2,0, xy(i1-2*js1,i2-2*
     & js2,i3-2*js3,0),xy(i1-2*js1,i2-2*js2,i3-2*js3,1),xy(i1-2*js1,
     & i2-2*js2,i3-2*js3,2),t, ex,uyym2, ey,vyym2, ez,wyym2)
                          call ogDeriv3(ep, 0,0,0,2, xy(i1-2*js1,i2-2*
     & js2,i3-2*js3,0),xy(i1-2*js1,i2-2*js2,i3-2*js3,1),xy(i1-2*js1,
     & i2-2*js2,i3-2*js3,2),t, ex,uzzm2, ey,vzzm2, ez,wzzm2)
                          call ogDeriv3(ep, 0,2,0,0, xy(i1+2*js1,i2+2*
     & js2,i3+2*js3,0),xy(i1+2*js1,i2+2*js2,i3+2*js3,1),xy(i1+2*js1,
     & i2+2*js2,i3+2*js3,2),t, ex,uxxp2, ey,vxxp2, ez,wxxp2)
                          call ogDeriv3(ep, 0,0,2,0, xy(i1+2*js1,i2+2*
     & js2,i3+2*js3,0),xy(i1+2*js1,i2+2*js2,i3+2*js3,1),xy(i1+2*js1,
     & i2+2*js2,i3+2*js3,2),t, ex,uyyp2, ey,vyyp2, ez,wyyp2)
                          call ogDeriv3(ep, 0,0,0,2, xy(i1+2*js1,i2+2*
     & js2,i3+2*js3,0),xy(i1+2*js1,i2+2*js2,i3+2*js3,1),xy(i1+2*js1,
     & i2+2*js2,i3+2*js3,2),t, ex,uzzp2, ey,vzzp2, ez,wzzp2)
                        uLaps=(8.*((uxxp1+uyyp1+uzzp1)-(uxxm1+uyym1+
     & uzzm1))-((uxxp2+uyyp2+uzzp2)-(uxxm2+uyym2+uzzm2)) )/(12.*dsa)
                        vLaps=(8.*((vxxp1+vyyp1+vzzp1)-(vxxm1+vyym1+
     & vzzm1))-((vxxp2+vyyp2+vzzp2)-(vxxm2+vyym2+vzzm2)) )/(12.*dsa)
                        wLaps=(8.*((wxxp1+wyyp1+wzzp1)-(wxxm1+wyym1+
     & wzzm1))-((wxxp2+wyyp2+wzzp2)-(wxxm2+wyym2+wzzm2)) )/(12.*dsa)
                      a1Dotu0=a11*u(i1,i2,i3,ex)+a12*u(i1,i2,i3,ey)+
     & a13*u(i1,i2,i3,ez)
                      a2Dotu0=a21*u(i1,i2,i3,ex)+a22*u(i1,i2,i3,ey)+
     & a23*u(i1,i2,i3,ez)
                      a1Doturr=a11*urr+a12*vrr+a13*wrr
                      a1Dotuss=a11*uss+a12*vss+a13*wss
                      a2Doturr=a21*urr+a22*vrr+a23*wrr
                      a2Dotuss=a21*uss+a22*vss+a23*wss
                      a3Dotur=a31*ur+a32*vr+a33*wr
                      a3Dotus=a31*us+a32*vs+a33*ws
                        ! we get a3.urss and a3.urrs from the equation
                        ! c22*uss = -( c11*urr + c33*utt + c1*ur + c2*us + c3*ut )
                        ! c11*urr = -( c22*uss + c33*utt + c1*ur + c2*us + c3*ut )
                        ! c22*urss = -( c22r*uss + c11*urrr + c11r*urr + c33*urtt + c33r*utt + ... )
                        urss = -(  c22r*uss + c11*urrr + c11r*urr + 
     & c33*urtt + c33r*utt + c1*urr + c1r*ur + c2*urs + c2r*us + c3*
     & urt + c3r*ut - uLapr )/c22
                        urrs = -(  c11s*urr + c22*usss + c22s*uss + 
     & c33*ustt + c33s*utt + c1*urs + c1s*ur + c2*uss + c2s*us + c3*
     & ust + c3s*ut - uLaps )/c11
                        vrss = -(  c22r*vss + c11*vrrr + c11r*vrr + 
     & c33*vrtt + c33r*vtt + c1*vrr + c1r*vr + c2*vrs + c2r*vs + c3*
     & vrt + c3r*vt - vLapr )/c22
                        vrrs = -(  c11s*vrr + c22*vsss + c22s*vss + 
     & c33*vstt + c33s*vtt + c1*vrs + c1s*vr + c2*vss + c2s*vs + c3*
     & vst + c3s*vt - vLaps )/c11
                        wrss = -(  c22r*wss + c11*wrrr + c11r*wrr + 
     & c33*wrtt + c33r*wtt + c1*wrr + c1r*wr + c2*wrs + c2r*ws + c3*
     & wrt + c3r*wt - wLapr )/c22
                        wrrs = -(  c11s*wrr + c22*wsss + c22s*wss + 
     & c33*wstt + c33s*wtt + c1*wrs + c1s*wr + c2*wss + c2s*ws + c3*
     & wst + c3s*wt - wLaps )/c11
                        a3Doturrr=a31*urrr+a32*vrrr+a33*wrrr
                        a3Dotusss=a31*usss+a32*vsss+a33*wsss
                        a3Doturss=a31*urss+a32*vrss+a33*wrss
                        a3Doturrs=a31*urrs+a32*vrrs+a33*wrrs
                      detnt=a33*a11*a22-a33*a12*a21-a13*a31*a22+a31*
     & a23*a12+a13*a32*a21-a32*a23*a11
                      ! loop over different ghost points here -- could make a single loop, 1...4 and use arrays of ms1(m) 
                      do m1=1,numberOfGhostPoints
                      do m2=1,numberOfGhostPoints
                       if( edgeDirection.eq.0 )then
                         ms1=0
                         ms2=(1-2*side2)*m1
                         ms3=(1-2*side3)*m2
                         drb=dr(1)*ms2
                         dsb=dr(2)*ms3
                       else if( edgeDirection.eq.1 )then
                         ms2=0
                         ms3=(1-2*side3)*m1
                         ms1=(1-2*side1)*m2
                         drb=dr(2)*ms3
                         dsb=dr(0)*ms1
                       else
                         ms3=0
                         ms1=(1-2*side1)*m1
                         ms2=(1-2*side2)*m2
                         drb=dr(0)*ms1
                         dsb=dr(1)*ms2
                       end if
                      ! **** this is really for order=4 -- no need to be so accurate for order 2 ******
                      ! Here are a1.u(i1-ms1,i2-ms2,i3-ms3,.) a2.u(...), a3.u(...)
                      ! a1Dotu and a2Dotu -- odd Taylor series
                      ! a3Dotu : even Taylor series
                      a1Dotu = 2.*a1Dotu0 -(a11*u(i1+ms1,i2+ms2,i3+ms3,
     & ex)+a12*u(i1+ms1,i2+ms2,i3+ms3,ey)+a13*u(i1+ms1,i2+ms2,i3+ms3,
     & ez)) + drb**2*(a1Doturr) + 2.*drb*dsb*a1Doturs + dsb**2*(
     & a1Dotuss)
                      a2Dotu = 2.*a2Dotu0 -(a21*u(i1+ms1,i2+ms2,i3+ms3,
     & ex)+a22*u(i1+ms1,i2+ms2,i3+ms3,ey)+a23*u(i1+ms1,i2+ms2,i3+ms3,
     & ez)) + drb**2*(a2Doturr) + 2.*drb*dsb*a2Doturs + dsb**2*(
     & a2Dotuss)
                        a3Dotu = (a31*u(i1+ms1,i2+ms2,i3+ms3,ex)+a32*u(
     & i1+ms1,i2+ms2,i3+ms3,ey)+a33*u(i1+ms1,i2+ms2,i3+ms3,ez))-2.*( 
     & drb*(a3Dotur) + dsb*(a3Dotus) ) -(1./3.)*( drb**3*(a3Doturrr) +
     &  dsb**3*(a3Dotusss) + 3.*drb**2*dsb*(a3Doturrs) + 3.*drb*dsb**
     & 2*(a3Doturss) )
                      ! Now given a1.u(-1), a2.u(-1) a3.u(-1) we solve for u(-1)
                      u(i1-ms1,i2-ms2,i3-ms3,ex)=(a33*a1DotU*a22-a13*
     & a3DotU*a22+a13*a32*a2DotU+a3DotU*a23*a12-a32*a23*a1DotU-a33*
     & a12*a2DotU)/detnt
                      u(i1-ms1,i2-ms2,i3-ms3,ey)=(-a23*a11*a3DotU+a23*
     & a1DotU*a31+a11*a33*a2DotU+a13*a21*a3DotU-a1DotU*a33*a21-a13*
     & a2DotU*a31)/detnt
                      u(i1-ms1,i2-ms2,i3-ms3,ez)=(a11*a3DotU*a22-a11*
     & a32*a2DotU-a12*a21*a3DotU+a12*a2DotU*a31-a1DotU*a31*a22+a1DotU*
     & a32*a21)/detnt
                        ! *** extrap for now ****
                        ! j1=i1-ms1
                        ! j2=i2-ms2
                        ! j3=i3-ms3
                        ! u(j1,j2,j3,ex)=5.*u(j1+ls1,j2+ls2,j3+ls3,ex)-10.*u(j1+2*ls1,j2+2*ls2,j3+2*ls3,ex)+10.*u(j1+3*ls1,j2+3*ls2,j3+3*ls3,ex)!               -5.*u(j1+4*ls1,j2+4*ls2,j3+4*ls3,ex)+u(j1+5*ls1,j2+5*ls2,j3+5*ls3,ex)
                        ! u(j1,j2,j3,ey)=5.*u(j1+ls1,j2+ls2,j3+ls3,ey)-10.*u(j1+2*ls1,j2+2*ls2,j3+2*ls3,ey)+10.*u(j1+3*ls1,j2+3*ls2,j3+3*ls3,ey)!               -5.*u(j1+4*ls1,j2+4*ls2,j3+4*ls3,ey)+u(j1+5*ls1,j2+5*ls2,j3+5*ls3,ey)
                        ! u(j1,j2,j3,ez)=5.*u(j1+ls1,j2+ls2,j3+ls3,ez)-10.*u(j1+2*ls1,j2+2*ls2,j3+2*ls3,ez)+10.*u(j1+3*ls1,j2+3*ls2,j3+3*ls3,ez)!               -5.*u(j1+4*ls1,j2+4*ls2,j3+4*ls3,ez)+u(j1+5*ls1,j2+5*ls2,j3+5*ls3,ez)
                      if( .true. .or. debug.gt.0 )then
                          call ogf3dfo(ep,fieldOption,xy(i1-ms1,i2-ms2,
     & i3-ms3,0),xy(i1-ms1,i2-ms2,i3-ms3,1),xy(i1-ms1,i2-ms2,i3-ms3,2)
     & ,t,uvm(0),uvm(1),uvm(2))
                        if( debug.gt.0 )then
                          write(*,'(" corner-edge-4: ghost-pt=",3i4," 
     & ls=",3i3," error=",3e9.1)') i1-ms1,i2-ms2,i3-ms3,ls1,ls2,ls3,u(
     & i1-ms1,i2-ms2,i3-ms3,ex)-uvm(0),u(i1-ms1,i2-ms2,i3-ms3,ey)-uvm(
     & 1),u(i1-ms1,i2-ms2,i3-ms3,ez)-uvm(2)
                          ! '
                        end if
                        ! *** for now reset the solution to the exact ***
                        ! u(i1-ms1,i2-ms2,i3-ms3,ex)=uvm(0)
                        ! u(i1-ms1,i2-ms2,i3-ms3,ey)=uvm(1)
                        ! u(i1-ms1,i2-ms2,i3-ms3,ez)=uvm(2)
                      end if
                      if( debug.gt.2 )then
                        write(*,'(" a11,a12,a13=",3f6.2)') a11,a12,a13
                        write(*,'(" a21,a22,a23=",3f6.2)') a21,a22,a23
                        write(*,'(" a31,a32,a33=",3f6.2)') a31,a32,a33
                        write(*,'("  a3Dotu,true=",2e11.3," err=",
     & e10.2)') a3Dotu,(a31*uvm(0)+a32*uvm(1)+a33*uvm(2)),a3Dotu-(a31*
     & uvm(0)+a32*uvm(1)+a33*uvm(2))
                         call ogf3dfo(ep,fieldOption,xy(i1-is1-js1,i2-
     & is2-js2,i3-is3-js3,0),xy(i1-is1-js1,i2-is2-js2,i3-is3-js3,1),
     & xy(i1-is1-js1,i2-is2-js2,i3-is3-js3,2),t,uvmm(0),uvmm(1),uvmm(
     & 2))
                         call ogf3dfo(ep,fieldOption,xy(i1-js1,i2-js2,
     & i3-js3,0),xy(i1-js1,i2-js2,i3-js3,1),xy(i1-js1,i2-js2,i3-js3,2)
     & ,t,uvzm(0),uvzm(1),uvzm(2))
                         call ogf3dfo(ep,fieldOption,xy(i1+is1-js1,i2+
     & is2-js2,i3+is3-js3,0),xy(i1+is1-js1,i2+is2-js2,i3+is3-js3,1),
     & xy(i1+is1-js1,i2+is2-js2,i3+is3-js3,2),t,uvpm(0),uvpm(1),uvpm(
     & 2))
                         call ogf3dfo(ep,fieldOption,xy(i1-is1,i2-is2,
     & i3-is3,0),xy(i1-is1,i2-is2,i3-is3,1),xy(i1-is1,i2-is2,i3-is3,2)
     & ,t,uvmz(0),uvmz(1),uvmz(2))
                         call ogf3dfo(ep,fieldOption,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),xy(i1,i2,i3,2),t,uvzz(0),uvzz(1),uvzz(2))
                         call ogf3dfo(ep,fieldOption,xy(i1+is1,i2+is2,
     & i3+is3,0),xy(i1+is1,i2+is2,i3+is3,1),xy(i1+is1,i2+is2,i3+is3,2)
     & ,t,uvpz(0),uvpz(1),uvpz(2))
                         call ogf3dfo(ep,fieldOption,xy(i1-is1+js1,i2-
     & is2+js2,i3-is3+js3,0),xy(i1-is1+js1,i2-is2+js2,i3-is3+js3,1),
     & xy(i1-is1+js1,i2-is2+js2,i3-is3+js3,2),t,uvmp(0),uvmp(1),uvmp(
     & 2))
                         call ogf3dfo(ep,fieldOption,xy(i1+js1,i2+js2,
     & i3+js3,0),xy(i1+js1,i2+js2,i3+js3,1),xy(i1+js1,i2+js2,i3+js3,2)
     & ,t,uvzp(0),uvzp(1),uvzp(2))
                         call ogf3dfo(ep,fieldOption,xy(i1+is1+js1,i2+
     & is2+js2,i3+is3+js3,0),xy(i1+is1+js1,i2+is2+js2,i3+is3+js3,1),
     & xy(i1+is1+js1,i2+is2+js2,i3+is3+js3,2),t,uvpp(0),uvpp(1),uvpp(
     & 2))
                       ur0= ( uvpz(0)-uvmz(0) )/(2.*dra)
                       us0= ( uvzp(0)-uvzm(0) )/(2.*dsa)
                       urr0= ( uvpz(0)-2.*uvzz(0)+uvmz(0) )/(dra**2)
                       uss0= ( uvzp(0)-2.*uvzz(0)+uvzm(0) )/(dsa**2)
                       urs0= ( uvpp(0)-uvmp(0)-uvpm(0)+uvmm(0) )/(4.*
     & dra*dsa)
                       vrs0= ( uvpp(1)-uvmp(1)-uvpm(1)+uvmm(1) )/(4.*
     & dra*dsa)
                       wrs0= ( uvpp(2)-uvmp(2)-uvpm(2)+uvmm(2) )/(4.*
     & dra*dsa)
                       urrs0=( (uvpp(0)-2.*uvzp(0)+uvmp(0))-(uvpm(0)-
     & 2.*uvzm(0)+uvmm(0)) )/(2.*dsa*dra**2)
                       vrrs0=( (uvpp(1)-2.*uvzp(1)+uvmp(1))-(uvpm(1)-
     & 2.*uvzm(1)+uvmm(1)) )/(2.*dsa*dra**2)
                       wrrs0=( (uvpp(2)-2.*uvzp(2)+uvmp(2))-(uvpm(2)-
     & 2.*uvzm(2)+uvmm(2)) )/(2.*dsa*dra**2)
                       urss0=( (uvpp(0)-2.*uvpz(0)+uvpm(0))-(uvmp(0)-
     & 2.*uvmz(0)+uvmm(0)) )/(2.*dra*dsa**2)
                       vrss0=( (uvpp(1)-2.*uvpz(1)+uvpm(1))-(uvmp(1)-
     & 2.*uvmz(1)+uvmm(1)) )/(2.*dra*dsa**2)
                       wrss0=( (uvpp(2)-2.*uvpz(2)+uvpm(2))-(uvmp(2)-
     & 2.*uvmz(2)+uvmm(2)) )/(2.*dra*dsa**2)
                        write(*,'(" u(i-is),u(i),u(i+is): err=",3e10.2)
     & ') u(i1-is1,i2-is2,i3-is3,ex)-uvmz(0),u(i1,i2,i3,ex)-uvzz(0),u(
     & i1+is1,i2+is2,i3+is3,ex)-uvpz(0)
                        write(*,'(" u(i-js),u(i),u(i+js): err=",3e10.2)
     & ') u(i1-js1,i2-js2,i3-js3,ex)-uvzm(0),u(i1,i2,i3,ex)-uvzz(0),u(
     & i1+js1,i2+js2,i3+js3,ex)-uvzp(0)
                        write(*,'(" ur, true2=",2e11.3," err=",e10.2)')
     &  ur,ur0,ur-ur0
                        write(*,'(" us, true2=",2e11.3," err=",e10.2)')
     &  us,us0,us-us0
                        write(*,'(" urr, true2=",2e11.3," err=",e10.2)
     & ') urr,urr0,urr-urr0
                        write(*,'(" uss, true2=",2e11.3," err=",e10.2)
     & ') uss,uss0,uss-uss0
                        write(*,'(" urs, true2=",2e11.3," err=",e10.2)
     & ') urs,urs0,urs-urs0
                        write(*,'(" vrs, true2=",2e11.3," err=",e10.2)
     & ') vrs,vrs0,vrs-vrs0
                        if( edgeDirection.eq.0 ) then
                          write(*,'("  vrs:true=",e11.3)') ust4(i1,i2,
     & i3,ey)
                        else if( edgeDirection.eq.1 )then
                          write(*,'("  vrs:true=",e11.3)') urt4(i1,i2,
     & i3,ey)
                        else
                          write(*,'("  vrs:true=",e11.3)') urs4(i1,i2,
     & i3,ey)
                        end if
                        write(*,'(" wrs, true2=",2e11.3," err=",e10.2)
     & ') wrs,wrs0,wrs-wrs0
                         write(*,'(" urrr,true=",2e11.3," err=",e10.2)
     & ') urrr,(u(i1+2*is1,i2+2*is2,i3+2*is3,ex)-2.*u(i1+is1,i2+is2,
     & i3+is3,ex)+2.*u(i1-is1,i2-is2,i3-is3,ex)-u(i1-2*is1,i2-2*is2,
     & i3-2*is3,ex))/(2.*dra**3),urrr-(u(i1+2*is1,i2+2*is2,i3+2*is3,
     & ex)-2.*u(i1+is1,i2+is2,i3+is3,ex)+2.*u(i1-is1,i2-is2,i3-is3,ex)
     & -u(i1-2*is1,i2-2*is2,i3-2*is3,ex))/(2.*dra**3)
                         write(*,'(" usss,true=",2e11.3," err=",e10.2)
     & ') usss,(u(i1+2*js1,i2+2*js2,i3+2*js3,ex)-2.*u(i1+js1,i2+js2,
     & i3+js3,ex)+2.*u(i1-js1,i2-js2,i3-js3,ex)-u(i1-2*js1,i2-2*js2,
     & i3-2*js3,ex))/(2.*dsa**3),usss-(u(i1+2*js1,i2+2*js2,i3+2*js3,
     & ex)-2.*u(i1+js1,i2+js2,i3+js3,ex)+2.*u(i1-js1,i2-js2,i3-js3,ex)
     & -u(i1-2*js1,i2-2*js2,i3-2*js3,ex))/(2.*dsa**3)
                         write(*,'(" uttt,true=",2e11.3," err=",e10.2)
     & ') uttt,(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ex)-2.*u(i1+ks1,i2+ks2,
     & i3+ks3,ex)+2.*u(i1-ks1,i2-ks2,i3-ks3,ex)-u(i1-2*ks1,i2-2*ks2,
     & i3-2*ks3,ex))/(2.*dta**3),uttt-(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,
     & ex)-2.*u(i1+ks1,i2+ks2,i3+ks3,ex)+2.*u(i1-ks1,i2-ks2,i3-ks3,ex)
     & -u(i1-2*ks1,i2-2*ks2,i3-2*ks3,ex))/(2.*dta**3)
                         write(*,'(" wrrr,true=",2e11.3," err=",e10.2)
     & ') wrrr,(u(i1+2*is1,i2+2*is2,i3+2*is3,ez)-2.*u(i1+is1,i2+is2,
     & i3+is3,ez)+2.*u(i1-is1,i2-is2,i3-is3,ez)-u(i1-2*is1,i2-2*is2,
     & i3-2*is3,ez))/(2.*dra**3),wrrr-(u(i1+2*is1,i2+2*is2,i3+2*is3,
     & ez)-2.*u(i1+is1,i2+is2,i3+is3,ez)+2.*u(i1-is1,i2-is2,i3-is3,ez)
     & -u(i1-2*is1,i2-2*is2,i3-2*is3,ez))/(2.*dra**3)
                         write(*,'(" wsss,true=",2e11.3," err=",e10.2)
     & ') wsss,(u(i1+2*js1,i2+2*js2,i3+2*js3,ez)-2.*u(i1+js1,i2+js2,
     & i3+js3,ez)+2.*u(i1-js1,i2-js2,i3-js3,ez)-u(i1-2*js1,i2-2*js2,
     & i3-2*js3,ez))/(2.*dsa**3),wsss-(u(i1+2*js1,i2+2*js2,i3+2*js3,
     & ez)-2.*u(i1+js1,i2+js2,i3+js3,ez)+2.*u(i1-js1,i2-js2,i3-js3,ez)
     & -u(i1-2*js1,i2-2*js2,i3-2*js3,ez))/(2.*dsa**3)
                         write(*,'(" wttt,true=",2e11.3," err=",e10.2)
     & ') wttt,(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ez)-2.*u(i1+ks1,i2+ks2,
     & i3+ks3,ez)+2.*u(i1-ks1,i2-ks2,i3-ks3,ez)-u(i1-2*ks1,i2-2*ks2,
     & i3-2*ks3,ez))/(2.*dta**3),wttt-(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,
     & ez)-2.*u(i1+ks1,i2+ks2,i3+ks3,ez)+2.*u(i1-ks1,i2-ks2,i3-ks3,ez)
     & -u(i1-2*ks1,i2-2*ks2,i3-2*ks3,ez))/(2.*dta**3)
                         write(*,'(" urrs,true2=",2e11.3," err=",e10.2)
     & ') urrs,urrs0,urrs-urrs0
                         write(*,'(" vrrs,true2=",2e11.3," err=",e10.2)
     & ') vrrs,vrrs0,vrrs-vrrs0
                         write(*,'(" wrrs,true2=",2e11.3," err=",e10.2)
     & ') wrrs,wrrs0,wrrs-wrrs0
                         write(*,'(" urss,true2=",2e11.3," err=",e10.2)
     & ') urss,urss0,urss-urss0
                         write(*,'(" vrss,true2=",2e11.3," err=",e10.2)
     & ') vrss,vrss0,vrss-vrss0
                         write(*,'(" wrss,true2=",2e11.3," err=",e10.2)
     & ') wrss,wrss0,wrss-wrss0
                      end if
                      end do
                      end do ! m1
                     else
                       ! ---------------- fill in ghost by extrapolation  --------------
                       !  *wdh* 2016/06/24 
                       ! loop over different ghost points here -- could make a single loop, 1...4 and use arrays of ms1(m) 
                      do m1=1,numberOfGhostPoints
                      do m2=1,numberOfGhostPoints
                       if( edgeDirection.eq.0 )then
                         ns1=0
                         ns2=(1-2*side2)
                         ns3=(1-2*side3)
                         ms1=0
                         ms2=(1-2*side2)*m1
                         ms3=(1-2*side3)*m2
                       else if( edgeDirection.eq.1 )then
                         ns2=0
                         ns3=(1-2*side3)
                         ns1=(1-2*side1)
                         ms2=0
                         ms3=(1-2*side3)*m1
                         ms1=(1-2*side1)*m2
                       else
                         ns3=0
                         ns1=(1-2*side1)
                         ns2=(1-2*side2)
                         ms3=0
                         ms1=(1-2*side1)*m1
                         ms2=(1-2*side2)*m2
                       end if
                        u(i1-ms1,i2-ms2,i3-ms3,ex)=(5.*u(i1-ms1+ns1,i2-
     & ms2+ns2,i3-ms3+ns3,ex)-10.*u(i1-ms1+2*ns1,i2-ms2+2*ns2,i3-ms3+
     & 2*ns3,ex)+10.*u(i1-ms1+3*ns1,i2-ms2+3*ns2,i3-ms3+3*ns3,ex)-5.*
     & u(i1-ms1+4*ns1,i2-ms2+4*ns2,i3-ms3+4*ns3,ex)+u(i1-ms1+5*ns1,i2-
     & ms2+5*ns2,i3-ms3+5*ns3,ex))
                        u(i1-ms1,i2-ms2,i3-ms3,ey)=(5.*u(i1-ms1+ns1,i2-
     & ms2+ns2,i3-ms3+ns3,ey)-10.*u(i1-ms1+2*ns1,i2-ms2+2*ns2,i3-ms3+
     & 2*ns3,ey)+10.*u(i1-ms1+3*ns1,i2-ms2+3*ns2,i3-ms3+3*ns3,ey)-5.*
     & u(i1-ms1+4*ns1,i2-ms2+4*ns2,i3-ms3+4*ns3,ey)+u(i1-ms1+5*ns1,i2-
     & ms2+5*ns2,i3-ms3+5*ns3,ey))
                        u(i1-ms1,i2-ms2,i3-ms3,ez)=(5.*u(i1-ms1+ns1,i2-
     & ms2+ns2,i3-ms3+ns3,ez)-10.*u(i1-ms1+2*ns1,i2-ms2+2*ns2,i3-ms3+
     & 2*ns3,ez)+10.*u(i1-ms1+3*ns1,i2-ms2+3*ns2,i3-ms3+3*ns3,ez)-5.*
     & u(i1-ms1+4*ns1,i2-ms2+4*ns2,i3-ms3+4*ns3,ez)+u(i1-ms1+5*ns1,i2-
     & ms2+5*ns2,i3-ms3+5*ns3,ez))
                      end do ! m2
                      end do ! m1
                     end if  ! end if mask
                     end do ! end do i1
                     end do ! end do i2
                     end do ! end do i3
                   else if( bc1.eq.perfectElectricalConductor .or. 
     & bc2.eq.perfectElectricalConductor )then
                    ! ***************************************************************************
                    ! ************* PEC FACE ON ONE ADJACENT FACE (CURVILINEAR) *****************
                    ! ***************************************************************************
                     ! *new* *wdh*  2015/07/12 
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                     if( mask(i1,i2,i3).ne.0 )then
                      ! ---------------- fill in ghost by extrapolation  --------------
                      do m1=1,numberOfGhostPoints
                      do m2=1,numberOfGhostPoints
                       if( edgeDirection.eq.0 )then
                         ns1=0
                         ns2=(1-2*side2)
                         ns3=(1-2*side3)
                         ms1=0
                         ms2=(1-2*side2)*m1
                         ms3=(1-2*side3)*m2
                       else if( edgeDirection.eq.1 )then
                         ns2=0
                         ns3=(1-2*side3)
                         ns1=(1-2*side1)
                         ms2=0
                         ms3=(1-2*side3)*m1
                         ms1=(1-2*side1)*m2
                       else
                         ns3=0
                         ns1=(1-2*side1)
                         ns2=(1-2*side2)
                         ms3=0
                         ms1=(1-2*side1)*m1
                         ms2=(1-2*side2)*m2
                       end if
                        u(i1-ms1,i2-ms2,i3-ms3,ex)=(5.*u(i1-ms1+ns1,i2-
     & ms2+ns2,i3-ms3+ns3,ex)-10.*u(i1-ms1+2*ns1,i2-ms2+2*ns2,i3-ms3+
     & 2*ns3,ex)+10.*u(i1-ms1+3*ns1,i2-ms2+3*ns2,i3-ms3+3*ns3,ex)-5.*
     & u(i1-ms1+4*ns1,i2-ms2+4*ns2,i3-ms3+4*ns3,ex)+u(i1-ms1+5*ns1,i2-
     & ms2+5*ns2,i3-ms3+5*ns3,ex))
                        u(i1-ms1,i2-ms2,i3-ms3,ey)=(5.*u(i1-ms1+ns1,i2-
     & ms2+ns2,i3-ms3+ns3,ey)-10.*u(i1-ms1+2*ns1,i2-ms2+2*ns2,i3-ms3+
     & 2*ns3,ey)+10.*u(i1-ms1+3*ns1,i2-ms2+3*ns2,i3-ms3+3*ns3,ey)-5.*
     & u(i1-ms1+4*ns1,i2-ms2+4*ns2,i3-ms3+4*ns3,ey)+u(i1-ms1+5*ns1,i2-
     & ms2+5*ns2,i3-ms3+5*ns3,ey))
                        u(i1-ms1,i2-ms2,i3-ms3,ez)=(5.*u(i1-ms1+ns1,i2-
     & ms2+ns2,i3-ms3+ns3,ez)-10.*u(i1-ms1+2*ns1,i2-ms2+2*ns2,i3-ms3+
     & 2*ns3,ez)+10.*u(i1-ms1+3*ns1,i2-ms2+3*ns2,i3-ms3+3*ns3,ez)-5.*
     & u(i1-ms1+4*ns1,i2-ms2+4*ns2,i3-ms3+4*ns3,ez)+u(i1-ms1+5*ns1,i2-
     & ms2+5*ns2,i3-ms3+5*ns3,ez))
                      end do ! m2
                      end do ! m1
                     end if  ! end if mask
                     end do ! end do i1
                     end do ! end do i2
                     end do ! end do i3
                   else if( bc1.eq.dirichlet .or. bc2.eq.dirichlet )
     & then
                     ! *wdh* 081124 -- do nothing here ---
                     ! This is a dirichlet BC 
                 ! *    do m1=1,numberOfGhostPoints
                 ! *    do m2=1,numberOfGhostPoints
                 ! * 
                 ! *     ! shift to ghost point "(m1,m2)"
                 ! *     if( edgeDirection.eq.2 )then 
                 ! *       js1=is1*m1  
                 ! *       js2=is2*m2
                 ! *       js3=0
                 ! *     else if( edgeDirection.eq.1 )then 
                 ! *       js1=is1*m1  
                 ! *       js2=0
                 ! *       js3=is3*m2
                 ! *     else 
                 ! *       js1=0
                 ! *       js2=is2*m1
                 ! *       js3=is3*m2
                 ! *     end if 
                 ! * 
                 ! *     do i3=n3a,n3b
                 ! *     do i2=n2a,n2b
                 ! *     do i1=n1a,n1b
                 ! *   
                 ! *       #If "twilightZone" == "twilightZone"
                 ! *         OGF3DFO(i1-js1,i2-js2,i3-js3,t, g1,g2,g3)
                 ! *       #End
                 ! *       u(i1-js1,i2-js2,i3-js3,ex)=g1
                 ! *       u(i1-js1,i2-js2,i3-js3,ey)=g2
                 ! *       u(i1-js1,i2-js2,i3-js3,ez)=g3
                 ! * 
                 ! *     end do ! end do i1
                 ! *     end do ! end do i2
                 ! *     end do ! end do i3
                 ! * 
                 ! *    end do
                 ! *    end do ! m1
                   else if( bc1.le.0 .or. bc2.le.0 )then
                     ! periodic or interpolation -- nothing to do
                   else if( bc1.eq.planeWaveBoundaryCondition 
     & .or.bc2.eq.planeWaveBoundaryCondition .or. 
     & bc1.eq.symmetryBoundaryCondition .or. 
     & bc2.eq.symmetryBoundaryCondition .or. (bc1.ge.abcEM2 .and. 
     & bc1.le.lastBC) .or. (bc2.ge.abcEM2 .and. bc2.le.lastBC))then
                      ! do nothing
                   else
                     write(*,'("ERROR: unknown boundary conditions bc1,
     & bc2=",2i3)') bc1,bc2
                     ! unknown boundary conditions
                     stop 8866
                   end if
                  end do
                  end do
                  end do  ! edge direction
                ! Finally assign points outside the vertices of the unit cube
                g1=0.
                g2=0.
                g3=0.
                do side3=0,1
                do side2=0,1
                do side1=0,1
                 ! assign ghost values outside the corner (vertex)
                 i1=gridIndexRange(side1,0)
                 i2=gridIndexRange(side2,1)
                 i3=gridIndexRange(side3,2)
                 is1=1-2*side1
                 is2=1-2*side2
                 is3=1-2*side3
                 if( boundaryCondition(side1,0)
     & .eq.perfectElectricalConductor .and.boundaryCondition(side2,1)
     & .eq.perfectElectricalConductor .and.boundaryCondition(side3,2)
     & .eq.perfectElectricalConductor )then
                   ! ------------------------------------------------------------
                   ! -------------- VERTEX adjacent to 3 PEC faces --------------
                   ! ------------------------------------------------------------
                    urr = urr2(i1,i2,i3,ex)
                    uss = uss2(i1,i2,i3,ex)
                    utt = utt2(i1,i2,i3,ex)
                    urs = urs2(i1,i2,i3,ex)
                    urt = urt2(i1,i2,i3,ex)
                    ust = ust2(i1,i2,i3,ex)
                    vrr = urr2(i1,i2,i3,ey)
                    vss = uss2(i1,i2,i3,ey)
                    vtt = utt2(i1,i2,i3,ey)
                    vrs = urs2(i1,i2,i3,ey)
                    vrt = urt2(i1,i2,i3,ey)
                    vst = ust2(i1,i2,i3,ey)
                    wrr = urr2(i1,i2,i3,ez)
                    wss = uss2(i1,i2,i3,ez)
                    wtt = utt2(i1,i2,i3,ez)
                    wrs = urs2(i1,i2,i3,ez)
                    wrt = urt2(i1,i2,i3,ez)
                    wst = ust2(i1,i2,i3,ez)
                  do m3=1,numberOfGhostPoints
                  do m2=1,numberOfGhostPoints
                  do m1=1,numberOfGhostPoints
                    js1=is1*m1  ! shift to ghost point "m"
                    js2=is2*m2
                    js3=is3*m3
                    dra=dr(0)*js1
                    dsa=dr(1)*js2
                    dta=dr(2)*js3
                       ! *new* 2015/07/12 
                       u(i1-js1,i2-js2,i3-js3,ex)=(5.*u(i1,i2,i3,ex)-
     & 10.*u(i1+js1,i2+js2,i3+js3,ex)+10.*u(i1+2*js1,i2+2*js2,i3+2*
     & js3,ex)-5.*u(i1+3*js1,i2+3*js2,i3+3*js3,ex)+u(i1+4*js1,i2+4*
     & js2,i3+4*js3,ex))
                       u(i1-js1,i2-js2,i3-js3,ey)=(5.*u(i1,i2,i3,ey)-
     & 10.*u(i1+js1,i2+js2,i3+js3,ey)+10.*u(i1+2*js1,i2+2*js2,i3+2*
     & js3,ey)-5.*u(i1+3*js1,i2+3*js2,i3+3*js3,ey)+u(i1+4*js1,i2+4*
     & js2,i3+4*js3,ey))
                       u(i1-js1,i2-js2,i3-js3,ez)=(5.*u(i1,i2,i3,ez)-
     & 10.*u(i1+js1,i2+js2,i3+js3,ez)+10.*u(i1+2*js1,i2+2*js2,i3+2*
     & js3,ez)-5.*u(i1+3*js1,i2+3*js2,i3+3*js3,ez)+u(i1+4*js1,i2+4*
     & js2,i3+4*js3,ez))
                     if( debug.gt.2 )then
                       write(*,'("Corner point from taylor: ghost-pt=",
     & 3i4," errors=",3e10.2)') i1-js1,i2-js2,i3-js3,u(i1-js1,i2-js2,
     & i3-js3,ex)-um,u(i1-js1,i2-js2,i3-js3,ey)-vm,u(i1-js1,i2-js2,i3-
     & js3,ez)-wm
                       ! write(*,'(" corner: dra,dsa,dta=",3f6.3," urr,uss,utt,urs,urt,ust=",6f8.3)') dra,dsa,dta,!    urr,uss,utt,urs,urt,ust
                       ! "
                     end if
                       ! Set the solution to exact for now
                       ! OGF3DFO(i1-js1,i2-js2,i3-js3,t, um,vm,wm)
                       ! u(i1-js1,i2-js2,i3-js3,ex)=um
                       ! u(i1-js1,i2-js2,i3-js3,ey)=vm
                       ! u(i1-js1,i2-js2,i3-js3,ez)=wm
                  end do
                  end do
                  end do
                 else if( .true. .and. (boundaryCondition(side1,0)
     & .eq.perfectElectricalConductor .or.boundaryCondition(side2,1)
     & .eq.perfectElectricalConductor .or.boundaryCondition(side3,2)
     & .eq.perfectElectricalConductor) )then
                   ! *new* *wdh* 2015/07/12 
                   ! -----------------------------------------------------------------
                   ! -------------- VERTEX adjacent to 1 or 2 PEC faces --------------
                   ! -----------------------------------------------------------------
                  do m3=1,numberOfGhostPoints
                  do m2=1,numberOfGhostPoints
                  do m1=1,numberOfGhostPoints
                    js1=is1*m1  ! shift to ghost point "m"
                    js2=is2*m2
                    js3=is3*m3
                     u(i1-js1,i2-js2,i3-js3,ex)=(5.*u(i1,i2,i3,ex)-10.*
     & u(i1+js1,i2+js2,i3+js3,ex)+10.*u(i1+2*js1,i2+2*js2,i3+2*js3,ex)
     & -5.*u(i1+3*js1,i2+3*js2,i3+3*js3,ex)+u(i1+4*js1,i2+4*js2,i3+4*
     & js3,ex))
                     u(i1-js1,i2-js2,i3-js3,ey)=(5.*u(i1,i2,i3,ey)-10.*
     & u(i1+js1,i2+js2,i3+js3,ey)+10.*u(i1+2*js1,i2+2*js2,i3+2*js3,ey)
     & -5.*u(i1+3*js1,i2+3*js2,i3+3*js3,ey)+u(i1+4*js1,i2+4*js2,i3+4*
     & js3,ey))
                     u(i1-js1,i2-js2,i3-js3,ez)=(5.*u(i1,i2,i3,ez)-10.*
     & u(i1+js1,i2+js2,i3+js3,ez)+10.*u(i1+2*js1,i2+2*js2,i3+2*js3,ez)
     & -5.*u(i1+3*js1,i2+3*js2,i3+3*js3,ez)+u(i1+4*js1,i2+4*js2,i3+4*
     & js3,ez))
                  end do ! end do m1
                  end do ! end do m2
                  end do ! end do m3
                 else if( boundaryCondition(side1,0).eq.dirichlet 
     & .or.boundaryCondition(side2,1).eq.dirichlet 
     & .or.boundaryCondition(side3,2).eq.dirichlet )then
                  ! *wdh* 081124 -- do nothing here ---
              ! *    do m3=1,numberOfGhostPoints
              ! *    do m2=1,numberOfGhostPoints
              ! *    do m1=1,numberOfGhostPoints
              ! *
              ! *      js1=is1*m1  ! shift to ghost point "m"
              ! *      js2=is2*m2
              ! *      js3=is3*m3
              ! *
              ! *      #If "twilightZone" == "twilightZone" 
              ! *        OGF3DFO(i1-js1,i2-js2,i3-js3,t, g1,g2,g3)
              ! *      #End
              ! *      u(i1-js1,i2-js2,i3-js3,ex)=g1
              ! *      u(i1-js1,i2-js2,i3-js3,ey)=g2
              ! *      u(i1-js1,i2-js2,i3-js3,ez)=g3
              ! *
              ! *    end do
              ! *    end do
              ! *    end do
                 else if( boundaryCondition(side1,0).le.0 
     & .or.boundaryCondition(side2,1).le.0 .or.boundaryCondition(
     & side3,2).le.0 )then
                    ! one or more boundaries are periodic or interpolation -- nothing to do
                 else if( boundaryCondition(side1,0)
     & .eq.planeWaveBoundaryCondition .or.boundaryCondition(side2,1)
     & .eq.planeWaveBoundaryCondition .or.boundaryCondition(side3,2)
     & .eq.planeWaveBoundaryCondition  .or. boundaryCondition(side1,0)
     & .eq.symmetryBoundaryCondition .or. boundaryCondition(side2,1)
     & .eq.symmetryBoundaryCondition .or. boundaryCondition(side3,2)
     & .eq.symmetryBoundaryCondition .or. (boundaryCondition(side1,0)
     & .ge.abcEM2 .and. boundaryCondition(side1,0).le.lastBC) .or. (
     & boundaryCondition(side2,1).ge.abcEM2 .and. boundaryCondition(
     & side2,1).le.lastBC) .or. (boundaryCondition(side3,2).ge.abcEM2 
     & .and. boundaryCondition(side3,2).le.lastBC)  )then
                   ! do nothing
                 else
                   write(*,'("ERROR: unknown boundary conditions at a 
     & 3D corner bc1,bc2,bc3=",2i3)') boundaryCondition(side1,0),
     & boundaryCondition(side2,1),boundaryCondition(side3,2)
                   ! '
                   ! unknown boundary conditions
                   stop 3399
                 end if
                end do
                end do
                end do
            end if
          end if
        end if
        return
        end
