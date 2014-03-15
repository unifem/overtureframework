! This file automatically generated from advOptSm.bf with bpp.
        subroutine advSm2dOrder2c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rsxy,xy,  um,u,un,f, bc, 
     & dis, varDis, ipar, rpar, ierr )
c======================================================================
c   Advance a time step for the equations of Solid Mechanics (linear elasticity for now)
c 
c nd : number of space dimensions
c
c ipar(0)  = option : option=0 - Elasticity+Artificial diffusion
c                           =1 - AD only
c
c  dis(i1,i2,i3) : temp space to hold artificial dissipation
c  varDis(i1,i2,i3) : coefficient of the variable artificial dissipation
c======================================================================
        implicit none
        integer nd, n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,
     & nd3b,nd4a,nd4b
        real um(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
        real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
        real un(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
        real f(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
        real dis(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
        real varDis(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
        real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
        real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
        integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
        integer bc(0:1,0:2),ierr
        integer ipar(0:*)
        real rpar(0:*)
c     ---- local variables -----
        integer c,i1,i2,i3,n,gridType,orderOfAccuracy,orderInTime,
     & debug,computeUt
        integer addForcing,orderOfDissipation,option
        integer useWhereMask,useWhereMaskSave,grid,
     & useVariableDissipation
        integer useConservative,combineDissipationWithAdvance
        integer uc,vc,wc
        integer materialFormat,myid
        real cc,dt,dy,dz,cdt,cdtdx,cdtdy,cdtdz,adc,adcdt,add,adddt,
     & dtOld,cu,cum
        real dt4by12
        real kx,ky,kz
        real t,ep
        real dx(0:2),dr(0:2)
        real ux0,vx0,wx0,uy0,vy0,wy0,uz0,vz0,wz0
        real dx2i,dy2i,dz2i,dxsqi,dysqi,dzsqi,dxi,dyi,dzi
        real dx12i,dy12i,dz12i,dxsq12i,dysq12i,dzsq12i,dxy4i,dxz4i,
     & dyz4,time0,time1
        real dxi4,dyi4,dzi4,dxdyi2,dxdzi2,dydzi2
        real uLap(-1:1,-1:1,0:5),uLapSq(0:5)
        real uLaprr2,uLapss2,uLaprs2,uLapr2,uLaps2
        real c0,csq,dtsq,cdtsq,cdtsq12,lap(0:20)
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
c...........start statement function
        integer kd,m
        real rx,ry,rz,sx,sy,sz,tx,ty,tz
c include 'declareDiffOrder2f.h'
c include 'declareDiffOrder4f.h'
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
        real sm22ru,sm22rv,       sm22u,sm22v
        real sm23ru,sm23rv,sm23rw,sm23u,sm23v,sm23w
        real sm42ru,sm42rv,       sm42u,sm42v
        real sm43ru,sm43rv,sm43rw,sm43u,sm43v,sm43w
        real sm22rut,sm22rvt,       sm22ut,sm22vt
        real sm23rut,sm23rvt,sm23rwt,sm23ut,sm23vt,sm23wt
        real sm42rut,sm42rvt,       sm42ut,sm42vt
        real sm43rut,sm43rvt,sm43rwt,sm43ut,sm43vt,sm43wt
        real c1,c2,c1dtsq, c2dtsq
        real maxwell2dr,maxwell3dr,maxwellr44,maxwellr66,maxwellr88
        real maxwellc22,maxwellc44,maxwellc66,maxwellc88
        real maxwell2dr44me,maxwell2dr66me,maxwell2dr88me
        real maxwell3dr44me,maxwell3dr66me,maxwell3dr88me
        real maxwellc44me,maxwellc66me,maxwellc88me
        real max2dc44me,max2dc44me2,max3dc44me
c real vr2,vs2,vrr2,vss2,vrs2,vLaplacian22
        real cdt4by360,cdt6by20160
        real lap2d2,lap3d2,lap2d4,lap3d4,lap2d6,lap3d6,lap2d8,lap3d8,
     & lap2d2Pow2,lap3d2Pow2,lap2d2Pow3,lap3d2Pow3,lap2d2Pow4,
     & lap3d2Pow4,lap2d4Pow2,lap3d4Pow2,lap2d4Pow3,lap3d4Pow3,
     & lap2d6Pow2,lap3d6Pow2
        real du,fd22d,fd23d,fd42d,fd43d,fd62d,fd63d,fd82d,fd83d
c real unxx22r,unyy22r,unxy22r,unx22r
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
c** defineDifferenceOrder2Components1(un,none)
c** defineDifferenceOrder4Components1(un,none)
c** defineDifferenceOrder2Components1(v,none)
c** defineDifferenceOrder4Components1(v,none)
        ! *************************************************
        ! *********2nd-order in space and time*************
        ! *************************************************
        ! --- 2D ---
        sm22ru(i1,i2,i3)=cu*u(i1,i2,i3,uc)+cum*um(i1,i2,i3,uc)+c2dtsq*(
     &  ulaplacian22r(i1,i2,i3,uc) )+c1dtsq*( uxx22r(i1,i2,i3,uc) + 
     & uxy22r(i1,i2,i3,vc) )
        sm22rv(i1,i2,i3)=cu*u(i1,i2,i3,vc)+cum*um(i1,i2,i3,vc)+c2dtsq*(
     &  ulaplacian22r(i1,i2,i3,vc) )+ c1dtsq*( uxy22r(i1,i2,i3,uc) + 
     & uyy22r(i1,i2,i3,vc) )
        sm22u(i1,i2,i3)=cu*u(i1,i2,i3,uc)+cum*um(i1,i2,i3,uc)+c2dtsq*( 
     & ulaplacian22(i1,i2,i3,uc) )+c1dtsq*( uxx22(i1,i2,i3,uc) + 
     & uxy22(i1,i2,i3,vc) )
        sm22v(i1,i2,i3)=cu*u(i1,i2,i3,vc)+cum*um(i1,i2,i3,vc)+c2dtsq*( 
     & ulaplacian22(i1,i2,i3,vc) ) + c1dtsq*( uxy22(i1,i2,i3,uc) + 
     & uyy22(i1,i2,i3,vc) )
          ! time derivatives only for MOL
        sm22rut(i1,i2,i3)=c2    *( ulaplacian22r(i1,i2,i3,uc) )+c1    *
     & ( uxx22r(i1,i2,i3,uc) + uxy22r(i1,i2,i3,vc) )
        sm22rvt(i1,i2,i3)=c2    *( ulaplacian22r(i1,i2,i3,vc) )+ c1    
     & *( uxy22r(i1,i2,i3,uc) + uyy22r(i1,i2,i3,vc) )
        sm22ut(i1,i2,i3)=c2    *( ulaplacian22(i1,i2,i3,uc) )+c1    *( 
     & uxx22(i1,i2,i3,uc) + uxy22(i1,i2,i3,vc) )
        sm22vt(i1,i2,i3)=c2    *( ulaplacian22(i1,i2,i3,vc) ) + c1    *
     & ( uxy22(i1,i2,i3,uc) + uyy22(i1,i2,i3,vc) )
        ! --- 3D ---
        sm23ru(i1,i2,i3)=cu*u(i1,i2,i3,uc)+cum*um(i1,i2,i3,uc)+c2dtsq*(
     &  ulaplacian23r(i1,i2,i3,uc) )+c1dtsq*( uxx23r(i1,i2,i3,uc) + 
     & uxy23r(i1,i2,i3,vc)+ uxz23r(i1,i2,i3,wc) )
        sm23rv(i1,i2,i3)=cu*u(i1,i2,i3,vc)+cum*um(i1,i2,i3,vc)+c2dtsq*(
     &  ulaplacian23r(i1,i2,i3,vc) )+c1dtsq*( uxy23r(i1,i2,i3,uc) + 
     & uyy23r(i1,i2,i3,vc)+ uyz23r(i1,i2,i3,wc) )
        sm23rw(i1,i2,i3)=cu*u(i1,i2,i3,wc)+cum*um(i1,i2,i3,wc)+c2dtsq*(
     &  ulaplacian23r(i1,i2,i3,wc) )+c1dtsq*( uxz23r(i1,i2,i3,uc) + 
     & uyz23r(i1,i2,i3,vc)+ uzz23r(i1,i2,i3,wc) )
        sm23u(i1,i2,i3)=cu*u(i1,i2,i3,uc)+cum*um(i1,i2,i3,uc)+c2dtsq*( 
     & ulaplacian23(i1,i2,i3,uc) )+c1dtsq*( uxx23(i1,i2,i3,uc) + 
     & uxy23(i1,i2,i3,vc)+ uxz23(i1,i2,i3,wc) )
        sm23v(i1,i2,i3)=cu*u(i1,i2,i3,vc)+cum*um(i1,i2,i3,vc)+c2dtsq*( 
     & ulaplacian23(i1,i2,i3,vc) )+c1dtsq*( uxy23(i1,i2,i3,uc) + 
     & uyy23(i1,i2,i3,vc)+ uyz23(i1,i2,i3,wc) )
        sm23w(i1,i2,i3)=cu*u(i1,i2,i3,wc)+cum*um(i1,i2,i3,wc)+c2dtsq*( 
     & ulaplacian23(i1,i2,i3,wc) )+c1dtsq*( uxz23(i1,i2,i3,uc) + 
     & uyz23(i1,i2,i3,vc)+ uzz23(i1,i2,i3,wc) )
         ! -- time derivatives
        sm23rut(i1,i2,i3)=c2    *( ulaplacian23r(i1,i2,i3,uc) )+c1    *
     & ( uxx23r(i1,i2,i3,uc) + uxy23r(i1,i2,i3,vc)+ uxz23r(i1,i2,i3,
     & wc) )
        sm23rvt(i1,i2,i3)=c2    *( ulaplacian23r(i1,i2,i3,vc) )+c1    *
     & ( uxy23r(i1,i2,i3,uc) + uyy23r(i1,i2,i3,vc)+ uyz23r(i1,i2,i3,
     & wc) )
        sm23rwt(i1,i2,i3)=c2    *( ulaplacian23r(i1,i2,i3,wc) )+c1    *
     & ( uxz23r(i1,i2,i3,uc) + uyz23r(i1,i2,i3,vc)+ uzz23r(i1,i2,i3,
     & wc) )
        sm23ut(i1,i2,i3)=c2    *( ulaplacian23(i1,i2,i3,uc) )+c1    *( 
     & uxx23(i1,i2,i3,uc) + uxy23(i1,i2,i3,vc)+ uxz23(i1,i2,i3,wc) )
        sm23vt(i1,i2,i3)=c2    *( ulaplacian23(i1,i2,i3,vc) )+c1    *( 
     & uxy23(i1,i2,i3,uc) + uyy23(i1,i2,i3,vc)+ uyz23(i1,i2,i3,wc) )
        sm23wt(i1,i2,i3)=c2    *( ulaplacian23(i1,i2,i3,wc) )+c1    *( 
     & uxz23(i1,i2,i3,uc) + uyz23(i1,i2,i3,vc)+ uzz23(i1,i2,i3,wc) )
        ! *************************************************
        ! *********4th-order in space and time*************
        ! *************************************************
        ! --- 2D ---
        sm42ru(i1,i2,i3)=2.*u(i1,i2,i3,uc)-um(i1,i2,i3,uc)+c2dtsq*( 
     & ulaplacian42r(i1,i2,i3,uc) )+c1dtsq*( uxx42r(i1,i2,i3,uc) + 
     & uxy42r(i1,i2,i3,vc) )
        sm42rv(i1,i2,i3)=2.*u(i1,i2,i3,vc)-um(i1,i2,i3,vc)+c2dtsq*( 
     & ulaplacian42r(i1,i2,i3,vc) )+ c1dtsq*( uxy42r(i1,i2,i3,uc) + 
     & uyy42r(i1,i2,i3,vc) )
        sm42u(i1,i2,i3)=2.*u(i1,i2,i3,uc)-um(i1,i2,i3,uc)+c2dtsq*( 
     & ulaplacian42(i1,i2,i3,uc) )+c1dtsq*( uxx42(i1,i2,i3,uc) + 
     & uxy42(i1,i2,i3,vc) )
        sm42v(i1,i2,i3)=2.*u(i1,i2,i3,vc)-um(i1,i2,i3,vc)+c2dtsq*( 
     & ulaplacian42(i1,i2,i3,vc) ) + c1dtsq*( uxy42(i1,i2,i3,uc) + 
     & uyy42(i1,i2,i3,vc) )
        ! --- 3D ---
        sm43ru(i1,i2,i3)=2.*u(i1,i2,i3,uc)-um(i1,i2,i3,uc)+c2dtsq*( 
     & ulaplacian43r(i1,i2,i3,uc) )+c1dtsq*( uxx43r(i1,i2,i3,uc) + 
     & uxy43r(i1,i2,i3,vc)+ uxz43r(i1,i2,i3,wc) )
        sm43rv(i1,i2,i3)=2.*u(i1,i2,i3,vc)-um(i1,i2,i3,vc)+c2dtsq*( 
     & ulaplacian43r(i1,i2,i3,vc) )+c1dtsq*( uxy43r(i1,i2,i3,uc) + 
     & uyy43r(i1,i2,i3,vc)+ uyz43r(i1,i2,i3,wc) )
        sm43rw(i1,i2,i3)=2.*u(i1,i2,i3,wc)-um(i1,i2,i3,wc)+c2dtsq*( 
     & ulaplacian43r(i1,i2,i3,wc) )+c1dtsq*( uxz43r(i1,i2,i3,uc) + 
     & uyz43r(i1,i2,i3,vc)+ uzz43r(i1,i2,i3,wc) )
        sm43u(i1,i2,i3)=2.*u(i1,i2,i3,uc)-um(i1,i2,i3,uc)+c2dtsq*( 
     & ulaplacian43(i1,i2,i3,uc) )+c1dtsq*( uxx43(i1,i2,i3,uc) + 
     & uxy43(i1,i2,i3,vc)+ uxz43(i1,i2,i3,wc) )
        sm43v(i1,i2,i3)=2.*u(i1,i2,i3,vc)-um(i1,i2,i3,vc)+c2dtsq*( 
     & ulaplacian43(i1,i2,i3,vc) )+c1dtsq*( uxy43(i1,i2,i3,uc) + 
     & uyy43(i1,i2,i3,vc)+ uyz43(i1,i2,i3,wc) )
        sm43w(i1,i2,i3)=2.*u(i1,i2,i3,wc)-um(i1,i2,i3,wc)+c2dtsq*( 
     & ulaplacian43(i1,i2,i3,wc) )+c1dtsq*( uxz43(i1,i2,i3,uc) + 
     & uyz43(i1,i2,i3,vc)+ uzz43(i1,i2,i3,wc) )
c    *** 2nd order ***
        lap2d2(i1,i2,i3,c)=(u(i1+1,i2,i3,c)-2.*u(i1,i2,i3,c)+u(i1-1,i2,
     & i3,c))*dxsqi+(u(i1,i2+1,i3,c)-2.*u(i1,i2,i3,c)+u(i1,i2-1,i3,c))
     & *dysqi
        lap3d2(i1,i2,i3,c)=(u(i1+1,i2,i3,c)-2.*u(i1,i2,i3,c)+u(i1-1,i2,
     & i3,c))*dxsqi+(u(i1,i2+1,i3,c)-2.*u(i1,i2,i3,c)+u(i1,i2-1,i3,c))
     & *dysqi+(u(i1,i2,i3+1,c)-2.*u(i1,i2,i3,c)+u(i1,i2,i3-1,c))*dzsqi
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
c    ** 4th order ****
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
c     *** 6th order ***
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
c     *** 8th order ***
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
c ******* artificial dissipation ******
        du(i1,i2,i3,c)=u(i1,i2,i3,c)-um(i1,i2,i3,c)
c      (2nd difference)
        fd22d(i1,i2,i3,c)= (     ( du(i1-1,i2,i3,c)+du(i1+1,i2,i3,c)+
     & du(i1,i2-1,i3,c)+du(i1,i2+1,i3,c) ) -4.*du(i1,i2,i3,c) )
c
        fd23d(i1,i2,i3,c)=(     ( du(i1-1,i2,i3,c)+du(i1+1,i2,i3,c)+du(
     & i1,i2-1,i3,c)+du(i1,i2+1,i3,c)+du(i1,i2,i3-1,c)+du(i1,i2,i3+1,
     & c) ) -6.*du(i1,i2,i3,c) )
c     -(fourth difference)
        fd42d(i1,i2,i3,c)= (    -( du(i1-2,i2,i3,c)+du(i1+2,i2,i3,c)+
     & du(i1,i2-2,i3,c)+du(i1,i2+2,i3,c) ) +4.*( du(i1-1,i2,i3,c)+du(
     & i1+1,i2,i3,c)+du(i1,i2-1,i3,c)+du(i1,i2+1,i3,c) ) -12.*du(i1,
     & i2,i3,c) )
c
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
c...........end   statement functions
        ! write(*,*) 'Inside advSM...'
        dt    =rpar(0)
        dx(0) =rpar(1)
        dx(1) =rpar(2)
        dx(2) =rpar(3)
        adc   =rpar(4)  ! coefficient of artificial dissipation
        dr(0) =rpar(5)
        dr(1) =rpar(6)
        dr(2) =rpar(7)
        c1    =rpar(8)
        c2    =rpar(9)
        kx    =rpar(10)
        ky    =rpar(11)
        kz    =rpar(12)
        ep    =rpar(13)
        t     =rpar(14)
        dtOld =rpar(15) ! dt used on the previous time step
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
        uc                 =ipar(6)
        vc                 =ipar(7)
        wc                 =ipar(8)
        useWhereMask       =ipar(9)
        timeSteppingMethod =ipar(10)
        useVariableDissipation=ipar(11)
        useConservative    =ipar(12)
        combineDissipationWithAdvance = ipar(13)
        debug              =ipar(14)
        computeUt          =ipar(15)
        materialFormat     =ipar(16)   ! 0=const, 1=piece-wise const, 2=varaiable
        myid               =ipar(17)
        cu=  2.     ! coeff. of u(t) in the time-step formula
        cum=-1.     ! coeff. of u(t-dtOld)
        csq=cc**2
        dtsq=dt**2
        cdt=cc*dt
        c1dtsq=c1*dtsq
        c2dtsq=c2*dtsq
        if( dtOld.le.0 )then
          write(*,'(" advSM:ERROR : dtOld<=0 ")')
          stop 8167
        end if
        if( dt.ne.dtOld )then
          write(*,'(" advSM:INFO: dt=",e12.4," <> dtOld=",e12.4," 
     & diff=",e9.2)') dt,dtOld,dt-dtOld
          if( orderOfAccuracy.ne.2 )then
            write(*,'(" advSM:ERROR: variable dt not implemented for 
     & orderOfAccuracy=",i4)') orderOfAccuracy
            ! '
            stop 8168
          end if
          ! adjust the coefficients for a variable time step : this is locally second order accurate
          cu= 1.+dt/dtOld     ! coeff. of u(t) in the time-step formula
          cum=-dt/dtOld       ! coeff. of u(t-dtOld)
          c1dtsq=c1*dt*(dt+dtOld)*.5
          c2dtsq=c2*dt*(dt+dtOld)*.5
        end if
c cdtsq=(cc**2)*(dt**2)
c cdtsq12=cdtsq*cdtsq/12.
c cdt4by360=(cdt)**4/360.
c cdt6by20160=cdt**6/(8.*7.*6.*5.*4.*3.)
        dt4by12=dtsq*dtsq/12.
c cdtdx = (cc*dt/dx(0))**2
c cdtdy = (cc*dt/dy)**2
c cdtdz = (cc*dt/dz)**2
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
        if( .false. .and. debug.gt.0 )then
         ! evaluate derivatives of the exact solution
         i1=5
         i2=5
         i3=0
         ! 
           call ogDeriv2(ep, 0,1,0,0, xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,
     & t, uc,ux0, vc,vx0)
           call ogDeriv2(ep, 0,0,1,0, xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,
     & t, uc,uy0, vc,vy0)
         write(*,'(" advOpt: i=",3i3," t,x,y=",3f6.2," ux,vx,uy,vy=",
     & 4f6.2)') i1,i2,i3,t,xy(i1,i2,i3,0),xy(i1,i2,i3,1),ux0,vx0,uy0,
     & vy0
        end if
        if( adc.gt.0. .and. combineDissipationWithAdvance.eq.0 )then
          ! ********************************************************************************************************
          ! ********************* Compute the dissipation and fill in the dis(i1,i2,i3,c) array ********************
          ! ********************************************************************************************************
          call ovtime( time0 )
         ! Here we assume that a (2m)th order method will only use dissipation of (2m) or (2m+2)
         if( computeUt.eq.0 )then
           !   adcdt=adc*dt
           adcdt = adc*(dt*(dt+dtOld)/2.)/dtOld  ! for variable time step *wdh* 100203
         else
          ! adcdt=adc/dt
          adcdt= adc/dtOld                    ! for variable time step *wdh* 100203
         end if
         if( orderOfDissipation.eq.4 )then
            ! write(*,*) 'Inside advSM: add dissipation order=4... option=',option
             if( useVariableDissipation.eq.0 )then
              if( useWhereMask.ne.0 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                if( mask(i1,i2,i3).gt.0 )then
                 dis(i1,i2,i3,uc)=adcdt*fd42d(i1,i2,i3,uc)
                 dis(i1,i2,i3,vc)=adcdt*fd42d(i1,i2,i3,vc)







                end if
               end do
               end do
               end do
              else
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                dis(i1,i2,i3,uc)=adcdt*fd42d(i1,i2,i3,uc)
                dis(i1,i2,i3,vc)=adcdt*fd42d(i1,i2,i3,vc)







               end do
               end do
               end do
              end if
             else
              ! write(*,'(" advOpt: apply 4th-order variable dissipation...")') 
              if( useWhereMask.ne.0 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                  if( varDis(i1,i2,i3).gt.0. .and. mask(i1,i2,i3).gt.0 
     & )then
                    dis(i1,i2,i3,uc)=adcdt*varDis(i1,i2,i3)*fd42d(i1,
     & i2,i3,uc)
                    dis(i1,i2,i3,vc)=adcdt*varDis(i1,i2,i3)*fd42d(i1,
     & i2,i3,vc)

c     write(*,'(" i=",3i3," varDis=",e10.2," diss=",3e10.2)') i1,i2,i3,varDis(i1,i2,i3),dis(i1,i2,i3,uc),c         dis(i1,i2,i3,vc),dis(i1,i2,i3,wc)
                  else
                    dis(i1,i2,i3,uc)=0.
                    dis(i1,i2,i3,vc)=0.

                  end if
                end do
                end do
                end do
              else
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                  if( varDis(i1,i2,i3).gt.0. )then
                    dis(i1,i2,i3,uc)=adcdt*varDis(i1,i2,i3)*fd42d(i1,
     & i2,i3,uc)
                    dis(i1,i2,i3,vc)=adcdt*varDis(i1,i2,i3)*fd42d(i1,
     & i2,i3,vc)

                  else
                    dis(i1,i2,i3,uc)=0.
                    dis(i1,i2,i3,vc)=0.

                  end if
                end do
                end do
                end do
              end if
             end if
          else if( orderOfDissipation.eq.2 )then
             if( useVariableDissipation.eq.0 )then
              if( useWhereMask.ne.0 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                if( mask(i1,i2,i3).gt.0 )then
                 dis(i1,i2,i3,uc)=adcdt*fd22d(i1,i2,i3,uc)
                 dis(i1,i2,i3,vc)=adcdt*fd22d(i1,i2,i3,vc)







                end if
               end do
               end do
               end do
              else
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                dis(i1,i2,i3,uc)=adcdt*fd22d(i1,i2,i3,uc)
                dis(i1,i2,i3,vc)=adcdt*fd22d(i1,i2,i3,vc)







               end do
               end do
               end do
              end if
             else
               stop 33333
             end if
          else
            write(*,*) 'advSM:ERROR orderOfDissipation=',
     & orderOfDissipation
            stop 5
          end if
          call ovtime( time1 )
          rpar(20)=time1-time0
        end if
        if( option.eq.1 ) then
          return
        end if
c write(*,'(" advSM: timeSteppingMethod=",i2)') timeSteppingMethod
        if( timeSteppingMethod.eq.defaultTimeStepping )then
         write(*,'(" advSM:ERROR: 
     & timeSteppingMethod=defaultTimeStepping -- this should be set")
     & ')
           ! '
         stop 83322
        end if
        if( gridType.eq.rectangular )then
        else
c       **********************************************
c       *************** curvilinear ******************
c       **********************************************
          if( useConservative.eq.0 )then
           ! *************** non-conservative *****************    
             if( computeUt.eq.0 )then
              if( addForcing.eq.0 .and. adc.le.0. )then
                if( useWhereMask.ne.0 )then
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                  if( mask(i1,i2,i3).gt.0 )then
                   un(i1,i2,i3,uc)=sm22u(i1,i2,i3)
                   un(i1,i2,i3,vc)=sm22v(i1,i2,i3)







                  end if
                 end do
                 end do
                 end do
                else
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                  un(i1,i2,i3,uc)=sm22u(i1,i2,i3)
                  un(i1,i2,i3,vc)=sm22v(i1,i2,i3)







                 end do
                 end do
                 end do
                end if
              else if( addForcing.ne.0 .and. adc.le.0. )then
c add forcing to the first 2 equations
                if( useWhereMask.ne.0 )then
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                  if( mask(i1,i2,i3).gt.0 )then
                   un(i1,i2,i3,uc)=sm22u(i1,i2,i3)+dtsq*f(i1,i2,i3,uc)
                   un(i1,i2,i3,vc)=sm22v(i1,i2,i3)+dtsq*f(i1,i2,i3,vc)







                  end if
                 end do
                 end do
                 end do
                else
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                  un(i1,i2,i3,uc)=sm22u(i1,i2,i3)+dtsq*f(i1,i2,i3,uc)
                  un(i1,i2,i3,vc)=sm22v(i1,i2,i3)+dtsq*f(i1,i2,i3,vc)







                 end do
                 end do
                 end do
                end if
              else if( addForcing.eq.0 .and. adc.gt.0. )then
c add dissipation to the first 3 equations
                if( useWhereMask.ne.0 )then
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                  if( mask(i1,i2,i3).gt.0 )then
                   un(i1,i2,i3,uc)=sm22u(i1,i2,i3)+dis(i1,i2,i3,uc)
                   un(i1,i2,i3,vc)=sm22v(i1,i2,i3)+dis(i1,i2,i3,vc)







                  end if
                 end do
                 end do
                 end do
                else
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                  un(i1,i2,i3,uc)=sm22u(i1,i2,i3)+dis(i1,i2,i3,uc)
                  un(i1,i2,i3,vc)=sm22v(i1,i2,i3)+dis(i1,i2,i3,vc)







                 end do
                 end do
                 end do
                end if
              else
c  add forcing and dissipation
                if( useWhereMask.ne.0 )then
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                  if( mask(i1,i2,i3).gt.0 )then
                   un(i1,i2,i3,uc)=sm22u(i1,i2,i3)+dtsq*f(i1,i2,i3,uc)+
     & dis(i1,i2,i3,uc)
                   un(i1,i2,i3,vc)=sm22v(i1,i2,i3)+dtsq*f(i1,i2,i3,vc)+
     & dis(i1,i2,i3,vc)







                  end if
                 end do
                 end do
                 end do
                else
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                  un(i1,i2,i3,uc)=sm22u(i1,i2,i3)+dtsq*f(i1,i2,i3,uc)+
     & dis(i1,i2,i3,uc)
                  un(i1,i2,i3,vc)=sm22v(i1,i2,i3)+dtsq*f(i1,i2,i3,vc)+
     & dis(i1,i2,i3,vc)







                 end do
                 end do
                 end do
                end if
              end if
             else
              if( addForcing.eq.0 .and. adc.le.0. )then
                if( useWhereMask.ne.0 )then
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                  if( mask(i1,i2,i3).gt.0 )then
                   un(i1,i2,i3,uc)=sm22ut(i1,i2,i3)
                   un(i1,i2,i3,vc)=sm22vt(i1,i2,i3)







                  end if
                 end do
                 end do
                 end do
                else
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                  un(i1,i2,i3,uc)=sm22ut(i1,i2,i3)
                  un(i1,i2,i3,vc)=sm22vt(i1,i2,i3)







                 end do
                 end do
                 end do
                end if
              else if( addForcing.ne.0 .and. adc.le.0. )then
c add forcing to the first 2 equations
                if( useWhereMask.ne.0 )then
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                  if( mask(i1,i2,i3).gt.0 )then
                   un(i1,i2,i3,uc)=sm22ut(i1,i2,i3)+f(i1,i2,i3,uc)
                   un(i1,i2,i3,vc)=sm22vt(i1,i2,i3)+f(i1,i2,i3,vc)







                  end if
                 end do
                 end do
                 end do
                else
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                  un(i1,i2,i3,uc)=sm22ut(i1,i2,i3)+f(i1,i2,i3,uc)
                  un(i1,i2,i3,vc)=sm22vt(i1,i2,i3)+f(i1,i2,i3,vc)







                 end do
                 end do
                 end do
                end if
              else if( addForcing.eq.0 .and. adc.gt.0. )then
c add dissipation to the first 3 equations
                if( useWhereMask.ne.0 )then
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                  if( mask(i1,i2,i3).gt.0 )then
                   un(i1,i2,i3,uc)=sm22ut(i1,i2,i3)+dis(i1,i2,i3,uc)
                   un(i1,i2,i3,vc)=sm22vt(i1,i2,i3)+dis(i1,i2,i3,vc)







                  end if
                 end do
                 end do
                 end do
                else
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                  un(i1,i2,i3,uc)=sm22ut(i1,i2,i3)+dis(i1,i2,i3,uc)
                  un(i1,i2,i3,vc)=sm22vt(i1,i2,i3)+dis(i1,i2,i3,vc)







                 end do
                 end do
                 end do
                end if
              else
c  add forcing and dissipation
                if( useWhereMask.ne.0 )then
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                  if( mask(i1,i2,i3).gt.0 )then
                   un(i1,i2,i3,uc)=sm22ut(i1,i2,i3)+f(i1,i2,i3,uc)+dis(
     & i1,i2,i3,uc)
                   un(i1,i2,i3,vc)=sm22vt(i1,i2,i3)+f(i1,i2,i3,vc)+dis(
     & i1,i2,i3,vc)







                  end if
                 end do
                 end do
                 end do
                else
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                  un(i1,i2,i3,uc)=sm22ut(i1,i2,i3)+f(i1,i2,i3,uc)+dis(
     & i1,i2,i3,uc)
                  un(i1,i2,i3,vc)=sm22vt(i1,i2,i3)+f(i1,i2,i3,vc)+dis(
     & i1,i2,i3,vc)







                 end do
                 end do
                 end do
                end if
              end if
             end if
          else if( useConservative.eq.1 )then
           ! *************** conservative *****************    
           stop 99422
          else
            ! *****************************************************
            ! ****************Old way******************************
            ! *****************************************************
         end if
        end if
        return
        end
