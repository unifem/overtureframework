! This file automatically generated from interface3d.bf with bpp.
c *******************************************************************************
c   Interface boundary conditions
c *******************************************************************************

c These next include files will define the macros that will define the difference approximations
c The actual macro is called below
c* #Include "defineDiffNewerOrder2f.h"
c* #Include "defineDiffNewerOrder4f.h"

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
c    defineParametricDerivativeMacros(u,dr,dx,DIM,ORDER,COMPONENTS,MAXDERIV)
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

! defineParametricDerivativeMacros(u,dr,dx,DIM,ORDER,COMPONENTS,MAXDERIV)
! 2D, order=6, components=1
! defineParametricDerivativeMacros(u,dr,dx,DIM,ORDER,COMPONENTS,MAXDERIV)

 ! *************** 0 components *************
 ! *************** 1 components *************
 ! *************** 2 components *************
 ! *************** 0 components *************
 ! *************** 1 components *************
 ! *************** 2 components *************
 ! *************** 0 components *************
 ! *************** 1 components *************
 ! *************** 2 components *************
 ! *************** 0 components *************
 ! *************** 1 components *************
 ! *************** 2 components *************

 ! *************** 0 components *************
 ! *************** 1 components *************
 ! *************** 2 components *************
 ! *************** 0 components *************
 ! *************** 1 components *************
 ! *************** 2 components *************
 ! *************** 0 components *************
 ! *************** 1 components *************
 ! *************** 2 components *************
 ! *************** 0 components *************
 ! *************** 1 components *************
 ! *************** 2 components *************

! ******************************************************************************************************************
! ************* These are altered version of those from insImp.h ***************************************************
! ******************************************************************************************************************


! ==========================================================================================
!  Evaluate the Jacobian and its derivatives (parametric and spatial). 
!    rsxy   : jacobian matrix name 
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

! ******************************************************************************************************************

! loop over the boundary points



! loop over the boundary points

! loop over the boundary points with a mask. 
! Assign pts where both mask1 and mask2 are discretization pts.
! If mask1>0 and mask2<0 then we just leave the extrapolated values in u1 and u2 .

! loop over the boundary points that includes ghost points in the tangential direction

! loop over the boundary points that includes ghost points in the tangential direction.
! Assign pts where both mask1 and mask2 are discretization pts.
! If mask1>0 and mask2<0 then we just leave the extrapolated values in u1 and u2 .


! Assign pts where both mask1 and mask2 are discretization pts.
! If mask1>0 and mask2<0 then we just leave the extrapolated values in u1 and u2 .







c This macro will assign the jump conditions on the boundary
c DIM (input): number of dimensions (2 or 3)
c GRIDTYPE (input) : curvilinear or rectangular

c ** Precompute the derivatives of rsxy ***
c assign rvx(m) = (rx,sy)
c        rvxx(m) = (rxx,sxx)
c* #beginMacro computeRxDerivatives(rv,rsxy,i1,i2,i3)
c* do m=0,nd-1
c*  rv ## x(m)   =rsxy(i1,i2,i3,m,0)
c*  rv ## y(m)   =rsxy(i1,i2,i3,m,1)
c* 
c*  rv ## xx(m)  =rsxy ## x22(i1,i2,i3,m,0)
c*  rv ## xy(m)  =rsxy ## x22(i1,i2,i3,m,1)
c*  rv ## yy(m)  =rsxy ## y22(i1,i2,i3,m,1)
c* 
c*  rv ## xxx(m) =rsxy ## xx22(i1,i2,i3,m,0)
c*  rv ## xxy(m) =rsxy ## xx22(i1,i2,i3,m,1)
c*  rv ## xyy(m) =rsxy ## xy22(i1,i2,i3,m,1)
c*  rv ## yyy(m) =rsxy ## yy22(i1,i2,i3,m,1)
c* 
c*  rv ## xxxx(m)=rsxy ## xxx22(i1,i2,i3,m,0)
c*  rv ## xxyy(m)=rsxy ## xyy22(i1,i2,i3,m,0)
c*  rv ## yyyy(m)=rsxy ## yyy22(i1,i2,i3,m,1)
c* end do
c* #endMacro
c* 
c* c assign some temporary variables that are used in the evaluation of the operators
c* #beginMacro setJacobian(rv,axis1,axisp1)
c*  rx   =rv ## x(axis1)   
c*  ry   =rv ## y(axis1)   
c*                     
c*  rxx  =rv ## xx(axis1)  
c*  rxy  =rv ## xy(axis1)  
c*  ryy  =rv ## yy(axis1)  
c*                     
c*  rxxx =rv ## xxx(axis1) 
c*  rxxy =rv ## xxy(axis1) 
c*  rxyy =rv ## xyy(axis1) 
c*  ryyy =rv ## yyy(axis1) 
c*                     
c*  rxxxx=rv ## xxxx(axis1)
c*  rxxyy=rv ## xxyy(axis1)
c*  ryyyy=rv ## yyyy(axis1)
c* 
c*  sx   =rv ## x(axis1p1)   
c*  sy   =rv ## y(axis1p1)   
c*                     
c*  sxx  =rv ## xx(axis1p1)  
c*  sxy  =rv ## xy(axis1p1)  
c*  syy  =rv ## yy(axis1p1)  
c*                     
c*  sxxx =rv ## xxx(axis1p1) 
c*  sxxy =rv ## xxy(axis1p1) 
c*  sxyy =rv ## xyy(axis1p1) 
c*  syyy =rv ## yyy(axis1p1) 
c*                     
c*  sxxxx=rv ## xxxx(axis1p1)
c*  sxxyy=rv ## xxyy(axis1p1)
c*  syyyy=rv ## yyyy(axis1p1)
c* 
c* #endMacro

! ********************************************************************************
!     Usage: setJacobianRS( aj1, r, s)
!            setJacobianRS( aj1, s, r)
! ********************************************************************************

! ***************************************************************************
! This macro will set the temp variables rx, rxx, ry, ryx, ...
! If axis=0 then
!   rx = ajrx
!   sx = ajsx
!    ...
!  else if axis=1
!    -- permute r <-> s 
!   rx = ajsx
!   sx = ajrx
!    ...
! ***************************************************************************

! ===================================================================================
!  Optimized periodic update: (only applied in serial)
!     update the periodic ghost points used by an interface on the grid face (side,axis)
! ===================================================================================

! ===================================================================================
!  Optimized periodic update:
!     update the periodic ghost points used by an interface on the grid face (side,axis)
! ===================================================================================


! ******************************************************************************
!   This next macro is called by other macros to evaluate the first and second derivatives
!   This macro assumes that opEvalJacobianDerivatives has been called
! ******************************************************************************


! *********************************************************************************
!   Evaluate derivatives for the 2nd-order 2D interface equations
! *********************************************************************************

! ******************************************************************************
!   This macro assumes that opEvalJacobianDerivatives has been called
! ******************************************************************************


! ******************************************************************************
!   This next macro is called by evalDerivs2dOrder4
!   This macro assumes that opEvalJacobianDerivatives has been called
! ******************************************************************************

! ******************************************************************************
!   Evaluate derivatives for the 4th-order 2D interface equations
! ******************************************************************************

! ******************************************************************************
!   Evaluate derivatives of the magnetic field for the 4th-order 2D interface equations
! ******************************************************************************

! ******************************************************************************
!   This next macro is called by other macros to evaluate the first and second derivatives
!   This macro assumes that opEvalJacobianDerivatives has been called
! ******************************************************************************


! *********************************************************************************
!   Evaluate derivatives for the 2nd-order 3D interface equations
! *********************************************************************************



      subroutine interface3dMaxwell( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     & gridIndexRange1, u1, mask1,rsxy1, xy1, boundaryCondition1, 
     & md1a,md1b,md2a,md2b,md3a,md3b,gridIndexRange2, u2, mask2,rsxy2,
     &  xy2, boundaryCondition2, ipar, rpar, aa2,aa4,aa8, ipvt2,ipvt4,
     & ipvt8, ierr )
c ===================================================================================
c  Interface boundary conditions for Maxwells Equations in 3D.
c
c  gridType : 0=rectangular, 1=curvilinear
c
c  u1: solution on the "left" of the interface
c  u2: solution on the "right" of the interface
c
c  aa2,aa4,aa8 : real work space arrays that must be saved from call to call
c  ipvt2,ipvt4,ipvt8: integer work space arrays that must be saved from call to call
c ===================================================================================

      implicit none

      integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, md1a,md1b,md2a,md2b,
     & md3a,md3b, n1a,n1b,n2a,n2b,n3a,n3b,  m1a,m1b,m2a,m2b,m3a,m3b,  
     & ierr

      real u1(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      integer mask1(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real rsxy1(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real xy1(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      integer gridIndexRange1(0:1,0:2),boundaryCondition1(0:1,0:2)

      real u2(md1a:md1b,md2a:md2b,md3a:md3b,0:*)
      integer mask2(md1a:md1b,md2a:md2b,md3a:md3b)
      real rsxy2(md1a:md1b,md2a:md2b,md3a:md3b,0:nd-1,0:nd-1)
      real xy2(md1a:md1b,md2a:md2b,md3a:md3b,0:nd-1)
      integer gridIndexRange2(0:1,0:2),boundaryCondition2(0:1,0:2)

      integer ipar(0:*)
      real rpar(0:*)

      ! work space arrays that must be saved from call to call:
      real aa2(0:1,0:1,0:1,0:*),aa4(0:3,0:3,0:1,0:*),aa8(0:7,0:7,0:1,
     & 0:*)
      integer ipvt2(0:1,0:*), ipvt4(0:3,0:*), ipvt8(0:7,0:*)

c     --- local variables ----

      integer side1,axis1,grid1,side2,axis2,grid2,gridType,
     & orderOfAccuracy,orderOfExtrapolation,useForcing,ex,ey,ez,hx,hy,
     & hz,useWhereMask,debug,solveForE,solveForH,axis1p1,axis1p2,
     & axis2p1,axis2p2,nn,n1,n2,twilightZone
      real dx1(0:2),dr1(0:2),dx2(0:2),dr2(0:2)
c      real dx(0:2),dr(0:2)
      real t,ep,dt,eps1,mu1,c1,eps2,mu2,c2,epsmu1,epsmu2
      integer axisp1,axisp2,i1,i2,i3,is1,is2,is3,j1,j2,j3,js1,js2,js3,
     & ks1,ks2,ks3,is,js,it,nit,k1,k2,k3
      integer interfaceOption,interfaceEquationsOption,initialized,
     & forcingOption

      integer numGhost,giveDiv
      integer nn1a,nn1b,nn2a,nn2b,nn3a,nn3b
      integer mm1a,mm1b,mm2a,mm2b,mm3a,mm3b
      integer m1,m2

      real rx1,ry1,rz1,rx2,ry2,rz2

      real aLap0,aLap1,bLap0,bLap1,aLapX0,aLapX1,bLapY0,bLapY1,cLapX0,
     & cLapX1,dLapY0,dLapY1,aLapSq0,aLapSq1,bLapSq0,bLapSq1
      real a0,a1,b0,b1,cc0,cc1,d0,d1,dr0,ds0
      real aNormSq,divu

      real epsRatio,an1,an2,an3,aNorm,ua,ub,uc,nDotU
      real epsx

      real tau1,tau2,clap1,clap2,u1Lap,v1Lap,w1Lap,u2Lap,v2Lap,w2Lap,
     & an1Cartesian,an2Cartesian,an3Cartesian
      real u1LapSq,v1LapSq,u2LapSq,v2LapSq,w1LapSq,w2LapSq


      integer np1a,np1b,np2a,np2b,np3a,np3b,diff(0:2)

      real rx,ry,rxx,rxy,ryy,rxxx,rxxy,rxyy,ryyy,rxxxx,rxxyy,ryyyy
      real sx,sy,sxx,sxy,syy,sxxx,sxxy,sxyy,syyy,sxxxx,sxxyy,syyyy

c     real rv1x(0:2),rv1y(0:2),rv1xx(0:2),rv1xy(0:2),rv1yy(0:2),rv1xxx(0:2),rv1xxy(0:2),rv1xyy(0:2),rv1yyy(0:2),c          rv1xxxx(0:2),rv1xxyy(0:2),rv1yyyy(0:2)
c     real sv1x(0:2),sv1y(0:2),sv1xx(0:2),sv1xy(0:2),sv1yy(0:2),sv1xxx(0:2),sv1xxy(0:2),sv1xyy(0:2),sv1yyy(0:2),c          sv1xxxx(0:2),sv1xxyy(0:2),sv1yyyy(0:2)
c     real rv2x(0:2),rv2y(0:2),rv2xx(0:2),rv2xy(0:2),rv2yy(0:2),rv2xxx(0:2),rv2xxy(0:2),rv2xyy(0:2),rv2yyy(0:2),c          rv2xxxx(0:2),rv2xxyy(0:2),rv2yyyy(0:2)
c     real sv2x(0:2),sv2y(0:2),sv2xx(0:2),sv2xy(0:2),sv2yy(0:2),sv2xxx(0:2),sv2xxy(0:2),sv2xyy(0:2),sv2yyy(0:2),c          sv2xxxx(0:2),sv2xxyy(0:2),sv2yyyy(0:2)

      integer numberOfEquations,job
      real a2(0:1,0:1),a4(0:3,0:3),a6(0:5,0:5),a8(0:7,0:7),a12(0:11,
     & 0:11),q(0:11),f(0:11),rcond,work(0:11)
      integer ipvt(0:11)

      real err
      integer debugFile,myid,parallel
      character*20 debugFileName

      ! for new evaluation method:
      real u1x,u1y,u1z,u1xx,u1xy,u1yy,u1xz,u1yz,u1zz
      real u2x,u2y,u2z,u2xx,u2xy,u2yy,u2xz,u2yz,u2zz

      real v1x,v1y,v1z,v1xx,v1xy,v1yy,v1xz,v1yz,v1zz
      real v2x,v2y,v2z,v2xx,v2xy,v2yy,v2xz,v2yz,v2zz

      real w1x,w1y,w1z,w1xx,w1xy,w1yy,w1xz,w1yz,w1zz
      real w2x,w2y,w2z,w2xx,w2xy,w2yy,w2xz,w2yz,w2zz

      real u1xxx,u1xxy,u1xyy,u1yyy, u1xxz,u1xzz,u1zzz, u1yyz, u1yzz
      real u2xxx,u2xxy,u2xyy,u2yyy, u2xxz,u2xzz,u2zzz, u2yyz, u2yzz
      real v1xxx,v1xxy,v1xyy,v1yyy, v1xxz,v1xzz,v1zzz, v1yyz, v1yzz
      real v2xxx,v2xxy,v2xyy,v2yyy, v2xxz,v2xzz,v2zzz, v2yyz, v2yzz
      real w1xxx,w1xxy,w1xyy,w1yyy, w1xxz,w1xzz,w1zzz, w1yyz, w1yzz
      real w2xxx,w2xxy,w2xyy,w2yyy, w2xxz,w2xzz,w2zzz, w2yyz, w2yzz

      real u1xxxx,u1xxyy,u1yyyy, u1xxzz,u1zzzz, u1yyzz
      real u2xxxx,u2xxyy,u2yyyy, u2xxzz,u2zzzz, u2yyzz
      real v1xxxx,v1xxyy,v1yyyy, v1xxzz,v1zzzz, v1yyzz
      real v2xxxx,v2xxyy,v2yyyy, v2xxzz,v2zzzz, v2yyzz
      real w1xxxx,w1xxyy,w1yyyy, w1xxzz,w1zzzz, w1yyzz
      real w2xxxx,w2xxyy,w2yyyy, w2xxzz,w2zzzz, w2yyzz

      real rxx1(0:2,0:2,0:2), rxx2(0:2,0:2,0:2)

      real dx112(0:2),dx122(0:2),dx212(0:2),dx222(0:2),dx141(0:2),
     & dx142(0:2),dx241(0:2),dx242(0:2)
      real dr114(0:2),dr214(0:2)

      real cem1,divE1,curlE1x,curlE1y,curlE1z,nDotCurlE1,nDotLapE1
      real cem2,divE2,curlE2x,curlE2y,curlE2z,nDotCurlE2,nDotLapE2
      real c1x,c1y,c1z
      real c2x,c2y,c2z

      ! these are for the exact solution from TZ flow: 
      real ue,ve,we
      real uex,uey,uez, vex,vey,vez, wex,wey,wez, hex,hey,hez
      real uexx,ueyy,uezz, vexx,veyy,vezz, wexx,weyy,wezz
      real ueLap, veLap, weLap
      real curlEex,curlEey,curlEez,nDotCurlEe,nDotLapEe
      real uexxx,uexxy,uexyy,ueyyy
      real vexxx,vexxy,vexyy,veyyy
      real wexxx,wexxy,wexyy,weyyy
      real uexxxx,uexxyy,ueyyyy,ueLapSq
      real vexxxx,vexxyy,veyyyy,veLapSq
      real wexxxx,wexxyy,weyyyy,weLapSq

      ! boundary conditions parameters
! define BC parameters for fortran routines
! boundary conditions
      integer dirichlet,perfectElectricalConductor,
     & perfectMagneticConductor,planeWaveBoundaryCondition,
     & interfaceBC,symmetryBoundaryCondition,abcEM2,abcPML,abc3,abc4,
     & abc5,rbcNonLocal,rbcLocal,lastBC
      parameter( dirichlet=1,perfectElectricalConductor=2,
     & perfectMagneticConductor=3,planeWaveBoundaryCondition=4,
     & symmetryBoundaryCondition=5,interfaceBC=6,abcEM2=7,abcPML=8,
     & abc3=9,abc4=10,abc5=11,rbcNonLocal=12,rbcLocal=13,lastBC=13 )

      integer rectangular,curvilinear
      parameter(rectangular=0,curvilinear=1)


c     --- start statement function ----
      integer kd,m,n
c     real rx,ry,rz,sx,sy,sz,tx,ty,tz
c*      declareDifferenceNewOrder2(u1,rsxy1,dr1,dx1,RX)
c*      declareDifferenceNewOrder2(u2,rsxy2,dr2,dx2,RX)

c*      declareDifferenceNewOrder4(u1,rsxy1,dr1,dx1,RX)
c*      declareDifferenceNewOrder4(u2,rsxy2,dr2,dx2,RX)

c.......statement functions for jacobian
c     rx(i1,i2,i3)=rsxy1(i1,i2,i3,0,0)
c     ry(i1,i2,i3)=rsxy1(i1,i2,i3,0,1)
c     rz(i1,i2,i3)=rsxy1(i1,i2,i3,0,2)
c     sx(i1,i2,i3)=rsxy1(i1,i2,i3,1,0)
c     sy(i1,i2,i3)=rsxy1(i1,i2,i3,1,1)
c     sz(i1,i2,i3)=rsxy1(i1,i2,i3,1,2)
c     tx(i1,i2,i3)=rsxy1(i1,i2,i3,2,0)
c     ty(i1,i2,i3)=rsxy1(i1,i2,i3,2,1)
c     tz(i1,i2,i3)=rsxy1(i1,i2,i3,2,2) 


c     The next macro call will define the difference approximation statement functions
c*      defineDifferenceNewOrder2Components1(u1,rsxy1,dr1,dx1,RX)
c*      defineDifferenceNewOrder2Components1(u2,rsxy2,dr2,dx2,RX)

c*      defineDifferenceNewOrder4Components1(u1,rsxy1,dr1,dx1,RX)
c*      defineDifferenceNewOrder4Components1(u2,rsxy2,dr2,dx2,RX)

      real t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,
     & t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t28,t29,t30,t31,
     & t32,t33,t34,t35,t36,t37,t38,t39,t40,t41,t42,t43,t44,t45,t46,
     & t47,t48,t49,t50,t51,t52,t53,t54,t55,t56,t57,t58,t59,t60,t61,
     & t62,t63,t64,t65,t66,t67,t68,t69,t70,t71,t72,t73,t74,t75,t76,
     & t77,t78,t79,t80,t81,t82,t83,t84,t85,t86,t87,t88,t89,t90,t91,
     & t92,t93,t94,t95,t96,t97,t98,t99,t100,t101,t102,t103,t104,t105,
     & t106,t107,t108,t109,t110,t111,t112,t113,t114,t115,t116,t117,
     & t118,t119,t120,t121,t122,t123,t124,t125,t126,t127,t128,t129,
     & t130,t131,t132,t133,t134,t135,t136,t137,t138,t139,t140,t141,
     & t142,t143,t144,t145,t146,t147,t148,t149,t150,t151,t152,t153,
     & t154,t155,t156,t157,t158,t159,t160,t161,t162,t163,t164,t165,
     & t166,t167,t168,t169,t170,t171,t172,t173,t174,t175,t176,t177,
     & t178,t179,t180,t181,t182,t183,t184,t185,t186,t187,t188,t189,
     & t190,t191,t192,t193,t194,t195,t196,t197,t198,t199,t200,t201,
     & t202,t203,t204,t205,t206,t207,t208,t209,t210,t211,t212,t213,
     & t214,t215,t216,t217,t218,t219,t220,t221,t222,t223,t224,t225,
     & t226,t227,t228,t229,t230,t231,t232,t233,t234,t235,t236,t237,
     & t238,t239,t240,t241,t242,t243,t244,t245,t246,t247,t248,t249,
     & t250,t251,t252,t253,t254,t255,t256,t257,t258,t259,t260,t261,
     & t262,t263,t264,t265,t266,t267,t268,t269,t270,t271,t272,t273,
     & t274,t275,t276,t277,t278,t279,t280,t281,t282,t283,t284,t285,
     & t286,t287,t288,t289,t290,t291,t292,t293,t294,t295,t296,t297,
     & t298,t299,t300,t301,t302,t303,t304,t305,t306,t307,t308,t309,
     & t310,t311,t312,t313,t314,t315,t316,t317,t318,t319,t320,t321,
     & t322,t323,t324,t325,t326,t327,t328,t329,t330,t331,t332,t333,
     & t334,t335,t336,t337,t338,t339,t340,t341,t342,t343,t344,t345,
     & t346,t347,t348,t349,t350,t351,t352,t353,t354,t355,t356,t357,
     & t358,t359,t360,t361,t362,t363,t364,t365,t366,t367,t368,t369,
     & t370,t371,t372,t373,t374,t375,t376,t377,t378,t379,t380,t381,
     & t382,t383,t384,t385,t386,t387,t388,t389,t390,t391,t392,t393,
     & t394,t395,t396,t397,t398,t399,t400,t401,t402,t403,t404,t405,
     & t406,t407,t408,t409,t410,t411,t412,t413,t414,t415,t416,t417,
     & t418,t419,t420,t421,t422,t423,t424,t425,t426,t427,t428,t429,
     & t430,t431,t432,t433,t434,t435,t436,t437,t438,t439,t440,t441,
     & t442,t443,t444,t445,t446,t447,t448,t449,t450,t451,t452,t453,
     & t454,t455,t456,t457,t458,t459,t460,t461,t462,t463,t464,t465,
     & t466,t467,t468,t469,t470,t471,t472,t473,t474,t475,t476,t477,
     & t478,t479,t480,t481,t482,t483,t484,t485,t486,t487,t488,t489,
     & t490,t491,t492,t493,t494,t495,t496,t497,t498,t499,t500,t501,
     & t502,t503,t504,t505,t506,t507,t508,t509,t510,t511,t512,t513,
     & t514,t515,t516,t517,t518,t519,t520,t521,t522,t523,t524,t525,
     & t526,t527,t528,t529,t530,t531,t532,t533,t534,t535,t536,t537,
     & t538,t539,t540,t541,t542,t543,t544,t545,t546,t547,t548,t549,
     & t550,t551,t552,t553,t554,t555,t556,t557,t558,t559,t560,t561,
     & t562,t563,t564,t565,t566,t567,t568,t569,t570,t571,t572,t573,
     & t574,t575,t576,t577,t578,t579,t580,t581,t582,t583,t584,t585,
     & t586,t587,t588,t589,t590,t591,t592,t593,t594,t595,t596,t597,
     & t598,t599,t600,t601,t602,t603,t604,t605,t606,t607,t608,t609,
     & t610,t611,t612,t613,t614,t615,t616,t617,t618,t619,t620,t621,
     & t622,t623,t624,t625,t626,t627,t628,t629,t630,t631,t632,t633,
     & t634,t635,t636,t637,t638,t639,t640,t641,t642,t643,t644,t645,
     & t646,t647,t648,t649,t650,t651,t652,t653,t654,t655,t656,t657,
     & t658,t659,t660,t661,t662,t663,t664,t665,t666,t667,t668,t669,
     & t670,t671,t672,t673,t674,t675,t676,t677,t678,t679,t680,t681,
     & t682,t683,t684,t685,t686,t687,t688,t689,t690,t691,t692,t693,
     & t694,t695,t696,t697,t698,t699,t700,t701,t702,t703,t704,t705,
     & t706,t707,t708,t709,t710,t711,t712,t713,t714,t715,t716,t717,
     & t718,t719,t720,t721,t722,t723,t724,t725,t726,t727,t728,t729,
     & t730,t731,t732,t733,t734,t735,t736,t737,t738,t739,t740,t741,
     & t742,t743,t744,t745,t746,t747,t748,t749,t750,t751,t752,t753,
     & t754,t755,t756,t757,t758,t759,t760,t761,t762,t763,t764,t765,
     & t766,t767,t768,t769,t770,t771,t772,t773,t774,t775,t776,t777,
     & t778,t779,t780,t781,t782,t783,t784,t785,t786,t787,t788,t789,
     & t790,t791,t792,t793,t794,t795,t796,t797,t798,t799,t800,t801,
     & t802,t803,t804,t805,t806,t807,t808,t809,t810,t811,t812,t813,
     & t814,t815,t816,t817,t818,t819,t820,t821,t822,t823,t824,t825,
     & t826,t827,t828,t829,t830,t831,t832,t833,t834,t835,t836,t837,
     & t838,t839,t840,t841,t842,t843,t844,t845,t846,t847,t848,t849,
     & t850,t851,t852,t853,t854,t855,t856,t857,t858,t859,t860,t861,
     & t862,t863,t864,t865,t866,t867,t868,t869,t870,t871,t872,t873,
     & t874,t875,t876,t877,t878,t879,t880,t881,t882,t883,t884,t885,
     & t886,t887,t888,t889,t890,t891,t892,t893,t894,t895,t896,t897,
     & t898,t899,t900,t901,t902,t903,t904,t905,t906,t907,t908,t909,
     & t910,t911,t912,t913,t914,t915,t916,t917,t918,t919,t920,t921,
     & t922,t923,t924,t925,t926,t927,t928,t929,t930,t931,t932,t933,
     & t934,t935,t936,t937,t938,t939,t940,t941,t942,t943,t944,t945,
     & t946,t947,t948,t949,t950,t951,t952,t953,t954,t955,t956,t957,
     & t958,t959,t960,t961,t962,t963,t964,t965,t966,t967,t968,t969,
     & t970,t971,t972,t973,t974,t975,t976,t977,t978,t979,t980,t981,
     & t982,t983,t984,t985,t986,t987,t988,t989,t990,t991,t992,t993,
     & t994,t995,t996,t997,t998,t999,t1000,t1001,t1002,t1003,t1004,
     & t1005,t1006,t1007,t1008,t1009,t1010,t1011,t1012,t1013,t1014,
     & t1015,t1016,t1017,t1018,t1019,t1020,t1021,t1022,t1023,t1024,
     & t1025,t1026,t1027,t1028,t1029,t1030,t1031,t1032,t1033,t1034,
     & t1035,t1036,t1037,t1038,t1039,t1040,t1041,t1042,t1043,t1044,
     & t1045,t1046,t1047,t1048,t1049,t1050,t1051,t1052,t1053,t1054,
     & t1055,t1056,t1057,t1058,t1059,t1060,t1061,t1062,t1063,t1064,
     & t1065,t1066,t1067,t1068,t1069,t1070,t1071,t1072,t1073,t1074,
     & t1075,t1076,t1077,t1078,t1079,t1080,t1081,t1082,t1083,t1084,
     & t1085,t1086,t1087,t1088,t1089,t1090,t1091,t1092,t1093,t1094,
     & t1095,t1096,t1097,t1098,t1099,t1100,t1101,t1102,t1103,t1104,
     & t1105,t1106,t1107,t1108,t1109,t1110,t1111,t1112,t1113,t1114,
     & t1115,t1116,t1117,t1118,t1119,t1120,t1121,t1122,t1123,t1124,
     & t1125,t1126,t1127,t1128,t1129,t1130,t1131,t1132,t1133,t1134,
     & t1135,t1136,t1137,t1138,t1139,t1140,t1141,t1142,t1143,t1144,
     & t1145,t1146,t1147,t1148,t1149,t1150,t1151,t1152,t1153,t1154,
     & t1155,t1156,t1157,t1158,t1159,t1160,t1161,t1162,t1163,t1164,
     & t1165,t1166,t1167,t1168,t1169,t1170,t1171,t1172,t1173,t1174,
     & t1175,t1176,t1177,t1178,t1179,t1180,t1181,t1182,t1183,t1184,
     & t1185,t1186,t1187,t1188,t1189,t1190,t1191,t1192,t1193,t1194,
     & t1195,t1196,t1197,t1198,t1199,t1200,t1201,t1202,t1203,t1204,
     & t1205,t1206,t1207,t1208,t1209,t1210,t1211,t1212,t1213,t1214,
     & t1215,t1216,t1217,t1218,t1219,t1220,t1221,t1222,t1223,t1224,
     & t1225,t1226,t1227,t1228,t1229,t1230,t1231,t1232,t1233,t1234,
     & t1235,t1236,t1237,t1238,t1239,t1240,t1241,t1242,t1243,t1244,
     & t1245,t1246,t1247,t1248,t1249,t1250,t1251,t1252,t1253,t1254,
     & t1255,t1256,t1257,t1258,t1259,t1260,t1261,t1262,t1263,t1264,
     & t1265,t1266,t1267,t1268,t1269,t1270,t1271,t1272,t1273,t1274,
     & t1275,t1276,t1277,t1278,t1279,t1280,t1281,t1282,t1283,t1284,
     & t1285,t1286,t1287,t1288,t1289,t1290,t1291,t1292,t1293,t1294,
     & t1295,t1296,t1297,t1298,t1299,t1300,t1301,t1302,t1303,t1304,
     & t1305,t1306,t1307,t1308,t1309,t1310,t1311,t1312,t1313,t1314,
     & t1315,t1316,t1317,t1318,t1319,t1320,t1321,t1322,t1323,t1324,
     & t1325,t1326,t1327,t1328,t1329,t1330,t1331,t1332,t1333,t1334,
     & t1335,t1336,t1337,t1338,t1339,t1340,t1341,t1342,t1343,t1344,
     & t1345,t1346,t1347,t1348,t1349,t1350,t1351,t1352,t1353,t1354,
     & t1355,t1356,t1357,t1358,t1359,t1360,t1361,t1362,t1363,t1364,
     & t1365,t1366,t1367,t1368,t1369,t1370,t1371,t1372,t1373,t1374,
     & t1375,t1376,t1377,t1378,t1379,t1380,t1381,t1382,t1383,t1384,
     & t1385,t1386,t1387,t1388,t1389,t1390,t1391,t1392,t1393,t1394,
     & t1395,t1396,t1397,t1398,t1399,t1400,t1401,t1402,t1403,t1404,
     & t1405,t1406,t1407,t1408,t1409,t1410,t1411,t1412,t1413,t1414,
     & t1415,t1416,t1417,t1418,t1419,t1420,t1421,t1422,t1423,t1424,
     & t1425,t1426,t1427,t1428,t1429,t1430,t1431,t1432,t1433,t1434,
     & t1435,t1436,t1437,t1438,t1439,t1440,t1441,t1442,t1443,t1444,
     & t1445,t1446,t1447,t1448,t1449,t1450,t1451,t1452,t1453,t1454,
     & t1455,t1456,t1457,t1458,t1459,t1460,t1461,t1462,t1463,t1464,
     & t1465,t1466,t1467,t1468,t1469,t1470,t1471,t1472,t1473,t1474,
     & t1475,t1476,t1477,t1478,t1479,t1480,t1481,t1482,t1483,t1484,
     & t1485,t1486,t1487,t1488,t1489,t1490,t1491,t1492,t1493,t1494,
     & t1495,t1496,t1497,t1498,t1499,t1500,t1501,t1502,t1503,t1504,
     & t1505,t1506,t1507,t1508,t1509,t1510,t1511,t1512,t1513,t1514,
     & t1515,t1516,t1517,t1518,t1519,t1520,t1521,t1522,t1523,t1524,
     & t1525,t1526,t1527,t1528,t1529,t1530,t1531,t1532,t1533,t1534,
     & t1535,t1536,t1537,t1538,t1539,t1540,t1541,t1542,t1543,t1544,
     & t1545,t1546,t1547,t1548,t1549,t1550,t1551,t1552,t1553,t1554,
     & t1555,t1556,t1557,t1558,t1559,t1560,t1561,t1562,t1563,t1564,
     & t1565,t1566,t1567,t1568,t1569,t1570,t1571,t1572,t1573,t1574,
     & t1575,t1576,t1577,t1578,t1579,t1580,t1581,t1582,t1583,t1584,
     & t1585,t1586,t1587,t1588,t1589,t1590,t1591,t1592,t1593,t1594,
     & t1595,t1596,t1597,t1598,t1599,t1600,t1601,t1602,t1603,t1604,
     & t1605,t1606,t1607,t1608,t1609,t1610,t1611,t1612,t1613,t1614,
     & t1615,t1616,t1617,t1618,t1619,t1620,t1621,t1622,t1623,t1624,
     & t1625,t1626,t1627,t1628,t1629,t1630,t1631,t1632,t1633,t1634,
     & t1635,t1636,t1637,t1638,t1639,t1640,t1641,t1642,t1643,t1644,
     & t1645,t1646,t1647,t1648,t1649,t1650,t1651,t1652,t1653,t1654,
     & t1655,t1656,t1657,t1658,t1659,t1660,t1661,t1662,t1663,t1664,
     & t1665,t1666,t1667,t1668,t1669,t1670,t1671,t1672,t1673,t1674,
     & t1675,t1676,t1677,t1678,t1679,t1680,t1681,t1682,t1683,t1684,
     & t1685,t1686,t1687,t1688,t1689,t1690,t1691,t1692,t1693,t1694,
     & t1695,t1696,t1697,t1698,t1699,t1700,t1701,t1702,t1703,t1704,
     & t1705,t1706,t1707,t1708,t1709,t1710,t1711,t1712,t1713,t1714,
     & t1715,t1716,t1717,t1718,t1719,t1720,t1721,t1722,t1723,t1724,
     & t1725,t1726,t1727,t1728,t1729,t1730,t1731,t1732,t1733,t1734,
     & t1735,t1736,t1737,t1738,t1739,t1740,t1741,t1742,t1743,t1744,
     & t1745,t1746,t1747,t1748,t1749,t1750,t1751,t1752,t1753,t1754,
     & t1755,t1756,t1757,t1758,t1759,t1760,t1761,t1762,t1763,t1764,
     & t1765,t1766,t1767,t1768,t1769,t1770,t1771,t1772,t1773,t1774,
     & t1775,t1776,t1777,t1778,t1779,t1780,t1781,t1782,t1783,t1784,
     & t1785,t1786,t1787,t1788,t1789,t1790,t1791,t1792,t1793,t1794,
     & t1795,t1796,t1797,t1798,t1799,t1800,t1801,t1802,t1803,t1804,
     & t1805,t1806,t1807,t1808,t1809,t1810,t1811,t1812,t1813,t1814,
     & t1815,t1816,t1817,t1818,t1819,t1820,t1821,t1822,t1823,t1824,
     & t1825,t1826,t1827,t1828,t1829,t1830,t1831,t1832,t1833,t1834,
     & t1835,t1836,t1837,t1838,t1839,t1840,t1841,t1842,t1843,t1844,
     & t1845,t1846,t1847,t1848,t1849,t1850,t1851,t1852,t1853,t1854,
     & t1855,t1856,t1857,t1858,t1859,t1860,t1861,t1862,t1863,t1864,
     & t1865,t1866,t1867,t1868,t1869,t1870,t1871,t1872,t1873,t1874,
     & t1875,t1876,t1877,t1878,t1879,t1880,t1881,t1882,t1883,t1884,
     & t1885,t1886,t1887,t1888,t1889,t1890,t1891,t1892,t1893,t1894,
     & t1895,t1896,t1897,t1898,t1899,t1900,t1901,t1902,t1903,t1904,
     & t1905,t1906,t1907,t1908,t1909,t1910,t1911,t1912,t1913,t1914,
     & t1915,t1916,t1917,t1918,t1919,t1920,t1921,t1922,t1923,t1924,
     & t1925,t1926,t1927,t1928,t1929,t1930,t1931,t1932,t1933,t1934,
     & t1935,t1936,t1937,t1938,t1939,t1940,t1941,t1942,t1943,t1944,
     & t1945,t1946,t1947,t1948,t1949,t1950,t1951,t1952,t1953,t1954,
     & t1955,t1956,t1957,t1958,t1959,t1960,t1961,t1962,t1963,t1964,
     & t1965,t1966,t1967,t1968,t1969,t1970,t1971,t1972,t1973,t1974,
     & t1975,t1976,t1977,t1978,t1979,t1980,t1981,t1982,t1983,t1984,
     & t1985,t1986,t1987,t1988,t1989,t1990,t1991,t1992,t1993,t1994,
     & t1995,t1996,t1997,t1998,t1999,t2000,t2001,t2002,t2003,t2004,
     & t2005,t2006,t2007,t2008,t2009,t2010,t2011,t2012,t2013,t2014,
     & t2015,t2016,t2017,t2018,t2019,t2020,t2021,t2022,t2023,t2024,
     & t2025,t2026,t2027,t2028,t2029,t2030,t2031,t2032,t2033,t2034,
     & t2035,t2036,t2037,t2038,t2039,t2040,t2041,t2042,t2043,t2044,
     & t2045,t2046,t2047,t2048,t2049,t2050,t2051,t2052,t2053,t2054,
     & t2055,t2056,t2057,t2058,t2059,t2060,t2061,t2062,t2063,t2064,
     & t2065,t2066,t2067,t2068,t2069,t2070,t2071,t2072,t2073,t2074,
     & t2075,t2076,t2077,t2078,t2079,t2080,t2081,t2082,t2083,t2084,
     & t2085,t2086,t2087,t2088,t2089,t2090,t2091,t2092,t2093,t2094,
     & t2095,t2096,t2097,t2098,t2099,t2100,t2101,t2102,t2103,t2104,
     & t2105,t2106,t2107,t2108,t2109,t2110,t2111,t2112,t2113,t2114,
     & t2115,t2116,t2117,t2118,t2119,t2120,t2121,t2122,t2123,t2124,
     & t2125,t2126,t2127,t2128,t2129,t2130,t2131,t2132,t2133,t2134,
     & t2135,t2136,t2137,t2138,t2139,t2140,t2141,t2142,t2143,t2144,
     & t2145,t2146,t2147,t2148,t2149,t2150,t2151,t2152,t2153,t2154,
     & t2155,t2156,t2157,t2158,t2159,t2160,t2161,t2162,t2163,t2164,
     & t2165,t2166,t2167,t2168,t2169,t2170,t2171,t2172,t2173,t2174,
     & t2175,t2176,t2177,t2178,t2179,t2180,t2181,t2182,t2183,t2184,
     & t2185,t2186,t2187,t2188,t2189,t2190,t2191,t2192,t2193,t2194,
     & t2195,t2196,t2197,t2198,t2199,t2200,t2201,t2202,t2203,t2204,
     & t2205,t2206,t2207,t2208,t2209,t2210,t2211,t2212,t2213,t2214,
     & t2215,t2216,t2217,t2218,t2219,t2220,t2221,t2222,t2223,t2224,
     & t2225,t2226,t2227,t2228,t2229,t2230,t2231,t2232,t2233,t2234,
     & t2235,t2236,t2237,t2238,t2239,t2240,t2241,t2242,t2243,t2244,
     & t2245,t2246,t2247,t2248,t2249,t2250,t2251,t2252,t2253,t2254,
     & t2255,t2256,t2257,t2258,t2259,t2260,t2261,t2262,t2263,t2264,
     & t2265,t2266,t2267,t2268,t2269,t2270,t2271,t2272,t2273,t2274,
     & t2275,t2276,t2277,t2278,t2279,t2280,t2281,t2282,t2283,t2284,
     & t2285,t2286,t2287,t2288,t2289,t2290,t2291,t2292,t2293,t2294,
     & t2295,t2296,t2297,t2298,t2299,t2300,t2301,t2302,t2303,t2304,
     & t2305,t2306,t2307,t2308,t2309,t2310,t2311,t2312,t2313,t2314,
     & t2315,t2316,t2317,t2318,t2319,t2320,t2321,t2322,t2323,t2324,
     & t2325,t2326,t2327,t2328,t2329,t2330,t2331,t2332,t2333,t2334,
     & t2335,t2336,t2337,t2338,t2339,t2340,t2341,t2342,t2343,t2344,
     & t2345,t2346,t2347,t2348,t2349,t2350,t2351,t2352,t2353,t2354,
     & t2355,t2356,t2357,t2358,t2359,t2360,t2361,t2362,t2363,t2364,
     & t2365,t2366,t2367,t2368,t2369,t2370,t2371,t2372,t2373,t2374,
     & t2375,t2376,t2377,t2378,t2379,t2380,t2381,t2382,t2383,t2384,
     & t2385,t2386,t2387,t2388,t2389,t2390,t2391,t2392,t2393,t2394,
     & t2395,t2396,t2397,t2398,t2399,t2400,t2401,t2402,t2403,t2404,
     & t2405,t2406,t2407,t2408,t2409,t2410,t2411,t2412,t2413,t2414,
     & t2415,t2416,t2417,t2418,t2419,t2420,t2421,t2422,t2423,t2424,
     & t2425,t2426,t2427,t2428,t2429,t2430,t2431,t2432,t2433,t2434,
     & t2435,t2436,t2437,t2438,t2439,t2440,t2441,t2442,t2443,t2444,
     & t2445,t2446,t2447,t2448,t2449,t2450,t2451,t2452,t2453,t2454,
     & t2455,t2456,t2457,t2458,t2459,t2460,t2461,t2462,t2463,t2464,
     & t2465,t2466,t2467,t2468,t2469,t2470,t2471,t2472,t2473,t2474,
     & t2475,t2476,t2477,t2478,t2479,t2480,t2481,t2482,t2483,t2484,
     & t2485,t2486,t2487,t2488,t2489,t2490,t2491,t2492,t2493,t2494,
     & t2495,t2496,t2497,t2498,t2499,t2500,t2501,t2502,t2503,t2504,
     & t2505,t2506,t2507,t2508,t2509,t2510,t2511,t2512,t2513,t2514,
     & t2515,t2516,t2517,t2518,t2519,t2520,t2521,t2522,t2523,t2524,
     & t2525,t2526,t2527,t2528,t2529,t2530,t2531,t2532,t2533,t2534,
     & t2535,t2536,t2537,t2538,t2539,t2540,t2541,t2542,t2543,t2544,
     & t2545,t2546,t2547,t2548,t2549,t2550,t2551,t2552,t2553,t2554,
     & t2555,t2556,t2557,t2558,t2559,t2560,t2561,t2562,t2563,t2564,
     & t2565,t2566,t2567,t2568,t2569,t2570,t2571,t2572,t2573,t2574,
     & t2575,t2576,t2577,t2578,t2579,t2580,t2581,t2582,t2583,t2584,
     & t2585,t2586,t2587,t2588,t2589,t2590,t2591,t2592,t2593,t2594,
     & t2595,t2596,t2597,t2598,t2599,t2600,t2601,t2602,t2603,t2604,
     & t2605,t2606,t2607,t2608,t2609,t2610,t2611,t2612,t2613,t2614,
     & t2615,t2616,t2617,t2618,t2619,t2620,t2621,t2622,t2623,t2624,
     & t2625,t2626,t2627,t2628,t2629,t2630,t2631,t2632,t2633,t2634,
     & t2635,t2636,t2637,t2638,t2639,t2640,t2641,t2642,t2643,t2644,
     & t2645,t2646,t2647,t2648,t2649,t2650,t2651,t2652,t2653,t2654,
     & t2655,t2656,t2657,t2658,t2659,t2660,t2661,t2662,t2663,t2664,
     & t2665,t2666,t2667,t2668,t2669,t2670,t2671,t2672,t2673,t2674,
     & t2675,t2676,t2677,t2678,t2679,t2680,t2681,t2682,t2683,t2684,
     & t2685,t2686,t2687,t2688,t2689,t2690,t2691,t2692,t2693,t2694,
     & t2695,t2696,t2697,t2698,t2699,t2700,t2701,t2702,t2703,t2704,
     & t2705,t2706,t2707,t2708,t2709,t2710,t2711,t2712,t2713,t2714,
     & t2715,t2716,t2717,t2718,t2719,t2720,t2721,t2722,t2723,t2724,
     & t2725,t2726,t2727,t2728,t2729,t2730,t2731,t2732,t2733,t2734,
     & t2735,t2736,t2737,t2738,t2739,t2740,t2741,t2742,t2743,t2744,
     & t2745,t2746,t2747,t2748,t2749,t2750,t2751,t2752,t2753,t2754,
     & t2755,t2756,t2757,t2758,t2759,t2760,t2761,t2762,t2763,t2764,
     & t2765,t2766,t2767,t2768,t2769,t2770,t2771,t2772,t2773,t2774,
     & t2775,t2776,t2777,t2778,t2779,t2780,t2781,t2782,t2783,t2784,
     & t2785,t2786,t2787,t2788,t2789,t2790,t2791,t2792,t2793,t2794,
     & t2795,t2796,t2797,t2798,t2799,t2800,t2801,t2802,t2803,t2804,
     & t2805,t2806,t2807,t2808,t2809,t2810,t2811,t2812,t2813,t2814,
     & t2815,t2816,t2817,t2818,t2819,t2820,t2821,t2822,t2823,t2824,
     & t2825,t2826,t2827,t2828,t2829,t2830,t2831,t2832,t2833,t2834,
     & t2835,t2836,t2837,t2838,t2839,t2840,t2841,t2842,t2843,t2844,
     & t2845,t2846,t2847,t2848,t2849,t2850,t2851,t2852,t2853,t2854,
     & t2855,t2856,t2857,t2858,t2859,t2860,t2861,t2862,t2863,t2864,
     & t2865,t2866,t2867,t2868,t2869,t2870,t2871,t2872,t2873,t2874,
     & t2875,t2876,t2877,t2878,t2879,t2880,t2881,t2882,t2883,t2884,
     & t2885,t2886,t2887,t2888,t2889,t2890,t2891,t2892,t2893,t2894,
     & t2895,t2896,t2897,t2898,t2899,t2900,t2901,t2902,t2903,t2904,
     & t2905,t2906,t2907,t2908,t2909,t2910,t2911,t2912,t2913,t2914,
     & t2915,t2916,t2917,t2918,t2919,t2920,t2921,t2922,t2923,t2924,
     & t2925,t2926,t2927,t2928,t2929,t2930,t2931,t2932,t2933,t2934,
     & t2935,t2936,t2937,t2938,t2939,t2940,t2941,t2942,t2943,t2944,
     & t2945,t2946,t2947,t2948,t2949,t2950,t2951,t2952,t2953,t2954,
     & t2955,t2956,t2957,t2958,t2959,t2960,t2961,t2962,t2963,t2964,
     & t2965,t2966,t2967,t2968,t2969,t2970,t2971,t2972,t2973,t2974,
     & t2975,t2976,t2977,t2978,t2979,t2980,t2981,t2982,t2983,t2984,
     & t2985,t2986,t2987,t2988,t2989,t2990,t2991,t2992,t2993,t2994,
     & t2995,t2996,t2997,t2998,t2999,t3000
      real uu1,uu1r,uu1s,uu1t,uu1rr,uu1rs,uu1ss,uu1rt,uu1st,uu1tt,
     & uu1rrr,uu1rrs,uu1rss,uu1sss,uu1rrt,uu1rst,uu1sst,uu1rtt,uu1stt,
     & uu1ttt,uu1rrrr,uu1rrrs,uu1rrss,uu1rsss,uu1ssss,uu1rrrt,uu1rrst,
     & uu1rsst,uu1ssst,uu1rrtt,uu1rstt,uu1sstt,uu1rttt,uu1sttt,
     & uu1tttt,uu1rrrrr,uu1rrrrs,uu1rrrss,uu1rrsss,uu1rssss,uu1sssss,
     & uu1rrrrt,uu1rrrst,uu1rrsst,uu1rssst,uu1sssst,uu1rrrtt,uu1rrstt,
     & uu1rsstt,uu1ssstt,uu1rrttt,uu1rsttt,uu1ssttt,uu1rtttt,uu1stttt,
     & uu1ttttt,uu1rrrrrr,uu1rrrrrs,uu1rrrrss,uu1rrrsss,uu1rrssss,
     & uu1rsssss,uu1ssssss,uu1rrrrrt,uu1rrrrst,uu1rrrsst,uu1rrssst,
     & uu1rsssst,uu1ssssst,uu1rrrrtt,uu1rrrstt,uu1rrsstt,uu1rssstt,
     & uu1sssstt,uu1rrrttt,uu1rrsttt,uu1rssttt,uu1sssttt,uu1rrtttt,
     & uu1rstttt,uu1sstttt,uu1rttttt,uu1sttttt,uu1tttttt
      real uu2,uu2r,uu2s,uu2t,uu2rr,uu2rs,uu2ss,uu2rt,uu2st,uu2tt,
     & uu2rrr,uu2rrs,uu2rss,uu2sss,uu2rrt,uu2rst,uu2sst,uu2rtt,uu2stt,
     & uu2ttt,uu2rrrr,uu2rrrs,uu2rrss,uu2rsss,uu2ssss,uu2rrrt,uu2rrst,
     & uu2rsst,uu2ssst,uu2rrtt,uu2rstt,uu2sstt,uu2rttt,uu2sttt,
     & uu2tttt,uu2rrrrr,uu2rrrrs,uu2rrrss,uu2rrsss,uu2rssss,uu2sssss,
     & uu2rrrrt,uu2rrrst,uu2rrsst,uu2rssst,uu2sssst,uu2rrrtt,uu2rrstt,
     & uu2rsstt,uu2ssstt,uu2rrttt,uu2rsttt,uu2ssttt,uu2rtttt,uu2stttt,
     & uu2ttttt,uu2rrrrrr,uu2rrrrrs,uu2rrrrss,uu2rrrsss,uu2rrssss,
     & uu2rsssss,uu2ssssss,uu2rrrrrt,uu2rrrrst,uu2rrrsst,uu2rrssst,
     & uu2rsssst,uu2ssssst,uu2rrrrtt,uu2rrrstt,uu2rrsstt,uu2rssstt,
     & uu2sssstt,uu2rrrttt,uu2rrsttt,uu2rssttt,uu2sssttt,uu2rrtttt,
     & uu2rstttt,uu2sstttt,uu2rttttt,uu2sttttt,uu2tttttt
      real vv1,vv1r,vv1s,vv1t,vv1rr,vv1rs,vv1ss,vv1rt,vv1st,vv1tt,
     & vv1rrr,vv1rrs,vv1rss,vv1sss,vv1rrt,vv1rst,vv1sst,vv1rtt,vv1stt,
     & vv1ttt,vv1rrrr,vv1rrrs,vv1rrss,vv1rsss,vv1ssss,vv1rrrt,vv1rrst,
     & vv1rsst,vv1ssst,vv1rrtt,vv1rstt,vv1sstt,vv1rttt,vv1sttt,
     & vv1tttt,vv1rrrrr,vv1rrrrs,vv1rrrss,vv1rrsss,vv1rssss,vv1sssss,
     & vv1rrrrt,vv1rrrst,vv1rrsst,vv1rssst,vv1sssst,vv1rrrtt,vv1rrstt,
     & vv1rsstt,vv1ssstt,vv1rrttt,vv1rsttt,vv1ssttt,vv1rtttt,vv1stttt,
     & vv1ttttt,vv1rrrrrr,vv1rrrrrs,vv1rrrrss,vv1rrrsss,vv1rrssss,
     & vv1rsssss,vv1ssssss,vv1rrrrrt,vv1rrrrst,vv1rrrsst,vv1rrssst,
     & vv1rsssst,vv1ssssst,vv1rrrrtt,vv1rrrstt,vv1rrsstt,vv1rssstt,
     & vv1sssstt,vv1rrrttt,vv1rrsttt,vv1rssttt,vv1sssttt,vv1rrtttt,
     & vv1rstttt,vv1sstttt,vv1rttttt,vv1sttttt,vv1tttttt
      real vv2,vv2r,vv2s,vv2t,vv2rr,vv2rs,vv2ss,vv2rt,vv2st,vv2tt,
     & vv2rrr,vv2rrs,vv2rss,vv2sss,vv2rrt,vv2rst,vv2sst,vv2rtt,vv2stt,
     & vv2ttt,vv2rrrr,vv2rrrs,vv2rrss,vv2rsss,vv2ssss,vv2rrrt,vv2rrst,
     & vv2rsst,vv2ssst,vv2rrtt,vv2rstt,vv2sstt,vv2rttt,vv2sttt,
     & vv2tttt,vv2rrrrr,vv2rrrrs,vv2rrrss,vv2rrsss,vv2rssss,vv2sssss,
     & vv2rrrrt,vv2rrrst,vv2rrsst,vv2rssst,vv2sssst,vv2rrrtt,vv2rrstt,
     & vv2rsstt,vv2ssstt,vv2rrttt,vv2rsttt,vv2ssttt,vv2rtttt,vv2stttt,
     & vv2ttttt,vv2rrrrrr,vv2rrrrrs,vv2rrrrss,vv2rrrsss,vv2rrssss,
     & vv2rsssss,vv2ssssss,vv2rrrrrt,vv2rrrrst,vv2rrrsst,vv2rrssst,
     & vv2rsssst,vv2ssssst,vv2rrrrtt,vv2rrrstt,vv2rrsstt,vv2rssstt,
     & vv2sssstt,vv2rrrttt,vv2rrsttt,vv2rssttt,vv2sssttt,vv2rrtttt,
     & vv2rstttt,vv2sstttt,vv2rttttt,vv2sttttt,vv2tttttt
      real ww1,ww1r,ww1s,ww1t,ww1rr,ww1rs,ww1ss,ww1rt,ww1st,ww1tt,
     & ww1rrr,ww1rrs,ww1rss,ww1sss,ww1rrt,ww1rst,ww1sst,ww1rtt,ww1stt,
     & ww1ttt,ww1rrrr,ww1rrrs,ww1rrss,ww1rsss,ww1ssss,ww1rrrt,ww1rrst,
     & ww1rsst,ww1ssst,ww1rrtt,ww1rstt,ww1sstt,ww1rttt,ww1sttt,
     & ww1tttt,ww1rrrrr,ww1rrrrs,ww1rrrss,ww1rrsss,ww1rssss,ww1sssss,
     & ww1rrrrt,ww1rrrst,ww1rrsst,ww1rssst,ww1sssst,ww1rrrtt,ww1rrstt,
     & ww1rsstt,ww1ssstt,ww1rrttt,ww1rsttt,ww1ssttt,ww1rtttt,ww1stttt,
     & ww1ttttt,ww1rrrrrr,ww1rrrrrs,ww1rrrrss,ww1rrrsss,ww1rrssss,
     & ww1rsssss,ww1ssssss,ww1rrrrrt,ww1rrrrst,ww1rrrsst,ww1rrssst,
     & ww1rsssst,ww1ssssst,ww1rrrrtt,ww1rrrstt,ww1rrsstt,ww1rssstt,
     & ww1sssstt,ww1rrrttt,ww1rrsttt,ww1rssttt,ww1sssttt,ww1rrtttt,
     & ww1rstttt,ww1sstttt,ww1rttttt,ww1sttttt,ww1tttttt
      real ww2,ww2r,ww2s,ww2t,ww2rr,ww2rs,ww2ss,ww2rt,ww2st,ww2tt,
     & ww2rrr,ww2rrs,ww2rss,ww2sss,ww2rrt,ww2rst,ww2sst,ww2rtt,ww2stt,
     & ww2ttt,ww2rrrr,ww2rrrs,ww2rrss,ww2rsss,ww2ssss,ww2rrrt,ww2rrst,
     & ww2rsst,ww2ssst,ww2rrtt,ww2rstt,ww2sstt,ww2rttt,ww2sttt,
     & ww2tttt,ww2rrrrr,ww2rrrrs,ww2rrrss,ww2rrsss,ww2rssss,ww2sssss,
     & ww2rrrrt,ww2rrrst,ww2rrsst,ww2rssst,ww2sssst,ww2rrrtt,ww2rrstt,
     & ww2rsstt,ww2ssstt,ww2rrttt,ww2rsttt,ww2ssttt,ww2rtttt,ww2stttt,
     & ww2ttttt,ww2rrrrrr,ww2rrrrrs,ww2rrrrss,ww2rrrsss,ww2rrssss,
     & ww2rsssss,ww2ssssss,ww2rrrrrt,ww2rrrrst,ww2rrrsst,ww2rrssst,
     & ww2rsssst,ww2ssssst,ww2rrrrtt,ww2rrrstt,ww2rrsstt,ww2rssstt,
     & ww2sssstt,ww2rrrttt,ww2rrsttt,ww2rssttt,ww2sssttt,ww2rrtttt,
     & ww2rstttt,ww2sstttt,ww2rttttt,ww2sttttt,ww2tttttt
       real aj1rx,aj1rxr,aj1rxs,aj1rxt,aj1rxrr,aj1rxrs,aj1rxss,aj1rxrt,
     & aj1rxst,aj1rxtt,aj1rxrrr,aj1rxrrs,aj1rxrss,aj1rxsss,aj1rxrrt,
     & aj1rxrst,aj1rxsst,aj1rxrtt,aj1rxstt,aj1rxttt,aj1rxrrrr,
     & aj1rxrrrs,aj1rxrrss,aj1rxrsss,aj1rxssss,aj1rxrrrt,aj1rxrrst,
     & aj1rxrsst,aj1rxssst,aj1rxrrtt,aj1rxrstt,aj1rxsstt,aj1rxrttt,
     & aj1rxsttt,aj1rxtttt,aj1rxrrrrr,aj1rxrrrrs,aj1rxrrrss,
     & aj1rxrrsss,aj1rxrssss,aj1rxsssss,aj1rxrrrrt,aj1rxrrrst,
     & aj1rxrrsst,aj1rxrssst,aj1rxsssst,aj1rxrrrtt,aj1rxrrstt,
     & aj1rxrsstt,aj1rxssstt,aj1rxrrttt,aj1rxrsttt,aj1rxssttt,
     & aj1rxrtttt,aj1rxstttt,aj1rxttttt,aj1rxrrrrrr,aj1rxrrrrrs,
     & aj1rxrrrrss,aj1rxrrrsss,aj1rxrrssss,aj1rxrsssss,aj1rxssssss,
     & aj1rxrrrrrt,aj1rxrrrrst,aj1rxrrrsst,aj1rxrrssst,aj1rxrsssst,
     & aj1rxssssst,aj1rxrrrrtt,aj1rxrrrstt,aj1rxrrsstt,aj1rxrssstt,
     & aj1rxsssstt,aj1rxrrrttt,aj1rxrrsttt,aj1rxrssttt,aj1rxsssttt,
     & aj1rxrrtttt,aj1rxrstttt,aj1rxsstttt,aj1rxrttttt,aj1rxsttttt,
     & aj1rxtttttt
       real aj1sx,aj1sxr,aj1sxs,aj1sxt,aj1sxrr,aj1sxrs,aj1sxss,aj1sxrt,
     & aj1sxst,aj1sxtt,aj1sxrrr,aj1sxrrs,aj1sxrss,aj1sxsss,aj1sxrrt,
     & aj1sxrst,aj1sxsst,aj1sxrtt,aj1sxstt,aj1sxttt,aj1sxrrrr,
     & aj1sxrrrs,aj1sxrrss,aj1sxrsss,aj1sxssss,aj1sxrrrt,aj1sxrrst,
     & aj1sxrsst,aj1sxssst,aj1sxrrtt,aj1sxrstt,aj1sxsstt,aj1sxrttt,
     & aj1sxsttt,aj1sxtttt,aj1sxrrrrr,aj1sxrrrrs,aj1sxrrrss,
     & aj1sxrrsss,aj1sxrssss,aj1sxsssss,aj1sxrrrrt,aj1sxrrrst,
     & aj1sxrrsst,aj1sxrssst,aj1sxsssst,aj1sxrrrtt,aj1sxrrstt,
     & aj1sxrsstt,aj1sxssstt,aj1sxrrttt,aj1sxrsttt,aj1sxssttt,
     & aj1sxrtttt,aj1sxstttt,aj1sxttttt,aj1sxrrrrrr,aj1sxrrrrrs,
     & aj1sxrrrrss,aj1sxrrrsss,aj1sxrrssss,aj1sxrsssss,aj1sxssssss,
     & aj1sxrrrrrt,aj1sxrrrrst,aj1sxrrrsst,aj1sxrrssst,aj1sxrsssst,
     & aj1sxssssst,aj1sxrrrrtt,aj1sxrrrstt,aj1sxrrsstt,aj1sxrssstt,
     & aj1sxsssstt,aj1sxrrrttt,aj1sxrrsttt,aj1sxrssttt,aj1sxsssttt,
     & aj1sxrrtttt,aj1sxrstttt,aj1sxsstttt,aj1sxrttttt,aj1sxsttttt,
     & aj1sxtttttt
       real aj1ry,aj1ryr,aj1rys,aj1ryt,aj1ryrr,aj1ryrs,aj1ryss,aj1ryrt,
     & aj1ryst,aj1rytt,aj1ryrrr,aj1ryrrs,aj1ryrss,aj1rysss,aj1ryrrt,
     & aj1ryrst,aj1rysst,aj1ryrtt,aj1rystt,aj1ryttt,aj1ryrrrr,
     & aj1ryrrrs,aj1ryrrss,aj1ryrsss,aj1ryssss,aj1ryrrrt,aj1ryrrst,
     & aj1ryrsst,aj1ryssst,aj1ryrrtt,aj1ryrstt,aj1rysstt,aj1ryrttt,
     & aj1rysttt,aj1rytttt,aj1ryrrrrr,aj1ryrrrrs,aj1ryrrrss,
     & aj1ryrrsss,aj1ryrssss,aj1rysssss,aj1ryrrrrt,aj1ryrrrst,
     & aj1ryrrsst,aj1ryrssst,aj1rysssst,aj1ryrrrtt,aj1ryrrstt,
     & aj1ryrsstt,aj1ryssstt,aj1ryrrttt,aj1ryrsttt,aj1ryssttt,
     & aj1ryrtttt,aj1rystttt,aj1ryttttt,aj1ryrrrrrr,aj1ryrrrrrs,
     & aj1ryrrrrss,aj1ryrrrsss,aj1ryrrssss,aj1ryrsssss,aj1ryssssss,
     & aj1ryrrrrrt,aj1ryrrrrst,aj1ryrrrsst,aj1ryrrssst,aj1ryrsssst,
     & aj1ryssssst,aj1ryrrrrtt,aj1ryrrrstt,aj1ryrrsstt,aj1ryrssstt,
     & aj1rysssstt,aj1ryrrrttt,aj1ryrrsttt,aj1ryrssttt,aj1rysssttt,
     & aj1ryrrtttt,aj1ryrstttt,aj1rysstttt,aj1ryrttttt,aj1rysttttt,
     & aj1rytttttt
       real aj1sy,aj1syr,aj1sys,aj1syt,aj1syrr,aj1syrs,aj1syss,aj1syrt,
     & aj1syst,aj1sytt,aj1syrrr,aj1syrrs,aj1syrss,aj1sysss,aj1syrrt,
     & aj1syrst,aj1sysst,aj1syrtt,aj1systt,aj1syttt,aj1syrrrr,
     & aj1syrrrs,aj1syrrss,aj1syrsss,aj1syssss,aj1syrrrt,aj1syrrst,
     & aj1syrsst,aj1syssst,aj1syrrtt,aj1syrstt,aj1sysstt,aj1syrttt,
     & aj1systtt,aj1sytttt,aj1syrrrrr,aj1syrrrrs,aj1syrrrss,
     & aj1syrrsss,aj1syrssss,aj1sysssss,aj1syrrrrt,aj1syrrrst,
     & aj1syrrsst,aj1syrssst,aj1sysssst,aj1syrrrtt,aj1syrrstt,
     & aj1syrsstt,aj1syssstt,aj1syrrttt,aj1syrsttt,aj1syssttt,
     & aj1syrtttt,aj1systttt,aj1syttttt,aj1syrrrrrr,aj1syrrrrrs,
     & aj1syrrrrss,aj1syrrrsss,aj1syrrssss,aj1syrsssss,aj1syssssss,
     & aj1syrrrrrt,aj1syrrrrst,aj1syrrrsst,aj1syrrssst,aj1syrsssst,
     & aj1syssssst,aj1syrrrrtt,aj1syrrrstt,aj1syrrsstt,aj1syrssstt,
     & aj1sysssstt,aj1syrrrttt,aj1syrrsttt,aj1syrssttt,aj1sysssttt,
     & aj1syrrtttt,aj1syrstttt,aj1sysstttt,aj1syrttttt,aj1systtttt,
     & aj1sytttttt
       real aj1rxx,aj1rxy,aj1rxz,aj1rxxx,aj1rxxy,aj1rxyy,aj1rxxz,
     & aj1rxyz,aj1rxzz,aj1rxxxx,aj1rxxxy,aj1rxxyy,aj1rxyyy,aj1rxxxz,
     & aj1rxxyz,aj1rxyyz,aj1rxxzz,aj1rxyzz,aj1rxzzz,aj1rxxxxx,
     & aj1rxxxxy,aj1rxxxyy,aj1rxxyyy,aj1rxyyyy,aj1rxxxxz,aj1rxxxyz,
     & aj1rxxyyz,aj1rxyyyz,aj1rxxxzz,aj1rxxyzz,aj1rxyyzz,aj1rxxzzz,
     & aj1rxyzzz,aj1rxzzzz,aj1rxxxxxx,aj1rxxxxxy,aj1rxxxxyy,
     & aj1rxxxyyy,aj1rxxyyyy,aj1rxyyyyy,aj1rxxxxxz,aj1rxxxxyz,
     & aj1rxxxyyz,aj1rxxyyyz,aj1rxyyyyz,aj1rxxxxzz,aj1rxxxyzz,
     & aj1rxxyyzz,aj1rxyyyzz,aj1rxxxzzz,aj1rxxyzzz,aj1rxyyzzz,
     & aj1rxxzzzz,aj1rxyzzzz,aj1rxzzzzz,aj1rxxxxxxx,aj1rxxxxxxy,
     & aj1rxxxxxyy,aj1rxxxxyyy,aj1rxxxyyyy,aj1rxxyyyyy,aj1rxyyyyyy,
     & aj1rxxxxxxz,aj1rxxxxxyz,aj1rxxxxyyz,aj1rxxxyyyz,aj1rxxyyyyz,
     & aj1rxyyyyyz,aj1rxxxxxzz,aj1rxxxxyzz,aj1rxxxyyzz,aj1rxxyyyzz,
     & aj1rxyyyyzz,aj1rxxxxzzz,aj1rxxxyzzz,aj1rxxyyzzz,aj1rxyyyzzz,
     & aj1rxxxzzzz,aj1rxxyzzzz,aj1rxyyzzzz,aj1rxxzzzzz,aj1rxyzzzzz,
     & aj1rxzzzzzz
       real aj1sxx,aj1sxy,aj1sxz,aj1sxxx,aj1sxxy,aj1sxyy,aj1sxxz,
     & aj1sxyz,aj1sxzz,aj1sxxxx,aj1sxxxy,aj1sxxyy,aj1sxyyy,aj1sxxxz,
     & aj1sxxyz,aj1sxyyz,aj1sxxzz,aj1sxyzz,aj1sxzzz,aj1sxxxxx,
     & aj1sxxxxy,aj1sxxxyy,aj1sxxyyy,aj1sxyyyy,aj1sxxxxz,aj1sxxxyz,
     & aj1sxxyyz,aj1sxyyyz,aj1sxxxzz,aj1sxxyzz,aj1sxyyzz,aj1sxxzzz,
     & aj1sxyzzz,aj1sxzzzz,aj1sxxxxxx,aj1sxxxxxy,aj1sxxxxyy,
     & aj1sxxxyyy,aj1sxxyyyy,aj1sxyyyyy,aj1sxxxxxz,aj1sxxxxyz,
     & aj1sxxxyyz,aj1sxxyyyz,aj1sxyyyyz,aj1sxxxxzz,aj1sxxxyzz,
     & aj1sxxyyzz,aj1sxyyyzz,aj1sxxxzzz,aj1sxxyzzz,aj1sxyyzzz,
     & aj1sxxzzzz,aj1sxyzzzz,aj1sxzzzzz,aj1sxxxxxxx,aj1sxxxxxxy,
     & aj1sxxxxxyy,aj1sxxxxyyy,aj1sxxxyyyy,aj1sxxyyyyy,aj1sxyyyyyy,
     & aj1sxxxxxxz,aj1sxxxxxyz,aj1sxxxxyyz,aj1sxxxyyyz,aj1sxxyyyyz,
     & aj1sxyyyyyz,aj1sxxxxxzz,aj1sxxxxyzz,aj1sxxxyyzz,aj1sxxyyyzz,
     & aj1sxyyyyzz,aj1sxxxxzzz,aj1sxxxyzzz,aj1sxxyyzzz,aj1sxyyyzzz,
     & aj1sxxxzzzz,aj1sxxyzzzz,aj1sxyyzzzz,aj1sxxzzzzz,aj1sxyzzzzz,
     & aj1sxzzzzzz
       real aj1ryx,aj1ryy,aj1ryz,aj1ryxx,aj1ryxy,aj1ryyy,aj1ryxz,
     & aj1ryyz,aj1ryzz,aj1ryxxx,aj1ryxxy,aj1ryxyy,aj1ryyyy,aj1ryxxz,
     & aj1ryxyz,aj1ryyyz,aj1ryxzz,aj1ryyzz,aj1ryzzz,aj1ryxxxx,
     & aj1ryxxxy,aj1ryxxyy,aj1ryxyyy,aj1ryyyyy,aj1ryxxxz,aj1ryxxyz,
     & aj1ryxyyz,aj1ryyyyz,aj1ryxxzz,aj1ryxyzz,aj1ryyyzz,aj1ryxzzz,
     & aj1ryyzzz,aj1ryzzzz,aj1ryxxxxx,aj1ryxxxxy,aj1ryxxxyy,
     & aj1ryxxyyy,aj1ryxyyyy,aj1ryyyyyy,aj1ryxxxxz,aj1ryxxxyz,
     & aj1ryxxyyz,aj1ryxyyyz,aj1ryyyyyz,aj1ryxxxzz,aj1ryxxyzz,
     & aj1ryxyyzz,aj1ryyyyzz,aj1ryxxzzz,aj1ryxyzzz,aj1ryyyzzz,
     & aj1ryxzzzz,aj1ryyzzzz,aj1ryzzzzz,aj1ryxxxxxx,aj1ryxxxxxy,
     & aj1ryxxxxyy,aj1ryxxxyyy,aj1ryxxyyyy,aj1ryxyyyyy,aj1ryyyyyyy,
     & aj1ryxxxxxz,aj1ryxxxxyz,aj1ryxxxyyz,aj1ryxxyyyz,aj1ryxyyyyz,
     & aj1ryyyyyyz,aj1ryxxxxzz,aj1ryxxxyzz,aj1ryxxyyzz,aj1ryxyyyzz,
     & aj1ryyyyyzz,aj1ryxxxzzz,aj1ryxxyzzz,aj1ryxyyzzz,aj1ryyyyzzz,
     & aj1ryxxzzzz,aj1ryxyzzzz,aj1ryyyzzzz,aj1ryxzzzzz,aj1ryyzzzzz,
     & aj1ryzzzzzz
       real aj1syx,aj1syy,aj1syz,aj1syxx,aj1syxy,aj1syyy,aj1syxz,
     & aj1syyz,aj1syzz,aj1syxxx,aj1syxxy,aj1syxyy,aj1syyyy,aj1syxxz,
     & aj1syxyz,aj1syyyz,aj1syxzz,aj1syyzz,aj1syzzz,aj1syxxxx,
     & aj1syxxxy,aj1syxxyy,aj1syxyyy,aj1syyyyy,aj1syxxxz,aj1syxxyz,
     & aj1syxyyz,aj1syyyyz,aj1syxxzz,aj1syxyzz,aj1syyyzz,aj1syxzzz,
     & aj1syyzzz,aj1syzzzz,aj1syxxxxx,aj1syxxxxy,aj1syxxxyy,
     & aj1syxxyyy,aj1syxyyyy,aj1syyyyyy,aj1syxxxxz,aj1syxxxyz,
     & aj1syxxyyz,aj1syxyyyz,aj1syyyyyz,aj1syxxxzz,aj1syxxyzz,
     & aj1syxyyzz,aj1syyyyzz,aj1syxxzzz,aj1syxyzzz,aj1syyyzzz,
     & aj1syxzzzz,aj1syyzzzz,aj1syzzzzz,aj1syxxxxxx,aj1syxxxxxy,
     & aj1syxxxxyy,aj1syxxxyyy,aj1syxxyyyy,aj1syxyyyyy,aj1syyyyyyy,
     & aj1syxxxxxz,aj1syxxxxyz,aj1syxxxyyz,aj1syxxyyyz,aj1syxyyyyz,
     & aj1syyyyyyz,aj1syxxxxzz,aj1syxxxyzz,aj1syxxyyzz,aj1syxyyyzz,
     & aj1syyyyyzz,aj1syxxxzzz,aj1syxxyzzz,aj1syxyyzzz,aj1syyyyzzz,
     & aj1syxxzzzz,aj1syxyzzzz,aj1syyyzzzz,aj1syxzzzzz,aj1syyzzzzz,
     & aj1syzzzzzz
       real aj1rz,aj1rzr,aj1rzs,aj1rzt,aj1rzrr,aj1rzrs,aj1rzss,aj1rzrt,
     & aj1rzst,aj1rztt,aj1rzrrr,aj1rzrrs,aj1rzrss,aj1rzsss,aj1rzrrt,
     & aj1rzrst,aj1rzsst,aj1rzrtt,aj1rzstt,aj1rzttt,aj1rzrrrr,
     & aj1rzrrrs,aj1rzrrss,aj1rzrsss,aj1rzssss,aj1rzrrrt,aj1rzrrst,
     & aj1rzrsst,aj1rzssst,aj1rzrrtt,aj1rzrstt,aj1rzsstt,aj1rzrttt,
     & aj1rzsttt,aj1rztttt,aj1rzrrrrr,aj1rzrrrrs,aj1rzrrrss,
     & aj1rzrrsss,aj1rzrssss,aj1rzsssss,aj1rzrrrrt,aj1rzrrrst,
     & aj1rzrrsst,aj1rzrssst,aj1rzsssst,aj1rzrrrtt,aj1rzrrstt,
     & aj1rzrsstt,aj1rzssstt,aj1rzrrttt,aj1rzrsttt,aj1rzssttt,
     & aj1rzrtttt,aj1rzstttt,aj1rzttttt,aj1rzrrrrrr,aj1rzrrrrrs,
     & aj1rzrrrrss,aj1rzrrrsss,aj1rzrrssss,aj1rzrsssss,aj1rzssssss,
     & aj1rzrrrrrt,aj1rzrrrrst,aj1rzrrrsst,aj1rzrrssst,aj1rzrsssst,
     & aj1rzssssst,aj1rzrrrrtt,aj1rzrrrstt,aj1rzrrsstt,aj1rzrssstt,
     & aj1rzsssstt,aj1rzrrrttt,aj1rzrrsttt,aj1rzrssttt,aj1rzsssttt,
     & aj1rzrrtttt,aj1rzrstttt,aj1rzsstttt,aj1rzrttttt,aj1rzsttttt,
     & aj1rztttttt
       real aj1sz,aj1szr,aj1szs,aj1szt,aj1szrr,aj1szrs,aj1szss,aj1szrt,
     & aj1szst,aj1sztt,aj1szrrr,aj1szrrs,aj1szrss,aj1szsss,aj1szrrt,
     & aj1szrst,aj1szsst,aj1szrtt,aj1szstt,aj1szttt,aj1szrrrr,
     & aj1szrrrs,aj1szrrss,aj1szrsss,aj1szssss,aj1szrrrt,aj1szrrst,
     & aj1szrsst,aj1szssst,aj1szrrtt,aj1szrstt,aj1szsstt,aj1szrttt,
     & aj1szsttt,aj1sztttt,aj1szrrrrr,aj1szrrrrs,aj1szrrrss,
     & aj1szrrsss,aj1szrssss,aj1szsssss,aj1szrrrrt,aj1szrrrst,
     & aj1szrrsst,aj1szrssst,aj1szsssst,aj1szrrrtt,aj1szrrstt,
     & aj1szrsstt,aj1szssstt,aj1szrrttt,aj1szrsttt,aj1szssttt,
     & aj1szrtttt,aj1szstttt,aj1szttttt,aj1szrrrrrr,aj1szrrrrrs,
     & aj1szrrrrss,aj1szrrrsss,aj1szrrssss,aj1szrsssss,aj1szssssss,
     & aj1szrrrrrt,aj1szrrrrst,aj1szrrrsst,aj1szrrssst,aj1szrsssst,
     & aj1szssssst,aj1szrrrrtt,aj1szrrrstt,aj1szrrsstt,aj1szrssstt,
     & aj1szsssstt,aj1szrrrttt,aj1szrrsttt,aj1szrssttt,aj1szsssttt,
     & aj1szrrtttt,aj1szrstttt,aj1szsstttt,aj1szrttttt,aj1szsttttt,
     & aj1sztttttt
       real aj1tx,aj1txr,aj1txs,aj1txt,aj1txrr,aj1txrs,aj1txss,aj1txrt,
     & aj1txst,aj1txtt,aj1txrrr,aj1txrrs,aj1txrss,aj1txsss,aj1txrrt,
     & aj1txrst,aj1txsst,aj1txrtt,aj1txstt,aj1txttt,aj1txrrrr,
     & aj1txrrrs,aj1txrrss,aj1txrsss,aj1txssss,aj1txrrrt,aj1txrrst,
     & aj1txrsst,aj1txssst,aj1txrrtt,aj1txrstt,aj1txsstt,aj1txrttt,
     & aj1txsttt,aj1txtttt,aj1txrrrrr,aj1txrrrrs,aj1txrrrss,
     & aj1txrrsss,aj1txrssss,aj1txsssss,aj1txrrrrt,aj1txrrrst,
     & aj1txrrsst,aj1txrssst,aj1txsssst,aj1txrrrtt,aj1txrrstt,
     & aj1txrsstt,aj1txssstt,aj1txrrttt,aj1txrsttt,aj1txssttt,
     & aj1txrtttt,aj1txstttt,aj1txttttt,aj1txrrrrrr,aj1txrrrrrs,
     & aj1txrrrrss,aj1txrrrsss,aj1txrrssss,aj1txrsssss,aj1txssssss,
     & aj1txrrrrrt,aj1txrrrrst,aj1txrrrsst,aj1txrrssst,aj1txrsssst,
     & aj1txssssst,aj1txrrrrtt,aj1txrrrstt,aj1txrrsstt,aj1txrssstt,
     & aj1txsssstt,aj1txrrrttt,aj1txrrsttt,aj1txrssttt,aj1txsssttt,
     & aj1txrrtttt,aj1txrstttt,aj1txsstttt,aj1txrttttt,aj1txsttttt,
     & aj1txtttttt
       real aj1ty,aj1tyr,aj1tys,aj1tyt,aj1tyrr,aj1tyrs,aj1tyss,aj1tyrt,
     & aj1tyst,aj1tytt,aj1tyrrr,aj1tyrrs,aj1tyrss,aj1tysss,aj1tyrrt,
     & aj1tyrst,aj1tysst,aj1tyrtt,aj1tystt,aj1tyttt,aj1tyrrrr,
     & aj1tyrrrs,aj1tyrrss,aj1tyrsss,aj1tyssss,aj1tyrrrt,aj1tyrrst,
     & aj1tyrsst,aj1tyssst,aj1tyrrtt,aj1tyrstt,aj1tysstt,aj1tyrttt,
     & aj1tysttt,aj1tytttt,aj1tyrrrrr,aj1tyrrrrs,aj1tyrrrss,
     & aj1tyrrsss,aj1tyrssss,aj1tysssss,aj1tyrrrrt,aj1tyrrrst,
     & aj1tyrrsst,aj1tyrssst,aj1tysssst,aj1tyrrrtt,aj1tyrrstt,
     & aj1tyrsstt,aj1tyssstt,aj1tyrrttt,aj1tyrsttt,aj1tyssttt,
     & aj1tyrtttt,aj1tystttt,aj1tyttttt,aj1tyrrrrrr,aj1tyrrrrrs,
     & aj1tyrrrrss,aj1tyrrrsss,aj1tyrrssss,aj1tyrsssss,aj1tyssssss,
     & aj1tyrrrrrt,aj1tyrrrrst,aj1tyrrrsst,aj1tyrrssst,aj1tyrsssst,
     & aj1tyssssst,aj1tyrrrrtt,aj1tyrrrstt,aj1tyrrsstt,aj1tyrssstt,
     & aj1tysssstt,aj1tyrrrttt,aj1tyrrsttt,aj1tyrssttt,aj1tysssttt,
     & aj1tyrrtttt,aj1tyrstttt,aj1tysstttt,aj1tyrttttt,aj1tysttttt,
     & aj1tytttttt
       real aj1tz,aj1tzr,aj1tzs,aj1tzt,aj1tzrr,aj1tzrs,aj1tzss,aj1tzrt,
     & aj1tzst,aj1tztt,aj1tzrrr,aj1tzrrs,aj1tzrss,aj1tzsss,aj1tzrrt,
     & aj1tzrst,aj1tzsst,aj1tzrtt,aj1tzstt,aj1tzttt,aj1tzrrrr,
     & aj1tzrrrs,aj1tzrrss,aj1tzrsss,aj1tzssss,aj1tzrrrt,aj1tzrrst,
     & aj1tzrsst,aj1tzssst,aj1tzrrtt,aj1tzrstt,aj1tzsstt,aj1tzrttt,
     & aj1tzsttt,aj1tztttt,aj1tzrrrrr,aj1tzrrrrs,aj1tzrrrss,
     & aj1tzrrsss,aj1tzrssss,aj1tzsssss,aj1tzrrrrt,aj1tzrrrst,
     & aj1tzrrsst,aj1tzrssst,aj1tzsssst,aj1tzrrrtt,aj1tzrrstt,
     & aj1tzrsstt,aj1tzssstt,aj1tzrrttt,aj1tzrsttt,aj1tzssttt,
     & aj1tzrtttt,aj1tzstttt,aj1tzttttt,aj1tzrrrrrr,aj1tzrrrrrs,
     & aj1tzrrrrss,aj1tzrrrsss,aj1tzrrssss,aj1tzrsssss,aj1tzssssss,
     & aj1tzrrrrrt,aj1tzrrrrst,aj1tzrrrsst,aj1tzrrssst,aj1tzrsssst,
     & aj1tzssssst,aj1tzrrrrtt,aj1tzrrrstt,aj1tzrrsstt,aj1tzrssstt,
     & aj1tzsssstt,aj1tzrrrttt,aj1tzrrsttt,aj1tzrssttt,aj1tzsssttt,
     & aj1tzrrtttt,aj1tzrstttt,aj1tzsstttt,aj1tzrttttt,aj1tzsttttt,
     & aj1tztttttt
       real aj1rzx,aj1rzy,aj1rzz,aj1rzxx,aj1rzxy,aj1rzyy,aj1rzxz,
     & aj1rzyz,aj1rzzz,aj1rzxxx,aj1rzxxy,aj1rzxyy,aj1rzyyy,aj1rzxxz,
     & aj1rzxyz,aj1rzyyz,aj1rzxzz,aj1rzyzz,aj1rzzzz,aj1rzxxxx,
     & aj1rzxxxy,aj1rzxxyy,aj1rzxyyy,aj1rzyyyy,aj1rzxxxz,aj1rzxxyz,
     & aj1rzxyyz,aj1rzyyyz,aj1rzxxzz,aj1rzxyzz,aj1rzyyzz,aj1rzxzzz,
     & aj1rzyzzz,aj1rzzzzz,aj1rzxxxxx,aj1rzxxxxy,aj1rzxxxyy,
     & aj1rzxxyyy,aj1rzxyyyy,aj1rzyyyyy,aj1rzxxxxz,aj1rzxxxyz,
     & aj1rzxxyyz,aj1rzxyyyz,aj1rzyyyyz,aj1rzxxxzz,aj1rzxxyzz,
     & aj1rzxyyzz,aj1rzyyyzz,aj1rzxxzzz,aj1rzxyzzz,aj1rzyyzzz,
     & aj1rzxzzzz,aj1rzyzzzz,aj1rzzzzzz,aj1rzxxxxxx,aj1rzxxxxxy,
     & aj1rzxxxxyy,aj1rzxxxyyy,aj1rzxxyyyy,aj1rzxyyyyy,aj1rzyyyyyy,
     & aj1rzxxxxxz,aj1rzxxxxyz,aj1rzxxxyyz,aj1rzxxyyyz,aj1rzxyyyyz,
     & aj1rzyyyyyz,aj1rzxxxxzz,aj1rzxxxyzz,aj1rzxxyyzz,aj1rzxyyyzz,
     & aj1rzyyyyzz,aj1rzxxxzzz,aj1rzxxyzzz,aj1rzxyyzzz,aj1rzyyyzzz,
     & aj1rzxxzzzz,aj1rzxyzzzz,aj1rzyyzzzz,aj1rzxzzzzz,aj1rzyzzzzz,
     & aj1rzzzzzzz
       real aj1szx,aj1szy,aj1szz,aj1szxx,aj1szxy,aj1szyy,aj1szxz,
     & aj1szyz,aj1szzz,aj1szxxx,aj1szxxy,aj1szxyy,aj1szyyy,aj1szxxz,
     & aj1szxyz,aj1szyyz,aj1szxzz,aj1szyzz,aj1szzzz,aj1szxxxx,
     & aj1szxxxy,aj1szxxyy,aj1szxyyy,aj1szyyyy,aj1szxxxz,aj1szxxyz,
     & aj1szxyyz,aj1szyyyz,aj1szxxzz,aj1szxyzz,aj1szyyzz,aj1szxzzz,
     & aj1szyzzz,aj1szzzzz,aj1szxxxxx,aj1szxxxxy,aj1szxxxyy,
     & aj1szxxyyy,aj1szxyyyy,aj1szyyyyy,aj1szxxxxz,aj1szxxxyz,
     & aj1szxxyyz,aj1szxyyyz,aj1szyyyyz,aj1szxxxzz,aj1szxxyzz,
     & aj1szxyyzz,aj1szyyyzz,aj1szxxzzz,aj1szxyzzz,aj1szyyzzz,
     & aj1szxzzzz,aj1szyzzzz,aj1szzzzzz,aj1szxxxxxx,aj1szxxxxxy,
     & aj1szxxxxyy,aj1szxxxyyy,aj1szxxyyyy,aj1szxyyyyy,aj1szyyyyyy,
     & aj1szxxxxxz,aj1szxxxxyz,aj1szxxxyyz,aj1szxxyyyz,aj1szxyyyyz,
     & aj1szyyyyyz,aj1szxxxxzz,aj1szxxxyzz,aj1szxxyyzz,aj1szxyyyzz,
     & aj1szyyyyzz,aj1szxxxzzz,aj1szxxyzzz,aj1szxyyzzz,aj1szyyyzzz,
     & aj1szxxzzzz,aj1szxyzzzz,aj1szyyzzzz,aj1szxzzzzz,aj1szyzzzzz,
     & aj1szzzzzzz
       real aj1txx,aj1txy,aj1txz,aj1txxx,aj1txxy,aj1txyy,aj1txxz,
     & aj1txyz,aj1txzz,aj1txxxx,aj1txxxy,aj1txxyy,aj1txyyy,aj1txxxz,
     & aj1txxyz,aj1txyyz,aj1txxzz,aj1txyzz,aj1txzzz,aj1txxxxx,
     & aj1txxxxy,aj1txxxyy,aj1txxyyy,aj1txyyyy,aj1txxxxz,aj1txxxyz,
     & aj1txxyyz,aj1txyyyz,aj1txxxzz,aj1txxyzz,aj1txyyzz,aj1txxzzz,
     & aj1txyzzz,aj1txzzzz,aj1txxxxxx,aj1txxxxxy,aj1txxxxyy,
     & aj1txxxyyy,aj1txxyyyy,aj1txyyyyy,aj1txxxxxz,aj1txxxxyz,
     & aj1txxxyyz,aj1txxyyyz,aj1txyyyyz,aj1txxxxzz,aj1txxxyzz,
     & aj1txxyyzz,aj1txyyyzz,aj1txxxzzz,aj1txxyzzz,aj1txyyzzz,
     & aj1txxzzzz,aj1txyzzzz,aj1txzzzzz,aj1txxxxxxx,aj1txxxxxxy,
     & aj1txxxxxyy,aj1txxxxyyy,aj1txxxyyyy,aj1txxyyyyy,aj1txyyyyyy,
     & aj1txxxxxxz,aj1txxxxxyz,aj1txxxxyyz,aj1txxxyyyz,aj1txxyyyyz,
     & aj1txyyyyyz,aj1txxxxxzz,aj1txxxxyzz,aj1txxxyyzz,aj1txxyyyzz,
     & aj1txyyyyzz,aj1txxxxzzz,aj1txxxyzzz,aj1txxyyzzz,aj1txyyyzzz,
     & aj1txxxzzzz,aj1txxyzzzz,aj1txyyzzzz,aj1txxzzzzz,aj1txyzzzzz,
     & aj1txzzzzzz
       real aj1tyx,aj1tyy,aj1tyz,aj1tyxx,aj1tyxy,aj1tyyy,aj1tyxz,
     & aj1tyyz,aj1tyzz,aj1tyxxx,aj1tyxxy,aj1tyxyy,aj1tyyyy,aj1tyxxz,
     & aj1tyxyz,aj1tyyyz,aj1tyxzz,aj1tyyzz,aj1tyzzz,aj1tyxxxx,
     & aj1tyxxxy,aj1tyxxyy,aj1tyxyyy,aj1tyyyyy,aj1tyxxxz,aj1tyxxyz,
     & aj1tyxyyz,aj1tyyyyz,aj1tyxxzz,aj1tyxyzz,aj1tyyyzz,aj1tyxzzz,
     & aj1tyyzzz,aj1tyzzzz,aj1tyxxxxx,aj1tyxxxxy,aj1tyxxxyy,
     & aj1tyxxyyy,aj1tyxyyyy,aj1tyyyyyy,aj1tyxxxxz,aj1tyxxxyz,
     & aj1tyxxyyz,aj1tyxyyyz,aj1tyyyyyz,aj1tyxxxzz,aj1tyxxyzz,
     & aj1tyxyyzz,aj1tyyyyzz,aj1tyxxzzz,aj1tyxyzzz,aj1tyyyzzz,
     & aj1tyxzzzz,aj1tyyzzzz,aj1tyzzzzz,aj1tyxxxxxx,aj1tyxxxxxy,
     & aj1tyxxxxyy,aj1tyxxxyyy,aj1tyxxyyyy,aj1tyxyyyyy,aj1tyyyyyyy,
     & aj1tyxxxxxz,aj1tyxxxxyz,aj1tyxxxyyz,aj1tyxxyyyz,aj1tyxyyyyz,
     & aj1tyyyyyyz,aj1tyxxxxzz,aj1tyxxxyzz,aj1tyxxyyzz,aj1tyxyyyzz,
     & aj1tyyyyyzz,aj1tyxxxzzz,aj1tyxxyzzz,aj1tyxyyzzz,aj1tyyyyzzz,
     & aj1tyxxzzzz,aj1tyxyzzzz,aj1tyyyzzzz,aj1tyxzzzzz,aj1tyyzzzzz,
     & aj1tyzzzzzz
       real aj1tzx,aj1tzy,aj1tzz,aj1tzxx,aj1tzxy,aj1tzyy,aj1tzxz,
     & aj1tzyz,aj1tzzz,aj1tzxxx,aj1tzxxy,aj1tzxyy,aj1tzyyy,aj1tzxxz,
     & aj1tzxyz,aj1tzyyz,aj1tzxzz,aj1tzyzz,aj1tzzzz,aj1tzxxxx,
     & aj1tzxxxy,aj1tzxxyy,aj1tzxyyy,aj1tzyyyy,aj1tzxxxz,aj1tzxxyz,
     & aj1tzxyyz,aj1tzyyyz,aj1tzxxzz,aj1tzxyzz,aj1tzyyzz,aj1tzxzzz,
     & aj1tzyzzz,aj1tzzzzz,aj1tzxxxxx,aj1tzxxxxy,aj1tzxxxyy,
     & aj1tzxxyyy,aj1tzxyyyy,aj1tzyyyyy,aj1tzxxxxz,aj1tzxxxyz,
     & aj1tzxxyyz,aj1tzxyyyz,aj1tzyyyyz,aj1tzxxxzz,aj1tzxxyzz,
     & aj1tzxyyzz,aj1tzyyyzz,aj1tzxxzzz,aj1tzxyzzz,aj1tzyyzzz,
     & aj1tzxzzzz,aj1tzyzzzz,aj1tzzzzzz,aj1tzxxxxxx,aj1tzxxxxxy,
     & aj1tzxxxxyy,aj1tzxxxyyy,aj1tzxxyyyy,aj1tzxyyyyy,aj1tzyyyyyy,
     & aj1tzxxxxxz,aj1tzxxxxyz,aj1tzxxxyyz,aj1tzxxyyyz,aj1tzxyyyyz,
     & aj1tzyyyyyz,aj1tzxxxxzz,aj1tzxxxyzz,aj1tzxxyyzz,aj1tzxyyyzz,
     & aj1tzyyyyzz,aj1tzxxxzzz,aj1tzxxyzzz,aj1tzxyyzzz,aj1tzyyyzzz,
     & aj1tzxxzzzz,aj1tzxyzzzz,aj1tzyyzzzz,aj1tzxzzzzz,aj1tzyzzzzz,
     & aj1tzzzzzzz
       real aj2rx,aj2rxr,aj2rxs,aj2rxt,aj2rxrr,aj2rxrs,aj2rxss,aj2rxrt,
     & aj2rxst,aj2rxtt,aj2rxrrr,aj2rxrrs,aj2rxrss,aj2rxsss,aj2rxrrt,
     & aj2rxrst,aj2rxsst,aj2rxrtt,aj2rxstt,aj2rxttt,aj2rxrrrr,
     & aj2rxrrrs,aj2rxrrss,aj2rxrsss,aj2rxssss,aj2rxrrrt,aj2rxrrst,
     & aj2rxrsst,aj2rxssst,aj2rxrrtt,aj2rxrstt,aj2rxsstt,aj2rxrttt,
     & aj2rxsttt,aj2rxtttt,aj2rxrrrrr,aj2rxrrrrs,aj2rxrrrss,
     & aj2rxrrsss,aj2rxrssss,aj2rxsssss,aj2rxrrrrt,aj2rxrrrst,
     & aj2rxrrsst,aj2rxrssst,aj2rxsssst,aj2rxrrrtt,aj2rxrrstt,
     & aj2rxrsstt,aj2rxssstt,aj2rxrrttt,aj2rxrsttt,aj2rxssttt,
     & aj2rxrtttt,aj2rxstttt,aj2rxttttt,aj2rxrrrrrr,aj2rxrrrrrs,
     & aj2rxrrrrss,aj2rxrrrsss,aj2rxrrssss,aj2rxrsssss,aj2rxssssss,
     & aj2rxrrrrrt,aj2rxrrrrst,aj2rxrrrsst,aj2rxrrssst,aj2rxrsssst,
     & aj2rxssssst,aj2rxrrrrtt,aj2rxrrrstt,aj2rxrrsstt,aj2rxrssstt,
     & aj2rxsssstt,aj2rxrrrttt,aj2rxrrsttt,aj2rxrssttt,aj2rxsssttt,
     & aj2rxrrtttt,aj2rxrstttt,aj2rxsstttt,aj2rxrttttt,aj2rxsttttt,
     & aj2rxtttttt
       real aj2sx,aj2sxr,aj2sxs,aj2sxt,aj2sxrr,aj2sxrs,aj2sxss,aj2sxrt,
     & aj2sxst,aj2sxtt,aj2sxrrr,aj2sxrrs,aj2sxrss,aj2sxsss,aj2sxrrt,
     & aj2sxrst,aj2sxsst,aj2sxrtt,aj2sxstt,aj2sxttt,aj2sxrrrr,
     & aj2sxrrrs,aj2sxrrss,aj2sxrsss,aj2sxssss,aj2sxrrrt,aj2sxrrst,
     & aj2sxrsst,aj2sxssst,aj2sxrrtt,aj2sxrstt,aj2sxsstt,aj2sxrttt,
     & aj2sxsttt,aj2sxtttt,aj2sxrrrrr,aj2sxrrrrs,aj2sxrrrss,
     & aj2sxrrsss,aj2sxrssss,aj2sxsssss,aj2sxrrrrt,aj2sxrrrst,
     & aj2sxrrsst,aj2sxrssst,aj2sxsssst,aj2sxrrrtt,aj2sxrrstt,
     & aj2sxrsstt,aj2sxssstt,aj2sxrrttt,aj2sxrsttt,aj2sxssttt,
     & aj2sxrtttt,aj2sxstttt,aj2sxttttt,aj2sxrrrrrr,aj2sxrrrrrs,
     & aj2sxrrrrss,aj2sxrrrsss,aj2sxrrssss,aj2sxrsssss,aj2sxssssss,
     & aj2sxrrrrrt,aj2sxrrrrst,aj2sxrrrsst,aj2sxrrssst,aj2sxrsssst,
     & aj2sxssssst,aj2sxrrrrtt,aj2sxrrrstt,aj2sxrrsstt,aj2sxrssstt,
     & aj2sxsssstt,aj2sxrrrttt,aj2sxrrsttt,aj2sxrssttt,aj2sxsssttt,
     & aj2sxrrtttt,aj2sxrstttt,aj2sxsstttt,aj2sxrttttt,aj2sxsttttt,
     & aj2sxtttttt
       real aj2ry,aj2ryr,aj2rys,aj2ryt,aj2ryrr,aj2ryrs,aj2ryss,aj2ryrt,
     & aj2ryst,aj2rytt,aj2ryrrr,aj2ryrrs,aj2ryrss,aj2rysss,aj2ryrrt,
     & aj2ryrst,aj2rysst,aj2ryrtt,aj2rystt,aj2ryttt,aj2ryrrrr,
     & aj2ryrrrs,aj2ryrrss,aj2ryrsss,aj2ryssss,aj2ryrrrt,aj2ryrrst,
     & aj2ryrsst,aj2ryssst,aj2ryrrtt,aj2ryrstt,aj2rysstt,aj2ryrttt,
     & aj2rysttt,aj2rytttt,aj2ryrrrrr,aj2ryrrrrs,aj2ryrrrss,
     & aj2ryrrsss,aj2ryrssss,aj2rysssss,aj2ryrrrrt,aj2ryrrrst,
     & aj2ryrrsst,aj2ryrssst,aj2rysssst,aj2ryrrrtt,aj2ryrrstt,
     & aj2ryrsstt,aj2ryssstt,aj2ryrrttt,aj2ryrsttt,aj2ryssttt,
     & aj2ryrtttt,aj2rystttt,aj2ryttttt,aj2ryrrrrrr,aj2ryrrrrrs,
     & aj2ryrrrrss,aj2ryrrrsss,aj2ryrrssss,aj2ryrsssss,aj2ryssssss,
     & aj2ryrrrrrt,aj2ryrrrrst,aj2ryrrrsst,aj2ryrrssst,aj2ryrsssst,
     & aj2ryssssst,aj2ryrrrrtt,aj2ryrrrstt,aj2ryrrsstt,aj2ryrssstt,
     & aj2rysssstt,aj2ryrrrttt,aj2ryrrsttt,aj2ryrssttt,aj2rysssttt,
     & aj2ryrrtttt,aj2ryrstttt,aj2rysstttt,aj2ryrttttt,aj2rysttttt,
     & aj2rytttttt
       real aj2sy,aj2syr,aj2sys,aj2syt,aj2syrr,aj2syrs,aj2syss,aj2syrt,
     & aj2syst,aj2sytt,aj2syrrr,aj2syrrs,aj2syrss,aj2sysss,aj2syrrt,
     & aj2syrst,aj2sysst,aj2syrtt,aj2systt,aj2syttt,aj2syrrrr,
     & aj2syrrrs,aj2syrrss,aj2syrsss,aj2syssss,aj2syrrrt,aj2syrrst,
     & aj2syrsst,aj2syssst,aj2syrrtt,aj2syrstt,aj2sysstt,aj2syrttt,
     & aj2systtt,aj2sytttt,aj2syrrrrr,aj2syrrrrs,aj2syrrrss,
     & aj2syrrsss,aj2syrssss,aj2sysssss,aj2syrrrrt,aj2syrrrst,
     & aj2syrrsst,aj2syrssst,aj2sysssst,aj2syrrrtt,aj2syrrstt,
     & aj2syrsstt,aj2syssstt,aj2syrrttt,aj2syrsttt,aj2syssttt,
     & aj2syrtttt,aj2systttt,aj2syttttt,aj2syrrrrrr,aj2syrrrrrs,
     & aj2syrrrrss,aj2syrrrsss,aj2syrrssss,aj2syrsssss,aj2syssssss,
     & aj2syrrrrrt,aj2syrrrrst,aj2syrrrsst,aj2syrrssst,aj2syrsssst,
     & aj2syssssst,aj2syrrrrtt,aj2syrrrstt,aj2syrrsstt,aj2syrssstt,
     & aj2sysssstt,aj2syrrrttt,aj2syrrsttt,aj2syrssttt,aj2sysssttt,
     & aj2syrrtttt,aj2syrstttt,aj2sysstttt,aj2syrttttt,aj2systtttt,
     & aj2sytttttt
       real aj2rxx,aj2rxy,aj2rxz,aj2rxxx,aj2rxxy,aj2rxyy,aj2rxxz,
     & aj2rxyz,aj2rxzz,aj2rxxxx,aj2rxxxy,aj2rxxyy,aj2rxyyy,aj2rxxxz,
     & aj2rxxyz,aj2rxyyz,aj2rxxzz,aj2rxyzz,aj2rxzzz,aj2rxxxxx,
     & aj2rxxxxy,aj2rxxxyy,aj2rxxyyy,aj2rxyyyy,aj2rxxxxz,aj2rxxxyz,
     & aj2rxxyyz,aj2rxyyyz,aj2rxxxzz,aj2rxxyzz,aj2rxyyzz,aj2rxxzzz,
     & aj2rxyzzz,aj2rxzzzz,aj2rxxxxxx,aj2rxxxxxy,aj2rxxxxyy,
     & aj2rxxxyyy,aj2rxxyyyy,aj2rxyyyyy,aj2rxxxxxz,aj2rxxxxyz,
     & aj2rxxxyyz,aj2rxxyyyz,aj2rxyyyyz,aj2rxxxxzz,aj2rxxxyzz,
     & aj2rxxyyzz,aj2rxyyyzz,aj2rxxxzzz,aj2rxxyzzz,aj2rxyyzzz,
     & aj2rxxzzzz,aj2rxyzzzz,aj2rxzzzzz,aj2rxxxxxxx,aj2rxxxxxxy,
     & aj2rxxxxxyy,aj2rxxxxyyy,aj2rxxxyyyy,aj2rxxyyyyy,aj2rxyyyyyy,
     & aj2rxxxxxxz,aj2rxxxxxyz,aj2rxxxxyyz,aj2rxxxyyyz,aj2rxxyyyyz,
     & aj2rxyyyyyz,aj2rxxxxxzz,aj2rxxxxyzz,aj2rxxxyyzz,aj2rxxyyyzz,
     & aj2rxyyyyzz,aj2rxxxxzzz,aj2rxxxyzzz,aj2rxxyyzzz,aj2rxyyyzzz,
     & aj2rxxxzzzz,aj2rxxyzzzz,aj2rxyyzzzz,aj2rxxzzzzz,aj2rxyzzzzz,
     & aj2rxzzzzzz
       real aj2sxx,aj2sxy,aj2sxz,aj2sxxx,aj2sxxy,aj2sxyy,aj2sxxz,
     & aj2sxyz,aj2sxzz,aj2sxxxx,aj2sxxxy,aj2sxxyy,aj2sxyyy,aj2sxxxz,
     & aj2sxxyz,aj2sxyyz,aj2sxxzz,aj2sxyzz,aj2sxzzz,aj2sxxxxx,
     & aj2sxxxxy,aj2sxxxyy,aj2sxxyyy,aj2sxyyyy,aj2sxxxxz,aj2sxxxyz,
     & aj2sxxyyz,aj2sxyyyz,aj2sxxxzz,aj2sxxyzz,aj2sxyyzz,aj2sxxzzz,
     & aj2sxyzzz,aj2sxzzzz,aj2sxxxxxx,aj2sxxxxxy,aj2sxxxxyy,
     & aj2sxxxyyy,aj2sxxyyyy,aj2sxyyyyy,aj2sxxxxxz,aj2sxxxxyz,
     & aj2sxxxyyz,aj2sxxyyyz,aj2sxyyyyz,aj2sxxxxzz,aj2sxxxyzz,
     & aj2sxxyyzz,aj2sxyyyzz,aj2sxxxzzz,aj2sxxyzzz,aj2sxyyzzz,
     & aj2sxxzzzz,aj2sxyzzzz,aj2sxzzzzz,aj2sxxxxxxx,aj2sxxxxxxy,
     & aj2sxxxxxyy,aj2sxxxxyyy,aj2sxxxyyyy,aj2sxxyyyyy,aj2sxyyyyyy,
     & aj2sxxxxxxz,aj2sxxxxxyz,aj2sxxxxyyz,aj2sxxxyyyz,aj2sxxyyyyz,
     & aj2sxyyyyyz,aj2sxxxxxzz,aj2sxxxxyzz,aj2sxxxyyzz,aj2sxxyyyzz,
     & aj2sxyyyyzz,aj2sxxxxzzz,aj2sxxxyzzz,aj2sxxyyzzz,aj2sxyyyzzz,
     & aj2sxxxzzzz,aj2sxxyzzzz,aj2sxyyzzzz,aj2sxxzzzzz,aj2sxyzzzzz,
     & aj2sxzzzzzz
       real aj2ryx,aj2ryy,aj2ryz,aj2ryxx,aj2ryxy,aj2ryyy,aj2ryxz,
     & aj2ryyz,aj2ryzz,aj2ryxxx,aj2ryxxy,aj2ryxyy,aj2ryyyy,aj2ryxxz,
     & aj2ryxyz,aj2ryyyz,aj2ryxzz,aj2ryyzz,aj2ryzzz,aj2ryxxxx,
     & aj2ryxxxy,aj2ryxxyy,aj2ryxyyy,aj2ryyyyy,aj2ryxxxz,aj2ryxxyz,
     & aj2ryxyyz,aj2ryyyyz,aj2ryxxzz,aj2ryxyzz,aj2ryyyzz,aj2ryxzzz,
     & aj2ryyzzz,aj2ryzzzz,aj2ryxxxxx,aj2ryxxxxy,aj2ryxxxyy,
     & aj2ryxxyyy,aj2ryxyyyy,aj2ryyyyyy,aj2ryxxxxz,aj2ryxxxyz,
     & aj2ryxxyyz,aj2ryxyyyz,aj2ryyyyyz,aj2ryxxxzz,aj2ryxxyzz,
     & aj2ryxyyzz,aj2ryyyyzz,aj2ryxxzzz,aj2ryxyzzz,aj2ryyyzzz,
     & aj2ryxzzzz,aj2ryyzzzz,aj2ryzzzzz,aj2ryxxxxxx,aj2ryxxxxxy,
     & aj2ryxxxxyy,aj2ryxxxyyy,aj2ryxxyyyy,aj2ryxyyyyy,aj2ryyyyyyy,
     & aj2ryxxxxxz,aj2ryxxxxyz,aj2ryxxxyyz,aj2ryxxyyyz,aj2ryxyyyyz,
     & aj2ryyyyyyz,aj2ryxxxxzz,aj2ryxxxyzz,aj2ryxxyyzz,aj2ryxyyyzz,
     & aj2ryyyyyzz,aj2ryxxxzzz,aj2ryxxyzzz,aj2ryxyyzzz,aj2ryyyyzzz,
     & aj2ryxxzzzz,aj2ryxyzzzz,aj2ryyyzzzz,aj2ryxzzzzz,aj2ryyzzzzz,
     & aj2ryzzzzzz
       real aj2syx,aj2syy,aj2syz,aj2syxx,aj2syxy,aj2syyy,aj2syxz,
     & aj2syyz,aj2syzz,aj2syxxx,aj2syxxy,aj2syxyy,aj2syyyy,aj2syxxz,
     & aj2syxyz,aj2syyyz,aj2syxzz,aj2syyzz,aj2syzzz,aj2syxxxx,
     & aj2syxxxy,aj2syxxyy,aj2syxyyy,aj2syyyyy,aj2syxxxz,aj2syxxyz,
     & aj2syxyyz,aj2syyyyz,aj2syxxzz,aj2syxyzz,aj2syyyzz,aj2syxzzz,
     & aj2syyzzz,aj2syzzzz,aj2syxxxxx,aj2syxxxxy,aj2syxxxyy,
     & aj2syxxyyy,aj2syxyyyy,aj2syyyyyy,aj2syxxxxz,aj2syxxxyz,
     & aj2syxxyyz,aj2syxyyyz,aj2syyyyyz,aj2syxxxzz,aj2syxxyzz,
     & aj2syxyyzz,aj2syyyyzz,aj2syxxzzz,aj2syxyzzz,aj2syyyzzz,
     & aj2syxzzzz,aj2syyzzzz,aj2syzzzzz,aj2syxxxxxx,aj2syxxxxxy,
     & aj2syxxxxyy,aj2syxxxyyy,aj2syxxyyyy,aj2syxyyyyy,aj2syyyyyyy,
     & aj2syxxxxxz,aj2syxxxxyz,aj2syxxxyyz,aj2syxxyyyz,aj2syxyyyyz,
     & aj2syyyyyyz,aj2syxxxxzz,aj2syxxxyzz,aj2syxxyyzz,aj2syxyyyzz,
     & aj2syyyyyzz,aj2syxxxzzz,aj2syxxyzzz,aj2syxyyzzz,aj2syyyyzzz,
     & aj2syxxzzzz,aj2syxyzzzz,aj2syyyzzzz,aj2syxzzzzz,aj2syyzzzzz,
     & aj2syzzzzzz
       real aj2rz,aj2rzr,aj2rzs,aj2rzt,aj2rzrr,aj2rzrs,aj2rzss,aj2rzrt,
     & aj2rzst,aj2rztt,aj2rzrrr,aj2rzrrs,aj2rzrss,aj2rzsss,aj2rzrrt,
     & aj2rzrst,aj2rzsst,aj2rzrtt,aj2rzstt,aj2rzttt,aj2rzrrrr,
     & aj2rzrrrs,aj2rzrrss,aj2rzrsss,aj2rzssss,aj2rzrrrt,aj2rzrrst,
     & aj2rzrsst,aj2rzssst,aj2rzrrtt,aj2rzrstt,aj2rzsstt,aj2rzrttt,
     & aj2rzsttt,aj2rztttt,aj2rzrrrrr,aj2rzrrrrs,aj2rzrrrss,
     & aj2rzrrsss,aj2rzrssss,aj2rzsssss,aj2rzrrrrt,aj2rzrrrst,
     & aj2rzrrsst,aj2rzrssst,aj2rzsssst,aj2rzrrrtt,aj2rzrrstt,
     & aj2rzrsstt,aj2rzssstt,aj2rzrrttt,aj2rzrsttt,aj2rzssttt,
     & aj2rzrtttt,aj2rzstttt,aj2rzttttt,aj2rzrrrrrr,aj2rzrrrrrs,
     & aj2rzrrrrss,aj2rzrrrsss,aj2rzrrssss,aj2rzrsssss,aj2rzssssss,
     & aj2rzrrrrrt,aj2rzrrrrst,aj2rzrrrsst,aj2rzrrssst,aj2rzrsssst,
     & aj2rzssssst,aj2rzrrrrtt,aj2rzrrrstt,aj2rzrrsstt,aj2rzrssstt,
     & aj2rzsssstt,aj2rzrrrttt,aj2rzrrsttt,aj2rzrssttt,aj2rzsssttt,
     & aj2rzrrtttt,aj2rzrstttt,aj2rzsstttt,aj2rzrttttt,aj2rzsttttt,
     & aj2rztttttt
       real aj2sz,aj2szr,aj2szs,aj2szt,aj2szrr,aj2szrs,aj2szss,aj2szrt,
     & aj2szst,aj2sztt,aj2szrrr,aj2szrrs,aj2szrss,aj2szsss,aj2szrrt,
     & aj2szrst,aj2szsst,aj2szrtt,aj2szstt,aj2szttt,aj2szrrrr,
     & aj2szrrrs,aj2szrrss,aj2szrsss,aj2szssss,aj2szrrrt,aj2szrrst,
     & aj2szrsst,aj2szssst,aj2szrrtt,aj2szrstt,aj2szsstt,aj2szrttt,
     & aj2szsttt,aj2sztttt,aj2szrrrrr,aj2szrrrrs,aj2szrrrss,
     & aj2szrrsss,aj2szrssss,aj2szsssss,aj2szrrrrt,aj2szrrrst,
     & aj2szrrsst,aj2szrssst,aj2szsssst,aj2szrrrtt,aj2szrrstt,
     & aj2szrsstt,aj2szssstt,aj2szrrttt,aj2szrsttt,aj2szssttt,
     & aj2szrtttt,aj2szstttt,aj2szttttt,aj2szrrrrrr,aj2szrrrrrs,
     & aj2szrrrrss,aj2szrrrsss,aj2szrrssss,aj2szrsssss,aj2szssssss,
     & aj2szrrrrrt,aj2szrrrrst,aj2szrrrsst,aj2szrrssst,aj2szrsssst,
     & aj2szssssst,aj2szrrrrtt,aj2szrrrstt,aj2szrrsstt,aj2szrssstt,
     & aj2szsssstt,aj2szrrrttt,aj2szrrsttt,aj2szrssttt,aj2szsssttt,
     & aj2szrrtttt,aj2szrstttt,aj2szsstttt,aj2szrttttt,aj2szsttttt,
     & aj2sztttttt
       real aj2tx,aj2txr,aj2txs,aj2txt,aj2txrr,aj2txrs,aj2txss,aj2txrt,
     & aj2txst,aj2txtt,aj2txrrr,aj2txrrs,aj2txrss,aj2txsss,aj2txrrt,
     & aj2txrst,aj2txsst,aj2txrtt,aj2txstt,aj2txttt,aj2txrrrr,
     & aj2txrrrs,aj2txrrss,aj2txrsss,aj2txssss,aj2txrrrt,aj2txrrst,
     & aj2txrsst,aj2txssst,aj2txrrtt,aj2txrstt,aj2txsstt,aj2txrttt,
     & aj2txsttt,aj2txtttt,aj2txrrrrr,aj2txrrrrs,aj2txrrrss,
     & aj2txrrsss,aj2txrssss,aj2txsssss,aj2txrrrrt,aj2txrrrst,
     & aj2txrrsst,aj2txrssst,aj2txsssst,aj2txrrrtt,aj2txrrstt,
     & aj2txrsstt,aj2txssstt,aj2txrrttt,aj2txrsttt,aj2txssttt,
     & aj2txrtttt,aj2txstttt,aj2txttttt,aj2txrrrrrr,aj2txrrrrrs,
     & aj2txrrrrss,aj2txrrrsss,aj2txrrssss,aj2txrsssss,aj2txssssss,
     & aj2txrrrrrt,aj2txrrrrst,aj2txrrrsst,aj2txrrssst,aj2txrsssst,
     & aj2txssssst,aj2txrrrrtt,aj2txrrrstt,aj2txrrsstt,aj2txrssstt,
     & aj2txsssstt,aj2txrrrttt,aj2txrrsttt,aj2txrssttt,aj2txsssttt,
     & aj2txrrtttt,aj2txrstttt,aj2txsstttt,aj2txrttttt,aj2txsttttt,
     & aj2txtttttt
       real aj2ty,aj2tyr,aj2tys,aj2tyt,aj2tyrr,aj2tyrs,aj2tyss,aj2tyrt,
     & aj2tyst,aj2tytt,aj2tyrrr,aj2tyrrs,aj2tyrss,aj2tysss,aj2tyrrt,
     & aj2tyrst,aj2tysst,aj2tyrtt,aj2tystt,aj2tyttt,aj2tyrrrr,
     & aj2tyrrrs,aj2tyrrss,aj2tyrsss,aj2tyssss,aj2tyrrrt,aj2tyrrst,
     & aj2tyrsst,aj2tyssst,aj2tyrrtt,aj2tyrstt,aj2tysstt,aj2tyrttt,
     & aj2tysttt,aj2tytttt,aj2tyrrrrr,aj2tyrrrrs,aj2tyrrrss,
     & aj2tyrrsss,aj2tyrssss,aj2tysssss,aj2tyrrrrt,aj2tyrrrst,
     & aj2tyrrsst,aj2tyrssst,aj2tysssst,aj2tyrrrtt,aj2tyrrstt,
     & aj2tyrsstt,aj2tyssstt,aj2tyrrttt,aj2tyrsttt,aj2tyssttt,
     & aj2tyrtttt,aj2tystttt,aj2tyttttt,aj2tyrrrrrr,aj2tyrrrrrs,
     & aj2tyrrrrss,aj2tyrrrsss,aj2tyrrssss,aj2tyrsssss,aj2tyssssss,
     & aj2tyrrrrrt,aj2tyrrrrst,aj2tyrrrsst,aj2tyrrssst,aj2tyrsssst,
     & aj2tyssssst,aj2tyrrrrtt,aj2tyrrrstt,aj2tyrrsstt,aj2tyrssstt,
     & aj2tysssstt,aj2tyrrrttt,aj2tyrrsttt,aj2tyrssttt,aj2tysssttt,
     & aj2tyrrtttt,aj2tyrstttt,aj2tysstttt,aj2tyrttttt,aj2tysttttt,
     & aj2tytttttt
       real aj2tz,aj2tzr,aj2tzs,aj2tzt,aj2tzrr,aj2tzrs,aj2tzss,aj2tzrt,
     & aj2tzst,aj2tztt,aj2tzrrr,aj2tzrrs,aj2tzrss,aj2tzsss,aj2tzrrt,
     & aj2tzrst,aj2tzsst,aj2tzrtt,aj2tzstt,aj2tzttt,aj2tzrrrr,
     & aj2tzrrrs,aj2tzrrss,aj2tzrsss,aj2tzssss,aj2tzrrrt,aj2tzrrst,
     & aj2tzrsst,aj2tzssst,aj2tzrrtt,aj2tzrstt,aj2tzsstt,aj2tzrttt,
     & aj2tzsttt,aj2tztttt,aj2tzrrrrr,aj2tzrrrrs,aj2tzrrrss,
     & aj2tzrrsss,aj2tzrssss,aj2tzsssss,aj2tzrrrrt,aj2tzrrrst,
     & aj2tzrrsst,aj2tzrssst,aj2tzsssst,aj2tzrrrtt,aj2tzrrstt,
     & aj2tzrsstt,aj2tzssstt,aj2tzrrttt,aj2tzrsttt,aj2tzssttt,
     & aj2tzrtttt,aj2tzstttt,aj2tzttttt,aj2tzrrrrrr,aj2tzrrrrrs,
     & aj2tzrrrrss,aj2tzrrrsss,aj2tzrrssss,aj2tzrsssss,aj2tzssssss,
     & aj2tzrrrrrt,aj2tzrrrrst,aj2tzrrrsst,aj2tzrrssst,aj2tzrsssst,
     & aj2tzssssst,aj2tzrrrrtt,aj2tzrrrstt,aj2tzrrsstt,aj2tzrssstt,
     & aj2tzsssstt,aj2tzrrrttt,aj2tzrrsttt,aj2tzrssttt,aj2tzsssttt,
     & aj2tzrrtttt,aj2tzrstttt,aj2tzsstttt,aj2tzrttttt,aj2tzsttttt,
     & aj2tztttttt
       real aj2rzx,aj2rzy,aj2rzz,aj2rzxx,aj2rzxy,aj2rzyy,aj2rzxz,
     & aj2rzyz,aj2rzzz,aj2rzxxx,aj2rzxxy,aj2rzxyy,aj2rzyyy,aj2rzxxz,
     & aj2rzxyz,aj2rzyyz,aj2rzxzz,aj2rzyzz,aj2rzzzz,aj2rzxxxx,
     & aj2rzxxxy,aj2rzxxyy,aj2rzxyyy,aj2rzyyyy,aj2rzxxxz,aj2rzxxyz,
     & aj2rzxyyz,aj2rzyyyz,aj2rzxxzz,aj2rzxyzz,aj2rzyyzz,aj2rzxzzz,
     & aj2rzyzzz,aj2rzzzzz,aj2rzxxxxx,aj2rzxxxxy,aj2rzxxxyy,
     & aj2rzxxyyy,aj2rzxyyyy,aj2rzyyyyy,aj2rzxxxxz,aj2rzxxxyz,
     & aj2rzxxyyz,aj2rzxyyyz,aj2rzyyyyz,aj2rzxxxzz,aj2rzxxyzz,
     & aj2rzxyyzz,aj2rzyyyzz,aj2rzxxzzz,aj2rzxyzzz,aj2rzyyzzz,
     & aj2rzxzzzz,aj2rzyzzzz,aj2rzzzzzz,aj2rzxxxxxx,aj2rzxxxxxy,
     & aj2rzxxxxyy,aj2rzxxxyyy,aj2rzxxyyyy,aj2rzxyyyyy,aj2rzyyyyyy,
     & aj2rzxxxxxz,aj2rzxxxxyz,aj2rzxxxyyz,aj2rzxxyyyz,aj2rzxyyyyz,
     & aj2rzyyyyyz,aj2rzxxxxzz,aj2rzxxxyzz,aj2rzxxyyzz,aj2rzxyyyzz,
     & aj2rzyyyyzz,aj2rzxxxzzz,aj2rzxxyzzz,aj2rzxyyzzz,aj2rzyyyzzz,
     & aj2rzxxzzzz,aj2rzxyzzzz,aj2rzyyzzzz,aj2rzxzzzzz,aj2rzyzzzzz,
     & aj2rzzzzzzz
       real aj2szx,aj2szy,aj2szz,aj2szxx,aj2szxy,aj2szyy,aj2szxz,
     & aj2szyz,aj2szzz,aj2szxxx,aj2szxxy,aj2szxyy,aj2szyyy,aj2szxxz,
     & aj2szxyz,aj2szyyz,aj2szxzz,aj2szyzz,aj2szzzz,aj2szxxxx,
     & aj2szxxxy,aj2szxxyy,aj2szxyyy,aj2szyyyy,aj2szxxxz,aj2szxxyz,
     & aj2szxyyz,aj2szyyyz,aj2szxxzz,aj2szxyzz,aj2szyyzz,aj2szxzzz,
     & aj2szyzzz,aj2szzzzz,aj2szxxxxx,aj2szxxxxy,aj2szxxxyy,
     & aj2szxxyyy,aj2szxyyyy,aj2szyyyyy,aj2szxxxxz,aj2szxxxyz,
     & aj2szxxyyz,aj2szxyyyz,aj2szyyyyz,aj2szxxxzz,aj2szxxyzz,
     & aj2szxyyzz,aj2szyyyzz,aj2szxxzzz,aj2szxyzzz,aj2szyyzzz,
     & aj2szxzzzz,aj2szyzzzz,aj2szzzzzz,aj2szxxxxxx,aj2szxxxxxy,
     & aj2szxxxxyy,aj2szxxxyyy,aj2szxxyyyy,aj2szxyyyyy,aj2szyyyyyy,
     & aj2szxxxxxz,aj2szxxxxyz,aj2szxxxyyz,aj2szxxyyyz,aj2szxyyyyz,
     & aj2szyyyyyz,aj2szxxxxzz,aj2szxxxyzz,aj2szxxyyzz,aj2szxyyyzz,
     & aj2szyyyyzz,aj2szxxxzzz,aj2szxxyzzz,aj2szxyyzzz,aj2szyyyzzz,
     & aj2szxxzzzz,aj2szxyzzzz,aj2szyyzzzz,aj2szxzzzzz,aj2szyzzzzz,
     & aj2szzzzzzz
       real aj2txx,aj2txy,aj2txz,aj2txxx,aj2txxy,aj2txyy,aj2txxz,
     & aj2txyz,aj2txzz,aj2txxxx,aj2txxxy,aj2txxyy,aj2txyyy,aj2txxxz,
     & aj2txxyz,aj2txyyz,aj2txxzz,aj2txyzz,aj2txzzz,aj2txxxxx,
     & aj2txxxxy,aj2txxxyy,aj2txxyyy,aj2txyyyy,aj2txxxxz,aj2txxxyz,
     & aj2txxyyz,aj2txyyyz,aj2txxxzz,aj2txxyzz,aj2txyyzz,aj2txxzzz,
     & aj2txyzzz,aj2txzzzz,aj2txxxxxx,aj2txxxxxy,aj2txxxxyy,
     & aj2txxxyyy,aj2txxyyyy,aj2txyyyyy,aj2txxxxxz,aj2txxxxyz,
     & aj2txxxyyz,aj2txxyyyz,aj2txyyyyz,aj2txxxxzz,aj2txxxyzz,
     & aj2txxyyzz,aj2txyyyzz,aj2txxxzzz,aj2txxyzzz,aj2txyyzzz,
     & aj2txxzzzz,aj2txyzzzz,aj2txzzzzz,aj2txxxxxxx,aj2txxxxxxy,
     & aj2txxxxxyy,aj2txxxxyyy,aj2txxxyyyy,aj2txxyyyyy,aj2txyyyyyy,
     & aj2txxxxxxz,aj2txxxxxyz,aj2txxxxyyz,aj2txxxyyyz,aj2txxyyyyz,
     & aj2txyyyyyz,aj2txxxxxzz,aj2txxxxyzz,aj2txxxyyzz,aj2txxyyyzz,
     & aj2txyyyyzz,aj2txxxxzzz,aj2txxxyzzz,aj2txxyyzzz,aj2txyyyzzz,
     & aj2txxxzzzz,aj2txxyzzzz,aj2txyyzzzz,aj2txxzzzzz,aj2txyzzzzz,
     & aj2txzzzzzz
       real aj2tyx,aj2tyy,aj2tyz,aj2tyxx,aj2tyxy,aj2tyyy,aj2tyxz,
     & aj2tyyz,aj2tyzz,aj2tyxxx,aj2tyxxy,aj2tyxyy,aj2tyyyy,aj2tyxxz,
     & aj2tyxyz,aj2tyyyz,aj2tyxzz,aj2tyyzz,aj2tyzzz,aj2tyxxxx,
     & aj2tyxxxy,aj2tyxxyy,aj2tyxyyy,aj2tyyyyy,aj2tyxxxz,aj2tyxxyz,
     & aj2tyxyyz,aj2tyyyyz,aj2tyxxzz,aj2tyxyzz,aj2tyyyzz,aj2tyxzzz,
     & aj2tyyzzz,aj2tyzzzz,aj2tyxxxxx,aj2tyxxxxy,aj2tyxxxyy,
     & aj2tyxxyyy,aj2tyxyyyy,aj2tyyyyyy,aj2tyxxxxz,aj2tyxxxyz,
     & aj2tyxxyyz,aj2tyxyyyz,aj2tyyyyyz,aj2tyxxxzz,aj2tyxxyzz,
     & aj2tyxyyzz,aj2tyyyyzz,aj2tyxxzzz,aj2tyxyzzz,aj2tyyyzzz,
     & aj2tyxzzzz,aj2tyyzzzz,aj2tyzzzzz,aj2tyxxxxxx,aj2tyxxxxxy,
     & aj2tyxxxxyy,aj2tyxxxyyy,aj2tyxxyyyy,aj2tyxyyyyy,aj2tyyyyyyy,
     & aj2tyxxxxxz,aj2tyxxxxyz,aj2tyxxxyyz,aj2tyxxyyyz,aj2tyxyyyyz,
     & aj2tyyyyyyz,aj2tyxxxxzz,aj2tyxxxyzz,aj2tyxxyyzz,aj2tyxyyyzz,
     & aj2tyyyyyzz,aj2tyxxxzzz,aj2tyxxyzzz,aj2tyxyyzzz,aj2tyyyyzzz,
     & aj2tyxxzzzz,aj2tyxyzzzz,aj2tyyyzzzz,aj2tyxzzzzz,aj2tyyzzzzz,
     & aj2tyzzzzzz
       real aj2tzx,aj2tzy,aj2tzz,aj2tzxx,aj2tzxy,aj2tzyy,aj2tzxz,
     & aj2tzyz,aj2tzzz,aj2tzxxx,aj2tzxxy,aj2tzxyy,aj2tzyyy,aj2tzxxz,
     & aj2tzxyz,aj2tzyyz,aj2tzxzz,aj2tzyzz,aj2tzzzz,aj2tzxxxx,
     & aj2tzxxxy,aj2tzxxyy,aj2tzxyyy,aj2tzyyyy,aj2tzxxxz,aj2tzxxyz,
     & aj2tzxyyz,aj2tzyyyz,aj2tzxxzz,aj2tzxyzz,aj2tzyyzz,aj2tzxzzz,
     & aj2tzyzzz,aj2tzzzzz,aj2tzxxxxx,aj2tzxxxxy,aj2tzxxxyy,
     & aj2tzxxyyy,aj2tzxyyyy,aj2tzyyyyy,aj2tzxxxxz,aj2tzxxxyz,
     & aj2tzxxyyz,aj2tzxyyyz,aj2tzyyyyz,aj2tzxxxzz,aj2tzxxyzz,
     & aj2tzxyyzz,aj2tzyyyzz,aj2tzxxzzz,aj2tzxyzzz,aj2tzyyzzz,
     & aj2tzxzzzz,aj2tzyzzzz,aj2tzzzzzz,aj2tzxxxxxx,aj2tzxxxxxy,
     & aj2tzxxxxyy,aj2tzxxxyyy,aj2tzxxyyyy,aj2tzxyyyyy,aj2tzyyyyyy,
     & aj2tzxxxxxz,aj2tzxxxxyz,aj2tzxxxyyz,aj2tzxxyyyz,aj2tzxyyyyz,
     & aj2tzyyyyyz,aj2tzxxxxzz,aj2tzxxxyzz,aj2tzxxyyzz,aj2tzxyyyzz,
     & aj2tzyyyyzz,aj2tzxxxzzz,aj2tzxxyzzz,aj2tzxyyzzz,aj2tzyyyzzz,
     & aj2tzxxzzzz,aj2tzxyzzzz,aj2tzyyzzzz,aj2tzxzzzzz,aj2tzyzzzzz,
     & aj2tzzzzzzz

c............... end statement functions

      ierr=0

      side1                =ipar(0)
      axis1                =ipar(1)
      grid1                =ipar(2)
      n1a                  =ipar(3)
      n1b                  =ipar(4)
      n2a                  =ipar(5)
      n2b                  =ipar(6)
      n3a                  =ipar(7)
      n3b                  =ipar(8)

      side2                =ipar(9)
      axis2                =ipar(10)
      grid2                =ipar(11)
      m1a                  =ipar(12)
      m1b                  =ipar(13)
      m2a                  =ipar(14)
      m2b                  =ipar(15)
      m3a                  =ipar(16)
      m3b                  =ipar(17)

      gridType             =ipar(18)
      orderOfAccuracy      =ipar(19)
      orderOfExtrapolation =ipar(20)  ! maximum allowable order of extrapolation
      useForcing           =ipar(21)
      ex                   =ipar(22)
      ey                   =ipar(23)
      ez                   =ipar(24)
      hx                   =ipar(25)
      hy                   =ipar(26)
      hz                   =ipar(27)
      solveForE            =ipar(28)
      solveForH            =ipar(29)
      useWhereMask         =ipar(30)
      debug                =ipar(31)
      nit                  =ipar(32)
      interfaceOption      =ipar(33)
      initialized          =ipar(34)
      myid                 =ipar(35)
      parallel             =ipar(36)
      forcingOption        =ipar(37)
      interfaceEquationsOption=ipar(38)

      dx1(0)                =rpar(0)
      dx1(1)                =rpar(1)
      dx1(2)                =rpar(2)
      dr1(0)                =rpar(3)
      dr1(1)                =rpar(4)
      dr1(2)                =rpar(5)

      dx2(0)                =rpar(6)
      dx2(1)                =rpar(7)
      dx2(2)                =rpar(8)
      dr2(0)                =rpar(9)
      dr2(1)                =rpar(10)
      dr2(2)                =rpar(11)

      t                    =rpar(12)
      ep                   =rpar(13) ! pointer for exact solution
      dt                   =rpar(14)
      eps1                 =rpar(15)
      mu1                  =rpar(16)
      c1                   =rpar(17)
      eps2                 =rpar(18)
      mu2                  =rpar(19)
      c2                   =rpar(20)

      epsmu1=eps1*mu1
      epsmu2=eps2*mu2

      twilightZone=useForcing

      debugFile=10
      if( initialized.eq.0 .and. debug.gt.0 )then
        ! open debug files
        ! open (debugFile,file=filen,status='unknown',form='formatted')
        if( myid.lt.10 )then
          write(debugFileName,'("mxi",i1,".fdebug")') myid
        else
          write(debugFileName,'("mxi",i2,".fdebug")') myid
        end if
        write(*,*) 'interface3d: myid=',myid,' open debug file:',
     & debugFileName
        open (debugFile,file=debugFileName,status='unknown',
     & form='formatted')
        ! '
        ! INQUIRE(FILE=filen, EXIST=filex)
      end if

      if( t.lt.dt )then
        write(debugFile,'(" +++++++++cgmx interface3d t=",e9.2," ++++++
     & ++")') t
           ! '
        write(debugFile,'(" interface3d new: nd=",i2," gridType=",i2)')
     &  nd,gridType
      end if

      if( abs(c1*c1-1./(mu1*eps1)).gt. 1.e-10 )then
        write(debugFile,'(" interface3d:ERROR: c1,eps1,mu1=",3e10.2," 
     & not consistent")') c1,eps1,mu1
           ! '
        stop 11
      end if
      if( abs(c2*c2-1./(mu2*eps2)).gt. 1.e-10 )then
        write(debugFile,'(" interface3d:ERROR: c2,eps2,mu2=",3e10.2," 
     & not consistent")') c2,eps2,mu2
           ! '
        stop 11
      end if

      if( .false. )then
        write(debugFile,'(" interface3d: eps1,eps2=",2f10.5," c1,c2=",
     & 2f10.5)') eps1,eps2,c1,c2
           ! '
      end if

      if( nit.lt.0 .or. nit.gt.100 )then
        write(debugFile,'(" interfaceBC: ERROR: nit=",i9)') nit
        nit=max(1,min(100,nit))
      end if

      if( debug.gt.1 )then
        write(debugFile,'("********************************************
     & ************************** ")')
        write(debugFile,'(" interface3d: **START** t=",e10.2)') t
        write(debugFile,'(" interface3d: **START** grid1=",i4," side1,
     & axis1=",2i2," bc=",6i3)') grid1,side1,axis1,boundaryCondition1(
     & 0,0),boundaryCondition1(1,0),boundaryCondition1(0,1),
     & boundaryCondition1(1,1),boundaryCondition1(0,2),
     & boundaryCondition1(1,2)
           ! '
        write(debugFile,'(" interface3d: **START** grid2=",i4," side2,
     & axis2=",2i2," bc=",6i3)') grid2,side2,axis2,boundaryCondition2(
     & 0,0),boundaryCondition2(1,0),boundaryCondition2(0,1),
     & boundaryCondition2(1,1),boundaryCondition2(0,2),
     & boundaryCondition2(1,2)
           ! '
        write(debugFile,'("n1a,n1b,...=",6i5)') n1a,n1b,n2a,n2b,n3a,n3b
        write(debugFile,'("m1a,m1b,...=",6i5)') m1a,m1b,m2a,m2b,m3a,m3b

      end if
      if( debug.gt.8 )then
       write(debugFile,'("start u1=",(3i4,1x,3e11.2))') (((i1,i2,i3,(
     & u1(i1,i2,i3,m),m=0,2),i1=nd1a,nd1b),i2=nd2a,nd2b),i3=nd3a,nd3b)
       write(debugFile,'("start u2=",(3i4,1x,3e11.2))') (((i1,i2,i3,(
     & u2(i1,i2,i3,m),m=0,2),i1=md1a,md1b),i2=md2a,md2b),i3=md3a,md3b)
      end if




      ! *** do this for now --- assume grids have equal spacing
c      dx(0)=dx1(0)
c      dx(1)=dx1(1)
c      dx(2)=dx1(2)

c      dr(0)=dr1(0)
c      dr(1)=dr1(1)
c      dr(2)=dr1(2)

      epsx=1.e-20  ! fix this

      do kd=0,nd-1
       dx112(kd) = 1./(2.*dx1(kd))
       dx122(kd) = 1./(dx1(kd)**2)
       dx212(kd) = 1./(2.*dx2(kd))
       dx222(kd) = 1./(dx2(kd)**2)

       dx141(kd) = 1./(12.*dx1(kd))
       dx142(kd) = 1./(12.*dx1(kd)**2)
       dx241(kd) = 1./(12.*dx2(kd))
       dx242(kd) = 1./(12.*dx2(kd)**2)

       dr114(kd) = 1./(12.*dr1(kd))
       dr214(kd) = 1./(12.*dr2(kd))
      end do

      numGhost=orderOfAccuracy/2
      giveDiv=0   ! set to 1 to give div(u) on both sides, rather than setting the jump in div(u)

      ! bounds for loops that include ghost points in the tangential directions:
      nn1a=n1a
      nn1b=n1b
      nn2a=n2a
      nn2b=n2b
      nn3a=n3a
      nn3b=n3b

      mm1a=m1a
      mm1b=m1b
      mm2a=m2a
      mm2b=m2b
      mm3a=m3a
      mm3b=m3b

      i3=n3a
      j3=m3a

      axis1p1=mod(axis1+1,nd)
      axis1p2=mod(axis1+2,nd)
      axis2p1=mod(axis2+1,nd)
      axis2p2=mod(axis2+2,nd)

      is1=0
      is2=0
      is3=0

      if( axis1.ne.0 )then
        ! include ghost lines in tangential periodic (and parallel) directions (for extrapolating)
        if( boundaryCondition1(0,0).lt.0 )then ! parallel ghost may only have bc<0 on one side
          nn1a=nn1a-numGhost
          if( boundaryCondition2(0,0).ge.0 )then
            write(*,'("Interaface3d: bc is inconsistent")')
            stop 178
          end if
        end if
        if( boundaryCondition1(1,0).lt.0 )then ! parallel ghost may only have bc<0 on one side
          nn1b=nn1b+numGhost
          if( boundaryCondition2(1,0).ge.0 )then
            write(*,'("Interaface3d: bc is inconsistent")')
            stop 179
          end if
        end if
      end if
      if( axis1.ne.1 )then
        ! include ghost lines in tangential periodic (and parallel) directions (for extrapolating)
        if( boundaryCondition1(0,1).lt.0 )then
          nn2a=nn2a-numGhost
          if( boundaryCondition2(0,1).ge.0 )then
            write(*,'("Interaface3d: bc is inconsistent")')
            stop 180
          end if
        end if
        if( boundaryCondition1(1,1).lt.0 )then
          nn2b=nn2b+numGhost
          if( boundaryCondition2(1,1).ge.0 )then
            write(*,'("Interaface3d: bc is inconsistent")')
            stop 181
          end if
        end if
      end if
      if( nd.eq.3 .and. axis1.ne.2 )then
        ! include ghost lines in tangential periodic (and parallel) directions (for extrapolating)
        if( boundaryCondition1(0,2).lt.0 )then
          nn3a=nn3a-numGhost
          if( boundaryCondition2(0,2).ge.0 )then
            write(*,'("Interaface3d: bc is inconsistent")')
            stop 182
          end if
        end if
        if( boundaryCondition1(1,2).lt.0 )then
          nn3b=nn3b+numGhost
          if( boundaryCondition2(1,2).ge.0 )then
            write(*,'("Interaface3d: bc is inconsistent")')
            stop 183
          end if
        end if
      end if

      if( axis1.eq.0 ) then
        is1=1-2*side1
        an1Cartesian=1. ! normal for a cartesian grid
        an2Cartesian=0.
        an3Cartesian=0.

      else if( axis1.eq.1 )then
        is2=1-2*side1
        an1Cartesian=0.
        an2Cartesian=1.
        an3Cartesian=0.

      else if( axis1.eq.2 )then
        is3=1-2*side1
        an1Cartesian=0.
        an2Cartesian=0.
        an3Cartesian=1.
      else
        stop 5528
      end if


      js1=0
      js2=0
      js3=0
      if( axis2.ne.0 )then
        if( boundaryCondition2(0,0).lt.0 )then
          mm1a=mm1a-numGhost
        end if
        if( boundaryCondition2(1,0).lt.0 )then
          mm1b=mm1b+numGhost
        end if
      end if
      if( axis2.ne.1 )then
        if( boundaryCondition2(0,1).lt.0 )then
          mm2a=mm2a-numGhost
        end if
        if( boundaryCondition2(1,1).lt.0 )then
          mm2b=mm2b+numGhost
        end if
      end if
      if( nd.eq.3 .and. axis2.ne.2 )then
        if( boundaryCondition2(0,2).lt.0 )then
          mm3a=mm3a-numGhost
        end if
        if( boundaryCondition2(1,2).lt.0 )then
          mm3b=mm3b+numGhost
        end if
      end if
      if( axis2.eq.0 ) then
        js1=1-2*side2
      else if( axis2.eq.1 ) then
        js2=1-2*side2
      else  if( axis2.eq.2 ) then
        js3=1-2*side2
      else
        stop 3384
      end if

      is=1-2*side1
      js=1-2*side2

      rx1=0.
      ry1=0.
      rz1=0.
      if( axis1.eq.0 )then
        rx1=1.
      else if( axis1.eq.1 )then
        ry1=1.
      else
        rz1=1.
      endif

      rx2=0.
      ry2=0.
      rz2=0.
      if( axis2.eq.0 )then
        rx2=1.
      else if( axis2.eq.1 )then
        ry2=1.
      else
        rz2=1.
      endif

      if( debug.gt.3 )then
        write(debugFile,'("nn1a,nn1b,...=",6i5)') nn1a,nn1b,nn2a,nn2b,
     & nn3a,nn3b
        write(debugFile,'("mm1a,mm1b,...=",6i5)') mm1a,mm1b,mm2a,mm2b,
     & mm3a,mm3b

      end if

      if( orderOfAccuracy.eq.2 .and. orderOfExtrapolation.lt.3 )then
        write(debugFile,'(" ERROR: interface3d: orderOfExtrapolation<3 
     & ")')
        stop 7716
      end if
      if( orderOfAccuracy.eq.4 .and. orderOfExtrapolation.lt.4 )then
        write(debugFile,'(" ERROR: interface3d: orderOfExtrapolation<4 
     & ")')
        stop 7716
      end if

      ! first time through check that the mask's are consistent
      ! For now we require the masks to both be positive at the same points on the interface
      ! We assign pts where both mask1 and mask2 are discretization pts.
      ! If mask1>0 and mask2<0 then we just leave the extrapolated values in u1 and u2 .  
      if( initialized.eq.0 )then
       if( nd.eq.2 )then
        ! check the consistency of the mask arrays
         i3=n3a
         j3=m3a
         j2=m2a
         do i2=n2a,n2b
          j1=m1a
          do i1=n1a,n1b
          m1 = mask1(i1,i2,i3)
          m2 = mask2(j1,j2,j3)
          if( (m1.gt.0 .and. m2.eq.0) .or. (m1.eq.0 .and. m2.gt.0) )
     & then
            write(debugFile,'(" interface3d:ERROR: mask1 and mask2 do 
     & not agree. One is >0 and one =0 ")')
             ! '
            stop 1111
          end if
           j1=j1+1
          end do
          j2=j2+1
         end do

       else if( nd.eq.3 )then
        ! check the consistency of the mask arrays
         j3=m3a
         do i3=n3a,n3b
         j2=m2a
         do i2=n2a,n2b
         j1=m1a
         do i1=n1a,n1b
          m1 = mask1(i1,i2,i3)
          m2 = mask2(j1,j2,j3)
          if( (m1.gt.0 .and. m2.eq.0) .or. (m1.eq.0 .and. m2.gt.0) )
     & then
            write(debugFile,'(" interface3d:ERROR: mask1 and mask2 do 
     & not agree. One is >0 and one =0")')
             ! '
            stop 1111
          end if
           j1=j1+1
          end do
          j2=j2+1
         end do
          j3=j3+1
         end do

       end if
       if( debug.gt.0 )then
         write(debugFile,'("cgmx:interface3d: The mask arrays for 
     & grid1=",i3," and grid2=",i3," were found to be consistent")') 
     & grid1,grid2
         ! ' 
       end if
      end if


      if( nd.eq.2 .and. orderOfAccuracy.eq.2 .and. 
     & gridType.eq.rectangular )then


        if( useForcing.ne.0 )then
          ! finish me 
          stop 7715
        end if


       if( .false. )then
        ! just copy values from ghost points for now
         i3=n3a
         j3=m3a
         j2=m2a
         do i2=n2a,n2b
          j1=m1a
          do i1=n1a,n1b
           if( mask1(i1,i2,i3).gt.0 .and. mask2(j1,j2,j3).gt.0 )then
          u1(i1-is1,i2-is2,i3,ex)=u2(j1+js1,j2+js2,j3,ex)
          u1(i1-is1,i2-is2,i3,ey)=u2(j1+js1,j2+js2,j3,ey)
          u1(i1-is1,i2-is2,i3,hz)=u2(j1+js1,j2+js2,j3,hz)
           end if
           j1=j1+1
          end do
          j2=j2+1
         end do
      else

        ! ---- first satisfy the jump conditions on the boundary --------
        !    [ eps n.u ] = 0
        !    [ tau.u ] = 0
          if( eps1.lt.eps2 )then
            epsRatio=eps1/eps2
             i3=n3a
             j3=m3a
             j2=mm2a
             do i2=nn2a,nn2b
              j1=mm1a
              do i1=nn1a,nn1b
              if( mask1(i1,i2,i3).gt.0 .and. mask2(j1,j2,j3).gt.0 )then
              ! eps2 n.u2 = eps1 n.u1
              !     tau.u2 = tau.u1
               an1=an1Cartesian
               an2=an2Cartesian
              ua=u1(i1,i2,i3,ex)
              ub=u1(i1,i2,i3,ey)
              nDotU = an1*ua+an2*ub
              if( twilightZone.eq.1 )then
               ! adjust for TZ forcing (here we assume the exact solution is the same on both sides)
               call ogderiv(ep, 0,0,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),0.,t, ex, ue )
               call ogderiv(ep, 0,0,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),0.,t, ey, ve )
               nDotU = nDotU - (an1*ue+an2*ve)
              end if
              ! u2 equals u1 but with normal component = eps1/eps2*(n.u1)
              u2(j1,j2,j3,ex) = ua + (nDotU*epsRatio - nDotU)*an1
              u2(j1,j2,j3,ey) = ub + (nDotU*epsRatio - nDotU)*an2
              u2(j1,j2,j3,hz) = u1(i1,i2,i3,hz)
               end if
               j1=j1+1
              end do
              j2=j2+1
             end do
          else
            epsRatio=eps2/eps1
             i3=n3a
             j3=m3a
             j2=mm2a
             do i2=nn2a,nn2b
              j1=mm1a
              do i1=nn1a,nn1b
              if( mask1(i1,i2,i3).gt.0 .and. mask2(j1,j2,j3).gt.0 )then
              ! eps2 n.u2 = eps1 n.u1
              !     tau.u2 = tau.u1
               an1=an1Cartesian
               an2=an2Cartesian
              ua=u2(j1,j2,j3,ex)
              ub=u2(j1,j2,j3,ey)
              nDotU = an1*ua+an2*ub
              if( twilightZone.eq.1 )then
               ! adjust for TZ forcing (here we assume the exact solution is the same on both sides)
               call ogderiv(ep, 0,0,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),0.,t, ex, ue )
               call ogderiv(ep, 0,0,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),0.,t, ey, ve )
        ! write(*,'(" jump: x,y=",2e10.2," ua,ue=",2e10.2," ub,ve=",2e10.2)') xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),ua,ue,ub,ve
               nDotU = nDotU - (an1*ue+an2*ve)
              end if
              u1(i1,i2,i3,ex) = ua + (nDotU*epsRatio - nDotU)*an1
              u1(i1,i2,i3,ey) = ub + (nDotU*epsRatio - nDotU)*an2
              u1(i1,i2,i3,hz) = u2(j1,j2,j3,hz)
               end if
               j1=j1+1
              end do
              j2=j2+1
             end do
          end if

        ! initialization step: assign first ghost line by extrapolation
        ! NOTE: assign ghost points outside the ends
        if( solveForE.ne.0 )then
          i3=n3a
          j3=m3a
          j2=mm2a
          do i2=nn2a,nn2b
           j1=mm1a
           do i1=nn1a,nn1b
           u1(i1-is1,i2-is2,i3,ex)=(3.*u1(i1,i2,i3,ex)-3.*u1(i1+is1,i2+
     & is2,i3+is3,ex)+u1(i1+2*is1,i2+2*is2,i3+2*is3,ex))
           u1(i1-is1,i2-is2,i3,ey)=(3.*u1(i1,i2,i3,ey)-3.*u1(i1+is1,i2+
     & is2,i3+is3,ey)+u1(i1+2*is1,i2+2*is2,i3+2*is3,ey))
           u1(i1-is1,i2-is2,i3,hz)=(3.*u1(i1,i2,i3,hz)-3.*u1(i1+is1,i2+
     & is2,i3+is3,hz)+u1(i1+2*is1,i2+2*is2,i3+2*is3,hz))
            j1=j1+1
           end do
           j2=j2+1
          end do
        end if
        if( solveForH .ne.0 )then
          stop 3017
        end if
        ! here are the real jump conditions
        !   [ u.x + v.y +w.z ] = 0
        !   [ u.xx + u.yy +u.zz ] = 0
        ! 
        !   [ tau1.(w.y-v.z, u.z-w.x, v.x-u.y)/mu] = 0 
        !   [ (v.xx+v.yy+v.zz)/eps ] = 0
        ! 
        !   [ tau2.(w.y-v.z, u.z-w.x, v.x-u.y)/mu] = 0 
        !   [ (w.xx+w.yy+w.zz)/eps ] = 0

         i3=n3a
         j3=m3a
         j2=m2a
         do i2=n2a,n2b
          j1=m1a
          do i1=n1a,n1b
           if( mask1(i1,i2,i3).gt.0 .and. mask2(j1,j2,j3).gt.0 )then
         ! first evaluate the equations we want to solve with the wrong values at the ghost points:

           ! NOTE: the jacobian derivatives can be computed once for all components
             uu1=u1(i1,i2,i3,ex) ! in the rectangular case just eval the solution
              u1x = (-u1(i1-1,i2,i3,ex)+u1(i1+1,i2,i3,ex))/(2.*dx1(0))
              u1y = (-u1(i1,i2-1,i3,ex)+u1(i1,i2+1,i3,ex))/(2.*dx1(1))
              u1xx = (u1(i1-1,i2,i3,ex)-2.*u1(i1,i2,i3,ex)+u1(i1+1,i2,
     & i3,ex))/(dx1(0)**2)
              u1yy = (u1(i1,i2-1,i3,ex)-2.*u1(i1,i2,i3,ex)+u1(i1,i2+1,
     & i3,ex))/(dx1(1)**2)
            u1Lap = u1xx+ u1yy
             vv1=u1(i1,i2,i3,ey) ! in the rectangular case just eval the solution
              v1x = (-u1(i1-1,i2,i3,ey)+u1(i1+1,i2,i3,ey))/(2.*dx1(0))
              v1y = (-u1(i1,i2-1,i3,ey)+u1(i1,i2+1,i3,ey))/(2.*dx1(1))
              v1xx = (u1(i1-1,i2,i3,ey)-2.*u1(i1,i2,i3,ey)+u1(i1+1,i2,
     & i3,ey))/(dx1(0)**2)
              v1yy = (u1(i1,i2-1,i3,ey)-2.*u1(i1,i2,i3,ey)+u1(i1,i2+1,
     & i3,ey))/(dx1(1)**2)
            v1Lap = v1xx+ v1yy
           ! NOTE: the jacobian derivatives can be computed once for all components
             uu2=u2(j1,j2,j3,ex) ! in the rectangular case just eval the solution
              u2x = (-u2(j1-1,j2,j3,ex)+u2(j1+1,j2,j3,ex))/(2.*dx2(0))
              u2y = (-u2(j1,j2-1,j3,ex)+u2(j1,j2+1,j3,ex))/(2.*dx2(1))
              u2xx = (u2(j1-1,j2,j3,ex)-2.*u2(j1,j2,j3,ex)+u2(j1+1,j2,
     & j3,ex))/(dx2(0)**2)
              u2yy = (u2(j1,j2-1,j3,ex)-2.*u2(j1,j2,j3,ex)+u2(j1,j2+1,
     & j3,ex))/(dx2(1)**2)
            u2Lap = u2xx+ u2yy
             vv2=u2(j1,j2,j3,ey) ! in the rectangular case just eval the solution
              v2x = (-u2(j1-1,j2,j3,ey)+u2(j1+1,j2,j3,ey))/(2.*dx2(0))
              v2y = (-u2(j1,j2-1,j3,ey)+u2(j1,j2+1,j3,ey))/(2.*dx2(1))
              v2xx = (u2(j1-1,j2,j3,ey)-2.*u2(j1,j2,j3,ey)+u2(j1+1,j2,
     & j3,ey))/(dx2(0)**2)
              v2yy = (u2(j1,j2-1,j3,ey)-2.*u2(j1,j2,j3,ey)+u2(j1,j2+1,
     & j3,ey))/(dx2(1)**2)
            v2Lap = v2xx+ v2yy

          f(0)=(u1x+v1y) - (u2x+v2y)
          f(1)=(u1xx+u1yy) - (u2xx+u2yy)

          f(2)=(v1x-u1y)/mu1 - (v2x-u2y)/mu2

          f(3)=(v1xx+v1yy)/epsmu1 - (v2xx+v2yy)/epsmu2

          ! write(debugFile,'(" --> i1,i2=",2i4," f(start)=",4f8.3)') i1,i2,f(0),f(1),f(2),f(3)

           ! here is the matrix of coefficients for the unknowns u1(-1),v1(-1),u2(-1),v2(-1)
           ! Solve:
           !     
           !       A [ U ] = A [ U(old) ] - [ f ]
           if( axis1.eq.0 )then
             a4(0,0) = -is1/(2.*dx1(0))    ! coeff of u1(-1) from [u.x+v.y]
             a4(0,1) = 0.                  ! coeff of v1(-1) from [u.x+v.y]

             a4(2,0) = 0.
             a4(2,1) = -is1/(2.*dx1(0))    ! coeff of v1(-1) from [v.x - u.y]
           else
             a4(0,0) = 0.
             a4(0,1) = -is2/(2.*dx1(1))    ! coeff of v1(-1) from [u.x+v.y]

             a4(2,0) =  is2/(2.*dx1(1))    ! coeff of u1(-1) from [v.x - u.y]
             a4(2,1) = 0.
           end if
           if( axis2.eq.0 )then
             a4(0,2) = js1/(2.*dx2(0))    ! coeff of u2(-1) from [u.x+v.y]
             a4(0,3) = 0.

             a4(2,2) = 0.
             a4(2,3) = js1/(2.*dx2(0))    ! coeff of v2(-1) from [v.x - u.y]
           else
             a4(0,2) = 0.
             a4(0,3) = js2/(2.*dx2(1))    ! coeff of v2(-1) from [u.x+v.y]

             a4(2,2) =-js2/(2.*dx2(1))    ! coeff of u2(-1) from [v.x - u.y]
             a4(2,3) = 0.
           end if

           a4(1,0) = 1./(dx1(axis1)**2)   ! coeff of u1(-1) from [u.xx + u.yy]
           a4(1,1) = 0.
           a4(1,2) =-1./(dx2(axis2)**2)   ! coeff of u2(-1) from [u.xx + u.yy]
           a4(1,3) = 0.

           a4(3,0) = 0.
           a4(3,1) = 1./(dx1(axis1)**2)/eps1 ! coeff of v1(-1) from [(v.xx+v.yy)/eps]
           a4(3,2) = 0.
           a4(3,3) =-1./(dx2(axis2)**2)/eps2 ! coeff of v2(-1) from [(v.xx+v.yy)/eps]


           q(0) = u1(i1-is1,i2-is2,i3,ex)
           q(1) = u1(i1-is1,i2-is2,i3,ey)
           q(2) = u2(j1-js1,j2-js2,j3,ex)
           q(3) = u2(j1-js1,j2-js2,j3,ey)

           ! subtract off the contributions from the wrong values at the ghost points:
           do n=0,3
             f(n) = (a4(n,0)*q(0)+a4(n,1)*q(1)+a4(n,2)*q(2)+a4(n,3)*q(
     & 3)) - f(n)
           end do
      ! write(debugFile,'(" --> i1,i2=",2i4," f(subtract)=",4f8.3)') i1,i2,f(0),f(1),f(2),f(3)
           ! solve A Q = F
           ! factor the matrix
           numberOfEquations=4
           call dgeco( a4(0,0), numberOfEquations, numberOfEquations, 
     & ipvt(0),rcond,work(0))
           ! solve
      ! write(debugFile,'(" --> i1,i2=",2i4," rcond=",e10.2)') i1,i2,rcond
           job=0
           call dgesl( a4(0,0), numberOfEquations, numberOfEquations, 
     & ipvt(0), f(0), job)
      ! write(debugFile,'(" --> i1,i2=",2i4," f(solve)=",4f8.3)') i1,i2,f(0),f(1),f(2),f(3)

           u1(i1-is1,i2-is2,i3,ex)=f(0)
           u1(i1-is1,i2-is2,i3,ey)=f(1)
           u2(j1-js1,j2-js2,j3,ex)=f(2)
           u2(j1-js1,j2-js2,j3,ey)=f(3)

      if( debug.gt.2 )then ! re-evaluate
           ! NOTE: the jacobian derivatives can be computed once for all components
             uu1=u1(i1,i2,i3,ex) ! in the rectangular case just eval the solution
              u1x = (-u1(i1-1,i2,i3,ex)+u1(i1+1,i2,i3,ex))/(2.*dx1(0))
              u1y = (-u1(i1,i2-1,i3,ex)+u1(i1,i2+1,i3,ex))/(2.*dx1(1))
              u1xx = (u1(i1-1,i2,i3,ex)-2.*u1(i1,i2,i3,ex)+u1(i1+1,i2,
     & i3,ex))/(dx1(0)**2)
              u1yy = (u1(i1,i2-1,i3,ex)-2.*u1(i1,i2,i3,ex)+u1(i1,i2+1,
     & i3,ex))/(dx1(1)**2)
            u1Lap = u1xx+ u1yy
             vv1=u1(i1,i2,i3,ey) ! in the rectangular case just eval the solution
              v1x = (-u1(i1-1,i2,i3,ey)+u1(i1+1,i2,i3,ey))/(2.*dx1(0))
              v1y = (-u1(i1,i2-1,i3,ey)+u1(i1,i2+1,i3,ey))/(2.*dx1(1))
              v1xx = (u1(i1-1,i2,i3,ey)-2.*u1(i1,i2,i3,ey)+u1(i1+1,i2,
     & i3,ey))/(dx1(0)**2)
              v1yy = (u1(i1,i2-1,i3,ey)-2.*u1(i1,i2,i3,ey)+u1(i1,i2+1,
     & i3,ey))/(dx1(1)**2)
            v1Lap = v1xx+ v1yy
           ! NOTE: the jacobian derivatives can be computed once for all components
             uu2=u2(j1,j2,j3,ex) ! in the rectangular case just eval the solution
              u2x = (-u2(j1-1,j2,j3,ex)+u2(j1+1,j2,j3,ex))/(2.*dx2(0))
              u2y = (-u2(j1,j2-1,j3,ex)+u2(j1,j2+1,j3,ex))/(2.*dx2(1))
              u2xx = (u2(j1-1,j2,j3,ex)-2.*u2(j1,j2,j3,ex)+u2(j1+1,j2,
     & j3,ex))/(dx2(0)**2)
              u2yy = (u2(j1,j2-1,j3,ex)-2.*u2(j1,j2,j3,ex)+u2(j1,j2+1,
     & j3,ex))/(dx2(1)**2)
            u2Lap = u2xx+ u2yy
             vv2=u2(j1,j2,j3,ey) ! in the rectangular case just eval the solution
              v2x = (-u2(j1-1,j2,j3,ey)+u2(j1+1,j2,j3,ey))/(2.*dx2(0))
              v2y = (-u2(j1,j2-1,j3,ey)+u2(j1,j2+1,j3,ey))/(2.*dx2(1))
              v2xx = (u2(j1-1,j2,j3,ey)-2.*u2(j1,j2,j3,ey)+u2(j1+1,j2,
     & j3,ey))/(dx2(0)**2)
              v2yy = (u2(j1,j2-1,j3,ey)-2.*u2(j1,j2,j3,ey)+u2(j1,j2+1,
     & j3,ey))/(dx2(1)**2)
            v2Lap = v2xx+ v2yy
          f(0)=(u1x+v1y) - (u2x+v2y)
          f(1)=(u1xx+u1yy) - (u2xx+u2yy)
          f(2)=(v1x-u1y)/mu1 - (v2x-u2y)/mu2
          f(3)=(v1xx+v1yy)/epsmu1 - (v2xx+v2yy)/epsmu2
        write(debugFile,'(" --> i1,i2=",2i4," f(re-eval)=",4e10.2)') 
     & i1,i2,f(0),f(1),f(2),f(3)
      end if

           ! do this for now
           u1(i1-is1,i2-is2,i3,hz)=u2(j1+js1,j2+js2,j3,hz)
           u2(j1-js1,j2-js2,j3,hz)=u1(i1+is1,i2+is2,i3,hz)


            end if
            j1=j1+1
           end do
           j2=j2+1
          end do

         ! opt periodic update
         if( parallel.eq.0 )then
          axisp1=mod(axis1+1,nd)
          if( boundaryCondition1(0,axisp1).lt.0 )then
           ! direction axisp1 is periodic
           diff(axis1)=0
           diff(axisp1)=gridIndexRange1(1,axisp1)-gridIndexRange1(0,
     & axisp1)
           if( side1.eq.0 )then
             ! assign 4 ghost points outside lower corner
             np1a=gridIndexRange1(0,0)-2
             np1b=gridIndexRange1(0,0)-1
             np2a=gridIndexRange1(0,1)-2
             np2b=gridIndexRange1(0,1)-1
             do i3=n3a,n3b
             do i2=np2a,np2b
             do i1=np1a,np1b
             do n=ex,hz
               ! write(*,'(" periodic i1,i2,i3,n=",4i4)') i1,i2,i3,n
              u1(i1,i2,i3,n) = u1(i1+diff(0),i2+diff(1),i3,n)
             end do
             end do
             end do
             end do
             ! assign 4 ghost points outside upper corner
             if( axis1.eq.0 )then
               np2a=gridIndexRange1(1,axisp1)+1
               np2b=gridIndexRange1(1,axisp1)+2
             else
               np1a=gridIndexRange1(1,axisp1)+1
               np1b=gridIndexRange1(1,axisp1)+2
             end if
             do i3=n3a,n3b
             do i2=np2a,np2b
             do i1=np1a,np1b
             do n=ex,hz
               ! write(*,'(" periodic i1,i2,i3,n=",4i4)') i1,i2,i3,n
              u1(i1,i2,i3,n) = u1(i1-diff(0),i2-diff(1),i3,n)
             end do
             end do
             end do
             end do
           else
             ! assign 4 ghost points outside upper corner
             np1a=gridIndexRange1(1,0)+1
             np1b=gridIndexRange1(1,0)+2
             np2a=gridIndexRange1(1,1)+1
             np2b=gridIndexRange1(1,1)+2
             do i3=n3a,n3b
             do i2=np2a,np2b
             do i1=np1a,np1b
             do n=ex,hz
               ! write(*,'(" periodic i1,i2,i3,n=",4i4)') i1,i2,i3,n
              u1(i1,i2,i3,n) = u1(i1-diff(0),i2-diff(1),i3,n)
             end do
             end do
             end do
             end do
             if( axis1.eq.0 )then
               np2a=gridIndexRange1(0,axisp1)-2
               np2b=gridIndexRange1(0,axisp1)-1
             else
               np1a=gridIndexRange1(0,axisp1)-2
               np1b=gridIndexRange1(0,axisp1)-1
             end if
             do i3=n3a,n3b
             do i2=np2a,np2b
             do i1=np1a,np1b
             do n=ex,hz
               ! write(*,'(" periodic i1,i2,i3,n=",4i4)') i1,i2,i3,n
              u1(i1,i2,i3,n) = u1(i1+diff(0),i2+diff(1),i3,n)
             end do
             end do
             end do
             end do
           end if
          endif
         end if
         if( parallel.eq.0 )then
          axisp1=mod(axis2+1,nd)
          if( boundaryCondition2(0,axisp1).lt.0 )then
           ! direction axisp1 is periodic
           diff(axis2)=0
           diff(axisp1)=gridIndexRange2(1,axisp1)-gridIndexRange2(0,
     & axisp1)
           if( side2.eq.0 )then
             ! assign 4 ghost points outside lower corner
             np1a=gridIndexRange2(0,0)-2
             np1b=gridIndexRange2(0,0)-1
             np2a=gridIndexRange2(0,1)-2
             np2b=gridIndexRange2(0,1)-1
             do i3=n3a,n3b
             do i2=np2a,np2b
             do i1=np1a,np1b
             do n=ex,hz
               ! write(*,'(" periodic i1,i2,i3,n=",4i4)') i1,i2,i3,n
              u2(i1,i2,i3,n) = u2(i1+diff(0),i2+diff(1),i3,n)
             end do
             end do
             end do
             end do
             ! assign 4 ghost points outside upper corner
             if( axis2.eq.0 )then
               np2a=gridIndexRange2(1,axisp1)+1
               np2b=gridIndexRange2(1,axisp1)+2
             else
               np1a=gridIndexRange2(1,axisp1)+1
               np1b=gridIndexRange2(1,axisp1)+2
             end if
             do i3=n3a,n3b
             do i2=np2a,np2b
             do i1=np1a,np1b
             do n=ex,hz
               ! write(*,'(" periodic i1,i2,i3,n=",4i4)') i1,i2,i3,n
              u2(i1,i2,i3,n) = u2(i1-diff(0),i2-diff(1),i3,n)
             end do
             end do
             end do
             end do
           else
             ! assign 4 ghost points outside upper corner
             np1a=gridIndexRange2(1,0)+1
             np1b=gridIndexRange2(1,0)+2
             np2a=gridIndexRange2(1,1)+1
             np2b=gridIndexRange2(1,1)+2
             do i3=n3a,n3b
             do i2=np2a,np2b
             do i1=np1a,np1b
             do n=ex,hz
               ! write(*,'(" periodic i1,i2,i3,n=",4i4)') i1,i2,i3,n
              u2(i1,i2,i3,n) = u2(i1-diff(0),i2-diff(1),i3,n)
             end do
             end do
             end do
             end do
             if( axis2.eq.0 )then
               np2a=gridIndexRange2(0,axisp1)-2
               np2b=gridIndexRange2(0,axisp1)-1
             else
               np1a=gridIndexRange2(0,axisp1)-2
               np1b=gridIndexRange2(0,axisp1)-1
             end if
             do i3=n3a,n3b
             do i2=np2a,np2b
             do i1=np1a,np1b
             do n=ex,hz
               ! write(*,'(" periodic i1,i2,i3,n=",4i4)') i1,i2,i3,n
              u2(i1,i2,i3,n) = u2(i1+diff(0),i2+diff(1),i3,n)
             end do
             end do
             end do
             end do
           end if
          endif
         end if
       end if

       else if( nd.eq.2 .and. orderOfAccuracy.eq.2 .and. 
     & gridType.eq.curvilinear )then

         ! *******************************
         ! ***** 2d curvilinear case *****
         ! *******************************




         ! ---- first satisfy the jump conditions on the boundary --------
         !    [ eps n.u ] = 0
         !    [ tau.u ] = 0
           if( eps1.lt.eps2 )then
             epsRatio=eps1/eps2
              i3=n3a
              j3=m3a
              j2=mm2a
              do i2=nn2a,nn2b
               j1=mm1a
               do i1=nn1a,nn1b
               if( mask1(i1,i2,i3).gt.0 .and. mask2(j1,j2,j3).gt.0 )
     & then
               ! eps2 n.u2 = eps1 n.u1
               !     tau.u2 = tau.u1
                an1=rsxy1(i1,i2,i3,axis1,0)   ! normal (an1,an2)
                an2=rsxy1(i1,i2,i3,axis1,1)
                aNorm=max(epsx,sqrt(an1**2+an2**2))
                an1=an1/aNorm
                an2=an2/aNorm
               ua=u1(i1,i2,i3,ex)
               ub=u1(i1,i2,i3,ey)
               nDotU = an1*ua+an2*ub
               if( twilightZone.eq.1 )then
                ! adjust for TZ forcing (here we assume the exact solution is the same on both sides)
                call ogderiv(ep, 0,0,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),0.,t, ex, ue )
                call ogderiv(ep, 0,0,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),0.,t, ey, ve )
                nDotU = nDotU - (an1*ue+an2*ve)
               end if
               ! u2 equals u1 but with normal component = eps1/eps2*(n.u1)
               u2(j1,j2,j3,ex) = ua + (nDotU*epsRatio - nDotU)*an1
               u2(j1,j2,j3,ey) = ub + (nDotU*epsRatio - nDotU)*an2
               u2(j1,j2,j3,hz) = u1(i1,i2,i3,hz)
                end if
                j1=j1+1
               end do
               j2=j2+1
              end do
           else
             epsRatio=eps2/eps1
              i3=n3a
              j3=m3a
              j2=mm2a
              do i2=nn2a,nn2b
               j1=mm1a
               do i1=nn1a,nn1b
               if( mask1(i1,i2,i3).gt.0 .and. mask2(j1,j2,j3).gt.0 )
     & then
               ! eps2 n.u2 = eps1 n.u1
               !     tau.u2 = tau.u1
                an1=rsxy1(i1,i2,i3,axis1,0)
                an2=rsxy1(i1,i2,i3,axis1,1)
                aNorm=max(epsx,sqrt(an1**2+an2**2))
                an1=an1/aNorm
                an2=an2/aNorm
               ua=u2(j1,j2,j3,ex)
               ub=u2(j1,j2,j3,ey)
               nDotU = an1*ua+an2*ub
               if( twilightZone.eq.1 )then
                ! adjust for TZ forcing (here we assume the exact solution is the same on both sides)
                call ogderiv(ep, 0,0,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),0.,t, ex, ue )
                call ogderiv(ep, 0,0,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),0.,t, ey, ve )
         ! write(*,'(" jump: x,y=",2e10.2," ua,ue=",2e10.2," ub,ve=",2e10.2)') xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),ua,ue,ub,ve
                nDotU = nDotU - (an1*ue+an2*ve)
               end if
               u1(i1,i2,i3,ex) = ua + (nDotU*epsRatio - nDotU)*an1
               u1(i1,i2,i3,ey) = ub + (nDotU*epsRatio - nDotU)*an2
               u1(i1,i2,i3,hz) = u2(j1,j2,j3,hz)
                end if
                j1=j1+1
               end do
               j2=j2+1
              end do
           end if

         ! initialization step: assign first ghost line by extrapolation
         ! NOTE: assign ghost points outside the ends
          i3=n3a
          j3=m3a
          j2=mm2a
          do i2=nn2a,nn2b
           j1=mm1a
           do i1=nn1a,nn1b
            u1(i1-is1,i2-is2,i3,ex)=(3.*u1(i1,i2,i3,ex)-3.*u1(i1+is1,
     & i2+is2,i3+is3,ex)+u1(i1+2*is1,i2+2*is2,i3+2*is3,ex))
            u1(i1-is1,i2-is2,i3,ey)=(3.*u1(i1,i2,i3,ey)-3.*u1(i1+is1,
     & i2+is2,i3+is3,ey)+u1(i1+2*is1,i2+2*is2,i3+2*is3,ey))
            u1(i1-is1,i2-is2,i3,hz)=(3.*u1(i1,i2,i3,hz)-3.*u1(i1+is1,
     & i2+is2,i3+is3,hz)+u1(i1+2*is1,i2+2*is2,i3+2*is3,hz))
c
            u2(j1-js1,j2-js2,j3,ex)=(3.*u2(j1,j2,j3,ex)-3.*u2(j1+js1,
     & j2+js2,j3+js3,ex)+u2(j1+2*js1,j2+2*js2,j3+2*js3,ex))
            u2(j1-js1,j2-js2,j3,ey)=(3.*u2(j1,j2,j3,ey)-3.*u2(j1+js1,
     & j2+js2,j3+js3,ey)+u2(j1+2*js1,j2+2*js2,j3+2*js3,ey))
            u2(j1-js1,j2-js2,j3,hz)=(3.*u2(j1,j2,j3,hz)-3.*u2(j1+js1,
     & j2+js2,j3+js3,hz)+u2(j1+2*js1,j2+2*js2,j3+2*js3,hz))

            j1=j1+1
           end do
           j2=j2+1
          end do

         ! here are the real jump conditions for the ghost points
         !   [ u.x + v.y ] = 0 = [ rx*ur + ry*vr + sx*us + sy*vs ] 
         !   [ n.(uv.xx + uv.yy) ] = 0
         !   [ v.x - u.y ] =0 
         !   [ tau.(uv.xx+uv.yy)/eps ] = 0

         ! ***** fix these for [mu] != 0 ****
         if( mu1.ne.mu2 )then
           stop 9923
         end if
          i3=n3a
          j3=m3a
          j2=m2a
          do i2=n2a,n2b
           j1=m1a
           do i1=n1a,n1b
            if( mask1(i1,i2,i3).gt.0 .and. mask2(j1,j2,j3).gt.0 )then

           ! here is the normal (assumed to be the same on both sides)
           an1=rsxy1(i1,i2,i3,axis1,0)   ! normal (an1,an2)
           an2=rsxy1(i1,i2,i3,axis1,1)
           aNorm=max(epsx,sqrt(an1**2+an2**2))
           an1=an1/aNorm
           an2=an2/aNorm
           tau1=-an2
           tau2= an1

           ! first evaluate the equations we want to solve with the wrong values at the ghost points:


            ! NOTE: the jacobian derivatives can be computed once for all components
             ! this next call will define the jacobian and its derivatives (parameteric and spatial)
             aj1rx = rsxy1(i1,i2,i3,0,0)
             aj1rxr = (-rsxy1(i1-1,i2,i3,0,0)+rsxy1(i1+1,i2,i3,0,0))/(
     & 2.*dr1(0))
             aj1rxs = (-rsxy1(i1,i2-1,i3,0,0)+rsxy1(i1,i2+1,i3,0,0))/(
     & 2.*dr1(1))
             aj1sx = rsxy1(i1,i2,i3,1,0)
             aj1sxr = (-rsxy1(i1-1,i2,i3,1,0)+rsxy1(i1+1,i2,i3,1,0))/(
     & 2.*dr1(0))
             aj1sxs = (-rsxy1(i1,i2-1,i3,1,0)+rsxy1(i1,i2+1,i3,1,0))/(
     & 2.*dr1(1))
             aj1ry = rsxy1(i1,i2,i3,0,1)
             aj1ryr = (-rsxy1(i1-1,i2,i3,0,1)+rsxy1(i1+1,i2,i3,0,1))/(
     & 2.*dr1(0))
             aj1rys = (-rsxy1(i1,i2-1,i3,0,1)+rsxy1(i1,i2+1,i3,0,1))/(
     & 2.*dr1(1))
             aj1sy = rsxy1(i1,i2,i3,1,1)
             aj1syr = (-rsxy1(i1-1,i2,i3,1,1)+rsxy1(i1+1,i2,i3,1,1))/(
     & 2.*dr1(0))
             aj1sys = (-rsxy1(i1,i2-1,i3,1,1)+rsxy1(i1,i2+1,i3,1,1))/(
     & 2.*dr1(1))
             aj1rxx = aj1rx*aj1rxr+aj1sx*aj1rxs
             aj1rxy = aj1ry*aj1rxr+aj1sy*aj1rxs
             aj1sxx = aj1rx*aj1sxr+aj1sx*aj1sxs
             aj1sxy = aj1ry*aj1sxr+aj1sy*aj1sxs
             aj1ryx = aj1rx*aj1ryr+aj1sx*aj1rys
             aj1ryy = aj1ry*aj1ryr+aj1sy*aj1rys
             aj1syx = aj1rx*aj1syr+aj1sx*aj1sys
             aj1syy = aj1ry*aj1syr+aj1sy*aj1sys
              uu1 = u1(i1,i2,i3,ex)
              uu1r = (-u1(i1-1,i2,i3,ex)+u1(i1+1,i2,i3,ex))/(2.*dr1(0))
              uu1s = (-u1(i1,i2-1,i3,ex)+u1(i1,i2+1,i3,ex))/(2.*dr1(1))
              uu1rr = (u1(i1-1,i2,i3,ex)-2.*u1(i1,i2,i3,ex)+u1(i1+1,i2,
     & i3,ex))/(dr1(0)**2)
              uu1rs = (-(-u1(i1-1,i2-1,i3,ex)+u1(i1-1,i2+1,i3,ex))/(2.*
     & dr1(1))+(-u1(i1+1,i2-1,i3,ex)+u1(i1+1,i2+1,i3,ex))/(2.*dr1(1)))
     & /(2.*dr1(0))
              uu1ss = (u1(i1,i2-1,i3,ex)-2.*u1(i1,i2,i3,ex)+u1(i1,i2+1,
     & i3,ex))/(dr1(1)**2)
               u1x = aj1rx*uu1r+aj1sx*uu1s
               u1y = aj1ry*uu1r+aj1sy*uu1s
               t1 = aj1rx**2
               t6 = aj1sx**2
               u1xx = t1*uu1rr+2*aj1rx*aj1sx*uu1rs+t6*uu1ss+aj1rxx*
     & uu1r+aj1sxx*uu1s
               t1 = aj1ry**2
               t6 = aj1sy**2
               u1yy = t1*uu1rr+2*aj1ry*aj1sy*uu1rs+t6*uu1ss+aj1ryy*
     & uu1r+aj1syy*uu1s
             u1Lap = u1xx+ u1yy
              vv1 = u1(i1,i2,i3,ey)
              vv1r = (-u1(i1-1,i2,i3,ey)+u1(i1+1,i2,i3,ey))/(2.*dr1(0))
              vv1s = (-u1(i1,i2-1,i3,ey)+u1(i1,i2+1,i3,ey))/(2.*dr1(1))
              vv1rr = (u1(i1-1,i2,i3,ey)-2.*u1(i1,i2,i3,ey)+u1(i1+1,i2,
     & i3,ey))/(dr1(0)**2)
              vv1rs = (-(-u1(i1-1,i2-1,i3,ey)+u1(i1-1,i2+1,i3,ey))/(2.*
     & dr1(1))+(-u1(i1+1,i2-1,i3,ey)+u1(i1+1,i2+1,i3,ey))/(2.*dr1(1)))
     & /(2.*dr1(0))
              vv1ss = (u1(i1,i2-1,i3,ey)-2.*u1(i1,i2,i3,ey)+u1(i1,i2+1,
     & i3,ey))/(dr1(1)**2)
               v1x = aj1rx*vv1r+aj1sx*vv1s
               v1y = aj1ry*vv1r+aj1sy*vv1s
               t1 = aj1rx**2
               t6 = aj1sx**2
               v1xx = t1*vv1rr+2*aj1rx*aj1sx*vv1rs+t6*vv1ss+aj1rxx*
     & vv1r+aj1sxx*vv1s
               t1 = aj1ry**2
               t6 = aj1sy**2
               v1yy = t1*vv1rr+2*aj1ry*aj1sy*vv1rs+t6*vv1ss+aj1ryy*
     & vv1r+aj1syy*vv1s
             v1Lap = v1xx+ v1yy
            ! NOTE: the jacobian derivatives can be computed once for all components
             ! this next call will define the jacobian and its derivatives (parameteric and spatial)
             aj2rx = rsxy2(j1,j2,j3,0,0)
             aj2rxr = (-rsxy2(j1-1,j2,j3,0,0)+rsxy2(j1+1,j2,j3,0,0))/(
     & 2.*dr2(0))
             aj2rxs = (-rsxy2(j1,j2-1,j3,0,0)+rsxy2(j1,j2+1,j3,0,0))/(
     & 2.*dr2(1))
             aj2sx = rsxy2(j1,j2,j3,1,0)
             aj2sxr = (-rsxy2(j1-1,j2,j3,1,0)+rsxy2(j1+1,j2,j3,1,0))/(
     & 2.*dr2(0))
             aj2sxs = (-rsxy2(j1,j2-1,j3,1,0)+rsxy2(j1,j2+1,j3,1,0))/(
     & 2.*dr2(1))
             aj2ry = rsxy2(j1,j2,j3,0,1)
             aj2ryr = (-rsxy2(j1-1,j2,j3,0,1)+rsxy2(j1+1,j2,j3,0,1))/(
     & 2.*dr2(0))
             aj2rys = (-rsxy2(j1,j2-1,j3,0,1)+rsxy2(j1,j2+1,j3,0,1))/(
     & 2.*dr2(1))
             aj2sy = rsxy2(j1,j2,j3,1,1)
             aj2syr = (-rsxy2(j1-1,j2,j3,1,1)+rsxy2(j1+1,j2,j3,1,1))/(
     & 2.*dr2(0))
             aj2sys = (-rsxy2(j1,j2-1,j3,1,1)+rsxy2(j1,j2+1,j3,1,1))/(
     & 2.*dr2(1))
             aj2rxx = aj2rx*aj2rxr+aj2sx*aj2rxs
             aj2rxy = aj2ry*aj2rxr+aj2sy*aj2rxs
             aj2sxx = aj2rx*aj2sxr+aj2sx*aj2sxs
             aj2sxy = aj2ry*aj2sxr+aj2sy*aj2sxs
             aj2ryx = aj2rx*aj2ryr+aj2sx*aj2rys
             aj2ryy = aj2ry*aj2ryr+aj2sy*aj2rys
             aj2syx = aj2rx*aj2syr+aj2sx*aj2sys
             aj2syy = aj2ry*aj2syr+aj2sy*aj2sys
              uu2 = u2(j1,j2,j3,ex)
              uu2r = (-u2(j1-1,j2,j3,ex)+u2(j1+1,j2,j3,ex))/(2.*dr2(0))
              uu2s = (-u2(j1,j2-1,j3,ex)+u2(j1,j2+1,j3,ex))/(2.*dr2(1))
              uu2rr = (u2(j1-1,j2,j3,ex)-2.*u2(j1,j2,j3,ex)+u2(j1+1,j2,
     & j3,ex))/(dr2(0)**2)
              uu2rs = (-(-u2(j1-1,j2-1,j3,ex)+u2(j1-1,j2+1,j3,ex))/(2.*
     & dr2(1))+(-u2(j1+1,j2-1,j3,ex)+u2(j1+1,j2+1,j3,ex))/(2.*dr2(1)))
     & /(2.*dr2(0))
              uu2ss = (u2(j1,j2-1,j3,ex)-2.*u2(j1,j2,j3,ex)+u2(j1,j2+1,
     & j3,ex))/(dr2(1)**2)
               u2x = aj2rx*uu2r+aj2sx*uu2s
               u2y = aj2ry*uu2r+aj2sy*uu2s
               t1 = aj2rx**2
               t6 = aj2sx**2
               u2xx = t1*uu2rr+2*aj2rx*aj2sx*uu2rs+t6*uu2ss+aj2rxx*
     & uu2r+aj2sxx*uu2s
               t1 = aj2ry**2
               t6 = aj2sy**2
               u2yy = t1*uu2rr+2*aj2ry*aj2sy*uu2rs+t6*uu2ss+aj2ryy*
     & uu2r+aj2syy*uu2s
             u2Lap = u2xx+ u2yy
              vv2 = u2(j1,j2,j3,ey)
              vv2r = (-u2(j1-1,j2,j3,ey)+u2(j1+1,j2,j3,ey))/(2.*dr2(0))
              vv2s = (-u2(j1,j2-1,j3,ey)+u2(j1,j2+1,j3,ey))/(2.*dr2(1))
              vv2rr = (u2(j1-1,j2,j3,ey)-2.*u2(j1,j2,j3,ey)+u2(j1+1,j2,
     & j3,ey))/(dr2(0)**2)
              vv2rs = (-(-u2(j1-1,j2-1,j3,ey)+u2(j1-1,j2+1,j3,ey))/(2.*
     & dr2(1))+(-u2(j1+1,j2-1,j3,ey)+u2(j1+1,j2+1,j3,ey))/(2.*dr2(1)))
     & /(2.*dr2(0))
              vv2ss = (u2(j1,j2-1,j3,ey)-2.*u2(j1,j2,j3,ey)+u2(j1,j2+1,
     & j3,ey))/(dr2(1)**2)
               v2x = aj2rx*vv2r+aj2sx*vv2s
               v2y = aj2ry*vv2r+aj2sy*vv2s
               t1 = aj2rx**2
               t6 = aj2sx**2
               v2xx = t1*vv2rr+2*aj2rx*aj2sx*vv2rs+t6*vv2ss+aj2rxx*
     & vv2r+aj2sxx*vv2s
               t1 = aj2ry**2
               t6 = aj2sy**2
               v2yy = t1*vv2rr+2*aj2ry*aj2sy*vv2rs+t6*vv2ss+aj2ryy*
     & vv2r+aj2syy*vv2s
             v2Lap = v2xx+ v2yy
            f(0)=(u1x+v1y) - (u2x+v2y)
            f(1)=( an1*u1Lap +an2*v1Lap )- ( an1*u2Lap +an2*v2Lap )
            f(2)=(v1x-u1y) - (v2x-u2y)
            f(3)=( tau1*u1Lap +tau2*v1Lap )/eps1 - ( tau1*u2Lap +tau2*
     & v2Lap )/eps2
            if( twilightZone.eq.1 )then
              call ogderiv(ep, 0,2,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, ex, uexx )
              call ogderiv(ep, 0,0,2,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, ex, ueyy )
              call ogderiv(ep, 0,2,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, ey, vexx )
              call ogderiv(ep, 0,0,2,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, ey, veyy )
              ueLap = uexx + ueyy
              veLap = vexx + veyy
              f(3) = f(3) - ( tau1*ueLap +tau2*veLap )*(1./eps1-
     & 1./eps2)
              ! write(debugFile,'(" u1Lap,ueLap=",2e10.2," v1Lap,veLap=",2e10.2)') u1Lap,ueLap,v1Lap,veLap
            end if

           ! write(debugFile,'(" --> order2-curv: i1,i2=",2i4," f(start)=",4f8.3)') i1,i2,f(0),f(1),f(2),f(3)
           ! write(debugFile,'(" --> u1(ghost),u1=",4f8.3)') u1(i1-is1,i2-is2,i3,ex),u1(i1,i2,i3,ex)
           ! write(debugFile,'(" --> u2(ghost),u2=",4f8.3)') u2(j1-js1,j2-js2,j3,ex),u2(j1,j2,j3,ex)
           ! '

           ! here is the matrix of coefficients for the unknowns u1(-1),v1(-1),u2(-1),v2(-1)
           ! Solve:
           !     
           !       A [ U ] = A [ U(old) ] - [ f ]
           a4(0,0) = -is*rsxy1(i1,i2,i3,axis1,0)/(2.*dr1(axis1))    ! coeff of u1(-1) from [u.x+v.y]
           a4(0,1) = -is*rsxy1(i1,i2,i3,axis1,1)/(2.*dr1(axis1))    ! coeff of v1(-1) from [u.x+v.y]
           a4(0,2) =  js*rsxy2(j1,j2,j3,axis2,0)/(2.*dr2(axis2))    ! coeff of u2(-1) from [u.x+v.y]
           a4(0,3) =  js*rsxy2(j1,j2,j3,axis2,1)/(2.*dr2(axis2))    ! coeff of v2(-1) from [u.x+v.y]

           a4(2,0) =  is*rsxy1(i1,i2,i3,axis1,1)/(2.*dr1(axis1))   ! coeff of u1(-1) from [v.x - u.y]
           a4(2,1) = -is*rsxy1(i1,i2,i3,axis1,0)/(2.*dr1(axis1))   ! coeff of v1(-1) from [v.x - u.y]

           a4(2,2) = -js*rsxy2(j1,j2,j3,axis2,1)/(2.*dr2(axis2))   ! coeff of u2(-1) from [v.x - u.y]
           a4(2,3) =  js*rsxy2(j1,j2,j3,axis2,0)/(2.*dr2(axis2))   ! coeff of v2(-1) from [v.x - u.y]


           ! coeff of u(-1) from lap = u.xx + u.yy
           rxx1(0,0,0)=aj1rxx
           rxx1(1,0,0)=aj1sxx
           rxx1(0,1,1)=aj1ryy
           rxx1(1,1,1)=aj1syy

           rxx2(0,0,0)=aj2rxx
           rxx2(1,0,0)=aj2sxx
           rxx2(0,1,1)=aj2ryy
           rxx2(1,1,1)=aj2syy

           ! clap1=(rsxy1(i1,i2,i3,axis1,0)**2+rsxy1(i1,i2,i3,axis1,1)**2)/(dr1(axis1)**2) !           -is*(rsxy1x22(i1,i2,i3,axis1,0)+rsxy1y22(i1,i2,i3,axis1,1))/(2.*dr1(axis1))
           ! clap2=(rsxy2(j1,j2,j3,axis2,0)**2+rsxy2(j1,j2,j3,axis2,1)**2)/(dr2(axis2)**2) !             -js*(rsxy2x22(j1,j2,j3,axis2,0)+rsxy2y22(j1,j2,j3,axis2,1))/(2.*dr2(axis2)) 
           clap1=(rsxy1(i1,i2,i3,axis1,0)**2+rsxy1(i1,i2,i3,axis1,1)**
     & 2)/(dr1(axis1)**2) -is*(rxx1(axis1,0,0)+rxx1(axis1,1,1))/(2.*
     & dr1(axis1))
           clap2=(rsxy2(j1,j2,j3,axis2,0)**2+rsxy2(j1,j2,j3,axis2,1)**
     & 2)/(dr2(axis2)**2) -js*(rxx2(axis2,0,0)+rxx2(axis2,1,1))/(2.*
     & dr2(axis2))

           !   [ n.(uv.xx + u.yy) ] = 0
           a4(1,0) = an1*clap1
           a4(1,1) = an2*clap1
           a4(1,2) =-an1*clap2
           a4(1,3) =-an2*clap2
           !   [ tau.(uv.xx+uv.yy)/eps ] = 0
           a4(3,0) = tau1*clap1/eps1
           a4(3,1) = tau2*clap1/eps1
           a4(3,2) =-tau1*clap2/eps2
           a4(3,3) =-tau2*clap2/eps2


           q(0) = u1(i1-is1,i2-is2,i3,ex)
           q(1) = u1(i1-is1,i2-is2,i3,ey)
           q(2) = u2(j1-js1,j2-js2,j3,ex)
           q(3) = u2(j1-js1,j2-js2,j3,ey)

           ! write(debugFile,'(" --> xy1=",4f8.3)') xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
           ! write(debugFile,'(" --> rsxy1=",4f8.3)') rsxy1(i1,i2,i3,0,0),rsxy1(i1,i2,i3,1,0),rsxy1(i1,i2,i3,0,1),rsxy1(i1,i2,i3,1,1)
           ! write(debugFile,'(" --> rsxy2=",4f8.3)') rsxy2(j1,j2,j3,0,0),rsxy2(j1,j2,j3,1,0),rsxy2(j1,j2,j3,0,1),rsxy2(j1,j2,j3,1,1)

           ! write(debugFile,'(" --> rxx1=",2f8.3)') rxx1(axis1,0,0),rxx1(axis1,1,1)
           ! write(debugFile,'(" --> rxx2=",2f8.3)') rxx2(axis2,0,0),rxx2(axis1,1,1)

           ! write(debugFile,'(" --> a4(0,.)=",4f8.3)') a4(0,0),a4(0,1),a4(0,2),a4(0,3)
           ! write(debugFile,'(" --> a4(1,.)=",4f8.3)') a4(1,0),a4(1,1),a4(1,2),a4(1,3)
           ! write(debugFile,'(" --> a4(2,.)=",4f8.3)') a4(2,0),a4(2,1),a4(2,2),a4(2,3)
           ! write(debugFile,'(" --> a4(3,.)=",4f8.3)') a4(3,0),a4(3,1),a4(3,2),a4(3,3)
           ! write(debugFile,'(" --> an1,an2=",2f8.3)') an1,an2
           ! write(debugFile,'(" --> clap1,clap2=",2f8.3)') clap1,clap2
           ! subtract off the contributions from the wrong values at the ghost points:
           do n=0,3
             f(n) = (a4(n,0)*q(0)+a4(n,1)*q(1)+a4(n,2)*q(2)+a4(n,3)*q(
     & 3)) - f(n)
           end do
           ! write(debugFile,'(" --> order2-curv: i1,i2=",2i4," f(subtract)=",4f8.3)') i1,i2,f(0),f(1),f(2),f(3)
           ! solve A Q = F
           ! factor the matrix
           numberOfEquations=4
           call dgeco( a4(0,0), numberOfEquations, numberOfEquations, 
     & ipvt(0),rcond,work(0))
           ! solve
           ! write(debugFile,'(" --> order2-curv: i1,i2=",2i4," rcond=",e10.2)') i1,i2,rcond
           job=0
           call dgesl( a4(0,0), numberOfEquations, numberOfEquations, 
     & ipvt(0), f(0), job)
           ! write(debugFile,'(" --> order2-curv: i1,i2=",2i4," f(solve)=",4f8.3)') i1,i2,f(0),f(1),f(2),f(3)

           u1(i1-is1,i2-is2,i3,ex)=f(0)
           u1(i1-is1,i2-is2,i3,ey)=f(1)
           u2(j1-js1,j2-js2,j3,ex)=f(2)
           u2(j1-js1,j2-js2,j3,ey)=f(3)

           if( debug.gt.3 )then ! re-evaluate
              ! NOTE: the jacobian derivatives can be computed once for all components
               ! this next call will define the jacobian and its derivatives (parameteric and spatial)
               aj1rx = rsxy1(i1,i2,i3,0,0)
               aj1rxr = (-rsxy1(i1-1,i2,i3,0,0)+rsxy1(i1+1,i2,i3,0,0))
     & /(2.*dr1(0))
               aj1rxs = (-rsxy1(i1,i2-1,i3,0,0)+rsxy1(i1,i2+1,i3,0,0))
     & /(2.*dr1(1))
               aj1sx = rsxy1(i1,i2,i3,1,0)
               aj1sxr = (-rsxy1(i1-1,i2,i3,1,0)+rsxy1(i1+1,i2,i3,1,0))
     & /(2.*dr1(0))
               aj1sxs = (-rsxy1(i1,i2-1,i3,1,0)+rsxy1(i1,i2+1,i3,1,0))
     & /(2.*dr1(1))
               aj1ry = rsxy1(i1,i2,i3,0,1)
               aj1ryr = (-rsxy1(i1-1,i2,i3,0,1)+rsxy1(i1+1,i2,i3,0,1))
     & /(2.*dr1(0))
               aj1rys = (-rsxy1(i1,i2-1,i3,0,1)+rsxy1(i1,i2+1,i3,0,1))
     & /(2.*dr1(1))
               aj1sy = rsxy1(i1,i2,i3,1,1)
               aj1syr = (-rsxy1(i1-1,i2,i3,1,1)+rsxy1(i1+1,i2,i3,1,1))
     & /(2.*dr1(0))
               aj1sys = (-rsxy1(i1,i2-1,i3,1,1)+rsxy1(i1,i2+1,i3,1,1))
     & /(2.*dr1(1))
               aj1rxx = aj1rx*aj1rxr+aj1sx*aj1rxs
               aj1rxy = aj1ry*aj1rxr+aj1sy*aj1rxs
               aj1sxx = aj1rx*aj1sxr+aj1sx*aj1sxs
               aj1sxy = aj1ry*aj1sxr+aj1sy*aj1sxs
               aj1ryx = aj1rx*aj1ryr+aj1sx*aj1rys
               aj1ryy = aj1ry*aj1ryr+aj1sy*aj1rys
               aj1syx = aj1rx*aj1syr+aj1sx*aj1sys
               aj1syy = aj1ry*aj1syr+aj1sy*aj1sys
                uu1 = u1(i1,i2,i3,ex)
                uu1r = (-u1(i1-1,i2,i3,ex)+u1(i1+1,i2,i3,ex))/(2.*dr1(
     & 0))
                uu1s = (-u1(i1,i2-1,i3,ex)+u1(i1,i2+1,i3,ex))/(2.*dr1(
     & 1))
                uu1rr = (u1(i1-1,i2,i3,ex)-2.*u1(i1,i2,i3,ex)+u1(i1+1,
     & i2,i3,ex))/(dr1(0)**2)
                uu1rs = (-(-u1(i1-1,i2-1,i3,ex)+u1(i1-1,i2+1,i3,ex))/(
     & 2.*dr1(1))+(-u1(i1+1,i2-1,i3,ex)+u1(i1+1,i2+1,i3,ex))/(2.*dr1(
     & 1)))/(2.*dr1(0))
                uu1ss = (u1(i1,i2-1,i3,ex)-2.*u1(i1,i2,i3,ex)+u1(i1,i2+
     & 1,i3,ex))/(dr1(1)**2)
                 u1x = aj1rx*uu1r+aj1sx*uu1s
                 u1y = aj1ry*uu1r+aj1sy*uu1s
                 t1 = aj1rx**2
                 t6 = aj1sx**2
                 u1xx = t1*uu1rr+2*aj1rx*aj1sx*uu1rs+t6*uu1ss+aj1rxx*
     & uu1r+aj1sxx*uu1s
                 t1 = aj1ry**2
                 t6 = aj1sy**2
                 u1yy = t1*uu1rr+2*aj1ry*aj1sy*uu1rs+t6*uu1ss+aj1ryy*
     & uu1r+aj1syy*uu1s
               u1Lap = u1xx+ u1yy
                vv1 = u1(i1,i2,i3,ey)
                vv1r = (-u1(i1-1,i2,i3,ey)+u1(i1+1,i2,i3,ey))/(2.*dr1(
     & 0))
                vv1s = (-u1(i1,i2-1,i3,ey)+u1(i1,i2+1,i3,ey))/(2.*dr1(
     & 1))
                vv1rr = (u1(i1-1,i2,i3,ey)-2.*u1(i1,i2,i3,ey)+u1(i1+1,
     & i2,i3,ey))/(dr1(0)**2)
                vv1rs = (-(-u1(i1-1,i2-1,i3,ey)+u1(i1-1,i2+1,i3,ey))/(
     & 2.*dr1(1))+(-u1(i1+1,i2-1,i3,ey)+u1(i1+1,i2+1,i3,ey))/(2.*dr1(
     & 1)))/(2.*dr1(0))
                vv1ss = (u1(i1,i2-1,i3,ey)-2.*u1(i1,i2,i3,ey)+u1(i1,i2+
     & 1,i3,ey))/(dr1(1)**2)
                 v1x = aj1rx*vv1r+aj1sx*vv1s
                 v1y = aj1ry*vv1r+aj1sy*vv1s
                 t1 = aj1rx**2
                 t6 = aj1sx**2
                 v1xx = t1*vv1rr+2*aj1rx*aj1sx*vv1rs+t6*vv1ss+aj1rxx*
     & vv1r+aj1sxx*vv1s
                 t1 = aj1ry**2
                 t6 = aj1sy**2
                 v1yy = t1*vv1rr+2*aj1ry*aj1sy*vv1rs+t6*vv1ss+aj1ryy*
     & vv1r+aj1syy*vv1s
               v1Lap = v1xx+ v1yy
              ! NOTE: the jacobian derivatives can be computed once for all components
               ! this next call will define the jacobian and its derivatives (parameteric and spatial)
               aj2rx = rsxy2(j1,j2,j3,0,0)
               aj2rxr = (-rsxy2(j1-1,j2,j3,0,0)+rsxy2(j1+1,j2,j3,0,0))
     & /(2.*dr2(0))
               aj2rxs = (-rsxy2(j1,j2-1,j3,0,0)+rsxy2(j1,j2+1,j3,0,0))
     & /(2.*dr2(1))
               aj2sx = rsxy2(j1,j2,j3,1,0)
               aj2sxr = (-rsxy2(j1-1,j2,j3,1,0)+rsxy2(j1+1,j2,j3,1,0))
     & /(2.*dr2(0))
               aj2sxs = (-rsxy2(j1,j2-1,j3,1,0)+rsxy2(j1,j2+1,j3,1,0))
     & /(2.*dr2(1))
               aj2ry = rsxy2(j1,j2,j3,0,1)
               aj2ryr = (-rsxy2(j1-1,j2,j3,0,1)+rsxy2(j1+1,j2,j3,0,1))
     & /(2.*dr2(0))
               aj2rys = (-rsxy2(j1,j2-1,j3,0,1)+rsxy2(j1,j2+1,j3,0,1))
     & /(2.*dr2(1))
               aj2sy = rsxy2(j1,j2,j3,1,1)
               aj2syr = (-rsxy2(j1-1,j2,j3,1,1)+rsxy2(j1+1,j2,j3,1,1))
     & /(2.*dr2(0))
               aj2sys = (-rsxy2(j1,j2-1,j3,1,1)+rsxy2(j1,j2+1,j3,1,1))
     & /(2.*dr2(1))
               aj2rxx = aj2rx*aj2rxr+aj2sx*aj2rxs
               aj2rxy = aj2ry*aj2rxr+aj2sy*aj2rxs
               aj2sxx = aj2rx*aj2sxr+aj2sx*aj2sxs
               aj2sxy = aj2ry*aj2sxr+aj2sy*aj2sxs
               aj2ryx = aj2rx*aj2ryr+aj2sx*aj2rys
               aj2ryy = aj2ry*aj2ryr+aj2sy*aj2rys
               aj2syx = aj2rx*aj2syr+aj2sx*aj2sys
               aj2syy = aj2ry*aj2syr+aj2sy*aj2sys
                uu2 = u2(j1,j2,j3,ex)
                uu2r = (-u2(j1-1,j2,j3,ex)+u2(j1+1,j2,j3,ex))/(2.*dr2(
     & 0))
                uu2s = (-u2(j1,j2-1,j3,ex)+u2(j1,j2+1,j3,ex))/(2.*dr2(
     & 1))
                uu2rr = (u2(j1-1,j2,j3,ex)-2.*u2(j1,j2,j3,ex)+u2(j1+1,
     & j2,j3,ex))/(dr2(0)**2)
                uu2rs = (-(-u2(j1-1,j2-1,j3,ex)+u2(j1-1,j2+1,j3,ex))/(
     & 2.*dr2(1))+(-u2(j1+1,j2-1,j3,ex)+u2(j1+1,j2+1,j3,ex))/(2.*dr2(
     & 1)))/(2.*dr2(0))
                uu2ss = (u2(j1,j2-1,j3,ex)-2.*u2(j1,j2,j3,ex)+u2(j1,j2+
     & 1,j3,ex))/(dr2(1)**2)
                 u2x = aj2rx*uu2r+aj2sx*uu2s
                 u2y = aj2ry*uu2r+aj2sy*uu2s
                 t1 = aj2rx**2
                 t6 = aj2sx**2
                 u2xx = t1*uu2rr+2*aj2rx*aj2sx*uu2rs+t6*uu2ss+aj2rxx*
     & uu2r+aj2sxx*uu2s
                 t1 = aj2ry**2
                 t6 = aj2sy**2
                 u2yy = t1*uu2rr+2*aj2ry*aj2sy*uu2rs+t6*uu2ss+aj2ryy*
     & uu2r+aj2syy*uu2s
               u2Lap = u2xx+ u2yy
                vv2 = u2(j1,j2,j3,ey)
                vv2r = (-u2(j1-1,j2,j3,ey)+u2(j1+1,j2,j3,ey))/(2.*dr2(
     & 0))
                vv2s = (-u2(j1,j2-1,j3,ey)+u2(j1,j2+1,j3,ey))/(2.*dr2(
     & 1))
                vv2rr = (u2(j1-1,j2,j3,ey)-2.*u2(j1,j2,j3,ey)+u2(j1+1,
     & j2,j3,ey))/(dr2(0)**2)
                vv2rs = (-(-u2(j1-1,j2-1,j3,ey)+u2(j1-1,j2+1,j3,ey))/(
     & 2.*dr2(1))+(-u2(j1+1,j2-1,j3,ey)+u2(j1+1,j2+1,j3,ey))/(2.*dr2(
     & 1)))/(2.*dr2(0))
                vv2ss = (u2(j1,j2-1,j3,ey)-2.*u2(j1,j2,j3,ey)+u2(j1,j2+
     & 1,j3,ey))/(dr2(1)**2)
                 v2x = aj2rx*vv2r+aj2sx*vv2s
                 v2y = aj2ry*vv2r+aj2sy*vv2s
                 t1 = aj2rx**2
                 t6 = aj2sx**2
                 v2xx = t1*vv2rr+2*aj2rx*aj2sx*vv2rs+t6*vv2ss+aj2rxx*
     & vv2r+aj2sxx*vv2s
                 t1 = aj2ry**2
                 t6 = aj2sy**2
                 v2yy = t1*vv2rr+2*aj2ry*aj2sy*vv2rs+t6*vv2ss+aj2ryy*
     & vv2r+aj2syy*vv2s
               v2Lap = v2xx+ v2yy
              f(0)=(u1x+v1y) - (u2x+v2y)
              f(1)=( an1*u1Lap +an2*v1Lap )- ( an1*u2Lap +an2*v2Lap )
              f(2)=(v1x-u1y) - (v2x-u2y)
              f(3)=( tau1*u1Lap +tau2*v1Lap )/eps1 - ( tau1*u2Lap +
     & tau2*v2Lap )/eps2
              if( twilightZone.eq.1 )then
                call ogderiv(ep, 0,2,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),0.,t, ex, uexx )
                call ogderiv(ep, 0,0,2,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),0.,t, ex, ueyy )
                call ogderiv(ep, 0,2,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),0.,t, ey, vexx )
                call ogderiv(ep, 0,0,2,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),0.,t, ey, veyy )
                ueLap = uexx + ueyy
                veLap = vexx + veyy
                f(3) = f(3) - ( tau1*ueLap +tau2*veLap )*(1./eps1-
     & 1./eps2)
                ! write(debugFile,'(" u1Lap,ueLap=",2e10.2," v1Lap,veLap=",2e10.2)') u1Lap,ueLap,v1Lap,veLap
              end if
             !write(debugFile,'(" --> order2-curv: xy1(ghost)=",2e11.3)') xy1(i1-is1,i2-is2,i3,0),xy1(i1-is1,i2-is2,i3,1)
             !write(debugFile,'(" --> order2-curv: xy2(ghost)=",2e11.3)') xy2(j1-js1,j2-js2,j3,0),xy2(j1-js1,j2-js2,j3,1)
             if( twilightZone.eq.1 )then
               call ogderiv(ep, 0,0,0,0, xy1(i1-is1,i2-is2,i3,0),xy1(
     & i1-is1,i2-is2,i3,1),0.,t, ex, uex  )
               call ogderiv(ep, 0,0,0,0, xy1(i1-is1,i2-is2,i3,0),xy1(
     & i1-is1,i2-is2,i3,1),0.,t, ey, uey  )
              write(debugFile,'(" --> order2-curv: i1,i2=",2i4," u1=",
     & 2e11.3," err=",2e11.3)') i1,i2,u1(i1-is1,i2-is2,i3,ex),u1(i1-
     & is1,i2-is2,i3,ey),u1(i1-is1,i2-is2,i3,ex)-uex,u1(i1-is1,i2-is2,
     & i3,ey)-uey
               ! '
             else
              write(debugFile,'(" --> order2-curv: i1,i2=",2i4," u1=",
     & 2e11.3)') i1,i2,u1(i1-is1,i2-is2,i3,ex),u1(i1-is1,i2-is2,i3,ey)
               ! '
             end if
             write(debugFile,'(" --> order2-curv: j1,j2=",2i4," u2=",
     & 2e11.3)') j1,j2,u2(j1-js1,j2-js2,j3,ex),u2(j1-js1,j2-js2,j3,ey)
               ! '
             write(debugFile,'(" --> order2-curv: i1,i2=",2i4," f(re-
     & eval)=",4e10.2)') i1,i2,f(0),f(1),f(2),f(3)
               ! '
           end if

           ! solve for Hz
           !  [ w.n/eps] = 0
           !  [ Lap(w)/eps] = 0

              ww1 = u1(i1,i2,i3,hz)
              ww1r = (-u1(i1-1,i2,i3,hz)+u1(i1+1,i2,i3,hz))/(2.*dr1(0))
              ww1s = (-u1(i1,i2-1,i3,hz)+u1(i1,i2+1,i3,hz))/(2.*dr1(1))
              ww1rr = (u1(i1-1,i2,i3,hz)-2.*u1(i1,i2,i3,hz)+u1(i1+1,i2,
     & i3,hz))/(dr1(0)**2)
              ww1rs = (-(-u1(i1-1,i2-1,i3,hz)+u1(i1-1,i2+1,i3,hz))/(2.*
     & dr1(1))+(-u1(i1+1,i2-1,i3,hz)+u1(i1+1,i2+1,i3,hz))/(2.*dr1(1)))
     & /(2.*dr1(0))
              ww1ss = (u1(i1,i2-1,i3,hz)-2.*u1(i1,i2,i3,hz)+u1(i1,i2+1,
     & i3,hz))/(dr1(1)**2)
               w1x = aj1rx*ww1r+aj1sx*ww1s
               w1y = aj1ry*ww1r+aj1sy*ww1s
               t1 = aj1rx**2
               t6 = aj1sx**2
               w1xx = t1*ww1rr+2*aj1rx*aj1sx*ww1rs+t6*ww1ss+aj1rxx*
     & ww1r+aj1sxx*ww1s
               t1 = aj1ry**2
               t6 = aj1sy**2
               w1yy = t1*ww1rr+2*aj1ry*aj1sy*ww1rs+t6*ww1ss+aj1ryy*
     & ww1r+aj1syy*ww1s
             w1Lap = w1xx+ w1yy
              ww2 = u2(j1,j2,j3,hz)
              ww2r = (-u2(j1-1,j2,j3,hz)+u2(j1+1,j2,j3,hz))/(2.*dr2(0))
              ww2s = (-u2(j1,j2-1,j3,hz)+u2(j1,j2+1,j3,hz))/(2.*dr2(1))
              ww2rr = (u2(j1-1,j2,j3,hz)-2.*u2(j1,j2,j3,hz)+u2(j1+1,j2,
     & j3,hz))/(dr2(0)**2)
              ww2rs = (-(-u2(j1-1,j2-1,j3,hz)+u2(j1-1,j2+1,j3,hz))/(2.*
     & dr2(1))+(-u2(j1+1,j2-1,j3,hz)+u2(j1+1,j2+1,j3,hz))/(2.*dr2(1)))
     & /(2.*dr2(0))
              ww2ss = (u2(j1,j2-1,j3,hz)-2.*u2(j1,j2,j3,hz)+u2(j1,j2+1,
     & j3,hz))/(dr2(1)**2)
               w2x = aj2rx*ww2r+aj2sx*ww2s
               w2y = aj2ry*ww2r+aj2sy*ww2s
               t1 = aj2rx**2
               t6 = aj2sx**2
               w2xx = t1*ww2rr+2*aj2rx*aj2sx*ww2rs+t6*ww2ss+aj2rxx*
     & ww2r+aj2sxx*ww2s
               t1 = aj2ry**2
               t6 = aj2sy**2
               w2yy = t1*ww2rr+2*aj2ry*aj2sy*ww2rs+t6*ww2ss+aj2ryy*
     & ww2r+aj2syy*ww2s
             w2Lap = w2xx+ w2yy
            f(0) = (an1*w1x+an2*w1y)/eps1 -(an1*w2x+an2*w2y)/eps2
            f(1) = w1Lap/eps1 - w2Lap/eps2
            if( twilightZone.eq.1 )then
              call ogderiv(ep, 0,1,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, hz, wex  )
              call ogderiv(ep, 0,0,1,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, hz, wey  )
              call ogderiv(ep, 0,2,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, hz, wexx )
              call ogderiv(ep, 0,0,2,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, hz, weyy )
              weLap = wexx + weyy
              f(0) = f(0) - (an1*wex+an2*wey)*(1./eps1 - 1./eps2)
              f(1) = f(1) - ( weLap )*(1./eps1 - 1./eps2)
            end if

           a2(0,0)=-is*(an1*rsxy1(i1,i2,i3,axis1,0)+an2*rsxy1(i1,i2,i3,
     & axis1,1))/(2.*dr1(axis1)*eps1)
           a2(0,1)= js*(an1*rsxy2(j1,j2,j3,axis2,0)+an2*rsxy2(j1,j2,j3,
     & axis2,1))/(2.*dr2(axis2)*eps2)

           a2(1,0)= clap1/eps1
           a2(1,1)=-clap2/eps2

           q(0) = u1(i1-is1,i2-is2,i3,hz)
           q(1) = u2(j1-js1,j2-js2,j3,hz)

           ! subtract off the contributions from the wrong values at the ghost points:
           do n=0,1
             f(n) = (a2(n,0)*q(0)+a2(n,1)*q(1)) - f(n)
           end do

           call dgeco( a2(0,0), 2, 2, ipvt(0),rcond,work(0))
           job=0
           call dgesl( a2(0,0), 2, 2, ipvt(0), f(0), job)

           u1(i1-is1,i2-is2,i3,hz)=f(0)
           u2(j1-js1,j2-js2,j3,hz)=f(1)

           ! u1(i1-is1,i2-is2,i3,hz)=u2(j1+js1,j2+js2,j3,hz) 
           ! u2(j1-js1,j2-js2,j3,hz)=u1(i1+is1,i2+is2,i3,hz)

           if( debug.gt.3 )then ! re-evaluate

                ww1 = u1(i1,i2,i3,hz)
                ww1r = (-u1(i1-1,i2,i3,hz)+u1(i1+1,i2,i3,hz))/(2.*dr1(
     & 0))
                ww1s = (-u1(i1,i2-1,i3,hz)+u1(i1,i2+1,i3,hz))/(2.*dr1(
     & 1))
                ww1rr = (u1(i1-1,i2,i3,hz)-2.*u1(i1,i2,i3,hz)+u1(i1+1,
     & i2,i3,hz))/(dr1(0)**2)
                ww1rs = (-(-u1(i1-1,i2-1,i3,hz)+u1(i1-1,i2+1,i3,hz))/(
     & 2.*dr1(1))+(-u1(i1+1,i2-1,i3,hz)+u1(i1+1,i2+1,i3,hz))/(2.*dr1(
     & 1)))/(2.*dr1(0))
                ww1ss = (u1(i1,i2-1,i3,hz)-2.*u1(i1,i2,i3,hz)+u1(i1,i2+
     & 1,i3,hz))/(dr1(1)**2)
                 w1x = aj1rx*ww1r+aj1sx*ww1s
                 w1y = aj1ry*ww1r+aj1sy*ww1s
                 t1 = aj1rx**2
                 t6 = aj1sx**2
                 w1xx = t1*ww1rr+2*aj1rx*aj1sx*ww1rs+t6*ww1ss+aj1rxx*
     & ww1r+aj1sxx*ww1s
                 t1 = aj1ry**2
                 t6 = aj1sy**2
                 w1yy = t1*ww1rr+2*aj1ry*aj1sy*ww1rs+t6*ww1ss+aj1ryy*
     & ww1r+aj1syy*ww1s
               w1Lap = w1xx+ w1yy
                ww2 = u2(j1,j2,j3,hz)
                ww2r = (-u2(j1-1,j2,j3,hz)+u2(j1+1,j2,j3,hz))/(2.*dr2(
     & 0))
                ww2s = (-u2(j1,j2-1,j3,hz)+u2(j1,j2+1,j3,hz))/(2.*dr2(
     & 1))
                ww2rr = (u2(j1-1,j2,j3,hz)-2.*u2(j1,j2,j3,hz)+u2(j1+1,
     & j2,j3,hz))/(dr2(0)**2)
                ww2rs = (-(-u2(j1-1,j2-1,j3,hz)+u2(j1-1,j2+1,j3,hz))/(
     & 2.*dr2(1))+(-u2(j1+1,j2-1,j3,hz)+u2(j1+1,j2+1,j3,hz))/(2.*dr2(
     & 1)))/(2.*dr2(0))
                ww2ss = (u2(j1,j2-1,j3,hz)-2.*u2(j1,j2,j3,hz)+u2(j1,j2+
     & 1,j3,hz))/(dr2(1)**2)
                 w2x = aj2rx*ww2r+aj2sx*ww2s
                 w2y = aj2ry*ww2r+aj2sy*ww2s
                 t1 = aj2rx**2
                 t6 = aj2sx**2
                 w2xx = t1*ww2rr+2*aj2rx*aj2sx*ww2rs+t6*ww2ss+aj2rxx*
     & ww2r+aj2sxx*ww2s
                 t1 = aj2ry**2
                 t6 = aj2sy**2
                 w2yy = t1*ww2rr+2*aj2ry*aj2sy*ww2rs+t6*ww2ss+aj2ryy*
     & ww2r+aj2syy*ww2s
               w2Lap = w2xx+ w2yy
              f(0) = (an1*w1x+an2*w1y)/eps1 -(an1*w2x+an2*w2y)/eps2
              f(1) = w1Lap/eps1 - w2Lap/eps2
              if( twilightZone.eq.1 )then
                call ogderiv(ep, 0,1,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),0.,t, hz, wex  )
                call ogderiv(ep, 0,0,1,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),0.,t, hz, wey  )
                call ogderiv(ep, 0,2,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),0.,t, hz, wexx )
                call ogderiv(ep, 0,0,2,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),0.,t, hz, weyy )
                weLap = wexx + weyy
                f(0) = f(0) - (an1*wex+an2*wey)*(1./eps1 - 1./eps2)
                f(1) = f(1) - ( weLap )*(1./eps1 - 1./eps2)
              end if

             write(debugFile,'(" --> order2-curv: i1,i2=",2i4," hz-f(
     & re-eval)=",4e10.2)') i1,i2,f(0),f(1)
               ! '
           end if

            end if
            j1=j1+1
           end do
           j2=j2+1
          end do

         ! now make sure that div(u)=0 etc.
         if( .false. )then
c2         beginLoops2d() ! =============== start loops =======================
c2
c2           ! 0  [ u.x + v.y ] = 0
c2           ! first evaluate the equations we want to solve with the wrong values at the ghost points:
c2           divu=u1x22(i1,i2,i3,ex)+u1y22(i1,i2,i3,ey)
c2           a0=-is*rsxy1(i1,i2,i3,axis1,0)*dr112(axis1)
c2           a1=-is*rsxy1(i1,i2,i3,axis1,1)*dr112(axis1)
c2           aNormSq=a0**2+a1**2
c2           ! now project:  a.uNew = a.uOld - div  ->  (div-a.uOld)+a.uNew = div(uNew) = 0
c2           u1(i1-is1,i2-is2,i3,ex)=u1(i1-is1,i2-is2,i3,ex)-divu*a0/aNormSq
c2           u1(i1-is1,i2-is2,i3,ey)=u1(i1-is1,i2-is2,i3,ey)-divu*a1/aNormSq
c2
c2           divu=u2x22(j1,j2,j3,ex)+u2y22(j1,j2,j3,ey)
c2           a0=-js*rsxy2(j1,j2,j3,axis2,0)*dr212(axis2) 
c2           a1=-js*rsxy2(j1,j2,j3,axis2,1)*dr212(axis2) 
c2           aNormSq=a0**2+a1**2
c2
c2           u2(j1-js1,j2-js2,j3,ex)=u2(j1-js1,j2-js2,j3,ex)-divu*a0/aNormSq
c2           u2(j1-js1,j2-js2,j3,ey)=u2(j1-js1,j2-js2,j3,ey)-divu*a1/aNormSq
c2
c2           if( debug.gt.0 )then
c2             write(debugFile,'(" --> 2cth: eval div1,div2=",2e10.2)') u1x22(i1,i2,i3,ex)+u1y22(i1,i2,i3,ey),u2x22(j1,j2,j3,ex)+u2y22(j1,j2,j3,ey)
c2           end if
c2         endLoops2d()
         end if

         ! periodic update **** THIS WON T WORK IN PARALLEL
         if( parallel.eq.0 )then
          axisp1=mod(axis1+1,nd)
          if( boundaryCondition1(0,axisp1).lt.0 )then
           ! direction axisp1 is periodic
           diff(axis1)=0
           diff(axisp1)=gridIndexRange1(1,axisp1)-gridIndexRange1(0,
     & axisp1)
           if( side1.eq.0 )then
             ! assign 4 ghost points outside lower corner
             np1a=gridIndexRange1(0,0)-2
             np1b=gridIndexRange1(0,0)-1
             np2a=gridIndexRange1(0,1)-2
             np2b=gridIndexRange1(0,1)-1
             do i3=n3a,n3b
             do i2=np2a,np2b
             do i1=np1a,np1b
             do n=ex,hz
               ! write(*,'(" periodic i1,i2,i3,n=",4i4)') i1,i2,i3,n
              u1(i1,i2,i3,n) = u1(i1+diff(0),i2+diff(1),i3,n)
             end do
             end do
             end do
             end do
             ! assign 4 ghost points outside upper corner
             if( axis1.eq.0 )then
               np2a=gridIndexRange1(1,axisp1)+1
               np2b=gridIndexRange1(1,axisp1)+2
             else
               np1a=gridIndexRange1(1,axisp1)+1
               np1b=gridIndexRange1(1,axisp1)+2
             end if
             do i3=n3a,n3b
             do i2=np2a,np2b
             do i1=np1a,np1b
             do n=ex,hz
               ! write(*,'(" periodic i1,i2,i3,n=",4i4)') i1,i2,i3,n
              u1(i1,i2,i3,n) = u1(i1-diff(0),i2-diff(1),i3,n)
             end do
             end do
             end do
             end do
           else
             ! assign 4 ghost points outside upper corner
             np1a=gridIndexRange1(1,0)+1
             np1b=gridIndexRange1(1,0)+2
             np2a=gridIndexRange1(1,1)+1
             np2b=gridIndexRange1(1,1)+2
             do i3=n3a,n3b
             do i2=np2a,np2b
             do i1=np1a,np1b
             do n=ex,hz
               ! write(*,'(" periodic i1,i2,i3,n=",4i4)') i1,i2,i3,n
              u1(i1,i2,i3,n) = u1(i1-diff(0),i2-diff(1),i3,n)
             end do
             end do
             end do
             end do
             if( axis1.eq.0 )then
               np2a=gridIndexRange1(0,axisp1)-2
               np2b=gridIndexRange1(0,axisp1)-1
             else
               np1a=gridIndexRange1(0,axisp1)-2
               np1b=gridIndexRange1(0,axisp1)-1
             end if
             do i3=n3a,n3b
             do i2=np2a,np2b
             do i1=np1a,np1b
             do n=ex,hz
               ! write(*,'(" periodic i1,i2,i3,n=",4i4)') i1,i2,i3,n
              u1(i1,i2,i3,n) = u1(i1+diff(0),i2+diff(1),i3,n)
             end do
             end do
             end do
             end do
           end if
          endif
         end if
         if( parallel.eq.0 )then
          axisp1=mod(axis2+1,nd)
          if( boundaryCondition2(0,axisp1).lt.0 )then
           ! direction axisp1 is periodic
           diff(axis2)=0
           diff(axisp1)=gridIndexRange2(1,axisp1)-gridIndexRange2(0,
     & axisp1)
           if( side2.eq.0 )then
             ! assign 4 ghost points outside lower corner
             np1a=gridIndexRange2(0,0)-2
             np1b=gridIndexRange2(0,0)-1
             np2a=gridIndexRange2(0,1)-2
             np2b=gridIndexRange2(0,1)-1
             do i3=n3a,n3b
             do i2=np2a,np2b
             do i1=np1a,np1b
             do n=ex,hz
               ! write(*,'(" periodic i1,i2,i3,n=",4i4)') i1,i2,i3,n
              u2(i1,i2,i3,n) = u2(i1+diff(0),i2+diff(1),i3,n)
             end do
             end do
             end do
             end do
             ! assign 4 ghost points outside upper corner
             if( axis2.eq.0 )then
               np2a=gridIndexRange2(1,axisp1)+1
               np2b=gridIndexRange2(1,axisp1)+2
             else
               np1a=gridIndexRange2(1,axisp1)+1
               np1b=gridIndexRange2(1,axisp1)+2
             end if
             do i3=n3a,n3b
             do i2=np2a,np2b
             do i1=np1a,np1b
             do n=ex,hz
               ! write(*,'(" periodic i1,i2,i3,n=",4i4)') i1,i2,i3,n
              u2(i1,i2,i3,n) = u2(i1-diff(0),i2-diff(1),i3,n)
             end do
             end do
             end do
             end do
           else
             ! assign 4 ghost points outside upper corner
             np1a=gridIndexRange2(1,0)+1
             np1b=gridIndexRange2(1,0)+2
             np2a=gridIndexRange2(1,1)+1
             np2b=gridIndexRange2(1,1)+2
             do i3=n3a,n3b
             do i2=np2a,np2b
             do i1=np1a,np1b
             do n=ex,hz
               ! write(*,'(" periodic i1,i2,i3,n=",4i4)') i1,i2,i3,n
              u2(i1,i2,i3,n) = u2(i1-diff(0),i2-diff(1),i3,n)
             end do
             end do
             end do
             end do
             if( axis2.eq.0 )then
               np2a=gridIndexRange2(0,axisp1)-2
               np2b=gridIndexRange2(0,axisp1)-1
             else
               np1a=gridIndexRange2(0,axisp1)-2
               np1b=gridIndexRange2(0,axisp1)-1
             end if
             do i3=n3a,n3b
             do i2=np2a,np2b
             do i1=np1a,np1b
             do n=ex,hz
               ! write(*,'(" periodic i1,i2,i3,n=",4i4)') i1,i2,i3,n
              u2(i1,i2,i3,n) = u2(i1+diff(0),i2+diff(1),i3,n)
             end do
             end do
             end do
             end do
           end if
          endif
         end if

       else if( .false. .and. orderOfAccuracy.eq.4 )then

         ! for testing -- just assign from the other ghost points

          i3=n3a
          j3=m3a
          j2=m2a
          do i2=n2a,n2b
           j1=m1a
           do i1=n1a,n1b
           u1(i1-is1,i2-is2,i3,ex)=u2(j1+js1,j2+js2,j3,ex)
           u1(i1-is1,i2-is2,i3,ey)=u2(j1+js1,j2+js2,j3,ey)
           u1(i1-is1,i2-is2,i3,hz)=u2(j1+js1,j2+js2,j3,hz)

           u2(j1-js1,j2-js2,j3,ex)=u1(i1+is1,i2+is2,i3,ex)
           u2(j1-js1,j2-js2,j3,ey)=u1(i1+is1,i2+is2,i3,ey)
           u2(j1-js1,j2-js2,j3,hz)=u1(i1+is1,i2+is2,i3,hz)

           u1(i1-2*is1,i2-2*is2,i3,ex)=u2(j1+2*js1,j2+2*js2,j3,ex)
           u1(i1-2*is1,i2-2*is2,i3,ey)=u2(j1+2*js1,j2+2*js2,j3,ey)
           u1(i1-2*is1,i2-2*is2,i3,hz)=u2(j1+2*js1,j2+2*js2,j3,hz)

           u2(j1-2*js1,j2-2*js2,j3,ex)=u1(i1+2*is1,i2+2*is2,i3,ex)
           u2(j1-2*js1,j2-2*js2,j3,ey)=u1(i1+2*is1,i2+2*is2,i3,ey)
           u2(j1-2*js1,j2-2*js2,j3,hz)=u1(i1+2*is1,i2+2*is2,i3,hz)

            j1=j1+1
           end do
           j2=j2+1
          end do

       else if( nd.eq.2 .and. orderOfAccuracy.eq.4 .and. 
     & gridType.eq.rectangular )then

         ! --------------- 4th Order Rectangular ---------------

         if( useForcing.ne.0 )then
           ! finish me 
           stop 7716
         end if
         ! ***** fix these for [mu] != 0 ****
         if( mu1.ne.mu2 )then
           stop 9924
         end if


         ! ---- first satisfy the jump conditions on the boundary --------
         !    [ eps n.u ] = 0
         !    [ tau.u ] = 0
           if( eps1.lt.eps2 )then
             epsRatio=eps1/eps2
              i3=n3a
              j3=m3a
              j2=mm2a
              do i2=nn2a,nn2b
               j1=mm1a
               do i1=nn1a,nn1b
               if( mask1(i1,i2,i3).gt.0 .and. mask2(j1,j2,j3).gt.0 )
     & then
               ! eps2 n.u2 = eps1 n.u1
               !     tau.u2 = tau.u1
                an1=an1Cartesian
                an2=an2Cartesian
               ua=u1(i1,i2,i3,ex)
               ub=u1(i1,i2,i3,ey)
               nDotU = an1*ua+an2*ub
               if( twilightZone.eq.1 )then
                ! adjust for TZ forcing (here we assume the exact solution is the same on both sides)
                call ogderiv(ep, 0,0,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),0.,t, ex, ue )
                call ogderiv(ep, 0,0,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),0.,t, ey, ve )
                nDotU = nDotU - (an1*ue+an2*ve)
               end if
               ! u2 equals u1 but with normal component = eps1/eps2*(n.u1)
               u2(j1,j2,j3,ex) = ua + (nDotU*epsRatio - nDotU)*an1
               u2(j1,j2,j3,ey) = ub + (nDotU*epsRatio - nDotU)*an2
               u2(j1,j2,j3,hz) = u1(i1,i2,i3,hz)
                end if
                j1=j1+1
               end do
               j2=j2+1
              end do
           else
             epsRatio=eps2/eps1
              i3=n3a
              j3=m3a
              j2=mm2a
              do i2=nn2a,nn2b
               j1=mm1a
               do i1=nn1a,nn1b
               if( mask1(i1,i2,i3).gt.0 .and. mask2(j1,j2,j3).gt.0 )
     & then
               ! eps2 n.u2 = eps1 n.u1
               !     tau.u2 = tau.u1
                an1=an1Cartesian
                an2=an2Cartesian
               ua=u2(j1,j2,j3,ex)
               ub=u2(j1,j2,j3,ey)
               nDotU = an1*ua+an2*ub
               if( twilightZone.eq.1 )then
                ! adjust for TZ forcing (here we assume the exact solution is the same on both sides)
                call ogderiv(ep, 0,0,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),0.,t, ex, ue )
                call ogderiv(ep, 0,0,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),0.,t, ey, ve )
         ! write(*,'(" jump: x,y=",2e10.2," ua,ue=",2e10.2," ub,ve=",2e10.2)') xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),ua,ue,ub,ve
                nDotU = nDotU - (an1*ue+an2*ve)
               end if
               u1(i1,i2,i3,ex) = ua + (nDotU*epsRatio - nDotU)*an1
               u1(i1,i2,i3,ey) = ub + (nDotU*epsRatio - nDotU)*an2
               u1(i1,i2,i3,hz) = u2(j1,j2,j3,hz)
                end if
                j1=j1+1
               end do
               j2=j2+1
              end do
           end if

         ! here are the real jump conditions for the ghost points
         ! 0  [ u.x + v.y ] = 0
         ! 1  [ u.xx + u.yy ] = 0
         ! 2  [ v.x - u.y ] =0 
         ! 3  [ (v.xx+v.yy)/eps ] = 0
         ! 4  [ (u.xx+u.yy).x + (v.xx+v.yy).y ] = 0  OR [ (u.xx).x + (v.xx).y ] = 0 OR  [ (u.yy).x + (v.yy).y ] = 0 
         ! 5  [ {(Delta v).x - (Delta u).y}/eps ] =0  -> [ {(v.xxx+v.xyy)-(u.xxy+u.yyy)}/eps ] = 0
         ! 6  [ Delta^2 u/eps ] = 0
         ! 7  [ Delta^2 v/eps^2 ] = 0 


         ! initialization step: assign first ghost line by extrapolation
         ! NOTE: assign ghost points outside the ends
          i3=n3a
          j3=m3a
          j2=mm2a
          do i2=nn2a,nn2b
           j1=mm1a
           do i1=nn1a,nn1b
           u1(i1-is1,i2-is2,i3,ex)=(4.*u1(i1,i2,i3,ex)-6.*u1(i1+is1,i2+
     & is2,i3+is3,ex)+4.*u1(i1+2*is1,i2+2*is2,i3+2*is3,ex)-u1(i1+3*
     & is1,i2+3*is2,i3+3*is3,ex))
           u1(i1-is1,i2-is2,i3,ey)=(4.*u1(i1,i2,i3,ey)-6.*u1(i1+is1,i2+
     & is2,i3+is3,ey)+4.*u1(i1+2*is1,i2+2*is2,i3+2*is3,ey)-u1(i1+3*
     & is1,i2+3*is2,i3+3*is3,ey))
           u1(i1-is1,i2-is2,i3,hz)=(4.*u1(i1,i2,i3,hz)-6.*u1(i1+is1,i2+
     & is2,i3+is3,hz)+4.*u1(i1+2*is1,i2+2*is2,i3+2*is3,hz)-u1(i1+3*
     & is1,i2+3*is2,i3+3*is3,hz))

           u2(j1-js1,j2-js2,j3,ex)=(4.*u2(j1,j2,j3,ex)-6.*u2(j1+js1,j2+
     & js2,j3+js3,ex)+4.*u2(j1+2*js1,j2+2*js2,j3+2*js3,ex)-u2(j1+3*
     & js1,j2+3*js2,j3+3*js3,ex))
           u2(j1-js1,j2-js2,j3,ey)=(4.*u2(j1,j2,j3,ey)-6.*u2(j1+js1,j2+
     & js2,j3+js3,ey)+4.*u2(j1+2*js1,j2+2*js2,j3+2*js3,ey)-u2(j1+3*
     & js1,j2+3*js2,j3+3*js3,ey))
           u2(j1-js1,j2-js2,j3,hz)=(4.*u2(j1,j2,j3,hz)-6.*u2(j1+js1,j2+
     & js2,j3+js3,hz)+4.*u2(j1+2*js1,j2+2*js2,j3+2*js3,hz)-u2(j1+3*
     & js1,j2+3*js2,j3+3*js3,hz))

           ! --- also extrap 2nd line for now
           ! u1(i1-2*is1,i2-2*is2,i3,ex)=extrap4(u1,i1-is1,i2-is2,i3,ex,is1,is2,is3)
           ! u1(i1-2*is1,i2-2*is2,i3,ey)=extrap4(u1,i1-is1,i2-is2,i3,ey,is1,is2,is3)
           ! u1(i1-2*is1,i2-2*is2,i3,hz)=extrap4(u1,i1-is1,i2-is2,i3,hz,is1,is2,is3)

           ! u2(j1-2*js1,j2-2*js2,j3,ex)=extrap4(u2,j1-js1,j2-js2,j3,ex,js1,js2,js3)
           ! u2(j1-2*js1,j2-2*js2,j3,ey)=extrap4(u2,j1-js1,j2-js2,j3,ey,js1,js2,js3)
           ! u2(j1-2*js1,j2-2*js2,j3,hz)=extrap4(u2,j1-js1,j2-js2,j3,hz,js1,js2,js3)
            j1=j1+1
           end do
           j2=j2+1
          end do

          i3=n3a
          j3=m3a
          j2=m2a
          do i2=n2a,n2b
           j1=m1a
           do i1=n1a,n1b
            if( mask1(i1,i2,i3).gt.0 .and. mask2(j1,j2,j3).gt.0 )then

           ! first evaluate the equations we want to solve with the wrong values at the ghost points:


           ! evalDerivs2dOrder4()
            ! These derivatives are computed to 2nd-order accuracy
            ! NOTE: the jacobian derivatives can be computed once for all components
              uu1=u1(i1,i2,i3,ex) ! in the rectangular case just eval the solution
               u1xxx = (-u1(i1-2,i2,i3,ex)+2.*u1(i1-1,i2,i3,ex)-2.*u1(
     & i1+1,i2,i3,ex)+u1(i1+2,i2,i3,ex))/(2.*dx1(0)**3)
               u1xxy = ((-u1(i1-1,i2-1,i3,ex)+u1(i1-1,i2+1,i3,ex))/(2.*
     & dx1(1))-2.*(-u1(i1,i2-1,i3,ex)+u1(i1,i2+1,i3,ex))/(2.*dx1(1))+(
     & -u1(i1+1,i2-1,i3,ex)+u1(i1+1,i2+1,i3,ex))/(2.*dx1(1)))/(dx1(0)*
     & *2)
               u1xyy = (-(u1(i1-1,i2-1,i3,ex)-2.*u1(i1-1,i2,i3,ex)+u1(
     & i1-1,i2+1,i3,ex))/(dx1(1)**2)+(u1(i1+1,i2-1,i3,ex)-2.*u1(i1+1,
     & i2,i3,ex)+u1(i1+1,i2+1,i3,ex))/(dx1(1)**2))/(2.*dx1(0))
               u1yyy = (-u1(i1,i2-2,i3,ex)+2.*u1(i1,i2-1,i3,ex)-2.*u1(
     & i1,i2+1,i3,ex)+u1(i1,i2+2,i3,ex))/(2.*dx1(1)**3)
               u1xxxx = (u1(i1-2,i2,i3,ex)-4.*u1(i1-1,i2,i3,ex)+6.*u1(
     & i1,i2,i3,ex)-4.*u1(i1+1,i2,i3,ex)+u1(i1+2,i2,i3,ex))/(dx1(0)**
     & 4)
               u1xxyy = ((u1(i1-1,i2-1,i3,ex)-2.*u1(i1-1,i2,i3,ex)+u1(
     & i1-1,i2+1,i3,ex))/(dx1(1)**2)-2.*(u1(i1,i2-1,i3,ex)-2.*u1(i1,
     & i2,i3,ex)+u1(i1,i2+1,i3,ex))/(dx1(1)**2)+(u1(i1+1,i2-1,i3,ex)-
     & 2.*u1(i1+1,i2,i3,ex)+u1(i1+1,i2+1,i3,ex))/(dx1(1)**2))/(dx1(0)*
     & *2)
               u1yyyy = (u1(i1,i2-2,i3,ex)-4.*u1(i1,i2-1,i3,ex)+6.*u1(
     & i1,i2,i3,ex)-4.*u1(i1,i2+1,i3,ex)+u1(i1,i2+2,i3,ex))/(dx1(1)**
     & 4)
             u1LapSq = u1xxxx +2.* u1xxyy + u1yyyy
              vv1=u1(i1,i2,i3,ey) ! in the rectangular case just eval the solution
               v1xxx = (-u1(i1-2,i2,i3,ey)+2.*u1(i1-1,i2,i3,ey)-2.*u1(
     & i1+1,i2,i3,ey)+u1(i1+2,i2,i3,ey))/(2.*dx1(0)**3)
               v1xxy = ((-u1(i1-1,i2-1,i3,ey)+u1(i1-1,i2+1,i3,ey))/(2.*
     & dx1(1))-2.*(-u1(i1,i2-1,i3,ey)+u1(i1,i2+1,i3,ey))/(2.*dx1(1))+(
     & -u1(i1+1,i2-1,i3,ey)+u1(i1+1,i2+1,i3,ey))/(2.*dx1(1)))/(dx1(0)*
     & *2)
               v1xyy = (-(u1(i1-1,i2-1,i3,ey)-2.*u1(i1-1,i2,i3,ey)+u1(
     & i1-1,i2+1,i3,ey))/(dx1(1)**2)+(u1(i1+1,i2-1,i3,ey)-2.*u1(i1+1,
     & i2,i3,ey)+u1(i1+1,i2+1,i3,ey))/(dx1(1)**2))/(2.*dx1(0))
               v1yyy = (-u1(i1,i2-2,i3,ey)+2.*u1(i1,i2-1,i3,ey)-2.*u1(
     & i1,i2+1,i3,ey)+u1(i1,i2+2,i3,ey))/(2.*dx1(1)**3)
               v1xxxx = (u1(i1-2,i2,i3,ey)-4.*u1(i1-1,i2,i3,ey)+6.*u1(
     & i1,i2,i3,ey)-4.*u1(i1+1,i2,i3,ey)+u1(i1+2,i2,i3,ey))/(dx1(0)**
     & 4)
               v1xxyy = ((u1(i1-1,i2-1,i3,ey)-2.*u1(i1-1,i2,i3,ey)+u1(
     & i1-1,i2+1,i3,ey))/(dx1(1)**2)-2.*(u1(i1,i2-1,i3,ey)-2.*u1(i1,
     & i2,i3,ey)+u1(i1,i2+1,i3,ey))/(dx1(1)**2)+(u1(i1+1,i2-1,i3,ey)-
     & 2.*u1(i1+1,i2,i3,ey)+u1(i1+1,i2+1,i3,ey))/(dx1(1)**2))/(dx1(0)*
     & *2)
               v1yyyy = (u1(i1,i2-2,i3,ey)-4.*u1(i1,i2-1,i3,ey)+6.*u1(
     & i1,i2,i3,ey)-4.*u1(i1,i2+1,i3,ey)+u1(i1,i2+2,i3,ey))/(dx1(1)**
     & 4)
             v1LapSq = v1xxxx +2.* v1xxyy + v1yyyy
            ! NOTE: the jacobian derivatives can be computed once for all components
              uu2=u2(j1,j2,j3,ex) ! in the rectangular case just eval the solution
               u2xxx = (-u2(j1-2,j2,j3,ex)+2.*u2(j1-1,j2,j3,ex)-2.*u2(
     & j1+1,j2,j3,ex)+u2(j1+2,j2,j3,ex))/(2.*dx2(0)**3)
               u2xxy = ((-u2(j1-1,j2-1,j3,ex)+u2(j1-1,j2+1,j3,ex))/(2.*
     & dx2(1))-2.*(-u2(j1,j2-1,j3,ex)+u2(j1,j2+1,j3,ex))/(2.*dx2(1))+(
     & -u2(j1+1,j2-1,j3,ex)+u2(j1+1,j2+1,j3,ex))/(2.*dx2(1)))/(dx2(0)*
     & *2)
               u2xyy = (-(u2(j1-1,j2-1,j3,ex)-2.*u2(j1-1,j2,j3,ex)+u2(
     & j1-1,j2+1,j3,ex))/(dx2(1)**2)+(u2(j1+1,j2-1,j3,ex)-2.*u2(j1+1,
     & j2,j3,ex)+u2(j1+1,j2+1,j3,ex))/(dx2(1)**2))/(2.*dx2(0))
               u2yyy = (-u2(j1,j2-2,j3,ex)+2.*u2(j1,j2-1,j3,ex)-2.*u2(
     & j1,j2+1,j3,ex)+u2(j1,j2+2,j3,ex))/(2.*dx2(1)**3)
               u2xxxx = (u2(j1-2,j2,j3,ex)-4.*u2(j1-1,j2,j3,ex)+6.*u2(
     & j1,j2,j3,ex)-4.*u2(j1+1,j2,j3,ex)+u2(j1+2,j2,j3,ex))/(dx2(0)**
     & 4)
               u2xxyy = ((u2(j1-1,j2-1,j3,ex)-2.*u2(j1-1,j2,j3,ex)+u2(
     & j1-1,j2+1,j3,ex))/(dx2(1)**2)-2.*(u2(j1,j2-1,j3,ex)-2.*u2(j1,
     & j2,j3,ex)+u2(j1,j2+1,j3,ex))/(dx2(1)**2)+(u2(j1+1,j2-1,j3,ex)-
     & 2.*u2(j1+1,j2,j3,ex)+u2(j1+1,j2+1,j3,ex))/(dx2(1)**2))/(dx2(0)*
     & *2)
               u2yyyy = (u2(j1,j2-2,j3,ex)-4.*u2(j1,j2-1,j3,ex)+6.*u2(
     & j1,j2,j3,ex)-4.*u2(j1,j2+1,j3,ex)+u2(j1,j2+2,j3,ex))/(dx2(1)**
     & 4)
             u2LapSq = u2xxxx +2.* u2xxyy + u2yyyy
              vv2=u2(j1,j2,j3,ey) ! in the rectangular case just eval the solution
               v2xxx = (-u2(j1-2,j2,j3,ey)+2.*u2(j1-1,j2,j3,ey)-2.*u2(
     & j1+1,j2,j3,ey)+u2(j1+2,j2,j3,ey))/(2.*dx2(0)**3)
               v2xxy = ((-u2(j1-1,j2-1,j3,ey)+u2(j1-1,j2+1,j3,ey))/(2.*
     & dx2(1))-2.*(-u2(j1,j2-1,j3,ey)+u2(j1,j2+1,j3,ey))/(2.*dx2(1))+(
     & -u2(j1+1,j2-1,j3,ey)+u2(j1+1,j2+1,j3,ey))/(2.*dx2(1)))/(dx2(0)*
     & *2)
               v2xyy = (-(u2(j1-1,j2-1,j3,ey)-2.*u2(j1-1,j2,j3,ey)+u2(
     & j1-1,j2+1,j3,ey))/(dx2(1)**2)+(u2(j1+1,j2-1,j3,ey)-2.*u2(j1+1,
     & j2,j3,ey)+u2(j1+1,j2+1,j3,ey))/(dx2(1)**2))/(2.*dx2(0))
               v2yyy = (-u2(j1,j2-2,j3,ey)+2.*u2(j1,j2-1,j3,ey)-2.*u2(
     & j1,j2+1,j3,ey)+u2(j1,j2+2,j3,ey))/(2.*dx2(1)**3)
               v2xxxx = (u2(j1-2,j2,j3,ey)-4.*u2(j1-1,j2,j3,ey)+6.*u2(
     & j1,j2,j3,ey)-4.*u2(j1+1,j2,j3,ey)+u2(j1+2,j2,j3,ey))/(dx2(0)**
     & 4)
               v2xxyy = ((u2(j1-1,j2-1,j3,ey)-2.*u2(j1-1,j2,j3,ey)+u2(
     & j1-1,j2+1,j3,ey))/(dx2(1)**2)-2.*(u2(j1,j2-1,j3,ey)-2.*u2(j1,
     & j2,j3,ey)+u2(j1,j2+1,j3,ey))/(dx2(1)**2)+(u2(j1+1,j2-1,j3,ey)-
     & 2.*u2(j1+1,j2,j3,ey)+u2(j1+1,j2+1,j3,ey))/(dx2(1)**2))/(dx2(0)*
     & *2)
               v2yyyy = (u2(j1,j2-2,j3,ey)-4.*u2(j1,j2-1,j3,ey)+6.*u2(
     & j1,j2,j3,ey)-4.*u2(j1,j2+1,j3,ey)+u2(j1,j2+2,j3,ey))/(dx2(1)**
     & 4)
             v2LapSq = v2xxxx +2.* v2xxyy + v2yyyy
            ! These derivatives are computed to 4th-order accuracy
            ! NOTE: the jacobian derivatives can be computed once for all components
              uu1=u1(i1,i2,i3,ex) ! in the rectangular case just eval the solution
               u1x = (u1(i1-2,i2,i3,ex)-8.*u1(i1-1,i2,i3,ex)+8.*u1(i1+
     & 1,i2,i3,ex)-u1(i1+2,i2,i3,ex))/(12.*dx1(0))
               u1y = (u1(i1,i2-2,i3,ex)-8.*u1(i1,i2-1,i3,ex)+8.*u1(i1,
     & i2+1,i3,ex)-u1(i1,i2+2,i3,ex))/(12.*dx1(1))
               u1xx = (-u1(i1-2,i2,i3,ex)+16.*u1(i1-1,i2,i3,ex)-30.*u1(
     & i1,i2,i3,ex)+16.*u1(i1+1,i2,i3,ex)-u1(i1+2,i2,i3,ex))/(12.*dx1(
     & 0)**2)
               u1yy = (-u1(i1,i2-2,i3,ex)+16.*u1(i1,i2-1,i3,ex)-30.*u1(
     & i1,i2,i3,ex)+16.*u1(i1,i2+1,i3,ex)-u1(i1,i2+2,i3,ex))/(12.*dx1(
     & 1)**2)
             u1Lap = u1xx+ u1yy
              vv1=u1(i1,i2,i3,ey) ! in the rectangular case just eval the solution
               v1x = (u1(i1-2,i2,i3,ey)-8.*u1(i1-1,i2,i3,ey)+8.*u1(i1+
     & 1,i2,i3,ey)-u1(i1+2,i2,i3,ey))/(12.*dx1(0))
               v1y = (u1(i1,i2-2,i3,ey)-8.*u1(i1,i2-1,i3,ey)+8.*u1(i1,
     & i2+1,i3,ey)-u1(i1,i2+2,i3,ey))/(12.*dx1(1))
               v1xx = (-u1(i1-2,i2,i3,ey)+16.*u1(i1-1,i2,i3,ey)-30.*u1(
     & i1,i2,i3,ey)+16.*u1(i1+1,i2,i3,ey)-u1(i1+2,i2,i3,ey))/(12.*dx1(
     & 0)**2)
               v1yy = (-u1(i1,i2-2,i3,ey)+16.*u1(i1,i2-1,i3,ey)-30.*u1(
     & i1,i2,i3,ey)+16.*u1(i1,i2+1,i3,ey)-u1(i1,i2+2,i3,ey))/(12.*dx1(
     & 1)**2)
             v1Lap = v1xx+ v1yy
            ! NOTE: the jacobian derivatives can be computed once for all components
              uu2=u2(j1,j2,j3,ex) ! in the rectangular case just eval the solution
               u2x = (u2(j1-2,j2,j3,ex)-8.*u2(j1-1,j2,j3,ex)+8.*u2(j1+
     & 1,j2,j3,ex)-u2(j1+2,j2,j3,ex))/(12.*dx2(0))
               u2y = (u2(j1,j2-2,j3,ex)-8.*u2(j1,j2-1,j3,ex)+8.*u2(j1,
     & j2+1,j3,ex)-u2(j1,j2+2,j3,ex))/(12.*dx2(1))
               u2xx = (-u2(j1-2,j2,j3,ex)+16.*u2(j1-1,j2,j3,ex)-30.*u2(
     & j1,j2,j3,ex)+16.*u2(j1+1,j2,j3,ex)-u2(j1+2,j2,j3,ex))/(12.*dx2(
     & 0)**2)
               u2yy = (-u2(j1,j2-2,j3,ex)+16.*u2(j1,j2-1,j3,ex)-30.*u2(
     & j1,j2,j3,ex)+16.*u2(j1,j2+1,j3,ex)-u2(j1,j2+2,j3,ex))/(12.*dx2(
     & 1)**2)
             u2Lap = u2xx+ u2yy
              vv2=u2(j1,j2,j3,ey) ! in the rectangular case just eval the solution
               v2x = (u2(j1-2,j2,j3,ey)-8.*u2(j1-1,j2,j3,ey)+8.*u2(j1+
     & 1,j2,j3,ey)-u2(j1+2,j2,j3,ey))/(12.*dx2(0))
               v2y = (u2(j1,j2-2,j3,ey)-8.*u2(j1,j2-1,j3,ey)+8.*u2(j1,
     & j2+1,j3,ey)-u2(j1,j2+2,j3,ey))/(12.*dx2(1))
               v2xx = (-u2(j1-2,j2,j3,ey)+16.*u2(j1-1,j2,j3,ey)-30.*u2(
     & j1,j2,j3,ey)+16.*u2(j1+1,j2,j3,ey)-u2(j1+2,j2,j3,ey))/(12.*dx2(
     & 0)**2)
               v2yy = (-u2(j1,j2-2,j3,ey)+16.*u2(j1,j2-1,j3,ey)-30.*u2(
     & j1,j2,j3,ey)+16.*u2(j1,j2+1,j3,ey)-u2(j1,j2+2,j3,ey))/(12.*dx2(
     & 1)**2)
             v2Lap = v2xx+ v2yy
           f(0)=(u1x+v1y) - (u2x+v2y)
           f(1)=(u1Lap) - (u2Lap)
           f(2)=(v1x-u1y) - (v2x-u2y)
           f(3)=(v1Lap)/eps1 - (v2Lap)/eps2
           ! These next we can do to 2nd order -- these need a value on the first ghost line --
           f(4)=(u1xxx+u1xyy+v1xxy+v1yyy) - (u2xxx+u2xyy+v2xxy+v2yyy)
           f(5)=((v1xxx+v1xyy)-(u1xxy+u1yyy))/eps1 - ((v2xxx+v2xyy)-(
     & u2xxy+u2yyy))/eps2
           f(6)=(u1LapSq)/eps1 - (u2LapSq)/eps2
           f(7)=(v1LapSq)/eps1**2 - (v2LapSq)/eps2**2

!      write(debugFile,'(" --> 4th: j1,j2=",2i4," u1xx,u1yy,u2xx,u2yy=",4e10.2)') j1,j2,u1xx42r(i1,i2,i3,ex),!          u1yy42r(i1,i2,i3,ex),u2xx42r(j1,j2,j3,ex),u2yy42r(j1,j2,j3,ex)
!      write(debugFile,'(" --> 4th: i1,i2=",2i4," f(start)=",8e10.2)') i1,i2,f(0),f(1),f(2),f(3),f(4),f(5),f(6),f(7)

           ! here is the matrix of coefficients for the unknowns u1(-1),v1(-1),u2(-1),v2(-1)
           ! Solve:
           !     
           !       A [ U ] = A [ U(old) ] - [ f ]
c      u1x43r(i1,i2,i3,kd)=(8.*(u1(i1+1,i2,i3,kd)-u1(i1-1,i2,i3,kd))-(
c     & u1(i1+2,i2,i3,kd)-u1(i1-2,i2,i3,kd)))*dx141(0)


           ! 0  [ u.x + v.y ] = 0
           a8(0,0) = -is*8.*rx1*dx141(axis1)     ! coeff of u1(-1) from [u.x+v.y]
           a8(0,1) = -is*8.*ry1*dx141(axis1)     ! coeff of v1(-1) from [u.x+v.y]
           a8(0,4) =  is*rx1*dx141(axis1)        ! u1(-2)
           a8(0,5) =  is*ry1*dx141(axis1)        ! v1(-2)

           a8(0,2) =  js*8.*rx2*dx241(axis2)     ! coeff of u2(-1) from [u.x+v.y]
           a8(0,3) =  js*8.*ry2*dx241(axis2)
           a8(0,6) = -js*   rx2*dx241(axis2)
           a8(0,7) = -js*   ry2*dx241(axis2)

           ! 1  [ u.xx + u.yy ] = 0
c      u1xx43r(i1,i2,i3,kd)=( -30.*u1(i1,i2,i3,kd)+16.*(u1(i1+1,i2,i3,
c     & kd)+u1(i1-1,i2,i3,kd))-(u1(i1+2,i2,i3,kd)+u1(i1-2,i2,i3,kd)) )*
c     & dx142(0)

           a8(1,0) = 16.*dx142(axis1)         ! coeff of u1(-1) from [u.xx + u.yy]
           a8(1,1) = 0.
           a8(1,4) =    -dx142(axis1)         ! coeff of u1(-2) from [u.xx + u.yy]
           a8(1,5) = 0.

           a8(1,2) =-16.*dx242(axis2)         ! coeff of u2(-1) from [u.xx + u.yy]
           a8(1,3) = 0.
           a8(1,6) =     dx242(axis2)         ! coeff of u2(-2) from [u.xx + u.yy]
           a8(1,7) = 0.


           ! 2  [ v.x - u.y ] =0 
           a8(2,0) =  is*8.*ry1*dx141(axis1)
           a8(2,1) = -is*8.*rx1*dx141(axis1)    ! coeff of v1(-1) from [v.x - u.y]
           a8(2,4) = -is*   ry1*dx141(axis1)
           a8(2,5) =  is*   rx1*dx141(axis1)

           a8(2,2) = -js*8.*ry2*dx241(axis2)
           a8(2,3) =  js*8.*rx2*dx241(axis2)
           a8(2,6) =  js*   ry2*dx241(axis2)
           a8(2,7) = -js*   rx2*dx241(axis2)

           ! 3  [ (v.xx+v.yy)/eps ] = 0
           a8(3,0) = 0.
           a8(3,1) = 16.*dx142(axis1)/eps1 ! coeff of v1(-1) from [(v.xx+v.yy)/eps]
           a8(3,4) = 0.
           a8(3,5) =    -dx142(axis1)/eps1 ! coeff of v1(-2) from [(v.xx+v.yy)/eps]

           a8(3,2) = 0.
           a8(3,3) =-16.*dx242(axis2)/eps2 ! coeff of v2(-1) from [(v.xx+v.yy)/eps]
           a8(3,6) = 0.
           a8(3,7) =     dx242(axis2)/eps2 ! coeff of v2(-2) from [(v.xx+v.yy)/eps]

           ! 4  [ (u.xx+u.yy).x + (v.xx+v.yy).y ] = 0
c     u1xxx22r(i1,i2,i3,kd)=(-2.*(u1(i1+1,i2,i3,kd)-u1(i1-1,i2,i3,kd))+
c    & (u1(i1+2,i2,i3,kd)-u1(i1-2,i2,i3,kd)) )*dx122(0)*dx112(0)
c    u1xxy22r(i1,i2,i3,kd)=( u1xx22r(i1,i2+1,i3,kd)-u1xx22r(i1,i2-1,
c     & i3,kd))/(2.*dx1(1))
c      u1yy23r(i1,i2,i3,kd)=(-2.*u1(i1,i2,i3,kd)+(u1(i1,i2+1,i3,kd)+u1(
c     & i1,i2-1,i3,kd)) )*dx122(1)
c     u1xyy22r(i1,i2,i3,kd)=( u1yy22r(i1+1,i2,i3,kd)-u1yy22r(i1-1,i2,
c     & i3,kd))/(2.*dx1(0))
          a8(4,0)= ( is*rx1*2.*dx122(axis1)*dx112(axis1)+is*rx1*2.*
     & dx122(1)/(2.*dx1(0)))
          a8(4,1)= ( is*ry1*2.*dx122(axis1)*dx112(axis1)+is*ry1*2.*
     & dx122(0)/(2.*dx1(1)))
          a8(4,4)= (-is*rx1   *dx122(axis1)*dx112(axis1) )
          a8(4,5)= (-is*ry1   *dx122(axis1)*dx112(axis1))

          a8(4,2)=-( js*rx2*2.*dx222(axis2)*dx212(axis2)+js*rx2*2.*
     & dx222(1)/(2.*dx2(0)))
          a8(4,3)=-( js*ry2*2.*dx222(axis2)*dx212(axis2)+js*ry2*2.*
     & dx222(0)/(2.*dx2(1)))
          a8(4,6)=-(-js*rx2   *dx222(axis2)*dx212(axis2))
          a8(4,7)=-(-js*ry2   *dx222(axis2)*dx212(axis2))

          ! 5  [ {(Delta v).x - (Delta u).y}/eps ] =0  -> [ {(v.xxx+v.xyy)-(u.xxy+u.yyy)}/eps ] = 0

          a8(5,0)=-( is*ry1*2.*dx122(axis1)*dx112(axis1)+is*ry1*2.*
     & dx122(0)/(2.*dx1(1)))/eps1
          a8(5,1)= ( is*rx1*2.*dx122(axis1)*dx112(axis1)+is*rx1*2.*
     & dx122(1)/(2.*dx1(0)))/eps1
          a8(5,4)=-(-is*ry1   *dx122(axis1)*dx112(axis1))/eps1
          a8(5,5)= (-is*rx1   *dx122(axis1)*dx112(axis1))/eps1

          a8(5,2)= ( js*ry2*2.*dx222(axis2)*dx212(axis2)+js*ry2*2.*
     & dx222(0)/(2.*dx2(1)))/eps2
          a8(5,3)=-( js*rx2*2.*dx222(axis2)*dx212(axis2)+js*rx2*2.*
     & dx222(1)/(2.*dx2(0)))/eps2
          a8(5,6)= (-js*ry2   *dx222(axis2)*dx212(axis2))/eps2
          a8(5,7)=-(-js*rx2   *dx222(axis2)*dx212(axis2))/eps2

           ! 6  [ Delta^2 u/eps ] = 0
c     u1LapSq22r(i1,i2,i3,kd)= ( 6.*u1(i1,i2,i3,kd)- 4.*(u1(i1+1,i2,i3,
c    & kd)+u1(i1-1,i2,i3,kd))+(u1(i1+2,i2,i3,kd)+u1(i1-2,i2,i3,kd)) )
c    & /(dx1(0)**4)+( 6.*u1(i1,i2,i3,kd)-4.*(u1(i1,i2+1,i3,kd)+u1(i1,
c    & i2-1,i3,kd)) +(u1(i1,i2+2,i3,kd)+u1(i1,i2-2,i3,kd)) )/(dx1(1)**
c    & 4)+( 8.*u1(i1,i2,i3,kd)-4.*(u1(i1+1,i2,i3,kd)+u1(i1-1,i2,i3,kd)
c    & +u1(i1,i2+1,i3,kd)+u1(i1,i2-1,i3,kd))+2.*(u1(i1+1,i2+1,i3,kd)+
c    & u1(i1-1,i2+1,i3,kd)+u1(i1+1,i2-1,i3,kd)+u1(i1-1,i2-1,i3,kd)) )
c    & /(dx1(0)**2*dx1(1)**2)

           a8(6,0) = -(4./(dx1(axis1)**4) +4./(dx1(0)**2*dx1(1)**2) )
     & /eps1
           a8(6,1) = 0.
           a8(6,4) =   1./(dx1(axis1)**4)/eps1
           a8(6,5) = 0.

           a8(6,2) = (4./(dx2(axis2)**4) +4./(dx1(0)**2*dx1(1)**2) )
     & /eps2
           a8(6,3) = 0.
           a8(6,6) =  -1./(dx2(axis2)**4)/eps2
           a8(6,7) = 0.

           ! 7  [ Delta^2 v/eps^2 ] = 0 
           a8(7,0) = 0.
           a8(7,1) = -(4./(dx1(axis1)**4) +4./(dx2(0)**2*dx2(1)**2) )
     & /eps1**2
           a8(7,4) = 0.
           a8(7,5) =   1./(dx1(axis1)**4)/eps1**2

           a8(7,2) = 0.
           a8(7,3) =  (4./(dx2(axis2)**4) +4./(dx2(0)**2*dx2(1)**2) )
     & /eps2**2
           a8(7,6) = 0.
           a8(7,7) =  -1./(dx2(axis2)**4)/eps2**2

           q(0) = u1(i1-is1,i2-is2,i3,ex)
           q(1) = u1(i1-is1,i2-is2,i3,ey)
           q(2) = u2(j1-js1,j2-js2,j3,ex)
           q(3) = u2(j1-js1,j2-js2,j3,ey)

           q(4) = u1(i1-2*is1,i2-2*is2,i3,ex)
           q(5) = u1(i1-2*is1,i2-2*is2,i3,ey)
           q(6) = u2(j1-2*js1,j2-2*js2,j3,ex)
           q(7) = u2(j1-2*js1,j2-2*js2,j3,ey)

!      write(debugFile,'(" --> 4th: i1,i2=",2i4," q=",8e10.2)') i1,i2,q(0),q(1),q(2),q(3),q(4),q(5),q(6),q(7)

           ! subtract off the contributions from the initial (wrong) values at the ghost points:
           do n=0,7
             f(n) = (a8(n,0)*q(0)+a8(n,1)*q(1)+a8(n,2)*q(2)+a8(n,3)*q(
     & 3)+a8(n,4)*q(4)+a8(n,5)*q(5)+a8(n,6)*q(6)+a8(n,7)*q(7)) - f(n)
           end do

           ! solve A Q = F
           ! factor the matrix
           numberOfEquations=8
           call dgeco( a8(0,0), numberOfEquations, numberOfEquations, 
     & ipvt(0),rcond,work(0))
           ! solve
           !write(debugFile,'(" --> 4th: i1,i2=",2i4," rcond=",e10.2)') i1,i2,rcond
           job=0
           call dgesl( a8(0,0), numberOfEquations, numberOfEquations, 
     & ipvt(0), f(0), job)

           !write(debugFile,'(" --> 4th: i1,i2=",2i4," f(solve)=",8e10.2)') i1,i2,f(0),f(1),f(2),f(3),f(4),f(5),f(6),f(7)

           if( .true. )then
           u1(i1-is1,i2-is2,i3,ex)=f(0)
           u1(i1-is1,i2-is2,i3,ey)=f(1)
           u2(j1-js1,j2-js2,j3,ex)=f(2)
           u2(j1-js1,j2-js2,j3,ey)=f(3)

           u1(i1-2*is1,i2-2*is2,i3,ex)=f(4)
           u1(i1-2*is1,i2-2*is2,i3,ey)=f(5)
           u2(j1-2*js1,j2-2*js2,j3,ex)=f(6)
           u2(j1-2*js1,j2-2*js2,j3,ey)=f(7)
           end if

          if( debug.gt.3 )then ! re-evaluate
            ! These derivatives are computed to 2nd-order accuracy
            ! NOTE: the jacobian derivatives can be computed once for all components
              uu1=u1(i1,i2,i3,ex) ! in the rectangular case just eval the solution
               u1xxx = (-u1(i1-2,i2,i3,ex)+2.*u1(i1-1,i2,i3,ex)-2.*u1(
     & i1+1,i2,i3,ex)+u1(i1+2,i2,i3,ex))/(2.*dx1(0)**3)
               u1xxy = ((-u1(i1-1,i2-1,i3,ex)+u1(i1-1,i2+1,i3,ex))/(2.*
     & dx1(1))-2.*(-u1(i1,i2-1,i3,ex)+u1(i1,i2+1,i3,ex))/(2.*dx1(1))+(
     & -u1(i1+1,i2-1,i3,ex)+u1(i1+1,i2+1,i3,ex))/(2.*dx1(1)))/(dx1(0)*
     & *2)
               u1xyy = (-(u1(i1-1,i2-1,i3,ex)-2.*u1(i1-1,i2,i3,ex)+u1(
     & i1-1,i2+1,i3,ex))/(dx1(1)**2)+(u1(i1+1,i2-1,i3,ex)-2.*u1(i1+1,
     & i2,i3,ex)+u1(i1+1,i2+1,i3,ex))/(dx1(1)**2))/(2.*dx1(0))
               u1yyy = (-u1(i1,i2-2,i3,ex)+2.*u1(i1,i2-1,i3,ex)-2.*u1(
     & i1,i2+1,i3,ex)+u1(i1,i2+2,i3,ex))/(2.*dx1(1)**3)
               u1xxxx = (u1(i1-2,i2,i3,ex)-4.*u1(i1-1,i2,i3,ex)+6.*u1(
     & i1,i2,i3,ex)-4.*u1(i1+1,i2,i3,ex)+u1(i1+2,i2,i3,ex))/(dx1(0)**
     & 4)
               u1xxyy = ((u1(i1-1,i2-1,i3,ex)-2.*u1(i1-1,i2,i3,ex)+u1(
     & i1-1,i2+1,i3,ex))/(dx1(1)**2)-2.*(u1(i1,i2-1,i3,ex)-2.*u1(i1,
     & i2,i3,ex)+u1(i1,i2+1,i3,ex))/(dx1(1)**2)+(u1(i1+1,i2-1,i3,ex)-
     & 2.*u1(i1+1,i2,i3,ex)+u1(i1+1,i2+1,i3,ex))/(dx1(1)**2))/(dx1(0)*
     & *2)
               u1yyyy = (u1(i1,i2-2,i3,ex)-4.*u1(i1,i2-1,i3,ex)+6.*u1(
     & i1,i2,i3,ex)-4.*u1(i1,i2+1,i3,ex)+u1(i1,i2+2,i3,ex))/(dx1(1)**
     & 4)
             u1LapSq = u1xxxx +2.* u1xxyy + u1yyyy
              vv1=u1(i1,i2,i3,ey) ! in the rectangular case just eval the solution
               v1xxx = (-u1(i1-2,i2,i3,ey)+2.*u1(i1-1,i2,i3,ey)-2.*u1(
     & i1+1,i2,i3,ey)+u1(i1+2,i2,i3,ey))/(2.*dx1(0)**3)
               v1xxy = ((-u1(i1-1,i2-1,i3,ey)+u1(i1-1,i2+1,i3,ey))/(2.*
     & dx1(1))-2.*(-u1(i1,i2-1,i3,ey)+u1(i1,i2+1,i3,ey))/(2.*dx1(1))+(
     & -u1(i1+1,i2-1,i3,ey)+u1(i1+1,i2+1,i3,ey))/(2.*dx1(1)))/(dx1(0)*
     & *2)
               v1xyy = (-(u1(i1-1,i2-1,i3,ey)-2.*u1(i1-1,i2,i3,ey)+u1(
     & i1-1,i2+1,i3,ey))/(dx1(1)**2)+(u1(i1+1,i2-1,i3,ey)-2.*u1(i1+1,
     & i2,i3,ey)+u1(i1+1,i2+1,i3,ey))/(dx1(1)**2))/(2.*dx1(0))
               v1yyy = (-u1(i1,i2-2,i3,ey)+2.*u1(i1,i2-1,i3,ey)-2.*u1(
     & i1,i2+1,i3,ey)+u1(i1,i2+2,i3,ey))/(2.*dx1(1)**3)
               v1xxxx = (u1(i1-2,i2,i3,ey)-4.*u1(i1-1,i2,i3,ey)+6.*u1(
     & i1,i2,i3,ey)-4.*u1(i1+1,i2,i3,ey)+u1(i1+2,i2,i3,ey))/(dx1(0)**
     & 4)
               v1xxyy = ((u1(i1-1,i2-1,i3,ey)-2.*u1(i1-1,i2,i3,ey)+u1(
     & i1-1,i2+1,i3,ey))/(dx1(1)**2)-2.*(u1(i1,i2-1,i3,ey)-2.*u1(i1,
     & i2,i3,ey)+u1(i1,i2+1,i3,ey))/(dx1(1)**2)+(u1(i1+1,i2-1,i3,ey)-
     & 2.*u1(i1+1,i2,i3,ey)+u1(i1+1,i2+1,i3,ey))/(dx1(1)**2))/(dx1(0)*
     & *2)
               v1yyyy = (u1(i1,i2-2,i3,ey)-4.*u1(i1,i2-1,i3,ey)+6.*u1(
     & i1,i2,i3,ey)-4.*u1(i1,i2+1,i3,ey)+u1(i1,i2+2,i3,ey))/(dx1(1)**
     & 4)
             v1LapSq = v1xxxx +2.* v1xxyy + v1yyyy
            ! NOTE: the jacobian derivatives can be computed once for all components
              uu2=u2(j1,j2,j3,ex) ! in the rectangular case just eval the solution
               u2xxx = (-u2(j1-2,j2,j3,ex)+2.*u2(j1-1,j2,j3,ex)-2.*u2(
     & j1+1,j2,j3,ex)+u2(j1+2,j2,j3,ex))/(2.*dx2(0)**3)
               u2xxy = ((-u2(j1-1,j2-1,j3,ex)+u2(j1-1,j2+1,j3,ex))/(2.*
     & dx2(1))-2.*(-u2(j1,j2-1,j3,ex)+u2(j1,j2+1,j3,ex))/(2.*dx2(1))+(
     & -u2(j1+1,j2-1,j3,ex)+u2(j1+1,j2+1,j3,ex))/(2.*dx2(1)))/(dx2(0)*
     & *2)
               u2xyy = (-(u2(j1-1,j2-1,j3,ex)-2.*u2(j1-1,j2,j3,ex)+u2(
     & j1-1,j2+1,j3,ex))/(dx2(1)**2)+(u2(j1+1,j2-1,j3,ex)-2.*u2(j1+1,
     & j2,j3,ex)+u2(j1+1,j2+1,j3,ex))/(dx2(1)**2))/(2.*dx2(0))
               u2yyy = (-u2(j1,j2-2,j3,ex)+2.*u2(j1,j2-1,j3,ex)-2.*u2(
     & j1,j2+1,j3,ex)+u2(j1,j2+2,j3,ex))/(2.*dx2(1)**3)
               u2xxxx = (u2(j1-2,j2,j3,ex)-4.*u2(j1-1,j2,j3,ex)+6.*u2(
     & j1,j2,j3,ex)-4.*u2(j1+1,j2,j3,ex)+u2(j1+2,j2,j3,ex))/(dx2(0)**
     & 4)
               u2xxyy = ((u2(j1-1,j2-1,j3,ex)-2.*u2(j1-1,j2,j3,ex)+u2(
     & j1-1,j2+1,j3,ex))/(dx2(1)**2)-2.*(u2(j1,j2-1,j3,ex)-2.*u2(j1,
     & j2,j3,ex)+u2(j1,j2+1,j3,ex))/(dx2(1)**2)+(u2(j1+1,j2-1,j3,ex)-
     & 2.*u2(j1+1,j2,j3,ex)+u2(j1+1,j2+1,j3,ex))/(dx2(1)**2))/(dx2(0)*
     & *2)
               u2yyyy = (u2(j1,j2-2,j3,ex)-4.*u2(j1,j2-1,j3,ex)+6.*u2(
     & j1,j2,j3,ex)-4.*u2(j1,j2+1,j3,ex)+u2(j1,j2+2,j3,ex))/(dx2(1)**
     & 4)
             u2LapSq = u2xxxx +2.* u2xxyy + u2yyyy
              vv2=u2(j1,j2,j3,ey) ! in the rectangular case just eval the solution
               v2xxx = (-u2(j1-2,j2,j3,ey)+2.*u2(j1-1,j2,j3,ey)-2.*u2(
     & j1+1,j2,j3,ey)+u2(j1+2,j2,j3,ey))/(2.*dx2(0)**3)
               v2xxy = ((-u2(j1-1,j2-1,j3,ey)+u2(j1-1,j2+1,j3,ey))/(2.*
     & dx2(1))-2.*(-u2(j1,j2-1,j3,ey)+u2(j1,j2+1,j3,ey))/(2.*dx2(1))+(
     & -u2(j1+1,j2-1,j3,ey)+u2(j1+1,j2+1,j3,ey))/(2.*dx2(1)))/(dx2(0)*
     & *2)
               v2xyy = (-(u2(j1-1,j2-1,j3,ey)-2.*u2(j1-1,j2,j3,ey)+u2(
     & j1-1,j2+1,j3,ey))/(dx2(1)**2)+(u2(j1+1,j2-1,j3,ey)-2.*u2(j1+1,
     & j2,j3,ey)+u2(j1+1,j2+1,j3,ey))/(dx2(1)**2))/(2.*dx2(0))
               v2yyy = (-u2(j1,j2-2,j3,ey)+2.*u2(j1,j2-1,j3,ey)-2.*u2(
     & j1,j2+1,j3,ey)+u2(j1,j2+2,j3,ey))/(2.*dx2(1)**3)
               v2xxxx = (u2(j1-2,j2,j3,ey)-4.*u2(j1-1,j2,j3,ey)+6.*u2(
     & j1,j2,j3,ey)-4.*u2(j1+1,j2,j3,ey)+u2(j1+2,j2,j3,ey))/(dx2(0)**
     & 4)
               v2xxyy = ((u2(j1-1,j2-1,j3,ey)-2.*u2(j1-1,j2,j3,ey)+u2(
     & j1-1,j2+1,j3,ey))/(dx2(1)**2)-2.*(u2(j1,j2-1,j3,ey)-2.*u2(j1,
     & j2,j3,ey)+u2(j1,j2+1,j3,ey))/(dx2(1)**2)+(u2(j1+1,j2-1,j3,ey)-
     & 2.*u2(j1+1,j2,j3,ey)+u2(j1+1,j2+1,j3,ey))/(dx2(1)**2))/(dx2(0)*
     & *2)
               v2yyyy = (u2(j1,j2-2,j3,ey)-4.*u2(j1,j2-1,j3,ey)+6.*u2(
     & j1,j2,j3,ey)-4.*u2(j1,j2+1,j3,ey)+u2(j1,j2+2,j3,ey))/(dx2(1)**
     & 4)
             v2LapSq = v2xxxx +2.* v2xxyy + v2yyyy
            ! These derivatives are computed to 4th-order accuracy
            ! NOTE: the jacobian derivatives can be computed once for all components
              uu1=u1(i1,i2,i3,ex) ! in the rectangular case just eval the solution
               u1x = (u1(i1-2,i2,i3,ex)-8.*u1(i1-1,i2,i3,ex)+8.*u1(i1+
     & 1,i2,i3,ex)-u1(i1+2,i2,i3,ex))/(12.*dx1(0))
               u1y = (u1(i1,i2-2,i3,ex)-8.*u1(i1,i2-1,i3,ex)+8.*u1(i1,
     & i2+1,i3,ex)-u1(i1,i2+2,i3,ex))/(12.*dx1(1))
               u1xx = (-u1(i1-2,i2,i3,ex)+16.*u1(i1-1,i2,i3,ex)-30.*u1(
     & i1,i2,i3,ex)+16.*u1(i1+1,i2,i3,ex)-u1(i1+2,i2,i3,ex))/(12.*dx1(
     & 0)**2)
               u1yy = (-u1(i1,i2-2,i3,ex)+16.*u1(i1,i2-1,i3,ex)-30.*u1(
     & i1,i2,i3,ex)+16.*u1(i1,i2+1,i3,ex)-u1(i1,i2+2,i3,ex))/(12.*dx1(
     & 1)**2)
             u1Lap = u1xx+ u1yy
              vv1=u1(i1,i2,i3,ey) ! in the rectangular case just eval the solution
               v1x = (u1(i1-2,i2,i3,ey)-8.*u1(i1-1,i2,i3,ey)+8.*u1(i1+
     & 1,i2,i3,ey)-u1(i1+2,i2,i3,ey))/(12.*dx1(0))
               v1y = (u1(i1,i2-2,i3,ey)-8.*u1(i1,i2-1,i3,ey)+8.*u1(i1,
     & i2+1,i3,ey)-u1(i1,i2+2,i3,ey))/(12.*dx1(1))
               v1xx = (-u1(i1-2,i2,i3,ey)+16.*u1(i1-1,i2,i3,ey)-30.*u1(
     & i1,i2,i3,ey)+16.*u1(i1+1,i2,i3,ey)-u1(i1+2,i2,i3,ey))/(12.*dx1(
     & 0)**2)
               v1yy = (-u1(i1,i2-2,i3,ey)+16.*u1(i1,i2-1,i3,ey)-30.*u1(
     & i1,i2,i3,ey)+16.*u1(i1,i2+1,i3,ey)-u1(i1,i2+2,i3,ey))/(12.*dx1(
     & 1)**2)
             v1Lap = v1xx+ v1yy
            ! NOTE: the jacobian derivatives can be computed once for all components
              uu2=u2(j1,j2,j3,ex) ! in the rectangular case just eval the solution
               u2x = (u2(j1-2,j2,j3,ex)-8.*u2(j1-1,j2,j3,ex)+8.*u2(j1+
     & 1,j2,j3,ex)-u2(j1+2,j2,j3,ex))/(12.*dx2(0))
               u2y = (u2(j1,j2-2,j3,ex)-8.*u2(j1,j2-1,j3,ex)+8.*u2(j1,
     & j2+1,j3,ex)-u2(j1,j2+2,j3,ex))/(12.*dx2(1))
               u2xx = (-u2(j1-2,j2,j3,ex)+16.*u2(j1-1,j2,j3,ex)-30.*u2(
     & j1,j2,j3,ex)+16.*u2(j1+1,j2,j3,ex)-u2(j1+2,j2,j3,ex))/(12.*dx2(
     & 0)**2)
               u2yy = (-u2(j1,j2-2,j3,ex)+16.*u2(j1,j2-1,j3,ex)-30.*u2(
     & j1,j2,j3,ex)+16.*u2(j1,j2+1,j3,ex)-u2(j1,j2+2,j3,ex))/(12.*dx2(
     & 1)**2)
             u2Lap = u2xx+ u2yy
              vv2=u2(j1,j2,j3,ey) ! in the rectangular case just eval the solution
               v2x = (u2(j1-2,j2,j3,ey)-8.*u2(j1-1,j2,j3,ey)+8.*u2(j1+
     & 1,j2,j3,ey)-u2(j1+2,j2,j3,ey))/(12.*dx2(0))
               v2y = (u2(j1,j2-2,j3,ey)-8.*u2(j1,j2-1,j3,ey)+8.*u2(j1,
     & j2+1,j3,ey)-u2(j1,j2+2,j3,ey))/(12.*dx2(1))
               v2xx = (-u2(j1-2,j2,j3,ey)+16.*u2(j1-1,j2,j3,ey)-30.*u2(
     & j1,j2,j3,ey)+16.*u2(j1+1,j2,j3,ey)-u2(j1+2,j2,j3,ey))/(12.*dx2(
     & 0)**2)
               v2yy = (-u2(j1,j2-2,j3,ey)+16.*u2(j1,j2-1,j3,ey)-30.*u2(
     & j1,j2,j3,ey)+16.*u2(j1,j2+1,j3,ey)-u2(j1,j2+2,j3,ey))/(12.*dx2(
     & 1)**2)
             v2Lap = v2xx+ v2yy
           f(0)=(u1x+v1y) - (u2x+v2y)
           f(1)=(u1Lap) - (u2Lap)
           f(2)=(v1x-u1y) - (v2x-u2y)
           f(3)=(v1Lap)/eps1 - (v2Lap)/eps2
           ! These next we can do to 2nd order -- these need a value on the first ghost line --
           f(4)=(u1xxx+u1xyy+v1xxy+v1yyy) - (u2xxx+u2xyy+v2xxy+v2yyy)
           f(5)=((v1xxx+v1xyy)-(u1xxy+u1yyy))/eps1 - ((v2xxx+v2xyy)-(
     & u2xxy+u2yyy))/eps2
           f(6)=(u1LapSq)/eps1 - (u2LapSq)/eps2
           f(7)=(v1LapSq)/eps1**2 - (v2LapSq)/eps2**2

           write(debugFile,'(" --> 4th: i1,i2=",2i4," f(re-eval)=",
     & 8e10.2)') i1,i2,f(0),f(1),f(2),f(3),f(4),f(5),f(6),f(7)
           ! '
          end if

           ! do this for now
           u1(i1-is1,i2-is2,i3,hz)=u2(j1+js1,j2+js2,j3,hz)
           u2(j1-js1,j2-js2,j3,hz)=u1(i1+is1,i2+is2,i3,hz)

           u1(i1-2*is1,i2-2*is2,i3,hz)=u2(j1+2*js1,j2+2*js2,j3,hz)
           u2(j1-2*js1,j2-2*js2,j3,hz)=u1(i1+2*is1,i2+2*is2,i3,hz)

            end if
            j1=j1+1
           end do
           j2=j2+1
          end do

         ! periodic update
         if( parallel.eq.0 )then
          axisp1=mod(axis1+1,nd)
          if( boundaryCondition1(0,axisp1).lt.0 )then
           ! direction axisp1 is periodic
           diff(axis1)=0
           diff(axisp1)=gridIndexRange1(1,axisp1)-gridIndexRange1(0,
     & axisp1)
           if( side1.eq.0 )then
             ! assign 4 ghost points outside lower corner
             np1a=gridIndexRange1(0,0)-2
             np1b=gridIndexRange1(0,0)-1
             np2a=gridIndexRange1(0,1)-2
             np2b=gridIndexRange1(0,1)-1
             do i3=n3a,n3b
             do i2=np2a,np2b
             do i1=np1a,np1b
             do n=ex,hz
               ! write(*,'(" periodic i1,i2,i3,n=",4i4)') i1,i2,i3,n
              u1(i1,i2,i3,n) = u1(i1+diff(0),i2+diff(1),i3,n)
             end do
             end do
             end do
             end do
             ! assign 4 ghost points outside upper corner
             if( axis1.eq.0 )then
               np2a=gridIndexRange1(1,axisp1)+1
               np2b=gridIndexRange1(1,axisp1)+2
             else
               np1a=gridIndexRange1(1,axisp1)+1
               np1b=gridIndexRange1(1,axisp1)+2
             end if
             do i3=n3a,n3b
             do i2=np2a,np2b
             do i1=np1a,np1b
             do n=ex,hz
               ! write(*,'(" periodic i1,i2,i3,n=",4i4)') i1,i2,i3,n
              u1(i1,i2,i3,n) = u1(i1-diff(0),i2-diff(1),i3,n)
             end do
             end do
             end do
             end do
           else
             ! assign 4 ghost points outside upper corner
             np1a=gridIndexRange1(1,0)+1
             np1b=gridIndexRange1(1,0)+2
             np2a=gridIndexRange1(1,1)+1
             np2b=gridIndexRange1(1,1)+2
             do i3=n3a,n3b
             do i2=np2a,np2b
             do i1=np1a,np1b
             do n=ex,hz
               ! write(*,'(" periodic i1,i2,i3,n=",4i4)') i1,i2,i3,n
              u1(i1,i2,i3,n) = u1(i1-diff(0),i2-diff(1),i3,n)
             end do
             end do
             end do
             end do
             if( axis1.eq.0 )then
               np2a=gridIndexRange1(0,axisp1)-2
               np2b=gridIndexRange1(0,axisp1)-1
             else
               np1a=gridIndexRange1(0,axisp1)-2
               np1b=gridIndexRange1(0,axisp1)-1
             end if
             do i3=n3a,n3b
             do i2=np2a,np2b
             do i1=np1a,np1b
             do n=ex,hz
               ! write(*,'(" periodic i1,i2,i3,n=",4i4)') i1,i2,i3,n
              u1(i1,i2,i3,n) = u1(i1+diff(0),i2+diff(1),i3,n)
             end do
             end do
             end do
             end do
           end if
          endif
         end if
         if( parallel.eq.0 )then
          axisp1=mod(axis2+1,nd)
          if( boundaryCondition2(0,axisp1).lt.0 )then
           ! direction axisp1 is periodic
           diff(axis2)=0
           diff(axisp1)=gridIndexRange2(1,axisp1)-gridIndexRange2(0,
     & axisp1)
           if( side2.eq.0 )then
             ! assign 4 ghost points outside lower corner
             np1a=gridIndexRange2(0,0)-2
             np1b=gridIndexRange2(0,0)-1
             np2a=gridIndexRange2(0,1)-2
             np2b=gridIndexRange2(0,1)-1
             do i3=n3a,n3b
             do i2=np2a,np2b
             do i1=np1a,np1b
             do n=ex,hz
               ! write(*,'(" periodic i1,i2,i3,n=",4i4)') i1,i2,i3,n
              u2(i1,i2,i3,n) = u2(i1+diff(0),i2+diff(1),i3,n)
             end do
             end do
             end do
             end do
             ! assign 4 ghost points outside upper corner
             if( axis2.eq.0 )then
               np2a=gridIndexRange2(1,axisp1)+1
               np2b=gridIndexRange2(1,axisp1)+2
             else
               np1a=gridIndexRange2(1,axisp1)+1
               np1b=gridIndexRange2(1,axisp1)+2
             end if
             do i3=n3a,n3b
             do i2=np2a,np2b
             do i1=np1a,np1b
             do n=ex,hz
               ! write(*,'(" periodic i1,i2,i3,n=",4i4)') i1,i2,i3,n
              u2(i1,i2,i3,n) = u2(i1-diff(0),i2-diff(1),i3,n)
             end do
             end do
             end do
             end do
           else
             ! assign 4 ghost points outside upper corner
             np1a=gridIndexRange2(1,0)+1
             np1b=gridIndexRange2(1,0)+2
             np2a=gridIndexRange2(1,1)+1
             np2b=gridIndexRange2(1,1)+2
             do i3=n3a,n3b
             do i2=np2a,np2b
             do i1=np1a,np1b
             do n=ex,hz
               ! write(*,'(" periodic i1,i2,i3,n=",4i4)') i1,i2,i3,n
              u2(i1,i2,i3,n) = u2(i1-diff(0),i2-diff(1),i3,n)
             end do
             end do
             end do
             end do
             if( axis2.eq.0 )then
               np2a=gridIndexRange2(0,axisp1)-2
               np2b=gridIndexRange2(0,axisp1)-1
             else
               np1a=gridIndexRange2(0,axisp1)-2
               np1b=gridIndexRange2(0,axisp1)-1
             end if
             do i3=n3a,n3b
             do i2=np2a,np2b
             do i1=np1a,np1b
             do n=ex,hz
               ! write(*,'(" periodic i1,i2,i3,n=",4i4)') i1,i2,i3,n
              u2(i1,i2,i3,n) = u2(i1+diff(0),i2+diff(1),i3,n)
             end do
             end do
             end do
             end do
           end if
          endif
         end if

       else if( nd.eq.2 .and. orderOfAccuracy.eq.4 .and. 
     & gridType.eq.curvilinear )then

         ! --------------- 4th Order Curvilinear ---------------

         ! ***** fix these for [mu] != 0 ****
         if( mu1.ne.mu2 )then
           stop 9925
         end if

         ! ---- first satisfy the jump conditions on the boundary --------
         !    [ eps n.u ] = 0
         !    [ tau.u ] = 0
         !    [ w ] = 0 
           if( eps1.lt.eps2 )then
             epsRatio=eps1/eps2
              i3=n3a
              j3=m3a
              j2=mm2a
              do i2=nn2a,nn2b
               j1=mm1a
               do i1=nn1a,nn1b
               if( mask1(i1,i2,i3).gt.0 .and. mask2(j1,j2,j3).gt.0 )
     & then
               ! eps2 n.u2 = eps1 n.u1
               !     tau.u2 = tau.u1
                an1=rsxy1(i1,i2,i3,axis1,0)   ! normal (an1,an2)
                an2=rsxy1(i1,i2,i3,axis1,1)
                aNorm=max(epsx,sqrt(an1**2+an2**2))
                an1=an1/aNorm
                an2=an2/aNorm
               ua=u1(i1,i2,i3,ex)
               ub=u1(i1,i2,i3,ey)
               nDotU = an1*ua+an2*ub
               if( twilightZone.eq.1 )then
                ! adjust for TZ forcing (here we assume the exact solution is the same on both sides)
                call ogderiv(ep, 0,0,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),0.,t, ex, ue )
                call ogderiv(ep, 0,0,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),0.,t, ey, ve )
                nDotU = nDotU - (an1*ue+an2*ve)
               end if
               ! u2 equals u1 but with normal component = eps1/eps2*(n.u1)
               u2(j1,j2,j3,ex) = ua + (nDotU*epsRatio - nDotU)*an1
               u2(j1,j2,j3,ey) = ub + (nDotU*epsRatio - nDotU)*an2
               u2(j1,j2,j3,hz) = u1(i1,i2,i3,hz)
                end if
                j1=j1+1
               end do
               j2=j2+1
              end do
           else
             epsRatio=eps2/eps1
              i3=n3a
              j3=m3a
              j2=mm2a
              do i2=nn2a,nn2b
               j1=mm1a
               do i1=nn1a,nn1b
               if( mask1(i1,i2,i3).gt.0 .and. mask2(j1,j2,j3).gt.0 )
     & then
               ! eps2 n.u2 = eps1 n.u1
               !     tau.u2 = tau.u1
                an1=rsxy1(i1,i2,i3,axis1,0)
                an2=rsxy1(i1,i2,i3,axis1,1)
                aNorm=max(epsx,sqrt(an1**2+an2**2))
                an1=an1/aNorm
                an2=an2/aNorm
               ua=u2(j1,j2,j3,ex)
               ub=u2(j1,j2,j3,ey)
               nDotU = an1*ua+an2*ub
               if( twilightZone.eq.1 )then
                ! adjust for TZ forcing (here we assume the exact solution is the same on both sides)
                call ogderiv(ep, 0,0,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),0.,t, ex, ue )
                call ogderiv(ep, 0,0,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),0.,t, ey, ve )
         ! write(*,'(" jump: x,y=",2e10.2," ua,ue=",2e10.2," ub,ve=",2e10.2)') xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),ua,ue,ub,ve
                nDotU = nDotU - (an1*ue+an2*ve)
               end if
               u1(i1,i2,i3,ex) = ua + (nDotU*epsRatio - nDotU)*an1
               u1(i1,i2,i3,ey) = ub + (nDotU*epsRatio - nDotU)*an2
               u1(i1,i2,i3,hz) = u2(j1,j2,j3,hz)
                end if
                j1=j1+1
               end do
               j2=j2+1
              end do
           end if

         ! here are the real jump conditions for the ghost points
         ! 0  [ u.x + v.y ] = 0
         ! 1  [ n.(uv.xx + uv.yy) ] = 0
         ! 2  [ v.x - u.y ] =0 
         ! 3  [ tau.(v.xx+v.yy)/eps ] = 0
         ! 4  [ (u.xx+u.yy).x + (v.xx+v.yy).y ] = 0  OR [ (u.xx).x + (v.xx).y ] = 0 OR  [ (u.yy).x + (v.yy).y ] = 0 
         ! 5  [ {(Delta v).x - (Delta u).y}/eps ] =0  -> [ {(v.xxx+v.xyy)-(u.xxy+u.yyy)}/eps ] = 0
         ! 6  [ n.Delta^2 uv/eps ] = 0
         ! 7  [ tau.Delta^2 uv/eps^2 ] = 0 



         ! initialization step: assign first ghost line by extrapolation
         ! NOTE: assign ghost points outside the ends


          i3=n3a
          j3=m3a
          j2=mm2a
          do i2=nn2a,nn2b
           j1=mm1a
           do i1=nn1a,nn1b

c          u1(i1-is1,i2-is2,i3,ex)=extrap2(u1,i1,i2,i3,ex,is1,is2,is3)
c          u1(i1-is1,i2-is2,i3,ey)=extrap2(u1,i1,i2,i3,ey,is1,is2,is3)
c          u1(i1-is1,i2-is2,i3,hz)=extrap2(u1,i1,i2,i3,hz,is1,is2,is3)

c          u2(j1-js1,j2-js2,j3,ex)=extrap2(u2,j1,j2,j3,ex,js1,js2,js3)
c          u2(j1-js1,j2-js2,j3,ey)=extrap2(u2,j1,j2,j3,ey,js1,js2,js3)
c          u2(j1-js1,j2-js2,j3,hz)=extrap2(u2,j1,j2,j3,hz,js1,js2,js3)
c
            u1(i1-is1,i2-is2,i3,ex)=(4.*u1(i1,i2,i3,ex)-6.*u1(i1+is1,
     & i2+is2,i3+is3,ex)+4.*u1(i1+2*is1,i2+2*is2,i3+2*is3,ex)-u1(i1+3*
     & is1,i2+3*is2,i3+3*is3,ex))
            u1(i1-is1,i2-is2,i3,ey)=(4.*u1(i1,i2,i3,ey)-6.*u1(i1+is1,
     & i2+is2,i3+is3,ey)+4.*u1(i1+2*is1,i2+2*is2,i3+2*is3,ey)-u1(i1+3*
     & is1,i2+3*is2,i3+3*is3,ey))
            u1(i1-is1,i2-is2,i3,hz)=(4.*u1(i1,i2,i3,hz)-6.*u1(i1+is1,
     & i2+is2,i3+is3,hz)+4.*u1(i1+2*is1,i2+2*is2,i3+2*is3,hz)-u1(i1+3*
     & is1,i2+3*is2,i3+3*is3,hz))
c
            u2(j1-js1,j2-js2,j3,ex)=(4.*u2(j1,j2,j3,ex)-6.*u2(j1+js1,
     & j2+js2,j3+js3,ex)+4.*u2(j1+2*js1,j2+2*js2,j3+2*js3,ex)-u2(j1+3*
     & js1,j2+3*js2,j3+3*js3,ex))
            u2(j1-js1,j2-js2,j3,ey)=(4.*u2(j1,j2,j3,ey)-6.*u2(j1+js1,
     & j2+js2,j3+js3,ey)+4.*u2(j1+2*js1,j2+2*js2,j3+2*js3,ey)-u2(j1+3*
     & js1,j2+3*js2,j3+3*js3,ey))
            u2(j1-js1,j2-js2,j3,hz)=(4.*u2(j1,j2,j3,hz)-6.*u2(j1+js1,
     & j2+js2,j3+js3,hz)+4.*u2(j1+2*js1,j2+2*js2,j3+2*js3,hz)-u2(j1+3*
     & js1,j2+3*js2,j3+3*js3,hz))

           ! --- also extrap 2nd line for now
           u1(i1-2*is1,i2-2*is2,i3,ex)=(4.*u1(i1-is1,i2-is2,i3,ex)-6.*
     & u1(i1-is1+is1,i2-is2+is2,i3+is3,ex)+4.*u1(i1-is1+2*is1,i2-is2+
     & 2*is2,i3+2*is3,ex)-u1(i1-is1+3*is1,i2-is2+3*is2,i3+3*is3,ex))
           u1(i1-2*is1,i2-2*is2,i3,ey)=(4.*u1(i1-is1,i2-is2,i3,ey)-6.*
     & u1(i1-is1+is1,i2-is2+is2,i3+is3,ey)+4.*u1(i1-is1+2*is1,i2-is2+
     & 2*is2,i3+2*is3,ey)-u1(i1-is1+3*is1,i2-is2+3*is2,i3+3*is3,ey))
           u1(i1-2*is1,i2-2*is2,i3,hz)=(4.*u1(i1-is1,i2-is2,i3,hz)-6.*
     & u1(i1-is1+is1,i2-is2+is2,i3+is3,hz)+4.*u1(i1-is1+2*is1,i2-is2+
     & 2*is2,i3+2*is3,hz)-u1(i1-is1+3*is1,i2-is2+3*is2,i3+3*is3,hz))

           u2(j1-2*js1,j2-2*js2,j3,ex)=(4.*u2(j1-js1,j2-js2,j3,ex)-6.*
     & u2(j1-js1+js1,j2-js2+js2,j3+js3,ex)+4.*u2(j1-js1+2*js1,j2-js2+
     & 2*js2,j3+2*js3,ex)-u2(j1-js1+3*js1,j2-js2+3*js2,j3+3*js3,ex))
           u2(j1-2*js1,j2-2*js2,j3,ey)=(4.*u2(j1-js1,j2-js2,j3,ey)-6.*
     & u2(j1-js1+js1,j2-js2+js2,j3+js3,ey)+4.*u2(j1-js1+2*js1,j2-js2+
     & 2*js2,j3+2*js3,ey)-u2(j1-js1+3*js1,j2-js2+3*js2,j3+3*js3,ey))
           u2(j1-2*js1,j2-2*js2,j3,hz)=(4.*u2(j1-js1,j2-js2,j3,hz)-6.*
     & u2(j1-js1+js1,j2-js2+js2,j3+js3,hz)+4.*u2(j1-js1+2*js1,j2-js2+
     & 2*js2,j3+2*js3,hz)-u2(j1-js1+3*js1,j2-js2+3*js2,j3+3*js3,hz))
            j1=j1+1
           end do
           j2=j2+1
          end do

         ! write(debugFile,'(">>> interface: order=4 initialized=",i4)') initialized

         do it=1,nit ! *** begin iteration ****

           err=0.
         ! =============== start loops ======================
         nn=-1 ! counts points on the interface
          i3=n3a
          j3=m3a
          j2=m2a
          do i2=n2a,n2b
           j1=m1a
           do i1=n1a,n1b
            if( mask1(i1,i2,i3).gt.0 .and. mask2(j1,j2,j3).gt.0 )then

           nn=nn+1

           ! here is the normal (assumed to be the same on both sides)
           an1=rsxy1(i1,i2,i3,axis1,0)   ! normal (an1,an2)
           an2=rsxy1(i1,i2,i3,axis1,1)
           aNorm=max(epsx,sqrt(an1**2+an2**2))
           an1=an1/aNorm
           an2=an2/aNorm
           tau1=-an2
           tau2= an1

           ! evalDerivs2dOrder4()
            ! These derivatives are computed to 2nd-order accuracy
            ! NOTE: the jacobian derivatives can be computed once for all components
             ! this next call will define the jacobian and its derivatives (parameteric and spatial)
             aj1rx = rsxy1(i1,i2,i3,0,0)
             aj1rxr = (-rsxy1(i1-1,i2,i3,0,0)+rsxy1(i1+1,i2,i3,0,0))/(
     & 2.*dr1(0))
             aj1rxs = (-rsxy1(i1,i2-1,i3,0,0)+rsxy1(i1,i2+1,i3,0,0))/(
     & 2.*dr1(1))
             aj1rxrr = (rsxy1(i1-1,i2,i3,0,0)-2.*rsxy1(i1,i2,i3,0,0)+
     & rsxy1(i1+1,i2,i3,0,0))/(dr1(0)**2)
             aj1rxrs = (-(-rsxy1(i1-1,i2-1,i3,0,0)+rsxy1(i1-1,i2+1,i3,
     & 0,0))/(2.*dr1(1))+(-rsxy1(i1+1,i2-1,i3,0,0)+rsxy1(i1+1,i2+1,i3,
     & 0,0))/(2.*dr1(1)))/(2.*dr1(0))
             aj1rxss = (rsxy1(i1,i2-1,i3,0,0)-2.*rsxy1(i1,i2,i3,0,0)+
     & rsxy1(i1,i2+1,i3,0,0))/(dr1(1)**2)
             aj1rxrrr = (-rsxy1(i1-2,i2,i3,0,0)+2.*rsxy1(i1-1,i2,i3,0,
     & 0)-2.*rsxy1(i1+1,i2,i3,0,0)+rsxy1(i1+2,i2,i3,0,0))/(2.*dr1(0)**
     & 3)
             aj1rxrrs = ((-rsxy1(i1-1,i2-1,i3,0,0)+rsxy1(i1-1,i2+1,i3,
     & 0,0))/(2.*dr1(1))-2.*(-rsxy1(i1,i2-1,i3,0,0)+rsxy1(i1,i2+1,i3,
     & 0,0))/(2.*dr1(1))+(-rsxy1(i1+1,i2-1,i3,0,0)+rsxy1(i1+1,i2+1,i3,
     & 0,0))/(2.*dr1(1)))/(dr1(0)**2)
             aj1rxrss = (-(rsxy1(i1-1,i2-1,i3,0,0)-2.*rsxy1(i1-1,i2,i3,
     & 0,0)+rsxy1(i1-1,i2+1,i3,0,0))/(dr1(1)**2)+(rsxy1(i1+1,i2-1,i3,
     & 0,0)-2.*rsxy1(i1+1,i2,i3,0,0)+rsxy1(i1+1,i2+1,i3,0,0))/(dr1(1)*
     & *2))/(2.*dr1(0))
             aj1rxsss = (-rsxy1(i1,i2-2,i3,0,0)+2.*rsxy1(i1,i2-1,i3,0,
     & 0)-2.*rsxy1(i1,i2+1,i3,0,0)+rsxy1(i1,i2+2,i3,0,0))/(2.*dr1(1)**
     & 3)
             aj1sx = rsxy1(i1,i2,i3,1,0)
             aj1sxr = (-rsxy1(i1-1,i2,i3,1,0)+rsxy1(i1+1,i2,i3,1,0))/(
     & 2.*dr1(0))
             aj1sxs = (-rsxy1(i1,i2-1,i3,1,0)+rsxy1(i1,i2+1,i3,1,0))/(
     & 2.*dr1(1))
             aj1sxrr = (rsxy1(i1-1,i2,i3,1,0)-2.*rsxy1(i1,i2,i3,1,0)+
     & rsxy1(i1+1,i2,i3,1,0))/(dr1(0)**2)
             aj1sxrs = (-(-rsxy1(i1-1,i2-1,i3,1,0)+rsxy1(i1-1,i2+1,i3,
     & 1,0))/(2.*dr1(1))+(-rsxy1(i1+1,i2-1,i3,1,0)+rsxy1(i1+1,i2+1,i3,
     & 1,0))/(2.*dr1(1)))/(2.*dr1(0))
             aj1sxss = (rsxy1(i1,i2-1,i3,1,0)-2.*rsxy1(i1,i2,i3,1,0)+
     & rsxy1(i1,i2+1,i3,1,0))/(dr1(1)**2)
             aj1sxrrr = (-rsxy1(i1-2,i2,i3,1,0)+2.*rsxy1(i1-1,i2,i3,1,
     & 0)-2.*rsxy1(i1+1,i2,i3,1,0)+rsxy1(i1+2,i2,i3,1,0))/(2.*dr1(0)**
     & 3)
             aj1sxrrs = ((-rsxy1(i1-1,i2-1,i3,1,0)+rsxy1(i1-1,i2+1,i3,
     & 1,0))/(2.*dr1(1))-2.*(-rsxy1(i1,i2-1,i3,1,0)+rsxy1(i1,i2+1,i3,
     & 1,0))/(2.*dr1(1))+(-rsxy1(i1+1,i2-1,i3,1,0)+rsxy1(i1+1,i2+1,i3,
     & 1,0))/(2.*dr1(1)))/(dr1(0)**2)
             aj1sxrss = (-(rsxy1(i1-1,i2-1,i3,1,0)-2.*rsxy1(i1-1,i2,i3,
     & 1,0)+rsxy1(i1-1,i2+1,i3,1,0))/(dr1(1)**2)+(rsxy1(i1+1,i2-1,i3,
     & 1,0)-2.*rsxy1(i1+1,i2,i3,1,0)+rsxy1(i1+1,i2+1,i3,1,0))/(dr1(1)*
     & *2))/(2.*dr1(0))
             aj1sxsss = (-rsxy1(i1,i2-2,i3,1,0)+2.*rsxy1(i1,i2-1,i3,1,
     & 0)-2.*rsxy1(i1,i2+1,i3,1,0)+rsxy1(i1,i2+2,i3,1,0))/(2.*dr1(1)**
     & 3)
             aj1ry = rsxy1(i1,i2,i3,0,1)
             aj1ryr = (-rsxy1(i1-1,i2,i3,0,1)+rsxy1(i1+1,i2,i3,0,1))/(
     & 2.*dr1(0))
             aj1rys = (-rsxy1(i1,i2-1,i3,0,1)+rsxy1(i1,i2+1,i3,0,1))/(
     & 2.*dr1(1))
             aj1ryrr = (rsxy1(i1-1,i2,i3,0,1)-2.*rsxy1(i1,i2,i3,0,1)+
     & rsxy1(i1+1,i2,i3,0,1))/(dr1(0)**2)
             aj1ryrs = (-(-rsxy1(i1-1,i2-1,i3,0,1)+rsxy1(i1-1,i2+1,i3,
     & 0,1))/(2.*dr1(1))+(-rsxy1(i1+1,i2-1,i3,0,1)+rsxy1(i1+1,i2+1,i3,
     & 0,1))/(2.*dr1(1)))/(2.*dr1(0))
             aj1ryss = (rsxy1(i1,i2-1,i3,0,1)-2.*rsxy1(i1,i2,i3,0,1)+
     & rsxy1(i1,i2+1,i3,0,1))/(dr1(1)**2)
             aj1ryrrr = (-rsxy1(i1-2,i2,i3,0,1)+2.*rsxy1(i1-1,i2,i3,0,
     & 1)-2.*rsxy1(i1+1,i2,i3,0,1)+rsxy1(i1+2,i2,i3,0,1))/(2.*dr1(0)**
     & 3)
             aj1ryrrs = ((-rsxy1(i1-1,i2-1,i3,0,1)+rsxy1(i1-1,i2+1,i3,
     & 0,1))/(2.*dr1(1))-2.*(-rsxy1(i1,i2-1,i3,0,1)+rsxy1(i1,i2+1,i3,
     & 0,1))/(2.*dr1(1))+(-rsxy1(i1+1,i2-1,i3,0,1)+rsxy1(i1+1,i2+1,i3,
     & 0,1))/(2.*dr1(1)))/(dr1(0)**2)
             aj1ryrss = (-(rsxy1(i1-1,i2-1,i3,0,1)-2.*rsxy1(i1-1,i2,i3,
     & 0,1)+rsxy1(i1-1,i2+1,i3,0,1))/(dr1(1)**2)+(rsxy1(i1+1,i2-1,i3,
     & 0,1)-2.*rsxy1(i1+1,i2,i3,0,1)+rsxy1(i1+1,i2+1,i3,0,1))/(dr1(1)*
     & *2))/(2.*dr1(0))
             aj1rysss = (-rsxy1(i1,i2-2,i3,0,1)+2.*rsxy1(i1,i2-1,i3,0,
     & 1)-2.*rsxy1(i1,i2+1,i3,0,1)+rsxy1(i1,i2+2,i3,0,1))/(2.*dr1(1)**
     & 3)
             aj1sy = rsxy1(i1,i2,i3,1,1)
             aj1syr = (-rsxy1(i1-1,i2,i3,1,1)+rsxy1(i1+1,i2,i3,1,1))/(
     & 2.*dr1(0))
             aj1sys = (-rsxy1(i1,i2-1,i3,1,1)+rsxy1(i1,i2+1,i3,1,1))/(
     & 2.*dr1(1))
             aj1syrr = (rsxy1(i1-1,i2,i3,1,1)-2.*rsxy1(i1,i2,i3,1,1)+
     & rsxy1(i1+1,i2,i3,1,1))/(dr1(0)**2)
             aj1syrs = (-(-rsxy1(i1-1,i2-1,i3,1,1)+rsxy1(i1-1,i2+1,i3,
     & 1,1))/(2.*dr1(1))+(-rsxy1(i1+1,i2-1,i3,1,1)+rsxy1(i1+1,i2+1,i3,
     & 1,1))/(2.*dr1(1)))/(2.*dr1(0))
             aj1syss = (rsxy1(i1,i2-1,i3,1,1)-2.*rsxy1(i1,i2,i3,1,1)+
     & rsxy1(i1,i2+1,i3,1,1))/(dr1(1)**2)
             aj1syrrr = (-rsxy1(i1-2,i2,i3,1,1)+2.*rsxy1(i1-1,i2,i3,1,
     & 1)-2.*rsxy1(i1+1,i2,i3,1,1)+rsxy1(i1+2,i2,i3,1,1))/(2.*dr1(0)**
     & 3)
             aj1syrrs = ((-rsxy1(i1-1,i2-1,i3,1,1)+rsxy1(i1-1,i2+1,i3,
     & 1,1))/(2.*dr1(1))-2.*(-rsxy1(i1,i2-1,i3,1,1)+rsxy1(i1,i2+1,i3,
     & 1,1))/(2.*dr1(1))+(-rsxy1(i1+1,i2-1,i3,1,1)+rsxy1(i1+1,i2+1,i3,
     & 1,1))/(2.*dr1(1)))/(dr1(0)**2)
             aj1syrss = (-(rsxy1(i1-1,i2-1,i3,1,1)-2.*rsxy1(i1-1,i2,i3,
     & 1,1)+rsxy1(i1-1,i2+1,i3,1,1))/(dr1(1)**2)+(rsxy1(i1+1,i2-1,i3,
     & 1,1)-2.*rsxy1(i1+1,i2,i3,1,1)+rsxy1(i1+1,i2+1,i3,1,1))/(dr1(1)*
     & *2))/(2.*dr1(0))
             aj1sysss = (-rsxy1(i1,i2-2,i3,1,1)+2.*rsxy1(i1,i2-1,i3,1,
     & 1)-2.*rsxy1(i1,i2+1,i3,1,1)+rsxy1(i1,i2+2,i3,1,1))/(2.*dr1(1)**
     & 3)
             aj1rxx = aj1rx*aj1rxr+aj1sx*aj1rxs
             aj1rxy = aj1ry*aj1rxr+aj1sy*aj1rxs
             aj1sxx = aj1rx*aj1sxr+aj1sx*aj1sxs
             aj1sxy = aj1ry*aj1sxr+aj1sy*aj1sxs
             aj1ryx = aj1rx*aj1ryr+aj1sx*aj1rys
             aj1ryy = aj1ry*aj1ryr+aj1sy*aj1rys
             aj1syx = aj1rx*aj1syr+aj1sx*aj1sys
             aj1syy = aj1ry*aj1syr+aj1sy*aj1sys
             t1 = aj1rx**2
             t6 = aj1sx**2
             aj1rxxx = t1*aj1rxrr+2*aj1rx*aj1sx*aj1rxrs+t6*aj1rxss+
     & aj1rxx*aj1rxr+aj1sxx*aj1rxs
             aj1rxxy = aj1ry*aj1rx*aj1rxrr+(aj1sy*aj1rx+aj1ry*aj1sx)*
     & aj1rxrs+aj1sy*aj1sx*aj1rxss+aj1rxy*aj1rxr+aj1sxy*aj1rxs
             t1 = aj1ry**2
             t6 = aj1sy**2
             aj1rxyy = t1*aj1rxrr+2*aj1ry*aj1sy*aj1rxrs+t6*aj1rxss+
     & aj1ryy*aj1rxr+aj1syy*aj1rxs
             t1 = aj1rx**2
             t6 = aj1sx**2
             aj1sxxx = t1*aj1sxrr+2*aj1rx*aj1sx*aj1sxrs+t6*aj1sxss+
     & aj1rxx*aj1sxr+aj1sxx*aj1sxs
             aj1sxxy = aj1ry*aj1rx*aj1sxrr+(aj1sy*aj1rx+aj1ry*aj1sx)*
     & aj1sxrs+aj1sy*aj1sx*aj1sxss+aj1rxy*aj1sxr+aj1sxy*aj1sxs
             t1 = aj1ry**2
             t6 = aj1sy**2
             aj1sxyy = t1*aj1sxrr+2*aj1ry*aj1sy*aj1sxrs+t6*aj1sxss+
     & aj1ryy*aj1sxr+aj1syy*aj1sxs
             t1 = aj1rx**2
             t6 = aj1sx**2
             aj1ryxx = t1*aj1ryrr+2*aj1rx*aj1sx*aj1ryrs+t6*aj1ryss+
     & aj1rxx*aj1ryr+aj1sxx*aj1rys
             aj1ryxy = aj1ry*aj1rx*aj1ryrr+(aj1sy*aj1rx+aj1ry*aj1sx)*
     & aj1ryrs+aj1sy*aj1sx*aj1ryss+aj1rxy*aj1ryr+aj1sxy*aj1rys
             t1 = aj1ry**2
             t6 = aj1sy**2
             aj1ryyy = t1*aj1ryrr+2*aj1ry*aj1sy*aj1ryrs+t6*aj1ryss+
     & aj1ryy*aj1ryr+aj1syy*aj1rys
             t1 = aj1rx**2
             t6 = aj1sx**2
             aj1syxx = t1*aj1syrr+2*aj1rx*aj1sx*aj1syrs+t6*aj1syss+
     & aj1rxx*aj1syr+aj1sxx*aj1sys
             aj1syxy = aj1ry*aj1rx*aj1syrr+(aj1sy*aj1rx+aj1ry*aj1sx)*
     & aj1syrs+aj1sy*aj1sx*aj1syss+aj1rxy*aj1syr+aj1sxy*aj1sys
             t1 = aj1ry**2
             t6 = aj1sy**2
             aj1syyy = t1*aj1syrr+2*aj1ry*aj1sy*aj1syrs+t6*aj1syss+
     & aj1ryy*aj1syr+aj1syy*aj1sys
             t1 = aj1rx**2
             t7 = aj1sx**2
             aj1rxxxx = t1*aj1rx*aj1rxrrr+3*t1*aj1sx*aj1rxrrs+3*aj1rx*
     & t7*aj1rxrss+t7*aj1sx*aj1rxsss+3*aj1rx*aj1rxx*aj1rxrr+(3*aj1sxx*
     & aj1rx+3*aj1sx*aj1rxx)*aj1rxrs+3*aj1sxx*aj1sx*aj1rxss+aj1rxxx*
     & aj1rxr+aj1sxxx*aj1rxs
             t1 = aj1rx**2
             t10 = aj1sx**2
             aj1rxxxy = aj1ry*t1*aj1rxrrr+(aj1sy*t1+2*aj1ry*aj1sx*
     & aj1rx)*aj1rxrrs+(aj1ry*t10+2*aj1sy*aj1sx*aj1rx)*aj1rxrss+aj1sy*
     & t10*aj1rxsss+(2*aj1rxy*aj1rx+aj1ry*aj1rxx)*aj1rxrr+(aj1ry*
     & aj1sxx+2*aj1sx*aj1rxy+2*aj1sxy*aj1rx+aj1sy*aj1rxx)*aj1rxrs+(
     & aj1sy*aj1sxx+2*aj1sxy*aj1sx)*aj1rxss+aj1rxxy*aj1rxr+aj1sxxy*
     & aj1rxs
             t1 = aj1ry**2
             t4 = aj1sy*aj1ry
             t8 = aj1sy*aj1rx+aj1ry*aj1sx
             t16 = aj1sy**2
             aj1rxxyy = t1*aj1rx*aj1rxrrr+(t4*aj1rx+aj1ry*t8)*aj1rxrrs+
     & (t4*aj1sx+aj1sy*t8)*aj1rxrss+t16*aj1sx*aj1rxsss+(aj1ryy*aj1rx+
     & 2*aj1ry*aj1rxy)*aj1rxrr+(2*aj1ry*aj1sxy+2*aj1sy*aj1rxy+aj1ryy*
     & aj1sx+aj1syy*aj1rx)*aj1rxrs+(aj1syy*aj1sx+2*aj1sy*aj1sxy)*
     & aj1rxss+aj1rxyy*aj1rxr+aj1sxyy*aj1rxs
             t1 = aj1ry**2
             t7 = aj1sy**2
             aj1rxyyy = aj1ry*t1*aj1rxrrr+3*t1*aj1sy*aj1rxrrs+3*aj1ry*
     & t7*aj1rxrss+t7*aj1sy*aj1rxsss+3*aj1ry*aj1ryy*aj1rxrr+(3*aj1syy*
     & aj1ry+3*aj1sy*aj1ryy)*aj1rxrs+3*aj1syy*aj1sy*aj1rxss+aj1ryyy*
     & aj1rxr+aj1syyy*aj1rxs
             t1 = aj1rx**2
             t7 = aj1sx**2
             aj1sxxxx = t1*aj1rx*aj1sxrrr+3*t1*aj1sx*aj1sxrrs+3*aj1rx*
     & t7*aj1sxrss+t7*aj1sx*aj1sxsss+3*aj1rx*aj1rxx*aj1sxrr+(3*aj1sxx*
     & aj1rx+3*aj1sx*aj1rxx)*aj1sxrs+3*aj1sxx*aj1sx*aj1sxss+aj1rxxx*
     & aj1sxr+aj1sxxx*aj1sxs
             t1 = aj1rx**2
             t10 = aj1sx**2
             aj1sxxxy = aj1ry*t1*aj1sxrrr+(aj1sy*t1+2*aj1ry*aj1sx*
     & aj1rx)*aj1sxrrs+(aj1ry*t10+2*aj1sy*aj1sx*aj1rx)*aj1sxrss+aj1sy*
     & t10*aj1sxsss+(2*aj1rxy*aj1rx+aj1ry*aj1rxx)*aj1sxrr+(aj1ry*
     & aj1sxx+2*aj1sx*aj1rxy+2*aj1sxy*aj1rx+aj1sy*aj1rxx)*aj1sxrs+(
     & aj1sy*aj1sxx+2*aj1sxy*aj1sx)*aj1sxss+aj1rxxy*aj1sxr+aj1sxxy*
     & aj1sxs
             t1 = aj1ry**2
             t4 = aj1sy*aj1ry
             t8 = aj1sy*aj1rx+aj1ry*aj1sx
             t16 = aj1sy**2
             aj1sxxyy = t1*aj1rx*aj1sxrrr+(t4*aj1rx+aj1ry*t8)*aj1sxrrs+
     & (t4*aj1sx+aj1sy*t8)*aj1sxrss+t16*aj1sx*aj1sxsss+(aj1ryy*aj1rx+
     & 2*aj1ry*aj1rxy)*aj1sxrr+(2*aj1ry*aj1sxy+2*aj1sy*aj1rxy+aj1ryy*
     & aj1sx+aj1syy*aj1rx)*aj1sxrs+(aj1syy*aj1sx+2*aj1sy*aj1sxy)*
     & aj1sxss+aj1rxyy*aj1sxr+aj1sxyy*aj1sxs
             t1 = aj1ry**2
             t7 = aj1sy**2
             aj1sxyyy = aj1ry*t1*aj1sxrrr+3*t1*aj1sy*aj1sxrrs+3*aj1ry*
     & t7*aj1sxrss+t7*aj1sy*aj1sxsss+3*aj1ry*aj1ryy*aj1sxrr+(3*aj1syy*
     & aj1ry+3*aj1sy*aj1ryy)*aj1sxrs+3*aj1syy*aj1sy*aj1sxss+aj1ryyy*
     & aj1sxr+aj1syyy*aj1sxs
             t1 = aj1rx**2
             t7 = aj1sx**2
             aj1ryxxx = t1*aj1rx*aj1ryrrr+3*t1*aj1sx*aj1ryrrs+3*aj1rx*
     & t7*aj1ryrss+t7*aj1sx*aj1rysss+3*aj1rx*aj1rxx*aj1ryrr+(3*aj1sxx*
     & aj1rx+3*aj1sx*aj1rxx)*aj1ryrs+3*aj1sxx*aj1sx*aj1ryss+aj1rxxx*
     & aj1ryr+aj1sxxx*aj1rys
             t1 = aj1rx**2
             t10 = aj1sx**2
             aj1ryxxy = aj1ry*t1*aj1ryrrr+(aj1sy*t1+2*aj1ry*aj1sx*
     & aj1rx)*aj1ryrrs+(aj1ry*t10+2*aj1sy*aj1sx*aj1rx)*aj1ryrss+aj1sy*
     & t10*aj1rysss+(2*aj1rxy*aj1rx+aj1ry*aj1rxx)*aj1ryrr+(aj1ry*
     & aj1sxx+2*aj1sx*aj1rxy+2*aj1sxy*aj1rx+aj1sy*aj1rxx)*aj1ryrs+(
     & aj1sy*aj1sxx+2*aj1sxy*aj1sx)*aj1ryss+aj1rxxy*aj1ryr+aj1sxxy*
     & aj1rys
             t1 = aj1ry**2
             t4 = aj1sy*aj1ry
             t8 = aj1sy*aj1rx+aj1ry*aj1sx
             t16 = aj1sy**2
             aj1ryxyy = t1*aj1rx*aj1ryrrr+(t4*aj1rx+aj1ry*t8)*aj1ryrrs+
     & (t4*aj1sx+aj1sy*t8)*aj1ryrss+t16*aj1sx*aj1rysss+(aj1ryy*aj1rx+
     & 2*aj1ry*aj1rxy)*aj1ryrr+(2*aj1ry*aj1sxy+2*aj1sy*aj1rxy+aj1ryy*
     & aj1sx+aj1syy*aj1rx)*aj1ryrs+(aj1syy*aj1sx+2*aj1sy*aj1sxy)*
     & aj1ryss+aj1rxyy*aj1ryr+aj1sxyy*aj1rys
             t1 = aj1ry**2
             t7 = aj1sy**2
             aj1ryyyy = aj1ry*t1*aj1ryrrr+3*t1*aj1sy*aj1ryrrs+3*aj1ry*
     & t7*aj1ryrss+t7*aj1sy*aj1rysss+3*aj1ry*aj1ryy*aj1ryrr+(3*aj1syy*
     & aj1ry+3*aj1sy*aj1ryy)*aj1ryrs+3*aj1syy*aj1sy*aj1ryss+aj1ryyy*
     & aj1ryr+aj1syyy*aj1rys
             t1 = aj1rx**2
             t7 = aj1sx**2
             aj1syxxx = t1*aj1rx*aj1syrrr+3*t1*aj1sx*aj1syrrs+3*aj1rx*
     & t7*aj1syrss+t7*aj1sx*aj1sysss+3*aj1rx*aj1rxx*aj1syrr+(3*aj1sxx*
     & aj1rx+3*aj1sx*aj1rxx)*aj1syrs+3*aj1sxx*aj1sx*aj1syss+aj1rxxx*
     & aj1syr+aj1sxxx*aj1sys
             t1 = aj1rx**2
             t10 = aj1sx**2
             aj1syxxy = aj1ry*t1*aj1syrrr+(aj1sy*t1+2*aj1ry*aj1sx*
     & aj1rx)*aj1syrrs+(aj1ry*t10+2*aj1sy*aj1sx*aj1rx)*aj1syrss+aj1sy*
     & t10*aj1sysss+(2*aj1rxy*aj1rx+aj1ry*aj1rxx)*aj1syrr+(aj1ry*
     & aj1sxx+2*aj1sx*aj1rxy+2*aj1sxy*aj1rx+aj1sy*aj1rxx)*aj1syrs+(
     & aj1sy*aj1sxx+2*aj1sxy*aj1sx)*aj1syss+aj1rxxy*aj1syr+aj1sxxy*
     & aj1sys
             t1 = aj1ry**2
             t4 = aj1sy*aj1ry
             t8 = aj1sy*aj1rx+aj1ry*aj1sx
             t16 = aj1sy**2
             aj1syxyy = t1*aj1rx*aj1syrrr+(t4*aj1rx+aj1ry*t8)*aj1syrrs+
     & (t4*aj1sx+aj1sy*t8)*aj1syrss+t16*aj1sx*aj1sysss+(aj1ryy*aj1rx+
     & 2*aj1ry*aj1rxy)*aj1syrr+(2*aj1ry*aj1sxy+2*aj1sy*aj1rxy+aj1ryy*
     & aj1sx+aj1syy*aj1rx)*aj1syrs+(aj1syy*aj1sx+2*aj1sy*aj1sxy)*
     & aj1syss+aj1rxyy*aj1syr+aj1sxyy*aj1sys
             t1 = aj1ry**2
             t7 = aj1sy**2
             aj1syyyy = aj1ry*t1*aj1syrrr+3*t1*aj1sy*aj1syrrs+3*aj1ry*
     & t7*aj1syrss+t7*aj1sy*aj1sysss+3*aj1ry*aj1ryy*aj1syrr+(3*aj1syy*
     & aj1ry+3*aj1sy*aj1ryy)*aj1syrs+3*aj1syy*aj1sy*aj1syss+aj1ryyy*
     & aj1syr+aj1syyy*aj1sys
              uu1 = u1(i1,i2,i3,ex)
              uu1r = (-u1(i1-1,i2,i3,ex)+u1(i1+1,i2,i3,ex))/(2.*dr1(0))
              uu1s = (-u1(i1,i2-1,i3,ex)+u1(i1,i2+1,i3,ex))/(2.*dr1(1))
              uu1rr = (u1(i1-1,i2,i3,ex)-2.*u1(i1,i2,i3,ex)+u1(i1+1,i2,
     & i3,ex))/(dr1(0)**2)
              uu1rs = (-(-u1(i1-1,i2-1,i3,ex)+u1(i1-1,i2+1,i3,ex))/(2.*
     & dr1(1))+(-u1(i1+1,i2-1,i3,ex)+u1(i1+1,i2+1,i3,ex))/(2.*dr1(1)))
     & /(2.*dr1(0))
              uu1ss = (u1(i1,i2-1,i3,ex)-2.*u1(i1,i2,i3,ex)+u1(i1,i2+1,
     & i3,ex))/(dr1(1)**2)
              uu1rrr = (-u1(i1-2,i2,i3,ex)+2.*u1(i1-1,i2,i3,ex)-2.*u1(
     & i1+1,i2,i3,ex)+u1(i1+2,i2,i3,ex))/(2.*dr1(0)**3)
              uu1rrs = ((-u1(i1-1,i2-1,i3,ex)+u1(i1-1,i2+1,i3,ex))/(2.*
     & dr1(1))-2.*(-u1(i1,i2-1,i3,ex)+u1(i1,i2+1,i3,ex))/(2.*dr1(1))+(
     & -u1(i1+1,i2-1,i3,ex)+u1(i1+1,i2+1,i3,ex))/(2.*dr1(1)))/(dr1(0)*
     & *2)
              uu1rss = (-(u1(i1-1,i2-1,i3,ex)-2.*u1(i1-1,i2,i3,ex)+u1(
     & i1-1,i2+1,i3,ex))/(dr1(1)**2)+(u1(i1+1,i2-1,i3,ex)-2.*u1(i1+1,
     & i2,i3,ex)+u1(i1+1,i2+1,i3,ex))/(dr1(1)**2))/(2.*dr1(0))
              uu1sss = (-u1(i1,i2-2,i3,ex)+2.*u1(i1,i2-1,i3,ex)-2.*u1(
     & i1,i2+1,i3,ex)+u1(i1,i2+2,i3,ex))/(2.*dr1(1)**3)
              uu1rrrr = (u1(i1-2,i2,i3,ex)-4.*u1(i1-1,i2,i3,ex)+6.*u1(
     & i1,i2,i3,ex)-4.*u1(i1+1,i2,i3,ex)+u1(i1+2,i2,i3,ex))/(dr1(0)**
     & 4)
              uu1rrrs = (-(-u1(i1-2,i2-1,i3,ex)+u1(i1-2,i2+1,i3,ex))/(
     & 2.*dr1(1))+2.*(-u1(i1-1,i2-1,i3,ex)+u1(i1-1,i2+1,i3,ex))/(2.*
     & dr1(1))-2.*(-u1(i1+1,i2-1,i3,ex)+u1(i1+1,i2+1,i3,ex))/(2.*dr1(
     & 1))+(-u1(i1+2,i2-1,i3,ex)+u1(i1+2,i2+1,i3,ex))/(2.*dr1(1)))/(
     & 2.*dr1(0)**3)
              uu1rrss = ((u1(i1-1,i2-1,i3,ex)-2.*u1(i1-1,i2,i3,ex)+u1(
     & i1-1,i2+1,i3,ex))/(dr1(1)**2)-2.*(u1(i1,i2-1,i3,ex)-2.*u1(i1,
     & i2,i3,ex)+u1(i1,i2+1,i3,ex))/(dr1(1)**2)+(u1(i1+1,i2-1,i3,ex)-
     & 2.*u1(i1+1,i2,i3,ex)+u1(i1+1,i2+1,i3,ex))/(dr1(1)**2))/(dr1(0)*
     & *2)
              uu1rsss = (-(-u1(i1-1,i2-2,i3,ex)+2.*u1(i1-1,i2-1,i3,ex)-
     & 2.*u1(i1-1,i2+1,i3,ex)+u1(i1-1,i2+2,i3,ex))/(2.*dr1(1)**3)+(-
     & u1(i1+1,i2-2,i3,ex)+2.*u1(i1+1,i2-1,i3,ex)-2.*u1(i1+1,i2+1,i3,
     & ex)+u1(i1+1,i2+2,i3,ex))/(2.*dr1(1)**3))/(2.*dr1(0))
              uu1ssss = (u1(i1,i2-2,i3,ex)-4.*u1(i1,i2-1,i3,ex)+6.*u1(
     & i1,i2,i3,ex)-4.*u1(i1,i2+1,i3,ex)+u1(i1,i2+2,i3,ex))/(dr1(1)**
     & 4)
               t1 = aj1rx**2
               t7 = aj1sx**2
               u1xxx = t1*aj1rx*uu1rrr+3*t1*aj1sx*uu1rrs+3*aj1rx*t7*
     & uu1rss+t7*aj1sx*uu1sss+3*aj1rx*aj1rxx*uu1rr+(3*aj1sxx*aj1rx+3*
     & aj1sx*aj1rxx)*uu1rs+3*aj1sxx*aj1sx*uu1ss+aj1rxxx*uu1r+aj1sxxx*
     & uu1s
               t1 = aj1rx**2
               t10 = aj1sx**2
               u1xxy = aj1ry*t1*uu1rrr+(aj1sy*t1+2*aj1ry*aj1sx*aj1rx)*
     & uu1rrs+(aj1ry*t10+2*aj1sy*aj1sx*aj1rx)*uu1rss+aj1sy*t10*uu1sss+
     & (2*aj1rxy*aj1rx+aj1ry*aj1rxx)*uu1rr+(aj1ry*aj1sxx+2*aj1sx*
     & aj1rxy+2*aj1sxy*aj1rx+aj1sy*aj1rxx)*uu1rs+(aj1sy*aj1sxx+2*
     & aj1sxy*aj1sx)*uu1ss+aj1rxxy*uu1r+aj1sxxy*uu1s
               t1 = aj1ry**2
               t4 = aj1sy*aj1ry
               t8 = aj1sy*aj1rx+aj1ry*aj1sx
               t16 = aj1sy**2
               u1xyy = t1*aj1rx*uu1rrr+(t4*aj1rx+aj1ry*t8)*uu1rrs+(t4*
     & aj1sx+aj1sy*t8)*uu1rss+t16*aj1sx*uu1sss+(aj1ryy*aj1rx+2*aj1ry*
     & aj1rxy)*uu1rr+(2*aj1ry*aj1sxy+2*aj1sy*aj1rxy+aj1ryy*aj1sx+
     & aj1syy*aj1rx)*uu1rs+(aj1syy*aj1sx+2*aj1sy*aj1sxy)*uu1ss+
     & aj1rxyy*uu1r+aj1sxyy*uu1s
               t1 = aj1ry**2
               t7 = aj1sy**2
               u1yyy = aj1ry*t1*uu1rrr+3*t1*aj1sy*uu1rrs+3*aj1ry*t7*
     & uu1rss+t7*aj1sy*uu1sss+3*aj1ry*aj1ryy*uu1rr+(3*aj1syy*aj1ry+3*
     & aj1sy*aj1ryy)*uu1rs+3*aj1syy*aj1sy*uu1ss+aj1ryyy*uu1r+aj1syyy*
     & uu1s
               t1 = aj1rx**2
               t2 = t1**2
               t8 = aj1sx**2
               t16 = t8**2
               t25 = aj1sxx*aj1rx
               t27 = t25+aj1sx*aj1rxx
               t28 = 3*t27
               t30 = 2*t27
               t46 = aj1rxx**2
               t60 = aj1sxx**2
               u1xxxx = t2*uu1rrrr+4*t1*aj1rx*aj1sx*uu1rrrs+6*t1*t8*
     & uu1rrss+4*aj1rx*t8*aj1sx*uu1rsss+t16*uu1ssss+6*t1*aj1rxx*
     & uu1rrr+(7*aj1sx*aj1rx*aj1rxx+aj1sxx*t1+aj1rx*t28+aj1rx*t30)*
     & uu1rrs+(aj1sx*t28+7*t25*aj1sx+aj1rxx*t8+aj1sx*t30)*uu1rss+6*t8*
     & aj1sxx*uu1sss+(4*aj1rx*aj1rxxx+3*t46)*uu1rr+(4*aj1sxxx*aj1rx+4*
     & aj1sx*aj1rxxx+6*aj1sxx*aj1rxx)*uu1rs+(4*aj1sxxx*aj1sx+3*t60)*
     & uu1ss+aj1rxxxx*uu1r+aj1sxxxx*uu1s
               t1 = aj1ry**2
               t2 = aj1rx**2
               t5 = aj1sy*aj1ry
               t11 = aj1sy*t2+2*aj1ry*aj1sx*aj1rx
               t16 = aj1sx**2
               t21 = aj1ry*t16+2*aj1sy*aj1sx*aj1rx
               t29 = aj1sy**2
               t38 = 2*aj1rxy*aj1rx+aj1ry*aj1rxx
               t52 = aj1sx*aj1rxy
               t54 = aj1sxy*aj1rx
               t57 = aj1ry*aj1sxx+2*t52+2*t54+aj1sy*aj1rxx
               t60 = 2*t52+2*t54
               t68 = aj1sy*aj1sxx+2*aj1sxy*aj1sx
               t92 = aj1rxy**2
               t110 = aj1sxy**2
               u1xxyy = t1*t2*uu1rrrr+(t5*t2+aj1ry*t11)*uu1rrrs+(aj1sy*
     & t11+aj1ry*t21)*uu1rrss+(aj1sy*t21+t5*t16)*uu1rsss+t29*t16*
     & uu1ssss+(2*aj1ry*aj1rxy*aj1rx+aj1ry*t38+aj1ryy*t2)*uu1rrr+(
     & aj1sy*t38+2*aj1sy*aj1rxy*aj1rx+2*aj1ryy*aj1sx*aj1rx+aj1syy*t2+
     & aj1ry*t57+aj1ry*t60)*uu1rrs+(aj1sy*t57+aj1ry*t68+aj1ryy*t16+2*
     & aj1ry*aj1sxy*aj1sx+2*aj1syy*aj1sx*aj1rx+aj1sy*t60)*uu1rss+(2*
     & aj1sy*aj1sxy*aj1sx+aj1sy*t68+aj1syy*t16)*uu1sss+(2*aj1rx*
     & aj1rxyy+aj1ryy*aj1rxx+2*aj1ry*aj1rxxy+2*t92)*uu1rr+(4*aj1sxy*
     & aj1rxy+2*aj1ry*aj1sxxy+aj1ryy*aj1sxx+2*aj1sy*aj1rxxy+2*aj1sxyy*
     & aj1rx+aj1syy*aj1rxx+2*aj1sx*aj1rxyy)*uu1rs+(2*t110+2*aj1sy*
     & aj1sxxy+aj1syy*aj1sxx+2*aj1sx*aj1sxyy)*uu1ss+aj1rxxyy*uu1r+
     & aj1sxxyy*uu1s
               t1 = aj1ry**2
               t2 = t1**2
               t8 = aj1sy**2
               t16 = t8**2
               t25 = aj1syy*aj1ry
               t27 = t25+aj1sy*aj1ryy
               t28 = 3*t27
               t30 = 2*t27
               t46 = aj1ryy**2
               t60 = aj1syy**2
               u1yyyy = t2*uu1rrrr+4*t1*aj1ry*aj1sy*uu1rrrs+6*t1*t8*
     & uu1rrss+4*aj1ry*t8*aj1sy*uu1rsss+t16*uu1ssss+6*t1*aj1ryy*
     & uu1rrr+(7*aj1sy*aj1ry*aj1ryy+aj1syy*t1+aj1ry*t28+aj1ry*t30)*
     & uu1rrs+(aj1sy*t28+7*t25*aj1sy+aj1ryy*t8+aj1sy*t30)*uu1rss+6*t8*
     & aj1syy*uu1sss+(4*aj1ry*aj1ryyy+3*t46)*uu1rr+(4*aj1syyy*aj1ry+4*
     & aj1sy*aj1ryyy+6*aj1syy*aj1ryy)*uu1rs+(4*aj1syyy*aj1sy+3*t60)*
     & uu1ss+aj1ryyyy*uu1r+aj1syyyy*uu1s
             u1LapSq = u1xxxx +2.* u1xxyy + u1yyyy
              vv1 = u1(i1,i2,i3,ey)
              vv1r = (-u1(i1-1,i2,i3,ey)+u1(i1+1,i2,i3,ey))/(2.*dr1(0))
              vv1s = (-u1(i1,i2-1,i3,ey)+u1(i1,i2+1,i3,ey))/(2.*dr1(1))
              vv1rr = (u1(i1-1,i2,i3,ey)-2.*u1(i1,i2,i3,ey)+u1(i1+1,i2,
     & i3,ey))/(dr1(0)**2)
              vv1rs = (-(-u1(i1-1,i2-1,i3,ey)+u1(i1-1,i2+1,i3,ey))/(2.*
     & dr1(1))+(-u1(i1+1,i2-1,i3,ey)+u1(i1+1,i2+1,i3,ey))/(2.*dr1(1)))
     & /(2.*dr1(0))
              vv1ss = (u1(i1,i2-1,i3,ey)-2.*u1(i1,i2,i3,ey)+u1(i1,i2+1,
     & i3,ey))/(dr1(1)**2)
              vv1rrr = (-u1(i1-2,i2,i3,ey)+2.*u1(i1-1,i2,i3,ey)-2.*u1(
     & i1+1,i2,i3,ey)+u1(i1+2,i2,i3,ey))/(2.*dr1(0)**3)
              vv1rrs = ((-u1(i1-1,i2-1,i3,ey)+u1(i1-1,i2+1,i3,ey))/(2.*
     & dr1(1))-2.*(-u1(i1,i2-1,i3,ey)+u1(i1,i2+1,i3,ey))/(2.*dr1(1))+(
     & -u1(i1+1,i2-1,i3,ey)+u1(i1+1,i2+1,i3,ey))/(2.*dr1(1)))/(dr1(0)*
     & *2)
              vv1rss = (-(u1(i1-1,i2-1,i3,ey)-2.*u1(i1-1,i2,i3,ey)+u1(
     & i1-1,i2+1,i3,ey))/(dr1(1)**2)+(u1(i1+1,i2-1,i3,ey)-2.*u1(i1+1,
     & i2,i3,ey)+u1(i1+1,i2+1,i3,ey))/(dr1(1)**2))/(2.*dr1(0))
              vv1sss = (-u1(i1,i2-2,i3,ey)+2.*u1(i1,i2-1,i3,ey)-2.*u1(
     & i1,i2+1,i3,ey)+u1(i1,i2+2,i3,ey))/(2.*dr1(1)**3)
              vv1rrrr = (u1(i1-2,i2,i3,ey)-4.*u1(i1-1,i2,i3,ey)+6.*u1(
     & i1,i2,i3,ey)-4.*u1(i1+1,i2,i3,ey)+u1(i1+2,i2,i3,ey))/(dr1(0)**
     & 4)
              vv1rrrs = (-(-u1(i1-2,i2-1,i3,ey)+u1(i1-2,i2+1,i3,ey))/(
     & 2.*dr1(1))+2.*(-u1(i1-1,i2-1,i3,ey)+u1(i1-1,i2+1,i3,ey))/(2.*
     & dr1(1))-2.*(-u1(i1+1,i2-1,i3,ey)+u1(i1+1,i2+1,i3,ey))/(2.*dr1(
     & 1))+(-u1(i1+2,i2-1,i3,ey)+u1(i1+2,i2+1,i3,ey))/(2.*dr1(1)))/(
     & 2.*dr1(0)**3)
              vv1rrss = ((u1(i1-1,i2-1,i3,ey)-2.*u1(i1-1,i2,i3,ey)+u1(
     & i1-1,i2+1,i3,ey))/(dr1(1)**2)-2.*(u1(i1,i2-1,i3,ey)-2.*u1(i1,
     & i2,i3,ey)+u1(i1,i2+1,i3,ey))/(dr1(1)**2)+(u1(i1+1,i2-1,i3,ey)-
     & 2.*u1(i1+1,i2,i3,ey)+u1(i1+1,i2+1,i3,ey))/(dr1(1)**2))/(dr1(0)*
     & *2)
              vv1rsss = (-(-u1(i1-1,i2-2,i3,ey)+2.*u1(i1-1,i2-1,i3,ey)-
     & 2.*u1(i1-1,i2+1,i3,ey)+u1(i1-1,i2+2,i3,ey))/(2.*dr1(1)**3)+(-
     & u1(i1+1,i2-2,i3,ey)+2.*u1(i1+1,i2-1,i3,ey)-2.*u1(i1+1,i2+1,i3,
     & ey)+u1(i1+1,i2+2,i3,ey))/(2.*dr1(1)**3))/(2.*dr1(0))
              vv1ssss = (u1(i1,i2-2,i3,ey)-4.*u1(i1,i2-1,i3,ey)+6.*u1(
     & i1,i2,i3,ey)-4.*u1(i1,i2+1,i3,ey)+u1(i1,i2+2,i3,ey))/(dr1(1)**
     & 4)
               t1 = aj1rx**2
               t7 = aj1sx**2
               v1xxx = t1*aj1rx*vv1rrr+3*t1*aj1sx*vv1rrs+3*aj1rx*t7*
     & vv1rss+t7*aj1sx*vv1sss+3*aj1rx*aj1rxx*vv1rr+(3*aj1sxx*aj1rx+3*
     & aj1sx*aj1rxx)*vv1rs+3*aj1sxx*aj1sx*vv1ss+aj1rxxx*vv1r+aj1sxxx*
     & vv1s
               t1 = aj1rx**2
               t10 = aj1sx**2
               v1xxy = aj1ry*t1*vv1rrr+(aj1sy*t1+2*aj1ry*aj1sx*aj1rx)*
     & vv1rrs+(aj1ry*t10+2*aj1sy*aj1sx*aj1rx)*vv1rss+aj1sy*t10*vv1sss+
     & (2*aj1rxy*aj1rx+aj1ry*aj1rxx)*vv1rr+(aj1ry*aj1sxx+2*aj1sx*
     & aj1rxy+2*aj1sxy*aj1rx+aj1sy*aj1rxx)*vv1rs+(aj1sy*aj1sxx+2*
     & aj1sxy*aj1sx)*vv1ss+aj1rxxy*vv1r+aj1sxxy*vv1s
               t1 = aj1ry**2
               t4 = aj1sy*aj1ry
               t8 = aj1sy*aj1rx+aj1ry*aj1sx
               t16 = aj1sy**2
               v1xyy = t1*aj1rx*vv1rrr+(t4*aj1rx+aj1ry*t8)*vv1rrs+(t4*
     & aj1sx+aj1sy*t8)*vv1rss+t16*aj1sx*vv1sss+(aj1ryy*aj1rx+2*aj1ry*
     & aj1rxy)*vv1rr+(2*aj1ry*aj1sxy+2*aj1sy*aj1rxy+aj1ryy*aj1sx+
     & aj1syy*aj1rx)*vv1rs+(aj1syy*aj1sx+2*aj1sy*aj1sxy)*vv1ss+
     & aj1rxyy*vv1r+aj1sxyy*vv1s
               t1 = aj1ry**2
               t7 = aj1sy**2
               v1yyy = aj1ry*t1*vv1rrr+3*t1*aj1sy*vv1rrs+3*aj1ry*t7*
     & vv1rss+t7*aj1sy*vv1sss+3*aj1ry*aj1ryy*vv1rr+(3*aj1syy*aj1ry+3*
     & aj1sy*aj1ryy)*vv1rs+3*aj1syy*aj1sy*vv1ss+aj1ryyy*vv1r+aj1syyy*
     & vv1s
               t1 = aj1rx**2
               t2 = t1**2
               t8 = aj1sx**2
               t16 = t8**2
               t25 = aj1sxx*aj1rx
               t27 = t25+aj1sx*aj1rxx
               t28 = 3*t27
               t30 = 2*t27
               t46 = aj1rxx**2
               t60 = aj1sxx**2
               v1xxxx = t2*vv1rrrr+4*t1*aj1rx*aj1sx*vv1rrrs+6*t1*t8*
     & vv1rrss+4*aj1rx*t8*aj1sx*vv1rsss+t16*vv1ssss+6*t1*aj1rxx*
     & vv1rrr+(7*aj1sx*aj1rx*aj1rxx+aj1sxx*t1+aj1rx*t28+aj1rx*t30)*
     & vv1rrs+(aj1sx*t28+7*t25*aj1sx+aj1rxx*t8+aj1sx*t30)*vv1rss+6*t8*
     & aj1sxx*vv1sss+(4*aj1rx*aj1rxxx+3*t46)*vv1rr+(4*aj1sxxx*aj1rx+4*
     & aj1sx*aj1rxxx+6*aj1sxx*aj1rxx)*vv1rs+(4*aj1sxxx*aj1sx+3*t60)*
     & vv1ss+aj1rxxxx*vv1r+aj1sxxxx*vv1s
               t1 = aj1ry**2
               t2 = aj1rx**2
               t5 = aj1sy*aj1ry
               t11 = aj1sy*t2+2*aj1ry*aj1sx*aj1rx
               t16 = aj1sx**2
               t21 = aj1ry*t16+2*aj1sy*aj1sx*aj1rx
               t29 = aj1sy**2
               t38 = 2*aj1rxy*aj1rx+aj1ry*aj1rxx
               t52 = aj1sx*aj1rxy
               t54 = aj1sxy*aj1rx
               t57 = aj1ry*aj1sxx+2*t52+2*t54+aj1sy*aj1rxx
               t60 = 2*t52+2*t54
               t68 = aj1sy*aj1sxx+2*aj1sxy*aj1sx
               t92 = aj1rxy**2
               t110 = aj1sxy**2
               v1xxyy = t1*t2*vv1rrrr+(t5*t2+aj1ry*t11)*vv1rrrs+(aj1sy*
     & t11+aj1ry*t21)*vv1rrss+(aj1sy*t21+t5*t16)*vv1rsss+t29*t16*
     & vv1ssss+(2*aj1ry*aj1rxy*aj1rx+aj1ry*t38+aj1ryy*t2)*vv1rrr+(
     & aj1sy*t38+2*aj1sy*aj1rxy*aj1rx+2*aj1ryy*aj1sx*aj1rx+aj1syy*t2+
     & aj1ry*t57+aj1ry*t60)*vv1rrs+(aj1sy*t57+aj1ry*t68+aj1ryy*t16+2*
     & aj1ry*aj1sxy*aj1sx+2*aj1syy*aj1sx*aj1rx+aj1sy*t60)*vv1rss+(2*
     & aj1sy*aj1sxy*aj1sx+aj1sy*t68+aj1syy*t16)*vv1sss+(2*aj1rx*
     & aj1rxyy+aj1ryy*aj1rxx+2*aj1ry*aj1rxxy+2*t92)*vv1rr+(4*aj1sxy*
     & aj1rxy+2*aj1ry*aj1sxxy+aj1ryy*aj1sxx+2*aj1sy*aj1rxxy+2*aj1sxyy*
     & aj1rx+aj1syy*aj1rxx+2*aj1sx*aj1rxyy)*vv1rs+(2*t110+2*aj1sy*
     & aj1sxxy+aj1syy*aj1sxx+2*aj1sx*aj1sxyy)*vv1ss+aj1rxxyy*vv1r+
     & aj1sxxyy*vv1s
               t1 = aj1ry**2
               t2 = t1**2
               t8 = aj1sy**2
               t16 = t8**2
               t25 = aj1syy*aj1ry
               t27 = t25+aj1sy*aj1ryy
               t28 = 3*t27
               t30 = 2*t27
               t46 = aj1ryy**2
               t60 = aj1syy**2
               v1yyyy = t2*vv1rrrr+4*t1*aj1ry*aj1sy*vv1rrrs+6*t1*t8*
     & vv1rrss+4*aj1ry*t8*aj1sy*vv1rsss+t16*vv1ssss+6*t1*aj1ryy*
     & vv1rrr+(7*aj1sy*aj1ry*aj1ryy+aj1syy*t1+aj1ry*t28+aj1ry*t30)*
     & vv1rrs+(aj1sy*t28+7*t25*aj1sy+aj1ryy*t8+aj1sy*t30)*vv1rss+6*t8*
     & aj1syy*vv1sss+(4*aj1ry*aj1ryyy+3*t46)*vv1rr+(4*aj1syyy*aj1ry+4*
     & aj1sy*aj1ryyy+6*aj1syy*aj1ryy)*vv1rs+(4*aj1syyy*aj1sy+3*t60)*
     & vv1ss+aj1ryyyy*vv1r+aj1syyyy*vv1s
             v1LapSq = v1xxxx +2.* v1xxyy + v1yyyy
            ! NOTE: the jacobian derivatives can be computed once for all components
             ! this next call will define the jacobian and its derivatives (parameteric and spatial)
             aj2rx = rsxy2(j1,j2,j3,0,0)
             aj2rxr = (-rsxy2(j1-1,j2,j3,0,0)+rsxy2(j1+1,j2,j3,0,0))/(
     & 2.*dr2(0))
             aj2rxs = (-rsxy2(j1,j2-1,j3,0,0)+rsxy2(j1,j2+1,j3,0,0))/(
     & 2.*dr2(1))
             aj2rxrr = (rsxy2(j1-1,j2,j3,0,0)-2.*rsxy2(j1,j2,j3,0,0)+
     & rsxy2(j1+1,j2,j3,0,0))/(dr2(0)**2)
             aj2rxrs = (-(-rsxy2(j1-1,j2-1,j3,0,0)+rsxy2(j1-1,j2+1,j3,
     & 0,0))/(2.*dr2(1))+(-rsxy2(j1+1,j2-1,j3,0,0)+rsxy2(j1+1,j2+1,j3,
     & 0,0))/(2.*dr2(1)))/(2.*dr2(0))
             aj2rxss = (rsxy2(j1,j2-1,j3,0,0)-2.*rsxy2(j1,j2,j3,0,0)+
     & rsxy2(j1,j2+1,j3,0,0))/(dr2(1)**2)
             aj2rxrrr = (-rsxy2(j1-2,j2,j3,0,0)+2.*rsxy2(j1-1,j2,j3,0,
     & 0)-2.*rsxy2(j1+1,j2,j3,0,0)+rsxy2(j1+2,j2,j3,0,0))/(2.*dr2(0)**
     & 3)
             aj2rxrrs = ((-rsxy2(j1-1,j2-1,j3,0,0)+rsxy2(j1-1,j2+1,j3,
     & 0,0))/(2.*dr2(1))-2.*(-rsxy2(j1,j2-1,j3,0,0)+rsxy2(j1,j2+1,j3,
     & 0,0))/(2.*dr2(1))+(-rsxy2(j1+1,j2-1,j3,0,0)+rsxy2(j1+1,j2+1,j3,
     & 0,0))/(2.*dr2(1)))/(dr2(0)**2)
             aj2rxrss = (-(rsxy2(j1-1,j2-1,j3,0,0)-2.*rsxy2(j1-1,j2,j3,
     & 0,0)+rsxy2(j1-1,j2+1,j3,0,0))/(dr2(1)**2)+(rsxy2(j1+1,j2-1,j3,
     & 0,0)-2.*rsxy2(j1+1,j2,j3,0,0)+rsxy2(j1+1,j2+1,j3,0,0))/(dr2(1)*
     & *2))/(2.*dr2(0))
             aj2rxsss = (-rsxy2(j1,j2-2,j3,0,0)+2.*rsxy2(j1,j2-1,j3,0,
     & 0)-2.*rsxy2(j1,j2+1,j3,0,0)+rsxy2(j1,j2+2,j3,0,0))/(2.*dr2(1)**
     & 3)
             aj2sx = rsxy2(j1,j2,j3,1,0)
             aj2sxr = (-rsxy2(j1-1,j2,j3,1,0)+rsxy2(j1+1,j2,j3,1,0))/(
     & 2.*dr2(0))
             aj2sxs = (-rsxy2(j1,j2-1,j3,1,0)+rsxy2(j1,j2+1,j3,1,0))/(
     & 2.*dr2(1))
             aj2sxrr = (rsxy2(j1-1,j2,j3,1,0)-2.*rsxy2(j1,j2,j3,1,0)+
     & rsxy2(j1+1,j2,j3,1,0))/(dr2(0)**2)
             aj2sxrs = (-(-rsxy2(j1-1,j2-1,j3,1,0)+rsxy2(j1-1,j2+1,j3,
     & 1,0))/(2.*dr2(1))+(-rsxy2(j1+1,j2-1,j3,1,0)+rsxy2(j1+1,j2+1,j3,
     & 1,0))/(2.*dr2(1)))/(2.*dr2(0))
             aj2sxss = (rsxy2(j1,j2-1,j3,1,0)-2.*rsxy2(j1,j2,j3,1,0)+
     & rsxy2(j1,j2+1,j3,1,0))/(dr2(1)**2)
             aj2sxrrr = (-rsxy2(j1-2,j2,j3,1,0)+2.*rsxy2(j1-1,j2,j3,1,
     & 0)-2.*rsxy2(j1+1,j2,j3,1,0)+rsxy2(j1+2,j2,j3,1,0))/(2.*dr2(0)**
     & 3)
             aj2sxrrs = ((-rsxy2(j1-1,j2-1,j3,1,0)+rsxy2(j1-1,j2+1,j3,
     & 1,0))/(2.*dr2(1))-2.*(-rsxy2(j1,j2-1,j3,1,0)+rsxy2(j1,j2+1,j3,
     & 1,0))/(2.*dr2(1))+(-rsxy2(j1+1,j2-1,j3,1,0)+rsxy2(j1+1,j2+1,j3,
     & 1,0))/(2.*dr2(1)))/(dr2(0)**2)
             aj2sxrss = (-(rsxy2(j1-1,j2-1,j3,1,0)-2.*rsxy2(j1-1,j2,j3,
     & 1,0)+rsxy2(j1-1,j2+1,j3,1,0))/(dr2(1)**2)+(rsxy2(j1+1,j2-1,j3,
     & 1,0)-2.*rsxy2(j1+1,j2,j3,1,0)+rsxy2(j1+1,j2+1,j3,1,0))/(dr2(1)*
     & *2))/(2.*dr2(0))
             aj2sxsss = (-rsxy2(j1,j2-2,j3,1,0)+2.*rsxy2(j1,j2-1,j3,1,
     & 0)-2.*rsxy2(j1,j2+1,j3,1,0)+rsxy2(j1,j2+2,j3,1,0))/(2.*dr2(1)**
     & 3)
             aj2ry = rsxy2(j1,j2,j3,0,1)
             aj2ryr = (-rsxy2(j1-1,j2,j3,0,1)+rsxy2(j1+1,j2,j3,0,1))/(
     & 2.*dr2(0))
             aj2rys = (-rsxy2(j1,j2-1,j3,0,1)+rsxy2(j1,j2+1,j3,0,1))/(
     & 2.*dr2(1))
             aj2ryrr = (rsxy2(j1-1,j2,j3,0,1)-2.*rsxy2(j1,j2,j3,0,1)+
     & rsxy2(j1+1,j2,j3,0,1))/(dr2(0)**2)
             aj2ryrs = (-(-rsxy2(j1-1,j2-1,j3,0,1)+rsxy2(j1-1,j2+1,j3,
     & 0,1))/(2.*dr2(1))+(-rsxy2(j1+1,j2-1,j3,0,1)+rsxy2(j1+1,j2+1,j3,
     & 0,1))/(2.*dr2(1)))/(2.*dr2(0))
             aj2ryss = (rsxy2(j1,j2-1,j3,0,1)-2.*rsxy2(j1,j2,j3,0,1)+
     & rsxy2(j1,j2+1,j3,0,1))/(dr2(1)**2)
             aj2ryrrr = (-rsxy2(j1-2,j2,j3,0,1)+2.*rsxy2(j1-1,j2,j3,0,
     & 1)-2.*rsxy2(j1+1,j2,j3,0,1)+rsxy2(j1+2,j2,j3,0,1))/(2.*dr2(0)**
     & 3)
             aj2ryrrs = ((-rsxy2(j1-1,j2-1,j3,0,1)+rsxy2(j1-1,j2+1,j3,
     & 0,1))/(2.*dr2(1))-2.*(-rsxy2(j1,j2-1,j3,0,1)+rsxy2(j1,j2+1,j3,
     & 0,1))/(2.*dr2(1))+(-rsxy2(j1+1,j2-1,j3,0,1)+rsxy2(j1+1,j2+1,j3,
     & 0,1))/(2.*dr2(1)))/(dr2(0)**2)
             aj2ryrss = (-(rsxy2(j1-1,j2-1,j3,0,1)-2.*rsxy2(j1-1,j2,j3,
     & 0,1)+rsxy2(j1-1,j2+1,j3,0,1))/(dr2(1)**2)+(rsxy2(j1+1,j2-1,j3,
     & 0,1)-2.*rsxy2(j1+1,j2,j3,0,1)+rsxy2(j1+1,j2+1,j3,0,1))/(dr2(1)*
     & *2))/(2.*dr2(0))
             aj2rysss = (-rsxy2(j1,j2-2,j3,0,1)+2.*rsxy2(j1,j2-1,j3,0,
     & 1)-2.*rsxy2(j1,j2+1,j3,0,1)+rsxy2(j1,j2+2,j3,0,1))/(2.*dr2(1)**
     & 3)
             aj2sy = rsxy2(j1,j2,j3,1,1)
             aj2syr = (-rsxy2(j1-1,j2,j3,1,1)+rsxy2(j1+1,j2,j3,1,1))/(
     & 2.*dr2(0))
             aj2sys = (-rsxy2(j1,j2-1,j3,1,1)+rsxy2(j1,j2+1,j3,1,1))/(
     & 2.*dr2(1))
             aj2syrr = (rsxy2(j1-1,j2,j3,1,1)-2.*rsxy2(j1,j2,j3,1,1)+
     & rsxy2(j1+1,j2,j3,1,1))/(dr2(0)**2)
             aj2syrs = (-(-rsxy2(j1-1,j2-1,j3,1,1)+rsxy2(j1-1,j2+1,j3,
     & 1,1))/(2.*dr2(1))+(-rsxy2(j1+1,j2-1,j3,1,1)+rsxy2(j1+1,j2+1,j3,
     & 1,1))/(2.*dr2(1)))/(2.*dr2(0))
             aj2syss = (rsxy2(j1,j2-1,j3,1,1)-2.*rsxy2(j1,j2,j3,1,1)+
     & rsxy2(j1,j2+1,j3,1,1))/(dr2(1)**2)
             aj2syrrr = (-rsxy2(j1-2,j2,j3,1,1)+2.*rsxy2(j1-1,j2,j3,1,
     & 1)-2.*rsxy2(j1+1,j2,j3,1,1)+rsxy2(j1+2,j2,j3,1,1))/(2.*dr2(0)**
     & 3)
             aj2syrrs = ((-rsxy2(j1-1,j2-1,j3,1,1)+rsxy2(j1-1,j2+1,j3,
     & 1,1))/(2.*dr2(1))-2.*(-rsxy2(j1,j2-1,j3,1,1)+rsxy2(j1,j2+1,j3,
     & 1,1))/(2.*dr2(1))+(-rsxy2(j1+1,j2-1,j3,1,1)+rsxy2(j1+1,j2+1,j3,
     & 1,1))/(2.*dr2(1)))/(dr2(0)**2)
             aj2syrss = (-(rsxy2(j1-1,j2-1,j3,1,1)-2.*rsxy2(j1-1,j2,j3,
     & 1,1)+rsxy2(j1-1,j2+1,j3,1,1))/(dr2(1)**2)+(rsxy2(j1+1,j2-1,j3,
     & 1,1)-2.*rsxy2(j1+1,j2,j3,1,1)+rsxy2(j1+1,j2+1,j3,1,1))/(dr2(1)*
     & *2))/(2.*dr2(0))
             aj2sysss = (-rsxy2(j1,j2-2,j3,1,1)+2.*rsxy2(j1,j2-1,j3,1,
     & 1)-2.*rsxy2(j1,j2+1,j3,1,1)+rsxy2(j1,j2+2,j3,1,1))/(2.*dr2(1)**
     & 3)
             aj2rxx = aj2rx*aj2rxr+aj2sx*aj2rxs
             aj2rxy = aj2ry*aj2rxr+aj2sy*aj2rxs
             aj2sxx = aj2rx*aj2sxr+aj2sx*aj2sxs
             aj2sxy = aj2ry*aj2sxr+aj2sy*aj2sxs
             aj2ryx = aj2rx*aj2ryr+aj2sx*aj2rys
             aj2ryy = aj2ry*aj2ryr+aj2sy*aj2rys
             aj2syx = aj2rx*aj2syr+aj2sx*aj2sys
             aj2syy = aj2ry*aj2syr+aj2sy*aj2sys
             t1 = aj2rx**2
             t6 = aj2sx**2
             aj2rxxx = t1*aj2rxrr+2*aj2rx*aj2sx*aj2rxrs+t6*aj2rxss+
     & aj2rxx*aj2rxr+aj2sxx*aj2rxs
             aj2rxxy = aj2ry*aj2rx*aj2rxrr+(aj2sy*aj2rx+aj2ry*aj2sx)*
     & aj2rxrs+aj2sy*aj2sx*aj2rxss+aj2rxy*aj2rxr+aj2sxy*aj2rxs
             t1 = aj2ry**2
             t6 = aj2sy**2
             aj2rxyy = t1*aj2rxrr+2*aj2ry*aj2sy*aj2rxrs+t6*aj2rxss+
     & aj2ryy*aj2rxr+aj2syy*aj2rxs
             t1 = aj2rx**2
             t6 = aj2sx**2
             aj2sxxx = t1*aj2sxrr+2*aj2rx*aj2sx*aj2sxrs+t6*aj2sxss+
     & aj2rxx*aj2sxr+aj2sxx*aj2sxs
             aj2sxxy = aj2ry*aj2rx*aj2sxrr+(aj2sy*aj2rx+aj2ry*aj2sx)*
     & aj2sxrs+aj2sy*aj2sx*aj2sxss+aj2rxy*aj2sxr+aj2sxy*aj2sxs
             t1 = aj2ry**2
             t6 = aj2sy**2
             aj2sxyy = t1*aj2sxrr+2*aj2ry*aj2sy*aj2sxrs+t6*aj2sxss+
     & aj2ryy*aj2sxr+aj2syy*aj2sxs
             t1 = aj2rx**2
             t6 = aj2sx**2
             aj2ryxx = t1*aj2ryrr+2*aj2rx*aj2sx*aj2ryrs+t6*aj2ryss+
     & aj2rxx*aj2ryr+aj2sxx*aj2rys
             aj2ryxy = aj2ry*aj2rx*aj2ryrr+(aj2sy*aj2rx+aj2ry*aj2sx)*
     & aj2ryrs+aj2sy*aj2sx*aj2ryss+aj2rxy*aj2ryr+aj2sxy*aj2rys
             t1 = aj2ry**2
             t6 = aj2sy**2
             aj2ryyy = t1*aj2ryrr+2*aj2ry*aj2sy*aj2ryrs+t6*aj2ryss+
     & aj2ryy*aj2ryr+aj2syy*aj2rys
             t1 = aj2rx**2
             t6 = aj2sx**2
             aj2syxx = t1*aj2syrr+2*aj2rx*aj2sx*aj2syrs+t6*aj2syss+
     & aj2rxx*aj2syr+aj2sxx*aj2sys
             aj2syxy = aj2ry*aj2rx*aj2syrr+(aj2sy*aj2rx+aj2ry*aj2sx)*
     & aj2syrs+aj2sy*aj2sx*aj2syss+aj2rxy*aj2syr+aj2sxy*aj2sys
             t1 = aj2ry**2
             t6 = aj2sy**2
             aj2syyy = t1*aj2syrr+2*aj2ry*aj2sy*aj2syrs+t6*aj2syss+
     & aj2ryy*aj2syr+aj2syy*aj2sys
             t1 = aj2rx**2
             t7 = aj2sx**2
             aj2rxxxx = t1*aj2rx*aj2rxrrr+3*t1*aj2sx*aj2rxrrs+3*aj2rx*
     & t7*aj2rxrss+t7*aj2sx*aj2rxsss+3*aj2rx*aj2rxx*aj2rxrr+(3*aj2sxx*
     & aj2rx+3*aj2sx*aj2rxx)*aj2rxrs+3*aj2sxx*aj2sx*aj2rxss+aj2rxxx*
     & aj2rxr+aj2sxxx*aj2rxs
             t1 = aj2rx**2
             t10 = aj2sx**2
             aj2rxxxy = aj2ry*t1*aj2rxrrr+(aj2sy*t1+2*aj2ry*aj2sx*
     & aj2rx)*aj2rxrrs+(aj2ry*t10+2*aj2sy*aj2sx*aj2rx)*aj2rxrss+aj2sy*
     & t10*aj2rxsss+(2*aj2rxy*aj2rx+aj2ry*aj2rxx)*aj2rxrr+(aj2ry*
     & aj2sxx+2*aj2sx*aj2rxy+2*aj2sxy*aj2rx+aj2sy*aj2rxx)*aj2rxrs+(
     & aj2sy*aj2sxx+2*aj2sxy*aj2sx)*aj2rxss+aj2rxxy*aj2rxr+aj2sxxy*
     & aj2rxs
             t1 = aj2ry**2
             t4 = aj2sy*aj2ry
             t8 = aj2sy*aj2rx+aj2ry*aj2sx
             t16 = aj2sy**2
             aj2rxxyy = t1*aj2rx*aj2rxrrr+(t4*aj2rx+aj2ry*t8)*aj2rxrrs+
     & (t4*aj2sx+aj2sy*t8)*aj2rxrss+t16*aj2sx*aj2rxsss+(aj2ryy*aj2rx+
     & 2*aj2ry*aj2rxy)*aj2rxrr+(2*aj2ry*aj2sxy+2*aj2sy*aj2rxy+aj2ryy*
     & aj2sx+aj2syy*aj2rx)*aj2rxrs+(aj2syy*aj2sx+2*aj2sy*aj2sxy)*
     & aj2rxss+aj2rxyy*aj2rxr+aj2sxyy*aj2rxs
             t1 = aj2ry**2
             t7 = aj2sy**2
             aj2rxyyy = aj2ry*t1*aj2rxrrr+3*t1*aj2sy*aj2rxrrs+3*aj2ry*
     & t7*aj2rxrss+t7*aj2sy*aj2rxsss+3*aj2ry*aj2ryy*aj2rxrr+(3*aj2syy*
     & aj2ry+3*aj2sy*aj2ryy)*aj2rxrs+3*aj2syy*aj2sy*aj2rxss+aj2ryyy*
     & aj2rxr+aj2syyy*aj2rxs
             t1 = aj2rx**2
             t7 = aj2sx**2
             aj2sxxxx = t1*aj2rx*aj2sxrrr+3*t1*aj2sx*aj2sxrrs+3*aj2rx*
     & t7*aj2sxrss+t7*aj2sx*aj2sxsss+3*aj2rx*aj2rxx*aj2sxrr+(3*aj2sxx*
     & aj2rx+3*aj2sx*aj2rxx)*aj2sxrs+3*aj2sxx*aj2sx*aj2sxss+aj2rxxx*
     & aj2sxr+aj2sxxx*aj2sxs
             t1 = aj2rx**2
             t10 = aj2sx**2
             aj2sxxxy = aj2ry*t1*aj2sxrrr+(aj2sy*t1+2*aj2ry*aj2sx*
     & aj2rx)*aj2sxrrs+(aj2ry*t10+2*aj2sy*aj2sx*aj2rx)*aj2sxrss+aj2sy*
     & t10*aj2sxsss+(2*aj2rxy*aj2rx+aj2ry*aj2rxx)*aj2sxrr+(aj2ry*
     & aj2sxx+2*aj2sx*aj2rxy+2*aj2sxy*aj2rx+aj2sy*aj2rxx)*aj2sxrs+(
     & aj2sy*aj2sxx+2*aj2sxy*aj2sx)*aj2sxss+aj2rxxy*aj2sxr+aj2sxxy*
     & aj2sxs
             t1 = aj2ry**2
             t4 = aj2sy*aj2ry
             t8 = aj2sy*aj2rx+aj2ry*aj2sx
             t16 = aj2sy**2
             aj2sxxyy = t1*aj2rx*aj2sxrrr+(t4*aj2rx+aj2ry*t8)*aj2sxrrs+
     & (t4*aj2sx+aj2sy*t8)*aj2sxrss+t16*aj2sx*aj2sxsss+(aj2ryy*aj2rx+
     & 2*aj2ry*aj2rxy)*aj2sxrr+(2*aj2ry*aj2sxy+2*aj2sy*aj2rxy+aj2ryy*
     & aj2sx+aj2syy*aj2rx)*aj2sxrs+(aj2syy*aj2sx+2*aj2sy*aj2sxy)*
     & aj2sxss+aj2rxyy*aj2sxr+aj2sxyy*aj2sxs
             t1 = aj2ry**2
             t7 = aj2sy**2
             aj2sxyyy = aj2ry*t1*aj2sxrrr+3*t1*aj2sy*aj2sxrrs+3*aj2ry*
     & t7*aj2sxrss+t7*aj2sy*aj2sxsss+3*aj2ry*aj2ryy*aj2sxrr+(3*aj2syy*
     & aj2ry+3*aj2sy*aj2ryy)*aj2sxrs+3*aj2syy*aj2sy*aj2sxss+aj2ryyy*
     & aj2sxr+aj2syyy*aj2sxs
             t1 = aj2rx**2
             t7 = aj2sx**2
             aj2ryxxx = t1*aj2rx*aj2ryrrr+3*t1*aj2sx*aj2ryrrs+3*aj2rx*
     & t7*aj2ryrss+t7*aj2sx*aj2rysss+3*aj2rx*aj2rxx*aj2ryrr+(3*aj2sxx*
     & aj2rx+3*aj2sx*aj2rxx)*aj2ryrs+3*aj2sxx*aj2sx*aj2ryss+aj2rxxx*
     & aj2ryr+aj2sxxx*aj2rys
             t1 = aj2rx**2
             t10 = aj2sx**2
             aj2ryxxy = aj2ry*t1*aj2ryrrr+(aj2sy*t1+2*aj2ry*aj2sx*
     & aj2rx)*aj2ryrrs+(aj2ry*t10+2*aj2sy*aj2sx*aj2rx)*aj2ryrss+aj2sy*
     & t10*aj2rysss+(2*aj2rxy*aj2rx+aj2ry*aj2rxx)*aj2ryrr+(aj2ry*
     & aj2sxx+2*aj2sx*aj2rxy+2*aj2sxy*aj2rx+aj2sy*aj2rxx)*aj2ryrs+(
     & aj2sy*aj2sxx+2*aj2sxy*aj2sx)*aj2ryss+aj2rxxy*aj2ryr+aj2sxxy*
     & aj2rys
             t1 = aj2ry**2
             t4 = aj2sy*aj2ry
             t8 = aj2sy*aj2rx+aj2ry*aj2sx
             t16 = aj2sy**2
             aj2ryxyy = t1*aj2rx*aj2ryrrr+(t4*aj2rx+aj2ry*t8)*aj2ryrrs+
     & (t4*aj2sx+aj2sy*t8)*aj2ryrss+t16*aj2sx*aj2rysss+(aj2ryy*aj2rx+
     & 2*aj2ry*aj2rxy)*aj2ryrr+(2*aj2ry*aj2sxy+2*aj2sy*aj2rxy+aj2ryy*
     & aj2sx+aj2syy*aj2rx)*aj2ryrs+(aj2syy*aj2sx+2*aj2sy*aj2sxy)*
     & aj2ryss+aj2rxyy*aj2ryr+aj2sxyy*aj2rys
             t1 = aj2ry**2
             t7 = aj2sy**2
             aj2ryyyy = aj2ry*t1*aj2ryrrr+3*t1*aj2sy*aj2ryrrs+3*aj2ry*
     & t7*aj2ryrss+t7*aj2sy*aj2rysss+3*aj2ry*aj2ryy*aj2ryrr+(3*aj2syy*
     & aj2ry+3*aj2sy*aj2ryy)*aj2ryrs+3*aj2syy*aj2sy*aj2ryss+aj2ryyy*
     & aj2ryr+aj2syyy*aj2rys
             t1 = aj2rx**2
             t7 = aj2sx**2
             aj2syxxx = t1*aj2rx*aj2syrrr+3*t1*aj2sx*aj2syrrs+3*aj2rx*
     & t7*aj2syrss+t7*aj2sx*aj2sysss+3*aj2rx*aj2rxx*aj2syrr+(3*aj2sxx*
     & aj2rx+3*aj2sx*aj2rxx)*aj2syrs+3*aj2sxx*aj2sx*aj2syss+aj2rxxx*
     & aj2syr+aj2sxxx*aj2sys
             t1 = aj2rx**2
             t10 = aj2sx**2
             aj2syxxy = aj2ry*t1*aj2syrrr+(aj2sy*t1+2*aj2ry*aj2sx*
     & aj2rx)*aj2syrrs+(aj2ry*t10+2*aj2sy*aj2sx*aj2rx)*aj2syrss+aj2sy*
     & t10*aj2sysss+(2*aj2rxy*aj2rx+aj2ry*aj2rxx)*aj2syrr+(aj2ry*
     & aj2sxx+2*aj2sx*aj2rxy+2*aj2sxy*aj2rx+aj2sy*aj2rxx)*aj2syrs+(
     & aj2sy*aj2sxx+2*aj2sxy*aj2sx)*aj2syss+aj2rxxy*aj2syr+aj2sxxy*
     & aj2sys
             t1 = aj2ry**2
             t4 = aj2sy*aj2ry
             t8 = aj2sy*aj2rx+aj2ry*aj2sx
             t16 = aj2sy**2
             aj2syxyy = t1*aj2rx*aj2syrrr+(t4*aj2rx+aj2ry*t8)*aj2syrrs+
     & (t4*aj2sx+aj2sy*t8)*aj2syrss+t16*aj2sx*aj2sysss+(aj2ryy*aj2rx+
     & 2*aj2ry*aj2rxy)*aj2syrr+(2*aj2ry*aj2sxy+2*aj2sy*aj2rxy+aj2ryy*
     & aj2sx+aj2syy*aj2rx)*aj2syrs+(aj2syy*aj2sx+2*aj2sy*aj2sxy)*
     & aj2syss+aj2rxyy*aj2syr+aj2sxyy*aj2sys
             t1 = aj2ry**2
             t7 = aj2sy**2
             aj2syyyy = aj2ry*t1*aj2syrrr+3*t1*aj2sy*aj2syrrs+3*aj2ry*
     & t7*aj2syrss+t7*aj2sy*aj2sysss+3*aj2ry*aj2ryy*aj2syrr+(3*aj2syy*
     & aj2ry+3*aj2sy*aj2ryy)*aj2syrs+3*aj2syy*aj2sy*aj2syss+aj2ryyy*
     & aj2syr+aj2syyy*aj2sys
              uu2 = u2(j1,j2,j3,ex)
              uu2r = (-u2(j1-1,j2,j3,ex)+u2(j1+1,j2,j3,ex))/(2.*dr2(0))
              uu2s = (-u2(j1,j2-1,j3,ex)+u2(j1,j2+1,j3,ex))/(2.*dr2(1))
              uu2rr = (u2(j1-1,j2,j3,ex)-2.*u2(j1,j2,j3,ex)+u2(j1+1,j2,
     & j3,ex))/(dr2(0)**2)
              uu2rs = (-(-u2(j1-1,j2-1,j3,ex)+u2(j1-1,j2+1,j3,ex))/(2.*
     & dr2(1))+(-u2(j1+1,j2-1,j3,ex)+u2(j1+1,j2+1,j3,ex))/(2.*dr2(1)))
     & /(2.*dr2(0))
              uu2ss = (u2(j1,j2-1,j3,ex)-2.*u2(j1,j2,j3,ex)+u2(j1,j2+1,
     & j3,ex))/(dr2(1)**2)
              uu2rrr = (-u2(j1-2,j2,j3,ex)+2.*u2(j1-1,j2,j3,ex)-2.*u2(
     & j1+1,j2,j3,ex)+u2(j1+2,j2,j3,ex))/(2.*dr2(0)**3)
              uu2rrs = ((-u2(j1-1,j2-1,j3,ex)+u2(j1-1,j2+1,j3,ex))/(2.*
     & dr2(1))-2.*(-u2(j1,j2-1,j3,ex)+u2(j1,j2+1,j3,ex))/(2.*dr2(1))+(
     & -u2(j1+1,j2-1,j3,ex)+u2(j1+1,j2+1,j3,ex))/(2.*dr2(1)))/(dr2(0)*
     & *2)
              uu2rss = (-(u2(j1-1,j2-1,j3,ex)-2.*u2(j1-1,j2,j3,ex)+u2(
     & j1-1,j2+1,j3,ex))/(dr2(1)**2)+(u2(j1+1,j2-1,j3,ex)-2.*u2(j1+1,
     & j2,j3,ex)+u2(j1+1,j2+1,j3,ex))/(dr2(1)**2))/(2.*dr2(0))
              uu2sss = (-u2(j1,j2-2,j3,ex)+2.*u2(j1,j2-1,j3,ex)-2.*u2(
     & j1,j2+1,j3,ex)+u2(j1,j2+2,j3,ex))/(2.*dr2(1)**3)
              uu2rrrr = (u2(j1-2,j2,j3,ex)-4.*u2(j1-1,j2,j3,ex)+6.*u2(
     & j1,j2,j3,ex)-4.*u2(j1+1,j2,j3,ex)+u2(j1+2,j2,j3,ex))/(dr2(0)**
     & 4)
              uu2rrrs = (-(-u2(j1-2,j2-1,j3,ex)+u2(j1-2,j2+1,j3,ex))/(
     & 2.*dr2(1))+2.*(-u2(j1-1,j2-1,j3,ex)+u2(j1-1,j2+1,j3,ex))/(2.*
     & dr2(1))-2.*(-u2(j1+1,j2-1,j3,ex)+u2(j1+1,j2+1,j3,ex))/(2.*dr2(
     & 1))+(-u2(j1+2,j2-1,j3,ex)+u2(j1+2,j2+1,j3,ex))/(2.*dr2(1)))/(
     & 2.*dr2(0)**3)
              uu2rrss = ((u2(j1-1,j2-1,j3,ex)-2.*u2(j1-1,j2,j3,ex)+u2(
     & j1-1,j2+1,j3,ex))/(dr2(1)**2)-2.*(u2(j1,j2-1,j3,ex)-2.*u2(j1,
     & j2,j3,ex)+u2(j1,j2+1,j3,ex))/(dr2(1)**2)+(u2(j1+1,j2-1,j3,ex)-
     & 2.*u2(j1+1,j2,j3,ex)+u2(j1+1,j2+1,j3,ex))/(dr2(1)**2))/(dr2(0)*
     & *2)
              uu2rsss = (-(-u2(j1-1,j2-2,j3,ex)+2.*u2(j1-1,j2-1,j3,ex)-
     & 2.*u2(j1-1,j2+1,j3,ex)+u2(j1-1,j2+2,j3,ex))/(2.*dr2(1)**3)+(-
     & u2(j1+1,j2-2,j3,ex)+2.*u2(j1+1,j2-1,j3,ex)-2.*u2(j1+1,j2+1,j3,
     & ex)+u2(j1+1,j2+2,j3,ex))/(2.*dr2(1)**3))/(2.*dr2(0))
              uu2ssss = (u2(j1,j2-2,j3,ex)-4.*u2(j1,j2-1,j3,ex)+6.*u2(
     & j1,j2,j3,ex)-4.*u2(j1,j2+1,j3,ex)+u2(j1,j2+2,j3,ex))/(dr2(1)**
     & 4)
               t1 = aj2rx**2
               t7 = aj2sx**2
               u2xxx = t1*aj2rx*uu2rrr+3*t1*aj2sx*uu2rrs+3*aj2rx*t7*
     & uu2rss+t7*aj2sx*uu2sss+3*aj2rx*aj2rxx*uu2rr+(3*aj2sxx*aj2rx+3*
     & aj2sx*aj2rxx)*uu2rs+3*aj2sxx*aj2sx*uu2ss+aj2rxxx*uu2r+aj2sxxx*
     & uu2s
               t1 = aj2rx**2
               t10 = aj2sx**2
               u2xxy = aj2ry*t1*uu2rrr+(aj2sy*t1+2*aj2ry*aj2sx*aj2rx)*
     & uu2rrs+(aj2ry*t10+2*aj2sy*aj2sx*aj2rx)*uu2rss+aj2sy*t10*uu2sss+
     & (2*aj2rxy*aj2rx+aj2ry*aj2rxx)*uu2rr+(aj2ry*aj2sxx+2*aj2sx*
     & aj2rxy+2*aj2sxy*aj2rx+aj2sy*aj2rxx)*uu2rs+(aj2sy*aj2sxx+2*
     & aj2sxy*aj2sx)*uu2ss+aj2rxxy*uu2r+aj2sxxy*uu2s
               t1 = aj2ry**2
               t4 = aj2sy*aj2ry
               t8 = aj2sy*aj2rx+aj2ry*aj2sx
               t16 = aj2sy**2
               u2xyy = t1*aj2rx*uu2rrr+(t4*aj2rx+aj2ry*t8)*uu2rrs+(t4*
     & aj2sx+aj2sy*t8)*uu2rss+t16*aj2sx*uu2sss+(aj2ryy*aj2rx+2*aj2ry*
     & aj2rxy)*uu2rr+(2*aj2ry*aj2sxy+2*aj2sy*aj2rxy+aj2ryy*aj2sx+
     & aj2syy*aj2rx)*uu2rs+(aj2syy*aj2sx+2*aj2sy*aj2sxy)*uu2ss+
     & aj2rxyy*uu2r+aj2sxyy*uu2s
               t1 = aj2ry**2
               t7 = aj2sy**2
               u2yyy = aj2ry*t1*uu2rrr+3*t1*aj2sy*uu2rrs+3*aj2ry*t7*
     & uu2rss+t7*aj2sy*uu2sss+3*aj2ry*aj2ryy*uu2rr+(3*aj2syy*aj2ry+3*
     & aj2sy*aj2ryy)*uu2rs+3*aj2syy*aj2sy*uu2ss+aj2ryyy*uu2r+aj2syyy*
     & uu2s
               t1 = aj2rx**2
               t2 = t1**2
               t8 = aj2sx**2
               t16 = t8**2
               t25 = aj2sxx*aj2rx
               t27 = t25+aj2sx*aj2rxx
               t28 = 3*t27
               t30 = 2*t27
               t46 = aj2rxx**2
               t60 = aj2sxx**2
               u2xxxx = t2*uu2rrrr+4*t1*aj2rx*aj2sx*uu2rrrs+6*t1*t8*
     & uu2rrss+4*aj2rx*t8*aj2sx*uu2rsss+t16*uu2ssss+6*t1*aj2rxx*
     & uu2rrr+(7*aj2sx*aj2rx*aj2rxx+aj2sxx*t1+aj2rx*t28+aj2rx*t30)*
     & uu2rrs+(aj2sx*t28+7*t25*aj2sx+aj2rxx*t8+aj2sx*t30)*uu2rss+6*t8*
     & aj2sxx*uu2sss+(4*aj2rx*aj2rxxx+3*t46)*uu2rr+(4*aj2sxxx*aj2rx+4*
     & aj2sx*aj2rxxx+6*aj2sxx*aj2rxx)*uu2rs+(4*aj2sxxx*aj2sx+3*t60)*
     & uu2ss+aj2rxxxx*uu2r+aj2sxxxx*uu2s
               t1 = aj2ry**2
               t2 = aj2rx**2
               t5 = aj2sy*aj2ry
               t11 = aj2sy*t2+2*aj2ry*aj2sx*aj2rx
               t16 = aj2sx**2
               t21 = aj2ry*t16+2*aj2sy*aj2sx*aj2rx
               t29 = aj2sy**2
               t38 = 2*aj2rxy*aj2rx+aj2ry*aj2rxx
               t52 = aj2sx*aj2rxy
               t54 = aj2sxy*aj2rx
               t57 = aj2ry*aj2sxx+2*t52+2*t54+aj2sy*aj2rxx
               t60 = 2*t52+2*t54
               t68 = aj2sy*aj2sxx+2*aj2sxy*aj2sx
               t92 = aj2rxy**2
               t110 = aj2sxy**2
               u2xxyy = t1*t2*uu2rrrr+(t5*t2+aj2ry*t11)*uu2rrrs+(aj2sy*
     & t11+aj2ry*t21)*uu2rrss+(aj2sy*t21+t5*t16)*uu2rsss+t29*t16*
     & uu2ssss+(2*aj2ry*aj2rxy*aj2rx+aj2ry*t38+aj2ryy*t2)*uu2rrr+(
     & aj2sy*t38+2*aj2sy*aj2rxy*aj2rx+2*aj2ryy*aj2sx*aj2rx+aj2syy*t2+
     & aj2ry*t57+aj2ry*t60)*uu2rrs+(aj2sy*t57+aj2ry*t68+aj2ryy*t16+2*
     & aj2ry*aj2sxy*aj2sx+2*aj2syy*aj2sx*aj2rx+aj2sy*t60)*uu2rss+(2*
     & aj2sy*aj2sxy*aj2sx+aj2sy*t68+aj2syy*t16)*uu2sss+(2*aj2rx*
     & aj2rxyy+aj2ryy*aj2rxx+2*aj2ry*aj2rxxy+2*t92)*uu2rr+(4*aj2sxy*
     & aj2rxy+2*aj2ry*aj2sxxy+aj2ryy*aj2sxx+2*aj2sy*aj2rxxy+2*aj2sxyy*
     & aj2rx+aj2syy*aj2rxx+2*aj2sx*aj2rxyy)*uu2rs+(2*t110+2*aj2sy*
     & aj2sxxy+aj2syy*aj2sxx+2*aj2sx*aj2sxyy)*uu2ss+aj2rxxyy*uu2r+
     & aj2sxxyy*uu2s
               t1 = aj2ry**2
               t2 = t1**2
               t8 = aj2sy**2
               t16 = t8**2
               t25 = aj2syy*aj2ry
               t27 = t25+aj2sy*aj2ryy
               t28 = 3*t27
               t30 = 2*t27
               t46 = aj2ryy**2
               t60 = aj2syy**2
               u2yyyy = t2*uu2rrrr+4*t1*aj2ry*aj2sy*uu2rrrs+6*t1*t8*
     & uu2rrss+4*aj2ry*t8*aj2sy*uu2rsss+t16*uu2ssss+6*t1*aj2ryy*
     & uu2rrr+(7*aj2sy*aj2ry*aj2ryy+aj2syy*t1+aj2ry*t28+aj2ry*t30)*
     & uu2rrs+(aj2sy*t28+7*t25*aj2sy+aj2ryy*t8+aj2sy*t30)*uu2rss+6*t8*
     & aj2syy*uu2sss+(4*aj2ry*aj2ryyy+3*t46)*uu2rr+(4*aj2syyy*aj2ry+4*
     & aj2sy*aj2ryyy+6*aj2syy*aj2ryy)*uu2rs+(4*aj2syyy*aj2sy+3*t60)*
     & uu2ss+aj2ryyyy*uu2r+aj2syyyy*uu2s
             u2LapSq = u2xxxx +2.* u2xxyy + u2yyyy
              vv2 = u2(j1,j2,j3,ey)
              vv2r = (-u2(j1-1,j2,j3,ey)+u2(j1+1,j2,j3,ey))/(2.*dr2(0))
              vv2s = (-u2(j1,j2-1,j3,ey)+u2(j1,j2+1,j3,ey))/(2.*dr2(1))
              vv2rr = (u2(j1-1,j2,j3,ey)-2.*u2(j1,j2,j3,ey)+u2(j1+1,j2,
     & j3,ey))/(dr2(0)**2)
              vv2rs = (-(-u2(j1-1,j2-1,j3,ey)+u2(j1-1,j2+1,j3,ey))/(2.*
     & dr2(1))+(-u2(j1+1,j2-1,j3,ey)+u2(j1+1,j2+1,j3,ey))/(2.*dr2(1)))
     & /(2.*dr2(0))
              vv2ss = (u2(j1,j2-1,j3,ey)-2.*u2(j1,j2,j3,ey)+u2(j1,j2+1,
     & j3,ey))/(dr2(1)**2)
              vv2rrr = (-u2(j1-2,j2,j3,ey)+2.*u2(j1-1,j2,j3,ey)-2.*u2(
     & j1+1,j2,j3,ey)+u2(j1+2,j2,j3,ey))/(2.*dr2(0)**3)
              vv2rrs = ((-u2(j1-1,j2-1,j3,ey)+u2(j1-1,j2+1,j3,ey))/(2.*
     & dr2(1))-2.*(-u2(j1,j2-1,j3,ey)+u2(j1,j2+1,j3,ey))/(2.*dr2(1))+(
     & -u2(j1+1,j2-1,j3,ey)+u2(j1+1,j2+1,j3,ey))/(2.*dr2(1)))/(dr2(0)*
     & *2)
              vv2rss = (-(u2(j1-1,j2-1,j3,ey)-2.*u2(j1-1,j2,j3,ey)+u2(
     & j1-1,j2+1,j3,ey))/(dr2(1)**2)+(u2(j1+1,j2-1,j3,ey)-2.*u2(j1+1,
     & j2,j3,ey)+u2(j1+1,j2+1,j3,ey))/(dr2(1)**2))/(2.*dr2(0))
              vv2sss = (-u2(j1,j2-2,j3,ey)+2.*u2(j1,j2-1,j3,ey)-2.*u2(
     & j1,j2+1,j3,ey)+u2(j1,j2+2,j3,ey))/(2.*dr2(1)**3)
              vv2rrrr = (u2(j1-2,j2,j3,ey)-4.*u2(j1-1,j2,j3,ey)+6.*u2(
     & j1,j2,j3,ey)-4.*u2(j1+1,j2,j3,ey)+u2(j1+2,j2,j3,ey))/(dr2(0)**
     & 4)
              vv2rrrs = (-(-u2(j1-2,j2-1,j3,ey)+u2(j1-2,j2+1,j3,ey))/(
     & 2.*dr2(1))+2.*(-u2(j1-1,j2-1,j3,ey)+u2(j1-1,j2+1,j3,ey))/(2.*
     & dr2(1))-2.*(-u2(j1+1,j2-1,j3,ey)+u2(j1+1,j2+1,j3,ey))/(2.*dr2(
     & 1))+(-u2(j1+2,j2-1,j3,ey)+u2(j1+2,j2+1,j3,ey))/(2.*dr2(1)))/(
     & 2.*dr2(0)**3)
              vv2rrss = ((u2(j1-1,j2-1,j3,ey)-2.*u2(j1-1,j2,j3,ey)+u2(
     & j1-1,j2+1,j3,ey))/(dr2(1)**2)-2.*(u2(j1,j2-1,j3,ey)-2.*u2(j1,
     & j2,j3,ey)+u2(j1,j2+1,j3,ey))/(dr2(1)**2)+(u2(j1+1,j2-1,j3,ey)-
     & 2.*u2(j1+1,j2,j3,ey)+u2(j1+1,j2+1,j3,ey))/(dr2(1)**2))/(dr2(0)*
     & *2)
              vv2rsss = (-(-u2(j1-1,j2-2,j3,ey)+2.*u2(j1-1,j2-1,j3,ey)-
     & 2.*u2(j1-1,j2+1,j3,ey)+u2(j1-1,j2+2,j3,ey))/(2.*dr2(1)**3)+(-
     & u2(j1+1,j2-2,j3,ey)+2.*u2(j1+1,j2-1,j3,ey)-2.*u2(j1+1,j2+1,j3,
     & ey)+u2(j1+1,j2+2,j3,ey))/(2.*dr2(1)**3))/(2.*dr2(0))
              vv2ssss = (u2(j1,j2-2,j3,ey)-4.*u2(j1,j2-1,j3,ey)+6.*u2(
     & j1,j2,j3,ey)-4.*u2(j1,j2+1,j3,ey)+u2(j1,j2+2,j3,ey))/(dr2(1)**
     & 4)
               t1 = aj2rx**2
               t7 = aj2sx**2
               v2xxx = t1*aj2rx*vv2rrr+3*t1*aj2sx*vv2rrs+3*aj2rx*t7*
     & vv2rss+t7*aj2sx*vv2sss+3*aj2rx*aj2rxx*vv2rr+(3*aj2sxx*aj2rx+3*
     & aj2sx*aj2rxx)*vv2rs+3*aj2sxx*aj2sx*vv2ss+aj2rxxx*vv2r+aj2sxxx*
     & vv2s
               t1 = aj2rx**2
               t10 = aj2sx**2
               v2xxy = aj2ry*t1*vv2rrr+(aj2sy*t1+2*aj2ry*aj2sx*aj2rx)*
     & vv2rrs+(aj2ry*t10+2*aj2sy*aj2sx*aj2rx)*vv2rss+aj2sy*t10*vv2sss+
     & (2*aj2rxy*aj2rx+aj2ry*aj2rxx)*vv2rr+(aj2ry*aj2sxx+2*aj2sx*
     & aj2rxy+2*aj2sxy*aj2rx+aj2sy*aj2rxx)*vv2rs+(aj2sy*aj2sxx+2*
     & aj2sxy*aj2sx)*vv2ss+aj2rxxy*vv2r+aj2sxxy*vv2s
               t1 = aj2ry**2
               t4 = aj2sy*aj2ry
               t8 = aj2sy*aj2rx+aj2ry*aj2sx
               t16 = aj2sy**2
               v2xyy = t1*aj2rx*vv2rrr+(t4*aj2rx+aj2ry*t8)*vv2rrs+(t4*
     & aj2sx+aj2sy*t8)*vv2rss+t16*aj2sx*vv2sss+(aj2ryy*aj2rx+2*aj2ry*
     & aj2rxy)*vv2rr+(2*aj2ry*aj2sxy+2*aj2sy*aj2rxy+aj2ryy*aj2sx+
     & aj2syy*aj2rx)*vv2rs+(aj2syy*aj2sx+2*aj2sy*aj2sxy)*vv2ss+
     & aj2rxyy*vv2r+aj2sxyy*vv2s
               t1 = aj2ry**2
               t7 = aj2sy**2
               v2yyy = aj2ry*t1*vv2rrr+3*t1*aj2sy*vv2rrs+3*aj2ry*t7*
     & vv2rss+t7*aj2sy*vv2sss+3*aj2ry*aj2ryy*vv2rr+(3*aj2syy*aj2ry+3*
     & aj2sy*aj2ryy)*vv2rs+3*aj2syy*aj2sy*vv2ss+aj2ryyy*vv2r+aj2syyy*
     & vv2s
               t1 = aj2rx**2
               t2 = t1**2
               t8 = aj2sx**2
               t16 = t8**2
               t25 = aj2sxx*aj2rx
               t27 = t25+aj2sx*aj2rxx
               t28 = 3*t27
               t30 = 2*t27
               t46 = aj2rxx**2
               t60 = aj2sxx**2
               v2xxxx = t2*vv2rrrr+4*t1*aj2rx*aj2sx*vv2rrrs+6*t1*t8*
     & vv2rrss+4*aj2rx*t8*aj2sx*vv2rsss+t16*vv2ssss+6*t1*aj2rxx*
     & vv2rrr+(7*aj2sx*aj2rx*aj2rxx+aj2sxx*t1+aj2rx*t28+aj2rx*t30)*
     & vv2rrs+(aj2sx*t28+7*t25*aj2sx+aj2rxx*t8+aj2sx*t30)*vv2rss+6*t8*
     & aj2sxx*vv2sss+(4*aj2rx*aj2rxxx+3*t46)*vv2rr+(4*aj2sxxx*aj2rx+4*
     & aj2sx*aj2rxxx+6*aj2sxx*aj2rxx)*vv2rs+(4*aj2sxxx*aj2sx+3*t60)*
     & vv2ss+aj2rxxxx*vv2r+aj2sxxxx*vv2s
               t1 = aj2ry**2
               t2 = aj2rx**2
               t5 = aj2sy*aj2ry
               t11 = aj2sy*t2+2*aj2ry*aj2sx*aj2rx
               t16 = aj2sx**2
               t21 = aj2ry*t16+2*aj2sy*aj2sx*aj2rx
               t29 = aj2sy**2
               t38 = 2*aj2rxy*aj2rx+aj2ry*aj2rxx
               t52 = aj2sx*aj2rxy
               t54 = aj2sxy*aj2rx
               t57 = aj2ry*aj2sxx+2*t52+2*t54+aj2sy*aj2rxx
               t60 = 2*t52+2*t54
               t68 = aj2sy*aj2sxx+2*aj2sxy*aj2sx
               t92 = aj2rxy**2
               t110 = aj2sxy**2
               v2xxyy = t1*t2*vv2rrrr+(t5*t2+aj2ry*t11)*vv2rrrs+(aj2sy*
     & t11+aj2ry*t21)*vv2rrss+(aj2sy*t21+t5*t16)*vv2rsss+t29*t16*
     & vv2ssss+(2*aj2ry*aj2rxy*aj2rx+aj2ry*t38+aj2ryy*t2)*vv2rrr+(
     & aj2sy*t38+2*aj2sy*aj2rxy*aj2rx+2*aj2ryy*aj2sx*aj2rx+aj2syy*t2+
     & aj2ry*t57+aj2ry*t60)*vv2rrs+(aj2sy*t57+aj2ry*t68+aj2ryy*t16+2*
     & aj2ry*aj2sxy*aj2sx+2*aj2syy*aj2sx*aj2rx+aj2sy*t60)*vv2rss+(2*
     & aj2sy*aj2sxy*aj2sx+aj2sy*t68+aj2syy*t16)*vv2sss+(2*aj2rx*
     & aj2rxyy+aj2ryy*aj2rxx+2*aj2ry*aj2rxxy+2*t92)*vv2rr+(4*aj2sxy*
     & aj2rxy+2*aj2ry*aj2sxxy+aj2ryy*aj2sxx+2*aj2sy*aj2rxxy+2*aj2sxyy*
     & aj2rx+aj2syy*aj2rxx+2*aj2sx*aj2rxyy)*vv2rs+(2*t110+2*aj2sy*
     & aj2sxxy+aj2syy*aj2sxx+2*aj2sx*aj2sxyy)*vv2ss+aj2rxxyy*vv2r+
     & aj2sxxyy*vv2s
               t1 = aj2ry**2
               t2 = t1**2
               t8 = aj2sy**2
               t16 = t8**2
               t25 = aj2syy*aj2ry
               t27 = t25+aj2sy*aj2ryy
               t28 = 3*t27
               t30 = 2*t27
               t46 = aj2ryy**2
               t60 = aj2syy**2
               v2yyyy = t2*vv2rrrr+4*t1*aj2ry*aj2sy*vv2rrrs+6*t1*t8*
     & vv2rrss+4*aj2ry*t8*aj2sy*vv2rsss+t16*vv2ssss+6*t1*aj2ryy*
     & vv2rrr+(7*aj2sy*aj2ry*aj2ryy+aj2syy*t1+aj2ry*t28+aj2ry*t30)*
     & vv2rrs+(aj2sy*t28+7*t25*aj2sy+aj2ryy*t8+aj2sy*t30)*vv2rss+6*t8*
     & aj2syy*vv2sss+(4*aj2ry*aj2ryyy+3*t46)*vv2rr+(4*aj2syyy*aj2ry+4*
     & aj2sy*aj2ryyy+6*aj2syy*aj2ryy)*vv2rs+(4*aj2syyy*aj2sy+3*t60)*
     & vv2ss+aj2ryyyy*vv2r+aj2syyyy*vv2s
             v2LapSq = v2xxxx +2.* v2xxyy + v2yyyy
            ! These derivatives are computed to 4th-order accuracy
            ! NOTE: the jacobian derivatives can be computed once for all components
             ! this next call will define the jacobian and its derivatives (parameteric and spatial)
             aj1rx = rsxy1(i1,i2,i3,0,0)
             aj1rxr = (rsxy1(i1-2,i2,i3,0,0)-8.*rsxy1(i1-1,i2,i3,0,0)+
     & 8.*rsxy1(i1+1,i2,i3,0,0)-rsxy1(i1+2,i2,i3,0,0))/(12.*dr1(0))
             aj1rxs = (rsxy1(i1,i2-2,i3,0,0)-8.*rsxy1(i1,i2-1,i3,0,0)+
     & 8.*rsxy1(i1,i2+1,i3,0,0)-rsxy1(i1,i2+2,i3,0,0))/(12.*dr1(1))
             aj1sx = rsxy1(i1,i2,i3,1,0)
             aj1sxr = (rsxy1(i1-2,i2,i3,1,0)-8.*rsxy1(i1-1,i2,i3,1,0)+
     & 8.*rsxy1(i1+1,i2,i3,1,0)-rsxy1(i1+2,i2,i3,1,0))/(12.*dr1(0))
             aj1sxs = (rsxy1(i1,i2-2,i3,1,0)-8.*rsxy1(i1,i2-1,i3,1,0)+
     & 8.*rsxy1(i1,i2+1,i3,1,0)-rsxy1(i1,i2+2,i3,1,0))/(12.*dr1(1))
             aj1ry = rsxy1(i1,i2,i3,0,1)
             aj1ryr = (rsxy1(i1-2,i2,i3,0,1)-8.*rsxy1(i1-1,i2,i3,0,1)+
     & 8.*rsxy1(i1+1,i2,i3,0,1)-rsxy1(i1+2,i2,i3,0,1))/(12.*dr1(0))
             aj1rys = (rsxy1(i1,i2-2,i3,0,1)-8.*rsxy1(i1,i2-1,i3,0,1)+
     & 8.*rsxy1(i1,i2+1,i3,0,1)-rsxy1(i1,i2+2,i3,0,1))/(12.*dr1(1))
             aj1sy = rsxy1(i1,i2,i3,1,1)
             aj1syr = (rsxy1(i1-2,i2,i3,1,1)-8.*rsxy1(i1-1,i2,i3,1,1)+
     & 8.*rsxy1(i1+1,i2,i3,1,1)-rsxy1(i1+2,i2,i3,1,1))/(12.*dr1(0))
             aj1sys = (rsxy1(i1,i2-2,i3,1,1)-8.*rsxy1(i1,i2-1,i3,1,1)+
     & 8.*rsxy1(i1,i2+1,i3,1,1)-rsxy1(i1,i2+2,i3,1,1))/(12.*dr1(1))
             aj1rxx = aj1rx*aj1rxr+aj1sx*aj1rxs
             aj1rxy = aj1ry*aj1rxr+aj1sy*aj1rxs
             aj1sxx = aj1rx*aj1sxr+aj1sx*aj1sxs
             aj1sxy = aj1ry*aj1sxr+aj1sy*aj1sxs
             aj1ryx = aj1rx*aj1ryr+aj1sx*aj1rys
             aj1ryy = aj1ry*aj1ryr+aj1sy*aj1rys
             aj1syx = aj1rx*aj1syr+aj1sx*aj1sys
             aj1syy = aj1ry*aj1syr+aj1sy*aj1sys
              uu1 = u1(i1,i2,i3,ex)
              uu1r = (u1(i1-2,i2,i3,ex)-8.*u1(i1-1,i2,i3,ex)+8.*u1(i1+
     & 1,i2,i3,ex)-u1(i1+2,i2,i3,ex))/(12.*dr1(0))
              uu1s = (u1(i1,i2-2,i3,ex)-8.*u1(i1,i2-1,i3,ex)+8.*u1(i1,
     & i2+1,i3,ex)-u1(i1,i2+2,i3,ex))/(12.*dr1(1))
              uu1rr = (-u1(i1-2,i2,i3,ex)+16.*u1(i1-1,i2,i3,ex)-30.*u1(
     & i1,i2,i3,ex)+16.*u1(i1+1,i2,i3,ex)-u1(i1+2,i2,i3,ex))/(12.*dr1(
     & 0)**2)
              uu1rs = ((u1(i1-2,i2-2,i3,ex)-8.*u1(i1-2,i2-1,i3,ex)+8.*
     & u1(i1-2,i2+1,i3,ex)-u1(i1-2,i2+2,i3,ex))/(12.*dr1(1))-8.*(u1(
     & i1-1,i2-2,i3,ex)-8.*u1(i1-1,i2-1,i3,ex)+8.*u1(i1-1,i2+1,i3,ex)-
     & u1(i1-1,i2+2,i3,ex))/(12.*dr1(1))+8.*(u1(i1+1,i2-2,i3,ex)-8.*
     & u1(i1+1,i2-1,i3,ex)+8.*u1(i1+1,i2+1,i3,ex)-u1(i1+1,i2+2,i3,ex))
     & /(12.*dr1(1))-(u1(i1+2,i2-2,i3,ex)-8.*u1(i1+2,i2-1,i3,ex)+8.*
     & u1(i1+2,i2+1,i3,ex)-u1(i1+2,i2+2,i3,ex))/(12.*dr1(1)))/(12.*
     & dr1(0))
              uu1ss = (-u1(i1,i2-2,i3,ex)+16.*u1(i1,i2-1,i3,ex)-30.*u1(
     & i1,i2,i3,ex)+16.*u1(i1,i2+1,i3,ex)-u1(i1,i2+2,i3,ex))/(12.*dr1(
     & 1)**2)
               u1x = aj1rx*uu1r+aj1sx*uu1s
               u1y = aj1ry*uu1r+aj1sy*uu1s
               t1 = aj1rx**2
               t6 = aj1sx**2
               u1xx = t1*uu1rr+2*aj1rx*aj1sx*uu1rs+t6*uu1ss+aj1rxx*
     & uu1r+aj1sxx*uu1s
               t1 = aj1ry**2
               t6 = aj1sy**2
               u1yy = t1*uu1rr+2*aj1ry*aj1sy*uu1rs+t6*uu1ss+aj1ryy*
     & uu1r+aj1syy*uu1s
             u1Lap = u1xx+ u1yy
              vv1 = u1(i1,i2,i3,ey)
              vv1r = (u1(i1-2,i2,i3,ey)-8.*u1(i1-1,i2,i3,ey)+8.*u1(i1+
     & 1,i2,i3,ey)-u1(i1+2,i2,i3,ey))/(12.*dr1(0))
              vv1s = (u1(i1,i2-2,i3,ey)-8.*u1(i1,i2-1,i3,ey)+8.*u1(i1,
     & i2+1,i3,ey)-u1(i1,i2+2,i3,ey))/(12.*dr1(1))
              vv1rr = (-u1(i1-2,i2,i3,ey)+16.*u1(i1-1,i2,i3,ey)-30.*u1(
     & i1,i2,i3,ey)+16.*u1(i1+1,i2,i3,ey)-u1(i1+2,i2,i3,ey))/(12.*dr1(
     & 0)**2)
              vv1rs = ((u1(i1-2,i2-2,i3,ey)-8.*u1(i1-2,i2-1,i3,ey)+8.*
     & u1(i1-2,i2+1,i3,ey)-u1(i1-2,i2+2,i3,ey))/(12.*dr1(1))-8.*(u1(
     & i1-1,i2-2,i3,ey)-8.*u1(i1-1,i2-1,i3,ey)+8.*u1(i1-1,i2+1,i3,ey)-
     & u1(i1-1,i2+2,i3,ey))/(12.*dr1(1))+8.*(u1(i1+1,i2-2,i3,ey)-8.*
     & u1(i1+1,i2-1,i3,ey)+8.*u1(i1+1,i2+1,i3,ey)-u1(i1+1,i2+2,i3,ey))
     & /(12.*dr1(1))-(u1(i1+2,i2-2,i3,ey)-8.*u1(i1+2,i2-1,i3,ey)+8.*
     & u1(i1+2,i2+1,i3,ey)-u1(i1+2,i2+2,i3,ey))/(12.*dr1(1)))/(12.*
     & dr1(0))
              vv1ss = (-u1(i1,i2-2,i3,ey)+16.*u1(i1,i2-1,i3,ey)-30.*u1(
     & i1,i2,i3,ey)+16.*u1(i1,i2+1,i3,ey)-u1(i1,i2+2,i3,ey))/(12.*dr1(
     & 1)**2)
               v1x = aj1rx*vv1r+aj1sx*vv1s
               v1y = aj1ry*vv1r+aj1sy*vv1s
               t1 = aj1rx**2
               t6 = aj1sx**2
               v1xx = t1*vv1rr+2*aj1rx*aj1sx*vv1rs+t6*vv1ss+aj1rxx*
     & vv1r+aj1sxx*vv1s
               t1 = aj1ry**2
               t6 = aj1sy**2
               v1yy = t1*vv1rr+2*aj1ry*aj1sy*vv1rs+t6*vv1ss+aj1ryy*
     & vv1r+aj1syy*vv1s
             v1Lap = v1xx+ v1yy
            ! NOTE: the jacobian derivatives can be computed once for all components
             ! this next call will define the jacobian and its derivatives (parameteric and spatial)
             aj2rx = rsxy2(j1,j2,j3,0,0)
             aj2rxr = (rsxy2(j1-2,j2,j3,0,0)-8.*rsxy2(j1-1,j2,j3,0,0)+
     & 8.*rsxy2(j1+1,j2,j3,0,0)-rsxy2(j1+2,j2,j3,0,0))/(12.*dr2(0))
             aj2rxs = (rsxy2(j1,j2-2,j3,0,0)-8.*rsxy2(j1,j2-1,j3,0,0)+
     & 8.*rsxy2(j1,j2+1,j3,0,0)-rsxy2(j1,j2+2,j3,0,0))/(12.*dr2(1))
             aj2sx = rsxy2(j1,j2,j3,1,0)
             aj2sxr = (rsxy2(j1-2,j2,j3,1,0)-8.*rsxy2(j1-1,j2,j3,1,0)+
     & 8.*rsxy2(j1+1,j2,j3,1,0)-rsxy2(j1+2,j2,j3,1,0))/(12.*dr2(0))
             aj2sxs = (rsxy2(j1,j2-2,j3,1,0)-8.*rsxy2(j1,j2-1,j3,1,0)+
     & 8.*rsxy2(j1,j2+1,j3,1,0)-rsxy2(j1,j2+2,j3,1,0))/(12.*dr2(1))
             aj2ry = rsxy2(j1,j2,j3,0,1)
             aj2ryr = (rsxy2(j1-2,j2,j3,0,1)-8.*rsxy2(j1-1,j2,j3,0,1)+
     & 8.*rsxy2(j1+1,j2,j3,0,1)-rsxy2(j1+2,j2,j3,0,1))/(12.*dr2(0))
             aj2rys = (rsxy2(j1,j2-2,j3,0,1)-8.*rsxy2(j1,j2-1,j3,0,1)+
     & 8.*rsxy2(j1,j2+1,j3,0,1)-rsxy2(j1,j2+2,j3,0,1))/(12.*dr2(1))
             aj2sy = rsxy2(j1,j2,j3,1,1)
             aj2syr = (rsxy2(j1-2,j2,j3,1,1)-8.*rsxy2(j1-1,j2,j3,1,1)+
     & 8.*rsxy2(j1+1,j2,j3,1,1)-rsxy2(j1+2,j2,j3,1,1))/(12.*dr2(0))
             aj2sys = (rsxy2(j1,j2-2,j3,1,1)-8.*rsxy2(j1,j2-1,j3,1,1)+
     & 8.*rsxy2(j1,j2+1,j3,1,1)-rsxy2(j1,j2+2,j3,1,1))/(12.*dr2(1))
             aj2rxx = aj2rx*aj2rxr+aj2sx*aj2rxs
             aj2rxy = aj2ry*aj2rxr+aj2sy*aj2rxs
             aj2sxx = aj2rx*aj2sxr+aj2sx*aj2sxs
             aj2sxy = aj2ry*aj2sxr+aj2sy*aj2sxs
             aj2ryx = aj2rx*aj2ryr+aj2sx*aj2rys
             aj2ryy = aj2ry*aj2ryr+aj2sy*aj2rys
             aj2syx = aj2rx*aj2syr+aj2sx*aj2sys
             aj2syy = aj2ry*aj2syr+aj2sy*aj2sys
              uu2 = u2(j1,j2,j3,ex)
              uu2r = (u2(j1-2,j2,j3,ex)-8.*u2(j1-1,j2,j3,ex)+8.*u2(j1+
     & 1,j2,j3,ex)-u2(j1+2,j2,j3,ex))/(12.*dr2(0))
              uu2s = (u2(j1,j2-2,j3,ex)-8.*u2(j1,j2-1,j3,ex)+8.*u2(j1,
     & j2+1,j3,ex)-u2(j1,j2+2,j3,ex))/(12.*dr2(1))
              uu2rr = (-u2(j1-2,j2,j3,ex)+16.*u2(j1-1,j2,j3,ex)-30.*u2(
     & j1,j2,j3,ex)+16.*u2(j1+1,j2,j3,ex)-u2(j1+2,j2,j3,ex))/(12.*dr2(
     & 0)**2)
              uu2rs = ((u2(j1-2,j2-2,j3,ex)-8.*u2(j1-2,j2-1,j3,ex)+8.*
     & u2(j1-2,j2+1,j3,ex)-u2(j1-2,j2+2,j3,ex))/(12.*dr2(1))-8.*(u2(
     & j1-1,j2-2,j3,ex)-8.*u2(j1-1,j2-1,j3,ex)+8.*u2(j1-1,j2+1,j3,ex)-
     & u2(j1-1,j2+2,j3,ex))/(12.*dr2(1))+8.*(u2(j1+1,j2-2,j3,ex)-8.*
     & u2(j1+1,j2-1,j3,ex)+8.*u2(j1+1,j2+1,j3,ex)-u2(j1+1,j2+2,j3,ex))
     & /(12.*dr2(1))-(u2(j1+2,j2-2,j3,ex)-8.*u2(j1+2,j2-1,j3,ex)+8.*
     & u2(j1+2,j2+1,j3,ex)-u2(j1+2,j2+2,j3,ex))/(12.*dr2(1)))/(12.*
     & dr2(0))
              uu2ss = (-u2(j1,j2-2,j3,ex)+16.*u2(j1,j2-1,j3,ex)-30.*u2(
     & j1,j2,j3,ex)+16.*u2(j1,j2+1,j3,ex)-u2(j1,j2+2,j3,ex))/(12.*dr2(
     & 1)**2)
               u2x = aj2rx*uu2r+aj2sx*uu2s
               u2y = aj2ry*uu2r+aj2sy*uu2s
               t1 = aj2rx**2
               t6 = aj2sx**2
               u2xx = t1*uu2rr+2*aj2rx*aj2sx*uu2rs+t6*uu2ss+aj2rxx*
     & uu2r+aj2sxx*uu2s
               t1 = aj2ry**2
               t6 = aj2sy**2
               u2yy = t1*uu2rr+2*aj2ry*aj2sy*uu2rs+t6*uu2ss+aj2ryy*
     & uu2r+aj2syy*uu2s
             u2Lap = u2xx+ u2yy
              vv2 = u2(j1,j2,j3,ey)
              vv2r = (u2(j1-2,j2,j3,ey)-8.*u2(j1-1,j2,j3,ey)+8.*u2(j1+
     & 1,j2,j3,ey)-u2(j1+2,j2,j3,ey))/(12.*dr2(0))
              vv2s = (u2(j1,j2-2,j3,ey)-8.*u2(j1,j2-1,j3,ey)+8.*u2(j1,
     & j2+1,j3,ey)-u2(j1,j2+2,j3,ey))/(12.*dr2(1))
              vv2rr = (-u2(j1-2,j2,j3,ey)+16.*u2(j1-1,j2,j3,ey)-30.*u2(
     & j1,j2,j3,ey)+16.*u2(j1+1,j2,j3,ey)-u2(j1+2,j2,j3,ey))/(12.*dr2(
     & 0)**2)
              vv2rs = ((u2(j1-2,j2-2,j3,ey)-8.*u2(j1-2,j2-1,j3,ey)+8.*
     & u2(j1-2,j2+1,j3,ey)-u2(j1-2,j2+2,j3,ey))/(12.*dr2(1))-8.*(u2(
     & j1-1,j2-2,j3,ey)-8.*u2(j1-1,j2-1,j3,ey)+8.*u2(j1-1,j2+1,j3,ey)-
     & u2(j1-1,j2+2,j3,ey))/(12.*dr2(1))+8.*(u2(j1+1,j2-2,j3,ey)-8.*
     & u2(j1+1,j2-1,j3,ey)+8.*u2(j1+1,j2+1,j3,ey)-u2(j1+1,j2+2,j3,ey))
     & /(12.*dr2(1))-(u2(j1+2,j2-2,j3,ey)-8.*u2(j1+2,j2-1,j3,ey)+8.*
     & u2(j1+2,j2+1,j3,ey)-u2(j1+2,j2+2,j3,ey))/(12.*dr2(1)))/(12.*
     & dr2(0))
              vv2ss = (-u2(j1,j2-2,j3,ey)+16.*u2(j1,j2-1,j3,ey)-30.*u2(
     & j1,j2,j3,ey)+16.*u2(j1,j2+1,j3,ey)-u2(j1,j2+2,j3,ey))/(12.*dr2(
     & 1)**2)
               v2x = aj2rx*vv2r+aj2sx*vv2s
               v2y = aj2ry*vv2r+aj2sy*vv2s
               t1 = aj2rx**2
               t6 = aj2sx**2
               v2xx = t1*vv2rr+2*aj2rx*aj2sx*vv2rs+t6*vv2ss+aj2rxx*
     & vv2r+aj2sxx*vv2s
               t1 = aj2ry**2
               t6 = aj2sy**2
               v2yy = t1*vv2rr+2*aj2ry*aj2sy*vv2rs+t6*vv2ss+aj2ryy*
     & vv2r+aj2syy*vv2s
             v2Lap = v2xx+ v2yy
           ! first evaluate the equations we want to solve with the wrong values at the ghost points:
            f(0)=(u1x+v1y) - (u2x+v2y)
            f(1)=(an1*u1Lap+an2*v1Lap) - (an1*u2Lap+an2*v2Lap)
            f(2)=(v1x-u1y) - (v2x-u2y)
            f(3)=(tau1*u1Lap+tau2*v1Lap)/eps1 - (tau1*u2Lap+tau2*v2Lap)
     & /eps2
            f(4)=(u1xxx+u1xyy+v1xxy+v1yyy) - (u2xxx+u2xyy+v2xxy+v2yyy)
            f(5)=((v1xxx+v1xyy)-(u1xxy+u1yyy))/eps1 - ((v2xxx+v2xyy)-(
     & u2xxy+u2yyy))/eps2
            f(6)=(an1*u1LapSq+an2*v1LapSq)/eps1 - (an1*u2LapSq+an2*
     & v2LapSq)/eps2
            f(7)=(tau1*u1LapSq+tau2*v1LapSq)/eps1**2 - (tau1*u2LapSq+
     & tau2*v2LapSq)/eps2**2
            if( twilightZone.eq.1 )then
              call ogderiv(ep, 0,2,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, ex, uexx )
              call ogderiv(ep, 0,0,2,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, ex, ueyy )
              call ogderiv(ep, 0,2,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, ey, vexx )
              call ogderiv(ep, 0,0,2,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, ey, veyy )
              call ogderiv(ep, 0,2,1,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, ex, uexxy )
              call ogderiv(ep, 0,0,3,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, ex, ueyyy )
              call ogderiv(ep, 0,3,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, ey, vexxx )
              call ogderiv(ep, 0,1,2,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, ey, vexyy )
              call ogderiv(ep, 0,4,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, ex, uexxxx )
              call ogderiv(ep, 0,2,2,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, ex, uexxyy )
              call ogderiv(ep, 0,0,4,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, ex, ueyyyy )
              call ogderiv(ep, 0,4,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, ey, vexxxx )
              call ogderiv(ep, 0,2,2,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, ey, vexxyy )
              call ogderiv(ep, 0,0,4,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, ey, veyyyy )
              ueLap = uexx + ueyy
              veLap = vexx + veyy
              ueLapSq = uexxxx + 2.*uexxyy + ueyyyy
              veLapSq = vexxxx + 2.*vexxyy + veyyyy
              f(3) = f(3) - ( tau1*ueLap +tau2*veLap )*(1./eps1-
     & 1./eps2)
              f(5) = f(5) - ((vexxx+vexyy)-(uexxy+ueyyy))*(1./eps1-
     & 1./eps2)
              f(6) = f(6) - (an1*ueLapSq+an2*veLapSq)*(1./eps1-1./eps2)
              f(7) = f(7) - (tau1*ueLapSq+tau2*veLapSq)*(1./eps1**2 - 
     & 1./eps2**2)
            end if


       if( debug.gt.7 ) write(debugFile,'(" --> 4cth: j1,j2=",2i4," 
     & u1xx,u1yy,u2xx,u2yy=",4e10.2)') j1,j2,u1xx,u1yy,u2xx,u2yy
        ! '
       if( debug.gt.3 ) write(debugFile,'(" --> 4cth: i1,i2=",2i4," f(
     & start)=",8e10.2)') i1,i2,f(0),f(1),f(2),f(3),f(4),f(5),f(6),f(
     & 7)
        ! '


c here are the macros from deriv.maple (file=derivMacros.h)










           ! here is the matrix of coefficients for the unknowns u1(-1),v1(-1),u2(-1),v2(-1)
           ! Solve:
           !     
           !       A [ U ] = A [ U(old) ] - [ f ]
c      u1r4(i1,i2,i3,kd)=(8.*(u1(i1+1,i2,i3,kd)-u1(i1-1,i2,i3,kd))-(u1(
c     & i1+2,i2,i3,kd)-u1(i1-2,i2,i3,kd)))*dr114(0)
c      u1x42(i1,i2,i3,kd)= rsxy1(i1,i2,i3,0,0)*u1r4(i1,i2,i3,kd)+rsxy1(
c     & i1,i2,i3,1,0)*u1s4(i1,i2,i3,kd)
c      u1y42(i1,i2,i3,kd)= rsxy1(i1,i2,i3,0,1)*u1r4(i1,i2,i3,kd)+rsxy1(
c     & i1,i2,i3,1,1)*u1s4(i1,i2,i3,kd)
c          a4(0,0) = -is*rsxy1(i1,i2,i3,axis1,0)/(2.*dr1(axis1))    ! coeff of u1(-1) from [u.x+v.y] 
c          a4(0,1) = -is*rsxy1(i1,i2,i3,axis1,1)/(2.*dr1(axis1))    ! coeff of v1(-1) from [u.x+v.y] 
c
c          a4(2,0) =  is*rsxy1(i1,i2,i3,axis1,1)/(2.*dr1(axis1))   ! coeff of u1(-1) from [v.x - u.y] 
c          a4(2,1) = -is*rsxy1(i1,i2,i3,axis1,0)/(2.*dr1(axis1))   ! coeff of v1(-1) from [v.x - u.y] 
c
c          a4(0,2) =  js*rsxy2(j1,j2,j3,axis2,0)/(2.*dr2(axis2))    ! coeff of u2(-1) from [u.x+v.y] 
c          a4(0,3) =  js*rsxy2(j1,j2,j3,axis2,1)/(2.*dr2(axis2))    ! coeff of v2(-1) from [u.x+v.y] 
c
c          a4(2,2) = -js*rsxy2(j1,j2,j3,axis2,1)/(2.*dr2(axis2))   ! coeff of u2(-1) from [v.x - u.y] 
c          a4(2,3) =  js*rsxy2(j1,j2,j3,axis2,0)/(2.*dr2(axis2))   ! coeff of v2(-1) from [v.x - u.y] 


           ! write(debugFile,'(" interface:E: initialized,it=",2i4)') initialized,it
           if( .false. .or. (initialized.eq.0 .and. it.eq.1) )then
             ! form the matrix (and save factor for later use)

             ! 0  [ u.x + v.y ] = 0
             aa8(0,0,0,nn) = -is*8.*rsxy1(i1,i2,i3,axis1,0)*dr114(
     & axis1)     ! coeff of u1(-1) from [u.x+v.y]
             aa8(0,1,0,nn) = -is*8.*rsxy1(i1,i2,i3,axis1,1)*dr114(
     & axis1)     ! coeff of v1(-1) from [u.x+v.y]
             aa8(0,4,0,nn) =  is*   rsxy1(i1,i2,i3,axis1,0)*dr114(
     & axis1)     ! u1(-2)
             aa8(0,5,0,nn) =  is*   rsxy1(i1,i2,i3,axis1,1)*dr114(
     & axis1)     ! v1(-2)

             aa8(0,2,0,nn) =  js*8.*rsxy2(j1,j2,j3,axis2,0)*dr214(
     & axis2)     ! coeff of u2(-1) from [u.x+v.y]
             aa8(0,3,0,nn) =  js*8.*rsxy2(j1,j2,j3,axis2,1)*dr214(
     & axis2)
             aa8(0,6,0,nn) = -js*   rsxy2(j1,j2,j3,axis2,0)*dr214(
     & axis2)
             aa8(0,7,0,nn) = -js*   rsxy2(j1,j2,j3,axis2,1)*dr214(
     & axis2)

           ! 1  [ u.xx + u.yy ] = 0
c this macro comes from deriv.maple
c return the coefficient of u(-1) in uxxx+uxyy
c#defineMacro lapCoeff4a(is,dr,ds) ((-1/3.*rxx*is-1/3.*ryy*is)/dr+(4/3.*rx**2+4/3.*ry**2)/dr**2)

c return the coefficient of u(-2) in uxxx+uxyy
c#defineMacro lapCoeff4b(is,dr,ds) ((1/24.*rxx*is+1/24.*ryy*is)/dr+(-1/12.*rx**2-1/12.*ry**2)/dr**2 )

             if( axis1.eq.0 )then
               rx   =aj1rx
               ry   =aj1ry
               rxx  =aj1rxx
               rxy  =aj1rxy
               ryy  =aj1ryy
               rxxx =aj1rxxx
               rxxy =aj1rxxy
               rxyy =aj1rxyy
               ryyy =aj1ryyy
               rxxxx=aj1rxxxx
               rxxyy=aj1rxxyy
               ryyyy=aj1ryyyy
               sx   =aj1sx
               sy   =aj1sy
               sxx  =aj1sxx
               sxy  =aj1sxy
               syy  =aj1syy
               sxxx =aj1sxxx
               sxxy =aj1sxxy
               sxyy =aj1sxyy
               syyy =aj1syyy
               sxxxx=aj1sxxxx
               sxxyy=aj1sxxyy
               syyyy=aj1syyyy
             else
               rx   =aj1sx
               ry   =aj1sy
               rxx  =aj1sxx
               rxy  =aj1sxy
               ryy  =aj1syy
               rxxx =aj1sxxx
               rxxy =aj1sxxy
               rxyy =aj1sxyy
               ryyy =aj1syyy
               rxxxx=aj1sxxxx
               rxxyy=aj1sxxyy
               ryyyy=aj1syyyy
               sx   =aj1rx
               sy   =aj1ry
               sxx  =aj1rxx
               sxy  =aj1rxy
               syy  =aj1ryy
               sxxx =aj1rxxx
               sxxy =aj1rxxy
               sxyy =aj1rxyy
               syyy =aj1ryyy
               sxxxx=aj1rxxxx
               sxxyy=aj1rxxyy
               syyyy=aj1ryyyy
             end if

             dr0=dr1(axis1)
             ds0=dr1(axis1p1)
             aLap0 = ((-2/3.*rxx*is-2/3.*ryy*is)/dr0+(4/3.*rx**2+4/3.*
     & ry**2)/dr0**2)
             aLap1 = ((1/12.*rxx*is+1/12.*ryy*is)/dr0+(-1/12.*rx**2-
     & 1/12.*ry**2)/dr0**2)

             if( axis2.eq.0 )then
               rx   =aj2rx
               ry   =aj2ry
               rxx  =aj2rxx
               rxy  =aj2rxy
               ryy  =aj2ryy
               rxxx =aj2rxxx
               rxxy =aj2rxxy
               rxyy =aj2rxyy
               ryyy =aj2ryyy
               rxxxx=aj2rxxxx
               rxxyy=aj2rxxyy
               ryyyy=aj2ryyyy
               sx   =aj2sx
               sy   =aj2sy
               sxx  =aj2sxx
               sxy  =aj2sxy
               syy  =aj2syy
               sxxx =aj2sxxx
               sxxy =aj2sxxy
               sxyy =aj2sxyy
               syyy =aj2syyy
               sxxxx=aj2sxxxx
               sxxyy=aj2sxxyy
               syyyy=aj2syyyy
             else
               rx   =aj2sx
               ry   =aj2sy
               rxx  =aj2sxx
               rxy  =aj2sxy
               ryy  =aj2syy
               rxxx =aj2sxxx
               rxxy =aj2sxxy
               rxyy =aj2sxyy
               ryyy =aj2syyy
               rxxxx=aj2sxxxx
               rxxyy=aj2sxxyy
               ryyyy=aj2syyyy
               sx   =aj2rx
               sy   =aj2ry
               sxx  =aj2rxx
               sxy  =aj2rxy
               syy  =aj2ryy
               sxxx =aj2rxxx
               sxxy =aj2rxxy
               sxyy =aj2rxyy
               syyy =aj2ryyy
               sxxxx=aj2rxxxx
               sxxyy=aj2rxxyy
               syyyy=aj2ryyyy
             end if
             dr0=dr2(axis2)
             ds0=dr2(axis2p1)
             bLap0 = ((-2/3.*rxx*js-2/3.*ryy*js)/dr0+(4/3.*rx**2+4/3.*
     & ry**2)/dr0**2)
             bLap1 = ((1/12.*rxx*js+1/12.*ryy*js)/dr0+(-1/12.*rx**2-
     & 1/12.*ry**2)/dr0**2)

            if( debug.gt.8 )then
             aa8(1,0,0,nn) = 16.*dx142(axis1)         ! coeff of u1(-1) from [u.xx + u.yy]
             aa8(1,4,0,nn) =    -dx142(axis1)         ! coeff of u1(-2) from [u.xx + u.yy]
              write(debugFile,'(" 4th: lap4: aLap0: rect=",e12.4," 
     & curv=",e12.4)') aLap0,aa8(1,0,0,nn)
              ! '
              write(debugFile,'(" 4th: lap4: aLap1: rect=",e12.4," 
     & curv=",e12.4)') aLap1,aa8(1,4,0,nn)
              ! '
            end if

             aa8(1,0,0,nn) = an1*aLap0       ! coeff of u1(-1) from [n.(u.xx + u.yy)]
             aa8(1,1,0,nn) = an2*aLap0
             aa8(1,4,0,nn) = an1*aLap1       ! coeff of u1(-2) from [n.(u.xx + u.yy)]
             aa8(1,5,0,nn) = an2*aLap1

             aa8(1,2,0,nn) =-an1*bLap0       ! coeff of u2(-1) from [n.(u.xx + u.yy)]
             aa8(1,3,0,nn) =-an2*bLap0
             aa8(1,6,0,nn) =-an1*bLap1       ! coeff of u2(-2) from [n.(u.xx + u.yy)]
             aa8(1,7,0,nn) =-an2*bLap1

           ! 2  [ v.x - u.y ] =0 
c          a8(2,0) =  is*8.*ry1*dx114(axis1)
c          a8(2,1) = -is*8.*rx1*dx114(axis1)    ! coeff of v1(-1) from [v.x - u.y] 
c          a8(2,4) = -is*   ry1*dx114(axis1)
c          a8(2,5) =  is*   rx1*dx114(axis1)
c          a8(2,2) = -js*8.*ry2*dx214(axis2)
c          a8(2,3) =  js*8.*rx2*dx214(axis2)
c          a8(2,6) =  js*   ry2*dx214(axis2)
c          a8(2,7) = -js*   rx2*dx214(axis2)

             aa8(2,0,0,nn) =  is*8.*rsxy1(i1,i2,i3,axis1,1)*dr114(
     & axis1)
             aa8(2,1,0,nn) = -is*8.*rsxy1(i1,i2,i3,axis1,0)*dr114(
     & axis1)
             aa8(2,4,0,nn) = -is*   rsxy1(i1,i2,i3,axis1,1)*dr114(
     & axis1)
             aa8(2,5,0,nn) =  is*   rsxy1(i1,i2,i3,axis1,0)*dr114(
     & axis1)

             aa8(2,2,0,nn) = -js*8.*rsxy2(j1,j2,j3,axis2,1)*dr214(
     & axis2)
             aa8(2,3,0,nn) =  js*8.*rsxy2(j1,j2,j3,axis2,0)*dr214(
     & axis2)
             aa8(2,6,0,nn) =  js*   rsxy2(j1,j2,j3,axis2,1)*dr214(
     & axis2)
             aa8(2,7,0,nn) = -js*   rsxy2(j1,j2,j3,axis2,0)*dr214(
     & axis2)

             ! 3  [ tau.(uv.xx+uv.yy)/eps ] = 0
             aa8(3,0,0,nn) =tau1*aLap0/eps1
             aa8(3,1,0,nn) =tau2*aLap0/eps1
             aa8(3,4,0,nn) =tau1*aLap1/eps1
             aa8(3,5,0,nn) =tau2*aLap1/eps1

             aa8(3,2,0,nn) =-tau1*bLap0/eps2
             aa8(3,3,0,nn) =-tau2*bLap0/eps2
             aa8(3,6,0,nn) =-tau1*bLap1/eps2
             aa8(3,7,0,nn) =-tau2*bLap1/eps2


             ! 4  [ (u.xx+u.yy).x + (v.xx+v.yy).y ] = 0

            if( axis1.eq.0 )then
              rx   =aj1rx
              ry   =aj1ry
              rxx  =aj1rxx
              rxy  =aj1rxy
              ryy  =aj1ryy
              rxxx =aj1rxxx
              rxxy =aj1rxxy
              rxyy =aj1rxyy
              ryyy =aj1ryyy
              rxxxx=aj1rxxxx
              rxxyy=aj1rxxyy
              ryyyy=aj1ryyyy
              sx   =aj1sx
              sy   =aj1sy
              sxx  =aj1sxx
              sxy  =aj1sxy
              syy  =aj1syy
              sxxx =aj1sxxx
              sxxy =aj1sxxy
              sxyy =aj1sxyy
              syyy =aj1syyy
              sxxxx=aj1sxxxx
              sxxyy=aj1sxxyy
              syyyy=aj1syyyy
            else
              rx   =aj1sx
              ry   =aj1sy
              rxx  =aj1sxx
              rxy  =aj1sxy
              ryy  =aj1syy
              rxxx =aj1sxxx
              rxxy =aj1sxxy
              rxyy =aj1sxyy
              ryyy =aj1syyy
              rxxxx=aj1sxxxx
              rxxyy=aj1sxxyy
              ryyyy=aj1syyyy
              sx   =aj1rx
              sy   =aj1ry
              sxx  =aj1rxx
              sxy  =aj1rxy
              syy  =aj1ryy
              sxxx =aj1rxxx
              sxxy =aj1rxxy
              sxyy =aj1rxyy
              syyy =aj1ryyy
              sxxxx=aj1rxxxx
              sxxyy=aj1rxxyy
              syyyy=aj1ryyyy
            end if

            dr0=dr1(axis1)
            ds0=dr1(axis1p1)
            aLapX0 = ((-1/2.*rxyy*is-1/2.*rxxx*is+(sy*(ry*sx*is+sy*rx*
     & is)+3*rx*sx**2*is+ry*sy*sx*is)/ds0**2)/dr0+(2*ry*rxy+3*rx*rxx+
     & ryy*rx)/dr0**2+(ry**2*rx*is+rx**3*is)/dr0**3)
            aLapX1 = ((-1/2.*rx**3*is-1/2.*ry**2*rx*is)/dr0**3)

            bLapY0 = ((-1/2.*ryyy*is-1/2.*rxxy*is+(3*ry*sy**2*is+ry*sx*
     & *2*is+2*sy*rx*sx*is)/ds0**2)/dr0+(2*rxy*rx+ry*rxx+3*ry*ryy)
     & /dr0**2+(ry**3*is+ry*rx**2*is)/dr0**3)
            bLapY1 = ((-1/2.*ry*rx**2*is-1/2.*ry**3*is)/dr0**3)

            if( axis2.eq.0 )then
              rx   =aj2rx
              ry   =aj2ry
              rxx  =aj2rxx
              rxy  =aj2rxy
              ryy  =aj2ryy
              rxxx =aj2rxxx
              rxxy =aj2rxxy
              rxyy =aj2rxyy
              ryyy =aj2ryyy
              rxxxx=aj2rxxxx
              rxxyy=aj2rxxyy
              ryyyy=aj2ryyyy
              sx   =aj2sx
              sy   =aj2sy
              sxx  =aj2sxx
              sxy  =aj2sxy
              syy  =aj2syy
              sxxx =aj2sxxx
              sxxy =aj2sxxy
              sxyy =aj2sxyy
              syyy =aj2syyy
              sxxxx=aj2sxxxx
              sxxyy=aj2sxxyy
              syyyy=aj2syyyy
            else
              rx   =aj2sx
              ry   =aj2sy
              rxx  =aj2sxx
              rxy  =aj2sxy
              ryy  =aj2syy
              rxxx =aj2sxxx
              rxxy =aj2sxxy
              rxyy =aj2sxyy
              ryyy =aj2syyy
              rxxxx=aj2sxxxx
              rxxyy=aj2sxxyy
              ryyyy=aj2syyyy
              sx   =aj2rx
              sy   =aj2ry
              sxx  =aj2rxx
              sxy  =aj2rxy
              syy  =aj2ryy
              sxxx =aj2rxxx
              sxxy =aj2rxxy
              sxyy =aj2rxyy
              syyy =aj2ryyy
              sxxxx=aj2rxxxx
              sxxyy=aj2rxxyy
              syyyy=aj2ryyyy
            end if

            dr0=dr2(axis2)
            ds0=dr2(axis2p1)
            cLapX0 = ((-1/2.*rxyy*js-1/2.*rxxx*js+(sy*(ry*sx*js+sy*rx*
     & js)+3*rx*sx**2*js+ry*sy*sx*js)/ds0**2)/dr0+(2*ry*rxy+3*rx*rxx+
     & ryy*rx)/dr0**2+(ry**2*rx*js+rx**3*js)/dr0**3)
            cLapX1 = ((-1/2.*rx**3*js-1/2.*ry**2*rx*js)/dr0**3)

            dLapY0 = ((-1/2.*ryyy*js-1/2.*rxxy*js+(3*ry*sy**2*js+ry*sx*
     & *2*js+2*sy*rx*sx*js)/ds0**2)/dr0+(2*rxy*rx+ry*rxx+3*ry*ryy)
     & /dr0**2+(ry**3*js+ry*rx**2*js)/dr0**3)
            dLapY1 = ((-1/2.*ry*rx**2*js-1/2.*ry**3*js)/dr0**3)


            ! 4  [ (u.xx+u.yy).x + (v.xx+v.yy).y ] = 0
            if( debug.gt.8 )then
            aa8(4,0,0,nn)= ( is*rx1*2.*dx122(axis1)*dx112(axis1)+is*
     & rx1*2.*dx122(1)/(2.*dx1(0)))
            aa8(4,1,0,nn)= ( is*ry1*2.*dx122(axis1)*dx112(axis1)+is*
     & ry1*2.*dx122(0)/(2.*dx1(1)))
            aa8(4,4,0,nn)= (-is*rx1   *dx122(axis1)*dx112(axis1) )
            aa8(4,5,0,nn)= (-is*ry1   *dx122(axis1)*dx112(axis1))
              write(debugFile,'(" 4th: xlap4: aLapX0: rect=",e12.4," 
     & curv=",e12.4)') aLapX0,aa8(4,0,0,nn)
              write(debugFile,'(" 4th: xlap4: aLapX1: rect=",e12.4," 
     & curv=",e12.4)') aLapX1,aa8(4,4,0,nn)
              write(debugFile,'(" 4th: ylap4: bLapY0: rect=",e12.4," 
     & curv=",e12.4)') bLapY0,aa8(4,1,0,nn)
              write(debugFile,'(" 4th: ylap4: bLapY1: rect=",e12.4," 
     & curv=",e12.4)') bLapY1,aa8(4,5,0,nn)
              ! '
            end if

            aa8(4,0,0,nn)= aLapX0
            aa8(4,1,0,nn)= bLapY0
            aa8(4,4,0,nn)= aLapX1
            aa8(4,5,0,nn)= bLapY1

            aa8(4,2,0,nn)=-cLapX0
            aa8(4,3,0,nn)=-dLapY0
            aa8(4,6,0,nn)=-cLapX1
            aa8(4,7,0,nn)=-dLapY1

            ! 5  [ {(Delta v).x - (Delta u).y}/eps ] =0  -> [ {(v.xxx+v.xyy)-(u.xxy+u.yyy)}/eps ] = 0

            aa8(5,0,0,nn)=-bLapY0/eps1
            aa8(5,1,0,nn)= aLapX0/eps1
            aa8(5,4,0,nn)=-bLapY1/eps1
            aa8(5,5,0,nn)= aLapX1/eps1

            aa8(5,2,0,nn)= dLapY0/eps2
            aa8(5,3,0,nn)=-cLapX0/eps2
            aa8(5,6,0,nn)= dLapY1/eps2
            aa8(5,7,0,nn)=-cLapX1/eps2


             ! 6  [ n.Delta^2 u/eps ] = 0

             ! assign rx,ry,rxx,rxy,... 

             if( axis1.eq.0 )then
               rx   =aj1rx
               ry   =aj1ry
               rxx  =aj1rxx
               rxy  =aj1rxy
               ryy  =aj1ryy
               rxxx =aj1rxxx
               rxxy =aj1rxxy
               rxyy =aj1rxyy
               ryyy =aj1ryyy
               rxxxx=aj1rxxxx
               rxxyy=aj1rxxyy
               ryyyy=aj1ryyyy
               sx   =aj1sx
               sy   =aj1sy
               sxx  =aj1sxx
               sxy  =aj1sxy
               syy  =aj1syy
               sxxx =aj1sxxx
               sxxy =aj1sxxy
               sxyy =aj1sxyy
               syyy =aj1syyy
               sxxxx=aj1sxxxx
               sxxyy=aj1sxxyy
               syyyy=aj1syyyy
             else
               rx   =aj1sx
               ry   =aj1sy
               rxx  =aj1sxx
               rxy  =aj1sxy
               ryy  =aj1syy
               rxxx =aj1sxxx
               rxxy =aj1sxxy
               rxyy =aj1sxyy
               ryyy =aj1syyy
               rxxxx=aj1sxxxx
               rxxyy=aj1sxxyy
               ryyyy=aj1syyyy
               sx   =aj1rx
               sy   =aj1ry
               sxx  =aj1rxx
               sxy  =aj1rxy
               syy  =aj1ryy
               sxxx =aj1rxxx
               sxxy =aj1rxxy
               sxyy =aj1rxyy
               syyy =aj1ryyy
               sxxxx=aj1rxxxx
               sxxyy=aj1rxxyy
               syyyy=aj1ryyyy
             end if

             dr0=dr1(axis1)
             ds0=dr1(axis1p1)
             aLapSq0 = ((-1/2.*rxxxx*is-rxxyy*is-1/2.*ryyyy*is+(2*sy*(
     & 2*rxy*sx*is+2*rx*sxy*is)+2*ry*(2*sxy*sx*is+sy*sxx*is)+7*rx*sxx*
     & sx*is+sy*(3*ry*syy*is+3*sy*ryy*is)+sx*(3*rx*sxx*is+3*rxx*sx*is)
     & +sx*(2*rxx*sx*is+2*rx*sxx*is)+2*sy*(2*rx*sxy*is+ry*sxx*is+2*
     & rxy*sx*is+sy*rxx*is)+7*ry*sy*syy*is+rxx*sx**2*is+4*ry*sxy*sx*
     & is+4*syy*rx*sx*is+2*ryy*sx**2*is+ryy*sy**2*is+sy*(2*sy*ryy*is+
     & 2*ry*syy*is))/ds0**2)/dr0+(3*ryy**2+3*rxx**2+4*rxy**2+4*ry*
     & rxxy+4*rx*rxxx+4*ry*ryyy+2*ryy*rxx+4*rx*rxyy+(2*ry*(-4*sy*rx*
     & sx-2*ry*sx**2)-12*ry**2*sy**2+2*sy*(-2*sy*rx**2-4*ry*rx*sx)-12*
     & rx**2*sx**2)/ds0**2)/dr0**2+(6*ry**2*ryy*is+4*ry*rxy*rx*is+2*
     & ry*(ry*rxx*is+2*rxy*rx*is)+6*rxx*rx**2*is+2*ryy*rx**2*is)/dr0**
     & 3+(-8*ry**2*rx**2-4*ry**4-4*rx**4)/dr0**4)
             aLapSq1 = ((-3*rxx*rx**2*is-ryy*rx**2*is-2*ry*rxy*rx*is-3*
     & ry**2*ryy*is+2*ry*(-rxy*rx*is-1/2.*ry*rxx*is))/dr0**3+(rx**4+2*
     & ry**2*rx**2+ry**4)/dr0**4)

             if( debug.gt.8 )then
               aa8(6,0,0,nn) = -(4./(dx1(axis1)**4) +4./(dx1(0)**2*dx1(
     & 1)**2) )
               aa8(6,4,0,nn) =   1./(dx1(axis1)**4)
               write(debugFile,'(" 4th: lapSq: aLapSq0: rect=",e12.4," 
     & curv=",e12.4)') aLapSq0,aa8(6,0,0,nn)
               ! '
               write(debugFile,'(" 4th: lapSq: aLapSq1: rect=",e12.4," 
     & curv=",e12.4)') aLapSq1,aa8(6,4,0,nn)
               ! '
             end if

             aa8(6,0,0,nn) = an1*aLapSq0/eps1
             aa8(6,1,0,nn) = an2*aLapSq0/eps1
             aa8(6,4,0,nn) = an1*aLapSq1/eps1
             aa8(6,5,0,nn) = an2*aLapSq1/eps1

             if( axis2.eq.0 )then
               rx   =aj2rx
               ry   =aj2ry
               rxx  =aj2rxx
               rxy  =aj2rxy
               ryy  =aj2ryy
               rxxx =aj2rxxx
               rxxy =aj2rxxy
               rxyy =aj2rxyy
               ryyy =aj2ryyy
               rxxxx=aj2rxxxx
               rxxyy=aj2rxxyy
               ryyyy=aj2ryyyy
               sx   =aj2sx
               sy   =aj2sy
               sxx  =aj2sxx
               sxy  =aj2sxy
               syy  =aj2syy
               sxxx =aj2sxxx
               sxxy =aj2sxxy
               sxyy =aj2sxyy
               syyy =aj2syyy
               sxxxx=aj2sxxxx
               sxxyy=aj2sxxyy
               syyyy=aj2syyyy
             else
               rx   =aj2sx
               ry   =aj2sy
               rxx  =aj2sxx
               rxy  =aj2sxy
               ryy  =aj2syy
               rxxx =aj2sxxx
               rxxy =aj2sxxy
               rxyy =aj2sxyy
               ryyy =aj2syyy
               rxxxx=aj2sxxxx
               rxxyy=aj2sxxyy
               ryyyy=aj2syyyy
               sx   =aj2rx
               sy   =aj2ry
               sxx  =aj2rxx
               sxy  =aj2rxy
               syy  =aj2ryy
               sxxx =aj2rxxx
               sxxy =aj2rxxy
               sxyy =aj2rxyy
               syyy =aj2ryyy
               sxxxx=aj2rxxxx
               sxxyy=aj2rxxyy
               syyyy=aj2ryyyy
             end if
             dr0=dr2(axis2)
             ds0=dr2(axis2p1)
             bLapSq0 = ((-1/2.*rxxxx*js-rxxyy*js-1/2.*ryyyy*js+(2*sy*(
     & 2*rxy*sx*js+2*rx*sxy*js)+2*ry*(2*sxy*sx*js+sy*sxx*js)+7*rx*sxx*
     & sx*js+sy*(3*ry*syy*js+3*sy*ryy*js)+sx*(3*rx*sxx*js+3*rxx*sx*js)
     & +sx*(2*rxx*sx*js+2*rx*sxx*js)+2*sy*(2*rx*sxy*js+ry*sxx*js+2*
     & rxy*sx*js+sy*rxx*js)+7*ry*sy*syy*js+rxx*sx**2*js+4*ry*sxy*sx*
     & js+4*syy*rx*sx*js+2*ryy*sx**2*js+ryy*sy**2*js+sy*(2*sy*ryy*js+
     & 2*ry*syy*js))/ds0**2)/dr0+(3*ryy**2+3*rxx**2+4*rxy**2+4*ry*
     & rxxy+4*rx*rxxx+4*ry*ryyy+2*ryy*rxx+4*rx*rxyy+(2*ry*(-4*sy*rx*
     & sx-2*ry*sx**2)-12*ry**2*sy**2+2*sy*(-2*sy*rx**2-4*ry*rx*sx)-12*
     & rx**2*sx**2)/ds0**2)/dr0**2+(6*ry**2*ryy*js+4*ry*rxy*rx*js+2*
     & ry*(ry*rxx*js+2*rxy*rx*js)+6*rxx*rx**2*js+2*ryy*rx**2*js)/dr0**
     & 3+(-8*ry**2*rx**2-4*ry**4-4*rx**4)/dr0**4)
             bLapSq1 = ((-3*rxx*rx**2*js-ryy*rx**2*js-2*ry*rxy*rx*js-3*
     & ry**2*ryy*js+2*ry*(-rxy*rx*js-1/2.*ry*rxx*js))/dr0**3+(rx**4+2*
     & ry**2*rx**2+ry**4)/dr0**4)

             aa8(6,2,0,nn) = -an1*bLapSq0/eps2
             aa8(6,3,0,nn) = -an2*bLapSq0/eps2
             aa8(6,6,0,nn) = -an1*bLapSq1/eps2
             aa8(6,7,0,nn) = -an2*bLapSq1/eps2

             ! 7  [ tau.Delta^2 v/eps^2 ] = 0 
             aa8(7,0,0,nn) = tau1*aLapSq0/eps1**2
             aa8(7,1,0,nn) = tau2*aLapSq0/eps1**2
             aa8(7,4,0,nn) = tau1*aLapSq1/eps1**2
             aa8(7,5,0,nn) = tau2*aLapSq1/eps1**2

             aa8(7,2,0,nn) = -tau1*bLapSq0/eps2**2
             aa8(7,3,0,nn) = -tau2*bLapSq0/eps2**2
             aa8(7,6,0,nn) = -tau1*bLapSq1/eps2**2
             aa8(7,7,0,nn) = -tau2*bLapSq1/eps2**2

             ! save a copy of the matrix
             do n2=0,7
             do n1=0,7
               aa8(n1,n2,1,nn)=aa8(n1,n2,0,nn)
             end do
             end do

             ! solve A Q = F
             ! factor the matrix
             numberOfEquations=8
             call dgeco( aa8(0,0,0,nn), numberOfEquations, 
     & numberOfEquations, ipvt8(0,nn),rcond,work(0))

             if( debug.gt.3 ) write(debugFile,'(" --> 4cth: i1,i2=",
     & 2i4," rcond=",e10.2)') i1,i2,rcond
             ! '
           end if


           q(0) = u1(i1-is1,i2-is2,i3,ex)
           q(1) = u1(i1-is1,i2-is2,i3,ey)
           q(2) = u2(j1-js1,j2-js2,j3,ex)
           q(3) = u2(j1-js1,j2-js2,j3,ey)

           q(4) = u1(i1-2*is1,i2-2*is2,i3,ex)
           q(5) = u1(i1-2*is1,i2-2*is2,i3,ey)
           q(6) = u2(j1-2*js1,j2-2*js2,j3,ex)
           q(7) = u2(j1-2*js1,j2-2*js2,j3,ey)

       if( debug.gt.4 ) write(debugFile,'(" --> 4cth: i1,i2=",2i4," 
     & q=",8e10.2)') i1,i2,(q(n),n=0,7)

           ! subtract off the contributions from the initial (wrong) values at the ghost points:
           do n=0,7
             f(n) = (aa8(n,0,1,nn)*q(0)+aa8(n,1,1,nn)*q(1)+aa8(n,2,1,
     & nn)*q(2)+aa8(n,3,1,nn)*q(3)+aa8(n,4,1,nn)*q(4)+aa8(n,5,1,nn)*q(
     & 5)+aa8(n,6,1,nn)*q(6)+aa8(n,7,1,nn)*q(7)) - f(n)
           end do

                                ! '

           ! solve A Q = F
           job=0
           numberOfEquations=8
           call dgesl( aa8(0,0,0,nn), numberOfEquations, 
     & numberOfEquations, ipvt8(0,nn), f(0), job)

       if( debug.gt.4 )then
          write(debugFile,'(" --> 4cth: i1,i2=",2i4," f(solve)=",
     & 8e10.2)') i1,i2,(f(n),n=0,7)
          write(debugFile,'(" --> 4cth: i1,i2=",2i4,"      f-q=",
     & 8e10.2)') i1,i2,(f(n)-q(n),n=0,7)
       end if
           ! '

           if( .true. )then
           u1(i1-is1,i2-is2,i3,ex)=f(0)
           u1(i1-is1,i2-is2,i3,ey)=f(1)
           u2(j1-js1,j2-js2,j3,ex)=f(2)
           u2(j1-js1,j2-js2,j3,ey)=f(3)

           u1(i1-2*is1,i2-2*is2,i3,ex)=f(4)
           u1(i1-2*is1,i2-2*is2,i3,ey)=f(5)
           u2(j1-2*js1,j2-2*js2,j3,ex)=f(6)
           u2(j1-2*js1,j2-2*js2,j3,ey)=f(7)
           end if

          if( debug.gt.0 )then ! re-evaluate

           ! compute the maximum change in the solution for this iteration
           do n=0,7
             err=max(err,abs(q(n)-f(n)))
           end do

            ! These derivatives are computed to 2nd-order accuracy
            ! NOTE: the jacobian derivatives can be computed once for all components
             ! this next call will define the jacobian and its derivatives (parameteric and spatial)
             aj1rx = rsxy1(i1,i2,i3,0,0)
             aj1rxr = (-rsxy1(i1-1,i2,i3,0,0)+rsxy1(i1+1,i2,i3,0,0))/(
     & 2.*dr1(0))
             aj1rxs = (-rsxy1(i1,i2-1,i3,0,0)+rsxy1(i1,i2+1,i3,0,0))/(
     & 2.*dr1(1))
             aj1rxrr = (rsxy1(i1-1,i2,i3,0,0)-2.*rsxy1(i1,i2,i3,0,0)+
     & rsxy1(i1+1,i2,i3,0,0))/(dr1(0)**2)
             aj1rxrs = (-(-rsxy1(i1-1,i2-1,i3,0,0)+rsxy1(i1-1,i2+1,i3,
     & 0,0))/(2.*dr1(1))+(-rsxy1(i1+1,i2-1,i3,0,0)+rsxy1(i1+1,i2+1,i3,
     & 0,0))/(2.*dr1(1)))/(2.*dr1(0))
             aj1rxss = (rsxy1(i1,i2-1,i3,0,0)-2.*rsxy1(i1,i2,i3,0,0)+
     & rsxy1(i1,i2+1,i3,0,0))/(dr1(1)**2)
             aj1rxrrr = (-rsxy1(i1-2,i2,i3,0,0)+2.*rsxy1(i1-1,i2,i3,0,
     & 0)-2.*rsxy1(i1+1,i2,i3,0,0)+rsxy1(i1+2,i2,i3,0,0))/(2.*dr1(0)**
     & 3)
             aj1rxrrs = ((-rsxy1(i1-1,i2-1,i3,0,0)+rsxy1(i1-1,i2+1,i3,
     & 0,0))/(2.*dr1(1))-2.*(-rsxy1(i1,i2-1,i3,0,0)+rsxy1(i1,i2+1,i3,
     & 0,0))/(2.*dr1(1))+(-rsxy1(i1+1,i2-1,i3,0,0)+rsxy1(i1+1,i2+1,i3,
     & 0,0))/(2.*dr1(1)))/(dr1(0)**2)
             aj1rxrss = (-(rsxy1(i1-1,i2-1,i3,0,0)-2.*rsxy1(i1-1,i2,i3,
     & 0,0)+rsxy1(i1-1,i2+1,i3,0,0))/(dr1(1)**2)+(rsxy1(i1+1,i2-1,i3,
     & 0,0)-2.*rsxy1(i1+1,i2,i3,0,0)+rsxy1(i1+1,i2+1,i3,0,0))/(dr1(1)*
     & *2))/(2.*dr1(0))
             aj1rxsss = (-rsxy1(i1,i2-2,i3,0,0)+2.*rsxy1(i1,i2-1,i3,0,
     & 0)-2.*rsxy1(i1,i2+1,i3,0,0)+rsxy1(i1,i2+2,i3,0,0))/(2.*dr1(1)**
     & 3)
             aj1sx = rsxy1(i1,i2,i3,1,0)
             aj1sxr = (-rsxy1(i1-1,i2,i3,1,0)+rsxy1(i1+1,i2,i3,1,0))/(
     & 2.*dr1(0))
             aj1sxs = (-rsxy1(i1,i2-1,i3,1,0)+rsxy1(i1,i2+1,i3,1,0))/(
     & 2.*dr1(1))
             aj1sxrr = (rsxy1(i1-1,i2,i3,1,0)-2.*rsxy1(i1,i2,i3,1,0)+
     & rsxy1(i1+1,i2,i3,1,0))/(dr1(0)**2)
             aj1sxrs = (-(-rsxy1(i1-1,i2-1,i3,1,0)+rsxy1(i1-1,i2+1,i3,
     & 1,0))/(2.*dr1(1))+(-rsxy1(i1+1,i2-1,i3,1,0)+rsxy1(i1+1,i2+1,i3,
     & 1,0))/(2.*dr1(1)))/(2.*dr1(0))
             aj1sxss = (rsxy1(i1,i2-1,i3,1,0)-2.*rsxy1(i1,i2,i3,1,0)+
     & rsxy1(i1,i2+1,i3,1,0))/(dr1(1)**2)
             aj1sxrrr = (-rsxy1(i1-2,i2,i3,1,0)+2.*rsxy1(i1-1,i2,i3,1,
     & 0)-2.*rsxy1(i1+1,i2,i3,1,0)+rsxy1(i1+2,i2,i3,1,0))/(2.*dr1(0)**
     & 3)
             aj1sxrrs = ((-rsxy1(i1-1,i2-1,i3,1,0)+rsxy1(i1-1,i2+1,i3,
     & 1,0))/(2.*dr1(1))-2.*(-rsxy1(i1,i2-1,i3,1,0)+rsxy1(i1,i2+1,i3,
     & 1,0))/(2.*dr1(1))+(-rsxy1(i1+1,i2-1,i3,1,0)+rsxy1(i1+1,i2+1,i3,
     & 1,0))/(2.*dr1(1)))/(dr1(0)**2)
             aj1sxrss = (-(rsxy1(i1-1,i2-1,i3,1,0)-2.*rsxy1(i1-1,i2,i3,
     & 1,0)+rsxy1(i1-1,i2+1,i3,1,0))/(dr1(1)**2)+(rsxy1(i1+1,i2-1,i3,
     & 1,0)-2.*rsxy1(i1+1,i2,i3,1,0)+rsxy1(i1+1,i2+1,i3,1,0))/(dr1(1)*
     & *2))/(2.*dr1(0))
             aj1sxsss = (-rsxy1(i1,i2-2,i3,1,0)+2.*rsxy1(i1,i2-1,i3,1,
     & 0)-2.*rsxy1(i1,i2+1,i3,1,0)+rsxy1(i1,i2+2,i3,1,0))/(2.*dr1(1)**
     & 3)
             aj1ry = rsxy1(i1,i2,i3,0,1)
             aj1ryr = (-rsxy1(i1-1,i2,i3,0,1)+rsxy1(i1+1,i2,i3,0,1))/(
     & 2.*dr1(0))
             aj1rys = (-rsxy1(i1,i2-1,i3,0,1)+rsxy1(i1,i2+1,i3,0,1))/(
     & 2.*dr1(1))
             aj1ryrr = (rsxy1(i1-1,i2,i3,0,1)-2.*rsxy1(i1,i2,i3,0,1)+
     & rsxy1(i1+1,i2,i3,0,1))/(dr1(0)**2)
             aj1ryrs = (-(-rsxy1(i1-1,i2-1,i3,0,1)+rsxy1(i1-1,i2+1,i3,
     & 0,1))/(2.*dr1(1))+(-rsxy1(i1+1,i2-1,i3,0,1)+rsxy1(i1+1,i2+1,i3,
     & 0,1))/(2.*dr1(1)))/(2.*dr1(0))
             aj1ryss = (rsxy1(i1,i2-1,i3,0,1)-2.*rsxy1(i1,i2,i3,0,1)+
     & rsxy1(i1,i2+1,i3,0,1))/(dr1(1)**2)
             aj1ryrrr = (-rsxy1(i1-2,i2,i3,0,1)+2.*rsxy1(i1-1,i2,i3,0,
     & 1)-2.*rsxy1(i1+1,i2,i3,0,1)+rsxy1(i1+2,i2,i3,0,1))/(2.*dr1(0)**
     & 3)
             aj1ryrrs = ((-rsxy1(i1-1,i2-1,i3,0,1)+rsxy1(i1-1,i2+1,i3,
     & 0,1))/(2.*dr1(1))-2.*(-rsxy1(i1,i2-1,i3,0,1)+rsxy1(i1,i2+1,i3,
     & 0,1))/(2.*dr1(1))+(-rsxy1(i1+1,i2-1,i3,0,1)+rsxy1(i1+1,i2+1,i3,
     & 0,1))/(2.*dr1(1)))/(dr1(0)**2)
             aj1ryrss = (-(rsxy1(i1-1,i2-1,i3,0,1)-2.*rsxy1(i1-1,i2,i3,
     & 0,1)+rsxy1(i1-1,i2+1,i3,0,1))/(dr1(1)**2)+(rsxy1(i1+1,i2-1,i3,
     & 0,1)-2.*rsxy1(i1+1,i2,i3,0,1)+rsxy1(i1+1,i2+1,i3,0,1))/(dr1(1)*
     & *2))/(2.*dr1(0))
             aj1rysss = (-rsxy1(i1,i2-2,i3,0,1)+2.*rsxy1(i1,i2-1,i3,0,
     & 1)-2.*rsxy1(i1,i2+1,i3,0,1)+rsxy1(i1,i2+2,i3,0,1))/(2.*dr1(1)**
     & 3)
             aj1sy = rsxy1(i1,i2,i3,1,1)
             aj1syr = (-rsxy1(i1-1,i2,i3,1,1)+rsxy1(i1+1,i2,i3,1,1))/(
     & 2.*dr1(0))
             aj1sys = (-rsxy1(i1,i2-1,i3,1,1)+rsxy1(i1,i2+1,i3,1,1))/(
     & 2.*dr1(1))
             aj1syrr = (rsxy1(i1-1,i2,i3,1,1)-2.*rsxy1(i1,i2,i3,1,1)+
     & rsxy1(i1+1,i2,i3,1,1))/(dr1(0)**2)
             aj1syrs = (-(-rsxy1(i1-1,i2-1,i3,1,1)+rsxy1(i1-1,i2+1,i3,
     & 1,1))/(2.*dr1(1))+(-rsxy1(i1+1,i2-1,i3,1,1)+rsxy1(i1+1,i2+1,i3,
     & 1,1))/(2.*dr1(1)))/(2.*dr1(0))
             aj1syss = (rsxy1(i1,i2-1,i3,1,1)-2.*rsxy1(i1,i2,i3,1,1)+
     & rsxy1(i1,i2+1,i3,1,1))/(dr1(1)**2)
             aj1syrrr = (-rsxy1(i1-2,i2,i3,1,1)+2.*rsxy1(i1-1,i2,i3,1,
     & 1)-2.*rsxy1(i1+1,i2,i3,1,1)+rsxy1(i1+2,i2,i3,1,1))/(2.*dr1(0)**
     & 3)
             aj1syrrs = ((-rsxy1(i1-1,i2-1,i3,1,1)+rsxy1(i1-1,i2+1,i3,
     & 1,1))/(2.*dr1(1))-2.*(-rsxy1(i1,i2-1,i3,1,1)+rsxy1(i1,i2+1,i3,
     & 1,1))/(2.*dr1(1))+(-rsxy1(i1+1,i2-1,i3,1,1)+rsxy1(i1+1,i2+1,i3,
     & 1,1))/(2.*dr1(1)))/(dr1(0)**2)
             aj1syrss = (-(rsxy1(i1-1,i2-1,i3,1,1)-2.*rsxy1(i1-1,i2,i3,
     & 1,1)+rsxy1(i1-1,i2+1,i3,1,1))/(dr1(1)**2)+(rsxy1(i1+1,i2-1,i3,
     & 1,1)-2.*rsxy1(i1+1,i2,i3,1,1)+rsxy1(i1+1,i2+1,i3,1,1))/(dr1(1)*
     & *2))/(2.*dr1(0))
             aj1sysss = (-rsxy1(i1,i2-2,i3,1,1)+2.*rsxy1(i1,i2-1,i3,1,
     & 1)-2.*rsxy1(i1,i2+1,i3,1,1)+rsxy1(i1,i2+2,i3,1,1))/(2.*dr1(1)**
     & 3)
             aj1rxx = aj1rx*aj1rxr+aj1sx*aj1rxs
             aj1rxy = aj1ry*aj1rxr+aj1sy*aj1rxs
             aj1sxx = aj1rx*aj1sxr+aj1sx*aj1sxs
             aj1sxy = aj1ry*aj1sxr+aj1sy*aj1sxs
             aj1ryx = aj1rx*aj1ryr+aj1sx*aj1rys
             aj1ryy = aj1ry*aj1ryr+aj1sy*aj1rys
             aj1syx = aj1rx*aj1syr+aj1sx*aj1sys
             aj1syy = aj1ry*aj1syr+aj1sy*aj1sys
             t1 = aj1rx**2
             t6 = aj1sx**2
             aj1rxxx = t1*aj1rxrr+2*aj1rx*aj1sx*aj1rxrs+t6*aj1rxss+
     & aj1rxx*aj1rxr+aj1sxx*aj1rxs
             aj1rxxy = aj1ry*aj1rx*aj1rxrr+(aj1sy*aj1rx+aj1ry*aj1sx)*
     & aj1rxrs+aj1sy*aj1sx*aj1rxss+aj1rxy*aj1rxr+aj1sxy*aj1rxs
             t1 = aj1ry**2
             t6 = aj1sy**2
             aj1rxyy = t1*aj1rxrr+2*aj1ry*aj1sy*aj1rxrs+t6*aj1rxss+
     & aj1ryy*aj1rxr+aj1syy*aj1rxs
             t1 = aj1rx**2
             t6 = aj1sx**2
             aj1sxxx = t1*aj1sxrr+2*aj1rx*aj1sx*aj1sxrs+t6*aj1sxss+
     & aj1rxx*aj1sxr+aj1sxx*aj1sxs
             aj1sxxy = aj1ry*aj1rx*aj1sxrr+(aj1sy*aj1rx+aj1ry*aj1sx)*
     & aj1sxrs+aj1sy*aj1sx*aj1sxss+aj1rxy*aj1sxr+aj1sxy*aj1sxs
             t1 = aj1ry**2
             t6 = aj1sy**2
             aj1sxyy = t1*aj1sxrr+2*aj1ry*aj1sy*aj1sxrs+t6*aj1sxss+
     & aj1ryy*aj1sxr+aj1syy*aj1sxs
             t1 = aj1rx**2
             t6 = aj1sx**2
             aj1ryxx = t1*aj1ryrr+2*aj1rx*aj1sx*aj1ryrs+t6*aj1ryss+
     & aj1rxx*aj1ryr+aj1sxx*aj1rys
             aj1ryxy = aj1ry*aj1rx*aj1ryrr+(aj1sy*aj1rx+aj1ry*aj1sx)*
     & aj1ryrs+aj1sy*aj1sx*aj1ryss+aj1rxy*aj1ryr+aj1sxy*aj1rys
             t1 = aj1ry**2
             t6 = aj1sy**2
             aj1ryyy = t1*aj1ryrr+2*aj1ry*aj1sy*aj1ryrs+t6*aj1ryss+
     & aj1ryy*aj1ryr+aj1syy*aj1rys
             t1 = aj1rx**2
             t6 = aj1sx**2
             aj1syxx = t1*aj1syrr+2*aj1rx*aj1sx*aj1syrs+t6*aj1syss+
     & aj1rxx*aj1syr+aj1sxx*aj1sys
             aj1syxy = aj1ry*aj1rx*aj1syrr+(aj1sy*aj1rx+aj1ry*aj1sx)*
     & aj1syrs+aj1sy*aj1sx*aj1syss+aj1rxy*aj1syr+aj1sxy*aj1sys
             t1 = aj1ry**2
             t6 = aj1sy**2
             aj1syyy = t1*aj1syrr+2*aj1ry*aj1sy*aj1syrs+t6*aj1syss+
     & aj1ryy*aj1syr+aj1syy*aj1sys
             t1 = aj1rx**2
             t7 = aj1sx**2
             aj1rxxxx = t1*aj1rx*aj1rxrrr+3*t1*aj1sx*aj1rxrrs+3*aj1rx*
     & t7*aj1rxrss+t7*aj1sx*aj1rxsss+3*aj1rx*aj1rxx*aj1rxrr+(3*aj1sxx*
     & aj1rx+3*aj1sx*aj1rxx)*aj1rxrs+3*aj1sxx*aj1sx*aj1rxss+aj1rxxx*
     & aj1rxr+aj1sxxx*aj1rxs
             t1 = aj1rx**2
             t10 = aj1sx**2
             aj1rxxxy = aj1ry*t1*aj1rxrrr+(aj1sy*t1+2*aj1ry*aj1sx*
     & aj1rx)*aj1rxrrs+(aj1ry*t10+2*aj1sy*aj1sx*aj1rx)*aj1rxrss+aj1sy*
     & t10*aj1rxsss+(2*aj1rxy*aj1rx+aj1ry*aj1rxx)*aj1rxrr+(aj1ry*
     & aj1sxx+2*aj1sx*aj1rxy+2*aj1sxy*aj1rx+aj1sy*aj1rxx)*aj1rxrs+(
     & aj1sy*aj1sxx+2*aj1sxy*aj1sx)*aj1rxss+aj1rxxy*aj1rxr+aj1sxxy*
     & aj1rxs
             t1 = aj1ry**2
             t4 = aj1sy*aj1ry
             t8 = aj1sy*aj1rx+aj1ry*aj1sx
             t16 = aj1sy**2
             aj1rxxyy = t1*aj1rx*aj1rxrrr+(t4*aj1rx+aj1ry*t8)*aj1rxrrs+
     & (t4*aj1sx+aj1sy*t8)*aj1rxrss+t16*aj1sx*aj1rxsss+(aj1ryy*aj1rx+
     & 2*aj1ry*aj1rxy)*aj1rxrr+(2*aj1ry*aj1sxy+2*aj1sy*aj1rxy+aj1ryy*
     & aj1sx+aj1syy*aj1rx)*aj1rxrs+(aj1syy*aj1sx+2*aj1sy*aj1sxy)*
     & aj1rxss+aj1rxyy*aj1rxr+aj1sxyy*aj1rxs
             t1 = aj1ry**2
             t7 = aj1sy**2
             aj1rxyyy = aj1ry*t1*aj1rxrrr+3*t1*aj1sy*aj1rxrrs+3*aj1ry*
     & t7*aj1rxrss+t7*aj1sy*aj1rxsss+3*aj1ry*aj1ryy*aj1rxrr+(3*aj1syy*
     & aj1ry+3*aj1sy*aj1ryy)*aj1rxrs+3*aj1syy*aj1sy*aj1rxss+aj1ryyy*
     & aj1rxr+aj1syyy*aj1rxs
             t1 = aj1rx**2
             t7 = aj1sx**2
             aj1sxxxx = t1*aj1rx*aj1sxrrr+3*t1*aj1sx*aj1sxrrs+3*aj1rx*
     & t7*aj1sxrss+t7*aj1sx*aj1sxsss+3*aj1rx*aj1rxx*aj1sxrr+(3*aj1sxx*
     & aj1rx+3*aj1sx*aj1rxx)*aj1sxrs+3*aj1sxx*aj1sx*aj1sxss+aj1rxxx*
     & aj1sxr+aj1sxxx*aj1sxs
             t1 = aj1rx**2
             t10 = aj1sx**2
             aj1sxxxy = aj1ry*t1*aj1sxrrr+(aj1sy*t1+2*aj1ry*aj1sx*
     & aj1rx)*aj1sxrrs+(aj1ry*t10+2*aj1sy*aj1sx*aj1rx)*aj1sxrss+aj1sy*
     & t10*aj1sxsss+(2*aj1rxy*aj1rx+aj1ry*aj1rxx)*aj1sxrr+(aj1ry*
     & aj1sxx+2*aj1sx*aj1rxy+2*aj1sxy*aj1rx+aj1sy*aj1rxx)*aj1sxrs+(
     & aj1sy*aj1sxx+2*aj1sxy*aj1sx)*aj1sxss+aj1rxxy*aj1sxr+aj1sxxy*
     & aj1sxs
             t1 = aj1ry**2
             t4 = aj1sy*aj1ry
             t8 = aj1sy*aj1rx+aj1ry*aj1sx
             t16 = aj1sy**2
             aj1sxxyy = t1*aj1rx*aj1sxrrr+(t4*aj1rx+aj1ry*t8)*aj1sxrrs+
     & (t4*aj1sx+aj1sy*t8)*aj1sxrss+t16*aj1sx*aj1sxsss+(aj1ryy*aj1rx+
     & 2*aj1ry*aj1rxy)*aj1sxrr+(2*aj1ry*aj1sxy+2*aj1sy*aj1rxy+aj1ryy*
     & aj1sx+aj1syy*aj1rx)*aj1sxrs+(aj1syy*aj1sx+2*aj1sy*aj1sxy)*
     & aj1sxss+aj1rxyy*aj1sxr+aj1sxyy*aj1sxs
             t1 = aj1ry**2
             t7 = aj1sy**2
             aj1sxyyy = aj1ry*t1*aj1sxrrr+3*t1*aj1sy*aj1sxrrs+3*aj1ry*
     & t7*aj1sxrss+t7*aj1sy*aj1sxsss+3*aj1ry*aj1ryy*aj1sxrr+(3*aj1syy*
     & aj1ry+3*aj1sy*aj1ryy)*aj1sxrs+3*aj1syy*aj1sy*aj1sxss+aj1ryyy*
     & aj1sxr+aj1syyy*aj1sxs
             t1 = aj1rx**2
             t7 = aj1sx**2
             aj1ryxxx = t1*aj1rx*aj1ryrrr+3*t1*aj1sx*aj1ryrrs+3*aj1rx*
     & t7*aj1ryrss+t7*aj1sx*aj1rysss+3*aj1rx*aj1rxx*aj1ryrr+(3*aj1sxx*
     & aj1rx+3*aj1sx*aj1rxx)*aj1ryrs+3*aj1sxx*aj1sx*aj1ryss+aj1rxxx*
     & aj1ryr+aj1sxxx*aj1rys
             t1 = aj1rx**2
             t10 = aj1sx**2
             aj1ryxxy = aj1ry*t1*aj1ryrrr+(aj1sy*t1+2*aj1ry*aj1sx*
     & aj1rx)*aj1ryrrs+(aj1ry*t10+2*aj1sy*aj1sx*aj1rx)*aj1ryrss+aj1sy*
     & t10*aj1rysss+(2*aj1rxy*aj1rx+aj1ry*aj1rxx)*aj1ryrr+(aj1ry*
     & aj1sxx+2*aj1sx*aj1rxy+2*aj1sxy*aj1rx+aj1sy*aj1rxx)*aj1ryrs+(
     & aj1sy*aj1sxx+2*aj1sxy*aj1sx)*aj1ryss+aj1rxxy*aj1ryr+aj1sxxy*
     & aj1rys
             t1 = aj1ry**2
             t4 = aj1sy*aj1ry
             t8 = aj1sy*aj1rx+aj1ry*aj1sx
             t16 = aj1sy**2
             aj1ryxyy = t1*aj1rx*aj1ryrrr+(t4*aj1rx+aj1ry*t8)*aj1ryrrs+
     & (t4*aj1sx+aj1sy*t8)*aj1ryrss+t16*aj1sx*aj1rysss+(aj1ryy*aj1rx+
     & 2*aj1ry*aj1rxy)*aj1ryrr+(2*aj1ry*aj1sxy+2*aj1sy*aj1rxy+aj1ryy*
     & aj1sx+aj1syy*aj1rx)*aj1ryrs+(aj1syy*aj1sx+2*aj1sy*aj1sxy)*
     & aj1ryss+aj1rxyy*aj1ryr+aj1sxyy*aj1rys
             t1 = aj1ry**2
             t7 = aj1sy**2
             aj1ryyyy = aj1ry*t1*aj1ryrrr+3*t1*aj1sy*aj1ryrrs+3*aj1ry*
     & t7*aj1ryrss+t7*aj1sy*aj1rysss+3*aj1ry*aj1ryy*aj1ryrr+(3*aj1syy*
     & aj1ry+3*aj1sy*aj1ryy)*aj1ryrs+3*aj1syy*aj1sy*aj1ryss+aj1ryyy*
     & aj1ryr+aj1syyy*aj1rys
             t1 = aj1rx**2
             t7 = aj1sx**2
             aj1syxxx = t1*aj1rx*aj1syrrr+3*t1*aj1sx*aj1syrrs+3*aj1rx*
     & t7*aj1syrss+t7*aj1sx*aj1sysss+3*aj1rx*aj1rxx*aj1syrr+(3*aj1sxx*
     & aj1rx+3*aj1sx*aj1rxx)*aj1syrs+3*aj1sxx*aj1sx*aj1syss+aj1rxxx*
     & aj1syr+aj1sxxx*aj1sys
             t1 = aj1rx**2
             t10 = aj1sx**2
             aj1syxxy = aj1ry*t1*aj1syrrr+(aj1sy*t1+2*aj1ry*aj1sx*
     & aj1rx)*aj1syrrs+(aj1ry*t10+2*aj1sy*aj1sx*aj1rx)*aj1syrss+aj1sy*
     & t10*aj1sysss+(2*aj1rxy*aj1rx+aj1ry*aj1rxx)*aj1syrr+(aj1ry*
     & aj1sxx+2*aj1sx*aj1rxy+2*aj1sxy*aj1rx+aj1sy*aj1rxx)*aj1syrs+(
     & aj1sy*aj1sxx+2*aj1sxy*aj1sx)*aj1syss+aj1rxxy*aj1syr+aj1sxxy*
     & aj1sys
             t1 = aj1ry**2
             t4 = aj1sy*aj1ry
             t8 = aj1sy*aj1rx+aj1ry*aj1sx
             t16 = aj1sy**2
             aj1syxyy = t1*aj1rx*aj1syrrr+(t4*aj1rx+aj1ry*t8)*aj1syrrs+
     & (t4*aj1sx+aj1sy*t8)*aj1syrss+t16*aj1sx*aj1sysss+(aj1ryy*aj1rx+
     & 2*aj1ry*aj1rxy)*aj1syrr+(2*aj1ry*aj1sxy+2*aj1sy*aj1rxy+aj1ryy*
     & aj1sx+aj1syy*aj1rx)*aj1syrs+(aj1syy*aj1sx+2*aj1sy*aj1sxy)*
     & aj1syss+aj1rxyy*aj1syr+aj1sxyy*aj1sys
             t1 = aj1ry**2
             t7 = aj1sy**2
             aj1syyyy = aj1ry*t1*aj1syrrr+3*t1*aj1sy*aj1syrrs+3*aj1ry*
     & t7*aj1syrss+t7*aj1sy*aj1sysss+3*aj1ry*aj1ryy*aj1syrr+(3*aj1syy*
     & aj1ry+3*aj1sy*aj1ryy)*aj1syrs+3*aj1syy*aj1sy*aj1syss+aj1ryyy*
     & aj1syr+aj1syyy*aj1sys
              uu1 = u1(i1,i2,i3,ex)
              uu1r = (-u1(i1-1,i2,i3,ex)+u1(i1+1,i2,i3,ex))/(2.*dr1(0))
              uu1s = (-u1(i1,i2-1,i3,ex)+u1(i1,i2+1,i3,ex))/(2.*dr1(1))
              uu1rr = (u1(i1-1,i2,i3,ex)-2.*u1(i1,i2,i3,ex)+u1(i1+1,i2,
     & i3,ex))/(dr1(0)**2)
              uu1rs = (-(-u1(i1-1,i2-1,i3,ex)+u1(i1-1,i2+1,i3,ex))/(2.*
     & dr1(1))+(-u1(i1+1,i2-1,i3,ex)+u1(i1+1,i2+1,i3,ex))/(2.*dr1(1)))
     & /(2.*dr1(0))
              uu1ss = (u1(i1,i2-1,i3,ex)-2.*u1(i1,i2,i3,ex)+u1(i1,i2+1,
     & i3,ex))/(dr1(1)**2)
              uu1rrr = (-u1(i1-2,i2,i3,ex)+2.*u1(i1-1,i2,i3,ex)-2.*u1(
     & i1+1,i2,i3,ex)+u1(i1+2,i2,i3,ex))/(2.*dr1(0)**3)
              uu1rrs = ((-u1(i1-1,i2-1,i3,ex)+u1(i1-1,i2+1,i3,ex))/(2.*
     & dr1(1))-2.*(-u1(i1,i2-1,i3,ex)+u1(i1,i2+1,i3,ex))/(2.*dr1(1))+(
     & -u1(i1+1,i2-1,i3,ex)+u1(i1+1,i2+1,i3,ex))/(2.*dr1(1)))/(dr1(0)*
     & *2)
              uu1rss = (-(u1(i1-1,i2-1,i3,ex)-2.*u1(i1-1,i2,i3,ex)+u1(
     & i1-1,i2+1,i3,ex))/(dr1(1)**2)+(u1(i1+1,i2-1,i3,ex)-2.*u1(i1+1,
     & i2,i3,ex)+u1(i1+1,i2+1,i3,ex))/(dr1(1)**2))/(2.*dr1(0))
              uu1sss = (-u1(i1,i2-2,i3,ex)+2.*u1(i1,i2-1,i3,ex)-2.*u1(
     & i1,i2+1,i3,ex)+u1(i1,i2+2,i3,ex))/(2.*dr1(1)**3)
              uu1rrrr = (u1(i1-2,i2,i3,ex)-4.*u1(i1-1,i2,i3,ex)+6.*u1(
     & i1,i2,i3,ex)-4.*u1(i1+1,i2,i3,ex)+u1(i1+2,i2,i3,ex))/(dr1(0)**
     & 4)
              uu1rrrs = (-(-u1(i1-2,i2-1,i3,ex)+u1(i1-2,i2+1,i3,ex))/(
     & 2.*dr1(1))+2.*(-u1(i1-1,i2-1,i3,ex)+u1(i1-1,i2+1,i3,ex))/(2.*
     & dr1(1))-2.*(-u1(i1+1,i2-1,i3,ex)+u1(i1+1,i2+1,i3,ex))/(2.*dr1(
     & 1))+(-u1(i1+2,i2-1,i3,ex)+u1(i1+2,i2+1,i3,ex))/(2.*dr1(1)))/(
     & 2.*dr1(0)**3)
              uu1rrss = ((u1(i1-1,i2-1,i3,ex)-2.*u1(i1-1,i2,i3,ex)+u1(
     & i1-1,i2+1,i3,ex))/(dr1(1)**2)-2.*(u1(i1,i2-1,i3,ex)-2.*u1(i1,
     & i2,i3,ex)+u1(i1,i2+1,i3,ex))/(dr1(1)**2)+(u1(i1+1,i2-1,i3,ex)-
     & 2.*u1(i1+1,i2,i3,ex)+u1(i1+1,i2+1,i3,ex))/(dr1(1)**2))/(dr1(0)*
     & *2)
              uu1rsss = (-(-u1(i1-1,i2-2,i3,ex)+2.*u1(i1-1,i2-1,i3,ex)-
     & 2.*u1(i1-1,i2+1,i3,ex)+u1(i1-1,i2+2,i3,ex))/(2.*dr1(1)**3)+(-
     & u1(i1+1,i2-2,i3,ex)+2.*u1(i1+1,i2-1,i3,ex)-2.*u1(i1+1,i2+1,i3,
     & ex)+u1(i1+1,i2+2,i3,ex))/(2.*dr1(1)**3))/(2.*dr1(0))
              uu1ssss = (u1(i1,i2-2,i3,ex)-4.*u1(i1,i2-1,i3,ex)+6.*u1(
     & i1,i2,i3,ex)-4.*u1(i1,i2+1,i3,ex)+u1(i1,i2+2,i3,ex))/(dr1(1)**
     & 4)
               t1 = aj1rx**2
               t7 = aj1sx**2
               u1xxx = t1*aj1rx*uu1rrr+3*t1*aj1sx*uu1rrs+3*aj1rx*t7*
     & uu1rss+t7*aj1sx*uu1sss+3*aj1rx*aj1rxx*uu1rr+(3*aj1sxx*aj1rx+3*
     & aj1sx*aj1rxx)*uu1rs+3*aj1sxx*aj1sx*uu1ss+aj1rxxx*uu1r+aj1sxxx*
     & uu1s
               t1 = aj1rx**2
               t10 = aj1sx**2
               u1xxy = aj1ry*t1*uu1rrr+(aj1sy*t1+2*aj1ry*aj1sx*aj1rx)*
     & uu1rrs+(aj1ry*t10+2*aj1sy*aj1sx*aj1rx)*uu1rss+aj1sy*t10*uu1sss+
     & (2*aj1rxy*aj1rx+aj1ry*aj1rxx)*uu1rr+(aj1ry*aj1sxx+2*aj1sx*
     & aj1rxy+2*aj1sxy*aj1rx+aj1sy*aj1rxx)*uu1rs+(aj1sy*aj1sxx+2*
     & aj1sxy*aj1sx)*uu1ss+aj1rxxy*uu1r+aj1sxxy*uu1s
               t1 = aj1ry**2
               t4 = aj1sy*aj1ry
               t8 = aj1sy*aj1rx+aj1ry*aj1sx
               t16 = aj1sy**2
               u1xyy = t1*aj1rx*uu1rrr+(t4*aj1rx+aj1ry*t8)*uu1rrs+(t4*
     & aj1sx+aj1sy*t8)*uu1rss+t16*aj1sx*uu1sss+(aj1ryy*aj1rx+2*aj1ry*
     & aj1rxy)*uu1rr+(2*aj1ry*aj1sxy+2*aj1sy*aj1rxy+aj1ryy*aj1sx+
     & aj1syy*aj1rx)*uu1rs+(aj1syy*aj1sx+2*aj1sy*aj1sxy)*uu1ss+
     & aj1rxyy*uu1r+aj1sxyy*uu1s
               t1 = aj1ry**2
               t7 = aj1sy**2
               u1yyy = aj1ry*t1*uu1rrr+3*t1*aj1sy*uu1rrs+3*aj1ry*t7*
     & uu1rss+t7*aj1sy*uu1sss+3*aj1ry*aj1ryy*uu1rr+(3*aj1syy*aj1ry+3*
     & aj1sy*aj1ryy)*uu1rs+3*aj1syy*aj1sy*uu1ss+aj1ryyy*uu1r+aj1syyy*
     & uu1s
               t1 = aj1rx**2
               t2 = t1**2
               t8 = aj1sx**2
               t16 = t8**2
               t25 = aj1sxx*aj1rx
               t27 = t25+aj1sx*aj1rxx
               t28 = 3*t27
               t30 = 2*t27
               t46 = aj1rxx**2
               t60 = aj1sxx**2
               u1xxxx = t2*uu1rrrr+4*t1*aj1rx*aj1sx*uu1rrrs+6*t1*t8*
     & uu1rrss+4*aj1rx*t8*aj1sx*uu1rsss+t16*uu1ssss+6*t1*aj1rxx*
     & uu1rrr+(7*aj1sx*aj1rx*aj1rxx+aj1sxx*t1+aj1rx*t28+aj1rx*t30)*
     & uu1rrs+(aj1sx*t28+7*t25*aj1sx+aj1rxx*t8+aj1sx*t30)*uu1rss+6*t8*
     & aj1sxx*uu1sss+(4*aj1rx*aj1rxxx+3*t46)*uu1rr+(4*aj1sxxx*aj1rx+4*
     & aj1sx*aj1rxxx+6*aj1sxx*aj1rxx)*uu1rs+(4*aj1sxxx*aj1sx+3*t60)*
     & uu1ss+aj1rxxxx*uu1r+aj1sxxxx*uu1s
               t1 = aj1ry**2
               t2 = aj1rx**2
               t5 = aj1sy*aj1ry
               t11 = aj1sy*t2+2*aj1ry*aj1sx*aj1rx
               t16 = aj1sx**2
               t21 = aj1ry*t16+2*aj1sy*aj1sx*aj1rx
               t29 = aj1sy**2
               t38 = 2*aj1rxy*aj1rx+aj1ry*aj1rxx
               t52 = aj1sx*aj1rxy
               t54 = aj1sxy*aj1rx
               t57 = aj1ry*aj1sxx+2*t52+2*t54+aj1sy*aj1rxx
               t60 = 2*t52+2*t54
               t68 = aj1sy*aj1sxx+2*aj1sxy*aj1sx
               t92 = aj1rxy**2
               t110 = aj1sxy**2
               u1xxyy = t1*t2*uu1rrrr+(t5*t2+aj1ry*t11)*uu1rrrs+(aj1sy*
     & t11+aj1ry*t21)*uu1rrss+(aj1sy*t21+t5*t16)*uu1rsss+t29*t16*
     & uu1ssss+(2*aj1ry*aj1rxy*aj1rx+aj1ry*t38+aj1ryy*t2)*uu1rrr+(
     & aj1sy*t38+2*aj1sy*aj1rxy*aj1rx+2*aj1ryy*aj1sx*aj1rx+aj1syy*t2+
     & aj1ry*t57+aj1ry*t60)*uu1rrs+(aj1sy*t57+aj1ry*t68+aj1ryy*t16+2*
     & aj1ry*aj1sxy*aj1sx+2*aj1syy*aj1sx*aj1rx+aj1sy*t60)*uu1rss+(2*
     & aj1sy*aj1sxy*aj1sx+aj1sy*t68+aj1syy*t16)*uu1sss+(2*aj1rx*
     & aj1rxyy+aj1ryy*aj1rxx+2*aj1ry*aj1rxxy+2*t92)*uu1rr+(4*aj1sxy*
     & aj1rxy+2*aj1ry*aj1sxxy+aj1ryy*aj1sxx+2*aj1sy*aj1rxxy+2*aj1sxyy*
     & aj1rx+aj1syy*aj1rxx+2*aj1sx*aj1rxyy)*uu1rs+(2*t110+2*aj1sy*
     & aj1sxxy+aj1syy*aj1sxx+2*aj1sx*aj1sxyy)*uu1ss+aj1rxxyy*uu1r+
     & aj1sxxyy*uu1s
               t1 = aj1ry**2
               t2 = t1**2
               t8 = aj1sy**2
               t16 = t8**2
               t25 = aj1syy*aj1ry
               t27 = t25+aj1sy*aj1ryy
               t28 = 3*t27
               t30 = 2*t27
               t46 = aj1ryy**2
               t60 = aj1syy**2
               u1yyyy = t2*uu1rrrr+4*t1*aj1ry*aj1sy*uu1rrrs+6*t1*t8*
     & uu1rrss+4*aj1ry*t8*aj1sy*uu1rsss+t16*uu1ssss+6*t1*aj1ryy*
     & uu1rrr+(7*aj1sy*aj1ry*aj1ryy+aj1syy*t1+aj1ry*t28+aj1ry*t30)*
     & uu1rrs+(aj1sy*t28+7*t25*aj1sy+aj1ryy*t8+aj1sy*t30)*uu1rss+6*t8*
     & aj1syy*uu1sss+(4*aj1ry*aj1ryyy+3*t46)*uu1rr+(4*aj1syyy*aj1ry+4*
     & aj1sy*aj1ryyy+6*aj1syy*aj1ryy)*uu1rs+(4*aj1syyy*aj1sy+3*t60)*
     & uu1ss+aj1ryyyy*uu1r+aj1syyyy*uu1s
             u1LapSq = u1xxxx +2.* u1xxyy + u1yyyy
              vv1 = u1(i1,i2,i3,ey)
              vv1r = (-u1(i1-1,i2,i3,ey)+u1(i1+1,i2,i3,ey))/(2.*dr1(0))
              vv1s = (-u1(i1,i2-1,i3,ey)+u1(i1,i2+1,i3,ey))/(2.*dr1(1))
              vv1rr = (u1(i1-1,i2,i3,ey)-2.*u1(i1,i2,i3,ey)+u1(i1+1,i2,
     & i3,ey))/(dr1(0)**2)
              vv1rs = (-(-u1(i1-1,i2-1,i3,ey)+u1(i1-1,i2+1,i3,ey))/(2.*
     & dr1(1))+(-u1(i1+1,i2-1,i3,ey)+u1(i1+1,i2+1,i3,ey))/(2.*dr1(1)))
     & /(2.*dr1(0))
              vv1ss = (u1(i1,i2-1,i3,ey)-2.*u1(i1,i2,i3,ey)+u1(i1,i2+1,
     & i3,ey))/(dr1(1)**2)
              vv1rrr = (-u1(i1-2,i2,i3,ey)+2.*u1(i1-1,i2,i3,ey)-2.*u1(
     & i1+1,i2,i3,ey)+u1(i1+2,i2,i3,ey))/(2.*dr1(0)**3)
              vv1rrs = ((-u1(i1-1,i2-1,i3,ey)+u1(i1-1,i2+1,i3,ey))/(2.*
     & dr1(1))-2.*(-u1(i1,i2-1,i3,ey)+u1(i1,i2+1,i3,ey))/(2.*dr1(1))+(
     & -u1(i1+1,i2-1,i3,ey)+u1(i1+1,i2+1,i3,ey))/(2.*dr1(1)))/(dr1(0)*
     & *2)
              vv1rss = (-(u1(i1-1,i2-1,i3,ey)-2.*u1(i1-1,i2,i3,ey)+u1(
     & i1-1,i2+1,i3,ey))/(dr1(1)**2)+(u1(i1+1,i2-1,i3,ey)-2.*u1(i1+1,
     & i2,i3,ey)+u1(i1+1,i2+1,i3,ey))/(dr1(1)**2))/(2.*dr1(0))
              vv1sss = (-u1(i1,i2-2,i3,ey)+2.*u1(i1,i2-1,i3,ey)-2.*u1(
     & i1,i2+1,i3,ey)+u1(i1,i2+2,i3,ey))/(2.*dr1(1)**3)
              vv1rrrr = (u1(i1-2,i2,i3,ey)-4.*u1(i1-1,i2,i3,ey)+6.*u1(
     & i1,i2,i3,ey)-4.*u1(i1+1,i2,i3,ey)+u1(i1+2,i2,i3,ey))/(dr1(0)**
     & 4)
              vv1rrrs = (-(-u1(i1-2,i2-1,i3,ey)+u1(i1-2,i2+1,i3,ey))/(
     & 2.*dr1(1))+2.*(-u1(i1-1,i2-1,i3,ey)+u1(i1-1,i2+1,i3,ey))/(2.*
     & dr1(1))-2.*(-u1(i1+1,i2-1,i3,ey)+u1(i1+1,i2+1,i3,ey))/(2.*dr1(
     & 1))+(-u1(i1+2,i2-1,i3,ey)+u1(i1+2,i2+1,i3,ey))/(2.*dr1(1)))/(
     & 2.*dr1(0)**3)
              vv1rrss = ((u1(i1-1,i2-1,i3,ey)-2.*u1(i1-1,i2,i3,ey)+u1(
     & i1-1,i2+1,i3,ey))/(dr1(1)**2)-2.*(u1(i1,i2-1,i3,ey)-2.*u1(i1,
     & i2,i3,ey)+u1(i1,i2+1,i3,ey))/(dr1(1)**2)+(u1(i1+1,i2-1,i3,ey)-
     & 2.*u1(i1+1,i2,i3,ey)+u1(i1+1,i2+1,i3,ey))/(dr1(1)**2))/(dr1(0)*
     & *2)
              vv1rsss = (-(-u1(i1-1,i2-2,i3,ey)+2.*u1(i1-1,i2-1,i3,ey)-
     & 2.*u1(i1-1,i2+1,i3,ey)+u1(i1-1,i2+2,i3,ey))/(2.*dr1(1)**3)+(-
     & u1(i1+1,i2-2,i3,ey)+2.*u1(i1+1,i2-1,i3,ey)-2.*u1(i1+1,i2+1,i3,
     & ey)+u1(i1+1,i2+2,i3,ey))/(2.*dr1(1)**3))/(2.*dr1(0))
              vv1ssss = (u1(i1,i2-2,i3,ey)-4.*u1(i1,i2-1,i3,ey)+6.*u1(
     & i1,i2,i3,ey)-4.*u1(i1,i2+1,i3,ey)+u1(i1,i2+2,i3,ey))/(dr1(1)**
     & 4)
               t1 = aj1rx**2
               t7 = aj1sx**2
               v1xxx = t1*aj1rx*vv1rrr+3*t1*aj1sx*vv1rrs+3*aj1rx*t7*
     & vv1rss+t7*aj1sx*vv1sss+3*aj1rx*aj1rxx*vv1rr+(3*aj1sxx*aj1rx+3*
     & aj1sx*aj1rxx)*vv1rs+3*aj1sxx*aj1sx*vv1ss+aj1rxxx*vv1r+aj1sxxx*
     & vv1s
               t1 = aj1rx**2
               t10 = aj1sx**2
               v1xxy = aj1ry*t1*vv1rrr+(aj1sy*t1+2*aj1ry*aj1sx*aj1rx)*
     & vv1rrs+(aj1ry*t10+2*aj1sy*aj1sx*aj1rx)*vv1rss+aj1sy*t10*vv1sss+
     & (2*aj1rxy*aj1rx+aj1ry*aj1rxx)*vv1rr+(aj1ry*aj1sxx+2*aj1sx*
     & aj1rxy+2*aj1sxy*aj1rx+aj1sy*aj1rxx)*vv1rs+(aj1sy*aj1sxx+2*
     & aj1sxy*aj1sx)*vv1ss+aj1rxxy*vv1r+aj1sxxy*vv1s
               t1 = aj1ry**2
               t4 = aj1sy*aj1ry
               t8 = aj1sy*aj1rx+aj1ry*aj1sx
               t16 = aj1sy**2
               v1xyy = t1*aj1rx*vv1rrr+(t4*aj1rx+aj1ry*t8)*vv1rrs+(t4*
     & aj1sx+aj1sy*t8)*vv1rss+t16*aj1sx*vv1sss+(aj1ryy*aj1rx+2*aj1ry*
     & aj1rxy)*vv1rr+(2*aj1ry*aj1sxy+2*aj1sy*aj1rxy+aj1ryy*aj1sx+
     & aj1syy*aj1rx)*vv1rs+(aj1syy*aj1sx+2*aj1sy*aj1sxy)*vv1ss+
     & aj1rxyy*vv1r+aj1sxyy*vv1s
               t1 = aj1ry**2
               t7 = aj1sy**2
               v1yyy = aj1ry*t1*vv1rrr+3*t1*aj1sy*vv1rrs+3*aj1ry*t7*
     & vv1rss+t7*aj1sy*vv1sss+3*aj1ry*aj1ryy*vv1rr+(3*aj1syy*aj1ry+3*
     & aj1sy*aj1ryy)*vv1rs+3*aj1syy*aj1sy*vv1ss+aj1ryyy*vv1r+aj1syyy*
     & vv1s
               t1 = aj1rx**2
               t2 = t1**2
               t8 = aj1sx**2
               t16 = t8**2
               t25 = aj1sxx*aj1rx
               t27 = t25+aj1sx*aj1rxx
               t28 = 3*t27
               t30 = 2*t27
               t46 = aj1rxx**2
               t60 = aj1sxx**2
               v1xxxx = t2*vv1rrrr+4*t1*aj1rx*aj1sx*vv1rrrs+6*t1*t8*
     & vv1rrss+4*aj1rx*t8*aj1sx*vv1rsss+t16*vv1ssss+6*t1*aj1rxx*
     & vv1rrr+(7*aj1sx*aj1rx*aj1rxx+aj1sxx*t1+aj1rx*t28+aj1rx*t30)*
     & vv1rrs+(aj1sx*t28+7*t25*aj1sx+aj1rxx*t8+aj1sx*t30)*vv1rss+6*t8*
     & aj1sxx*vv1sss+(4*aj1rx*aj1rxxx+3*t46)*vv1rr+(4*aj1sxxx*aj1rx+4*
     & aj1sx*aj1rxxx+6*aj1sxx*aj1rxx)*vv1rs+(4*aj1sxxx*aj1sx+3*t60)*
     & vv1ss+aj1rxxxx*vv1r+aj1sxxxx*vv1s
               t1 = aj1ry**2
               t2 = aj1rx**2
               t5 = aj1sy*aj1ry
               t11 = aj1sy*t2+2*aj1ry*aj1sx*aj1rx
               t16 = aj1sx**2
               t21 = aj1ry*t16+2*aj1sy*aj1sx*aj1rx
               t29 = aj1sy**2
               t38 = 2*aj1rxy*aj1rx+aj1ry*aj1rxx
               t52 = aj1sx*aj1rxy
               t54 = aj1sxy*aj1rx
               t57 = aj1ry*aj1sxx+2*t52+2*t54+aj1sy*aj1rxx
               t60 = 2*t52+2*t54
               t68 = aj1sy*aj1sxx+2*aj1sxy*aj1sx
               t92 = aj1rxy**2
               t110 = aj1sxy**2
               v1xxyy = t1*t2*vv1rrrr+(t5*t2+aj1ry*t11)*vv1rrrs+(aj1sy*
     & t11+aj1ry*t21)*vv1rrss+(aj1sy*t21+t5*t16)*vv1rsss+t29*t16*
     & vv1ssss+(2*aj1ry*aj1rxy*aj1rx+aj1ry*t38+aj1ryy*t2)*vv1rrr+(
     & aj1sy*t38+2*aj1sy*aj1rxy*aj1rx+2*aj1ryy*aj1sx*aj1rx+aj1syy*t2+
     & aj1ry*t57+aj1ry*t60)*vv1rrs+(aj1sy*t57+aj1ry*t68+aj1ryy*t16+2*
     & aj1ry*aj1sxy*aj1sx+2*aj1syy*aj1sx*aj1rx+aj1sy*t60)*vv1rss+(2*
     & aj1sy*aj1sxy*aj1sx+aj1sy*t68+aj1syy*t16)*vv1sss+(2*aj1rx*
     & aj1rxyy+aj1ryy*aj1rxx+2*aj1ry*aj1rxxy+2*t92)*vv1rr+(4*aj1sxy*
     & aj1rxy+2*aj1ry*aj1sxxy+aj1ryy*aj1sxx+2*aj1sy*aj1rxxy+2*aj1sxyy*
     & aj1rx+aj1syy*aj1rxx+2*aj1sx*aj1rxyy)*vv1rs+(2*t110+2*aj1sy*
     & aj1sxxy+aj1syy*aj1sxx+2*aj1sx*aj1sxyy)*vv1ss+aj1rxxyy*vv1r+
     & aj1sxxyy*vv1s
               t1 = aj1ry**2
               t2 = t1**2
               t8 = aj1sy**2
               t16 = t8**2
               t25 = aj1syy*aj1ry
               t27 = t25+aj1sy*aj1ryy
               t28 = 3*t27
               t30 = 2*t27
               t46 = aj1ryy**2
               t60 = aj1syy**2
               v1yyyy = t2*vv1rrrr+4*t1*aj1ry*aj1sy*vv1rrrs+6*t1*t8*
     & vv1rrss+4*aj1ry*t8*aj1sy*vv1rsss+t16*vv1ssss+6*t1*aj1ryy*
     & vv1rrr+(7*aj1sy*aj1ry*aj1ryy+aj1syy*t1+aj1ry*t28+aj1ry*t30)*
     & vv1rrs+(aj1sy*t28+7*t25*aj1sy+aj1ryy*t8+aj1sy*t30)*vv1rss+6*t8*
     & aj1syy*vv1sss+(4*aj1ry*aj1ryyy+3*t46)*vv1rr+(4*aj1syyy*aj1ry+4*
     & aj1sy*aj1ryyy+6*aj1syy*aj1ryy)*vv1rs+(4*aj1syyy*aj1sy+3*t60)*
     & vv1ss+aj1ryyyy*vv1r+aj1syyyy*vv1s
             v1LapSq = v1xxxx +2.* v1xxyy + v1yyyy
            ! NOTE: the jacobian derivatives can be computed once for all components
             ! this next call will define the jacobian and its derivatives (parameteric and spatial)
             aj2rx = rsxy2(j1,j2,j3,0,0)
             aj2rxr = (-rsxy2(j1-1,j2,j3,0,0)+rsxy2(j1+1,j2,j3,0,0))/(
     & 2.*dr2(0))
             aj2rxs = (-rsxy2(j1,j2-1,j3,0,0)+rsxy2(j1,j2+1,j3,0,0))/(
     & 2.*dr2(1))
             aj2rxrr = (rsxy2(j1-1,j2,j3,0,0)-2.*rsxy2(j1,j2,j3,0,0)+
     & rsxy2(j1+1,j2,j3,0,0))/(dr2(0)**2)
             aj2rxrs = (-(-rsxy2(j1-1,j2-1,j3,0,0)+rsxy2(j1-1,j2+1,j3,
     & 0,0))/(2.*dr2(1))+(-rsxy2(j1+1,j2-1,j3,0,0)+rsxy2(j1+1,j2+1,j3,
     & 0,0))/(2.*dr2(1)))/(2.*dr2(0))
             aj2rxss = (rsxy2(j1,j2-1,j3,0,0)-2.*rsxy2(j1,j2,j3,0,0)+
     & rsxy2(j1,j2+1,j3,0,0))/(dr2(1)**2)
             aj2rxrrr = (-rsxy2(j1-2,j2,j3,0,0)+2.*rsxy2(j1-1,j2,j3,0,
     & 0)-2.*rsxy2(j1+1,j2,j3,0,0)+rsxy2(j1+2,j2,j3,0,0))/(2.*dr2(0)**
     & 3)
             aj2rxrrs = ((-rsxy2(j1-1,j2-1,j3,0,0)+rsxy2(j1-1,j2+1,j3,
     & 0,0))/(2.*dr2(1))-2.*(-rsxy2(j1,j2-1,j3,0,0)+rsxy2(j1,j2+1,j3,
     & 0,0))/(2.*dr2(1))+(-rsxy2(j1+1,j2-1,j3,0,0)+rsxy2(j1+1,j2+1,j3,
     & 0,0))/(2.*dr2(1)))/(dr2(0)**2)
             aj2rxrss = (-(rsxy2(j1-1,j2-1,j3,0,0)-2.*rsxy2(j1-1,j2,j3,
     & 0,0)+rsxy2(j1-1,j2+1,j3,0,0))/(dr2(1)**2)+(rsxy2(j1+1,j2-1,j3,
     & 0,0)-2.*rsxy2(j1+1,j2,j3,0,0)+rsxy2(j1+1,j2+1,j3,0,0))/(dr2(1)*
     & *2))/(2.*dr2(0))
             aj2rxsss = (-rsxy2(j1,j2-2,j3,0,0)+2.*rsxy2(j1,j2-1,j3,0,
     & 0)-2.*rsxy2(j1,j2+1,j3,0,0)+rsxy2(j1,j2+2,j3,0,0))/(2.*dr2(1)**
     & 3)
             aj2sx = rsxy2(j1,j2,j3,1,0)
             aj2sxr = (-rsxy2(j1-1,j2,j3,1,0)+rsxy2(j1+1,j2,j3,1,0))/(
     & 2.*dr2(0))
             aj2sxs = (-rsxy2(j1,j2-1,j3,1,0)+rsxy2(j1,j2+1,j3,1,0))/(
     & 2.*dr2(1))
             aj2sxrr = (rsxy2(j1-1,j2,j3,1,0)-2.*rsxy2(j1,j2,j3,1,0)+
     & rsxy2(j1+1,j2,j3,1,0))/(dr2(0)**2)
             aj2sxrs = (-(-rsxy2(j1-1,j2-1,j3,1,0)+rsxy2(j1-1,j2+1,j3,
     & 1,0))/(2.*dr2(1))+(-rsxy2(j1+1,j2-1,j3,1,0)+rsxy2(j1+1,j2+1,j3,
     & 1,0))/(2.*dr2(1)))/(2.*dr2(0))
             aj2sxss = (rsxy2(j1,j2-1,j3,1,0)-2.*rsxy2(j1,j2,j3,1,0)+
     & rsxy2(j1,j2+1,j3,1,0))/(dr2(1)**2)
             aj2sxrrr = (-rsxy2(j1-2,j2,j3,1,0)+2.*rsxy2(j1-1,j2,j3,1,
     & 0)-2.*rsxy2(j1+1,j2,j3,1,0)+rsxy2(j1+2,j2,j3,1,0))/(2.*dr2(0)**
     & 3)
             aj2sxrrs = ((-rsxy2(j1-1,j2-1,j3,1,0)+rsxy2(j1-1,j2+1,j3,
     & 1,0))/(2.*dr2(1))-2.*(-rsxy2(j1,j2-1,j3,1,0)+rsxy2(j1,j2+1,j3,
     & 1,0))/(2.*dr2(1))+(-rsxy2(j1+1,j2-1,j3,1,0)+rsxy2(j1+1,j2+1,j3,
     & 1,0))/(2.*dr2(1)))/(dr2(0)**2)
             aj2sxrss = (-(rsxy2(j1-1,j2-1,j3,1,0)-2.*rsxy2(j1-1,j2,j3,
     & 1,0)+rsxy2(j1-1,j2+1,j3,1,0))/(dr2(1)**2)+(rsxy2(j1+1,j2-1,j3,
     & 1,0)-2.*rsxy2(j1+1,j2,j3,1,0)+rsxy2(j1+1,j2+1,j3,1,0))/(dr2(1)*
     & *2))/(2.*dr2(0))
             aj2sxsss = (-rsxy2(j1,j2-2,j3,1,0)+2.*rsxy2(j1,j2-1,j3,1,
     & 0)-2.*rsxy2(j1,j2+1,j3,1,0)+rsxy2(j1,j2+2,j3,1,0))/(2.*dr2(1)**
     & 3)
             aj2ry = rsxy2(j1,j2,j3,0,1)
             aj2ryr = (-rsxy2(j1-1,j2,j3,0,1)+rsxy2(j1+1,j2,j3,0,1))/(
     & 2.*dr2(0))
             aj2rys = (-rsxy2(j1,j2-1,j3,0,1)+rsxy2(j1,j2+1,j3,0,1))/(
     & 2.*dr2(1))
             aj2ryrr = (rsxy2(j1-1,j2,j3,0,1)-2.*rsxy2(j1,j2,j3,0,1)+
     & rsxy2(j1+1,j2,j3,0,1))/(dr2(0)**2)
             aj2ryrs = (-(-rsxy2(j1-1,j2-1,j3,0,1)+rsxy2(j1-1,j2+1,j3,
     & 0,1))/(2.*dr2(1))+(-rsxy2(j1+1,j2-1,j3,0,1)+rsxy2(j1+1,j2+1,j3,
     & 0,1))/(2.*dr2(1)))/(2.*dr2(0))
             aj2ryss = (rsxy2(j1,j2-1,j3,0,1)-2.*rsxy2(j1,j2,j3,0,1)+
     & rsxy2(j1,j2+1,j3,0,1))/(dr2(1)**2)
             aj2ryrrr = (-rsxy2(j1-2,j2,j3,0,1)+2.*rsxy2(j1-1,j2,j3,0,
     & 1)-2.*rsxy2(j1+1,j2,j3,0,1)+rsxy2(j1+2,j2,j3,0,1))/(2.*dr2(0)**
     & 3)
             aj2ryrrs = ((-rsxy2(j1-1,j2-1,j3,0,1)+rsxy2(j1-1,j2+1,j3,
     & 0,1))/(2.*dr2(1))-2.*(-rsxy2(j1,j2-1,j3,0,1)+rsxy2(j1,j2+1,j3,
     & 0,1))/(2.*dr2(1))+(-rsxy2(j1+1,j2-1,j3,0,1)+rsxy2(j1+1,j2+1,j3,
     & 0,1))/(2.*dr2(1)))/(dr2(0)**2)
             aj2ryrss = (-(rsxy2(j1-1,j2-1,j3,0,1)-2.*rsxy2(j1-1,j2,j3,
     & 0,1)+rsxy2(j1-1,j2+1,j3,0,1))/(dr2(1)**2)+(rsxy2(j1+1,j2-1,j3,
     & 0,1)-2.*rsxy2(j1+1,j2,j3,0,1)+rsxy2(j1+1,j2+1,j3,0,1))/(dr2(1)*
     & *2))/(2.*dr2(0))
             aj2rysss = (-rsxy2(j1,j2-2,j3,0,1)+2.*rsxy2(j1,j2-1,j3,0,
     & 1)-2.*rsxy2(j1,j2+1,j3,0,1)+rsxy2(j1,j2+2,j3,0,1))/(2.*dr2(1)**
     & 3)
             aj2sy = rsxy2(j1,j2,j3,1,1)
             aj2syr = (-rsxy2(j1-1,j2,j3,1,1)+rsxy2(j1+1,j2,j3,1,1))/(
     & 2.*dr2(0))
             aj2sys = (-rsxy2(j1,j2-1,j3,1,1)+rsxy2(j1,j2+1,j3,1,1))/(
     & 2.*dr2(1))
             aj2syrr = (rsxy2(j1-1,j2,j3,1,1)-2.*rsxy2(j1,j2,j3,1,1)+
     & rsxy2(j1+1,j2,j3,1,1))/(dr2(0)**2)
             aj2syrs = (-(-rsxy2(j1-1,j2-1,j3,1,1)+rsxy2(j1-1,j2+1,j3,
     & 1,1))/(2.*dr2(1))+(-rsxy2(j1+1,j2-1,j3,1,1)+rsxy2(j1+1,j2+1,j3,
     & 1,1))/(2.*dr2(1)))/(2.*dr2(0))
             aj2syss = (rsxy2(j1,j2-1,j3,1,1)-2.*rsxy2(j1,j2,j3,1,1)+
     & rsxy2(j1,j2+1,j3,1,1))/(dr2(1)**2)
             aj2syrrr = (-rsxy2(j1-2,j2,j3,1,1)+2.*rsxy2(j1-1,j2,j3,1,
     & 1)-2.*rsxy2(j1+1,j2,j3,1,1)+rsxy2(j1+2,j2,j3,1,1))/(2.*dr2(0)**
     & 3)
             aj2syrrs = ((-rsxy2(j1-1,j2-1,j3,1,1)+rsxy2(j1-1,j2+1,j3,
     & 1,1))/(2.*dr2(1))-2.*(-rsxy2(j1,j2-1,j3,1,1)+rsxy2(j1,j2+1,j3,
     & 1,1))/(2.*dr2(1))+(-rsxy2(j1+1,j2-1,j3,1,1)+rsxy2(j1+1,j2+1,j3,
     & 1,1))/(2.*dr2(1)))/(dr2(0)**2)
             aj2syrss = (-(rsxy2(j1-1,j2-1,j3,1,1)-2.*rsxy2(j1-1,j2,j3,
     & 1,1)+rsxy2(j1-1,j2+1,j3,1,1))/(dr2(1)**2)+(rsxy2(j1+1,j2-1,j3,
     & 1,1)-2.*rsxy2(j1+1,j2,j3,1,1)+rsxy2(j1+1,j2+1,j3,1,1))/(dr2(1)*
     & *2))/(2.*dr2(0))
             aj2sysss = (-rsxy2(j1,j2-2,j3,1,1)+2.*rsxy2(j1,j2-1,j3,1,
     & 1)-2.*rsxy2(j1,j2+1,j3,1,1)+rsxy2(j1,j2+2,j3,1,1))/(2.*dr2(1)**
     & 3)
             aj2rxx = aj2rx*aj2rxr+aj2sx*aj2rxs
             aj2rxy = aj2ry*aj2rxr+aj2sy*aj2rxs
             aj2sxx = aj2rx*aj2sxr+aj2sx*aj2sxs
             aj2sxy = aj2ry*aj2sxr+aj2sy*aj2sxs
             aj2ryx = aj2rx*aj2ryr+aj2sx*aj2rys
             aj2ryy = aj2ry*aj2ryr+aj2sy*aj2rys
             aj2syx = aj2rx*aj2syr+aj2sx*aj2sys
             aj2syy = aj2ry*aj2syr+aj2sy*aj2sys
             t1 = aj2rx**2
             t6 = aj2sx**2
             aj2rxxx = t1*aj2rxrr+2*aj2rx*aj2sx*aj2rxrs+t6*aj2rxss+
     & aj2rxx*aj2rxr+aj2sxx*aj2rxs
             aj2rxxy = aj2ry*aj2rx*aj2rxrr+(aj2sy*aj2rx+aj2ry*aj2sx)*
     & aj2rxrs+aj2sy*aj2sx*aj2rxss+aj2rxy*aj2rxr+aj2sxy*aj2rxs
             t1 = aj2ry**2
             t6 = aj2sy**2
             aj2rxyy = t1*aj2rxrr+2*aj2ry*aj2sy*aj2rxrs+t6*aj2rxss+
     & aj2ryy*aj2rxr+aj2syy*aj2rxs
             t1 = aj2rx**2
             t6 = aj2sx**2
             aj2sxxx = t1*aj2sxrr+2*aj2rx*aj2sx*aj2sxrs+t6*aj2sxss+
     & aj2rxx*aj2sxr+aj2sxx*aj2sxs
             aj2sxxy = aj2ry*aj2rx*aj2sxrr+(aj2sy*aj2rx+aj2ry*aj2sx)*
     & aj2sxrs+aj2sy*aj2sx*aj2sxss+aj2rxy*aj2sxr+aj2sxy*aj2sxs
             t1 = aj2ry**2
             t6 = aj2sy**2
             aj2sxyy = t1*aj2sxrr+2*aj2ry*aj2sy*aj2sxrs+t6*aj2sxss+
     & aj2ryy*aj2sxr+aj2syy*aj2sxs
             t1 = aj2rx**2
             t6 = aj2sx**2
             aj2ryxx = t1*aj2ryrr+2*aj2rx*aj2sx*aj2ryrs+t6*aj2ryss+
     & aj2rxx*aj2ryr+aj2sxx*aj2rys
             aj2ryxy = aj2ry*aj2rx*aj2ryrr+(aj2sy*aj2rx+aj2ry*aj2sx)*
     & aj2ryrs+aj2sy*aj2sx*aj2ryss+aj2rxy*aj2ryr+aj2sxy*aj2rys
             t1 = aj2ry**2
             t6 = aj2sy**2
             aj2ryyy = t1*aj2ryrr+2*aj2ry*aj2sy*aj2ryrs+t6*aj2ryss+
     & aj2ryy*aj2ryr+aj2syy*aj2rys
             t1 = aj2rx**2
             t6 = aj2sx**2
             aj2syxx = t1*aj2syrr+2*aj2rx*aj2sx*aj2syrs+t6*aj2syss+
     & aj2rxx*aj2syr+aj2sxx*aj2sys
             aj2syxy = aj2ry*aj2rx*aj2syrr+(aj2sy*aj2rx+aj2ry*aj2sx)*
     & aj2syrs+aj2sy*aj2sx*aj2syss+aj2rxy*aj2syr+aj2sxy*aj2sys
             t1 = aj2ry**2
             t6 = aj2sy**2
             aj2syyy = t1*aj2syrr+2*aj2ry*aj2sy*aj2syrs+t6*aj2syss+
     & aj2ryy*aj2syr+aj2syy*aj2sys
             t1 = aj2rx**2
             t7 = aj2sx**2
             aj2rxxxx = t1*aj2rx*aj2rxrrr+3*t1*aj2sx*aj2rxrrs+3*aj2rx*
     & t7*aj2rxrss+t7*aj2sx*aj2rxsss+3*aj2rx*aj2rxx*aj2rxrr+(3*aj2sxx*
     & aj2rx+3*aj2sx*aj2rxx)*aj2rxrs+3*aj2sxx*aj2sx*aj2rxss+aj2rxxx*
     & aj2rxr+aj2sxxx*aj2rxs
             t1 = aj2rx**2
             t10 = aj2sx**2
             aj2rxxxy = aj2ry*t1*aj2rxrrr+(aj2sy*t1+2*aj2ry*aj2sx*
     & aj2rx)*aj2rxrrs+(aj2ry*t10+2*aj2sy*aj2sx*aj2rx)*aj2rxrss+aj2sy*
     & t10*aj2rxsss+(2*aj2rxy*aj2rx+aj2ry*aj2rxx)*aj2rxrr+(aj2ry*
     & aj2sxx+2*aj2sx*aj2rxy+2*aj2sxy*aj2rx+aj2sy*aj2rxx)*aj2rxrs+(
     & aj2sy*aj2sxx+2*aj2sxy*aj2sx)*aj2rxss+aj2rxxy*aj2rxr+aj2sxxy*
     & aj2rxs
             t1 = aj2ry**2
             t4 = aj2sy*aj2ry
             t8 = aj2sy*aj2rx+aj2ry*aj2sx
             t16 = aj2sy**2
             aj2rxxyy = t1*aj2rx*aj2rxrrr+(t4*aj2rx+aj2ry*t8)*aj2rxrrs+
     & (t4*aj2sx+aj2sy*t8)*aj2rxrss+t16*aj2sx*aj2rxsss+(aj2ryy*aj2rx+
     & 2*aj2ry*aj2rxy)*aj2rxrr+(2*aj2ry*aj2sxy+2*aj2sy*aj2rxy+aj2ryy*
     & aj2sx+aj2syy*aj2rx)*aj2rxrs+(aj2syy*aj2sx+2*aj2sy*aj2sxy)*
     & aj2rxss+aj2rxyy*aj2rxr+aj2sxyy*aj2rxs
             t1 = aj2ry**2
             t7 = aj2sy**2
             aj2rxyyy = aj2ry*t1*aj2rxrrr+3*t1*aj2sy*aj2rxrrs+3*aj2ry*
     & t7*aj2rxrss+t7*aj2sy*aj2rxsss+3*aj2ry*aj2ryy*aj2rxrr+(3*aj2syy*
     & aj2ry+3*aj2sy*aj2ryy)*aj2rxrs+3*aj2syy*aj2sy*aj2rxss+aj2ryyy*
     & aj2rxr+aj2syyy*aj2rxs
             t1 = aj2rx**2
             t7 = aj2sx**2
             aj2sxxxx = t1*aj2rx*aj2sxrrr+3*t1*aj2sx*aj2sxrrs+3*aj2rx*
     & t7*aj2sxrss+t7*aj2sx*aj2sxsss+3*aj2rx*aj2rxx*aj2sxrr+(3*aj2sxx*
     & aj2rx+3*aj2sx*aj2rxx)*aj2sxrs+3*aj2sxx*aj2sx*aj2sxss+aj2rxxx*
     & aj2sxr+aj2sxxx*aj2sxs
             t1 = aj2rx**2
             t10 = aj2sx**2
             aj2sxxxy = aj2ry*t1*aj2sxrrr+(aj2sy*t1+2*aj2ry*aj2sx*
     & aj2rx)*aj2sxrrs+(aj2ry*t10+2*aj2sy*aj2sx*aj2rx)*aj2sxrss+aj2sy*
     & t10*aj2sxsss+(2*aj2rxy*aj2rx+aj2ry*aj2rxx)*aj2sxrr+(aj2ry*
     & aj2sxx+2*aj2sx*aj2rxy+2*aj2sxy*aj2rx+aj2sy*aj2rxx)*aj2sxrs+(
     & aj2sy*aj2sxx+2*aj2sxy*aj2sx)*aj2sxss+aj2rxxy*aj2sxr+aj2sxxy*
     & aj2sxs
             t1 = aj2ry**2
             t4 = aj2sy*aj2ry
             t8 = aj2sy*aj2rx+aj2ry*aj2sx
             t16 = aj2sy**2
             aj2sxxyy = t1*aj2rx*aj2sxrrr+(t4*aj2rx+aj2ry*t8)*aj2sxrrs+
     & (t4*aj2sx+aj2sy*t8)*aj2sxrss+t16*aj2sx*aj2sxsss+(aj2ryy*aj2rx+
     & 2*aj2ry*aj2rxy)*aj2sxrr+(2*aj2ry*aj2sxy+2*aj2sy*aj2rxy+aj2ryy*
     & aj2sx+aj2syy*aj2rx)*aj2sxrs+(aj2syy*aj2sx+2*aj2sy*aj2sxy)*
     & aj2sxss+aj2rxyy*aj2sxr+aj2sxyy*aj2sxs
             t1 = aj2ry**2
             t7 = aj2sy**2
             aj2sxyyy = aj2ry*t1*aj2sxrrr+3*t1*aj2sy*aj2sxrrs+3*aj2ry*
     & t7*aj2sxrss+t7*aj2sy*aj2sxsss+3*aj2ry*aj2ryy*aj2sxrr+(3*aj2syy*
     & aj2ry+3*aj2sy*aj2ryy)*aj2sxrs+3*aj2syy*aj2sy*aj2sxss+aj2ryyy*
     & aj2sxr+aj2syyy*aj2sxs
             t1 = aj2rx**2
             t7 = aj2sx**2
             aj2ryxxx = t1*aj2rx*aj2ryrrr+3*t1*aj2sx*aj2ryrrs+3*aj2rx*
     & t7*aj2ryrss+t7*aj2sx*aj2rysss+3*aj2rx*aj2rxx*aj2ryrr+(3*aj2sxx*
     & aj2rx+3*aj2sx*aj2rxx)*aj2ryrs+3*aj2sxx*aj2sx*aj2ryss+aj2rxxx*
     & aj2ryr+aj2sxxx*aj2rys
             t1 = aj2rx**2
             t10 = aj2sx**2
             aj2ryxxy = aj2ry*t1*aj2ryrrr+(aj2sy*t1+2*aj2ry*aj2sx*
     & aj2rx)*aj2ryrrs+(aj2ry*t10+2*aj2sy*aj2sx*aj2rx)*aj2ryrss+aj2sy*
     & t10*aj2rysss+(2*aj2rxy*aj2rx+aj2ry*aj2rxx)*aj2ryrr+(aj2ry*
     & aj2sxx+2*aj2sx*aj2rxy+2*aj2sxy*aj2rx+aj2sy*aj2rxx)*aj2ryrs+(
     & aj2sy*aj2sxx+2*aj2sxy*aj2sx)*aj2ryss+aj2rxxy*aj2ryr+aj2sxxy*
     & aj2rys
             t1 = aj2ry**2
             t4 = aj2sy*aj2ry
             t8 = aj2sy*aj2rx+aj2ry*aj2sx
             t16 = aj2sy**2
             aj2ryxyy = t1*aj2rx*aj2ryrrr+(t4*aj2rx+aj2ry*t8)*aj2ryrrs+
     & (t4*aj2sx+aj2sy*t8)*aj2ryrss+t16*aj2sx*aj2rysss+(aj2ryy*aj2rx+
     & 2*aj2ry*aj2rxy)*aj2ryrr+(2*aj2ry*aj2sxy+2*aj2sy*aj2rxy+aj2ryy*
     & aj2sx+aj2syy*aj2rx)*aj2ryrs+(aj2syy*aj2sx+2*aj2sy*aj2sxy)*
     & aj2ryss+aj2rxyy*aj2ryr+aj2sxyy*aj2rys
             t1 = aj2ry**2
             t7 = aj2sy**2
             aj2ryyyy = aj2ry*t1*aj2ryrrr+3*t1*aj2sy*aj2ryrrs+3*aj2ry*
     & t7*aj2ryrss+t7*aj2sy*aj2rysss+3*aj2ry*aj2ryy*aj2ryrr+(3*aj2syy*
     & aj2ry+3*aj2sy*aj2ryy)*aj2ryrs+3*aj2syy*aj2sy*aj2ryss+aj2ryyy*
     & aj2ryr+aj2syyy*aj2rys
             t1 = aj2rx**2
             t7 = aj2sx**2
             aj2syxxx = t1*aj2rx*aj2syrrr+3*t1*aj2sx*aj2syrrs+3*aj2rx*
     & t7*aj2syrss+t7*aj2sx*aj2sysss+3*aj2rx*aj2rxx*aj2syrr+(3*aj2sxx*
     & aj2rx+3*aj2sx*aj2rxx)*aj2syrs+3*aj2sxx*aj2sx*aj2syss+aj2rxxx*
     & aj2syr+aj2sxxx*aj2sys
             t1 = aj2rx**2
             t10 = aj2sx**2
             aj2syxxy = aj2ry*t1*aj2syrrr+(aj2sy*t1+2*aj2ry*aj2sx*
     & aj2rx)*aj2syrrs+(aj2ry*t10+2*aj2sy*aj2sx*aj2rx)*aj2syrss+aj2sy*
     & t10*aj2sysss+(2*aj2rxy*aj2rx+aj2ry*aj2rxx)*aj2syrr+(aj2ry*
     & aj2sxx+2*aj2sx*aj2rxy+2*aj2sxy*aj2rx+aj2sy*aj2rxx)*aj2syrs+(
     & aj2sy*aj2sxx+2*aj2sxy*aj2sx)*aj2syss+aj2rxxy*aj2syr+aj2sxxy*
     & aj2sys
             t1 = aj2ry**2
             t4 = aj2sy*aj2ry
             t8 = aj2sy*aj2rx+aj2ry*aj2sx
             t16 = aj2sy**2
             aj2syxyy = t1*aj2rx*aj2syrrr+(t4*aj2rx+aj2ry*t8)*aj2syrrs+
     & (t4*aj2sx+aj2sy*t8)*aj2syrss+t16*aj2sx*aj2sysss+(aj2ryy*aj2rx+
     & 2*aj2ry*aj2rxy)*aj2syrr+(2*aj2ry*aj2sxy+2*aj2sy*aj2rxy+aj2ryy*
     & aj2sx+aj2syy*aj2rx)*aj2syrs+(aj2syy*aj2sx+2*aj2sy*aj2sxy)*
     & aj2syss+aj2rxyy*aj2syr+aj2sxyy*aj2sys
             t1 = aj2ry**2
             t7 = aj2sy**2
             aj2syyyy = aj2ry*t1*aj2syrrr+3*t1*aj2sy*aj2syrrs+3*aj2ry*
     & t7*aj2syrss+t7*aj2sy*aj2sysss+3*aj2ry*aj2ryy*aj2syrr+(3*aj2syy*
     & aj2ry+3*aj2sy*aj2ryy)*aj2syrs+3*aj2syy*aj2sy*aj2syss+aj2ryyy*
     & aj2syr+aj2syyy*aj2sys
              uu2 = u2(j1,j2,j3,ex)
              uu2r = (-u2(j1-1,j2,j3,ex)+u2(j1+1,j2,j3,ex))/(2.*dr2(0))
              uu2s = (-u2(j1,j2-1,j3,ex)+u2(j1,j2+1,j3,ex))/(2.*dr2(1))
              uu2rr = (u2(j1-1,j2,j3,ex)-2.*u2(j1,j2,j3,ex)+u2(j1+1,j2,
     & j3,ex))/(dr2(0)**2)
              uu2rs = (-(-u2(j1-1,j2-1,j3,ex)+u2(j1-1,j2+1,j3,ex))/(2.*
     & dr2(1))+(-u2(j1+1,j2-1,j3,ex)+u2(j1+1,j2+1,j3,ex))/(2.*dr2(1)))
     & /(2.*dr2(0))
              uu2ss = (u2(j1,j2-1,j3,ex)-2.*u2(j1,j2,j3,ex)+u2(j1,j2+1,
     & j3,ex))/(dr2(1)**2)
              uu2rrr = (-u2(j1-2,j2,j3,ex)+2.*u2(j1-1,j2,j3,ex)-2.*u2(
     & j1+1,j2,j3,ex)+u2(j1+2,j2,j3,ex))/(2.*dr2(0)**3)
              uu2rrs = ((-u2(j1-1,j2-1,j3,ex)+u2(j1-1,j2+1,j3,ex))/(2.*
     & dr2(1))-2.*(-u2(j1,j2-1,j3,ex)+u2(j1,j2+1,j3,ex))/(2.*dr2(1))+(
     & -u2(j1+1,j2-1,j3,ex)+u2(j1+1,j2+1,j3,ex))/(2.*dr2(1)))/(dr2(0)*
     & *2)
              uu2rss = (-(u2(j1-1,j2-1,j3,ex)-2.*u2(j1-1,j2,j3,ex)+u2(
     & j1-1,j2+1,j3,ex))/(dr2(1)**2)+(u2(j1+1,j2-1,j3,ex)-2.*u2(j1+1,
     & j2,j3,ex)+u2(j1+1,j2+1,j3,ex))/(dr2(1)**2))/(2.*dr2(0))
              uu2sss = (-u2(j1,j2-2,j3,ex)+2.*u2(j1,j2-1,j3,ex)-2.*u2(
     & j1,j2+1,j3,ex)+u2(j1,j2+2,j3,ex))/(2.*dr2(1)**3)
              uu2rrrr = (u2(j1-2,j2,j3,ex)-4.*u2(j1-1,j2,j3,ex)+6.*u2(
     & j1,j2,j3,ex)-4.*u2(j1+1,j2,j3,ex)+u2(j1+2,j2,j3,ex))/(dr2(0)**
     & 4)
              uu2rrrs = (-(-u2(j1-2,j2-1,j3,ex)+u2(j1-2,j2+1,j3,ex))/(
     & 2.*dr2(1))+2.*(-u2(j1-1,j2-1,j3,ex)+u2(j1-1,j2+1,j3,ex))/(2.*
     & dr2(1))-2.*(-u2(j1+1,j2-1,j3,ex)+u2(j1+1,j2+1,j3,ex))/(2.*dr2(
     & 1))+(-u2(j1+2,j2-1,j3,ex)+u2(j1+2,j2+1,j3,ex))/(2.*dr2(1)))/(
     & 2.*dr2(0)**3)
              uu2rrss = ((u2(j1-1,j2-1,j3,ex)-2.*u2(j1-1,j2,j3,ex)+u2(
     & j1-1,j2+1,j3,ex))/(dr2(1)**2)-2.*(u2(j1,j2-1,j3,ex)-2.*u2(j1,
     & j2,j3,ex)+u2(j1,j2+1,j3,ex))/(dr2(1)**2)+(u2(j1+1,j2-1,j3,ex)-
     & 2.*u2(j1+1,j2,j3,ex)+u2(j1+1,j2+1,j3,ex))/(dr2(1)**2))/(dr2(0)*
     & *2)
              uu2rsss = (-(-u2(j1-1,j2-2,j3,ex)+2.*u2(j1-1,j2-1,j3,ex)-
     & 2.*u2(j1-1,j2+1,j3,ex)+u2(j1-1,j2+2,j3,ex))/(2.*dr2(1)**3)+(-
     & u2(j1+1,j2-2,j3,ex)+2.*u2(j1+1,j2-1,j3,ex)-2.*u2(j1+1,j2+1,j3,
     & ex)+u2(j1+1,j2+2,j3,ex))/(2.*dr2(1)**3))/(2.*dr2(0))
              uu2ssss = (u2(j1,j2-2,j3,ex)-4.*u2(j1,j2-1,j3,ex)+6.*u2(
     & j1,j2,j3,ex)-4.*u2(j1,j2+1,j3,ex)+u2(j1,j2+2,j3,ex))/(dr2(1)**
     & 4)
               t1 = aj2rx**2
               t7 = aj2sx**2
               u2xxx = t1*aj2rx*uu2rrr+3*t1*aj2sx*uu2rrs+3*aj2rx*t7*
     & uu2rss+t7*aj2sx*uu2sss+3*aj2rx*aj2rxx*uu2rr+(3*aj2sxx*aj2rx+3*
     & aj2sx*aj2rxx)*uu2rs+3*aj2sxx*aj2sx*uu2ss+aj2rxxx*uu2r+aj2sxxx*
     & uu2s
               t1 = aj2rx**2
               t10 = aj2sx**2
               u2xxy = aj2ry*t1*uu2rrr+(aj2sy*t1+2*aj2ry*aj2sx*aj2rx)*
     & uu2rrs+(aj2ry*t10+2*aj2sy*aj2sx*aj2rx)*uu2rss+aj2sy*t10*uu2sss+
     & (2*aj2rxy*aj2rx+aj2ry*aj2rxx)*uu2rr+(aj2ry*aj2sxx+2*aj2sx*
     & aj2rxy+2*aj2sxy*aj2rx+aj2sy*aj2rxx)*uu2rs+(aj2sy*aj2sxx+2*
     & aj2sxy*aj2sx)*uu2ss+aj2rxxy*uu2r+aj2sxxy*uu2s
               t1 = aj2ry**2
               t4 = aj2sy*aj2ry
               t8 = aj2sy*aj2rx+aj2ry*aj2sx
               t16 = aj2sy**2
               u2xyy = t1*aj2rx*uu2rrr+(t4*aj2rx+aj2ry*t8)*uu2rrs+(t4*
     & aj2sx+aj2sy*t8)*uu2rss+t16*aj2sx*uu2sss+(aj2ryy*aj2rx+2*aj2ry*
     & aj2rxy)*uu2rr+(2*aj2ry*aj2sxy+2*aj2sy*aj2rxy+aj2ryy*aj2sx+
     & aj2syy*aj2rx)*uu2rs+(aj2syy*aj2sx+2*aj2sy*aj2sxy)*uu2ss+
     & aj2rxyy*uu2r+aj2sxyy*uu2s
               t1 = aj2ry**2
               t7 = aj2sy**2
               u2yyy = aj2ry*t1*uu2rrr+3*t1*aj2sy*uu2rrs+3*aj2ry*t7*
     & uu2rss+t7*aj2sy*uu2sss+3*aj2ry*aj2ryy*uu2rr+(3*aj2syy*aj2ry+3*
     & aj2sy*aj2ryy)*uu2rs+3*aj2syy*aj2sy*uu2ss+aj2ryyy*uu2r+aj2syyy*
     & uu2s
               t1 = aj2rx**2
               t2 = t1**2
               t8 = aj2sx**2
               t16 = t8**2
               t25 = aj2sxx*aj2rx
               t27 = t25+aj2sx*aj2rxx
               t28 = 3*t27
               t30 = 2*t27
               t46 = aj2rxx**2
               t60 = aj2sxx**2
               u2xxxx = t2*uu2rrrr+4*t1*aj2rx*aj2sx*uu2rrrs+6*t1*t8*
     & uu2rrss+4*aj2rx*t8*aj2sx*uu2rsss+t16*uu2ssss+6*t1*aj2rxx*
     & uu2rrr+(7*aj2sx*aj2rx*aj2rxx+aj2sxx*t1+aj2rx*t28+aj2rx*t30)*
     & uu2rrs+(aj2sx*t28+7*t25*aj2sx+aj2rxx*t8+aj2sx*t30)*uu2rss+6*t8*
     & aj2sxx*uu2sss+(4*aj2rx*aj2rxxx+3*t46)*uu2rr+(4*aj2sxxx*aj2rx+4*
     & aj2sx*aj2rxxx+6*aj2sxx*aj2rxx)*uu2rs+(4*aj2sxxx*aj2sx+3*t60)*
     & uu2ss+aj2rxxxx*uu2r+aj2sxxxx*uu2s
               t1 = aj2ry**2
               t2 = aj2rx**2
               t5 = aj2sy*aj2ry
               t11 = aj2sy*t2+2*aj2ry*aj2sx*aj2rx
               t16 = aj2sx**2
               t21 = aj2ry*t16+2*aj2sy*aj2sx*aj2rx
               t29 = aj2sy**2
               t38 = 2*aj2rxy*aj2rx+aj2ry*aj2rxx
               t52 = aj2sx*aj2rxy
               t54 = aj2sxy*aj2rx
               t57 = aj2ry*aj2sxx+2*t52+2*t54+aj2sy*aj2rxx
               t60 = 2*t52+2*t54
               t68 = aj2sy*aj2sxx+2*aj2sxy*aj2sx
               t92 = aj2rxy**2
               t110 = aj2sxy**2
               u2xxyy = t1*t2*uu2rrrr+(t5*t2+aj2ry*t11)*uu2rrrs+(aj2sy*
     & t11+aj2ry*t21)*uu2rrss+(aj2sy*t21+t5*t16)*uu2rsss+t29*t16*
     & uu2ssss+(2*aj2ry*aj2rxy*aj2rx+aj2ry*t38+aj2ryy*t2)*uu2rrr+(
     & aj2sy*t38+2*aj2sy*aj2rxy*aj2rx+2*aj2ryy*aj2sx*aj2rx+aj2syy*t2+
     & aj2ry*t57+aj2ry*t60)*uu2rrs+(aj2sy*t57+aj2ry*t68+aj2ryy*t16+2*
     & aj2ry*aj2sxy*aj2sx+2*aj2syy*aj2sx*aj2rx+aj2sy*t60)*uu2rss+(2*
     & aj2sy*aj2sxy*aj2sx+aj2sy*t68+aj2syy*t16)*uu2sss+(2*aj2rx*
     & aj2rxyy+aj2ryy*aj2rxx+2*aj2ry*aj2rxxy+2*t92)*uu2rr+(4*aj2sxy*
     & aj2rxy+2*aj2ry*aj2sxxy+aj2ryy*aj2sxx+2*aj2sy*aj2rxxy+2*aj2sxyy*
     & aj2rx+aj2syy*aj2rxx+2*aj2sx*aj2rxyy)*uu2rs+(2*t110+2*aj2sy*
     & aj2sxxy+aj2syy*aj2sxx+2*aj2sx*aj2sxyy)*uu2ss+aj2rxxyy*uu2r+
     & aj2sxxyy*uu2s
               t1 = aj2ry**2
               t2 = t1**2
               t8 = aj2sy**2
               t16 = t8**2
               t25 = aj2syy*aj2ry
               t27 = t25+aj2sy*aj2ryy
               t28 = 3*t27
               t30 = 2*t27
               t46 = aj2ryy**2
               t60 = aj2syy**2
               u2yyyy = t2*uu2rrrr+4*t1*aj2ry*aj2sy*uu2rrrs+6*t1*t8*
     & uu2rrss+4*aj2ry*t8*aj2sy*uu2rsss+t16*uu2ssss+6*t1*aj2ryy*
     & uu2rrr+(7*aj2sy*aj2ry*aj2ryy+aj2syy*t1+aj2ry*t28+aj2ry*t30)*
     & uu2rrs+(aj2sy*t28+7*t25*aj2sy+aj2ryy*t8+aj2sy*t30)*uu2rss+6*t8*
     & aj2syy*uu2sss+(4*aj2ry*aj2ryyy+3*t46)*uu2rr+(4*aj2syyy*aj2ry+4*
     & aj2sy*aj2ryyy+6*aj2syy*aj2ryy)*uu2rs+(4*aj2syyy*aj2sy+3*t60)*
     & uu2ss+aj2ryyyy*uu2r+aj2syyyy*uu2s
             u2LapSq = u2xxxx +2.* u2xxyy + u2yyyy
              vv2 = u2(j1,j2,j3,ey)
              vv2r = (-u2(j1-1,j2,j3,ey)+u2(j1+1,j2,j3,ey))/(2.*dr2(0))
              vv2s = (-u2(j1,j2-1,j3,ey)+u2(j1,j2+1,j3,ey))/(2.*dr2(1))
              vv2rr = (u2(j1-1,j2,j3,ey)-2.*u2(j1,j2,j3,ey)+u2(j1+1,j2,
     & j3,ey))/(dr2(0)**2)
              vv2rs = (-(-u2(j1-1,j2-1,j3,ey)+u2(j1-1,j2+1,j3,ey))/(2.*
     & dr2(1))+(-u2(j1+1,j2-1,j3,ey)+u2(j1+1,j2+1,j3,ey))/(2.*dr2(1)))
     & /(2.*dr2(0))
              vv2ss = (u2(j1,j2-1,j3,ey)-2.*u2(j1,j2,j3,ey)+u2(j1,j2+1,
     & j3,ey))/(dr2(1)**2)
              vv2rrr = (-u2(j1-2,j2,j3,ey)+2.*u2(j1-1,j2,j3,ey)-2.*u2(
     & j1+1,j2,j3,ey)+u2(j1+2,j2,j3,ey))/(2.*dr2(0)**3)
              vv2rrs = ((-u2(j1-1,j2-1,j3,ey)+u2(j1-1,j2+1,j3,ey))/(2.*
     & dr2(1))-2.*(-u2(j1,j2-1,j3,ey)+u2(j1,j2+1,j3,ey))/(2.*dr2(1))+(
     & -u2(j1+1,j2-1,j3,ey)+u2(j1+1,j2+1,j3,ey))/(2.*dr2(1)))/(dr2(0)*
     & *2)
              vv2rss = (-(u2(j1-1,j2-1,j3,ey)-2.*u2(j1-1,j2,j3,ey)+u2(
     & j1-1,j2+1,j3,ey))/(dr2(1)**2)+(u2(j1+1,j2-1,j3,ey)-2.*u2(j1+1,
     & j2,j3,ey)+u2(j1+1,j2+1,j3,ey))/(dr2(1)**2))/(2.*dr2(0))
              vv2sss = (-u2(j1,j2-2,j3,ey)+2.*u2(j1,j2-1,j3,ey)-2.*u2(
     & j1,j2+1,j3,ey)+u2(j1,j2+2,j3,ey))/(2.*dr2(1)**3)
              vv2rrrr = (u2(j1-2,j2,j3,ey)-4.*u2(j1-1,j2,j3,ey)+6.*u2(
     & j1,j2,j3,ey)-4.*u2(j1+1,j2,j3,ey)+u2(j1+2,j2,j3,ey))/(dr2(0)**
     & 4)
              vv2rrrs = (-(-u2(j1-2,j2-1,j3,ey)+u2(j1-2,j2+1,j3,ey))/(
     & 2.*dr2(1))+2.*(-u2(j1-1,j2-1,j3,ey)+u2(j1-1,j2+1,j3,ey))/(2.*
     & dr2(1))-2.*(-u2(j1+1,j2-1,j3,ey)+u2(j1+1,j2+1,j3,ey))/(2.*dr2(
     & 1))+(-u2(j1+2,j2-1,j3,ey)+u2(j1+2,j2+1,j3,ey))/(2.*dr2(1)))/(
     & 2.*dr2(0)**3)
              vv2rrss = ((u2(j1-1,j2-1,j3,ey)-2.*u2(j1-1,j2,j3,ey)+u2(
     & j1-1,j2+1,j3,ey))/(dr2(1)**2)-2.*(u2(j1,j2-1,j3,ey)-2.*u2(j1,
     & j2,j3,ey)+u2(j1,j2+1,j3,ey))/(dr2(1)**2)+(u2(j1+1,j2-1,j3,ey)-
     & 2.*u2(j1+1,j2,j3,ey)+u2(j1+1,j2+1,j3,ey))/(dr2(1)**2))/(dr2(0)*
     & *2)
              vv2rsss = (-(-u2(j1-1,j2-2,j3,ey)+2.*u2(j1-1,j2-1,j3,ey)-
     & 2.*u2(j1-1,j2+1,j3,ey)+u2(j1-1,j2+2,j3,ey))/(2.*dr2(1)**3)+(-
     & u2(j1+1,j2-2,j3,ey)+2.*u2(j1+1,j2-1,j3,ey)-2.*u2(j1+1,j2+1,j3,
     & ey)+u2(j1+1,j2+2,j3,ey))/(2.*dr2(1)**3))/(2.*dr2(0))
              vv2ssss = (u2(j1,j2-2,j3,ey)-4.*u2(j1,j2-1,j3,ey)+6.*u2(
     & j1,j2,j3,ey)-4.*u2(j1,j2+1,j3,ey)+u2(j1,j2+2,j3,ey))/(dr2(1)**
     & 4)
               t1 = aj2rx**2
               t7 = aj2sx**2
               v2xxx = t1*aj2rx*vv2rrr+3*t1*aj2sx*vv2rrs+3*aj2rx*t7*
     & vv2rss+t7*aj2sx*vv2sss+3*aj2rx*aj2rxx*vv2rr+(3*aj2sxx*aj2rx+3*
     & aj2sx*aj2rxx)*vv2rs+3*aj2sxx*aj2sx*vv2ss+aj2rxxx*vv2r+aj2sxxx*
     & vv2s
               t1 = aj2rx**2
               t10 = aj2sx**2
               v2xxy = aj2ry*t1*vv2rrr+(aj2sy*t1+2*aj2ry*aj2sx*aj2rx)*
     & vv2rrs+(aj2ry*t10+2*aj2sy*aj2sx*aj2rx)*vv2rss+aj2sy*t10*vv2sss+
     & (2*aj2rxy*aj2rx+aj2ry*aj2rxx)*vv2rr+(aj2ry*aj2sxx+2*aj2sx*
     & aj2rxy+2*aj2sxy*aj2rx+aj2sy*aj2rxx)*vv2rs+(aj2sy*aj2sxx+2*
     & aj2sxy*aj2sx)*vv2ss+aj2rxxy*vv2r+aj2sxxy*vv2s
               t1 = aj2ry**2
               t4 = aj2sy*aj2ry
               t8 = aj2sy*aj2rx+aj2ry*aj2sx
               t16 = aj2sy**2
               v2xyy = t1*aj2rx*vv2rrr+(t4*aj2rx+aj2ry*t8)*vv2rrs+(t4*
     & aj2sx+aj2sy*t8)*vv2rss+t16*aj2sx*vv2sss+(aj2ryy*aj2rx+2*aj2ry*
     & aj2rxy)*vv2rr+(2*aj2ry*aj2sxy+2*aj2sy*aj2rxy+aj2ryy*aj2sx+
     & aj2syy*aj2rx)*vv2rs+(aj2syy*aj2sx+2*aj2sy*aj2sxy)*vv2ss+
     & aj2rxyy*vv2r+aj2sxyy*vv2s
               t1 = aj2ry**2
               t7 = aj2sy**2
               v2yyy = aj2ry*t1*vv2rrr+3*t1*aj2sy*vv2rrs+3*aj2ry*t7*
     & vv2rss+t7*aj2sy*vv2sss+3*aj2ry*aj2ryy*vv2rr+(3*aj2syy*aj2ry+3*
     & aj2sy*aj2ryy)*vv2rs+3*aj2syy*aj2sy*vv2ss+aj2ryyy*vv2r+aj2syyy*
     & vv2s
               t1 = aj2rx**2
               t2 = t1**2
               t8 = aj2sx**2
               t16 = t8**2
               t25 = aj2sxx*aj2rx
               t27 = t25+aj2sx*aj2rxx
               t28 = 3*t27
               t30 = 2*t27
               t46 = aj2rxx**2
               t60 = aj2sxx**2
               v2xxxx = t2*vv2rrrr+4*t1*aj2rx*aj2sx*vv2rrrs+6*t1*t8*
     & vv2rrss+4*aj2rx*t8*aj2sx*vv2rsss+t16*vv2ssss+6*t1*aj2rxx*
     & vv2rrr+(7*aj2sx*aj2rx*aj2rxx+aj2sxx*t1+aj2rx*t28+aj2rx*t30)*
     & vv2rrs+(aj2sx*t28+7*t25*aj2sx+aj2rxx*t8+aj2sx*t30)*vv2rss+6*t8*
     & aj2sxx*vv2sss+(4*aj2rx*aj2rxxx+3*t46)*vv2rr+(4*aj2sxxx*aj2rx+4*
     & aj2sx*aj2rxxx+6*aj2sxx*aj2rxx)*vv2rs+(4*aj2sxxx*aj2sx+3*t60)*
     & vv2ss+aj2rxxxx*vv2r+aj2sxxxx*vv2s
               t1 = aj2ry**2
               t2 = aj2rx**2
               t5 = aj2sy*aj2ry
               t11 = aj2sy*t2+2*aj2ry*aj2sx*aj2rx
               t16 = aj2sx**2
               t21 = aj2ry*t16+2*aj2sy*aj2sx*aj2rx
               t29 = aj2sy**2
               t38 = 2*aj2rxy*aj2rx+aj2ry*aj2rxx
               t52 = aj2sx*aj2rxy
               t54 = aj2sxy*aj2rx
               t57 = aj2ry*aj2sxx+2*t52+2*t54+aj2sy*aj2rxx
               t60 = 2*t52+2*t54
               t68 = aj2sy*aj2sxx+2*aj2sxy*aj2sx
               t92 = aj2rxy**2
               t110 = aj2sxy**2
               v2xxyy = t1*t2*vv2rrrr+(t5*t2+aj2ry*t11)*vv2rrrs+(aj2sy*
     & t11+aj2ry*t21)*vv2rrss+(aj2sy*t21+t5*t16)*vv2rsss+t29*t16*
     & vv2ssss+(2*aj2ry*aj2rxy*aj2rx+aj2ry*t38+aj2ryy*t2)*vv2rrr+(
     & aj2sy*t38+2*aj2sy*aj2rxy*aj2rx+2*aj2ryy*aj2sx*aj2rx+aj2syy*t2+
     & aj2ry*t57+aj2ry*t60)*vv2rrs+(aj2sy*t57+aj2ry*t68+aj2ryy*t16+2*
     & aj2ry*aj2sxy*aj2sx+2*aj2syy*aj2sx*aj2rx+aj2sy*t60)*vv2rss+(2*
     & aj2sy*aj2sxy*aj2sx+aj2sy*t68+aj2syy*t16)*vv2sss+(2*aj2rx*
     & aj2rxyy+aj2ryy*aj2rxx+2*aj2ry*aj2rxxy+2*t92)*vv2rr+(4*aj2sxy*
     & aj2rxy+2*aj2ry*aj2sxxy+aj2ryy*aj2sxx+2*aj2sy*aj2rxxy+2*aj2sxyy*
     & aj2rx+aj2syy*aj2rxx+2*aj2sx*aj2rxyy)*vv2rs+(2*t110+2*aj2sy*
     & aj2sxxy+aj2syy*aj2sxx+2*aj2sx*aj2sxyy)*vv2ss+aj2rxxyy*vv2r+
     & aj2sxxyy*vv2s
               t1 = aj2ry**2
               t2 = t1**2
               t8 = aj2sy**2
               t16 = t8**2
               t25 = aj2syy*aj2ry
               t27 = t25+aj2sy*aj2ryy
               t28 = 3*t27
               t30 = 2*t27
               t46 = aj2ryy**2
               t60 = aj2syy**2
               v2yyyy = t2*vv2rrrr+4*t1*aj2ry*aj2sy*vv2rrrs+6*t1*t8*
     & vv2rrss+4*aj2ry*t8*aj2sy*vv2rsss+t16*vv2ssss+6*t1*aj2ryy*
     & vv2rrr+(7*aj2sy*aj2ry*aj2ryy+aj2syy*t1+aj2ry*t28+aj2ry*t30)*
     & vv2rrs+(aj2sy*t28+7*t25*aj2sy+aj2ryy*t8+aj2sy*t30)*vv2rss+6*t8*
     & aj2syy*vv2sss+(4*aj2ry*aj2ryyy+3*t46)*vv2rr+(4*aj2syyy*aj2ry+4*
     & aj2sy*aj2ryyy+6*aj2syy*aj2ryy)*vv2rs+(4*aj2syyy*aj2sy+3*t60)*
     & vv2ss+aj2ryyyy*vv2r+aj2syyyy*vv2s
             v2LapSq = v2xxxx +2.* v2xxyy + v2yyyy
            ! These derivatives are computed to 4th-order accuracy
            ! NOTE: the jacobian derivatives can be computed once for all components
             ! this next call will define the jacobian and its derivatives (parameteric and spatial)
             aj1rx = rsxy1(i1,i2,i3,0,0)
             aj1rxr = (rsxy1(i1-2,i2,i3,0,0)-8.*rsxy1(i1-1,i2,i3,0,0)+
     & 8.*rsxy1(i1+1,i2,i3,0,0)-rsxy1(i1+2,i2,i3,0,0))/(12.*dr1(0))
             aj1rxs = (rsxy1(i1,i2-2,i3,0,0)-8.*rsxy1(i1,i2-1,i3,0,0)+
     & 8.*rsxy1(i1,i2+1,i3,0,0)-rsxy1(i1,i2+2,i3,0,0))/(12.*dr1(1))
             aj1sx = rsxy1(i1,i2,i3,1,0)
             aj1sxr = (rsxy1(i1-2,i2,i3,1,0)-8.*rsxy1(i1-1,i2,i3,1,0)+
     & 8.*rsxy1(i1+1,i2,i3,1,0)-rsxy1(i1+2,i2,i3,1,0))/(12.*dr1(0))
             aj1sxs = (rsxy1(i1,i2-2,i3,1,0)-8.*rsxy1(i1,i2-1,i3,1,0)+
     & 8.*rsxy1(i1,i2+1,i3,1,0)-rsxy1(i1,i2+2,i3,1,0))/(12.*dr1(1))
             aj1ry = rsxy1(i1,i2,i3,0,1)
             aj1ryr = (rsxy1(i1-2,i2,i3,0,1)-8.*rsxy1(i1-1,i2,i3,0,1)+
     & 8.*rsxy1(i1+1,i2,i3,0,1)-rsxy1(i1+2,i2,i3,0,1))/(12.*dr1(0))
             aj1rys = (rsxy1(i1,i2-2,i3,0,1)-8.*rsxy1(i1,i2-1,i3,0,1)+
     & 8.*rsxy1(i1,i2+1,i3,0,1)-rsxy1(i1,i2+2,i3,0,1))/(12.*dr1(1))
             aj1sy = rsxy1(i1,i2,i3,1,1)
             aj1syr = (rsxy1(i1-2,i2,i3,1,1)-8.*rsxy1(i1-1,i2,i3,1,1)+
     & 8.*rsxy1(i1+1,i2,i3,1,1)-rsxy1(i1+2,i2,i3,1,1))/(12.*dr1(0))
             aj1sys = (rsxy1(i1,i2-2,i3,1,1)-8.*rsxy1(i1,i2-1,i3,1,1)+
     & 8.*rsxy1(i1,i2+1,i3,1,1)-rsxy1(i1,i2+2,i3,1,1))/(12.*dr1(1))
             aj1rxx = aj1rx*aj1rxr+aj1sx*aj1rxs
             aj1rxy = aj1ry*aj1rxr+aj1sy*aj1rxs
             aj1sxx = aj1rx*aj1sxr+aj1sx*aj1sxs
             aj1sxy = aj1ry*aj1sxr+aj1sy*aj1sxs
             aj1ryx = aj1rx*aj1ryr+aj1sx*aj1rys
             aj1ryy = aj1ry*aj1ryr+aj1sy*aj1rys
             aj1syx = aj1rx*aj1syr+aj1sx*aj1sys
             aj1syy = aj1ry*aj1syr+aj1sy*aj1sys
              uu1 = u1(i1,i2,i3,ex)
              uu1r = (u1(i1-2,i2,i3,ex)-8.*u1(i1-1,i2,i3,ex)+8.*u1(i1+
     & 1,i2,i3,ex)-u1(i1+2,i2,i3,ex))/(12.*dr1(0))
              uu1s = (u1(i1,i2-2,i3,ex)-8.*u1(i1,i2-1,i3,ex)+8.*u1(i1,
     & i2+1,i3,ex)-u1(i1,i2+2,i3,ex))/(12.*dr1(1))
              uu1rr = (-u1(i1-2,i2,i3,ex)+16.*u1(i1-1,i2,i3,ex)-30.*u1(
     & i1,i2,i3,ex)+16.*u1(i1+1,i2,i3,ex)-u1(i1+2,i2,i3,ex))/(12.*dr1(
     & 0)**2)
              uu1rs = ((u1(i1-2,i2-2,i3,ex)-8.*u1(i1-2,i2-1,i3,ex)+8.*
     & u1(i1-2,i2+1,i3,ex)-u1(i1-2,i2+2,i3,ex))/(12.*dr1(1))-8.*(u1(
     & i1-1,i2-2,i3,ex)-8.*u1(i1-1,i2-1,i3,ex)+8.*u1(i1-1,i2+1,i3,ex)-
     & u1(i1-1,i2+2,i3,ex))/(12.*dr1(1))+8.*(u1(i1+1,i2-2,i3,ex)-8.*
     & u1(i1+1,i2-1,i3,ex)+8.*u1(i1+1,i2+1,i3,ex)-u1(i1+1,i2+2,i3,ex))
     & /(12.*dr1(1))-(u1(i1+2,i2-2,i3,ex)-8.*u1(i1+2,i2-1,i3,ex)+8.*
     & u1(i1+2,i2+1,i3,ex)-u1(i1+2,i2+2,i3,ex))/(12.*dr1(1)))/(12.*
     & dr1(0))
              uu1ss = (-u1(i1,i2-2,i3,ex)+16.*u1(i1,i2-1,i3,ex)-30.*u1(
     & i1,i2,i3,ex)+16.*u1(i1,i2+1,i3,ex)-u1(i1,i2+2,i3,ex))/(12.*dr1(
     & 1)**2)
               u1x = aj1rx*uu1r+aj1sx*uu1s
               u1y = aj1ry*uu1r+aj1sy*uu1s
               t1 = aj1rx**2
               t6 = aj1sx**2
               u1xx = t1*uu1rr+2*aj1rx*aj1sx*uu1rs+t6*uu1ss+aj1rxx*
     & uu1r+aj1sxx*uu1s
               t1 = aj1ry**2
               t6 = aj1sy**2
               u1yy = t1*uu1rr+2*aj1ry*aj1sy*uu1rs+t6*uu1ss+aj1ryy*
     & uu1r+aj1syy*uu1s
             u1Lap = u1xx+ u1yy
              vv1 = u1(i1,i2,i3,ey)
              vv1r = (u1(i1-2,i2,i3,ey)-8.*u1(i1-1,i2,i3,ey)+8.*u1(i1+
     & 1,i2,i3,ey)-u1(i1+2,i2,i3,ey))/(12.*dr1(0))
              vv1s = (u1(i1,i2-2,i3,ey)-8.*u1(i1,i2-1,i3,ey)+8.*u1(i1,
     & i2+1,i3,ey)-u1(i1,i2+2,i3,ey))/(12.*dr1(1))
              vv1rr = (-u1(i1-2,i2,i3,ey)+16.*u1(i1-1,i2,i3,ey)-30.*u1(
     & i1,i2,i3,ey)+16.*u1(i1+1,i2,i3,ey)-u1(i1+2,i2,i3,ey))/(12.*dr1(
     & 0)**2)
              vv1rs = ((u1(i1-2,i2-2,i3,ey)-8.*u1(i1-2,i2-1,i3,ey)+8.*
     & u1(i1-2,i2+1,i3,ey)-u1(i1-2,i2+2,i3,ey))/(12.*dr1(1))-8.*(u1(
     & i1-1,i2-2,i3,ey)-8.*u1(i1-1,i2-1,i3,ey)+8.*u1(i1-1,i2+1,i3,ey)-
     & u1(i1-1,i2+2,i3,ey))/(12.*dr1(1))+8.*(u1(i1+1,i2-2,i3,ey)-8.*
     & u1(i1+1,i2-1,i3,ey)+8.*u1(i1+1,i2+1,i3,ey)-u1(i1+1,i2+2,i3,ey))
     & /(12.*dr1(1))-(u1(i1+2,i2-2,i3,ey)-8.*u1(i1+2,i2-1,i3,ey)+8.*
     & u1(i1+2,i2+1,i3,ey)-u1(i1+2,i2+2,i3,ey))/(12.*dr1(1)))/(12.*
     & dr1(0))
              vv1ss = (-u1(i1,i2-2,i3,ey)+16.*u1(i1,i2-1,i3,ey)-30.*u1(
     & i1,i2,i3,ey)+16.*u1(i1,i2+1,i3,ey)-u1(i1,i2+2,i3,ey))/(12.*dr1(
     & 1)**2)
               v1x = aj1rx*vv1r+aj1sx*vv1s
               v1y = aj1ry*vv1r+aj1sy*vv1s
               t1 = aj1rx**2
               t6 = aj1sx**2
               v1xx = t1*vv1rr+2*aj1rx*aj1sx*vv1rs+t6*vv1ss+aj1rxx*
     & vv1r+aj1sxx*vv1s
               t1 = aj1ry**2
               t6 = aj1sy**2
               v1yy = t1*vv1rr+2*aj1ry*aj1sy*vv1rs+t6*vv1ss+aj1ryy*
     & vv1r+aj1syy*vv1s
             v1Lap = v1xx+ v1yy
            ! NOTE: the jacobian derivatives can be computed once for all components
             ! this next call will define the jacobian and its derivatives (parameteric and spatial)
             aj2rx = rsxy2(j1,j2,j3,0,0)
             aj2rxr = (rsxy2(j1-2,j2,j3,0,0)-8.*rsxy2(j1-1,j2,j3,0,0)+
     & 8.*rsxy2(j1+1,j2,j3,0,0)-rsxy2(j1+2,j2,j3,0,0))/(12.*dr2(0))
             aj2rxs = (rsxy2(j1,j2-2,j3,0,0)-8.*rsxy2(j1,j2-1,j3,0,0)+
     & 8.*rsxy2(j1,j2+1,j3,0,0)-rsxy2(j1,j2+2,j3,0,0))/(12.*dr2(1))
             aj2sx = rsxy2(j1,j2,j3,1,0)
             aj2sxr = (rsxy2(j1-2,j2,j3,1,0)-8.*rsxy2(j1-1,j2,j3,1,0)+
     & 8.*rsxy2(j1+1,j2,j3,1,0)-rsxy2(j1+2,j2,j3,1,0))/(12.*dr2(0))
             aj2sxs = (rsxy2(j1,j2-2,j3,1,0)-8.*rsxy2(j1,j2-1,j3,1,0)+
     & 8.*rsxy2(j1,j2+1,j3,1,0)-rsxy2(j1,j2+2,j3,1,0))/(12.*dr2(1))
             aj2ry = rsxy2(j1,j2,j3,0,1)
             aj2ryr = (rsxy2(j1-2,j2,j3,0,1)-8.*rsxy2(j1-1,j2,j3,0,1)+
     & 8.*rsxy2(j1+1,j2,j3,0,1)-rsxy2(j1+2,j2,j3,0,1))/(12.*dr2(0))
             aj2rys = (rsxy2(j1,j2-2,j3,0,1)-8.*rsxy2(j1,j2-1,j3,0,1)+
     & 8.*rsxy2(j1,j2+1,j3,0,1)-rsxy2(j1,j2+2,j3,0,1))/(12.*dr2(1))
             aj2sy = rsxy2(j1,j2,j3,1,1)
             aj2syr = (rsxy2(j1-2,j2,j3,1,1)-8.*rsxy2(j1-1,j2,j3,1,1)+
     & 8.*rsxy2(j1+1,j2,j3,1,1)-rsxy2(j1+2,j2,j3,1,1))/(12.*dr2(0))
             aj2sys = (rsxy2(j1,j2-2,j3,1,1)-8.*rsxy2(j1,j2-1,j3,1,1)+
     & 8.*rsxy2(j1,j2+1,j3,1,1)-rsxy2(j1,j2+2,j3,1,1))/(12.*dr2(1))
             aj2rxx = aj2rx*aj2rxr+aj2sx*aj2rxs
             aj2rxy = aj2ry*aj2rxr+aj2sy*aj2rxs
             aj2sxx = aj2rx*aj2sxr+aj2sx*aj2sxs
             aj2sxy = aj2ry*aj2sxr+aj2sy*aj2sxs
             aj2ryx = aj2rx*aj2ryr+aj2sx*aj2rys
             aj2ryy = aj2ry*aj2ryr+aj2sy*aj2rys
             aj2syx = aj2rx*aj2syr+aj2sx*aj2sys
             aj2syy = aj2ry*aj2syr+aj2sy*aj2sys
              uu2 = u2(j1,j2,j3,ex)
              uu2r = (u2(j1-2,j2,j3,ex)-8.*u2(j1-1,j2,j3,ex)+8.*u2(j1+
     & 1,j2,j3,ex)-u2(j1+2,j2,j3,ex))/(12.*dr2(0))
              uu2s = (u2(j1,j2-2,j3,ex)-8.*u2(j1,j2-1,j3,ex)+8.*u2(j1,
     & j2+1,j3,ex)-u2(j1,j2+2,j3,ex))/(12.*dr2(1))
              uu2rr = (-u2(j1-2,j2,j3,ex)+16.*u2(j1-1,j2,j3,ex)-30.*u2(
     & j1,j2,j3,ex)+16.*u2(j1+1,j2,j3,ex)-u2(j1+2,j2,j3,ex))/(12.*dr2(
     & 0)**2)
              uu2rs = ((u2(j1-2,j2-2,j3,ex)-8.*u2(j1-2,j2-1,j3,ex)+8.*
     & u2(j1-2,j2+1,j3,ex)-u2(j1-2,j2+2,j3,ex))/(12.*dr2(1))-8.*(u2(
     & j1-1,j2-2,j3,ex)-8.*u2(j1-1,j2-1,j3,ex)+8.*u2(j1-1,j2+1,j3,ex)-
     & u2(j1-1,j2+2,j3,ex))/(12.*dr2(1))+8.*(u2(j1+1,j2-2,j3,ex)-8.*
     & u2(j1+1,j2-1,j3,ex)+8.*u2(j1+1,j2+1,j3,ex)-u2(j1+1,j2+2,j3,ex))
     & /(12.*dr2(1))-(u2(j1+2,j2-2,j3,ex)-8.*u2(j1+2,j2-1,j3,ex)+8.*
     & u2(j1+2,j2+1,j3,ex)-u2(j1+2,j2+2,j3,ex))/(12.*dr2(1)))/(12.*
     & dr2(0))
              uu2ss = (-u2(j1,j2-2,j3,ex)+16.*u2(j1,j2-1,j3,ex)-30.*u2(
     & j1,j2,j3,ex)+16.*u2(j1,j2+1,j3,ex)-u2(j1,j2+2,j3,ex))/(12.*dr2(
     & 1)**2)
               u2x = aj2rx*uu2r+aj2sx*uu2s
               u2y = aj2ry*uu2r+aj2sy*uu2s
               t1 = aj2rx**2
               t6 = aj2sx**2
               u2xx = t1*uu2rr+2*aj2rx*aj2sx*uu2rs+t6*uu2ss+aj2rxx*
     & uu2r+aj2sxx*uu2s
               t1 = aj2ry**2
               t6 = aj2sy**2
               u2yy = t1*uu2rr+2*aj2ry*aj2sy*uu2rs+t6*uu2ss+aj2ryy*
     & uu2r+aj2syy*uu2s
             u2Lap = u2xx+ u2yy
              vv2 = u2(j1,j2,j3,ey)
              vv2r = (u2(j1-2,j2,j3,ey)-8.*u2(j1-1,j2,j3,ey)+8.*u2(j1+
     & 1,j2,j3,ey)-u2(j1+2,j2,j3,ey))/(12.*dr2(0))
              vv2s = (u2(j1,j2-2,j3,ey)-8.*u2(j1,j2-1,j3,ey)+8.*u2(j1,
     & j2+1,j3,ey)-u2(j1,j2+2,j3,ey))/(12.*dr2(1))
              vv2rr = (-u2(j1-2,j2,j3,ey)+16.*u2(j1-1,j2,j3,ey)-30.*u2(
     & j1,j2,j3,ey)+16.*u2(j1+1,j2,j3,ey)-u2(j1+2,j2,j3,ey))/(12.*dr2(
     & 0)**2)
              vv2rs = ((u2(j1-2,j2-2,j3,ey)-8.*u2(j1-2,j2-1,j3,ey)+8.*
     & u2(j1-2,j2+1,j3,ey)-u2(j1-2,j2+2,j3,ey))/(12.*dr2(1))-8.*(u2(
     & j1-1,j2-2,j3,ey)-8.*u2(j1-1,j2-1,j3,ey)+8.*u2(j1-1,j2+1,j3,ey)-
     & u2(j1-1,j2+2,j3,ey))/(12.*dr2(1))+8.*(u2(j1+1,j2-2,j3,ey)-8.*
     & u2(j1+1,j2-1,j3,ey)+8.*u2(j1+1,j2+1,j3,ey)-u2(j1+1,j2+2,j3,ey))
     & /(12.*dr2(1))-(u2(j1+2,j2-2,j3,ey)-8.*u2(j1+2,j2-1,j3,ey)+8.*
     & u2(j1+2,j2+1,j3,ey)-u2(j1+2,j2+2,j3,ey))/(12.*dr2(1)))/(12.*
     & dr2(0))
              vv2ss = (-u2(j1,j2-2,j3,ey)+16.*u2(j1,j2-1,j3,ey)-30.*u2(
     & j1,j2,j3,ey)+16.*u2(j1,j2+1,j3,ey)-u2(j1,j2+2,j3,ey))/(12.*dr2(
     & 1)**2)
               v2x = aj2rx*vv2r+aj2sx*vv2s
               v2y = aj2ry*vv2r+aj2sy*vv2s
               t1 = aj2rx**2
               t6 = aj2sx**2
               v2xx = t1*vv2rr+2*aj2rx*aj2sx*vv2rs+t6*vv2ss+aj2rxx*
     & vv2r+aj2sxx*vv2s
               t1 = aj2ry**2
               t6 = aj2sy**2
               v2yy = t1*vv2rr+2*aj2ry*aj2sy*vv2rs+t6*vv2ss+aj2ryy*
     & vv2r+aj2syy*vv2s
             v2Lap = v2xx+ v2yy
            f(0)=(u1x+v1y) - (u2x+v2y)
            f(1)=(an1*u1Lap+an2*v1Lap) - (an1*u2Lap+an2*v2Lap)
            f(2)=(v1x-u1y) - (v2x-u2y)
            f(3)=(tau1*u1Lap+tau2*v1Lap)/eps1 - (tau1*u2Lap+tau2*v2Lap)
     & /eps2
            f(4)=(u1xxx+u1xyy+v1xxy+v1yyy) - (u2xxx+u2xyy+v2xxy+v2yyy)
            f(5)=((v1xxx+v1xyy)-(u1xxy+u1yyy))/eps1 - ((v2xxx+v2xyy)-(
     & u2xxy+u2yyy))/eps2
            f(6)=(an1*u1LapSq+an2*v1LapSq)/eps1 - (an1*u2LapSq+an2*
     & v2LapSq)/eps2
            f(7)=(tau1*u1LapSq+tau2*v1LapSq)/eps1**2 - (tau1*u2LapSq+
     & tau2*v2LapSq)/eps2**2
            if( twilightZone.eq.1 )then
              call ogderiv(ep, 0,2,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, ex, uexx )
              call ogderiv(ep, 0,0,2,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, ex, ueyy )
              call ogderiv(ep, 0,2,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, ey, vexx )
              call ogderiv(ep, 0,0,2,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, ey, veyy )
              call ogderiv(ep, 0,2,1,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, ex, uexxy )
              call ogderiv(ep, 0,0,3,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, ex, ueyyy )
              call ogderiv(ep, 0,3,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, ey, vexxx )
              call ogderiv(ep, 0,1,2,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, ey, vexyy )
              call ogderiv(ep, 0,4,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, ex, uexxxx )
              call ogderiv(ep, 0,2,2,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, ex, uexxyy )
              call ogderiv(ep, 0,0,4,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, ex, ueyyyy )
              call ogderiv(ep, 0,4,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, ey, vexxxx )
              call ogderiv(ep, 0,2,2,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, ey, vexxyy )
              call ogderiv(ep, 0,0,4,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, ey, veyyyy )
              ueLap = uexx + ueyy
              veLap = vexx + veyy
              ueLapSq = uexxxx + 2.*uexxyy + ueyyyy
              veLapSq = vexxxx + 2.*vexxyy + veyyyy
              f(3) = f(3) - ( tau1*ueLap +tau2*veLap )*(1./eps1-
     & 1./eps2)
              f(5) = f(5) - ((vexxx+vexyy)-(uexxy+ueyyy))*(1./eps1-
     & 1./eps2)
              f(6) = f(6) - (an1*ueLapSq+an2*veLapSq)*(1./eps1-1./eps2)
              f(7) = f(7) - (tau1*ueLapSq+tau2*veLapSq)*(1./eps1**2 - 
     & 1./eps2**2)
            end if

           if( debug.gt.3 ) write(debugFile,'(" --> 4cth: i1,i2=",2i4,
     & " f(re-eval)=",8e10.2)') i1,i2,f(0),f(1),f(2),f(3),f(4),f(5),f(
     & 6),f(7)
             ! '
          end if

           ! ******************************************************
           ! solve for Hz
           !  [ w.n/eps ] = 0
           !  [ lap(w)/eps ] = 0
           !  [ lap(w).n/eps**2 ] = 0
           !  [ lapSq(w)/eps**2 ] = 0

           ! first evaluate the equations we want to solve with the wrong values at the ghost points:
            ! These derivatives are computed to 4th-order accuracy
             ! this next call will define the jacobian and its derivatives (parameteric and spatial)
             aj1rx = rsxy1(i1,i2,i3,0,0)
             aj1rxr = (rsxy1(i1-2,i2,i3,0,0)-8.*rsxy1(i1-1,i2,i3,0,0)+
     & 8.*rsxy1(i1+1,i2,i3,0,0)-rsxy1(i1+2,i2,i3,0,0))/(12.*dr1(0))
             aj1rxs = (rsxy1(i1,i2-2,i3,0,0)-8.*rsxy1(i1,i2-1,i3,0,0)+
     & 8.*rsxy1(i1,i2+1,i3,0,0)-rsxy1(i1,i2+2,i3,0,0))/(12.*dr1(1))
             aj1sx = rsxy1(i1,i2,i3,1,0)
             aj1sxr = (rsxy1(i1-2,i2,i3,1,0)-8.*rsxy1(i1-1,i2,i3,1,0)+
     & 8.*rsxy1(i1+1,i2,i3,1,0)-rsxy1(i1+2,i2,i3,1,0))/(12.*dr1(0))
             aj1sxs = (rsxy1(i1,i2-2,i3,1,0)-8.*rsxy1(i1,i2-1,i3,1,0)+
     & 8.*rsxy1(i1,i2+1,i3,1,0)-rsxy1(i1,i2+2,i3,1,0))/(12.*dr1(1))
             aj1ry = rsxy1(i1,i2,i3,0,1)
             aj1ryr = (rsxy1(i1-2,i2,i3,0,1)-8.*rsxy1(i1-1,i2,i3,0,1)+
     & 8.*rsxy1(i1+1,i2,i3,0,1)-rsxy1(i1+2,i2,i3,0,1))/(12.*dr1(0))
             aj1rys = (rsxy1(i1,i2-2,i3,0,1)-8.*rsxy1(i1,i2-1,i3,0,1)+
     & 8.*rsxy1(i1,i2+1,i3,0,1)-rsxy1(i1,i2+2,i3,0,1))/(12.*dr1(1))
             aj1sy = rsxy1(i1,i2,i3,1,1)
             aj1syr = (rsxy1(i1-2,i2,i3,1,1)-8.*rsxy1(i1-1,i2,i3,1,1)+
     & 8.*rsxy1(i1+1,i2,i3,1,1)-rsxy1(i1+2,i2,i3,1,1))/(12.*dr1(0))
             aj1sys = (rsxy1(i1,i2-2,i3,1,1)-8.*rsxy1(i1,i2-1,i3,1,1)+
     & 8.*rsxy1(i1,i2+1,i3,1,1)-rsxy1(i1,i2+2,i3,1,1))/(12.*dr1(1))
             aj1rxx = aj1rx*aj1rxr+aj1sx*aj1rxs
             aj1rxy = aj1ry*aj1rxr+aj1sy*aj1rxs
             aj1sxx = aj1rx*aj1sxr+aj1sx*aj1sxs
             aj1sxy = aj1ry*aj1sxr+aj1sy*aj1sxs
             aj1ryx = aj1rx*aj1ryr+aj1sx*aj1rys
             aj1ryy = aj1ry*aj1ryr+aj1sy*aj1rys
             aj1syx = aj1rx*aj1syr+aj1sx*aj1sys
             aj1syy = aj1ry*aj1syr+aj1sy*aj1sys
              ww1 = u1(i1,i2,i3,hz)
              ww1r = (u1(i1-2,i2,i3,hz)-8.*u1(i1-1,i2,i3,hz)+8.*u1(i1+
     & 1,i2,i3,hz)-u1(i1+2,i2,i3,hz))/(12.*dr1(0))
              ww1s = (u1(i1,i2-2,i3,hz)-8.*u1(i1,i2-1,i3,hz)+8.*u1(i1,
     & i2+1,i3,hz)-u1(i1,i2+2,i3,hz))/(12.*dr1(1))
              ww1rr = (-u1(i1-2,i2,i3,hz)+16.*u1(i1-1,i2,i3,hz)-30.*u1(
     & i1,i2,i3,hz)+16.*u1(i1+1,i2,i3,hz)-u1(i1+2,i2,i3,hz))/(12.*dr1(
     & 0)**2)
              ww1rs = ((u1(i1-2,i2-2,i3,hz)-8.*u1(i1-2,i2-1,i3,hz)+8.*
     & u1(i1-2,i2+1,i3,hz)-u1(i1-2,i2+2,i3,hz))/(12.*dr1(1))-8.*(u1(
     & i1-1,i2-2,i3,hz)-8.*u1(i1-1,i2-1,i3,hz)+8.*u1(i1-1,i2+1,i3,hz)-
     & u1(i1-1,i2+2,i3,hz))/(12.*dr1(1))+8.*(u1(i1+1,i2-2,i3,hz)-8.*
     & u1(i1+1,i2-1,i3,hz)+8.*u1(i1+1,i2+1,i3,hz)-u1(i1+1,i2+2,i3,hz))
     & /(12.*dr1(1))-(u1(i1+2,i2-2,i3,hz)-8.*u1(i1+2,i2-1,i3,hz)+8.*
     & u1(i1+2,i2+1,i3,hz)-u1(i1+2,i2+2,i3,hz))/(12.*dr1(1)))/(12.*
     & dr1(0))
              ww1ss = (-u1(i1,i2-2,i3,hz)+16.*u1(i1,i2-1,i3,hz)-30.*u1(
     & i1,i2,i3,hz)+16.*u1(i1,i2+1,i3,hz)-u1(i1,i2+2,i3,hz))/(12.*dr1(
     & 1)**2)
               w1x = aj1rx*ww1r+aj1sx*ww1s
               w1y = aj1ry*ww1r+aj1sy*ww1s
               t1 = aj1rx**2
               t6 = aj1sx**2
               w1xx = t1*ww1rr+2*aj1rx*aj1sx*ww1rs+t6*ww1ss+aj1rxx*
     & ww1r+aj1sxx*ww1s
               t1 = aj1ry**2
               t6 = aj1sy**2
               w1yy = t1*ww1rr+2*aj1ry*aj1sy*ww1rs+t6*ww1ss+aj1ryy*
     & ww1r+aj1syy*ww1s
             w1Lap = w1xx+ w1yy
             ! this next call will define the jacobian and its derivatives (parameteric and spatial)
             aj2rx = rsxy2(j1,j2,j3,0,0)
             aj2rxr = (rsxy2(j1-2,j2,j3,0,0)-8.*rsxy2(j1-1,j2,j3,0,0)+
     & 8.*rsxy2(j1+1,j2,j3,0,0)-rsxy2(j1+2,j2,j3,0,0))/(12.*dr2(0))
             aj2rxs = (rsxy2(j1,j2-2,j3,0,0)-8.*rsxy2(j1,j2-1,j3,0,0)+
     & 8.*rsxy2(j1,j2+1,j3,0,0)-rsxy2(j1,j2+2,j3,0,0))/(12.*dr2(1))
             aj2sx = rsxy2(j1,j2,j3,1,0)
             aj2sxr = (rsxy2(j1-2,j2,j3,1,0)-8.*rsxy2(j1-1,j2,j3,1,0)+
     & 8.*rsxy2(j1+1,j2,j3,1,0)-rsxy2(j1+2,j2,j3,1,0))/(12.*dr2(0))
             aj2sxs = (rsxy2(j1,j2-2,j3,1,0)-8.*rsxy2(j1,j2-1,j3,1,0)+
     & 8.*rsxy2(j1,j2+1,j3,1,0)-rsxy2(j1,j2+2,j3,1,0))/(12.*dr2(1))
             aj2ry = rsxy2(j1,j2,j3,0,1)
             aj2ryr = (rsxy2(j1-2,j2,j3,0,1)-8.*rsxy2(j1-1,j2,j3,0,1)+
     & 8.*rsxy2(j1+1,j2,j3,0,1)-rsxy2(j1+2,j2,j3,0,1))/(12.*dr2(0))
             aj2rys = (rsxy2(j1,j2-2,j3,0,1)-8.*rsxy2(j1,j2-1,j3,0,1)+
     & 8.*rsxy2(j1,j2+1,j3,0,1)-rsxy2(j1,j2+2,j3,0,1))/(12.*dr2(1))
             aj2sy = rsxy2(j1,j2,j3,1,1)
             aj2syr = (rsxy2(j1-2,j2,j3,1,1)-8.*rsxy2(j1-1,j2,j3,1,1)+
     & 8.*rsxy2(j1+1,j2,j3,1,1)-rsxy2(j1+2,j2,j3,1,1))/(12.*dr2(0))
             aj2sys = (rsxy2(j1,j2-2,j3,1,1)-8.*rsxy2(j1,j2-1,j3,1,1)+
     & 8.*rsxy2(j1,j2+1,j3,1,1)-rsxy2(j1,j2+2,j3,1,1))/(12.*dr2(1))
             aj2rxx = aj2rx*aj2rxr+aj2sx*aj2rxs
             aj2rxy = aj2ry*aj2rxr+aj2sy*aj2rxs
             aj2sxx = aj2rx*aj2sxr+aj2sx*aj2sxs
             aj2sxy = aj2ry*aj2sxr+aj2sy*aj2sxs
             aj2ryx = aj2rx*aj2ryr+aj2sx*aj2rys
             aj2ryy = aj2ry*aj2ryr+aj2sy*aj2rys
             aj2syx = aj2rx*aj2syr+aj2sx*aj2sys
             aj2syy = aj2ry*aj2syr+aj2sy*aj2sys
              ww2 = u2(j1,j2,j3,hz)
              ww2r = (u2(j1-2,j2,j3,hz)-8.*u2(j1-1,j2,j3,hz)+8.*u2(j1+
     & 1,j2,j3,hz)-u2(j1+2,j2,j3,hz))/(12.*dr2(0))
              ww2s = (u2(j1,j2-2,j3,hz)-8.*u2(j1,j2-1,j3,hz)+8.*u2(j1,
     & j2+1,j3,hz)-u2(j1,j2+2,j3,hz))/(12.*dr2(1))
              ww2rr = (-u2(j1-2,j2,j3,hz)+16.*u2(j1-1,j2,j3,hz)-30.*u2(
     & j1,j2,j3,hz)+16.*u2(j1+1,j2,j3,hz)-u2(j1+2,j2,j3,hz))/(12.*dr2(
     & 0)**2)
              ww2rs = ((u2(j1-2,j2-2,j3,hz)-8.*u2(j1-2,j2-1,j3,hz)+8.*
     & u2(j1-2,j2+1,j3,hz)-u2(j1-2,j2+2,j3,hz))/(12.*dr2(1))-8.*(u2(
     & j1-1,j2-2,j3,hz)-8.*u2(j1-1,j2-1,j3,hz)+8.*u2(j1-1,j2+1,j3,hz)-
     & u2(j1-1,j2+2,j3,hz))/(12.*dr2(1))+8.*(u2(j1+1,j2-2,j3,hz)-8.*
     & u2(j1+1,j2-1,j3,hz)+8.*u2(j1+1,j2+1,j3,hz)-u2(j1+1,j2+2,j3,hz))
     & /(12.*dr2(1))-(u2(j1+2,j2-2,j3,hz)-8.*u2(j1+2,j2-1,j3,hz)+8.*
     & u2(j1+2,j2+1,j3,hz)-u2(j1+2,j2+2,j3,hz))/(12.*dr2(1)))/(12.*
     & dr2(0))
              ww2ss = (-u2(j1,j2-2,j3,hz)+16.*u2(j1,j2-1,j3,hz)-30.*u2(
     & j1,j2,j3,hz)+16.*u2(j1,j2+1,j3,hz)-u2(j1,j2+2,j3,hz))/(12.*dr2(
     & 1)**2)
               w2x = aj2rx*ww2r+aj2sx*ww2s
               w2y = aj2ry*ww2r+aj2sy*ww2s
               t1 = aj2rx**2
               t6 = aj2sx**2
               w2xx = t1*ww2rr+2*aj2rx*aj2sx*ww2rs+t6*ww2ss+aj2rxx*
     & ww2r+aj2sxx*ww2s
               t1 = aj2ry**2
               t6 = aj2sy**2
               w2yy = t1*ww2rr+2*aj2ry*aj2sy*ww2rs+t6*ww2ss+aj2ryy*
     & ww2r+aj2syy*ww2s
             w2Lap = w2xx+ w2yy
            ! These derivatives are computed to 2nd-order accuracy
             ! this next call will define the jacobian and its derivatives (parameteric and spatial)
             aj1rx = rsxy1(i1,i2,i3,0,0)
             aj1rxr = (-rsxy1(i1-1,i2,i3,0,0)+rsxy1(i1+1,i2,i3,0,0))/(
     & 2.*dr1(0))
             aj1rxs = (-rsxy1(i1,i2-1,i3,0,0)+rsxy1(i1,i2+1,i3,0,0))/(
     & 2.*dr1(1))
             aj1sx = rsxy1(i1,i2,i3,1,0)
             aj1sxr = (-rsxy1(i1-1,i2,i3,1,0)+rsxy1(i1+1,i2,i3,1,0))/(
     & 2.*dr1(0))
             aj1sxs = (-rsxy1(i1,i2-1,i3,1,0)+rsxy1(i1,i2+1,i3,1,0))/(
     & 2.*dr1(1))
             aj1ry = rsxy1(i1,i2,i3,0,1)
             aj1ryr = (-rsxy1(i1-1,i2,i3,0,1)+rsxy1(i1+1,i2,i3,0,1))/(
     & 2.*dr1(0))
             aj1rys = (-rsxy1(i1,i2-1,i3,0,1)+rsxy1(i1,i2+1,i3,0,1))/(
     & 2.*dr1(1))
             aj1sy = rsxy1(i1,i2,i3,1,1)
             aj1syr = (-rsxy1(i1-1,i2,i3,1,1)+rsxy1(i1+1,i2,i3,1,1))/(
     & 2.*dr1(0))
             aj1sys = (-rsxy1(i1,i2-1,i3,1,1)+rsxy1(i1,i2+1,i3,1,1))/(
     & 2.*dr1(1))
             aj1rxx = aj1rx*aj1rxr+aj1sx*aj1rxs
             aj1rxy = aj1ry*aj1rxr+aj1sy*aj1rxs
             aj1sxx = aj1rx*aj1sxr+aj1sx*aj1sxs
             aj1sxy = aj1ry*aj1sxr+aj1sy*aj1sxs
             aj1ryx = aj1rx*aj1ryr+aj1sx*aj1rys
             aj1ryy = aj1ry*aj1ryr+aj1sy*aj1rys
             aj1syx = aj1rx*aj1syr+aj1sx*aj1sys
             aj1syy = aj1ry*aj1syr+aj1sy*aj1sys
              ww1 = u1(i1,i2,i3,hz)
              ww1r = (-u1(i1-1,i2,i3,hz)+u1(i1+1,i2,i3,hz))/(2.*dr1(0))
              ww1s = (-u1(i1,i2-1,i3,hz)+u1(i1,i2+1,i3,hz))/(2.*dr1(1))
              ww1rr = (u1(i1-1,i2,i3,hz)-2.*u1(i1,i2,i3,hz)+u1(i1+1,i2,
     & i3,hz))/(dr1(0)**2)
              ww1rs = (-(-u1(i1-1,i2-1,i3,hz)+u1(i1-1,i2+1,i3,hz))/(2.*
     & dr1(1))+(-u1(i1+1,i2-1,i3,hz)+u1(i1+1,i2+1,i3,hz))/(2.*dr1(1)))
     & /(2.*dr1(0))
              ww1ss = (u1(i1,i2-1,i3,hz)-2.*u1(i1,i2,i3,hz)+u1(i1,i2+1,
     & i3,hz))/(dr1(1)**2)
              ww1rrr = (-u1(i1-2,i2,i3,hz)+2.*u1(i1-1,i2,i3,hz)-2.*u1(
     & i1+1,i2,i3,hz)+u1(i1+2,i2,i3,hz))/(2.*dr1(0)**3)
              ww1rrs = ((-u1(i1-1,i2-1,i3,hz)+u1(i1-1,i2+1,i3,hz))/(2.*
     & dr1(1))-2.*(-u1(i1,i2-1,i3,hz)+u1(i1,i2+1,i3,hz))/(2.*dr1(1))+(
     & -u1(i1+1,i2-1,i3,hz)+u1(i1+1,i2+1,i3,hz))/(2.*dr1(1)))/(dr1(0)*
     & *2)
              ww1rss = (-(u1(i1-1,i2-1,i3,hz)-2.*u1(i1-1,i2,i3,hz)+u1(
     & i1-1,i2+1,i3,hz))/(dr1(1)**2)+(u1(i1+1,i2-1,i3,hz)-2.*u1(i1+1,
     & i2,i3,hz)+u1(i1+1,i2+1,i3,hz))/(dr1(1)**2))/(2.*dr1(0))
              ww1sss = (-u1(i1,i2-2,i3,hz)+2.*u1(i1,i2-1,i3,hz)-2.*u1(
     & i1,i2+1,i3,hz)+u1(i1,i2+2,i3,hz))/(2.*dr1(1)**3)
              ww1rrrr = (u1(i1-2,i2,i3,hz)-4.*u1(i1-1,i2,i3,hz)+6.*u1(
     & i1,i2,i3,hz)-4.*u1(i1+1,i2,i3,hz)+u1(i1+2,i2,i3,hz))/(dr1(0)**
     & 4)
              ww1rrrs = (-(-u1(i1-2,i2-1,i3,hz)+u1(i1-2,i2+1,i3,hz))/(
     & 2.*dr1(1))+2.*(-u1(i1-1,i2-1,i3,hz)+u1(i1-1,i2+1,i3,hz))/(2.*
     & dr1(1))-2.*(-u1(i1+1,i2-1,i3,hz)+u1(i1+1,i2+1,i3,hz))/(2.*dr1(
     & 1))+(-u1(i1+2,i2-1,i3,hz)+u1(i1+2,i2+1,i3,hz))/(2.*dr1(1)))/(
     & 2.*dr1(0)**3)
              ww1rrss = ((u1(i1-1,i2-1,i3,hz)-2.*u1(i1-1,i2,i3,hz)+u1(
     & i1-1,i2+1,i3,hz))/(dr1(1)**2)-2.*(u1(i1,i2-1,i3,hz)-2.*u1(i1,
     & i2,i3,hz)+u1(i1,i2+1,i3,hz))/(dr1(1)**2)+(u1(i1+1,i2-1,i3,hz)-
     & 2.*u1(i1+1,i2,i3,hz)+u1(i1+1,i2+1,i3,hz))/(dr1(1)**2))/(dr1(0)*
     & *2)
              ww1rsss = (-(-u1(i1-1,i2-2,i3,hz)+2.*u1(i1-1,i2-1,i3,hz)-
     & 2.*u1(i1-1,i2+1,i3,hz)+u1(i1-1,i2+2,i3,hz))/(2.*dr1(1)**3)+(-
     & u1(i1+1,i2-2,i3,hz)+2.*u1(i1+1,i2-1,i3,hz)-2.*u1(i1+1,i2+1,i3,
     & hz)+u1(i1+1,i2+2,i3,hz))/(2.*dr1(1)**3))/(2.*dr1(0))
              ww1ssss = (u1(i1,i2-2,i3,hz)-4.*u1(i1,i2-1,i3,hz)+6.*u1(
     & i1,i2,i3,hz)-4.*u1(i1,i2+1,i3,hz)+u1(i1,i2+2,i3,hz))/(dr1(1)**
     & 4)
               t1 = aj1rx**2
               t7 = aj1sx**2
               w1xxx = t1*aj1rx*ww1rrr+3*t1*aj1sx*ww1rrs+3*aj1rx*t7*
     & ww1rss+t7*aj1sx*ww1sss+3*aj1rx*aj1rxx*ww1rr+(3*aj1sxx*aj1rx+3*
     & aj1sx*aj1rxx)*ww1rs+3*aj1sxx*aj1sx*ww1ss+aj1rxxx*ww1r+aj1sxxx*
     & ww1s
               t1 = aj1rx**2
               t10 = aj1sx**2
               w1xxy = aj1ry*t1*ww1rrr+(aj1sy*t1+2*aj1ry*aj1sx*aj1rx)*
     & ww1rrs+(aj1ry*t10+2*aj1sy*aj1sx*aj1rx)*ww1rss+aj1sy*t10*ww1sss+
     & (2*aj1rxy*aj1rx+aj1ry*aj1rxx)*ww1rr+(aj1ry*aj1sxx+2*aj1sx*
     & aj1rxy+2*aj1sxy*aj1rx+aj1sy*aj1rxx)*ww1rs+(aj1sy*aj1sxx+2*
     & aj1sxy*aj1sx)*ww1ss+aj1rxxy*ww1r+aj1sxxy*ww1s
               t1 = aj1ry**2
               t4 = aj1sy*aj1ry
               t8 = aj1sy*aj1rx+aj1ry*aj1sx
               t16 = aj1sy**2
               w1xyy = t1*aj1rx*ww1rrr+(t4*aj1rx+aj1ry*t8)*ww1rrs+(t4*
     & aj1sx+aj1sy*t8)*ww1rss+t16*aj1sx*ww1sss+(aj1ryy*aj1rx+2*aj1ry*
     & aj1rxy)*ww1rr+(2*aj1ry*aj1sxy+2*aj1sy*aj1rxy+aj1ryy*aj1sx+
     & aj1syy*aj1rx)*ww1rs+(aj1syy*aj1sx+2*aj1sy*aj1sxy)*ww1ss+
     & aj1rxyy*ww1r+aj1sxyy*ww1s
               t1 = aj1ry**2
               t7 = aj1sy**2
               w1yyy = aj1ry*t1*ww1rrr+3*t1*aj1sy*ww1rrs+3*aj1ry*t7*
     & ww1rss+t7*aj1sy*ww1sss+3*aj1ry*aj1ryy*ww1rr+(3*aj1syy*aj1ry+3*
     & aj1sy*aj1ryy)*ww1rs+3*aj1syy*aj1sy*ww1ss+aj1ryyy*ww1r+aj1syyy*
     & ww1s
               t1 = aj1rx**2
               t2 = t1**2
               t8 = aj1sx**2
               t16 = t8**2
               t25 = aj1sxx*aj1rx
               t27 = t25+aj1sx*aj1rxx
               t28 = 3*t27
               t30 = 2*t27
               t46 = aj1rxx**2
               t60 = aj1sxx**2
               w1xxxx = t2*ww1rrrr+4*t1*aj1rx*aj1sx*ww1rrrs+6*t1*t8*
     & ww1rrss+4*aj1rx*t8*aj1sx*ww1rsss+t16*ww1ssss+6*t1*aj1rxx*
     & ww1rrr+(7*aj1sx*aj1rx*aj1rxx+aj1sxx*t1+aj1rx*t28+aj1rx*t30)*
     & ww1rrs+(aj1sx*t28+7*t25*aj1sx+aj1rxx*t8+aj1sx*t30)*ww1rss+6*t8*
     & aj1sxx*ww1sss+(4*aj1rx*aj1rxxx+3*t46)*ww1rr+(4*aj1sxxx*aj1rx+4*
     & aj1sx*aj1rxxx+6*aj1sxx*aj1rxx)*ww1rs+(4*aj1sxxx*aj1sx+3*t60)*
     & ww1ss+aj1rxxxx*ww1r+aj1sxxxx*ww1s
               t1 = aj1ry**2
               t2 = aj1rx**2
               t5 = aj1sy*aj1ry
               t11 = aj1sy*t2+2*aj1ry*aj1sx*aj1rx
               t16 = aj1sx**2
               t21 = aj1ry*t16+2*aj1sy*aj1sx*aj1rx
               t29 = aj1sy**2
               t38 = 2*aj1rxy*aj1rx+aj1ry*aj1rxx
               t52 = aj1sx*aj1rxy
               t54 = aj1sxy*aj1rx
               t57 = aj1ry*aj1sxx+2*t52+2*t54+aj1sy*aj1rxx
               t60 = 2*t52+2*t54
               t68 = aj1sy*aj1sxx+2*aj1sxy*aj1sx
               t92 = aj1rxy**2
               t110 = aj1sxy**2
               w1xxyy = t1*t2*ww1rrrr+(t5*t2+aj1ry*t11)*ww1rrrs+(aj1sy*
     & t11+aj1ry*t21)*ww1rrss+(aj1sy*t21+t5*t16)*ww1rsss+t29*t16*
     & ww1ssss+(2*aj1ry*aj1rxy*aj1rx+aj1ry*t38+aj1ryy*t2)*ww1rrr+(
     & aj1sy*t38+2*aj1sy*aj1rxy*aj1rx+2*aj1ryy*aj1sx*aj1rx+aj1syy*t2+
     & aj1ry*t57+aj1ry*t60)*ww1rrs+(aj1sy*t57+aj1ry*t68+aj1ryy*t16+2*
     & aj1ry*aj1sxy*aj1sx+2*aj1syy*aj1sx*aj1rx+aj1sy*t60)*ww1rss+(2*
     & aj1sy*aj1sxy*aj1sx+aj1sy*t68+aj1syy*t16)*ww1sss+(2*aj1rx*
     & aj1rxyy+aj1ryy*aj1rxx+2*aj1ry*aj1rxxy+2*t92)*ww1rr+(4*aj1sxy*
     & aj1rxy+2*aj1ry*aj1sxxy+aj1ryy*aj1sxx+2*aj1sy*aj1rxxy+2*aj1sxyy*
     & aj1rx+aj1syy*aj1rxx+2*aj1sx*aj1rxyy)*ww1rs+(2*t110+2*aj1sy*
     & aj1sxxy+aj1syy*aj1sxx+2*aj1sx*aj1sxyy)*ww1ss+aj1rxxyy*ww1r+
     & aj1sxxyy*ww1s
               t1 = aj1ry**2
               t2 = t1**2
               t8 = aj1sy**2
               t16 = t8**2
               t25 = aj1syy*aj1ry
               t27 = t25+aj1sy*aj1ryy
               t28 = 3*t27
               t30 = 2*t27
               t46 = aj1ryy**2
               t60 = aj1syy**2
               w1yyyy = t2*ww1rrrr+4*t1*aj1ry*aj1sy*ww1rrrs+6*t1*t8*
     & ww1rrss+4*aj1ry*t8*aj1sy*ww1rsss+t16*ww1ssss+6*t1*aj1ryy*
     & ww1rrr+(7*aj1sy*aj1ry*aj1ryy+aj1syy*t1+aj1ry*t28+aj1ry*t30)*
     & ww1rrs+(aj1sy*t28+7*t25*aj1sy+aj1ryy*t8+aj1sy*t30)*ww1rss+6*t8*
     & aj1syy*ww1sss+(4*aj1ry*aj1ryyy+3*t46)*ww1rr+(4*aj1syyy*aj1ry+4*
     & aj1sy*aj1ryyy+6*aj1syy*aj1ryy)*ww1rs+(4*aj1syyy*aj1sy+3*t60)*
     & ww1ss+aj1ryyyy*ww1r+aj1syyyy*ww1s
             w1LapSq = w1xxxx +2.* w1xxyy + w1yyyy
             ! this next call will define the jacobian and its derivatives (parameteric and spatial)
             aj2rx = rsxy2(j1,j2,j3,0,0)
             aj2rxr = (-rsxy2(j1-1,j2,j3,0,0)+rsxy2(j1+1,j2,j3,0,0))/(
     & 2.*dr2(0))
             aj2rxs = (-rsxy2(j1,j2-1,j3,0,0)+rsxy2(j1,j2+1,j3,0,0))/(
     & 2.*dr2(1))
             aj2sx = rsxy2(j1,j2,j3,1,0)
             aj2sxr = (-rsxy2(j1-1,j2,j3,1,0)+rsxy2(j1+1,j2,j3,1,0))/(
     & 2.*dr2(0))
             aj2sxs = (-rsxy2(j1,j2-1,j3,1,0)+rsxy2(j1,j2+1,j3,1,0))/(
     & 2.*dr2(1))
             aj2ry = rsxy2(j1,j2,j3,0,1)
             aj2ryr = (-rsxy2(j1-1,j2,j3,0,1)+rsxy2(j1+1,j2,j3,0,1))/(
     & 2.*dr2(0))
             aj2rys = (-rsxy2(j1,j2-1,j3,0,1)+rsxy2(j1,j2+1,j3,0,1))/(
     & 2.*dr2(1))
             aj2sy = rsxy2(j1,j2,j3,1,1)
             aj2syr = (-rsxy2(j1-1,j2,j3,1,1)+rsxy2(j1+1,j2,j3,1,1))/(
     & 2.*dr2(0))
             aj2sys = (-rsxy2(j1,j2-1,j3,1,1)+rsxy2(j1,j2+1,j3,1,1))/(
     & 2.*dr2(1))
             aj2rxx = aj2rx*aj2rxr+aj2sx*aj2rxs
             aj2rxy = aj2ry*aj2rxr+aj2sy*aj2rxs
             aj2sxx = aj2rx*aj2sxr+aj2sx*aj2sxs
             aj2sxy = aj2ry*aj2sxr+aj2sy*aj2sxs
             aj2ryx = aj2rx*aj2ryr+aj2sx*aj2rys
             aj2ryy = aj2ry*aj2ryr+aj2sy*aj2rys
             aj2syx = aj2rx*aj2syr+aj2sx*aj2sys
             aj2syy = aj2ry*aj2syr+aj2sy*aj2sys
              ww2 = u2(j1,j2,j3,hz)
              ww2r = (-u2(j1-1,j2,j3,hz)+u2(j1+1,j2,j3,hz))/(2.*dr2(0))
              ww2s = (-u2(j1,j2-1,j3,hz)+u2(j1,j2+1,j3,hz))/(2.*dr2(1))
              ww2rr = (u2(j1-1,j2,j3,hz)-2.*u2(j1,j2,j3,hz)+u2(j1+1,j2,
     & j3,hz))/(dr2(0)**2)
              ww2rs = (-(-u2(j1-1,j2-1,j3,hz)+u2(j1-1,j2+1,j3,hz))/(2.*
     & dr2(1))+(-u2(j1+1,j2-1,j3,hz)+u2(j1+1,j2+1,j3,hz))/(2.*dr2(1)))
     & /(2.*dr2(0))
              ww2ss = (u2(j1,j2-1,j3,hz)-2.*u2(j1,j2,j3,hz)+u2(j1,j2+1,
     & j3,hz))/(dr2(1)**2)
              ww2rrr = (-u2(j1-2,j2,j3,hz)+2.*u2(j1-1,j2,j3,hz)-2.*u2(
     & j1+1,j2,j3,hz)+u2(j1+2,j2,j3,hz))/(2.*dr2(0)**3)
              ww2rrs = ((-u2(j1-1,j2-1,j3,hz)+u2(j1-1,j2+1,j3,hz))/(2.*
     & dr2(1))-2.*(-u2(j1,j2-1,j3,hz)+u2(j1,j2+1,j3,hz))/(2.*dr2(1))+(
     & -u2(j1+1,j2-1,j3,hz)+u2(j1+1,j2+1,j3,hz))/(2.*dr2(1)))/(dr2(0)*
     & *2)
              ww2rss = (-(u2(j1-1,j2-1,j3,hz)-2.*u2(j1-1,j2,j3,hz)+u2(
     & j1-1,j2+1,j3,hz))/(dr2(1)**2)+(u2(j1+1,j2-1,j3,hz)-2.*u2(j1+1,
     & j2,j3,hz)+u2(j1+1,j2+1,j3,hz))/(dr2(1)**2))/(2.*dr2(0))
              ww2sss = (-u2(j1,j2-2,j3,hz)+2.*u2(j1,j2-1,j3,hz)-2.*u2(
     & j1,j2+1,j3,hz)+u2(j1,j2+2,j3,hz))/(2.*dr2(1)**3)
              ww2rrrr = (u2(j1-2,j2,j3,hz)-4.*u2(j1-1,j2,j3,hz)+6.*u2(
     & j1,j2,j3,hz)-4.*u2(j1+1,j2,j3,hz)+u2(j1+2,j2,j3,hz))/(dr2(0)**
     & 4)
              ww2rrrs = (-(-u2(j1-2,j2-1,j3,hz)+u2(j1-2,j2+1,j3,hz))/(
     & 2.*dr2(1))+2.*(-u2(j1-1,j2-1,j3,hz)+u2(j1-1,j2+1,j3,hz))/(2.*
     & dr2(1))-2.*(-u2(j1+1,j2-1,j3,hz)+u2(j1+1,j2+1,j3,hz))/(2.*dr2(
     & 1))+(-u2(j1+2,j2-1,j3,hz)+u2(j1+2,j2+1,j3,hz))/(2.*dr2(1)))/(
     & 2.*dr2(0)**3)
              ww2rrss = ((u2(j1-1,j2-1,j3,hz)-2.*u2(j1-1,j2,j3,hz)+u2(
     & j1-1,j2+1,j3,hz))/(dr2(1)**2)-2.*(u2(j1,j2-1,j3,hz)-2.*u2(j1,
     & j2,j3,hz)+u2(j1,j2+1,j3,hz))/(dr2(1)**2)+(u2(j1+1,j2-1,j3,hz)-
     & 2.*u2(j1+1,j2,j3,hz)+u2(j1+1,j2+1,j3,hz))/(dr2(1)**2))/(dr2(0)*
     & *2)
              ww2rsss = (-(-u2(j1-1,j2-2,j3,hz)+2.*u2(j1-1,j2-1,j3,hz)-
     & 2.*u2(j1-1,j2+1,j3,hz)+u2(j1-1,j2+2,j3,hz))/(2.*dr2(1)**3)+(-
     & u2(j1+1,j2-2,j3,hz)+2.*u2(j1+1,j2-1,j3,hz)-2.*u2(j1+1,j2+1,j3,
     & hz)+u2(j1+1,j2+2,j3,hz))/(2.*dr2(1)**3))/(2.*dr2(0))
              ww2ssss = (u2(j1,j2-2,j3,hz)-4.*u2(j1,j2-1,j3,hz)+6.*u2(
     & j1,j2,j3,hz)-4.*u2(j1,j2+1,j3,hz)+u2(j1,j2+2,j3,hz))/(dr2(1)**
     & 4)
               t1 = aj2rx**2
               t7 = aj2sx**2
               w2xxx = t1*aj2rx*ww2rrr+3*t1*aj2sx*ww2rrs+3*aj2rx*t7*
     & ww2rss+t7*aj2sx*ww2sss+3*aj2rx*aj2rxx*ww2rr+(3*aj2sxx*aj2rx+3*
     & aj2sx*aj2rxx)*ww2rs+3*aj2sxx*aj2sx*ww2ss+aj2rxxx*ww2r+aj2sxxx*
     & ww2s
               t1 = aj2rx**2
               t10 = aj2sx**2
               w2xxy = aj2ry*t1*ww2rrr+(aj2sy*t1+2*aj2ry*aj2sx*aj2rx)*
     & ww2rrs+(aj2ry*t10+2*aj2sy*aj2sx*aj2rx)*ww2rss+aj2sy*t10*ww2sss+
     & (2*aj2rxy*aj2rx+aj2ry*aj2rxx)*ww2rr+(aj2ry*aj2sxx+2*aj2sx*
     & aj2rxy+2*aj2sxy*aj2rx+aj2sy*aj2rxx)*ww2rs+(aj2sy*aj2sxx+2*
     & aj2sxy*aj2sx)*ww2ss+aj2rxxy*ww2r+aj2sxxy*ww2s
               t1 = aj2ry**2
               t4 = aj2sy*aj2ry
               t8 = aj2sy*aj2rx+aj2ry*aj2sx
               t16 = aj2sy**2
               w2xyy = t1*aj2rx*ww2rrr+(t4*aj2rx+aj2ry*t8)*ww2rrs+(t4*
     & aj2sx+aj2sy*t8)*ww2rss+t16*aj2sx*ww2sss+(aj2ryy*aj2rx+2*aj2ry*
     & aj2rxy)*ww2rr+(2*aj2ry*aj2sxy+2*aj2sy*aj2rxy+aj2ryy*aj2sx+
     & aj2syy*aj2rx)*ww2rs+(aj2syy*aj2sx+2*aj2sy*aj2sxy)*ww2ss+
     & aj2rxyy*ww2r+aj2sxyy*ww2s
               t1 = aj2ry**2
               t7 = aj2sy**2
               w2yyy = aj2ry*t1*ww2rrr+3*t1*aj2sy*ww2rrs+3*aj2ry*t7*
     & ww2rss+t7*aj2sy*ww2sss+3*aj2ry*aj2ryy*ww2rr+(3*aj2syy*aj2ry+3*
     & aj2sy*aj2ryy)*ww2rs+3*aj2syy*aj2sy*ww2ss+aj2ryyy*ww2r+aj2syyy*
     & ww2s
               t1 = aj2rx**2
               t2 = t1**2
               t8 = aj2sx**2
               t16 = t8**2
               t25 = aj2sxx*aj2rx
               t27 = t25+aj2sx*aj2rxx
               t28 = 3*t27
               t30 = 2*t27
               t46 = aj2rxx**2
               t60 = aj2sxx**2
               w2xxxx = t2*ww2rrrr+4*t1*aj2rx*aj2sx*ww2rrrs+6*t1*t8*
     & ww2rrss+4*aj2rx*t8*aj2sx*ww2rsss+t16*ww2ssss+6*t1*aj2rxx*
     & ww2rrr+(7*aj2sx*aj2rx*aj2rxx+aj2sxx*t1+aj2rx*t28+aj2rx*t30)*
     & ww2rrs+(aj2sx*t28+7*t25*aj2sx+aj2rxx*t8+aj2sx*t30)*ww2rss+6*t8*
     & aj2sxx*ww2sss+(4*aj2rx*aj2rxxx+3*t46)*ww2rr+(4*aj2sxxx*aj2rx+4*
     & aj2sx*aj2rxxx+6*aj2sxx*aj2rxx)*ww2rs+(4*aj2sxxx*aj2sx+3*t60)*
     & ww2ss+aj2rxxxx*ww2r+aj2sxxxx*ww2s
               t1 = aj2ry**2
               t2 = aj2rx**2
               t5 = aj2sy*aj2ry
               t11 = aj2sy*t2+2*aj2ry*aj2sx*aj2rx
               t16 = aj2sx**2
               t21 = aj2ry*t16+2*aj2sy*aj2sx*aj2rx
               t29 = aj2sy**2
               t38 = 2*aj2rxy*aj2rx+aj2ry*aj2rxx
               t52 = aj2sx*aj2rxy
               t54 = aj2sxy*aj2rx
               t57 = aj2ry*aj2sxx+2*t52+2*t54+aj2sy*aj2rxx
               t60 = 2*t52+2*t54
               t68 = aj2sy*aj2sxx+2*aj2sxy*aj2sx
               t92 = aj2rxy**2
               t110 = aj2sxy**2
               w2xxyy = t1*t2*ww2rrrr+(t5*t2+aj2ry*t11)*ww2rrrs+(aj2sy*
     & t11+aj2ry*t21)*ww2rrss+(aj2sy*t21+t5*t16)*ww2rsss+t29*t16*
     & ww2ssss+(2*aj2ry*aj2rxy*aj2rx+aj2ry*t38+aj2ryy*t2)*ww2rrr+(
     & aj2sy*t38+2*aj2sy*aj2rxy*aj2rx+2*aj2ryy*aj2sx*aj2rx+aj2syy*t2+
     & aj2ry*t57+aj2ry*t60)*ww2rrs+(aj2sy*t57+aj2ry*t68+aj2ryy*t16+2*
     & aj2ry*aj2sxy*aj2sx+2*aj2syy*aj2sx*aj2rx+aj2sy*t60)*ww2rss+(2*
     & aj2sy*aj2sxy*aj2sx+aj2sy*t68+aj2syy*t16)*ww2sss+(2*aj2rx*
     & aj2rxyy+aj2ryy*aj2rxx+2*aj2ry*aj2rxxy+2*t92)*ww2rr+(4*aj2sxy*
     & aj2rxy+2*aj2ry*aj2sxxy+aj2ryy*aj2sxx+2*aj2sy*aj2rxxy+2*aj2sxyy*
     & aj2rx+aj2syy*aj2rxx+2*aj2sx*aj2rxyy)*ww2rs+(2*t110+2*aj2sy*
     & aj2sxxy+aj2syy*aj2sxx+2*aj2sx*aj2sxyy)*ww2ss+aj2rxxyy*ww2r+
     & aj2sxxyy*ww2s
               t1 = aj2ry**2
               t2 = t1**2
               t8 = aj2sy**2
               t16 = t8**2
               t25 = aj2syy*aj2ry
               t27 = t25+aj2sy*aj2ryy
               t28 = 3*t27
               t30 = 2*t27
               t46 = aj2ryy**2
               t60 = aj2syy**2
               w2yyyy = t2*ww2rrrr+4*t1*aj2ry*aj2sy*ww2rrrs+6*t1*t8*
     & ww2rrss+4*aj2ry*t8*aj2sy*ww2rsss+t16*ww2ssss+6*t1*aj2ryy*
     & ww2rrr+(7*aj2sy*aj2ry*aj2ryy+aj2syy*t1+aj2ry*t28+aj2ry*t30)*
     & ww2rrs+(aj2sy*t28+7*t25*aj2sy+aj2ryy*t8+aj2sy*t30)*ww2rss+6*t8*
     & aj2syy*ww2sss+(4*aj2ry*aj2ryyy+3*t46)*ww2rr+(4*aj2syyy*aj2ry+4*
     & aj2sy*aj2ryyy+6*aj2syy*aj2ryy)*ww2rs+(4*aj2syyy*aj2sy+3*t60)*
     & ww2ss+aj2ryyyy*ww2r+aj2syyyy*ww2s
             w2LapSq = w2xxxx +2.* w2xxyy + w2yyyy
            f(0)=(an1*w1x+an2*w1y)/eps1 - (an1*w2x+an2*w2y)/eps2
            f(1)=w1Lap/eps1 - w2Lap/eps2
            f(2)=(an1*(w1xxx+w1xyy)+an2*(w1xxy+w1yyy))/eps1**2 - (an1*(
     & w2xxx+w2xyy)+an2*(w2xxy+w2yyy))/eps2**2
            f(3)=w1LapSq/eps1**2 - w2LapSq/eps2**2
            if( twilightZone.eq.1 )then
              call ogderiv(ep, 0,1,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, hz, wex  )
              call ogderiv(ep, 0,0,1,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, hz, wey  )
              call ogderiv(ep, 0,2,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, hz, wexx )
              call ogderiv(ep, 0,0,2,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, hz, weyy )
              call ogderiv(ep, 0,3,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, hz, wexxx )
              call ogderiv(ep, 0,2,1,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, hz, wexxy )
              call ogderiv(ep, 0,1,2,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, hz, wexyy )
              call ogderiv(ep, 0,0,3,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, hz, weyyy )
              call ogderiv(ep, 0,4,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, hz, wexxxx )
              call ogderiv(ep, 0,2,2,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, hz, wexxyy )
              call ogderiv(ep, 0,0,4,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, hz, weyyyy )
              weLap = wexx + weyy
              weLapSq = wexxxx + 2.*wexxyy + weyyyy
              f(0) = f(0) - (an1*wex+an2*wey)*(1./eps1 - 1./eps2)
              f(1) = f(1) - ( weLap )*(1./eps1 - 1./eps2)
              f(2) = f(2) - (an1*(wexxx+wexyy)+an2*(wexxy+weyyy))*(
     & 1./eps1**2 - 1./eps2**2)
              f(3) = f(3) - weLapSq*(1./eps1**2 - 1./eps2**2)
            end if

           if( .false. .or. (initialized.eq.0 .and. it.eq.1) )then
             ! form the matrix for computing Hz (and save factor for later use)

             ! 1: [ w.n/eps ] = 0
             a0 = (an1*rsxy1(i1,i2,i3,axis1,0)+an2*rsxy1(i1,i2,i3,
     & axis1,1))*dr114(axis1)/eps1
             b0 = (an1*rsxy2(j1,j2,j3,axis2,0)+an2*rsxy2(j1,j2,j3,
     & axis2,1))*dr214(axis2)/eps2
             aa4(0,0,0,nn) = -is*8.*a0
             aa4(0,2,0,nn) =  is*   a0
             aa4(0,1,0,nn) =  js*8.*b0
             aa4(0,3,0,nn) = -js*   b0

             ! 2: [ lap(w)/eps ] = 0 
             aa4(1,0,0,nn) = aLap0/eps1
             aa4(1,2,0,nn) = aLap1/eps1
             aa4(1,1,0,nn) =-bLap0/eps2
             aa4(1,3,0,nn) =-bLap1/eps2

             ! 3  [ (an1*(w.xx+w.yy).x + an2.(w.xx+w.yy).y)/eps**2 ] = 0
             aa4(2,0,0,nn)= (an1*aLapX0+an2*bLapY0)/eps1**2
             aa4(2,2,0,nn)= (an1*aLapX1+an2*bLapY1)/eps1**2
             aa4(2,1,0,nn)=-(an1*cLapX0+an2*dLapY0)/eps2**2
             aa4(2,3,0,nn)=-(an1*cLapX1+an2*dLapY1)/eps2**2

             ! 4 [ lapSq(w)/eps**2 ] = 0 
             aa4(3,0,0,nn) = aLapSq0/eps1**2
             aa4(3,2,0,nn) = aLapSq1/eps1**2
             aa4(3,1,0,nn) =-bLapSq0/eps2**2
             aa4(3,3,0,nn) =-bLapSq1/eps2**2

             ! save a copy of the matrix
             do n2=0,3
             do n1=0,3
               aa4(n1,n2,1,nn)=aa4(n1,n2,0,nn)
             end do
             end do

             ! factor the matrix
             numberOfEquations=4
             call dgeco( aa4(0,0,0,nn), numberOfEquations, 
     & numberOfEquations, ipvt4(0,nn),rcond,work(0))
           end if

           q(0) = u1(i1-is1,i2-is2,i3,hz)
           q(1) = u2(j1-js1,j2-js2,j3,hz)
           q(2) = u1(i1-2*is1,i2-2*is2,i3,hz)
           q(3) = u2(j1-2*js1,j2-2*js2,j3,hz)

           ! subtract off the contributions from the wrong values at the ghost points:
           do n=0,3
             f(n) = (aa4(n,0,1,nn)*q(0)+aa4(n,1,1,nn)*q(1)+aa4(n,2,1,
     & nn)*q(2)+aa4(n,3,1,nn)*q(3)) - f(n)
           end do
           ! solve
           numberOfEquations=4
           job=0
           call dgesl( aa4(0,0,0,nn), numberOfEquations, 
     & numberOfEquations, ipvt4(0,nn), f(0), job)

           u1(i1-is1,i2-is2,i3,hz)=f(0)
           u2(j1-js1,j2-js2,j3,hz)=f(1)
           u1(i1-2*is1,i2-2*is2,i3,hz)=f(2)
           u2(j1-2*js1,j2-2*js2,j3,hz)=f(3)

          if( debug.gt.0 )then ! re-evaluate

            ! These derivatives are computed to 4th-order accuracy
             ! this next call will define the jacobian and its derivatives (parameteric and spatial)
             aj1rx = rsxy1(i1,i2,i3,0,0)
             aj1rxr = (rsxy1(i1-2,i2,i3,0,0)-8.*rsxy1(i1-1,i2,i3,0,0)+
     & 8.*rsxy1(i1+1,i2,i3,0,0)-rsxy1(i1+2,i2,i3,0,0))/(12.*dr1(0))
             aj1rxs = (rsxy1(i1,i2-2,i3,0,0)-8.*rsxy1(i1,i2-1,i3,0,0)+
     & 8.*rsxy1(i1,i2+1,i3,0,0)-rsxy1(i1,i2+2,i3,0,0))/(12.*dr1(1))
             aj1sx = rsxy1(i1,i2,i3,1,0)
             aj1sxr = (rsxy1(i1-2,i2,i3,1,0)-8.*rsxy1(i1-1,i2,i3,1,0)+
     & 8.*rsxy1(i1+1,i2,i3,1,0)-rsxy1(i1+2,i2,i3,1,0))/(12.*dr1(0))
             aj1sxs = (rsxy1(i1,i2-2,i3,1,0)-8.*rsxy1(i1,i2-1,i3,1,0)+
     & 8.*rsxy1(i1,i2+1,i3,1,0)-rsxy1(i1,i2+2,i3,1,0))/(12.*dr1(1))
             aj1ry = rsxy1(i1,i2,i3,0,1)
             aj1ryr = (rsxy1(i1-2,i2,i3,0,1)-8.*rsxy1(i1-1,i2,i3,0,1)+
     & 8.*rsxy1(i1+1,i2,i3,0,1)-rsxy1(i1+2,i2,i3,0,1))/(12.*dr1(0))
             aj1rys = (rsxy1(i1,i2-2,i3,0,1)-8.*rsxy1(i1,i2-1,i3,0,1)+
     & 8.*rsxy1(i1,i2+1,i3,0,1)-rsxy1(i1,i2+2,i3,0,1))/(12.*dr1(1))
             aj1sy = rsxy1(i1,i2,i3,1,1)
             aj1syr = (rsxy1(i1-2,i2,i3,1,1)-8.*rsxy1(i1-1,i2,i3,1,1)+
     & 8.*rsxy1(i1+1,i2,i3,1,1)-rsxy1(i1+2,i2,i3,1,1))/(12.*dr1(0))
             aj1sys = (rsxy1(i1,i2-2,i3,1,1)-8.*rsxy1(i1,i2-1,i3,1,1)+
     & 8.*rsxy1(i1,i2+1,i3,1,1)-rsxy1(i1,i2+2,i3,1,1))/(12.*dr1(1))
             aj1rxx = aj1rx*aj1rxr+aj1sx*aj1rxs
             aj1rxy = aj1ry*aj1rxr+aj1sy*aj1rxs
             aj1sxx = aj1rx*aj1sxr+aj1sx*aj1sxs
             aj1sxy = aj1ry*aj1sxr+aj1sy*aj1sxs
             aj1ryx = aj1rx*aj1ryr+aj1sx*aj1rys
             aj1ryy = aj1ry*aj1ryr+aj1sy*aj1rys
             aj1syx = aj1rx*aj1syr+aj1sx*aj1sys
             aj1syy = aj1ry*aj1syr+aj1sy*aj1sys
              ww1 = u1(i1,i2,i3,hz)
              ww1r = (u1(i1-2,i2,i3,hz)-8.*u1(i1-1,i2,i3,hz)+8.*u1(i1+
     & 1,i2,i3,hz)-u1(i1+2,i2,i3,hz))/(12.*dr1(0))
              ww1s = (u1(i1,i2-2,i3,hz)-8.*u1(i1,i2-1,i3,hz)+8.*u1(i1,
     & i2+1,i3,hz)-u1(i1,i2+2,i3,hz))/(12.*dr1(1))
              ww1rr = (-u1(i1-2,i2,i3,hz)+16.*u1(i1-1,i2,i3,hz)-30.*u1(
     & i1,i2,i3,hz)+16.*u1(i1+1,i2,i3,hz)-u1(i1+2,i2,i3,hz))/(12.*dr1(
     & 0)**2)
              ww1rs = ((u1(i1-2,i2-2,i3,hz)-8.*u1(i1-2,i2-1,i3,hz)+8.*
     & u1(i1-2,i2+1,i3,hz)-u1(i1-2,i2+2,i3,hz))/(12.*dr1(1))-8.*(u1(
     & i1-1,i2-2,i3,hz)-8.*u1(i1-1,i2-1,i3,hz)+8.*u1(i1-1,i2+1,i3,hz)-
     & u1(i1-1,i2+2,i3,hz))/(12.*dr1(1))+8.*(u1(i1+1,i2-2,i3,hz)-8.*
     & u1(i1+1,i2-1,i3,hz)+8.*u1(i1+1,i2+1,i3,hz)-u1(i1+1,i2+2,i3,hz))
     & /(12.*dr1(1))-(u1(i1+2,i2-2,i3,hz)-8.*u1(i1+2,i2-1,i3,hz)+8.*
     & u1(i1+2,i2+1,i3,hz)-u1(i1+2,i2+2,i3,hz))/(12.*dr1(1)))/(12.*
     & dr1(0))
              ww1ss = (-u1(i1,i2-2,i3,hz)+16.*u1(i1,i2-1,i3,hz)-30.*u1(
     & i1,i2,i3,hz)+16.*u1(i1,i2+1,i3,hz)-u1(i1,i2+2,i3,hz))/(12.*dr1(
     & 1)**2)
               w1x = aj1rx*ww1r+aj1sx*ww1s
               w1y = aj1ry*ww1r+aj1sy*ww1s
               t1 = aj1rx**2
               t6 = aj1sx**2
               w1xx = t1*ww1rr+2*aj1rx*aj1sx*ww1rs+t6*ww1ss+aj1rxx*
     & ww1r+aj1sxx*ww1s
               t1 = aj1ry**2
               t6 = aj1sy**2
               w1yy = t1*ww1rr+2*aj1ry*aj1sy*ww1rs+t6*ww1ss+aj1ryy*
     & ww1r+aj1syy*ww1s
             w1Lap = w1xx+ w1yy
             ! this next call will define the jacobian and its derivatives (parameteric and spatial)
             aj2rx = rsxy2(j1,j2,j3,0,0)
             aj2rxr = (rsxy2(j1-2,j2,j3,0,0)-8.*rsxy2(j1-1,j2,j3,0,0)+
     & 8.*rsxy2(j1+1,j2,j3,0,0)-rsxy2(j1+2,j2,j3,0,0))/(12.*dr2(0))
             aj2rxs = (rsxy2(j1,j2-2,j3,0,0)-8.*rsxy2(j1,j2-1,j3,0,0)+
     & 8.*rsxy2(j1,j2+1,j3,0,0)-rsxy2(j1,j2+2,j3,0,0))/(12.*dr2(1))
             aj2sx = rsxy2(j1,j2,j3,1,0)
             aj2sxr = (rsxy2(j1-2,j2,j3,1,0)-8.*rsxy2(j1-1,j2,j3,1,0)+
     & 8.*rsxy2(j1+1,j2,j3,1,0)-rsxy2(j1+2,j2,j3,1,0))/(12.*dr2(0))
             aj2sxs = (rsxy2(j1,j2-2,j3,1,0)-8.*rsxy2(j1,j2-1,j3,1,0)+
     & 8.*rsxy2(j1,j2+1,j3,1,0)-rsxy2(j1,j2+2,j3,1,0))/(12.*dr2(1))
             aj2ry = rsxy2(j1,j2,j3,0,1)
             aj2ryr = (rsxy2(j1-2,j2,j3,0,1)-8.*rsxy2(j1-1,j2,j3,0,1)+
     & 8.*rsxy2(j1+1,j2,j3,0,1)-rsxy2(j1+2,j2,j3,0,1))/(12.*dr2(0))
             aj2rys = (rsxy2(j1,j2-2,j3,0,1)-8.*rsxy2(j1,j2-1,j3,0,1)+
     & 8.*rsxy2(j1,j2+1,j3,0,1)-rsxy2(j1,j2+2,j3,0,1))/(12.*dr2(1))
             aj2sy = rsxy2(j1,j2,j3,1,1)
             aj2syr = (rsxy2(j1-2,j2,j3,1,1)-8.*rsxy2(j1-1,j2,j3,1,1)+
     & 8.*rsxy2(j1+1,j2,j3,1,1)-rsxy2(j1+2,j2,j3,1,1))/(12.*dr2(0))
             aj2sys = (rsxy2(j1,j2-2,j3,1,1)-8.*rsxy2(j1,j2-1,j3,1,1)+
     & 8.*rsxy2(j1,j2+1,j3,1,1)-rsxy2(j1,j2+2,j3,1,1))/(12.*dr2(1))
             aj2rxx = aj2rx*aj2rxr+aj2sx*aj2rxs
             aj2rxy = aj2ry*aj2rxr+aj2sy*aj2rxs
             aj2sxx = aj2rx*aj2sxr+aj2sx*aj2sxs
             aj2sxy = aj2ry*aj2sxr+aj2sy*aj2sxs
             aj2ryx = aj2rx*aj2ryr+aj2sx*aj2rys
             aj2ryy = aj2ry*aj2ryr+aj2sy*aj2rys
             aj2syx = aj2rx*aj2syr+aj2sx*aj2sys
             aj2syy = aj2ry*aj2syr+aj2sy*aj2sys
              ww2 = u2(j1,j2,j3,hz)
              ww2r = (u2(j1-2,j2,j3,hz)-8.*u2(j1-1,j2,j3,hz)+8.*u2(j1+
     & 1,j2,j3,hz)-u2(j1+2,j2,j3,hz))/(12.*dr2(0))
              ww2s = (u2(j1,j2-2,j3,hz)-8.*u2(j1,j2-1,j3,hz)+8.*u2(j1,
     & j2+1,j3,hz)-u2(j1,j2+2,j3,hz))/(12.*dr2(1))
              ww2rr = (-u2(j1-2,j2,j3,hz)+16.*u2(j1-1,j2,j3,hz)-30.*u2(
     & j1,j2,j3,hz)+16.*u2(j1+1,j2,j3,hz)-u2(j1+2,j2,j3,hz))/(12.*dr2(
     & 0)**2)
              ww2rs = ((u2(j1-2,j2-2,j3,hz)-8.*u2(j1-2,j2-1,j3,hz)+8.*
     & u2(j1-2,j2+1,j3,hz)-u2(j1-2,j2+2,j3,hz))/(12.*dr2(1))-8.*(u2(
     & j1-1,j2-2,j3,hz)-8.*u2(j1-1,j2-1,j3,hz)+8.*u2(j1-1,j2+1,j3,hz)-
     & u2(j1-1,j2+2,j3,hz))/(12.*dr2(1))+8.*(u2(j1+1,j2-2,j3,hz)-8.*
     & u2(j1+1,j2-1,j3,hz)+8.*u2(j1+1,j2+1,j3,hz)-u2(j1+1,j2+2,j3,hz))
     & /(12.*dr2(1))-(u2(j1+2,j2-2,j3,hz)-8.*u2(j1+2,j2-1,j3,hz)+8.*
     & u2(j1+2,j2+1,j3,hz)-u2(j1+2,j2+2,j3,hz))/(12.*dr2(1)))/(12.*
     & dr2(0))
              ww2ss = (-u2(j1,j2-2,j3,hz)+16.*u2(j1,j2-1,j3,hz)-30.*u2(
     & j1,j2,j3,hz)+16.*u2(j1,j2+1,j3,hz)-u2(j1,j2+2,j3,hz))/(12.*dr2(
     & 1)**2)
               w2x = aj2rx*ww2r+aj2sx*ww2s
               w2y = aj2ry*ww2r+aj2sy*ww2s
               t1 = aj2rx**2
               t6 = aj2sx**2
               w2xx = t1*ww2rr+2*aj2rx*aj2sx*ww2rs+t6*ww2ss+aj2rxx*
     & ww2r+aj2sxx*ww2s
               t1 = aj2ry**2
               t6 = aj2sy**2
               w2yy = t1*ww2rr+2*aj2ry*aj2sy*ww2rs+t6*ww2ss+aj2ryy*
     & ww2r+aj2syy*ww2s
             w2Lap = w2xx+ w2yy
            ! These derivatives are computed to 2nd-order accuracy
             ! this next call will define the jacobian and its derivatives (parameteric and spatial)
             aj1rx = rsxy1(i1,i2,i3,0,0)
             aj1rxr = (-rsxy1(i1-1,i2,i3,0,0)+rsxy1(i1+1,i2,i3,0,0))/(
     & 2.*dr1(0))
             aj1rxs = (-rsxy1(i1,i2-1,i3,0,0)+rsxy1(i1,i2+1,i3,0,0))/(
     & 2.*dr1(1))
             aj1sx = rsxy1(i1,i2,i3,1,0)
             aj1sxr = (-rsxy1(i1-1,i2,i3,1,0)+rsxy1(i1+1,i2,i3,1,0))/(
     & 2.*dr1(0))
             aj1sxs = (-rsxy1(i1,i2-1,i3,1,0)+rsxy1(i1,i2+1,i3,1,0))/(
     & 2.*dr1(1))
             aj1ry = rsxy1(i1,i2,i3,0,1)
             aj1ryr = (-rsxy1(i1-1,i2,i3,0,1)+rsxy1(i1+1,i2,i3,0,1))/(
     & 2.*dr1(0))
             aj1rys = (-rsxy1(i1,i2-1,i3,0,1)+rsxy1(i1,i2+1,i3,0,1))/(
     & 2.*dr1(1))
             aj1sy = rsxy1(i1,i2,i3,1,1)
             aj1syr = (-rsxy1(i1-1,i2,i3,1,1)+rsxy1(i1+1,i2,i3,1,1))/(
     & 2.*dr1(0))
             aj1sys = (-rsxy1(i1,i2-1,i3,1,1)+rsxy1(i1,i2+1,i3,1,1))/(
     & 2.*dr1(1))
             aj1rxx = aj1rx*aj1rxr+aj1sx*aj1rxs
             aj1rxy = aj1ry*aj1rxr+aj1sy*aj1rxs
             aj1sxx = aj1rx*aj1sxr+aj1sx*aj1sxs
             aj1sxy = aj1ry*aj1sxr+aj1sy*aj1sxs
             aj1ryx = aj1rx*aj1ryr+aj1sx*aj1rys
             aj1ryy = aj1ry*aj1ryr+aj1sy*aj1rys
             aj1syx = aj1rx*aj1syr+aj1sx*aj1sys
             aj1syy = aj1ry*aj1syr+aj1sy*aj1sys
              ww1 = u1(i1,i2,i3,hz)
              ww1r = (-u1(i1-1,i2,i3,hz)+u1(i1+1,i2,i3,hz))/(2.*dr1(0))
              ww1s = (-u1(i1,i2-1,i3,hz)+u1(i1,i2+1,i3,hz))/(2.*dr1(1))
              ww1rr = (u1(i1-1,i2,i3,hz)-2.*u1(i1,i2,i3,hz)+u1(i1+1,i2,
     & i3,hz))/(dr1(0)**2)
              ww1rs = (-(-u1(i1-1,i2-1,i3,hz)+u1(i1-1,i2+1,i3,hz))/(2.*
     & dr1(1))+(-u1(i1+1,i2-1,i3,hz)+u1(i1+1,i2+1,i3,hz))/(2.*dr1(1)))
     & /(2.*dr1(0))
              ww1ss = (u1(i1,i2-1,i3,hz)-2.*u1(i1,i2,i3,hz)+u1(i1,i2+1,
     & i3,hz))/(dr1(1)**2)
              ww1rrr = (-u1(i1-2,i2,i3,hz)+2.*u1(i1-1,i2,i3,hz)-2.*u1(
     & i1+1,i2,i3,hz)+u1(i1+2,i2,i3,hz))/(2.*dr1(0)**3)
              ww1rrs = ((-u1(i1-1,i2-1,i3,hz)+u1(i1-1,i2+1,i3,hz))/(2.*
     & dr1(1))-2.*(-u1(i1,i2-1,i3,hz)+u1(i1,i2+1,i3,hz))/(2.*dr1(1))+(
     & -u1(i1+1,i2-1,i3,hz)+u1(i1+1,i2+1,i3,hz))/(2.*dr1(1)))/(dr1(0)*
     & *2)
              ww1rss = (-(u1(i1-1,i2-1,i3,hz)-2.*u1(i1-1,i2,i3,hz)+u1(
     & i1-1,i2+1,i3,hz))/(dr1(1)**2)+(u1(i1+1,i2-1,i3,hz)-2.*u1(i1+1,
     & i2,i3,hz)+u1(i1+1,i2+1,i3,hz))/(dr1(1)**2))/(2.*dr1(0))
              ww1sss = (-u1(i1,i2-2,i3,hz)+2.*u1(i1,i2-1,i3,hz)-2.*u1(
     & i1,i2+1,i3,hz)+u1(i1,i2+2,i3,hz))/(2.*dr1(1)**3)
              ww1rrrr = (u1(i1-2,i2,i3,hz)-4.*u1(i1-1,i2,i3,hz)+6.*u1(
     & i1,i2,i3,hz)-4.*u1(i1+1,i2,i3,hz)+u1(i1+2,i2,i3,hz))/(dr1(0)**
     & 4)
              ww1rrrs = (-(-u1(i1-2,i2-1,i3,hz)+u1(i1-2,i2+1,i3,hz))/(
     & 2.*dr1(1))+2.*(-u1(i1-1,i2-1,i3,hz)+u1(i1-1,i2+1,i3,hz))/(2.*
     & dr1(1))-2.*(-u1(i1+1,i2-1,i3,hz)+u1(i1+1,i2+1,i3,hz))/(2.*dr1(
     & 1))+(-u1(i1+2,i2-1,i3,hz)+u1(i1+2,i2+1,i3,hz))/(2.*dr1(1)))/(
     & 2.*dr1(0)**3)
              ww1rrss = ((u1(i1-1,i2-1,i3,hz)-2.*u1(i1-1,i2,i3,hz)+u1(
     & i1-1,i2+1,i3,hz))/(dr1(1)**2)-2.*(u1(i1,i2-1,i3,hz)-2.*u1(i1,
     & i2,i3,hz)+u1(i1,i2+1,i3,hz))/(dr1(1)**2)+(u1(i1+1,i2-1,i3,hz)-
     & 2.*u1(i1+1,i2,i3,hz)+u1(i1+1,i2+1,i3,hz))/(dr1(1)**2))/(dr1(0)*
     & *2)
              ww1rsss = (-(-u1(i1-1,i2-2,i3,hz)+2.*u1(i1-1,i2-1,i3,hz)-
     & 2.*u1(i1-1,i2+1,i3,hz)+u1(i1-1,i2+2,i3,hz))/(2.*dr1(1)**3)+(-
     & u1(i1+1,i2-2,i3,hz)+2.*u1(i1+1,i2-1,i3,hz)-2.*u1(i1+1,i2+1,i3,
     & hz)+u1(i1+1,i2+2,i3,hz))/(2.*dr1(1)**3))/(2.*dr1(0))
              ww1ssss = (u1(i1,i2-2,i3,hz)-4.*u1(i1,i2-1,i3,hz)+6.*u1(
     & i1,i2,i3,hz)-4.*u1(i1,i2+1,i3,hz)+u1(i1,i2+2,i3,hz))/(dr1(1)**
     & 4)
               t1 = aj1rx**2
               t7 = aj1sx**2
               w1xxx = t1*aj1rx*ww1rrr+3*t1*aj1sx*ww1rrs+3*aj1rx*t7*
     & ww1rss+t7*aj1sx*ww1sss+3*aj1rx*aj1rxx*ww1rr+(3*aj1sxx*aj1rx+3*
     & aj1sx*aj1rxx)*ww1rs+3*aj1sxx*aj1sx*ww1ss+aj1rxxx*ww1r+aj1sxxx*
     & ww1s
               t1 = aj1rx**2
               t10 = aj1sx**2
               w1xxy = aj1ry*t1*ww1rrr+(aj1sy*t1+2*aj1ry*aj1sx*aj1rx)*
     & ww1rrs+(aj1ry*t10+2*aj1sy*aj1sx*aj1rx)*ww1rss+aj1sy*t10*ww1sss+
     & (2*aj1rxy*aj1rx+aj1ry*aj1rxx)*ww1rr+(aj1ry*aj1sxx+2*aj1sx*
     & aj1rxy+2*aj1sxy*aj1rx+aj1sy*aj1rxx)*ww1rs+(aj1sy*aj1sxx+2*
     & aj1sxy*aj1sx)*ww1ss+aj1rxxy*ww1r+aj1sxxy*ww1s
               t1 = aj1ry**2
               t4 = aj1sy*aj1ry
               t8 = aj1sy*aj1rx+aj1ry*aj1sx
               t16 = aj1sy**2
               w1xyy = t1*aj1rx*ww1rrr+(t4*aj1rx+aj1ry*t8)*ww1rrs+(t4*
     & aj1sx+aj1sy*t8)*ww1rss+t16*aj1sx*ww1sss+(aj1ryy*aj1rx+2*aj1ry*
     & aj1rxy)*ww1rr+(2*aj1ry*aj1sxy+2*aj1sy*aj1rxy+aj1ryy*aj1sx+
     & aj1syy*aj1rx)*ww1rs+(aj1syy*aj1sx+2*aj1sy*aj1sxy)*ww1ss+
     & aj1rxyy*ww1r+aj1sxyy*ww1s
               t1 = aj1ry**2
               t7 = aj1sy**2
               w1yyy = aj1ry*t1*ww1rrr+3*t1*aj1sy*ww1rrs+3*aj1ry*t7*
     & ww1rss+t7*aj1sy*ww1sss+3*aj1ry*aj1ryy*ww1rr+(3*aj1syy*aj1ry+3*
     & aj1sy*aj1ryy)*ww1rs+3*aj1syy*aj1sy*ww1ss+aj1ryyy*ww1r+aj1syyy*
     & ww1s
               t1 = aj1rx**2
               t2 = t1**2
               t8 = aj1sx**2
               t16 = t8**2
               t25 = aj1sxx*aj1rx
               t27 = t25+aj1sx*aj1rxx
               t28 = 3*t27
               t30 = 2*t27
               t46 = aj1rxx**2
               t60 = aj1sxx**2
               w1xxxx = t2*ww1rrrr+4*t1*aj1rx*aj1sx*ww1rrrs+6*t1*t8*
     & ww1rrss+4*aj1rx*t8*aj1sx*ww1rsss+t16*ww1ssss+6*t1*aj1rxx*
     & ww1rrr+(7*aj1sx*aj1rx*aj1rxx+aj1sxx*t1+aj1rx*t28+aj1rx*t30)*
     & ww1rrs+(aj1sx*t28+7*t25*aj1sx+aj1rxx*t8+aj1sx*t30)*ww1rss+6*t8*
     & aj1sxx*ww1sss+(4*aj1rx*aj1rxxx+3*t46)*ww1rr+(4*aj1sxxx*aj1rx+4*
     & aj1sx*aj1rxxx+6*aj1sxx*aj1rxx)*ww1rs+(4*aj1sxxx*aj1sx+3*t60)*
     & ww1ss+aj1rxxxx*ww1r+aj1sxxxx*ww1s
               t1 = aj1ry**2
               t2 = aj1rx**2
               t5 = aj1sy*aj1ry
               t11 = aj1sy*t2+2*aj1ry*aj1sx*aj1rx
               t16 = aj1sx**2
               t21 = aj1ry*t16+2*aj1sy*aj1sx*aj1rx
               t29 = aj1sy**2
               t38 = 2*aj1rxy*aj1rx+aj1ry*aj1rxx
               t52 = aj1sx*aj1rxy
               t54 = aj1sxy*aj1rx
               t57 = aj1ry*aj1sxx+2*t52+2*t54+aj1sy*aj1rxx
               t60 = 2*t52+2*t54
               t68 = aj1sy*aj1sxx+2*aj1sxy*aj1sx
               t92 = aj1rxy**2
               t110 = aj1sxy**2
               w1xxyy = t1*t2*ww1rrrr+(t5*t2+aj1ry*t11)*ww1rrrs+(aj1sy*
     & t11+aj1ry*t21)*ww1rrss+(aj1sy*t21+t5*t16)*ww1rsss+t29*t16*
     & ww1ssss+(2*aj1ry*aj1rxy*aj1rx+aj1ry*t38+aj1ryy*t2)*ww1rrr+(
     & aj1sy*t38+2*aj1sy*aj1rxy*aj1rx+2*aj1ryy*aj1sx*aj1rx+aj1syy*t2+
     & aj1ry*t57+aj1ry*t60)*ww1rrs+(aj1sy*t57+aj1ry*t68+aj1ryy*t16+2*
     & aj1ry*aj1sxy*aj1sx+2*aj1syy*aj1sx*aj1rx+aj1sy*t60)*ww1rss+(2*
     & aj1sy*aj1sxy*aj1sx+aj1sy*t68+aj1syy*t16)*ww1sss+(2*aj1rx*
     & aj1rxyy+aj1ryy*aj1rxx+2*aj1ry*aj1rxxy+2*t92)*ww1rr+(4*aj1sxy*
     & aj1rxy+2*aj1ry*aj1sxxy+aj1ryy*aj1sxx+2*aj1sy*aj1rxxy+2*aj1sxyy*
     & aj1rx+aj1syy*aj1rxx+2*aj1sx*aj1rxyy)*ww1rs+(2*t110+2*aj1sy*
     & aj1sxxy+aj1syy*aj1sxx+2*aj1sx*aj1sxyy)*ww1ss+aj1rxxyy*ww1r+
     & aj1sxxyy*ww1s
               t1 = aj1ry**2
               t2 = t1**2
               t8 = aj1sy**2
               t16 = t8**2
               t25 = aj1syy*aj1ry
               t27 = t25+aj1sy*aj1ryy
               t28 = 3*t27
               t30 = 2*t27
               t46 = aj1ryy**2
               t60 = aj1syy**2
               w1yyyy = t2*ww1rrrr+4*t1*aj1ry*aj1sy*ww1rrrs+6*t1*t8*
     & ww1rrss+4*aj1ry*t8*aj1sy*ww1rsss+t16*ww1ssss+6*t1*aj1ryy*
     & ww1rrr+(7*aj1sy*aj1ry*aj1ryy+aj1syy*t1+aj1ry*t28+aj1ry*t30)*
     & ww1rrs+(aj1sy*t28+7*t25*aj1sy+aj1ryy*t8+aj1sy*t30)*ww1rss+6*t8*
     & aj1syy*ww1sss+(4*aj1ry*aj1ryyy+3*t46)*ww1rr+(4*aj1syyy*aj1ry+4*
     & aj1sy*aj1ryyy+6*aj1syy*aj1ryy)*ww1rs+(4*aj1syyy*aj1sy+3*t60)*
     & ww1ss+aj1ryyyy*ww1r+aj1syyyy*ww1s
             w1LapSq = w1xxxx +2.* w1xxyy + w1yyyy
             ! this next call will define the jacobian and its derivatives (parameteric and spatial)
             aj2rx = rsxy2(j1,j2,j3,0,0)
             aj2rxr = (-rsxy2(j1-1,j2,j3,0,0)+rsxy2(j1+1,j2,j3,0,0))/(
     & 2.*dr2(0))
             aj2rxs = (-rsxy2(j1,j2-1,j3,0,0)+rsxy2(j1,j2+1,j3,0,0))/(
     & 2.*dr2(1))
             aj2sx = rsxy2(j1,j2,j3,1,0)
             aj2sxr = (-rsxy2(j1-1,j2,j3,1,0)+rsxy2(j1+1,j2,j3,1,0))/(
     & 2.*dr2(0))
             aj2sxs = (-rsxy2(j1,j2-1,j3,1,0)+rsxy2(j1,j2+1,j3,1,0))/(
     & 2.*dr2(1))
             aj2ry = rsxy2(j1,j2,j3,0,1)
             aj2ryr = (-rsxy2(j1-1,j2,j3,0,1)+rsxy2(j1+1,j2,j3,0,1))/(
     & 2.*dr2(0))
             aj2rys = (-rsxy2(j1,j2-1,j3,0,1)+rsxy2(j1,j2+1,j3,0,1))/(
     & 2.*dr2(1))
             aj2sy = rsxy2(j1,j2,j3,1,1)
             aj2syr = (-rsxy2(j1-1,j2,j3,1,1)+rsxy2(j1+1,j2,j3,1,1))/(
     & 2.*dr2(0))
             aj2sys = (-rsxy2(j1,j2-1,j3,1,1)+rsxy2(j1,j2+1,j3,1,1))/(
     & 2.*dr2(1))
             aj2rxx = aj2rx*aj2rxr+aj2sx*aj2rxs
             aj2rxy = aj2ry*aj2rxr+aj2sy*aj2rxs
             aj2sxx = aj2rx*aj2sxr+aj2sx*aj2sxs
             aj2sxy = aj2ry*aj2sxr+aj2sy*aj2sxs
             aj2ryx = aj2rx*aj2ryr+aj2sx*aj2rys
             aj2ryy = aj2ry*aj2ryr+aj2sy*aj2rys
             aj2syx = aj2rx*aj2syr+aj2sx*aj2sys
             aj2syy = aj2ry*aj2syr+aj2sy*aj2sys
              ww2 = u2(j1,j2,j3,hz)
              ww2r = (-u2(j1-1,j2,j3,hz)+u2(j1+1,j2,j3,hz))/(2.*dr2(0))
              ww2s = (-u2(j1,j2-1,j3,hz)+u2(j1,j2+1,j3,hz))/(2.*dr2(1))
              ww2rr = (u2(j1-1,j2,j3,hz)-2.*u2(j1,j2,j3,hz)+u2(j1+1,j2,
     & j3,hz))/(dr2(0)**2)
              ww2rs = (-(-u2(j1-1,j2-1,j3,hz)+u2(j1-1,j2+1,j3,hz))/(2.*
     & dr2(1))+(-u2(j1+1,j2-1,j3,hz)+u2(j1+1,j2+1,j3,hz))/(2.*dr2(1)))
     & /(2.*dr2(0))
              ww2ss = (u2(j1,j2-1,j3,hz)-2.*u2(j1,j2,j3,hz)+u2(j1,j2+1,
     & j3,hz))/(dr2(1)**2)
              ww2rrr = (-u2(j1-2,j2,j3,hz)+2.*u2(j1-1,j2,j3,hz)-2.*u2(
     & j1+1,j2,j3,hz)+u2(j1+2,j2,j3,hz))/(2.*dr2(0)**3)
              ww2rrs = ((-u2(j1-1,j2-1,j3,hz)+u2(j1-1,j2+1,j3,hz))/(2.*
     & dr2(1))-2.*(-u2(j1,j2-1,j3,hz)+u2(j1,j2+1,j3,hz))/(2.*dr2(1))+(
     & -u2(j1+1,j2-1,j3,hz)+u2(j1+1,j2+1,j3,hz))/(2.*dr2(1)))/(dr2(0)*
     & *2)
              ww2rss = (-(u2(j1-1,j2-1,j3,hz)-2.*u2(j1-1,j2,j3,hz)+u2(
     & j1-1,j2+1,j3,hz))/(dr2(1)**2)+(u2(j1+1,j2-1,j3,hz)-2.*u2(j1+1,
     & j2,j3,hz)+u2(j1+1,j2+1,j3,hz))/(dr2(1)**2))/(2.*dr2(0))
              ww2sss = (-u2(j1,j2-2,j3,hz)+2.*u2(j1,j2-1,j3,hz)-2.*u2(
     & j1,j2+1,j3,hz)+u2(j1,j2+2,j3,hz))/(2.*dr2(1)**3)
              ww2rrrr = (u2(j1-2,j2,j3,hz)-4.*u2(j1-1,j2,j3,hz)+6.*u2(
     & j1,j2,j3,hz)-4.*u2(j1+1,j2,j3,hz)+u2(j1+2,j2,j3,hz))/(dr2(0)**
     & 4)
              ww2rrrs = (-(-u2(j1-2,j2-1,j3,hz)+u2(j1-2,j2+1,j3,hz))/(
     & 2.*dr2(1))+2.*(-u2(j1-1,j2-1,j3,hz)+u2(j1-1,j2+1,j3,hz))/(2.*
     & dr2(1))-2.*(-u2(j1+1,j2-1,j3,hz)+u2(j1+1,j2+1,j3,hz))/(2.*dr2(
     & 1))+(-u2(j1+2,j2-1,j3,hz)+u2(j1+2,j2+1,j3,hz))/(2.*dr2(1)))/(
     & 2.*dr2(0)**3)
              ww2rrss = ((u2(j1-1,j2-1,j3,hz)-2.*u2(j1-1,j2,j3,hz)+u2(
     & j1-1,j2+1,j3,hz))/(dr2(1)**2)-2.*(u2(j1,j2-1,j3,hz)-2.*u2(j1,
     & j2,j3,hz)+u2(j1,j2+1,j3,hz))/(dr2(1)**2)+(u2(j1+1,j2-1,j3,hz)-
     & 2.*u2(j1+1,j2,j3,hz)+u2(j1+1,j2+1,j3,hz))/(dr2(1)**2))/(dr2(0)*
     & *2)
              ww2rsss = (-(-u2(j1-1,j2-2,j3,hz)+2.*u2(j1-1,j2-1,j3,hz)-
     & 2.*u2(j1-1,j2+1,j3,hz)+u2(j1-1,j2+2,j3,hz))/(2.*dr2(1)**3)+(-
     & u2(j1+1,j2-2,j3,hz)+2.*u2(j1+1,j2-1,j3,hz)-2.*u2(j1+1,j2+1,j3,
     & hz)+u2(j1+1,j2+2,j3,hz))/(2.*dr2(1)**3))/(2.*dr2(0))
              ww2ssss = (u2(j1,j2-2,j3,hz)-4.*u2(j1,j2-1,j3,hz)+6.*u2(
     & j1,j2,j3,hz)-4.*u2(j1,j2+1,j3,hz)+u2(j1,j2+2,j3,hz))/(dr2(1)**
     & 4)
               t1 = aj2rx**2
               t7 = aj2sx**2
               w2xxx = t1*aj2rx*ww2rrr+3*t1*aj2sx*ww2rrs+3*aj2rx*t7*
     & ww2rss+t7*aj2sx*ww2sss+3*aj2rx*aj2rxx*ww2rr+(3*aj2sxx*aj2rx+3*
     & aj2sx*aj2rxx)*ww2rs+3*aj2sxx*aj2sx*ww2ss+aj2rxxx*ww2r+aj2sxxx*
     & ww2s
               t1 = aj2rx**2
               t10 = aj2sx**2
               w2xxy = aj2ry*t1*ww2rrr+(aj2sy*t1+2*aj2ry*aj2sx*aj2rx)*
     & ww2rrs+(aj2ry*t10+2*aj2sy*aj2sx*aj2rx)*ww2rss+aj2sy*t10*ww2sss+
     & (2*aj2rxy*aj2rx+aj2ry*aj2rxx)*ww2rr+(aj2ry*aj2sxx+2*aj2sx*
     & aj2rxy+2*aj2sxy*aj2rx+aj2sy*aj2rxx)*ww2rs+(aj2sy*aj2sxx+2*
     & aj2sxy*aj2sx)*ww2ss+aj2rxxy*ww2r+aj2sxxy*ww2s
               t1 = aj2ry**2
               t4 = aj2sy*aj2ry
               t8 = aj2sy*aj2rx+aj2ry*aj2sx
               t16 = aj2sy**2
               w2xyy = t1*aj2rx*ww2rrr+(t4*aj2rx+aj2ry*t8)*ww2rrs+(t4*
     & aj2sx+aj2sy*t8)*ww2rss+t16*aj2sx*ww2sss+(aj2ryy*aj2rx+2*aj2ry*
     & aj2rxy)*ww2rr+(2*aj2ry*aj2sxy+2*aj2sy*aj2rxy+aj2ryy*aj2sx+
     & aj2syy*aj2rx)*ww2rs+(aj2syy*aj2sx+2*aj2sy*aj2sxy)*ww2ss+
     & aj2rxyy*ww2r+aj2sxyy*ww2s
               t1 = aj2ry**2
               t7 = aj2sy**2
               w2yyy = aj2ry*t1*ww2rrr+3*t1*aj2sy*ww2rrs+3*aj2ry*t7*
     & ww2rss+t7*aj2sy*ww2sss+3*aj2ry*aj2ryy*ww2rr+(3*aj2syy*aj2ry+3*
     & aj2sy*aj2ryy)*ww2rs+3*aj2syy*aj2sy*ww2ss+aj2ryyy*ww2r+aj2syyy*
     & ww2s
               t1 = aj2rx**2
               t2 = t1**2
               t8 = aj2sx**2
               t16 = t8**2
               t25 = aj2sxx*aj2rx
               t27 = t25+aj2sx*aj2rxx
               t28 = 3*t27
               t30 = 2*t27
               t46 = aj2rxx**2
               t60 = aj2sxx**2
               w2xxxx = t2*ww2rrrr+4*t1*aj2rx*aj2sx*ww2rrrs+6*t1*t8*
     & ww2rrss+4*aj2rx*t8*aj2sx*ww2rsss+t16*ww2ssss+6*t1*aj2rxx*
     & ww2rrr+(7*aj2sx*aj2rx*aj2rxx+aj2sxx*t1+aj2rx*t28+aj2rx*t30)*
     & ww2rrs+(aj2sx*t28+7*t25*aj2sx+aj2rxx*t8+aj2sx*t30)*ww2rss+6*t8*
     & aj2sxx*ww2sss+(4*aj2rx*aj2rxxx+3*t46)*ww2rr+(4*aj2sxxx*aj2rx+4*
     & aj2sx*aj2rxxx+6*aj2sxx*aj2rxx)*ww2rs+(4*aj2sxxx*aj2sx+3*t60)*
     & ww2ss+aj2rxxxx*ww2r+aj2sxxxx*ww2s
               t1 = aj2ry**2
               t2 = aj2rx**2
               t5 = aj2sy*aj2ry
               t11 = aj2sy*t2+2*aj2ry*aj2sx*aj2rx
               t16 = aj2sx**2
               t21 = aj2ry*t16+2*aj2sy*aj2sx*aj2rx
               t29 = aj2sy**2
               t38 = 2*aj2rxy*aj2rx+aj2ry*aj2rxx
               t52 = aj2sx*aj2rxy
               t54 = aj2sxy*aj2rx
               t57 = aj2ry*aj2sxx+2*t52+2*t54+aj2sy*aj2rxx
               t60 = 2*t52+2*t54
               t68 = aj2sy*aj2sxx+2*aj2sxy*aj2sx
               t92 = aj2rxy**2
               t110 = aj2sxy**2
               w2xxyy = t1*t2*ww2rrrr+(t5*t2+aj2ry*t11)*ww2rrrs+(aj2sy*
     & t11+aj2ry*t21)*ww2rrss+(aj2sy*t21+t5*t16)*ww2rsss+t29*t16*
     & ww2ssss+(2*aj2ry*aj2rxy*aj2rx+aj2ry*t38+aj2ryy*t2)*ww2rrr+(
     & aj2sy*t38+2*aj2sy*aj2rxy*aj2rx+2*aj2ryy*aj2sx*aj2rx+aj2syy*t2+
     & aj2ry*t57+aj2ry*t60)*ww2rrs+(aj2sy*t57+aj2ry*t68+aj2ryy*t16+2*
     & aj2ry*aj2sxy*aj2sx+2*aj2syy*aj2sx*aj2rx+aj2sy*t60)*ww2rss+(2*
     & aj2sy*aj2sxy*aj2sx+aj2sy*t68+aj2syy*t16)*ww2sss+(2*aj2rx*
     & aj2rxyy+aj2ryy*aj2rxx+2*aj2ry*aj2rxxy+2*t92)*ww2rr+(4*aj2sxy*
     & aj2rxy+2*aj2ry*aj2sxxy+aj2ryy*aj2sxx+2*aj2sy*aj2rxxy+2*aj2sxyy*
     & aj2rx+aj2syy*aj2rxx+2*aj2sx*aj2rxyy)*ww2rs+(2*t110+2*aj2sy*
     & aj2sxxy+aj2syy*aj2sxx+2*aj2sx*aj2sxyy)*ww2ss+aj2rxxyy*ww2r+
     & aj2sxxyy*ww2s
               t1 = aj2ry**2
               t2 = t1**2
               t8 = aj2sy**2
               t16 = t8**2
               t25 = aj2syy*aj2ry
               t27 = t25+aj2sy*aj2ryy
               t28 = 3*t27
               t30 = 2*t27
               t46 = aj2ryy**2
               t60 = aj2syy**2
               w2yyyy = t2*ww2rrrr+4*t1*aj2ry*aj2sy*ww2rrrs+6*t1*t8*
     & ww2rrss+4*aj2ry*t8*aj2sy*ww2rsss+t16*ww2ssss+6*t1*aj2ryy*
     & ww2rrr+(7*aj2sy*aj2ry*aj2ryy+aj2syy*t1+aj2ry*t28+aj2ry*t30)*
     & ww2rrs+(aj2sy*t28+7*t25*aj2sy+aj2ryy*t8+aj2sy*t30)*ww2rss+6*t8*
     & aj2syy*ww2sss+(4*aj2ry*aj2ryyy+3*t46)*ww2rr+(4*aj2syyy*aj2ry+4*
     & aj2sy*aj2ryyy+6*aj2syy*aj2ryy)*ww2rs+(4*aj2syyy*aj2sy+3*t60)*
     & ww2ss+aj2ryyyy*ww2r+aj2syyyy*ww2s
             w2LapSq = w2xxxx +2.* w2xxyy + w2yyyy
            f(0)=(an1*w1x+an2*w1y)/eps1 - (an1*w2x+an2*w2y)/eps2
            f(1)=w1Lap/eps1 - w2Lap/eps2
            f(2)=(an1*(w1xxx+w1xyy)+an2*(w1xxy+w1yyy))/eps1**2 - (an1*(
     & w2xxx+w2xyy)+an2*(w2xxy+w2yyy))/eps2**2
            f(3)=w1LapSq/eps1**2 - w2LapSq/eps2**2
            if( twilightZone.eq.1 )then
              call ogderiv(ep, 0,1,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, hz, wex  )
              call ogderiv(ep, 0,0,1,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, hz, wey  )
              call ogderiv(ep, 0,2,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, hz, wexx )
              call ogderiv(ep, 0,0,2,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, hz, weyy )
              call ogderiv(ep, 0,3,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, hz, wexxx )
              call ogderiv(ep, 0,2,1,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, hz, wexxy )
              call ogderiv(ep, 0,1,2,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, hz, wexyy )
              call ogderiv(ep, 0,0,3,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, hz, weyyy )
              call ogderiv(ep, 0,4,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, hz, wexxxx )
              call ogderiv(ep, 0,2,2,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, hz, wexxyy )
              call ogderiv(ep, 0,0,4,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,0.,t, hz, weyyyy )
              weLap = wexx + weyy
              weLapSq = wexxxx + 2.*wexxyy + weyyyy
              f(0) = f(0) - (an1*wex+an2*wey)*(1./eps1 - 1./eps2)
              f(1) = f(1) - ( weLap )*(1./eps1 - 1./eps2)
              f(2) = f(2) - (an1*(wexxx+wexyy)+an2*(wexxy+weyyy))*(
     & 1./eps1**2 - 1./eps2**2)
              f(3) = f(3) - weLapSq*(1./eps1**2 - 1./eps2**2)
            end if

           if( debug.gt.3 ) write(debugFile,'(" --> 4cth: i1,i2=",2i4,
     & " hz-f(re-eval)=",8e10.2)') i1,i2,f(0),f(1),f(2),f(3)
             ! '
          end if



           ! ***********************

           ! u1(i1-is1,i2-is2,i3,hz)=u2(j1+js1,j2+js2,j3,hz) 
           ! u2(j1-js1,j2-js2,j3,hz)=u1(i1+is1,i2+is2,i3,hz)
           ! u1(i1-2*is1,i2-2*is2,i3,hz)=u2(j1+2*js1,j2+2*js2,j3,hz) 
           ! u2(j1-2*js1,j2-2*js2,j3,hz)=u1(i1+2*is1,i2+2*is2,i3,hz)

            end if
            j1=j1+1
           end do
           j2=j2+1
          end do
         ! =============== end loops =======================

         if( parallel.eq.0 )then
          axisp1=mod(axis1+1,nd)
          if( boundaryCondition1(0,axisp1).lt.0 )then
           ! direction axisp1 is periodic
           diff(axis1)=0
           diff(axisp1)=gridIndexRange1(1,axisp1)-gridIndexRange1(0,
     & axisp1)
           if( side1.eq.0 )then
             ! assign 4 ghost points outside lower corner
             np1a=gridIndexRange1(0,0)-2
             np1b=gridIndexRange1(0,0)-1
             np2a=gridIndexRange1(0,1)-2
             np2b=gridIndexRange1(0,1)-1
             do i3=n3a,n3b
             do i2=np2a,np2b
             do i1=np1a,np1b
             do n=ex,hz
               ! write(*,'(" periodic i1,i2,i3,n=",4i4)') i1,i2,i3,n
              u1(i1,i2,i3,n) = u1(i1+diff(0),i2+diff(1),i3,n)
             end do
             end do
             end do
             end do
             ! assign 4 ghost points outside upper corner
             if( axis1.eq.0 )then
               np2a=gridIndexRange1(1,axisp1)+1
               np2b=gridIndexRange1(1,axisp1)+2
             else
               np1a=gridIndexRange1(1,axisp1)+1
               np1b=gridIndexRange1(1,axisp1)+2
             end if
             do i3=n3a,n3b
             do i2=np2a,np2b
             do i1=np1a,np1b
             do n=ex,hz
               ! write(*,'(" periodic i1,i2,i3,n=",4i4)') i1,i2,i3,n
              u1(i1,i2,i3,n) = u1(i1-diff(0),i2-diff(1),i3,n)
             end do
             end do
             end do
             end do
           else
             ! assign 4 ghost points outside upper corner
             np1a=gridIndexRange1(1,0)+1
             np1b=gridIndexRange1(1,0)+2
             np2a=gridIndexRange1(1,1)+1
             np2b=gridIndexRange1(1,1)+2
             do i3=n3a,n3b
             do i2=np2a,np2b
             do i1=np1a,np1b
             do n=ex,hz
               ! write(*,'(" periodic i1,i2,i3,n=",4i4)') i1,i2,i3,n
              u1(i1,i2,i3,n) = u1(i1-diff(0),i2-diff(1),i3,n)
             end do
             end do
             end do
             end do
             if( axis1.eq.0 )then
               np2a=gridIndexRange1(0,axisp1)-2
               np2b=gridIndexRange1(0,axisp1)-1
             else
               np1a=gridIndexRange1(0,axisp1)-2
               np1b=gridIndexRange1(0,axisp1)-1
             end if
             do i3=n3a,n3b
             do i2=np2a,np2b
             do i1=np1a,np1b
             do n=ex,hz
               ! write(*,'(" periodic i1,i2,i3,n=",4i4)') i1,i2,i3,n
              u1(i1,i2,i3,n) = u1(i1+diff(0),i2+diff(1),i3,n)
             end do
             end do
             end do
             end do
           end if
          endif
         end if
         if( parallel.eq.0 )then
          axisp1=mod(axis2+1,nd)
          if( boundaryCondition2(0,axisp1).lt.0 )then
           ! direction axisp1 is periodic
           diff(axis2)=0
           diff(axisp1)=gridIndexRange2(1,axisp1)-gridIndexRange2(0,
     & axisp1)
           if( side2.eq.0 )then
             ! assign 4 ghost points outside lower corner
             np1a=gridIndexRange2(0,0)-2
             np1b=gridIndexRange2(0,0)-1
             np2a=gridIndexRange2(0,1)-2
             np2b=gridIndexRange2(0,1)-1
             do i3=n3a,n3b
             do i2=np2a,np2b
             do i1=np1a,np1b
             do n=ex,hz
               ! write(*,'(" periodic i1,i2,i3,n=",4i4)') i1,i2,i3,n
              u2(i1,i2,i3,n) = u2(i1+diff(0),i2+diff(1),i3,n)
             end do
             end do
             end do
             end do
             ! assign 4 ghost points outside upper corner
             if( axis2.eq.0 )then
               np2a=gridIndexRange2(1,axisp1)+1
               np2b=gridIndexRange2(1,axisp1)+2
             else
               np1a=gridIndexRange2(1,axisp1)+1
               np1b=gridIndexRange2(1,axisp1)+2
             end if
             do i3=n3a,n3b
             do i2=np2a,np2b
             do i1=np1a,np1b
             do n=ex,hz
               ! write(*,'(" periodic i1,i2,i3,n=",4i4)') i1,i2,i3,n
              u2(i1,i2,i3,n) = u2(i1-diff(0),i2-diff(1),i3,n)
             end do
             end do
             end do
             end do
           else
             ! assign 4 ghost points outside upper corner
             np1a=gridIndexRange2(1,0)+1
             np1b=gridIndexRange2(1,0)+2
             np2a=gridIndexRange2(1,1)+1
             np2b=gridIndexRange2(1,1)+2
             do i3=n3a,n3b
             do i2=np2a,np2b
             do i1=np1a,np1b
             do n=ex,hz
               ! write(*,'(" periodic i1,i2,i3,n=",4i4)') i1,i2,i3,n
              u2(i1,i2,i3,n) = u2(i1-diff(0),i2-diff(1),i3,n)
             end do
             end do
             end do
             end do
             if( axis2.eq.0 )then
               np2a=gridIndexRange2(0,axisp1)-2
               np2b=gridIndexRange2(0,axisp1)-1
             else
               np1a=gridIndexRange2(0,axisp1)-2
               np1b=gridIndexRange2(0,axisp1)-1
             end if
             do i3=n3a,n3b
             do i2=np2a,np2b
             do i1=np1a,np1b
             do n=ex,hz
               ! write(*,'(" periodic i1,i2,i3,n=",4i4)') i1,i2,i3,n
              u2(i1,i2,i3,n) = u2(i1+diff(0),i2+diff(1),i3,n)
             end do
             end do
             end do
             end do
           end if
          endif
         end if

           if( debug.gt.0 )then
             write(debugFile,'(" ***it=",i2," max-diff = ",e11.2)') it,
     & err
           end if
           if( debug.gt.3 )then
             write(*,'(" ***it=",i2," max-diff = ",e11.2)') it,err
           end if
         end do ! ************** end iteration **************


         ! now make sure that div(u)=0 etc.
         if( .false. )then
!*         beginLoops2d() ! =============== start loops =======================

           ! 0  [ u.x + v.y ] = 0
c           a8(0,0) = -is*8.*rsxy1(i1,i2,i3,axis1,0)*dr114(axis1)     ! coeff of u1(-1) from [u.x+v.y] 
c           a8(0,1) = -is*8.*rsxy1(i1,i2,i3,axis1,1)*dr114(axis1)     ! coeff of v1(-1) from [u.x+v.y] 
c           a8(0,4) =  is*   rsxy1(i1,i2,i3,axis1,0)*dr114(axis1)     ! u1(-2)
c           a8(0,5) =  is*   rsxy1(i1,i2,i3,axis1,1)*dr114(axis1)     ! v1(-2) 

c           a8(0,2) =  js*8.*rsxy2(j1,j2,j3,axis2,0)*dr214(axis2)     ! coeff of u2(-1) from [u.x+v.y] 
c           a8(0,3) =  js*8.*rsxy2(j1,j2,j3,axis2,1)*dr214(axis2)  
c           a8(0,6) = -js*   rsxy2(j1,j2,j3,axis2,0)*dr214(axis2) 
c           a8(0,7) = -js*   rsxy2(j1,j2,j3,axis2,1)*dr214(axis2) 

           ! first evaluate the equations we want to solve with the wrong values at the ghost points:
!*           divu=u1x42(i1,i2,i3,ex)+u1y42(i1,i2,i3,ey)
!*           a0=is*   rsxy1(i1,i2,i3,axis1,0)*dr114(axis1)
!*           a1=is*   rsxy1(i1,i2,i3,axis1,1)*dr114(axis1)
!*           aNormSq=a0**2+a1**2
!*           ! now project:  a.uNew = a.uOld - div  ->  (div-a.uOld)+a.uNew = div(uNew) = 0
!*           u1(i1-2*is1,i2-2*is2,i3,ex)=u1(i1-2*is1,i2-2*is2,i3,ex)-divu*a0/aNormSq
!*           u1(i1-2*is1,i2-2*is2,i3,ey)=u1(i1-2*is1,i2-2*is2,i3,ey)-divu*a1/aNormSq
!*
!*           divu=u2x42(j1,j2,j3,ex)+u2y42(j1,j2,j3,ey)
!*           a0=js*   rsxy2(j1,j2,j3,axis2,0)*dr214(axis2) 
!*           a1=js*   rsxy2(j1,j2,j3,axis2,1)*dr214(axis2) 
!*           aNormSq=a0**2+a1**2
!*
!*           u2(j1-2*js1,j2-2*js2,j3,ex)=u2(j1-2*js1,j2-2*js2,j3,ex)-divu*a0/aNormSq
!*           u2(j1-2*js1,j2-2*js2,j3,ey)=u2(j1-2*js1,j2-2*js2,j3,ey)-divu*a1/aNormSq
!*
!*           if( debug.gt.0 )then
!*             divu=u1x42(i1,i2,i3,ex)+u1y42(i1,i2,i3,ey)
!*              write(debugFile,'(" --> 4cth: eval div1,div2=",2e10.2)') u1x42(i1,i2,i3,ex)+u1y42(i1,i2,i3,ey),u2x42(j1,j2,j3,ex)+u2y42(j1,j2,j3,ey)
!*           end if
!*         endLoops2d()
       end if



       else if( nd.eq.3 .and. (orderOfAccuracy.eq.2 .or. 
     & orderOfAccuracy.eq.2 ) .and. gridType.eq.curvilinear )then

         ! *******************************
         ! ***** 3D curvilinear case *****
         ! *******************************

        ! **NOTE** For now we use this 2nd order version for 4th order and just assign the 2nd ghost line below

        if( solveForH .ne.0 )then
          stop 3017
        end if


        if( .false. )then
           j3=mm3a
           do i3=nn3a,nn3b
           j2=mm2a
           do i2=nn2a,nn2b
            j1=mm1a
            do i1=nn1a,nn1b
           write(debugFile,'(" -->START v1(",i2,":",i2,",",i2,",",i2,")
     &  =",3f9.4)') i1-1,i1+1,i2,i3,u1(i1-1,i2,i3,ey),u1(i1,i2,i3,ey),
     & u1(i1+1,i2,i3,ey)
           ! '
             j1=j1+1
            end do
            j2=j2+1
           end do
            j3=j3+1
           end do
        end if

         ! ---- first satisfy the jump conditions on the boundary --------
         !    [ eps n.u ] = 0
         !    [ tau.u ] = 0
           ! *** 3D ***
           if( eps1.lt.eps2 )then
             epsRatio=eps1/eps2
              j3=mm3a
              do i3=nn3a,nn3b
              j2=mm2a
              do i2=nn2a,nn2b
               j1=mm1a
               do i1=nn1a,nn1b
               if( mask1(i1,i2,i3).gt.0 .and. mask2(j1,j2,j3).gt.0 )
     & then
               ! eps2 n.u2 = eps1 n.u1
               !     tau.u2 = tau.u1
                an1=rsxy1(i1,i2,i3,axis1,0)   ! normal (an1,an2)
                an2=rsxy1(i1,i2,i3,axis1,1)
                an3=rsxy1(i1,i2,i3,axis1,2)
                aNorm=max(epsx,sqrt(an1**2+an2**2+an3**2))
                an1=an1/aNorm
                an2=an2/aNorm
                an3=an3/aNorm
               ua=u1(i1,i2,i3,ex)
               ub=u1(i1,i2,i3,ey)
               uc=u1(i1,i2,i3,ez)
               nDotU = an1*ua+an2*ub+an3*uc
               if( twilightZone.eq.1 )then
                ! adjust for TZ forcing (here we assume the exact solution is the same on both sides)
                call ogderiv(ep, 0,0,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),xy1(i1,i2,i3,2),t, ex, ue )
                call ogderiv(ep, 0,0,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),xy1(i1,i2,i3,2),t, ey, ve )
                call ogderiv(ep, 0,0,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),xy1(i1,i2,i3,2),t, ez, we )
                nDotU = nDotU - (an1*ue+an2*ve+an3*we)
               end if
               ! u2 equals u1 but with normal component = eps1/eps2*(n.u1)
               u2(j1,j2,j3,ex) = ua + (nDotU*epsRatio - nDotU)*an1
               u2(j1,j2,j3,ey) = ub + (nDotU*epsRatio - nDotU)*an2
               u2(j1,j2,j3,ez) = uc + (nDotU*epsRatio - nDotU)*an3
         !   write(*,'(" jump(1): (i1,i2,i3)=",3i3," j1,j2,j3=",3i3)') i1,i2,i3,j1,j2,j3
         !   write(*,'(" jump(1): x,y,z=",3e10.2," ua,ue=",2e10.2," ub,ve=",2e10.2," uc,we=",2e10.2)') xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),xy1(i1,i2,i3,2),ua,ue,ub,ve,uc,we
               end if
                j1=j1+1
               end do
               j2=j2+1
              end do
               j3=j3+1
              end do
           else
             epsRatio=eps2/eps1
              j3=mm3a
              do i3=nn3a,nn3b
              j2=mm2a
              do i2=nn2a,nn2b
               j1=mm1a
               do i1=nn1a,nn1b
               if( mask1(i1,i2,i3).gt.0 .and. mask2(j1,j2,j3).gt.0 )
     & then
               ! eps2 n.u2 = eps1 n.u1
               !     tau.u2 = tau.u1
                an1=rsxy1(i1,i2,i3,axis1,0)   ! normal (an1,an2)
                an2=rsxy1(i1,i2,i3,axis1,1)
                an3=rsxy1(i1,i2,i3,axis1,2)
                aNorm=max(epsx,sqrt(an1**2+an2**2+an3**2))
                an1=an1/aNorm
                an2=an2/aNorm
                an3=an3/aNorm
               ua=u2(j1,j2,j3,ex)
               ub=u2(j1,j2,j3,ey)
               uc=u2(j1,j2,j3,ez)
               nDotU = an1*ua+an2*ub+an3*uc
               if( twilightZone.eq.1 )then
                ! adjust for TZ forcing (here we assume the exact solution is the same on both sides)
                call ogderiv(ep, 0,0,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),xy1(i1,i2,i3,2),t, ex, ue )
                call ogderiv(ep, 0,0,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),xy1(i1,i2,i3,2),t, ey, ve )
                call ogderiv(ep, 0,0,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),xy1(i1,i2,i3,2),t, ez, we )
                nDotU = nDotU - (an1*ue+an2*ve+an3*we)
         !   write(*,'(" jump(2): (i1,i2,i3)=",3i3," j1,j2,j3=",3i3)') i1,i2,i3,j1,j2,j3
         !   write(*,'(" jump(2): x,y,z=",3e10.2," ua,ue=",2e10.2," ub,ve=",2e10.2," uc,we=",2e10.2)') xy1(i1,i2,i3,0),xy1(i1,i2,i3,1),xy1(i1,i2,i3,2),ua,ue,ub,ve,uc,we
               end if
               u1(i1,i2,i3,ex) = ua + (nDotU*epsRatio - nDotU)*an1
               u1(i1,i2,i3,ey) = ub + (nDotU*epsRatio - nDotU)*an2
               u1(i1,i2,i3,ez) = uc + (nDotU*epsRatio - nDotU)*an3
               end if
                j1=j1+1
               end do
               j2=j2+1
              end do
               j3=j3+1
              end do
           end if

        if( .false. )then
           j3=mm3a
           do i3=nn3a,nn3b
           j2=mm2a
           do i2=nn2a,nn2b
            j1=mm1a
            do i1=nn1a,nn1b
           write(debugFile,'(" -->JUMP v1(",i2,":",i2,",",i2,",",i2,") 
     & =",3f9.4)') i1-1,i1+1,i2,i3,u1(i1-1,i2,i3,ey),u1(i1,i2,i3,ey),
     & u1(i1+1,i2,i3,ey)
           ! '
             j1=j1+1
            end do
            j2=j2+1
           end do
            j3=j3+1
           end do
        end if
         ! initialization step: assign first ghost line by extrapolation
         ! NOTE: assign ghost points outside the ends
         if( .true. )then
          j3=mm3a
          do i3=nn3a,nn3b
          j2=mm2a
          do i2=nn2a,nn2b
           j1=mm1a
           do i1=nn1a,nn1b
            u1(i1-is1,i2-is2,i3-is3,ex)=(3.*u1(i1,i2,i3,ex)-3.*u1(i1+
     & is1,i2+is2,i3+is3,ex)+u1(i1+2*is1,i2+2*is2,i3+2*is3,ex))
            u1(i1-is1,i2-is2,i3-is3,ey)=(3.*u1(i1,i2,i3,ey)-3.*u1(i1+
     & is1,i2+is2,i3+is3,ey)+u1(i1+2*is1,i2+2*is2,i3+2*is3,ey))
            u1(i1-is1,i2-is2,i3-is3,ez)=(3.*u1(i1,i2,i3,ez)-3.*u1(i1+
     & is1,i2+is2,i3+is3,ez)+u1(i1+2*is1,i2+2*is2,i3+2*is3,ez))

            u2(j1-js1,j2-js2,j3-js3,ex)=(3.*u2(j1,j2,j3,ex)-3.*u2(j1+
     & js1,j2+js2,j3+js3,ex)+u2(j1+2*js1,j2+2*js2,j3+2*js3,ex))
            u2(j1-js1,j2-js2,j3-js3,ey)=(3.*u2(j1,j2,j3,ey)-3.*u2(j1+
     & js1,j2+js2,j3+js3,ey)+u2(j1+2*js1,j2+2*js2,j3+2*js3,ey))
            u2(j1-js1,j2-js2,j3-js3,ez)=(3.*u2(j1,j2,j3,ez)-3.*u2(j1+
     & js1,j2+js2,j3+js3,ez)+u2(j1+2*js1,j2+2*js2,j3+2*js3,ez))
            j1=j1+1
           end do
           j2=j2+1
          end do
           j3=j3+1
          end do
         end if

         if( .false. )then
          ! just copy values from ghost points for now -- this will be the true soln if eps1=eps2 and grids match
           j3=m3a
           do i3=n3a,n3b
           j2=m2a
           do i2=n2a,n2b
           j1=m1a
           do i1=n1a,n1b
           u1(i1-is1,i2-is2,i3-is3,ex)=u2(j1+js1,j2+js2,j3+js3,ex)
           u1(i1-is1,i2-is2,i3-is3,ey)=u2(j1+js1,j2+js2,j3+js3,ey)
           u1(i1-is1,i2-is2,i3-is3,ez)=u2(j1+js1,j2+js2,j3+js3,ez)
           u2(j1-js1,j2-js2,j3-js3,ex)=u1(i1+is1,i2+is2,i3+is3,ex)
           u2(j1-js1,j2-js2,j3-js3,ey)=u1(i1+is1,i2+is2,i3+is3,ey)
           u2(j1-js1,j2-js2,j3-js3,ez)=u1(i1+is1,i2+is2,i3+is3,ez)
             j1=j1+1
            end do
            j2=j2+1
           end do
            j3=j3+1
           end do
         end if

        if( .false. )then
           j3=mm3a
           do i3=nn3a,nn3b
           j2=mm2a
           do i2=nn2a,nn2b
            j1=mm1a
            do i1=nn1a,nn1b
           write(debugFile,'(" -->EXTRAP v1(",i2,":",i2,",",i2,",",i2,
     & ") =",3f9.4)') i1-1,i1+1,i2,i3,u1(i1-1,i2,i3,ey),u1(i1,i2,i3,
     & ey),u1(i1+1,i2,i3,ey)
           ! '
             j1=j1+1
            end do
            j2=j2+1
           end do
            j3=j3+1
           end do
        end if


         ! here are the jump conditions for the ghost points
         !   [ div(E) n + (curl(E)- n.curl(E) n )/mu ] =0                 (3 eqns)
         !   [ Lap(E)/(eps*mu) + (1/mu)*(1-1/eps)*( n.Lap(E) ) n ] = 0    (3 eqns)

         ! These correspond to the 6 conditions:
         !   [ div(E) ] =0 
         !   [ tau. curl(E)/mu ] = 0       (2 tangents)
         !   [ n.Lap(E)/mu ] = 0 
         !   [ tau.Lap(E)/(eps*mu) ] = 0   (2 tangents)


          j3=m3a
          do i3=n3a,n3b
          j2=m2a
          do i2=n2a,n2b
          j1=m1a
          do i1=n1a,n1b
          if( mask1(i1,i2,i3).gt.0 .and. mask2(j1,j2,j3).gt.0 )then

           ! here is the normal (assumed to be the same on both sides)
           an1=rsxy1(i1,i2,i3,axis1,0)   ! normal (an1,an2)
           an2=rsxy1(i1,i2,i3,axis1,1)
           an3=rsxy1(i1,i2,i3,axis1,2)
           aNorm=max(epsx,sqrt(an1**2+an2**2+an3**2))
           an1=an1/aNorm
           an2=an2/aNorm
           an3=an3/aNorm


           ! --- first evaluate the equations we want to solve with the wrong values at the ghost points:

           cem1=(1.-1./eps1)/mu1
           cem2=(1.-1./eps2)/mu2
           ! evalInterfaceDerivatives3d
            ! NOTE: the jacobian derivatives can be computed once for all components
             ! this next call will define the jacobian and its derivatives (parameteric and spatial)
             aj1rx = rsxy1(i1,i2,i3,0,0)
             aj1rxr = (-rsxy1(i1-1,i2,i3,0,0)+rsxy1(i1+1,i2,i3,0,0))/(
     & 2.*dr1(0))
             aj1rxs = (-rsxy1(i1,i2-1,i3,0,0)+rsxy1(i1,i2+1,i3,0,0))/(
     & 2.*dr1(1))
             aj1rxt = (-rsxy1(i1,i2,i3-1,0,0)+rsxy1(i1,i2,i3+1,0,0))/(
     & 2.*dr1(2))
             aj1sx = rsxy1(i1,i2,i3,1,0)
             aj1sxr = (-rsxy1(i1-1,i2,i3,1,0)+rsxy1(i1+1,i2,i3,1,0))/(
     & 2.*dr1(0))
             aj1sxs = (-rsxy1(i1,i2-1,i3,1,0)+rsxy1(i1,i2+1,i3,1,0))/(
     & 2.*dr1(1))
             aj1sxt = (-rsxy1(i1,i2,i3-1,1,0)+rsxy1(i1,i2,i3+1,1,0))/(
     & 2.*dr1(2))
             aj1tx = rsxy1(i1,i2,i3,2,0)
             aj1txr = (-rsxy1(i1-1,i2,i3,2,0)+rsxy1(i1+1,i2,i3,2,0))/(
     & 2.*dr1(0))
             aj1txs = (-rsxy1(i1,i2-1,i3,2,0)+rsxy1(i1,i2+1,i3,2,0))/(
     & 2.*dr1(1))
             aj1txt = (-rsxy1(i1,i2,i3-1,2,0)+rsxy1(i1,i2,i3+1,2,0))/(
     & 2.*dr1(2))
             aj1ry = rsxy1(i1,i2,i3,0,1)
             aj1ryr = (-rsxy1(i1-1,i2,i3,0,1)+rsxy1(i1+1,i2,i3,0,1))/(
     & 2.*dr1(0))
             aj1rys = (-rsxy1(i1,i2-1,i3,0,1)+rsxy1(i1,i2+1,i3,0,1))/(
     & 2.*dr1(1))
             aj1ryt = (-rsxy1(i1,i2,i3-1,0,1)+rsxy1(i1,i2,i3+1,0,1))/(
     & 2.*dr1(2))
             aj1sy = rsxy1(i1,i2,i3,1,1)
             aj1syr = (-rsxy1(i1-1,i2,i3,1,1)+rsxy1(i1+1,i2,i3,1,1))/(
     & 2.*dr1(0))
             aj1sys = (-rsxy1(i1,i2-1,i3,1,1)+rsxy1(i1,i2+1,i3,1,1))/(
     & 2.*dr1(1))
             aj1syt = (-rsxy1(i1,i2,i3-1,1,1)+rsxy1(i1,i2,i3+1,1,1))/(
     & 2.*dr1(2))
             aj1ty = rsxy1(i1,i2,i3,2,1)
             aj1tyr = (-rsxy1(i1-1,i2,i3,2,1)+rsxy1(i1+1,i2,i3,2,1))/(
     & 2.*dr1(0))
             aj1tys = (-rsxy1(i1,i2-1,i3,2,1)+rsxy1(i1,i2+1,i3,2,1))/(
     & 2.*dr1(1))
             aj1tyt = (-rsxy1(i1,i2,i3-1,2,1)+rsxy1(i1,i2,i3+1,2,1))/(
     & 2.*dr1(2))
             aj1rz = rsxy1(i1,i2,i3,0,2)
             aj1rzr = (-rsxy1(i1-1,i2,i3,0,2)+rsxy1(i1+1,i2,i3,0,2))/(
     & 2.*dr1(0))
             aj1rzs = (-rsxy1(i1,i2-1,i3,0,2)+rsxy1(i1,i2+1,i3,0,2))/(
     & 2.*dr1(1))
             aj1rzt = (-rsxy1(i1,i2,i3-1,0,2)+rsxy1(i1,i2,i3+1,0,2))/(
     & 2.*dr1(2))
             aj1sz = rsxy1(i1,i2,i3,1,2)
             aj1szr = (-rsxy1(i1-1,i2,i3,1,2)+rsxy1(i1+1,i2,i3,1,2))/(
     & 2.*dr1(0))
             aj1szs = (-rsxy1(i1,i2-1,i3,1,2)+rsxy1(i1,i2+1,i3,1,2))/(
     & 2.*dr1(1))
             aj1szt = (-rsxy1(i1,i2,i3-1,1,2)+rsxy1(i1,i2,i3+1,1,2))/(
     & 2.*dr1(2))
             aj1tz = rsxy1(i1,i2,i3,2,2)
             aj1tzr = (-rsxy1(i1-1,i2,i3,2,2)+rsxy1(i1+1,i2,i3,2,2))/(
     & 2.*dr1(0))
             aj1tzs = (-rsxy1(i1,i2-1,i3,2,2)+rsxy1(i1,i2+1,i3,2,2))/(
     & 2.*dr1(1))
             aj1tzt = (-rsxy1(i1,i2,i3-1,2,2)+rsxy1(i1,i2,i3+1,2,2))/(
     & 2.*dr1(2))
             aj1rxx = aj1rx*aj1rxr+aj1sx*aj1rxs+aj1tx*aj1rxt
             aj1rxy = aj1ry*aj1rxr+aj1sy*aj1rxs+aj1ty*aj1rxt
             aj1rxz = aj1rz*aj1rxr+aj1sz*aj1rxs+aj1tz*aj1rxt
             aj1sxx = aj1rx*aj1sxr+aj1sx*aj1sxs+aj1tx*aj1sxt
             aj1sxy = aj1ry*aj1sxr+aj1sy*aj1sxs+aj1ty*aj1sxt
             aj1sxz = aj1rz*aj1sxr+aj1sz*aj1sxs+aj1tz*aj1sxt
             aj1txx = aj1rx*aj1txr+aj1sx*aj1txs+aj1tx*aj1txt
             aj1txy = aj1ry*aj1txr+aj1sy*aj1txs+aj1ty*aj1txt
             aj1txz = aj1rz*aj1txr+aj1sz*aj1txs+aj1tz*aj1txt
             aj1ryx = aj1rx*aj1ryr+aj1sx*aj1rys+aj1tx*aj1ryt
             aj1ryy = aj1ry*aj1ryr+aj1sy*aj1rys+aj1ty*aj1ryt
             aj1ryz = aj1rz*aj1ryr+aj1sz*aj1rys+aj1tz*aj1ryt
             aj1syx = aj1rx*aj1syr+aj1sx*aj1sys+aj1tx*aj1syt
             aj1syy = aj1ry*aj1syr+aj1sy*aj1sys+aj1ty*aj1syt
             aj1syz = aj1rz*aj1syr+aj1sz*aj1sys+aj1tz*aj1syt
             aj1tyx = aj1rx*aj1tyr+aj1sx*aj1tys+aj1tx*aj1tyt
             aj1tyy = aj1ry*aj1tyr+aj1sy*aj1tys+aj1ty*aj1tyt
             aj1tyz = aj1rz*aj1tyr+aj1sz*aj1tys+aj1tz*aj1tyt
             aj1rzx = aj1rx*aj1rzr+aj1sx*aj1rzs+aj1tx*aj1rzt
             aj1rzy = aj1ry*aj1rzr+aj1sy*aj1rzs+aj1ty*aj1rzt
             aj1rzz = aj1rz*aj1rzr+aj1sz*aj1rzs+aj1tz*aj1rzt
             aj1szx = aj1rx*aj1szr+aj1sx*aj1szs+aj1tx*aj1szt
             aj1szy = aj1ry*aj1szr+aj1sy*aj1szs+aj1ty*aj1szt
             aj1szz = aj1rz*aj1szr+aj1sz*aj1szs+aj1tz*aj1szt
             aj1tzx = aj1rx*aj1tzr+aj1sx*aj1tzs+aj1tx*aj1tzt
             aj1tzy = aj1ry*aj1tzr+aj1sy*aj1tzs+aj1ty*aj1tzt
             aj1tzz = aj1rz*aj1tzr+aj1sz*aj1tzs+aj1tz*aj1tzt
              uu1 = u1(i1,i2,i3,ex)
              uu1r = (-u1(i1-1,i2,i3,ex)+u1(i1+1,i2,i3,ex))/(2.*dr1(0))
              uu1s = (-u1(i1,i2-1,i3,ex)+u1(i1,i2+1,i3,ex))/(2.*dr1(1))
              uu1t = (-u1(i1,i2,i3-1,ex)+u1(i1,i2,i3+1,ex))/(2.*dr1(2))
              uu1rr = (u1(i1-1,i2,i3,ex)-2.*u1(i1,i2,i3,ex)+u1(i1+1,i2,
     & i3,ex))/(dr1(0)**2)
              uu1rs = (-(-u1(i1-1,i2-1,i3,ex)+u1(i1-1,i2+1,i3,ex))/(2.*
     & dr1(1))+(-u1(i1+1,i2-1,i3,ex)+u1(i1+1,i2+1,i3,ex))/(2.*dr1(1)))
     & /(2.*dr1(0))
              uu1ss = (u1(i1,i2-1,i3,ex)-2.*u1(i1,i2,i3,ex)+u1(i1,i2+1,
     & i3,ex))/(dr1(1)**2)
              uu1rt = (-(-u1(i1-1,i2,i3-1,ex)+u1(i1-1,i2,i3+1,ex))/(2.*
     & dr1(2))+(-u1(i1+1,i2,i3-1,ex)+u1(i1+1,i2,i3+1,ex))/(2.*dr1(2)))
     & /(2.*dr1(0))
              uu1st = (-(-u1(i1,i2-1,i3-1,ex)+u1(i1,i2-1,i3+1,ex))/(2.*
     & dr1(2))+(-u1(i1,i2+1,i3-1,ex)+u1(i1,i2+1,i3+1,ex))/(2.*dr1(2)))
     & /(2.*dr1(1))
              uu1tt = (u1(i1,i2,i3-1,ex)-2.*u1(i1,i2,i3,ex)+u1(i1,i2,
     & i3+1,ex))/(dr1(2)**2)
               u1x = aj1rx*uu1r+aj1sx*uu1s+aj1tx*uu1t
               u1y = aj1ry*uu1r+aj1sy*uu1s+aj1ty*uu1t
               u1z = aj1rz*uu1r+aj1sz*uu1s+aj1tz*uu1t
               t1 = aj1rx**2
               t6 = aj1sx**2
               t14 = aj1tx**2
               u1xx = t1*uu1rr+2*aj1rx*aj1sx*uu1rs+t6*uu1ss+2*aj1rx*
     & aj1tx*uu1rt+2*aj1sx*aj1tx*uu1st+t14*uu1tt+aj1rxx*uu1r+aj1sxx*
     & uu1s+aj1txx*uu1t
               t1 = aj1ry**2
               t6 = aj1sy**2
               t14 = aj1ty**2
               u1yy = t1*uu1rr+2*aj1ry*aj1sy*uu1rs+t6*uu1ss+2*aj1ry*
     & aj1ty*uu1rt+2*aj1sy*aj1ty*uu1st+t14*uu1tt+aj1ryy*uu1r+aj1syy*
     & uu1s+aj1tyy*uu1t
               t1 = aj1rz**2
               t6 = aj1sz**2
               t14 = aj1tz**2
               u1zz = t1*uu1rr+2*aj1rz*aj1sz*uu1rs+t6*uu1ss+2*aj1rz*
     & aj1tz*uu1rt+2*aj1sz*aj1tz*uu1st+t14*uu1tt+aj1rzz*uu1r+aj1szz*
     & uu1s+aj1tzz*uu1t
             u1Lap = u1xx+ u1yy+ u1zz
              vv1 = u1(i1,i2,i3,ey)
              vv1r = (-u1(i1-1,i2,i3,ey)+u1(i1+1,i2,i3,ey))/(2.*dr1(0))
              vv1s = (-u1(i1,i2-1,i3,ey)+u1(i1,i2+1,i3,ey))/(2.*dr1(1))
              vv1t = (-u1(i1,i2,i3-1,ey)+u1(i1,i2,i3+1,ey))/(2.*dr1(2))
              vv1rr = (u1(i1-1,i2,i3,ey)-2.*u1(i1,i2,i3,ey)+u1(i1+1,i2,
     & i3,ey))/(dr1(0)**2)
              vv1rs = (-(-u1(i1-1,i2-1,i3,ey)+u1(i1-1,i2+1,i3,ey))/(2.*
     & dr1(1))+(-u1(i1+1,i2-1,i3,ey)+u1(i1+1,i2+1,i3,ey))/(2.*dr1(1)))
     & /(2.*dr1(0))
              vv1ss = (u1(i1,i2-1,i3,ey)-2.*u1(i1,i2,i3,ey)+u1(i1,i2+1,
     & i3,ey))/(dr1(1)**2)
              vv1rt = (-(-u1(i1-1,i2,i3-1,ey)+u1(i1-1,i2,i3+1,ey))/(2.*
     & dr1(2))+(-u1(i1+1,i2,i3-1,ey)+u1(i1+1,i2,i3+1,ey))/(2.*dr1(2)))
     & /(2.*dr1(0))
              vv1st = (-(-u1(i1,i2-1,i3-1,ey)+u1(i1,i2-1,i3+1,ey))/(2.*
     & dr1(2))+(-u1(i1,i2+1,i3-1,ey)+u1(i1,i2+1,i3+1,ey))/(2.*dr1(2)))
     & /(2.*dr1(1))
              vv1tt = (u1(i1,i2,i3-1,ey)-2.*u1(i1,i2,i3,ey)+u1(i1,i2,
     & i3+1,ey))/(dr1(2)**2)
               v1x = aj1rx*vv1r+aj1sx*vv1s+aj1tx*vv1t
               v1y = aj1ry*vv1r+aj1sy*vv1s+aj1ty*vv1t
               v1z = aj1rz*vv1r+aj1sz*vv1s+aj1tz*vv1t
               t1 = aj1rx**2
               t6 = aj1sx**2
               t14 = aj1tx**2
               v1xx = t1*vv1rr+2*aj1rx*aj1sx*vv1rs+t6*vv1ss+2*aj1rx*
     & aj1tx*vv1rt+2*aj1sx*aj1tx*vv1st+t14*vv1tt+aj1rxx*vv1r+aj1sxx*
     & vv1s+aj1txx*vv1t
               t1 = aj1ry**2
               t6 = aj1sy**2
               t14 = aj1ty**2
               v1yy = t1*vv1rr+2*aj1ry*aj1sy*vv1rs+t6*vv1ss+2*aj1ry*
     & aj1ty*vv1rt+2*aj1sy*aj1ty*vv1st+t14*vv1tt+aj1ryy*vv1r+aj1syy*
     & vv1s+aj1tyy*vv1t
               t1 = aj1rz**2
               t6 = aj1sz**2
               t14 = aj1tz**2
               v1zz = t1*vv1rr+2*aj1rz*aj1sz*vv1rs+t6*vv1ss+2*aj1rz*
     & aj1tz*vv1rt+2*aj1sz*aj1tz*vv1st+t14*vv1tt+aj1rzz*vv1r+aj1szz*
     & vv1s+aj1tzz*vv1t
             v1Lap = v1xx+ v1yy+ v1zz
              ww1 = u1(i1,i2,i3,ez)
              ww1r = (-u1(i1-1,i2,i3,ez)+u1(i1+1,i2,i3,ez))/(2.*dr1(0))
              ww1s = (-u1(i1,i2-1,i3,ez)+u1(i1,i2+1,i3,ez))/(2.*dr1(1))
              ww1t = (-u1(i1,i2,i3-1,ez)+u1(i1,i2,i3+1,ez))/(2.*dr1(2))
              ww1rr = (u1(i1-1,i2,i3,ez)-2.*u1(i1,i2,i3,ez)+u1(i1+1,i2,
     & i3,ez))/(dr1(0)**2)
              ww1rs = (-(-u1(i1-1,i2-1,i3,ez)+u1(i1-1,i2+1,i3,ez))/(2.*
     & dr1(1))+(-u1(i1+1,i2-1,i3,ez)+u1(i1+1,i2+1,i3,ez))/(2.*dr1(1)))
     & /(2.*dr1(0))
              ww1ss = (u1(i1,i2-1,i3,ez)-2.*u1(i1,i2,i3,ez)+u1(i1,i2+1,
     & i3,ez))/(dr1(1)**2)
              ww1rt = (-(-u1(i1-1,i2,i3-1,ez)+u1(i1-1,i2,i3+1,ez))/(2.*
     & dr1(2))+(-u1(i1+1,i2,i3-1,ez)+u1(i1+1,i2,i3+1,ez))/(2.*dr1(2)))
     & /(2.*dr1(0))
              ww1st = (-(-u1(i1,i2-1,i3-1,ez)+u1(i1,i2-1,i3+1,ez))/(2.*
     & dr1(2))+(-u1(i1,i2+1,i3-1,ez)+u1(i1,i2+1,i3+1,ez))/(2.*dr1(2)))
     & /(2.*dr1(1))
              ww1tt = (u1(i1,i2,i3-1,ez)-2.*u1(i1,i2,i3,ez)+u1(i1,i2,
     & i3+1,ez))/(dr1(2)**2)
               w1x = aj1rx*ww1r+aj1sx*ww1s+aj1tx*ww1t
               w1y = aj1ry*ww1r+aj1sy*ww1s+aj1ty*ww1t
               w1z = aj1rz*ww1r+aj1sz*ww1s+aj1tz*ww1t
               t1 = aj1rx**2
               t6 = aj1sx**2
               t14 = aj1tx**2
               w1xx = t1*ww1rr+2*aj1rx*aj1sx*ww1rs+t6*ww1ss+2*aj1rx*
     & aj1tx*ww1rt+2*aj1sx*aj1tx*ww1st+t14*ww1tt+aj1rxx*ww1r+aj1sxx*
     & ww1s+aj1txx*ww1t
               t1 = aj1ry**2
               t6 = aj1sy**2
               t14 = aj1ty**2
               w1yy = t1*ww1rr+2*aj1ry*aj1sy*ww1rs+t6*ww1ss+2*aj1ry*
     & aj1ty*ww1rt+2*aj1sy*aj1ty*ww1st+t14*ww1tt+aj1ryy*ww1r+aj1syy*
     & ww1s+aj1tyy*ww1t
               t1 = aj1rz**2
               t6 = aj1sz**2
               t14 = aj1tz**2
               w1zz = t1*ww1rr+2*aj1rz*aj1sz*ww1rs+t6*ww1ss+2*aj1rz*
     & aj1tz*ww1rt+2*aj1sz*aj1tz*ww1st+t14*ww1tt+aj1rzz*ww1r+aj1szz*
     & ww1s+aj1tzz*ww1t
             w1Lap = w1xx+ w1yy+ w1zz
            ! NOTE: the jacobian derivatives can be computed once for all components
             ! this next call will define the jacobian and its derivatives (parameteric and spatial)
             aj2rx = rsxy2(j1,j2,j3,0,0)
             aj2rxr = (-rsxy2(j1-1,j2,j3,0,0)+rsxy2(j1+1,j2,j3,0,0))/(
     & 2.*dr2(0))
             aj2rxs = (-rsxy2(j1,j2-1,j3,0,0)+rsxy2(j1,j2+1,j3,0,0))/(
     & 2.*dr2(1))
             aj2rxt = (-rsxy2(j1,j2,j3-1,0,0)+rsxy2(j1,j2,j3+1,0,0))/(
     & 2.*dr2(2))
             aj2sx = rsxy2(j1,j2,j3,1,0)
             aj2sxr = (-rsxy2(j1-1,j2,j3,1,0)+rsxy2(j1+1,j2,j3,1,0))/(
     & 2.*dr2(0))
             aj2sxs = (-rsxy2(j1,j2-1,j3,1,0)+rsxy2(j1,j2+1,j3,1,0))/(
     & 2.*dr2(1))
             aj2sxt = (-rsxy2(j1,j2,j3-1,1,0)+rsxy2(j1,j2,j3+1,1,0))/(
     & 2.*dr2(2))
             aj2tx = rsxy2(j1,j2,j3,2,0)
             aj2txr = (-rsxy2(j1-1,j2,j3,2,0)+rsxy2(j1+1,j2,j3,2,0))/(
     & 2.*dr2(0))
             aj2txs = (-rsxy2(j1,j2-1,j3,2,0)+rsxy2(j1,j2+1,j3,2,0))/(
     & 2.*dr2(1))
             aj2txt = (-rsxy2(j1,j2,j3-1,2,0)+rsxy2(j1,j2,j3+1,2,0))/(
     & 2.*dr2(2))
             aj2ry = rsxy2(j1,j2,j3,0,1)
             aj2ryr = (-rsxy2(j1-1,j2,j3,0,1)+rsxy2(j1+1,j2,j3,0,1))/(
     & 2.*dr2(0))
             aj2rys = (-rsxy2(j1,j2-1,j3,0,1)+rsxy2(j1,j2+1,j3,0,1))/(
     & 2.*dr2(1))
             aj2ryt = (-rsxy2(j1,j2,j3-1,0,1)+rsxy2(j1,j2,j3+1,0,1))/(
     & 2.*dr2(2))
             aj2sy = rsxy2(j1,j2,j3,1,1)
             aj2syr = (-rsxy2(j1-1,j2,j3,1,1)+rsxy2(j1+1,j2,j3,1,1))/(
     & 2.*dr2(0))
             aj2sys = (-rsxy2(j1,j2-1,j3,1,1)+rsxy2(j1,j2+1,j3,1,1))/(
     & 2.*dr2(1))
             aj2syt = (-rsxy2(j1,j2,j3-1,1,1)+rsxy2(j1,j2,j3+1,1,1))/(
     & 2.*dr2(2))
             aj2ty = rsxy2(j1,j2,j3,2,1)
             aj2tyr = (-rsxy2(j1-1,j2,j3,2,1)+rsxy2(j1+1,j2,j3,2,1))/(
     & 2.*dr2(0))
             aj2tys = (-rsxy2(j1,j2-1,j3,2,1)+rsxy2(j1,j2+1,j3,2,1))/(
     & 2.*dr2(1))
             aj2tyt = (-rsxy2(j1,j2,j3-1,2,1)+rsxy2(j1,j2,j3+1,2,1))/(
     & 2.*dr2(2))
             aj2rz = rsxy2(j1,j2,j3,0,2)
             aj2rzr = (-rsxy2(j1-1,j2,j3,0,2)+rsxy2(j1+1,j2,j3,0,2))/(
     & 2.*dr2(0))
             aj2rzs = (-rsxy2(j1,j2-1,j3,0,2)+rsxy2(j1,j2+1,j3,0,2))/(
     & 2.*dr2(1))
             aj2rzt = (-rsxy2(j1,j2,j3-1,0,2)+rsxy2(j1,j2,j3+1,0,2))/(
     & 2.*dr2(2))
             aj2sz = rsxy2(j1,j2,j3,1,2)
             aj2szr = (-rsxy2(j1-1,j2,j3,1,2)+rsxy2(j1+1,j2,j3,1,2))/(
     & 2.*dr2(0))
             aj2szs = (-rsxy2(j1,j2-1,j3,1,2)+rsxy2(j1,j2+1,j3,1,2))/(
     & 2.*dr2(1))
             aj2szt = (-rsxy2(j1,j2,j3-1,1,2)+rsxy2(j1,j2,j3+1,1,2))/(
     & 2.*dr2(2))
             aj2tz = rsxy2(j1,j2,j3,2,2)
             aj2tzr = (-rsxy2(j1-1,j2,j3,2,2)+rsxy2(j1+1,j2,j3,2,2))/(
     & 2.*dr2(0))
             aj2tzs = (-rsxy2(j1,j2-1,j3,2,2)+rsxy2(j1,j2+1,j3,2,2))/(
     & 2.*dr2(1))
             aj2tzt = (-rsxy2(j1,j2,j3-1,2,2)+rsxy2(j1,j2,j3+1,2,2))/(
     & 2.*dr2(2))
             aj2rxx = aj2rx*aj2rxr+aj2sx*aj2rxs+aj2tx*aj2rxt
             aj2rxy = aj2ry*aj2rxr+aj2sy*aj2rxs+aj2ty*aj2rxt
             aj2rxz = aj2rz*aj2rxr+aj2sz*aj2rxs+aj2tz*aj2rxt
             aj2sxx = aj2rx*aj2sxr+aj2sx*aj2sxs+aj2tx*aj2sxt
             aj2sxy = aj2ry*aj2sxr+aj2sy*aj2sxs+aj2ty*aj2sxt
             aj2sxz = aj2rz*aj2sxr+aj2sz*aj2sxs+aj2tz*aj2sxt
             aj2txx = aj2rx*aj2txr+aj2sx*aj2txs+aj2tx*aj2txt
             aj2txy = aj2ry*aj2txr+aj2sy*aj2txs+aj2ty*aj2txt
             aj2txz = aj2rz*aj2txr+aj2sz*aj2txs+aj2tz*aj2txt
             aj2ryx = aj2rx*aj2ryr+aj2sx*aj2rys+aj2tx*aj2ryt
             aj2ryy = aj2ry*aj2ryr+aj2sy*aj2rys+aj2ty*aj2ryt
             aj2ryz = aj2rz*aj2ryr+aj2sz*aj2rys+aj2tz*aj2ryt
             aj2syx = aj2rx*aj2syr+aj2sx*aj2sys+aj2tx*aj2syt
             aj2syy = aj2ry*aj2syr+aj2sy*aj2sys+aj2ty*aj2syt
             aj2syz = aj2rz*aj2syr+aj2sz*aj2sys+aj2tz*aj2syt
             aj2tyx = aj2rx*aj2tyr+aj2sx*aj2tys+aj2tx*aj2tyt
             aj2tyy = aj2ry*aj2tyr+aj2sy*aj2tys+aj2ty*aj2tyt
             aj2tyz = aj2rz*aj2tyr+aj2sz*aj2tys+aj2tz*aj2tyt
             aj2rzx = aj2rx*aj2rzr+aj2sx*aj2rzs+aj2tx*aj2rzt
             aj2rzy = aj2ry*aj2rzr+aj2sy*aj2rzs+aj2ty*aj2rzt
             aj2rzz = aj2rz*aj2rzr+aj2sz*aj2rzs+aj2tz*aj2rzt
             aj2szx = aj2rx*aj2szr+aj2sx*aj2szs+aj2tx*aj2szt
             aj2szy = aj2ry*aj2szr+aj2sy*aj2szs+aj2ty*aj2szt
             aj2szz = aj2rz*aj2szr+aj2sz*aj2szs+aj2tz*aj2szt
             aj2tzx = aj2rx*aj2tzr+aj2sx*aj2tzs+aj2tx*aj2tzt
             aj2tzy = aj2ry*aj2tzr+aj2sy*aj2tzs+aj2ty*aj2tzt
             aj2tzz = aj2rz*aj2tzr+aj2sz*aj2tzs+aj2tz*aj2tzt
              uu2 = u2(j1,j2,j3,ex)
              uu2r = (-u2(j1-1,j2,j3,ex)+u2(j1+1,j2,j3,ex))/(2.*dr2(0))
              uu2s = (-u2(j1,j2-1,j3,ex)+u2(j1,j2+1,j3,ex))/(2.*dr2(1))
              uu2t = (-u2(j1,j2,j3-1,ex)+u2(j1,j2,j3+1,ex))/(2.*dr2(2))
              uu2rr = (u2(j1-1,j2,j3,ex)-2.*u2(j1,j2,j3,ex)+u2(j1+1,j2,
     & j3,ex))/(dr2(0)**2)
              uu2rs = (-(-u2(j1-1,j2-1,j3,ex)+u2(j1-1,j2+1,j3,ex))/(2.*
     & dr2(1))+(-u2(j1+1,j2-1,j3,ex)+u2(j1+1,j2+1,j3,ex))/(2.*dr2(1)))
     & /(2.*dr2(0))
              uu2ss = (u2(j1,j2-1,j3,ex)-2.*u2(j1,j2,j3,ex)+u2(j1,j2+1,
     & j3,ex))/(dr2(1)**2)
              uu2rt = (-(-u2(j1-1,j2,j3-1,ex)+u2(j1-1,j2,j3+1,ex))/(2.*
     & dr2(2))+(-u2(j1+1,j2,j3-1,ex)+u2(j1+1,j2,j3+1,ex))/(2.*dr2(2)))
     & /(2.*dr2(0))
              uu2st = (-(-u2(j1,j2-1,j3-1,ex)+u2(j1,j2-1,j3+1,ex))/(2.*
     & dr2(2))+(-u2(j1,j2+1,j3-1,ex)+u2(j1,j2+1,j3+1,ex))/(2.*dr2(2)))
     & /(2.*dr2(1))
              uu2tt = (u2(j1,j2,j3-1,ex)-2.*u2(j1,j2,j3,ex)+u2(j1,j2,
     & j3+1,ex))/(dr2(2)**2)
               u2x = aj2rx*uu2r+aj2sx*uu2s+aj2tx*uu2t
               u2y = aj2ry*uu2r+aj2sy*uu2s+aj2ty*uu2t
               u2z = aj2rz*uu2r+aj2sz*uu2s+aj2tz*uu2t
               t1 = aj2rx**2
               t6 = aj2sx**2
               t14 = aj2tx**2
               u2xx = t1*uu2rr+2*aj2rx*aj2sx*uu2rs+t6*uu2ss+2*aj2rx*
     & aj2tx*uu2rt+2*aj2sx*aj2tx*uu2st+t14*uu2tt+aj2rxx*uu2r+aj2sxx*
     & uu2s+aj2txx*uu2t
               t1 = aj2ry**2
               t6 = aj2sy**2
               t14 = aj2ty**2
               u2yy = t1*uu2rr+2*aj2ry*aj2sy*uu2rs+t6*uu2ss+2*aj2ry*
     & aj2ty*uu2rt+2*aj2sy*aj2ty*uu2st+t14*uu2tt+aj2ryy*uu2r+aj2syy*
     & uu2s+aj2tyy*uu2t
               t1 = aj2rz**2
               t6 = aj2sz**2
               t14 = aj2tz**2
               u2zz = t1*uu2rr+2*aj2rz*aj2sz*uu2rs+t6*uu2ss+2*aj2rz*
     & aj2tz*uu2rt+2*aj2sz*aj2tz*uu2st+t14*uu2tt+aj2rzz*uu2r+aj2szz*
     & uu2s+aj2tzz*uu2t
             u2Lap = u2xx+ u2yy+ u2zz
              vv2 = u2(j1,j2,j3,ey)
              vv2r = (-u2(j1-1,j2,j3,ey)+u2(j1+1,j2,j3,ey))/(2.*dr2(0))
              vv2s = (-u2(j1,j2-1,j3,ey)+u2(j1,j2+1,j3,ey))/(2.*dr2(1))
              vv2t = (-u2(j1,j2,j3-1,ey)+u2(j1,j2,j3+1,ey))/(2.*dr2(2))
              vv2rr = (u2(j1-1,j2,j3,ey)-2.*u2(j1,j2,j3,ey)+u2(j1+1,j2,
     & j3,ey))/(dr2(0)**2)
              vv2rs = (-(-u2(j1-1,j2-1,j3,ey)+u2(j1-1,j2+1,j3,ey))/(2.*
     & dr2(1))+(-u2(j1+1,j2-1,j3,ey)+u2(j1+1,j2+1,j3,ey))/(2.*dr2(1)))
     & /(2.*dr2(0))
              vv2ss = (u2(j1,j2-1,j3,ey)-2.*u2(j1,j2,j3,ey)+u2(j1,j2+1,
     & j3,ey))/(dr2(1)**2)
              vv2rt = (-(-u2(j1-1,j2,j3-1,ey)+u2(j1-1,j2,j3+1,ey))/(2.*
     & dr2(2))+(-u2(j1+1,j2,j3-1,ey)+u2(j1+1,j2,j3+1,ey))/(2.*dr2(2)))
     & /(2.*dr2(0))
              vv2st = (-(-u2(j1,j2-1,j3-1,ey)+u2(j1,j2-1,j3+1,ey))/(2.*
     & dr2(2))+(-u2(j1,j2+1,j3-1,ey)+u2(j1,j2+1,j3+1,ey))/(2.*dr2(2)))
     & /(2.*dr2(1))
              vv2tt = (u2(j1,j2,j3-1,ey)-2.*u2(j1,j2,j3,ey)+u2(j1,j2,
     & j3+1,ey))/(dr2(2)**2)
               v2x = aj2rx*vv2r+aj2sx*vv2s+aj2tx*vv2t
               v2y = aj2ry*vv2r+aj2sy*vv2s+aj2ty*vv2t
               v2z = aj2rz*vv2r+aj2sz*vv2s+aj2tz*vv2t
               t1 = aj2rx**2
               t6 = aj2sx**2
               t14 = aj2tx**2
               v2xx = t1*vv2rr+2*aj2rx*aj2sx*vv2rs+t6*vv2ss+2*aj2rx*
     & aj2tx*vv2rt+2*aj2sx*aj2tx*vv2st+t14*vv2tt+aj2rxx*vv2r+aj2sxx*
     & vv2s+aj2txx*vv2t
               t1 = aj2ry**2
               t6 = aj2sy**2
               t14 = aj2ty**2
               v2yy = t1*vv2rr+2*aj2ry*aj2sy*vv2rs+t6*vv2ss+2*aj2ry*
     & aj2ty*vv2rt+2*aj2sy*aj2ty*vv2st+t14*vv2tt+aj2ryy*vv2r+aj2syy*
     & vv2s+aj2tyy*vv2t
               t1 = aj2rz**2
               t6 = aj2sz**2
               t14 = aj2tz**2
               v2zz = t1*vv2rr+2*aj2rz*aj2sz*vv2rs+t6*vv2ss+2*aj2rz*
     & aj2tz*vv2rt+2*aj2sz*aj2tz*vv2st+t14*vv2tt+aj2rzz*vv2r+aj2szz*
     & vv2s+aj2tzz*vv2t
             v2Lap = v2xx+ v2yy+ v2zz
              ww2 = u2(j1,j2,j3,ez)
              ww2r = (-u2(j1-1,j2,j3,ez)+u2(j1+1,j2,j3,ez))/(2.*dr2(0))
              ww2s = (-u2(j1,j2-1,j3,ez)+u2(j1,j2+1,j3,ez))/(2.*dr2(1))
              ww2t = (-u2(j1,j2,j3-1,ez)+u2(j1,j2,j3+1,ez))/(2.*dr2(2))
              ww2rr = (u2(j1-1,j2,j3,ez)-2.*u2(j1,j2,j3,ez)+u2(j1+1,j2,
     & j3,ez))/(dr2(0)**2)
              ww2rs = (-(-u2(j1-1,j2-1,j3,ez)+u2(j1-1,j2+1,j3,ez))/(2.*
     & dr2(1))+(-u2(j1+1,j2-1,j3,ez)+u2(j1+1,j2+1,j3,ez))/(2.*dr2(1)))
     & /(2.*dr2(0))
              ww2ss = (u2(j1,j2-1,j3,ez)-2.*u2(j1,j2,j3,ez)+u2(j1,j2+1,
     & j3,ez))/(dr2(1)**2)
              ww2rt = (-(-u2(j1-1,j2,j3-1,ez)+u2(j1-1,j2,j3+1,ez))/(2.*
     & dr2(2))+(-u2(j1+1,j2,j3-1,ez)+u2(j1+1,j2,j3+1,ez))/(2.*dr2(2)))
     & /(2.*dr2(0))
              ww2st = (-(-u2(j1,j2-1,j3-1,ez)+u2(j1,j2-1,j3+1,ez))/(2.*
     & dr2(2))+(-u2(j1,j2+1,j3-1,ez)+u2(j1,j2+1,j3+1,ez))/(2.*dr2(2)))
     & /(2.*dr2(1))
              ww2tt = (u2(j1,j2,j3-1,ez)-2.*u2(j1,j2,j3,ez)+u2(j1,j2,
     & j3+1,ez))/(dr2(2)**2)
               w2x = aj2rx*ww2r+aj2sx*ww2s+aj2tx*ww2t
               w2y = aj2ry*ww2r+aj2sy*ww2s+aj2ty*ww2t
               w2z = aj2rz*ww2r+aj2sz*ww2s+aj2tz*ww2t
               t1 = aj2rx**2
               t6 = aj2sx**2
               t14 = aj2tx**2
               w2xx = t1*ww2rr+2*aj2rx*aj2sx*ww2rs+t6*ww2ss+2*aj2rx*
     & aj2tx*ww2rt+2*aj2sx*aj2tx*ww2st+t14*ww2tt+aj2rxx*ww2r+aj2sxx*
     & ww2s+aj2txx*ww2t
               t1 = aj2ry**2
               t6 = aj2sy**2
               t14 = aj2ty**2
               w2yy = t1*ww2rr+2*aj2ry*aj2sy*ww2rs+t6*ww2ss+2*aj2ry*
     & aj2ty*ww2rt+2*aj2sy*aj2ty*ww2st+t14*ww2tt+aj2ryy*ww2r+aj2syy*
     & ww2s+aj2tyy*ww2t
               t1 = aj2rz**2
               t6 = aj2sz**2
               t14 = aj2tz**2
               w2zz = t1*ww2rr+2*aj2rz*aj2sz*ww2rs+t6*ww2ss+2*aj2rz*
     & aj2tz*ww2rt+2*aj2sz*aj2tz*ww2st+t14*ww2tt+aj2rzz*ww2r+aj2szz*
     & ww2s+aj2tzz*ww2t
             w2Lap = w2xx+ w2yy+ w2zz
            divE1 = u1x+v1y+w1z
            curlE1x = w1y-v1z
            curlE1y = u1z-w1x
            curlE1z = v1x-u1y
            nDotCurlE1=an1*curlE1x+an2*curlE1y+an3*curlE1z
            nDotLapE1 = an1*u1Lap + an2*v1Lap + an3*w1Lap
            divE2 = u2x+v2y+w2z
            curlE2x = w2y-v2z
            curlE2y = u2z-w2x
            curlE2z = v2x-u2y
            nDotCurlE2=an1*curlE2x+an2*curlE2y+an3*curlE2z
            nDotLapE2 = an1*u2Lap + an2*v2Lap + an3*w2Lap
            f(0)=( divE1*an1 + (curlE1x- nDotCurlE1*an1)/mu1 ) - ( 
     & divE2*an1 + (curlE2x- nDotCurlE2*an1)/mu2 )
            f(1)=( divE1*an2 + (curlE1y- nDotCurlE1*an2)/mu1 ) - ( 
     & divE2*an2 + (curlE2y- nDotCurlE2*an2)/mu2 )
            f(2)=( divE1*an3 + (curlE1z- nDotCurlE1*an3)/mu1 ) - ( 
     & divE2*an3 + (curlE2z- nDotCurlE2*an3)/mu2 )
            f(3)=( u1Lap/(epsmu1) + cem1*nDotLapE1*an1 ) - ( u2Lap/(
     & epsmu2) + cem2*nDotLapE2*an1 )
            f(4)=( v1Lap/(epsmu1) + cem1*nDotLapE1*an2 ) - ( v2Lap/(
     & epsmu2) + cem2*nDotLapE2*an2 )
            f(5)=( w1Lap/(epsmu1) + cem1*nDotLapE1*an3 ) - ( w2Lap/(
     & epsmu2) + cem2*nDotLapE2*an3 )
            if( twilightZone.eq.1 )then
              call ogderiv(ep, 0,1,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,xy1(i1,i2,i3,2),t, ex, uex  )
              call ogderiv(ep, 0,0,1,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,xy1(i1,i2,i3,2),t, ex, uey  )
              call ogderiv(ep, 0,0,0,1, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,xy1(i1,i2,i3,2),t, ex, uez  )
              call ogderiv(ep, 0,2,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,xy1(i1,i2,i3,2),t, ex, uexx )
              call ogderiv(ep, 0,0,2,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,xy1(i1,i2,i3,2),t, ex, ueyy )
              call ogderiv(ep, 0,0,0,2, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,xy1(i1,i2,i3,2),t, ex, uezz )
              call ogderiv(ep, 0,1,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,xy1(i1,i2,i3,2),t, ey, vex  )
              call ogderiv(ep, 0,0,1,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,xy1(i1,i2,i3,2),t, ey, vey  )
              call ogderiv(ep, 0,0,0,1, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,xy1(i1,i2,i3,2),t, ey, vez  )
              call ogderiv(ep, 0,2,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,xy1(i1,i2,i3,2),t, ey, vexx )
              call ogderiv(ep, 0,0,2,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,xy1(i1,i2,i3,2),t, ey, veyy )
              call ogderiv(ep, 0,0,0,2, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,xy1(i1,i2,i3,2),t, ey, vezz )
              call ogderiv(ep, 0,1,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,xy1(i1,i2,i3,2),t, ez, wex  )
              call ogderiv(ep, 0,0,1,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,xy1(i1,i2,i3,2),t, ez, wey  )
              call ogderiv(ep, 0,0,0,1, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,xy1(i1,i2,i3,2),t, ez, wez  )
              call ogderiv(ep, 0,2,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,xy1(i1,i2,i3,2),t, ez, wexx )
              call ogderiv(ep, 0,0,2,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,xy1(i1,i2,i3,2),t, ez, weyy )
              call ogderiv(ep, 0,0,0,2, xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
     & ,xy1(i1,i2,i3,2),t, ez, wezz )
              ueLap = uexx+ueyy+uezz
              veLap = vexx+veyy+vezz
              weLap = wexx+weyy+wezz
              curlEex = wey-vez
              curlEey = uez-wex
              curlEez = vex-uey
              nDotCurlEe=an1*curlEex+an2*curlEey+an3*curlEez
              nDotLapEe=an1*ueLap+an2*veLap+an3*weLap
              f(0)= f(0) - ( (curlEex- nDotCurlEe*an1)*(1./mu1-1./mu2) 
     & )
              f(1)= f(1) - ( (curlEey- nDotCurlEe*an2)*(1./mu1-1./mu2) 
     & )
              f(2)= f(2) - ( (curlEez- nDotCurlEe*an3)*(1./mu1-1./mu2) 
     & )
              f(3)= f(3) - ( ueLap*(1./epsmu1-1./epsmu2) + nDotLapEe*
     & an1*(cem1-cem2) )
              f(4)= f(4) - ( veLap*(1./epsmu1-1./epsmu2) + nDotLapEe*
     & an2*(cem1-cem2) )
              f(5)= f(5) - ( weLap*(1./epsmu1-1./epsmu2) + nDotLapEe*
     & an3*(cem1-cem2) )
            end if

           if( debug.gt.4 )then
            write(debugFile,'(" --> 3d-order2-curv: i1,i2,i3=",3i4," f(
     & start)=",6f8.3)') i1,i2,i3,f(0),f(1),f(2),f(3),f(4),f(5)
            ! '
            write(debugFile,'(" --> u1x,u1y,u1z,v1x,v1y,v1z=",6f8.4)') 
     & u1x,u1y,u1z,v1x,v1y,v1z
            write(debugFile,'(" --> u2x,u2y,u2z,v2x,v2y,v2z=",6f8.4)') 
     & u2x,u2y,u2z,v2x,v2y,v2z

            write(debugFile,'(" --> vv1r,vv1s,vv1t         =",3e9.2)') 
     & vv1r,vv1s,vv1t
            do k3=-1,1
            do k2=-1,1
            write(debugFile,'(" --> v1: =",3f8.4)') u1(i1-1,i2+k2,i3+
     & k3,ey),u1(i1,i2+k2,i3+k3,ey),u1(i1+1,i2+k2,i3+k3,ey)
            end do
            end do
            do k3=-1,1
            do k2=-1,1
            write(debugFile,'(" --> v2: =",3f8.4)') u2(j1-1,j2+k2,j3+
     & k3,ey),u2(j1,j2+k2,j3+k3,ey),u2(j1+1,j2+k2,j3+k3,ey)
            end do
            end do
            ! '
           end if

           ! here is the matrix of coefficients for the unknowns u1(-1),v1(-1),w1(-1),  u2(-1),v2(-1),w2(-1)
           ! Solve:
           !     
           !       A [ U ] = A [ U(old) ] - [ f ]

           c1x = -is*rsxy1(i1,i2,i3,axis1,0)/(2.*dr1(axis1))    ! coeff of u1(-1) from D.x
           c1y = -is*rsxy1(i1,i2,i3,axis1,1)/(2.*dr1(axis1))    ! coeff of u1(-1) from D.y
           c1z = -is*rsxy1(i1,i2,i3,axis1,2)/(2.*dr1(axis1))    ! coeff of u1(-1) from D.z

           c2x = -js*rsxy2(j1,j2,j3,axis2,0)/(2.*dr2(axis2))
           c2y = -js*rsxy2(j1,j2,j3,axis2,1)/(2.*dr2(axis2))
           c2z = -js*rsxy2(j1,j2,j3,axis2,2)/(2.*dr2(axis2))

           rxx1(0,0,0)=aj1rxx
           rxx1(0,1,1)=aj1ryy
           rxx1(0,2,2)=aj1rzz
           rxx1(1,0,0)=aj1sxx
           rxx1(1,1,1)=aj1syy
           rxx1(1,2,2)=aj1szz
           rxx1(2,0,0)=aj1txx
           rxx1(2,1,1)=aj1tyy
           rxx1(2,2,2)=aj1tzz

           rxx2(0,0,0)=aj2rxx
           rxx2(0,1,1)=aj2ryy
           rxx2(0,2,2)=aj2rzz
           rxx2(1,0,0)=aj2sxx
           rxx2(1,1,1)=aj2syy
           rxx2(1,2,2)=aj2szz
           rxx2(2,0,0)=aj2txx
           rxx2(2,1,1)=aj2tyy
           rxx2(2,2,2)=aj2tzz

           ! clap1 : coeff of u(-1) from lap = u.xx + u.yy + u.zz

           ! clap1=(rsxy1(i1,i2,i3,axis1,0)**2+rsxy1(i1,i2,i3,axis1,1)**2)/(dr1(axis1)**2) !           -is*(rsxy1x22(i1,i2,i3,axis1,0)+rsxy1y22(i1,i2,i3,axis1,1))/(2.*dr1(axis1))
           ! clap2=(rsxy2(j1,j2,j3,axis2,0)**2+rsxy2(j1,j2,j3,axis2,1)**2)/(dr2(axis2)**2) !             -js*(rsxy2x22(j1,j2,j3,axis2,0)+rsxy2y22(j1,j2,j3,axis2,1))/(2.*dr2(axis2)) 
           clap1=(rsxy1(i1,i2,i3,axis1,0)**2+rsxy1(i1,i2,i3,axis1,1)**
     & 2+rsxy1(i1,i2,i3,axis1,2)**2)/(dr1(axis1)**2) -is*(rxx1(axis1,
     & 0,0)+rxx1(axis1,1,1)+rxx1(axis1,2,2))/(2.*dr1(axis1))
           clap2=(rsxy2(j1,j2,j3,axis2,0)**2+rsxy2(j1,j2,j3,axis2,1)**
     & 2+rsxy2(j1,j2,j3,axis2,2)**2)/(dr2(axis2)**2) -js*(rxx2(axis2,
     & 0,0)+rxx2(axis2,1,1)+rxx2(axis2,2,2))/(2.*dr2(axis2))

           ! cdivE1 =  u.c1x + v.c1y + w.c1z
           ! nDotCurlE1 = (w1y-v1z)*an1 + (u1z-w1x)*an2 + (v1x-u1y)*an3

           ! (u.x+v.y+w.z)*an1 + ( w1y-v1z - nDotCurlE1*an1)/mu1
           a6(0,0) = ( c1x*an1 + (         - (c1z*an2-c1y*an3)*an1 )
     & /mu1 ) ! coeff of u1(-1)
           a6(0,1) = ( c1y*an1 + (    -c1z - (c1x*an3-c1z*an1)*an1 )
     & /mu1 ) ! coeff of v1(-1)
           a6(0,2) = ( c1z*an1 + ( c1y     - (c1y*an1-c1x*an2)*an1 )
     & /mu1 ) ! coeff of w1(-1)

           a6(0,3) =-( c2x*an1 + (         - (c2z*an2-c2y*an3)*an1 )
     & /mu2 ) ! coeff of u2(-1)
           a6(0,4) =-( c2y*an1 + (    -c2z - (c2x*an3-c2z*an1)*an1 )
     & /mu2 ) ! coeff of v2(-1)
           a6(0,5) =-( c2z*an1 + ( c2y     - (c2y*an1-c2x*an2)*an1 )
     & /mu2 ) ! coeff of w2(-1)

           ! (u.x+v.y+w.z)*an2 + ( u1z-w1x - nDotCurlE1*an2)/mu1
           a6(1,0) = ( c1x*an2 + ( c1z     - (c1z*an2-c1y*an3)*an2 )
     & /mu1 ) ! coeff of u1(-1)
           a6(1,1) = ( c1y*an2 + (         - (c1x*an3+c1z*an1)*an2 )
     & /mu1 ) ! coeff of v1(-1)
           a6(1,2) = ( c1z*an2 + (    -c1x - (c1y*an1-c1x*an2)*an2 )
     & /mu1 ) ! coeff of w1(-1)

           a6(1,3) =-( c2x*an2 + ( c2z     - (c2z*an2-c2y*an3)*an2 )
     & /mu2 ) ! coeff of u2(-1)
           a6(1,4) =-( c2y*an2 + (         - (c2x*an3+c2z*an1)*an2 )
     & /mu2 ) ! coeff of v2(-1)
           a6(1,5) =-( c2z*an2 + (    -c2x - (c2y*an1-c2x*an2)*an2 )
     & /mu2 ) ! coeff of w2(-1)

           ! (u.x+v.y+w.z)*an3 + ( v1x-u1y - nDotCurlE1*an2)/mu1
           a6(2,0) = ( c1x*an3 + (    -c1y - (c1z*an2-c1y*an3)*an3 )
     & /mu1 ) ! coeff of u1(-1)
           a6(2,1) = ( c1y*an3 + ( c1x     - (c1x*an3+c1z*an1)*an3 )
     & /mu1 ) ! coeff of v1(-1)
           a6(2,2) = ( c1z*an3 + (         - (c1y*an1-c1x*an2)*an3 )
     & /mu1 ) ! coeff of w1(-1)

           a6(2,3) =-( c2x*an3 + (    -c2y - (c2z*an2-c2y*an3)*an3 )
     & /mu2 ) ! coeff of u2(-1)
           a6(2,4) =-( c2y*an3 + ( c2x     - (c2x*an3+c2z*an1)*an3 )
     & /mu2 ) ! coeff of v2(-1)
           a6(2,5) =-( c2z*an3 + (         - (c2y*an1-c2x*an2)*an3 )
     & /mu2 ) ! coeff of w2(-1)

           !  u1Lap/(epsmu1) + cem1*( an1*u1Lap + an2*v1Lap + an3*w1Lap )*an1
           a6(3,0) = ( clap1/(epsmu1) + cem1*( an1*clap1               
     &           )*an1 ) ! coeff of u1(-1)
           a6(3,1) = (                  cem1*(             an2*clap1   
     &           )*an1 )
           a6(3,2) = (                  cem1*(                         
     & an3*clap1 )*an1 )

           a6(3,3) =-( clap2/(epsmu2) + cem2*( an1*clap2               
     &           )*an1 ) ! coeff of u2(-1)
           a6(3,4) =-(                  cem2*(             an2*clap2   
     &           )*an1 )
           a6(3,5) =-(                  cem2*(                         
     & an3*clap2 )*an1 )

           !  v1Lap/(epsmu1) + cem1*( an1*u1Lap + an2*v1Lap + an3*w1Lap )*an2
           a6(4,0) = (                  cem1*( an1*clap1               
     &           )*an2 ) ! coeff of u1(-1)
           a6(4,1) = ( clap1/(epsmu1) + cem1*(             an2*clap1   
     &           )*an2 )
           a6(4,2) = (                  cem1*(                         
     & an3*clap1 )*an2 )

           a6(4,3) =-(                  cem2*( an1*clap2               
     &           )*an2 ) ! coeff of u2(-1)
           a6(4,4) =-( clap2/(epsmu2) + cem2*(             an2*clap2   
     &           )*an2 )
           a6(4,5) =-(                  cem2*(                         
     & an3*clap2 )*an2 )

           !  w1Lap/(epsmu1) + cem1*( an1*u1Lap + an2*v1Lap + an3*w1Lap )*an3
           a6(5,0) = (                  cem1*( an1*clap1               
     &           )*an3 ) ! coeff of u1(-1)
           a6(5,1) = (                  cem1*(             an2*clap1   
     &           )*an3 )
           a6(5,2) = ( clap1/(epsmu1) + cem1*(                         
     & an3*clap1 )*an3 )

           a6(5,3) =-(                  cem2*( an1*clap2               
     &           )*an3 ) ! coeff of u2(-1)
           a6(5,4) =-(                  cem2*(             an2*clap2   
     &           )*an3 )
           a6(5,5) =-( clap2/(epsmu2) + cem2*(                         
     & an3*clap2 )*an3 )


           q(0) = u1(i1-is1,i2-is2,i3-is3,ex)
           q(1) = u1(i1-is1,i2-is2,i3-is3,ey)
           q(2) = u1(i1-is1,i2-is2,i3-is3,ez)
           q(3) = u2(j1-js1,j2-js2,j3-js3,ex)
           q(4) = u2(j1-js1,j2-js2,j3-js3,ey)
           q(5) = u2(j1-js1,j2-js2,j3-js3,ez)

           ! subtract off the contributions from the wrong values at the ghost points:
           do n=0,5
             f(n) = (a6(n,0)*q(0)+a6(n,1)*q(1)+a6(n,2)*q(2)+a6(n,3)*q(
     & 3)+a6(n,4)*q(4)+a6(n,5)*q(5)) - f(n)
           end do
      ! write(debugFile,'(" --> 3d:order2-c: f(subtract)=",6f8.3)') f(0),f(1),f(2),f(3),f(4),f(5)
           ! solve A Q = F
           ! factor the matrix
           numberOfEquations=6
           call dgeco( a6(0,0), numberOfEquations, numberOfEquations, 
     & ipvt(0),rcond,work(0))
           ! solve
      ! write(debugFile,'(" --> 3d:order2-c: rcond=",e10.2)') rcond
           job=0
           call dgesl( a6(0,0), numberOfEquations, numberOfEquations, 
     & ipvt(0), f(0), job)
      ! write(debugFile,'(" --> 3d:order2-c: f(solve)=",6f8.3)') f(0),f(1),f(2),f(3),f(4),f(5)
      ! write(debugFile,'(" --> 3d:order2-c:        q=",6f8.3)') q(0),q(1),q(2),q(3),q(4),q(5)

           u1(i1-is1,i2-is2,i3-is3,ex)=f(0)
           u1(i1-is1,i2-is2,i3-is3,ey)=f(1)
           u1(i1-is1,i2-is2,i3-is3,ez)=f(2)
           u2(j1-js1,j2-js2,j3-js3,ex)=f(3)
           u2(j1-js1,j2-js2,j3-js3,ey)=f(4)
           u2(j1-js1,j2-js2,j3-js3,ez)=f(5)

           if( .false. )then
           u1(i1-is1,i2-is2,i3-is3,ex)=q(0)
           u1(i1-is1,i2-is2,i3-is3,ey)=q(1)
           u1(i1-is1,i2-is2,i3-is3,ez)=q(2)
           u2(j1-js1,j2-js2,j3-js3,ex)=q(3)
           u2(j1-js1,j2-js2,j3-js3,ey)=q(4)
           u2(j1-js1,j2-js2,j3-js3,ez)=q(5)
           end if

           if( debug.gt.3 )then ! re-evaluate
             ! NOTE: the jacobian derivatives can be computed once for all components
              ! this next call will define the jacobian and its derivatives (parameteric and spatial)
              aj1rx = rsxy1(i1,i2,i3,0,0)
              aj1rxr = (-rsxy1(i1-1,i2,i3,0,0)+rsxy1(i1+1,i2,i3,0,0))/(
     & 2.*dr1(0))
              aj1rxs = (-rsxy1(i1,i2-1,i3,0,0)+rsxy1(i1,i2+1,i3,0,0))/(
     & 2.*dr1(1))
              aj1rxt = (-rsxy1(i1,i2,i3-1,0,0)+rsxy1(i1,i2,i3+1,0,0))/(
     & 2.*dr1(2))
              aj1sx = rsxy1(i1,i2,i3,1,0)
              aj1sxr = (-rsxy1(i1-1,i2,i3,1,0)+rsxy1(i1+1,i2,i3,1,0))/(
     & 2.*dr1(0))
              aj1sxs = (-rsxy1(i1,i2-1,i3,1,0)+rsxy1(i1,i2+1,i3,1,0))/(
     & 2.*dr1(1))
              aj1sxt = (-rsxy1(i1,i2,i3-1,1,0)+rsxy1(i1,i2,i3+1,1,0))/(
     & 2.*dr1(2))
              aj1tx = rsxy1(i1,i2,i3,2,0)
              aj1txr = (-rsxy1(i1-1,i2,i3,2,0)+rsxy1(i1+1,i2,i3,2,0))/(
     & 2.*dr1(0))
              aj1txs = (-rsxy1(i1,i2-1,i3,2,0)+rsxy1(i1,i2+1,i3,2,0))/(
     & 2.*dr1(1))
              aj1txt = (-rsxy1(i1,i2,i3-1,2,0)+rsxy1(i1,i2,i3+1,2,0))/(
     & 2.*dr1(2))
              aj1ry = rsxy1(i1,i2,i3,0,1)
              aj1ryr = (-rsxy1(i1-1,i2,i3,0,1)+rsxy1(i1+1,i2,i3,0,1))/(
     & 2.*dr1(0))
              aj1rys = (-rsxy1(i1,i2-1,i3,0,1)+rsxy1(i1,i2+1,i3,0,1))/(
     & 2.*dr1(1))
              aj1ryt = (-rsxy1(i1,i2,i3-1,0,1)+rsxy1(i1,i2,i3+1,0,1))/(
     & 2.*dr1(2))
              aj1sy = rsxy1(i1,i2,i3,1,1)
              aj1syr = (-rsxy1(i1-1,i2,i3,1,1)+rsxy1(i1+1,i2,i3,1,1))/(
     & 2.*dr1(0))
              aj1sys = (-rsxy1(i1,i2-1,i3,1,1)+rsxy1(i1,i2+1,i3,1,1))/(
     & 2.*dr1(1))
              aj1syt = (-rsxy1(i1,i2,i3-1,1,1)+rsxy1(i1,i2,i3+1,1,1))/(
     & 2.*dr1(2))
              aj1ty = rsxy1(i1,i2,i3,2,1)
              aj1tyr = (-rsxy1(i1-1,i2,i3,2,1)+rsxy1(i1+1,i2,i3,2,1))/(
     & 2.*dr1(0))
              aj1tys = (-rsxy1(i1,i2-1,i3,2,1)+rsxy1(i1,i2+1,i3,2,1))/(
     & 2.*dr1(1))
              aj1tyt = (-rsxy1(i1,i2,i3-1,2,1)+rsxy1(i1,i2,i3+1,2,1))/(
     & 2.*dr1(2))
              aj1rz = rsxy1(i1,i2,i3,0,2)
              aj1rzr = (-rsxy1(i1-1,i2,i3,0,2)+rsxy1(i1+1,i2,i3,0,2))/(
     & 2.*dr1(0))
              aj1rzs = (-rsxy1(i1,i2-1,i3,0,2)+rsxy1(i1,i2+1,i3,0,2))/(
     & 2.*dr1(1))
              aj1rzt = (-rsxy1(i1,i2,i3-1,0,2)+rsxy1(i1,i2,i3+1,0,2))/(
     & 2.*dr1(2))
              aj1sz = rsxy1(i1,i2,i3,1,2)
              aj1szr = (-rsxy1(i1-1,i2,i3,1,2)+rsxy1(i1+1,i2,i3,1,2))/(
     & 2.*dr1(0))
              aj1szs = (-rsxy1(i1,i2-1,i3,1,2)+rsxy1(i1,i2+1,i3,1,2))/(
     & 2.*dr1(1))
              aj1szt = (-rsxy1(i1,i2,i3-1,1,2)+rsxy1(i1,i2,i3+1,1,2))/(
     & 2.*dr1(2))
              aj1tz = rsxy1(i1,i2,i3,2,2)
              aj1tzr = (-rsxy1(i1-1,i2,i3,2,2)+rsxy1(i1+1,i2,i3,2,2))/(
     & 2.*dr1(0))
              aj1tzs = (-rsxy1(i1,i2-1,i3,2,2)+rsxy1(i1,i2+1,i3,2,2))/(
     & 2.*dr1(1))
              aj1tzt = (-rsxy1(i1,i2,i3-1,2,2)+rsxy1(i1,i2,i3+1,2,2))/(
     & 2.*dr1(2))
              aj1rxx = aj1rx*aj1rxr+aj1sx*aj1rxs+aj1tx*aj1rxt
              aj1rxy = aj1ry*aj1rxr+aj1sy*aj1rxs+aj1ty*aj1rxt
              aj1rxz = aj1rz*aj1rxr+aj1sz*aj1rxs+aj1tz*aj1rxt
              aj1sxx = aj1rx*aj1sxr+aj1sx*aj1sxs+aj1tx*aj1sxt
              aj1sxy = aj1ry*aj1sxr+aj1sy*aj1sxs+aj1ty*aj1sxt
              aj1sxz = aj1rz*aj1sxr+aj1sz*aj1sxs+aj1tz*aj1sxt
              aj1txx = aj1rx*aj1txr+aj1sx*aj1txs+aj1tx*aj1txt
              aj1txy = aj1ry*aj1txr+aj1sy*aj1txs+aj1ty*aj1txt
              aj1txz = aj1rz*aj1txr+aj1sz*aj1txs+aj1tz*aj1txt
              aj1ryx = aj1rx*aj1ryr+aj1sx*aj1rys+aj1tx*aj1ryt
              aj1ryy = aj1ry*aj1ryr+aj1sy*aj1rys+aj1ty*aj1ryt
              aj1ryz = aj1rz*aj1ryr+aj1sz*aj1rys+aj1tz*aj1ryt
              aj1syx = aj1rx*aj1syr+aj1sx*aj1sys+aj1tx*aj1syt
              aj1syy = aj1ry*aj1syr+aj1sy*aj1sys+aj1ty*aj1syt
              aj1syz = aj1rz*aj1syr+aj1sz*aj1sys+aj1tz*aj1syt
              aj1tyx = aj1rx*aj1tyr+aj1sx*aj1tys+aj1tx*aj1tyt
              aj1tyy = aj1ry*aj1tyr+aj1sy*aj1tys+aj1ty*aj1tyt
              aj1tyz = aj1rz*aj1tyr+aj1sz*aj1tys+aj1tz*aj1tyt
              aj1rzx = aj1rx*aj1rzr+aj1sx*aj1rzs+aj1tx*aj1rzt
              aj1rzy = aj1ry*aj1rzr+aj1sy*aj1rzs+aj1ty*aj1rzt
              aj1rzz = aj1rz*aj1rzr+aj1sz*aj1rzs+aj1tz*aj1rzt
              aj1szx = aj1rx*aj1szr+aj1sx*aj1szs+aj1tx*aj1szt
              aj1szy = aj1ry*aj1szr+aj1sy*aj1szs+aj1ty*aj1szt
              aj1szz = aj1rz*aj1szr+aj1sz*aj1szs+aj1tz*aj1szt
              aj1tzx = aj1rx*aj1tzr+aj1sx*aj1tzs+aj1tx*aj1tzt
              aj1tzy = aj1ry*aj1tzr+aj1sy*aj1tzs+aj1ty*aj1tzt
              aj1tzz = aj1rz*aj1tzr+aj1sz*aj1tzs+aj1tz*aj1tzt
               uu1 = u1(i1,i2,i3,ex)
               uu1r = (-u1(i1-1,i2,i3,ex)+u1(i1+1,i2,i3,ex))/(2.*dr1(0)
     & )
               uu1s = (-u1(i1,i2-1,i3,ex)+u1(i1,i2+1,i3,ex))/(2.*dr1(1)
     & )
               uu1t = (-u1(i1,i2,i3-1,ex)+u1(i1,i2,i3+1,ex))/(2.*dr1(2)
     & )
               uu1rr = (u1(i1-1,i2,i3,ex)-2.*u1(i1,i2,i3,ex)+u1(i1+1,
     & i2,i3,ex))/(dr1(0)**2)
               uu1rs = (-(-u1(i1-1,i2-1,i3,ex)+u1(i1-1,i2+1,i3,ex))/(
     & 2.*dr1(1))+(-u1(i1+1,i2-1,i3,ex)+u1(i1+1,i2+1,i3,ex))/(2.*dr1(
     & 1)))/(2.*dr1(0))
               uu1ss = (u1(i1,i2-1,i3,ex)-2.*u1(i1,i2,i3,ex)+u1(i1,i2+
     & 1,i3,ex))/(dr1(1)**2)
               uu1rt = (-(-u1(i1-1,i2,i3-1,ex)+u1(i1-1,i2,i3+1,ex))/(
     & 2.*dr1(2))+(-u1(i1+1,i2,i3-1,ex)+u1(i1+1,i2,i3+1,ex))/(2.*dr1(
     & 2)))/(2.*dr1(0))
               uu1st = (-(-u1(i1,i2-1,i3-1,ex)+u1(i1,i2-1,i3+1,ex))/(
     & 2.*dr1(2))+(-u1(i1,i2+1,i3-1,ex)+u1(i1,i2+1,i3+1,ex))/(2.*dr1(
     & 2)))/(2.*dr1(1))
               uu1tt = (u1(i1,i2,i3-1,ex)-2.*u1(i1,i2,i3,ex)+u1(i1,i2,
     & i3+1,ex))/(dr1(2)**2)
                u1x = aj1rx*uu1r+aj1sx*uu1s+aj1tx*uu1t
                u1y = aj1ry*uu1r+aj1sy*uu1s+aj1ty*uu1t
                u1z = aj1rz*uu1r+aj1sz*uu1s+aj1tz*uu1t
                t1 = aj1rx**2
                t6 = aj1sx**2
                t14 = aj1tx**2
                u1xx = t1*uu1rr+2*aj1rx*aj1sx*uu1rs+t6*uu1ss+2*aj1rx*
     & aj1tx*uu1rt+2*aj1sx*aj1tx*uu1st+t14*uu1tt+aj1rxx*uu1r+aj1sxx*
     & uu1s+aj1txx*uu1t
                t1 = aj1ry**2
                t6 = aj1sy**2
                t14 = aj1ty**2
                u1yy = t1*uu1rr+2*aj1ry*aj1sy*uu1rs+t6*uu1ss+2*aj1ry*
     & aj1ty*uu1rt+2*aj1sy*aj1ty*uu1st+t14*uu1tt+aj1ryy*uu1r+aj1syy*
     & uu1s+aj1tyy*uu1t
                t1 = aj1rz**2
                t6 = aj1sz**2
                t14 = aj1tz**2
                u1zz = t1*uu1rr+2*aj1rz*aj1sz*uu1rs+t6*uu1ss+2*aj1rz*
     & aj1tz*uu1rt+2*aj1sz*aj1tz*uu1st+t14*uu1tt+aj1rzz*uu1r+aj1szz*
     & uu1s+aj1tzz*uu1t
              u1Lap = u1xx+ u1yy+ u1zz
               vv1 = u1(i1,i2,i3,ey)
               vv1r = (-u1(i1-1,i2,i3,ey)+u1(i1+1,i2,i3,ey))/(2.*dr1(0)
     & )
               vv1s = (-u1(i1,i2-1,i3,ey)+u1(i1,i2+1,i3,ey))/(2.*dr1(1)
     & )
               vv1t = (-u1(i1,i2,i3-1,ey)+u1(i1,i2,i3+1,ey))/(2.*dr1(2)
     & )
               vv1rr = (u1(i1-1,i2,i3,ey)-2.*u1(i1,i2,i3,ey)+u1(i1+1,
     & i2,i3,ey))/(dr1(0)**2)
               vv1rs = (-(-u1(i1-1,i2-1,i3,ey)+u1(i1-1,i2+1,i3,ey))/(
     & 2.*dr1(1))+(-u1(i1+1,i2-1,i3,ey)+u1(i1+1,i2+1,i3,ey))/(2.*dr1(
     & 1)))/(2.*dr1(0))
               vv1ss = (u1(i1,i2-1,i3,ey)-2.*u1(i1,i2,i3,ey)+u1(i1,i2+
     & 1,i3,ey))/(dr1(1)**2)
               vv1rt = (-(-u1(i1-1,i2,i3-1,ey)+u1(i1-1,i2,i3+1,ey))/(
     & 2.*dr1(2))+(-u1(i1+1,i2,i3-1,ey)+u1(i1+1,i2,i3+1,ey))/(2.*dr1(
     & 2)))/(2.*dr1(0))
               vv1st = (-(-u1(i1,i2-1,i3-1,ey)+u1(i1,i2-1,i3+1,ey))/(
     & 2.*dr1(2))+(-u1(i1,i2+1,i3-1,ey)+u1(i1,i2+1,i3+1,ey))/(2.*dr1(
     & 2)))/(2.*dr1(1))
               vv1tt = (u1(i1,i2,i3-1,ey)-2.*u1(i1,i2,i3,ey)+u1(i1,i2,
     & i3+1,ey))/(dr1(2)**2)
                v1x = aj1rx*vv1r+aj1sx*vv1s+aj1tx*vv1t
                v1y = aj1ry*vv1r+aj1sy*vv1s+aj1ty*vv1t
                v1z = aj1rz*vv1r+aj1sz*vv1s+aj1tz*vv1t
                t1 = aj1rx**2
                t6 = aj1sx**2
                t14 = aj1tx**2
                v1xx = t1*vv1rr+2*aj1rx*aj1sx*vv1rs+t6*vv1ss+2*aj1rx*
     & aj1tx*vv1rt+2*aj1sx*aj1tx*vv1st+t14*vv1tt+aj1rxx*vv1r+aj1sxx*
     & vv1s+aj1txx*vv1t
                t1 = aj1ry**2
                t6 = aj1sy**2
                t14 = aj1ty**2
                v1yy = t1*vv1rr+2*aj1ry*aj1sy*vv1rs+t6*vv1ss+2*aj1ry*
     & aj1ty*vv1rt+2*aj1sy*aj1ty*vv1st+t14*vv1tt+aj1ryy*vv1r+aj1syy*
     & vv1s+aj1tyy*vv1t
                t1 = aj1rz**2
                t6 = aj1sz**2
                t14 = aj1tz**2
                v1zz = t1*vv1rr+2*aj1rz*aj1sz*vv1rs+t6*vv1ss+2*aj1rz*
     & aj1tz*vv1rt+2*aj1sz*aj1tz*vv1st+t14*vv1tt+aj1rzz*vv1r+aj1szz*
     & vv1s+aj1tzz*vv1t
              v1Lap = v1xx+ v1yy+ v1zz
               ww1 = u1(i1,i2,i3,ez)
               ww1r = (-u1(i1-1,i2,i3,ez)+u1(i1+1,i2,i3,ez))/(2.*dr1(0)
     & )
               ww1s = (-u1(i1,i2-1,i3,ez)+u1(i1,i2+1,i3,ez))/(2.*dr1(1)
     & )
               ww1t = (-u1(i1,i2,i3-1,ez)+u1(i1,i2,i3+1,ez))/(2.*dr1(2)
     & )
               ww1rr = (u1(i1-1,i2,i3,ez)-2.*u1(i1,i2,i3,ez)+u1(i1+1,
     & i2,i3,ez))/(dr1(0)**2)
               ww1rs = (-(-u1(i1-1,i2-1,i3,ez)+u1(i1-1,i2+1,i3,ez))/(
     & 2.*dr1(1))+(-u1(i1+1,i2-1,i3,ez)+u1(i1+1,i2+1,i3,ez))/(2.*dr1(
     & 1)))/(2.*dr1(0))
               ww1ss = (u1(i1,i2-1,i3,ez)-2.*u1(i1,i2,i3,ez)+u1(i1,i2+
     & 1,i3,ez))/(dr1(1)**2)
               ww1rt = (-(-u1(i1-1,i2,i3-1,ez)+u1(i1-1,i2,i3+1,ez))/(
     & 2.*dr1(2))+(-u1(i1+1,i2,i3-1,ez)+u1(i1+1,i2,i3+1,ez))/(2.*dr1(
     & 2)))/(2.*dr1(0))
               ww1st = (-(-u1(i1,i2-1,i3-1,ez)+u1(i1,i2-1,i3+1,ez))/(
     & 2.*dr1(2))+(-u1(i1,i2+1,i3-1,ez)+u1(i1,i2+1,i3+1,ez))/(2.*dr1(
     & 2)))/(2.*dr1(1))
               ww1tt = (u1(i1,i2,i3-1,ez)-2.*u1(i1,i2,i3,ez)+u1(i1,i2,
     & i3+1,ez))/(dr1(2)**2)
                w1x = aj1rx*ww1r+aj1sx*ww1s+aj1tx*ww1t
                w1y = aj1ry*ww1r+aj1sy*ww1s+aj1ty*ww1t
                w1z = aj1rz*ww1r+aj1sz*ww1s+aj1tz*ww1t
                t1 = aj1rx**2
                t6 = aj1sx**2
                t14 = aj1tx**2
                w1xx = t1*ww1rr+2*aj1rx*aj1sx*ww1rs+t6*ww1ss+2*aj1rx*
     & aj1tx*ww1rt+2*aj1sx*aj1tx*ww1st+t14*ww1tt+aj1rxx*ww1r+aj1sxx*
     & ww1s+aj1txx*ww1t
                t1 = aj1ry**2
                t6 = aj1sy**2
                t14 = aj1ty**2
                w1yy = t1*ww1rr+2*aj1ry*aj1sy*ww1rs+t6*ww1ss+2*aj1ry*
     & aj1ty*ww1rt+2*aj1sy*aj1ty*ww1st+t14*ww1tt+aj1ryy*ww1r+aj1syy*
     & ww1s+aj1tyy*ww1t
                t1 = aj1rz**2
                t6 = aj1sz**2
                t14 = aj1tz**2
                w1zz = t1*ww1rr+2*aj1rz*aj1sz*ww1rs+t6*ww1ss+2*aj1rz*
     & aj1tz*ww1rt+2*aj1sz*aj1tz*ww1st+t14*ww1tt+aj1rzz*ww1r+aj1szz*
     & ww1s+aj1tzz*ww1t
              w1Lap = w1xx+ w1yy+ w1zz
             ! NOTE: the jacobian derivatives can be computed once for all components
              ! this next call will define the jacobian and its derivatives (parameteric and spatial)
              aj2rx = rsxy2(j1,j2,j3,0,0)
              aj2rxr = (-rsxy2(j1-1,j2,j3,0,0)+rsxy2(j1+1,j2,j3,0,0))/(
     & 2.*dr2(0))
              aj2rxs = (-rsxy2(j1,j2-1,j3,0,0)+rsxy2(j1,j2+1,j3,0,0))/(
     & 2.*dr2(1))
              aj2rxt = (-rsxy2(j1,j2,j3-1,0,0)+rsxy2(j1,j2,j3+1,0,0))/(
     & 2.*dr2(2))
              aj2sx = rsxy2(j1,j2,j3,1,0)
              aj2sxr = (-rsxy2(j1-1,j2,j3,1,0)+rsxy2(j1+1,j2,j3,1,0))/(
     & 2.*dr2(0))
              aj2sxs = (-rsxy2(j1,j2-1,j3,1,0)+rsxy2(j1,j2+1,j3,1,0))/(
     & 2.*dr2(1))
              aj2sxt = (-rsxy2(j1,j2,j3-1,1,0)+rsxy2(j1,j2,j3+1,1,0))/(
     & 2.*dr2(2))
              aj2tx = rsxy2(j1,j2,j3,2,0)
              aj2txr = (-rsxy2(j1-1,j2,j3,2,0)+rsxy2(j1+1,j2,j3,2,0))/(
     & 2.*dr2(0))
              aj2txs = (-rsxy2(j1,j2-1,j3,2,0)+rsxy2(j1,j2+1,j3,2,0))/(
     & 2.*dr2(1))
              aj2txt = (-rsxy2(j1,j2,j3-1,2,0)+rsxy2(j1,j2,j3+1,2,0))/(
     & 2.*dr2(2))
              aj2ry = rsxy2(j1,j2,j3,0,1)
              aj2ryr = (-rsxy2(j1-1,j2,j3,0,1)+rsxy2(j1+1,j2,j3,0,1))/(
     & 2.*dr2(0))
              aj2rys = (-rsxy2(j1,j2-1,j3,0,1)+rsxy2(j1,j2+1,j3,0,1))/(
     & 2.*dr2(1))
              aj2ryt = (-rsxy2(j1,j2,j3-1,0,1)+rsxy2(j1,j2,j3+1,0,1))/(
     & 2.*dr2(2))
              aj2sy = rsxy2(j1,j2,j3,1,1)
              aj2syr = (-rsxy2(j1-1,j2,j3,1,1)+rsxy2(j1+1,j2,j3,1,1))/(
     & 2.*dr2(0))
              aj2sys = (-rsxy2(j1,j2-1,j3,1,1)+rsxy2(j1,j2+1,j3,1,1))/(
     & 2.*dr2(1))
              aj2syt = (-rsxy2(j1,j2,j3-1,1,1)+rsxy2(j1,j2,j3+1,1,1))/(
     & 2.*dr2(2))
              aj2ty = rsxy2(j1,j2,j3,2,1)
              aj2tyr = (-rsxy2(j1-1,j2,j3,2,1)+rsxy2(j1+1,j2,j3,2,1))/(
     & 2.*dr2(0))
              aj2tys = (-rsxy2(j1,j2-1,j3,2,1)+rsxy2(j1,j2+1,j3,2,1))/(
     & 2.*dr2(1))
              aj2tyt = (-rsxy2(j1,j2,j3-1,2,1)+rsxy2(j1,j2,j3+1,2,1))/(
     & 2.*dr2(2))
              aj2rz = rsxy2(j1,j2,j3,0,2)
              aj2rzr = (-rsxy2(j1-1,j2,j3,0,2)+rsxy2(j1+1,j2,j3,0,2))/(
     & 2.*dr2(0))
              aj2rzs = (-rsxy2(j1,j2-1,j3,0,2)+rsxy2(j1,j2+1,j3,0,2))/(
     & 2.*dr2(1))
              aj2rzt = (-rsxy2(j1,j2,j3-1,0,2)+rsxy2(j1,j2,j3+1,0,2))/(
     & 2.*dr2(2))
              aj2sz = rsxy2(j1,j2,j3,1,2)
              aj2szr = (-rsxy2(j1-1,j2,j3,1,2)+rsxy2(j1+1,j2,j3,1,2))/(
     & 2.*dr2(0))
              aj2szs = (-rsxy2(j1,j2-1,j3,1,2)+rsxy2(j1,j2+1,j3,1,2))/(
     & 2.*dr2(1))
              aj2szt = (-rsxy2(j1,j2,j3-1,1,2)+rsxy2(j1,j2,j3+1,1,2))/(
     & 2.*dr2(2))
              aj2tz = rsxy2(j1,j2,j3,2,2)
              aj2tzr = (-rsxy2(j1-1,j2,j3,2,2)+rsxy2(j1+1,j2,j3,2,2))/(
     & 2.*dr2(0))
              aj2tzs = (-rsxy2(j1,j2-1,j3,2,2)+rsxy2(j1,j2+1,j3,2,2))/(
     & 2.*dr2(1))
              aj2tzt = (-rsxy2(j1,j2,j3-1,2,2)+rsxy2(j1,j2,j3+1,2,2))/(
     & 2.*dr2(2))
              aj2rxx = aj2rx*aj2rxr+aj2sx*aj2rxs+aj2tx*aj2rxt
              aj2rxy = aj2ry*aj2rxr+aj2sy*aj2rxs+aj2ty*aj2rxt
              aj2rxz = aj2rz*aj2rxr+aj2sz*aj2rxs+aj2tz*aj2rxt
              aj2sxx = aj2rx*aj2sxr+aj2sx*aj2sxs+aj2tx*aj2sxt
              aj2sxy = aj2ry*aj2sxr+aj2sy*aj2sxs+aj2ty*aj2sxt
              aj2sxz = aj2rz*aj2sxr+aj2sz*aj2sxs+aj2tz*aj2sxt
              aj2txx = aj2rx*aj2txr+aj2sx*aj2txs+aj2tx*aj2txt
              aj2txy = aj2ry*aj2txr+aj2sy*aj2txs+aj2ty*aj2txt
              aj2txz = aj2rz*aj2txr+aj2sz*aj2txs+aj2tz*aj2txt
              aj2ryx = aj2rx*aj2ryr+aj2sx*aj2rys+aj2tx*aj2ryt
              aj2ryy = aj2ry*aj2ryr+aj2sy*aj2rys+aj2ty*aj2ryt
              aj2ryz = aj2rz*aj2ryr+aj2sz*aj2rys+aj2tz*aj2ryt
              aj2syx = aj2rx*aj2syr+aj2sx*aj2sys+aj2tx*aj2syt
              aj2syy = aj2ry*aj2syr+aj2sy*aj2sys+aj2ty*aj2syt
              aj2syz = aj2rz*aj2syr+aj2sz*aj2sys+aj2tz*aj2syt
              aj2tyx = aj2rx*aj2tyr+aj2sx*aj2tys+aj2tx*aj2tyt
              aj2tyy = aj2ry*aj2tyr+aj2sy*aj2tys+aj2ty*aj2tyt
              aj2tyz = aj2rz*aj2tyr+aj2sz*aj2tys+aj2tz*aj2tyt
              aj2rzx = aj2rx*aj2rzr+aj2sx*aj2rzs+aj2tx*aj2rzt
              aj2rzy = aj2ry*aj2rzr+aj2sy*aj2rzs+aj2ty*aj2rzt
              aj2rzz = aj2rz*aj2rzr+aj2sz*aj2rzs+aj2tz*aj2rzt
              aj2szx = aj2rx*aj2szr+aj2sx*aj2szs+aj2tx*aj2szt
              aj2szy = aj2ry*aj2szr+aj2sy*aj2szs+aj2ty*aj2szt
              aj2szz = aj2rz*aj2szr+aj2sz*aj2szs+aj2tz*aj2szt
              aj2tzx = aj2rx*aj2tzr+aj2sx*aj2tzs+aj2tx*aj2tzt
              aj2tzy = aj2ry*aj2tzr+aj2sy*aj2tzs+aj2ty*aj2tzt
              aj2tzz = aj2rz*aj2tzr+aj2sz*aj2tzs+aj2tz*aj2tzt
               uu2 = u2(j1,j2,j3,ex)
               uu2r = (-u2(j1-1,j2,j3,ex)+u2(j1+1,j2,j3,ex))/(2.*dr2(0)
     & )
               uu2s = (-u2(j1,j2-1,j3,ex)+u2(j1,j2+1,j3,ex))/(2.*dr2(1)
     & )
               uu2t = (-u2(j1,j2,j3-1,ex)+u2(j1,j2,j3+1,ex))/(2.*dr2(2)
     & )
               uu2rr = (u2(j1-1,j2,j3,ex)-2.*u2(j1,j2,j3,ex)+u2(j1+1,
     & j2,j3,ex))/(dr2(0)**2)
               uu2rs = (-(-u2(j1-1,j2-1,j3,ex)+u2(j1-1,j2+1,j3,ex))/(
     & 2.*dr2(1))+(-u2(j1+1,j2-1,j3,ex)+u2(j1+1,j2+1,j3,ex))/(2.*dr2(
     & 1)))/(2.*dr2(0))
               uu2ss = (u2(j1,j2-1,j3,ex)-2.*u2(j1,j2,j3,ex)+u2(j1,j2+
     & 1,j3,ex))/(dr2(1)**2)
               uu2rt = (-(-u2(j1-1,j2,j3-1,ex)+u2(j1-1,j2,j3+1,ex))/(
     & 2.*dr2(2))+(-u2(j1+1,j2,j3-1,ex)+u2(j1+1,j2,j3+1,ex))/(2.*dr2(
     & 2)))/(2.*dr2(0))
               uu2st = (-(-u2(j1,j2-1,j3-1,ex)+u2(j1,j2-1,j3+1,ex))/(
     & 2.*dr2(2))+(-u2(j1,j2+1,j3-1,ex)+u2(j1,j2+1,j3+1,ex))/(2.*dr2(
     & 2)))/(2.*dr2(1))
               uu2tt = (u2(j1,j2,j3-1,ex)-2.*u2(j1,j2,j3,ex)+u2(j1,j2,
     & j3+1,ex))/(dr2(2)**2)
                u2x = aj2rx*uu2r+aj2sx*uu2s+aj2tx*uu2t
                u2y = aj2ry*uu2r+aj2sy*uu2s+aj2ty*uu2t
                u2z = aj2rz*uu2r+aj2sz*uu2s+aj2tz*uu2t
                t1 = aj2rx**2
                t6 = aj2sx**2
                t14 = aj2tx**2
                u2xx = t1*uu2rr+2*aj2rx*aj2sx*uu2rs+t6*uu2ss+2*aj2rx*
     & aj2tx*uu2rt+2*aj2sx*aj2tx*uu2st+t14*uu2tt+aj2rxx*uu2r+aj2sxx*
     & uu2s+aj2txx*uu2t
                t1 = aj2ry**2
                t6 = aj2sy**2
                t14 = aj2ty**2
                u2yy = t1*uu2rr+2*aj2ry*aj2sy*uu2rs+t6*uu2ss+2*aj2ry*
     & aj2ty*uu2rt+2*aj2sy*aj2ty*uu2st+t14*uu2tt+aj2ryy*uu2r+aj2syy*
     & uu2s+aj2tyy*uu2t
                t1 = aj2rz**2
                t6 = aj2sz**2
                t14 = aj2tz**2
                u2zz = t1*uu2rr+2*aj2rz*aj2sz*uu2rs+t6*uu2ss+2*aj2rz*
     & aj2tz*uu2rt+2*aj2sz*aj2tz*uu2st+t14*uu2tt+aj2rzz*uu2r+aj2szz*
     & uu2s+aj2tzz*uu2t
              u2Lap = u2xx+ u2yy+ u2zz
               vv2 = u2(j1,j2,j3,ey)
               vv2r = (-u2(j1-1,j2,j3,ey)+u2(j1+1,j2,j3,ey))/(2.*dr2(0)
     & )
               vv2s = (-u2(j1,j2-1,j3,ey)+u2(j1,j2+1,j3,ey))/(2.*dr2(1)
     & )
               vv2t = (-u2(j1,j2,j3-1,ey)+u2(j1,j2,j3+1,ey))/(2.*dr2(2)
     & )
               vv2rr = (u2(j1-1,j2,j3,ey)-2.*u2(j1,j2,j3,ey)+u2(j1+1,
     & j2,j3,ey))/(dr2(0)**2)
               vv2rs = (-(-u2(j1-1,j2-1,j3,ey)+u2(j1-1,j2+1,j3,ey))/(
     & 2.*dr2(1))+(-u2(j1+1,j2-1,j3,ey)+u2(j1+1,j2+1,j3,ey))/(2.*dr2(
     & 1)))/(2.*dr2(0))
               vv2ss = (u2(j1,j2-1,j3,ey)-2.*u2(j1,j2,j3,ey)+u2(j1,j2+
     & 1,j3,ey))/(dr2(1)**2)
               vv2rt = (-(-u2(j1-1,j2,j3-1,ey)+u2(j1-1,j2,j3+1,ey))/(
     & 2.*dr2(2))+(-u2(j1+1,j2,j3-1,ey)+u2(j1+1,j2,j3+1,ey))/(2.*dr2(
     & 2)))/(2.*dr2(0))
               vv2st = (-(-u2(j1,j2-1,j3-1,ey)+u2(j1,j2-1,j3+1,ey))/(
     & 2.*dr2(2))+(-u2(j1,j2+1,j3-1,ey)+u2(j1,j2+1,j3+1,ey))/(2.*dr2(
     & 2)))/(2.*dr2(1))
               vv2tt = (u2(j1,j2,j3-1,ey)-2.*u2(j1,j2,j3,ey)+u2(j1,j2,
     & j3+1,ey))/(dr2(2)**2)
                v2x = aj2rx*vv2r+aj2sx*vv2s+aj2tx*vv2t
                v2y = aj2ry*vv2r+aj2sy*vv2s+aj2ty*vv2t
                v2z = aj2rz*vv2r+aj2sz*vv2s+aj2tz*vv2t
                t1 = aj2rx**2
                t6 = aj2sx**2
                t14 = aj2tx**2
                v2xx = t1*vv2rr+2*aj2rx*aj2sx*vv2rs+t6*vv2ss+2*aj2rx*
     & aj2tx*vv2rt+2*aj2sx*aj2tx*vv2st+t14*vv2tt+aj2rxx*vv2r+aj2sxx*
     & vv2s+aj2txx*vv2t
                t1 = aj2ry**2
                t6 = aj2sy**2
                t14 = aj2ty**2
                v2yy = t1*vv2rr+2*aj2ry*aj2sy*vv2rs+t6*vv2ss+2*aj2ry*
     & aj2ty*vv2rt+2*aj2sy*aj2ty*vv2st+t14*vv2tt+aj2ryy*vv2r+aj2syy*
     & vv2s+aj2tyy*vv2t
                t1 = aj2rz**2
                t6 = aj2sz**2
                t14 = aj2tz**2
                v2zz = t1*vv2rr+2*aj2rz*aj2sz*vv2rs+t6*vv2ss+2*aj2rz*
     & aj2tz*vv2rt+2*aj2sz*aj2tz*vv2st+t14*vv2tt+aj2rzz*vv2r+aj2szz*
     & vv2s+aj2tzz*vv2t
              v2Lap = v2xx+ v2yy+ v2zz
               ww2 = u2(j1,j2,j3,ez)
               ww2r = (-u2(j1-1,j2,j3,ez)+u2(j1+1,j2,j3,ez))/(2.*dr2(0)
     & )
               ww2s = (-u2(j1,j2-1,j3,ez)+u2(j1,j2+1,j3,ez))/(2.*dr2(1)
     & )
               ww2t = (-u2(j1,j2,j3-1,ez)+u2(j1,j2,j3+1,ez))/(2.*dr2(2)
     & )
               ww2rr = (u2(j1-1,j2,j3,ez)-2.*u2(j1,j2,j3,ez)+u2(j1+1,
     & j2,j3,ez))/(dr2(0)**2)
               ww2rs = (-(-u2(j1-1,j2-1,j3,ez)+u2(j1-1,j2+1,j3,ez))/(
     & 2.*dr2(1))+(-u2(j1+1,j2-1,j3,ez)+u2(j1+1,j2+1,j3,ez))/(2.*dr2(
     & 1)))/(2.*dr2(0))
               ww2ss = (u2(j1,j2-1,j3,ez)-2.*u2(j1,j2,j3,ez)+u2(j1,j2+
     & 1,j3,ez))/(dr2(1)**2)
               ww2rt = (-(-u2(j1-1,j2,j3-1,ez)+u2(j1-1,j2,j3+1,ez))/(
     & 2.*dr2(2))+(-u2(j1+1,j2,j3-1,ez)+u2(j1+1,j2,j3+1,ez))/(2.*dr2(
     & 2)))/(2.*dr2(0))
               ww2st = (-(-u2(j1,j2-1,j3-1,ez)+u2(j1,j2-1,j3+1,ez))/(
     & 2.*dr2(2))+(-u2(j1,j2+1,j3-1,ez)+u2(j1,j2+1,j3+1,ez))/(2.*dr2(
     & 2)))/(2.*dr2(1))
               ww2tt = (u2(j1,j2,j3-1,ez)-2.*u2(j1,j2,j3,ez)+u2(j1,j2,
     & j3+1,ez))/(dr2(2)**2)
                w2x = aj2rx*ww2r+aj2sx*ww2s+aj2tx*ww2t
                w2y = aj2ry*ww2r+aj2sy*ww2s+aj2ty*ww2t
                w2z = aj2rz*ww2r+aj2sz*ww2s+aj2tz*ww2t
                t1 = aj2rx**2
                t6 = aj2sx**2
                t14 = aj2tx**2
                w2xx = t1*ww2rr+2*aj2rx*aj2sx*ww2rs+t6*ww2ss+2*aj2rx*
     & aj2tx*ww2rt+2*aj2sx*aj2tx*ww2st+t14*ww2tt+aj2rxx*ww2r+aj2sxx*
     & ww2s+aj2txx*ww2t
                t1 = aj2ry**2
                t6 = aj2sy**2
                t14 = aj2ty**2
                w2yy = t1*ww2rr+2*aj2ry*aj2sy*ww2rs+t6*ww2ss+2*aj2ry*
     & aj2ty*ww2rt+2*aj2sy*aj2ty*ww2st+t14*ww2tt+aj2ryy*ww2r+aj2syy*
     & ww2s+aj2tyy*ww2t
                t1 = aj2rz**2
                t6 = aj2sz**2
                t14 = aj2tz**2
                w2zz = t1*ww2rr+2*aj2rz*aj2sz*ww2rs+t6*ww2ss+2*aj2rz*
     & aj2tz*ww2rt+2*aj2sz*aj2tz*ww2st+t14*ww2tt+aj2rzz*ww2r+aj2szz*
     & ww2s+aj2tzz*ww2t
              w2Lap = w2xx+ w2yy+ w2zz
             divE1 = u1x+v1y+w1z
             curlE1x = w1y-v1z
             curlE1y = u1z-w1x
             curlE1z = v1x-u1y
             nDotCurlE1=an1*curlE1x+an2*curlE1y+an3*curlE1z
             nDotLapE1 = an1*u1Lap + an2*v1Lap + an3*w1Lap
             divE2 = u2x+v2y+w2z
             curlE2x = w2y-v2z
             curlE2y = u2z-w2x
             curlE2z = v2x-u2y
             nDotCurlE2=an1*curlE2x+an2*curlE2y+an3*curlE2z
             nDotLapE2 = an1*u2Lap + an2*v2Lap + an3*w2Lap
             f(0)=( divE1*an1 + (curlE1x- nDotCurlE1*an1)/mu1 ) - ( 
     & divE2*an1 + (curlE2x- nDotCurlE2*an1)/mu2 )
             f(1)=( divE1*an2 + (curlE1y- nDotCurlE1*an2)/mu1 ) - ( 
     & divE2*an2 + (curlE2y- nDotCurlE2*an2)/mu2 )
             f(2)=( divE1*an3 + (curlE1z- nDotCurlE1*an3)/mu1 ) - ( 
     & divE2*an3 + (curlE2z- nDotCurlE2*an3)/mu2 )
             f(3)=( u1Lap/(epsmu1) + cem1*nDotLapE1*an1 ) - ( u2Lap/(
     & epsmu2) + cem2*nDotLapE2*an1 )
             f(4)=( v1Lap/(epsmu1) + cem1*nDotLapE1*an2 ) - ( v2Lap/(
     & epsmu2) + cem2*nDotLapE2*an2 )
             f(5)=( w1Lap/(epsmu1) + cem1*nDotLapE1*an3 ) - ( w2Lap/(
     & epsmu2) + cem2*nDotLapE2*an3 )
             if( twilightZone.eq.1 )then
               call ogderiv(ep, 0,1,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),xy1(i1,i2,i3,2),t, ex, uex  )
               call ogderiv(ep, 0,0,1,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),xy1(i1,i2,i3,2),t, ex, uey  )
               call ogderiv(ep, 0,0,0,1, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),xy1(i1,i2,i3,2),t, ex, uez  )
               call ogderiv(ep, 0,2,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),xy1(i1,i2,i3,2),t, ex, uexx )
               call ogderiv(ep, 0,0,2,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),xy1(i1,i2,i3,2),t, ex, ueyy )
               call ogderiv(ep, 0,0,0,2, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),xy1(i1,i2,i3,2),t, ex, uezz )
               call ogderiv(ep, 0,1,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),xy1(i1,i2,i3,2),t, ey, vex  )
               call ogderiv(ep, 0,0,1,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),xy1(i1,i2,i3,2),t, ey, vey  )
               call ogderiv(ep, 0,0,0,1, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),xy1(i1,i2,i3,2),t, ey, vez  )
               call ogderiv(ep, 0,2,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),xy1(i1,i2,i3,2),t, ey, vexx )
               call ogderiv(ep, 0,0,2,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),xy1(i1,i2,i3,2),t, ey, veyy )
               call ogderiv(ep, 0,0,0,2, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),xy1(i1,i2,i3,2),t, ey, vezz )
               call ogderiv(ep, 0,1,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),xy1(i1,i2,i3,2),t, ez, wex  )
               call ogderiv(ep, 0,0,1,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),xy1(i1,i2,i3,2),t, ez, wey  )
               call ogderiv(ep, 0,0,0,1, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),xy1(i1,i2,i3,2),t, ez, wez  )
               call ogderiv(ep, 0,2,0,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),xy1(i1,i2,i3,2),t, ez, wexx )
               call ogderiv(ep, 0,0,2,0, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),xy1(i1,i2,i3,2),t, ez, weyy )
               call ogderiv(ep, 0,0,0,2, xy1(i1,i2,i3,0),xy1(i1,i2,i3,
     & 1),xy1(i1,i2,i3,2),t, ez, wezz )
               ueLap = uexx+ueyy+uezz
               veLap = vexx+veyy+vezz
               weLap = wexx+weyy+wezz
               curlEex = wey-vez
               curlEey = uez-wex
               curlEez = vex-uey
               nDotCurlEe=an1*curlEex+an2*curlEey+an3*curlEez
               nDotLapEe=an1*ueLap+an2*veLap+an3*weLap
               f(0)= f(0) - ( (curlEex- nDotCurlEe*an1)*(1./mu1-1./mu2)
     &  )
               f(1)= f(1) - ( (curlEey- nDotCurlEe*an2)*(1./mu1-1./mu2)
     &  )
               f(2)= f(2) - ( (curlEez- nDotCurlEe*an3)*(1./mu1-1./mu2)
     &  )
               f(3)= f(3) - ( ueLap*(1./epsmu1-1./epsmu2) + nDotLapEe*
     & an1*(cem1-cem2) )
               f(4)= f(4) - ( veLap*(1./epsmu1-1./epsmu2) + nDotLapEe*
     & an2*(cem1-cem2) )
               f(5)= f(5) - ( weLap*(1./epsmu1-1./epsmu2) + nDotLapEe*
     & an3*(cem1-cem2) )
             end if
            write(debugFile,'(" --> 3d-order2-c: i1,i2,i3=",3i4," f(re-
     & eval)=",6e10.2)') i1,i2,i3,f(0),f(1),f(2),f(3),f(4),f(5)
              ! '
           end if

           end if
            j1=j1+1
           end do
           j2=j2+1
          end do
           j3=j3+1
          end do

         if( orderOfAccuracy.eq.4 )then
         ! -- For now we just extrapolate the 2nd ghost line for 4th order --
         ! note: extrap outside all pts (interp pts)
          j3=m3a
          do i3=n3a,n3b
          j2=m2a
          do i2=n2a,n2b
          j1=m1a
          do i1=n1a,n1b
            u1(i1-2*is1,i2-2*is2,i3-2*is3,ex)=(5.*u1(i1-is1,i2-is2,i3-
     & is3,ex)-10.*u1(i1-is1+is1,i2-is2+is2,i3-is3+is3,ex)+10.*u1(i1-
     & is1+2*is1,i2-is2+2*is2,i3-is3+2*is3,ex)-5.*u1(i1-is1+3*is1,i2-
     & is2+3*is2,i3-is3+3*is3,ex)+u1(i1-is1+4*is1,i2-is2+4*is2,i3-is3+
     & 4*is3,ex))
            u1(i1-2*is1,i2-2*is2,i3-2*is3,ey)=(5.*u1(i1-is1,i2-is2,i3-
     & is3,ey)-10.*u1(i1-is1+is1,i2-is2+is2,i3-is3+is3,ey)+10.*u1(i1-
     & is1+2*is1,i2-is2+2*is2,i3-is3+2*is3,ey)-5.*u1(i1-is1+3*is1,i2-
     & is2+3*is2,i3-is3+3*is3,ey)+u1(i1-is1+4*is1,i2-is2+4*is2,i3-is3+
     & 4*is3,ey))
            u1(i1-2*is1,i2-2*is2,i3-2*is3,ez)=(5.*u1(i1-is1,i2-is2,i3-
     & is3,ez)-10.*u1(i1-is1+is1,i2-is2+is2,i3-is3+is3,ez)+10.*u1(i1-
     & is1+2*is1,i2-is2+2*is2,i3-is3+2*is3,ez)-5.*u1(i1-is1+3*is1,i2-
     & is2+3*is2,i3-is3+3*is3,ez)+u1(i1-is1+4*is1,i2-is2+4*is2,i3-is3+
     & 4*is3,ez))

            u2(j1-2*js1,j2-2*js2,j3-2*js3,ex)=(5.*u2(j1-js1,j2-js2,j3-
     & js3,ex)-10.*u2(j1-js1+js1,j2-js2+js2,j3-js3+js3,ex)+10.*u2(j1-
     & js1+2*js1,j2-js2+2*js2,j3-js3+2*js3,ex)-5.*u2(j1-js1+3*js1,j2-
     & js2+3*js2,j3-js3+3*js3,ex)+u2(j1-js1+4*js1,j2-js2+4*js2,j3-js3+
     & 4*js3,ex))
            u2(j1-2*js1,j2-2*js2,j3-2*js3,ey)=(5.*u2(j1-js1,j2-js2,j3-
     & js3,ey)-10.*u2(j1-js1+js1,j2-js2+js2,j3-js3+js3,ey)+10.*u2(j1-
     & js1+2*js1,j2-js2+2*js2,j3-js3+2*js3,ey)-5.*u2(j1-js1+3*js1,j2-
     & js2+3*js2,j3-js3+3*js3,ey)+u2(j1-js1+4*js1,j2-js2+4*js2,j3-js3+
     & 4*js3,ey))
            u2(j1-2*js1,j2-2*js2,j3-2*js3,ez)=(5.*u2(j1-js1,j2-js2,j3-
     & js3,ez)-10.*u2(j1-js1+js1,j2-js2+js2,j3-js3+js3,ez)+10.*u2(j1-
     & js1+2*js1,j2-js2+2*js2,j3-js3+2*js3,ez)-5.*u2(j1-js1+3*js1,j2-
     & js2+3*js2,j3-js3+3*js3,ez)+u2(j1-js1+4*js1,j2-js2+4*js2,j3-js3+
     & 4*js3,ez))
            j1=j1+1
           end do
           j2=j2+1
          end do
           j3=j3+1
          end do
         end if


         ! periodic update
         if( parallel.eq.0 )then
          axisp1=mod(axis1+1,nd)
          axisp2=mod(axis1+2,nd)
          if( boundaryCondition1(0,axisp1).lt.0 .or. 
     & boundaryCondition1(0,axisp2).lt.0 )then
           ! We assume this is done by the calling program
           ! write(*,'("periodicUpdate3d: finish me")')
           ! stop 
          end if
         end if
         if( parallel.eq.0 )then
          axisp1=mod(axis2+1,nd)
          axisp2=mod(axis2+2,nd)
          if( boundaryCondition2(0,axisp1).lt.0 .or. 
     & boundaryCondition2(0,axisp2).lt.0 )then
           ! We assume this is done by the calling program
           ! write(*,'("periodicUpdate3d: finish me")')
           ! stop 
          end if
         end if


       else if( nd.eq.3 .and. orderOfAccuracy.eq.4 .and. 
     & gridType.eq.curvilinear )then

         ! this 3d 4th-order version is in interface3dOrder4.bf:

         call mxInterface3dOrder4( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     & gridIndexRange1, u1, mask1,rsxy1, xy1, boundaryCondition1, 
     & md1a,md1b,md2a,md2b,md3a,md3b,gridIndexRange2, u2, mask2,rsxy2,
     &  xy2, boundaryCondition2, ipar, rpar, aa2,aa4,aa8, ipvt2,ipvt4,
     & ipvt8, ierr )

       else
         write(debugFile,'("interface3d: ERROR: unknown options nd,
     & order=",2i3)') nd,orderOfAccuracy
         stop 3214
       end if

      return
      end
