! This file automatically generated from mxProjectInterp.bf with bpp.
c =================================================================================================
c
c  Project the values at interpolation points to satisfy the divergence constraint 
c               div( eps*E) = rho
c
c =================================================================================================

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

! ******************************************************************************
!   This macro will evaluate the divergence
! ******************************************************************************







! =============================================================================================
!   Macro to evaluate the divergence
! =============================================================================================

! =============================================================================================
!   Macro to pproject the divergence at discretization points next to interpolation points.
!   Input:
!     $DIM, $ORDER, $GRIDTYPE (perl variables)
! =============================================================================================

      subroutine mxProjectInterp( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     & gridIndexRange, u, mask, rsxy, xy, boundaryCondition, ipar, 
     & rpar, ierr )
c =================================================================================================
c  Project the values at interpolation points to satisfy the divergence constraint 
c               div( eps*E) = rho
c 
c   u (input) : solution to be projected 
c 
c =================================================================================================


      implicit none

      integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, ndf1a,ndf1b,ndf2a,
     & ndf2b,ndf3a,ndf3b,n1a,n1b,n2a,n2b,n3a,n3b, ierr

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer gridIndexRange(0:1,0:2)

      integer ipar(0:*),boundaryCondition(0:1,0:2)
      real rpar(0:*)

c     ... local
      integer option,ex,ey,ez,hx,hy,hz,e1,e2,e3,debug,myid,
     & orderOfAccuracy
      real omega,dt,t
      integer side,axis,axisp1,axisp2,i1,i2,i3,j1,j2,j3,k1,k2,k3,is1,
     & is2,is3,grid,gridType
      integer numGhost,numGhost3,w1,w2,w3
      real res,ax,ay,az,aSq,ar,as,at,dx(0:2),dr(0:2)
      logical found

      integer rectangular,curvilinear
      parameter(rectangular=0,curvilinear=1)

      ! for new evaluation method:
      real ux,uy,uz,uxx,uxy,uyy,uxz,uyz,uzz
      real vx,vy,vz,vxx,vxy,vyy,vxz,vyz,vzz
      real wx,wy,wz,wxx,wxy,wyy,wxz,wyz,wzz

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
      real uu,uur,uus,uut,uurr,uurs,uuss,uurt,uust,uutt,uurrr,uurrs,
     & uurss,uusss,uurrt,uurst,uusst,uurtt,uustt,uuttt,uurrrr,uurrrs,
     & uurrss,uursss,uussss,uurrrt,uurrst,uursst,uussst,uurrtt,uurstt,
     & uusstt,uurttt,uusttt,uutttt,uurrrrr,uurrrrs,uurrrss,uurrsss,
     & uurssss,uusssss,uurrrrt,uurrrst,uurrsst,uurssst,uusssst,
     & uurrrtt,uurrstt,uursstt,uussstt,uurrttt,uursttt,uussttt,
     & uurtttt,uustttt,uuttttt,uurrrrrr,uurrrrrs,uurrrrss,uurrrsss,
     & uurrssss,uursssss,uussssss,uurrrrrt,uurrrrst,uurrrsst,uurrssst,
     & uursssst,uussssst,uurrrrtt,uurrrstt,uurrsstt,uurssstt,uusssstt,
     & uurrrttt,uurrsttt,uurssttt,uusssttt,uurrtttt,uurstttt,uusstttt,
     & uurttttt,uusttttt,uutttttt
      real vv,vvr,vvs,vvt,vvrr,vvrs,vvss,vvrt,vvst,vvtt,vvrrr,vvrrs,
     & vvrss,vvsss,vvrrt,vvrst,vvsst,vvrtt,vvstt,vvttt,vvrrrr,vvrrrs,
     & vvrrss,vvrsss,vvssss,vvrrrt,vvrrst,vvrsst,vvssst,vvrrtt,vvrstt,
     & vvsstt,vvrttt,vvsttt,vvtttt,vvrrrrr,vvrrrrs,vvrrrss,vvrrsss,
     & vvrssss,vvsssss,vvrrrrt,vvrrrst,vvrrsst,vvrssst,vvsssst,
     & vvrrrtt,vvrrstt,vvrsstt,vvssstt,vvrrttt,vvrsttt,vvssttt,
     & vvrtttt,vvstttt,vvttttt,vvrrrrrr,vvrrrrrs,vvrrrrss,vvrrrsss,
     & vvrrssss,vvrsssss,vvssssss,vvrrrrrt,vvrrrrst,vvrrrsst,vvrrssst,
     & vvrsssst,vvssssst,vvrrrrtt,vvrrrstt,vvrrsstt,vvrssstt,vvsssstt,
     & vvrrrttt,vvrrsttt,vvrssttt,vvsssttt,vvrrtttt,vvrstttt,vvsstttt,
     & vvrttttt,vvsttttt,vvtttttt
      real ww,wwr,wws,wwt,wwrr,wwrs,wwss,wwrt,wwst,wwtt,wwrrr,wwrrs,
     & wwrss,wwsss,wwrrt,wwrst,wwsst,wwrtt,wwstt,wwttt,wwrrrr,wwrrrs,
     & wwrrss,wwrsss,wwssss,wwrrrt,wwrrst,wwrsst,wwssst,wwrrtt,wwrstt,
     & wwsstt,wwrttt,wwsttt,wwtttt,wwrrrrr,wwrrrrs,wwrrrss,wwrrsss,
     & wwrssss,wwsssss,wwrrrrt,wwrrrst,wwrrsst,wwrssst,wwsssst,
     & wwrrrtt,wwrrstt,wwrsstt,wwssstt,wwrrttt,wwrsttt,wwssttt,
     & wwrtttt,wwstttt,wwttttt,wwrrrrrr,wwrrrrrs,wwrrrrss,wwrrrsss,
     & wwrrssss,wwrsssss,wwssssss,wwrrrrrt,wwrrrrst,wwrrrsst,wwrrssst,
     & wwrsssst,wwssssst,wwrrrrtt,wwrrrstt,wwrrsstt,wwrssstt,wwsssstt,
     & wwrrrttt,wwrrsttt,wwrssttt,wwsssttt,wwrrtttt,wwrstttt,wwsstttt,
     & wwrttttt,wwsttttt,wwtttttt
       real ajrx,ajrxr,ajrxs,ajrxt,ajrxrr,ajrxrs,ajrxss,ajrxrt,ajrxst,
     & ajrxtt,ajrxrrr,ajrxrrs,ajrxrss,ajrxsss,ajrxrrt,ajrxrst,ajrxsst,
     & ajrxrtt,ajrxstt,ajrxttt,ajrxrrrr,ajrxrrrs,ajrxrrss,ajrxrsss,
     & ajrxssss,ajrxrrrt,ajrxrrst,ajrxrsst,ajrxssst,ajrxrrtt,ajrxrstt,
     & ajrxsstt,ajrxrttt,ajrxsttt,ajrxtttt,ajrxrrrrr,ajrxrrrrs,
     & ajrxrrrss,ajrxrrsss,ajrxrssss,ajrxsssss,ajrxrrrrt,ajrxrrrst,
     & ajrxrrsst,ajrxrssst,ajrxsssst,ajrxrrrtt,ajrxrrstt,ajrxrsstt,
     & ajrxssstt,ajrxrrttt,ajrxrsttt,ajrxssttt,ajrxrtttt,ajrxstttt,
     & ajrxttttt,ajrxrrrrrr,ajrxrrrrrs,ajrxrrrrss,ajrxrrrsss,
     & ajrxrrssss,ajrxrsssss,ajrxssssss,ajrxrrrrrt,ajrxrrrrst,
     & ajrxrrrsst,ajrxrrssst,ajrxrsssst,ajrxssssst,ajrxrrrrtt,
     & ajrxrrrstt,ajrxrrsstt,ajrxrssstt,ajrxsssstt,ajrxrrrttt,
     & ajrxrrsttt,ajrxrssttt,ajrxsssttt,ajrxrrtttt,ajrxrstttt,
     & ajrxsstttt,ajrxrttttt,ajrxsttttt,ajrxtttttt
       real ajsx,ajsxr,ajsxs,ajsxt,ajsxrr,ajsxrs,ajsxss,ajsxrt,ajsxst,
     & ajsxtt,ajsxrrr,ajsxrrs,ajsxrss,ajsxsss,ajsxrrt,ajsxrst,ajsxsst,
     & ajsxrtt,ajsxstt,ajsxttt,ajsxrrrr,ajsxrrrs,ajsxrrss,ajsxrsss,
     & ajsxssss,ajsxrrrt,ajsxrrst,ajsxrsst,ajsxssst,ajsxrrtt,ajsxrstt,
     & ajsxsstt,ajsxrttt,ajsxsttt,ajsxtttt,ajsxrrrrr,ajsxrrrrs,
     & ajsxrrrss,ajsxrrsss,ajsxrssss,ajsxsssss,ajsxrrrrt,ajsxrrrst,
     & ajsxrrsst,ajsxrssst,ajsxsssst,ajsxrrrtt,ajsxrrstt,ajsxrsstt,
     & ajsxssstt,ajsxrrttt,ajsxrsttt,ajsxssttt,ajsxrtttt,ajsxstttt,
     & ajsxttttt,ajsxrrrrrr,ajsxrrrrrs,ajsxrrrrss,ajsxrrrsss,
     & ajsxrrssss,ajsxrsssss,ajsxssssss,ajsxrrrrrt,ajsxrrrrst,
     & ajsxrrrsst,ajsxrrssst,ajsxrsssst,ajsxssssst,ajsxrrrrtt,
     & ajsxrrrstt,ajsxrrsstt,ajsxrssstt,ajsxsssstt,ajsxrrrttt,
     & ajsxrrsttt,ajsxrssttt,ajsxsssttt,ajsxrrtttt,ajsxrstttt,
     & ajsxsstttt,ajsxrttttt,ajsxsttttt,ajsxtttttt
       real ajry,ajryr,ajrys,ajryt,ajryrr,ajryrs,ajryss,ajryrt,ajryst,
     & ajrytt,ajryrrr,ajryrrs,ajryrss,ajrysss,ajryrrt,ajryrst,ajrysst,
     & ajryrtt,ajrystt,ajryttt,ajryrrrr,ajryrrrs,ajryrrss,ajryrsss,
     & ajryssss,ajryrrrt,ajryrrst,ajryrsst,ajryssst,ajryrrtt,ajryrstt,
     & ajrysstt,ajryrttt,ajrysttt,ajrytttt,ajryrrrrr,ajryrrrrs,
     & ajryrrrss,ajryrrsss,ajryrssss,ajrysssss,ajryrrrrt,ajryrrrst,
     & ajryrrsst,ajryrssst,ajrysssst,ajryrrrtt,ajryrrstt,ajryrsstt,
     & ajryssstt,ajryrrttt,ajryrsttt,ajryssttt,ajryrtttt,ajrystttt,
     & ajryttttt,ajryrrrrrr,ajryrrrrrs,ajryrrrrss,ajryrrrsss,
     & ajryrrssss,ajryrsssss,ajryssssss,ajryrrrrrt,ajryrrrrst,
     & ajryrrrsst,ajryrrssst,ajryrsssst,ajryssssst,ajryrrrrtt,
     & ajryrrrstt,ajryrrsstt,ajryrssstt,ajrysssstt,ajryrrrttt,
     & ajryrrsttt,ajryrssttt,ajrysssttt,ajryrrtttt,ajryrstttt,
     & ajrysstttt,ajryrttttt,ajrysttttt,ajrytttttt
       real ajsy,ajsyr,ajsys,ajsyt,ajsyrr,ajsyrs,ajsyss,ajsyrt,ajsyst,
     & ajsytt,ajsyrrr,ajsyrrs,ajsyrss,ajsysss,ajsyrrt,ajsyrst,ajsysst,
     & ajsyrtt,ajsystt,ajsyttt,ajsyrrrr,ajsyrrrs,ajsyrrss,ajsyrsss,
     & ajsyssss,ajsyrrrt,ajsyrrst,ajsyrsst,ajsyssst,ajsyrrtt,ajsyrstt,
     & ajsysstt,ajsyrttt,ajsysttt,ajsytttt,ajsyrrrrr,ajsyrrrrs,
     & ajsyrrrss,ajsyrrsss,ajsyrssss,ajsysssss,ajsyrrrrt,ajsyrrrst,
     & ajsyrrsst,ajsyrssst,ajsysssst,ajsyrrrtt,ajsyrrstt,ajsyrsstt,
     & ajsyssstt,ajsyrrttt,ajsyrsttt,ajsyssttt,ajsyrtttt,ajsystttt,
     & ajsyttttt,ajsyrrrrrr,ajsyrrrrrs,ajsyrrrrss,ajsyrrrsss,
     & ajsyrrssss,ajsyrsssss,ajsyssssss,ajsyrrrrrt,ajsyrrrrst,
     & ajsyrrrsst,ajsyrrssst,ajsyrsssst,ajsyssssst,ajsyrrrrtt,
     & ajsyrrrstt,ajsyrrsstt,ajsyrssstt,ajsysssstt,ajsyrrrttt,
     & ajsyrrsttt,ajsyrssttt,ajsysssttt,ajsyrrtttt,ajsyrstttt,
     & ajsysstttt,ajsyrttttt,ajsysttttt,ajsytttttt
       real ajrxx,ajrxy,ajrxz,ajrxxx,ajrxxy,ajrxyy,ajrxxz,ajrxyz,
     & ajrxzz,ajrxxxx,ajrxxxy,ajrxxyy,ajrxyyy,ajrxxxz,ajrxxyz,ajrxyyz,
     & ajrxxzz,ajrxyzz,ajrxzzz,ajrxxxxx,ajrxxxxy,ajrxxxyy,ajrxxyyy,
     & ajrxyyyy,ajrxxxxz,ajrxxxyz,ajrxxyyz,ajrxyyyz,ajrxxxzz,ajrxxyzz,
     & ajrxyyzz,ajrxxzzz,ajrxyzzz,ajrxzzzz,ajrxxxxxx,ajrxxxxxy,
     & ajrxxxxyy,ajrxxxyyy,ajrxxyyyy,ajrxyyyyy,ajrxxxxxz,ajrxxxxyz,
     & ajrxxxyyz,ajrxxyyyz,ajrxyyyyz,ajrxxxxzz,ajrxxxyzz,ajrxxyyzz,
     & ajrxyyyzz,ajrxxxzzz,ajrxxyzzz,ajrxyyzzz,ajrxxzzzz,ajrxyzzzz,
     & ajrxzzzzz,ajrxxxxxxx,ajrxxxxxxy,ajrxxxxxyy,ajrxxxxyyy,
     & ajrxxxyyyy,ajrxxyyyyy,ajrxyyyyyy,ajrxxxxxxz,ajrxxxxxyz,
     & ajrxxxxyyz,ajrxxxyyyz,ajrxxyyyyz,ajrxyyyyyz,ajrxxxxxzz,
     & ajrxxxxyzz,ajrxxxyyzz,ajrxxyyyzz,ajrxyyyyzz,ajrxxxxzzz,
     & ajrxxxyzzz,ajrxxyyzzz,ajrxyyyzzz,ajrxxxzzzz,ajrxxyzzzz,
     & ajrxyyzzzz,ajrxxzzzzz,ajrxyzzzzz,ajrxzzzzzz
       real ajsxx,ajsxy,ajsxz,ajsxxx,ajsxxy,ajsxyy,ajsxxz,ajsxyz,
     & ajsxzz,ajsxxxx,ajsxxxy,ajsxxyy,ajsxyyy,ajsxxxz,ajsxxyz,ajsxyyz,
     & ajsxxzz,ajsxyzz,ajsxzzz,ajsxxxxx,ajsxxxxy,ajsxxxyy,ajsxxyyy,
     & ajsxyyyy,ajsxxxxz,ajsxxxyz,ajsxxyyz,ajsxyyyz,ajsxxxzz,ajsxxyzz,
     & ajsxyyzz,ajsxxzzz,ajsxyzzz,ajsxzzzz,ajsxxxxxx,ajsxxxxxy,
     & ajsxxxxyy,ajsxxxyyy,ajsxxyyyy,ajsxyyyyy,ajsxxxxxz,ajsxxxxyz,
     & ajsxxxyyz,ajsxxyyyz,ajsxyyyyz,ajsxxxxzz,ajsxxxyzz,ajsxxyyzz,
     & ajsxyyyzz,ajsxxxzzz,ajsxxyzzz,ajsxyyzzz,ajsxxzzzz,ajsxyzzzz,
     & ajsxzzzzz,ajsxxxxxxx,ajsxxxxxxy,ajsxxxxxyy,ajsxxxxyyy,
     & ajsxxxyyyy,ajsxxyyyyy,ajsxyyyyyy,ajsxxxxxxz,ajsxxxxxyz,
     & ajsxxxxyyz,ajsxxxyyyz,ajsxxyyyyz,ajsxyyyyyz,ajsxxxxxzz,
     & ajsxxxxyzz,ajsxxxyyzz,ajsxxyyyzz,ajsxyyyyzz,ajsxxxxzzz,
     & ajsxxxyzzz,ajsxxyyzzz,ajsxyyyzzz,ajsxxxzzzz,ajsxxyzzzz,
     & ajsxyyzzzz,ajsxxzzzzz,ajsxyzzzzz,ajsxzzzzzz
       real ajryx,ajryy,ajryz,ajryxx,ajryxy,ajryyy,ajryxz,ajryyz,
     & ajryzz,ajryxxx,ajryxxy,ajryxyy,ajryyyy,ajryxxz,ajryxyz,ajryyyz,
     & ajryxzz,ajryyzz,ajryzzz,ajryxxxx,ajryxxxy,ajryxxyy,ajryxyyy,
     & ajryyyyy,ajryxxxz,ajryxxyz,ajryxyyz,ajryyyyz,ajryxxzz,ajryxyzz,
     & ajryyyzz,ajryxzzz,ajryyzzz,ajryzzzz,ajryxxxxx,ajryxxxxy,
     & ajryxxxyy,ajryxxyyy,ajryxyyyy,ajryyyyyy,ajryxxxxz,ajryxxxyz,
     & ajryxxyyz,ajryxyyyz,ajryyyyyz,ajryxxxzz,ajryxxyzz,ajryxyyzz,
     & ajryyyyzz,ajryxxzzz,ajryxyzzz,ajryyyzzz,ajryxzzzz,ajryyzzzz,
     & ajryzzzzz,ajryxxxxxx,ajryxxxxxy,ajryxxxxyy,ajryxxxyyy,
     & ajryxxyyyy,ajryxyyyyy,ajryyyyyyy,ajryxxxxxz,ajryxxxxyz,
     & ajryxxxyyz,ajryxxyyyz,ajryxyyyyz,ajryyyyyyz,ajryxxxxzz,
     & ajryxxxyzz,ajryxxyyzz,ajryxyyyzz,ajryyyyyzz,ajryxxxzzz,
     & ajryxxyzzz,ajryxyyzzz,ajryyyyzzz,ajryxxzzzz,ajryxyzzzz,
     & ajryyyzzzz,ajryxzzzzz,ajryyzzzzz,ajryzzzzzz
       real ajsyx,ajsyy,ajsyz,ajsyxx,ajsyxy,ajsyyy,ajsyxz,ajsyyz,
     & ajsyzz,ajsyxxx,ajsyxxy,ajsyxyy,ajsyyyy,ajsyxxz,ajsyxyz,ajsyyyz,
     & ajsyxzz,ajsyyzz,ajsyzzz,ajsyxxxx,ajsyxxxy,ajsyxxyy,ajsyxyyy,
     & ajsyyyyy,ajsyxxxz,ajsyxxyz,ajsyxyyz,ajsyyyyz,ajsyxxzz,ajsyxyzz,
     & ajsyyyzz,ajsyxzzz,ajsyyzzz,ajsyzzzz,ajsyxxxxx,ajsyxxxxy,
     & ajsyxxxyy,ajsyxxyyy,ajsyxyyyy,ajsyyyyyy,ajsyxxxxz,ajsyxxxyz,
     & ajsyxxyyz,ajsyxyyyz,ajsyyyyyz,ajsyxxxzz,ajsyxxyzz,ajsyxyyzz,
     & ajsyyyyzz,ajsyxxzzz,ajsyxyzzz,ajsyyyzzz,ajsyxzzzz,ajsyyzzzz,
     & ajsyzzzzz,ajsyxxxxxx,ajsyxxxxxy,ajsyxxxxyy,ajsyxxxyyy,
     & ajsyxxyyyy,ajsyxyyyyy,ajsyyyyyyy,ajsyxxxxxz,ajsyxxxxyz,
     & ajsyxxxyyz,ajsyxxyyyz,ajsyxyyyyz,ajsyyyyyyz,ajsyxxxxzz,
     & ajsyxxxyzz,ajsyxxyyzz,ajsyxyyyzz,ajsyyyyyzz,ajsyxxxzzz,
     & ajsyxxyzzz,ajsyxyyzzz,ajsyyyyzzz,ajsyxxzzzz,ajsyxyzzzz,
     & ajsyyyzzzz,ajsyxzzzzz,ajsyyzzzzz,ajsyzzzzzz
       real ajrz,ajrzr,ajrzs,ajrzt,ajrzrr,ajrzrs,ajrzss,ajrzrt,ajrzst,
     & ajrztt,ajrzrrr,ajrzrrs,ajrzrss,ajrzsss,ajrzrrt,ajrzrst,ajrzsst,
     & ajrzrtt,ajrzstt,ajrzttt,ajrzrrrr,ajrzrrrs,ajrzrrss,ajrzrsss,
     & ajrzssss,ajrzrrrt,ajrzrrst,ajrzrsst,ajrzssst,ajrzrrtt,ajrzrstt,
     & ajrzsstt,ajrzrttt,ajrzsttt,ajrztttt,ajrzrrrrr,ajrzrrrrs,
     & ajrzrrrss,ajrzrrsss,ajrzrssss,ajrzsssss,ajrzrrrrt,ajrzrrrst,
     & ajrzrrsst,ajrzrssst,ajrzsssst,ajrzrrrtt,ajrzrrstt,ajrzrsstt,
     & ajrzssstt,ajrzrrttt,ajrzrsttt,ajrzssttt,ajrzrtttt,ajrzstttt,
     & ajrzttttt,ajrzrrrrrr,ajrzrrrrrs,ajrzrrrrss,ajrzrrrsss,
     & ajrzrrssss,ajrzrsssss,ajrzssssss,ajrzrrrrrt,ajrzrrrrst,
     & ajrzrrrsst,ajrzrrssst,ajrzrsssst,ajrzssssst,ajrzrrrrtt,
     & ajrzrrrstt,ajrzrrsstt,ajrzrssstt,ajrzsssstt,ajrzrrrttt,
     & ajrzrrsttt,ajrzrssttt,ajrzsssttt,ajrzrrtttt,ajrzrstttt,
     & ajrzsstttt,ajrzrttttt,ajrzsttttt,ajrztttttt
       real ajsz,ajszr,ajszs,ajszt,ajszrr,ajszrs,ajszss,ajszrt,ajszst,
     & ajsztt,ajszrrr,ajszrrs,ajszrss,ajszsss,ajszrrt,ajszrst,ajszsst,
     & ajszrtt,ajszstt,ajszttt,ajszrrrr,ajszrrrs,ajszrrss,ajszrsss,
     & ajszssss,ajszrrrt,ajszrrst,ajszrsst,ajszssst,ajszrrtt,ajszrstt,
     & ajszsstt,ajszrttt,ajszsttt,ajsztttt,ajszrrrrr,ajszrrrrs,
     & ajszrrrss,ajszrrsss,ajszrssss,ajszsssss,ajszrrrrt,ajszrrrst,
     & ajszrrsst,ajszrssst,ajszsssst,ajszrrrtt,ajszrrstt,ajszrsstt,
     & ajszssstt,ajszrrttt,ajszrsttt,ajszssttt,ajszrtttt,ajszstttt,
     & ajszttttt,ajszrrrrrr,ajszrrrrrs,ajszrrrrss,ajszrrrsss,
     & ajszrrssss,ajszrsssss,ajszssssss,ajszrrrrrt,ajszrrrrst,
     & ajszrrrsst,ajszrrssst,ajszrsssst,ajszssssst,ajszrrrrtt,
     & ajszrrrstt,ajszrrsstt,ajszrssstt,ajszsssstt,ajszrrrttt,
     & ajszrrsttt,ajszrssttt,ajszsssttt,ajszrrtttt,ajszrstttt,
     & ajszsstttt,ajszrttttt,ajszsttttt,ajsztttttt
       real ajtx,ajtxr,ajtxs,ajtxt,ajtxrr,ajtxrs,ajtxss,ajtxrt,ajtxst,
     & ajtxtt,ajtxrrr,ajtxrrs,ajtxrss,ajtxsss,ajtxrrt,ajtxrst,ajtxsst,
     & ajtxrtt,ajtxstt,ajtxttt,ajtxrrrr,ajtxrrrs,ajtxrrss,ajtxrsss,
     & ajtxssss,ajtxrrrt,ajtxrrst,ajtxrsst,ajtxssst,ajtxrrtt,ajtxrstt,
     & ajtxsstt,ajtxrttt,ajtxsttt,ajtxtttt,ajtxrrrrr,ajtxrrrrs,
     & ajtxrrrss,ajtxrrsss,ajtxrssss,ajtxsssss,ajtxrrrrt,ajtxrrrst,
     & ajtxrrsst,ajtxrssst,ajtxsssst,ajtxrrrtt,ajtxrrstt,ajtxrsstt,
     & ajtxssstt,ajtxrrttt,ajtxrsttt,ajtxssttt,ajtxrtttt,ajtxstttt,
     & ajtxttttt,ajtxrrrrrr,ajtxrrrrrs,ajtxrrrrss,ajtxrrrsss,
     & ajtxrrssss,ajtxrsssss,ajtxssssss,ajtxrrrrrt,ajtxrrrrst,
     & ajtxrrrsst,ajtxrrssst,ajtxrsssst,ajtxssssst,ajtxrrrrtt,
     & ajtxrrrstt,ajtxrrsstt,ajtxrssstt,ajtxsssstt,ajtxrrrttt,
     & ajtxrrsttt,ajtxrssttt,ajtxsssttt,ajtxrrtttt,ajtxrstttt,
     & ajtxsstttt,ajtxrttttt,ajtxsttttt,ajtxtttttt
       real ajty,ajtyr,ajtys,ajtyt,ajtyrr,ajtyrs,ajtyss,ajtyrt,ajtyst,
     & ajtytt,ajtyrrr,ajtyrrs,ajtyrss,ajtysss,ajtyrrt,ajtyrst,ajtysst,
     & ajtyrtt,ajtystt,ajtyttt,ajtyrrrr,ajtyrrrs,ajtyrrss,ajtyrsss,
     & ajtyssss,ajtyrrrt,ajtyrrst,ajtyrsst,ajtyssst,ajtyrrtt,ajtyrstt,
     & ajtysstt,ajtyrttt,ajtysttt,ajtytttt,ajtyrrrrr,ajtyrrrrs,
     & ajtyrrrss,ajtyrrsss,ajtyrssss,ajtysssss,ajtyrrrrt,ajtyrrrst,
     & ajtyrrsst,ajtyrssst,ajtysssst,ajtyrrrtt,ajtyrrstt,ajtyrsstt,
     & ajtyssstt,ajtyrrttt,ajtyrsttt,ajtyssttt,ajtyrtttt,ajtystttt,
     & ajtyttttt,ajtyrrrrrr,ajtyrrrrrs,ajtyrrrrss,ajtyrrrsss,
     & ajtyrrssss,ajtyrsssss,ajtyssssss,ajtyrrrrrt,ajtyrrrrst,
     & ajtyrrrsst,ajtyrrssst,ajtyrsssst,ajtyssssst,ajtyrrrrtt,
     & ajtyrrrstt,ajtyrrsstt,ajtyrssstt,ajtysssstt,ajtyrrrttt,
     & ajtyrrsttt,ajtyrssttt,ajtysssttt,ajtyrrtttt,ajtyrstttt,
     & ajtysstttt,ajtyrttttt,ajtysttttt,ajtytttttt
       real ajtz,ajtzr,ajtzs,ajtzt,ajtzrr,ajtzrs,ajtzss,ajtzrt,ajtzst,
     & ajtztt,ajtzrrr,ajtzrrs,ajtzrss,ajtzsss,ajtzrrt,ajtzrst,ajtzsst,
     & ajtzrtt,ajtzstt,ajtzttt,ajtzrrrr,ajtzrrrs,ajtzrrss,ajtzrsss,
     & ajtzssss,ajtzrrrt,ajtzrrst,ajtzrsst,ajtzssst,ajtzrrtt,ajtzrstt,
     & ajtzsstt,ajtzrttt,ajtzsttt,ajtztttt,ajtzrrrrr,ajtzrrrrs,
     & ajtzrrrss,ajtzrrsss,ajtzrssss,ajtzsssss,ajtzrrrrt,ajtzrrrst,
     & ajtzrrsst,ajtzrssst,ajtzsssst,ajtzrrrtt,ajtzrrstt,ajtzrsstt,
     & ajtzssstt,ajtzrrttt,ajtzrsttt,ajtzssttt,ajtzrtttt,ajtzstttt,
     & ajtzttttt,ajtzrrrrrr,ajtzrrrrrs,ajtzrrrrss,ajtzrrrsss,
     & ajtzrrssss,ajtzrsssss,ajtzssssss,ajtzrrrrrt,ajtzrrrrst,
     & ajtzrrrsst,ajtzrrssst,ajtzrsssst,ajtzssssst,ajtzrrrrtt,
     & ajtzrrrstt,ajtzrrsstt,ajtzrssstt,ajtzsssstt,ajtzrrrttt,
     & ajtzrrsttt,ajtzrssttt,ajtzsssttt,ajtzrrtttt,ajtzrstttt,
     & ajtzsstttt,ajtzrttttt,ajtzsttttt,ajtztttttt
       real ajrzx,ajrzy,ajrzz,ajrzxx,ajrzxy,ajrzyy,ajrzxz,ajrzyz,
     & ajrzzz,ajrzxxx,ajrzxxy,ajrzxyy,ajrzyyy,ajrzxxz,ajrzxyz,ajrzyyz,
     & ajrzxzz,ajrzyzz,ajrzzzz,ajrzxxxx,ajrzxxxy,ajrzxxyy,ajrzxyyy,
     & ajrzyyyy,ajrzxxxz,ajrzxxyz,ajrzxyyz,ajrzyyyz,ajrzxxzz,ajrzxyzz,
     & ajrzyyzz,ajrzxzzz,ajrzyzzz,ajrzzzzz,ajrzxxxxx,ajrzxxxxy,
     & ajrzxxxyy,ajrzxxyyy,ajrzxyyyy,ajrzyyyyy,ajrzxxxxz,ajrzxxxyz,
     & ajrzxxyyz,ajrzxyyyz,ajrzyyyyz,ajrzxxxzz,ajrzxxyzz,ajrzxyyzz,
     & ajrzyyyzz,ajrzxxzzz,ajrzxyzzz,ajrzyyzzz,ajrzxzzzz,ajrzyzzzz,
     & ajrzzzzzz,ajrzxxxxxx,ajrzxxxxxy,ajrzxxxxyy,ajrzxxxyyy,
     & ajrzxxyyyy,ajrzxyyyyy,ajrzyyyyyy,ajrzxxxxxz,ajrzxxxxyz,
     & ajrzxxxyyz,ajrzxxyyyz,ajrzxyyyyz,ajrzyyyyyz,ajrzxxxxzz,
     & ajrzxxxyzz,ajrzxxyyzz,ajrzxyyyzz,ajrzyyyyzz,ajrzxxxzzz,
     & ajrzxxyzzz,ajrzxyyzzz,ajrzyyyzzz,ajrzxxzzzz,ajrzxyzzzz,
     & ajrzyyzzzz,ajrzxzzzzz,ajrzyzzzzz,ajrzzzzzzz
       real ajszx,ajszy,ajszz,ajszxx,ajszxy,ajszyy,ajszxz,ajszyz,
     & ajszzz,ajszxxx,ajszxxy,ajszxyy,ajszyyy,ajszxxz,ajszxyz,ajszyyz,
     & ajszxzz,ajszyzz,ajszzzz,ajszxxxx,ajszxxxy,ajszxxyy,ajszxyyy,
     & ajszyyyy,ajszxxxz,ajszxxyz,ajszxyyz,ajszyyyz,ajszxxzz,ajszxyzz,
     & ajszyyzz,ajszxzzz,ajszyzzz,ajszzzzz,ajszxxxxx,ajszxxxxy,
     & ajszxxxyy,ajszxxyyy,ajszxyyyy,ajszyyyyy,ajszxxxxz,ajszxxxyz,
     & ajszxxyyz,ajszxyyyz,ajszyyyyz,ajszxxxzz,ajszxxyzz,ajszxyyzz,
     & ajszyyyzz,ajszxxzzz,ajszxyzzz,ajszyyzzz,ajszxzzzz,ajszyzzzz,
     & ajszzzzzz,ajszxxxxxx,ajszxxxxxy,ajszxxxxyy,ajszxxxyyy,
     & ajszxxyyyy,ajszxyyyyy,ajszyyyyyy,ajszxxxxxz,ajszxxxxyz,
     & ajszxxxyyz,ajszxxyyyz,ajszxyyyyz,ajszyyyyyz,ajszxxxxzz,
     & ajszxxxyzz,ajszxxyyzz,ajszxyyyzz,ajszyyyyzz,ajszxxxzzz,
     & ajszxxyzzz,ajszxyyzzz,ajszyyyzzz,ajszxxzzzz,ajszxyzzzz,
     & ajszyyzzzz,ajszxzzzzz,ajszyzzzzz,ajszzzzzzz
       real ajtxx,ajtxy,ajtxz,ajtxxx,ajtxxy,ajtxyy,ajtxxz,ajtxyz,
     & ajtxzz,ajtxxxx,ajtxxxy,ajtxxyy,ajtxyyy,ajtxxxz,ajtxxyz,ajtxyyz,
     & ajtxxzz,ajtxyzz,ajtxzzz,ajtxxxxx,ajtxxxxy,ajtxxxyy,ajtxxyyy,
     & ajtxyyyy,ajtxxxxz,ajtxxxyz,ajtxxyyz,ajtxyyyz,ajtxxxzz,ajtxxyzz,
     & ajtxyyzz,ajtxxzzz,ajtxyzzz,ajtxzzzz,ajtxxxxxx,ajtxxxxxy,
     & ajtxxxxyy,ajtxxxyyy,ajtxxyyyy,ajtxyyyyy,ajtxxxxxz,ajtxxxxyz,
     & ajtxxxyyz,ajtxxyyyz,ajtxyyyyz,ajtxxxxzz,ajtxxxyzz,ajtxxyyzz,
     & ajtxyyyzz,ajtxxxzzz,ajtxxyzzz,ajtxyyzzz,ajtxxzzzz,ajtxyzzzz,
     & ajtxzzzzz,ajtxxxxxxx,ajtxxxxxxy,ajtxxxxxyy,ajtxxxxyyy,
     & ajtxxxyyyy,ajtxxyyyyy,ajtxyyyyyy,ajtxxxxxxz,ajtxxxxxyz,
     & ajtxxxxyyz,ajtxxxyyyz,ajtxxyyyyz,ajtxyyyyyz,ajtxxxxxzz,
     & ajtxxxxyzz,ajtxxxyyzz,ajtxxyyyzz,ajtxyyyyzz,ajtxxxxzzz,
     & ajtxxxyzzz,ajtxxyyzzz,ajtxyyyzzz,ajtxxxzzzz,ajtxxyzzzz,
     & ajtxyyzzzz,ajtxxzzzzz,ajtxyzzzzz,ajtxzzzzzz
       real ajtyx,ajtyy,ajtyz,ajtyxx,ajtyxy,ajtyyy,ajtyxz,ajtyyz,
     & ajtyzz,ajtyxxx,ajtyxxy,ajtyxyy,ajtyyyy,ajtyxxz,ajtyxyz,ajtyyyz,
     & ajtyxzz,ajtyyzz,ajtyzzz,ajtyxxxx,ajtyxxxy,ajtyxxyy,ajtyxyyy,
     & ajtyyyyy,ajtyxxxz,ajtyxxyz,ajtyxyyz,ajtyyyyz,ajtyxxzz,ajtyxyzz,
     & ajtyyyzz,ajtyxzzz,ajtyyzzz,ajtyzzzz,ajtyxxxxx,ajtyxxxxy,
     & ajtyxxxyy,ajtyxxyyy,ajtyxyyyy,ajtyyyyyy,ajtyxxxxz,ajtyxxxyz,
     & ajtyxxyyz,ajtyxyyyz,ajtyyyyyz,ajtyxxxzz,ajtyxxyzz,ajtyxyyzz,
     & ajtyyyyzz,ajtyxxzzz,ajtyxyzzz,ajtyyyzzz,ajtyxzzzz,ajtyyzzzz,
     & ajtyzzzzz,ajtyxxxxxx,ajtyxxxxxy,ajtyxxxxyy,ajtyxxxyyy,
     & ajtyxxyyyy,ajtyxyyyyy,ajtyyyyyyy,ajtyxxxxxz,ajtyxxxxyz,
     & ajtyxxxyyz,ajtyxxyyyz,ajtyxyyyyz,ajtyyyyyyz,ajtyxxxxzz,
     & ajtyxxxyzz,ajtyxxyyzz,ajtyxyyyzz,ajtyyyyyzz,ajtyxxxzzz,
     & ajtyxxyzzz,ajtyxyyzzz,ajtyyyyzzz,ajtyxxzzzz,ajtyxyzzzz,
     & ajtyyyzzzz,ajtyxzzzzz,ajtyyzzzzz,ajtyzzzzzz
       real ajtzx,ajtzy,ajtzz,ajtzxx,ajtzxy,ajtzyy,ajtzxz,ajtzyz,
     & ajtzzz,ajtzxxx,ajtzxxy,ajtzxyy,ajtzyyy,ajtzxxz,ajtzxyz,ajtzyyz,
     & ajtzxzz,ajtzyzz,ajtzzzz,ajtzxxxx,ajtzxxxy,ajtzxxyy,ajtzxyyy,
     & ajtzyyyy,ajtzxxxz,ajtzxxyz,ajtzxyyz,ajtzyyyz,ajtzxxzz,ajtzxyzz,
     & ajtzyyzz,ajtzxzzz,ajtzyzzz,ajtzzzzz,ajtzxxxxx,ajtzxxxxy,
     & ajtzxxxyy,ajtzxxyyy,ajtzxyyyy,ajtzyyyyy,ajtzxxxxz,ajtzxxxyz,
     & ajtzxxyyz,ajtzxyyyz,ajtzyyyyz,ajtzxxxzz,ajtzxxyzz,ajtzxyyzz,
     & ajtzyyyzz,ajtzxxzzz,ajtzxyzzz,ajtzyyzzz,ajtzxzzzz,ajtzyzzzz,
     & ajtzzzzzz,ajtzxxxxxx,ajtzxxxxxy,ajtzxxxxyy,ajtzxxxyyy,
     & ajtzxxyyyy,ajtzxyyyyy,ajtzyyyyyy,ajtzxxxxxz,ajtzxxxxyz,
     & ajtzxxxyyz,ajtzxxyyyz,ajtzxyyyyz,ajtzyyyyyz,ajtzxxxxzz,
     & ajtzxxxyzz,ajtzxxyyzz,ajtzxyyyzz,ajtzyyyyzz,ajtzxxxzzz,
     & ajtzxxyzzz,ajtzxyyzzz,ajtzyyyzzz,ajtzxxzzzz,ajtzxyzzzz,
     & ajtzyyzzzz,ajtzxzzzzz,ajtzyzzzzz,ajtzzzzzzz


      ierr=0


      option = ipar(0)
      ex     = ipar(1)
      ey     = ipar(2)
      ez     = ipar(3)
      hx     = ipar(4)
      hy     = ipar(5)
      hz     = ipar(6)
      debug  = ipar(7)
      myid   = ipar(8)
      orderOfAccuracy=ipar(9)
      grid   = ipar(10)
      gridType=ipar(11)

      omega = rpar(0)  ! for potential future use
      dt    = rpar(1)
      t     = rpar(2)
      dx(0) = rpar(3)
      dx(1) = rpar(4)
      dx(2) = rpar(5)
      dr(0) = rpar(6)
      dr(1) = rpar(7)
      dr(2) = rpar(8)

      e1 = ex
      e2 = ey
      e3 = e2 + 1 ! hz or ez

      ! we just need to include 1 ghost point since we change only the interpolation point
      ! next to the interior point. 
      ! numGhost=1   ! orderOfAccuracy/2

      numGhost=orderOfAccuracy/2

      numGhost3=numGhost
      w1=numGhost
      w2=numGhost
      w3=numGhost
      if( nd.eq.2 )then
        numGhost3=0
        w3=0
      end if

      !  *** add ghost so we find interp pts **
      n1a=gridIndexRange(0,0)-numGhost
      n1b=gridIndexRange(1,0)+numGhost
      n2a=gridIndexRange(0,1)-numGhost
      n2b=gridIndexRange(1,1)+numGhost
      n3a=gridIndexRange(0,2)-numGhost3
      n3b=gridIndexRange(1,2)+numGhost3

      if( .false. .or. (t.le. dt .and. myid.eq.0) )then
        write(*,'(" mxProjectInterp: init : t,dt = ",2(e10.2,1x))') t,
     & dt
        if( .true. .or. debug.gt.3 )then
          write(*,'(" grid=",i4," orderOfAccuracy=",i4,", debug=",i6)')
     &  grid,orderOfAccuracy,debug
          write(*,'(" gridIndexRange= ",3(2i5,1x))') gridIndexRange
          write(*,'(" mxProjectInterp: ex,ey,hz = ",3i2)') ex,ey,hz
          write(*,'(" mxProjectInterp: n1a,n1b,n2a,n2b,n3a,n3b = ",3(
     & 2i5,1x))') n1a,n1b,n2a,n2b,n3a,n3b
        end if
      end if

! #defineMacro divr(i1,i2,i3) ( (u(i1+1,i2,i3,ex)-u(i1-1,i2,i3,ex))/(2.*dx(0)) +!                               (u(i1,i2+1,i3,ey)-u(i1,i2-1,i3,ey))/(2.*dx(1)) )

      if( nd.eq.2 .and. gridType.eq.rectangular .and. 
     & orderOfAccuracy.eq.2 )then

         do i3=n3a,n3b
         do i2=n2a,n2b
         do i1=n1a,n1b
           if( mask(i1,i2,i3).lt.0 )then
             ! interp. pt. found -- find a nearby discretization pt
             ! write(*,'(" projectInterp: interp found: i1,i2=",2i4)') i1,i2
             ! *********** fix me: define bounds in terms of n1a,... ********
             do j3=i3-w3,i3+w3
             do j2=i2-w2,i2+w2,w2
             do j1=i1-w1,i1+w1,w1
              if( iabs(j1-i1)+iabs(j2-i2).eq.numGhost .and. (mask(j1,
     & j2,j3).gt.0) )then
               ! Enforce div(E)=0 at point jv by adjusting point iv
                    ux = (-u(j1-1,j2,j3,ex)+u(j1+1,j2,j3,ex))/(2.*dx(0)
     & )
                    vy = (-u(j1,j2-1,j3,ey)+u(j1,j2+1,j3,ey))/(2.*dx(1)
     & )
                  res = ux+ vy
               !  uur = (-u(j1-1,j2,j3,ex)+u(j1+1,j2,j3,ex))/(2.*dr(0))
               !  uus = (-u(j1,j2-1,j3,ex)+u(j1,j2+1,j3,ex))/(2.*dr(1))
               ! ux = ajrx*uur+ajsx*uus
               ! vy = ajry*vvr+ajsy*vvs
               ! (ax,ay) :  coefficients of Ex(iv) and Ey(iv) in div(E)(jv)
                 ax = (i1-j1)/(2.*dx(0))
                 ay = (i2-j2)/(2.*dx(1))
                 aSq = ax**2 + ay**2
               ! adjust the interpolation pt: 
               u(i1,i2,i3,ex) = u(i1,i2,i3,ex) - res*ax/aSq   ! project Ev(jv) so div(E)=0
               u(i1,i2,i3,ey) = u(i1,i2,i3,ey) - res*ay/aSq
               ! check ...
               if( .true. .or. debug.gt.2 )then
                      ux = (-u(j1-1,j2,j3,ex)+u(j1+1,j2,j3,ex))/(2.*dx(
     & 0))
                      vy = (-u(j1,j2-1,j3,ey)+u(j1,j2+1,j3,ey))/(2.*dx(
     & 1))
                    res = ux+ vy
                 write(*,'(" projectInterp: interp: grid=",i4," i1,
     & i2=",2i4,", interior:j1,j2=",2i4," new div=",e10.2)') grid,i1,
     & i2,j1,j2,res
               end if
             end if
             end do
             end do
             end do
           end if
         end do
         end do
         end do

      else if( nd.eq.2 .and. gridType.eq.curvilinear .and. 
     & orderOfAccuracy.eq.2 )then

         do i3=n3a,n3b
         do i2=n2a,n2b
         do i1=n1a,n1b
           if( mask(i1,i2,i3).lt.0 )then
             ! interp. pt. found -- find a nearby discretization pt
             ! write(*,'(" projectInterp: interp found: i1,i2=",2i4)') i1,i2
             ! *********** fix me: define bounds in terms of n1a,... ********
             do j3=i3-w3,i3+w3
             do j2=i2-w2,i2+w2,w2
             do j1=i1-w1,i1+w1,w1
              if( iabs(j1-i1)+iabs(j2-i2).eq.numGhost .and. (mask(j1,
     & j2,j3).gt.0) )then
               ! Enforce div(E)=0 at point jv by adjusting point iv
                  ! this next call will define the jacobian and its derivatives (parameteric and spatial)
                  ajrx = rsxy(j1,j2,j3,0,0)
                  ajsx = rsxy(j1,j2,j3,1,0)
                  ajry = rsxy(j1,j2,j3,0,1)
                  ajsy = rsxy(j1,j2,j3,1,1)
                   uu = u(j1,j2,j3,ex)
                   uur = (-u(j1-1,j2,j3,ex)+u(j1+1,j2,j3,ex))/(2.*dr(0)
     & )
                   uus = (-u(j1,j2-1,j3,ex)+u(j1,j2+1,j3,ex))/(2.*dr(1)
     & )
                   vv = u(j1,j2,j3,ey)
                   vvr = (-u(j1-1,j2,j3,ey)+u(j1+1,j2,j3,ey))/(2.*dr(0)
     & )
                   vvs = (-u(j1,j2-1,j3,ey)+u(j1,j2+1,j3,ey))/(2.*dr(1)
     & )
                    ux = ajrx*uur+ajsx*uus
                    vy = ajry*vvr+ajsy*vvs
                  res = ux+ vy
               !  uur = (-u(j1-1,j2,j3,ex)+u(j1+1,j2,j3,ex))/(2.*dr(0))
               !  uus = (-u(j1,j2-1,j3,ex)+u(j1,j2+1,j3,ex))/(2.*dr(1))
               ! ux = ajrx*uur+ajsx*uus
               ! vy = ajry*vvr+ajsy*vvs
               ! (ax,ay) :  coefficients of Ex(iv) and Ey(iv) in div(E)(jv)
                 ar = (i1-j1)/(2.*dr(0))
                 as = (i2-j2)/(2.*dr(1))
                 ax = ajrx*ar+ajsx*as
                 ay = ajry*ar+ajsy*as
                 aSq = ax**2 + ay**2
               ! adjust the interpolation pt: 
               u(i1,i2,i3,ex) = u(i1,i2,i3,ex) - res*ax/aSq   ! project Ev(jv) so div(E)=0
               u(i1,i2,i3,ey) = u(i1,i2,i3,ey) - res*ay/aSq
               ! check ...
               if( .true. .or. debug.gt.2 )then
                    ! this next call will define the jacobian and its derivatives (parameteric and spatial)
                    ajrx = rsxy(j1,j2,j3,0,0)
                    ajsx = rsxy(j1,j2,j3,1,0)
                    ajry = rsxy(j1,j2,j3,0,1)
                    ajsy = rsxy(j1,j2,j3,1,1)
                     uu = u(j1,j2,j3,ex)
                     uur = (-u(j1-1,j2,j3,ex)+u(j1+1,j2,j3,ex))/(2.*dr(
     & 0))
                     uus = (-u(j1,j2-1,j3,ex)+u(j1,j2+1,j3,ex))/(2.*dr(
     & 1))
                     vv = u(j1,j2,j3,ey)
                     vvr = (-u(j1-1,j2,j3,ey)+u(j1+1,j2,j3,ey))/(2.*dr(
     & 0))
                     vvs = (-u(j1,j2-1,j3,ey)+u(j1,j2+1,j3,ey))/(2.*dr(
     & 1))
                      ux = ajrx*uur+ajsx*uus
                      vy = ajry*vvr+ajsy*vvs
                    res = ux+ vy
                 write(*,'(" projectInterp: interp: grid=",i4," i1,
     & i2=",2i4,", interior:j1,j2=",2i4," new div=",e10.2)') grid,i1,
     & i2,j1,j2,res
               end if
             end if
             end do
             end do
             end do
           end if
         end do
         end do
         end do

      else if( nd.eq.2 .and. gridType.eq.rectangular .and. 
     & orderOfAccuracy.eq.4 )then

         do i3=n3a,n3b
         do i2=n2a,n2b
         do i1=n1a,n1b
           if( mask(i1,i2,i3).lt.0 )then
             ! interp. pt. found -- find a nearby discretization pt
             ! write(*,'(" projectInterp: interp found: i1,i2=",2i4)') i1,i2
               ! only change interp points in the 2nd layer
             ! *********** fix me: define bounds in terms of n1a,... ********
             do j3=i3-w3,i3+w3
             do j2=i2-w2,i2+w2,w2
             do j1=i1-w1,i1+w1,w1
              if( iabs(j1-i1)+iabs(j2-i2).eq.numGhost .and. (
     & j1.ge.n1a.and.j1.le.n1b.and.j2.ge.n2a.and.j2.le.n2b.and.mask(
     & j1,j2,j3).gt.0.and.mask((i1+j1)/2,(i2+j2)/2,(i3+j3)/2).lt.0) )
     & then
               ! Enforce div(E)=0 at point jv by adjusting point iv
                    ux = (u(j1-2,j2,j3,ex)-8.*u(j1-1,j2,j3,ex)+8.*u(j1+
     & 1,j2,j3,ex)-u(j1+2,j2,j3,ex))/(12.*dx(0))
                    vy = (u(j1,j2-2,j3,ey)-8.*u(j1,j2-1,j3,ey)+8.*u(j1,
     & j2+1,j3,ey)-u(j1,j2+2,j3,ey))/(12.*dx(1))
                  res = ux+ vy
               !  uur = (-u(j1-1,j2,j3,ex)+u(j1+1,j2,j3,ex))/(2.*dr(0))
               !  uus = (-u(j1,j2-1,j3,ex)+u(j1,j2+1,j3,ex))/(2.*dr(1))
               ! ux = ajrx*uur+ajsx*uus
               ! vy = ajry*vvr+ajsy*vvs
               ! (ax,ay) :  coefficients of Ex(iv) and Ey(iv) in div(E)(jv)
                 !ax = (i1-j1)*8./(12.*dx(0))  ! first ghost line coeff
                 !ay = (i2-j2)*8./(12.*dx(1))
                 ax = -(i1-j1)*.5/(12.*dx(0))
                 ay = -(i2-j2)*.5/(12.*dx(1))
                 aSq = ax**2 + ay**2
               ! adjust the interpolation pt: 
               u(i1,i2,i3,ex) = u(i1,i2,i3,ex) - res*ax/aSq   ! project Ev(jv) so div(E)=0
               u(i1,i2,i3,ey) = u(i1,i2,i3,ey) - res*ay/aSq
               ! check ...
               if( .true. .or. debug.gt.2 )then
                      ux = (u(j1-2,j2,j3,ex)-8.*u(j1-1,j2,j3,ex)+8.*u(
     & j1+1,j2,j3,ex)-u(j1+2,j2,j3,ex))/(12.*dx(0))
                      vy = (u(j1,j2-2,j3,ey)-8.*u(j1,j2-1,j3,ey)+8.*u(
     & j1,j2+1,j3,ey)-u(j1,j2+2,j3,ey))/(12.*dx(1))
                    res = ux+ vy
                 write(*,'(" projectInterp: interp: grid=",i4," i1,
     & i2=",2i4,", interior:j1,j2=",2i4," new div=",e10.2)') grid,i1,
     & i2,j1,j2,res
               end if
             end if
             end do
             end do
             end do
           end if
         end do
         end do
         end do

      else if( nd.eq.2 .and. gridType.eq.curvilinear .and. 
     & orderOfAccuracy.eq.4 )then

         do i3=n3a,n3b
         do i2=n2a,n2b
         do i1=n1a,n1b
           if( mask(i1,i2,i3).lt.0 )then
             ! interp. pt. found -- find a nearby discretization pt
             ! write(*,'(" projectInterp: interp found: i1,i2=",2i4)') i1,i2
               ! only change interp points in the 2nd layer
             ! *********** fix me: define bounds in terms of n1a,... ********
             do j3=i3-w3,i3+w3
             do j2=i2-w2,i2+w2,w2
             do j1=i1-w1,i1+w1,w1
              if( iabs(j1-i1)+iabs(j2-i2).eq.numGhost .and. (
     & j1.ge.n1a.and.j1.le.n1b.and.j2.ge.n2a.and.j2.le.n2b.and.mask(
     & j1,j2,j3).gt.0.and.mask((i1+j1)/2,(i2+j2)/2,(i3+j3)/2).lt.0) )
     & then
               ! Enforce div(E)=0 at point jv by adjusting point iv
                  ! this next call will define the jacobian and its derivatives (parameteric and spatial)
                  ajrx = rsxy(j1,j2,j3,0,0)
                  ajsx = rsxy(j1,j2,j3,1,0)
                  ajry = rsxy(j1,j2,j3,0,1)
                  ajsy = rsxy(j1,j2,j3,1,1)
                   uu = u(j1,j2,j3,ex)
                   uur = (u(j1-2,j2,j3,ex)-8.*u(j1-1,j2,j3,ex)+8.*u(j1+
     & 1,j2,j3,ex)-u(j1+2,j2,j3,ex))/(12.*dr(0))
                   uus = (u(j1,j2-2,j3,ex)-8.*u(j1,j2-1,j3,ex)+8.*u(j1,
     & j2+1,j3,ex)-u(j1,j2+2,j3,ex))/(12.*dr(1))
                   vv = u(j1,j2,j3,ey)
                   vvr = (u(j1-2,j2,j3,ey)-8.*u(j1-1,j2,j3,ey)+8.*u(j1+
     & 1,j2,j3,ey)-u(j1+2,j2,j3,ey))/(12.*dr(0))
                   vvs = (u(j1,j2-2,j3,ey)-8.*u(j1,j2-1,j3,ey)+8.*u(j1,
     & j2+1,j3,ey)-u(j1,j2+2,j3,ey))/(12.*dr(1))
                    ux = ajrx*uur+ajsx*uus
                    vy = ajry*vvr+ajsy*vvs
                  res = ux+ vy
               !  uur = (-u(j1-1,j2,j3,ex)+u(j1+1,j2,j3,ex))/(2.*dr(0))
               !  uus = (-u(j1,j2-1,j3,ex)+u(j1,j2+1,j3,ex))/(2.*dr(1))
               ! ux = ajrx*uur+ajsx*uus
               ! vy = ajry*vvr+ajsy*vvs
               ! (ax,ay) :  coefficients of Ex(iv) and Ey(iv) in div(E)(jv)
                 !ar = (i1-j1)*8./(12.*dr(0))
                 !as = (i2-j2)*8./(12.*dr(1))
                 ar = -(i1-j1)*.5/(12.*dr(0))
                 as = -(i2-j2)*.5/(12.*dr(1))
                 ax = ajrx*ar+ajsx*as
                 ay = ajry*ar+ajsy*as
                 aSq = ax**2 + ay**2
               ! adjust the interpolation pt: 
               u(i1,i2,i3,ex) = u(i1,i2,i3,ex) - res*ax/aSq   ! project Ev(jv) so div(E)=0
               u(i1,i2,i3,ey) = u(i1,i2,i3,ey) - res*ay/aSq
               ! check ...
               if( .true. .or. debug.gt.2 )then
                    ! this next call will define the jacobian and its derivatives (parameteric and spatial)
                    ajrx = rsxy(j1,j2,j3,0,0)
                    ajsx = rsxy(j1,j2,j3,1,0)
                    ajry = rsxy(j1,j2,j3,0,1)
                    ajsy = rsxy(j1,j2,j3,1,1)
                     uu = u(j1,j2,j3,ex)
                     uur = (u(j1-2,j2,j3,ex)-8.*u(j1-1,j2,j3,ex)+8.*u(
     & j1+1,j2,j3,ex)-u(j1+2,j2,j3,ex))/(12.*dr(0))
                     uus = (u(j1,j2-2,j3,ex)-8.*u(j1,j2-1,j3,ex)+8.*u(
     & j1,j2+1,j3,ex)-u(j1,j2+2,j3,ex))/(12.*dr(1))
                     vv = u(j1,j2,j3,ey)
                     vvr = (u(j1-2,j2,j3,ey)-8.*u(j1-1,j2,j3,ey)+8.*u(
     & j1+1,j2,j3,ey)-u(j1+2,j2,j3,ey))/(12.*dr(0))
                     vvs = (u(j1,j2-2,j3,ey)-8.*u(j1,j2-1,j3,ey)+8.*u(
     & j1,j2+1,j3,ey)-u(j1,j2+2,j3,ey))/(12.*dr(1))
                      ux = ajrx*uur+ajsx*uus
                      vy = ajry*vvr+ajsy*vvs
                    res = ux+ vy
                 write(*,'(" projectInterp: interp: grid=",i4," i1,
     & i2=",2i4,", interior:j1,j2=",2i4," new div=",e10.2)') grid,i1,
     & i2,j1,j2,res
               end if
             end if
             end do
             end do
             end do
           end if
         end do
         end do
         end do

      else if( nd.eq.3 .and. gridType.eq.rectangular .and. 
     & orderOfAccuracy.eq.2 )then

         do i3=n3a,n3b
         do i2=n2a,n2b
         do i1=n1a,n1b
           if( mask(i1,i2,i3).lt.0 )then
             ! interp. pt. found -- find a nearby discretization pt
             ! write(*,'(" projectInterp: interp found: i1,i2=",2i4)') i1,i2
             ! *********** fix me: define bounds in terms of n1a,... ********
             do j3=i3-w3,i3+w3
             do j2=i2-w2,i2+w2,w2
             do j1=i1-w1,i1+w1,w1
              if( iabs(j1-i1)+iabs(j2-i2)+iabs(j3-i3).eq.numGhost 
     & .and. (mask(j1,j2,j3).gt.0) )then
               ! Enforce div(E)=0 at point jv by adjusting point iv
                    ux = (-u(j1-1,j2,j3,ex)+u(j1+1,j2,j3,ex))/(2.*dx(0)
     & )
                    vy = (-u(j1,j2-1,j3,ey)+u(j1,j2+1,j3,ey))/(2.*dx(1)
     & )
                  res = ux+ vy
               !  uur = (-u(j1-1,j2,j3,ex)+u(j1+1,j2,j3,ex))/(2.*dr(0))
               !  uus = (-u(j1,j2-1,j3,ex)+u(j1,j2+1,j3,ex))/(2.*dr(1))
               ! ux = ajrx*uur+ajsx*uus
               ! vy = ajry*vvr+ajsy*vvs
               ! (ax,ay) :  coefficients of Ex(iv) and Ey(iv) in div(E)(jv)
                 ax = (i1-j1)/(2.*dx(0))
                 ay = (i2-j2)/(2.*dx(1))
                  az = (i3-j3)/(2.*dx(2))
                 aSq = ax**2 + ay**2 + az**2
               ! adjust the interpolation pt: 
               u(i1,i2,i3,ex) = u(i1,i2,i3,ex) - res*ax/aSq   ! project Ev(jv) so div(E)=0
               u(i1,i2,i3,ey) = u(i1,i2,i3,ey) - res*ay/aSq
                u(i1,i2,i3,ez) = u(i1,i2,i3,ez) - res*az/aSq
               ! check ...
               if( .true. .or. debug.gt.2 )then
                      ux = (-u(j1-1,j2,j3,ex)+u(j1+1,j2,j3,ex))/(2.*dx(
     & 0))
                      vy = (-u(j1,j2-1,j3,ey)+u(j1,j2+1,j3,ey))/(2.*dx(
     & 1))
                    res = ux+ vy
                 write(*,'(" projectInterp: interp: grid=",i4," i1,
     & i2=",2i4,", interior:j1,j2=",2i4," new div=",e10.2)') grid,i1,
     & i2,j1,j2,res
               end if
             end if
             end do
             end do
             end do
           end if
         end do
         end do
         end do

      else if( nd.eq.3 .and. gridType.eq.curvilinear .and. 
     & orderOfAccuracy.eq.2 )then

         do i3=n3a,n3b
         do i2=n2a,n2b
         do i1=n1a,n1b
           if( mask(i1,i2,i3).lt.0 )then
             ! interp. pt. found -- find a nearby discretization pt
             ! write(*,'(" projectInterp: interp found: i1,i2=",2i4)') i1,i2
             ! *********** fix me: define bounds in terms of n1a,... ********
             do j3=i3-w3,i3+w3
             do j2=i2-w2,i2+w2,w2
             do j1=i1-w1,i1+w1,w1
              if( iabs(j1-i1)+iabs(j2-i2)+iabs(j3-i3).eq.numGhost 
     & .and. (mask(j1,j2,j3).gt.0) )then
               ! Enforce div(E)=0 at point jv by adjusting point iv
                  ! this next call will define the jacobian and its derivatives (parameteric and spatial)
                  ajrx = rsxy(j1,j2,j3,0,0)
                  ajsx = rsxy(j1,j2,j3,1,0)
                  ajtx = rsxy(j1,j2,j3,2,0)
                  ajry = rsxy(j1,j2,j3,0,1)
                  ajsy = rsxy(j1,j2,j3,1,1)
                  ajty = rsxy(j1,j2,j3,2,1)
                  ajrz = rsxy(j1,j2,j3,0,2)
                  ajsz = rsxy(j1,j2,j3,1,2)
                  ajtz = rsxy(j1,j2,j3,2,2)
                   uu = u(j1,j2,j3,ex)
                   uur = (-u(j1-1,j2,j3,ex)+u(j1+1,j2,j3,ex))/(2.*dr(0)
     & )
                   uus = (-u(j1,j2-1,j3,ex)+u(j1,j2+1,j3,ex))/(2.*dr(1)
     & )
                   uut = (-u(j1,j2,j3-1,ex)+u(j1,j2,j3+1,ex))/(2.*dr(2)
     & )
                   vv = u(j1,j2,j3,ey)
                   vvr = (-u(j1-1,j2,j3,ey)+u(j1+1,j2,j3,ey))/(2.*dr(0)
     & )
                   vvs = (-u(j1,j2-1,j3,ey)+u(j1,j2+1,j3,ey))/(2.*dr(1)
     & )
                   vvt = (-u(j1,j2,j3-1,ey)+u(j1,j2,j3+1,ey))/(2.*dr(2)
     & )
                    ux = ajrx*uur+ajsx*uus+ajtx*uut
                    vy = ajry*vvr+ajsy*vvs+ajty*vvt
                  res = ux+ vy
               !  uur = (-u(j1-1,j2,j3,ex)+u(j1+1,j2,j3,ex))/(2.*dr(0))
               !  uus = (-u(j1,j2-1,j3,ex)+u(j1,j2+1,j3,ex))/(2.*dr(1))
               ! ux = ajrx*uur+ajsx*uus
               ! vy = ajry*vvr+ajsy*vvs
               ! (ax,ay) :  coefficients of Ex(iv) and Ey(iv) in div(E)(jv)
                 ar = (i1-j1)/(2.*dr(0))
                 as = (i2-j2)/(2.*dr(1))
                  at = (i3-j3)/(2.*dr(2))
                 ax = ajrx*ar+ajsx*as+ajtx*at
                 ay = ajry*ar+ajsy*as+ajty*at
                 az = ajrz*ar+ajsz*as+ajtz*at
                 aSq = ax**2 + ay**2 + az**2
               ! adjust the interpolation pt: 
               u(i1,i2,i3,ex) = u(i1,i2,i3,ex) - res*ax/aSq   ! project Ev(jv) so div(E)=0
               u(i1,i2,i3,ey) = u(i1,i2,i3,ey) - res*ay/aSq
                u(i1,i2,i3,ez) = u(i1,i2,i3,ez) - res*az/aSq
               ! check ...
               if( .true. .or. debug.gt.2 )then
                    ! this next call will define the jacobian and its derivatives (parameteric and spatial)
                    ajrx = rsxy(j1,j2,j3,0,0)
                    ajsx = rsxy(j1,j2,j3,1,0)
                    ajtx = rsxy(j1,j2,j3,2,0)
                    ajry = rsxy(j1,j2,j3,0,1)
                    ajsy = rsxy(j1,j2,j3,1,1)
                    ajty = rsxy(j1,j2,j3,2,1)
                    ajrz = rsxy(j1,j2,j3,0,2)
                    ajsz = rsxy(j1,j2,j3,1,2)
                    ajtz = rsxy(j1,j2,j3,2,2)
                     uu = u(j1,j2,j3,ex)
                     uur = (-u(j1-1,j2,j3,ex)+u(j1+1,j2,j3,ex))/(2.*dr(
     & 0))
                     uus = (-u(j1,j2-1,j3,ex)+u(j1,j2+1,j3,ex))/(2.*dr(
     & 1))
                     uut = (-u(j1,j2,j3-1,ex)+u(j1,j2,j3+1,ex))/(2.*dr(
     & 2))
                     vv = u(j1,j2,j3,ey)
                     vvr = (-u(j1-1,j2,j3,ey)+u(j1+1,j2,j3,ey))/(2.*dr(
     & 0))
                     vvs = (-u(j1,j2-1,j3,ey)+u(j1,j2+1,j3,ey))/(2.*dr(
     & 1))
                     vvt = (-u(j1,j2,j3-1,ey)+u(j1,j2,j3+1,ey))/(2.*dr(
     & 2))
                      ux = ajrx*uur+ajsx*uus+ajtx*uut
                      vy = ajry*vvr+ajsy*vvs+ajty*vvt
                    res = ux+ vy
                 write(*,'(" projectInterp: interp: grid=",i4," i1,
     & i2=",2i4,", interior:j1,j2=",2i4," new div=",e10.2)') grid,i1,
     & i2,j1,j2,res
               end if
             end if
             end do
             end do
             end do
           end if
         end do
         end do
         end do

      else if( nd.eq.3 .and. gridType.eq.rectangular .and. 
     & orderOfAccuracy.eq.4 )then

         do i3=n3a,n3b
         do i2=n2a,n2b
         do i1=n1a,n1b
           if( mask(i1,i2,i3).lt.0 )then
             ! interp. pt. found -- find a nearby discretization pt
             ! write(*,'(" projectInterp: interp found: i1,i2=",2i4)') i1,i2
               ! only change interp points in the 2nd layer
             ! *********** fix me: define bounds in terms of n1a,... ********
             do j3=i3-w3,i3+w3
             do j2=i2-w2,i2+w2,w2
             do j1=i1-w1,i1+w1,w1
              if( iabs(j1-i1)+iabs(j2-i2)+iabs(j3-i3).eq.numGhost 
     & .and. (
     & j1.ge.n1a.and.j1.le.n1b.and.j2.ge.n2a.and.j2.le.n2b.and.mask(
     & j1,j2,j3).gt.0.and.mask((i1+j1)/2,(i2+j2)/2,(i3+j3)/2).lt.0) )
     & then
               ! Enforce div(E)=0 at point jv by adjusting point iv
                    ux = (u(j1-2,j2,j3,ex)-8.*u(j1-1,j2,j3,ex)+8.*u(j1+
     & 1,j2,j3,ex)-u(j1+2,j2,j3,ex))/(12.*dx(0))
                    vy = (u(j1,j2-2,j3,ey)-8.*u(j1,j2-1,j3,ey)+8.*u(j1,
     & j2+1,j3,ey)-u(j1,j2+2,j3,ey))/(12.*dx(1))
                  res = ux+ vy
               !  uur = (-u(j1-1,j2,j3,ex)+u(j1+1,j2,j3,ex))/(2.*dr(0))
               !  uus = (-u(j1,j2-1,j3,ex)+u(j1,j2+1,j3,ex))/(2.*dr(1))
               ! ux = ajrx*uur+ajsx*uus
               ! vy = ajry*vvr+ajsy*vvs
               ! (ax,ay) :  coefficients of Ex(iv) and Ey(iv) in div(E)(jv)
                 !ax = (i1-j1)*8./(12.*dx(0))  ! first ghost line coeff
                 !ay = (i2-j2)*8./(12.*dx(1))
                 ax = -(i1-j1)*.5/(12.*dx(0))
                 ay = -(i2-j2)*.5/(12.*dx(1))
                  !az = (i3-j3)*8./(12.*dx(2))
                  az = -(i3-j3)*.5/(12.*dx(2))
                 aSq = ax**2 + ay**2 + az**2
               ! adjust the interpolation pt: 
               u(i1,i2,i3,ex) = u(i1,i2,i3,ex) - res*ax/aSq   ! project Ev(jv) so div(E)=0
               u(i1,i2,i3,ey) = u(i1,i2,i3,ey) - res*ay/aSq
                u(i1,i2,i3,ez) = u(i1,i2,i3,ez) - res*az/aSq
               ! check ...
               if( .true. .or. debug.gt.2 )then
                      ux = (u(j1-2,j2,j3,ex)-8.*u(j1-1,j2,j3,ex)+8.*u(
     & j1+1,j2,j3,ex)-u(j1+2,j2,j3,ex))/(12.*dx(0))
                      vy = (u(j1,j2-2,j3,ey)-8.*u(j1,j2-1,j3,ey)+8.*u(
     & j1,j2+1,j3,ey)-u(j1,j2+2,j3,ey))/(12.*dx(1))
                    res = ux+ vy
                 write(*,'(" projectInterp: interp: grid=",i4," i1,
     & i2=",2i4,", interior:j1,j2=",2i4," new div=",e10.2)') grid,i1,
     & i2,j1,j2,res
               end if
             end if
             end do
             end do
             end do
           end if
         end do
         end do
         end do

      else if( nd.eq.3 .and. gridType.eq.curvilinear .and. 
     & orderOfAccuracy.eq.4 )then

         do i3=n3a,n3b
         do i2=n2a,n2b
         do i1=n1a,n1b
           if( mask(i1,i2,i3).lt.0 )then
             ! interp. pt. found -- find a nearby discretization pt
             ! write(*,'(" projectInterp: interp found: i1,i2=",2i4)') i1,i2
               ! only change interp points in the 2nd layer
             ! *********** fix me: define bounds in terms of n1a,... ********
             do j3=i3-w3,i3+w3
             do j2=i2-w2,i2+w2,w2
             do j1=i1-w1,i1+w1,w1
              if( iabs(j1-i1)+iabs(j2-i2)+iabs(j3-i3).eq.numGhost 
     & .and. (
     & j1.ge.n1a.and.j1.le.n1b.and.j2.ge.n2a.and.j2.le.n2b.and.mask(
     & j1,j2,j3).gt.0.and.mask((i1+j1)/2,(i2+j2)/2,(i3+j3)/2).lt.0) )
     & then
               ! Enforce div(E)=0 at point jv by adjusting point iv
                  ! this next call will define the jacobian and its derivatives (parameteric and spatial)
                  ajrx = rsxy(j1,j2,j3,0,0)
                  ajsx = rsxy(j1,j2,j3,1,0)
                  ajtx = rsxy(j1,j2,j3,2,0)
                  ajry = rsxy(j1,j2,j3,0,1)
                  ajsy = rsxy(j1,j2,j3,1,1)
                  ajty = rsxy(j1,j2,j3,2,1)
                  ajrz = rsxy(j1,j2,j3,0,2)
                  ajsz = rsxy(j1,j2,j3,1,2)
                  ajtz = rsxy(j1,j2,j3,2,2)
                   uu = u(j1,j2,j3,ex)
                   uur = (u(j1-2,j2,j3,ex)-8.*u(j1-1,j2,j3,ex)+8.*u(j1+
     & 1,j2,j3,ex)-u(j1+2,j2,j3,ex))/(12.*dr(0))
                   uus = (u(j1,j2-2,j3,ex)-8.*u(j1,j2-1,j3,ex)+8.*u(j1,
     & j2+1,j3,ex)-u(j1,j2+2,j3,ex))/(12.*dr(1))
                   uut = (u(j1,j2,j3-2,ex)-8.*u(j1,j2,j3-1,ex)+8.*u(j1,
     & j2,j3+1,ex)-u(j1,j2,j3+2,ex))/(12.*dr(2))
                   vv = u(j1,j2,j3,ey)
                   vvr = (u(j1-2,j2,j3,ey)-8.*u(j1-1,j2,j3,ey)+8.*u(j1+
     & 1,j2,j3,ey)-u(j1+2,j2,j3,ey))/(12.*dr(0))
                   vvs = (u(j1,j2-2,j3,ey)-8.*u(j1,j2-1,j3,ey)+8.*u(j1,
     & j2+1,j3,ey)-u(j1,j2+2,j3,ey))/(12.*dr(1))
                   vvt = (u(j1,j2,j3-2,ey)-8.*u(j1,j2,j3-1,ey)+8.*u(j1,
     & j2,j3+1,ey)-u(j1,j2,j3+2,ey))/(12.*dr(2))
                    ux = ajrx*uur+ajsx*uus+ajtx*uut
                    vy = ajry*vvr+ajsy*vvs+ajty*vvt
                  res = ux+ vy
               !  uur = (-u(j1-1,j2,j3,ex)+u(j1+1,j2,j3,ex))/(2.*dr(0))
               !  uus = (-u(j1,j2-1,j3,ex)+u(j1,j2+1,j3,ex))/(2.*dr(1))
               ! ux = ajrx*uur+ajsx*uus
               ! vy = ajry*vvr+ajsy*vvs
               ! (ax,ay) :  coefficients of Ex(iv) and Ey(iv) in div(E)(jv)
                 !ar = (i1-j1)*8./(12.*dr(0))
                 !as = (i2-j2)*8./(12.*dr(1))
                 ar = -(i1-j1)*.5/(12.*dr(0))
                 as = -(i2-j2)*.5/(12.*dr(1))
                  !at = (i3-j3)*8./(12.*dr(2))
                  at = -(i3-j3)*.5/(12.*dr(2))
                 ax = ajrx*ar+ajsx*as+ajtx*at
                 ay = ajry*ar+ajsy*as+ajty*at
                 az = ajrz*ar+ajsz*as+ajtz*at
                 aSq = ax**2 + ay**2 + az**2
               ! adjust the interpolation pt: 
               u(i1,i2,i3,ex) = u(i1,i2,i3,ex) - res*ax/aSq   ! project Ev(jv) so div(E)=0
               u(i1,i2,i3,ey) = u(i1,i2,i3,ey) - res*ay/aSq
                u(i1,i2,i3,ez) = u(i1,i2,i3,ez) - res*az/aSq
               ! check ...
               if( .true. .or. debug.gt.2 )then
                    ! this next call will define the jacobian and its derivatives (parameteric and spatial)
                    ajrx = rsxy(j1,j2,j3,0,0)
                    ajsx = rsxy(j1,j2,j3,1,0)
                    ajtx = rsxy(j1,j2,j3,2,0)
                    ajry = rsxy(j1,j2,j3,0,1)
                    ajsy = rsxy(j1,j2,j3,1,1)
                    ajty = rsxy(j1,j2,j3,2,1)
                    ajrz = rsxy(j1,j2,j3,0,2)
                    ajsz = rsxy(j1,j2,j3,1,2)
                    ajtz = rsxy(j1,j2,j3,2,2)
                     uu = u(j1,j2,j3,ex)
                     uur = (u(j1-2,j2,j3,ex)-8.*u(j1-1,j2,j3,ex)+8.*u(
     & j1+1,j2,j3,ex)-u(j1+2,j2,j3,ex))/(12.*dr(0))
                     uus = (u(j1,j2-2,j3,ex)-8.*u(j1,j2-1,j3,ex)+8.*u(
     & j1,j2+1,j3,ex)-u(j1,j2+2,j3,ex))/(12.*dr(1))
                     uut = (u(j1,j2,j3-2,ex)-8.*u(j1,j2,j3-1,ex)+8.*u(
     & j1,j2,j3+1,ex)-u(j1,j2,j3+2,ex))/(12.*dr(2))
                     vv = u(j1,j2,j3,ey)
                     vvr = (u(j1-2,j2,j3,ey)-8.*u(j1-1,j2,j3,ey)+8.*u(
     & j1+1,j2,j3,ey)-u(j1+2,j2,j3,ey))/(12.*dr(0))
                     vvs = (u(j1,j2-2,j3,ey)-8.*u(j1,j2-1,j3,ey)+8.*u(
     & j1,j2+1,j3,ey)-u(j1,j2+2,j3,ey))/(12.*dr(1))
                     vvt = (u(j1,j2,j3-2,ey)-8.*u(j1,j2,j3-1,ey)+8.*u(
     & j1,j2,j3+1,ey)-u(j1,j2,j3+2,ey))/(12.*dr(2))
                      ux = ajrx*uur+ajsx*uus+ajtx*uut
                      vy = ajry*vvr+ajsy*vvs+ajty*vvt
                    res = ux+ vy
                 write(*,'(" projectInterp: interp: grid=",i4," i1,
     & i2=",2i4,", interior:j1,j2=",2i4," new div=",e10.2)') grid,i1,
     & i2,j1,j2,res
               end if
             end if
             end do
             end do
             end do
           end if
         end do
         end do
         end do

      else

        write(*,'("mxProjectInterp:ERROR: not implemented for nd,
     & gridType,orderOfAccuracy=",3i3)') nd,gridType,orderOfAccuracy

        stop 12763

      end if


      return
      end
