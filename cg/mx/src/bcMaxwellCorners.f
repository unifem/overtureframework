! This file automatically generated from bcMaxwellCorners.bf with bpp.
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



! -------------------------------------------------------------------------------------------------------
! Macro: third-order extrapolation:
!    (j1,j2,j3)    : point to extrapolate
!    (is1,is2,is3) : direction to extrapolate
! -------------------------------------------------------------------------------------------------------

! -------------------------------------------------------------------------------------------------------
! Macro: fifth-order extrapolation:
!    (j1,j2,j3)    : point to extrapolate
!    (is1,is2,is3) : direction to extrapolate
! -------------------------------------------------------------------------------------------------------

! -------------------------------------------------------------------------------------------------------
!  Macro: 3rd-order extrapolation
! -------------------------------------------------------------------------------------------------------

! -------------------------------------------------------------------------------------------------------
!  Macro: 5th-order extrapolation
! -------------------------------------------------------------------------------------------------------


! ===============================================================================
!  Set the tangential component to zero on the boundary in 2D
! ===============================================================================

! ===============================================================================
!  Set the tangential component to zero on the boundary in 3D
! ===============================================================================


! This formula is just Taylor series: (for odd functions)

! This formula is just Taylor series: (for odd functions)


! This formula is also Taylor series (for even functions)

! ===================================================================================
! Determine the values at ghost points outside corners
!  
!
! GRIDTYPE: curvilinear, rectangular
! The formula for urs,vrs are from bc4v.maple
! ===================================================================================



! =================================================================================
!  Assign extended boundary points (from bc4c.maple)
!
!  Solve for the 8 unknowns
!         u(i1-1,i2,i3,ex), u(i1-2,i2,i3,ex), u(i1,i2-1,i3,ex), u(i1,i2-2,ex)
!         u(i1-1,i2,i3,ey), u(i1-2,i2,i3,ey), u(i1,i2-1,i3,ey), u(i1,i2-2,ey)
!   
! Use:
!   1) tangential components are zero (4 equations)
!   2) Use the equations on the corner  (2 equations)
!   3) Extrapolate normal components at (i1-2) and (i2-2)  (2 equations)
!
! NOTE: Call this macro with axis=0 and axisp1=1 for all sides and dra=dr(0)*is1, dsa=dr(1)*is2
! =================================================================================


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
!
! Here are the derivatives we can compute directly based on (is1,is2,i3) (js1,js2,js3) (ks1,ks2,k3)
!================================================================================

! ======================================================================================
! Here are the derivatives that we need to use difference code for each values of axis
! ======================================================================================


! Taylor series (designed for odd functions)




! *********************************************************************************
! ****************Assign Ghost Points Outside of Edges*****************************
!
!   GRIDTYPE: rectangular, curvilinear
!   ORDER: 2, 4, 6, ..
! *********************************************************************************







! ======================================================================================
!   Assign edges and corner points next to edges in 3D
!
!  Set the normal component of the solution on the extended boundaries (points N in figure)
!      Note: the corner ghost points "C" are set in assignEdgeCorners, called below
!              |
!              X
!              |
!        N--N--X--X----
!              |
!        C  C  N
!              |
!        C  C  N
!
! =================================================================================

! =================================================================================================
! 4th-order Taylor approximation for 3D corners -- the truncation looks like dr^4*u_rrrr
! =================================================================================================

! finish this 
! #defineMacro taylorOdd3dOrder6(cc,ks1,ks2,ks3,dr1,dr2,dr3,urr,uss,utt,urs,urt,ust) !    2.*u(i1,i2,i3,cc)-u(i1+ks1,i2+ks2,i3+ks3,cc) !       + ( (dr1)**2*urr+(dr2)**2*uss+(dr3)**2*utt+2.*(dr1)*(dr2)*urs+2.*(dr1)*(dr3)*urt+2.*(dr2)*(dr3)*ust )
!        + (1./12.)*( (dr1)**4*urrrr + (dr2)**4*ussss + (dr3)**4*utttt !                     + 4.*(dr1)**3*(dr2)*urrrs + 4.*(dr1)**3*(dr3)*urrrt + 4.*(dr1)*(dr2)**3*ursss
!                     + 4.*(dr1)*(dr3)**3*urttt + 4.*(dr2)*(dr3)**3*usttt + 4.*(dr2)**3*(dr3)*ussst !                     + 6.*(dr1)**2*(dr2)**2*urrss + 6.*(dr1)**2*(dr3)**2*urrtt + 6.*(drs)**2*(dr3)**2*usstt !    *check this*   + 12.*(dr1)**2*(dr2)*(dr3)*urrst + 12.*(dr1)*(dr2)**2*(dr3)*ursst + 12.*(dr1)*(dr2)*(dr3)**2*urstt
                                      )

!*****************************************************************************************************
!   Assign corners and edges in 3D
! 
!  ORDER: 2,4,6,8
!  GRIDTYPE: 
!  FORCING:
! 
! NOTE: tangential components have already been assigned on the extended boundary by assignBoundary3d
!*****************************************************************************************************


! =================================================================================
!   Assign values in the corners in 2D
!
!  Set the normal component of the solution on the extended boundaries (points N in figure)
!  Set the corner points "C" -- odd symmetry about the corner
!              |
!              X
!              |
!        N--N--X--X----
!              |
!        C  C  N
!              |
!        C  C  N
!
! =================================================================================





! ************************************************************************************
!  NAME : name of the subroutine
!  ORDER : order of accuracy
! ************************************************************************************



