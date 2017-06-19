! *******************************************************************************
!   Non-reflecting Boundary Condition Utility functions
! *******************************************************************************

! Here are macros that define the planeWave solution
#Include "planeWave.h"

#beginMacro beginLoops()
do i3=n3a,n3b
do i2=n2a,n2b
do i1=n1a,n1b
#endMacro

#beginMacro endLoops()
end do
end do
end do
#endMacro


! ======================================================================
! Loop over faces of the grid and assign points near that boundary
! ======================================================================
#beginMacro beginLoopOverSidesForAdjustingIncidentField()

 ! adjust this many points near the boundary (needs to include width of extrapolation too!)
 ! halfWidth = orderOfAccuracy/2 ** now passed in**

 ! restrict all bounds to fit in the array dimensions: 
 dim(0,0)=nd1a
 dim(1,0)=nd1b
 dim(0,1)=nd2a
 dim(1,1)=nd2b
 dim(0,2)=nd3a
 dim(1,2)=nd3b

 ! gid(size,axis) : holds bounds on the points that still may need adjusting 
 !                : as we assign points near faces, gid will be reduced in size so that we 
 !                : do not adjust points multiple times.  
 gid(0,2)=0
 gid(1,2)=0
 range(0,2)=0
 range(1,2)=0
 do axis=0,nd-1
 do side=0,1
  if( boundaryCondition(side,axis).eq.abcPML )then
   gid(side,axis)=dim(side,axis)
  else
   gid(side,axis)=gridIndexRange(side,axis)-halfWidth*(1-2*side)
  end if
 end do
 end do 


 do axis=0,nd-1
 do side=0,1
  if( boundaryCondition(side,axis).ge.abcEM2 .and. boundaryCondition(side,axis).le.rbcLocal )then
   do a=0,nd-1
   do s=0,1
    range(s,a)=gid(s,a)
   end do
   end do
   if( boundaryCondition(side,axis).eq.abcPML )then
     ! we need to adjust more lines for a PML
     range(  side,axis)=dim(side,axis)
     range(1-side,axis)=gridIndexRange(side,axis)+(numberLinesForPML+halfWidth)*(1-2*side) ! check this 
   else
     range(0,axis)=gridIndexRange(side,axis)-halfWidth
     range(1,axis)=gridIndexRange(side,axis)+halfWidth
   end if 

   ! restrict all bounds to fit in the array dimensions: 
   do a=0,nd-1
   do s=0,1
    range(s,a)=max(dim(0,a),min(dim(1,a), range(s,a) ))
   end do
   end do
  
   ! reduce gid bounds for future tangential directions so pts are not assigned twice
   ! gid(side,axis)=gridIndexRange(side,axis)+(halfWidth+1)*(1-2*side)
   gid(side,axis)=range(1-side,axis)+(1)*(1-2*side)

   n1a=range(0,0)
   n1b=range(1,0)
   n2a=range(0,1)
   n2b=range(1,1)
   n3a=range(0,2)
   n3b=range(1,2)

   if( debug.gt.0 )then
     write(*,'(" abc:adjustIncindent: side,axis,bc=",3i2," n1a,n1b,...=",3(i3,1x))') side,axis,boundaryCondition(side,axis),n1a,n1b,n2a,n2b,n3a,n3b
     write(*,'(" abc:adjustIncindent: gid=",4i4)') gid(0,0),gid(1,0),gid(0,1),gid(1,1)
     ! ' 
   end if

#endMacro

#beginMacro endLoopOverSidesForAdjustingIncidentField()
  end if
 end do
 end do
#endMacro

! These next formulae must match the one in getInitialConditions.bC
#defineMacro AMP2D(x,y,t) (.5*(1.-tanh(beta*twoPi*(nv(0)*((x)-xv0(0))+nv(1)*((y)-xv0(1))-cc*(t)))))
#defineMacro AMP3D(x,y,z,t) (.5*(1.-tanh(beta*twoPi*(nv(0)*((x)-xv0(0))+nv(1)*((y)-xv0(1))+nv(2)*((z)-xv0(2))-cc*(t)))))

! ===================================================================================
! --- Subtract/add the incident wave, before/after applying the non-reflecting BC ---
! OP : "+" or "-" 
! ADJUST : YES or NO to adjust for the bounding box or not
! ===================================================================================
#beginMacro makeAdjustmentForIncidentField(OP,ADJUST)
 if( debug.gt.4 )then
   write(*,'(" adjust OP: kx,ky,kz,eps,cc",5e10.3)') kx,ky,kz,eps,cc
 end if
 amp=1.
 beginLoopOverSidesForAdjustingIncidentField()
   if( nd.eq.2 )then
     beginLoops()
       if( gridType.eq.rectangular )then
         x = xa(0)+i1*dx(0)
         y = xa(1)+i2*dx(1)
       else
         x=xy(i1,i2,i3,0)
         y=xy(i1,i2,i3,1)
       end if
       ! if( debug.gt.1 )then
       !  t0=planeWave2Dhz0(x,y,t-dt)
       !  write(*,'("nrbc: adjust OP: i=",2i3," Hz,true=",2e10.3)') i1,i2,u(i1,i2,i3,hz),t0
       ! end if
       #If #ADJUST eq "YES"
         if( x.ge.icBoundingBox(0,0) .and. x.le.icBoundingBox(1,0) .and. \
             y.ge.icBoundingBox(0,1) .and. y.le.icBoundingBox(1,1) )then
           if( smoothBoundingBox.eq.1 )then
             amp = AMP2D(x,y,t)
           else
             amp=1.
           end if
         ! if( amp.gt. 1.e-9 )then
         !   if( x.gt.1.5 .and. y.gt.-.05 .and. y.lt.0.5 )then
         !     write(*,'(" x,amp=",2e10.2)') x,amp
         !   end if
       #Elif #ADJUST eq "NO"
       #Else
          ! ERROR
          stop 666 
       #End

       u(i1,i2,i3,ex) = u(i1,i2,i3,ex) OP amp*planeWave2Dex0(x,y,t-dt)
       u(i1,i2,i3,ey) = u(i1,i2,i3,ey) OP amp*planeWave2Dey0(x,y,t-dt)
       u(i1,i2,i3,hz) = u(i1,i2,i3,hz) OP amp*planeWave2Dhz0(x,y,t-dt)

       un(i1,i2,i3,ex)= un(i1,i2,i3,ex) OP amp*planeWave2Dex0(x,y,t)
       un(i1,i2,i3,ey)= un(i1,i2,i3,ey) OP amp*planeWave2Dey0(x,y,t)
       un(i1,i2,i3,hz)= un(i1,i2,i3,hz) OP amp*planeWave2Dhz0(x,y,t)

       if( adjustThreeLevels.eq.1 )then
        um(i1,i2,i3,ex) = um(i1,i2,i3,ex) OP amp*planeWave2Dex0(x,y,t-2.*dt)
        um(i1,i2,i3,ey) = um(i1,i2,i3,ey) OP amp*planeWave2Dey0(x,y,t-2.*dt)
        um(i1,i2,i3,hz) = um(i1,i2,i3,hz) OP amp*planeWave2Dhz0(x,y,t-2.*dt)
       end if

       #If #ADJUST eq "YES"
        endif
       #End
     endLoops()
   else
     beginLoops()
       if( gridType.eq.rectangular )then
         x = xa(0)+i1*dx(0)
         y = xa(1)+i2*dx(1)
         z = xa(2)+i3*dx(2)
       else
         x=xy(i1,i2,i3,0)
         y=xy(i1,i2,i3,1)
         z=xy(i1,i2,i3,2)
       end if

       #If #ADJUST eq "YES"
       if( x.ge.icBoundingBox(0,0) .and. x.le.icBoundingBox(1,0) .and. \
           y.ge.icBoundingBox(0,1) .and. y.le.icBoundingBox(1,1) .and. \
           z.ge.icBoundingBox(0,2) .and. z.le.icBoundingBox(1,2) )then
         if( smoothBoundingBox.eq.1 )then
           amp = AMP3D(x,y,z,t)
         else
           amp=1.
         end if
       #End

       u(i1,i2,i3,ex) = u(i1,i2,i3,ex) OP amp*planeWave3Dex0(x,y,z,t-dt)
       u(i1,i2,i3,ey) = u(i1,i2,i3,ey) OP amp*planeWave3Dey0(x,y,z,t-dt)
       u(i1,i2,i3,ez) = u(i1,i2,i3,ez) OP amp*planeWave3Dez0(x,y,z,t-dt)

       un(i1,i2,i3,ex)=un(i1,i2,i3,ex) OP amp*planeWave3Dex0(x,y,z,t)
       un(i1,i2,i3,ey)=un(i1,i2,i3,ey) OP amp*planeWave3Dey0(x,y,z,t)
       un(i1,i2,i3,ez)=un(i1,i2,i3,ez) OP amp*planeWave3Dez0(x,y,z,t)

       if( adjustThreeLevels.eq.1 )then
        um(i1,i2,i3,ex)=um(i1,i2,i3,ex) OP amp*planeWave3Dex0(x,y,z,t-2.*dt)
        um(i1,i2,i3,ey)=um(i1,i2,i3,ey) OP amp*planeWave3Dey0(x,y,z,t-2.*dt)
        um(i1,i2,i3,ez)=um(i1,i2,i3,ez) OP amp*planeWave3Dez0(x,y,z,t-2.*dt)
       end if
       #If #ADJUST eq "YES"
       endif
       #End
     endLoops()
   end if
 endLoopOverSidesForAdjustingIncidentField()
#endMacro


      subroutine adjustForIncident( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
                                    gridIndexRange, um, u, un, mask,rsxy, xy,icBoundingBox,\
                                    bc, boundaryCondition, ipar, rpar, ierr )
! ===================================================================================
!  Non-reflecting BC utility routine: 
!      Subtract/add an incident field near boundaries before/after a non-reflecting BC is applied.
!
!  gridType : 0=rectangular, 1=curvilinear
!  useForcing : 1=use f for RHS to BC
!  side,axis : 0:1 and 0:2
!
!  um : solution at time t-2*dt
!  u : solution at time t-dt
!  un : solution at time t
!  icBoundingBox : i we are given an initial condition bounding box (with positive volume) then only adjust points in this box
!
! ===================================================================================

      implicit none

      integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,\
              n1a,n1b,n2a,n2b,n3a,n3b, ndc, bc,ierr

      real um(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      real un(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      integer gridIndexRange(0:1,0:2)
      real icBoundingBox(0:1,0:2)

      integer ipar(0:*),boundaryCondition(0:1,0:2)
      real rpar(0:*),pwc(0:5)

!     --- local variables ----
      
      integer side,axis,gridType,orderOfAccuracy,orderOfExtrapolation,useForcing,\
        ex,ey,ez,hx,hy,hz,useWhereMask,grid,debug,side1,side2,side3
      real dx(0:2),dr(0:2),xa(0:2)
      real t,ep,dt,c      
      real dxa,dya,dza
      integer axisp1,axisp2,i1,i2,i3,is1,is2,is3,js1,js2,js3,ks1,ks2,ks3,is
      integer extra,extra1a,extra1b,extra2a,extra2b,extra3a,extra3b,numberOfGhostPoints
      integer edgeDirection,sidea,sideb,sidec,bc1,bc2,numberLinesForPML

      real p0,p2,q0,q2,c1abcem2,c2abcem2
      real an1,an2,an3,aNorm,epsX

      real rx0,ry0,rz0 , rxx0,ryy0, rzz0 
      real dr0,cxt,cxx,cyy,czz,cm1,g,bxx,byy,bzz
      real rxNorm, rxNormSq, Dn2, Lu, ur0,urr0, unr0, unrr0
      real ux0,uy0,uz0, uxx0,uyy0,uzz0
      real unx0,uny0,unz0, unxx0,unyy0,unzz0
      real t0,t1,t2

      real eps,mu,kx,ky,kz,slowStartInterval,twoPi,cc

      integer range(0:1,0:2), gid(0:1,0:2), dim(0:1,0:2), halfWidth, s,a
      integer adjustForIncidentField,adjustThreeLevels
      logical adjustForBoundingBox
      real x,y,z

      ! parameters for tanh() in smooth transition for IC bounding box:
      real amp, beta, nv(0:2), xv0(0:2)
      integer smoothBoundingBox

      ! boundary conditions parameters
      #Include "bcDefineFortranInclude.h"

      integer rectangular,curvilinear
      parameter(\
        rectangular=0,\
        curvilinear=1)


!$$$c     --- start statement function ----
!$$$      integer kd,m,n
!$$$      real rx,ry,rz,sx,sy,sz,tx,ty,tz
!$$$      ! include 'declareDiffOrder2f.h'
!$$$      ! include 'declareDiffOrder4f.h'
!$$$c*      declareDifferenceOrder2(u,RX)
!$$$c*      declareDifferenceOrder4(u,RX)
!$$$      declareDifferenceOrder2(u,RX)
!$$$      declareDifferenceOrder2(un,none)
!$$$
!$$$      declareDifferenceOrder4(u,RX)
!$$$#Include "declareJacobianDerivatives.h"
!$$$
!$$$c.......statement functions for jacobian
!$$$      rx(i1,i2,i3)=rsxy(i1,i2,i3,0,0)
!$$$      ry(i1,i2,i3)=rsxy(i1,i2,i3,0,1)
!$$$      rz(i1,i2,i3)=rsxy(i1,i2,i3,0,2)
!$$$      sx(i1,i2,i3)=rsxy(i1,i2,i3,1,0)
!$$$      sy(i1,i2,i3)=rsxy(i1,i2,i3,1,1)
!$$$      sz(i1,i2,i3)=rsxy(i1,i2,i3,1,2)
!$$$      tx(i1,i2,i3)=rsxy(i1,i2,i3,2,0)
!$$$      ty(i1,i2,i3)=rsxy(i1,i2,i3,2,1)
!$$$      tz(i1,i2,i3)=rsxy(i1,i2,i3,2,2)
!$$$
!$$$
!$$$c     The next macro call will define the difference approximation statement functions
!$$$      defineDifferenceOrder2Components1(u,RX)
!$$$      defineDifferenceOrder2Components1(un,none)
!$$$      defineDifferenceOrder4Components1(u,RX)
!$$$
!$$$#Include "jacobianDerivatives.h"

!............... end statement functions

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
     
      adjustForIncidentField=ipar(25)
      numberLinesForPML    =ipar(26)
      adjustThreeLevels    =ipar(27)

      halfWidth            =ipar(31) ! *new* June 20, 2016 *wdh*
      smoothBoundingBox    =ipar(35) ! smooth the IC at the bounding box edge

      dx(0)                =rpar(0)
      dx(1)                =rpar(1)
      dx(2)                =rpar(2)
      dr(0)                =rpar(3)
      dr(1)                =rpar(4)
      dr(2)                =rpar(5)
      t                    =rpar(6)
      ep                   =rpar(7) ! pointer for exact solution
      dt                   =rpar(8)
      c                    =rpar(9)
     
      eps                  =rpar(10)
      mu                   =rpar(11)
      kx                   =rpar(12)  ! for plane wave forcing
      ky                   =rpar(13)
      kz                   =rpar(14)
      slowStartInterval    =rpar(15)

      pwc(0)               =rpar(20) ! coeffs. for plane wave 
      pwc(1)               =rpar(21)
      pwc(2)               =rpar(22)
      pwc(3)               =rpar(23)
      pwc(4)               =rpar(24)
      pwc(5)               =rpar(25)
      
      xa(0)                =rpar(26)  ! for rectangular grids
      xa(1)                =rpar(27)
      xa(2)                =rpar(28)

      ! parameters for tanh() in smooth transition for IC bounding box:
      beta = rpar(29)
      nv(0)  =rpar(30)
      nv(1)  =rpar(31)
      nv(2)  =rpar(32)
      xv0(0) =rpar(33)
      xv0(1) =rpar(34)
      xv0(2) =rpar(35)

      

      if( abs(pwc(0))+abs(pwc(1))+abs(pwc(2)) .eq. 0. )then
        ! sanity check
        stop 12345
      end if
      if( debug.gt.1 )then
        write(*,'(" adjustForIncident",i2,": **START** grid=",i4," side,axis=",2i2)') adjustForIncidentField,grid,side,axis
        ! ' 
      end if
     
      ! If we are given an initial condition bounding box then only adjust points in this box
      if( icBoundingBox(0,0) .lt. icBoundingBox(1,0) )then
        adjustForBoundingBox=.true.
        ! write(*,'(" adjustForIncident: adjustForBoundingBox, grid=",i4," t=",e9.3)') grid,t
      else
        adjustForBoundingBox=.false.
      end if
      ! write(*,'(" adjustForIncident: icbb=[",e8.2,",",e8.2,"][",e8.2,",",e8.2,"][",e8.2,",",e8.2,"]")') icBoundingBox(0,0),icBoundingBox(1,0),\
      !    icBoundingBox(0,1),icBoundingBox(1,1),icBoundingBox(0,2),icBoundingBox(1,2)

      ! adjustForBoundingBox=.false.

      ! for plane wave forcing 
      twoPi=8.*atan2(1.,1.)
      cc= c*sqrt( kx*kx+ky*ky+kz*kz )

      if( adjustForIncidentField.eq.-1 )then
        ! --- Subtract off the incident wave, before applying the non-reflecting BC ---
        if( adjustForBoundingBox )then
          makeAdjustmentForIncidentField(-,YES)
        else
          makeAdjustmentForIncidentField(-,NO)
        end if
      else if( adjustForIncidentField.eq.1 )then
        ! --- Add back the incident wave, before applying the non-reflecting BC ---
        if( adjustForBoundingBox )then
          makeAdjustmentForIncidentField(+,YES)
        else
          makeAdjustmentForIncidentField(+,NO)
        end if
      else
        write(*,'(" adjustForIncident: unexpected value for adjustForIncidentField=",i6)') adjustForIncidentField
        ! ' 
        stop 20031
      end if

      return
      end
