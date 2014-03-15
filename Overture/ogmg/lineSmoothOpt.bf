! These next include files will define the macros that will define the difference approximations
! The actual macro is called below
#Include "defineDiffOrder2f.h"
#Include "defineDiffOrder4f.h"


! These next include file will define the macros that will define the difference approximations (in op/src)
! Defines getDuDx2(u,aj,ff), getDuDxx2(u,aj,ff), getDuDx3(u,aj,ff), ...  etc. 
#Include "derivMacroDefinitions.h"

! Define 
!    defineParametricDerivativeMacros(u,dr,dx,DIM,ORDER,COMPONENTS,MAXDERIV)
!       defines -> ur2, us2, ux2, uy2, ...            (2D)
!                  ur3, us3, ut3, ux3, uy3, uz3, ...  (3D)
#Include "defineParametricDerivMacros.h"
defineParametricDerivativeMacros(rsxy,dr,dx,3,2,2,2)
defineParametricDerivativeMacros(rsxy,dr,dx,3,4,2,2)

! ==========================================================================================
!  Evaluate the Jacobian and its derivatives (parametric and spatial). 
!    aj     : prefix for the name of the resulting jacobian variables, 
!             e.g. ajrx, ajsy, ajrxx, ajsxy, ...
!    MAXDER : number of derivatives to evaluate.  
! ==========================================================================================
#beginMacro opEvalJacobianDerivatives(aj,MAXDER)

#If $GRIDTYPE eq "curvilinear"
 ! this next call will define the jacobian and its derivatives (parameteric and spatial)
 #peval evalJacobianDerivatives(rsxy,i1,i2,i3,aj,$DIM,$ORDER,MAXDER)

#End

#endMacro 

! This next macro will evaluate r,s,t derivatives of the jacobian 
! u = jacobian name (rsxy), v=prefix for derivatives: vrxr, vrys, 
#beginMacro evalParametricJacobianDerivatives(u,i1,i2,i3,v,DIM,ORDER,MAXDERIV)
#If DIM == 2
evalParametricDerivativesComponents2(u,i1,i2,i3,0,0, v ## rx,DIM,ORDER,MAXDERIV)
evalParametricDerivativesComponents2(u,i1,i2,i3,1,0, v ## sx,DIM,ORDER,MAXDERIV)
evalParametricDerivativesComponents2(u,i1,i2,i3,0,1, v ## ry,DIM,ORDER,MAXDERIV)
evalParametricDerivativesComponents2(u,i1,i2,i3,1,1, v ## sy,DIM,ORDER,MAXDERIV)
#End
#If DIM == 3
evalParametricDerivativesComponents2(u,i1,i2,i3,0,0, v ## rx,DIM,ORDER,MAXDERIV)
evalParametricDerivativesComponents2(u,i1,i2,i3,1,0, v ## sx,DIM,ORDER,MAXDERIV)
evalParametricDerivativesComponents2(u,i1,i2,i3,2,0, v ## tx,DIM,ORDER,MAXDERIV)
evalParametricDerivativesComponents2(u,i1,i2,i3,0,1, v ## ry,DIM,ORDER,MAXDERIV)
evalParametricDerivativesComponents2(u,i1,i2,i3,1,1, v ## sy,DIM,ORDER,MAXDERIV)
evalParametricDerivativesComponents2(u,i1,i2,i3,2,1, v ## ty,DIM,ORDER,MAXDERIV)
evalParametricDerivativesComponents2(u,i1,i2,i3,0,2, v ## rz,DIM,ORDER,MAXDERIV)
evalParametricDerivativesComponents2(u,i1,i2,i3,1,2, v ## sz,DIM,ORDER,MAXDERIV)
evalParametricDerivativesComponents2(u,i1,i2,i3,2,2, v ## tz,DIM,ORDER,MAXDERIV)
#End
#endMacro

! ==========================================================================================
!  Evaluate the Jacobian and its parametric derivatives
!    aj     : prefix for the name of the resulting jacobian variables, 
!             e.g. ajrx, ajsy, ajrxx, ajsxy, ...
!    MAXDER : number of derivatives to evaluate.  
! ==========================================================================================
#beginMacro opEvalParametricJacobianDerivatives(aj,MAXDER)

#If $GRIDTYPE eq "curvilinear"
 ! this next call will define the jacobian and its derivatives (parameteric and spatial)
 #peval evalParametricJacobianDerivatives(rsxy,i1,i2,i3,aj,$DIM,$ORDER,MAXDER)

#End

#endMacro 


#beginMacro beginLoop(n1a,n1b,n2a,n2b,n3a,n3b)
do i3=n3a,n3b
do i2=n2a,n2b
do i1=n1a,n1b 
#endMacro
#beginMacro beginLoopJ(n1a,n1b,n2a,n2b,n3a,n3b)
do j3=n3a,n3b
do j2=n2a,n2b
do j1=n1a,n1b 
#endMacro
#beginMacro endLoop()
end do
end do
end do
#endMacro


! return the loop indicies for the "boundary" (side,axis) shifted by "shift"
#beginMacro getBoundaryIndex(side,axis,m1a,m1b,m2a,m2b,m3a,m3b,shift)
 m1a=n1a
 m1b=n1b
 m2a=n2a
 m2b=n2b
 m3a=n3a
 m3b=n3b
 if( axis.eq.0 )then
  if( side.eq.0 )then
    m1a=n1a+shift
  else
    m1a=n1b-shift
  end if
  m1b=m1a
 else if( axis.eq.1 )then
  if( side.eq.0 )then
    m2a=n2a+shift
  else
    m2a=n2b-shift
  end if
  m2b=m2a
 else
  if( side.eq.0 )then
    m3a=n3a+shift
  else
    m3a=n3b-shift
  end if
  m3b=m3a
 end if
#endMacro

! =====================================================================================
! Macro: Fill the extrapolation coefficients into the pentadiagonal system.
!  *wdh* added 110309
! 
! k1,k2,k3 : fill in this point
! a1,a2,a3,a4,a5 : pentadiagonal matrix - MUST be ordered correctly -- extrapolated point is "a1"
! orderOfExtrap : order of extrapolation 
! =====================================================================================
#beginMacro fillExtrapolationPentaDiagonal(k1,k2,k3,a1,a2,a3,a4,a5,orderOfExtrap)
  if( orderOfExtrap.eq.4 )then
    a1(k1,k2,k3)= 1.
    a2(k1,k2,k3)=-4.
    a3(k1,k2,k3)= 6.
    a4(k1,k2,k3)=-4.
    a5(k1,k2,k3)= 1.
  else if( orderOfExtrap.eq.5 )then
    ! 5-th order extrap (NOTE: this does not fit entirely in the matrix so we need a residual)
    a1(k1,k2,k3)=  1.
    a2(k1,k2,k3)= -5.
    a3(k1,k2,k3)= 10.
    a4(k1,k2,k3)=-10.
    a5(k1,k2,k3)=  5.
  else if( orderOfExtrap.eq.2 )then
    a1(k1,k2,k3)= 0.
    a2(k1,k2,k3)= 1.
    a3(k1,k2,k3)=-2.
    a4(k1,k2,k3)= 1.
    a5(k1,k2,k3)= 0.
  else
    stop 18520
  end if
#endMacro


#beginMacro fillExtrapolation4(k1,k2,k3,a1,a2,a3,a4,a5)
  a1(k1,k2,k3)= 1.
  a2(k1,k2,k3)=-4.
  a3(k1,k2,k3)= 6.
  a4(k1,k2,k3)=-4.
  a5(k1,k2,k3)= 1.
#endMacro

#beginMacro fillExtrapolation2(k1,k2,k3,a1,a2,a3)
  a1(k1,k2,k3)= 1.
  a2(k1,k2,k3)=-2.
  a3(k1,k2,k3)= 1.
#endMacro


! ======================================================================================
! Macro: Fill extrapolation coefficients into the first ghost for penta-diagonal system
! SIDE : left or right
! g1a,g1b,g2a,g2b,g3a,g3b : loop bounds
! *wdh* 11023 : added loop args to this macro and the ones below
! ======================================================================================
#beginMacro extrapFirstGhost(SIDE,g1a,g1b,g2a,g2b,g3a,g3b)
! write(*,'(''+++++extrapFirstGhost [SIDE]: l1='',6i3)') g1a,g1b,g2a,g2b,g3a,g3b
#If #SIDE == "left"
 ! 1st ghost line on left:
 !       [  c  d  e  a  b ]
 !      i= -1 -0  1  2  3
 if( orderOfExtrapD.eq.4 )then
  beginLoop(g1a,g1b,g2a,g2b,g3a,g3b)
    c(i1,i2,i3)= 1.
    d(i1,i2,i3)=-4.
    e(i1,i2,i3)= 6.
    a(i1,i2,i3)=-4.
    b(i1,i2,i3)= 1.
  endLoop()
 else if( orderOfExtrapD.eq.2 )then
  beginLoop(g1a,g1b,g2a,g2b,g3a,g3b)
    c(i1,i2,i3)= 1.
    d(i1,i2,i3)=-2.
    e(i1,i2,i3)= 1.
    a(i1,i2,i3)= 0.
    b(i1,i2,i3)= 0.
  endLoop()
 else
   stop 63
 end if
#Elif #SIDE == "right" 
 ! 1st ghost line on right:
 !       [  d  e  a  b  c ]
 !   i=n+[ -3 -2 -1  0  1 ]
 if( orderOfExtrapD.eq.4 )then
  beginLoop(g1a,g1b,g2a,g2b,g3a,g3b)
    d(i1,i2,i3)= 1.
    e(i1,i2,i3)=-4.
    a(i1,i2,i3)= 6.
    b(i1,i2,i3)=-4.
    c(i1,i2,i3)= 1.
  endLoop()
 else if( orderOfExtrapD.eq.2 )then
  beginLoop(g1a,g1b,g2a,g2b,g3a,g3b)
    d(i1,i2,i3)= 0.
    e(i1,i2,i3)= 0.
    a(i1,i2,i3)= 1.
    b(i1,i2,i3)=-2.
    c(i1,i2,i3)= 1.
  endLoop()
 else
   stop 64
 end if
#Else
  stop 66
#End
#endMacro

! ===========================================================================================
! Macro: Fill odd-symmetry coefficients into the first ghost for penta-diagonal system
! SIDE : left or right
! g1a,g1b,g2a,g2b,g3a,g3b : loop bounds
! ===========================================================================================
#beginMacro oddSymmetryFirstGhost(SIDE,g1a,g1b,g2a,g2b,g3a,g3b)
#If #SIDE == "left"
 ! 1st ghost line on left:
 !       [  b  c  d  e  a ]
 !     i=[ -2 -1  0  1  2 ]
 beginLoop(g1a,g1b,g2a,g2b,g3a,g3b)
    b(i1,i2,i3)= 0.
    c(i1,i2,i3)= 1.
    d(i1,i2,i3)=-2.
    e(i1,i2,i3)= 1. 
    a(i1,i2,i3)= 0.
 endLoop()
#Elif #SIDE == "right" 
! 1st ghost line on right:
!       [  e  a  b  c  d ]
!  i=n+ [ -2 -1  0  1  2 ]
 beginLoop(g1a,g1b,g2a,g2b,g3a,g3b)
   e(i1,i2,i3)= 0.
   a(i1,i2,i3)= 1. 
   b(i1,i2,i3)=-2.
   c(i1,i2,i3)= 1. 
   d(i1,i2,i3)= 0. 
 endLoop()
#Else
  stop 99
#End
#endMacro

! ===========================================================================================
! Macro: Fill even-symmetry coefficients into the first ghost for penta-diagonal system
! SIDE : left or right
! g1a,g1b,g2a,g2b,g3a,g3b : loop bounds
! ===========================================================================================
#beginMacro evenSymmetryFirstGhost(SIDE,g1a,g1b,g2a,g2b,g3a,g3b)
#If #SIDE == "left"
 ! 1st ghost line on left:
 !       [  b  c  d  e  a ]
 !     i=[ -2 -1  0  1  2 ]
 beginLoop(g1a,g1b,g2a,g2b,g3a,g3b)
    b(i1,i2,i3)= 0.
    c(i1,i2,i3)= 1.
    d(i1,i2,i3)= 0.
    e(i1,i2,i3)=-1. 
    a(i1,i2,i3)= 0.
 endLoop()
#Elif #SIDE == "right" 
! 1st ghost line on right:
!       [  e  a  b  c  d ]
!  i=n+ [ -2 -1  0  1  2 ]
 beginLoop(g1a,g1b,g2a,g2b,g3a,g3b)
   e(i1,i2,i3)= 0.
   a(i1,i2,i3)=-1. 
   b(i1,i2,i3)= 0.
   c(i1,i2,i3)= 1. 
   d(i1,i2,i3)= 0. 
 endLoop()
#Else
  stop 99
#End
#endMacro

! ===========================================================================================
! Macro: Fill even-symmetry coefficients into the second ghost for penta-diagonal system
! SIDE : left or right
! g1a,g1b,g2a,g2b,g3a,g3b : loop bounds
! ===========================================================================================
#beginMacro evenSymmetrySecondGhost(SIDE,g1a,g1b,g2a,g2b,g3a,g3b)
#If #SIDE == "left"
 ! 2nd ghost line on left:
 !       [  c  d  e  a  b ]
 !     i=[ -2 -1  0  1  2 ]
 ! *wdh* 11023 - fixed (was setting symmetry on first ghost)
 beginLoop(g1a,g1b,g2a,g2b,g3a,g3b)
    c(i1,i2,i3)= 1.
    d(i1,i2,i3)= 0.
    e(i1,i2,i3)= 0.
    a(i1,i2,i3)= 0.
    b(i1,i2,i3)=-1.
 endLoop()
#Elif #SIDE == "right" 
 ! 2nd ghost line on right:
 !       [  d  e  a  b  c ]
 !   i=n+[ -2 -1  0  1  2 ]
 beginLoop(g1a,g1b,g2a,g2b,g3a,g3b)
    d(i1,i2,i3)=-1.
    e(i1,i2,i3)= 0.
    a(i1,i2,i3)= 0.
    b(i1,i2,i3)= 0.
    c(i1,i2,i3)= 1.
 endLoop()
#Else
  stop 99
#End
#endMacro

! ===========================================================================================
!   -- FIX ME --
!  *wdh* 110308 -- this was replaced by the macro below 
!  This version was only correct for Cartesian grids. 
! ==========================================================================================
#beginMacro mixedToSecondOrderPentaDiagonalTwoLinesOLD(GRIDTYPE,SIDE)
#If #SIDE == "left"
 ! 2nd ghost line on left:
 !       [  c  d  e  a  b ]
 !     i=[ -2 -1  0  1  2 ]
 ! 1st ghost line on left:
 !       [  b  c  d  e  a ]
 !     i=[ -2 -1  0  1  2 ]
 ! write(*,'("lineSmoothOpt: mixed to 2nd orderBC: level=",i2)') level
 beginLoop(l1a,l1b,l2a,l2b,l3a,l3b)
    c(i1,i2,i3)= 1.
    d(i1,i2,i3)= 0.
    e(i1,i2,i3)= 0.
    a(i1,i2,i3)= 0.
    b(i1,i2,i3)=-1.

    j1=i1+is1
    j2=i2+is2
    j3=i3+is3
    b(j1,j2,j3)= 0.
    c(j1,j2,j3)= 1.
    d(j1,j2,j3)= 0.
    e(j1,j2,j3)=-1.
    a(j1,j2,j3)= 0.
 endLoop()
#Elif #SIDE == "right" 
 ! 2nd ghost line on right:
 !       [  d  e  a  b  c ]
 !   i=n+[ -2 -1  0  1  2 ]
 ! 1st ghost line on right:
 !       [  e  a  b  c  d ]
 !  i=n+ [ -2 -1  0  1  2 ]
 beginLoop(l1a,l1b,l2a,l2b,l3a,l3b)
    d(i1,i2,i3)=-1.
    e(i1,i2,i3)= 0.
    a(i1,i2,i3)= 0.
    b(i1,i2,i3)= 0.
    c(i1,i2,i3)= 1.

    j1=i1+is1
    j2=i2+is2
    j3=i3+is3
    e(j1,j2,j3)= 0.
    a(j1,j2,j3)=-1.
    b(j1,j2,j3)= 0.
    c(j1,j2,j3)= 1.
    d(j1,j2,j3)= 0.
 endLoop()
#Else
  stop 99
#End
#endMacro

! =====================================================================================================
! Fill a Second-order accurate Mixed/Neumann BC into the pental-diagonal matrix ON TWO LINES
!
! NOTE: this BC is normally used on lower levels when the fourth-order accurate approximation is used on level=0
!
! We discretize the following BC to second order: 
! 
! a1*( n1*ux + n2*ux + n3*uz ) + a0*u = f 
! a1*( (n1*rx+n2*ry+n3*rz)*ur + (n1*sx+n2*sy+n3*sz)*us + (n1*tx+n2*ty+n3*st)*ut ) + a0*u = f 
!
! On the FIRST ghostline we use
!        ur = (u(i1+1,i2,i3) - u(i1-1,i2,i3) )/(2*dr(0))
! On the SECOND ghostline we use the wide formula
!        ur = (u(i1+2,i2,i3) - u(i1-2,i2,i3) )/(4*dr(0))
!
! GRIDTYPE: rectangular or curvilinear
! SIDE: left or right
! =====================================================================================================
#beginMacro mixedToSecondOrderPentaDiagonalTwoLines(GRIDTYPE,SIDE)
 ! write(*,'("lineSmoothOpt:mixedToSecondOrder: side,axis=",2i2, " a0,a1=",2f6.2)') side,axis,a0,a1

 is = 1-2*side

 beginLoop(m1a,m1b,m2a,m2b,m3a,m3b)
  j1=i1+is1 ! (j1,j2,j3) is the boundary point
  j2=i2+is2
  j3=i3+is3

  k1=i1-is1 ! (k1,k2,k3) is the 2nd ghost line
  k2=i2-is2
  k3=i3-is3

  if( mask(j1,j2,j3).gt.0 ) then

   #If #GRIDTYPE == "rectangular"
     t1 = (-is*a1)/(2.*dx(axis))

   #Elif #GRIDTYPE == "curvilinear"
    ! Curvilinear:
    ! (an1,an2,an3) is the outward normal
    an1 = rsxy(j1,j2,j3,axis,0)
    an2 = rsxy(j1,j2,j3,axis,1)
    if( nd.eq.2 )then
     aNormi = (-is)/sqrt(an1**2+an2**2)
     an1=an1*aNormi
     an2=an2*aNormi
     t1=a1*( an1*rsxy(j1,j2,j3,axis,0)+an2*rsxy(j1,j2,j3,axis,1) )/(2.*dr(axis))
    else
     an3 = rsxy(j1,j2,j3,axis,2)
     aNormi = (-is)/sqrt(an1**2+an2**2+an3**2)
     an1=an1*aNormi
     an2=an2*aNormi
     an3=an3*aNormi
     t1=a1*( an1*rsxy(j1,j2,j3,axis,0)+an2*rsxy(j1,j2,j3,axis,1)+an3*rsxy(j1,j2,j3,axis,2) )/(2.*dr(axis))
    end if
   #Else
    stop 9555
   #End


    #If #SIDE == "left"

     ! 1st ghost line on left:
     !       [  b  c  d  e  a ]
     !     i=[ -2 -1  0  1  2 ]
     b(i1,i2,i3)= 0.
     c(i1,i2,i3)= -t1
     d(i1,i2,i3)= a0
     e(i1,i2,i3)=  t1
     a(i1,i2,i3)= 0.

     ! Fill second ghost line on left
     ! 2nd ghost line on left:
     !       [  c  d  e  a  b ]
     !     i=[ -2 -1  0  1  2 ]
     c(k1,k2,k3)= -t1*.5
     d(k1,k2,k3)= 0.
     e(k1,k2,k3)= a0  
     a(k1,k2,k3)= 0.
     b(k1,k2,k3)=  t1*.5

    #Elif #SIDE == "right" 

     ! 1st ghost line on right:
     !       [  e  a  b  c  d ]
     !  i=n+ [ -2 -1  0  1  2 ]
     e(i1,i2,i3)= 0.
     a(i1,i2,i3)= -t1
     b(i1,i2,i3)= a0
     c(i1,i2,i3)=  t1
     d(i1,i2,i3)= 0.

     ! 2nd ghost line on right:
     !       [  d  e  a  b  c ]
     !   i=n+[ -2 -1  0  1  2 ]
     d(k1,k2,k3)= -t1*.5
     e(k1,k2,k3)=  0.
     a(k1,k2,k3)= a0
     b(k1,k2,k3)=  0.
     c(k1,k2,k3)=  t1*.5

    #Else
      stop 9555
     #End

    ! write(*,'("lineSmoothBuild: i1,i2=",2i3," [B C A]=",3e10.2)') i1,i2,B(i1,i2,i3),C(i1,i2,i3),A(i1,i2,i3)

  else if( mask(j1,j2,j3).lt.0 )then

   ! What order should this be?
   #If #SIDE == "left"
    ! 1st ghost line on left: -- extrap to order=2 -- this should be fine for lower levels
    fillExtrapolationPentaDiagonal(i1,i2,i3,b,c,d,e,a,2)
    ! 2nd ghost on left: -extrap to order 4 (this is like D_0^2 when D+D-u=0)
    fillExtrapolation4(k1,k2,k3,c,d,e,a,b)
   #Elif #SIDE == "right" 
    ! 1st ghost line on right: -- extrap to order=2 -- this should be fine for lower levels
    ! note: reverse order since we extrap last ghost
    fillExtrapolationPentaDiagonal(i1,i2,i3,d,c,b,a,e,2)
    ! 2nd ghost on right:
    ! note: reverse order of c,d,e,a,b
    fillExtrapolation4(k1,k2,k3,c,b,a,e,d)
   #Else
     stop 9666
   #End

  end if

 endLoop()

#endMacro





! =====================================================================================================
! Fill a Second-order accurate Mixed/Neumann BC into the pental-diagonal matrix
!
! We discretize the following BC to second order: 
! 
! a1*( n1*ux + n2*ux + n3*uz ) + a0*u = f 
! a1*( (n1*rx+n2*ry+n3*rz)*ur + (n1*sx+n2*sy+n3*sz)*us + (n1*tx+n2*ty+n3*st)*ut ) + a0*u = f 
!
!
! GRIDTYPE: rectangular or curvilinear
! SIDE: left or right
! B,C,D,E,A : names of penta-diagonal matrices (ordered for the left to right)
! =====================================================================================================
#beginMacro fillMixedToSecondOrderPentaDiagonal(GRIDTYPE,SIDE,B,C,D,E,A)
 ! write(*,'("lineSmoothOpt:mixedToSecondOrder: side,axis=",2i2, " a0,a1=",2f6.2)') side,axis,a0,a1

 is = 1-2*side

 beginLoop(m1a,m1b,m2a,m2b,m3a,m3b)
  j1=i1+is1 ! (j1,j2,j3) is the boundary point
  j2=i2+is2
  j3=i3+is3
  if( mask(j1,j2,j3).gt.0 ) then

   #If #GRIDTYPE == "rectangular"
     t1 = (-is*a1)/(2.*dx(axis))

   #Elif #GRIDTYPE == "curvilinear"
    ! Curvilinear:
    ! (an1,an2,an3) is the outward normal
    an1 = rsxy(j1,j2,j3,axis,0)
    an2 = rsxy(j1,j2,j3,axis,1)
    if( nd.eq.2 )then
     aNormi = (-is)/sqrt(an1**2+an2**2)
     an1=an1*aNormi
     an2=an2*aNormi
     t1=a1*( an1*rsxy(j1,j2,j3,axis,0)+an2*rsxy(j1,j2,j3,axis,1) )/(2.*dr(axis))
    else
     an3 = rsxy(j1,j2,j3,axis,2)
     aNormi = (-is)/sqrt(an1**2+an2**2+an3**2)
     an1=an1*aNormi
     an2=an2*aNormi
     an3=an3*aNormi
     t1=a1*( an1*rsxy(j1,j2,j3,axis,0)+an2*rsxy(j1,j2,j3,axis,1)+an3*rsxy(j1,j2,j3,axis,2) )/(2.*dr(axis))
    end if
   #Else
    stop 9555
   #End

    B(i1,i2,i3)= 0.
    C(i1,i2,i3)= -t1
    D(i1,i2,i3)= a0
    E(i1,i2,i3)=  t1
    A(i1,i2,i3)= 0.

    ! write(*,'("lineSmoothBuild: i1,i2=",2i3," [B C A]=",3e10.2)') i1,i2,B(i1,i2,i3),C(i1,i2,i3),A(i1,i2,i3)

  else if( mask(j1,j2,j3).lt.0 )then

   ! What order should this be?
   #If #SIDE == "left"
    ! 1st ghost line on left:
    !       [  b  c  d  e  a ]
    !     i=[ -2 -1  0  1  2 ]
    fillExtrapolation4(i1,i2,i3,B,C,D,E,A)
   #Elif #SIDE == "right" 
    ! note: reverse order of b,c,d,e,a since we extrap point a: 
    fillExtrapolation4(i1,i2,i3,A,E,D,C,B)
   #Else
     stop 9666
   #End

  end if

 endLoop()

#endMacro


! =====================================================================================================
! Fill a Mixed/Neumann BC into the tridiagonal matrix
!
! We discretize the following BC to second order: 
! 
! a1*( n1*ux + n2*ux + n3*uz ) + a0*u = f 
! a1*( (n1*rx+n2*ry+n3*rz)*ur + (n1*sx+n2*sy+n3*sz)*us + (n1*tx+n2*ty+n3*st)*ut ) + a0*u = f 
!
!
! B,C,A : names of tridiagonal matrices for ghost-pt, boundary-pt , first-interior-pt
! DIM: 2 or 3 
! =====================================================================================================
#beginMacro fillMixedToSecondOrder(B,C,A)
 ! write(*,'("lineSmoothOpt:mixedToSecondOrder: side,axis=",2i2, " a0,a1=",2f6.2)') side,axis,a0,a1

 is = 1-2*side

 beginLoop(m1a,m1b,m2a,m2b,m3a,m3b)
  j1=i1+is1 ! (j1,j2,j3) is the boundary point
  j2=i2+is2
  j3=i3+is3
  if( mask(j1,j2,j3).gt.0 ) then

    ! Curvilinear:
    ! (an1,an2,an3) is the outward normal
    an1 = rsxy(j1,j2,j3,axis,0)
    an2 = rsxy(j1,j2,j3,axis,1)
    if( nd.eq.2 )then
     aNormi = (-is)/sqrt(an1**2+an2**2)
     an1=an1*aNormi
     an2=an2*aNormi
     t1=a1*( an1*rsxy(j1,j2,j3,axis,0)+an2*rsxy(j1,j2,j3,axis,1) )/(2.*dr(axis))
    else
     an3 = rsxy(j1,j2,j3,axis,2)
     aNormi = (-is)/sqrt(an1**2+an2**2+an3**2)
     an1=an1*aNormi
     an2=an2*aNormi
     an3=an3*aNormi
     t1=a1*( an1*rsxy(j1,j2,j3,axis,0)+an2*rsxy(j1,j2,j3,axis,1)+an3*rsxy(j1,j2,j3,axis,2) )/(2.*dr(axis))
    end if

    B(i1,i2,i3)= -t1
    C(i1,i2,i3)= a0
    A(i1,i2,i3)=  t1

    ! write(*,'("lineSmoothBuild: i1,i2=",2i3," [B C A]=",3e10.2)') i1,i2,B(i1,i2,i3),C(i1,i2,i3),A(i1,i2,i3)

  else if( mask(j1,j2,j3).lt.0 )then

   fillExtrapolation2(i1,i2,i3,B,C,A)

  end if

 endLoop()

#endMacro


! ============================================================================
! *OLD* Fourth-order Neumann equation and normal derivative of equation
! ============================================================================
#beginMacro neumannAndEquationRectangularOld(aa,bb,cc,dd,ee)
 nsign = 2*side-1
 if( nd.eq.2 )then
   diag=2.*a0*dx(axis)**3/(a1*nsign*dx(axisp1)**2)
 else
   diag=(2.*a0*dx(axis)**3/(a1*nsign))*(1./dx(axisp1)**2+1./dx(axisp2)**2)
 end if

 ! write(*,'('' neumannAndEquationRectangular: a0,a1,diag='',3f6.2)') a0,a1,diag

 beginLoopJ(l1a,l1b,l2a,l2b,l3a,l3b)
  i1=j1+2*is1 ! (i1,i2,i3) is the boundary point
  i2=j2+2*is2 ! (j1,j2,j3) is the 2nd ghost line 
  i3=j3+2*is3
  if( mask(i1,i2,i3).gt.0 ) then
    ! This is the operator for urrr*dr^3
    aa(j1,j2,j3)=-.5
    bb(j1,j2,j3)= 1.
    cc(j1,j2,j3)= diag
    dd(j1,j2,j3)=-1.
    ee(j1,j2,j3)= .5
  else if( mask(i1,i2,i3).lt.0 ) then
    fillExtrapolation4(j1,j2,j3,b,c,d,e,a)
  end if
 endLoop()

#endMacro


! This next file defines the macro call appearing in the next function
#Include "neumannEquationBC.h"
#Include "neumannEquationForcing.h"

! ============================================================================
! *OLD* Fourth-order Neumann equation and normal derivative of equation
! ============================================================================
#beginMacro neumannAndEquationCurvilinearOld(DIR,DIM,aa,bb,cc,dd,ee)
 nsign = 2*side-1

write(*,'('' neumannAndEquationCurvilinear: a0,a1='',2f6.2)') a0,a1

 beginLoopJ(l1a,l1b,l2a,l2b,l3a,l3b)
  i1=j1+2*is1 ! (i1,i2,i3) is the boundary point
  i2=j2+2*is2 ! (j1,j2,j3) is the 2nd ghost line 
  i3=j3+2*is3
  if( mask(i1,i2,i3).gt.0 ) then

    fourthOrderNeumannEquationBC(DIR,DIM)

    ! write(*,'(''LS:matrix: side,axis,axisp1,b0,b1,b2,b3 ='',2i2,i3,4e11.3)') side,axis,axisp1,b0,b1,b2,b3

    aa(j1,j2,j3)=-.5/dr(axis)**3
    bb(j1,j2,j3)= 1./dr(axis)**3
    cc(j1,j2,j3)= 2.*b2/dr(axisp1)**2 - b0
    dd(j1,j2,j3)=-1./dr(axis)**3
    ee(j1,j2,j3)= .5/dr(axis)**3
  else if( mask(i1,i2,i3).lt.0 ) then
    fillExtrapolation4(j1,j2,j3,b,c,d,e,a)
  end if
 endLoop()

#endMacro


#Include "neumannEquationBC.new.h"

! ============================================================================
! Fourth-order Neumann equation and normal derivative of equation
!
!    u.xx + u.yy + u.zz = f
!    a1n*u.x + a0*u = g     -> u.x = (g-a0*u)/a1n 
!
!    u.xxx = f.x - ( u.xyy + u.xzz )
!          = f.x - ( g.yy -a0*u.yy + g.zz - a0*u.zz )/a1n 
! 
! Here is the numerical boundary condition:
!    u.xxx + (a0/a1n)*u.xx = f.x - ( g.yy+g.zz -a0*f )/a1n 
! 
! ============================================================================
#beginMacro neumannAndEquationRectangular(aa,bb,cc,dd,ee)

 if( equationToSolve.ne.laplaceEquation )then
   write(*,'("Ogmg:LSB:ERROR: equation!=laplace")')
   write(*,'("equationToSolve=",i2)') equationToSolve
   write(*,'("gridType=",i2)') gridType
   write(*,'("sparseStencil=",i2)') sparseStencil

   ! stop 5053
 end if

 nsign = 2*side-1
 diag=(a0/(a1*nsign))*dx(axis)  ! u_xx * dx^3

 write(*,'('' neumannAndEquationRectangular: a0,a1,diag='',3f6.2)') a0,a1,diag

 beginLoopJ(l1a,l1b,l2a,l2b,l3a,l3b)
  i1=j1+2*is1 ! (i1,i2,i3) is the boundary point
  i2=j2+2*is2 ! (j1,j2,j3) is the 2nd ghost line 
  i3=j3+2*is3
  if( mask(i1,i2,i3).gt.0 ) then
    ! This is the operator for urrr*dr^3
    aa(j1,j2,j3)=-.5
    bb(j1,j2,j3)= 1.  + diag
    cc(j1,j2,j3)=   -2.*diag
    dd(j1,j2,j3)=-1.  + diag
    ee(j1,j2,j3)= .5
  else if( mask(i1,i2,i3).lt.0 ) then
    fillExtrapolation4(j1,j2,j3,b,c,d,e,a)
  end if
 endLoop()

#endMacro



! ============================================================================
! Fourth-order Neumann equation and normal derivative of equation
!    ***New version: urrr + br2*urr - b0*u = gb
! ============================================================================
#beginMacro neumannAndEquationCurvilinear(DIR,DIM,aa,bb,cc,dd,ee)

  ! NOTE: This routine fills in the equation at the 2ND GHOST LINE

 if( equationToSolve.ne.laplaceEquation )then
   write(*,'("Ogmg:LSB:ERROR: equation!=laplace")')
   write(*,'("equationToSolve=",i2)') equationToSolve
   write(*,'("gridType=",i2)') gridType
   write(*,'("sparseStencil=",i2)') sparseStencil

   stop 5054
 end if

 nsign = 2*side-1

 ! write(*,'(''MGLS: neumannAndEquationCurvilinear: a0,a1='',2f6.2)') a0,a1

 diag=0. ! for now
 beginLoopJ(l1a,l1b,l2a,l2b,l3a,l3b)
  i1=j1+2*is1 ! (i1,i2,i3) is the boundary point
  i2=j2+2*is2 ! (j1,j2,j3) is the 2nd ghost line 
  i3=j3+2*is3
  if( mask(i1,i2,i3).gt.0 ) then

    fourthOrderNeumannEquationBCNew(DIR,DIM)


    aa(j1,j2,j3)=-.5/dr(axis)**3
    bb(j1,j2,j3)= 1./dr(axis)**3    +br2/dr(axis)**2
    cc(j1,j2,j3)=                -2.*br2/dr(axis)**2 - b0 
    dd(j1,j2,j3)=-1./dr(axis)**3    +br2/dr(axis)**2
    ee(j1,j2,j3)= .5/dr(axis)**3

    ! write(*,'("LS:neumannBC: myid=",i3," side,axis=",i2,i2," j1,j2=",i3,i3," b0,b1,b3,br2 =",4e11.3)') myid,side,axis,j1,j2, b0,b1,b3,br2
    ! write(*,'("            : myid=",i3," aa,bb,cc,dd,ee=",5e11.3)') myid,aa(j1,j2,j3),bb(j1,j2,j3),cc(j1,j2,j3),dd(j1,j2,j3),ee(j1,j2,j3)

  else if( mask(i1,i2,i3).lt.0 ) then
    fillExtrapolation4(j1,j2,j3,b,c,d,e,a)
  end if
 endLoop()

#endMacro

! Define 3d, order=4, PDE.r + Neumann BC: 
#Include "neumannEquationLineBC3d.h"

! ============================================================================
! Fourth-order Neumann equation and normal derivative of equation
!    ***New version: urrr + br2*urr - b0*u = gb
! 
!   See ogmg/doc/neumann.maple
! ============================================================================
#beginMacro neumannAndEquationCurvilinear3d(DIR,aa,bb,cc,dd,ee)

 if( equationToSolve.ne.laplaceEquation )then
   write(*,'("Ogmg:LSB:ERROR: equation!=laplace")')
   write(*,'("equationToSolve=",i2)') equationToSolve
   write(*,'("gridType=",i2)') gridType
   write(*,'("sparseStencil=",i2)') sparseStencil

   stop 5054
 end if

 nsign = 2*side-1

 ! write(*,'(''MGLS: neumannAndEquationCurvilinear: a0,a1='',2f6.2)') a0,a1

 diag=0. ! for now
 beginLoopJ(l1a,l1b,l2a,l2b,l3a,l3b)
  i1=j1+2*is1 ! (i1,i2,i3) is the boundary point
  i2=j2+2*is2 ! (j1,j2,j3) is the 2nd ghost line 
  i3=j3+2*is3
  if( mask(i1,i2,i3).gt.0 ) then

   ! We need 2 parameteric and 1 real derivative. Do this for now: 
   opEvalJacobianDerivatives(aj,2)

   neumannAndEquationLineBC3dOrder4(DIR,LHS)

   ! write(*,'(''LS:matrix: side,axis,axisp1,b0,b1,b3,br2 ='',2i2,i3,4e11.3)') side,axis,axisp1,b0,b1,b3,br2

   ! bn2 = -(-cRRr*anR**3*cRR+cRT*cRRt*anR**3+cRS*cRRs*anR**3-ccR*anR**3*cRR)/anR**3/cRR**2

   ! write(*,'("LS-NE4:matrix: i1,i2=",2i3," Values:")') i1,i2
   ! write(*,'("  cRR,anR,cRRr,cRT,cRRt,cRS,cRRs,ccR=",8e11.3)') cRR,anR,cRRr,cRT,cRRt,cRS,cRRs,ccR

! b0  = -(2*cRS**2*anRs*a0s*anR+cRS**2*anRss*anR*a0-2*cRS*cRT*a0st*anR**2-cRS*ccR*anR**2*a0s-cRS*cRSs*anR**2*a0s-cRS*cRTs*anR**2*a0t-cRS*ccRs*anR**2*a0+cSS*cRR*a0ss*anR**2+2*cSS*cRR*anRs**2*a0-cRT*ccR*anR**2*a0t-cRT*cRSt*anR**2*a0s+cRT*c0t*anR**3-c0r*anR**3*cRR-cRS**2*a0ss*anR**2-2*cRS**2*anRs**2*a0+cRS*c0s*anR**3-cRT**2*a0tt*anR**2-2*cRT**2*anRt**2*a0+2*cRS*cRT*anRs*a0t*anR-4*cRS*cRT*anRs*anRt*a0+2*cRS*cRT*anRt*a0s*anR+2*cRS*cRT*anRst*anR*a0+cRS*ccR*anR*anRs*a0+cRS*cRSs*anR*anRs*a0+cRS*cRTs*anR*anRt*a0-2*cSS*cRR*anRs*a0s*anR-cSS*cRR*anRss*anR*a0-cRT*cRTt*anR**2*a0t-cRT*ccRt*anR**2*a0+cRT**2*anRtt*anR*a0+2*cRT**2*anRt*a0t*anR+cST*cRR*a0st*anR**2+cTT*cRR*a0tt*anR**2+2*cTT*cRR*anRt**2*a0+ccS*anR**2*cRR*a0s+ccT*anR**2*cRR*a0t+c0*anR**2*cRR*a0+cRSr*anR**2*cRR*a0s+cRTr*anR**2*cRR*a0t+ccRr*anR**2*cRR*a0+cRT*ccR*anR*anRt*a0+cRT*cRSt*anR*anRs*a0+cRT*cRTt*anR*anRt*a0-cST*cRR*anRs*a0t*anR+2*cST*cRR*anRs*anRt*a0-cST*cRR*anRt*a0s*anR-cST*cRR*anRst*anR*a0-cTT*cRR*anRtt*anR*a0-2*cTT*cRR*anRt*a0t*anR-ccS*anR*cRR*anRs*a0-ccT*anR*cRR*anRt*a0-cRSr*anR*cRR*anRs*a0-cRTr*anR*cRR*anRt*a0)/anR**3/cRR**2

   ! write(*,'("anRs,a0s,anRss,a0st,cRTs,a0t,a0,cSS,a0ss=",9e10.2)') anRs,a0s,anRss,a0st,cRTs,a0t,a0,cSS,a0ss

   ! write(*,'(" b0,bn2 =",4e11.3)') b0,bn2
  
   aa(j1,j2,j3)=-.5/dr(axis)**3
   bb(j1,j2,j3)= 1./dr(axis)**3    +bn2/dr(axis)**2
   cc(j1,j2,j3)=                -2.*bn2/dr(axis)**2 - b0 
   dd(j1,j2,j3)=-1./dr(axis)**3    +bn2/dr(axis)**2
   ee(j1,j2,j3)= .5/dr(axis)**3
  else if( mask(i1,i2,i3).lt.0 ) then
    fillExtrapolation4(j1,j2,j3,b,c,d,e,a)
  end if
 endLoop()

#endMacro



! =================================================================================
! Fill the matrix with the equations for parallel ghost boundaries (dirichlet)
! =================================================================================
#beginMacro fillMatrixParallelGhost()
if( orderOfAccuracy.eq.2 )then

  getBoundaryIndex(side,axis,m1a,m1b,m2a,m2b,m3a,m3b,0)   ! first ghost line
  beginLoop(m1a,m1b,m2a,m2b,m3a,m3b)
   if( mask(i1,i2,i3) .gt. 0 ) then
    a(i1,i2,i3)=0.
    b(i1,i2,i3)=1.
    c(i1,i2,i3)=0.
   end if
  endLoop()

else if( orderOfAccuracy.eq.4 )then

  ! NOTE: peta-diagonal matrix always has "c" on the diagonal
  getBoundaryIndex(side,axis,m1a,m1b,m2a,m2b,m3a,m3b,0)   ! second parallel ghost line

  beginLoop(m1a,m1b,m2a,m2b,m3a,m3b)
   if( mask(i1,i2,i3) .gt. 0 ) then
    a(i1,i2,i3)=0.
    b(i1,i2,i3)=0.
    c(i1,i2,i3)=1.
    d(i1,i2,i3)=0.
    e(i1,i2,i3)=0.
   end if
   j1=i1+is1 ! (j1,j2,j3) is the 1st parallel ghost
   j2=i2+is2
   j3=i3+is3 
   if( mask(j1,j2,j3) .gt. 0 ) then
    a(j1,j2,j3)=0.
    b(j1,j2,j3)=0.
    c(j1,j2,j3)=1.
    d(j1,j2,j3)=0.
    e(j1,j2,j3)=0.
   end if
  endLoop()

end if            
#endMacro

! ------------------------------------------------------------------------------------------------
! Macro to declare variables for the 4th order Neumann BC's
! ------------------------------------------------------------------------------------------------
#beginMacro declareNeumannEquationVariables()

 integer ax1,ax2
 integer iv(0:2),dv(0:2),mdim(0:1,0:2)
 
 real n1,n1r,n1rr, n1s,n1ss, n1t,n1tt, n1rs, n1rt, n1st
 real n2,n2r,n2rr, n2s,n2ss, n2t,n2tt, n2rs, n2rt, n2st 
 real n3,n3r,n3rr, n3s,n3ss, n3t,n3tt, n3rs, n3rt, n3st 
 real an1,an1s,an1ss,an2,an2s,an2ss,an1r,an1rr,an2r,an2rs,an2rr, an3
 real ff,ffs,ffr,g,gs,gss,gr,grr,grs,grt, gt,gst,gtt, fft,ffst,fftt
 real c11,c11r,c11s,c12,c12r,c12s,c22,c22r,c22s,c1,c1r,c1s,c2,c2r,c2s
 real b0,b1,b2,b3,bf

 real br,brr,brrr,bs,bss,bsss,bt,btt,bttt, brs,brt,bst, brrs, brrt, brss, brtt, bsst, bstt, brst, br2, bn1,bn2,bn3
 real cxx,cyy,czz,cxy,cxz,cyz,cx,cy,cz,c0
 real cRR,cSS,cTT,cRS,cRT,cST,ccR,ccS,ccT
 real cRRr,cSSr,cTTr,cRSr,cRTr,cSTr,ccRr,ccSr,ccTr,c0r
 real cRRs,cSSs,cTTs,cRSs,cRTs,cSTs,ccRs,ccSs,ccTs,c0s
 real cRRt,cSSt,cTTt,cRSt,cRTt,cSTt,ccRt,ccSt,ccTt,c0t

 real ani,anir,anis,anit, anirr,anirs, anirt, aniss, anist, anitt
 real anR, anRr,anRs,anRt, anRrr,anRrs, anRrt, anRss, anRst, anRtt
 real anS, anSr,anSs,anSt, anSrr,anSrs, anSrt, anSss, anSst, anStt
 real anT, anTr,anTs,anTt, anTrr,anTrs, anTrt, anTss, anTst, anTtt
 real a0r,a0s,a0t, a0rr,a0ss,a0tt,a0rs,a0rt,a0st
 real bogus

 real ajrxxr
 real ajrxxs
 real ajrxxt
 real ajrxyr
 real ajrxys
 real ajrxyt
 real ajrxzr
 real ajrxzs
 real ajrxzt
 real ajryxr
 real ajryxs
 real ajryxt
 real ajryyr
 real ajryys
 real ajryyt
 real ajryzr
 real ajryzs
 real ajryzt
 real ajrzxr
 real ajrzxs
 real ajrzxt
 real ajrzyr
 real ajrzys
 real ajrzyt
 real ajrzzr
 real ajrzzs
 real ajrzzt
 real ajsxxr
 real ajsxxs
 real ajsxxt
 real ajsxyr
 real ajsxys
 real ajsxyt
 real ajsxzr
 real ajsxzs
 real ajsxzt
 real ajsyxr
 real ajsyxs
 real ajsyxt
 real ajsyyr
 real ajsyys
 real ajsyyt
 real ajsyzr
 real ajsyzs
 real ajsyzt
 real ajszxr
 real ajszxs
 real ajszxt
 real ajszyr
 real ajszys
 real ajszyt
 real ajszzr
 real ajszzs
 real ajszzt
 real ajtxxr
 real ajtxxs
 real ajtxxt
 real ajtxyr
 real ajtxys
 real ajtxyt
 real ajtxzr
 real ajtxzs
 real ajtxzt
 real ajtyxr
 real ajtyxs
 real ajtyxt
 real ajtyyr
 real ajtyys
 real ajtyyt
 real ajtyzr
 real ajtyzs
 real ajtyzt
 real ajtzxr
 real ajtzxs
 real ajtzxt
 real ajtzyr
 real ajtzys
 real ajtzyt
 real ajtzzr
 real ajtzzs
 real ajtzzt

 declareTemporaryVariables(2,2)
 declareJacobianDerivativeVariables(aj,3)     ! declareJacobianDerivativeVariables(aj,DIM)
#endMacro

#beginMacro setBogus10(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10)
 x1=bogus
 x2=bogus
 x3=bogus
 x4=bogus
 x5=bogus
 x6=bogus
 x7=bogus
 x8=bogus
 x9=bogus
 x10=bogus
#endMacro
#beginMacro setBogus15(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15)
 x1=bogus
 x2=bogus
 x3=bogus
 x4=bogus
 x5=bogus
 x6=bogus
 x7=bogus
 x8=bogus
 x9=bogus
 x10=bogus
 x11=bogus
 x12=bogus
 x13=bogus
 x14=bogus
 x15=bogus
#endMacro

#beginMacro setZero10(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10)
 x1=0.
 x2=0.
 x3=0.
 x4=0.
 x5=0.
 x6=0.
 x7=0.
 x8=0.
 x9=0.
 x10=0.
#endMacro
 
      subroutine lineSmoothBuild( nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &    nda1a,nda1b,nda2a,nda2b,nda3a,nda3b,
     &    ndc, coeff, cc, a, b, c, d, e, s, u, f, mask, rsxy, ipar, rpar, ndbcd,bcData )
! ===================================================================================
!  Line smooth -- build the tridiagonal/pentadiagonal matrices
!
!  a,b,c : for tridiagonal
!  a,b,c,d,e : for pentadiagonal
!
! Order of penta-diangonal entries: 
!     [ c d e a b
!     [ b c d e a
!     [ a b c d e 
!     [   a b c d e 
!     [     a b c d e 
!             . . . . .
!                a b c d e   ]
!                  a b c d e ]
!                  e a b c d ]
!                  d e a b c ]
!
! ===================================================================================

      implicit none
      integer nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,ndc,ndbcd
      integer nda1a,nda1b,nda2a,nda2b,nda3a,nda3b

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real bcData(0:ndbcd-1,0:1,0:2)
      integer ipar(0:*)

      real a(nda1a:nda1b,nda2a:nda2b,nda3a:nda3b)
      real b(nda1a:nda1b,nda2a:nda2b,nda3a:nda3b)
      real c(nda1a:nda1b,nda2a:nda2b,nda3a:nda3b)
      real d(nda1a:nda1b,nda2a:nda2b,nda3a:nda3b)
      real e(nda1a:nda1b,nda2a:nda2b,nda3a:nda3b)

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real f(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real coeff(0:ndc-1,nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real s(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real cc(0:*)
      real rpar(0:*)

!....local variables
      integer n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c
      integer sparseStencil,orderOfAccuracy,bcOptionD,bcOptionN,myid
      integer i1,i2,i3,m1a,m1b,m1c,m2a,m2b,m2c,m3a,m3b,m3c,j1,j2,j3,is1,is2,is3,kd,is
      integer i1m1,i1p1,i2m1,i2p1,i3m1,i3p1,k1,k2,k3
      integer l1a,l1b,l2a,l2b,l3a,l3b
      integer grid,level,mm

      integer direction,width,width2,md,mdm1,mdp1,mdm2,mdp2,side,md2,md2m1,md2p1,ms2
      integer axis,axisp1,axisp2
      integer bc(0:1,0:2),orderOfExtrapD,orderOfExtrapN,useBoundaryForcing,isNeumannBC(0:1)
      real dx(0:2),dr(0:2)
      real dxi,dx2i,dxm,diag,nsign,aNormi

      real a0,a1,a2,alpha1,alpha2
      real rxi,ryi,sxi,syi,rxr,rxs,sxr,sxs,ryr,rys,syr,sys
      real rxxi,ryyi,sxxi,syyi
      real rxrr,rxrs,rxss,ryrr,ryrs,ryss
      real sxrr,sxrs,sxss,syrr,syrs,syss
      real rxx,ryy,sxx,syy
      real rxxr,ryyr,rxxs,ryys, sxxr,syyr,sxxs,syys
      real rxNormI,rxNormIs,rxNormIss,rxNormIr,rxNormIrr
      real sxNormI,sxNormIs,sxNormIss,sxNormIr,sxNormIrr

      real fv(-1:1,-1:1,-1:1), gv(-1:1,-1:1,-1:1)

      ! Delare variables for the order 4 Neumann BCs
      declareNeumannEquationVariables()


      integer general, sparse, constantCoefficients, 
     &   sparseConstantCoefficients,sparseVariableCoefficients,
     &   variableCoefficients
      parameter( general=0, 
     &           sparse=1, 
     &           constantCoefficients=2,
     &           sparseConstantCoefficients=3,
     &           sparseVariableCoefficients=4,
     &           variableCoefficients=5 )

      integer dirichlet,neumann,mixed,equation,extrapolation,
     &        combination,equationToSecondOrder,mixedToSecondOrder,
     &        evenSymmetry,oddSymmetry,extrapolateTwoGhostLines,parallelGhostBoundary

      parameter( 
     &     dirichlet=1,
     &     neumann=2,
     &     mixed=3,
     &     equation=4,
     &     extrapolation=5,
     &     combination=6,
     &     equationToSecondOrder=7,
     &     mixedToSecondOrder=8,
     &     evenSymmetry=9,
     &     oddSymmetry=10,
     &     extrapolateTwoGhostLines=11,
     &     parallelGhostBoundary=20 )

      integer gridType
      integer rectangular,curvilinear
      parameter(
     &     rectangular=0,
     &     curvilinear=1)

      integer equationToSolve
      integer userDefined,laplaceEquation,divScalarGradOperator,
     &  heatEquationOperator,variableHeatEquationOperator,
     &   divScalarGradHeatOperator,secondOrderConstantCoefficients,
     & axisymmetricLaplaceEquation
      parameter(
     & userDefined=0,
     & laplaceEquation=1,
     & divScalarGradOperator=2,              ! div[ s[x] grad ]
     & heatEquationOperator=3,               ! I + c0*Delta
     & variableHeatEquationOperator=4,       ! I + s[x]*Delta
     & divScalarGradHeatOperator=5,  ! I + div[ s[x] grad ]
     & secondOrderConstantCoefficients=6,
     & axisymmetricLaplaceEquation=7 )

      real rx,ry,rz,sx,sy,sz,tx,ty,tz

      declareDifferenceOrder2(u,RX)
      declareDifferenceOrder4(u,RX)

!....start statement functions 

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
      defineDifferenceOrder2Components0(u,RX)
      defineDifferenceOrder4Components0(u,RX)
!....end statement function

      nd              =ipar(0)
      direction       =ipar(1)
      sparseStencil   =ipar(2)
      orderOfAccuracy =ipar(3)
      n1a             =ipar(4)
      n1b             =ipar(5)
      n1c             =ipar(6)
      n2a             =ipar(7)
      n2b             =ipar(8)
      n2c             =ipar(9)
      n3a             =ipar(10)
      n3b             =ipar(11)
      n3c             =ipar(12)
      bc(0,0)         =ipar(13)
      bc(1,0)         =ipar(14)
      bc(0,1)         =ipar(15)
      bc(1,1)         =ipar(16)
      bc(0,2)         =ipar(17)
      bc(1,2)         =ipar(18)
      bcOptionD       =ipar(19)  ! BC option for Dirichlet BC's
      bcOptionN       =ipar(20)  ! BC option for Neumann BC's
      orderOfExtrapD  =ipar(21)  ! for dirichlet
      orderOfExtrapN  =ipar(22)  ! for neumann
      gridType        =ipar(23)

      level           =ipar(31)
      equationToSolve =ipar(32)
      useBoundaryForcing=ipar(33)
      isNeumannBC(0)  =ipar(34)      
      isNeumannBC(1)  =ipar(35)      

      myid            =ipar(36)

      dx(0)           =rpar(0)
      dx(1)           =rpar(1)
      dx(2)           =rpar(2)
      dr(0)           =rpar(3) ! **** added 
      dr(1)           =rpar(4)
      dr(2)           =rpar(5)

      bogus=1.e20
      if( .true. )then

       setBogus10(n1,n1r,n1rr, n1s,n1ss, n1t,n1tt, n1rs, n1rt, n1st)
       setBogus10(n2,n2r,n2rr, n2s,n2ss, n2t,n2tt, n2rs, n2rt, n2st)
       setBogus10(n3,n3r,n3rr, n3s,n3ss, n3t,n3tt, n3rs, n3rt, n3st)
       setBogus10(an1,an1s,an1ss,an2,an2s,an2ss,an1r,an1rr,an2r,an2rs)
       setBogus15(an2rr,ff,ffs,ffr,g,gs,gss,gr,grr,grs,grt, gt,gst,gtt, fft)
       setBogus10(ffst,fftt, ffst,fftt, ffst,fftt, ffst,fftt, ffst,fftt )
       setBogus15(c11,c11r,c11s,c12,c12r,c12s,c22,c22r,c22s,c1,c1r,c1s,c2,c2r,c2s)
 
       setBogus10(a0,a0r,a0s,a0t,a0rr,a0ss,a0tt,a0rs,a0rt,a0st)

       setBogus10(b0,b1,b2,b3,bf,br2, b0,b1,b2,b3)
       setBogus10(cxx,cyy,czz,cxy,cxz,cyz,cx,cy,cz,c0)
       setBogus10(cRR,cSS,cTT,cRS,cRT,cST,ccR,ccS,ccT,ccT)
       setBogus10(cRRr,cSSr,cTTr,cRSr,cRTr,cSTr,ccRr,ccSr,ccTr,c0r)
       setBogus10(cRRt,cSSt,cTTt,cRSt,cRTt,cSTt,ccRt,ccSt,ccTt,c0t)
 
       setBogus10(ani,anir,anis,anit, anirr,anirs, anirt, aniss, anist, anitt)
       setBogus10(anR, anRr,anRs,anRt, anRrr,anRrs, anRrt, anRss, anRst, anRtt)
       setBogus10(anS, anSr,anSs,anSt, anSrr,anSrs, anSrt, anSss, anSst, anStt)
       setBogus10(anT, anTr,anTs,anTt, anTrr,anTrs, anTrt, anTss, anTst, anTtt)
 
       setBogus15(br,brr,brrr,bs,bss,bsss,bt,btt,bttt, brs,brt,bst, brrs, brrt, brss)
       setBogus10(brtt, bsst, bstt, brst, br2, bn1,bn2,bn3, bn3,bn3 )

       ajrxxr=bogus
       ajrxxs=bogus
       ajrxxt=bogus
       ajrxyr=bogus
       ajrxys=bogus
       ajrxyt=bogus
       ajrxzr=bogus
       ajrxzs=bogus
       ajrxzt=bogus
       ajryxr=bogus
       ajryxs=bogus
       ajryxt=bogus
       ajryyr=bogus
       ajryys=bogus
       ajryyt=bogus
       ajryzr=bogus
       ajryzs=bogus
       ajryzt=bogus
       ajrzxr=bogus
       ajrzxs=bogus
       ajrzxt=bogus
       ajrzyr=bogus
       ajrzys=bogus
       ajrzyt=bogus
       ajrzzr=bogus
       ajrzzs=bogus
       ajrzzt=bogus
       ajsxxr=bogus
       ajsxxs=bogus
       ajsxxt=bogus
       ajsxyr=bogus
       ajsxys=bogus
       ajsxyt=bogus
       ajsxzr=bogus
       ajsxzs=bogus
       ajsxzt=bogus
       ajsyxr=bogus
       ajsyxs=bogus
       ajsyxt=bogus
       ajsyyr=bogus
       ajsyys=bogus
       ajsyyt=bogus
       ajsyzr=bogus
       ajsyzs=bogus
       ajsyzt=bogus
       ajszxr=bogus
       ajszxs=bogus
       ajszxt=bogus
       ajszyr=bogus
       ajszys=bogus
       ajszyt=bogus
       ajszzr=bogus
       ajszzs=bogus
       ajszzt=bogus
       ajtxxr=bogus
       ajtxxs=bogus
       ajtxxt=bogus
       ajtxyr=bogus
       ajtxys=bogus
       ajtxyt=bogus
       ajtxzr=bogus
       ajtxzs=bogus
       ajtxzt=bogus
       ajtyxr=bogus
       ajtyxs=bogus
       ajtyxt=bogus
       ajtyyr=bogus
       ajtyys=bogus
       ajtyyt=bogus
       ajtyzr=bogus
       ajtyzs=bogus
       ajtyzt=bogus
       ajtzxr=bogus
       ajtzxs=bogus
       ajtzxt=bogus
       ajtzyr=bogus
       ajtzys=bogus
       ajtzyt=bogus
       ajtzzr=bogus
       ajtzzs=bogus
       ajtzzt=bogus
      end if



      ! Initialize a0 and derivatives (a0 is the coeff u in the Mixed BC) -- for now a0 is constant (a0 is set later)
      setZero10(a0,a0r,a0s,a0t,a0rr,a0ss,a0tt,a0rs,a0rt,a0st)

      ! write(*,'(''lineSmoothBuild: level,bcOptionD,bcOptionN='',3i2)') level,bcOptionD,bcOptionN

      axis=direction ! do axis=0,nd
      axisp1=mod(axis+1,nd)
      axisp2=mod(axis+2,nd)


      width = orderOfAccuracy+1   ! 3 or 5

      width2=3  ! for 2nd order

      ! md =  diagonal term
      if( nd.eq.2 )then
        md=(width*width)/2      ! 4 or 12 
        md2=(3*3)/2             ! for a 2nd order-accurate stencil
      else if( nd.eq.3 )then
        md=(width*width*width)/2 ! 13 or 62
        md2=(3*3*3)/2
      else
        md=width/2              ! 1
        md2=3/2
      end if

      ! form the tridiagonal matrices 
      if( direction.eq.0 )then
        mdm2=md-2
        mdm1=md-1
        mdp1=md+1
        mdp2=md+2

        md2m1=md2-1
        md2p1=md2+1

        ms2 = 1 ! shift for 2nd-order stencil in a fourth order operator
      else if( direction.eq.1 )then
        mdm2=md-2*width
        mdm1=md-  width
        mdp1=md+  width
        mdp2=md+2*width

        md2m1=md2-width2
        md2p1=md2+width2

        ms2 = width ! shift for 2nd-order stencil in a fourth order operator
      else if( direction.eq.2 )then
        mdm2=md-2*width**2
        mdm1=md-  width**2
        mdp1=md+  width**2
        mdp2=md+2*width**2

        md2m1=md2-width2**2
        md2p1=md2+width2**2

        ms2 = width*width ! shift for 2nd-order stencil in a fourth order operator
      else
        write(*,*) 'lineSmoothFactor:ERROR: invalid direction! '
      end if
      
      

      if( sparseStencil.eq.constantCoefficients .or. sparseStencil.eq.sparseConstantCoefficients ) then

        ! ================================================
        ! ========== constant coefficients ===============
        ! ================================================

        if( orderOfAccuracy.eq.2 )then

          beginLoop(n1a,n1b,n2a,n2b,n3a,n3b)
	    if( mask(i1,i2,i3) .gt. 0 ) then
	      a(i1,i2,i3)=cc(mdm1)
	      b(i1,i2,i3)=cc(md)
	      c(i1,i2,i3)=cc(mdp1)
!  write(*,'('' i1,i2,i3,a,b,c='',3i3,3f8.2)') i1,i2,i3,a(i1,i2,i3),b(i1,i2,i3),c(i1,i2,i3)
	    else 
	      a(i1,i2,i3)=0.
	      b(i1,i2,i3)=1.
	      c(i1,i2,i3)=0.
            end if
          endLoop()

        else if( orderOfAccuracy.eq.4 )then

          beginLoop(n1a,n1b,n2a,n2b,n3a,n3b)
	    if( mask(i1,i2,i3) .gt. 0 ) then
	      a(i1,i2,i3)=cc(mdm2)
	      b(i1,i2,i3)=cc(mdm1)
	      c(i1,i2,i3)=cc(md)
	      d(i1,i2,i3)=cc(mdp1)
	      e(i1,i2,i3)=cc(mdp2)
	    else 
	      a(i1,i2,i3)=0.
	      b(i1,i2,i3)=0.
	      c(i1,i2,i3)=1.
	      d(i1,i2,i3)=0.
	      e(i1,i2,i3)=0.
            end if
          endLoop()

        else
          write(*,*) 'lineSmoothBuild: invalid orderOfAccuracy=',orderOfAccuracy
          stop 6
        end if

        dxi = 1./dx(direction)
        dx2i = dxi*dxi
        dxm = 1./dx(0)**2 + 1./dx(1)**2
        if( nd.eq.3 )then
          dxm=dxm+1./dx(2)**2
        end if
 
        is1=0
        is2=0
        is3=0
        do side=0,1
          if( axis.eq.0 )then
            is1=1-2*side
          else if( axis.eq.1 )then
            is2=1-2*side
          else
            is3=1-2*side
          end if
          if( bc(side,axis).gt.0 )then

            if( bc(side,axis).eq.parallelGhostBoundary )then
              ! parallel ghost boundaries get a dirichlet condition
              fillMatrixParallelGhost()

  	    else if( bc(side,axis).eq.extrapolation )then

            ! **********************************************************
            ! ******************** Dirichlet BC ************************
            ! **********************************************************
  
              if( orderOfAccuracy.eq.2 )then

                getBoundaryIndex(side,axis,m1a,m1b,m2a,m2b,m3a,m3b,0)
                beginLoop(m1a,m1b,m2a,m2b,m3a,m3b)
  	         if( mask(i1,i2,i3) .gt. 0 ) then
  		  a(i1,i2,i3)=0.
  		  b(i1,i2,i3)=1.
  		  c(i1,i2,i3)=0.
                 end if
                endLoop()

              else if( orderOfAccuracy.eq.4 )then

                getBoundaryIndex(side,axis,l1a,l1b,l2a,l2b,l3a,l3b,0)  ! first ghost line
                getBoundaryIndex(side,axis,m1a,m1b,m2a,m2b,m3a,m3b,1)  ! boundary

                if( side.eq.0 )then
                 ! 1st ghost line on left:
                 !       [  c  d  e  a  b ]
                 !      i= -1 -0  1  2  3
                  if( bcOptionD.eq.0 )then
                   ! extrapolate
                   extrapFirstGhost(left,l1a,l1b,l2a,l2b,l3a,l3b)
                  else if( bcOptionD.eq.1 )then
                    ! use eqn to 2nd order on the boundary
                    if( equationToSolve.ne.laplaceEquation )then
                      write(*,*) "Ogmg:LSB:ERROR: equation.ne.laplace bcOptionD=",bcOptionD
                      stop 5051
                    end if

                    beginLoop(l1a,l1b,l2a,l2b,l3a,l3b)
                     j1=i1+is1 ! (j1,j2,j3) is the boundary point
                     j2=i2+is2
                     j3=i3+is3
  	             if( mask(j1,j2,j3).gt.0 ) then
	              c(i1,i2,i3)= dx2i
	              d(i1,i2,i3)=-2.*dxm
	              e(i1,i2,i3)= dx2i
	              a(i1,i2,i3)= 0.
	              b(i1,i2,i3)= 0.
                     else if( mask(j1,j2,j3).lt.0 ) then
                       fillExtrapolation4(i1,i2,i3,c,d,e,a,b)
                     end if
                    endLoop()
                  else if( bcOptionD.eq.2 )then
                    oddSymmetryFirstGhost(left,l1a,l1b,l2a,l2b,l3a,l3b)
                  else
                    stop 71
                  end if
                  ! boundary on left:
                  !       [  b  c  d  e  a ]
                  !      i= -1  0  1  2  3
                  beginLoop(m1a,m1b,m2a,m2b,m3a,m3b)
  	           if( mask(i1,i2,i3) .gt. 0 ) then
	            b(i1,i2,i3)=0.
	            c(i1,i2,i3)=1.
	            d(i1,i2,i3)=0.
	            e(i1,i2,i3)=0.
	            a(i1,i2,i3)=0.
                   end if
                  endLoop()

                else ! side.eq.1

                  ! 1st ghost line on right:
                  !       [  d  e  a  b  c ]
                  !   i=n+[ -3 -2 -1  0  1 ]

                  if( bcOptionD.eq.0 )then
                    ! extrapolate
                   extrapFirstGhost(right,l1a,l1b,l2a,l2b,l3a,l3b)

                  else if( bcOptionD.eq.1 )then
                    ! use eqn to 2nd order on the boundary for first ghost
                    if( equationToSolve.ne.laplaceEquation )then
                      write(*,'("Ogmg:LSB:ERROR: equation!=laplace")')
                      stop 5052
                    end if
                    beginLoop(l1a,l1b,l2a,l2b,l3a,l3b)
                     j1=i1+is1 ! (j1,j2,j3) is the boundary point
                     j2=i2+is2
                     j3=i3+is3
  	             if( mask(j1,j2,j3).gt.0 ) then
	              d(i1,i2,i3)= 0.
	              e(i1,i2,i3)= 0.
	              a(i1,i2,i3)= dx2i
	              b(i1,i2,i3)=-2.*dxm
	              c(i1,i2,i3)= dx2i
                     else if( mask(j1,j2,j3).lt.0 ) then
                       fillExtrapolation4(i1,i2,i3,c,b,a,e,d)
                     end if
                    endLoop()
                  else if( bcOptionD.eq.2 )then
                    oddSymmetryFirstGhost(right,l1a,l1b,l2a,l2b,l3a,l3b)
                  else
                    stop 6
                  end if
                  ! boundary on right:
                  !       [  e  a  b  c  d ]
                  !   i=n+[ -3 -2 -1  0  1 ]
                  beginLoop(m1a,m1b,m2a,m2b,m3a,m3b)
  	           if( mask(i1,i2,i3) .gt. 0 ) then
	            e(i1,i2,i3)=0.
	            a(i1,i2,i3)=0.
	            b(i1,i2,i3)=0.
	            c(i1,i2,i3)=1.
	            d(i1,i2,i3)=0.
                   end if
                  endLoop()
                end if
              else
                write(*,*) 'lineSmoothBuild: invalid orderOfAccuracy=',orderOfAccuracy
                stop 6
              end if

            else if( axis.eq.direction )then ! only apply Neumman BC if axis==direction

              ! ********************************************************
              ! *****************NEUMANN RECTANGULAR *******************
              ! ********************************************************
  	      ! apply a neumann or mixed BC on this side.

              !    a0*u + a1*u.n = f
       	      a0=bcData(0,side,axis)
  	      a1=bcData(1,side,axis)
              if( a0.eq.0. .and. a1.eq.0. )then
                write(*,*) 'lineSmoothBuild:ERROR: a0 and a1 are both zero'
                write(*,*) 'side,axis,a0,a1,dxi=',side,axis,a0,a1,dxi
                stop 5
              end if	      
              if( orderOfAccuracy.eq.2 )then

               getBoundaryIndex(side,axis,m1a,m1b,m2a,m2b,m3a,m3b,0)  ! first ghost line

               if( side.eq.0 )then
                 !       [  b  c  a  ]
                 !         -1  0  1  
                beginLoop(m1a,m1b,m2a,m2b,m3a,m3b)
                  j1=i1+is1 ! (j1,j2,j3) is the boundary point
                  j2=i2+is2
                  j3=i3+is3
  	          if( mask(j1,j2,j3).gt.0 ) then
  		    b(i1,i2,i3)= a1*dxi*.5
  		    c(i1,i2,i3)= a0
  		    a(i1,i2,i3)=-a1*dxi*.5
                  else if( mask(j1,j2,j3).lt.0 )then
                    fillExtrapolation2(i1,i2,i3,b,c,a)
                  end if
                endLoop()
  	       else
                 !       [  c  a  b  ]
                 !         -1  0  1  
                beginLoop(m1a,m1b,m2a,m2b,m3a,m3b)
                  j1=i1+is1 ! (j1,j2,j3) is the boundary point
                  j2=i2+is2
                  j3=i3+is3
  	          if( mask(j1,j2,j3).gt.0 ) then
  		   c(i1,i2,i3)=-a1*dxi*.5
  		   a(i1,i2,i3)= a0
  		   b(i1,i2,i3)= a1*dxi*.5
                  else if( mask(j1,j2,j3).lt.0 )then
                    fillExtrapolation2(i1,i2,i3,c,a,b)
                  end if
                endLoop()
               end if

              else if( orderOfAccuracy.eq.4 )then

               getBoundaryIndex(side,axis,l1a,l1b,l2a,l2b,l3a,l3b,0)  ! 2nd ghost line
               getBoundaryIndex(side,axis,m1a,m1b,m2a,m2b,m3a,m3b,1)  ! first ghost line

               if( side.eq.0 )then
                 ! 2nd ghost line on left:
                 !       [  c  d  e  a  b ]
                 !     i=[ -2 -1  0  1  2 ]
                if( bcOptionN.eq.0 )then
                 ! extrapolate 2nd ghost
                 beginLoop(l1a,l1b,l2a,l2b,l3a,l3b)
                  ! *wdh* 110309: 
                  fillExtrapolationPentaDiagonal(i1,i2,i3,c,d,e,a,b,orderOfExtrapN)
	           ! c(i1,i2,i3)= 1.
	           ! d(i1,i2,i3)=-4.
	           ! e(i1,i2,i3)= 6.
	           ! a(i1,i2,i3)=-4.
	           ! b(i1,i2,i3)= 1.
                 endLoop()
                else if( bcOptionN.eq.1 )then
                 ! u.xxx = 
                 neumannAndEquationRectangular(c,d,e,a,b)
!$$$                 beginLoop(l1a,l1b,l2a,l2b,l3a,l3b)
!$$$                  j1=i1+is1 ! (j1,j2,j3) is the boundary point
!$$$                  j2=i2+is2
!$$$                  j3=i3+is3
!$$$  	          if( mask(j1,j2,j3).gt.0 ) then
!$$$	           c(i1,i2,i3)=-1.
!$$$	           d(i1,i2,i3)= 2.
!$$$	           e(i1,i2,i3)= 0.
!$$$	           a(i1,i2,i3)=-2.
!$$$	           b(i1,i2,i3)= 1.
!$$$                  else if( mask(j1,j2,j3).lt.0 ) then
!$$$                    fillExtrapolation4(i1,i2,i3,b,c,d,e,a)
!$$$                  end if
!$$$                 endLoop()

                else if( bcOptionN.eq.2 )then
                  evenSymmetrySecondGhost(left,l1a,l1b,l2a,l2b,l3a,l3b)
                else if( bcOptionN.eq.3 )then
                  ! mixed BC for both first and second ghost lines

                  ! *new* 110308 -- apply real 2nd-order approximations on two ghost 
                  ! write(*,'(">>>lineSmoothBuild: 2nd order Neumann/mixed on TWO lines")')

                  mixedToSecondOrderPentaDiagonalTwoLines(rectangular,left)
                else 
                  stop 12
                end if
                 ! 1st ghost line on left:
                 !       [  b  c  d  e  a ]
                 !     i=[ -2 -1  0  1  2 ]
                if( bcOptionN.eq.0 .or. bcOptionN.eq.1 )then
                 beginLoop(m1a,m1b,m2a,m2b,m3a,m3b)
                  j1=i1+is1 ! (j1,j2,j3) is the boundary point
                  j2=i2+is2
                  j3=i3+is3
  	          if( mask(j1,j2,j3).gt.0 ) then
	           b(i1,i2,i3)=-a1*dxi/12.    
	           c(i1,i2,i3)= a1*dxi*8./12. 
	           d(i1,i2,i3)= a0            
	           e(i1,i2,i3)=-a1*dxi*8./12. 
	           a(i1,i2,i3)= a1*dxi/12.    
                  else if( mask(j1,j2,j3).lt.0 ) then
                    fillExtrapolation4(i1,i2,i3,b,c,d,e,a)
                  end if
                 endLoop()
                else if( bcOptionN.eq.2 ) then
                  evenSymmetryFirstGhost(left,m1a,m1b,m2a,m2b,m3a,m3b)
                else if( bcOptionN.eq.3 )then
                  ! mixed BC already done
                else
                  stop 14
                end if

  	       else ! side==1

                 ! 2nd ghost line on right:
                 !       [  d  e  a  b  c ]
                 !   i=n+[ -2 -1  0  1  2 ]
                 if( bcOptionN.eq.0 )then
                  ! extrapolate 2nd ghost
                  beginLoop(l1a,l1b,l2a,l2b,l3a,l3b)
                   fillExtrapolationPentaDiagonal(i1,i2,i3,c,b,a,e,d,orderOfExtrapN)
	           ! d(i1,i2,i3)= 1.
	           ! e(i1,i2,i3)=-4.
	           ! a(i1,i2,i3)= 6.
	           ! b(i1,i2,i3)=-4.
	           ! c(i1,i2,i3)= 1.
                  endLoop()
                 else if( bcOptionN.eq.1 )then
                  ! u.xxx = 
                  neumannAndEquationRectangular(d,e,a,b,c)

!$$$                  beginLoop(l1a,l1b,l2a,l2b,l3a,l3b)
!$$$                   j1=i1+is1 ! (j1,j2,j3) is the boundary point
!$$$                   j2=i2+is2
!$$$                   j3=i3+is3
!$$$  	           if( mask(j1,j2,j3).gt.0 ) then
!$$$	            d(i1,i2,i3)=-1.
!$$$	            e(i1,i2,i3)= 2.
!$$$	            a(i1,i2,i3)= 0.
!$$$	            b(i1,i2,i3)=-2.
!$$$	            c(i1,i2,i3)= 1.
!$$$                   else if( mask(j1,j2,j3).lt.0 ) then
!$$$                     fillExtrapolation4(i1,i2,i3,b,c,d,e,a)
!$$$                   end if
!$$$                  endLoop()

                 else if( bcOptionN.eq.2 )then
                  evenSymmetrySecondGhost(right,l1a,l1b,l2a,l2b,l3a,l3b)
                 else if( bcOptionN.eq.3 )then
                   ! mixed BC for both first and second ghost lines

                  ! *new* 110308 -- apply real 2nd-order approximations on two ghost 
                  ! write(*,'(">>>lineSmoothBuild: 2nd order Neumann/mixed on TWO lines")')

                   mixedToSecondOrderPentaDiagonalTwoLines(rectangular,right)
                 else 
                   stop 12
                 end if

                 ! 1st ghost line on right:
                 !       [  e  a  b  c  d ]
                 !  i=n+ [ -2 -1  0  1  2 ]
                 if( bcOptionN.eq.0 .or. bcOptionN.eq.1 )then
                  beginLoop(m1a,m1b,m2a,m2b,m3a,m3b)
                   j1=i1+is1 ! (j1,j2,j3) is the boundary point
                   j2=i2+is2
                   j3=i3+is3
  	           if( mask(j1,j2,j3).gt.0 ) then
	            e(i1,i2,i3)= a1*dxi/12.
	            a(i1,i2,i3)=-a1*dxi*8./12.
	            b(i1,i2,i3)= a0
	            c(i1,i2,i3)= a1*dxi*8./12.
	            d(i1,i2,i3)=-a1*dxi/12.    
                   else if( mask(j1,j2,j3).lt.0 ) then
                     fillExtrapolation4(i1,i2,i3,d,c,b,a,e)
                   end if
                  endLoop()
                 else if( bcOptionN.eq.2 ) then
                   evenSymmetryFirstGhost(right,m1a,m1b,m2a,m2b,m3a,m3b)
                 else if( bcOptionN.eq.3 )then
                   ! mixed BC alread done
                  else
                   stop 14
                 end if
               end if  ! side

              else
                write(*,*) 'lineSmoothBuild: invalid orderOfAccuracy=',orderOfAccuracy
                stop 6
              end if

            end if
          end if

        end do ! end do side
	      
      else

        ! ================================================================
        ! ========== Curvilinear and variable coefficients ===============
        ! ================================================================

        if( orderOfAccuracy.eq.2 )then
         beginLoop(n1a,n1b,n2a,n2b,n3a,n3b)
	  if( mask(i1,i2,i3) .gt. 0 ) then
	    a(i1,i2,i3)=coeff(mdm1,i1,i2,i3)
	    b(i1,i2,i3)=coeff(md  ,i1,i2,i3)
	    c(i1,i2,i3)=coeff(mdp1,i1,i2,i3)
  	  else 
	    a(i1,i2,i3)=0.
	    b(i1,i2,i3)=1.
	    c(i1,i2,i3)=0.
	  end if
         endLoop()

        else if( orderOfAccuracy.eq.4 )then

         beginLoop(n1a,n1b,n2a,n2b,n3a,n3b)
	  if( mask(i1,i2,i3) .gt. 0 ) then
	    a(i1,i2,i3)=coeff(mdm2,i1,i2,i3)
	    b(i1,i2,i3)=coeff(mdm1,i1,i2,i3)
	    c(i1,i2,i3)=coeff(md  ,i1,i2,i3)
	    d(i1,i2,i3)=coeff(mdp1,i1,i2,i3)
	    e(i1,i2,i3)=coeff(mdp2,i1,i2,i3)
  	  else 
	    a(i1,i2,i3)=0.
	    b(i1,i2,i3)=0.
	    c(i1,i2,i3)=1.
	    d(i1,i2,i3)=0.
	    e(i1,i2,i3)=0.
	  end if
         endLoop()

        else
          write(*,*) 'lineSmoothBuild: invalid orderOfAccuracy=',orderOfAccuracy
          stop 6
        end if

        ! fix up boundary conditions 
        axis=direction
        is1=0
        is2=0
        is3=0
        do side=0,1
          if( axis.eq.0 )then
            is1=1-2*side
          else if( axis.eq.1 )then
            is2=1-2*side
          else
            is3=1-2*side
          end if

          if( bc(side,axis).eq.parallelGhostBoundary )then

            ! parallel ghost boundaries get a dirichlet condition
            fillMatrixParallelGhost()

          else if( bc(side,direction).gt.0 .and. bc(side,direction).eq.extrapolation )then 

            ! ******************************************************************
            ! **********************DIRICHLET VAR*******************************
            ! ******************************************************************

            ! Add extrapolation BC's on "dirichlet" BC's
            if( orderOfAccuracy.eq.4 )then

              getBoundaryIndex(side,axis,l1a,l1b,l2a,l2b,l3a,l3b,0)  ! first ghost line
      ! write(*,'(''+++++lineSmoothOpt: bcOptionD,l1='',i2,2x,6i3)') bcOptionD,l1a,l1b,l2a,l2b,l3a,l3b

              if( side.eq.0 )then
               ! 1st ghost line on left:
               !       [  c  d  e  a  b ]
               !      i= -1 -0  1  2  3
                if( bcOptionD.eq.0 )then
                 ! extrapolate
                  extrapFirstGhost(left,l1a,l1b,l2a,l2b,l3a,l3b)
                else if( bcOptionD.eq.1 )then
                  ! use eqn to 2nd order on the boundary
                  ! **** we could store the eqn coeff's in coeff
                  !  
                  !  20 21 22 23 24
                  !  15 16 17 18 19
                  !  10 11 12 13 14
                  !   5  6  7  8  9
                  !   0  1  2  3  4
                  !
                  ! The 2nd-order stencil is shifted to the right or left in the 4th order stencil
                  beginLoop(l1a,l1b,l2a,l2b,l3a,l3b)
                   j1=i1+is1 ! (j1,j2,j3) is the boundary point
                   j2=i2+is2
                   j3=i3+is3
  	           if( mask(j1,j2,j3).gt.0 ) then
	            c(i1,i2,i3)=coeff(mdm1+ms2,i1,i2,i3)  
	            d(i1,i2,i3)=coeff(md  +ms2,i1,i2,i3)
	            e(i1,i2,i3)=coeff(mdp1+ms2,i1,i2,i3)
	            a(i1,i2,i3)=0.
	            b(i1,i2,i3)=0. 
                    ! write(*,'('' lineSmooth:BC:EQN2:i1,i2,mdm1,md,mdp1,c,d,e='',5i3,3e10.2)') i1,i2,mdm1,md,mdp1,c(i1,i2,i3),d(i1,i2,i3),e(i1,i2,i3)
                   else if( mask(j1,j2,j3).lt.0 ) then
                     fillExtrapolation4(i1,i2,i3,c,d,e,a,b)
                   end if
                  endLoop()
                else if( bcOptionD.eq.2 )then
                  oddSymmetryFirstGhost(left,l1a,l1b,l2a,l2b,l3a,l3b)
                else
                  stop 6
                end if

              else  ! side==1 : 

                ! 1st ghost line on right:
                !       [  d  e  a  b  c ]
                !   i=n+[ -3 -2 -1  0  1 ]
                if( bcOptionD.eq.0 )then
                 ! extrapolate
                  extrapFirstGhost(right,l1a,l1b,l2a,l2b,l3a,l3b)
                else if( bcOptionD.eq.1 )then
                  ! use eqn to 2nd order on the boundary
                  ! **** we could store the eqn coeff's in coeff
                  beginLoop(l1a,l1b,l2a,l2b,l3a,l3b)
                   j1=i1+is1 ! (j1,j2,j3) is the boundary point
                   j2=i2+is2
                   j3=i3+is3
  	           if( mask(j1,j2,j3).gt.0 ) then
	            d(i1,i2,i3)= 0.
	            e(i1,i2,i3)= 0.
	            a(i1,i2,i3)=coeff(mdm1-ms2,i1,i2,i3)  
	            b(i1,i2,i3)=coeff(md  -ms2,i1,i2,i3)
	            c(i1,i2,i3)=coeff(mdp1-ms2,i1,i2,i3)
                    ! write(*,'('' lineSmooth:BC:EQN2:i1,i2,mdm1,md,mdp1,a,b,c='',5i3,3e10.2)') i1,i2,mdm1,md,mdp1,a(i1,i2,i3),b(i1,i2,i3),c(i1,i2,i3)
                    ! write(*,'(" coeff on ghost=",(20f5.2)') (coeff(mm,i1,i2,i3),mm=0,ndc-1)

                   else if( mask(j1,j2,j3).lt.0 ) then
                     fillExtrapolation4(i1,i2,i3,c,b,a,e,d)
                   end if
                  endLoop()
                 else if( bcOptionD.eq.2 )then
                   oddSymmetryFirstGhost(right,l1a,l1b,l2a,l2b,l3a,l3b)
                 else
                   stop 6
                end if
              end if

            end if

          else if( bc(side,direction).gt.0 .and. bc(side,direction).ne.extrapolation )then 

            ! ***********************************************************************
            ! *****************  Neumann or mixed BC  *******************************
            ! ***********************************************************************
  
            !    a0*u + a1*u.n = f
       	    a0=bcData(0,side,axis)
  	    a1=bcData(1,side,axis)

            if( orderOfAccuracy.eq.2 )then

             ! ------------------------------------------------------------
             ! ----------- 2nd order accuracy Neumann or mixed BC ---------
             ! ------------------------------------------------------------

             if( bcOptionN.eq.2 )then
               a0=0.
               a1=1.
             end if
             getBoundaryIndex(side,direction,m1a,m1b,m2a,m2b,m3a,m3b,0)  ! first ghost line
             if( side.eq.0 )then
               !       [  b  c  a  ]
               !         -1  0  1  
              ! *wdh* 110220: bcOptionN==3 should use true mixed-BC
              ! *wdh* if( bcOptionN.eq.2 .or. bcOptionN.eq.3 )then
              if( bcOptionN.eq.2 )then
               ! symmetry or mixed-symmetry condition (used on lower levels)
               ! write(*,'("lineSmoothBuild: Use mixed-symmetry BC for Neumann, level=",i3," a0,a1=",2f5.2)') level,a0,a1
               if( a0.eq.0. .and. a1.eq.0. )then
                 write(*,'("lineSmoothBuild:ERROR: mixed-symmetry BC for Neumann: a0=0 and a1=0!")')
                 stop 9021
               end if
                 !       [  b  c  a  ]
                 !         -1  0  1  
                dxi=1.
                beginLoop(m1a,m1b,m2a,m2b,m3a,m3b)
                  j1=i1+is1 ! (j1,j2,j3) is the boundary point
                  j2=i2+is2
                  j3=i3+is3
  	          if( mask(j1,j2,j3).gt.0 ) then
  		    b(i1,i2,i3)= a1*dxi*.5
  		    c(i1,i2,i3)= a0
  		    a(i1,i2,i3)=-a1*dxi*.5
                  else if( mask(j1,j2,j3).lt.0 )then
                    fillExtrapolation2(i1,i2,i3,b,c,a)
                  end if
                endLoop()

              else if( bcOptionN.eq.3 )then
                ! Use true mixed or Neumann BC

                ! write(*,'("lineSmoothBuild: Use mixed/Neumann BC left level=",i3," a0,a1=",2f5.2," dx=",e8.2)') level,a0,a1,dx(direction)

                if( gridType.eq.rectangular )then
                 dxi = 1./dx(direction)
                 beginLoop(m1a,m1b,m2a,m2b,m3a,m3b)
                  j1=i1+is1 ! (j1,j2,j3) is the boundary point
                  j2=i2+is2
                  j3=i3+is3
  	          if( mask(j1,j2,j3).gt.0 ) then
  		    b(i1,i2,i3)= a1*dxi*.5
  		    c(i1,i2,i3)= a0
  		    a(i1,i2,i3)=-a1*dxi*.5

                    ! write(*,'("lineSmoothBuild: i1,i2=",2i3," [b c a]=",3e10.2)') i1,i2,b(i1,i2,i3),c(i1,i2,i3),a(i1,i2,i3)
               
                  else if( mask(j1,j2,j3).lt.0 )then
                    fillExtrapolation2(i1,i2,i3,b,c,a)
                  end if
                 endLoop()
                else
                 ! curvilinear-grid  mixed BC 
                 fillMixedToSecondOrder(b,c,a)

                end if

              else
               ! use equation in coeff
               ! write(*,'("lineSmoothBuild: Mixed/neumann BC: use coeff, level=",i3," a0,a1=",2f5.2)') level,a0,a1

               beginLoop(m1a,m1b,m2a,m2b,m3a,m3b)
                j1=i1+is1 ! (j1,j2,j3) is the boundary point
                j2=i2+is2
                j3=i3+is3
     	       if( mask(j1,j2,j3).gt.0 ) then
   	        b(i1,i2,i3)=coeff(mdm1,i1,i2,i3)  ! note: c
   	        c(i1,i2,i3)=coeff(md  ,i1,i2,i3)
   	        a(i1,i2,i3)=coeff(mdp1,i1,i2,i3)

                ! write(*,'("lineSmoothBuild: i1,i2=",2i3," [b c a]=",3e10.2)') i1,i2,b(i1,i2,i3),c(i1,i2,i3),a(i1,i2,i3)

                else if( mask(j1,j2,j3).lt.0 ) then
                 fillExtrapolation2(i1,i2,i3,b,c,a)
                end if
               endLoop()
              end if

             else
               !       [  c  a  b  ]
               !         -1  0  1  
              ! *wdh* 110220: bcOptionN==3 should use true mixed-BC
              ! *wdh* 110220 if( bcOptionN.eq.2 .or. bcOptionN.eq.3 )then
              if( bcOptionN.eq.2 )then
               ! mixed-symmetry condition (used on lower levels)
               ! write(*,'("lineSmoothBuild: Use mixed-symmetry BC for Neumann, level=",i3," a0,a1=",2f5.2)') level,a0,a1
               if( a0.eq.0. .and. a1.eq.0. )then
                 write(*,'("lineSmoothBuild:ERROR: mixed-symmetry BC for Neumann: a0=0 and a1=0!")')
                 stop 9022
               end if
                 !       [  c  a  b  ]
                 !         -1  0  1  
                dxi=1.
                beginLoop(m1a,m1b,m2a,m2b,m3a,m3b)
                  j1=i1+is1 ! (j1,j2,j3) is the boundary point
                  j2=i2+is2
                  j3=i3+is3
  	          if( mask(j1,j2,j3).gt.0 ) then
  		   c(i1,i2,i3)=-a1*dxi*.5
  		   a(i1,i2,i3)= a0
  		   b(i1,i2,i3)= a1*dxi*.5
                  else if( mask(j1,j2,j3).lt.0 )then
                    fillExtrapolation2(i1,i2,i3,c,a,b)
                  end if
                endLoop()

              else if( bcOptionN.eq.3 )then
                ! -- Use true mixed or Neumann BC --

                ! write(*,'("lineSmoothBuild: Use mixed/Neumann BC right level=",i3," a0,a1=",2f5.2)') level,a0,a1

                if( gridType.eq.rectangular )then
                 dxi = 1./dx(direction)
                 beginLoop(m1a,m1b,m2a,m2b,m3a,m3b)
                  j1=i1+is1 ! (j1,j2,j3) is the boundary point
                  j2=i2+is2
                  j3=i3+is3
  	          if( mask(j1,j2,j3).gt.0 ) then
  		   c(i1,i2,i3)=-a1*dxi*.5
  		   a(i1,i2,i3)= a0
  		   b(i1,i2,i3)= a1*dxi*.5
                  else if( mask(j1,j2,j3).lt.0 )then
                    fillExtrapolation2(i1,i2,i3,c,a,b)
                  end if
                 endLoop()
                else

                 ! curvilinear-grid  mixed BC 
                 fillMixedToSecondOrder(c,a,b)

                end if

              else
               beginLoop(m1a,m1b,m2a,m2b,m3a,m3b)
                j1=i1+is1 ! (j1,j2,j3) is the boundary point
                j2=i2+is2
                j3=i3+is3
     	       if( mask(j1,j2,j3).gt.0 ) then
   	        c(i1,i2,i3)=coeff(mdm1,i1,i2,i3)  ! note: c
   	        a(i1,i2,i3)=coeff(md  ,i1,i2,i3)
   	        b(i1,i2,i3)=coeff(mdp1,i1,i2,i3)
                else if( mask(j1,j2,j3).lt.0 ) then
                 fillExtrapolation2(i1,i2,i3,c,a,b)
                end if
               endLoop()
              end if
             end if

            else if( orderOfAccuracy.eq.4 )then

              ! -----------------------------------------------
              ! ---------- Order 4 Neumann/Mixed --------------
              ! -----------------------------------------------

              !    a0*u + a1*u.n = f
       	      a0=bcData(0,side,axis)
  	      a1=bcData(1,side,axis)
              if( a0.eq.0. .and. a1.eq.0. )then
                write(*,*) 'lineSmoothBuild:ERROR: a0 and a1 are both zero'
                write(*,*) 'side,axis,a0,a1,dxi=',side,axis,a0,a1,dxi
                stop 5
              end if	      

             getBoundaryIndex(side,axis,l1a,l1b,l2a,l2b,l3a,l3b,0)  ! 2nd ghost line
             getBoundaryIndex(side,axis,m1a,m1b,m2a,m2b,m3a,m3b,1)  ! first ghost line

             ! write(*,*) "lineSmoothBuild: order=4 bcOptionN=",bcOptionN

             if( side.eq.0 )then
               ! 2nd ghost line on left:
               !       [  c  d  e  a  b ]
               !    i= [ -2 -1  0  1  2 ]
              if( bcOptionN.eq.0 )then
               ! extrapolate 2nd ghost
               beginLoop(l1a,l1b,l2a,l2b,l3a,l3b)
                fillExtrapolationPentaDiagonal(i1,i2,i3,c,d,e,a,b,orderOfExtrapN)
	        !  c(i1,i2,i3)= 1.
	        !  d(i1,i2,i3)=-4.
	        !  e(i1,i2,i3)= 6.
	        !  a(i1,i2,i3)=-4.
	        !  b(i1,i2,i3)= 1.
               endLoop()
              else if( bcOptionN.eq.1 )then
               ! u.rrr = 

               if( gridType.eq.rectangular )then ! *wdh* 100506
                neumannAndEquationRectangular(c,d,e,a,b)
               else ! curvilinear case
                if( axis.eq.0 .and. nd.eq.2 )then
                  neumannAndEquationCurvilinear(R,2,c,d,e,a,b)
                else if( axis.eq.1 .and. nd.eq.2 )then
                  neumannAndEquationCurvilinear(S,2,c,d,e,a,b)

                else if( axis.eq.0 .and. nd.eq.3 )then
#perl $DIM=3; $ORDER=4; $GRIDTYPE="curvilinear";
                  neumannAndEquationCurvilinear3d(R,c,d,e,a,b)
                else if( axis.eq.1 .and. nd.eq.3 )then
                  neumannAndEquationCurvilinear3d(S,c,d,e,a,b)
                else if( axis.eq.2 .and. nd.eq.3 )then
                  neumannAndEquationCurvilinear3d(T,c,d,e,a,b)
                else
                  stop 88
                end if
               end if

              else if( bcOptionN.eq.2 )then
                 evenSymmetrySecondGhost(left,l1a,l1b,l2a,l2b,l3a,l3b)
              else if( bcOptionN.eq.3 )then
                ! mixed BC for both first and second ghost lines

                ! *new* 110308 -- apply real 2nd-order approximations on two ghost 
                ! write(*,'(">>>lineSmoothBuild: 2nd order Neumann/mixed on TWO lines")')

                mixedToSecondOrderPentaDiagonalTwoLines(curvilinear,left)
              else
                write(*,*) 'lineSmoothBuild:ERROR: unknown bcOptionN'
                stop 14
              end if

              ! 1st ghost line on left:
              !       [  b  c  d  e  a ]
              !    i= [ -2 -1  0  1  2 ]
               if( bcOptionN.eq.0 .and. isNeumannBC(0).eq.1 .and. level.gt.0 )then
                ! this is really a Neumann or Mixed BC *wdh* 110224
                ! On lower levels we fill in the BC to 2nd order

                 ! write(*,'(" lineSmoothOpt: fill mixed BC left 2nd order for penta")') 

                 if( gridType.eq.rectangular )then 
                  fillMixedToSecondOrderPentaDiagonal(rectangular,left,b,c,d,e,a)
                 else
                  fillMixedToSecondOrderPentaDiagonal(curvilinear,left,b,c,d,e,a)
                 end if
               else if( bcOptionN.eq.0 .or. bcOptionN.eq.1 )then

                 ! BC is stored in the coeff matrix
                 beginLoop(m1a,m1b,m2a,m2b,m3a,m3b)
                  j1=i1+is1 ! (j1,j2,j3) is the boundary point
                  j2=i2+is2
                  j3=i3+is3
      	          if( mask(j1,j2,j3).gt.0 ) then
                    b(i1,i2,i3)=coeff(mdm2,i1,i2,i3)
                    c(i1,i2,i3)=coeff(mdm1,i1,i2,i3)
                    d(i1,i2,i3)=coeff(md  ,i1,i2,i3)
                    e(i1,i2,i3)=coeff(mdp1,i1,i2,i3)
                    a(i1,i2,i3)=coeff(mdp2,i1,i2,i3)

       ! write(*,'(" LS:neumann : myid=",i3,"i1,i2=",i3,i3," a,b,c,d,e=",5e11.3)') myid,i1,i2,a(i1,i2,i3),b(i1,i2,i3),c(i1,i2,i3),d(i1,i2,i3),e(i1,i2,i3)

                  else if( mask(j1,j2,j3).lt.0 ) then
                    fillExtrapolation4(i1,i2,i3,b,c,d,e,a)
                  end if
                 endLoop()

              else if( bcOptionN.eq.2 )then
                evenSymmetryFirstGhost(left,m1a,m1b,m2a,m2b,m3a,m3b)
              else if( bcOptionN.eq.3 )then
                ! mixed BC already done
              else
                stop 17
              end if

             else  ! side==1


              ! 2nd ghost line on right:
              !       [  d  e  a  b  c ]
              !  i=n+ [ -2 -1  0  1  2 ]
              if( bcOptionN.eq.0 )then
               ! extrapolate 2nd ghost
               beginLoop(l1a,l1b,l2a,l2b,l3a,l3b)
                fillExtrapolationPentaDiagonal(i1,i2,i3,c,b,a,e,d,orderOfExtrapN)
	        ! d(i1,i2,i3)= 1.
	        ! e(i1,i2,i3)=-4.
	        ! a(i1,i2,i3)= 6.
	        ! b(i1,i2,i3)=-4.
	        ! c(i1,i2,i3)= 1.
               endLoop()
              else if( bcOptionN.eq.1 )then
               ! u.rrr = 
               if( gridType.eq.rectangular )then ! *wdh* 100506
                 neumannAndEquationRectangular(d,e,a,b,c)
               else  ! curvilinear case
                if( axis.eq.0 .and. nd.eq.2 )then
                  neumannAndEquationCurvilinear(R,2,d,e,a,b,c)
                else if( axis.eq.1 .and. nd.eq.2 )then
                  neumannAndEquationCurvilinear(S,2,d,e,a,b,c)

                else if( axis.eq.0 .and. nd.eq.3 )then
                  neumannAndEquationCurvilinear3d(R,d,e,a,b,c)
                else if( axis.eq.1 .and. nd.eq.3 )then
                  neumannAndEquationCurvilinear3d(S,d,e,a,b,c)
                else if( axis.eq.2 .and. nd.eq.3 )then
                  neumannAndEquationCurvilinear3d(T,d,e,a,b,c)
                else
                  stop 88
                end if
               end if

              else if( bcOptionN.eq.2 )then
                evenSymmetrySecondGhost(right,l1a,l1b,l2a,l2b,l3a,l3b)
              else if( bcOptionN.eq.3 )then
                ! mixed BC for both first and second ghost lines

                ! *new* 110308 -- apply real 2nd-order approximations on two ghost 
                ! write(*,'(">>>lineSmoothBuild: 2nd order Neumann/mixed on TWO lines")')

                mixedToSecondOrderPentaDiagonalTwoLines(curvilinear,right)
              else
                stop 14
              end if
              ! 1st ghost line on right:
              !       [  e  a  b  c  d ]
              !  i=n+ [ -2 -1  0  1  2 ]
              if( bcOptionN.eq.0 .and. isNeumannBC(1).eq.1 .and. level.gt.0 )then
                ! this is really a Neumann or Mixed BC *wdh* 110224
                ! On lower levels we fill in the BC to 2nd order

                 ! write(*,'("$$$ lineSmoothOpt: fill mixed BC right 2nd order for penta")') 

                 if( gridType.eq.rectangular )then 
                  fillMixedToSecondOrderPentaDiagonal(rectangular,right,e,a,b,c,d)
                 else
                  fillMixedToSecondOrderPentaDiagonal(curvilinear,right,e,a,b,c,d)
                 end if

              else if( bcOptionN.eq.0 .or. bcOptionN.eq.1 )then

                ! BC is store in the coeff matrix 

                beginLoop(m1a,m1b,m2a,m2b,m3a,m3b)
                 j1=i1+is1 ! (j1,j2,j3) is the boundary point
                 j2=i2+is2
                 j3=i3+is3
    	         if( mask(j1,j2,j3).gt.0 ) then
                  e(i1,i2,i3)=coeff(mdm2,i1,i2,i3)
                  a(i1,i2,i3)=coeff(mdm1,i1,i2,i3)
                  b(i1,i2,i3)=coeff(md  ,i1,i2,i3)
                  c(i1,i2,i3)=coeff(mdp1,i1,i2,i3)
                  d(i1,i2,i3)=coeff(mdp2,i1,i2,i3)
                 else if( mask(j1,j2,j3).lt.0 ) then
                   fillExtrapolation4(i1,i2,i3,d,c,b,a,e)
                 end if
                endLoop()

              else if( bcOptionN.eq.2 )then
                evenSymmetryFirstGhost(right,m1a,m1b,m2a,m2b,m3a,m3b)
              else if( bcOptionN.eq.3 )then
                ! mixed BC already done
              else
                stop 91
              end if

             end if
            else
              stop 6
            end if
  
          end if
        end do ! do side

      end if

      return
      end      

! Define the fourth-order accurate neumann BC which uses the normal derivative of the equation on the
! boundary. 
#beginMacro neumannAndEquationOld(FORCING,DIR,DIM)

 a0=bcData(0,side,axis)
 a1=bcData(1,side,axis)
 if( a1.eq.0. )then
   write(*,*) 'lineSmoothRHS:ERROR: a1=0!'
   stop 2
 end if
   
 if( gridType.eq.rectangular )then

! write(*,*) 'LSRHSt:4th-order neumann+EQN2 (rect)'
!   write(*,'(''LSRHS:4th neumannAndEqn (rect) nn2a,nn2b='',2i3,)') nn2a,nn2b

   drn=dx(axis)
   nsign = 2*side-1
   cf1= nsign*(drn**3)/3. !  030525: add nsign to cf1,cg1,cf2,cg2
   cg1= 2.*nsign*drn

   cf2=nsign*(drn**3)*8/3.
   cg2=nsign*4.*drn

   ! (i1,i2,i3) = boundary point
   ! (j1,j2,j3) = ghost point
   do i3=m3a,m3b,m3c
    j3=i3-is3
   do i2=m2a,m2b,m2c
    j2=i2-is2
   do i1=m1a,m1b,m1c
    j1=i1-is1
    if( mask(i1,i2,i3).gt.0 )then

    ! the rhs for the mixed BC is stored in the ghost point value of f
    defineNeumannEquationForcing(nn1a,nn1b,nn2a,nn2b,nn3a,nn3b,FORCING,rectangular,DIR,DIM)

    #If #DIR eq "R"
!     uss=uyy22r(i1,i2,i3)
      uss=(u(i1,i2+1,i3)+u(i1,i2-1,i3))/dx(1)**2 ! leave off diagonal term which goes into the matrix
    #Elif #DIR eq "S"
!      urr=uxx22r(i1,i2,i3)  ! need to 2nd-order
      urr=(u(i1+1,i2,i3)+u(i1-1,i2,i3))/dx(0)**2 ! leave off diagonal term which goes into the matrix
    #Else
      stop 7
    #End

!      ur=( g - a0*u(i1,i2,i3) )/(a1*nsign)
!      urrr= ffr - (gss - a0*uss )/(a1*nsign) ! 030525
   
!   write(*,'(''LNSM : i1,i2,i3,g,ffr,gss,u,uss ='',3i3,5f11.6)') i1,i2,i3,g,ffr,gss,u(i1,i2,i3),(u(i1,i2+1,i3)-2.*u(i1,i2,i3)+u(i1,i2-1,i3))/dx(1)**2
!  write(*,'('' i1,i2,i3,f,f,f='',3i3,4e11.2)') i1,i2,i3,f(i1,i2,i3),f(i1+2*is1,i2+2*is2,i3),f(i1+is1,i2+is2,i3)

     r(j1,j2,j3)=g 
    #If #DIR eq "R"
      r(i1-2*is1,i2,i3)=(ffr-(gss- a0*uss )/(a1*nsign))*dx(axis)**3  
    #Else
      r(i1,i2-2*is2,i3)=(ffs-(grr- a0*urr )/(a1*nsign))*dx(axis)**3  
    #End

    else
      r(j1,j2,j3)=0.
      r(i1-2*is1,i2-2*is2,i3-2*is3)=0.
    end if

  end do
  end do
  end do



 else
   ! **** curvilinear case ****

   if( axis.gt.1 )then
     write(*,*) 'lineSmoothRHS:ERROR: this option not implemented yet'
     write(*,*) 'axis=',axis
     stop 12
   end if

   ! write(*,*) 'lineSmoothRHS:4th-order neumann and L.n - DIR)'

   nsign = 2*side-1

   ! (i1,i2,i3) = boundary point
   ! (j1,j2,j3) = ghost point
   do i3=m3a,m3b,m3c
    j3=i3-is3
   do i2=m2a,m2b,m2c
    j2=i2-is2
   do i1=m1a,m1b,m1c
    j1=i1-is1
    if( mask(i1,i2,i3).gt.0 )then

    ! the rhs for the mixed BC is stored in the ghost point value of f
    defineNeumannEquationForcing(nn1a,nn1b,nn2a,nn2b,nn3a,nn3b,FORCING,curvilinear,DIR,DIM)

    fourthOrderNeumannEquationBC(DIR,DIM)

    #If #DIR eq "R"
      us=us4(i1,i2,i3)
!     uss=uss2(i1,i2,i3) 
      uss=(u(i1,i2+1,i3)+u(i1,i2-1,i3))/dr(1)**2 ! leave off diagonal term which goes into the matrix
      usss=usss2(i1,i2,i3)
    #Elif #DIR eq "S"
      ur=ur4(i1,i2,i3)
!      urr=urr2(i1,i2,i3)  ! need to 2nd-order
      urr=(u(i1+1,i2,i3)+u(i1-1,i2,i3))/dr(0)**2 ! leave off diagonal term which goes into the matrix
      urrr=urrr2(i1,i2,i3)
    #Else
      stop 7
    #End

!    write(*,'(''LSmOpt: i1,i2,i3,g,gs,gss,ff,ffr,ffs,uss ='',3i3,7e9.2)') i1,i2,i3,g,gs,gss,ff,ffr,ffs,uss2(i1,i2,i3)
!    write(*,'(''LSmOpt: an1,an2,c11,b0,b1,b2,b3,bf,b0+bf ='',9e10.2)') an1,an2,c11,b0,b1,b2,b3,bf,b0+bf

    #If #DIR eq "R"

      r(i1-is1,i2-is2,i3-is3)=g - an2*us
      r(i1-2*is1,i2-2*is2,i3-2*is3)=b1*us+b2*uss+b3*usss+bf 
    #Else

      ! write(*,'(''i='',i3,i3,'' us='',f6.2,'', approx='',f6.2)') i1,i2,us,us2(i1,i2+is2,i3)
      ! write(*,'(''i='',i3,i3,'' urs='',f6.2,'', approx='',f6.2)') i1,i2,urs,urs2(i1,i2+is2,i3)
      ! write(*,'(''i='',i3,i3,'' uss='',f6.2,'', approx='',f6.2)') i1,i2,uss,uss2(i1,i2+is2,i3)
      ! write(*,'(''i='',i3,i3,'' usss='',f6.2,'', approx='',f6.2)') i1,i2,usss,usss2(i1,i2+is2,i3)

      r(i1-is1,i2-is2,i3-is3)=g- an1*ur
      r(i1-2*is1,i2-2*is2,i3-2*is3)=b1*ur+b2*urr+b3*urrr+bf 
    #End
   
    end if
  end do
  end do
  end do

 end if
#endMacro



! ---------------------------------------------------------------------------------------------------------
! Define the fourth-order accurate neumann BC which uses the normal derivative of the equation on the
! boundary. 
!
! Cartesian Grid: 
!    u.xx + u.yy + u.zz = f
!    a1n*u.x + a0*u = g     -> u.x = (g-a0*u)/a1n 
!
!    u.xxx = f.x - ( u.xyy + u.xzz )
!          = f.x - ( g.yy -a0*u.yy + g.zz - a0*u.zz )/a1n 
! 
! Here is the numerical boundary condition:
!    u.xxx + (a0/a1n)*u.xx = f.x - ( g.yy+g.zz -a0*f )/a1n 
! 
! Curvilinear grid:
!   See ogmg/doc/neumann.maple
! --------------------------------------------------------------------------------------------------------
#beginMacro neumannAndEquation(FORCING,DIR,DIM)

 a0=bcData(0,side,axis)
 a1=bcData(1,side,axis)
 if( a1.eq.0. )then
   write(*,*) 'lineSmoothRHS:ERROR: a1=0!'
   stop 2
 end if
   
 if( gridType.eq.rectangular )then

! write(*,*) 'LSRHSt:4th-order neumann+EQN2 (rect)'
!   write(*,'(''LSRHS:4th neumannAndEqn (rect) nn2a,nn2b='',2i3,)') nn2a,nn2b

   drn=dx(axis)
   nsign = 2*side-1

   ! (i1,i2,i3) = boundary point
   ! (j1,j2,j3) = ghost point
   do i3=m3a,m3b,m3c
    j3=i3-is3
   do i2=m2a,m2b,m2c
    j2=i2-is2
   do i1=m1a,m1b,m1c
    j1=i1-is1
    if( mask(i1,i2,i3).gt.0 )then


    ! the rhs for the mixed BC is stored in the ghost point value of f
    defineNeumannEquationForcing(nn1a,nn1b,nn2a,nn2b,nn3a,nn3b,FORCING,rectangular,DIR,DIM)

!   write(*,'(''LNSM : i1,i2,i3,g,ffr,gss,u,uss ='',3i3,5f11.6)') i1,i2,i3,g,ffr,gss,u(i1,i2,i3),(u(i1,i2+1,i3)-2.*u(i1,i2,i3)+u(i1,i2-1,i3))/dx(1)**2
!  write(*,'('' i1,i2,i3,f,f,f='',3i3,4e11.2)') i1,i2,i3,f(i1,i2,i3),f(i1+2*is1,i2+2*is2,i3),f(i1+is1,i2+is2,i3)

    r(j1,j2,j3)=g 
    #If #DIR eq "R"
     #If #DIM eq "2"
      r(i1-2*is1,i2,i3)=(ffr-(gss- a0*ff )/(a1*nsign))*dx(axis)**3  
     #Else
      r(i1-2*is1,i2,i3)=(ffr-(gss+gtt- a0*ff )/(a1*nsign))*dx(axis)**3  
     #End
    #Elif #DIR eq "S"
     #If #DIM eq "2"
      r(i1,i2-2*is2,i3)=(ffs-(grr- a0*ff )/(a1*nsign))*dx(axis)**3  
     #Else
      r(i1,i2-2*is2,i3)=(ffs-(grr+gtt- a0*ff )/(a1*nsign))*dx(axis)**3  
     #End
    #Elif #DIR eq "T"
      r(i1,i2,i3-2*is3)=(fft-(grr+gss- a0*ff )/(a1*nsign))*dx(axis)**3 
    #Else
       stop 4502
    #End

    else
      r(j1,j2,j3)=0.
      r(i1-2*is1,i2-2*is2,i3-2*is3)=0.
    end if

  end do
  end do
  end do



 else
   ! **** curvilinear case ****


   nsign = 2*side-1

   ! (i1,i2,i3) = boundary point
   ! (j1,j2,j3) = ghost point
   do i3=m3a,m3b,m3c
    j3=i3-is3
   do i2=m2a,m2b,m2c
    j2=i2-is2
   do i1=m1a,m1b,m1c
      j1=i1-is1
     if( mask(i1,i2,i3).gt.0 )then

    ! the rhs for the mixed BC is stored in the ghost point value of f
    defineNeumannEquationForcing(nn1a,nn1b,nn2a,nn2b,nn3a,nn3b,FORCING,curvilinear,DIR,DIM)

    #If #DIM eq 2 
      ! ----- 2D ------
     fourthOrderNeumannEquationBCNew(DIR,DIM)

     #If #DIR eq "R"
      us=us4(i1,i2,i3)
      usss=usss2(i1,i2,i3)
     #Elif #DIR eq "S"
      ur=ur4(i1,i2,i3)
      urrr=urrr2(i1,i2,i3)
     #Else
      stop 7
     #End

!    write(*,'(''LSmOpt: i1,i2,i3,g,gs,gss,ff,ffr,ffs,uss ='',3i3,7e9.2)') i1,i2,i3,g,gs,gss,ff,ffr,ffs,uss2(i1,i2,i3)
!    write(*,'(''LSmOpt: an1,an2,c11,b0,b1,b2,b3,bf,b0+bf ='',9e10.2)') an1,an2,c11,b0,b1,b2,b3,bf,b0+bf

     #If #DIR eq "R"

      r(i1-is1,i2-is2,i3-is3)=g - an2*us
      r(i1-2*is1,i2-2*is2,i3-2*is3)=b1*us+b3*usss+bf 

     #Else

      ! write(*,'(''i='',i3,i3,'' us='',f6.2,'', approx='',f6.2)') i1,i2,us,us2(i1,i2+is2,i3)
      ! write(*,'(''i='',i3,i3,'' urs='',f6.2,'', approx='',f6.2)') i1,i2,urs,urs2(i1,i2+is2,i3)
      ! write(*,'(''i='',i3,i3,'' uss='',f6.2,'', approx='',f6.2)') i1,i2,uss,uss2(i1,i2+is2,i3)
      ! write(*,'(''i='',i3,i3,'' usss='',f6.2,'', approx='',f6.2)') i1,i2,usss,usss2(i1,i2+is2,i3)

      r(i1-is1,i2-is2,i3-is3)=g- an1*ur
      r(i1-2*is1,i2-2*is2,i3-2*is3)=b1*ur+b3*urrr+bf 
     #End

   #Elif #DIM eq "3" 
     ! ---- 3D ----

    ! We need 2 parameteric and 1 real derivative. Do this for now: 
    opEvalJacobianDerivatives(aj,2)

    neumannAndEquationLineBC3dOrder4(DIR,RHS)


     #If #DIR eq "R"

      us=us4(i1,i2,i3)
      uss=uss4(i1,i2,i3)
      usss=usss2(i1,i2,i3)

      ut=ut4(i1,i2,i3)
      utt=utt4(i1,i2,i3)
      uttt=uttt2(i1,i2,i3)

      ust=ust4(i1,i2,i3)

      usst=usst2(i1,i2,i3)
      ustt=ustt2(i1,i2,i3)

! bf  = -(-cRT**2*anRtt*anR*g-2*cRS**2*anRs*gs*anR-cRS**2*anRss*anR*g+cRS*cRTs*anR**2*gt+cRS*ccRs*anR**2*g+cRS*ccR*anR**2*gs+cRS*cRSs*anR**2*gs+2*cRS*cRT*gst*anR**2-cSS*cRR*gss*anR**2-2*cSS*cRR*anRs**2*g+ffr*anR**3*cRR+cRS**2*gss*anR**2+2*cRS**2*anRs**2*g-cRS*ffs*anR**3-2*cRS*cRT*anRs*gt*anR+4*cRS*cRT*anRs*anRt*g-2*cRS*cRT*anRt*gs*anR-2*cRS*cRT*anRst*anR*g-cRS*ccR*anR*anRs*g-cRS*cRSs*anR*anRs*g-cRS*cRTs*anR*anRt*g+2*cSS*cRR*anRs*gs*anR-cRT*fft*anR**3+2*cRT**2*anRt**2*g+cRT**2*gtt*anR**2+cSS*cRR*anRss*anR*g-2*cRT**2*anRt*gt*anR+cRT*ccR*anR**2*gt+cRT*cRSt*anR**2*gs+cRT*cRTt*anR**2*gt+cRT*ccRt*anR**2*g-cST*cRR*gst*anR**2-2*cTT*cRR*anRt**2*g-cTT*cRR*gtt*anR**2-ccS*anR**2*cRR*gs-ccT*anR**2*cRR*gt-c0*anR**2*cRR*g-cRSr*anR**2*cRR*gs-cRTr*anR**2*cRR*gt-ccRr*anR**2*cRR*g-cRT*ccR*anR*anRt*g-cRT*cRSt*anR*anRs*g-cRT*cRTt*anR*anRt*g+cST*cRR*anRs*gt*anR-2*cST*cRR*anRs*anRt*g+cST*cRR*anRt*gs*anR+cST*cRR*anRst*anR*g+cTT*cRR*anRtt*anR*g+2*cTT*cRR*anRt*gt*anR+ccS*anR*cRR*anRs*g+ccT*anR*cRR*anRt*g+cRSr*anR*cRR*anRs*g+cRTr*anR*cRR*anRt*g)/anR**3/cRR**2

   ! write(*,'("LS-NE4:RHS: i1,i2=",2i3,",  -- Values:")') i1,i2
   ! write(*,'("  g,gs,gt,gss,gtt,ff,ffs,fft=",12e10.2)') g,gs,gt,gss,gtt,ff,ffs,fft
   ! write(*,'("  bf,us,ut,uss,utt,ust,usss,uttt,usst,ustt=",12e10.2)') bf,us,ut,uss,utt,ust,usss,uttt,usst,ustt
   ! write(*,'("  bs,bss,bsss, bt,btt,bttt, bst,bsst,bstt,bf=",10e10.2)') bs,bss,bsss, bt,btt,bttt, bst,bsst,bstt,bf


      r(i1-is1,i2-is2,i3-is3)=g - anS*us -anT*ut 
      r(i1-2*is1,i2-2*is2,i3-2*is3)=-( bs*us +bss*uss +bsss*usss +bt*ut +btt*utt + bttt*uttt + bst*ust + bsst*usst + bstt*ustt +bf )

   ! write(*,'("  r(-1),r(-2)=",10e10.2)') r(i1-is1,i2-is2,i3-is3),r(i1-2*is1,i2-2*is2,i3-2*is3)
      ! TEMP
      ! r(i1-is1,i2-is2,i3-is3)=0. 
      ! r(i1-2*is1,i2-2*is2,i3-2*is3)=0.

     #Elif #DIR eq "S"

      ur=ur4(i1,i2,i3)
      urr=urr4(i1,i2,i3)
      urrr=urrr2(i1,i2,i3)

      ut=ut4(i1,i2,i3)
      utt=utt4(i1,i2,i3)
      uttt=uttt2(i1,i2,i3)

      urt=urt4(i1,i2,i3)

      urrt=urrt2(i1,i2,i3)
      urtt=urtt2(i1,i2,i3)

      r(i1-is1,i2-is2,i3-is3)=g - anR*ur -anT*ut 
      r(i1-2*is1,i2-2*is2,i3-2*is3)=-( br*ur +brr*urr +brrr*urrr +bt*ut +btt*utt + bttt*uttt + brt*urt + brrt*urrt + brtt*urtt +bf )

     #Elif #DIR eq "T"

      ur=ur4(i1,i2,i3)
      urr=urr4(i1,i2,i3)
      urrr=urrr2(i1,i2,i3)

      us=us4(i1,i2,i3)
      uss=uss4(i1,i2,i3)
      usss=usss2(i1,i2,i3)

      urs=urs4(i1,i2,i3)

      urrs=urrs2(i1,i2,i3)
      urss=urss2(i1,i2,i3)

      r(i1-is1,i2-is2,i3-is3)=g - anR*ur -anS*us 
      r(i1-2*is1,i2-2*is2,i3-2*is3)=-( br*ur +brr*urr +brrr*urrr +bs*us +bss*uss + bsss*usss + brs*urs + brrs*urrs + brss*urss +bf )

     #Else
       stop 3035
     #End

   #End
   
    else
      r(j1,j2,j3)=0.
      r(i1-2*is1,i2-2*is2,i3-2*is3)=0.
    end if
  end do
  end do
  end do

 end if
#endMacro

! --- These macros are from similar ones in defectOpt ----
!  - for boundary conditions we may need to evaluate the coefficients at (j1,j2,j3) 
!    and the solution centered at (i1,i2,i3)
#defineMacro op2d(j1,j2,j3,i1,i2,i3) (\
     c(m11,j1,j2,j3)*u(i1-1,i2-1,i3)+ \
     c(m21,j1,j2,j3)*u(i1  ,i2-1,i3)+ \
     c(m31,j1,j2,j3)*u(i1+1,i2-1,i3)+ \
     c(m12,j1,j2,j3)*u(i1-1,i2  ,i3)+ \
     c(m22,j1,j2,j3)*u(i1  ,i2  ,i3)+ \
     c(m32,j1,j2,j3)*u(i1+1,i2  ,i3)+ \
     c(m13,j1,j2,j3)*u(i1-1,i2+1,i3)+ \
     c(m23,j1,j2,j3)*u(i1  ,i2+1,i3)+ \
     c(m33,j1,j2,j3)*u(i1+1,i2+1,i3) )
! line smooth direction 1 
#defineMacro op2dl0(j1,j2,j3,i1,i2,i3) (  \
     c(m11,j1,j2,j3)*u(i1-1,i2-1,i3)+ \
     c(m21,j1,j2,j3)*u(i1  ,i2-1,i3)+ \
     c(m31,j1,j2,j3)*u(i1+1,i2-1,i3)+ \
     c(m13,j1,j2,j3)*u(i1-1,i2+1,i3)+ \
     c(m23,j1,j2,j3)*u(i1  ,i2+1,i3)+ \
     c(m33,j1,j2,j3)*u(i1+1,i2+1,i3) )
#defineMacro op2dl1(j1,j2,j3,i1,i2,i3) ( \
     c(m11,j1,j2,j3)*u(i1-1,i2-1,i3)+ \
     c(m31,j1,j2,j3)*u(i1+1,i2-1,i3)+ \
     c(m12,j1,j2,j3)*u(i1-1,i2  ,i3)+ \
     c(m32,j1,j2,j3)*u(i1+1,i2  ,i3)+ \
     c(m13,j1,j2,j3)*u(i1-1,i2+1,i3)+ \
     c(m33,j1,j2,j3)*u(i1+1,i2+1,i3) )
#defineMacro op2dSparse(j1,j2,j3,i1,i2,i3) ( \
     c(m22,j1,j2,j3)*u(i1  ,i2  ,i3)+ \
     c(m32,j1,j2,j3)*u(i1+1,i2  ,i3)+ \
     c(m23,j1,j2,j3)*u(i1  ,i2+1,i3)+ \
     c(m12,j1,j2,j3)*u(i1-1,i2  ,i3)+ \
     c(m21,j1,j2,j3)*u(i1  ,i2-1,i3) )
#defineMacro op2dSparsel0(j1,j2,j3,i1,i2,i3) ( \
     c(m23,j1,j2,j3)*u(i1  ,i2+1,i3)+ \
     c(m21,j1,j2,j3)*u(i1  ,i2-1,i3) )
#defineMacro op2dSparsel1(j1,j2,j3,i1,i2,i3) ( \
     c(m32,j1,j2,j3)*u(i1+1,i2  ,i3)+ \
     c(m12,j1,j2,j3)*u(i1-1,i2  ,i3) )
#defineMacro op2dCC(i1,i2,i3) ( \
     cc(m11)*u(i1-1,i2-1,i3)+ \
     cc(m21)*u(i1  ,i2-1,i3)+ \
     cc(m31)*u(i1+1,i2-1,i3)+ \
     cc(m12)*u(i1-1,i2  ,i3)+ \
     cc(m22)*u(i1  ,i2  ,i3)+ \
     cc(m32)*u(i1+1,i2  ,i3)+ \
     cc(m13)*u(i1-1,i2+1,i3)+ \
     cc(m23)*u(i1  ,i2+1,i3)+ \
     cc(m33)*u(i1+1,i2+1,i3) )
#defineMacro op2dCCl0(i1,i2,i3) ( \
     cc(m11)*u(i1-1,i2-1,i3)+ \
     cc(m21)*u(i1  ,i2-1,i3)+ \
     cc(m31)*u(i1+1,i2-1,i3)+ \
     cc(m13)*u(i1-1,i2+1,i3)+ \
     cc(m23)*u(i1  ,i2+1,i3)+ \
     cc(m33)*u(i1+1,i2+1,i3) )
#defineMacro op2dCCl1(i1,i2,i3) ( \
     cc(m11)*u(i1-1,i2-1,i3)+ \
     cc(m31)*u(i1+1,i2-1,i3)+ \
     cc(m12)*u(i1-1,i2  ,i3)+ \
     cc(m32)*u(i1+1,i2  ,i3)+ \
     cc(m13)*u(i1-1,i2+1,i3)+ \
     cc(m33)*u(i1+1,i2+1,i3) )
#defineMacro op2dSparseCC(i1,i2,i3) ( \
     cc(m22)*u(i1  ,i2  ,i3)+ \
     cc(m32)*u(i1+1,i2  ,i3)+ \
     cc(m23)*u(i1  ,i2+1,i3)+ \
     cc(m12)*u(i1-1,i2  ,i3)+ \
     cc(m21)*u(i1  ,i2-1,i3) )
#defineMacro op2dSparseCCl0(i1,i2,i3) ( \
     cc(m23)*u(i1  ,i2+1,i3)+ \
     cc(m21)*u(i1  ,i2-1,i3) )
#defineMacro op2dSparseCCl1(i1,i2,i3) ( \
     cc(m32)*u(i1+1,i2  ,i3)+ \
     cc(m12)*u(i1-1,i2  ,i3) )

#defineMacro op3d(j1,j2,j3,i1,i2,i3) ( \
                    c(m111,j1,j2,j3)*u(i1-1,i2-1,i3-1)+ \
                    c(m211,j1,j2,j3)*u(i1  ,i2-1,i3-1)+ \
                    c(m311,j1,j2,j3)*u(i1+1,i2-1,i3-1)+ \
                    c(m121,j1,j2,j3)*u(i1-1,i2  ,i3-1)+ \
                    c(m221,j1,j2,j3)*u(i1  ,i2  ,i3-1)+ \
                    c(m321,j1,j2,j3)*u(i1+1,i2  ,i3-1)+ \
                    c(m131,j1,j2,j3)*u(i1-1,i2+1,i3-1)+ \
                    c(m231,j1,j2,j3)*u(i1  ,i2+1,i3-1)+ \
                    c(m331,j1,j2,j3)*u(i1+1,i2+1,i3-1)+ \
                    c(m112,j1,j2,j3)*u(i1-1,i2-1,i3  )+ \
                    c(m212,j1,j2,j3)*u(i1  ,i2-1,i3  )+ \
                    c(m312,j1,j2,j3)*u(i1+1,i2-1,i3  )+ \
                    c(m122,j1,j2,j3)*u(i1-1,i2  ,i3  )+ \
                    c(m222,j1,j2,j3)*u(i1  ,i2  ,i3  )+ \
                    c(m322,j1,j2,j3)*u(i1+1,i2  ,i3  )+ \
                    c(m132,j1,j2,j3)*u(i1-1,i2+1,i3  )+ \
                    c(m232,j1,j2,j3)*u(i1  ,i2+1,i3  )+ \
                    c(m332,j1,j2,j3)*u(i1+1,i2+1,i3  )+ \
                    c(m113,j1,j2,j3)*u(i1-1,i2-1,i3+1)+ \
                    c(m213,j1,j2,j3)*u(i1  ,i2-1,i3+1)+ \
                    c(m313,j1,j2,j3)*u(i1+1,i2-1,i3+1)+ \
                    c(m123,j1,j2,j3)*u(i1-1,i2  ,i3+1)+ \
                    c(m223,j1,j2,j3)*u(i1  ,i2  ,i3+1)+ \
                    c(m323,j1,j2,j3)*u(i1+1,i2  ,i3+1)+ \
                    c(m133,j1,j2,j3)*u(i1-1,i2+1,i3+1)+ \
                    c(m233,j1,j2,j3)*u(i1  ,i2+1,i3+1)+ \
                    c(m333,j1,j2,j3)*u(i1+1,i2+1,i3+1) )
 #defineMacro op3dl0(j1,j2,j3,i1,i2,i3) ( \
                    c(m111,j1,j2,j3)*u(i1-1,i2-1,i3-1)+ \
                    c(m211,j1,j2,j3)*u(i1  ,i2-1,i3-1)+ \
                    c(m311,j1,j2,j3)*u(i1+1,i2-1,i3-1)+ \
                    c(m121,j1,j2,j3)*u(i1-1,i2  ,i3-1)+ \
                    c(m221,j1,j2,j3)*u(i1  ,i2  ,i3-1)+ \
                    c(m321,j1,j2,j3)*u(i1+1,i2  ,i3-1)+ \
                    c(m131,j1,j2,j3)*u(i1-1,i2+1,i3-1)+ \
                    c(m231,j1,j2,j3)*u(i1  ,i2+1,i3-1)+ \
                    c(m331,j1,j2,j3)*u(i1+1,i2+1,i3-1)+ \
                    c(m112,j1,j2,j3)*u(i1-1,i2-1,i3  )+ \
                    c(m212,j1,j2,j3)*u(i1  ,i2-1,i3  )+ \
                    c(m312,j1,j2,j3)*u(i1+1,i2-1,i3  )+ \
                    c(m132,j1,j2,j3)*u(i1-1,i2+1,i3  )+ \
                    c(m232,j1,j2,j3)*u(i1  ,i2+1,i3  )+ \
                    c(m332,j1,j2,j3)*u(i1+1,i2+1,i3  )+ \
                    c(m113,j1,j2,j3)*u(i1-1,i2-1,i3+1)+ \
                    c(m213,j1,j2,j3)*u(i1  ,i2-1,i3+1)+ \
                    c(m313,j1,j2,j3)*u(i1+1,i2-1,i3+1)+ \
                    c(m123,j1,j2,j3)*u(i1-1,i2  ,i3+1)+ \
                    c(m223,j1,j2,j3)*u(i1  ,i2  ,i3+1)+ \
                    c(m323,j1,j2,j3)*u(i1+1,i2  ,i3+1)+ \
                    c(m133,j1,j2,j3)*u(i1-1,i2+1,i3+1)+ \
                    c(m233,j1,j2,j3)*u(i1  ,i2+1,i3+1)+ \
                    c(m333,j1,j2,j3)*u(i1+1,i2+1,i3+1) )
#defineMacro op3dl1(j1,j2,j3,i1,i2,i3) ( \
                    c(m111,j1,j2,j3)*u(i1-1,i2-1,i3-1)+ \
                    c(m211,j1,j2,j3)*u(i1  ,i2-1,i3-1)+ \
                    c(m311,j1,j2,j3)*u(i1+1,i2-1,i3-1)+ \
                    c(m121,j1,j2,j3)*u(i1-1,i2  ,i3-1)+ \
                    c(m221,j1,j2,j3)*u(i1  ,i2  ,i3-1)+ \
                    c(m321,j1,j2,j3)*u(i1+1,i2  ,i3-1)+ \
                    c(m131,j1,j2,j3)*u(i1-1,i2+1,i3-1)+ \
                    c(m231,j1,j2,j3)*u(i1  ,i2+1,i3-1)+ \
                    c(m331,j1,j2,j3)*u(i1+1,i2+1,i3-1)+ \
                    c(m112,j1,j2,j3)*u(i1-1,i2-1,i3  )+ \
                    c(m312,j1,j2,j3)*u(i1+1,i2-1,i3  )+ \
                    c(m122,j1,j2,j3)*u(i1-1,i2  ,i3  )+ \
                    c(m322,j1,j2,j3)*u(i1+1,i2  ,i3  )+ \
                    c(m132,j1,j2,j3)*u(i1-1,i2+1,i3  )+ \
                    c(m332,j1,j2,j3)*u(i1+1,i2+1,i3  )+ \
                    c(m113,j1,j2,j3)*u(i1-1,i2-1,i3+1)+ \
                    c(m213,j1,j2,j3)*u(i1  ,i2-1,i3+1)+ \
                    c(m313,j1,j2,j3)*u(i1+1,i2-1,i3+1)+ \
                    c(m123,j1,j2,j3)*u(i1-1,i2  ,i3+1)+ \
                    c(m223,j1,j2,j3)*u(i1  ,i2  ,i3+1)+ \
                    c(m323,j1,j2,j3)*u(i1+1,i2  ,i3+1)+ \
                    c(m133,j1,j2,j3)*u(i1-1,i2+1,i3+1)+ \
                    c(m233,j1,j2,j3)*u(i1  ,i2+1,i3+1)+ \
                    c(m333,j1,j2,j3)*u(i1+1,i2+1,i3+1) )
#defineMacro op3dl2(j1,j2,j3,i1,i2,i3) ( \
                    c(m111,j1,j2,j3)*u(i1-1,i2-1,i3-1)+ \
                    c(m211,j1,j2,j3)*u(i1  ,i2-1,i3-1)+ \
                    c(m311,j1,j2,j3)*u(i1+1,i2-1,i3-1)+ \
                    c(m121,j1,j2,j3)*u(i1-1,i2  ,i3-1)+ \
                    c(m321,j1,j2,j3)*u(i1+1,i2  ,i3-1)+ \
                    c(m131,j1,j2,j3)*u(i1-1,i2+1,i3-1)+ \
                    c(m231,j1,j2,j3)*u(i1  ,i2+1,i3-1)+ \
                    c(m331,j1,j2,j3)*u(i1+1,i2+1,i3-1)+ \
                    c(m112,j1,j2,j3)*u(i1-1,i2-1,i3  )+ \
                    c(m212,j1,j2,j3)*u(i1  ,i2-1,i3  )+ \
                    c(m312,j1,j2,j3)*u(i1+1,i2-1,i3  )+ \
                    c(m122,j1,j2,j3)*u(i1-1,i2  ,i3  )+ \
                    c(m322,j1,j2,j3)*u(i1+1,i2  ,i3  )+ \
                    c(m132,j1,j2,j3)*u(i1-1,i2+1,i3  )+ \
                    c(m232,j1,j2,j3)*u(i1  ,i2+1,i3  )+ \
                    c(m332,j1,j2,j3)*u(i1+1,i2+1,i3  )+ \
                    c(m113,j1,j2,j3)*u(i1-1,i2-1,i3+1)+ \
                    c(m213,j1,j2,j3)*u(i1  ,i2-1,i3+1)+ \
                    c(m313,j1,j2,j3)*u(i1+1,i2-1,i3+1)+ \
                    c(m123,j1,j2,j3)*u(i1-1,i2  ,i3+1)+ \
                    c(m323,j1,j2,j3)*u(i1+1,i2  ,i3+1)+ \
                    c(m133,j1,j2,j3)*u(i1-1,i2+1,i3+1)+ \
                    c(m233,j1,j2,j3)*u(i1  ,i2+1,i3+1)+ \
                    c(m333,j1,j2,j3)*u(i1+1,i2+1,i3+1) )
#defineMacro op3dSparse(j1,j2,j3,i1,i2,i3) ( \
                       c(m221,j1,j2,j3)*u(i1  ,i2  ,i3-1)+ \
                       c(m212,j1,j2,j3)*u(i1  ,i2-1,i3  )+ \
                       c(m122,j1,j2,j3)*u(i1-1,i2  ,i3  )+ \
                       c(m222,j1,j2,j3)*u(i1  ,i2  ,i3  )+ \
                       c(m322,j1,j2,j3)*u(i1+1,i2  ,i3  )+ \
                       c(m232,j1,j2,j3)*u(i1  ,i2+1,i3  )+ \
                       c(m223,j1,j2,j3)*u(i1  ,i2  ,i3+1) )
#defineMacro op3dSparsel0(j1,j2,j3,i1,i2,i3) ( \
                       c(m221,j1,j2,j3)*u(i1  ,i2  ,i3-1)+ \
                       c(m212,j1,j2,j3)*u(i1  ,i2-1,i3  )+ \
                       c(m232,j1,j2,j3)*u(i1  ,i2+1,i3  )+ \
                       c(m223,j1,j2,j3)*u(i1  ,i2  ,i3+1) )
#defineMacro op3dSparsel1(j1,j2,j3,i1,i2,i3) ( \
                       c(m221,j1,j2,j3)*u(i1  ,i2  ,i3-1)+ \
                       c(m122,j1,j2,j3)*u(i1-1,i2  ,i3  )+ \
                       c(m322,j1,j2,j3)*u(i1+1,i2  ,i3  )+ \
                       c(m223,j1,j2,j3)*u(i1  ,i2  ,i3+1) )
#defineMacro op3dSparsel2(j1,j2,j3,i1,i2,i3) ( \
                       c(m212,j1,j2,j3)*u(i1  ,i2-1,i3  )+ \
                       c(m122,j1,j2,j3)*u(i1-1,i2  ,i3  )+ \
                       c(m322,j1,j2,j3)*u(i1+1,i2  ,i3  )+ \
                       c(m232,j1,j2,j3)*u(i1  ,i2+1,i3  ) )

#defineMacro op3dSparseCC(i1,i2,i3) ( \
     cc(m221)*u(i1  ,i2  ,i3-1)+ \
     cc(m212)*u(i1  ,i2-1,i3  )+ \
     cc(m122)*u(i1-1,i2  ,i3  )+ \
     cc(m222)*u(i1  ,i2  ,i3  )+ \
     cc(m322)*u(i1+1,i2  ,i3  )+ \
     cc(m232)*u(i1  ,i2+1,i3  )+ \
     cc(m223)*u(i1  ,i2  ,i3+1) )
 #defineMacroop3dSparseCCl0(i1,i2,i3) ( \
     cc(m221)*u(i1  ,i2  ,i3-1)+ \
     cc(m212)*u(i1  ,i2-1,i3  )+ \
     cc(m232)*u(i1  ,i2+1,i3  )+ \
     cc(m223)*u(i1  ,i2  ,i3+1) )
#defineMacro op3dSparseCCl1(i1,i2,i3) ( \
     cc(m221)*u(i1  ,i2  ,i3-1)+ \
     cc(m122)*u(i1-1,i2  ,i3  )+ \
     cc(m322)*u(i1+1,i2  ,i3  )+ \
     cc(m223)*u(i1  ,i2  ,i3+1) )
#defineMacro op3dSparseCCl2(i1,i2,i3) ( \
     cc(m212)*u(i1  ,i2-1,i3  )+ \
     cc(m122)*u(i1-1,i2  ,i3  )+ \
     cc(m322)*u(i1+1,i2  ,i3  )+ \
     cc(m232)*u(i1  ,i2+1,i3  ) )

#defineMacro op3dCC(i1,i2,i3) ( \
                    cc(m111)*u(i1-1,i2-1,i3-1)+ \
                    cc(m211)*u(i1  ,i2-1,i3-1)+ \
                    cc(m311)*u(i1+1,i2-1,i3-1)+ \
                    cc(m121)*u(i1-1,i2  ,i3-1)+ \
                    cc(m221)*u(i1  ,i2  ,i3-1)+ \
                    cc(m321)*u(i1+1,i2  ,i3-1)+ \
                    cc(m131)*u(i1-1,i2+1,i3-1)+ \
                    cc(m231)*u(i1  ,i2+1,i3-1)+ \
                    cc(m331)*u(i1+1,i2+1,i3-1)+ \
                    cc(m112)*u(i1-1,i2-1,i3  )+ \
                    cc(m212)*u(i1  ,i2-1,i3  )+ \
                    cc(m312)*u(i1+1,i2-1,i3  )+ \
                    cc(m122)*u(i1-1,i2  ,i3  )+ \
                    cc(m222)*u(i1  ,i2  ,i3  )+ \
                    cc(m322)*u(i1+1,i2  ,i3  )+ \
                    cc(m132)*u(i1-1,i2+1,i3  )+ \
                    cc(m232)*u(i1  ,i2+1,i3  )+ \
                    cc(m332)*u(i1+1,i2+1,i3  )+ \
                    cc(m113)*u(i1-1,i2-1,i3+1)+ \
                    cc(m213)*u(i1  ,i2-1,i3+1)+ \
                    cc(m313)*u(i1+1,i2-1,i3+1)+ \
                    cc(m123)*u(i1-1,i2  ,i3+1)+ \
                    cc(m223)*u(i1  ,i2  ,i3+1)+ \
                    cc(m323)*u(i1+1,i2  ,i3+1)+ \
                    cc(m133)*u(i1-1,i2+1,i3+1)+ \
                    cc(m233)*u(i1  ,i2+1,i3+1)+ \
                    cc(m333)*u(i1+1,i2+1,i3+1) )
#defineMacro op3dCCl0(i1,i2,i3) ( \
                    cc(m111)*u(i1-1,i2-1,i3-1)+ \
                    cc(m211)*u(i1  ,i2-1,i3-1)+ \
                    cc(m311)*u(i1+1,i2-1,i3-1)+ \
                    cc(m121)*u(i1-1,i2  ,i3-1)+ \
                    cc(m221)*u(i1  ,i2  ,i3-1)+ \
                    cc(m321)*u(i1+1,i2  ,i3-1)+ \
                    cc(m131)*u(i1-1,i2+1,i3-1)+ \
                    cc(m231)*u(i1  ,i2+1,i3-1)+ \
                    cc(m331)*u(i1+1,i2+1,i3-1)+ \
                    cc(m112)*u(i1-1,i2-1,i3  )+ \
                    cc(m212)*u(i1  ,i2-1,i3  )+ \
                    cc(m312)*u(i1+1,i2-1,i3  )+ \
                    cc(m132)*u(i1-1,i2+1,i3  )+ \
                    cc(m232)*u(i1  ,i2+1,i3  )+ \
                    cc(m332)*u(i1+1,i2+1,i3  )+ \
                    cc(m113)*u(i1-1,i2-1,i3+1)+ \
                    cc(m213)*u(i1  ,i2-1,i3+1)+ \
                    cc(m313)*u(i1+1,i2-1,i3+1)+ \
                    cc(m123)*u(i1-1,i2  ,i3+1)+ \
                    cc(m223)*u(i1  ,i2  ,i3+1)+ \
                    cc(m323)*u(i1+1,i2  ,i3+1)+ \
                    cc(m133)*u(i1-1,i2+1,i3+1)+ \
                    cc(m233)*u(i1  ,i2+1,i3+1)+ \
                    cc(m333)*u(i1+1,i2+1,i3+1) )
#defineMacro op3dCCl1(i1,i2,i3) ( \
                    cc(m111)*u(i1-1,i2-1,i3-1)+ \
                    cc(m211)*u(i1  ,i2-1,i3-1)+ \
                    cc(m311)*u(i1+1,i2-1,i3-1)+ \
                    cc(m121)*u(i1-1,i2  ,i3-1)+ \
                    cc(m221)*u(i1  ,i2  ,i3-1)+ \
                    cc(m321)*u(i1+1,i2  ,i3-1)+ \
                    cc(m131)*u(i1-1,i2+1,i3-1)+ \
                    cc(m231)*u(i1  ,i2+1,i3-1)+ \
                    cc(m331)*u(i1+1,i2+1,i3-1)+ \
                    cc(m112)*u(i1-1,i2-1,i3  )+ \
                    cc(m312)*u(i1+1,i2-1,i3  )+ \
                    cc(m122)*u(i1-1,i2  ,i3  )+ \
                    cc(m322)*u(i1+1,i2  ,i3  )+ \
                    cc(m132)*u(i1-1,i2+1,i3  )+ \
                    cc(m332)*u(i1+1,i2+1,i3  )+ \
                    cc(m113)*u(i1-1,i2-1,i3+1)+ \
                    cc(m213)*u(i1  ,i2-1,i3+1)+ \
                    cc(m313)*u(i1+1,i2-1,i3+1)+ \
                    cc(m123)*u(i1-1,i2  ,i3+1)+ \
                    cc(m223)*u(i1  ,i2  ,i3+1)+ \
                    cc(m323)*u(i1+1,i2  ,i3+1)+ \
                    cc(m133)*u(i1-1,i2+1,i3+1)+ \
                    cc(m233)*u(i1  ,i2+1,i3+1)+ \
                    cc(m333)*u(i1+1,i2+1,i3+1) )
#defineMacro op3dCCl2(i1,i2,i3) ( \
                    cc(m111)*u(i1-1,i2-1,i3-1)+ \
                    cc(m211)*u(i1  ,i2-1,i3-1)+ \
                    cc(m311)*u(i1+1,i2-1,i3-1)+ \
                    cc(m121)*u(i1-1,i2  ,i3-1)+ \
                    cc(m321)*u(i1+1,i2  ,i3-1)+ \
                    cc(m131)*u(i1-1,i2+1,i3-1)+ \
                    cc(m231)*u(i1  ,i2+1,i3-1)+ \
                    cc(m331)*u(i1+1,i2+1,i3-1)+ \
                    cc(m112)*u(i1-1,i2-1,i3  )+ \
                    cc(m212)*u(i1  ,i2-1,i3  )+ \
                    cc(m312)*u(i1+1,i2-1,i3  )+ \
                    cc(m122)*u(i1-1,i2  ,i3  )+ \
                    cc(m322)*u(i1+1,i2  ,i3  )+ \
                    cc(m132)*u(i1-1,i2+1,i3  )+ \
                    cc(m232)*u(i1  ,i2+1,i3  )+ \
                    cc(m332)*u(i1+1,i2+1,i3  )+ \
                    cc(m113)*u(i1-1,i2-1,i3+1)+ \
                    cc(m213)*u(i1  ,i2-1,i3+1)+ \
                    cc(m313)*u(i1+1,i2-1,i3+1)+ \
                    cc(m123)*u(i1-1,i2  ,i3+1)+ \
                    cc(m323)*u(i1+1,i2  ,i3+1)+ \
                    cc(m133)*u(i1-1,i2+1,i3+1)+ \
                    cc(m233)*u(i1  ,i2+1,i3+1)+ \
                    cc(m333)*u(i1+1,i2+1,i3+1) )

! ===============================================================================
! Loop over boundary points (alos compute the ghost points)
!   (i1,i2,i3) = boundary point
!   (j1,j2,j3) = ghost point
!   
! ===============================================================================
#beginMacro beginBoundaryLoop()
 do i3=m3a,m3b,m3c
 j3=i3-is3
 do i2=m2a,m2b,m2c
 j2=i2-is2
 do i1=m1a,m1b,m1c
 j1=i1-is1
#endMacro

#beginMacro endBoundaryLoop()
 end do
 end do
 end do
#endMacro

! ===============================================================================
! Line-smooth RHS for Neumann/Mixed BC, 2nd-order, constant coefficients
! ===============================================================================
#beginMacro neumannSecondOrderConstCoeff()

 if( useBoundaryForcing.eq.1 )then
  beginBoundaryLoop()
   if( mask(i1,i2,i3).gt.0 )then
     r(j1,j2,j3)=f(j1,j2,j3)
   else
     r(j1,j2,j3)=0.
   end if
  endBoundaryLoop()
 else
  beginBoundaryLoop()
   r(j1,j2,j3)=0.
  endBoundaryLoop()
 end if
#endMacro


! ===================================================================================
! Line-smooth RHS for Neumann/Mixed BC, 2nd-order, stored in the coefficient matrix
!
!  DIM : 2 or 3
!  DIR : 0, 1 or 2 for line smooth in direction DIR
! ===================================================================================
#beginMacro neumannSecondOrder(DIR,DIM)

 if( useBoundaryForcing.eq.1 )then
  beginBoundaryLoop()
   if( mask(i1,i2,i3).gt.0 )then
    r(j1,j2,j3)=f(j1,j2,j3) - op ## DIM ## dl ## DIR(j1,j2,j3,i1,i2,i3)
    ! write(*,'(" LS-NRHS: set j1,j2=",2i3," rhs=",e10.2," f=",e10.2)') j1,j2,r(j1,j2,j3),f(j1,j2,j3)
    ! write(*,'(" c11,c12,c13=",3e10.2," c21,c22,c23=",3e10.2," c31,c32,c33=",3e10.2)') \
    !     c(m11,j1,j2,j3),c(m12,j1,j2,j3),c(m13,j1,j2,j3),\
    !     c(m21,j1,j2,j3),c(m22,j1,j2,j3),c(m23,j1,j2,j3),\
    !     c(m31,j1,j2,j3),c(m32,j1,j2,j3),c(m33,j1,j2,j3)
   else
    r(j1,j2,j3)=0.
   end if
  endBoundaryLoop()
 else
  beginBoundaryLoop()
   if( mask(i1,i2,i3).gt.0 )then
    r(j1,j2,j3)=            - op ## DIM ## dl ## DIR(j1,j2,j3,i1,i2,i3)
   else
    r(j1,j2,j3)=0.
   end if
  endBoundaryLoop()
 end if

#endMacro

! ===============================================================================
! Line-smooth RHS for true Neumann/Mixed BC, 2nd-order, curvilinear
!
! We discretize the following BC to second order: 
! 
! a1*( n1*ux + n2*ux + n3*uz ) + a0*u = f 
! a1*( (n1*rx+n2*ry+n3*rz)*ur + (n1*sx+n2*sy+n3*sz)*us + (n1*tx+n2*ty+n3*st)*ut ) + a0*u = f 
!
! ===============================================================================
#beginMacro mixedSecondOrder()

 a0=bcData(0,side,axis)
 a1=bcData(1,side,axis)

 ! write(*,'(" lineSmoothRHS: assign RHS for mixedSecondOrder, side,axis,grid=",3i3," a0,a1=",2f6.2)') side,axis,grid,a0,a1
 is=1-2*side
 axisp1 = mod(axis+1,nd)
 axisp2 = mod(axis+2,nd)
 beginBoundaryLoop()
   ! Boundary: (i1,i2,i3)
   ! Ghost:    (j1,j2,j3)
   if( mask(i1,i2,i3).gt.0 )then

    ! compute ur and us to second order:
    urv(0) = (u(i1+1,i2,i3)-u(i1-1,i2,i3))/(2.*dr(0))
    urv(1) = (u(i1,i2+1,i3)-u(i1,i2-1,i3))/(2.*dr(1))

    an1 = rsxy(i1,i2,i3,axis,0)
    an2 = rsxy(i1,i2,i3,axis,1)

    if( nd.eq.2 )then

     aNormi = (-is)/sqrt(an1**2+an2**2)
     an1=an1*aNormi
     an2=an2*aNormi
     t1=a1*( an1*rsxy(i1,i2,i3,axisp1,0)+an2*rsxy(i1,i2,i3,axisp1,1) )
     
     if( useBoundaryForcing.eq.1 )then
       r(j1,j2,j3)=f(j1,j2,j3) - ( t1*urv(axisp1) )
     else
       r(j1,j2,j3)=            - ( t1*urv(axisp1) )
     end if

    else

     ! compute ut to second order:
     urv(2) = (u(i1,i2,i3+1)-u(i1,i2,i3-1))/(2.*dr(2))

     an3 = rsxy(i1,i2,i3,axis,2)
     aNormi = (-is)/sqrt(an1**2+an2**2+an3**2)
     an1=an1*aNormi
     an2=an2*aNormi
     an3=an3*aNormi
     t1=a1*( an1*rsxy(i1,i2,i3,axisp1,0)+an2*rsxy(i1,i2,i3,axisp1,1)+an3*rsxy(i1,i2,i3,axisp1,2) )
     t2=a1*( an1*rsxy(i1,i2,i3,axisp2,0)+an2*rsxy(i1,i2,i3,axisp2,1)+an3*rsxy(i1,i2,i3,axisp2,2) )

     if( useBoundaryForcing.eq.1 )then
       r(j1,j2,j3)=f(j1,j2,j3) - ( t1*urv(axisp1) +t2*urv(axisp2) )
     else
       r(j1,j2,j3)=            - ( t1*urv(axisp1) +t2*urv(axisp2) )
     end if
    end if

   else
    r(j1,j2,j3)=0.
   end if
 endBoundaryLoop()


#endMacro


! ===============================================================================
! Line-smooth RHS for true Neumann/Mixed BC, 2nd-order, curvilinear, TWO LINES 
!   *wdh* 110308
!
! Note: This BC is used on lower levels when fourth-order is used on the finest level.
!
! We discretize the following BC to second order: 
! 
! a1*( n1*ux + n2*ux + n3*uz ) + a0*u = f 
! a1*( (n1*rx+n2*ry+n3*rz)*ur + (n1*sx+n2*sy+n3*sz)*us + (n1*tx+n2*ty+n3*st)*ut ) + a0*u = f 
!
! ===============================================================================
#beginMacro mixedSecondOrderTwoLines()

 a0=bcData(0,side,axis)
 a1=bcData(1,side,axis)

 ! write(*,'(">>> lineSmoothRHS: assign RHS for mixedSecondOrderTwoLines, side,axis,grid=",3i3," a0,a1=",2f6.2)') side,axis,grid,a0,a1

 is=1-2*side
 axisp1 = mod(axis+1,nd)
 axisp2 = mod(axis+2,nd)
 beginBoundaryLoop()
   ! Boundary: (i1,i2,i3)
   ! Ghost:    (j1,j2,j3)

   k1=i1-2*is1 ! (k1,k2,k3) is the 2nd ghost line
   k2=i2-2*is2
   k3=i3-2*is3
   if( mask(i1,i2,i3).gt.0 )then

    ! compute ur and us to second order:
    urv(0)  = (u(i1+1,i2,i3)-u(i1-1,i2,i3))/(2.*dr(0))
    urv(1)  = (u(i1,i2+1,i3)-u(i1,i2-1,i3))/(2.*dr(1))
    ! use wider stencil for 2nd ghost 
    urv2(0) = (u(i1+2,i2,i3)-u(i1-2,i2,i3))/(4.*dr(0))
    urv2(1) = (u(i1,i2+2,i3)-u(i1,i2-2,i3))/(4.*dr(1))

    an1 = rsxy(i1,i2,i3,axis,0)
    an2 = rsxy(i1,i2,i3,axis,1)

    if( nd.eq.2 )then

     aNormi = (-is)/sqrt(an1**2+an2**2)
     an1=an1*aNormi
     an2=an2*aNormi
     t1=a1*( an1*rsxy(i1,i2,i3,axisp1,0)+an2*rsxy(i1,i2,i3,axisp1,1) )
     
     if( useBoundaryForcing.eq.1 )then
       ! Note forcing is stored in the ghost point of f
       r(j1,j2,j3)=f(j1,j2,j3) - ( t1*urv(axisp1) )
       r(k1,k2,k3)=f(j1,j2,j3) - ( t1*urv2(axisp1) )
     else
       r(j1,j2,j3)=            - ( t1*urv(axisp1) )
       r(k1,k2,k3)=            - ( t1*urv2(axisp1) )
     end if

    else

     ! compute ut to second order:
     urv(2)  = (u(i1,i2,i3+1)-u(i1,i2,i3-1))/(2.*dr(2))
     urv2(2) = (u(i1,i2,i3+2)-u(i1,i2,i3-2))/(4.*dr(2))

     an3 = rsxy(i1,i2,i3,axis,2)
     aNormi = (-is)/sqrt(an1**2+an2**2+an3**2)
     an1=an1*aNormi
     an2=an2*aNormi
     an3=an3*aNormi
     t1=a1*( an1*rsxy(i1,i2,i3,axisp1,0)+an2*rsxy(i1,i2,i3,axisp1,1)+an3*rsxy(i1,i2,i3,axisp1,2) )
     t2=a1*( an1*rsxy(i1,i2,i3,axisp2,0)+an2*rsxy(i1,i2,i3,axisp2,1)+an3*rsxy(i1,i2,i3,axisp2,2) )

     if( useBoundaryForcing.eq.1 )then
       ! Note forcing is stored in the ghost point of f
       r(j1,j2,j3)=f(j1,j2,j3) - ( t1*urv(axisp1)  +t2*urv(axisp2) )
       r(k1,k2,k3)=f(j1,j2,j3) - ( t1*urv2(axisp1) +t2*urv2(axisp2) )
     else
       r(j1,j2,j3)=            - ( t1*urv(axisp1)  +t2*urv(axisp2) )
       r(k1,k2,k3)=            - ( t1*urv2(axisp1) +t2*urv2(axisp2) )
     end if
    end if

   else
    r(j1,j2,j3)=0.
    r(k1,k2,k3)=0.
   end if
 endBoundaryLoop()


#endMacro

      subroutine lineSmoothRHS( nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &    ndc, c, cc,  r, s, u, f, mask, rsxy, ipar, rpar, ndbcd,bcData )
! ===================================================================================
!  Line smooth assign RHS
!
!  r : rhs to be filled in
!  
! ===================================================================================

      implicit none
      integer nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,ndc,ndbcd
      integer nda1a,nda1b,nda2a,nda2b,nda3a,nda3b

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real bcData(0:ndbcd-1,0:1,0:2)
      integer ipar(0:*)


      real r(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real f(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real c(1:ndc,nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real s(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real cc(0:*)
      real rpar(0:*)

!....local variables
      integer n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c,sparseStencil,orderOfAccuracy,bcOptionD,bcOptionN
      integer i1,i2,i3,m1a,m1b,m1c,m2a,m2b,m2c,m3a,m3b,m3c,j1,j2,j3,is1,is2,is3,is
      integer i1m1,i1p1,i2m1,i2p1,i3m1,i3p1,k1,k2,k3
      integer l1a,l1b,l2a,l2b,l3a,l3b,kd,shift
      integer nn1a,nn1b,nn2a,nn2b,nn3a,nn3b
      integer useBoundaryForcing,grid,level,axisp1,axisp2
      integer direction,width,width2,md,mdm1,mdp1,mdm2,mdp2,side,axis,md2,md2m1,md2p1,ms2
      integer bc(0:1,0:2),orderOfExtrapD,orderOfExtrapN,gridType,isNeumannBC(0:1)
      real dx(0:2),dr(0:2),urv(0:2),urv2(0:2)
      real dxi,dx2i,dxm

      real nsign,aNormi

      real drn
      real cf1,cf2,cg1,cg2
      real uu,us,uss,usss,ur,urr,urrr,urs,urss,urrs
      real ut,utt,uttt, ust,urt, urrt, usst, urtt, ustt, urst

      real a0,a1,a2,alpha1,alpha2
      real rxi,ryi,sxi,syi,rxr,rxs,sxr,sxs,ryr,rys,syr,sys
      real rxxi,ryyi,sxxi,syyi
      real rxrr,rxrs,rxss,ryrr,ryrs,ryss
      real sxrr,sxrs,sxss,syrr,syrs,syss
      real rxx,ryy,sxx,syy
      real rxxr,ryyr,rxxs,ryys, sxxr,syyr,sxxs,syys
      real rxNormI,rxNormIs,rxNormIss,rxNormIr,rxNormIrr
      real sxNormI,sxNormIs,sxNormIss,sxNormIr,sxNormIrr
      ! real n1,n1s,n1ss,n2,n2s,n2ss,n1r,n2r,n1rr,n2rr
      ! real an1,an1s,an1ss,an2,an2s,an2ss,an1r,an1rr,an2r,an2rs,an2rr
      ! real ff,ffs,ffr,g,gs,gss,gr,grr,gtt,fft,gt,gst,grt,grs
      ! real c11,c11r,c11s,c12,c12r,c12s,c22,c22r,c22s,c1,c1r,c1s,c2,c2r,c2s,c0,c0r,c0s
      ! real b0,b1,b2,b3,bf,br2

      real fv(-1:1,-1:1,-1:1), gv(-1:1,-1:1,-1:1)

      ! Delare variables for the order 4 Neumann BCs
      declareNeumannEquationVariables()


      integer m11,m12,m13,m14,m15,
     &        m21,m22,m23,m24,m25,
     &        m31,m32,m33,m34,m35,
     &        m41,m42,m43,m44,m45,
     &        m51,m52,m53,m54,m55
      integer    m111,m211,m311,m411,m511,
     &           m121,m221,m321,m421,m521,
     &           m131,m231,m331,m431,m531,
     &           m141,m241,m341,m441,m541,
     &           m151,m251,m351,m451,m551,
     &           m112,m212,m312,m412,m512,
     &           m122,m222,m322,m422,m522,
     &           m132,m232,m332,m432,m532,
     &           m142,m242,m342,m442,m542,
     &           m152,m252,m352,m452,m552,
     &           m113,m213,m313,m413,m513,
     &           m123,m223,m323,m423,m523,
     &           m133,m233,m333,m433,m533,
     &           m143,m243,m343,m443,m543,
     &           m153,m253,m353,m453,m553,
     &           m114,m214,m314,m414,m514,
     &           m124,m224,m324,m424,m524,
     &           m134,m234,m334,m434,m534,
     &           m144,m244,m344,m444,m544,
     &           m154,m254,m354,m454,m554,
     &           m115,m215,m315,m415,m515,
     &           m125,m225,m325,m425,m525,
     &           m135,m235,m335,m435,m535,
     &           m145,m245,m345,m445,m545,
     &           m155,m255,m355,m455,m555

      integer general, sparse, constantCoefficients, 
     &   sparseConstantCoefficients,sparseVariableCoefficients,
     &   variableCoefficients
      parameter( general=0, 
     &           sparse=1, 
     &           constantCoefficients=2,
     &           sparseConstantCoefficients=3,
     &           sparseVariableCoefficients=4,
     &           variableCoefficients=5 )

      integer dirichlet,neumann,mixed,equation,extrapolation,
     &        combination 

      parameter( 
     &     dirichlet=1,
     &     neumann=2,
     &     mixed=3,
     &     equation=4,
     &     extrapolation=5,
     &     combination=6 )

      integer rectangular,curvilinear
      parameter(
     &     rectangular=0,
     &     curvilinear=1)

      integer equationToSolve
      integer userDefined,laplaceEquation,divScalarGradOperator,
     &  heatEquationOperator,variableHeatEquationOperator,
     &   divScalarGradHeatOperator,secondOrderConstantCoefficients,
     & axisymmetricLaplaceEquation
      parameter(
     & userDefined=0,
     & laplaceEquation=1,
     & divScalarGradOperator=2,              ! div[ s[x] grad ]
     & heatEquationOperator=3,               ! I + c0*Delta
     & variableHeatEquationOperator=4,       ! I + s[x]*Delta
     & divScalarGradHeatOperator=5,  ! I + div[ s[x] grad ]
     & secondOrderConstantCoefficients=6,
     & axisymmetricLaplaceEquation=7 )

      real rx,ry,rz,sx,sy,sz,tx,ty,tz
      declareDifferenceOrder2(u,RX)
      declareDifferenceOrder4(u,RX)

      real urss2,urtt2, urrs2, ustt2, urrt2, usst2

!....start statement functions 

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
      defineDifferenceOrder2Components0(u,RX)
      defineDifferenceOrder4Components0(u,RX)


      ! Here are some derivatives that haven't been defined yet: 
      urss2(i1,i2,i3)=(uss2(i1+1,i2,i3)-uss2(i1-1,i2,i3))*d12(0)
      urtt2(i1,i2,i3)=(utt2(i1+1,i2,i3)-utt2(i1-1,i2,i3))*d12(0)

      urrs2(i1,i2,i3)=(urr2(i1,i2+1,i3)-urr2(i1,i2-1,i3))*d12(1)
      ustt2(i1,i2,i3)=(utt2(i1,i2+1,i3)-utt2(i1,i2-1,i3))*d12(1)

      urrt2(i1,i2,i3)=(urr2(i1,i2,i3+1)-urr2(i1,i2,i3-1))*d12(2)
      usst2(i1,i2,i3)=(uss2(i1,i2,i3+1)-uss2(i1,i2,i3-1))*d12(2)


!....end statement function

!      write(*,9000) (((f(i1,i2,i3),i1=nd1a,nd1b),i2=nd2a,nd2b),i3=nd3a,nd3b)
! 9000 format(<nd1b-nd1a+1>(f4.1,1x))

      nd              =ipar(0)
      direction       =ipar(1)
      sparseStencil   =ipar(2)
      orderOfAccuracy =ipar(3)
      n1a             =ipar(4)
      n1b             =ipar(5)
      n1c             =ipar(6)
      n2a             =ipar(7)
      n2b             =ipar(8)
      n2c             =ipar(9)
      n3a             =ipar(10)
      n3b             =ipar(11)
      n3c             =ipar(12)
      bc(0,0)         =ipar(13)
      bc(1,0)         =ipar(14)
      bc(0,1)         =ipar(15)
      bc(1,1)         =ipar(16)
      bc(0,2)         =ipar(17)
      bc(1,2)         =ipar(18)
      bcOptionD       =ipar(19)  ! BC option for Dirichlet BC's
      bcOptionN       =ipar(20)  ! BC option for Neumann BC's
      orderOfExtrapD  =ipar(21)  ! for dirichlet
      orderOfExtrapN  =ipar(22)  ! for neumann
      gridType        =ipar(23)  ! **** added
      nn1a            =ipar(24)  ! these define the full bounds assuming no zebra solve, 
      nn1b            =ipar(25)  ! end points are used to determine when one-sided differences of f and g are needed
      nn2a            =ipar(26)
      nn2b            =ipar(27)
      nn3a            =ipar(28)
      nn3b            =ipar(29)

      grid            =ipar(30)
      level           =ipar(31)
      equationToSolve =ipar(32)
      useBoundaryForcing=ipar(33)
      isNeumannBC(0)  =ipar(34)
      isNeumannBC(1)  =ipar(35)


      dx(0)           =rpar(0)
      dx(1)           =rpar(1)
      dx(2)           =rpar(2)
      dr(0)           =rpar(3) ! **** added 
      dr(1)           =rpar(4)
      dr(2)           =rpar(5)

      ! Initialize a0 and derivatives (a0 is the coeff u in the Mixed BC) -- for now a0 is constant (a0 is set later)
      setZero10(a0,a0r,a0s,a0t,a0rr,a0ss,a0tt,a0rs,a0rt,a0st)


      if( nd.eq.2 .and. orderOfAccuracy.eq.2 )then
       m11=1                 ! MCE(-1,-1, 0)
       m21=2                 ! MCE( 0,-1, 0)
       m31=3                 ! MCE(+1,-1, 0)
       m12=4                 ! MCE(-1, 0, 0)
       m22=5                 ! MCE( 0, 0, 0)
       m32=6                 ! MCE(+1, 0, 0)
       m13=7                 ! MCE(-1,+1, 0)
       m23=8                 ! MCE( 0,+1, 0)
       m33=9     
      else if( nd.eq.2 .and. orderOfAccuracy.eq.4 )then
       m11=1     
       m21=2     
       m31=3     
       m41=4     
       m51=5     
       m12=6     
       m22=7     
       m32=8     
       m42=9     
       m52=10    
       m13=11    
       m23=12    
       m33=13    
       m43=14    
       m53=15    
       m14=16    
       m24=17    
       m34=18    
       m44=19    
       m54=20    
       m15=21    
       m25=22    
       m35=23    
       m45=24    
       m55=25    

      else if( nd.eq.3 .and. orderOfAccuracy.eq.2 )then
       m111=1 
       m211=2 
       m311=3 
       m121=4 
       m221=5 
       m321=6 
       m131=7 
       m231=8 
       m331=9 
       m112=10
       m212=11
       m312=12
       m122=13
       m222=14
       m322=15 
       m132=16
       m232=17
       m332=18
       m113=19
       m213=20
       m313=21
       m123=22
       m223=23
       m323=24 
       m133=25
       m233=26
       m333=27
      else if( nd.eq.3 .and. orderOfAccuracy.eq.4 )then
       m111=1 
       m211=2 
       m311=3 
       m411=4 
       m511=5 
       m121=6 
       m221=7 
       m321=8 
       m421=9 
       m521=10
       m131=11
       m231=12
       m331=13
       m431=14
       m531=15
       m141=16
       m241=17
       m341=18
       m441=19
       m541=20
       m151=21
       m251=22
       m351=23
       m451=24
       m551=25

       m112=26
       m212=27
       m312=28
       m412=29
       m512=30
       m122=31 
       m222=32
       m322=33
       m422=34
       m522=35
       m132=36
       m232=37
       m332=38
       m432=39
       m532=40
       m142=41
       m242=42
       m342=43
       m442=44
       m542=45
       m152=46
       m252=47
       m352=48
       m452=49
       m552=50

       m113=51 
       m213=52 
       m313=53 
       m413=54 
       m513=55 
       m123=56 
       m223=57 
       m323=58 
       m423=59 
       m523=60
       m133=61
       m233=62
       m333=63
       m433=64
       m533=65
       m143=66
       m243=67
       m343=68
       m443=69
       m543=70
       m153=71
       m253=72
       m353=73
       m453=74
       m553=75

       m114=76
       m214=77
       m314=78
       m414=79
       m514=80
       m124=81 
       m224=82
       m324=83
       m424=84
       m524=85
       m134=86
       m234=87
       m334=88
       m434=89
       m534=90
       m144=91
       m244=92
       m344=93
       m444=94
       m544=95
       m154=96
       m254=97
       m354=98
       m454=99
       m554=100

       m115=101 
       m215=102 
       m315=103 
       m415=104 
       m515=105 
       m125=106 
       m225=107 
       m325=108 
       m425=109 
       m525=110
       m135=111
       m235=112
       m335=113
       m435=114
       m535=115
       m145=116
       m245=117
       m345=118
       m445=119
       m545=120
       m155=121
       m255=122
       m355=123
       m455=124
       m555=125
      else
        stop 5561
      end if

      axis=direction ! we only fill in BC's along the direction of the line solve
      is1=0
      is2=0
      is3=0
      do side=0,1
        if( axis.eq.0 )then
          is1=1-2*side
        else if( axis.eq.1 )then
          is2=1-2*side
        else
          is3=1-2*side
        end if
        if( bc(side,axis).eq.equation )then

         ! write(*,'("LineSmoothRHS: Neumann: level=",i2," side,axis=",2i2," bc=",i3," bcOptionN=",i2," sparseStencil=",i3," useBoundaryForcing=",i3)') level,side,axis,bc(side,axis),bcOptionN,sparseStencil,useBoundaryForcing

         ! --------------------------
         ! ---- Neumann or mixed ----
         ! --------------------------


         if( orderOfAccuracy.eq.2 )then
           ! --------- 2nd order ------------

           shift=1 ! shift to boundary (1 ghost)
           getBoundaryIndex(side,axis,m1a,m1b,m2a,m2b,m3a,m3b,shift) ! boundary 
           m1c=n1c
           m2c=n2c
           m3c=n3c

           ! bcOptionN = 0 : extrapolate
           !           = 1 : Use equation 
           !           = 2 : even symmetry
           !           = 3 : use mixed BC to second order 

           if( bcOptionN.le.0 .or. bcOptionN.gt.3 )then
             write(*,'("lineSmoothRHS: ERROR: unexpected bcOptionN=",i6)') bcOptionN
             stop 8822
           end if
           ! *wdh* 110220 if( bcOptionN.ne.1 .or. 
           if( bcOptionN.eq.2 .or. ( bcOptionN.eq.3 .and. gridType.eq.rectangular ) .or. \
               ( (sparseStencil.eq.constantCoefficients .or. sparseStencil.eq.sparseConstantCoefficients ) \
                                           .and. gridType.eq.rectangular)  )then
             ! Neumann/mixed, 2nd-order, const. coeff and rectangular

             ! write(*,'("LineSmoothRHS: Mixed/Neumann-rect : level=",i2," side,axis=",2i2," bc=",i3," bcOptionN=",i2," sparseStencil=",i3," useBoundaryForcing=",i3)') level,side,axis,bc(side,axis),bcOptionN,sparseStencil,useBoundaryForcing


             neumannSecondOrderConstCoeff()

           else if( bcOptionN.eq.3 )then
             ! True Neumann/mixed, 2nd-order
             if( gridType.eq.rectangular )then
               stop 4321
             end if

             mixedSecondOrder()

           else
             ! Neumann/mixed, 2nd-order, BC is stored in the coeff array
            
             !  write(*,'("LineSmoothRHS: Mixed/Neumann-curv : level=",i2," side,axis=",2i2," bc=",i3," bcOptionN=",i2," sparseStencil=",i3," useBoundaryForcing=",i3)') level,side,axis,bc(side,axis),bcOptionN,sparseStencil,useBoundaryForcing

            if( axis.eq.0 .and. nd.eq.2 )then
             neumannSecondOrder(0,2)
            else if( axis.eq.1 .and. nd.eq.2 )then
             neumannSecondOrder(1,2)

            else if( axis.eq.0 .and. nd.eq.3 )then
             neumannSecondOrder(0,3)
            else if( axis.eq.1 .and. nd.eq.3 )then
             neumannSecondOrder(1,3)
            else if( axis.eq.2 .and. nd.eq.3 )then
             neumannSecondOrder(2,3)
            else
             stop 116
            end if

           end if

         else if( orderOfAccuracy.eq.4 )then
           ! --------- 4th order ------------

          shift=2 ! shift to boundary (there are 2 ghost)
          getBoundaryIndex(side,axis,m1a,m1b,m2a,m2b,m3a,m3b,shift) ! boundary 
          m1c=n1c
          m2c=n2c
          m3c=n3c

          if( bcOptionN.eq.1 )then

           if( axis.eq.0 .and. nd.eq.2 )then
            neumannAndEquation(forcing,R,2)
           else if( axis.eq.1 .and. nd.eq.2 )then
            neumannAndEquation(forcing,S,2)
           else if( axis.eq.0 .and. nd.eq.3 )then
            neumannAndEquation(forcing,R,3)
           else if( axis.eq.1 .and. nd.eq.3 )then
            neumannAndEquation(forcing,S,3)
           else if( axis.eq.2 .and. nd.eq.3 )then
            neumannAndEquation(forcing,T,3)
           else
            stop 10
           end if

          else if( bcOptionN.eq.0 .and. isNeumannBC(side).eq.1 .and. level.gt.0 .and. gridType.eq.curvilinear )then

           ! second-order Neumann/mixed BC ise used on lower levels of a 4th-order method
           ! write(*,'("$$$lineSmoothRHS: assign RHS for 2nd order Neumann/mixed on lower levels")')

           mixedSecondOrder()

          else if( bcOptionN.eq.3. )then
           ! *new* 110308 -- apply real 2nd-order approximations on two ghost 
           ! write(*,'(">>>lineSmoothRHS: assign RHS for 2nd order Neumann/mixed on TWO lines")')
             
           if( gridType.eq.curvilinear )then
             mixedSecondOrderTwoLines()
           else
             ! On Cartesian grids there is no RHS to fill in unless there is a forcing. This only
             ! occurs on level=0 in whcih case this BC should not normally be used.:
             if( level.eq.0 )then
               write(*,'("lineSmoothRHS:ERROR: unexpected case encountered")')
               stop 1212
             end if
           end if

          else 

             ! -- these remaining cases should have already been done 
             ! in lineSmooth.bC: lineSmoothBoundaryConditions (macro)

          end if
         else
          write(*,'(" ERROR: lineSmoothRHS: orderOfAccuracy=",i4)') orderOfAccuracy
          stop 8145
         end if
        end if
      end do

      return 
      end


#beginMacro beginLoop()
do i3=n3a,n3b,n3c
do i2=n2a,n2b,n2c
do i1=n1a,n1b,n1c
#endMacro
#beginMacro endLoop()
end do
end do
end do
#endMacro

#beginMacro computeOmega2d(k1,k2,k3)
 c1=abs(c(m1a,k1,k2,k3)+c(m1b,k1,k2,k3))  
 c2=abs(c(m2a,k1,k2,k3)+c(m2b,k1,k2,k3))
 ! At the end points the coefficients may be zero
!  if( c1+c2 .gt. 0. )then
   cmax=1.-min(c1,c2)/(c1+c2)
!  else
!    cmax=.5
!  end if
 omega=variableOmegaFactor/(1.+sqrt(1.-cmax**2))
 ! omega=1.072*(.666-cmax)/.166 + 1.145*(cmax-.5)/.166
 ! write(*,'(''k1,k2='',2i3,'' c1,c2,cmax,omega='',4(f9.4,1x))') k1,k2,c1,c2,cmax,omega
#endMacro

#beginMacro computeOmega2dFourthOrder(k1,k2,k3)
 c1=abs(c(m1a,k1,k2,k3)+c(m1b,k1,k2,k3))  
 c2=abs(c(m2a,k1,k2,k3)+c(m2b,k1,k2,k3))
 ! At the end points the coefficients may be zero
!  if( c1+c2 .gt. 0. )then
   cmin=min(c1,c2)/(c1+c2)
!  else
!    cmin=.5
!  end if
 omega=(1.23-.16*cmin)*variableOmegaFactor  ! w(.5)=1.15 w(.25)=1.19 -- this is for a W[2,1] ***
 ! write(*,'(''k1,k2='',2i3,'' cmin,omega='',2(f7.4,1x))') k1,k2,cmin,omega
#endMacro

! *********  FINISH ME *****
#beginMacro computeOmega3d(k1,k2,k3)
 write(*,'(" lineSmoothUpdate:ERROR: finish me for 3D and variable Omega")')
 stop 6298
 c1=abs(c(m1a,k1,k2,k3)+c(m1b,k1,k2,k3))  
 c2=abs(c(m2a,k1,k2,k3)+c(m2b,k1,k2,k3))
 ! At the end points the coefficients may be zero
!  if( c1+c2 .gt. 0. )then
   cmax=1.-min(c1,c2)/(c1+c2)
!  else
!    cmax=.5
!  end if
 omega=variableOmegaFactor/(1.+sqrt(1.-cmax**2))
 ! omega=1.072*(.666-cmax)/.166 + 1.145*(cmax-.5)/.166
 ! write(*,'(''k1,k2='',2i3,'' c1,c2,cmax,omega='',4(f9.4,1x))') k1,k2,c1,c2,cmax,omega
#endMacro

! *********  FINISH ME *****
#beginMacro computeOmega3dFourthOrder(k1,k2,k3)
 write(*,'(" lineSmoothUpdate:ERROR: finish me for 3D and variable Omega")')
 stop 6297
 c1=abs(c(m1a,k1,k2,k3)+c(m1b,k1,k2,k3))  
 c2=abs(c(m2a,k1,k2,k3)+c(m2b,k1,k2,k3))
 ! At the end points the coefficients may be zero
!  if( c1+c2 .gt. 0. )then
   cmin=min(c1,c2)/(c1+c2)
!  else
!    cmin=.5
!  end if
 omega=(1.23-.16*cmin)*variableOmegaFactor  ! w(.5)=1.15 w(.25)=1.19 -- this is for a W[2,1] ***
 ! write(*,'(''k1,k2='',2i3,'' cmin,omega='',2(f7.4,1x))') k1,k2,cmin,omega
#endMacro

      subroutine lineSmoothUpdate( nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &     u, defect, mask, ndc, c, ipar, rpar )
! ===================================================================================
!  Line smooth: update the solution u
!
! Update u: 
!     u(i1,i2,i3)=(1-omega)m*u(i1,i2,i3)+omega*defect(i1,i2,i3) 
!  
! ===================================================================================

      implicit none
      integer nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,ndc

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer ipar(0:*)

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real defect(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real c(1:ndc,nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real rpar(0:*)

!................ local
      integer orderOfAccuracy,i1,i2,i3,variableCoefficients,direction,useOmega
      integer n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c
      integer m1a,m1b,m2a,m2b,width,wBase,k1,k1Old
      integer j1,j2,j3,shift3

      real omega,omegam,variableOmegaFactor,c1,c2,cmin,cmax

!............. start statement function
      integer m123
      m123(i1,i2,i3)=i1-wBase+width*(i2-wBase+width*(i3-wBase))+1  ! index into 3d stencil, 
!............. end statement function

      n1a                 =ipar(0)
      n1b                 =ipar(1)
      n1c                 =ipar(2)
      n2a                 =ipar(3)
      n2b                 =ipar(4)
      n2c                 =ipar(5)
      n3a                 =ipar(6)
      n3b                 =ipar(7)
      n3c                 =ipar(8)
      direction           =ipar(9)
      orderOfAccuracy     =ipar(10)
      variableCoefficients=ipar(11)
      useOmega            =ipar(12)

      omega               =rpar(0)
      variableOmegaFactor =rpar(1)

      if( orderOfAccuracy.eq.2 )then
        width=3
        wBase=-1
      else
        width=5
        wBase=-2
      end if
      
      ! write(*,'("lineSmoothUpdate: direction,variableCoefficients,useOmega=",3i3," omega=",f6.3)') direction,variableCoefficients,useOmega,omega

      if( useOmega.eq.0 )then
        
        beginLoop()
	  if( mask(i1,i2,i3).ge.0 )then
	    u(i1,i2,i3)=defect(i1,i2,i3)
          end if
        endLoop()

      else if( useOmega.eq.1 .or. variableCoefficients.eq.0 )then
        ! use constant omega or constant coefficients -> use constant omega
        omegam=1.-omega
        beginLoop()
	  if( mask(i1,i2,i3).ge.0 )then
	    u(i1,i2,i3)=omegam*u(i1,i2,i3)+omega*defect(i1,i2,i3)
          end if
        endLoop()

      else 
        if( variableCoefficients.ne.1 )then
          write(*,'("ERROR: variableCoefficients.ne.1")')
          stop 123
        end if

        ! use variable omega and variable coeff -> use variable omega
    
        ! (m1a,m1b) and (m2a,m2b) : use these indicies to determine the size of the coefficients
        ! in the two tangential directions to direction -- used in the calculation of omega
        if( direction.eq.0 )then
          m1a=m123( 0,-1, 0)  
          m1b=m123( 0, 1, 0)
          m2a=m123( 0, 0,-1)
          m2b=m123( 0, 0, 1)
        else if( direction.eq.1 )then
          m1a=m123(-1, 0, 0)
          m1b=m123( 1, 0, 0)
          m2a=m123( 0, 0,-1)
          m2b=m123( 0, 0, 1)
        else if( direction.eq.2 )then
          m1a=m123(-1, 0, 0)
          m1b=m123( 1, 0, 0)
          m2a=m123( 0,-1, 0)
          m2b=m123( 0, 1, 0)
        else
          stop 55
        end if

        ! write(*,'(" direction=",i1,", ndc=",i3,", m1a,m1b,m2a,m2b=",6i3)') direction,ndc,m1a,m1b,m2a,m2b

        if( nd.eq.2 )then
          ! --- 2D ---
          shift3=0
          if( orderOfAccuracy.eq.2 )then
            do i3=n3a,n3b,n3c
              j3=max(n3a+shift3,min(n3b-shift3,i3)) ! avoid boundaries for omega -- coefficients could be zero (BC)
            do i2=n2a,n2b,n2c
              j2=max(n2a+1,min(n2b-1,i2)) 
            do i1=n1a,n1b,n1c
  	    if( mask(i1,i2,i3).ge.0 )then
                j1=max(n1a+1,min(n1b-1,i1)) 
                computeOmega2d(j1,j2,j3)
  	      u(i1,i2,i3)=(1.-omega)*u(i1,i2,i3)+omega*defect(i1,i2,i3)
              end if
            endLoop()
          else
            do i3=n3a,n3b,n3c
              j3=max(n3a+shift3,min(n3b-shift3,i3)) ! avoid boundaries for omega -- coefficients could be zero (BC)
            do i2=n2a,n2b,n2c
              j2=max(n2a+2,min(n2b-2,i2)) 
            do i1=n1a,n1b,n1c
  	    if( mask(i1,i2,i3).ge.0 )then
                j1=max(n1a+2,min(n1b-2,i1)) 
                computeOmega2dFourthOrder(j1,j2,j3)
  	      u(i1,i2,i3)=(1.-omega)*u(i1,i2,i3)+omega*defect(i1,i2,i3)
              end if
            endLoop()
          end if

        else 
          ! --- 3D ---
          shift3=orderOfAccuracy/2

          if( orderOfAccuracy.eq.2 )then
            do i3=n3a,n3b,n3c
              j3=max(n3a+shift3,min(n3b-shift3,i3)) ! avoid boundaries for omega -- coefficients could be zero (BC)
            do i2=n2a,n2b,n2c
              j2=max(n2a+1,min(n2b-1,i2)) 
            do i1=n1a,n1b,n1c
  	    if( mask(i1,i2,i3).ge.0 )then
                j1=max(n1a+1,min(n1b-1,i1)) 
                computeOmega3d(j1,j2,j3)
  	      u(i1,i2,i3)=(1.-omega)*u(i1,i2,i3)+omega*defect(i1,i2,i3)
              end if
            endLoop()
          else
            do i3=n3a,n3b,n3c
              j3=max(n3a+shift3,min(n3b-shift3,i3)) ! avoid boundaries for omega -- coefficients could be zero (BC)
            do i2=n2a,n2b,n2c
              j2=max(n2a+2,min(n2b-2,i2)) 
            do i1=n1a,n1b,n1c
  	    if( mask(i1,i2,i3).ge.0 )then
                j1=max(n1a+2,min(n1b-2,i1)) 
                computeOmega3dFourthOrder(j1,j2,j3)
  	      u(i1,i2,i3)=(1.-omega)*u(i1,i2,i3)+omega*defect(i1,i2,i3)
              end if
            endLoop()
          end if

        end if

      end if

      return 
      end
