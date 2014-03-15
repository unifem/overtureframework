c *******************************************************************************
c   Solid Mechanics boundary conditions
c *******************************************************************************

c These next include files will define the macros that will define the difference approximations
c The actual macro is called below
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

#beginMacro beginGhostLoops3d()
 do i3=nn3a,nn3b
 do i2=nn2a,nn2b
 do i1=nn1a,nn1b
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

c This macro will assign the jump conditions on the boundary
c DIM (input): number of dimensions (2 or 3)
c GRIDTYPE (input) : curvilinear or rectangular
#beginMacro boundaryJumpConditions(DIM,GRIDTYPE)
 #If #DIM eq "2"
  if( eps1.lt.eps2 )then
    epsRatio=eps1/eps2
    beginGhostLoops2d()
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
      ! u2 equals u1 but with normal component = eps1/eps2*(n.u1)
      u2(j1,j2,j3,ex) = ua + (nDotU*epsRatio - nDotU)*an1
      u2(j1,j2,j3,ey) = ub + (nDotU*epsRatio - nDotU)*an2
      u2(j1,j2,j3,hz) = u1(i1,i2,i3,hz)
    endLoops2d()
  else
    epsRatio=eps2/eps1
    beginGhostLoops2d()
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

      u1(i1,i2,i3,ex) = ua + (nDotU*epsRatio - nDotU)*an1
      u1(i1,i2,i3,ey) = ub + (nDotU*epsRatio - nDotU)*an2
      u1(i1,i2,i3,hz) = u2(j1,j2,j3,hz)
    endLoops2d()
  end if
 #Else
   stop 7742
 #End
#endMacro

c ** Precompute the derivatives of rsxy ***
c assign rvx(m) = (rx,sy)
c        rvxx(m) = (rxx,sxx)
#beginMacro computeRxDerivatives(rv,rsxy,i1,i2,i3)
do m=0,nd-1
 rv ## x(m)   =rsxy(i1,i2,i3,m,0)
 rv ## y(m)   =rsxy(i1,i2,i3,m,1)

 rv ## xx(m)  =rsxy ## x22(i1,i2,i3,m,0)
 rv ## xy(m)  =rsxy ## x22(i1,i2,i3,m,1)
 rv ## yy(m)  =rsxy ## y22(i1,i2,i3,m,1)

 rv ## xxx(m) =rsxy ## xx22(i1,i2,i3,m,0)
 rv ## xxy(m) =rsxy ## xx22(i1,i2,i3,m,1)
 rv ## xyy(m) =rsxy ## xy22(i1,i2,i3,m,1)
 rv ## yyy(m) =rsxy ## yy22(i1,i2,i3,m,1)

 rv ## xxxx(m)=rsxy ## xxx22(i1,i2,i3,m,0)
 rv ## xxyy(m)=rsxy ## xyy22(i1,i2,i3,m,0)
 rv ## yyyy(m)=rsxy ## yyy22(i1,i2,i3,m,1)
end do
#endMacro

c assign some temporary variables that are used in the evaluation of the operators
#beginMacro setJacobian(rv,axis1,axisp1)
 rx   =rv ## x(axis1)   
 ry   =rv ## y(axis1)   
                    
 rxx  =rv ## xx(axis1)  
 rxy  =rv ## xy(axis1)  
 ryy  =rv ## yy(axis1)  
                    
 rxxx =rv ## xxx(axis1) 
 rxxy =rv ## xxy(axis1) 
 rxyy =rv ## xyy(axis1) 
 ryyy =rv ## yyy(axis1) 
                    
 rxxxx=rv ## xxxx(axis1)
 rxxyy=rv ## xxyy(axis1)
 ryyyy=rv ## yyyy(axis1)

 sx   =rv ## x(axis1p1)   
 sy   =rv ## y(axis1p1)   
                    
 sxx  =rv ## xx(axis1p1)  
 sxy  =rv ## xy(axis1p1)  
 syy  =rv ## yy(axis1p1)  
                    
 sxxx =rv ## xxx(axis1p1) 
 sxxy =rv ## xxy(axis1p1) 
 sxyy =rv ## xyy(axis1p1) 
 syyy =rv ## yyy(axis1p1) 
                    
 sxxxx=rv ## xxxx(axis1p1)
 sxxyy=rv ## xxyy(axis1p1)
 syyyy=rv ## yyyy(axis1p1)

#endMacro


! update the periodic ghost points
#beginMacro periodicUpdate2d(u,bc,gid,side,axis)

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


#endMacro


c ************************************************************************************************
c  This macro is used for looping over the faces of a grid to assign booundary conditions
c
c extra: extra points to assign
c          Case 1: extra=numberOfGhostPoints -- for assigning extended boundaries
c          Case 2: extra=-1 -- for assigning ghost points but not including extended boundaries
c numberOfGhostPoints : number of ghost points (1 for 2nd order, 2 for fourth-order ...)
c
c
c Output:
c  n1a,n1b,n2a,n2b,n3a,n3b : from gridIndexRange
c  nn1a,nn1b,nn2a,nn2b,nn3a,nn3b : includes "extra" points
c 
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
   extra1a=max(0,extra1a) ! over-ride extra=-1 : assign ends in periodic directions (or internal parallel boundaries)
 else if( boundaryCondition(0,0).eq.0 )then
   extra1a=numberOfGhostPoints  ! include interpolation points since we assign ghost points outside these
 end if
 ! **NOTE** the bc on the right may be negative even it is not on the left (for parallel)
 if( boundaryCondition(1,0).lt.0 )then
   extra1b=max(0,extra1b) ! over-ride extra=-1 : assign ends in periodic directions
 else if( boundaryCondition(1,0).eq.0 )then
   extra1b=numberOfGhostPoints
 end if

 if( boundaryCondition(0,1).lt.0 )then
   extra2a=max(0,extra2a) ! over-ride extra=-1 : assign ends in periodic directions (or internal parallel boundaries)
 else if( boundaryCondition(0,1).eq.0 )then
   extra2a=numberOfGhostPoints  ! include interpolation points since we assign ghost points outside these
 end if
 ! **NOTE** the bc on the right may be negative even it is not on the left (for parallel)
 if( boundaryCondition(1,1).lt.0 )then
   extra2b=max(0,extra2b) ! over-ride extra=-1 : assign ends in periodic directions
 else if( boundaryCondition(1,1).eq.0 )then
   extra2b=numberOfGhostPoints
 end if

 if(  nd.eq.3 )then
  if( boundaryCondition(0,2).lt.0 )then
    extra3a=max(0,extra3a) ! over-ride extra=-1 : assign ends in periodic directions (or internal parallel boundaries)
  else if( boundaryCondition(0,2).eq.0 )then
    extra3a=numberOfGhostPoints  ! include interpolation points since we assign ghost points outside these
  end if
  ! **NOTE** the bc on the right may be negative even it is not on the left (for parallel)
  if( boundaryCondition(1,2).lt.0 )then
    extra3b=max(0,extra3b) ! over-ride extra=-1 : assign ends in periodic directions
  else if( boundaryCondition(1,2).eq.0 )then
    extra3b=numberOfGhostPoints
  end if
 end if

 do axis=0,nd-1
 do side=0,1

   if( boundaryCondition(side,axis).gt.0 )then

     ! write(*,'(" bcOpt: side,axis,bc=",3i2)') side,axis,boundaryCondition(side,axis)

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

!*      ! (js1,js2,js3) used to compute tangential derivatives
!*      js1=0
!*      js2=0
!*      js3=0
!*      if( axisp1.eq.0 )then
!*        js1=1-2*side
!*      else if( axisp1.eq.1 )then
!*        js2=1-2*side
!*      else if( axisp1.eq.2 )then
!*        js3=1-2*side
!*      else
!*        stop 5
!*      end if
!* 
!*      ! (ks1,ks2,ks3) used to compute second tangential derivative
!*      ks1=0
!*      ks2=0
!*      ks3=0
!*      if( axisp2.eq.0 )then
!*        ks1=1-2*side
!*      else if( axisp2.eq.1 )then
!*        ks2=1-2*side
!*      else if( axisp2.eq.2 )then
!*        ks3=1-2*side
!*      else
!*        stop 5
!*      end if

     if( debug.gt.7 )then
       write(*,'(" bcOpt: grid,side,axis=",3i3,", \
         loop bounds: n1a,n1b,n2a,n2b,n3a,n3b=",6i3)') grid,side,axis,\
         n1a,n1b,n2a,n2b,n3a,n3b

     end if
   end if ! if bc>0 

   ! On interfaces we should use the bcf array values even for TZ since then
   ! we get a coupling at the interface: 
   !   bcf = n.sigma(fluid) + [ n.sigma_e(solid) - n.sigma_e(fluid) ]
   if( interfaceType(side,axis,grid).eq.noInterface )then
     assignTwilightZone=twilightZone
   else
     assignTwilightZone=0  ! this will turn off the use of TZ
   end if

#endMacro

#beginMacro endLoopOverSides()
 end do ! end side
 end do ! end axis
#endMacro


#beginMacro getNormal2d(i1,i2,i3,axis)
 an1 = rsxy(i1,i2,i3,axis,0)
 an2 = rsxy(i1,i2,i3,axis,1)
 aNormi = -is/max(epsx,sqrt(an1**2 + an2**2))
 an1=an1*aNormi
 an2=an2*aNormi
#endMacro

#beginMacro getNormal3d(i1,i2,i3,axis)
 an1 = rsxy(i1,i2,i3,axis,0)
 an2 = rsxy(i1,i2,i3,axis,1)
 an3 = rsxy(i1,i2,i3,axis,2)
 aNormi = -is/max(epsx,sqrt(an1**2 + an2**2+ an3**2))
 an1=an1*aNormi
 an2=an2*aNormi
 an3=an3*aNormi
#endMacro

! ==========================================================================
! Apply a stress free BC -- rectangular and 2d
! 
! FORCING equals noForcing or forcing
! ==========================================================================
#beginMacro tractionBCRectangular3dMacro(FORCING)
alpha=lambda/(lambda+2.*mu)
beta=1./(lambda+2.*mu)
if( axis.eq.0 )then
  ! u.x = -alpha*(v.y+w.z)
  ! v.x = -u.y  
  ! w.x = -u.z
 beginLoopsMask3d()
  vy=uy23r(i1,i2,i3,vc)
  wz=uz23r(i1,i2,i3,wc)
  uy=uy23r(i1,i2,i3,uc)
  uz=uz23r(i1,i2,i3,uc)
  #If #FORCING eq "forcing" 
   if( assignTwilightZone.eq.0 )then
    u(i1-is1,i2-is2,i3-is3,uc)=u(i1+is1,i2+is2,i3+is3,uc)+dx(0)*2.*(\
                          is1*alpha*(vy+wz)+ beta*bcf(side,axis,i1,i2,i3,uc) )
    u(i1-is1,i2-is2,i3-is3,vc)=u(i1+is1,i2+is2,i3+is3,vc)+dx(0)*2.*(\
                          is1*uy+         (1./mu)*bcf(side,axis,i1,i2,i3,vc) )
    u(i1-is1,i2-is2,i3-is3,wc)=u(i1+is1,i2+is2,i3+is3,wc)+dx(0)*2.*(\
                          is1*uz+         (1./mu)*bcf(side,axis,i1,i2,i3,wc) )
   else
    OGDERIV3D(0,1,0,0,i1,i2,i3,t,ux0,vx0,wx0)
    OGDERIV3D(0,0,1,0,i1,i2,i3,t,uy0,vy0,wy0)
    OGDERIV3D(0,0,0,1,i1,i2,i3,t,uz0,vz0,wz0)
    u(i1-is1,i2-is2,i3-is3,uc)=u(i1+is1,i2+is2,i3+is3,uc)-is1*dx(0)*2.*(-alpha*(vy+wz)+ux0+alpha*(vy0+wz0))
    u(i1-is1,i2-is2,i3-is3,vc)=u(i1+is1,i2+is2,i3+is3,vc)-is1*dx(0)*2.*(-uy           +vx0+uy0)
    u(i1-is1,i2-is2,i3-is3,wc)=u(i1+is1,i2+is2,i3+is3,wc)-is1*dx(0)*2.*(-uz           +wx0+uz0)
   end if
  #Else
   u(i1-is1,i2-is2,i3-is3,uc)=u(i1+is1,i2+is2,i3+is3,uc)-is1*dx(0)*2.*(-alpha*(vy+wz))
   u(i1-is1,i2-is2,i3-is3,vc)=u(i1+is1,i2+is2,i3+is3,vc)-is1*dx(0)*2.*(-uy)
   u(i1-is1,i2-is2,i3-is3,wc)=u(i1+is1,i2+is2,i3+is3,wc)-is1*dx(0)*2.*(-uz)
  #End

 endLoopsMask3d()

else if( axis.eq.1 )then
! u.y = - v.x
! v.y = -alpha*(u.x+w.z)
! w.y = - v.z
 beginLoopsMask3d()
  vx=ux23r(i1,i2,i3,vc)
  ux=ux23r(i1,i2,i3,uc)
  wz=uz23r(i1,i2,i3,wc)
  vz=uz23r(i1,i2,i3,vc)
  #If #FORCING eq "forcing" 
   if( assignTwilightZone.eq.0 )then
    u(i1-is1,i2-is2,i3-is3,uc)=u(i1+is1,i2+is2,i3+is3,uc)+dx(1)*2.*(\
                          is2*vx +         (1./mu)*bcf(side,axis,i1,i2,i3,uc))
    u(i1-is1,i2-is2,i3-is3,vc)=u(i1+is1,i2+is2,i3+is3,vc)+dx(1)*2.*(\
                          is2*alpha*(ux+wz) + beta*bcf(side,axis,i1,i2,i3,vc))
    u(i1-is1,i2-is2,i3-is3,wc)=u(i1+is1,i2+is2,i3+is3,wc)+dx(1)*2.*(\
                          is2*vz +         (1./mu)*bcf(side,axis,i1,i2,i3,wc) )
   else
    OGDERIV3D(0,1,0,0,i1,i2,i3,t,ux0,vx0,wx0)
    OGDERIV3D(0,0,1,0,i1,i2,i3,t,uy0,vy0,wy0)
    OGDERIV3D(0,0,0,1,i1,i2,i3,t,uz0,vz0,wz0)
    u(i1-is1,i2-is2,i3-is3,uc)=u(i1+is1,i2+is2,i3+is3,uc)-is2*dx(1)*2.*(-vx            +uy0+vx0)
    u(i1-is1,i2-is2,i3-is3,vc)=u(i1+is1,i2+is2,i3+is3,vc)-is2*dx(1)*2.*(-alpha*(ux+wz) +vy0+alpha*(ux0+wz0))
    u(i1-is1,i2-is2,i3-is3,wc)=u(i1+is1,i2+is2,i3+is3,wc)-is2*dx(1)*2.*(-vz            +wy0+vz0)
   end if
  #Else
   u(i1-is1,i2-is2,i3-is3,uc)=u(i1+is1,i2+is2,i3+is3,uc)-is2*dx(1)*2.*(-vx)
   u(i1-is1,i2-is2,i3-is3,vc)=u(i1+is1,i2+is2,i3+is3,vc)-is2*dx(1)*2.*(-alpha*(ux+wz))
   u(i1-is1,i2-is2,i3-is3,wc)=u(i1+is1,i2+is2,i3+is3,wc)-is2*dx(1)*2.*(-vz)
  #End
 endLoopsMask3d()

else 

! u.z = - w.x
! v.z = - w.y
! w.z = -alpha*(u.x+v.y)
 beginLoopsMask3d()
  wx=ux23r(i1,i2,i3,wc)
  wy=uy23r(i1,i2,i3,wc)
  ux=ux23r(i1,i2,i3,uc)
  vy=uy23r(i1,i2,i3,vc)
  #If #FORCING eq "forcing" 
   if( assignTwilightZone.eq.0 )then
    u(i1-is1,i2-is2,i3-is3,uc)=u(i1+is1,i2+is2,i3+is3,uc)+dx(2)*2.*( \
                          is3*wx +         (1./mu)*bcf(side,axis,i1,i2,i3,uc))
    u(i1-is1,i2-is2,i3-is3,vc)=u(i1+is1,i2+is2,i3+is3,vc)+dx(2)*2.*( \
                          is3*wy +         (1./mu)*bcf(side,axis,i1,i2,i3,vc))
    u(i1-is1,i2-is2,i3-is3,wc)=u(i1+is1,i2+is2,i3+is3,wc)+dx(2)*2.*(\
                          is3*alpha*(ux+vy) + beta*bcf(side,axis,i1,i2,i3,wc))
   else
    OGDERIV3D(0,1,0,0,i1,i2,i3,t,ux0,vx0,wx0)
    OGDERIV3D(0,0,1,0,i1,i2,i3,t,uy0,vy0,wy0)
    OGDERIV3D(0,0,0,1,i1,i2,i3,t,uz0,vz0,wz0)
    u(i1-is1,i2-is2,i3-is3,uc)=u(i1+is1,i2+is2,i3+is3,uc)-is3*dx(2)*2.*(-wx            +uz0+wx0)
    u(i1-is1,i2-is2,i3-is3,vc)=u(i1+is1,i2+is2,i3+is3,vc)-is3*dx(2)*2.*(-wy            +vz0+wy0)
    u(i1-is1,i2-is2,i3-is3,wc)=u(i1+is1,i2+is2,i3+is3,wc)-is3*dx(2)*2.*(-alpha*(ux+vy) +wz0+alpha*(ux0+vy0))
   end if
  #Else
   u(i1-is1,i2-is2,i3-is3,uc)=u(i1+is1,i2+is2,i3+is3,uc)-is3*dx(2)*2.*(-wx)
   u(i1-is1,i2-is2,i3-is3,vc)=u(i1+is1,i2+is2,i3+is3,vc)-is3*dx(2)*2.*(-wy)
   u(i1-is1,i2-is2,i3-is3,wc)=u(i1+is1,i2+is2,i3+is3,wc)-is3*dx(2)*2.*(-alpha*(ux+vy))
  #End
 endLoopsMask3d()

end if      
#endMacro


! ==========================================================================
! Apply a stress free BC -- curvilinear and 2d
! 
! FORCING equals noForcing or forcing
! ==========================================================================
#beginMacro tractionBCCurvilinear2dMacro(FORCING)
alpha=lambda+2*mu
beginLoops2d()

 ! Solve n.tauv = 0 
 !    -->   A uv.r + B uv.s = 0

 ! here is the normal (assumed to be the same on both sides)
 ! *wdh* 080523 -- for the real outward normal we should multiple by (-is)   **fix this** 
 an1=rsxy(i1,i2,i3,axis,0)   ! normal (an1,an2)
 an2=rsxy(i1,i2,i3,axis,1)
 aNorm=max(epsx,sqrt(an1**2+an2**2))
 an1=an1/aNorm
 an2=an2/aNorm

 ux=ux22(i1,i2,i3,uc)
 uy=uy22(i1,i2,i3,uc)
 vx=ux22(i1,i2,i3,vc)
 vy=uy22(i1,i2,i3,vc)

 ! components of the stress tensor: 
 tau11 = alpha*ux + lambda*vy
 tau12 = mu*( uy + vx )
 tau21 = tau12
 tau22 = lambda*ux + alpha*vy

 ! here are  the equations we mean to satisfy:  
 f(0) = an1*tau11+an2*tau21
 f(1) = an1*tau12+an2*tau22
 #If #FORCING eq "forcing" 
  if( assignTwilightZone.eq.0 )then
   ! forced case: solve n.tau - f = 0 
   ! *wdh* 080523 multiply bcf by is since (an1,an2) is not the outward normal
   f(0) = f(0) + is*bcf(side,axis,i1,i2,i3,uc)
   f(1) = f(1) + is*bcf(side,axis,i1,i2,i3,vc)
  else
   OGDERIV2D(0,1,0,0,i1,i2,i3,t,ux0,vx0)
   OGDERIV2D(0,0,1,0,i1,i2,i3,t,uy0,vy0)
   f(0) = f(0) - ( an1*(alpha*ux0 + lambda*vy0) + an2*(mu*(uy0+vx0) ) )
   f(1) = f(1) - ( an1*(mu*(uy0+vx0) )          + an2*(lambda*ux0 + alpha*vy0) )
  end if
 #End

 !  [ a2(0,0) a2(0,1) ][ u(-1) ] =  RHS
 !  [ a2(1,0) a2(1,1) ][ v(-1) ]   
 a2(0,0)=-is*( an1*alpha *rsxy(i1,i2,i3,axis,0)+an2*mu*rsxy(i1,i2,i3,axis,1) )/(2.*dr(axis))
 a2(0,1)=-is*( an1*lambda*rsxy(i1,i2,i3,axis,1)+an2*mu*rsxy(i1,i2,i3,axis,0) )/(2.*dr(axis))

 a2(1,0)=-is*( an1*mu*rsxy(i1,i2,i3,axis,1)+an2*lambda*rsxy(i1,i2,i3,axis,0) )/(2.*dr(axis))
 a2(1,1)=-is*( an1*mu*rsxy(i1,i2,i3,axis,0)+an2*alpha *rsxy(i1,i2,i3,axis,1) )/(2.*dr(axis))

 ! here are the wrong ghostpoint values
 q(0) = u(i1-is1,i2-is2,i3,uc)
 q(1) = u(i1-is1,i2-is2,i3,vc)

 ! subtract off the contributions from the wrong values at the ghost points:
 do n=0,1
   f(n) = (a2(n,0)*q(0)+a2(n,1)*q(1)) - f(n)
 end do

 call dgeco( a2(0,0), 2, 2, ipvt(0),rcond,work(0))
 call dgesl( a2(0,0), 2, 2, ipvt(0), f(0), job)

 u(i1-is1,i2-is2,i3,uc)=f(0)
 u(i1-is1,i2-is2,i3,vc)=f(1)

 if( debug.gt.0 )then ! re-evaluate

   ux=ux22(i1,i2,i3,uc)
   uy=uy22(i1,i2,i3,uc)
   vx=ux22(i1,i2,i3,vc)
   vy=uy22(i1,i2,i3,vc)

   tau11 = alpha*ux + lambda*vy
   tau12 = mu*( uy + vx )
   tau21 = tau12
   tau22 = lambda*ux + alpha*vy

   f(0) = an1*tau11+an2*tau21
   f(1) = an1*tau12+an2*tau22
   #If #FORCING eq "forcing" 
    if( assignTwilightZone.eq.0 )then
     ! *wdh* 080523 multiply bcf by is since (an1,an2) is not the outward normal
     f(0) = f(0) + is*bcf(side,axis,i1,i2,i3,uc)
     f(1) = f(1) + is*bcf(side,axis,i1,i2,i3,vc)
    else
     OGDERIV2D(0,1,0,0,i1,i2,i3,t,ux0,vx0)
     OGDERIV2D(0,0,1,0,i1,i2,i3,t,uy0,vy0)
     f(0) = f(0) - ( an1*(alpha*ux0 + lambda*vy0) + an2*(mu*(uy0+vx0) ) )
     f(1) = f(1) - ( an1*(mu*(uy0+vx0) )          + an2*(lambda*ux0 + alpha*vy0) )
    end if
   #End

   ! write(*,'(" --> order2-curv: i1,i2=",2i4," n.tau=",4e10.2)') i1,i2,f(0),f(1)
     ! '
 end if

endLoops2d()

#endMacro

! ==========================================================================
! Apply a stress free BC -- curvilinear and 3d
! 
! FORCING equals noForcing or forcing
! ==========================================================================
#beginMacro tractionBCCurvilinear3dMacro(FORCING)
alpha=lambda+2*mu

beginLoopsMask3d()

 ! Solve n.tauv = 0 
 !    -->   A uv.r + B uv.s = 0

 ! here is the normal (assumed to be the same on both sides)
 an1=rsxy(i1,i2,i3,axis,0)   ! normal (an1,an2,an3)
 an2=rsxy(i1,i2,i3,axis,1)
 an3=rsxy(i1,i2,i3,axis,2)
 aNorm=max(epsx,sqrt(an1**2+an2**2+an3**2))
 an1=an1/aNorm
 an2=an2/aNorm
 an3=an3/aNorm

 ux=ux23(i1,i2,i3,uc)
 uy=uy23(i1,i2,i3,uc)
 uz=uz23(i1,i2,i3,uc)
 vx=ux23(i1,i2,i3,vc)
 vy=uy23(i1,i2,i3,vc)
 vz=uz23(i1,i2,i3,vc)
 wx=ux23(i1,i2,i3,wc)
 wy=uy23(i1,i2,i3,wc)
 wz=uz23(i1,i2,i3,wc)

 tau11 = alpha*ux + lambda*(vy+wz)
 tau12 = mu*( uy + vx )
 tau13 = mu*( uz + wx )
 tau21 = tau12
 tau22 = lambda*(ux+wz) + alpha*vy
 tau23 = mu*( vz + wy )
 tau31 = tau13
 tau32 = tau23
 tau33 = lambda*(ux+vy) + alpha*wz

 ! here are  the equations we mean to satisfy:  
 f(0) = an1*tau11+an2*tau21+an3*tau31
 f(1) = an1*tau12+an2*tau22+an3*tau32
 f(2) = an1*tau13+an2*tau23+an3*tau33

 #If #FORCING eq "forcing" 

  if( assignTwilightZone.eq.0 )then
   ! forced case: solve n.tau - f = 0 
   ! *wdh* 080523 multiply bcf by is since (an1,an2) is not the outward normal
   f(0) = f(0) + is*bcf(side,axis,i1,i2,i3,uc)
   f(1) = f(1) + is*bcf(side,axis,i1,i2,i3,vc)
   f(2) = f(2) + is*bcf(side,axis,i1,i2,i3,wc)
  else

   OGDERIV3D(0,1,0,0,i1,i2,i3,t,ux0,vx0,wx0)
   OGDERIV3D(0,0,1,0,i1,i2,i3,t,uy0,vy0,wy0)
   OGDERIV3D(0,0,0,1,i1,i2,i3,t,uz0,vz0,wz0) 

   f(0) = f(0) - ( an1*(alpha*ux0+lambda*(vy0+wz0))+an2*(mu*(uy0+vx0))+an3*(mu*(uz0+wx0)) )
   f(1) = f(1) - ( an1*(mu*(uy0+vx0))+an2*(lambda*(ux0+wz0)+alpha*vy0)+an3*(mu*(vz0+wy0)) )
   f(2) = f(2) - ( an1*(mu*(uz0+wx0))+an2*(mu*(vz0+wy0))+an3*(lambda*(ux0+vy0)+alpha*wz0) )
  end if
 #End

 !  [ a3(0,0) a3(0,1) a3(0,2) ][ u(-1) ] =  RHS
 !  [ a3(1,0) a3(1,1) a3(1,2) ][ v(-1) ]   
 !  [ a3(2,0) a3(2,1) a3(2,2) ][ w(-1) ]   
 a3(0,0)=-is*( an1*alpha* rsxy(i1,i2,i3,axis,0)+\
               an2*mu*    rsxy(i1,i2,i3,axis,1)+\
               an3*mu*    rsxy(i1,i2,i3,axis,2) )/(2.*dr(axis))
 a3(0,1)=-is*( an1*lambda*rsxy(i1,i2,i3,axis,1)+\
               an2*mu*    rsxy(i1,i2,i3,axis,0) )/(2.*dr(axis))
 a3(0,2)=-is*( an1*lambda*rsxy(i1,i2,i3,axis,2)+\
               an3*mu*    rsxy(i1,i2,i3,axis,0) )/(2.*dr(axis))

 a3(1,0)=-is*( an1*mu*    rsxy(i1,i2,i3,axis,1)+\
               an2*lambda*rsxy(i1,i2,i3,axis,0) )/(2.*dr(axis))
 a3(1,1)=-is*( an1*mu*    rsxy(i1,i2,i3,axis,0)+\
               an2*alpha* rsxy(i1,i2,i3,axis,1)+\
               an3*mu*    rsxy(i1,i2,i3,axis,2) )/(2.*dr(axis))
 a3(1,2)=-is*( an2*lambda*rsxy(i1,i2,i3,axis,2)+\
               an3*mu*    rsxy(i1,i2,i3,axis,1) )/(2.*dr(axis))

 a3(2,0)=-is*( an1*mu*    rsxy(i1,i2,i3,axis,2)+\
               an3*lambda*rsxy(i1,i2,i3,axis,0) )/(2.*dr(axis))
 a3(2,1)=-is*( an2*mu*    rsxy(i1,i2,i3,axis,2)+\
               an3*lambda*rsxy(i1,i2,i3,axis,1) )/(2.*dr(axis))
 a3(2,2)=-is*( an1*mu*    rsxy(i1,i2,i3,axis,0)+\
               an2*mu*    rsxy(i1,i2,i3,axis,1)+\
               an3*alpha* rsxy(i1,i2,i3,axis,2) )/(2.*dr(axis))

 ! here are the wrong ghostpoint values
 q(0) = u(i1-is1,i2-is2,i3-is3,uc)
 q(1) = u(i1-is1,i2-is2,i3-is3,vc)
 q(2) = u(i1-is1,i2-is2,i3-is3,wc)

 ! subtract off the contributions from the wrong values at the ghost points:
 do n=0,2
   f(n) = (a3(n,0)*q(0)+a3(n,1)*q(1)+a3(n,2)*q(2)) - f(n)
 end do

 call dgeco( a3(0,0), 3, 3, ipvt(0),rcond,work(0))
 call dgesl( a3(0,0), 3, 3, ipvt(0), f(0), job)

 u(i1-is1,i2-is2,i3-is3,uc)=f(0)
 u(i1-is1,i2-is2,i3-is3,vc)=f(1)
 u(i1-is1,i2-is2,i3-is3,wc)=f(2)

 if( debug.gt.0 )then ! re-evaluate

   ux=ux23(i1,i2,i3,uc)
   uy=uy23(i1,i2,i3,uc)
   uz=uz23(i1,i2,i3,uc)
   vx=ux23(i1,i2,i3,vc)
   vy=uy23(i1,i2,i3,vc)
   vz=uz23(i1,i2,i3,vc)
   wx=ux23(i1,i2,i3,wc)
   wy=uy23(i1,i2,i3,wc)
   wz=uz23(i1,i2,i3,wc)

   tau11 = alpha*ux + lambda*(vy+wz)
   tau12 = mu*( uy + vx )
   tau13 = mu*( uz + wx )
   tau21 = tau12
   tau22 = lambda*(ux+wz) + alpha*vy
   tau23 = mu*( vz + wy )
   tau31 = tau13
   tau32 = tau23
   tau33 = lambda*(ux+vy) + alpha*wz

   ! here are  the equations we mean to satisfy:  
   f(0) = an1*tau11+an2*tau21+an3*tau31
   f(1) = an1*tau12+an2*tau22+an3*tau32
   f(2) = an1*tau13+an2*tau23+an3*tau33
   #If #FORCING eq "forcing" 
  
    if( assignTwilightZone.eq.0 )then
     ! forced case: solve n.tau - f = 0 
     ! *wdh* 080523 multiply bcf by is since (an1,an2) is not the outward normal
     f(0) = f(0) + is*bcf(side,axis,i1,i2,i3,uc)
     f(1) = f(1) + is*bcf(side,axis,i1,i2,i3,vc)
     f(2) = f(2) + is*bcf(side,axis,i1,i2,i3,wc)
    else
     OGDERIV3D(0,1,0,0,i1,i2,i3,t,ux0,vx0,wx0)
     OGDERIV3D(0,0,1,0,i1,i2,i3,t,uy0,vy0,wy0)
     OGDERIV3D(0,0,0,1,i1,i2,i3,t,uz0,vz0,wz0)
  
     f(0) = f(0) - ( an1*(alpha*ux0+lambda*(vy0+wz0))+an2*(mu*(uy0+vx0))+an3*(mu*(uz0+wx0)) )
     f(1) = f(1) - ( an1*(mu*(uy0+vx0))+an2*(lambda*(ux0+wz0)+alpha*vy0)+an3*(mu*(vz0+wy0)) )
     f(2) = f(2) - ( an1*(mu*(uz0+wx0))+an2*(mu*(vz0+wy0))+an3*(lambda*(ux0+vy0)+alpha*wz0) )
    end if 
   #End

   write(*,'(" --> bc: (",i1,",",i1,") i1,i2,i3=",3i4," n.tau=",4e10.2)') side,axis,i1,i2,i3,f(0),f(1),f(2)
     ! '
 end if

endLoopsMask3d()

#endMacro


! ==========================================================================
! Apply a displacementBC BC -- 2d
! 
! FORCING equals noForcing or forcing
! ORDER : 2 or 4
! ==========================================================================
#beginMacro displacementBC2dMacro(FORCING,ORDER)
 ! *********** NOT currently used ***************
 beginGhostLoops3d()
   #If #FORCING ne "forcing"
     u(i1,i2,i3,uc)=0.
     u(i1,i2,i3,vc)=0.
   #Else
     OGF2D(i1,i2,i3,t,u0,v0)
     u(i1,i2,i3,uc)=u0
     u(i1,i2,i3,vc)=v0
   #End
   #If #ORDER eq "2" 
     u(i1-is1,i2-is2,i3,uc)=extrap3(u,i1,i2,i3,uc,is1,is2,is3)
     u(i1-is1,i2-is2,i3,vc)=extrap3(u,i1,i2,i3,vc,is1,is2,is3)
   #Else
     u(i1-is1,i2-is2,i3,uc)=extrap5(u,i1,i2,i3,uc,is1,is2,is3)
     u(i1-is1,i2-is2,i3,vc)=extrap5(u,i1,i2,i3,vc,is1,is2,is3)
   #End
 endLoops3d()
#endMacro


! ==========================================================================
! Apply a displacementBC BC -- 3d
! 
! FORCING equals noForcing or forcing
! ==========================================================================
#beginMacro displacementBC3dMacro(FORCING,ORDER)
 ! *********** NOT currently used ***************
 beginGhostLoops3d()
   #If #FORCING ne "forcing"
     u(i1,i2,i3,uc)=0.
     u(i1,i2,i3,vc)=0.
     u(i1,i2,i3,wc)=0.
   #Else
     OGF3D(i1,i2,i3,t,u0,v0,w0)
     u(i1,i2,i3,uc)=u0
     u(i1,i2,i3,vc)=v0
     u(i1,i2,i3,wc)=w0
   #End
   #If #ORDER eq "2"
     u(i1-is1,i2-is2,i3-is3,uc)=extrap3(u,i1,i2,i3,uc,is1,is2,is3)
     u(i1-is1,i2-is2,i3-is3,vc)=extrap3(u,i1,i2,i3,vc,is1,is2,is3)
     u(i1-is1,i2-is2,i3-is3,wc)=extrap3(u,i1,i2,i3,wc,is1,is2,is3)
   #Else
     u(i1-is1,i2-is2,i3-is3,uc)=extrap5(u,i1,i2,i3,uc,is1,is2,is3)
     u(i1-is1,i2-is2,i3-is3,vc)=extrap5(u,i1,i2,i3,vc,is1,is2,is3)
     u(i1-is1,i2-is2,i3-is3,wc)=extrap5(u,i1,i2,i3,wc,is1,is2,is3)
   #End
 endLoops3d()
#endMacro

! ====================================================================================
! Evaluate the slip-wall equations
! ====================================================================================
#beginMacro evalSlipWallEquations2d(FORCING)
 ux=ux22(i1,i2,i3,uc)
 uy=uy22(i1,i2,i3,uc)
 vx=ux22(i1,i2,i3,vc)
 vy=uy22(i1,i2,i3,vc)

 ! components of the stress tensor: 
 tau11 = alpha*ux + lambda*vy
 tau12 = mu*( uy + vx )
 tau21 = tau12
 tau22 = lambda*ux + alpha*vy

 ! f(m) holds the current residuals in the equations we mean to satisfy:  
 ! f(0) = an1*u(i1-is1,i2-is2,i3,uc)+an2*u(i1-is1,i2-is2,i3,vc) - ( an1*um + an2*vm )
 f(0) = 0.
 f(1) = t1*(an1*tau11+an2*tau21) + t2*(an1*tau12+an2*tau22)

 #If #FORCING eq "forcing" 
  if( assignTwilightZone.eq.0 )then
   ! forced case: what should this be ??  do nothing for now 
   !f(0) = f(0) + is*bcf(side,axis,i1,i2,i3,uc)
   !f(1) = f(1) + is*bcf(side,axis,i1,i2,i3,vc)
  else
   ! We could do the following: 
   !OGF2D(i1-is1,i2-is2,i3,t,um,vm)
   !f(0) = an1*u(i1-is1,i2-is2,i3,uc)+an2*u(i1-is1,i2-is2,i3,vc) - ( an1*um + an2*vm )
   OGDERIV2D(0,1,0,0,i1,i2,i3,t,ux0,vx0)
   OGDERIV2D(0,0,1,0,i1,i2,i3,t,uy0,vy0)
   f(1) = f(1) - t1*( an1*(alpha*ux0 + lambda*vy0) + an2*(mu*(uy0+vx0) ) )\
               - t2*( an1*(mu*(uy0+vx0) )          + an2*(lambda*ux0 + alpha*vy0) )
  end if
 #End
#endMacro

! ==========================================================================
! Apply a slip-wall BC -- curvilinear and 2d
! 
! FORCING equals noForcing or forcing
! ==========================================================================
#beginMacro slipWallBCCurvilinear2dMacro(FORCING)

alpha=lambda+2*mu

beginLoops2d()

!  if( debug.gt.2 )then ! re-evaluate
! 
!    OGF2D(i1-is1,i2-is2,i3,t,u0,v0)
!    write(*,'(" --> slipWall:curv:START i1,i2=",2i4," ghost: (u,v)=",2e10.2," (ue,ve)=",2e10.2)') i1,i2,u(i1-is1,i2-is2,i3,uc),u(i1-is1,i2-is2,i3,vc),u0,v0
!    ! ' 
!  end if

 ! Solve 
 !       n.u(-1) = given (by extrapolation)
 !       n.tauv.t = 0 
 !    -->   A uv.r + B uv.s = 0

 ! here is the normal 
 getNormal2d(i1,i2,i3,axis)

 ! tangent: 
 t1 =-an2
 t2 = an1 

 evalSlipWallEquations2d(FORCING)

 !  [ a2(0,0) a2(0,1) ][ u(-1) ] =  RHS
 !  [ a2(1,0) a2(1,1) ][ v(-1) ]   
 !a2(0,0)=-is*( an1*alpha *rsxy(i1,i2,i3,axis,0)+an2*mu*rsxy(i1,i2,i3,axis,1) )/(2.*dr(axis))
 !a2(0,1)=-is*( an1*lambda*rsxy(i1,i2,i3,axis,1)+an2*mu*rsxy(i1,i2,i3,axis,0) )/(2.*dr(axis))
 !
 !a2(1,0)=-is*( an1*mu*rsxy(i1,i2,i3,axis,1)+an2*lambda*rsxy(i1,i2,i3,axis,0) )/(2.*dr(axis))
 !a2(1,1)=-is*( an1*mu*rsxy(i1,i2,i3,axis,0)+an2*alpha *rsxy(i1,i2,i3,axis,1) )/(2.*dr(axis))

 a2(0,0)=an1
 a2(0,1)=an2

 a2(1,0)=-is*( t1*( an1*alpha *rsxy(i1,i2,i3,axis,0)+an2*mu*rsxy(i1,i2,i3,axis,1) )\
              +t2*( an1*mu*rsxy(i1,i2,i3,axis,1)+an2*lambda*rsxy(i1,i2,i3,axis,0) ) )/(2.*dr(axis))
 a2(1,1)=-is*( t1*( an1*lambda*rsxy(i1,i2,i3,axis,1)+an2*mu*rsxy(i1,i2,i3,axis,0) )\
              +t2*( an1*mu*rsxy(i1,i2,i3,axis,0)+an2*alpha *rsxy(i1,i2,i3,axis,1) ) )/(2.*dr(axis))

 ! here are the wrong ghostpoint values
 q(0) = u(i1-is1,i2-is2,i3,uc)
 q(1) = u(i1-is1,i2-is2,i3,vc)

 ! subtract off the contributions from the wrong values at the ghost points:
 do n=0,1
   f(n) = (a2(n,0)*q(0)+a2(n,1)*q(1)) - f(n)
 end do

 ! write(*,'(" --> slipWall:curv:Before solve q=",2e10.2," f=",2e10.2)') q(0),q(1),f(0),f(1)
 ! write(*,'(" --> slipWall:curv:Before solve a2=",4e10.2)') a2(0,0),a2(0,1),a2(1,0),a2(1,1)

 call dgeco( a2(0,0), 2, 2, ipvt(0),rcond,work(0))
 call dgesl( a2(0,0), 2, 2, ipvt(0), f(0), job)

 u(i1-is1,i2-is2,i3,uc)=f(0)
 u(i1-is1,i2-is2,i3,vc)=f(1)

 if( debug.gt.3 )then ! re-evaluate

!    OGF2D(i1-is1,i2-is2,i3,t,u0,v0)
!    write(*,'(" --> slipWall:curv: i1,i2=",2i4," (u,v)=",2e10.2," (ue,ve)=",2e10.2)') i1,i2,u(i1-is1,i2-is2,i3,uc),u(i1-is1,i2-is2,i3,vc),u0,v0
!    u(i1-is1,i2-is2,i3,uc)=u0
!    u(i1-is1,i2-is2,i3,vc)=v0


   evalSlipWallEquations2d(FORCING)

   write(*,'(" --> slipWall:curv: i1,i2=",2i4," f=",2e10.2," an1,an2=",2f6.2," is,is1,is2=",3i2)') i1,i2,f(0),f(1),an1,an2,is,is1,is2
   !'

   ! write(*,'(" --> slipWall:curv: i1,i2,i3=",3i4," dr=",2e10.2)') i1,i2,i3,dr(0),dr(1)
   ! write(*,'(" --> slipWall:curv: i1,i2,i3=",3i4," assignTwilightZone,addBoundaryForcing=",2i3,", rcond=",e10.2)') i1,i2,i3,assignTwilightZone,addBoundaryForcing(side,axis),rcond
   ! write(*,'(" --> slipWall:curv: i1,i2=",2i4," rsxy=",4e10.2)') i1,i2,rsxy(i1,i2,i3,0,0),rsxy(i1,i2,i3,1,0),rsxy(i1,i2,i3,0,1),rsxy(i1,i2,i3,1,1)
     ! '
 end if

endLoops2d()

#endMacro



c =================================================================================
c   Assign values in the corners in 2D (see bcMaxwellCorners.bf)
c
c  Set the normal component of the solution on the extended boundaries (points N in figure)
c  Set the corner points "C" 
c              |
c              X
c              |
c        N--N--X--X----
c              |
c        C  C  N
c              |
c        C  C  N
c
c ORDER: 2 or 4
c GRIDTYPE: rectangular, curvilinear
c FORCING: none, twilightZone
c =================================================================================
#beginMacro assignCorners2d(ORDER,GRIDTYPE,FORCING)

  axis=0
  axisp1=1

  i3=gridIndexRange(0,2)
  numberOfGhostPoints=orderOfAccuracy/2


  do side1=0,1
  do side2=0,1
  if( boundaryCondition(side1,0).eq.tractionBC .and.\
      boundaryCondition(side2,1).eq.tractionBC )then

    i1=gridIndexRange(side1,0) ! (i1,i2,i3)=corner point
    i2=gridIndexRange(side2,1)

    ! write(*,'("bcOpt: assign corner side1,side2,i1,i2,i3=",2i2,3i5)') side1,side2,i1,i2,i3

    is1=1-2*side1
    is2=1-2*side2

!   dra=dr(0)*is1
!   dsa=dr(1)*is2

    ! First assign normal component of the displacement:
    ! u.x=u.xxx=0 --> u is even in x
    ! v.y=v.yyy=0 --> v is even in y
    do m=1,numberOfGhostPoints

      js1=is1*m  ! shift to ghost point "m"
      js2=is2*m

      #If #GRIDTYPE == "rectangular"

        u(i1-js1,i2,i3,uc)=u(i1+js1,i2,i3,uc)
        u(i1,i2-js2,i3,vc)=u(i1,i2+js2,i3,vc)

        #If #FORCING == "twilightZone"
          OGF2D(i1-js1,i2,i3,t,um,vm)
          OGF2D(i1+js1,i2,i3,t,up,vp)
          u(i1-js1,i2,i3,uc)=u(i1-js1,i2,i3,uc) + um-up

          OGF2D(i1,i2-js2,i3,t,um,vm)
          OGF2D(i1,i2+js2,i3,t,up,vp)
          u(i1,i2-js2,i3,vc)=u(i1,i2-js2,i3,vc) + vm-vp

        #Elif #FORCING == "none"
        #Else
          stop 6767
        #End

      #Elif #GRIDTYPE == "curvlinear"
        stop 1116
      #Else
        stop 1117
      #End
    end do 

    ! Now assign the tangential components of the displacement 
    alpha=lambda/(lambda+2.*mu)  
    #If #ORDER eq "2" 
      js1=is1
      js2=is2
      #If #GRIDTYPE == "rectangular"
        ! u.yy = alpha*u.xx
        ! v.xx = alpha*v.yy

        u(i1,i2-js2,i3,uc)=2.*u(i1,i2,i3,uc)-u(i1,i2+js2,i3,uc) +dx(1)**2*alpha*uxx22r(i1,i2,i3,uc)
        u(i1-js1,i2,i3,vc)=2.*u(i1,i2,i3,vc)-u(i1+js1,i2,i3,vc) +dx(0)**2*alpha*uyy22r(i1,i2,i3,vc)
      
        #If #FORCING == "twilightZone"

          OGDERIV2D(0,2,0,0,i1,i2,i3,t,uxx0,vxx0)
          OGDERIV2D(0,0,2,0,i1,i2,i3,t,uyy0,vyy0)

          u(i1,i2-js2,i3,uc)=u(i1,i2-js2,i3,uc) +dx(1)**2*( -alpha*uxx0+ uyy0)
          u(i1-js1,i2,i3,vc)=u(i1-js1,i2,i3,vc) +dx(0)**2*( -alpha*vyy0+ vxx0)

          if( debug.gt.0 )then
            OGF2D(i1-js1,i2,i3,t,um,vm)
            OGF2D(i1,i2-js2,i3,t,up,vp)
            write(*,'(" bcOpt:corner: i1,i2=",2i4," uerr,verr=",4e10.2)') i1,i2,\
                  u(i1-js1,i2,i3,uc)-um,u(i1-js1,i2,i3,vc)-vm,\
                  u(i1,i2-js2,i3,uc)-up,u(i1,i2-js2,i3,vc)-vp
            ! '
          end if

        #Elif #FORCING == "none"
        #Else
          stop 6767
        #End
          
      #Elif #GRIDTYPE == "curvlinear"
        stop 1116
      #Else
        stop 1117
      #End

    #Elif #ORDER eq "4" 
      stop 2221
    #Else
    #End


    ! Now do corner (C) points
    ! Taylor series: 
    !   u(-x,-y) = u(x,y) - 2*x*u.x(0,0) - 2*y*u.y(0,0) + O( h^3 )
  ! ** u(i1-is1,i2-is2,i3,uc)=u(i1+is1,i2+is2,i3,uc) -2.*is1*dx(0)*ux22r(i1,i2,i3,uc) -2.*is2*dx(1)*uy22r(i1,i2,i3,uc)
  ! ** u(i1-is1,i2-is2,i3,vc)=u(i1+is1,i2+is2,i3,vc) -2.*is1*dx(0)*ux22r(i1,i2,i3,vc) -2.*is2*dx(1)*uy22r(i1,i2,i3,vc)

    ! This version uses u.xy = - v.xx, v.xy = - u.yy
    u(i1-is1,i2-is2,i3,uc)=2.*u(i1,i2,i3,uc) - u(i1+is1,i2+is2,i3,uc) \
                     +dx(0)**2*uxx22r(i1,i2,i3,uc) - 2.*dx(0)*dx(1)*uxx22r(i1,i2,i3,vc) +dx(1)**2*uyy22r(i1,i2,i3,uc)
    u(i1-is1,i2-is2,i3,vc)=2.*u(i1,i2,i3,vc) - u(i1+is1,i2+is2,i3,vc) \
                     +dx(0)**2*uxx22r(i1,i2,i3,vc) - 2.*dx(0)*dx(1)*uyy22r(i1,i2,i3,uc) +dx(1)**2*uyy22r(i1,i2,i3,vc)

  else if( (boundaryCondition(side1,0).eq.tractionBC .and. boundaryCondition(side2,1).eq.displacementBC) .or.\
           (boundaryCondition(side1,0).eq.displacementBC  .and. boundaryCondition(side2,1).eq.tractionBC) )then 

    ! displacementBC next to stress free
    stop 2311

  else if( boundaryCondition(side1,0).eq.displacementBC .and. boundaryCondition(side2,1).eq.displacementBC )then

    ! displacementBC next to displacementBC
    ! do we need to do anything in this case ? *wdh* 071012
    ! stop 2312

  else if( boundaryCondition(side1,0).gt.0 .and. boundaryCondition(side2,1).gt.0 )then

    ! unknown 
    stop 2313

  end if
  end do
  end do

#endMacro


#beginMacro assignCorners3d(ORDER,GRIDTYPE,FORCING)
  numberOfGhostPoints=orderOfAccuracy/2


  ! Assign the edges
  assignEdges3d(ORDER,GRIDTYPE,FORCING)



  ! Finally assign points outside the vertices of the unit cube
  g1=0.
  g2=0.
  g3=0.

  do side3=0,1
  do side2=0,1
  do side1=0,1

   ! assign ghost values outside the corner (vertex)
   i1=gridIndexRange(side1,0)
   i2=gridIndexRange(side2,1)
   i3=gridIndexRange(side3,2)
   is1=1-2*side1
   is2=1-2*side2
   is3=1-2*side3

   if( boundaryCondition(side1,0).eq.perfectElectricalConductor .and.\
       boundaryCondition(side2,1).eq.perfectElectricalConductor .and.\
       boundaryCondition(side3,2).eq.perfectElectricalConductor )then

   end if

  end do
  end do
  end do
#endMacro



c$$$
c$$$
c$$$#beginMacro getBoundaryForcing()
c$$$ if( side.eq.0 .and.axis.eq.0 )then
c$$$   beginGhostLoops3d()
c$$$    f(i1,i2,i3,uc)=bcf00(i1,i2,i3,uc)
c$$$#endMacro 


      subroutine bcOptSM( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
                          gridIndexRange, u, mask,rsxy, xy, ndMatProp,matIndex,matValpc,matVal, boundaryCondition, \
                          addBoundaryForcing, interfaceType, dim, bcf00,bcf10,bcf01,bcf11,bcf02,bcf12,\
                          bcf0,bcOffset,ipar, rpar, ierr )
c ===================================================================================
c  Boundary conditions for solid mechanics
c
c  gridType : 0=rectangular, 1=curvilinear
c
c  c2= mu/rho, c1=(mu+lambda)/rho;
c 
c The forcing for the boundary conditions can be accessed in two ways. One can either 
c use the arrays: 
c       bcf00(i1,i2,i3,m), bcf10(i1,i2,i3,m), bcf01(i1,i2,i3,m), bcf11(i1,i2,i3,m), 
c       bcf02(i1,i2,i3,m), bcf12(i1,i2,i3,m)
c which provide values for the 6 different faces in 6 different arrays. One can also
c access the same values using the single statement function
c         bcf(side,axis,i1,i2,i3,m)
c which is defined below. 
c ===================================================================================

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

      ! work space arrays that must be saved from call to call:
c**      real aa2(0:1,0:1,0:1,0:*),aa4(0:3,0:3,0:1,0:*),aa8(0:7,0:7,0:1,0:*)
c**      integer ipvt2(0:1,0:*), ipvt4(0:3,0:*), ipvt8(0:7,0:*)

c     --- local variables ----
      
      integer side,axis,grid,gridType,orderOfAccuracy,orderOfExtrapolation,twilightZone,assignTwilightZone,\
        uc,vc,wc,useWhereMask,debug,nn,n1,n2
      real dx(0:2),dr(0:2)
      real t,ep,dt,c1,c2,rho,mu,lambda,alpha,beta
      integer axisp1,axisp2,i1,i2,i3,is1,is2,is3,j1,j2,j3,js1,js2,js3,ks1,ks2,ks3,is,js,it,nit
      integer option,initialized

      integer numGhost,numberOfGhostPoints
      integer side1,side2
      integer n1a,n1b,n2a,n2b,n3a,n3b
      integer nn1a,nn1b,nn2a,nn2b,nn3a,nn3b
      integer extra1a,extra1b,extra2a,extra2b,extra3a,extra3b

c**      real rx1,ry1,rx2,ry2

c**      real aLap0,aLap1,bLap0,bLap1,aLapX0,aLapX1,bLapY0,bLapY1,cLapX0,cLapX1,dLapY0,dLapY1,aLapSq0,aLapSq1,bLapSq0,bLapSq1
      real a11,a12,a21,a22,det,b0,b1,b2

      real a0,a1,cc0,cc1,d0,d1,dr0,ds0
      real aNormSq,divu,uAve

      real epsRatio,an1,an2,an3,aNorm,aNormi,ua,ub,nDotU,t1,t2,t3
      real epsx,tmp

      real tau11,tau12,tau13,tau21,tau22,tau23,tau31,tau32,tau33
      real ux,uy,uz,vx,vy,vz,wx,wy,wz
      real ux0,uy0,uz0,vx0,vy0,vz0,wx0,wy0,wz0
      real uxx0,uxy0,uxz0,uyy0,uyz0,uzz0
      real vxx0,vxy0,vxz0,vyy0,vyz0,vzz0
      real wxx0,wxy0,wxz0,wyy0,wyz0,wzz0
      real u0,v0,w0, u1,v1,w1
      real um,up,vm,vp,wm,wp

      real tau1,tau2,tau3,clap1,clap2,ulap1,vlap1,wlap1,ulap2,vlap2,wlap2,an1Cartesian,an2Cartesian
c**   real ulapSq1,vlapSq1,ulapSq2,vlapSq2,wlapSq1,wlapSq2

c**      integer np1a,np1b,np2a,np2b,np3a,np3b,diff(0:2)

c**      real rx,ry,rxx,rxy,ryy,rxxx,rxxy,rxyy,ryyy,rxxxx,rxxyy,ryyyy
c**      real sx,sy,sxx,sxy,syy,sxxx,sxxy,sxyy,syyy,sxxxx,sxxyy,syyyy

c**      real rv1x(0:2),rv1y(0:2),rv1xx(0:2),rv1xy(0:2),rv1yy(0:2),rv1xxx(0:2),rv1xxy(0:2),rv1xyy(0:2),rv1yyy(0:2),\
c**           rv1xxxx(0:2),rv1xxyy(0:2),rv1yyyy(0:2)
c**      real sv1x(0:2),sv1y(0:2),sv1xx(0:2),sv1xy(0:2),sv1yy(0:2),sv1xxx(0:2),sv1xxy(0:2),sv1xyy(0:2),sv1yyy(0:2),\
c**           sv1xxxx(0:2),sv1xxyy(0:2),sv1yyyy(0:2)
c**      real rv2x(0:2),rv2y(0:2),rv2xx(0:2),rv2xy(0:2),rv2yy(0:2),rv2xxx(0:2),rv2xxy(0:2),rv2xyy(0:2),rv2yyy(0:2),\
c**           rv2xxxx(0:2),rv2xxyy(0:2),rv2yyyy(0:2)
c**      real sv2x(0:2),sv2y(0:2),sv2xx(0:2),sv2xy(0:2),sv2yy(0:2),sv2xxx(0:2),sv2xxy(0:2),sv2xyy(0:2),sv2yyy(0:2),\
c**           sv2xxxx(0:2),sv2xxyy(0:2),sv2yyyy(0:2)

      integer numberOfEquations,job
      real a2(0:1,0:1),a3(0:2,0:2),a4(0:3,0:3),a8(0:7,0:7),q(0:11),f(0:11),rcond,work(0:11)
      integer ipvt(0:11)

      real err

      ! boundary conditions parameters and interfaceType values
      #Include "bcDefineFortranInclude.h"
 
      integer rectangular,curvilinear
      parameter(\
        rectangular=0,\
        curvilinear=1)


c     --- start statement function ----
      real bcf
      integer kd,m,n
      real uxOneSided
c     real rx,ry,rz,sx,sy,sz,tx,ty,tz
      declareDifferenceNewOrder2(u,rsxy,dr,dx,RX)

      declareDifferenceNewOrder4(u,rsxy,dr,dx,RX)

c     The next macro call will define the difference approximation statement functions
      defineDifferenceNewOrder2Components1(u,rsxy,dr,dx,RX)

      defineDifferenceNewOrder4Components1(u,rsxy,dr,dx,RX)

      ! 4th-order 1 sided derivative  extrap=(1 5 10 10 5 1)
      uxOneSided(i1,i2,i3,m)=-(10./3.)*u(i1,i2,i3,m)+6.*u(i1+is1,i2+is2,i3+is3,m)-2.*u(i1+2*is1,i2+2*is2,i3+2*is3,m)\
                             +(1./3.)*u(i1+3*is1,i2+3*is2,i3+3*is3,m)

      ! Here is the the generic boundary condition forcing array. It uses the bcOffset(side,axis) values as an
      ! an offset from the bcf0 array to access the bcf10, bcf01, bcf11, ... arrays
      bcf(side,axis,i1,i2,i3,m) = bcf0(bcOffset(side,axis) + \
          (i1-dim(0,0,side,axis)+(dim(1,0,side,axis)-dim(0,0,side,axis)+1)* \
          (i2-dim(0,1,side,axis)+(dim(1,1,side,axis)-dim(0,1,side,axis)+1)* \
          (i3-dim(0,2,side,axis)+(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(m)))))

c............... end statement functions

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

      job=0  ! *wdh* 090101

      if( debug.gt.3 )then
        write(*,'(" bcOptSm: mu,lambda,c1,c2=",4f10.5," gridType=",i2)') mu,lambda,c1,c2,gridType
           ! '
      end if

      if( debug.gt.7 )then
        write(*,'(" bcOptSm: **START** grid=",i4," uc,vc,wc=",3i2)') grid,uc,vc,wc
           ! '
      end if
      if( debug.gt.7 )then
       n1a=gridIndexRange(0,0)
       n1b=gridIndexRange(1,0)
       n2a=gridIndexRange(0,1)
       n2b=gridIndexRange(1,1)
       n3a=gridIndexRange(0,2)
       n3b=gridIndexRange(1,2)
       write(*,'(" bcOpt: grid=",i3,",n1a,n1b,n2a,n2b,n3a,n3b=",6i3)') grid,n1a,n1b,n2a,n2b,n3a,n3b
       ! write(*,*) 'bcOptSm: u=',((((u(i1,i2,i3,m),m=0,nd-1),i1=n1a,n1b),i2=n2a,n2b),i3=n3a,n3b)
      end if
     
      if( materialFormat.ne.constantMaterialProperties )then
        write(*,'(" ***bcOptSm:ERROR: Finish me for variable material")')
        stop 7736
      end if

      epsx=1.e-20  ! fix this 


      numGhost=orderOfAccuracy/2

      ! write(*,*) 'bcOffset:',bcOffset

      ! assign corners and edges (3d)
      if( .false. )then ! *wdh* 071027 -- turn this off for now ---
      if( orderOfAccuracy.eq.2 .and. nd.eq.2 )then

        ! *** fix this for traction BCs with forcing 

       ! For interfaces and TZ it is ok to just set the corners using TZ. This is maybe cheating a bit.
       if( gridType.eq.rectangular )then
        if( twilightZone.eq.0 )then
          assignCorners2d(2,rectangular,none)
        else
          assignCorners2d(2,rectangular,twilightZone)
        end if
       else
        if( twilightZone.eq.0 )then
          assignCorners2d(2,curvilinear,none)
        else
          assignCorners2d(2,curvilinear,twilightZone)
        end if
       end if      

      else if( orderOfAccuracy.eq.2 .and. nd.eq.3 )then

c$$$       if( gridType.eq.rectangular )then
c$$$        if( twilightZone.eq.0 )then
c$$$          assignCorners3d(2,rectangular,none)
c$$$        else
c$$$          assignCorners2d(2,rectangular,twilightZone)
c$$$        end if
c$$$       else
c$$$        if( twilightZone.eq.0 )then
c$$$          assignCorners3d(2,curvilinear,none)
c$$$        else
c$$$          assignCorners3d(2,curvilinear,twilightZone)
c$$$        end if
c$$$       end if      

      else
         stop 5533
      end if

      end if


      if( .false. )then
        ! check the boundary forcing arrays: check that bcf(side,axis,i1,i2,i3,m) agrees with bcf00, bcf10, ...
       write(*,*) dim
      beginLoopOverSides(numGhost,numGhost)
       write(*,'(" BCF: side,axis=",2i3," bcOffset(side,axis)=",i8)') side,axis,bcOffset(side,axis)

       if( addBoundaryForcing(side,axis).ne.0 )then
         beginLoops3d()
           if( side.eq.0 .and. axis.eq.0 )then
             do m=0,nd-1
               tmp = bcf00(i1,i2,i3,m) - bcf(side,axis,i1,i2,i3,m)
               write(*,'(" BCF(0,0): i=",3i3," f=",e8.2,2x,e8.2," diff=",e8.2)') i1,i2,i3,bcf00(i1,i2,i3,m),bcf(side,axis,i1,i2,i3,m),tmp
               ! '
             end do
           else if( side.eq.1 .and. axis.eq.0 )then
             do m=0,nd-1
               tmp = bcf10(i1,i2,i3,m) - bcf(side,axis,i1,i2,i3,m)
               write(*,'(" BCF(1,0): i=",3i3," f=",e8.2,2x,e8.2," diff=",e8.2)') i1,i2,i3,bcf10(i1,i2,i3,m),bcf(side,axis,i1,i2,i3,m),tmp
               ! '
             end do
           else if( side.eq.0 .and. axis.eq.1 )then
             do m=0,nd-1
               tmp = bcf01(i1,i2,i3,m) - bcf(side,axis,i1,i2,i3,m)
               write(*,'(" BCF(0,1): i=",3i3," f=",e8.2,2x,e8.2," diff=",e8.2)') i1,i2,i3,bcf01(i1,i2,i3,m),bcf(side,axis,i1,i2,i3,m),tmp
               ! '
             end do
           else if( side.eq.1 .and. axis.eq.1 )then
             do m=0,nd-1
               tmp = bcf11(i1,i2,i3,m) - bcf(side,axis,i1,i2,i3,m)
               write(*,'(" BCF(1,1): i=",3i3," f=",e8.2,2x,e8.2," diff=",e8.2)') i1,i2,i3,bcf11(i1,i2,i3,m),bcf(side,axis,i1,i2,i3,m),tmp
               ! '
             end do
           else if( side.eq.0 .and. axis.eq.2 )then
             do m=0,nd-1
               tmp = bcf02(i1,i2,i3,m) - bcf(side,axis,i1,i2,i3,m)
               write(*,'(" BCF(0,2): i=",3i3," f=",e8.2,2x,e8.2," diff=",e8.2)') i1,i2,i3,bcf02(i1,i2,i3,m),bcf(side,axis,i1,i2,i3,m),tmp
               ! '
             end do
           else if( side.eq.1 .and. axis.eq.2 )then
             do m=0,nd-1
               tmp = bcf12(i1,i2,i3,m) - bcf(side,axis,i1,i2,i3,m)
               write(*,'(" BCF(1,2): i=",3i3," f=",e8.2,2x,e8.2," diff=",e8.2)') i1,i2,i3,bcf12(i1,i2,i3,m),bcf(side,axis,i1,i2,i3,m),tmp
               ! '
             end do
           end if
         endLoops3d()
       end if
      endLoopOverSides()
      end if


      if( nd.eq.2 )then

        ! *********************************** 
        ! **************** 2D ***************
        ! *********************************** 

        ! -----------------------------------
        ! -----------2nd Order---------------
        ! -----------------------------------

       if( orderOfAccuracy.eq.2 .and. gridType.eq.rectangular )then
  
        beginLoopOverSides(numGhost,numGhost)
         if( boundaryCondition(side,axis).eq.displacementBC )then

           ! displacement BC's are done in the calling routine

           if( addBoundaryForcing(side,axis).eq.0 )then
            ! displacementBC2dMacro(noForcing,2)
           else
            ! displacementBC2dMacro(forcing,2)
           end if


         else if( boundaryCondition(side,axis).eq.tractionBC )then 
         
           ! first extrap values to ghost points (may be needed at corners)
           ! *wdh* 081117 -- this is still needed
           beginGhostLoops2d()
             u(i1-is1,i2-is2,i3,uc)=extrap3(u,i1,i2,i3,uc,is1,is2,is3)
             u(i1-is1,i2-is2,i3,vc)=extrap3(u,i1,i2,i3,vc,is1,is2,is3)
           endLoops2d()

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

         else if( boundaryCondition(side,axis).eq.dirichletBoundaryCondition .or.\
                  boundaryCondition(side,axis).eq.symmetry )then
           ! do nothing here
         else if( boundaryCondition(side,axis).gt.0 )then

           stop 1193

         end if

        endLoopOverSides()

        ! *********** now apply BC's that assign the ghost values *********
        beginLoopOverSides(numGhost,numGhost)

         if( boundaryCondition(side,axis).eq.tractionBC )then 
         
           alpha=lambda/(lambda+2.*mu)
           beta =1./(lambda+2.*mu)
           if( axis.eq.0 )then
             ! u.x = -alpha*v.y  
             ! v.x = -u.y       
            if( addBoundaryForcing(side,axis).eq.0 )then
             ! no forcing 
             beginLoopsMask2d()
              u(i1-is1,i2-is2,i3,uc)=u(i1+is1,i2+is2,i3,uc)+is1*dx(0)*2.*alpha*uy22r(i1,i2,i3,vc)
              u(i1-is1,i2-is2,i3,vc)=u(i1+is1,i2+is2,i3,vc)+is1*dx(0)*2.*      uy22r(i1,i2,i3,uc)
             endLoopsMask2d()

            else if( assignTwilightZone.eq.0 )then
              ! include forcing terms 
             beginLoopsMask2d()
              u(i1-is1,i2-is2,i3,uc)=u(i1+is1,i2+is2,i3,uc)+dx(0)*2.*(\
                                 is1*alpha*uy22r(i1,i2,i3,vc) + beta*bcf(side,axis,i1,i2,i3,uc) )
              u(i1-is1,i2-is2,i3,vc)=u(i1+is1,i2+is2,i3,vc)+dx(0)*2.*(\
                                 is1*uy22r(i1,i2,i3,uc)    + (1./mu)*bcf(side,axis,i1,i2,i3,vc) )
             endLoopsMask2d()

            else
             ! Twilight-zone: 
             ! u.x = -alpha*v.y + ue.x -alpha*ve.y 
             beginLoopsMask2d()
              OGDERIV2D(0,1,0,0,i1,i2,i3,t,ux0,vx0)
              OGDERIV2D(0,0,1,0,i1,i2,i3,t,uy0,vy0)
              u(i1-is1,i2-is2,i3,uc)=u(i1+is1,i2+is2,i3,uc)-\
                          is1*dx(0)*2.*(-alpha*uy22r(i1,i2,i3,vc) + ux0 +alpha*vy0 )
              u(i1-is1,i2-is2,i3,vc)=u(i1+is1,i2+is2,i3,vc)-\
                          is1*dx(0)*2.*(-uy22r(i1,i2,i3,uc) + vx0 + uy0 )
!     write(*,'("i1,i2=",2i3," ux0,vx0,uy0,vy0=",4e10.2)') i1,i2, ux0,vx0,uy0,vy0                
             endLoopsMask2d()
            end if

           else
           ! u.y = - v.x
           ! v.y = -alpha*u.x 
            if( addBoundaryForcing(side,axis).eq.0 )then
             beginLoopsMask2d()
              u(i1-is1,i2-is2,i3,uc)=u(i1+is1,i2+is2,i3,uc)+is2*dx(1)*2.*      ux22r(i1,i2,i3,vc)
              u(i1-is1,i2-is2,i3,vc)=u(i1+is1,i2+is2,i3,vc)+is2*dx(1)*2.*alpha*ux22r(i1,i2,i3,uc)
             endLoopsMask2d()

            else if( assignTwilightZone.eq.0 )then
              ! include forcing terms
             beginLoopsMask2d()
              u(i1-is1,i2-is2,i3,uc)=u(i1+is1,i2+is2,i3,uc)+dx(1)*2.*(\
                                   is2*ux22r(i1,i2,i3,vc)    + (1./mu)*bcf(side,axis,i1,i2,i3,uc) )
              u(i1-is1,i2-is2,i3,vc)=u(i1+is1,i2+is2,i3,vc)+dx(1)*2.*(\
                                   is2*alpha*ux22r(i1,i2,i3,uc) + beta*bcf(side,axis,i1,i2,i3,vc) )
             endLoopsMask2d()

            else
             ! Twilight-zone: 
             beginLoopsMask2d()
              OGDERIV2D(0,1,0,0,i1,i2,i3,t,ux0,vx0)
              OGDERIV2D(0,0,1,0,i1,i2,i3,t,uy0,vy0)
              u(i1-is1,i2-is2,i3,uc)=u(i1+is1,i2+is2,i3,uc)-is2*dx(1)*2.*(      -ux22r(i1,i2,i3,vc)+uy0+vx0)
              u(i1-is1,i2-is2,i3,vc)=u(i1+is1,i2+is2,i3,vc)-is2*dx(1)*2.*(-alpha*ux22r(i1,i2,i3,uc)+vy0+alpha*ux0)
             endLoopsMask2d()
            end if
           end if      

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

         end if ! end bc 

        endLoopOverSides()


       else if( orderOfAccuracy.eq.2 .and. gridType.eq.curvilinear )then
  
        ! *********************************************
        ! ************* 2d Curvilinear ****************
        ! *********************************************

        beginLoopOverSides(numGhost,numGhost)

         if( boundaryCondition(side,axis).eq.displacementBC )then

           ! For now displacement BC's are done by the calling program
           if( addBoundaryForcing(side,axis).eq.0 )then
            ! displacementBC2dMacro(noForcing,2)
           else
            ! displacementBC2dMacro(forcing,2)
           end if

          else if( boundaryCondition(side,axis).eq.tractionBC )then 
         
           ! first extrap values to ghost points (may be needed at corners)
           beginGhostLoops2d()
             u(i1-is1,i2-is2,i3,uc)=extrap3(u,i1,i2,i3,uc,is1,is2,is3)
             u(i1-is1,i2-is2,i3,vc)=extrap3(u,i1,i2,i3,vc,is1,is2,is3)
           endLoops2d()
        
          else if( boundaryCondition(side,axis).eq.slipWall )then

           if( addBoundaryForcing(side,axis).eq.0 )then
             ! no forcing 
             beginLoopsMask2d()
              ! (an1,an2) : outward normal is=1-2*side
              getNormal2d(i1,i2,i3,axis)
              u1 = u(i1,i2,i3,uc)
              v1 = u(i1,i2,i3,vc)
              nDotU = an1*u1 + an2*v1  
              u(i1,i2,i3,uc)=u1 - nDotU*an1
              u(i1,i2,i3,vc)=v1 - nDotU*an2
             endLoopsMask2d()

            else if( assignTwilightZone.eq.0 )then
              ! include forcing terms 
              ! n.u = n.g 
             beginLoopsMask2d()
              getNormal2d(i1,i2,i3,axis)
              u1 = u(i1,i2,i3,uc)
              v1 = u(i1,i2,i3,vc)
              nDotU = an1*(u1-bcf(side,axis,i1,i2,i3,uc)) + an2*(v1-bcf(side,axis,i1,i2,i3,vc))
              u(i1,i2,i3,uc)=u1 - nDotU*an1
              u(i1,i2,i3,vc)=v1 - nDotU*an2
             endLoopsMask2d()

            else
             ! Twilight-zone: 
             !   n.u = n.ue
             beginLoopsMask2d()
              getNormal2d(i1,i2,i3,axis)
              OGF2D(i1,i2,i3,t,u0,v0)
              u1 = u(i1,i2,i3,uc)
              v1 = u(i1,i2,i3,vc)
              nDotU = an1*(u1-u0) + an2*(v1-v0)
              u(i1,i2,i3,uc)=u1 - nDotU*an1
              u(i1,i2,i3,vc)=v1 - nDotU*an2
             endLoopsMask2d()
            end if

            ! extrap values to the ghost line 
            beginGhostLoops2d()
             u(i1-is1,i2-is2,i3,uc)=extrap3(u,i1,i2,i3,uc,is1,is2,is3)
             u(i1-is1,i2-is2,i3,vc)=extrap3(u,i1,i2,i3,vc,is1,is2,is3)
            endLoops2d()

          else if( boundaryCondition(side,axis).eq.dirichletBoundaryCondition .or.\
                   boundaryCondition(side,axis).eq.symmetry )then
            ! do nothing here
          else if( boundaryCondition(side,axis).gt.0 )then

           stop 1193

          end if

        endLoopOverSides()

        ! ** now apply BC's that assign the ghost values *********
        beginLoopOverSides(numGhost,numGhost)

         if( boundaryCondition(side,axis).eq.tractionBC )then 

           if( addBoundaryForcing(side,axis).eq.0 )then
            tractionBCCurvilinear2dMacro(noForcing)
           else
            tractionBCCurvilinear2dMacro(forcing)
           end if

         else if( boundaryCondition(side,axis).eq.slipWall )then

           if( addBoundaryForcing(side,axis).eq.0 )then
            slipWallBCCurvilinear2dMacro(noForcing)
           else
            slipWallBCCurvilinear2dMacro(forcing)
           end if

         end if

        endLoopOverSides()


        ! -----------------------------------
        ! -----------4th Order---------------
        ! -----------------------------------

       else if( orderOfAccuracy.eq.4 .and. gridType.eq.rectangular )then
  
        beginLoopOverSides(numGhost,numGhost)
         if( boundaryCondition(side,axis).eq.displacementBC )then

           ! For now displacement BC's are done by the calling program
           if( addBoundaryForcing(side,axis).eq.0 )then
            ! displacementBC2dMacro(noForcing,4)
           else
            ! displacementBC2dMacro(forcing,4)
           end if


         else if( boundaryCondition(side,axis).eq.tractionBC )then 
         
           ! first extrap values to ghost points (may be needed at corners)
           beginGhostLoops2d()
             u(i1-is1,i2-is2,i3,uc)=extrap5(u,i1,i2,i3,uc,is1,is2,is3)
             u(i1-is1,i2-is2,i3,vc)=extrap5(u,i1,i2,i3,vc,is1,is2,is3)

             u(i1-2*is1,i2-2*is2,i3,uc)=extrap5(u,i1-is1,i2-is2,i3,uc,is1,is2,is3)
             u(i1-2*is1,i2-2*is2,i3,vc)=extrap5(u,i1-is1,i2-is2,i3,vc,is1,is2,is3)
           endLoops2d()
        

         else if( boundaryCondition(side,axis).eq.dirichletBoundaryCondition .or.\
                  boundaryCondition(side,axis).eq.symmetry )then
           ! do nothing here
         else if( boundaryCondition(side,axis).gt.0 )then

           stop 1193

         end if

        endLoopOverSides()

        ! ** now apply BC's that assign the ghost values *********
        beginLoopOverSides(numGhost,numGhost)


         if( boundaryCondition(side,axis).eq.tractionBC )then 
         
           alpha=lambda/(lambda+2.*mu)
           if( axis.eq.0 )then
             ! u.x = -alpha*v.y  
             ! v.x = -u.y       

            if( addBoundaryForcing(side,axis).eq.0 )then
             beginLoopsMask2d()
              u(i1-is1,i2-is2,i3,uc)=uxOneSided(i1,i2,i3,uc)+is1*dx(0)*4.*alpha*uy42r(i1,i2,i3,vc)
              u(i1-is1,i2-is2,i3,vc)=uxOneSided(i1,i2,i3,vc)+is1*dx(0)*4.*      uy42r(i1,i2,i3,uc)
             endLoopsMask2d()
            else if( assignTwilightZone.eq.0 )then
              ! finish me
              stop 1609
            else
             ! u.x = -alpha*v.y + ue.x -alpha*ve.y 
             beginLoopsMask2d()
              OGDERIV2D(0,1,0,0,i1,i2,i3,t,ux0,vx0)
              OGDERIV2D(0,0,1,0,i1,i2,i3,t,uy0,vy0)
              u(i1-is1,i2-is2,i3,uc)=uxOneSided(i1,i2,i3,uc)-\
                          is1*dx(0)*4.*(-alpha*uy22r(i1,i2,i3,vc) + ux0 +alpha*vy0 )
              u(i1-is1,i2-is2,i3,vc)=uxOneSided(i1,i2,i3,vc)-\
                          is1*dx(0)*4.*(-uy22r(i1,i2,i3,uc) + vx0 + uy0 )
!     write(*,'("i1,i2=",2i3," ux0,vx0,uy0,vy0=",4e10.2)') i1,i2, ux0,vx0,uy0,vy0                
             endLoopsMask2d()
            end if

           else
             ! u.y = - v.x
             ! v.y = -alpha*u.x 
            if( addBoundaryForcing(side,axis).eq.0 )then
             beginLoopsMask2d()
              u(i1-is1,i2-is2,i3,uc)=uxOneSided(i1,i2,i3,uc)+is2*dx(1)*4.*      ux42r(i1,i2,i3,vc)
              u(i1-is1,i2-is2,i3,vc)=uxOneSided(i1,i2,i3,vc)+is2*dx(1)*4.*alpha*ux42r(i1,i2,i3,uc)
             endLoopsMask2d()
            else if( assignTwilightZone.eq.0 )then
              ! finish me
              stop 1633
            else
             beginLoopsMask2d()
              OGDERIV2D(0,1,0,0,i1,i2,i3,t,ux0,vx0)
              OGDERIV2D(0,0,1,0,i1,i2,i3,t,uy0,vy0)
              u(i1-is1,i2-is2,i3,uc)=uxOneSided(i1,i2,i3,uc)-is2*dx(1)*4.*(      -ux42r(i1,i2,i3,vc)+uy0+vx0)
              u(i1-is1,i2-is2,i3,vc)=uxOneSided(i1,i2,i3,vc)-is2*dx(1)*4.*(-alpha*ux42r(i1,i2,i3,uc)+vy0+alpha*ux0)
             endLoopsMask2d()
            end if
           end if      

           ! Now extrap second ghost line
           beginGhostLoops2d()
             u(i1-2*is1,i2-2*is2,i3,uc)=extrap5(u,i1-is1,i2-is2,i3,uc,is1,is2,is3)
             u(i1-2*is1,i2-2*is2,i3,vc)=extrap5(u,i1-is1,i2-is2,i3,vc,is1,is2,is3)
           endLoops2d()


         end if

        endLoopOverSides()


       else if( orderOfAccuracy.eq.4 .and. gridType.eq.curvilinear )then
  
        ! *********************************************
        ! ************* 2d Curvilinear ****************
        ! *********************************************

        beginLoopOverSides(numGhost,numGhost)

         if( boundaryCondition(side,axis).eq.displacementBC )then

           ! For now displacement BC's are done by the calling program
           if( addBoundaryForcing(side,axis).eq.0 )then
            ! displacementBC2dMacro(noForcing,4)
           else
            ! displacementBC2dMacro(forcing,4)
           end if

          else if( boundaryCondition(side,axis).eq.tractionBC )then 
         
           ! first extrap values to ghost points (may be needed at corners)
           beginGhostLoops2d()
             u(i1-is1,i2-is2,i3,uc)=extrap5(u,i1,i2,i3,uc,is1,is2,is3)
             u(i1-is1,i2-is2,i3,vc)=extrap5(u,i1,i2,i3,vc,is1,is2,is3)
           endLoops2d()
        
          else if( boundaryCondition(side,axis).eq.dirichletBoundaryCondition .or.\
                   boundaryCondition(side,axis).eq.symmetry )then
           ! do nothing here
          else if( boundaryCondition(side,axis).gt.0 )then

           stop 1193

          end if

        endLoopOverSides()

        ! ** now apply BC's that assign the ghost values *********
        beginLoopOverSides(numGhost,numGhost)

         if( boundaryCondition(side,axis).eq.tractionBC )then 

           if( addBoundaryForcing(side,axis).eq.0 )then
            tractionBCCurvilinear2dMacro(noForcing)
           else
            tractionBCCurvilinear2dMacro(forcing)
           end if

         end if

        endLoopOverSides()


       else 
         ! un-known nd and orderOfAccuracy
         stop 6663
       end if



      else if( nd.eq.3 )then
       !    *************************
       !    ********** 3D ***********
       !    *************************

       if( orderOfAccuracy.eq.2 .and. gridType.eq.rectangular )then
  
        beginLoopOverSides(numGhost,numGhost)
         if( boundaryCondition(side,axis).eq.displacementBC )then

           if( addBoundaryForcing(side,axis).eq.0 )then
            ! displacementBC3dMacro(noForcing,2)
           else
            ! displacementBC3dMacro(forcing,2)
           end if

         else if( boundaryCondition(side,axis).eq.tractionBC )then 
         
           ! first extrap values to ghost points (may be needed at corners)
           beginGhostLoops3d()
             u(i1-is1,i2-is2,i3-is3,uc)=extrap3(u,i1,i2,i3,uc,is1,is2,is3)
             u(i1-is1,i2-is2,i3-is3,vc)=extrap3(u,i1,i2,i3,vc,is1,is2,is3)
             u(i1-is1,i2-is2,i3-is3,wc)=extrap3(u,i1,i2,i3,wc,is1,is2,is3)
           endLoops3d()

         else if( boundaryCondition(side,axis).eq.dirichletBoundaryCondition .or.\
                  boundaryCondition(side,axis).eq.symmetry )then
           ! do nothing here

         else if( boundaryCondition(side,axis).gt.0 )then

           stop 1193

         end if

        endLoopOverSides()

        ! ** now apply BC's that assign the ghost values *********
        beginLoopOverSides(numGhost,numGhost)

         if( boundaryCondition(side,axis).eq.tractionBC )then 
         
           if( addBoundaryForcing(side,axis).eq.0 )then
            tractionBCRectangular3dMacro(noForcing)
           else
            tractionBCRectangular3dMacro(forcing)
           end if

         end if

        endLoopOverSides()


       else if( orderOfAccuracy.eq.2 .and. gridType.eq.curvilinear )then
  
        ! *********************************************
        ! ************* 3d Curvilinear ****************
        ! *********************************************

        beginLoopOverSides(numGhost,numGhost)

         if( boundaryCondition(side,axis).eq.displacementBC )then
           ! note: we can assign ghost pts in tangential dir too:

           if( addBoundaryForcing(side,axis).eq.0 )then
            ! displacementBC3dMacro(noForcing,2)
           else
            ! displacementBC3dMacro(forcing,2)
           end if

          else if( boundaryCondition(side,axis).eq.tractionBC )then 
         
           ! first extrap values to ghost points (may be needed at corners)
           beginGhostLoops3d()
             u(i1-is1,i2-is2,i3-is3,uc)=extrap3(u,i1,i2,i3,uc,is1,is2,is3)
             u(i1-is1,i2-is2,i3-is3,vc)=extrap3(u,i1,i2,i3,vc,is1,is2,is3)
             u(i1-is1,i2-is2,i3-is3,wc)=extrap3(u,i1,i2,i3,wc,is1,is2,is3)
           endLoops3d()
        
          else if( boundaryCondition(side,axis).eq.dirichletBoundaryCondition .or.\
                   boundaryCondition(side,axis).eq.symmetry )then
            ! do nothing here

          else if( boundaryCondition(side,axis).gt.0 )then

           stop 1193

          end if

        endLoopOverSides()

        ! ** now apply BC's that assign the ghost values *********
        beginLoopOverSides(numGhost,numGhost)

         if( boundaryCondition(side,axis).eq.tractionBC )then 

           if( addBoundaryForcing(side,axis).eq.0 )then
            tractionBCCurvilinear3dMacro(noForcing)
           else
            tractionBCCurvilinear3dMacro(forcing)
           end if

         end if

        endLoopOverSides()


       else 
         ! un-known nd and orderOfAccuracy
         stop 6663
       end if

      else
        ! unknown nd 
        stop 8826 
      end if 

      return
      end
