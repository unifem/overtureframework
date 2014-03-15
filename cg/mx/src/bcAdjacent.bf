c *******************************************************************************
c   Assign boundary conditions on adjacent faces
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
     u(j1,j2,j3,ex)=extrap2(u,j1+js1,j2+js2,j3+js3,ex,js1,js2,js3)
     u(j1,j2,j3,ey)=extrap2(u,j1+js1,j2+js2,j3+js3,ey,js1,js2,js3)
     u(j1,j2,j3,ez)=extrap2(u,j1+js1,j2+js2,j3+js3,ez,js1,js2,js3)
   else                                                                           
     ! extrap first line to a max of order 3 in case we adjust for incident fields 
     if( m1.le.1 .and. m2.le.1 .and. m3.le.1 )then
       u(j1,j2,j3,ex)=extrap3(u,j1+js1,j2+js2,j3+js3,ex,js1,js2,js3)
       u(j1,j2,j3,ey)=extrap3(u,j1+js1,j2+js2,j3+js3,ey,js1,js2,js3)
       u(j1,j2,j3,ez)=extrap3(u,j1+js1,j2+js2,j3+js3,ez,js1,js2,js3)
     else
       ! 2nd-ghost line 
       u(j1,j2,j3,ex)=extrap4(u,j1+js1,j2+js2,j3+js3,ex,js1,js2,js3)
       u(j1,j2,j3,ey)=extrap4(u,j1+js1,j2+js2,j3+js3,ey,js1,js2,js3)
       u(j1,j2,j3,ez)=extrap4(u,j1+js1,j2+js2,j3+js3,ez,js1,js2,js3)
     end if
  end if
  end if
 end do
 end do
 end do
#endMacro

! ============================================================================
! Macro to set a symmetry BC on corner ghost points
! ============================================================================
#beginMacro bcSymmetryGhost(ex,ey,ez,numGhost1,numGhost2,numGhost3)

 do m3=m3a,numGhost3
 do m2=m2a,numGhost2
 do m1=m1a,numGhost1
   ! assign ghost point (j1,j2,j3) ... 
   j1=i1-is1*m1
   j2=i2-is2*m2
   j3=i3-is3*m3
   !  from symmetry point (k1,k2,k3)
   k1=i1+ks1*m1
   k2=i2+ks2*m2
   k3=i3+ks3*m3
   ! 
   u(j1,j2,j3,ex)=u(k1,k2,k3,ex)
   u(j1,j2,j3,ey)=u(k1,k2,k3,ey)
   u(j1,j2,j3,ez)=u(k1,k2,k3,ez)

   ! write(*,'(" bcSymmetryGhost: set pt (j1,j2,j3)=",3i4," equal to pt (k1,k2,k3)=",3i4)') j1,j2,j3,k1,k2,k3

 end do
 end do
 end do
#endMacro


#beginMacro beginEdgeLoops()
 do direction=0,1 ! loop over tangential directions to the face (side,axis)

   ! The edge is formed from faces (side,axis) and (sidep,axisp)
   axisp = mod( axis+direction+1,nd)  ! tangential axis

   edgeDirection = mod( axis+2-direction,nd)   ! tangential direction to the edge 

   do sidep=0,1   ! left and right faces 

     bcp = boundaryCondition(sidep,axisp )
     if( bcp.eq.symmetryBoundaryCondition .or. (bcp.ge.abcEM2 .and. bcp.le.abc5) )then
       ! adjacent face is a symmetry or ABC 

       ! symmetry BC sets: ghost pts:  u(-is1,-is2,-is3) = u(ks1,ks2,ks3) 
       is1=0
       is2=0
       is3=0
       ks1=0
       ks2=0
       ks3=0
       ! assign extended boundaries so that we get corners: 
       extra=numberOfGhostPoints
       n1a=gridIndexRange(0,0)-extra
       n1b=gridIndexRange(1,0)+extra
       n2a=gridIndexRange(0,1)-extra
       n2b=gridIndexRange(1,1)+extra
       n3a=gridIndexRange(0,2)-extra
       n3b=gridIndexRange(1,2)+extra
       if( axis.eq.0 )then
         is1=1-2*side
         ks1=-is1
         n1a=gridIndexRange(side,axis)
         n1b=n1a
       else if( axis.eq.1 )then
         is2=1-2*side
         ks2=-is2
         n2a=gridIndexRange(side,axis)
         n2b=n2a
       else
         is3=1-2*side
         ks3=-is3
         n3a=gridIndexRange(side,axis)
         n3b=n3a
       end if
       ! We reflect about axisp: 
       if( axisp.eq.0 )then
         is1=1-2*sidep
         ks1=is1
         n1a=gridIndexRange(sidep,axisp)
         n1b=n1a
       else if( axisp.eq.1 )then
         is2=1-2*sidep
         ks2=is2
         n2a=gridIndexRange(sidep,axisp)
         n2b=n2a
       else
         is3=1-2*sidep
         ks3=is3
         n3a=gridIndexRange(sidep,axisp)
         n3b=n3a
       end if

       numGhost1=numberOfGhostPoints
       numGhost2=numberOfGhostPoints
       numGhost3=numberOfGhostPoints
       if( edgeDirection.eq.0 )then
        numGhost1=1
       else if(edgeDirection.eq.1 )then 
        numGhost2=1
       else
        numGhost3=1
       end if
#endMacro ! edgeLoops
! 
#beginMacro endEdgeLoops()
  end if ! bcp 
end do ! sidep
end do ! direction
#endMacro

      subroutine bcAdjacent( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
                               gridIndexRange, u, up, mask,rsxy, xy,\
                               boundaryCondition, ipar, rpar, ierr )
! ===================================================================================
!  Assign boundary conditions on adjacent faces
!
!  gridType : 0=rectangular, 1=curvilinear
!  useForcing : 1=use f for RHS to BC
!  side,axis : 0:1 and 0:2
!
!  up : solution at time t-dt
!  u : solution at time t (apply BC to this solution)
!
! ===================================================================================

      implicit none

      integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,\
              n1a,n1b,n2a,n2b,n3a,n3b, ndc, ierr

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      real up(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
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
      integer i1,i2,i3,is1,is2,is3,js1,js2,js3,ks1,ks2,ks3,is,j1,j2,j3,k1,k2,k3,m1,m2,m3,mSum
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

      real ax,ay,az,aSq,div

      integer bc,bcp1,bcp2, axisp1,axisp2,sidev(0:2), side1a,side1b,side2a,side2b,side3a,side3b
      integer m1a,m2a,m3a, sidep,axisp, bcp, direction, numGhost1, numGhost2, numGhost3

      logical adjacentFaceIsABC

      ! boundary conditions parameters
      #Include "bcDefineFortranInclude.h"

      integer rectangular,curvilinear
      parameter(\
        rectangular=0,\
        curvilinear=1)

!     --- start statement function ----
      integer kd,m,n
      real rx,ry,rz,sx,sy,sz,tx,ty,tz
      declareDifferenceOrder2(u,RX)
!*       declareDifferenceOrder2(un,none)

      declareDifferenceOrder4(u,RX)
!*       declareDifferenceOrder4(un,none)
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


!*     The next macro call will define the difference approximation statement functions
      defineDifferenceOrder2Components1(u,RX)
!*       defineDifferenceOrder2Components1(un,none)
      defineDifferenceOrder4Components1(u,RX)
!*       defineDifferenceOrder4Components1(un,none)

!* #Include "jacobianDerivatives.h"

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

      if( t.le.dt .or. debug.gt.1 )then
        write(*,'(" bcAdjacent: **START** grid=",i4," side,axis=",2i2)') grid,side,axis
      end if
     
      if( axis.lt.0 .or. axis.gt. nd-1 .or. side.lt.0 .or. side.gt.1 )then
        write(*,'(" bcAdjacent:ERROR: invalid side or axis, side,axis,nd==",3i6)') side,axis,nd
        stop 5555
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

      ! ------------------------------------------------------------------------
      ! ------------------Corners-----------------------------------------------
      ! ------------------------------------------------------------------------

      ! We need to assign points "C" in the corner region:
      !
      !  (sidep1,axisp1)
      !              |  |  |
      !              +--+--+--
      !              |  |  |
      !        X--X--X--+--+--  <- (side,axis)
      !        |  |  |
      !        C--C--X
      !        |  |  |
      !        C--C--X
      ! 
 
      ! The master face is : (side,axis)
      side1a=0
      side1b=1
      side2a=0
      side2b=1
      side3a=0
      side3b=1
      if( axis.eq.0 )then
        side1a=side
        side1b=side
      else if( axis.eq.1 )then
        side2a=side
        side2b=side
      else 
        side3a=side
        side3b=side
      end if        

      axisp1 = mod( axis+1,nd)   ! adjacent dir
      axisp2 = mod( axis+2,nd)   ! adjacent dir

      ! m1a, m2a, m3a are used as the starting ghost point 
      m1a=1
      m2a=1
      m3a=1
       ! apply symmetry BC to extended boundary too: 
      if( axis.eq.0 )then
        m1a=0
      else if( axis.eq.1 )then
        m2a=0
      else
        m3a=0
      end if

      is2=0
      is3=0
      j3=0
      k3=0


      bc=boundaryCondition(side,axis)
      if( bc.le.0 )then
        write(*,'(" bcAdjacent:ERROR: bc<=0 !")') 
        stop 6666
      end if

      if( nd.eq.2 )then

       ! **** 2D ****

       i3=gridIndexRange(0,2)


       do side2=side2a,side2b  ! i2 
       do side1=side1a,side1b  ! i1
        sidev(0)=side1 
        sidev(1)=side2
        is1=1-2*side1  ! for the ghost pt
        is2=1-2*side2
        ks1=is1        ! for the symmetry pt
        ks2=is2
        ks3=is3
        if( bc.ne.symmetryBoundaryCondition )then
          ! do not reflect symmetry pt about a non-symmetry BC
          if( axis.eq.0 )then
            ks1=-is1
          else if( axis.eq.1 )then
            ks2=-is2
          else
            ks3=-is3
          end if
        end if

        bcp1=boundaryCondition(sidev(axisp1),axisp1)

        if( bcp1.eq.symmetryBoundaryCondition .or. (bcp1.ge.abcEM2 .and. bcp1.le.abc5) )then

          ! --- Adjacent face is symmetry  or ABC 

          i1=gridIndexRange(side1,0) ! (i1,i2,i3)=corner point
          i2=gridIndexRange(side2,1)

          if( mask(i1,i2,i3).gt.0 )then ! *wdh* 090712

           ! write(*,'(" bcAdjacent: corner: grid, side,axis, side1,side2, i1,i2=",i4,2(i2,i2,1x),2i5)') grid,side,axis,side1,side2,i1,i2
 
 
           ! --- Now assign all ghost points adjacent to this boundary
           ! extrapolateGhost(ex,ey,hz,numberOfGhostPoints,numberOfGhostPoints,0)
           bcSymmetryGhost(ex,ey,hz,numberOfGhostPoints,numberOfGhostPoints,1)


           ! project the corner point to be div free
           is1=0
           is2=0
           if( axis.eq.0 )then
             is2=1-2*side2
             is=is2
           else
             is1=1-2*side1
             is=is1
           end if
           if( gridType.eq.curvilinear .and. orderOfAccuracy.eq.4 )then
            ! ----------------------------
            ! set div(E)=0 *wdh* 090712
            ! (-u(i+2) +8*(u(i+1)-u(i-1)) + u(i-2))/(12*dx) + vy = 0 
            ! try this: set first ghost line
            div = ux42(i1,i2,i3,ex)+uy42(i1,i2,i3,ey)
            ax = -is*rsxy(i1,i2,i3,axisp1,0)/(1.5*dr(axisp1))  ! coeff of u(-1)  8/12 = 1/(1.5)
            ay = -is*rsxy(i1,i2,i3,axisp1,1)/(1.5*dr(axisp1))
            aSq = max( epsX, ax**2 + ay**2)
            u(i1-is1,i2-is2,i3-is3,ex) = u(i1-is1,i2-is2,i3-is3,ex) - div*ax/aSq
            u(i1-is1,i2-is2,i3-is3,ey) = u(i1-is1,i2-is2,i3-is3,ey) - div*ay/aSq

            !write(*,'(" bcAdjacent: i1,i2=",2i3," dr(0),dr(1),rx,ry=",4e10.2)') i1,i2,dr(0),dr(1),rsxy(i1,i2,i3,0,0),rsxy(i1,i2,i3,0,1)
            !write(*,'(" bcAdjacent: i1,i2=",2i3," div0,ax,ay,div=",4e10.2)') i1,i2,div,ax,ay,ux42(i1,i2,i3,ex)+uy42(i1,i2,i3,ey)

           end if

          end if ! mask 
         end if ! if bcp1
       end do  ! end do side1
       end do  ! end do side2    

      else 

       ! ***** 3D *****


        beginEdgeLoops()
         beginLoops()
          if( mask(i1,i2,i3).gt.0 )then 
            bcSymmetryGhost(ex,ey,ez,numGhost1,numGhost2,numGhost3)
          end if
         endLoops()
        endEdgeLoops()


      end if



      return
      end
