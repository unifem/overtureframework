c *******************************************************************************
c   Absorbing boundary conditions
c *******************************************************************************

c These next include files will define the macros that will define the difference approximations
c The actual macro is called below
#Include "defineDiffOrder2f.h"
#Include "defineDiffOrder4f.h"


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

c ************************************************************************************************
c  This macro is used for looping over the faces of a grid to assign booundary conditions
c
c extra: extra points to assign
c          Case 1: extra=numberOfGhostPoints -- for assigning extended boundaries
c          Case 2: extra=-1 -- for assigning ghost points but not including extended boundaries
c numberOfGhostPoints : number of ghost points (1 for 2nd order, 2 for fourth-order ...)
c ***********************************************************************************************
#beginMacro beginLoopOverSides(extra,numberOfGhostPoints)
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
   extra1a=max(0,extra1a) ! over-ride extra=-1 : assign ends in periodic directions
   extra1b=extra1a
 else
   if( boundaryCondition(0,0).eq.0 )then
     extra1a=numberOfGhostPoints  ! include interpolation points since we assign ghost points outside these
   end if
   if( boundaryCondition(1,0).eq.0 )then
     extra1b=numberOfGhostPoints
   end if
 end if
 if( boundaryCondition(0,1).lt.0 )then
  extra2a=max(0,extra2a) ! over-ride extra=-1 : assign ends in periodic directions
  extra2b=extra2a
 else 
   if( boundaryCondition(0,1).eq.0 )then
     extra2a=numberOfGhostPoints
   end if
   if( boundaryCondition(1,1).eq.0 )then
     extra2b=numberOfGhostPoints
   end if
 end if
 if(  nd.eq.3 .and. boundaryCondition(0,2).lt.0 )then
  extra3a=max(0,extra3a) ! over-ride extra=-1 : assign ends in periodic directions
  extra3b=extra3a
 else 
   if( boundaryCondition(0,2).eq.0 )then
     extra3a=numberOfGhostPoints
   end if
   if( boundaryCondition(1,2).eq.0 )then
     extra3b=numberOfGhostPoints
   end if
 end if

 do axis=0,nd-1
 do side=0,1

   if( boundaryCondition(side,axis).ge.abcEM2 .and. boundaryCondition(side,axis).le.abc5 )then

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

#endMacro

#beginMacro endLoopOverSides()
   end if
 end do
 end do
#endMacro

c ========================================================================
c Begin loop over edges in 3D
c ========================================================================
#beginMacro beginEdgeLoops()
 do edgeDirection=0,2 ! direction parallel to the edge
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

#endMacro

#beginMacro endEdgeLoops()
 end do ! end sideb
 end do ! end sidea
 end do ! end edgeDirection
#endMacro


! ABC - Engquist Majda order 2
! This is only a first order in time approx.
! Generalized form:
! u.xt = c1abcem2*u.xx + c2abcem2*( u.yy + u.zz )
!   Taylor: p0=1 p2=-1/2
!   Cheby:  p0=1.00023, p2=-.515555

#defineMacro ABCEM2Xa(i1,i2,i3,cc) un(i1+is1,i2+is2,i3+is3,cc) \
                    - (u(i1+is1,i2+is2,i3+is3,cc)-u(i1-is1,i2-is2,i3-is3,cc))\
                    - (2.*dxa*dt)*( c1abcem2*uxx22r(i1,i2,i3,cc) + c2abcem2*uyy22r(i1,i2,i3,cc) )
#defineMacro ABCEM2Ya(i1,i2,i3,cc) un(i1+is1,i2+is2,i3+is3,cc) \
                    - (u(i1+is1,i2+is2,i3+is3,cc)-u(i1-is1,i2-is2,i3-is3,cc))\
                    - (2.*dya*dt)*( c1abcem2*uyy22r(i1,i2,i3,cc) + c2abcem2*uxx22r(i1,i2,i3,cc) )

#defineMacro ABCEM23DXa(i1,i2,i3,is1,is2,is3,cc) un(i1+is1,i2+is2,i3+is3,cc) \
                    - (u(i1+is1,i2+is2,i3+is3,cc)-u(i1-is1,i2-is2,i3-is3,cc))\
                    - (2.*dxa*dt)*( c1abcem2*uxx23r(i1,i2,i3,cc) + c2abcem2*(uyy23r(i1,i2,i3,cc)+uzz23r(i1,i2,i3,cc)) )
#defineMacro ABCEM23DYa(i1,i2,i3,is1,is2,is3,cc) un(i1+is1,i2+is2,i3+is3,cc) \
                    - (u(i1+is1,i2+is2,i3+is3,cc)-u(i1-is1,i2-is2,i3-is3,cc))\
                    - (2.*dya*dt)*( c1abcem2*uyy23r(i1,i2,i3,cc) + c2abcem2*(uxx23r(i1,i2,i3,cc)+uzz23r(i1,i2,i3,cc)) )
#defineMacro ABCEM23DZa(i1,i2,i3,is1,is2,is3,cc) un(i1+is1,i2+is2,i3+is3,cc) \
                    - (u(i1+is1,i2+is2,i3+is3,cc)-u(i1-is1,i2-is2,i3-is3,cc))\
                    - (2.*dza*dt)*( c1abcem2*uzz23r(i1,i2,i3,cc) + c2abcem2*(uxx23r(i1,i2,i3,cc)+uyy23r(i1,i2,i3,cc)) )

! Here is a 2nd-order in time approx
#defineMacro ABCEM2X(i1,i2,i3,cc) (un(i1+is1,i2+is2,i3+is3,cc) \
                    - (u(i1+is1,i2+is2,i3+is3,cc)-u(i1-is1,i2-is2,i3-is3,cc))\
         - (dxa*dt)*( c1abcem2*uxx22r(i1,i2,i3,cc) + c2abcem2*uyy22r(i1,i2,i3,cc) \
                     +c1abcem2*(                    -2.*un(i1,i2,i3,cc)+un(i1+is1,i2,i3,cc))/dxa**2 \
                     +c2abcem2*( un(i1  ,i2-1,i3,cc)-2.*un(i1,i2,i3,cc)+un(i1  ,i2+1,i3,cc))/dx(1)**2 )\
                                  )/(1.+c1abcem2*dt/dxa)
#defineMacro ABCEM2Y(i1,i2,i3,cc) (un(i1+is1,i2+is2,i3+is3,cc) \
                    - (u(i1+is1,i2+is2,i3+is3,cc)-u(i1-is1,i2-is2,i3-is3,cc))\
         - (dya*dt)*( c1abcem2*uyy22r(i1,i2,i3,cc) + c2abcem2*uxx22r(i1,i2,i3,cc) \
                     +c1abcem2*(                    -2.*un(i1,i2,i3,cc)+un(i1,i2+is2,i3,cc))/dya**2 \
                     +c2abcem2*( un(i1-1,i2  ,i3,cc)-2.*un(i1,i2,i3,cc)+un(i1+1,i2  ,i3,cc))/dx(0)**2 )\
                                  )/(1.+c1abcem2*dt/dya)

#defineMacro ABCEM23DX(i1,i2,i3,cc) (un(i1+is1,i2+is2,i3+is3,cc) \
                    - (u(i1+is1,i2+is2,i3+is3,cc)-u(i1-is1,i2-is2,i3-is3,cc))\
         - (dxa*dt)*( c1abcem2*uxx23r(i1,i2,i3,cc) + c2abcem2*(uyy23r(i1,i2,i3,cc)+uzz23r(i1,i2,i3,cc)) \
                     +c1abcem2*(                    -2.*un(i1,i2,i3,cc)+un(i1+is1,i2,i3,cc))/dxa**2 \
                     +c2abcem2*( un(i1  ,i2-1,i3,cc)-2.*un(i1,i2,i3,cc)+un(i1  ,i2+1,i3,cc))/dx(1)**2 \
                     +c2abcem2*( un(i1  ,i2,i3-1,cc)-2.*un(i1,i2,i3,cc)+un(i1  ,i2,i3+1,cc))/dx(2)**2 )\
                                  )/(1.+c1abcem2*dt/dxa)
#defineMacro ABCEM23DY(i1,i2,i3,cc) (un(i1+is1,i2+is2,i3+is3,cc) \
                    - (u(i1+is1,i2+is2,i3+is3,cc)-u(i1-is1,i2-is2,i3-is3,cc))\
         - (dya*dt)*( c1abcem2*uyy23r(i1,i2,i3,cc) + c2abcem2*(uxx23r(i1,i2,i3,cc)+uzz23r(i1,i2,i3,cc)) \
                     +c1abcem2*(                    -2.*un(i1,i2,i3,cc)+un(i1,i2+is2,i3,cc))/dya**2 \
                     +c2abcem2*( un(i1-1,i2  ,i3,cc)-2.*un(i1,i2,i3,cc)+un(i1+1,i2  ,i3,cc))/dx(0)**2 \
                     +c2abcem2*( un(i1  ,i2,i3-1,cc)-2.*un(i1,i2,i3,cc)+un(i1  ,i2,i3+1,cc))/dx(2)**2 )\
                                  )/(1.+c1abcem2*dt/dya)

#defineMacro ABCEM23DZ(i1,i2,i3,cc) (un(i1+is1,i2+is2,i3+is3,cc) \
                    - (u(i1+is1,i2+is2,i3+is3,cc)-u(i1-is1,i2-is2,i3-is3,cc))\
         - (dza*dt)*( c1abcem2*uzz23r(i1,i2,i3,cc) +.5*(uxx23r(i1,i2,i3,cc)+uyy23r(i1,i2,i3,cc)) \
                     +c1abcem2*(                    -2.*un(i1,i2,i3,cc)+un(i1,i2,i3+is3,cc))/dza**2 \
                     +c2abcem2*( un(i1-1,i2  ,i3,cc)-2.*un(i1,i2,i3,cc)+un(i1+1,i2  ,i3,cc))/dx(0)**2 \
                     +c2abcem2*( un(i1  ,i2-1,i3,cc)-2.*un(i1,i2,i3,cc)+un(i1  ,i2+1,i3,cc))/dx(1)**2 )\
                                  )/(1.+c1abcem2*dt/dza)


#beginMacro extrapLine2Order4(i1,i2,i3,is1,is2,is3,cc)
  un(i1-2*is1,i2-2*is2,i3-2*is3,cc)=4.*un(i1-is1,i2-is2,i3-is3,cc)-6.*un(i1,i2,i3,cc)\
                                   +4.*un(i1+is1,i2+is2,i3+is3,cc)-un(i1+2*is1,i2+2*is2,i3+2*is3,cc)
#endMacro




      subroutine abcSolidMechanics( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
                               ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,\
                               gridIndexRange, u, un, f,mask,rsxy, xy,\
                               bc, boundaryCondition, ipar, rpar, ierr )
c ===================================================================================
c  Absorbing boundary conditions for Solid Mechanics
c
c  gridType : 0=rectangular, 1=curvilinear
c  useForcing : 1=use f for RHS to BC
c  side,axis : 0:1 and 0:2
c
c  u : solution at time t-dt
c  un : solution at time t (apply BC to this solution)
c
c ===================================================================================

      implicit none

      integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,\
              n1a,n1b,n2a,n2b,n3a,n3b, ndc, bc,ierr

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      real un(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      real f(ndf1a:ndf1b,ndf2a:ndf2b,ndf3a:ndf3b,0:*)
      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      integer gridIndexRange(0:1,0:2)

      integer ipar(0:*),boundaryCondition(0:1,0:2)
      real rpar(0:*)

c     --- local variables ----
      
      integer side,axis,gridType,orderOfAccuracy,orderOfExtrapolation,useForcing,\
        ex,ey,ez,hx,hy,hz,useWhereMask,grid,debug,side1,side2,side3
      real dx(0:2),dr(0:2),t,ep,dt,c      
      real dxa,dya,dza
      integer axisp1,axisp2,i1,i2,i3,is1,is2,is3,js1,js2,js3,ks1,ks2,ks3
      integer extra,extra1a,extra1b,extra2a,extra2b,extra3a,extra3b,numberOfGhostPoints
      integer edgeDirection,sidea,sideb,sidec,bc1,bc2

      real p0,p2,q0,q2,c1abcem2,c2abcem2

      ! boundary conditions parameters
      #Include "bcDefineFortranInclude.h"

      integer rectangular,curvilinear
      parameter(\
        rectangular=0,\
        curvilinear=1)


c     --- start statement function ----
      integer kd,m,n
      real rx,ry,rz,sx,sy,sz,tx,ty,tz
      ! include 'declareDiffOrder2f.h'
      ! include 'declareDiffOrder4f.h'
      declareDifferenceOrder2(u,RX)
      declareDifferenceOrder4(u,RX)

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
      defineDifferenceOrder4Components1(u,RX)

c............... end statement functions

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
     
      if( debug.gt.1 )then
        write(*,'(" abcMaxwell: **START** grid=",i4," side,axis=",2i2)') grid,side,axis
      end if
     
      ! Engquist-Majda 2nd-order
      !    u.xt = (1/c)*u.tt - c/2 * (u.yy + u.zz)   at x=0
      !         = c*( u.xx + .5*( u.yy + u.zz ) 
     
      ! We need un : u(t+dt) 
      !         u  : u(t)

      ! Generalized form:
      ! u.xt = c1abcem2*u.xx + c2abcem2*( u.yy + u.zz )
      !   Taylor: p0=1 p2=-1/2
      !   Cheby:  p0=1.00023, p2=-.515555
      p0=1.  
      p2=-.5
      ! p0=1.00023   !   Cheby on a subinterval
      ! p2=-.515555  !   Cheby on a subinterval
      c1abcem2=c*p0
      c2abcem2=c*(p0+p2)


      extra=-1  ! no need to do corners -- these are already done in another way
      extra=0 ! re-compute corners
      numberOfGhostPoints=orderOfAccuracy/2

      dxa=dx(0) 
      dya=dx(1) 
      dza=dx(2) 

      ! ------------------------------------------------------------------------
      ! ------------------Corners-----------------------------------------------
      ! ------------------------------------------------------------------------

      if( nd.eq.2 )then
       i3=gridIndexRange(0,2)
       do side1=0,1
       do side2=0,1
        if( boundaryCondition(side1,0).ge.abcEM2 .and. boundaryCondition(side1,0).le.abc5 .and. \
            boundaryCondition(side2,1).ge.abcEM2 .and. boundaryCondition(side2,1).le.abc5 )then

          i1=gridIndexRange(side1,0) ! (i1,i2,i3)=corner point
          i2=gridIndexRange(side2,1)

          ! write(*,'(" ABC:set corner: grid,side1,side2,i1,i2=",3i3,2i5)') grid,side1,side2,i1,i2

          is1=1-2*side1
          is2=0
          is3=0

          ! Use first-order-in-time formula since it doesn't require other ghost point at new time (un)
          un(i1-is1,i2-is2,i3-is3,ex)=ABCEM2Xa(i1,i2,i3,ex)
          un(i1-is1,i2-is2,i3-is3,ey)=ABCEM2Xa(i1,i2,i3,ey)
          un(i1-is1,i2-is2,i3-is3,hz)=ABCEM2Xa(i1,i2,i3,hz)
          
          is1=0
          is2=1-2*side2

          un(i1-is1,i2-is2,i3-is3,ex)=ABCEM2Ya(i1,i2,i3,ex)
          un(i1-is1,i2-is2,i3-is3,ey)=ABCEM2Ya(i1,i2,i3,ey)
          un(i1-is1,i2-is2,i3-is3,hz)=ABCEM2Ya(i1,i2,i3,hz)

          ! extrap corner -- could do better
          is1=1-2*side1
          is2=1-2*side2
          un(i1-is1,i2-is2,i3,ex)=2.*un(i1,i2,i3,ex)-un(i1+is1,i2+is2,i3,ex)
          un(i1-is1,i2-is2,i3,ey)=2.*un(i1,i2,i3,ey)-un(i1+is1,i2+is2,i3,ey)
          un(i1-is1,i2-is2,i3,hz)=                   un(i1+is1,i2+is2,i3,hz)

          if( orderOfAccuracy.eq.4 )then

           is1=1-2*side1
           is2=0
           extrapLine2Order4(i1,i2,i3,is1,is2,is3,ex)
           extrapLine2Order4(i1,i2,i3,is1,is2,is3,ey)
           extrapLine2Order4(i1,i2,i3,is1,is2,is3,hz)

           is1=0
           is2=1-2*side2
           
           extrapLine2Order4(i1,i2,i3,is1,is2,is3,ex)
           extrapLine2Order4(i1,i2,i3,is1,is2,is3,ey)
           extrapLine2Order4(i1,i2,i3,is1,is2,is3,hz)

           is1=1-2*side1
           is2=1-2*side2
           
           extrapLine2Order4(i1,i2,i3,is1,is2,is3,ex)
           extrapLine2Order4(i1,i2,i3,is1,is2,is3,ey)
           extrapLine2Order4(i1,i2,i3,is1,is2,is3,hz)

          end if


        end if
       end do          
       end do          

      else ! ***** 3D *****

       beginEdgeLoops()
        if( bc1.ge.abcEM2 .and. bc1.le.abc5 .and. \
            bc2.ge.abcEM2 .and. bc2.le.abc5 )then

         if( edgeDirection.eq.0 )then

          i2=n2a
          i3=n3a
          do i1=n1a,n1b
            ! Use first-order-in-time formula since it doesn't require other ghost point at new time (un)
            un(i1,i2,i3-is3,ex)=ABCEM23DZa(i1,i2,i3,0,0,is3,ex)
            un(i1,i2,i3-is3,ey)=ABCEM23DZa(i1,i2,i3,0,0,is3,ey)
            un(i1,i2,i3-is3,ez)=ABCEM23DZa(i1,i2,i3,0,0,is3,ez)
          
            un(i1,i2-is2,i3,ex)=ABCEM23DYa(i1,i2,i3,0,is2,0,ex)
            un(i1,i2-is2,i3,ey)=ABCEM23DYa(i1,i2,i3,0,is2,0,ey)
            un(i1,i2-is2,i3,ez)=ABCEM23DYa(i1,i2,i3,0,is2,0,ez)

            ! extrap edge-corner point -- could do better
            un(i1,i2-is2,i3-is3,ex)=                   un(i1,i2+is2,i3+is3,ex)
            un(i1,i2-is2,i3-is3,ey)=2.*un(i1,i2,i3,ex)-un(i1,i2+is2,i3+is3,ey)
            un(i1,i2-is2,i3-is3,ez)=2.*un(i1,i2,i3,ez)-un(i1,i2+is2,i3+is3,ez)

            if( orderOfAccuracy.eq.4 )then
             extrapLine2Order4(i1,i2,i3,0,0,is3,ex)
             extrapLine2Order4(i1,i2,i3,0,0,is3,ey)
             extrapLine2Order4(i1,i2,i3,0,0,is3,hz)

             extrapLine2Order4(i1,i2,i3,0,is2,0,ex)
             extrapLine2Order4(i1,i2,i3,0,is2,0,ey)
             extrapLine2Order4(i1,i2,i3,0,is2,0,hz)

             extrapLine2Order4(i1,i2,i3,is1,is2,is3,ex)
             extrapLine2Order4(i1,i2,i3,is1,is2,is3,ey)
             extrapLine2Order4(i1,i2,i3,is1,is2,is3,hz)
           end if

          end do

         else if( edgeDirection.eq.1 )then

          i1=n1a
          i3=n3a
          do i2=n2a,n2b
            ! Use first-order-in-time formula since it doesn't require other ghost point at new time (un)
            un(i1-is1,i2,i3,ex)=ABCEM23DXa(i1,i2,i3,is1,0,0,ex)
            un(i1-is1,i2,i3,ey)=ABCEM23DXa(i1,i2,i3,is1,0,0,ey)
            un(i1-is1,i2,i3,ez)=ABCEM23DXa(i1,i2,i3,is1,0,0,ez)

            un(i1,i2,i3-is3,ex)=ABCEM23DZa(i1,i2,i3,0,0,is3,ex)
            un(i1,i2,i3-is3,ey)=ABCEM23DZa(i1,i2,i3,0,0,is3,ey)
            un(i1,i2,i3-is3,ez)=ABCEM23DZa(i1,i2,i3,0,0,is3,ez)
          
            un(i1-is1,i2,i3-is3,ex)=2.*un(i1,i2,i3,ex)-un(i1+is1,i2,i3+is3,ex)
            un(i1-is1,i2,i3-is3,ey)=                   un(i1+is1,i2,i3+is3,ey)
            un(i1-is1,i2,i3-is3,ez)=2.*un(i1,i2,i3,ez)-un(i1+is1,i2,i3+is3,ez)

            if( orderOfAccuracy.eq.4 )then
             extrapLine2Order4(i1,i2,i3,is1,0,0,ex)
             extrapLine2Order4(i1,i2,i3,is1,0,0,ey)
             extrapLine2Order4(i1,i2,i3,is1,0,0,hz)

             extrapLine2Order4(i1,i2,i3,0,0,is3,ex)
             extrapLine2Order4(i1,i2,i3,0,0,is3,ey)
             extrapLine2Order4(i1,i2,i3,0,0,is3,hz)

             extrapLine2Order4(i1,i2,i3,is1,is2,is3,ex)
             extrapLine2Order4(i1,i2,i3,is1,is2,is3,ey)
             extrapLine2Order4(i1,i2,i3,is1,is2,is3,hz)
            end if

          end do

         else if( edgeDirection.eq.2 )then
          ! write(*,'(" ABC:set corner: grid,side1,side2,i1,i2=",3i3,2i5)') grid,side1,side2,i1,i2
          i1=n1a
          i2=n2a
          do i3=n3a,n3b
            ! Use first-order-in-time formula since it doesn't require other ghost point at new time (un)
            un(i1-is1,i2,i3,ex)=ABCEM23DXa(i1,i2,i3,is1,0,0,ex)
            un(i1-is1,i2,i3,ey)=ABCEM23DXa(i1,i2,i3,is1,0,0,ey)
            un(i1-is1,i2,i3,ez)=ABCEM23DXa(i1,i2,i3,is1,0,0,ez)
          
            un(i1,i2-is2,i3,ex)=ABCEM23DYa(i1,i2,i3,0,is2,0,ex)
            un(i1,i2-is2,i3,ey)=ABCEM23DYa(i1,i2,i3,0,is2,0,ey)
            un(i1,i2-is2,i3,ez)=ABCEM23DYa(i1,i2,i3,0,is2,0,ez)

            ! extrap edge-corner point -- could do better
            un(i1-is1,i2-is2,i3,ex)=2.*un(i1,i2,i3,ex)-un(i1+is1,i2+is2,i3,ex)
            un(i1-is1,i2-is2,i3,ey)=2.*un(i1,i2,i3,ey)-un(i1+is1,i2+is2,i3,ey)
            un(i1-is1,i2-is2,i3,ez)=                   un(i1+is1,i2+is2,i3,ez)

            if( orderOfAccuracy.eq.4 )then
             extrapLine2Order4(i1,i2,i3,is1,0,0,ex)
             extrapLine2Order4(i1,i2,i3,is1,0,0,ey)
             extrapLine2Order4(i1,i2,i3,is1,0,0,hz)

             extrapLine2Order4(i1,i2,i3,0,is2,0,ex)
             extrapLine2Order4(i1,i2,i3,0,is2,0,ey)
             extrapLine2Order4(i1,i2,i3,0,is2,0,hz)

             extrapLine2Order4(i1,i2,i3,is1,is2,is3,ex)
             extrapLine2Order4(i1,i2,i3,is1,is2,is3,ey)
             extrapLine2Order4(i1,i2,i3,is1,is2,is3,hz)
            end if
          end do

         end if ! end if edgeDirection
        end if ! bc
       endEdgeLoops()

       ! ***** vertices *****
       !  normal-direction:     u -> +u
       !  tangential-direction  u -> -u
       !     u(-1,-1,-1) = +u(1,1,1)
       do side3=0,1
       do side2=0,1
       do side1=0,1
        if( boundaryCondition(side1,0).ge.abcEM2 .and. boundaryCondition(side1,0).le.abc5 .and. \
            boundaryCondition(side2,1).ge.abcEM2 .and. boundaryCondition(side2,1).le.abc5 .and. \
            boundaryCondition(side3,2).ge.abcEM2 .and. boundaryCondition(side3,2).le.abc5 )then
         i1=gridIndexRange(side1,0)
         i2=gridIndexRange(side2,1)
         i3=gridIndexRange(side3,2)

         is1=1-2*side1
         is2=1-2*side2
         is3=1-2*side3

         un(i1-is1,i2-is2,i3-is3,ex)=un(i1+is1,i2+is2,i3+is3,ex)
         un(i1-is1,i2-is2,i3-is3,ey)=un(i1+is1,i2+is2,i3+is3,ey)
         un(i1-is1,i2-is2,i3-is3,ez)=un(i1+is1,i2+is2,i3+is3,ez)
        end if
       end do
       end do
       end do

      end if

      ! -------------------------------------------------------------------------
      ! ------------------Loop over Sides----------------------------------------
      ! -------------------------------------------------------------------------
      beginLoopOverSides(extra,numberOfGhostPoints)


       if( gridType.eq.rectangular .and. orderOfACcuracy.eq.2 )then
        ! ***********************************************
        ! ************rectangular grid*******************
        ! ***********************************************

       
        ! write(*,'(" Apply abcEM2: grid,side,axis=",3i3," dt,c=",2e12.3)') grid,side,axis,dt,c
        if( nd.eq.2 )then
         if( axis.eq.0 )then
          beginLoops()
       
           un(i1-is1,i2-is2,i3-is3,ex)=ABCEM2X(i1,i2,i3,ex)
           un(i1-is1,i2-is2,i3-is3,ey)=ABCEM2X(i1,i2,i3,ey)
           un(i1-is1,i2-is2,i3-is3,hz)=ABCEM2X(i1,i2,i3,hz)
       
          endLoops()
         else if( axis.eq.1 )then
          beginLoops()
           un(i1-is1,i2-is2,i3-is3,ex)=ABCEM2Y(i1,i2,i3,ex)
           un(i1-is1,i2-is2,i3-is3,ey)=ABCEM2Y(i1,i2,i3,ey)
           un(i1-is1,i2-is2,i3-is3,hz)=ABCEM2Y(i1,i2,i3,hz)
          endLoops()
         else
          stop 94677
         end if

        else ! ***** 3D *****
         if( axis.eq.0 )then
          beginLoops()
           un(i1-is1,i2-is2,i3-is3,ex)=ABCEM23DX(i1,i2,i3,ex)
           un(i1-is1,i2-is2,i3-is3,ey)=ABCEM23DX(i1,i2,i3,ey)
           un(i1-is1,i2-is2,i3-is3,ez)=ABCEM23DX(i1,i2,i3,ez)
          endLoops()
         else if( axis.eq.1 )then
          beginLoops()
           un(i1-is1,i2-is2,i3-is3,ex)=ABCEM23DY(i1,i2,i3,ex)
           un(i1-is1,i2-is2,i3-is3,ey)=ABCEM23DY(i1,i2,i3,ey)
           un(i1-is1,i2-is2,i3-is3,ez)=ABCEM23DY(i1,i2,i3,ez)
          endLoops()
         else if( axis.eq.2 )then
          beginLoops()
           un(i1-is1,i2-is2,i3-is3,ex)=ABCEM23DZ(i1,i2,i3,ex)
           un(i1-is1,i2-is2,i3-is3,ey)=ABCEM23DZ(i1,i2,i3,ey)
           un(i1-is1,i2-is2,i3-is3,ez)=ABCEM23DZ(i1,i2,i3,ez)
          endLoops()
         else
          stop 46766
         end if

        end if
     
       else if( gridType.eq.rectangular .and. orderOfAccuracy.eq.4 )then

        ! ***********************************************
        ! ************rectangular grid*******************
        ! ************ fourth-order   *******************
        ! ***********************************************

       
        ! >>>>>>>>>>>>>>>> this is only second-order ---- fix this <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


        ! write(*,'(" Apply abcEM2: grid,side,axis=",3i3," dt,c=",2e12.3)') grid,side,axis,dt,c
        if( nd.eq.2 )then
         if( axis.eq.0 )then
          beginLoops()
       
           un(i1-is1,i2-is2,i3-is3,ex)=ABCEM2X(i1,i2,i3,ex)
           un(i1-is1,i2-is2,i3-is3,ey)=ABCEM2X(i1,i2,i3,ey)
           un(i1-is1,i2-is2,i3-is3,hz)=ABCEM2X(i1,i2,i3,hz)
       
           extrapLine2Order4(i1,i2,i3,is1,is2,is3,ex)
           extrapLine2Order4(i1,i2,i3,is1,is2,is3,ey)
           extrapLine2Order4(i1,i2,i3,is1,is2,is3,hz)

          endLoops()
         else if( axis.eq.1 )then
          beginLoops()
           un(i1-is1,i2-is2,i3-is3,ex)=ABCEM2Y(i1,i2,i3,ex)
           un(i1-is1,i2-is2,i3-is3,ey)=ABCEM2Y(i1,i2,i3,ey)
           un(i1-is1,i2-is2,i3-is3,hz)=ABCEM2Y(i1,i2,i3,hz)

           extrapLine2Order4(i1,i2,i3,is1,is2,is3,ex)
           extrapLine2Order4(i1,i2,i3,is1,is2,is3,ey)
           extrapLine2Order4(i1,i2,i3,is1,is2,is3,hz)

          endLoops()
         else
          stop 9477
         end if

        else ! ***** 3D *****
         if( axis.eq.0 )then
          beginLoops()
           un(i1-is1,i2-is2,i3-is3,ex)=ABCEM23DX(i1,i2,i3,ex)
           un(i1-is1,i2-is2,i3-is3,ey)=ABCEM23DX(i1,i2,i3,ey)
           un(i1-is1,i2-is2,i3-is3,ez)=ABCEM23DX(i1,i2,i3,ez)

           extrapLine2Order4(i1,i2,i3,is1,is2,is3,ex)
           extrapLine2Order4(i1,i2,i3,is1,is2,is3,ey)
           extrapLine2Order4(i1,i2,i3,is1,is2,is3,ez)
          endLoops()
         else if( axis.eq.1 )then
          beginLoops()
           un(i1-is1,i2-is2,i3-is3,ex)=ABCEM23DY(i1,i2,i3,ex)
           un(i1-is1,i2-is2,i3-is3,ey)=ABCEM23DY(i1,i2,i3,ey)
           un(i1-is1,i2-is2,i3-is3,ez)=ABCEM23DY(i1,i2,i3,ez)

           extrapLine2Order4(i1,i2,i3,is1,is2,is3,ex)
           extrapLine2Order4(i1,i2,i3,is1,is2,is3,ey)
           extrapLine2Order4(i1,i2,i3,is1,is2,is3,ez)
          endLoops()
         else if( axis.eq.2 )then
          beginLoops()
           un(i1-is1,i2-is2,i3-is3,ex)=ABCEM23DZ(i1,i2,i3,ex)
           un(i1-is1,i2-is2,i3-is3,ey)=ABCEM23DZ(i1,i2,i3,ey)
           un(i1-is1,i2-is2,i3-is3,ez)=ABCEM23DZ(i1,i2,i3,ez)

           extrapLine2Order4(i1,i2,i3,is1,is2,is3,ex)
           extrapLine2Order4(i1,i2,i3,is1,is2,is3,ey)
           extrapLine2Order4(i1,i2,i3,is1,is2,is3,ez)
          endLoops()
         else
          stop 94766
         end if

        end if
     
       else
         stop 2255
       end if

      endLoopOverSides()

      return
      end
