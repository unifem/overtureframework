! This file automatically generated from bcOptMaxwell4.bf with bpp.
! *****************************************************
! *** Fourth order boundary conditions for Maxwell ****
! *****************************************************


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






! ----------------------------------------------------------------------------------
! Macro - loop over boundary points including extended boundary
!   extra : extend the boundary by this many point 
! ----------------------------------------------------------------------------------


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

























! --------------------------------------------------------------------
! Macro Evaluate the boundary forcing 2D
! 
!  x,y,t (input) : point to evaluate at 
!  numberOfTimeDerivatives : evaluate this time derivative
!  ubc(.)  (output) : ubc(ex), etc. 
! --------------------------------------------------------------------

! --------------------------------------------------------------------
! Macro Evaluate the boundary forcing 3D
! 
!  x,y,z,t (input) : point to evaluate at 
!  numberOfTimeDerivatives : evaluate this time derivative
!  ubc(.)  (output) : ubc(ex), etc. 
! --------------------------------------------------------------------



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
! -*- mode: f90; -*-

! **************************************************
! Here are macros that define the:
!      planeWave solution 
! **************************************************

! ======================================================================
!  Slow start function 
!    tba = length of slow start interval (<0 mean no slow start)
! ======================================================================

! cubic ramp
! tba=max(REAL_EPSILON,tb-ta);
! dta=t-ta;
      
! This (cubic) ramp has 1-derivative zero at t=0 and t=tba

! This ramp has 3-derivatives zero at t=0 and t=1
! This is from ramp.maple
! r=-84*t**5+35*t**4-20*t**7+70*t**6
! rt=-420*t**4+140*t**3-140*t**6+420*t**5
! rtt=-1680*t**3+420*t**2-840*t**5+2100*t**4
! rttt=-5040*t**2+840*t-4200*t**4+8400*t**3


! This ramp has 4-derivatives zero at t=0 and t=1
! This is from ramp.maple
! r=126*(t)**5-315*(t)**8+70*(t)**9-420*(t)**6+540*(t)**7
! rt=630*(t)**4-2520*(t)**7+630*(t)**8-2520*(t)**5+3780*(t)**6
! rtt=2520*(t)**3-17640*(t)**6+5040*(t)**7-12600*(t)**4+22680*(t)**5
! rttt=7560*(t)**2-105840*(t)**5+35280*(t)**6-50400*(t)**3+113400*(t)**4


! ============================================================
!  Initialize parameters for the boundary forcing
!   tba: slow start time interval -- no slow start if this is negative
! ===========================================================

! **************** Here is the new generic plane wave solution *******************

! component n=ex,ey,ez, hx,hy,hz (assumes ex=0)
! one time derivative:
! two time derivatives:
! three time derivatives:

! *************** Here is the 2D planeWave solution ******************************


! one time derivative:

! two time derivatives:

! three time derivatives:

! four time derivatives:

! Here are the slow start versions

! one time derivative:

! two time derivatives:

! three time derivatives:

! four time derivatives:


! **************** Here is the 3D planeWave solution ***************************************



! one time derivative:


! two time derivatives:


! three time derivatives:


! four time derivatives:


! Here are the slow start versions


! one time derivative:


! two time derivatives:

! three time derivatives:

! four time derivatives:


! -------------------------------------------------------------------
! Helper function: Return minus the second time derivative
! -------------------------------------------------------------------


! --------------------------------------------------------------------
! Evaluate the plane wave in 2D
! 
!  x,y,t (input) : point to evaluate at 
!  numberOfTimeDerivatives : evaluate this time derivative
!  ubc(.)  (output) : ubc(ex), etc. 
! --------------------------------------------------------------------


! --------------------------------------------------------------------
! Evaluate the plane wave in 3D
! 
!  x,y,z,t (input) : point to evaluate at 
!  numberOfTimeDerivatives : evaluate this time derivative
!  ubc(.)  (output) : ubc(ex), etc. 
! --------------------------------------------------------------------

! ----- Here are macros for the chirped-plane wave -----
! -*- mode: f90; -*-


! ==============================================================================================
! ======================  Macros for the chirped plane wave  ===================================
! ==============================================================================================

! ----------------------------------------------------------------------------------
! Macro: Utility function used in computing zero or more derivatives of the chirp 
! ---------------------------------------------------------------------------------
!- #beginMacro getChirp( chirp )
!-  ! phi = phase
!-  phi = cc*xi + cpwAlpha*xi**2
!- 
!-  ! amplitude: approx. equal to 1 over the interval [ta,tb]
!-  tanha = tanh(cpwBeta*(t-cpwTa)) 
!-  tanhb = tanh(cpwBeta*(t-cpwTb)) 
!-  amp = cpwAmp*.5*( tanha - tanhb )
!- 
!-  if( numberOfTimeDerivatives.eq.0 )then
!- 
!-    ! -- chirp function --
!-    chirp = amp*sin(twoPi*phi)
!- 
!-  else if(  numberOfTimeDerivatives.eq.1 )then
!-    ! get 1st time derivative  *check me*
!- 
!-    ! d(phi)/dt:  (plus a factor of twoPi)
!-    phip = twoPi*(cc+2.*cpwAlpha*xi) 
!-    ! d(amp)/dt: 
!-    tanhap = cpwBeta*(1.-tanha**2) 
!-    tanhbp = cpwBeta*(1.-tanhb**2) 
!-    ampp =  cpwAmp*.5*( tanhap - tanhbp )
!- 
!-    ! -- d(chirp)/dt --
!-    chirp = amp*phip*cos(twoPi*phi) + ampp*sin(twoPi*phi)
!- 
!-  else if(  numberOfTimeDerivatives.eq.2 )then
!-    ! get 2nd time derivative  *check me*
!- 
!-    ! d(phi)/dt: 
!-    phip = twoPi*(cc+2.*cpwAlpha*xi) 
!-    ! d^2(phi)/dt^2 : 
!-    phipp = twoPi*(2.*cpwAlpha) 
!- 
!-    ! d(amp)/dt: 
!-    tanhap = cpwBeta*(1.-tanha**2) 
!-    tanhbp = cpwBeta*(1.-tanhb**2) 
!-    ampp =  cpwAmp*.5*( tanhap - tanhbp )
!-   
!-    ! d^2(amp)/dt^2: 
!-    tanhapp = -2.*(cpwBeta**2)*tanha*tanhap
!-    tanhbpp = -2.*(cpwBeta**2)*tanhb*tanhbp
!-    amppp =  cpwAmp*.5*( tanhapp - tanhbpp )
!- 
!-    sinp=sin(twoPi*phi)
!-    cosp=cos(twoPi*phi)
!- 
!-    ! -- d^2(chirp)/dt^2 --
!-    chirp = amp*( -(phip**2)*sinp + phipp*cosp ) !-          + 2.*ampp*phip*cosp + amppp*sinp
!- 
!-  else
!-    write(*,'(" getChirp:ERROR: too many derivatives requested")')
!-    stop 4927
!-  end if
!- #endMacro 

! --------------------------------------------------------------------
! Evaluate the chirped plane wave in 2D
! 
!  x,y,t (input) : point to evaluate at 
!  numberOfTimeDerivatives : evaluate this time derivative
!  ubc(.)  (output) : ubc(ex), etc. 
! --------------------------------------------------------------------

! --------------------------------------------------------------------
! Evaluate the chirped plane wave in 3D
! 
!  x,y,z,t (input) : point to evaluate at 
!  numberOfTimeDerivatives : evaluate this time derivative
!  ubc(.)  (output) : ubc(ex), etc. 
! --------------------------------------------------------------------




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







