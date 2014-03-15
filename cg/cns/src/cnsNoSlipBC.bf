c ***********************************************************************
c
c          Routines for applying a no-slip wall BC
c
c ***********************************************************************

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

#beginMacro loopse4(e1,e2,e3,e4)
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
   e1
   e2
   e3
   e4
  end if
 end do
 end do
 end do
#endMacro

#beginMacro loopse4NoMask(e1,e2,e3,e4)
 n1a=nr(0,0)
 n1b=nr(1,0)
 n2a=nr(0,1)
 n2b=nr(1,1)
 n3a=nr(0,2)
 n3b=nr(1,2)
 do i3=n3a,n3b
 do i2=n2a,n2b
 do i1=n1a,n1b
     e1
     e2
     e3
     e4
 end do
 end do
 end do
#endMacro


#beginMacro extrapTwoGhost(ORDER,DIR)
 do c=uc,uc+nd-1
 #If #ORDER == "5" 
  #If #DIR == "r"
    u(i1-is,i2,i3,c)=5.*(u(i1   ,i2,i3,c)-u(i1+3*is,i2,i3,c))\
                   -10.*(u(i1+is,i2,i3,c)-u(i1+2*is,i2,i3,c))+u(i1+4*is,i2,i3,c)
    u(i1-2*is,i2,i3,c)=5.*(u(i1-is,i2,i3,c)-u(i1+2*is,i2,i3,c))\
                     -10.*(u(i1   ,i2,i3,c)-u(i1+  is,i2,i3,c))+u(i1+3*is,i2,i3,c)
  #Elif #DIR == "s"
    u(i1,i2-is,i3,c)=5.*(u(i1,i2   ,i3,c)-u(i1,i2+3*is,i3,c))\
                   -10.*(u(i1,i2+is,i3,c)-u(i1,i2+2*is,i3,c))+u(i1,i2+4*is,i3,c)
    u(i1,i2-2*is,i3,c)=5.*(u(i1,i2-is,i3,c)-u(i1,i2+2*is,i3,c))\
                     -10.*(u(i1,i2   ,i3,c)-u(i1,i2+  is,i3,c))+u(i1,i2+3*is,i3,c)

    ! write(*,'(''extrap (DIR) c='',i2,''i='',i4,i4,2x,i4,i4)') c,i1,i2-is,i1,i2-2*is

    ! write(*,'(''extrap (DIR) c='',i2,''i='',i4,i4,2x,i4,i4)') c,i1,i2-is,i1,i2-2*is
    ! u(i1,i2-is,i3,c)=ogf(exact,x(i1,i2-is,i3,0),x(i1,i2-is,i3,1),0.,c,t)
    ! u(i1,i2-2*is,i3,c)=ogf(exact,x(i1,i2-2*is,i3,0),x(i1,i2-2*is,i3,1),0.,c,t)
  #Elif #DIR == "t"
    u(i1,i2,i3-is,c)=5.*(u(i1,i2,i3   ,c)-u(i1,i2,i3+3*is,c))\
                   -10.*(u(i1,i2,i3+is,c)-u(i1,i2,i3+2*is,c))+u(i1,i2,i3+4*is,c)
    u(i1,i2,i3-2*is,c)=5.*(u(i1,i2,i3-is,c)-u(i1,i2,i3+2*is,c))\
                     -10.*(u(i1,i2,i3   ,c)-u(i1,i2,i3+  is,c))+u(i1,i2,i3+3*is,c)
  #Else
   write(*,*) 'ERROR:unknown dir'
   stop 8
  #End
 #Else
   write(*,*) 'ERROR:unknown extrap order'
   stop 7
 #End
 end do
#endMacro

#beginMacro extrapolate(ORDER)
 if( kd2.eq.0 )then
   loopse4($extrapTwoGhost(ORDER,r),,,)
 else if( kd2.eq.1 )then
   loopse4($extrapTwoGhost(ORDER,s),,,)
 else
   loopse4($extrapTwoGhost(ORDER,t),,,)
 end if
#endMacro


#beginMacro getSolutionDerivatives(i1,i2,i3,tm)
 ! Here are the solution and derivatives at the previous time
 r0  = u2(i1,i2,i3,rc)
 rx0 = u2x22(i1,i2,i3,rc)
 ry0 = u2y22(i1,i2,i3,rc)
 rxx0= u2xx22(i1,i2,i3,rc)
 rxy0= u2xy22(i1,i2,i3,rc)
 ryy0= u2yy22(i1,i2,i3,rc)

 u0  = u2(i1,i2,i3,uc)
 ux0 = u2x22(i1,i2,i3,uc)
 uy0 = u2y22(i1,i2,i3,uc)
 uxx0= u2xx22(i1,i2,i3,uc)
 uxy0= u2xy22(i1,i2,i3,uc)
 uyy0= u2yy22(i1,i2,i3,uc)

 v0  = u2(i1,i2,i3,vc)
 vx0 = u2x22(i1,i2,i3,vc)
 vy0 = u2y22(i1,i2,i3,vc)
 vxx0= u2xx22(i1,i2,i3,vc)
 vxy0= u2xy22(i1,i2,i3,vc)
 vyy0= u2yy22(i1,i2,i3,vc)

 q0  = u2(i1,i2,i3,tc)
 qx0 = u2x22(i1,i2,i3,tc)
 qy0 = u2y22(i1,i2,i3,tc)
 qxx0= u2xx22(i1,i2,i3,tc)
 qxy0= u2xy22(i1,i2,i3,tc)
 qyy0= u2yy22(i1,i2,i3,tc)

 p0 = r0*q0                     ! Rg needed
 px0 =rx0*q0+r0*qx0 
 py0 =ry0*q0+r0*qy0 

 pxx0=rxx0*q0+rx0*qx0 + rx0*qx0+r0*qxx0 
 pxy0=rxy0*q0+ry0*qx0 + rx0*qy0+r0*qxy0 
 pyy0=ryy0*q0+ry0*qy0 + ry0*qy0+r0*qyy0 

 if( twilightZone.ne.0 )then
   ! evaluate TZ forcing at t
   call ogftaylor(ep,x(i1,i2,i3,0),x(i1,i2,i3,1),z0,tm,nd,fv) 
 end if
 if( gridIsMoving.ne.0 )then
   ! -- add moving grid terms ----
   if( twilightZone.eq.0 )then
     do mm=0,19
       fv(mm)=0.
     end do
   end if

   ! *** note: we need gv at t-dt here -- 
   fv( 0)=fv( 0) + gv2(i1,i2,i3,0)*rx0 + gv2(i1,i2,i3,1)*ry0 
   fv( 1)=fv( 1) + gv2(i1,i2,i3,0)*ux0 + gv2(i1,i2,i3,1)*uy0   
   fv( 2)=fv( 2) + gv2(i1,i2,i3,0)*vx0 + gv2(i1,i2,i3,1)*vy0   
   fv( 3)=fv( 3) + gv2(i1,i2,i3,0)*qx0 + gv2(i1,i2,i3,1)*qy0   
   fv( 4)=fv( 4) + gv2(i1,i2,i3,0)*px0 + gv2(i1,i2,i3,1)*py0   
                     
   ! estimate gtt (we cannot use gtt if we are not on the boundary)
   gttu0 = (gv(i1,i2,i3,0)-gv2(i1,i2,i3,0))/dt
   gttv0 = (gv(i1,i2,i3,1)-gv2(i1,i2,i3,1))/dt
   fv( 5)=fv( 5) + gttu0*rx0 + gttv0*ry0 + gv2(i1,i2,i3,0)*rtx0 + gv2(i1,i2,i3,1)*rty0 
   fv( 6)=fv( 6) + gttu0*ux0 + gttv0*uy0 + gv2(i1,i2,i3,0)*utx0 + gv2(i1,i2,i3,1)*uty0 
   fv( 7)=fv( 7) + gttu0*vx0 + gttv0*vy0 + gv2(i1,i2,i3,0)*vtx0 + gv2(i1,i2,i3,1)*vty0 
   fv( 8)=fv( 8) + gttu0*qx0 + gttv0*qy0 + gv2(i1,i2,i3,0)*qtx0 + gv2(i1,i2,i3,1)*qty0 
   fv( 9)=fv( 9) + gttu0*px0 + gttv0*py0 + gv2(i1,i2,i3,0)*ptx0 + gv2(i1,i2,i3,1)*pty0 

   ! we need derivatives of the grid velocity:

   gvux0=gv2x22(i1,i2,i3,0)
   gvuy0=gv2y22(i1,i2,i3,0)
   gvvx0=gv2x22(i1,i2,i3,1)
   gvvy0=gv2y22(i1,i2,i3,1)

   fv(10)=fv(10) + gv2(i1,i2,i3,0)*rxx0 + gv2(i1,i2,i3,1)*rxy0 + gvux0*rx0 + gvvx0*ry0 
   fv(11)=fv(11) + gv2(i1,i2,i3,0)*uxx0 + gv2(i1,i2,i3,1)*uxy0 + gvux0*ux0 + gvvx0*uy0   
   fv(12)=fv(12) + gv2(i1,i2,i3,0)*vxx0 + gv2(i1,i2,i3,1)*vxy0 + gvux0*vx0 + gvvx0*vy0   
   fv(13)=fv(13) + gv2(i1,i2,i3,0)*qxx0 + gv2(i1,i2,i3,1)*qxy0 + gvux0*qx0 + gvvx0*qy0   
   fv(14)=fv(14) + gv2(i1,i2,i3,0)*pxx0 + gv2(i1,i2,i3,1)*pxy0 + gvux0*px0 + gvvx0*py0  
                   
   fv(15)=fv(15) + gv2(i1,i2,i3,0)*rxy0 + gv2(i1,i2,i3,1)*ryy0 + gvuy0*rx0 + gvvy0*ry0 
   fv(16)=fv(16) + gv2(i1,i2,i3,0)*uxy0 + gv2(i1,i2,i3,1)*uyy0 + gvuy0*ux0 + gvvy0*uy0   
   fv(17)=fv(17) + gv2(i1,i2,i3,0)*vxy0 + gv2(i1,i2,i3,1)*vyy0 + gvuy0*vx0 + gvvy0*vy0   
   fv(18)=fv(18) + gv2(i1,i2,i3,0)*qxy0 + gv2(i1,i2,i3,1)*qyy0 + gvuy0*qx0 + gvvy0*qy0   
   fv(19)=fv(19) + gv2(i1,i2,i3,0)*pxy0 + gv2(i1,i2,i3,1)*pyy0 + gvuy0*px0 + gvvy0*py0  


 end if



 pt0 = -( u0*px0 + v0*py0 + gamma*p0*(ux0+vy0) -fv(4) )
 ptx0 =-( ux0*px0+vx0*py0 + gamma*px0*(ux0+vy0) + u0*pxx0+v0*pxy0 + gamma*p0*(uxx0+vxy0) -fv(14) )
 pty0 =-( uy0*px0+vy0*py0 + gamma*py0*(ux0+vy0) + u0*pxy0+v0*pyy0 + gamma*p0*(uxy0+vyy0) -fv(19) )

 qt0 = -( u0*qx0 + v0*qy0 + gm1*q0*(ux0+vy0) -fv(3) )
 qtx0 =-( ux0*qx0+vx0*qy0 + gm1*qx0*(ux0+vy0) + u0*qxx0+v0*qxy0 + gm1*q0*(uxx0+vxy0) -fv(13) )
 qty0 =-( uy0*qx0+vy0*qy0 + gm1*qy0*(ux0+vy0) + u0*qxy0+v0*qyy0 + gm1*q0*(uxy0+vyy0) -fv(18) )

 rt0 = -( u0*rx0+v0*ry0 + r0*(ux0+vy0) -fv(0) )
 rtx0= -( ux0*rx0 +rx0*ux0 +vx0*ry0 + rx0*vy0 + u0*rxx0+v0*rxy0 + r0*(uxx0+vxy0) -fv(10) )
 rty0= -( uy0*rx0 +ry0*ux0 +vy0*ry0 + ry0*vy0 + u0*rxy0+v0*ryy0 + r0*(uxy0+vyy0) -fv(15) ) 

 ut0 = -( u0*ux0 + v0*uy0 + px0/r0 -fv(1) )
 utx0= -( ux0*ux0 +u0*uxx0 + vx0*uy0 + v0*uxy0 + pxx0/r0 - px0*rx0/(r0**2) -fv(11) )
 uty0= -( uy0*ux0 +u0*uxy0 + vy0*uy0 + v0*uyy0 + pxy0/r0 - px0*ry0/(r0**2) -fv(16) )

 vt0 = -( u0*vx0 + v0*vy0 + py0/r0 -fv(2) )
 vtx0= -( ux0*vx0 +u0*vxx0 + vx0*vy0 + v0*vxy0 + pxy0/r0 - py0*rx0/(r0**2) -fv(12) )
 vty0= -( uy0*vx0 +u0*vxy0 + vy0*vy0 + v0*vyy0 + pyy0/r0 - py0*ry0/(r0**2) -fv(17) )



 rtt0= -( ut0*rx0+vt0*ry0 + rt0*(ux0+vy0) + u0*rtx0+v0*rty0 + r0*(utx0+vty0) -fv(5) )

 utt0= -( ut0*ux0 + vt0*uy0 + ptx0/r0 + u0*utx0 + v0*uty0 - px0*rt0/(r0**2) -fv(6) )
 vtt0= -( ut0*vx0 + vt0*vy0 + pty0/r0 + u0*vtx0 + v0*vty0 - py0*rt0/(r0**2) -fv(7) )

 ptt0= -( ut0*px0 + vt0*py0 + gamma*pt0*(ux0+vy0) + u0*ptx0 + v0*pty0 + gamma*p0*(utx0+vty0) -fv(9) )
 qtt0= -( ut0*qx0 + vt0*qy0 + gm1*qt0*(ux0+vy0) + u0*qtx0 + v0*qty0 + gm1*q0*(utx0+vty0) -fv(8) )
#endMacro



      subroutine cnsNoSlipBC(nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
          ipar,rpar, u, gv, gtt, mask, x,rsxy, bc, indexRange, exact, uKnown, ierr )         
c========================================================================
c
c     Apply a no-slip wall boundary condition 
c
c  u : solution at time t
c 
c gv (input) : g' -  gridVelocity at time t (for moving grids)
c gtt (input) : g'' - we need the gridAcceleration on the boundaries
c
c========================================================================
      implicit none
      integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b
c     integer *8 exact ! holds pointer to OGFunction
      integer exact ! holds pointer to OGFunction
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
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
      integer is,j1,j2,j3,side,axis,twilightZone,bcOption,knownSolution,numberOfComponents
      integer rc,tc,uc,vc,wc,sc,unc,utc,n
      integer grid,orderOfAccuracy,gridIsMoving,useWhereMask
      integer gridType,isAxisymmetric,numberOfSpecies,radialAxis,axisymmetricWithSwirl,urc,uac
      integer nr(0:1,0:2)

      real sxi,syi,szi,txi,tyi,tzi,rxi,ryi,rzi
      real pn,rho,rhon,nDotGradR,nDotGradS,tp,tpn,rhor,rhos,tps,ps,tpm,pm,pp,pr,tpr
      integer axisp1

      integer is1,is2,is3,js1,js2,js3,ks1,ks2,ks3,s,m,mm

      real t,dt
      real an1,an2,an3,nDotU,aNorm,epsx,gamma
      real dr(0:2),dx(0:2),gravity(0:2)
      real mu,kThermal,Rg,gm1,a43,a13,alpha,sgn,fr

c     real u0s,v0s,w0s,sgn

c     real rra,ura,vra,wra, rsa,usa,vsa,wsa, urra,vrra,wrra, ussa,vssa,wssa, rrsa, ursa,vrsa,wrsa               
c     real rxa,rya,sxa,sya, rxra,ryra,sxra,syra, rxsa,rysa,sxsa,sysa
c     real ra,ua,va,wa,fra,rhot

c     real hx,hy,gm1
      real r0,rx0,ry0,rz0,rxx0,rxy0,ryy0,rxz0,ryz0,rzz0, rt0,rtx0,rty0,rtt0  
      real u0,ux0,uy0,uz0,uxx0,uxy0,uyy0,uxz0,uyz0,uzz0, ut0,utx0,uty0,utt0
      real v0,vx0,vy0,vz0,vxx0,vxy0,vyy0,vxz0,vyz0,vzz0, vt0,vtx0,vty0,vtt0   
      real w0,wx0,wy0,wz0,wxx0,wxy0,wyy0,wxz0,wyz0,wzz0, wt0,wtx0,wty0,wtt0   
      real p0,px0,py0,pz0,pxx0,pxy0,pyy0,pxz0,pyz0,pzz0, pt0,ptx0,pty0,ptt0   
      real q0,qx0,qy0,qz0,qxx0,qxy0,qyy0,qxz0,qyz0,qzz0, qt0,qtx0,qty0,qtt0   
      real T0,Tx0,Ty0,Tz0,Txx0,Txy0,Tyy0,Txz0,Tyz0,Tzz0, Tt0,Ttx0,Tty0,Ttt0   

c     real fv(0:20),uv(0:10),z0,tm,ad2dt
      real ep ! holds the pointer to the TZ function
      integer debug

c     real r1,u1,v1,q1,p1,s1, s0,st0,stt0
c     real ur1,vr1,nDotU1,nDotuv(2),adu(0:10),usp,usm
      integer k1,k2,k3

c     real rr0,rxr0,ryr0
c     real ur0,uxr0,uyr0
c     real vr0,vxr0,vyr0
c     real qr0,qxr0,qyr0
c     real pr0,pxr0,pyr0
c     real utr0,vtr0
c     real u2xr22,u2xs22,u2yr22,u2ys22
c     real gvux0,gvuy0,gvvx0,gvvy0,gttu0,gttv0
c     real s1p,sr

c..................
      integer rectangular,curvilinear
      parameter( rectangular=0, curvilinear=1 )

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
     &     axisymmetric
      parameter( noSlipWall=1,inflowWithVelocityGiven=2,
     & slipWall=4,outflow=5,convectiveOutflow=14,tractionFree=15,
     & inflowWithPandTV=3,
     &  dirichletBoundaryCondition=12,
     &  symmetry=11,axisymmetric=13 )

      integer supersonicFlowInAnExpandingChannel,userDefinedKnownSolution
      parameter( userDefinedKnownSolution=1, supersonicFlowInAnExpandingChannel=2 )

      ! declare variables for difference approximations of u and RX
      declareDifferenceOrder2(u,RX)

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

c     The next macro call will define the difference approximation statement functions
      defineDifferenceOrder2Components1(u,RX)
          
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

      gravity(0)        =rpar(11)  ! new
      gravity(1)        =rpar(12)
      gravity(2)        =rpar(13)
      mu                =rpar(14)
      kThermal          =rpar(15)
      Rg                =rpar(16)

c      write(*,'(" **** cnsNoSlipBC: mu,kThermal,Rg=",3e9.2," gravity=",3f6.1)') \
c          mu,kThermal,Rg,gravity(0),gravity(1),gravity(2)
c      ! ' 

      gm1=gamma-1.
      ad2=10.  ! artificial dissipation

      a43=4./3.
      a13=1./3. 

      do axis=0,2
      do side=0,1
        nr(side,axis)=indexRange(side,axis)
      end do
      end do


      if( nd.eq.2 .and. gridType.eq.rectangular .and. twilightZone.eq.0 )then

        ! *********************************************************************
        ! ******* 2D non-moving, rectangular **********************************
        ! *********************************************************************

        if( gridIsMoving.ne.0 )then
          write(*,'("cnsNoSlipBC:ERROR: gridIsMoving not implemented yet for rectangular")')
          ! '
          stop 6642
        end if

        do axis=0,nd-1
        do side=0,1
        if( bc(side,axis).eq.noSlipWall )then

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

c          beginLoopsNoMask()
c            u(i1,i2,i3,uc)=0.
c            u(i1,i2,i3,vc)=0.
c          endLoopsNoMask()

          ! BC on rho comes from 
          !     p.n/p = r.n/r + T.n/T
          !     p.n = n.( viscous terms + gravity )
          ! Leads to 
          !   r.n = alpha*r 
          !   alpha = p.n/p - T.n/T 

          an1=-is1   ! outward normal=(an1,an2)
          an2=-is2

          beginLoopsNoMask()

            uxx0 = uxx22r(i1,i2,i3,uc)
            uxy0 = uxy22r(i1,i2,i3,uc)
            uyy0 = uyy22r(i1,i2,i3,uc)

            vxx0 = uxx22r(i1,i2,i3,vc)
            vxy0 = uxy22r(i1,i2,i3,vc)
            vyy0 = uyy22r(i1,i2,i3,vc)

            Tx0 = ux22r(i1,i2,i3,tc)
            Ty0 = uy22r(i1,i2,i3,tc)

            px0 = mu*(a43*uxx0+uyy0+a13*vxy0) + gravity(0)
            py0 = mu*(vxx0+a43*vyy0+a13*uxy0) + gravity(1)

            r0 = u(i1,i2,i3,rc)
            T0 = u(i1,i2,i3,tc)
            p0 = r0*Rg*T0
            alpha = (an1*px0+an2*py0)/p0 - (an1*Tx0+an2*Ty0)/T0

            u(i1-is1,i2-is2,i3,rc )=u(i1+is1,i2+is2,i3,rc) + 2.*dx(axis)*alpha 

c     write(*,'(" noSlip: i=(",i2,",",i2,") r0,T0,p0,alpha=",4f7.2," r(ghost)=",f5.2)')\
c            i1,i2,r0,T0,p0,alpha,u(i1-is1,i2-is2,i3,rc )
          ! '

          endLoopsNoMask()

          nr(0,axis)=indexRange(0,axis)  ! reset
          nr(1,axis)=indexRange(1,axis)

        end if
        end do
        end do

      else if( nd.eq.2 .and. gridType.eq.curvilinear .and. twilightZone.eq.0 )then
     
        do axis=0,nd-1
        do side=0,1
        if( bc(side,axis).eq.noSlipWall )then
 
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

          ! write(*,'("apply rho BC from p=r*R*T")')

          beginLoops()

            ! BC on rho comes from 
            !     p.n/p = r.n/r + T.n/T
            !     p.n = n.( viscous terms + gravity )
            ! Leads to 
            !   r.n = alpha*r 
            !   alpha = p.n/p - T.n/T 

            rxi = rsxy(i1,i2,i3,axis,0)
            ryi = rsxy(i1,i2,i3,axis,1)
            ! sxi = rsxy(i1,i2,i3,axisp1,0)
            ! syi = rsxy(i1,i2,i3,axisp1,1)

            an1=-rxi*sgn  ! here is the outward normal 
            an2=-ryi*sgn
            aNorm=sqrt(max(epsx,an1**2+an2**2))
            an1=an1/aNorm
            an2=an2/aNorm

            nDotGradR=an1*rxi+an2*ryi
            ! nDotGradS=an1*sxi+an2*syi

            uxx0 = uxx22(i1,i2,i3,uc)
            uxy0 = uxy22(i1,i2,i3,uc)
            uyy0 = uyy22(i1,i2,i3,uc)

            vxx0 = uxx22(i1,i2,i3,vc)
            vxy0 = uxy22(i1,i2,i3,vc)
            vyy0 = uyy22(i1,i2,i3,vc)

            Tx0 = ux22(i1,i2,i3,tc)
            Ty0 = uy22(i1,i2,i3,tc)

            px0 = mu*(a43*uxx0+uyy0+a13*vxy0) + gravity(0)  
            py0 = mu*(vxx0+a43*vyy0+a13*uxy0) + gravity(1)

            r0 = u(i1,i2,i3,rc)
            T0 = u(i1,i2,i3,tc)
            p0 = r0*Rg*T0
            alpha = (an1*px0+an2*py0)/p0 - (an1*Tx0+an2*Ty0)/T0

            ! fr = rho.n - r.n*rho.r
            !  To evaluate fr, we first compute rho.n using the current (wrong ghost values) of rho 
            !   note: axis=0:  ur = (u(i1+is1,i2+is2,i3,rc)-u(i1-is1,i2-is2,i3,rc))/(2.*sgn*dr(axis))
            rx0 = ux22(i1,i2,i3,rc)
            ry0 = uy22(i1,i2,i3,rc)
            fr = an1*rx0+an2*ry0 - \
                 nDotGradR*(u(i1+is1,i2+is2,i3,rc)-u(i1-is1,i2-is2,i3,rc))/(2.*sgn*dr(axis))

            u(i1-is1,i2-is2,i3,rc)=u(i1+is1,i2+is2,i3,rc)-2.*sgn*dr(axis)/nDotGradR*(alpha - fr )

          endLoops()

          nr(0,axis)=indexRange(0,axis)  ! reset
          nr(1,axis)=indexRange(1,axis)

        end if
        end do
        end do

      else if( nd.eq.3 .and. gridType.eq.rectangular .and. twilightZone.eq.0 )then

        ! *********************************************************************
        ! ******* 3D non-moving, rectangular **********************************
        ! *********************************************************************

        if( gridIsMoving.ne.0 )then
          write(*,'("cnsNoSlipBC:ERROR: gridIsMoving not implemented yet for rectangular")')
          ! '
          stop 6643
        end if

        do axis=0,nd-1
        do side=0,1
        if( bc(side,axis).eq.noSlipWall )then

          nr(0,axis)=indexRange(side,axis)   ! set nr to point to this side
          nr(1,axis)=indexRange(side,axis)

          if( axis.eq.0 )then
            is1 = 1-2*side
            is2=0
            is3=0
          else if( axis.eq.1 )then
            is1=0
            is2=1-2*side
            is3=0
          else
            is1=0
            is2=0
            is3=1-2*side
          end if
          ks1=2*is1
          ks2=2*is2
          ks3=2*is3

          ! BC on rho comes from 
          !     p.n/p = r.n/r + T.n/T
          !     p.n = n.( viscous terms + gravity )
          ! Leads to 
          !   r.n = alpha*r 
          !   alpha = p.n/p - T.n/T 

          an1=-is1   ! outward normal=(an1,an2)
          an2=-is2
          an3=-is3

          beginLoopsNoMask()

            uxx0 = uxx23r(i1,i2,i3,uc)
            uxy0 = uxy23r(i1,i2,i3,uc)
            uxz0 = uxz23r(i1,i2,i3,uc)
            uyy0 = uyy23r(i1,i2,i3,uc)
            uyz0 = uyz23r(i1,i2,i3,uc)
            uzz0 = uzz23r(i1,i2,i3,uc)

            vxx0 = uxx23r(i1,i2,i3,vc)
            vxy0 = uxy23r(i1,i2,i3,vc)
            vxz0 = uxz23r(i1,i2,i3,vc)
            vyy0 = uyy23r(i1,i2,i3,vc)
            vyz0 = uyz23r(i1,i2,i3,vc)
            vzz0 = uzz23r(i1,i2,i3,vc)

            wxx0 = uxx23r(i1,i2,i3,wc)
            wxy0 = uxy23r(i1,i2,i3,wc)
            wxz0 = uxz23r(i1,i2,i3,wc)
            wyy0 = uyy23r(i1,i2,i3,wc)
            wyz0 = uyz23r(i1,i2,i3,wc)
            wzz0 = uzz23r(i1,i2,i3,wc)


            Tx0 = ux23r(i1,i2,i3,tc)
            Ty0 = uy23r(i1,i2,i3,tc)
            Tz0 = uz23r(i1,i2,i3,tc)

            ! grad(p) from the momentum equations on the boundary with u=0 
            px0 = mu*(a43*uxx0+uyy0+uzz0+a13*(vxy0+wxz0)) + gravity(0)
            py0 = mu*(vxx0+a43*vyy0+vzz0+a13*(uxy0+wyz0)) + gravity(1)
            pz0 = mu*(wxx0+wyy0+a43*wzz0+a13*(uxz0+vyz0)) + gravity(2)

            r0 = u(i1,i2,i3,rc)
            T0 = u(i1,i2,i3,tc)
            p0 = r0*Rg*T0
            alpha = (an1*px0+an2*py0+an3*pz0)/p0 - (an1*Tx0+an2*Ty0+an3*Tz0)/T0

            u(i1-is1,i2-is2,i3-is3,rc )=u(i1+is1,i2+is2,i3+is3,rc) + 2.*dx(axis)*alpha 

c     write(*,'(" noSlip: i=(",i2,",",i2,") r0,T0,p0,alpha=",4f7.2," r(ghost)=",f5.2)')\
c            i1,i2,r0,T0,p0,alpha,u(i1-is1,i2-is2,i3,rc )
          ! '

          endLoopsNoMask()

          nr(0,axis)=indexRange(0,axis)  ! reset
          nr(1,axis)=indexRange(1,axis)

        end if
        end do
        end do

      else if( nd.eq.3 .and. gridType.eq.curvilinear .and. twilightZone.eq.0 )then
     
        do axis=0,nd-1
        do side=0,1
        if( bc(side,axis).eq.noSlipWall )then

          nr(0,axis)=indexRange(side,axis)   ! set nr to point to this side
          nr(1,axis)=indexRange(side,axis)

          if( axis.eq.0 )then
            is1 = 1-2*side
            is2=0
            is3=0
          else if( axis.eq.1 )then
            is1=0
            is2=1-2*side
            is3=0
          else
            is1=0
            is2=0
            is3=1-2*side
          end if
          sgn=1-2*side
          ks1=2*is1
          ks2=2*is2
          ks3=2*is3

          ! BC on rho comes from 
          !     p.n/p = r.n/r + T.n/T
          !     p.n = n.( viscous terms + gravity )
          ! Leads to 
          !   r.n = alpha*r 
          !   alpha = p.n/p - T.n/T 

          beginLoopsNoMask()

            rxi = rsxy(i1,i2,i3,axis,0)
            ryi = rsxy(i1,i2,i3,axis,1)
            rzi = rsxy(i1,i2,i3,axis,2)

            an1=-rxi*sgn  ! here is the outward normal 
            an2=-ryi*sgn
            an3=-rzi*sgn

            aNorm=sqrt(max(epsx,an1**2+an2**2+an3**2))
            an1=an1/aNorm
            an2=an2/aNorm
            an2=an3/aNorm

            nDotGradR=an1*rxi+an2*ryi+an3*rzi

            uxx0 = uxx23(i1,i2,i3,uc)
            uxy0 = uxy23(i1,i2,i3,uc)
            uxz0 = uxz23(i1,i2,i3,uc)
            uyy0 = uyy23(i1,i2,i3,uc)
            uyz0 = uyz23(i1,i2,i3,uc)
            uzz0 = uzz23(i1,i2,i3,uc)

            vxx0 = uxx23(i1,i2,i3,vc)
            vxy0 = uxy23(i1,i2,i3,vc)
            vxz0 = uxz23(i1,i2,i3,vc)
            vyy0 = uyy23(i1,i2,i3,vc)
            vyz0 = uyz23(i1,i2,i3,vc)
            vzz0 = uzz23(i1,i2,i3,vc)

            wxx0 = uxx23(i1,i2,i3,wc)
            wxy0 = uxy23(i1,i2,i3,wc)
            wxz0 = uxz23(i1,i2,i3,wc)
            wyy0 = uyy23(i1,i2,i3,wc)
            wyz0 = uyz23(i1,i2,i3,wc)
            wzz0 = uzz23(i1,i2,i3,wc)


            Tx0 = ux23(i1,i2,i3,tc)
            Ty0 = uy23(i1,i2,i3,tc)
            Tz0 = uz23(i1,i2,i3,tc)

            ! grad(p) from the momentum equations on the boundary with u=0 
            px0 = mu*(a43*uxx0+uyy0+uzz0+a13*(vxy0+wxz0)) + gravity(0)
            py0 = mu*(vxx0+a43*vyy0+vzz0+a13*(uxy0+wyz0)) + gravity(1)
            pz0 = mu*(wxx0+wyy0+a43*wzz0+a13*(uxz0+vyz0)) + gravity(2)

            r0 = u(i1,i2,i3,rc)
            T0 = u(i1,i2,i3,tc)
            p0 = r0*Rg*T0
            alpha = (an1*px0+an2*py0+an3*pz0)/p0 - (an1*Tx0+an2*Ty0+an3*Tz0)/T0

            ! fr = rho.n - r.n*rho.r
            !  To evaluate fr, we first compute rho.n using the current (wrong ghost values) of rho 
            rx0 = ux23(i1,i2,i3,rc)
            ry0 = uy23(i1,i2,i3,rc)
            rz0 = uz23(i1,i2,i3,rc)
            fr = an1*rx0+an2*ry0+an3*rz0 - \
                 nDotGradR*(u(i1+is1,i2+is2,i3+is3,rc)-u(i1-is1,i2-is2,i3-is3,rc))/(2.*sgn*dr(axis))

            u(i1-is1,i2-is2,i3-is3,rc)=u(i1+is1,i2+is2,i3+is3,rc)-2.*sgn*dr(axis)/nDotGradR*(alpha-fr)

c     write(*,'(" noSlip: i=(",i2,",",i2,") r0,T0,p0,alpha=",4f7.2," r(ghost)=",f5.2)')\
c            i1,i2,r0,T0,p0,alpha,u(i1-is1,i2-is2,i3,rc )
          ! '

          endLoopsNoMask()

          nr(0,axis)=indexRange(0,axis)  ! reset
          nr(1,axis)=indexRange(1,axis)

        end if
        end do
        end do

      else

      end if


      return
      end



