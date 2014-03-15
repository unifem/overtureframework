! This file automatically generated from cnsFarFieldBC.bf with bpp.
c
c routines for applying a far-field BC
c

c Use this next macro to declare the statement functions that are defined below
c To include derivatives of rx use OPTION=RX


c Define statement functions for difference approximations of order 2 
c To include derivatives of rx use OPTION=RX
c To include derivatives of rx use OPTION=RX













      subroutine cnsFarFieldBC(nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,
     & nd4b,ipar,rpar, u, u2,  gv, gv2, gtt, mask, x,rsxy, bc, 
     & indexRange, exact, uKnown, ierr )
c========================================================================
c
c     Apply a far field boundary condition 
c
c  u : solution at time t
c  u2 : solution at time t-dt
c 
c gv (input) : g' -  gridVelocity at time t (for moving grids)
c gvt (input) : g'' - we need the gridAcceleration on the boundaries
c gvtt (input) : g''' - we may need the 3rd time derivative of g on the boudary
c
c========================================================================
      implicit none
      integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b
c     integer *8 exact ! holds pointer to OGFunction
      integer exact ! holds pointer to OGFunction
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real u2(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      real gv2(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      real gtt(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      real x(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real uKnown(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real rpar(0:*)

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer indexRange(0:1,0:2), bc(0:1,0:2)
      integer ipar(0:*),ierr

c.......local

      integer kd,kd3,i1,i2,i3,n1a,n1b,n2a,n2b,n3a,n3b
      integer is,j1,j2,j3,side,axis,twilightZone,bcOption,knownSolution
      integer rc,tc,uc,vc,wc,sc,unc,utc,n
      integer grid,orderOfAccuracy,gridIsMoving,useWhereMask
      integer gridType,isAxisymmetric,numberOfSpecies
      integer nr(0:1,0:2)

      real sxi,syi,szi,txi,tyi,tzi,rxi,ryi,rzi
      real pn,rho,rhon,nDotGradR,nDotGradS,tp,tpn,rhor,rhos,tps,ps,tpm,
     & pm,pp,pr,tpr
      integer axisp1

      real un,c

      integer is1,is2,is3,js1,js2,js3,ks1,ks2,ks3,s,m,mm

      real t,dt
      real an1,an2,an3,nDotU,aNorm,epsx,gamma
      real dr(0:2),dx(0:2),ad(0:10)
      real us0,vs0,w0s,sgn

      real rra,ura,vra,wra, rsa,usa,vsa,wsa, urra,vrra,wrra, ussa,vssa,
     & wssa, rrsa, ursa,vrsa,wrsa
      real rxa,rya,sxa,sya, rxra,ryra,sxra,syra, rxsa,rysa,sxsa,sysa
      real ra,ua,va,wa,fra,rhot

      real hx,hy,gm1
      real r0,rx0,ry0,rxx0,rxy0,ryy0, rt0,rtx0,rty0,rtt0
      real u0,ux0,uy0,uxx0,uxy0,uyy0, ut0,utx0,uty0,utt0
      real v0,vx0,vy0,vxx0,vxy0,vyy0, vt0,vtx0,vty0,vtt0
      real p0,px0,py0,pxx0,pxy0,pyy0, pt0,ptx0,pty0,ptt0
      real q0,qx0,qy0,qxx0,qxy0,qyy0, qt0,qtx0,qty0,qtt0
      real fv(0:20),uv(0:10),z0,tm,ad2dt
      real ep ! holds the pointer to the TZ function
      integer debug
      logical testSym,getGhostByTaylor,addFouthOrderAD

      real r1,u1,v1,q1,p1,s1, s0,st0,stt0
      real ur1,vr1,qr1,nDotU1,nDotuv(2),adu(0:10),usp,usm
      integer k1,k2,k3

      real rr0,rxr0,ryr0
      real ur0,uxr0,uyr0
      real vr0,vxr0,vyr0
      real qr0,qxr0,qyr0
      real pr0,pxr0,pyr0
      real utr0,vtr0
      real u2xr22,u2xs22,u2yr22,u2ys22
      real gvux0,gvuy0,gvvx0,gvvy0,gttu0,gttv0
      real s1p,sr

      real c0,c1,dm,dp,d0,dc,h

      real ux1,uxx1,rm,uxm,uxxm,rhox
      real xri,yri,xsi,ysi

      real Rg
      integer ii
      real ajac,rrt,rt,urt,vrt,qrt,rr1,tau1,tau2,px,py,aurr,avrr,aur,
     & avr
      real tp0,divu,rxU,sxU,pnm1,pnp1,pns,un0,vn0,rxn,ryn,sxn,syn,ank,
     & rxUn,sxUn
      real rxri,ryri,sxri,syri,rxsi,rysi,sxsi,sysi,divun,sxUt,an1r,
     & an2r,an1s,an2s,rv2t,Lnu
      real a11,a12,a21,a22,f1,f2,det, um, vm,tauDotU,b1,b2
      real pra(-2:402), psa(-2:402)  ! fix these ******************************************

      real prr,prs,pss,pst, divur, divus, rxUr, sxUr, rxUs, sxUs
      real gtu0,gtv0, gtru0,gtrv0, gtsu0, gtsv0, gttru0, gttrv0, 
     & gttsu0, gttsv0, gu0, gv0, gur0, gvr0, gus0, gvs0
      real gtttu0, gtttv0
      real fut, fvt, fpr, fps
      real urr0,urs0,uss0, vrr0,vrs0,vss0, rs0
      real term1, term2, dtEps, drEps, dsEps, fu0, fv0
      real ute,vte,uxe,uye,vxe,vye,ure,vre,use,vse,pne,prre, rrre,qrre,
     & qre, utte,vtte
      real urre,vrre,uxxe,uxye,vxxe,vxye,vrse,rrse,qrse,prse,pse, vyye,
     & vsse
      real rxe,rye,qxe,qye,rre,fr0
      real re1,re2,re3,qe1,qe2,qe3
      real tHalf,frr,fur,fvr,fqr
      real re,qe,rte,qte,rxte,qxte,ryte,qyte,pxte,pyte,prte,resid1,prt,
     &  pnte,pnt, rtte
      integer ms1,ms2,ms3,sideb,side2
      real uxte,vxte,uyte,vyte,urte,vrte,uste,vste,rxUt
      real qxxe,qxye,qyye,qxre,qyre
      real rxxe,rxye,ryye,rxre,ryre
      real xrri,yrri,ajacr, xssi,yssi,ajacs
      real fpr1,fpr2,fpr3,fpr4, fps1,fps2,fps3,fps4
      real qse,qsse,qxse,qyse
      real rse,rsse,rxse,ryse,rrte,uxre,vyre,divure,pxxe,pxye,pyye
      real psse,pste,pe,pre,cnSmooth
      integer it3, nitStage3

      integer it,nit,numberOfComponents,ncm1,kv(0:2)
c..................
      integer rectangular,curvilinear
      parameter( rectangular=0, curvilinear=1 )

      integer slipWallSymmetry, slipWallPressureEntropySymmetry, 
     & slipWallTaylor, slipWallCharacteristic,slipWallDerivative
      parameter( slipWallSymmetry=0, slipWallPressureEntropySymmetry=1,
     &  slipWallTaylor=2, slipWallCharacteristic=3,
     & slipWallDerivative=4 )

      integer
     &     noSlipWall,
     &     inflowWithVelocityGiven,
     &     slipWall,
     &     outflow,
     &     convectiveOutflow,
     &     tractionFree,
     &     inflowWithPandTV,
     &     dirichletBoundaryCondition,
     &     symmetry,
     &     axisymmetric,
     &     farField,
     &     neumannBoundaryCondition
      parameter( noSlipWall=1,inflowWithVelocityGiven=2,
     & slipWall=4,outflow=5,convectiveOutflow=14,tractionFree=15,
     & inflowWithPandTV=3,
     &  dirichletBoundaryCondition=12,
     & symmetry=11,axisymmetric=13, farField=16, 
     & neumannBoundaryCondition=18 )

      ! declare variables for difference approximations of u and RX
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
      ! declare difference approximations for u2
       real u2r2
       real u2s2
       real u2t2
       real u2rr2
       real u2ss2
       real u2rs2
       real u2tt2
       real u2rt2
       real u2st2
       real u2rrr2
       real u2sss2
       real u2ttt2
       real u2x21
       real u2y21
       real u2z21
       real u2x22
       real u2y22
       real u2z22
       real u2x23
       real u2y23
       real u2z23
       real u2xx21
       real u2yy21
       real u2xy21
       real u2xz21
       real u2yz21
       real u2zz21
       real u2laplacian21
       real u2xx22
       real u2yy22
       real u2xy22
       real u2xz22
       real u2yz22
       real u2zz22
       real u2laplacian22
       real u2xx23
       real u2yy23
       real u2zz23
       real u2xy23
       real u2xz23
       real u2yz23
       real u2laplacian23
       real u2x23r
       real u2y23r
       real u2z23r
       real u2xx23r
       real u2yy23r
       real u2xy23r
       real u2zz23r
       real u2xz23r
       real u2yz23r
       real u2x21r
       real u2y21r
       real u2z21r
       real u2xx21r
       real u2yy21r
       real u2zz21r
       real u2xy21r
       real u2xz21r
       real u2yz21r
       real u2laplacian21r
       real u2x22r
       real u2y22r
       real u2z22r
       real u2xx22r
       real u2yy22r
       real u2zz22r
       real u2xy22r
       real u2xz22r
       real u2yz22r
       real u2laplacian22r
       real u2laplacian23r
       real u2xxx22r
       real u2yyy22r
       real u2xxy22r
       real u2xyy22r
       real u2xxxx22r
       real u2yyyy22r
       real u2xxyy22r
       real u2xxx23r
       real u2yyy23r
       real u2zzz23r
       real u2xxy23r
       real u2xxz23r
       real u2xyy23r
       real u2yyz23r
       real u2xzz23r
       real u2yzz23r
       real u2xxxx23r
       real u2yyyy23r
       real u2zzzz23r
       real u2xxyy23r
       real u2xxzz23r
       real u2yyzz23r
       real u2LapSq22r
       real u2LapSq23r
      ! declare for derivatives of gv
       real gvr2
       real gvs2
       real gvt2
       real gvrr2
       real gvss2
       real gvrs2
       real gvtt2
       real gvrt2
       real gvst2
       real gvrrr2
       real gvsss2
       real gvttt2
       real gvx21
       real gvy21
       real gvz21
       real gvx22
       real gvy22
       real gvz22
       real gvx23
       real gvy23
       real gvz23
       real gvxx21
       real gvyy21
       real gvxy21
       real gvxz21
       real gvyz21
       real gvzz21
       real gvlaplacian21
       real gvxx22
       real gvyy22
       real gvxy22
       real gvxz22
       real gvyz22
       real gvzz22
       real gvlaplacian22
       real gvxx23
       real gvyy23
       real gvzz23
       real gvxy23
       real gvxz23
       real gvyz23
       real gvlaplacian23
       real gvx23r
       real gvy23r
       real gvz23r
       real gvxx23r
       real gvyy23r
       real gvxy23r
       real gvzz23r
       real gvxz23r
       real gvyz23r
       real gvx21r
       real gvy21r
       real gvz21r
       real gvxx21r
       real gvyy21r
       real gvzz21r
       real gvxy21r
       real gvxz21r
       real gvyz21r
       real gvlaplacian21r
       real gvx22r
       real gvy22r
       real gvz22r
       real gvxx22r
       real gvyy22r
       real gvzz22r
       real gvxy22r
       real gvxz22r
       real gvyz22r
       real gvlaplacian22r
       real gvlaplacian23r
       real gvxxx22r
       real gvyyy22r
       real gvxxy22r
       real gvxyy22r
       real gvxxxx22r
       real gvyyyy22r
       real gvxxyy22r
       real gvxxx23r
       real gvyyy23r
       real gvzzz23r
       real gvxxy23r
       real gvxxz23r
       real gvxyy23r
       real gvyyz23r
       real gvxzz23r
       real gvyzz23r
       real gvxxxx23r
       real gvyyyy23r
       real gvzzzz23r
       real gvxxyy23r
       real gvxxzz23r
       real gvyyzz23r
       real gvLapSq22r
       real gvLapSq23r
      ! declare for derivatives of gv2
       real gv2r2
       real gv2s2
       real gv2t2
       real gv2rr2
       real gv2ss2
       real gv2rs2
       real gv2tt2
       real gv2rt2
       real gv2st2
       real gv2rrr2
       real gv2sss2
       real gv2ttt2
       real gv2x21
       real gv2y21
       real gv2z21
       real gv2x22
       real gv2y22
       real gv2z22
       real gv2x23
       real gv2y23
       real gv2z23
       real gv2xx21
       real gv2yy21
       real gv2xy21
       real gv2xz21
       real gv2yz21
       real gv2zz21
       real gv2laplacian21
       real gv2xx22
       real gv2yy22
       real gv2xy22
       real gv2xz22
       real gv2yz22
       real gv2zz22
       real gv2laplacian22
       real gv2xx23
       real gv2yy23
       real gv2zz23
       real gv2xy23
       real gv2xz23
       real gv2yz23
       real gv2laplacian23
       real gv2x23r
       real gv2y23r
       real gv2z23r
       real gv2xx23r
       real gv2yy23r
       real gv2xy23r
       real gv2zz23r
       real gv2xz23r
       real gv2yz23r
       real gv2x21r
       real gv2y21r
       real gv2z21r
       real gv2xx21r
       real gv2yy21r
       real gv2zz21r
       real gv2xy21r
       real gv2xz21r
       real gv2yz21r
       real gv2laplacian21r
       real gv2x22r
       real gv2y22r
       real gv2z22r
       real gv2xx22r
       real gv2yy22r
       real gv2zz22r
       real gv2xy22r
       real gv2xz22r
       real gv2yz22r
       real gv2laplacian22r
       real gv2laplacian23r
       real gv2xxx22r
       real gv2yyy22r
       real gv2xxy22r
       real gv2xyy22r
       real gv2xxxx22r
       real gv2yyyy22r
       real gv2xxyy22r
       real gv2xxx23r
       real gv2yyy23r
       real gv2zzz23r
       real gv2xxy23r
       real gv2xxz23r
       real gv2xyy23r
       real gv2yyz23r
       real gv2xzz23r
       real gv2yzz23r
       real gv2xxxx23r
       real gv2yyyy23r
       real gv2zzzz23r
       real gv2xxyy23r
       real gv2xxzz23r
       real gv2yyzz23r
       real gv2LapSq22r
       real gv2LapSq23r

c .............. begin statement functions
      real rx,ry,rz,sx,sy,sz,tx,ty,tz
      real ogf,diss2,ad2,disst2,tanDiss2

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

      diss2(i1,i2,i3,n)=ad2dt*(u2(i1+1,i2,i3,n)+u2(i1-1,i2,i3,n)+u2(i1,
     & i2-1,i3,n)+u2(i1,i2+1,i3,n)-4.*u2(i1,i2,i3,n))

      disst2(i1,i2,i3,n)=ad2dt*(u2(i1+js1,i2+js2,i3,n)+u2(i1-js1,i2-
     & js2,i3,n)-2.*u2(i1,i2,i3,n))

      ! another form of tangential dissipation: 
      tanDiss2(i1,i2,i3,n)=(1.+adu(n))*ad2dt*(u2(i1+js1,i2+js2,i3,n)+
     & u2(i1-js1,i2-js2,i3,n)-2.*u2(i1,i2,i3,n))


c     The next macro call will define the difference approximation statement functions
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
      u2r2(i1,i2,i3,kd)=(u2(i1+1,i2,i3,kd)-u2(i1-1,i2,i3,kd))*d12(0)
      u2s2(i1,i2,i3,kd)=(u2(i1,i2+1,i3,kd)-u2(i1,i2-1,i3,kd))*d12(1)
      u2t2(i1,i2,i3,kd)=(u2(i1,i2,i3+1,kd)-u2(i1,i2,i3-1,kd))*d12(2)
      u2rr2(i1,i2,i3,kd)=(-2.*u2(i1,i2,i3,kd)+(u2(i1+1,i2,i3,kd)+u2(i1-
     & 1,i2,i3,kd)) )*d22(0)
      u2ss2(i1,i2,i3,kd)=(-2.*u2(i1,i2,i3,kd)+(u2(i1,i2+1,i3,kd)+u2(i1,
     & i2-1,i3,kd)) )*d22(1)
      u2rs2(i1,i2,i3,kd)=(u2r2(i1,i2+1,i3,kd)-u2r2(i1,i2-1,i3,kd))*d12(
     & 1)
      u2tt2(i1,i2,i3,kd)=(-2.*u2(i1,i2,i3,kd)+(u2(i1,i2,i3+1,kd)+u2(i1,
     & i2,i3-1,kd)) )*d22(2)
      u2rt2(i1,i2,i3,kd)=(u2r2(i1,i2,i3+1,kd)-u2r2(i1,i2,i3-1,kd))*d12(
     & 2)
      u2st2(i1,i2,i3,kd)=(u2s2(i1,i2,i3+1,kd)-u2s2(i1,i2,i3-1,kd))*d12(
     & 2)
      u2rrr2(i1,i2,i3,kd)=(-2.*(u2(i1+1,i2,i3,kd)-u2(i1-1,i2,i3,kd))+(
     & u2(i1+2,i2,i3,kd)-u2(i1-2,i2,i3,kd)) )*d22(0)*d12(0)
      u2sss2(i1,i2,i3,kd)=(-2.*(u2(i1,i2+1,i3,kd)-u2(i1,i2-1,i3,kd))+(
     & u2(i1,i2+2,i3,kd)-u2(i1,i2-2,i3,kd)) )*d22(1)*d12(1)
      u2ttt2(i1,i2,i3,kd)=(-2.*(u2(i1,i2,i3+1,kd)-u2(i1,i2,i3-1,kd))+(
     & u2(i1,i2,i3+2,kd)-u2(i1,i2,i3-2,kd)) )*d22(2)*d12(2)
      u2x21(i1,i2,i3,kd)= rx(i1,i2,i3)*u2r2(i1,i2,i3,kd)
      u2y21(i1,i2,i3,kd)=0
      u2z21(i1,i2,i3,kd)=0
      u2x22(i1,i2,i3,kd)= rx(i1,i2,i3)*u2r2(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & u2s2(i1,i2,i3,kd)
      u2y22(i1,i2,i3,kd)= ry(i1,i2,i3)*u2r2(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & u2s2(i1,i2,i3,kd)
      u2z22(i1,i2,i3,kd)=0
      u2x23(i1,i2,i3,kd)=rx(i1,i2,i3)*u2r2(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & u2s2(i1,i2,i3,kd)+tx(i1,i2,i3)*u2t2(i1,i2,i3,kd)
      u2y23(i1,i2,i3,kd)=ry(i1,i2,i3)*u2r2(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & u2s2(i1,i2,i3,kd)+ty(i1,i2,i3)*u2t2(i1,i2,i3,kd)
      u2z23(i1,i2,i3,kd)=rz(i1,i2,i3)*u2r2(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & u2s2(i1,i2,i3,kd)+tz(i1,i2,i3)*u2t2(i1,i2,i3,kd)
      u2xx21(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*u2rr2(i1,i2,i3,kd)+(rxx22(
     & i1,i2,i3))*u2r2(i1,i2,i3,kd)
      u2yy21(i1,i2,i3,kd)=0
      u2xy21(i1,i2,i3,kd)=0
      u2xz21(i1,i2,i3,kd)=0
      u2yz21(i1,i2,i3,kd)=0
      u2zz21(i1,i2,i3,kd)=0
      u2laplacian21(i1,i2,i3,kd)=u2xx21(i1,i2,i3,kd)
      u2xx22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*u2rr2(i1,i2,i3,kd)+2.*(rx(
     & i1,i2,i3)*sx(i1,i2,i3))*u2rs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2)*
     & u2ss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3))*u2r2(i1,i2,i3,kd)+(sxx22(
     & i1,i2,i3))*u2s2(i1,i2,i3,kd)
      u2yy22(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*u2rr2(i1,i2,i3,kd)+2.*(ry(
     & i1,i2,i3)*sy(i1,i2,i3))*u2rs2(i1,i2,i3,kd)+(sy(i1,i2,i3)**2)*
     & u2ss2(i1,i2,i3,kd)+(ryy22(i1,i2,i3))*u2r2(i1,i2,i3,kd)+(syy22(
     & i1,i2,i3))*u2s2(i1,i2,i3,kd)
      u2xy22(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*u2rr2(i1,i2,i3,kd)+
     & (rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*u2rs2(i1,
     & i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*u2ss2(i1,i2,i3,kd)+rxy22(
     & i1,i2,i3)*u2r2(i1,i2,i3,kd)+sxy22(i1,i2,i3)*u2s2(i1,i2,i3,kd)
      u2xz22(i1,i2,i3,kd)=0
      u2yz22(i1,i2,i3,kd)=0
      u2zz22(i1,i2,i3,kd)=0
      u2laplacian22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & u2rr2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*
     & sy(i1,i2,i3))*u2rs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)*
     & *2)*u2ss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3)+ryy22(i1,i2,i3))*u2r2(
     & i1,i2,i3,kd)+(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*u2s2(i1,i2,i3,
     & kd)
      u2xx23(i1,i2,i3,kd)=rx(i1,i2,i3)**2*u2rr2(i1,i2,i3,kd)+sx(i1,i2,
     & i3)**2*u2ss2(i1,i2,i3,kd)+tx(i1,i2,i3)**2*u2tt2(i1,i2,i3,kd)+
     & 2.*rx(i1,i2,i3)*sx(i1,i2,i3)*u2rs2(i1,i2,i3,kd)+2.*rx(i1,i2,i3)
     & *tx(i1,i2,i3)*u2rt2(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,i2,i3)*
     & u2st2(i1,i2,i3,kd)+rxx23(i1,i2,i3)*u2r2(i1,i2,i3,kd)+sxx23(i1,
     & i2,i3)*u2s2(i1,i2,i3,kd)+txx23(i1,i2,i3)*u2t2(i1,i2,i3,kd)
      u2yy23(i1,i2,i3,kd)=ry(i1,i2,i3)**2*u2rr2(i1,i2,i3,kd)+sy(i1,i2,
     & i3)**2*u2ss2(i1,i2,i3,kd)+ty(i1,i2,i3)**2*u2tt2(i1,i2,i3,kd)+
     & 2.*ry(i1,i2,i3)*sy(i1,i2,i3)*u2rs2(i1,i2,i3,kd)+2.*ry(i1,i2,i3)
     & *ty(i1,i2,i3)*u2rt2(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,i2,i3)*
     & u2st2(i1,i2,i3,kd)+ryy23(i1,i2,i3)*u2r2(i1,i2,i3,kd)+syy23(i1,
     & i2,i3)*u2s2(i1,i2,i3,kd)+tyy23(i1,i2,i3)*u2t2(i1,i2,i3,kd)
      u2zz23(i1,i2,i3,kd)=rz(i1,i2,i3)**2*u2rr2(i1,i2,i3,kd)+sz(i1,i2,
     & i3)**2*u2ss2(i1,i2,i3,kd)+tz(i1,i2,i3)**2*u2tt2(i1,i2,i3,kd)+
     & 2.*rz(i1,i2,i3)*sz(i1,i2,i3)*u2rs2(i1,i2,i3,kd)+2.*rz(i1,i2,i3)
     & *tz(i1,i2,i3)*u2rt2(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,i2,i3)*
     & u2st2(i1,i2,i3,kd)+rzz23(i1,i2,i3)*u2r2(i1,i2,i3,kd)+szz23(i1,
     & i2,i3)*u2s2(i1,i2,i3,kd)+tzz23(i1,i2,i3)*u2t2(i1,i2,i3,kd)
      u2xy23(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*u2rr2(i1,i2,i3,kd)+
     & sx(i1,i2,i3)*sy(i1,i2,i3)*u2ss2(i1,i2,i3,kd)+tx(i1,i2,i3)*ty(
     & i1,i2,i3)*u2tt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,
     & i2,i3)*sx(i1,i2,i3))*u2rs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(i1,i2,
     & i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*u2rt2(i1,i2,i3,kd)+(sx(i1,i2,i3)
     & *ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*u2st2(i1,i2,i3,kd)+
     & rxy23(i1,i2,i3)*u2r2(i1,i2,i3,kd)+sxy23(i1,i2,i3)*u2s2(i1,i2,
     & i3,kd)+txy23(i1,i2,i3)*u2t2(i1,i2,i3,kd)
      u2xz23(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*u2rr2(i1,i2,i3,kd)+
     & sx(i1,i2,i3)*sz(i1,i2,i3)*u2ss2(i1,i2,i3,kd)+tx(i1,i2,i3)*tz(
     & i1,i2,i3)*u2tt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,
     & i2,i3)*sx(i1,i2,i3))*u2rs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(i1,i2,
     & i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*u2rt2(i1,i2,i3,kd)+(sx(i1,i2,i3)
     & *tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*u2st2(i1,i2,i3,kd)+
     & rxz23(i1,i2,i3)*u2r2(i1,i2,i3,kd)+sxz23(i1,i2,i3)*u2s2(i1,i2,
     & i3,kd)+txz23(i1,i2,i3)*u2t2(i1,i2,i3,kd)
      u2yz23(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*u2rr2(i1,i2,i3,kd)+
     & sy(i1,i2,i3)*sz(i1,i2,i3)*u2ss2(i1,i2,i3,kd)+ty(i1,i2,i3)*tz(
     & i1,i2,i3)*u2tt2(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,
     & i2,i3)*sy(i1,i2,i3))*u2rs2(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(i1,i2,
     & i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*u2rt2(i1,i2,i3,kd)+(sy(i1,i2,i3)
     & *tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*u2st2(i1,i2,i3,kd)+
     & ryz23(i1,i2,i3)*u2r2(i1,i2,i3,kd)+syz23(i1,i2,i3)*u2s2(i1,i2,
     & i3,kd)+tyz23(i1,i2,i3)*u2t2(i1,i2,i3,kd)
      u2laplacian23(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*u2rr2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)*
     & *2+sz(i1,i2,i3)**2)*u2ss2(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(i1,
     & i2,i3)**2+tz(i1,i2,i3)**2)*u2tt2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*
     & sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,
     & i3))*u2rs2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(i1,
     & i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*u2rt2(i1,i2,i3,
     & kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)+
     & sz(i1,i2,i3)*tz(i1,i2,i3))*u2st2(i1,i2,i3,kd)+(rxx23(i1,i2,i3)+
     & ryy23(i1,i2,i3)+rzz23(i1,i2,i3))*u2r2(i1,i2,i3,kd)+(sxx23(i1,
     & i2,i3)+syy23(i1,i2,i3)+szz23(i1,i2,i3))*u2s2(i1,i2,i3,kd)+(
     & txx23(i1,i2,i3)+tyy23(i1,i2,i3)+tzz23(i1,i2,i3))*u2t2(i1,i2,i3,
     & kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      u2x23r(i1,i2,i3,kd)=(u2(i1+1,i2,i3,kd)-u2(i1-1,i2,i3,kd))*h12(0)
      u2y23r(i1,i2,i3,kd)=(u2(i1,i2+1,i3,kd)-u2(i1,i2-1,i3,kd))*h12(1)
      u2z23r(i1,i2,i3,kd)=(u2(i1,i2,i3+1,kd)-u2(i1,i2,i3-1,kd))*h12(2)
      u2xx23r(i1,i2,i3,kd)=(-2.*u2(i1,i2,i3,kd)+(u2(i1+1,i2,i3,kd)+u2(
     & i1-1,i2,i3,kd)) )*h22(0)
      u2yy23r(i1,i2,i3,kd)=(-2.*u2(i1,i2,i3,kd)+(u2(i1,i2+1,i3,kd)+u2(
     & i1,i2-1,i3,kd)) )*h22(1)
      u2xy23r(i1,i2,i3,kd)=(u2x23r(i1,i2+1,i3,kd)-u2x23r(i1,i2-1,i3,kd)
     & )*h12(1)
      u2zz23r(i1,i2,i3,kd)=(-2.*u2(i1,i2,i3,kd)+(u2(i1,i2,i3+1,kd)+u2(
     & i1,i2,i3-1,kd)) )*h22(2)
      u2xz23r(i1,i2,i3,kd)=(u2x23r(i1,i2,i3+1,kd)-u2x23r(i1,i2,i3-1,kd)
     & )*h12(2)
      u2yz23r(i1,i2,i3,kd)=(u2y23r(i1,i2,i3+1,kd)-u2y23r(i1,i2,i3-1,kd)
     & )*h12(2)
      u2x21r(i1,i2,i3,kd)= u2x23r(i1,i2,i3,kd)
      u2y21r(i1,i2,i3,kd)= u2y23r(i1,i2,i3,kd)
      u2z21r(i1,i2,i3,kd)= u2z23r(i1,i2,i3,kd)
      u2xx21r(i1,i2,i3,kd)= u2xx23r(i1,i2,i3,kd)
      u2yy21r(i1,i2,i3,kd)= u2yy23r(i1,i2,i3,kd)
      u2zz21r(i1,i2,i3,kd)= u2zz23r(i1,i2,i3,kd)
      u2xy21r(i1,i2,i3,kd)= u2xy23r(i1,i2,i3,kd)
      u2xz21r(i1,i2,i3,kd)= u2xz23r(i1,i2,i3,kd)
      u2yz21r(i1,i2,i3,kd)= u2yz23r(i1,i2,i3,kd)
      u2laplacian21r(i1,i2,i3,kd)=u2xx23r(i1,i2,i3,kd)
      u2x22r(i1,i2,i3,kd)= u2x23r(i1,i2,i3,kd)
      u2y22r(i1,i2,i3,kd)= u2y23r(i1,i2,i3,kd)
      u2z22r(i1,i2,i3,kd)= u2z23r(i1,i2,i3,kd)
      u2xx22r(i1,i2,i3,kd)= u2xx23r(i1,i2,i3,kd)
      u2yy22r(i1,i2,i3,kd)= u2yy23r(i1,i2,i3,kd)
      u2zz22r(i1,i2,i3,kd)= u2zz23r(i1,i2,i3,kd)
      u2xy22r(i1,i2,i3,kd)= u2xy23r(i1,i2,i3,kd)
      u2xz22r(i1,i2,i3,kd)= u2xz23r(i1,i2,i3,kd)
      u2yz22r(i1,i2,i3,kd)= u2yz23r(i1,i2,i3,kd)
      u2laplacian22r(i1,i2,i3,kd)=u2xx23r(i1,i2,i3,kd)+u2yy23r(i1,i2,
     & i3,kd)
      u2laplacian23r(i1,i2,i3,kd)=u2xx23r(i1,i2,i3,kd)+u2yy23r(i1,i2,
     & i3,kd)+u2zz23r(i1,i2,i3,kd)
      u2xxx22r(i1,i2,i3,kd)=(-2.*(u2(i1+1,i2,i3,kd)-u2(i1-1,i2,i3,kd))+
     & (u2(i1+2,i2,i3,kd)-u2(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
      u2yyy22r(i1,i2,i3,kd)=(-2.*(u2(i1,i2+1,i3,kd)-u2(i1,i2-1,i3,kd))+
     & (u2(i1,i2+2,i3,kd)-u2(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
      u2xxy22r(i1,i2,i3,kd)=( u2xx22r(i1,i2+1,i3,kd)-u2xx22r(i1,i2-1,
     & i3,kd))/(2.*dx(1))
      u2xyy22r(i1,i2,i3,kd)=( u2yy22r(i1+1,i2,i3,kd)-u2yy22r(i1-1,i2,
     & i3,kd))/(2.*dx(0))
      u2xxxx22r(i1,i2,i3,kd)=(6.*u2(i1,i2,i3,kd)-4.*(u2(i1+1,i2,i3,kd)+
     & u2(i1-1,i2,i3,kd))+(u2(i1+2,i2,i3,kd)+u2(i1-2,i2,i3,kd)) )/(dx(
     & 0)**4)
      u2yyyy22r(i1,i2,i3,kd)=(6.*u2(i1,i2,i3,kd)-4.*(u2(i1,i2+1,i3,kd)+
     & u2(i1,i2-1,i3,kd))+(u2(i1,i2+2,i3,kd)+u2(i1,i2-2,i3,kd)) )/(dx(
     & 1)**4)
      u2xxyy22r(i1,i2,i3,kd)=( 4.*u2(i1,i2,i3,kd)     -2.*(u2(i1+1,i2,
     & i3,kd)+u2(i1-1,i2,i3,kd)+u2(i1,i2+1,i3,kd)+u2(i1,i2-1,i3,kd))  
     &  +   (u2(i1+1,i2+1,i3,kd)+u2(i1-1,i2+1,i3,kd)+u2(i1+1,i2-1,i3,
     & kd)+u2(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      ! 2D laplacian squared = u2.xxxx + 2 u2.xxyy + u2.yyyy
      u2LapSq22r(i1,i2,i3,kd)= ( 6.*u2(i1,i2,i3,kd)   - 4.*(u2(i1+1,i2,
     & i3,kd)+u2(i1-1,i2,i3,kd))    +(u2(i1+2,i2,i3,kd)+u2(i1-2,i2,i3,
     & kd)) )/(dx(0)**4) +( 6.*u2(i1,i2,i3,kd)    -4.*(u2(i1,i2+1,i3,
     & kd)+u2(i1,i2-1,i3,kd))    +(u2(i1,i2+2,i3,kd)+u2(i1,i2-2,i3,kd)
     & ) )/(dx(1)**4)  +( 8.*u2(i1,i2,i3,kd)     -4.*(u2(i1+1,i2,i3,
     & kd)+u2(i1-1,i2,i3,kd)+u2(i1,i2+1,i3,kd)+u2(i1,i2-1,i3,kd))   +
     & 2.*(u2(i1+1,i2+1,i3,kd)+u2(i1-1,i2+1,i3,kd)+u2(i1+1,i2-1,i3,kd)
     & +u2(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      u2xxx23r(i1,i2,i3,kd)=(-2.*(u2(i1+1,i2,i3,kd)-u2(i1-1,i2,i3,kd))+
     & (u2(i1+2,i2,i3,kd)-u2(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
      u2yyy23r(i1,i2,i3,kd)=(-2.*(u2(i1,i2+1,i3,kd)-u2(i1,i2-1,i3,kd))+
     & (u2(i1,i2+2,i3,kd)-u2(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
      u2zzz23r(i1,i2,i3,kd)=(-2.*(u2(i1,i2,i3+1,kd)-u2(i1,i2,i3-1,kd))+
     & (u2(i1,i2,i3+2,kd)-u2(i1,i2,i3-2,kd)) )*h22(1)*h12(2)
      u2xxy23r(i1,i2,i3,kd)=( u2xx22r(i1,i2+1,i3,kd)-u2xx22r(i1,i2-1,
     & i3,kd))/(2.*dx(1))
      u2xyy23r(i1,i2,i3,kd)=( u2yy22r(i1+1,i2,i3,kd)-u2yy22r(i1-1,i2,
     & i3,kd))/(2.*dx(0))
      u2xxz23r(i1,i2,i3,kd)=( u2xx22r(i1,i2,i3+1,kd)-u2xx22r(i1,i2,i3-
     & 1,kd))/(2.*dx(2))
      u2yyz23r(i1,i2,i3,kd)=( u2yy22r(i1,i2,i3+1,kd)-u2yy22r(i1,i2,i3-
     & 1,kd))/(2.*dx(2))
      u2xzz23r(i1,i2,i3,kd)=( u2zz22r(i1+1,i2,i3,kd)-u2zz22r(i1-1,i2,
     & i3,kd))/(2.*dx(0))
      u2yzz23r(i1,i2,i3,kd)=( u2zz22r(i1,i2+1,i3,kd)-u2zz22r(i1,i2-1,
     & i3,kd))/(2.*dx(1))
      u2xxxx23r(i1,i2,i3,kd)=(6.*u2(i1,i2,i3,kd)-4.*(u2(i1+1,i2,i3,kd)+
     & u2(i1-1,i2,i3,kd))+(u2(i1+2,i2,i3,kd)+u2(i1-2,i2,i3,kd)) )/(dx(
     & 0)**4)
      u2yyyy23r(i1,i2,i3,kd)=(6.*u2(i1,i2,i3,kd)-4.*(u2(i1,i2+1,i3,kd)+
     & u2(i1,i2-1,i3,kd))+(u2(i1,i2+2,i3,kd)+u2(i1,i2-2,i3,kd)) )/(dx(
     & 1)**4)
      u2zzzz23r(i1,i2,i3,kd)=(6.*u2(i1,i2,i3,kd)-4.*(u2(i1,i2,i3+1,kd)+
     & u2(i1,i2,i3-1,kd))+(u2(i1,i2,i3+2,kd)+u2(i1,i2,i3-2,kd)) )/(dx(
     & 2)**4)
      u2xxyy23r(i1,i2,i3,kd)=( 4.*u2(i1,i2,i3,kd)     -2.*(u2(i1+1,i2,
     & i3,kd)+u2(i1-1,i2,i3,kd)+u2(i1,i2+1,i3,kd)+u2(i1,i2-1,i3,kd))  
     &  +   (u2(i1+1,i2+1,i3,kd)+u2(i1-1,i2+1,i3,kd)+u2(i1+1,i2-1,i3,
     & kd)+u2(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      u2xxzz23r(i1,i2,i3,kd)=( 4.*u2(i1,i2,i3,kd)     -2.*(u2(i1+1,i2,
     & i3,kd)+u2(i1-1,i2,i3,kd)+u2(i1,i2,i3+1,kd)+u2(i1,i2,i3-1,kd))  
     &  +   (u2(i1+1,i2,i3+1,kd)+u2(i1-1,i2,i3+1,kd)+u2(i1+1,i2,i3-1,
     & kd)+u2(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)
      u2yyzz23r(i1,i2,i3,kd)=( 4.*u2(i1,i2,i3,kd)     -2.*(u2(i1,i2+1,
     & i3,kd)  +u2(i1,i2-1,i3,kd)+  u2(i1,i2  ,i3+1,kd)+u2(i1,i2  ,i3-
     & 1,kd))   +   (u2(i1,i2+1,i3+1,kd)+u2(i1,i2-1,i3+1,kd)+u2(i1,i2+
     & 1,i3-1,kd)+u2(i1,i2-1,i3-1,kd)) )/(dx(1)**2*dx(2)**2)
      ! 3D laplacian squared = u2.xxxx + u2.yyyy + u2.zzzz + 2 (u2.xxyy + u2.xxzz + u2.yyzz )
      u2LapSq23r(i1,i2,i3,kd)= ( 6.*u2(i1,i2,i3,kd)   - 4.*(u2(i1+1,i2,
     & i3,kd)+u2(i1-1,i2,i3,kd))    +(u2(i1+2,i2,i3,kd)+u2(i1-2,i2,i3,
     & kd)) )/(dx(0)**4) +( 6.*u2(i1,i2,i3,kd)    -4.*(u2(i1,i2+1,i3,
     & kd)+u2(i1,i2-1,i3,kd))    +(u2(i1,i2+2,i3,kd)+u2(i1,i2-2,i3,kd)
     & ) )/(dx(1)**4)  +( 6.*u2(i1,i2,i3,kd)    -4.*(u2(i1,i2,i3+1,kd)
     & +u2(i1,i2,i3-1,kd))    +(u2(i1,i2,i3+2,kd)+u2(i1,i2,i3-2,kd)) )
     & /(dx(2)**4)  +( 8.*u2(i1,i2,i3,kd)     -4.*(u2(i1+1,i2,i3,kd)  
     & +u2(i1-1,i2,i3,kd)  +u2(i1  ,i2+1,i3,kd)+u2(i1  ,i2-1,i3,kd))  
     &  +2.*(u2(i1+1,i2+1,i3,kd)+u2(i1-1,i2+1,i3,kd)+u2(i1+1,i2-1,i3,
     & kd)+u2(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)+( 8.*u2(i1,i2,
     & i3,kd)     -4.*(u2(i1+1,i2,i3,kd)  +u2(i1-1,i2,i3,kd)  +u2(i1  
     & ,i2,i3+1,kd)+u2(i1  ,i2,i3-1,kd))   +2.*(u2(i1+1,i2,i3+1,kd)+
     & u2(i1-1,i2,i3+1,kd)+u2(i1+1,i2,i3-1,kd)+u2(i1-1,i2,i3-1,kd)) )
     & /(dx(0)**2*dx(2)**2)+( 8.*u2(i1,i2,i3,kd)     -4.*(u2(i1,i2+1,
     & i3,kd)  +u2(i1,i2-1,i3,kd)  +u2(i1,i2  ,i3+1,kd)+u2(i1,i2  ,i3-
     & 1,kd))   +2.*(u2(i1,i2+1,i3+1,kd)+u2(i1,i2-1,i3+1,kd)+u2(i1,i2+
     & 1,i3-1,kd)+u2(i1,i2-1,i3-1,kd)) )/(dx(1)**2*dx(2)**2)
      gvr2(i1,i2,i3,kd)=(gv(i1+1,i2,i3,kd)-gv(i1-1,i2,i3,kd))*d12(0)
      gvs2(i1,i2,i3,kd)=(gv(i1,i2+1,i3,kd)-gv(i1,i2-1,i3,kd))*d12(1)
      gvt2(i1,i2,i3,kd)=(gv(i1,i2,i3+1,kd)-gv(i1,i2,i3-1,kd))*d12(2)
      gvrr2(i1,i2,i3,kd)=(-2.*gv(i1,i2,i3,kd)+(gv(i1+1,i2,i3,kd)+gv(i1-
     & 1,i2,i3,kd)) )*d22(0)
      gvss2(i1,i2,i3,kd)=(-2.*gv(i1,i2,i3,kd)+(gv(i1,i2+1,i3,kd)+gv(i1,
     & i2-1,i3,kd)) )*d22(1)
      gvrs2(i1,i2,i3,kd)=(gvr2(i1,i2+1,i3,kd)-gvr2(i1,i2-1,i3,kd))*d12(
     & 1)
      gvtt2(i1,i2,i3,kd)=(-2.*gv(i1,i2,i3,kd)+(gv(i1,i2,i3+1,kd)+gv(i1,
     & i2,i3-1,kd)) )*d22(2)
      gvrt2(i1,i2,i3,kd)=(gvr2(i1,i2,i3+1,kd)-gvr2(i1,i2,i3-1,kd))*d12(
     & 2)
      gvst2(i1,i2,i3,kd)=(gvs2(i1,i2,i3+1,kd)-gvs2(i1,i2,i3-1,kd))*d12(
     & 2)
      gvrrr2(i1,i2,i3,kd)=(-2.*(gv(i1+1,i2,i3,kd)-gv(i1-1,i2,i3,kd))+(
     & gv(i1+2,i2,i3,kd)-gv(i1-2,i2,i3,kd)) )*d22(0)*d12(0)
      gvsss2(i1,i2,i3,kd)=(-2.*(gv(i1,i2+1,i3,kd)-gv(i1,i2-1,i3,kd))+(
     & gv(i1,i2+2,i3,kd)-gv(i1,i2-2,i3,kd)) )*d22(1)*d12(1)
      gvttt2(i1,i2,i3,kd)=(-2.*(gv(i1,i2,i3+1,kd)-gv(i1,i2,i3-1,kd))+(
     & gv(i1,i2,i3+2,kd)-gv(i1,i2,i3-2,kd)) )*d22(2)*d12(2)
      gvx21(i1,i2,i3,kd)= rx(i1,i2,i3)*gvr2(i1,i2,i3,kd)
      gvy21(i1,i2,i3,kd)=0
      gvz21(i1,i2,i3,kd)=0
      gvx22(i1,i2,i3,kd)= rx(i1,i2,i3)*gvr2(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & gvs2(i1,i2,i3,kd)
      gvy22(i1,i2,i3,kd)= ry(i1,i2,i3)*gvr2(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & gvs2(i1,i2,i3,kd)
      gvz22(i1,i2,i3,kd)=0
      gvx23(i1,i2,i3,kd)=rx(i1,i2,i3)*gvr2(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & gvs2(i1,i2,i3,kd)+tx(i1,i2,i3)*gvt2(i1,i2,i3,kd)
      gvy23(i1,i2,i3,kd)=ry(i1,i2,i3)*gvr2(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & gvs2(i1,i2,i3,kd)+ty(i1,i2,i3)*gvt2(i1,i2,i3,kd)
      gvz23(i1,i2,i3,kd)=rz(i1,i2,i3)*gvr2(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & gvs2(i1,i2,i3,kd)+tz(i1,i2,i3)*gvt2(i1,i2,i3,kd)
      gvxx21(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*gvrr2(i1,i2,i3,kd)+(rxx22(
     & i1,i2,i3))*gvr2(i1,i2,i3,kd)
      gvyy21(i1,i2,i3,kd)=0
      gvxy21(i1,i2,i3,kd)=0
      gvxz21(i1,i2,i3,kd)=0
      gvyz21(i1,i2,i3,kd)=0
      gvzz21(i1,i2,i3,kd)=0
      gvlaplacian21(i1,i2,i3,kd)=gvxx21(i1,i2,i3,kd)
      gvxx22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*gvrr2(i1,i2,i3,kd)+2.*(rx(
     & i1,i2,i3)*sx(i1,i2,i3))*gvrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2)*
     & gvss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3))*gvr2(i1,i2,i3,kd)+(sxx22(
     & i1,i2,i3))*gvs2(i1,i2,i3,kd)
      gvyy22(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*gvrr2(i1,i2,i3,kd)+2.*(ry(
     & i1,i2,i3)*sy(i1,i2,i3))*gvrs2(i1,i2,i3,kd)+(sy(i1,i2,i3)**2)*
     & gvss2(i1,i2,i3,kd)+(ryy22(i1,i2,i3))*gvr2(i1,i2,i3,kd)+(syy22(
     & i1,i2,i3))*gvs2(i1,i2,i3,kd)
      gvxy22(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*gvrr2(i1,i2,i3,kd)+
     & (rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*gvrs2(i1,
     & i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*gvss2(i1,i2,i3,kd)+rxy22(
     & i1,i2,i3)*gvr2(i1,i2,i3,kd)+sxy22(i1,i2,i3)*gvs2(i1,i2,i3,kd)
      gvxz22(i1,i2,i3,kd)=0
      gvyz22(i1,i2,i3,kd)=0
      gvzz22(i1,i2,i3,kd)=0
      gvlaplacian22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & gvrr2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*
     & sy(i1,i2,i3))*gvrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)*
     & *2)*gvss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3)+ryy22(i1,i2,i3))*gvr2(
     & i1,i2,i3,kd)+(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*gvs2(i1,i2,i3,
     & kd)
      gvxx23(i1,i2,i3,kd)=rx(i1,i2,i3)**2*gvrr2(i1,i2,i3,kd)+sx(i1,i2,
     & i3)**2*gvss2(i1,i2,i3,kd)+tx(i1,i2,i3)**2*gvtt2(i1,i2,i3,kd)+
     & 2.*rx(i1,i2,i3)*sx(i1,i2,i3)*gvrs2(i1,i2,i3,kd)+2.*rx(i1,i2,i3)
     & *tx(i1,i2,i3)*gvrt2(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,i2,i3)*
     & gvst2(i1,i2,i3,kd)+rxx23(i1,i2,i3)*gvr2(i1,i2,i3,kd)+sxx23(i1,
     & i2,i3)*gvs2(i1,i2,i3,kd)+txx23(i1,i2,i3)*gvt2(i1,i2,i3,kd)
      gvyy23(i1,i2,i3,kd)=ry(i1,i2,i3)**2*gvrr2(i1,i2,i3,kd)+sy(i1,i2,
     & i3)**2*gvss2(i1,i2,i3,kd)+ty(i1,i2,i3)**2*gvtt2(i1,i2,i3,kd)+
     & 2.*ry(i1,i2,i3)*sy(i1,i2,i3)*gvrs2(i1,i2,i3,kd)+2.*ry(i1,i2,i3)
     & *ty(i1,i2,i3)*gvrt2(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,i2,i3)*
     & gvst2(i1,i2,i3,kd)+ryy23(i1,i2,i3)*gvr2(i1,i2,i3,kd)+syy23(i1,
     & i2,i3)*gvs2(i1,i2,i3,kd)+tyy23(i1,i2,i3)*gvt2(i1,i2,i3,kd)
      gvzz23(i1,i2,i3,kd)=rz(i1,i2,i3)**2*gvrr2(i1,i2,i3,kd)+sz(i1,i2,
     & i3)**2*gvss2(i1,i2,i3,kd)+tz(i1,i2,i3)**2*gvtt2(i1,i2,i3,kd)+
     & 2.*rz(i1,i2,i3)*sz(i1,i2,i3)*gvrs2(i1,i2,i3,kd)+2.*rz(i1,i2,i3)
     & *tz(i1,i2,i3)*gvrt2(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,i2,i3)*
     & gvst2(i1,i2,i3,kd)+rzz23(i1,i2,i3)*gvr2(i1,i2,i3,kd)+szz23(i1,
     & i2,i3)*gvs2(i1,i2,i3,kd)+tzz23(i1,i2,i3)*gvt2(i1,i2,i3,kd)
      gvxy23(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*gvrr2(i1,i2,i3,kd)+
     & sx(i1,i2,i3)*sy(i1,i2,i3)*gvss2(i1,i2,i3,kd)+tx(i1,i2,i3)*ty(
     & i1,i2,i3)*gvtt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,
     & i2,i3)*sx(i1,i2,i3))*gvrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(i1,i2,
     & i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*gvrt2(i1,i2,i3,kd)+(sx(i1,i2,i3)
     & *ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*gvst2(i1,i2,i3,kd)+
     & rxy23(i1,i2,i3)*gvr2(i1,i2,i3,kd)+sxy23(i1,i2,i3)*gvs2(i1,i2,
     & i3,kd)+txy23(i1,i2,i3)*gvt2(i1,i2,i3,kd)
      gvxz23(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*gvrr2(i1,i2,i3,kd)+
     & sx(i1,i2,i3)*sz(i1,i2,i3)*gvss2(i1,i2,i3,kd)+tx(i1,i2,i3)*tz(
     & i1,i2,i3)*gvtt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,
     & i2,i3)*sx(i1,i2,i3))*gvrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(i1,i2,
     & i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*gvrt2(i1,i2,i3,kd)+(sx(i1,i2,i3)
     & *tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*gvst2(i1,i2,i3,kd)+
     & rxz23(i1,i2,i3)*gvr2(i1,i2,i3,kd)+sxz23(i1,i2,i3)*gvs2(i1,i2,
     & i3,kd)+txz23(i1,i2,i3)*gvt2(i1,i2,i3,kd)
      gvyz23(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*gvrr2(i1,i2,i3,kd)+
     & sy(i1,i2,i3)*sz(i1,i2,i3)*gvss2(i1,i2,i3,kd)+ty(i1,i2,i3)*tz(
     & i1,i2,i3)*gvtt2(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,
     & i2,i3)*sy(i1,i2,i3))*gvrs2(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(i1,i2,
     & i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*gvrt2(i1,i2,i3,kd)+(sy(i1,i2,i3)
     & *tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*gvst2(i1,i2,i3,kd)+
     & ryz23(i1,i2,i3)*gvr2(i1,i2,i3,kd)+syz23(i1,i2,i3)*gvs2(i1,i2,
     & i3,kd)+tyz23(i1,i2,i3)*gvt2(i1,i2,i3,kd)
      gvlaplacian23(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*gvrr2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)*
     & *2+sz(i1,i2,i3)**2)*gvss2(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(i1,
     & i2,i3)**2+tz(i1,i2,i3)**2)*gvtt2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*
     & sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,
     & i3))*gvrs2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(i1,
     & i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*gvrt2(i1,i2,i3,
     & kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)+
     & sz(i1,i2,i3)*tz(i1,i2,i3))*gvst2(i1,i2,i3,kd)+(rxx23(i1,i2,i3)+
     & ryy23(i1,i2,i3)+rzz23(i1,i2,i3))*gvr2(i1,i2,i3,kd)+(sxx23(i1,
     & i2,i3)+syy23(i1,i2,i3)+szz23(i1,i2,i3))*gvs2(i1,i2,i3,kd)+(
     & txx23(i1,i2,i3)+tyy23(i1,i2,i3)+tzz23(i1,i2,i3))*gvt2(i1,i2,i3,
     & kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      gvx23r(i1,i2,i3,kd)=(gv(i1+1,i2,i3,kd)-gv(i1-1,i2,i3,kd))*h12(0)
      gvy23r(i1,i2,i3,kd)=(gv(i1,i2+1,i3,kd)-gv(i1,i2-1,i3,kd))*h12(1)
      gvz23r(i1,i2,i3,kd)=(gv(i1,i2,i3+1,kd)-gv(i1,i2,i3-1,kd))*h12(2)
      gvxx23r(i1,i2,i3,kd)=(-2.*gv(i1,i2,i3,kd)+(gv(i1+1,i2,i3,kd)+gv(
     & i1-1,i2,i3,kd)) )*h22(0)
      gvyy23r(i1,i2,i3,kd)=(-2.*gv(i1,i2,i3,kd)+(gv(i1,i2+1,i3,kd)+gv(
     & i1,i2-1,i3,kd)) )*h22(1)
      gvxy23r(i1,i2,i3,kd)=(gvx23r(i1,i2+1,i3,kd)-gvx23r(i1,i2-1,i3,kd)
     & )*h12(1)
      gvzz23r(i1,i2,i3,kd)=(-2.*gv(i1,i2,i3,kd)+(gv(i1,i2,i3+1,kd)+gv(
     & i1,i2,i3-1,kd)) )*h22(2)
      gvxz23r(i1,i2,i3,kd)=(gvx23r(i1,i2,i3+1,kd)-gvx23r(i1,i2,i3-1,kd)
     & )*h12(2)
      gvyz23r(i1,i2,i3,kd)=(gvy23r(i1,i2,i3+1,kd)-gvy23r(i1,i2,i3-1,kd)
     & )*h12(2)
      gvx21r(i1,i2,i3,kd)= gvx23r(i1,i2,i3,kd)
      gvy21r(i1,i2,i3,kd)= gvy23r(i1,i2,i3,kd)
      gvz21r(i1,i2,i3,kd)= gvz23r(i1,i2,i3,kd)
      gvxx21r(i1,i2,i3,kd)= gvxx23r(i1,i2,i3,kd)
      gvyy21r(i1,i2,i3,kd)= gvyy23r(i1,i2,i3,kd)
      gvzz21r(i1,i2,i3,kd)= gvzz23r(i1,i2,i3,kd)
      gvxy21r(i1,i2,i3,kd)= gvxy23r(i1,i2,i3,kd)
      gvxz21r(i1,i2,i3,kd)= gvxz23r(i1,i2,i3,kd)
      gvyz21r(i1,i2,i3,kd)= gvyz23r(i1,i2,i3,kd)
      gvlaplacian21r(i1,i2,i3,kd)=gvxx23r(i1,i2,i3,kd)
      gvx22r(i1,i2,i3,kd)= gvx23r(i1,i2,i3,kd)
      gvy22r(i1,i2,i3,kd)= gvy23r(i1,i2,i3,kd)
      gvz22r(i1,i2,i3,kd)= gvz23r(i1,i2,i3,kd)
      gvxx22r(i1,i2,i3,kd)= gvxx23r(i1,i2,i3,kd)
      gvyy22r(i1,i2,i3,kd)= gvyy23r(i1,i2,i3,kd)
      gvzz22r(i1,i2,i3,kd)= gvzz23r(i1,i2,i3,kd)
      gvxy22r(i1,i2,i3,kd)= gvxy23r(i1,i2,i3,kd)
      gvxz22r(i1,i2,i3,kd)= gvxz23r(i1,i2,i3,kd)
      gvyz22r(i1,i2,i3,kd)= gvyz23r(i1,i2,i3,kd)
      gvlaplacian22r(i1,i2,i3,kd)=gvxx23r(i1,i2,i3,kd)+gvyy23r(i1,i2,
     & i3,kd)
      gvlaplacian23r(i1,i2,i3,kd)=gvxx23r(i1,i2,i3,kd)+gvyy23r(i1,i2,
     & i3,kd)+gvzz23r(i1,i2,i3,kd)
      gvxxx22r(i1,i2,i3,kd)=(-2.*(gv(i1+1,i2,i3,kd)-gv(i1-1,i2,i3,kd))+
     & (gv(i1+2,i2,i3,kd)-gv(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
      gvyyy22r(i1,i2,i3,kd)=(-2.*(gv(i1,i2+1,i3,kd)-gv(i1,i2-1,i3,kd))+
     & (gv(i1,i2+2,i3,kd)-gv(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
      gvxxy22r(i1,i2,i3,kd)=( gvxx22r(i1,i2+1,i3,kd)-gvxx22r(i1,i2-1,
     & i3,kd))/(2.*dx(1))
      gvxyy22r(i1,i2,i3,kd)=( gvyy22r(i1+1,i2,i3,kd)-gvyy22r(i1-1,i2,
     & i3,kd))/(2.*dx(0))
      gvxxxx22r(i1,i2,i3,kd)=(6.*gv(i1,i2,i3,kd)-4.*(gv(i1+1,i2,i3,kd)+
     & gv(i1-1,i2,i3,kd))+(gv(i1+2,i2,i3,kd)+gv(i1-2,i2,i3,kd)) )/(dx(
     & 0)**4)
      gvyyyy22r(i1,i2,i3,kd)=(6.*gv(i1,i2,i3,kd)-4.*(gv(i1,i2+1,i3,kd)+
     & gv(i1,i2-1,i3,kd))+(gv(i1,i2+2,i3,kd)+gv(i1,i2-2,i3,kd)) )/(dx(
     & 1)**4)
      gvxxyy22r(i1,i2,i3,kd)=( 4.*gv(i1,i2,i3,kd)     -2.*(gv(i1+1,i2,
     & i3,kd)+gv(i1-1,i2,i3,kd)+gv(i1,i2+1,i3,kd)+gv(i1,i2-1,i3,kd))  
     &  +   (gv(i1+1,i2+1,i3,kd)+gv(i1-1,i2+1,i3,kd)+gv(i1+1,i2-1,i3,
     & kd)+gv(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      ! 2D laplacian squared = gv.xxxx + 2 gv.xxyy + gv.yyyy
      gvLapSq22r(i1,i2,i3,kd)= ( 6.*gv(i1,i2,i3,kd)   - 4.*(gv(i1+1,i2,
     & i3,kd)+gv(i1-1,i2,i3,kd))    +(gv(i1+2,i2,i3,kd)+gv(i1-2,i2,i3,
     & kd)) )/(dx(0)**4) +( 6.*gv(i1,i2,i3,kd)    -4.*(gv(i1,i2+1,i3,
     & kd)+gv(i1,i2-1,i3,kd))    +(gv(i1,i2+2,i3,kd)+gv(i1,i2-2,i3,kd)
     & ) )/(dx(1)**4)  +( 8.*gv(i1,i2,i3,kd)     -4.*(gv(i1+1,i2,i3,
     & kd)+gv(i1-1,i2,i3,kd)+gv(i1,i2+1,i3,kd)+gv(i1,i2-1,i3,kd))   +
     & 2.*(gv(i1+1,i2+1,i3,kd)+gv(i1-1,i2+1,i3,kd)+gv(i1+1,i2-1,i3,kd)
     & +gv(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      gvxxx23r(i1,i2,i3,kd)=(-2.*(gv(i1+1,i2,i3,kd)-gv(i1-1,i2,i3,kd))+
     & (gv(i1+2,i2,i3,kd)-gv(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
      gvyyy23r(i1,i2,i3,kd)=(-2.*(gv(i1,i2+1,i3,kd)-gv(i1,i2-1,i3,kd))+
     & (gv(i1,i2+2,i3,kd)-gv(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
      gvzzz23r(i1,i2,i3,kd)=(-2.*(gv(i1,i2,i3+1,kd)-gv(i1,i2,i3-1,kd))+
     & (gv(i1,i2,i3+2,kd)-gv(i1,i2,i3-2,kd)) )*h22(1)*h12(2)
      gvxxy23r(i1,i2,i3,kd)=( gvxx22r(i1,i2+1,i3,kd)-gvxx22r(i1,i2-1,
     & i3,kd))/(2.*dx(1))
      gvxyy23r(i1,i2,i3,kd)=( gvyy22r(i1+1,i2,i3,kd)-gvyy22r(i1-1,i2,
     & i3,kd))/(2.*dx(0))
      gvxxz23r(i1,i2,i3,kd)=( gvxx22r(i1,i2,i3+1,kd)-gvxx22r(i1,i2,i3-
     & 1,kd))/(2.*dx(2))
      gvyyz23r(i1,i2,i3,kd)=( gvyy22r(i1,i2,i3+1,kd)-gvyy22r(i1,i2,i3-
     & 1,kd))/(2.*dx(2))
      gvxzz23r(i1,i2,i3,kd)=( gvzz22r(i1+1,i2,i3,kd)-gvzz22r(i1-1,i2,
     & i3,kd))/(2.*dx(0))
      gvyzz23r(i1,i2,i3,kd)=( gvzz22r(i1,i2+1,i3,kd)-gvzz22r(i1,i2-1,
     & i3,kd))/(2.*dx(1))
      gvxxxx23r(i1,i2,i3,kd)=(6.*gv(i1,i2,i3,kd)-4.*(gv(i1+1,i2,i3,kd)+
     & gv(i1-1,i2,i3,kd))+(gv(i1+2,i2,i3,kd)+gv(i1-2,i2,i3,kd)) )/(dx(
     & 0)**4)
      gvyyyy23r(i1,i2,i3,kd)=(6.*gv(i1,i2,i3,kd)-4.*(gv(i1,i2+1,i3,kd)+
     & gv(i1,i2-1,i3,kd))+(gv(i1,i2+2,i3,kd)+gv(i1,i2-2,i3,kd)) )/(dx(
     & 1)**4)
      gvzzzz23r(i1,i2,i3,kd)=(6.*gv(i1,i2,i3,kd)-4.*(gv(i1,i2,i3+1,kd)+
     & gv(i1,i2,i3-1,kd))+(gv(i1,i2,i3+2,kd)+gv(i1,i2,i3-2,kd)) )/(dx(
     & 2)**4)
      gvxxyy23r(i1,i2,i3,kd)=( 4.*gv(i1,i2,i3,kd)     -2.*(gv(i1+1,i2,
     & i3,kd)+gv(i1-1,i2,i3,kd)+gv(i1,i2+1,i3,kd)+gv(i1,i2-1,i3,kd))  
     &  +   (gv(i1+1,i2+1,i3,kd)+gv(i1-1,i2+1,i3,kd)+gv(i1+1,i2-1,i3,
     & kd)+gv(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      gvxxzz23r(i1,i2,i3,kd)=( 4.*gv(i1,i2,i3,kd)     -2.*(gv(i1+1,i2,
     & i3,kd)+gv(i1-1,i2,i3,kd)+gv(i1,i2,i3+1,kd)+gv(i1,i2,i3-1,kd))  
     &  +   (gv(i1+1,i2,i3+1,kd)+gv(i1-1,i2,i3+1,kd)+gv(i1+1,i2,i3-1,
     & kd)+gv(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)
      gvyyzz23r(i1,i2,i3,kd)=( 4.*gv(i1,i2,i3,kd)     -2.*(gv(i1,i2+1,
     & i3,kd)  +gv(i1,i2-1,i3,kd)+  gv(i1,i2  ,i3+1,kd)+gv(i1,i2  ,i3-
     & 1,kd))   +   (gv(i1,i2+1,i3+1,kd)+gv(i1,i2-1,i3+1,kd)+gv(i1,i2+
     & 1,i3-1,kd)+gv(i1,i2-1,i3-1,kd)) )/(dx(1)**2*dx(2)**2)
      ! 3D laplacian squared = gv.xxxx + gv.yyyy + gv.zzzz + 2 (gv.xxyy + gv.xxzz + gv.yyzz )
      gvLapSq23r(i1,i2,i3,kd)= ( 6.*gv(i1,i2,i3,kd)   - 4.*(gv(i1+1,i2,
     & i3,kd)+gv(i1-1,i2,i3,kd))    +(gv(i1+2,i2,i3,kd)+gv(i1-2,i2,i3,
     & kd)) )/(dx(0)**4) +( 6.*gv(i1,i2,i3,kd)    -4.*(gv(i1,i2+1,i3,
     & kd)+gv(i1,i2-1,i3,kd))    +(gv(i1,i2+2,i3,kd)+gv(i1,i2-2,i3,kd)
     & ) )/(dx(1)**4)  +( 6.*gv(i1,i2,i3,kd)    -4.*(gv(i1,i2,i3+1,kd)
     & +gv(i1,i2,i3-1,kd))    +(gv(i1,i2,i3+2,kd)+gv(i1,i2,i3-2,kd)) )
     & /(dx(2)**4)  +( 8.*gv(i1,i2,i3,kd)     -4.*(gv(i1+1,i2,i3,kd)  
     & +gv(i1-1,i2,i3,kd)  +gv(i1  ,i2+1,i3,kd)+gv(i1  ,i2-1,i3,kd))  
     &  +2.*(gv(i1+1,i2+1,i3,kd)+gv(i1-1,i2+1,i3,kd)+gv(i1+1,i2-1,i3,
     & kd)+gv(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)+( 8.*gv(i1,i2,
     & i3,kd)     -4.*(gv(i1+1,i2,i3,kd)  +gv(i1-1,i2,i3,kd)  +gv(i1  
     & ,i2,i3+1,kd)+gv(i1  ,i2,i3-1,kd))   +2.*(gv(i1+1,i2,i3+1,kd)+
     & gv(i1-1,i2,i3+1,kd)+gv(i1+1,i2,i3-1,kd)+gv(i1-1,i2,i3-1,kd)) )
     & /(dx(0)**2*dx(2)**2)+( 8.*gv(i1,i2,i3,kd)     -4.*(gv(i1,i2+1,
     & i3,kd)  +gv(i1,i2-1,i3,kd)  +gv(i1,i2  ,i3+1,kd)+gv(i1,i2  ,i3-
     & 1,kd))   +2.*(gv(i1,i2+1,i3+1,kd)+gv(i1,i2-1,i3+1,kd)+gv(i1,i2+
     & 1,i3-1,kd)+gv(i1,i2-1,i3-1,kd)) )/(dx(1)**2*dx(2)**2)
      gv2r2(i1,i2,i3,kd)=(gv2(i1+1,i2,i3,kd)-gv2(i1-1,i2,i3,kd))*d12(0)
      gv2s2(i1,i2,i3,kd)=(gv2(i1,i2+1,i3,kd)-gv2(i1,i2-1,i3,kd))*d12(1)
      gv2t2(i1,i2,i3,kd)=(gv2(i1,i2,i3+1,kd)-gv2(i1,i2,i3-1,kd))*d12(2)
      gv2rr2(i1,i2,i3,kd)=(-2.*gv2(i1,i2,i3,kd)+(gv2(i1+1,i2,i3,kd)+
     & gv2(i1-1,i2,i3,kd)) )*d22(0)
      gv2ss2(i1,i2,i3,kd)=(-2.*gv2(i1,i2,i3,kd)+(gv2(i1,i2+1,i3,kd)+
     & gv2(i1,i2-1,i3,kd)) )*d22(1)
      gv2rs2(i1,i2,i3,kd)=(gv2r2(i1,i2+1,i3,kd)-gv2r2(i1,i2-1,i3,kd))*
     & d12(1)
      gv2tt2(i1,i2,i3,kd)=(-2.*gv2(i1,i2,i3,kd)+(gv2(i1,i2,i3+1,kd)+
     & gv2(i1,i2,i3-1,kd)) )*d22(2)
      gv2rt2(i1,i2,i3,kd)=(gv2r2(i1,i2,i3+1,kd)-gv2r2(i1,i2,i3-1,kd))*
     & d12(2)
      gv2st2(i1,i2,i3,kd)=(gv2s2(i1,i2,i3+1,kd)-gv2s2(i1,i2,i3-1,kd))*
     & d12(2)
      gv2rrr2(i1,i2,i3,kd)=(-2.*(gv2(i1+1,i2,i3,kd)-gv2(i1-1,i2,i3,kd))
     & +(gv2(i1+2,i2,i3,kd)-gv2(i1-2,i2,i3,kd)) )*d22(0)*d12(0)
      gv2sss2(i1,i2,i3,kd)=(-2.*(gv2(i1,i2+1,i3,kd)-gv2(i1,i2-1,i3,kd))
     & +(gv2(i1,i2+2,i3,kd)-gv2(i1,i2-2,i3,kd)) )*d22(1)*d12(1)
      gv2ttt2(i1,i2,i3,kd)=(-2.*(gv2(i1,i2,i3+1,kd)-gv2(i1,i2,i3-1,kd))
     & +(gv2(i1,i2,i3+2,kd)-gv2(i1,i2,i3-2,kd)) )*d22(2)*d12(2)
      gv2x21(i1,i2,i3,kd)= rx(i1,i2,i3)*gv2r2(i1,i2,i3,kd)
      gv2y21(i1,i2,i3,kd)=0
      gv2z21(i1,i2,i3,kd)=0
      gv2x22(i1,i2,i3,kd)= rx(i1,i2,i3)*gv2r2(i1,i2,i3,kd)+sx(i1,i2,i3)
     & *gv2s2(i1,i2,i3,kd)
      gv2y22(i1,i2,i3,kd)= ry(i1,i2,i3)*gv2r2(i1,i2,i3,kd)+sy(i1,i2,i3)
     & *gv2s2(i1,i2,i3,kd)
      gv2z22(i1,i2,i3,kd)=0
      gv2x23(i1,i2,i3,kd)=rx(i1,i2,i3)*gv2r2(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & gv2s2(i1,i2,i3,kd)+tx(i1,i2,i3)*gv2t2(i1,i2,i3,kd)
      gv2y23(i1,i2,i3,kd)=ry(i1,i2,i3)*gv2r2(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & gv2s2(i1,i2,i3,kd)+ty(i1,i2,i3)*gv2t2(i1,i2,i3,kd)
      gv2z23(i1,i2,i3,kd)=rz(i1,i2,i3)*gv2r2(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & gv2s2(i1,i2,i3,kd)+tz(i1,i2,i3)*gv2t2(i1,i2,i3,kd)
      gv2xx21(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*gv2rr2(i1,i2,i3,kd)+(
     & rxx22(i1,i2,i3))*gv2r2(i1,i2,i3,kd)
      gv2yy21(i1,i2,i3,kd)=0
      gv2xy21(i1,i2,i3,kd)=0
      gv2xz21(i1,i2,i3,kd)=0
      gv2yz21(i1,i2,i3,kd)=0
      gv2zz21(i1,i2,i3,kd)=0
      gv2laplacian21(i1,i2,i3,kd)=gv2xx21(i1,i2,i3,kd)
      gv2xx22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*gv2rr2(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*gv2rs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*gv2ss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3))*gv2r2(i1,i2,i3,kd)+(
     & sxx22(i1,i2,i3))*gv2s2(i1,i2,i3,kd)
      gv2yy22(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*gv2rr2(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*gv2rs2(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*gv2ss2(i1,i2,i3,kd)+(ryy22(i1,i2,i3))*gv2r2(i1,i2,i3,kd)+(
     & syy22(i1,i2,i3))*gv2s2(i1,i2,i3,kd)
      gv2xy22(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*gv2rr2(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & gv2rs2(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*gv2ss2(i1,i2,i3,
     & kd)+rxy22(i1,i2,i3)*gv2r2(i1,i2,i3,kd)+sxy22(i1,i2,i3)*gv2s2(
     & i1,i2,i3,kd)
      gv2xz22(i1,i2,i3,kd)=0
      gv2yz22(i1,i2,i3,kd)=0
      gv2zz22(i1,i2,i3,kd)=0
      gv2laplacian22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & gv2rr2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)
     & *sy(i1,i2,i3))*gv2rs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2)*gv2ss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3)+ryy22(i1,i2,i3))*
     & gv2r2(i1,i2,i3,kd)+(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*gv2s2(i1,
     & i2,i3,kd)
      gv2xx23(i1,i2,i3,kd)=rx(i1,i2,i3)**2*gv2rr2(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*gv2ss2(i1,i2,i3,kd)+tx(i1,i2,i3)**2*gv2tt2(i1,i2,i3,
     & kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*gv2rs2(i1,i2,i3,kd)+2.*rx(i1,
     & i2,i3)*tx(i1,i2,i3)*gv2rt2(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,
     & i2,i3)*gv2st2(i1,i2,i3,kd)+rxx23(i1,i2,i3)*gv2r2(i1,i2,i3,kd)+
     & sxx23(i1,i2,i3)*gv2s2(i1,i2,i3,kd)+txx23(i1,i2,i3)*gv2t2(i1,i2,
     & i3,kd)
      gv2yy23(i1,i2,i3,kd)=ry(i1,i2,i3)**2*gv2rr2(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*gv2ss2(i1,i2,i3,kd)+ty(i1,i2,i3)**2*gv2tt2(i1,i2,i3,
     & kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*gv2rs2(i1,i2,i3,kd)+2.*ry(i1,
     & i2,i3)*ty(i1,i2,i3)*gv2rt2(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,
     & i2,i3)*gv2st2(i1,i2,i3,kd)+ryy23(i1,i2,i3)*gv2r2(i1,i2,i3,kd)+
     & syy23(i1,i2,i3)*gv2s2(i1,i2,i3,kd)+tyy23(i1,i2,i3)*gv2t2(i1,i2,
     & i3,kd)
      gv2zz23(i1,i2,i3,kd)=rz(i1,i2,i3)**2*gv2rr2(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*gv2ss2(i1,i2,i3,kd)+tz(i1,i2,i3)**2*gv2tt2(i1,i2,i3,
     & kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*gv2rs2(i1,i2,i3,kd)+2.*rz(i1,
     & i2,i3)*tz(i1,i2,i3)*gv2rt2(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,
     & i2,i3)*gv2st2(i1,i2,i3,kd)+rzz23(i1,i2,i3)*gv2r2(i1,i2,i3,kd)+
     & szz23(i1,i2,i3)*gv2s2(i1,i2,i3,kd)+tzz23(i1,i2,i3)*gv2t2(i1,i2,
     & i3,kd)
      gv2xy23(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*gv2rr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*gv2ss2(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & ty(i1,i2,i3)*gv2tt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(
     & i1,i2,i3)*sx(i1,i2,i3))*gv2rs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(
     & i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*gv2rt2(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*gv2st2(i1,i2,
     & i3,kd)+rxy23(i1,i2,i3)*gv2r2(i1,i2,i3,kd)+sxy23(i1,i2,i3)*
     & gv2s2(i1,i2,i3,kd)+txy23(i1,i2,i3)*gv2t2(i1,i2,i3,kd)
      gv2xz23(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*gv2rr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*gv2ss2(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & tz(i1,i2,i3)*gv2tt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sx(i1,i2,i3))*gv2rs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*gv2rt2(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*gv2st2(i1,i2,
     & i3,kd)+rxz23(i1,i2,i3)*gv2r2(i1,i2,i3,kd)+sxz23(i1,i2,i3)*
     & gv2s2(i1,i2,i3,kd)+txz23(i1,i2,i3)*gv2t2(i1,i2,i3,kd)
      gv2yz23(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*gv2rr2(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*gv2ss2(i1,i2,i3,kd)+ty(i1,i2,i3)*
     & tz(i1,i2,i3)*gv2tt2(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sy(i1,i2,i3))*gv2rs2(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*gv2rt2(i1,i2,i3,kd)+(sy(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*gv2st2(i1,i2,
     & i3,kd)+ryz23(i1,i2,i3)*gv2r2(i1,i2,i3,kd)+syz23(i1,i2,i3)*
     & gv2s2(i1,i2,i3,kd)+tyz23(i1,i2,i3)*gv2t2(i1,i2,i3,kd)
      gv2laplacian23(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*gv2rr2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)
     & **2+sz(i1,i2,i3)**2)*gv2ss2(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(
     & i1,i2,i3)**2+tz(i1,i2,i3)**2)*gv2tt2(i1,i2,i3,kd)+2.*(rx(i1,i2,
     & i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,
     & i2,i3))*gv2rs2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(
     & i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*gv2rt2(i1,i2,
     & i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)
     & +sz(i1,i2,i3)*tz(i1,i2,i3))*gv2st2(i1,i2,i3,kd)+(rxx23(i1,i2,
     & i3)+ryy23(i1,i2,i3)+rzz23(i1,i2,i3))*gv2r2(i1,i2,i3,kd)+(sxx23(
     & i1,i2,i3)+syy23(i1,i2,i3)+szz23(i1,i2,i3))*gv2s2(i1,i2,i3,kd)+(
     & txx23(i1,i2,i3)+tyy23(i1,i2,i3)+tzz23(i1,i2,i3))*gv2t2(i1,i2,
     & i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      gv2x23r(i1,i2,i3,kd)=(gv2(i1+1,i2,i3,kd)-gv2(i1-1,i2,i3,kd))*h12(
     & 0)
      gv2y23r(i1,i2,i3,kd)=(gv2(i1,i2+1,i3,kd)-gv2(i1,i2-1,i3,kd))*h12(
     & 1)
      gv2z23r(i1,i2,i3,kd)=(gv2(i1,i2,i3+1,kd)-gv2(i1,i2,i3-1,kd))*h12(
     & 2)
      gv2xx23r(i1,i2,i3,kd)=(-2.*gv2(i1,i2,i3,kd)+(gv2(i1+1,i2,i3,kd)+
     & gv2(i1-1,i2,i3,kd)) )*h22(0)
      gv2yy23r(i1,i2,i3,kd)=(-2.*gv2(i1,i2,i3,kd)+(gv2(i1,i2+1,i3,kd)+
     & gv2(i1,i2-1,i3,kd)) )*h22(1)
      gv2xy23r(i1,i2,i3,kd)=(gv2x23r(i1,i2+1,i3,kd)-gv2x23r(i1,i2-1,i3,
     & kd))*h12(1)
      gv2zz23r(i1,i2,i3,kd)=(-2.*gv2(i1,i2,i3,kd)+(gv2(i1,i2,i3+1,kd)+
     & gv2(i1,i2,i3-1,kd)) )*h22(2)
      gv2xz23r(i1,i2,i3,kd)=(gv2x23r(i1,i2,i3+1,kd)-gv2x23r(i1,i2,i3-1,
     & kd))*h12(2)
      gv2yz23r(i1,i2,i3,kd)=(gv2y23r(i1,i2,i3+1,kd)-gv2y23r(i1,i2,i3-1,
     & kd))*h12(2)
      gv2x21r(i1,i2,i3,kd)= gv2x23r(i1,i2,i3,kd)
      gv2y21r(i1,i2,i3,kd)= gv2y23r(i1,i2,i3,kd)
      gv2z21r(i1,i2,i3,kd)= gv2z23r(i1,i2,i3,kd)
      gv2xx21r(i1,i2,i3,kd)= gv2xx23r(i1,i2,i3,kd)
      gv2yy21r(i1,i2,i3,kd)= gv2yy23r(i1,i2,i3,kd)
      gv2zz21r(i1,i2,i3,kd)= gv2zz23r(i1,i2,i3,kd)
      gv2xy21r(i1,i2,i3,kd)= gv2xy23r(i1,i2,i3,kd)
      gv2xz21r(i1,i2,i3,kd)= gv2xz23r(i1,i2,i3,kd)
      gv2yz21r(i1,i2,i3,kd)= gv2yz23r(i1,i2,i3,kd)
      gv2laplacian21r(i1,i2,i3,kd)=gv2xx23r(i1,i2,i3,kd)
      gv2x22r(i1,i2,i3,kd)= gv2x23r(i1,i2,i3,kd)
      gv2y22r(i1,i2,i3,kd)= gv2y23r(i1,i2,i3,kd)
      gv2z22r(i1,i2,i3,kd)= gv2z23r(i1,i2,i3,kd)
      gv2xx22r(i1,i2,i3,kd)= gv2xx23r(i1,i2,i3,kd)
      gv2yy22r(i1,i2,i3,kd)= gv2yy23r(i1,i2,i3,kd)
      gv2zz22r(i1,i2,i3,kd)= gv2zz23r(i1,i2,i3,kd)
      gv2xy22r(i1,i2,i3,kd)= gv2xy23r(i1,i2,i3,kd)
      gv2xz22r(i1,i2,i3,kd)= gv2xz23r(i1,i2,i3,kd)
      gv2yz22r(i1,i2,i3,kd)= gv2yz23r(i1,i2,i3,kd)
      gv2laplacian22r(i1,i2,i3,kd)=gv2xx23r(i1,i2,i3,kd)+gv2yy23r(i1,
     & i2,i3,kd)
      gv2laplacian23r(i1,i2,i3,kd)=gv2xx23r(i1,i2,i3,kd)+gv2yy23r(i1,
     & i2,i3,kd)+gv2zz23r(i1,i2,i3,kd)
      gv2xxx22r(i1,i2,i3,kd)=(-2.*(gv2(i1+1,i2,i3,kd)-gv2(i1-1,i2,i3,
     & kd))+(gv2(i1+2,i2,i3,kd)-gv2(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
      gv2yyy22r(i1,i2,i3,kd)=(-2.*(gv2(i1,i2+1,i3,kd)-gv2(i1,i2-1,i3,
     & kd))+(gv2(i1,i2+2,i3,kd)-gv2(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
      gv2xxy22r(i1,i2,i3,kd)=( gv2xx22r(i1,i2+1,i3,kd)-gv2xx22r(i1,i2-
     & 1,i3,kd))/(2.*dx(1))
      gv2xyy22r(i1,i2,i3,kd)=( gv2yy22r(i1+1,i2,i3,kd)-gv2yy22r(i1-1,
     & i2,i3,kd))/(2.*dx(0))
      gv2xxxx22r(i1,i2,i3,kd)=(6.*gv2(i1,i2,i3,kd)-4.*(gv2(i1+1,i2,i3,
     & kd)+gv2(i1-1,i2,i3,kd))+(gv2(i1+2,i2,i3,kd)+gv2(i1-2,i2,i3,kd))
     &  )/(dx(0)**4)
      gv2yyyy22r(i1,i2,i3,kd)=(6.*gv2(i1,i2,i3,kd)-4.*(gv2(i1,i2+1,i3,
     & kd)+gv2(i1,i2-1,i3,kd))+(gv2(i1,i2+2,i3,kd)+gv2(i1,i2-2,i3,kd))
     &  )/(dx(1)**4)
      gv2xxyy22r(i1,i2,i3,kd)=( 4.*gv2(i1,i2,i3,kd)     -2.*(gv2(i1+1,
     & i2,i3,kd)+gv2(i1-1,i2,i3,kd)+gv2(i1,i2+1,i3,kd)+gv2(i1,i2-1,i3,
     & kd))   +   (gv2(i1+1,i2+1,i3,kd)+gv2(i1-1,i2+1,i3,kd)+gv2(i1+1,
     & i2-1,i3,kd)+gv2(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      ! 2D laplacian squared = gv2.xxxx + 2 gv2.xxyy + gv2.yyyy
      gv2LapSq22r(i1,i2,i3,kd)= ( 6.*gv2(i1,i2,i3,kd)   - 4.*(gv2(i1+1,
     & i2,i3,kd)+gv2(i1-1,i2,i3,kd))    +(gv2(i1+2,i2,i3,kd)+gv2(i1-2,
     & i2,i3,kd)) )/(dx(0)**4) +( 6.*gv2(i1,i2,i3,kd)    -4.*(gv2(i1,
     & i2+1,i3,kd)+gv2(i1,i2-1,i3,kd))    +(gv2(i1,i2+2,i3,kd)+gv2(i1,
     & i2-2,i3,kd)) )/(dx(1)**4)  +( 8.*gv2(i1,i2,i3,kd)     -4.*(gv2(
     & i1+1,i2,i3,kd)+gv2(i1-1,i2,i3,kd)+gv2(i1,i2+1,i3,kd)+gv2(i1,i2-
     & 1,i3,kd))   +2.*(gv2(i1+1,i2+1,i3,kd)+gv2(i1-1,i2+1,i3,kd)+gv2(
     & i1+1,i2-1,i3,kd)+gv2(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      gv2xxx23r(i1,i2,i3,kd)=(-2.*(gv2(i1+1,i2,i3,kd)-gv2(i1-1,i2,i3,
     & kd))+(gv2(i1+2,i2,i3,kd)-gv2(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
      gv2yyy23r(i1,i2,i3,kd)=(-2.*(gv2(i1,i2+1,i3,kd)-gv2(i1,i2-1,i3,
     & kd))+(gv2(i1,i2+2,i3,kd)-gv2(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
      gv2zzz23r(i1,i2,i3,kd)=(-2.*(gv2(i1,i2,i3+1,kd)-gv2(i1,i2,i3-1,
     & kd))+(gv2(i1,i2,i3+2,kd)-gv2(i1,i2,i3-2,kd)) )*h22(1)*h12(2)
      gv2xxy23r(i1,i2,i3,kd)=( gv2xx22r(i1,i2+1,i3,kd)-gv2xx22r(i1,i2-
     & 1,i3,kd))/(2.*dx(1))
      gv2xyy23r(i1,i2,i3,kd)=( gv2yy22r(i1+1,i2,i3,kd)-gv2yy22r(i1-1,
     & i2,i3,kd))/(2.*dx(0))
      gv2xxz23r(i1,i2,i3,kd)=( gv2xx22r(i1,i2,i3+1,kd)-gv2xx22r(i1,i2,
     & i3-1,kd))/(2.*dx(2))
      gv2yyz23r(i1,i2,i3,kd)=( gv2yy22r(i1,i2,i3+1,kd)-gv2yy22r(i1,i2,
     & i3-1,kd))/(2.*dx(2))
      gv2xzz23r(i1,i2,i3,kd)=( gv2zz22r(i1+1,i2,i3,kd)-gv2zz22r(i1-1,
     & i2,i3,kd))/(2.*dx(0))
      gv2yzz23r(i1,i2,i3,kd)=( gv2zz22r(i1,i2+1,i3,kd)-gv2zz22r(i1,i2-
     & 1,i3,kd))/(2.*dx(1))
      gv2xxxx23r(i1,i2,i3,kd)=(6.*gv2(i1,i2,i3,kd)-4.*(gv2(i1+1,i2,i3,
     & kd)+gv2(i1-1,i2,i3,kd))+(gv2(i1+2,i2,i3,kd)+gv2(i1-2,i2,i3,kd))
     &  )/(dx(0)**4)
      gv2yyyy23r(i1,i2,i3,kd)=(6.*gv2(i1,i2,i3,kd)-4.*(gv2(i1,i2+1,i3,
     & kd)+gv2(i1,i2-1,i3,kd))+(gv2(i1,i2+2,i3,kd)+gv2(i1,i2-2,i3,kd))
     &  )/(dx(1)**4)
      gv2zzzz23r(i1,i2,i3,kd)=(6.*gv2(i1,i2,i3,kd)-4.*(gv2(i1,i2,i3+1,
     & kd)+gv2(i1,i2,i3-1,kd))+(gv2(i1,i2,i3+2,kd)+gv2(i1,i2,i3-2,kd))
     &  )/(dx(2)**4)
      gv2xxyy23r(i1,i2,i3,kd)=( 4.*gv2(i1,i2,i3,kd)     -2.*(gv2(i1+1,
     & i2,i3,kd)+gv2(i1-1,i2,i3,kd)+gv2(i1,i2+1,i3,kd)+gv2(i1,i2-1,i3,
     & kd))   +   (gv2(i1+1,i2+1,i3,kd)+gv2(i1-1,i2+1,i3,kd)+gv2(i1+1,
     & i2-1,i3,kd)+gv2(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      gv2xxzz23r(i1,i2,i3,kd)=( 4.*gv2(i1,i2,i3,kd)     -2.*(gv2(i1+1,
     & i2,i3,kd)+gv2(i1-1,i2,i3,kd)+gv2(i1,i2,i3+1,kd)+gv2(i1,i2,i3-1,
     & kd))   +   (gv2(i1+1,i2,i3+1,kd)+gv2(i1-1,i2,i3+1,kd)+gv2(i1+1,
     & i2,i3-1,kd)+gv2(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)
      gv2yyzz23r(i1,i2,i3,kd)=( 4.*gv2(i1,i2,i3,kd)     -2.*(gv2(i1,i2+
     & 1,i3,kd)  +gv2(i1,i2-1,i3,kd)+  gv2(i1,i2  ,i3+1,kd)+gv2(i1,i2 
     &  ,i3-1,kd))   +   (gv2(i1,i2+1,i3+1,kd)+gv2(i1,i2-1,i3+1,kd)+
     & gv2(i1,i2+1,i3-1,kd)+gv2(i1,i2-1,i3-1,kd)) )/(dx(1)**2*dx(2)**
     & 2)
      ! 3D laplacian squared = gv2.xxxx + gv2.yyyy + gv2.zzzz + 2 (gv2.xxyy + gv2.xxzz + gv2.yyzz )
      gv2LapSq23r(i1,i2,i3,kd)= ( 6.*gv2(i1,i2,i3,kd)   - 4.*(gv2(i1+1,
     & i2,i3,kd)+gv2(i1-1,i2,i3,kd))    +(gv2(i1+2,i2,i3,kd)+gv2(i1-2,
     & i2,i3,kd)) )/(dx(0)**4) +( 6.*gv2(i1,i2,i3,kd)    -4.*(gv2(i1,
     & i2+1,i3,kd)+gv2(i1,i2-1,i3,kd))    +(gv2(i1,i2+2,i3,kd)+gv2(i1,
     & i2-2,i3,kd)) )/(dx(1)**4)  +( 6.*gv2(i1,i2,i3,kd)    -4.*(gv2(
     & i1,i2,i3+1,kd)+gv2(i1,i2,i3-1,kd))    +(gv2(i1,i2,i3+2,kd)+gv2(
     & i1,i2,i3-2,kd)) )/(dx(2)**4)  +( 8.*gv2(i1,i2,i3,kd)     -4.*(
     & gv2(i1+1,i2,i3,kd)  +gv2(i1-1,i2,i3,kd)  +gv2(i1  ,i2+1,i3,kd)+
     & gv2(i1  ,i2-1,i3,kd))   +2.*(gv2(i1+1,i2+1,i3,kd)+gv2(i1-1,i2+
     & 1,i3,kd)+gv2(i1+1,i2-1,i3,kd)+gv2(i1-1,i2-1,i3,kd)) )/(dx(0)**
     & 2*dx(1)**2)+( 8.*gv2(i1,i2,i3,kd)     -4.*(gv2(i1+1,i2,i3,kd)  
     & +gv2(i1-1,i2,i3,kd)  +gv2(i1  ,i2,i3+1,kd)+gv2(i1  ,i2,i3-1,kd)
     & )   +2.*(gv2(i1+1,i2,i3+1,kd)+gv2(i1-1,i2,i3+1,kd)+gv2(i1+1,i2,
     & i3-1,kd)+gv2(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)+( 8.*gv2(
     & i1,i2,i3,kd)     -4.*(gv2(i1,i2+1,i3,kd)  +gv2(i1,i2-1,i3,kd)  
     & +gv2(i1,i2  ,i3+1,kd)+gv2(i1,i2  ,i3-1,kd))   +2.*(gv2(i1,i2+1,
     & i3+1,kd)+gv2(i1,i2-1,i3+1,kd)+gv2(i1,i2+1,i3-1,kd)+gv2(i1,i2-1,
     & i3-1,kd)) )/(dx(1)**2*dx(2)**2)

      u2xr22(i1,i2,i3,kd)= rx(i1,i2,i3)*u2rr2(i1,i2,i3,kd)+sx(i1,i2,i3)
     & *
     & u2rs2(i1,i2,i3,kd)+rxr2(i1,i2,i3)*u2r2(i1,i2,i3,kd)+sxr2(i1,i2,
     & i3)*
     & u2s2(i1,i2,i3,kd)
      u2xs22(i1,i2,i3,kd)= rx(i1,i2,i3)*u2rs2(i1,i2,i3,kd)+sx(i1,i2,i3)
     & *
     & u2ss2(i1,i2,i3,kd)+rxs2(i1,i2,i3)*u2r2(i1,i2,i3,kd)+sxs2(i1,i2,
     & i3)*
     & u2s2(i1,i2,i3,kd)

      u2yr22(i1,i2,i3,kd)= ry(i1,i2,i3)*u2rr2(i1,i2,i3,kd)+sy(i1,i2,i3)
     & *
     & u2rs2(i1,i2,i3,kd)+ryr2(i1,i2,i3)*u2r2(i1,i2,i3,kd)+syr2(i1,i2,
     & i3)*
     & u2s2(i1,i2,i3,kd)
      u2ys22(i1,i2,i3,kd)= ry(i1,i2,i3)*u2rs2(i1,i2,i3,kd)+sy(i1,i2,i3)
     & *
     & u2ss2(i1,i2,i3,kd)+rys2(i1,i2,i3)*u2r2(i1,i2,i3,kd)+sys2(i1,i2,
     & i3)*
     & u2s2(i1,i2,i3,kd)

c     --- end statement functions

c .............. end statement functions


      ierr=0
      ! write(*,*) 'Inside cnsFarFieldBC'

      rc                =ipar(0)
      uc                =ipar(1)
      vc                =ipar(2)
      wc                =ipar(3)
      tc                =ipar(4)
      sc                =ipar(5)
      numberOfSpecies   =ipar(6)
      grid              =ipar(7)
      gridType          =ipar(8)
      orderOfAccuracy   =ipar(9)
      gridIsMoving      =ipar(10)
      useWhereMask      =ipar(11)
      isAxisymmetric    =ipar(12)
      twilightZone      =ipar(13)
      bcOption          =ipar(14)
      debug             =ipar(15)
      knownSolution     =ipar(16)

      dx(0)             =rpar(0)
      dx(1)             =rpar(1)
      dx(2)             =rpar(2)
      dr(0)             =rpar(3)
      dr(1)             =rpar(4)
      dr(2)             =rpar(5)
      t                 =rpar(6)
      dt                =rpar(7)
      epsx              =rpar(8)
      gamma             =rpar(9)
      ep                =rpar(10) !  holds the pointer to the TZ function

      ! coefficient of the artificial diffusion:
      do n=0,4
        ad(n)=rpar(20+n)
      end do

      Rg=1.

      numberOfComponents=4
      ncm1= numberOfComponents-1

      ! write(*,'(" **** farFieldBC: bcOption=",i4)') bcOption

      gm1=gamma-1.
      ad2=10.  ! artificial dissipation


      do axis=0,2
      do side=0,1
         nr(side,axis)=indexRange(side,axis)
      end do
      end do


      if( .false. .and. nd.eq.2 .and. gridType.eq.rectangular .and. 
     & twilightZone.eq.0 )then

        ! *********************************************************************
        ! ******* 2D non-moving, rectangular **********************************
        ! *********************************************************************

        if( gridIsMoving.ne.0 )then
          write(*,'("cnsFarFieldBC:ERROR: gridIsMoving not implemented 
     & yet for rectangular")')
          ! '
          stop 6642
        end if

        do axis=0,nd-1
        do side=0,1
        if( bc(side,axis).eq.farField )then

          nr(0,axis)=indexRange(side,axis)   ! set nr to point to this side
          nr(1,axis)=indexRange(side,axis)

          unc = uc+axis                ! normal component is uc or vc
          utc = uc+mod(axis+1,2)       ! tangential component

          if( axis.eq.0 )then
            is1 = 1-2*side
            is2=0
          else
            is1=0
            is2=1-2*side
          end if
          ks1=2*is1
          ks2=2*is2
           n1a=nr(0,0)
           n1b=nr(1,0)
           n2a=nr(0,1)
           n2b=nr(1,1)
           n3a=nr(0,2)
           n3b=nr(1,2)
           do i3=n3a,n3b
           do i2=n2a,n2b
           do i1=n1a,n1b
            ! do as a separate loop:  u(i1,i2,i3,unc)=0.
            u(i1-is1,i2-is2,i3,unc)=2.*u(i1,i2,i3,unc)-u(i1+is1,i2+is2,
     & i3,unc)
            u(i1-is1,i2-is2,i3,utc)=u(i1+is1,i2+is2,i3,utc)

            u(i1-is1,i2-is2,i3,rc )=u(i1+is1,i2+is2,i3, rc)
            u(i1-is1,i2-is2,i3,tc )=u(i1+is1,i2+is2,i3, tc)

            ! --- 2nd ghost line: ----
            u(i1-ks1,i2-ks2,i3,unc)=2.*u(i1,i2,i3,unc)-u(i1+ks1,i2+ks2,
     & i3,unc)
            u(i1-ks1,i2-ks2,i3,utc)=u(i1+ks1,i2+ks2,i3,utc)

            u(i1-ks1,i2-ks2,i3,rc )=u(i1+ks1,i2+ks2,i3, rc)
            u(i1-ks1,i2-ks2,i3,tc )=u(i1+ks1,i2+ks2,i3, tc)
           end do
           end do
           end do

          nr(0,axis)=indexRange(0,axis)  ! reset
          nr(1,axis)=indexRange(1,axis)

        end if ! if( bc.eq.farField )
        end do
        end do



      ! ******************* Farfield BC for curvilinear grids **********************************
      else if( .true. .or. (nd.eq.2 .and. gridType.eq.curvilinear) )
     & then

        do axis=0,nd-1
        do side=0,1
        if( bc(side,axis).eq.farField )then

          nr(0,axis)=indexRange(side,axis)   ! set nr to point to this side
          nr(1,axis)=indexRange(side,axis)

          axisp1 = mod(axis+1,nd)
          if( axis.eq.0 )then
            is1 = 1-2*side
            is2=0
            js1=0
            js2=1
          else
            is1=0
            is2=1-2*side
            js1=1
            js2=0
          end if
          sgn=1-2*side
          ks1=2*is1
          ks2=2*is2

          if( gridIsMoving.eq.1 )then
            write(*,'(" cnsFarField: --> moving grids")')
            ! '
          endif

          if( debug.gt.1 ) then
            write(*,'(" cnsFarField: side,axis=",2i2)') side,axis
          end if
          if( dt.lt.0. )then
            write(*,'(" ***cnsFarField:WARNING: dt<0 for t=",e12.3)') t
            dt=0.
          else
            if( debug.gt.1 ) then
              write(*,'(" ***cnsFarField:INFO: t,dt=",2(e12.3,1x))') t,
     & dt
            end if
          end if

          tm=t-dt
          z0=0.

          ii=nr(0,axisp1)
           n1a=nr(0,0)
           n1b=nr(1,0)
           n2a=nr(0,1)
           n2b=nr(1,1)
           n3a=nr(0,2)
           n3b=nr(1,2)
           do i3=n3a,n3b
           do i2=n2a,n2b
           do i1=n1a,n1b
          if( mask(i1,i2,i3).gt.0 )then

            ! *NOTE* rxi is either r.x (axis==0) or s.x (axis==1) 
            rxi = rsxy(i1,i2,i3,axis,0)
            ryi = rsxy(i1,i2,i3,axis,1)
            sxi = rsxy(i1,i2,i3,axisp1,0)
            syi = rsxy(i1,i2,i3,axisp1,1)

            an1=-rxi*sgn
            an2=-ryi*sgn
            aNorm=sqrt(max(epsx,an1**2+an2**2))
            an1=an1/aNorm
            an2=an2/aNorm

            ajac=rxi*syi-ryi*sxi
            xri= syi/ajac
            yri=-sxi/ajac
            xsi=-ryi/ajac
            ysi= rxi/ajac


            do mm=1,2  ! ghost lines 1 and 2

              j1=i1-is1*mm
              j2=i2-is2*mm
              j3=i3-is3*mm

              k1=i1+is1*mm
              k2=i2+is2*mm
              k3=i3+is3*mm

              ! For now set: D+D-( u ) = D+D-( uKnown )
             if( .false. )then ! *wdh* 051206
              do m=0,ncm1
                ! u(i1,i2,i3,m)=u(k1,k2,k3,m)
                ! u(j1,j2,j3,m)=2.*u(i1,i2,i3,m)-u(k1,k2,k3,m)
                u(j1,j2,j3,m)=u(k1,k2,k3,m)
              end do

              if(  knownSolution.gt.0 )then
                do m=0,ncm1
                  ! u(i1,i2,i3,m)=u(i1,i2,i3,m) + uKnown(i1,i2,i3,m)-uKnown(k1,k2,k3,m)
                  ! u(j1,j2,j3,m)=u(j1,j2,j3,m) + (uKnown(j1,j2,j3,m)-2.*uKnown(i1,i2,i3,m)+uKnown(k1,k2,k3,m))
                  u(j1,j2,j3,m)=u(j1,j2,j3,m) + (uKnown(j1,j2,j3,m)-
     & uKnown(k1,k2,k3,m))
                end do
              end if

             else if( .true. )then ! *wdh* 051207

              ! -------------------------------------------------

               ! check for supersonic or sub-sonic outflow

              ! un = component of velocity normal to boundary
              ! c = speed of sound
              un=an1*u(i1,i2,i3,uc)+an2*u(i1,i2,i3,vc)
              c = sqrt(gamma*u(i1,i2,i3,tc))
              if( abs(un).ge.c )then
                ! supersonic outflow -- extrap all variables.
!      write(*,'("cnsFarField:side,axis,i1,i2,un,c --> supersonic outflow",4i3,2f7.3)')!              side,axis,i1,i2,un,c
               ! '
                do m=0,ncm1
!                  u(j1,j2,j3,m)=2.*u(i1,i2,i3,m)-u(k1,k2,k3,m)
                  u(j1,j2,j3,m)=3.*u(j1+is1,j2+is2,j3+is3,m)-3.*u(j1+2*
     & is1,j2+2*is2,j3+2*is3,m)+u(j1+3*is1,j2+3*is2,j3+3*is3,m)
                end do
              else
               do m=0,ncm1
                ! u(i1,i2,i3,m)=u(k1,k2,k3,m)
                ! u(j1,j2,j3,m)=2.*u(i1,i2,i3,m)-u(k1,k2,k3,m)
                ! u(j1,j2,j3,m)=u(k1,k2,k3,m)
                u(j1,j2,j3,m)=3.*u(j1+is1,j2+is2,j3+is3,m)-3.*u(j1+2*
     & is1,j2+2*is2,j3+2*is3,m)+u(j1+3*is1,j2+3*is2,j3+3*is3,m)
               end do
               m=tc
               u(j1,j2,j3,m)=u(k1,k2,k3,m)
               if(  knownSolution.gt.0 )then
                 do m=0,ncm1
                  ! u(i1,i2,i3,m)=u(i1,i2,i3,m) + uKnown(i1,i2,i3,m)-uKnown(k1,k2,k3,m)
                  ! u(j1,j2,j3,m)=u(j1,j2,j3,m) + (uKnown(j1,j2,j3,m)-2.*uKnown(i1,i2,i3,m)+uKnown(k1,k2,k3,m))
                  ! u(j1,j2,j3,m)=u(j1,j2,j3,m) + (uKnown(j1,j2,j3,m)-uKnown(k1,k2,k3,m))
                  u(j1,j2,j3,m)=u(j1,j2,j3,m) + (uKnown(j1,j2,j3,m)-3.*
     & uKnown(j1+is1,j2+is2,j3+is3,m)+3.*uKnown(j1+2*is1,j2+2*is2,j3+
     & 2*is3,m)-uKnown(j1+3*is1,j2+3*is2,j3+3*is3,m))
                 end do
                 m=tc
                 u(j1,j2,j3,m)=u(j1,j2,j3,m) + (uKnown(j1,j2,j3,m)-
     & uKnown(k1,k2,k3,m))
               end if
              end if
              ! -------------------------------------------------

             else if( .false. )then

              ! try this: Extrap all except for T, Give T.n = 
              ! try this: Give D+D+()=D+D-(true) for all , then D0(T)=D0(true)
              do m=0,ncm1
               if( .true. .or. mm.eq.1 )then
                u(j1,j2,j3,m)=3.*u(j1+is1,j2+is2,j3+is3,m)-3.*u(j1+2*
     & is1,j2+2*is2,j3+2*is3,m)+u(j1+3*is1,j2+3*is2,j3+3*is3,m)
               else
                u(j1,j2,j3,m)=2.*u(j1+is1,j2+is2,j3+is3,m)-u(j1+2*is1,
     & j2+2*is2,j3+2*is3,m)
                ! u(j1,j2,j3,m)=2.*u(i1,i2,i3,m)-u(k1,k2,k3,m)
               end if
                ! u(j1,j2,j3,m)=2.*u(i1,i2,i3,m)-u(k1,k2,k3,m)
              end do
              !  u(j1,j2,j3,m)=2.*u(j1+is1,j2+is2,j3+is3,m)-u(j1+2*is1,j2+2*is2,j3+2*is3,m)
              m=tc
              u(j1,j2,j3,m)=u(k1,k2,k3,m)

              if(  knownSolution.gt.0 )then
                do m=0,ncm1
                  ! u(j1,j2,j3,m)=u(j1,j2,j3,m) + (uKnown(j1,j2,j3,m)-2.*uKnown(i1,i2,i3,m)+uKnown(k1,k2,k3,m))
                 if( .true. .or. mm.eq.1 )then
                  u(j1,j2,j3,m)=u(j1,j2,j3,m) + (uKnown(j1,j2,j3,m)-3.*
     & uKnown(j1+is1,j2+is2,j3+is3,m)+3.*uKnown(j1+2*is1,j2+2*is2,j3+
     & 2*is3,m)-uKnown(j1+3*is1,j2+3*is2,j3+3*is3,m))
                 else
                  u(j1,j2,j3,m)=u(j1,j2,j3,m) + (uKnown(j1,j2,j3,m)-2.*
     & uKnown(j1+is1,j2+is2,j3+is3,m)+uKnown(j1+2*is1,j2+2*is2,j3+2*
     & is3,m))
                  !u(j1,j2,j3,m)=u(j1,j2,j3,m) + (uKnown(j1,j2,j3,m)-2.*uKnown(i1,i2,i3,m)+uKnown(k1,k2,k3,m))
                 end if
                end do
                m=tc
                u(j1,j2,j3,m)=u(j1,j2,j3,m) + (uKnown(j1,j2,j3,m)-
     & uKnown(k1,k2,k3,m))
              end if

             end if

            end do


           else ! mask(i1,i2,i3) <=0
              ! ---------------------------------------------------------------------------------
              ! set points outside of interp or unused points 
              ! ---------------------------------------------------------------------------------
              ! -- note that we need to set ghost points
              ! where mask(i1,i2,i3)=0 if we are next to an interpolation point (pts 1,3 below)
              !                      0  I  X   X  X   <- inside
              !                      0  I  X   X  X   <- boundary
              !                      1  2  g   g  g   <- ghost line 1
              !                      3  4  g   g  g   <- ghost line 2
              rxi = rsxy(i1,i2,i3,axis,0)
              ryi = rsxy(i1,i2,i3,axis,1)
              sxi = rsxy(i1,i2,i3,axisp1,0)
              syi = rsxy(i1,i2,i3,axisp1,1)
              an1=-rxi*sgn
              an2=-ryi*sgn
              aNorm=sqrt(max(epsx,an1**2+an2**2))
              an1=an1/aNorm
              an2=an2/aNorm
              do mm=1,2   ! assign values on two ghost lines
                j1=i1-is1*mm
                j2=i2-is2*mm
                j3=i3-is3*mm
                k1=i1+is1*mm
                k2=i2+is2*mm
                k3=i3+is3*mm
                u(j1,j2,j3,rc)=u(k1,k2,k3,rc)   ! apply symmetry, is this ok ?
                u(j1,j2,j3,tc)=u(k1,k2,k3,tc)
                u(j1,j2,j3,uc) =3.*u(j1+is1,j2+is2,j3,uc)-3.*u(j1+2*
     & is1,j2+2*is2,j3,uc)+u(j1+3*is1,j2+3*is2,j3,uc)
                u(j1,j2,j3,vc) =3.*u(j1+is1,j2+is2,j3,vc)-3.*u(j1+2*
     & is1,j2+2*is2,j3,vc)+u(j1+3*is1,j2+3*is2,j3,vc)
                ! extrap normal component of u 
                !   -- this extrpolation will be consistent with an odd symmetry condition (u.rr=0)
                nDotU1 = an1*( 2.*u(i1,i2,i3,uc)-u(k1,k2,k3,uc) ) + 
     & an2*( 2.*u(i1,i2,i3,vc)-u(k1,k2,k3,vc) )
                ! set the normal component to be nDotU1
                nDotU = an1*u(j1,j2,j3,uc)+an2*u(j1,j2,j3,vc) - nDotU1
                u(j1,j2,j3,uc)=u(j1,j2,j3,uc)- nDotU*an1
                u(j1,j2,j3,vc)=u(j1,j2,j3,vc)- nDotU*an2
              end do

           end if
           ii=ii+1
            end do
            end do
            end do

           ! ** assign values on the extended ghost lines
           ! assignEndValues()

          nr(0,axis)=indexRange(0,axis)  ! reset
          nr(1,axis)=indexRange(1,axis)

        end if ! if( bc.eq.farField )
        end do
        end do

      else

        write(*,'("cnsFarFieldBC2:ERROR:Unknown bcOption=",i5)') 
     & bcOption
        stop 17942

      end if


      return
      end



