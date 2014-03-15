! This file automatically generated from insLineSolveNew.bf with bpp.
c***********************************************************************************************
c 
c   *NEW* Steady-state line-solver routines for the incompressible NS plus some turbulence models
c
c***********************************************************************************************

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



! define the INITIALZE macro: 
c Here are the statements we use to initialize the main subroutines below



c ** Include "defineSelfAdjointMacros.h"

c ************************ NEW WAY ************************

c These next include file will define the macros that will define the difference approximations (in op/src)
c Defines getDuDx2(u,aj,ff), getDuDxx2(u,aj,ff), getDuDx3(u,aj,ff), ...  etc. 
c #Include "derivMacroDefinitions.h"


c Define 
c    defineParametricDerivativeMacros(u,dr,DIM,ORDER,COMPONENTS,MAXDERIV)
c       defines -> ur2, us2, ux2, uy2, ...            (2D)
c                  ur3, us3, ut3, ux3, uy3, uz3, ...  (3D)
!* #Include "defineParametricDerivMacros.h"

! defineParametricDerivativeMacros(u,dr,DIM,ORDER,COMPONENTS,MAXDERIV)

!*  defineParametricDerivativeMacros(u,dr,3,2,1,2)
!*  defineParametricDerivativeMacros(rsxy,dr,3,2,2,1)

! Example to define orders 2,4,6: 
! defineParametricDerivativeMacros(u1,dr1,2,2,1,6)
! defineParametricDerivativeMacros(u1,dr1,2,4,1,4)
! defineParametricDerivativeMacros(u1,dr1,2,6,1,2)

! construct an include file that declares temporary variables:
!**#beginFile insLSdeclareTemporaryVariablesOrder2.h
!**      !  declareTemporaryVariables(DIM,MAXDERIV)   ! I think DIM and MAXDERIV are ignored for now
!**      declareTemporaryVariables(2,2)
!**      declareJacobianDerivativeVariables(aj,3)     ! declareJacobianDerivativeVariables(aj,DIM)
!**#endFile


! --- Macros to define INS and Temperature line-solve functions:---
!          fillEquationsRectangularGridINS
!          fillEquationsCurvilinearGridINS
! 
! --- Macros to define INS and Temperature line-solve functions:---
!          fillEquationsRectangularGridINS
!          fillEquationsCurvilinearGridINS
!          fillEquationsRectangularGridTemperature
!          fillEquationsCurvilinearGridTemperature
!          computeResidualINS

!     This file is included in insLineSolveNew.bf 


c ===================================================================================
c        INS: Incompressible Navier Stokes 
c  dir: 0,1,2
c====================================================================================


c ===================================================================================
c  ****** Incompressible NS, No turbulence model *****
c  dir: 0,1,2
c====================================================================================


c ===================================================================================
c        INS: Incompressible Navier Stokes Temperature Equation
c  dir: 0,1,2
c====================================================================================


c ===================================================================================
c  ****** Temperature Equation for INS *****
c  dir: 0,1,2
c====================================================================================



c==================================================================================
c  Residual Computation for INS: incompressible Navier Stokes (including Boussinesq)
c
c Macro args:
c  GRIDTYPE: rectangular, curvilinear
c====================================================================================

! --- Macros to define Visco Plastic line-solve functions:---
!     fillEquationsRectangularGridVP, computeResidualVP
c ************************************************************************
c   Visco-Plastic Model: 
c     Macros to define the equations for the line solver 
c
c  This file is included by insLineSolveNew.bf 
c
c In 2d the momentum equations are: 
c
c   u.t + u.grad(u) + p.x = Dx( 2*vp*u.x ) + Dy(   vp*u.y ) + Dy( vp*v.x )
c   v.t + u.grad(v) + p.y = Dx(   vp*v.x ) + Dy( 2*vp*v.y ) + Dx( vp*u.y )
c
c ************************************************************************

! The next include file defines conservative approximations to coefficent matrices
! ========== This include file contains conservative approximations to coefficient operators ================
!
!    ajac2d(i1,i2,i3)
!    ajac3d(i1,i2,i3
!    getCoeffForDxADxPlusDyBDy(au, azmz,amzz,azzz,apzz,azpz, bzmz,bmzz,bzzz,bpzz,bzpz )
!    getCoeffForDyADx( au, azmz,amzz,azzz,apzz,azpz )
!    getCoeffForDxADy( au, azmz,amzz,azzz,apzz,azpz )
!    setDivTensorGradCoeff2d(cmp,eqn,a11ph,a11mh,a22ph,a22mh,a12pzz,a12mzz,a21zpz,a21zmz)
!    scaleCoefficients( a11ph,a11mh,a22ph,a22mh,a12pzz,a12mzz,a21zpz,a21zmz )
!    getCoeffForDxADxPlusDyBDyPlusDzCDz(au, azzm,azmz,amzz,azzz,apzz,azpz,azzp,...)
!    getCoeffForDxADy3d(au, X, Y, azzm,azmz,amzz,azzz,apzz,azpz,azzp )


! =============================================================================================================
!  Declare variables used in compute the coefficients for a non-linear viscosity
! =============================================================================================================



! ==================================================================================================
! Define the coefficients in the conservative discretization of: 
!         L = Dx( a*Dx ) + Dy( b*Dy ) 
! 
!   L   = (1/J)*[ Dr( J*(rx,ry).(aDx,bDy)) + Ds( J*(sx,sy).(aDx,bDy)) ] 
!       = (1/J)*[ Dr( J*a*rx(rx*Dr + sx*Ds) + J*b*ry*(ry*Dr + sy*Ds)
!                +Ds( J*a*sx(rx*Dr + sx*Ds) + J*b*sy*(ry*Dr + sy*Ds) ] 
!       = (1/J)*[ Dr( a11 Dr) + Dr( a12 Ds) + Ds( a21 Dr) + Ds( a22 Ds) ]
! where 
!  a11 = J ( a rx^2 + b ry^2 )
!  a12 = J ( a rx*sx + b ry*sy )
!  a21 = a12 
!  a22 = J ( a sx^2 + b sy^2 )
!  a = a(i1,i2,i3), b=b(i1,i2,i3)
!
! Macro Arguments:
!   au : prefix of the computed coefficients
!   azmz,amzz,azzz,apzz,azpz : a(i1,i2-1,i2),a(i1-1,i2,i3),a(i1,i2,i3),a(i1+1,i2,i3),a(i1,i2+1,i3)
!   bzmz,bmzz,bzzz,bpzz,bzpz : b(i1,i2-1,i2),b(i1-1,i2,i3),b(i1,i2,i3),b(i1+1,i2,i3),b(i1,i2+1,i3)
! The following jacobian values should also be defined:
!    ajzmz,ajmzz,ajzzz,ajpzz,ajzpz : aj(i1,i2-1,i2),aj(i1-1,i2,i3),aj(i1,i2,i3),aj(i1+1,i2,i3),aj(i1,i2+1,i3)
! ==================================================================================================

! ==================================================================================================
! Define the coefficients in the conservative discretization of: 
!         L = Dy( a*Dx ) 
!  L = div( (0,aDx) ) 
!  L = (1/J)*[ Dr( J*(rx,ry).(0,aDx)) + Ds( J*(sx,sy).(0,aDx)) ]
!    = (1/J)*[ Dr( J*a*ry*(rx*Dr + sx*Ds)) + Ds( J*a*sy*(rx*Dr + sx*Ds) ) ]
!    = (1/J)*( Dr( a11 Dr) + Dr( a12 Ds) + Ds( a21 Dr) + Ds( a22 Ds) ) u 
! where
!  a11 = J ( a ry*rx )
!  a12 = J ( a ry*sx )
!  a21 = J ( a sy*rx )
!  a22 = J ( a sy*sx )
!  a = a(i1,i2,i3)
!
! Macro Arguments:
!   au : prefix of the computed coefficients
!   azmz,amzz,azzz,apzz,azpz : a(i1,i2-1,i2),a(i1-1,i2,i3),a(i1,i2,i3),a(i1+1,i2,i3),a(i1,i2+1,i3)
! The following jacobian values should also be defined:
!    ajzmz,ajmzz,ajzzz,ajpzz,ajzpz : aj(i1,i2-1,i2),aj(i1-1,i2,i3),aj(i1,i2,i3),aj(i1+1,i2,i3),aj(i1,i2+1,i3)
! ==================================================================================================

! ==================================================================================================
! Define the coefficients in the conservative discretization of: 
!         L = Dx( a*Dy ) 
!
!  L = div( (aDy,0) ) 
!  L = (1/J)*[ Dr( J*(rx,ry).(aDy,0)) + Ds(J*(sx,sy).(aDy,0)) ]
!    = (1/J)*[ Dr( J*a*rx*(ry*Dr + sy*Ds)) + Ds( J*a*sx*(ry*Dr + sy*Ds) ) ]
!    = (1/J)*[ Dr( a11 Dr) + Dr( a12 Ds) + Ds( a21 Dr) + Ds( a22 Ds) ]
! where
!  a11 = J ( a rx*ry )
!  a12 = J ( a rx*sy )
!  a21 = J ( a sx*ry )
!  a22 = J ( a sx*sy )
! Macro Arguments:
!   au : prefix of the computed coefficients
!   azmz,amzz,azzz,apzz,azpz : a(i1,i2-1,i2),a(i1-1,i2,i3),a(i1,i2,i3),a(i1+1,i2,i3),a(i1,i2+1,i3)
! The following jacobian values should also be defined:
!    ajzmz,ajmzz,ajzzz,ajpzz,ajzpz : aj(i1,i2-1,i2),aj(i1-1,i2,i3),aj(i1,i2,i3),aj(i1+1,i2,i3),aj(i1,i2+1,i3)
! ==================================================================================================


! =============================================================================================================
! Assign the coefficients for a component of the conservative discretization of the div(tensor grad) operator
! =============================================================================================================


! =======================================================================================================
! This macro scaled the coefficients that appear in the discretization of the div(tensor grad) operator
! =======================================================================================================



! ****************************************************************
! ****************** THREE DIMENSIONS ****************************
! ****************************************************************


! ==================================================================================================
! Define the coefficients in the conservative discretization of: 
!         L = Dx( a*Dx ) + Dy( b*Dy ) + Dz( c*Dz )
! 
!   L   = (1/J)*[ Dr( J*(rx,ry,rz).(aDx,bDy,cDz)) + Ds( J*(sx,sy,sz).(aDx,bDy,cDz)) + Dt( J*(tx,ty,tz).(aDx,bDy,cDz)) ] 
!       = (1/J)*[ Dr( J*a*rx(rx*Dr + sx*Ds+ tx*Dt) + J*b*ry*(ry*Dr + sy*Ds+ ty*Dt) + J*c*rz*(rz*Dr + sz*Ds+ tz*Dt)
!                +Ds( J*a*sx(rx*Dr + sx*Ds+ tx*Dt) + J*b*sy*(ry*Dr + sy*Ds+ ty*Dt) + J*c*sz*(rz*Dr + sz*Ds+ tz*Dt)
!                +Dt( J*a*tx(rx*Dr + sx*Ds+ tx*Dt) + J*b*ty*(ry*Dr + sy*Ds+ ty*Dt) + J*c*tz*(rz*Dr + sz*Ds+ tz*Dt)
!       = (1/J)*[ Dr( a11 Dr) + Dr( a12 Ds) + Dr( a13 Dt) + Ds( a21 Dr) + Ds( a22 Ds) + Ds( a23 Dt) + Dt( a31 Dr) + Dt( a32 Ds) + Dt( a33 Dt) ]
! where 
!  a11 = J ( a rx^2 + b ry^2 + c rz^2 )
!  a12 = J ( a rx*sx + b ry*sy + b rz*sz )
!  a13 = J ( a rx*tx + b ry*ty + b rz*tz )
!  a21 = a12 
!  a22 = J ( a sx^2 + b sy^2 + c sz^2 )
!  a23 = J ( a sx*tx + b sy*ty + b sz*tz )
!  a31 = a13
!  a32 = a23 
!  a33 = J ( a tx^2 + b ty^2 + c tz^2 )
!  a = a(i1,i2,i3), b=b(i1,i2,i3)
!
! Macro Arguments:
!   au : prefix of the computed coefficients
!   azzm,azmz,amzz,azzz,apzz,azpz,azzp : a(i1,i2,i3-1),a(i1,i2-1,i3),a(i1-1,i2,i3),a(i1,i2,i3),a(i1+1,i2,i3),a(i1,i2+1,i3),a(i1,i2,i3+1)
!   bzzm,bzmz,bmzz,bzzz,bpzz,bzpz,bzzp : b(i1,i2,i3-1),b(i1,i2-1,i3),b(i1-1,i2,i3),b(i1,i2,i3),b(i1+1,i2,i3),b(i1,i2+1,i3),b(i1,i2,i3+1)
!   czzm,czmz,cmzz,czzz,cpzz,czpz,czzp : c(i1,i2,i3-1),c(i1,i2-1,i3),c(i1-1,i2,i3),c(i1,i2,i3),c(i1+1,i2,i3),c(i1,i2+1,i3),c(i1,i2,i3+1)
! The following jacobian values should also be defined:
!    ajzmz,ajzzm,ajmzz,ajzzz,ajpzz,ajzpz,ajzzp : aj(i1,i2,i3-1),aj(i1,i2-1,i3),aj(i1-1,i2,i3),aj(i1,i2,i3),aj(i1+1,i2,i3),aj(i1,i2+1,i3),aj(i1,i2,i3+1)
! ==================================================================================================

! ==========================================================================================================================================
! 
! Define the coefficients in the conservative discretization of any mixed or non-mixed derivative: 
! 
!     L = D_X( a*D_Y )  where X=x, y, or z and Y=x, y, or z
!
! Example: 
!  L = div( (aDy,0,0) ) 
!  L = (1/J)*[ Dr( J*(rx,ry,rx).(aDy,0,0)) + Ds(J*(sx,sy,sz).(aDy,0,0))+ Dt(J*(tx,ty,tz).(aDy,0,0))  ]
!    = (1/J)*[ Dr( J*a*rx(ry*Dr + sy*Ds+ ty*Dt) 
!             +Ds( J*a*sx(ry*Dr + sy*Ds+ ty*Dt)
!             +Dt( J*a*tx(ry*Dr + sy*Ds+ ty*Dt) ]
!    = (1/J)*[ Dr( a11 Dr) + Dr( a12 Ds) + Dr( a13 Dt) + Ds( a21 Dr) + Ds( a22 Ds) + Ds( a23 Dt) + Dt( a31 Dr) + Dt( a32 Ds) + Dt( a33 Dt) ]
! where 
!  a11 = J ( a rx*ry )
!  a12 = J ( a rx*sy )
!  a13 = J ( a rx*ty )
!  a21 = J ( a sx*ry )
!  a22 = J ( a sx*sy )
!  a23 = J ( a sx*ty )
!  a31 = J ( a tx*ry )
!  a32 = J ( a tx*sy )
!  a33 = J ( a tx*ty )
!  a = a(i1,i2,i3)
!
! Macro Arguments:
!   au : prefix of the computed coefficients
!   X,Y : X=[x,y,z] and Y=[x,y,z] to compute the coeffcients of D_X( a D_Y )
!   azzm,azmz,amzz,azzz,apzz,azpz,azzp : a(i1,i2,i3-1),a(i1,i2-1,i3),a(i1-1,i2,i3),a(i1,i2,i3),a(i1+1,i2,i3),a(i1,i2+1,i3),a(i1,i2,i3+1)
! The following jacobian values should also be defined:
!    ajzmz,ajzzm,ajmzz,ajzzz,ajpzz,ajzpz,ajzzp : aj(i1,i2,i3-1),aj(i1,i2-1,i3),aj(i1-1,i2,i3),aj(i1,i2,i3),aj(i1+1,i2,i3),aj(i1,i2+1,i3),aj(i1,i2,i3+1)
! ===========================================================================================================================================



! ================================================================================================================================================
!    Assign the coefficients for a component of the conservative 3D discretization of the div(tensor grad) operator
! 
!  L  = (1/J)*[ Dr( a11 Dr) + Dr( a12 Ds) + Dr( a13 Dt) + Ds( a21 Dr) + Ds( a22 Ds) + Ds( a23 Dt) + Dt( a31 Dr) + Dt( a32 Ds) + Dt( a33 Dt) ]
!     Dr( a11 Dr)u = a11pzz*( u(i1+1)-u(i1) ) - a11mzz*( u(i1)-u(i1-1))
!                  = a11pzz*u(i1+1) -(a11pzz+a11mzz)*u(i1) +a11mzz*u(i1-1) 
!     Dr( a13 Dt ) = D0r( a13 D0t ) = a13pzz*( u(i1+1,i2,i3+1)-u(i1+1,i2,i3-1)) - a13mzz*( u(i1-1,i2,i3+1)-u(i1-1,i2,i3-1) )
!     Ds( a23 Dt ) = D0s( a23 D0t ) = a23zpz*( u(i1,i2+1,i3+1)-u(i1,i2+1,i3-1)) - a23zmz*( u(i1,i2-1,i3+1)-u(i1,i2-1,i3-1) )
!
!     Dt( a31 Dr ) = D0t( a31 D0r ) = a31zzp*( u(i1+1,i2,i3+1)-u(i1-1,i2,i3+1)) - a31zzm*( u(i1+1,i2,i3-1)-u(i1-1,i2,i3-1) )
!     Dt( a32 Ds ) = D0t( a32 D0s ) = a32zzp*( u(i1,i2+1,i3+1)-u(i1,i2-1,i3+1)) - a32zzm*( u(i1,i2+1,i3-1)-u(i1,i2-1,i3-1) )
!
! Macro args:
!   cmp,eqn : component and equation
!   A : generic name for coefficients
! =================================================================================================================================================


! =======================================================================================================
! This macro scaled the coefficients that appear in the discretization of the div(tensor grad) operator
! =======================================================================================================


c ===================================================================================
c        VP: incompressible Navier Stokes with a visco-plastic model
c                 *** rectangular grid version ***
c Macro arguments:
c  dir: 0,1,2
c====================================================================================


c ===================================================================================
c        VP: incompressible Navier Stokes with a visco-plastic model
c                 *** curvilinear grid version ***
c Macro arguments:
c  dir: 0,1,2
c====================================================================================




c==================================================================================
c  Residual Computation for VP: incompressible Navier Stokes with a visco-plastic model
c
c Macro args:
c  GRIDTYPE: rectangular, curvilinear
c====================================================================================

! --- Macros for the Spalart-Almaras turbulence model
! --- Macros for the Spalart-Alamras model for the line solver ----
! this file is includes by insLineSOlveNew.bf 


c Define the turbulent eddy viscosity and its derivatives given chi3=chi^3 


c Define the turbulent eddy viscosity and its derivatives  



c Macro to define the set of computations required to compute values for the SA turbulence model.
c used in the macros below

c Macro for the SA TM on rectangular grids
c Only the equation for the turbulence eddy viscosity is done here



! --- Macros for the Baldwin-Lomax approx. 
! define computeBLNuT() :   Macro to compute Baldwin-Lomax Turbulent viscosity
! This file ins included by insLineSolveNew.bf 

c Define the turbulent eddy viscosity and its derivatives for the BL model 


c Define the turbulent eddy viscosity and its derivatives  


c **************************************************************
c   Macro to compute Baldwin-Lomax Turbulent viscosity
c **************************************************************




c ===========================================================================================
c dim : number of dimensions 2,3
c EQN : INS=incompressible N-S, SA=Spalart-Allmaras, TEMPERATURE
c equations e1,e2,...,e10 are for the matrix
c equations e11,e12,... are for the RHS
c ===========================================================================================



c ===========================================================================================
c Loops: 4th-order version
c
c dim : number of dimensions 2,3
c EQN : INS=incompressible N-S, SA=Spalart-Allmaras, TEMPERATURE
c equations e1,e2,...,e10 are for the matrix
c equations e11,e12,... are for the RHS
c ===========================================================================================






c* c ***********************************************************************
c* c Fill in the matrix and RHS on the boundary
c* c EQN : INS=incompressible N-S, SA=Spalart-Allmaras, TEMPERATURE
c* c ***********************************************************************
c* #beginMacro loopsBC(dim,EQN, e1,e2,e3,e4,e5,e6)
c*  do i3=n3a,n3b
c*  do i2=n2a,n2b
c*  do i1=n1a,n1b
c*   if( mask(i1,i2,i3).gt.0 )then
c*    e1
c*    e2
c*    e3
c*    e4
c*    e5
c*    e6
c*   else
c* c for interpolation points or unused:
c*    am(i1,i2,i3)=0.
c*    bm(i1,i2,i3)=1.
c*    cm(i1,i2,i3)=0.
c* #If #EQN == "TEMPERATURE"
c*    f(i1,i2,i3,fct)=uu(tc)
c* #Else
c*    f(i1,i2,i3,fcu)=uu(uc)
c*    f(i1,i2,i3,fcv)=uu(vc)
c* #If #dim == "3"
c*    f(i1,i2,i3,fcw)=uu(wc)
c* #End
c* #End
c* #If #EQN == "SA"
c*    f(i1,i2,i3,nc)=uu(nc)
c* #End      
c*   end if
c*  end do
c*  end do
c*  end do
c* #endMacro
c* 
c* c ***********************************************************************
c* c Fill in the matrix and RHS on the boundary
c* c  SOLVER: INS, SPAL
c* c ***********************************************************************
c* #beginMacro loopsMatrixBC(SOLVER,e1,e2,e3,e4,e5,e6)
c* #If #SOLVER == "INS" || #SOLVER == "INSVP"
c*  if( nd.eq.2 )then
c*   if( option.eq.assignINS )then
c*    loopsBC(2,INS,e1,e2,e3, e4,e5,e6)
c*   else if( option.eq.assignTemperature )then
c*    loopsBC(2,TEMPERATURE,e1,e2,e3, e4,e5,e6)
c*   end if
c*  else
c*   if( option.eq.assignINS )then
c*    loopsBC(3,INS,e1,e2,e3, e4,e5,e6)
c*   else if( option.eq.assignTemperature )then
c*    loopsBC(3,TEMPERATURE,e1,e2,e3, e4,e5,e6)
c*   end if
c*  end if
c* #Elif #SOLVER == "SPAL"
c*  if( nd.eq.2 )then
c*   if( option.eq.assignSpalartAllmaras )then
c*    loopsBC(2,SA,e1,e2,e3, e4,e5,e6)
c*   end if
c*  else
c*   if( option.eq.assignSpalartAllmaras )then
c*    loopsBC(3,SA,e1,e2,e3, e4,e5,e6)
c*   end if
c*  end if
c* #Else
c*   stop 8862
c* #End
c* #endMacro
c* 
c* 
c* c ***********************************************************************
c* c Fill in the matrix and RHS on the boundary, fourth-order version
c* c EQN : INS=incompressible N-S, SA=Spalart-Allmaras, TEMPERATURE
c* c ***********************************************************************
c* #beginMacro loopsBC4(dim,EQN, e1,e2,e3,e4,e5,e6)
c*   do i3=n3a,n3b
c*   do i2=n2a,n2b
c*   do i1=n1a,n1b
c*     if( mask(i1,i2,i3).gt.0 )then
c*       e1
c*       e2
c*       e3
c*       e4
c*       e5
c*       e6
c*     else
c* c for interpolation points or unused:
c*       am(i1,i2,i3)=0.
c*       bm(i1,i2,i3)=0.
c*       cm(i1,i2,i3)=1.
c*       dm(i1,i2,i3)=0.
c*       em(i1,i2,i3)=0.
c*       am(i1-is1,i2-is2,i3-is3)=0.
c*       bm(i1-is1,i2-is2,i3-is3)=0.
c*       cm(i1-is1,i2-is2,i3-is3)=1.
c*       dm(i1-is1,i2-is2,i3-is3)=0.
c*       em(i1-is1,i2-is2,i3-is3)=0.
c* #If #EQN == "TEMPERATURE"
c*       f(i1,i2,i3,fct)=uu(tc)
c*       f(i1-is1,i2-is2,i3-is3,fct)=u(i1-is1,i2-is2,i3-is3,tc)
c* #Else
c*       f(i1,i2,i3,fcu)=uu(uc)
c*       f(i1,i2,i3,fcv)=uu(vc)
c*       f(i1-is1,i2-is2,i3-is3,fcu)=u(i1-is1,i2-is2,i3-is3,uc)
c*       f(i1-is1,i2-is2,i3-is3,fcv)=u(i1-is1,i2-is2,i3-is3,vc)
c* #If #dim == "3"
c*       f(i1,i2,i3,fcw)=uu(wc)
c*       f(i1-is1,i2-is2,i3-is3,fcw)=u(i1-is1,i2-is2,i3-is3,wc)
c* #End
c* #End
c* #If #EQN == "SA"
c*       f(i1,i2,i3,nc)=uu(nc)
c*       f(i1-is1,i2-is2,i3-is3,nc)=u(i1-is1,i2-is2,i3-is3,nc)
c* #End    
c*     end if
c*   end do
c*   end do
c*   end do
c* #endMacro
c* 
c* c ***********************************************************************
c* c Fill in the matrix and RHS on the boundary -- fourth-order version
c* c  SOLVER: INS, SPAL
c* c ***********************************************************************
c* #beginMacro loopsMatrixBC4(SOLVER,e1,e2,e3,e4,e5,e6)
c* #If #SOLVER == "INS"
c*  if( nd.eq.2 )then
c*   if( option.eq.assignINS )then
c*    loopsBC4(2,INS,e1,e2,e3, e4,e5,e6)
c*   else if( option.eq.assignTemperature )then
c*    loopsBC4(2,TEMPERATURE,e1,e2,e3, e4,e5,e6)
c*   end if
c*  else
c*   if( option.eq.assignINS )then
c*    loopsBC4(3,INS,e1,e2,e3, e4,e5,e6)
c*   else if( option.eq.assignTemperature )then
c*    loopsBC4(3,TEMPERATURE,e1,e2,e3, e4,e5,e6)
c*   end if
c*  end if
c* #Elif #SOLVER == "SPAL"
c*  if( nd.eq.2 )then
c*   if( option.eq.assignSpalartAllmaras )then
c*    loopsBC4(2,SA,e1,e2,e3, e4,e5,e6)
c*   end if
c*  else
c*   if( option.eq.assignSpalartAllmaras )then
c*    loopsBC4(3,SA,e1,e2,e3, e4,e5,e6)
c*   end if
c*  end if
c* #Else
c*   stop 7715
c* #End
c* !  if( nd.eq.2 )then
c* !    if( option.eq.assignINS )then
c* !      loopsBC4(2,INS,e1,e2,e3, e4,e5,e6)
c* !    else if( option.eq.assignTemperature )then
c* !      loopsBC4(2,TEMPERATURE,e1,e2,e3, e4,e5,e6)
c* !    else if( option.eq.assignSpalartAllmaras )then
c* !      loopsBC4(2,SA,e1,e2,e3, e4,e5,e6)
c* !    end if
c* !  else
c* !    if( option.eq.assignINS )then
c* !      loopsBC4(3,INS,e1,e2,e3, e4,e5,e6)
c* !    else if( option.eq.assignTemperature )then
c* !      loopsBC4(3,TEMPERATURE,e1,e2,e3, e4,e5,e6)
c* !    else if( option.eq.assignSpalartAllmaras )then
c* !      loopsBC4(3,SA,e1,e2,e3, e4,e5,e6)
c* !    end if
c* !  end if
c* #endMacro


c Define the artificial diffusion coefficients
c gt should be R or C
c tb should be blank or SA (for Splarat-Allmaras)



c=======================================================================
c Define the stuff needed for 2nd-order + 4th-order artificial dissipation
c define: adCoeff2, adCoeff4 and the inline macro ade(cc) (for the rhs)
c=======================================================================

c =======================================================================================
c =======================================================================================





c ===================================================================================
c   ******* fillEquationsRectangularGrid ***********
c
c  SOLVER: INS, INSSPAL, INSBL, INS_TEMPERATURE, INSVP
c  dir: 0,1,2
c====================================================================================

c ===================================================================================
c  ******* fillEquationsCurvilinearGrid *********
c
c  SOLVER: INS, INSSPAL, INSBL, INS_TEMPERATURE 
c  dir: 0,1,2
c====================================================================================







c ===========================================================
c SOLVER: INS, INSSPAL, INSBL, INS_TEMPERATURE
c GRIDTYPE: rectangular, curvilinear
c ===========================================================





c ================================================================================
c Define the subroutine that builds the tridiagonal matrxix for a  given solver
c
c SOLVER: INS, INSSPAL, INSBL, INSVP
c=================================================================================





c      buildFile(INSSPAL,lineSolveINSSPAL)
c      buildFile(INSBL,lineSolveINSBL)



      subroutine insLineSetupNew(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,md1a,md1b,md2a,md2b,md3a,md3b, 
     & mask,rsxy,  u,gv,dt,f,dw, dir,am,bm,cm,dm,em,  bc, 
     & boundaryCondition, ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,
     & ndbcd3b,ndbcd4a,ndbcd4b,bcData, ipar, rpar, ierr )
c======================================================================
c
c         ************* INS Line Solver Function ***************
c
c This function can:
c  (1) Fill in the matrix coefficents for line solvers
c  (2) Assign the right-hand-side values in f
c  (3) Compute the residual  
c
c NOTES:
c   Fill in the interior equation for points (n1a:n1b,n2a:n2b,n3a:n3b)
c   Fill in the BC equations for points outside this (along the line solver direction)
c   
c nd : number of space dimensions
c
c n1a,n1b,n2a,n2b,n3a,n3b : INTERIOR points (does not include boundary points along axis=dir)
c
c dir : 0,1,2 - direction of line 
c a,b,c : output: tridiagonal matrix
c a,b,c,d,e  : output: penta-diagonal matrix (for fourth-order)
c
c ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b : dimensions for the bcData array
c bcData : holds coefficients for BC's
c
c dw: distance to wall for SA TM
c======================================================================
      implicit none
      integer nd, n1a,n1b,n2a,n2b,n3a,n3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,
     & md1a,md1b,md2a,md2b,md3a,md3b,
     & dir

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real dw(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)

      real am(md1a:md1b,md2a:md2b,md3a:md3b)
      real bm(md1a:md1b,md2a:md2b,md3a:md3b)
      real cm(md1a:md1b,md2a:md2b,md3a:md3b)
      real dm(md1a:md1b,md2a:md2b,md3a:md3b)
      real em(md1a:md1b,md2a:md2b,md3a:md3b)

      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)

      real dt(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real f(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer bc(0:1,0:*),boundaryCondition(0:1,0:*), ierr

      ! bcData(component+numberOfComponents*(0),side,axis,grid)  
      integer ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,
     & ndbcd4b
      real bcData(ndbcd1a:ndbcd1b,ndbcd2a:ndbcd2b,ndbcd3a:ndbcd3b,
     & ndbcd4a:ndbcd4b)

      integer ipar(0:*)
      real rpar(0:*)

c     ---- local variables -----
      integer option
      integer assignINS,assignSpalartAllmaras,setupSweep,
     & assignTemperature
      parameter( assignINS=0, assignSpalartAllmaras=1, setupSweep=2, 
     & assignTemperature=3 )

      integer pdeModel,standardModel,BoussinesqModel,viscoPlasticModel
      parameter( standardModel=0,BoussinesqModel=1,viscoPlasticModel=2 
     & )

      integer turbulenceModel,noTurbulenceModel
      integer baldwinLomax,spalartAllmaras,kEpsilon,kOmega
      parameter (noTurbulenceModel=0,baldwinLomax=1,kEpsilon=2,
     & kOmega=3,spalartAllmaras=4 )

c     --- end statement functions

      ierr=0
      ! write(*,*) 'Inside insLineSolve'

      option            = ipar(21)
      turbulenceModel   = ipar(23)
      pdeModel          = ipar(27)

      if( turbulenceModel.eq.noTurbulenceModel )then
        if( pdeModel.eq.standardModel .or. pdeModel.eq.BoussinesqModel 
     & .or. option.eq.assignTemperature )then
          call lineSolveNewINS(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,md1a,md1b,md2a,md2b,md3a,md3b, 
     & mask,rsxy,  u,gv,dt,f,dw, dir,am,bm,cm,dm,em,  bc, 
     & boundaryCondition, ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,
     & ndbcd3b,ndbcd4a,ndbcd4b,bcData, ipar, rpar, ierr )
        else if( pdeModel.eq.viscoPlasticModel )then
         if( .false. .or. option.eq.assignTemperature )then
          call lineSolveINS(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,
     & nd2b,nd3a,nd3b,nd4a,nd4b,md1a,md1b,md2a,md2b,md3a,md3b, mask,
     & rsxy,  u,gv,dt,f,dw, dir,am,bm,cm,dm,em,  bc, 
     & boundaryCondition, ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,
     & ndbcd3b,ndbcd4a,ndbcd4b,bcData, ipar, rpar, ierr )
         else
          call lineSolveNewINSVP(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,md1a,md1b,md2a,md2b,md3a,md3b, 
     & mask,rsxy,  u,gv,dt,f,dw, dir,am,bm,cm,dm,em,  bc, 
     & boundaryCondition, ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,
     & ndbcd3b,ndbcd4a,ndbcd4b,bcData, ipar, rpar, ierr )
         end if
        else
          stop 5533
        end if

c      else if( turbulenceModel.eq.spalartAllmaras )then
c        call lineSolveNewINSSPAL(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,c          md1a,md1b,md2a,md2b,md3a,md3b, mask,rsxy,  u,gv,dt,f,dw, dir,am,bm,cm,dm,em,  bc, boundaryCondition, c          ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b,bcData, ipar, rpar, ierr )
c      else if( turbulenceModel.eq.baldwinLomax )then
c        call lineSolveNewINSBL(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,c          md1a,md1b,md2a,md2b,md3a,md3b, mask,rsxy,  u,gv,dt,f,dw, dir,am,bm,cm,dm,em,  bc, boundaryCondition, c          ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b,bcData, ipar, rpar, ierr )
      else
        write(*,*) 'insLineSetup:Unknown turbulenceModel=',
     & turbulenceModel
        stop 444
      end if

      return
      end







c====================================================================
c Define first derivatives and the coeffciients adc2 and adc4 for the 
c artficial dissipation
c====================================================================







! ins residual function: 
! visco-plastic residual function: 



      subroutine computeResidualNew(nd,
     & n1a,n1b,n2a,n2b,n3a,n3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,
     & mask,rsxy,  u,gv,dt,f,dw, residual,
     & bc, ipar, rpar, ierr )
c======================================================================
c
c  *********** Compute the residual *****************
c
c nd : number of space dimensions
c
c u : input - current solution
c f : input rhs forcing
c
c dw: distance to wall for SA TM
c======================================================================
      implicit none
      integer nd, n1a,n1b,n2a,n2b,n3a,n3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real dw(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real residual(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)

      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)

      real dt(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real f(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer bc(0:1,0:*),ierr
      real dtScale,cfl

      integer ipar(0:*)
      real rpar(0:*)

c     ---- local variables -----
      integer option
      integer assignINS,assignSpalartAllmaras,setupSweep,
     & assignTemperature
      parameter( assignINS=0, assignSpalartAllmaras=1, setupSweep=2, 
     & assignTemperature=3 )

      integer turbulenceModel,noTurbulenceModel
      integer baldwinLomax,spalartAllmaras,kEpsilon,kOmega
      parameter (noTurbulenceModel=0,baldwinLomax=1,kEpsilon=2,
     & kOmega=3,spalartAllmaras=4 )

      integer rectangular,curvilinear
      parameter( rectangular=0, curvilinear=1 )

      integer pdeModel,standardModel,BoussinesqModel,viscoPlasticModel
      parameter( standardModel=0,BoussinesqModel=1,viscoPlasticModel=2 
     & )


      ierr=0
      option            = ipar(21)
      turbulenceModel   = ipar(23)
      pdeModel          = ipar(27)


      if( (pdeModel.eq.standardModel .or. pdeModel.eq.BoussinesqModel )
     &  .and. turbulenceModel.eq.noTurbulenceModel )then

        call lineSolveResidualINS(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rsxy,  u,gv,dt,f,dw, 
     & residual, bc, ipar, rpar, ierr )


      else if( pdeModel.eq.viscoPlasticModel )then

        call lineSolveResidualVP(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rsxy,  u,gv,dt,f,dw, 
     & residual, bc, ipar, rpar, ierr )

      else
        ! for now use old way for BL, SA
        call computeResidual(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,
     & nd2b,nd3a,nd3b,nd4a,nd4b,mask,rsxy,  u,gv,dt,f,dw, residual, 
     & bc, ipar, rpar, ierr )
      end if


      return
      end
