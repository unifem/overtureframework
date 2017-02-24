! This file automatically generated from insdtINS.bf with bpp.
! ===========================================================================
!   Incompressible Navier-Stokes : explicit discretization 
! ===========================================================================


!  -*- mode: F90 -*-
! ===============================================================================
! This file is included by
!     insdtINS.bf 
!     insdtKE.bf
!     insdtSPAL.bf
!     insdtVP.bf
! ==============================================================================

! These next include files will define the macros that will define the difference approximations
! The actual macro is called below
c Use this next macro to declare the statement functions that are defined below
c To include derivatives of rx use OPTION=RX


c Define statement functions for difference approximations of order 2 
c To include derivatives of rx use OPTION=RX
c To include derivatives of rx use OPTION=RX



c Use this next macro to declare the statement functions that are defined below
c To include derivatives of rx use OPTION=RX


c Define statement functions for difference approximations of order 4 
c To include derivatives of rx use OPTION=RX
c To include derivatives of rx use OPTION=RX

c **********************************************************************
c  This file contains some commonly used macros.
c **********************************************************************


c Define macros for the derivatives based on the dimension, order of accuracy and grid-type


c================================================================
c Input:
c ADTYPE: AD2, AD4, AD24
c TURBULENCE_MODEL: INS, SPAL
c Output:
c  artificialDissipation(cc) : inline macro for u,v,w
c  artificialDissipationTM(cc) : inline macro for "n" or k or epsilon
c================================================================


c================================================================
c Input:
c ADTYPE: AD2, AD4, AD24
c TURBULENCE_MODEL: INS, SPAL
c Output:
c  artificialDissipation(cc) : inline macro for u,v,w
c  artificialDissipationTM(cc) : inline macro for "n" or k or epsilon
c================================================================










! Define the artificial diffusion coefficients
! gt should be R or C (gridType is Rectangular or Curvilinear)
! tb should be blank or SA  (SA=Spalart-Allamras turbulence model)

! Define macros for the derivatives based on the dimension, order of accuracy and grid-type


! =============================================================
! Compute derivatives of u,v,w 
! =============================================================





!***************************************************************
!  Define the equations for EXPLICIT time stepping
!
! ORDER: 2,4
! DIM: 2,3
! GRIDTYPE: rectangular, curvilinear
!
!***************************************************************



!====================================================================================
!
!====================================================================================


!======================================================================================
! Define the subroutine to compute du/dt
!
!======================================================================================

! 
! : empty version for linking when we do not want an option
!





!  -*- mode: F90 -*-
! Define macros to evaluate an advection term (a.grad) u
!



! --------------------------------------------------------------------------------
!  Macro: getUpwindAdvection
! 
! --------------------------------------------------------------------------------


! --------------------------------------------------------------------------------
!  Macro: getBwenoAdvection
! --------------------------------------------------------------------------------


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

!====================================================================
! This macro will build the statements that form the body of the loop
!
! IMPEXP: EXPLICIT, EXPLICIT_ONLY, BOTH
! SCALAR: NONE
!         PASSIVE - include equations for a passive scalar
! AXISYMMETRIC : YES or NO
! UPWIND : CENTERED, UPWIND or BWENO
!====================================================================

 !====================================================================
 ! *OLD*
 ! This macro will build the statements that form the body of the loop
 !
 ! IMPEXP: EXPLICIT, EXPLICIT_ONLY, BOTH
 ! SCALAR: NONE
 !         PASSIVE - include equations for a passive scalar
 ! AXISYMMETRIC : YES or NO
 !====================================================================



