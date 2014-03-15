c *******************************************************************************
c   Absorbing boundary conditions
c *******************************************************************************

c These next include files will define the macros that will define the difference approximations
c The actual macro is called below
#Include "defineDiffOrder2f.h"
#Include "defineDiffOrder4f.h"

c Here are macros that define the planeWave solution
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

#beginMacro beginLoopsMask()
do i3=n3a,n3b
do i2=n2a,n2b
do i1=n1a,n1b
if( mask(i1,i2,i3).gt.0 )then
#endMacro

#beginMacro endLoopsMask()
end if
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

     ! write(*,'(" abc: grid,side,axis,bc=",3i2)') grid,side,axis,boundaryCondition(side,axis)

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
     is = 1 - 2*side
     
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

! -------------------- CARTESIAN GRID ---------------------
#defineMacro ABCEM2Xa(i1,i2,i3,cc) (un(i1+is1,i2+is2,i3+is3,cc) \
                    - (u(i1+is1,i2+is2,i3+is3,cc)-u(i1-is1,i2-is2,i3-is3,cc))\
                    - (2.*dxa*dt)*( c1abcem2*uxx22r(i1,i2,i3,cc) + c2abcem2*uyy22r(i1,i2,i3,cc) ) )
#defineMacro ABCEM2Ya(i1,i2,i3,cc) (un(i1+is1,i2+is2,i3+is3,cc) \
                    - (u(i1+is1,i2+is2,i3+is3,cc)-u(i1-is1,i2-is2,i3-is3,cc))\
                    - (2.*dya*dt)*( c1abcem2*uyy22r(i1,i2,i3,cc) + c2abcem2*uxx22r(i1,i2,i3,cc) ) )

#defineMacro ABCEM23DXa(i1,i2,i3,is1,is2,is3,cc) (un(i1+is1,i2+is2,i3+is3,cc) \
                  - (u(i1+is1,i2+is2,i3+is3,cc)-u(i1-is1,i2-is2,i3-is3,cc))\
                  - (2.*dxa*dt)*( c1abcem2*uxx23r(i1,i2,i3,cc) + c2abcem2*(uyy23r(i1,i2,i3,cc)+uzz23r(i1,i2,i3,cc)) ) )
#defineMacro ABCEM23DYa(i1,i2,i3,is1,is2,is3,cc) (un(i1+is1,i2+is2,i3+is3,cc) \
                  - (u(i1+is1,i2+is2,i3+is3,cc)-u(i1-is1,i2-is2,i3-is3,cc))\
                  - (2.*dya*dt)*( c1abcem2*uyy23r(i1,i2,i3,cc) + c2abcem2*(uxx23r(i1,i2,i3,cc)+uzz23r(i1,i2,i3,cc)) ) )
#defineMacro ABCEM23DZa(i1,i2,i3,is1,is2,is3,cc) (un(i1+is1,i2+is2,i3+is3,cc) \
                  - (u(i1+is1,i2+is2,i3+is3,cc)-u(i1-is1,i2-is2,i3-is3,cc))\
                  - (2.*dza*dt)*( c1abcem2*uzz23r(i1,i2,i3,cc) + c2abcem2*(uxx23r(i1,i2,i3,cc)+uyy23r(i1,i2,i3,cc)) ) )

! Here is a 2nd-order in time approx
#defineMacro ABCEM2X(i1,i2,i3,cc) ( (un(i1+is1,i2+is2,i3+is3,cc) \
                    - (u(i1+is1,i2+is2,i3+is3,cc)-u(i1-is1,i2-is2,i3-is3,cc))\
         - (dxa*dt)*( c1abcem2*uxx22r(i1,i2,i3,cc) + c2abcem2*uyy22r(i1,i2,i3,cc) \
                     +c1abcem2*(                    -2.*un(i1,i2,i3,cc)+un(i1+is1,i2,i3,cc))/dxa**2 \
                     +c2abcem2*( un(i1  ,i2-1,i3,cc)-2.*un(i1,i2,i3,cc)+un(i1  ,i2+1,i3,cc))/dx(1)**2 )\
                                  )/(1.+c1abcem2*dt/dxa) )
#defineMacro ABCEM2Y(i1,i2,i3,cc) ( (un(i1+is1,i2+is2,i3+is3,cc) \
                    - (u(i1+is1,i2+is2,i3+is3,cc)-u(i1-is1,i2-is2,i3-is3,cc))\
         - (dya*dt)*( c1abcem2*uyy22r(i1,i2,i3,cc) + c2abcem2*uxx22r(i1,i2,i3,cc) \
                     +c1abcem2*(                    -2.*un(i1,i2,i3,cc)+un(i1,i2+is2,i3,cc))/dya**2 \
                     +c2abcem2*( un(i1-1,i2  ,i3,cc)-2.*un(i1,i2,i3,cc)+un(i1+1,i2  ,i3,cc))/dx(0)**2 )\
                                  )/(1.+c1abcem2*dt/dya) )

#defineMacro ABCEM23DX(i1,i2,i3,cc) ( (un(i1+is1,i2+is2,i3+is3,cc) \
                    - (u(i1+is1,i2+is2,i3+is3,cc)-u(i1-is1,i2-is2,i3-is3,cc))\
         - (dxa*dt)*( c1abcem2*uxx23r(i1,i2,i3,cc) + c2abcem2*(uyy23r(i1,i2,i3,cc)+uzz23r(i1,i2,i3,cc)) \
                     +c1abcem2*(                    -2.*un(i1,i2,i3,cc)+un(i1+is1,i2,i3,cc))/dxa**2 \
                     +c2abcem2*( un(i1  ,i2-1,i3,cc)-2.*un(i1,i2,i3,cc)+un(i1  ,i2+1,i3,cc))/dx(1)**2 \
                     +c2abcem2*( un(i1  ,i2,i3-1,cc)-2.*un(i1,i2,i3,cc)+un(i1  ,i2,i3+1,cc))/dx(2)**2 )\
                                  )/(1.+c1abcem2*dt/dxa) )
#defineMacro ABCEM23DY(i1,i2,i3,cc) ( (un(i1+is1,i2+is2,i3+is3,cc) \
                    - (u(i1+is1,i2+is2,i3+is3,cc)-u(i1-is1,i2-is2,i3-is3,cc))\
         - (dya*dt)*( c1abcem2*uyy23r(i1,i2,i3,cc) + c2abcem2*(uxx23r(i1,i2,i3,cc)+uzz23r(i1,i2,i3,cc)) \
                     +c1abcem2*(                    -2.*un(i1,i2,i3,cc)+un(i1,i2+is2,i3,cc))/dya**2 \
                     +c2abcem2*( un(i1-1,i2  ,i3,cc)-2.*un(i1,i2,i3,cc)+un(i1+1,i2  ,i3,cc))/dx(0)**2 \
                     +c2abcem2*( un(i1  ,i2,i3-1,cc)-2.*un(i1,i2,i3,cc)+un(i1  ,i2,i3+1,cc))/dx(2)**2 )\
                                  )/(1.+c1abcem2*dt/dya) )

#defineMacro ABCEM23DZ(i1,i2,i3,cc) ( (un(i1+is1,i2+is2,i3+is3,cc) \
                    - (u(i1+is1,i2+is2,i3+is3,cc)-u(i1-is1,i2-is2,i3-is3,cc))\
         - (dza*dt)*( c1abcem2*uzz23r(i1,i2,i3,cc) + c2abcem2*(uxx23r(i1,i2,i3,cc)+uyy23r(i1,i2,i3,cc)) \
                     +c1abcem2*(                    -2.*un(i1,i2,i3,cc)+un(i1,i2,i3+is3,cc))/dza**2 \
                     +c2abcem2*( un(i1-1,i2  ,i3,cc)-2.*un(i1,i2,i3,cc)+un(i1+1,i2  ,i3,cc))/dx(0)**2 \
                     +c2abcem2*( un(i1  ,i2-1,i3,cc)-2.*un(i1,i2,i3,cc)+un(i1  ,i2+1,i3,cc))/dx(1)**2 )\
                                  )/(1.+c1abcem2*dt/dza) )


#beginMacro extrapLine1Order2(i1,i2,i3,is1,is2,is3,cc)
  un(i1-is1,i2-is2,i3-is3,cc)=2.*un(i1,i2,i3,cc)-un(i1+is1,i2+is2,i3+is3,cc)
#endMacro

#beginMacro extrapLine1Order3(i1,i2,i3,is1,is2,is3,cc)
  un(i1-is1,i2-is2,i3-is3,cc)=3.*un(i1,i2,i3,cc)-3.*un(i1+is1,i2+is2,i3+is3,cc)\
                             +un(i1+2*is1,i2+2*is2,i3+2*is3,cc)
#endMacro

#beginMacro extrapLine1Order4(i1,i2,i3,is1,is2,is3,cc)
  un(i1-is1,i2-is2,i3-is3,cc)=4.*un(i1,i2,i3,cc)-6.*un(i1+is1,i2+is2,i3+is3,cc)\
                             +4.*un(i1+2*is1,i2+2*is2,i3+2*is3,cc)-un(i1+3*is1,i2+3*is2,i3+3*is3,cc)
#endMacro

#beginMacro extrapLine2Order4(i1,i2,i3,is1,is2,is3,cc)
  un(i1-2*is1,i2-2*is2,i3-2*is3,cc)=4.*un(i1-is1,i2-is2,i3-is3,cc)-6.*un(i1,i2,i3,cc)\
                                   +4.*un(i1+is1,i2+is2,i3+is3,cc)-un(i1+2*is1,i2+2*is2,i3+2*is3,cc)
#endMacro

#defineMacro extrap2(uu,k1,k2,k3,kc,ks1,ks2,ks3) \
            (2.*uu(k1,k2,k3,kc)-uu(k1+ks1,k2+ks2,k3+ks3,kc))

#defineMacro extrap3(uu,k1,k2,k3,kc,ks1,ks2,ks3) \
            (3.*uu(k1,k2,k3,kc)-3.*uu(k1+ks1,k2+ks2,k3+ks3,kc)\
            +   uu(k1+2*ks1,k2+2*ks2,k3+2*ks3,kc))

#defineMacro extrap4(uu,k1,k2,k3,kc,ks1,ks2,ks3) \
            (4.*uu(k1,k2,k3,kc)-6.*uu(k1+ks1,k2+ks2,k3+ks3,kc)\
            +4.*uu(k1+2*ks1,k2+2*ks2,k3+2*ks3,kc)-uu(k1+3*ks1,k2+3*ks2,k3+3*ks3,kc))

#defineMacro extrap5(uu,k1,k2,k3,kc,ks1,ks2,ks3) \
            (5.*uu(k1,k2,k3,kc)-10.*uu(k1+ks1,k2+ks2,k3+ks3,kc)\
            +10.*uu(k1+2*ks1,k2+2*ks2,k3+2*ks3,kc)-5.*uu(k1+3*ks1,k2+3*ks2,k3+3*ks3,kc)\
            +uu(k1+4*ks1,k2+4*ks2,k3+4*ks3,kc))

#beginMacro getNormal2d(i1,i2,i3, axis)
 an1=rsxy(i1,i2,i3,axis,0)
 an2=rsxy(i1,i2,i3,axis,1)
 aNorm = 1./max(epsX,sqrt(an1**2+an2**2))
 an1=an1*aNorm
 an2=an2*aNorm
#endMacro

#beginMacro getNormal3d(i1,i2,i3, axis)
 an1=rsxy(i1,i2,i3,axis,0)
 an2=rsxy(i1,i2,i3,axis,1)
 an3=rsxy(i1,i2,i3,axis,2)
 aNorm = 1./max(epsX,sqrt(an1**2+an2**2+an3**2))
 an1=an1*aNorm
 an2=an2*aNorm
 an3=an3*aNorm
#endMacro


! ======================================================================================
! Setup Macro to apply the 2nd-order accurate Engquist-Majda ABC on a curvilinear grid
! 
! On a Curvlinear grid we write:
!        u_tt = L u  
!        u_tt = D_n^2 u + (L-D_n^2) u 
! where the "normal" derivative is 
!        D_n = sqrt( rx^2 + ry^2) D_r 
! 
!    sqrt( rx^2 + ry^2) u_{rt} = c1abcem2*( (rx^2 + ry^2) u_{rr} ) + c2abcem2*( L - (rx^2 + ry^2) u_{rr} )
! 
! ======================================================================================
#beginMacro abcSetup2d(i1,i2,i3,is1,is2,is3,side,axis)
  is =1-2*side
  dr0=dr(axis)
  rx0 = rsxy(i1,i2,i3,axis,0)
  ry0 = rsxy(i1,i2,i3,axis,1)
  rxNormSq = rx0**2 + ry0**2 
  rxNorm = max( epsX, sqrt(rxNormSq) )

  rxx0 = rsxyx22(i1,i2,i3,axis,0)
  ryy0 = rsxyy22(i1,i2,i3,axis,1)

  ! cm1 : coeff of u(i1-is1,i2-is2,i3-is3,cc) in g (given below): 
  cm1 = -rxNorm/(2.*dr0*dt) -.5*( c1abcem2*( rxNormSq/dr0**2 ) + c2abcem2*( -is*(rxx0+ryy0)/(2.*dr0) ) )
#endMacro


! ======================================================================================
! Macro to apply the 2nd-order accurate Engquist-Majda ABC on a curvilinear grid
! ======================================================================================
#beginMacro abc2d(i1,i2,i3,is1,is2,is3,side,axis,cc)

  ! u: derivatives at time t: 
  ! un: derivatives at time t+dt : evaluate using the incorrect ghost values 
  if( axis.eq.0 )then
    ur0   =   ur2(i1,i2,i3,cc)
    urr0  =  urr2(i1,i2,i3,cc)
    unr0  =  unr2(i1,i2,i3,cc)
    unrr0 = unrr2(i1,i2,i3,cc)
  else
    ur0   =   us2(i1,i2,i3,cc)
    urr0  =  uss2(i1,i2,i3,cc)
    unr0  =  uns2(i1,i2,i3,cc)
    unrr0 = unss2(i1,i2,i3,cc)
  end if
  uxx0 = uxx22(i1,i2,i3,cc)
  uyy0 = uyy22(i1,i2,i3,cc)

  unxx0 = unxx22(i1,i2,i3,cc)
  unyy0 = unyy22(i1,i2,i3,cc)


  ! first evaluate the BC using the incorrect ghost values 

  Dn2 = rxNormSq*(unrr0+urr0) 
  Lu  = unxx0+unyy0 + uxx0+uyy0 
  g = is*rxNorm*(unr0-ur0)/dt -.5*( c1abcem2*( Dn2 ) + c2abcem2*( Lu - Dn2 ) )

  ! note: this assumes an orthogonal grid -- we should make sure that the 
  !       ghost values have an initial guess in them (extrapolate ?)

  un(i1-is1,i2-is2,i3-is3,cc) = -(g - cm1*un(i1-is1,i2-is2,i3-is3,cc) )/cm1 

#endMacro

! First-order in time explicit version: 
#beginMacro abc2de(i1,i2,i3,is1,is2,is3,side,axis,cc)

  if( axis.eq.0 )then
    ur0   =   ur2(i1,i2,i3,cc)
    urr0  =  urr2(i1,i2,i3,cc)
    unr0  =  unr2(i1,i2,i3,cc)
  else
    ur0   =   us2(i1,i2,i3,cc)
    urr0  =  uss2(i1,i2,i3,cc)
    unr0  =  uns2(i1,i2,i3,cc)
  end if
  uxx0 = uxx22(i1,i2,i3,cc)
  uyy0 = uyy22(i1,i2,i3,cc)


  ! first evaluate the BC using the incorrect ghost values 
  Dn2 = rxNormSq*(urr0) 
  Lu  = uxx0+uyy0 
  g = is*rxNorm*(unr0-ur0)/dt - ( c1abcem2*( Dn2 ) + c2abcem2*( Lu - Dn2 ) )

  ! note: this assumes an orthogonal grid -- we should make sure that the 
  !       ghost values have an initial guess in them (extrapolate ?)
  cm1 = -rxNorm/(2.*dr0*dt) 

  un(i1-is1,i2-is2,i3-is3,cc) = -(g - cm1*un(i1-is1,i2-is2,i3-is3,cc) )/cm1 

#endMacro

#beginMacro abcSetup3d(i1,i2,i3,is1,is2,is3,side,axis)
  is =1-2*side
  dr0=dr(axis)
  rx0 = rsxy(i1,i2,i3,axis,0)
  ry0 = rsxy(i1,i2,i3,axis,1)
  rz0 = rsxy(i1,i2,i3,axis,2)
  rxNormSq = rx0**2 + ry0**2 + rz0**2
  rxNorm = max( epsX, sqrt(rxNormSq) )

  rxx0 = rsxyx23(i1,i2,i3,axis,0)
  ryy0 = rsxyy23(i1,i2,i3,axis,1)
  rzz0 = rsxyz23(i1,i2,i3,axis,2)

  ! cm1 : coeff of u(i1-is1,i2-is2,i3-is3,cc) in g (given below): 
  cm1 = -rxNorm/(2.*dr0*dt) -.5*( c1abcem2*( rxNormSq/dr0**2 ) + c2abcem2*( -is*(rxx0+ryy0+rzz0)/(2.*dr0) ) )
#endMacro

#beginMacro abc3d(i1,i2,i3,is1,is2,is3,side,axis,cc)

  ! derivatives at time t: 
  ! derivatives at time t+dt : evaluate using the incorrect ghost values 
  if( axis.eq.0 )then
    ur0   =   ur2(i1,i2,i3,cc)
    urr0  =  urr2(i1,i2,i3,cc)
    unr0  =  unr2(i1,i2,i3,cc)
    unrr0 = unrr2(i1,i2,i3,cc)
  else if( axis.eq.1 )then
    ur0   =   us2(i1,i2,i3,cc)
    urr0  =  uss2(i1,i2,i3,cc)
    unr0  =  uns2(i1,i2,i3,cc)
    unrr0 = unss2(i1,i2,i3,cc)
  else
    ur0   =   ut2(i1,i2,i3,cc)
    urr0  =  utt2(i1,i2,i3,cc)
    unr0  =  unt2(i1,i2,i3,cc)
    unrr0 = untt2(i1,i2,i3,cc)
  end if
  uxx0 = uxx23(i1,i2,i3,cc)
  uyy0 = uyy23(i1,i2,i3,cc)
  uzz0 = uzz23(i1,i2,i3,cc)

  unxx0 = unxx23(i1,i2,i3,cc)
  unyy0 = unyy23(i1,i2,i3,cc)
  unzz0 = unzz23(i1,i2,i3,cc)

  ! first evaluate the BC using the incorrect ghost values 

  Dn2 = rxNormSq*(unrr0+urr0) 
  Lu  = unxx0 + unyy0 + unzz0 + uxx0 + uyy0 + uzz0 
  g = is*rxNorm*(unr0-ur0)/dt -.5*( c1abcem2*( Dn2 ) + c2abcem2*( Lu - Dn2 ) )

  ! note: this assumes an orthogonal grid -- we should make sure that the 
  !       ghost values have an initial guess in them (extrapolate ?)

  un(i1-is1,i2-is2,i3-is3,cc) = -(g - cm1*un(i1-is1,i2-is2,i3-is3,cc) )/cm1 

#endMacro

! ============================================================================
! Macro to extrapolate points adjacent to an edge or corner
! EXCEPT for the first ghost point on the extended boundary
! ============================================================================
#beginMacro extrapolateGhost(ex,ey,ez,numGhost1,numGhost2,numGhost3)
 do m3=0,numGhost3
 do m2=0,numGhost2
 do m1=0,numGhost1
  mSum = m1+m2+m3
  if( mSum.gt.0 .and. mSum.ne.1 )then ! mSum=1 : these points have already been assigned
   ! extrap ghost point (j1,j2,j3)
   j1=i1-is1*m1
   j2=i2-is2*m2
   j3=i3-is3*m3
   ! js1=0 if m1=0 and js1=is1 if m1>0
   js1 = is1*min(m1,1)
   js2 = is2*min(m2,1)
   js3 = is3*min(m3,1)
   if( orderOfAccuracy.eq.2 )then
     un(j1,j2,j3,ex)=extrap2(un,j1+js1,j2+js2,j3+js3,ex,js1,js2,js3)
     un(j1,j2,j3,ey)=extrap2(un,j1+js1,j2+js2,j3+js3,ey,js1,js2,js3)
     un(j1,j2,j3,ez)=extrap2(un,j1+js1,j2+js2,j3+js3,ez,js1,js2,js3)
   else                                                                           
     ! extrap first line to a max of order 3 in case we adjust for incident fields 
     if( m1.le.1 .and. m2.le.1 .and. m3.le.1 )then
       un(j1,j2,j3,ex)=extrap3(un,j1+js1,j2+js2,j3+js3,ex,js1,js2,js3)
       un(j1,j2,j3,ey)=extrap3(un,j1+js1,j2+js2,j3+js3,ey,js1,js2,js3)
       un(j1,j2,j3,ez)=extrap3(un,j1+js1,j2+js2,j3+js3,ez,js1,js2,js3)
     else
       ! 2nd-ghost line 
       un(j1,j2,j3,ex)=extrap4(un,j1+js1,j2+js2,j3+js3,ex,js1,js2,js3)
       un(j1,j2,j3,ey)=extrap4(un,j1+js1,j2+js2,j3+js3,ey,js1,js2,js3)
       un(j1,j2,j3,ez)=extrap4(un,j1+js1,j2+js2,j3+js3,ez,js1,js2,js3)
     end if
  end if
  end if
 end do
 end do
 end do
#endMacro


      subroutine abcMaxwell( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
                               ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,\
                               gridIndexRange, u, un, f,mask,rsxy, xy,\
                               bc, boundaryCondition, ipar, rpar, ierr )
! ===================================================================================
!  Absorbing boundary conditions for Maxwell's Equations.
!
!  gridType : 0=rectangular, 1=curvilinear
!  useForcing : 1=use f for RHS to BC
!  side,axis : 0:1 and 0:2
!
!  u : solution at time t-dt
!  un : solution at time t (apply BC to this solution)
!
! ===================================================================================

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

!     --- local variables ----
      
      integer side,axis,gridType,orderOfAccuracy,orderOfExtrapolation,useForcing,\
        ex,ey,ez,hx,hy,hz,useWhereMask,grid,debug,side1,side2,side3
      real dx(0:2),dr(0:2),t,ep,dt,c      
      real dxa,dya,dza
      integer axisp1,axisp2,i1,i2,i3,is1,is2,is3,js1,js2,js3,ks1,ks2,ks3,is,j1,j2,j3,m1,m2,m3,mSum
      integer ip1,ip2,ip3,ig1,ig2,ig3,ghost1,ghost2,ghost3
      integer extra,extra1a,extra1b,extra2a,extra2b,extra3a,extra3b,numberOfGhostPoints
      integer edgeDirection,sidea,sideb,sidec,bc1,bc2,bc3

      real p0,p2,q0,q2,c1abcem2,c2abcem2
      real an1,an2,an3,aNorm,epsX

      real rx0,ry0,rz0 , rxx0,ryy0, rzz0 
      real dr0,cxt,cxx,cyy,czz,cm1,g,bxx,byy,bzz
      real rxNorm, rxNormSq, Dn2, Lu, ur0,urr0, unr0, unrr0
      real ux0,uy0,uz0, uxx0,uyy0,uzz0
      real unx0,uny0,unz0, unxx0,unyy0,unzz0
      real t0,t1,t2

      real eps,mu,kx,ky,kz,slowStartInterval,twoPi,cc

      real ax,ay,az,aSq,div,divCoeff
      logical adjacentFaceIsABC
      integer projectDivLine

      ! boundary conditions parameters
      #Include "bcDefineFortranInclude.h"

      integer rectangular,curvilinear
      parameter(\
        rectangular=0,\
        curvilinear=1)


!     --- start statement function ----
      integer kd,m,n
      real rx,ry,rz,sx,sy,sz,tx,ty,tz
      ! include 'declareDiffOrder2f.h'
      ! include 'declareDiffOrder4f.h'
!*      declareDifferenceOrder2(u,RX)
!*      declareDifferenceOrder4(u,RX)
      declareDifferenceOrder2(u,RX)
      declareDifferenceOrder2(un,none)

      declareDifferenceOrder4(u,RX)
      declareDifferenceOrder4(un,none)
#Include "declareJacobianDerivatives.h"

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
      defineDifferenceOrder2Components1(u,RX)
      defineDifferenceOrder2Components1(un,none)
      defineDifferenceOrder4Components1(u,RX)
      defineDifferenceOrder4Components1(un,none)

#Include "jacobianDerivatives.h"

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

      ! For fourth order, when we set div(E)=0 on the face we can change the first or second ghost point: 
      ! NOTE: for the mx/cmd/abc.cmd test of square128.order4, the errors in div(E) are 5 times smaller with projectDivLine=1
      !   *** thus just stick with this ***
      ! NOTE: projectDivLine=2 has not been fully tested
      projectDivLine=1
      if( projectDivLine.eq.1 )then
        ! set first ghost line
        divCoeff=-8./12. ! coeff of u(-1) in fourth order formula for div
      else if( projectDivLine.eq.2 )then
        ! set 2nd ghost line
        divCoeff= 1./12. ! coeff of u(-2) in fourth order formula for div
      end if
      if( debug.gt.1 )then
        write(*,'(" abcMaxwell: **START** grid=",i4," side,axis=",2i2," projectDivLine=",i2)') grid,side,axis,projectDivLine
      end if
     
      ! for plane wave forcing 
      twoPi=8.*atan2(1.,1.)
      cc= c*sqrt( kx*kx+ky*ky+kz*kz )

      epsX=1.e-30 ! fix this ***

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

      if( gridType.eq.curvilinear )then
        ! do this for testing:
        dx(0)=dr(0)
        dx(1)=dr(1)
        dx(2)=dr(2)
      end if

      dxa=dx(0) 
      dya=dx(1) 
      dza=dx(2) 

      if( gridType.eq.curvilinear )then
        ! On a curvilinear grid we need to make sure that there are valid values on the
        ! first ghost line -- these may be used on non-orthogonal grids (cross terms in uxx, uyy, ...)
        beginLoopOverSides(extra,numberOfGhostPoints)
          if( orderOfAccuracy.eq.2 )then

           ! ** we could also impose the first-order in time explicit formula **
           beginLoops()
            extrapLine1Order2(i1,i2,i3,is1,is2,is3,ex)
            extrapLine1Order2(i1,i2,i3,is1,is2,is3,ey)
            extrapLine1Order2(i1,i2,i3,is1,is2,is3,hz)
           endLoops()

          else if( orderOfAccuracy.eq.4 )then

           ! extrap to order 3 in case we adjust for incident fields 
           beginLoops()
            extrapLine1Order3(i1,i2,i3,is1,is2,is3,ex)
            extrapLine1Order3(i1,i2,i3,is1,is2,is3,ey)
            extrapLine1Order3(i1,i2,i3,is1,is2,is3,hz)
           endLoops()

          else
            stop 8822 ! unknown orderOfAccuracy
          end if

        endLoopOverSides()

      end if

      ! ------------------------------------------------------------------------
      ! ------------------Corners-----------------------------------------------
      ! ------------------------------------------------------------------------

      ! We need to assign points in the corner region:
      !
      !           |  |  |
      !           +--+--+--
      !           |  |  |
      !     D--A--X--+--+--
      !     |  |  |
      !     D--C--B
      !     |  |  |
      !     D--D--D
      ! 
      if( nd.eq.2 )then

       ! **** 2D ****

       i3=gridIndexRange(0,2)
       do side1=0,1
       do side2=0,1

        bc1=boundaryCondition(side1,0)
        bc2=boundaryCondition(side2,1)
        if( ((bc1.ge.abcEM2 .and. bc1.le.abc5) .and. bc2.gt.0 ) .or. \
            ((bc2.ge.abcEM2 .and. bc2.le.abc5) .and. bc1.gt.0 ) )then

          ! --- One of the faces at this corner is an ABC and the other has bc>0 ---         

          ! Adjacent side is also an ABC: 
          adjacentFaceIsABC = bc1.ge.abcEM2 .and. bc1.le.abc5 .and. \
                              bc2.ge.abcEM2 .and. bc2.le.abc5

          i1=gridIndexRange(side1,0) ! (i1,i2,i3)=corner point
          i2=gridIndexRange(side2,1)

          if( mask(i1,i2,i3).gt.0 )then ! *wdh* 090712

           ! write(*,'(" ABC:set corner: grid,side1,side2,i1,i2=",3i3,2i5)') grid,side1,side2,i1,i2
 
           ! --- start by extrapolating all points on the extended boundary and adjacent to the corner ---
           !* is1=1-2*side1
           !* is2=1-2*side2
           !* is3=0
           !* j3=0
           !* extrapolateGhost(ex,ey,hz,numberOfGhostPoints,numberOfGhostPoints,0)


           ! --- Assign point 'A' on the extended boundary ---
           if( adjacentFaceIsABC )then
             is1=1-2*side1
             is2=0
             is3=0
 
             ! Use first-order-in-time formula since it doesn't require other ghost point at new time (un)
             if( gridType.eq.rectangular )then
              un(i1-is1,i2-is2,i3-is3,ex)=ABCEM2Xa(i1,i2,i3,ex)
              un(i1-is1,i2-is2,i3-is3,ey)=ABCEM2Xa(i1,i2,i3,ey)
              un(i1-is1,i2-is2,i3-is3,hz)=ABCEM2Xa(i1,i2,i3,hz)
             else
 
              side=side1
              axis=0 
              abcSetup2d(i1,i2,i3,is1,is2,is3,side,axis)
              abc2d(i1,i2,i3,is1,is2,is3,side,axis,ex)
              abc2d(i1,i2,i3,is1,is2,is3,side,axis,ey)
              abc2d(i1,i2,i3,is1,is2,is3,side,axis,hz) 
 
             end if
           
             ! --- Assign point 'B'  on the extended boundary --
             is1=0
             is2=1-2*side2
 
             if( gridType.eq.rectangular )then
              un(i1-is1,i2-is2,i3-is3,ex)=ABCEM2Ya(i1,i2,i3,ex)
              un(i1-is1,i2-is2,i3-is3,ey)=ABCEM2Ya(i1,i2,i3,ey)
              un(i1-is1,i2-is2,i3-is3,hz)=ABCEM2Ya(i1,i2,i3,hz)
             else
   
              side=side2
              axis=1 
              abcSetup2d(i1,i2,i3,is1,is2,is3,side,axis)
              abc2d(i1,i2,i3,is1,is2,is3,side,axis,ex)
              abc2d(i1,i2,i3,is1,is2,is3,side,axis,ey)
              abc2d(i1,i2,i3,is1,is2,is3,side,axis,hz) 
             end if
           end if ! end if adjacentFaceIsABC
 
           ! --- Now extrapolate all other points on the extended boundary and adjacent to the corner ---
           is1=1-2*side1
           is2=1-2*side2
           is3=0
           j3=0
           extrapolateGhost(ex,ey,hz,numberOfGhostPoints,numberOfGhostPoints,0)

          end if ! mask 
         end if ! if one face is ABC
       end do          
       end do          

      else 

       ! ***** 3D *****

       beginEdgeLoops()
        if( ( (bc1.ge.abcEM2 .and. bc1.le.abc5) .and. bc2.gt.0 ) .or. \
            ( (bc2.ge.abcEM2 .and. bc2.le.abc5) .and. bc1.gt.0 ) )then

         ! --- One face is an ABC  and the other has bc>0 ---

         ! Adjacent side is also an ABC: 
         adjacentFaceIsABC = bc1.ge.abcEM2 .and. bc1.le.abc5 .and. \
                             bc2.ge.abcEM2 .and. bc2.le.abc5

         if( edgeDirection.eq.0 )then

          i2=n2a
          i3=n3a
          do i1=n1a,n1b
          if( mask(i1,i2,i3).gt.0 )then ! *wdh* 090712


            ! Use first-order-in-time formula since it doesn't require other ghost point at new time (un)
            if( adjacentFaceIsABC )then
             if( gridType.eq.rectangular )then
              un(i1,i2,i3-is3,ex)=ABCEM23DZa(i1,i2,i3,0,0,is3,ex)
              un(i1,i2,i3-is3,ey)=ABCEM23DZa(i1,i2,i3,0,0,is3,ey)
              un(i1,i2,i3-is3,ez)=ABCEM23DZa(i1,i2,i3,0,0,is3,ez)
           
              un(i1,i2-is2,i3,ex)=ABCEM23DYa(i1,i2,i3,0,is2,0,ex)
              un(i1,i2-is2,i3,ey)=ABCEM23DYa(i1,i2,i3,0,is2,0,ey)
              un(i1,i2-is2,i3,ez)=ABCEM23DYa(i1,i2,i3,0,is2,0,ez)
             else
 
              abcSetup3d(i1,i2,i3,0,0,is3,side3,2)
              abc3d(i1,i2,i3,0,0,is3,side3,2,ex)
              abc3d(i1,i2,i3,0,0,is3,side3,2,ey)
              abc3d(i1,i2,i3,0,0,is3,side3,2,ez) 
 
              abcSetup3d(i1,i2,i3,0,is2,0,side2,1)
              abc3d(i1,i2,i3,0,is2,0,side2,1,ex)
              abc3d(i1,i2,i3,0,is2,0,side2,1,ey)
              abc3d(i1,i2,i3,0,is2,0,side2,1,ez) 
 
             end if
            end if

            extrapolateGhost(ex,ey,ez,0,numberOfGhostPoints,numberOfGhostPoints)

          end if
          end do

         else if( edgeDirection.eq.1 )then

          i1=n1a
          i3=n3a
          do i2=n2a,n2b
          if( mask(i1,i2,i3).gt.0 )then ! *wdh* 090712

            if( adjacentFaceIsABC )then
              ! Use first-order-in-time formula since it doesn't require other ghost point at new time (un)
              if( gridType.eq.rectangular )then
               un(i1-is1,i2,i3,ex)=ABCEM23DXa(i1,i2,i3,is1,0,0,ex)
               un(i1-is1,i2,i3,ey)=ABCEM23DXa(i1,i2,i3,is1,0,0,ey)
               un(i1-is1,i2,i3,ez)=ABCEM23DXa(i1,i2,i3,is1,0,0,ez)
  
               un(i1,i2,i3-is3,ex)=ABCEM23DZa(i1,i2,i3,0,0,is3,ex)
               un(i1,i2,i3-is3,ey)=ABCEM23DZa(i1,i2,i3,0,0,is3,ey)
               un(i1,i2,i3-is3,ez)=ABCEM23DZa(i1,i2,i3,0,0,is3,ez)
  
              else
  
               abcSetup3d(i1,i2,i3,is1,0,0,side1,0)
               abc3d(i1,i2,i3,is1,0,0,side1,0,ex)
               abc3d(i1,i2,i3,is1,0,0,side1,0,ey)
               abc3d(i1,i2,i3,is1,0,0,side1,0,ez) 
  
               abcSetup3d(i1,i2,i3,0,0,is3,side3,2)
               abc3d(i1,i2,i3,0,0,is3,side3,2,ex)
               abc3d(i1,i2,i3,0,0,is3,side3,2,ey)
               abc3d(i1,i2,i3,0,0,is3,side3,2,ez) 
  
              end if
            end if          

            extrapolateGhost(ex,ey,ez,numberOfGhostPoints,0,numberOfGhostPoints)

          end if
          end do

         else if( edgeDirection.eq.2 )then
          ! write(*,'(" ABC:set corner: grid,side1,side2,i1,i2=",3i3,2i5)') grid,side1,side2,i1,i2
          i1=n1a
          i2=n2a
          do i3=n3a,n3b
          if( mask(i1,i2,i3).gt.0 )then ! *wdh* 090712

            if( adjacentFaceIsABC )then
              ! Use first-order-in-time formula since it doesn't require other ghost point at new time (un)
              if( gridType.eq.rectangular )then
               un(i1-is1,i2,i3,ex)=ABCEM23DXa(i1,i2,i3,is1,0,0,ex)
               un(i1-is1,i2,i3,ey)=ABCEM23DXa(i1,i2,i3,is1,0,0,ey)
               un(i1-is1,i2,i3,ez)=ABCEM23DXa(i1,i2,i3,is1,0,0,ez)
            
               un(i1,i2-is2,i3,ex)=ABCEM23DYa(i1,i2,i3,0,is2,0,ex)
               un(i1,i2-is2,i3,ey)=ABCEM23DYa(i1,i2,i3,0,is2,0,ey)
               un(i1,i2-is2,i3,ez)=ABCEM23DYa(i1,i2,i3,0,is2,0,ez)
  
              else
  
               abcSetup3d(i1,i2,i3,is1,0,0,side1,0)
               abc3d(i1,i2,i3,is1,0,0,side1,0,ex)
               abc3d(i1,i2,i3,is1,0,0,side1,0,ey)
               abc3d(i1,i2,i3,is1,0,0,side1,0,ez) 
  
               abcSetup3d(i1,i2,i3,0,is2,0,side2,1)
               abc3d(i1,i2,i3,0,is2,0,side2,1,ex)
               abc3d(i1,i2,i3,0,is2,0,side2,1,ey)
               abc3d(i1,i2,i3,0,is2,0,side2,1,ez) 
  
              end if
            end if

            extrapolateGhost(ex,ey,ez,numberOfGhostPoints,numberOfGhostPoints,0)

          end if
          end do

         end if ! end if edgeDirection
        end if ! bc
       endEdgeLoops()

       ! ***** vertices in 3D  *****
       if( .false. )then
         ! *** old way ***
         !  normal-direction:     u -> +u
         !  tangential-direction  u -> -u
         !     u(-1,-1,-1) = +u(1,1,1)
         do side3=0,1
         do side2=0,1
         do side1=0,1
          bc1=boundaryCondition(side1,0)
          bc2=boundaryCondition(side2,1)
          bc3=boundaryCondition(side3,2)
          if( bc1.ge.abcEM2 .and. bc1.le.abc5 .and. \
              bc2.ge.abcEM2 .and. bc2.le.abc5 .and. \
              bc3.ge.abcEM2 .and. bc3.le.abc5 )then
           i1=gridIndexRange(side1,0)
           i2=gridIndexRange(side2,1)
           i3=gridIndexRange(side3,2)
           if( mask(i1,i2,i3).gt.0 )then ! *wdh* 090712
  
            is1=1-2*side1
            is2=1-2*side2
            is3=1-2*side3
  
            un(i1-is1,i2-is2,i3-is3,ex)=un(i1+is1,i2+is2,i3+is3,ex)
            un(i1-is1,i2-is2,i3-is3,ey)=un(i1+is1,i2+is2,i3+is3,ey)
            un(i1-is1,i2-is2,i3-is3,ez)=un(i1+is1,i2+is2,i3+is3,ez)
           end if ! mask
          end if
         end do
         end do
         end do
       else
         ! **new way** 090718 

         do side3=0,1
         do side2=0,1
         do side1=0,1
          bc1=boundaryCondition(side1,0)
          bc2=boundaryCondition(side2,1)
          bc3=boundaryCondition(side3,2)
          if( ( (bc1.ge.abcEM2 .and. bc1.le.abc5) .or. \
                (bc2.ge.abcEM2 .and. bc2.le.abc5) .or. \
                (bc3.ge.abcEM2 .and. bc3.le.abc5) ) .and.  \
                ( bc1.gt.0 .and. bc2.gt.0 .and. bc3.gt.0 ) )then
           ! Three physical faces meet at this corner and at least one face is an ABC
           i1=gridIndexRange(side1,0)
           i2=gridIndexRange(side2,1)
           i3=gridIndexRange(side3,2)
           if( mask(i1,i2,i3).gt.0 )then 
  
            is1=1-2*side1
            is2=1-2*side2
            is3=1-2*side3

            extrapolateGhost(ex,ey,ez,numberOfGhostPoints,numberOfGhostPoints,numberOfGhostPoints)

           end if ! mask
          end if
         end do
         end do
         end do
       end if

      end if




      ! -------------------------------------------------------------------------
      ! ------------------Loop over Sides----------------------------------------
      ! -------------------------------------------------------------------------
      beginLoopOverSides(extra,numberOfGhostPoints)


       if( gridType.eq.rectangular .and. orderOfAccuracy.eq.2 )then
        ! ***********************************************
        ! ************rectangular grid*******************
        ! ***********************************************

       
        ! write(*,'(" Apply abcEM2: grid,side,axis=",3i3," dt,c=",2e12.3," is1,is2=",2i2)') grid,side,axis,dt,c,is1,is2
        if( nd.eq.2 )then
         if( axis.eq.0 )then
          beginLoopsMask()
       
           ! un(i1-is1,i2-is2,i3-is3,ex)=ABCEM2X(i1,i2,i3,ex)
           un(i1-is1,i2-is2,i3-is3,ey)=ABCEM2X(i1,i2,i3,ey)
           un(i1-is1,i2-is2,i3-is3,hz)=ABCEM2X(i1,i2,i3,hz)
       
           ! set div(E)=0 *wdh* 090712
           ! (u(i+1)-u(i-1))/(2*dx) + vy = 0 
           un(i1-is1,i2,i3,ex)=un(i1+is1,i2,i3,ex)+ 2.*is1*dx(axis)*uny22r(i1,i2,i3,ey)

           ! write(1,'("t=",e10.2,", i1,i2=",2i3," u(-1),v(-1),ux,vy,ux+vy=",5e10.2)') t,i1,i2,un(i1-is1,i2,i3,ex),un(i1-is1,i2-is2,i3-is3,ey),unx22r(i1,i2,i3,ex),uny22r(i1,i2,i3,ey), unx22r(i1,i2,i3,ex)+uny22r(i1,i2,i3,ey)

          endLoopsMask()
         else if( axis.eq.1 )then
          beginLoopsMask()
           un(i1-is1,i2-is2,i3-is3,ex)=ABCEM2Y(i1,i2,i3,ex)
           ! un(i1-is1,i2-is2,i3-is3,ey)=ABCEM2Y(i1,i2,i3,ey)
           un(i1-is1,i2-is2,i3-is3,hz)=ABCEM2Y(i1,i2,i3,hz)

           ! set div(E)=0 *wdh* 090712
           un(i1,i2-is2,i3,ey)=un(i1,i2+is2,i3,ey) + 2.*is2*dx(axis)*unx22r(i1,i2,i3,ex)

          endLoopsMask()
         else
          stop 9467
         end if

        else ! ***** 3D *****
         if( axis.eq.0 )then
          beginLoopsMask()
           ! un(i1-is1,i2-is2,i3-is3,ex)=ABCEM23DX(i1,i2,i3,ex)
           un(i1-is1,i2-is2,i3-is3,ey)=ABCEM23DX(i1,i2,i3,ey)
           un(i1-is1,i2-is2,i3-is3,ez)=ABCEM23DX(i1,i2,i3,ez)

           ! set div(E)=0 *wdh* 090712
           un(i1-is1,i2,i3,ex)=un(i1+is1,i2,i3,ex)+ 2.*is1*dx(axis)*(uny23r(i1,i2,i3,ey)+unz23r(i1,i2,i3,ez))

          endLoopsMask()
         else if( axis.eq.1 )then
          beginLoopsMask()
           un(i1-is1,i2-is2,i3-is3,ex)=ABCEM23DY(i1,i2,i3,ex)
           ! un(i1-is1,i2-is2,i3-is3,ey)=ABCEM23DY(i1,i2,i3,ey)
           un(i1-is1,i2-is2,i3-is3,ez)=ABCEM23DY(i1,i2,i3,ez)

           ! set div(E)=0 *wdh* 090712
           un(i1,i2-is2,i3,ey)=un(i1,i2+is2,i3,ey) + 2.*is2*dx(axis)*(unx22r(i1,i2,i3,ex)+unz23r(i1,i2,i3,ez))

          endLoopsMask()
         else if( axis.eq.2 )then
          beginLoopsMask()
           un(i1-is1,i2-is2,i3-is3,ex)=ABCEM23DZ(i1,i2,i3,ex)
           un(i1-is1,i2-is2,i3-is3,ey)=ABCEM23DZ(i1,i2,i3,ey)
           ! un(i1-is1,i2-is2,i3-is3,ez)=ABCEM23DZ(i1,i2,i3,ez)

           ! set div(E)=0 *wdh* 090712
           un(i1,i2,i3-is3,ez)=un(i1,i2,i3+is3,ez) + 2.*is3*dx(axis)*(unx22r(i1,i2,i3,ex)+uny23r(i1,i2,i3,ey))

          endLoopsMask()
         else
          stop 9468
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
          beginLoopsMask()
       
           un(i1-is1,i2-is2,i3-is3,ex)=ABCEM2X(i1,i2,i3,ex)
           un(i1-is1,i2-is2,i3-is3,ey)=ABCEM2X(i1,i2,i3,ey)
           un(i1-is1,i2-is2,i3-is3,hz)=ABCEM2X(i1,i2,i3,hz)
       
           extrapLine2Order4(i1,i2,i3,is1,is2,is3,ex)
           extrapLine2Order4(i1,i2,i3,is1,is2,is3,ey)
           extrapLine2Order4(i1,i2,i3,is1,is2,is3,hz)

           ! set div(E)=0 *wdh* 090712
           ! (-u(i+2) +8*(u(i+1)-u(i-1)) + u(i-2))/(12*dx) + vy = 0 

           if( projectDivLine.eq.1 )then
             ! set first ghost line
             un(i1-is1,i2,i3,ex)=(-un(i1+2*is1,i2,i3,ex)+8.*un(i1+is1,i2,i3,ex)+un(i1-2*is1,i2,i3,ex))/8. \
                                  + 1.5*is1*dx(axis)*uny42r(i1,i2,i3,ey)
           else if( projectDivLine.eq.2 )then
            ! change second ghost:
             un(i1-2*is1,i2,i3,ex)=(un(i1+2*is1,i2,i3,ex)-8.*un(i1+is1,i2,i3,ex)+8.*un(i1-is1,i2,i3,ex)) \
                                 - 12.*is1*dx(axis)*uny42r(i1,i2,i3,ey)
           end if
           ! write(1,'("t=",e10.2,", i1,i2=",2i3," div42d=",e10.2)') t,i1,i2,unx42r(i1,i2,i3,ex)+uny42r(i1,i2,i3,ey)
          endLoopsMask()
         else if( axis.eq.1 )then
          beginLoopsMask()
           un(i1-is1,i2-is2,i3-is3,ex)=ABCEM2Y(i1,i2,i3,ex)
           un(i1-is1,i2-is2,i3-is3,ey)=ABCEM2Y(i1,i2,i3,ey)
           un(i1-is1,i2-is2,i3-is3,hz)=ABCEM2Y(i1,i2,i3,hz)

           extrapLine2Order4(i1,i2,i3,is1,is2,is3,ex)
           extrapLine2Order4(i1,i2,i3,is1,is2,is3,ey)
           extrapLine2Order4(i1,i2,i3,is1,is2,is3,hz)

           ! set div(E)=0 *wdh* 090712
           if( projectDivLine.eq.1 )then
             ! set first ghost line
             un(i1,i2-is2,i3,ey)=(-un(i1,i2+2*is2,i3,ey)+8.*un(i1,i2+is2,i3,ey)+un(i1,i2-2*is2,i3,ey))/8. \
                                 + 1.5*is2*dx(axis)*unx42r(i1,i2,i3,ex)
           else if( projectDivLine.eq.2 )then
            ! change second ghost:
             un(i1,i2-2*is2,i3,ey)=(un(i1,i2+2*is2,i3,ey)-8.*un(i1,i2+is2,i3,ey)+8.*un(i1,i2-is2,i3,ey)) \
                                 - 12.*is2*dx(axis)*unx42r(i1,i2,i3,ex)
           end if
           ! write(1,'("t=",e10.2,", i1,i2=",2i3," div42d=",e10.2)') t,i1,i2,unx42r(i1,i2,i3,ex)+uny42r(i1,i2,i3,ey)
          endLoopsMask()
         else
          stop 9469
         end if

        else ! ***** 3D *****
         if( axis.eq.0 )then
          beginLoopsMask()
           un(i1-is1,i2-is2,i3-is3,ex)=ABCEM23DX(i1,i2,i3,ex)
           un(i1-is1,i2-is2,i3-is3,ey)=ABCEM23DX(i1,i2,i3,ey)
           un(i1-is1,i2-is2,i3-is3,ez)=ABCEM23DX(i1,i2,i3,ez)

           extrapLine2Order4(i1,i2,i3,is1,is2,is3,ex)
           extrapLine2Order4(i1,i2,i3,is1,is2,is3,ey)
           extrapLine2Order4(i1,i2,i3,is1,is2,is3,ez)

           ! set div(E)=0 *wdh* 090712
           if( projectDivLine.eq.1 )then
             ! set first ghost line
             un(i1-is1,i2,i3,ex)=(-un(i1+2*is1,i2,i3,ex)+8.*un(i1+is1,i2,i3,ex)+un(i1-2*is1,i2,i3,ex))/8. \
                                 + 1.5*is1*dx(axis)*(uny43r(i1,i2,i3,ey)+unz43r(i1,i2,i3,ez))
           else if( projectDivLine.eq.2 )then
            ! change second ghost:
             un(i1-2*is1,i2,i3,ex)=(un(i1+2*is1,i2,i3,ex)-8.*un(i1+is1,i2,i3,ex)+8.*un(i1-is1,i2,i3,ex)) \
                                 - 12.*is1*dx(axis)*(uny43r(i1,i2,i3,ey)+unz43r(i1,i2,i3,ez))
           end if

           ! write(1,'("t=",e10.2,", i1,i2,i3=",3i3," div43d=",e10.2)') t,i1,i2,i3,unx43r(i1,i2,i3,ex)+uny43r(i1,i2,i3,ey)+unz43r(i1,i2,i3,ez)

          endLoopsMask()
         else if( axis.eq.1 )then
          beginLoopsMask()
           un(i1-is1,i2-is2,i3-is3,ex)=ABCEM23DY(i1,i2,i3,ex)
           un(i1-is1,i2-is2,i3-is3,ey)=ABCEM23DY(i1,i2,i3,ey)
           un(i1-is1,i2-is2,i3-is3,ez)=ABCEM23DY(i1,i2,i3,ez)

           extrapLine2Order4(i1,i2,i3,is1,is2,is3,ex)
           extrapLine2Order4(i1,i2,i3,is1,is2,is3,ey)
           extrapLine2Order4(i1,i2,i3,is1,is2,is3,ez)

           ! set div(E)=0 *wdh* 090712
           if( projectDivLine.eq.1 )then
             ! set first ghost line
             un(i1,i2-is2,i3,ey)=(-un(i1,i2+2*is2,i3,ey)+8.*un(i1,i2+is2,i3,ey)+un(i1,i2-2*is2,i3,ey))/8. \
                                 + 1.5*is2*dx(axis)*(unx43r(i1,i2,i3,ex)+unz43r(i1,i2,i3,ez))
           else if( projectDivLine.eq.2 )then
            ! change second ghost:
             un(i1,i2-2*is2,i3,ey)=(un(i1,i2+2*is2,i3,ey)-8.*un(i1,i2+is2,i3,ey)+8.*un(i1,i2-is2,i3,ey)) \
                                 - 12.*is2*dx(axis)*(unx43r(i1,i2,i3,ex)+unz43r(i1,i2,i3,ez))
           end if

           ! write(1,'("t=",e10.2,", i1,i2,i3=",3i3," div43d=",e10.2)') t,i1,i2,i3,unx43r(i1,i2,i3,ex)+uny43r(i1,i2,i3,ey)+unz43r(i1,i2,i3,ez)

          endLoopsMask()
         else if( axis.eq.2 )then
          beginLoopsMask()
           un(i1-is1,i2-is2,i3-is3,ex)=ABCEM23DZ(i1,i2,i3,ex)
           un(i1-is1,i2-is2,i3-is3,ey)=ABCEM23DZ(i1,i2,i3,ey)
           un(i1-is1,i2-is2,i3-is3,ez)=ABCEM23DZ(i1,i2,i3,ez)

           extrapLine2Order4(i1,i2,i3,is1,is2,is3,ex)
           extrapLine2Order4(i1,i2,i3,is1,is2,is3,ey)
           extrapLine2Order4(i1,i2,i3,is1,is2,is3,ez)

           ! set div(E)=0 *wdh* 090712
           if( projectDivLine.eq.1 )then
             ! set first ghost line
             un(i1,i2,i3-is3,ez)=(-un(i1,i2,i3+2*is3,ez)+8.*un(i1,i2,i3+is3,ez)+un(i1,i2,i3-2*is3,ez))/8. \
                               + 1.5*is3*dx(axis)*(unx43r(i1,i2,i3,ex)+uny43r(i1,i2,i3,ey))
           else if( projectDivLine.eq.2 )then
            ! change second ghost:
             un(i1,i2,i3-2*is3,ez)=(un(i1,i2,i3+2*is3,ez)-8.*un(i1,i2,i3+is3,ez)+8.*un(i1,i2,i3-is3,ez)) \
                               - 12.*is3*dx(axis)*(unx43r(i1,i2,i3,ex)+uny43r(i1,i2,i3,ey))
           end if

          ! write(1,'("t=",e10.2,", i1,i2,i3=",3i3," div43d=",e10.2)') t,i1,i2,i3,unx43r(i1,i2,i3,ex)+uny43r(i1,i2,i3,ey)+unz43r(i1,i2,i3,ez)

          endLoopsMask()
         else
          stop 9470
         end if

        end if
     
       else if( gridType.eq.curvilinear .and. orderOfAccuracy.eq.2 )then
        ! ***********************************************
        ! ************curvilinear grid*******************
        ! ***********************************************

        ! --- this really assumes that the boundary is not curved : do this for now ----

        ! write(*,'(" Apply abcEM2: grid,side,axis=",3i3," dt,c=",2e12.3)') grid,side,axis,dt,c
        if( nd.eq.2 )then
          beginLoopsMask()

           abcSetup2d(i1,i2,i3,is1,is2,is3,side,axis)
           abc2d(i1,i2,i3,is1,is2,is3,side,axis,ex)
           abc2d(i1,i2,i3,is1,is2,is3,side,axis,ey)
           abc2d(i1,i2,i3,is1,is2,is3,side,axis,hz) 
           
           ! ----------------------------
           ! set div(E)=0 *wdh* 090712
           ! ux = rx*ur + sx*us 
           ! vy = ry*ur + sy*us
           ! div = ax*un(-1) + ay*vn(-1) + ( div - a.uv(-1) )
           !   ax = rx/(2dr) : coeff of u(-1)
           !   ay = ry/(2dr) : coeff of v(-1)
           ! project (u(-1),v(-1)) so that
           !   av.uvn = f = a.uv - div 
           !   uvn = uv + (f - av.uv) av/aSq
           !   uvn = uv + (-div) av/aSq
           div = unx22(i1,i2,i3,ex)+uny22(i1,i2,i3,ey)
           ax = -is*rsxy(i1,i2,i3,axis,0)/(2.*dr(axis))
           ay = -is*rsxy(i1,i2,i3,axis,1)/(2.*dr(axis))
           aSq = max( epsX, ax**2 + ay**2)
           un(i1-is1,i2-is2,i3-is3,ex) = un(i1-is1,i2-is2,i3-is3,ex) - div*ax/aSq
           un(i1-is1,i2-is2,i3-is3,ey) = un(i1-is1,i2-is2,i3-is3,ey) - div*ay/aSq

           ! write(1,'("t=",e10.2,", i1,i2=",2i3," ux,vy,ux+vy=",5e10.2)') t,i1,i2,unx22(i1,i2,i3,ex),uny22(i1,i2,i3,ey), unx22(i1,i2,i3,ex)+uny22(i1,i2,i3,ey)           

           ! if( debug.gt.0 )then 
           !  if( axis.eq.0 )then
           !   t0=ABCEM2X(i1,i2,i3,ex)
           !   t1=ABCEM2X(i1,i2,i3,ey)
           !   t2=ABCEM2X(i1,i2,i3,hz)
           !  else
           !   t0=ABCEM2Y(i1,i2,i3,ex)
           !   t1=ABCEM2Y(i1,i2,i3,ey)
           !   t2=ABCEM2Y(i1,i2,i3,hz)
           !  end if

           ! write(*,'(" abc: i=",2i3," ex,t0, ey,t1, hz,t2=",3(2e10.2,1x))') i1,i2,un(i1-is1,i2-is2,i3-is3,ex),\
           !   t0,un(i1-is1,i2-is2,i3-is3,ey),t1,un(i1-is1,i2-is2,i3-is3,hz),t2
           !   ! ' 
           ! end if


          endLoopsMask()

        else ! ***** 3D *****

          beginLoopsMask()
           abcSetup3d(i1,i2,i3,is1,is2,is3,side,axis)
           abc3d(i1,i2,i3,is1,is2,is3,side,axis,ex)
           abc3d(i1,i2,i3,is1,is2,is3,side,axis,ey)
           abc3d(i1,i2,i3,is1,is2,is3,side,axis,ez) 

           ! ----------------------------
           ! set div(E)=0 *wdh* 090712
           div = unx23(i1,i2,i3,ex)+uny23(i1,i2,i3,ey)+unz23(i1,i2,i3,ez)
           ax = -is*rsxy(i1,i2,i3,axis,0)/(2.*dr(axis))
           ay = -is*rsxy(i1,i2,i3,axis,1)/(2.*dr(axis))
           az = -is*rsxy(i1,i2,i3,axis,2)/(2.*dr(axis))
           aSq = max( epsX, ax**2 + ay**2+ az**2)
           un(i1-is1,i2-is2,i3-is3,ex) = un(i1-is1,i2-is2,i3-is3,ex) - div*ax/aSq
           un(i1-is1,i2-is2,i3-is3,ey) = un(i1-is1,i2-is2,i3-is3,ey) - div*ay/aSq
           un(i1-is1,i2-is2,i3-is3,ez) = un(i1-is1,i2-is2,i3-is3,ez) - div*az/aSq

           ! write(1,'("t=",e10.2,", i1,i2,i3=",3i3," div=",e10.2)') t,i1,i2,i3,unx23(i1,i2,i3,ex)+uny23(i1,i2,i3,ey)+unz23(i1,i2,i3,ez)

           ! if( debug.gt.0 )then 
           !  if( axis.eq.0 )then
           !   t0=ABCEM23DX(i1,i2,i3,ex)
           !   t1=ABCEM23DX(i1,i2,i3,ey)
           !   t2=ABCEM23DX(i1,i2,i3,ez)
           !  else if( axis.eq.1 )then
           !   t0=ABCEM23DY(i1,i2,i3,ex)
           !   t1=ABCEM23DY(i1,i2,i3,ey)
           !   t2=ABCEM23DY(i1,i2,i3,ez)
           !  else 
           !   t0=ABCEM23DZ(i1,i2,i3,ex)
           !   t1=ABCEM23DZ(i1,i2,i3,ey)
           !   t2=ABCEM23DZ(i1,i2,i3,ez)
           !  end if
           ! 
           ! write(*,'(" abc: i=",3i3," ex,t0, ey,t1, ez,t2=",3(2e10.2,1x))') i1,i2,i3,un(i1-is1,i2-is2,i3-is3,ex),\
           !   t0,un(i1-is1,i2-is2,i3-is3,ey),t1,un(i1-is1,i2-is2,i3-is3,ez),t2
           !   ! ' 
           ! end if

          endLoopsMask()

        end if
     

       else if( gridType.eq.curvilinear .and. orderOfAccuracy.eq.4 )then
        ! ***********************************************
        ! ************curvilinear grid*******************
        ! ***********************************************

       
         ! >>>>>>>>>>>>>>>> this is only second-order ---- fix this <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<       

        ! write(*,'(" Apply abcEM2: grid,side,axis=",3i3," dt,c=",2e12.3)') grid,side,axis,dt,c
        if( nd.eq.2 )then
          beginLoopsMask()
       
           abcSetup2d(i1,i2,i3,is1,is2,is3,side,axis)
           abc2d(i1,i2,i3,is1,is2,is3,side,axis,ex)
           abc2d(i1,i2,i3,is1,is2,is3,side,axis,ey)
           abc2d(i1,i2,i3,is1,is2,is3,side,axis,hz) 
       
           extrapLine2Order4(i1,i2,i3,is1,is2,is3,ex)
           extrapLine2Order4(i1,i2,i3,is1,is2,is3,ey)
           extrapLine2Order4(i1,i2,i3,is1,is2,is3,hz)

           ! ----------------------------
           ! set div(E)=0 *wdh* 090712
           ! (-u(i+2) +8*(u(i+1)-u(i-1)) + u(i-2))/(12*dx) + vy = 0 
           ! try this: set first ghost line
           div = unx42(i1,i2,i3,ex)+uny42(i1,i2,i3,ey)
           !ax = -is*rsxy(i1,i2,i3,axis,0)/(1.5*dr(axis))  ! coeff of u(-1)  8/12 = 1/(1.5)
           !ay = -is*rsxy(i1,i2,i3,axis,1)/(1.5*dr(axis))
           ax = divCoeff*is*rsxy(i1,i2,i3,axis,0)/dr(axis)  ! coeff of u(-1) or u(-2)
           ay = divCoeff*is*rsxy(i1,i2,i3,axis,1)/dr(axis)
           aSq = max( epsX, ax**2 + ay**2)
           if( projectDivLine.eq.1 )then
             ! set first ghost line
             un(i1-is1,i2-is2,i3-is3,ex) = un(i1-is1,i2-is2,i3-is3,ex) - div*ax/aSq
             un(i1-is1,i2-is2,i3-is3,ey) = un(i1-is1,i2-is2,i3-is3,ey) - div*ay/aSq
           else if( projectDivLine.eq.2 )then
             ! set 2nd ghost line
             un(i1-2*is1,i2-2*is2,i3-2*is3,ex) = un(i1-2*is1,i2-2*is2,i3-2*is3,ex) - div*ax/aSq
             un(i1-2*is1,i2-2*is2,i3-2*is3,ey) = un(i1-2*is1,i2-2*is2,i3-2*is3,ey) - div*ay/aSq
           end if
           ! write(1,'("t=",e10.2,", i1,i2=",2i3," div42dc=",e10.2)') t,i1,i2,unx42(i1,i2,i3,ex)+uny42(i1,i2,i3,ey)


          endLoopsMask()

        else ! ***** 3D *****

          beginLoopsMask()
           abcSetup3d(i1,i2,i3,is1,is2,is3,side,axis)
           abc3d(i1,i2,i3,is1,is2,is3,side,axis,ex)
           abc3d(i1,i2,i3,is1,is2,is3,side,axis,ey)
           abc3d(i1,i2,i3,is1,is2,is3,side,axis,ez) 

           extrapLine2Order4(i1,i2,i3,is1,is2,is3,ex)
           extrapLine2Order4(i1,i2,i3,is1,is2,is3,ey)
           extrapLine2Order4(i1,i2,i3,is1,is2,is3,ez)

           ! ----------------------------
           ! set div(E)=0 *wdh* 090712
           div = unx43(i1,i2,i3,ex)+uny43(i1,i2,i3,ey)+unz43(i1,i2,i3,ez)
           ! ax = -is*rsxy(i1,i2,i3,axis,0)/(1.5*dr(axis))
           ! ay = -is*rsxy(i1,i2,i3,axis,1)/(1.5*dr(axis))
           ! az = -is*rsxy(i1,i2,i3,axis,2)/(1.5*dr(axis))
           ax = divCoeff*is*rsxy(i1,i2,i3,axis,0)/dr(axis)
           ay = divCoeff*is*rsxy(i1,i2,i3,axis,1)/dr(axis)
           az = divCoeff*is*rsxy(i1,i2,i3,axis,2)/dr(axis)
           aSq = max( epsX, ax**2 + ay**2+ az**2)
           if( projectDivLine.eq.1 )then
             un(i1-is1,i2-is2,i3-is3,ex) = un(i1-is1,i2-is2,i3-is3,ex) - div*ax/aSq
             un(i1-is1,i2-is2,i3-is3,ey) = un(i1-is1,i2-is2,i3-is3,ey) - div*ay/aSq
             un(i1-is1,i2-is2,i3-is3,ez) = un(i1-is1,i2-is2,i3-is3,ez) - div*az/aSq
           else if( projectDivLine.eq.2 )then
             ! set 2nd ghost line
             un(i1-2*is1,i2-2*is2,i3-2*is3,ex) = un(i1-2*is1,i2-2*is2,i3-2*is3,ex) - div*ax/aSq
             un(i1-2*is1,i2-2*is2,i3-2*is3,ey) = un(i1-2*is1,i2-2*is2,i3-2*is3,ey) - div*ay/aSq
             un(i1-2*is1,i2-2*is2,i3-2*is3,ez) = un(i1-2*is1,i2-2*is2,i3-2*is3,ez) - div*az/aSq
           end if

           ! write(1,'("t=",e10.2,", i1,i2,i3=",3i3," div43dc=",e10.2)') t,i1,i2,i3,unx43(i1,i2,i3,ex)+uny43(i1,i2,i3,ey)+unz43(i1,i2,i3,ez)

          endLoopsMask()

        end if
     



       else
         write(*,'(" ABC: ERROR gridType=",i2," orderOfAccuracy=",i4," unexpected")') gridType,orderOfAccuracy

         stop 2255
       end if

      endLoopOverSides()

      return
      end
