! This file automatically generated from bcOptMaxwell4.bf with bpp.
! *** Fourth order boundary conditions for Maxwell ****


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

!**************************************************************************

! Include macros that are common to different orders of accuracy

!         -*- mode: F90 -*-
! Macros that are common to different orders of accuracy



! This version can optionally eval time-derivative:

! This version can optionally eval time-derivative:




! use the mask 





! Tangent vectors (un-normalized)
        









! ======== WARNING: These next derivatives are really R and S derivatives ===============






! ***************************************************************************************************
! *************************  here are versions for 3d **********************************************
! ***************************************************************************************************




! Here are versions that use a precomputed jacobian
                                              
           


















! ***************************************************************************************************













! These next approximations are from diff.maple







! ========================================================================================


! **note** this next one only works in 2D


! **note** this next one only works in 2D



! These next two came from ov/bpp/test11.bf
! **note** THESE ARE WRONG ****


























!=====================================================================================
! Boundary conditions for a rectangular grid:
!   Normal component of E is even symmetry
!   Tangential components of E are odd symmetry
! In 2d: normal component of Hz is even symmetry (Neumann BC)
!
! DIM: 2,3
! ORDER: 2,4,6,8
! FORCING: none,twilightZone
!=====================================================================================


! ************************************************************************************************
!  This macro is used for looping over the faces of a grid to assign boundary conditions
!
! extra: extra points to assign
!          Case 1: extra=numberOfGhostPoints -- for assigning extended boundaries
!          Case 2: extra=-1 -- for assigning ghost points but not including extended boundaries
! numberOfGhostPoints : number of ghost points (1 for 2nd order, 2 for fourth-order ...)
! ***********************************************************************************************







!**************************************************************************

! Here are macros that define the planeWave solution
c **************************************************
c Here are macros that define the:
c      planeWave solution 
c **************************************************

c ======================================================================
c  Slow start function 
c    tba = length of slow start interval (<0 mean no slow start)
c ======================================================================

c cubic ramp
c tba=max(REAL_EPSILON,tb-ta);
c dta=t-ta;
      
c This (cubic) ramp has 1-derivative zero at t=0 and t=tba

c This ramp has 3-derivatives zero at t=0 and t=1
c This is from ramp.maple
c r=-84*t**5+35*t**4-20*t**7+70*t**6
c rt=-420*t**4+140*t**3-140*t**6+420*t**5
c rtt=-1680*t**3+420*t**2-840*t**5+2100*t**4
c rttt=-5040*t**2+840*t-4200*t**4+8400*t**3


c This ramp has 4-derivatives zero at t=0 and t=1
c This is from ramp.maple
c r=126*(t)**5-315*(t)**8+70*(t)**9-420*(t)**6+540*(t)**7
c rt=630*(t)**4-2520*(t)**7+630*(t)**8-2520*(t)**5+3780*(t)**6
c rtt=2520*(t)**3-17640*(t)**6+5040*(t)**7-12600*(t)**4+22680*(t)**5
c rttt=7560*(t)**2-105840*(t)**5+35280*(t)**6-50400*(t)**3+113400*(t)**4


c ============================================================
c  Initialize parameters for the boundary forcing
c   tba: slow start time interval -- no slow start if this is negative
c ===========================================================

c **************** Here is the new generic plane wave solution *******************

! component n=ex,ey,ez, hx,hy,hz (assumes ex=0)
! one time derivative:
! two time derivatives:
! three time derivatives:

c *************** Here is the 2D planeWave solution ******************************


c one time derivative:

c two time derivatives:

c three time derivatives:

c four time derivatives:

c Here are the slow start versions

c one time derivative:

c two time derivatives:

c three time derivatives:

c four time derivatives:


c **************** Here is the 3D planeWave solution ***************************************



c one time derivative:


c two time derivatives:


c three time derivatives:


c four time derivatives:


c Here are the slow start versions


c one time derivative:


c two time derivatives:

c three time derivatives:

c four time derivatives:


c Helper function: Return minus the second time derivative



!===================================================================================
!  Put the inner loop for the 4th-order BC here so we can repeat it for testing 
!==================================================================================
! #beginMacro bcCurv2dOrder4InnerLoop()
! #endMacro

! This macro is for the BC on Hz in 2D

! -------------------------------------------------------------------------------------------------------
! Macro: fifth-order extrapolation:
! -------------------------------------------------------------------------------------------------------


! ===================================================================================
!  BCs for curvilinear grids in 2D
!
!  FORCING: none, twilightZone
! ===================================================================================



! ==========================================================================
!  Define some metric (and equation coefficients) terms and their derivatives
!
!  DAr4, DArr4, ... normal derivative
! ==========================================================================

! ==========================================================================
!  Define some metric (and equation coefficients) terms and their derivatives
!
! Here are the derivatives we can compute directly based on (is1,is2,i3) (js1,js2,js3) (ks1,ks2,k3)
! ==========================================================================

! ==========================================================================
!  Define some metric (and equation coefficients) terms and their derivatives
!
! Here are the derivatives that we need to use difference code for each values of axis
! ==========================================================================



!================================================================================
! Compute tangential derivatives
!================================================================================

!================================================================================
! Compute tangential derivatives
!
! Here are the derivatives we can compute directly based on (is1,is2,i3) (js1,js2,js3) (ks1,ks2,k3)
!================================================================================

! ======================================================================================
! Here are the derivatives that we need to use difference code for each values of axis
! ======================================================================================


! ==========================================================================
!  Define some metric (and equation coefficients) terms and their derivatives
!  **** for the extrapolation case ***
! Here are the derivatives we can compute directly based on (is1,is2,i3) (js1,js2,js3) (ks1,ks2,k3)
! ==========================================================================

!================================================================================
! Compute tangential derivatives for extrapolation case
!
! Here are the derivatives we can compute directly based on (is1,is2,i3) (js1,js2,js3) (ks1,ks2,k3)
!================================================================================

! 2nd-order One-sided approximations:


!=============================================================================================
!  BCs for curvilinear grids in 3D
!
! Note:
!   The equations are generated assuming that r is the normal direction.
!   We need to permute the (r,s,t) derivatives according to the value of axis.
!      axis=0: (r,s,t)
!      axis=1: (s,t,r)
!      axis=2: (t,r,s)
!
!  FORCING: none, twilightZone
!=============================================================================================

! ***** Step 1 : assign values using extrapolation of the normal component ***






! **************************************************************
! *****************   Correction Step **************************
! **************************************************************







