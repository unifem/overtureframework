! This file automatically generated from getViscosity.bf with bpp.


c ************** Evaluate the (non-linear) viscosity  ******************

c   bpp -I$Overture/include -I/home/henshaw.0/Overture/op/src -I/home/henshaw.0/Overture/op/doc getViscosity.bf
c   bpp -quiet -clean -I$Overture/include -I/home/henshaw.0/Overture/op/src -I/home/henshaw.0/Overture/op/doc getViscosity.bf


c --- See --- op/fortranCoeff/opcoeff.bf 
c             op/include/defineConservative.h
c --- See --- mx/src/interfaceMacros.bf <-- mixes different orders of accuracy 

c       *****************************************
c       ** Macros for the Visco-plastic Model ***
c       *****************************************

c  **   NOTE: you should also change viscoPlasticMacrosCpp.h if you change this file ***

c ===============================================================
c Here is effective strain rate (plus a small value)
c         sqrt( (2/3)*eDot_ij eDot_ij ) + epsVP
c Also define the derivatives of the square of the effective strain rate
c  
c ===============================================================





c =====================================================================================
c Here is the coefficient of viscosity for the visco plastic model 
c 
c   esr = effective strain rate = || (2/3)*eDot_ij ||
c
c   nuT = etaVP + (yieldStressVP/esr)*(1.-exp(-exponentVP*esr))
c 
c =====================================================================================

c =====================================================================================
c Here is the coefficient of viscosity for the visco plastic model and its first derivative
c   esr = effective strain rate = || (2/3)*eDot_ij ||
c
c   nuT = etaVP + (yieldStressVP/esr)*(1.-exp(-exponentVP*esr))
c 
c   nuTd = D( nuT )/D( esr**2)  = D( nuT )/D( esr ) * ( 1 / (2*esr ) )
c =====================================================================================



c =============================================================================
c Evaluate the coefficients of the visco-plastic model
c   This macro assumes u0x,u0y,u0z, v0x,v0y,v0z, w0x,w0y,w0z are defined
c   Used in insdt.bf
c ============================================================================


c ============================================================================
c Define the visco-plastic viscosity and first derivatives
c   Used in inspf.bf
c ============================================================================

c ============================================================================
c Define the visco-plastic viscosity and first two derivatives 
c   Used in inspf.bf
c ============================================================================



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









c ===============================================================
c Here is effective strain rate (plus a small value)
c         sqrt( (2/3)*eDot_ij eDot_ij ) + epsVP
c  
c ===============================================================

c =====================================================================================
c Here is the coefficient of viscosity for the visco plastic model 
c
c   nuT = ( etaVP + (yieldStressVP/esr)*(1.-exp(-exponentVP*esr)) )
c where 
c   esr = effective strain rate = || (2/3)*eDot_ij ||
c
c Macro arguments: 
c   DIM : 2,3 
c   GRIDTYPE: rectangular, curvilinear
c =====================================================================================



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


c -- define bpp macros for coefficient operators (from op/src/stencilCoeff.maple)
! This file, opStencilCoeffOrder2.h,  was generated by Overture/op/src/stencilCoeff.maple

! ****************************************************************************** 
! **** This file contains bpp macros to evaluate the coefficient matrix for **** 
! **** derivatives such as x,y,z,xx,yy,zz,laplacian,rr,ss,tt,rrrr,ssss      **** 
! ****************************************************************************** 


! *** dim=2, orderOfAccuracy = 2 *** 





















! *** dim=3, orderOfAccuracy = 2 *** 






























! #Include opStencilCoeffOrder4.h
! #Include opStencilCoeffOrder6.h
! #Include opStencilCoeffOrder8.h


c These next include file will define the macros that will define the difference approximations (in op/src)
c Defines getDuDx2(u,aj,ff), getDuDxx2(u,aj,ff), getDuDx3(u,aj,ff), ...  etc. 


! ****** Dimension 2 ******
 ! getDuDx2 operation count     : additions+2*multiplications+assignments
 ! getDuDx2 optimization savings: -assignments
 ! getDuDy2 operation count     : additions+2*multiplications+assignments
 ! getDuDy2 optimization savings: -assignments
 ! getDuDxx2 operation count     : 9*multiplications+3*assignments+4*additions
 ! getDuDxx2 optimization savings: -3*assignments
 ! getDuDxy2 operation count     : 5*additions+9*multiplications+assignments
 ! getDuDxy2 optimization savings: -assignments
 ! getDuDyy2 operation count     : 9*multiplications+3*assignments+4*additions
 ! getDuDyy2 optimization savings: -3*assignments
 ! getDuDxxx2 operation count     : 25*multiplications+3*assignments+9*additions
 ! getDuDxxx2 optimization savings: 2*multiplications-3*assignments
 ! getDuDxxy2 operation count     : 33*multiplications+3*assignments+15*additions
 ! getDuDxxy2 optimization savings: 2*multiplications-3*assignments
 ! getDuDxyy2 operation count     : 32*multiplications+5*assignments+16*additions
 ! getDuDxyy2 optimization savings: additions+3*multiplications-5*assignments
 ! getDuDyyy2 operation count     : 25*multiplications+3*assignments+9*additions
 ! getDuDyyy2 optimization savings: 2*multiplications-3*assignments
 ! getDuDxxxx2 operation count     : 59*multiplications+11*assignments+24*additions
 ! getDuDxxxx2 optimization savings: 3*additions+23*multiplications-11*assignments
 ! getDuDxxxy2 operation count     : 86*multiplications+11*assignments+37*additions
 ! getDuDxxxy2 optimization savings: 2*additions+24*multiplications-11*assignments
 ! getDuDxxyy2 operation count     : 105*multiplications+16*assignments+50*additions
 ! getDuDxxyy2 optimization savings: 8*additions+37*multiplications-16*assignments
 ! getDuDxyyy2 operation count     : 89*multiplications+21*assignments+52*additions
 ! getDuDxyyy2 optimization savings: 17*additions+47*multiplications-21*assignments
 ! getDuDyyyy2 operation count     : 59*multiplications+11*assignments+24*additions
 ! getDuDyyyy2 optimization savings: 3*additions+23*multiplications-11*assignments
 ! getDuDxxxxx2 operation count     : 154*multiplications+43*assignments+82*additions
 ! getDuDxxxxx2 optimization savings: 39*additions+196*multiplications-43*assignments
 ! getDuDxxxxy2 operation count     : 207*multiplications+42*assignments+102*additions
 ! getDuDxxxxy2 optimization savings: 45*additions+211*multiplications-42*assignments
 ! getDuDxxxyy2 operation count     : 249*multiplications+55*assignments+126*additions
 ! getDuDxxxyy2 optimization savings: 49*additions+241*multiplications-55*assignments
 ! getDuDxxyyy2 operation count     : 239*multiplications+67*assignments+150*additions
 ! getDuDxxyyy2 optimization savings: 97*additions+363*multiplications-67*assignments
 ! getDuDxyyyy2 operation count     : 215*multiplications+73*assignments+150*additions
 ! getDuDxyyyy2 optimization savings: 153*additions+371*multiplications-73*assignments
 ! getDuDyyyyy2 operation count     : 154*multiplications+43*assignments+82*additions
 ! getDuDyyyyy2 optimization savings: 39*additions+196*multiplications-43*assignments
 ! getDuDxxxxxx2 operation count     : 364*multiplications+134*assignments+258*additions
 ! getDuDxxxxxx2 optimization savings: 345*additions+1337*multiplications-134*assignments
 ! getDuDxxxxxy2 operation count     : 525*multiplications+149*assignments+326*additions
 ! getDuDxxxxxy2 optimization savings: 409*additions+1502*multiplications-149*assignments
 ! getDuDxxxxyy2 operation count     : 543*multiplications+173*assignments+339*additions
 ! getDuDxxxxyy2 optimization savings: 415*additions+1552*multiplications-173*assignments
 ! getDuDxxxyyy2 operation count     : 510*multiplications+172*assignments+360*additions
 ! getDuDxxxyyy2 optimization savings: 463*additions+1755*multiplications-172*assignments
 ! getDuDxxyyyy2 operation count     : 482*multiplications+184*assignments+391*additions
 ! getDuDxxyyyy2 optimization savings: 731*additions+2241*multiplications-184*assignments
 ! getDuDxyyyyy2 operation count     : 456*multiplications+188*assignments+384*additions
 ! getDuDxyyyyy2 optimization savings: 1019*additions+2233*multiplications-188*assignments
 ! getDuDyyyyyy2 operation count     : 366*multiplications+133*assignments+258*additions
 ! getDuDyyyyyy2 optimization savings: 345*additions+1335*multiplications-133*assignments


! ****** Dimension 3 ******
 ! getDuDx3 operation count     : 2*additions+3*multiplications+assignments
 ! getDuDx3 optimization savings: -assignments
 ! getDuDy3 operation count     : 2*additions+3*multiplications+assignments
 ! getDuDy3 optimization savings: -assignments
 ! getDuDz3 operation count     : 2*additions+3*multiplications+assignments
 ! getDuDz3 optimization savings: -assignments
 ! getDuDxx3 operation count     : 18*multiplications+4*assignments+8*additions
 ! getDuDxx3 optimization savings: -4*assignments
 ! getDuDxy3 operation count     : 11*additions+18*multiplications+assignments
 ! getDuDxy3 optimization savings: -assignments
 ! getDuDyy3 operation count     : 18*multiplications+4*assignments+8*additions
 ! getDuDyy3 optimization savings: -4*assignments
 ! getDuDxz3 operation count     : 11*additions+18*multiplications+assignments
 ! getDuDxz3 optimization savings: -assignments
 ! getDuDyz3 operation count     : 11*additions+18*multiplications+assignments
 ! getDuDyz3 optimization savings: -assignments
 ! getDuDzz3 operation count     : 18*multiplications+4*assignments+8*additions
 ! getDuDzz3 optimization savings: -4*assignments
 ! getDuDxxx3 operation count     : 58*multiplications+4*assignments+21*additions
 ! getDuDxxx3 optimization savings: 6*multiplications-4*assignments
 ! getDuDxxy3 operation count     : 82*multiplications+7*assignments+38*additions
 ! getDuDxxy3 optimization savings: 9*multiplications-7*assignments
 ! getDuDxyy3 operation count     : 76*multiplications+10*assignments+41*additions
 ! getDuDxyy3 optimization savings: 6*additions+15*multiplications-10*assignments
 ! getDuDyyy3 operation count     : 58*multiplications+4*assignments+21*additions
 ! getDuDyyy3 optimization savings: 6*multiplications-4*assignments
 ! getDuDxxz3 operation count     : 82*multiplications+7*assignments+38*additions
 ! getDuDxxz3 optimization savings: 9*multiplications-7*assignments
 ! getDuDxyz3 operation count     : 50*additions+79*multiplications+4*assignments
 ! getDuDxyz3 optimization savings: 6*additions+12*multiplications-4*assignments
 ! getDuDyyz3 operation count     : 82*multiplications+7*assignments+38*additions
 ! getDuDyyz3 optimization savings: 9*multiplications-7*assignments
 ! getDuDxzz3 operation count     : 76*multiplications+10*assignments+41*additions
 ! getDuDxzz3 optimization savings: 6*additions+15*multiplications-10*assignments
 ! getDuDyzz3 operation count     : 76*multiplications+10*assignments+41*additions
 ! getDuDyzz3 optimization savings: 6*additions+15*multiplications-10*assignments
 ! getDuDzzz3 operation count     : 58*multiplications+4*assignments+21*additions
 ! getDuDzzz3 optimization savings: 6*multiplications-4*assignments
 ! getDuDxxxx3 operation count     : 161*multiplications+29*assignments+71*additions
 ! getDuDxxxx3 optimization savings: 15*additions+98*multiplications-29*assignments
 ! getDuDxxxy3 operation count     : 246*multiplications+28*assignments+110*additions
 ! getDuDxxxy3 optimization savings: 12*additions+109*multiplications-28*assignments
 ! getDuDxxyy3 operation count     : 280*multiplications+43*assignments+142*additions
 ! getDuDxxyy3 optimization savings: 46*additions+192*multiplications-43*assignments
 ! getDuDxyyy3 operation count     : 235*multiplications+49*assignments+148*additions
 ! getDuDxyyy3 optimization savings: 97*additions+225*multiplications-49*assignments
 ! getDuDyyyy3 operation count     : 161*multiplications+29*assignments+71*additions
 ! getDuDyyyy3 optimization savings: 15*additions+98*multiplications-29*assignments
 ! getDuDxxxz3 operation count     : 247*multiplications+27*assignments+110*additions
 ! getDuDxxxz3 optimization savings: 12*additions+108*multiplications-27*assignments
 ! getDuDxxyz3 operation count     : 292*multiplications+31*assignments+154*additions
 ! getDuDxxyz3 optimization savings: 46*additions+186*multiplications-31*assignments
 ! getDuDxyyz3 operation count     : 271*multiplications+166*additions+31*assignments
 ! getDuDxyyz3 optimization savings: 97*additions+207*multiplications-31*assignments
 ! getDuDyyyz3 operation count     : 247*multiplications+27*assignments+110*additions
 ! getDuDyyyz3 optimization savings: 12*additions+108*multiplications-27*assignments
 ! getDuDxxzz3 operation count     : 277*multiplications+46*assignments+142*additions
 ! getDuDxxzz3 optimization savings: 46*additions+195*multiplications-46*assignments
 ! getDuDxyzz3 operation count     : 256*multiplications+52*assignments+175*additions
 ! getDuDxyzz3 optimization savings: 115*additions+222*multiplications-52*assignments
 ! getDuDyyzz3 operation count     : 277*multiplications+46*assignments+142*additions
 ! getDuDyyzz3 optimization savings: 46*additions+195*multiplications-46*assignments
 ! getDuDxzzz3 operation count     : 235*multiplications+49*assignments+148*additions
 ! getDuDxzzz3 optimization savings: 97*additions+225*multiplications-49*assignments
 ! getDuDyzzz3 operation count     : 235*multiplications+49*assignments+148*additions
 ! getDuDyzzz3 optimization savings: 97*additions+225*multiplications-49*assignments
 ! getDuDzzzz3 operation count     : 161*multiplications+29*assignments+71*additions
 ! getDuDzzzz3 optimization savings: 15*additions+98*multiplications-29*assignments
 ! getDuDxxxxx3 operation count     : 480*multiplications+117*assignments+279*additions
 ! getDuDxxxxx3 optimization savings: 239*additions+1000*multiplications-117*assignments
 ! getDuDxxxxy3 operation count     : 644*multiplications+120*assignments+345*additions
 ! getDuDxxxxy3 optimization savings: 266*additions+1103*multiplications-120*assignments
 ! getDuDxxxyy3 operation count     : 402*additions+732*multiplications+150*assignments
 ! getDuDxxxyy3 optimization savings: 311*additions+1309*multiplications-150*assignments
 ! getDuDxxyyy3 operation count     : 685*multiplications+177*assignments+458*additions
 ! getDuDxxyyy3 optimization savings: 576*additions+1896*multiplications-177*assignments
 ! getDuDxyyyy3 operation count     : 619*multiplications+186*assignments+461*additions
 ! getDuDxyyyy3 optimization savings: 924*additions+1926*multiplications-186*assignments
 ! getDuDyyyyy3 operation count     : 479*multiplications+117*assignments+279*additions
 ! getDuDyyyyy3 optimization savings: 239*additions+1001*multiplications-117*assignments
 ! getDuDxxxxz3 operation count     : 646*multiplications+120*assignments+345*additions
 ! getDuDxxxxz3 optimization savings: 266*additions+1101*multiplications-120*assignments
 ! getDuDxxxyz3 operation count     : 833*multiplications+116*assignments+441*additions
 ! getDuDxxxyz3 optimization savings: 323*additions+1298*multiplications-116*assignments
 ! getDuDxxyyz3 operation count     : 889*multiplications+138*assignments+512*additions
 ! getDuDxxyyz3 optimization savings: 594*additions+1827*multiplications-138*assignments
 ! getDuDxyyyz3 operation count     : 754*multiplications+156*assignments+536*additions
 ! getDuDxyyyz3 optimization savings: 984*additions+1908*multiplications-156*assignments
 ! getDuDyyyyz3 operation count     : 648*multiplications+119*assignments+345*additions
 ! getDuDyyyyz3 optimization savings: 266*additions+1099*multiplications-119*assignments
 ! getDuDxxxzz3 operation count     : 730*multiplications+149*assignments+402*additions
 ! getDuDxxxzz3 optimization savings: 311*additions+1311*multiplications-149*assignments
 ! getDuDxxyzz3 operation count     : 754*multiplications+183*assignments+500*additions
 ! getDuDxxyzz3 optimization savings: 606*additions+1899*multiplications-183*assignments
 ! getDuDxyyzz3 operation count     : 524*additions+727*multiplications+174*assignments
 ! getDuDxyyzz3 optimization savings: 978*additions+1953*multiplications-174*assignments
 ! getDuDyyyzz3 operation count     : 730*multiplications+149*assignments+402*additions
 ! getDuDyyyzz3 optimization savings: 311*additions+1311*multiplications-149*assignments
 ! getDuDxxzzz3 operation count     : 679*multiplications+183*assignments+458*additions
 ! getDuDxxzzz3 optimization savings: 576*additions+1902*multiplications-183*assignments
 ! getDuDxyzzz3 operation count     : 658*multiplications+195*assignments+533*additions
 ! getDuDxyzzz3 optimization savings: 1095*additions+2004*multiplications-195*assignments
 ! getDuDyyzzz3 operation count     : 679*multiplications+183*assignments+458*additions
 ! getDuDyyzzz3 optimization savings: 576*additions+1902*multiplications-183*assignments
 ! getDuDxzzzz3 operation count     : 619*multiplications+186*assignments+461*additions
 ! getDuDxzzzz3 optimization savings: 924*additions+1926*multiplications-186*assignments
 ! getDuDyzzzz3 operation count     : 619*multiplications+186*assignments+461*additions
 ! getDuDyzzzz3 optimization savings: 924*additions+1926*multiplications-186*assignments
 ! getDuDzzzzz3 operation count     : 481*multiplications+115*assignments+279*additions
 ! getDuDzzzzz3 optimization savings: 239*additions+999*multiplications-115*assignments
 ! getDuDxxxxxx3 operation count     : 1232*multiplications+380*assignments+916*additions
 ! getDuDxxxxxx3 optimization savings: 2414*additions+8130*multiplications-380*assignments
 ! getDuDxxxxxy3 operation count     : 1742*multiplications+419*assignments+1142*additions
 ! getDuDxxxxxy3 optimization savings: 2790*additions+9123*multiplications-419*assignments
 ! getDuDxxxxyy3 operation count     : 1728*multiplications+492*assignments+1158*additions
 ! getDuDxxxxyy3 optimization savings: 2837*additions+9455*multiplications-492*assignments
 ! getDuDxxxyyy3 operation count     : 1194*additions+1600*multiplications+481*assignments
 ! getDuDxxxyyy3 optimization savings: 3128*additions+10552*multiplications-481*assignments
 ! getDuDxxyyyy3 operation count     : 1265*additions+1496*multiplications+508*assignments
 ! getDuDxxyyyy3 optimization savings: 4761*additions+13467*multiplications-508*assignments
 ! getDuDxyyyyy3 operation count     : 1421*multiplications+505*assignments+1253*additions
 ! getDuDxyyyyy3 optimization savings: 6954*additions+13503*multiplications-505*assignments
 ! getDuDyyyyyy3 operation count     : 1230*multiplications+381*assignments+916*additions
 ! getDuDyyyyyy3 optimization savings: 2414*additions+8132*multiplications-381*assignments
 ! getDuDxxxxxz3 operation count     : 1741*multiplications+417*assignments+1142*additions
 ! getDuDxxxxxz3 optimization savings: 2790*additions+9124*multiplications-417*assignments
 ! getDuDxxxxyz3 operation count     : 2098*multiplications+378*assignments+1289*additions
 ! getDuDxxxxyz3 optimization savings: 3120*additions+10006*multiplications-378*assignments
 ! getDuDxxxyyz3 operation count     : 2226*multiplications+460*assignments+1406*additions
 ! getDuDxxxyyz3 optimization savings: 3465*additions+11159*multiplications-460*assignments
 ! getDuDxxyyyz3 operation count     : 2063*multiplications+503*assignments+1531*additions
 ! getDuDxxyyyz3 optimization savings: 5227*additions+14391*multiplications-503*assignments
 ! getDuDxyyyyz3 operation count     : 1552*additions+1886*multiplications+521*assignments
 ! getDuDxyyyyz3 optimization savings: 7924*additions+14415*multiplications-521*assignments
 ! getDuDyyyyyz3 operation count     : 1745*multiplications+412*assignments+1142*additions
 ! getDuDyyyyyz3 optimization savings: 2790*additions+9120*multiplications-412*assignments
 ! getDuDxxxxzz3 operation count     : 1733*multiplications+489*assignments+1158*additions
 ! getDuDxxxxzz3 optimization savings: 2837*additions+9450*multiplications-489*assignments
 ! getDuDxxxyzz3 operation count     : 1971*multiplications+514*assignments+1338*additions
 ! getDuDxxxyzz3 optimization savings: 3398*additions+11018*multiplications-514*assignments
 ! getDuDxxyyzz3 operation count     : 2018*multiplications+554*assignments+1463*additions
 ! getDuDxxyyzz3 optimization savings: 5157*additions+14124*multiplications-554*assignments
 ! getDuDxyyyzz3 operation count     : 1502*additions+1835*multiplications+572*assignments
 ! getDuDxyyyzz3 optimization savings: 7830*additions+14196*multiplications-572*assignments
 ! getDuDyyyyzz3 operation count     : 1735*multiplications+490*assignments+1158*additions
 ! getDuDyyyyzz3 optimization savings: 2837*additions+9448*multiplications-490*assignments
 ! getDuDxxxzzz3 operation count     : 1595*multiplications+477*assignments+1194*additions
 ! getDuDxxxzzz3 optimization savings: 3128*additions+10557*multiplications-477*assignments
 ! getDuDxxyzzz3 operation count     : 1631*multiplications+526*assignments+1373*additions
 ! getDuDxxyzzz3 optimization savings: 5079*additions+13824*multiplications-526*assignments
 ! getDuDxyyzzz3 operation count     : 1415*additions+1610*multiplications+511*assignments
 ! getDuDxyyzzz3 optimization savings: 7512*additions+14169*multiplications-511*assignments
 ! getDuDyyyzzz3 operation count     : 1595*multiplications+477*assignments+1194*additions
 ! getDuDyyyzzz3 optimization savings: 3128*additions+10557*multiplications-477*assignments
 ! getDuDxxzzzz3 operation count     : 1487*multiplications+517*assignments+1265*additions
 ! getDuDxxzzzz3 optimization savings: 4761*additions+13476*multiplications-517*assignments
 ! getDuDxyzzzz3 operation count     : 1478*multiplications+538*assignments+1406*additions
 ! getDuDxyzzzz3 optimization savings: 8178*additions+14193*multiplications-538*assignments
 ! getDuDyyzzzz3 operation count     : 1487*multiplications+517*assignments+1265*additions
 ! getDuDyyzzzz3 optimization savings: 4761*additions+13476*multiplications-517*assignments
 ! getDuDxzzzzz3 operation count     : 1421*multiplications+505*assignments+1253*additions
 ! getDuDxzzzzz3 optimization savings: 6954*additions+13503*multiplications-505*assignments
 ! getDuDyzzzzz3 operation count     : 1421*multiplications+505*assignments+1253*additions
 ! getDuDyzzzzz3 optimization savings: 6954*additions+13503*multiplications-505*assignments
 ! getDuDzzzzzz3 operation count     : 1232*multiplications+382*assignments+916*additions
 ! getDuDzzzzzz3 optimization savings: 2414*additions+8130*multiplications-382*assignments

c Define 
c    defineParametricDerivativeMacros(u,dr,DIM,ORDER,COMPONENTS,MAXDERIV)
c       defines -> ur2, us2, ux2, uy2, ...            (2D)
c                  ur3, us3, ut3, ux3, uy3, uz3, ...  (3D)
! This file was generated by weights.maple


! This next macro will evaluate parametric derivatives and save in temporaries
!   u is the variable name, v is the prefix for the temporaries, e.g.
!   For example, lines of the following form will be generated:
!      v = u(i1,i2,i3) 
!      vr = ur4(i1,i2,i3) 

! This next macro will evaluate parametric derivatives and save in temporaries
!   u is the variable name, v is the prefix for the temporaries, e.g.
!   For example, lines of the following form will be generated:
!      v = u(i1,i2,i3) 
!      vr = ur4(i1,i2,i3) 

! This next macro will evaluate parametric derivatives and save in temporaries
!   u is the variable name, v is the prefix for the temporaries, e.g.
!   For example, lines of the following form will be generated:
!      v = u(i1,i2,i3) 
!      vr = ur4(i1,i2,i3) 

! This next macro will evaluate x,y,z derivatives using temporaries already computed 
!   u1 is the variable name, aj the jaocbian name and v is the prefix for the temporaries
!   For example, lines of the following form will be generated:
!      getDuDx2(u1,aj,vx) 
!      getDuDxy2(u1,aj,vxy) 
!      getDuDxxx2(u1,aj,vxxx) 

! This next macro will evaluate x,y,z derivatives using temporaries already computed 
!   u1 is the variable name, aj the jaocbian name and v is the prefix for the temporaries
!   For example, lines of the following form will be generated:
!      getDuDx2(u1,aj,vx) 
!      getDuDxy2(u1,aj,vxy) 
!      getDuDxxx2(u1,aj,vxxx) 

! This next macro will evaluate x,y,z derivatives using temporaries already computed 
!   u1 is the variable name, aj the jaocbian name and v is the prefix for the temporaries
!   For example, lines of the following form will be generated:
!      getDuDx2(u1,aj,vx) 
!      getDuDxy2(u1,aj,vxy) 
!      getDuDxxx2(u1,aj,vxxx) 

! This next macro will evaluate x,y,z derivatives of the jacobian 

! u = jacobian name (rsxy), v=prefix for derivatives: vrxr, vrys, 

! defineParametricDerivativeMacros(u,DIM,ORDER,COMPONENTS,MAXDERIV)
! 2D, order=6, components=1
! defineParametricDerivativeMacros(u,dr,DIM,ORDER,COMPONENTS,MAXDERIV)

! defineParametricDerivativeMacros(u,dr,2,2,1,2)
 ! *************** 0 components *************
 ! *************** 1 components *************
 ! *************** 2 components *************
!  defineParametricDerivativeMacros(rsxy,dr,3,4,2,2)
!  defineParametricDerivativeMacros(rsxy,dr,3,6,2,2)

 ! *************** 0 components *************
 ! *************** 1 components *************
 ! *************** 2 components *************
!  defineParametricDerivativeMacros(ul,dr,3,2,1,2)


! Example to define orders 2,4,6: 
! defineParametricDerivativeMacros(u1,dr1,2,2,1,6)
! defineParametricDerivativeMacros(u1,dr1,2,4,1,4)
! defineParametricDerivativeMacros(u1,dr1,2,6,1,2)

! construct an include file that declares temporary variables:


! define macros for conservative operators: (in op/src)
!  defines getConservativeCoeff( OPERATOR,s,coeff ), OPERATOR=divScalarGrad, ...
! #Include "conservativeCoefficientMacros.h"

c From opcoeff.bf




! ==========================================================================================
!  Evaluate the Jacobian and it's derivatives (parametric and spatial). 
!    aj     : prefix for the name of the resulting jacobian variables, 
!             e.g. ajrx, ajsy, ajrxx, ajsxy, ...
!    MAXDER : number of derivatives to evaluate.  
! ==========================================================================================

! ==========================================================================================
!  Evaluate the parametric derivatives of u.
!    u      : evaluate derivatives of this function.
!    uc     : component to evaluate
!    uu     : prefix for the name of the resulting derivatives, e.g. uur, uus, uurr, ...
!    MAXDER : number of derivatives to evaluate.  
! ==========================================================================================


! ==========================================================================================
!  Evaluate a derivative. (assumes parametric derivatives have already been evaluated)
!   DERIV   : name of the derivative. One of 
!                x,y,z,xx,xy,xz,...
!    u      : evaluate derivatives of this function.
!    uc     : component to evaluate
!    uu     : prefix for the name of the resulting derivatives (same name used with opEvalParametricDerivative) 
!    aj     : prefix for the name of the jacobian variables.
!    ud     : derivative is assigned to this variable.
! ==========================================================================================



c =====================================================================================
c Here is the coefficient of viscosity for the visco plastic model 
c
c   nuT = ( etaVP + (yieldStressVP/esr)*(1.-exp(-exponentVP*esr)) )
c where 
c   esr = effective strain rate = || (2/3)*eDot_ij ||
c
c Macro arguments: 
c   DIM : 2,3 
c   GRIDTYPE: rectangular, curvilinear
c =====================================================================================


      subroutine getViscosity(nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,
     & nd4b,mask,xy,rsxy,  u, visc, gv,dw,  bc, ipar, rpar, ierr )
c======================================================================
c 
c    Compute the coefficient of the (nonlinear) viscosity 
c       for the Incompressible Navier Stokes Equations
c    -----------------------------------------------------
c
c
c nd : number of space dimensions
c nd1a,nd1b,nd2a,nd2b,nd3a,nd3b : array dimensions
c
c mask : 
c xy : 
c rsxy : 
c u : holds the current solution, used to evaluate the viscosity coefficient.
c visc : output, the coefficient of the (nonlinear) viscosity
c gv : gridVelocity for moving grids
c dw : distance to the wall for some turbulence models
c 
c======================================================================
      implicit none
      integer nd, ndc, n1a,n1b,n2a,n2b,n3a,n3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b
      integer nde,nr1a,nr1b,nr2a,nr2b,nr3a,nr3b

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)

      real visc(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)

      real gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      real dw(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer bc(0:1,0:2),indexRange(0:1,0:2),ierr

      integer ipar(0:*)
      real rpar(0:*)

c     ---- local variables -----
      integer c,e,i1,i2,i3,m1,m2,m3,j1,j2,j3,ghostLine,n,i1m,i2m,i3m,
     & i1p,i2p,i3p
      integer side,axis,is1,is2,is3,mm,eqnTemp,debug
      integer kd,kd3,orderOfAccuracy,gridIsMoving,orderOfExtrap,
     & orderOfExtrapolation,orderOfExtrapolationForOutflow
      integer numberOfComponentsForCoefficients,stencilSize
      integer gridIsImplicit,implicitOption,implicitMethod,
     & isAxisymmetric,use2ndOrderAD,use4thOrderAD
      integer pc,uc,vc,wc,sc,nc,kc,ec,tc,grid,m,advectPassiveScalar
      real nu,dt,nuPassiveScalar,adcPassiveScalar
      real gravity(0:2), thermalExpansivity, adcBoussinesq,kThermal
      real dxi,dyi,dzi,dri,dsi,dti,dr2i,ds2i,dt2i
      real ad21,ad22,ad41,ad42,cd22,cd42,adc
      real ad21n,ad22n,ad41n,ad42n,cd22n,cd42n
      real yy,yEps

      integer fillCoefficients,evalRightHandSide,evalResidual,
     & evalResidualForBoundaryConditions

      integer equationNumberBase1,equationNumberLength1,
     & equationNumberBase2,equationNumberLength2,equationNumberBase3,
     & equationNumberLength3,equationOffset

      integer gridType
      integer rectangular,curvilinear
      parameter( rectangular=0, curvilinear=1 )

      integer turbulenceModel,noTurbulenceModel
      integer baldwinLomax,spalartAllmaras,kEpsilon,kOmega
      parameter (noTurbulenceModel=0,baldwinLomax=1,kEpsilon=2,
     & kOmega=3,spalartAllmaras=4 )

      integer computeAllTerms,
     &     doNotComputeImplicitTerms,
     &     computeImplicitTermsSeparately,
     &     computeAllWithWeightedImplicit

      parameter( computeAllTerms=0,
     &           doNotComputeImplicitTerms=1,
     &           computeImplicitTermsSeparately=2,
     &           computeAllWithWeightedImplicit=3 )

      real implicitFactor

      integer implicitVariation
      integer implicitViscous, implicitAdvectionAndViscous, 
     & implicitFullLinearized
      parameter( implicitViscous=0,
     &           implicitAdvectionAndViscous=1,
     &           implicitFullLinearized=2 )

      integer pdeModel,standardModel,BoussinesqModel,viscoPlasticModel
      parameter( standardModel=0,BoussinesqModel=1,viscoPlasticModel=2 
     & )

      real rx,ry,rz,sx,sy,sz,tx,ty,tz
      real aj
      real dr(0:2), dx(0:2)

      integer ncc,halfWidth,halfWidth3

      real u0x,u0y,u0z, v0x,v0y,v0z, w0x,w0y,w0z, p0x,p0y,p0z
      real ulx,uly,ulz, vlx,vly,vlz, wlx,wly,wlz, plx,ply,plz
      real u0xx,u0xy,u0xz,u0yy,u0yz,u0zz
      real v0xx,v0xy,v0xz,v0yy,v0yz,v0zz
      real w0xx,w0xy,w0xz,w0yy,w0yz,w0zz

      ! --- visco plastic variables ---
c     --- visco plastic ---
       real nuVP,etaVP,yieldStressVP,exponentVP,epsVP
       real nuViscoPlastic, etaViscoPlastic, yieldStressViscoPlastic, 
     & exponentViscoPlastic, epsViscoPlastic
       real eDotNorm,exp0,esr
       real uax,uay,vax,vay
       real nu0ph,nu0mh,nu1ph,nu1mh
       real nuzmz,numzz,nuzzz,nupzz,nuzpz
       real ajzmz,ajmzz,ajzzz,ajpzz,ajzpz
       real a11mzz,a11zzz,a11pzz,a11ph,a11mh
       real a22zmz,a22zzz,a22zpz,a22ph,a22mh
       real a12mzz,a12zzz,a12pzz
       real a21zmz,a21zzz,a21zpz
       real b11mzz,b11zzz,b11pzz,b11ph,b11mh
       real b22zmz,b22zzz,b22zpz,b22ph,b22mh
       real b12mzz,b12zzz,b12pzz
       real b21zmz,b21zzz,b21zpz
       real au11mzz,au11zzz,au11pzz,au11ph,au11mh
       real au22zmz,au22zzz,au22zpz,au22ph,au22mh
       real au12mzz,au12zzz,au12pzz
       real au21zmz,au21zzz,au21zpz
       real bu11mzz,bu11zzz,bu11pzz,bu11ph,bu11mh
       real bu22zmz,bu22zzz,bu22zpz,bu22ph,bu22mh
       real bu12mzz,bu12zzz,bu12pzz
       real bu21zmz,bu21zzz,bu21zpz
       real av11mzz,av11zzz,av11pzz,av11ph,av11mh
       real av22zmz,av22zzz,av22zpz,av22ph,av22mh
       real av12mzz,av12zzz,av12pzz
       real av21zmz,av21zzz,av21zpz
       real bv11mzz,bv11zzz,bv11pzz,bv11ph,bv11mh
       real bv22zmz,bv22zzz,bv22zpz,bv22ph,bv22mh
       real bv12mzz,bv12zzz,bv12pzz
       real bv21zmz,bv21zzz,bv21zpz

      ! This include file (created above) declares variables needed by the getDuDx() macros.
      include 'getViscosityDeclareTemporaryVariablesOrder2.h'

c     --- begin statement functions
      rx(i1,i2,i3)=rsxy(i1,i2,i3,0,0)
      ry(i1,i2,i3)=rsxy(i1,i2,i3,0,1)
      rz(i1,i2,i3)=rsxy(i1,i2,i3,0,2)
      sx(i1,i2,i3)=rsxy(i1,i2,i3,1,0)
      sy(i1,i2,i3)=rsxy(i1,i2,i3,1,1)
      sz(i1,i2,i3)=rsxy(i1,i2,i3,1,2)
      tx(i1,i2,i3)=rsxy(i1,i2,i3,2,0)
      ty(i1,i2,i3)=rsxy(i1,i2,i3,2,1)
      tz(i1,i2,i3)=rsxy(i1,i2,i3,2,2)
c     --- end statement functions

      ierr=0

      n1a                =ipar(0)
      n1b                =ipar(1)
      n2a                =ipar(2)
      n2b                =ipar(3)
      n3a                =ipar(4)
      n3b                =ipar(5)

      pc                 =ipar(6)
      uc                 =ipar(7)
      vc                 =ipar(8)
      wc                 =ipar(9)
      nc                 =ipar(10)
      sc                 =ipar(11)
      tc                 =ipar(12)
      grid               =ipar(13)
      orderOfAccuracy    =ipar(14)
      gridIsMoving       =ipar(15) ! *************

      implicitVariation  =ipar(16) ! **new**

      fillCoefficients   =ipar(17) ! new
      evalRightHandSide  =ipar(18) ! new

      gridIsImplicit     =ipar(19)
      implicitMethod     =ipar(20)
      implicitOption     =ipar(21)
      isAxisymmetric     =ipar(22)
      use2ndOrderAD      =ipar(23)
      use4thOrderAD      =ipar(24)
      advectPassiveScalar=ipar(25)
      gridType           =ipar(26)
      turbulenceModel    =ipar(27)
      pdeModel           =ipar(28)
      numberOfComponentsForCoefficients =ipar(29) ! number of components for coefficients
      stencilSize        =ipar(30)

      equationOffset        = ipar(31)
      equationNumberBase1   = ipar(32)
      equationNumberLength1 = ipar(33)
      equationNumberBase2   = ipar(34)
      equationNumberLength2 = ipar(35)
      equationNumberBase3   = ipar(36)
      equationNumberLength3 = ipar(37)

      indexRange(0,0)    =ipar(38)
      indexRange(1,0)    =ipar(39)
      indexRange(0,1)    =ipar(40)
      indexRange(1,1)    =ipar(41)
      indexRange(0,2)    =ipar(42)
      indexRange(1,2)    =ipar(43)

      orderOfExtrapolation=ipar(44)
      orderOfExtrapolationForOutflow=ipar(45)

      evalResidual      = ipar(46)
      evalResidualForBoundaryConditions=ipar(47)
      debug             = ipar(48)

      dr(0)             =rpar(0)
      dr(1)             =rpar(1)
      dr(2)             =rpar(2)
      dx(0)             =rpar(3)
      dx(1)             =rpar(4)
      dx(2)             =rpar(5)

      dt                =rpar(6) ! **new**
      implicitFactor    =rpar(7) ! **new**

      nu                =rpar(8)
      ad21              =rpar(9)
      ad22              =rpar(10)
      ad41              =rpar(11)
      ad42              =rpar(12)
      nuPassiveScalar   =rpar(13)
      adcPassiveScalar  =rpar(14)
      ad21n             =rpar(15)
      ad22n             =rpar(16)
      ad41n             =rpar(17)
      ad42n             =rpar(18)
      yEps              =rpar(19) ! for axisymmetric

      gravity(0)        =rpar(20)
      gravity(1)        =rpar(21)
      gravity(2)        =rpar(22)
      thermalExpansivity=rpar(23)
      adcBoussinesq     =rpar(24) ! coefficient of artificial diffusion for Boussinesq T equation
      kThermal          =rpar(25)

      nuViscoPlastic         =rpar(26)
      etaViscoPlastic        =rpar(27)
      yieldStressViscoPlastic=rpar(28)
      exponentViscoPlastic   =rpar(29)
      epsViscoPlastic        =rpar(30)   ! small parameter used to offset the effective strain rate

      ! here are the names used by the getViscoPlasticViscosity macro -- what should we do about this ? 
      etaVP=etaViscoPlastic
      yieldStressVP=yieldStressViscoPlastic
      exponentVP=exponentViscoPlastic
      epsVP=epsViscoPlastic

      ncc=numberOfComponentsForCoefficients ! number of components for coefficients

      kc=nc
      ec=kc+1

      if( orderOfAccuracy.ne.2 .and. orderOfAccuracy.ne.4 )then
        write(*,'("insdt:ERROR orderOfAccuracy=",i6)') orderOfAccuracy
        stop 1
      end if
      if( gridType.ne.rectangular .and. gridType.ne.curvilinear )then
        write(*,'("insdt:ERROR gridType=",i6)') gridType
        stop 2
      end if
      if( uc.lt.0 .or. vc.lt.0 .or. (nd.eq.3 .and. wc.lt.0) )then
        write(*,'("insdt:ERROR uc,vc,ws=",3i6)') uc,vc,wc
        stop 4
      end if

c      write(*,'("insdt: turbulenceModel=",2i6)') turbulenceModel
c      write(*,'("insdt: nd,uc,vc,wc,kc=",2i6)') nd,uc,vc,wc,kc

      if( turbulenceModel.eq.kEpsilon .and. (kc.lt.uc+nd .or. 
     & kc.gt.1000) )then
        write(*,'("insdt:ERROR in kc: nd,uc,vc,wc,kc=",2i6)') nd,uc,vc,
     & wc,kc
        stop 5
      end if

      ! ==== Assign all possible points ====
      n1a=nd1a+1
      n1b=nd1b-1
      n2a=nd2a+1
      n2b=nd2b-1
      if( nd.gt.2 )then
        n3a=nd3a+1
        n3b=nd3b-1
      else
        n3a=nd3a
        n3b=nd3b
      end if
      if( .true. .or. debug.gt.1 )then
        write(*,'("****** getViscosity etaVP,yieldStressVP=",2e10.2)') 
     & etaVP,yieldStressVP
      end if

! Define operator parameters
!   $DIM : number of spatial dimensions
!   $ORDER : order of accuracy of an approximation
!   $GRIDTYPE : rectangular or curvilinear
!   $MATRIX_STENCIL_WIDTH : space in the global coeff matrix was allocated to hold this size stencil
!   $STENCIL_WIDTH : stencil width of the local coeff-matrix (such as xCoeff, yCoeff, lapCoeff, ...)

      if( nd.eq.2 .and. gridType.eq.curvilinear )then

       ! **** to do : optimize this for backward-Euler : fe=0, fi=0 !!
       if( pdeModel.eq.viscoPlasticModel )then
         do i3=n3a,n3b
         do i2=n2a,n2b
         do i1=n1a,n1b
          if( mask(i1,i2,i3).ne.0 )then
         ! Evaluate the jacobian derivatives used by the coefficient and forward derivatives:
         ! opEvalJacobianDerivatives(MAXDER) : MAXDER = max number of derivatives to precompute.
          ! this next call will define the jacobian and its derivatives (parameteric and spatial)
          ajrx = rsxy(i1,i2,i3,0,0)
          ajsx = rsxy(i1,i2,i3,1,0)
          ajry = rsxy(i1,i2,i3,0,1)
          ajsy = rsxy(i1,i2,i3,1,1)
         ! evaluate forward derivatives of the current solution: 
         ! First evaluate the parametric derivatives of u, used later by the spatial derivatives. 
         ! MAXDER = max number of parametric derivatives to precompute.
          uu = u(i1,i2,i3,uc)
          uur = (-u(i1-1,i2,i3,uc)+u(i1+1,i2,i3,uc))/(2.*dr(0))
          uus = (-u(i1,i2-1,i3,uc)+u(i1,i2+1,i3,uc))/(2.*dr(1))
         ! Evaluate the spatial derivatives of u:
           u0x = ajrx*uur+ajsx*uus
           u0y = ajry*uur+ajsy*uus
         ! Evaluate the spatial derivatives of v:
          vv = u(i1,i2,i3,vc)
          vvr = (-u(i1-1,i2,i3,vc)+u(i1+1,i2,i3,vc))/(2.*dr(0))
          vvs = (-u(i1,i2-1,i3,vc)+u(i1,i2+1,i3,vc))/(2.*dr(1))
           v0x = ajrx*vvr+ajsx*vvs
           v0y = ajry*vvr+ajsy*vvs
         esr = (sqrt((2./3.)*(u0x**2+v0y**2+.5*(u0y+v0x)**2))+epsVP)
        ! this next macro is in viscoPlasticMacros.h 
          exp0 = exp(-exponentVP*esr)
          visc(i1,i2,i3,0) = (etaVP + (yieldStressVP/esr)*(1.-exp0))
          end if
         end do
         end do
         end do
       else
        write(*,'("getViscosity:ERROR: unknown pdeModel=",i6)') 
     & pdeModel
       end if

      else if(  nd.eq.2 .and. gridType.eq.rectangular )then

       ! **** to do : optimize this for backward-Euler : fe=0, fi=0 !!
       if( pdeModel.eq.viscoPlasticModel )then
         do i3=n3a,n3b
         do i2=n2a,n2b
         do i1=n1a,n1b
          if( mask(i1,i2,i3).ne.0 )then
         ! Evaluate the jacobian derivatives used by the coefficient and forward derivatives:
         ! opEvalJacobianDerivatives(MAXDER) : MAXDER = max number of derivatives to precompute.
         ! evaluate forward derivatives of the current solution: 
         ! First evaluate the parametric derivatives of u, used later by the spatial derivatives. 
         ! MAXDER = max number of parametric derivatives to precompute.
          uu=u(i1,i2,i3,uc) ! in the rectangular case just eval the solution
         ! Evaluate the spatial derivatives of u:
           u0x = (-u(i1-1,i2,i3,uc)+u(i1+1,i2,i3,uc))/(2.*dx(0))
           u0y = (-u(i1,i2-1,i3,uc)+u(i1,i2+1,i3,uc))/(2.*dx(1))
         ! Evaluate the spatial derivatives of v:
          vv=u(i1,i2,i3,vc) ! in the rectangular case just eval the solution
           v0x = (-u(i1-1,i2,i3,vc)+u(i1+1,i2,i3,vc))/(2.*dx(0))
           v0y = (-u(i1,i2-1,i3,vc)+u(i1,i2+1,i3,vc))/(2.*dx(1))
         esr = (sqrt((2./3.)*(u0x**2+v0y**2+.5*(u0y+v0x)**2))+epsVP)
        ! this next macro is in viscoPlasticMacros.h 
          exp0 = exp(-exponentVP*esr)
          visc(i1,i2,i3,0) = (etaVP + (yieldStressVP/esr)*(1.-exp0))
          end if
         end do
         end do
         end do
       else
        write(*,'("getViscosity:ERROR: unknown pdeModel=",i6)') 
     & pdeModel
       end if

      else if( nd.eq.3 .and. gridType.eq.curvilinear )then

       ! **** to do : optimize this for backward-Euler : fe=0, fi=0 !!
       if( pdeModel.eq.viscoPlasticModel )then
         do i3=n3a,n3b
         do i2=n2a,n2b
         do i1=n1a,n1b
          if( mask(i1,i2,i3).ne.0 )then
         ! Evaluate the jacobian derivatives used by the coefficient and forward derivatives:
         ! opEvalJacobianDerivatives(MAXDER) : MAXDER = max number of derivatives to precompute.
          ! this next call will define the jacobian and its derivatives (parameteric and spatial)
          ajrx = rsxy(i1,i2,i3,0,0)
          ajsx = rsxy(i1,i2,i3,1,0)
          ajtx = rsxy(i1,i2,i3,2,0)
          ajry = rsxy(i1,i2,i3,0,1)
          ajsy = rsxy(i1,i2,i3,1,1)
          ajty = rsxy(i1,i2,i3,2,1)
          ajrz = rsxy(i1,i2,i3,0,2)
          ajsz = rsxy(i1,i2,i3,1,2)
          ajtz = rsxy(i1,i2,i3,2,2)
         ! evaluate forward derivatives of the current solution: 
         ! First evaluate the parametric derivatives of u, used later by the spatial derivatives. 
         ! MAXDER = max number of parametric derivatives to precompute.
          uu = u(i1,i2,i3,uc)
          uur = (-u(i1-1,i2,i3,uc)+u(i1+1,i2,i3,uc))/(2.*dr(0))
          uus = (-u(i1,i2-1,i3,uc)+u(i1,i2+1,i3,uc))/(2.*dr(1))
          uut = (-u(i1,i2,i3-1,uc)+u(i1,i2,i3+1,uc))/(2.*dr(2))
         ! Evaluate the spatial derivatives of u:
           u0x = ajrx*uur+ajsx*uus+ajtx*uut
           u0y = ajry*uur+ajsy*uus+ajty*uut
         ! Evaluate the spatial derivatives of v:
          vv = u(i1,i2,i3,vc)
          vvr = (-u(i1-1,i2,i3,vc)+u(i1+1,i2,i3,vc))/(2.*dr(0))
          vvs = (-u(i1,i2-1,i3,vc)+u(i1,i2+1,i3,vc))/(2.*dr(1))
          vvt = (-u(i1,i2,i3-1,vc)+u(i1,i2,i3+1,vc))/(2.*dr(2))
           v0x = ajrx*vvr+ajsx*vvs+ajtx*vvt
           v0y = ajry*vvr+ajsy*vvs+ajty*vvt
         esr = (sqrt((2./3.)*(u0x**2+v0y**2+.5*(u0y+v0x)**2))+epsVP)
           u0z = ajrz*uur+ajsz*uus+ajtz*uut
           v0z = ajrz*vvr+ajsz*vvs+ajtz*vvt
          ww = u(i1,i2,i3,wc)
          wwr = (-u(i1-1,i2,i3,wc)+u(i1+1,i2,i3,wc))/(2.*dr(0))
          wws = (-u(i1,i2-1,i3,wc)+u(i1,i2+1,i3,wc))/(2.*dr(1))
          wwt = (-u(i1,i2,i3-1,wc)+u(i1,i2,i3+1,wc))/(2.*dr(2))
           w0x = ajrx*wwr+ajsx*wws+ajtx*wwt
           w0y = ajry*wwr+ajsy*wws+ajty*wwt
           w0z = ajrz*wwr+ajsz*wws+ajtz*wwt
         ! finish me for 3d
         stop 3916
        ! this next macro is in viscoPlasticMacros.h 
          exp0 = exp(-exponentVP*esr)
          visc(i1,i2,i3,0) = (etaVP + (yieldStressVP/esr)*(1.-exp0))
          end if
         end do
         end do
         end do
       else
        write(*,'("getViscosity:ERROR: unknown pdeModel=",i6)') 
     & pdeModel
       end if

      else if(  nd.eq.3 .and. gridType.eq.rectangular )then

       ! **** to do : optimize this for backward-Euler : fe=0, fi=0 !!
       if( pdeModel.eq.viscoPlasticModel )then
         do i3=n3a,n3b
         do i2=n2a,n2b
         do i1=n1a,n1b
          if( mask(i1,i2,i3).ne.0 )then
         ! Evaluate the jacobian derivatives used by the coefficient and forward derivatives:
         ! opEvalJacobianDerivatives(MAXDER) : MAXDER = max number of derivatives to precompute.
         ! evaluate forward derivatives of the current solution: 
         ! First evaluate the parametric derivatives of u, used later by the spatial derivatives. 
         ! MAXDER = max number of parametric derivatives to precompute.
          uu=u(i1,i2,i3,uc) ! in the rectangular case just eval the solution
         ! Evaluate the spatial derivatives of u:
           u0x = (-u(i1-1,i2,i3,uc)+u(i1+1,i2,i3,uc))/(2.*dx(0))
           u0y = (-u(i1,i2-1,i3,uc)+u(i1,i2+1,i3,uc))/(2.*dx(1))
         ! Evaluate the spatial derivatives of v:
          vv=u(i1,i2,i3,vc) ! in the rectangular case just eval the solution
           v0x = (-u(i1-1,i2,i3,vc)+u(i1+1,i2,i3,vc))/(2.*dx(0))
           v0y = (-u(i1,i2-1,i3,vc)+u(i1,i2+1,i3,vc))/(2.*dx(1))
         esr = (sqrt((2./3.)*(u0x**2+v0y**2+.5*(u0y+v0x)**2))+epsVP)
           u0z = (-u(i1,i2,i3-1,uc)+u(i1,i2,i3+1,uc))/(2.*dx(2))
           v0z = (-u(i1,i2,i3-1,vc)+u(i1,i2,i3+1,vc))/(2.*dx(2))
          ww=u(i1,i2,i3,wc) ! in the rectangular case just eval the solution
           w0x = (-u(i1-1,i2,i3,wc)+u(i1+1,i2,i3,wc))/(2.*dx(0))
           w0y = (-u(i1,i2-1,i3,wc)+u(i1,i2+1,i3,wc))/(2.*dx(1))
           w0z = (-u(i1,i2,i3-1,wc)+u(i1,i2,i3+1,wc))/(2.*dx(2))
         ! finish me for 3d
         stop 3916
        ! this next macro is in viscoPlasticMacros.h 
          exp0 = exp(-exponentVP*esr)
          visc(i1,i2,i3,0) = (etaVP + (yieldStressVP/esr)*(1.-exp0))
          end if
         end do
         end do
         end do
       else
        write(*,'("getViscosity:ERROR: unknown pdeModel=",i6)') 
     & pdeModel
       end if

      else
        stop 1709
      end if

      return
      end


