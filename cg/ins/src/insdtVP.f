! This file automatically generated from insdtVP.bf with bpp.
c ==============================================================================
c  Incompressible NS Visco-Plastic explicit discretization
c ==============================================================================

c ===============================================================================
c This file is included by
c     insdtINS.bf 
c     insdtKE.bf
c     insdtSPAL.bf
c     insdtVP.bf
c ==============================================================================

c These next include files will define the macros that will define the difference approximations
c The actual macro is called below
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










c Define the artificial diffusion coefficients
c gt should be R or C (gridType is Rectangular or Curvilinear)
c tb should be blank or SA  (SA=Spalart-Allamras turbulence model)

c Define macros for the derivatives based on the dimension, order of accuracy and grid-type


c =============================================================
c Compute derivatives of u,v,w 
c =============================================================





c***************************************************************
c  Define the equations for EXPLICIT time stepping
c
c ORDER: 2,4
c DIM: 2,3
c GRIDTYPE: rectangular, curvilinear
c
c***************************************************************



c====================================================================================
c
c====================================================================================


c======================================================================================
c Define the subroutine to compute du/dt
c
c======================================================================================

c 
c : empty version for linking when we do not want an option
c





c =============================================================================
c Evaluate the coefficients of the visco-plastic model
c ============================================================================


c====================================================================
c This macro will build the statements that form the body of the loop
c
c IMPEXP: EXPLICIT, EXPLICIT_ONLY, BOTH
c SCALAR: NONE
c         PASSIVE - include equations for a passive scalar
c AXISYMMETRIC : YES or NO
c====================================================================


      ! Visco-plastic case
