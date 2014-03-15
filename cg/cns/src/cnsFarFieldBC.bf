c
c routines for applying a far-field BC
c

#Include "defineDiffOrder2f.h"


#beginMacro beginLoops()
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
#endMacro

#beginMacro endLoops()
  end if
 end do
 end do
 end do
#endMacro

#beginMacro beginLoopsNoMask()
 n1a=nr(0,0)
 n1b=nr(1,0)
 n2a=nr(0,1)
 n2b=nr(1,1)
 n3a=nr(0,2)
 n3b=nr(1,2)
 do i3=n3a,n3b
 do i2=n2a,n2b
 do i1=n1a,n1b
#endMacro

#beginMacro endLoopsNoMask()
 end do
 end do
 end do
#endMacro


#beginMacro assignEndValues()
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
         kv(axisp1) = kv(axisp1) + (nr(1,axisp1)-nr(0,axisp1))*(1-2*side2)
         !write(*,'(" end of stage I: periodic update j1,j2,j3=",3i4," from k1,k2,k3=",3i4)') \
         !       j1,j2,j3,kv(0),kv(1),kv(2)
         ! '
         do m=0,ncm1
           u(j1,j2,j3,m)=u(kv(0),kv(1),kv(2),m)
         end do

       else if( .true. )then ! turn this off for now  ***********

       do m=0,ncm1
         u(j1,j2,j3,m)=3.*u(j1+  ms1,j2+  ms2,j3+  ms3,m)\
                      -3.*u(j1+2*ms1,j2+2*ms2,j3+2*ms3,m)\
                         +u(j1+3*ms1,j2+3*ms2,j3+3*ms3,m)
       end do
       end if
     end do
   end do
#endMacro

#beginMacro setGhostValuesAtInterpAndUnused()
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

   u(j1,j2,j3,uc) =3.*u(j1+is1,j2+is2,j3,uc)-3.*u(j1+2*is1,j2+2*is2,j3,uc)+u(j1+3*is1,j2+3*is2,j3,uc)
   u(j1,j2,j3,vc) =3.*u(j1+is1,j2+is2,j3,vc)-3.*u(j1+2*is1,j2+2*is2,j3,vc)+u(j1+3*is1,j2+3*is2,j3,vc)

   ! extrap normal component of u 
   !   -- this extrpolation will be consistent with an odd symmetry condition (u.rr=0)
   nDotU1 = an1*( 2.*u(i1,i2,i3,uc)-u(k1,k2,k3,uc) ) + an2*( 2.*u(i1,i2,i3,vc)-u(k1,k2,k3,vc) )

   ! set the normal component to be nDotU1
   nDotU = an1*u(j1,j2,j3,uc)+an2*u(j1,j2,j3,vc) - nDotU1
   u(j1,j2,j3,uc)=u(j1,j2,j3,uc)- nDotU*an1
   u(j1,j2,j3,vc)=u(j1,j2,j3,vc)- nDotU*an2

 end do
 
#endMacro


      subroutine cnsFarFieldBC(nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
          ipar,rpar, u, u2,  gv, gv2, gtt, mask, x,rsxy, bc, indexRange, exact, uKnown, ierr )         
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
      real pn,rho,rhon,nDotGradR,nDotGradS,tp,tpn,rhor,rhos,tps,ps,tpm,pm,pp,pr,tpr
      integer axisp1

      real un,c

      integer is1,is2,is3,js1,js2,js3,ks1,ks2,ks3,s,m,mm

      real t,dt
      real an1,an2,an3,nDotU,aNorm,epsx,gamma
      real dr(0:2),dx(0:2),ad(0:10)
      real us0,vs0,w0s,sgn

      real rra,ura,vra,wra, rsa,usa,vsa,wsa, urra,vrra,wrra, ussa,vssa,wssa, rrsa, ursa,vrsa,wrsa               
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
      real ajac,rrt,rt,urt,vrt,qrt,rr1,tau1,tau2,px,py,aurr,avrr,aur,avr
      real tp0,divu,rxU,sxU,pnm1,pnp1,pns,un0,vn0,rxn,ryn,sxn,syn,ank,rxUn,sxUn
      real rxri,ryri,sxri,syri,rxsi,rysi,sxsi,sysi,divun,sxUt,an1r,an2r,an1s,an2s,rv2t,Lnu
      real a11,a12,a21,a22,f1,f2,det, um, vm,tauDotU,b1,b2
      real pra(-2:402), psa(-2:402)  ! fix these ******************************************

      real prr,prs,pss,pst, divur, divus, rxUr, sxUr, rxUs, sxUs
      real gtu0,gtv0, gtru0,gtrv0, gtsu0, gtsv0, gttru0, gttrv0, gttsu0, gttsv0, gu0, gv0, gur0, gvr0, gus0, gvs0
      real gtttu0, gtttv0
      real fut, fvt, fpr, fps
      real urr0,urs0,uss0, vrr0,vrs0,vss0, rs0
      real term1, term2, dtEps, drEps, dsEps, fu0, fv0
      real ute,vte,uxe,uye,vxe,vye,ure,vre,use,vse,pne,prre, rrre,qrre,qre, utte,vtte
      real urre,vrre,uxxe,uxye,vxxe,vxye,vrse,rrse,qrse,prse,pse, vyye,vsse
      real rxe,rye,qxe,qye,rre,fr0
      real re1,re2,re3,qe1,qe2,qe3
      real tHalf,frr,fur,fvr,fqr
      real re,qe,rte,qte,rxte,qxte,ryte,qyte,pxte,pyte,prte,resid1,prt, pnte,pnt, rtte
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

      integer slipWallSymmetry, slipWallPressureEntropySymmetry, slipWallTaylor, slipWallCharacteristic,\
              slipWallDerivative
      parameter( slipWallSymmetry=0, slipWallPressureEntropySymmetry=1, slipWallTaylor=2, slipWallCharacteristic=3,\
                 slipWallDerivative=4 )

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
     &  symmetry=11,axisymmetric=13, farField=16, neumannBoundaryCondition=18 )

      ! declare variables for difference approximations of u and RX
      declareDifferenceOrder2(u,RX)
      ! declare difference approximations for u2
      declareDifferenceOrder2(u2,NONE)
      ! declare for derivatives of gv
      declareDifferenceOrder2(gv,NONE)
      ! declare for derivatives of gv2
      declareDifferenceOrder2(gv2,NONE)

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

      diss2(i1,i2,i3,n)=ad2dt*(u2(i1+1,i2,i3,n)+u2(i1-1,i2,i3,n)+u2(i1,i2-1,i3,n)+u2(i1,i2+1,i3,n)-4.*u2(i1,i2,i3,n))

      disst2(i1,i2,i3,n)=ad2dt*(u2(i1+js1,i2+js2,i3,n)+u2(i1-js1,i2-js2,i3,n)-2.*u2(i1,i2,i3,n))

      ! another form of tangential dissipation: 
      tanDiss2(i1,i2,i3,n)=(1.+adu(n))*ad2dt*(u2(i1+js1,i2+js2,i3,n)+u2(i1-js1,i2-js2,i3,n)-2.*u2(i1,i2,i3,n))


c     The next macro call will define the difference approximation statement functions
      defineDifferenceOrder2Components1(u,RX)
      defineDifferenceOrder2Components1(u2,NONE)
      defineDifferenceOrder2Components1(gv,NONE)
      defineDifferenceOrder2Components1(gv2,NONE)

      u2xr22(i1,i2,i3,kd)= rx(i1,i2,i3)*u2rr2(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & u2rs2(i1,i2,i3,kd)+rxr2(i1,i2,i3)*u2r2(i1,i2,i3,kd)+sxr2(i1,i2,i3)*
     & u2s2(i1,i2,i3,kd)
      u2xs22(i1,i2,i3,kd)= rx(i1,i2,i3)*u2rs2(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & u2ss2(i1,i2,i3,kd)+rxs2(i1,i2,i3)*u2r2(i1,i2,i3,kd)+sxs2(i1,i2,i3)*
     & u2s2(i1,i2,i3,kd)
          
      u2yr22(i1,i2,i3,kd)= ry(i1,i2,i3)*u2rr2(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & u2rs2(i1,i2,i3,kd)+ryr2(i1,i2,i3)*u2r2(i1,i2,i3,kd)+syr2(i1,i2,i3)*
     & u2s2(i1,i2,i3,kd)
      u2ys22(i1,i2,i3,kd)= ry(i1,i2,i3)*u2rs2(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & u2ss2(i1,i2,i3,kd)+rys2(i1,i2,i3)*u2r2(i1,i2,i3,kd)+sys2(i1,i2,i3)*
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


      if( .false. .and. nd.eq.2 .and. gridType.eq.rectangular .and. twilightZone.eq.0 )then

        ! *********************************************************************
        ! ******* 2D non-moving, rectangular **********************************
        ! *********************************************************************

        if( gridIsMoving.ne.0 )then
          write(*,'("cnsFarFieldBC:ERROR: gridIsMoving not implemented yet for rectangular")')
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
          beginLoopsNoMask()
            ! do as a separate loop:  u(i1,i2,i3,unc)=0.
            u(i1-is1,i2-is2,i3,unc)=2.*u(i1,i2,i3,unc)-u(i1+is1,i2+is2,i3,unc)
            u(i1-is1,i2-is2,i3,utc)=u(i1+is1,i2+is2,i3,utc)

            u(i1-is1,i2-is2,i3,rc )=u(i1+is1,i2+is2,i3, rc)
            u(i1-is1,i2-is2,i3,tc )=u(i1+is1,i2+is2,i3, tc)

            ! --- 2nd ghost line: ----
            u(i1-ks1,i2-ks2,i3,unc)=2.*u(i1,i2,i3,unc)-u(i1+ks1,i2+ks2,i3,unc)
            u(i1-ks1,i2-ks2,i3,utc)=u(i1+ks1,i2+ks2,i3,utc)

            u(i1-ks1,i2-ks2,i3,rc )=u(i1+ks1,i2+ks2,i3, rc)
            u(i1-ks1,i2-ks2,i3,tc )=u(i1+ks1,i2+ks2,i3, tc)
          endLoopsNoMask()

          nr(0,axis)=indexRange(0,axis)  ! reset
          nr(1,axis)=indexRange(1,axis)

        end if ! if( bc.eq.farField )
        end do
        end do



      ! ******************* Farfield BC for curvilinear grids **********************************
      else if( .true. .or. (nd.eq.2 .and. gridType.eq.curvilinear) )then
     
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
              write(*,'(" ***cnsFarField:INFO: t,dt=",2(e12.3,1x))') t,dt
            end if
          end if

          tm=t-dt
          z0=0.

          ii=nr(0,axisp1)
          beginLoopsNoMask()  
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
                  u(j1,j2,j3,m)=u(j1,j2,j3,m) + (uKnown(j1,j2,j3,m)-uKnown(k1,k2,k3,m))
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
!      write(*,'("cnsFarField:side,axis,i1,i2,un,c --> supersonic outflow",4i3,2f7.3)')\
!              side,axis,i1,i2,un,c
               ! '
                do m=0,ncm1
!                  u(j1,j2,j3,m)=2.*u(i1,i2,i3,m)-u(k1,k2,k3,m)
                  u(j1,j2,j3,m)=3.*u(j1+is1,j2+is2,j3+is3,m)-3.*u(j1+2*is1,j2+2*is2,j3+2*is3,m)+u(j1+3*is1,j2+3*is2,j3+3*is3,m)
                end do
              else
               do m=0,ncm1
                ! u(i1,i2,i3,m)=u(k1,k2,k3,m)
                ! u(j1,j2,j3,m)=2.*u(i1,i2,i3,m)-u(k1,k2,k3,m)
                ! u(j1,j2,j3,m)=u(k1,k2,k3,m)
                u(j1,j2,j3,m)=3.*u(j1+is1,j2+is2,j3+is3,m)-3.*u(j1+2*is1,j2+2*is2,j3+2*is3,m)+u(j1+3*is1,j2+3*is2,j3+3*is3,m)
               end do
               m=tc
               u(j1,j2,j3,m)=u(k1,k2,k3,m)
               if(  knownSolution.gt.0 )then
                 do m=0,ncm1
                  ! u(i1,i2,i3,m)=u(i1,i2,i3,m) + uKnown(i1,i2,i3,m)-uKnown(k1,k2,k3,m)
                  ! u(j1,j2,j3,m)=u(j1,j2,j3,m) + (uKnown(j1,j2,j3,m)-2.*uKnown(i1,i2,i3,m)+uKnown(k1,k2,k3,m))
                  ! u(j1,j2,j3,m)=u(j1,j2,j3,m) + (uKnown(j1,j2,j3,m)-uKnown(k1,k2,k3,m))
                  u(j1,j2,j3,m)=u(j1,j2,j3,m) + (uKnown(j1,j2,j3,m)-3.*uKnown(j1+is1,j2+is2,j3+is3,m)\
                                   +3.*uKnown(j1+2*is1,j2+2*is2,j3+2*is3,m)-uKnown(j1+3*is1,j2+3*is2,j3+3*is3,m))
                 end do
                 m=tc
                 u(j1,j2,j3,m)=u(j1,j2,j3,m) + (uKnown(j1,j2,j3,m)-uKnown(k1,k2,k3,m))
               end if
              end if
              ! -------------------------------------------------

             else if( .false. )then

              ! try this: Extrap all except for T, Give T.n = 
              ! try this: Give D+D+()=D+D-(true) for all , then D0(T)=D0(true)
              do m=0,ncm1
               if( .true. .or. mm.eq.1 )then
                u(j1,j2,j3,m)=3.*u(j1+is1,j2+is2,j3+is3,m)-3.*u(j1+2*is1,j2+2*is2,j3+2*is3,m)+u(j1+3*is1,j2+3*is2,j3+3*is3,m)
               else
                u(j1,j2,j3,m)=2.*u(j1+is1,j2+is2,j3+is3,m)-u(j1+2*is1,j2+2*is2,j3+2*is3,m)
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
                  u(j1,j2,j3,m)=u(j1,j2,j3,m) + (uKnown(j1,j2,j3,m)-3.*uKnown(j1+is1,j2+is2,j3+is3,m)\
                                   +3.*uKnown(j1+2*is1,j2+2*is2,j3+2*is3,m)-uKnown(j1+3*is1,j2+3*is2,j3+3*is3,m))
                 else
                  u(j1,j2,j3,m)=u(j1,j2,j3,m) + (uKnown(j1,j2,j3,m)-2.*uKnown(j1+is1,j2+is2,j3+is3,m)\
                                 +uKnown(j1+2*is1,j2+2*is2,j3+2*is3,m))
                  !u(j1,j2,j3,m)=u(j1,j2,j3,m) + (uKnown(j1,j2,j3,m)-2.*uKnown(i1,i2,i3,m)+uKnown(k1,k2,k3,m))
                 end if
                end do
                m=tc
                u(j1,j2,j3,m)=u(j1,j2,j3,m) + (uKnown(j1,j2,j3,m)-uKnown(k1,k2,k3,m))
              end if
            
             end if
             
            end do


           else ! mask(i1,i2,i3) <=0 
             setGhostValuesAtInterpAndUnused()

           end if
           ii=ii+1
           endLoopsNoMask()

           ! ** assign values on the extended ghost lines
           ! assignEndValues()

          nr(0,axis)=indexRange(0,axis)  ! reset
          nr(1,axis)=indexRange(1,axis)

        end if ! if( bc.eq.farField )
        end do
        end do

      else

        write(*,'("cnsFarFieldBC2:ERROR:Unknown bcOption=",i5)') bcOption
        stop 17942

      end if


      return
      end



