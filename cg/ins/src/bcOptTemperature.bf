! ==================================================================================
!   Assign Temperature Boundary Conditions for INS + Boussinesq 
!
! *wdh* 110311
! ==================================================================================

! These next include files will define the macros that will define the difference approximations
! The actual macro is called below
#Include "defineDiffNewerOrder2f.h"
#Include "defineDiffNewerOrder4f.h"


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



! ************************************************************************************************
!  This macro is used for looping over the faces of a grid to assign booundary conditions
!
! extra: extra points to assign
!          Case 1: extra=numberOfGhostPoints -- for assigning extended boundaries
!          Case 2: extra=-1 -- for assigning ghost points but not including extended boundaries
! numberOfGhostPoints : number of ghost points (1 for 2nd order, 2 for fourth-order ...)
!
!
! Output:
!  n1a,n1b,n2a,n2b,n3a,n3b : from gridIndexRange
!  nn1a,nn1b,nn2a,nn2b,nn3a,nn3b : includes "extra" points
! 
! ***********************************************************************************************
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

     if( debug.gt.7 )then
       write(*,'(" bcOptT: grid,side,axis=",3i3,", \
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



! =================================================================================
!   Assign values in the corners in 2D (see bcMaxwellCorners.bf)
!
!  Set the normal component of the solution on the extended boundaries (points N in figure)
!  Set the corner points "C" 
!              |
!              X
!              |
!        N--N--X--X----
!              |
!        C  C  N
!              |
!        C  C  N
!
! ORDER: 2 or 4
! GRIDTYPE: rectangular, curvilinear
! FORCING: none, twilightZone
! =================================================================================
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





! =========================================================================
! Compute the normal on a curvilinear grid.
!
! Assumes is=1-2*side is defined. 
! =========================================================================
#beginMacro getNormal(j1,j2,j3)
    an1 = rsxy(j1,j2,j3,axis,0)
    an2 = rsxy(j1,j2,j3,axis,1)
    if( nd.eq.2 )then
     aNormi = (-is)/sqrt(an1**2+an2**2)
     an1=an1*aNormi
     an2=an2*aNormi
    else
     an3 = rsxy(j1,j2,j3,axis,2)
     aNormi = (-is)/sqrt(an1**2+an2**2+an3**2)
     an1=an1*aNormi
     an2=an2*aNormi
     an3=an3*aNormi
    end if
#endMacro



      subroutine bcOptTemperature( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
                          gridIndexRange, dimRange, isPeriodic, u, mask,rsxy, xy, boundaryCondition, \
                          addBoundaryForcing, interfaceType, ndb, bcData,  \
                          dim, bcf0, bcOffset, ipar, rpar, ierr )
! ===================================================================================
!  Boundary conditions for solid mechanics
!
!  gridType : 0=rectangular, 1=curvilinear
!
!  c2= mu/rho, c1=(mu+lambda)/rho;
! 
! The forcing for the boundary conditions can be accessed using the statement function:
!         bcf(side,axis,i1,i2,i3,m)
! which is defined below. 
! ===================================================================================

      implicit none

      integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, ndb, ierr

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      integer gridIndexRange(0:1,0:2),boundaryCondition(0:1,0:2), dimRange(0:1,0:2), isPeriodic(0:*)

      integer addBoundaryForcing(0:1,0:2)
      integer interfaceType(0:1,0:2,0:*)
      integer dim(0:1,0:2,0:1,0:2)

      real bcf0(0:*)
      integer*8 bcOffset(0:1,0:2)

      real bcData(0:ndb-1,0:1,0:nd-1,0:*)

      integer ipar(0:*)
      real rpar(0:*)

!     --- local variables ----
      
      integer pc,uc,vc,wc,sc,tc,numberOfComponents
      integer grid,gridType,orderOfAccuracy,gridIsMoving,useWhereMask,gridIsImplicit,implicitMethod
      integer implicitOption,isAxisymmetric,use2ndOrderAD,use4thOrderAD,twilightZone,numberOfProcessors
      integer outflowOption,orderOfExtrapolationForOutflow,debug,myid,assignTemperature,assignTwilightZone

      real nu,t,ad21,ad22,ad41,ad42,nuPassiveScalar,adcPassiveScalar,ajs,thermalExpansivity,epsx,REAL_MIN 
      real ep
      real a0,a1,an1,an2,an3,aNormi, t1,t2,t3
      real dx(0:2),dr(0:2),gravity(0:2)

      real dxn,b0,b1,te,tex,tey,tez,ff,urv(0:2),ur0



      integer side,axis,axisp1,axisp2,i1,i2,i3,is1,is2,is3,j1,j2,j3,js1,js2,js3,k1,k2,k3,ks1,ks2,ks3,is,js

      integer numGhost,numberOfGhostPoints
      integer side1,side2,side3
      integer n1a,n1b,n2a,n2b,n3a,n3b
      integer nn1a,nn1b,nn2a,nn2b,nn3a,nn3b
      integer extra1a,extra1b,extra2a,extra2b,extra3a,extra3b

      integer cornerBC(0:2,0:2,0:2), iparc(0:10), orderOfExtrapolationForCorners
      real rparc(0:10)


      ! boundary conditions parameters and interfaceType values
      #Include "bcDefineFortran.h"
 
      integer rectangular,curvilinear
      parameter(\
        rectangular=0,\
        curvilinear=1)

      ! Temperature boundary conditions:
      integer tbc(2,3)
      integer dirichlet,neumann,extrapolate,evenSymmetry
      parameter( dirichlet=1, neumann=2, extrapolate=3, evenSymmetry=4  )

!     --- start statement function ----
      real bcf,mixedRHS,mixedCoeff,mixedNormalCoeff
      integer kd,m,n,component
      real uxOneSided
!     real rx,ry,rz,sx,sy,sz,tx,ty,tz
      declareDifferenceNewOrder2(u,rsxy,dr,dx,RX)

      declareDifferenceNewOrder4(u,rsxy,dr,dx,RX)

!     The next macro call will define the difference approximation statement functions
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

      mixedRHS(component,side,axis,grid) = bcData(component+numberOfComponents*(0),side,axis,grid)
      mixedCoeff(component,side,axis,grid) = bcData(component+numberOfComponents*(1),side,axis,grid)
      mixedNormalCoeff(component,side,axis,grid) =  bcData(component+numberOfComponents*(2),side,axis,grid)

!............... end statement functions

      ierr=0

      pc                =ipar(0)
      uc                =ipar(1)
      vc                =ipar(2)
      wc                =ipar(3)
      sc                =ipar(4)
      grid              =ipar(5)
      gridType          =ipar(6)
      orderOfAccuracy   =ipar(7)
      gridIsMoving      =ipar(8)
      useWhereMask      =ipar(9)
      gridIsImplicit    =ipar(10)
      implicitMethod    =ipar(11)
      implicitOption    =ipar(12)
      isAxisymmetric    =ipar(13)
      use2ndOrderAD     =ipar(14)
      use4thOrderAD     =ipar(15)
      twilightZone      =ipar(16)
      numberOfProcessors=ipar(17)
      outflowOption     =ipar(18)
      orderOfExtrapolationForOutflow=ipar(19)
      debug             =ipar(20)
      myid              =ipar(21)
      assignTemperature =ipar(22)
      tc                =ipar(23)

      numberOfComponents=ipar(24) ! new *wdh* 110311 - for bcData 

      dx(0)             =rpar(0)
      dx(1)             =rpar(1)
      dx(2)             =rpar(2)
      dr(0)             =rpar(3)
      dr(1)             =rpar(4)
      dr(2)             =rpar(5)
      nu                =rpar(6)
      t                 =rpar(7)
      ad21              =rpar(8)
      ad22              =rpar(9)
      ad41              =rpar(10)
      ad42              =rpar(11)
      nuPassiveScalar   =rpar(12)
      adcPassiveScalar  =rpar(13)
      ajs               =rpar(14)
      gravity(0)        =rpar(15)
      gravity(1)        =rpar(16)
      gravity(2)        =rpar(17)
      thermalExpansivity=rpar(18)

      ep                =rpar(19) ! pointer for exact solution -- new : 110311 
      REAL_MIN          =rpar(20)

      if( t.le.0. .and. debug.gt.3 )then

        write(*,'(" bcOptT: grid=",i4," gridType=",i2," orderOfAccuracy=",i2," assignTemperature,tc=",2i3)') grid,gridType,orderOfAccuracy,assignTemperature,tc

        write(*,'(" bcOptT: gravity=",3f8.3," thermalExpansivity=",e10.2)') gravity(0),gravity(1),gravity(2),thermalExpansivity
        write(*,'(" bcOptT: REAL_MIN=",e10.2)') REAL_MIN
      end if

      if( debug.gt.7 )then
       write(*,'(" bcOptT: **START** grid=",i4," uc,vc,wc=",3i2)') grid,uc,vc,wc
       n1a=gridIndexRange(0,0)
       n1b=gridIndexRange(1,0)
       n2a=gridIndexRange(0,1)
       n2b=gridIndexRange(1,1)
       n3a=gridIndexRange(0,2)
       n3b=gridIndexRange(1,2)
       write(*,'(" bcOptT: grid=",i3,",n1a,n1b,n2a,n2b,n3a,n3b=",6i3)') grid,n1a,n1b,n2a,n2b,n3a,n3b
       ! write(*,*) 'bcOptT: u=',((((u(i1,i2,i3,m),m=0,nd-1),i1=n1a,n1b),i2=n2a,n2b),i3=n3a,n3b)
       ! write(*,*) 'bcOffset:',bcOffset
      end if
     

      epsx=REAL_MIN*100.

      if( orderOfAccuracy.ne.4 )then
        write(*,'("bcOptT:ERROR: orderOfAccuracy.ne.4")')
        stop 1111
      end if

      numGhost=orderOfAccuracy/2


      ! -- Determine the type of Temperature BC for each face:
      !    tbc(side,axis) = -1          : periodic
      !                   =  0          : interpolation
      !                   = dirichlet
      !                   = neumann
      !                   = extrapolate 
      do axis=0,nd-1
      do side=0,1

        tbc(side,axis)=-2

        if( boundaryCondition(side,axis).le.0 )then
          ! periodic or interpolation (or parallel ghost)
          tbc(side,axis)=boundaryCondition(side,axis)

        else if( boundaryCondition(side,axis).eq.noSlipWall .or. \
                 boundaryCondition(side,axis).eq.inflowWithVelocityGiven .or. \
                 boundaryCondition(side,axis).eq.slipWall .or. \
                 boundaryCondition(side,axis).eq.axisymmetric .or. \
                 boundaryCondition(side,axis).eq.dirichletBoundaryCondition .or. \
                 boundaryCondition(side,axis).eq.inflowWithPandTV  \
                )then
          ! BC: a0*T + a1*T.n = 
	  a0=mixedCoeff(tc,side,axis,grid)
	  a1=mixedNormalCoeff(tc,side,axis,grid)

          if( boundaryCondition(side,axis).eq.slipWall .or.  \
              boundaryCondition(side,axis).eq.axisymmetric)then
           ! -- slip wall is always a Neumann condition (see below as well)
           a0=0.
           a1=1.
          end if

          if( t.le.0. .and. debug.gt.3 )then
            write(*,'("bcOptT: (side,axis)=(",2i2,") bc=",i2," a0,a1=",2f8.4," addBoundaryForcing=",i2)') side,axis,boundaryCondition(side,axis),a0,a1,addBoundaryForcing(side,axis)
          end if

          if( a1.eq.0. )then
            tbc(side,axis) = dirichlet
          else
            tbc(side,axis) = neumann        
          end if

        else if( boundaryCondition(side,axis).eq.outflow )then

          if( outflowOption.eq.extrapolateOutflow )then
            tbc(side,axis) = extrapolate
          else if( outflowOption.eq.neumannAtOuflow )then
            if( a1.eq.0. )then
              tbc(side,axis) = dirichlet
            else
              tbc(side,axis) = neumann        
            end if            
          else
           write(*,'("bcOptT:ERROR: unknown outflowOption=",i6)') outflowOption
           stop 1112
          end if
        
          if( t.le.0. .and. debug.gt.3 )then
            write(*,'("bcOptT: (side,axis)=(",2i2,") OUTFLOW: tbc=",i2," dirichlet,neumann,extrap=",3i2," addBoundaryForcing=",i2)') side,axis,tbc(side,axis),dirichlet,neumann,extrapolate,addBoundaryForcing(side,axis)
          end if

        else if( boundaryCondition(side,axis).eq.symmetry .or. \
                 boundaryCondition(side,axis).eq.axisymmetric )then

          tbc(side,axis) = evenSymmetry
        
        else

          write(*,'("bcOptT:ERROR: unknown boundaryCondition=",i6," (side,axis,grid)=(",3i5)') boundaryCondition(side,axis),side,axis,grid
          stop 1113

        end if
      end do
      end do

      ! ---------------------------------------------------------------
      ! ----------- STAGE I : Assign Dirichlet Conditions -------------
      ! ---------------------------------------------------------------

      ! NOTE: the numGhost args are used in ghost loops
      beginLoopOverSides(numGhost,numGhost)

        if( tbc(side,axis).eq.dirichlet )then

          beginLoops3d()
           if( mask(i1,i2,i3).ne.0 )then
            if( addBoundaryForcing(side,axis).eq.0 .and. assignTwilightZone.eq.0 )then
              ! Do not use bcf - should we use bcData ? 
              ff = mixedRHS(tc,side,axis,grid)
            else if( assignTwilightZone.eq.0 )then
              ! RHS is found here: 
              ff = bcf(side,axis,i1,i2,i3,tc)
            else
              ! compute RHS from TZ
              if( nd.eq.2 )then
                call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,tc,te )
              else
                call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,tc,te )
              end if
              ff = te
            end if

            u(i1,i2,i3,tc)=ff

            ! For Dirichlet BC's extrap to order 4 is good enough for fourth order I believe.
            ! But use extrap5 to be exact for polynomials of degree 4 

            ! extrap first ghost
            j1=i1-is1
            j2=i2-is2
            j3=i3-is3
            u(j1,j2,j3,tc)=extrap5(u,i1,i2,i3,tc,is1,is2,is3) 

            ! extrap second ghost
            k1=j1-is1
            k2=j2-is2
            k3=j3-is3
            u(k1,k2,k3,tc)=extrap5(u,j1,j2,j3,tc,is1,is2,is3)
           end if ! mask .ne. 0
          endLoops3d()

        end if ! end if dirichlet 

      endLoopOverSides()

      ! --  Extrap values on remaining sides to give initial values 
      !     --> really we only need to do this along extended boundaries on
      !         curvilinear grids so we have values for the Neumann BC
      beginLoopOverSides(numGhost,numGhost)

        ! For now outflow (extrapolate) is done here too

        if( tbc(side,axis).ne.dirichlet )then
          beginLoops3d()
            ! extrap first ghost
            j1=i1-is1
            j2=i2-is2
            j3=i3-is3
            u(j1,j2,j3,tc)=extrap5(u,i1,i2,i3,tc,is1,is2,is3)

            ! extrap second ghost
            k1=j1-is1
            k2=j2-is2
            k3=j3-is3
            u(k1,k2,k3,tc)=extrap5(u,j1,j2,j3,tc,is1,is2,is3)
          endLoops3d()
        end if

      endLoopOverSides()      


      ! ---------------------------------------------------------------------
      ! ----------- STAGE II : Neumann-like Boundary Conditions -------------
      ! ---------------------------------------------------------------------

      beginLoopOverSides(numGhost,numGhost)

       if( tbc(side,axis).eq.neumann )then

         ! BC: a0*T + a1*T.n = 
	 a0=mixedCoeff(tc,side,axis,grid)
	 a1=mixedNormalCoeff(tc,side,axis,grid)

         if( boundaryCondition(side,axis).eq.slipWall .or.  \
             boundaryCondition(side,axis).eq.axisymmetric .or.\
             boundaryCondition(side,axis).eq.outflow )then
           ! -- slip wall is always a Neumann condition

           ! NOTE: we could instead apply a symmetry condition for slip+Cartesian or axisymmetric+Cartesian

           a0=0.
           a1=1.
         end if


         ! rectangular case:
         if( gridType.eq.rectangular )then
           ! compute the outward normal (an1,an2,an3)
           an1 = 0.
           an2 = 0.
           an3 = 0.
           if( axis.eq.0 )then
            an1=-is
           else if( axis.eq.1 )then
            an2=-is
           else
            an3=-is
           end if
           dxn=dx(axis)
           b0=-4.*dxn*a0/a1-10./3.
           b1=4.*(dxn/a1)
         end if

         beginLoops3d()

          ! first ghost pt:
          j1=i1-is1
          j2=i2-is2
          j3=i3-is3
          ! 2nd ghost:
          k1=j1-is1
          k2=j2-is2
          k3=j3-is3
          if( mask(i1,i2,i3).gt.0 )then
          

           if( gridType.eq.curvilinear )then
            ! compute the outward normal (an1,an2,an3)
            getNormal(i1,i2,i3)
           end if

           if( addBoundaryForcing(side,axis).eq.0  .and. assignTwilightZone.eq.0 )then
             ! Do not use bcf - should we use bcData ? 
             if( boundaryCondition(side,axis).eq.outflow )then
               ff=0. ! *wdh* 2013/01/32
             else
               ff = mixedRHS(tc,side,axis,grid)
             end if
           else if( assignTwilightZone.eq.0 )then
             ! RHS is found here: 
             ff = bcf(side,axis,i1,i2,i3,tc)
           else
             ! compute RHS from TZ
             if( nd.eq.2 )then
               call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,tc,te )
               call ogDeriv(ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,tc,tex)
               call ogDeriv(ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,tc,tey)
               ff = a0*te + a1*( an1*tex + an2*tey )
             else
               call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,tc,te )
               call ogDeriv(ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,tc,tex)
               call ogDeriv(ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,tc,tey)
               call ogDeriv(ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,tc,tez)
               ff = a0*te + a1*( an1*tex + an2*tey + an3*tez )
             end if
            end if


           ! --- assign 2 ghost points using:
           !  (1) Apply Neumann BC to 4th order
           !  (2) Extrap. 2nd ghost to 5th order
           if( gridType.eq.rectangular )then

            ! write(*,'(" TBC: j1,j2=",2i3," u,ff=",2e12.2)') j1,j2,ff,u(j1,j2,j3,tc)
            
            u(j1,j2,j3,tc)=  b0*u(j1+  is1,j2+  is2,j3+  is3,tc)\
                            +6.*u(j1+2*is1,j2+2*is2,j3+2*is3,tc)\
                            -2.*u(j1+3*is1,j2+3*is2,j3+3*is3,tc)\
                               +u(j1+4*is1,j2+4*is2,j3+4*is3,tc)/3.\
                              +b1*ff


           else ! curvilinear grid: 

            ! a1*( n1*ux + n2*ux + n3*uz ) + a0*u = f 
            ! a1*( (n1*rx+n2*ry+n3*rz)*ur + (n1*sx+n2*sy+n3*sz)*us + (n1*tx+n2*ty+n3*st)*ut ) + a0*u = f 

            !       d14(kd) = 1./(12.*dr(kd))
            !       ur4(i1,i2,i3,kd)=(8.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))-(u(i1+2,
            !        & i2,i3,kd)-u(i1-2,i2,i3,kd)))*d14(0)

            ! ur = f -> 
            ! u(-2) -8*u(-1) =            -8*u(1)   + u(2)        + 12*dr( f )    --- (A)
            ! u(-2) -5*u(-1) = -10*u(0) + 10*u(1) - 5*u(2) + u(3)                 --- (B)

            ! A - B = 
            !       -3*u(-1) =  10*u(0) - 18*u(1) + 6*u(2) - u(3) + 12*dr*( f ) 

            urv(0) = ur4(i1,i2,i3,tc)
            urv(1) = us4(i1,i2,i3,tc)

            if( nd.eq.2 )then
              t1=a1*( an1*rsxy(i1,i2,i3,axis  ,0)+an2*rsxy(i1,i2,i3,axis  ,1) )
              t2=a1*( an1*rsxy(i1,i2,i3,axisp1,0)+an2*rsxy(i1,i2,i3,axisp1,1) )

              ur0 = (ff - ( t2*urv(axisp1) + a0*u(i1,i2,i3,tc) ) )/t1
            else
              urv(2) = ut4(i1,i2,i3,tc)
              t1=a1*( an1*rsxy(i1,i2,i3,axis  ,0)+an2*rsxy(i1,i2,i3,axis  ,1)+an3*rsxy(i1,i2,i3,axis  ,2) )
              t2=a1*( an1*rsxy(i1,i2,i3,axisp1,0)+an2*rsxy(i1,i2,i3,axisp1,1)+an3*rsxy(i1,i2,i3,axisp1,2) )
              t3=a1*( an1*rsxy(i1,i2,i3,axisp2,0)+an2*rsxy(i1,i2,i3,axisp2,1)+an3*rsxy(i1,i2,i3,axisp2,2) )

              ur0 = ( ff - ( t2*urv(axisp1) + t3*urv(axisp2) + a0*u(i1,i2,i3,tc) ) )/t1
            end if

            u(j1,j2,j3,tc) = (-10./3.)*u(j1+  is1,j2+  is2,j3+  is3,tc)\
                                   +6.*u(j1+2*is1,j2+2*is2,j3+2*is3,tc)\
                                   -2.*u(j1+3*is1,j2+3*is2,j3+3*is3,tc)\
                               +(1./3)*u(j1+4*is1,j2+4*is2,j3+4*is3,tc)\
                                -4.*is*dr(axis)*ur0

           end if

           ! For Neumann BC's it IS necessary to extrap to order 5 for fourth order. 
           ! extrap second ghost
           u(k1,k2,k3,tc)=extrap5(u,j1,j2,j3,tc,is1,is2,is3)

          else if( mask(i1,i2,i3).lt.0 )then
           ! extrap ghost outside interp. pts

           u(j1,j2,j3,tc)=extrap5(u,i1,i2,i3,tc,is1,is2,is3)
           u(k1,k2,k3,tc)=extrap5(u,j1,j2,j3,tc,is1,is2,is3)

          end if

         endLoops3d()


       end if  ! end if tbc == neumann

      endLoopOverSides()


      ! ---------------------------------
      ! --- assign corners and edges: ---
      ! ---------------------------------

      do side3=0,2
      do side2=0,2
      do side1=0,2
        cornerBC(side1,side2,side3)=0         ! extrapolateCorner=0, (BoundaryConditionParameters)
      end do
      end do
      end do

      orderOfExtrapolationForCorners=5

      iparc(0)=tc
      iparc(1)=tc
      iparc(2)=0                              ! useWhereMask;
      iparc(3)=orderOfExtrapolationForCorners
      iparc(4)=numGhost                       ! numberOfCornerGhostLinesToAssign
      iparc(5)=0                              ! cornerExtrapolationOption : 0=extrap along diagonals
      iparc(6)=0                              ! vectorSymmetryCornerComponent
      iparc(7)=gridType

      rparc(0)=epsx ! normEps

      ! Note: is it ok to use gridIndexRange instead of indexRange here: ??
      call fixBoundaryCornersOpt( nd, \
      nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,0,tc,\
      nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, \
      u,mask,rsxy, gridIndexRange, dimRange, \
      isPeriodic, boundaryCondition, cornerBC, iparc, rparc )


      return
      end

