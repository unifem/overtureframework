c     *******************************************************************************
c     Solid Mechanics boundary conditions -- conservative version
c     *******************************************************************************

c     These next include files will define the macros that will define the difference approximations
c     The actual macro is called below
#Include "defineDiffNewerOrder2f.h"
#Include "defineDiffNewerOrder4f.h"

#beginMacro OGF2D(i1,i2,i3,t,u0,v0)
      call ogf2d(ep,xy(i1,i2,i3,0),xy(i1,i2,i3,1),t,u0,v0)
#endMacro

#beginMacro OGF3D(i1,i2,i3,t,u0,v0,w0)
      call ogf3d(ep,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,u0,v0,w0)
#endMacro

#beginMacro OGDERIV2D(ntd,nxd,nyd,nzd,i1,i2,i3,t,ux,vx)
      call ogDeriv2(ep, ntd,nxd,nyd,nzd, xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t, uc,ux, vc,vx)
#endMacro

#beginMacro OGDERIV3D(ntd,nxd,nyd,nzd,i1,i2,i3,t,ux,vx,wx)
      call ogDeriv3(ep, ntd,nxd,nyd,nzd, xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t, uc,ux, vc,vx, wc,wx)
#endMacro


! *wdh* 091205 -- changed most loops to use the mask (any loops setting ghost values) ----

#beginMacro beginLoops(n1a,n1b,n2a,n2b,n3a,n3b,na,nb)
do i3=n3a,n3b
do i2=n2a,n2b
do i1=n1a,n1b
do n=na,nb
!write(*,'(" periodic i1,i2,i3,n=",4i4)') i1,i2,i3,n
#endMacro

#beginMacro beginLoopsMask3dE()
do i3=n3ae,n3be
do i2=n2ae,n2be
do i1=n1ae,n1be
 if( mask(i1,i2,i3).gt.0 )then
#endMacro

#beginMacro endLoops()
 end do
 end do
 end do
 end do
#endMacro

#beginMacro beginLoops2d()
 i3=n3a
 do i2=n2a,n2b
  do i1=n1a,n1b
#endMacro
#beginMacro endLoops2d()
  end do
 end do
#endMacro

#beginMacro beginLoopsMask2d()
 i3=n3a
 do i2=n2a,n2b
 do i1=n1a,n1b
 if( mask(i1,i2,i3).gt.0 )then
#endMacro
#beginMacro endLoopsMask2d()
 end if
 end do
 end do
#endMacro

#beginMacro beginGhostLoops2d()
      i3=n3a
      do i2=nn2a,nn2b
       do i1=nn1a,nn1b
#endMacro

#beginMacro beginLoops3d()
        do i3=n3a,n3b
         do i2=n2a,n2b
          do i1=n1a,n1b
#endMacro

#beginMacro endLoops3d()
          end do
         end do
        end do
#endMacro

#beginMacro beginGhostLoops3d()
        do i3=nn3a,nn3b
         do i2=nn2a,nn2b
          do i1=nn1a,nn1b
#endMacro

#beginMacro beginLoopsMask3d()
 do i3=n3a,n3b
 do i2=n2a,n2b
 do i1=n1a,n1b
 if( mask(i1,i2,i3).gt.0 )then
#endMacro

#beginMacro endLoopsMask3d()
 end if
 end do
 end do
 end do
#endMacro


c     ************************************************************************************************
c     This macro is used for looping over the faces of a grid to assign booundary conditions
c     
c     extra: extra points to assign
c     Case 1: extra=numberOfGhostPoints -- for assigning extended boundaries
c     Case 2: extra=-1 -- for assigning ghost points but not including extended boundaries
c     numberOfGhostPoints : number of ghost points (1 for 2nd order, 2 for fourth-order ...)
c     
c     
c     Output:
c     n1a,n1b,n2a,n2b,n3a,n3b : from gridIndexRange
c     nn1a,nn1b,nn2a,nn2b,nn3a,nn3b : includes "extra" points
c     
c     ***********************************************************************************************
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
            extra1a=max(0,extra1a) ! over-ride extra=-1 : assign ends in periodic directions (or internal parallel boundaries)
           else if( boundaryCondition(0,0).eq.0 )then
            extra1a=numberOfGhostPoints ! include interpolation points since we assign ghost points outside these
           end if
!     **NOTE** the bc on the right may be negative even it is not on the left (for parallel)
           if( boundaryCondition(1,0).lt.0 )then
            extra1b=max(0,extra1b) ! over-ride extra=-1 : assign ends in periodic directions
           else if( boundaryCondition(1,0).eq.0 )then
            extra1b=numberOfGhostPoints
           end if

           if( boundaryCondition(0,1).lt.0 )then
            extra2a=max(0,extra2a) ! over-ride extra=-1 : assign ends in periodic directions (or internal parallel boundaries)
           else if( boundaryCondition(0,1).eq.0 )then
            extra2a=numberOfGhostPoints ! include interpolation points since we assign ghost points outside these
           end if
!     **NOTE** the bc on the right may be negative even it is not on the left (for parallel)
           if( boundaryCondition(1,1).lt.0 )then
            extra2b=max(0,extra2b) ! over-ride extra=-1 : assign ends in periodic directions
           else if( boundaryCondition(1,1).eq.0 )then
            extra2b=numberOfGhostPoints
           end if

           if(  nd.eq.3 )then
            if( boundaryCondition(0,2).lt.0 )then
             extra3a=max(0,extra3a) ! over-ride extra=-1 : assign ends in periodic directions (or internal parallel boundaries)
            else if( boundaryCondition(0,2).eq.0 )then
             extra3a=numberOfGhostPoints ! include interpolation points since we assign ghost points outside these
            end if
!     **NOTE** the bc on the right may be negative even it is not on the left (for parallel)
            if( boundaryCondition(1,2).lt.0 )then
             extra3b=max(0,extra3b) ! over-ride extra=-1 : assign ends in periodic directions
            else if( boundaryCondition(1,2).eq.0 )then
             extra3b=numberOfGhostPoints
            end if
           end if

           do axis=0,nd-1
            do side=0,1

             if( boundaryCondition(side,axis).gt.0 )then

!     write(*,'(" bcOpt: side,axis,bc=",3i2)') side,axis,boundaryCondition(side,axis)

     n1a=gridIndexRange(0,0)
     n1b=gridIndexRange(1,0)
     n2a=gridIndexRange(0,1)
     n2b=gridIndexRange(1,1)
     n3a=gridIndexRange(0,2)
     n3b=gridIndexRange(1,2)
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


     nn1a=gridIndexRange(0,0)-extra1a
     nn1b=gridIndexRange(1,0)+extra1b
     nn2a=gridIndexRange(0,1)-extra2a
     nn2b=gridIndexRange(1,1)+extra2b
     nn3a=gridIndexRange(0,2)-extra3a
     nn3b=gridIndexRange(1,2)+extra3b
     if( axis.eq.0 )then
 nn1a=gridIndexRange(side,axis)
 nn1b=gridIndexRange(side,axis)
     else if( axis.eq.1 )then
 nn2a=gridIndexRange(side,axis)
 nn2b=gridIndexRange(side,axis)
     else
 nn3a=gridIndexRange(side,axis)
 nn3b=gridIndexRange(side,axis)
     end if

     is=1-2*side

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

     i3=n3a
     if( debug.gt.7 )then
 write(*,'(" bcOpt: grid,side,axis=",3i3,", \
 loop bounds: n1a,n1b,n2a,n2b,n3a,n3b=",6i3)') grid,side,axis,\
 n1a,n1b,n2a,n2b,n3a,n3b

     end if
end if! if bc>0
#endMacro

#beginMacro endLoopOverSides()
            end do ! end side
           end do  ! end axis
#endMacro
!     
!     Below are the conservative stress free BC on rectangular grids
!     
#beginMacro CornerTangentDer2d(ut,uc,dir,ud)
           ut=(1.0*ud/dx(dir))*(u(i1+ud*(1-dir),i2+ud*dir,i3,uc)-u(i1,i2,i3,uc))
#endMacro

#beginMacro DoXStressFree()
           u(i1-is1,i2-is2,i3,uc)=u(i1,i2,i3,uc)\
           -is1*dx(0)*(-is1*0.25*(lam2muM(i1,i2)+lam2muM(i1+is1,i2+is2))*(1./dx(0))*(u(i1+is1,i2+is2,i3,uc)-u(i1,i2,i3,uc))\
           -lamM(i1,i2)*vy\
           +Forceu)\
           /(0.25*(lam2muM(i1,i2)+lam2muM(i1-is1,i2-is2)))
           u(i1-is1,i2-is2,i3,vc)=u(i1,i2,i3,vc)\
           -is1*dx(0)*(-is1*0.25*(muM(i1,i2)+muM(i1+is1,i2+is2))*(1.0/dx(0))*(u(i1+is1,i2+is2,i3,vc)-u(i1,i2,i3,vc))\
           -muM(i1,i2)*uy\
           +Forcev)/\
           (0.25*(muM(i1,i2)+muM(i1-is1,i2-is2)))
#endMacro

#beginMacro DoYStressFree()
           u(i1-is1,i2-is2,i3,vc)=u(i1,i2,i3,vc)\
           -is2*dx(1)*(-is2*0.25*(lam2muM(i1,i2)+lam2muM(i1+is1,i2+is2))*(1.0/dx(1))*(u(i1+is1,i2+is2,i3,vc)-u(i1,i2,i3,vc))\
           -lamM(i1,i2)*ux\
           +Forcev)\
           /(0.25*(lam2muM(i1,i2)+lam2muM(i1-is1,i2-is2)))
           u(i1-is1,i2-is2,i3,uc)=u(i1,i2,i3,uc)\
           -is2*dx(1)*(-is2*0.25*(muM(i1,i2)+muM(i1+is1,i2+is2))*(1.0/dx(1))*(u(i1+is1,i2+is2,i3,uc)-u(i1,i2,i3,uc))\
           -muM(i1,i2)*vx\
           +Forceu)\
           /(0.25*(muM(i1,i2)+muM(i1-is1,i2-is2)))
#endMacro


#beginMacro SolveForDnInGP(C1,C2,C3,C4,Fu,Fv)
!     [ a2(0,0) a2(0,1) ][ DNu(gp) ] =  FU
!     [ a2(1,0) a2(1,1) ][ DNv(gp) ] =  FV
           a2(0,0)=(C1)
           a2(0,1)=(C2)
           a2(1,0)=a2(0,1)
!     a2(1,0)=(C3)
           a2(1,1)=(C4)
           f(0) = (Fu)
           f(1) = (Fv)
!     write (*,*) "a(0,0), a(0,1), a(1,0), a(1,1), f(0), f(1) " ,a2(0,0), a2(0,1), a2(1,0), a2(1,1), f(0), f(1)
           call dgeco( a2(0,0), 2, 2, ipvt(0),rcond,work(0))
           call dgesl( a2(0,0), 2, 2, ipvt(0), f(0), job)
#endMacro

#defineMacro Egp(C1,C2,C3,C4) (0.5*(C1(i1-is1,i2-is2)*C2(i1-is1,i2-is2)*C3(i1-is1,i2-is2)*C4(i1-is1,i2-is2)+C1(i1,i2)*C2(i1,i2)*C3(i1,i2)*C4(i1,i2)))
#defineMacro Eip(C1,C2,C3,C4) (0.5*(C1(i1+is1,i2+is2)*C2(i1+is1,i2+is2)*C3(i1+is1,i2+is2)*C4(i1+is1,i2+is2)+C1(i1,i2)*C2(i1,i2)*C3(i1,i2)*C4(i1,i2)))
#defineMacro E0(C1,C2,C3,C4) (C1(i1,i2)*C2(i1,i2)*C3(i1,i2)*C4(i1,i2))
#defineMacro Dnip(uc) ((1.*(is1+is2))/dr(axis)*(u(i1+is1,i2+is2,i3,uc)-u(i1,i2,i3,uc)))
#defineMacro Dt0(uc) (0.5/dr(1-axis))*(u(i1+axis,i2+(1-axis),i3,uc)-u(i1-axis,i2-(1-axis),i3,uc))
#defineMacro DtC(uc,dir,ud) ((1.0*ud/dr(dir))*(u(i1+ud*(1-dir),i2+ud*dir,i3,uc)-u(i1,i2,i3,uc)))

!     ==========================================================================
!     Constervative
!     Apply a stress free BC -- curvilinear and 2d
!     
!     FORCING equals noForcing or forcing
!     ==========================================================================
#beginMacro tractionBCCurvilinear2dCons(FORCING)
           if (axis.eq.0) then
            beginLoopsMask2d()
#If #FORCING eq "forcing"
            OGDERIV2D(0,1,0,0,i1,i2,i3,t,ux0,vx0)
            OGDERIV2D(0,0,1,0,i1,i2,i3,t,uy0,vy0)
            SolveForDnInGP((0.5*(Egp(Jac,qx,qx,lam2muM)+Egp(Jac,qy,qy,muM))),\
            (0.5*(Egp(Jac,qx,qy,lamM)+Egp(Jac,qy,qx,muM))),\
            (0.5*(Egp(Jac,qx,qy,muM)+Egp(Jac,qy,qx,lamM))),\
            (0.5*(Egp(Jac,qy,qy,lam2muM)+Egp(Jac,qx,qx,muM))),\
            -(0.5*(Eip(Jac,qx,qx,lam2muM)+Eip(Jac,qy,qy,muM))*(Dnip(uc))\
            +0.5*(Eip(Jac,qx,qy,lamM)+Eip(Jac,qy,qx,muM))*(Dnip(vc))\
            +(E0(Jac,qx,rx,lam2muM)+E0(Jac,qy,ry,muM))*(Dt0(uc))\
            +(E0(Jac,qx,ry,lamM)+E0(Jac,qy,rx,muM))*(Dt0(vc)))\
            +Jac(i1,i2)*(qx(i1,i2)*(lam2muM(i1,i2)*ux0+lamM(i1,i2)*vy0)+qy(i1,i2)*(muM(i1,i2)*(vx0+uy0))),\
            -(0.5*(Eip(Jac,qx,qy,muM)+Eip(Jac,qy,qx,lamM))*(Dnip(uc))\
            +0.5*(Eip(Jac,qy,qy,lam2muM)+Eip(Jac,qx,qx,muM))*(Dnip(vc))\
            +(E0(Jac,qx,ry,muM)+E0(Jac,qy,rx,lamM))*(Dt0(uc))\
            +(E0(Jac,qx,rx,muM)+E0(Jac,qy,ry,lam2muM))*(Dt0(vc)))\
            +Jac(i1,i2)*(qx(i1,i2)*(muM(i1,i2)*(vx0+uy0))+qy(i1,i2)*(lam2muM(i1,i2)*vy0+lamM(i1,i2)*ux0)))
#Else
            if (addBoundaryForcing(side,axis).eq.0) then
             SolveForDnInGP((0.5*(Egp(Jac,qx,qx,lam2muM)+Egp(Jac,qy,qy,muM))),\
             (0.5*(Egp(Jac,qx,qy,lamM)+Egp(Jac,qy,qx,muM))),\
             (0.5*(Egp(Jac,qx,qy,muM)+Egp(Jac,qy,qx,lamM))),\
             (0.5*(Egp(Jac,qy,qy,lam2muM)+Egp(Jac,qx,qx,muM))),\
             -(0.5*(Eip(Jac,qx,qx,lam2muM)+Eip(Jac,qy,qy,muM))*(Dnip(uc))\
             +0.5*(Eip(Jac,qx,qy,lamM)+Eip(Jac,qy,qx,muM))*(Dnip(vc))\
             +(E0(Jac,qx,rx,lam2muM)+E0(Jac,qy,ry,muM))*(Dt0(uc))\
             +(E0(Jac,qx,ry,lamM)+E0(Jac,qy,rx,muM))*(Dt0(vc))),\
             -(0.5*(Eip(Jac,qx,qy,muM)+Eip(Jac,qy,qx,lamM))*(Dnip(uc))\
             +0.5*(Eip(Jac,qy,qy,lam2muM)+Eip(Jac,qx,qx,muM))*(Dnip(vc))\
             +(E0(Jac,qx,ry,muM)+E0(Jac,qy,rx,lamM))*(Dt0(uc))\
             +(E0(Jac,qx,rx,muM)+E0(Jac,qy,ry,lam2muM))*(Dt0(vc))))
            else
             SolveForDnInGP((0.5*(Egp(Jac,qx,qx,lam2muM)+Egp(Jac,qy,qy,muM))),\
             (0.5*(Egp(Jac,qx,qy,lamM)+Egp(Jac,qy,qx,muM))),\
             (0.5*(Egp(Jac,qx,qy,muM)+Egp(Jac,qy,qx,lamM))),\
             (0.5*(Egp(Jac,qy,qy,lam2muM)+Egp(Jac,qx,qx,muM))),\
             -(0.5*(Eip(Jac,qx,qx,lam2muM)+Eip(Jac,qy,qy,muM))*(Dnip(uc))\
             +0.5*(Eip(Jac,qx,qy,lamM)+Eip(Jac,qy,qx,muM))*(Dnip(vc))\
             +(E0(Jac,qx,rx,lam2muM)+E0(Jac,qy,ry,muM))*(Dt0(uc))\
             +(E0(Jac,qx,ry,lamM)+E0(Jac,qy,rx,muM))*(Dt0(vc)))\
             -is*Jac(i1,i2)*(sqrt(qx(i1,i2)*qx(i1,i2)+qy(i1,i2)*qy(i1,i2)))*(bcf(side,axis,i1,i2,i3,uc)),\
             -(0.5*(Eip(Jac,qx,qy,muM)+Eip(Jac,qy,qx,lamM))*(Dnip(uc))\
             +0.5*(Eip(Jac,qy,qy,lam2muM)+Eip(Jac,qx,qx,muM))*(Dnip(vc))\
             +(E0(Jac,qx,ry,muM)+E0(Jac,qy,rx,lamM))*(Dt0(uc))\
             +(E0(Jac,qx,rx,muM)+E0(Jac,qy,ry,lam2muM))*(Dt0(vc)))\
             -is*Jac(i1,i2)*(sqrt(qx(i1,i2)*qx(i1,i2)+qy(i1,i2)*qy(i1,i2)))*(bcf(side,axis,i1,i2,i3,vc)))
            end if
#End
            u(i1-is1,i2-is2,i3,uc)=u(i1,i2,i3,uc)-(is1+is2)*f(0)*dr(axis)
            u(i1-is1,i2-is2,i3,vc)=u(i1,i2,i3,vc)-(is1+is2)*f(1)*dr(axis)
            endLoopsMask2d()
           else
            beginLoopsMask2d()
#If #FORCING eq "forcing"
            OGDERIV2D(0,1,0,0,i1,i2,i3,t,ux0,vx0)
            OGDERIV2D(0,0,1,0,i1,i2,i3,t,uy0,vy0)
            SolveForDnInGP((0.5*(Egp(Jac,rx,rx,lam2muM)+Egp(Jac,ry,ry,muM))),\
            (0.5*(Egp(Jac,rx,ry,lamM)+Egp(Jac,ry,rx,muM))),\
            (0.5*(Egp(Jac,rx,ry,muM)+Egp(Jac,ry,rx,lamM))),\
            (0.5*(Egp(Jac,ry,ry,lam2muM)+Egp(Jac,rx,rx,muM))),\
            -(0.5*(Eip(Jac,rx,rx,lam2muM)+Eip(Jac,ry,ry,muM))*(Dnip(uc))\
            +0.5*(Eip(Jac,rx,ry,lamM)+Eip(Jac,ry,rx,muM))*(Dnip(vc))\
            +(E0(Jac,rx,qx,lam2muM)+E0(Jac,ry,qy,muM))*(Dt0(uc))\
            +(E0(Jac,rx,qy,lamM)+E0(Jac,ry,qx,muM))*(Dt0(vc)))\
            +Jac(i1,i2)*(rx(i1,i2)*(lam2muM(i1,i2)*ux0+lamM(i1,i2)*vy0)+ry(i1,i2)*(muM(i1,i2)*(vx0+uy0))),\
            -(0.5*(Eip(Jac,rx,ry,muM)+Eip(Jac,ry,rx,lamM))*(Dnip(uc))\
            +0.5*(Eip(Jac,ry,ry,lam2muM)+Eip(Jac,rx,rx,muM))*(Dnip(vc))\
            +(E0(Jac,rx,qy,muM)+E0(Jac,ry,qx,lamM))*(Dt0(uc))\
            +(E0(Jac,rx,qx,muM)+E0(Jac,ry,qy,lam2muM))*(Dt0(vc)))\
            +Jac(i1,i2)*(rx(i1,i2)*(muM(i1,i2)*(vx0+uy0))+ry(i1,i2)*(lam2muM(i1,i2)*vy0+lamM(i1,i2)*ux0)))
#Else
            if (addBoundaryForcing(side,axis).eq.0) then
             SolveForDnInGP((0.5*(Egp(Jac,rx,rx,lam2muM)+Egp(Jac,ry,ry,muM))),\
             (0.5*(Egp(Jac,rx,ry,lamM)+Egp(Jac,ry,rx,muM))),\
             (0.5*(Egp(Jac,rx,ry,muM)+Egp(Jac,ry,rx,lamM))),\
             (0.5*(Egp(Jac,ry,ry,lam2muM)+Egp(Jac,rx,rx,muM))),\
             -(0.5*(Eip(Jac,rx,rx,lam2muM)+Eip(Jac,ry,ry,muM))*(Dnip(uc))\
             +0.5*(Eip(Jac,rx,ry,lamM)+Eip(Jac,ry,rx,muM))*(Dnip(vc))\
             +(E0(Jac,rx,qx,lam2muM)+E0(Jac,ry,qy,muM))*(Dt0(uc))\
             +(E0(Jac,rx,qy,lamM)+E0(Jac,ry,qx,muM))*(Dt0(vc))),\
             -(0.5*(Eip(Jac,rx,ry,muM)+Eip(Jac,ry,rx,lamM))*(Dnip(uc))\
             +0.5*(Eip(Jac,ry,ry,lam2muM)+Eip(Jac,rx,rx,muM))*(Dnip(vc))\
             +(E0(Jac,rx,qy,muM)+E0(Jac,ry,qx,lamM))*(Dt0(uc))\
             +(E0(Jac,rx,qx,muM)+E0(Jac,ry,qy,lam2muM))*(Dt0(vc))))
            else
             SolveForDnInGP((0.5*(Egp(Jac,rx,rx,lam2muM)+Egp(Jac,ry,ry,muM))),\
             (0.5*(Egp(Jac,rx,ry,lamM)+Egp(Jac,ry,rx,muM))),\
             (0.5*(Egp(Jac,rx,ry,muM)+Egp(Jac,ry,rx,lamM))),\
             (0.5*(Egp(Jac,ry,ry,lam2muM)+Egp(Jac,rx,rx,muM))),\
             -(0.5*(Eip(Jac,rx,rx,lam2muM)+Eip(Jac,ry,ry,muM))*(Dnip(uc))\
             +0.5*(Eip(Jac,rx,ry,lamM)+Eip(Jac,ry,rx,muM))*(Dnip(vc))\
             +(E0(Jac,rx,qx,lam2muM)+E0(Jac,ry,qy,muM))*(Dt0(uc))\
             +(E0(Jac,rx,qy,lamM)+E0(Jac,ry,qx,muM))*(Dt0(vc)))\
             -is*Jac(i1,i2)*(sqrt(rx(i1,i2)*rx(i1,i2)+ry(i1,i2)*ry(i1,i2)))*(bcf(side,axis,i1,i2,i3,uc)),\
             -(0.5*(Eip(Jac,rx,ry,muM)+Eip(Jac,ry,rx,lamM))*(Dnip(uc))\
             +0.5*(Eip(Jac,ry,ry,lam2muM)+Eip(Jac,rx,rx,muM))*(Dnip(vc))\
             +(E0(Jac,rx,qy,muM)+E0(Jac,ry,qx,lamM))*(Dt0(uc))\
             +(E0(Jac,rx,qx,muM)+E0(Jac,ry,qy,lam2muM))*(Dt0(vc)))\
             -is*Jac(i1,i2)*(sqrt(rx(i1,i2)*rx(i1,i2)+ry(i1,i2)*ry(i1,i2)))*(bcf(side,axis,i1,i2,i3,vc)))
            end if
#End
            u(i1-is1,i2-is2,i3,uc)=u(i1,i2,i3,uc)-(is1+is2)*f(0)*dr(axis)
            u(i1-is1,i2-is2,i3,vc)=u(i1,i2,i3,vc)-(is1+is2)*f(1)*dr(axis)
            endLoopsMask2d()
           end if
#endMacro

#beginMacro CorrectCornersCurvilinear2dCons(FORCING)
           if (axis.eq.0) then
!     ! Correct the corners if adjacent sides have stress free bc
!     ! We use D+ or D- for the tangential derivative to get energy est.
#If #FORCING eq "forcing"
            do idxs=0,1
             if( boundaryCondition(idxs,1).eq.tractionBC )then
              i2=n2a*(1-idxs)+n2b*idxs
              i1=n1a
              OGDERIV2D(0,1,0,0,i1,i2,i3,t,ux0,vx0)
              OGDERIV2D(0,0,1,0,i1,i2,i3,t,uy0,vy0)
              SolveForDnInGP((0.5*(Egp(Jac,qx,qx,lam2muM)+Egp(Jac,qy,qy,muM))),\
              (0.5*(Egp(Jac,qx,qy,lamM)+Egp(Jac,qy,qx,muM))),\
              (0.5*(Egp(Jac,qx,qy,muM)+Egp(Jac,qy,qx,lamM))),\
              (0.5*(Egp(Jac,qy,qy,lam2muM)+Egp(Jac,qx,qx,muM))),\
              -(0.5*(Eip(Jac,qx,qx,lam2muM)+Eip(Jac,qy,qy,muM))*(Dnip(uc))\
              +0.5*(Eip(Jac,qx,qy,lamM)+Eip(Jac,qy,qx,muM))*(Dnip(vc))\
              +(E0(Jac,qx,rx,lam2muM)+E0(Jac,qy,ry,muM))*(DtC(uc,1,(1-2*idxs)))\
              +(E0(Jac,qx,ry,lamM)+E0(Jac,qy,rx,muM))*(DtC(vc,1,(1-2*idxs))))\
              +Jac(i1,i2)*(qx(i1,i2)*(lam2muM(i1,i2)*ux0+lamM(i1,i2)*vy0)+qy(i1,i2)*(muM(i1,i2)*(vx0+uy0))),\
              -(0.5*(Eip(Jac,qx,qy,muM)+Eip(Jac,qy,qx,lamM))*(Dnip(uc))\
              +0.5*(Eip(Jac,qy,qy,lam2muM)+Eip(Jac,qx,qx,muM))*(Dnip(vc))\
              +(E0(Jac,qx,ry,muM)+E0(Jac,qy,rx,lamM))*(DtC(uc,1,(1-2*idxs)))\
              +(E0(Jac,qx,rx,muM)+E0(Jac,qy,ry,lam2muM))*(DtC(vc,1,(1-2*idxs))))\
              +Jac(i1,i2)*(qx(i1,i2)*(muM(i1,i2)*(vx0+uy0))+qy(i1,i2)*(lam2muM(i1,i2)*vy0+lamM(i1,i2)*ux0)))
              u(i1-is1,i2-is2,i3,uc)=u(i1,i2,i3,uc)-(is1+is2)*f(0)*dr(axis)
              u(i1-is1,i2-is2,i3,vc)=u(i1,i2,i3,vc)-(is1+is2)*f(1)*dr(axis)
             end if
            end do
#Else
            do idxs=0,1
             if( boundaryCondition(idxs,1).eq.tractionBC )then
              i2=n2a*(1-idxs)+n2b*idxs
              i1=n1a
              if (addBoundaryForcing(side,axis).eq.0) then
               SolveForDnInGP((0.5*(Egp(Jac,qx,qx,lam2muM)+Egp(Jac,qy,qy,muM))),\
               (0.5*(Egp(Jac,qx,qy,lamM)+Egp(Jac,qy,qx,muM))),\
               (0.5*(Egp(Jac,qx,qy,muM)+Egp(Jac,qy,qx,lamM))),\
               (0.5*(Egp(Jac,qy,qy,lam2muM)+Egp(Jac,qx,qx,muM))),\
               -(0.5*(Eip(Jac,qx,qx,lam2muM)+Eip(Jac,qy,qy,muM))*(Dnip(uc))\
               +0.5*(Eip(Jac,qx,qy,lamM)+Eip(Jac,qy,qx,muM))*(Dnip(vc))\
               +(E0(Jac,qx,rx,lam2muM)+E0(Jac,qy,ry,muM))*(DtC(uc,1,(1-2*idxs)))\
               +(E0(Jac,qx,ry,lamM)+E0(Jac,qy,rx,muM))*(DtC(vc,1,(1-2*idxs)))),\
               -(0.5*(Eip(Jac,qx,qy,muM)+Eip(Jac,qy,qx,lamM))*(Dnip(uc))\
               +0.5*(Eip(Jac,qy,qy,lam2muM)+Eip(Jac,qx,qx,muM))*(Dnip(vc))\
               +(E0(Jac,qx,ry,muM)+E0(Jac,qy,rx,lamM))*(DtC(uc,1,(1-2*idxs)))\
               +(E0(Jac,qx,rx,muM)+E0(Jac,qy,ry,lam2muM))*(DtC(vc,1,(1-2*idxs)))))
              else
               SolveForDnInGP((0.5*(Egp(Jac,qx,qx,lam2muM)+Egp(Jac,qy,qy,muM))),\
               (0.5*(Egp(Jac,qx,qy,lamM)+Egp(Jac,qy,qx,muM))),\
               (0.5*(Egp(Jac,qx,qy,muM)+Egp(Jac,qy,qx,lamM))),\
               (0.5*(Egp(Jac,qy,qy,lam2muM)+Egp(Jac,qx,qx,muM))),\
               -(0.5*(Eip(Jac,qx,qx,lam2muM)+Eip(Jac,qy,qy,muM))*(Dnip(uc))\
               +0.5*(Eip(Jac,qx,qy,lamM)+Eip(Jac,qy,qx,muM))*(Dnip(vc))\
               +(E0(Jac,qx,rx,lam2muM)+E0(Jac,qy,ry,muM))*(DtC(uc,1,(1-2*idxs)))\
               +(E0(Jac,qx,ry,lamM)+E0(Jac,qy,rx,muM))*(DtC(vc,1,(1-2*idxs))))\
               -is*Jac(i1,i2)*(sqrt(qx(i1,i2)*qx(i1,i2)+qy(i1,i2)*qy(i1,i2)))*(bcf(side,axis,i1,i2,i3,uc)),\
               -(0.5*(Eip(Jac,qx,qy,muM)+Eip(Jac,qy,qx,lamM))*(Dnip(uc))\
               +0.5*(Eip(Jac,qy,qy,lam2muM)+Eip(Jac,qx,qx,muM))*(Dnip(vc))\
               +(E0(Jac,qx,ry,muM)+E0(Jac,qy,rx,lamM))*(DtC(uc,1,(1-2*idxs)))\
               +(E0(Jac,qx,rx,muM)+E0(Jac,qy,ry,lam2muM))*(DtC(vc,1,(1-2*idxs))))\
               -is*Jac(i1,i2)*(sqrt(qx(i1,i2)*qx(i1,i2)+qy(i1,i2)*qy(i1,i2)))*(bcf(side,axis,i1,i2,i3,vc)))
              end if
              u(i1-is1,i2-is2,i3,uc)=u(i1,i2,i3,uc)-(is1+is2)*f(0)*dr(axis)
              u(i1-is1,i2-is2,i3,vc)=u(i1,i2,i3,vc)-(is1+is2)*f(1)*dr(axis)
             end if
            end do
#End
           else                 ! the other axis
!     ! Correct the corners if adjacent sides have stress free bc
!     ! We use D+ or D- for the tangential derivative to get energy est.
#If #FORCING eq "forcing"
            do idxs=0,1
             if( boundaryCondition(idxs,0).eq.tractionBC )then
              i2=n2a
              i1=n1a*(1-idxs)+n1b*idxs
              OGDERIV2D(0,1,0,0,i1,i2,i3,t,ux0,vx0)
              OGDERIV2D(0,0,1,0,i1,i2,i3,t,uy0,vy0)
              SolveForDnInGP((0.5*(Egp(Jac,rx,rx,lam2muM)+Egp(Jac,ry,ry,muM))),\
              (0.5*(Egp(Jac,rx,ry,lamM)+Egp(Jac,ry,rx,muM))),\
              (0.5*(Egp(Jac,rx,ry,muM)+Egp(Jac,ry,rx,lamM))),\
              (0.5*(Egp(Jac,ry,ry,lam2muM)+Egp(Jac,rx,rx,muM))),\
              -(0.5*(Eip(Jac,rx,rx,lam2muM)+Eip(Jac,ry,ry,muM))*(Dnip(uc))\
              +0.5*(Eip(Jac,rx,ry,lamM)+Eip(Jac,ry,rx,muM))*(Dnip(vc))\
              +(E0(Jac,rx,qx,lam2muM)+E0(Jac,ry,qy,muM))*(DtC(uc,0,(1-2*idxs)))\
              +(E0(Jac,rx,qy,lamM)+E0(Jac,ry,qx,muM))*(DtC(vc,0,(1-2*idxs))))\
              +Jac(i1,i2)*(rx(i1,i2)*(lam2muM(i1,i2)*ux0+lamM(i1,i2)*vy0)+ry(i1,i2)*(muM(i1,i2)*(vx0+uy0))),\
              -(0.5*(Eip(Jac,rx,ry,muM)+Eip(Jac,ry,rx,lamM))*(Dnip(uc))\
              +0.5*(Eip(Jac,ry,ry,lam2muM)+Eip(Jac,rx,rx,muM))*(Dnip(vc))\
              +(E0(Jac,rx,qy,muM)+E0(Jac,ry,qx,lamM))*(DtC(uc,0,(1-2*idxs)))\
              +(E0(Jac,rx,qx,muM)+E0(Jac,ry,qy,lam2muM))*(DtC(vc,0,(1-2*idxs))))\
              +Jac(i1,i2)*(rx(i1,i2)*(muM(i1,i2)*(vx0+uy0))+ry(i1,i2)*(lam2muM(i1,i2)*vy0+lamM(i1,i2)*ux0)))
              u(i1-is1,i2-is2,i3,uc)=u(i1,i2,i3,uc)-(is1+is2)*f(0)*dr(axis)
              u(i1-is1,i2-is2,i3,vc)=u(i1,i2,i3,vc)-(is1+is2)*f(1)*dr(axis)
             end if
            end do
#Else
            do idxs=0,1
             if( boundaryCondition(idxs,0).eq.tractionBC )then
              i2=n2a
              i1=n1a*(1-idxs)+n1b*idxs
              if (addBoundaryForcing(side,axis).eq.0) then
               SolveForDnInGP((0.5*(Egp(Jac,rx,rx,lam2muM)+Egp(Jac,ry,ry,muM))),\
               (0.5*(Egp(Jac,rx,ry,lamM)+Egp(Jac,ry,rx,muM))),\
               (0.5*(Egp(Jac,rx,ry,muM)+Egp(Jac,ry,rx,lamM))),\
               (0.5*(Egp(Jac,ry,ry,lam2muM)+Egp(Jac,rx,rx,muM))),\
               -(0.5*(Eip(Jac,rx,rx,lam2muM)+Eip(Jac,ry,ry,muM))*(Dnip(uc))\
               +0.5*(Eip(Jac,rx,ry,lamM)+Eip(Jac,ry,rx,muM))*(Dnip(vc))\
               +(E0(Jac,rx,qx,lam2muM)+E0(Jac,ry,qy,muM))*(DtC(uc,0,(1-2*idxs)))\
               +(E0(Jac,rx,qy,lamM)+E0(Jac,ry,qx,muM))*(DtC(vc,0,(1-2*idxs)))),\
               -(0.5*(Eip(Jac,rx,ry,muM)+Eip(Jac,ry,rx,lamM))*(Dnip(uc))\
               +0.5*(Eip(Jac,ry,ry,lam2muM)+Eip(Jac,rx,rx,muM))*(Dnip(vc))\
               +(E0(Jac,rx,qy,muM)+E0(Jac,ry,qx,lamM))*(DtC(uc,0,(1-2*idxs)))\
               +(E0(Jac,rx,qx,muM)+E0(Jac,ry,qy,lam2muM))*(DtC(vc,0,(1-2*idxs)))))\
              else
               SolveForDnInGP((0.5*(Egp(Jac,rx,rx,lam2muM)+Egp(Jac,ry,ry,muM))),\
               (0.5*(Egp(Jac,rx,ry,lamM)+Egp(Jac,ry,rx,muM))),\
               (0.5*(Egp(Jac,rx,ry,muM)+Egp(Jac,ry,rx,lamM))),\
               (0.5*(Egp(Jac,ry,ry,lam2muM)+Egp(Jac,rx,rx,muM))),\
               -(0.5*(Eip(Jac,rx,rx,lam2muM)+Eip(Jac,ry,ry,muM))*(Dnip(uc))\
               +0.5*(Eip(Jac,rx,ry,lamM)+Eip(Jac,ry,rx,muM))*(Dnip(vc))\
               +(E0(Jac,rx,qx,lam2muM)+E0(Jac,ry,qy,muM))*(DtC(uc,0,(1-2*idxs)))\
               +(E0(Jac,rx,qy,lamM)+E0(Jac,ry,qx,muM))*(DtC(vc,0,(1-2*idxs))))\
               -is*Jac(i1,i2)*(sqrt(rx(i1,i2)*rx(i1,i2)+ry(i1,i2)*ry(i1,i2)))*(bcf(side,axis,i1,i2,i3,uc)),\
               -(0.5*(Eip(Jac,rx,ry,muM)+Eip(Jac,ry,rx,lamM))*(Dnip(uc))\
               +0.5*(Eip(Jac,ry,ry,lam2muM)+Eip(Jac,rx,rx,muM))*(Dnip(vc))\
               +(E0(Jac,rx,qy,muM)+E0(Jac,ry,qx,lamM))*(DtC(uc,0,(1-2*idxs)))\
               +(E0(Jac,rx,qx,muM)+E0(Jac,ry,qy,lam2muM))*(DtC(vc,0,(1-2*idxs))))\
               -is*Jac(i1,i2)*(sqrt(rx(i1,i2)*rx(i1,i2)+ry(i1,i2)*ry(i1,i2)))*(bcf(side,axis,i1,i2,i3,vc)))
              end if
              u(i1-is1,i2-is2,i3,uc)=u(i1,i2,i3,uc)-(is1+is2)*f(0)*dr(axis)
              u(i1-is1,i2-is2,i3,vc)=u(i1,i2,i3,vc)-(is1+is2)*f(1)*dr(axis)
             end if
            end do
#End
           end if
#endMacro

!     ! 3D stuff
#defineMacro E3gp(MAT,AX1,AX2,C1,C2) (0.5*(JAC3(i1-is1,i2-is2,i3-is3)*rsxy(i1-is1,i2-is2,i3-is3,AX1,C1)\
*     rsxy(i1-is1,i2-is2,i3-is3,AX2,C2)*MAT(i1-is1,i2-is2,i3-is3)+JAC3(i1,i2,i3)*rsxy(i1,i2,i3,AX1,C1)*rsxy(i1,i2,i3,AX2,C2)*MAT(i1,i2,i3)))
#defineMacro E3ip(MAT,AX1,AX2,C1,C2) (0.5*(JAC3(i1+is1,i2+is2,i3+is3)*rsxy(i1+is1,i2+is2,i3+is3,AX1,C1)\
*     rsxy(i1+is1,i2+is2,i3+is3,AX2,C2)*MAT(i1+is1,i2+is2,i3+is3)+JAC3(i1,i2,i3)*rsxy(i1,i2,i3,AX1,C1)*rsxy(i1,i2,i3,AX2,C2)*MAT(i1,i2,i3)))
#defineMacro E30(MAT,AX1,AX2,C1,C2) (JAC3(i1,i2,i3)*rsxy(i1,i2,i3,AX1,C1)*rsxy(i1,i2,i3,AX2,C2)*MAT(i1,i2,i3))

#beginMacro assignMatrix(a,b,c,ae1,ae2,ae3)
           ae1=0.5*(E3gp(lam2mu3M,axis,axis,a,a)+E3gp(mu3M,axis,axis,b,b)+E3gp(mu3M,axis,axis,c,c))
           ae2=0.5*(E3gp(lam3M,axis,axis,a,b)+E3gp(mu3M,axis,axis,b,a))
           ae3=0.5*(E3gp(lam3M,axis,axis,a,c)+E3gp(mu3M,axis,axis,c,a))
#endMacro

#defineMacro D0axp1(uc) is*0.5*dri(axisp1)*(u(i1+is3,i2+is1,i3+is2,uc)-u(i1-is3,i2-is1,i3-is2,uc))
#defineMacro D0axp2(uc) is*0.5*dri(axisp2)*(u(i1+is2,i2+is3,i3+is1,uc)-u(i1-is2,i2-is3,i3-is1,uc))
#defineMacro D3nip(uc) is*(dri(axis)*(u(i1+is1,i2+is2,i3+is3,uc)-u(i1,i2,i3,uc)))
#defineMacro RHTERM(MAT,aa,bb,cc) 0.5*E3ip(MAT,axis,axis,aa,bb)*D3nip(cc)+E30(MAT,axis,axisp1,aa,bb)*D0axp1(cc)+E30(MAT,axis,axisp2,aa,bb)*D0axp2(cc)
#beginMacro assignRHside3D(a,b,c,rh1)
           rh1=RHTERM(lam2mu3M,a,a,a)+RHTERM(lam3M,a,b,b)+RHTERM(lam3M,a,c,c)+RHTERM(mu3M,b,b,a)+RHTERM(mu3M,b,a,b)+RHTERM(mu3M,c,c,a)+RHTERM(mu3M,c,a,c)
#endMacro

!     Edge stuff
#defineMacro D0axp1E(uc) is*(1.d0-iep11*iep12*0.5d0)*dri(axisp1)*(u(i1+is3*iep11,i2+is1*iep11,i3+is2*iep11,uc)-u(i1-is3*iep12,i2-is1*iep12,i3-is2*iep12,uc))
#defineMacro D0axp2E(uc) is*(1.d0-iep21*iep22*0.5d0)*dri(axisp2)*(u(i1+is2*iep21,i2+is3*iep21,i3+is1*iep21,uc)-u(i1-is2*iep22,i2-is3*iep22,i3-is1*iep22,uc))
#defineMacro RHTERME(MAT,aa,bb,cc) 0.5*E3ip(MAT,axis,axis,aa,bb)*D3nip(cc)+E30(MAT,axis,axisp1,aa,bb)*D0axp1E(cc)+E30(MAT,axis,axisp2,aa,bb)*D0axp2E(cc)
#beginMacro assignRHside3DE(a,b,c,rh1)
           rh1=RHTERME(lam2mu3M,a,a,a)+RHTERME(lam3M,a,b,b)+RHTERME(lam3M,a,c,c)+RHTERME(mu3M,b,b,a)+RHTERME(mu3M,b,a,b)+RHTERME(mu3M,c,c,a)+RHTERME(mu3M,c,a,c)
#endMacro

! ================================================================================================
! ================================================================================================
#beginMacro doEdge()
 if( twilightZone.eq.0 )then
   if(addBoundaryForcing(side,axis).eq.0) then
    beginLoopsMask3dE()
    assignRHside3DE(uc,vc,wc,f(0))
    assignRHside3DE(vc,wc,uc,f(1))
    assignRHside3DE(wc,uc,vc,f(2))
    assignMatrix(uc,vc,wc,a3(0,0),a3(0,1),a3(0,2))
    assignMatrix(vc,wc,uc,a3(1,1),a3(1,2),a3(1,0))
    assignMatrix(wc,uc,vc,a3(2,2),a3(2,0),a3(2,1))
    f(0) = -f(0)
    f(1) = -f(1)
    f(2) = -f(2)
    call dgeco( a3(0,0), 3, 3, ipvt(0),rcond,work(0))
    call dgesl( a3(0,0), 3, 3, ipvt(0), f(0), job)
    u(i1-is1,i2-is2,i3-is3,uc)=u(i1,i2,i3,uc)-is*f(0)*dr(axis)
    u(i1-is1,i2-is2,i3-is3,vc)=u(i1,i2,i3,vc)-is*f(1)*dr(axis)
    u(i1-is1,i2-is2,i3-is3,wc)=u(i1,i2,i3,wc)-is*f(2)*dr(axis)
    endLoopsMask3d()
   else
    beginLoopsMask3dE()
    assignRHside3DE(uc,vc,wc,f(0))
    assignRHside3DE(vc,wc,uc,f(1))
    assignRHside3DE(wc,uc,vc,f(2))
    assignMatrix(uc,vc,wc,a3(0,0),a3(0,1),a3(0,2))
    assignMatrix(vc,wc,uc,a3(1,1),a3(1,2),a3(1,0))
    assignMatrix(wc,uc,vc,a3(2,2),a3(2,0),a3(2,1))
    an1=rsxy(i1,i2,i3,axis,0) ! normal (an1,an2,an3)
    an2=rsxy(i1,i2,i3,axis,1)
    an3=rsxy(i1,i2,i3,axis,2)
    alpha=lam2mu3M(i1,i2,i3)
    mu=mu3M(i1,i2,i3)
    lambda=lam3M(i1,i2,i3)
    !     subtract off twilight
    f(0) = -f(0) - is*Jac3(i1,i2,i3)*(bcf(side,axis,i1,i2,i3,uc))
    f(1) = -f(1) - is*Jac3(i1,i2,i3)*(bcf(side,axis,i1,i2,i3,vc))
    f(2) = -f(2) - is*Jac3(i1,i2,i3)*(bcf(side,axis,i1,i2,i3,wc))
    call dgeco( a3(0,0), 3, 3, ipvt(0),rcond,work(0))
    call dgesl( a3(0,0), 3, 3, ipvt(0), f(0), job)
    u(i1-is1,i2-is2,i3-is3,uc)=u(i1,i2,i3,uc)-is*f(0)*dr(axis)
    u(i1-is1,i2-is2,i3-is3,vc)=u(i1,i2,i3,vc)-is*f(1)*dr(axis)
    u(i1-is1,i2-is2,i3-is3,wc)=u(i1,i2,i3,wc)-is*f(2)*dr(axis)
    endLoopsMask3d()
   end if
 else
   beginLoopsMask3dE()
   assignRHside3DE(uc,vc,wc,f(0))
   assignRHside3DE(vc,wc,uc,f(1))
   assignRHside3DE(wc,uc,vc,f(2))
   assignMatrix(uc,vc,wc,a3(0,0),a3(0,1),a3(0,2))
   assignMatrix(vc,wc,uc,a3(1,1),a3(1,2),a3(1,0))
   assignMatrix(wc,uc,vc,a3(2,2),a3(2,0),a3(2,1))
   OGDERIV3D(0,1,0,0,i1,i2,i3,t,ux0,vx0,wx0)
   OGDERIV3D(0,0,1,0,i1,i2,i3,t,uy0,vy0,wy0)
   OGDERIV3D(0,0,0,1,i1,i2,i3,t,uz0,vz0,wz0)
   an1=rsxy(i1,i2,i3,axis,0) ! normal (an1,an2,an3)
   an2=rsxy(i1,i2,i3,axis,1)
   an3=rsxy(i1,i2,i3,axis,2)
   alpha=lam2mu3M(i1,i2,i3)
   mu=mu3M(i1,i2,i3)
   lambda=lam3M(i1,i2,i3)
   !     subtract off twilight
   f(0) = -f(0) + Jac3(i1,i2,i3)*( an1*(alpha*ux0+lambda*(vy0+wz0))+an2*(mu*(uy0+vx0))+an3*(mu*(uz0+wx0)))
   f(1) = -f(1) + Jac3(i1,i2,i3)*( an1*(mu*(uy0+vx0))+an2*(lambda*(ux0+wz0)+alpha*vy0)+an3*(mu*(vz0+wy0)))
   f(2) = -f(2) + Jac3(i1,i2,i3)*( an1*(mu*(uz0+wx0))+an2*(mu*(vz0+wy0))+an3*(lambda*(ux0+vy0)+alpha*wz0))
   call dgeco( a3(0,0), 3, 3, ipvt(0),rcond,work(0))
   call dgesl( a3(0,0), 3, 3, ipvt(0), f(0), job)
   u(i1-is1,i2-is2,i3-is3,uc)=u(i1,i2,i3,uc)-is*f(0)*dr(axis)
   u(i1-is1,i2-is2,i3-is3,vc)=u(i1,i2,i3,vc)-is*f(1)*dr(axis)
   u(i1-is1,i2-is2,i3-is3,wc)=u(i1,i2,i3,wc)-is*f(2)*dr(axis)
   endLoopsMask3d()
 end if
#endMacro


!     ! 3D Cartesian stuff


#defineMacro EC3gp(MAT) (0.5*(MAT(i1-is1,i2-is2,i3-is3)+MAT(i1,i2,i3)))
#defineMacro EC3ip(MAT) (0.5*(MAT(i1+is1,i2+is2,i3+is3)+MAT(i1,i2,i3)))
#defineMacro EC30(MAT) (MAT(i1,i2,i3))
#defineMacro DC0axp1(uc) is*0.5*dxi(axisp1)*(u(i1+is3,i2+is1,i3+is2,uc)-u(i1-is3,i2-is1,i3-is2,uc))
#defineMacro DC0axp2(uc) is*0.5*dxi(axisp2)*(u(i1+is2,i2+is3,i3+is1,uc)-u(i1-is2,i2-is3,i3-is1,uc))
#defineMacro DC3nip(uc) is*(dxi(axis)*(u(i1+is1,i2+is2,i3+is3,uc)-u(i1,i2,i3,uc)))


!     ==========================================================================
!     Apply a stress free BC -- rectangular and 3d
!     
!     FORCING equals noForcing or forcing
!     ==========================================================================
#beginMacro tractionBCRectangular3dMacro(FORCING)
           if( axis.eq.0 )then
!     (2 mu lam) u.x = -lam (v.y+w.z)
!     mu v.x = - mu u.y
!     mu w.x = -mu u.z
            beginLoopsMask3d()
#If #FORCING eq "forcing"
            OGDERIV3D(0,1,0,0,i1,i2,i3,t,ux0,vx0,wx0)
            OGDERIV3D(0,0,1,0,i1,i2,i3,t,uy0,vy0,wy0)
            OGDERIV3D(0,0,0,1,i1,i2,i3,t,uz0,vz0,wz0)
            u(i1-is1,i2-is2,i3-is3,uc)=u(i1,i2,i3,uc)-is*dx(0)*2.*(1.d0/EC3gp(lam2mu3M))*(-(0.5d0*EC3ip(lam2mu3M)*DC3nip(uc)+EC30(lam3M)*(DC0axp1(vc)+DC0axp2(wc)))+EC30(lam2mu3M)*ux0+EC30(lam3M)*(vy0+wz0))
            u(i1-is1,i2-is2,i3-is3,vc)=u(i1,i2,i3,vc)-is*dx(0)*2.*(1.d0/EC3gp(mu3M))*(-(0.5d0*EC3ip(mu3M)*DC3nip(vc)+EC30(mu3M)*(DC0axp1(uc)))+EC30(mu3M)*vx0+EC30(mu3M)*(uy0))
            u(i1-is1,i2-is2,i3-is3,wc)=u(i1,i2,i3,wc)-is*dx(0)*2.*(1.d0/EC3gp(mu3M))*(-(0.5d0*EC3ip(mu3M)*DC3nip(wc)+EC30(mu3M)*(DC0axp2(uc)))+EC30(mu3M)*wx0+EC30(mu3M)*(uz0))
#Else
            if (addBoundaryForcing(side,axis).eq.0) then
!     No forcing
             u(i1-is1,i2-is2,i3-is3,uc)=u(i1,i2,i3,uc)-is*dx(0)*2.*(1.d0/EC3gp(lam2mu3M))*(-(0.5d0*EC3ip(lam2mu3M)*DC3nip(uc)+EC30(lam3M)*(DC0axp1(vc)+DC0axp2(wc))))
             u(i1-is1,i2-is2,i3-is3,vc)=u(i1,i2,i3,vc)-is*dx(0)*2.*(1.d0/EC3gp(mu3M))*(-(0.5d0*EC3ip(mu3M)*DC3nip(vc)+EC30(mu3M)*(DC0axp1(uc))))
             u(i1-is1,i2-is2,i3-is3,wc)=u(i1,i2,i3,wc)-is*dx(0)*2.*(1.d0/EC3gp(mu3M))*(-(0.5d0*EC3ip(mu3M)*DC3nip(wc)+EC30(mu3M)*(DC0axp2(uc))))
            else
!     Forcing
             u(i1-is1,i2-is2,i3-is3,uc)=u(i1,i2,i3,uc)-is*dx(0)*2.*(1.d0/EC3gp(lam2mu3M))*(-(0.5d0*EC3ip(lam2mu3M)*DC3nip(uc)+EC30(lam3M)*(DC0axp1(vc)+DC0axp2(wc)))-is*bcf(side,axis,i1,i2,i3,uc))
             u(i1-is1,i2-is2,i3-is3,vc)=u(i1,i2,i3,vc)-is*dx(0)*2.*(1.d0/EC3gp(mu3M))*(-(0.5d0*EC3ip(mu3M)*DC3nip(vc)+EC30(mu3M)*(DC0axp1(uc)))-is*bcf(side,axis,i1,i2,i3,vc))
             u(i1-is1,i2-is2,i3-is3,wc)=u(i1,i2,i3,wc)-is*dx(0)*2.*(1.d0/EC3gp(mu3M))*(-(0.5d0*EC3ip(mu3M)*DC3nip(wc)+EC30(mu3M)*(DC0axp2(uc)))-is*bcf(side,axis,i1,i2,i3,wc))
            endif
#End
            endLoopsMask3d()

           else if( axis.eq.1 )then
!     mu u.y = - mu v.x
!     lam2mu v.y = -lam*(u.x+w.z)
!     mu w.y = - mu v.z
            beginLoopsMask3d()
#If #FORCING eq "forcing"
            OGDERIV3D(0,1,0,0,i1,i2,i3,t,ux0,vx0,wx0)
            OGDERIV3D(0,0,1,0,i1,i2,i3,t,uy0,vy0,wy0)
            OGDERIV3D(0,0,0,1,i1,i2,i3,t,uz0,vz0,wz0)
            u(i1-is1,i2-is2,i3-is3,vc)=u(i1,i2,i3,vc)-is*dx(1)*2.*(1.d0/EC3gp(lam2mu3M))*(-(0.5d0*EC3ip(lam2mu3M)*DC3nip(vc)+EC30(lam3M)*(DC0axp1(wc)+DC0axp2(uc)))+EC30(lam2mu3M)*vy0+EC30(lam3M)*(ux0+wz0))
            u(i1-is1,i2-is2,i3-is3,wc)=u(i1,i2,i3,wc)-is*dx(1)*2.*(1.d0/EC3gp(mu3M))*(-(0.5d0*EC3ip(mu3M)*DC3nip(wc)+EC30(mu3M)*(DC0axp1(vc)))+EC30(mu3M)*wy0+EC30(mu3M)*(vz0))
            u(i1-is1,i2-is2,i3-is3,uc)=u(i1,i2,i3,uc)-is*dx(1)*2.*(1.d0/EC3gp(mu3M))*(-(0.5d0*EC3ip(mu3M)*DC3nip(uc)+EC30(mu3M)*(DC0axp2(vc)))+EC30(mu3M)*uy0+EC30(mu3M)*(vx0))
#Else
            if (addBoundaryForcing(side,axis).eq.0) then
!     No forcing
             u(i1-is1,i2-is2,i3-is3,vc)=u(i1,i2,i3,vc)-is*dx(1)*2.*(1.d0/EC3gp(lam2mu3M))*(-(0.5d0*EC3ip(lam2mu3M)*DC3nip(vc)+EC30(lam3M)*(DC0axp1(wc)+DC0axp2(uc))))
             u(i1-is1,i2-is2,i3-is3,wc)=u(i1,i2,i3,wc)-is*dx(1)*2.*(1.d0/EC3gp(mu3M))*(-(0.5d0*EC3ip(mu3M)*DC3nip(wc)+EC30(mu3M)*(DC0axp1(vc))))
             u(i1-is1,i2-is2,i3-is3,uc)=u(i1,i2,i3,uc)-is*dx(1)*2.*(1.d0/EC3gp(mu3M))*(-(0.5d0*EC3ip(mu3M)*DC3nip(uc)+EC30(mu3M)*(DC0axp2(vc))))
            else
           ! *wdh* 091109 bcf wrong
             u(i1-is1,i2-is2,i3-is3,vc)=u(i1,i2,i3,vc)-is*dx(1)*2.*(1.d0/EC3gp(lam2mu3M))*(-(0.5d0*EC3ip(lam2mu3M)*DC3nip(vc)+EC30(lam3M)*(DC0axp1(wc)+DC0axp2(uc)))-is*bcf(side,axis,i1,i2,i3,vc))
             u(i1-is1,i2-is2,i3-is3,wc)=u(i1,i2,i3,wc)-is*dx(1)*2.*(1.d0/EC3gp(mu3M))*(-(0.5d0*EC3ip(mu3M)*DC3nip(wc)+EC30(mu3M)*(DC0axp1(vc)))-is*bcf(side,axis,i1,i2,i3,wc))
             u(i1-is1,i2-is2,i3-is3,uc)=u(i1,i2,i3,uc)-is*dx(1)*2.*(1.d0/EC3gp(mu3M))*(-(0.5d0*EC3ip(mu3M)*DC3nip(uc)+EC30(mu3M)*(DC0axp2(vc)))-is*bcf(side,axis,i1,i2,i3,uc))
            end if
#End
            endLoopsMask3d()
           else if( axis.eq.2 )then
!     mu u.z = - mu w.x
!     mu v.z = - mu w.y
!     lam2mu w.z = -lam*(u.x+v.y)
            beginLoopsMask3d()
#If #FORCING eq "forcing"
            OGDERIV3D(0,1,0,0,i1,i2,i3,t,ux0,vx0,wx0)
            OGDERIV3D(0,0,1,0,i1,i2,i3,t,uy0,vy0,wy0)
            OGDERIV3D(0,0,0,1,i1,i2,i3,t,uz0,vz0,wz0)
            u(i1-is1,i2-is2,i3-is3,wc)=u(i1,i2,i3,wc)-is*dx(2)*2.*(1.d0/EC3gp(lam2mu3M))*(-(0.5d0*EC3ip(lam2mu3M)*DC3nip(wc)+EC30(lam3M)*(DC0axp1(uc)+DC0axp2(vc)))+EC30(lam2mu3M)*wz0+EC30(lam3M)*(vy0+ux0))
            u(i1-is1,i2-is2,i3-is3,uc)=u(i1,i2,i3,uc)-is*dx(2)*2.*(1.d0/EC3gp(mu3M))*(-(0.5d0*EC3ip(mu3M)*DC3nip(uc)+EC30(mu3M)*(DC0axp1(wc)))+EC30(mu3M)*wx0+EC30(mu3M)*(uz0))
            u(i1-is1,i2-is2,i3-is3,vc)=u(i1,i2,i3,vc)-is*dx(2)*2.*(1.d0/EC3gp(mu3M))*(-(0.5d0*EC3ip(mu3M)*DC3nip(vc)+EC30(mu3M)*(DC0axp2(wc)))+EC30(mu3M)*wy0+EC30(mu3M)*(vz0))
#Else
            if (addBoundaryForcing(side,axis).eq.0) then
             u(i1-is1,i2-is2,i3-is3,wc)=u(i1,i2,i3,wc)-is*dx(2)*2.*(1.d0/EC3gp(lam2mu3M))*(-(0.5d0*EC3ip(lam2mu3M)*DC3nip(wc)+EC30(lam3M)*(DC0axp1(uc)+DC0axp2(vc))))
             u(i1-is1,i2-is2,i3-is3,uc)=u(i1,i2,i3,uc)-is*dx(2)*2.*(1.d0/EC3gp(mu3M))*(-(0.5d0*EC3ip(mu3M)*DC3nip(uc)+EC30(mu3M)*(DC0axp1(wc))))
             u(i1-is1,i2-is2,i3-is3,vc)=u(i1,i2,i3,vc)-is*dx(2)*2.*(1.d0/EC3gp(mu3M))*(-(0.5d0*EC3ip(mu3M)*DC3nip(vc)+EC30(mu3M)*(DC0axp2(wc))))
            else
             u(i1-is1,i2-is2,i3-is3,wc)=u(i1,i2,i3,wc)-is*dx(2)*2.*(1.d0/EC3gp(lam2mu3M))*(-(0.5d0*EC3ip(lam2mu3M)*DC3nip(wc)+EC30(lam3M)*(DC0axp1(uc)+DC0axp2(vc)))-is*bcf(side,axis,i1,i2,i3,wc))
             u(i1-is1,i2-is2,i3-is3,uc)=u(i1,i2,i3,uc)-is*dx(2)*2.*(1.d0/EC3gp(mu3M))*(-(0.5d0*EC3ip(mu3M)*DC3nip(uc)+EC30(mu3M)*(DC0axp1(wc)))-is*bcf(side,axis,i1,i2,i3,uc))
             u(i1-is1,i2-is2,i3-is3,vc)=u(i1,i2,i3,vc)-is*dx(2)*2.*(1.d0/EC3gp(mu3M))*(-(0.5d0*EC3ip(mu3M)*DC3nip(vc)+EC30(mu3M)*(DC0axp2(wc)))-is*bcf(side,axis,i1,i2,i3,vc))
            end if
#End
            endLoopsMask3d()
           end if
#endMacro

!     Special cases for the edge and corners
#defineMacro DEC0axp1(uc) is*(1.d0-iep11*iep12*0.5d0)*dxi(axisp1)*(u(i1+is3*iep11,i2+is1*iep11,i3+is2*iep11,uc)-u(i1-is3*iep12,i2-is1*iep12,i3-is2*iep12,uc))
#defineMacro DEC0axp2(uc) is*(1.d0-iep21*iep22*0.5d0)*dxi(axisp2)*(u(i1+is2*iep21,i2+is3*iep21,i3+is1*iep21,uc)-u(i1-is2*iep22,i2-is3*iep22,i3-is1*iep22,uc))

#beginMacro tractionBCRectangular3dMacroX(FORCING)
!     (2 mu lam) u.x = -lam (v.y+w.z)
!     mu v.x = - mu u.y
!     mu w.x = -mu u.z
           beginLoopsMask3dE()
#If #FORCING eq "forcing"
           OGDERIV3D(0,1,0,0,i1,i2,i3,t,ux0,vx0,wx0)
           OGDERIV3D(0,0,1,0,i1,i2,i3,t,uy0,vy0,wy0)
           OGDERIV3D(0,0,0,1,i1,i2,i3,t,uz0,vz0,wz0)
           u(i1-is1,i2-is2,i3-is3,uc)=u(i1,i2,i3,uc)-is*dx(0)*2.*(1.d0/EC3gp(lam2mu3M))*(-(0.5d0*EC3ip(lam2mu3M)*DC3nip(uc)+EC30(lam3M)*(DEC0axp1(vc)+DEC0axp2(wc)))+EC30(lam2mu3M)*ux0+EC30(lam3M)*(vy0+wz0))
           u(i1-is1,i2-is2,i3-is3,vc)=u(i1,i2,i3,vc)-is*dx(0)*2.*(1.d0/EC3gp(mu3M))*(-(0.5d0*EC3ip(mu3M)*DC3nip(vc)+EC30(mu3M)*(DEC0axp1(uc)))+EC30(mu3M)*vx0+EC30(mu3M)*(uy0))
           u(i1-is1,i2-is2,i3-is3,wc)=u(i1,i2,i3,wc)-is*dx(0)*2.*(1.d0/EC3gp(mu3M))*(-(0.5d0*EC3ip(mu3M)*DC3nip(wc)+EC30(mu3M)*(DEC0axp2(uc)))+EC30(mu3M)*wx0+EC30(mu3M)*(uz0))
#Else
           if (addBoundaryForcing(side,axis).eq.0) then
            u(i1-is1,i2-is2,i3-is3,uc)=u(i1,i2,i3,uc)-is*dx(0)*2.*(1.d0/EC3gp(lam2mu3M))*(-(0.5d0*EC3ip(lam2mu3M)*DC3nip(uc)+EC30(lam3M)*(DEC0axp1(vc)+DEC0axp2(wc))))
            u(i1-is1,i2-is2,i3-is3,vc)=u(i1,i2,i3,vc)-is*dx(0)*2.*(1.d0/EC3gp(mu3M))*(-(0.5d0*EC3ip(mu3M)*DC3nip(vc)+EC30(mu3M)*(DEC0axp1(uc))))
            u(i1-is1,i2-is2,i3-is3,wc)=u(i1,i2,i3,wc)-is*dx(0)*2.*(1.d0/EC3gp(mu3M))*(-(0.5d0*EC3ip(mu3M)*DC3nip(wc)+EC30(mu3M)*(DEC0axp2(uc))))
           else
            u(i1-is1,i2-is2,i3-is3,uc)=u(i1,i2,i3,uc)-is*dx(0)*2.*(1.d0/EC3gp(lam2mu3M))*(-(0.5d0*EC3ip(lam2mu3M)*DC3nip(uc)+EC30(lam3M)*(DEC0axp1(vc)+DEC0axp2(wc)))-is*bcf(side,axis,i1,i2,i3,uc))
            u(i1-is1,i2-is2,i3-is3,vc)=u(i1,i2,i3,vc)-is*dx(0)*2.*(1.d0/EC3gp(mu3M))*(-(0.5d0*EC3ip(mu3M)*DC3nip(vc)+EC30(mu3M)*(DEC0axp1(uc)))-is*bcf(side,axis,i1,i2,i3,vc))
            u(i1-is1,i2-is2,i3-is3,wc)=u(i1,i2,i3,wc)-is*dx(0)*2.*(1.d0/EC3gp(mu3M))*(-(0.5d0*EC3ip(mu3M)*DC3nip(wc)+EC30(mu3M)*(DEC0axp2(uc)))-is*bcf(side,axis,i1,i2,i3,wc))
           end if
#End
           endLoopsMask3d()
#endMacro

#beginMacro tractionBCRectangular3dMacroY(FORCING)
!     mu u.y = - mu v.x
!     lam2mu v.y = -lam*(u.x+w.z)
!     mu w.y = - mu v.z
           beginLoopsMask3dE()
#If #FORCING eq "forcing"
           OGDERIV3D(0,1,0,0,i1,i2,i3,t,ux0,vx0,wx0)
           OGDERIV3D(0,0,1,0,i1,i2,i3,t,uy0,vy0,wy0)
           OGDERIV3D(0,0,0,1,i1,i2,i3,t,uz0,vz0,wz0)
           u(i1-is1,i2-is2,i3-is3,vc)=u(i1,i2,i3,vc)-is*dx(1)*2.*(1.d0/EC3gp(lam2mu3M))*(-(0.5d0*EC3ip(lam2mu3M)*DC3nip(vc)+EC30(lam3M)*(DEC0axp1(wc)+DEC0axp2(uc)))+EC30(lam2mu3M)*vy0+EC30(lam3M)*(ux0+wz0))
           u(i1-is1,i2-is2,i3-is3,wc)=u(i1,i2,i3,wc)-is*dx(1)*2.*(1.d0/EC3gp(mu3M))*(-(0.5d0*EC3ip(mu3M)*DC3nip(wc)+EC30(mu3M)*(DEC0axp1(vc)))+EC30(mu3M)*wy0+EC30(mu3M)*(vz0))
           u(i1-is1,i2-is2,i3-is3,uc)=u(i1,i2,i3,uc)-is*dx(1)*2.*(1.d0/EC3gp(mu3M))*(-(0.5d0*EC3ip(mu3M)*DC3nip(uc)+EC30(mu3M)*(DEC0axp2(vc)))+EC30(mu3M)*uy0+EC30(mu3M)*(vx0))
#Else
           if (addBoundaryForcing(side,axis).eq.0) then
            u(i1-is1,i2-is2,i3-is3,vc)=u(i1,i2,i3,vc)-is*dx(1)*2.*(1.d0/EC3gp(lam2mu3M))*(-(0.5d0*EC3ip(lam2mu3M)*DC3nip(vc)+EC30(lam3M)*(DEC0axp1(wc)+DEC0axp2(uc))))
            u(i1-is1,i2-is2,i3-is3,wc)=u(i1,i2,i3,wc)-is*dx(1)*2.*(1.d0/EC3gp(mu3M))*(-(0.5d0*EC3ip(mu3M)*DC3nip(wc)+EC30(mu3M)*(DEC0axp1(vc))))
            u(i1-is1,i2-is2,i3-is3,uc)=u(i1,i2,i3,uc)-is*dx(1)*2.*(1.d0/EC3gp(mu3M))*(-(0.5d0*EC3ip(mu3M)*DC3nip(uc)+EC30(mu3M)*(DEC0axp2(vc))))
           else
            ! *wdh* 091109 -- bcf --
            u(i1-is1,i2-is2,i3-is3,vc)=u(i1,i2,i3,vc)-is*dx(1)*2.*(1.d0/EC3gp(lam2mu3M))*(-(0.5d0*EC3ip(lam2mu3M)*DC3nip(vc)+EC30(lam3M)*(DEC0axp1(wc)+DEC0axp2(uc)))-is*bcf(side,axis,i1,i2,i3,vc))
            u(i1-is1,i2-is2,i3-is3,wc)=u(i1,i2,i3,wc)-is*dx(1)*2.*(1.d0/EC3gp(mu3M))*(-(0.5d0*EC3ip(mu3M)*DC3nip(wc)+EC30(mu3M)*(DEC0axp1(vc)))-is*bcf(side,axis,i1,i2,i3,wc))
            u(i1-is1,i2-is2,i3-is3,uc)=u(i1,i2,i3,uc)-is*dx(1)*2.*(1.d0/EC3gp(mu3M))*(-(0.5d0*EC3ip(mu3M)*DC3nip(uc)+EC30(mu3M)*(DEC0axp2(vc)))-is*bcf(side,axis,i1,i2,i3,uc))
           end if
#End
           endLoopsMask3d()
#endMacro

#beginMacro tractionBCRectangular3dMacroZ(FORCING)
!     mu u.z = - mu w.x
!     mu v.z = - mu w.y
!     lam2mu w.z = -lam*(u.x+v.y)
           beginLoopsMask3dE()
#If #FORCING eq "forcing"
           OGDERIV3D(0,1,0,0,i1,i2,i3,t,ux0,vx0,wx0)
           OGDERIV3D(0,0,1,0,i1,i2,i3,t,uy0,vy0,wy0)
           OGDERIV3D(0,0,0,1,i1,i2,i3,t,uz0,vz0,wz0)
           u(i1-is1,i2-is2,i3-is3,wc)=u(i1,i2,i3,wc)-is*dx(2)*2.*(1.d0/EC3gp(lam2mu3M))*(-(0.5d0*EC3ip(lam2mu3M)*DC3nip(wc)+EC30(lam3M)*(DEC0axp1(uc)+DEC0axp2(vc)))+EC30(lam2mu3M)*wz0+EC30(lam3M)*(vy0+ux0))
           u(i1-is1,i2-is2,i3-is3,uc)=u(i1,i2,i3,uc)-is*dx(2)*2.*(1.d0/EC3gp(mu3M))*(-(0.5d0*EC3ip(mu3M)*DC3nip(uc)+EC30(mu3M)*(DEC0axp1(wc)))+EC30(mu3M)*wx0+EC30(mu3M)*(uz0))
           u(i1-is1,i2-is2,i3-is3,vc)=u(i1,i2,i3,vc)-is*dx(2)*2.*(1.d0/EC3gp(mu3M))*(-(0.5d0*EC3ip(mu3M)*DC3nip(vc)+EC30(mu3M)*(DEC0axp2(wc)))+EC30(mu3M)*wy0+EC30(mu3M)*(vz0))
#Else
           if (addBoundaryForcing(side,axis).eq.0) then
            u(i1-is1,i2-is2,i3-is3,wc)=u(i1,i2,i3,wc)-is*dx(2)*2.*(1.d0/EC3gp(lam2mu3M))*(-(0.5d0*EC3ip(lam2mu3M)*DC3nip(wc)+EC30(lam3M)*(DEC0axp1(uc)+DEC0axp2(vc))))
            u(i1-is1,i2-is2,i3-is3,uc)=u(i1,i2,i3,uc)-is*dx(2)*2.*(1.d0/EC3gp(mu3M))*(-(0.5d0*EC3ip(mu3M)*DC3nip(uc)+EC30(mu3M)*(DEC0axp1(wc))))
            u(i1-is1,i2-is2,i3-is3,vc)=u(i1,i2,i3,vc)-is*dx(2)*2.*(1.d0/EC3gp(mu3M))*(-(0.5d0*EC3ip(mu3M)*DC3nip(vc)+EC30(mu3M)*(DEC0axp2(wc))))
           else
            ! *wdh* 091109 -- bcf --
            u(i1-is1,i2-is2,i3-is3,wc)=u(i1,i2,i3,wc)-is*dx(2)*2.*(1.d0/EC3gp(lam2mu3M))*(-(0.5d0*EC3ip(lam2mu3M)*DC3nip(wc)+EC30(lam3M)*(DEC0axp1(uc)+DEC0axp2(vc)))-is*bcf(side,axis,i1,i2,i3,wc))
            u(i1-is1,i2-is2,i3-is3,uc)=u(i1,i2,i3,uc)-is*dx(2)*2.*(1.d0/EC3gp(mu3M))*(-(0.5d0*EC3ip(mu3M)*DC3nip(uc)+EC30(mu3M)*(DEC0axp1(wc)))-is*bcf(side,axis,i1,i2,i3,uc))
            u(i1-is1,i2-is2,i3-is3,vc)=u(i1,i2,i3,vc)-is*dx(2)*2.*(1.d0/EC3gp(mu3M))*(-(0.5d0*EC3ip(mu3M)*DC3nip(vc)+EC30(mu3M)*(DEC0axp2(wc)))-is*bcf(side,axis,i1,i2,i3,vc))
           endif
#End
           endLoopsMask3d()
#endMacro

! *wdh* 091205
#defineMacro extrap3(uu,k1,k2,k3,kc,ks1,ks2,ks3) \
            (3.*uu(k1,k2,k3,kc)-3.*uu(k1+ks1,k2+ks2,k3+ks3,kc)\
            +   uu(k1+2*ks1,k2+2*ks2,k3+2*ks3,kc))


      subroutine bcOptSmCons( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
           gridIndexRange, u, mask,rsxy, xy, ndMatProp,matIndex,matValpc,matVal, boundaryCondition, \
           addBoundaryForcing, interfaceType, dim, bcf00,bcf10,bcf01,bcf11,bcf02,bcf12,\
           bcf0,bcOffset,ipar, rpar, ierr )
!     ===================================================================================
!     Boundary conditions for solid mechanics
!     
!     gridType : 0=rectangular, 1=curvilinear
!     
!     c2= mu/rho, c1=(mu+lambda)/rho;
!     
!     The forcing for the boundary conditions can be accessed in two ways. One can either
!     use the arrays:
!     bcf00(i1,i2,i3,m), bcf10(i1,i2,i3,m), bcf01(i1,i2,i3,m), bcf11(i1,i2,i3,m),
!     bcf02(i1,i2,i3,m), bcf12(i1,i2,i3,m)
!     which provide values for the 6 different faces in 6 different arrays. One can also
!     access the same values using the single statement function
!     bcf(side,axis,i1,i2,i3,m)
!     which is defined below.

!     ===================================================================================

      implicit none

      integer nd, \
      nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, \
      ierr
      
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      integer gridIndexRange(0:1,0:2),boundaryCondition(0:1,0:2)

      integer addBoundaryForcing(0:1,0:2)
      integer interfaceType(0:1,0:2,0:*)
      integer dim(0:1,0:2,0:1,0:2)

      real bcf00(dim(0,0,0,0):dim(1,0,0,0), dim(0,1,0,0):dim(1,1,0,0), dim(0,2,0,0):dim(1,2,0,0),0:*)
      real bcf10(dim(0,0,1,0):dim(1,0,1,0), dim(0,1,1,0):dim(1,1,1,0), dim(0,2,1,0):dim(1,2,1,0),0:*)
      real bcf01(dim(0,0,0,1):dim(1,0,0,1), dim(0,1,0,1):dim(1,1,0,1), dim(0,2,0,1):dim(1,2,0,1),0:*)
      real bcf11(dim(0,0,1,1):dim(1,0,1,1), dim(0,1,1,1):dim(1,1,1,1), dim(0,2,1,1):dim(1,2,1,1),0:*)
      real bcf02(dim(0,0,0,2):dim(1,0,0,2), dim(0,1,0,2):dim(1,1,0,2), dim(0,2,0,2):dim(1,2,0,2),0:*)
      real bcf12(dim(0,0,1,2):dim(1,0,1,2), dim(0,1,1,2):dim(1,1,1,2), dim(0,2,1,2):dim(1,2,1,2),0:*)

      real bcf0(0:*)
      integer*8 bcOffset(0:1,0:2)

      integer ipar(0:*)
      real rpar(0:*)

      ! -- Declare arrays for variable material properties --
      include 'declareVarMatProp.h'

      !     --- local variables ----

      integer side,axis,grid,gridType,orderOfAccuracy,orderOfExtrapolation,twilightZone,\
      uc,vc,wc,useWhereMask,debug,nn,n1,n2
      real dx(0:2),dr(0:2),dri(0:3),dxi(0:3)
      real t,ep,dt,c1,c2,rho,mu,lambda,alpha
      integer axisp1,axisp2,i1,i2,i3,is1,is2,is3,j1,j2,j3,js1,js2,js3,ks1,ks2,ks3,is,js,it,nit
      integer iep1,iep2,iep11,iep12,iep21,iep22
      integer option,initialized,itc

      integer numGhost,numberOfGhostPoints
      integer side1,side2,idxs,i
      integer n1a,n1b,n2a,n2b,n3a,n3b
      integer n1ae,n1be,n2ae,n2be,n3ae,n3be
      integer nn1a,nn1b,nn2a,nn2b,nn3a,nn3b
      integer extra1a,extra1b,extra2a,extra2b,extra3a,extra3b
      
      real a11,a12,a21,a22,det,b0,b1,b2,FSBCtmp

      real a0,a1,cc0,cc1,d0,d1,dr0,ds0
      real aNormSq,divu,uAve,Forceu,Forcev,jac,qx,qy,rx,ry

      real epsRatio,an1,an2,an3,aNorm,ua,ub,nDotU
      real epsx

      real tau11,tau12,tau13,tau21,tau22,tau23,tau31,tau32,tau33
      real ux,uy,uz,vx,vy,vz,wx,wy,wz
      real ux0,uy0,uz0,vx0,vy0,vz0,wx0,wy0,wz0
      real uxx0,uxy0,uxz0,uyy0,uyz0,uzz0
      real vxx0,vxy0,vxz0,vyy0,vyz0,vzz0
      real wxx0,wxy0,wxz0,wyy0,wyz0,wzz0
      real u0,v0,w0
      real um,up,vm,vp,wm,wp
      real lam2muM,lamM,muM,lam3M,lam2mu3M,mu3M,jac3,lambdaPlus2mu
      real tau1,tau2,tau3,clap1,clap2,ulap1,vlap1,wlap1,ulap2,vlap2,wlap2,an1Cartesian,an2Cartesian

      integer numberOfEquations,job
      real a2(0:1,0:1),a3(0:2,0:2),a4(0:3,0:3),a8(0:7,0:7),q(0:11),f(0:11),ipvt(0:11),rcond,work(0:11)

      ! *wdh* 091205 
      real u1,v1,beta
      integer assignTwilightZone

      real err

!     define boundary conditions:
#Include "bcDefineFortranInclude.h"

      integer rectangular,curvilinear
      parameter(\
      rectangular=0,\
      curvilinear=1)
      
      
      !     --- start statement function ----
      real rhopc,mupc,lambdapc,lam2mupc, rhov,muv,lambdav,lam2muv

      real bcf
      integer kd,m,n
      real uxOneSided
      declareDifferenceNewOrder2(u,rsxy,dr,dx,RX)
      declareDifferenceNewOrder4(u,rsxy,dr,dx,RX)

      !     The next macro call will define the difference approximation statement functions
      defineDifferenceNewOrder2Components1(u,rsxy,dr,dx,RX)
      defineDifferenceNewOrder4Components1(u,rsxy,dr,dx,RX)
      
      !     Here is the the generic boundary condition forcing array. It uses the bcOffset(side,axis) values as an
      !     an offset from the bcf0 array to access the bcf10, bcf01, bcf11, ... arrays
      bcf(side,axis,i1,i2,i3,m) = bcf0(bcOffset(side,axis) + \
      (i1-dim(0,0,side,axis)+(dim(1,0,side,axis)-dim(0,0,side,axis)+1)* \
      (i2-dim(0,1,side,axis)+(dim(1,1,side,axis)-dim(0,1,side,axis)+1)* \
      (i3-dim(0,2,side,axis)+(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(m)))))

      qx(i1,i2)=rsxy(i1,i2,nd3a,0,0)
      qy(i1,i2)=rsxy(i1,i2,nd3a,0,1)
      rx(i1,i2)=rsxy(i1,i2,nd3a,1,0)
      ry(i1,i2)=rsxy(i1,i2,nd3a,1,1)
      Jac(i1,i2)=1.d0/(qx(i1,i2)*ry(i1,i2)-rx(i1,i2)*qy(i1,i2))
      Jac3(i1,i2,i3)=(1.d0/(rsxy(i1,i2,i3,0,0)*rsxy(i1,i2,i3,1,1)*rsxy(i1,i2,i3,2,2)+rsxy(i1,i2,i3,0,1)*rsxy(i1,i2,i3,1,2)*rsxy(i1,i2,i3,2,0)+rsxy(i1,i2,i3,0,2)*rsxy(i1,i2,i3,1,0)*rsxy(i1,i2,i3,2,1)-rsxy(i1,i2,i3,2,0)*rsxy(i1,i2,i3,1,1)*rsxy(i1,i2,i3,0,2)-rsxy(i1,i2,i3,2,1)*rsxy(i1,i2,i3,1,2)*rsxy(i1,i2,i3,0,0)-rsxy(i1,i2,i3,2,2)*rsxy(i1,i2,i3,1,0)*rsxy(i1,i2,i3,0,1)))

      ! (rho,mu,lambda) for materialFormat=piecewiseConstantMaterialProperties
      rhopc(i1,i2)    = matValpc( 0, matIndex(i1,i2))
      mupc(i1,i2)     = matValpc( 1, matIndex(i1,i2))
      lambdapc(i1,i2) = matValpc( 2, matIndex(i1,i2))
      lam2mupc(i1,i2) = (matValpc( 2, matIndex(i1,i2))+2.0*matValpc( 1, matIndex(i1,i2)))

      ! (rho,mu,lambda) for materialFormat=variableMaterialProperties
      rhov(i1,i2)    = matVal(i1,i2,0)
      muv(i1,i2)     = matVal(i1,i2,1)
      lambdav(i1,i2) = matVal(i1,i2,2)
      lam2muv(i1,i2) = (matVal(i1,i2,2)+2.0*matVal(i1,i2,1))

      ! *wdh* These next are not correct for rho different from 1
!!$      lamM(i1,i2)=(c1-c2)
!!$      muM(i1,i2)=c2
!!$      lam2muM(i1,i2)=(lamM(i1,i2)+2.0*muM(i1,12))
!!$      lam3M(i1,i2,i3)=(c1-c2)
!!$      mu3M(i1,i2,i3)=c2
!!$      lam2mu3M(i1,i2,i3)=(lam3M(i1,i2,i3)+2.0*mu3M(i1,12,i3))
!!$      lamM(i1,i2)=(c1-c2)

      lamM(i1,i2)=lambda
      muM(i1,i2)=mu
      lam2muM(i1,i2)=lambdaPlus2mu
      lam3M(i1,i2,i3)=lambda
      mu3M(i1,i2,i3)=mu
      lam2mu3M(i1,i2,i3)=lambdaPlus2mu
      !...............end statement functions

      ierr=0

      nd                   =ipar(0)
      grid                 =ipar(1)
      uc                   =ipar(2)
      vc                   =ipar(3)
      wc                   =ipar(4)
      gridType             =ipar(5)
      orderOfAccuracy      =ipar(6)
      orderOfExtrapolation =ipar(7)
      twilightZone         =ipar(8)
      useWhereMask         =ipar(9)
      debug                =ipar(10)

      materialFormat       =ipar(15)

      dx(0)                =rpar(0)
      dx(1)                =rpar(1)
      dx(2)                =rpar(2)
      dr(0)                =rpar(3)
      dr(1)                =rpar(4)
      dr(2)                =rpar(5)
      t                    =rpar(6)
      ep                   =rpar(7) ! pointer for exact solution
      dt                   =rpar(8)
      rho                  =rpar(9)
      mu                   =rpar(10)
      lambda               =rpar(11)
      c1                   =rpar(12)
      c2                   =rpar(13)

      job=0                ! *wdh* 090101

      lambdaPlus2mu=lambda+2.*mu

      ! *wdh* This needs to be fixed for intefaces: -- see bcOptSm.bf 
      assignTwilightZone=twilightZone
      
      !     write(*,'(" bcOptSmCons t=",e10.2)') t
      
      if( debug.gt.3 )then
         write(*,'(" bcOptSm: rho,mu,lambda=",3e12.2," gridType=",i2)') rho,mu,lambda,gridType
         !     '
      end if

      if( debug.gt.7 )then
         write(*,'(" bcOptSm: **START** grid=",i4," uc,vc,wc=",3i2)') grid,uc,vc,wc
         !     '
      end if
      if( debug.gt.7 )then
         write(*,*) 'u=',((((u(i1,i2,i3,m),m=0,2),i1=n1a,n1b),i2=n2a,n2b),i3=n3a,n3b)
      end if


      ! --- Output rho, mu and lambda at t=0 for testing ---
      if( materialFormat.ne.0 .and. t.le.0 .and. (nd1b-nd1a)*(nd2b-nd2a).lt. 1000 )then

       write(*,'("bcOptSmCons: rho:")')
       do i2=nd2b,nd2a,-1
         if( materialFormat.eq.piecewiseConstantMaterialProperties )then
          write(*,'(100(f5.1))') (rhopc(i1,i2),i1=nd1a,nd1b)
         else
          write(*,'(100(f5.1))') (rhov(i1,i2),i1=nd1a,nd1b)
         end if
       end do 
       write(*,'("bcOptSmCons: mu:")')
       do i2=nd2b,nd2a,-1
         if( materialFormat.eq.piecewiseConstantMaterialProperties )then
          write(*,'(100(f5.1))') (mupc(i1,i2),i1=nd1a,nd1b)
         else
          write(*,'(100(f5.1))') (muv(i1,i2),i1=nd1a,nd1b)
         end if
       end do 
       write(*,'("bcOptSmCons: lambda:")')
       do i2=nd2b,nd2a,-1
         if( materialFormat.eq.piecewiseConstantMaterialProperties )then
          write(*,'(100(f5.1))') (lambdapc(i1,i2),i1=nd1a,nd1b)
         else
          write(*,'(100(f5.1))') (lambdav(i1,i2),i1=nd1a,nd1b)
         end if
       end do 

      end if

      if( materialFormat.ne.constantMaterialProperties .and. \
          (boundaryCondition(0,0).eq.tractionBC .or. \
           boundaryCondition(1,0).eq.tractionBC .or. \
           boundaryCondition(0,1).eq.tractionBC .or. \
           boundaryCondition(1,1).eq.tractionBC .or. \
           boundaryCondition(0,2).eq.tractionBC .or. \
           boundaryCondition(1,2).eq.tractionBC )     )then
        write(*,'(" ***bcOptSmCons:ERROR: Finish me for variable material and traction BCs")')
        write(*,'(" ***bcOptSmCons: t=",e10.2)') t
        stop 5529
      end if


      epsx=1.e-20          ! fix this
      numGhost=orderOfAccuracy/2
      do i=0,2
         dri(i)=1.0d0/dr(i)
         dxi(i)=1.0d0/dx(i)
      enddo

      if( nd.eq.2 )then
        ! ***********************************
        ! **************** 2D ***************
        ! ***********************************

        ! -----------------------------------
        ! -----------2nd Order---------------
        ! -----------------------------------
        ! *wdh* 091205 -- add slip wall from SOS-NC
        if( orderOfAccuracy.eq.2 .and. gridType.eq.rectangular )then
  
          beginLoopOverSides(numGhost,numGhost)
           if( boundaryCondition(side,axis).eq.displacementBC )then
           else if( boundaryCondition(side,axis).eq.tractionBC )then 
           else if( boundaryCondition(side,axis).eq.slipWall )then

             ! set n.u = given on the boundary 
             an1=0.  ! (an1,an2) = outward normal 
             an2=0.
             if( axis.eq.0 )then
               an1=2*side-1
             else
               an2=2*side-1
             end if

             if( addBoundaryForcing(side,axis).eq.0 )then
               ! no forcing 
               beginLoops2d()
                 u1 = u(i1,i2,i3,uc)
                 v1 = u(i1,i2,i3,vc)
                 nDotU = an1*u1 + an2*v1  
                 u(i1,i2,i3,uc)=u1 - nDotU*an1
                 u(i1,i2,i3,vc)=v1 - nDotU*an2
               endLoops2d()

             else if( assignTwilightZone.eq.0 )then
              ! include forcing terms 
              ! n.u = n.g 
               beginLoops2d()
                 u1 = u(i1,i2,i3,uc)
                 v1 = u(i1,i2,i3,vc)
                 nDotU = an1*(u1-bcf(side,axis,i1,i2,i3,uc)) + an2*(v1-bcf(side,axis,i1,i2,i3,vc))
                 u(i1,i2,i3,uc)=u1 - nDotU*an1
                 u(i1,i2,i3,vc)=v1 - nDotU*an2
               endLoops2d()

             else
               ! Twilight-zone: 
               !   n.u = n.ue
               beginLoops2d()
                 OGF2D(i1,i2,i3,t,u0,v0)
                 u1 = u(i1,i2,i3,uc)
                 v1 = u(i1,i2,i3,vc)
                 nDotU = an1*(u1-u0) + an2*(v1-v0)
                 u(i1,i2,i3,uc)=u1 - nDotU*an1
                 u(i1,i2,i3,vc)=v1 - nDotU*an2
               endLoops2d()
             end if

             ! extrap values to the ghost line 
             beginGhostLoops2d()
               u(i1-is1,i2-is2,i3,uc)=extrap3(u,i1,i2,i3,uc,is1,is2,is3)
               u(i1-is1,i2-is2,i3,vc)=extrap3(u,i1,i2,i3,vc,is1,is2,is3)
              endLoops2d()

            else if( boundaryCondition(side,axis).eq.dirichletBoundaryCondition.or.\
                     boundaryCondition(side,axis).eq.symmetry  )then
              ! do nothing here
            else if( boundaryCondition(side,axis).gt.0 )then

             write(*,'("bcOptSmCons: un-implemented bc=",i2)') boundaryCondition(side,axis)
             stop 9393

            end if

          endLoopOverSides()

        else
          beginLoopOverSides(numGhost,numGhost)
            if( boundaryCondition(side,axis).eq.slipWall )then
    
              ! finish me 
              write(*,'("bcOptSmCons: slipWall : finish me ")') 
              stop 8282
            end if
          endLoopOverSides()
        end if


        !     DEAA: changed into energy conserving bc
        !     

        !     ****** now apply BC's that assign the ghost values *********

        if( orderOfAccuracy.eq.2 .and. gridType.eq.rectangular )then
          beginLoopOverSides(numGhost,numGhost)
            if( boundaryCondition(side,axis).eq.tractionBC )then
              if( axis.eq.0 )then
                !     (2mu+lam) u.x + lam*v.y =fu
                !     mu(v.x +u.y) = fv
                if( twilightZone.eq.0 )then
                  beginLoopsMask2d()
                    if (addBoundaryForcing(side,axis).eq.0) then
                      !     No forcing
                      Forceu=0
                      Forcev=0
                    else
                      Forceu=(2.*side-1.)*bcf(side,axis,i1,i2,i3,uc)
                      Forcev=(2.*side-1.)*bcf(side,axis,i1,i2,i3,vc)
                    end if
                    vy=uy22r(i1,i2,i3,vc)
                    uy=uy22r(i1,i2,i3,uc)
                    DoXStressFree()
                  endLoopsMask2d()
                  ! Correct the corners if adjacent sides have stress free bc
                  ! We use D+ or D- for the tangential derivative to get energy est.
                  do idxs=0,1
                    if( boundaryCondition(idxs,1).eq.tractionBC )then
                      i2=n2a*(1-idxs)+n2b*idxs
                      i1=n1a
                      CornerTangentDer2d(uy,uc,1,(1-2*idxs))
                      CornerTangentDer2d(vy,vc,1,(1-2*idxs))
                      DoXStressFree()
                    end if
                  end do

                else
                  !     Twilight
                  !     (2mu+lam) u.x + lam*v.y = (2mu+lam) ue.x + lam*ve.y
                  !     mu(v.x +u.y) = mu(ve.x +ue.y)
                  beginLoopsMask2d()
                    OGDERIV2D(0,1,0,0,i1,i2,i3,t,ux0,vx0)
                    OGDERIV2D(0,0,1,0,i1,i2,i3,t,uy0,vy0)
                    Forceu=(lam2muM(i1,i2)*ux0+lamM(i1,i2)*vy0)
                    Forcev=(muM(i1,i2)*uy0+muM(i1,i2)*vx0)
                    vy=uy22r(i1,i2,i3,vc)
                    uy=uy22r(i1,i2,i3,uc)
                    DoXStressFree()
                  endLoopsMask2d()
                  ! Correct the corners if adjacent sides have stress free bc
                  ! We use D+ or D- for the tangential derivative to get energy est.
                  do idxs=0,1
                    if( boundaryCondition(idxs,1).eq.tractionBC )then
                      i2=n2a*(1-idxs)+n2b*idxs
                      i1=n1a
                      OGDERIV2D(0,1,0,0,i1,i2,i3,t,ux0,vx0)
                      OGDERIV2D(0,0,1,0,i1,i2,i3,t,uy0,vy0)
                      Forceu=(lam2muM(i1,i2)*ux0+lamM(i1,i2)*vy0)
                      Forcev=(muM(i1,i2)*uy0+muM(i1,i2)*vx0)
                      CornerTangentDer2d(uy,uc,1,(1-2*idxs))
                      CornerTangentDer2d(vy,vc,1,(1-2*idxs))
                      DoXStressFree()
                      !  OGDERIV2D(0,0,0,0,i1-is1,i2-is2,i3,t,ux0,vx0)
                      !  u(i1-is1,i2-is2,i3,uc)=ux0 
                      !  u(i1-is1,i2-is2,i3,vc)=vx0 
                    end if
                  end do
                end if           ! end if forcing

              else ! axis==1
                !     (2mu+lam) v.x + lam*u.y =fv
                !     mu(u.x +v.y) = fu
                if( twilightZone.eq.0 )then
                  beginLoopsMask2d()
                    if (addBoundaryForcing(side,axis).eq.0) then
                      !     No forcing
                      Forceu=0
                      Forcev=0
                    else
                      Forceu=(2.*side-1.)*bcf(side,axis,i1,i2,i3,uc)
                      Forcev=(2.*side-1.)*bcf(side,axis,i1,i2,i3,vc)
                    end if
                    ux=ux22r(i1,i2,i3,uc)
                    vx=ux22r(i1,i2,i3,vc)
                    DoYStressFree()
                  endLoopsMask2d()
                  ! Correct the corners if adjacent sides have stress free bc
                  ! We use D+ or D- for the tangential derivative to get energy est.
                  do idxs=0,1
                    if( boundaryCondition(idxs,0).eq.tractionBC )then
                      i2=n2a
                      i1=n1a*(1-idxs)+n1b*idxs
                      CornerTangentDer2d(ux,uc,0,(1-2*idxs))
                      CornerTangentDer2d(vx,vc,0,(1-2*idxs))
                      DoYStressFree()
                    end if
                  end do
                else
                  !     Twilight
                  beginLoopsMask2d()
                    OGDERIV2D(0,1,0,0,i1,i2,i3,t,ux0,vx0)
                    OGDERIV2D(0,0,1,0,i1,i2,i3,t,uy0,vy0)
                    Forcev=lam2muM(i1,i2)*vy0 +lamM(i1,i2)*ux0
                    Forceu=muM(i1,i2)*vx0 +muM(i1,i2)*uy0
                    ux=ux22r(i1,i2,i3,uc)
                    vx=ux22r(i1,i2,i3,vc)
                    DoYStressFree()
                  endLoopsMask2d()
                  ! Correct the corners if adjacent sides have stress free bc
                  ! We use D+ or D- for the tangential derivative to get energy est.
                  do idxs=0,1
                    if( boundaryCondition(idxs,0).eq.tractionBC )then
                      i2=n2a
                      i1=n1a*(1-idxs)+n1b*idxs
                      OGDERIV2D(0,1,0,0,i1,i2,i3,t,ux0,vx0)
                      OGDERIV2D(0,0,1,0,i1,i2,i3,t,uy0,vy0)
                      Forcev=lam2muM(i1,i2)*vy0 +lamM(i1,i2)*ux0
                      Forceu=muM(i1,i2)*vx0 +muM(i1,i2)*uy0
                      CornerTangentDer2d(ux,uc,0,(1-2*idxs))
                      CornerTangentDer2d(vx,vc,0,(1-2*idxs))
                      DoYStressFree()
                      !  OGDERIV2D(0,0,0,0,i1-is1,i2-is2,i3,t,ux0,vx0)
                      !  u(i1-is1,i2-is2,i3,uc)=ux0 
                      !  u(i1-is1,i2-is2,i3,vc)=vx0 
                    end if
                  end do
                end if           ! end if forcing
              end if            ! end if axis

            ! *wdh* 091205 -- add slip wall from SOS-NC 
            else if( boundaryCondition(side,axis).eq.slipWall )then 
             
              alpha=lambda/(lambda+2.*mu)
              beta =1./(lambda+2.*mu)
              if( axis.eq.0 )then
                ! v.x = -u.y       
                if( addBoundaryForcing(side,axis).eq.0 )then
                  ! no forcing 
                  beginLoopsMask2d()
                    u(i1-is1,i2-is2,i3,vc)=u(i1+is1,i2+is2,i3,vc)+is1*dx(0)*2.*      uy22r(i1,i2,i3,uc)
                  endLoopsMask2d()
  
                else if( assignTwilightZone.eq.0 )then
                  ! include forcing terms 
                  beginLoopsMask2d()
                    u(i1-is1,i2-is2,i3,vc)=u(i1+is1,i2+is2,i3,vc)+dx(0)*2.*(\
                                   is1*uy22r(i1,i2,i3,uc)    + (1./mu)*bcf(side,axis,i1,i2,i3,vc) )
                  endLoopsMask2d()
  
                else 
                  ! Twilight-zone: 
                  ! u.x = -alpha*v.y + ue.x -alpha*ve.y 
                  beginLoopsMask2d()
                    OGDERIV2D(0,1,0,0,i1,i2,i3,t,ux0,vx0)
                    OGDERIV2D(0,0,1,0,i1,i2,i3,t,uy0,vy0)
                    u(i1-is1,i2-is2,i3,vc)=u(i1+is1,i2+is2,i3,vc)-\
                            is1*dx(0)*2.*(-uy22r(i1,i2,i3,uc) + vx0 + uy0 )
                    !     write(*,'("i1,i2=",2i3," ux0,vx0,uy0,vy0=",4e10.2)') i1,i2, ux0,vx0,uy0,vy0                
                  endLoopsMask2d()
                end if
  
              else if( axis.eq.1 )then
                ! u.y = - v.x
                if( addBoundaryForcing(side,axis).eq.0 )then
                  beginLoopsMask2d()
                    u(i1-is1,i2-is2,i3,uc)=u(i1+is1,i2+is2,i3,uc)+is2*dx(1)*2.*      ux22r(i1,i2,i3,vc)
                  endLoopsMask2d()
  
                else if( assignTwilightZone.eq.0 )then
                  ! include forcing terms
                  beginLoopsMask2d()
                    u(i1-is1,i2-is2,i3,uc)=u(i1+is1,i2+is2,i3,uc)+dx(1)*2.*(\
                                     is2*ux22r(i1,i2,i3,vc)    + (1./mu)*bcf(side,axis,i1,i2,i3,uc) )
                  endLoopsMask2d()
  
                else
                  ! Twilight-zone: 
                  beginLoopsMask2d()
                    OGDERIV2D(0,1,0,0,i1,i2,i3,t,ux0,vx0)
                    OGDERIV2D(0,0,1,0,i1,i2,i3,t,uy0,vy0)
                    u(i1-is1,i2-is2,i3,uc)=u(i1+is1,i2+is2,i3,uc)-is2*dx(1)*2.*(      -ux22r(i1,i2,i3,vc)+uy0+vx0)
                  endLoopsMask2d()
                end if
              end if      

            end if             ! end if bc 
          endLoopOverSides()


        else if( orderOfAccuracy.eq.2 .and. gridType.eq.curvilinear )then
          !     *********************************************
          !     ************* 2d Curvilinear ****************
          !     *********************************************
          !     DEAA: changed into energy conserving bc 071118
          !     ** now apply BC's that assign the ghost values *********
          beginLoopOverSides(numGhost,numGhost)
            if( boundaryCondition(side,axis).eq.tractionBC )then
              if( twilightZone.eq.0 )then
                tractionBCCurvilinear2dCons(noForcing)
                CorrectCornersCurvilinear2dCons(noForcing)
              else
                tractionBCCurvilinear2dCons(forcing)
                CorrectCornersCurvilinear2dCons(forcing)
              end if
            end if
          endLoopOverSides()
        else
          !     un-known nd and orderOfAccuracy
          stop 6663
        end if

      else if( nd.eq.3 )then
        !     *************************
        !     ********** 3D ***********
        !     *************************

        !     Rectangular
        if( orderOfAccuracy.eq.2 .and. gridType.eq.rectangular )then
          !     Then do stress BC
          beginLoopOverSides(numGhost,numGhost)
            if(boundaryCondition(side,axis).eq.tractionBC)then
              if( twilightZone.eq.0 )then
                tractionBCRectangular3dMacro(noForcing)
              else
                tractionBCRectangular3dMacro(forcing)
              end if
            end if
          endLoopOverSides()
          !     Correct edges
          beginLoopOverSides(numGhost,numGhost)
            if ((axis.eq.0).and.(boundaryCondition(side,0).eq.tractionBC).and.(boundaryCondition(0,1).eq.tractionBC)) then
              n1ae=n1a
              n1be=n1b
              n2ae=n2a
              n2be=n2a
              n3ae=n3a
              n3be=n3b
              iep11=1-side
              iep12=side
              iep21=1
              iep22=1
              if( twilightZone.eq.0 )then
                tractionBCRectangular3dMacroX(noForcing)
              else
                tractionBCRectangular3dMacroX(forcing)
              end if
            end if
            if ((axis.eq.0).and.(boundaryCondition(side,0).eq.tractionBC).and.(boundaryCondition(1,1).eq.tractionBC)) then
              n1ae=n1a
              n1be=n1b
              n2ae=n2b
              n2be=n2b
              n3ae=n3a
              n3be=n3b
              iep11=side
              iep12=1-side
              iep21=1
              iep22=1
              if( twilightZone.eq.0 )then
                tractionBCRectangular3dMacroX(noForcing)
              else
                tractionBCRectangular3dMacroX(forcing)
              end if
            end if
            if ((axis.eq.0).and.(boundaryCondition(side,0).eq.tractionBC).and.(boundaryCondition(0,2).eq.tractionBC)) then
              n1ae=n1a
              n1be=n1b
              n2ae=n2a
              n2be=n2b
              n3ae=n3a
              n3be=n3a
              iep11=1
              iep12=1
              iep21=1-side
              iep22=side
              if( twilightZone.eq.0 )then
                tractionBCRectangular3dMacroX(noForcing)
              else
                tractionBCRectangular3dMacroX(forcing)
              end if
            end if
            if ((axis.eq.0).and.(boundaryCondition(side,0).eq.tractionBC).and.(boundaryCondition(1,2).eq.tractionBC)) then
              n1ae=n1a
              n1be=n1b
              n2ae=n2a
              n2be=n2b
              n3ae=n3b
              n3be=n3b
              iep11=1
              iep12=1
              iep21=side
              iep22=1-side
              if( twilightZone.eq.0 )then
                tractionBCRectangular3dMacroX(noForcing)
              else
                tractionBCRectangular3dMacroX(forcing)
              end if
            end if

            !     Axis 1
            if ((axis.eq.1).and.(boundaryCondition(side,1).eq.tractionBC).and.(boundaryCondition(0,2).eq.tractionBC)) then
              n1ae=n1a
              n1be=n1b
              n2ae=n2a
              n2be=n2b
              n3ae=n3a
              n3be=n3a
              iep11=1-side
              iep12=side
              iep21=1
              iep22=1
              if( twilightZone.eq.0 )then
                tractionBCRectangular3dMacroY(noForcing)
              else
                tractionBCRectangular3dMacroY(forcing)
              end if
            end if
            if ((axis.eq.1).and.(boundaryCondition(side,1).eq.tractionBC).and.(boundaryCondition(1,2).eq.tractionBC)) then
              n1ae=n1a
              n1be=n1b
              n2ae=n2a
              n2be=n2b
              n3ae=n3b
              n3be=n3b
              iep11=side
              iep12=1-side
              iep21=1
              iep22=1
              if( twilightZone.eq.0 )then
                tractionBCRectangular3dMacroY(noForcing)
              else
                tractionBCRectangular3dMacroY(forcing)
              end if
            end if
            if ((axis.eq.1).and.(boundaryCondition(side,1).eq.tractionBC).and.(boundaryCondition(0,0).eq.tractionBC)) then
              n1ae=n1a
              n1be=n1a
              n2ae=n2a
              n2be=n2b
              n3ae=n3a
              n3be=n3b
              iep11=1
              iep12=1
              iep21=1-side
              iep22=side
              if( twilightZone.eq.0 )then
                tractionBCRectangular3dMacroY(noForcing)
              else
                tractionBCRectangular3dMacroY(forcing)
              end if
            end if
            if ((axis.eq.1).and.(boundaryCondition(side,1).eq.tractionBC).and.(boundaryCondition(1,0).eq.tractionBC)) then
              n1ae=n1b
              n1be=n1b
              n2ae=n2a
              n2be=n2b
              n3ae=n3a
              n3be=n3b
              iep11=1
              iep12=1
              iep21=side
              iep22=1-side
              if( twilightZone.eq.0 )then
                tractionBCRectangular3dMacroY(noForcing)
              else
                tractionBCRectangular3dMacroY(forcing)
              end if
            end if
            !     Axis 2
            if ((axis.eq.2).and.(boundaryCondition(side,2).eq.tractionBC).and.(boundaryCondition(0,0).eq.tractionBC)) then
              n1ae=n1a
              n1be=n1a
              n2ae=n2a
              n2be=n2b
              n3ae=n3a
              n3be=n3b
              iep11=1-side
              iep12=side
              iep21=1
              iep22=1
              if( twilightZone.eq.0 )then
                tractionBCRectangular3dMacroZ(noForcing)
              else
                tractionBCRectangular3dMacroZ(forcing)
              end if
            end if
            if ((axis.eq.2).and.(boundaryCondition(side,2).eq.tractionBC).and.(boundaryCondition(1,0).eq.tractionBC)) then
              n1ae=n1b
              n1be=n1b
              n2ae=n2a
              n2be=n2b
              n3ae=n3a
              n3be=n3b
              iep11=side
              iep12=1-side
              iep21=1
              iep22=1
              if( twilightZone.eq.0 )then
                tractionBCRectangular3dMacroZ(noForcing)
              else
                tractionBCRectangular3dMacroZ(forcing)
              end if
            end if
            if ((axis.eq.2).and.(boundaryCondition(side,2).eq.tractionBC).and.(boundaryCondition(0,1).eq.tractionBC)) then
              n1ae=n1a
              n1be=n1b
              n2ae=n2a
              n2be=n2a
              n3ae=n3a
              n3be=n3b
              iep11=1
              iep12=1
              iep21=1-side
              iep22=side
              if( twilightZone.eq.0 )then
                tractionBCRectangular3dMacroZ(noForcing)
              else
                tractionBCRectangular3dMacroZ(forcing)
              end if
            end if
            if ((axis.eq.2).and.(boundaryCondition(side,2).eq.tractionBC).and.(boundaryCondition(1,1).eq.tractionBC)) then
              n1ae=n1a
              n1be=n1b
              n2ae=n2b
              n2be=n2b
              n3ae=n3a
              n3be=n3b
              iep11=1
              iep12=1
              iep21=side
              iep22=1-side
              if( twilightZone.eq.0 )then
                tractionBCRectangular3dMacroZ(noForcing)
              else
                tractionBCRectangular3dMacroZ(forcing)
              end if
            end if
          endLoopOverSides()
          !     Correct corners
          beginLoopOverSides(numGhost,numGhost)
            if ((axis.eq.0).and.(boundaryCondition(side,0).eq.tractionBC).and.(boundaryCondition(0,1).eq.tractionBC).and.(boundaryCondition(0,2).eq.tractionBC)) then
              n1ae=n1a
              n1be=n1b
              n2ae=n2a
              n2be=n2a
              n3ae=n3a
              n3be=n3a
              iep11=1-side
              iep12=side
              iep21=1-side
              iep22=side
              if( twilightZone.eq.0 )then
                tractionBCRectangular3dMacroX(noForcing)
              else
                tractionBCRectangular3dMacroX(forcing)
              end if
            end if
            if ((axis.eq.0).and.(boundaryCondition(side,0).eq.tractionBC).and.(boundaryCondition(0,1).eq.tractionBC).and.(boundaryCondition(1,2).eq.tractionBC)) then
              n1ae=n1a
              n1be=n1b
              n2ae=n2a
              n2be=n2a
              n3ae=n3b
              n3be=n3b
              iep11=1-side
              iep12=side
              iep21=side
              iep22=1-side
              if( twilightZone.eq.0 )then
                tractionBCRectangular3dMacroX(noForcing)
              else
                tractionBCRectangular3dMacroX(forcing)
              end if
            end if

            if ((axis.eq.0).and.(boundaryCondition(side,0).eq.tractionBC).and.(boundaryCondition(1,1).eq.tractionBC).and.(boundaryCondition(0,2).eq.tractionBC)) then
              n1ae=n1a
              n1be=n1b
              n2ae=n2b
              n2be=n2b
              n3ae=n3a
              n3be=n3a
              iep11=side
              iep12=1-side
              iep21=1-side
              iep22=side
              if( twilightZone.eq.0 )then
                tractionBCRectangular3dMacroX(noForcing)
              else
                tractionBCRectangular3dMacroX(forcing)
              end if
            end if
            if ((axis.eq.0).and.(boundaryCondition(side,0).eq.tractionBC).and.(boundaryCondition(1,1).eq.tractionBC).and.(boundaryCondition(1,2).eq.tractionBC)) then
              n1ae=n1a
              n1be=n1b
              n2ae=n2b
              n2be=n2b
              n3ae=n3b
              n3be=n3b
              iep11=side
              iep12=1-side
              iep21=side
              iep22=1-side
              if( twilightZone.eq.0 )then
                tractionBCRectangular3dMacroX(noForcing)
              else
                tractionBCRectangular3dMacroX(forcing)
              end if
            end if
            !     Axis 1
            if ((axis.eq.1).and.(boundaryCondition(side,1).eq.tractionBC).and.(boundaryCondition(0,2).eq.tractionBC).and.(boundaryCondition(0,0).eq.tractionBC)) then
              n1ae=n1a
              n1be=n1a
              n2ae=n2a
              n2be=n2b
              n3ae=n3a
              n3be=n3a
              iep11=1-side
              iep12=side
              iep21=1-side
              iep22=side
              if( twilightZone.eq.0 )then
                tractionBCRectangular3dMacroY(noForcing)
              else
                tractionBCRectangular3dMacroY(forcing)
              end if
            end if
            if ((axis.eq.1).and.(boundaryCondition(side,1).eq.tractionBC).and.(boundaryCondition(0,2).eq.tractionBC).and.(boundaryCondition(1,0).eq.tractionBC)) then
              n1ae=n1b
              n1be=n1b
              n2ae=n2a
              n2be=n2b
              n3ae=n3a
              n3be=n3a
              iep11=1-side
              iep12=side
              iep21=side
              iep22=1-side
              if( twilightZone.eq.0 )then
                tractionBCRectangular3dMacroY(noForcing)
              else
                tractionBCRectangular3dMacroY(forcing)
              end if
            end if
            if ((axis.eq.1).and.(boundaryCondition(side,1).eq.tractionBC).and.(boundaryCondition(1,2).eq.tractionBC).and.(boundaryCondition(0,0).eq.tractionBC)) then
              n1ae=n1a
              n1be=n1a
              n2ae=n2a
              n2be=n2b
              n3ae=n3b
              n3be=n3b
              iep11=side
              iep12=1-side
              iep21=1-side
              iep22=side
              if( twilightZone.eq.0 )then
                tractionBCRectangular3dMacroY(noForcing)
              else
                tractionBCRectangular3dMacroY(forcing)
              end if
            end if
            if ((axis.eq.1).and.(boundaryCondition(side,1).eq.tractionBC).and.(boundaryCondition(1,2).eq.tractionBC).and.(boundaryCondition(1,0).eq.tractionBC)) then
              n1ae=n1b
              n1be=n1b
              n2ae=n2a
              n2be=n2b
              n3ae=n3b
              n3be=n3b
              iep11=side
              iep12=1-side
              iep21=side
              iep22=1-side
              if( twilightZone.eq.0 )then
                tractionBCRectangular3dMacroY(noForcing)
              else
                tractionBCRectangular3dMacroY(forcing)
              end if
            end if
    
            !     Axis 2
            if ((axis.eq.2).and.(boundaryCondition(side,2).eq.tractionBC).and.(boundaryCondition(0,0).eq.tractionBC).and.(boundaryCondition(0,1).eq.tractionBC)) then
              n1ae=n1a
              n1be=n1a
              n2ae=n2a
              n2be=n2a
              n3ae=n3a
              n3be=n3b
              iep11=1-side
              iep12=side
              iep21=1-side
              iep22=side
              if( twilightZone.eq.0 )then
                tractionBCRectangular3dMacroZ(noForcing)
              else
                tractionBCRectangular3dMacroZ(forcing)
              end if
            end if
            if ((axis.eq.2).and.(boundaryCondition(side,2).eq.tractionBC).and.(boundaryCondition(0,0).eq.tractionBC).and.(boundaryCondition(1,1).eq.tractionBC)) then
              n1ae=n1a
              n1be=n1a
              n2ae=n2b
              n2be=n2b
              n3ae=n3a
              n3be=n3b
              iep11=1-side
              iep12=side
              iep21=side
              iep22=1-side
              if( twilightZone.eq.0 )then
                tractionBCRectangular3dMacroZ(noForcing)
              else
                tractionBCRectangular3dMacroZ(forcing)
              end if
            end if
            if ((axis.eq.2).and.(boundaryCondition(side,2).eq.tractionBC).and.(boundaryCondition(1,0).eq.tractionBC).and.(boundaryCondition(0,1).eq.tractionBC)) then
              n1ae=n1b
              n1be=n1b
              n2ae=n2a
              n2be=n2a
              n3ae=n3a
              n3be=n3b
              iep11=side
              iep12=1-side
              iep21=1-side
              iep22=side
              if( twilightZone.eq.0 )then
                tractionBCRectangular3dMacroZ(noForcing)
              else
                tractionBCRectangular3dMacroZ(forcing)
              end if
            end if
            if ((axis.eq.2).and.(boundaryCondition(side,2).eq.tractionBC).and.(boundaryCondition(1,0).eq.tractionBC).and.(boundaryCondition(1,1).eq.tractionBC)) then
              n1ae=n1b
              n1be=n1b
              n2ae=n2b
              n2be=n2b
              n3ae=n3a
              n3be=n3b
              iep11=side
              iep12=1-side
              iep21=side
              iep22=1-side
              if( twilightZone.eq.0 )then
                tractionBCRectangular3dMacroZ(noForcing)
              else
                tractionBCRectangular3dMacroZ(forcing)
              end if
            end if
          endLoopOverSides()

        else if( orderOfAccuracy.eq.2 .and. gridType.eq.curvilinear )then
          !     *********************************************
          !     ************* 3d Curvilinear ****************
          !     *********************************************

          !     Then do stress BC
          beginLoopOverSides(numGhost,numGhost)
            if(boundaryCondition(side,axis).eq.tractionBC)then
              if( twilightZone.eq.0 )then
                if(addBoundaryForcing(side,axis).eq.0) then
                  beginLoopsMask3d()
                    assignRHside3D(uc,vc,wc,f(0))
                    assignRHside3D(vc,wc,uc,f(1))
                    assignRHside3D(wc,uc,vc,f(2))
                    assignMatrix(uc,vc,wc,a3(0,0),a3(0,1),a3(0,2))
                    assignMatrix(vc,wc,uc,a3(1,1),a3(1,2),a3(1,0))
                    assignMatrix(wc,uc,vc,a3(2,2),a3(2,0),a3(2,1))
                    !     [ a3(0,0) a3(0,1) a3(0,2) ][ Dnu(-1) ] =  RHS
                    !     [ a3(1,0) a3(1,1) a3(1,2) ][ dnv(-1) ]
                    !     [ a3(2,0) a3(2,1) a3(2,2) ][ dnw(-1) ]
                    f(0) = -f(0)
                    f(1) = -f(1)
                    f(2) = -f(2)
                    call dgeco( a3(0,0), 3, 3, ipvt(0),rcond,work(0))
                    call dgesl( a3(0,0), 3, 3, ipvt(0), f(0), job)
                    u(i1-is1,i2-is2,i3-is3,uc)=u(i1,i2,i3,uc)-is*f(0)*dr(axis)
                    u(i1-is1,i2-is2,i3-is3,vc)=u(i1,i2,i3,vc)-is*f(1)*dr(axis)
                    u(i1-is1,i2-is2,i3-is3,wc)=u(i1,i2,i3,wc)-is*f(2)*dr(axis)
                  endLoopsMask3d()
                else             ! We force the boundary
                  beginLoopsMask3d()
                    assignRHside3D(uc,vc,wc,f(0))
                    assignRHside3D(vc,wc,uc,f(1))
                    assignRHside3D(wc,uc,vc,f(2))
                    assignMatrix(uc,vc,wc,a3(0,0),a3(0,1),a3(0,2))
                    assignMatrix(vc,wc,uc,a3(1,1),a3(1,2),a3(1,0))
                    assignMatrix(wc,uc,vc,a3(2,2),a3(2,0),a3(2,1))
                    !     [ a3(0,0) a3(0,1) a3(0,2) ][ Dnu(-1) ] =  RHS
                    !     [ a3(1,0) a3(1,1) a3(1,2) ][ dnv(-1) ]
                    !     [ a3(2,0) a3(2,1) a3(2,2) ][ dnw(-1) ]
                    an1=rsxy(i1,i2,i3,axis,0) ! normal (an1,an2,an3)
                    an2=rsxy(i1,i2,i3,axis,1)
                    an3=rsxy(i1,i2,i3,axis,2)
                    alpha=lam2mu3M(i1,i2,i3)
                    mu=mu3M(i1,i2,i3)
                    lambda=lam3M(i1,i2,i3)
                    !     subtract off forcing
                    f(0) = -f(0) - is*Jac3(i1,i2,i3)*(bcf(side,axis,i1,i2,i3,uc))
                    f(1) = -f(1) - is*Jac3(i1,i2,i3)*(bcf(side,axis,i1,i2,i3,vc))
                    ! *wdh* 091109 -- bcf --
                    f(2) = -f(2) - is*Jac3(i1,i2,i3)*(bcf(side,axis,i1,i2,i3,wc))
                    call dgeco( a3(0,0), 3, 3, ipvt(0),rcond,work(0))
                    call dgesl( a3(0,0), 3, 3, ipvt(0), f(0), job)
                    u(i1-is1,i2-is2,i3-is3,uc)=u(i1,i2,i3,uc)-is*f(0)*dr(axis)
                    u(i1-is1,i2-is2,i3-is3,vc)=u(i1,i2,i3,vc)-is*f(1)*dr(axis)
                    u(i1-is1,i2-is2,i3-is3,wc)=u(i1,i2,i3,wc)-is*f(2)*dr(axis)
                  endLoopsMask3d()
                end if
              else
                beginLoopsMask3d()
                  assignRHside3D(uc,vc,wc,f(0))
                  assignRHside3D(vc,wc,uc,f(1))
                  assignRHside3D(wc,uc,vc,f(2))
                  assignMatrix(uc,vc,wc,a3(0,0),a3(0,1),a3(0,2))
                  assignMatrix(vc,wc,uc,a3(1,1),a3(1,2),a3(1,0))
                  assignMatrix(wc,uc,vc,a3(2,2),a3(2,0),a3(2,1))
                  OGDERIV3D(0,1,0,0,i1,i2,i3,t,ux0,vx0,wx0)
                  OGDERIV3D(0,0,1,0,i1,i2,i3,t,uy0,vy0,wy0)
                  OGDERIV3D(0,0,0,1,i1,i2,i3,t,uz0,vz0,wz0)
                  !     [ a3(0,0) a3(0,1) a3(0,2) ][ Dnu(-1) ] =  RHS
                  !     [ a3(1,0) a3(1,1) a3(1,2) ][ dnv(-1) ]
                  !     [ a3(2,0) a3(2,1) a3(2,2) ][ dnw(-1) ]
                  an1=rsxy(i1,i2,i3,axis,0) ! normal (an1,an2,an3)
                  an2=rsxy(i1,i2,i3,axis,1)
                  an3=rsxy(i1,i2,i3,axis,2)
                  alpha=lam2mu3M(i1,i2,i3)
                  mu=mu3M(i1,i2,i3)
                  lambda=lam3M(i1,i2,i3)
                  !     subtract off twilight
                  f(0) = -f(0) + Jac3(i1,i2,i3)*( an1*(alpha*ux0+lambda*(vy0+wz0))+an2*(mu*(uy0+vx0))+an3*(mu*(uz0+wx0)))
                  f(1) = -f(1) + Jac3(i1,i2,i3)*( an1*(mu*(uy0+vx0))+an2*(lambda*(ux0+wz0)+alpha*vy0)+an3*(mu*(vz0+wy0)))
                  f(2) = -f(2) + Jac3(i1,i2,i3)*( an1*(mu*(uz0+wx0))+an2*(mu*(vz0+wy0))+an3*(lambda*(ux0+vy0)+alpha*wz0))
                  call dgeco( a3(0,0), 3, 3, ipvt(0),rcond,work(0))
                  call dgesl( a3(0,0), 3, 3, ipvt(0), f(0), job)
                  u(i1-is1,i2-is2,i3-is3,uc)=u(i1,i2,i3,uc)-is*f(0)*dr(axis)
                  u(i1-is1,i2-is2,i3-is3,vc)=u(i1,i2,i3,vc)-is*f(1)*dr(axis)
                  u(i1-is1,i2-is2,i3-is3,wc)=u(i1,i2,i3,wc)-is*f(2)*dr(axis)
                endLoopsMask3d()
              end if
            end if
          endLoopOverSides()
          !     Correct edges
          beginLoopOverSides(numGhost,numGhost)
            if ((axis.eq.0).and.(boundaryCondition(side,0).eq.tractionBC).and.(boundaryCondition(0,1).eq.tractionBC)) then
              n1ae=n1a
              n1be=n1b
              n2ae=n2a
              n2be=n2a
              n3ae=n3a
              n3be=n3b
              iep11=1-side
              iep12=side
              iep21=1
              iep22=1
              call bcEdge( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
              gridIndexRange, u, mask,rsxy, xy, boundaryCondition, \
              ipar, rpar, ierr , n1ae, n1be, n2ae, n2be, n3ae, n3be,\
              iep11, iep12, iep21, iep22,axis,axisp1,axisp2,is1,is2,is3,is,\
              bcf00,bcf10,bcf01,bcf11,bcf02,bcf12,bcf0,bcOffset,\
              addBoundaryForcing,interfaceType,dim,side)

!     doEdge()
            end if
            if ((axis.eq.0).and.(boundaryCondition(side,0).eq.tractionBC).and.(boundaryCondition(1,1).eq.tractionBC)) then
              n1ae=n1a
              n1be=n1b
              n2ae=n2b
              n2be=n2b
              n3ae=n3a
              n3be=n3b
              iep11=side
              iep12=1-side
              iep21=1
              iep22=1
              call bcEdge( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
              gridIndexRange, u, mask,rsxy, xy, boundaryCondition, \
              ipar, rpar, ierr , n1ae, n1be, n2ae, n2be, n3ae, n3be,\
              iep11, iep12, iep21, iep22,axis,axisp1,axisp2,is1,is2,is3,is,\
              bcf00,bcf10,bcf01,bcf11,bcf02,bcf12,bcf0,bcOffset,\
              addBoundaryForcing,interfaceType,dim,side)

!     doEdge()
            end if
            if ((axis.eq.0).and.(boundaryCondition(side,0).eq.tractionBC).and.(boundaryCondition(0,2).eq.tractionBC)) then
              n1ae=n1a
              n1be=n1b
              n2ae=n2a
              n2be=n2b
              n3ae=n3a
              n3be=n3a
              iep11=1
              iep12=1
              iep21=1-side
              iep22=side
              call bcEdge( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
              gridIndexRange, u, mask,rsxy, xy, boundaryCondition, \
              ipar, rpar, ierr , n1ae, n1be, n2ae, n2be, n3ae, n3be,\
              iep11, iep12, iep21, iep22,axis,axisp1,axisp2,is1,is2,is3,is,\
              bcf00,bcf10,bcf01,bcf11,bcf02,bcf12,bcf0,bcOffset,\
              addBoundaryForcing,interfaceType,dim,side)

!     doEdge()
            end if
            if ((axis.eq.0).and.(boundaryCondition(side,0).eq.tractionBC).and.(boundaryCondition(1,2).eq.tractionBC)) then
              n1ae=n1a
              n1be=n1b
              n2ae=n2a
              n2be=n2b
              n3ae=n3b
              n3be=n3b
              iep11=1
              iep12=1
              iep21=side
              iep22=1-side
              call bcEdge( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
              gridIndexRange, u, mask,rsxy, xy, boundaryCondition, \
              ipar, rpar, ierr , n1ae, n1be, n2ae, n2be, n3ae, n3be,\
              iep11, iep12, iep21, iep22,axis,axisp1,axisp2,is1,is2,is3,is,\
              bcf00,bcf10,bcf01,bcf11,bcf02,bcf12,bcf0,bcOffset,\
              addBoundaryForcing,interfaceType,dim,side)

!     doEdge()
            end if

!     Axis 1
            if ((axis.eq.1).and.(boundaryCondition(side,1).eq.tractionBC).and.(boundaryCondition(0,2).eq.tractionBC)) then
              n1ae=n1a
              n1be=n1b
              n2ae=n2a
              n2be=n2b
              n3ae=n3a
              n3be=n3a
              iep11=1-side
              iep12=side
              iep21=1
              iep22=1
              call bcEdge( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
              gridIndexRange, u, mask,rsxy, xy, boundaryCondition, \
              ipar, rpar, ierr , n1ae, n1be, n2ae, n2be, n3ae, n3be,\
              iep11, iep12, iep21, iep22,axis,axisp1,axisp2,is1,is2,is3,is,\
              bcf00,bcf10,bcf01,bcf11,bcf02,bcf12,bcf0,bcOffset,\
              addBoundaryForcing,interfaceType,dim,side)

!     doEdge()
             end if
             if ((axis.eq.1).and.(boundaryCondition(side,1).eq.tractionBC).and.(boundaryCondition(1,2).eq.tractionBC)) then
              n1ae=n1a
              n1be=n1b
              n2ae=n2a
              n2be=n2b
              n3ae=n3b
              n3be=n3b
              iep11=side
              iep12=1-side
              iep21=1
              iep22=1
              call bcEdge( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
              gridIndexRange, u, mask,rsxy, xy, boundaryCondition, \
              ipar, rpar, ierr , n1ae, n1be, n2ae, n2be, n3ae, n3be,\
              iep11, iep12, iep21, iep22,axis,axisp1,axisp2,is1,is2,is3,is,\
              bcf00,bcf10,bcf01,bcf11,bcf02,bcf12,bcf0,bcOffset,\
              addBoundaryForcing,interfaceType,dim,side)

!     doEdge()
            end if
            if ((axis.eq.1).and.(boundaryCondition(side,1).eq.tractionBC).and.(boundaryCondition(0,0).eq.tractionBC)) then
              n1ae=n1a
              n1be=n1a
              n2ae=n2a
              n2be=n2b
              n3ae=n3a
              n3be=n3b
              iep11=1
              iep12=1
              iep21=1-side
              iep22=side
              call bcEdge( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
              gridIndexRange, u, mask,rsxy, xy, boundaryCondition, \
              ipar, rpar, ierr , n1ae, n1be, n2ae, n2be, n3ae, n3be,\
              iep11, iep12, iep21, iep22,axis,axisp1,axisp2,is1,is2,is3,is,\
              bcf00,bcf10,bcf01,bcf11,bcf02,bcf12,bcf0,bcOffset,\
              addBoundaryForcing,interfaceType,dim,side)

!     doEdge()
            end if
            if ((axis.eq.1).and.(boundaryCondition(side,1).eq.tractionBC).and.(boundaryCondition(1,0).eq.tractionBC)) then
              n1ae=n1b
              n1be=n1b
              n2ae=n2a
              n2be=n2b
              n3ae=n3a
              n3be=n3b
              iep11=1
              iep12=1
              iep21=side
              iep22=1-side
              call bcEdge( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
              gridIndexRange, u, mask,rsxy, xy, boundaryCondition, \
              ipar, rpar, ierr , n1ae, n1be, n2ae, n2be, n3ae, n3be,\
              iep11, iep12, iep21, iep22,axis,axisp1,axisp2,is1,is2,is3,is,\
              bcf00,bcf10,bcf01,bcf11,bcf02,bcf12,bcf0,bcOffset,\
              addBoundaryForcing,interfaceType,dim,side)

!     doEdge()
            end if
!     Axis 2
            if ((axis.eq.2).and.(boundaryCondition(side,2).eq.tractionBC).and.(boundaryCondition(0,0).eq.tractionBC)) then
              n1ae=n1a
              n1be=n1a
              n2ae=n2a
              n2be=n2b
              n3ae=n3a
              n3be=n3b
              iep11=1-side
              iep12=side
              iep21=1
              iep22=1
              call bcEdge( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
              gridIndexRange, u, mask,rsxy, xy, boundaryCondition, \
              ipar, rpar, ierr , n1ae, n1be, n2ae, n2be, n3ae, n3be,\
              iep11, iep12, iep21, iep22,axis,axisp1,axisp2,is1,is2,is3,is,\
              bcf00,bcf10,bcf01,bcf11,bcf02,bcf12,bcf0,bcOffset,\
              addBoundaryForcing,interfaceType,dim,side)

!     doEdge()
            end if
            if ((axis.eq.2).and.(boundaryCondition(side,2).eq.tractionBC).and.(boundaryCondition(1,0).eq.tractionBC)) then
              n1ae=n1b
              n1be=n1b
              n2ae=n2a
              n2be=n2b
              n3ae=n3a
              n3be=n3b
              iep11=side
              iep12=1-side
              iep21=1
              iep22=1
              call bcEdge( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
              gridIndexRange, u, mask,rsxy, xy, boundaryCondition, \
              ipar, rpar, ierr , n1ae, n1be, n2ae, n2be, n3ae, n3be,\
              iep11, iep12, iep21, iep22,axis,axisp1,axisp2,is1,is2,is3,is,\
              bcf00,bcf10,bcf01,bcf11,bcf02,bcf12,bcf0,bcOffset,\
              addBoundaryForcing,interfaceType,dim,side)

!     doEdge()
            end if
            if ((axis.eq.2).and.(boundaryCondition(side,2).eq.tractionBC).and.(boundaryCondition(0,1).eq.tractionBC)) then
              n1ae=n1a
              n1be=n1b
              n2ae=n2a
              n2be=n2a
              n3ae=n3a
              n3be=n3b
              iep11=1
              iep12=1
              iep21=1-side
              iep22=side
              call bcEdge( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
              gridIndexRange, u, mask,rsxy, xy, boundaryCondition, \
              ipar, rpar, ierr , n1ae, n1be, n2ae, n2be, n3ae, n3be,\
              iep11, iep12, iep21, iep22,axis,axisp1,axisp2,is1,is2,is3,is,\
              bcf00,bcf10,bcf01,bcf11,bcf02,bcf12,bcf0,bcOffset,\
              addBoundaryForcing,interfaceType,dim,side)

!     doEdge()
            end if
            if ((axis.eq.2).and.(boundaryCondition(side,2).eq.tractionBC).and.(boundaryCondition(1,1).eq.tractionBC)) then
              n1ae=n1a
              n1be=n1b
              n2ae=n2b
              n2be=n2b
              n3ae=n3a
              n3be=n3b
              iep11=1
              iep12=1
              iep21=side
              iep22=1-side
              call bcEdge( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
              gridIndexRange, u, mask,rsxy, xy, boundaryCondition, \
              ipar, rpar, ierr , n1ae, n1be, n2ae, n2be, n3ae, n3be,\
              iep11, iep12, iep21, iep22,axis,axisp1,axisp2,is1,is2,is3,is,\
              bcf00,bcf10,bcf01,bcf11,bcf02,bcf12,bcf0,bcOffset,\
              addBoundaryForcing,interfaceType,dim,side)

!     doEdge()
            end if
          endLoopOverSides()

!     Correct corners
          beginLoopOverSides(numGhost,numGhost)
            if ((axis.eq.0).and.(boundaryCondition(side,0).eq.tractionBC).and.(boundaryCondition(0,1).eq.tractionBC).and.(boundaryCondition(0,2).eq.tractionBC)) then
              n1ae=n1a
              n1be=n1b
              n2ae=n2a
              n2be=n2a
              n3ae=n3a
              n3be=n3a
              iep11=1-side
              iep12=side
              iep21=1-side
              iep22=side
              call bcEdge( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
              gridIndexRange, u, mask,rsxy, xy, boundaryCondition, \
              ipar, rpar, ierr , n1ae, n1be, n2ae, n2be, n3ae, n3be,\
              iep11, iep12, iep21, iep22,axis,axisp1,axisp2,is1,is2,is3,is,\
              bcf00,bcf10,bcf01,bcf11,bcf02,bcf12,bcf0,bcOffset,\
              addBoundaryForcing,interfaceType,dim,side)

!     doEdge()
            end if
            if ((axis.eq.0).and.(boundaryCondition(side,0).eq.tractionBC).and.(boundaryCondition(0,1).eq.tractionBC).and.(boundaryCondition(1,2).eq.tractionBC)) then
              n1ae=n1a
              n1be=n1b
              n2ae=n2a
              n2be=n2a
              n3ae=n3b
              n3be=n3b
              iep11=1-side
              iep12=side
              iep21=side
              iep22=1-side
              call bcEdge( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
              gridIndexRange, u, mask,rsxy, xy, boundaryCondition, \
              ipar, rpar, ierr , n1ae, n1be, n2ae, n2be, n3ae, n3be,\
              iep11, iep12, iep21, iep22,axis,axisp1,axisp2,is1,is2,is3,is,\
              bcf00,bcf10,bcf01,bcf11,bcf02,bcf12,bcf0,bcOffset,\
              addBoundaryForcing,interfaceType,dim,side)

!     doEdge()
            end if

            if ((axis.eq.0).and.(boundaryCondition(side,0).eq.tractionBC).and.(boundaryCondition(1,1).eq.tractionBC).and.(boundaryCondition(0,2).eq.tractionBC)) then
              n1ae=n1a
              n1be=n1b
              n2ae=n2b
              n2be=n2b
              n3ae=n3a
              n3be=n3a
              iep11=side
              iep12=1-side
              iep21=1-side
              iep22=side
              call bcEdge( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
              gridIndexRange, u, mask,rsxy, xy, boundaryCondition, \
              ipar, rpar, ierr , n1ae, n1be, n2ae, n2be, n3ae, n3be,\
              iep11, iep12, iep21, iep22,axis,axisp1,axisp2,is1,is2,is3,is,\
              bcf00,bcf10,bcf01,bcf11,bcf02,bcf12,bcf0,bcOffset,\
              addBoundaryForcing,interfaceType,dim,side)

!     doEdge()
            end if
            if ((axis.eq.0).and.(boundaryCondition(side,0).eq.tractionBC).and.(boundaryCondition(1,1).eq.tractionBC).and.(boundaryCondition(1,2).eq.tractionBC)) then
              n1ae=n1a
              n1be=n1b
              n2ae=n2b
              n2be=n2b
              n3ae=n3b
              n3be=n3b
              iep11=side
              iep12=1-side
              iep21=side
              iep22=1-side
              call bcEdge( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
              gridIndexRange, u, mask,rsxy, xy, boundaryCondition, \
              ipar, rpar, ierr , n1ae, n1be, n2ae, n2be, n3ae, n3be,\
              iep11, iep12, iep21, iep22,axis,axisp1,axisp2,is1,is2,is3,is,\
              bcf00,bcf10,bcf01,bcf11,bcf02,bcf12,bcf0,bcOffset,\
              addBoundaryForcing,interfaceType,dim,side)

!     doEdge()
            end if
!     Axis 1
            if ((axis.eq.1).and.(boundaryCondition(side,1).eq.tractionBC).and.(boundaryCondition(0,2).eq.tractionBC).and.(boundaryCondition(0,0).eq.tractionBC)) then
              n1ae=n1a
              n1be=n1a
              n2ae=n2a
              n2be=n2b
              n3ae=n3a
              n3be=n3a
              iep11=1-side
              iep12=side
              iep21=1-side
              iep22=side
              call bcEdge( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
              gridIndexRange, u, mask,rsxy, xy, boundaryCondition, \
              ipar, rpar, ierr , n1ae, n1be, n2ae, n2be, n3ae, n3be,\
              iep11, iep12, iep21, iep22,axis,axisp1,axisp2,is1,is2,is3,is,\
              bcf00,bcf10,bcf01,bcf11,bcf02,bcf12,bcf0,bcOffset,\
              addBoundaryForcing,interfaceType,dim,side)

!     doEdge()
            end if
            if ((axis.eq.1).and.(boundaryCondition(side,1).eq.tractionBC).and.(boundaryCondition(0,2).eq.tractionBC).and.(boundaryCondition(1,0).eq.tractionBC)) then
              n1ae=n1b
              n1be=n1b
              n2ae=n2a
              n2be=n2b
              n3ae=n3a
              n3be=n3a
              iep11=1-side
              iep12=side
              iep21=side
              iep22=1-side
              call bcEdge( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
              gridIndexRange, u, mask,rsxy, xy, boundaryCondition, \
              ipar, rpar, ierr , n1ae, n1be, n2ae, n2be, n3ae, n3be,\
              iep11, iep12, iep21, iep22,axis,axisp1,axisp2,is1,is2,is3,is,\
              bcf00,bcf10,bcf01,bcf11,bcf02,bcf12,bcf0,bcOffset,\
              addBoundaryForcing,interfaceType,dim,side)

            !     doEdge()
            end if
            if ((axis.eq.1).and.(boundaryCondition(side,1).eq.tractionBC).and.(boundaryCondition(1,2).eq.tractionBC).and.(boundaryCondition(0,0).eq.tractionBC)) then
              n1ae=n1a
              n1be=n1a
              n2ae=n2a
              n2be=n2b
              n3ae=n3b
              n3be=n3b
              iep11=side
              iep12=1-side
              iep21=1-side
              iep22=side
              call bcEdge( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
              gridIndexRange, u, mask,rsxy, xy, boundaryCondition, \
              ipar, rpar, ierr , n1ae, n1be, n2ae, n2be, n3ae, n3be,\
              iep11, iep12, iep21, iep22,axis,axisp1,axisp2,is1,is2,is3,is,\
              bcf00,bcf10,bcf01,bcf11,bcf02,bcf12,bcf0,bcOffset,\
              addBoundaryForcing,interfaceType,dim,side)

            !     doEdge()
            end if
            if ((axis.eq.1).and.(boundaryCondition(side,1).eq.tractionBC).and.(boundaryCondition(1,2).eq.tractionBC).and.(boundaryCondition(1,0).eq.tractionBC)) then
              n1ae=n1b
              n1be=n1b
              n2ae=n2a
              n2be=n2b
              n3ae=n3b
              n3be=n3b
              iep11=side
              iep12=1-side
              iep21=side
              iep22=1-side
              call bcEdge( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
              gridIndexRange, u, mask,rsxy, xy, boundaryCondition, \
              ipar, rpar, ierr , n1ae, n1be, n2ae, n2be, n3ae, n3be,\
              iep11, iep12, iep21, iep22,axis,axisp1,axisp2,is1,is2,is3,is,\
              bcf00,bcf10,bcf01,bcf11,bcf02,bcf12,bcf0,bcOffset,\
              addBoundaryForcing,interfaceType,dim,side)

            !     doEdge()
            end if

            !     Axis 2
            if ((axis.eq.2).and.(boundaryCondition(side,2).eq.tractionBC).and.(boundaryCondition(0,0).eq.tractionBC).and.(boundaryCondition(0,1).eq.tractionBC)) then
              n1ae=n1a
              n1be=n1a
              n2ae=n2a
              n2be=n2a
              n3ae=n3a
              n3be=n3b
              iep11=1-side
              iep12=side
              iep21=1-side
              iep22=side
              call bcEdge( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
              gridIndexRange, u, mask,rsxy, xy, boundaryCondition, \
              ipar, rpar, ierr , n1ae, n1be, n2ae, n2be, n3ae, n3be,\
              iep11, iep12, iep21, iep22,axis,axisp1,axisp2,is1,is2,is3,is,\
              bcf00,bcf10,bcf01,bcf11,bcf02,bcf12,bcf0,bcOffset,\
              addBoundaryForcing,interfaceType,dim,side)

             !     doEdge()
            end if
            if ((axis.eq.2).and.(boundaryCondition(side,2).eq.tractionBC).and.(boundaryCondition(0,0).eq.tractionBC).and.(boundaryCondition(1,1).eq.tractionBC)) then
              n1ae=n1a
              n1be=n1a
              n2ae=n2b
              n2be=n2b
              n3ae=n3a
              n3be=n3b
              iep11=1-side
              iep12=side
              iep21=side
              iep22=1-side
              call bcEdge( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
              gridIndexRange, u, mask,rsxy, xy, boundaryCondition, \
              ipar, rpar, ierr , n1ae, n1be, n2ae, n2be, n3ae, n3be,\
              iep11, iep12, iep21, iep22,axis,axisp1,axisp2,is1,is2,is3,is,\
              bcf00,bcf10,bcf01,bcf11,bcf02,bcf12,bcf0,bcOffset,\
              addBoundaryForcing,interfaceType,dim,side)

              !     doEdge()
            end if
            if ((axis.eq.2).and.(boundaryCondition(side,2).eq.tractionBC).and.(boundaryCondition(1,0).eq.tractionBC).and.(boundaryCondition(0,1).eq.tractionBC)) then
              n1ae=n1b
              n1be=n1b
              n2ae=n2a
              n2be=n2a
              n3ae=n3a
              n3be=n3b
              iep11=side
              iep12=1-side
              iep21=1-side
              iep22=side
              call bcEdge( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
              gridIndexRange, u, mask,rsxy, xy, boundaryCondition, \
              ipar, rpar, ierr , n1ae, n1be, n2ae, n2be, n3ae, n3be,\
              iep11, iep12, iep21, iep22,axis,axisp1,axisp2,is1,is2,is3,is,\
              bcf00,bcf10,bcf01,bcf11,bcf02,bcf12,bcf0,bcOffset,\
              addBoundaryForcing,interfaceType,dim,side)

              !     doEdge()
            end if
            if ((axis.eq.2).and.(boundaryCondition(side,2).eq.tractionBC).and.(boundaryCondition(1,0).eq.tractionBC).and.(boundaryCondition(1,1).eq.tractionBC)) then
              n1ae=n1b
              n1be=n1b
              n2ae=n2b
              n2be=n2b
              n3ae=n3a
              n3be=n3b
              iep11=side
              iep12=1-side
              iep21=side
              iep22=1-side
              call bcEdge( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
              gridIndexRange, u, mask,rsxy, xy, boundaryCondition, \
              ipar, rpar, ierr , n1ae, n1be, n2ae, n2be, n3ae, n3be,\
              iep11, iep12, iep21, iep22,axis,axisp1,axisp2,is1,is2,is3,is,\
              bcf00,bcf10,bcf01,bcf11,bcf02,bcf12,bcf0,bcOffset,\
              addBoundaryForcing,interfaceType,dim,side)

              !     doEdge()
            end if
          endLoopOverSides()
        else
          !     un-known nd and orderOfAccuracy
          stop 6663
        end if

      else
        !     unknown nd 
        stop 8826
      end if

      return
      end


!     This subroutine is used instead of the macro doEdge()
!     to produce shorter source code.
      subroutine bcEdge( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
        gridIndexRange, u, mask,rsxy, xy, boundaryCondition, \
        ipar, rpar, ierr , n1ae, n1be, n2ae, n2be, n3ae, n3be,\
        iep11, iep12, iep21, iep22,axis,axisp1,axisp2,is1,is2,is3,is,\
        bcf00,bcf10,bcf01,bcf11,bcf02,bcf12,bcf0,bcOffset,\
        addBoundaryForcing,interfaceType,dim,side)

      implicit none

      integer nd, \
      nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, \
      ierr

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      integer gridIndexRange(0:1,0:2),boundaryCondition(0:1,0:2)

      integer ipar(0:*)
      real rpar(0:*)

      integer*8 bcOffset(0:1,0:2)
      integer addBoundaryForcing(0:1,0:2)
      integer interfaceType(0:1,0:2,0:*)
      integer dim(0:1,0:2,0:1,0:2)
      real bcf00(dim(0,0,0,0):dim(1,0,0,0), dim(0,1,0,0):dim(1,1,0,0), dim(0,2,0,0):dim(1,2,0,0),0:*)
      real bcf10(dim(0,0,1,0):dim(1,0,1,0), dim(0,1,1,0):dim(1,1,1,0), dim(0,2,1,0):dim(1,2,1,0),0:*)
      real bcf01(dim(0,0,0,1):dim(1,0,0,1), dim(0,1,0,1):dim(1,1,0,1), dim(0,2,0,1):dim(1,2,0,1),0:*)
      real bcf11(dim(0,0,1,1):dim(1,0,1,1), dim(0,1,1,1):dim(1,1,1,1), dim(0,2,1,1):dim(1,2,1,1),0:*)
      real bcf02(dim(0,0,0,2):dim(1,0,0,2), dim(0,1,0,2):dim(1,1,0,2), dim(0,2,0,2):dim(1,2,0,2),0:*)
      real bcf12(dim(0,0,1,2):dim(1,0,1,2), dim(0,1,1,2):dim(1,1,1,2), dim(0,2,1,2):dim(1,2,1,2),0:*)

      real bcf0(0:*)


!     --- local variables ----

      integer side,axis,grid,gridType,orderOfAccuracy,orderOfExtrapolation,twilightZone,\
      uc,vc,wc,useWhereMask,debug,nn,n1,n2
      real dx(0:2),dr(0:2),dri(0:3),dxi(0:3)
      real t,ep,dt,c1,c2,rho,mu,lambda,alpha
      integer axisp1,axisp2,i1,i2,i3,is1,is2,is3,j1,j2,j3,js1,js2,js3,ks1,ks2,ks3,is,js,it,nit
      integer iep1,iep2,iep11,iep12,iep21,iep22
      integer option,initialized,itc

      integer numGhost,numberOfGhostPoints
      integer side1,side2,idxs,i
      integer n1a,n1b,n2a,n2b,n3a,n3b
      integer n1ae,n1be,n2ae,n2be,n3ae,n3be
      integer nn1a,nn1b,nn2a,nn2b,nn3a,nn3b
      integer extra1a,extra1b,extra2a,extra2b,extra3a,extra3b
      real epsRatio,an1,an2,an3,aNorm,ua,ub,nDotU
      real epsx
      real ux0,uy0,uz0,vx0,vy0,vz0,wx0,wy0,wz0
      real lam2muM,lamM,muM,lam3M,lam2mu3M,mu3M,jac3,lambdaPlus2mu
      integer numberOfEquations,job
      real a2(0:1,0:1),a3(0:2,0:2),a4(0:3,0:3),a8(0:7,0:7),q(0:11),f(0:11),ipvt(0:11),rcond,work(0:11)

      real err

!     define boundary conditions:
#Include "bcDefineFortranInclude.h"

      integer rectangular,curvilinear
      parameter(\
      rectangular=0,\
      curvilinear=1)


!     --- start statement function ----
      integer kd,m,n
      real bcf
      Jac3(i1,i2,i3)=(1.d0/(rsxy(i1,i2,i3,0,0)*rsxy(i1,i2,i3,1,1)*rsxy(i1,i2,i3,2,2)+rsxy(i1,i2,i3,0,1)*rsxy(i1,i2,i3,1,2)*rsxy(i1,i2,i3,2,0)+rsxy(i1,i2,i3,0,2)*rsxy(i1,i2,i3,1,0)*rsxy(i1,i2,i3,2,1)-rsxy(i1,i2,i3,2,0)*rsxy(i1,i2,i3,1,1)*rsxy(i1,i2,i3,0,2)-rsxy(i1,i2,i3,2,1)*rsxy(i1,i2,i3,1,2)*rsxy(i1,i2,i3,0,0)-rsxy(i1,i2,i3,2,2)*rsxy(i1,i2,i3,1,0)*rsxy(i1,i2,i3,0,1)))

!!$      lamM(i1,i2)=(c1-c2)
!!$      muM(i1,i2)=c2
!!$      lam2muM(i1,i2)=(lamM(i1,i2)+2.0*muM(i1,12))
!!$      lam3M(i1,i2,i3)=(c1-c2)
!!$      mu3M(i1,i2,i3)=c2
!!$      lam2mu3M(i1,i2,i3)=(lam3M(i1,i2,i3)+2.0*mu3M(i1,12,i3))

      lamM(i1,i2)=lambda
      muM(i1,i2)=mu
      lam2muM(i1,i2)=lambdaPlus2mu
      lam3M(i1,i2,i3)=lambda
      mu3M(i1,i2,i3)=mu
      lam2mu3M(i1,i2,i3)=lambdaPlus2mu

!     Here is the the generic boundary condition forcing array. It uses the bcOffset(side,axis) values as an
!     an offset from the bcf0 array to access the bcf10, bcf01, bcf11, ... arrays
      bcf(side,axis,i1,i2,i3,m) = bcf0(bcOffset(side,axis) + \
      (i1-dim(0,0,side,axis)+(dim(1,0,side,axis)-dim(0,0,side,axis)+1)* \
      (i2-dim(0,1,side,axis)+(dim(1,1,side,axis)-dim(0,1,side,axis)+1)* \
      (i3-dim(0,2,side,axis)+(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(m)))))

!...............end statement functions

      ierr=0

      nd                   =ipar(0)
      grid                 =ipar(1)
      uc                   =ipar(2)
      vc                   =ipar(3)
      wc                   =ipar(4)
      gridType             =ipar(5)
      orderOfAccuracy      =ipar(6)
      orderOfExtrapolation =ipar(7)
      twilightZone         =ipar(8)
      useWhereMask         =ipar(9)
      debug                =ipar(10)

      dx(0)                =rpar(0)
      dx(1)                =rpar(1)
      dx(2)                =rpar(2)
      dr(0)                =rpar(3)
      dr(1)                =rpar(4)
      dr(2)                =rpar(5)
      t                    =rpar(6)
      ep                   =rpar(7) ! pointer for exact solution
      dt                   =rpar(8)
      rho                  =rpar(9)
      mu                   =rpar(10)
      lambda               =rpar(11)
      c1                   =rpar(12)
      c2                   =rpar(13)

      job=0                     ! *wdh* 090101

      if (0.eq.1) write (*,*) "DEAA INNER",n3ae,n3be,n2ae,n2be,n1ae,n1be,iep11,
     &     iep12, iep21, iep22
!     write(*,'(" bcOptSmCons t=",e10.2)') t

      epsx=1.e-20              ! fix this
      numGhost=orderOfAccuracy/2
      do i=0,2
       dri(i)=1.0d0/dr(i)
      enddo
      doEdge()

      return
      end

