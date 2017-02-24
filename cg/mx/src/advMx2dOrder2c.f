! This file automatically generated from advOpt.bf with bpp.
        subroutine advMx2dOrder2c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rsxy,  um,u,un,f,fa, v,vvt2,
     & ut3,vvt4,ut5,ut6,ut7, bc, dis, varDis, ipar, rpar, ierr )
       !======================================================================
       !   Advance a time step for Maxwells equations
       !     OPTIMIZED version for rectangular grids.
       ! nd : number of space dimensions
       !
       ! ipar(0)  = option : option=0 - Maxwell+Artificial diffusion
       !                           =1 - AD only
       !
       !  dis(i1,i2,i3) : temp space to hold artificial dissipation
       !  varDis(i1,i2,i3) : coefficient of the variable artificial dissipation
       !======================================================================
        implicit none
        integer nd, n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,
     & nd3b,nd4a,nd4b
        real um(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
        real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
        real un(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
        real f(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
        real fa(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b,0:*)  ! forcings at different times
        real v(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
        real vvt2(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
        real ut3(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
        real vvt4(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
        real ut5(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
        real ut6(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
        real ut7(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
        real dis(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
        real varDis(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
        real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
        integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
        integer bc(0:1,0:2),ierr
        integer ipar(0:*)
        real rpar(0:*)
       !     ---- local variables -----
        integer c,i1,i2,i3,n,gridType,orderOfAccuracy,orderInTime
        integer addForcing,orderOfDissipation,option
        integer useWhereMask,useWhereMaskSave,solveForE,solveForH,grid,
     & useVariableDissipation
        integer useCurvilinearOpt,useConservative,
     & combineDissipationWithAdvance,useDivergenceCleaning
        integer useNewForcingMethod,numberOfForcingFunctions,fcur,
     & fnext,fprev
        integer ex,ey,ez, hx,hy,hz
        real t,cc,dt,dy,dz,cdt,cdtdx,cdtdy,cdtdz,adc,adcdt,add,adddt
        real dt4by12
        real eps,mu,sigmaE,sigmaH,kx,ky,kz,
     & divergenceCleaningCoefficient
        logical addDissipation
        real dx(0:2),dr(0:2)
        real dx2i,dy2i,dz2i,dxsqi,dysqi,dzsqi,dxi,dyi,dzi
        real dx12i,dy12i,dz12i,dxsq12i,dysq12i,dzsq12i,dxy4i,dxz4i,
     & dyz4,time0,time1
        real dxi4,dyi4,dzi4,dxdyi2,dxdzi2,dydzi2
        real uLap(-1:1,-1:1,0:5),uLapSq(0:5)
        real uLaprr2,uLapss2,uLaprs2,uLapr2,uLaps2
        real c0,c1,csq,dtsq,cdtsq,cdtsq12,lap(0:20),cdtSqBy12
        real c40,c41,c42,c43
        real c60,c61,c62,c63,c64,c65
        real c80,c81,c82,c83,c84,c85,c86,c87
        real c00lap2d6,c10lap2d6,c01lap2d6,c20lap2d6,c02lap2d6,
     & c30lap2d6,c03lap2d6
        real c00lap2d8,c10lap2d8,c01lap2d8,c20lap2d8,c02lap2d8,
     & c30lap2d8,c03lap2d8,c40lap2d8,c04lap2d8
        real c000lap3d6,c100lap3d6,c010lap3d6,c001lap3d6,c200lap3d6,
     & c020lap3d6,c002lap3d6,c300lap3d6,c030lap3d6,c003lap3d6
        real c000lap3d8,c100lap3d8,c010lap3d8,c001lap3d8,c200lap3d8,
     & c020lap3d8,c002lap3d8,c300lap3d8,c030lap3d8,c003lap3d8,
     & c400lap3d8,c040lap3d8,c004lap3d8
        integer rectangular,curvilinear
        parameter( rectangular=0, curvilinear=1 )
        integer timeSteppingMethod
        integer defaultTimeStepping,adamsSymmetricOrder3,
     & rungeKuttaFourthOrder,stoermerTimeStepping,
     & modifiedEquationTimeStepping
        parameter(defaultTimeStepping=0,adamsSymmetricOrder3=1,
     & rungeKuttaFourthOrder=2,stoermerTimeStepping=3,
     & modifiedEquationTimeStepping=4)
        ! Dispersion models
        integer noDispersion,drude
        parameter( noDispersion=0, drude=1 )
       !...........start statement function
        integer kd,m
        real rx,ry,rz,sx,sy,sz,tx,ty,tz
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
         real unr2
         real uns2
         real unt2
         real unrr2
         real unss2
         real unrs2
         real untt2
         real unrt2
         real unst2
         real unrrr2
         real unsss2
         real unttt2
         real unx21
         real uny21
         real unz21
         real unx22
         real uny22
         real unz22
         real unx23
         real uny23
         real unz23
         real unxx21
         real unyy21
         real unxy21
         real unxz21
         real unyz21
         real unzz21
         real unlaplacian21
         real unxx22
         real unyy22
         real unxy22
         real unxz22
         real unyz22
         real unzz22
         real unlaplacian22
         real unxx23
         real unyy23
         real unzz23
         real unxy23
         real unxz23
         real unyz23
         real unlaplacian23
         real unx23r
         real uny23r
         real unz23r
         real unxx23r
         real unyy23r
         real unxy23r
         real unzz23r
         real unxz23r
         real unyz23r
         real unx21r
         real uny21r
         real unz21r
         real unxx21r
         real unyy21r
         real unzz21r
         real unxy21r
         real unxz21r
         real unyz21r
         real unlaplacian21r
         real unx22r
         real uny22r
         real unz22r
         real unxx22r
         real unyy22r
         real unzz22r
         real unxy22r
         real unxz22r
         real unyz22r
         real unlaplacian22r
         real unlaplacian23r
         real unxxx22r
         real unyyy22r
         real unxxy22r
         real unxyy22r
         real unxxxx22r
         real unyyyy22r
         real unxxyy22r
         real unxxx23r
         real unyyy23r
         real unzzz23r
         real unxxy23r
         real unxxz23r
         real unxyy23r
         real unyyz23r
         real unxzz23r
         real unyzz23r
         real unxxxx23r
         real unyyyy23r
         real unzzzz23r
         real unxxyy23r
         real unxxzz23r
         real unyyzz23r
         real unLapSq22r
         real unLapSq23r
         real vr2
         real vs2
         real vt2
         real vrr2
         real vss2
         real vrs2
         real vtt2
         real vrt2
         real vst2
         real vrrr2
         real vsss2
         real vttt2
         real vx21
         real vy21
         real vz21
         real vx22
         real vy22
         real vz22
         real vx23
         real vy23
         real vz23
         real vxx21
         real vyy21
         real vxy21
         real vxz21
         real vyz21
         real vzz21
         real vlaplacian21
         real vxx22
         real vyy22
         real vxy22
         real vxz22
         real vyz22
         real vzz22
         real vlaplacian22
         real vxx23
         real vyy23
         real vzz23
         real vxy23
         real vxz23
         real vyz23
         real vlaplacian23
         real vx23r
         real vy23r
         real vz23r
         real vxx23r
         real vyy23r
         real vxy23r
         real vzz23r
         real vxz23r
         real vyz23r
         real vx21r
         real vy21r
         real vz21r
         real vxx21r
         real vyy21r
         real vzz21r
         real vxy21r
         real vxz21r
         real vyz21r
         real vlaplacian21r
         real vx22r
         real vy22r
         real vz22r
         real vxx22r
         real vyy22r
         real vzz22r
         real vxy22r
         real vxz22r
         real vyz22r
         real vlaplacian22r
         real vlaplacian23r
         real vxxx22r
         real vyyy22r
         real vxxy22r
         real vxyy22r
         real vxxxx22r
         real vyyyy22r
         real vxxyy22r
         real vxxx23r
         real vyyy23r
         real vzzz23r
         real vxxy23r
         real vxxz23r
         real vxyy23r
         real vyyz23r
         real vxzz23r
         real vyzz23r
         real vxxxx23r
         real vyyyy23r
         real vzzzz23r
         real vxxyy23r
         real vxxzz23r
         real vyyzz23r
         real vLapSq22r
         real vLapSq23r
         real umr2
         real ums2
         real umt2
         real umrr2
         real umss2
         real umrs2
         real umtt2
         real umrt2
         real umst2
         real umrrr2
         real umsss2
         real umttt2
         real umx21
         real umy21
         real umz21
         real umx22
         real umy22
         real umz22
         real umx23
         real umy23
         real umz23
         real umxx21
         real umyy21
         real umxy21
         real umxz21
         real umyz21
         real umzz21
         real umlaplacian21
         real umxx22
         real umyy22
         real umxy22
         real umxz22
         real umyz22
         real umzz22
         real umlaplacian22
         real umxx23
         real umyy23
         real umzz23
         real umxy23
         real umxz23
         real umyz23
         real umlaplacian23
         real umx23r
         real umy23r
         real umz23r
         real umxx23r
         real umyy23r
         real umxy23r
         real umzz23r
         real umxz23r
         real umyz23r
         real umx21r
         real umy21r
         real umz21r
         real umxx21r
         real umyy21r
         real umzz21r
         real umxy21r
         real umxz21r
         real umyz21r
         real umlaplacian21r
         real umx22r
         real umy22r
         real umz22r
         real umxx22r
         real umyy22r
         real umzz22r
         real umxy22r
         real umxz22r
         real umyz22r
         real umlaplacian22r
         real umlaplacian23r
         real umxxx22r
         real umyyy22r
         real umxxy22r
         real umxyy22r
         real umxxxx22r
         real umyyyy22r
         real umxxyy22r
         real umxxx23r
         real umyyy23r
         real umzzz23r
         real umxxy23r
         real umxxz23r
         real umxyy23r
         real umyyz23r
         real umxzz23r
         real umyzz23r
         real umxxxx23r
         real umyyyy23r
         real umzzzz23r
         real umxxyy23r
         real umxxzz23r
         real umyyzz23r
         real umLapSq22r
         real umLapSq23r
         real ffr2
         real ffs2
         real fft2
         real ffrr2
         real ffss2
         real ffrs2
         real fftt2
         real ffrt2
         real ffst2
         real ffrrr2
         real ffsss2
         real ffttt2
         real ffx21
         real ffy21
         real ffz21
         real ffx22
         real ffy22
         real ffz22
         real ffx23
         real ffy23
         real ffz23
         real ffxx21
         real ffyy21
         real ffxy21
         real ffxz21
         real ffyz21
         real ffzz21
         real fflaplacian21
         real ffxx22
         real ffyy22
         real ffxy22
         real ffxz22
         real ffyz22
         real ffzz22
         real fflaplacian22
         real ffxx23
         real ffyy23
         real ffzz23
         real ffxy23
         real ffxz23
         real ffyz23
         real fflaplacian23
         real ffx23r
         real ffy23r
         real ffz23r
         real ffxx23r
         real ffyy23r
         real ffxy23r
         real ffzz23r
         real ffxz23r
         real ffyz23r
         real ffx21r
         real ffy21r
         real ffz21r
         real ffxx21r
         real ffyy21r
         real ffzz21r
         real ffxy21r
         real ffxz21r
         real ffyz21r
         real fflaplacian21r
         real ffx22r
         real ffy22r
         real ffz22r
         real ffxx22r
         real ffyy22r
         real ffzz22r
         real ffxy22r
         real ffxz22r
         real ffyz22r
         real fflaplacian22r
         real fflaplacian23r
         real ffxxx22r
         real ffyyy22r
         real ffxxy22r
         real ffxyy22r
         real ffxxxx22r
         real ffyyyy22r
         real ffxxyy22r
         real ffxxx23r
         real ffyyy23r
         real ffzzz23r
         real ffxxy23r
         real ffxxz23r
         real ffxyy23r
         real ffyyz23r
         real ffxzz23r
         real ffyzz23r
         real ffxxxx23r
         real ffyyyy23r
         real ffzzzz23r
         real ffxxyy23r
         real ffxxzz23r
         real ffyyzz23r
         real ffLapSq22r
         real ffLapSq23r
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
         real unr4
         real uns4
         real unt4
         real unrr4
         real unss4
         real untt4
         real unrs4
         real unrt4
         real unst4
         real unx41
         real uny41
         real unz41
         real unx42
         real uny42
         real unz42
         real unx43
         real uny43
         real unz43
         real unxx41
         real unyy41
         real unxy41
         real unxz41
         real unyz41
         real unzz41
         real unlaplacian41
         real unxx42
         real unyy42
         real unxy42
         real unxz42
         real unyz42
         real unzz42
         real unlaplacian42
         real unxx43
         real unyy43
         real unzz43
         real unxy43
         real unxz43
         real unyz43
         real unlaplacian43
         real unx43r
         real uny43r
         real unz43r
         real unxx43r
         real unyy43r
         real unzz43r
         real unxy43r
         real unxz43r
         real unyz43r
         real unx41r
         real uny41r
         real unz41r
         real unxx41r
         real unyy41r
         real unzz41r
         real unxy41r
         real unxz41r
         real unyz41r
         real unlaplacian41r
         real unx42r
         real uny42r
         real unz42r
         real unxx42r
         real unyy42r
         real unzz42r
         real unxy42r
         real unxz42r
         real unyz42r
         real unlaplacian42r
         real unlaplacian43r
         real vr4
         real vs4
         real vt4
         real vrr4
         real vss4
         real vtt4
         real vrs4
         real vrt4
         real vst4
         real vx41
         real vy41
         real vz41
         real vx42
         real vy42
         real vz42
         real vx43
         real vy43
         real vz43
         real vxx41
         real vyy41
         real vxy41
         real vxz41
         real vyz41
         real vzz41
         real vlaplacian41
         real vxx42
         real vyy42
         real vxy42
         real vxz42
         real vyz42
         real vzz42
         real vlaplacian42
         real vxx43
         real vyy43
         real vzz43
         real vxy43
         real vxz43
         real vyz43
         real vlaplacian43
         real vx43r
         real vy43r
         real vz43r
         real vxx43r
         real vyy43r
         real vzz43r
         real vxy43r
         real vxz43r
         real vyz43r
         real vx41r
         real vy41r
         real vz41r
         real vxx41r
         real vyy41r
         real vzz41r
         real vxy41r
         real vxz41r
         real vyz41r
         real vlaplacian41r
         real vx42r
         real vy42r
         real vz42r
         real vxx42r
         real vyy42r
         real vzz42r
         real vxy42r
         real vxz42r
         real vyz42r
         real vlaplacian42r
         real vlaplacian43r
        real maxwell2dr,maxwell3dr,maxwellr44,maxwellr66,maxwellr88
        real maxwellc22,maxwellc44,maxwellc66,maxwellc88
        real maxwell2dr44me,maxwell2dr66me,maxwell2dr88me
        real maxwell3dr44me,maxwell3dr66me,maxwell3dr88me
        real maxwellc44me,maxwellc66me,maxwellc88me
        real max2dc44me,max2dc44me2,max3dc44me
        real mxdc2d2Ex,mxdc2d2Ey,mxdc2d4Ex,mxdc2d4Ey, mxdc2d4cEx,
     & mxdc2d4cEy
        real mxdc2d2cEx,mxdc2d2cEy
        real mxdc3d2Ex,mxdc3d2Ey,mxdc3d2Ez,mxdc3d2Hx,mxdc3d2Hy,
     & mxdc3d2Hz
        real mxdc3d2cEx,mxdc3d2cEy,mxdc3d2cEz,mxdc3d2cHx,mxdc3d2cHy,
     & mxdc3d2cHz
        real mxdc2d4cConsEx,mxdc2d4cConsEy,mxdc2d4cConsEz
        real mxdc3d4Ex,mxdc3d4Ey,mxdc3d4Ez,mxdc3d4Hx,mxdc3d4Hy,
     & mxdc3d4Hz
       ! real vr2,vs2,vrr2,vss2,vrs2,vLaplacian22
        real cdt4by360,cdt6by20160
        real lap2d2,lap3d2,lap2d4,lap3d4,lap2d6,lap3d6,lap2d8,lap3d8,
     & lap2d2Pow2,lap3d2Pow2,lap2d2Pow3,lap3d2Pow3,lap2d2Pow4,
     & lap3d2Pow4,lap2d4Pow2,lap3d4Pow2,lap2d4Pow3,lap3d4Pow3,
     & lap2d6Pow2,lap3d6Pow2
        real lap2d2m,lap3d2m
        real du,fd22d,fd23d,fd42d,fd43d,fd62d,fd63d,fd82d,fd83d
        ! forcing correction functions: 
        real lap2d2f,f2drme44, lap3d2f, f3drme44, f2dcme44, f3dcme44, 
     & ff
        ! div cleaning: 
        real dc,dcp,cdc0,cdc1,cdcxx,cdcyy,cdczz,cdcEdx,cdcEdy,cdcEdz,
     & cdcHdx,cdcHdy,cdcHdz,cdcf
        real cdcE,cdcELap,cdcELapsq,cdcELapm,cdcHzxLap,cdcHzyLap
        real cdcH,cdcHLap,cdcHLapsq,cdcHLapm
        ! dispersion
        integer dispersionModel,pxc,pyc,pzc,qxc,qyc,qzc,rxc,ryc,rzc
       ! real unxx22r,unyy22r,unxy22r,unx22r
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
       !     The next macro will define the difference approximation statement functions
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
        unr2(i1,i2,i3,kd)=(un(i1+1,i2,i3,kd)-un(i1-1,i2,i3,kd))*d12(0)
        uns2(i1,i2,i3,kd)=(un(i1,i2+1,i3,kd)-un(i1,i2-1,i3,kd))*d12(1)
        unt2(i1,i2,i3,kd)=(un(i1,i2,i3+1,kd)-un(i1,i2,i3-1,kd))*d12(2)
        unrr2(i1,i2,i3,kd)=(-2.*un(i1,i2,i3,kd)+(un(i1+1,i2,i3,kd)+un(
     & i1-1,i2,i3,kd)) )*d22(0)
        unss2(i1,i2,i3,kd)=(-2.*un(i1,i2,i3,kd)+(un(i1,i2+1,i3,kd)+un(
     & i1,i2-1,i3,kd)) )*d22(1)
        unrs2(i1,i2,i3,kd)=(unr2(i1,i2+1,i3,kd)-unr2(i1,i2-1,i3,kd))*
     & d12(1)
        untt2(i1,i2,i3,kd)=(-2.*un(i1,i2,i3,kd)+(un(i1,i2,i3+1,kd)+un(
     & i1,i2,i3-1,kd)) )*d22(2)
        unrt2(i1,i2,i3,kd)=(unr2(i1,i2,i3+1,kd)-unr2(i1,i2,i3-1,kd))*
     & d12(2)
        unst2(i1,i2,i3,kd)=(uns2(i1,i2,i3+1,kd)-uns2(i1,i2,i3-1,kd))*
     & d12(2)
        unrrr2(i1,i2,i3,kd)=(-2.*(un(i1+1,i2,i3,kd)-un(i1-1,i2,i3,kd))+
     & (un(i1+2,i2,i3,kd)-un(i1-2,i2,i3,kd)) )*d22(0)*d12(0)
        unsss2(i1,i2,i3,kd)=(-2.*(un(i1,i2+1,i3,kd)-un(i1,i2-1,i3,kd))+
     & (un(i1,i2+2,i3,kd)-un(i1,i2-2,i3,kd)) )*d22(1)*d12(1)
        unttt2(i1,i2,i3,kd)=(-2.*(un(i1,i2,i3+1,kd)-un(i1,i2,i3-1,kd))+
     & (un(i1,i2,i3+2,kd)-un(i1,i2,i3-2,kd)) )*d22(2)*d12(2)
        unx21(i1,i2,i3,kd)= rx(i1,i2,i3)*unr2(i1,i2,i3,kd)
        uny21(i1,i2,i3,kd)=0
        unz21(i1,i2,i3,kd)=0
        unx22(i1,i2,i3,kd)= rx(i1,i2,i3)*unr2(i1,i2,i3,kd)+sx(i1,i2,i3)
     & *uns2(i1,i2,i3,kd)
        uny22(i1,i2,i3,kd)= ry(i1,i2,i3)*unr2(i1,i2,i3,kd)+sy(i1,i2,i3)
     & *uns2(i1,i2,i3,kd)
        unz22(i1,i2,i3,kd)=0
        unx23(i1,i2,i3,kd)=rx(i1,i2,i3)*unr2(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & uns2(i1,i2,i3,kd)+tx(i1,i2,i3)*unt2(i1,i2,i3,kd)
        uny23(i1,i2,i3,kd)=ry(i1,i2,i3)*unr2(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & uns2(i1,i2,i3,kd)+ty(i1,i2,i3)*unt2(i1,i2,i3,kd)
        unz23(i1,i2,i3,kd)=rz(i1,i2,i3)*unr2(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & uns2(i1,i2,i3,kd)+tz(i1,i2,i3)*unt2(i1,i2,i3,kd)
        unxx21(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*unrr2(i1,i2,i3,kd)+(
     & rxx22(i1,i2,i3))*unr2(i1,i2,i3,kd)
        unyy21(i1,i2,i3,kd)=0
        unxy21(i1,i2,i3,kd)=0
        unxz21(i1,i2,i3,kd)=0
        unyz21(i1,i2,i3,kd)=0
        unzz21(i1,i2,i3,kd)=0
        unlaplacian21(i1,i2,i3,kd)=unxx21(i1,i2,i3,kd)
        unxx22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*unrr2(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*unrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2)
     & *unss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3))*unr2(i1,i2,i3,kd)+(sxx22(
     & i1,i2,i3))*uns2(i1,i2,i3,kd)
        unyy22(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*unrr2(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*unrs2(i1,i2,i3,kd)+(sy(i1,i2,i3)**2)
     & *unss2(i1,i2,i3,kd)+(ryy22(i1,i2,i3))*unr2(i1,i2,i3,kd)+(syy22(
     & i1,i2,i3))*uns2(i1,i2,i3,kd)
        unxy22(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*unrr2(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & unrs2(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*unss2(i1,i2,i3,kd)
     & +rxy22(i1,i2,i3)*unr2(i1,i2,i3,kd)+sxy22(i1,i2,i3)*uns2(i1,i2,
     & i3,kd)
        unxz22(i1,i2,i3,kd)=0
        unyz22(i1,i2,i3,kd)=0
        unzz22(i1,i2,i3,kd)=0
        unlaplacian22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & unrr2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*
     & sy(i1,i2,i3))*unrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)*
     & *2)*unss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3)+ryy22(i1,i2,i3))*unr2(
     & i1,i2,i3,kd)+(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*uns2(i1,i2,i3,
     & kd)
        unxx23(i1,i2,i3,kd)=rx(i1,i2,i3)**2*unrr2(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*unss2(i1,i2,i3,kd)+tx(i1,i2,i3)**2*untt2(i1,i2,i3,kd)
     & +2.*rx(i1,i2,i3)*sx(i1,i2,i3)*unrs2(i1,i2,i3,kd)+2.*rx(i1,i2,
     & i3)*tx(i1,i2,i3)*unrt2(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,i2,
     & i3)*unst2(i1,i2,i3,kd)+rxx23(i1,i2,i3)*unr2(i1,i2,i3,kd)+sxx23(
     & i1,i2,i3)*uns2(i1,i2,i3,kd)+txx23(i1,i2,i3)*unt2(i1,i2,i3,kd)
        unyy23(i1,i2,i3,kd)=ry(i1,i2,i3)**2*unrr2(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*unss2(i1,i2,i3,kd)+ty(i1,i2,i3)**2*untt2(i1,i2,i3,kd)
     & +2.*ry(i1,i2,i3)*sy(i1,i2,i3)*unrs2(i1,i2,i3,kd)+2.*ry(i1,i2,
     & i3)*ty(i1,i2,i3)*unrt2(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,i2,
     & i3)*unst2(i1,i2,i3,kd)+ryy23(i1,i2,i3)*unr2(i1,i2,i3,kd)+syy23(
     & i1,i2,i3)*uns2(i1,i2,i3,kd)+tyy23(i1,i2,i3)*unt2(i1,i2,i3,kd)
        unzz23(i1,i2,i3,kd)=rz(i1,i2,i3)**2*unrr2(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*unss2(i1,i2,i3,kd)+tz(i1,i2,i3)**2*untt2(i1,i2,i3,kd)
     & +2.*rz(i1,i2,i3)*sz(i1,i2,i3)*unrs2(i1,i2,i3,kd)+2.*rz(i1,i2,
     & i3)*tz(i1,i2,i3)*unrt2(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,i2,
     & i3)*unst2(i1,i2,i3,kd)+rzz23(i1,i2,i3)*unr2(i1,i2,i3,kd)+szz23(
     & i1,i2,i3)*uns2(i1,i2,i3,kd)+tzz23(i1,i2,i3)*unt2(i1,i2,i3,kd)
        unxy23(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*unrr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*unss2(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & ty(i1,i2,i3)*untt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(
     & i1,i2,i3)*sx(i1,i2,i3))*unrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(i1,
     & i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*unrt2(i1,i2,i3,kd)+(sx(i1,i2,
     & i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*unst2(i1,i2,i3,kd)+
     & rxy23(i1,i2,i3)*unr2(i1,i2,i3,kd)+sxy23(i1,i2,i3)*uns2(i1,i2,
     & i3,kd)+txy23(i1,i2,i3)*unt2(i1,i2,i3,kd)
        unxz23(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*unrr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*unss2(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & tz(i1,i2,i3)*untt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sx(i1,i2,i3))*unrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(i1,
     & i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*unrt2(i1,i2,i3,kd)+(sx(i1,i2,
     & i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*unst2(i1,i2,i3,kd)+
     & rxz23(i1,i2,i3)*unr2(i1,i2,i3,kd)+sxz23(i1,i2,i3)*uns2(i1,i2,
     & i3,kd)+txz23(i1,i2,i3)*unt2(i1,i2,i3,kd)
        unyz23(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*unrr2(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*unss2(i1,i2,i3,kd)+ty(i1,i2,i3)*
     & tz(i1,i2,i3)*untt2(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sy(i1,i2,i3))*unrs2(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(i1,
     & i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*unrt2(i1,i2,i3,kd)+(sy(i1,i2,
     & i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*unst2(i1,i2,i3,kd)+
     & ryz23(i1,i2,i3)*unr2(i1,i2,i3,kd)+syz23(i1,i2,i3)*uns2(i1,i2,
     & i3,kd)+tyz23(i1,i2,i3)*unt2(i1,i2,i3,kd)
        unlaplacian23(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*unrr2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)*
     & *2+sz(i1,i2,i3)**2)*unss2(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(i1,
     & i2,i3)**2+tz(i1,i2,i3)**2)*untt2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*
     & sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,
     & i3))*unrs2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(i1,
     & i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*unrt2(i1,i2,i3,
     & kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)+
     & sz(i1,i2,i3)*tz(i1,i2,i3))*unst2(i1,i2,i3,kd)+(rxx23(i1,i2,i3)+
     & ryy23(i1,i2,i3)+rzz23(i1,i2,i3))*unr2(i1,i2,i3,kd)+(sxx23(i1,
     & i2,i3)+syy23(i1,i2,i3)+szz23(i1,i2,i3))*uns2(i1,i2,i3,kd)+(
     & txx23(i1,i2,i3)+tyy23(i1,i2,i3)+tzz23(i1,i2,i3))*unt2(i1,i2,i3,
     & kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
        unx23r(i1,i2,i3,kd)=(un(i1+1,i2,i3,kd)-un(i1-1,i2,i3,kd))*h12(
     & 0)
        uny23r(i1,i2,i3,kd)=(un(i1,i2+1,i3,kd)-un(i1,i2-1,i3,kd))*h12(
     & 1)
        unz23r(i1,i2,i3,kd)=(un(i1,i2,i3+1,kd)-un(i1,i2,i3-1,kd))*h12(
     & 2)
        unxx23r(i1,i2,i3,kd)=(-2.*un(i1,i2,i3,kd)+(un(i1+1,i2,i3,kd)+
     & un(i1-1,i2,i3,kd)) )*h22(0)
        unyy23r(i1,i2,i3,kd)=(-2.*un(i1,i2,i3,kd)+(un(i1,i2+1,i3,kd)+
     & un(i1,i2-1,i3,kd)) )*h22(1)
        unxy23r(i1,i2,i3,kd)=(unx23r(i1,i2+1,i3,kd)-unx23r(i1,i2-1,i3,
     & kd))*h12(1)
        unzz23r(i1,i2,i3,kd)=(-2.*un(i1,i2,i3,kd)+(un(i1,i2,i3+1,kd)+
     & un(i1,i2,i3-1,kd)) )*h22(2)
        unxz23r(i1,i2,i3,kd)=(unx23r(i1,i2,i3+1,kd)-unx23r(i1,i2,i3-1,
     & kd))*h12(2)
        unyz23r(i1,i2,i3,kd)=(uny23r(i1,i2,i3+1,kd)-uny23r(i1,i2,i3-1,
     & kd))*h12(2)
        unx21r(i1,i2,i3,kd)= unx23r(i1,i2,i3,kd)
        uny21r(i1,i2,i3,kd)= uny23r(i1,i2,i3,kd)
        unz21r(i1,i2,i3,kd)= unz23r(i1,i2,i3,kd)
        unxx21r(i1,i2,i3,kd)= unxx23r(i1,i2,i3,kd)
        unyy21r(i1,i2,i3,kd)= unyy23r(i1,i2,i3,kd)
        unzz21r(i1,i2,i3,kd)= unzz23r(i1,i2,i3,kd)
        unxy21r(i1,i2,i3,kd)= unxy23r(i1,i2,i3,kd)
        unxz21r(i1,i2,i3,kd)= unxz23r(i1,i2,i3,kd)
        unyz21r(i1,i2,i3,kd)= unyz23r(i1,i2,i3,kd)
        unlaplacian21r(i1,i2,i3,kd)=unxx23r(i1,i2,i3,kd)
        unx22r(i1,i2,i3,kd)= unx23r(i1,i2,i3,kd)
        uny22r(i1,i2,i3,kd)= uny23r(i1,i2,i3,kd)
        unz22r(i1,i2,i3,kd)= unz23r(i1,i2,i3,kd)
        unxx22r(i1,i2,i3,kd)= unxx23r(i1,i2,i3,kd)
        unyy22r(i1,i2,i3,kd)= unyy23r(i1,i2,i3,kd)
        unzz22r(i1,i2,i3,kd)= unzz23r(i1,i2,i3,kd)
        unxy22r(i1,i2,i3,kd)= unxy23r(i1,i2,i3,kd)
        unxz22r(i1,i2,i3,kd)= unxz23r(i1,i2,i3,kd)
        unyz22r(i1,i2,i3,kd)= unyz23r(i1,i2,i3,kd)
        unlaplacian22r(i1,i2,i3,kd)=unxx23r(i1,i2,i3,kd)+unyy23r(i1,i2,
     & i3,kd)
        unlaplacian23r(i1,i2,i3,kd)=unxx23r(i1,i2,i3,kd)+unyy23r(i1,i2,
     & i3,kd)+unzz23r(i1,i2,i3,kd)
        unxxx22r(i1,i2,i3,kd)=(-2.*(un(i1+1,i2,i3,kd)-un(i1-1,i2,i3,kd)
     & )+(un(i1+2,i2,i3,kd)-un(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
        unyyy22r(i1,i2,i3,kd)=(-2.*(un(i1,i2+1,i3,kd)-un(i1,i2-1,i3,kd)
     & )+(un(i1,i2+2,i3,kd)-un(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
        unxxy22r(i1,i2,i3,kd)=( unxx22r(i1,i2+1,i3,kd)-unxx22r(i1,i2-1,
     & i3,kd))/(2.*dx(1))
        unxyy22r(i1,i2,i3,kd)=( unyy22r(i1+1,i2,i3,kd)-unyy22r(i1-1,i2,
     & i3,kd))/(2.*dx(0))
        unxxxx22r(i1,i2,i3,kd)=(6.*un(i1,i2,i3,kd)-4.*(un(i1+1,i2,i3,
     & kd)+un(i1-1,i2,i3,kd))+(un(i1+2,i2,i3,kd)+un(i1-2,i2,i3,kd)) )
     & /(dx(0)**4)
        unyyyy22r(i1,i2,i3,kd)=(6.*un(i1,i2,i3,kd)-4.*(un(i1,i2+1,i3,
     & kd)+un(i1,i2-1,i3,kd))+(un(i1,i2+2,i3,kd)+un(i1,i2-2,i3,kd)) )
     & /(dx(1)**4)
        unxxyy22r(i1,i2,i3,kd)=( 4.*un(i1,i2,i3,kd)     -2.*(un(i1+1,
     & i2,i3,kd)+un(i1-1,i2,i3,kd)+un(i1,i2+1,i3,kd)+un(i1,i2-1,i3,kd)
     & )   +   (un(i1+1,i2+1,i3,kd)+un(i1-1,i2+1,i3,kd)+un(i1+1,i2-1,
     & i3,kd)+un(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
        ! 2D laplacian squared = un.xxxx + 2 un.xxyy + un.yyyy
        unLapSq22r(i1,i2,i3,kd)= ( 6.*un(i1,i2,i3,kd)   - 4.*(un(i1+1,
     & i2,i3,kd)+un(i1-1,i2,i3,kd))    +(un(i1+2,i2,i3,kd)+un(i1-2,i2,
     & i3,kd)) )/(dx(0)**4) +( 6.*un(i1,i2,i3,kd)    -4.*(un(i1,i2+1,
     & i3,kd)+un(i1,i2-1,i3,kd))    +(un(i1,i2+2,i3,kd)+un(i1,i2-2,i3,
     & kd)) )/(dx(1)**4)  +( 8.*un(i1,i2,i3,kd)     -4.*(un(i1+1,i2,
     & i3,kd)+un(i1-1,i2,i3,kd)+un(i1,i2+1,i3,kd)+un(i1,i2-1,i3,kd))  
     &  +2.*(un(i1+1,i2+1,i3,kd)+un(i1-1,i2+1,i3,kd)+un(i1+1,i2-1,i3,
     & kd)+un(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
        unxxx23r(i1,i2,i3,kd)=(-2.*(un(i1+1,i2,i3,kd)-un(i1-1,i2,i3,kd)
     & )+(un(i1+2,i2,i3,kd)-un(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
        unyyy23r(i1,i2,i3,kd)=(-2.*(un(i1,i2+1,i3,kd)-un(i1,i2-1,i3,kd)
     & )+(un(i1,i2+2,i3,kd)-un(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
        unzzz23r(i1,i2,i3,kd)=(-2.*(un(i1,i2,i3+1,kd)-un(i1,i2,i3-1,kd)
     & )+(un(i1,i2,i3+2,kd)-un(i1,i2,i3-2,kd)) )*h22(1)*h12(2)
        unxxy23r(i1,i2,i3,kd)=( unxx22r(i1,i2+1,i3,kd)-unxx22r(i1,i2-1,
     & i3,kd))/(2.*dx(1))
        unxyy23r(i1,i2,i3,kd)=( unyy22r(i1+1,i2,i3,kd)-unyy22r(i1-1,i2,
     & i3,kd))/(2.*dx(0))
        unxxz23r(i1,i2,i3,kd)=( unxx22r(i1,i2,i3+1,kd)-unxx22r(i1,i2,
     & i3-1,kd))/(2.*dx(2))
        unyyz23r(i1,i2,i3,kd)=( unyy22r(i1,i2,i3+1,kd)-unyy22r(i1,i2,
     & i3-1,kd))/(2.*dx(2))
        unxzz23r(i1,i2,i3,kd)=( unzz22r(i1+1,i2,i3,kd)-unzz22r(i1-1,i2,
     & i3,kd))/(2.*dx(0))
        unyzz23r(i1,i2,i3,kd)=( unzz22r(i1,i2+1,i3,kd)-unzz22r(i1,i2-1,
     & i3,kd))/(2.*dx(1))
        unxxxx23r(i1,i2,i3,kd)=(6.*un(i1,i2,i3,kd)-4.*(un(i1+1,i2,i3,
     & kd)+un(i1-1,i2,i3,kd))+(un(i1+2,i2,i3,kd)+un(i1-2,i2,i3,kd)) )
     & /(dx(0)**4)
        unyyyy23r(i1,i2,i3,kd)=(6.*un(i1,i2,i3,kd)-4.*(un(i1,i2+1,i3,
     & kd)+un(i1,i2-1,i3,kd))+(un(i1,i2+2,i3,kd)+un(i1,i2-2,i3,kd)) )
     & /(dx(1)**4)
        unzzzz23r(i1,i2,i3,kd)=(6.*un(i1,i2,i3,kd)-4.*(un(i1,i2,i3+1,
     & kd)+un(i1,i2,i3-1,kd))+(un(i1,i2,i3+2,kd)+un(i1,i2,i3-2,kd)) )
     & /(dx(2)**4)
        unxxyy23r(i1,i2,i3,kd)=( 4.*un(i1,i2,i3,kd)     -2.*(un(i1+1,
     & i2,i3,kd)+un(i1-1,i2,i3,kd)+un(i1,i2+1,i3,kd)+un(i1,i2-1,i3,kd)
     & )   +   (un(i1+1,i2+1,i3,kd)+un(i1-1,i2+1,i3,kd)+un(i1+1,i2-1,
     & i3,kd)+un(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
        unxxzz23r(i1,i2,i3,kd)=( 4.*un(i1,i2,i3,kd)     -2.*(un(i1+1,
     & i2,i3,kd)+un(i1-1,i2,i3,kd)+un(i1,i2,i3+1,kd)+un(i1,i2,i3-1,kd)
     & )   +   (un(i1+1,i2,i3+1,kd)+un(i1-1,i2,i3+1,kd)+un(i1+1,i2,i3-
     & 1,kd)+un(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)
        unyyzz23r(i1,i2,i3,kd)=( 4.*un(i1,i2,i3,kd)     -2.*(un(i1,i2+
     & 1,i3,kd)  +un(i1,i2-1,i3,kd)+  un(i1,i2  ,i3+1,kd)+un(i1,i2  ,
     & i3-1,kd))   +   (un(i1,i2+1,i3+1,kd)+un(i1,i2-1,i3+1,kd)+un(i1,
     & i2+1,i3-1,kd)+un(i1,i2-1,i3-1,kd)) )/(dx(1)**2*dx(2)**2)
        ! 3D laplacian squared = un.xxxx + un.yyyy + un.zzzz + 2 (un.xxyy + un.xxzz + un.yyzz )
        unLapSq23r(i1,i2,i3,kd)= ( 6.*un(i1,i2,i3,kd)   - 4.*(un(i1+1,
     & i2,i3,kd)+un(i1-1,i2,i3,kd))    +(un(i1+2,i2,i3,kd)+un(i1-2,i2,
     & i3,kd)) )/(dx(0)**4) +( 6.*un(i1,i2,i3,kd)    -4.*(un(i1,i2+1,
     & i3,kd)+un(i1,i2-1,i3,kd))    +(un(i1,i2+2,i3,kd)+un(i1,i2-2,i3,
     & kd)) )/(dx(1)**4)  +( 6.*un(i1,i2,i3,kd)    -4.*(un(i1,i2,i3+1,
     & kd)+un(i1,i2,i3-1,kd))    +(un(i1,i2,i3+2,kd)+un(i1,i2,i3-2,kd)
     & ) )/(dx(2)**4)  +( 8.*un(i1,i2,i3,kd)     -4.*(un(i1+1,i2,i3,
     & kd)  +un(i1-1,i2,i3,kd)  +un(i1  ,i2+1,i3,kd)+un(i1  ,i2-1,i3,
     & kd))   +2.*(un(i1+1,i2+1,i3,kd)+un(i1-1,i2+1,i3,kd)+un(i1+1,i2-
     & 1,i3,kd)+un(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)+( 8.*un(i1,
     & i2,i3,kd)     -4.*(un(i1+1,i2,i3,kd)  +un(i1-1,i2,i3,kd)  +un(
     & i1  ,i2,i3+1,kd)+un(i1  ,i2,i3-1,kd))   +2.*(un(i1+1,i2,i3+1,
     & kd)+un(i1-1,i2,i3+1,kd)+un(i1+1,i2,i3-1,kd)+un(i1-1,i2,i3-1,kd)
     & ) )/(dx(0)**2*dx(2)**2)+( 8.*un(i1,i2,i3,kd)     -4.*(un(i1,i2+
     & 1,i3,kd)  +un(i1,i2-1,i3,kd)  +un(i1,i2  ,i3+1,kd)+un(i1,i2  ,
     & i3-1,kd))   +2.*(un(i1,i2+1,i3+1,kd)+un(i1,i2-1,i3+1,kd)+un(i1,
     & i2+1,i3-1,kd)+un(i1,i2-1,i3-1,kd)) )/(dx(1)**2*dx(2)**2)
        unr4(i1,i2,i3,kd)=(8.*(un(i1+1,i2,i3,kd)-un(i1-1,i2,i3,kd))-(
     & un(i1+2,i2,i3,kd)-un(i1-2,i2,i3,kd)))*d14(0)
        uns4(i1,i2,i3,kd)=(8.*(un(i1,i2+1,i3,kd)-un(i1,i2-1,i3,kd))-(
     & un(i1,i2+2,i3,kd)-un(i1,i2-2,i3,kd)))*d14(1)
        unt4(i1,i2,i3,kd)=(8.*(un(i1,i2,i3+1,kd)-un(i1,i2,i3-1,kd))-(
     & un(i1,i2,i3+2,kd)-un(i1,i2,i3-2,kd)))*d14(2)
        unrr4(i1,i2,i3,kd)=(-30.*un(i1,i2,i3,kd)+16.*(un(i1+1,i2,i3,kd)
     & +un(i1-1,i2,i3,kd))-(un(i1+2,i2,i3,kd)+un(i1-2,i2,i3,kd)) )*
     & d24(0)
        unss4(i1,i2,i3,kd)=(-30.*un(i1,i2,i3,kd)+16.*(un(i1,i2+1,i3,kd)
     & +un(i1,i2-1,i3,kd))-(un(i1,i2+2,i3,kd)+un(i1,i2-2,i3,kd)) )*
     & d24(1)
        untt4(i1,i2,i3,kd)=(-30.*un(i1,i2,i3,kd)+16.*(un(i1,i2,i3+1,kd)
     & +un(i1,i2,i3-1,kd))-(un(i1,i2,i3+2,kd)+un(i1,i2,i3-2,kd)) )*
     & d24(2)
        unrs4(i1,i2,i3,kd)=(8.*(unr4(i1,i2+1,i3,kd)-unr4(i1,i2-1,i3,kd)
     & )-(unr4(i1,i2+2,i3,kd)-unr4(i1,i2-2,i3,kd)))*d14(1)
        unrt4(i1,i2,i3,kd)=(8.*(unr4(i1,i2,i3+1,kd)-unr4(i1,i2,i3-1,kd)
     & )-(unr4(i1,i2,i3+2,kd)-unr4(i1,i2,i3-2,kd)))*d14(2)
        unst4(i1,i2,i3,kd)=(8.*(uns4(i1,i2,i3+1,kd)-uns4(i1,i2,i3-1,kd)
     & )-(uns4(i1,i2,i3+2,kd)-uns4(i1,i2,i3-2,kd)))*d14(2)
        unx41(i1,i2,i3,kd)= rx(i1,i2,i3)*unr4(i1,i2,i3,kd)
        uny41(i1,i2,i3,kd)=0
        unz41(i1,i2,i3,kd)=0
        unx42(i1,i2,i3,kd)= rx(i1,i2,i3)*unr4(i1,i2,i3,kd)+sx(i1,i2,i3)
     & *uns4(i1,i2,i3,kd)
        uny42(i1,i2,i3,kd)= ry(i1,i2,i3)*unr4(i1,i2,i3,kd)+sy(i1,i2,i3)
     & *uns4(i1,i2,i3,kd)
        unz42(i1,i2,i3,kd)=0
        unx43(i1,i2,i3,kd)=rx(i1,i2,i3)*unr4(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & uns4(i1,i2,i3,kd)+tx(i1,i2,i3)*unt4(i1,i2,i3,kd)
        uny43(i1,i2,i3,kd)=ry(i1,i2,i3)*unr4(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & uns4(i1,i2,i3,kd)+ty(i1,i2,i3)*unt4(i1,i2,i3,kd)
        unz43(i1,i2,i3,kd)=rz(i1,i2,i3)*unr4(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & uns4(i1,i2,i3,kd)+tz(i1,i2,i3)*unt4(i1,i2,i3,kd)
        unxx41(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*unrr4(i1,i2,i3,kd)+(
     & rxx42(i1,i2,i3))*unr4(i1,i2,i3,kd)
        unyy41(i1,i2,i3,kd)=0
        unxy41(i1,i2,i3,kd)=0
        unxz41(i1,i2,i3,kd)=0
        unyz41(i1,i2,i3,kd)=0
        unzz41(i1,i2,i3,kd)=0
        unlaplacian41(i1,i2,i3,kd)=unxx41(i1,i2,i3,kd)
        unxx42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*unrr4(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*unrs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2)
     & *unss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3))*unr4(i1,i2,i3,kd)+(sxx42(
     & i1,i2,i3))*uns4(i1,i2,i3,kd)
        unyy42(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*unrr4(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*unrs4(i1,i2,i3,kd)+(sy(i1,i2,i3)**2)
     & *unss4(i1,i2,i3,kd)+(ryy42(i1,i2,i3))*unr4(i1,i2,i3,kd)+(syy42(
     & i1,i2,i3))*uns4(i1,i2,i3,kd)
        unxy42(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*unrr4(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & unrs4(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*unss4(i1,i2,i3,kd)
     & +rxy42(i1,i2,i3)*unr4(i1,i2,i3,kd)+sxy42(i1,i2,i3)*uns4(i1,i2,
     & i3,kd)
        unxz42(i1,i2,i3,kd)=0
        unyz42(i1,i2,i3,kd)=0
        unzz42(i1,i2,i3,kd)=0
        unlaplacian42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & unrr4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*
     & sy(i1,i2,i3))*unrs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)*
     & *2)*unss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3)+ryy42(i1,i2,i3))*unr4(
     & i1,i2,i3,kd)+(sxx42(i1,i2,i3)+syy42(i1,i2,i3))*uns4(i1,i2,i3,
     & kd)
        unxx43(i1,i2,i3,kd)=rx(i1,i2,i3)**2*unrr4(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*unss4(i1,i2,i3,kd)+tx(i1,i2,i3)**2*untt4(i1,i2,i3,kd)
     & +2.*rx(i1,i2,i3)*sx(i1,i2,i3)*unrs4(i1,i2,i3,kd)+2.*rx(i1,i2,
     & i3)*tx(i1,i2,i3)*unrt4(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,i2,
     & i3)*unst4(i1,i2,i3,kd)+rxx43(i1,i2,i3)*unr4(i1,i2,i3,kd)+sxx43(
     & i1,i2,i3)*uns4(i1,i2,i3,kd)+txx43(i1,i2,i3)*unt4(i1,i2,i3,kd)
        unyy43(i1,i2,i3,kd)=ry(i1,i2,i3)**2*unrr4(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*unss4(i1,i2,i3,kd)+ty(i1,i2,i3)**2*untt4(i1,i2,i3,kd)
     & +2.*ry(i1,i2,i3)*sy(i1,i2,i3)*unrs4(i1,i2,i3,kd)+2.*ry(i1,i2,
     & i3)*ty(i1,i2,i3)*unrt4(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,i2,
     & i3)*unst4(i1,i2,i3,kd)+ryy43(i1,i2,i3)*unr4(i1,i2,i3,kd)+syy43(
     & i1,i2,i3)*uns4(i1,i2,i3,kd)+tyy43(i1,i2,i3)*unt4(i1,i2,i3,kd)
        unzz43(i1,i2,i3,kd)=rz(i1,i2,i3)**2*unrr4(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*unss4(i1,i2,i3,kd)+tz(i1,i2,i3)**2*untt4(i1,i2,i3,kd)
     & +2.*rz(i1,i2,i3)*sz(i1,i2,i3)*unrs4(i1,i2,i3,kd)+2.*rz(i1,i2,
     & i3)*tz(i1,i2,i3)*unrt4(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,i2,
     & i3)*unst4(i1,i2,i3,kd)+rzz43(i1,i2,i3)*unr4(i1,i2,i3,kd)+szz43(
     & i1,i2,i3)*uns4(i1,i2,i3,kd)+tzz43(i1,i2,i3)*unt4(i1,i2,i3,kd)
        unxy43(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*unrr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*unss4(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & ty(i1,i2,i3)*untt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(
     & i1,i2,i3)*sx(i1,i2,i3))*unrs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(i1,
     & i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*unrt4(i1,i2,i3,kd)+(sx(i1,i2,
     & i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*unst4(i1,i2,i3,kd)+
     & rxy43(i1,i2,i3)*unr4(i1,i2,i3,kd)+sxy43(i1,i2,i3)*uns4(i1,i2,
     & i3,kd)+txy43(i1,i2,i3)*unt4(i1,i2,i3,kd)
        unxz43(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*unrr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*unss4(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & tz(i1,i2,i3)*untt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sx(i1,i2,i3))*unrs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(i1,
     & i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*unrt4(i1,i2,i3,kd)+(sx(i1,i2,
     & i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*unst4(i1,i2,i3,kd)+
     & rxz43(i1,i2,i3)*unr4(i1,i2,i3,kd)+sxz43(i1,i2,i3)*uns4(i1,i2,
     & i3,kd)+txz43(i1,i2,i3)*unt4(i1,i2,i3,kd)
        unyz43(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*unrr4(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*unss4(i1,i2,i3,kd)+ty(i1,i2,i3)*
     & tz(i1,i2,i3)*untt4(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sy(i1,i2,i3))*unrs4(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(i1,
     & i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*unrt4(i1,i2,i3,kd)+(sy(i1,i2,
     & i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*unst4(i1,i2,i3,kd)+
     & ryz43(i1,i2,i3)*unr4(i1,i2,i3,kd)+syz43(i1,i2,i3)*uns4(i1,i2,
     & i3,kd)+tyz43(i1,i2,i3)*unt4(i1,i2,i3,kd)
        unlaplacian43(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*unrr4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)*
     & *2+sz(i1,i2,i3)**2)*unss4(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(i1,
     & i2,i3)**2+tz(i1,i2,i3)**2)*untt4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*
     & sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,
     & i3))*unrs4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(i1,
     & i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*unrt4(i1,i2,i3,
     & kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)+
     & sz(i1,i2,i3)*tz(i1,i2,i3))*unst4(i1,i2,i3,kd)+(rxx43(i1,i2,i3)+
     & ryy43(i1,i2,i3)+rzz43(i1,i2,i3))*unr4(i1,i2,i3,kd)+(sxx43(i1,
     & i2,i3)+syy43(i1,i2,i3)+szz43(i1,i2,i3))*uns4(i1,i2,i3,kd)+(
     & txx43(i1,i2,i3)+tyy43(i1,i2,i3)+tzz43(i1,i2,i3))*unt4(i1,i2,i3,
     & kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
        unx43r(i1,i2,i3,kd)=(8.*(un(i1+1,i2,i3,kd)-un(i1-1,i2,i3,kd))-(
     & un(i1+2,i2,i3,kd)-un(i1-2,i2,i3,kd)))*h41(0)
        uny43r(i1,i2,i3,kd)=(8.*(un(i1,i2+1,i3,kd)-un(i1,i2-1,i3,kd))-(
     & un(i1,i2+2,i3,kd)-un(i1,i2-2,i3,kd)))*h41(1)
        unz43r(i1,i2,i3,kd)=(8.*(un(i1,i2,i3+1,kd)-un(i1,i2,i3-1,kd))-(
     & un(i1,i2,i3+2,kd)-un(i1,i2,i3-2,kd)))*h41(2)
        unxx43r(i1,i2,i3,kd)=( -30.*un(i1,i2,i3,kd)+16.*(un(i1+1,i2,i3,
     & kd)+un(i1-1,i2,i3,kd))-(un(i1+2,i2,i3,kd)+un(i1-2,i2,i3,kd)) )*
     & h42(0)
        unyy43r(i1,i2,i3,kd)=( -30.*un(i1,i2,i3,kd)+16.*(un(i1,i2+1,i3,
     & kd)+un(i1,i2-1,i3,kd))-(un(i1,i2+2,i3,kd)+un(i1,i2-2,i3,kd)) )*
     & h42(1)
        unzz43r(i1,i2,i3,kd)=( -30.*un(i1,i2,i3,kd)+16.*(un(i1,i2,i3+1,
     & kd)+un(i1,i2,i3-1,kd))-(un(i1,i2,i3+2,kd)+un(i1,i2,i3-2,kd)) )*
     & h42(2)
        unxy43r(i1,i2,i3,kd)=( (un(i1+2,i2+2,i3,kd)-un(i1-2,i2+2,i3,kd)
     & - un(i1+2,i2-2,i3,kd)+un(i1-2,i2-2,i3,kd)) +8.*(un(i1-1,i2+2,
     & i3,kd)-un(i1-1,i2-2,i3,kd)-un(i1+1,i2+2,i3,kd)+un(i1+1,i2-2,i3,
     & kd) +un(i1+2,i2-1,i3,kd)-un(i1-2,i2-1,i3,kd)-un(i1+2,i2+1,i3,
     & kd)+un(i1-2,i2+1,i3,kd))+64.*(un(i1+1,i2+1,i3,kd)-un(i1-1,i2+1,
     & i3,kd)- un(i1+1,i2-1,i3,kd)+un(i1-1,i2-1,i3,kd)))*(h41(0)*h41(
     & 1))
        unxz43r(i1,i2,i3,kd)=( (un(i1+2,i2,i3+2,kd)-un(i1-2,i2,i3+2,kd)
     & -un(i1+2,i2,i3-2,kd)+un(i1-2,i2,i3-2,kd)) +8.*(un(i1-1,i2,i3+2,
     & kd)-un(i1-1,i2,i3-2,kd)-un(i1+1,i2,i3+2,kd)+un(i1+1,i2,i3-2,kd)
     &  +un(i1+2,i2,i3-1,kd)-un(i1-2,i2,i3-1,kd)- un(i1+2,i2,i3+1,kd)+
     & un(i1-2,i2,i3+1,kd)) +64.*(un(i1+1,i2,i3+1,kd)-un(i1-1,i2,i3+1,
     & kd)-un(i1+1,i2,i3-1,kd)+un(i1-1,i2,i3-1,kd)) )*(h41(0)*h41(2))
        unyz43r(i1,i2,i3,kd)=( (un(i1,i2+2,i3+2,kd)-un(i1,i2-2,i3+2,kd)
     & -un(i1,i2+2,i3-2,kd)+un(i1,i2-2,i3-2,kd)) +8.*(un(i1,i2-1,i3+2,
     & kd)-un(i1,i2-1,i3-2,kd)-un(i1,i2+1,i3+2,kd)+un(i1,i2+1,i3-2,kd)
     &  +un(i1,i2+2,i3-1,kd)-un(i1,i2-2,i3-1,kd)-un(i1,i2+2,i3+1,kd)+
     & un(i1,i2-2,i3+1,kd)) +64.*(un(i1,i2+1,i3+1,kd)-un(i1,i2-1,i3+1,
     & kd)-un(i1,i2+1,i3-1,kd)+un(i1,i2-1,i3-1,kd)) )*(h41(1)*h41(2))
        unx41r(i1,i2,i3,kd)= unx43r(i1,i2,i3,kd)
        uny41r(i1,i2,i3,kd)= uny43r(i1,i2,i3,kd)
        unz41r(i1,i2,i3,kd)= unz43r(i1,i2,i3,kd)
        unxx41r(i1,i2,i3,kd)= unxx43r(i1,i2,i3,kd)
        unyy41r(i1,i2,i3,kd)= unyy43r(i1,i2,i3,kd)
        unzz41r(i1,i2,i3,kd)= unzz43r(i1,i2,i3,kd)
        unxy41r(i1,i2,i3,kd)= unxy43r(i1,i2,i3,kd)
        unxz41r(i1,i2,i3,kd)= unxz43r(i1,i2,i3,kd)
        unyz41r(i1,i2,i3,kd)= unyz43r(i1,i2,i3,kd)
        unlaplacian41r(i1,i2,i3,kd)=unxx43r(i1,i2,i3,kd)
        unx42r(i1,i2,i3,kd)= unx43r(i1,i2,i3,kd)
        uny42r(i1,i2,i3,kd)= uny43r(i1,i2,i3,kd)
        unz42r(i1,i2,i3,kd)= unz43r(i1,i2,i3,kd)
        unxx42r(i1,i2,i3,kd)= unxx43r(i1,i2,i3,kd)
        unyy42r(i1,i2,i3,kd)= unyy43r(i1,i2,i3,kd)
        unzz42r(i1,i2,i3,kd)= unzz43r(i1,i2,i3,kd)
        unxy42r(i1,i2,i3,kd)= unxy43r(i1,i2,i3,kd)
        unxz42r(i1,i2,i3,kd)= unxz43r(i1,i2,i3,kd)
        unyz42r(i1,i2,i3,kd)= unyz43r(i1,i2,i3,kd)
        unlaplacian42r(i1,i2,i3,kd)=unxx43r(i1,i2,i3,kd)+unyy43r(i1,i2,
     & i3,kd)
        unlaplacian43r(i1,i2,i3,kd)=unxx43r(i1,i2,i3,kd)+unyy43r(i1,i2,
     & i3,kd)+unzz43r(i1,i2,i3,kd)
        vr2(i1,i2,i3,kd)=(v(i1+1,i2,i3,kd)-v(i1-1,i2,i3,kd))*d12(0)
        vs2(i1,i2,i3,kd)=(v(i1,i2+1,i3,kd)-v(i1,i2-1,i3,kd))*d12(1)
        vt2(i1,i2,i3,kd)=(v(i1,i2,i3+1,kd)-v(i1,i2,i3-1,kd))*d12(2)
        vrr2(i1,i2,i3,kd)=(-2.*v(i1,i2,i3,kd)+(v(i1+1,i2,i3,kd)+v(i1-1,
     & i2,i3,kd)) )*d22(0)
        vss2(i1,i2,i3,kd)=(-2.*v(i1,i2,i3,kd)+(v(i1,i2+1,i3,kd)+v(i1,
     & i2-1,i3,kd)) )*d22(1)
        vrs2(i1,i2,i3,kd)=(vr2(i1,i2+1,i3,kd)-vr2(i1,i2-1,i3,kd))*d12(
     & 1)
        vtt2(i1,i2,i3,kd)=(-2.*v(i1,i2,i3,kd)+(v(i1,i2,i3+1,kd)+v(i1,
     & i2,i3-1,kd)) )*d22(2)
        vrt2(i1,i2,i3,kd)=(vr2(i1,i2,i3+1,kd)-vr2(i1,i2,i3-1,kd))*d12(
     & 2)
        vst2(i1,i2,i3,kd)=(vs2(i1,i2,i3+1,kd)-vs2(i1,i2,i3-1,kd))*d12(
     & 2)
        vrrr2(i1,i2,i3,kd)=(-2.*(v(i1+1,i2,i3,kd)-v(i1-1,i2,i3,kd))+(v(
     & i1+2,i2,i3,kd)-v(i1-2,i2,i3,kd)) )*d22(0)*d12(0)
        vsss2(i1,i2,i3,kd)=(-2.*(v(i1,i2+1,i3,kd)-v(i1,i2-1,i3,kd))+(v(
     & i1,i2+2,i3,kd)-v(i1,i2-2,i3,kd)) )*d22(1)*d12(1)
        vttt2(i1,i2,i3,kd)=(-2.*(v(i1,i2,i3+1,kd)-v(i1,i2,i3-1,kd))+(v(
     & i1,i2,i3+2,kd)-v(i1,i2,i3-2,kd)) )*d22(2)*d12(2)
        vx21(i1,i2,i3,kd)= rx(i1,i2,i3)*vr2(i1,i2,i3,kd)
        vy21(i1,i2,i3,kd)=0
        vz21(i1,i2,i3,kd)=0
        vx22(i1,i2,i3,kd)= rx(i1,i2,i3)*vr2(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & vs2(i1,i2,i3,kd)
        vy22(i1,i2,i3,kd)= ry(i1,i2,i3)*vr2(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & vs2(i1,i2,i3,kd)
        vz22(i1,i2,i3,kd)=0
        vx23(i1,i2,i3,kd)=rx(i1,i2,i3)*vr2(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & vs2(i1,i2,i3,kd)+tx(i1,i2,i3)*vt2(i1,i2,i3,kd)
        vy23(i1,i2,i3,kd)=ry(i1,i2,i3)*vr2(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & vs2(i1,i2,i3,kd)+ty(i1,i2,i3)*vt2(i1,i2,i3,kd)
        vz23(i1,i2,i3,kd)=rz(i1,i2,i3)*vr2(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & vs2(i1,i2,i3,kd)+tz(i1,i2,i3)*vt2(i1,i2,i3,kd)
        vxx21(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vrr2(i1,i2,i3,kd)+(rxx22(
     & i1,i2,i3))*vr2(i1,i2,i3,kd)
        vyy21(i1,i2,i3,kd)=0
        vxy21(i1,i2,i3,kd)=0
        vxz21(i1,i2,i3,kd)=0
        vyz21(i1,i2,i3,kd)=0
        vzz21(i1,i2,i3,kd)=0
        vlaplacian21(i1,i2,i3,kd)=vxx21(i1,i2,i3,kd)
        vxx22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vrr2(i1,i2,i3,kd)+2.*(rx(
     & i1,i2,i3)*sx(i1,i2,i3))*vrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2)*
     & vss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3))*vr2(i1,i2,i3,kd)+(sxx22(i1,
     & i2,i3))*vs2(i1,i2,i3,kd)
        vyy22(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*vrr2(i1,i2,i3,kd)+2.*(ry(
     & i1,i2,i3)*sy(i1,i2,i3))*vrs2(i1,i2,i3,kd)+(sy(i1,i2,i3)**2)*
     & vss2(i1,i2,i3,kd)+(ryy22(i1,i2,i3))*vr2(i1,i2,i3,kd)+(syy22(i1,
     & i2,i3))*vs2(i1,i2,i3,kd)
        vxy22(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vrr2(i1,i2,i3,kd)+
     & (rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*vrs2(i1,
     & i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vss2(i1,i2,i3,kd)+rxy22(i1,
     & i2,i3)*vr2(i1,i2,i3,kd)+sxy22(i1,i2,i3)*vs2(i1,i2,i3,kd)
        vxz22(i1,i2,i3,kd)=0
        vyz22(i1,i2,i3,kd)=0
        vzz22(i1,i2,i3,kd)=0
        vlaplacian22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & vrr2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*
     & sy(i1,i2,i3))*vrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)**
     & 2)*vss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3)+ryy22(i1,i2,i3))*vr2(i1,
     & i2,i3,kd)+(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*vs2(i1,i2,i3,kd)
        vxx23(i1,i2,i3,kd)=rx(i1,i2,i3)**2*vrr2(i1,i2,i3,kd)+sx(i1,i2,
     & i3)**2*vss2(i1,i2,i3,kd)+tx(i1,i2,i3)**2*vtt2(i1,i2,i3,kd)+2.*
     & rx(i1,i2,i3)*sx(i1,i2,i3)*vrs2(i1,i2,i3,kd)+2.*rx(i1,i2,i3)*tx(
     & i1,i2,i3)*vrt2(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,i2,i3)*vst2(
     & i1,i2,i3,kd)+rxx23(i1,i2,i3)*vr2(i1,i2,i3,kd)+sxx23(i1,i2,i3)*
     & vs2(i1,i2,i3,kd)+txx23(i1,i2,i3)*vt2(i1,i2,i3,kd)
        vyy23(i1,i2,i3,kd)=ry(i1,i2,i3)**2*vrr2(i1,i2,i3,kd)+sy(i1,i2,
     & i3)**2*vss2(i1,i2,i3,kd)+ty(i1,i2,i3)**2*vtt2(i1,i2,i3,kd)+2.*
     & ry(i1,i2,i3)*sy(i1,i2,i3)*vrs2(i1,i2,i3,kd)+2.*ry(i1,i2,i3)*ty(
     & i1,i2,i3)*vrt2(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,i2,i3)*vst2(
     & i1,i2,i3,kd)+ryy23(i1,i2,i3)*vr2(i1,i2,i3,kd)+syy23(i1,i2,i3)*
     & vs2(i1,i2,i3,kd)+tyy23(i1,i2,i3)*vt2(i1,i2,i3,kd)
        vzz23(i1,i2,i3,kd)=rz(i1,i2,i3)**2*vrr2(i1,i2,i3,kd)+sz(i1,i2,
     & i3)**2*vss2(i1,i2,i3,kd)+tz(i1,i2,i3)**2*vtt2(i1,i2,i3,kd)+2.*
     & rz(i1,i2,i3)*sz(i1,i2,i3)*vrs2(i1,i2,i3,kd)+2.*rz(i1,i2,i3)*tz(
     & i1,i2,i3)*vrt2(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,i2,i3)*vst2(
     & i1,i2,i3,kd)+rzz23(i1,i2,i3)*vr2(i1,i2,i3,kd)+szz23(i1,i2,i3)*
     & vs2(i1,i2,i3,kd)+tzz23(i1,i2,i3)*vt2(i1,i2,i3,kd)
        vxy23(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vrr2(i1,i2,i3,kd)+
     & sx(i1,i2,i3)*sy(i1,i2,i3)*vss2(i1,i2,i3,kd)+tx(i1,i2,i3)*ty(i1,
     & i2,i3)*vtt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,
     & i3)*sx(i1,i2,i3))*vrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(i1,i2,i3)+
     & ry(i1,i2,i3)*tx(i1,i2,i3))*vrt2(i1,i2,i3,kd)+(sx(i1,i2,i3)*ty(
     & i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*vst2(i1,i2,i3,kd)+rxy23(
     & i1,i2,i3)*vr2(i1,i2,i3,kd)+sxy23(i1,i2,i3)*vs2(i1,i2,i3,kd)+
     & txy23(i1,i2,i3)*vt2(i1,i2,i3,kd)
        vxz23(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*vrr2(i1,i2,i3,kd)+
     & sx(i1,i2,i3)*sz(i1,i2,i3)*vss2(i1,i2,i3,kd)+tx(i1,i2,i3)*tz(i1,
     & i2,i3)*vtt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,i2,
     & i3)*sx(i1,i2,i3))*vrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(i1,i2,i3)+
     & rz(i1,i2,i3)*tx(i1,i2,i3))*vrt2(i1,i2,i3,kd)+(sx(i1,i2,i3)*tz(
     & i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*vst2(i1,i2,i3,kd)+rxz23(
     & i1,i2,i3)*vr2(i1,i2,i3,kd)+sxz23(i1,i2,i3)*vs2(i1,i2,i3,kd)+
     & txz23(i1,i2,i3)*vt2(i1,i2,i3,kd)
        vyz23(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*vrr2(i1,i2,i3,kd)+
     & sy(i1,i2,i3)*sz(i1,i2,i3)*vss2(i1,i2,i3,kd)+ty(i1,i2,i3)*tz(i1,
     & i2,i3)*vtt2(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,i2,
     & i3)*sy(i1,i2,i3))*vrs2(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(i1,i2,i3)+
     & rz(i1,i2,i3)*ty(i1,i2,i3))*vrt2(i1,i2,i3,kd)+(sy(i1,i2,i3)*tz(
     & i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*vst2(i1,i2,i3,kd)+ryz23(
     & i1,i2,i3)*vr2(i1,i2,i3,kd)+syz23(i1,i2,i3)*vs2(i1,i2,i3,kd)+
     & tyz23(i1,i2,i3)*vt2(i1,i2,i3,kd)
        vlaplacian23(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*vrr2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)**
     & 2+sz(i1,i2,i3)**2)*vss2(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(i1,i2,
     & i3)**2+tz(i1,i2,i3)**2)*vtt2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(
     & i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,i3))
     & *vrs2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(i1,i2,i3)*
     & ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*vrt2(i1,i2,i3,kd)+2.*(
     & sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)+sz(i1,i2,
     & i3)*tz(i1,i2,i3))*vst2(i1,i2,i3,kd)+(rxx23(i1,i2,i3)+ryy23(i1,
     & i2,i3)+rzz23(i1,i2,i3))*vr2(i1,i2,i3,kd)+(sxx23(i1,i2,i3)+
     & syy23(i1,i2,i3)+szz23(i1,i2,i3))*vs2(i1,i2,i3,kd)+(txx23(i1,i2,
     & i3)+tyy23(i1,i2,i3)+tzz23(i1,i2,i3))*vt2(i1,i2,i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
        vx23r(i1,i2,i3,kd)=(v(i1+1,i2,i3,kd)-v(i1-1,i2,i3,kd))*h12(0)
        vy23r(i1,i2,i3,kd)=(v(i1,i2+1,i3,kd)-v(i1,i2-1,i3,kd))*h12(1)
        vz23r(i1,i2,i3,kd)=(v(i1,i2,i3+1,kd)-v(i1,i2,i3-1,kd))*h12(2)
        vxx23r(i1,i2,i3,kd)=(-2.*v(i1,i2,i3,kd)+(v(i1+1,i2,i3,kd)+v(i1-
     & 1,i2,i3,kd)) )*h22(0)
        vyy23r(i1,i2,i3,kd)=(-2.*v(i1,i2,i3,kd)+(v(i1,i2+1,i3,kd)+v(i1,
     & i2-1,i3,kd)) )*h22(1)
        vxy23r(i1,i2,i3,kd)=(vx23r(i1,i2+1,i3,kd)-vx23r(i1,i2-1,i3,kd))
     & *h12(1)
        vzz23r(i1,i2,i3,kd)=(-2.*v(i1,i2,i3,kd)+(v(i1,i2,i3+1,kd)+v(i1,
     & i2,i3-1,kd)) )*h22(2)
        vxz23r(i1,i2,i3,kd)=(vx23r(i1,i2,i3+1,kd)-vx23r(i1,i2,i3-1,kd))
     & *h12(2)
        vyz23r(i1,i2,i3,kd)=(vy23r(i1,i2,i3+1,kd)-vy23r(i1,i2,i3-1,kd))
     & *h12(2)
        vx21r(i1,i2,i3,kd)= vx23r(i1,i2,i3,kd)
        vy21r(i1,i2,i3,kd)= vy23r(i1,i2,i3,kd)
        vz21r(i1,i2,i3,kd)= vz23r(i1,i2,i3,kd)
        vxx21r(i1,i2,i3,kd)= vxx23r(i1,i2,i3,kd)
        vyy21r(i1,i2,i3,kd)= vyy23r(i1,i2,i3,kd)
        vzz21r(i1,i2,i3,kd)= vzz23r(i1,i2,i3,kd)
        vxy21r(i1,i2,i3,kd)= vxy23r(i1,i2,i3,kd)
        vxz21r(i1,i2,i3,kd)= vxz23r(i1,i2,i3,kd)
        vyz21r(i1,i2,i3,kd)= vyz23r(i1,i2,i3,kd)
        vlaplacian21r(i1,i2,i3,kd)=vxx23r(i1,i2,i3,kd)
        vx22r(i1,i2,i3,kd)= vx23r(i1,i2,i3,kd)
        vy22r(i1,i2,i3,kd)= vy23r(i1,i2,i3,kd)
        vz22r(i1,i2,i3,kd)= vz23r(i1,i2,i3,kd)
        vxx22r(i1,i2,i3,kd)= vxx23r(i1,i2,i3,kd)
        vyy22r(i1,i2,i3,kd)= vyy23r(i1,i2,i3,kd)
        vzz22r(i1,i2,i3,kd)= vzz23r(i1,i2,i3,kd)
        vxy22r(i1,i2,i3,kd)= vxy23r(i1,i2,i3,kd)
        vxz22r(i1,i2,i3,kd)= vxz23r(i1,i2,i3,kd)
        vyz22r(i1,i2,i3,kd)= vyz23r(i1,i2,i3,kd)
        vlaplacian22r(i1,i2,i3,kd)=vxx23r(i1,i2,i3,kd)+vyy23r(i1,i2,i3,
     & kd)
        vlaplacian23r(i1,i2,i3,kd)=vxx23r(i1,i2,i3,kd)+vyy23r(i1,i2,i3,
     & kd)+vzz23r(i1,i2,i3,kd)
        vxxx22r(i1,i2,i3,kd)=(-2.*(v(i1+1,i2,i3,kd)-v(i1-1,i2,i3,kd))+(
     & v(i1+2,i2,i3,kd)-v(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
        vyyy22r(i1,i2,i3,kd)=(-2.*(v(i1,i2+1,i3,kd)-v(i1,i2-1,i3,kd))+(
     & v(i1,i2+2,i3,kd)-v(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
        vxxy22r(i1,i2,i3,kd)=( vxx22r(i1,i2+1,i3,kd)-vxx22r(i1,i2-1,i3,
     & kd))/(2.*dx(1))
        vxyy22r(i1,i2,i3,kd)=( vyy22r(i1+1,i2,i3,kd)-vyy22r(i1-1,i2,i3,
     & kd))/(2.*dx(0))
        vxxxx22r(i1,i2,i3,kd)=(6.*v(i1,i2,i3,kd)-4.*(v(i1+1,i2,i3,kd)+
     & v(i1-1,i2,i3,kd))+(v(i1+2,i2,i3,kd)+v(i1-2,i2,i3,kd)) )/(dx(0)*
     & *4)
        vyyyy22r(i1,i2,i3,kd)=(6.*v(i1,i2,i3,kd)-4.*(v(i1,i2+1,i3,kd)+
     & v(i1,i2-1,i3,kd))+(v(i1,i2+2,i3,kd)+v(i1,i2-2,i3,kd)) )/(dx(1)*
     & *4)
        vxxyy22r(i1,i2,i3,kd)=( 4.*v(i1,i2,i3,kd)     -2.*(v(i1+1,i2,
     & i3,kd)+v(i1-1,i2,i3,kd)+v(i1,i2+1,i3,kd)+v(i1,i2-1,i3,kd))   + 
     &   (v(i1+1,i2+1,i3,kd)+v(i1-1,i2+1,i3,kd)+v(i1+1,i2-1,i3,kd)+v(
     & i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
        ! 2D laplacian squared = v.xxxx + 2 v.xxyy + v.yyyy
        vLapSq22r(i1,i2,i3,kd)= ( 6.*v(i1,i2,i3,kd)   - 4.*(v(i1+1,i2,
     & i3,kd)+v(i1-1,i2,i3,kd))    +(v(i1+2,i2,i3,kd)+v(i1-2,i2,i3,kd)
     & ) )/(dx(0)**4) +( 6.*v(i1,i2,i3,kd)    -4.*(v(i1,i2+1,i3,kd)+v(
     & i1,i2-1,i3,kd))    +(v(i1,i2+2,i3,kd)+v(i1,i2-2,i3,kd)) )/(dx(
     & 1)**4)  +( 8.*v(i1,i2,i3,kd)     -4.*(v(i1+1,i2,i3,kd)+v(i1-1,
     & i2,i3,kd)+v(i1,i2+1,i3,kd)+v(i1,i2-1,i3,kd))   +2.*(v(i1+1,i2+
     & 1,i3,kd)+v(i1-1,i2+1,i3,kd)+v(i1+1,i2-1,i3,kd)+v(i1-1,i2-1,i3,
     & kd)) )/(dx(0)**2*dx(1)**2)
        vxxx23r(i1,i2,i3,kd)=(-2.*(v(i1+1,i2,i3,kd)-v(i1-1,i2,i3,kd))+(
     & v(i1+2,i2,i3,kd)-v(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
        vyyy23r(i1,i2,i3,kd)=(-2.*(v(i1,i2+1,i3,kd)-v(i1,i2-1,i3,kd))+(
     & v(i1,i2+2,i3,kd)-v(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
        vzzz23r(i1,i2,i3,kd)=(-2.*(v(i1,i2,i3+1,kd)-v(i1,i2,i3-1,kd))+(
     & v(i1,i2,i3+2,kd)-v(i1,i2,i3-2,kd)) )*h22(1)*h12(2)
        vxxy23r(i1,i2,i3,kd)=( vxx22r(i1,i2+1,i3,kd)-vxx22r(i1,i2-1,i3,
     & kd))/(2.*dx(1))
        vxyy23r(i1,i2,i3,kd)=( vyy22r(i1+1,i2,i3,kd)-vyy22r(i1-1,i2,i3,
     & kd))/(2.*dx(0))
        vxxz23r(i1,i2,i3,kd)=( vxx22r(i1,i2,i3+1,kd)-vxx22r(i1,i2,i3-1,
     & kd))/(2.*dx(2))
        vyyz23r(i1,i2,i3,kd)=( vyy22r(i1,i2,i3+1,kd)-vyy22r(i1,i2,i3-1,
     & kd))/(2.*dx(2))
        vxzz23r(i1,i2,i3,kd)=( vzz22r(i1+1,i2,i3,kd)-vzz22r(i1-1,i2,i3,
     & kd))/(2.*dx(0))
        vyzz23r(i1,i2,i3,kd)=( vzz22r(i1,i2+1,i3,kd)-vzz22r(i1,i2-1,i3,
     & kd))/(2.*dx(1))
        vxxxx23r(i1,i2,i3,kd)=(6.*v(i1,i2,i3,kd)-4.*(v(i1+1,i2,i3,kd)+
     & v(i1-1,i2,i3,kd))+(v(i1+2,i2,i3,kd)+v(i1-2,i2,i3,kd)) )/(dx(0)*
     & *4)
        vyyyy23r(i1,i2,i3,kd)=(6.*v(i1,i2,i3,kd)-4.*(v(i1,i2+1,i3,kd)+
     & v(i1,i2-1,i3,kd))+(v(i1,i2+2,i3,kd)+v(i1,i2-2,i3,kd)) )/(dx(1)*
     & *4)
        vzzzz23r(i1,i2,i3,kd)=(6.*v(i1,i2,i3,kd)-4.*(v(i1,i2,i3+1,kd)+
     & v(i1,i2,i3-1,kd))+(v(i1,i2,i3+2,kd)+v(i1,i2,i3-2,kd)) )/(dx(2)*
     & *4)
        vxxyy23r(i1,i2,i3,kd)=( 4.*v(i1,i2,i3,kd)     -2.*(v(i1+1,i2,
     & i3,kd)+v(i1-1,i2,i3,kd)+v(i1,i2+1,i3,kd)+v(i1,i2-1,i3,kd))   + 
     &   (v(i1+1,i2+1,i3,kd)+v(i1-1,i2+1,i3,kd)+v(i1+1,i2-1,i3,kd)+v(
     & i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
        vxxzz23r(i1,i2,i3,kd)=( 4.*v(i1,i2,i3,kd)     -2.*(v(i1+1,i2,
     & i3,kd)+v(i1-1,i2,i3,kd)+v(i1,i2,i3+1,kd)+v(i1,i2,i3-1,kd))   + 
     &   (v(i1+1,i2,i3+1,kd)+v(i1-1,i2,i3+1,kd)+v(i1+1,i2,i3-1,kd)+v(
     & i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)
        vyyzz23r(i1,i2,i3,kd)=( 4.*v(i1,i2,i3,kd)     -2.*(v(i1,i2+1,
     & i3,kd)  +v(i1,i2-1,i3,kd)+  v(i1,i2  ,i3+1,kd)+v(i1,i2  ,i3-1,
     & kd))   +   (v(i1,i2+1,i3+1,kd)+v(i1,i2-1,i3+1,kd)+v(i1,i2+1,i3-
     & 1,kd)+v(i1,i2-1,i3-1,kd)) )/(dx(1)**2*dx(2)**2)
        ! 3D laplacian squared = v.xxxx + v.yyyy + v.zzzz + 2 (v.xxyy + v.xxzz + v.yyzz )
        vLapSq23r(i1,i2,i3,kd)= ( 6.*v(i1,i2,i3,kd)   - 4.*(v(i1+1,i2,
     & i3,kd)+v(i1-1,i2,i3,kd))    +(v(i1+2,i2,i3,kd)+v(i1-2,i2,i3,kd)
     & ) )/(dx(0)**4) +( 6.*v(i1,i2,i3,kd)    -4.*(v(i1,i2+1,i3,kd)+v(
     & i1,i2-1,i3,kd))    +(v(i1,i2+2,i3,kd)+v(i1,i2-2,i3,kd)) )/(dx(
     & 1)**4)  +( 6.*v(i1,i2,i3,kd)    -4.*(v(i1,i2,i3+1,kd)+v(i1,i2,
     & i3-1,kd))    +(v(i1,i2,i3+2,kd)+v(i1,i2,i3-2,kd)) )/(dx(2)**4) 
     &  +( 8.*v(i1,i2,i3,kd)     -4.*(v(i1+1,i2,i3,kd)  +v(i1-1,i2,i3,
     & kd)  +v(i1  ,i2+1,i3,kd)+v(i1  ,i2-1,i3,kd))   +2.*(v(i1+1,i2+
     & 1,i3,kd)+v(i1-1,i2+1,i3,kd)+v(i1+1,i2-1,i3,kd)+v(i1-1,i2-1,i3,
     & kd)) )/(dx(0)**2*dx(1)**2)+( 8.*v(i1,i2,i3,kd)     -4.*(v(i1+1,
     & i2,i3,kd)  +v(i1-1,i2,i3,kd)  +v(i1  ,i2,i3+1,kd)+v(i1  ,i2,i3-
     & 1,kd))   +2.*(v(i1+1,i2,i3+1,kd)+v(i1-1,i2,i3+1,kd)+v(i1+1,i2,
     & i3-1,kd)+v(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)+( 8.*v(i1,
     & i2,i3,kd)     -4.*(v(i1,i2+1,i3,kd)  +v(i1,i2-1,i3,kd)  +v(i1,
     & i2  ,i3+1,kd)+v(i1,i2  ,i3-1,kd))   +2.*(v(i1,i2+1,i3+1,kd)+v(
     & i1,i2-1,i3+1,kd)+v(i1,i2+1,i3-1,kd)+v(i1,i2-1,i3-1,kd)) )/(dx(
     & 1)**2*dx(2)**2)
        vr4(i1,i2,i3,kd)=(8.*(v(i1+1,i2,i3,kd)-v(i1-1,i2,i3,kd))-(v(i1+
     & 2,i2,i3,kd)-v(i1-2,i2,i3,kd)))*d14(0)
        vs4(i1,i2,i3,kd)=(8.*(v(i1,i2+1,i3,kd)-v(i1,i2-1,i3,kd))-(v(i1,
     & i2+2,i3,kd)-v(i1,i2-2,i3,kd)))*d14(1)
        vt4(i1,i2,i3,kd)=(8.*(v(i1,i2,i3+1,kd)-v(i1,i2,i3-1,kd))-(v(i1,
     & i2,i3+2,kd)-v(i1,i2,i3-2,kd)))*d14(2)
        vrr4(i1,i2,i3,kd)=(-30.*v(i1,i2,i3,kd)+16.*(v(i1+1,i2,i3,kd)+v(
     & i1-1,i2,i3,kd))-(v(i1+2,i2,i3,kd)+v(i1-2,i2,i3,kd)) )*d24(0)
        vss4(i1,i2,i3,kd)=(-30.*v(i1,i2,i3,kd)+16.*(v(i1,i2+1,i3,kd)+v(
     & i1,i2-1,i3,kd))-(v(i1,i2+2,i3,kd)+v(i1,i2-2,i3,kd)) )*d24(1)
        vtt4(i1,i2,i3,kd)=(-30.*v(i1,i2,i3,kd)+16.*(v(i1,i2,i3+1,kd)+v(
     & i1,i2,i3-1,kd))-(v(i1,i2,i3+2,kd)+v(i1,i2,i3-2,kd)) )*d24(2)
        vrs4(i1,i2,i3,kd)=(8.*(vr4(i1,i2+1,i3,kd)-vr4(i1,i2-1,i3,kd))-(
     & vr4(i1,i2+2,i3,kd)-vr4(i1,i2-2,i3,kd)))*d14(1)
        vrt4(i1,i2,i3,kd)=(8.*(vr4(i1,i2,i3+1,kd)-vr4(i1,i2,i3-1,kd))-(
     & vr4(i1,i2,i3+2,kd)-vr4(i1,i2,i3-2,kd)))*d14(2)
        vst4(i1,i2,i3,kd)=(8.*(vs4(i1,i2,i3+1,kd)-vs4(i1,i2,i3-1,kd))-(
     & vs4(i1,i2,i3+2,kd)-vs4(i1,i2,i3-2,kd)))*d14(2)
        vx41(i1,i2,i3,kd)= rx(i1,i2,i3)*vr4(i1,i2,i3,kd)
        vy41(i1,i2,i3,kd)=0
        vz41(i1,i2,i3,kd)=0
        vx42(i1,i2,i3,kd)= rx(i1,i2,i3)*vr4(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & vs4(i1,i2,i3,kd)
        vy42(i1,i2,i3,kd)= ry(i1,i2,i3)*vr4(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & vs4(i1,i2,i3,kd)
        vz42(i1,i2,i3,kd)=0
        vx43(i1,i2,i3,kd)=rx(i1,i2,i3)*vr4(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & vs4(i1,i2,i3,kd)+tx(i1,i2,i3)*vt4(i1,i2,i3,kd)
        vy43(i1,i2,i3,kd)=ry(i1,i2,i3)*vr4(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & vs4(i1,i2,i3,kd)+ty(i1,i2,i3)*vt4(i1,i2,i3,kd)
        vz43(i1,i2,i3,kd)=rz(i1,i2,i3)*vr4(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & vs4(i1,i2,i3,kd)+tz(i1,i2,i3)*vt4(i1,i2,i3,kd)
        vxx41(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vrr4(i1,i2,i3,kd)+(rxx42(
     & i1,i2,i3))*vr4(i1,i2,i3,kd)
        vyy41(i1,i2,i3,kd)=0
        vxy41(i1,i2,i3,kd)=0
        vxz41(i1,i2,i3,kd)=0
        vyz41(i1,i2,i3,kd)=0
        vzz41(i1,i2,i3,kd)=0
        vlaplacian41(i1,i2,i3,kd)=vxx41(i1,i2,i3,kd)
        vxx42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vrr4(i1,i2,i3,kd)+2.*(rx(
     & i1,i2,i3)*sx(i1,i2,i3))*vrs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2)*
     & vss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3))*vr4(i1,i2,i3,kd)+(sxx42(i1,
     & i2,i3))*vs4(i1,i2,i3,kd)
        vyy42(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*vrr4(i1,i2,i3,kd)+2.*(ry(
     & i1,i2,i3)*sy(i1,i2,i3))*vrs4(i1,i2,i3,kd)+(sy(i1,i2,i3)**2)*
     & vss4(i1,i2,i3,kd)+(ryy42(i1,i2,i3))*vr4(i1,i2,i3,kd)+(syy42(i1,
     & i2,i3))*vs4(i1,i2,i3,kd)
        vxy42(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vrr4(i1,i2,i3,kd)+
     & (rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*vrs4(i1,
     & i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vss4(i1,i2,i3,kd)+rxy42(i1,
     & i2,i3)*vr4(i1,i2,i3,kd)+sxy42(i1,i2,i3)*vs4(i1,i2,i3,kd)
        vxz42(i1,i2,i3,kd)=0
        vyz42(i1,i2,i3,kd)=0
        vzz42(i1,i2,i3,kd)=0
        vlaplacian42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & vrr4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*
     & sy(i1,i2,i3))*vrs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)**
     & 2)*vss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3)+ryy42(i1,i2,i3))*vr4(i1,
     & i2,i3,kd)+(sxx42(i1,i2,i3)+syy42(i1,i2,i3))*vs4(i1,i2,i3,kd)
        vxx43(i1,i2,i3,kd)=rx(i1,i2,i3)**2*vrr4(i1,i2,i3,kd)+sx(i1,i2,
     & i3)**2*vss4(i1,i2,i3,kd)+tx(i1,i2,i3)**2*vtt4(i1,i2,i3,kd)+2.*
     & rx(i1,i2,i3)*sx(i1,i2,i3)*vrs4(i1,i2,i3,kd)+2.*rx(i1,i2,i3)*tx(
     & i1,i2,i3)*vrt4(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,i2,i3)*vst4(
     & i1,i2,i3,kd)+rxx43(i1,i2,i3)*vr4(i1,i2,i3,kd)+sxx43(i1,i2,i3)*
     & vs4(i1,i2,i3,kd)+txx43(i1,i2,i3)*vt4(i1,i2,i3,kd)
        vyy43(i1,i2,i3,kd)=ry(i1,i2,i3)**2*vrr4(i1,i2,i3,kd)+sy(i1,i2,
     & i3)**2*vss4(i1,i2,i3,kd)+ty(i1,i2,i3)**2*vtt4(i1,i2,i3,kd)+2.*
     & ry(i1,i2,i3)*sy(i1,i2,i3)*vrs4(i1,i2,i3,kd)+2.*ry(i1,i2,i3)*ty(
     & i1,i2,i3)*vrt4(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,i2,i3)*vst4(
     & i1,i2,i3,kd)+ryy43(i1,i2,i3)*vr4(i1,i2,i3,kd)+syy43(i1,i2,i3)*
     & vs4(i1,i2,i3,kd)+tyy43(i1,i2,i3)*vt4(i1,i2,i3,kd)
        vzz43(i1,i2,i3,kd)=rz(i1,i2,i3)**2*vrr4(i1,i2,i3,kd)+sz(i1,i2,
     & i3)**2*vss4(i1,i2,i3,kd)+tz(i1,i2,i3)**2*vtt4(i1,i2,i3,kd)+2.*
     & rz(i1,i2,i3)*sz(i1,i2,i3)*vrs4(i1,i2,i3,kd)+2.*rz(i1,i2,i3)*tz(
     & i1,i2,i3)*vrt4(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,i2,i3)*vst4(
     & i1,i2,i3,kd)+rzz43(i1,i2,i3)*vr4(i1,i2,i3,kd)+szz43(i1,i2,i3)*
     & vs4(i1,i2,i3,kd)+tzz43(i1,i2,i3)*vt4(i1,i2,i3,kd)
        vxy43(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vrr4(i1,i2,i3,kd)+
     & sx(i1,i2,i3)*sy(i1,i2,i3)*vss4(i1,i2,i3,kd)+tx(i1,i2,i3)*ty(i1,
     & i2,i3)*vtt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,
     & i3)*sx(i1,i2,i3))*vrs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(i1,i2,i3)+
     & ry(i1,i2,i3)*tx(i1,i2,i3))*vrt4(i1,i2,i3,kd)+(sx(i1,i2,i3)*ty(
     & i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*vst4(i1,i2,i3,kd)+rxy43(
     & i1,i2,i3)*vr4(i1,i2,i3,kd)+sxy43(i1,i2,i3)*vs4(i1,i2,i3,kd)+
     & txy43(i1,i2,i3)*vt4(i1,i2,i3,kd)
        vxz43(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*vrr4(i1,i2,i3,kd)+
     & sx(i1,i2,i3)*sz(i1,i2,i3)*vss4(i1,i2,i3,kd)+tx(i1,i2,i3)*tz(i1,
     & i2,i3)*vtt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,i2,
     & i3)*sx(i1,i2,i3))*vrs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(i1,i2,i3)+
     & rz(i1,i2,i3)*tx(i1,i2,i3))*vrt4(i1,i2,i3,kd)+(sx(i1,i2,i3)*tz(
     & i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*vst4(i1,i2,i3,kd)+rxz43(
     & i1,i2,i3)*vr4(i1,i2,i3,kd)+sxz43(i1,i2,i3)*vs4(i1,i2,i3,kd)+
     & txz43(i1,i2,i3)*vt4(i1,i2,i3,kd)
        vyz43(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*vrr4(i1,i2,i3,kd)+
     & sy(i1,i2,i3)*sz(i1,i2,i3)*vss4(i1,i2,i3,kd)+ty(i1,i2,i3)*tz(i1,
     & i2,i3)*vtt4(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,i2,
     & i3)*sy(i1,i2,i3))*vrs4(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(i1,i2,i3)+
     & rz(i1,i2,i3)*ty(i1,i2,i3))*vrt4(i1,i2,i3,kd)+(sy(i1,i2,i3)*tz(
     & i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*vst4(i1,i2,i3,kd)+ryz43(
     & i1,i2,i3)*vr4(i1,i2,i3,kd)+syz43(i1,i2,i3)*vs4(i1,i2,i3,kd)+
     & tyz43(i1,i2,i3)*vt4(i1,i2,i3,kd)
        vlaplacian43(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*vrr4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)**
     & 2+sz(i1,i2,i3)**2)*vss4(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(i1,i2,
     & i3)**2+tz(i1,i2,i3)**2)*vtt4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(
     & i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,i3))
     & *vrs4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(i1,i2,i3)*
     & ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*vrt4(i1,i2,i3,kd)+2.*(
     & sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)+sz(i1,i2,
     & i3)*tz(i1,i2,i3))*vst4(i1,i2,i3,kd)+(rxx43(i1,i2,i3)+ryy43(i1,
     & i2,i3)+rzz43(i1,i2,i3))*vr4(i1,i2,i3,kd)+(sxx43(i1,i2,i3)+
     & syy43(i1,i2,i3)+szz43(i1,i2,i3))*vs4(i1,i2,i3,kd)+(txx43(i1,i2,
     & i3)+tyy43(i1,i2,i3)+tzz43(i1,i2,i3))*vt4(i1,i2,i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
        vx43r(i1,i2,i3,kd)=(8.*(v(i1+1,i2,i3,kd)-v(i1-1,i2,i3,kd))-(v(
     & i1+2,i2,i3,kd)-v(i1-2,i2,i3,kd)))*h41(0)
        vy43r(i1,i2,i3,kd)=(8.*(v(i1,i2+1,i3,kd)-v(i1,i2-1,i3,kd))-(v(
     & i1,i2+2,i3,kd)-v(i1,i2-2,i3,kd)))*h41(1)
        vz43r(i1,i2,i3,kd)=(8.*(v(i1,i2,i3+1,kd)-v(i1,i2,i3-1,kd))-(v(
     & i1,i2,i3+2,kd)-v(i1,i2,i3-2,kd)))*h41(2)
        vxx43r(i1,i2,i3,kd)=( -30.*v(i1,i2,i3,kd)+16.*(v(i1+1,i2,i3,kd)
     & +v(i1-1,i2,i3,kd))-(v(i1+2,i2,i3,kd)+v(i1-2,i2,i3,kd)) )*h42(0)
        vyy43r(i1,i2,i3,kd)=( -30.*v(i1,i2,i3,kd)+16.*(v(i1,i2+1,i3,kd)
     & +v(i1,i2-1,i3,kd))-(v(i1,i2+2,i3,kd)+v(i1,i2-2,i3,kd)) )*h42(1)
        vzz43r(i1,i2,i3,kd)=( -30.*v(i1,i2,i3,kd)+16.*(v(i1,i2,i3+1,kd)
     & +v(i1,i2,i3-1,kd))-(v(i1,i2,i3+2,kd)+v(i1,i2,i3-2,kd)) )*h42(2)
        vxy43r(i1,i2,i3,kd)=( (v(i1+2,i2+2,i3,kd)-v(i1-2,i2+2,i3,kd)- 
     & v(i1+2,i2-2,i3,kd)+v(i1-2,i2-2,i3,kd)) +8.*(v(i1-1,i2+2,i3,kd)-
     & v(i1-1,i2-2,i3,kd)-v(i1+1,i2+2,i3,kd)+v(i1+1,i2-2,i3,kd) +v(i1+
     & 2,i2-1,i3,kd)-v(i1-2,i2-1,i3,kd)-v(i1+2,i2+1,i3,kd)+v(i1-2,i2+
     & 1,i3,kd))+64.*(v(i1+1,i2+1,i3,kd)-v(i1-1,i2+1,i3,kd)- v(i1+1,
     & i2-1,i3,kd)+v(i1-1,i2-1,i3,kd)))*(h41(0)*h41(1))
        vxz43r(i1,i2,i3,kd)=( (v(i1+2,i2,i3+2,kd)-v(i1-2,i2,i3+2,kd)-v(
     & i1+2,i2,i3-2,kd)+v(i1-2,i2,i3-2,kd)) +8.*(v(i1-1,i2,i3+2,kd)-v(
     & i1-1,i2,i3-2,kd)-v(i1+1,i2,i3+2,kd)+v(i1+1,i2,i3-2,kd) +v(i1+2,
     & i2,i3-1,kd)-v(i1-2,i2,i3-1,kd)- v(i1+2,i2,i3+1,kd)+v(i1-2,i2,
     & i3+1,kd)) +64.*(v(i1+1,i2,i3+1,kd)-v(i1-1,i2,i3+1,kd)-v(i1+1,
     & i2,i3-1,kd)+v(i1-1,i2,i3-1,kd)) )*(h41(0)*h41(2))
        vyz43r(i1,i2,i3,kd)=( (v(i1,i2+2,i3+2,kd)-v(i1,i2-2,i3+2,kd)-v(
     & i1,i2+2,i3-2,kd)+v(i1,i2-2,i3-2,kd)) +8.*(v(i1,i2-1,i3+2,kd)-v(
     & i1,i2-1,i3-2,kd)-v(i1,i2+1,i3+2,kd)+v(i1,i2+1,i3-2,kd) +v(i1,
     & i2+2,i3-1,kd)-v(i1,i2-2,i3-1,kd)-v(i1,i2+2,i3+1,kd)+v(i1,i2-2,
     & i3+1,kd)) +64.*(v(i1,i2+1,i3+1,kd)-v(i1,i2-1,i3+1,kd)-v(i1,i2+
     & 1,i3-1,kd)+v(i1,i2-1,i3-1,kd)) )*(h41(1)*h41(2))
        vx41r(i1,i2,i3,kd)= vx43r(i1,i2,i3,kd)
        vy41r(i1,i2,i3,kd)= vy43r(i1,i2,i3,kd)
        vz41r(i1,i2,i3,kd)= vz43r(i1,i2,i3,kd)
        vxx41r(i1,i2,i3,kd)= vxx43r(i1,i2,i3,kd)
        vyy41r(i1,i2,i3,kd)= vyy43r(i1,i2,i3,kd)
        vzz41r(i1,i2,i3,kd)= vzz43r(i1,i2,i3,kd)
        vxy41r(i1,i2,i3,kd)= vxy43r(i1,i2,i3,kd)
        vxz41r(i1,i2,i3,kd)= vxz43r(i1,i2,i3,kd)
        vyz41r(i1,i2,i3,kd)= vyz43r(i1,i2,i3,kd)
        vlaplacian41r(i1,i2,i3,kd)=vxx43r(i1,i2,i3,kd)
        vx42r(i1,i2,i3,kd)= vx43r(i1,i2,i3,kd)
        vy42r(i1,i2,i3,kd)= vy43r(i1,i2,i3,kd)
        vz42r(i1,i2,i3,kd)= vz43r(i1,i2,i3,kd)
        vxx42r(i1,i2,i3,kd)= vxx43r(i1,i2,i3,kd)
        vyy42r(i1,i2,i3,kd)= vyy43r(i1,i2,i3,kd)
        vzz42r(i1,i2,i3,kd)= vzz43r(i1,i2,i3,kd)
        vxy42r(i1,i2,i3,kd)= vxy43r(i1,i2,i3,kd)
        vxz42r(i1,i2,i3,kd)= vxz43r(i1,i2,i3,kd)
        vyz42r(i1,i2,i3,kd)= vyz43r(i1,i2,i3,kd)
        vlaplacian42r(i1,i2,i3,kd)=vxx43r(i1,i2,i3,kd)+vyy43r(i1,i2,i3,
     & kd)
        vlaplacian43r(i1,i2,i3,kd)=vxx43r(i1,i2,i3,kd)+vyy43r(i1,i2,i3,
     & kd)+vzz43r(i1,i2,i3,kd)
        umr2(i1,i2,i3,kd)=(um(i1+1,i2,i3,kd)-um(i1-1,i2,i3,kd))*d12(0)
        ums2(i1,i2,i3,kd)=(um(i1,i2+1,i3,kd)-um(i1,i2-1,i3,kd))*d12(1)
        umt2(i1,i2,i3,kd)=(um(i1,i2,i3+1,kd)-um(i1,i2,i3-1,kd))*d12(2)
        umrr2(i1,i2,i3,kd)=(-2.*um(i1,i2,i3,kd)+(um(i1+1,i2,i3,kd)+um(
     & i1-1,i2,i3,kd)) )*d22(0)
        umss2(i1,i2,i3,kd)=(-2.*um(i1,i2,i3,kd)+(um(i1,i2+1,i3,kd)+um(
     & i1,i2-1,i3,kd)) )*d22(1)
        umrs2(i1,i2,i3,kd)=(umr2(i1,i2+1,i3,kd)-umr2(i1,i2-1,i3,kd))*
     & d12(1)
        umtt2(i1,i2,i3,kd)=(-2.*um(i1,i2,i3,kd)+(um(i1,i2,i3+1,kd)+um(
     & i1,i2,i3-1,kd)) )*d22(2)
        umrt2(i1,i2,i3,kd)=(umr2(i1,i2,i3+1,kd)-umr2(i1,i2,i3-1,kd))*
     & d12(2)
        umst2(i1,i2,i3,kd)=(ums2(i1,i2,i3+1,kd)-ums2(i1,i2,i3-1,kd))*
     & d12(2)
        umrrr2(i1,i2,i3,kd)=(-2.*(um(i1+1,i2,i3,kd)-um(i1-1,i2,i3,kd))+
     & (um(i1+2,i2,i3,kd)-um(i1-2,i2,i3,kd)) )*d22(0)*d12(0)
        umsss2(i1,i2,i3,kd)=(-2.*(um(i1,i2+1,i3,kd)-um(i1,i2-1,i3,kd))+
     & (um(i1,i2+2,i3,kd)-um(i1,i2-2,i3,kd)) )*d22(1)*d12(1)
        umttt2(i1,i2,i3,kd)=(-2.*(um(i1,i2,i3+1,kd)-um(i1,i2,i3-1,kd))+
     & (um(i1,i2,i3+2,kd)-um(i1,i2,i3-2,kd)) )*d22(2)*d12(2)
        umx21(i1,i2,i3,kd)= rx(i1,i2,i3)*umr2(i1,i2,i3,kd)
        umy21(i1,i2,i3,kd)=0
        umz21(i1,i2,i3,kd)=0
        umx22(i1,i2,i3,kd)= rx(i1,i2,i3)*umr2(i1,i2,i3,kd)+sx(i1,i2,i3)
     & *ums2(i1,i2,i3,kd)
        umy22(i1,i2,i3,kd)= ry(i1,i2,i3)*umr2(i1,i2,i3,kd)+sy(i1,i2,i3)
     & *ums2(i1,i2,i3,kd)
        umz22(i1,i2,i3,kd)=0
        umx23(i1,i2,i3,kd)=rx(i1,i2,i3)*umr2(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & ums2(i1,i2,i3,kd)+tx(i1,i2,i3)*umt2(i1,i2,i3,kd)
        umy23(i1,i2,i3,kd)=ry(i1,i2,i3)*umr2(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & ums2(i1,i2,i3,kd)+ty(i1,i2,i3)*umt2(i1,i2,i3,kd)
        umz23(i1,i2,i3,kd)=rz(i1,i2,i3)*umr2(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & ums2(i1,i2,i3,kd)+tz(i1,i2,i3)*umt2(i1,i2,i3,kd)
        umxx21(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*umrr2(i1,i2,i3,kd)+(
     & rxx22(i1,i2,i3))*umr2(i1,i2,i3,kd)
        umyy21(i1,i2,i3,kd)=0
        umxy21(i1,i2,i3,kd)=0
        umxz21(i1,i2,i3,kd)=0
        umyz21(i1,i2,i3,kd)=0
        umzz21(i1,i2,i3,kd)=0
        umlaplacian21(i1,i2,i3,kd)=umxx21(i1,i2,i3,kd)
        umxx22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*umrr2(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*umrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2)
     & *umss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3))*umr2(i1,i2,i3,kd)+(sxx22(
     & i1,i2,i3))*ums2(i1,i2,i3,kd)
        umyy22(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*umrr2(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*umrs2(i1,i2,i3,kd)+(sy(i1,i2,i3)**2)
     & *umss2(i1,i2,i3,kd)+(ryy22(i1,i2,i3))*umr2(i1,i2,i3,kd)+(syy22(
     & i1,i2,i3))*ums2(i1,i2,i3,kd)
        umxy22(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*umrr2(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & umrs2(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*umss2(i1,i2,i3,kd)
     & +rxy22(i1,i2,i3)*umr2(i1,i2,i3,kd)+sxy22(i1,i2,i3)*ums2(i1,i2,
     & i3,kd)
        umxz22(i1,i2,i3,kd)=0
        umyz22(i1,i2,i3,kd)=0
        umzz22(i1,i2,i3,kd)=0
        umlaplacian22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & umrr2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*
     & sy(i1,i2,i3))*umrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)*
     & *2)*umss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3)+ryy22(i1,i2,i3))*umr2(
     & i1,i2,i3,kd)+(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*ums2(i1,i2,i3,
     & kd)
        umxx23(i1,i2,i3,kd)=rx(i1,i2,i3)**2*umrr2(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*umss2(i1,i2,i3,kd)+tx(i1,i2,i3)**2*umtt2(i1,i2,i3,kd)
     & +2.*rx(i1,i2,i3)*sx(i1,i2,i3)*umrs2(i1,i2,i3,kd)+2.*rx(i1,i2,
     & i3)*tx(i1,i2,i3)*umrt2(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,i2,
     & i3)*umst2(i1,i2,i3,kd)+rxx23(i1,i2,i3)*umr2(i1,i2,i3,kd)+sxx23(
     & i1,i2,i3)*ums2(i1,i2,i3,kd)+txx23(i1,i2,i3)*umt2(i1,i2,i3,kd)
        umyy23(i1,i2,i3,kd)=ry(i1,i2,i3)**2*umrr2(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*umss2(i1,i2,i3,kd)+ty(i1,i2,i3)**2*umtt2(i1,i2,i3,kd)
     & +2.*ry(i1,i2,i3)*sy(i1,i2,i3)*umrs2(i1,i2,i3,kd)+2.*ry(i1,i2,
     & i3)*ty(i1,i2,i3)*umrt2(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,i2,
     & i3)*umst2(i1,i2,i3,kd)+ryy23(i1,i2,i3)*umr2(i1,i2,i3,kd)+syy23(
     & i1,i2,i3)*ums2(i1,i2,i3,kd)+tyy23(i1,i2,i3)*umt2(i1,i2,i3,kd)
        umzz23(i1,i2,i3,kd)=rz(i1,i2,i3)**2*umrr2(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*umss2(i1,i2,i3,kd)+tz(i1,i2,i3)**2*umtt2(i1,i2,i3,kd)
     & +2.*rz(i1,i2,i3)*sz(i1,i2,i3)*umrs2(i1,i2,i3,kd)+2.*rz(i1,i2,
     & i3)*tz(i1,i2,i3)*umrt2(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,i2,
     & i3)*umst2(i1,i2,i3,kd)+rzz23(i1,i2,i3)*umr2(i1,i2,i3,kd)+szz23(
     & i1,i2,i3)*ums2(i1,i2,i3,kd)+tzz23(i1,i2,i3)*umt2(i1,i2,i3,kd)
        umxy23(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*umrr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*umss2(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & ty(i1,i2,i3)*umtt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(
     & i1,i2,i3)*sx(i1,i2,i3))*umrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(i1,
     & i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*umrt2(i1,i2,i3,kd)+(sx(i1,i2,
     & i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*umst2(i1,i2,i3,kd)+
     & rxy23(i1,i2,i3)*umr2(i1,i2,i3,kd)+sxy23(i1,i2,i3)*ums2(i1,i2,
     & i3,kd)+txy23(i1,i2,i3)*umt2(i1,i2,i3,kd)
        umxz23(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*umrr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*umss2(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & tz(i1,i2,i3)*umtt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sx(i1,i2,i3))*umrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(i1,
     & i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*umrt2(i1,i2,i3,kd)+(sx(i1,i2,
     & i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*umst2(i1,i2,i3,kd)+
     & rxz23(i1,i2,i3)*umr2(i1,i2,i3,kd)+sxz23(i1,i2,i3)*ums2(i1,i2,
     & i3,kd)+txz23(i1,i2,i3)*umt2(i1,i2,i3,kd)
        umyz23(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*umrr2(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*umss2(i1,i2,i3,kd)+ty(i1,i2,i3)*
     & tz(i1,i2,i3)*umtt2(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sy(i1,i2,i3))*umrs2(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(i1,
     & i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*umrt2(i1,i2,i3,kd)+(sy(i1,i2,
     & i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*umst2(i1,i2,i3,kd)+
     & ryz23(i1,i2,i3)*umr2(i1,i2,i3,kd)+syz23(i1,i2,i3)*ums2(i1,i2,
     & i3,kd)+tyz23(i1,i2,i3)*umt2(i1,i2,i3,kd)
        umlaplacian23(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*umrr2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)*
     & *2+sz(i1,i2,i3)**2)*umss2(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(i1,
     & i2,i3)**2+tz(i1,i2,i3)**2)*umtt2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*
     & sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,
     & i3))*umrs2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(i1,
     & i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*umrt2(i1,i2,i3,
     & kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)+
     & sz(i1,i2,i3)*tz(i1,i2,i3))*umst2(i1,i2,i3,kd)+(rxx23(i1,i2,i3)+
     & ryy23(i1,i2,i3)+rzz23(i1,i2,i3))*umr2(i1,i2,i3,kd)+(sxx23(i1,
     & i2,i3)+syy23(i1,i2,i3)+szz23(i1,i2,i3))*ums2(i1,i2,i3,kd)+(
     & txx23(i1,i2,i3)+tyy23(i1,i2,i3)+tzz23(i1,i2,i3))*umt2(i1,i2,i3,
     & kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
        umx23r(i1,i2,i3,kd)=(um(i1+1,i2,i3,kd)-um(i1-1,i2,i3,kd))*h12(
     & 0)
        umy23r(i1,i2,i3,kd)=(um(i1,i2+1,i3,kd)-um(i1,i2-1,i3,kd))*h12(
     & 1)
        umz23r(i1,i2,i3,kd)=(um(i1,i2,i3+1,kd)-um(i1,i2,i3-1,kd))*h12(
     & 2)
        umxx23r(i1,i2,i3,kd)=(-2.*um(i1,i2,i3,kd)+(um(i1+1,i2,i3,kd)+
     & um(i1-1,i2,i3,kd)) )*h22(0)
        umyy23r(i1,i2,i3,kd)=(-2.*um(i1,i2,i3,kd)+(um(i1,i2+1,i3,kd)+
     & um(i1,i2-1,i3,kd)) )*h22(1)
        umxy23r(i1,i2,i3,kd)=(umx23r(i1,i2+1,i3,kd)-umx23r(i1,i2-1,i3,
     & kd))*h12(1)
        umzz23r(i1,i2,i3,kd)=(-2.*um(i1,i2,i3,kd)+(um(i1,i2,i3+1,kd)+
     & um(i1,i2,i3-1,kd)) )*h22(2)
        umxz23r(i1,i2,i3,kd)=(umx23r(i1,i2,i3+1,kd)-umx23r(i1,i2,i3-1,
     & kd))*h12(2)
        umyz23r(i1,i2,i3,kd)=(umy23r(i1,i2,i3+1,kd)-umy23r(i1,i2,i3-1,
     & kd))*h12(2)
        umx21r(i1,i2,i3,kd)= umx23r(i1,i2,i3,kd)
        umy21r(i1,i2,i3,kd)= umy23r(i1,i2,i3,kd)
        umz21r(i1,i2,i3,kd)= umz23r(i1,i2,i3,kd)
        umxx21r(i1,i2,i3,kd)= umxx23r(i1,i2,i3,kd)
        umyy21r(i1,i2,i3,kd)= umyy23r(i1,i2,i3,kd)
        umzz21r(i1,i2,i3,kd)= umzz23r(i1,i2,i3,kd)
        umxy21r(i1,i2,i3,kd)= umxy23r(i1,i2,i3,kd)
        umxz21r(i1,i2,i3,kd)= umxz23r(i1,i2,i3,kd)
        umyz21r(i1,i2,i3,kd)= umyz23r(i1,i2,i3,kd)
        umlaplacian21r(i1,i2,i3,kd)=umxx23r(i1,i2,i3,kd)
        umx22r(i1,i2,i3,kd)= umx23r(i1,i2,i3,kd)
        umy22r(i1,i2,i3,kd)= umy23r(i1,i2,i3,kd)
        umz22r(i1,i2,i3,kd)= umz23r(i1,i2,i3,kd)
        umxx22r(i1,i2,i3,kd)= umxx23r(i1,i2,i3,kd)
        umyy22r(i1,i2,i3,kd)= umyy23r(i1,i2,i3,kd)
        umzz22r(i1,i2,i3,kd)= umzz23r(i1,i2,i3,kd)
        umxy22r(i1,i2,i3,kd)= umxy23r(i1,i2,i3,kd)
        umxz22r(i1,i2,i3,kd)= umxz23r(i1,i2,i3,kd)
        umyz22r(i1,i2,i3,kd)= umyz23r(i1,i2,i3,kd)
        umlaplacian22r(i1,i2,i3,kd)=umxx23r(i1,i2,i3,kd)+umyy23r(i1,i2,
     & i3,kd)
        umlaplacian23r(i1,i2,i3,kd)=umxx23r(i1,i2,i3,kd)+umyy23r(i1,i2,
     & i3,kd)+umzz23r(i1,i2,i3,kd)
        umxxx22r(i1,i2,i3,kd)=(-2.*(um(i1+1,i2,i3,kd)-um(i1-1,i2,i3,kd)
     & )+(um(i1+2,i2,i3,kd)-um(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
        umyyy22r(i1,i2,i3,kd)=(-2.*(um(i1,i2+1,i3,kd)-um(i1,i2-1,i3,kd)
     & )+(um(i1,i2+2,i3,kd)-um(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
        umxxy22r(i1,i2,i3,kd)=( umxx22r(i1,i2+1,i3,kd)-umxx22r(i1,i2-1,
     & i3,kd))/(2.*dx(1))
        umxyy22r(i1,i2,i3,kd)=( umyy22r(i1+1,i2,i3,kd)-umyy22r(i1-1,i2,
     & i3,kd))/(2.*dx(0))
        umxxxx22r(i1,i2,i3,kd)=(6.*um(i1,i2,i3,kd)-4.*(um(i1+1,i2,i3,
     & kd)+um(i1-1,i2,i3,kd))+(um(i1+2,i2,i3,kd)+um(i1-2,i2,i3,kd)) )
     & /(dx(0)**4)
        umyyyy22r(i1,i2,i3,kd)=(6.*um(i1,i2,i3,kd)-4.*(um(i1,i2+1,i3,
     & kd)+um(i1,i2-1,i3,kd))+(um(i1,i2+2,i3,kd)+um(i1,i2-2,i3,kd)) )
     & /(dx(1)**4)
        umxxyy22r(i1,i2,i3,kd)=( 4.*um(i1,i2,i3,kd)     -2.*(um(i1+1,
     & i2,i3,kd)+um(i1-1,i2,i3,kd)+um(i1,i2+1,i3,kd)+um(i1,i2-1,i3,kd)
     & )   +   (um(i1+1,i2+1,i3,kd)+um(i1-1,i2+1,i3,kd)+um(i1+1,i2-1,
     & i3,kd)+um(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
        ! 2D laplacian squared = um.xxxx + 2 um.xxyy + um.yyyy
        umLapSq22r(i1,i2,i3,kd)= ( 6.*um(i1,i2,i3,kd)   - 4.*(um(i1+1,
     & i2,i3,kd)+um(i1-1,i2,i3,kd))    +(um(i1+2,i2,i3,kd)+um(i1-2,i2,
     & i3,kd)) )/(dx(0)**4) +( 6.*um(i1,i2,i3,kd)    -4.*(um(i1,i2+1,
     & i3,kd)+um(i1,i2-1,i3,kd))    +(um(i1,i2+2,i3,kd)+um(i1,i2-2,i3,
     & kd)) )/(dx(1)**4)  +( 8.*um(i1,i2,i3,kd)     -4.*(um(i1+1,i2,
     & i3,kd)+um(i1-1,i2,i3,kd)+um(i1,i2+1,i3,kd)+um(i1,i2-1,i3,kd))  
     &  +2.*(um(i1+1,i2+1,i3,kd)+um(i1-1,i2+1,i3,kd)+um(i1+1,i2-1,i3,
     & kd)+um(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
        umxxx23r(i1,i2,i3,kd)=(-2.*(um(i1+1,i2,i3,kd)-um(i1-1,i2,i3,kd)
     & )+(um(i1+2,i2,i3,kd)-um(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
        umyyy23r(i1,i2,i3,kd)=(-2.*(um(i1,i2+1,i3,kd)-um(i1,i2-1,i3,kd)
     & )+(um(i1,i2+2,i3,kd)-um(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
        umzzz23r(i1,i2,i3,kd)=(-2.*(um(i1,i2,i3+1,kd)-um(i1,i2,i3-1,kd)
     & )+(um(i1,i2,i3+2,kd)-um(i1,i2,i3-2,kd)) )*h22(1)*h12(2)
        umxxy23r(i1,i2,i3,kd)=( umxx22r(i1,i2+1,i3,kd)-umxx22r(i1,i2-1,
     & i3,kd))/(2.*dx(1))
        umxyy23r(i1,i2,i3,kd)=( umyy22r(i1+1,i2,i3,kd)-umyy22r(i1-1,i2,
     & i3,kd))/(2.*dx(0))
        umxxz23r(i1,i2,i3,kd)=( umxx22r(i1,i2,i3+1,kd)-umxx22r(i1,i2,
     & i3-1,kd))/(2.*dx(2))
        umyyz23r(i1,i2,i3,kd)=( umyy22r(i1,i2,i3+1,kd)-umyy22r(i1,i2,
     & i3-1,kd))/(2.*dx(2))
        umxzz23r(i1,i2,i3,kd)=( umzz22r(i1+1,i2,i3,kd)-umzz22r(i1-1,i2,
     & i3,kd))/(2.*dx(0))
        umyzz23r(i1,i2,i3,kd)=( umzz22r(i1,i2+1,i3,kd)-umzz22r(i1,i2-1,
     & i3,kd))/(2.*dx(1))
        umxxxx23r(i1,i2,i3,kd)=(6.*um(i1,i2,i3,kd)-4.*(um(i1+1,i2,i3,
     & kd)+um(i1-1,i2,i3,kd))+(um(i1+2,i2,i3,kd)+um(i1-2,i2,i3,kd)) )
     & /(dx(0)**4)
        umyyyy23r(i1,i2,i3,kd)=(6.*um(i1,i2,i3,kd)-4.*(um(i1,i2+1,i3,
     & kd)+um(i1,i2-1,i3,kd))+(um(i1,i2+2,i3,kd)+um(i1,i2-2,i3,kd)) )
     & /(dx(1)**4)
        umzzzz23r(i1,i2,i3,kd)=(6.*um(i1,i2,i3,kd)-4.*(um(i1,i2,i3+1,
     & kd)+um(i1,i2,i3-1,kd))+(um(i1,i2,i3+2,kd)+um(i1,i2,i3-2,kd)) )
     & /(dx(2)**4)
        umxxyy23r(i1,i2,i3,kd)=( 4.*um(i1,i2,i3,kd)     -2.*(um(i1+1,
     & i2,i3,kd)+um(i1-1,i2,i3,kd)+um(i1,i2+1,i3,kd)+um(i1,i2-1,i3,kd)
     & )   +   (um(i1+1,i2+1,i3,kd)+um(i1-1,i2+1,i3,kd)+um(i1+1,i2-1,
     & i3,kd)+um(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
        umxxzz23r(i1,i2,i3,kd)=( 4.*um(i1,i2,i3,kd)     -2.*(um(i1+1,
     & i2,i3,kd)+um(i1-1,i2,i3,kd)+um(i1,i2,i3+1,kd)+um(i1,i2,i3-1,kd)
     & )   +   (um(i1+1,i2,i3+1,kd)+um(i1-1,i2,i3+1,kd)+um(i1+1,i2,i3-
     & 1,kd)+um(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)
        umyyzz23r(i1,i2,i3,kd)=( 4.*um(i1,i2,i3,kd)     -2.*(um(i1,i2+
     & 1,i3,kd)  +um(i1,i2-1,i3,kd)+  um(i1,i2  ,i3+1,kd)+um(i1,i2  ,
     & i3-1,kd))   +   (um(i1,i2+1,i3+1,kd)+um(i1,i2-1,i3+1,kd)+um(i1,
     & i2+1,i3-1,kd)+um(i1,i2-1,i3-1,kd)) )/(dx(1)**2*dx(2)**2)
        ! 3D laplacian squared = um.xxxx + um.yyyy + um.zzzz + 2 (um.xxyy + um.xxzz + um.yyzz )
        umLapSq23r(i1,i2,i3,kd)= ( 6.*um(i1,i2,i3,kd)   - 4.*(um(i1+1,
     & i2,i3,kd)+um(i1-1,i2,i3,kd))    +(um(i1+2,i2,i3,kd)+um(i1-2,i2,
     & i3,kd)) )/(dx(0)**4) +( 6.*um(i1,i2,i3,kd)    -4.*(um(i1,i2+1,
     & i3,kd)+um(i1,i2-1,i3,kd))    +(um(i1,i2+2,i3,kd)+um(i1,i2-2,i3,
     & kd)) )/(dx(1)**4)  +( 6.*um(i1,i2,i3,kd)    -4.*(um(i1,i2,i3+1,
     & kd)+um(i1,i2,i3-1,kd))    +(um(i1,i2,i3+2,kd)+um(i1,i2,i3-2,kd)
     & ) )/(dx(2)**4)  +( 8.*um(i1,i2,i3,kd)     -4.*(um(i1+1,i2,i3,
     & kd)  +um(i1-1,i2,i3,kd)  +um(i1  ,i2+1,i3,kd)+um(i1  ,i2-1,i3,
     & kd))   +2.*(um(i1+1,i2+1,i3,kd)+um(i1-1,i2+1,i3,kd)+um(i1+1,i2-
     & 1,i3,kd)+um(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)+( 8.*um(i1,
     & i2,i3,kd)     -4.*(um(i1+1,i2,i3,kd)  +um(i1-1,i2,i3,kd)  +um(
     & i1  ,i2,i3+1,kd)+um(i1  ,i2,i3-1,kd))   +2.*(um(i1+1,i2,i3+1,
     & kd)+um(i1-1,i2,i3+1,kd)+um(i1+1,i2,i3-1,kd)+um(i1-1,i2,i3-1,kd)
     & ) )/(dx(0)**2*dx(2)**2)+( 8.*um(i1,i2,i3,kd)     -4.*(um(i1,i2+
     & 1,i3,kd)  +um(i1,i2-1,i3,kd)  +um(i1,i2  ,i3+1,kd)+um(i1,i2  ,
     & i3-1,kd))   +2.*(um(i1,i2+1,i3+1,kd)+um(i1,i2-1,i3+1,kd)+um(i1,
     & i2+1,i3-1,kd)+um(i1,i2-1,i3-1,kd)) )/(dx(1)**2*dx(2)**2)
        ffr2(i1,i2,i3,kd)=(ff(i1+1,i2,i3,kd)-ff(i1-1,i2,i3,kd))*d12(0)
        ffs2(i1,i2,i3,kd)=(ff(i1,i2+1,i3,kd)-ff(i1,i2-1,i3,kd))*d12(1)
        fft2(i1,i2,i3,kd)=(ff(i1,i2,i3+1,kd)-ff(i1,i2,i3-1,kd))*d12(2)
        ffrr2(i1,i2,i3,kd)=(-2.*ff(i1,i2,i3,kd)+(ff(i1+1,i2,i3,kd)+ff(
     & i1-1,i2,i3,kd)) )*d22(0)
        ffss2(i1,i2,i3,kd)=(-2.*ff(i1,i2,i3,kd)+(ff(i1,i2+1,i3,kd)+ff(
     & i1,i2-1,i3,kd)) )*d22(1)
        ffrs2(i1,i2,i3,kd)=(ffr2(i1,i2+1,i3,kd)-ffr2(i1,i2-1,i3,kd))*
     & d12(1)
        fftt2(i1,i2,i3,kd)=(-2.*ff(i1,i2,i3,kd)+(ff(i1,i2,i3+1,kd)+ff(
     & i1,i2,i3-1,kd)) )*d22(2)
        ffrt2(i1,i2,i3,kd)=(ffr2(i1,i2,i3+1,kd)-ffr2(i1,i2,i3-1,kd))*
     & d12(2)
        ffst2(i1,i2,i3,kd)=(ffs2(i1,i2,i3+1,kd)-ffs2(i1,i2,i3-1,kd))*
     & d12(2)
        ffrrr2(i1,i2,i3,kd)=(-2.*(ff(i1+1,i2,i3,kd)-ff(i1-1,i2,i3,kd))+
     & (ff(i1+2,i2,i3,kd)-ff(i1-2,i2,i3,kd)) )*d22(0)*d12(0)
        ffsss2(i1,i2,i3,kd)=(-2.*(ff(i1,i2+1,i3,kd)-ff(i1,i2-1,i3,kd))+
     & (ff(i1,i2+2,i3,kd)-ff(i1,i2-2,i3,kd)) )*d22(1)*d12(1)
        ffttt2(i1,i2,i3,kd)=(-2.*(ff(i1,i2,i3+1,kd)-ff(i1,i2,i3-1,kd))+
     & (ff(i1,i2,i3+2,kd)-ff(i1,i2,i3-2,kd)) )*d22(2)*d12(2)
        ffx21(i1,i2,i3,kd)= rx(i1,i2,i3)*ffr2(i1,i2,i3,kd)
        ffy21(i1,i2,i3,kd)=0
        ffz21(i1,i2,i3,kd)=0
        ffx22(i1,i2,i3,kd)= rx(i1,i2,i3)*ffr2(i1,i2,i3,kd)+sx(i1,i2,i3)
     & *ffs2(i1,i2,i3,kd)
        ffy22(i1,i2,i3,kd)= ry(i1,i2,i3)*ffr2(i1,i2,i3,kd)+sy(i1,i2,i3)
     & *ffs2(i1,i2,i3,kd)
        ffz22(i1,i2,i3,kd)=0
        ffx23(i1,i2,i3,kd)=rx(i1,i2,i3)*ffr2(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & ffs2(i1,i2,i3,kd)+tx(i1,i2,i3)*fft2(i1,i2,i3,kd)
        ffy23(i1,i2,i3,kd)=ry(i1,i2,i3)*ffr2(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & ffs2(i1,i2,i3,kd)+ty(i1,i2,i3)*fft2(i1,i2,i3,kd)
        ffz23(i1,i2,i3,kd)=rz(i1,i2,i3)*ffr2(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & ffs2(i1,i2,i3,kd)+tz(i1,i2,i3)*fft2(i1,i2,i3,kd)
        ffxx21(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*ffrr2(i1,i2,i3,kd)+(
     & rxx22(i1,i2,i3))*ffr2(i1,i2,i3,kd)
        ffyy21(i1,i2,i3,kd)=0
        ffxy21(i1,i2,i3,kd)=0
        ffxz21(i1,i2,i3,kd)=0
        ffyz21(i1,i2,i3,kd)=0
        ffzz21(i1,i2,i3,kd)=0
        fflaplacian21(i1,i2,i3,kd)=ffxx21(i1,i2,i3,kd)
        ffxx22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*ffrr2(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*ffrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2)
     & *ffss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3))*ffr2(i1,i2,i3,kd)+(sxx22(
     & i1,i2,i3))*ffs2(i1,i2,i3,kd)
        ffyy22(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*ffrr2(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*ffrs2(i1,i2,i3,kd)+(sy(i1,i2,i3)**2)
     & *ffss2(i1,i2,i3,kd)+(ryy22(i1,i2,i3))*ffr2(i1,i2,i3,kd)+(syy22(
     & i1,i2,i3))*ffs2(i1,i2,i3,kd)
        ffxy22(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*ffrr2(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & ffrs2(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*ffss2(i1,i2,i3,kd)
     & +rxy22(i1,i2,i3)*ffr2(i1,i2,i3,kd)+sxy22(i1,i2,i3)*ffs2(i1,i2,
     & i3,kd)
        ffxz22(i1,i2,i3,kd)=0
        ffyz22(i1,i2,i3,kd)=0
        ffzz22(i1,i2,i3,kd)=0
        fflaplacian22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & ffrr2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*
     & sy(i1,i2,i3))*ffrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)*
     & *2)*ffss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3)+ryy22(i1,i2,i3))*ffr2(
     & i1,i2,i3,kd)+(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*ffs2(i1,i2,i3,
     & kd)
        ffxx23(i1,i2,i3,kd)=rx(i1,i2,i3)**2*ffrr2(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*ffss2(i1,i2,i3,kd)+tx(i1,i2,i3)**2*fftt2(i1,i2,i3,kd)
     & +2.*rx(i1,i2,i3)*sx(i1,i2,i3)*ffrs2(i1,i2,i3,kd)+2.*rx(i1,i2,
     & i3)*tx(i1,i2,i3)*ffrt2(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,i2,
     & i3)*ffst2(i1,i2,i3,kd)+rxx23(i1,i2,i3)*ffr2(i1,i2,i3,kd)+sxx23(
     & i1,i2,i3)*ffs2(i1,i2,i3,kd)+txx23(i1,i2,i3)*fft2(i1,i2,i3,kd)
        ffyy23(i1,i2,i3,kd)=ry(i1,i2,i3)**2*ffrr2(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*ffss2(i1,i2,i3,kd)+ty(i1,i2,i3)**2*fftt2(i1,i2,i3,kd)
     & +2.*ry(i1,i2,i3)*sy(i1,i2,i3)*ffrs2(i1,i2,i3,kd)+2.*ry(i1,i2,
     & i3)*ty(i1,i2,i3)*ffrt2(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,i2,
     & i3)*ffst2(i1,i2,i3,kd)+ryy23(i1,i2,i3)*ffr2(i1,i2,i3,kd)+syy23(
     & i1,i2,i3)*ffs2(i1,i2,i3,kd)+tyy23(i1,i2,i3)*fft2(i1,i2,i3,kd)
        ffzz23(i1,i2,i3,kd)=rz(i1,i2,i3)**2*ffrr2(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*ffss2(i1,i2,i3,kd)+tz(i1,i2,i3)**2*fftt2(i1,i2,i3,kd)
     & +2.*rz(i1,i2,i3)*sz(i1,i2,i3)*ffrs2(i1,i2,i3,kd)+2.*rz(i1,i2,
     & i3)*tz(i1,i2,i3)*ffrt2(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,i2,
     & i3)*ffst2(i1,i2,i3,kd)+rzz23(i1,i2,i3)*ffr2(i1,i2,i3,kd)+szz23(
     & i1,i2,i3)*ffs2(i1,i2,i3,kd)+tzz23(i1,i2,i3)*fft2(i1,i2,i3,kd)
        ffxy23(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*ffrr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*ffss2(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & ty(i1,i2,i3)*fftt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(
     & i1,i2,i3)*sx(i1,i2,i3))*ffrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(i1,
     & i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*ffrt2(i1,i2,i3,kd)+(sx(i1,i2,
     & i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*ffst2(i1,i2,i3,kd)+
     & rxy23(i1,i2,i3)*ffr2(i1,i2,i3,kd)+sxy23(i1,i2,i3)*ffs2(i1,i2,
     & i3,kd)+txy23(i1,i2,i3)*fft2(i1,i2,i3,kd)
        ffxz23(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*ffrr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*ffss2(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & tz(i1,i2,i3)*fftt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sx(i1,i2,i3))*ffrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(i1,
     & i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*ffrt2(i1,i2,i3,kd)+(sx(i1,i2,
     & i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*ffst2(i1,i2,i3,kd)+
     & rxz23(i1,i2,i3)*ffr2(i1,i2,i3,kd)+sxz23(i1,i2,i3)*ffs2(i1,i2,
     & i3,kd)+txz23(i1,i2,i3)*fft2(i1,i2,i3,kd)
        ffyz23(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*ffrr2(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*ffss2(i1,i2,i3,kd)+ty(i1,i2,i3)*
     & tz(i1,i2,i3)*fftt2(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sy(i1,i2,i3))*ffrs2(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(i1,
     & i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*ffrt2(i1,i2,i3,kd)+(sy(i1,i2,
     & i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*ffst2(i1,i2,i3,kd)+
     & ryz23(i1,i2,i3)*ffr2(i1,i2,i3,kd)+syz23(i1,i2,i3)*ffs2(i1,i2,
     & i3,kd)+tyz23(i1,i2,i3)*fft2(i1,i2,i3,kd)
        fflaplacian23(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*ffrr2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)*
     & *2+sz(i1,i2,i3)**2)*ffss2(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(i1,
     & i2,i3)**2+tz(i1,i2,i3)**2)*fftt2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*
     & sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,
     & i3))*ffrs2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(i1,
     & i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*ffrt2(i1,i2,i3,
     & kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)+
     & sz(i1,i2,i3)*tz(i1,i2,i3))*ffst2(i1,i2,i3,kd)+(rxx23(i1,i2,i3)+
     & ryy23(i1,i2,i3)+rzz23(i1,i2,i3))*ffr2(i1,i2,i3,kd)+(sxx23(i1,
     & i2,i3)+syy23(i1,i2,i3)+szz23(i1,i2,i3))*ffs2(i1,i2,i3,kd)+(
     & txx23(i1,i2,i3)+tyy23(i1,i2,i3)+tzz23(i1,i2,i3))*fft2(i1,i2,i3,
     & kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
        ffx23r(i1,i2,i3,kd)=(ff(i1+1,i2,i3,kd)-ff(i1-1,i2,i3,kd))*h12(
     & 0)
        ffy23r(i1,i2,i3,kd)=(ff(i1,i2+1,i3,kd)-ff(i1,i2-1,i3,kd))*h12(
     & 1)
        ffz23r(i1,i2,i3,kd)=(ff(i1,i2,i3+1,kd)-ff(i1,i2,i3-1,kd))*h12(
     & 2)
        ffxx23r(i1,i2,i3,kd)=(-2.*ff(i1,i2,i3,kd)+(ff(i1+1,i2,i3,kd)+
     & ff(i1-1,i2,i3,kd)) )*h22(0)
        ffyy23r(i1,i2,i3,kd)=(-2.*ff(i1,i2,i3,kd)+(ff(i1,i2+1,i3,kd)+
     & ff(i1,i2-1,i3,kd)) )*h22(1)
        ffxy23r(i1,i2,i3,kd)=(ffx23r(i1,i2+1,i3,kd)-ffx23r(i1,i2-1,i3,
     & kd))*h12(1)
        ffzz23r(i1,i2,i3,kd)=(-2.*ff(i1,i2,i3,kd)+(ff(i1,i2,i3+1,kd)+
     & ff(i1,i2,i3-1,kd)) )*h22(2)
        ffxz23r(i1,i2,i3,kd)=(ffx23r(i1,i2,i3+1,kd)-ffx23r(i1,i2,i3-1,
     & kd))*h12(2)
        ffyz23r(i1,i2,i3,kd)=(ffy23r(i1,i2,i3+1,kd)-ffy23r(i1,i2,i3-1,
     & kd))*h12(2)
        ffx21r(i1,i2,i3,kd)= ffx23r(i1,i2,i3,kd)
        ffy21r(i1,i2,i3,kd)= ffy23r(i1,i2,i3,kd)
        ffz21r(i1,i2,i3,kd)= ffz23r(i1,i2,i3,kd)
        ffxx21r(i1,i2,i3,kd)= ffxx23r(i1,i2,i3,kd)
        ffyy21r(i1,i2,i3,kd)= ffyy23r(i1,i2,i3,kd)
        ffzz21r(i1,i2,i3,kd)= ffzz23r(i1,i2,i3,kd)
        ffxy21r(i1,i2,i3,kd)= ffxy23r(i1,i2,i3,kd)
        ffxz21r(i1,i2,i3,kd)= ffxz23r(i1,i2,i3,kd)
        ffyz21r(i1,i2,i3,kd)= ffyz23r(i1,i2,i3,kd)
        fflaplacian21r(i1,i2,i3,kd)=ffxx23r(i1,i2,i3,kd)
        ffx22r(i1,i2,i3,kd)= ffx23r(i1,i2,i3,kd)
        ffy22r(i1,i2,i3,kd)= ffy23r(i1,i2,i3,kd)
        ffz22r(i1,i2,i3,kd)= ffz23r(i1,i2,i3,kd)
        ffxx22r(i1,i2,i3,kd)= ffxx23r(i1,i2,i3,kd)
        ffyy22r(i1,i2,i3,kd)= ffyy23r(i1,i2,i3,kd)
        ffzz22r(i1,i2,i3,kd)= ffzz23r(i1,i2,i3,kd)
        ffxy22r(i1,i2,i3,kd)= ffxy23r(i1,i2,i3,kd)
        ffxz22r(i1,i2,i3,kd)= ffxz23r(i1,i2,i3,kd)
        ffyz22r(i1,i2,i3,kd)= ffyz23r(i1,i2,i3,kd)
        fflaplacian22r(i1,i2,i3,kd)=ffxx23r(i1,i2,i3,kd)+ffyy23r(i1,i2,
     & i3,kd)
        fflaplacian23r(i1,i2,i3,kd)=ffxx23r(i1,i2,i3,kd)+ffyy23r(i1,i2,
     & i3,kd)+ffzz23r(i1,i2,i3,kd)
        ffxxx22r(i1,i2,i3,kd)=(-2.*(ff(i1+1,i2,i3,kd)-ff(i1-1,i2,i3,kd)
     & )+(ff(i1+2,i2,i3,kd)-ff(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
        ffyyy22r(i1,i2,i3,kd)=(-2.*(ff(i1,i2+1,i3,kd)-ff(i1,i2-1,i3,kd)
     & )+(ff(i1,i2+2,i3,kd)-ff(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
        ffxxy22r(i1,i2,i3,kd)=( ffxx22r(i1,i2+1,i3,kd)-ffxx22r(i1,i2-1,
     & i3,kd))/(2.*dx(1))
        ffxyy22r(i1,i2,i3,kd)=( ffyy22r(i1+1,i2,i3,kd)-ffyy22r(i1-1,i2,
     & i3,kd))/(2.*dx(0))
        ffxxxx22r(i1,i2,i3,kd)=(6.*ff(i1,i2,i3,kd)-4.*(ff(i1+1,i2,i3,
     & kd)+ff(i1-1,i2,i3,kd))+(ff(i1+2,i2,i3,kd)+ff(i1-2,i2,i3,kd)) )
     & /(dx(0)**4)
        ffyyyy22r(i1,i2,i3,kd)=(6.*ff(i1,i2,i3,kd)-4.*(ff(i1,i2+1,i3,
     & kd)+ff(i1,i2-1,i3,kd))+(ff(i1,i2+2,i3,kd)+ff(i1,i2-2,i3,kd)) )
     & /(dx(1)**4)
        ffxxyy22r(i1,i2,i3,kd)=( 4.*ff(i1,i2,i3,kd)     -2.*(ff(i1+1,
     & i2,i3,kd)+ff(i1-1,i2,i3,kd)+ff(i1,i2+1,i3,kd)+ff(i1,i2-1,i3,kd)
     & )   +   (ff(i1+1,i2+1,i3,kd)+ff(i1-1,i2+1,i3,kd)+ff(i1+1,i2-1,
     & i3,kd)+ff(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
        ! 2D laplacian squared = ff.xxxx + 2 ff.xxyy + ff.yyyy
        ffLapSq22r(i1,i2,i3,kd)= ( 6.*ff(i1,i2,i3,kd)   - 4.*(ff(i1+1,
     & i2,i3,kd)+ff(i1-1,i2,i3,kd))    +(ff(i1+2,i2,i3,kd)+ff(i1-2,i2,
     & i3,kd)) )/(dx(0)**4) +( 6.*ff(i1,i2,i3,kd)    -4.*(ff(i1,i2+1,
     & i3,kd)+ff(i1,i2-1,i3,kd))    +(ff(i1,i2+2,i3,kd)+ff(i1,i2-2,i3,
     & kd)) )/(dx(1)**4)  +( 8.*ff(i1,i2,i3,kd)     -4.*(ff(i1+1,i2,
     & i3,kd)+ff(i1-1,i2,i3,kd)+ff(i1,i2+1,i3,kd)+ff(i1,i2-1,i3,kd))  
     &  +2.*(ff(i1+1,i2+1,i3,kd)+ff(i1-1,i2+1,i3,kd)+ff(i1+1,i2-1,i3,
     & kd)+ff(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
        ffxxx23r(i1,i2,i3,kd)=(-2.*(ff(i1+1,i2,i3,kd)-ff(i1-1,i2,i3,kd)
     & )+(ff(i1+2,i2,i3,kd)-ff(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
        ffyyy23r(i1,i2,i3,kd)=(-2.*(ff(i1,i2+1,i3,kd)-ff(i1,i2-1,i3,kd)
     & )+(ff(i1,i2+2,i3,kd)-ff(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
        ffzzz23r(i1,i2,i3,kd)=(-2.*(ff(i1,i2,i3+1,kd)-ff(i1,i2,i3-1,kd)
     & )+(ff(i1,i2,i3+2,kd)-ff(i1,i2,i3-2,kd)) )*h22(1)*h12(2)
        ffxxy23r(i1,i2,i3,kd)=( ffxx22r(i1,i2+1,i3,kd)-ffxx22r(i1,i2-1,
     & i3,kd))/(2.*dx(1))
        ffxyy23r(i1,i2,i3,kd)=( ffyy22r(i1+1,i2,i3,kd)-ffyy22r(i1-1,i2,
     & i3,kd))/(2.*dx(0))
        ffxxz23r(i1,i2,i3,kd)=( ffxx22r(i1,i2,i3+1,kd)-ffxx22r(i1,i2,
     & i3-1,kd))/(2.*dx(2))
        ffyyz23r(i1,i2,i3,kd)=( ffyy22r(i1,i2,i3+1,kd)-ffyy22r(i1,i2,
     & i3-1,kd))/(2.*dx(2))
        ffxzz23r(i1,i2,i3,kd)=( ffzz22r(i1+1,i2,i3,kd)-ffzz22r(i1-1,i2,
     & i3,kd))/(2.*dx(0))
        ffyzz23r(i1,i2,i3,kd)=( ffzz22r(i1,i2+1,i3,kd)-ffzz22r(i1,i2-1,
     & i3,kd))/(2.*dx(1))
        ffxxxx23r(i1,i2,i3,kd)=(6.*ff(i1,i2,i3,kd)-4.*(ff(i1+1,i2,i3,
     & kd)+ff(i1-1,i2,i3,kd))+(ff(i1+2,i2,i3,kd)+ff(i1-2,i2,i3,kd)) )
     & /(dx(0)**4)
        ffyyyy23r(i1,i2,i3,kd)=(6.*ff(i1,i2,i3,kd)-4.*(ff(i1,i2+1,i3,
     & kd)+ff(i1,i2-1,i3,kd))+(ff(i1,i2+2,i3,kd)+ff(i1,i2-2,i3,kd)) )
     & /(dx(1)**4)
        ffzzzz23r(i1,i2,i3,kd)=(6.*ff(i1,i2,i3,kd)-4.*(ff(i1,i2,i3+1,
     & kd)+ff(i1,i2,i3-1,kd))+(ff(i1,i2,i3+2,kd)+ff(i1,i2,i3-2,kd)) )
     & /(dx(2)**4)
        ffxxyy23r(i1,i2,i3,kd)=( 4.*ff(i1,i2,i3,kd)     -2.*(ff(i1+1,
     & i2,i3,kd)+ff(i1-1,i2,i3,kd)+ff(i1,i2+1,i3,kd)+ff(i1,i2-1,i3,kd)
     & )   +   (ff(i1+1,i2+1,i3,kd)+ff(i1-1,i2+1,i3,kd)+ff(i1+1,i2-1,
     & i3,kd)+ff(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
        ffxxzz23r(i1,i2,i3,kd)=( 4.*ff(i1,i2,i3,kd)     -2.*(ff(i1+1,
     & i2,i3,kd)+ff(i1-1,i2,i3,kd)+ff(i1,i2,i3+1,kd)+ff(i1,i2,i3-1,kd)
     & )   +   (ff(i1+1,i2,i3+1,kd)+ff(i1-1,i2,i3+1,kd)+ff(i1+1,i2,i3-
     & 1,kd)+ff(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)
        ffyyzz23r(i1,i2,i3,kd)=( 4.*ff(i1,i2,i3,kd)     -2.*(ff(i1,i2+
     & 1,i3,kd)  +ff(i1,i2-1,i3,kd)+  ff(i1,i2  ,i3+1,kd)+ff(i1,i2  ,
     & i3-1,kd))   +   (ff(i1,i2+1,i3+1,kd)+ff(i1,i2-1,i3+1,kd)+ff(i1,
     & i2+1,i3-1,kd)+ff(i1,i2-1,i3-1,kd)) )/(dx(1)**2*dx(2)**2)
        ! 3D laplacian squared = ff.xxxx + ff.yyyy + ff.zzzz + 2 (ff.xxyy + ff.xxzz + ff.yyzz )
        ffLapSq23r(i1,i2,i3,kd)= ( 6.*ff(i1,i2,i3,kd)   - 4.*(ff(i1+1,
     & i2,i3,kd)+ff(i1-1,i2,i3,kd))    +(ff(i1+2,i2,i3,kd)+ff(i1-2,i2,
     & i3,kd)) )/(dx(0)**4) +( 6.*ff(i1,i2,i3,kd)    -4.*(ff(i1,i2+1,
     & i3,kd)+ff(i1,i2-1,i3,kd))    +(ff(i1,i2+2,i3,kd)+ff(i1,i2-2,i3,
     & kd)) )/(dx(1)**4)  +( 6.*ff(i1,i2,i3,kd)    -4.*(ff(i1,i2,i3+1,
     & kd)+ff(i1,i2,i3-1,kd))    +(ff(i1,i2,i3+2,kd)+ff(i1,i2,i3-2,kd)
     & ) )/(dx(2)**4)  +( 8.*ff(i1,i2,i3,kd)     -4.*(ff(i1+1,i2,i3,
     & kd)  +ff(i1-1,i2,i3,kd)  +ff(i1  ,i2+1,i3,kd)+ff(i1  ,i2-1,i3,
     & kd))   +2.*(ff(i1+1,i2+1,i3,kd)+ff(i1-1,i2+1,i3,kd)+ff(i1+1,i2-
     & 1,i3,kd)+ff(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)+( 8.*ff(i1,
     & i2,i3,kd)     -4.*(ff(i1+1,i2,i3,kd)  +ff(i1-1,i2,i3,kd)  +ff(
     & i1  ,i2,i3+1,kd)+ff(i1  ,i2,i3-1,kd))   +2.*(ff(i1+1,i2,i3+1,
     & kd)+ff(i1-1,i2,i3+1,kd)+ff(i1+1,i2,i3-1,kd)+ff(i1-1,i2,i3-1,kd)
     & ) )/(dx(0)**2*dx(2)**2)+( 8.*ff(i1,i2,i3,kd)     -4.*(ff(i1,i2+
     & 1,i3,kd)  +ff(i1,i2-1,i3,kd)  +ff(i1,i2  ,i3+1,kd)+ff(i1,i2  ,
     & i3-1,kd))   +2.*(ff(i1,i2+1,i3+1,kd)+ff(i1,i2-1,i3+1,kd)+ff(i1,
     & i2+1,i3-1,kd)+ff(i1,i2-1,i3-1,kd)) )/(dx(1)**2*dx(2)**2)
        ! 2nd-order in space and time
        maxwell2dr(i1,i2,i3,n)=2.*u(i1,i2,i3,n)-um(i1,i2,i3,n)+cdtdx*(
     & u(i1-1,i2,i3,n)+u(i1+1,i2,i3,n)-2.*u(i1,i2,i3,n))+cdtdy*(u(i1,
     & i2-1,i3,n)+u(i1,i2+1,i3,n)-2.*u(i1,i2,i3,n))
        maxwell3dr(i1,i2,i3,n)=2.*u(i1,i2,i3,n)-um(i1,i2,i3,n)+cdtdx*(
     & u(i1-1,i2,i3,n)+u(i1+1,i2,i3,n)-2.*u(i1,i2,i3,n))+cdtdy*(u(i1,
     & i2-1,i3,n)+u(i1,i2+1,i3,n)-2.*u(i1,i2,i3,n))+cdtdz*(u(i1,i2,i3-
     & 1,n)+u(i1,i2,i3+1,n)-2.*u(i1,i2,i3,n))
        maxwellc22(i1,i2,i3,n)=2.*u(i1,i2,i3,n)-um(i1,i2,i3,n)+dtsq*f(
     & i1,i2,i3,n)
        ! 2D, 2nd-order, div cleaning:
        !    D+tD-t( E ) + alpha*( D0t E ) = c^2 Delta(E) + alpha*( (1/eps) Curl ( H ) )
        ! - rectangular: 
        mxdc2d2Ex(i1,i2,i3) = cdc0*u(i1,i2,i3,ex)+cdc1*um(i1,i2,i3,ex)+
     & cdcxx*(u(i1-1,i2,i3,ex)+u(i1+1,i2,i3,ex)-2.*u(i1,i2,i3,ex))+
     & cdcyy*(u(i1,i2-1,i3,ex)+u(i1,i2+1,i3,ex)-2.*u(i1,i2,i3,ex))+
     & cdcEdy*( u(i1,i2+1,i3,hz)-u(i1,i2-1,i3,hz) )
        mxdc2d2Ey(i1,i2,i3) = cdc0*u(i1,i2,i3,ey)+cdc1*um(i1,i2,i3,ey)+
     & cdcxx*(u(i1-1,i2,i3,ey)+u(i1+1,i2,i3,ey)-2.*u(i1,i2,i3,ey))+
     & cdcyy*(u(i1,i2-1,i3,ey)+u(i1,i2+1,i3,ey)-2.*u(i1,i2,i3,ey))-
     & cdcEdx*( u(i1+1,i2,i3,hz)-u(i1-1,i2,i3,hz) )
        ! - 2D curvilinear:  (assumes f contains Delta u )
        mxdc2d2cEx(i1,i2,i3) = cdc0*u(i1,i2,i3,ex)+cdc1*um(i1,i2,i3,ex)
     & + cdcf*f(i1,i2,i3,ex)+ cdcE*uy22(i1,i2,i3,hz)
        mxdc2d2cEy(i1,i2,i3) = cdc0*u(i1,i2,i3,ey)+cdc1*um(i1,i2,i3,ey)
     & + cdcf*f(i1,i2,i3,ey)- cdcE*ux22(i1,i2,i3,hz)
        ! Stoermer: 4th order in space and 4th order in time:
        maxwellr44(i1,i2,i3,n)=2.*u(I1,I2,I3,n)-um(I1,I2,I3,n)+c40*lap(
     & n)+c41*v(I1,I2,I3,n)+c42*vvt2(I1,I2,I3,n)+c43*ut3(I1,I2,I3,n)
        maxwellc44(i1,i2,i3,n)=2.*u(I1,I2,I3,n)-um(I1,I2,I3,n)+c40*f(
     & i1,i2,i3,n)+c41*v(I1,I2,I3,n)+c42*vvt2(I1,I2,I3,n)+c43*ut3(I1,
     & I2,I3,n)
        ! Stoermer: 6th order in space and 6th order in time:
        maxwellr66(i1,i2,i3,n)=2.*u(I1,I2,I3,n)-um(I1,I2,I3,n)+c60*lap(
     & n)+c61*v(I1,I2,I3,n)+c62*vvt2(I1,I2,I3,n)+c63*ut3(I1,I2,I3,n)+
     & c64*vvt4(I1,I2,I3,n)+c65*ut5(I1,I2,I3,n)
        maxwellc66(i1,i2,i3,n)=2.*u(I1,I2,I3,n)-um(I1,I2,I3,n)+c60*f(
     & i1,i2,i3,n)+c61*v(I1,I2,I3,n)+c62*vvt2(I1,I2,I3,n)+c63*ut3(I1,
     & I2,I3,n)+c64*vvt4(I1,I2,I3,n)+c65*ut5(I1,I2,I3,n)
        ! Stoermer: 8th order in space and 8th order in time:
        maxwellr88(i1,i2,i3,n)=2.*u(I1,I2,I3,n)-um(I1,I2,I3,n)+c80*lap(
     & n)+c81*v(I1,I2,I3,n)+c82*vvt2(I1,I2,I3,n)+c83*ut3(I1,I2,I3,n)+
     & c84*vvt4(I1,I2,I3,n)+c85*ut5(I1,I2,I3,n)+c86*ut6(I1,I2,I3,n)+
     & c87*ut7(I1,I2,I3,n)
        maxwellc88(i1,i2,i3,n)=2.*u(I1,I2,I3,n)-um(I1,I2,I3,n)+c80*f(
     & i1,i2,i3,n)+c81*v(I1,I2,I3,n)+c82*vvt2(I1,I2,I3,n)+c83*ut3(I1,
     & I2,I3,n)+c84*vvt4(I1,I2,I3,n)+c85*ut5(I1,I2,I3,n)+c86*ut6(I1,
     & I2,I3,n)+c87*ut7(I1,I2,I3,n)
        !    *** 2nd order ***
        lap2d2(i1,i2,i3,c)=(u(i1+1,i2,i3,c)-2.*u(i1,i2,i3,c)+u(i1-1,i2,
     & i3,c))*dxsqi+(u(i1,i2+1,i3,c)-2.*u(i1,i2,i3,c)+u(i1,i2-1,i3,c))
     & *dysqi
        lap2d2m(i1,i2,i3,c)=(um(i1+1,i2,i3,c)-2.*um(i1,i2,i3,c)+um(i1-
     & 1,i2,i3,c))*dxsqi+(um(i1,i2+1,i3,c)-2.*um(i1,i2,i3,c)+um(i1,i2-
     & 1,i3,c))*dysqi
        lap3d2(i1,i2,i3,c)=(u(i1+1,i2,i3,c)-2.*u(i1,i2,i3,c)+u(i1-1,i2,
     & i3,c))*dxsqi+(u(i1,i2+1,i3,c)-2.*u(i1,i2,i3,c)+u(i1,i2-1,i3,c))
     & *dysqi+(u(i1,i2,i3+1,c)-2.*u(i1,i2,i3,c)+u(i1,i2,i3-1,c))*dzsqi
        lap3d2m(i1,i2,i3,c)=(um(i1+1,i2,i3,c)-2.*um(i1,i2,i3,c)+um(i1-
     & 1,i2,i3,c))*dxsqi+(um(i1,i2+1,i3,c)-2.*um(i1,i2,i3,c)+um(i1,i2-
     & 1,i3,c))*dysqi+(um(i1,i2,i3+1,c)-2.*um(i1,i2,i3,c)+um(i1,i2,i3-
     & 1,c))*dzsqi
        lap2d2f(i1,i2,i3,c,m)=(fa(i1+1,i2,i3,c,m)-2.*fa(i1,i2,i3,c,m)+
     & fa(i1-1,i2,i3,c,m))*dxsqi+(fa(i1,i2+1,i3,c,m)-2.*fa(i1,i2,i3,c,
     & m)+fa(i1,i2-1,i3,c,m))*dysqi
        lap3d2f(i1,i2,i3,c,m)=(fa(i1+1,i2,i3,c,m)-2.*fa(i1,i2,i3,c,m)+
     & fa(i1-1,i2,i3,c,m))*dxsqi+(fa(i1,i2+1,i3,c,m)-2.*fa(i1,i2,i3,c,
     & m)+fa(i1,i2-1,i3,c,m))*dysqi+(fa(i1,i2,i3+1,c,m)-2.*fa(i1,i2,
     & i3,c,m)+fa(i1,i2,i3-1,c,m))*dzsqi
        ! 2D laplacian squared = u.xxxx + 2 u.xxyy + u.yyyy
        lap2d2Pow2(i1,i2,i3,c)= ( 6.*u(i1,i2,i3,c)   - 4.*(u(i1+1,i2,
     & i3,c)+u(i1-1,i2,i3,c))    +(u(i1+2,i2,i3,c)+u(i1-2,i2,i3,c)) )*
     & dxi4 +( 6.*u(i1,i2,i3,c)    -4.*(u(i1,i2+1,i3,c)+u(i1,i2-1,i3,
     & c))    +(u(i1,i2+2,i3,c)+u(i1,i2-2,i3,c)) )*dyi4  +( 8.*u(i1,
     & i2,i3,c)     -4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)+u(i1,i2+1,i3,
     & c)+u(i1,i2-1,i3,c))   +2.*(u(i1+1,i2+1,i3,c)+u(i1-1,i2+1,i3,c)+
     & u(i1+1,i2-1,i3,c)+u(i1-1,i2-1,i3,c)) )*dxdyi2
        ! 3D laplacian squared = u.xxxx + u.yyyy + u.zzzz + 2 (u.xxyy + u.xxzz + u.yyzz )
        lap3d2Pow2(i1,i2,i3,c)= ( 6.*u(i1,i2,i3,c)   - 4.*(u(i1+1,i2,
     & i3,c)+u(i1-1,i2,i3,c))    +(u(i1+2,i2,i3,c)+u(i1-2,i2,i3,c)) )*
     & dxi4 +(  +6.*u(i1,i2,i3,c)    -4.*(u(i1,i2+1,i3,c)+u(i1,i2-1,
     & i3,c))    +(u(i1,i2+2,i3,c)+u(i1,i2-2,i3,c)) )*dyi4+(  +6.*u(
     & i1,i2,i3,c)    -4.*(u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c))    +(u(i1,
     & i2,i3+2,c)+u(i1,i2,i3-2,c)) )*dzi4+(8.*u(i1,i2,i3,c)     -4.*(
     & u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)+u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c)
     & )   +2.*(u(i1+1,i2+1,i3,c)+u(i1-1,i2+1,i3,c)+u(i1+1,i2-1,i3,c)+
     & u(i1-1,i2-1,i3,c)) )*dxdyi2 +(8.*u(i1,i2,i3,c)     -4.*(u(i1+1,
     & i2,i3,c)+u(i1-1,i2,i3,c)+u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c))   +
     & 2.*(u(i1+1,i2,i3+1,c)+u(i1-1,i2,i3+1,c)+u(i1+1,i2,i3-1,c)+u(i1-
     & 1,i2,i3-1,c)) )*dxdzi2 +(8.*u(i1,i2,i3,c)     -4.*(u(i1,i2+1,
     & i3,c)+u(i1,i2-1,i3,c)+u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c))   +2.*(
     & u(i1,i2+1,i3+1,c)+u(i1,i2-1,i3+1,c)+u(i1,i2+1,i3-1,c)+u(i1,i2-
     & 1,i3-1,c)) )*dydzi2
        lap2d2Pow3(i1,i2,i3,c)=(lap2d2Pow2(i1+1,i2,i3,c)-2.*lap2d2Pow2(
     & i1,i2,i3,c)+lap2d2Pow2(i1-1,i2,i3,c))*dxsqi+(lap2d2Pow2(i1,i2+
     & 1,i3,c)-2.*lap2d2Pow2(i1,i2,i3,c)+lap2d2Pow2(i1,i2-1,i3,c))*
     & dysqi
        lap3d2Pow3(i1,i2,i3,c)=(lap3d2Pow2(i1+1,i2,i3,c)-2.*lap3d2Pow2(
     & i1,i2,i3,c)+lap3d2Pow2(i1-1,i2,i3,c))*dxsqi+(lap3d2Pow2(i1,i2+
     & 1,i3,c)-2.*lap3d2Pow2(i1,i2,i3,c)+lap3d2Pow2(i1,i2-1,i3,c))*
     & dysqi+(lap3d2Pow2(i1,i2,i3+1,c)-2.*lap3d2Pow2(i1,i2,i3,c)+
     & lap3d2Pow2(i1,i2,i3-1,c))*dzsqi
        lap2d2Pow4(i1,i2,i3,c)=(6.*lap2d2Pow2(i1,i2,i3,c)-4.*(
     & lap2d2Pow2(i1+1,i2,i3,c)+lap2d2Pow2(i1-1,i2,i3,c))+(lap2d2Pow2(
     & i1+2,i2,i3,c)+lap2d2Pow2(i1-2,i2,i3,c)))*dxi4+(6.*lap2d2Pow2(
     & i1,i2,i3,c)-4.*(lap2d2Pow2(i1,i2+1,i3,c)+lap2d2Pow2(i1,i2-1,i3,
     & c))+(lap2d2Pow2(i1,i2+2,i3,c)+lap2d2Pow2(i1,i2-2,i3,c)))*dyi4+(
     & 8.*lap2d2Pow2(i1,i2,i3,c)-4.*(lap2d2Pow2(i1+1,i2,i3,c)+
     & lap2d2Pow2(i1-1,i2,i3,c)+lap2d2Pow2(i1,i2+1,i3,c)+lap2d2Pow2(
     & i1,i2-1,i3,c))+2.*(lap2d2Pow2(i1+1,i2+1,i3,c)+lap2d2Pow2(i1-1,
     & i2+1,i3,c)+lap2d2Pow2(i1+1,i2-1,i3,c)+lap2d2Pow2(i1-1,i2-1,i3,
     & c)))*dxdyi2
        lap3d2Pow4(i1,i2,i3,c)=(6.*lap3d2Pow2(i1,i2,i3,c)-4.*(
     & lap3d2Pow2(i1+1,i2,i3,c)+lap3d2Pow2(i1-1,i2,i3,c))+(lap3d2Pow2(
     & i1+2,i2,i3,c)+lap3d2Pow2(i1-2,i2,i3,c)))*dxi4+(+6.*lap3d2Pow2(
     & i1,i2,i3,c)-4.*(lap3d2Pow2(i1,i2+1,i3,c)+lap3d2Pow2(i1,i2-1,i3,
     & c))+(lap3d2Pow2(i1,i2+2,i3,c)+lap3d2Pow2(i1,i2-2,i3,c)))*dyi4+(
     & +6.*lap3d2Pow2(i1,i2,i3,c)-4.*(lap3d2Pow2(i1,i2,i3+1,c)+
     & lap3d2Pow2(i1,i2,i3-1,c))+(lap3d2Pow2(i1,i2,i3+2,c)+lap3d2Pow2(
     & i1,i2,i3-2,c)))*dzi4+(8.*lap3d2Pow2(i1,i2,i3,c)-4.*(lap3d2Pow2(
     & i1+1,i2,i3,c)+lap3d2Pow2(i1-1,i2,i3,c)+lap3d2Pow2(i1,i2+1,i3,c)
     & +lap3d2Pow2(i1,i2-1,i3,c))+2.*(lap3d2Pow2(i1+1,i2+1,i3,c)+
     & lap3d2Pow2(i1-1,i2+1,i3,c)+lap3d2Pow2(i1+1,i2-1,i3,c)+
     & lap3d2Pow2(i1-1,i2-1,i3,c)))*dxdyi2+(8.*lap3d2Pow2(i1,i2,i3,c)-
     & 4.*(lap3d2Pow2(i1+1,i2,i3,c)+lap3d2Pow2(i1-1,i2,i3,c)+
     & lap3d2Pow2(i1,i2,i3+1,c)+lap3d2Pow2(i1,i2,i3-1,c))+2.*(
     & lap3d2Pow2(i1+1,i2,i3+1,c)+lap3d2Pow2(i1-1,i2,i3+1,c)+
     & lap3d2Pow2(i1+1,i2,i3-1,c)+lap3d2Pow2(i1-1,i2,i3-1,c)))*dxdzi2+
     & (8.*lap3d2Pow2(i1,i2,i3,c)-4.*(lap3d2Pow2(i1,i2+1,i3,c)+
     & lap3d2Pow2(i1,i2-1,i3,c)+lap3d2Pow2(i1,i2,i3+1,c)+lap3d2Pow2(
     & i1,i2,i3-1,c))+2.*(lap3d2Pow2(i1,i2+1,i3+1,c)+lap3d2Pow2(i1,i2-
     & 1,i3+1,c)+lap3d2Pow2(i1,i2+1,i3-1,c)+lap3d2Pow2(i1,i2-1,i3-1,c)
     & ))*dydzi2
       !    ** 4th order ****
        lap2d4(i1,i2,i3,c)=( -30.*u(i1,i2,i3,c)     +16.*(u(i1+1,i2,i3,
     & c)+u(i1-1,i2,i3,c))     -(u(i1+2,i2,i3,c)+u(i1-2,i2,i3,c)) )*
     & dxsq12i + ( -30.*u(i1,i2,i3,c)     +16.*(u(i1,i2+1,i3,c)+u(i1,
     & i2-1,i3,c))     -(u(i1,i2+2,i3,c)+u(i1,i2-2,i3,c)) )*dysq12i
        lap3d4(i1,i2,i3,c)=lap2d4(i1,i2,i3,c)+ ( -30.*u(i1,i2,i3,c)    
     &   +16.*(u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c))      -(u(i1,i2,i3+2,c)
     & +u(i1,i2,i3-2,c)) )*dzsq12i
        lap2d4Pow2(i1,i2,i3,c)=(-30.*lap2d4(i1,i2,i3,c)+16.*(lap2d4(i1+
     & 1,i2,i3,c)+lap2d4(i1-1,i2,i3,c))-(lap2d4(i1+2,i2,i3,c)+lap2d4(
     & i1-2,i2,i3,c)))*dxsq12i+(-30.*lap2d4(i1,i2,i3,c)+16.*(lap2d4(
     & i1,i2+1,i3,c)+lap2d4(i1,i2-1,i3,c))-(lap2d4(i1,i2+2,i3,c)+
     & lap2d4(i1,i2-2,i3,c)))*dysq12i
        lap3d4Pow2(i1,i2,i3,c)=(-30.*lap3d4(i1,i2,i3,c)+16.*(lap3d4(i1+
     & 1,i2,i3,c)+lap3d4(i1-1,i2,i3,c))-(lap3d4(i1+2,i2,i3,c)+lap3d4(
     & i1-2,i2,i3,c)))*dxsq12i+(-30.*lap3d4(i1,i2,i3,c)+16.*(lap3d4(
     & i1,i2+1,i3,c)+lap3d4(i1,i2-1,i3,c))-(lap3d4(i1,i2+2,i3,c)+
     & lap3d4(i1,i2-2,i3,c)))*dysq12i+(-30.*lap3d4(i1,i2,i3,c)+16.*(
     & lap3d4(i1,i2,i3+1,c)+lap3d4(i1,i2,i3-1,c))-(lap3d4(i1,i2,i3+2,
     & c)+lap3d4(i1,i2,i3-2,c)))*dzsq12i
        lap2d4Pow3(i1,i2,i3,c)=(-30.*lap2d4Pow2(i1,i2,i3,c)+16.*(
     & lap2d4Pow2(i1+1,i2,i3,c)+lap2d4Pow2(i1-1,i2,i3,c))-(lap2d4Pow2(
     & i1+2,i2,i3,c)+lap2d4Pow2(i1-2,i2,i3,c)))*dxsq12i+(-30.*
     & lap2d4Pow2(i1,i2,i3,c)+16.*(lap2d4Pow2(i1,i2+1,i3,c)+
     & lap2d4Pow2(i1,i2-1,i3,c))-(lap2d4Pow2(i1,i2+2,i3,c)+lap2d4Pow2(
     & i1,i2-2,i3,c)))*dysq12i
        lap3d4Pow3(i1,i2,i3,c)=(-30.*lap3d4Pow2(i1,i2,i3,c)+16.*(
     & lap3d4Pow2(i1+1,i2,i3,c)+lap3d4Pow2(i1-1,i2,i3,c))-(lap3d4Pow2(
     & i1+2,i2,i3,c)+lap3d4Pow2(i1-2,i2,i3,c)))*dxsq12i+(-30.*
     & lap3d4Pow2(i1,i2,i3,c)+16.*(lap3d4Pow2(i1,i2+1,i3,c)+
     & lap3d4Pow2(i1,i2-1,i3,c))-(lap3d4Pow2(i1,i2+2,i3,c)+lap3d4Pow2(
     & i1,i2-2,i3,c)))*dysq12i+(-30.*lap3d4Pow2(i1,i2,i3,c)+16.*(
     & lap3d4Pow2(i1,i2,i3+1,c)+lap3d4Pow2(i1,i2,i3-1,c))-(lap3d4Pow2(
     & i1,i2,i3+2,c)+lap3d4Pow2(i1,i2,i3-2,c)))*dzsq12i
       !     *** 6th order ***
        lap2d6(i1,i2,i3,c)= c00lap2d6*u(i1,i2,i3,c)     +c10lap2d6*(u(
     & i1+1,i2,i3,c)+u(i1-1,i2,i3,c)) +c01lap2d6*(u(i1,i2+1,i3,c)+u(
     & i1,i2-1,i3,c)) +c20lap2d6*(u(i1+2,i2,i3,c)+u(i1-2,i2,i3,c)) +
     & c02lap2d6*(u(i1,i2+2,i3,c)+u(i1,i2-2,i3,c)) +c30lap2d6*(u(i1+3,
     & i2,i3,c)+u(i1-3,i2,i3,c)) +c03lap2d6*(u(i1,i2+3,i3,c)+u(i1,i2-
     & 3,i3,c))
        lap3d6(i1,i2,i3,c)=c000lap3d6*u(i1,i2,i3,c) +c100lap3d6*(u(i1+
     & 1,i2,i3,c)+u(i1-1,i2,i3,c)) +c010lap3d6*(u(i1,i2+1,i3,c)+u(i1,
     & i2-1,i3,c)) +c001lap3d6*(u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c)) +
     & c200lap3d6*(u(i1+2,i2,i3,c)+u(i1-2,i2,i3,c)) +c020lap3d6*(u(i1,
     & i2+2,i3,c)+u(i1,i2-2,i3,c)) +c002lap3d6*(u(i1,i2,i3+2,c)+u(i1,
     & i2,i3-2,c)) +c300lap3d6*(u(i1+3,i2,i3,c)+u(i1-3,i2,i3,c)) +
     & c030lap3d6*(u(i1,i2+3,i3,c)+u(i1,i2-3,i3,c)) +c003lap3d6*(u(i1,
     & i2,i3+3,c)+u(i1,i2,i3-3,c))
        lap2d6Pow2(i1,i2,i3,c)=c00lap2d6*lap2d6(i1,i2,i3,c)+c10lap2d6*(
     & lap2d6(i1+1,i2,i3,c)+lap2d6(i1-1,i2,i3,c))+c01lap2d6*(lap2d6(
     & i1,i2+1,i3,c)+lap2d6(i1,i2-1,i3,c))+c20lap2d6*(lap2d6(i1+2,i2,
     & i3,c)+lap2d6(i1-2,i2,i3,c))+c02lap2d6*(lap2d6(i1,i2+2,i3,c)+
     & lap2d6(i1,i2-2,i3,c))+c30lap2d6*(lap2d6(i1+3,i2,i3,c)+lap2d6(
     & i1-3,i2,i3,c))+c03lap2d6*(lap2d6(i1,i2+3,i3,c)+lap2d6(i1,i2-3,
     & i3,c))
        lap3d6Pow2(i1,i2,i3,c)=c000lap3d6*lap3d6(i1,i2,i3,c)+
     & c100lap3d6*(lap3d6(i1+1,i2,i3,c)+lap3d6(i1-1,i2,i3,c))+
     & c010lap3d6*(lap3d6(i1,i2+1,i3,c)+lap3d6(i1,i2-1,i3,c))+
     & c001lap3d6*(lap3d6(i1,i2,i3+1,c)+lap3d6(i1,i2,i3-1,c))+
     & c200lap3d6*(lap3d6(i1+2,i2,i3,c)+lap3d6(i1-2,i2,i3,c))+
     & c020lap3d6*(lap3d6(i1,i2+2,i3,c)+lap3d6(i1,i2-2,i3,c))+
     & c002lap3d6*(lap3d6(i1,i2,i3+2,c)+lap3d6(i1,i2,i3-2,c))+
     & c300lap3d6*(lap3d6(i1+3,i2,i3,c)+lap3d6(i1-3,i2,i3,c))+
     & c030lap3d6*(lap3d6(i1,i2+3,i3,c)+lap3d6(i1,i2-3,i3,c))+
     & c003lap3d6*(lap3d6(i1,i2,i3+3,c)+lap3d6(i1,i2,i3-3,c))
       !     *** 8th order ***
        lap2d8(i1,i2,i3,c)=c00lap2d8*u(i1,i2,i3,c)      +c10lap2d8*(u(
     & i1+1,i2,i3,c)+u(i1-1,i2,i3,c))     +c01lap2d8*(u(i1,i2+1,i3,c)+
     & u(i1,i2-1,i3,c)) +c20lap2d8*(u(i1+2,i2,i3,c)+u(i1-2,i2,i3,c))  
     & +c02lap2d8*(u(i1,i2+2,i3,c)+u(i1,i2-2,i3,c)) +c30lap2d8*(u(i1+
     & 3,i2,i3,c)+u(i1-3,i2,i3,c))  +c03lap2d8*(u(i1,i2+3,i3,c)+u(i1,
     & i2-3,i3,c)) +c40lap2d8*(u(i1+4,i2,i3,c)+u(i1-4,i2,i3,c))  +
     & c04lap2d8*(u(i1,i2+4,i3,c)+u(i1,i2-4,i3,c))
        lap3d8(i1,i2,i3,c)=c000lap3d8*u(i1,i2,i3,c)      +c100lap3d8*(
     & u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c))     +c010lap3d8*(u(i1,i2+1,i3,
     & c)+u(i1,i2-1,i3,c)) +c001lap3d8*(u(i1,i2,i3+1,c)+u(i1,i2,i3-1,
     & c)) +c200lap3d8*(u(i1+2,i2,i3,c)+u(i1-2,i2,i3,c))  +c020lap3d8*
     & (u(i1,i2+2,i3,c)+u(i1,i2-2,i3,c)) +c002lap3d8*(u(i1,i2,i3+2,c)+
     & u(i1,i2,i3-2,c)) +c300lap3d8*(u(i1+3,i2,i3,c)+u(i1-3,i2,i3,c)) 
     &  +c030lap3d8*(u(i1,i2+3,i3,c)+u(i1,i2-3,i3,c)) +c003lap3d8*(u(
     & i1,i2,i3+3,c)+u(i1,i2,i3-3,c)) +c400lap3d8*(u(i1+4,i2,i3,c)+u(
     & i1-4,i2,i3,c))  +c040lap3d8*(u(i1,i2+4,i3,c)+u(i1,i2-4,i3,c)) +
     & c004lap3d8*(u(i1,i2,i3+4,c)+u(i1,i2,i3-4,c))
       ! ******* artificial dissipation ******
        du(i1,i2,i3,c)=u(i1,i2,i3,c)-um(i1,i2,i3,c)
       !      (2nd difference)
        fd22d(i1,i2,i3,c)= (     ( du(i1-1,i2,i3,c)+du(i1+1,i2,i3,c)+
     & du(i1,i2-1,i3,c)+du(i1,i2+1,i3,c) ) -4.*du(i1,i2,i3,c) )
       !
        fd23d(i1,i2,i3,c)=(     ( du(i1-1,i2,i3,c)+du(i1+1,i2,i3,c)+du(
     & i1,i2-1,i3,c)+du(i1,i2+1,i3,c)+du(i1,i2,i3-1,c)+du(i1,i2,i3+1,
     & c) ) -6.*du(i1,i2,i3,c) )
       !     -(fourth difference)
        fd42d(i1,i2,i3,c)= (    -( du(i1-2,i2,i3,c)+du(i1+2,i2,i3,c)+
     & du(i1,i2-2,i3,c)+du(i1,i2+2,i3,c) ) +4.*( du(i1-1,i2,i3,c)+du(
     & i1+1,i2,i3,c)+du(i1,i2-1,i3,c)+du(i1,i2+1,i3,c) ) -12.*du(i1,
     & i2,i3,c) )
       !
        fd43d(i1,i2,i3,c)=(    -( du(i1-2,i2,i3,c)+du(i1+2,i2,i3,c)+du(
     & i1,i2-2,i3,c)+du(i1,i2+2,i3,c)+du(i1,i2,i3-2,c)+du(i1,i2,i3+2,
     & c) ) +4.*( du(i1-1,i2,i3,c)+du(i1+1,i2,i3,c)+du(i1,i2-1,i3,c)+
     & du(i1,i2+1,i3,c)+du(i1,i2,i3-1,c)+du(i1,i2,i3+1,c) ) -18.*du(
     & i1,i2,i3,c) )
        ! (sixth  difference)
        fd62d(i1,i2,i3,c)= (     ( du(i1-3,i2,i3,c)+du(i1+3,i2,i3,c)+
     & du(i1,i2-3,i3,c)+du(i1,i2+3,i3,c) ) -6.*( du(i1-2,i2,i3,c)+du(
     & i1+2,i2,i3,c)+du(i1,i2-2,i3,c)+du(i1,i2+2,i3,c) ) +15.*( du(i1-
     & 1,i2,i3,c)+du(i1+1,i2,i3,c)+du(i1,i2-1,i3,c)+du(i1,i2+1,i3,c) )
     &  -40.*du(i1,i2,i3,c) )
        fd63d(i1,i2,i3,c)=(     ( du(i1-3,i2,i3,c)+du(i1+3,i2,i3,c)+du(
     & i1,i2-3,i3,c)+du(i1,i2+3,i3,c)+du(i1,i2,i3-3,c)+du(i1,i2,i3+3,
     & c) ) -6.*( du(i1-2,i2,i3,c)+du(i1+2,i2,i3,c)+du(i1,i2-2,i3,c)+
     & du(i1,i2+2,i3,c)+du(i1,i2,i3-2,c)+du(i1,i2,i3+2,c) ) +15.*( du(
     & i1-1,i2,i3,c)+du(i1+1,i2,i3,c)+du(i1,i2-1,i3,c)+du(i1,i2+1,i3,
     & c)+du(i1,i2,i3-1,c)+du(i1,i2,i3+1,c) ) -60.*du(i1,i2,i3,c) )
        ! -(eighth  difference)
        fd82d(i1,i2,i3,c)= (    -( du(i1-4,i2,i3,c)+du(i1+4,i2,i3,c)+
     & du(i1,i2-4,i3,c)+du(i1,i2+4,i3,c) ) +8.*( du(i1-3,i2,i3,c)+du(
     & i1+3,i2,i3,c)+du(i1,i2-3,i3,c)+du(i1,i2+3,i3,c) ) -28.*( du(i1-
     & 2,i2,i3,c)+du(i1+2,i2,i3,c)+du(i1,i2-2,i3,c)+du(i1,i2+2,i3,c) )
     &  +56.*( du(i1-1,i2,i3,c)+du(i1+1,i2,i3,c)+du(i1,i2-1,i3,c)+du(
     & i1,i2+1,i3,c) ) -140.*du(i1,i2,i3,c) )
        fd83d(i1,i2,i3,c)=(    -( du(i1-4,i2,i3,c)+du(i1+4,i2,i3,c)+du(
     & i1,i2-4,i3,c)+du(i1,i2+4,i3,c)+du(i1,i2,i3-4,c)+du(i1,i2,i3+4,
     & c) ) +8.*( du(i1-3,i2,i3,c)+du(i1+3,i2,i3,c)+du(i1,i2-3,i3,c)+
     & du(i1,i2+3,i3,c)+du(i1,i2,i3-3,c)+du(i1,i2,i3+3,c) ) -28.*( du(
     & i1-2,i2,i3,c)+du(i1+2,i2,i3,c)+du(i1,i2-2,i3,c)+du(i1,i2+2,i3,
     & c)+du(i1,i2,i3-2,c)+du(i1,i2,i3+2,c) ) +56.*( du(i1-1,i2,i3,c)+
     & du(i1+1,i2,i3,c)+du(i1,i2-1,i3,c)+du(i1,i2+1,i3,c)+du(i1,i2,i3-
     & 1,c)+du(i1,i2,i3+1,c) ) -210.*du(i1,i2,i3,c) )
       !     **** Modified equation method: ****
        maxwell2dr44me(i1,i2,i3,n)=2.*u(i1,i2,i3,n)-um(i1,i2,i3,n)+
     & cdtsq*lap2d4(i1,i2,i3,n)+cdtsq12*lap2d2Pow2(i1,i2,i3,n)
        maxwell3dr44me(i1,i2,i3,n)=2.*u(i1,i2,i3,n)-um(i1,i2,i3,n)+
     & cdtsq*lap3d4(i1,i2,i3,n)+cdtsq12*lap3d2Pow2(i1,i2,i3,n)
        maxwell2dr66me(i1,i2,i3,n)=2.*u(i1,i2,i3,n)-um(i1,i2,i3,n)+
     & cdtsq*lap2d6(i1,i2,i3,n)+cdtsq12  *lap2d4Pow2(i1,i2,i3,n)+
     & cdt4by360*lap2d2Pow3(i1,i2,i3,n)
        maxwell3dr66me(i1,i2,i3,n)=2.*u(i1,i2,i3,n)-um(i1,i2,i3,n)+
     & cdtsq*lap3d6(i1,i2,i3,n)+cdtsq12*  lap3d4Pow2(i1,i2,i3,n)+
     & cdt4by360*lap3d2Pow3(i1,i2,i3,n)
        maxwell2dr88me(i1,i2,i3,n)=2.*u(i1,i2,i3,n)-um(i1,i2,i3,n)+
     & cdtsq*lap2d8(i1,i2,i3,n)+cdtsq12*lap2d6Pow2(i1,i2,i3,n)+
     & cdt4by360*lap2d4Pow3(i1,i2,i3,n)+cdt6by20160*lap2d2Pow4(i1,i2,
     & i3,n)
        maxwell3dr88me(i1,i2,i3,n)=2.*u(i1,i2,i3,n)-um(i1,i2,i3,n)+
     & cdtsq*lap3d8(i1,i2,i3,n)+cdtsq12*lap3d6Pow2(i1,i2,i3,n)+
     & cdt4by360*lap3d4Pow3(i1,i2,i3,n)+cdt6by20160*lap3d2Pow4(i1,i2,
     & i3,n)
       !    -- forcing correction for modified equation method ---
       !        RHS = f + (dt^2/12)*( c^2 * Delta f + f_tt )
       !  Approximate the term in brackets to 2nd-order
       ! ---- Cartesian grids:
       ! f2drme44(i1,i2,i3,n) = fa(i1,i2,i3,n,fcur)
       ! f2drme44(i1,i2,i3,n) = f(i1,i2,i3,n)
       f2drme44(i1,i2,i3,n) = fa(i1,i2,i3,n,fcur)+cdtSqBy12*lap2d2f(i1,
     & i2,i3,n,fcur) +(fa(i1,i2,i3,n,fnext)-2.*fa(i1,i2,i3,n,fcur)+fa(
     & i1,i2,i3,n,fprev))/(12.)
       f3drme44(i1,i2,i3,n) = fa(i1,i2,i3,n,fcur)+cdtSqBy12*lap3d2f(i1,
     & i2,i3,n,fcur) +(fa(i1,i2,i3,n,fnext)-2.*fa(i1,i2,i3,n,fcur)+fa(
     & i1,i2,i3,n,fprev))/(12.)
       ! ---- Curvilinear grids
       ff(i1,i2,i3,n) = fa(i1,i2,i3,n,fcur)
       f2dcme44(i1,i2,i3,n) = fa(i1,i2,i3,n,fcur)+cdtSqBy12*
     & ffLaplacian22(i1,i2,i3,n) +(fa(i1,i2,i3,n,fnext)-2.*fa(i1,i2,
     & i3,n,fcur)+fa(i1,i2,i3,n,fprev))/(12.)
       f3dcme44(i1,i2,i3,n) = fa(i1,i2,i3,n,fcur)+cdtSqBy12*
     & ffLaplacian23(i1,i2,i3,n) +(fa(i1,i2,i3,n,fnext)-2.*fa(i1,i2,
     & i3,n,fcur)+fa(i1,i2,i3,n,fprev))/(12.)
        ! f  = csq*Lap4(u)+f,  v= (csq*Lap2)**2
        maxwellc44me(i1,i2,i3,n)=2.*u(i1,i2,i3,n)-um(i1,i2,i3,n)+dtsq*
     & f(i1,i2,i3,n)+dt4by12*v(I1,I2,I3,n)
        ! these next are only valid for second order accuracy in time:
        maxwellc66me(i1,i2,i3,n)=2.*u(i1,i2,i3,n)-um(i1,i2,i3,n)+dtsq*
     & f(i1,i2,i3,n)
        maxwellc88me(i1,i2,i3,n)=2.*u(i1,i2,i3,n)-um(i1,i2,i3,n)+dtsq*
     & f(i1,i2,i3,n)
        ! for non-conservative modified-equation:
        max2dc44me(i1,i2,i3,n)=2.*u(i1,i2,i3,n)-um(i1,i2,i3,n)+cdtsq*
     & uLaplacian42(i1,i2,i3,n)+cdtsq12*uLapSq(n)
        ! This version for the 2-stage computation:
       !$$$ vr2(i1,i2,i3,kd)=(v(i1+1,i2,i3,kd)-v(i1-1,i2,i3,kd))*d12(0)
       !$$$ vs2(i1,i2,i3,kd)=(v(i1,i2+1,i3,kd)-v(i1,i2-1,i3,kd))*d12(1)
       !$$$
       !$$$ vrr2(i1,i2,i3,kd)=(-2.*v(i1,i2,i3,kd)+(v(i1+1,i2,i3,kd)+v(i1-1,i2,i3,kd)) )*d22(0)
       !$$$ vss2(i1,i2,i3,kd)=(-2.*v(i1,i2,i3,kd)+(v(i1,i2+1,i3,kd)+v(i1,i2-1,i3,kd)) )*d22(1)
       !$$$ vrs2(i1,i2,i3,kd)=(vr2(i1,i2+1,i3,kd)-vr2(i1,i2-1,i3,kd))*d12(1)
       !$$$
       !$$$ vlaplacian22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*!$$$      vrr2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*!$$$      sy(i1,i2,i3))*vrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)**!$$$      2)*vss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3)+ryy22(i1,i2,i3))*vr2(i1,!$$$      i2,i3,kd)+(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*vs2(i1,i2,i3,kd)
        max2dc44me2(i1,i2,i3,n)=2.*u(i1,i2,i3,n)-um(i1,i2,i3,n)+cdtsq*
     & uLaplacian42(i1,i2,i3,n)+cdtsq12*vLaplacian22(i1,i2,i3,n)
        max3dc44me(i1,i2,i3,n)=2.*u(i1,i2,i3,n)-um(i1,i2,i3,n)+cdtsq*
     & uLaplacian43(i1,i2,i3,n)+cdtsq12*vLaplacian23(i1,i2,i3,n)
        ! 2D, 4th-order, div cleaning:
        ! We could further optimize the D0x(lap2d2) ...
       !!$ mxdc2d4Ex(i1,i2,i3) = cdc0*u(i1,i2,i3,ex)+cdc1*um(i1,i2,i3,ex)!!$            +cdcLap*lap2d4(i1,i2,i3,ex)!!$            +cdcLapsq*lap2d2Pow2(i1,i2,i3,ex)!!$            +cdcHz*uy42r(i1,i2,i3,hz)!!$            +cdcHzyLap*( lap2d2(i1,i2+1,i3,hz)-lap2d2(i1,i2-1,i3,hz) )
       !!$
       !!$ mxdc2d4Ey(i1,i2,i3) = cdc0*u(i1,i2,i3,ey)+cdc1*um(i1,i2,i3,ey)!!$            +cdcLap*lap2d4(i1,i2,i3,ey)!!$            +cdcLapsq*lap2d2Pow2(i1,i2,i3,ey)!!$            -cdcHz*ux42r(i1,i2,i3,hz)!!$            -cdcHzxLap*( lap2d2(i1+1,i2,i3,hz)-lap2d2(i1-1,i2,i3,hz) )
        ! 2D, 4th-order, rectangular, div cleaning:
        ! new version : here we replace curl( Delta H ) by an approx. to E_ttt that uses Delta E_t 
        mxdc2d4Ex(i1,i2,i3) = cdc0*u(i1,i2,i3,ex)+cdc1*um(i1,i2,i3,ex)+
     & cdcELap*lap2d4(i1,i2,i3,ex)+cdcELapsq*lap2d2Pow2(i1,i2,i3,ex)+
     & cdcE*uy42r(i1,i2,i3,hz)+cdcELapm*( lap2d2m(i1,i2,i3,ex) )
        mxdc2d4Ey(i1,i2,i3) = cdc0*u(i1,i2,i3,ey)+cdc1*um(i1,i2,i3,ey)+
     & cdcELap*lap2d4(i1,i2,i3,ey)+cdcELapsq*lap2d2Pow2(i1,i2,i3,ey)-
     & cdcE*ux42r(i1,i2,i3,hz)+cdcELapm*( lap2d2m(i1,i2,i3,ey) )
        ! 2d, 4th order, curvilinear (conservative), div cleaning (f=Lap(E), v=Lapsq(E))
        mxdc2d4cConsEx(i1,i2,i3) = cdc0*u(i1,i2,i3,ex)+cdc1*um(i1,i2,
     & i3,ex)+cdcELap*f(i1,i2,i3,ex)+cdcELapsq*v(i1,i2,i3,ex)+cdcE*
     & uy42(i1,i2,i3,hz)+cdcELapm*( umLaplacian22(i1,i2,i3,ex) )
        mxdc2d4cConsEy(i1,i2,i3) = cdc0*u(i1,i2,i3,ey)+cdc1*um(i1,i2,
     & i3,ey)+cdcELap*f(i1,i2,i3,ey)+cdcELapsq*v(i1,i2,i3,ey)-cdcE*
     & ux42(i1,i2,i3,hz)+cdcELapm*( umLaplacian22(i1,i2,i3,ey) )
        ! 2D, 4th-order, curvilinear, div cleaning: **check me**
        mxdc2d4cEx(i1,i2,i3) = cdc0*u(i1,i2,i3,ex)+cdc1*um(i1,i2,i3,ex)
     & +cdcELap*uLaplacian42(i1,i2,i3,ex)+cdcELapsq*vLaplacian22(i1,
     & i2,i3,ex)+cdcE*uy42(i1,i2,i3,hz)+cdcELapm*( umLaplacian22(i1,
     & i2,i3,ex) )
        mxdc2d4cEy(i1,i2,i3) = cdc0*u(i1,i2,i3,ey)+cdc1*um(i1,i2,i3,ey)
     & +cdcELap*uLaplacian42(i1,i2,i3,ey)+cdcELapsq*vLaplacian22(i1,
     & i2,i3,ey)-cdcE*ux42(i1,i2,i3,hz)+cdcELapm*( umLaplacian22(i1,
     & i2,i3,ey) )
       !...........end   statement functions
        ! write(*,*) 'Inside advMaxwell...'
        cc    =rpar(0)  ! this is c
        dt    =rpar(1)
        dx(0) =rpar(2)
        dx(1) =rpar(3)
        dx(2) =rpar(4)
        adc   =rpar(5)  ! coefficient of artificial dissipation
        add   =rpar(6)  ! coefficient of divergence damping
        dr(0) =rpar(7)
        dr(1) =rpar(8)
        dr(2) =rpar(9)
        eps   =rpar(10)
        mu    =rpar(11)
        kx    =rpar(12)
        ky    =rpar(13)
        kz    =rpar(14)
        sigmaE=rpar(15)  ! electric conductivity (for lossy materials, complex index of refraction)
        sigmaH=rpar(16)  ! magnetic conductivity
        divergenceCleaningCoefficient=rpar(17)
        t     =rpar(18)
        rpar(20)=0.  ! return the time used for adding dissipation
        dy=dx(1)  ! Are these needed?
        dz=dx(2)
        ! timeForArtificialDissipation=rpar(6) ! return value
        option             =ipar(0)
        gridType           =ipar(1)
        orderOfAccuracy    =ipar(2)
        orderInTime        =ipar(3)
        addForcing         =ipar(4)
        orderOfDissipation =ipar(5)
        ex                 =ipar(6)
        ey                 =ipar(7)
        ez                 =ipar(8)
        hx                 =ipar(9)
        hy                 =ipar(10)
        hz                 =ipar(11)
        solveForE          =ipar(12)
        solveForH          =ipar(13)
        useWhereMask       =ipar(14)
        timeSteppingMethod =ipar(15)
        useVariableDissipation        =ipar(16)
        useCurvilinearOpt             =ipar(17)
        useConservative               =ipar(18)
        combineDissipationWithAdvance =ipar(19)
        useDivergenceCleaning         =ipar(20)
        useNewForcingMethod           =ipar(21)
        numberOfForcingFunctions      =ipar(22)
        fcur                          =ipar(23)
        dispersionModel     =ipar(24)
        pxc                 =ipar(25)
        pyc                 =ipar(26)
        pzc                 =ipar(27)
        qxc                 =ipar(28)
        qyc                 =ipar(29)
        qzc                 =ipar(30)
        rxc                 =ipar(31)
        ryc                 =ipar(32)
        rzc                 =ipar(33)
        fprev = mod(fcur-1+numberOfForcingFunctions,max(1,
     & numberOfForcingFunctions))
        fnext = mod(fcur+1                         ,max(1,
     & numberOfForcingFunctions))
        ! addDissipation=.true. if we add the dissipation in the dis(i1,i2,i3,c) array
        !  if combineDissipationWithAdvance.ne.0 we compute the dissipation on the fly in the time step
        !  rather than pre-computing it in diss(i1,i2,i3,c)
        addDissipation = adc.gt.0. .and. 
     & combineDissipationWithAdvance.eq.0
        adcdt=adc*dt
        csq=cc**2
        dtsq=dt**2
        cdt=cc*dt
        cdtsq=(cc**2)*(dt**2)
        cdtsq12=cdtsq*cdtsq/12.  ! c^4 dt^4 /14
        cdt4by360=(cdt)**4/360.
        cdt6by20160=cdt**6/(8.*7.*6.*5.*4.*3.)
        cdtSqBy12= cdtsq/12.   ! c^2*dt*2/12
        dt4by12=dtsq*dtsq/12.
        cdtdx = (cc*dt/dx(0))**2
        cdtdy = (cc*dt/dy)**2
        cdtdz = (cc*dt/dz)**2
        dxsqi=1./(dx(0)**2)
        dysqi=1./(dy**2)
        dzsqi=1./(dz**2)
        dxsq12i=1./(12.*dx(0)**2)
        dysq12i=1./(12.*dy**2)
        dzsq12i=1./(12.*dz**2)
        dxi4=1./(dx(0)**4)
        dyi4=1./(dy**4)
        dxdyi2=1./(dx(0)*dx(0)*dy*dy)
        dzi4=1./(dz**4)
        dxdzi2=1./(dx(0)*dx(0)*dz*dz)
        dydzi2=1./(dy*dy*dz*dz)
        if( t.eq.0. .and. dispersionModel.ne.noDispersion )then
           write(*,'("--advOpt-- dispersionModel=",i4," px,py,pz=",3i2)
     & ') dispersionModel,pxc,pyc,pzc
        end if
        if( useDivergenceCleaning.eq.1 )then
          ! Here are the coefficients that define the div cleaning formulae
          !    D+tD-t( E ) + alpha*( D0t E ) = c^2 Delta(E) + alpha*( (1/eps) Curl ( H ) )
          if( orderOfAccuracy.eq.2 )then
            ! 2D, 2nd-order, div cleaning:
            dc = divergenceCleaningCoefficient
            dcp = 1. + dc*dt*.5
            cdc0 = 2./dcp
            cdc1 = -(1.-dc*dt*.5)/dcp
            cdcxx = (cc*dt)**2/(dx(0)**2)/dcp
            cdcyy = (cc*dt)**2/(dx(1)**2)/dcp
            cdczz = (cc*dt)**2/(dx(2)**2)/dcp
            ! for div(H) damping in E eqn:
            cdcEdx= dc*dt**2/(eps*2.*dx(0))/dcp
            cdcEdy= dc*dt**2/(eps*2.*dx(1))/dcp
            cdcEdz= dc*dt**2/(eps*2.*dx(2))/dcp
            ! for div(E) damping in H eqn:
            cdcHdx= dc*dt**2/(mu*2.*dx(0))/dcp
            cdcHdy= dc*dt**2/(mu*2.*dx(1))/dcp
            cdcHdz= dc*dt**2/(mu*2.*dx(2))/dcp
            ! These next two are for the curvilinear case:
            cdcf = (cc*dt)**2/dcp
            cdcE = dc*dt**2/(eps)/dcp
            cdcH = dc*dt**2/(mu )/dcp
            if( t.eq.0. )then
              write(*,'(" advOpt: order=2 : div clean: dc,cc,dt,eps,
     & mu=",5e10.2)') dc,cc,dt,eps,mu
              write(*,'(" advOpt: div clean: cdc0,cdc1,cdcxx,cdcyy,
     & cdcHdy,cdcHdx=",6e10.2)') cdc0,cdc1,cdcxx,cdcyy,cdcHdy,cdcHdx
            end if
          else if( orderOfAccuracy.eq.4 )then
            dc = divergenceCleaningCoefficient
            dcp = 1. + dc*dt*.5
            cdc0 = 2./dcp
            cdc1 = -(1.-dc*dt*.5)/dcp
            cdcE= dc*dt**2/(eps)/dcp
            cdcELap= ((cc*dt)**2/dcp)*( 1. + dc*dt/(6.*eps) )
            cdcELapsq = ((cc*dt)**4/12./dcp)*( 1. + dc*dt/eps )
            cdcELapm = ((cc*dt)**2/dcp)*( - dc*dt/(6.*eps) )
            cdcH= dc*dt**2/(mu )/dcp
            cdcHLap= ((cc*dt)**2/dcp)*( 1. + dc*dt/(6.*mu) )
            cdcHLapsq = ((cc*dt)**4/12./dcp)*( 1. + dc*dt/mu )
            cdcHLapm = ((cc*dt)**2/dcp)*( - dc*dt/(6.*mu ) )
            if( t.eq.0. )then
              write(*,'(" advOpt: order=4 :  div clean: dc,cc,dt,eps,
     & mu=",5e10.2)') dc,cc,dt,eps,mu
              write(*,'(" advOpt: div clean: cdc0,cdc1,cdcELap,
     & cdcELapsq,cdcE,cdcELapm=",8e10.2)') cdc0,cdc1,cdcELap,
     & cdcELapsq,cdcE,cdcELapm
            end if
          else
           write(*,'(" advOpt.bf: un-implemented orderOfAccuracy for 
     & div-cleaning")')
           stop 2277
          end if
        end if
        if( orderOfAccuracy.eq.6 )then
          if( nd.eq.2 )then
            c00lap2d6=csq*(-49./18.)*(1./dx(0)**2+1./dy**2)
            c10lap2d6=csq*(1.5     )*(1./dx(0)**2)
            c01lap2d6=csq*(1.5     )*(1./dy**2)
            c20lap2d6=csq*(-3./20. )*(1./dx(0)**2)
            c02lap2d6=csq*(-3./20. )*(1./dy**2)
            c30lap2d6=csq*(1./90.  )*(1./dx(0)**2)
            c03lap2d6=csq*(1./90.  )*(1./dy**2)
          else
            c000lap3d6=csq*(-49./18.)*(1./dx(0)**2+1./dy**2+1./dz**2)
            c100lap3d6=csq*(1.5     )*(1./dx(0)**2)
            c010lap3d6=csq*(1.5     )*(1./dy**2)
            c001lap3d6=csq*(1.5     )*(1./dz**2)
            c200lap3d6=csq*(-3./20. )*(1./dx(0)**2)
            c020lap3d6=csq*(-3./20. )*(1./dy**2)
            c002lap3d6=csq*(-3./20. )*(1./dz**2)
            c300lap3d6=csq*(1./90.  )*(1./dx(0)**2)
            c030lap3d6=csq*(1./90.  )*(1./dy**2)
            c003lap3d6=csq*(1./90.  )*(1./dz**2)
          end if
        end if
        if( orderOfAccuracy.eq.8 )then
          if( nd.eq.2 )then
            c00lap2d8=csq*(-205./72.)*(1./dx(0)**2+1./dy**2)
            c10lap2d8=csq*(8./5.    )*(1./dx(0)**2)
            c01lap2d8=csq*(8./5.    )*(1./dy**2)
            c20lap2d8=csq*(-1./5.   )*(1./dx(0)**2)
            c02lap2d8=csq*(-1./5.   )*(1./dy**2)
            c30lap2d8=csq*(8./315.  )*(1./dx(0)**2)
            c03lap2d8=csq*(8./315.  )*(1./dy**2)
            c40lap2d8=csq*(-1./560. )*(1./dx(0)**2)
            c04lap2d8=csq*(-1./560. )*(1./dy**2)
          else
            c000lap3d8=csq*(-205./72.)*(1./dx(0)**2+1./dy**2+1./dz**2)
            c100lap3d8=csq*(8./5.    )*(1./dx(0)**2)
            c010lap3d8=csq*(8./5.    )*(1./dy**2)
            c001lap3d8=csq*(8./5.    )*(1./dz**2)
            c200lap3d8=csq*(-1./5.   )*(1./dx(0)**2)
            c020lap3d8=csq*(-1./5.   )*(1./dy**2)
            c002lap3d8=csq*(-1./5.   )*(1./dz**2)
            c300lap3d8=csq*(8./315.  )*(1./dx(0)**2)
            c030lap3d8=csq*(8./315.  )*(1./dy**2)
            c003lap3d8=csq*(8./315.  )*(1./dz**2)
            c400lap3d8=csq*(-1./560. )*(1./dx(0)**2)
            c040lap3d8=csq*(-1./560. )*(1./dy**2)
            c004lap3d8=csq*(-1./560. )*(1./dz**2)
          end if
        end if
        if( orderInTime.eq.4 )then
          c40=( 7./6. )*dtsq
          c41=(-5./12.)*dtsq
          c42=( 1./3. )*dtsq
          c43=(-1./12.)*dtsq
        else if( orderInTime.eq.6 )then
          c60=( 317./240.)*dtsq    ! from stoermer.maple
          c61=(-266./240.)*dtsq
          c62=( 374./240.)*dtsq
          c63=(-276./240.)*dtsq
          c64=( 109./240.)*dtsq
          c65=( -18./240.)*dtsq
        else if( orderInTime.eq.8 )then
       !     g := 1/60480 (236568 fv[4] + 88324 fv[0] - 121797 fv[1] + 245598 fv[2] 
       !     + 33190 fv[6] - 4125 fv[7] - 300227 fv[3] - 117051 fv[5])
          c80=(  88324./60480.)*dtsq ! from stoermer.maple
          c81=(-121797./60480.)*dtsq
          c82=( 245598./60480.)*dtsq
          c83=(-300227./60480.)*dtsq
          c84=( 236568./60480.)*dtsq
          c85=(-117051./60480.)*dtsq
          c86=(  33190./60480.)*dtsq
          c87=(  -4125./60480.)*dtsq
        end if
         ! This next function will:
         !   (1) optionally compute the dissipation and fill in the diss array 
         !            if: (adc.gt.0. .and. combineDissipationWithAdvance.eq.0
         !   (2) add the divergence damping
         !         if( add.gt.0. )
        if( nd.eq.2 .and. orderOfAccuracy.eq.2 )then
          call advMxDiss2dOrder2(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rsxy,  um,u,un,f, v,vvt2,
     & ut3,vvt4,ut5,ut6,ut7, bc, dis, varDis, ipar, rpar, ierr )
        else if(  nd.eq.2 .and. orderOfAccuracy.eq.4 )then
          call advMxDiss2dOrder4(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rsxy,  um,u,un,f, v,vvt2,
     & ut3,vvt4,ut5,ut6,ut7, bc, dis, varDis, ipar, rpar, ierr )
        else if( nd.eq.3 .and. orderOfAccuracy.eq.2 )then
          call advMxDiss3dOrder2(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rsxy,  um,u,un,f, v,vvt2,
     & ut3,vvt4,ut5,ut6,ut7, bc, dis, varDis, ipar, rpar, ierr )
        else if(  nd.eq.3 .and. orderOfAccuracy.eq.4 )then
          call advMxDiss3dOrder4(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rsxy,  um,u,un,f, v,vvt2,
     & ut3,vvt4,ut5,ut6,ut7, bc, dis, varDis, ipar, rpar, ierr )
        else
          if( (adc.gt.0. .and. combineDissipationWithAdvance.eq.0) 
     & .or. add.gt.0. )then
            stop 1116
          end if
        end if
        if( option.eq.1 ) then
          return
        end if
       ! write(*,'(" advMaxwell: timeSteppingMethod=",i2)') timeSteppingMethod
        if( timeSteppingMethod.eq.defaultTimeStepping )then
         write(*,'(" advMaxwell:ERROR: 
     & timeSteppingMethod=defaultTimeStepping -- this should be set")
     & ')
           ! '
         stop 83322
        end if
        if( gridType.eq.rectangular )then
        else
       !       **********************************************
       !       *************** curvilinear ******************
       !       **********************************************
          if( useCurvilinearOpt.eq.1 .and. useConservative.eq.0 )then
           ! **************************************************
           ! *************** NON-CONSERVATIVE *****************    
           ! **************************************************
       !$$$     loopsFCD(un(i1,i2,i3,ex)=maxwellc22(i1,i2,i3,ex),!$$$              un(i1,i2,i3,ey)=maxwellc22(i1,i2,i3,ey),!$$$              un(i1,i2,i3,ez)=maxwellc22(i1,i2,i3,ez),!$$$               ,,,!$$$              un(i1,i2,i3,hx)=maxwellc22(i1,i2,i3,hx),!$$$              un(i1,i2,i3,hy)=maxwellc22(i1,i2,i3,hy),!$$$              un(i1,i2,i3,hz)=maxwellc22(i1,i2,i3,hz),!$$$              ,,)
            stop 88044
          else if( useCurvilinearOpt.eq.1 .and. useConservative.eq.1 )
     & then
           ! *************** conservative *****************    
           stop 94422
          else
            ! *****************************************************
            ! ****************Old way******************************
            ! *****************************************************
            ! **************** CONSERVATIVE DIFFERENCE *****************
            !  The Lapacian and Laplacian squared have already been computed by the calling program 
          ! In these cases we are given the Laplacian on input
           if( useDivergenceCleaning.eq.0 )then
            if( .not.addDissipation )then
             if( nd.eq.2 )then
              ! This next line assumes we solve for ex,ey and hz
              if( useWhereMask.ne.0 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                if( mask(i1,i2,i3).gt.0 )then
                 un(i1,i2,i3,ex)=maxwellc22(i1,i2,i3,ex)
                 un(i1,i2,i3,ey)=maxwellc22(i1,i2,i3,ey)


                 un(i1,i2,i3,hz)=maxwellc22(i1,i2,i3,hz)




                end if
               end do
               end do
               end do
              else
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                un(i1,i2,i3,ex)=maxwellc22(i1,i2,i3,ex)
                un(i1,i2,i3,ey)=maxwellc22(i1,i2,i3,ey)


                un(i1,i2,i3,hz)=maxwellc22(i1,i2,i3,hz)




               end do
               end do
               end do
              end if
             else
              if( solveForE.ne.0 .and. solveForH.ne.0 )then
                if( useWhereMask.ne.0 )then
                  do i3=n3a,n3b
                  do i2=n2a,n2b
                  do i1=n1a,n1b
                    if( mask(i1,i2,i3).gt.0 )then
                      un(i1,i2,i3,ex)=maxwellc22(i1,i2,i3,ex)
                      un(i1,i2,i3,ey)=maxwellc22(i1,i2,i3,ey)
                      un(i1,i2,i3,ez)=maxwellc22(i1,i2,i3,ez)



                      un(i1,i2,i3,hx)=maxwellc22(i1,i2,i3,hx)
                      un(i1,i2,i3,hy)=maxwellc22(i1,i2,i3,hy)
                      un(i1,i2,i3,hz)=maxwellc22(i1,i2,i3,hz)









                    end if
                  end do
                  end do
                  end do
                else
                  do i3=n3a,n3b
                  do i2=n2a,n2b
                  do i1=n1a,n1b
                      un(i1,i2,i3,ex)=maxwellc22(i1,i2,i3,ex)
                      un(i1,i2,i3,ey)=maxwellc22(i1,i2,i3,ey)
                      un(i1,i2,i3,ez)=maxwellc22(i1,i2,i3,ez)



                      un(i1,i2,i3,hx)=maxwellc22(i1,i2,i3,hx)
                      un(i1,i2,i3,hy)=maxwellc22(i1,i2,i3,hy)
                      un(i1,i2,i3,hz)=maxwellc22(i1,i2,i3,hz)









                  end do
                  end do
                  end do
                end if
              else if( solveForE.ne.0 ) then
                if( useWhereMask.ne.0 )then
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                  if( mask(i1,i2,i3).gt.0 )then
                   un(i1,i2,i3,ex)=maxwellc22(i1,i2,i3,ex)
                   un(i1,i2,i3,ey)=maxwellc22(i1,i2,i3,ey)
                   un(i1,i2,i3,ez)=maxwellc22(i1,i2,i3,ez)






                  end if
                 end do
                 end do
                 end do
                else
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                  un(i1,i2,i3,ex)=maxwellc22(i1,i2,i3,ex)
                  un(i1,i2,i3,ey)=maxwellc22(i1,i2,i3,ey)
                  un(i1,i2,i3,ez)=maxwellc22(i1,i2,i3,ez)






                 end do
                 end do
                 end do
                end if
              else
                if( useWhereMask.ne.0 )then
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                  if( mask(i1,i2,i3).gt.0 )then
                   un(i1,i2,i3,hx)=maxwellc22(i1,i2,i3,hx)
                   un(i1,i2,i3,hy)=maxwellc22(i1,i2,i3,hy)
                   un(i1,i2,i3,hz)=maxwellc22(i1,i2,i3,hz)






                  end if
                 end do
                 end do
                 end do
                else
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                  un(i1,i2,i3,hx)=maxwellc22(i1,i2,i3,hx)
                  un(i1,i2,i3,hy)=maxwellc22(i1,i2,i3,hy)
                  un(i1,i2,i3,hz)=maxwellc22(i1,i2,i3,hz)






                 end do
                 end do
                 end do
                end if
              end if
             end if
            else ! add dissipation too
             if( nd.eq.2 )then
              ! This next line assumes we solve for ex,ey and hz
              if( useWhereMask.ne.0 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                if( mask(i1,i2,i3).gt.0 )then
                 un(i1,i2,i3,ex)=maxwellc22(i1,i2,i3,ex)+dis(i1,i2,i3,
     & ex)
                 un(i1,i2,i3,ey)=maxwellc22(i1,i2,i3,ey)+dis(i1,i2,i3,
     & ey)


                 un(i1,i2,i3,hz)=maxwellc22(i1,i2,i3,hz)+dis(i1,i2,i3,
     & hz)




                end if
               end do
               end do
               end do
              else
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                un(i1,i2,i3,ex)=maxwellc22(i1,i2,i3,ex)+dis(i1,i2,i3,
     & ex)
                un(i1,i2,i3,ey)=maxwellc22(i1,i2,i3,ey)+dis(i1,i2,i3,
     & ey)


                un(i1,i2,i3,hz)=maxwellc22(i1,i2,i3,hz)+dis(i1,i2,i3,
     & hz)




               end do
               end do
               end do
              end if
             else
              if( solveForE.ne.0 .and. solveForH.ne.0 )then
                if( useWhereMask.ne.0 )then
                  do i3=n3a,n3b
                  do i2=n2a,n2b
                  do i1=n1a,n1b
                    if( mask(i1,i2,i3).gt.0 )then
                      un(i1,i2,i3,ex)=maxwellc22(i1,i2,i3,ex)+dis(i1,
     & i2,i3,ex)
                      un(i1,i2,i3,ey)=maxwellc22(i1,i2,i3,ey)+dis(i1,
     & i2,i3,ey)
                      un(i1,i2,i3,ez)=maxwellc22(i1,i2,i3,ez)+dis(i1,
     & i2,i3,ez)



                      un(i1,i2,i3,hx)=maxwellc22(i1,i2,i3,hx)+dis(i1,
     & i2,i3,hx)
                      un(i1,i2,i3,hy)=maxwellc22(i1,i2,i3,hy)+dis(i1,
     & i2,i3,hy)
                      un(i1,i2,i3,hz)=maxwellc22(i1,i2,i3,hz)+dis(i1,
     & i2,i3,hz)









                    end if
                  end do
                  end do
                  end do
                else
                  do i3=n3a,n3b
                  do i2=n2a,n2b
                  do i1=n1a,n1b
                      un(i1,i2,i3,ex)=maxwellc22(i1,i2,i3,ex)+dis(i1,
     & i2,i3,ex)
                      un(i1,i2,i3,ey)=maxwellc22(i1,i2,i3,ey)+dis(i1,
     & i2,i3,ey)
                      un(i1,i2,i3,ez)=maxwellc22(i1,i2,i3,ez)+dis(i1,
     & i2,i3,ez)



                      un(i1,i2,i3,hx)=maxwellc22(i1,i2,i3,hx)+dis(i1,
     & i2,i3,hx)
                      un(i1,i2,i3,hy)=maxwellc22(i1,i2,i3,hy)+dis(i1,
     & i2,i3,hy)
                      un(i1,i2,i3,hz)=maxwellc22(i1,i2,i3,hz)+dis(i1,
     & i2,i3,hz)









                  end do
                  end do
                  end do
                end if
              else if( solveForE.ne.0 ) then
                if( useWhereMask.ne.0 )then
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                  if( mask(i1,i2,i3).gt.0 )then
                   un(i1,i2,i3,ex)=maxwellc22(i1,i2,i3,ex)+dis(i1,i2,
     & i3,ex)
                   un(i1,i2,i3,ey)=maxwellc22(i1,i2,i3,ey)+dis(i1,i2,
     & i3,ey)
                   un(i1,i2,i3,ez)=maxwellc22(i1,i2,i3,ez)+dis(i1,i2,
     & i3,ez)






                  end if
                 end do
                 end do
                 end do
                else
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                  un(i1,i2,i3,ex)=maxwellc22(i1,i2,i3,ex)+dis(i1,i2,i3,
     & ex)
                  un(i1,i2,i3,ey)=maxwellc22(i1,i2,i3,ey)+dis(i1,i2,i3,
     & ey)
                  un(i1,i2,i3,ez)=maxwellc22(i1,i2,i3,ez)+dis(i1,i2,i3,
     & ez)






                 end do
                 end do
                 end do
                end if
              else
                if( useWhereMask.ne.0 )then
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                  if( mask(i1,i2,i3).gt.0 )then
                   un(i1,i2,i3,hx)=maxwellc22(i1,i2,i3,hx)+dis(i1,i2,
     & i3,hx)
                   un(i1,i2,i3,hy)=maxwellc22(i1,i2,i3,hy)+dis(i1,i2,
     & i3,hy)
                   un(i1,i2,i3,hz)=maxwellc22(i1,i2,i3,hz)+dis(i1,i2,
     & i3,hz)






                  end if
                 end do
                 end do
                 end do
                else
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                  un(i1,i2,i3,hx)=maxwellc22(i1,i2,i3,hx)+dis(i1,i2,i3,
     & hx)
                  un(i1,i2,i3,hy)=maxwellc22(i1,i2,i3,hy)+dis(i1,i2,i3,
     & hy)
                  un(i1,i2,i3,hz)=maxwellc22(i1,i2,i3,hz)+dis(i1,i2,i3,
     & hz)






                 end do
                 end do
                 end do
                end if
              end if
             end if
            end if
           else
              ! 2D, 2nd-order, curvilinear, div cleaning:
              !    D+tD-t( E ) + alpha*( D0t E ) = c^2 Delta(E) + alpha*( (1/eps) Curl ( H ) )
             write(*,'("advMaxwell: 2D, 2nd-order, curv, div 
     & cleaning... t=",e10.2,", adcdt=",e10.2 )') t,adcdt
             if( .not.addDissipation )then
              if( nd.eq.2 )then
               ! This next line assumes we solve for ex,ey and hz
               if( useWhereMask.ne.0 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                 if( mask(i1,i2,i3).gt.0 )then
                  un(i1,i2,i3,ex)=mxdc2d2cEx(i1,i2,i3)
                  un(i1,i2,i3,ey)=mxdc2d2cEy(i1,i2,i3)


                  un(i1,i2,i3,hz)=maxwellc22(i1,i2,i3,hz)




                 end if
                end do
                end do
                end do
               else
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                 un(i1,i2,i3,ex)=mxdc2d2cEx(i1,i2,i3)
                 un(i1,i2,i3,ey)=mxdc2d2cEy(i1,i2,i3)


                 un(i1,i2,i3,hz)=maxwellc22(i1,i2,i3,hz)




                end do
                end do
                end do
               end if
              else
               if( solveForE.ne.0 .and. solveForH.ne.0 )then
                 if( useWhereMask.ne.0 )then
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                     if( mask(i1,i2,i3).gt.0 )then
                       un(i1,i2,i3,ex)=mxdc2d2cEx(i1,i2,i3)
                       un(i1,i2,i3,ey)=mxdc2d2cEy(i1,i2,i3)
                       un(i1,i2,i3,ez)=maxwellc22(i1,i2,i3,ez)



                       un(i1,i2,i3,hx)=maxwellc22(i1,i2,i3,hx)
                       un(i1,i2,i3,hy)=maxwellc22(i1,i2,i3,hy)
                       un(i1,i2,i3,hz)=maxwellc22(i1,i2,i3,hz)









                     end if
                   end do
                   end do
                   end do
                 else
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                       un(i1,i2,i3,ex)=mxdc2d2cEx(i1,i2,i3)
                       un(i1,i2,i3,ey)=mxdc2d2cEy(i1,i2,i3)
                       un(i1,i2,i3,ez)=maxwellc22(i1,i2,i3,ez)



                       un(i1,i2,i3,hx)=maxwellc22(i1,i2,i3,hx)
                       un(i1,i2,i3,hy)=maxwellc22(i1,i2,i3,hy)
                       un(i1,i2,i3,hz)=maxwellc22(i1,i2,i3,hz)









                   end do
                   end do
                   end do
                 end if
               else if( solveForE.ne.0 ) then
                 if( useWhereMask.ne.0 )then
                  do i3=n3a,n3b
                  do i2=n2a,n2b
                  do i1=n1a,n1b
                   if( mask(i1,i2,i3).gt.0 )then
                    un(i1,i2,i3,ex)=mxdc2d2cEx(i1,i2,i3)
                    un(i1,i2,i3,ey)=mxdc2d2cEy(i1,i2,i3)
                    un(i1,i2,i3,ez)=maxwellc22(i1,i2,i3,ez)






                   end if
                  end do
                  end do
                  end do
                 else
                  do i3=n3a,n3b
                  do i2=n2a,n2b
                  do i1=n1a,n1b
                   un(i1,i2,i3,ex)=mxdc2d2cEx(i1,i2,i3)
                   un(i1,i2,i3,ey)=mxdc2d2cEy(i1,i2,i3)
                   un(i1,i2,i3,ez)=maxwellc22(i1,i2,i3,ez)






                  end do
                  end do
                  end do
                 end if
               else
                 if( useWhereMask.ne.0 )then
                  do i3=n3a,n3b
                  do i2=n2a,n2b
                  do i1=n1a,n1b
                   if( mask(i1,i2,i3).gt.0 )then
                    un(i1,i2,i3,hx)=maxwellc22(i1,i2,i3,hx)
                    un(i1,i2,i3,hy)=maxwellc22(i1,i2,i3,hy)
                    un(i1,i2,i3,hz)=maxwellc22(i1,i2,i3,hz)






                   end if
                  end do
                  end do
                  end do
                 else
                  do i3=n3a,n3b
                  do i2=n2a,n2b
                  do i1=n1a,n1b
                   un(i1,i2,i3,hx)=maxwellc22(i1,i2,i3,hx)
                   un(i1,i2,i3,hy)=maxwellc22(i1,i2,i3,hy)
                   un(i1,i2,i3,hz)=maxwellc22(i1,i2,i3,hz)






                  end do
                  end do
                  end do
                 end if
               end if
              end if
             else ! add dissipation too
              if( nd.eq.2 )then
               ! This next line assumes we solve for ex,ey and hz
               if( useWhereMask.ne.0 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                 if( mask(i1,i2,i3).gt.0 )then
                  un(i1,i2,i3,ex)=mxdc2d2cEx(i1,i2,i3)+dis(i1,i2,i3,ex)
                  un(i1,i2,i3,ey)=mxdc2d2cEy(i1,i2,i3)+dis(i1,i2,i3,ey)


                  un(i1,i2,i3,hz)=maxwellc22(i1,i2,i3,hz)+dis(i1,i2,i3,
     & hz)




                 end if
                end do
                end do
                end do
               else
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                 un(i1,i2,i3,ex)=mxdc2d2cEx(i1,i2,i3)+dis(i1,i2,i3,ex)
                 un(i1,i2,i3,ey)=mxdc2d2cEy(i1,i2,i3)+dis(i1,i2,i3,ey)


                 un(i1,i2,i3,hz)=maxwellc22(i1,i2,i3,hz)+dis(i1,i2,i3,
     & hz)




                end do
                end do
                end do
               end if
              else
               if( solveForE.ne.0 .and. solveForH.ne.0 )then
                 if( useWhereMask.ne.0 )then
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                     if( mask(i1,i2,i3).gt.0 )then
                       un(i1,i2,i3,ex)=mxdc2d2cEx(i1,i2,i3)+dis(i1,i2,
     & i3,ex)
                       un(i1,i2,i3,ey)=mxdc2d2cEy(i1,i2,i3)+dis(i1,i2,
     & i3,ey)
                       un(i1,i2,i3,ez)=maxwellc22(i1,i2,i3,ez)+dis(i1,
     & i2,i3,ez)



                       un(i1,i2,i3,hx)=maxwellc22(i1,i2,i3,hx)+dis(i1,
     & i2,i3,hx)
                       un(i1,i2,i3,hy)=maxwellc22(i1,i2,i3,hy)+dis(i1,
     & i2,i3,hy)
                       un(i1,i2,i3,hz)=maxwellc22(i1,i2,i3,hz)+dis(i1,
     & i2,i3,hz)









                     end if
                   end do
                   end do
                   end do
                 else
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                       un(i1,i2,i3,ex)=mxdc2d2cEx(i1,i2,i3)+dis(i1,i2,
     & i3,ex)
                       un(i1,i2,i3,ey)=mxdc2d2cEy(i1,i2,i3)+dis(i1,i2,
     & i3,ey)
                       un(i1,i2,i3,ez)=maxwellc22(i1,i2,i3,ez)+dis(i1,
     & i2,i3,ez)



                       un(i1,i2,i3,hx)=maxwellc22(i1,i2,i3,hx)+dis(i1,
     & i2,i3,hx)
                       un(i1,i2,i3,hy)=maxwellc22(i1,i2,i3,hy)+dis(i1,
     & i2,i3,hy)
                       un(i1,i2,i3,hz)=maxwellc22(i1,i2,i3,hz)+dis(i1,
     & i2,i3,hz)









                   end do
                   end do
                   end do
                 end if
               else if( solveForE.ne.0 ) then
                 if( useWhereMask.ne.0 )then
                  do i3=n3a,n3b
                  do i2=n2a,n2b
                  do i1=n1a,n1b
                   if( mask(i1,i2,i3).gt.0 )then
                    un(i1,i2,i3,ex)=mxdc2d2cEx(i1,i2,i3)+dis(i1,i2,i3,
     & ex)
                    un(i1,i2,i3,ey)=mxdc2d2cEy(i1,i2,i3)+dis(i1,i2,i3,
     & ey)
                    un(i1,i2,i3,ez)=maxwellc22(i1,i2,i3,ez)+dis(i1,i2,
     & i3,ez)






                   end if
                  end do
                  end do
                  end do
                 else
                  do i3=n3a,n3b
                  do i2=n2a,n2b
                  do i1=n1a,n1b
                   un(i1,i2,i3,ex)=mxdc2d2cEx(i1,i2,i3)+dis(i1,i2,i3,
     & ex)
                   un(i1,i2,i3,ey)=mxdc2d2cEy(i1,i2,i3)+dis(i1,i2,i3,
     & ey)
                   un(i1,i2,i3,ez)=maxwellc22(i1,i2,i3,ez)+dis(i1,i2,
     & i3,ez)






                  end do
                  end do
                  end do
                 end if
               else
                 if( useWhereMask.ne.0 )then
                  do i3=n3a,n3b
                  do i2=n2a,n2b
                  do i1=n1a,n1b
                   if( mask(i1,i2,i3).gt.0 )then
                    un(i1,i2,i3,hx)=maxwellc22(i1,i2,i3,hx)+dis(i1,i2,
     & i3,hx)
                    un(i1,i2,i3,hy)=maxwellc22(i1,i2,i3,hy)+dis(i1,i2,
     & i3,hy)
                    un(i1,i2,i3,hz)=maxwellc22(i1,i2,i3,hz)+dis(i1,i2,
     & i3,hz)






                   end if
                  end do
                  end do
                  end do
                 else
                  do i3=n3a,n3b
                  do i2=n2a,n2b
                  do i1=n1a,n1b
                   un(i1,i2,i3,hx)=maxwellc22(i1,i2,i3,hx)+dis(i1,i2,
     & i3,hx)
                   un(i1,i2,i3,hy)=maxwellc22(i1,i2,i3,hy)+dis(i1,i2,
     & i3,hy)
                   un(i1,i2,i3,hz)=maxwellc22(i1,i2,i3,hz)+dis(i1,i2,
     & i3,hz)






                  end do
                  end do
                  end do
                 end if
               end if
              end if
             end if
           end if
         end if
        end if
        return
        end
