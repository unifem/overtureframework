! This file automatically generated from div.bf with bpp.
c This file contains:
c
c   divergenceFDeriv  : compute the divergence, conservative and non-conservative, orders 2,4,6,8
c

c These next include files will define the macros that will define the difference approximations


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

c  3D

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







c =====================================================================
c Main Macro to evaluate the divergence
c
c  DIM : 2,3
c ORDER: 2,4,6,8
c =====================================================================

      subroutine divergenceFDeriv( nd,
     &    nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &    ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b, ! dimensions for u
     &    ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b, ! dimensions for deriv
     &    n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,
     &    dx, dr,
     &    rsxy, jac, u,s, deriv,
     &    ndw,w,  ! work space
     &    derivOption, derivType, gridType, order, averagingType,
     &    dir1, dir2  )
c======================================================================
c  Discretizations for
c           div
c  
c ca,cb : assign components c=ca,..,cb (base 0)
c derivOption : 4=divergence
c derivType : 0=nonconservative, 1=conservative, 2=conservative+symmetric
c gridType: 0=rectangular, 1=non-rectangular
c order : 2 or 4
c averagingType : arithmeticAverage=0, harmonicAverage=1
c dir1,dir2 : for derivOption=derivativeScalarDerivative
c rsxy : not used if rectangular
c dr : 
c 
c======================================================================
      implicit none
      integer nd,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b,
     &  n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,
     &  derivOption, derivType, gridType, order, averagingType,ndw,
     & dir1,dir2

      real dx(0:2),dr(0:2)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real jac(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real s(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      real u(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,ndu4a:ndu4b)
      real deriv(ndd1a:ndd1b,ndd2a:ndd2b,ndd3a:ndd3b,ndd4a:ndd4b)
      real w(0:*)

c      real h21(3),d22(3),d12(3),h22(3)
c      real d24(3),d14(3),h42(3),h41(3)
      integer i1,i2,i3
      real ux,vy,wz

      integer n,nda,ndwMin,c1,c2,c3
      integer laplace,divScalarGrad,derivativeScalarDerivative,
     & divTensorGrad,divergence
      parameter(laplace=0,divScalarGrad=1,derivativeScalarDerivative=2,
     & divTensorGrad=3,divergence=4)

      integer rectangular,curvilinear
      parameter( rectangular=0,curvilinear=1 )

      integer nonConservative,conservative,conservativeAndSymmetric
      parameter( nonConservative=0, conservative=1, 
     & conservativeAndSymmetric=2)

c     --- start statement function ----
      integer kd,m
      real u1,u2

      real rx,rxr,rxs,rxt,rxrr,rxrs,rxss,rxrt,rxst,rxtt,rxrrr,rxrrs,
     & rxrss,rxsss,rxrrt,rxrst,rxsst,rxrtt,rxstt,rxttt,rxrrrr,rxrrrs,
     & rxrrss,rxrsss,rxssss,rxrrrt,rxrrst,rxrsst,rxssst,rxrrtt,rxrstt,
     & rxsstt,rxrttt,rxsttt,rxtttt,rxrrrrr,rxrrrrs,rxrrrss,rxrrsss,
     & rxrssss,rxsssss,rxrrrrt,rxrrrst,rxrrsst,rxrssst,rxsssst,
     & rxrrrtt,rxrrstt,rxrsstt,rxssstt,rxrrttt,rxrsttt,rxssttt,
     & rxrtttt,rxstttt,rxttttt,rxrrrrrr,rxrrrrrs,rxrrrrss,rxrrrsss,
     & rxrrssss,rxrsssss,rxssssss,rxrrrrrt,rxrrrrst,rxrrrsst,rxrrssst,
     & rxrsssst,rxssssst,rxrrrrtt,rxrrrstt,rxrrsstt,rxrssstt,rxsssstt,
     & rxrrrttt,rxrrsttt,rxrssttt,rxsssttt,rxrrtttt,rxrstttt,rxsstttt,
     & rxrttttt,rxsttttt,rxtttttt
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

       real rxrx,rxrxr,rxrxs,rxrxt,rxrxrr,rxrxrs,rxrxss,rxrxrt,rxrxst,
     & rxrxtt,rxrxrrr,rxrxrrs,rxrxrss,rxrxsss,rxrxrrt,rxrxrst,rxrxsst,
     & rxrxrtt,rxrxstt,rxrxttt,rxrxrrrr,rxrxrrrs,rxrxrrss,rxrxrsss,
     & rxrxssss,rxrxrrrt,rxrxrrst,rxrxrsst,rxrxssst,rxrxrrtt,rxrxrstt,
     & rxrxsstt,rxrxrttt,rxrxsttt,rxrxtttt,rxrxrrrrr,rxrxrrrrs,
     & rxrxrrrss,rxrxrrsss,rxrxrssss,rxrxsssss,rxrxrrrrt,rxrxrrrst,
     & rxrxrrsst,rxrxrssst,rxrxsssst,rxrxrrrtt,rxrxrrstt,rxrxrsstt,
     & rxrxssstt,rxrxrrttt,rxrxrsttt,rxrxssttt,rxrxrtttt,rxrxstttt,
     & rxrxttttt,rxrxrrrrrr,rxrxrrrrrs,rxrxrrrrss,rxrxrrrsss,
     & rxrxrrssss,rxrxrsssss,rxrxssssss,rxrxrrrrrt,rxrxrrrrst,
     & rxrxrrrsst,rxrxrrssst,rxrxrsssst,rxrxssssst,rxrxrrrrtt,
     & rxrxrrrstt,rxrxrrsstt,rxrxrssstt,rxrxsssstt,rxrxrrrttt,
     & rxrxrrsttt,rxrxrssttt,rxrxsssttt,rxrxrrtttt,rxrxrstttt,
     & rxrxsstttt,rxrxrttttt,rxrxsttttt,rxrxtttttt
       real rxsx,rxsxr,rxsxs,rxsxt,rxsxrr,rxsxrs,rxsxss,rxsxrt,rxsxst,
     & rxsxtt,rxsxrrr,rxsxrrs,rxsxrss,rxsxsss,rxsxrrt,rxsxrst,rxsxsst,
     & rxsxrtt,rxsxstt,rxsxttt,rxsxrrrr,rxsxrrrs,rxsxrrss,rxsxrsss,
     & rxsxssss,rxsxrrrt,rxsxrrst,rxsxrsst,rxsxssst,rxsxrrtt,rxsxrstt,
     & rxsxsstt,rxsxrttt,rxsxsttt,rxsxtttt,rxsxrrrrr,rxsxrrrrs,
     & rxsxrrrss,rxsxrrsss,rxsxrssss,rxsxsssss,rxsxrrrrt,rxsxrrrst,
     & rxsxrrsst,rxsxrssst,rxsxsssst,rxsxrrrtt,rxsxrrstt,rxsxrsstt,
     & rxsxssstt,rxsxrrttt,rxsxrsttt,rxsxssttt,rxsxrtttt,rxsxstttt,
     & rxsxttttt,rxsxrrrrrr,rxsxrrrrrs,rxsxrrrrss,rxsxrrrsss,
     & rxsxrrssss,rxsxrsssss,rxsxssssss,rxsxrrrrrt,rxsxrrrrst,
     & rxsxrrrsst,rxsxrrssst,rxsxrsssst,rxsxssssst,rxsxrrrrtt,
     & rxsxrrrstt,rxsxrrsstt,rxsxrssstt,rxsxsssstt,rxsxrrrttt,
     & rxsxrrsttt,rxsxrssttt,rxsxsssttt,rxsxrrtttt,rxsxrstttt,
     & rxsxsstttt,rxsxrttttt,rxsxsttttt,rxsxtttttt
       real rxry,rxryr,rxrys,rxryt,rxryrr,rxryrs,rxryss,rxryrt,rxryst,
     & rxrytt,rxryrrr,rxryrrs,rxryrss,rxrysss,rxryrrt,rxryrst,rxrysst,
     & rxryrtt,rxrystt,rxryttt,rxryrrrr,rxryrrrs,rxryrrss,rxryrsss,
     & rxryssss,rxryrrrt,rxryrrst,rxryrsst,rxryssst,rxryrrtt,rxryrstt,
     & rxrysstt,rxryrttt,rxrysttt,rxrytttt,rxryrrrrr,rxryrrrrs,
     & rxryrrrss,rxryrrsss,rxryrssss,rxrysssss,rxryrrrrt,rxryrrrst,
     & rxryrrsst,rxryrssst,rxrysssst,rxryrrrtt,rxryrrstt,rxryrsstt,
     & rxryssstt,rxryrrttt,rxryrsttt,rxryssttt,rxryrtttt,rxrystttt,
     & rxryttttt,rxryrrrrrr,rxryrrrrrs,rxryrrrrss,rxryrrrsss,
     & rxryrrssss,rxryrsssss,rxryssssss,rxryrrrrrt,rxryrrrrst,
     & rxryrrrsst,rxryrrssst,rxryrsssst,rxryssssst,rxryrrrrtt,
     & rxryrrrstt,rxryrrsstt,rxryrssstt,rxrysssstt,rxryrrrttt,
     & rxryrrsttt,rxryrssttt,rxrysssttt,rxryrrtttt,rxryrstttt,
     & rxrysstttt,rxryrttttt,rxrysttttt,rxrytttttt
       real rxsy,rxsyr,rxsys,rxsyt,rxsyrr,rxsyrs,rxsyss,rxsyrt,rxsyst,
     & rxsytt,rxsyrrr,rxsyrrs,rxsyrss,rxsysss,rxsyrrt,rxsyrst,rxsysst,
     & rxsyrtt,rxsystt,rxsyttt,rxsyrrrr,rxsyrrrs,rxsyrrss,rxsyrsss,
     & rxsyssss,rxsyrrrt,rxsyrrst,rxsyrsst,rxsyssst,rxsyrrtt,rxsyrstt,
     & rxsysstt,rxsyrttt,rxsysttt,rxsytttt,rxsyrrrrr,rxsyrrrrs,
     & rxsyrrrss,rxsyrrsss,rxsyrssss,rxsysssss,rxsyrrrrt,rxsyrrrst,
     & rxsyrrsst,rxsyrssst,rxsysssst,rxsyrrrtt,rxsyrrstt,rxsyrsstt,
     & rxsyssstt,rxsyrrttt,rxsyrsttt,rxsyssttt,rxsyrtttt,rxsystttt,
     & rxsyttttt,rxsyrrrrrr,rxsyrrrrrs,rxsyrrrrss,rxsyrrrsss,
     & rxsyrrssss,rxsyrsssss,rxsyssssss,rxsyrrrrrt,rxsyrrrrst,
     & rxsyrrrsst,rxsyrrssst,rxsyrsssst,rxsyssssst,rxsyrrrrtt,
     & rxsyrrrstt,rxsyrrsstt,rxsyrssstt,rxsysssstt,rxsyrrrttt,
     & rxsyrrsttt,rxsyrssttt,rxsysssttt,rxsyrrtttt,rxsyrstttt,
     & rxsysstttt,rxsyrttttt,rxsysttttt,rxsytttttt
       real rxrxx,rxrxy,rxrxz,rxrxxx,rxrxxy,rxrxyy,rxrxxz,rxrxyz,
     & rxrxzz,rxrxxxx,rxrxxxy,rxrxxyy,rxrxyyy,rxrxxxz,rxrxxyz,rxrxyyz,
     & rxrxxzz,rxrxyzz,rxrxzzz,rxrxxxxx,rxrxxxxy,rxrxxxyy,rxrxxyyy,
     & rxrxyyyy,rxrxxxxz,rxrxxxyz,rxrxxyyz,rxrxyyyz,rxrxxxzz,rxrxxyzz,
     & rxrxyyzz,rxrxxzzz,rxrxyzzz,rxrxzzzz,rxrxxxxxx,rxrxxxxxy,
     & rxrxxxxyy,rxrxxxyyy,rxrxxyyyy,rxrxyyyyy,rxrxxxxxz,rxrxxxxyz,
     & rxrxxxyyz,rxrxxyyyz,rxrxyyyyz,rxrxxxxzz,rxrxxxyzz,rxrxxyyzz,
     & rxrxyyyzz,rxrxxxzzz,rxrxxyzzz,rxrxyyzzz,rxrxxzzzz,rxrxyzzzz,
     & rxrxzzzzz,rxrxxxxxxx,rxrxxxxxxy,rxrxxxxxyy,rxrxxxxyyy,
     & rxrxxxyyyy,rxrxxyyyyy,rxrxyyyyyy,rxrxxxxxxz,rxrxxxxxyz,
     & rxrxxxxyyz,rxrxxxyyyz,rxrxxyyyyz,rxrxyyyyyz,rxrxxxxxzz,
     & rxrxxxxyzz,rxrxxxyyzz,rxrxxyyyzz,rxrxyyyyzz,rxrxxxxzzz,
     & rxrxxxyzzz,rxrxxyyzzz,rxrxyyyzzz,rxrxxxzzzz,rxrxxyzzzz,
     & rxrxyyzzzz,rxrxxzzzzz,rxrxyzzzzz,rxrxzzzzzz
       real rxsxx,rxsxy,rxsxz,rxsxxx,rxsxxy,rxsxyy,rxsxxz,rxsxyz,
     & rxsxzz,rxsxxxx,rxsxxxy,rxsxxyy,rxsxyyy,rxsxxxz,rxsxxyz,rxsxyyz,
     & rxsxxzz,rxsxyzz,rxsxzzz,rxsxxxxx,rxsxxxxy,rxsxxxyy,rxsxxyyy,
     & rxsxyyyy,rxsxxxxz,rxsxxxyz,rxsxxyyz,rxsxyyyz,rxsxxxzz,rxsxxyzz,
     & rxsxyyzz,rxsxxzzz,rxsxyzzz,rxsxzzzz,rxsxxxxxx,rxsxxxxxy,
     & rxsxxxxyy,rxsxxxyyy,rxsxxyyyy,rxsxyyyyy,rxsxxxxxz,rxsxxxxyz,
     & rxsxxxyyz,rxsxxyyyz,rxsxyyyyz,rxsxxxxzz,rxsxxxyzz,rxsxxyyzz,
     & rxsxyyyzz,rxsxxxzzz,rxsxxyzzz,rxsxyyzzz,rxsxxzzzz,rxsxyzzzz,
     & rxsxzzzzz,rxsxxxxxxx,rxsxxxxxxy,rxsxxxxxyy,rxsxxxxyyy,
     & rxsxxxyyyy,rxsxxyyyyy,rxsxyyyyyy,rxsxxxxxxz,rxsxxxxxyz,
     & rxsxxxxyyz,rxsxxxyyyz,rxsxxyyyyz,rxsxyyyyyz,rxsxxxxxzz,
     & rxsxxxxyzz,rxsxxxyyzz,rxsxxyyyzz,rxsxyyyyzz,rxsxxxxzzz,
     & rxsxxxyzzz,rxsxxyyzzz,rxsxyyyzzz,rxsxxxzzzz,rxsxxyzzzz,
     & rxsxyyzzzz,rxsxxzzzzz,rxsxyzzzzz,rxsxzzzzzz
       real rxryx,rxryy,rxryz,rxryxx,rxryxy,rxryyy,rxryxz,rxryyz,
     & rxryzz,rxryxxx,rxryxxy,rxryxyy,rxryyyy,rxryxxz,rxryxyz,rxryyyz,
     & rxryxzz,rxryyzz,rxryzzz,rxryxxxx,rxryxxxy,rxryxxyy,rxryxyyy,
     & rxryyyyy,rxryxxxz,rxryxxyz,rxryxyyz,rxryyyyz,rxryxxzz,rxryxyzz,
     & rxryyyzz,rxryxzzz,rxryyzzz,rxryzzzz,rxryxxxxx,rxryxxxxy,
     & rxryxxxyy,rxryxxyyy,rxryxyyyy,rxryyyyyy,rxryxxxxz,rxryxxxyz,
     & rxryxxyyz,rxryxyyyz,rxryyyyyz,rxryxxxzz,rxryxxyzz,rxryxyyzz,
     & rxryyyyzz,rxryxxzzz,rxryxyzzz,rxryyyzzz,rxryxzzzz,rxryyzzzz,
     & rxryzzzzz,rxryxxxxxx,rxryxxxxxy,rxryxxxxyy,rxryxxxyyy,
     & rxryxxyyyy,rxryxyyyyy,rxryyyyyyy,rxryxxxxxz,rxryxxxxyz,
     & rxryxxxyyz,rxryxxyyyz,rxryxyyyyz,rxryyyyyyz,rxryxxxxzz,
     & rxryxxxyzz,rxryxxyyzz,rxryxyyyzz,rxryyyyyzz,rxryxxxzzz,
     & rxryxxyzzz,rxryxyyzzz,rxryyyyzzz,rxryxxzzzz,rxryxyzzzz,
     & rxryyyzzzz,rxryxzzzzz,rxryyzzzzz,rxryzzzzzz
       real rxsyx,rxsyy,rxsyz,rxsyxx,rxsyxy,rxsyyy,rxsyxz,rxsyyz,
     & rxsyzz,rxsyxxx,rxsyxxy,rxsyxyy,rxsyyyy,rxsyxxz,rxsyxyz,rxsyyyz,
     & rxsyxzz,rxsyyzz,rxsyzzz,rxsyxxxx,rxsyxxxy,rxsyxxyy,rxsyxyyy,
     & rxsyyyyy,rxsyxxxz,rxsyxxyz,rxsyxyyz,rxsyyyyz,rxsyxxzz,rxsyxyzz,
     & rxsyyyzz,rxsyxzzz,rxsyyzzz,rxsyzzzz,rxsyxxxxx,rxsyxxxxy,
     & rxsyxxxyy,rxsyxxyyy,rxsyxyyyy,rxsyyyyyy,rxsyxxxxz,rxsyxxxyz,
     & rxsyxxyyz,rxsyxyyyz,rxsyyyyyz,rxsyxxxzz,rxsyxxyzz,rxsyxyyzz,
     & rxsyyyyzz,rxsyxxzzz,rxsyxyzzz,rxsyyyzzz,rxsyxzzzz,rxsyyzzzz,
     & rxsyzzzzz,rxsyxxxxxx,rxsyxxxxxy,rxsyxxxxyy,rxsyxxxyyy,
     & rxsyxxyyyy,rxsyxyyyyy,rxsyyyyyyy,rxsyxxxxxz,rxsyxxxxyz,
     & rxsyxxxyyz,rxsyxxyyyz,rxsyxyyyyz,rxsyyyyyyz,rxsyxxxxzz,
     & rxsyxxxyzz,rxsyxxyyzz,rxsyxyyyzz,rxsyyyyyzz,rxsyxxxzzz,
     & rxsyxxyzzz,rxsyxyyzzz,rxsyyyyzzz,rxsyxxzzzz,rxsyxyzzzz,
     & rxsyyyzzzz,rxsyxzzzzz,rxsyyzzzzz,rxsyzzzzzz
       real rxrz,rxrzr,rxrzs,rxrzt,rxrzrr,rxrzrs,rxrzss,rxrzrt,rxrzst,
     & rxrztt,rxrzrrr,rxrzrrs,rxrzrss,rxrzsss,rxrzrrt,rxrzrst,rxrzsst,
     & rxrzrtt,rxrzstt,rxrzttt,rxrzrrrr,rxrzrrrs,rxrzrrss,rxrzrsss,
     & rxrzssss,rxrzrrrt,rxrzrrst,rxrzrsst,rxrzssst,rxrzrrtt,rxrzrstt,
     & rxrzsstt,rxrzrttt,rxrzsttt,rxrztttt,rxrzrrrrr,rxrzrrrrs,
     & rxrzrrrss,rxrzrrsss,rxrzrssss,rxrzsssss,rxrzrrrrt,rxrzrrrst,
     & rxrzrrsst,rxrzrssst,rxrzsssst,rxrzrrrtt,rxrzrrstt,rxrzrsstt,
     & rxrzssstt,rxrzrrttt,rxrzrsttt,rxrzssttt,rxrzrtttt,rxrzstttt,
     & rxrzttttt,rxrzrrrrrr,rxrzrrrrrs,rxrzrrrrss,rxrzrrrsss,
     & rxrzrrssss,rxrzrsssss,rxrzssssss,rxrzrrrrrt,rxrzrrrrst,
     & rxrzrrrsst,rxrzrrssst,rxrzrsssst,rxrzssssst,rxrzrrrrtt,
     & rxrzrrrstt,rxrzrrsstt,rxrzrssstt,rxrzsssstt,rxrzrrrttt,
     & rxrzrrsttt,rxrzrssttt,rxrzsssttt,rxrzrrtttt,rxrzrstttt,
     & rxrzsstttt,rxrzrttttt,rxrzsttttt,rxrztttttt
       real rxsz,rxszr,rxszs,rxszt,rxszrr,rxszrs,rxszss,rxszrt,rxszst,
     & rxsztt,rxszrrr,rxszrrs,rxszrss,rxszsss,rxszrrt,rxszrst,rxszsst,
     & rxszrtt,rxszstt,rxszttt,rxszrrrr,rxszrrrs,rxszrrss,rxszrsss,
     & rxszssss,rxszrrrt,rxszrrst,rxszrsst,rxszssst,rxszrrtt,rxszrstt,
     & rxszsstt,rxszrttt,rxszsttt,rxsztttt,rxszrrrrr,rxszrrrrs,
     & rxszrrrss,rxszrrsss,rxszrssss,rxszsssss,rxszrrrrt,rxszrrrst,
     & rxszrrsst,rxszrssst,rxszsssst,rxszrrrtt,rxszrrstt,rxszrsstt,
     & rxszssstt,rxszrrttt,rxszrsttt,rxszssttt,rxszrtttt,rxszstttt,
     & rxszttttt,rxszrrrrrr,rxszrrrrrs,rxszrrrrss,rxszrrrsss,
     & rxszrrssss,rxszrsssss,rxszssssss,rxszrrrrrt,rxszrrrrst,
     & rxszrrrsst,rxszrrssst,rxszrsssst,rxszssssst,rxszrrrrtt,
     & rxszrrrstt,rxszrrsstt,rxszrssstt,rxszsssstt,rxszrrrttt,
     & rxszrrsttt,rxszrssttt,rxszsssttt,rxszrrtttt,rxszrstttt,
     & rxszsstttt,rxszrttttt,rxszsttttt,rxsztttttt
       real rxtx,rxtxr,rxtxs,rxtxt,rxtxrr,rxtxrs,rxtxss,rxtxrt,rxtxst,
     & rxtxtt,rxtxrrr,rxtxrrs,rxtxrss,rxtxsss,rxtxrrt,rxtxrst,rxtxsst,
     & rxtxrtt,rxtxstt,rxtxttt,rxtxrrrr,rxtxrrrs,rxtxrrss,rxtxrsss,
     & rxtxssss,rxtxrrrt,rxtxrrst,rxtxrsst,rxtxssst,rxtxrrtt,rxtxrstt,
     & rxtxsstt,rxtxrttt,rxtxsttt,rxtxtttt,rxtxrrrrr,rxtxrrrrs,
     & rxtxrrrss,rxtxrrsss,rxtxrssss,rxtxsssss,rxtxrrrrt,rxtxrrrst,
     & rxtxrrsst,rxtxrssst,rxtxsssst,rxtxrrrtt,rxtxrrstt,rxtxrsstt,
     & rxtxssstt,rxtxrrttt,rxtxrsttt,rxtxssttt,rxtxrtttt,rxtxstttt,
     & rxtxttttt,rxtxrrrrrr,rxtxrrrrrs,rxtxrrrrss,rxtxrrrsss,
     & rxtxrrssss,rxtxrsssss,rxtxssssss,rxtxrrrrrt,rxtxrrrrst,
     & rxtxrrrsst,rxtxrrssst,rxtxrsssst,rxtxssssst,rxtxrrrrtt,
     & rxtxrrrstt,rxtxrrsstt,rxtxrssstt,rxtxsssstt,rxtxrrrttt,
     & rxtxrrsttt,rxtxrssttt,rxtxsssttt,rxtxrrtttt,rxtxrstttt,
     & rxtxsstttt,rxtxrttttt,rxtxsttttt,rxtxtttttt
       real rxty,rxtyr,rxtys,rxtyt,rxtyrr,rxtyrs,rxtyss,rxtyrt,rxtyst,
     & rxtytt,rxtyrrr,rxtyrrs,rxtyrss,rxtysss,rxtyrrt,rxtyrst,rxtysst,
     & rxtyrtt,rxtystt,rxtyttt,rxtyrrrr,rxtyrrrs,rxtyrrss,rxtyrsss,
     & rxtyssss,rxtyrrrt,rxtyrrst,rxtyrsst,rxtyssst,rxtyrrtt,rxtyrstt,
     & rxtysstt,rxtyrttt,rxtysttt,rxtytttt,rxtyrrrrr,rxtyrrrrs,
     & rxtyrrrss,rxtyrrsss,rxtyrssss,rxtysssss,rxtyrrrrt,rxtyrrrst,
     & rxtyrrsst,rxtyrssst,rxtysssst,rxtyrrrtt,rxtyrrstt,rxtyrsstt,
     & rxtyssstt,rxtyrrttt,rxtyrsttt,rxtyssttt,rxtyrtttt,rxtystttt,
     & rxtyttttt,rxtyrrrrrr,rxtyrrrrrs,rxtyrrrrss,rxtyrrrsss,
     & rxtyrrssss,rxtyrsssss,rxtyssssss,rxtyrrrrrt,rxtyrrrrst,
     & rxtyrrrsst,rxtyrrssst,rxtyrsssst,rxtyssssst,rxtyrrrrtt,
     & rxtyrrrstt,rxtyrrsstt,rxtyrssstt,rxtysssstt,rxtyrrrttt,
     & rxtyrrsttt,rxtyrssttt,rxtysssttt,rxtyrrtttt,rxtyrstttt,
     & rxtysstttt,rxtyrttttt,rxtysttttt,rxtytttttt
       real rxtz,rxtzr,rxtzs,rxtzt,rxtzrr,rxtzrs,rxtzss,rxtzrt,rxtzst,
     & rxtztt,rxtzrrr,rxtzrrs,rxtzrss,rxtzsss,rxtzrrt,rxtzrst,rxtzsst,
     & rxtzrtt,rxtzstt,rxtzttt,rxtzrrrr,rxtzrrrs,rxtzrrss,rxtzrsss,
     & rxtzssss,rxtzrrrt,rxtzrrst,rxtzrsst,rxtzssst,rxtzrrtt,rxtzrstt,
     & rxtzsstt,rxtzrttt,rxtzsttt,rxtztttt,rxtzrrrrr,rxtzrrrrs,
     & rxtzrrrss,rxtzrrsss,rxtzrssss,rxtzsssss,rxtzrrrrt,rxtzrrrst,
     & rxtzrrsst,rxtzrssst,rxtzsssst,rxtzrrrtt,rxtzrrstt,rxtzrsstt,
     & rxtzssstt,rxtzrrttt,rxtzrsttt,rxtzssttt,rxtzrtttt,rxtzstttt,
     & rxtzttttt,rxtzrrrrrr,rxtzrrrrrs,rxtzrrrrss,rxtzrrrsss,
     & rxtzrrssss,rxtzrsssss,rxtzssssss,rxtzrrrrrt,rxtzrrrrst,
     & rxtzrrrsst,rxtzrrssst,rxtzrsssst,rxtzssssst,rxtzrrrrtt,
     & rxtzrrrstt,rxtzrrsstt,rxtzrssstt,rxtzsssstt,rxtzrrrttt,
     & rxtzrrsttt,rxtzrssttt,rxtzsssttt,rxtzrrtttt,rxtzrstttt,
     & rxtzsstttt,rxtzrttttt,rxtzsttttt,rxtztttttt
       real rxrzx,rxrzy,rxrzz,rxrzxx,rxrzxy,rxrzyy,rxrzxz,rxrzyz,
     & rxrzzz,rxrzxxx,rxrzxxy,rxrzxyy,rxrzyyy,rxrzxxz,rxrzxyz,rxrzyyz,
     & rxrzxzz,rxrzyzz,rxrzzzz,rxrzxxxx,rxrzxxxy,rxrzxxyy,rxrzxyyy,
     & rxrzyyyy,rxrzxxxz,rxrzxxyz,rxrzxyyz,rxrzyyyz,rxrzxxzz,rxrzxyzz,
     & rxrzyyzz,rxrzxzzz,rxrzyzzz,rxrzzzzz,rxrzxxxxx,rxrzxxxxy,
     & rxrzxxxyy,rxrzxxyyy,rxrzxyyyy,rxrzyyyyy,rxrzxxxxz,rxrzxxxyz,
     & rxrzxxyyz,rxrzxyyyz,rxrzyyyyz,rxrzxxxzz,rxrzxxyzz,rxrzxyyzz,
     & rxrzyyyzz,rxrzxxzzz,rxrzxyzzz,rxrzyyzzz,rxrzxzzzz,rxrzyzzzz,
     & rxrzzzzzz,rxrzxxxxxx,rxrzxxxxxy,rxrzxxxxyy,rxrzxxxyyy,
     & rxrzxxyyyy,rxrzxyyyyy,rxrzyyyyyy,rxrzxxxxxz,rxrzxxxxyz,
     & rxrzxxxyyz,rxrzxxyyyz,rxrzxyyyyz,rxrzyyyyyz,rxrzxxxxzz,
     & rxrzxxxyzz,rxrzxxyyzz,rxrzxyyyzz,rxrzyyyyzz,rxrzxxxzzz,
     & rxrzxxyzzz,rxrzxyyzzz,rxrzyyyzzz,rxrzxxzzzz,rxrzxyzzzz,
     & rxrzyyzzzz,rxrzxzzzzz,rxrzyzzzzz,rxrzzzzzzz
       real rxszx,rxszy,rxszz,rxszxx,rxszxy,rxszyy,rxszxz,rxszyz,
     & rxszzz,rxszxxx,rxszxxy,rxszxyy,rxszyyy,rxszxxz,rxszxyz,rxszyyz,
     & rxszxzz,rxszyzz,rxszzzz,rxszxxxx,rxszxxxy,rxszxxyy,rxszxyyy,
     & rxszyyyy,rxszxxxz,rxszxxyz,rxszxyyz,rxszyyyz,rxszxxzz,rxszxyzz,
     & rxszyyzz,rxszxzzz,rxszyzzz,rxszzzzz,rxszxxxxx,rxszxxxxy,
     & rxszxxxyy,rxszxxyyy,rxszxyyyy,rxszyyyyy,rxszxxxxz,rxszxxxyz,
     & rxszxxyyz,rxszxyyyz,rxszyyyyz,rxszxxxzz,rxszxxyzz,rxszxyyzz,
     & rxszyyyzz,rxszxxzzz,rxszxyzzz,rxszyyzzz,rxszxzzzz,rxszyzzzz,
     & rxszzzzzz,rxszxxxxxx,rxszxxxxxy,rxszxxxxyy,rxszxxxyyy,
     & rxszxxyyyy,rxszxyyyyy,rxszyyyyyy,rxszxxxxxz,rxszxxxxyz,
     & rxszxxxyyz,rxszxxyyyz,rxszxyyyyz,rxszyyyyyz,rxszxxxxzz,
     & rxszxxxyzz,rxszxxyyzz,rxszxyyyzz,rxszyyyyzz,rxszxxxzzz,
     & rxszxxyzzz,rxszxyyzzz,rxszyyyzzz,rxszxxzzzz,rxszxyzzzz,
     & rxszyyzzzz,rxszxzzzzz,rxszyzzzzz,rxszzzzzzz
       real rxtxx,rxtxy,rxtxz,rxtxxx,rxtxxy,rxtxyy,rxtxxz,rxtxyz,
     & rxtxzz,rxtxxxx,rxtxxxy,rxtxxyy,rxtxyyy,rxtxxxz,rxtxxyz,rxtxyyz,
     & rxtxxzz,rxtxyzz,rxtxzzz,rxtxxxxx,rxtxxxxy,rxtxxxyy,rxtxxyyy,
     & rxtxyyyy,rxtxxxxz,rxtxxxyz,rxtxxyyz,rxtxyyyz,rxtxxxzz,rxtxxyzz,
     & rxtxyyzz,rxtxxzzz,rxtxyzzz,rxtxzzzz,rxtxxxxxx,rxtxxxxxy,
     & rxtxxxxyy,rxtxxxyyy,rxtxxyyyy,rxtxyyyyy,rxtxxxxxz,rxtxxxxyz,
     & rxtxxxyyz,rxtxxyyyz,rxtxyyyyz,rxtxxxxzz,rxtxxxyzz,rxtxxyyzz,
     & rxtxyyyzz,rxtxxxzzz,rxtxxyzzz,rxtxyyzzz,rxtxxzzzz,rxtxyzzzz,
     & rxtxzzzzz,rxtxxxxxxx,rxtxxxxxxy,rxtxxxxxyy,rxtxxxxyyy,
     & rxtxxxyyyy,rxtxxyyyyy,rxtxyyyyyy,rxtxxxxxxz,rxtxxxxxyz,
     & rxtxxxxyyz,rxtxxxyyyz,rxtxxyyyyz,rxtxyyyyyz,rxtxxxxxzz,
     & rxtxxxxyzz,rxtxxxyyzz,rxtxxyyyzz,rxtxyyyyzz,rxtxxxxzzz,
     & rxtxxxyzzz,rxtxxyyzzz,rxtxyyyzzz,rxtxxxzzzz,rxtxxyzzzz,
     & rxtxyyzzzz,rxtxxzzzzz,rxtxyzzzzz,rxtxzzzzzz
       real rxtyx,rxtyy,rxtyz,rxtyxx,rxtyxy,rxtyyy,rxtyxz,rxtyyz,
     & rxtyzz,rxtyxxx,rxtyxxy,rxtyxyy,rxtyyyy,rxtyxxz,rxtyxyz,rxtyyyz,
     & rxtyxzz,rxtyyzz,rxtyzzz,rxtyxxxx,rxtyxxxy,rxtyxxyy,rxtyxyyy,
     & rxtyyyyy,rxtyxxxz,rxtyxxyz,rxtyxyyz,rxtyyyyz,rxtyxxzz,rxtyxyzz,
     & rxtyyyzz,rxtyxzzz,rxtyyzzz,rxtyzzzz,rxtyxxxxx,rxtyxxxxy,
     & rxtyxxxyy,rxtyxxyyy,rxtyxyyyy,rxtyyyyyy,rxtyxxxxz,rxtyxxxyz,
     & rxtyxxyyz,rxtyxyyyz,rxtyyyyyz,rxtyxxxzz,rxtyxxyzz,rxtyxyyzz,
     & rxtyyyyzz,rxtyxxzzz,rxtyxyzzz,rxtyyyzzz,rxtyxzzzz,rxtyyzzzz,
     & rxtyzzzzz,rxtyxxxxxx,rxtyxxxxxy,rxtyxxxxyy,rxtyxxxyyy,
     & rxtyxxyyyy,rxtyxyyyyy,rxtyyyyyyy,rxtyxxxxxz,rxtyxxxxyz,
     & rxtyxxxyyz,rxtyxxyyyz,rxtyxyyyyz,rxtyyyyyyz,rxtyxxxxzz,
     & rxtyxxxyzz,rxtyxxyyzz,rxtyxyyyzz,rxtyyyyyzz,rxtyxxxzzz,
     & rxtyxxyzzz,rxtyxyyzzz,rxtyyyyzzz,rxtyxxzzzz,rxtyxyzzzz,
     & rxtyyyzzzz,rxtyxzzzzz,rxtyyzzzzz,rxtyzzzzzz
       real rxtzx,rxtzy,rxtzz,rxtzxx,rxtzxy,rxtzyy,rxtzxz,rxtzyz,
     & rxtzzz,rxtzxxx,rxtzxxy,rxtzxyy,rxtzyyy,rxtzxxz,rxtzxyz,rxtzyyz,
     & rxtzxzz,rxtzyzz,rxtzzzz,rxtzxxxx,rxtzxxxy,rxtzxxyy,rxtzxyyy,
     & rxtzyyyy,rxtzxxxz,rxtzxxyz,rxtzxyyz,rxtzyyyz,rxtzxxzz,rxtzxyzz,
     & rxtzyyzz,rxtzxzzz,rxtzyzzz,rxtzzzzz,rxtzxxxxx,rxtzxxxxy,
     & rxtzxxxyy,rxtzxxyyy,rxtzxyyyy,rxtzyyyyy,rxtzxxxxz,rxtzxxxyz,
     & rxtzxxyyz,rxtzxyyyz,rxtzyyyyz,rxtzxxxzz,rxtzxxyzz,rxtzxyyzz,
     & rxtzyyyzz,rxtzxxzzz,rxtzxyzzz,rxtzyyzzz,rxtzxzzzz,rxtzyzzzz,
     & rxtzzzzzz,rxtzxxxxxx,rxtzxxxxxy,rxtzxxxxyy,rxtzxxxyyy,
     & rxtzxxyyyy,rxtzxyyyyy,rxtzyyyyyy,rxtzxxxxxz,rxtzxxxxyz,
     & rxtzxxxyyz,rxtzxxyyyz,rxtzxyyyyz,rxtzyyyyyz,rxtzxxxxzz,
     & rxtzxxxyzz,rxtzxxyyzz,rxtzxyyyzz,rxtzyyyyzz,rxtzxxxzzz,
     & rxtzxxyzzz,rxtzxyyzzz,rxtzyyyzzz,rxtzxxzzzz,rxtzxyzzzz,
     & rxtzyyzzzz,rxtzxzzzzz,rxtzyzzzzz,rxtzzzzzzz


      if( derivOption.ne.divergence )then
        write(*,'("divergenceFDeriv:ERROR:derivOption=",i6)') 
     & derivOption
        ! "
        stop 9273
      end if

      n=ndd4a

      c1=ca     ! ****
      c2=ca+1
      c3=ca+2

c       write(*,'(" i=",2i3," u=",2e11.2," div=",e11.2)') 
c     & i1,i2,u(i1,i2,i3,c1),u(i1,i2,i3,c2),deriv(i1,i2,i3,n)

c     Evaluate the derivative

      if( nd.eq.2 )then
        ! ********************************
        ! ************* 2D ***************
        ! ********************************

        if( order.eq.2 )then
c  *** Evaluate the divergence ***
           if( gridType.eq.rectangular )then
            ! Cartesian, dim=2, order=2 
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
               deriv(i1,i2,i3,n)=(-u(i1-1,i2,i3,c1)+u(i1+1,i2,i3,c1))/(
     & 2.*dx(0))+(-u(i1,i2-1,i3,c2)+u(i1,i2+1,i3,c2))/(2.*dx(1))
             end do
             end do
             end do
           else if( derivType.eq.nonConservative )then
            ! Curvilinear, non-conservative, dim=2, order=2 
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
             ! order 4:
             !                                        2,2,MAXDERIV
             rxrx = rsxy(i1,i2,i3,0,0)
             rxsx = rsxy(i1,i2,i3,1,0)
             rxry = rsxy(i1,i2,i3,0,1)
             rxsy = rsxy(i1,i2,i3,1,1)
             !                                                      2,2,MAXDERIV
             uu = u(i1,i2,i3,c1)
             uur = (-u(i1-1,i2,i3,c1)+u(i1+1,i2,i3,c1))/(2.*dr(0))
             uus = (-u(i1,i2-1,i3,c1)+u(i1,i2+1,i3,c1))/(2.*dr(1))
             ux = rxrx*uur+rxsx*uus
             uu = u(i1,i2,i3,c2)
             uur = (-u(i1-1,i2,i3,c2)+u(i1+1,i2,i3,c2))/(2.*dr(0))
             uus = (-u(i1,i2-1,i3,c2)+u(i1,i2+1,i3,c2))/(2.*dr(1))
             vy = rxry*uur+rxsy*uus
               deriv(i1,i2,i3,n)=ux+vy
             end do
             end do
             end do
           else
             ! conservative, curvilinear, dim=2, order=2 
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                 deriv(i1,i2,i3,n)=((-(jac(i1-1,i2,i3)*(rsxy(i1-1,i2,
     & i3,0,0)*u(i1-1,i2,i3,c1)+rsxy(i1-1,i2,i3,0,1)*u(i1-1,i2,i3,c2))
     & )+(jac(i1+1,i2,i3)*(rsxy(i1+1,i2,i3,0,0)*u(i1+1,i2,i3,c1)+rsxy(
     & i1+1,i2,i3,0,1)*u(i1+1,i2,i3,c2))))/(2.*dr(0))+(-(jac(i1,i2-1,
     & i3)*(rsxy(i1,i2-1,i3,1,0)*u(i1,i2-1,i3,c1)+rsxy(i1,i2-1,i3,1,1)
     & *u(i1,i2-1,i3,c2)))+(jac(i1,i2+1,i3)*(rsxy(i1,i2+1,i3,1,0)*u(
     & i1,i2+1,i3,c1)+rsxy(i1,i2+1,i3,1,1)*u(i1,i2+1,i3,c2))))/(2.*dr(
     & 1)))/jac(i1,i2,i3)
                end do
                end do
                end do
           end if
        else if( order.eq.4 )then
c  *** Evaluate the divergence ***
           if( gridType.eq.rectangular )then
            ! Cartesian, dim=2, order=4 
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
               deriv(i1,i2,i3,n)=(u(i1-2,i2,i3,c1)-8.*u(i1-1,i2,i3,c1)+
     & 8.*u(i1+1,i2,i3,c1)-u(i1+2,i2,i3,c1))/(12.*dx(0))+(u(i1,i2-2,
     & i3,c2)-8.*u(i1,i2-1,i3,c2)+8.*u(i1,i2+1,i3,c2)-u(i1,i2+2,i3,c2)
     & )/(12.*dx(1))
             end do
             end do
             end do
           else if( derivType.eq.nonConservative )then
            ! Curvilinear, non-conservative, dim=2, order=4 
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
             ! order 4:
             !                                        2,4,MAXDERIV
             rxrx = rsxy(i1,i2,i3,0,0)
             rxsx = rsxy(i1,i2,i3,1,0)
             rxry = rsxy(i1,i2,i3,0,1)
             rxsy = rsxy(i1,i2,i3,1,1)
             !                                                      2,4,MAXDERIV
             uu = u(i1,i2,i3,c1)
             uur = (u(i1-2,i2,i3,c1)-8.*u(i1-1,i2,i3,c1)+8.*u(i1+1,i2,
     & i3,c1)-u(i1+2,i2,i3,c1))/(12.*dr(0))
             uus = (u(i1,i2-2,i3,c1)-8.*u(i1,i2-1,i3,c1)+8.*u(i1,i2+1,
     & i3,c1)-u(i1,i2+2,i3,c1))/(12.*dr(1))
             ux = rxrx*uur+rxsx*uus
             uu = u(i1,i2,i3,c2)
             uur = (u(i1-2,i2,i3,c2)-8.*u(i1-1,i2,i3,c2)+8.*u(i1+1,i2,
     & i3,c2)-u(i1+2,i2,i3,c2))/(12.*dr(0))
             uus = (u(i1,i2-2,i3,c2)-8.*u(i1,i2-1,i3,c2)+8.*u(i1,i2+1,
     & i3,c2)-u(i1,i2+2,i3,c2))/(12.*dr(1))
             vy = rxry*uur+rxsy*uus
               deriv(i1,i2,i3,n)=ux+vy
             end do
             end do
             end do
           else
             ! conservative, curvilinear, dim=2, order=4 
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                 deriv(i1,i2,i3,n)=(((jac(i1-2,i2,i3)*(rsxy(i1-2,i2,i3,
     & 0,0)*u(i1-2,i2,i3,c1)+rsxy(i1-2,i2,i3,0,1)*u(i1-2,i2,i3,c2)))-
     & 8.*(jac(i1-1,i2,i3)*(rsxy(i1-1,i2,i3,0,0)*u(i1-1,i2,i3,c1)+
     & rsxy(i1-1,i2,i3,0,1)*u(i1-1,i2,i3,c2)))+8.*(jac(i1+1,i2,i3)*(
     & rsxy(i1+1,i2,i3,0,0)*u(i1+1,i2,i3,c1)+rsxy(i1+1,i2,i3,0,1)*u(
     & i1+1,i2,i3,c2)))-(jac(i1+2,i2,i3)*(rsxy(i1+2,i2,i3,0,0)*u(i1+2,
     & i2,i3,c1)+rsxy(i1+2,i2,i3,0,1)*u(i1+2,i2,i3,c2))))/(12.*dr(0))+
     & ((jac(i1,i2-2,i3)*(rsxy(i1,i2-2,i3,1,0)*u(i1,i2-2,i3,c1)+rsxy(
     & i1,i2-2,i3,1,1)*u(i1,i2-2,i3,c2)))-8.*(jac(i1,i2-1,i3)*(rsxy(
     & i1,i2-1,i3,1,0)*u(i1,i2-1,i3,c1)+rsxy(i1,i2-1,i3,1,1)*u(i1,i2-
     & 1,i3,c2)))+8.*(jac(i1,i2+1,i3)*(rsxy(i1,i2+1,i3,1,0)*u(i1,i2+1,
     & i3,c1)+rsxy(i1,i2+1,i3,1,1)*u(i1,i2+1,i3,c2)))-(jac(i1,i2+2,i3)
     & *(rsxy(i1,i2+2,i3,1,0)*u(i1,i2+2,i3,c1)+rsxy(i1,i2+2,i3,1,1)*u(
     & i1,i2+2,i3,c2))))/(12.*dr(1)))/jac(i1,i2,i3)
                end do
                end do
                end do
           end if
        else if( order.eq.6 )then
c  *** Evaluate the divergence ***
           if( gridType.eq.rectangular )then
            ! Cartesian, dim=2, order=6 
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
               deriv(i1,i2,i3,n)=(-u(i1-3,i2,i3,c1)+9.*u(i1-2,i2,i3,c1)
     & -45.*u(i1-1,i2,i3,c1)+45.*u(i1+1,i2,i3,c1)-9.*u(i1+2,i2,i3,c1)+
     & u(i1+3,i2,i3,c1))/(60.*dx(0))+(-u(i1,i2-3,i3,c2)+9.*u(i1,i2-2,
     & i3,c2)-45.*u(i1,i2-1,i3,c2)+45.*u(i1,i2+1,i3,c2)-9.*u(i1,i2+2,
     & i3,c2)+u(i1,i2+3,i3,c2))/(60.*dx(1))
             end do
             end do
             end do
           else if( derivType.eq.nonConservative )then
            ! Curvilinear, non-conservative, dim=2, order=6 
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
             ! order 4:
             !                                        2,6,MAXDERIV
             rxrx = rsxy(i1,i2,i3,0,0)
             rxsx = rsxy(i1,i2,i3,1,0)
             rxry = rsxy(i1,i2,i3,0,1)
             rxsy = rsxy(i1,i2,i3,1,1)
             !                                                      2,6,MAXDERIV
             uu = u(i1,i2,i3,c1)
             uur = (-u(i1-3,i2,i3,c1)+9.*u(i1-2,i2,i3,c1)-45.*u(i1-1,
     & i2,i3,c1)+45.*u(i1+1,i2,i3,c1)-9.*u(i1+2,i2,i3,c1)+u(i1+3,i2,
     & i3,c1))/(60.*dr(0))
             uus = (-u(i1,i2-3,i3,c1)+9.*u(i1,i2-2,i3,c1)-45.*u(i1,i2-
     & 1,i3,c1)+45.*u(i1,i2+1,i3,c1)-9.*u(i1,i2+2,i3,c1)+u(i1,i2+3,i3,
     & c1))/(60.*dr(1))
             ux = rxrx*uur+rxsx*uus
             uu = u(i1,i2,i3,c2)
             uur = (-u(i1-3,i2,i3,c2)+9.*u(i1-2,i2,i3,c2)-45.*u(i1-1,
     & i2,i3,c2)+45.*u(i1+1,i2,i3,c2)-9.*u(i1+2,i2,i3,c2)+u(i1+3,i2,
     & i3,c2))/(60.*dr(0))
             uus = (-u(i1,i2-3,i3,c2)+9.*u(i1,i2-2,i3,c2)-45.*u(i1,i2-
     & 1,i3,c2)+45.*u(i1,i2+1,i3,c2)-9.*u(i1,i2+2,i3,c2)+u(i1,i2+3,i3,
     & c2))/(60.*dr(1))
             vy = rxry*uur+rxsy*uus
               deriv(i1,i2,i3,n)=ux+vy
             end do
             end do
             end do
           else
             ! conservative, curvilinear, dim=2, order=6 
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                 deriv(i1,i2,i3,n)=((-(jac(i1-3,i2,i3)*(rsxy(i1-3,i2,
     & i3,0,0)*u(i1-3,i2,i3,c1)+rsxy(i1-3,i2,i3,0,1)*u(i1-3,i2,i3,c2))
     & )+9.*(jac(i1-2,i2,i3)*(rsxy(i1-2,i2,i3,0,0)*u(i1-2,i2,i3,c1)+
     & rsxy(i1-2,i2,i3,0,1)*u(i1-2,i2,i3,c2)))-45.*(jac(i1-1,i2,i3)*(
     & rsxy(i1-1,i2,i3,0,0)*u(i1-1,i2,i3,c1)+rsxy(i1-1,i2,i3,0,1)*u(
     & i1-1,i2,i3,c2)))+45.*(jac(i1+1,i2,i3)*(rsxy(i1+1,i2,i3,0,0)*u(
     & i1+1,i2,i3,c1)+rsxy(i1+1,i2,i3,0,1)*u(i1+1,i2,i3,c2)))-9.*(jac(
     & i1+2,i2,i3)*(rsxy(i1+2,i2,i3,0,0)*u(i1+2,i2,i3,c1)+rsxy(i1+2,
     & i2,i3,0,1)*u(i1+2,i2,i3,c2)))+(jac(i1+3,i2,i3)*(rsxy(i1+3,i2,
     & i3,0,0)*u(i1+3,i2,i3,c1)+rsxy(i1+3,i2,i3,0,1)*u(i1+3,i2,i3,c2))
     & ))/(60.*dr(0))+(-(jac(i1,i2-3,i3)*(rsxy(i1,i2-3,i3,1,0)*u(i1,
     & i2-3,i3,c1)+rsxy(i1,i2-3,i3,1,1)*u(i1,i2-3,i3,c2)))+9.*(jac(i1,
     & i2-2,i3)*(rsxy(i1,i2-2,i3,1,0)*u(i1,i2-2,i3,c1)+rsxy(i1,i2-2,
     & i3,1,1)*u(i1,i2-2,i3,c2)))-45.*(jac(i1,i2-1,i3)*(rsxy(i1,i2-1,
     & i3,1,0)*u(i1,i2-1,i3,c1)+rsxy(i1,i2-1,i3,1,1)*u(i1,i2-1,i3,c2))
     & )+45.*(jac(i1,i2+1,i3)*(rsxy(i1,i2+1,i3,1,0)*u(i1,i2+1,i3,c1)+
     & rsxy(i1,i2+1,i3,1,1)*u(i1,i2+1,i3,c2)))-9.*(jac(i1,i2+2,i3)*(
     & rsxy(i1,i2+2,i3,1,0)*u(i1,i2+2,i3,c1)+rsxy(i1,i2+2,i3,1,1)*u(
     & i1,i2+2,i3,c2)))+(jac(i1,i2+3,i3)*(rsxy(i1,i2+3,i3,1,0)*u(i1,
     & i2+3,i3,c1)+rsxy(i1,i2+3,i3,1,1)*u(i1,i2+3,i3,c2))))/(60.*dr(1)
     & ))/jac(i1,i2,i3)
                end do
                end do
                end do
           end if
        else if( order.eq.8 )then
c  *** Evaluate the divergence ***
           if( gridType.eq.rectangular )then
            ! Cartesian, dim=2, order=8 
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
               deriv(i1,i2,i3,n)=(3.*u(i1-4,i2,i3,c1)-32.*u(i1-3,i2,i3,
     & c1)+168.*u(i1-2,i2,i3,c1)-672.*u(i1-1,i2,i3,c1)+672.*u(i1+1,i2,
     & i3,c1)-168.*u(i1+2,i2,i3,c1)+32.*u(i1+3,i2,i3,c1)-3.*u(i1+4,i2,
     & i3,c1))/(840.*dx(0))+(3.*u(i1,i2-4,i3,c2)-32.*u(i1,i2-3,i3,c2)+
     & 168.*u(i1,i2-2,i3,c2)-672.*u(i1,i2-1,i3,c2)+672.*u(i1,i2+1,i3,
     & c2)-168.*u(i1,i2+2,i3,c2)+32.*u(i1,i2+3,i3,c2)-3.*u(i1,i2+4,i3,
     & c2))/(840.*dx(1))
             end do
             end do
             end do
           else if( derivType.eq.nonConservative )then
            ! Curvilinear, non-conservative, dim=2, order=8 
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
             ! order 4:
             !                                        2,8,MAXDERIV
             rxrx = rsxy(i1,i2,i3,0,0)
             rxsx = rsxy(i1,i2,i3,1,0)
             rxry = rsxy(i1,i2,i3,0,1)
             rxsy = rsxy(i1,i2,i3,1,1)
             !                                                      2,8,MAXDERIV
             uu = u(i1,i2,i3,c1)
             uur = (3.*u(i1-4,i2,i3,c1)-32.*u(i1-3,i2,i3,c1)+168.*u(i1-
     & 2,i2,i3,c1)-672.*u(i1-1,i2,i3,c1)+672.*u(i1+1,i2,i3,c1)-168.*u(
     & i1+2,i2,i3,c1)+32.*u(i1+3,i2,i3,c1)-3.*u(i1+4,i2,i3,c1))/(840.*
     & dr(0))
             uus = (3.*u(i1,i2-4,i3,c1)-32.*u(i1,i2-3,i3,c1)+168.*u(i1,
     & i2-2,i3,c1)-672.*u(i1,i2-1,i3,c1)+672.*u(i1,i2+1,i3,c1)-168.*u(
     & i1,i2+2,i3,c1)+32.*u(i1,i2+3,i3,c1)-3.*u(i1,i2+4,i3,c1))/(840.*
     & dr(1))
             ux = rxrx*uur+rxsx*uus
             uu = u(i1,i2,i3,c2)
             uur = (3.*u(i1-4,i2,i3,c2)-32.*u(i1-3,i2,i3,c2)+168.*u(i1-
     & 2,i2,i3,c2)-672.*u(i1-1,i2,i3,c2)+672.*u(i1+1,i2,i3,c2)-168.*u(
     & i1+2,i2,i3,c2)+32.*u(i1+3,i2,i3,c2)-3.*u(i1+4,i2,i3,c2))/(840.*
     & dr(0))
             uus = (3.*u(i1,i2-4,i3,c2)-32.*u(i1,i2-3,i3,c2)+168.*u(i1,
     & i2-2,i3,c2)-672.*u(i1,i2-1,i3,c2)+672.*u(i1,i2+1,i3,c2)-168.*u(
     & i1,i2+2,i3,c2)+32.*u(i1,i2+3,i3,c2)-3.*u(i1,i2+4,i3,c2))/(840.*
     & dr(1))
             vy = rxry*uur+rxsy*uus
               deriv(i1,i2,i3,n)=ux+vy
             end do
             end do
             end do
           else
             ! conservative, curvilinear, dim=2, order=8 
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                 deriv(i1,i2,i3,n)=((3.*(jac(i1-4,i2,i3)*(rsxy(i1-4,i2,
     & i3,0,0)*u(i1-4,i2,i3,c1)+rsxy(i1-4,i2,i3,0,1)*u(i1-4,i2,i3,c2))
     & )-32.*(jac(i1-3,i2,i3)*(rsxy(i1-3,i2,i3,0,0)*u(i1-3,i2,i3,c1)+
     & rsxy(i1-3,i2,i3,0,1)*u(i1-3,i2,i3,c2)))+168.*(jac(i1-2,i2,i3)*(
     & rsxy(i1-2,i2,i3,0,0)*u(i1-2,i2,i3,c1)+rsxy(i1-2,i2,i3,0,1)*u(
     & i1-2,i2,i3,c2)))-672.*(jac(i1-1,i2,i3)*(rsxy(i1-1,i2,i3,0,0)*u(
     & i1-1,i2,i3,c1)+rsxy(i1-1,i2,i3,0,1)*u(i1-1,i2,i3,c2)))+672.*(
     & jac(i1+1,i2,i3)*(rsxy(i1+1,i2,i3,0,0)*u(i1+1,i2,i3,c1)+rsxy(i1+
     & 1,i2,i3,0,1)*u(i1+1,i2,i3,c2)))-168.*(jac(i1+2,i2,i3)*(rsxy(i1+
     & 2,i2,i3,0,0)*u(i1+2,i2,i3,c1)+rsxy(i1+2,i2,i3,0,1)*u(i1+2,i2,
     & i3,c2)))+32.*(jac(i1+3,i2,i3)*(rsxy(i1+3,i2,i3,0,0)*u(i1+3,i2,
     & i3,c1)+rsxy(i1+3,i2,i3,0,1)*u(i1+3,i2,i3,c2)))-3.*(jac(i1+4,i2,
     & i3)*(rsxy(i1+4,i2,i3,0,0)*u(i1+4,i2,i3,c1)+rsxy(i1+4,i2,i3,0,1)
     & *u(i1+4,i2,i3,c2))))/(840.*dr(0))+(3.*(jac(i1,i2-4,i3)*(rsxy(
     & i1,i2-4,i3,1,0)*u(i1,i2-4,i3,c1)+rsxy(i1,i2-4,i3,1,1)*u(i1,i2-
     & 4,i3,c2)))-32.*(jac(i1,i2-3,i3)*(rsxy(i1,i2-3,i3,1,0)*u(i1,i2-
     & 3,i3,c1)+rsxy(i1,i2-3,i3,1,1)*u(i1,i2-3,i3,c2)))+168.*(jac(i1,
     & i2-2,i3)*(rsxy(i1,i2-2,i3,1,0)*u(i1,i2-2,i3,c1)+rsxy(i1,i2-2,
     & i3,1,1)*u(i1,i2-2,i3,c2)))-672.*(jac(i1,i2-1,i3)*(rsxy(i1,i2-1,
     & i3,1,0)*u(i1,i2-1,i3,c1)+rsxy(i1,i2-1,i3,1,1)*u(i1,i2-1,i3,c2))
     & )+672.*(jac(i1,i2+1,i3)*(rsxy(i1,i2+1,i3,1,0)*u(i1,i2+1,i3,c1)+
     & rsxy(i1,i2+1,i3,1,1)*u(i1,i2+1,i3,c2)))-168.*(jac(i1,i2+2,i3)*(
     & rsxy(i1,i2+2,i3,1,0)*u(i1,i2+2,i3,c1)+rsxy(i1,i2+2,i3,1,1)*u(
     & i1,i2+2,i3,c2)))+32.*(jac(i1,i2+3,i3)*(rsxy(i1,i2+3,i3,1,0)*u(
     & i1,i2+3,i3,c1)+rsxy(i1,i2+3,i3,1,1)*u(i1,i2+3,i3,c2)))-3.*(jac(
     & i1,i2+4,i3)*(rsxy(i1,i2+4,i3,1,0)*u(i1,i2+4,i3,c1)+rsxy(i1,i2+
     & 4,i3,1,1)*u(i1,i2+4,i3,c2))))/(840.*dr(1)))/jac(i1,i2,i3)
                end do
                end do
                end do
           end if
        else
          stop 6134
        end if

      else if( nd.eq.3 )then
        ! ********************************
        ! ************* 3D ***************
        ! ********************************

        if( order.eq.2 )then
c  *** Evaluate the divergence ***
           if( gridType.eq.rectangular )then
            ! Cartesian, dim=3, order=2 
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
               deriv(i1,i2,i3,n)=(-u(i1-1,i2,i3,c1)+u(i1+1,i2,i3,c1))/(
     & 2.*dx(0))+(-u(i1,i2-1,i3,c2)+u(i1,i2+1,i3,c2))/(2.*dx(1))+(-u(
     & i1,i2,i3-1,c3)+u(i1,i2,i3+1,c3))/(2.*dx(2))
             end do
             end do
             end do
           else if( derivType.eq.nonConservative )then
            ! Curvilinear, non-conservative, dim=3, order=2 
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
             ! order 4:
             !                                        3,2,MAXDERIV
             rxrx = rsxy(i1,i2,i3,0,0)
             rxsx = rsxy(i1,i2,i3,1,0)
             rxtx = rsxy(i1,i2,i3,2,0)
             rxry = rsxy(i1,i2,i3,0,1)
             rxsy = rsxy(i1,i2,i3,1,1)
             rxty = rsxy(i1,i2,i3,2,1)
             rxrz = rsxy(i1,i2,i3,0,2)
             rxsz = rsxy(i1,i2,i3,1,2)
             rxtz = rsxy(i1,i2,i3,2,2)
             !                                                      3,2,MAXDERIV
             uu = u(i1,i2,i3,c1)
             uur = (-u(i1-1,i2,i3,c1)+u(i1+1,i2,i3,c1))/(2.*dr(0))
             uus = (-u(i1,i2-1,i3,c1)+u(i1,i2+1,i3,c1))/(2.*dr(1))
             uut = (-u(i1,i2,i3-1,c1)+u(i1,i2,i3+1,c1))/(2.*dr(2))
             ux = rxrx*uur+rxsx*uus+rxtx*uut
             uu = u(i1,i2,i3,c2)
             uur = (-u(i1-1,i2,i3,c2)+u(i1+1,i2,i3,c2))/(2.*dr(0))
             uus = (-u(i1,i2-1,i3,c2)+u(i1,i2+1,i3,c2))/(2.*dr(1))
             uut = (-u(i1,i2,i3-1,c2)+u(i1,i2,i3+1,c2))/(2.*dr(2))
             vy = rxry*uur+rxsy*uus+rxty*uut
               uu = u(i1,i2,i3,c3)
               uur = (-u(i1-1,i2,i3,c3)+u(i1+1,i2,i3,c3))/(2.*dr(0))
               uus = (-u(i1,i2-1,i3,c3)+u(i1,i2+1,i3,c3))/(2.*dr(1))
               uut = (-u(i1,i2,i3-1,c3)+u(i1,i2,i3+1,c3))/(2.*dr(2))
               wz = rxrz*uur+rxsz*uus+rxtz*uut
               deriv(i1,i2,i3,n)=ux+vy+wz
             end do
             end do
             end do
           else
             ! conservative, curvilinear, dim=3, order=2 
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                 deriv(i1,i2,i3,n)=((-(jac(i1-1,i2,i3)*(rsxy(i1-1,i2,
     & i3,0,0)*u(i1-1,i2,i3,c1)+rsxy(i1-1,i2,i3,0,1)*u(i1-1,i2,i3,c2)+
     & rsxy(i1-1,i2,i3,0,2)*u(i1-1,i2,i3,c3)))+(jac(i1+1,i2,i3)*(rsxy(
     & i1+1,i2,i3,0,0)*u(i1+1,i2,i3,c1)+rsxy(i1+1,i2,i3,0,1)*u(i1+1,
     & i2,i3,c2)+rsxy(i1+1,i2,i3,0,2)*u(i1+1,i2,i3,c3))))/(2.*dr(0))+(
     & -(jac(i1,i2-1,i3)*(rsxy(i1,i2-1,i3,1,0)*u(i1,i2-1,i3,c1)+rsxy(
     & i1,i2-1,i3,1,1)*u(i1,i2-1,i3,c2)+rsxy(i1,i2-1,i3,1,2)*u(i1,i2-
     & 1,i3,c3)))+(jac(i1,i2+1,i3)*(rsxy(i1,i2+1,i3,1,0)*u(i1,i2+1,i3,
     & c1)+rsxy(i1,i2+1,i3,1,1)*u(i1,i2+1,i3,c2)+rsxy(i1,i2+1,i3,1,2)*
     & u(i1,i2+1,i3,c3))))/(2.*dr(1))+(-(jac(i1,i2,i3-1)*(rsxy(i1,i2,
     & i3-1,2,0)*u(i1,i2,i3-1,c1)+rsxy(i1,i2,i3-1,2,1)*u(i1,i2,i3-1,
     & c2)+rsxy(i1,i2,i3-1,2,2)*u(i1,i2,i3-1,c3)))+(jac(i1,i2,i3+1)*(
     & rsxy(i1,i2,i3+1,2,0)*u(i1,i2,i3+1,c1)+rsxy(i1,i2,i3+1,2,1)*u(
     & i1,i2,i3+1,c2)+rsxy(i1,i2,i3+1,2,2)*u(i1,i2,i3+1,c3))))/(2.*dr(
     & 2)))/jac(i1,i2,i3)
                end do
                end do
                end do
           end if
        else if( order.eq.4 )then
c  *** Evaluate the divergence ***
           if( gridType.eq.rectangular )then
            ! Cartesian, dim=3, order=4 
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
               deriv(i1,i2,i3,n)=(u(i1-2,i2,i3,c1)-8.*u(i1-1,i2,i3,c1)+
     & 8.*u(i1+1,i2,i3,c1)-u(i1+2,i2,i3,c1))/(12.*dx(0))+(u(i1,i2-2,
     & i3,c2)-8.*u(i1,i2-1,i3,c2)+8.*u(i1,i2+1,i3,c2)-u(i1,i2+2,i3,c2)
     & )/(12.*dx(1))+(u(i1,i2,i3-2,c3)-8.*u(i1,i2,i3-1,c3)+8.*u(i1,i2,
     & i3+1,c3)-u(i1,i2,i3+2,c3))/(12.*dx(2))
             end do
             end do
             end do
           else if( derivType.eq.nonConservative )then
            ! Curvilinear, non-conservative, dim=3, order=4 
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
             ! order 4:
             !                                        3,4,MAXDERIV
             rxrx = rsxy(i1,i2,i3,0,0)
             rxsx = rsxy(i1,i2,i3,1,0)
             rxtx = rsxy(i1,i2,i3,2,0)
             rxry = rsxy(i1,i2,i3,0,1)
             rxsy = rsxy(i1,i2,i3,1,1)
             rxty = rsxy(i1,i2,i3,2,1)
             rxrz = rsxy(i1,i2,i3,0,2)
             rxsz = rsxy(i1,i2,i3,1,2)
             rxtz = rsxy(i1,i2,i3,2,2)
             !                                                      3,4,MAXDERIV
             uu = u(i1,i2,i3,c1)
             uur = (u(i1-2,i2,i3,c1)-8.*u(i1-1,i2,i3,c1)+8.*u(i1+1,i2,
     & i3,c1)-u(i1+2,i2,i3,c1))/(12.*dr(0))
             uus = (u(i1,i2-2,i3,c1)-8.*u(i1,i2-1,i3,c1)+8.*u(i1,i2+1,
     & i3,c1)-u(i1,i2+2,i3,c1))/(12.*dr(1))
             uut = (u(i1,i2,i3-2,c1)-8.*u(i1,i2,i3-1,c1)+8.*u(i1,i2,i3+
     & 1,c1)-u(i1,i2,i3+2,c1))/(12.*dr(2))
             ux = rxrx*uur+rxsx*uus+rxtx*uut
             uu = u(i1,i2,i3,c2)
             uur = (u(i1-2,i2,i3,c2)-8.*u(i1-1,i2,i3,c2)+8.*u(i1+1,i2,
     & i3,c2)-u(i1+2,i2,i3,c2))/(12.*dr(0))
             uus = (u(i1,i2-2,i3,c2)-8.*u(i1,i2-1,i3,c2)+8.*u(i1,i2+1,
     & i3,c2)-u(i1,i2+2,i3,c2))/(12.*dr(1))
             uut = (u(i1,i2,i3-2,c2)-8.*u(i1,i2,i3-1,c2)+8.*u(i1,i2,i3+
     & 1,c2)-u(i1,i2,i3+2,c2))/(12.*dr(2))
             vy = rxry*uur+rxsy*uus+rxty*uut
               uu = u(i1,i2,i3,c3)
               uur = (u(i1-2,i2,i3,c3)-8.*u(i1-1,i2,i3,c3)+8.*u(i1+1,
     & i2,i3,c3)-u(i1+2,i2,i3,c3))/(12.*dr(0))
               uus = (u(i1,i2-2,i3,c3)-8.*u(i1,i2-1,i3,c3)+8.*u(i1,i2+
     & 1,i3,c3)-u(i1,i2+2,i3,c3))/(12.*dr(1))
               uut = (u(i1,i2,i3-2,c3)-8.*u(i1,i2,i3-1,c3)+8.*u(i1,i2,
     & i3+1,c3)-u(i1,i2,i3+2,c3))/(12.*dr(2))
               wz = rxrz*uur+rxsz*uus+rxtz*uut
               deriv(i1,i2,i3,n)=ux+vy+wz
             end do
             end do
             end do
           else
             ! conservative, curvilinear, dim=3, order=4 
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                 deriv(i1,i2,i3,n)=(((jac(i1-2,i2,i3)*(rsxy(i1-2,i2,i3,
     & 0,0)*u(i1-2,i2,i3,c1)+rsxy(i1-2,i2,i3,0,1)*u(i1-2,i2,i3,c2)+
     & rsxy(i1-2,i2,i3,0,2)*u(i1-2,i2,i3,c3)))-8.*(jac(i1-1,i2,i3)*(
     & rsxy(i1-1,i2,i3,0,0)*u(i1-1,i2,i3,c1)+rsxy(i1-1,i2,i3,0,1)*u(
     & i1-1,i2,i3,c2)+rsxy(i1-1,i2,i3,0,2)*u(i1-1,i2,i3,c3)))+8.*(jac(
     & i1+1,i2,i3)*(rsxy(i1+1,i2,i3,0,0)*u(i1+1,i2,i3,c1)+rsxy(i1+1,
     & i2,i3,0,1)*u(i1+1,i2,i3,c2)+rsxy(i1+1,i2,i3,0,2)*u(i1+1,i2,i3,
     & c3)))-(jac(i1+2,i2,i3)*(rsxy(i1+2,i2,i3,0,0)*u(i1+2,i2,i3,c1)+
     & rsxy(i1+2,i2,i3,0,1)*u(i1+2,i2,i3,c2)+rsxy(i1+2,i2,i3,0,2)*u(
     & i1+2,i2,i3,c3))))/(12.*dr(0))+((jac(i1,i2-2,i3)*(rsxy(i1,i2-2,
     & i3,1,0)*u(i1,i2-2,i3,c1)+rsxy(i1,i2-2,i3,1,1)*u(i1,i2-2,i3,c2)+
     & rsxy(i1,i2-2,i3,1,2)*u(i1,i2-2,i3,c3)))-8.*(jac(i1,i2-1,i3)*(
     & rsxy(i1,i2-1,i3,1,0)*u(i1,i2-1,i3,c1)+rsxy(i1,i2-1,i3,1,1)*u(
     & i1,i2-1,i3,c2)+rsxy(i1,i2-1,i3,1,2)*u(i1,i2-1,i3,c3)))+8.*(jac(
     & i1,i2+1,i3)*(rsxy(i1,i2+1,i3,1,0)*u(i1,i2+1,i3,c1)+rsxy(i1,i2+
     & 1,i3,1,1)*u(i1,i2+1,i3,c2)+rsxy(i1,i2+1,i3,1,2)*u(i1,i2+1,i3,
     & c3)))-(jac(i1,i2+2,i3)*(rsxy(i1,i2+2,i3,1,0)*u(i1,i2+2,i3,c1)+
     & rsxy(i1,i2+2,i3,1,1)*u(i1,i2+2,i3,c2)+rsxy(i1,i2+2,i3,1,2)*u(
     & i1,i2+2,i3,c3))))/(12.*dr(1))+((jac(i1,i2,i3-2)*(rsxy(i1,i2,i3-
     & 2,2,0)*u(i1,i2,i3-2,c1)+rsxy(i1,i2,i3-2,2,1)*u(i1,i2,i3-2,c2)+
     & rsxy(i1,i2,i3-2,2,2)*u(i1,i2,i3-2,c3)))-8.*(jac(i1,i2,i3-1)*(
     & rsxy(i1,i2,i3-1,2,0)*u(i1,i2,i3-1,c1)+rsxy(i1,i2,i3-1,2,1)*u(
     & i1,i2,i3-1,c2)+rsxy(i1,i2,i3-1,2,2)*u(i1,i2,i3-1,c3)))+8.*(jac(
     & i1,i2,i3+1)*(rsxy(i1,i2,i3+1,2,0)*u(i1,i2,i3+1,c1)+rsxy(i1,i2,
     & i3+1,2,1)*u(i1,i2,i3+1,c2)+rsxy(i1,i2,i3+1,2,2)*u(i1,i2,i3+1,
     & c3)))-(jac(i1,i2,i3+2)*(rsxy(i1,i2,i3+2,2,0)*u(i1,i2,i3+2,c1)+
     & rsxy(i1,i2,i3+2,2,1)*u(i1,i2,i3+2,c2)+rsxy(i1,i2,i3+2,2,2)*u(
     & i1,i2,i3+2,c3))))/(12.*dr(2)))/jac(i1,i2,i3)
                end do
                end do
                end do
           end if
        else if( order.eq.6 )then
c  *** Evaluate the divergence ***
           if( gridType.eq.rectangular )then
            ! Cartesian, dim=3, order=6 
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
               deriv(i1,i2,i3,n)=(-u(i1-3,i2,i3,c1)+9.*u(i1-2,i2,i3,c1)
     & -45.*u(i1-1,i2,i3,c1)+45.*u(i1+1,i2,i3,c1)-9.*u(i1+2,i2,i3,c1)+
     & u(i1+3,i2,i3,c1))/(60.*dx(0))+(-u(i1,i2-3,i3,c2)+9.*u(i1,i2-2,
     & i3,c2)-45.*u(i1,i2-1,i3,c2)+45.*u(i1,i2+1,i3,c2)-9.*u(i1,i2+2,
     & i3,c2)+u(i1,i2+3,i3,c2))/(60.*dx(1))+(-u(i1,i2,i3-3,c3)+9.*u(
     & i1,i2,i3-2,c3)-45.*u(i1,i2,i3-1,c3)+45.*u(i1,i2,i3+1,c3)-9.*u(
     & i1,i2,i3+2,c3)+u(i1,i2,i3+3,c3))/(60.*dx(2))
             end do
             end do
             end do
           else if( derivType.eq.nonConservative )then
            ! Curvilinear, non-conservative, dim=3, order=6 
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
             ! order 4:
             !                                        3,6,MAXDERIV
             rxrx = rsxy(i1,i2,i3,0,0)
             rxsx = rsxy(i1,i2,i3,1,0)
             rxtx = rsxy(i1,i2,i3,2,0)
             rxry = rsxy(i1,i2,i3,0,1)
             rxsy = rsxy(i1,i2,i3,1,1)
             rxty = rsxy(i1,i2,i3,2,1)
             rxrz = rsxy(i1,i2,i3,0,2)
             rxsz = rsxy(i1,i2,i3,1,2)
             rxtz = rsxy(i1,i2,i3,2,2)
             !                                                      3,6,MAXDERIV
             uu = u(i1,i2,i3,c1)
             uur = (-u(i1-3,i2,i3,c1)+9.*u(i1-2,i2,i3,c1)-45.*u(i1-1,
     & i2,i3,c1)+45.*u(i1+1,i2,i3,c1)-9.*u(i1+2,i2,i3,c1)+u(i1+3,i2,
     & i3,c1))/(60.*dr(0))
             uus = (-u(i1,i2-3,i3,c1)+9.*u(i1,i2-2,i3,c1)-45.*u(i1,i2-
     & 1,i3,c1)+45.*u(i1,i2+1,i3,c1)-9.*u(i1,i2+2,i3,c1)+u(i1,i2+3,i3,
     & c1))/(60.*dr(1))
             uut = (-u(i1,i2,i3-3,c1)+9.*u(i1,i2,i3-2,c1)-45.*u(i1,i2,
     & i3-1,c1)+45.*u(i1,i2,i3+1,c1)-9.*u(i1,i2,i3+2,c1)+u(i1,i2,i3+3,
     & c1))/(60.*dr(2))
             ux = rxrx*uur+rxsx*uus+rxtx*uut
             uu = u(i1,i2,i3,c2)
             uur = (-u(i1-3,i2,i3,c2)+9.*u(i1-2,i2,i3,c2)-45.*u(i1-1,
     & i2,i3,c2)+45.*u(i1+1,i2,i3,c2)-9.*u(i1+2,i2,i3,c2)+u(i1+3,i2,
     & i3,c2))/(60.*dr(0))
             uus = (-u(i1,i2-3,i3,c2)+9.*u(i1,i2-2,i3,c2)-45.*u(i1,i2-
     & 1,i3,c2)+45.*u(i1,i2+1,i3,c2)-9.*u(i1,i2+2,i3,c2)+u(i1,i2+3,i3,
     & c2))/(60.*dr(1))
             uut = (-u(i1,i2,i3-3,c2)+9.*u(i1,i2,i3-2,c2)-45.*u(i1,i2,
     & i3-1,c2)+45.*u(i1,i2,i3+1,c2)-9.*u(i1,i2,i3+2,c2)+u(i1,i2,i3+3,
     & c2))/(60.*dr(2))
             vy = rxry*uur+rxsy*uus+rxty*uut
               uu = u(i1,i2,i3,c3)
               uur = (-u(i1-3,i2,i3,c3)+9.*u(i1-2,i2,i3,c3)-45.*u(i1-1,
     & i2,i3,c3)+45.*u(i1+1,i2,i3,c3)-9.*u(i1+2,i2,i3,c3)+u(i1+3,i2,
     & i3,c3))/(60.*dr(0))
               uus = (-u(i1,i2-3,i3,c3)+9.*u(i1,i2-2,i3,c3)-45.*u(i1,
     & i2-1,i3,c3)+45.*u(i1,i2+1,i3,c3)-9.*u(i1,i2+2,i3,c3)+u(i1,i2+3,
     & i3,c3))/(60.*dr(1))
               uut = (-u(i1,i2,i3-3,c3)+9.*u(i1,i2,i3-2,c3)-45.*u(i1,
     & i2,i3-1,c3)+45.*u(i1,i2,i3+1,c3)-9.*u(i1,i2,i3+2,c3)+u(i1,i2,
     & i3+3,c3))/(60.*dr(2))
               wz = rxrz*uur+rxsz*uus+rxtz*uut
               deriv(i1,i2,i3,n)=ux+vy+wz
             end do
             end do
             end do
           else
             ! conservative, curvilinear, dim=3, order=6 
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                 deriv(i1,i2,i3,n)=((-(jac(i1-3,i2,i3)*(rsxy(i1-3,i2,
     & i3,0,0)*u(i1-3,i2,i3,c1)+rsxy(i1-3,i2,i3,0,1)*u(i1-3,i2,i3,c2)+
     & rsxy(i1-3,i2,i3,0,2)*u(i1-3,i2,i3,c3)))+9.*(jac(i1-2,i2,i3)*(
     & rsxy(i1-2,i2,i3,0,0)*u(i1-2,i2,i3,c1)+rsxy(i1-2,i2,i3,0,1)*u(
     & i1-2,i2,i3,c2)+rsxy(i1-2,i2,i3,0,2)*u(i1-2,i2,i3,c3)))-45.*(
     & jac(i1-1,i2,i3)*(rsxy(i1-1,i2,i3,0,0)*u(i1-1,i2,i3,c1)+rsxy(i1-
     & 1,i2,i3,0,1)*u(i1-1,i2,i3,c2)+rsxy(i1-1,i2,i3,0,2)*u(i1-1,i2,
     & i3,c3)))+45.*(jac(i1+1,i2,i3)*(rsxy(i1+1,i2,i3,0,0)*u(i1+1,i2,
     & i3,c1)+rsxy(i1+1,i2,i3,0,1)*u(i1+1,i2,i3,c2)+rsxy(i1+1,i2,i3,0,
     & 2)*u(i1+1,i2,i3,c3)))-9.*(jac(i1+2,i2,i3)*(rsxy(i1+2,i2,i3,0,0)
     & *u(i1+2,i2,i3,c1)+rsxy(i1+2,i2,i3,0,1)*u(i1+2,i2,i3,c2)+rsxy(
     & i1+2,i2,i3,0,2)*u(i1+2,i2,i3,c3)))+(jac(i1+3,i2,i3)*(rsxy(i1+3,
     & i2,i3,0,0)*u(i1+3,i2,i3,c1)+rsxy(i1+3,i2,i3,0,1)*u(i1+3,i2,i3,
     & c2)+rsxy(i1+3,i2,i3,0,2)*u(i1+3,i2,i3,c3))))/(60.*dr(0))+(-(
     & jac(i1,i2-3,i3)*(rsxy(i1,i2-3,i3,1,0)*u(i1,i2-3,i3,c1)+rsxy(i1,
     & i2-3,i3,1,1)*u(i1,i2-3,i3,c2)+rsxy(i1,i2-3,i3,1,2)*u(i1,i2-3,
     & i3,c3)))+9.*(jac(i1,i2-2,i3)*(rsxy(i1,i2-2,i3,1,0)*u(i1,i2-2,
     & i3,c1)+rsxy(i1,i2-2,i3,1,1)*u(i1,i2-2,i3,c2)+rsxy(i1,i2-2,i3,1,
     & 2)*u(i1,i2-2,i3,c3)))-45.*(jac(i1,i2-1,i3)*(rsxy(i1,i2-1,i3,1,
     & 0)*u(i1,i2-1,i3,c1)+rsxy(i1,i2-1,i3,1,1)*u(i1,i2-1,i3,c2)+rsxy(
     & i1,i2-1,i3,1,2)*u(i1,i2-1,i3,c3)))+45.*(jac(i1,i2+1,i3)*(rsxy(
     & i1,i2+1,i3,1,0)*u(i1,i2+1,i3,c1)+rsxy(i1,i2+1,i3,1,1)*u(i1,i2+
     & 1,i3,c2)+rsxy(i1,i2+1,i3,1,2)*u(i1,i2+1,i3,c3)))-9.*(jac(i1,i2+
     & 2,i3)*(rsxy(i1,i2+2,i3,1,0)*u(i1,i2+2,i3,c1)+rsxy(i1,i2+2,i3,1,
     & 1)*u(i1,i2+2,i3,c2)+rsxy(i1,i2+2,i3,1,2)*u(i1,i2+2,i3,c3)))+(
     & jac(i1,i2+3,i3)*(rsxy(i1,i2+3,i3,1,0)*u(i1,i2+3,i3,c1)+rsxy(i1,
     & i2+3,i3,1,1)*u(i1,i2+3,i3,c2)+rsxy(i1,i2+3,i3,1,2)*u(i1,i2+3,
     & i3,c3))))/(60.*dr(1))+(-(jac(i1,i2,i3-3)*(rsxy(i1,i2,i3-3,2,0)*
     & u(i1,i2,i3-3,c1)+rsxy(i1,i2,i3-3,2,1)*u(i1,i2,i3-3,c2)+rsxy(i1,
     & i2,i3-3,2,2)*u(i1,i2,i3-3,c3)))+9.*(jac(i1,i2,i3-2)*(rsxy(i1,
     & i2,i3-2,2,0)*u(i1,i2,i3-2,c1)+rsxy(i1,i2,i3-2,2,1)*u(i1,i2,i3-
     & 2,c2)+rsxy(i1,i2,i3-2,2,2)*u(i1,i2,i3-2,c3)))-45.*(jac(i1,i2,
     & i3-1)*(rsxy(i1,i2,i3-1,2,0)*u(i1,i2,i3-1,c1)+rsxy(i1,i2,i3-1,2,
     & 1)*u(i1,i2,i3-1,c2)+rsxy(i1,i2,i3-1,2,2)*u(i1,i2,i3-1,c3)))+
     & 45.*(jac(i1,i2,i3+1)*(rsxy(i1,i2,i3+1,2,0)*u(i1,i2,i3+1,c1)+
     & rsxy(i1,i2,i3+1,2,1)*u(i1,i2,i3+1,c2)+rsxy(i1,i2,i3+1,2,2)*u(
     & i1,i2,i3+1,c3)))-9.*(jac(i1,i2,i3+2)*(rsxy(i1,i2,i3+2,2,0)*u(
     & i1,i2,i3+2,c1)+rsxy(i1,i2,i3+2,2,1)*u(i1,i2,i3+2,c2)+rsxy(i1,
     & i2,i3+2,2,2)*u(i1,i2,i3+2,c3)))+(jac(i1,i2,i3+3)*(rsxy(i1,i2,
     & i3+3,2,0)*u(i1,i2,i3+3,c1)+rsxy(i1,i2,i3+3,2,1)*u(i1,i2,i3+3,
     & c2)+rsxy(i1,i2,i3+3,2,2)*u(i1,i2,i3+3,c3))))/(60.*dr(2)))/jac(
     & i1,i2,i3)
                end do
                end do
                end do
           end if
        else if( order.eq.8 )then
c  *** Evaluate the divergence ***
           if( gridType.eq.rectangular )then
            ! Cartesian, dim=3, order=8 
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
               deriv(i1,i2,i3,n)=(3.*u(i1-4,i2,i3,c1)-32.*u(i1-3,i2,i3,
     & c1)+168.*u(i1-2,i2,i3,c1)-672.*u(i1-1,i2,i3,c1)+672.*u(i1+1,i2,
     & i3,c1)-168.*u(i1+2,i2,i3,c1)+32.*u(i1+3,i2,i3,c1)-3.*u(i1+4,i2,
     & i3,c1))/(840.*dx(0))+(3.*u(i1,i2-4,i3,c2)-32.*u(i1,i2-3,i3,c2)+
     & 168.*u(i1,i2-2,i3,c2)-672.*u(i1,i2-1,i3,c2)+672.*u(i1,i2+1,i3,
     & c2)-168.*u(i1,i2+2,i3,c2)+32.*u(i1,i2+3,i3,c2)-3.*u(i1,i2+4,i3,
     & c2))/(840.*dx(1))+(3.*u(i1,i2,i3-4,c3)-32.*u(i1,i2,i3-3,c3)+
     & 168.*u(i1,i2,i3-2,c3)-672.*u(i1,i2,i3-1,c3)+672.*u(i1,i2,i3+1,
     & c3)-168.*u(i1,i2,i3+2,c3)+32.*u(i1,i2,i3+3,c3)-3.*u(i1,i2,i3+4,
     & c3))/(840.*dx(2))
             end do
             end do
             end do
           else if( derivType.eq.nonConservative )then
            ! Curvilinear, non-conservative, dim=3, order=8 
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
             ! order 4:
             !                                        3,8,MAXDERIV
             rxrx = rsxy(i1,i2,i3,0,0)
             rxsx = rsxy(i1,i2,i3,1,0)
             rxtx = rsxy(i1,i2,i3,2,0)
             rxry = rsxy(i1,i2,i3,0,1)
             rxsy = rsxy(i1,i2,i3,1,1)
             rxty = rsxy(i1,i2,i3,2,1)
             rxrz = rsxy(i1,i2,i3,0,2)
             rxsz = rsxy(i1,i2,i3,1,2)
             rxtz = rsxy(i1,i2,i3,2,2)
             !                                                      3,8,MAXDERIV
             uu = u(i1,i2,i3,c1)
             uur = (3.*u(i1-4,i2,i3,c1)-32.*u(i1-3,i2,i3,c1)+168.*u(i1-
     & 2,i2,i3,c1)-672.*u(i1-1,i2,i3,c1)+672.*u(i1+1,i2,i3,c1)-168.*u(
     & i1+2,i2,i3,c1)+32.*u(i1+3,i2,i3,c1)-3.*u(i1+4,i2,i3,c1))/(840.*
     & dr(0))
             uus = (3.*u(i1,i2-4,i3,c1)-32.*u(i1,i2-3,i3,c1)+168.*u(i1,
     & i2-2,i3,c1)-672.*u(i1,i2-1,i3,c1)+672.*u(i1,i2+1,i3,c1)-168.*u(
     & i1,i2+2,i3,c1)+32.*u(i1,i2+3,i3,c1)-3.*u(i1,i2+4,i3,c1))/(840.*
     & dr(1))
             uut = (3.*u(i1,i2,i3-4,c1)-32.*u(i1,i2,i3-3,c1)+168.*u(i1,
     & i2,i3-2,c1)-672.*u(i1,i2,i3-1,c1)+672.*u(i1,i2,i3+1,c1)-168.*u(
     & i1,i2,i3+2,c1)+32.*u(i1,i2,i3+3,c1)-3.*u(i1,i2,i3+4,c1))/(840.*
     & dr(2))
             ux = rxrx*uur+rxsx*uus+rxtx*uut
             uu = u(i1,i2,i3,c2)
             uur = (3.*u(i1-4,i2,i3,c2)-32.*u(i1-3,i2,i3,c2)+168.*u(i1-
     & 2,i2,i3,c2)-672.*u(i1-1,i2,i3,c2)+672.*u(i1+1,i2,i3,c2)-168.*u(
     & i1+2,i2,i3,c2)+32.*u(i1+3,i2,i3,c2)-3.*u(i1+4,i2,i3,c2))/(840.*
     & dr(0))
             uus = (3.*u(i1,i2-4,i3,c2)-32.*u(i1,i2-3,i3,c2)+168.*u(i1,
     & i2-2,i3,c2)-672.*u(i1,i2-1,i3,c2)+672.*u(i1,i2+1,i3,c2)-168.*u(
     & i1,i2+2,i3,c2)+32.*u(i1,i2+3,i3,c2)-3.*u(i1,i2+4,i3,c2))/(840.*
     & dr(1))
             uut = (3.*u(i1,i2,i3-4,c2)-32.*u(i1,i2,i3-3,c2)+168.*u(i1,
     & i2,i3-2,c2)-672.*u(i1,i2,i3-1,c2)+672.*u(i1,i2,i3+1,c2)-168.*u(
     & i1,i2,i3+2,c2)+32.*u(i1,i2,i3+3,c2)-3.*u(i1,i2,i3+4,c2))/(840.*
     & dr(2))
             vy = rxry*uur+rxsy*uus+rxty*uut
               uu = u(i1,i2,i3,c3)
               uur = (3.*u(i1-4,i2,i3,c3)-32.*u(i1-3,i2,i3,c3)+168.*u(
     & i1-2,i2,i3,c3)-672.*u(i1-1,i2,i3,c3)+672.*u(i1+1,i2,i3,c3)-
     & 168.*u(i1+2,i2,i3,c3)+32.*u(i1+3,i2,i3,c3)-3.*u(i1+4,i2,i3,c3))
     & /(840.*dr(0))
               uus = (3.*u(i1,i2-4,i3,c3)-32.*u(i1,i2-3,i3,c3)+168.*u(
     & i1,i2-2,i3,c3)-672.*u(i1,i2-1,i3,c3)+672.*u(i1,i2+1,i3,c3)-
     & 168.*u(i1,i2+2,i3,c3)+32.*u(i1,i2+3,i3,c3)-3.*u(i1,i2+4,i3,c3))
     & /(840.*dr(1))
               uut = (3.*u(i1,i2,i3-4,c3)-32.*u(i1,i2,i3-3,c3)+168.*u(
     & i1,i2,i3-2,c3)-672.*u(i1,i2,i3-1,c3)+672.*u(i1,i2,i3+1,c3)-
     & 168.*u(i1,i2,i3+2,c3)+32.*u(i1,i2,i3+3,c3)-3.*u(i1,i2,i3+4,c3))
     & /(840.*dr(2))
               wz = rxrz*uur+rxsz*uus+rxtz*uut
               deriv(i1,i2,i3,n)=ux+vy+wz
             end do
             end do
             end do
           else
             ! conservative, curvilinear, dim=3, order=8 
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                 deriv(i1,i2,i3,n)=((3.*(jac(i1-4,i2,i3)*(rsxy(i1-4,i2,
     & i3,0,0)*u(i1-4,i2,i3,c1)+rsxy(i1-4,i2,i3,0,1)*u(i1-4,i2,i3,c2)+
     & rsxy(i1-4,i2,i3,0,2)*u(i1-4,i2,i3,c3)))-32.*(jac(i1-3,i2,i3)*(
     & rsxy(i1-3,i2,i3,0,0)*u(i1-3,i2,i3,c1)+rsxy(i1-3,i2,i3,0,1)*u(
     & i1-3,i2,i3,c2)+rsxy(i1-3,i2,i3,0,2)*u(i1-3,i2,i3,c3)))+168.*(
     & jac(i1-2,i2,i3)*(rsxy(i1-2,i2,i3,0,0)*u(i1-2,i2,i3,c1)+rsxy(i1-
     & 2,i2,i3,0,1)*u(i1-2,i2,i3,c2)+rsxy(i1-2,i2,i3,0,2)*u(i1-2,i2,
     & i3,c3)))-672.*(jac(i1-1,i2,i3)*(rsxy(i1-1,i2,i3,0,0)*u(i1-1,i2,
     & i3,c1)+rsxy(i1-1,i2,i3,0,1)*u(i1-1,i2,i3,c2)+rsxy(i1-1,i2,i3,0,
     & 2)*u(i1-1,i2,i3,c3)))+672.*(jac(i1+1,i2,i3)*(rsxy(i1+1,i2,i3,0,
     & 0)*u(i1+1,i2,i3,c1)+rsxy(i1+1,i2,i3,0,1)*u(i1+1,i2,i3,c2)+rsxy(
     & i1+1,i2,i3,0,2)*u(i1+1,i2,i3,c3)))-168.*(jac(i1+2,i2,i3)*(rsxy(
     & i1+2,i2,i3,0,0)*u(i1+2,i2,i3,c1)+rsxy(i1+2,i2,i3,0,1)*u(i1+2,
     & i2,i3,c2)+rsxy(i1+2,i2,i3,0,2)*u(i1+2,i2,i3,c3)))+32.*(jac(i1+
     & 3,i2,i3)*(rsxy(i1+3,i2,i3,0,0)*u(i1+3,i2,i3,c1)+rsxy(i1+3,i2,
     & i3,0,1)*u(i1+3,i2,i3,c2)+rsxy(i1+3,i2,i3,0,2)*u(i1+3,i2,i3,c3))
     & )-3.*(jac(i1+4,i2,i3)*(rsxy(i1+4,i2,i3,0,0)*u(i1+4,i2,i3,c1)+
     & rsxy(i1+4,i2,i3,0,1)*u(i1+4,i2,i3,c2)+rsxy(i1+4,i2,i3,0,2)*u(
     & i1+4,i2,i3,c3))))/(840.*dr(0))+(3.*(jac(i1,i2-4,i3)*(rsxy(i1,
     & i2-4,i3,1,0)*u(i1,i2-4,i3,c1)+rsxy(i1,i2-4,i3,1,1)*u(i1,i2-4,
     & i3,c2)+rsxy(i1,i2-4,i3,1,2)*u(i1,i2-4,i3,c3)))-32.*(jac(i1,i2-
     & 3,i3)*(rsxy(i1,i2-3,i3,1,0)*u(i1,i2-3,i3,c1)+rsxy(i1,i2-3,i3,1,
     & 1)*u(i1,i2-3,i3,c2)+rsxy(i1,i2-3,i3,1,2)*u(i1,i2-3,i3,c3)))+
     & 168.*(jac(i1,i2-2,i3)*(rsxy(i1,i2-2,i3,1,0)*u(i1,i2-2,i3,c1)+
     & rsxy(i1,i2-2,i3,1,1)*u(i1,i2-2,i3,c2)+rsxy(i1,i2-2,i3,1,2)*u(
     & i1,i2-2,i3,c3)))-672.*(jac(i1,i2-1,i3)*(rsxy(i1,i2-1,i3,1,0)*u(
     & i1,i2-1,i3,c1)+rsxy(i1,i2-1,i3,1,1)*u(i1,i2-1,i3,c2)+rsxy(i1,
     & i2-1,i3,1,2)*u(i1,i2-1,i3,c3)))+672.*(jac(i1,i2+1,i3)*(rsxy(i1,
     & i2+1,i3,1,0)*u(i1,i2+1,i3,c1)+rsxy(i1,i2+1,i3,1,1)*u(i1,i2+1,
     & i3,c2)+rsxy(i1,i2+1,i3,1,2)*u(i1,i2+1,i3,c3)))-168.*(jac(i1,i2+
     & 2,i3)*(rsxy(i1,i2+2,i3,1,0)*u(i1,i2+2,i3,c1)+rsxy(i1,i2+2,i3,1,
     & 1)*u(i1,i2+2,i3,c2)+rsxy(i1,i2+2,i3,1,2)*u(i1,i2+2,i3,c3)))+
     & 32.*(jac(i1,i2+3,i3)*(rsxy(i1,i2+3,i3,1,0)*u(i1,i2+3,i3,c1)+
     & rsxy(i1,i2+3,i3,1,1)*u(i1,i2+3,i3,c2)+rsxy(i1,i2+3,i3,1,2)*u(
     & i1,i2+3,i3,c3)))-3.*(jac(i1,i2+4,i3)*(rsxy(i1,i2+4,i3,1,0)*u(
     & i1,i2+4,i3,c1)+rsxy(i1,i2+4,i3,1,1)*u(i1,i2+4,i3,c2)+rsxy(i1,
     & i2+4,i3,1,2)*u(i1,i2+4,i3,c3))))/(840.*dr(1))+(3.*(jac(i1,i2,
     & i3-4)*(rsxy(i1,i2,i3-4,2,0)*u(i1,i2,i3-4,c1)+rsxy(i1,i2,i3-4,2,
     & 1)*u(i1,i2,i3-4,c2)+rsxy(i1,i2,i3-4,2,2)*u(i1,i2,i3-4,c3)))-
     & 32.*(jac(i1,i2,i3-3)*(rsxy(i1,i2,i3-3,2,0)*u(i1,i2,i3-3,c1)+
     & rsxy(i1,i2,i3-3,2,1)*u(i1,i2,i3-3,c2)+rsxy(i1,i2,i3-3,2,2)*u(
     & i1,i2,i3-3,c3)))+168.*(jac(i1,i2,i3-2)*(rsxy(i1,i2,i3-2,2,0)*u(
     & i1,i2,i3-2,c1)+rsxy(i1,i2,i3-2,2,1)*u(i1,i2,i3-2,c2)+rsxy(i1,
     & i2,i3-2,2,2)*u(i1,i2,i3-2,c3)))-672.*(jac(i1,i2,i3-1)*(rsxy(i1,
     & i2,i3-1,2,0)*u(i1,i2,i3-1,c1)+rsxy(i1,i2,i3-1,2,1)*u(i1,i2,i3-
     & 1,c2)+rsxy(i1,i2,i3-1,2,2)*u(i1,i2,i3-1,c3)))+672.*(jac(i1,i2,
     & i3+1)*(rsxy(i1,i2,i3+1,2,0)*u(i1,i2,i3+1,c1)+rsxy(i1,i2,i3+1,2,
     & 1)*u(i1,i2,i3+1,c2)+rsxy(i1,i2,i3+1,2,2)*u(i1,i2,i3+1,c3)))-
     & 168.*(jac(i1,i2,i3+2)*(rsxy(i1,i2,i3+2,2,0)*u(i1,i2,i3+2,c1)+
     & rsxy(i1,i2,i3+2,2,1)*u(i1,i2,i3+2,c2)+rsxy(i1,i2,i3+2,2,2)*u(
     & i1,i2,i3+2,c3)))+32.*(jac(i1,i2,i3+3)*(rsxy(i1,i2,i3+3,2,0)*u(
     & i1,i2,i3+3,c1)+rsxy(i1,i2,i3+3,2,1)*u(i1,i2,i3+3,c2)+rsxy(i1,
     & i2,i3+3,2,2)*u(i1,i2,i3+3,c3)))-3.*(jac(i1,i2,i3+4)*(rsxy(i1,
     & i2,i3+4,2,0)*u(i1,i2,i3+4,c1)+rsxy(i1,i2,i3+4,2,1)*u(i1,i2,i3+
     & 4,c2)+rsxy(i1,i2,i3+4,2,2)*u(i1,i2,i3+4,c3))))/(840.*dr(2)))
     & /jac(i1,i2,i3)
                end do
                end do
                end do
           end if
        else
          stop 6134
        end if
      else
        stop 11
      end if

c*      ! Cartesian:
c*      beginLoops()
c*        ! getDuDx2(u,rsxy,ux)
c*
c*        deriv(i1,i2,i3,0)=ux4(i1,i2,i3,c1)+uy4(i1,i2,i3,c2)
c*      endLoops()
c*
c*      ! Curvilinear, non-conservative
c*      beginLoops()
c*        ! order 4:
c*        !                                        DIM,ORDER,MAXDERIV
c*        evalJacobianDerivatives(rsxy,i1,i2,i3,rx,2,4,0)
c*
c*
c*        !                                                      DIM,ORDER,MAXDERIV
c*        evalParametricDerivativesComponents1(u,i1,i2,i3,c1, uu,2,4,1)
c*        getDuDx2(uu,rx,ux)
c*
c*        evalParametricDerivativesComponents1(u,i1,i2,i3,c2, uu,2,4,1)
c*        getDuDx2(uu,rx,vy)
c*
c*        deriv(i1,i2,i3,0)=ux+vy
c*      endLoops()
c*
c*     
c*#defineMacro u1(i1,i2,i3) (jac(i1,i2,i3)*(rsxy(i1,i2,i3,0,0)*u(i1,i2,i3,c1)+rsxy(i1,i2,i3,0,1)*u(i1,i2,i3,c2)))
c*#defineMacro u2(i1,i2,i3) (jac(i1,i2,i3)*(rsxy(i1,i2,i3,1,0)*u(i1,i2,i3,c1)+rsxy(i1,i2,i3,1,1)*u(i1,i2,i3,c2)))
c*
c*      ! conservative, curvilinear
c*      beginLoops()
c*        deriv(i1,i2,i3,0)=(u1r4(i1,i2,i3)+u2s4(i1,i2,i3))/jac(i1,i2,i3)
c*      endLoops()
c*


      return
      end



