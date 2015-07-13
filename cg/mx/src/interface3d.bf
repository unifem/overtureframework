! *******************************************************************************
!   Interface boundary conditions
! *******************************************************************************

! These next include files will define the macros that will define the difference approximations
! The actual macro is called below
!* #Include "defineDiffNewerOrder2f.h"
!* #Include "defineDiffNewerOrder4f.h"

! These next include file will define the macros that will define the difference approximations (in op/src)
! Defines getDuDx2(u,aj,ff), getDuDxx2(u,aj,ff), getDuDx3(u,aj,ff), ...  etc. 
#Include "derivMacroDefinitions.h"

! Define 
!    defineParametricDerivativeMacros(u,dr,dx,DIM,ORDER,COMPONENTS,MAXDERIV)
!       defines -> ur2, us2, ux2, uy2, ...            (2D)
!                  ur3, us3, ut3, ux3, uy3, uz3, ...  (3D)
#Include "defineParametricDerivMacros.h"

! defineParametricDerivativeMacros(u,dr,dx,DIM,ORDER,COMPONENTS,MAXDERIV)
! 2D, order=6, components=1
! defineParametricDerivativeMacros(u,dr,dx,DIM,ORDER,COMPONENTS,MAXDERIV)

 defineParametricDerivativeMacros(rsxy1,dr1,dx1,3,2,2,4)
 defineParametricDerivativeMacros(rsxy1,dr1,dx1,3,4,2,2)
 defineParametricDerivativeMacros(u1,dr1,dx1,3,2,1,4)
 defineParametricDerivativeMacros(u1,dr1,dx1,3,4,1,2)

 defineParametricDerivativeMacros(rsxy2,dr2,dx2,3,2,2,4)
 defineParametricDerivativeMacros(rsxy2,dr2,dx2,3,4,2,2)
 defineParametricDerivativeMacros(u2,dr2,dx2,3,2,1,4)
 defineParametricDerivativeMacros(u2,dr2,dx2,3,4,1,2)

! ******************************************************************************************************************
! ************* These are altered version of those from insImp.h ***************************************************
! ******************************************************************************************************************


! ==========================================================================================
!  Evaluate the Jacobian and its derivatives (parametric and spatial). 
!    rsxy   : jacobian matrix name 
!    aj     : prefix for the name of the resulting jacobian variables, 
!             e.g. ajrx, ajsy, ajrxx, ajsxy, ...
!    MAXDER : number of derivatives to evaluate.  
! ==========================================================================================
#beginMacro opEvalJacobianDerivatives(rsxy,i1,i2,i3,aj,MAXDER)

#If $GRIDTYPE eq "curvilinear"
 ! this next call will define the jacobian and its derivatives (parameteric and spatial)
 #peval evalJacobianDerivatives(rsxy,i1,i2,i3,aj,$DIM,$ORDER,MAXDER)

#End

#endMacro 

! ==========================================================================================
!  Evaluate the parametric derivatives of u.
!    u      : evaluate derivatives of this function.
!    uc     : component to evaluate
!    uu     : prefix for the name of the resulting derivatives, e.g. uur, uus, uurr, ...
!    MAXDER : number of derivatives to evaluate.  
! ==========================================================================================
#beginMacro opEvalParametricDerivative(u,i1,i2,i3,uc,uu,MAXDER)
#If $GRIDTYPE eq "curvilinear" 
 #peval evalParametricDerivativesComponents1(u,i1,i2,i3,uc, uu,$DIM,$ORDER,MAXDER)
#Else
 uu=u(i1,i2,i3,uc) ! in the rectangular case just eval the solution
#End
#endMacro


! ==========================================================================================
!  Evaluate a derivative. (assumes parametric derivatives have already been evaluated)
!   DERIV   : name of the derivative. One of 
!                x,y,z,xx,xy,xz,...
!    u      : evaluate derivatives of this function.
!    uc     : component to evaluate
!    uu     : prefix for the name of the resulting derivatives (same name used with opEvalParametricDerivative) 
!    aj     : prefix for the name of the jacobian variables.
!    ud     : derivative is assigned to this variable.
! ==========================================================================================
#beginMacro getOp(DERIV, u,i1,i2,i3,uc,uu,aj,ud )

 #If $GRIDTYPE eq "curvilinear" 
  #peval getDuD ## DERIV ## $DIM(uu,aj,ud)  ! Note: The perl variables are evaluated when the macro is USED. 
 #Else
  #peval ud = u ## DERIV ## $ORDER(i1,i2,i3,uc)
 #End

#endMacro

! ******************************************************************************************************************

! loop over the boundary points
#beginMacro beginLoops(n1a,n1b,n2a,n2b,n3a,n3b,na,nb)
do i3=n3a,n3b
do i2=n2a,n2b
do i1=n1a,n1b
do n=na,nb
  ! write(*,'(" periodic i1,i2,i3,n=",4i4)') i1,i2,i3,n
#endMacro

#beginMacro endLoops()
end do
end do
end do
end do
#endMacro


! loop over the boundary points
#beginMacro beginLoops2d()
 i3=n3a
 j3=m3a

 j2=m2a
 do i2=n2a,n2b
  j1=m1a
  do i1=n1a,n1b
#endMacro
#beginMacro endLoops2d()
   j1=j1+1
  end do
  j2=j2+1
 end do
#endMacro

! loop over the boundary points with a mask. 
! Assign pts where both mask1 and mask2 are discretization pts.
! If mask1>0 and mask2<0 then we just leave the extrapolated values in u1 and u2 .
#beginMacro beginLoopsMask2d()
 i3=n3a
 j3=m3a

 j2=m2a
 do i2=n2a,n2b
  j1=m1a
  do i1=n1a,n1b
   if( mask1(i1,i2,i3).gt.0 .and. mask2(j1,j2,j3).gt.0 )then
#endMacro
#beginMacro endLoopsMask2d()
   end if
   j1=j1+1
  end do
  j2=j2+1
 end do
#endMacro

! loop over the boundary points that includes ghost points in the tangential direction
#beginMacro beginGhostLoops2d()
 i3=n3a
 j3=m3a
 j2=mm2a
 do i2=nn2a,nn2b
  j1=mm1a
  do i1=nn1a,nn1b
#endMacro

! loop over the boundary points that includes ghost points in the tangential direction.
! Assign pts where both mask1 and mask2 are discretization pts.
! If mask1>0 and mask2<0 then we just leave the extrapolated values in u1 and u2 .
#beginMacro beginGhostLoopsMask2d()
 i3=n3a
 j3=m3a
 j2=mm2a
 do i2=nn2a,nn2b
  j1=mm1a
  do i1=nn1a,nn1b
  if( mask1(i1,i2,i3).gt.0 .and. mask2(j1,j2,j3).gt.0 )then
#endMacro

#beginMacro beginLoops3d()
 j3=m3a
 do i3=n3a,n3b
 j2=m2a
 do i2=n2a,n2b
 j1=m1a
 do i1=n1a,n1b
#endMacro
#beginMacro endLoops3d()
   j1=j1+1
  end do
  j2=j2+1
 end do
  j3=j3+1
 end do
#endMacro

! Assign pts where both mask1 and mask2 are discretization pts.
! If mask1>0 and mask2<0 then we just leave the extrapolated values in u1 and u2 .
#beginMacro beginLoopsMask3d()
 j3=m3a
 do i3=n3a,n3b
 j2=m2a
 do i2=n2a,n2b
 j1=m1a
 do i1=n1a,n1b
 if( mask1(i1,i2,i3).gt.0 .and. mask2(j1,j2,j3).gt.0 )then
#endMacro
#beginMacro endLoopsMask3d()
  end if
   j1=j1+1
  end do
  j2=j2+1
 end do
  j3=j3+1
 end do
#endMacro

#beginMacro beginGhostLoops3d()
 j3=mm3a
 do i3=nn3a,nn3b
 j2=mm2a
 do i2=nn2a,nn2b
  j1=mm1a
  do i1=nn1a,nn1b
#endMacro

#beginMacro beginGhostLoopsMask3d()
 j3=mm3a
 do i3=nn3a,nn3b
 j2=mm2a
 do i2=nn2a,nn2b
  j1=mm1a
  do i1=nn1a,nn1b
  if( mask1(i1,i2,i3).gt.0 .and. mask2(j1,j2,j3).gt.0 )then
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

! This macro will assign the jump conditions on the boundary
! DIM (input): number of dimensions (2 or 3)
! GRIDTYPE (input) : curvilinear or rectangular
#beginMacro boundaryJumpConditions(DIM,GRIDTYPE)
 #If #DIM eq "2"
  if( eps1.lt.eps2 )then
    epsRatio=eps1/eps2
    beginGhostLoopsMask2d()
      ! eps2 n.u2 = eps1 n.u1
      !     tau.u2 = tau.u1

      #If #GRIDTYPE eq "curvilinear"
       an1=rsxy1(i1,i2,i3,axis1,0)   ! normal (an1,an2)
       an2=rsxy1(i1,i2,i3,axis1,1)
       aNorm=max(epsx,sqrt(an1**2+an2**2))
       an1=an1/aNorm
       an2=an2/aNorm
      #Elif #GRIDTYPE eq "rectangular"
       an1=an1Cartesian
       an2=an2Cartesian
      #Else
         stop 1111
      #End
      ua=u1(i1,i2,i3,ex)
      ub=u1(i1,i2,i3,ey)
      nDotU = an1*ua+an2*ub
      if( twilightZone.eq.1 )then
       ! adjust for TZ forcing (here we assume the exact solution is the same on both sides)
       call ogderiv(ep, 0,0,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t, ex, ue )
       call ogderiv(ep, 0,0,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t, ey, ve )
       nDotU = nDotU - (an1*ue+an2*ve)
      end if

      ! u2 equals u1 but with normal component = eps1/eps2*(n.u1)
      u2(j1,j2,j3,ex) = ua + (nDotU*epsRatio - nDotU)*an1
      u2(j1,j2,j3,ey) = ub + (nDotU*epsRatio - nDotU)*an2
      u2(j1,j2,j3,hz) = u1(i1,i2,i3,hz)


    endLoopsMask2d()
  else
    epsRatio=eps2/eps1
    beginGhostLoopsMask2d()
      ! eps2 n.u2 = eps1 n.u1
      !     tau.u2 = tau.u1

      #If #GRIDTYPE eq "curvilinear"
       an1=rsxy1(i1,i2,i3,axis1,0)
       an2=rsxy1(i1,i2,i3,axis1,1)
       aNorm=max(epsx,sqrt(an1**2+an2**2))
       an1=an1/aNorm
       an2=an2/aNorm
      #Elif #GRIDTYPE eq "rectangular"
       an1=an1Cartesian
       an2=an2Cartesian
      #Else
        stop 1112
      #End
      ua=u2(j1,j2,j3,ex)
      ub=u2(j1,j2,j3,ey)

      nDotU = an1*ua+an2*ub
      if( twilightZone.eq.1 )then
       ! adjust for TZ forcing (here we assume the exact solution is the same on both sides)
       call ogderiv(ep, 0,0,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t, ex, ue )
       call ogderiv(ep, 0,0,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t, ey, ve )
! write(*,'(" jump: x,y=",2e10.2," ua,ue=",2e10.2," ub,ve=",2e10.2)') xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),ua,ue,ub,ve
       nDotU = nDotU - (an1*ue+an2*ve)
      end if

      u1(i1,i2,i3,ex) = ua + (nDotU*epsRatio - nDotU)*an1
      u1(i1,i2,i3,ey) = ub + (nDotU*epsRatio - nDotU)*an2
      u1(i1,i2,i3,hz) = u2(j1,j2,j3,hz)
    endLoopsMask2d()
  end if
 #Else
  ! *** 3D ***
  if( eps1.lt.eps2 )then
    epsRatio=eps1/eps2
    beginGhostLoopsMask3d()
      ! eps2 n.u2 = eps1 n.u1
      !     tau.u2 = tau.u1

      #If #GRIDTYPE eq "curvilinear"
       an1=rsxy1(i1,i2,i3,axis1,0)   ! normal (an1,an2)
       an2=rsxy1(i1,i2,i3,axis1,1)
       an3=rsxy1(i1,i2,i3,axis1,2)
       aNorm=max(epsx,sqrt(an1**2+an2**2+an3**2))
       an1=an1/aNorm
       an2=an2/aNorm
       an3=an3/aNorm
      #Elif #GRIDTYPE eq "rectangular"
       an1=an1Cartesian
       an2=an2Cartesian
       an3=an3Cartesian
      #Else
         stop 1111
      #End
      ua=u1(i1,i2,i3,ex)
      ub=u1(i1,i2,i3,ey)
      uc=u1(i1,i2,i3,ez)
      nDotU = an1*ua+an2*ub+an3*uc
      if( twilightZone.eq.1 )then
       ! adjust for TZ forcing (here we assume the exact solution is the same on both sides)
       call ogderiv(ep, 0,0,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),xy1(i1,i2,i3,2),t, ex, ue )
       call ogderiv(ep, 0,0,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),xy1(i1,i2,i3,2),t, ey, ve )
       call ogderiv(ep, 0,0,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),xy1(i1,i2,i3,2),t, ez, we )
       nDotU = nDotU - (an1*ue+an2*ve+an3*we)
      end if
      ! u2 equals u1 but with normal component = eps1/eps2*(n.u1)
      u2(j1,j2,j3,ex) = ua + (nDotU*epsRatio - nDotU)*an1
      u2(j1,j2,j3,ey) = ub + (nDotU*epsRatio - nDotU)*an2
      u2(j1,j2,j3,ez) = uc + (nDotU*epsRatio - nDotU)*an3

!   write(*,'(" jump(1): (i1,i2,i3)=",3i3," j1,j2,j3=",3i3)') i1,i2,i3,j1,j2,j3
!   write(*,'(" jump(1): x,y,z=",3e10.2," ua,ue=",2e10.2," ub,ve=",2e10.2," uc,we=",2e10.2)') xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),xy1(i1,i2,i3,2),ua,ue,ub,ve,uc,we

    endLoopsMask3d()
  else
    epsRatio=eps2/eps1
    beginGhostLoopsMask3d()
      ! eps2 n.u2 = eps1 n.u1
      !     tau.u2 = tau.u1

      #If #GRIDTYPE eq "curvilinear"
       an1=rsxy1(i1,i2,i3,axis1,0)   ! normal (an1,an2)
       an2=rsxy1(i1,i2,i3,axis1,1)
       an3=rsxy1(i1,i2,i3,axis1,2)
       aNorm=max(epsx,sqrt(an1**2+an2**2+an3**2))
       an1=an1/aNorm
       an2=an2/aNorm
       an3=an3/aNorm
      #Elif #GRIDTYPE eq "rectangular"
       an1=an1Cartesian
       an2=an2Cartesian
       an3=an3Cartesian
      #Else
         stop 1111
      #End
      ua=u2(j1,j2,j3,ex)
      ub=u2(j1,j2,j3,ey)
      uc=u2(j1,j2,j3,ez)

      nDotU = an1*ua+an2*ub+an3*uc
      if( twilightZone.eq.1 )then
       ! adjust for TZ forcing (here we assume the exact solution is the same on both sides)
       call ogderiv(ep, 0,0,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),xy1(i1,i2,i3,2),t, ex, ue )
       call ogderiv(ep, 0,0,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),xy1(i1,i2,i3,2),t, ey, ve )
       call ogderiv(ep, 0,0,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),xy1(i1,i2,i3,2),t, ez, we )
       nDotU = nDotU - (an1*ue+an2*ve+an3*we)

!   write(*,'(" jump(2): (i1,i2,i3)=",3i3," j1,j2,j3=",3i3)') i1,i2,i3,j1,j2,j3
!   write(*,'(" jump(2): x,y,z=",3e10.2," ua,ue=",2e10.2," ub,ve=",2e10.2," uc,we=",2e10.2)') xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),xy1(i1,i2,i3,2),ua,ue,ub,ve,uc,we

      end if

      u1(i1,i2,i3,ex) = ua + (nDotU*epsRatio - nDotU)*an1
      u1(i1,i2,i3,ey) = ub + (nDotU*epsRatio - nDotU)*an2
      u1(i1,i2,i3,ez) = uc + (nDotU*epsRatio - nDotU)*an3
    endLoopsMask3d()
  end if
 #End
#endMacro

! ** Precompute the derivatives of rsxy ***
! assign rvx(m) = (rx,sy)
!        rvxx(m) = (rxx,sxx)
!* #beginMacro computeRxDerivatives(rv,rsxy,i1,i2,i3)
!* do m=0,nd-1
!*  rv ## x(m)   =rsxy(i1,i2,i3,m,0)
!*  rv ## y(m)   =rsxy(i1,i2,i3,m,1)
!* 
!*  rv ## xx(m)  =rsxy ## x22(i1,i2,i3,m,0)
!*  rv ## xy(m)  =rsxy ## x22(i1,i2,i3,m,1)
!*  rv ## yy(m)  =rsxy ## y22(i1,i2,i3,m,1)
!* 
!*  rv ## xxx(m) =rsxy ## xx22(i1,i2,i3,m,0)
!*  rv ## xxy(m) =rsxy ## xx22(i1,i2,i3,m,1)
!*  rv ## xyy(m) =rsxy ## xy22(i1,i2,i3,m,1)
!*  rv ## yyy(m) =rsxy ## yy22(i1,i2,i3,m,1)
!* 
!*  rv ## xxxx(m)=rsxy ## xxx22(i1,i2,i3,m,0)
!*  rv ## xxyy(m)=rsxy ## xyy22(i1,i2,i3,m,0)
!*  rv ## yyyy(m)=rsxy ## yyy22(i1,i2,i3,m,1)
!* end do
!* #endMacro
!* 
!* c assign some temporary variables that are used in the evaluation of the operators
!* #beginMacro setJacobian(rv,axis1,axisp1)
!*  rx   =rv ## x(axis1)   
!*  ry   =rv ## y(axis1)   
!*                     
!*  rxx  =rv ## xx(axis1)  
!*  rxy  =rv ## xy(axis1)  
!*  ryy  =rv ## yy(axis1)  
!*                     
!*  rxxx =rv ## xxx(axis1) 
!*  rxxy =rv ## xxy(axis1) 
!*  rxyy =rv ## xyy(axis1) 
!*  ryyy =rv ## yyy(axis1) 
!*                     
!*  rxxxx=rv ## xxxx(axis1)
!*  rxxyy=rv ## xxyy(axis1)
!*  ryyyy=rv ## yyyy(axis1)
!* 
!*  sx   =rv ## x(axis1p1)   
!*  sy   =rv ## y(axis1p1)   
!*                     
!*  sxx  =rv ## xx(axis1p1)  
!*  sxy  =rv ## xy(axis1p1)  
!*  syy  =rv ## yy(axis1p1)  
!*                     
!*  sxxx =rv ## xxx(axis1p1) 
!*  sxxy =rv ## xxy(axis1p1) 
!*  sxyy =rv ## xyy(axis1p1) 
!*  syyy =rv ## yyy(axis1p1) 
!*                     
!*  sxxxx=rv ## xxxx(axis1p1)
!*  sxxyy=rv ## xxyy(axis1p1)
!*  syyyy=rv ## yyyy(axis1p1)
!* 
!* #endMacro

! ********************************************************************************
!     Usage: setJacobianRS( aj1, r, s)
!            setJacobianRS( aj1, s, r)
! ********************************************************************************
#beginMacro setJacobianRS(aj, R, S)
 rx   =aj ## R ## x
 ry   =aj ## R ## y
                    
 rxx  =aj ## R ## xx  
 rxy  =aj ## R ## xy  
 ryy  =aj ## R ## yy  
                    
 rxxx =aj ## R ## xxx 
 rxxy =aj ## R ## xxy 
 rxyy =aj ## R ## xyy 
 ryyy =aj ## R ## yyy 
                    
 rxxxx=aj ## R ## xxxx
 rxxyy=aj ## R ## xxyy
 ryyyy=aj ## R ## yyyy

 sx   =aj ## S ## x   
 sy   =aj ## S ## y   
                    
 sxx  =aj ## S ## xx  
 sxy  =aj ## S ## xy  
 syy  =aj ## S ## yy  
                    
 sxxx =aj ## S ## xxx 
 sxxy =aj ## S ## xxy 
 sxyy =aj ## S ## xyy 
 syyy =aj ## S ## yyy 
                    
 sxxxx=aj ## S ## xxxx
 sxxyy=aj ## S ## xxyy
 syyyy=aj ## S ## yyyy

#endMacro

! ***************************************************************************
! This macro will set the temp variables rx, rxx, ry, ryx, ...
! If axis=0 then
!   rx = ajrx
!   sx = ajsx
!    ...
!  else if axis=1
!    -- permute r <-> s 
!   rx = ajsx
!   sx = ajrx
!    ...
! ***************************************************************************
#beginMacro setJacobian(aj, axis)
if( axis.eq.0 )then
 setJacobianRS( aj, r, s)
else
 setJacobianRS( aj, s, r)
end if

#endMacro

! ===================================================================================
!  Optimized periodic update: (only applied in serial)
!     update the periodic ghost points used by an interface on the grid face (side,axis)
! ===================================================================================
#beginMacro periodicUpdate2d(u,bc,gid,side,axis)
if( parallel.eq.0 )then
 axisp1=mod(axis+1,nd)
 if( bc(0,axisp1).lt.0 )then
  ! direction axisp1 is periodic
  diff(axis)=0
  diff(axisp1)=gid(1,axisp1)-gid(0,axisp1)

  if( side.eq.0 )then
    ! assign 4 ghost points outside lower corner
    np1a=gid(0,0)-2
    np1b=gid(0,0)-1
    np2a=gid(0,1)-2
    np2b=gid(0,1)-1

    beginLoops(np1a,np1b,np2a,np2b,n3a,n3b,ex,hz)
     u(i1,i2,i3,n) = u(i1+diff(0),i2+diff(1),i3,n)
    endLoops()

    ! assign 4 ghost points outside upper corner
    if( axis.eq.0 )then
      np2a=gid(1,axisp1)+1
      np2b=gid(1,axisp1)+2
    else
      np1a=gid(1,axisp1)+1
      np1b=gid(1,axisp1)+2
    end if

    beginLoops(np1a,np1b,np2a,np2b,n3a,n3b,ex,hz)
     u(i1,i2,i3,n) = u(i1-diff(0),i2-diff(1),i3,n)
    endLoops()

  else

    ! assign 4 ghost points outside upper corner
    np1a=gid(1,0)+1
    np1b=gid(1,0)+2
    np2a=gid(1,1)+1
    np2b=gid(1,1)+2

    beginLoops(np1a,np1b,np2a,np2b,n3a,n3b,ex,hz)
     u(i1,i2,i3,n) = u(i1-diff(0),i2-diff(1),i3,n)
    endLoops()

    if( axis.eq.0 )then
      np2a=gid(0,axisp1)-2
      np2b=gid(0,axisp1)-1
    else
      np1a=gid(0,axisp1)-2
      np1b=gid(0,axisp1)-1
    end if

    beginLoops(np1a,np1b,np2a,np2b,n3a,n3b,ex,hz)
     u(i1,i2,i3,n) = u(i1+diff(0),i2+diff(1),i3,n)
    endLoops()
  end if

 endif
end if

#endMacro

! ===================================================================================
!  Optimized periodic update:
!     update the periodic ghost points used by an interface on the grid face (side,axis)
! ===================================================================================
#beginMacro periodicUpdate3d(u,bc,gid,side,axis)
if( parallel.eq.0 )then
 axisp1=mod(axis+1,nd)
 axisp2=mod(axis+2,nd)
 if( bc(0,axisp1).lt.0 .or. bc(0,axisp2).lt.0 )then
  ! We assume this is done by the calling program
  ! write(*,'("periodicUpdate3d: finish me")')
  ! stop 
 end if
end if
#endMacro


! ******************************************************************************
!   This next macro is called by other macros to evaluate the first and second derivatives
!   This macro assumes that opEvalJacobianDerivatives has been called
! ******************************************************************************
#beginMacro evalSecondDerivs(rsxy1,aj1,u1,i1,i2,i3,ex,uu1,uu)
 opEvalParametricDerivative(u1,i1,i2,i3,ex,uu1,2)    ! computes uu1r, uu1s 
 getOp(x ,u1,i1,i2,i3,ex,uu1,aj1,uu ## x)            ! u1.x
 getOp(y ,u1,i1,i2,i3,ex,uu1,aj1,uu ## y)            ! u1.y
 getOp(xx,u1,i1,i2,i3,ex,uu1,aj1,uu ## xx)
 getOp(yy,u1,i1,i2,i3,ex,uu1,aj1,uu ## yy)
 uu ## Lap = uu ## xx+ uu ## yy
#endMacro 


! *********************************************************************************
!   Evaluate derivatives for the 2nd-order 2D interface equations
! *********************************************************************************
#beginMacro evalInterfaceDerivatives2d()
 ! NOTE: the jacobian derivatives can be computed once for all components
 opEvalJacobianDerivatives(rsxy1,i1,i2,i3,aj1,1)
 evalSecondDerivs(rsxy1,aj1,u1,i1,i2,i3,ex,uu1,u1)
 evalSecondDerivs(rsxy1,aj1,u1,i1,i2,i3,ey,vv1,v1)

 ! NOTE: the jacobian derivatives can be computed once for all components
 opEvalJacobianDerivatives(rsxy2,j1,j2,j3,aj2,1)
 evalSecondDerivs(rsxy2,aj2,u2,j1,j2,j3,ex,uu2,u2)
 evalSecondDerivs(rsxy2,aj2,u2,j1,j2,j3,ey,vv2,v2)
#endMacro

! ******************************************************************************
!   This macro assumes that opEvalJacobianDerivatives has been called
! ******************************************************************************
#beginMacro evalMagneticFieldInterfaceDerivatives2d()
 evalSecondDerivs(rsxy1,aj1,u1,i1,i2,i3,hz,ww1,w1)
 evalSecondDerivs(rsxy2,aj2,u2,j1,j2,j3,hz,ww2,w2)
#endMacro


! ******************************************************************************
!   This next macro is called by evalDerivs2dOrder4
!   This macro assumes that opEvalJacobianDerivatives has been called
! ******************************************************************************
#beginMacro evalFourthDerivs(rsxy1,aj1,u1,i1,i2,i3,ex,uu1,uu)
 opEvalParametricDerivative(u1,i1,i2,i3,ex,uu1,4)    ! computes uu1r, uu1s 
 getOp(xxx ,u1,i1,i2,i3,ex,uu1,aj1,uu ## xxx)       ! u1.xxx
 getOp(xxy ,u1,i1,i2,i3,ex,uu1,aj1,uu ## xxy)       ! u1.xxy
 getOp(xyy ,u1,i1,i2,i3,ex,uu1,aj1,uu ## xyy) 
 getOp(yyy ,u1,i1,i2,i3,ex,uu1,aj1,uu ## yyy) 
 getOp(xxxx,u1,i1,i2,i3,ex,uu1,aj1,uu ## xxxx) 
 getOp(xxyy,u1,i1,i2,i3,ex,uu1,aj1,uu ## xxyy) 
 getOp(yyyy,u1,i1,i2,i3,ex,uu1,aj1,uu ## yyyy) 
 uu ## LapSq = uu ## xxxx +2.* uu ## xxyy + uu ## yyyy
#endMacro 

! ******************************************************************************
!   Evaluate derivatives for the 4th-order 2D interface equations
! ******************************************************************************
#beginMacro evalDerivs2dOrder4()

#perl $ORDER=2;
 ! These derivatives are computed to 2nd-order accuracy

 ! NOTE: the jacobian derivatives can be computed once for all components
 opEvalJacobianDerivatives(rsxy1,i1,i2,i3,aj1,3)
 evalFourthDerivs(rsxy1,aj1,u1,i1,i2,i3,ex,uu1,u1)
 evalFourthDerivs(rsxy1,aj1,u1,i1,i2,i3,ey,vv1,v1)

 ! NOTE: the jacobian derivatives can be computed once for all components
 opEvalJacobianDerivatives(rsxy2,j1,j2,j3,aj2,3)
 evalFourthDerivs(rsxy2,aj2,u2,j1,j2,j3,ex,uu2,u2)
 evalFourthDerivs(rsxy2,aj2,u2,j1,j2,j3,ey,vv2,v2)

#perl $ORDER=4;
 ! These derivatives are computed to 4th-order accuracy

 ! NOTE: the jacobian derivatives can be computed once for all components
 opEvalJacobianDerivatives(rsxy1,i1,i2,i3,aj1,1)
 evalSecondDerivs(rsxy1,aj1,u1,i1,i2,i3,ex,uu1,u1)
 evalSecondDerivs(rsxy1,aj1,u1,i1,i2,i3,ey,vv1,v1)

 ! NOTE: the jacobian derivatives can be computed once for all components
 opEvalJacobianDerivatives(rsxy2,j1,j2,j3,aj2,1)
 evalSecondDerivs(rsxy2,aj2,u2,j1,j2,j3,ex,uu2,u2)
 evalSecondDerivs(rsxy2,aj2,u2,j1,j2,j3,ey,vv2,v2)

#endMacro

! ******************************************************************************
!   Evaluate derivatives of the magnetic field for the 4th-order 2D interface equations
! ******************************************************************************
#beginMacro evalMagneticDerivs2dOrder4()

#perl $ORDER=4;
 ! These derivatives are computed to 4th-order accuracy
 opEvalJacobianDerivatives(rsxy1,i1,i2,i3,aj1,1)
 evalSecondDerivs(rsxy1,aj1,u1,i1,i2,i3,hz,ww1,w1)

 opEvalJacobianDerivatives(rsxy2,j1,j2,j3,aj2,1)
 evalSecondDerivs(rsxy2,aj2,u2,j1,j2,j3,hz,ww2,w2)

#perl $ORDER=2;
 ! These derivatives are computed to 2nd-order accuracy
 opEvalJacobianDerivatives(rsxy1,i1,i2,i3,aj1,1)
 evalFourthDerivs(rsxy1,aj1,u1,i1,i2,i3,hz,ww1,w1)

 opEvalJacobianDerivatives(rsxy2,j1,j2,j3,aj2,1)
 evalFourthDerivs(rsxy2,aj2,u2,j1,j2,j3,hz,ww2,w2)

#endMacro

! ******************************************************************************
!   This next macro is called by other macros to evaluate the first and second derivatives
!   This macro assumes that opEvalJacobianDerivatives has been called
! ******************************************************************************
#beginMacro evalSecondDerivs3d(rsxy1,aj1,u1,i1,i2,i3,ex,uu1,uu)
 opEvalParametricDerivative(u1,i1,i2,i3,ex,uu1,2)    ! computes uu1r, uu1s 
 getOp(x ,u1,i1,i2,i3,ex,uu1,aj1,uu ## x)            ! u1.x
 getOp(y ,u1,i1,i2,i3,ex,uu1,aj1,uu ## y)            ! u1.y
 getOp(z ,u1,i1,i2,i3,ex,uu1,aj1,uu ## z)            ! u1.z
 getOp(xx,u1,i1,i2,i3,ex,uu1,aj1,uu ## xx)
 getOp(yy,u1,i1,i2,i3,ex,uu1,aj1,uu ## yy)
 getOp(zz,u1,i1,i2,i3,ex,uu1,aj1,uu ## zz)
 uu ## Lap = uu ## xx+ uu ## yy+ uu ## zz
#endMacro 


! *********************************************************************************
!   Evaluate derivatives for the 2nd-order 3D interface equations
! *********************************************************************************
#beginMacro evalInterfaceDerivatives3d()
 ! NOTE: the jacobian derivatives can be computed once for all components
 opEvalJacobianDerivatives(rsxy1,i1,i2,i3,aj1,1)
 evalSecondDerivs3d(rsxy1,aj1,u1,i1,i2,i3,ex,uu1,u1)
 evalSecondDerivs3d(rsxy1,aj1,u1,i1,i2,i3,ey,vv1,v1)
 evalSecondDerivs3d(rsxy1,aj1,u1,i1,i2,i3,ez,ww1,w1)

 ! NOTE: the jacobian derivatives can be computed once for all components
 opEvalJacobianDerivatives(rsxy2,j1,j2,j3,aj2,1)
 evalSecondDerivs3d(rsxy2,aj2,u2,j1,j2,j3,ex,uu2,u2)
 evalSecondDerivs3d(rsxy2,aj2,u2,j1,j2,j3,ey,vv2,v2)
 evalSecondDerivs3d(rsxy2,aj2,u2,j1,j2,j3,ez,ww2,w2)
#endMacro



      subroutine interface3dMaxwell( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
                               gridIndexRange1, u1, mask1,rsxy1, xy1, boundaryCondition1, \
                               md1a,md1b,md2a,md2b,md3a,md3b,\
                               gridIndexRange2, u2, mask2,rsxy2, xy2, boundaryCondition2, \
                               ipar, rpar, \
                               aa2,aa4,aa8, ipvt2,ipvt4,ipvt8, \
                               ierr )
! ===================================================================================
!  Interface boundary conditions for Maxwells Equations in 3D.
!
!  gridType : 0=rectangular, 1=curvilinear
!
!  u1: solution on the "left" of the interface
!  u2: solution on the "right" of the interface
!
!  aa2,aa4,aa8 : real work space arrays that must be saved from call to call
!  ipvt2,ipvt4,ipvt8: integer work space arrays that must be saved from call to call
! ===================================================================================

      implicit none

      integer nd, \
              nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, \
              md1a,md1b,md2a,md2b,md3a,md3b, \
              n1a,n1b,n2a,n2b,n3a,n3b,  \
              m1a,m1b,m2a,m2b,m3a,m3b,  \
              ierr

      real u1(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      integer mask1(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real rsxy1(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real xy1(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      integer gridIndexRange1(0:1,0:2),boundaryCondition1(0:1,0:2)

      real u2(md1a:md1b,md2a:md2b,md3a:md3b,0:*)
      integer mask2(md1a:md1b,md2a:md2b,md3a:md3b)
      real rsxy2(md1a:md1b,md2a:md2b,md3a:md3b,0:nd-1,0:nd-1)
      real xy2(md1a:md1b,md2a:md2b,md3a:md3b,0:nd-1)
      integer gridIndexRange2(0:1,0:2),boundaryCondition2(0:1,0:2)

      integer ipar(0:*)
      real rpar(0:*)

      ! work space arrays that must be saved from call to call:
      real aa2(0:1,0:1,0:1,0:*),aa4(0:3,0:3,0:1,0:*),aa8(0:7,0:7,0:1,0:*)
      integer ipvt2(0:1,0:*), ipvt4(0:3,0:*), ipvt8(0:7,0:*)

!     --- local variables ----
      
      integer side1,axis1,grid1,side2,axis2,grid2,gridType,orderOfAccuracy,orderOfExtrapolation,useForcing,\
        ex,ey,ez,hx,hy,hz,useWhereMask,debug,solveForE,solveForH,axis1p1,axis1p2,axis2p1,axis2p2,nn,n1,n2,\
        twilightZone
      real dx1(0:2),dr1(0:2),dx2(0:2),dr2(0:2)
!      real dx(0:2),dr(0:2)
      real t,ep,dt,eps1,mu1,c1,eps2,mu2,c2,epsmu1,epsmu2
      integer axisp1,axisp2,i1,i2,i3,is1,is2,is3,j1,j2,j3,js1,js2,js3,ks1,ks2,ks3,is,js,it,nit,k1,k2,k3
      integer interfaceOption,interfaceEquationsOption,initialized,forcingOption

      integer numGhost,giveDiv
      integer nn1a,nn1b,nn2a,nn2b,nn3a,nn3b
      integer mm1a,mm1b,mm2a,mm2b,mm3a,mm3b
      integer m1,m2

      real rx1,ry1,rz1,rx2,ry2,rz2

      real aLap0,aLap1,bLap0,bLap1,aLapX0,aLapX1,bLapY0,bLapY1,cLapX0,cLapX1,dLapY0,dLapY1,aLapSq0,aLapSq1,bLapSq0,bLapSq1
      real a0,a1,b0,b1,cc0,cc1,d0,d1,dr0,ds0
      real aNormSq,divu

      real epsRatio,an1,an2,an3,aNorm,ua,ub,uc,nDotU
      real epsx

      real tau1,tau2,clap1,clap2,u1Lap,v1Lap,w1Lap,u2Lap,v2Lap,w2Lap,an1Cartesian,an2Cartesian,an3Cartesian
      real u1LapSq,v1LapSq,u2LapSq,v2LapSq,w1LapSq,w2LapSq


      integer np1a,np1b,np2a,np2b,np3a,np3b,diff(0:2)

      real rx,ry,rxx,rxy,ryy,rxxx,rxxy,rxyy,ryyy,rxxxx,rxxyy,ryyyy
      real sx,sy,sxx,sxy,syy,sxxx,sxxy,sxyy,syyy,sxxxx,sxxyy,syyyy

!     real rv1x(0:2),rv1y(0:2),rv1xx(0:2),rv1xy(0:2),rv1yy(0:2),rv1xxx(0:2),rv1xxy(0:2),rv1xyy(0:2),rv1yyy(0:2),\
!          rv1xxxx(0:2),rv1xxyy(0:2),rv1yyyy(0:2)
!     real sv1x(0:2),sv1y(0:2),sv1xx(0:2),sv1xy(0:2),sv1yy(0:2),sv1xxx(0:2),sv1xxy(0:2),sv1xyy(0:2),sv1yyy(0:2),\
!          sv1xxxx(0:2),sv1xxyy(0:2),sv1yyyy(0:2)
!     real rv2x(0:2),rv2y(0:2),rv2xx(0:2),rv2xy(0:2),rv2yy(0:2),rv2xxx(0:2),rv2xxy(0:2),rv2xyy(0:2),rv2yyy(0:2),\
!          rv2xxxx(0:2),rv2xxyy(0:2),rv2yyyy(0:2)
!     real sv2x(0:2),sv2y(0:2),sv2xx(0:2),sv2xy(0:2),sv2yy(0:2),sv2xxx(0:2),sv2xxy(0:2),sv2xyy(0:2),sv2yyy(0:2),\
!          sv2xxxx(0:2),sv2xxyy(0:2),sv2yyyy(0:2)

      integer numberOfEquations,job
      real a2(0:1,0:1),a4(0:3,0:3),a6(0:5,0:5),a8(0:7,0:7),a12(0:11,0:11),q(0:11),f(0:11),rcond,work(0:11)
      integer ipvt(0:11)

      real err
      integer debugFile,myid,parallel
      character*20 debugFileName

      ! for new evaluation method:
      real u1x,u1y,u1z,u1xx,u1xy,u1yy,u1xz,u1yz,u1zz
      real u2x,u2y,u2z,u2xx,u2xy,u2yy,u2xz,u2yz,u2zz

      real v1x,v1y,v1z,v1xx,v1xy,v1yy,v1xz,v1yz,v1zz
      real v2x,v2y,v2z,v2xx,v2xy,v2yy,v2xz,v2yz,v2zz

      real w1x,w1y,w1z,w1xx,w1xy,w1yy,w1xz,w1yz,w1zz
      real w2x,w2y,w2z,w2xx,w2xy,w2yy,w2xz,w2yz,w2zz

      real u1xxx,u1xxy,u1xyy,u1yyy, u1xxz,u1xzz,u1zzz, u1yyz, u1yzz
      real u2xxx,u2xxy,u2xyy,u2yyy, u2xxz,u2xzz,u2zzz, u2yyz, u2yzz
      real v1xxx,v1xxy,v1xyy,v1yyy, v1xxz,v1xzz,v1zzz, v1yyz, v1yzz
      real v2xxx,v2xxy,v2xyy,v2yyy, v2xxz,v2xzz,v2zzz, v2yyz, v2yzz
      real w1xxx,w1xxy,w1xyy,w1yyy, w1xxz,w1xzz,w1zzz, w1yyz, w1yzz
      real w2xxx,w2xxy,w2xyy,w2yyy, w2xxz,w2xzz,w2zzz, w2yyz, w2yzz

      real u1xxxx,u1xxyy,u1yyyy, u1xxzz,u1zzzz, u1yyzz
      real u2xxxx,u2xxyy,u2yyyy, u2xxzz,u2zzzz, u2yyzz
      real v1xxxx,v1xxyy,v1yyyy, v1xxzz,v1zzzz, v1yyzz
      real v2xxxx,v2xxyy,v2yyyy, v2xxzz,v2zzzz, v2yyzz
      real w1xxxx,w1xxyy,w1yyyy, w1xxzz,w1zzzz, w1yyzz
      real w2xxxx,w2xxyy,w2yyyy, w2xxzz,w2zzzz, w2yyzz

      real rxx1(0:2,0:2,0:2), rxx2(0:2,0:2,0:2)

      real dx112(0:2),dx122(0:2),dx212(0:2),dx222(0:2),dx141(0:2),dx142(0:2),dx241(0:2),dx242(0:2)
      real dr114(0:2),dr214(0:2)

      real cem1,divE1,curlE1x,curlE1y,curlE1z,nDotCurlE1,nDotLapE1
      real cem2,divE2,curlE2x,curlE2y,curlE2z,nDotCurlE2,nDotLapE2
      real c1x,c1y,c1z
      real c2x,c2y,c2z

      ! these are for the exact solution from TZ flow: 
      real ue,ve,we
      real uex,uey,uez, vex,vey,vez, wex,wey,wez, hex,hey,hez
      real uexx,ueyy,uezz, vexx,veyy,vezz, wexx,weyy,wezz
      real ueLap, veLap, weLap
      real curlEex,curlEey,curlEez,nDotCurlEe,nDotLapEe
      real uexxx,uexxy,uexyy,ueyyy
      real vexxx,vexxy,vexyy,veyyy
      real wexxx,wexxy,wexyy,weyyy
      real uexxxx,uexxyy,ueyyyy,ueLapSq
      real vexxxx,vexxyy,veyyyy,veLapSq
      real wexxxx,wexxyy,weyyyy,weLapSq

      ! boundary conditions parameters
      #Include "bcDefineFortranInclude.h"
 
      integer rectangular,curvilinear
      parameter(\
        rectangular=0,\
        curvilinear=1)


!     --- start statement function ----
      integer kd,m,n
!     real rx,ry,rz,sx,sy,sz,tx,ty,tz
!*      declareDifferenceNewOrder2(u1,rsxy1,dr1,dx1,RX)
!*      declareDifferenceNewOrder2(u2,rsxy2,dr2,dx2,RX)

!*      declareDifferenceNewOrder4(u1,rsxy1,dr1,dx1,RX)
!*      declareDifferenceNewOrder4(u2,rsxy2,dr2,dx2,RX)

!.......statement functions for jacobian
!     rx(i1,i2,i3)=rsxy1(i1,i2,i3,0,0)
!     ry(i1,i2,i3)=rsxy1(i1,i2,i3,0,1)
!     rz(i1,i2,i3)=rsxy1(i1,i2,i3,0,2)
!     sx(i1,i2,i3)=rsxy1(i1,i2,i3,1,0)
!     sy(i1,i2,i3)=rsxy1(i1,i2,i3,1,1)
!     sz(i1,i2,i3)=rsxy1(i1,i2,i3,1,2)
!     tx(i1,i2,i3)=rsxy1(i1,i2,i3,2,0)
!     ty(i1,i2,i3)=rsxy1(i1,i2,i3,2,1)
!     tz(i1,i2,i3)=rsxy1(i1,i2,i3,2,2) 


!     The next macro call will define the difference approximation statement functions
!*      defineDifferenceNewOrder2Components1(u1,rsxy1,dr1,dx1,RX)
!*      defineDifferenceNewOrder2Components1(u2,rsxy2,dr2,dx2,RX)

!*      defineDifferenceNewOrder4Components1(u1,rsxy1,dr1,dx1,RX)
!*      defineDifferenceNewOrder4Components1(u2,rsxy2,dr2,dx2,RX)

      declareTemporaryVariables(2,2)
      declareParametricDerivativeVariables(uu1,3)   ! declare temp variables uu, uur, uus, ...
      declareParametricDerivativeVariables(uu2,3) 
      declareParametricDerivativeVariables(vv1,3)   ! declare temp variables uu, uur, uus, ...
      declareParametricDerivativeVariables(vv2,3) 
      declareParametricDerivativeVariables(ww1,3)   ! declare temp variables uu, uur, uus, ...
      declareParametricDerivativeVariables(ww2,3) 
      declareJacobianDerivativeVariables(aj1,3)     ! declareJacobianDerivativeVariables(aj,DIM)
      declareJacobianDerivativeVariables(aj2,3)     ! declareJacobianDerivativeVariables(aj,DIM)

!............... end statement functions

      ierr=0

      side1                =ipar(0)
      axis1                =ipar(1)
      grid1                =ipar(2)
      n1a                  =ipar(3)
      n1b                  =ipar(4)
      n2a                  =ipar(5)
      n2b                  =ipar(6)
      n3a                  =ipar(7)
      n3b                  =ipar(8)

      side2                =ipar(9)
      axis2                =ipar(10)
      grid2                =ipar(11)
      m1a                  =ipar(12)
      m1b                  =ipar(13)
      m2a                  =ipar(14)
      m2b                  =ipar(15)
      m3a                  =ipar(16)
      m3b                  =ipar(17)

      gridType             =ipar(18)
      orderOfAccuracy      =ipar(19)
      orderOfExtrapolation =ipar(20)  ! maximum allowable order of extrapolation
      useForcing           =ipar(21)
      ex                   =ipar(22)
      ey                   =ipar(23)
      ez                   =ipar(24)
      hx                   =ipar(25)
      hy                   =ipar(26)
      hz                   =ipar(27)
      solveForE            =ipar(28)
      solveForH            =ipar(29)
      useWhereMask         =ipar(30)
      debug                =ipar(31)
      nit                  =ipar(32)
      interfaceOption      =ipar(33)
      initialized          =ipar(34)
      myid                 =ipar(35)
      parallel             =ipar(36)
      forcingOption        =ipar(37)
      interfaceEquationsOption=ipar(38)
     
      dx1(0)                =rpar(0)
      dx1(1)                =rpar(1)
      dx1(2)                =rpar(2)
      dr1(0)                =rpar(3)
      dr1(1)                =rpar(4)
      dr1(2)                =rpar(5)

      dx2(0)                =rpar(6)
      dx2(1)                =rpar(7)
      dx2(2)                =rpar(8)
      dr2(0)                =rpar(9)
      dr2(1)                =rpar(10)
      dr2(2)                =rpar(11)

      t                    =rpar(12)
      ep                   =rpar(13) ! pointer for exact solution
      dt                   =rpar(14)
      eps1                 =rpar(15)
      mu1                  =rpar(16)
      c1                   =rpar(17)
      eps2                 =rpar(18)
      mu2                  =rpar(19)
      c2                   =rpar(20)
     
      epsmu1=eps1*mu1
      epsmu2=eps2*mu2

      twilightZone=useForcing

      debugFile=10
      if( initialized.eq.0 .and. debug.gt.0 )then
        ! open debug files
        ! open (debugFile,file=filen,status='unknown',form='formatted')
        if( myid.lt.10 )then
          write(debugFileName,'("mxi",i1,".fdebug")') myid
        else
          write(debugFileName,'("mxi",i2,".fdebug")') myid
        end if
        write(*,*) 'interface3d: myid=',myid,' open debug file:',debugFileName
        open (debugFile,file=debugFileName,status='unknown',form='formatted')
        ! '
        ! INQUIRE(FILE=filen, EXIST=filex)
      end if

      if( t.lt.dt )then
        write(debugFile,'(" +++++++++cgmx interface3d t=",e9.2," ++++++++")') t
           ! '
        write(debugFile,'(" interface3d new: nd=",i2," gridType=",i2)') nd,gridType
      end if

      if( abs(c1*c1-1./(mu1*eps1)).gt. 1.e-10 )then
        write(debugFile,'(" interface3d:ERROR: c1,eps1,mu1=",3e10.2," not consistent")') c1,eps1,mu1
           ! '
        stop 11
      end if
      if( abs(c2*c2-1./(mu2*eps2)).gt. 1.e-10 )then
        write(debugFile,'(" interface3d:ERROR: c2,eps2,mu2=",3e10.2," not consistent")') c2,eps2,mu2
           ! '
        stop 11
      end if

      if( .false. )then
        write(debugFile,'(" interface3d: eps1,eps2=",2f10.5," c1,c2=",2f10.5)') eps1,eps2,c1,c2
           ! '
      end if

      if( nit.lt.0 .or. nit.gt.100 )then
        write(debugFile,'(" interfaceBC: ERROR: nit=",i9)') nit
        nit=max(1,min(100,nit))
      end if

      if( debug.gt.1 )then
        write(debugFile,'("********************************************************************** ")')
        write(debugFile,'(" interface3d: **START** t=",e10.2)') t
        write(debugFile,'(" interface3d: **START** grid1=",i4," side1,axis1=",2i2," bc=",6i3)') grid1,side1,axis1,\
           boundaryCondition1(0,0),boundaryCondition1(1,0),boundaryCondition1(0,1),boundaryCondition1(1,1),boundaryCondition1(0,2),boundaryCondition1(1,2)
           ! '
        write(debugFile,'(" interface3d: **START** grid2=",i4," side2,axis2=",2i2," bc=",6i3)') grid2,side2,axis2,\
           boundaryCondition2(0,0),boundaryCondition2(1,0),boundaryCondition2(0,1),boundaryCondition2(1,1),boundaryCondition2(0,2),boundaryCondition2(1,2)
           ! '
        write(debugFile,'("n1a,n1b,...=",6i5)') n1a,n1b,n2a,n2b,n3a,n3b
        write(debugFile,'("m1a,m1b,...=",6i5)') m1a,m1b,m2a,m2b,m3a,m3b

      end if
      if( debug.gt.8 )then
       write(debugFile,'("start u1=",(3i4,1x,3e11.2))') (((i1,i2,i3,(u1(i1,i2,i3,m),m=0,2),i1=nd1a,nd1b),i2=nd2a,nd2b),i3=nd3a,nd3b)
       write(debugFile,'("start u2=",(3i4,1x,3e11.2))') (((i1,i2,i3,(u2(i1,i2,i3,m),m=0,2),i1=md1a,md1b),i2=md2a,md2b),i3=md3a,md3b)
      end if
     



      ! *** do this for now --- assume grids have equal spacing
!      dx(0)=dx1(0)
!      dx(1)=dx1(1)
!      dx(2)=dx1(2)

!      dr(0)=dr1(0)
!      dr(1)=dr1(1)
!      dr(2)=dr1(2)

      epsx=1.e-20  ! fix this 

      do kd=0,nd-1
       dx112(kd) = 1./(2.*dx1(kd))
       dx122(kd) = 1./(dx1(kd)**2)
       dx212(kd) = 1./(2.*dx2(kd))
       dx222(kd) = 1./(dx2(kd)**2)

       dx141(kd) = 1./(12.*dx1(kd))
       dx142(kd) = 1./(12.*dx1(kd)**2)
       dx241(kd) = 1./(12.*dx2(kd))
       dx242(kd) = 1./(12.*dx2(kd)**2)

       dr114(kd) = 1./(12.*dr1(kd))
       dr214(kd) = 1./(12.*dr2(kd))
      end do

      numGhost=orderOfAccuracy/2
      giveDiv=0   ! set to 1 to give div(u) on both sides, rather than setting the jump in div(u)

      ! bounds for loops that include ghost points in the tangential directions:
      nn1a=n1a
      nn1b=n1b
      nn2a=n2a
      nn2b=n2b
      nn3a=n3a
      nn3b=n3b

      mm1a=m1a
      mm1b=m1b
      mm2a=m2a
      mm2b=m2b
      mm3a=m3a
      mm3b=m3b

      i3=n3a
      j3=m3a

      axis1p1=mod(axis1+1,nd)
      axis1p2=mod(axis1+2,nd)
      axis2p1=mod(axis2+1,nd)
      axis2p2=mod(axis2+2,nd)

      is1=0
      is2=0
      is3=0

      if( axis1.ne.0 )then
        ! include ghost lines in tangential periodic (and parallel) directions (for extrapolating)
        ! *wdh* Also include ghost on interpolation boundaries 2015/06/29 
        if( boundaryCondition1(0,0).le.0 )then ! parallel ghost may only have bc<0 on one side
          nn1a=nn1a-numGhost
          if( boundaryCondition2(0,0).gt.0 )then
            write(*,'("Interaface3d: bc is inconsistent")')
            stop 178
          end if
        end if
        if( boundaryCondition1(1,0).le.0 )then ! parallel ghost may only have bc<0 on one side
          nn1b=nn1b+numGhost
          if( boundaryCondition2(1,0).gt.0 )then
            write(*,'("Interaface3d: bc is inconsistent")')
            stop 179
          end if
        end if
      end if
      if( axis1.ne.1 )then
        ! include ghost lines in tangential periodic (and parallel) directions (for extrapolating)
        if( boundaryCondition1(0,1).le.0 )then
          nn2a=nn2a-numGhost
          if( boundaryCondition2(0,1).gt.0 )then
            write(*,'("Interaface3d: bc is inconsistent")')
            stop 180
          end if
        end if
        if( boundaryCondition1(1,1).le.0 )then
          nn2b=nn2b+numGhost
          if( boundaryCondition2(1,1).gt.0 )then
            write(*,'("Interaface3d: bc is inconsistent")')
            stop 181
          end if
        end if
      end if
      if( nd.eq.3 .and. axis1.ne.2 )then
        ! include ghost lines in tangential periodic (and parallel) directions (for extrapolating)
        if( boundaryCondition1(0,2).le.0 )then
          nn3a=nn3a-numGhost
          if( boundaryCondition2(0,2).gt.0 )then
            write(*,'("Interaface3d: bc is inconsistent")')
            stop 182
          end if
        end if
        if( boundaryCondition1(1,2).le.0 )then
          nn3b=nn3b+numGhost
          if( boundaryCondition2(1,2).gt.0 )then
            write(*,'("Interaface3d: bc is inconsistent")')
            stop 183
          end if
        end if
      end if

      if( axis1.eq.0 ) then
        is1=1-2*side1
        an1Cartesian=1. ! normal for a cartesian grid
        an2Cartesian=0.
        an3Cartesian=0.

      else if( axis1.eq.1 )then
        is2=1-2*side1
        an1Cartesian=0.
        an2Cartesian=1.
        an3Cartesian=0.

      else if( axis1.eq.2 )then
        is3=1-2*side1
        an1Cartesian=0.
        an2Cartesian=0.
        an3Cartesian=1.
      else
        stop 5528
      end if


      js1=0
      js2=0
      js3=0
      if( axis2.ne.0 )then
        if( boundaryCondition2(0,0).le.0 )then
          mm1a=mm1a-numGhost
        end if
        if( boundaryCondition2(1,0).le.0 )then
          mm1b=mm1b+numGhost
        end if
      end if
      if( axis2.ne.1 )then
        if( boundaryCondition2(0,1).le.0 )then
          mm2a=mm2a-numGhost
        end if
        if( boundaryCondition2(1,1).le.0 )then
          mm2b=mm2b+numGhost
        end if
      end if
      if( nd.eq.3 .and. axis2.ne.2 )then
        if( boundaryCondition2(0,2).le.0 )then
          mm3a=mm3a-numGhost
        end if
        if( boundaryCondition2(1,2).le.0 )then
          mm3b=mm3b+numGhost
        end if
      end if
      if( axis2.eq.0 ) then
        js1=1-2*side2
      else if( axis2.eq.1 ) then
        js2=1-2*side2
      else  if( axis2.eq.2 ) then
        js3=1-2*side2
      else
        stop 3384
      end if

      is=1-2*side1
      js=1-2*side2

      rx1=0.
      ry1=0.
      rz1=0.
      if( axis1.eq.0 )then
        rx1=1.
      else if( axis1.eq.1 )then
        ry1=1.
      else 
        rz1=1.
      endif

      rx2=0.
      ry2=0.
      rz2=0.
      if( axis2.eq.0 )then
        rx2=1.
      else if( axis2.eq.1 )then
        ry2=1.
      else 
        rz2=1.
      endif

      if( debug.gt.3 )then
        write(debugFile,'("nn1a,nn1b,...=",6i5)') nn1a,nn1b,nn2a,nn2b,nn3a,nn3b
        write(debugFile,'("mm1a,mm1b,...=",6i5)') mm1a,mm1b,mm2a,mm2b,mm3a,mm3b

      end if

      if( orderOfAccuracy.eq.2 .and. orderOfExtrapolation.lt.3 )then
        write(debugFile,'(" ERROR: interface3d: orderOfExtrapolation<3 ")')
        stop 7716
      end if
      if( orderOfAccuracy.eq.4 .and. orderOfExtrapolation.lt.4 )then
        write(debugFile,'(" ERROR: interface3d: orderOfExtrapolation<4 ")')
        stop 7716
      end if

      ! first time through check that the mask's are consistent
      ! For now we require the masks to both be positive at the same points on the interface
      ! We assign pts where both mask1 and mask2 are discretization pts.
      ! If mask1>0 and mask2<0 then we just leave the extrapolated values in u1 and u2 .  
      if( initialized.eq.0 )then
       if( nd.eq.2 )then
        ! check the consistency of the mask arrays
        beginLoops2d()
          m1 = mask1(i1,i2,i3)
          m2 = mask2(j1,j2,j3)
          if( (m1.gt.0 .and. m2.eq.0) .or. (m1.eq.0 .and. m2.gt.0) )then
            write(debugFile,'(" interface3d:ERROR: mask1 and mask2 do not agree. One is >0 and one =0 ")')
             ! '
            stop 1111
          end if 
        endLoops2d()

       else if( nd.eq.3 )then
        ! check the consistency of the mask arrays
        beginLoops3d()
          m1 = mask1(i1,i2,i3)
          m2 = mask2(j1,j2,j3)
          if( (m1.gt.0 .and. m2.eq.0) .or. (m1.eq.0 .and. m2.gt.0) )then
            write(debugFile,'(" interface3d:ERROR: mask1 and mask2 do not agree. One is >0 and one =0")')
             ! '
            stop 1111
          end if 
        endLoops3d()

       end if
       if( debug.gt.0 )then
         write(debugFile,'("cgmx:interface3d: The mask arrays for grid1=",i3," and grid2=",i3," were found to be consistent")') grid1,grid2
         ! ' 
       end if
      end if


      if( nd.eq.2 .and. orderOfAccuracy.eq.2 .and. gridType.eq.rectangular )then
 
#perl $DIM=2; $GRIDTYPE="rectangular"; $ORDER=2;

        if( useForcing.ne.0 )then 
          ! finish me 
          stop 7715
        end if


       if( .false. )then
        ! just copy values from ghost points for now
        beginLoopsMask2d()
          u1(i1-is1,i2-is2,i3,ex)=u2(j1+js1,j2+js2,j3,ex)
          u1(i1-is1,i2-is2,i3,ey)=u2(j1+js1,j2+js2,j3,ey)
          u1(i1-is1,i2-is2,i3,hz)=u2(j1+js1,j2+js2,j3,hz) 
        endLoopsMask2d()
      else

        ! ---- first satisfy the jump conditions on the boundary --------
        !    [ eps n.u ] = 0
        !    [ tau.u ] = 0
        boundaryJumpConditions(2,rectangular)

        ! initialization step: assign first ghost line by extrapolation
        ! NOTE: assign ghost points outside the ends
        if( solveForE.ne.0 )then
         beginGhostLoops2d()
           u1(i1-is1,i2-is2,i3,ex)=extrap3(u1,i1,i2,i3,ex,is1,is2,is3)
           u1(i1-is1,i2-is2,i3,ey)=extrap3(u1,i1,i2,i3,ey,is1,is2,is3)
           u1(i1-is1,i2-is2,i3,hz)=extrap3(u1,i1,i2,i3,hz,is1,is2,is3)
         endLoops2d()
        end if
        if( solveForH .ne.0 )then
          stop 3017
        end if
        ! here are the real jump conditions
        !   [ u.x + v.y +w.z ] = 0
        !   [ u.xx + u.yy +u.zz ] = 0
        ! 
        !   [ tau1.(w.y-v.z, u.z-w.x, v.x-u.y)/mu] = 0 
        !   [ (v.xx+v.yy+v.zz)/eps ] = 0
        ! 
        !   [ tau2.(w.y-v.z, u.z-w.x, v.x-u.y)/mu] = 0 
        !   [ (w.xx+w.yy+w.zz)/eps ] = 0

        beginLoopsMask2d()
         ! first evaluate the equations we want to solve with the wrong values at the ghost points:

          evalInterfaceDerivatives2d()
         
          f(0)=(u1x+v1y) - \
               (u2x+v2y)
          f(1)=(u1xx+u1yy) - \
               (u2xx+u2yy)

          f(2)=(v1x-u1y)/mu1 - \
               (v2x-u2y)/mu2
          
          f(3)=(v1xx+v1yy)/epsmu1 - \
               (v2xx+v2yy)/epsmu2
    
          ! write(debugFile,'(" --> i1,i2=",2i4," f(start)=",4f8.3)') i1,i2,f(0),f(1),f(2),f(3)

           ! here is the matrix of coefficients for the unknowns u1(-1),v1(-1),u2(-1),v2(-1)
           ! Solve:
           !     
           !       A [ U ] = A [ U(old) ] - [ f ]
           if( axis1.eq.0 )then
             a4(0,0) = -is1/(2.*dx1(0))    ! coeff of u1(-1) from [u.x+v.y] 
             a4(0,1) = 0.                  ! coeff of v1(-1) from [u.x+v.y] 
           
             a4(2,0) = 0.
             a4(2,1) = -is1/(2.*dx1(0))    ! coeff of v1(-1) from [v.x - u.y] 
           else 
             a4(0,0) = 0.                 
             a4(0,1) = -is2/(2.*dx1(1))    ! coeff of v1(-1) from [u.x+v.y] 

             a4(2,0) =  is2/(2.*dx1(1))    ! coeff of u1(-1) from [v.x - u.y] 
             a4(2,1) = 0.
           end if
           if( axis2.eq.0 )then
             a4(0,2) = js1/(2.*dx2(0))    ! coeff of u2(-1) from [u.x+v.y] 
             a4(0,3) = 0. 
           
             a4(2,2) = 0.
             a4(2,3) = js1/(2.*dx2(0))    ! coeff of v2(-1) from [v.x - u.y]
           else
             a4(0,2) = 0. 
             a4(0,3) = js2/(2.*dx2(1))    ! coeff of v2(-1) from [u.x+v.y] 

             a4(2,2) =-js2/(2.*dx2(1))    ! coeff of u2(-1) from [v.x - u.y] 
             a4(2,3) = 0.
           end if

           a4(1,0) = 1./(dx1(axis1)**2)   ! coeff of u1(-1) from [u.xx + u.yy]
           a4(1,1) = 0. 
           a4(1,2) =-1./(dx2(axis2)**2)   ! coeff of u2(-1) from [u.xx + u.yy]
           a4(1,3) = 0. 
             
           a4(3,0) = 0.                      
           a4(3,1) = 1./(dx1(axis1)**2)/eps1 ! coeff of v1(-1) from [(v.xx+v.yy)/eps]
           a4(3,2) = 0. 
           a4(3,3) =-1./(dx2(axis2)**2)/eps2 ! coeff of v2(-1) from [(v.xx+v.yy)/eps]
             

           q(0) = u1(i1-is1,i2-is2,i3,ex)
           q(1) = u1(i1-is1,i2-is2,i3,ey)
           q(2) = u2(j1-js1,j2-js2,j3,ex)
           q(3) = u2(j1-js1,j2-js2,j3,ey)

           ! subtract off the contributions from the wrong values at the ghost points:
           do n=0,3
             f(n) = (a4(n,0)*q(0)+a4(n,1)*q(1)+a4(n,2)*q(2)+a4(n,3)*q(3)) - f(n)
           end do
      ! write(debugFile,'(" --> i1,i2=",2i4," f(subtract)=",4f8.3)') i1,i2,f(0),f(1),f(2),f(3)
           ! solve A Q = F
           ! factor the matrix
           numberOfEquations=4
           call dgeco( a4(0,0), numberOfEquations, numberOfEquations, ipvt(0),rcond,work(0))
           ! solve
      ! write(debugFile,'(" --> i1,i2=",2i4," rcond=",e10.2)') i1,i2,rcond
           job=0
           call dgesl( a4(0,0), numberOfEquations, numberOfEquations, ipvt(0), f(0), job)
      ! write(debugFile,'(" --> i1,i2=",2i4," f(solve)=",4f8.3)') i1,i2,f(0),f(1),f(2),f(3)

           u1(i1-is1,i2-is2,i3,ex)=f(0)
           u1(i1-is1,i2-is2,i3,ey)=f(1)
           u2(j1-js1,j2-js2,j3,ex)=f(2)
           u2(j1-js1,j2-js2,j3,ey)=f(3)

      if( debug.gt.2 )then ! re-evaluate
          evalInterfaceDerivatives2d()
          f(0)=(u1x+v1y) - \
               (u2x+v2y)
          f(1)=(u1xx+u1yy) - \
               (u2xx+u2yy)
          f(2)=(v1x-u1y)/mu1 - \
               (v2x-u2y)/mu2
          f(3)=(v1xx+v1yy)/epsmu1 - \
               (v2xx+v2yy)/epsmu2
        write(debugFile,'(" --> i1,i2=",2i4," f(re-eval)=",4e10.2)') i1,i2,f(0),f(1),f(2),f(3)
      end if

           ! do this for now
           u1(i1-is1,i2-is2,i3,hz)=u2(j1+js1,j2+js2,j3,hz) 
           u2(j1-js1,j2-js2,j3,hz)=u1(i1+is1,i2+is2,i3,hz)


         endLoopsMask2d()

         ! opt periodic update
         periodicUpdate2d(u1,boundaryCondition1,gridIndexRange1,side1,axis1)
         periodicUpdate2d(u2,boundaryCondition2,gridIndexRange2,side2,axis2)
       end if

       else if( nd.eq.2 .and. orderOfAccuracy.eq.2 .and. gridType.eq.curvilinear )then

         ! *******************************
         ! ***** 2d curvilinear case *****
         ! *******************************


#perl $DIM=2; $GRIDTYPE="curvilinear"; $ORDER=2; 


         ! ---- first satisfy the jump conditions on the boundary --------
         !    [ eps n.u ] = 0
         !    [ tau.u ] = 0
         boundaryJumpConditions(2,curvilinear)

         ! initialization step: assign first ghost line by extrapolation
         ! NOTE: assign ghost points outside the ends
         beginGhostLoops2d()
            u1(i1-is1,i2-is2,i3,ex)=extrap3(u1,i1,i2,i3,ex,is1,is2,is3)
            u1(i1-is1,i2-is2,i3,ey)=extrap3(u1,i1,i2,i3,ey,is1,is2,is3)
            u1(i1-is1,i2-is2,i3,hz)=extrap3(u1,i1,i2,i3,hz,is1,is2,is3)
!
            u2(j1-js1,j2-js2,j3,ex)=extrap3(u2,j1,j2,j3,ex,js1,js2,js3)
            u2(j1-js1,j2-js2,j3,ey)=extrap3(u2,j1,j2,j3,ey,js1,js2,js3)
            u2(j1-js1,j2-js2,j3,hz)=extrap3(u2,j1,j2,j3,hz,js1,js2,js3)

         endLoops2d()

         ! here are the real jump conditions for the ghost points
         !   [ u.x + v.y ] = 0 = [ rx*ur + ry*vr + sx*us + sy*vs ] 
         !   [ n.(uv.xx + uv.yy) ] = 0
         !   [ v.x - u.y ] =0 
         !   [ tau.(uv.xx+uv.yy)/eps ] = 0

         ! ***** fix these for [mu] != 0 ****
         if( mu1.ne.mu2 )then
           stop 9923
         end if
         beginLoopsMask2d()

           ! here is the normal (assumed to be the same on both sides)
           an1=rsxy1(i1,i2,i3,axis1,0)   ! normal (an1,an2)
           an2=rsxy1(i1,i2,i3,axis1,1)
           aNorm=max(epsx,sqrt(an1**2+an2**2))
           an1=an1/aNorm
           an2=an2/aNorm
           tau1=-an2
           tau2= an1

           ! first evaluate the equations we want to solve with the wrong values at the ghost points:

#beginMacro eval2dJumpOrder2()
 f(0)=(u1x+v1y) - \
      (u2x+v2y)
 f(1)=( an1*u1Lap +an2*v1Lap )- \
      ( an1*u2Lap +an2*v2Lap )
 f(2)=(v1x-u1y) - \
      (v2x-u2y)
 f(3)=( tau1*u1Lap +tau2*v1Lap )/eps1 - \
      ( tau1*u2Lap +tau2*v2Lap )/eps2
 if( twilightZone.eq.1 )then
   call ogderiv(ep, 0,2,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t, ex, uexx )
   call ogderiv(ep, 0,0,2,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t, ex, ueyy )
   call ogderiv(ep, 0,2,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t, ey, vexx )
   call ogderiv(ep, 0,0,2,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t, ey, veyy )

   ueLap = uexx + ueyy
   veLap = vexx + veyy
   f(3) = f(3) - ( tau1*ueLap +tau2*veLap )*(1./eps1-1./eps2)

   ! write(debugFile,'(" u1Lap,ueLap=",2e10.2," v1Lap,veLap=",2e10.2)') u1Lap,ueLap,v1Lap,veLap

 end if
#endMacro

           evalInterfaceDerivatives2d()
           eval2dJumpOrder2()

           ! write(debugFile,'(" --> order2-curv: i1,i2=",2i4," f(start)=",4f8.3)') i1,i2,f(0),f(1),f(2),f(3)
           ! write(debugFile,'(" --> u1(ghost),u1=",4f8.3)') u1(i1-is1,i2-is2,i3,ex),u1(i1,i2,i3,ex)
           ! write(debugFile,'(" --> u2(ghost),u2=",4f8.3)') u2(j1-js1,j2-js2,j3,ex),u2(j1,j2,j3,ex)
           ! '

           ! here is the matrix of coefficients for the unknowns u1(-1),v1(-1),u2(-1),v2(-1)
           ! Solve:
           !     
           !       A [ U ] = A [ U(old) ] - [ f ]
           a4(0,0) = -is*rsxy1(i1,i2,i3,axis1,0)/(2.*dr1(axis1))    ! coeff of u1(-1) from [u.x+v.y] 
           a4(0,1) = -is*rsxy1(i1,i2,i3,axis1,1)/(2.*dr1(axis1))    ! coeff of v1(-1) from [u.x+v.y] 
           a4(0,2) =  js*rsxy2(j1,j2,j3,axis2,0)/(2.*dr2(axis2))    ! coeff of u2(-1) from [u.x+v.y] 
           a4(0,3) =  js*rsxy2(j1,j2,j3,axis2,1)/(2.*dr2(axis2))    ! coeff of v2(-1) from [u.x+v.y] 

           a4(2,0) =  is*rsxy1(i1,i2,i3,axis1,1)/(2.*dr1(axis1))   ! coeff of u1(-1) from [v.x - u.y] 
           a4(2,1) = -is*rsxy1(i1,i2,i3,axis1,0)/(2.*dr1(axis1))   ! coeff of v1(-1) from [v.x - u.y] 

           a4(2,2) = -js*rsxy2(j1,j2,j3,axis2,1)/(2.*dr2(axis2))   ! coeff of u2(-1) from [v.x - u.y] 
           a4(2,3) =  js*rsxy2(j1,j2,j3,axis2,0)/(2.*dr2(axis2))   ! coeff of v2(-1) from [v.x - u.y] 


           ! coeff of u(-1) from lap = u.xx + u.yy
           rxx1(0,0,0)=aj1rxx
           rxx1(1,0,0)=aj1sxx
           rxx1(0,1,1)=aj1ryy
           rxx1(1,1,1)=aj1syy

           rxx2(0,0,0)=aj2rxx
           rxx2(1,0,0)=aj2sxx
           rxx2(0,1,1)=aj2ryy
           rxx2(1,1,1)=aj2syy

           ! clap1=(rsxy1(i1,i2,i3,axis1,0)**2+rsxy1(i1,i2,i3,axis1,1)**2)/(dr1(axis1)**2) \
           !           -is*(rsxy1x22(i1,i2,i3,axis1,0)+rsxy1y22(i1,i2,i3,axis1,1))/(2.*dr1(axis1))
           ! clap2=(rsxy2(j1,j2,j3,axis2,0)**2+rsxy2(j1,j2,j3,axis2,1)**2)/(dr2(axis2)**2) \
           !             -js*(rsxy2x22(j1,j2,j3,axis2,0)+rsxy2y22(j1,j2,j3,axis2,1))/(2.*dr2(axis2)) 
           clap1=(rsxy1(i1,i2,i3,axis1,0)**2+rsxy1(i1,i2,i3,axis1,1)**2)/(dr1(axis1)**2) \
                     -is*(rxx1(axis1,0,0)+rxx1(axis1,1,1))/(2.*dr1(axis1))
           clap2=(rsxy2(j1,j2,j3,axis2,0)**2+rsxy2(j1,j2,j3,axis2,1)**2)/(dr2(axis2)**2) \
                     -js*(rxx2(axis2,0,0)+rxx2(axis2,1,1))/(2.*dr2(axis2)) 

           !   [ n.(uv.xx + u.yy) ] = 0
           a4(1,0) = an1*clap1
           a4(1,1) = an2*clap1
           a4(1,2) =-an1*clap2
           a4(1,3) =-an2*clap2
           !   [ tau.(uv.xx+uv.yy)/eps ] = 0
           a4(3,0) = tau1*clap1/eps1
           a4(3,1) = tau2*clap1/eps1
           a4(3,2) =-tau1*clap2/eps2
           a4(3,3) =-tau2*clap2/eps2
             

           q(0) = u1(i1-is1,i2-is2,i3,ex)
           q(1) = u1(i1-is1,i2-is2,i3,ey)
           q(2) = u2(j1-js1,j2-js2,j3,ex)
           q(3) = u2(j1-js1,j2-js2,j3,ey)

           ! write(debugFile,'(" --> xy1=",4f8.3)') xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
           ! write(debugFile,'(" --> rsxy1=",4f8.3)') rsxy1(i1,i2,i3,0,0),rsxy1(i1,i2,i3,1,0),rsxy1(i1,i2,i3,0,1),rsxy1(i1,i2,i3,1,1)
           ! write(debugFile,'(" --> rsxy2=",4f8.3)') rsxy2(j1,j2,j3,0,0),rsxy2(j1,j2,j3,1,0),rsxy2(j1,j2,j3,0,1),rsxy2(j1,j2,j3,1,1)

           ! write(debugFile,'(" --> rxx1=",2f8.3)') rxx1(axis1,0,0),rxx1(axis1,1,1)
           ! write(debugFile,'(" --> rxx2=",2f8.3)') rxx2(axis2,0,0),rxx2(axis1,1,1)

           ! write(debugFile,'(" --> a4(0,.)=",4f8.3)') a4(0,0),a4(0,1),a4(0,2),a4(0,3)
           ! write(debugFile,'(" --> a4(1,.)=",4f8.3)') a4(1,0),a4(1,1),a4(1,2),a4(1,3)
           ! write(debugFile,'(" --> a4(2,.)=",4f8.3)') a4(2,0),a4(2,1),a4(2,2),a4(2,3)
           ! write(debugFile,'(" --> a4(3,.)=",4f8.3)') a4(3,0),a4(3,1),a4(3,2),a4(3,3)
           ! write(debugFile,'(" --> an1,an2=",2f8.3)') an1,an2
           ! write(debugFile,'(" --> clap1,clap2=",2f8.3)') clap1,clap2
           ! subtract off the contributions from the wrong values at the ghost points:
           do n=0,3
             f(n) = (a4(n,0)*q(0)+a4(n,1)*q(1)+a4(n,2)*q(2)+a4(n,3)*q(3)) - f(n)
           end do
           ! write(debugFile,'(" --> order2-curv: i1,i2=",2i4," f(subtract)=",4f8.3)') i1,i2,f(0),f(1),f(2),f(3)
           ! solve A Q = F
           ! factor the matrix
           numberOfEquations=4
           call dgeco( a4(0,0), numberOfEquations, numberOfEquations, ipvt(0),rcond,work(0))
           ! solve
           ! write(debugFile,'(" --> order2-curv: i1,i2=",2i4," rcond=",e10.2)') i1,i2,rcond
           job=0
           call dgesl( a4(0,0), numberOfEquations, numberOfEquations, ipvt(0), f(0), job)
           ! write(debugFile,'(" --> order2-curv: i1,i2=",2i4," f(solve)=",4f8.3)') i1,i2,f(0),f(1),f(2),f(3)

           u1(i1-is1,i2-is2,i3,ex)=f(0)
           u1(i1-is1,i2-is2,i3,ey)=f(1)
           u2(j1-js1,j2-js2,j3,ex)=f(2)
           u2(j1-js1,j2-js2,j3,ey)=f(3)

           if( debug.gt.3 )then ! re-evaluate
             evalInterfaceDerivatives2d()
             eval2dJumpOrder2()
             !write(debugFile,'(" --> order2-curv: xy1(ghost)=",2e11.3)') xy1(i1-is1,i2-is2,i3,0),xy1(i1-is1,i2-is2,i3,1)
             !write(debugFile,'(" --> order2-curv: xy2(ghost)=",2e11.3)') xy2(j1-js1,j2-js2,j3,0),xy2(j1-js1,j2-js2,j3,1)
             if( twilightZone.eq.1 )then
               call ogderiv(ep, 0,0,0,0, xy1(i1-is1,i2-is2,i3,0),xy1(i1-is1,i2-is2,i3,1),0.,t, ex, uex  )
               call ogderiv(ep, 0,0,0,0, xy1(i1-is1,i2-is2,i3,0),xy1(i1-is1,i2-is2,i3,1),0.,t, ey, uey  )
              write(debugFile,'(" --> order2-curv: i1,i2=",2i4," u1=",2e11.3," err=",2e11.3)') i1,i2,u1(i1-is1,i2-is2,i3,ex),u1(i1-is1,i2-is2,i3,ey),u1(i1-is1,i2-is2,i3,ex)-uex,u1(i1-is1,i2-is2,i3,ey)-uey
               ! '
             else
              write(debugFile,'(" --> order2-curv: i1,i2=",2i4," u1=",2e11.3)') i1,i2,u1(i1-is1,i2-is2,i3,ex),u1(i1-is1,i2-is2,i3,ey)
               ! '
             end if
             write(debugFile,'(" --> order2-curv: j1,j2=",2i4," u2=",2e11.3)') j1,j2,u2(j1-js1,j2-js2,j3,ex),u2(j1-js1,j2-js2,j3,ey)
               ! '
             write(debugFile,'(" --> order2-curv: i1,i2=",2i4," f(re-eval)=",4e10.2)') i1,i2,f(0),f(1),f(2),f(3)
               ! '
           end if

           ! solve for Hz
           !  [ w.n/eps] = 0
           !  [ Lap(w)/eps] = 0

#beginMacro evalMagneticField2dJumpOrder2()
 f(0) = (an1*w1x+an2*w1y)/eps1 -\
        (an1*w2x+an2*w2y)/eps2
 f(1) = w1Lap/eps1 - w2Lap/eps2
 if( twilightZone.eq.1 )then

   call ogderiv(ep, 0,1,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t, hz, wex  )
   call ogderiv(ep, 0,0,1,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t, hz, wey  )
   call ogderiv(ep, 0,2,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t, hz, wexx )
   call ogderiv(ep, 0,0,2,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t, hz, weyy )
   weLap = wexx + weyy
   f(0) = f(0) - (an1*wex+an2*wey)*(1./eps1 - 1./eps2)
   f(1) = f(1) - ( weLap )*(1./eps1 - 1./eps2)
 end if
#endMacro
           evalMagneticFieldInterfaceDerivatives2d()
           evalMagneticField2dJumpOrder2()

           a2(0,0)=-is*(an1*rsxy1(i1,i2,i3,axis1,0)+an2*rsxy1(i1,i2,i3,axis1,1))/(2.*dr1(axis1)*eps1)
           a2(0,1)= js*(an1*rsxy2(j1,j2,j3,axis2,0)+an2*rsxy2(j1,j2,j3,axis2,1))/(2.*dr2(axis2)*eps2)

           a2(1,0)= clap1/eps1
           a2(1,1)=-clap2/eps2

           q(0) = u1(i1-is1,i2-is2,i3,hz)
           q(1) = u2(j1-js1,j2-js2,j3,hz)

           ! subtract off the contributions from the wrong values at the ghost points:
           do n=0,1
             f(n) = (a2(n,0)*q(0)+a2(n,1)*q(1)) - f(n)
           end do

           call dgeco( a2(0,0), 2, 2, ipvt(0),rcond,work(0))
           job=0
           call dgesl( a2(0,0), 2, 2, ipvt(0), f(0), job)

           u1(i1-is1,i2-is2,i3,hz)=f(0)
           u2(j1-js1,j2-js2,j3,hz)=f(1)

           ! u1(i1-is1,i2-is2,i3,hz)=u2(j1+js1,j2+js2,j3,hz) 
           ! u2(j1-js1,j2-js2,j3,hz)=u1(i1+is1,i2+is2,i3,hz)

           if( debug.gt.3 )then ! re-evaluate

             evalMagneticFieldInterfaceDerivatives2d()
             evalMagneticField2dJumpOrder2()

             write(debugFile,'(" --> order2-curv: i1,i2=",2i4," hz-f(re-eval)=",4e10.2)') i1,i2,f(0),f(1)
               ! '
           end if

         endLoopsMask2d()

         ! now make sure that div(u)=0 etc.
         if( .false. )then
!2         beginLoops2d() ! =============== start loops =======================
!2
!2           ! 0  [ u.x + v.y ] = 0
!2           ! first evaluate the equations we want to solve with the wrong values at the ghost points:
!2           divu=u1x22(i1,i2,i3,ex)+u1y22(i1,i2,i3,ey)
!2           a0=-is*rsxy1(i1,i2,i3,axis1,0)*dr112(axis1)
!2           a1=-is*rsxy1(i1,i2,i3,axis1,1)*dr112(axis1)
!2           aNormSq=a0**2+a1**2
!2           ! now project:  a.uNew = a.uOld - div  ->  (div-a.uOld)+a.uNew = div(uNew) = 0
!2           u1(i1-is1,i2-is2,i3,ex)=u1(i1-is1,i2-is2,i3,ex)-divu*a0/aNormSq
!2           u1(i1-is1,i2-is2,i3,ey)=u1(i1-is1,i2-is2,i3,ey)-divu*a1/aNormSq
!2
!2           divu=u2x22(j1,j2,j3,ex)+u2y22(j1,j2,j3,ey)
!2           a0=-js*rsxy2(j1,j2,j3,axis2,0)*dr212(axis2) 
!2           a1=-js*rsxy2(j1,j2,j3,axis2,1)*dr212(axis2) 
!2           aNormSq=a0**2+a1**2
!2
!2           u2(j1-js1,j2-js2,j3,ex)=u2(j1-js1,j2-js2,j3,ex)-divu*a0/aNormSq
!2           u2(j1-js1,j2-js2,j3,ey)=u2(j1-js1,j2-js2,j3,ey)-divu*a1/aNormSq
!2
!2           if( debug.gt.0 )then
!2             write(debugFile,'(" --> 2cth: eval div1,div2=",2e10.2)') u1x22(i1,i2,i3,ex)+u1y22(i1,i2,i3,ey),u2x22(j1,j2,j3,ex)+u2y22(j1,j2,j3,ey)
!2           end if
!2         endLoops2d()
         end if

         ! periodic update **** THIS WON T WORK IN PARALLEL
         periodicUpdate2d(u1,boundaryCondition1,gridIndexRange1,side1,axis1)
         periodicUpdate2d(u2,boundaryCondition2,gridIndexRange2,side2,axis2)

       else if( .false. .and. orderOfAccuracy.eq.4 )then

         ! for testing -- just assign from the other ghost points

         beginLoops2d()
           u1(i1-is1,i2-is2,i3,ex)=u2(j1+js1,j2+js2,j3,ex)
           u1(i1-is1,i2-is2,i3,ey)=u2(j1+js1,j2+js2,j3,ey)
           u1(i1-is1,i2-is2,i3,hz)=u2(j1+js1,j2+js2,j3,hz) 

           u2(j1-js1,j2-js2,j3,ex)=u1(i1+is1,i2+is2,i3,ex)
           u2(j1-js1,j2-js2,j3,ey)=u1(i1+is1,i2+is2,i3,ey)
           u2(j1-js1,j2-js2,j3,hz)=u1(i1+is1,i2+is2,i3,hz)

           u1(i1-2*is1,i2-2*is2,i3,ex)=u2(j1+2*js1,j2+2*js2,j3,ex)
           u1(i1-2*is1,i2-2*is2,i3,ey)=u2(j1+2*js1,j2+2*js2,j3,ey)
           u1(i1-2*is1,i2-2*is2,i3,hz)=u2(j1+2*js1,j2+2*js2,j3,hz) 

           u2(j1-2*js1,j2-2*js2,j3,ex)=u1(i1+2*is1,i2+2*is2,i3,ex)
           u2(j1-2*js1,j2-2*js2,j3,ey)=u1(i1+2*is1,i2+2*is2,i3,ey)
           u2(j1-2*js1,j2-2*js2,j3,hz)=u1(i1+2*is1,i2+2*is2,i3,hz)

         endLoops2d()

       else if( nd.eq.2 .and. orderOfAccuracy.eq.4 .and. gridType.eq.rectangular )then
#perl $DIM=2; $GRIDTYPE="rectangular"; $ORDER=4;
  
         ! --------------- 4th Order Rectangular ---------------

         if( useForcing.ne.0 )then 
           ! finish me 
           stop 7716
         end if
         ! ***** fix these for [mu] != 0 ****
         if( mu1.ne.mu2 )then
           stop 9924
         end if


         ! ---- first satisfy the jump conditions on the boundary --------
         !    [ eps n.u ] = 0
         !    [ tau.u ] = 0
         boundaryJumpConditions(2,rectangular)

         ! here are the real jump conditions for the ghost points
         ! 0  [ u.x + v.y ] = 0
         ! 1  [ u.xx + u.yy ] = 0
         ! 2  [ v.x - u.y ] =0 
         ! 3  [ (v.xx+v.yy)/eps ] = 0
         ! 4  [ (u.xx+u.yy).x + (v.xx+v.yy).y ] = 0  OR [ (u.xx).x + (v.xx).y ] = 0 OR  [ (u.yy).x + (v.yy).y ] = 0 
         ! 5  [ {(Delta v).x - (Delta u).y}/eps ] =0  -> [ {(v.xxx+v.xyy)-(u.xxy+u.yyy)}/eps ] = 0
         ! 6  [ Delta^2 u/eps ] = 0
         ! 7  [ Delta^2 v/eps^2 ] = 0 


         ! initialization step: assign first ghost line by extrapolation
         ! NOTE: assign ghost points outside the ends
         beginGhostLoops2d()
           ! extrap to order 5 so exact for degree 4 *wdh* 2015/06/29
           u1(i1-is1,i2-is2,i3,ex)=extrap5(u1,i1,i2,i3,ex,is1,is2,is3)
           u1(i1-is1,i2-is2,i3,ey)=extrap5(u1,i1,i2,i3,ey,is1,is2,is3)
           u1(i1-is1,i2-is2,i3,hz)=extrap5(u1,i1,i2,i3,hz,is1,is2,is3)

           u2(j1-js1,j2-js2,j3,ex)=extrap5(u2,j1,j2,j3,ex,js1,js2,js3)
           u2(j1-js1,j2-js2,j3,ey)=extrap5(u2,j1,j2,j3,ey,js1,js2,js3)
           u2(j1-js1,j2-js2,j3,hz)=extrap5(u2,j1,j2,j3,hz,js1,js2,js3)

           ! u1(i1-is1,i2-is2,i3,ex)=extrap4(u1,i1,i2,i3,ex,is1,is2,is3)
           ! u1(i1-is1,i2-is2,i3,ey)=extrap4(u1,i1,i2,i3,ey,is1,is2,is3)
           ! u1(i1-is1,i2-is2,i3,hz)=extrap4(u1,i1,i2,i3,hz,is1,is2,is3)

           ! u2(j1-js1,j2-js2,j3,ex)=extrap4(u2,j1,j2,j3,ex,js1,js2,js3)
           ! u2(j1-js1,j2-js2,j3,ey)=extrap4(u2,j1,j2,j3,ey,js1,js2,js3)
           ! u2(j1-js1,j2-js2,j3,hz)=extrap4(u2,j1,j2,j3,hz,js1,js2,js3)

           ! --- also extrap 2nd line for now
           ! u1(i1-2*is1,i2-2*is2,i3,ex)=extrap4(u1,i1-is1,i2-is2,i3,ex,is1,is2,is3)
           ! u1(i1-2*is1,i2-2*is2,i3,ey)=extrap4(u1,i1-is1,i2-is2,i3,ey,is1,is2,is3)
           ! u1(i1-2*is1,i2-2*is2,i3,hz)=extrap4(u1,i1-is1,i2-is2,i3,hz,is1,is2,is3)

           ! u2(j1-2*js1,j2-2*js2,j3,ex)=extrap4(u2,j1-js1,j2-js2,j3,ex,js1,js2,js3)
           ! u2(j1-2*js1,j2-2*js2,j3,ey)=extrap4(u2,j1-js1,j2-js2,j3,ey,js1,js2,js3)
           ! u2(j1-2*js1,j2-2*js2,j3,hz)=extrap4(u2,j1-js1,j2-js2,j3,hz,js1,js2,js3)
         endLoops2d()

         beginLoopsMask2d() ! =============== start loops =======================

           ! first evaluate the equations we want to solve with the wrong values at the ghost points:


           ! evalDerivs2dOrder4()
           evalDerivs2dOrder4()
           f(0)=(u1x+v1y) - \
                (u2x+v2y)
           f(1)=(u1Lap) - \
                (u2Lap)
           f(2)=(v1x-u1y) - \
                (v2x-u2y)
           f(3)=(v1Lap)/eps1 - \
                (v2Lap)/eps2
           ! These next we can do to 2nd order -- these need a value on the first ghost line --
           f(4)=(u1xxx+u1xyy+v1xxy+v1yyy) - \
                (u2xxx+u2xyy+v2xxy+v2yyy)
           f(5)=((v1xxx+v1xyy)-(u1xxy+u1yyy))/eps1 - \
                ((v2xxx+v2xyy)-(u2xxy+u2yyy))/eps2
           f(6)=(u1LapSq)/eps1 - \
                (u2LapSq)/eps2
           f(7)=(v1LapSq)/eps1**2 - \
                (v2LapSq)/eps2**2
           
!      write(debugFile,'(" --> 4th: j1,j2=",2i4," u1xx,u1yy,u2xx,u2yy=",4e10.2)') j1,j2,u1xx42r(i1,i2,i3,ex),\
!          u1yy42r(i1,i2,i3,ex),u2xx42r(j1,j2,j3,ex),u2yy42r(j1,j2,j3,ex)
!      write(debugFile,'(" --> 4th: i1,i2=",2i4," f(start)=",8e10.2)') i1,i2,f(0),f(1),f(2),f(3),f(4),f(5),f(6),f(7)

           ! here is the matrix of coefficients for the unknowns u1(-1),v1(-1),u2(-1),v2(-1)
           ! Solve:
           !     
           !       A [ U ] = A [ U(old) ] - [ f ]
!      u1x43r(i1,i2,i3,kd)=(8.*(u1(i1+1,i2,i3,kd)-u1(i1-1,i2,i3,kd))-(
!     & u1(i1+2,i2,i3,kd)-u1(i1-2,i2,i3,kd)))*dx141(0)


           ! 0  [ u.x + v.y ] = 0
           a8(0,0) = -is*8.*rx1*dx141(axis1)     ! coeff of u1(-1) from [u.x+v.y] 
           a8(0,1) = -is*8.*ry1*dx141(axis1)     ! coeff of v1(-1) from [u.x+v.y] 
           a8(0,4) =  is*rx1*dx141(axis1)        ! u1(-2)
           a8(0,5) =  is*ry1*dx141(axis1)        ! v1(-2) 

           a8(0,2) =  js*8.*rx2*dx241(axis2)     ! coeff of u2(-1) from [u.x+v.y] 
           a8(0,3) =  js*8.*ry2*dx241(axis2) 
           a8(0,6) = -js*   rx2*dx241(axis2) 
           a8(0,7) = -js*   ry2*dx241(axis2) 

           ! 1  [ u.xx + u.yy ] = 0
!      u1xx43r(i1,i2,i3,kd)=( -30.*u1(i1,i2,i3,kd)+16.*(u1(i1+1,i2,i3,
!     & kd)+u1(i1-1,i2,i3,kd))-(u1(i1+2,i2,i3,kd)+u1(i1-2,i2,i3,kd)) )*
!     & dx142(0)
           
           a8(1,0) = 16.*dx142(axis1)         ! coeff of u1(-1) from [u.xx + u.yy]
           a8(1,1) = 0. 
           a8(1,4) =    -dx142(axis1)         ! coeff of u1(-2) from [u.xx + u.yy]
           a8(1,5) = 0. 

           a8(1,2) =-16.*dx242(axis2)         ! coeff of u2(-1) from [u.xx + u.yy]
           a8(1,3) = 0. 
           a8(1,6) =     dx242(axis2)         ! coeff of u2(-2) from [u.xx + u.yy]
           a8(1,7) = 0. 


           ! 2  [ v.x - u.y ] =0 
           a8(2,0) =  is*8.*ry1*dx141(axis1)
           a8(2,1) = -is*8.*rx1*dx141(axis1)    ! coeff of v1(-1) from [v.x - u.y] 
           a8(2,4) = -is*   ry1*dx141(axis1)
           a8(2,5) =  is*   rx1*dx141(axis1)

           a8(2,2) = -js*8.*ry2*dx241(axis2)
           a8(2,3) =  js*8.*rx2*dx241(axis2)
           a8(2,6) =  js*   ry2*dx241(axis2)
           a8(2,7) = -js*   rx2*dx241(axis2)

           ! 3  [ (v.xx+v.yy)/eps ] = 0
           a8(3,0) = 0.                      
           a8(3,1) = 16.*dx142(axis1)/eps1 ! coeff of v1(-1) from [(v.xx+v.yy)/eps]
           a8(3,4) = 0.                      
           a8(3,5) =    -dx142(axis1)/eps1 ! coeff of v1(-2) from [(v.xx+v.yy)/eps]

           a8(3,2) = 0. 
           a8(3,3) =-16.*dx242(axis2)/eps2 ! coeff of v2(-1) from [(v.xx+v.yy)/eps]
           a8(3,6) = 0. 
           a8(3,7) =     dx242(axis2)/eps2 ! coeff of v2(-2) from [(v.xx+v.yy)/eps]

           ! 4  [ (u.xx+u.yy).x + (v.xx+v.yy).y ] = 0
!     u1xxx22r(i1,i2,i3,kd)=(-2.*(u1(i1+1,i2,i3,kd)-u1(i1-1,i2,i3,kd))+
!    & (u1(i1+2,i2,i3,kd)-u1(i1-2,i2,i3,kd)) )*dx122(0)*dx112(0)
!    u1xxy22r(i1,i2,i3,kd)=( u1xx22r(i1,i2+1,i3,kd)-u1xx22r(i1,i2-1,
!     & i3,kd))/(2.*dx1(1))
!      u1yy23r(i1,i2,i3,kd)=(-2.*u1(i1,i2,i3,kd)+(u1(i1,i2+1,i3,kd)+u1(
!     & i1,i2-1,i3,kd)) )*dx122(1)
!     u1xyy22r(i1,i2,i3,kd)=( u1yy22r(i1+1,i2,i3,kd)-u1yy22r(i1-1,i2,
!     & i3,kd))/(2.*dx1(0))
          a8(4,0)= ( is*rx1*2.*dx122(axis1)*dx112(axis1)+is*rx1*2.*dx122(1)/(2.*dx1(0)))
          a8(4,1)= ( is*ry1*2.*dx122(axis1)*dx112(axis1)+is*ry1*2.*dx122(0)/(2.*dx1(1)))
          a8(4,4)= (-is*rx1   *dx122(axis1)*dx112(axis1) )  
          a8(4,5)= (-is*ry1   *dx122(axis1)*dx112(axis1))

          a8(4,2)=-( js*rx2*2.*dx222(axis2)*dx212(axis2)+js*rx2*2.*dx222(1)/(2.*dx2(0)))
          a8(4,3)=-( js*ry2*2.*dx222(axis2)*dx212(axis2)+js*ry2*2.*dx222(0)/(2.*dx2(1)))
          a8(4,6)=-(-js*rx2   *dx222(axis2)*dx212(axis2))   
          a8(4,7)=-(-js*ry2   *dx222(axis2)*dx212(axis2))

          ! 5  [ {(Delta v).x - (Delta u).y}/eps ] =0  -> [ {(v.xxx+v.xyy)-(u.xxy+u.yyy)}/eps ] = 0

          a8(5,0)=-( is*ry1*2.*dx122(axis1)*dx112(axis1)+is*ry1*2.*dx122(0)/(2.*dx1(1)))/eps1
          a8(5,1)= ( is*rx1*2.*dx122(axis1)*dx112(axis1)+is*rx1*2.*dx122(1)/(2.*dx1(0)))/eps1
          a8(5,4)=-(-is*ry1   *dx122(axis1)*dx112(axis1))/eps1
          a8(5,5)= (-is*rx1   *dx122(axis1)*dx112(axis1))/eps1   

          a8(5,2)= ( js*ry2*2.*dx222(axis2)*dx212(axis2)+js*ry2*2.*dx222(0)/(2.*dx2(1)))/eps2
          a8(5,3)=-( js*rx2*2.*dx222(axis2)*dx212(axis2)+js*rx2*2.*dx222(1)/(2.*dx2(0)))/eps2
          a8(5,6)= (-js*ry2   *dx222(axis2)*dx212(axis2))/eps2
          a8(5,7)=-(-js*rx2   *dx222(axis2)*dx212(axis2))/eps2   

           ! 6  [ Delta^2 u/eps ] = 0
!     u1LapSq22r(i1,i2,i3,kd)= ( 6.*u1(i1,i2,i3,kd)- 4.*(u1(i1+1,i2,i3,
!    & kd)+u1(i1-1,i2,i3,kd))+(u1(i1+2,i2,i3,kd)+u1(i1-2,i2,i3,kd)) )
!    & /(dx1(0)**4)+( 6.*u1(i1,i2,i3,kd)-4.*(u1(i1,i2+1,i3,kd)+u1(i1,
!    & i2-1,i3,kd)) +(u1(i1,i2+2,i3,kd)+u1(i1,i2-2,i3,kd)) )/(dx1(1)**
!    & 4)+( 8.*u1(i1,i2,i3,kd)-4.*(u1(i1+1,i2,i3,kd)+u1(i1-1,i2,i3,kd)
!    & +u1(i1,i2+1,i3,kd)+u1(i1,i2-1,i3,kd))+2.*(u1(i1+1,i2+1,i3,kd)+
!    & u1(i1-1,i2+1,i3,kd)+u1(i1+1,i2-1,i3,kd)+u1(i1-1,i2-1,i3,kd)) )
!    & /(dx1(0)**2*dx1(1)**2)

           a8(6,0) = -(4./(dx1(axis1)**4) +4./(dx1(0)**2*dx1(1)**2) )/eps1
           a8(6,1) = 0.
           a8(6,4) =   1./(dx1(axis1)**4)/eps1
           a8(6,5) = 0.

           a8(6,2) = (4./(dx2(axis2)**4) +4./(dx1(0)**2*dx1(1)**2) )/eps2
           a8(6,3) = 0.
           a8(6,6) =  -1./(dx2(axis2)**4)/eps2
           a8(6,7) = 0.

           ! 7  [ Delta^2 v/eps^2 ] = 0 
           a8(7,0) = 0.
           a8(7,1) = -(4./(dx1(axis1)**4) +4./(dx2(0)**2*dx2(1)**2) )/eps1**2
           a8(7,4) = 0.
           a8(7,5) =   1./(dx1(axis1)**4)/eps1**2

           a8(7,2) = 0.
           a8(7,3) =  (4./(dx2(axis2)**4) +4./(dx2(0)**2*dx2(1)**2) )/eps2**2
           a8(7,6) = 0.
           a8(7,7) =  -1./(dx2(axis2)**4)/eps2**2

           q(0) = u1(i1-is1,i2-is2,i3,ex)
           q(1) = u1(i1-is1,i2-is2,i3,ey)
           q(2) = u2(j1-js1,j2-js2,j3,ex)
           q(3) = u2(j1-js1,j2-js2,j3,ey)

           q(4) = u1(i1-2*is1,i2-2*is2,i3,ex)
           q(5) = u1(i1-2*is1,i2-2*is2,i3,ey)
           q(6) = u2(j1-2*js1,j2-2*js2,j3,ex)
           q(7) = u2(j1-2*js1,j2-2*js2,j3,ey)

!      write(debugFile,'(" --> 4th: i1,i2=",2i4," q=",8e10.2)') i1,i2,q(0),q(1),q(2),q(3),q(4),q(5),q(6),q(7)

           ! subtract off the contributions from the initial (wrong) values at the ghost points:
           do n=0,7
             f(n) = (a8(n,0)*q(0)+a8(n,1)*q(1)+a8(n,2)*q(2)+a8(n,3)*q(3)+\
                     a8(n,4)*q(4)+a8(n,5)*q(5)+a8(n,6)*q(6)+a8(n,7)*q(7)) - f(n)
           end do

           ! solve A Q = F
           ! factor the matrix
           numberOfEquations=8
           call dgeco( a8(0,0), numberOfEquations, numberOfEquations, ipvt(0),rcond,work(0))
           ! solve
           !write(debugFile,'(" --> 4th: i1,i2=",2i4," rcond=",e10.2)') i1,i2,rcond
           job=0
           call dgesl( a8(0,0), numberOfEquations, numberOfEquations, ipvt(0), f(0), job)

           !write(debugFile,'(" --> 4th: i1,i2=",2i4," f(solve)=",8e10.2)') i1,i2,f(0),f(1),f(2),f(3),f(4),f(5),f(6),f(7)

           if( .true. )then
           u1(i1-is1,i2-is2,i3,ex)=f(0)
           u1(i1-is1,i2-is2,i3,ey)=f(1)
           u2(j1-js1,j2-js2,j3,ex)=f(2)
           u2(j1-js1,j2-js2,j3,ey)=f(3)

           u1(i1-2*is1,i2-2*is2,i3,ex)=f(4)
           u1(i1-2*is1,i2-2*is2,i3,ey)=f(5)
           u2(j1-2*js1,j2-2*js2,j3,ex)=f(6)
           u2(j1-2*js1,j2-2*js2,j3,ey)=f(7)
           end if

          if( debug.gt.3 )then ! re-evaluate
           evalDerivs2dOrder4()
           f(0)=(u1x+v1y) - \
                (u2x+v2y)
           f(1)=(u1Lap) - \
                (u2Lap)
           f(2)=(v1x-u1y) - \
                (v2x-u2y)
           f(3)=(v1Lap)/eps1 - \
                (v2Lap)/eps2
           ! These next we can do to 2nd order -- these need a value on the first ghost line --
           f(4)=(u1xxx+u1xyy+v1xxy+v1yyy) - \
                (u2xxx+u2xyy+v2xxy+v2yyy)
           f(5)=((v1xxx+v1xyy)-(u1xxy+u1yyy))/eps1 - \
                ((v2xxx+v2xyy)-(u2xxy+u2yyy))/eps2
           f(6)=(u1LapSq)/eps1 - \
                (u2LapSq)/eps2
           f(7)=(v1LapSq)/eps1**2 - \
                (v2LapSq)/eps2**2
    
           write(debugFile,'(" --> 4th: i1,i2=",2i4," f(re-eval)=",8e10.2)') i1,i2,f(0),f(1),f(2),f(3),f(4),f(5),f(6),f(7) 
           ! '
          end if

           ! do this for now
           u1(i1-is1,i2-is2,i3,hz)=u2(j1+js1,j2+js2,j3,hz) 
           u2(j1-js1,j2-js2,j3,hz)=u1(i1+is1,i2+is2,i3,hz)

           u1(i1-2*is1,i2-2*is2,i3,hz)=u2(j1+2*js1,j2+2*js2,j3,hz) 
           u2(j1-2*js1,j2-2*js2,j3,hz)=u1(i1+2*is1,i2+2*is2,i3,hz)

         endLoopsMask2d()

         ! periodic update
         periodicUpdate2d(u1,boundaryCondition1,gridIndexRange1,side1,axis1)
         periodicUpdate2d(u2,boundaryCondition2,gridIndexRange2,side2,axis2)

       else if( nd.eq.2 .and. orderOfAccuracy.eq.4 .and. gridType.eq.curvilinear )then
#perl $DIM=2; $GRIDTYPE="curvilinear"; $ORDER=4;
  
         ! --------------- 4th Order Curvilinear ---------------

         ! ***** fix these for [mu] != 0 ****
         if( mu1.ne.mu2 )then
           stop 9925
         end if

         ! ---- first satisfy the jump conditions on the boundary --------
         !    [ eps n.u ] = 0
         !    [ tau.u ] = 0
         !    [ w ] = 0 
         boundaryJumpConditions(2,curvilinear)

         ! here are the real jump conditions for the ghost points
         ! 0  [ u.x + v.y ] = 0
         ! 1  [ n.(uv.xx + uv.yy) ] = 0
         ! 2  [ v.x - u.y ] =0 
         ! 3  [ tau.(v.xx+v.yy)/eps ] = 0
         ! 4  [ (u.xx+u.yy).x + (v.xx+v.yy).y ] = 0  OR [ (u.xx).x + (v.xx).y ] = 0 OR  [ (u.yy).x + (v.yy).y ] = 0 
         ! 5  [ {(Delta v).x - (Delta u).y}/eps ] =0  -> [ {(v.xxx+v.xyy)-(u.xxy+u.yyy)}/eps ] = 0
         ! 6  [ n.Delta^2 uv/eps ] = 0
         ! 7  [ tau.Delta^2 uv/eps^2 ] = 0 



         ! initialization step: assign first ghost line by extrapolation
         ! NOTE: assign ghost points outside the ends

         
         beginGhostLoops2d()

           ! extrap to order 5 so exact for degree 4 *wdh* 2015/06/29
           u1(i1-is1,i2-is2,i3,ex)=extrap5(u1,i1,i2,i3,ex,is1,is2,is3)
           u1(i1-is1,i2-is2,i3,ey)=extrap5(u1,i1,i2,i3,ey,is1,is2,is3)
           u1(i1-is1,i2-is2,i3,hz)=extrap5(u1,i1,i2,i3,hz,is1,is2,is3)

           u2(j1-js1,j2-js2,j3,ex)=extrap5(u2,j1,j2,j3,ex,js1,js2,js3)
           u2(j1-js1,j2-js2,j3,ey)=extrap5(u2,j1,j2,j3,ey,js1,js2,js3)
           u2(j1-js1,j2-js2,j3,hz)=extrap5(u2,j1,j2,j3,hz,js1,js2,js3)

           ! --- also extrap 2nd line for now
           u1(i1-2*is1,i2-2*is2,i3,ex)=extrap5(u1,i1-is1,i2-is2,i3,ex,is1,is2,is3)
           u1(i1-2*is1,i2-2*is2,i3,ey)=extrap5(u1,i1-is1,i2-is2,i3,ey,is1,is2,is3)
           u1(i1-2*is1,i2-2*is2,i3,hz)=extrap5(u1,i1-is1,i2-is2,i3,hz,is1,is2,is3)

           u2(j1-2*js1,j2-2*js2,j3,ex)=extrap5(u2,j1-js1,j2-js2,j3,ex,js1,js2,js3)
           u2(j1-2*js1,j2-2*js2,j3,ey)=extrap5(u2,j1-js1,j2-js2,j3,ey,js1,js2,js3)
           u2(j1-2*js1,j2-2*js2,j3,hz)=extrap5(u2,j1-js1,j2-js2,j3,hz,js1,js2,js3)

           ! u1(i1-is1,i2-is2,i3,ex)=extrap4(u1,i1,i2,i3,ex,is1,is2,is3)
           ! u1(i1-is1,i2-is2,i3,ey)=extrap4(u1,i1,i2,i3,ey,is1,is2,is3)
           ! u1(i1-is1,i2-is2,i3,hz)=extrap4(u1,i1,i2,i3,hz,is1,is2,is3)

           ! u2(j1-js1,j2-js2,j3,ex)=extrap4(u2,j1,j2,j3,ex,js1,js2,js3)
           ! u2(j1-js1,j2-js2,j3,ey)=extrap4(u2,j1,j2,j3,ey,js1,js2,js3)
           ! u2(j1-js1,j2-js2,j3,hz)=extrap4(u2,j1,j2,j3,hz,js1,js2,js3)

           ! --- also extrap 2nd line for now
           ! u1(i1-2*is1,i2-2*is2,i3,ex)=extrap4(u1,i1-is1,i2-is2,i3,ex,is1,is2,is3)
           ! u1(i1-2*is1,i2-2*is2,i3,ey)=extrap4(u1,i1-is1,i2-is2,i3,ey,is1,is2,is3)
           ! u1(i1-2*is1,i2-2*is2,i3,hz)=extrap4(u1,i1-is1,i2-is2,i3,hz,is1,is2,is3)

           ! u2(j1-2*js1,j2-2*js2,j3,ex)=extrap4(u2,j1-js1,j2-js2,j3,ex,js1,js2,js3)
           ! u2(j1-2*js1,j2-2*js2,j3,ey)=extrap4(u2,j1-js1,j2-js2,j3,ey,js1,js2,js3)
           ! u2(j1-2*js1,j2-2*js2,j3,hz)=extrap4(u2,j1-js1,j2-js2,j3,hz,js1,js2,js3)
         endLoops2d()

         ! write(debugFile,'(">>> interface: order=4 initialized=",i4)') initialized

         do it=1,nit ! *** begin iteration ****

           err=0.
         ! =============== start loops ======================
         nn=-1 ! counts points on the interface
         beginLoopsMask2d() 

           nn=nn+1

           ! here is the normal (assumed to be the same on both sides)
           an1=rsxy1(i1,i2,i3,axis1,0)   ! normal (an1,an2)
           an2=rsxy1(i1,i2,i3,axis1,1)
           aNorm=max(epsx,sqrt(an1**2+an2**2))
           an1=an1/aNorm
           an2=an2/aNorm
           tau1=-an2
           tau2= an1

#beginMacro eval2dJumpOrder4()
 f(0)=(u1x+v1y) - \
      (u2x+v2y)
 f(1)=(an1*u1Lap+an2*v1Lap) - \
      (an1*u2Lap+an2*v2Lap)
 f(2)=(v1x-u1y) - \
      (v2x-u2y)
 f(3)=(tau1*u1Lap+tau2*v1Lap)/eps1 - \
      (tau1*u2Lap+tau2*v2Lap)/eps2
 f(4)=(u1xxx+u1xyy+v1xxy+v1yyy) - \
      (u2xxx+u2xyy+v2xxy+v2yyy)
 f(5)=((v1xxx+v1xyy)-(u1xxy+u1yyy))/eps1 - \
      ((v2xxx+v2xyy)-(u2xxy+u2yyy))/eps2
 f(6)=(an1*u1LapSq+an2*v1LapSq)/eps1 - \
      (an1*u2LapSq+an2*v2LapSq)/eps2
 f(7)=(tau1*u1LapSq+tau2*v1LapSq)/eps1**2 - \
      (tau1*u2LapSq+tau2*v2LapSq)/eps2**2
     
 if( twilightZone.eq.1 )then
   call ogderiv(ep, 0,2,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t, ex, uexx )
   call ogderiv(ep, 0,0,2,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t, ex, ueyy )
   call ogderiv(ep, 0,2,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t, ey, vexx )
   call ogderiv(ep, 0,0,2,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t, ey, veyy )

   call ogderiv(ep, 0,2,1,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t, ex, uexxy )
   call ogderiv(ep, 0,0,3,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t, ex, ueyyy )

   call ogderiv(ep, 0,3,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t, ey, vexxx )
   call ogderiv(ep, 0,1,2,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t, ey, vexyy )

   call ogderiv(ep, 0,4,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t, ex, uexxxx )
   call ogderiv(ep, 0,2,2,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t, ex, uexxyy )
   call ogderiv(ep, 0,0,4,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t, ex, ueyyyy )

   call ogderiv(ep, 0,4,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t, ey, vexxxx )
   call ogderiv(ep, 0,2,2,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t, ey, vexxyy )
   call ogderiv(ep, 0,0,4,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t, ey, veyyyy )

   ueLap = uexx + ueyy
   veLap = vexx + veyy
   ueLapSq = uexxxx + 2.*uexxyy + ueyyyy
   veLapSq = vexxxx + 2.*vexxyy + veyyyy

   f(3) = f(3) - ( tau1*ueLap +tau2*veLap )*(1./eps1-1./eps2)
   f(5) = f(5) - ((vexxx+vexyy)-(uexxy+ueyyy))*(1./eps1-1./eps2)
   f(6) = f(6) - (an1*ueLapSq+an2*veLapSq)*(1./eps1-1./eps2)
   f(7) = f(7) - (tau1*ueLapSq+tau2*veLapSq)*(1./eps1**2 - 1./eps2**2)
 end if
#endMacro
           ! evalDerivs2dOrder4()
           evalDerivs2dOrder4()
           ! first evaluate the equations we want to solve with the wrong values at the ghost points:
           eval2dJumpOrder4()


       if( debug.gt.7 ) write(debugFile,'(" --> 4cth: j1,j2=",2i4," u1xx,u1yy,u2xx,u2yy=",4e10.2)') j1,j2,u1xx,\
           u1yy,u2xx,u2yy
        ! '
       if( debug.gt.3 ) write(debugFile,'(" --> 4cth: i1,i2=",2i4," f(start)=",8e10.2)') i1,i2,f(0),f(1),f(2),f(3),f(4),f(5),f(6),f(7)
        ! '


! here are the macros from deriv.maple (file=derivMacros.h)

#defineMacro lapCoeff4a(is,dr,ds) ( (-2/3.*rxx*is-2/3.*ryy*is)/dr+(4/3.*rx**2+4/3.*ry**2)/dr**2 )

#defineMacro lapCoeff4b(is,dr,ds) ( (1/12.*rxx*is+1/12.*ryy*is)/dr+(-1/12.*rx**2-1/12.*ry**2)/dr**2 )

#defineMacro xLapCoeff4a(is,dr,ds) ( (-1/2.*rxyy*is-1/2.*rxxx*is+(sy*(ry*sx*is+sy*rx*is)+3*rx*sx**2*is+ry*sy*sx*is)/ds**2)/dr+(2*ry*rxy+3*rx*rxx+ryy*rx)/dr**2+(ry**2*rx*is+rx**3*is)/dr**3 )

#defineMacro xLapCoeff4b(is,dr,ds) ( (-1/2.*rx**3*is-1/2.*ry**2*rx*is)/dr**3 )

#defineMacro yLapCoeff4a(is,dr,ds) ( (-1/2.*ryyy*is-1/2.*rxxy*is+(3*ry*sy**2*is+ry*sx**2*is+2*sy*rx*sx*is)/ds**2)/dr+(2*rxy*rx+ry*rxx+3*ry*ryy)/dr**2+(ry**3*is+ry*rx**2*is)/dr**3 )

#defineMacro yLapCoeff4b(is,dr,ds) ( (-1/2.*ry*rx**2*is-1/2.*ry**3*is)/dr**3 )

#defineMacro lapSqCoeff4a(is,dr,ds) ( (-1/2.*rxxxx*is-rxxyy*is-1/2.*ryyyy*is+(2*sy*(2*rxy*sx*is+2*rx*sxy*is)+2*ry*(2*sxy*sx*is+sy*sxx*is)+7*rx*sxx*sx*is+sy*(3*ry*syy*is+3*sy*ryy*is)+sx*(3*rx*sxx*is+3*rxx*sx*is)+sx*(2*rxx*sx*is+2*rx*sxx*is)+2*sy*(2*rx*sxy*is+ry*sxx*is+2*rxy*sx*is+sy*rxx*is)+7*ry*sy*syy*is+rxx*sx**2*is+4*ry*sxy*sx*is+4*syy*rx*sx*is+2*ryy*sx**2*is+ryy*sy**2*is+sy*(2*sy*ryy*is+2*ry*syy*is))/ds**2)/dr+(3*ryy**2+3*rxx**2+4*rxy**2+4*ry*rxxy+4*rx*rxxx+4*ry*ryyy+2*ryy*rxx+4*rx*rxyy+(2*ry*(-4*sy*rx*sx-2*ry*sx**2)-12*ry**2*sy**2+2*sy*(-2*sy*rx**2-4*ry*rx*sx)-12*rx**2*sx**2)/ds**2)/dr**2+(6*ry**2*ryy*is+4*ry*rxy*rx*is+2*ry*(ry*rxx*is+2*rxy*rx*is)+6*rxx*rx**2*is+2*ryy*rx**2*is)/dr**3+(-8*ry**2*rx**2-4*ry**4-4*rx**4)/dr**4 )

#defineMacro lapSqCoeff4b(is,dr,ds) ( (-3*rxx*rx**2*is-ryy*rx**2*is-2*ry*rxy*rx*is-3*ry**2*ryy*is+2*ry*(-rxy*rx*is-1/2.*ry*rxx*is))/dr**3+(rx**4+2*ry**2*rx**2+ry**4)/dr**4 )


           ! here is the matrix of coefficients for the unknowns u1(-1),v1(-1),u2(-1),v2(-1)
           ! Solve:
           !     
           !       A [ U ] = A [ U(old) ] - [ f ]
!      u1r4(i1,i2,i3,kd)=(8.*(u1(i1+1,i2,i3,kd)-u1(i1-1,i2,i3,kd))-(u1(
!     & i1+2,i2,i3,kd)-u1(i1-2,i2,i3,kd)))*dr114(0)
!      u1x42(i1,i2,i3,kd)= rsxy1(i1,i2,i3,0,0)*u1r4(i1,i2,i3,kd)+rsxy1(
!     & i1,i2,i3,1,0)*u1s4(i1,i2,i3,kd)
!      u1y42(i1,i2,i3,kd)= rsxy1(i1,i2,i3,0,1)*u1r4(i1,i2,i3,kd)+rsxy1(
!     & i1,i2,i3,1,1)*u1s4(i1,i2,i3,kd)
!          a4(0,0) = -is*rsxy1(i1,i2,i3,axis1,0)/(2.*dr1(axis1))    ! coeff of u1(-1) from [u.x+v.y] 
!          a4(0,1) = -is*rsxy1(i1,i2,i3,axis1,1)/(2.*dr1(axis1))    ! coeff of v1(-1) from [u.x+v.y] 
!
!          a4(2,0) =  is*rsxy1(i1,i2,i3,axis1,1)/(2.*dr1(axis1))   ! coeff of u1(-1) from [v.x - u.y] 
!          a4(2,1) = -is*rsxy1(i1,i2,i3,axis1,0)/(2.*dr1(axis1))   ! coeff of v1(-1) from [v.x - u.y] 
!
!          a4(0,2) =  js*rsxy2(j1,j2,j3,axis2,0)/(2.*dr2(axis2))    ! coeff of u2(-1) from [u.x+v.y] 
!          a4(0,3) =  js*rsxy2(j1,j2,j3,axis2,1)/(2.*dr2(axis2))    ! coeff of v2(-1) from [u.x+v.y] 
!
!          a4(2,2) = -js*rsxy2(j1,j2,j3,axis2,1)/(2.*dr2(axis2))   ! coeff of u2(-1) from [v.x - u.y] 
!          a4(2,3) =  js*rsxy2(j1,j2,j3,axis2,0)/(2.*dr2(axis2))   ! coeff of v2(-1) from [v.x - u.y] 


           ! write(debugFile,'(" interface:E: initialized,it=",2i4)') initialized,it
           if( .false. .or. (initialized.eq.0 .and. it.eq.1) )then
             ! form the matrix (and save factor for later use)

             ! 0  [ u.x + v.y ] = 0
             aa8(0,0,0,nn) = -is*8.*rsxy1(i1,i2,i3,axis1,0)*dr114(axis1)     ! coeff of u1(-1) from [u.x+v.y] 
             aa8(0,1,0,nn) = -is*8.*rsxy1(i1,i2,i3,axis1,1)*dr114(axis1)     ! coeff of v1(-1) from [u.x+v.y] 
             aa8(0,4,0,nn) =  is*   rsxy1(i1,i2,i3,axis1,0)*dr114(axis1)     ! u1(-2)
             aa8(0,5,0,nn) =  is*   rsxy1(i1,i2,i3,axis1,1)*dr114(axis1)     ! v1(-2) 
  
             aa8(0,2,0,nn) =  js*8.*rsxy2(j1,j2,j3,axis2,0)*dr214(axis2)     ! coeff of u2(-1) from [u.x+v.y] 
             aa8(0,3,0,nn) =  js*8.*rsxy2(j1,j2,j3,axis2,1)*dr214(axis2)  
             aa8(0,6,0,nn) = -js*   rsxy2(j1,j2,j3,axis2,0)*dr214(axis2) 
             aa8(0,7,0,nn) = -js*   rsxy2(j1,j2,j3,axis2,1)*dr214(axis2)  

           ! 1  [ u.xx + u.yy ] = 0
! this macro comes from deriv.maple
! return the coefficient of u(-1) in uxxx+uxyy
!#defineMacro lapCoeff4a(is,dr,ds) ((-1/3.*rxx*is-1/3.*ryy*is)/dr+(4/3.*rx**2+4/3.*ry**2)/dr**2)

! return the coefficient of u(-2) in uxxx+uxyy
!#defineMacro lapCoeff4b(is,dr,ds) ((1/24.*rxx*is+1/24.*ryy*is)/dr+(-1/12.*rx**2-1/12.*ry**2)/dr**2 )

             setJacobian( aj1, axis1)

             dr0=dr1(axis1)
             ds0=dr1(axis1p1)
             aLap0 = lapCoeff4a(is,dr0,ds0)
             aLap1 = lapCoeff4b(is,dr0,ds0)
  
             setJacobian( aj2, axis2)
             dr0=dr2(axis2)
             ds0=dr2(axis2p1)
             bLap0 = lapCoeff4a(js,dr0,ds0)
             bLap1 = lapCoeff4b(js,dr0,ds0)
  
            if( debug.gt.8 )then
             aa8(1,0,0,nn) = 16.*dx142(axis1)         ! coeff of u1(-1) from [u.xx + u.yy]
             aa8(1,4,0,nn) =    -dx142(axis1)         ! coeff of u1(-2) from [u.xx + u.yy]
              write(debugFile,'(" 4th: lap4: aLap0: rect=",e12.4," curv=",e12.4)') aLap0,aa8(1,0,0,nn)
              ! '
              write(debugFile,'(" 4th: lap4: aLap1: rect=",e12.4," curv=",e12.4)') aLap1,aa8(1,4,0,nn)
              ! '
            end if
  
             aa8(1,0,0,nn) = an1*aLap0       ! coeff of u1(-1) from [n.(u.xx + u.yy)]
             aa8(1,1,0,nn) = an2*aLap0 
             aa8(1,4,0,nn) = an1*aLap1       ! coeff of u1(-2) from [n.(u.xx + u.yy)]
             aa8(1,5,0,nn) = an2*aLap1  
             
             aa8(1,2,0,nn) =-an1*bLap0       ! coeff of u2(-1) from [n.(u.xx + u.yy)]
             aa8(1,3,0,nn) =-an2*bLap0
             aa8(1,6,0,nn) =-an1*bLap1       ! coeff of u2(-2) from [n.(u.xx + u.yy)]
             aa8(1,7,0,nn) =-an2*bLap1
  
           ! 2  [ v.x - u.y ] =0 
!          a8(2,0) =  is*8.*ry1*dx114(axis1)
!          a8(2,1) = -is*8.*rx1*dx114(axis1)    ! coeff of v1(-1) from [v.x - u.y] 
!          a8(2,4) = -is*   ry1*dx114(axis1)
!          a8(2,5) =  is*   rx1*dx114(axis1)
!          a8(2,2) = -js*8.*ry2*dx214(axis2)
!          a8(2,3) =  js*8.*rx2*dx214(axis2)
!          a8(2,6) =  js*   ry2*dx214(axis2)
!          a8(2,7) = -js*   rx2*dx214(axis2)

             aa8(2,0,0,nn) =  is*8.*rsxy1(i1,i2,i3,axis1,1)*dr114(axis1)    
             aa8(2,1,0,nn) = -is*8.*rsxy1(i1,i2,i3,axis1,0)*dr114(axis1)    
             aa8(2,4,0,nn) = -is*   rsxy1(i1,i2,i3,axis1,1)*dr114(axis1)       
             aa8(2,5,0,nn) =  is*   rsxy1(i1,i2,i3,axis1,0)*dr114(axis1)       
  
             aa8(2,2,0,nn) = -js*8.*rsxy2(j1,j2,j3,axis2,1)*dr214(axis2)  
             aa8(2,3,0,nn) =  js*8.*rsxy2(j1,j2,j3,axis2,0)*dr214(axis2)    
             aa8(2,6,0,nn) =  js*   rsxy2(j1,j2,j3,axis2,1)*dr214(axis2)  
             aa8(2,7,0,nn) = -js*   rsxy2(j1,j2,j3,axis2,0)*dr214(axis2) 
  
             ! 3  [ tau.(uv.xx+uv.yy)/eps ] = 0
             aa8(3,0,0,nn) =tau1*aLap0/eps1
             aa8(3,1,0,nn) =tau2*aLap0/eps1
             aa8(3,4,0,nn) =tau1*aLap1/eps1
             aa8(3,5,0,nn) =tau2*aLap1/eps1
  
             aa8(3,2,0,nn) =-tau1*bLap0/eps2
             aa8(3,3,0,nn) =-tau2*bLap0/eps2
             aa8(3,6,0,nn) =-tau1*bLap1/eps2
             aa8(3,7,0,nn) =-tau2*bLap1/eps2
  
  
             ! 4  [ (u.xx+u.yy).x + (v.xx+v.yy).y ] = 0
  
            setJacobian( aj1, axis1)
  
            dr0=dr1(axis1)
            ds0=dr1(axis1p1)
            aLapX0 = xLapCoeff4a(is,dr0,ds0)
            aLapX1 = xLapCoeff4b(is,dr0,ds0)
  
            bLapY0 = yLapCoeff4a(is,dr0,ds0)
            bLapY1 = yLapCoeff4b(is,dr0,ds0)
  
            setJacobian( aj2, axis2)
  
            dr0=dr2(axis2)
            ds0=dr2(axis2p1)
            cLapX0 = xLapCoeff4a(js,dr0,ds0)
            cLapX1 = xLapCoeff4b(js,dr0,ds0)
  
            dLapY0 = yLapCoeff4a(js,dr0,ds0)
            dLapY1 = yLapCoeff4b(js,dr0,ds0)
  
  
            ! 4  [ (u.xx+u.yy).x + (v.xx+v.yy).y ] = 0
            if( debug.gt.8 )then
            aa8(4,0,0,nn)= ( is*rx1*2.*dx122(axis1)*dx112(axis1)+is*rx1*2.*dx122(1)/(2.*dx1(0)))
            aa8(4,1,0,nn)= ( is*ry1*2.*dx122(axis1)*dx112(axis1)+is*ry1*2.*dx122(0)/(2.*dx1(1)))
            aa8(4,4,0,nn)= (-is*rx1   *dx122(axis1)*dx112(axis1) )  
            aa8(4,5,0,nn)= (-is*ry1   *dx122(axis1)*dx112(axis1))
              write(debugFile,'(" 4th: xlap4: aLapX0: rect=",e12.4," curv=",e12.4)') aLapX0,aa8(4,0,0,nn)
              write(debugFile,'(" 4th: xlap4: aLapX1: rect=",e12.4," curv=",e12.4)') aLapX1,aa8(4,4,0,nn)
              write(debugFile,'(" 4th: ylap4: bLapY0: rect=",e12.4," curv=",e12.4)') bLapY0,aa8(4,1,0,nn)
              write(debugFile,'(" 4th: ylap4: bLapY1: rect=",e12.4," curv=",e12.4)') bLapY1,aa8(4,5,0,nn)
              ! '
            end if
  
            aa8(4,0,0,nn)= aLapX0
            aa8(4,1,0,nn)= bLapY0
            aa8(4,4,0,nn)= aLapX1
            aa8(4,5,0,nn)= bLapY1
  
            aa8(4,2,0,nn)=-cLapX0
            aa8(4,3,0,nn)=-dLapY0
            aa8(4,6,0,nn)=-cLapX1
            aa8(4,7,0,nn)=-dLapY1
  
            ! 5  [ {(Delta v).x - (Delta u).y}/eps ] =0  -> [ {(v.xxx+v.xyy)-(u.xxy+u.yyy)}/eps ] = 0
  
            aa8(5,0,0,nn)=-bLapY0/eps1
            aa8(5,1,0,nn)= aLapX0/eps1
            aa8(5,4,0,nn)=-bLapY1/eps1
            aa8(5,5,0,nn)= aLapX1/eps1
  
            aa8(5,2,0,nn)= dLapY0/eps2
            aa8(5,3,0,nn)=-cLapX0/eps2
            aa8(5,6,0,nn)= dLapY1/eps2
            aa8(5,7,0,nn)=-cLapX1/eps2
  
  
             ! 6  [ n.Delta^2 u/eps ] = 0
  
             ! assign rx,ry,rxx,rxy,... 

             setJacobian( aj1, axis1)

             dr0=dr1(axis1)
             ds0=dr1(axis1p1)
             aLapSq0 = lapSqCoeff4a(is,dr0,ds0)
             aLapSq1 = lapSqCoeff4b(is,dr0,ds0)
  
             if( debug.gt.8 )then
               aa8(6,0,0,nn) = -(4./(dx1(axis1)**4) +4./(dx1(0)**2*dx1(1)**2) )
               aa8(6,4,0,nn) =   1./(dx1(axis1)**4)
               write(debugFile,'(" 4th: lapSq: aLapSq0: rect=",e12.4," curv=",e12.4)') aLapSq0,aa8(6,0,0,nn)
               ! '
               write(debugFile,'(" 4th: lapSq: aLapSq1: rect=",e12.4," curv=",e12.4)') aLapSq1,aa8(6,4,0,nn)
               ! '
             end if
  
             aa8(6,0,0,nn) = an1*aLapSq0/eps1
             aa8(6,1,0,nn) = an2*aLapSq0/eps1
             aa8(6,4,0,nn) = an1*aLapSq1/eps1
             aa8(6,5,0,nn) = an2*aLapSq1/eps1
  
             setJacobian( aj2, axis2)
             dr0=dr2(axis2)
             ds0=dr2(axis2p1)
             bLapSq0 = lapSqCoeff4a(js,dr0,ds0)
             bLapSq1 = lapSqCoeff4b(js,dr0,ds0)
  
             aa8(6,2,0,nn) = -an1*bLapSq0/eps2
             aa8(6,3,0,nn) = -an2*bLapSq0/eps2
             aa8(6,6,0,nn) = -an1*bLapSq1/eps2
             aa8(6,7,0,nn) = -an2*bLapSq1/eps2
  
             ! 7  [ tau.Delta^2 v/eps^2 ] = 0 
             aa8(7,0,0,nn) = tau1*aLapSq0/eps1**2
             aa8(7,1,0,nn) = tau2*aLapSq0/eps1**2
             aa8(7,4,0,nn) = tau1*aLapSq1/eps1**2
             aa8(7,5,0,nn) = tau2*aLapSq1/eps1**2
  
             aa8(7,2,0,nn) = -tau1*bLapSq0/eps2**2
             aa8(7,3,0,nn) = -tau2*bLapSq0/eps2**2
             aa8(7,6,0,nn) = -tau1*bLapSq1/eps2**2
             aa8(7,7,0,nn) = -tau2*bLapSq1/eps2**2
  
             ! save a copy of the matrix
             do n2=0,7
             do n1=0,7
               aa8(n1,n2,1,nn)=aa8(n1,n2,0,nn)
             end do
             end do
  
             ! solve A Q = F
             ! factor the matrix
             numberOfEquations=8
             call dgeco( aa8(0,0,0,nn), numberOfEquations, numberOfEquations, ipvt8(0,nn),rcond,work(0))

             if( debug.gt.3 ) write(debugFile,'(" --> 4cth: i1,i2=",2i4," rcond=",e10.2)') i1,i2,rcond
             ! '
           end if


           q(0) = u1(i1-is1,i2-is2,i3,ex)
           q(1) = u1(i1-is1,i2-is2,i3,ey)
           q(2) = u2(j1-js1,j2-js2,j3,ex)
           q(3) = u2(j1-js1,j2-js2,j3,ey)

           q(4) = u1(i1-2*is1,i2-2*is2,i3,ex)
           q(5) = u1(i1-2*is1,i2-2*is2,i3,ey)
           q(6) = u2(j1-2*js1,j2-2*js2,j3,ex)
           q(7) = u2(j1-2*js1,j2-2*js2,j3,ey)

       if( debug.gt.4 ) write(debugFile,'(" --> 4cth: i1,i2=",2i4," q=",8e10.2)') i1,i2,(q(n),n=0,7)

           ! subtract off the contributions from the initial (wrong) values at the ghost points:
           do n=0,7
             f(n) = (aa8(n,0,1,nn)*q(0)+aa8(n,1,1,nn)*q(1)+aa8(n,2,1,nn)*q(2)+aa8(n,3,1,nn)*q(3)+\
                     aa8(n,4,1,nn)*q(4)+aa8(n,5,1,nn)*q(5)+aa8(n,6,1,nn)*q(6)+aa8(n,7,1,nn)*q(7)) - f(n)
           end do

                                ! '

           ! solve A Q = F
           job=0
           numberOfEquations=8
           call dgesl( aa8(0,0,0,nn), numberOfEquations, numberOfEquations, ipvt8(0,nn), f(0), job)

       if( debug.gt.4 )then
          write(debugFile,'(" --> 4cth: i1,i2=",2i4," f(solve)=",8e10.2)') i1,i2,(f(n),n=0,7)
          write(debugFile,'(" --> 4cth: i1,i2=",2i4,"      f-q=",8e10.2)') i1,i2,(f(n)-q(n),n=0,7)
       end if
           ! '

           if( .true. )then
           u1(i1-is1,i2-is2,i3,ex)=f(0)
           u1(i1-is1,i2-is2,i3,ey)=f(1)
           u2(j1-js1,j2-js2,j3,ex)=f(2)
           u2(j1-js1,j2-js2,j3,ey)=f(3)

           u1(i1-2*is1,i2-2*is2,i3,ex)=f(4)
           u1(i1-2*is1,i2-2*is2,i3,ey)=f(5)
           u2(j1-2*js1,j2-2*js2,j3,ex)=f(6)
           u2(j1-2*js1,j2-2*js2,j3,ey)=f(7)
           end if

          if( debug.gt.0 )then ! re-evaluate

           ! compute the maximum change in the solution for this iteration
           do n=0,7
             err=max(err,abs(q(n)-f(n)))
           end do

           evalDerivs2dOrder4()
           eval2dJumpOrder4()
    
           if( debug.gt.3 ) write(debugFile,'(" --> 4cth: i1,i2=",2i4," f(re-eval)=",8e10.2)') i1,i2,f(0),f(1),f(2),f(3),f(4),f(5),f(6),f(7)
             ! '
          end if

           ! ******************************************************
           ! solve for Hz
           !  [ w.n/eps ] = 0
           !  [ lap(w)/eps ] = 0
           !  [ lap(w).n/eps**2 ] = 0
           !  [ lapSq(w)/eps**2 ] = 0

#beginMacro evalMagneticField2dJumpOrder4()
 f(0)=(an1*w1x+an2*w1y)/eps1 - \
      (an1*w2x+an2*w2y)/eps2
 f(1)=w1Lap/eps1 - \
      w2Lap/eps2
 f(2)=(an1*(w1xxx+w1xyy)+an2*(w1xxy+w1yyy))/eps1**2 - \
      (an1*(w2xxx+w2xyy)+an2*(w2xxy+w2yyy))/eps2**2
 f(3)=w1LapSq/eps1**2 - \
      w2LapSq/eps2**2
 if( twilightZone.eq.1 )then

   call ogderiv(ep, 0,1,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t, hz, wex  )
   call ogderiv(ep, 0,0,1,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t, hz, wey  )
   call ogderiv(ep, 0,2,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t, hz, wexx )
   call ogderiv(ep, 0,0,2,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t, hz, weyy )

   call ogderiv(ep, 0,3,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t, hz, wexxx )
   call ogderiv(ep, 0,2,1,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t, hz, wexxy )
   call ogderiv(ep, 0,1,2,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t, hz, wexyy )
   call ogderiv(ep, 0,0,3,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t, hz, weyyy )

   call ogderiv(ep, 0,4,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t, hz, wexxxx )
   call ogderiv(ep, 0,2,2,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t, hz, wexxyy )
   call ogderiv(ep, 0,0,4,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t, hz, weyyyy )

   weLap = wexx + weyy
   weLapSq = wexxxx + 2.*wexxyy + weyyyy

   f(0) = f(0) - (an1*wex+an2*wey)*(1./eps1 - 1./eps2)
   f(1) = f(1) - ( weLap )*(1./eps1 - 1./eps2)
   f(2) = f(2) - (an1*(wexxx+wexyy)+an2*(wexxy+weyyy))*(1./eps1**2 - 1./eps2**2)
   f(3) = f(3) - weLapSq*(1./eps1**2 - 1./eps2**2)

 end if
#endMacro
           ! first evaluate the equations we want to solve with the wrong values at the ghost points:
           evalMagneticDerivs2dOrder4()
           evalMagneticField2dJumpOrder4()

           if( .false. .or. (initialized.eq.0 .and. it.eq.1) )then
             ! form the matrix for computing Hz (and save factor for later use)

             ! 1: [ w.n/eps ] = 0
             a0 = (an1*rsxy1(i1,i2,i3,axis1,0)+an2*rsxy1(i1,i2,i3,axis1,1))*dr114(axis1)/eps1
             b0 = (an1*rsxy2(j1,j2,j3,axis2,0)+an2*rsxy2(j1,j2,j3,axis2,1))*dr214(axis2)/eps2
             aa4(0,0,0,nn) = -is*8.*a0
             aa4(0,2,0,nn) =  is*   a0
             aa4(0,1,0,nn) =  js*8.*b0
             aa4(0,3,0,nn) = -js*   b0
  
             ! 2: [ lap(w)/eps ] = 0 
             aa4(1,0,0,nn) = aLap0/eps1
             aa4(1,2,0,nn) = aLap1/eps1
             aa4(1,1,0,nn) =-bLap0/eps2
             aa4(1,3,0,nn) =-bLap1/eps2
  
             ! 3  [ (an1*(w.xx+w.yy).x + an2.(w.xx+w.yy).y)/eps**2 ] = 0
             aa4(2,0,0,nn)= (an1*aLapX0+an2*bLapY0)/eps1**2
             aa4(2,2,0,nn)= (an1*aLapX1+an2*bLapY1)/eps1**2
             aa4(2,1,0,nn)=-(an1*cLapX0+an2*dLapY0)/eps2**2
             aa4(2,3,0,nn)=-(an1*cLapX1+an2*dLapY1)/eps2**2
  
             ! 4 [ lapSq(w)/eps**2 ] = 0 
             aa4(3,0,0,nn) = aLapSq0/eps1**2
             aa4(3,2,0,nn) = aLapSq1/eps1**2
             aa4(3,1,0,nn) =-bLapSq0/eps2**2
             aa4(3,3,0,nn) =-bLapSq1/eps2**2

             ! save a copy of the matrix
             do n2=0,3
             do n1=0,3
               aa4(n1,n2,1,nn)=aa4(n1,n2,0,nn)
             end do
             end do
  
             ! factor the matrix
             numberOfEquations=4
             call dgeco( aa4(0,0,0,nn), numberOfEquations, numberOfEquations, ipvt4(0,nn),rcond,work(0))
           end if

           q(0) = u1(i1-is1,i2-is2,i3,hz)
           q(1) = u2(j1-js1,j2-js2,j3,hz)
           q(2) = u1(i1-2*is1,i2-2*is2,i3,hz)
           q(3) = u2(j1-2*js1,j2-2*js2,j3,hz)

           ! subtract off the contributions from the wrong values at the ghost points:
           do n=0,3
             f(n) = (aa4(n,0,1,nn)*q(0)+aa4(n,1,1,nn)*q(1)+aa4(n,2,1,nn)*q(2)+aa4(n,3,1,nn)*q(3)) - f(n)
           end do
           ! solve
           numberOfEquations=4
           job=0
           call dgesl( aa4(0,0,0,nn), numberOfEquations, numberOfEquations, ipvt4(0,nn), f(0), job)

           u1(i1-is1,i2-is2,i3,hz)=f(0)
           u2(j1-js1,j2-js2,j3,hz)=f(1)
           u1(i1-2*is1,i2-2*is2,i3,hz)=f(2)
           u2(j1-2*js1,j2-2*js2,j3,hz)=f(3)

          if( debug.gt.0 )then ! re-evaluate

           evalMagneticDerivs2dOrder4()
           evalMagneticField2dJumpOrder4()
    
           if( debug.gt.3 ) write(debugFile,'(" --> 4cth: i1,i2=",2i4," hz-f(re-eval)=",8e10.2)') i1,i2,f(0),f(1),f(2),f(3)
             ! '
          end if



           ! ***********************

           ! u1(i1-is1,i2-is2,i3,hz)=u2(j1+js1,j2+js2,j3,hz) 
           ! u2(j1-js1,j2-js2,j3,hz)=u1(i1+is1,i2+is2,i3,hz)
           ! u1(i1-2*is1,i2-2*is2,i3,hz)=u2(j1+2*js1,j2+2*js2,j3,hz) 
           ! u2(j1-2*js1,j2-2*js2,j3,hz)=u1(i1+2*is1,i2+2*is2,i3,hz)

         endLoopsMask2d()
         ! =============== end loops =======================
      
         periodicUpdate2d(u1,boundaryCondition1,gridIndexRange1,side1,axis1)
         periodicUpdate2d(u2,boundaryCondition2,gridIndexRange2,side2,axis2)

           if( debug.gt.0 )then 
             write(debugFile,'(" ***it=",i2," max-diff = ",e11.2)') it,err
           end if
           if( debug.gt.3 )then 
             write(*,'(" ***it=",i2," max-diff = ",e11.2)') it,err
           end if
         end do ! ************** end iteration **************


         ! now make sure that div(u)=0 etc.
         if( .false. )then
!*         beginLoops2d() ! =============== start loops =======================

           ! 0  [ u.x + v.y ] = 0
!           a8(0,0) = -is*8.*rsxy1(i1,i2,i3,axis1,0)*dr114(axis1)     ! coeff of u1(-1) from [u.x+v.y] 
!           a8(0,1) = -is*8.*rsxy1(i1,i2,i3,axis1,1)*dr114(axis1)     ! coeff of v1(-1) from [u.x+v.y] 
!           a8(0,4) =  is*   rsxy1(i1,i2,i3,axis1,0)*dr114(axis1)     ! u1(-2)
!           a8(0,5) =  is*   rsxy1(i1,i2,i3,axis1,1)*dr114(axis1)     ! v1(-2) 

!           a8(0,2) =  js*8.*rsxy2(j1,j2,j3,axis2,0)*dr214(axis2)     ! coeff of u2(-1) from [u.x+v.y] 
!           a8(0,3) =  js*8.*rsxy2(j1,j2,j3,axis2,1)*dr214(axis2)  
!           a8(0,6) = -js*   rsxy2(j1,j2,j3,axis2,0)*dr214(axis2) 
!           a8(0,7) = -js*   rsxy2(j1,j2,j3,axis2,1)*dr214(axis2) 

           ! first evaluate the equations we want to solve with the wrong values at the ghost points:
!*           divu=u1x42(i1,i2,i3,ex)+u1y42(i1,i2,i3,ey)
!*           a0=is*   rsxy1(i1,i2,i3,axis1,0)*dr114(axis1)
!*           a1=is*   rsxy1(i1,i2,i3,axis1,1)*dr114(axis1)
!*           aNormSq=a0**2+a1**2
!*           ! now project:  a.uNew = a.uOld - div  ->  (div-a.uOld)+a.uNew = div(uNew) = 0
!*           u1(i1-2*is1,i2-2*is2,i3,ex)=u1(i1-2*is1,i2-2*is2,i3,ex)-divu*a0/aNormSq
!*           u1(i1-2*is1,i2-2*is2,i3,ey)=u1(i1-2*is1,i2-2*is2,i3,ey)-divu*a1/aNormSq
!*
!*           divu=u2x42(j1,j2,j3,ex)+u2y42(j1,j2,j3,ey)
!*           a0=js*   rsxy2(j1,j2,j3,axis2,0)*dr214(axis2) 
!*           a1=js*   rsxy2(j1,j2,j3,axis2,1)*dr214(axis2) 
!*           aNormSq=a0**2+a1**2
!*
!*           u2(j1-2*js1,j2-2*js2,j3,ex)=u2(j1-2*js1,j2-2*js2,j3,ex)-divu*a0/aNormSq
!*           u2(j1-2*js1,j2-2*js2,j3,ey)=u2(j1-2*js1,j2-2*js2,j3,ey)-divu*a1/aNormSq
!*
!*           if( debug.gt.0 )then
!*             divu=u1x42(i1,i2,i3,ex)+u1y42(i1,i2,i3,ey)
!*              write(debugFile,'(" --> 4cth: eval div1,div2=",2e10.2)') u1x42(i1,i2,i3,ex)+u1y42(i1,i2,i3,ey),u2x42(j1,j2,j3,ex)+u2y42(j1,j2,j3,ey)
!*           end if
!*         endLoops2d()
       end if



       else if( nd.eq.3 .and. (orderOfAccuracy.eq.2 .or. orderOfAccuracy.eq.2 ) .and. gridType.eq.curvilinear )then

         ! *******************************
         ! ***** 3D curvilinear case *****
         ! *******************************

        ! **NOTE** For now we use this 2nd order version for 4th order and just assign the 2nd ghost line below

        if( solveForH .ne.0 )then
          stop 3017
        end if

#perl $DIM=3; $GRIDTYPE="curvilinear"; $ORDER=2; 

        if( .false. )then
          beginGhostLoops3d()
           write(debugFile,'(" -->START v1(",i2,":",i2,",",i2,",",i2,") =",3f9.4)') i1-1,i1+1,i2,i3,u1(i1-1,i2,i3,ey),u1(i1,i2,i3,ey),u1(i1+1,i2,i3,ey)
           ! '
          endLoops3d()
        end if

         ! ---- first satisfy the jump conditions on the boundary --------
         !    [ eps n.u ] = 0
         !    [ tau.u ] = 0
         boundaryJumpConditions(3,curvilinear)

        if( .false. )then
          beginGhostLoops3d()
           write(debugFile,'(" -->JUMP v1(",i2,":",i2,",",i2,",",i2,") =",3f9.4)') i1-1,i1+1,i2,i3,u1(i1-1,i2,i3,ey),u1(i1,i2,i3,ey),u1(i1+1,i2,i3,ey)
           ! '
          endLoops3d()
        end if
         ! initialization step: assign first ghost line by extrapolation
         ! NOTE: assign ghost points outside the ends
         if( .true. )then
         beginGhostLoops3d()
            u1(i1-is1,i2-is2,i3-is3,ex)=extrap3(u1,i1,i2,i3,ex,is1,is2,is3)
            u1(i1-is1,i2-is2,i3-is3,ey)=extrap3(u1,i1,i2,i3,ey,is1,is2,is3)
            u1(i1-is1,i2-is2,i3-is3,ez)=extrap3(u1,i1,i2,i3,ez,is1,is2,is3)

            u2(j1-js1,j2-js2,j3-js3,ex)=extrap3(u2,j1,j2,j3,ex,js1,js2,js3)
            u2(j1-js1,j2-js2,j3-js3,ey)=extrap3(u2,j1,j2,j3,ey,js1,js2,js3)
            u2(j1-js1,j2-js2,j3-js3,ez)=extrap3(u2,j1,j2,j3,ez,js1,js2,js3)
         endLoops3d()
         end if

         if( .false. )then
          ! just copy values from ghost points for now -- this will be the true soln if eps1=eps2 and grids match
          beginLoops3d()
           u1(i1-is1,i2-is2,i3-is3,ex)=u2(j1+js1,j2+js2,j3+js3,ex)
           u1(i1-is1,i2-is2,i3-is3,ey)=u2(j1+js1,j2+js2,j3+js3,ey)
           u1(i1-is1,i2-is2,i3-is3,ez)=u2(j1+js1,j2+js2,j3+js3,ez) 
           u2(j1-js1,j2-js2,j3-js3,ex)=u1(i1+is1,i2+is2,i3+is3,ex)
           u2(j1-js1,j2-js2,j3-js3,ey)=u1(i1+is1,i2+is2,i3+is3,ey)
           u2(j1-js1,j2-js2,j3-js3,ez)=u1(i1+is1,i2+is2,i3+is3,ez)
          endLoops3d()
         end if

        if( .false. )then
          beginGhostLoops3d()
           write(debugFile,'(" -->EXTRAP v1(",i2,":",i2,",",i2,",",i2,") =",3f9.4)') i1-1,i1+1,i2,i3,u1(i1-1,i2,i3,ey),u1(i1,i2,i3,ey),u1(i1+1,i2,i3,ey)
           ! '
          endLoops3d()
        end if


         ! here are the jump conditions for the ghost points
         !   [ div(E) n + (curl(E)- n.curl(E) n )/mu ] =0                 (3 eqns)
         !   [ Lap(E)/(eps*mu) + (1/mu)*(1-1/eps)*( n.Lap(E) ) n ] = 0    (3 eqns)

         ! These correspond to the 6 conditions:
         !   [ div(E) ] =0 
         !   [ tau. curl(E)/mu ] = 0       (2 tangents)
         !   [ n.Lap(E)/mu ] = 0 
         !   [ tau.Lap(E)/(eps*mu) ] = 0   (2 tangents)


         beginLoopsMask3d()

           ! here is the normal (assumed to be the same on both sides)
           an1=rsxy1(i1,i2,i3,axis1,0)   ! normal (an1,an2)
           an2=rsxy1(i1,i2,i3,axis1,1)
           an3=rsxy1(i1,i2,i3,axis1,2)
           aNorm=max(epsx,sqrt(an1**2+an2**2+an3**2))
           an1=an1/aNorm
           an2=an2/aNorm
           an3=an3/aNorm

#beginMacro eval3dJumpOrder2()
 divE1 = u1x+v1y+w1z
 curlE1x = w1y-v1z
 curlE1y = u1z-w1x
 curlE1z = v1x-u1y
 nDotCurlE1=an1*curlE1x+an2*curlE1y+an3*curlE1z
 nDotLapE1 = an1*u1Lap + an2*v1Lap + an3*w1Lap

 divE2 = u2x+v2y+w2z
 curlE2x = w2y-v2z
 curlE2y = u2z-w2x
 curlE2z = v2x-u2y
 nDotCurlE2=an1*curlE2x+an2*curlE2y+an3*curlE2z
 nDotLapE2 = an1*u2Lap + an2*v2Lap + an3*w2Lap


 f(0)=( divE1*an1 + (curlE1x- nDotCurlE1*an1)/mu1 ) - ( divE2*an1 + (curlE2x- nDotCurlE2*an1)/mu2 )
 f(1)=( divE1*an2 + (curlE1y- nDotCurlE1*an2)/mu1 ) - ( divE2*an2 + (curlE2y- nDotCurlE2*an2)/mu2 )
 f(2)=( divE1*an3 + (curlE1z- nDotCurlE1*an3)/mu1 ) - ( divE2*an3 + (curlE2z- nDotCurlE2*an3)/mu2 )


 f(3)=( u1Lap/(epsmu1) + cem1*nDotLapE1*an1 ) - ( u2Lap/(epsmu2) + cem2*nDotLapE2*an1 )
 f(4)=( v1Lap/(epsmu1) + cem1*nDotLapE1*an2 ) - ( v2Lap/(epsmu2) + cem2*nDotLapE2*an2 )
 f(5)=( w1Lap/(epsmu1) + cem1*nDotLapE1*an3 ) - ( w2Lap/(epsmu2) + cem2*nDotLapE2*an3 )

 if( twilightZone.eq.1 )then

   call ogderiv(ep, 0,1,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),xy1(i1,i2,i3,2),t, ex, uex  )
   call ogderiv(ep, 0,0,1,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),xy1(i1,i2,i3,2),t, ex, uey  )
   call ogderiv(ep, 0,0,0,1, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),xy1(i1,i2,i3,2),t, ex, uez  )
   call ogderiv(ep, 0,2,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),xy1(i1,i2,i3,2),t, ex, uexx )
   call ogderiv(ep, 0,0,2,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),xy1(i1,i2,i3,2),t, ex, ueyy )
   call ogderiv(ep, 0,0,0,2, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),xy1(i1,i2,i3,2),t, ex, uezz )

   call ogderiv(ep, 0,1,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),xy1(i1,i2,i3,2),t, ey, vex  )
   call ogderiv(ep, 0,0,1,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),xy1(i1,i2,i3,2),t, ey, vey  )
   call ogderiv(ep, 0,0,0,1, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),xy1(i1,i2,i3,2),t, ey, vez  )
   call ogderiv(ep, 0,2,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),xy1(i1,i2,i3,2),t, ey, vexx )
   call ogderiv(ep, 0,0,2,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),xy1(i1,i2,i3,2),t, ey, veyy )
   call ogderiv(ep, 0,0,0,2, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),xy1(i1,i2,i3,2),t, ey, vezz )

   call ogderiv(ep, 0,1,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),xy1(i1,i2,i3,2),t, ez, wex  )
   call ogderiv(ep, 0,0,1,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),xy1(i1,i2,i3,2),t, ez, wey  )
   call ogderiv(ep, 0,0,0,1, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),xy1(i1,i2,i3,2),t, ez, wez  )
   call ogderiv(ep, 0,2,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),xy1(i1,i2,i3,2),t, ez, wexx )
   call ogderiv(ep, 0,0,2,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),xy1(i1,i2,i3,2),t, ez, weyy )
   call ogderiv(ep, 0,0,0,2, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),xy1(i1,i2,i3,2),t, ez, wezz )

   ueLap = uexx+ueyy+uezz
   veLap = vexx+veyy+vezz
   weLap = wexx+weyy+wezz

   curlEex = wey-vez
   curlEey = uez-wex
   curlEez = vex-uey
   nDotCurlEe=an1*curlEex+an2*curlEey+an3*curlEez
   nDotLapEe=an1*ueLap+an2*veLap+an3*weLap

   f(0)= f(0) - ( (curlEex- nDotCurlEe*an1)*(1./mu1-1./mu2) )
   f(1)= f(1) - ( (curlEey- nDotCurlEe*an2)*(1./mu1-1./mu2) )
   f(2)= f(2) - ( (curlEez- nDotCurlEe*an3)*(1./mu1-1./mu2) )

   f(3)= f(3) - ( ueLap*(1./epsmu1-1./epsmu2) + nDotLapEe*an1*(cem1-cem2) )
   f(4)= f(4) - ( veLap*(1./epsmu1-1./epsmu2) + nDotLapEe*an2*(cem1-cem2) )
   f(5)= f(5) - ( weLap*(1./epsmu1-1./epsmu2) + nDotLapEe*an3*(cem1-cem2) ) 

 end if

#endMacro

           ! --- first evaluate the equations we want to solve with the wrong values at the ghost points:

           cem1=(1.-1./eps1)/mu1
           cem2=(1.-1./eps2)/mu2
           ! evalInterfaceDerivatives3d
           evalInterfaceDerivatives3d()
           eval3dJumpOrder2()

           if( debug.gt.4 )then
            write(debugFile,'(" --> 3d-order2-curv: i1,i2,i3=",3i4," f(start)=",6f8.3)') i1,i2,i3,f(0),f(1),f(2),f(3),f(4),f(5)
            ! '
            write(debugFile,'(" --> u1x,u1y,u1z,v1x,v1y,v1z=",6f8.4)') u1x,u1y,u1z,v1x,v1y,v1z
            write(debugFile,'(" --> u2x,u2y,u2z,v2x,v2y,v2z=",6f8.4)') u2x,u2y,u2z,v2x,v2y,v2z
 
            write(debugFile,'(" --> vv1r,vv1s,vv1t         =",3e9.2)') vv1r,vv1s,vv1t
            do k3=-1,1
            do k2=-1,1
            write(debugFile,'(" --> v1: =",3f8.4)') u1(i1-1,i2+k2,i3+k3,ey),u1(i1,i2+k2,i3+k3,ey),u1(i1+1,i2+k2,i3+k3,ey)
            end do
            end do
            do k3=-1,1
            do k2=-1,1
            write(debugFile,'(" --> v2: =",3f8.4)') u2(j1-1,j2+k2,j3+k3,ey),u2(j1,j2+k2,j3+k3,ey),u2(j1+1,j2+k2,j3+k3,ey)
            end do
            end do
            ! '
           end if

           ! here is the matrix of coefficients for the unknowns u1(-1),v1(-1),w1(-1),  u2(-1),v2(-1),w2(-1)
           ! Solve:
           !     
           !       A [ U ] = A [ U(old) ] - [ f ]

           c1x = -is*rsxy1(i1,i2,i3,axis1,0)/(2.*dr1(axis1))    ! coeff of u1(-1) from D.x
           c1y = -is*rsxy1(i1,i2,i3,axis1,1)/(2.*dr1(axis1))    ! coeff of u1(-1) from D.y 
           c1z = -is*rsxy1(i1,i2,i3,axis1,2)/(2.*dr1(axis1))    ! coeff of u1(-1) from D.z

           c2x = -js*rsxy2(j1,j2,j3,axis2,0)/(2.*dr2(axis2))
           c2y = -js*rsxy2(j1,j2,j3,axis2,1)/(2.*dr2(axis2))
           c2z = -js*rsxy2(j1,j2,j3,axis2,2)/(2.*dr2(axis2))

           rxx1(0,0,0)=aj1rxx
           rxx1(0,1,1)=aj1ryy
           rxx1(0,2,2)=aj1rzz
           rxx1(1,0,0)=aj1sxx
           rxx1(1,1,1)=aj1syy
           rxx1(1,2,2)=aj1szz
           rxx1(2,0,0)=aj1txx
           rxx1(2,1,1)=aj1tyy
           rxx1(2,2,2)=aj1tzz

           rxx2(0,0,0)=aj2rxx
           rxx2(0,1,1)=aj2ryy
           rxx2(0,2,2)=aj2rzz
           rxx2(1,0,0)=aj2sxx
           rxx2(1,1,1)=aj2syy
           rxx2(1,2,2)=aj2szz
           rxx2(2,0,0)=aj2txx
           rxx2(2,1,1)=aj2tyy
           rxx2(2,2,2)=aj2tzz

           ! clap1 : coeff of u(-1) from lap = u.xx + u.yy + u.zz

           ! clap1=(rsxy1(i1,i2,i3,axis1,0)**2+rsxy1(i1,i2,i3,axis1,1)**2)/(dr1(axis1)**2) \
           !           -is*(rsxy1x22(i1,i2,i3,axis1,0)+rsxy1y22(i1,i2,i3,axis1,1))/(2.*dr1(axis1))
           ! clap2=(rsxy2(j1,j2,j3,axis2,0)**2+rsxy2(j1,j2,j3,axis2,1)**2)/(dr2(axis2)**2) \
           !             -js*(rsxy2x22(j1,j2,j3,axis2,0)+rsxy2y22(j1,j2,j3,axis2,1))/(2.*dr2(axis2)) 
           clap1=(rsxy1(i1,i2,i3,axis1,0)**2+rsxy1(i1,i2,i3,axis1,1)**2+rsxy1(i1,i2,i3,axis1,2)**2)/(dr1(axis1)**2) \
                     -is*(rxx1(axis1,0,0)+rxx1(axis1,1,1)+rxx1(axis1,2,2))/(2.*dr1(axis1))
           clap2=(rsxy2(j1,j2,j3,axis2,0)**2+rsxy2(j1,j2,j3,axis2,1)**2+rsxy2(j1,j2,j3,axis2,2)**2)/(dr2(axis2)**2) \
                     -js*(rxx2(axis2,0,0)+rxx2(axis2,1,1)+rxx2(axis2,2,2))/(2.*dr2(axis2)) 

           ! cdivE1 =  u.c1x + v.c1y + w.c1z
           ! nDotCurlE1 = (w1y-v1z)*an1 + (u1z-w1x)*an2 + (v1x-u1y)*an3

           ! (u.x+v.y+w.z)*an1 + ( w1y-v1z - nDotCurlE1*an1)/mu1
           a6(0,0) = ( c1x*an1 + (         - (c1z*an2-c1y*an3)*an1 )/mu1 ) ! coeff of u1(-1)
           a6(0,1) = ( c1y*an1 + (    -c1z - (c1x*an3-c1z*an1)*an1 )/mu1 ) ! coeff of v1(-1)
           a6(0,2) = ( c1z*an1 + ( c1y     - (c1y*an1-c1x*an2)*an1 )/mu1 ) ! coeff of w1(-1)

           a6(0,3) =-( c2x*an1 + (         - (c2z*an2-c2y*an3)*an1 )/mu2 ) ! coeff of u2(-1)
           a6(0,4) =-( c2y*an1 + (    -c2z - (c2x*an3-c2z*an1)*an1 )/mu2 ) ! coeff of v2(-1)
           a6(0,5) =-( c2z*an1 + ( c2y     - (c2y*an1-c2x*an2)*an1 )/mu2 ) ! coeff of w2(-1)

           ! (u.x+v.y+w.z)*an2 + ( u1z-w1x - nDotCurlE1*an2)/mu1
           a6(1,0) = ( c1x*an2 + ( c1z     - (c1z*an2-c1y*an3)*an2 )/mu1 ) ! coeff of u1(-1)
           a6(1,1) = ( c1y*an2 + (         - (c1x*an3+c1z*an1)*an2 )/mu1 ) ! coeff of v1(-1)
           a6(1,2) = ( c1z*an2 + (    -c1x - (c1y*an1-c1x*an2)*an2 )/mu1 ) ! coeff of w1(-1)

           a6(1,3) =-( c2x*an2 + ( c2z     - (c2z*an2-c2y*an3)*an2 )/mu2 ) ! coeff of u2(-1)
           a6(1,4) =-( c2y*an2 + (         - (c2x*an3+c2z*an1)*an2 )/mu2 ) ! coeff of v2(-1)
           a6(1,5) =-( c2z*an2 + (    -c2x - (c2y*an1-c2x*an2)*an2 )/mu2 ) ! coeff of w2(-1)

           ! (u.x+v.y+w.z)*an3 + ( v1x-u1y - nDotCurlE1*an2)/mu1
           a6(2,0) = ( c1x*an3 + (    -c1y - (c1z*an2-c1y*an3)*an3 )/mu1 ) ! coeff of u1(-1)
           a6(2,1) = ( c1y*an3 + ( c1x     - (c1x*an3+c1z*an1)*an3 )/mu1 ) ! coeff of v1(-1)
           a6(2,2) = ( c1z*an3 + (         - (c1y*an1-c1x*an2)*an3 )/mu1 ) ! coeff of w1(-1)

           a6(2,3) =-( c2x*an3 + (    -c2y - (c2z*an2-c2y*an3)*an3 )/mu2 ) ! coeff of u2(-1)
           a6(2,4) =-( c2y*an3 + ( c2x     - (c2x*an3+c2z*an1)*an3 )/mu2 ) ! coeff of v2(-1)
           a6(2,5) =-( c2z*an3 + (         - (c2y*an1-c2x*an2)*an3 )/mu2 ) ! coeff of w2(-1)

           !  u1Lap/(epsmu1) + cem1*( an1*u1Lap + an2*v1Lap + an3*w1Lap )*an1
           a6(3,0) = ( clap1/(epsmu1) + cem1*( an1*clap1                         )*an1 ) ! coeff of u1(-1)
           a6(3,1) = (                  cem1*(             an2*clap1             )*an1 )
           a6(3,2) = (                  cem1*(                         an3*clap1 )*an1 )

           a6(3,3) =-( clap2/(epsmu2) + cem2*( an1*clap2                         )*an1 ) ! coeff of u2(-1)
           a6(3,4) =-(                  cem2*(             an2*clap2             )*an1 )
           a6(3,5) =-(                  cem2*(                         an3*clap2 )*an1 )

           !  v1Lap/(epsmu1) + cem1*( an1*u1Lap + an2*v1Lap + an3*w1Lap )*an2
           a6(4,0) = (                  cem1*( an1*clap1                         )*an2 ) ! coeff of u1(-1)
           a6(4,1) = ( clap1/(epsmu1) + cem1*(             an2*clap1             )*an2 )
           a6(4,2) = (                  cem1*(                         an3*clap1 )*an2 )

           a6(4,3) =-(                  cem2*( an1*clap2                         )*an2 ) ! coeff of u2(-1)
           a6(4,4) =-( clap2/(epsmu2) + cem2*(             an2*clap2             )*an2 )
           a6(4,5) =-(                  cem2*(                         an3*clap2 )*an2 )

           !  w1Lap/(epsmu1) + cem1*( an1*u1Lap + an2*v1Lap + an3*w1Lap )*an3
           a6(5,0) = (                  cem1*( an1*clap1                         )*an3 ) ! coeff of u1(-1)
           a6(5,1) = (                  cem1*(             an2*clap1             )*an3 )
           a6(5,2) = ( clap1/(epsmu1) + cem1*(                         an3*clap1 )*an3 )

           a6(5,3) =-(                  cem2*( an1*clap2                         )*an3 ) ! coeff of u2(-1)
           a6(5,4) =-(                  cem2*(             an2*clap2             )*an3 )
           a6(5,5) =-( clap2/(epsmu2) + cem2*(                         an3*clap2 )*an3 )


           q(0) = u1(i1-is1,i2-is2,i3-is3,ex)
           q(1) = u1(i1-is1,i2-is2,i3-is3,ey)
           q(2) = u1(i1-is1,i2-is2,i3-is3,ez)
           q(3) = u2(j1-js1,j2-js2,j3-js3,ex)
           q(4) = u2(j1-js1,j2-js2,j3-js3,ey)
           q(5) = u2(j1-js1,j2-js2,j3-js3,ez)

           ! subtract off the contributions from the wrong values at the ghost points:
           do n=0,5
             f(n) = (a6(n,0)*q(0)+a6(n,1)*q(1)+a6(n,2)*q(2)+a6(n,3)*q(3)+a6(n,4)*q(4)+a6(n,5)*q(5)) - f(n)
           end do
      ! write(debugFile,'(" --> 3d:order2-c: f(subtract)=",6f8.3)') f(0),f(1),f(2),f(3),f(4),f(5)
           ! solve A Q = F
           ! factor the matrix
           numberOfEquations=6
           call dgeco( a6(0,0), numberOfEquations, numberOfEquations, ipvt(0),rcond,work(0))
           ! solve
      ! write(debugFile,'(" --> 3d:order2-c: rcond=",e10.2)') rcond
           job=0
           call dgesl( a6(0,0), numberOfEquations, numberOfEquations, ipvt(0), f(0), job)
      ! write(debugFile,'(" --> 3d:order2-c: f(solve)=",6f8.3)') f(0),f(1),f(2),f(3),f(4),f(5)
      ! write(debugFile,'(" --> 3d:order2-c:        q=",6f8.3)') q(0),q(1),q(2),q(3),q(4),q(5)

           u1(i1-is1,i2-is2,i3-is3,ex)=f(0)
           u1(i1-is1,i2-is2,i3-is3,ey)=f(1)
           u1(i1-is1,i2-is2,i3-is3,ez)=f(2)
           u2(j1-js1,j2-js2,j3-js3,ex)=f(3)
           u2(j1-js1,j2-js2,j3-js3,ey)=f(4)
           u2(j1-js1,j2-js2,j3-js3,ez)=f(5)

           if( .false. )then
           u1(i1-is1,i2-is2,i3-is3,ex)=q(0)
           u1(i1-is1,i2-is2,i3-is3,ey)=q(1)
           u1(i1-is1,i2-is2,i3-is3,ez)=q(2)
           u2(j1-js1,j2-js2,j3-js3,ex)=q(3)
           u2(j1-js1,j2-js2,j3-js3,ey)=q(4)
           u2(j1-js1,j2-js2,j3-js3,ez)=q(5)
           end if

           if( debug.gt.3 )then ! re-evaluate
            evalInterfaceDerivatives3d()
            eval3dJumpOrder2()
            write(debugFile,'(" --> 3d-order2-c: i1,i2,i3=",3i4," f(re-eval)=",6e10.2)') i1,i2,i3,f(0),f(1),f(2),f(3),f(4),f(5)
              ! '
           end if

         endLoopsMask3d()

         if( orderOfAccuracy.eq.4 )then
         ! -- For now we just extrapolate the 2nd ghost line for 4th order --
         ! note: extrap outside all pts (interp pts)
         beginLoops3d()
            u1(i1-2*is1,i2-2*is2,i3-2*is3,ex)=extrap5(u1,i1-is1,i2-is2,i3-is3,ex,is1,is2,is3)
            u1(i1-2*is1,i2-2*is2,i3-2*is3,ey)=extrap5(u1,i1-is1,i2-is2,i3-is3,ey,is1,is2,is3)
            u1(i1-2*is1,i2-2*is2,i3-2*is3,ez)=extrap5(u1,i1-is1,i2-is2,i3-is3,ez,is1,is2,is3)

            u2(j1-2*js1,j2-2*js2,j3-2*js3,ex)=extrap5(u2,j1-js1,j2-js2,j3-js3,ex,js1,js2,js3)
            u2(j1-2*js1,j2-2*js2,j3-2*js3,ey)=extrap5(u2,j1-js1,j2-js2,j3-js3,ey,js1,js2,js3)
            u2(j1-2*js1,j2-2*js2,j3-2*js3,ez)=extrap5(u2,j1-js1,j2-js2,j3-js3,ez,js1,js2,js3)
         endLoops3d()
         end if
         

         ! periodic update
         periodicUpdate3d(u1,boundaryCondition1,gridIndexRange1,side1,axis1)
         periodicUpdate3d(u2,boundaryCondition2,gridIndexRange2,side2,axis2)


       else if( nd.eq.3 .and. orderOfAccuracy.eq.4 .and. gridType.eq.curvilinear )then

         ! this 3d 4th-order version is in interface3dOrder4.bf:

         call mxInterface3dOrder4( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
                               gridIndexRange1, u1, mask1,rsxy1, xy1, boundaryCondition1, \
                               md1a,md1b,md2a,md2b,md3a,md3b,\
                               gridIndexRange2, u2, mask2,rsxy2, xy2, boundaryCondition2, \
                               ipar, rpar, \
                               aa2,aa4,aa8, ipvt2,ipvt4,ipvt8, \
                               ierr )

       else
         write(debugFile,'("interface3d: ERROR: unknown options nd,order=",2i3)') nd,orderOfAccuracy
         stop 3214
       end if

      return
      end
