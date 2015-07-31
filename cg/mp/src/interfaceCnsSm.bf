c *******************************************************************************
c   Interface boundary conditions
c *******************************************************************************

c These next include files will define the macros that will define the difference approximations
c The actual macro is called below
#Include "defineDiffNewerOrder2f.h"
#Include "defineDiffNewerOrder4f.h"


! ====================================================================
! Look up an integer parameter from the data base
! ====================================================================
#beginMacro getIntParameter(pdb,name,value)
 ok=getInt(pdb,name,value) 
 if( ok.eq.0 )then
   write(*,'("*** interfaceCnsSm:ERROR: unable to find ",a)') name
   stop 1122
 end if
#endMacro

! ====================================================================
! Look up an integer parameter from the Cgins data base
! ====================================================================
#beginMacro getIntParameterCgins(pdb,name,value)
 ok=getIntCgins(pdb,name,value) 
 if( ok.eq.0 )then
   write(*,'("*** interfaceCnsSm:ERROR: unable to find ",a)')  name
   stop 1122
 end if
#endMacro

! ====================================================================
! Look up an integer parameter from the Cgins data base
! ====================================================================
#beginMacro getIntParameterCgcns(pdb,name,value)
 ok=getIntCgcns(pdb,name,value) 
 if( ok.eq.0 )then
   write(*,'("*** interfaceCnsSm:ERROR: unable to find",a)')  name
   stop 1122
 end if
#endMacro

! ====================================================================
! Look up an integer parameter from the Cgsm data base
! ====================================================================
#beginMacro getIntParameterCgsm(pdb,name,value)
 ok=getIntCgsm(pdb,name,value) 
 if( ok.eq.0 )then
   write(*,'("*** interfaceCnsSm:ERROR: unable to find ",a)') name
   stop 1122
 end if
#endMacro


! ====================================================================
! Look up a real parameter from the data base
! ====================================================================
#beginMacro getRealParameter(pdb,name)
 ok=getReal(pdb,'name',name) 
 if( ok.eq.0 )then
   write(*,'("*** interfaceCnsSm:ERROR: unable to find name")') 
   stop 1133
 end if
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


! This macro handles the case when the one boundary is over i1 and the other over j2 (over vice versa)
#beginMacro beginIJLoops2d()
 j3d=0
 if( m1a.eq.m1b )then
  j1d=0
 else if( m1a.lt.m1b )then
  j1d=+1
 else
  j1d=-1
 end if
 if( m2a.eq.m2b )then
  j2d=0
 else if( m2a.lt.m2b )then
  j2d=+1
 else
  j2d=-1
 end if

 i3=n3a
 j3=m3a

 j1=m1a
 j2=m2a
 do i2=n2a,n2b
  do i1=n1a,n1b
#endMacro
#beginMacro endIJLoops2d()
   j1=j1+j1d
   j2=j2+j2d
  end do
 end do
#endMacro


#beginMacro beginIJGhostLoops2d()
 j3d=0
 if( m1a.eq.m1b )then
  j1d=0
 else if( m1a.lt.m1b )then
  j1d=+1
 else
  j1d=-1
 end if
 if( m2a.eq.m2b )then
  j2d=0
 else if( m2a.lt.m2b )then
  j2d=+1
 else
  j2d=-1
 end if
 i3=n3a
 j3=m3a
 j2=mm2a
 j1=mm1a
 do i2=nn2a,nn2b
  do i1=nn1a,nn1b
#endMacro


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

#beginMacro beginGhostLoops2d()
 i3=n3a
 j3=m3a
 j2=mm2a
 do i2=nn2a,nn2b
  j1=mm1a
  do i1=nn1a,nn1b
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

#beginMacro beginGhostLoops3d()
 j3=mm3a
 do i3=nn3a,nn3b
 j2=mm2a
 do i2=nn2a,nn2b
  j1=mm1a
  do i1=nn1a,nn1b
#endMacro

#defineMacro extrap2(uu,k1,k2,k3,kc,ks1,ks2,ks3) \
            (2.*uu(k1,k2,k3,kc)-uu(k1+ks1,k2+ks2,k3+ks3,kc))

#defineMacro extrap3(uu,k1,k2,k3,kc,ks1,ks2,ks3) \
            (3.*uu(k1,k2,k3,kc)-3.*uu(k1+ks1,k2+ks2,k3+ks3,kc)\
            +   uu(k1+2*ks1,k2+2*ks2,k3+2*ks3,kc))

! Use a different index to start extrapolating at: 
#defineMacro extrapolate1(uu,k1,k2,k3,kc,ks1,ks2,ks3) \
            (uu(k1+ks1,k2+ks2,k3+ks3,kc))

#defineMacro extrapolate3(uu,k1,k2,k3,kc,ks1,ks2,ks3) \
            (3.*uu(k1+ks1,k2+ks2,k3+ks3,kc)-3.*uu(k1+2*ks1,k2+2*ks2,k3+2*ks3,kc)\
            +   uu(k1+3*ks1,k2+3*ks2,k3+3*ks3,kc))

#defineMacro extrap4(uu,k1,k2,k3,kc,ks1,ks2,ks3) \
            (4.*uu(k1,k2,k3,kc)-6.*uu(k1+ks1,k2+ks2,k3+ks3,kc)\
            +4.*uu(k1+2*ks1,k2+2*ks2,k3+2*ks3,kc)-uu(k1+3*ks1,k2+3*ks2,k3+3*ks3,kc))

#defineMacro extrap5(uu,k1,k2,k3,kc,ks1,ks2,ks3) \
            (5.*uu(k1,k2,k3,kc)-10.*uu(k1+ks1,k2+ks2,k3+ks3,kc)\
            +10.*uu(k1+2*ks1,k2+2*ks2,k3+2*ks3,kc)-5.*uu(k1+3*ks1,k2+3*ks2,k3+3*ks3,kc)\
            +uu(k1+4*ks1,k2+4*ks2,k3+4*ks3,kc))




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


#beginMacro getExact(ep,xy, i1,i2,i3,m, ue )
 ue=ogf(ep,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,kd3),m,t)
#endMacro

#beginMacro getDerivs(ep,xy, i1,i2,i3,m, uex,uey,uez, uexx,ueyy,uezz )
 call ogderiv(ep, 0,1,0,0, xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,kd3),t,m,uex)
 call ogderiv(ep, 0,0,1,0, xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,kd3),t,m,uey)
 call ogderiv(ep, 0,2,0,0, xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,kd3),t,m,uexx)
 call ogderiv(ep, 0,0,2,0, xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,kd3),t,m,ueyy)
if( nd.gt.2 )then
  call ogderiv(ep, 0,0,0,1, xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,kd3),t,m,uez)
  call ogderiv(ep, 0,0,0,2, xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,kd3),t,m,uezz)
end if
#endMacro

! Convert conservative vars (rho,rho*u,rho*v,E) to primitive vars (rho,u,v,p)
#beginMacro convertToPrim(i1,i2,i3)
  rhof = u1(i1,i2,i3,rc)
  u1(i1,i2,i3,uc)=u1(i1,i2,i3,uc)/rhof
  u1(i1,i2,i3,vc)=u1(i1,i2,i3,vc)/rhof
  u1(i1,i2,i3,pc)=(gamma-1.)*( u1(i1,i2,i3,tc) -.5*rhof*( u1(i1,i2,i3,uc)**2 + u1(i1,i2,i3,vc)**2 ) )
#endMacro

! Convert primitive vars (rho,u,v,p) to conservative vars (rho,rho*u,rho*v,E)
#beginMacro convertToCons(i1,i2,i3)
  rhof = u1(i1,i2,i3,rc)
  u1(i1,i2,i3,pc)=u1(i1,i2,i3,pc)/(gamma-1.) + .5*rhof*( u1(i1,i2,i3,uc)**2 + u1(i1,i2,i3,vc)**2 )
  u1(i1,i2,i3,uc)=u1(i1,i2,i3,uc)*rhof
  u1(i1,i2,i3,vc)=u1(i1,i2,i3,vc)*rhof
#endMacro


! ==================================================================================
!  Limited extrapolation of point u(j1,j2,j3,m) in the direction is1,is2,is3
!  m = component number 
!  From cg/cns/src/cnsSlipWallBC2.bf 
! ==================================================================================
#beginMacro limitedExtrapolation(u,j1,j2,j3,is1,is2,is3,m)
  du1 = u(j1+is1,j2+is2,j3,m)
  du2 = 2.*u(j1+is1,j2+is2,j3,m)-u(j1+2*is1,j2+2*is2,j3,m) 
  du3 = 3.*u(j1+is1,j2+is2,j3,m)-3.*u(j1+2*is1,j2+2*is2,j3,m)+u(j1+3*is1,j2+3*is2,j3,m)

  uNorm= uEps+ abs(du3) + abs(u(j1+is1,j2+is2,j3,m))+abs(u(j1+2*is1,j2+2*is2,j3,m))
  cdl=1.
  alpha = cdl*( abs(du3-du2)/uNorm )
  alpha =min(1.,alpha)

  !   u(j1,j2,j3,m)=(1.-alpha)*du3+alpha*du2
  ! Use 3rd-order extrap for smooth solutions, use 1st order extra for for non-smooth:
  u(j1,j2,j3,m)=(1.-alpha)*du3+alpha*du1

#endMacro

! ==================================================================================
!  Limited extrapolation of point u(j1,j2,j3,m) in the direction is1,is2,is3
!  m = component number 
!  us : use this value as the limiting value (usually "symmetric" point across a boundary)
!  From cg/cns/src/cnsSlipWallBC2.bf 
! ==================================================================================
#beginMacro limitedRhoExtrapolation(u,j1,j2,j3,is1,is2,is3,m, us)
  du1 =    u(j1+is1,j2+is2,j3,m)
  du2 = 2.*u(j1+is1,j2+is2,j3,m)-u(j1+2*is1,j2+2*is2,j3,m) 
  du3 = 3.*u(j1+is1,j2+is2,j3,m)-3.*u(j1+2*is1,j2+2*is2,j3,m)+u(j1+3*is1,j2+3*is2,j3,m)

  uNorm= uEps+ abs(du3) + abs(u(j1+is1,j2+is2,j3,m))+abs(u(j1+2*is1,j2+2*is2,j3,m))
  cdl=1.
  alpha = cdl*( abs(du3-du2)/uNorm )

  alpha =min(1.,alpha)

  ! Use 3rd-order extrap for smooth solutions, use "us" for non-smooth:
  u(j1,j2,j3,m)=(1.-alpha)*du3+alpha*(us)

  ! write(debugFile,'(" limitedRhoExtrap: ",2i3," du3,du2,uNorm,alpha,uus, uu=",6e12.3)') j1,j2,du3,du2,uNorm,alpha,us,u(j1,j2,j3,m)
#endMacro

#beginMacro applyLowerBound( u,uMin )
 u=max(u,uMin)
#endMacro

#beginMacro applyUpperBound( u,uMax )
 u=min(u,uMax)
#endMacro

! ==================================================================================
!  Set u(j1,j2,j3,m) =uVal unless the 3rd order extrapolated value does not agree with it
!  m = component number 
! ==================================================================================
#beginMacro limitWithExtrap(uVal,  u,j1,j2,j3,is1,is2,is3,m)
  du1 = u(j1+is1,j2+is2,j3,m)
  du2 = 2.*u(j1+is1,j2+is2,j3,m)-u(j1+2*is1,j2+2*is2,j3,m) 
  du3 = 3.*u(j1+is1,j2+is2,j3,m)-3.*u(j1+2*is1,j2+2*is2,j3,m)+u(j1+3*is1,j2+3*is2,j3,m)

  uNorm= uEps+ abs(du3) + abs(u(j1+is1,j2+is2,j3,m))+abs(u(j1+2*is1,j2+2*is2,j3,m))
  cdl=1.
  alpha = cdl*( abs(du3-uVal)/uNorm )  ! note uVal
  alpha =min(1.,alpha)

  !   u(j1,j2,j3,m)=(1.-alpha)*du3+alpha*du2
  ! Use uVal for smooth solutions, use 1st order extra for for non-smooth:
  u(j1,j2,j3,m)=(1.-alpha)*uVal + alpha*du1

#endMacro


#beginMacro assignCornerGhostPoints()
 ! ----------------------------------
 ! --- Assign corner ghost points ---
 ! ----------------------------------

 ! Assign corner ghost and extended boundary points (marked X below):
 ! 
 !     X--X--X--+--+--      
 !     |  |  |  |  | 
 !     X--X--X--+--+--  
 !     |  |  |  |  | 
 !     G--G--B--B--B--      B = boundary 
 !     |  |  |  |  |
 !     G--G--B--+--+--      G = ghost points already assigned above
 !     |  |  |  |  |
 !     G--G--B--+--+--  
 !     |  |  |  |  |


 ! (side1,axis1) and (side2,axis2)

 numberOfCornerGhost=2

 do adjSide=0,1 ! adjacent sides

   ! set domain 1 "i" loop bounds
   if( axis1.eq.0 )then
     ! we extrap in the (ks1,ks2,ks3) dir: 
     ks1=0
     ks2=1-2*adjSide
     ks3=0
     if( side1.eq.0 )then
       ng1a=n1a-numberOfCornerGhost
       ng1b=n1a   ! include extended boundary points 
       ng1c=1    
     else
       ng1a=n1a
       ng1b=n1a+numberOfCornerGhost
       ng1c=1
     end if
     if( adjSide.eq.0 )then
      ng2a=n2a-1
      ng2b=n2a-numberOfCornerGhost
      ng2c=-1
     else
      ng2a=n2b+1
      ng2b=n2b+numberOfCornerGhost
      ng2c=+1
     end if
   else ! axis1=1 
     ! we extrap in the (ks1,ks2,ks3) dir: 
     ks1=1-2*adjSide
     ks2=0
     ks3=0
     if( side1.eq.0 )then
       ng2a=n2a-numberOfCornerGhost
       ng2b=n2a
       ng2c=1
     else
       ng2a=n2a
       ng2b=n2a+numberOfCornerGhost
       ng2c=1
     end if
     if( adjSide.eq.0 )then
      ng1a=n1a-1
      ng1b=n1a-numberOfCornerGhost
      ng1c=-1
     else
      ng1a=n1b+1
      ng1b=n1b+numberOfCornerGhost
      ng1c=+1
     end if
   end if
   ! set domain 2 "j" loop bounds
   if( axis2.eq.0 )then
     ! we extrap in the (ls1,ls2,ls3) dir: 
     ls1=0
     ls2=1-2*adjSide
     ls3=0
     if( side2.eq.0 )then
       mg1a=m1a-numberOfCornerGhost
       mg1b=m1a
       mg1c=1
     else
       mg1a=m1a
       mg1b=m1a+numberOfCornerGhost
       mg1c=1
     end if
     if( adjSide.eq.0 )then
      mg2a=m2a-1
      mg2b=m2a-numberOfCornerGhost
      mg2c=-1
     else
      mg2a=m2b+1
      mg2b=m2b+numberOfCornerGhost
      mg2c=+1
     end if
   else ! axis2=1 
     ! we extrap in the (ls1,ls2,ls3) dir: 
     ls1=1-2*adjSide
     ls2=0
     ls3=0
     if( side2.eq.0 )then
       mg2a=m2a-numberOfCornerGhost
       mg2b=m2a
       mg2c=1
     else
       mg2a=m2a
       mg2b=m2a+numberOfCornerGhost
       mg2c=1
     end if
     if( adjSide.eq.0 )then
      mg1a=m1a-1
      mg1b=m1a-numberOfCornerGhost
      mg1c=-1
     else
      mg1a=m1b+1
      mg1b=m1b+numberOfCornerGhost
      mg1c=+1
     end if
   end if

!  write(*,'("IBC: set corner ghost (ng1a,ng1b,ng1c)=(",3i3,") (ng2a,ng2b,ng2c)=(",3i3,") (ks1,ks2)=(",2i2,")")') ng1a,ng1b,ng1c,ng2a,ng2b,ng2c,ks1,ks2
!  write(*,'("IBC: set corner ghost (mg1a,mg1b,mg1c)=(",3i3,") (mg2a,mg2b,mg2c)=(",3i3,") (ls1,ls2)=(",2i2,")")') mg1a,mg1b,mg1c,mg2a,mg2b,mg2c,ls1,ls2

   ! **** FIX ME FOR THE GENERAL CASE ****
   if( boundaryCondition1(axis1p1,adjSide).gt.0 )then
     j3=m3a
     i3=n3a
     do i2=ng2a,ng2b,ng2c
      do i1=ng1a,ng1b,ng1c
       u1(i1,i2,i3,rc)=extrapolate3(u1,i1,i2,i3,rc,ks1,ks2,ks3)
       u1(i1,i2,i3,uc)=extrapolate3(u1,i1,i2,i3,uc,ks1,ks2,ks3)
       u1(i1,i2,i3,vc)=extrapolate3(u1,i1,i2,i3,vc,ks1,ks2,ks3)
       u1(i1,i2,i3,tc)=extrapolate3(u1,i1,i2,i3,tc,ks1,ks2,ks3)
      end do 
     end do 
   end if  
   if( boundaryCondition2(axis2p1,adjSide).gt.0 )then 
     do j2=mg2a,mg2b,mg2c
      do j1=mg1a,mg1b,mg1c
       u2(j1,j2,j3,u1c)=extrapolate3(u2,j1,j2,j3,u1c,ls1,ls2,ls3)
       u2(j1,j2,j3,u2c)=extrapolate3(u2,j1,j2,j3,u2c,ls1,ls2,ls3)
  
       u2(j1,j2,j3,v1c)=extrapolate3(u2,j1,j2,j3,v1c,ls1,ls2,ls3)
       u2(j1,j2,j3,v2c)=extrapolate3(u2,j1,j2,j3,v2c,ls1,ls2,ls3)
       u2(j1,j2,j3,s11c)=extrapolate3(u2,j1,j2,j3,s11c,ls1,ls2,ls3)
       u2(j1,j2,j3,s12c)=extrapolate3(u2,j1,j2,j3,s12c,ls1,ls2,ls3)
       u2(j1,j2,j3,s22c)=extrapolate3(u2,j1,j2,j3,s22c,ls1,ls2,ls3)
       ! u2(j1,j2,j3,s21c)=u2(j1,j2,j3,s12c)
       u2(j1,j2,j3,s21c)=extrapolate3(u2,j1,j2,j3,s21c,ls1,ls2,ls3)
      end do 
     end do 
   end if

 end do

#endMacro


! compute the  fluid normal (n1f,n2f)
#beginMacro getFluidNormal()
 rx1=rsxy1(i1,i2,i3,axis1,0)   
 ry1=rsxy1(i1,i2,i3,axis1,1)
 r1Norm=normalSign1*max(epsx,sqrt(rx1**2+ry1**2))
 n1f=-rx1/r1Norm  ! NOTE: flip sign of fluid normal
 n2f=-ry1/r1Norm
#endMacro	


! compute the solid normal (n1s,n2s)
#beginMacro getSolidNormal()
 rx2=rsxy2(j1,j2,j3,axis2,0)
 ry2=rsxy2(j1,j2,j3,axis2,1)
 r2Norm=normalSign2*max(epsx,sqrt(rx2**2+ry2**2))
 n1s=rx2/r2Norm
 n2s=ry2/r2Norm
#endMacro

! =================================================================================
!   Extrap two ghost lines
! =================================================================================
#beginMacro extrapTwoGhostMacro()
 beginIJGhostLoops2d()

   ! first ghost line 
   i1g=i1-is1
   i2g=i2-is2
   i3g=i3-is3
   u1(i1g,i2g,i3g,rc)=extrapolate3(u1,i1g,i2g,i3g,rc,is1,is2,is3)
   u1(i1g,i2g,i3g,uc)=extrapolate3(u1,i1g,i2g,i3g,uc,is1,is2,is3)
   u1(i1g,i2g,i3g,vc)=extrapolate3(u1,i1g,i2g,i3g,vc,is1,is2,is3)
   u1(i1g,i2g,i3g,pc)=extrapolate3(u1,i1g,i2g,i3g,pc,is1,is2,is3)

   j1g=j1-js1
   j2g=j2-js2
   j3g=j3-js3
   u2(j1g,j2g,j3g,u1c)=extrapolate3(u2,j1g,j2g,j3g,u1c,js1,js2,js3)
   u2(j1g,j2g,j3g,u2c)=extrapolate3(u2,j1g,j2g,j3g,u2c,js1,js2,js3)

   u2(j1g,j2g,j3g,v1c)=extrapolate3(u2,j1g,j2g,j3g,v1c,js1,js2,js3)
   u2(j1g,j2g,j3g,v2c)=extrapolate3(u2,j1g,j2g,j3g,v2c,js1,js2,js3)

   u2(j1g,j2g,j3g,s11c)=extrapolate3(u2,j1g,j2g,j3g,s11c,js1,js2,js3)
   u2(j1g,j2g,j3g,s12c)=extrapolate3(u2,j1g,j2g,j3g,s12c,js1,js2,js3)
   u2(j1g,j2g,j3g,s22c)=extrapolate3(u2,j1g,j2g,j3g,s22c,js1,js2,js3)
   ! u2(j1g,j2g,j3g,s21c)=u2(j1g,j2g,j3g,s12c)
   u2(j1g,j2g,j3g,s21c)=extrapolate3(u2,j1g,j2g,j3g,s21c,js1,js2,js3)

   ! -- second ghost 
   i1g=i1-2*is1
   i2g=i2-2*is2
   i3g=i3-2*is3
   u1(i1g,i2g,i3g,rc)=extrapolate3(u1,i1g,i2g,i3g,rc,is1,is2,is3)
   u1(i1g,i2g,i3g,uc)=extrapolate3(u1,i1g,i2g,i3g,uc,is1,is2,is3)
   u1(i1g,i2g,i3g,vc)=extrapolate3(u1,i1g,i2g,i3g,vc,is1,is2,is3)
   u1(i1g,i2g,i3g,pc)=extrapolate3(u1,i1g,i2g,i3g,pc,is1,is2,is3)

   j1g=j1-2*js1
   j2g=j2-2*js2
   j3g=j3-2*js3
   u2(j1g,j2g,j3g,u1c)=extrapolate3(u2,j1g,j2g,j3g,u1c,js1,js2,js3)
   u2(j1g,j2g,j3g,u2c)=extrapolate3(u2,j1g,j2g,j3g,u2c,js1,js2,js3)

   u2(j1g,j2g,j3g,v1c)=extrapolate3(u2,j1g,j2g,j3g,v1c,js1,js2,js3)
   u2(j1g,j2g,j3g,v2c)=extrapolate3(u2,j1g,j2g,j3g,v2c,js1,js2,js3)

   u2(j1g,j2g,j3g,s11c)=extrapolate3(u2,j1g,j2g,j3g,s11c,js1,js2,js3)
   u2(j1g,j2g,j3g,s12c)=extrapolate3(u2,j1g,j2g,j3g,s12c,js1,js2,js3)
   u2(j1g,j2g,j3g,s22c)=extrapolate3(u2,j1g,j2g,j3g,s22c,js1,js2,js3)
   ! u2(j1g,j2g,j3g,s21c)=u2(j1g,j2g,j3g,s12c)
   u2(j1g,j2g,j3g,s21c)=extrapolate3(u2,j1g,j2g,j3g,s21c,js1,js2,js3)

 endIJLoops2d()
#endMacro


! =================================================================================
!   Apply limited extrapolation and then apply lower bounds
! =================================================================================
#beginMacro extrapAndBoundMacro()
 beginIJGhostLoops2d()
   ! NOTE: first extrap both lines before applying lower bounds

   ! first ghost line 
   i1g=i1-is1
   i2g=i2-is2
   i3g=i3-is3
   ! u1(i1-is1,i2-is2,i3,rc)=extrapolate3(u1,i1g,i2g,i3g,rc,is1,is2,is3)
   limitedRhoExtrapolation(u1, i1g,i2g,i3g,is1,is2,is3,rc, u1(i1+is1,i2+is2,i3+is3,rc))
   u1(i1g,i2g,i3g,uc)=extrapolate3(u1,i1g,i2g,i3g,uc,is1,is2,is3)
   u1(i1g,i2g,i3g,vc)=extrapolate3(u1,i1g,i2g,i3g,vc,is1,is2,is3)
   ! u1(i1g,i2g,i3g,pc)=extrapolate3(u1,i1g,i2g,i3g,pc,is1,is2,is3)
   limitedExtrapolation(u1, i1g,i2g,i3g,is1,is2,is3,pc)

   j1g=j1-js1
   j2g=j2-js2
   j3g=j3-js3
   u2(j1g,j2g,j3g,u1c)=extrapolate3(u2,j1g,j2g,j3g,u1c,js1,js2,js3)
   u2(j1g,j2g,j3g,u2c)=extrapolate3(u2,j1g,j2g,j3g,u2c,js1,js2,js3)

   u2(j1g,j2g,j3g,v1c)=extrapolate3(u2,j1g,j2g,j3g,v1c,js1,js2,js3)
   u2(j1g,j2g,j3g,v2c)=extrapolate3(u2,j1g,j2g,j3g,v2c,js1,js2,js3)

   u2(j1g,j2g,j3g,s11c)=extrapolate3(u2,j1g,j2g,j3g,s11c,js1,js2,js3)
   u2(j1g,j2g,j3g,s12c)=extrapolate3(u2,j1g,j2g,j3g,s12c,js1,js2,js3)
   u2(j1g,j2g,j3g,s22c)=extrapolate3(u2,j1g,j2g,j3g,s22c,js1,js2,js3)
   ! u2(j1g,j2g,j3g,s21c)=u2(j1g,j2g,j3g,s12c)
   u2(j1g,j2g,j3g,s21c)=extrapolate3(u2,j1g,j2g,j3g,s21c,js1,js2,js3)

   ! -- second ghost 
   i1g=i1-2*is1
   i2g=i2-2*is2
   i3g=i3-2*is3
   limitedRhoExtrapolation(u1, i1g,i2g,i3g,is1,is2,is3,rc, u1(i1+2*is1,i2+2*is2,i3+2*is3,rc))
   u1(i1g,i2g,i3g,uc)=extrapolate3(u1,i1g,i2g,i3g,uc,is1,is2,is3)
   u1(i1g,i2g,i3g,vc)=extrapolate3(u1,i1g,i2g,i3g,vc,is1,is2,is3)
   limitedExtrapolation(u1, i1g,i2g,i3g,is1,is2,is3,pc)

   j1g=j1-2*js1
   j2g=j2-2*js2
   j3g=j3-2*js3
   u2(j1g,j2g,j3g,u1c)=extrapolate3(u2,j1g,j2g,j3g,u1c,js1,js2,js3)
   u2(j1g,j2g,j3g,u2c)=extrapolate3(u2,j1g,j2g,j3g,u2c,js1,js2,js3)

   u2(j1g,j2g,j3g,v1c)=extrapolate3(u2,j1g,j2g,j3g,v1c,js1,js2,js3)
   u2(j1g,j2g,j3g,v2c)=extrapolate3(u2,j1g,j2g,j3g,v2c,js1,js2,js3)

   u2(j1g,j2g,j3g,s11c)=extrapolate3(u2,j1g,j2g,j3g,s11c,js1,js2,js3)
   u2(j1g,j2g,j3g,s12c)=extrapolate3(u2,j1g,j2g,j3g,s12c,js1,js2,js3)
   u2(j1g,j2g,j3g,s22c)=extrapolate3(u2,j1g,j2g,j3g,s22c,js1,js2,js3)
   ! u2(j1g,j2g,j3g,s21c)=u2(j1g,j2g,j3g,s12c)
   u2(j1g,j2g,j3g,s21c)=extrapolate3(u2,j1g,j2g,j3g,s21c,js1,js2,js3)

   ! apply lower bounds (note: do this after extrap of 2nd ghost)
   i1g=i1-is1
   i2g=i2-is2
   i3g=i3-is3
   applyLowerBound( u1(i1g,i2g,i3g,rc),rhoMin )
   applyLowerBound( u1(i1g,i2g,i3g,pc),pMin )

   i1g=i1-2*is1
   i2g=i2-2*is2
   i3g=i3-2*is3
   applyLowerBound( u1(i1g,i2g,i3g,rc),rhoMin )
   applyLowerBound( u1(i1g,i2g,i3g,pc),pMin )

   ! *wdh* TURN THESE OFF -- why are they here? (Thanks to DWS)
   ! j1g=j1-js1
   ! j2g=j2-js2
   ! j3g=j3-js3
   ! applyUpperBound( u2(j1g,j2g,j3g,s11c),pOffset )  ! FIX ME

   ! j1g=j1-2*js1
   ! j2g=j2-2*js2
   ! j3g=j3-2*js3
   ! applyUpperBound( u2(j1g,j2g,j3g,s11c),pOffset )  ! FIX ME


 endIJLoops2d()
#endMacro

! =================================================================================
!   Compute deformation gradient tensor
! =================================================================================
#beginMacro getDeformationGradients()
  ! compute p11tilde and p12tilde
 p11tilde=rsxy2(j1,j2,j3,axis2,0)*u2(j1,j2,j3,s11c)+rsxy2(j1,j2,j3,axis2,1)*u2(j1,j2,j3,s21c)
 p12tilde=rsxy2(j1,j2,j3,axis2,0)*u2(j1,j2,j3,s12c)+rsxy2(j1,j2,j3,axis2,1)*u2(j1,j2,j3,s22c)

 if( axis2.eq.0 )then
    ! determine components of the deformation gradient tensor (iterate on u1r,u2r so that these agree with P11tilde,P12tilde)
    u1s=(u2(j1,j2+1,j3,u1c)-u2(j1,j2-1,j3,u1c))/(2.0*dr2(1))
    u2s=(u2(j1,j2+1,j3,u2c)-u2(j1,j2-1,j3,u2c))/(2.0*dr2(1))
    ! initialize
    u1r0=js*(u2(j1+js1,j2,j3,u1c)-u2(j1,j2,j3,u1c))/dr2(0)
    u2r0=js*(u2(j1+js1,j2,j3,u2c)-u2(j1,j2,j3,u2c))/dr2(0)
    u1r=u1r0
    u2r=u2r0
 else
    ! determine components of the deformation gradient tensor (iterate on u1s,u2s so that these agree with P11tilde,P12tilde)
    u1r=(u2(j1+1,j2,j3,u1c)-u2(j1-1,j2,j3,u1c))/(2.0*dr2(0))
    u2r=(u2(j1+1,j2,j3,u2c)-u2(j1-1,j2,j3,u2c))/(2.0*dr2(0))
    ! initialize
    u1s0=js*(u2(j1,j2+js2,j3,u1c)-u2(j1,j2,j3,u1c))/dr2(1)
    u2s0=js*(u2(j1,j2+js2,j3,u2c)-u2(j1,j2,j3,u2c))/dr2(1)
    u1s=u1s0
    u2s=u2s0
 end if
 
 if (printStuff) then
    write(6,*)'p11tilde,p12tilde=',p11tilde,p12tilde
    write(6,*)'u1r,u2r,u1s,u2s=',u1r,u2r,u1s,u2s
 end if

 ! Newton iteration for u1r,u2r (axis2=0) or u1s,u2s (axis2=1)
 iter=1
 istop=0
 bmax=10.*toler
 do while (bmax.gt.toler)

    u1x=rsxy2(j1,j2,j3,0,0)*u1r+rsxy2(j1,j2,j3,1,0)*u1s
    u1y=rsxy2(j1,j2,j3,0,1)*u1r+rsxy2(j1,j2,j3,1,1)*u1s
    u2x=rsxy2(j1,j2,j3,0,0)*u2r+rsxy2(j1,j2,j3,1,0)*u2s
    u2y=rsxy2(j1,j2,j3,0,1)*u2r+rsxy2(j1,j2,j3,1,1)*u2s

    ! compute stress and the deriv based on current deformation gradient
    du(1,1)=u1x
    du(1,2)=u1y
    du(2,1)=u2x
    du(2,2)=u2y
    cpar(1)=lambda   ! Lame constants
    cpar(2)=mu
    ideriv=1         ! compute dpdf
    call smgetdp (du,p,dpdf,cpar,ideriv,itype)

    !  construct linear system
    b1=rsxy2(j1,j2,j3,axis2,0)*p(1,1)+rsxy2(j1,j2,j3,axis2,1)*p(2,1)-p11tilde
    b2=rsxy2(j1,j2,j3,axis2,0)*p(1,2)+rsxy2(j1,j2,j3,axis2,1)*p(2,2)-p12tilde
    a11= rsxy2(j1,j2,j3,axis2,0)*(dpdf(1,1)*rsxy2(j1,j2,j3,axis2,0)+dpdf(1,2)*rsxy2(j1,j2,j3,axis2,1)) \
        +rsxy2(j1,j2,j3,axis2,1)*(dpdf(3,1)*rsxy2(j1,j2,j3,axis2,0)+dpdf(3,2)*rsxy2(j1,j2,j3,axis2,1))
    a12= rsxy2(j1,j2,j3,axis2,0)*(dpdf(1,3)*rsxy2(j1,j2,j3,axis2,0)+dpdf(1,4)*rsxy2(j1,j2,j3,axis2,1)) \
        +rsxy2(j1,j2,j3,axis2,1)*(dpdf(3,3)*rsxy2(j1,j2,j3,axis2,0)+dpdf(3,4)*rsxy2(j1,j2,j3,axis2,1))
    a21= rsxy2(j1,j2,j3,axis2,0)*(dpdf(2,1)*rsxy2(j1,j2,j3,axis2,0)+dpdf(2,2)*rsxy2(j1,j2,j3,axis2,1)) \
        +rsxy2(j1,j2,j3,axis2,1)*(dpdf(4,1)*rsxy2(j1,j2,j3,axis2,0)+dpdf(4,2)*rsxy2(j1,j2,j3,axis2,1))
    a22= rsxy2(j1,j2,j3,axis2,0)*(dpdf(2,3)*rsxy2(j1,j2,j3,axis2,0)+dpdf(2,4)*rsxy2(j1,j2,j3,axis2,1)) \
        +rsxy2(j1,j2,j3,axis2,1)*(dpdf(4,3)*rsxy2(j1,j2,j3,axis2,0)+dpdf(4,4)*rsxy2(j1,j2,j3,axis2,1))

    ! solve the 2x2 system
    determ=a11*a22-a12*a21
    du1=(b1*a22-b2*a12)/determ
    du2=(b2*a11-b1*a21)/determ

    ! compute max residual of the stress condition and update
    bmax=max(abs(b1),abs(b2))/lambda

    if (printStuff) then
       write(6,*)'iter,determ,du1,du2,bmax=',iter,determ,du1,du2,bmax
    end if

    if (istop.ne.0) then
       ! write(6,*)p(1,1),p(1,2),p(2,1),p(2,2)
       ! write(6,*)a11,a12,a21,a22,b1,b2
       write(6,'(1x,i2,3(1x,1pe15.8))')iter,du1,du2,bmax
       ! write(6,*)axis2
    end if

    iter=iter+1
    if( axis2.eq.0 )then
       u1r=u1r-du1
       u2r=u2r-du2
       ! check for convergence
       if (iter.gt.itmax) then
          write(6,*)'Error (interface code) : Newton failed to converge'
          if (istop.eq.0) then
             iter=1
             istop=1
             u1r=u1r0
             u2r=u2r0
          else
             stop 8885
          end if
       end if
    else
       u1s=u1s-du1
       u2s=u2s-du2
       ! check for convergence
       if (iter.gt.itmax) then
          write(6,*)'Error (interface code) : Newton failed to converge'
          if (istop.eq.0) then
             iter=1
             istop=1
             u1s=u1s0
             u2s=u2s0
          else
             stop 8895
          end if
       end if
    end if

 end do

 if (printStuff) then
    write(6,*)'u1s,u2s=',u1s,u2s
    write(6,*)'u1r,u2r=',u1r,u2r
    fact=rsxy2(j1,j2,j3,axis2,0)**2+rsxy2(j1,j2,j3,axis2,1)**2
    !   a11=a11/fact
    !   a12=a12/fact
    !   a21=a21/fact
    !   a22=a22/fact
    write(6,*)'a11,a12,a21,a22=',a11,a12,a21,a22
    write(6,*)'lambda,mu=',lambda,mu
    ! pause
 end if
#endMacro

#beginMacro smoothDir1(u,i1,i2,i3,c)
  u(i1,i2,i3,c)= u(i1,i2,i3,c) + omega*( u(i1+1,i2,i3,c)-2.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c))
#endMacro

#beginMacro smoothDir2(u,i1,i2,i3,c)
  u(i1,i2,i3,c)= u(i1,i2,i3,c) + omega*( u(i1,i2+1,i3,c)-2.*u(i1,i2,i3,c)+u(i1,i2-1,i3,c))
#endMacro

#beginMacro periodicUpdate1(u,i1,i2,i3,c, n1a,n1b,n2a,n2b,n3a,n3b)
  u(n1a-2,n2a,n3a,c)= u(n1b-2,n2a,n3a,c)
  u(n1a-1,n2a,n3a,c)= u(n1b-1,n2a,n3a,c)
  u(n1b  ,n2a,n3a,c)= u(n1a  ,n2a,n3a,c)
  u(n1b+1,n2a,n3a,c)= u(n1a+1,n2a,n3a,c)
  u(n1b+2,n2a,n3a,c)= u(n1a+2,n2a,n3a,c)
#endMacro

#beginMacro periodicUpdate2(u,i1,i2,i3,c, n1a,n1b,n2a,n2b,n3a,n3b)
  u(n1a,n2a-1,n3a,c)= u(n1a,n2b-1,n3a,c)
  u(n1a,n2b  ,n3a,c)= u(n1a,n2a  ,n3a,c)
  u(n1a,n2b+1,n3a,c)= u(n1a,n2a+1,n3a,c)
#endMacro

! ================================================================================
!   Update periodic boundaries on the interface
! ================================================================================
#beginMacro periodicUpdateInterface()
  ! periodic update 
  if( boundaryCondition1(0,axis1p1).lt.0 )then
    ! write(*,'(" interfaceCnsSM: periodic update u1..")')
    if( axis1p1.eq.0 )then
      periodicUpdate1(u1,i1,i2,i3,rc, n1a,n1b,n2a,n2b,n3a,n3b)
      periodicUpdate1(u1,i1,i2,i3,uc, n1a,n1b,n2a,n2b,n3a,n3b)
      periodicUpdate1(u1,i1,i2,i3,vc, n1a,n1b,n2a,n2b,n3a,n3b)
      periodicUpdate1(u1,i1,i2,i3,tc, n1a,n1b,n2a,n2b,n3a,n3b)
    else
      periodicUpdate2(u1,i1,i2,i3,rc, n1a,n1b,n2a,n2b,n3a,n3b)
      periodicUpdate2(u1,i1,i2,i3,uc, n1a,n1b,n2a,n2b,n3a,n3b)
      periodicUpdate2(u1,i1,i2,i3,vc, n1a,n1b,n2a,n2b,n3a,n3b)
      periodicUpdate2(u1,i1,i2,i3,tc, n1a,n1b,n2a,n2b,n3a,n3b)
    end if
  end if

  if( boundaryCondition2(0,axis2p1).lt.0 )then
    ! write(*,'(" interfaceCnsSM: periodic update u2..")')
    if( axis2p1.eq.0 )then
      periodicUpdate1(u2,j1,j2,j3,u1c, m1a,m1b,m2a,m2b,m3a,m3b)
      periodicUpdate1(u2,j1,j2,j3,u2c, m1a,m1b,m2a,m2b,m3a,m3b)
      periodicUpdate1(u2,j1,j2,j3,v1c, m1a,m1b,m2a,m2b,m3a,m3b)
      periodicUpdate1(u2,j1,j2,j3,v2c, m1a,m1b,m2a,m2b,m3a,m3b)
    else
      periodicUpdate2(u2,j1,j2,j3,u1c, m1a,m1b,m2a,m2b,m3a,m3b)
      periodicUpdate2(u2,j1,j2,j3,u2c, m1a,m1b,m2a,m2b,m3a,m3b)
      periodicUpdate2(u2,j1,j2,j3,v1c, m1a,m1b,m2a,m2b,m3a,m3b)
      periodicUpdate2(u2,j1,j2,j3,v2c, m1a,m1b,m2a,m2b,m3a,m3b)

    end if
  end if
#endMacro

! ================================================================================
!   Smooth values on the interface
! 
! ================================================================================
#beginMacro smoothInterfaceValues()

if( .false. )then
  ! *** THIS IS UNDER CONSTRUCTION ***
  write(*,'(" interfaceCnsSM: smooth interface values before project")')
  write(*,'(" interfaceCnsSM: axis1=",i2," axis2=",i2)') axis1,axis2
  write(*,'(" interfaceCnsSM: axis1p1=",i2," axis2p1=",i2)') axis1p1,axis2p1
  write(*,'(" interfaceCnsSM: n1a,n1b=",2i4," n2a,n2b=",2i4)') n1a,n1b,n2a,n2b
  write(*,'(" interfaceCnsSM: m1a,m1b=",2i4," m2a,m2b=",2i4)') m1a,m1b,m2a,m2b

  ! periodic update  **THIS IS NEEDED** 
  periodicUpdateInterface()
 
  beginIJLoops2d()

   omega=.25/4. ! for 2nd order
   if( axis1.eq.0 )then
     smoothDir2(u1,i1,i2,i3,rc)
     smoothDir2(u1,i1,i2,i3,uc)
     smoothDir2(u1,i1,i2,i3,vc)
     smoothDir2(u1,i1,i2,i3,tc)
   else
     smoothDir1(u1,i1,i2,i3,rc)
     smoothDir1(u1,i1,i2,i3,uc)
     smoothDir1(u1,i1,i2,i3,vc)
     smoothDir1(u1,i1,i2,i3,tc)
   end if

   ! omega=0.
   if( axis2.eq.0 )then
     smoothDir2(u2,j1,j2,j3,u1c)
     smoothDir2(u2,j1,j2,j3,u2c)
     smoothDir2(u2,j1,j2,j3,v1c)
     smoothDir2(u2,j1,j2,j3,v1c)
   else
     smoothDir1(u2,j1,j2,j3,u1c)
     smoothDir1(u2,j1,j2,j3,u2c)
     smoothDir1(u2,j1,j2,j3,v1c)
     smoothDir1(u2,j1,j2,j3,v1c)

   end if

  endIJLoops2d()

  ! periodic update 
  periodicUpdateInterface()

end if

#endMacro

! =================================================================================
!   Compute eigen-structure of the T matrix (see Riemann problem notes)
! =================================================================================
#beginMacro checkEigenStructure()

 printChecks=.true.

! compute the solid normal (n1s,n2s)
 getSolidNormal()

! solid tangent
 t1s= n2s
 t2s=-n1s

! compute bk(j,k)=ni*Kijkl*nl for j=1,2 and k=1,2
 bk(1,1)=n1s*(dpdf(1,1)*n1s+dpdf(1,2)*n2s)+n2s*(dpdf(3,1)*n1s+dpdf(3,2)*n2s)
 bk(1,2)=n1s*(dpdf(1,3)*n1s+dpdf(1,4)*n2s)+n2s*(dpdf(3,3)*n1s+dpdf(3,4)*n2s)
 bk(2,1)=n1s*(dpdf(2,1)*n1s+dpdf(2,2)*n2s)+n2s*(dpdf(4,1)*n1s+dpdf(4,2)*n2s)
 bk(2,2)=n1s*(dpdf(2,3)*n1s+dpdf(2,4)*n2s)+n2s*(dpdf(4,3)*n1s+dpdf(4,4)*n2s)

! compute tk(1,1)=ni*bk(i,j)*nj
 tk(1,1)=n1s*(bk(1,1)*n1s+bk(1,2)*n2s)+n2s*(bk(2,1)*n1s+bk(2,2)*n2s)

! compute tk(1,2)=ni*bk(i,j)*tj
 tk(1,2)=n1s*(bk(1,1)*t1s+bk(1,2)*t2s)+n2s*(bk(2,1)*t1s+bk(2,2)*t2s)

! compute tk(2,1)=ti*bk(i,j)*nj
 tk(2,1)=t1s*(bk(1,1)*n1s+bk(1,2)*n2s)+t2s*(bk(2,1)*n1s+bk(2,2)*n2s)

! compute tk(2,2)=ti*bk(i,j)*tj
 tk(2,2)=t1s*(bk(1,1)*t1s+bk(1,2)*t2s)+t2s*(bk(2,1)*t1s+bk(2,2)*t2s)

! eigenvalues and eigenvectors of tk
 trace=(tk(1,1)+tk(2,2))/2.0
 determ=tk(1,1)*tk(2,2)-tk(1,2)*tk(2,1)
 discriminant=trace**2-determ
 if( discriminant.le.0.0 )then
   write(6,*)'Error (interface code) : eigenvalues are complex'
   stop 8886
 end if
 eval(1)=trace+sqrt(discriminant)
 eval(2)=trace-sqrt(discriminant)
 if( eval(2).le.0.0 )then
   write(6,*)'Error (interface code) : eigenvalues are imaginary'
   stop 8887
 end if
 do k=1,2
   if( abs(eval(k)-tk(1,1)).gt.abs(eval(k)-tk(2,2)) )then
     evec(k,1)=tk(1,2)/(eval(k)-tk(1,1))
     evec(k,2)=1.0
   else
     evec(k,1)=1.0
     evec(k,2)=tk(2,1)/(eval(k)-tk(2,2))
   end if
   rad=sqrt(evec(k,1)**2+evec(k,2)**2)
   evec(k,1)=evec(k,1)/rad
   evec(k,2)=evec(k,2)/rad
 end do

! compute the fluid normal (n1f,n2f)
 getFluidNormal()

! fluid tangent
 t1f= n2f
 t2f=-n1f

! compute a and b vectors (see notes)
 av(1)=n1f*n1s+n2f*n2s
 av(2)=n1f*t1s+n2f*t2s
 bv(1)=t1f*n1s+t2f*n2s
 bv(2)=t1f*t1s+t2f*t2s

! checks
 chk(1)=abs(tk(1,2)-tk(2,1))                  ! symmetry of T
 chk(2)=abs(av(1)*evec(2,1)+av(2)*evec(2,2))  ! a^T rs = 0
 chk(3)=abs(bv(1)*evec(1,1)+bv(2)*evec(1,2))  ! b^T rp = 0

! print checks
 if (printChecks) then
   write(89,'(2(1x,i4),3(1x,1pe10.3))')j1,j2,chk(1),chk(2),chk(3)
 end if

#endMacro


! =================================================================================
!   Compute solid impedances
! =================================================================================
#beginMacro getSolidImpedances()

! compute the solid normal (n1s,n2s)
 getSolidNormal()

! compute the fluid normal (n1f,n2f)
 getFluidNormal()

! may want to use all solid normals to compare with linear elasticity
 if (useAllSolidNormals) then
   n1f=n1s
   n2f=n2s
 end if

! fluid tangent
 t1f= n2f
 t2f=-n1f

! check value of "meth"
 if (meth.eq.0) then

! impedances based on linear model
   cp=sqrt((lambda+2.*mu)/rhos)
   zs=rhos*cp

! no tangent stress contribution
   ks=0.0

 else

! impedances based on full model (TZ must be "off" so that dpdf is available)
   if (twilightZone.ne.0) then
     write(6,*)'Error (interfaceCnsSm) : meth.ne.0 and TZ is on'
     stop 3580
   end if

! compute bk(j,k)=nis*Kijkl*nls for j=1,2 and k=1,2  (Note: bk is symmetric, could just set bk(2,1)=bk(1,2).)
   bk(1,1)=n1s*(dpdf(1,1)*n1s+dpdf(1,2)*n2s)+n2s*(dpdf(3,1)*n1s+dpdf(3,2)*n2s)
   bk(1,2)=n1s*(dpdf(1,3)*n1s+dpdf(1,4)*n2s)+n2s*(dpdf(3,3)*n1s+dpdf(3,4)*n2s)
   bk(2,1)=n1s*(dpdf(2,1)*n1s+dpdf(2,2)*n2s)+n2s*(dpdf(4,1)*n1s+dpdf(4,2)*n2s)
   bk(2,2)=n1s*(dpdf(2,3)*n1s+dpdf(2,4)*n2s)+n2s*(dpdf(4,3)*n1s+dpdf(4,4)*n2s)

! compute tk(1,1)=nif*bk(i,j)*njf
   tk(1,1)=n1f*(bk(1,1)*n1f+bk(1,2)*n2f)+n2f*(bk(2,1)*n1f+bk(2,2)*n2f)

! compute tk(1,2)=nif*bk(i,j)*tjf
   tk(1,2)=n1f*(bk(1,1)*t1f+bk(1,2)*t2f)+n2f*(bk(2,1)*t1f+bk(2,2)*t2f)

! compute tk(2,1)=tif*bk(i,j)*njf  (Note: tk is symmetric, could just set tk(2,1)=tk(1,2).)
   tk(2,1)=t1f*(bk(1,1)*n1f+bk(1,2)*n2f)+t2f*(bk(2,1)*n1f+bk(2,2)*n2f)

! compute tk(2,2)=tif*bk(i,j)*tjf
   tk(2,2)=t1f*(bk(1,1)*t1f+bk(1,2)*t2f)+t2f*(bk(2,1)*t1f+bk(2,2)*t2f)

! eigenvalues of tk
   trace=(tk(1,1)+tk(2,2))/2.0
   discriminant=((tk(1,1)-tk(2,2))/2.0)**2+tk(1,2)**2
   eval(1)=trace+sqrt(discriminant)
   eval(2)=trace-sqrt(discriminant)
   if( eval(2).le.0.0 )then
     write(6,*)'Error (interface code) : eigenvalues are imaginary'
     stop 8887
   end if

!   write(33,'(2(1x,1pe15.8))')eval(1),tk(2,2)

! left eigenvector corresponding to dominant eigenvalue.  There is a basic assumption
! here that tk is close to the matrix [lambda+2*mu, 0; 0, mu]
   evec(1,2)=tk(1,2)/(eval(1)-tk(2,2))
   rad=sqrt(evec(1,2)**2+1.0)
   evec(1,1)=1.0/rad
   evec(1,2)=evec(1,2)/rad

! Nanson's formula
   beta=(n1f*(f11s*n1s+f12s*n2s)+n2f*(f21s*n1s+f22s*n2s))/aj

! zps and zss
   zps=beta*sqrt(rhos*eval(1))
   zss=beta*sqrt(rhos*eval(2))

! zs and ks
   zs=zps*zss/(zss*evec(1,1)**2+zps*evec(1,2)**2)
   ks=evec(1,1)*evec(1,2)*(zps-zss)/(zss*evec(1,1)**2+zps*evec(1,2)**2)

!   write(33,'(3(1x,1pe15.8))')beta,zs,ks

 end if

#endMacro


! ================================================================================
! Project values on the fluid-solid interface for linear elasticity
! 
! ================================================================================
#beginMacro projectInterfaceValuesLinearElasticity()

 ! if( conservativeVariables.ne.1 )then
 !   stop 227745
 ! end if
 beginIJLoops2d()
   ! if( .false. .or. (i1.gt.n1a .and. i1.lt.n1b) )then  ! ********** TEST **************
   ! if( i1.gt.n1a )then  ! ********** TEST **************
 if( .true. )then

    ! if( i1.eq.0 )then ! ********** TEST ***********
    !   u1(i1,i2,i3,vc)=0.
    !   u2(j1,j2,j3,u2c)=0.
    !   u2(j1,j2,j3,v2c)=0.
    !   u2(j1,j2,j3,s22c)=0.
    ! end if


    ! fluid: 
    if( conservativeVariables.eq.1 )then
       rhof= u1(i1,i2,i3,rc)
       v1f = u1(i1,i2,i3,uc)/rhof
       v2f = u1(i1,i2,i3,vc)/rhof
       ef  = u1(i1,i2,i3,tc) ! in conservative vars this is E = p/(gamma-1) + .5*rho*v^2 
       pf = (gamma-1.)*( ef-.5*rhof*(v1f**2+v2f**2) )    ! p 
    else
       ! input vars are (rho,u,v,w,T)
       rhof= u1(i1,i2,i3,rc)
       v1f = u1(i1,i2,i3,uc)
       v2f = u1(i1,i2,i3,vc)
       pf = rhof*u1(i1,i2,i3,tc)  ! p=rho*T

    endif

    applyLowerBound( rhof,rhoMin )
    applyLowerBound( pf,pMin )

    pf0 = -(pf-pOffset)                               ! traction = pf0*nf

    af = sqrt(gamma*pf/rhof)
    zf = rhof*af   ! fluid impedance 

    ! NOTE:
    ! -- the fluid normal is flipped to point into the fluid domain 
    ! -- the solid normal points out of the solid domain (in the same general direction as nf)
    !
    !          ----solid-----------I------fluid-------------
    !                            n -> 
    ! 
    !          ----fluid ----------I------solid ------------
    !                            <- n 
    !
    ! NOTE: the direction of the normal, n, determines uLeft and uRight. If n goes from solid to fluid then
    !   uLeft=solid and uRight=fluid

    ! compute the fluid normal (n1f,n2f)
    getFluidNormal()
    ! vf : normal component of the fluid velocity
    vf = n1f*v1f + n2f*v2f ! 

    ! call ogDeriv(ep,0,0,0,0,xy(i1-2*is1,i2-2*is2,i3,0),xy(i1-2*is1,i2-2*is2,i3,1),0.,t,n,uem)


    ! solid:
    v1s = u2(j1,j2,j3,v1c)
    v2s = u2(j1,j2,j3,v2c)
    s11s =u2(j1,j2,j3,s11c)
    s12s =u2(j1,j2,j3,s12c)
    s22s =u2(j1,j2,j3,s22c)

    ! compute the solid normal (n1s,n2s)
    getSolidNormal()

    ! vs : normal component of the solid velocity 
    ! vs = n1s*v1s + n2s*v2s   
    ! We follow what was done in the standard VS/SF algorithm in which case we use the
    ! fluid normal to determine the normal component of the velocity
    ! use fluid normal here
    vs = n1f*v1s + n2f*v2s   ! normal component of the solid velocity 

    ! solid traction is ns.sigmas:
    traction1 = n1s*s11s + n2s*s12s  
    traction2 = n1s*s12s + n2s*s22s

    ! ps is the normal component of the solid traction
    ! ps = n1s*traction1+n2s*traction2  ! ps = n.sigma.n 
    ! use fluid normal here
    ps = n1f*traction1+n2f*traction2  ! ps = n.sigma.n 

    if( twilightZone.ne.0 )then
       ! -- compute the exact solution for TZ --
       ! write(*,'(" >>>>>>>>>>>> interfaceCnsSm: twilightZone is ON <<<<<<<<<<<<<<<<")') 

       call ogDeriv(ep1,0,0,0,0,xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t,rc,rhoe)
       call ogDeriv(ep1,0,0,0,0,xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t,uc,v1fe)
       call ogDeriv(ep1,0,0,0,0,xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t,vc,v2fe)
       call ogDeriv(ep1,0,0,0,0,xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t,tc,tfe)
       pfe = rhoe*tfe

       call ogDeriv(ep2,0,0,0,0,xy2(j1,j2,j3,0),xy2(j1,j2,j3,1),0.,t,v1c,v1se)
       call ogDeriv(ep2,0,0,0,0,xy2(j1,j2,j3,0),xy2(j1,j2,j3,1),0.,t,v2c,v2se)
       call ogDeriv(ep2,0,0,0,0,xy2(j1,j2,j3,0),xy2(j1,j2,j3,1),0.,t,s11c,s11se)
       call ogDeriv(ep2,0,0,0,0,xy2(j1,j2,j3,0),xy2(j1,j2,j3,1),0.,t,s12c,s12se)
       call ogDeriv(ep2,0,0,0,0,xy2(j1,j2,j3,0),xy2(j1,j2,j3,1),0.,t,s21c,s21se)
       call ogDeriv(ep2,0,0,0,0,xy2(j1,j2,j3,0),xy2(j1,j2,j3,1),0.,t,s22c,s22se)

       ! if( twilightZone.ne.0 )then ! ********** TEST ***********
       !  write(*,'("Start: rhof,v1f,v2f,pf = ",4e10.2)') rhof,v1f,v2f,pf
       !  write(*,'("Start: pf0,pfe, pOffset = ",3e10.2)') pf0,pfe,pOffset
       ! end if

       ! With TZ we compute the impedance weighted average to 
       !         [v] = [v_exact]
       !         [sigma] = [sigma_exact]
       vf = vf - (n1f*v1fe + n2f*v2fe)  ! subtract off exact 
       pf0 = pf0 - (-pfe+pOffset)        ! subtract off exact 
       vs = vs - (n1f*v1se + n2f*v2se)  ! subtract off exact 

       traction1e = n1s*s11se + n2s*s12se  
       traction2e = n1s*s12se + n2s*s22se
       ps = ps - (n1f*traction1e+n2f*traction2e)  ! subtract off exact 

    end if

    ! ------------------------------------
    ! --- project the interface values ---
    ! ------------------------------------

    ! impedance weighted averages: 
    wf = zf
    ws = zs

    vi = (wf*vf  + ws*vs + pf0-ps          )/( ws+wf )  ! interface velocity 
    pi = (ws*pf0 + wf*ps + ws*wf*( vf-vs ) )/( ws+wf )  ! interface "pressure" n.s.n 

    ! vi = (wf*vf  + ws*vs                  )/( ws+wf )  ! interface velocity 
    ! pi = (ws*pf0 + wf*ps                  )/( ws+wf )  ! interface "pressure" n.s.n 

    ! vi = (eps2*vf  + eps1*vs)/( eps1+eps2 )  ! interface velocity 
    ! pi = (eps1*pf0 + eps2*ps)/( eps1+eps2 )  ! interface "pressure" n.s.n 


    ! if( twilightZone.ne.0 )then ! ********** TEST ***********
    !  write(*,'(" vf,vs,vi = ",3e10.2)') vf,vs,vi
    !  write(*,'(" pf0,ps,pi = ",3e10.2)') pf0,ps,pi
    ! end if

    ! if( i1.eq.0 )then ! ********** TEST ***********
    !   vi=0.
    !   pi=pf0
    ! end if

    ! if( .false. )then ! *************** TEST ***********
    !  vi = (wf*vf  + ws*vs )/( ws+wf )  
    !  pi = (ws*pf0 + wf*ps )/( ws+wf ) 
    ! end if
    ! if( .false. )then ! *************** TEST ***********
    !  vi = (wf*vf  + ws*vs )/( ws+wf )  
    !  pi = pf0
    ! end if
    ! if( .false. )then ! *************** TEST ***********
    !   vi = vs
    !   pi = pf0
    ! end if

    if( .false. )then
       write(*,'(" IP:fluid: (i1,i2)=(",i3,i3,") pf=",e10.3," pOffset=",e10.3)') i1,i2,pf,pOffset
       write(*,'(" IP:fluid: (i1,i2)=(",i3,i3,") n=(",2f6.3,") (rhof,v1f,v2f,-pf)=(",4e9.2,") zf=",e8.2," af=",e8.2)') i1,i2,n1f,n2f,rhof,v1f,v2f,-pf+pOffset,zf,af

       write(*,'(" IP:solid: (j1,j2)=(",i3,i3,") n=(",2f6.3,") (rhos,v1s,v2s, ps)=(",4e9.2,") zs=",e9.3," as=",e8.2," (u1,u2)=",2e9.2,")")') j1,j2,n1s,n2s,rhos,v1s,v2s,ps,zs,cp,u2(j1,j2,j3,u1c),u2(j1,j2,j3,u2c)
       write(*,'(" IP:solid: (j1,j2)=(",i3,i3,") (s11,s12,s22)=(",3e9.2,")")') j1,j2,s11s,s12s,s22s
       write(*,'(" IP: -> vi=",e16.7," pi=",e16.7)') vi,pi


       write(*,'(" IP: (vf,vs)=",2e10.2," -> vi=",e16.7," (-pf,ps)=",2e10.2," pi=",e16.7)') vf,vs,vi, -pf+pOffset,ps,pi
       write(1,'(" IP: (vf,vs)=",2e10.2," -> vi=",e16.7," (-pf,ps)=",2e10.2," pi=",e16.7)') vf,vs,vi, -pf+pOffset,ps,pi

    end if


    ! Here is the solution to the nonlinear FSR
    if( interfaceOption.eq.5 )then
       ! -- this won't work with twilightZone !
       solid(0)=rhos
       solid(1)=vs
       solid(2)=ps
       solid(3)=cp
       fluid(0)=rhof
       fluid(1)=vf  
       fluid(2)=pf  
       fluid(3)=gamma
       fluid(4)=pOffset
       call fluidSolidRiemannSolution( solid, fluid, fsr )
       ! write(*,'(" nonlinear-FSR: rhoi,vi,pi=",3e13.5)') fsr(0),fsr(1),-fsr(2)+pOffset
       rhofi = fsr(0)
       vi    = fsr(1)
       pi    = -fsr(2)+pOffset
    end if

    ! -- here is the projection --
    !  Set normal component of fluid velocity to be vi:
    v1f = v1f + (vi-vf)*n1f
    v2f = v2f + (vi-vf)*n2f

    ! ***********
    ! if( .false. )then
    !   v1f=vi
    !   v2f=0.
    ! end if

    !  Adjust rhof using: Entropy const : p/rho^gamma = K

    pif = -pi+pOffset  ! new fluid pressure
    if( twilightZone.ne.0 )then
       ! For TZ re-adjust the full fluid pressure
       pif = pif + pfe 
    end if

    if( .true. )then 
       applyLowerBound( pif,pMin )
       pi = -pif+pOffset  ! adjust interface stress

       if( interfaceOption.eq.1 )then
          rhofi = rhof*(pif/pf)**(1./gamma) !  choose interface rho from S=const
       end if
       applyLowerBound( rhofi,rhoMin )

    end if

    ! write(*,'("After project pif,pfe = ",2e10.2)') pif,pfe
    ! write(*,'("After project rhofi,rhoe = ",2e10.2)') rhofi,rhoe

    u1(i1,i2,i3,rc) = rhofi

    if( conservativeVariables.eq.1 )then
       u1(i1,i2,i3,uc) = v1f*rhofi
       u1(i1,i2,i3,vc) = v2f*rhofi
       eif = pif/(gamma-1.)+.5*rhofi*(v1f**2+v2f**2)
       u1(i1,i2,i3,tc)=eif
    else
       u1(i1,i2,i3,uc) = v1f
       u1(i1,i2,i3,vc) = v2f
       u1(i1,i2,i3,tc)=pif/rhofi
    end if

    !  Set normal component of solid velocity to be vi:
    ! *** which normals should we use here ??
    ! u2(j1,j2,j3,v1c) = u2(j1,j2,j3,v1c) + (vi-vs)*n1s
    ! u2(j1,j2,j3,v2c) = u2(j1,j2,j3,v2c) + (vi-vs)*n2s
    ! Use fluid normal since vs = nf.vvs
    u2(j1,j2,j3,v1c) = u2(j1,j2,j3,v1c) + (vi-vs)*n1f
    u2(j1,j2,j3,v2c) = u2(j1,j2,j3,v2c) + (vi-vs)*n2f

    !  Assign the stress in the solid: 
    !       sigmas.ns = g = pi nf 
    ! We follow the projection that would have been used in the standard algorithm
    ! In this case the traction vector pi*nf would be passed to the solid mechanics code
    ! where the solid normal and tangent would then be used to set the components of the stress.
    ! 
    !  In 2d we use the 3 equations:
    !      ns.s.ns = ns.g = f11 = pi ns.nf 
    !      ts.s.ns = tf.g = f12 = pi ts.nf
    !      ts.s.ts = ts.s(old).ts = f22  (i.e. do not change this component)
    if( .true. )then
       ! use a mix of solid and fluid normals
       t1s=-n2s !  solid tangent 
       t2s= n1s 
       f11 = pi *( n1s*n1f + n2s*n2f )
       f12 = pi *( t1s*n1f + t2s*n2f )
       f22=t1s*t1s*s11s + 2.*t1s*t2s*s12s + t2s*t2s*s22s 
       u2(j1,j2,j3,s11c) = n1s*n1s*f11 + 2.*n1s*t1s      *f12 + t1s*t1s*f22
       u2(j1,j2,j3,s12c) = n1s*n2s*f11 +(n1s*t2s+n2s*t1s)*f12 + t1s*t2s*f22
       u2(j1,j2,j3,s22c) = n2s*n2s*f11 + 2.*n2s*t2s      *f12 + t2s*t2s*f22

       u2(j1,j2,j3,s21c) =   u2(j1,j2,j3,s12c)
    else 
       ! use fluid normal --- what about the tangent ?? ********** FIX
       t1s=-n2f !  solid tangent 
       t2s= n1f 
       f11 = pi
       f12=0.
       f22=t1s*t1s*s11s + 2.*t1s*t2s*s12s + t2s*t2s*s22s 
       u2(j1,j2,j3,s11c) = n1f*n1f*f11 + 2.*n1f*t1s      *f12 + t1s*t1s*f22
       u2(j1,j2,j3,s12c) = n1f*n2f*f11 +(n1f*t2s+n2f*t1s)*f12 + t1s*t2s*f22
       u2(j1,j2,j3,s22c) = n2f*n2f*f11 + 2.*n2f*t2s      *f12 + t2s*t2s*f22

       u2(j1,j2,j3,s21c) =   u2(j1,j2,j3,s12c)
    end if


    if( twilightZone.ne.0 )then
       ! TZ corrections: 

       ! *wdh* 2013/07/31 this is not needed: 
       ! u2(j1,j2,j3,v1c) = u2(j1,j2,j3,v1c) + v1se
       ! u2(j1,j2,j3,v2c) = u2(j1,j2,j3,v2c) + v2se

       u2(j1,j2,j3,s11c) = u2(j1,j2,j3,s11c) + s11se
       u2(j1,j2,j3,s12c) = u2(j1,j2,j3,s12c) + s12se
       u2(j1,j2,j3,s21c) = u2(j1,j2,j3,s21c) + s21se
       u2(j1,j2,j3,s22c) = u2(j1,j2,j3,s22c) + s22se

       ! Do this for testing:  
       ! rhofi=rhoe
       ! v1f=v1fe
       ! v2f=v2fe
       ! pif=rhoe*tfe

       ! write(*,'(" IP: conservativeVariables=",i2)') conservativeVariables
       ! write(*,'(" IP: (i1,i2)=(",i3,i3,") Fluid: u,ue=",2e10.3," v,ve=",2e10.3," p,pe=",2e10.3)') i1,i2,u1(i1,i2,i3,uc)/rhofi,v1fe,u1(i1,i2,i3,vc)/rhofi,v2fe,pif,pfe

       if( conservativeVariables.eq.1 )then
          u1(i1,i2,i3,rc) = rhofi
          u1(i1,i2,i3,uc) = v1f*rhofi
          u1(i1,i2,i3,vc) = v2f*rhofi
          eif = pif/(gamma-1.)+.5*rhofi*(v1f**2+v2f**2)
          u1(i1,i2,i3,tc)=eif
       else
          u1(i1,i2,i3,uc) = v1f
          u1(i1,i2,i3,vc) = v2f
          u1(i1,i2,i3,tc)=pif/rhofi
       end if

       ! *** THIS IS WRONG: *wdh* 2013/05/03 -- fix me  .. fixed 2012/06/08
       !   u2(j1,j2,j3,v1c)=v1se
       !   u2(j1,j2,j3,v2c)=v2se
       !   u2(j1,j2,j3,s11c)=s11se
       !   u2(j1,j2,j3,s12c)=s12se
       !   u2(j1,j2,j3,s21c)=s21se
       !   u2(j1,j2,j3,s22c)=s22se

    end if



 end if
 endIJLoops2d()

#endMacro


! ================================================================================
! Project values on the fluid-solid interface for NON-linear elasticity
! 
! This is the new code that assumes a general solid mechanics model (such as SVK)
! ================================================================================
#beginMacro projectInterfaceValuesNonLinearElasticity()

 do k=1,4
    err1(k)=0.
 end do
 do k=1,6
    err2(k)=0.
 end do

 useAllSolidNormals=.false.
 compareWithExact=.false.
 compareWithBill=.false.
 printErrorsAfterTime=0.3
 ! itype=1          ! linear elasticity
 ! itype=2          ! SVK
 ! itype=4          ! Neo-Hookean
 itype=pdeTypeForGodunovMethodCgsm

 nonlinearProjectionMethod=0
 if (interfaceOption.eq.5) then
   nonlinearProjectionMethod=1
   if( t.lt.2*dt )then
     write(*,'(" USE full nonlinear ProjectionMethod")') 
   end if
 end if
 meth=nonlinearProjectionMethod
 ! write(6,*)'meth =',meth
 ! pause
 ! meth=0  =>  linear impedances, no tangential stress contribution
 ! meth=1  =>  full impedances with tangential stress contribution

 if (meth.ne.0.and.twilightZone.ne.0) then
   write(6,*)'Error (interfaceCnsSm) : meth.ne.0 and TZ is on'
   stop 3579
 end if


!   write(6,*)'TZ=',twilightZone
!   pause
!   write(6,*)'pOffset=',pOffset
!   pause
!   write(6,*)'type =',pdeTypeForGodunovMethodCgsm
!   pause

!   bmax=0.d0
!   do k=0,7
!     bmax=max(abs(u2(0,0,0,k)-u2(251,0,0,k)),bmax)
!     bmax=max(abs(u2(-1,0,0,k)-u2(250,0,0,k)),bmax)
!     bmax=max(abs(u2(1,0,0,k)-u2(252,0,0,k)),bmax)
!   end do
!   if (bmax.gt.1.e-8) then
!     write(6,*)v1c,v2c,s11c,s12c,s21c,s22c,u1c,u2c
!     do k=0,7
!       write(6,'(1x,i2,2(1x,1pe15.8))')k,u2(0,0,0,k),u2(251,0,0,k)-u2(0,0,0,k)
!     end do
!     do j1=0,251
!       write(6,'(1x,i3,1x,1pe15.8)')j1,u2(j1,0,0,0)
!     end do
!     pause
!   end if

 beginIJLoops2d()

 if (compareWithExact) then

    if (twilightZone.eq.0) then
       write(6,*)'Error (interfaceCnsSm) : compareWithExact is true, but TZ is off'
       stop 1234
    end if

    ! Exact fluid variables on the boundary
    call ogDeriv(ep1,0,0,0,0,xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t,rc,rhoe)
    call ogDeriv(ep1,0,0,0,0,xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t,uc,v1fe)
    call ogDeriv(ep1,0,0,0,0,xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t,vc,v2fe)
    call ogDeriv(ep1,0,0,0,0,xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t,tc,tfe)
    if( conservativeVariables.eq.1 )then
       u1(i1,i2,i3,rc)=rhoe
       u1(i1,i2,i3,uc)=rhoe*v1fe
       u1(i1,i2,i3,vc)=rhoe*v2fe
       u1(i1,i2,i3,tc)=rhoe*tfe/(gamma-1)+.5*rhoe*(v1fe**2+v2fe**2) 
    else
       u1(i1,i2,i3,rc)=rhoe
       u1(i1,i2,i3,uc)=v1fe
       u1(i1,i2,i3,vc)=v2fe
       u1(i1,i2,i3,tc)=tfe
    end if
    
    ! Exact solid variables on the boundary
    call ogDeriv(ep2,0,0,0,0,xy2(j1,j2,j3,0),xy2(j1,j2,j3,1),0.,t,v1c,v1se)
    call ogDeriv(ep2,0,0,0,0,xy2(j1,j2,j3,0),xy2(j1,j2,j3,1),0.,t,v2c,v2se)
    call ogDeriv(ep2,0,0,0,0,xy2(j1,j2,j3,0),xy2(j1,j2,j3,1),0.,t,s11c,p11se)
    call ogDeriv(ep2,0,0,0,0,xy2(j1,j2,j3,0),xy2(j1,j2,j3,1),0.,t,s12c,p12se)
    call ogDeriv(ep2,0,0,0,0,xy2(j1,j2,j3,0),xy2(j1,j2,j3,1),0.,t,s21c,p21se)
    call ogDeriv(ep2,0,0,0,0,xy2(j1,j2,j3,0),xy2(j1,j2,j3,1),0.,t,s22c,p22se)
    u2(j1,j2,j3,v1c)=v1se
    u2(j1,j2,j3,v2c)=v2se
    u2(j1,j2,j3,s11c)=p11se
    u2(j1,j2,j3,s12c)=p12se
    u2(j1,j2,j3,s21c)=p21se
    u2(j1,j2,j3,s22c)=p22se

 end if

 ! u2(j1,j2,j3,s21c)=u2(j1,j2,j3,s12c)

 printStuff=.false.
 ! if (i1.eq.87.and.t.gt.0.3) printStuff=.true.
 if (printStuff) then
    write(6,*)'here i am, axis1,axis2=',axis1,axis2
    write(6,*)'conservativeVariables=',conservativeVariables
    write(6,*)'fluid=',u1(i1,i2,i3,rc),u1(i1,i2,i3,uc),u1(i1,i2,i3,vc),u1(i1,i2,i3,tc)
    write(6,*)'solid=',u2(j1,j2,j3,v1c),u2(j1,j2,j3,v2c),u2(j1,j2,j3,s11c),u2(j1,j2,j3,s12c),u2(j1,j2,j3,s21c),u2(j1,j2,j3,s22c)
    ! pause
 end if

 ! write(6,'(6(1x,i4))')i1,i2,i3,j1,j2,j3

 ! Determine deformation gradients (u1x,u1y,u2x,u2y).  If TZ is "on", then determine this tensor exactly
 if (twilightZone.eq.0) then

    getDeformationGradients()

    ! checkEigenStructure()

 else

    call ogDeriv(ep2,0,1,0,0,xy2(j1,j2,j3,0),xy2(j1,j2,j3,1),0.,t,u1c,u1x)
    call ogDeriv(ep2,0,1,0,0,xy2(j1,j2,j3,0),xy2(j1,j2,j3,1),0.,t,u2c,u2x)
    call ogDeriv(ep2,0,0,1,0,xy2(j1,j2,j3,0),xy2(j1,j2,j3,1),0.,t,u1c,u1y)
    call ogDeriv(ep2,0,0,1,0,xy2(j1,j2,j3,0),xy2(j1,j2,j3,1),0.,t,u2c,u2y)

 end if

 ! Here is the deformation gradient tensor and its Jacobian determinant (exact if TZ is "on")
 f11s=1.0+u1x
 f12s=    u1y
 f21s=    u2x
 f22s=1.0+u2y
 aj=f11s*f22s-f12s*f21s

 ! Here is the Cauchy stress  ( sigma=(1/J)*F*P, J=det(F) )
 s11s=(f11s*u2(j1,j2,j3,s11c)+f12s*u2(j1,j2,j3,s21c))/aj
 s12s=(f11s*u2(j1,j2,j3,s12c)+f12s*u2(j1,j2,j3,s22c))/aj
 s21s=(f21s*u2(j1,j2,j3,s11c)+f22s*u2(j1,j2,j3,s21c))/aj
 s22s=(f21s*u2(j1,j2,j3,s12c)+f22s*u2(j1,j2,j3,s22c))/aj

 ! Solid velocity
 v1s = u2(j1,j2,j3,v1c)
 v2s = u2(j1,j2,j3,v2c)

 ! Compute fluid and solid normals, and compute solid impedances
 getSolidImpedances()

 ! Compute vs : normal component of the solid velocity
 vs = n1f*v1s + n2f*v2s 

 ! nf.sigma.nf
 ps=(n1f*s11s+n2f*s21s)*n1f+(n1f*s12s+n2f*s22s)*n2f

 ! nf.sigma.tf
 pst=(n1f*s11s+n2f*s21s)*t1f+(n1f*s12s+n2f*s22s)*t2f

 ! Fluid state
 if( conservativeVariables.eq.1 )then
   rhof = u1(i1,i2,i3,rc)
   v1f  = u1(i1,i2,i3,uc)/rhof
   v2f  = u1(i1,i2,i3,vc)/rhof
   ef   = u1(i1,i2,i3,tc) ! in conservative vars this is E = p/(gamma-1) + .5*rho*v^2 
   pf   = (gamma-1.)*( ef-.5*rhof*(v1f**2+v2f**2) )
 else
   ! input vars are (rho,u,v,w,T)
   rhof = u1(i1,i2,i3,rc)
   v1f  = u1(i1,i2,i3,uc)
   v2f  = u1(i1,i2,i3,vc)
   pf   = rhof*u1(i1,i2,i3,tc)  ! p=rho*T
 end if

 ! Fluid sound speed and impedance
 af = sqrt(gamma*pf/rhof)
 zf = rhof*af 

 ! Compute vf : normal component of the fluid velocity
 vf = n1f*v1f + n2f*v2f

 ! Compute the normal component of the fluid traction, this should be matched with nf.sigma.nf
 pf0 = -(pf-pOffset)

 ! TZ corrections
 if( twilightZone.ne.0 )then

    ! exact fluid state
    call ogDeriv(ep1,0,0,0,0,xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t,rc,rhoe)
    call ogDeriv(ep1,0,0,0,0,xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t,uc,v1fe)
    call ogDeriv(ep1,0,0,0,0,xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t,vc,v2fe)
    call ogDeriv(ep1,0,0,0,0,xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t,tc,tfe)
    pfe = rhoe*tfe

    ! exact solid state
    call ogDeriv(ep2,0,0,0,0,xy2(j1,j2,j3,0),xy2(j1,j2,j3,1),0.,t,v1c,v1se)
    call ogDeriv(ep2,0,0,0,0,xy2(j1,j2,j3,0),xy2(j1,j2,j3,1),0.,t,v2c,v2se)
    call ogDeriv(ep2,0,0,0,0,xy2(j1,j2,j3,0),xy2(j1,j2,j3,1),0.,t,s11c,p11se)
    call ogDeriv(ep2,0,0,0,0,xy2(j1,j2,j3,0),xy2(j1,j2,j3,1),0.,t,s12c,p12se)
    call ogDeriv(ep2,0,0,0,0,xy2(j1,j2,j3,0),xy2(j1,j2,j3,1),0.,t,s21c,p21se)
    call ogDeriv(ep2,0,0,0,0,xy2(j1,j2,j3,0),xy2(j1,j2,j3,1),0.,t,s22c,p22se)

    ! With TZ we compute the impedance weighted average to 
    !         [v] = [v_exact]
    !         [sigma] = [sigma_exact]
    vf = vf - (n1f*v1fe + n2f*v2fe)                               ! subtract off exact 
    pf0 = pf0 - (-pfe+pOffset)                                    ! subtract off exact 
    vs = vs - (n1f*v1se + n2f*v2se)                               ! subtract off exact 
    s11se=(f11s*p11se+f12s*p21se)/aj
    s12se=(f11s*p12se+f12s*p22se)/aj
    s21se=(f21s*p11se+f22s*p21se)/aj
    s22se=(f21s*p12se+f22s*p22se)/aj
    pse=(n1f*s11se+n2f*s21se)*n1f+(n1f*s12se+n2f*s22se)*n2f       ! exact nf.sigma.nf
    pst1e=-( n1f*s11se+n2f*s21se)*n2f+( n1f*s12se+n2f*s22se)*n1f  ! exact nf.sigma.tf
    pst2e= (-n2f*s11se+n1f*s21se)*n1f+(-n2f*s12se+n1f*s22se)*n2f  ! exact tf.sigma.nf
    ps = ps - pse                                                 ! subtract off exact 

 end if

 ! Compute inpedance-weighted averages
 wf = zf
 ws = zs
 vi = (wf*vf  + ws*vs + pf0-ps          + ks*pst    )/( ws+wf )  ! interface velocity 
 pi = (ws*pf0 + wf*ps + ws*wf*( vf-vs ) - wf*ks*pst )/( ws+wf )  ! interface "pressure" n.s.n 

 ! Set normal component of fluid velocity to be vi
 v1f = v1f + (vi-vf)*n1f
 v2f = v2f + (vi-vf)*n2f

 ! New fluid pressure
 pif = -pi+pOffset
 if( twilightZone.ne.0 )then
    pif = pif + pfe  ! re-adjust the full fluid pressure for TZ
 end if

 ! Limited fluid interface stress
 !             applyLowerBound( pif,pMin )

 ! Set density assuming constant entropy
 !             if( interfaceOption.eq.1 )then
 rhofi = rhof*(pif/pf)**(1./gamma)
 ! *wdh* if (twilightZone.ne.0) rhofi=rhoe
 !*wdh*
 if( twilightZone.ne.0 )then
   ! Do not change the density from the predicted value: 
   rhofi=rhof
 end if 
 !             end if
 !             applyLowerBound( rhofi,rhoMin )
 !             rhofi=rhof

 ! Set new fluid state
 u1rc = rhofi
 !             u1(i1,i2,i3,rc) = rhofi
 if( conservativeVariables.eq.1 )then
    u1uc = v1f*rhofi
    !               u1(i1,i2,i3,uc) = v1f*rhofi
    u1vc = v2f*rhofi
    !               u1(i1,i2,i3,vc) = v2f*rhofi
    eif = pif/(gamma-1.)+.5*rhofi*(v1f**2+v2f**2)
    u1tc=eif
    !               u1(i1,i2,i3,tc)=eif
 else
    u1uc = v1f
    u1vc = v2f
    u1tc = pif/rhofi
    !               u1(i1,i2,i3,uc) = v1f
    !               u1(i1,i2,i3,vc) = v2f
    !               u1(i1,i2,i3,tc) = pif/rhofi
 end if

 ! use exact fluid values for debugging
 if (twilightZone.ne.0.and. .false.) then
    call ogDeriv(ep1,0,0,0,0,xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t,rc,u1rc)
    if( conservativeVariables.eq.1 )then
       call ogDeriv(ep1,0,0,0,0,xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t,uc,u1uc)
       call ogDeriv(ep1,0,0,0,0,xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t,vc,u1vc)
       u1uc=u1rc*u1uc
       u1vc=u1rc*u1vc
       call ogDeriv(ep1,0,0,0,0,xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t,tc,u1tc)
       u1tc=u1rc*u1tc/(gamma-1.)+.5*(u1uc**2+u1vc**2)/u1rc
       write(6,*)'here i am'
       ! pause
    else
       call ogDeriv(ep1,0,0,0,0,xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t,uc,u1uc)
       call ogDeriv(ep1,0,0,0,0,xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t,vc,u1vc)
       call ogDeriv(ep1,0,0,0,0,xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t,tc,u1tc)
    end if
 end if


 ! Set new solid velocity
 u2v1c = v1s + (vi-vs)*n1f
 u2v2c = v2s + (vi-vs)*n2f
 !             u2(j1,j2,j3,v1c) = v1s + (vi-vs)*n1f
 !             u2(j1,j2,j3,v2c) = v2s + (vi-vs)*n2f

 ! Set new solid Cauchy stress
 ps=pi                                                     ! new nf.sigma.nf
 pst1=0.                                                   ! new nf.sigma.tf
 pst2=0.                                                   ! new tf.sigma.nf     (sigma is symmetric)
 pst3=-(-n2f*s11s+n1f*s21s)*n2f+(-n2f*s12s+n1f*s22s)*n1f   ! keep old tf.sigma.tf
 if( twilightZone.ne.0 )then
    ps=ps+pse
    pst1=pst1e
    pst2=pst2e
 end if
 s11s=n1f*(n1f*ps-n2f*pst2)-n2f*(n1f*pst1-n2f*pst3)
 s12s=n2f*(n1f*ps-n2f*pst2)+n1f*(n1f*pst1-n2f*pst3)
 s21s=n1f*(n2f*ps+n1f*pst2)-n2f*(n2f*pst1+n1f*pst3)
 s22s=n2f*(n2f*ps+n1f*pst2)+n1f*(n2f*pst1+n1f*pst3)

 ! Note: assume R=[nf tf], where nf=fluid normal, tf=fluid tangent, so R=[n1f -n2f ; n2f n1f].
 ! Let K = R^T * sigma * R.  The values computed above are components of K, i.e. K=[ps pst1; pst2 pst3].
 ! The "projected" value is ps, while pst1=pts2=0 since the fluid supports no shear.  The final value
 ! pst3 should remain unchanged.

 ! Once the components of K are computed, then the Cauchy stress is given by sigma = R * K * R^T.  This
 ! is the calculation of s11s, s12s, etc above.

 ! Set new nominal stress  ( P=J*F^{-1}*sigma ).  Note that J*F^{-1}=[f22s -f12s; -f21s f11s]
 u2s11c= f22s*s11s-f12s*s21s
 u2s12c= f22s*s12s-f12s*s22s
 u2s21c=-f21s*s11s+f11s*s21s
 u2s22c=-f21s*s12s+f11s*s22s
 !             u2(j1,j2,j3,s11c)= f22s*s11s-f12s*s21s
 !             u2(j1,j2,j3,s12c)= f22s*s12s-f12s*s22s
 !             u2(j1,j2,j3,s21c)=-f21s*s11s+f11s*s21s
 !             u2(j1,j2,j3,s22c)=-f21s*s12s+f11s*s22s


 ! Set exact displacements *wdh* turn this off 2013/11/27
 ! if (twilightZone.ne.0) then
 !    call ogDeriv(ep2,0,0,0,0,xy2(j1,j2,j3,0),xy2(j1,j2,j3,1),0.,t,u1c,u2(j1,j2,j3,u1c))
 !    call ogDeriv(ep2,0,0,0,0,xy2(j1,j2,j3,0),xy2(j1,j2,j3,1),0.,t,u2c,u2(j1,j2,j3,u2c))
 ! end if

 ! use exact solid values for debugging
 if (twilightZone.ne.0.and. .false.) then
    call ogDeriv(ep2,0,0,0,0,xy2(j1,j2,j3,0),xy2(j1,j2,j3,1),0.,t,v1c,u2v1c)
    call ogDeriv(ep2,0,0,0,0,xy2(j1,j2,j3,0),xy2(j1,j2,j3,1),0.,t,v2c,u2v2c)
    !               call ogDeriv(ep2,0,0,0,0,xy2(j1,j2,j3,0),xy2(j1,j2,j3,1),0.,t,s11c,u2s11c)
    !               call ogDeriv(ep2,0,0,0,0,xy2(j1,j2,j3,0),xy2(j1,j2,j3,1),0.,t,s12c,u2s12c)
    !               call ogDeriv(ep2,0,0,0,0,xy2(j1,j2,j3,0),xy2(j1,j2,j3,1),0.,t,s21c,u2s21c)
    !               call ogDeriv(ep2,0,0,0,0,xy2(j1,j2,j3,0),xy2(j1,j2,j3,1),0.,t,s22c,u2s22c)
    !               write(6,*)'oops'
    !               pause
 end if

 ! Check results against exact states on the boundary
 if (compareWithExact) then

    ! Check fluid variables on the boundary
    call ogDeriv(ep1,0,0,0,0,xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t,rc,rhoe)
    call ogDeriv(ep1,0,0,0,0,xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t,uc,v1fe)
    call ogDeriv(ep1,0,0,0,0,xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t,vc,v2fe)
    call ogDeriv(ep1,0,0,0,0,xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t,tc,tfe)
    if( conservativeVariables.eq.1 )then
       err1(1)=abs(u1rc-rhoe)
       err1(2)=abs(u1uc-rhoe*v1fe)
       err1(3)=abs(u1vc-rhoe*v2fe)
       err1(4)=abs(u1tc-(rhoe*tfe/(gamma-1)+.5*rhoe*(v1fe**2+v2fe**2))) 
       !                 err1(1)=abs(u1rc)
       !                 err1(2)=abs(u1uc)
       !                 err1(3)=abs(u1vc)
       !                 err1(4)=abs(u1tc) 
    else
       err1(1)=abs(u1rc-rhoe)
       err1(2)=abs(u1uc-v1fe)
       err1(3)=abs(u1vc-v2fe)
       err1(4)=abs(u1tc-tfe)
    end if

    ! Check solid variables on the boundary
    call ogDeriv(ep2,0,0,0,0,xy2(j1,j2,j3,0),xy2(j1,j2,j3,1),0.,t,v1c,v1se)
    call ogDeriv(ep2,0,0,0,0,xy2(j1,j2,j3,0),xy2(j1,j2,j3,1),0.,t,v2c,v2se)
    call ogDeriv(ep2,0,0,0,0,xy2(j1,j2,j3,0),xy2(j1,j2,j3,1),0.,t,s11c,p11se)
    call ogDeriv(ep2,0,0,0,0,xy2(j1,j2,j3,0),xy2(j1,j2,j3,1),0.,t,s12c,p12se)
    call ogDeriv(ep2,0,0,0,0,xy2(j1,j2,j3,0),xy2(j1,j2,j3,1),0.,t,s21c,p21se)
    call ogDeriv(ep2,0,0,0,0,xy2(j1,j2,j3,0),xy2(j1,j2,j3,1),0.,t,s22c,p22se)
    err2(1)=abs(u2v1c-v1se)
    err2(2)=abs(u2v2c-v2se)
    err2(3)=abs(u2s11c-p11se)
    err2(4)=abs(u2s12c-p12se)
    err2(5)=abs(u2s21c-p21se)
    err2(6)=abs(u2s22c-p22se)
    !               err2(1)=abs(u2v1c)
    !               err2(2)=abs(u2v2c)
    !               err2(3)=abs(u2s11c)
    !               err2(4)=abs(u2s12c)
    !               err2(5)=abs(u2s21c)
    !               err2(6)=abs(u2s22c)

    err1max=0.
    do k=1,4
       err1max=max(err1(k),err1max)
    end do
    err2max=0.
    do k=1,6
       err2max=max(err2(k),err2max)
    end do
    if (err1max.gt.1.e-14.or.err2max.gt.1.e-14) then
       write(6,*)'** Fluid errors:'
       do k=1,4
          write(6,'(1x,i1,1x,1pe10.3)')k,err1(k)
       end do
       write(6,*)'** Solid errors:'
       do k=1,6
          write(6,'(1x,i1,1x,1pe10.3)')k,err2(k)
       end do
       ! pause
    end if

 end if

 !  cgmp elasticPiston -method=cns -cnsVariation=godunov -g="elasticPistonGrid4" -tp=.05 -tf=.05 -smVariation=g -godunovType=2 -problem=0 -tz=poly -debug=3 -go=go -piGhostOption=0 -pi=1
 !  cgmp noplot elasticPiston -method=cns -cnsVariation=godunov -g="elasticPistonGridfx2fy2.hdf" -tp=.1 -tf=.1 -smVariation=g -godunovType=2 -problem=0 -tz=poly -debug=3 -go=go -piGhostOption=0 -pi=1

 if (compareWithBill) then

    !
    !  ****  HERE IS BILLS CODE TO COMPARE RESULTS  ****
    !

    ! if( .false. .or. (i1.gt.n1a .and. i1.lt.n1b) )then  ! ********** TEST **************
    ! if( i1.gt.n1a )then  ! ********** TEST **************
    if( .true. )then

       ! if( i1.eq.0 )then ! ********** TEST ***********
       !   u1(i1,i2,i3,vc)=0.
       !   u2(j1,j2,j3,u2c)=0.
       !   u2(j1,j2,j3,v2c)=0.
       !   u2(j1,j2,j3,s22c)=0.
       ! end if


       ! fluid: 
       if( conservativeVariables.eq.1 )then
          rhof= u1(i1,i2,i3,rc)
          v1f = u1(i1,i2,i3,uc)/rhof
          v2f = u1(i1,i2,i3,vc)/rhof
          ef  = u1(i1,i2,i3,tc) ! in conservative vars this is E = p/(gamma-1) + .5*rho*v^2 
          pf = (gamma-1.)*( ef-.5*rhof*(v1f**2+v2f**2) )    ! p 
       else
          ! input vars are (rho,u,v,w,T)
          rhof= u1(i1,i2,i3,rc)
          v1f = u1(i1,i2,i3,uc)
          v2f = u1(i1,i2,i3,vc)
          pf = rhof*u1(i1,i2,i3,tc)  ! p=rho*T

       endif

       applyLowerBound( rhof,rhoMin )
       applyLowerBound( pf,pMin )

       pf0 = -(pf-pOffset)                               ! traction = pf0*nf

       af = sqrt(gamma*pf/rhof)
       zf = rhof*af   ! fluid impedance 

       ! NOTE:
       ! -- the fluid normal is flipped to point into the fluid domain 
       ! -- the solid normal points out of the solid domain (in the same general direction as nf)
       !
       !          ----solid-----------I------fluid-------------
       !                            n -> 
       ! 
       !          ----fluid ----------I------solid ------------
       !                            <- n 
       !
       ! NOTE: the direction of the normal, n, determines uLeft and uRight. If n goes from solid to fluid then
       !   uLeft=solid and uRight=fluid

       ! compute the fluid normal (n1f,n2f)
       getFluidNormal()

       if (useAllSolidNormals) then
          getSolidNormal()
          n1f=n1s
          n2f=n2s
       end if

       ! vf : normal component of the fluid velocity
       vf = n1f*v1f + n2f*v2f ! 

       ! call ogDeriv(ep,0,0,0,0,xy(i1-2*is1,i2-2*is2,i3,0),xy(i1-2*is1,i2-2*is2,i3,1),0.,t,n,uem)


       ! solid:
       v1s = u2(j1,j2,j3,v1c)
       v2s = u2(j1,j2,j3,v2c)
       s11s =u2(j1,j2,j3,s11c)
       s12s =u2(j1,j2,j3,s12c)
       s22s =u2(j1,j2,j3,s22c)

       ! compute the solid normal (n1s,n2s)
       getSolidNormal()

       ! vs : normal component of the solid velocity 
       ! vs = n1s*v1s + n2s*v2s   
       ! We follow what was done in the standard VS/SF algorithm in which case we use the
       ! fluid normal to determine the normal component of the velocity
       ! use fluid normal here
       vs = n1f*v1s + n2f*v2s   ! normal component of the solid velocity 

       ! solid traction is ns.sigmas:
       traction1 = n1s*s11s + n2s*s12s
       traction2 = n1s*s12s + n2s*s22s

       if (printStuff) then
          ! if (i1.eq.87) then
          write(6,*)'traction1,traction2=',traction1,traction2
          write(6,*)'n1f,n2f,an1f,an2f=',n1f,n2f,an1f,an2f
          write(6,*)'n1s,n2s,an1,an2=',n1s,n2s,an1,an2
       end if

       ! ps is the normal component of the solid traction
       ! ps = n1s*traction1+n2s*traction2  ! ps = n.sigma.n 
       ! use fluid normal here
       ps = n1f*traction1+n2f*traction2  ! ps = n.sigma.n 

       if( twilightZone.ne.0 )then
          ! -- compute the exact solution for TZ --
          ! write(*,'(" >>>>>>>>>>>> interfaceCnsSm: twilightZone is ON <<<<<<<<<<<<<<<<")') 

          call ogDeriv(ep1,0,0,0,0,xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t,rc,rhoe)
          call ogDeriv(ep1,0,0,0,0,xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t,uc,v1fe)
          call ogDeriv(ep1,0,0,0,0,xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t,vc,v2fe)
          call ogDeriv(ep1,0,0,0,0,xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),0.,t,tc,tfe)
          pfe = rhoe*tfe

          call ogDeriv(ep2,0,0,0,0,xy2(j1,j2,j3,0),xy2(j1,j2,j3,1),0.,t,v1c,v1se)
          call ogDeriv(ep2,0,0,0,0,xy2(j1,j2,j3,0),xy2(j1,j2,j3,1),0.,t,v2c,v2se)
          call ogDeriv(ep2,0,0,0,0,xy2(j1,j2,j3,0),xy2(j1,j2,j3,1),0.,t,s11c,s11se)
          call ogDeriv(ep2,0,0,0,0,xy2(j1,j2,j3,0),xy2(j1,j2,j3,1),0.,t,s12c,s12se)
          call ogDeriv(ep2,0,0,0,0,xy2(j1,j2,j3,0),xy2(j1,j2,j3,1),0.,t,s21c,s21se)
          call ogDeriv(ep2,0,0,0,0,xy2(j1,j2,j3,0),xy2(j1,j2,j3,1),0.,t,s22c,s22se)

          ! if( twilightZone.ne.0 )then ! ********** TEST ***********
          !  write(*,'("Start: rhof,v1f,v2f,pf = ",4e10.2)') rhof,v1f,v2f,pf
          !  write(*,'("Start: pf0,pfe, pOffset = ",3e10.2)') pf0,pfe,pOffset
          ! end if

          ! With TZ we compute the impedance weighted average to 
          !         [v] = [v_exact]
          !         [sigma] = [sigma_exact]
          vf = vf - (n1f*v1fe + n2f*v2fe)  ! subtract off exact 
          pf0 = pf0 - (-pfe+pOffset)        ! subtract off exact 
          vs = vs - (n1f*v1se + n2f*v2se)  ! subtract off exact 

          traction1e = n1s*s11se + n2s*s12se  
          traction2e = n1s*s12se + n2s*s22se
          ps = ps - (n1f*traction1e+n2f*traction2e)  ! subtract off exact 

       end if

       ! ------------------------------------
       ! --- project the interface values ---
       ! ------------------------------------

       ! impedance weighted averages: 
       wf = zf
       ! ws = zs
       ws = rhos*sqrt((lambda+2.*mu)/rhos)

       vi = (wf*vf  + ws*vs + pf0-ps          )/( ws+wf )  ! interface velocity 
       pi = (ws*pf0 + wf*ps + ws*wf*( vf-vs ) )/( ws+wf )  ! interface "pressure" n.s.n 

       ! vi = (wf*vf  + ws*vs                  )/( ws+wf )  ! interface velocity 
       ! pi = (ws*pf0 + wf*ps                  )/( ws+wf )  ! interface "pressure" n.s.n 

       ! vi = (eps2*vf  + eps1*vs)/( eps1+eps2 )  ! interface velocity 
       ! pi = (eps1*pf0 + eps2*ps)/( eps1+eps2 )  ! interface "pressure" n.s.n 


       ! if( twilightZone.ne.0 )then ! ********** TEST ***********
       !  write(*,'(" vf,vs,vi = ",3e10.2)') vf,vs,vi
       !  write(*,'(" pf0,ps,pi = ",3e10.2)') pf0,ps,pi
       ! end if

       ! if( i1.eq.0 )then ! ********** TEST ***********
       !   vi=0.
       !   pi=pf0
       ! end if

       ! if( .false. )then ! *************** TEST ***********
       !  vi = (wf*vf  + ws*vs )/( ws+wf )  
       !  pi = (ws*pf0 + wf*ps )/( ws+wf ) 
       ! end if
       ! if( .false. )then ! *************** TEST ***********
       !  vi = (wf*vf  + ws*vs )/( ws+wf )  
       !  pi = pf0
       ! end if
       ! if( .false. )then ! *************** TEST ***********
       !   vi = vs
       !   pi = pf0
       ! end if

       if( .false. )then
          write(*,'(" IP:fluid: (i1,i2)=(",i3,i3,") pf=",e10.3," pOffset=",e10.3)') i1,i2,pf,pOffset
          write(*,'(" IP:fluid: (i1,i2)=(",i3,i3,") n=(",2f6.3,") (rhof,v1f,v2f,-pf)=(",4e9.2,") zf=",e8.2," af=",e8.2)') i1,i2,n1f,n2f,rhof,v1f,v2f,-pf+pOffset,zf,af

          write(*,'(" IP:solid: (j1,j2)=(",i3,i3,") n=(",2f6.3,") (rhos,v1s,v2s, ps)=(",4e9.2,") zs=",e9.3," as=",e8.2," (u1,u2)=",2e9.2,")")') j1,j2,n1s,n2s,rhos,v1s,v2s,ps,zs,cp,u2(j1,j2,j3,u1c),u2(j1,j2,j3,u2c)
          write(*,'(" IP:solid: (j1,j2)=(",i3,i3,") (s11,s12,s22)=(",3e9.2,")")') j1,j2,s11s,s12s,s22s
          write(*,'(" IP: -> vi=",e16.7," pi=",e16.7)') vi,pi


          write(*,'(" IP: (vf,vs)=",2e10.2," -> vi=",e16.7," (-pf,ps)=",2e10.2," pi=",e16.7)') vf,vs,vi, -pf+pOffset,ps,pi
          write(1,'(" IP: (vf,vs)=",2e10.2," -> vi=",e16.7," (-pf,ps)=",2e10.2," pi=",e16.7)') vf,vs,vi, -pf+pOffset,ps,pi

       end if


       ! Here is the solution to the nonlinear FSR
       if( interfaceOption.eq.5 )then
          ! -- this will not work with twilightZone !
          solid(0)=rhos
          solid(1)=vs
          solid(2)=ps
          solid(3)=cp
          fluid(0)=rhof
          fluid(1)=vf  
          fluid(2)=pf  
          fluid(3)=gamma
          fluid(4)=pOffset
          call fluidSolidRiemannSolution( solid, fluid, fsr )
          ! write(*,'(" nonlinear-FSR: rhoi,vi,pi=",3e13.5)') fsr(0),fsr(1),-fsr(2)+pOffset
          rhofi = fsr(0)
          vi    = fsr(1)
          pi    = -fsr(2)+pOffset
       end if

       ! -- here is the projection --
       !  Set normal component of fluid velocity to be vi:
       v1f = v1f + (vi-vf)*n1f
       v2f = v2f + (vi-vf)*n2f


       !          err1(2)=max(abs(v1f-u1uc),err1(2))
       !          err1(3)=max(abs(v2f-u1vc),err1(3))


       ! ***********
       ! if( .false. )then
       !   v1f=vi
       !   v2f=0.
       ! end if

       !  Adjust rhof using: Entropy const : p/rho^gamma = K

       pif = -pi+pOffset  ! new fluid pressure
       if( twilightZone.ne.0 )then
          ! For TZ re-adjust the full fluid pressure
          pif = pif + pfe 
       end if

       !          err1(4)=max(abs(pif-u1tc),err1(4))


       if( .true. )then 
          applyLowerBound( pif,pMin )
          pi = -pif+pOffset  ! adjust interface stress

          if( interfaceOption.eq.1 )then
             rhofi = rhof*(pif/pf)**(1./gamma) !  choose interface rho from S=const
          end if
          applyLowerBound( rhofi,rhoMin )

       end if

       ! write(*,'("After project pif,pfe = ",2e10.2)') pif,pfe
       ! write(*,'("After project rhofi,rhoe = ",2e10.2)') rhofi,rhoe

       u1(i1,i2,i3,rc) = rhofi

       if( conservativeVariables.eq.1 )then
          u1(i1,i2,i3,uc) = v1f*rhofi
          u1(i1,i2,i3,vc) = v2f*rhofi
          eif = pif/(gamma-1.)+.5*rhofi*(v1f**2+v2f**2)
          u1(i1,i2,i3,tc)=eif
       else
          u1(i1,i2,i3,uc) = v1f
          u1(i1,i2,i3,vc) = v2f
          u1(i1,i2,i3,tc)=pif/rhofi
       end if

       !  Set normal component of solid velocity to be vi:
       ! *** which normals should we use here ??
       ! u2(j1,j2,j3,v1c) = u2(j1,j2,j3,v1c) + (vi-vs)*n1s
       ! u2(j1,j2,j3,v2c) = u2(j1,j2,j3,v2c) + (vi-vs)*n2s
       ! Use fluid normal since vs = nf.vvs
       u2(j1,j2,j3,v1c) = u2(j1,j2,j3,v1c) + (vi-vs)*n1f
       u2(j1,j2,j3,v2c) = u2(j1,j2,j3,v2c) + (vi-vs)*n2f

       !  Assign the stress in the solid: 
       !       sigmas.ns = g = pi nf 
       ! We follow the projection that would have been used in the standard algorithm
       ! In this case the traction vector pi*nf would be passed to the solid mechanics code
       ! where the solid normal and tangent would then be used to set the components of the stress.
       ! 
       !  In 2d we use the 3 equations:
       !      ns.s.ns = ns.g = f11 = pi ns.nf 
       !      ts.s.ns = tf.g = f12 = pi ts.nf
       !      ts.s.ts = ts.s(old).ts = f22  (i.e. do not change this component)
       if( .true. )then
          ! use a mix of solid and fluid normals
          t1s=-n2s !  solid tangent 
          t2s= n1s 
          f11 = pi *( n1s*n1f + n2s*n2f )
          f12 = pi *( t1s*n1f + t2s*n2f )
          f22=t1s*t1s*s11s + 2.*t1s*t2s*s12s + t2s*t2s*s22s 
          u2(j1,j2,j3,s11c) = n1s*n1s*f11 + 2.*n1s*t1s      *f12 + t1s*t1s*f22
          u2(j1,j2,j3,s12c) = n1s*n2s*f11 +(n1s*t2s+n2s*t1s)*f12 + t1s*t2s*f22
          u2(j1,j2,j3,s22c) = n2s*n2s*f11 + 2.*n2s*t2s      *f12 + t2s*t2s*f22

          u2(j1,j2,j3,s21c) =   u2(j1,j2,j3,s12c)
       else 
          ! use fluid normal --- what about the tangent ?? ********** FIX
          t1s=-n2f !  solid tangent 
          t2s= n1f 
          f11 = pi
          f12=0.
          f22=t1s*t1s*s11s + 2.*t1s*t2s*s12s + t2s*t2s*s22s 
          u2(j1,j2,j3,s11c) = n1f*n1f*f11 + 2.*n1f*t1s      *f12 + t1s*t1s*f22
          u2(j1,j2,j3,s12c) = n1f*n2f*f11 +(n1f*t2s+n2f*t1s)*f12 + t1s*t2s*f22
          u2(j1,j2,j3,s22c) = n2f*n2f*f11 + 2.*n2f*t2s      *f12 + t2s*t2s*f22

          u2(j1,j2,j3,s21c) =   u2(j1,j2,j3,s12c)
       end if


       if( twilightZone.ne.0 )then
          ! TZ corrections: 

          u2(j1,j2,j3,v1c) = u2(j1,j2,j3,v1c) + v1se
          u2(j1,j2,j3,v2c) = u2(j1,j2,j3,v2c) + v2se

          u2(j1,j2,j3,s11c) = u2(j1,j2,j3,s11c) + s11se
          u2(j1,j2,j3,s12c) = u2(j1,j2,j3,s12c) + s12se
          u2(j1,j2,j3,s21c) = u2(j1,j2,j3,s21c) + s21se
          u2(j1,j2,j3,s22c) = u2(j1,j2,j3,s22c) + s22se

          ! Do this for testing:  
          ! rhofi=rhoe
          ! v1f=v1fe
          ! v2f=v2fe
          ! pif=rhoe*tfe

          ! write(*,'(" IP: conservativeVariables=",i2)') conservativeVariables
          ! write(*,'(" IP: (i1,i2)=(",i3,i3,") Fluid: u,ue=",2e10.3," v,ve=",2e10.3," p,pe=",2e10.3)') i1,i2,u1(i1,i2,i3,uc)/rhofi,v1fe,u1(i1,i2,i3,vc)/rhofi,v2fe,pif,pfe

          if( conservativeVariables.eq.1 )then
             u1(i1,i2,i3,rc) = rhofi
             u1(i1,i2,i3,uc) = v1f*rhofi
             u1(i1,i2,i3,vc) = v2f*rhofi
             eif = pif/(gamma-1.)+.5*rhofi*(v1f**2+v2f**2)
             u1(i1,i2,i3,tc)=eif
          else
             u1(i1,i2,i3,uc) = v1f
             u1(i1,i2,i3,vc) = v2f
             u1(i1,i2,i3,tc)=pif/rhofi
          end if

          u2(j1,j2,j3,v1c)=v1se
          u2(j1,j2,j3,v2c)=v2se
          u2(j1,j2,j3,s11c)=s11se
          u2(j1,j2,j3,s12c)=s12se
          u2(j1,j2,j3,s21c)=s21se
          u2(j1,j2,j3,s22c)=s22se

       end if



    end if


    err1(1)=max(abs(u1(i1,i2,i3,rc)-u1rc),err1(1))
    err1(2)=max(abs(u1(i1,i2,i3,uc)-u1uc),err1(2))
    err1(3)=max(abs(u1(i1,i2,i3,vc)-u1vc),err1(3))
    err1(4)=max(abs(u1(i1,i2,i3,tc)-u1tc),err1(4))

    err2(1)=max(abs(u2(j1,j2,j3,v1c)-u2v1c),err2(1))
    err2(2)=max(abs(u2(j1,j2,j3,v2c)-u2v2c),err2(2))
    err2(3)=max(abs(u2(j1,j2,j3,s11c)-u2s11c),err2(3))
    err2(4)=max(abs(u2(j1,j2,j3,s12c)-u2s12c),err2(4))
    err2(5)=max(abs(u2(j1,j2,j3,s21c)-u2s21c),err2(5))
    err2(6)=max(abs(u2(j1,j2,j3,s22c)-u2s22c),err2(6))


 else ! compare with Bill ?

    u1(i1,i2,i3,rc)=u1rc
    u1(i1,i2,i3,uc)=u1uc
    u1(i1,i2,i3,vc)=u1vc
    u1(i1,i2,i3,tc)=u1tc

    u2(j1,j2,j3,v1c)=u2v1c
    u2(j1,j2,j3,v2c)=u2v2c
    u2(j1,j2,j3,s11c)=u2s11c
    u2(j1,j2,j3,s12c)=u2s12c
    u2(j1,j2,j3,s21c)=u2s21c
    u2(j1,j2,j3,s22c)=u2s22c

 end if



 endIJLoops2d()

 if (compareWithBill) then
    if (t.ge.printErrorsAfterTime) then
       do k=1,4
          write(6,*)'err1=',err1(k)
       end do
       do k=1,6
          write(6,*)'err2=',err2(k)
       end do
       ! pause
    end if
 end if

#endMacro


! ================================================================================
! Assign ghost values at the interface using compatibility conditions.
! 
! ================================================================================
#beginMacro assignGhostFromCompatibility()

 beginIJLoops2d()

   i1g=i1-is1
   i2g=i2-is2
   i3g=i3-is3

   j1g=j1-js1
   j2g=j2-js2
   j3g=j3-js3

   ! compute the fluid normal (n1f,n2f)
   getFluidNormal()
   
   ! compute the solid normal (n1s,n2s)
   getSolidNormal()

   ! -- eval the fluid and solid momentum equations on the boundary using "one-sided" approx.  --

   ! fluid momentum equation:
   rhof = u1(i1,i2,i3,rc)
   v1mg1f = u1(i1,i2,i3,uc)-gv1(i1,i2,i3,0)
   v2mg2f = u1(i1,i2,i3,vc)-gv1(i1,i2,i3,1)
   !nDotVGradV = n1f*( v1mg1f*u1x22(i1,i2,i3,uc) + v2mg2f*u1y22(i1,i2,i3,uc) ) +\
   !             n2f*( v1mg1f*u1x22(i1,i2,i3,vc) + v2mg2f*u1y22(i1,i2,i3,vc) )

   ! Note: the nonlinear terms actually simplify to only tangential derivatives: (since n.(vf-Gv)=0)
   if( axis1.eq.0 )then
      nDotVGradV = ( v1mg1f*rsxy1(i1,i2,i3,axis1p1,0) + v2mg2f*rsxy1(i1,i2,i3,axis1p1,1) )*\
      ( n1f*u1s2(i1,i2,i3,uc) + n2f*u1s2(i1,i2,i3,vc) )
   else
      nDotVGradV = ( v1mg1f*rsxy1(i1,i2,i3,axis1p1,0) + v2mg2f*rsxy1(i1,i2,i3,axis1p1,1) )*\
      ( n1f*u1r2(i1,i2,i3,uc) + n2f*u1r2(i1,i2,i3,vc) )
   end if

   ! nDotGradp0 = n.grad(p) evaluated with wrong ghost point
   nDotGradp0 = n1f*u1x22(i1,i2,i3,pc) + n2f*u1y22(i1,i2,i3,pc) 

   ! v1tf = - u1x22(i1,i2,i3,pc)/rhof - v1mg1f*u1x22(i1,i2,i3,uc) - v2mg2f*u1y22(i1,i2,i3,uc) 
   ! v2tf = - u1y22(i1,i2,i3,pc)/rhof - v1mg1f*u1x22(i1,i2,i3,vc) - v2mg2f*u1y22(i1,i2,i3,vc)

   ! vtf = n.( vf.t )
   ! NOTE: We SHOULD project (n.vf).t = n.( vf.t ) + (n.t).(vf) : TODO add (n.t).(vf) term!
   ! vtf = n1f*v1tf + n2f*v2tf 
   vtf = - nDotVGradV - nDotGradp0/rhof

   ! solid momentum equation:
   v1ts = (u2x22(j1,j2,j3,s11c) + u2y22(j1,j2,j3,s12c))/rhos 
   v2ts = (u2x22(j1,j2,j3,s12c) + u2y22(j1,j2,j3,s22c))/rhos 
   vts = n1s*v1ts + n2s*v2ts 

   ! --- evaluate the fluid and solid stress equations ---

   ! fluid stress equation : (-p)_t = (uv-Gv).Grad p + gamma*p*div(u) 
   ! vfDotGradp =v1mg1f*u1x22(i1,i2,i3,pc) + v2mg2f*u1y22(i1,i2,i3,pc)  
   ! Note: the nonlinear terms actually simplify to only tangential derivatives: (since n.(vf-Gv)=0)
   if( axis1.eq.0 )then
      vfDotGradp = ( v1mg1f*rsxy1(i1,i2,i3,axis1p1,0) + v2mg2f*rsxy1(i1,i2,i3,axis1p1,1) )*( u1s2(i1,i2,i3,pc) )
   else
      vfDotGradp = ( v1mg1f*rsxy1(i1,i2,i3,axis1p1,0) + v2mg2f*rsxy1(i1,i2,i3,axis1p1,1) )*( u1r2(i1,i2,i3,pc) )

   end if

   ! divVf0 = div(vf) evaluated with wrong ghost values for v
   divVf0 = u1x22(i1,i2,i3,uc)+u1y22(i1,i2,i3,vc)
   ! NOTE: todo: add (nv.t)*v term!
   sigmatf = vfDotGradp + gamma*u1(i1,i2,i3,pc)*divVf0

   ! solid stress equation:
   v1x = u2x22(j1,j2,j3,v1c)
   v1y = u2y22(j1,j2,j3,v1c)
   v2x = u2x22(j1,j2,j3,v2c)
   v2y = u2y22(j1,j2,j3,v2c)

   sigma11ts = (lambda+2.*mu)*v1x + lambda*v2y
   sigma12ts = mu*(v1y+v2x)
   sigma22ts = (lambda+2.*mu)*v2y + lambda*v1x

   sigmats = n1s*(n1s*sigma11ts + n2s*sigma12ts)   + n2s*(n1s*sigma12ts + n2s*sigma22ts)


   af = sqrt(gamma*u1(i1,i2,i3,pc)/rhof)  ! fluid speed of sound
   ! impedance weighted averages: 
   wf = rhof*af   ! zf 
   ws = rhos*cp   ! zs

   vDotI     = (wf*vtf     + ws*vts    )/(wf+ws)
   sigmaDotI = (ws*sigmatf + wf*sigmats)/(wf+ws)

   ! set the ghost values of velocity and stress/pressure

   ! n.grad( p ) = -rho*( vDotI - nv.[(uv-gv).Grad(uv)] )
   ! n.grad(p) = (n1*rx+n2*ry)*pr + (n1*sx+n2*sy)*ps
   ! nDotGrap = (nDotGradp0 - cm1*pOld) + cm1*pNew
   ! cm1 = coefficient of ghost value for p, p(-1) in  n.grad(p)
   cm1 = -is*(n1f*rx1 + n2f*ry1)/(2.*dr1(axis1))

   nDotGrap = -rhof*( vDotI + nDotVGradV )  ! new value of n.grad(p)
   pm1 = u1(i1-is1,i2-is2,i3,pc)            ! old value of p on ghost

   pNew=( nDotGrap - (nDotGradp0 -cm1*pm1) )/cm1 
   u1(i1g,i2g,i3g,pc) = pNew
   limitWithExtrap(pNew,  u1,i1g,i2g,i3g,is1,is2,is3,pc)
   ! The lower bound on p is set below

   if( .false. )then
      write(*,'("IBC: i1,i2=",2i3,", is1,is2=",2i3,", j1,j2=",2i3,", js1,js2=",2i3)') i1,i2,is1,is2,j1,j2,js1,js2
      ! write(*,'("IBC:  vf(0),vs(0)=",2e14.5," -p(0),sigma11(0)=",2e14.5)') u1(i1,i2,i3,uc),u2(j1,j2,j3,v1c),-u1(i1,i2,i3,pc)+pOffset,u2(j1,j2,j3,s11c)
      write(*,'("IBC:  [v]=",e9.2," [s]=",e9.2," vf,vs=",2e16.8)') u1(i1,i2,i3,uc)-u2(j1,j2,j3,v1c),-u1(i1,i2,i3,pc)+pOffset-u2(j1,j2,j3,s11c),u1(i1,i2,i3,uc),u2(j1,j2,j3,v1c)
      ! write(*,'("IBC:  p(-1),pe=",2e12.3)') pNew,pm1
   end if

   ! --------

   ! rhos*vDotI =n.( s11.x + s12.y, s12.x + s22.y )
   ! ->  n1*(rx*s11r+ry*s12r) + n2*(rx*s12r+ry*s22r) + ...
   !  rhos*vDotI = [ n1*rx*s11(-1) + n1*ry*s12(-1) + n2*rx*s12(-1) + n2*ry*s22(-1)]/(2*dr) + ( n.div(s) - ... 
   !             = [ sigma_nn(-1) ]*|grad(r)|/(2*dr) + ( n.div(s) - 
   !  Note: sigma_nn = n1^2*s11 + 2*n1*n2*s12 + n2^2*s22 
   ! -> sigma_nn(-1) =  g 
   !    sigma_nt(-1) = unchanged
   !    sigma_tt(-1) = unchanged

   !  In 2d we use the 3 equations:
   !      n.s.n = n.g = f11
   !      t.s.n = t.g = f12 
   !      t.s.t = t.s(old).t = f22  (i.e. do not change this component)
   if( .true. )then ! --
      t1s=-n2s !  solid tangent 
      t2s= n1s 
      nDotDivSigma0 = rhos*vts ! evaluated with wrong value on ghost 
      s11s=u2(j1g,j2g,j3g,s11c)
      s12s=u2(j1g,j2g,j3g,s12c)
      s22s=u2(j1g,j2g,j3g,s22c)

      csm = -js*( n1s*rx2*s11s + n1s*ry2*s12s + n2s*rx2*s12s + n2s*ry2*s22s)/(2.*dr2(axis2))
      f11 = -js*( rhos*vDotI - (nDotDivSigma0 - csm) )*(2.*dr2(axis2)/r2Norm)
      f12=n1s*t1s*s11s + (n1s*t2s+n2s*t1s)*s12s + n2s*t2s*s22s
      f22=t1s*t1s*s11s + 2.*t1s*t2s*s12s + t2s*t2s*s22s 

      s11sNew = n1s*n1s*f11 + 2.*n1s*t1s      *f12 + t1s*t1s*f22
      s12sNew = n1s*n2s*f11 +(n1s*t2s+n2s*t1s)*f12 + t1s*t2s*f22
      s22sNew = n2s*n2s*f11 + 2.*n2s*t2s      *f12 + t2s*t2s*f22

      ! try this:
      ! s11sNew =u2(j1+js1,j2+js2,j3,s11c) - js*(2.*dr2(axis2)/rx2)*( rhos*n1s*vDotI )

      ! u2(j1-js1,j2-js2,j3,s11c)=s11s
      ! u2(j1-js1,j2-js2,j3,s11c)=s11sNew

      u2(j1g,j2g,j3g,s11c) = n1s*n1s*f11 + 2.*n1s*t1s      *f12 + t1s*t1s*f22
      u2(j1g,j2g,j3g,s12c) = n1s*n2s*f11 +(n1s*t2s+n2s*t1s)*f12 + t1s*t2s*f22
      u2(j1g,j2g,j3g,s22c) = n2s*n2s*f11 + 2.*n2s*t2s      *f12 + t2s*t2s*f22

      limitWithExtrap(u2(j1g,j2g,j3g,s11c),  u2,j1g,j2g,j3g,js1,js2,js3,s11c)
      limitWithExtrap(u2(j1g,j2g,j3g,s12c),  u2,j1g,j2g,j3g,js1,js2,js3,s12c)
      limitWithExtrap(u2(j1g,j2g,j3g,s22c),  u2,j1g,j2g,j3g,js1,js2,js3,s22c)
      u2(j1g,j2g,j3g,s21c) = u2(j1g,j2g,j3g,s12c)

      if( .false. )then
         write(*,'("IBC:  s11,s11e=(",3e12.4,") s12=(",2e12.4,") s22=(",2e12.4,")")') \
         s11sNew,s11s,n1s*n1s*f11 + 2.*n1s*t1s      *f12 + t1s*t1s*f22,s12sNew,s12s,s22sNew,s22s
         write(*,'("IBC:  vtf,vts,vDotI=",3e12.4,")")') vtf,vts,vDotI
      end if
   end if
   if( .true. )then ! --

      ! (-p)_t = gamma*p*div(u)
      ! div(v) = (divOld - cum1*uOld(-1) - cvm1vOld(-1)) + cum1*u(-1) + cvm1*v(-1) 
      ! cum1*u(-1) + cvm1*v(-1) = div(v) - (divOld - cum1*uOld(-1) - cvm1vOld(-1))
      !                         = g 
      divVf = (sigmaDotI- vfDotGradp) /( gamma*u1(i1,i2,i3,pc) )
      ! div(v) = (rx*ur + ry*vr) + ( sx*us+sy*vs) 
      ! cum1 : coeff of u(-1) in div(v)
      ! cvm1 : coeff of v(-1) in div(v)
      cum1 = -is*rx1/(2.*dr1(axis1))
      cvm1 = -is*ry1/(2.*dr1(axis1))

      ! divVf0 = div(v) with wrong ghost value 
      um1=u1(i1g,i2g,i3g,uc)  
      vm1=u1(i1g,i2g,i3g,vc)
      g = divVf -( divVf0 - cum1*um1 - cvm1*vm1 )

      ! project [u(-1),v(-1)] so that cum1*u(-1) + cvm1*v(-1) = g 
      ! *check me*
      cNormSq = cum1**2 + cvm1**2 
      v1fNew = um1 + (g-cum1*um1-cvm1*vm1)*cum1/cNormSq
      v2fNew = vm1 + (g-cum1*um1-cvm1*vm1)*cvm1/cNormSq
      u1(i1g,i2g,i3g,uc)=um1 + (g-cum1*um1-cvm1*vm1)*cum1/cNormSq
      u1(i1g,i2g,i3g,vc)=vm1 + (g-cum1*um1-cvm1*vm1)*cvm1/cNormSq

      limitWithExtrap(u1(i1g,i2g,i3g,uc),  u1,i1g,i2g,i3g,is1,is2,is3,uc)
      limitWithExtrap(u1(i1g,i2g,i3g,vc),  u1,i1g,i2g,i3g,is1,is2,is3,vc)

      if( .false. )then
         write(*,'("IBC:  v1f,v1fe=(",2e12.3,") v2f,v2fe=(",2e12.3,")")') \
         v1fNew,um1,v2fNew,vm1
      end if
      ! n.sigma_t = sigmaDotI n 
      ! n1*( alpha*v1_x + lam*v2_y ) + n2*( v1_y + v2_x           ) = sigmaDotI n1
      ! n1*( v1_y + v2_x           ) + n2*( alpha*v2_y + lam*v1_x ) = sigmaDotI n2

      ! These imply: 
      ! a11*v1(-1) + a12*v2(-1) = g1
      ! a21*v1(-1) + a22*v2(-1) = g2

      alpha=lambda+2.*mu
      a11 = -js*( n1s*rx2*alpha +n2s*ry2*mu )/(2.*dr2(axis2))
      a12 = -js*( n1s*ry2*lambda+n2s*rx2*mu )/(2.*dr2(axis2))

      a22 = -js*( n2s*ry2*alpha +n1s*rx2*mu )/(2.*dr2(axis2))
      a21 = -js*( n2s*rx2*lambda+n1s*ry2*mu )/(2.*dr2(axis2))

      v1m = u2(j1-js1,j2-js2,j3,v1c)  ! wrong ghost values
      v2m = u2(j1-js1,j2-js2,j3,v2c)

      g1 = sigmaDotI*n1s - ( n1s*sigma11ts + n2s*sigma12ts - a11*v1m -a12*v2m )
      g2 = sigmaDotI*n2s - ( n1s*sigma12ts + n2s*sigma22ts - a21*v1m -a22*v2m )

      deti = 1./(a11*a22-a12*a21)

      ! v1sNew = (a22*g1 - a12*g2)*deti
      ! v2sNew = (a11*g2 - a21*g1)*deti 

      u2(j1g,j2g,j3g,v1c) = (a22*g1 - a12*g2)*deti
      u2(j1g,j2g,j3g,v2c) = (a11*g2 - a21*g1)*deti 

      limitWithExtrap(u2(j1g,j2g,j3g,v1c),  u2,j1g,j2g,j3g,js1,js2,js3,v1c)
      limitWithExtrap(u2(j1g,j2g,j3g,v2c),  u2,j1g,j2g,j3g,js1,js2,js3,v2c)

      ! write(*,'("IBC:  v1s,v1se=(",2e12.3,") v2s,v2se=(",2e12.3,")")') \
      !     v1sNew,v1m,v2sNew,v2m

      ! set the displacement:   ! finish me ...
      ! u1New = u2(j1+js1,j2+js2,j3,u1c) -js*2.*(dr2(axis2)/rx2)*( u2(j1,j2,j3,s11c)/alpha )  
      ! write(*,'("IBC:  u1c=",i2," u1s,u1e=(",2e12.3,")")') u1c,u1New,u2(j1-js1,j2-js2,j3,u1c)

      ! u2(j1-js1,j2-js2,j3,u1c)=u1New
      ! u2(j1-js1,j2-js2,j3,u2c)=0.

      if( .false. )then
         write(*,'("IBC:(",2i3,") Ghost1: (r,u,p)=(",3e12.5,") (u,v,s)=(",3e12.5,")")') i1,i2,\
         u1(i1-is1,i2-is2,i3,rc),u1(i1-is1,i2-is2,i3,uc),u1(i1-is1,i2-is2,i3,pc),\
         u2(j1-js1,j2-js2,j3,u1c),u2(j1-js1,j2-js2,j3,v1c),u2(j1-js1,j2-js2,j3,s11c)
      end if


   end if

   ! 2nd ghost 
   i1g=i1-2*is1
   i2g=i2-2*is2
   i3g=i3-2*is3

   limitedExtrapolation(u1, i1g,i2g,i3g,is1,is2,is3,uc)   
   limitedExtrapolation(u1, i1g,i2g,i3g,is1,is2,is3,vc)   
   limitedExtrapolation(u1, i1g,i2g,i3g,is1,is2,is3,pc)            

   j1g=j1-2*js1
   j2g=j2-2*js2
   j3g=j3-2*js3

   limitedExtrapolation(u2, j1g,j2g,j3g,js1,js2,js3,u1c)
   limitedExtrapolation(u2, j1g,j2g,j3g,js1,js2,js3,u2c)

   limitedExtrapolation(u2, j1g,j2g,j3g,js1,js2,js3,v1c)
   limitedExtrapolation(u2, j1g,j2g,j3g,js1,js2,js3,v2c)

   limitedExtrapolation(u2, j1g,j2g,j3g,js1,js2,js3,s11c)
   limitedExtrapolation(u2, j1g,j2g,j3g,js1,js2,js3,s12c)
   limitedExtrapolation(u2, j1g,j2g,j3g,js1,js2,js3,s22c)
   u2(j1g,j2g,j3g,s21c)=u2(j1g,j2g,j3g,s12c)


   ! lower bounds: *NOTE*

   ! applyLowerBound( u1(i1g,i2g,i3g,pc),pMin )
   i1g=i1-is1
   i2g=i2-is2
   i3g=i3-is3
   if( u1(i1g,i2g,i3g,pc).lt.pMin )then
      u1(i1g,i2g,i3g,pc)=u1(i1+is1,i2+is2,i3,pc)
   end if

   limitedRhoExtrapolation(u1, i1g,i2g,i3g,is1,is2,is3,rc, u1(i1+is1,i2+is2,i3+is3,rc))
   if( u1(i1g,i2g,i3g,rc).lt.rhoMin )then
      u1(i1g,i2g,i3g,rc)=u1(i1+is1,i2+is2,i3,rc)
   end if

   i1g=i1-2*is1
   i2g=i2-2*is2
   i3g=i3-2*is3
   ! applyLowerBound( u1(i1g,i2g,i3g,pc),pMin )
   if( u1(i1g,i2g,i3g,pc).lt.pMin )then
      u1(i1g,i2g,i3g,pc)=u1(i1+2*is1,i2+2*is2,i3,pc)
   end if

   limitedRhoExtrapolation(u1, i1g,i2g,i3g,is1,is2,is3,rc, u1(i1+2*is1,i2+2*is2,i3+2*is3,rc))
   if( u1(i1g,i2g,i3g,rc).lt.rhoMin )then
      u1(i1g,i2g,i3g,rc)=u1(i1+2*is1,i2+2*is2,i3,rc)
   end if

   if( u2(j1g,j2g,j3g,s11c).gt.pOffset )then  ! fix me --------------
      u2(j1g,j2g,j3g,s11c)=pOffset
   end if

 endIJLoops2d()
#endMacro


! =================================================================================
!   Assign exact values to two ghost lines for testing 
! =================================================================================
#beginMacro assignExactAtTwoGhostMacro()
 beginIJGhostLoops2d()

   ! ----------- first ghost line ---------------
   i1g=i1-is1
   i2g=i2-is2
   i3g=i3-is3

   call ogDeriv(ep1,0,0,0,0,xy1(i1g,i2g,i3g,0),xy1(i1g,i2g,i3g,1),0.,t,rc,rhoe)
   call ogDeriv(ep1,0,0,0,0,xy1(i1g,i2g,i3g,0),xy1(i1g,i2g,i3g,1),0.,t,uc,v1fe)
   call ogDeriv(ep1,0,0,0,0,xy1(i1g,i2g,i3g,0),xy1(i1g,i2g,i3g,1),0.,t,vc,v2fe)
   call ogDeriv(ep1,0,0,0,0,xy1(i1g,i2g,i3g,0),xy1(i1g,i2g,i3g,1),0.,t,tc,tfe)
   u1(i1g,i2g,i3g,rc)=rhoe
   u1(i1g,i2g,i3g,uc)=v1fe
   u1(i1g,i2g,i3g,vc)=v2fe
   u1(i1g,i2g,i3g,pc)=rhoe*tfe

   j1g=j1-js1
   j2g=j2-js2
   j3g=j3-js3

   call ogDeriv(ep2,0,0,0,0,xy2(j1g,j2g,j3g,0),xy2(j1g,j2g,j3g,1),0.,t,u1c,u1se)
   call ogDeriv(ep2,0,0,0,0,xy2(j1g,j2g,j3g,0),xy2(j1g,j2g,j3g,1),0.,t,u2c,u2se)
   call ogDeriv(ep2,0,0,0,0,xy2(j1g,j2g,j3g,0),xy2(j1g,j2g,j3g,1),0.,t,v1c,v1se)
   call ogDeriv(ep2,0,0,0,0,xy2(j1g,j2g,j3g,0),xy2(j1g,j2g,j3g,1),0.,t,v2c,v2se)
   call ogDeriv(ep2,0,0,0,0,xy2(j1g,j2g,j3g,0),xy2(j1g,j2g,j3g,1),0.,t,s11c,s11se)
   call ogDeriv(ep2,0,0,0,0,xy2(j1g,j2g,j3g,0),xy2(j1g,j2g,j3g,1),0.,t,s12c,s12se)
   call ogDeriv(ep2,0,0,0,0,xy2(j1g,j2g,j3g,0),xy2(j1g,j2g,j3g,1),0.,t,s21c,s21se)
   call ogDeriv(ep2,0,0,0,0,xy2(j1g,j2g,j3g,0),xy2(j1g,j2g,j3g,1),0.,t,s22c,s22se)

   u2(j1g,j2g,j3g,u1c)=u1se
   u2(j1g,j2g,j3g,u2c)=u2se
   u2(j1g,j2g,j3g,v1c)=v1se
   u2(j1g,j2g,j3g,v2c)=v2se
   u2(j1g,j2g,j3g,s11c)=s11se
   u2(j1g,j2g,j3g,s12c)=s12se
   u2(j1g,j2g,j3g,s22c)=s22se
   u2(j1g,j2g,j3g,s21c)=s21se

   ! ----------------- second ghost ---------------------
   i1g=i1-2*is1
   i2g=i2-2*is2
   i3g=i3-2*is3

   call ogDeriv(ep1,0,0,0,0,xy1(i1g,i2g,i3g,0),xy1(i1g,i2g,i3g,1),0.,t,rc,rhoe)
   call ogDeriv(ep1,0,0,0,0,xy1(i1g,i2g,i3g,0),xy1(i1g,i2g,i3g,1),0.,t,uc,v1fe)
   call ogDeriv(ep1,0,0,0,0,xy1(i1g,i2g,i3g,0),xy1(i1g,i2g,i3g,1),0.,t,vc,v2fe)
   call ogDeriv(ep1,0,0,0,0,xy1(i1g,i2g,i3g,0),xy1(i1g,i2g,i3g,1),0.,t,tc,tfe)
   u1(i1g,i2g,i3g,rc)=rhoe
   u1(i1g,i2g,i3g,uc)=v1fe
   u1(i1g,i2g,i3g,vc)=v2fe
   u1(i1g,i2g,i3g,pc)=rhoe*tfe

   j1g=j1-2*js1
   j2g=j2-2*js2
   j3g=j3-2*js3

   call ogDeriv(ep2,0,0,0,0,xy2(j1g,j2g,j3g,0),xy2(j1g,j2g,j3g,1),0.,t,u1c,u1se)
   call ogDeriv(ep2,0,0,0,0,xy2(j1g,j2g,j3g,0),xy2(j1g,j2g,j3g,1),0.,t,u2c,u2se)
   call ogDeriv(ep2,0,0,0,0,xy2(j1g,j2g,j3g,0),xy2(j1g,j2g,j3g,1),0.,t,v1c,v1se)
   call ogDeriv(ep2,0,0,0,0,xy2(j1g,j2g,j3g,0),xy2(j1g,j2g,j3g,1),0.,t,v2c,v2se)
   call ogDeriv(ep2,0,0,0,0,xy2(j1g,j2g,j3g,0),xy2(j1g,j2g,j3g,1),0.,t,s11c,s11se)
   call ogDeriv(ep2,0,0,0,0,xy2(j1g,j2g,j3g,0),xy2(j1g,j2g,j3g,1),0.,t,s12c,s12se)
   call ogDeriv(ep2,0,0,0,0,xy2(j1g,j2g,j3g,0),xy2(j1g,j2g,j3g,1),0.,t,s21c,s21se)
   call ogDeriv(ep2,0,0,0,0,xy2(j1g,j2g,j3g,0),xy2(j1g,j2g,j3g,1),0.,t,s22c,s22se)

   u2(j1g,j2g,j3g,u1c)=u1se
   u2(j1g,j2g,j3g,u2c)=u2se
   u2(j1g,j2g,j3g,v1c)=v1se
   u2(j1g,j2g,j3g,v2c)=v2se
   u2(j1g,j2g,j3g,s11c)=s11se
   u2(j1g,j2g,j3g,s12c)=s12se
   u2(j1g,j2g,j3g,s22c)=s22se
   u2(j1g,j2g,j3g,s21c)=s21se

 endIJLoops2d()
#endMacro




      subroutine interfaceCnsSm( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
                               gridIndexRange1, u1, mask1,rsxy1, xy1, gv1, boundaryCondition1, \
                               md1a,md1b,md2a,md2b,md3a,md3b,\
                               gridIndexRange2, u2, mask2,rsxy2, xy2, gv2, boundaryCondition2, \
                               ipar, rpar, pdb1, pdb2,\
                               aa2,aa4,aa8, ipvt2,ipvt4,ipvt8, \
                               ierr )
! ===================================================================================
!  Apply interface conditions for a compressible fluid (CNS) next to an elastic solid (SM)
!
!  gridType : 0=rectangular, 1=curvilinear
!
!  u1: fluid solution 
!  gv1 : grid velocity for the fluid
!
!  u2: solid solution 
!
! NOT used: 
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
      real gv1(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      integer gridIndexRange1(0:1,0:2),boundaryCondition1(0:1,0:2)

      real u2(md1a:md1b,md2a:md2b,md3a:md3b,0:*)
      integer mask2(md1a:md1b,md2a:md2b,md3a:md3b)
      real rsxy2(md1a:md1b,md2a:md2b,md3a:md3b,0:nd-1,0:nd-1)
      real xy2(md1a:md1b,md2a:md2b,md3a:md3b,0:nd-1)
      real gv2(md1a:md1b,md2a:md2b,md3a:md3b,0:*)
      integer gridIndexRange2(0:1,0:2),boundaryCondition2(0:1,0:2)

      integer ipar(0:*)
      real rpar(0:*)

      ! work space arrays that must be saved from call to call:
      real aa2(0:1,0:1,0:1,0:*),aa4(0:3,0:3,0:1,0:*),aa8(0:7,0:7,0:1,0:*)
      integer ipvt2(0:1,0:*), ipvt4(0:3,0:*), ipvt8(0:7,0:*)

      real ogf
      double precision pdb1, pdb2  ! pointer to the parameter data bases

!     --- local variables ----
      
      integer ok,getInt,getReal,getIntCgcns,getIntCgins,getIntCgsm

      integer side1,axis1,grid1,side2,axis2,grid2,gridType,orderOfAccuracy,orderOfExtrapolation,useForcing,\
        tc1,tc2,useWhereMask,debug,axis1p1,axis2p1,nn,n1,n2,np,myid,iofile,normalSign1,normalSign2

      integer rc,uc,vc,wc,tc,pc
      integer u1c,u2c,u3c, v1c,v2c,v3c, s11c,s12c,s13c,s21c,s22c,s23c,s31c,s32c,s33c

      real gamma,pOffset,rhos, mu, lambda 

      real rhof,v1f,v2f,ef,pf,pf0,af,n1f,n2f,vf,zf,zs
      real v1s,v2s,s11s,s12s,s21s,s22s,n1s,n2s,vs,traction1,traction2,ps,vi,pi,cp
      real rhofi,tfi,pif,eif,t1s,t2s,f11,f12,f22

      real v1tf,v2tf,vtf,v1ts,v2ts,vts, sigmatf,v1x,v1y,v2x,v2y, sigma11ts,sigma12ts,sigma22ts,sigmats
      real ws,wf,vDotI,sigmaDotI,v1mg1f,v2mg2f,nDotVGradV

      real cm1,nDotGradp0,nDotGrap
      real divVf,cum1,cvm1,um1,vm1,g,cNormSq,pm1,pNew, s11sNew,s12sNew,s22sNew, v1fNew,v2fNew, v1sNew,v2sNew, u1New

      real r1Norm,r2Norm, nDotDivSigma0,csm,divVf0,vfDotGradp, alpha,g1,g2,deti,v1m,v2m

      integer i1g,i2g,i3g,i1e,i2e,i3e
      integer j1g,j2g,j3g,j1e,j2e,j3e

      integer conservativeVariables,debugFile,interfaceOption,extrapGhost,interfaceProjectionGhostOption

      integer adjSide,numberOfCornerGhost
      integer ng1a,ng1b,ng1c, ng2a,ng2b,ng2c, ng3a,ng3b,ng3c
      integer mg1a,mg1b,mg1c, mg2a,mg2b,mg2c, mg3a,mg3b,mg3c

      real fluid(0:5),solid(0:5), fsr(0:5)


      real dx1(0:2),dr1(0:2),dx2(0:2),dr2(0:2)
      real dx(0:2),dr(0:2)
      real t,ep1,ep2,dt,ktc1,ktc2,kappa1,kappa2
      integer axisp1,axisp2,i1,i2,i3,is1,is2,is3,j1,j2,j3,js1,js2,js3,ks1,ks2,ks3,ls1,ls2,ls3,is,js,it,nit
      integer j1d,j2d,j3d
      integer materialInterfaceOption,option,initialized,twilightZone
      integer id1(0:2),id2(0:2),id3(0:2), jd1(0:2),jd2(0:2),jd3(0:2)
      integer i1p,i2p,i3p, j1p,j2p,j3p, side, ia1,ia2,ia3, ja1,ja2,ja3

      real u1e,u1ex,u1ey,u1ez, u1exx,u1eyy,u1ezz
      real u2e,u2ex,u2ey,u2ez, u2exx,u2eyy,u2ezz

      real rhoe,v1fe,v2fe,v3fe,tfe,pfe
      real u1se,u2se,v1se,v2se,s11se,s12se,s13se,s21se,s22se,s23se,s31se,s32se,s33se, traction1e, traction2e
      real u1sxe,u1sye,u2sxe,u2sye

      integer numGhost
      integer nn1a,nn1b,nn2a,nn2b,nn3a,nn3b
      integer mm1a,mm1b,mm2a,mm2b,mm3a,mm3b

      real du1,du2,du3,uEps,uNorm,cdl, rhoMin,pMin, omega

      real rx1,ry1,rx2,ry2

      real aLap0,aLap1,bLap0,bLap1,aLapX0,aLapX1,bLapY0,bLapY1,cLapX0,cLapX1,dLapY0,dLapY1,aLapSq0,aLapSq1,bLapSq0,bLapSq1
      real a11,a12,a21,a22,det,b0,b1,b2

      real a0,a1,cc0,cc1,d0,d1,dr0,ds0
      real aNormSq,divu,uAve,ktcAve

      real epsRatio,an1,an2,an3,aNorm,ua,ub,nDotU
      real epsx

      real tau1,tau2,tau3,clap1,clap2,ulap1,vlap1,wlap1,ulap2,vlap2,wlap2,an1Cartesian,an2Cartesian
      real ulapSq1,vlapSq1,ulapSq2,vlapSq2,wlapSq1,wlapSq2

      integer np1a,np1b,np2a,np2b,np3a,np3b,diff(0:2)

      real rx,ry,rxx,rxy,ryy,rxxx,rxxy,rxyy,ryyy,rxxxx,rxxyy,ryyyy
      real sx,sy,sxx,sxy,syy,sxxx,sxxy,sxyy,syyy,sxxxx,sxxyy,syyyy

!       real rv1x(0:2),rv1y(0:2),rv1xx(0:2),rv1xy(0:2),rv1yy(0:2),rv1xxx(0:2),rv1xxy(0:2),rv1xyy(0:2),rv1yyy(0:2),\
!            rv1xxxx(0:2),rv1xxyy(0:2),rv1yyyy(0:2)
!       real sv1x(0:2),sv1y(0:2),sv1xx(0:2),sv1xy(0:2),sv1yy(0:2),sv1xxx(0:2),sv1xxy(0:2),sv1xyy(0:2),sv1yyy(0:2),\
!            sv1xxxx(0:2),sv1xxyy(0:2),sv1yyyy(0:2)
!       real rv2x(0:2),rv2y(0:2),rv2xx(0:2),rv2xy(0:2),rv2yy(0:2),rv2xxx(0:2),rv2xxy(0:2),rv2xyy(0:2),rv2yyy(0:2),\
!            rv2xxxx(0:2),rv2xxyy(0:2),rv2yyyy(0:2)
!       real sv2x(0:2),sv2y(0:2),sv2xx(0:2),sv2xy(0:2),sv2yy(0:2),sv2xxx(0:2),sv2xxy(0:2),sv2xyy(0:2),sv2yyy(0:2),\
!            sv2xxxx(0:2),sv2xxyy(0:2),sv2yyyy(0:2)

      integer numberOfEquations,job
      real a2(0:1,0:1),a4(0:3,0:3),a8(0:7,0:7),q(0:11),f(0:11),rcond,work(0:11)
      integer ipvt(0:11)
      real aa(0:1,0:1), ff(0:11), rr(0:11)
      real err
! ** variables used by nonlinear elasticity case

      logical printStuff,useAllSolidNormals,compareWithBill,compareWithExact,printChecks
      real printErrorsAfterTime

      real bk(2,2),tk(2,2),av(2),bv(2),chk(3),t1f,t2f

      real p11tilde,p12tilde
      real u1r,u2r,u1s,u2s,u1r0,u2r0,u1s0,u2s0
      real u1x,u2x,u1y,u2y

      real f11s,f12s,f21s,f22s,aj
      real p11se,p12se,p21se,p22se,pse,pst1e,pst2e,pst1,pst2,pst3

      real du(2,2),p(2,2),dpdf(4,4),cpar(10),pe(2,2)
      integer ideriv,itype

      integer nonlinearProjectionMethod, meth
      real ks,zps,zss,pst

      real determ,du1r,du2r,du1s,du2s

      integer iter,itmax,istop,k,itsm
      real bmax,toler

      ! parameter(toler=1.e-12,itmax=10)
      parameter(toler=1.e-12,itmax=20) ! *wdh* 2015/07/16 -- allow more it's for neo-Hookean

      real trace,discriminant,eval(2),evec(2,2),rad
      real coef1,coef2,det2,beta,sigma1,sigma2

      real aNormif,an1f,an2f,dvn,dsn,dst
      real cf,cs,zp,evpn,evsn,evpt,evst,denom,xi

      real fact,aNormi,v1fi,v2fi,pfi

      real err1(4),u1rc,u1uc,u1vc,u1tc
      real err2(6),u2v1c,u2v2c,u2s11c,u2s12c,u2s21c,u2s22c
      real err1max,err2max

      integer pdeModelCgsm,pdeTypeForGodunovMethodCgsm
      integer pdeVariationCgcns

!**   ! boundary conditions parameters
!**   #Include "bcDefineFortranInclude.h"
 
      integer rectangular,curvilinear
      parameter(\
        rectangular=0,\
        curvilinear=1)


      integer kd,m,n,kd3
      logical extrapTangential
!     real rx,ry,rz,sx,sy,sz,tx,ty,tz
      declareDifferenceNewOrder2(u1,rsxy1,dr1,dx1,RX)
      declareDifferenceNewOrder2(u2,rsxy2,dr2,dx2,RX)

      declareDifferenceNewOrder4(u1,rsxy1,dr1,dx1,RX)
      declareDifferenceNewOrder4(u2,rsxy2,dr2,dx2,RX)

!     --- start statement function ----

      det2(j1,j2,j3)=1.0/(rsxy2(j1,j2,j3,0,0)*rsxy2(j1,j2,j3,1,1)-rsxy2(j1,j2,j3,0,1)*rsxy2(j1,j2,j3,1,0))

!**      extrapTangential(bc) = bc.lt.0
      extrapTangential(m) = .true.

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
      defineDifferenceNewOrder2Components1(u1,rsxy1,dr1,dx1,RX)
      defineDifferenceNewOrder2Components1(u2,rsxy2,dr2,dx2,RX)

      defineDifferenceNewOrder4Components1(u1,rsxy1,dr1,dx1,RX)
      defineDifferenceNewOrder4Components1(u2,rsxy2,dr2,dx2,RX)

!............... end statement functions

      ierr=0

      side1                =ipar( 0)
      axis1                =ipar( 1)
      grid1                =ipar( 2)
      n1a                  =ipar( 3)
      n1b                  =ipar( 4)
      n2a                  =ipar( 5)
      n2b                  =ipar( 6)
      n3a                  =ipar( 7)
      n3b                  =ipar( 8)

      side2                =ipar( 9)
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
      orderOfExtrapolation =ipar(20)
      useForcing           =ipar(21)
      np                   =ipar(22)
      myid                 =ipar(23)
      normalSign1          =ipar(24)  ! for parallel we may flip the sign of the normal
      normalSign2          =ipar(25)  ! for parallel we may flip the sign of the normal
      useWhereMask         =ipar(26)
      debug                =ipar(27)
      nit                  =ipar(28)
      materialInterfaceOption=ipar(29)
      initialized          =ipar(30)
      rc                   =ipar(31)
      uc                   =ipar(32)
      vc                   =ipar(33)
      wc                   =ipar(34)
      tc                   =ipar(35)
      u1c                  =ipar(36)
      u2c                  =ipar(37)
      u3c                  =ipar(38)
      v1c                  =ipar(39)
      v2c                  =ipar(40)
      v3c                  =ipar(41)
      s11c                 =ipar(42)
      s12c                 =ipar(43)
      s13c                 =ipar(44)
      s21c                 =ipar(45)
      s22c                 =ipar(46)
      s23c                 =ipar(47)
      s31c                 =ipar(48)
      s32c                 =ipar(49)
      s33c                 =ipar(50)
      option               =ipar(51) ! 0=set values on the interface, 1=set ghost values 
      twilightZone         =ipar(52)
      interfaceOption      =ipar(53)
      !! interfaceOption=1 ! 1=linear-FSR, 5=non-linear FSR  *************** FIX ME ********** -> interfaceProjectionOption
      interfaceProjectionGhostOption = ipar(54)

      dx1(0)               =rpar(0)
      dx1(1)               =rpar(1)
      dx1(2)               =rpar(2)
      dr1(0)               =rpar(3)
      dr1(1)               =rpar(4)
      dr1(2)               =rpar(5)

      dx2(0)               =rpar(6)
      dx2(1)               =rpar(7)
      dx2(2)               =rpar(8)
      dr2(0)               =rpar(9)
      dr2(1)               =rpar(10)
      dr2(2)               =rpar(11)

      t                    =rpar(12)
      ep1                  =rpar(13) ! pointer for exact solution
      ep2                  =rpar(14) ! pointer for exact solution
      dt                   =rpar(15)
      gamma                =rpar(16)
      pOffset              =rpar(17)
      rhos                 =rpar(18)
      mu                   =rpar(19)
      lambda               =rpar(20)

      debugFile=1 

      if( t.le.dt )then
        write(*,'("interfaceCnsSm: t=",e9.2,", interfaceOption=",i2," pOffset=",e9.2)') t,interfaceOption,pOffset
      end if

      ! write(*,'("interfaceCnsSm: t=",e9.2,", option=",i4)') t,option
      ! if( option.eq.1 )then
      !   return
      ! end if

      ! -- look up additional parameters here --

      ! Cgsm: Solid parameters:
      !   pdeModel = 0 : linear elasticity
      !            = 1 : nonlinear model
      !   pdeTypeForGodunovMethod = 0 : linear elasticity
      !                           = 1 : non-linear elasticity solver but in linear mode 
      !                           = 2 : SVK model 
      getIntParameterCgsm(pdb2,'pdeModel',pdeModelCgsm) ! for solid
      ! write(*,'(" interfaceCnsSm: Cgsm: pdeModel=",i3)') pdeModelCgsm

      getIntParameterCgsm(pdb2,'pdeTypeForGodunovMethod',pdeTypeForGodunovMethodCgsm) ! for solid
      ! write(*,'(" interfaceCnsSm: Cgsm: pdeTypeForGodunovMethod=",i3)') pdeTypeForGodunovMethodCgsm

      ! Fluid parameters:
      getIntParameterCgcns(pdb1,'pdeVariation',pdeVariationCgcns) ! for fluid
      ! write(*,'(" interfaceCnsSm: Cgcns: pdeVariation=",i3)') pdeVariationCgcns


      pc=tc ! store p here
      cp = sqrt((lambda+2.*mu)/rhos)
      zs = rhos*cp ! solid impedance

      kd3=min(nd-1,2)  ! for indexing xy
      
      ! kkc 080516 
      ktcAve = ktc1+ktc2

      ! iofile = file for debug output
      if( np.eq.1 )then
        iofile=6
      else
        iofile=10+myid
      end if

      if( t.le.2.*dt )then

      
        write(*,'("interfaceCnsSm: t=",e10.2," (rc,uc,vc,tc)=(",4i2,") (v1c,v2c,s11c,s12c,s21c,s22c)=(",6i3,")")') \
             t,rc,uc,vc,tc,v1c,v2c,s11c,s12c,s21c,s22c
        write(*,'("              : dt,gamma,pOffset,rhos,mu,lambda=",6e10.2,")")') dt,gamma,pOffset,rhos,mu,lambda
        write(*,'("              : solid imped: zs=",e10.2," interfaceOption=",i3)') zs,interfaceOption

        ! stop 1234
      end if



      if( debug.gt.3 )then
        write(iofile,'(" interfaceCnsSm: ktc1,ktc2=",2f10.5," kappa1,kappa2=",2e10.2," gridType,tc1,tc2=",3i2)') ktc1,ktc2,kappa1,kappa2,gridType,tc1,tc2
           ! '
      end if

      if( nit.lt.0 .or. nit.gt.100 )then
        write(*,'(" interfaceBC: ERROR: nit=",i9)') nit
        nit=max(1,min(100,nit))
      end if

      if( debug.gt.3 )then
        write(iofile,'(" interfaceCnsSm: **START** grid1=",i4," side1,axis1=",2i2)') grid1,side1,axis1
           ! '
        write(iofile,'(" interfaceCnsSm: **START** grid2=",i4," side2,axis2=",2i2)') grid2,side2,axis2
           ! '
        write(iofile,'("n1a,n1b,...=",6i5)') n1a,n1b,n2a,n2b,n3a,n3b
        write(iofile,'("m1a,m1b,...=",6i5)') m1a,m1b,m2a,m2b,m3a,m3b
        write(iofile,'("dr1,dr2=",6e9.2)') dr1(0),dr1(1),dr1(2),dr2(0),dr2(1),dr2(2)

      end if
      if( debug.gt.7 )then
      write(iofile,*) 'u1=',((((u1(i1,i2,i3,m),m=0,2),i1=n1a,n1b),i2=n2a,n2b),i3=n3a,n3b)
      write(iofile,*) 'u2=',((((u2(i1,i2,i3,m),m=0,2),i1=m1a,m1b),i2=m2a,m2b),i3=m3a,m3b)

      end if
     
      ! *** do this for now --- assume grids have equal spacing
      dx(0)=dx1(0)
      dx(1)=dx1(1)
      dx(2)=dx1(2)

      dr(0)=dr1(0)
      dr(1)=dr1(1)
      dr(2)=dr1(2)

      epsx=1.e-20  ! fix this 

      uEps=1.e-4 ! for scaling in the limited extrapolation -- fix me --
      ! rhoMin=1.e-5
      rhoMin=1.e-4
      pMin=1.e-8 


      numGhost=orderOfAccuracy/2


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
      axis2p1=mod(axis2+1,nd)

      is1=0
      is2=0
      is3=0

      do m=0,2
        id1(m)=0
        id2(m)=0
        id3(m)=0
        jd1(m)=0
        jd2(m)=0
        jd3(m)=0
      end do

      if( axis1.eq.0 ) then
        is1=1-2*side1
        id1(axis1)=is1
        ! include ghost lines in tangential directions (for extrapolating) :
        if( extrapTangential(boundaryCondition1(0,1)) )then 
          nn2a=nn2a-numGhost
          nn2b=nn2b+numGhost
        end if
        if( nd.eq.3 .and. extrapTangential(boundaryCondition1(0,2)) )then
          nn3a=nn3a-numGhost
          nn3b=nn3b+numGhost
        end if
      else if( axis1.eq.1 )then
        is2=1-2*side1
        id2(axis1)=is2
        if( extrapTangential(boundaryCondition1(0,0)) )then
          nn1a=nn1a-numGhost
          nn1b=nn1b+numGhost
        end if
        if( nd.eq.3 .and. extrapTangential(boundaryCondition1(0,2)) )then
          nn3a=nn3a-numGhost
          nn3b=nn3b+numGhost
        end if
      else if( axis1.eq.2 )then
        is3=1-2*side1
        id3(axis1)=is3
        if( extrapTangential(boundaryCondition1(0,0)) )then
          nn1a=nn1a-numGhost
          nn1b=nn1b+numGhost
        end if
        if( extrapTangential(boundaryCondition1(0,1)) )then
          nn2a=nn2a-numGhost
          nn2b=nn2b+numGhost
        end if
      else
        ! invalid value for axis1
        stop 1143
      end if


      js1=0
      js2=0
      js3=0
      if( axis2.eq.0 ) then
        js1=1-2*side2
        jd1(axis2)=js1
        if( extrapTangential(boundaryCondition2(0,1)) )then
          mm2a=mm2a-numGhost
          mm2b=mm2b+numGhost
        end if
        if( nd.eq.3 .and. extrapTangential(boundaryCondition2(0,2)) )then
          mm3a=mm3a-numGhost
          mm3b=mm3b+numGhost
        end if
      else if( axis2.eq.1 ) then
        js2=1-2*side2
        jd2(axis2)=js2
        if( extrapTangential(boundaryCondition2(0,0)) )then
          mm1a=mm1a-numGhost
          mm1b=mm1b+numGhost
        end if
        if( nd.eq.3 .and. extrapTangential(boundaryCondition2(0,2)) )then
          mm3a=mm3a-numGhost
          mm3b=mm3b+numGhost
        end if
      else if( axis2.eq.2 ) then
        js3=1-2*side2
        jd3(axis2)=js3
        if( extrapTangential(boundaryCondition2(0,0)) )then
          mm1a=mm1a-numGhost
          mm1b=mm1b+numGhost
        end if
        if( extrapTangential(boundaryCondition2(0,1)) )then
          mm2a=mm2a-numGhost
          mm2b=mm2b+numGhost
        end if
      else
        ! invalid value for axis2
        stop 1144
      end if

      is=1-2*side1
      js=1-2*side2


      if( debug.gt.3 )then
        write(iofile,'("nn1a,nn1b,...=",6i5)') nn1a,nn1b,nn2a,nn2b,nn3a,nn3b
        write(iofile,'("mm1a,mm1b,...=",6i5)') mm1a,mm1b,mm2a,mm2b,mm3a,mm3b

      end if

      if( option.eq.0 )then
        conservativeVariables=1
      else 
        conservativeVariables=0
      endif

      ! write(*,'("interfaceCnsSm: t=",e10.3,", conservativeVariables=",i2," normalSign1,normalSign2=",2i3)') t,conservativeVariables,normalSign1,normalSign2

      if( nd.eq.2 )then

        ! *********************************** 
        ! **************** 2D ***************
        ! *********************************** 

       ! write(6,*)'here i am'
       ! pause

       if( orderOfAccuracy.eq.2 .and. gridType.eq.rectangular )then
        ! we do not implement this case
        stop 8904

       else if( orderOfAccuracy.eq.2 .and. gridType.eq.curvilinear )then



         if( option.eq.0 )then
         ! ====================================================================
         ! ============== Stage I - set interface values  =====================
         ! ====================================================================

         ! write(6,*)'Stage I here i am'
         ! pause

         ! Solve interface jump conditions:
         !    [ n.v ] = 0
         !    [ n.sigma ] = 0
 
          ! -- optionally smooth interface values before projection ---
          do itsm=1,2 ! fix me -- 
            smoothInterfaceValues()
          end do

          if( pdeTypeForGodunovMethodCgsm.le.1 )then  ! this is the original code for linear elasticity

           ! Note: pdeTypeForGodunovMethodCgsm=0 : linear elasticity code
           !                                  =1 : SVK code but run in linear mode
           projectInterfaceValuesLinearElasticity()

          else   ! this is the new code that assumes a general solid mechanics model (such as SVK)

           projectInterfaceValuesNonLinearElasticity()

          end if  ! choice of solid mechanics model

         end if  ! end if option==0 



         if( option.eq.1 )then
         ! =================================================================
         ! ============== Stage II - set ghost values  =====================
         ! =================================================================

          ! write(6,*)'Here I am : option=1'
          ! pause

           ! -- Assume we always have primitive variables at this stage --
           if( conservativeVariables.ne.0 )then
             stop 1887
           end if

          extrapGhost=1
          if( extrapGhost.eq.1 )then
            ! wdh 110521 : try this  -- assign corner ghost assumes that the ghost points have good values
            extrapTwoGhostMacro()
          end if

          ! assign corner ghost points here to set the values on the extended boundary 
          if( extrapGhost.eq.1 )then
           assignCornerGhostPoints()
          end if

          ! convert fluid variables near the boundary to (rho,u,v,p)
          ! do enough points so we can take a derivative on the boundary or extrapolate the ghost pts
          if( conservativeVariables.eq.1 )then
            stop 1887
            ! beginIJGhostLoops2d()
            !   convertToPrim( i1,i2,i3 )
            !   convertToPrim( i1+is1,i2+is2,i3 )
            !   convertToPrim( i1+2*is1,i2+2*is2,i3 )
            ! endIJLoops2d()
          else
            ! convert T to p 
            beginIJGhostLoops2d()
             do m=-2,2
              u1(i1+m*is1,i2+m*is2,i3,tc)=u1(i1+m*is1,i2+m*is2,i3,rc)*u1(i1+m*is1,i2+m*is2,i3,tc)  ! p =rho*T 
             end do
            endIJLoops2d()
          end if
 
          ! === The fluid variables are now (rho,u,v,p) ====
 

          ! extrap ghost points -- the next loop includes extra points in the tangential directions
          if(  extrapGhost.eq.1 )then
            ! NOTE: first extrap both lines before applying lower bounds
            extrapAndBoundMacro()
          end if
 
          ! interfaceProjectionGhostOption = 0 : extrapolate ghost
          !                                = 1 : compute ghost from compatibility
          !                                = 2 : use exact values for TZ
          if( interfaceProjectionGhostOption.eq.0 )then

            write(*,'(" Interface: extrapolate ghost")') 
    
          else if( interfaceProjectionGhostOption.eq.2 )then
            ! SET EXACT GHOST VALUES FOR TESTING
            if( twilightZone.eq.0 )then
              ! TZ should be on with this option
              stop 44088
            end if
            assignExactAtTwoGhostMacro()

          else if( interfaceProjectionGhostOption.eq.1 )then 

            write(*,'(" Interface: compute ghost from compatibility")') 

            ! -------------------------------------------------
            ! ---- Assign ghost points from compatibility -----
            ! -------------------------------------------------

            ! Use the time derivatives of the jump conditions to derive equations for 
            ! the ghost values of the velocity and stress
            !   [ n.v_t ] = 0
            !   [ n.sigma_t ] = 0 
            !

            assignGhostFromCompatibility()


          else 

            ! Unknown interfaceProjectionGhostOption
            write(*,'("ERROR interfaceProjectionGhostOption=",i6)') interfaceProjectionGhostOption
            stop 7290

          end if


          ! extrap 2nd ghost -- do this for now
         if( .false. )then
          beginIJGhostLoops2d()

            i1g=i1-2*is1
            i2g=i2-2*is2
            i3g=i3-2*is3
            i1e=i1-is1
            i2e=i2-is2
            i3e=i3-is3
 
            j1g=j1-2*js1
            j2g=j2-2*js2
            j3g=j3-2*js3
            j1e=j1-js1
            j2e=j2-js2
            j3e=j3-js3

            ! u1(i1g,i2g,i3g,rc)=extrap3(u1,i1e,i2e,i3e,rc,is1,is2,is3)
            limitedRhoExtrapolation(u1, i1g,i2g,i3g,is1,is2,is3,rc, u1(i1+2*is1,i2+2*is2,i3+2*is3,rc))
            applyLowerBound( u1(i1g,i2g,i3g,rc),rhoMin )

            u1(i1g,i2g,i3g,uc)=extrap3(u1,i1e,i2e,i3e,uc,is1,is2,is3)
            u1(i1g,i2g,i3g,vc)=extrap3(u1,i1e,i2e,i3e,vc,is1,is2,is3)
            ! u1(i1g,i2g,i3g,pc)=extrap3(u1,i1e,i2e,i3e,pc,is1,is2,is3)
            limitedExtrapolation(u1, i1g,i2g,i3g,is1,is2,is3,pc )
            applyLowerBound( u1(i1g,i2g,i3g,pc),pMin )
 
            u2(j1g,j2g,j3g,u1c)=extrap3(u2,j1e,j2e,j3e,u1c,js1,js2,js3)
            u2(j1g,j2g,j3g,u2c)=extrap3(u2,j1e,j2e,j3e,u2c,js1,js2,js3)

            u2(j1g,j2g,j3g,v1c)=extrap3(u2,j1e,j2e,j3e,v1c,js1,js2,js3)
            u2(j1g,j2g,j3g,v2c)=extrap3(u2,j1e,j2e,j3e,v2c,js1,js2,js3)
 
            u2(j1g,j2g,j3g,s11c)=extrap3(u2,j1e,j2e,j3e,s11c,js1,js2,js3)
            u2(j1g,j2g,j3g,s12c)=extrap3(u2,j1e,j2e,j3e,s12c,js1,js2,js3)
            u2(j1g,j2g,j3g,s22c)=extrap3(u2,j1e,j2e,j3e,s22c,js1,js2,js3)
            u2(j1g,j2g,j3g,s21c)=u2(j1g,j2g,j3g,s12c)

            if( u2(j1g,j2g,j3g,s11c).gt.pOffset )then  ! fix me --------------
              u2(j1g,j2g,j3g,s11c)=pOffset
            end if

          endIJLoops2d()
         end if
     
         if (.false.) then

           ! itype=1          ! linear elasticity
            itype=2          ! SVK
            if (itype.gt.0) then
               ! write(6,*)'here i am'
               ! pause
               beginIJLoops2d()

               ! compute p11tilde and p12tilde
               p11tilde=rsxy2(j1,j2,j3,axis2,0)*u2(j1,j2,j3,s11c)+rsxy2(j1,j2,j3,axis2,1)*u2(j1,j2,j3,s21c)
               p12tilde=rsxy2(j1,j2,j3,axis2,0)*u2(j1,j2,j3,s12c)+rsxy2(j1,j2,j3,axis2,1)*u2(j1,j2,j3,s22c)

               if( axis2.eq.0 )then
                  ! determine components of the deformation gradient tensor (iterate on u1r,u2r so that these agree with P11tilde,P12tilde)
                  u1s=(u2(j1,j2+1,j3,u1c)-u2(j1,j2-1,j3,u1c))/(2.0*dr2(1))
                  u2s=(u2(j1,j2+1,j3,u2c)-u2(j1,j2-1,j3,u2c))/(2.0*dr2(1))
                  ! initialize
                  u1r0=js*(u2(j1+js1,j2,j3,u1c)-u2(j1,j2,j3,u1c))/dr2(0)
                  u2r0=js*(u2(j1+js1,j2,j3,u2c)-u2(j1,j2,j3,u2c))/dr2(0)
                  u1r=u1r0
                  u2r=u2r0
               else
                  ! determine components of the deformation gradient tensor (iterate on u1s,u2s so that these agree with P11tilde,P12tilde)
                  u1r=(u2(j1+1,j2,j3,u1c)-u2(j1-1,j2,j3,u1c))/(2.0*dr2(0))
                  u2r=(u2(j1+1,j2,j3,u2c)-u2(j1-1,j2,j3,u2c))/(2.0*dr2(0))
                  ! initialize
                  u1s0=js*(u2(j1,j2+js2,j3,u1c)-u2(j1,j2,j3,u1c))/dr2(1)
                  u2s0=js*(u2(j1,j2+js2,j3,u2c)-u2(j1,j2,j3,u2c))/dr2(1)
                  u1s=u1s0
                  u2s=u2s0
               end if
               
               ! Newton iteration for u1r,u2r (axis2=0) or u1s,u2s (axis2=1)
               iter=1
               istop=0
               bmax=10.*toler
               do while (bmax.gt.toler)

                  u1x=rsxy2(j1,j2,j3,0,0)*u1r+rsxy2(j1,j2,j3,1,0)*u1s
                  u1y=rsxy2(j1,j2,j3,0,1)*u1r+rsxy2(j1,j2,j3,1,1)*u1s
                  u2x=rsxy2(j1,j2,j3,0,0)*u2r+rsxy2(j1,j2,j3,1,0)*u2s
                  u2y=rsxy2(j1,j2,j3,0,1)*u2r+rsxy2(j1,j2,j3,1,1)*u2s

                  ! compute stress and the deriv based on current deformation gradient
                  du(1,1)=u1x
                  du(1,2)=u1y
                  du(2,1)=u2x
                  du(2,2)=u2y
                  cpar(1)=lambda   ! Lame constants
                  cpar(2)=mu
                  ideriv=1         ! compute dpdf
                  call smgetdp (du,p,dpdf,cpar,ideriv,itype)

                  !  construct linear system
                  b1=rsxy2(j1,j2,j3,axis2,0)*p(1,1)+rsxy2(j1,j2,j3,axis2,1)*p(2,1)-p11tilde
                  b2=rsxy2(j1,j2,j3,axis2,0)*p(1,2)+rsxy2(j1,j2,j3,axis2,1)*p(2,2)-p12tilde
                  a11= rsxy2(j1,j2,j3,axis2,0)*(dpdf(1,1)*rsxy2(j1,j2,j3,axis2,0)+dpdf(1,2)*rsxy2(j1,j2,j3,axis2,1)) \
                  +rsxy2(j1,j2,j3,axis2,1)*(dpdf(3,1)*rsxy2(j1,j2,j3,axis2,0)+dpdf(3,2)*rsxy2(j1,j2,j3,axis2,1))
                  a12= rsxy2(j1,j2,j3,axis2,0)*(dpdf(1,3)*rsxy2(j1,j2,j3,axis2,0)+dpdf(1,4)*rsxy2(j1,j2,j3,axis2,1)) \
                  +rsxy2(j1,j2,j3,axis2,1)*(dpdf(3,3)*rsxy2(j1,j2,j3,axis2,0)+dpdf(3,4)*rsxy2(j1,j2,j3,axis2,1))
                  a21= rsxy2(j1,j2,j3,axis2,0)*(dpdf(2,1)*rsxy2(j1,j2,j3,axis2,0)+dpdf(2,2)*rsxy2(j1,j2,j3,axis2,1)) \
                  +rsxy2(j1,j2,j3,axis2,1)*(dpdf(4,1)*rsxy2(j1,j2,j3,axis2,0)+dpdf(4,2)*rsxy2(j1,j2,j3,axis2,1))
                  a22= rsxy2(j1,j2,j3,axis2,0)*(dpdf(2,3)*rsxy2(j1,j2,j3,axis2,0)+dpdf(2,4)*rsxy2(j1,j2,j3,axis2,1)) \
                  +rsxy2(j1,j2,j3,axis2,1)*(dpdf(4,3)*rsxy2(j1,j2,j3,axis2,0)+dpdf(4,4)*rsxy2(j1,j2,j3,axis2,1))

                  ! solve the 2x2 system
                  determ=a11*a22-a12*a21
                  du1=(b1*a22-b2*a12)/determ
                  du2=(b2*a11-b1*a21)/determ

                  ! compute max residual of the stress condition and update
                  bmax=max(abs(b1),abs(b2))/lambda

                  if (istop.ne.0) then
                     ! write(6,*)p(1,1),p(1,2),p(2,1),p(2,2)
                     ! write(6,*)a11,a12,a21,a22,b1,b2
                     write(6,'(1x,i2,3(1x,1pe15.8))')iter,du1,du2,bmax
                     ! write(6,*)axis2
                  end if

                  iter=iter+1
                  if( axis2.eq.0 )then
                     u1r=u1r-du1
                     u2r=u2r-du2
                     ! check for convergence
                     if (iter.gt.itmax) then
                        write(6,*)'Error (interface code) : Newton failed to converge'
                        if (istop.eq.0) then
                           iter=1
                           istop=1
                           u1r=u1r0
                           u2r=u2r0
                        else
                           stop 8887
                        end if
                     end if
                  else
                     u1s=u1s-du1
                     u2s=u2s-du2
                     ! check for convergence
                     if (iter.gt.itmax) then
                        write(6,*)'Error (interface code) : Newton failed to converge'
                        if (istop.eq.0) then
                           iter=1
                           istop=1
                           u1s=u1s0
                           u2s=u2s0
                        else
                           stop 8897
                        end if
                     end if
                  end if

               end do

               ! Note to myself: is=1 if side=0 (case 1 in my notes)

               ! compute components of n.sigma = beta * n0.P , where n and n0 are inward normals to the solid
               ! set displacement in the ghost point (provisional values)
               if( axis2.eq.0 )then
                  u2(j1-js1,j2,j3,u1c)=u2(j1+js1,j2,j3,u1c)-2.*js*dr2(0)*u1r
                  u2(j1-js1,j2,j3,u2c)=u2(j1+js1,j2,j3,u2c)-2.*js*dr2(0)*u2r
               else
                  u2(j1,j2-js2,j3,u1c)=u2(j1,j2+js2,j3,u1c)-2.*js*dr2(1)*u1s
                  u2(j1,j2-js2,j3,u2c)=u2(j1,j2+js2,j3,u2c)-2.*js*dr2(1)*u2s
               end if

               endIJLoops2d()
            end if

         end if

          if( .false. )then
           write(debugFile,'(" ")')
           i1=2
           i3=0
           do m=0,3
             write(debugFile,'("t=",f8.4," fluid=[",13(f7.4,1x),"]")') t, (u1(i1,i2,i3,m),i2=n2a-2,min(n2a+10,nd2b))
           end do
           j2=2
           j3=0
           do m=0,7
             write(debugFile,'("t=",f8.4," solid=[",13(f7.4,1x),"]")') t, (u2(j1,j2,j3,m),j1=max(md1a,m1b-10),m1b+2)
           end do
          end if



          ! convert fluid variables near the boundary back to conservative
          if( conservativeVariables.eq.1 )then
            beginIJGhostLoops2d()
              convertToCons( i1-2*is1,i2-2*is2,i3 )
              convertToCons( i1-is1,i2-is2,i3 )
              convertToCons( i1,i2,i3 )
              convertToCons( i1+is1,i2+is2,i3 )
              convertToCons( i1+2*is1,i2+2*is2,i3 )
            endIJLoops2d()
          else
            ! convert p to T 
            beginIJGhostLoops2d()
             do m=-2,2
              u1(i1+m*is1,i2+m*is2,i3,tc)=u1(i1+m*is1,i2+m*is2,i3,tc)/u1(i1+m*is1,i2+m*is2,i3,rc)  ! T=p/rho
             end do
            endIJLoops2d()
          end if


          if( .true. )then
           assignCornerGhostPoints()
          end if

        end if ! end if option == 1



       else 
  
         ! We should never get here 
         stop 1674


       end if
      end if



      return
      end
