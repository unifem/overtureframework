!  -*- mode: F90 -*-
! Define macros to evaluate an advection term (a.grad) u
!



! --------------------------------------------------------------------------------
!  Macro: getUpwindAdvection
! 
! --------------------------------------------------------------------------------
#beginMacro getUpwindAdvection(u,i1,i2,i3,SCALAR,DIM,ORDER,GRIDTYPE, agu)

 #If #GRIDTYPE eq "rectangular"
  ! --- CARTESIAN GRID ---
  !- agu(uc,uc)=UU(uc)*UX(uc)
  !- agu(vc,uc)=UU(vc)*UY(uc)

  !- agu(uc,vc)=UU(uc)*UX(vc)
  !- agu(vc,vc)=UU(vc)*UY(vc)

  ! -- first order upwind --
  if( upwindOrder.eq.1 )then
   au = u(i1,i2,i3,uc)
   if( au.gt.0. )then
     ! u*ux = u*D-x(u)
     agu(uc,uc)= au*(u(i1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(dx(0)) 
     ! u*vx = u*D-x(v)
     agu(uc,vc)= au*(u(i1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(dx(0))
   else
     ! u*ux = u*D+x(u)
     agu(uc,uc)= au*(u(i1+1,i2,i3,uc)-u(i1,i2,i3,uc))/(dx(0))
     ! u*vx = u*D+x(v)
     agu(uc,vc)= au*(u(i1+1,i2,i3,vc)-u(i1,i2,i3,vc))/(dx(0))
   end if
 
   au = u(i1,i2,i3,vc)
   if( au.gt.0. )then
     ! v*uy = v*D-y(u)
     agu(vc,uc)= au*(u(i1,i2,i3,uc)-u(i1,i2-1,i3,uc))/(dx(1))
     ! v*vy = v*D-y(v)
     agu(vc,vc)= au*(u(i1,i2,i3,vc)-u(i1,i2-1,i3,vc))/(dx(1))
   else
     ! v*uy = v*D+y(u)
     agu(vc,uc)= au*(u(i1,i2+1,i3,uc)-u(i1,i2,i3,uc))/(dx(1))
     ! v*vy = v*D+y(v) 
     agu(vc,vc)= au*(u(i1,i2+1,i3,vc)-u(i1,i2,i3,vc))/(dx(1))
   end if

    #If #DIM eq "3"
      ! finish me 

      stop 777

    #End

  else
    write(*,'(" finish me, upwindOrder=",i2)') upwindOrder
    stop 222
  end if

 #Elif #GRIDTYPE eq "curvilinear"

  ! --- CURVILINEAR GRID ---
!  #If #DIM eq "2"
!    au = rsxy(i1,i2,i3,0,0)*u(i1,i2,i3,uc)+rsxy(i1,i2,i3,0,1)*u(i1,i2,i3,vc)
!  #Else
!    au = rsxy(i1,i2,i3,0,0)*u(i1,i2,i3,uc)+rsxy(i1,i2,i3,0,1)*u(i1,i2,i3,vc)+rsxy(i1,i2,i3,0,2)*u(i1,i2,i3,wc)
!  #End
!
!  if( au.gt.0. )then
!    agu(uc,uc)= au*(u(i1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(dr(0))
!  else
!    agu(uc,uc)= au*(u(i1+1,i2,i3,uc)-u(i1,i2,i3,uc))/(dr(0))
!  end if
  agu(uc,uc)=UU(uc)*UX(uc)
  agu(vc,uc)=UU(vc)*UY(uc)

  agu(uc,vc)=UU(uc)*UX(vc)
  agu(vc,vc)=UU(vc)*UY(vc)

 #Else
   write(*,'(" getUpwindAdvection: finish me")' )
   stop 7171
 #End

#endMacro 


! --------------------------------------------------------------------------------
!  Macro: getBwenoAdvection
! --------------------------------------------------------------------------------
#beginMacro getBwenoAdvection(u,i1,i2,i3,SCALAR,DIM,ORDER,GRIDTYPE, agu)

  write(*,'(" getBewnoAdvection: finish me")' )
  stop 222

#endMacro 


! --------------------------------------------------------------------------------
!  Macro: getAdvection
! 
! u(i1,i2,i3,.) (input) : current solution
! (i1,i2,i3) (input) : get advection terms solution at this point
! advectionOption (input) : 
! SCALAR: NONE
!         PASSIVE - include equations for a passive scalar
!
! DIM,ORDER,GRIDTYPE (input) :
! UPWIND (input) : CENTERED, UPWIND or BWENO
! 
!  agu(m,n) (output) : m=0,1,nd, n=0,1,nd
!     agu(0,0) : u*ux 
!     agu(1,0) : v*uy
!     agu(2,0) : w*uz 
!
!     agu(0,1) : u*vx 
!     agu(1,1) : v*vy
!     agu(2,1) : w*vz 
!
!     agu(0,2) : u*wx 
!     agu(1,2) : v*wy
!     agu(2,2) : w*wz 
! --------------------------------------------------------------------------------
#beginMacro getAdvection(u,i1,i2,i3,SCALAR,DIM,ORDER,GRIDTYPE,UPWIND, agu)

#If (#UPWIND == "CENTERED") 
 ! -- centered advection ---
 ! write(*,'(" getAdvection -- centered")')
 #If #DIM == "2"
  ! -- 2D --
  agu(uc,uc)=UU(uc)*UX(uc)
  agu(vc,uc)=UU(vc)*UY(uc)

  agu(uc,vc)=UU(uc)*UX(vc)
  agu(vc,vc)=UU(vc)*UY(vc)

 #Else
  ! -- 3D -- *check me*
  agu(uc,uc)=UU(uc)*UX(uc)
  agu(vc,uc)=UU(vc)*UY(uc)
  agu(wc,uc)=UU(wc)*UY(uc)

  agu(uc,vc)=UU(uc)*UX(vc)
  agu(vc,vc)=UU(vc)*UY(vc)
  agu(wc,vc)=UU(wc)*UY(vc)

  agu(uc,wc)=UU(uc)*UX(wc)
  agu(vc,wc)=UU(vc)*UY(wc)
  agu(wc,wc)=UU(wc)*UY(wc)

 #End

#Elif #UPWIND == "UPWIND" 

  ! --- upwind scheme ---
  ! for testing output this next message:
  if( t.le. 0. )then
    write(*,'(" getAdvection upwind scheme (7)")') 
  end if
  getUpwindAdvection(u,i1,i2,i3,SCALAR,DIM,ORDER,GRIDTYPE, agu)

#Elif #UPWIND == "BWENO" 

  ! --- Bweno scheme ---
  getBwenoAdvection(u,i1,i2,i3,SCALAR,DIM,ORDER,GRIDTYPE, agu)

#Else

  write(*,'(" getAdvection:ERROR: unknown advectionOption.")' )
  stop 999

#End

#endMacro
