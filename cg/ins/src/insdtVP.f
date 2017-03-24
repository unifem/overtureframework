! This file automatically generated from insdtVP.bf with bpp.
c ==============================================================================
c  Incompressible NS Visco-Plastic explicit discretization
c ==============================================================================

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





c =============================================================================
c Evaluate the coefficients of the visco-plastic model
c ============================================================================

! ----------- FINISH ME : upwind advection ------

c====================================================================
c This macro will build the statements that form the body of the loop
c
c IMPEXP: EXPLICIT, EXPLICIT_ONLY, BOTH
c SCALAR: NONE
c         PASSIVE - include equations for a passive scalar
c AXISYMMETRIC : YES or NO
c====================================================================


      ! Visco-plastic case
