! This file automatically generated from cnsSlipWallBC2.bf with bpp.
c
c routines for applying a slip wall BC
c

c Use this next macro to declare the statement functions that are defined below
c To include derivatives of rx use OPTION=RX


c Define statement functions for difference approximations of order 2 
c To include derivatives of rx use OPTION=RX
c To include derivatives of rx use OPTION=RX




! ====================================================================
! Look up an integer parameter from the data base
! ====================================================================

! ====================================================================
! Look up a real parameter from the data base
! ====================================================================










! ================================================================================================================
! This version also sets pra(ii), psa(ii)   -- maybe not used anymore 
! ================================================================================================================




c ======================================================================================
c Macro to evaluate the coefficinets and RHS for the derivative slipw all BC
c  AXIS (input) : 0 or 1 
c ======================================================================================

      subroutine cnsSlipWallBC2(nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,
     & nd4b,ipar,rpar, u, u2,  gv, gv2, gtt, mask, x,rsxy, bc, 
     & indexRange, interfaceType, exact, uKnown, pdb, ierr )
c========================================================================
c
c     Apply a slip wall boundary condition 
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
      integer indexRange(0:1,0:2), bc(0:1,0:2), interfaceType(0:1,0:2)
      integer ipar(0:*),ierr

      double precision pdb  ! pointer to data base

c.......local

      integer kd,kd3,i1,i2,i3,n1a,n1b,n2a,n2b,n3a,n3b
      integer is,j1,j2,j3,side,axis,twilightZone,bcOption,knownSolution
      integer rc,tc,uc,vc,wc,sc,unc,utc,n
      integer grid,orderOfAccuracy,gridIsMoving,useWhereMask,
     & applyInterfaceBoundaryConditions
      integer gridType,isAxisymmetric,numberOfSpecies,radialAxis,
     & axisymmetricWithSwirl,urc,uac
      integer nr(0:1,0:2)

      integer ok,getInt,getReal
      real densityLowerBound, pressureLowerBound
      integer checkForWallHeating

      real sxi,syi,szi,txi,tyi,tzi,rxi,ryi,rzi
      real pn,rho,rhon,nDotGradR,nDotGradS,tp,tpn,rhor,rhos,tps,ps,tpm,
     & pm,pp,pr,tpr
      integer axisp1

      integer is1,is2,is3,js1,js2,js3,ks1,ks2,ks3,s,m,mm

      real t,dt
      real an1,an2,an3,nDotU,aNorm,epsx,gamma
      real dr(0:2),dx(0:2),ad(0:10)
      real us0,vs0,w0s,sgn

      real rra,ura,vra,wra, rsa,usa,vsa,wsa, urra,vrra,wrra, ussa,vssa,
     & wssa, rrsa, ursa,vrsa,wrsa
      real rxa,rya,sxa,sya, rxra,ryra,sxra,syra, rxsa,rysa,sxsa,sysa
      real ra,ua,va,wa,fra,rhot

      real gur,gus,gvr,gvs,rxt,ryt,sxt,syt,fact,an1t,an2t

      real hx,hy,gm1
      real r0,rx0,ry0,rxx0,rxy0,ryy0, rt0,rtx0,rty0,rtt0
      real u0,ux0,uy0,uxx0,uxy0,uyy0, ut0,utx0,uty0,utt0
      real v0,vx0,vy0,vxx0,vxy0,vyy0, vt0,vtx0,vty0,vtt0
      real p0,px0,py0,pxx0,pxy0,pyy0, pt0,ptx0,pty0,ptt0
      real q0,qx0,qy0,qxx0,qxy0,qyy0, qt0,qtx0,qty0,qtt0
      real fv(0:20),uv(0:20),z0,tm,ad2dt
      real ep ! holds the pointer to the TZ function
      integer debug
      logical testSym,getGhostByTaylor,addFouthOrderAD,
     & applyBoundaryContactFix

      real r1,u1,v1,q1,p1,s1, s0,st0,stt0, p2
      real ur1,vr1,qr1,nDotU1,nDotuv(2),adu(0:20),usp,usm
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

      integer ndMax
      parameter( ndMax=802 )

      real pra(-2:ndMax), psa(-2:ndMax)  ! fix these ******************************************

      real prr,prs,pss,pst, divur, divus, rxUr, sxUr, rxUs, sxUs
      real pzz,pmz,ppz,pzm,pzp,pm1,pm2,rm1,rm2,kappa
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
      real te,txe,tye,pxe,pye,ue,ve

      real re1,re2,re3,qe1,qe2,qe3,dRho,dTemp
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
      real du0,du1,du2,du3,alpha,cdl,uEps, du,uNorm,dus, pEps,omega,
     & pNorm

      integer it,nit,numberOfComponents,ncm1,kv(0:2)
      integer secondGhostLineOption

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

      ! From Parameters.h : 
      integer noInterface,heatFluxInterface,tractionInterface,
     & tractionAndHeatFluxInterface
      parameter( noInterface=0,heatFluxInterface=1,tractionInterface=2,
     & tractionAndHeatFluxInterface=3 )

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
      ! write(*,*) 'Inside cnsSlipWallBC'



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

      numberOfComponents=ipar(17)
      radialAxis        =ipar(18)  ! =axis1 if x=0 is axis of cylindrical symmetry, =axis2 if y=0 is..
      axisymmetricWithSwirl=ipar(19)
      ! we sometimes turn off application of the interface conditions when they are done by cgmp:
      applyInterfaceBoundaryConditions=ipar(20)

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

      urc = uc+radialAxis  ! radial velocity for isAxisymmetric
      uac = uc+radialAxis-1 ! axial velocity

      if( debug.gt.2 ) then
        write(*,'("cnsSlipWallBC2: t=",e10.2," grid=",i3)') t,grid
      end if

      ! coefficient of the artificial diffusion:
      do n=0,4
        ad(n)=rpar(20+n)
      end do

      Rg=1.

      ! numberOfComponents=4
      if( numberOfComponents.gt.20 )then
        write(*,'("cnsSlipWallBC2:ERROR numberOfComponents is greater 
     & than expected")')
        ! '
        stop 6734
      end if
      if( rc.ne.0 )then
        write(*,'("cnsSlipWallBC2:ERROR expecting rc=0")')
        ! '
        stop 6735
      end if
      ncm1= numberOfComponents-1

      ! write(*,'(" **** slipWallBC: bcOption=",i4)') bcOption

      gm1=gamma-1.
      ad2=10.  ! artificial dissipation

      uEps=1.e-4 !

      ! Look up parameters from the data base,  *new way*
       ok=getReal(pdb,'densityLowerBound',densityLowerBound)
       if( ok.eq.0 )then
         write(*,'("*** cnsSlipWallBC2:ERROR: unable to find 
     & densityLowerBound")')
         stop 1133
       end if
       ok=getReal(pdb,'pressureLowerBound',pressureLowerBound)
       if( ok.eq.0 )then
         write(*,'("*** cnsSlipWallBC2:ERROR: unable to find 
     & pressureLowerBound")')
         stop 1133
       end if

       ok=getInt(pdb,'checkForWallHeating',checkForWallHeating)
       if( ok.eq.0 )then
         write(*,'("*** cnsSlipWallBC2:ERROR: unable to find 
     & checkForWallHeating")')
         stop 1122
       end if

      cdl=2. ! 1.  ! coefficient of the limiter function (0=no limiter)
      if( twilightZone.ne.0 )then
        cdl=0. ! turn off limiting for TZ -- *wdh* 110709
      end if

      ! write(*,'("*** slipWallBC2: densityLowerBound,pressureLowerBound=",2e10.2)') densityLowerBound,pressureLowerBound

      ! bcOption=slipWallSymmetry
      ! bcOption=slipWallPressureEntropySymmetry

!       i1=2
!       i2=2
!       i3=0
!       write(*,*) 'insbc4: x,y,u,err = ',x(i1,i2,i3,0),x(i1,i2,i3,1),ogf(exact,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,uc,t),!                                     u(i1,i2,i3,uc)-ogf(exact,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,uc,t)

!      if( gridIsMoving.ne.0 )then
!        stop 5
!      end if


      do axis=0,2
      do side=0,1
         nr(side,axis)=indexRange(side,axis)
      end do
      end do


      if( .false. .and. bcOption.eq.slipWallPressureEntropySymmetry 
     & .and. knownSolution.gt.0 )then
      do i3=nd3a,nd3b
        do i2=nd2a,nd2b
          do i1=nd1a,nd1b
            u2(i1,i2,i3,rc)=uKnown(i1,i2,i3,rc)
            u2(i1,i2,i3,uc)=uKnown(i1,i2,i3,uc)
            u2(i1,i2,i3,vc)=uKnown(i1,i2,i3,vc)
            u2(i1,i2,i3,tc)=uKnown(i1,i2,i3,tc)
          end do
        end do
      end do
      end if

! ===============================================================================================
!  Macro to extrapolate to order 3
! ===============================================================================================







      if( nd.eq.2 .and. gridType.eq.rectangular .and. 
     & twilightZone.eq.0  )then

        ! *********************************************************************
        ! ************************** 2D rectangular ***************************
        ! *********************************************************************

        if( t.le.dt .and. debug.gt.1 )then
         if( gridIsMoving.eq.0 )then
           write(*,'(" cnsSlipWall: rectangular non-moving grids: 
     & grid=",i3)') grid
           ! '
         else
           ! *** NOTE: normally a moving grid will not be rectangular anymore (since it may rotate for e.g.)
           write(*,'(" cnsSlipWall: rectangular MOVING grids: grid=",
     & i3)') grid
           ! '
         endif
        end if

        do axis=0,nd-1
        do side=0,1


        if( bc(side,axis).eq.slipWall  )then

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
          sgn=1-2*side
          is3=0
          js3=0

           n1a=nr(0,0)
           n1b=nr(1,0)
           n2a=nr(0,1)
           n2b=nr(1,1)
           n3a=nr(0,2)
           n3b=nr(1,2)
           do i3=n3a,n3b
           do i2=n2a,n2b
           do i1=n1a,n1b

           ! done  as a separate loop:  u(i1,i2,i3,unc)=0. or = gv

           j1=i1-is1
           j2=i2-is2
           j3=i3-is3
           k1=i1+is1 ! (k1,k2,k3) is the symmetry value used by limted rho extrap
           k2=i2+is2
           k3=i3+is3
             ! here du2=2nd-order approximation, du3=third order
             ! Blend the 2nd and 3rd order based on the difference 
             !   (which equals the second difference: uNew(-1)-2*u(0)+u(1))
             du1 = u(j1+is1,j2+is2,j3,rc)
             du2 = 2.*u(j1+is1,j2+is2,j3,rc)-u(j1+2*is1,j2+2*is2,j3,rc)
             du3 = 3.*u(j1+is1,j2+is2,j3,rc)-3.*u(j1+2*is1,j2+2*is2,j3,
     & rc)+u(j1+3*is1,j2+3*is2,j3,rc)
             uNorm= uEps+ abs(du3) + abs(u(j1+is1,j2+is2,j3,rc))+abs(u(
     & j1+2*is1,j2+2*is2,j3,rc))
           ! **   du = abs(du3-u(j1+is1,j2+is2,j3,rc))/uNorm    ! changed 050711
           ! **   alpha = cdl*( du**2 + abs(du3-du2)/uNorm )
             alpha = cdl*( abs(du3-du2)/uNorm )
           !  alpha = cdl*( abs(du3-du2)/uNorm )
             alpha =min(1.,alpha)
             dus = u(k1,k2,k3,rc)   ! use symmetric BC as limited value
             if( du2.lt.densityLowerBound .or. 
     & du3.lt.densityLowerBound )then
               u(j1,j2,j3,rc) = max(densityLowerBound,dus)
             else
               u(j1,j2,j3,rc)=(1.-alpha)*du3+alpha*dus
             end if
             ! ** u(j1,j2,j3,rc)=(1.-alpha)*du2+alpha*dus
             ! here du2=2nd-order approximation, du3=third order
             ! Blend the 2nd and 3rd order based on the difference 
             !   (which equals the second difference: uNew(-1)-2*u(0)+u(1))
             du1 = u(j1+is1,j2+is2,j3,uc)
             du2 = 2.*u(j1+is1,j2+is2,j3,uc)-u(j1+2*is1,j2+2*is2,j3,uc)
             du3 = 3.*u(j1+is1,j2+is2,j3,uc)-3.*u(j1+2*is1,j2+2*is2,j3,
     & uc)+u(j1+3*is1,j2+3*is2,j3,uc)
             !   alpha = cdl*(abs(du3-u(j1+is1,j2+is2,j3,uc))+abs(du3-du2))/(uEps+abs(u(j1+is1,j2+is2,j3,uc))+abs(u(j1+2*is1,j2+2*is2,j3,uc)))
             ! alpha = cdl*(abs(du3-du2))/(.1+abs(u(j1+is1,j2+is2,j3,uc))+abs(u(j1+2*is1,j2+2*is2,j3,uc)))
             uNorm= uEps+ abs(du3) + abs(u(j1+is1,j2+is2,j3,uc))+abs(u(
     & j1+2*is1,j2+2*is2,j3,uc))
           ! **  du = abs(du3-u(j1+is1,j2+is2,j3,uc))/uNorm  ! changed 050711
           ! **  alpha = cdl*( du**2 + abs(du3-du2)/uNorm )
             alpha = cdl*( abs(du3-du2)/uNorm )
           !   alpha = cdl*( abs(du3-du2)/uNorm )
             alpha =min(1.,alpha)
             ! if( mm.eq.1 )then
             !   u(j1,j2,j3,uc)=(1.-alpha)*du3+alpha*du2
               u(j1,j2,j3,uc)=(1.-alpha)*du3+alpha*du1
             ! else
             !   u(j1,j2,j3,uc)=(1.-alpha)*du2+alpha*du1
             ! end if
             ! here du2=2nd-order approximation, du3=third order
             ! Blend the 2nd and 3rd order based on the difference 
             !   (which equals the second difference: uNew(-1)-2*u(0)+u(1))
             du1 = u(j1+is1,j2+is2,j3,vc)
             du2 = 2.*u(j1+is1,j2+is2,j3,vc)-u(j1+2*is1,j2+2*is2,j3,vc)
             du3 = 3.*u(j1+is1,j2+is2,j3,vc)-3.*u(j1+2*is1,j2+2*is2,j3,
     & vc)+u(j1+3*is1,j2+3*is2,j3,vc)
             !   alpha = cdl*(abs(du3-u(j1+is1,j2+is2,j3,vc))+abs(du3-du2))/(uEps+abs(u(j1+is1,j2+is2,j3,vc))+abs(u(j1+2*is1,j2+2*is2,j3,vc)))
             ! alpha = cdl*(abs(du3-du2))/(.1+abs(u(j1+is1,j2+is2,j3,vc))+abs(u(j1+2*is1,j2+2*is2,j3,vc)))
             uNorm= uEps+ abs(du3) + abs(u(j1+is1,j2+is2,j3,vc))+abs(u(
     & j1+2*is1,j2+2*is2,j3,vc))
           ! **  du = abs(du3-u(j1+is1,j2+is2,j3,vc))/uNorm  ! changed 050711
           ! **  alpha = cdl*( du**2 + abs(du3-du2)/uNorm )
             alpha = cdl*( abs(du3-du2)/uNorm )
           !   alpha = cdl*( abs(du3-du2)/uNorm )
             alpha =min(1.,alpha)
             ! if( mm.eq.1 )then
             !   u(j1,j2,j3,vc)=(1.-alpha)*du3+alpha*du2
               u(j1,j2,j3,vc)=(1.-alpha)*du3+alpha*du1
             ! else
             !   u(j1,j2,j3,vc)=(1.-alpha)*du2+alpha*du1
             ! end if

           ! assign T from p.x=-g.tt, (  u.t + p.x =0 ,  u.t = g.tt on the boundary)
           rho=u(i1,i2,i3,rc)
           tp=u(i1,i2,i3,tc)

           if( gridIsMoving.eq.0 )then
             pr=0.
           else
             pr = -sgn*rho*( gtt(i1,i2,i3,axis) )   ! +- p.x or +- p.y
           end if
           rhor=sgn*(u(i1+is1,i2+is2,i3,rc)-u(i1-is1,i2-is2,i3,rc))/(
     & 2.*dx(axis))

           tpr = (pr-rhor*tp)/rho        ! assumes p=r*T

           u(i1-  is1,i2-  is2,i3,tc)=u(i1+  is1,i2+  is2,i3,tc) -sgn*
     & 2.*dx(axis)*tpr
           u(i1-2*is1,i2-2*is2,i3,tc)=u(i1+2*is1,i2+2*is2,i3,tc) -sgn*
     & 4.*dx(axis)*tpr

             ! Adjust the value computed from the p.n condition
             ! here du2=2nd-order approximation, du3=third order
             ! Blend the 2nd and 3rd order based on the difference 
             !   (which equals the second difference: uNew(-1)-2*u(0)+u(1))
             du1 = u(j1+is1,j2+is2,j3,tc)
             du2 = 2.*u(j1+is1,j2+is2,j3,tc)-u(j1+2*is1,j2+2*is2,j3,tc)
             du3 = u(j1,j2,j3,tc) ! already computed
             uNorm= uEps+ .5*( abs(u(j1+is1,j2+is2,j3,tc))+abs(u(j1+2*
     & is1,j2+2*is2,j3,tc)) )
             ! Why do we do this: *wdh* turn off 110809
             ! du = abs(du3-u(j1+is1,j2+is2,j3,tc))/uNorm
             ! alpha = cdl*( du**2 + abs(du3-du2)/uNorm )
             alpha = cdl*( abs(du3-du2)/uNorm )
             !  alpha = cdl*( abs(du3-du2)/uNorm )
             alpha =min(1.,alpha)
             dus = u(k1,k2,k3,tc)   ! use symmetric BC as limited value
             ! if( mm.eq.1 )then
               u(j1,j2,j3,tc)=(1.-alpha)*du3+alpha*dus
             ! else
             !   u(j1,j2,j3,tc)=(1.-alpha)*du2+alpha*dus
             ! end if


           ! --- 2nd ghost line: ----
           j1=i1-2*is1
           j2=i2-2*is2
           j3=i3-2*is3
           k1=i1+2*is1 ! (k1,k2,k3) is the symmetry value used by limted rho extrap
           k2=i2+2*is2
           k3=i3+2*is3

             ! here du2=2nd-order approximation, du3=third order
             ! Blend the 2nd and 3rd order based on the difference 
             !   (which equals the second difference: uNew(-1)-2*u(0)+u(1))
             du1 = u(j1+is1,j2+is2,j3,rc)
             du2 = 2.*u(j1+is1,j2+is2,j3,rc)-u(j1+2*is1,j2+2*is2,j3,rc)
             du3 = 3.*u(j1+is1,j2+is2,j3,rc)-3.*u(j1+2*is1,j2+2*is2,j3,
     & rc)+u(j1+3*is1,j2+3*is2,j3,rc)
             uNorm= uEps+ abs(du3) + abs(u(j1+is1,j2+is2,j3,rc))+abs(u(
     & j1+2*is1,j2+2*is2,j3,rc))
           ! **   du = abs(du3-u(j1+is1,j2+is2,j3,rc))/uNorm    ! changed 050711
           ! **   alpha = cdl*( du**2 + abs(du3-du2)/uNorm )
             alpha = cdl*( abs(du3-du2)/uNorm )
           !  alpha = cdl*( abs(du3-du2)/uNorm )
             alpha =min(1.,alpha)
             dus = u(k1,k2,k3,rc)   ! use symmetric BC as limited value
             if( du2.lt.densityLowerBound .or. 
     & du3.lt.densityLowerBound )then
               u(j1,j2,j3,rc) = max(densityLowerBound,dus)
             else
               u(j1,j2,j3,rc)=(1.-alpha)*du3+alpha*dus
             end if
             ! ** u(j1,j2,j3,rc)=(1.-alpha)*du2+alpha*dus
             ! here du2=2nd-order approximation, du3=third order
             ! Blend the 2nd and 3rd order based on the difference 
             !   (which equals the second difference: uNew(-1)-2*u(0)+u(1))
             du1 = u(j1+is1,j2+is2,j3,uc)
             du2 = 2.*u(j1+is1,j2+is2,j3,uc)-u(j1+2*is1,j2+2*is2,j3,uc)
             du3 = 3.*u(j1+is1,j2+is2,j3,uc)-3.*u(j1+2*is1,j2+2*is2,j3,
     & uc)+u(j1+3*is1,j2+3*is2,j3,uc)
             !   alpha = cdl*(abs(du3-u(j1+is1,j2+is2,j3,uc))+abs(du3-du2))/(uEps+abs(u(j1+is1,j2+is2,j3,uc))+abs(u(j1+2*is1,j2+2*is2,j3,uc)))
             ! alpha = cdl*(abs(du3-du2))/(.1+abs(u(j1+is1,j2+is2,j3,uc))+abs(u(j1+2*is1,j2+2*is2,j3,uc)))
             uNorm= uEps+ abs(du3) + abs(u(j1+is1,j2+is2,j3,uc))+abs(u(
     & j1+2*is1,j2+2*is2,j3,uc))
           ! **  du = abs(du3-u(j1+is1,j2+is2,j3,uc))/uNorm  ! changed 050711
           ! **  alpha = cdl*( du**2 + abs(du3-du2)/uNorm )
             alpha = cdl*( abs(du3-du2)/uNorm )
           !   alpha = cdl*( abs(du3-du2)/uNorm )
             alpha =min(1.,alpha)
             ! if( mm.eq.1 )then
             !   u(j1,j2,j3,uc)=(1.-alpha)*du3+alpha*du2
               u(j1,j2,j3,uc)=(1.-alpha)*du3+alpha*du1
             ! else
             !   u(j1,j2,j3,uc)=(1.-alpha)*du2+alpha*du1
             ! end if
             ! here du2=2nd-order approximation, du3=third order
             ! Blend the 2nd and 3rd order based on the difference 
             !   (which equals the second difference: uNew(-1)-2*u(0)+u(1))
             du1 = u(j1+is1,j2+is2,j3,vc)
             du2 = 2.*u(j1+is1,j2+is2,j3,vc)-u(j1+2*is1,j2+2*is2,j3,vc)
             du3 = 3.*u(j1+is1,j2+is2,j3,vc)-3.*u(j1+2*is1,j2+2*is2,j3,
     & vc)+u(j1+3*is1,j2+3*is2,j3,vc)
             !   alpha = cdl*(abs(du3-u(j1+is1,j2+is2,j3,vc))+abs(du3-du2))/(uEps+abs(u(j1+is1,j2+is2,j3,vc))+abs(u(j1+2*is1,j2+2*is2,j3,vc)))
             ! alpha = cdl*(abs(du3-du2))/(.1+abs(u(j1+is1,j2+is2,j3,vc))+abs(u(j1+2*is1,j2+2*is2,j3,vc)))
             uNorm= uEps+ abs(du3) + abs(u(j1+is1,j2+is2,j3,vc))+abs(u(
     & j1+2*is1,j2+2*is2,j3,vc))
           ! **  du = abs(du3-u(j1+is1,j2+is2,j3,vc))/uNorm  ! changed 050711
           ! **  alpha = cdl*( du**2 + abs(du3-du2)/uNorm )
             alpha = cdl*( abs(du3-du2)/uNorm )
           !   alpha = cdl*( abs(du3-du2)/uNorm )
             alpha =min(1.,alpha)
             ! if( mm.eq.1 )then
             !   u(j1,j2,j3,vc)=(1.-alpha)*du3+alpha*du2
               u(j1,j2,j3,vc)=(1.-alpha)*du3+alpha*du1
             ! else
             !   u(j1,j2,j3,vc)=(1.-alpha)*du2+alpha*du1
             ! end if
             ! Adjust the value computed from the p.n condition
             ! here du2=2nd-order approximation, du3=third order
             ! Blend the 2nd and 3rd order based on the difference 
             !   (which equals the second difference: uNew(-1)-2*u(0)+u(1))
             du1 = u(j1+is1,j2+is2,j3,tc)
             du2 = 2.*u(j1+is1,j2+is2,j3,tc)-u(j1+2*is1,j2+2*is2,j3,tc)
             du3 = u(j1,j2,j3,tc) ! already computed
             uNorm= uEps+ .5*( abs(u(j1+is1,j2+is2,j3,tc))+abs(u(j1+2*
     & is1,j2+2*is2,j3,tc)) )
             ! Why do we do this: *wdh* turn off 110809
             ! du = abs(du3-u(j1+is1,j2+is2,j3,tc))/uNorm
             ! alpha = cdl*( du**2 + abs(du3-du2)/uNorm )
             alpha = cdl*( abs(du3-du2)/uNorm )
             !  alpha = cdl*( abs(du3-du2)/uNorm )
             alpha =min(1.,alpha)
             dus = u(k1,k2,k3,tc)   ! use symmetric BC as limited value
             ! if( mm.eq.1 )then
               u(j1,j2,j3,tc)=(1.-alpha)*du3+alpha*dus
             ! else
             !   u(j1,j2,j3,tc)=(1.-alpha)*du2+alpha*dus
             ! end if


           end do
           end do
           end do

          ! species
          if( numberOfSpecies.gt.0 )then
            n1a=nr(0,0)
            n1b=nr(1,0)
            n2a=nr(0,1)
            n2b=nr(1,1)
            n3a=nr(0,2)
            n3b=nr(1,2)
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
            do s=sc,sc+numberOfSpecies-1
             j1=i1-is1
             j2=i2-is2
             j3=i3-is3

               ! here du2=2nd-order approximation, du3=third order
               ! Blend the 2nd and 3rd order based on the difference 
               !   (which equals the second difference: uNew(-1)-2*u(0)+u(1))
               du1 = u(j1+is1,j2+is2,j3,sc)
               du2 = 2.*u(j1+is1,j2+is2,j3,sc)-u(j1+2*is1,j2+2*is2,j3,
     & sc)
               du3 = 3.*u(j1+is1,j2+is2,j3,sc)-3.*u(j1+2*is1,j2+2*is2,
     & j3,sc)+u(j1+3*is1,j2+3*is2,j3,sc)
               !   alpha = cdl*(abs(du3-u(j1+is1,j2+is2,j3,sc))+abs(du3-du2))/(uEps+abs(u(j1+is1,j2+is2,j3,sc))+abs(u(j1+2*is1,j2+2*is2,j3,sc)))
               ! alpha = cdl*(abs(du3-du2))/(.1+abs(u(j1+is1,j2+is2,j3,sc))+abs(u(j1+2*is1,j2+2*is2,j3,sc)))
               uNorm= uEps+ abs(du3) + abs(u(j1+is1,j2+is2,j3,sc))+abs(
     & u(j1+2*is1,j2+2*is2,j3,sc))
             ! **  du = abs(du3-u(j1+is1,j2+is2,j3,sc))/uNorm  ! changed 050711
             ! **  alpha = cdl*( du**2 + abs(du3-du2)/uNorm )
               alpha = cdl*( abs(du3-du2)/uNorm )
             !   alpha = cdl*( abs(du3-du2)/uNorm )
               alpha =min(1.,alpha)
               ! if( mm.eq.1 )then
               !   u(j1,j2,j3,sc)=(1.-alpha)*du3+alpha*du2
                 u(j1,j2,j3,sc)=(1.-alpha)*du3+alpha*du1
               ! else
               !   u(j1,j2,j3,sc)=(1.-alpha)*du2+alpha*du1
               ! end if

             j1=i1-2*is1
             j2=i2-2*is2
             j3=i3-2*is3
               ! here du2=2nd-order approximation, du3=third order
               ! Blend the 2nd and 3rd order based on the difference 
               !   (which equals the second difference: uNew(-1)-2*u(0)+u(1))
               du1 = u(j1+is1,j2+is2,j3,sc)
               du2 = 2.*u(j1+is1,j2+is2,j3,sc)-u(j1+2*is1,j2+2*is2,j3,
     & sc)
               du3 = 3.*u(j1+is1,j2+is2,j3,sc)-3.*u(j1+2*is1,j2+2*is2,
     & j3,sc)+u(j1+3*is1,j2+3*is2,j3,sc)
               !   alpha = cdl*(abs(du3-u(j1+is1,j2+is2,j3,sc))+abs(du3-du2))/(uEps+abs(u(j1+is1,j2+is2,j3,sc))+abs(u(j1+2*is1,j2+2*is2,j3,sc)))
               ! alpha = cdl*(abs(du3-du2))/(.1+abs(u(j1+is1,j2+is2,j3,sc))+abs(u(j1+2*is1,j2+2*is2,j3,sc)))
               uNorm= uEps+ abs(du3) + abs(u(j1+is1,j2+is2,j3,sc))+abs(
     & u(j1+2*is1,j2+2*is2,j3,sc))
             ! **  du = abs(du3-u(j1+is1,j2+is2,j3,sc))/uNorm  ! changed 050711
             ! **  alpha = cdl*( du**2 + abs(du3-du2)/uNorm )
               alpha = cdl*( abs(du3-du2)/uNorm )
             !   alpha = cdl*( abs(du3-du2)/uNorm )
               alpha =min(1.,alpha)
               ! if( mm.eq.1 )then
               !   u(j1,j2,j3,sc)=(1.-alpha)*du3+alpha*du2
                 u(j1,j2,j3,sc)=(1.-alpha)*du3+alpha*du1
               ! else
               !   u(j1,j2,j3,sc)=(1.-alpha)*du2+alpha*du1
               ! end if

            end do
            end do
            end do
            end do
          end if

          nr(0,axis)=indexRange(0,axis)  ! reset
          nr(1,axis)=indexRange(1,axis)


        else if( .false. .and. bc(side,axis).eq.symmetry  )then  ! don't use for now

          ! ************** symmetry ****************

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

            u(i1,i2,i3,unc)=0.
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

          ! species
          if( numberOfSpecies.gt.0 )then
             n1a=nr(0,0)
             n1b=nr(1,0)
             n2a=nr(0,1)
             n2b=nr(1,1)
             n3a=nr(0,2)
             n3b=nr(1,2)
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
             do s=sc,sc+numberOfSpecies-1
              u(i1-is1,i2-is2,i3,s)= u(i1+is1,i2+is2,i3,s)
              u(i1-ks1,i2-ks2,i3,s)= u(i1+ks1,i2+ks2,i3,s)
             end do
             end do
             end do
             end do
          end if

          nr(0,axis)=indexRange(0,axis)  ! reset
          nr(1,axis)=indexRange(1,axis)

        else if( bc(side,axis).eq.axisymmetric )then

          ! axisymmetric 

          nr(0,axis)=indexRange(side,axis)   ! set nr to point to this side
          nr(1,axis)=indexRange(side,axis)

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
            u(i1,i2,i3,vc)=0.
            u(i1-is1,i2-is2,i3,urc)=-u(i1+is1,i2+is2,i3,urc)
            u(i1-ks1,i2-ks2,i3,urc)=-u(i1+ks1,i2+ks2,i3,urc)

            u(i1-is1,i2-is2,i3,rc)= u(i1+is1,i2+is2,i3,rc)
            u(i1-ks1,i2-ks2,i3,rc)= u(i1+ks1,i2+ks2,i3,rc)

            u(i1-is1,i2-is2,i3,uac)= u(i1+is1,i2+is2,i3,uac)
            u(i1-ks1,i2-ks2,i3,uac)= u(i1+ks1,i2+ks2,i3,uac)

            u(i1-is1,i2-is2,i3,tc)= u(i1+is1,i2+is2,i3,tc)
            u(i1-ks1,i2-ks2,i3,tc)= u(i1+ks1,i2+ks2,i3,tc)

           end do
           end do
           end do

          if( axisymmetricWithSwirl.eq.1 )then
             n1a=nr(0,0)
             n1b=nr(1,0)
             n2a=nr(0,1)
             n2b=nr(1,1)
             n3a=nr(0,2)
             n3b=nr(1,2)
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
              u(i1-is1,i2-is2,i3,wc)= u(i1+is1,i2+is2,i3,wc)
              u(i1-ks1,i2-ks2,i3,wc)= u(i1+ks1,i2+ks2,i3,wc)
             end do
             end do
             end do
          end if

          ! species
          if( numberOfSpecies.gt.0 )then
             n1a=nr(0,0)
             n1b=nr(1,0)
             n2a=nr(0,1)
             n2b=nr(1,1)
             n3a=nr(0,2)
             n3b=nr(1,2)
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
             do s=sc,sc+numberOfSpecies-1
              u(i1-is1,i2-is2,i3,s)= u(i1+is1,i2+is2,i3,s)
              u(i1-ks1,i2-ks2,i3,s)= u(i1+ks1,i2+ks2,i3,s)
             end do
             end do
             end do
             end do
          end if

          nr(0,axis)=indexRange(0,axis)  ! reset
          nr(1,axis)=indexRange(1,axis)

        end if
        end do
        end do





      ! **************** NEWEST Extrapolation Derivative Method **********************

      else if( .true. .and. nd.eq.2 .and. gridType.eq.curvilinear 
     & .and. bcOption.eq.slipWallDerivative )then


        if( t.le.dt .and. gridIsMoving.eq.1 )then
          write(*,'(" cnsSlipWall: *NEW* slipWallDerivative used with 
     & curvilinear moving grids")')
          ! '
        endif

        is3=0
        js3=0
        do axis=0,nd-1
        do side=0,1

        if( bc(side,axis).eq.slipWall )then

         if( t.gt.0 .and. applyInterfaceBoundaryConditions.eq.0 .and. 
     & interfaceType(side,axis).eq.tractionInterface )then
          write(*,'(" cnsSlipWall: SKIP BC for (side,axis)=(",2i2,") 
     & interfaceType=",i3)') side,axis,interfaceType(side,axis)
         else

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

          ! ******************************************************************
          ! **********************slipWallDerivative *************************
          ! ******************************************************************


          if( debug.gt.1 ) then
            write(*,'(" cnsSlipWall: *NEW* slipWallDerivative used, 
     & t=",e10.2)') t
          end if

          ! ad2=5.
          ! ad2=max(10.,.5/dr(axisp1)) ! try this
          ! ad2=max(10.,1./dr(axisp1)) ! try this
          ad2=0.
          ! ad2=5.  ! for tanDiss2
          ad2dt=ad2*dt
          tm=t-dt
          z0=0.

          applyBoundaryContactFix=.false.
          addFouthOrderAD=.false.
          secondGhostLineOption=0 ! 2 ! 1 ! 0=default, 1=first-order extrap, 2=D+D-



          ! ======== START LOOP OVER BOUNDARY ======
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


            do mm=1,2
              ! extrap values on ghost line mm (1 and 2)
              j1=i1-is1*mm
              j2=i2-is2*mm
              j3=i3-is3*mm

              k1=i1+is1*mm
              k2=i2+is2*mm
              k3=i3+is3*mm

              ! treat rho differently -- blend with the symmetry condition
              m=rc
                ! here du2=2nd-order approximation, du3=third order
                ! Blend the 2nd and 3rd order based on the difference 
                !   (which equals the second difference: uNew(-1)-2*u(0)+u(1))
                du1 = u(j1+is1,j2+is2,j3,m)
                du2 = 2.*u(j1+is1,j2+is2,j3,m)-u(j1+2*is1,j2+2*is2,j3,
     & m)
                du3 = 3.*u(j1+is1,j2+is2,j3,m)-3.*u(j1+2*is1,j2+2*is2,
     & j3,m)+u(j1+3*is1,j2+3*is2,j3,m)
                uNorm= uEps+ abs(du3) + abs(u(j1+is1,j2+is2,j3,m))+abs(
     & u(j1+2*is1,j2+2*is2,j3,m))
              ! **   du = abs(du3-u(j1+is1,j2+is2,j3,m))/uNorm    ! changed 050711
              ! **   alpha = cdl*( du**2 + abs(du3-du2)/uNorm )
                alpha = cdl*( abs(du3-du2)/uNorm )
              !  alpha = cdl*( abs(du3-du2)/uNorm )
                alpha =min(1.,alpha)
                dus = u(k1,k2,k3,m)   ! use symmetric BC as limited value
                if( du2.lt.densityLowerBound .or. 
     & du3.lt.densityLowerBound )then
                  u(j1,j2,j3,m) = max(densityLowerBound,dus)
                else
                  u(j1,j2,j3,m)=(1.-alpha)*du3+alpha*dus
                end if
                ! ** u(j1,j2,j3,m)=(1.-alpha)*du2+alpha*dus
              if( mm.eq.2 )then
                if( secondGhostLineOption.eq.0 )then
                  ! use default
                else if( secondGhostLineOption.eq.1 )then
                  u(j1,j2,j3,m)=u(j1+is1,j2+is2,j3,m)
                else if( secondGhostLineOption.eq.2 )then
                  u(j1,j2,j3,m)=2.*u(j1+is1,j2+is2,j3,m)-u(j1+2*is1,j2+
     & 2*is2,j3,m)
                end if
              end if
              !  give values to all other components -- assumes rc==0 ---
              do m=1,ncm1
                !  if( mm.le.2 )then
                ! u(j1,j2,j3,m)=3.*u(j1+is1,j2+is2,j3,m)-3.*u(j1+2*is1,j2+2*is2,j3,m)+u(j1+3*is1,j2+3*is2,j3,m)
                !   else
                ! u(j1,j2,j3,m)=2.*u(i1,i2,i3,m)-u(k1,k2,k3,m)
                ! u(j1,j2,j3,m)=2.*u(i1,i2,i3,m)-u(k1,k2,k3,m)
                ! u(j1,j2,j3,m)=3.*u(j1+is1,j2+is2,j3,m)-3.*u(j1+2*is1,j2+2*is2,j3,m)+u(j1+3*is1,j2+3*is2,j3,m)

                ! *wdh* 11/07/10 -- do not apply limited extrap to the velocities. This seemed
                ! to work better in the 1-D fsi code.  
                ! if( m.eq.tc )then
                !   limitedExtrapolation(j1,j2,j3,m)
                ! else
                !   ! extrap3(j1,j2,j3,m)
                ! end if

                ! *wdh* Go back to limited extrapolation on all variables 2011/08/12 - better for elastic piston hit
                ! by a shock ??
                  ! here du2=2nd-order approximation, du3=third order
                  ! Blend the 2nd and 3rd order based on the difference 
                  !   (which equals the second difference: uNew(-1)-2*u(0)+u(1))
                  du1 = u(j1+is1,j2+is2,j3,m)
                  du2 = 2.*u(j1+is1,j2+is2,j3,m)-u(j1+2*is1,j2+2*is2,
     & j3,m)
                  du3 = 3.*u(j1+is1,j2+is2,j3,m)-3.*u(j1+2*is1,j2+2*
     & is2,j3,m)+u(j1+3*is1,j2+3*is2,j3,m)
                  !   alpha = cdl*(abs(du3-u(j1+is1,j2+is2,j3,m))+abs(du3-du2))/(uEps+abs(u(j1+is1,j2+is2,j3,m))+abs(u(j1+2*is1,j2+2*is2,j3,m)))
                  ! alpha = cdl*(abs(du3-du2))/(.1+abs(u(j1+is1,j2+is2,j3,m))+abs(u(j1+2*is1,j2+2*is2,j3,m)))
                  uNorm= uEps+ abs(du3) + abs(u(j1+is1,j2+is2,j3,m))+
     & abs(u(j1+2*is1,j2+2*is2,j3,m))
                ! **  du = abs(du3-u(j1+is1,j2+is2,j3,m))/uNorm  ! changed 050711
                ! **  alpha = cdl*( du**2 + abs(du3-du2)/uNorm )
                  alpha = cdl*( abs(du3-du2)/uNorm )
                !   alpha = cdl*( abs(du3-du2)/uNorm )
                  alpha =min(1.,alpha)
                  ! if( mm.eq.1 )then
                  !   u(j1,j2,j3,m)=(1.-alpha)*du3+alpha*du2
                    u(j1,j2,j3,m)=(1.-alpha)*du3+alpha*du1
                  ! else
                  !   u(j1,j2,j3,m)=(1.-alpha)*du2+alpha*du1
                  ! end if

                if( mm.eq.2 )then
                  if( secondGhostLineOption.eq.0 )then
                    ! use default
                  else if( secondGhostLineOption.eq.1 )then
                    u(j1,j2,j3,m)=u(j1+is1,j2+is2,j3,m)
                  else if( secondGhostLineOption.eq.2 )then
                    u(j1,j2,j3,m)=2.*u(j1+is1,j2+is2,j3,m)-u(j1+2*is1,
     & j2+2*is2,j3,m)
                  end if
                end if
                !   end if
              end do
              ! m=rc
              ! u(j1,j2,j3,m)=3.*u(j1+is1,j2+is2,j3,m)-3.*u(j1+2*is1,j2+2*is2,j3,m)+u(j1+3*is1,j2+3*is2,j3,m)
              ! m=uc
              ! u(j1,j2,j3,m)=3.*u(j1+is1,j2+is2,j3,m)-3.*u(j1+2*is1,j2+2*is2,j3,m)+u(j1+3*is1,j2+3*is2,j3,m)

              ! u.x = -[ v.y + (p.t + vp.y)/(gamma*p) ] 
              ! uh=rxi*dr(axis)*sgn*mm
              ! p0=u(i1,i2,i3,rc)*u(i1,i2,i3,tc)
              ! p2=u2(i1,i2,i3,rc)*u2(i1,i2,i3,tc)
              ! pt0=(p0-p2)/dt
              ! u(j1,j2,j3,uc)=u(k1,k2,k3,uc) - 2.*h*( -pt0/(gamma*p0) )

              ! nDotuv(mm)=an1*u1+an2*v1 

              ! *wdh* 2012/03/13 -- try this to detect wall heating/cooling 
              if( checkForWallHeating.eq.1 )then
                dRho =u(i1+is1,i2+is2,i3+is3,rc)-u(i1,i2,i3,rc)
                dTemp=u(i1+is1,i2+is2,i3+is3,tc)-u(i1,i2,i3,tc)
                dp  =(u(i1+is1,i2+is2,i3+is3,rc)*u(i1+is1,i2+is2,i3+
     & is3,tc) - u(i1,i2,i3,rc)*u(i1,i2,i3,tc))
                ! if( dRho*dTemp .lt. 0. )then
                if( dRho*dTemp .lt. -.05*abs(dp) )then
                  ! Wall heating or cooling: 
                  !   p is nearly constant
                  !   D+rho is large and of opposite sign to D+T 
                  ! if D+(rho) * D+( T ) < -|D+ p| : use symmetry condition:
                  !  -- The factor of 4. here is just a fudge factor chosen from a couple of examples
                  alpha = min( 1., abs(4.*dRho*dTemp/( abs(dp)+
     & pressureLowerBound )))


                  u(j1,j2,j3,rc)=alpha*u(k1,k2,k3,rc) + (1.-alpha)*u(
     & j1,j2,j3,rc)
                  u(j1,j2,j3,tc)=alpha*u(k1,k2,k3,tc) + (1.-alpha)*u(
     & j1,j2,j3,tc)

                  ! -- now fix up the velocity --
                  ! (um, vm) : fully limited values for the velocity

                  ! n.u(-1) = n.( 2*u(0)-u(+1) )
                  ! t.u(-1) = t.u(+1) 
                  ! first make both components even
                  um=u(k1,k2,k3,uc)
                  vm=u(k1,k2,k3,vc)
                  ! now fix-up the normal component
                  nDotU=an1*(-um + 2.*u(i1,i2,i3,uc)-u(k1,k2,k3,uc) )+
     & an2*(-vm + 2.*u(i1,i2,i3,vc)-u(k1,k2,k3,vc) )
                  um=um+ nDotU*an1
                  vm=vm+ nDotU*an2
                  u(j1,j2,j3,uc)=alpha*um + (1.-alpha)*u(j1,j2,j3,uc)
                  u(j1,j2,j3,vc)=alpha*vm + (1.-alpha)*u(j1,j2,j3,vc)

                  if( mm.eq.1 .and. alpha.gt.0.5 )then
                    ! Set value of rho and T on the boundary to the average of u(-1) and u(+1)
                    u(i1,i2,i3,rc)=.5*(u(j1,j2,j3,rc)+u(k1,k2,k3,rc))
                    u(i1,i2,i3,tc)=.5*(u(j1,j2,j3,tc)+u(k1,k2,k3,tc))

                    ! also average u and v on the boundary
                    u(i1,i2,i3,uc)=.5*(u(j1,j2,j3,uc)+u(k1,k2,k3,uc))
                    u(i1,i2,i3,vc)=.5*(u(j1,j2,j3,vc)+u(k1,k2,k3,vc))

                  end if

                end if
              end if

              ! *wdh* 2011/07/11 - apply lower bound to rho on the ghost points
              u(j1,j2,j3,rc) = max(u(j1,j2,j3,rc),densityLowerBound)


            end do ! mm


            ! try a second order 1-sided for rho only
            ! m=rc
            ! rr1 = (-3.*u(i1,i2,i3,m)+4.*u(i1+is1,i2+is2,i3,m)-u(i1+2*is1,i2+2*is2,i3,m))/(2.*dr(axis))


            ! if( .false. .and. twilightZone.ne.0 )then
            !   ! for testing fixup extrapolation of rho,u,v
            !   ! u(i1-is1,i2-is2,i3,rc) = 3.*u(i1) -3*u(i1+is1)+u(i1+2*is2)
            !   ! u(i1-is1,i2-is2,i3,rc) = u(i1-is1) + ue(i1-is1) -3.*ue(i1) + 3*ue(i1+is2)-ue(i1+is2)
            !   do m=0,nd
            !     u(i1-is1,i2-is2,i3,m)=u(i1-is1,i2-is2,i3,m) + !                          ogf(ep,x(i1-  is1,i2-  is2,i3,0),x(i1-  is1,i2-  is2,i3,1),0.,m,t) !                      -3.*ogf(ep,x(i1      ,i2      ,i3,0),x(i1      ,i2      ,i3,1),0.,m,t) !                      +3.*ogf(ep,x(i1+  is1,i2+  is2,i3,0),x(i1+  is1,i2+  is2,i3,1),0.,m,t) !                      -   ogf(ep,x(i1+2*is1,i2+2*is2,i3,0),x(i1+2*is1,i2+2*is2,i3,1),0.,m,t) 
            !   end do
            ! end if

            if( twilightZone.ne.0 )then
              ! Where there is inflow set exact values for (rho,u,v)  *wdh* 2011/07/10
              ! Note: (an1,an2) is the outward normal so inflow is where n.uv < 0 
              if( an1*u(i1,i2,i3,uc)+an2*u(i1,i2,i3,vc).lt.0. )then
               do mm=1,2
                j1=i1-is1*mm
                j2=i2-is2*mm
                j3=i3-is3*mm
                do m=0,nd
                  u(j1,j2,j3,m)=ogf(ep,x(j1,j2,j3,0),x(j1,j2,j3,1),0.,
     & m,t)
                end do
               end do
              end if

            end if

            if( .false. )then
              ! for testing -- set predicted ghost values to be exact
              do mm=1,2
                j1=i1-is1*mm
                j2=i2-is2*mm
                j3=i3-is3*mm
                do m=0,ncm1
                  u(j1,j2,j3,m)=ogf(ep,x(j1,j2,j3,0),x(j1,j2,j3,1),0.,
     & m,t)
                end do
              end do
            end if

            if( axis.eq.0 )then
              rr1 = ur2(i1,i2,i3,rc)
            else
              rr1 = us2(i1,i2,i3,rc)
            end if

            ! BC: p.n = rho*( ... )
            rho = u(i1,i2,i3,rc)
            tp  = u(i1,i2,i3,tc)
            us0=(u(i1+js1,i2+js2,i3,uc)-u(i1-js1,i2-js2,i3,uc))/(2.*dr(
     & axisp1))
            vs0=(u(i1+js1,i2+js2,i3,vc)-u(i1-js1,i2-js2,i3,vc))/(2.*dr(
     & axisp1))

            pn = -rho*( sxi*u(i1,i2,i3,uc)+syi*u(i1,i2,i3,vc) )*(an1*
     & us0+an2*vs0)
            ! *wdh* 110709 - do not add grid acceleration for TZ:
            if( gridIsMoving.ne.0 .and. twilightZone.eq.0 )then

              ! write(*,'(" -- add gtt term to pn, gtt=",2f6.3)') gtt(i1,i2,i3,0),gtt(i1,i2,i3,1)

              pn = pn - rho*( an1*gtt(i1,i2,i3,0)+an2*gtt(i1,i2,i3,1) )
     &  + rho*( sxi*gv(i1,i2,i3,0)+syi*gv(i1,i2,i3,1) )*(an1*us0+an2*
     & vs0)

              ! -- Add term   d(n)/dt . ( u- gv ) ---

              ! Compute the time derivatives of the normal:
              ! We use the matrix formula for the Jaconian: 
              !           [ r.x ] * [x.r] = I 
              ! -> [ rt.x ]*[x.r] + [r.x]*[xt.r] = 0 
              ! xt.r, xt.s, yt.r, yt.s  come from spatial derivatives of the grid velocity:
              gur = ( gv(i1+1,i2,i3,0) - gv(i1-1,i2,i3,0) )/(2.*dr(0))  ! xt.r
              gus = ( gv(i1,i2+1,i3,0) - gv(i1,i2-1,i3,0) )/(2.*dr(1))  ! xt.s

              gvr = ( gv(i1+1,i2,i3,1) - gv(i1-1,i2,i3,1) )/(2.*dr(0))
              gvs = ( gv(i1,i2+1,i3,1) - gv(i1,i2-1,i3,1) )/(2.*dr(1))

              rxa = rsxy(i1,i2,i3,0,0)
              rya = rsxy(i1,i2,i3,0,1)
              sxa = rsxy(i1,i2,i3,1,0)
              sya = rsxy(i1,i2,i3,1,1)
              if( axis.eq.0 )then
                rxt = -( gur*rxa*rxa + gus*rxa*sxa + gvr*rxa*rya + gvs*
     & rya*sxa )
                ryt = -( gur*rxa*rya + gus*rxa*sya + gvr*rya*rya + gvs*
     & rya*sya )
                ! Outward normal:
                fact = -sgn*( rxt*rya-ryt*rxa)/( (rxa*rxa+rya*rya)**(
     & 1.5) )
                an1t = rya*fact
                an2t =-rxa*fact

              else

                sxt = -( gur*rxa*sxa + gus*sxa*sxa + gvr*rxa*sya + gvs*
     & sxa*sya )
                syt = -( gur*rya*sxa + gus*sxa*sya + gvr*rya*sya + gvs*
     & sya*sya )
                ! Outward normal:
                fact = -sgn*( sxt*sya-syt*sxa)/( (sxa*sxa+sya*sya)**(
     & 1.5) )
                an1t = sya*fact
                an2t =-sxa*fact

              end if

              pn = pn + rho*( an1t*( u(i1,i2,i3,uc)-gv(i1,i2,i3,0) ) + 
     & an2t*( u(i1,i2,i3,vc)-gv(i1,i2,i3,1) ) )

              !write(*,'(" slipBC: gv    =",2e12.5)') gv(i1,i2,i3,0),gv(i1,i2,i3,1)
              !write(*,'(" slipBC: (u,v) =",2e12.5)') u(i1,i2,i3,uc),u(i1,i2,i3,vc)

              !write(*,'(" slipBC: n.gtt, nt.(u-gt) =",2e13.5)') an1*gtt(i1,i2,i3,0)+an2*gtt(i1,i2,i3,1),!     an1t*( u(i1,i2,i3,uc)-gv(i1,i2,i3,0) ) + an2t*( u(i1,i2,i3,vc)-gv(i1,i2,i3,1) )
              !write(*,'(" slipBC: pn=",e12.5)') pn

              !write(*,'(" slipBC: u.gradu=",e10.3)') ( sxi*u(i1,i2,i3,uc)+syi*u(i1,i2,i3,vc) )*(an1*us0+an2*vs0)
              !write(*,'(" slipBC: g.gradu=",e10.3)') ( sxi*gv(i1,i2,i3,0)+syi*gv(i1,i2,i3,1) )*(an1*us0+an2*vs0)
              !write(*,'(" slipBC: pn     =",e10.3)') pn
            end if

            if( addFouthOrderAD )then
              !include  fourth order artificial diffusion in the momentum equations
              pn = pn + an1*ad(uc)*( (-12.*u2(i1,i2,i3,uc)+4.*(u2(i1+1,
     & i2,i3,uc)+u2(i1-1,i2,i3,uc)+u2(i1,i2+1,i3,uc)+u2(i1,i2-1,i3,uc)
     & )-(u2(i1+2,i2,i3,uc)+u2(i1-2,i2,i3,uc)+u2(i1,i2+2,i3,uc)+u2(i1,
     & i2-2,i3,uc))) ) + an2*ad(vc)*( (-12.*u2(i1,i2,i3,vc)+4.*(u2(i1+
     & 1,i2,i3,vc)+u2(i1-1,i2,i3,vc)+u2(i1,i2+1,i3,vc)+u2(i1,i2-1,i3,
     & vc))-(u2(i1+2,i2,i3,vc)+u2(i1-2,i2,i3,vc)+u2(i1,i2+2,i3,vc)+u2(
     & i1,i2-2,i3,vc))) )
            end if

            if( twilightZone.ne.0 )then
              ! evaluate TZ forcing at t

              if( .true. )then
               ! -- new way *wdh* 110710 -- basically same results as old but probably faster
               call ogderiv(ep, 0,0,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,rc,re)
               call ogderiv(ep, 0,0,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,uc,ue)
               call ogderiv(ep, 0,0,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,vc,ve)
               call ogderiv(ep, 0,0,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,tc,te)

               call ogderiv(ep, 0,1,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,rc,rxe)
               call ogderiv(ep, 0,1,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,uc,uxe)
               call ogderiv(ep, 0,1,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,vc,vxe)
               call ogderiv(ep, 0,1,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,tc,txe)

               call ogderiv(ep, 0,0,1,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,rc,rye)
               call ogderiv(ep, 0,0,1,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,uc,uye)
               call ogderiv(ep, 0,0,1,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,vc,vye)
               call ogderiv(ep, 0,0,1,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,tc,tye)
               ! p= rho*T
               pxe = rxe*te+re*txe
               pye = rye*te+re*tye
               use = xsi*uxe + ysi*uye
               vse = xsi*vxe + ysi*vye
               pn = pn + re*( sxi*ue + syi*ve )*( an1*use + an2*vse ) +
     &  an1*pxe + an2*pye

               ! pn = an1*pxe + an2*pye ! for testing, set pn=exact

              else
                ! ----- OLD:
                ! fv(0) = rf = rt + u*rx+v*ry + r*(ux+vy);
                ! fv(1) = uf = ut + u*ux+v*uy + px/r;
                call ogftaylor(ep,x(i1,i2,i3,0),x(i1,i2,i3,1),z0,t,nd,
     & fv)
                call ogderiv(ep, 1,0,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,uc,ute)
                call ogderiv(ep, 1,0,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,vc,vte)
                call ogderiv(ep, 0,1,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,uc,uxe)
                call ogderiv(ep, 0,1,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,vc,vxe)
                call ogderiv(ep, 0,0,1,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,uc,uye)
                call ogderiv(ep, 0,0,1,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,vc,vye)
                rxU=rxi*u(i1,i2,i3,uc)+ryi*u(i1,i2,i3,vc)
                ure = xri*uxe+yri*uye ! ur (exact)
                vre = xri*vxe+yri*vye ! ur (exact)
                pn = pn + rho*( an1*(fv(1)-ute-rxU*ure) + an2*(fv(2)-
     & vte-rxU*vre) )
              end if

              ! write(*,'(" cnsSlipWall: slipWallDerivative set TZ for pn")')

            end if


            if( .true. )then
              ! *new* way  *wdh* 2011/07/10
              ! pn = an1*px + an2*py = (an1*rx +an2*ry)*pr + (an1*sx + an2*sy)*ps 

              ! compute p=rho*T at neighbouring points
              pmz = u(i1-1,i2  ,i3,rc)*u(i1-1,i2  ,i3,tc)
              ppz = u(i1+1,i2  ,i3,rc)*u(i1+1,i2  ,i3,tc)
              pzz = u(i1  ,i2  ,i3,rc)*u(i1  ,i2  ,i3,tc)
              pzm = u(i1  ,i2-1,i3,rc)*u(i1  ,i2-1,i3,tc)
              pzp = u(i1  ,i2+1,i3,rc)*u(i1  ,i2+1,i3,tc)

              ! (an1*rx +an2*ry)*(ppz-pmz)/(2*dr) + (an1*sx + an2*sy)*(pzp-pzm)/(2*ds) = pn 
              if( axis.eq.0 )then
                ps = (pzp-pzm)/(2.*dr(1))
                pr = (pn - (an1*rsxy(i1,i2,i3,1,0) + an2*rsxy(i1,i2,i3,
     & 1,1))*ps )/((an1*rsxy(i1,i2,i3,0,0) +an2*rsxy(i1,i2,i3,0,1)))
                ! pr = (ppz-pmz)/(2*dr)
                if( side.eq.0 )then
                  pm1 = ppz - 2.*dr(0)*pr
                  pm2 = u(i1+2,i2  ,i3,rc)*u(i1+2,i2  ,i3,tc) - 4.*dr(
     & 0)*pr
                else if( side.eq.1 )then
                  pm1 = pmz + 2.*dr(0)*pr
                  pm2 = u(i1-2,i2  ,i3,rc)*u(i1-2,i2  ,i3,tc) + 4.*dr(
     & 0)*pr
                end if
              else
                pr = (ppz-pmz)/(2.*dr(0))
                ps = (pn - (an1*rsxy(i1,i2,i3,0,0) + an2*rsxy(i1,i2,i3,
     & 0,1))*pr )/((an1*rsxy(i1,i2,i3,1,0) +an2*rsxy(i1,i2,i3,1,1)))
                ! ps = (pzp-pzm)/(2*ds)
                if( side.eq.0 )then
                  pm1 = pzp - 2.*dr(1)*ps
                  pm2 = u(i1  ,i2+2,i3,rc)*u(i1  ,i2+2,i3,tc) - 4.*dr(
     & 1)*ps
                else if( side.eq.1 )then
                  pm1 = pzm + 2.*dr(1)*ps
                  pm2 = u(i1  ,i2-2,i3,rc)*u(i1  ,i2-2,i3,tc) + 4.*dr(
     & 1)*ps
                end if
              end if

              if( .false. )then
                call ogderiv(ep, 0,0,0,0, x(i1-is1,i2-is2,i3,0),x(i1-
     & is1,i2-is2,i3,1),z0,t,rc,re)
                call ogderiv(ep, 0,0,0,0, x(i1-is1,i2-is2,i3,0),x(i1-
     & is1,i2-is2,i3,1),z0,t,tc,te)

                pe=re*te
                write(*,'(" SlipBC2: p at ghost=(",i3,",",i3"): pm1=",
     & e10.3," exact=",e10.3)') i1,i2,pm1,pe
                pm1=pe

              end if

              pm1 = max(pm1,pressureLowerBound)
              pm2 = max(pm2,pressureLowerBound)


              ! Set T at the ghost value from p/rho
              u(i1-  is1,i2-  is2,i3,tc)= pm1/u(i1-  is1,i2-  is2,i3,
     & rc)
              ! limitedExtrapolation(i1-2*is1,i2-2*is2,i3,tc)
              u(i1-2*is1,i2-2*is2,i3,tc)= pm2/u(i1-2*is1,i2-2*is2,i3,
     & rc)

              !write(*,'(" slipBC: p, pm1 =",2e10.3)') pzz,pm1
              !write(*,'(" slipBC: T, Tm1 =",2e10.3)') u(i1,i2,i3,tc),u(i1-is1,i2-is2,i3,tc)

              if( .false. )then
                ! compute T and rho from isentropic relationship: p/rho^gamma = const
                kappa = (rho*tp)/rho**gamma
                rm1 = (pm1/kappa)**(1./gamma)
                rm2 = (pm2/kappa)**(1./gamma)
                if( twilightZone.ne.0 )then
                 ! p/rho^gamma = pe/rhoe^gamma 
                 ! given p(-1) compute rho(-1) = rhoe*(p/pe)**(1/gamma)
                 call ogderiv(ep, 0,0,0,0, x(i1-is1,i2-is2,i3,0),x(i1-
     & is1,i2-is2,i3,1),z0,t,rc,re)
                 call ogderiv(ep, 0,0,0,0, x(i1-is1,i2-is2,i3,0),x(i1-
     & is1,i2-is2,i3,1),z0,t,tc,te)
                 pe = re*te
                 rm1 = re*(pm1/pe)**(1./gamma)

                 call ogderiv(ep, 0,0,0,0, x(i1-2*is1,i2-2*is2,i3,0),x(
     & i1-2*is1,i2-2*is2,i3,1),z0,t,rc,re)
                 call ogderiv(ep, 0,0,0,0, x(i1-2*is1,i2-2*is2,i3,0),x(
     & i1-2*is1,i2-2*is2,i3,1),z0,t,tc,te)
                 pe = re*te
                 rm2 = re*(pm2/pe)**(1./gamma)

                end if
                u(i1-  is1,i2-  is2,i3,rc) = rm1
                u(i1-  is1,i2-  is2,i3,tc) = pm1/rm1
                u(i1-2*is1,i2-2*is2,i3,rc)= rm2
                u(i1-2*is1,i2-2*is2,i3,tc)= pm2/rm2
              end if
            else
              ! old way
              rhos=(u(i1+js1,i2+js2,i3,rc)-u(i1-js1,i2-js2,i3,rc))/(2.*
     & dr(axisp1))
              tps =(u(i1+js1,i2+js2,i3,tc)-u(i1-js1,i2-js2,i3,tc))/(2.*
     & dr(axisp1))
              ps =  rhos*tp + rho*tps
              pr = ( pn - (an1*sxi+an2*syi)*ps )/(an1*rxi+an2*ryi)

              ! now get a guess for T(-1) given rhor and pr 
              rhor=rr1
              rho=u(i1,i2,i3,rc)
              tp=u(i1,i2,i3,tc)
              tpr = (pr-rhor*tp)/rho

              u(i1-  is1,i2-  is2,i3,tc)=u(i1+  is1,i2+  is2,i3,tc) -
     & sgn*2.*dr(axis)*tpr
              u(i1-2*is1,i2-2*is2,i3,tc)=u(i1+2*is1,i2+2*is2,i3,tc) -
     & sgn*4.*dr(axis)*tpr
            end if

            ! *wdh* turn off this second limiter step now that we apply lower bounds to p 
            ! limitedTemperatureExtrapolation(i1-is1,i2-is2,i3,tc)
            ! limitedTemperatureExtrapolation(i1-2*is1,i2-2*is2,i3,tc)

            !write(*,'(" slipBC: limited: T, Tm1 =",2e10.3)') u(i1,i2,i3,tc),u(i1-is1,i2-is2,i3,tc)

            if( applyBoundaryContactFix )then
              pNorm=max(abs(rhor),abs(tpr))*.1
              omega=.125
              pEps= - (pr*pr + rho*tp*1.e-8 )
              if( rhor*tpr.lt.pEps .and. abs(pr).lt.pNorm )then
                ! This is a cosmetic fix to remove a "contact" discontinuity that sits
                ! on the boundary with a jump in rho, a jump in T but no jump in p
                write(*,'("Apply contact fix at SWBC i1,i2=",2i4," 
     & rhor,tpr,pr=",3e10.3)') i1,i2,rhor,tpr,pr
                ! '

                ! u(i1,i2,i3,rc)=(1.-omega)*u(i1,i2,i3,rc)+!           omega*(2.*u(i1+  is1,i2+  is2,i3,rc)-u(i1+2*is1,i2+2*is2,i3,rc))
                ! u(i1,i2,i3,tc)=(1.-omega)*u(i1,i2,i3,tc)+!           omega*(2.*u(i1+  is1,i2+  is2,i3,tc)-u(i1+2*is1,i2+2*is2,i3,tc))

                u(i1,i2,i3,rc)=(1.-omega)*u(i1,i2,i3,rc)+ omega*u(i1+  
     & is1,i2+  is2,i3,rc)
                u(i1,i2,i3,tc)=(1.-omega)*u(i1,i2,i3,tc)+ omega*u(i1+  
     & is1,i2+  is2,i3,tc)
              end if
            end if


c$$$            if( secondGhostLineOption.eq.0 )then
c$$$            else if( secondGhostLineOption.eq.1 )then
c$$$              u(i1-2*is1,i2-2*is2,i3,tc)=u(i1-is1,i2-is2,i3,tc)
c$$$            else if( secondGhostLineOption.eq.2 )then
c$$$              u(i1-2*is1,i2-2*is2,i3,tc)=2.*u(i1-is1,i2-is2,i3,tc)-u(i1,i2,i3,tc)
c$$$            else
c$$$              stop 8527
c$$$            end if

            if( .true. .and. debug.gt.4 .and. twilightZone.ne.0  )then ! *************************

              ! check solution
              do m=0,ncm1
                uv(m)=ogf(ep,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,m,t)
              end do
              call ogderiv(ep, 0,1,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),z0,
     & t,rc,rxe)
              call ogderiv(ep, 0,1,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),z0,
     & t,tc,qxe)
              call ogderiv(ep, 0,0,1,0, x(i1,i2,i3,0),x(i1,i2,i3,1),z0,
     & t,rc,rye)
              call ogderiv(ep, 0,0,1,0, x(i1,i2,i3,0),x(i1,i2,i3,1),z0,
     & t,tc,qye)

              rre= xri*rxe+ yri*rye

              ! here is p.n = rho*T.n + rho.n*T  (exact)
              pne = uv(rc)*(an1*qxe+an2*qye) + (an1*rxe+an2*rye)*uv(tc)

              do mm=1,2
               j1=i1-is1*mm
               j2=i2-is2*mm
               j3=i3-is3*mm
               do m=0,ncm1
                 uv(m)=ogf(ep,x(j1,j2,j3,0),x(j1,j2,j3,1),z0,m,t)
               end do

              write(*,'("--> ghost at t=",e10.3," j1,j2=",2i3," ruvT=",
     & 4(f8.5,1x)," err=",4(e8.1,1x)," pn,err=",f10.5,1x,e10.1)') t,
     & j1,j2,u(j1,j2,j3,rc),u(j1,j2,j3,uc),u(j1,j2,j3,vc),u(j1,j2,j3,
     & tc),abs(u(j1,j2,j3,rc)-uv(rc)),abs(u(j1,j2,j3,uc)-uv(uc)),abs(
     & u(j1,j2,j3,vc)-uv(vc)),abs(u(j1,j2,j3,tc)-uv(tc)),pn,abs(pn-
     & pne)
                !'
              do m=0,ncm1
                uv(m)=ogf(ep,x(j1,j2,j3,0),x(j1,j2,j3,1),0.,m,tm)
              end do
              write(*,'("  ghost at t-dt: ruv2=",3(f10.5,1x)," err2=",
     & 3(e10.3,1x))') u2(j1,j2,j3,rc),u2(j1,j2,j3,uc),u2(j1,j2,j3,vc),
     & abs(u2(j1,j2,j3,rc)-uv(rc)),abs(u2(j1,j2,j3,uc)-uv(uc)),abs(u2(
     & j1,j2,j3,vc)-uv(vc))
               !'
              end do

            end if ! check solution



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
           end do
           end do
           end do


          ! ** assign values on the extended ghost lines
             ! ** assign values on the extended ghost lines
             !
             !         X  X  +
             !         G  G  + ---------------------
             !         G  G  +
             !         G  G  +
             !         G  G  +
             !         G  G  +
             !         G  G  + ---------------------
             !         X  X  +
             ! write(*,'(" end of stage I: nr = ",4i4)') nr(0,0),nr(1,0),nr(0,1),nr(1,1)
             ms1=0
             ms2=0
             ms3=0
             i3=nr(0,2)
             j3=i3
             side2=-1
             do sideb=0,1
               ! used side2 instead if sideb, this is needed to avoid a bug with pgf77 compiled with -O !
               side2=side2+1
               if( axis.eq.0 )then
                 i1=nr(side ,0)
                 i2=nr(side2,1)
                 ms2=1-2*side2
                 ! write(*,'(" end of stage I: sideb,side2,nr(0,1),nr(sideb,1),nr(side2,1)=",10i4)') sideb,side2,nr(0,1),nr(sideb,1),nr(side2,1)
                 ! '
               else
                 i1=nr(side2,0)
                 i2=nr(side ,1)
                 ms1=1-2*side2
               end if
               do mm=1,2   ! two ghost lines
                 if( axis.eq.0 )then
                   j1=i1-is1*mm
                   j2=i2-ms2
                 else
                   j1=i1-ms1
                   j2=i2-is2*mm
                 end if
                 ! write(*,'(" end of stage I: fill extended ghost value i1,i2,ms1,ms2,j1,j2=",6i4)') i1,i2,ms1,ms2,j1,j2
                 ! '
                 if( bc(side2,axisp1).lt.0 )then
                   ! apply periodicity
                   kv(0)=j1
                   kv(1)=j2
                   kv(2)=j3
                   kv(axisp1) = kv(axisp1) + (nr(1,axisp1)-nr(0,axisp1)
     & )*(1-2*side2)
                   !write(*,'(" end of stage I: periodic update j1,j2,j3=",3i4," from k1,k2,k3=",3i4)') !       j1,j2,j3,kv(0),kv(1),kv(2)
                   ! '
                   do m=0,ncm1
                     u(j1,j2,j3,m)=u(kv(0),kv(1),kv(2),m)
                   end do
                 else if( .true. )then ! turn this off for now  ***********
                 do m=0,ncm1
                   u(j1,j2,j3,m)=3.*u(j1+  ms1,j2+  ms2,j3+  ms3,m)-3.*
     & u(j1+2*ms1,j2+2*ms2,j3+2*ms3,m)+u(j1+3*ms1,j2+3*ms2,j3+3*ms3,m)
                 end do
                 end if
               end do
             end do


          nr(0,axis)=indexRange(0,axis)  ! reset
          nr(1,axis)=indexRange(1,axis)


          ! ******************** END SlipWallDerivative ***********************************
          ! ***************************************************************************

         end if ! not tractionInterface
        else if( bc(side,axis).eq.axisymmetric )then

          ! axisymmetric -- this is not quite right for stretched grids ---- fix this ---

          nr(0,axis)=indexRange(side,axis)   ! set nr to point to this side
          nr(1,axis)=indexRange(side,axis)

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
            u(i1,i2,i3,vc)=0.
            u(i1-is1,i2-is2,i3,urc)=-u(i1+is1,i2+is2,i3,urc)
            u(i1-ks1,i2-ks2,i3,urc)=-u(i1+ks1,i2+ks2,i3,urc)

            u(i1-is1,i2-is2,i3,rc)= u(i1+is1,i2+is2,i3,rc)
            u(i1-ks1,i2-ks2,i3,rc)= u(i1+ks1,i2+ks2,i3,rc)

            u(i1-is1,i2-is2,i3,uac)= u(i1+is1,i2+is2,i3,uac)
            u(i1-ks1,i2-ks2,i3,uac)= u(i1+ks1,i2+ks2,i3,uac)

            u(i1-is1,i2-is2,i3,tc)= u(i1+is1,i2+is2,i3,tc)
            u(i1-ks1,i2-ks2,i3,tc)= u(i1+ks1,i2+ks2,i3,tc)

           end do
           end do
           end do

          if( axisymmetricWithSwirl.eq.1 )then
             n1a=nr(0,0)
             n1b=nr(1,0)
             n2a=nr(0,1)
             n2b=nr(1,1)
             n3a=nr(0,2)
             n3b=nr(1,2)
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
              u(i1-is1,i2-is2,i3,wc)= u(i1+is1,i2+is2,i3,wc)
              u(i1-ks1,i2-ks2,i3,wc)= u(i1+ks1,i2+ks2,i3,wc)
             end do
             end do
             end do
          end if

          ! species
          do s=sc,sc+numberOfSpecies-1
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
            u(i1-is1,i2-is2,i3,s)= u(i1+is1,i2+is2,i3,s)
            u(i1-ks1,i2-ks2,i3,s)= u(i1+ks1,i2+ks2,i3,s)
            end if
           end do
           end do
           end do
          end do

          nr(0,axis)=indexRange(0,axis)  ! reset
          nr(1,axis)=indexRange(1,axis)

        end if

        end do
        end do






      ! ******************* new slipWallDerivative for curvilinear grids **********************************
      else if( nd.eq.2 .and. gridType.eq.curvilinear .and. 
     & bcOption.eq.slipWallDerivative )then

        if( .true. )then

          ! *wdh* 2012/03/30
          write(*,'("cnsSlipwallBC2: ERROR: THIS OPTION IS OUTDATED I 
     & THINK!")')
          stop 6634
        end if

        ! check the bounds on pra(ii) and psa(ii)
        if( nd1a.gt.ndMax .or. nd2a.gt.ndMax )then
          write(*,'("cnsSlipwallBC2: ERROR: dimension ndMax exceeded")
     & ')
          stop 111
        end if


        do axis=0,nd-1
        do side=0,1
        if( bc(side,axis).eq.slipWall )then

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

          if( bcOption.eq.slipWallDerivative .and. dt.gt.0 )then

           ! ******************************************************************
           ! **********************slipWallDerivative**************************
           ! ******************************************************************

           if( gridIsMoving.eq.1 )then
             write(*,'(" cnsSlipWall: *NEW* slipWallDerivative used 
     & with moving grids")')
             ! '
           endif

           if( debug.gt.1 ) then
             write(*,'(" cnsSlipWall: *NEW* slipWallDerivative used")')
           end if
           if( dt.lt.0. )then
             write(*,'(" ***cnsSlipWall:WARNING: dt<0 for t=",e12.3)') 
     & t
             dt=0.
           else
             if( debug.gt.1 ) then
               write(*,'(" ***cnsSlipWall:INFO: t,dt=",2(e12.3,1x))') 
     & t,dt
             end if
           end if

           ! ad2=5.
           ! ad2=max(10.,.5/dr(axisp1)) ! try this
           ! ad2=max(10.,1./dr(axisp1)) ! try this
           ad2=0.
           ! ad2=5.  ! for tanDiss2
           ad2dt=ad2*dt
           tm=t-dt
           z0=0.

           drEps=1.e-4
           dtEps=drEps
           dsEps=drEps

           ! cnSmooth: for adding add smoothing in the normal direction for ghost points
           ! if cnSmooth==1 --> D+D-u = 0 : choose cnSmooth=c*h^2 I think
           cnSmooth=0. ! 1. ! 1.*dr(axis)


           getGhostByTaylor=.true.
           addFouthOrderAD=.true.
           secondGhostLineOption=0 ! 1 ! 1=first-order extrap



           ! ======== STAGE I: predict values for rho.r, u.r, v.r, and T.r from a forward euler step
           !                   compute pr(i), ps(i)
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


c$$$            if( twilightZone.ne.0 )then
c$$$              ! evaluate TZ forcing at t
c$$$              call ogderiv(ep, 1,0,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),z0,tm,rc,rte)
c$$$              call ogderiv(ep, 0,1,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),z0,tm,rc,rxe)
c$$$              call ogderiv(ep, 0,0,1,0, x(i1,i2,i3,0),x(i1,i2,i3,1),z0,tm,rc,rye)
c$$$              call ogderiv(ep, 0,2,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),z0,tm,rc,rxxe)
c$$$              call ogderiv(ep, 0,1,1,0, x(i1,i2,i3,0),x(i1,i2,i3,1),z0,tm,rc,rxye)
c$$$              call ogderiv(ep, 0,0,2,0, x(i1,i2,i3,0),x(i1,i2,i3,1),z0,tm,rc,ryye)
c$$$              call ogderiv(ep, 1,1,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),z0,tm,rc,rxte)
c$$$              call ogderiv(ep, 1,0,1,0, x(i1,i2,i3,0),x(i1,i2,i3,1),z0,tm,rc,ryte)
c$$$              call ogderiv(ep, 2,0,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),z0,tm,rc,rtte)
c$$$      write(*,'("--> stage I: i1,i2=",2i3," err: rt,rx,ry,rxx,rxy,ryy,rxt,ryt,rtt=",10(e8.1,1x))') i1,i2,c$$$                abs(rt0-rte),abs(rx0-rxe),abs(ry0-rye),abs(rxx0-rxxe),abs(rxy0-rxye),abs(ryy0-ryye),c$$$                abs(rtx0-rxte),abs(rty0-ryte),abs(rtt0-rtte)
c$$$              !'
c$$$            end if


            if( getGhostByTaylor )then
            ! Get r, u,v at ghost points by Taylor series from the boundary at the previous time 
             do mm=1,2
              j1=i1-is1*mm
              j2=i2-is2*mm
              j3=i3-is3*mm


              ! u.tau \approx sx*(rx*ur + sx*us) + sy*(ry*ur+sy*us) / sqrt( sx^2 + sy^2 )
              ! compute coeffcienst of dissipation that are proprootional to |u.tau| 
              ! computeAdu(j1,j2,j3,js1,js2,js3)
              if( .false. )then
                hx = x(j1,j2,j3,0)-x(i1,i2,i3,0)
                hy = x(j1,j2,j3,1)-x(i1,i2,i3,1)
                r1 = r0 + dt*rt0 + hx*rx0 + hy*ry0 +.5*dt**2*rtt0 + dt*
     & ( hx*rtx0+hy*rty0 ) + .5*hx**2*rxx0 + hx*hy*rxy0 + .5*hy**2*
     & ryy0
                u1 = u0 + dt*ut0 + hx*ux0 + hy*uy0 +.5*dt**2*utt0 + dt*
     & ( hx*utx0+hy*uty0 ) + .5*hx**2*uxx0 + hx*hy*uxy0 + .5*hy**2*
     & uyy0
                v1 = v0 + dt*vt0 + hx*vx0 + hy*vy0 +.5*dt**2*vtt0 + dt*
     & ( hx*vtx0+hy*vty0 ) + .5*hx**2*vxx0 + hx*hy*vxy0 + .5*hy**2*
     & vyy0
                q1 = q0 + dt*qt0 + hx*qx0 + hy*qy0 +.5*dt**2*qtt0 + dt*
     & ( hx*qtx0+hy*qty0 ) + .5*hx**2*qxx0 + hx*hy*qxy0 + .5*hy**2*
     & qyy0

               ! **NOTE** these are NOT exact for poly degree=(2,1) since r.xxt, r.xyt etc are non-zero
               u(j1,j2,j3,rc)=r1
               u(j1,j2,j3,uc)=u1
               u(j1,j2,j3,vc)=v1
               u(j1,j2,j3,tc)=q1
             else
               ! extrap ghost values
               k1=i1+is1*mm
               k2=i2+is2*mm
               k3=i3+is3*mm

c Extrapolate with a limiter -- check the monotonicity of the solution
c Use a lower order approximation if the solution is not monotone



               do m=0,3
                !  if( mm.le.2 )then
                ! u(j1,j2,j3,m)=3.*u(j1+is1,j2+is2,j3,m)-3.*u(j1+2*is1,j2+2*is2,j3,m)+u(j1+3*is1,j2+3*is2,j3,m)
                !   else
                ! u(j1,j2,j3,m)=2.*u(i1,i2,i3,m)-u(k1,k2,k3,m)
                ! limitedExtrapolation(j1,j2,j3,m)
                ! u(j1,j2,j3,m)=2.*u(i1,i2,i3,m)-u(k1,k2,k3,m)
                u(j1,j2,j3,m)=3.*u(j1+is1,j2+is2,j3,m)-3.*u(j1+2*is1,
     & j2+2*is2,j3,m)+u(j1+3*is1,j2+3*is2,j3,m)

                if( mm.eq.2 )then
                  if( secondGhostLineOption.eq.0 )then
                    ! use default
                  else if( secondGhostLineOption.eq.1 )then
                    u(j1,j2,j3,m)=u(j1+is1,j2+is2,j3,m)
                  end if
                end if
                !   end if
               end do
               ! m=rc
               ! u(j1,j2,j3,m)=3.*u(j1+is1,j2+is2,j3,m)-3.*u(j1+2*is1,j2+2*is2,j3,m)+u(j1+3*is1,j2+3*is2,j3,m)

               ! m=uc
               ! u(j1,j2,j3,m)=3.*u(j1+is1,j2+is2,j3,m)-3.*u(j1+2*is1,j2+2*is2,j3,m)+u(j1+3*is1,j2+3*is2,j3,m)

               ! u.x = -[ v.y + (p.t + vp.y)/(gamma*p) ] 
               ! uh=rxi*dr(axis)*sgn*mm
               ! p0=u(i1,i2,i3,rc)*u(i1,i2,i3,tc)
               ! p2=u2(i1,i2,i3,rc)*u2(i1,i2,i3,tc)
               ! pt0=(p0-p2)/dt
               ! u(j1,j2,j3,uc)=u(k1,k2,k3,uc) - 2.*h*( -pt0/(gamma*p0) )

             end if


              ! nDotuv(mm)=an1*u1+an2*v1 
             end do
             if( axis.eq.0 )then
               rr1 = ur2(i1,i2,i3,rc)
             else
               rr1 = us2(i1,i2,i3,rc)
             end if

             ! try a second order 1-sided for rho only
             ! m=rc
             ! rr1 = (-3.*u(i1,i2,i3,m)+4.*u(i1+is1,i2+is2,i3,m)-u(i1+2*is1,i2+2*is2,i3,m))/(2.*dr(axis))

            else
              ! get r,u,v at ghost using the equation r.r, u.r, v.r

              ! Here are the solution and derivatives for grid function u2 and gridVelocity gv2 at time tm
              ! gv2 and gv are used to compute gtt = (gv-gv2)/dt
              r0  = u2 (i1,i2,i3,rc)
              rx0 = u2 x22(i1,i2,i3,rc)
              ry0 = u2 y22(i1,i2,i3,rc)
              rxx0= u2 xx22(i1,i2,i3,rc)
              rxy0= u2 xy22(i1,i2,i3,rc)
              ryy0= u2 yy22(i1,i2,i3,rc)
              u0  = u2 (i1,i2,i3,uc)
              ux0 = u2 x22(i1,i2,i3,uc)
              uy0 = u2 y22(i1,i2,i3,uc)
              uxx0= u2 xx22(i1,i2,i3,uc)
              uxy0= u2 xy22(i1,i2,i3,uc)
              uyy0= u2 yy22(i1,i2,i3,uc)
              v0  = u2 (i1,i2,i3,vc)
              vx0 = u2 x22(i1,i2,i3,vc)
              vy0 = u2 y22(i1,i2,i3,vc)
              vxx0= u2 xx22(i1,i2,i3,vc)
              vxy0= u2 xy22(i1,i2,i3,vc)
              vyy0= u2 yy22(i1,i2,i3,vc)
              q0  = u2 (i1,i2,i3,tc)
              qx0 = u2 x22(i1,i2,i3,tc)
              qy0 = u2 y22(i1,i2,i3,tc)
              qxx0= u2 xx22(i1,i2,i3,tc)
              qxy0= u2 xy22(i1,i2,i3,tc)
              qyy0= u2 yy22(i1,i2,i3,tc)
              p0 = r0*q0                     ! Rg needed
              px0 =rx0*q0+r0*qx0
              py0 =ry0*q0+r0*qy0
              pxx0=rxx0*q0+rx0*qx0 + rx0*qx0+r0*qxx0
              pxy0=rxy0*q0+ry0*qx0 + rx0*qy0+r0*qxy0
              pyy0=ryy0*q0+ry0*qy0 + ry0*qy0+r0*qyy0
              if( twilightZone.ne.0 )then
                ! evaluate TZ forcing at t
                call ogftaylor(ep,x(i1,i2,i3,0),x(i1,i2,i3,1),z0,tm,nd,
     & fv)
              end if
              if( gridIsMoving.ne.0 )then
                ! -- add moving grid terms ----
                if( twilightZone.eq.0 )then
                  do mm=0,19
                    fv(mm)=0.
                  end do
                end if
                ! *** note: we need gv2 at tm here -- 
                fv( 0)=fv( 0) + gv2(i1,i2,i3,0)*rx0 + gv2(i1,i2,i3,1)*
     & ry0
                fv( 1)=fv( 1) + gv2(i1,i2,i3,0)*ux0 + gv2(i1,i2,i3,1)*
     & uy0
                fv( 2)=fv( 2) + gv2(i1,i2,i3,0)*vx0 + gv2(i1,i2,i3,1)*
     & vy0
                fv( 3)=fv( 3) + gv2(i1,i2,i3,0)*qx0 + gv2(i1,i2,i3,1)*
     & qy0
                fv( 4)=fv( 4) + gv2(i1,i2,i3,0)*px0 + gv2(i1,i2,i3,1)*
     & py0
                ! estimate gtt (we cannot use gtt if we are not on the boundary)
                ! **** this is not correct -- rtx0 etc. are not known yet
                stop 1224
                gttu0 = (gv(i1,i2,i3,0)-gv2(i1,i2,i3,0))/dt
                gttv0 = (gv(i1,i2,i3,1)-gv2(i1,i2,i3,1))/dt
                fv( 5)=fv( 5) + gttu0*rx0 + gttv0*ry0 + gv2(i1,i2,i3,0)
     & *rtx0 + gv2(i1,i2,i3,1)*rty0
                fv( 6)=fv( 6) + gttu0*ux0 + gttv0*uy0 + gv2(i1,i2,i3,0)
     & *utx0 + gv2(i1,i2,i3,1)*uty0
                fv( 7)=fv( 7) + gttu0*vx0 + gttv0*vy0 + gv2(i1,i2,i3,0)
     & *vtx0 + gv2(i1,i2,i3,1)*vty0
                fv( 8)=fv( 8) + gttu0*qx0 + gttv0*qy0 + gv2(i1,i2,i3,0)
     & *qtx0 + gv2(i1,i2,i3,1)*qty0
                fv( 9)=fv( 9) + gttu0*px0 + gttv0*py0 + gv2(i1,i2,i3,0)
     & *ptx0 + gv2(i1,i2,i3,1)*pty0
                ! we need derivatives of the grid velocity:
                gvux0=gv2 x22(i1,i2,i3,0)
                gvuy0=gv2 y22(i1,i2,i3,0)
                gvvx0=gv2 x22(i1,i2,i3,1)
                gvvy0=gv2 y22(i1,i2,i3,1)
                fv(10)=fv(10) + gv2(i1,i2,i3,0)*rxx0 + gv2(i1,i2,i3,1)*
     & rxy0 + gvux0*rx0 + gvvx0*ry0
                fv(11)=fv(11) + gv2(i1,i2,i3,0)*uxx0 + gv2(i1,i2,i3,1)*
     & uxy0 + gvux0*ux0 + gvvx0*uy0
                fv(12)=fv(12) + gv2(i1,i2,i3,0)*vxx0 + gv2(i1,i2,i3,1)*
     & vxy0 + gvux0*vx0 + gvvx0*vy0
                fv(13)=fv(13) + gv2(i1,i2,i3,0)*qxx0 + gv2(i1,i2,i3,1)*
     & qxy0 + gvux0*qx0 + gvvx0*qy0
                fv(14)=fv(14) + gv2(i1,i2,i3,0)*pxx0 + gv2(i1,i2,i3,1)*
     & pxy0 + gvux0*px0 + gvvx0*py0
                fv(15)=fv(15) + gv2(i1,i2,i3,0)*rxy0 + gv2(i1,i2,i3,1)*
     & ryy0 + gvuy0*rx0 + gvvy0*ry0
                fv(16)=fv(16) + gv2(i1,i2,i3,0)*uxy0 + gv2(i1,i2,i3,1)*
     & uyy0 + gvuy0*ux0 + gvvy0*uy0
                fv(17)=fv(17) + gv2(i1,i2,i3,0)*vxy0 + gv2(i1,i2,i3,1)*
     & vyy0 + gvuy0*vx0 + gvvy0*vy0
                fv(18)=fv(18) + gv2(i1,i2,i3,0)*qxy0 + gv2(i1,i2,i3,1)*
     & qyy0 + gvuy0*qx0 + gvvy0*qy0
                fv(19)=fv(19) + gv2(i1,i2,i3,0)*pxy0 + gv2(i1,i2,i3,1)*
     & pyy0 + gvuy0*px0 + gvvy0*py0
              end if
              pt0 = -( u0*px0 + v0*py0 + gamma*p0*(ux0+vy0) -fv(4) )
              ptx0 =-( ux0*px0+vx0*py0 + gamma*px0*(ux0+vy0) + u0*pxx0+
     & v0*pxy0 + gamma*p0*(uxx0+vxy0) -fv(14) )
              pty0 =-( uy0*px0+vy0*py0 + gamma*py0*(ux0+vy0) + u0*pxy0+
     & v0*pyy0 + gamma*p0*(uxy0+vyy0) -fv(19) )
              qt0 = -( u0*qx0 + v0*qy0 + gm1*q0*(ux0+vy0) -fv(3) )
              qtx0 =-( ux0*qx0+vx0*qy0 + gm1*qx0*(ux0+vy0) + u0*qxx0+
     & v0*qxy0 + gm1*q0*(uxx0+vxy0) -fv(13) )
              qty0 =-( uy0*qx0+vy0*qy0 + gm1*qy0*(ux0+vy0) + u0*qxy0+
     & v0*qyy0 + gm1*q0*(uxy0+vyy0) -fv(18) )
              rt0 = -( u0*rx0+v0*ry0 + r0*(ux0+vy0) -fv(0) )
              rtx0= -( ux0*rx0 +rx0*ux0 +vx0*ry0 + rx0*vy0 + u0*rxx0+
     & v0*rxy0 + r0*(uxx0+vxy0) -fv(10) )
              rty0= -( uy0*rx0 +ry0*ux0 +vy0*ry0 + ry0*vy0 + u0*rxy0+
     & v0*ryy0 + r0*(uxy0+vyy0) -fv(15) )
              ut0 = -( u0*ux0 + v0*uy0 + px0/r0 -fv(1) )
              utx0= -( ux0*ux0 +u0*uxx0 + vx0*uy0 + v0*uxy0 + pxx0/r0 -
     &  px0*rx0/(r0**2) -fv(11) )
              uty0= -( uy0*ux0 +u0*uxy0 + vy0*uy0 + v0*uyy0 + pxy0/r0 -
     &  px0*ry0/(r0**2) -fv(16) )
              vt0 = -( u0*vx0 + v0*vy0 + py0/r0 -fv(2) )
              vtx0= -( ux0*vx0 +u0*vxx0 + vx0*vy0 + v0*vxy0 + pxy0/r0 -
     &  py0*rx0/(r0**2) -fv(12) )
              vty0= -( uy0*vx0 +u0*vxy0 + vy0*vy0 + v0*vyy0 + pyy0/r0 -
     &  py0*ry0/(r0**2) -fv(17) )
              rtt0= -( ut0*rx0+vt0*ry0 + rt0*(ux0+vy0) + u0*rtx0+v0*
     & rty0 + r0*(utx0+vty0) -fv(5) )
              utt0= -( ut0*ux0 + vt0*uy0 + ptx0/r0 + u0*utx0 + v0*uty0 
     & - px0*rt0/(r0**2) -fv(6) )
              vtt0= -( ut0*vx0 + vt0*vy0 + pty0/r0 + u0*vtx0 + v0*vty0 
     & - py0*rt0/(r0**2) -fv(7) )
              ptt0= -( ut0*px0 + vt0*py0 + gamma*pt0*(ux0+vy0) + u0*
     & ptx0 + v0*pty0 + gamma*p0*(utx0+vty0) -fv(9) )
              qtt0= -( ut0*qx0 + vt0*qy0 + gm1*qt0*(ux0+vy0) + u0*qtx0 
     & + v0*qty0 + gm1*q0*(utx0+vty0) -fv(8) )

             if( addFouthOrderAD )then
              !include  fourth order artificial diffusion 
              rt0 = rt0 + ad(rc)*(-12.*u2(i1,i2,i3,rc)+4.*(u2(i1+1,i2,
     & i3,rc)+u2(i1-1,i2,i3,rc)+u2(i1,i2+1,i3,rc)+u2(i1,i2-1,i3,rc))-(
     & u2(i1+2,i2,i3,rc)+u2(i1-2,i2,i3,rc)+u2(i1,i2+2,i3,rc)+u2(i1,i2-
     & 2,i3,rc)))
              ut0 = ut0 + ad(uc)*(-12.*u2(i1,i2,i3,uc)+4.*(u2(i1+1,i2,
     & i3,uc)+u2(i1-1,i2,i3,uc)+u2(i1,i2+1,i3,uc)+u2(i1,i2-1,i3,uc))-(
     & u2(i1+2,i2,i3,uc)+u2(i1-2,i2,i3,uc)+u2(i1,i2+2,i3,uc)+u2(i1,i2-
     & 2,i3,uc)))
              vt0 = vt0 + ad(vc)*(-12.*u2(i1,i2,i3,vc)+4.*(u2(i1+1,i2,
     & i3,vc)+u2(i1-1,i2,i3,vc)+u2(i1,i2+1,i3,vc)+u2(i1,i2-1,i3,vc))-(
     & u2(i1+2,i2,i3,vc)+u2(i1-2,i2,i3,vc)+u2(i1,i2+2,i3,vc)+u2(i1,i2-
     & 2,i3,vc)))
              qt0 = qt0 + ad(tc)*(-12.*u2(i1,i2,i3,tc)+4.*(u2(i1+1,i2,
     & i3,tc)+u2(i1-1,i2,i3,tc)+u2(i1,i2+1,i3,tc)+u2(i1,i2-1,i3,tc))-(
     & u2(i1+2,i2,i3,tc)+u2(i1-2,i2,i3,tc)+u2(i1,i2+2,i3,tc)+u2(i1,i2-
     & 2,i3,tc)))
             end if



             rrt = rtx0*xri + rty0*yri     ! (rho.r).t = ...
             urt = utx0*xri + uty0*yri
             vrt = vtx0*xri + vty0*yri
             qrt = qtx0*xri + qty0*yri

             if( axis.eq.0 )then
               rr1=u2r2(i1,i2,i3,rc)+dt*( rrt )   ! rho.r
               ur1=u2r2(i1,i2,i3,uc)+dt*( urt )   ! u.r
               vr1=u2r2(i1,i2,i3,vc)+dt*( vrt )   ! v.r
               qr1=u2r2(i1,i2,i3,tc)+dt*( qrt )   ! T.r
             else
               rr1=u2s2(i1,i2,i3,rc)+dt*( rrt )   ! rho.r
               ur1=u2s2(i1,i2,i3,uc)+dt*( urt )   ! u.r
               vr1=u2s2(i1,i2,i3,vc)+dt*( vrt )   ! v.r
               qr1=u2s2(i1,i2,i3,tc)+dt*( qrt )   ! v.r
             end if




             do mm=1,2
               j1=i1-is1*mm
               j2=i2-is2*mm
               j3=i3-is3*mm
               k1=i1+is1*mm
               k2=i2+is2*mm
               k3=i3+is3*mm

               h=dr(axis)*sgn*mm

                   u(j1,j2,j3,rc)=u(k1,k2,k3,rc) -2.*h*(rr1 + 0.)
                   u(j1,j2,j3,uc)=u(k1,k2,k3,uc) -2.*h*(ur1 + 0.)
                   u(j1,j2,j3,vc)=u(k1,k2,k3,vc) -2.*h*(vr1 + 0.)
                   u(j1,j2,j3,tc)=u(k1,k2,k3,tc) -2.*h*(qr1 + 0.)

               ! add smoothing in the normal direction
               ! if cnSmooth==1 --> D+D-u = 0 : choose cnSmooth=c*h^2 I think
               ! do m=0,ncm1
               !   u(j1,j2,j3,m)=(1.-cnSmooth)*u(j1,j2,j3,m)+cnSmooth*(2.*u(i1,i2,i3,m)-u(k1,k2,k3,m))
               ! end do

             end do
           end if

            if( .false. )then
              ! for testing -- set predicted value to be exact
              do mm=1,2
                j1=i1-is1*mm
                j2=i2-is2*mm
                j3=i3-is3*mm
                do m=0,ncm1
                  u(j1,j2,j3,m)=ogf(ep,x(j1,j2,j3,0),x(j1,j2,j3,1),0.,
     & m,t)
                end do
              end do
            end if


            ! BC: p.n = rho*( ... )
            rho = u(i1,i2,i3,rc)
            tp  = u(i1,i2,i3,tc)
            us0=(u(i1+js1,i2+js2,i3,uc)-u(i1-js1,i2-js2,i3,uc))/(2.*dr(
     & axisp1))
            vs0=(u(i1+js1,i2+js2,i3,vc)-u(i1-js1,i2-js2,i3,vc))/(2.*dr(
     & axisp1))

            pn = -rho*( sxi*u(i1,i2,i3,uc)+syi*u(i1,i2,i3,vc) )*(an1*
     & us0+an2*vs0)
            if( twilightZone.ne.0 )then
              ! evaluate TZ forcing at t
              call ogftaylor(ep,x(i1,i2,i3,0),x(i1,i2,i3,1),z0,t,nd,fv)


              call ogderiv(ep, 1,0,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),z0,
     & t,uc,ute)
              call ogderiv(ep, 1,0,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),z0,
     & t,vc,vte)

              call ogderiv(ep, 0,1,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),z0,
     & t,uc,uxe)
              call ogderiv(ep, 0,1,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),z0,
     & t,vc,vxe)

              call ogderiv(ep, 0,0,1,0, x(i1,i2,i3,0),x(i1,i2,i3,1),z0,
     & t,uc,uye)
              call ogderiv(ep, 0,0,1,0, x(i1,i2,i3,0),x(i1,i2,i3,1),z0,
     & t,vc,vye)

              rxU=rxi*u(i1,i2,i3,uc)+ryi*u(i1,i2,i3,vc)
              ure = xri*uxe+yri*uye ! ur (exact)
              vre = xri*vxe+yri*vye ! ur (exact)
              pn = pn + rho*( an1*(fv(1)-ute-rxU*ure) + an2*(fv(2)-vte-
     & rxU*vre) )

            end if
            if( gridIsMoving.ne.0 )then
              ! write(*,'(" -- add gtt term to pn, gtt=",2f6.3)') gtt(i1,i2,i3,0),gtt(i1,i2,i3,1)
              pn = pn - rho*( an1*gtt(i1,i2,i3,0)+an2*gtt(i1,i2,i3,1) )
     &  + rho*( sxi*gv(i1,i2,i3,0)+syi*gv(i1,i2,i3,1) )*(an1*us0+an2*
     & vs0)
            end if

            if( addFouthOrderAD )then
              !include  fourth order artificial diffusion in the momentum equations
              pn = pn + an1*ad(uc)*( (-12.*u2(i1,i2,i3,uc)+4.*(u2(i1+1,
     & i2,i3,uc)+u2(i1-1,i2,i3,uc)+u2(i1,i2+1,i3,uc)+u2(i1,i2-1,i3,uc)
     & )-(u2(i1+2,i2,i3,uc)+u2(i1-2,i2,i3,uc)+u2(i1,i2+2,i3,uc)+u2(i1,
     & i2-2,i3,uc))) ) + an2*ad(vc)*( (-12.*u2(i1,i2,i3,vc)+4.*(u2(i1+
     & 1,i2,i3,vc)+u2(i1-1,i2,i3,vc)+u2(i1,i2+1,i3,vc)+u2(i1,i2-1,i3,
     & vc))-(u2(i1+2,i2,i3,vc)+u2(i1-2,i2,i3,vc)+u2(i1,i2+2,i3,vc)+u2(
     & i1,i2-2,i3,vc))) )
            end if


            rhos=(u(i1+js1,i2+js2,i3,rc)-u(i1-js1,i2-js2,i3,rc))/(2.*
     & dr(axisp1))
            tps =(u(i1+js1,i2+js2,i3,tc)-u(i1-js1,i2-js2,i3,tc))/(2.*
     & dr(axisp1))
            ps =  rhos*tp + rho*tps

            ! pra(ii) and psa(ii) are really pr and ps for both axis==0 and axis==1
            if( axis.eq.0 )then
              pra(ii) = ( pn - (an1*sxi+an2*syi)*ps )/(an1*rxi+an2*ryi)
              psa(ii) = ps
            else
              pra(ii) = ps
              psa(ii) = ( pn - (an1*sxi+an2*syi)*ps )/(an1*rxi+an2*ryi)
            end if

            ! now get a guess for T(-1) given rhor and pr (from the predictor) --- added 050625
            rhor=rr1
            if( axis.eq.0 )then
              pr=pra(ii)
            else
              pr=psa(ii)
            end if
            rho=u(i1,i2,i3,rc)
            tp=u(i1,i2,i3,tc)
            tpr = (pr-rhor*tp)/rho

            u(i1-  is1,i2-  is2,i3,tc)=u(i1+  is1,i2+  is2,i3,tc) -sgn*
     & 2.*dr(axis)*tpr
            if( secondGhostLineOption.eq.0 )then
              u(i1-2*is1,i2-2*is2,i3,tc)=u(i1+2*is1,i2+2*is2,i3,tc) -
     & sgn*4.*dr(axis)*tpr
            else if( secondGhostLineOption.eq.1 )then
              u(i1-2*is1,i2-2*is2,i3,tc)=u(i1-is1,i2-is2,i3,tc)
            else
              stop 8527
            end if

            if( .true. .and. debug.gt.2. .and. twilightZone.ne.0  )then

              ! check solution
              do m=0,ncm1
                uv(m)=ogf(ep,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,m,t)
              end do
              call ogderiv(ep, 0,1,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),z0,
     & t,rc,rxe)
              call ogderiv(ep, 0,1,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),z0,
     & t,tc,qxe)
              call ogderiv(ep, 0,0,1,0, x(i1,i2,i3,0),x(i1,i2,i3,1),z0,
     & t,rc,rye)
              call ogderiv(ep, 0,0,1,0, x(i1,i2,i3,0),x(i1,i2,i3,1),z0,
     & t,tc,qye)

              rre= xri*rxe+ yri*rye

              ! here is p.n = rho*T.n + rho.n*T  (exact)
              pne = uv(rc)*(an1*qxe+an2*qye) + (an1*rxe+an2*rye)*uv(tc)

              do mm=1,2
               j1=i1-is1*mm
               j2=i2-is2*mm
               j3=i3-is3*mm
               do m=0,ncm1
                 uv(m)=ogf(ep,x(j1,j2,j3,0),x(j1,j2,j3,1),0.,m,t)
               end do

              write(*,'("--> stage I: j1,j2=",2i3," ruv=",3(f8.5,1x)," 
     & err=",3(e8.1,1x)," pn,err=",f10.5,1x,e10.1)') j1,j2,u(j1,j2,j3,
     & rc),u(j1,j2,j3,uc),u(j1,j2,j3,vc),abs(u(j1,j2,j3,rc)-uv(rc)),
     & abs(u(j1,j2,j3,uc)-uv(uc)),abs(u(j1,j2,j3,vc)-uv(vc)),pn,abs(
     & pn-pne)
                !'

c$$$              call ogderiv(ep, 0,2,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),z0,t,rc,rxxe)
c$$$              call ogderiv(ep, 0,0,2,0, x(i1,i2,i3,0),x(i1,i2,i3,1),z0,t,rc,ryye)
c$$$
c$$$              write(*,'("  rx0,ry0=",2e10.2," err=",2e10.2,"  rtx0,try0 =",2e10.2)') rx0,ry0,c$$$                    abs(rx0-rxe),abs(ry0-rye),rtx0,rty0 
c$$$              write(*,'("  rxx0,ryy0=",2e10.2," err=",2e10.2," rr1,rre,u2r2(r),ur2(r)=",4e10.2)') rxx0,ryy0,c$$$                    abs(rxx0-rxxe),abs(ryy0-ryye),rr1,rre,u2r2(i1,i2,i3,rc),ur2(i1,i2,i3,rc)
c$$$
c$$$              ! '"
c$$$
c$$$               do m=0,ncm1
c$$$                 uv(m)=ogf(ep,x(k1,k2,k3,0),x(k1,k2,k3,1),0.,m,t)
c$$$               end do
c$$$              write(*,'("   k1,k2,k3:  ruv=",3(f10.5,1x)," err=",3(e10.3,1x))') u(k1,k2,k3,rc),u(k1,k2,k3,uc),c$$$              u(k1,k2,k3,vc),abs(u(k1,k2,k3,rc)-uv(rc)),abs(u(k1,k2,k3,uc)-uv(uc)),c$$$              abs(u(k1,k2,k3,vc)-uv(vc))
c$$$              ! '
              do m=0,ncm1
                uv(m)=ogf(ep,x(j1,j2,j3,0),x(j1,j2,j3,1),0.,m,tm)
              end do
              write(*,'("  ghost:  t=t-dt: ruv2=",3(f10.5,1x)," err2=",
     & 3(e10.3,1x))') u2(j1,j2,j3,rc),u2(j1,j2,j3,uc),u2(j1,j2,j3,vc),
     & abs(u2(j1,j2,j3,rc)-uv(rc)),abs(u2(j1,j2,j3,uc)-uv(uc)),abs(u2(
     & j1,j2,j3,vc)-uv(vc))
               !'
              end do

            end if



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
              ! give values to pr and ps on the extended boundary (so we can compute prs)
              ii=nr(0,axisp1)
              ! pra(ii-1)=2.*pra(ii)-pra(ii+1)
              ! psa(ii-1)=2.*psa(ii)-psa(ii+1)
              pra(ii-1)=3.*pra(ii)-3.*pra(ii+1)+pra(ii+2)
              psa(ii-1)=3.*psa(ii)-3.*psa(ii+1)+psa(ii+2)
              ii=nr(1,axisp1)
              ! pra(ii+1)=2.*pra(ii)-pra(ii-1)
              ! psa(ii+1)=2.*psa(ii)-psa(ii-1)
              pra(ii+1)=3.*pra(ii)-3.*pra(ii-1)+pra(ii-2)
              psa(ii+1)=3.*psa(ii)-3.*psa(ii-1)+psa(ii-2)
              ! ** assign values on the extended ghost lines
              !
              !         X  X  +
              !         G  G  + ---------------------
              !         G  G  +
              !         G  G  +
              !         G  G  +
              !         G  G  +
              !         G  G  + ---------------------
              !         X  X  +
              ! write(*,'(" end of stage I: nr = ",4i4)') nr(0,0),nr(1,0),nr(0,1),nr(1,1)
              ms1=0
              ms2=0
              ms3=0
              i3=nr(0,2)
              j3=i3
              side2=-1
              do sideb=0,1
                ! used side2 instead if sideb, this is needed to avoid a bug with pgf77 compiled with -O !
                side2=side2+1
                if( axis.eq.0 )then
                  i1=nr(side ,0)
                  i2=nr(side2,1)
                  ms2=1-2*side2
                  ! write(*,'(" end of stage I: sideb,side2,nr(0,1),nr(sideb,1),nr(side2,1)=",10i4)') sideb,side2,nr(0,1),nr(sideb,1),nr(side2,1)
                  ! '
                else
                  i1=nr(side2,0)
                  i2=nr(side ,1)
                  ms1=1-2*side2
                end if
                do mm=1,2   ! two ghost lines
                  if( axis.eq.0 )then
                    j1=i1-is1*mm
                    j2=i2-ms2
                  else
                    j1=i1-ms1
                    j2=i2-is2*mm
                  end if
                  ! write(*,'(" end of stage I: fill extended ghost value i1,i2,ms1,ms2,j1,j2=",6i4)') i1,i2,ms1,ms2,j1,j2
                  ! '
                  if( bc(side2,axisp1).lt.0 )then
                    ! apply periodicity
                    kv(0)=j1
                    kv(1)=j2
                    kv(2)=j3
                    kv(axisp1) = kv(axisp1) + (nr(1,axisp1)-nr(0,
     & axisp1))*(1-2*side2)
                    !write(*,'(" end of stage I: periodic update j1,j2,j3=",3i4," from k1,k2,k3=",3i4)') !       j1,j2,j3,kv(0),kv(1),kv(2)
                    ! '
                    do m=0,ncm1
                      u(j1,j2,j3,m)=u(kv(0),kv(1),kv(2),m)
                    end do
                  else if( .true. )then ! turn this off for now  ***********
                  do m=0,ncm1
                    u(j1,j2,j3,m)=3.*u(j1+  ms1,j2+  ms2,j3+  ms3,m)-
     & 3.*u(j1+2*ms1,j2+2*ms2,j3+2*ms3,m)+u(j1+3*ms1,j2+3*ms2,j3+3*
     & ms3,m)
                  end do
                  end if
                end do
              end do

      if( .false. )then
        stop 1234
      end if


           ! =========== STAGE II: apply BC on n.u ================= 
           !            a1.(uv.rr)+a2.(uv.r) ... =0
         if( .not.getGhostByTaylor )then
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


            rxi = rsxy(i1,i2,i3,0,0)
            ryi = rsxy(i1,i2,i3,0,1)
            sxi = rsxy(i1,i2,i3,1,0)
            syi = rsxy(i1,i2,i3,1,1)

            if( axis.eq.0 )then
              an1=-rxi*sgn
              an2=-ryi*sgn
            else
              an1=-sxi*sgn
              an2=-syi*sgn
            end if
            aNorm=sqrt(max(epsx,an1**2+an2**2))
            an1=an1/aNorm  ! normal (an1,an2)
            an2=an2/aNorm

            tau1=-an2  ! tangent (tau1,tau2)
            tau2= an1

            ajac=rxi*syi-ryi*sxi
            xri= syi/ajac
            yri=-sxi/ajac
            xsi=-ryi/ajac
            ysi= rxi/ajac


            pr = pra(ii)    ! this is really pr
            ps = psa(ii)

            px = rxi*pr+sxi*ps
            py = ryi*pr+syi*ps

            r0 =u(i1,i2,i3,rc)
            tp0=u(i1,i2,i3,tc)
            u0 =u(i1,i2,i3,uc)
            v0 =u(i1,i2,i3,vc)

            nDotGradR=an1*rxi+an2*ryi
            nDotGradS=an1*sxi+an2*syi

            p0=r0*Rg*tp0
            pn = nDotGradR*pr + nDotGradS*ps

	    ! BC for n.u:
            ! div(uv).n  + [ gm1*(p.n)*div(uv) + U.n*p.r ]/(gamma*p)= stuff


            ux0 = ux22(i1,i2,i3,uc)
            ! uy0 = uy22(i1,i2,i3,uc)
            ! vx0 = ux22(i1,i2,i3,vc)
            vy0 = uy22(i1,i2,i3,vc)
            divu=ux0+vy0

            urr0=urr2(i1,i2,i3,uc)
            vrr0=urr2(i1,i2,i3,vc)
            urs0=urs2(i1,i2,i3,uc)
            vrs0=urs2(i1,i2,i3,vc)
            uss0=uss2(i1,i2,i3,uc)
            vss0=uss2(i1,i2,i3,vc)

            rr0=ur2(i1,i2,i3,rc)
            ur0=ur2(i1,i2,i3,uc)
            vr0=ur2(i1,i2,i3,vc)

            rs0=us2(i1,i2,i3,rc)
            us0 = us2(i1,i2,i3,uc)
            vs0 = us2(i1,i2,i3,vc)


            rxri=rxr2(i1,i2,i3)
            ryri=ryr2(i1,i2,i3)
            sxri=sxr2(i1,i2,i3)
            syri=syr2(i1,i2,i3)

            rxsi=rxs2(i1,i2,i3)
            rysi=rys2(i1,i2,i3)
            sxsi=sxs2(i1,i2,i3)
            sysi=sys2(i1,i2,i3)

            divur =  rxi*urr0+ryi*vrr0 + rxri*ur0+ryri*vr0 + sxi*urs0+
     & syi*vrs0 + sxri*us0+syri*vs0

            divus =  rxi*urs0+ryi*vrs0 + rxsi*ur0+rysi*vr0 + sxi*uss0+
     & syi*vss0 + sxsi*us0+sysi*vs0

            ! Here we compute the coefficients of uv.rr and uv.r


            ! evaluate the expression with current values at ghost points
            rxU=rxi*u0+ryi*v0    ! grad(r).uv = U
            sxU=sxi*u0+syi*v0    ! grad(s).uv = V

            rxUr = rxi*ur0+ryi*vr0 + rxri*u0+ryri*v0          ! (grad(r).u).n
            sxUr = sxi*ur0+syi*vr0 + sxri*u0+syri*v0          ! (grad(s).u).n

            rxUs=rxi*us0+ryi*vs0 + rxsi*u0+rysi*v0
            sxUs=sxi*us0+syi*vs0 + sxsi*u0+sysi*v0


              if( twilightZone.ne.0  )then
                 ! add forcing for sxUt and pst:
                if( axis.eq.0 )then
                  call ogftaylor(ep,x(i1,i2,i3,0)+xsi*dsEps,x(i1,i2,i3,
     & 1)+ysi*dsEps,z0,t,nd,fv)
                  fps=fv(4)
                  call ogftaylor(ep,x(i1,i2,i3,0)-xsi*dsEps,x(i1,i2,i3,
     & 1)-ysi*dsEps,z0,t,nd,fv)
                  fps=(fps-fv(4))/(2.*dsEps) ! fp.s
                else
                  call ogftaylor(ep,x(i1,i2,i3,0)+xri*drEps,x(i1,i2,i3,
     & 1)+yri*drEps,z0,t,nd,fv)
                  fpr=fv(4)
                  call ogftaylor(ep,x(i1,i2,i3,0)-xri*drEps,x(i1,i2,i3,
     & 1)-yri*drEps,z0,t,nd,fv)
                  fpr=(fpr-fv(4))/(2.*drEps) ! fp.r
                end if
                call ogderiv(ep, 0,1,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,rc,rxe)
                call ogderiv(ep, 0,1,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,uc,uxe)
                call ogderiv(ep, 0,1,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,vc,vxe)
                call ogderiv(ep, 0,0,1,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,rc,rye)
                call ogderiv(ep, 0,0,1,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,uc,uye)
                call ogderiv(ep, 0,0,1,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,vc,vye)
                ! rho.r, u.r and v.r (exact)
                rre= xri*rxe+yri*rye
                ure= xri*uxe+yri*uye
                vre= xri*vxe+yri*vye
                rse= xsi*rxe+ysi*rye
                use= xsi*uxe+ysi*uye
                vse= xsi*vxe+ysi*vye
                ! dtEps=dt*.001
                call ogftaylor(ep,x(i1,i2,i3,0),x(i1,i2,i3,1),z0,t+
     & dtEps,nd,fv)
                ! I think these need to be corrected these for moving grids !*******************
                fut=fv(1)
                fvt=fv(2)
                call ogftaylor(ep,x(i1,i2,i3,0),x(i1,i2,i3,1),z0,t-
     & dtEps,nd,fv)
                fut = (fut-fv(1))/(2.*dtEps) ! fu.t
                fvt = (fvt-fv(2))/(2.*dtEps) ! fv.t
                call ogderiv(ep, 0,0,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,rc,re)
                call ogderiv(ep, 0,0,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,tc,qe)
                pe=re*Rg*qe
                call ogderiv(ep, 1,0,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,rc,rte)
                call ogderiv(ep, 1,0,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,tc,qte)
                call ogderiv(ep, 0,1,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,rc,rxe)
                call ogderiv(ep, 0,1,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,tc,qxe)
                call ogderiv(ep, 0,0,1,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,rc,rye)
                call ogderiv(ep, 0,0,1,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,tc,qye)
                call ogderiv(ep, 1,1,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,rc,rxte)
                call ogderiv(ep, 1,1,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,tc,qxte)
                call ogderiv(ep, 1,0,1,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,rc,ryte)
                call ogderiv(ep, 1,0,1,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,tc,qyte)
                call ogderiv(ep, 0,2,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,tc,qxxe)
                call ogderiv(ep, 0,1,1,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,tc,qxye)
                call ogderiv(ep, 0,0,2,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,tc,qyye)
                call ogderiv(ep, 0,2,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,rc,rxxe)
                call ogderiv(ep, 0,1,1,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,rc,rxye)
                call ogderiv(ep, 0,0,2,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,rc,ryye)
                ! we need to add rho*d/dt( V*(n.uv).s ) = rho*( V.t(n.u).s + V*( n.(u.st)) )
                call ogderiv(ep, 2,0,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,uc,utte)
                call ogderiv(ep, 2,0,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,vc,vtte)
                call ogderiv(ep, 1,0,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,uc,ute)
                call ogderiv(ep, 1,0,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,vc,vte)
                call ogderiv(ep, 1,1,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,uc,uxte)
                call ogderiv(ep, 1,1,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,vc,vxte)
                call ogderiv(ep, 1,0,1,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,uc,uyte)
                call ogderiv(ep, 1,0,1,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,vc,vyte)
                if( axis.eq.0 )then
                  call ogftaylor(ep,x(i1,i2,i3,0)-2.*xri*drEps,x(i1,i2,
     & i3,1)-2.*yri*drEps,z0,t,nd,fv)
                  fpr1=fv(4)
                  call ogftaylor(ep,x(i1,i2,i3,0)-   xri*drEps,x(i1,i2,
     & i3,1)-   yri*drEps,z0,t,nd,fv)
                  fpr2=fv(4)
                  call ogftaylor(ep,x(i1,i2,i3,0)+   xri*drEps,x(i1,i2,
     & i3,1)+   yri*drEps,z0,t,nd,fv)
                  fpr3=fv(4)
                  call ogftaylor(ep,x(i1,i2,i3,0)+2.*xri*drEps,x(i1,i2,
     & i3,1)+2.*yri*drEps,z0,t,nd,fv)
                  fpr4=fv(4)
                  fpr=(-fpr4+8.*(fpr3-fpr2)+fpr1)/(12.*drEps) ! fp.r
                else
                  call ogftaylor(ep,x(i1,i2,i3,0)-2.*xsi*dsEps,x(i1,i2,
     & i3,1)-2.*ysi*dsEps,z0,t,nd,fv)
                  fps1=fv(4)
                  call ogftaylor(ep,x(i1,i2,i3,0)-   xsi*dsEps,x(i1,i2,
     & i3,1)-   ysi*dsEps,z0,t,nd,fv)
                  fps2=fv(4)
                  call ogftaylor(ep,x(i1,i2,i3,0)+   xsi*dsEps,x(i1,i2,
     & i3,1)+   ysi*dsEps,z0,t,nd,fv)
                  fps3=fv(4)
                  call ogftaylor(ep,x(i1,i2,i3,0)+2.*xsi*dsEps,x(i1,i2,
     & i3,1)+2.*ysi*dsEps,z0,t,nd,fv)
                  fps4=fv(4)
                  fps=(-fps4+8.*(fps3-fps2)+fps1)/(12.*dsEps) ! fp.s
                end if
                ajacr=rxri*syi+rxi*syri - (ryri*sxi + ryi*sxri )
                xrri = (syri/ajac - syi*(ajacr)/ajac**2)
                yrri =-(sxri/ajac - sxi*(ajacr)/ajac**2)
                qre  = xri*qxe+yri*qye
                qxre = xri*qxxe+yri*qxye
                qyre = xri*qxye+yri*qyye
                qrre = xrri*qxe+xri*qxre + yrri*qye + yri*qyre
                rre  = xri*rxe+yri*rye
                rxre = xri*rxxe+yri*rxye
                ryre = xri*rxye+yri*ryye
                rrre = xrri*rxe+xri*rxre + yrri*rye + yri*ryre
                pre = re*qre+rre*qe
                prre = re*qrre + 2.*rre*qre + rrre*qe
                ! xsi=-ryi/ajac
                ! ysi= rxi/ajac
                ajacs=rxsi*syi+rxi*sysi - (rysi*sxi + ryi*sxsi )
                xssi =-(rysi/ajac - ryi*(ajacs)/ajac**2)
                yssi = (rxsi/ajac - rxi*(ajacs)/ajac**2)
                qse  = xsi*qxe+ysi*qye
                qxse = xsi*qxxe+ysi*qxye
                qyse = xsi*qxye+ysi*qyye
                qsse = xssi*qxe+xsi*qxse + yssi*qye + ysi*qyse
                rse  = xsi*rxe+ysi*rye
                rxse = xsi*rxxe+ysi*rxye
                ryse = xsi*rxye+ysi*ryye
                rsse = xssi*rxe+xsi*rxse + yssi*rye + ysi*ryse
                pse = re*qse+rse*qe
                psse = re*qsse + 2.*rse*qse + rsse*qe
                pxte = rxte*qe + rxe*qte + rte*qxe + re*qxte
                pyte = ryte*qe + rye*qte + rte*qye + re*qyte
                prte= xri*pxte + yri*pyte
                pste= xsi*pxte + ysi*pyte
                call ogftaylor(ep,x(i1,i2,i3,0),x(i1,i2,i3,1),z0,t,nd,
     & fv)
                if( axis.eq.0 )then
                  fr0=fv(0) - rxU*rre
                  fu0=fv(1) - rxU*ure
                  fv0=fv(2) - rxU*vre
                  fpr = fpr - rxU*prre
                else
                  fr0=fv(0) - sxU*rse
                  fu0=fv(1) - sxU*use
                  fv0=fv(2) - sxU*vse
                  fps = fps - sxU*psse
                end if
                urte=xri*uxte+yri*uyte
                vrte=xri*vxte+yri*vyte
                uste=xsi*uxte+ysi*uyte
                vste=xsi*vxte+ysi*vyte
              else  ! no TZ
                fps=0.
                fpr=0.
                fu0=0.
                fv0=0.
                fut=0.
                fvt=0.
              end if


            if( gridIsMoving.ne.0 )then
              gtu0=gv(i1,i2,i3,0)
              gtv0=gv(i1,i2,i3,1)
              gtru0=gvr2(i1,i2,i3,0)
              gtrv0=gvr2(i1,i2,i3,1)
              gtsu0=gvs2(i1,i2,i3,0)
              gtsv0=gvs2(i1,i2,i3,1)

              gttu0 = gtt(i1,i2,i3,0)
              gttv0 = gtt(i1,i2,i3,1)
              gtttu0=gtt(i1,i2,i3,nd)
              gtttv0=gtt(i1,i2,i3,nd+1)
            end if

            ! ..................................................
            if( axis.eq.0 )then

                  aurr=nDotGradR*rxi   ! coefficients of uv.rr
                  avrr=nDotGradR*ryi
                  aur=nDotGradR*rxri + (gamma*nDotGradR*pr -nDotGradS*
     & ps)*rxi/(gamma*p0) ! coefficients of uv.r
                  avr=nDotGradR*ryri + (gamma*nDotGradR*pr -nDotGradS*
     & ps)*ryi/(gamma*p0)
                if( gridIsMoving.ne.0 )then
                   rxUr = rxUr - (rxi*gtru0+ryi*gtrv0 + rxri*gtu0+ryri*
     & gtv0)
                   sxUr = sxUr - (sxi*gtru0+syi*gtrv0 + sxri*gtu0+syri*
     & gtv0)
                end if
                 ut0 = sxU*us0 + px/r0 -fu0
                 vt0 = sxU*vs0 + py/r0 -fv0
                ! sxUt = d/dt( sxi*(u-gut) + syi*(v-gvt) )
                 sxUt=-( sxi*ut0 + syi*vt0 )
                 if( gridIsMoving.ne.0 )then
                   sxUt= sxUt + ( sxi*gtt(i1,i2,i3,0)+syi*gtt(i1,i2,i3,
     & 1) )
                   sxUs= sxUs -(sxi*gtsu0+syi*gtsv0 + sxsi*gtu0+sysi*
     & gtv0)
                 end if
                 ! compute n.s 
                 ank = (rxs2(i1,i2,i3)*ryi-rys2(i1,i2,i3)*rxi)/aNorm**3
                 an1s= sgn*ryi*ank
                 an2s=-sgn*rxi*ank
                 ! need ghost values for pra, psa here:
                 prs = (pra(ii+1)-pra(ii-1))/(2.*dr(axisp1))
                 pss=(psa(ii+1)-psa(ii-1))/(2.*dr(axisp1))
                 pst= fps -( sxUs*ps+sxU*pss+gamma*ps*divu+gamma*p0*
     & divus )
                ! term1 = rho* d/dt( V n.uv.s ) = rho*d/dt( V[ (n.uv).s - (n.s).uv ] )
                !        =  rho*{ V.t*[ (n.gt).s - (n.s).uv ] + V*[ (n.gtt).s - (n.s).(uv.t) ] }
                 term1= -r0*(  sxUt*(an1s*u0 +an2s*v0 ) +sxU *(an1s*
     & ut0+an2s*vt0) )
                 term2=0.
                 if( gridIsMoving.ne.0 )then
                   ! we need some terms here:
                   gttsu0 = (gtt(i1,i2+1,i3,0)-gtt(i1,i2-1,i3,0))/(2.*
     & dr(1))
                   gttsv0 = (gtt(i1,i2+1,i3,1)-gtt(i1,i2-1,i3,1))/(2.*
     & dr(1))
                   term1 = term1 + r0*( sxUt*(an1s*gu0+an2s*gv0 + an1*
     & gus0+an2*gvs0) +sxU*( an1s*gttu0+an2s*gttv0 + an1*gttsu0+an2*
     & gttsv0) )
                   term2 = r0*(an1*gtttu0+an2*gtttv0)
                 end if
                if( twilightZone.ne.0  )then
                  ! we need to add rho*d/dt( V*(n.uv).s ) = rho*( V.t(n.u).s + V*( n.(u.st)) )
                  ! replace term2
                  term2 = r0*(an1*utte + an2*vtte)
                    term1 = term1 + r0*( sxUt*(an1s*u0+an2s*v0 + an1*
     & us0+an2*vs0) + sxU*(an1s*ute+an2s*vte + an1*uste+an2*vste) )
                    ! add rho*d/dt( U n.ur )
                    rxUt=rxi*ute+ryi*vte
                    term1=term1 + r0*( rxUt*(an1*ure +an2*vre) + rxU *(
     & an1*urte+an2*vrte) )
                end if
                  Lnu = nDotGradR*( gamma*p0*divur  + gm1*pr*divu + 
     & rxUr*pr -(nDotGradS/nDotGradR)*ps*divu  +( sxU*prs + sxUr*ps ) 
     & - fpr ) - nDotGradS*pst - (pn/r0)*(sxU*rs0-fr0) + r0*(an1*fut+
     & an2*fvt) -term1 -term2
                  Lnu = Lnu/(gamma*p0)
                  b1 = aurr*urr0+avrr*vrr0 + aur*ur0 + avr*vr0  - Lnu

            else if ( axis.eq.1 )then

                  aurr=nDotGradS*sxi   ! coefficients of uv.ss
                  avrr=nDotGradS*syi
                  aur=nDotGradS*sxsi + (gamma*nDotGradS*ps -nDotGradR*
     & pr)*sxi/(gamma*p0) ! coefficients of uv.s
                  avr=nDotGradS*sysi + (gamma*nDotGradS*ps -nDotGradR*
     & pr)*syi/(gamma*p0)
                if( gridIsMoving.ne.0 )then
                   rxUs = rxUs - (rxi*gtsu0+ryi*gtsv0 + rxsi*gtu0+rysi*
     & gtv0)
                   sxUs = sxUs - (sxi*gtsu0+syi*gtsv0 + sxsi*gtu0+sysi*
     & gtv0)
                end if
                 ut0 = rxU*ur0 + px/r0 -fu0
                 vt0 = rxU*vr0 + py/r0 -fv0
                ! sxUt = d/dt( sxi*(u-gut) + syi*(v-gvt) )
                 rxUt=-( rxi*ut0 + ryi*vt0 )
                 if( gridIsMoving.ne.0 )then
                   rxUt= rxUt + ( rxi*gtt(i1,i2,i3,0)+ryi*gtt(i1,i2,i3,
     & 1) )
                   rxUr= rxUr -(rxi*gtru0+ryi*gtrv0 + rxri*gtu0+ryri*
     & gtv0)
                 end if
                 ! compute n.r
                 ank = (sxr2(i1,i2,i3)*syi-syr2(i1,i2,i3)*sxi)/aNorm**3
                 an1r= sgn*syi*ank
                 an2r=-sgn*sxi*ank
                 ! need ghost values for pra, psa here:
                 prs = (psa(ii+1)-psa(ii-1))/(2.*dr(axisp1))
                 prr=(pra(ii+1)-pra(ii-1))/(2.*dr(axisp1))
                 prt= fpr -( rxUr*pr+rxU*prr+gamma*pr*divu+gamma*p0*
     & divur )
                ! term1 = rho* d/dt( V n.uv.s ) = rho*d/dt( V[ (n.uv).s - (n.s).uv ] )
                !        =  rho*{ V.t*[ (n.gt).s - (n.s).uv ] + V*[ (n.gtt).s - (n.s).(uv.t) ] }
                 term1= -r0*(  rxUt*(an1r*u0 +an2r*v0 ) +rxU *(an1r*
     & ut0+an2r*vt0) )
                 term2=0.
                 if( gridIsMoving.ne.0 )then
                   ! we need some terms here:
                   gttru0 = (gtt(i1+1,i2,i3,0)-gtt(i1-1,i2,i3,0))/(2.*
     & dr(0))
                   gttrv0 = (gtt(i1+1,i2,i3,1)-gtt(i1-1,i2,i3,1))/(2.*
     & dr(0))
                   term1 = term1 + r0*( rxUt*(an1r*gu0  +an2r*gv0   + 
     & an1*gur0  +an2*gvr0) +rxU*( an1r*gttu0+an2r*gttv0 + an1*gttru0+
     & an2*gttrv0) )
                   term2 = r0*(an1*gtttu0+an2*gtttv0)
                 end if
                if( twilightZone.ne.0  )then
                  ! we need to add rho*d/dt( V*(n.uv).s ) = rho*( V.t(n.u).s + V*( n.(u.st)) )
                  ! replace term2
                  term2 = r0*(an1*utte + an2*vtte)
                    term1 = term1 + r0*( rxUt*(an1r*u0+an2r*v0 + an1*
     & ur0+an2*vr0) + rxU*(an1r*ute+an2r*vte + an1*urte+an2*vrte) )
                    ! add rho*d/dt( U n.ur )
                    sxUt=sxi*ute+syi*vte
                    term1=term1 + r0*( sxUt*(an1*use +an2*vse) + sxU *(
     & an1*uste+an2*vste) )
                end if
                  Lnu = nDotGradS*( gamma*p0*divus  + gm1*ps*divu + 
     & sxUs*ps -(nDotGradR/nDotGradS)*pr*divu  +( rxU*prs + rxUs*pr ) 
     & - fps ) - nDotGradR*prt - (pn/r0)*(rxU*rr0-fr0) + r0*(an1*fut+
     & an2*fvt) -term1 -term2
                  Lnu = Lnu/(gamma*p0)
                  b1 = aurr*uss0+avrr*vss0 + aur*us0 + avr*vs0  - Lnu

            end if

            if( .true. .and. debug.gt.2 )then
              write(*,'(" Stage II i=",i3,i3," Lnu=",e12.4)') i1,i2,Lnu
              write(*,'("   nDotGradr,nDotGrads,an1s,an2s=",10(f6.2,1x)
     & )') nDotGradr,nDotGrads,an1s,an2s
              write(*,'("   divu,divur,pr,ps,prs,pn,rs0,rxU,sxU=",10(
     & f6.2,1x))') divu,divur,pr,ps,prs,pn,rs0,rxU,sxU
              write(*,'("   urr,urs,uss,ur,us,vrr,vrs,vss,vr,vs=",10(
     & f6.2,1x))') urr0,urs0,uss0,ur0,us0,vrr0,vrs0,vss0,vr0,vs0
              write(*,'("   aurr,avrr,aur,avr,tau1,tau2,term1,term2=",
     & 10(e9.2,1x))') aurr,avrr,aur,avr,tau1,tau2,term1,term2
              ! ' 

              call ogderiv(ep, 1,0,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),z0,
     & t,vc,vte)
              write(*,'("   sxUt=",f9.3," err=",e10.1)') sxUt,abs(sxUt-
     & (sxi*ute+syi*vte))
              write(*,'("   rxUr=",f9.3," err=",e10.1)') rxUr,abs(rxUr-
     & (rxi*ure+ryi*vre+rxri*u0+ryri*v0))
              write(*,'("   sxUr=",f9.3," err=",e10.1)') sxUr,abs(sxUr-
     & (sxi*ure+syi*vre+sxri*u0+syri*v0))

              pnte = an1*pxte+an2*pyte
              pnt = (pn/r0)*(fr0-sxU*rs0-r0*divu) + r0*(an1*fut+an2*
     & fvt) -term1 - term2


              ! pre=  xri*pxe + yri*pye
              pxxe = re*qxxe + 2.*rxe*qxe + rxxe*qe
              pxye = re*qxye + rxe*qye + rye*qxe + rxye*qe
              pyye = re*qyye + 2.*rye*qye + ryye*qe
              prse=  xri*( xsi*pxxe + ysi*pxye )+ yri*( xsi*pxye + ysi*
     & pyye )

              write(*,'("   pnt,pnte=",2e9.2," err=",e8.2," prs,err=",
     & 2e9.2)') pnt,pnte,abs(pnt-pnte),prs,abs(prs-prse)
              ! '
              if( axis.eq.0 )then
                ! Here we assume that the predicted values are good -- needed for divu and divur
                ! prt = -( rxU*prre + rxUr*pr + sxU*prs + sxUr*ps + gamma*p0*divur + gamma*pr*divu - fpr )
                call ogderiv(ep, 0,2,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,uc,uxxe)
                call ogderiv(ep, 0,1,1,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,uc,uxye)
                call ogderiv(ep, 0,0,2,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,vc,vyye)
                call ogderiv(ep, 0,1,1,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,vc,vxye)
                uxre = xri*uxxe+yri*uxye  ! only valid for cartesian or rotated cartesian
                vyre = xri*vxye+yri*vyye
                divure = uxre+vyre

                prt = -( rxUr*pr + sxU*prs + sxUr*ps + gamma*p0*divur +
     &  gamma*pr*divu - fpr )

                 write(*,'("   prt,err=",2e9.2,",  pr,err=",2e9.2,",  
     & ps,err=",2e9.2," divur,err=",2e9.2)') prt,abs(prt-prte),pr,abs(
     & pr-pre),ps,abs(ps-pse),divur,abs(divur-divure)
                 ! '
              end if
            end if


            ! solve:
            !     aurr*uv.rr + aur*uv.r  = b1 = aurr*uvOld.rr + aur*uvOld.r - Lnu(uOld)
            !     tau.(u(-1),v(-1))      = b2 = tau.(uOld(-1),vOld(-1))
            !

      !    aurr=nDotGradR*rxi   ! coefficients of uv.rr 
      !    avrr=nDotGradR*ryi
      !    aur=nDotGradR*rxri + (gamma*nDotGradR*pr -nDotGradS*ps)*rxi/(gamma*p0) ! coefficients of uv.r
      !    avr=nDotGradR*ryri + (gamma*nDotGradR*pr -nDotGradS*ps)*ryi/(gamma*p0)

            if( .false. )then
              ! simplify the BC for testing -- most simple
              if( axis.eq.0 )then
                aurr=1.
                avrr=0.
                aur=1.
                avr=0.
                call ogderiv(ep, 0,2,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,uc,uxxe)
                urre=xri**2*uxxe  ! only for rectangular
                b1 = aurr*urre + aur*ure ! aurr*urre + aur*ure
              else

                aurr=0.
                avrr=1.
                aur=0.
                avr=1.
                call ogderiv(ep, 0,0,2,0, x(i1,i2,i3,0),x(i1,i2,i3,1),
     & z0,t,vc,vyye)
                vsse=ysi**2*vyye  ! only for rectangular
                b1 = avrr*vsse + avr*vse

              end if
            else if( .false. )then

              ! simplify the BC for testing -- less simple

      !    Lnu = nDotGradR*( gamma*p0*divur  !                + gm1*pr*divu + rxUr*pr -(nDotGradS/nDotGradR)*ps*divu  !                +( sxU*prs + sxUr*ps ) - fpr !                     ) !               - nDotGradS*pst - (pn/r0)*(sxU*rs0-fr0) + r0*(an1*fut+an2*fvt) -term1 -term2 

              ! try:
              !  nDotGradR*( gamma*p0*divur +gamma*pr*divu + rxUr*pr ) = nDotGradR*( fpr - [ rxU*prre + (V*ps).r ] )

              aurr=nDotGradR*rxi
              avrr=nDotGradR*ryi
              aur=nDotGradR*rxri + (gamma*nDotGradR*pr -nDotGradS*ps)*
     & rxi/(gamma*p0) !  rxi/(gamma*p0)
              avr=nDotGradR*ryri + (gamma*nDotGradR*pr -nDotGradS*ps)*
     & ryi/(gamma*p0)
              call ogderiv(ep, 0,2,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),z0,
     & t,uc,uxxe)
              urre=xri**2*uxxe  ! only for rectangular
              call ogderiv(ep, 0,1,1,0, x(i1,i2,i3,0),x(i1,i2,i3,1),z0,
     & t,uc,vxye)
              vrse=xri*ysi*vxye ! only for rectangular

              call ogderiv(ep, 0,1,1,0, x(i1,i2,i3,0),x(i1,i2,i3,1),z0,
     & t,rc,rxye)
              rrse=xri*ysi*rxye ! only for rectangular
              call ogderiv(ep, 0,1,1,0, x(i1,i2,i3,0),x(i1,i2,i3,1),z0,
     & t,tc,qxye)
              qrse=xri*ysi*qxye ! only for rectangular
              prse=re*qrse + rre*qse + rse*qre + rrse*qe

              b1 = nDotGradR*rxi*urre + (gamma*nDotGradR*pr -nDotGradS*
     & ps)*rxi/(gamma*pe)*ure

              ! this next term appears in divur
              b1 = b1 + nDotGradR*(syi*vrs0)   - nDotGradR*(syi*vrse)
              b1 = b1 - (pn/r0)*(sxU*rs0)  + (pn/r0)*(sxU*rse)
              ! b1 = b1 +( sxU*prs ) - ( sxU*prse )
              ! b1 = b1 +( sxUr*ps ) - ( vre*pse )
              ! b1 = b1 +( sxUr*pse ) - ( vre*pse )
              ! toruble: b1 = b1 +( vre*ps ) - ( vre*pse )
              b1 = b1 +( sxU*prs + sxUr*ps ) - ( sxU*prse + sxUr*pse )
            end if

            do mm=1,2
              j1=i1-is1*mm
              j2=i2-is2*mm
              j3=i3-is3*mm
              k1=i1+is1*mm
              k2=i2+is2*mm
              k3=i3+is3*mm

              !  b1 = aurr*urr0+avrr*vrr0 + aur*ur0 + avr*vr0  - Lnu
              !  b1 = aurr*uss0+avrr*vss0 + aur*us0 + avr*vs0  - Lnu

              ! Solve 
              !   [ a11 a12 ][ u(-1) ] = [ f1 ]
              !   [ a21 a22 ][ v(-1) ] = [ f2 ]

              h = dr(axis)*mm*sgn

              a11 = aurr/h**2 - aur/(2.*h)
              a12 = avrr/h**2 - avr/(2.*h)
              a21=tau1
              a22=tau2
              f1 = b1 - aurr*(-2.*u(i1,i2,i3,uc)+u(k1,k2,k3,uc))/h**2 -
     &  avrr*(-2.*u(i1,i2,i3,vc)+u(k1,k2,k3,vc))/h**2 - aur *( u(k1,
     & k2,k3,uc) )/(2.*h)- avr *( u(k1,k2,k3,vc) )/(2.*h)
              f2=tau1*u(j1,j2,j3,uc)+tau2*u(j1,j2,j3,vc)

              det=a11*a22-a12*a21
                  u(j1,j2,j3,uc)=(a22*f1-a12*f2)/det
              u(j1,j2,j3,vc)=(a11*f2-a21*f1)/det
            end do


            if( (.false. .or. debug.gt.2) .and. gridIsMoving.ne.0 )then
              write(*,'(" slip-deriv** i=",i3,i3," u,gv==",10(f6.4,1x))
     & ') i1,i2,u(i1,i2,i3,uc),gv(i1,i2,i3,0)
              write(*,'(" slip-deriv: i=",i3,i3," pn,pr,ps,rhor,gv,
     & gtt=",10(e9.2,1x))') i1,i2,pn,pr,ps,rhor,gv(i1,i2,i3,0),gv(i1,
     & i2,i3,1),gtt(i1,i2,i3,0),gtt(i1,i2,i3,1),gtt(i1,i2,i3,2),gtt(
     & i1,i2,i3,3)
              ! '
            end if
            if( .true. .and. debug.gt.2 .and. twilightZone.ne.0  )then

              ! check solution
              do mm=1,2
               j1=i1-is1*mm
               j2=i2-is2*mm
               j3=i3-is3*mm
               do m=0,ncm1
                 uv(m)=ogf(ep,x(j1,j2,j3,0),x(j1,j2,j3,1),0.,m,t)
               end do


              write(*,'("--> stage II: j1,j2=",2i3," uv=",2(f10.5,1x),
     & " err=",2(e10.1,1x)," ps,pse=",2f10.5," err=",e10.1)') j1,j2,u(
     & j1,j2,j3,uc),u(j1,j2,j3,vc),abs(u(j1,j2,j3,uc)-uv(uc)),abs(u(
     & j1,j2,j3,vc)-uv(vc)),ps,pse,pse-ps
            ! '
              end do
            end if


            ii=ii+1
            end if
           end do
           end do
           end do

           ! ** assign values on the extended ghost lines
              ! ** assign values on the extended ghost lines
              !
              !         X  X  +
              !         G  G  + ---------------------
              !         G  G  +
              !         G  G  +
              !         G  G  +
              !         G  G  +
              !         G  G  + ---------------------
              !         X  X  +
              ! write(*,'(" end of stage I: nr = ",4i4)') nr(0,0),nr(1,0),nr(0,1),nr(1,1)
              ms1=0
              ms2=0
              ms3=0
              i3=nr(0,2)
              j3=i3
              side2=-1
              do sideb=0,1
                ! used side2 instead if sideb, this is needed to avoid a bug with pgf77 compiled with -O !
                side2=side2+1
                if( axis.eq.0 )then
                  i1=nr(side ,0)
                  i2=nr(side2,1)
                  ms2=1-2*side2
                  ! write(*,'(" end of stage I: sideb,side2,nr(0,1),nr(sideb,1),nr(side2,1)=",10i4)') sideb,side2,nr(0,1),nr(sideb,1),nr(side2,1)
                  ! '
                else
                  i1=nr(side2,0)
                  i2=nr(side ,1)
                  ms1=1-2*side2
                end if
                do mm=1,2   ! two ghost lines
                  if( axis.eq.0 )then
                    j1=i1-is1*mm
                    j2=i2-ms2
                  else
                    j1=i1-ms1
                    j2=i2-is2*mm
                  end if
                  ! write(*,'(" end of stage I: fill extended ghost value i1,i2,ms1,ms2,j1,j2=",6i4)') i1,i2,ms1,ms2,j1,j2
                  ! '
                  if( bc(side2,axisp1).lt.0 )then
                    ! apply periodicity
                    kv(0)=j1
                    kv(1)=j2
                    kv(2)=j3
                    kv(axisp1) = kv(axisp1) + (nr(1,axisp1)-nr(0,
     & axisp1))*(1-2*side2)
                    !write(*,'(" end of stage I: periodic update j1,j2,j3=",3i4," from k1,k2,k3=",3i4)') !       j1,j2,j3,kv(0),kv(1),kv(2)
                    ! '
                    do m=0,ncm1
                      u(j1,j2,j3,m)=u(kv(0),kv(1),kv(2),m)
                    end do
                  else if( .true. )then ! turn this off for now  ***********
                  do m=0,ncm1
                    u(j1,j2,j3,m)=3.*u(j1+  ms1,j2+  ms2,j3+  ms3,m)-
     & 3.*u(j1+2*ms1,j2+2*ms2,j3+2*ms3,m)+u(j1+3*ms1,j2+3*ms2,j3+3*
     & ms3,m)
                  end do
                  end if
                end do
              end do
         end if ! if( .not.getGhostByTaylor

      if( .false. )then
        stop 4321
      end if

           ! ======== STAGE III: corrector step: 
           !           Update rho.r, tau.(uv.r) from (rho.r).t= trapezodial rule in time
           !           Compute T(-1) from rho.r and p.r 
           ! ** nitStage3=1 ! 3  ! iterate on stage III
           ! ** do it3=1,nitStage3

         if( .not.getGhostByTaylor )then

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

            tau1=-an2  ! tangent (tau1,tau2)
            tau2= an1

            ajac=rxi*syi-ryi*sxi
            xri= syi/ajac
            yri=-sxi/ajac
            xsi=-ryi/ajac
            ysi= rxi/ajac

c$$$             rxi = rsxy(i1,i2,i3,0,0)
c$$$             ryi = rsxy(i1,i2,i3,0,1)
c$$$             sxi = rsxy(i1,i2,i3,1,0)
c$$$             syi = rsxy(i1,i2,i3,1,1)
c$$$ 
c$$$             if( axis.eq.0 )then
c$$$               an1=-rxi*sgn  
c$$$               an2=-ryi*sgn
c$$$             else
c$$$               an1=-sxi*sgn  
c$$$               an2=-syi*sgn
c$$$             end if
c$$$             aNorm=sqrt(max(epsx,an1**2+an2**2))
c$$$             an1=an1/aNorm  ! normal (an1,an2)
c$$$             an2=an2/aNorm
c$$$ 
c$$$             tau1=-an2  ! tangent (tau1,tau2)
c$$$             tau2= an1
c$$$ 
c$$$             ajac=rxi*syi-ryi*sxi
c$$$             xri= syi/ajac
c$$$             yri=-sxi/ajac
c$$$             xsi=-ryi/ajac
c$$$             ysi= rxi/ajac

              ! Here are the solution and derivatives for grid function u2 and gridVelocity gv2 at time tm
              ! gv2 and gv are used to compute gtt = (gv-gv2)/dt
              r0  = u2 (i1,i2,i3,rc)
              rx0 = u2 x22(i1,i2,i3,rc)
              ry0 = u2 y22(i1,i2,i3,rc)
              rxx0= u2 xx22(i1,i2,i3,rc)
              rxy0= u2 xy22(i1,i2,i3,rc)
              ryy0= u2 yy22(i1,i2,i3,rc)
              u0  = u2 (i1,i2,i3,uc)
              ux0 = u2 x22(i1,i2,i3,uc)
              uy0 = u2 y22(i1,i2,i3,uc)
              uxx0= u2 xx22(i1,i2,i3,uc)
              uxy0= u2 xy22(i1,i2,i3,uc)
              uyy0= u2 yy22(i1,i2,i3,uc)
              v0  = u2 (i1,i2,i3,vc)
              vx0 = u2 x22(i1,i2,i3,vc)
              vy0 = u2 y22(i1,i2,i3,vc)
              vxx0= u2 xx22(i1,i2,i3,vc)
              vxy0= u2 xy22(i1,i2,i3,vc)
              vyy0= u2 yy22(i1,i2,i3,vc)
              q0  = u2 (i1,i2,i3,tc)
              qx0 = u2 x22(i1,i2,i3,tc)
              qy0 = u2 y22(i1,i2,i3,tc)
              qxx0= u2 xx22(i1,i2,i3,tc)
              qxy0= u2 xy22(i1,i2,i3,tc)
              qyy0= u2 yy22(i1,i2,i3,tc)
              p0 = r0*q0                     ! Rg needed
              px0 =rx0*q0+r0*qx0
              py0 =ry0*q0+r0*qy0
              pxx0=rxx0*q0+rx0*qx0 + rx0*qx0+r0*qxx0
              pxy0=rxy0*q0+ry0*qx0 + rx0*qy0+r0*qxy0
              pyy0=ryy0*q0+ry0*qy0 + ry0*qy0+r0*qyy0
              if( twilightZone.ne.0 )then
                ! evaluate TZ forcing at t
                call ogftaylor(ep,x(i1,i2,i3,0),x(i1,i2,i3,1),z0,tm,nd,
     & fv)
              end if
              if( gridIsMoving.ne.0 )then
                ! -- add moving grid terms ----
                if( twilightZone.eq.0 )then
                  do mm=0,19
                    fv(mm)=0.
                  end do
                end if
                ! *** note: we need gv2 at tm here -- 
                fv( 0)=fv( 0) + gv2(i1,i2,i3,0)*rx0 + gv2(i1,i2,i3,1)*
     & ry0
                fv( 1)=fv( 1) + gv2(i1,i2,i3,0)*ux0 + gv2(i1,i2,i3,1)*
     & uy0
                fv( 2)=fv( 2) + gv2(i1,i2,i3,0)*vx0 + gv2(i1,i2,i3,1)*
     & vy0
                fv( 3)=fv( 3) + gv2(i1,i2,i3,0)*qx0 + gv2(i1,i2,i3,1)*
     & qy0
                fv( 4)=fv( 4) + gv2(i1,i2,i3,0)*px0 + gv2(i1,i2,i3,1)*
     & py0
                ! estimate gtt (we cannot use gtt if we are not on the boundary)
                ! **** this is not correct -- rtx0 etc. are not known yet
                stop 1224
                gttu0 = (gv(i1,i2,i3,0)-gv2(i1,i2,i3,0))/dt
                gttv0 = (gv(i1,i2,i3,1)-gv2(i1,i2,i3,1))/dt
                fv( 5)=fv( 5) + gttu0*rx0 + gttv0*ry0 + gv2(i1,i2,i3,0)
     & *rtx0 + gv2(i1,i2,i3,1)*rty0
                fv( 6)=fv( 6) + gttu0*ux0 + gttv0*uy0 + gv2(i1,i2,i3,0)
     & *utx0 + gv2(i1,i2,i3,1)*uty0
                fv( 7)=fv( 7) + gttu0*vx0 + gttv0*vy0 + gv2(i1,i2,i3,0)
     & *vtx0 + gv2(i1,i2,i3,1)*vty0
                fv( 8)=fv( 8) + gttu0*qx0 + gttv0*qy0 + gv2(i1,i2,i3,0)
     & *qtx0 + gv2(i1,i2,i3,1)*qty0
                fv( 9)=fv( 9) + gttu0*px0 + gttv0*py0 + gv2(i1,i2,i3,0)
     & *ptx0 + gv2(i1,i2,i3,1)*pty0
                ! we need derivatives of the grid velocity:
                gvux0=gv2 x22(i1,i2,i3,0)
                gvuy0=gv2 y22(i1,i2,i3,0)
                gvvx0=gv2 x22(i1,i2,i3,1)
                gvvy0=gv2 y22(i1,i2,i3,1)
                fv(10)=fv(10) + gv2(i1,i2,i3,0)*rxx0 + gv2(i1,i2,i3,1)*
     & rxy0 + gvux0*rx0 + gvvx0*ry0
                fv(11)=fv(11) + gv2(i1,i2,i3,0)*uxx0 + gv2(i1,i2,i3,1)*
     & uxy0 + gvux0*ux0 + gvvx0*uy0
                fv(12)=fv(12) + gv2(i1,i2,i3,0)*vxx0 + gv2(i1,i2,i3,1)*
     & vxy0 + gvux0*vx0 + gvvx0*vy0
                fv(13)=fv(13) + gv2(i1,i2,i3,0)*qxx0 + gv2(i1,i2,i3,1)*
     & qxy0 + gvux0*qx0 + gvvx0*qy0
                fv(14)=fv(14) + gv2(i1,i2,i3,0)*pxx0 + gv2(i1,i2,i3,1)*
     & pxy0 + gvux0*px0 + gvvx0*py0
                fv(15)=fv(15) + gv2(i1,i2,i3,0)*rxy0 + gv2(i1,i2,i3,1)*
     & ryy0 + gvuy0*rx0 + gvvy0*ry0
                fv(16)=fv(16) + gv2(i1,i2,i3,0)*uxy0 + gv2(i1,i2,i3,1)*
     & uyy0 + gvuy0*ux0 + gvvy0*uy0
                fv(17)=fv(17) + gv2(i1,i2,i3,0)*vxy0 + gv2(i1,i2,i3,1)*
     & vyy0 + gvuy0*vx0 + gvvy0*vy0
                fv(18)=fv(18) + gv2(i1,i2,i3,0)*qxy0 + gv2(i1,i2,i3,1)*
     & qyy0 + gvuy0*qx0 + gvvy0*qy0
                fv(19)=fv(19) + gv2(i1,i2,i3,0)*pxy0 + gv2(i1,i2,i3,1)*
     & pyy0 + gvuy0*px0 + gvvy0*py0
              end if
              pt0 = -( u0*px0 + v0*py0 + gamma*p0*(ux0+vy0) -fv(4) )
              ptx0 =-( ux0*px0+vx0*py0 + gamma*px0*(ux0+vy0) + u0*pxx0+
     & v0*pxy0 + gamma*p0*(uxx0+vxy0) -fv(14) )
              pty0 =-( uy0*px0+vy0*py0 + gamma*py0*(ux0+vy0) + u0*pxy0+
     & v0*pyy0 + gamma*p0*(uxy0+vyy0) -fv(19) )
              qt0 = -( u0*qx0 + v0*qy0 + gm1*q0*(ux0+vy0) -fv(3) )
              qtx0 =-( ux0*qx0+vx0*qy0 + gm1*qx0*(ux0+vy0) + u0*qxx0+
     & v0*qxy0 + gm1*q0*(uxx0+vxy0) -fv(13) )
              qty0 =-( uy0*qx0+vy0*qy0 + gm1*qy0*(ux0+vy0) + u0*qxy0+
     & v0*qyy0 + gm1*q0*(uxy0+vyy0) -fv(18) )
              rt0 = -( u0*rx0+v0*ry0 + r0*(ux0+vy0) -fv(0) )
              rtx0= -( ux0*rx0 +rx0*ux0 +vx0*ry0 + rx0*vy0 + u0*rxx0+
     & v0*rxy0 + r0*(uxx0+vxy0) -fv(10) )
              rty0= -( uy0*rx0 +ry0*ux0 +vy0*ry0 + ry0*vy0 + u0*rxy0+
     & v0*ryy0 + r0*(uxy0+vyy0) -fv(15) )
              ut0 = -( u0*ux0 + v0*uy0 + px0/r0 -fv(1) )
              utx0= -( ux0*ux0 +u0*uxx0 + vx0*uy0 + v0*uxy0 + pxx0/r0 -
     &  px0*rx0/(r0**2) -fv(11) )
              uty0= -( uy0*ux0 +u0*uxy0 + vy0*uy0 + v0*uyy0 + pxy0/r0 -
     &  px0*ry0/(r0**2) -fv(16) )
              vt0 = -( u0*vx0 + v0*vy0 + py0/r0 -fv(2) )
              vtx0= -( ux0*vx0 +u0*vxx0 + vx0*vy0 + v0*vxy0 + pxy0/r0 -
     &  py0*rx0/(r0**2) -fv(12) )
              vty0= -( uy0*vx0 +u0*vxy0 + vy0*vy0 + v0*vyy0 + pyy0/r0 -
     &  py0*ry0/(r0**2) -fv(17) )
              rtt0= -( ut0*rx0+vt0*ry0 + rt0*(ux0+vy0) + u0*rtx0+v0*
     & rty0 + r0*(utx0+vty0) -fv(5) )
              utt0= -( ut0*ux0 + vt0*uy0 + ptx0/r0 + u0*utx0 + v0*uty0 
     & - px0*rt0/(r0**2) -fv(6) )
              vtt0= -( ut0*vx0 + vt0*vy0 + pty0/r0 + u0*vtx0 + v0*vty0 
     & - py0*rt0/(r0**2) -fv(7) )
              ptt0= -( ut0*px0 + vt0*py0 + gamma*pt0*(ux0+vy0) + u0*
     & ptx0 + v0*pty0 + gamma*p0*(utx0+vty0) -fv(9) )
              qtt0= -( ut0*qx0 + vt0*qy0 + gm1*qt0*(ux0+vy0) + u0*qtx0 
     & + v0*qty0 + gm1*q0*(utx0+vty0) -fv(8) )
             rrt = rtx0*xri + rty0*yri     ! (rho.r).t = ...
             urt = utx0*xri + uty0*yri
             vrt = vtx0*xri + vty0*yri
             qrt = qtx0*xri + qty0*yri

              ! Here are the solution and derivatives for grid function u and gridVelocity gv at time t
              ! gv2 and gv are used to compute gtt = (gv-gv2)/dt
              r0  = u (i1,i2,i3,rc)
              rx0 = u x22(i1,i2,i3,rc)
              ry0 = u y22(i1,i2,i3,rc)
              rxx0= u xx22(i1,i2,i3,rc)
              rxy0= u xy22(i1,i2,i3,rc)
              ryy0= u yy22(i1,i2,i3,rc)
              u0  = u (i1,i2,i3,uc)
              ux0 = u x22(i1,i2,i3,uc)
              uy0 = u y22(i1,i2,i3,uc)
              uxx0= u xx22(i1,i2,i3,uc)
              uxy0= u xy22(i1,i2,i3,uc)
              uyy0= u yy22(i1,i2,i3,uc)
              v0  = u (i1,i2,i3,vc)
              vx0 = u x22(i1,i2,i3,vc)
              vy0 = u y22(i1,i2,i3,vc)
              vxx0= u xx22(i1,i2,i3,vc)
              vxy0= u xy22(i1,i2,i3,vc)
              vyy0= u yy22(i1,i2,i3,vc)
              q0  = u (i1,i2,i3,tc)
              qx0 = u x22(i1,i2,i3,tc)
              qy0 = u y22(i1,i2,i3,tc)
              qxx0= u xx22(i1,i2,i3,tc)
              qxy0= u xy22(i1,i2,i3,tc)
              qyy0= u yy22(i1,i2,i3,tc)
              p0 = r0*q0                     ! Rg needed
              px0 =rx0*q0+r0*qx0
              py0 =ry0*q0+r0*qy0
              pxx0=rxx0*q0+rx0*qx0 + rx0*qx0+r0*qxx0
              pxy0=rxy0*q0+ry0*qx0 + rx0*qy0+r0*qxy0
              pyy0=ryy0*q0+ry0*qy0 + ry0*qy0+r0*qyy0
              if( twilightZone.ne.0 )then
                ! evaluate TZ forcing at t
                call ogftaylor(ep,x(i1,i2,i3,0),x(i1,i2,i3,1),z0,t,nd,
     & fv)
              end if
              if( gridIsMoving.ne.0 )then
                ! -- add moving grid terms ----
                if( twilightZone.eq.0 )then
                  do mm=0,19
                    fv(mm)=0.
                  end do
                end if
                ! *** note: we need gv at t here -- 
                fv( 0)=fv( 0) + gv(i1,i2,i3,0)*rx0 + gv(i1,i2,i3,1)*ry0
                fv( 1)=fv( 1) + gv(i1,i2,i3,0)*ux0 + gv(i1,i2,i3,1)*uy0
                fv( 2)=fv( 2) + gv(i1,i2,i3,0)*vx0 + gv(i1,i2,i3,1)*vy0
                fv( 3)=fv( 3) + gv(i1,i2,i3,0)*qx0 + gv(i1,i2,i3,1)*qy0
                fv( 4)=fv( 4) + gv(i1,i2,i3,0)*px0 + gv(i1,i2,i3,1)*py0
                ! estimate gtt (we cannot use gtt if we are not on the boundary)
                ! **** this is not correct -- rtx0 etc. are not known yet
                stop 1224
                gttu0 = (gv(i1,i2,i3,0)-gv2(i1,i2,i3,0))/dt
                gttv0 = (gv(i1,i2,i3,1)-gv2(i1,i2,i3,1))/dt
                fv( 5)=fv( 5) + gttu0*rx0 + gttv0*ry0 + gv(i1,i2,i3,0)*
     & rtx0 + gv(i1,i2,i3,1)*rty0
                fv( 6)=fv( 6) + gttu0*ux0 + gttv0*uy0 + gv(i1,i2,i3,0)*
     & utx0 + gv(i1,i2,i3,1)*uty0
                fv( 7)=fv( 7) + gttu0*vx0 + gttv0*vy0 + gv(i1,i2,i3,0)*
     & vtx0 + gv(i1,i2,i3,1)*vty0
                fv( 8)=fv( 8) + gttu0*qx0 + gttv0*qy0 + gv(i1,i2,i3,0)*
     & qtx0 + gv(i1,i2,i3,1)*qty0
                fv( 9)=fv( 9) + gttu0*px0 + gttv0*py0 + gv(i1,i2,i3,0)*
     & ptx0 + gv(i1,i2,i3,1)*pty0
                ! we need derivatives of the grid velocity:
                gvux0=gv x22(i1,i2,i3,0)
                gvuy0=gv y22(i1,i2,i3,0)
                gvvx0=gv x22(i1,i2,i3,1)
                gvvy0=gv y22(i1,i2,i3,1)
                fv(10)=fv(10) + gv(i1,i2,i3,0)*rxx0 + gv(i1,i2,i3,1)*
     & rxy0 + gvux0*rx0 + gvvx0*ry0
                fv(11)=fv(11) + gv(i1,i2,i3,0)*uxx0 + gv(i1,i2,i3,1)*
     & uxy0 + gvux0*ux0 + gvvx0*uy0
                fv(12)=fv(12) + gv(i1,i2,i3,0)*vxx0 + gv(i1,i2,i3,1)*
     & vxy0 + gvux0*vx0 + gvvx0*vy0
                fv(13)=fv(13) + gv(i1,i2,i3,0)*qxx0 + gv(i1,i2,i3,1)*
     & qxy0 + gvux0*qx0 + gvvx0*qy0
                fv(14)=fv(14) + gv(i1,i2,i3,0)*pxx0 + gv(i1,i2,i3,1)*
     & pxy0 + gvux0*px0 + gvvx0*py0
                fv(15)=fv(15) + gv(i1,i2,i3,0)*rxy0 + gv(i1,i2,i3,1)*
     & ryy0 + gvuy0*rx0 + gvvy0*ry0
                fv(16)=fv(16) + gv(i1,i2,i3,0)*uxy0 + gv(i1,i2,i3,1)*
     & uyy0 + gvuy0*ux0 + gvvy0*uy0
                fv(17)=fv(17) + gv(i1,i2,i3,0)*vxy0 + gv(i1,i2,i3,1)*
     & vyy0 + gvuy0*vx0 + gvvy0*vy0
                fv(18)=fv(18) + gv(i1,i2,i3,0)*qxy0 + gv(i1,i2,i3,1)*
     & qyy0 + gvuy0*qx0 + gvvy0*qy0
                fv(19)=fv(19) + gv(i1,i2,i3,0)*pxy0 + gv(i1,i2,i3,1)*
     & pyy0 + gvuy0*px0 + gvvy0*py0
              end if
              pt0 = -( u0*px0 + v0*py0 + gamma*p0*(ux0+vy0) -fv(4) )
              ptx0 =-( ux0*px0+vx0*py0 + gamma*px0*(ux0+vy0) + u0*pxx0+
     & v0*pxy0 + gamma*p0*(uxx0+vxy0) -fv(14) )
              pty0 =-( uy0*px0+vy0*py0 + gamma*py0*(ux0+vy0) + u0*pxy0+
     & v0*pyy0 + gamma*p0*(uxy0+vyy0) -fv(19) )
              qt0 = -( u0*qx0 + v0*qy0 + gm1*q0*(ux0+vy0) -fv(3) )
              qtx0 =-( ux0*qx0+vx0*qy0 + gm1*qx0*(ux0+vy0) + u0*qxx0+
     & v0*qxy0 + gm1*q0*(uxx0+vxy0) -fv(13) )
              qty0 =-( uy0*qx0+vy0*qy0 + gm1*qy0*(ux0+vy0) + u0*qxy0+
     & v0*qyy0 + gm1*q0*(uxy0+vyy0) -fv(18) )
              rt0 = -( u0*rx0+v0*ry0 + r0*(ux0+vy0) -fv(0) )
              rtx0= -( ux0*rx0 +rx0*ux0 +vx0*ry0 + rx0*vy0 + u0*rxx0+
     & v0*rxy0 + r0*(uxx0+vxy0) -fv(10) )
              rty0= -( uy0*rx0 +ry0*ux0 +vy0*ry0 + ry0*vy0 + u0*rxy0+
     & v0*ryy0 + r0*(uxy0+vyy0) -fv(15) )
              ut0 = -( u0*ux0 + v0*uy0 + px0/r0 -fv(1) )
              utx0= -( ux0*ux0 +u0*uxx0 + vx0*uy0 + v0*uxy0 + pxx0/r0 -
     &  px0*rx0/(r0**2) -fv(11) )
              uty0= -( uy0*ux0 +u0*uxy0 + vy0*uy0 + v0*uyy0 + pxy0/r0 -
     &  px0*ry0/(r0**2) -fv(16) )
              vt0 = -( u0*vx0 + v0*vy0 + py0/r0 -fv(2) )
              vtx0= -( ux0*vx0 +u0*vxx0 + vx0*vy0 + v0*vxy0 + pxy0/r0 -
     &  py0*rx0/(r0**2) -fv(12) )
              vty0= -( uy0*vx0 +u0*vxy0 + vy0*vy0 + v0*vyy0 + pyy0/r0 -
     &  py0*ry0/(r0**2) -fv(17) )
              rtt0= -( ut0*rx0+vt0*ry0 + rt0*(ux0+vy0) + u0*rtx0+v0*
     & rty0 + r0*(utx0+vty0) -fv(5) )
              utt0= -( ut0*ux0 + vt0*uy0 + ptx0/r0 + u0*utx0 + v0*uty0 
     & - px0*rt0/(r0**2) -fv(6) )
              vtt0= -( ut0*vx0 + vt0*vy0 + pty0/r0 + u0*vtx0 + v0*vty0 
     & - py0*rt0/(r0**2) -fv(7) )
              ptt0= -( ut0*px0 + vt0*py0 + gamma*pt0*(ux0+vy0) + u0*
     & ptx0 + v0*pty0 + gamma*p0*(utx0+vty0) -fv(9) )
              qtt0= -( ut0*qx0 + vt0*qy0 + gm1*qt0*(ux0+vy0) + u0*qtx0 
     & + v0*qty0 + gm1*q0*(utx0+vty0) -fv(8) )
             ! average ut(t-dt) and u(t)
             rrt = .5*( rrt + rtx0*xri + rty0*yri )    ! (rho.r).t = ...
             urt = .5*( urt + utx0*xri + uty0*yri )
             vrt = .5*( vrt + vtx0*xri + vty0*yri )
             qrt = .5*( qrt + qtx0*xri + qty0*yri )

             if( .false. .and. twilightZone.ne.0  )then ! ******** not needed here -- already done ****
               ! add in the r-derivatives of the forcing at t=t+dt/2.
               ! drEps=dr(axis)*.001
               tHalf = t+.5*dt
               call ogftaylor(ep,x(i1,i2,i3,0)+xri*drEps,x(i1,i2,i3,1)+
     & yri*drEps,z0,tHalf,nd,fv)
               frr=fv(0)
               fur=fv(1)
               fvr=fv(2)
               fqr=fv(3)
               call ogftaylor(ep,x(i1,i2,i3,0)-xri*drEps,x(i1,i2,i3,1)-
     & yri*drEps,z0,tHalf,nd,fv)

               frr=(frr-fv(0))/(2.*drEps) ! fr.r
               fur=(fur-fv(1))/(2.*drEps) ! fu.r
               fvr=(fvr-fv(2))/(2.*drEps) ! fv.r
               fqr=(fqr-fv(3))/(2.*drEps) ! fq.r

               rrt=rrt + frr
               urt=urt + fur
               vrt=vrt + fvr
               qrt=qrt + fqr

             end if

            if( .false. )then
              ! use exact value for rho.rt
              call ogderiv(ep, 1,1,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),z0,
     & t,rc,rxte)
              call ogderiv(ep, 1,0,1,0, x(i1,i2,i3,0),x(i1,i2,i3,1),z0,
     & t,rc,ryte)
              rrte  = xri*rxte+yri*ryte

              rrt=rrte
            end if

            if( axis.eq.0 )then
              rr1=u2r2(i1,i2,i3,rc)+dt*( rrt )   ! rho.r
              ur1=u2r2(i1,i2,i3,uc)+dt*( urt )   ! u.r
              vr1=u2r2(i1,i2,i3,vc)+dt*( vrt )   ! v.r
              qr1=u2r2(i1,i2,i3,tc)+dt*( qrt )   ! q.r
            else
              rr1=u2s2(i1,i2,i3,rc)+dt*( rrt )   ! rho.r
              ur1=u2s2(i1,i2,i3,uc)+dt*( urt )   ! u.r
              vr1=u2s2(i1,i2,i3,vc)+dt*( vrt )   ! v.r
              qr1=u2s2(i1,i2,i3,tc)+dt*( qrt )   ! q.r
            end if

            if( .false. )then
              ! use exact value for d(rho)/dr
              call ogderiv(ep, 0,1,0,0, x(i1,i2,i3,0),x(i1,i2,i3,1),z0,
     & t,rc,rxe)
              call ogderiv(ep, 0,0,1,0, x(i1,i2,i3,0),x(i1,i2,i3,1),z0,
     & t,rc,rye)
              rre  = xri*rxe+yri*rye

              rr1=rre
            end if

             do mm=1,2
              j1=i1-is1*mm
              j2=i2-is2*mm
              j3=i3-is3*mm
              k1=i1+is1*mm
              k2=i2+is2*mm
              k3=i3+is3*mm

              h=dr(axis)*sgn*mm

                  u(j1,j2,j3,rc)=u(k1,k2,k3,rc) -2.*h*( rr1 + 0. )
                  u(j1,j2,j3,tc)=u(k1,k2,k3,tc) -2.*h*( qr1 + 0. )

              ! only update the tangential component of the velocity: (the normal component should be correct)
              um = u(k1,k2,k3,uc) -2.*h*(ur1 + 0. )
              vm = u(k1,k2,k3,vc) -2.*h*(vr1 + 0. )

              tauDotU=tau1*(um-u(j1,j2,j3,uc))+tau2*(vm-u(j1,j2,j3,vc))
                  u(j1,j2,j3,uc)=u(j1,j2,j3,uc) + tauDotU*tau1
                  u(j1,j2,j3,vc)=u(j1,j2,j3,vc) + tauDotU*tau2
             end do

             rhor=rr1

             ! try this one sided approx for rho
             ! rhor = sgn*(-u(i1,i2,i3,rc)+u(i1+is1,i2+is2,i3,rc))/(dr(axis))
             ! rhor = sgn*(-3.*u(i1,i2,i3,rc)+2.*u(i1+is1,i2+is2,i3,rc)-u(i1+2*is1,i2+2*is2,i3,rc))/(2.*dr(axis))

             ! now get T given rhor and pr 
             if( axis.eq.0 )then
               pr=pra(ii)
             else
               pr=psa(ii)
             end if
             rho=u(i1,i2,i3,rc)
             tp=u(i1,i2,i3,tc)
             tpr = (pr-rhor*tp)/rho

             if( .true. )then  ! try turning this off
               u(i1-  is1,i2-  is2,i3,tc)=u(i1+  is1,i2+  is2,i3,tc) -
     & sgn*2.*dr(axis)*tpr
               u(i1-2*is1,i2-2*is2,i3,tc)=u(i1+2*is1,i2+2*is2,i3,tc) -
     & sgn*4.*dr(axis)*tpr
             end if

             ! ======== STAGE III normal smoothing step
             ! add smoothing in the normal direction
             ! if cnSmooth==1 --> D+D-u = 0 : choose cnSmooth=c*h^2 I think
             do mm=1,2
               j1=i1-is1*mm
               j2=i2-is2*mm
               j3=i3-is3*mm
               k1=i1+is1*mm
               k2=i2+is2*mm
               k3=i3+is3*mm
               do m=0,ncm1
               ! do m=0,2  ! smooth r,u,v
                 u(j1,j2,j3,m)=(1.-cnSmooth)*u(j1,j2,j3,m)+cnSmooth*(
     & 2.*u(i1,i2,i3,m)-u(k1,k2,k3,m))
               end do

             end do


            end if  ! end if mask

            if( .true. .and. debug.gt.2 .and. twilightZone.ne.0  )then
              ! check the solution
              do mm=1,2
               j1=i1-is1*mm
               j2=i2-is2*mm
               j3=i3-is3*mm
               do m=0,ncm1
                 uv(m)=ogf(ep,x(j1,j2,j3,0),x(j1,j2,j3,1),0.,m,t)
               end do

               write(*,'(" Stage III: j1,j2=",2i3," q=",4(f8.5,1x)," 
     & err=",4(e8.1,1x))') j1,j2,u(j1,j2,j3,rc),u(j1,j2,j3,uc),u(j1,
     & j2,j3,vc),u(j1,j2,j3,tc),abs(u(j1,j2,j3,rc)-uv(rc)),abs(u(j1,
     & j2,j3,uc)-uv(uc)),abs(u(j1,j2,j3,vc)-uv(vc)),abs(u(j1,j2,j3,tc)
     & -uv(tc))
                ! '
               ! write(*,'("          : rrt,urt,vrt=",4(f8.3,1x))') rrt,urt,vrt
             end do
            end if


           ii=ii+1
            end do
            end do
            end do
           ! ** end do ! it3

           ! ---- add some tangential artificial dissipation to the ghost points ----
           nit=0 ! 3
           do it=1,nit
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
             do mm=1,2       ! 2 ghost lines
               j1=i1-is1*mm
               j2=i2-is2*mm
               j3=i3-is3*mm

               do m=0,ncm1
                 u(j1,j2,j3,m) = u(j1,j2,j3,m)+(5.*dt)*((u(j1+js1,j2+
     & js2,j3,m)-2.*u(j1,j2,j3,m)+u(j1-js1,j2-js2,j3,m)))
               end do

             end do
             end if
            end do
            end do
            end do
           end do

           if( .false. .and. twilightZone.ne.0 )then
             write(*,'(" **** STAGE IV : set rho to be exact solution 
     & on ghost ***")')
             ! '
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
             do mm=1,2       ! 2 ghost lines
               j1=i1-is1*mm
               j2=i2-is2*mm
               j3=i3-is3*mm

               do m=0,ncm1
                 uv(m)=ogf(ep,x(j1,j2,j3,0),x(j1,j2,j3,1),0.,m,t)
               end do
               !  set rho to be exact on ghost ***********
               ! u(j1,j2,j3,rc)=uv(0) 
               ! u(j1,j2,j3,uc)=uv(1) 
               ! u(j1,j2,j3,vc)=uv(2) 
               ! u(j1,j2,j3,tc)=uv(3) 

             end do
               end if
              end do
              end do
              end do
           end if

           ! ** assign values on the extended ghost lines
              ! ** assign values on the extended ghost lines
              !
              !         X  X  +
              !         G  G  + ---------------------
              !         G  G  +
              !         G  G  +
              !         G  G  +
              !         G  G  +
              !         G  G  + ---------------------
              !         X  X  +
              ! write(*,'(" end of stage I: nr = ",4i4)') nr(0,0),nr(1,0),nr(0,1),nr(1,1)
              ms1=0
              ms2=0
              ms3=0
              i3=nr(0,2)
              j3=i3
              side2=-1
              do sideb=0,1
                ! used side2 instead if sideb, this is needed to avoid a bug with pgf77 compiled with -O !
                side2=side2+1
                if( axis.eq.0 )then
                  i1=nr(side ,0)
                  i2=nr(side2,1)
                  ms2=1-2*side2
                  ! write(*,'(" end of stage I: sideb,side2,nr(0,1),nr(sideb,1),nr(side2,1)=",10i4)') sideb,side2,nr(0,1),nr(sideb,1),nr(side2,1)
                  ! '
                else
                  i1=nr(side2,0)
                  i2=nr(side ,1)
                  ms1=1-2*side2
                end if
                do mm=1,2   ! two ghost lines
                  if( axis.eq.0 )then
                    j1=i1-is1*mm
                    j2=i2-ms2
                  else
                    j1=i1-ms1
                    j2=i2-is2*mm
                  end if
                  ! write(*,'(" end of stage I: fill extended ghost value i1,i2,ms1,ms2,j1,j2=",6i4)') i1,i2,ms1,ms2,j1,j2
                  ! '
                  if( bc(side2,axisp1).lt.0 )then
                    ! apply periodicity
                    kv(0)=j1
                    kv(1)=j2
                    kv(2)=j3
                    kv(axisp1) = kv(axisp1) + (nr(1,axisp1)-nr(0,
     & axisp1))*(1-2*side2)
                    !write(*,'(" end of stage I: periodic update j1,j2,j3=",3i4," from k1,k2,k3=",3i4)') !       j1,j2,j3,kv(0),kv(1),kv(2)
                    ! '
                    do m=0,ncm1
                      u(j1,j2,j3,m)=u(kv(0),kv(1),kv(2),m)
                    end do
                  else if( .true. )then ! turn this off for now  ***********
                  do m=0,ncm1
                    u(j1,j2,j3,m)=3.*u(j1+  ms1,j2+  ms2,j3+  ms3,m)-
     & 3.*u(j1+2*ms1,j2+2*ms2,j3+2*ms3,m)+u(j1+3*ms1,j2+3*ms2,j3+3*
     & ms3,m)
                  end do
                  end if
                end do
              end do

         end if ! end if( .not.getGhostByTaylor )


      if( .false. )then
        stop 57321
      end if

          else if( dt.gt.0. )then
            stop 88225
          end if ! bcOption

          nr(0,axis)=indexRange(0,axis)  ! reset
          nr(1,axis)=indexRange(1,axis)




        else if( bc(side,axis).eq.axisymmetric )then

          ! axisymmetric -- this is not quite right for stretched grids ---- fix this ---

          nr(0,axis)=indexRange(side,axis)   ! set nr to point to this side
          nr(1,axis)=indexRange(side,axis)

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
            if( mask(i1,i2,i3).gt.0 )then
            u(i1,i2,i3,vc)=0.
            u(i1-is1,i2-is2,i3,vc)=-u(i1+is1,i2+is2,i3,vc)
            u(i1-ks1,i2-ks2,i3,vc)=-u(i1+ks1,i2+ks2,i3,vc)

            u(i1-is1,i2-is2,i3,rc)= u(i1+is1,i2+is2,i3,rc)
            u(i1-ks1,i2-ks2,i3,rc)= u(i1+ks1,i2+ks2,i3,rc)

            u(i1-is1,i2-is2,i3,uc)= u(i1+is1,i2+is2,i3,uc)
            u(i1-ks1,i2-ks2,i3,uc)= u(i1+ks1,i2+ks2,i3,uc)

            u(i1-is1,i2-is2,i3,tc)= u(i1+is1,i2+is2,i3,tc)
            u(i1-ks1,i2-ks2,i3,tc)= u(i1+ks1,i2+ks2,i3,tc)

            end if
           end do
           end do
           end do
          ! species
          do s=sc,sc+numberOfSpecies-1
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
            u(i1-is1,i2-is2,i3,s)= u(i1+is1,i2+is2,i3,s)
            u(i1-ks1,i2-ks2,i3,s)= u(i1+ks1,i2+ks2,i3,s)
            end if
           end do
           end do
           end do
          end do

          nr(0,axis)=indexRange(0,axis)  ! reset
          nr(1,axis)=indexRange(1,axis)

        end if

        end do
        end do

      else

        write(*,'("cnsSlipWallBC2:ERROR:Unknown bcOption=",i5)') 
     & bcOption
        stop 17942

      end if


      return
      end



