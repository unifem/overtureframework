! *******************************************************************************
! ************** Boundary Conditions for Maxwell ********************************
! *******************************************************************************




! These next include files will define the macros that will define the difference approximations
! The actual macro is called below
#Include "defineDiffOrder2f.h"
#Include "defineDiffOrder4f.h"


!**************************************************************************

! Include macros that are common to different orders of accuracy

#Include "bcOptMaxwellMacros.h"

!**************************************************************************

! Here are macros that define the planeWave solution
#Include "planeWave.h"

! ----- Here are macros for the chirped-plane wave -----
#Include "chirpedPlaneWave.h"

! ----- Here are macros for the dispersive plane wave -----
#Include "dispersivePlaneWave.h"

! -------------------------------------------------------------------------------------------------------
! Macro: third-order extrapolation:
! -------------------------------------------------------------------------------------------------------
#defineMacro extrap3(ec,j1,j2,j3,is1,is2,is3)\
      ( 3.*u(j1      ,j2      ,j3      ,ec)-3.*u(j1+  is1,j2+  is2,j3+  is3,ec)+u(j1+2*is1,j2+2*is2,j3+2*is3,ec) )

! -------------------------------------------------------------------------------------------------------
! Macro: fifth-order extrapolation:
! -------------------------------------------------------------------------------------------------------
#defineMacro extrap5(ec,j1,j2,j3,is1,is2,is3)\
      ( 5.*u(j1      ,j2      ,j3      ,ec)-10.*u(j1+  is1,j2+  is2,j3+  is3,ec)+10.*u(j1+2*is1,j2+2*is2,j3+2*is3,ec)\
       -5.*u(j1+3*is1,j2+3*is2,j3+3*is3,ec)+    u(j1+4*is1,j2+4*is2,j3+4*is3,ec) ) 
!========================================================================================
!  Boundary conditions for a curvilinear grid
!
!      D0r( a1.uv ) + D0s( a2.uv) = 0
!      D+^4( tau.uv ) = 0                 (use an even derivative)
!      
!  FORCING: none, twilightZone
!========================================================================================
#beginMacro bcCurvilinear2dOrder2(FORCING)

 ! assign values on boundary when there are boundary forcings
 !! assignBoundaryForcingBoundaryValuesCurvilinear(2)

 dra = dr(axis)*(1-2*side)
 dsa = dr(axisp1)*(1-2*side)
 beginLoops()

  if( mask(i1,i2,i3).gt.0 )then ! check for mask added 2015/06/01 *wdh*
   ! ---- Boundary point is a physical point ---

   jac=1./(rx(i1-is1,i2-is2,i3-is3)*sy(i1-is1,i2-is2,i3-is3)-ry(i1-is1,i2-is2,i3-is3)*sx(i1-is1,i2-is2,i3-is3))  

   a11 =rsxy(i1-is1,i2-is2,i3-is3,axis  ,0)*jac
   a12 =rsxy(i1-is1,i2-is2,i3-is3,axis  ,1)*jac
   tau1=rsxy(i1-is1,i2-is2,i3-is3,axisp1,0)
   tau2=rsxy(i1-is1,i2-is2,i3-is3,axisp1,1)

   det = -tau2*a11+a12*tau1

   jacp1=1./(rx(i1+is1,i2+is2,i3+is3)*sy(i1+is1,i2+is2,i3+is3)-ry(i1+is1,i2+is2,i3+is3)*sx(i1+is1,i2+is2,i3+is3))          
   a11p1=jacp1*rsxy(i1+is1,i2+is2,i3+is3,axis,0)
   a12p1=jacp1*rsxy(i1+is1,i2+is2,i3+is3,axis,1)


   ! ** tau1DotU=tau1*u(i1,i2,i3,ex)+tau2*u(i1,i2,i3,ey)
   ! ** u(i1,i2,i3,ex)=u(i1,i2,i3,ex)-tau1DotU*tau1
   ! ** u(i1,i2,i3,ey)=u(i1,i2,i3,ey)-tau1DotU*tau2

   ! gx2=tau1*(3.*u(i1,i2,i3,ex)-3.*u(i1+is1,i2+is2,i3+is3,ex)+u(i1+2*is1,i2+2*is2,i3,ex))\
   !    +tau2*(3.*u(i1,i2,i3,ey)-3.*u(i1+is1,i2+is2,i3+is3,ey)+u(i1+2*is1,i2+2*is2,i3,ey))

   ! use even derivatives of the tangential component:
   ! gx2=tau1*(2.*u(i1,i2,i3,ex)-u(i1+is1,i2+is2,i3+is3,ex))\
   !    +tau2*(2.*u(i1,i2,i3,ey)-u(i1+is1,i2+is2,i3+is3,ey))
   
   gx2=tau1*(4.*u(i1,i2,i3,ex)-6.*u(i1+is1,i2+is2,i3,ex)+4.*u(i1+2*is1,i2+2*is2,i3,ex)-u(i1+3*is1,i2+3*is2,i3,ex))\
      +tau2*(4.*u(i1,i2,i3,ey)-6.*u(i1+is1,i2+is2,i3,ey)+4.*u(i1+2*is1,i2+2*is2,i3,ey)-u(i1+3*is1,i2+3*is2,i3,ey))

   g2a=0.

   ! include a2 terms in case tangential components are non-zero:
   a21zp1=A21(i1+js1,i2+js2,i3)
   a22zp1=A22(i1+js1,i2+js2,i3)
   a21zm1=A21(i1-js1,i2-js2,i3)
   a22zm1=A22(i1-js1,i2-js2,i3)
   g1a=    a11p1*u(i1+is1,i2+is2,i3,ex)+ a12p1*u(i1+is1,i2+is2,i3,ey)\
     + ( (a21zp1*u(i1+js1,i2+js2,i3,ex)+a22zp1*u(i1+js1,i2+js2,i3,ey))\
        -(a21zm1*u(i1-js1,i2-js2,i3,ex)+a22zm1*u(i1-js1,i2-js2,i3,ey)) )*dra/dsa


   if( boundaryForcingOption.ne.noBoundaryForcing )then
     ! ---- compute RHS for HZ ----
     ! --- add boundary forcing when we are directly computing the scattered field ---

     if( .true. )then
       ! *new way* 2016/08/08
       numberOfTimeDerivatives=fieldOption+1
       x0=xy(i1,i2,i3,0)
       y0=xy(i1,i2,i3,1)
       getBoundaryForcing2D(x0,y0,t,numberOfTimeDerivatives,ubv)
       u0t=-ubv(ex)
       v0t=-ubv(ey)
       if( .false. )then
         ! check time derivative by differences
         numberOfTimeDerivatives=0
         dteps=1.e-4
         getBoundaryForcing2D(x0,y0,t,numberOfTimeDerivatives,uv)
         getBoundaryForcing2D(x0,y0,t-dteps,numberOfTimeDerivatives,uvm)
         utDiff = (uv(ey)-uvm(ey))/dteps
         write(*,'(" Ey_t, Ey_t(diff) err=",3e12.3)') ubv(ey),utDiff,ubv(ey)-utDiff
       end if
     else if( boundaryForcingOption.eq.planeWaveBoundaryForcing )then 
       ! *old way*
       ! *** for planeWaveBoundaryForcing we need to use: u.t=w.y and v.t=-w.x =>
       ! *****  (n1,n2).(w.x,w.y) = -n1*v.t + n2*u.t
       !  OR    (rx,ry).(w.x,w.y) = -rx*v.t + ry*u.t
       !   (rx**2+ry**2) w.r + (rx*sx+ry*sy)*ws = -rx*vt + ry*ut 
       x0=xy(i1,i2,i3,0)
       y0=xy(i1,i2,i3,1)
       ! Note minus sign since we are subtracting out the incident field
       if( fieldOption.eq.0 )then
         u0t=-planeWave2Dext(x0,y0,t) 
         v0t=-planeWave2Deyt(x0,y0,t)
       else
         ! we are assigning time derivatives (sosup)
         u0t=-planeWave2Dextt(x0,y0,t) 
         v0t=-planeWave2Deytt(x0,y0,t)
       endif
     end if

     g2a=(2.*dra)*( \
              (rsxy(i1,i2,i3,axis,0)*rsxy(i1,i2,i3,axisp1,0)+rsxy(i1,i2,i3,axis,1)*rsxy(i1,i2,i3,axisp1,1))* \
              (u(i1+js1,i2+js2,i3,hz)-u(i1-js1,i2-js2,i3,hz))/(2.*dsa) \
         + rsxy(i1,i2,i3,axis,0)*v0t - rsxy(i1,i2,i3,axis,1)*u0t )/(rsxy(i1,i2,i3,axis,0)**2+rsxy(i1,i2,i3,axis,1)**2)

  ! write(*,'(" bc2: i=",3i3," side,axis,is1,is2,js1,js2=",6i3," dra,dsa=",2e10.2)') i1,i2,i3,side,axis,is1,is2,js1,js2,dra,dsa

   else
     ! this is new: include RHS 040116  -- note: this will be zero if the grid is orthogonal
     g2a=(2.*dra)*( \
              (rsxy(i1,i2,i3,axis,0)*rsxy(i1,i2,i3,axisp1,0)+rsxy(i1,i2,i3,axis,1)*rsxy(i1,i2,i3,axisp1,1))* \
              (u(i1+js1,i2+js2,i3,hz)-u(i1-js1,i2-js2,i3,hz))/(2.*dsa) \
                   )/(rsxy(i1,i2,i3,axis,0)**2+rsxy(i1,i2,i3,axis,1)**2)
   end if


   #If #FORCING == "twilightZone"

     ! Since div(E)=0 for the TZ solutions, we do not need to adjust the BC's
     ! *wdh* 2015/05/31 

     !* g1a= a11p1*u(i1+is1,i2+is2,i3,ex)+ a12p1*u(i1+is1,i2+is2,i3,ey)

     ! Evaluate true solution (fieldOption==0) or its time derivative (fieldOption==1)
     !* call ogf2dfo(ep,fieldOption,xy(i1-is1,i2-is2,i3,0),xy(i1-is1,i2-is2,i3,1),t, um,vm,wm)
     !* call ogf2dfo(ep,fieldOption,xy(i1    ,i2    ,i3,0),xy(i1    ,i2    ,i3,1),t, u0,v0,w0)
     !* call ogf2dfo(ep,fieldOption,xy(i1+is1,i2+is2,i3,0),xy(i1+is1,i2+is2,i3,1),t, up,vp,wp)

     !* g1a=g1a+ (  a11*um+  a12*vm ) - (a11p1*up+a12p1*vp)
     !* gx2=gx2- (tau1*(2.*u0-up -um) \
     !*          +tau2*(2.*v0-vp -vm) )

     ! old: g2a=g2a+ wm-wp

     call ogf2dfo(ep,ex,ey,hz,fieldOption,xy(i1-is1,i2-is2,i3,0),xy(i1-is1,i2-is2,i3,1),t, um,vm,wm)
     call ogf2dfo(ep,ex,ey,hz,fieldOption,xy(i1+is1,i2+is2,i3,0),xy(i1+is1,i2+is2,i3,1),t, up,vp,wp)
     call ogf2dfo(ep,ex,ey,hz,fieldOption,xy(i1-js1,i2-js2,i3,0),xy(i1-js1,i2-js2,i3,1),t, uzm,vzm,wzm)
     call ogf2dfo(ep,ex,ey,hz,fieldOption,xy(i1+js1,i2+js2,i3,0),xy(i1+js1,i2+js2,i3,1),t, uzp,vzp,wzp)
     ws = (wzp-wzm)/(2.*dsa)  
     wr = (wp - wm)/(2.*dra)
     wx = rsxy(i1,i2,i3,axis,0)*wr + rsxy(i1,i2,i3,axisp1,0)*ws
     wy = rsxy(i1,i2,i3,axis,1)*wr + rsxy(i1,i2,i3,axisp1,1)*ws
     g2a=g2a -  (2.*dra)*( rsxy(i1,i2,i3,axis,0)*wx + rsxy(i1,i2,i3,axis,1)*wy )/(rsxy(i1,i2,i3,axis,0)**2+rsxy(i1,i2,i3,axis,1)**2)


     ! write(*,'("bcOpt: side,axis=",2i2," i1,i2=",2i4," x,y=",2f6.3," ex,ey,hz=",3f6.2)') \
     !      side,axis,i1,i2,xy(i1    ,i2    ,i3,0),xy(i1    ,i2    ,i3,1),u0,v0,w0

   #Elif #FORCING == "none"
   #Else
     stop 2785
   #End

   if( useChargeDensity.eq.1 )then
    ! div(eps*E) = rho , rho is saved in f(i1,i2,i3,0)
    jac0=1./(rx(i1,i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3))  
    g1a = g1a - 2.*dra*f(i1,i2,i3,0)*jac0/eps
   end if


   u(i1-is1,i2-is2,i3-is3,ex) = (-tau2*g1a +a12*gx2)/det

   u(i1-is1,i2-is2,i3-is3,ey) = ( tau1*g1a -a11*gx2)/det

   u(i1-is1,i2-is2,i3-is3,hz)=u(i1+is1,i2+is2,i3+is3,hz) + g2a

  else if( mask(i1,i2,i3).lt.0 )then
   ! ---- Boundary point is an interpolation point ---
   ! extrapolate ghost points:
    u(i1-is1,i2-is2,i3-is3,ex)=extrap3(ex,i1,i2,i3,is1,is2,is3)
    u(i1-is1,i2-is2,i3-is3,ey)=extrap3(ey,i1,i2,i3,is1,is2,is3)
    u(i1-is1,i2-is2,i3-is3,hz)=extrap3(hz,i1,i2,i3,is1,is2,is3)
  else
    ! boundary point is unused -- extrap for now ??
    u(i1-is1,i2-is2,i3-is3,ex)=extrap3(ex,i1,i2,i3,is1,is2,is3)
    u(i1-is1,i2-is2,i3-is3,ey)=extrap3(ey,i1,i2,i3,is1,is2,is3)
    u(i1-is1,i2-is2,i3-is3,hz)=extrap3(hz,i1,i2,i3,is1,is2,is3)
  end if

 endLoops()
 if( .false. )then
   beginLoops()
    a11p1= A11(i1+  is1,i2+  is2,i3) 
    a11m1= A11(i1-  is1,i2-  is2,i3) 
    a12p1= A12(i1+  is1,i2+  is2,i3) 
    a12m1= A12(i1-  is1,i2-  is2,i3) 
     a21zp1=A21(i1+js1,i2+js2,i3)
     a22zp1=A22(i1+js1,i2+js2,i3)
     a21zm1=A21(i1-js1,i2-js2,i3)
     a22zm1=A22(i1-js1,i2-js2,i3)
    divc2= (a11p1*u(i1+  is1,i2+  is2,i3,ex)-a11m1*u(i1-  is1,i2-  is2,i3,ex))/(2.*dra) \
          +(a12p1*u(i1+  is1,i2+  is2,i3,ey)-a12m1*u(i1-  is1,i2-  is2,i3,ey))/(2.*dra) \
          +(a21zp1*u(i1+  js1,i2+  js2,i3,ex)-a21zm1*u(i1-  js1,i2-  js2,i3,ex))/(2.*dsa) \
          +(a22zp1*u(i1+  js1,i2+  js2,i3,ey)-a22zm1*u(i1-  js1,i2-  js2,i3,ey))/(2.*dsa)

     write(*,'(" bc2: i=",3i3," u=",2f12.9," divc2=",e10.2," div2=",e10.2)') i1,i2,i3,u(i1,i2,i3,ex),u(i1,i2,i3,ey),divc2,ux22(i1,i2,i3,ex)+uy22(i1,i2,i3,ey)
         ! '
   endLoops()
 end if
#endMacro

#defineMacro RXDET(i1,i2,i3)\
    (rx(i1,i2,i3)*(sy(i1,i2,i3)*tz(i1,i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))\
    +ry(i1,i2,i3)*(sz(i1,i2,i3)*tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))\
    +rz(i1,i2,i3)*(sx(i1,i2,i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3)))

! =============================================================================================
! These formulae are from maxwell/bc.maple
! =============================================================================================
#beginMacro bcCurvilinear3dOrder2(FORCING)

 ! assign values on boundary when there are boundary forcings
 !! assignBoundaryForcingBoundaryValuesCurvilinear(3)

 ! Since is1 is +1 or -1 we need to flip the sign of dr in the derivative approximations
 dra = dr(axis  )*(1-2*side)
 dsa = dr(axisp1)*(1-2*side)
 dta = dr(axisp2)*(1-2*side)

 beginLoops()

  if( mask(i1,i2,i3).gt.0 )then ! check for mask added 2015/06/01 *wdh*
   ! ---- Boundary point is a physical point ---

   jac=1./RXDET(i1-is1,i2-is2,i3-is3)

   a11m =rsxy(i1-is1,i2-is2,i3-is3,axis  ,0)*jac
   a12m =rsxy(i1-is1,i2-is2,i3-is3,axis  ,1)*jac
   a13m =rsxy(i1-is1,i2-is2,i3-is3,axis  ,2)*jac

   tau11=rsxy(i1-is1,i2-is2,i3-is3,axisp1,0)
   tau12=rsxy(i1-is1,i2-is2,i3-is3,axisp1,1)
   tau13=rsxy(i1-is1,i2-is2,i3-is3,axisp1,2)

   tau21=rsxy(i1-is1,i2-is2,i3-is3,axisp2,0)
   tau22=rsxy(i1-is1,i2-is2,i3-is3,axisp2,1)
   tau23=rsxy(i1-is1,i2-is2,i3-is3,axisp2,2)

   det = (-tau12*a13m*tau21+a12m*tau13*tau21-a11m*tau22*tau13+a13m*tau22*tau11+tau12*a11m*tau23-a12m*tau11*tau23)

   jacp1=1./RXDET(i1+is1,i2+is2,i3+is3)
   a11p=rsxy(i1+is1,i2+is2,i3+is3,axis,0)*jacp1
   a12p=rsxy(i1+is1,i2+is2,i3+is3,axis,1)*jacp1
   a13p=rsxy(i1+is1,i2+is2,i3+is3,axis,2)*jacp1


   ! ** tau1DotU=tau11*u(i1,i2,i3,ex)+tau12*u(i1,i2,i3,ey)+tau13*u(i1,i2,i3,ez)
   ! ** tau2DotU=tau21*u(i1,i2,i3,ex)+tau22*u(i1,i2,i3,ey)+tau23*u(i1,i2,i3,ez)

   ! ** this assumes tau1 and tau2 are orthogonal **
   ! ** u(i1,i2,i3,ex)=u(i1,i2,i3,ex)-tau1DotU*tau11-tau2DotU*tau21
   ! ** u(i1,i2,i3,ey)=u(i1,i2,i3,ey)-tau1DotU*tau12-tau2DotU*tau22
   ! ** u(i1,i2,i3,ez)=u(i1,i2,i3,ez)-tau1DotU*tau13-tau2DotU*tau23

   ! use even derivatives of the tangential component:
   ! gx1=tau11*(2.*u(i1,i2,i3,ex)-u(i1+is1,i2+is2,i3+is3,ex))\
   !    +tau12*(2.*u(i1,i2,i3,ey)-u(i1+is1,i2+is2,i3+is3,ey))\
   !    +tau13*(2.*u(i1,i2,i3,ez)-u(i1+is1,i2+is2,i3+is3,ez))

   ! gx2=tau21*(2.*u(i1,i2,i3,ex)-u(i1+is1,i2+is2,i3+is3,ex))\
   !    +tau22*(2.*u(i1,i2,i3,ey)-u(i1+is1,i2+is2,i3+is3,ey))\
   !    +tau23*(2.*u(i1,i2,i3,ez)-u(i1+is1,i2+is2,i3+is3,ez))

   ! Use higher order ** 040120 **
   gx1=tau11*(4.*u(i1,i2,i3,ex)-6.*u(i1+is1,i2+is2,i3+is3,ex)+4.*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)\
                                                                -u(i1+3*is1,i2+3*is2,i3+3*is3,ex))\
      +tau12*(4.*u(i1,i2,i3,ey)-6.*u(i1+is1,i2+is2,i3+is3,ey)+4.*u(i1+2*is1,i2+2*is2,i3+2*is3,ey)\
                                                                -u(i1+3*is1,i2+3*is2,i3+3*is3,ey))\
      +tau13*(4.*u(i1,i2,i3,ez)-6.*u(i1+is1,i2+is2,i3+is3,ez)+4.*u(i1+2*is1,i2+2*is2,i3+2*is3,ez)\
                                                                -u(i1+3*is1,i2+3*is2,i3+3*is3,ez))
   
   gx2=tau21*(4.*u(i1,i2,i3,ex)-6.*u(i1+is1,i2+is2,i3+is3,ex)+4.*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)\
                                                                -u(i1+3*is1,i2+3*is2,i3+3*is3,ex))\
      +tau22*(4.*u(i1,i2,i3,ey)-6.*u(i1+is1,i2+is2,i3+is3,ey)+4.*u(i1+2*is1,i2+2*is2,i3+2*is3,ey)\
                                                                -u(i1+3*is1,i2+3*is2,i3+3*is3,ey))\
      +tau23*(4.*u(i1,i2,i3,ez)-6.*u(i1+is1,i2+is2,i3+is3,ez)+4.*u(i1+2*is1,i2+2*is2,i3+2*is3,ez)\
                                                                -u(i1+3*is1,i2+3*is2,i3+3*is3,ez))

   ! g1 = RHS for divergence equation  a1.u(-1) = g1
   ! Use this next option always (needed for TZ too) *wdh* 2015/05/31
   if( .true. .or. boundaryForcingOption.eq.planeWaveBoundaryForcing )then
    ! include a2,a3 terms in case tangential components are non-zero:
    a21zp1=A21D3(i1+js1,i2+js2,i3+js3)
    a22zp1=A22D3(i1+js1,i2+js2,i3+js3)
    a23zp1=A23D3(i1+js1,i2+js2,i3+js3)
    a21zm1=A21D3(i1-js1,i2-js2,i3-js3)
    a22zm1=A22D3(i1-js1,i2-js2,i3-js3)
    a23zm1=A23D3(i1-js1,i2-js2,i3-js3)
 
    a31zp1=A31D3(i1+ks1,i2+ks2,i3+ks3)
    a32zp1=A32D3(i1+ks1,i2+ks2,i3+ks3)
    a33zp1=A33D3(i1+ks1,i2+ks2,i3+ks3)
    a31zm1=A31D3(i1-ks1,i2-ks2,i3-ks3)
    a32zm1=A32D3(i1-ks1,i2-ks2,i3-ks3)
    a33zm1=A33D3(i1-ks1,i2-ks2,i3-ks3)
 
    g1=a11p*u(i1+is1,i2+is2,i3+is3,ex)+a12p*u(i1+is1,i2+is2,i3+is3,ey)+a13p*u(i1+is1,i2+is2,i3+is3,ez)\
      + ( (a21zp1*u(i1+js1,i2+js2,i3+js3,ex)+a22zp1*u(i1+js1,i2+js2,i3+js3,ey)+a23zp1*u(i1+js1,i2+js2,i3+js3,ez))\
         -(a21zm1*u(i1-js1,i2-js2,i3-js3,ex)+a22zm1*u(i1-js1,i2-js2,i3-js3,ey)+a23zm1*u(i1-js1,i2-js2,i3-js3,ez))\
            )*dra/dsa \
      + ( (a31zp1*u(i1+ks1,i2+ks2,i3+ks3,ex)+a32zp1*u(i1+ks1,i2+ks2,i3+ks3,ey)+a33zp1*u(i1+ks1,i2+ks2,i3+ks3,ez))\
         -(a31zm1*u(i1-ks1,i2-ks2,i3-ks3,ex)+a32zm1*u(i1-ks1,i2-ks2,i3-ks3,ey)+a33zm1*u(i1-ks1,i2-ks2,i3-ks3,ez))\
            )*dra/dta
   else
     g1=a11p*u(i1+is1,i2+is2,i3+is3,ex)+a12p*u(i1+is1,i2+is2,i3+is3,ey)+a13p*u(i1+is1,i2+is2,i3+is3,ez)
   end if
   #If #FORCING == "twilightZone"

     ! Since div(E)=0 for the TZ solutions, we do not need to adjust the BC's
     ! *wdh* 2015/05/31 

     !* OGF3DFO(i1-is1,i2-is2,i3-is3,t,uvm(0),uvm(1),uvm(2)) 
     !* OGF3DFO(i1    ,i2    ,i3    ,t,uv0(0),uv0(1),uv0(2))
     !* OGF3DFO(i1+is1,i2+is2,i3+is3,t,uvp(0),uvp(1),uvp(2))

     !* gx1=gx1-(tau11*(2.*uv0(0)-uvp(0)-uvm(0))\
     !*         +tau12*(2.*uv0(1)-uvp(1)-uvm(1))\
     !*         +tau13*(2.*uv0(2)-uvp(2)-uvm(2)) )

     !* gx2=gx2-(tau21*(2.*uv0(0)-uvp(0)-uvm(0))\
     !*         +tau22*(2.*uv0(1)-uvp(1)-uvm(1))\
     !*         +tau23*(2.*uv0(2)-uvp(2)-uvm(2)) )

     !* g1=g1+ (a11m*uvm(0)+a12m*uvm(1)+a13m*uvm(2)) \
     !*     - ( a11p*uvp(0)+a12p*uvp(1)+a13p*uvp(2) )

   #Elif #FORCING == "none"
   #Else
     stop 2785
   #End

   if( useChargeDensity.eq.1 )then
    ! div(eps*E) = rho , rho is saved in f(i1,i2,i3,0)
    jac0=1./RXDET(i1,i2,i3)
    g1a = g1a - 2.*dra*f(i1,i2,i3,0)*jac0/eps
   end if

  det=tau11*a13m*tau22-a11m*tau13*tau22-a13m*tau21*tau12+a12m*tau21*tau13-tau11*a12m*tau23+a11m*tau12*tau23

  u(i1-is1,i2-is2,i3-is3,ex) = -(-a12m*gx2*tau13+a12m*tau23*gx1+a13m*tau12*gx2-a13m*gx1*tau22+g1*tau22*tau13-g1*tau23*tau12)/det
  u(i1-is1,i2-is2,i3-is3,ez) = -(-tau11*g1*tau22+tau11*a12m*gx2-a11m*tau12*gx2+a11m*gx1*tau22-a12m*tau21*gx1+g1*tau21*tau12)/det
  u(i1-is1,i2-is2,i3-is3,ey) = (tau21*g1*tau13-a11m*gx2*tau13+a11m*tau23*gx1+gx2*tau11*a13m-tau23*tau11*g1-tau21*a13m*gx1)/det


  ! if( debug.gt.31 )then
  !  write(*,'(" bc2: i=",3i4," u(-1)=",3e11.3,", err=",3e10.2)') i1,i2,i3,\
  !    u(i1-is1,i2-is2,i3-is3,ex),u(i1-is1,i2-is2,i3-is3,ey),u(i1-is1,i2-is2,i3-is3,ez),\
  !    u(i1-is1,i2-is2,i3-is3,ex)-uvm(0),u(i1-is1,i2-is2,i3-is3,ey)-uvm(1),u(i1-is1,i2-is2,i3-is3,ez)-uvm(2)
  !  
  !  write(*,'(" d0(a1.u)=",e10.2," tau1.d+d-(u)=",e10.2," tau2.d+d-(u)=",e10.2)') \
  !    (a11p*(u(i1+is1,i2+is2,i3+is3,ex)-uvp(0))\
  !    +a12p*(u(i1+is1,i2+is2,i3+is3,ey)-uvp(1))+a13p*(u(i1+is1,i2+is2,i3+is3,ez)-uvp(2)))\
  !   -(a11m*(u(i1-is1,i2-is2,i3-is3,ex)-uvm(0))+a12m*(u(i1-is1,i2-is2,i3-is3,ey)-uvm(1))\
  !    +a13m*(u(i1-is1,i2-is2,i3-is3,ez)-uvm(2))),\
  !    tau11*((u(i1-is1,i2-is2,i3-is3,ex)-uvm(0))-2.*(u(i1,i2,i3,ex)-uv0(0))+(u(i1+is1,i2+is2,i3+is3,ex)-uvp(0)))\
  !   +tau12*((u(i1-is1,i2-is2,i3-is3,ey)-uvm(1))-2.*(u(i1,i2,i3,ey)-uv0(1))+(u(i1+is1,i2+is2,i3+is3,ey)-uvp(1)))\
  !   +tau13*((u(i1-is1,i2-is2,i3-is3,ez)-uvm(2))-2.*(u(i1,i2,i3,ez)-uv0(2))+(u(i1+is1,i2+is2,i3+is3,ez)-uvp(2))),\
  !    tau21*((u(i1-is1,i2-is2,i3-is3,ex)-uvm(0))-2.*(u(i1,i2,i3,ex)-uv0(0))+(u(i1+is1,i2+is2,i3+is3,ex)-uvp(0)))\
  !   +tau22*((u(i1-is1,i2-is2,i3-is3,ey)-uvm(1))-2.*(u(i1,i2,i3,ey)-uv0(1))+(u(i1+is1,i2+is2,i3+is3,ey)-uvp(1)))\
  !   +tau23*((u(i1-is1,i2-is2,i3-is3,ez)-uvm(2))-2.*(u(i1,i2,i3,ez)-uv0(2))+(u(i1+is1,i2+is2,i3+is3,ez)-uvp(2)))
  !   ! "'
  ! end if

  else if( mask(i1,i2,i3).lt.0 )then
   ! ---- Boundary point is an interpolation point ---
   ! extrapolate ghost points:
    u(i1-is1,i2-is2,i3-is3,ex)=extrap3(ex,i1,i2,i3,is1,is2,is3)
    u(i1-is1,i2-is2,i3-is3,ey)=extrap3(ey,i1,i2,i3,is1,is2,is3)
    u(i1-is1,i2-is2,i3-is3,ez)=extrap3(ez,i1,i2,i3,is1,is2,is3)
  else
    ! boundary point is unused -- extrap for now ??
    u(i1-is1,i2-is2,i3-is3,ex)=extrap3(ex,i1,i2,i3,is1,is2,is3)
    u(i1-is1,i2-is2,i3-is3,ey)=extrap3(ey,i1,i2,i3,is1,is2,is3)
    u(i1-is1,i2-is2,i3-is3,ez)=extrap3(ez,i1,i2,i3,is1,is2,is3)
  end if

 endLoops()
 if( .false. .and. boundaryForcingOption.eq.planeWaveBoundaryForcing )then
  beginLoops()

   jac=1./RXDET(i1-is1,i2-is2,i3-is3)
   a11m =rsxy(i1-is1,i2-is2,i3-is3,axis  ,0)*jac
   a12m =rsxy(i1-is1,i2-is2,i3-is3,axis  ,1)*jac
   a13m =rsxy(i1-is1,i2-is2,i3-is3,axis  ,2)*jac

   jacp1=1./RXDET(i1+is1,i2+is2,i3+is3)
   a11p=rsxy(i1+is1,i2+is2,i3+is3,axis,0)*jacp1
   a12p=rsxy(i1+is1,i2+is2,i3+is3,axis,1)*jacp1
   a13p=rsxy(i1+is1,i2+is2,i3+is3,axis,2)*jacp1

    a21zp1=A21D3(i1+js1,i2+js2,i3+js3)
    a22zp1=A22D3(i1+js1,i2+js2,i3+js3)
    a23zp1=A23D3(i1+js1,i2+js2,i3+js3)
    a21zm1=A21D3(i1-js1,i2-js2,i3-js3)
    a22zm1=A22D3(i1-js1,i2-js2,i3-js3)
    a23zm1=A23D3(i1-js1,i2-js2,i3-js3)
 
    a31zp1=A31D3(i1+ks1,i2+ks2,i3+ks3)
    a32zp1=A32D3(i1+ks1,i2+ks2,i3+ks3)
    a33zp1=A33D3(i1+ks1,i2+ks2,i3+ks3)
    a31zm1=A31D3(i1-ks1,i2-ks2,i3-ks3)
    a32zm1=A32D3(i1-ks1,i2-ks2,i3-ks3)
    a33zm1=A33D3(i1-ks1,i2-ks2,i3-ks3)

    g1=( (a11p*u(i1+is1,i2+is2,i3+is3,ex)+a12p*u(i1+is1,i2+is2,i3+is3,ey)+a13p*u(i1+is1,i2+is2,i3+is3,ez))\
        -(a11m*u(i1-is1,i2-is2,i3-is3,ex)+a12m*u(i1-is1,i2-is2,i3-is3,ey)+a13m*u(i1-is1,i2-is2,i3-is3,ez))\
           )/(2.*dra) \
      + ( (a21zp1*u(i1+js1,i2+js2,i3+js3,ex)+a22zp1*u(i1+js1,i2+js2,i3+js3,ey)+a23zp1*u(i1+js1,i2+js2,i3+js3,ez))\
         -(a21zm1*u(i1-js1,i2-js2,i3-js3,ex)+a22zm1*u(i1-js1,i2-js2,i3-js3,ey)+a23zm1*u(i1-js1,i2-js2,i3-js3,ez))\
            )/(2.*dsa) \
      + ( (a31zp1*u(i1+ks1,i2+ks2,i3+ks3,ex)+a32zp1*u(i1+ks1,i2+ks2,i3+ks3,ey)+a33zp1*u(i1+ks1,i2+ks2,i3+ks3,ez))\
         -(a31zm1*u(i1-ks1,i2-ks2,i3-ks3,ex)+a32zm1*u(i1-ks1,i2-ks2,i3-ks3,ey)+a33zm1*u(i1-ks1,i2-ks2,i3-ks3,ez))\
            )/(2.*dta)

  write(*,'(" bc2-3d: i=",3i4," divc=",e10.2," div=",e10.2)') i1,i2,i3,g1,ux23(i1,i2,i3,ex)+uy23(i1,i2,i3,ey)+uz23(i1,i2,i3,ez)

  endLoops()
 end if

#endMacro



#beginMacro buildFile(NAME,DIM,ORDER)
#beginFile NAME.f
 BC_MAXWELL(NAME,DIM,ORDER)
#endFile
#endMacro

      buildFile(bcOptMaxwell2dOrder2,2,2)
      buildFile(bcOptMaxwell3dOrder2,3,2)

!     order=4: generated by bcOptMaxwell4.bf

      buildFile(bcOptMaxwell2dOrder6,2,6)
      buildFile(bcOptMaxwell3dOrder6,3,6)

      buildFile(bcOptMaxwell2dOrder8,2,8)
      buildFile(bcOptMaxwell3dOrder8,3,8)


! *** build null versions that are faster to compile

#beginMacro BC_MAXWELL_NULL(NAME,DIM,ORDER)
 subroutine NAME( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
                  ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,\
                  gridIndexRange,dimension,u,f,mask,rsxy, xy,\
                  bc, boundaryCondition, ipar, rpar, ierr )
    write(*,'("ERROR: NAME called!")')
    write(*,'("     : This routine is not implemented or not compiled!")')
    stop 12345

  return
  end
#endMacro

#beginMacro buildFileNull(NAME,DIM,ORDER)
#beginFile NAME ## Null.f
 BC_MAXWELL_NULL(NAME,DIM,ORDER)
#endFile
#endMacro

      buildFileNull(bcOptMaxwell2dOrder6,2,6)
      buildFileNull(bcOptMaxwell3dOrder6,3,6)

      buildFileNull(bcOptMaxwell2dOrder8,2,8)
      buildFileNull(bcOptMaxwell3dOrder8,3,8)


      subroutine bcOptMaxwell( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
                               ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,\
                               gridIndexRange,dimension,u,f,mask,rsxy, xy,\
                               bc, boundaryCondition, ipar, rpar, ierr )
! ===================================================================================
!  Optimised Boundary conditions for Maxwell's Equations.
!
!  gridType : 0=rectangular, 1=curvilinear
!  useForcing : 1=use f for RHS to BC
!  side,axis : 0:1 and 0:2
! ===================================================================================

      implicit none

      integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,\
              n1a,n1b,n2a,n2b,n3a,n3b, ndc, bc,ierr

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      real f(ndf1a:ndf1b,ndf2a:ndf2b,ndf3a:ndf3b,0:*)
      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      integer gridIndexRange(0:1,0:2),dimension(0:1,0:2)

      integer ipar(0:*),boundaryCondition(0:1,0:2)
      real rpar(0:*)

!     --- local variables ----
      
      integer orderOfAccuracy

      ierr=0

      orderOfAccuracy      =ipar(9)

      ! -------- ASSIGN boundary values, corners and edges (3d) -----
      !      (assign PEC boundary values here)
      if( orderOfAccuracy.eq.2 )then
        call cornersMxOrder2(nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
                              ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,\
                              gridIndexRange,dimension,u,f,mask,rsxy, xy,\
                              bc, boundaryCondition, ipar, rpar, ierr )
      else if( orderOfAccuracy.eq.4 )then
        call cornersMxOrder4(nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
                              ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,\
                              gridIndexRange,dimension,u,f,mask,rsxy, xy,\
                              bc, boundaryCondition, ipar, rpar, ierr )
      else if( orderOfAccuracy.eq.6 )then
        call cornersMxOrder6(nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
                              ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,\
                              gridIndexRange,dimension,u,f,mask,rsxy, xy,\
                              bc, boundaryCondition, ipar, rpar, ierr )
      else if( orderOfAccuracy.eq.8 )then
        call cornersMxOrder8(nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
                              ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,\
                              gridIndexRange,dimension,u,f,mask,rsxy, xy,\
                              bc, boundaryCondition, ipar, rpar, ierr )
      else
         stop 5533
      end if

      ! ok if( .true. ) return ! **********************************************************
     
      if( nd.eq.2 )then
        if( orderOfAccuracy.eq.2 )then
          call bcOptMaxwell2dOrder2(nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
                                    ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,\
                                    gridIndexRange,dimension,u,f,mask,rsxy, xy,\
                                    bc, boundaryCondition, ipar, rpar, ierr )
        else if( orderOfAccuracy.eq.4 )then
          call bcOptMaxwell2dOrder4(nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
                                    ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,\
                                    gridIndexRange,dimension,u,f,mask,rsxy, xy,\
                                    bc, boundaryCondition, ipar, rpar, ierr )
        else if( orderOfAccuracy.eq.6 )then
          call bcOptMaxwell2dOrder6(nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
                                    ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,\
                                    gridIndexRange,dimension,u,f,mask,rsxy, xy,\
                                    bc, boundaryCondition, ipar, rpar, ierr )
        else if( orderOfAccuracy.eq.8 )then
          call bcOptMaxwell2dOrder8(nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
                                    ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,\
                                    gridIndexRange,dimension,u,f,mask,rsxy, xy,\
                                    bc, boundaryCondition, ipar, rpar, ierr )
        else
          stop 5533
        end if
      else if( nd.eq.3 )then
        if( orderOfAccuracy.eq.2 )then
          call bcOptMaxwell3dOrder2(nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
                                    ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,\
                                    gridIndexRange,dimension,u,f,mask,rsxy, xy,\
                                    bc, boundaryCondition, ipar, rpar, ierr )
        else if( orderOfAccuracy.eq.4 )then
          call bcOptMaxwell3dOrder4(nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
                                    ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,\
                                    gridIndexRange,dimension,u,f,mask,rsxy, xy,\
                                    bc, boundaryCondition, ipar, rpar, ierr )
        else if( orderOfAccuracy.eq.6 )then
          call bcOptMaxwell3dOrder6(nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
                                    ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,\
                                    gridIndexRange,dimension,u,f,mask,rsxy, xy,\
                                    bc, boundaryCondition, ipar, rpar, ierr )
        else if( orderOfAccuracy.eq.8 )then
          call bcOptMaxwell3dOrder8(nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
                                    ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,\
                                    gridIndexRange,dimension,u,f,mask,rsxy, xy,\
                                    bc, boundaryCondition, ipar, rpar, ierr )
        else
          stop 5533
        end if
      else
        stop 8822
      end if

      return
      end



#beginMacro loops1(expression)
do c=ca,cb
do i1=n1a,n1b
do i3=n3a,n3b
do i2=n2a,n2b
  expression
end do
end do
end do
end do
#endMacro

#beginMacro loops2(expression)
do c=ca,cb
do i3=n3a,n3b
do i2=n2a,n2b
do i1=n1a,n1b
  expression
end do
end do
end do
end do
#endMacro


      subroutine periodicUpdateMaxwell(nd, 
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,
     & u,ca,cb, indexRange, gridIndexRange, dimension, 
     & isPeriodic )
!======================================================================
!  Optimised Boundary Conditions
!         
! nd : number of space dimensions
! ca,cb : assign components c=uC(ca),..,uC(cb)
! useWhereMask : if not equal to zero, only apply the BC where mask(i1,i2,i3).ne.0
!======================================================================
      implicit none
      integer nd, 
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b

      integer isPeriodic(0:2),indexRange(0:1,0:2)
      integer gridIndexRange(0:1,0:2),dimension(0:1,0:2)

      real u(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,0:*)
      integer ca,cb

!     --- local variables 
      integer c,i1,i2,i3,axis,diff
      integer n1a,n1b, n2a,n2b, n3a,n3b


      n1a=dimension(0,0)
      n1b=dimension(1,0)
      n2a=dimension(0,1)
      n2b=dimension(1,1)
      n3a=dimension(0,2)
      n3b=dimension(1,2)

      do axis=0,nd-1
        if( isPeriodic(axis).ne.0 )then
!         length of the period:
          diff=gridIndexRange(1,axis)-gridIndexRange(0,axis)
!         assign all ghost points on "left"
!         I[i]=Range(dimension(Start,axis),indexRange(Start,axis)-1);
!         u(I[0],I[1],I[2],I[3])=u(I[0]+diff[0],I[1]+diff[1],I[2]+diff[2],I[3]+diff[3]);
!         // assign all ghost points on "right"
!         I[i]=Range(indexRange(End,axis)+1,dimension(End,axis));
!         u(I[0],I[1],I[2],I[3])=u(I[0]-diff[0],I[1]-diff[1],I[2]-diff[2],I[3]-diff[3]);
          
          if( axis.eq.0 )then
            n1a=dimension(0,0)
            n1b=indexRange(0,0)-1
            loops1(u(i1,i2,i3,c)=u(i1+diff,i2,i3,c))
            n1a=indexRange(1,0)+1
            n1b=dimension(1,0)
            loops1(u(i1,i2,i3,c)=u(i1-diff,i2,i3,c))
            n1a=dimension(0,0)
            n1b=dimension(1,0)
          else if( axis.eq.1 )then
            n2a=dimension(0,1)
            n2b=indexRange(0,1)-1
            loops2(u(i1,i2,i3,c)=u(i1,i2+diff,i3,c))
            n2a=indexRange(1,1)+1
            n2b=dimension(1,1)
            loops2(u(i1,i2,i3,c)=u(i1,i2-diff,i3,c))
            n2a=dimension(0,1)
            n2b=dimension(1,1)
          else
            n3a=dimension(0,2)
            n3b=indexRange(0,2)-1
            loops2(u(i1,i2,i3,c)=u(i1,i2,i3+diff,c))
            n3a=indexRange(1,2)+1
            n3b=dimension(1,2)
            loops2(u(i1,i2,i3,c)=u(i1,i2,i3-diff,c))
            n3a=dimension(0,2)
            n3b=dimension(1,2)
          end if

        end if
      end do
      return
      end
