! This file automatically generated from radEval.bf with bpp.
! -- evaluate the radiation BC's


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

! defineParametricDerivativeMacros(u,dr,dx,DIM,ORDER,COMPONENTS,MAXDERIV)

! 2d, order=4, components=1, derivatives=2:
! *************** 0 components *************
! *************** 1 components *************
! *************** 2 components *************
! 2d, order=2, components=1, derivatives=4:
! *************** 0 components *************
! *************** 1 components *************
! *************** 2 components *************

! *************** 0 components *************
! *************** 1 components *************
! *************** 2 components *************
! *************** 0 components *************
! *************** 1 components *************
! *************** 2 components *************

! 2D, order=4, components=2, 1-derivative
! *************** 0 components *************
! *************** 1 components *************
! *************** 2 components *************
! 2d, odrer=2, components=2, 3-derivatives
! *************** 0 components *************
! *************** 1 components *************
! *************** 2 components *************






      subroutine radEval( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     & gridIndexRange, u1, u, xy, rsxy, boundaryCondition, md1a,md1b,
     & md2a,md2b,huv, sd1a,sd1b,sd2a,sd2b,sd3a,sd3b,sd4a,sd4b,uSave,
     & ipar, rpar, ierr )
! ===================================================================================
!  Radition boundary conditions for Maxwell's Equations.
!      
!     Apply the BC of the form  u.t + u.n + H(u) = 0
!
!
!  huv(i,m,n) : Kernel and its derivatives, i=tangential index, m=derivative, n=component
!
! ===================================================================================

      implicit none

      integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, n1a,n1b,n2a,n2b,n3a,
     & n3b,na,nb, md1a,md1b,md2a,md2b,currentTimeLevel,
     & numberOfTimeLevels,sd1a,sd1b,sd2a,sd2b,sd3a,sd3b,sd4a,sd4b,ierr

      real u1(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real uSave(sd1a:sd1b,sd2a:sd2b,sd3a:sd3b,sd4a:sd4b)

      real huv(md1a:md1b,md2a:md2b,0:*)

      integer gridIndexRange,boundaryCondition
      integer ipar(0:*)
      real rpar(0:*)
!     --- local variables ----

      real dx(0:2),dr(0:2)
      real t,dt,eps,mu,c
      integer side,axis,gridType,orderOfAccuracy,grid,kernelType
      integer debug,i1,i2,i3,is1,is2,is3,im,i,ii
!      integer ex,ey,ez,hx,hy,hz
      integer nn1a,nn1b,nn2a,nn2b,nn3a,nn3b, m1a,m1b,m2a,m2b,m3a,m3b,
     & numGhost
      integer mm1,mm2,mm3,mm4
      integer kx,ky,kxx,kxy,kyy,kxxx,kxxy,kxyy,kyyy
      real ux,uxx,uyy,uLap,uxxx,uxxy,uxyy,uyyy,uLapx, uxxxx,uxxxy,
     & uxxyy,uxyyy,uyyyy, uLapSq,uLapxx,uLapyy
      real uy,uty,uxy
      real utxyy,ut,utt,utx,uttt,uttx,utxx,utttt, utttx,uttxx,utxxx
      real uxxri,uxyri,uyyri
      real utxy,utyy,utty
      real uxxxri,uxxyri,uxyyri,uyyyri
      real utxxy,utyyy, uttxy,uttyy, uttty

      real um1,um2  ,h,alpha
      real utrue
      real v,vt,tm,vtt,vttt,vtttt, vtx, vx
      real hu,hux,huxx,huxxx,huyy,huxy,huLap,huxyy,huxxy,huyyy
      real huyyi,huxyyi
      real hx,hy, huy, z0
      real ogf
      real ep ! holds the pointer to the TZ function

      integer i1m, i2m, np1, np2
      real r,an1,an2,aNorm,uri,uxri,uyri

      integer rectangular,curvilinear
      parameter(rectangular=0,curvilinear=1)

      integer planar,slab,cylindrical,spherical
      parameter(planar=0,slab=1,cylindrical=2,spherical=3 )

      !  declareTemporaryVariables(DIM,MAXDERIV)
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

      ! declareParametricDerivativeVariables(v,DIM)
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

       real aj4rx,aj4rxr,aj4rxs,aj4rxt,aj4rxrr,aj4rxrs,aj4rxss,aj4rxrt,
     & aj4rxst,aj4rxtt,aj4rxrrr,aj4rxrrs,aj4rxrss,aj4rxsss,aj4rxrrt,
     & aj4rxrst,aj4rxsst,aj4rxrtt,aj4rxstt,aj4rxttt,aj4rxrrrr,
     & aj4rxrrrs,aj4rxrrss,aj4rxrsss,aj4rxssss,aj4rxrrrt,aj4rxrrst,
     & aj4rxrsst,aj4rxssst,aj4rxrrtt,aj4rxrstt,aj4rxsstt,aj4rxrttt,
     & aj4rxsttt,aj4rxtttt,aj4rxrrrrr,aj4rxrrrrs,aj4rxrrrss,
     & aj4rxrrsss,aj4rxrssss,aj4rxsssss,aj4rxrrrrt,aj4rxrrrst,
     & aj4rxrrsst,aj4rxrssst,aj4rxsssst,aj4rxrrrtt,aj4rxrrstt,
     & aj4rxrsstt,aj4rxssstt,aj4rxrrttt,aj4rxrsttt,aj4rxssttt,
     & aj4rxrtttt,aj4rxstttt,aj4rxttttt,aj4rxrrrrrr,aj4rxrrrrrs,
     & aj4rxrrrrss,aj4rxrrrsss,aj4rxrrssss,aj4rxrsssss,aj4rxssssss,
     & aj4rxrrrrrt,aj4rxrrrrst,aj4rxrrrsst,aj4rxrrssst,aj4rxrsssst,
     & aj4rxssssst,aj4rxrrrrtt,aj4rxrrrstt,aj4rxrrsstt,aj4rxrssstt,
     & aj4rxsssstt,aj4rxrrrttt,aj4rxrrsttt,aj4rxrssttt,aj4rxsssttt,
     & aj4rxrrtttt,aj4rxrstttt,aj4rxsstttt,aj4rxrttttt,aj4rxsttttt,
     & aj4rxtttttt
       real aj4sx,aj4sxr,aj4sxs,aj4sxt,aj4sxrr,aj4sxrs,aj4sxss,aj4sxrt,
     & aj4sxst,aj4sxtt,aj4sxrrr,aj4sxrrs,aj4sxrss,aj4sxsss,aj4sxrrt,
     & aj4sxrst,aj4sxsst,aj4sxrtt,aj4sxstt,aj4sxttt,aj4sxrrrr,
     & aj4sxrrrs,aj4sxrrss,aj4sxrsss,aj4sxssss,aj4sxrrrt,aj4sxrrst,
     & aj4sxrsst,aj4sxssst,aj4sxrrtt,aj4sxrstt,aj4sxsstt,aj4sxrttt,
     & aj4sxsttt,aj4sxtttt,aj4sxrrrrr,aj4sxrrrrs,aj4sxrrrss,
     & aj4sxrrsss,aj4sxrssss,aj4sxsssss,aj4sxrrrrt,aj4sxrrrst,
     & aj4sxrrsst,aj4sxrssst,aj4sxsssst,aj4sxrrrtt,aj4sxrrstt,
     & aj4sxrsstt,aj4sxssstt,aj4sxrrttt,aj4sxrsttt,aj4sxssttt,
     & aj4sxrtttt,aj4sxstttt,aj4sxttttt,aj4sxrrrrrr,aj4sxrrrrrs,
     & aj4sxrrrrss,aj4sxrrrsss,aj4sxrrssss,aj4sxrsssss,aj4sxssssss,
     & aj4sxrrrrrt,aj4sxrrrrst,aj4sxrrrsst,aj4sxrrssst,aj4sxrsssst,
     & aj4sxssssst,aj4sxrrrrtt,aj4sxrrrstt,aj4sxrrsstt,aj4sxrssstt,
     & aj4sxsssstt,aj4sxrrrttt,aj4sxrrsttt,aj4sxrssttt,aj4sxsssttt,
     & aj4sxrrtttt,aj4sxrstttt,aj4sxsstttt,aj4sxrttttt,aj4sxsttttt,
     & aj4sxtttttt
       real aj4ry,aj4ryr,aj4rys,aj4ryt,aj4ryrr,aj4ryrs,aj4ryss,aj4ryrt,
     & aj4ryst,aj4rytt,aj4ryrrr,aj4ryrrs,aj4ryrss,aj4rysss,aj4ryrrt,
     & aj4ryrst,aj4rysst,aj4ryrtt,aj4rystt,aj4ryttt,aj4ryrrrr,
     & aj4ryrrrs,aj4ryrrss,aj4ryrsss,aj4ryssss,aj4ryrrrt,aj4ryrrst,
     & aj4ryrsst,aj4ryssst,aj4ryrrtt,aj4ryrstt,aj4rysstt,aj4ryrttt,
     & aj4rysttt,aj4rytttt,aj4ryrrrrr,aj4ryrrrrs,aj4ryrrrss,
     & aj4ryrrsss,aj4ryrssss,aj4rysssss,aj4ryrrrrt,aj4ryrrrst,
     & aj4ryrrsst,aj4ryrssst,aj4rysssst,aj4ryrrrtt,aj4ryrrstt,
     & aj4ryrsstt,aj4ryssstt,aj4ryrrttt,aj4ryrsttt,aj4ryssttt,
     & aj4ryrtttt,aj4rystttt,aj4ryttttt,aj4ryrrrrrr,aj4ryrrrrrs,
     & aj4ryrrrrss,aj4ryrrrsss,aj4ryrrssss,aj4ryrsssss,aj4ryssssss,
     & aj4ryrrrrrt,aj4ryrrrrst,aj4ryrrrsst,aj4ryrrssst,aj4ryrsssst,
     & aj4ryssssst,aj4ryrrrrtt,aj4ryrrrstt,aj4ryrrsstt,aj4ryrssstt,
     & aj4rysssstt,aj4ryrrrttt,aj4ryrrsttt,aj4ryrssttt,aj4rysssttt,
     & aj4ryrrtttt,aj4ryrstttt,aj4rysstttt,aj4ryrttttt,aj4rysttttt,
     & aj4rytttttt
       real aj4sy,aj4syr,aj4sys,aj4syt,aj4syrr,aj4syrs,aj4syss,aj4syrt,
     & aj4syst,aj4sytt,aj4syrrr,aj4syrrs,aj4syrss,aj4sysss,aj4syrrt,
     & aj4syrst,aj4sysst,aj4syrtt,aj4systt,aj4syttt,aj4syrrrr,
     & aj4syrrrs,aj4syrrss,aj4syrsss,aj4syssss,aj4syrrrt,aj4syrrst,
     & aj4syrsst,aj4syssst,aj4syrrtt,aj4syrstt,aj4sysstt,aj4syrttt,
     & aj4systtt,aj4sytttt,aj4syrrrrr,aj4syrrrrs,aj4syrrrss,
     & aj4syrrsss,aj4syrssss,aj4sysssss,aj4syrrrrt,aj4syrrrst,
     & aj4syrrsst,aj4syrssst,aj4sysssst,aj4syrrrtt,aj4syrrstt,
     & aj4syrsstt,aj4syssstt,aj4syrrttt,aj4syrsttt,aj4syssttt,
     & aj4syrtttt,aj4systttt,aj4syttttt,aj4syrrrrrr,aj4syrrrrrs,
     & aj4syrrrrss,aj4syrrrsss,aj4syrrssss,aj4syrsssss,aj4syssssss,
     & aj4syrrrrrt,aj4syrrrrst,aj4syrrrsst,aj4syrrssst,aj4syrsssst,
     & aj4syssssst,aj4syrrrrtt,aj4syrrrstt,aj4syrrsstt,aj4syrssstt,
     & aj4sysssstt,aj4syrrrttt,aj4syrrsttt,aj4syrssttt,aj4sysssttt,
     & aj4syrrtttt,aj4syrstttt,aj4sysstttt,aj4syrttttt,aj4systtttt,
     & aj4sytttttt
       real aj4rxx,aj4rxy,aj4rxz,aj4rxxx,aj4rxxy,aj4rxyy,aj4rxxz,
     & aj4rxyz,aj4rxzz,aj4rxxxx,aj4rxxxy,aj4rxxyy,aj4rxyyy,aj4rxxxz,
     & aj4rxxyz,aj4rxyyz,aj4rxxzz,aj4rxyzz,aj4rxzzz,aj4rxxxxx,
     & aj4rxxxxy,aj4rxxxyy,aj4rxxyyy,aj4rxyyyy,aj4rxxxxz,aj4rxxxyz,
     & aj4rxxyyz,aj4rxyyyz,aj4rxxxzz,aj4rxxyzz,aj4rxyyzz,aj4rxxzzz,
     & aj4rxyzzz,aj4rxzzzz,aj4rxxxxxx,aj4rxxxxxy,aj4rxxxxyy,
     & aj4rxxxyyy,aj4rxxyyyy,aj4rxyyyyy,aj4rxxxxxz,aj4rxxxxyz,
     & aj4rxxxyyz,aj4rxxyyyz,aj4rxyyyyz,aj4rxxxxzz,aj4rxxxyzz,
     & aj4rxxyyzz,aj4rxyyyzz,aj4rxxxzzz,aj4rxxyzzz,aj4rxyyzzz,
     & aj4rxxzzzz,aj4rxyzzzz,aj4rxzzzzz,aj4rxxxxxxx,aj4rxxxxxxy,
     & aj4rxxxxxyy,aj4rxxxxyyy,aj4rxxxyyyy,aj4rxxyyyyy,aj4rxyyyyyy,
     & aj4rxxxxxxz,aj4rxxxxxyz,aj4rxxxxyyz,aj4rxxxyyyz,aj4rxxyyyyz,
     & aj4rxyyyyyz,aj4rxxxxxzz,aj4rxxxxyzz,aj4rxxxyyzz,aj4rxxyyyzz,
     & aj4rxyyyyzz,aj4rxxxxzzz,aj4rxxxyzzz,aj4rxxyyzzz,aj4rxyyyzzz,
     & aj4rxxxzzzz,aj4rxxyzzzz,aj4rxyyzzzz,aj4rxxzzzzz,aj4rxyzzzzz,
     & aj4rxzzzzzz
       real aj4sxx,aj4sxy,aj4sxz,aj4sxxx,aj4sxxy,aj4sxyy,aj4sxxz,
     & aj4sxyz,aj4sxzz,aj4sxxxx,aj4sxxxy,aj4sxxyy,aj4sxyyy,aj4sxxxz,
     & aj4sxxyz,aj4sxyyz,aj4sxxzz,aj4sxyzz,aj4sxzzz,aj4sxxxxx,
     & aj4sxxxxy,aj4sxxxyy,aj4sxxyyy,aj4sxyyyy,aj4sxxxxz,aj4sxxxyz,
     & aj4sxxyyz,aj4sxyyyz,aj4sxxxzz,aj4sxxyzz,aj4sxyyzz,aj4sxxzzz,
     & aj4sxyzzz,aj4sxzzzz,aj4sxxxxxx,aj4sxxxxxy,aj4sxxxxyy,
     & aj4sxxxyyy,aj4sxxyyyy,aj4sxyyyyy,aj4sxxxxxz,aj4sxxxxyz,
     & aj4sxxxyyz,aj4sxxyyyz,aj4sxyyyyz,aj4sxxxxzz,aj4sxxxyzz,
     & aj4sxxyyzz,aj4sxyyyzz,aj4sxxxzzz,aj4sxxyzzz,aj4sxyyzzz,
     & aj4sxxzzzz,aj4sxyzzzz,aj4sxzzzzz,aj4sxxxxxxx,aj4sxxxxxxy,
     & aj4sxxxxxyy,aj4sxxxxyyy,aj4sxxxyyyy,aj4sxxyyyyy,aj4sxyyyyyy,
     & aj4sxxxxxxz,aj4sxxxxxyz,aj4sxxxxyyz,aj4sxxxyyyz,aj4sxxyyyyz,
     & aj4sxyyyyyz,aj4sxxxxxzz,aj4sxxxxyzz,aj4sxxxyyzz,aj4sxxyyyzz,
     & aj4sxyyyyzz,aj4sxxxxzzz,aj4sxxxyzzz,aj4sxxyyzzz,aj4sxyyyzzz,
     & aj4sxxxzzzz,aj4sxxyzzzz,aj4sxyyzzzz,aj4sxxzzzzz,aj4sxyzzzzz,
     & aj4sxzzzzzz
       real aj4ryx,aj4ryy,aj4ryz,aj4ryxx,aj4ryxy,aj4ryyy,aj4ryxz,
     & aj4ryyz,aj4ryzz,aj4ryxxx,aj4ryxxy,aj4ryxyy,aj4ryyyy,aj4ryxxz,
     & aj4ryxyz,aj4ryyyz,aj4ryxzz,aj4ryyzz,aj4ryzzz,aj4ryxxxx,
     & aj4ryxxxy,aj4ryxxyy,aj4ryxyyy,aj4ryyyyy,aj4ryxxxz,aj4ryxxyz,
     & aj4ryxyyz,aj4ryyyyz,aj4ryxxzz,aj4ryxyzz,aj4ryyyzz,aj4ryxzzz,
     & aj4ryyzzz,aj4ryzzzz,aj4ryxxxxx,aj4ryxxxxy,aj4ryxxxyy,
     & aj4ryxxyyy,aj4ryxyyyy,aj4ryyyyyy,aj4ryxxxxz,aj4ryxxxyz,
     & aj4ryxxyyz,aj4ryxyyyz,aj4ryyyyyz,aj4ryxxxzz,aj4ryxxyzz,
     & aj4ryxyyzz,aj4ryyyyzz,aj4ryxxzzz,aj4ryxyzzz,aj4ryyyzzz,
     & aj4ryxzzzz,aj4ryyzzzz,aj4ryzzzzz,aj4ryxxxxxx,aj4ryxxxxxy,
     & aj4ryxxxxyy,aj4ryxxxyyy,aj4ryxxyyyy,aj4ryxyyyyy,aj4ryyyyyyy,
     & aj4ryxxxxxz,aj4ryxxxxyz,aj4ryxxxyyz,aj4ryxxyyyz,aj4ryxyyyyz,
     & aj4ryyyyyyz,aj4ryxxxxzz,aj4ryxxxyzz,aj4ryxxyyzz,aj4ryxyyyzz,
     & aj4ryyyyyzz,aj4ryxxxzzz,aj4ryxxyzzz,aj4ryxyyzzz,aj4ryyyyzzz,
     & aj4ryxxzzzz,aj4ryxyzzzz,aj4ryyyzzzz,aj4ryxzzzzz,aj4ryyzzzzz,
     & aj4ryzzzzzz
       real aj4syx,aj4syy,aj4syz,aj4syxx,aj4syxy,aj4syyy,aj4syxz,
     & aj4syyz,aj4syzz,aj4syxxx,aj4syxxy,aj4syxyy,aj4syyyy,aj4syxxz,
     & aj4syxyz,aj4syyyz,aj4syxzz,aj4syyzz,aj4syzzz,aj4syxxxx,
     & aj4syxxxy,aj4syxxyy,aj4syxyyy,aj4syyyyy,aj4syxxxz,aj4syxxyz,
     & aj4syxyyz,aj4syyyyz,aj4syxxzz,aj4syxyzz,aj4syyyzz,aj4syxzzz,
     & aj4syyzzz,aj4syzzzz,aj4syxxxxx,aj4syxxxxy,aj4syxxxyy,
     & aj4syxxyyy,aj4syxyyyy,aj4syyyyyy,aj4syxxxxz,aj4syxxxyz,
     & aj4syxxyyz,aj4syxyyyz,aj4syyyyyz,aj4syxxxzz,aj4syxxyzz,
     & aj4syxyyzz,aj4syyyyzz,aj4syxxzzz,aj4syxyzzz,aj4syyyzzz,
     & aj4syxzzzz,aj4syyzzzz,aj4syzzzzz,aj4syxxxxxx,aj4syxxxxxy,
     & aj4syxxxxyy,aj4syxxxyyy,aj4syxxyyyy,aj4syxyyyyy,aj4syyyyyyy,
     & aj4syxxxxxz,aj4syxxxxyz,aj4syxxxyyz,aj4syxxyyyz,aj4syxyyyyz,
     & aj4syyyyyyz,aj4syxxxxzz,aj4syxxxyzz,aj4syxxyyzz,aj4syxyyyzz,
     & aj4syyyyyzz,aj4syxxxzzz,aj4syxxyzzz,aj4syxyyzzz,aj4syyyyzzz,
     & aj4syxxzzzz,aj4syxyzzzz,aj4syyyzzzz,aj4syxzzzzz,aj4syyzzzzz,
     & aj4syzzzzzz
!     declareJacobianDerivativeVariables(aj6,2)

!     --- start statement function ----
      integer kd,m,n
!      declareDifferenceOrder2(u,RX)
!      declareDifferenceOrder4(u,RX)


      hu(i,n)    = huv(i,0,n)
      hux(i,n)   = huv(i,kx,n)
      huxx(i,n)  = huv(i,kxx,n)
      huxxx(i,n) = huv(i,kxxx,n)

      huy(i,n)   = huv(i,ky,n)
      huxy(i,n)  = huv(i,kxy,n)
      huyy(i,n)  = huv(i,kyy,n)
      huxyy(i,n) = huv(i,kxyy,n)
      huxxy(i,n) = huv(i,kxxy,n)
      huyyy(i,n) = huv(i,kyyy,n)

!      defineDifferenceOrder2Components1(u,RX)
!      defineDifferenceOrder4Components1(u,RX)

!............... end statement functions

      ierr=0

      side                 =ipar(0)
      axis                 =ipar(1)
      grid                 =ipar(2)
      n1a                  =ipar(3)
      n1b                  =ipar(4)
      n2a                  =ipar(5)
      n2b                  =ipar(6)
      n3a                  =ipar(7)
      n3b                  =ipar(8)
      na                   =ipar(9)
      nb                   =ipar(10)
      currentTimeLevel     =ipar(11)
      numberOfTimeLevels   =ipar(12)

      gridType             =ipar(13)
      orderOfAccuracy      =ipar(14)
      debug                =ipar(15)
      kernelType           =ipar(16)

      dx(0)                =rpar(0)
      dx(1)                =rpar(1)
      dx(2)                =rpar(2)
      dr(0)                =rpar(3)
      dr(1)                =rpar(4)
      dr(2)                =rpar(5)

      alpha                =rpar(6) ! damping
      ep                   =rpar(7) ! twilight zone pointer

      t                    =rpar(10)
      dt                   =rpar(11)
      eps                  =rpar(12)
      mu                   =rpar(13)
      c                    =rpar(14)

      z0=0.

!     numGhost=orderOfAccuracy/2

      ! bounds for loops 
      nn1a=n1a
      nn1b=n1b
      nn2a=n2a
      nn2b=n2b
      nn3a=n3a
      nn3b=n3b

      m = currentTimeLevel
      mm1 = mod(m-1+numberOfTimeLevels,numberOfTimeLevels)
      mm2 = mod(m-2+numberOfTimeLevels,numberOfTimeLevels)
      mm3 = mod(m-3+numberOfTimeLevels,numberOfTimeLevels)
      mm4 = mod(m-4+numberOfTimeLevels,numberOfTimeLevels)


      if( kernelType.eq.planar )then
        kx=1
        kxx=2
        kxxx=3
      else
        kx=1
        ky=2
        kxx=3
        kxy=4
        kyy=5
        kxxx=6
        kxxy=7
        kxyy=8
        kyyy=9
      end if

      if( nd.eq.2 )then

        i3=n3a

        is1=0
        is2=0
        is3=0

        if( axis.eq.0 ) then
          is1=1-2*side
          if( side.eq.0 )then
            nn1b=nn1a
          else
            nn1a=nn1b
          end if
        else
          is2=1-2*side
          if( side.eq.0 )then
            nn2b=nn2a
          else
            nn2a=nn2b
          end if
        end if



        if( kernelType.eq.planar )then
          ! ************* planar interface *******************
          if( axis.ne.0 )then
            write(*,'("radEval:ERROR: not implemented for axis=",i3)') 
     & axis
            stop 1163
          end if

         if( orderOfAccuracy.eq.2 )then
          do i3=nn3a,nn3b
          do i2=nn2a,nn2b
          do i1=nn1a,nn1b
          do n=na,nb
            ! write(*,'(" periodic i1,i2,i3,n=",4i4)') i1,i2,i3,n
!     
           ux = (-u(i1-1,i2,i3,n)+u(i1+1,i2,i3,n))/(2.*dx(0))
           uxx =(u(i1-1,i2,i3,n)-2.*u(i1,i2,i3,n)+u(i1+1,i2,i3,n))/(dx(
     & 0)**2)
           uyy =(u(i1,i2-1,i3,n)-2.*u(i1,i2,i3,n)+u(i1,i2+1,i3,n))/(dx(
     & 1)**2)
           uLap = uxx+uyy
           utt  = uLap



           h=dx(0)*is1
           im=i1-is1
            um1 = u(i1,i2,i3,n) + dt*( ux*is1 - hu(i2,n) )  - h*ux +.5*
     & dt*dt*utt -dt*h*( uxx*is1 - hux(i2,n) ) + .5*h*h*uxx+dt*alpha*(
     &  u(im,i2-1,i3,n)-2.*u(im,i2,i3,n)+u(im,i2+1,i3,n) )

           h=2.*dx(0)*is1
           im=i1-2*is1
            um2 = u(i1,i2,i3,n) + dt*( ux*is1 - hu(i2,n) )  - h*ux +.5*
     & dt*dt*utt -dt*h*( uxx*is1 - hux(i2,n) ) + .5*h*h*uxx+dt*alpha*(
     &  u(im,i2-1,i3,n)-2.*u(im,i2,i3,n)+u(im,i2+1,i3,n) )

           u1(i1-is1,i2,i3,n)=um1
           u1(i1-2*is1,i2,i3,n)=um2

          end do
          end do
          end do
          end do


         else if( orderOfAccuracy.eq.4 )then


          do i3=nn3a,nn3b
          do i2=nn2a,nn2b
          do i1=nn1a,nn1b
          do n=na,nb
            ! write(*,'(" periodic i1,i2,i3,n=",4i4)') i1,i2,i3,n
!     
           ux = (u(i1-2,i2,i3,n)-8.*u(i1-1,i2,i3,n)+8.*u(i1+1,i2,i3,n)-
     & u(i1+2,i2,i3,n))/(12.*dx(0))
           uxx =(-u(i1-2,i2,i3,n)+16.*u(i1-1,i2,i3,n)-30.*u(i1,i2,i3,n)
     & +16.*u(i1+1,i2,i3,n)-u(i1+2,i2,i3,n))/(12.*dx(0)**2)
           uyy =(-u(i1,i2-2,i3,n)+16.*u(i1,i2-1,i3,n)-30.*u(i1,i2,i3,n)
     & +16.*u(i1,i2+1,i3,n)-u(i1,i2+2,i3,n))/(12.*dx(1)**2)
           uLap = uxx+uyy

           uxxx = (-u(i1-2,i2,i3,n)+2.*u(i1-1,i2,i3,n)-2.*u(i1+1,i2,i3,
     & n)+u(i1+2,i2,i3,n))/(2.*dx(0)**3)
           uxyy = (-(u(i1-1,i2-1,i3,n)-2.*u(i1-1,i2,i3,n)+u(i1-1,i2+1,
     & i3,n))/(dx(1)**2)+(u(i1+1,i2-1,i3,n)-2.*u(i1+1,i2,i3,n)+u(i1+1,
     & i2+1,i3,n))/(dx(1)**2))/(2.*dx(0))

           uLapx= uxxx+uxyy

           uxxxx =(u(i1-2,i2,i3,n)-4.*u(i1-1,i2,i3,n)+6.*u(i1,i2,i3,n)-
     & 4.*u(i1+1,i2,i3,n)+u(i1+2,i2,i3,n))/(dx(0)**4)
           uxxyy =((u(i1-1,i2-1,i3,n)-2.*u(i1-1,i2,i3,n)+u(i1-1,i2+1,
     & i3,n))/(dx(1)**2)-2.*(u(i1,i2-1,i3,n)-2.*u(i1,i2,i3,n)+u(i1,i2+
     & 1,i3,n))/(dx(1)**2)+(u(i1+1,i2-1,i3,n)-2.*u(i1+1,i2,i3,n)+u(i1+
     & 1,i2+1,i3,n))/(dx(1)**2))/(dx(0)**2)
           uyyyy =(u(i1,i2-2,i3,n)-4.*u(i1,i2-1,i3,n)+6.*u(i1,i2,i3,n)-
     & 4.*u(i1,i2+1,i3,n)+u(i1,i2+2,i3,n))/(dx(1)**4)

           uLapSq=uxxxx+2.*uxxyy+uyyyy
           uLapxx=uxxxx+uxxyy


           huyyi = (hu(i2-1,n)-2.*hu(i2,n)+hu(i2+1,n))/(dx(1)**2)
           huxyyi= (hux(i2-1,n)-2.*hux(i2,n)+hux(i2+1,n))/(dx(1)**2)
           huLap = huxx(i2,n)+huyyi

           utx = uxx*is1 - hux(i2,n)
           utxyy = uxxyy*is1 - huxyyi ! utx = uxx - hux  -> (utx)yy = uxxyy - (hux)yy

           ut   = ux*is1 - hu(i2,n)
           utt  = uLap
           uttt = uLapx*is1 -huLap
           uttx = uLapx
           utxx = uxxx*is1 -huxx(i2,n)

           utxxx = uxxxx*is1 - huxxx(i2,n)
           utttt = uLapSq
           utttx = utxxx + utxyy   ! (uxx+uyy).tx
           uttxx = uLapxx


           if( debug.gt.1 )then
            tm=t-dt
            call getExactSolution( xy(i1,i2,i3,0),xy(i1,i2,i3,1),tm,0,
     & 0,1,vt)
      write(*,'("** tm=",f6.4," i1,i2=",2i3)') tm,i1,i2
          ! "'
      write(*,'("   ++ ut,vt=",2e10.2," err=",e8.2)') ut,vt,ut-vt

            call getExactSolution( xy(i1,i2,i3,0),xy(i1,i2,i3,1),tm,1,
     & 0,0,vx)
      write(*,'("   ++ ux,vx=",2e10.2," err=",e8.2)') ux,vx,ux-vx

      write(*,'("   ++ vt-vx+hu=",e8.2)') vt-vx+hu(i2,n)


            call getExactSolution( xy(i1,i2,i3,0),xy(i1,i2,i3,1),tm,1,
     & 0,1,vtx)
      write(*,'("   ++ utx,vtx=",2e10.2," err=",e8.2)') utx,vtx,utx-vtx

            call getExactSolution( xy(i1,i2,i3,0),xy(i1,i2,i3,1),tm,0,
     & 0,3,vttt)
      write(*,'("   ++ uttt,vttt=",2e10.2," err=",e8.2)') uttt,vttt,
     & uttt-vttt

            call getExactSolution( xy(i1,i2,i3,0),xy(i1,i2,i3,1),tm,0,
     & 0,4,vtttt)
      write(*,'("   ++ utttt,vtttt=",2e10.2," err=",e8.2)') utttt,
     & vtttt,utttt-vtttt

           end if
          ! ******
          ! uttt=vttt

           h=dx(0)*is1
           im=i1-is1
            um1 = u(i1,i2,i3,n) + dt*ut - h*ux +.5*dt*dt*utt -dt*h*utx 
     & + .5*h*h*uxx+ dt*dt*dt/6.*uttt -dt*dt*h*.5*uttx + dt*h*h*.5*
     & utxx - h*h*h/6.*uxxx + dt*dt*dt*dt/24.*utttt -dt*dt*dt*h/6.*
     & utttx + dt*dt*h*h/4.*uttxx -dt*h*h*h/6.*utxxx + h*h*h*h/24.*
     & uxxxx +dt*alpha*( -u(im,i2-2,i3,n)+4.*u(im,i2-1,i3,n)-6.*u(im,
     & i2,i3,n)+4.*u(im,i2+1,i3,n)-u(im,i2+2,i3,n) )



           h=2.*dx(0)*is1
           im=i1-2*is1
            um2 = u(i1,i2,i3,n) + dt*ut - h*ux +.5*dt*dt*utt -dt*h*utx 
     & + .5*h*h*uxx+ dt*dt*dt/6.*uttt -dt*dt*h*.5*uttx + dt*h*h*.5*
     & utxx - h*h*h/6.*uxxx + dt*dt*dt*dt/24.*utttt -dt*dt*dt*h/6.*
     & utttx + dt*dt*h*h/4.*uttxx -dt*h*h*h/6.*utxxx + h*h*h*h/24.*
     & uxxxx +dt*alpha*( -u(im,i2-2,i3,n)+4.*u(im,i2-1,i3,n)-6.*u(im,
     & i2,i3,n)+4.*u(im,i2+1,i3,n)-u(im,i2+2,i3,n) )

           u1(i1-is1,i2,i3,n)=um1
           u1(i1-2*is1,i2,i3,n)=um2

           if( debug.gt.1 )then
            call getExactSolution( xy(i1-is1,i2,i3,0),xy(i1-is1,i2,i3,
     & 1),t,0,0,0,utrue)
            write(*,'(" radEval: t,i2,um1,utrue,err=",e8.2,i3,3e10.3)')
     &  t,i2,um1,utrue,um1-utrue
          ! ** u1(i1-1,i2,i3,n)=utrue

            call getExactSolution( xy(i1-2*is1,i2,i3,0),xy(i1-2*is1,i2,
     & i3,1),t,0,0,0,utrue)
            write(*,'("        : t,i2,um2,utrue,err=",e8.2,i3,3e10.3)')
     &  t,i2,um2,utrue,um2-utrue
          ! ** u1(i1-2,i2,i3,n)=utrue
           end if

          end do
          end do
          end do
          end do

         else
           stop 8832 ! unknown orderOfAccuracy
         end if

        else if( kernelType.eq.cylindrical )then

           ! **************Cylindrical boundary**********************************
         if( orderOfAccuracy.eq.2 )then
          if( axis.eq.0 )then
            ii=nn2a
          else
            ii=nn1a
          end if
          do i2=nn2a,nn2b
          do i1=nn1a,nn1b
!     
          !  evalJacobianDerivatives(u,v,DIM,ORDER,MAXDERIV)
          ! NOTE: jacobians need 1 less derivative  ****************** no need to repeat for all components!
          !evalJacobianDerivatives(rsxy,i1,i2,i3,aj4,2,4,1)
           aj2rx = rsxy(i1,i2,i3,0,0)
           aj2rxr = (-rsxy(i1-1,i2,i3,0,0)+rsxy(i1+1,i2,i3,0,0))/(2.*
     & dr(0))
           aj2rxs = (-rsxy(i1,i2-1,i3,0,0)+rsxy(i1,i2+1,i3,0,0))/(2.*
     & dr(1))
           aj2sx = rsxy(i1,i2,i3,1,0)
           aj2sxr = (-rsxy(i1-1,i2,i3,1,0)+rsxy(i1+1,i2,i3,1,0))/(2.*
     & dr(0))
           aj2sxs = (-rsxy(i1,i2-1,i3,1,0)+rsxy(i1,i2+1,i3,1,0))/(2.*
     & dr(1))
           aj2ry = rsxy(i1,i2,i3,0,1)
           aj2ryr = (-rsxy(i1-1,i2,i3,0,1)+rsxy(i1+1,i2,i3,0,1))/(2.*
     & dr(0))
           aj2rys = (-rsxy(i1,i2-1,i3,0,1)+rsxy(i1,i2+1,i3,0,1))/(2.*
     & dr(1))
           aj2sy = rsxy(i1,i2,i3,1,1)
           aj2syr = (-rsxy(i1-1,i2,i3,1,1)+rsxy(i1+1,i2,i3,1,1))/(2.*
     & dr(0))
           aj2sys = (-rsxy(i1,i2-1,i3,1,1)+rsxy(i1,i2+1,i3,1,1))/(2.*
     & dr(1))
           aj2rxx = aj2rx*aj2rxr+aj2sx*aj2rxs
           aj2rxy = aj2ry*aj2rxr+aj2sy*aj2rxs
           aj2sxx = aj2rx*aj2sxr+aj2sx*aj2sxs
           aj2sxy = aj2ry*aj2sxr+aj2sy*aj2sxs
           aj2ryx = aj2rx*aj2ryr+aj2sx*aj2rys
           aj2ryy = aj2ry*aj2ryr+aj2sy*aj2rys
           aj2syx = aj2rx*aj2syr+aj2sx*aj2sys
           aj2syy = aj2ry*aj2syr+aj2sy*aj2sys
           do n=na,nb

            ! evalParametricDerivativesComponents1(u,i1,i2,i3,m, v,DIM,ORDER,MAXDERIV)
            uu = u(i1,i2,i3,n)
            uur = (-u(i1-1,i2,i3,n)+u(i1+1,i2,i3,n))/(2.*dr(0))
            uus = (-u(i1,i2-1,i3,n)+u(i1,i2+1,i3,n))/(2.*dr(1))
            uurr = (u(i1-1,i2,i3,n)-2.*u(i1,i2,i3,n)+u(i1+1,i2,i3,n))/(
     & dr(0)**2)
            uurs = (-(-u(i1-1,i2-1,i3,n)+u(i1-1,i2+1,i3,n))/(2.*dr(1))+
     & (-u(i1+1,i2-1,i3,n)+u(i1+1,i2+1,i3,n))/(2.*dr(1)))/(2.*dr(0))
            uuss = (u(i1,i2-1,i3,n)-2.*u(i1,i2,i3,n)+u(i1,i2+1,i3,n))/(
     & dr(1)**2)

            ux = aj2rx*uur+aj2sx*uus
            uy = aj2ry*uur+aj2sy*uus

            t1 = aj2rx**2
            t6 = aj2sx**2
            uxx = t1*uurr+2*aj2rx*aj2sx*uurs+t6*uuss+aj2rxx*uur+aj2sxx*
     & uus
            uxy = aj2ry*aj2rx*uurr+(aj2sy*aj2rx+aj2ry*aj2sx)*uurs+
     & aj2sy*aj2sx*uuss+aj2rxy*uur+aj2sxy*uus
            t1 = aj2ry**2
            t6 = aj2sy**2
            uyy = t1*uurr+2*aj2ry*aj2sy*uurs+t6*uuss+aj2ryy*uur+aj2syy*
     & uus

            uLap = uxx+uyy
            utt  = uLap

            r = sqrt( xy(i1,i2,i3,0)**2 + xy(i1,i2,i3,1)**2 )
            ! outward normal:
            an1= -rsxy(i1,i2,i3,axis,0)*is2
            an2= -rsxy(i1,i2,i3,axis,1)*is2
            aNorm = sqrt( an1**2 + an2**2 )
            an1=an1/aNorm
            an2=an2/aNorm
            uri = an1*ux+an2*uy     ! u.n = u.r

            uxri = an1*uxx+an2*uxy   ! (ux).r = n.grad(ux)
            uyri = an1*uxy+an2*uyy

            ut = -c*( uri  + u(i1,i2,i3,n)/(2.*r) + hu(ii,n) )
            utx =-c*( uxri +            ux/(2.*r) + hux(ii,n) )
            uty =-c*( uyri +            uy/(2.*r) + huy(ii,n) )


            if( debug.gt.0 )then
             tm=t-dt
             call ogderiv(ep,1,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),z0,
     & tm,n,ut )
             call ogderiv(ep,1,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),z0,
     & tm,n,utx )
             call ogderiv(ep,1,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),z0,
     & tm,n,uty )
            end if

            hx = xy(i1-is1,i2-is2,i3,0)- xy(i1,i2,i3,0)
            hy = xy(i1-is1,i2-is2,i3,1)- xy(i1,i2,i3,1)
            i1m=i1-is1
            i2m=i2-is2
             um1 = u(i1,i2,i3,n) + dt*ut  + hx*ux + hy*uy +.5*dt*dt*
     & utt + dt*hx*utx + dt*hy*uty + .5*( hx**2*uxx +2.*hx*hy*uxy+hy**
     & 2*uyy )+dt*alpha*( u(i1m-is2,i2m-is1,i3,n)-2.*u(i1m,i2m,i3,n)+
     & u(i1m+is2,i2m+is1,i3,n) )

            hx = xy(i1-2*is1,i2-2*is2,i3,0)- xy(i1,i2,i3,0)
            hy = xy(i1-2*is1,i2-2*is2,i3,1)- xy(i1,i2,i3,1)
            i1m=i1-2*is1
            i2m=i2-2*is2
             um2 = u(i1,i2,i3,n) + dt*ut  + hx*ux + hy*uy +.5*dt*dt*
     & utt + dt*hx*utx + dt*hy*uty + .5*( hx**2*uxx +2.*hx*hy*uxy+hy**
     & 2*uyy )+dt*alpha*( u(i1m-is2,i2m-is1,i3,n)-2.*u(i1m,i2m,i3,n)+
     & u(i1m+is2,i2m+is1,i3,n) )

            u1(i1-is1,i2-is2,i3,n)=um1
            u1(i1-2*is1,i2-2*is2,i3,n)=um2

            if( debug.gt.0 )then
             utrue=ogf(ep,xy(i1-is1,i2-is2,i3,0),xy(i1-is1,i2-is2,i3,1)
     & ,z0,n,t)
             write(*,'(" radEval: t,ii,n,um1,utrue,err=",e8.2,i3,i2,1x,
     & 3e10.2)') t,ii,n,um1,utrue,um1-utrue
             u1(i1-is1,i2-is2,i3,n)=utrue

             utrue=ogf(ep,xy(i1-2*is1,i2-2*is2,i3,0),xy(i1-2*is1,i2-2*
     & is2,i3,1),z0,n,t)
             write(*,'("        : t,ii,n,um2,utrue,err=",e8.2,i3,i2,1x,
     & 3e10.2)') t,ii,n,um2,utrue,um2-utrue
             u1(i1-2*is1,i2-2*is2,i3,n)=utrue
            end if


            enddo
            ii=ii+1
          enddo
          enddo

         else if( orderOfAccuracy.eq.4 )then
          if( axis.eq.0 )then
            ii=nn2a
          else
            ii=nn1a
          end if
          do i2=nn2a,nn2b
          do i1=nn1a,nn1b
!     
          !  evalJacobianDerivatives(u,v,DIM,ORDER,MAXDERIV)
          ! NOTE: jacobians need 1 less derivative  ****************** no need to repeat for all components!
           aj4rx = rsxy(i1,i2,i3,0,0)
           aj4rxr = (rsxy(i1-2,i2,i3,0,0)-8.*rsxy(i1-1,i2,i3,0,0)+8.*
     & rsxy(i1+1,i2,i3,0,0)-rsxy(i1+2,i2,i3,0,0))/(12.*dr(0))
           aj4rxs = (rsxy(i1,i2-2,i3,0,0)-8.*rsxy(i1,i2-1,i3,0,0)+8.*
     & rsxy(i1,i2+1,i3,0,0)-rsxy(i1,i2+2,i3,0,0))/(12.*dr(1))
           aj4sx = rsxy(i1,i2,i3,1,0)
           aj4sxr = (rsxy(i1-2,i2,i3,1,0)-8.*rsxy(i1-1,i2,i3,1,0)+8.*
     & rsxy(i1+1,i2,i3,1,0)-rsxy(i1+2,i2,i3,1,0))/(12.*dr(0))
           aj4sxs = (rsxy(i1,i2-2,i3,1,0)-8.*rsxy(i1,i2-1,i3,1,0)+8.*
     & rsxy(i1,i2+1,i3,1,0)-rsxy(i1,i2+2,i3,1,0))/(12.*dr(1))
           aj4ry = rsxy(i1,i2,i3,0,1)
           aj4ryr = (rsxy(i1-2,i2,i3,0,1)-8.*rsxy(i1-1,i2,i3,0,1)+8.*
     & rsxy(i1+1,i2,i3,0,1)-rsxy(i1+2,i2,i3,0,1))/(12.*dr(0))
           aj4rys = (rsxy(i1,i2-2,i3,0,1)-8.*rsxy(i1,i2-1,i3,0,1)+8.*
     & rsxy(i1,i2+1,i3,0,1)-rsxy(i1,i2+2,i3,0,1))/(12.*dr(1))
           aj4sy = rsxy(i1,i2,i3,1,1)
           aj4syr = (rsxy(i1-2,i2,i3,1,1)-8.*rsxy(i1-1,i2,i3,1,1)+8.*
     & rsxy(i1+1,i2,i3,1,1)-rsxy(i1+2,i2,i3,1,1))/(12.*dr(0))
           aj4sys = (rsxy(i1,i2-2,i3,1,1)-8.*rsxy(i1,i2-1,i3,1,1)+8.*
     & rsxy(i1,i2+1,i3,1,1)-rsxy(i1,i2+2,i3,1,1))/(12.*dr(1))
           aj4rxx = aj4rx*aj4rxr+aj4sx*aj4rxs
           aj4rxy = aj4ry*aj4rxr+aj4sy*aj4rxs
           aj4sxx = aj4rx*aj4sxr+aj4sx*aj4sxs
           aj4sxy = aj4ry*aj4sxr+aj4sy*aj4sxs
           aj4ryx = aj4rx*aj4ryr+aj4sx*aj4rys
           aj4ryy = aj4ry*aj4ryr+aj4sy*aj4rys
           aj4syx = aj4rx*aj4syr+aj4sx*aj4sys
           aj4syy = aj4ry*aj4syr+aj4sy*aj4sys
           aj2rx = rsxy(i1,i2,i3,0,0)
           aj2rxr = (-rsxy(i1-1,i2,i3,0,0)+rsxy(i1+1,i2,i3,0,0))/(2.*
     & dr(0))
           aj2rxs = (-rsxy(i1,i2-1,i3,0,0)+rsxy(i1,i2+1,i3,0,0))/(2.*
     & dr(1))
           aj2rxrr = (rsxy(i1-1,i2,i3,0,0)-2.*rsxy(i1,i2,i3,0,0)+rsxy(
     & i1+1,i2,i3,0,0))/(dr(0)**2)
           aj2rxrs = (-(-rsxy(i1-1,i2-1,i3,0,0)+rsxy(i1-1,i2+1,i3,0,0))
     & /(2.*dr(1))+(-rsxy(i1+1,i2-1,i3,0,0)+rsxy(i1+1,i2+1,i3,0,0))/(
     & 2.*dr(1)))/(2.*dr(0))
           aj2rxss = (rsxy(i1,i2-1,i3,0,0)-2.*rsxy(i1,i2,i3,0,0)+rsxy(
     & i1,i2+1,i3,0,0))/(dr(1)**2)
           aj2rxrrr = (-rsxy(i1-2,i2,i3,0,0)+2.*rsxy(i1-1,i2,i3,0,0)-
     & 2.*rsxy(i1+1,i2,i3,0,0)+rsxy(i1+2,i2,i3,0,0))/(2.*dr(0)**3)
           aj2rxrrs = ((-rsxy(i1-1,i2-1,i3,0,0)+rsxy(i1-1,i2+1,i3,0,0))
     & /(2.*dr(1))-2.*(-rsxy(i1,i2-1,i3,0,0)+rsxy(i1,i2+1,i3,0,0))/(
     & 2.*dr(1))+(-rsxy(i1+1,i2-1,i3,0,0)+rsxy(i1+1,i2+1,i3,0,0))/(2.*
     & dr(1)))/(dr(0)**2)
           aj2rxrss = (-(rsxy(i1-1,i2-1,i3,0,0)-2.*rsxy(i1-1,i2,i3,0,0)
     & +rsxy(i1-1,i2+1,i3,0,0))/(dr(1)**2)+(rsxy(i1+1,i2-1,i3,0,0)-2.*
     & rsxy(i1+1,i2,i3,0,0)+rsxy(i1+1,i2+1,i3,0,0))/(dr(1)**2))/(2.*
     & dr(0))
           aj2rxsss = (-rsxy(i1,i2-2,i3,0,0)+2.*rsxy(i1,i2-1,i3,0,0)-
     & 2.*rsxy(i1,i2+1,i3,0,0)+rsxy(i1,i2+2,i3,0,0))/(2.*dr(1)**3)
           aj2sx = rsxy(i1,i2,i3,1,0)
           aj2sxr = (-rsxy(i1-1,i2,i3,1,0)+rsxy(i1+1,i2,i3,1,0))/(2.*
     & dr(0))
           aj2sxs = (-rsxy(i1,i2-1,i3,1,0)+rsxy(i1,i2+1,i3,1,0))/(2.*
     & dr(1))
           aj2sxrr = (rsxy(i1-1,i2,i3,1,0)-2.*rsxy(i1,i2,i3,1,0)+rsxy(
     & i1+1,i2,i3,1,0))/(dr(0)**2)
           aj2sxrs = (-(-rsxy(i1-1,i2-1,i3,1,0)+rsxy(i1-1,i2+1,i3,1,0))
     & /(2.*dr(1))+(-rsxy(i1+1,i2-1,i3,1,0)+rsxy(i1+1,i2+1,i3,1,0))/(
     & 2.*dr(1)))/(2.*dr(0))
           aj2sxss = (rsxy(i1,i2-1,i3,1,0)-2.*rsxy(i1,i2,i3,1,0)+rsxy(
     & i1,i2+1,i3,1,0))/(dr(1)**2)
           aj2sxrrr = (-rsxy(i1-2,i2,i3,1,0)+2.*rsxy(i1-1,i2,i3,1,0)-
     & 2.*rsxy(i1+1,i2,i3,1,0)+rsxy(i1+2,i2,i3,1,0))/(2.*dr(0)**3)
           aj2sxrrs = ((-rsxy(i1-1,i2-1,i3,1,0)+rsxy(i1-1,i2+1,i3,1,0))
     & /(2.*dr(1))-2.*(-rsxy(i1,i2-1,i3,1,0)+rsxy(i1,i2+1,i3,1,0))/(
     & 2.*dr(1))+(-rsxy(i1+1,i2-1,i3,1,0)+rsxy(i1+1,i2+1,i3,1,0))/(2.*
     & dr(1)))/(dr(0)**2)
           aj2sxrss = (-(rsxy(i1-1,i2-1,i3,1,0)-2.*rsxy(i1-1,i2,i3,1,0)
     & +rsxy(i1-1,i2+1,i3,1,0))/(dr(1)**2)+(rsxy(i1+1,i2-1,i3,1,0)-2.*
     & rsxy(i1+1,i2,i3,1,0)+rsxy(i1+1,i2+1,i3,1,0))/(dr(1)**2))/(2.*
     & dr(0))
           aj2sxsss = (-rsxy(i1,i2-2,i3,1,0)+2.*rsxy(i1,i2-1,i3,1,0)-
     & 2.*rsxy(i1,i2+1,i3,1,0)+rsxy(i1,i2+2,i3,1,0))/(2.*dr(1)**3)
           aj2ry = rsxy(i1,i2,i3,0,1)
           aj2ryr = (-rsxy(i1-1,i2,i3,0,1)+rsxy(i1+1,i2,i3,0,1))/(2.*
     & dr(0))
           aj2rys = (-rsxy(i1,i2-1,i3,0,1)+rsxy(i1,i2+1,i3,0,1))/(2.*
     & dr(1))
           aj2ryrr = (rsxy(i1-1,i2,i3,0,1)-2.*rsxy(i1,i2,i3,0,1)+rsxy(
     & i1+1,i2,i3,0,1))/(dr(0)**2)
           aj2ryrs = (-(-rsxy(i1-1,i2-1,i3,0,1)+rsxy(i1-1,i2+1,i3,0,1))
     & /(2.*dr(1))+(-rsxy(i1+1,i2-1,i3,0,1)+rsxy(i1+1,i2+1,i3,0,1))/(
     & 2.*dr(1)))/(2.*dr(0))
           aj2ryss = (rsxy(i1,i2-1,i3,0,1)-2.*rsxy(i1,i2,i3,0,1)+rsxy(
     & i1,i2+1,i3,0,1))/(dr(1)**2)
           aj2ryrrr = (-rsxy(i1-2,i2,i3,0,1)+2.*rsxy(i1-1,i2,i3,0,1)-
     & 2.*rsxy(i1+1,i2,i3,0,1)+rsxy(i1+2,i2,i3,0,1))/(2.*dr(0)**3)
           aj2ryrrs = ((-rsxy(i1-1,i2-1,i3,0,1)+rsxy(i1-1,i2+1,i3,0,1))
     & /(2.*dr(1))-2.*(-rsxy(i1,i2-1,i3,0,1)+rsxy(i1,i2+1,i3,0,1))/(
     & 2.*dr(1))+(-rsxy(i1+1,i2-1,i3,0,1)+rsxy(i1+1,i2+1,i3,0,1))/(2.*
     & dr(1)))/(dr(0)**2)
           aj2ryrss = (-(rsxy(i1-1,i2-1,i3,0,1)-2.*rsxy(i1-1,i2,i3,0,1)
     & +rsxy(i1-1,i2+1,i3,0,1))/(dr(1)**2)+(rsxy(i1+1,i2-1,i3,0,1)-2.*
     & rsxy(i1+1,i2,i3,0,1)+rsxy(i1+1,i2+1,i3,0,1))/(dr(1)**2))/(2.*
     & dr(0))
           aj2rysss = (-rsxy(i1,i2-2,i3,0,1)+2.*rsxy(i1,i2-1,i3,0,1)-
     & 2.*rsxy(i1,i2+1,i3,0,1)+rsxy(i1,i2+2,i3,0,1))/(2.*dr(1)**3)
           aj2sy = rsxy(i1,i2,i3,1,1)
           aj2syr = (-rsxy(i1-1,i2,i3,1,1)+rsxy(i1+1,i2,i3,1,1))/(2.*
     & dr(0))
           aj2sys = (-rsxy(i1,i2-1,i3,1,1)+rsxy(i1,i2+1,i3,1,1))/(2.*
     & dr(1))
           aj2syrr = (rsxy(i1-1,i2,i3,1,1)-2.*rsxy(i1,i2,i3,1,1)+rsxy(
     & i1+1,i2,i3,1,1))/(dr(0)**2)
           aj2syrs = (-(-rsxy(i1-1,i2-1,i3,1,1)+rsxy(i1-1,i2+1,i3,1,1))
     & /(2.*dr(1))+(-rsxy(i1+1,i2-1,i3,1,1)+rsxy(i1+1,i2+1,i3,1,1))/(
     & 2.*dr(1)))/(2.*dr(0))
           aj2syss = (rsxy(i1,i2-1,i3,1,1)-2.*rsxy(i1,i2,i3,1,1)+rsxy(
     & i1,i2+1,i3,1,1))/(dr(1)**2)
           aj2syrrr = (-rsxy(i1-2,i2,i3,1,1)+2.*rsxy(i1-1,i2,i3,1,1)-
     & 2.*rsxy(i1+1,i2,i3,1,1)+rsxy(i1+2,i2,i3,1,1))/(2.*dr(0)**3)
           aj2syrrs = ((-rsxy(i1-1,i2-1,i3,1,1)+rsxy(i1-1,i2+1,i3,1,1))
     & /(2.*dr(1))-2.*(-rsxy(i1,i2-1,i3,1,1)+rsxy(i1,i2+1,i3,1,1))/(
     & 2.*dr(1))+(-rsxy(i1+1,i2-1,i3,1,1)+rsxy(i1+1,i2+1,i3,1,1))/(2.*
     & dr(1)))/(dr(0)**2)
           aj2syrss = (-(rsxy(i1-1,i2-1,i3,1,1)-2.*rsxy(i1-1,i2,i3,1,1)
     & +rsxy(i1-1,i2+1,i3,1,1))/(dr(1)**2)+(rsxy(i1+1,i2-1,i3,1,1)-2.*
     & rsxy(i1+1,i2,i3,1,1)+rsxy(i1+1,i2+1,i3,1,1))/(dr(1)**2))/(2.*
     & dr(0))
           aj2sysss = (-rsxy(i1,i2-2,i3,1,1)+2.*rsxy(i1,i2-1,i3,1,1)-
     & 2.*rsxy(i1,i2+1,i3,1,1)+rsxy(i1,i2+2,i3,1,1))/(2.*dr(1)**3)
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
           aj2rxxxx = t1*aj2rx*aj2rxrrr+3*t1*aj2sx*aj2rxrrs+3*aj2rx*t7*
     & aj2rxrss+t7*aj2sx*aj2rxsss+3*aj2rx*aj2rxx*aj2rxrr+(3*aj2sxx*
     & aj2rx+3*aj2sx*aj2rxx)*aj2rxrs+3*aj2sxx*aj2sx*aj2rxss+aj2rxxx*
     & aj2rxr+aj2sxxx*aj2rxs
           t1 = aj2rx**2
           t10 = aj2sx**2
           aj2rxxxy = aj2ry*t1*aj2rxrrr+(aj2sy*t1+2*aj2ry*aj2sx*aj2rx)*
     & aj2rxrrs+(aj2ry*t10+2*aj2sy*aj2sx*aj2rx)*aj2rxrss+aj2sy*t10*
     & aj2rxsss+(2*aj2rxy*aj2rx+aj2ry*aj2rxx)*aj2rxrr+(aj2ry*aj2sxx+2*
     & aj2sx*aj2rxy+2*aj2sxy*aj2rx+aj2sy*aj2rxx)*aj2rxrs+(aj2sy*
     & aj2sxx+2*aj2sxy*aj2sx)*aj2rxss+aj2rxxy*aj2rxr+aj2sxxy*aj2rxs
           t1 = aj2ry**2
           t4 = aj2sy*aj2ry
           t8 = aj2sy*aj2rx+aj2ry*aj2sx
           t16 = aj2sy**2
           aj2rxxyy = t1*aj2rx*aj2rxrrr+(t4*aj2rx+aj2ry*t8)*aj2rxrrs+(
     & t4*aj2sx+aj2sy*t8)*aj2rxrss+t16*aj2sx*aj2rxsss+(aj2ryy*aj2rx+2*
     & aj2ry*aj2rxy)*aj2rxrr+(2*aj2ry*aj2sxy+2*aj2sy*aj2rxy+aj2ryy*
     & aj2sx+aj2syy*aj2rx)*aj2rxrs+(aj2syy*aj2sx+2*aj2sy*aj2sxy)*
     & aj2rxss+aj2rxyy*aj2rxr+aj2sxyy*aj2rxs
           t1 = aj2ry**2
           t7 = aj2sy**2
           aj2rxyyy = aj2ry*t1*aj2rxrrr+3*t1*aj2sy*aj2rxrrs+3*aj2ry*t7*
     & aj2rxrss+t7*aj2sy*aj2rxsss+3*aj2ry*aj2ryy*aj2rxrr+(3*aj2syy*
     & aj2ry+3*aj2sy*aj2ryy)*aj2rxrs+3*aj2syy*aj2sy*aj2rxss+aj2ryyy*
     & aj2rxr+aj2syyy*aj2rxs
           t1 = aj2rx**2
           t7 = aj2sx**2
           aj2sxxxx = t1*aj2rx*aj2sxrrr+3*t1*aj2sx*aj2sxrrs+3*aj2rx*t7*
     & aj2sxrss+t7*aj2sx*aj2sxsss+3*aj2rx*aj2rxx*aj2sxrr+(3*aj2sxx*
     & aj2rx+3*aj2sx*aj2rxx)*aj2sxrs+3*aj2sxx*aj2sx*aj2sxss+aj2rxxx*
     & aj2sxr+aj2sxxx*aj2sxs
           t1 = aj2rx**2
           t10 = aj2sx**2
           aj2sxxxy = aj2ry*t1*aj2sxrrr+(aj2sy*t1+2*aj2ry*aj2sx*aj2rx)*
     & aj2sxrrs+(aj2ry*t10+2*aj2sy*aj2sx*aj2rx)*aj2sxrss+aj2sy*t10*
     & aj2sxsss+(2*aj2rxy*aj2rx+aj2ry*aj2rxx)*aj2sxrr+(aj2ry*aj2sxx+2*
     & aj2sx*aj2rxy+2*aj2sxy*aj2rx+aj2sy*aj2rxx)*aj2sxrs+(aj2sy*
     & aj2sxx+2*aj2sxy*aj2sx)*aj2sxss+aj2rxxy*aj2sxr+aj2sxxy*aj2sxs
           t1 = aj2ry**2
           t4 = aj2sy*aj2ry
           t8 = aj2sy*aj2rx+aj2ry*aj2sx
           t16 = aj2sy**2
           aj2sxxyy = t1*aj2rx*aj2sxrrr+(t4*aj2rx+aj2ry*t8)*aj2sxrrs+(
     & t4*aj2sx+aj2sy*t8)*aj2sxrss+t16*aj2sx*aj2sxsss+(aj2ryy*aj2rx+2*
     & aj2ry*aj2rxy)*aj2sxrr+(2*aj2ry*aj2sxy+2*aj2sy*aj2rxy+aj2ryy*
     & aj2sx+aj2syy*aj2rx)*aj2sxrs+(aj2syy*aj2sx+2*aj2sy*aj2sxy)*
     & aj2sxss+aj2rxyy*aj2sxr+aj2sxyy*aj2sxs
           t1 = aj2ry**2
           t7 = aj2sy**2
           aj2sxyyy = aj2ry*t1*aj2sxrrr+3*t1*aj2sy*aj2sxrrs+3*aj2ry*t7*
     & aj2sxrss+t7*aj2sy*aj2sxsss+3*aj2ry*aj2ryy*aj2sxrr+(3*aj2syy*
     & aj2ry+3*aj2sy*aj2ryy)*aj2sxrs+3*aj2syy*aj2sy*aj2sxss+aj2ryyy*
     & aj2sxr+aj2syyy*aj2sxs
           t1 = aj2rx**2
           t7 = aj2sx**2
           aj2ryxxx = t1*aj2rx*aj2ryrrr+3*t1*aj2sx*aj2ryrrs+3*aj2rx*t7*
     & aj2ryrss+t7*aj2sx*aj2rysss+3*aj2rx*aj2rxx*aj2ryrr+(3*aj2sxx*
     & aj2rx+3*aj2sx*aj2rxx)*aj2ryrs+3*aj2sxx*aj2sx*aj2ryss+aj2rxxx*
     & aj2ryr+aj2sxxx*aj2rys
           t1 = aj2rx**2
           t10 = aj2sx**2
           aj2ryxxy = aj2ry*t1*aj2ryrrr+(aj2sy*t1+2*aj2ry*aj2sx*aj2rx)*
     & aj2ryrrs+(aj2ry*t10+2*aj2sy*aj2sx*aj2rx)*aj2ryrss+aj2sy*t10*
     & aj2rysss+(2*aj2rxy*aj2rx+aj2ry*aj2rxx)*aj2ryrr+(aj2ry*aj2sxx+2*
     & aj2sx*aj2rxy+2*aj2sxy*aj2rx+aj2sy*aj2rxx)*aj2ryrs+(aj2sy*
     & aj2sxx+2*aj2sxy*aj2sx)*aj2ryss+aj2rxxy*aj2ryr+aj2sxxy*aj2rys
           t1 = aj2ry**2
           t4 = aj2sy*aj2ry
           t8 = aj2sy*aj2rx+aj2ry*aj2sx
           t16 = aj2sy**2
           aj2ryxyy = t1*aj2rx*aj2ryrrr+(t4*aj2rx+aj2ry*t8)*aj2ryrrs+(
     & t4*aj2sx+aj2sy*t8)*aj2ryrss+t16*aj2sx*aj2rysss+(aj2ryy*aj2rx+2*
     & aj2ry*aj2rxy)*aj2ryrr+(2*aj2ry*aj2sxy+2*aj2sy*aj2rxy+aj2ryy*
     & aj2sx+aj2syy*aj2rx)*aj2ryrs+(aj2syy*aj2sx+2*aj2sy*aj2sxy)*
     & aj2ryss+aj2rxyy*aj2ryr+aj2sxyy*aj2rys
           t1 = aj2ry**2
           t7 = aj2sy**2
           aj2ryyyy = aj2ry*t1*aj2ryrrr+3*t1*aj2sy*aj2ryrrs+3*aj2ry*t7*
     & aj2ryrss+t7*aj2sy*aj2rysss+3*aj2ry*aj2ryy*aj2ryrr+(3*aj2syy*
     & aj2ry+3*aj2sy*aj2ryy)*aj2ryrs+3*aj2syy*aj2sy*aj2ryss+aj2ryyy*
     & aj2ryr+aj2syyy*aj2rys
           t1 = aj2rx**2
           t7 = aj2sx**2
           aj2syxxx = t1*aj2rx*aj2syrrr+3*t1*aj2sx*aj2syrrs+3*aj2rx*t7*
     & aj2syrss+t7*aj2sx*aj2sysss+3*aj2rx*aj2rxx*aj2syrr+(3*aj2sxx*
     & aj2rx+3*aj2sx*aj2rxx)*aj2syrs+3*aj2sxx*aj2sx*aj2syss+aj2rxxx*
     & aj2syr+aj2sxxx*aj2sys
           t1 = aj2rx**2
           t10 = aj2sx**2
           aj2syxxy = aj2ry*t1*aj2syrrr+(aj2sy*t1+2*aj2ry*aj2sx*aj2rx)*
     & aj2syrrs+(aj2ry*t10+2*aj2sy*aj2sx*aj2rx)*aj2syrss+aj2sy*t10*
     & aj2sysss+(2*aj2rxy*aj2rx+aj2ry*aj2rxx)*aj2syrr+(aj2ry*aj2sxx+2*
     & aj2sx*aj2rxy+2*aj2sxy*aj2rx+aj2sy*aj2rxx)*aj2syrs+(aj2sy*
     & aj2sxx+2*aj2sxy*aj2sx)*aj2syss+aj2rxxy*aj2syr+aj2sxxy*aj2sys
           t1 = aj2ry**2
           t4 = aj2sy*aj2ry
           t8 = aj2sy*aj2rx+aj2ry*aj2sx
           t16 = aj2sy**2
           aj2syxyy = t1*aj2rx*aj2syrrr+(t4*aj2rx+aj2ry*t8)*aj2syrrs+(
     & t4*aj2sx+aj2sy*t8)*aj2syrss+t16*aj2sx*aj2sysss+(aj2ryy*aj2rx+2*
     & aj2ry*aj2rxy)*aj2syrr+(2*aj2ry*aj2sxy+2*aj2sy*aj2rxy+aj2ryy*
     & aj2sx+aj2syy*aj2rx)*aj2syrs+(aj2syy*aj2sx+2*aj2sy*aj2sxy)*
     & aj2syss+aj2rxyy*aj2syr+aj2sxyy*aj2sys
           t1 = aj2ry**2
           t7 = aj2sy**2
           aj2syyyy = aj2ry*t1*aj2syrrr+3*t1*aj2sy*aj2syrrs+3*aj2ry*t7*
     & aj2syrss+t7*aj2sy*aj2sysss+3*aj2ry*aj2ryy*aj2syrr+(3*aj2syy*
     & aj2ry+3*aj2sy*aj2ryy)*aj2syrs+3*aj2syy*aj2sy*aj2syss+aj2ryyy*
     & aj2syr+aj2syyy*aj2sys
           do n=na,nb

            ! *** first eval the first and second derivatives to 4th order
            ! evalParametricDerivativesComponents1(u,i1,i2,i3,m, v,DIM,ORDER,MAXDERIV)
            uu = u(i1,i2,i3,n)
            uur = (u(i1-2,i2,i3,n)-8.*u(i1-1,i2,i3,n)+8.*u(i1+1,i2,i3,
     & n)-u(i1+2,i2,i3,n))/(12.*dr(0))
            uus = (u(i1,i2-2,i3,n)-8.*u(i1,i2-1,i3,n)+8.*u(i1,i2+1,i3,
     & n)-u(i1,i2+2,i3,n))/(12.*dr(1))
            uurr = (-u(i1-2,i2,i3,n)+16.*u(i1-1,i2,i3,n)-30.*u(i1,i2,
     & i3,n)+16.*u(i1+1,i2,i3,n)-u(i1+2,i2,i3,n))/(12.*dr(0)**2)
            uurs = ((u(i1-2,i2-2,i3,n)-8.*u(i1-2,i2-1,i3,n)+8.*u(i1-2,
     & i2+1,i3,n)-u(i1-2,i2+2,i3,n))/(12.*dr(1))-8.*(u(i1-1,i2-2,i3,n)
     & -8.*u(i1-1,i2-1,i3,n)+8.*u(i1-1,i2+1,i3,n)-u(i1-1,i2+2,i3,n))/(
     & 12.*dr(1))+8.*(u(i1+1,i2-2,i3,n)-8.*u(i1+1,i2-1,i3,n)+8.*u(i1+
     & 1,i2+1,i3,n)-u(i1+1,i2+2,i3,n))/(12.*dr(1))-(u(i1+2,i2-2,i3,n)-
     & 8.*u(i1+2,i2-1,i3,n)+8.*u(i1+2,i2+1,i3,n)-u(i1+2,i2+2,i3,n))/(
     & 12.*dr(1)))/(12.*dr(0))
            uuss = (-u(i1,i2-2,i3,n)+16.*u(i1,i2-1,i3,n)-30.*u(i1,i2,
     & i3,n)+16.*u(i1,i2+1,i3,n)-u(i1,i2+2,i3,n))/(12.*dr(1)**2)

            ux = aj4rx*uur+aj4sx*uus
            uy = aj4ry*uur+aj4sy*uus

            t1 = aj4rx**2
            t6 = aj4sx**2
            uxx = t1*uurr+2*aj4rx*aj4sx*uurs+t6*uuss+aj4rxx*uur+aj4sxx*
     & uus
            uxy = aj4ry*aj4rx*uurr+(aj4sy*aj4rx+aj4ry*aj4sx)*uurs+
     & aj4sy*aj4sx*uuss+aj4rxy*uur+aj4sxy*uus
            t1 = aj4ry**2
            t6 = aj4sy**2
            uyy = t1*uurr+2*aj4ry*aj4sy*uurs+t6*uuss+aj4ryy*uur+aj4syy*
     & uus

            ! Now evaluate the 3rd and 4th derivatives to 2nd order
            uu = u(i1,i2,i3,n)
            uur = (-u(i1-1,i2,i3,n)+u(i1+1,i2,i3,n))/(2.*dr(0))
            uus = (-u(i1,i2-1,i3,n)+u(i1,i2+1,i3,n))/(2.*dr(1))
            uurr = (u(i1-1,i2,i3,n)-2.*u(i1,i2,i3,n)+u(i1+1,i2,i3,n))/(
     & dr(0)**2)
            uurs = (-(-u(i1-1,i2-1,i3,n)+u(i1-1,i2+1,i3,n))/(2.*dr(1))+
     & (-u(i1+1,i2-1,i3,n)+u(i1+1,i2+1,i3,n))/(2.*dr(1)))/(2.*dr(0))
            uuss = (u(i1,i2-1,i3,n)-2.*u(i1,i2,i3,n)+u(i1,i2+1,i3,n))/(
     & dr(1)**2)
            uurrr = (-u(i1-2,i2,i3,n)+2.*u(i1-1,i2,i3,n)-2.*u(i1+1,i2,
     & i3,n)+u(i1+2,i2,i3,n))/(2.*dr(0)**3)
            uurrs = ((-u(i1-1,i2-1,i3,n)+u(i1-1,i2+1,i3,n))/(2.*dr(1))-
     & 2.*(-u(i1,i2-1,i3,n)+u(i1,i2+1,i3,n))/(2.*dr(1))+(-u(i1+1,i2-1,
     & i3,n)+u(i1+1,i2+1,i3,n))/(2.*dr(1)))/(dr(0)**2)
            uurss = (-(u(i1-1,i2-1,i3,n)-2.*u(i1-1,i2,i3,n)+u(i1-1,i2+
     & 1,i3,n))/(dr(1)**2)+(u(i1+1,i2-1,i3,n)-2.*u(i1+1,i2,i3,n)+u(i1+
     & 1,i2+1,i3,n))/(dr(1)**2))/(2.*dr(0))
            uusss = (-u(i1,i2-2,i3,n)+2.*u(i1,i2-1,i3,n)-2.*u(i1,i2+1,
     & i3,n)+u(i1,i2+2,i3,n))/(2.*dr(1)**3)
            uurrrr = (u(i1-2,i2,i3,n)-4.*u(i1-1,i2,i3,n)+6.*u(i1,i2,i3,
     & n)-4.*u(i1+1,i2,i3,n)+u(i1+2,i2,i3,n))/(dr(0)**4)
            uurrrs = (-(-u(i1-2,i2-1,i3,n)+u(i1-2,i2+1,i3,n))/(2.*dr(1)
     & )+2.*(-u(i1-1,i2-1,i3,n)+u(i1-1,i2+1,i3,n))/(2.*dr(1))-2.*(-u(
     & i1+1,i2-1,i3,n)+u(i1+1,i2+1,i3,n))/(2.*dr(1))+(-u(i1+2,i2-1,i3,
     & n)+u(i1+2,i2+1,i3,n))/(2.*dr(1)))/(2.*dr(0)**3)
            uurrss = ((u(i1-1,i2-1,i3,n)-2.*u(i1-1,i2,i3,n)+u(i1-1,i2+
     & 1,i3,n))/(dr(1)**2)-2.*(u(i1,i2-1,i3,n)-2.*u(i1,i2,i3,n)+u(i1,
     & i2+1,i3,n))/(dr(1)**2)+(u(i1+1,i2-1,i3,n)-2.*u(i1+1,i2,i3,n)+u(
     & i1+1,i2+1,i3,n))/(dr(1)**2))/(dr(0)**2)
            uursss = (-(-u(i1-1,i2-2,i3,n)+2.*u(i1-1,i2-1,i3,n)-2.*u(
     & i1-1,i2+1,i3,n)+u(i1-1,i2+2,i3,n))/(2.*dr(1)**3)+(-u(i1+1,i2-2,
     & i3,n)+2.*u(i1+1,i2-1,i3,n)-2.*u(i1+1,i2+1,i3,n)+u(i1+1,i2+2,i3,
     & n))/(2.*dr(1)**3))/(2.*dr(0))
            uussss = (u(i1,i2-2,i3,n)-4.*u(i1,i2-1,i3,n)+6.*u(i1,i2,i3,
     & n)-4.*u(i1,i2+1,i3,n)+u(i1,i2+2,i3,n))/(dr(1)**4)

            t1 = aj2rx**2
            t7 = aj2sx**2
            uxxx = t1*aj2rx*uurrr+3*t1*aj2sx*uurrs+3*aj2rx*t7*uurss+t7*
     & aj2sx*uusss+3*aj2rx*aj2rxx*uurr+(3*aj2sxx*aj2rx+3*aj2sx*aj2rxx)
     & *uurs+3*aj2sxx*aj2sx*uuss+aj2rxxx*uur+aj2sxxx*uus
            t1 = aj2rx**2
            t10 = aj2sx**2
            uxxy = aj2ry*t1*uurrr+(aj2sy*t1+2*aj2ry*aj2sx*aj2rx)*uurrs+
     & (aj2ry*t10+2*aj2sy*aj2sx*aj2rx)*uurss+aj2sy*t10*uusss+(2*
     & aj2rxy*aj2rx+aj2ry*aj2rxx)*uurr+(aj2ry*aj2sxx+2*aj2sx*aj2rxy+2*
     & aj2sxy*aj2rx+aj2sy*aj2rxx)*uurs+(aj2sy*aj2sxx+2*aj2sxy*aj2sx)*
     & uuss+aj2rxxy*uur+aj2sxxy*uus
            t1 = aj2ry**2
            t4 = aj2sy*aj2ry
            t8 = aj2sy*aj2rx+aj2ry*aj2sx
            t16 = aj2sy**2
            uxyy = t1*aj2rx*uurrr+(t4*aj2rx+aj2ry*t8)*uurrs+(t4*aj2sx+
     & aj2sy*t8)*uurss+t16*aj2sx*uusss+(aj2ryy*aj2rx+2*aj2ry*aj2rxy)*
     & uurr+(2*aj2ry*aj2sxy+2*aj2sy*aj2rxy+aj2ryy*aj2sx+aj2syy*aj2rx)*
     & uurs+(aj2syy*aj2sx+2*aj2sy*aj2sxy)*uuss+aj2rxyy*uur+aj2sxyy*uus
            t1 = aj2ry**2
            t7 = aj2sy**2
            uyyy = aj2ry*t1*uurrr+3*t1*aj2sy*uurrs+3*aj2ry*t7*uurss+t7*
     & aj2sy*uusss+3*aj2ry*aj2ryy*uurr+(3*aj2syy*aj2ry+3*aj2sy*aj2ryy)
     & *uurs+3*aj2syy*aj2sy*uuss+aj2ryyy*uur+aj2syyy*uus

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
            uxxxx = t2*uurrrr+4*t1*aj2rx*aj2sx*uurrrs+6*t1*t8*uurrss+4*
     & aj2rx*t8*aj2sx*uursss+t16*uussss+6*t1*aj2rxx*uurrr+(7*aj2sx*
     & aj2rx*aj2rxx+aj2sxx*t1+aj2rx*t28+aj2rx*t30)*uurrs+(aj2sx*t28+7*
     & t25*aj2sx+aj2rxx*t8+aj2sx*t30)*uurss+6*t8*aj2sxx*uusss+(4*
     & aj2rx*aj2rxxx+3*t46)*uurr+(4*aj2sxxx*aj2rx+4*aj2sx*aj2rxxx+6*
     & aj2sxx*aj2rxx)*uurs+(4*aj2sxxx*aj2sx+3*t60)*uuss+aj2rxxxx*uur+
     & aj2sxxxx*uus
            t1 = aj2rx**2
            t2 = t1*aj2rx
            t11 = aj2ry*aj2rx
            t12 = aj2sx**2
            t19 = aj2sy*aj2rx
            t22 = t12*aj2sx
            t33 = aj2sx*aj2rxy
            t37 = aj2sxy*aj2rx
            t39 = 2*t33+2*t37
            t44 = 3*aj2sxx*aj2rx+3*aj2sx*aj2rxx
            uxxxy = aj2ry*t2*uurrrr+(3*aj2ry*t1*aj2sx+aj2sy*t2)*uurrrs+
     & (3*t11*t12+3*aj2sy*t1*aj2sx)*uurrss+(3*t19*t12+aj2ry*t22)*
     & uursss+aj2sy*t22*uussss+(3*aj2rxy*t1+3*t11*aj2rxx)*uurrr+(4*
     & t33*aj2rx+aj2sxy*t1+aj2rx*t39+aj2ry*t44+3*t19*aj2rxx)*uurrs+(
     & aj2rxy*t12+aj2sy*t44+3*aj2ry*aj2sxx*aj2sx+4*t37*aj2sx+aj2sx*
     & t39)*uurss+(3*aj2sy*aj2sxx*aj2sx+3*t12*aj2sxy)*uusss+(3*aj2rxy*
     & aj2rxx+3*aj2rx*aj2rxxy+aj2ry*aj2rxxx)*uurr+(3*aj2sxx*aj2rxy+
     & aj2ry*aj2sxxx+3*aj2sxy*aj2rxx+3*aj2sx*aj2rxxy+3*aj2sxxy*aj2rx+
     & aj2sy*aj2rxxx)*uurs+(3*aj2sxxy*aj2sx+aj2sy*aj2sxxx+3*aj2sxy*
     & aj2sxx)*uuss+aj2rxxxy*uur+aj2sxxxy*uus
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
            uxxyy = t1*t2*uurrrr+(t5*t2+aj2ry*t11)*uurrrs+(aj2sy*t11+
     & aj2ry*t21)*uurrss+(aj2sy*t21+t5*t16)*uursss+t29*t16*uussss+(2*
     & aj2ry*aj2rxy*aj2rx+aj2ry*t38+aj2ryy*t2)*uurrr+(aj2sy*t38+2*
     & aj2sy*aj2rxy*aj2rx+2*aj2ryy*aj2sx*aj2rx+aj2syy*t2+aj2ry*t57+
     & aj2ry*t60)*uurrs+(aj2sy*t57+aj2ry*t68+aj2ryy*t16+2*aj2ry*
     & aj2sxy*aj2sx+2*aj2syy*aj2sx*aj2rx+aj2sy*t60)*uurss+(2*aj2sy*
     & aj2sxy*aj2sx+aj2sy*t68+aj2syy*t16)*uusss+(2*aj2rx*aj2rxyy+
     & aj2ryy*aj2rxx+2*aj2ry*aj2rxxy+2*t92)*uurr+(4*aj2sxy*aj2rxy+2*
     & aj2ry*aj2sxxy+aj2ryy*aj2sxx+2*aj2sy*aj2rxxy+2*aj2sxyy*aj2rx+
     & aj2syy*aj2rxx+2*aj2sx*aj2rxyy)*uurs+(2*t110+2*aj2sy*aj2sxxy+
     & aj2syy*aj2sxx+2*aj2sx*aj2sxyy)*uuss+aj2rxxyy*uur+aj2sxxyy*uus
            t1 = aj2ry**2
            t7 = aj2sy*aj2ry
            t11 = aj2sy*aj2rx+aj2ry*aj2sx
            t13 = t7*aj2rx+aj2ry*t11
            t20 = t7*aj2sx+aj2sy*t11
            t25 = aj2sy**2
            t33 = aj2ryy*aj2rx
            t34 = aj2ry*aj2rxy
            t35 = t33+t34
            t38 = t33+2*t34
            t49 = aj2ry*aj2sxy
            t51 = aj2sy*aj2rxy
            t53 = aj2ryy*aj2sx
            t54 = aj2syy*aj2rx
            t55 = 2*t49+2*t51+t53+t54
            t57 = t51+t53+t54+t49
            t62 = aj2syy*aj2sx
            t63 = aj2sy*aj2sxy
            t65 = t62+2*t63
            t69 = t63+t62
            uxyyy = t1*aj2ry*aj2rx*uurrrr+(aj2sy*t1*aj2rx+aj2ry*t13)*
     & uurrrs+(aj2sy*t13+aj2ry*t20)*uurrss+(aj2sy*t20+aj2ry*t25*aj2sx)
     & *uursss+t25*aj2sy*aj2sx*uussss+(aj2ry*t35+aj2ry*t38+aj2ryy*
     & aj2ry*aj2rx)*uurrr+(aj2sy*t38+aj2sy*t35+aj2ryy*t11+aj2syy*
     & aj2ry*aj2rx+aj2ry*t55+aj2ry*t57)*uurrs+(aj2sy*t55+aj2ry*t65+
     & aj2ryy*aj2sy*aj2sx+aj2ry*t69+aj2syy*t11+aj2sy*t57)*uurss+(
     & aj2sy*t69+aj2sy*t65+aj2syy*aj2sy*aj2sx)*uusss+(3*aj2ry*aj2rxyy+
     & aj2ryyy*aj2rx+3*aj2ryy*aj2rxy)*uurr+(3*aj2ry*aj2sxyy+3*aj2sy*
     & aj2rxyy+aj2syyy*aj2rx+3*aj2syy*aj2rxy+aj2ryyy*aj2sx+3*aj2ryy*
     & aj2sxy)*uurs+(aj2syyy*aj2sx+3*aj2sy*aj2sxyy+3*aj2syy*aj2sxy)*
     & uuss+aj2rxyyy*uur+aj2sxyyy*uus
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
            uyyyy = t2*uurrrr+4*t1*aj2ry*aj2sy*uurrrs+6*t1*t8*uurrss+4*
     & aj2ry*t8*aj2sy*uursss+t16*uussss+6*t1*aj2ryy*uurrr+(7*aj2sy*
     & aj2ry*aj2ryy+aj2syy*t1+aj2ry*t28+aj2ry*t30)*uurrs+(aj2sy*t28+7*
     & t25*aj2sy+aj2ryy*t8+aj2sy*t30)*uurss+6*t8*aj2syy*uusss+(4*
     & aj2ry*aj2ryyy+3*t46)*uurr+(4*aj2syyy*aj2ry+4*aj2sy*aj2ryyy+6*
     & aj2syy*aj2ryy)*uurs+(4*aj2syyy*aj2sy+3*t60)*uuss+aj2ryyyy*uur+
     & aj2syyyy*uus


            uLap = uxx+uyy
            utt  = uLap

            r = sqrt( xy(i1,i2,i3,0)**2 + xy(i1,i2,i3,1)**2 )
            ! outward normal:
            an1= -rsxy(i1,i2,i3,axis,0)*is2
            an2= -rsxy(i1,i2,i3,axis,1)*is2
            aNorm = sqrt( an1**2 + an2**2 )
            an1=an1/aNorm
            an2=an2/aNorm
            uri = an1*ux+an2*uy     ! u.n = u.r

            uxri = an1*uxx+an2*uxy   ! (ux).r = n.grad(ux)
            uyri = an1*uxy+an2*uyy


            uxxri = an1*uxxx+an2*uxxy
            uxyri = an1*uxxy+an2*uxyy
            uyyri = an1*uxyy+an2*uyyy

            uxxxri = an1*uxxxx+an2*uxxxy
            uxxyri = an1*uxxxy+an2*uxxyy
            uxyyri = an1*uxxyy+an2*uxyyy
            uyyyri = an1*uxyyy+an2*uyyyy

            ut   =-c*( uri   + u(i1,i2,i3,n)/(2.*r) +   hu(ii,n) )
            utx  =-c*( uxri  +            ux/(2.*r) +  hux(ii,n) )
            uty  =-c*( uyri  +            uy/(2.*r) +  huy(ii,n) )
            utxx =-c*( uxxri +           uxx/(2.*r) + huxx(ii,n) )
            utxy =-c*( uxyri +           uxy/(2.*r) + huxy(ii,n) )
            utyy =-c*( uyyri +           uyy/(2.*r) + huyy(ii,n) )

            utxxx=-c*( uxxxri +         uxxx/(2.*r) + huxxx(ii,n) )
            utxxy=-c*( uxxyri +         uxxy/(2.*r) + huxxy(ii,n) )
            utxyy=-c*( uxyyri +         uxyy/(2.*r) + huxyy(ii,n) )
            utyyy=-c*( uyyyri +         uyyy/(2.*r) + huyyy(ii,n) )

            ! *** note: could also just multiply dt by c ****

            uttt = (utxx+utyy)*c**2

            uttx = (uxxx+uxyy)*c**2
            utty = (uxxy+uyyy)*c**2

            utttt= (uxxxx+2.*uxxyy+uyyyy)*c**4
            uttxx= (uxxxx+uxxyy)*c**2
            uttyy= (uxxyy+uyyyy)*c**2
            uttxy= (uxxxy+uxyyy)*c**2

            utttx= (utxxx+utxyy)*c**2
            uttty= (utxxy+utyyy)*c**2

  ! **** check the 4th-order terms ***
! t^4+(4*y+4*x)*t^3+(6*y^2+6*x^2+12*x*y)*t^2+(4*y^3+12*x^2*y+4*x^3+12*x*y^2)*t+x^4+y^4+6*x^2*y^2+4*x*y^3+4*x^3*y

            if( debug.gt.0 )then
             tm=t-dt
             call ogderiv(ep,1,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),z0,
     & tm,n,ut )
             call ogderiv(ep,1,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),z0,
     & tm,n,utx )
             call ogderiv(ep,1,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),z0,
     & tm,n,uty )

             call ogderiv(ep,3,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),z0,
     & tm,n,uttt )
             call ogderiv(ep,1,2,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),z0,
     & tm,n,utxx)
             call ogderiv(ep,1,1,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),z0,
     & tm,n,utxy)
             call ogderiv(ep,1,0,2,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),z0,
     & tm,n,utyy)

             call ogderiv(ep,3,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),z0,
     & tm,n,utttx )
             call ogderiv(ep,3,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),z0,
     & tm,n,uttty )

             call ogderiv(ep,1,3,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),z0,
     & tm,n,utxxx)
             call ogderiv(ep,1,2,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),z0,
     & tm,n,utxxy)
             call ogderiv(ep,1,1,2,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),z0,
     & tm,n,utxyy)
             call ogderiv(ep,1,0,3,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),z0,
     & tm,n,utyyy)


            end if

            hx = xy(i1-is1,i2-is2,i3,0)- xy(i1,i2,i3,0)
            hy = xy(i1-is1,i2-is2,i3,1)- xy(i1,i2,i3,1)
            i1m=i1-is1
            i2m=i2-is2
             um1 = u(i1,i2,i3,n) + dt*ut  + hx*ux + hy*uy +.5*dt*dt*
     & utt + dt*hx*utx + dt*hy*uty + .5*( hx**2*uxx +2.*hx*hy*uxy+hy**
     & 2*uyy )+(1./6.)*( dt**3*uttt + 3.*dt**2*(hx*uttx+hy*utty) + 3.*
     & dt*(hx**2*utxx+2.*hx*hy*utxy+hy**2*utyy) +hx**3*uxxx + 3.*hx**
     & 2*hy*uxxy + 3.*hx*hy**2*uxyy + hy**3*uyyy ) +(1./24.)*( dt**4*
     & utttt + 4.*dt**3*( hx*utttx +hy*uttty) +6.*dt**2*( hx**2*uttxx+
     & hy**2*uttyy+2.*hx*hy*uttxy )+4.*dt*( hx**3*utxxx + hy**3*utyyy 
     & + 3.*hx**2*hy*utxxy + 3.*hx*hy**2*utxyy )+ hx**4*uxxxx + 4.*hx*
     & *3*hy*uxxxy + 6.*hx**2*hy**2*uxxyy + 4.*hx*hy**3*uxyyy + hy**4*
     & uyyyy )  +dt*alpha*( -u(i1m-2*is2,i2m-2*is1,i3,n)+4.*u(i1m-is2,
     & i2m-is1,i3,n)-6.*u(i1m,i2m,i3,n)+4.*u(i1m+is2,i2m+is1,i3,n)-u(
     & i1m+2*is2,i2m+2*is1,i3,n) )

            hx = xy(i1-2*is1,i2-2*is2,i3,0)- xy(i1,i2,i3,0)
            hy = xy(i1-2*is1,i2-2*is2,i3,1)- xy(i1,i2,i3,1)
            i1m=i1-2*is1
            i2m=i2-2*is2
             um2 = u(i1,i2,i3,n) + dt*ut  + hx*ux + hy*uy +.5*dt*dt*
     & utt + dt*hx*utx + dt*hy*uty + .5*( hx**2*uxx +2.*hx*hy*uxy+hy**
     & 2*uyy )+(1./6.)*( dt**3*uttt + 3.*dt**2*(hx*uttx+hy*utty) + 3.*
     & dt*(hx**2*utxx+2.*hx*hy*utxy+hy**2*utyy) +hx**3*uxxx + 3.*hx**
     & 2*hy*uxxy + 3.*hx*hy**2*uxyy + hy**3*uyyy ) +(1./24.)*( dt**4*
     & utttt + 4.*dt**3*( hx*utttx +hy*uttty) +6.*dt**2*( hx**2*uttxx+
     & hy**2*uttyy+2.*hx*hy*uttxy )+4.*dt*( hx**3*utxxx + hy**3*utyyy 
     & + 3.*hx**2*hy*utxxy + 3.*hx*hy**2*utxyy )+ hx**4*uxxxx + 4.*hx*
     & *3*hy*uxxxy + 6.*hx**2*hy**2*uxxyy + 4.*hx*hy**3*uxyyy + hy**4*
     & uyyyy )  +dt*alpha*( -u(i1m-2*is2,i2m-2*is1,i3,n)+4.*u(i1m-is2,
     & i2m-is1,i3,n)-6.*u(i1m,i2m,i3,n)+4.*u(i1m+is2,i2m+is1,i3,n)-u(
     & i1m+2*is2,i2m+2*is1,i3,n) )

            u1(i1-is1,i2-is2,i3,n)=um1
            u1(i1-2*is1,i2-2*is2,i3,n)=um2

            if( debug.gt.0 )then
             utrue=ogf(ep,xy(i1-is1,i2-is2,i3,0),xy(i1-is1,i2-is2,i3,1)
     & ,z0,n,t)
             write(*,'(" radEval: t,ii,n,um1,utrue,err=",e8.2,i3,i2,1x,
     & 3e10.2)') t,ii,n,um1,utrue,um1-utrue
             u1(i1-is1,i2-is2,i3,n)=utrue

             utrue=ogf(ep,xy(i1-2*is1,i2-2*is2,i3,0),xy(i1-2*is1,i2-2*
     & is2,i3,1),z0,n,t)
             write(*,'("        : t,ii,n,um2,utrue,err=",e8.2,i3,i2,1x,
     & 3e10.2)') t,ii,n,um2,utrue,um2-utrue
             u1(i1-2*is1,i2-2*is2,i3,n)=utrue
            end if


            enddo
            ii=ii+1
          enddo
          enddo

         else

          write(*,'("Unimplemented order of accuracy")')
          stop 8263


         end if


        else
          write(*,'("Unknown Kernel type to evaluate")')
          stop 8264
        end if


      else
                                ! 3D
        stop 6676
      end if


      ! ============ Now save some derivatives ====================

      if( kernelType.eq.cylindrical )then
        ! -- for cylindrical we will need to do a periodic update first ---
        ! update periodic end-pts and ghost-points so we can compute derivatives ux, uy, ... on the boundary
        ! 
        !        |                         |
        !   X  X |                         |  X X
        !   X  X +-------------------------+  X X
        !   X  X n1a                     n1b  X X
        !
        if( axis.eq.1 )then
          ! [n1a,n1b] : indexRange (does not include periodic image) u(n1b+1,i2,i3,n)=u(n1a,i2,i3,n)
          i3=n3a
          np1=n1b-n1a+1
          numGhost=orderOfAccuracy/2
          if( side.eq.0 )then
            m2a=n2a-numGhost
            m2b=n2a+numGhost
          else
            m2a=n2b-numGhost
            m2b=n2b+numGhost
          end if
          do n=na,nb
          do i2=m2a,m2b
            do i1=nd1a,n1a-1
              u1(i1,i2,i3,n)=u1(i1+np1,i2,i3,n)
            end do
            do i1=n1b+1,nd1b
              u1(i1,i2,i3,n)=u1(i1-np1,i2,i3,n)
            end do
          end do
          end do
        else
          stop 5532
        end if
      end if

      i3=n3a
      if( kernelType.eq.planar .and. orderOfAccuracy.eq.2 ) then
        ! evaluate first two x-derivatives to second-order
        if( axis.eq.0 )then
          if( side.eq.0 )then
            i1=n1a
          else
            i1=n1b
          end if
          do n=na,nb
            do i2=n2a,n2b
              uSave(i2,m,1,n)=(-u1(i1-1,i2,i3,n)+u1(i1+1,i2,i3,n))/(2.*
     & dx(0))
              ! uSave(i2,m,2,n)=u1xx2(i1,i2,i3,n)
            end do
          end do
        else if( axis.eq.1 )then
          if( side.eq.0 )then
            i2=n2a
          else
            i2=n2b
          end if
          do n=na,nb
            do i1=n1a,n1b
              uSave(i1,m,1,n)=(-u1(i1,i2-1,i3,n)+u1(i1,i2+1,i3,n))/(2.*
     & dx(1))
              ! uSave(i1,m,2,n)=u1yy2(i1,i2,i3,n)
            end do
          end do
        else
          stop 6639
        end if
      else if( kernelType.eq.planar .and. orderOfAccuracy.eq.4 ) then
        ! evaluate first 3 derivatives 

        if( axis.eq.0 )then
          if( side.eq.0 )then
            i1=n1a
          else
            i1=n1b
          end if
          do n=na,nb
            do i2=n2a,n2b
              uSave(i2,m,1,n)=(u1(i1-2,i2,i3,n)-8.*u1(i1-1,i2,i3,n)+8.*
     & u1(i1+1,i2,i3,n)-u1(i1+2,i2,i3,n))/(12.*dx(0))
              uSave(i2,m,2,n)=(-u1(i1-2,i2,i3,n)+16.*u1(i1-1,i2,i3,n)-
     & 30.*u1(i1,i2,i3,n)+16.*u1(i1+1,i2,i3,n)-u1(i1+2,i2,i3,n))/(12.*
     & dx(0)**2)
              uSave(i2,m,3,n)=(-u1(i1-2,i2,i3,n)+2.*u1(i1-1,i2,i3,n)-
     & 2.*u1(i1+1,i2,i3,n)+u1(i1+2,i2,i3,n))/(2.*dx(0)**3)
            end do
          end do
        else if( axis.eq.1 )then
          if( side.eq.0 )then
            i2=n2a
          else
            i2=n2b
          end if
          do n=na,nb
            do i1=n1a,n1b
              uSave(i1,m,1,n)=(u1(i1,i2-2,i3,n)-8.*u1(i1,i2-1,i3,n)+8.*
     & u1(i1,i2+1,i3,n)-u1(i1,i2+2,i3,n))/(12.*dx(1))
              uSave(i1,m,2,n)=(-u1(i1,i2-2,i3,n)+16.*u1(i1,i2-1,i3,n)-
     & 30.*u1(i1,i2,i3,n)+16.*u1(i1,i2+1,i3,n)-u1(i1,i2+2,i3,n))/(12.*
     & dx(1)**2)
              uSave(i1,m,3,n)=(-u1(i1,i2-2,i3,n)+2.*u1(i1,i2-1,i3,n)-
     & 2.*u1(i1,i2+1,i3,n)+u1(i1,i2+2,i3,n))/(2.*dx(1)**3)
            end do
          end do
        else
          stop 6639
        end if

      else if( kernelType.eq.cylindrical .and. orderOfAccuracy.eq.2 ) 
     & then
        ! evaluate first 3 derivatives 

          if( axis.eq.0 )then
            ii=nn2a
          else
            ii=nn1a
          end if
          do i2=nn2a,nn2b
          do i1=nn1a,nn1b
            aj2rx = rsxy(i1,i2,i3,0,0)
            aj2rxr = (-rsxy(i1-1,i2,i3,0,0)+rsxy(i1+1,i2,i3,0,0))/(2.*
     & dr(0))
            aj2rxs = (-rsxy(i1,i2-1,i3,0,0)+rsxy(i1,i2+1,i3,0,0))/(2.*
     & dr(1))
            aj2sx = rsxy(i1,i2,i3,1,0)
            aj2sxr = (-rsxy(i1-1,i2,i3,1,0)+rsxy(i1+1,i2,i3,1,0))/(2.*
     & dr(0))
            aj2sxs = (-rsxy(i1,i2-1,i3,1,0)+rsxy(i1,i2+1,i3,1,0))/(2.*
     & dr(1))
            aj2ry = rsxy(i1,i2,i3,0,1)
            aj2ryr = (-rsxy(i1-1,i2,i3,0,1)+rsxy(i1+1,i2,i3,0,1))/(2.*
     & dr(0))
            aj2rys = (-rsxy(i1,i2-1,i3,0,1)+rsxy(i1,i2+1,i3,0,1))/(2.*
     & dr(1))
            aj2sy = rsxy(i1,i2,i3,1,1)
            aj2syr = (-rsxy(i1-1,i2,i3,1,1)+rsxy(i1+1,i2,i3,1,1))/(2.*
     & dr(0))
            aj2sys = (-rsxy(i1,i2-1,i3,1,1)+rsxy(i1,i2+1,i3,1,1))/(2.*
     & dr(1))
            aj2rxx = aj2rx*aj2rxr+aj2sx*aj2rxs
            aj2rxy = aj2ry*aj2rxr+aj2sy*aj2rxs
            aj2sxx = aj2rx*aj2sxr+aj2sx*aj2sxs
            aj2sxy = aj2ry*aj2sxr+aj2sy*aj2sxs
            aj2ryx = aj2rx*aj2ryr+aj2sx*aj2rys
            aj2ryy = aj2ry*aj2ryr+aj2sy*aj2rys
            aj2syx = aj2rx*aj2syr+aj2sx*aj2sys
            aj2syy = aj2ry*aj2syr+aj2sy*aj2sys
            do n=na,nb
              uu = u1(i1,i2,i3,n)
              uur = (-u1(i1-1,i2,i3,n)+u1(i1+1,i2,i3,n))/(2.*dr(0))
              uus = (-u1(i1,i2-1,i3,n)+u1(i1,i2+1,i3,n))/(2.*dr(1))
              ux = aj2rx*uur+aj2sx*uus
              uy = aj2ry*uur+aj2sy*uus
              uSave(ii,m,1,n)=ux
              uSave(ii,m,2,n)=uy
            end do
            ii=ii+1
          end do
          end do


      else if( kernelType.eq.cylindrical .and. orderOfAccuracy.eq.4 ) 
     & then
        ! evaluate first 3 derivatives 

          if( axis.eq.0 )then
            ii=nn2a
          else
            ii=nn1a
          end if
          do i2=nn2a,nn2b
          do i1=nn1a,nn1b
            !  evalJacobianDerivatives(u,v,DIM,ORDER,MAXDERIV)
            aj4rx = rsxy(i1,i2,i3,0,0)
            aj4rxr = (rsxy(i1-2,i2,i3,0,0)-8.*rsxy(i1-1,i2,i3,0,0)+8.*
     & rsxy(i1+1,i2,i3,0,0)-rsxy(i1+2,i2,i3,0,0))/(12.*dr(0))
            aj4rxs = (rsxy(i1,i2-2,i3,0,0)-8.*rsxy(i1,i2-1,i3,0,0)+8.*
     & rsxy(i1,i2+1,i3,0,0)-rsxy(i1,i2+2,i3,0,0))/(12.*dr(1))
            aj4sx = rsxy(i1,i2,i3,1,0)
            aj4sxr = (rsxy(i1-2,i2,i3,1,0)-8.*rsxy(i1-1,i2,i3,1,0)+8.*
     & rsxy(i1+1,i2,i3,1,0)-rsxy(i1+2,i2,i3,1,0))/(12.*dr(0))
            aj4sxs = (rsxy(i1,i2-2,i3,1,0)-8.*rsxy(i1,i2-1,i3,1,0)+8.*
     & rsxy(i1,i2+1,i3,1,0)-rsxy(i1,i2+2,i3,1,0))/(12.*dr(1))
            aj4ry = rsxy(i1,i2,i3,0,1)
            aj4ryr = (rsxy(i1-2,i2,i3,0,1)-8.*rsxy(i1-1,i2,i3,0,1)+8.*
     & rsxy(i1+1,i2,i3,0,1)-rsxy(i1+2,i2,i3,0,1))/(12.*dr(0))
            aj4rys = (rsxy(i1,i2-2,i3,0,1)-8.*rsxy(i1,i2-1,i3,0,1)+8.*
     & rsxy(i1,i2+1,i3,0,1)-rsxy(i1,i2+2,i3,0,1))/(12.*dr(1))
            aj4sy = rsxy(i1,i2,i3,1,1)
            aj4syr = (rsxy(i1-2,i2,i3,1,1)-8.*rsxy(i1-1,i2,i3,1,1)+8.*
     & rsxy(i1+1,i2,i3,1,1)-rsxy(i1+2,i2,i3,1,1))/(12.*dr(0))
            aj4sys = (rsxy(i1,i2-2,i3,1,1)-8.*rsxy(i1,i2-1,i3,1,1)+8.*
     & rsxy(i1,i2+1,i3,1,1)-rsxy(i1,i2+2,i3,1,1))/(12.*dr(1))
            aj4rxx = aj4rx*aj4rxr+aj4sx*aj4rxs
            aj4rxy = aj4ry*aj4rxr+aj4sy*aj4rxs
            aj4sxx = aj4rx*aj4sxr+aj4sx*aj4sxs
            aj4sxy = aj4ry*aj4sxr+aj4sy*aj4sxs
            aj4ryx = aj4rx*aj4ryr+aj4sx*aj4rys
            aj4ryy = aj4ry*aj4ryr+aj4sy*aj4rys
            aj4syx = aj4rx*aj4syr+aj4sx*aj4sys
            aj4syy = aj4ry*aj4syr+aj4sy*aj4sys
            aj2rx = rsxy(i1,i2,i3,0,0)
            aj2rxr = (-rsxy(i1-1,i2,i3,0,0)+rsxy(i1+1,i2,i3,0,0))/(2.*
     & dr(0))
            aj2rxs = (-rsxy(i1,i2-1,i3,0,0)+rsxy(i1,i2+1,i3,0,0))/(2.*
     & dr(1))
            aj2rxrr = (rsxy(i1-1,i2,i3,0,0)-2.*rsxy(i1,i2,i3,0,0)+rsxy(
     & i1+1,i2,i3,0,0))/(dr(0)**2)
            aj2rxrs = (-(-rsxy(i1-1,i2-1,i3,0,0)+rsxy(i1-1,i2+1,i3,0,0)
     & )/(2.*dr(1))+(-rsxy(i1+1,i2-1,i3,0,0)+rsxy(i1+1,i2+1,i3,0,0))/(
     & 2.*dr(1)))/(2.*dr(0))
            aj2rxss = (rsxy(i1,i2-1,i3,0,0)-2.*rsxy(i1,i2,i3,0,0)+rsxy(
     & i1,i2+1,i3,0,0))/(dr(1)**2)
            aj2rxrrr = (-rsxy(i1-2,i2,i3,0,0)+2.*rsxy(i1-1,i2,i3,0,0)-
     & 2.*rsxy(i1+1,i2,i3,0,0)+rsxy(i1+2,i2,i3,0,0))/(2.*dr(0)**3)
            aj2rxrrs = ((-rsxy(i1-1,i2-1,i3,0,0)+rsxy(i1-1,i2+1,i3,0,0)
     & )/(2.*dr(1))-2.*(-rsxy(i1,i2-1,i3,0,0)+rsxy(i1,i2+1,i3,0,0))/(
     & 2.*dr(1))+(-rsxy(i1+1,i2-1,i3,0,0)+rsxy(i1+1,i2+1,i3,0,0))/(2.*
     & dr(1)))/(dr(0)**2)
            aj2rxrss = (-(rsxy(i1-1,i2-1,i3,0,0)-2.*rsxy(i1-1,i2,i3,0,
     & 0)+rsxy(i1-1,i2+1,i3,0,0))/(dr(1)**2)+(rsxy(i1+1,i2-1,i3,0,0)-
     & 2.*rsxy(i1+1,i2,i3,0,0)+rsxy(i1+1,i2+1,i3,0,0))/(dr(1)**2))/(
     & 2.*dr(0))
            aj2rxsss = (-rsxy(i1,i2-2,i3,0,0)+2.*rsxy(i1,i2-1,i3,0,0)-
     & 2.*rsxy(i1,i2+1,i3,0,0)+rsxy(i1,i2+2,i3,0,0))/(2.*dr(1)**3)
            aj2sx = rsxy(i1,i2,i3,1,0)
            aj2sxr = (-rsxy(i1-1,i2,i3,1,0)+rsxy(i1+1,i2,i3,1,0))/(2.*
     & dr(0))
            aj2sxs = (-rsxy(i1,i2-1,i3,1,0)+rsxy(i1,i2+1,i3,1,0))/(2.*
     & dr(1))
            aj2sxrr = (rsxy(i1-1,i2,i3,1,0)-2.*rsxy(i1,i2,i3,1,0)+rsxy(
     & i1+1,i2,i3,1,0))/(dr(0)**2)
            aj2sxrs = (-(-rsxy(i1-1,i2-1,i3,1,0)+rsxy(i1-1,i2+1,i3,1,0)
     & )/(2.*dr(1))+(-rsxy(i1+1,i2-1,i3,1,0)+rsxy(i1+1,i2+1,i3,1,0))/(
     & 2.*dr(1)))/(2.*dr(0))
            aj2sxss = (rsxy(i1,i2-1,i3,1,0)-2.*rsxy(i1,i2,i3,1,0)+rsxy(
     & i1,i2+1,i3,1,0))/(dr(1)**2)
            aj2sxrrr = (-rsxy(i1-2,i2,i3,1,0)+2.*rsxy(i1-1,i2,i3,1,0)-
     & 2.*rsxy(i1+1,i2,i3,1,0)+rsxy(i1+2,i2,i3,1,0))/(2.*dr(0)**3)
            aj2sxrrs = ((-rsxy(i1-1,i2-1,i3,1,0)+rsxy(i1-1,i2+1,i3,1,0)
     & )/(2.*dr(1))-2.*(-rsxy(i1,i2-1,i3,1,0)+rsxy(i1,i2+1,i3,1,0))/(
     & 2.*dr(1))+(-rsxy(i1+1,i2-1,i3,1,0)+rsxy(i1+1,i2+1,i3,1,0))/(2.*
     & dr(1)))/(dr(0)**2)
            aj2sxrss = (-(rsxy(i1-1,i2-1,i3,1,0)-2.*rsxy(i1-1,i2,i3,1,
     & 0)+rsxy(i1-1,i2+1,i3,1,0))/(dr(1)**2)+(rsxy(i1+1,i2-1,i3,1,0)-
     & 2.*rsxy(i1+1,i2,i3,1,0)+rsxy(i1+1,i2+1,i3,1,0))/(dr(1)**2))/(
     & 2.*dr(0))
            aj2sxsss = (-rsxy(i1,i2-2,i3,1,0)+2.*rsxy(i1,i2-1,i3,1,0)-
     & 2.*rsxy(i1,i2+1,i3,1,0)+rsxy(i1,i2+2,i3,1,0))/(2.*dr(1)**3)
            aj2ry = rsxy(i1,i2,i3,0,1)
            aj2ryr = (-rsxy(i1-1,i2,i3,0,1)+rsxy(i1+1,i2,i3,0,1))/(2.*
     & dr(0))
            aj2rys = (-rsxy(i1,i2-1,i3,0,1)+rsxy(i1,i2+1,i3,0,1))/(2.*
     & dr(1))
            aj2ryrr = (rsxy(i1-1,i2,i3,0,1)-2.*rsxy(i1,i2,i3,0,1)+rsxy(
     & i1+1,i2,i3,0,1))/(dr(0)**2)
            aj2ryrs = (-(-rsxy(i1-1,i2-1,i3,0,1)+rsxy(i1-1,i2+1,i3,0,1)
     & )/(2.*dr(1))+(-rsxy(i1+1,i2-1,i3,0,1)+rsxy(i1+1,i2+1,i3,0,1))/(
     & 2.*dr(1)))/(2.*dr(0))
            aj2ryss = (rsxy(i1,i2-1,i3,0,1)-2.*rsxy(i1,i2,i3,0,1)+rsxy(
     & i1,i2+1,i3,0,1))/(dr(1)**2)
            aj2ryrrr = (-rsxy(i1-2,i2,i3,0,1)+2.*rsxy(i1-1,i2,i3,0,1)-
     & 2.*rsxy(i1+1,i2,i3,0,1)+rsxy(i1+2,i2,i3,0,1))/(2.*dr(0)**3)
            aj2ryrrs = ((-rsxy(i1-1,i2-1,i3,0,1)+rsxy(i1-1,i2+1,i3,0,1)
     & )/(2.*dr(1))-2.*(-rsxy(i1,i2-1,i3,0,1)+rsxy(i1,i2+1,i3,0,1))/(
     & 2.*dr(1))+(-rsxy(i1+1,i2-1,i3,0,1)+rsxy(i1+1,i2+1,i3,0,1))/(2.*
     & dr(1)))/(dr(0)**2)
            aj2ryrss = (-(rsxy(i1-1,i2-1,i3,0,1)-2.*rsxy(i1-1,i2,i3,0,
     & 1)+rsxy(i1-1,i2+1,i3,0,1))/(dr(1)**2)+(rsxy(i1+1,i2-1,i3,0,1)-
     & 2.*rsxy(i1+1,i2,i3,0,1)+rsxy(i1+1,i2+1,i3,0,1))/(dr(1)**2))/(
     & 2.*dr(0))
            aj2rysss = (-rsxy(i1,i2-2,i3,0,1)+2.*rsxy(i1,i2-1,i3,0,1)-
     & 2.*rsxy(i1,i2+1,i3,0,1)+rsxy(i1,i2+2,i3,0,1))/(2.*dr(1)**3)
            aj2sy = rsxy(i1,i2,i3,1,1)
            aj2syr = (-rsxy(i1-1,i2,i3,1,1)+rsxy(i1+1,i2,i3,1,1))/(2.*
     & dr(0))
            aj2sys = (-rsxy(i1,i2-1,i3,1,1)+rsxy(i1,i2+1,i3,1,1))/(2.*
     & dr(1))
            aj2syrr = (rsxy(i1-1,i2,i3,1,1)-2.*rsxy(i1,i2,i3,1,1)+rsxy(
     & i1+1,i2,i3,1,1))/(dr(0)**2)
            aj2syrs = (-(-rsxy(i1-1,i2-1,i3,1,1)+rsxy(i1-1,i2+1,i3,1,1)
     & )/(2.*dr(1))+(-rsxy(i1+1,i2-1,i3,1,1)+rsxy(i1+1,i2+1,i3,1,1))/(
     & 2.*dr(1)))/(2.*dr(0))
            aj2syss = (rsxy(i1,i2-1,i3,1,1)-2.*rsxy(i1,i2,i3,1,1)+rsxy(
     & i1,i2+1,i3,1,1))/(dr(1)**2)
            aj2syrrr = (-rsxy(i1-2,i2,i3,1,1)+2.*rsxy(i1-1,i2,i3,1,1)-
     & 2.*rsxy(i1+1,i2,i3,1,1)+rsxy(i1+2,i2,i3,1,1))/(2.*dr(0)**3)
            aj2syrrs = ((-rsxy(i1-1,i2-1,i3,1,1)+rsxy(i1-1,i2+1,i3,1,1)
     & )/(2.*dr(1))-2.*(-rsxy(i1,i2-1,i3,1,1)+rsxy(i1,i2+1,i3,1,1))/(
     & 2.*dr(1))+(-rsxy(i1+1,i2-1,i3,1,1)+rsxy(i1+1,i2+1,i3,1,1))/(2.*
     & dr(1)))/(dr(0)**2)
            aj2syrss = (-(rsxy(i1-1,i2-1,i3,1,1)-2.*rsxy(i1-1,i2,i3,1,
     & 1)+rsxy(i1-1,i2+1,i3,1,1))/(dr(1)**2)+(rsxy(i1+1,i2-1,i3,1,1)-
     & 2.*rsxy(i1+1,i2,i3,1,1)+rsxy(i1+1,i2+1,i3,1,1))/(dr(1)**2))/(
     & 2.*dr(0))
            aj2sysss = (-rsxy(i1,i2-2,i3,1,1)+2.*rsxy(i1,i2-1,i3,1,1)-
     & 2.*rsxy(i1,i2+1,i3,1,1)+rsxy(i1,i2+2,i3,1,1))/(2.*dr(1)**3)
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
            aj2rxxxy = aj2ry*t1*aj2rxrrr+(aj2sy*t1+2*aj2ry*aj2sx*aj2rx)
     & *aj2rxrrs+(aj2ry*t10+2*aj2sy*aj2sx*aj2rx)*aj2rxrss+aj2sy*t10*
     & aj2rxsss+(2*aj2rxy*aj2rx+aj2ry*aj2rxx)*aj2rxrr+(aj2ry*aj2sxx+2*
     & aj2sx*aj2rxy+2*aj2sxy*aj2rx+aj2sy*aj2rxx)*aj2rxrs+(aj2sy*
     & aj2sxx+2*aj2sxy*aj2sx)*aj2rxss+aj2rxxy*aj2rxr+aj2sxxy*aj2rxs
            t1 = aj2ry**2
            t4 = aj2sy*aj2ry
            t8 = aj2sy*aj2rx+aj2ry*aj2sx
            t16 = aj2sy**2
            aj2rxxyy = t1*aj2rx*aj2rxrrr+(t4*aj2rx+aj2ry*t8)*aj2rxrrs+(
     & t4*aj2sx+aj2sy*t8)*aj2rxrss+t16*aj2sx*aj2rxsss+(aj2ryy*aj2rx+2*
     & aj2ry*aj2rxy)*aj2rxrr+(2*aj2ry*aj2sxy+2*aj2sy*aj2rxy+aj2ryy*
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
            aj2sxxxy = aj2ry*t1*aj2sxrrr+(aj2sy*t1+2*aj2ry*aj2sx*aj2rx)
     & *aj2sxrrs+(aj2ry*t10+2*aj2sy*aj2sx*aj2rx)*aj2sxrss+aj2sy*t10*
     & aj2sxsss+(2*aj2rxy*aj2rx+aj2ry*aj2rxx)*aj2sxrr+(aj2ry*aj2sxx+2*
     & aj2sx*aj2rxy+2*aj2sxy*aj2rx+aj2sy*aj2rxx)*aj2sxrs+(aj2sy*
     & aj2sxx+2*aj2sxy*aj2sx)*aj2sxss+aj2rxxy*aj2sxr+aj2sxxy*aj2sxs
            t1 = aj2ry**2
            t4 = aj2sy*aj2ry
            t8 = aj2sy*aj2rx+aj2ry*aj2sx
            t16 = aj2sy**2
            aj2sxxyy = t1*aj2rx*aj2sxrrr+(t4*aj2rx+aj2ry*t8)*aj2sxrrs+(
     & t4*aj2sx+aj2sy*t8)*aj2sxrss+t16*aj2sx*aj2sxsss+(aj2ryy*aj2rx+2*
     & aj2ry*aj2rxy)*aj2sxrr+(2*aj2ry*aj2sxy+2*aj2sy*aj2rxy+aj2ryy*
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
            aj2ryxxy = aj2ry*t1*aj2ryrrr+(aj2sy*t1+2*aj2ry*aj2sx*aj2rx)
     & *aj2ryrrs+(aj2ry*t10+2*aj2sy*aj2sx*aj2rx)*aj2ryrss+aj2sy*t10*
     & aj2rysss+(2*aj2rxy*aj2rx+aj2ry*aj2rxx)*aj2ryrr+(aj2ry*aj2sxx+2*
     & aj2sx*aj2rxy+2*aj2sxy*aj2rx+aj2sy*aj2rxx)*aj2ryrs+(aj2sy*
     & aj2sxx+2*aj2sxy*aj2sx)*aj2ryss+aj2rxxy*aj2ryr+aj2sxxy*aj2rys
            t1 = aj2ry**2
            t4 = aj2sy*aj2ry
            t8 = aj2sy*aj2rx+aj2ry*aj2sx
            t16 = aj2sy**2
            aj2ryxyy = t1*aj2rx*aj2ryrrr+(t4*aj2rx+aj2ry*t8)*aj2ryrrs+(
     & t4*aj2sx+aj2sy*t8)*aj2ryrss+t16*aj2sx*aj2rysss+(aj2ryy*aj2rx+2*
     & aj2ry*aj2rxy)*aj2ryrr+(2*aj2ry*aj2sxy+2*aj2sy*aj2rxy+aj2ryy*
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
            aj2syxxy = aj2ry*t1*aj2syrrr+(aj2sy*t1+2*aj2ry*aj2sx*aj2rx)
     & *aj2syrrs+(aj2ry*t10+2*aj2sy*aj2sx*aj2rx)*aj2syrss+aj2sy*t10*
     & aj2sysss+(2*aj2rxy*aj2rx+aj2ry*aj2rxx)*aj2syrr+(aj2ry*aj2sxx+2*
     & aj2sx*aj2rxy+2*aj2sxy*aj2rx+aj2sy*aj2rxx)*aj2syrs+(aj2sy*
     & aj2sxx+2*aj2sxy*aj2sx)*aj2syss+aj2rxxy*aj2syr+aj2sxxy*aj2sys
            t1 = aj2ry**2
            t4 = aj2sy*aj2ry
            t8 = aj2sy*aj2rx+aj2ry*aj2sx
            t16 = aj2sy**2
            aj2syxyy = t1*aj2rx*aj2syrrr+(t4*aj2rx+aj2ry*t8)*aj2syrrs+(
     & t4*aj2sx+aj2sy*t8)*aj2syrss+t16*aj2sx*aj2sysss+(aj2ryy*aj2rx+2*
     & aj2ry*aj2rxy)*aj2syrr+(2*aj2ry*aj2sxy+2*aj2sy*aj2rxy+aj2ryy*
     & aj2sx+aj2syy*aj2rx)*aj2syrs+(aj2syy*aj2sx+2*aj2sy*aj2sxy)*
     & aj2syss+aj2rxyy*aj2syr+aj2sxyy*aj2sys
            t1 = aj2ry**2
            t7 = aj2sy**2
            aj2syyyy = aj2ry*t1*aj2syrrr+3*t1*aj2sy*aj2syrrs+3*aj2ry*
     & t7*aj2syrss+t7*aj2sy*aj2sysss+3*aj2ry*aj2ryy*aj2syrr+(3*aj2syy*
     & aj2ry+3*aj2sy*aj2ryy)*aj2syrs+3*aj2syy*aj2sy*aj2syss+aj2ryyy*
     & aj2syr+aj2syyy*aj2sys
            do n=na,nb
              ! evalParametricDerivativesComponents1(u,i1,i2,i3,m, v,DIM,ORDER,MAXDERIV)
              uu = u1(i1,i2,i3,n)
              uur = (u1(i1-2,i2,i3,n)-8.*u1(i1-1,i2,i3,n)+8.*u1(i1+1,
     & i2,i3,n)-u1(i1+2,i2,i3,n))/(12.*dr(0))
              uus = (u1(i1,i2-2,i3,n)-8.*u1(i1,i2-1,i3,n)+8.*u1(i1,i2+
     & 1,i3,n)-u1(i1,i2+2,i3,n))/(12.*dr(1))
              uurr = (-u1(i1-2,i2,i3,n)+16.*u1(i1-1,i2,i3,n)-30.*u1(i1,
     & i2,i3,n)+16.*u1(i1+1,i2,i3,n)-u1(i1+2,i2,i3,n))/(12.*dr(0)**2)
              uurs = ((u1(i1-2,i2-2,i3,n)-8.*u1(i1-2,i2-1,i3,n)+8.*u1(
     & i1-2,i2+1,i3,n)-u1(i1-2,i2+2,i3,n))/(12.*dr(1))-8.*(u1(i1-1,i2-
     & 2,i3,n)-8.*u1(i1-1,i2-1,i3,n)+8.*u1(i1-1,i2+1,i3,n)-u1(i1-1,i2+
     & 2,i3,n))/(12.*dr(1))+8.*(u1(i1+1,i2-2,i3,n)-8.*u1(i1+1,i2-1,i3,
     & n)+8.*u1(i1+1,i2+1,i3,n)-u1(i1+1,i2+2,i3,n))/(12.*dr(1))-(u1(
     & i1+2,i2-2,i3,n)-8.*u1(i1+2,i2-1,i3,n)+8.*u1(i1+2,i2+1,i3,n)-u1(
     & i1+2,i2+2,i3,n))/(12.*dr(1)))/(12.*dr(0))
              uuss = (-u1(i1,i2-2,i3,n)+16.*u1(i1,i2-1,i3,n)-30.*u1(i1,
     & i2,i3,n)+16.*u1(i1,i2+1,i3,n)-u1(i1,i2+2,i3,n))/(12.*dr(1)**2)

              ux = aj4rx*uur+aj4sx*uus
              uy = aj4ry*uur+aj4sy*uus

              t1 = aj4rx**2
              t6 = aj4sx**2
              uxx = t1*uurr+2*aj4rx*aj4sx*uurs+t6*uuss+aj4rxx*uur+
     & aj4sxx*uus
              uxy = aj4ry*aj4rx*uurr+(aj4sy*aj4rx+aj4ry*aj4sx)*uurs+
     & aj4sy*aj4sx*uuss+aj4rxy*uur+aj4sxy*uus
              t1 = aj4ry**2
              t6 = aj4sy**2
              uyy = t1*uurr+2*aj4ry*aj4sy*uurs+t6*uuss+aj4ryy*uur+
     & aj4syy*uus

              ! Now evaluate the 3rd derivatives to 2nd order
              uu = u1(i1,i2,i3,n)
              uur = (-u1(i1-1,i2,i3,n)+u1(i1+1,i2,i3,n))/(2.*dr(0))
              uus = (-u1(i1,i2-1,i3,n)+u1(i1,i2+1,i3,n))/(2.*dr(1))
              uurr = (u1(i1-1,i2,i3,n)-2.*u1(i1,i2,i3,n)+u1(i1+1,i2,i3,
     & n))/(dr(0)**2)
              uurs = (-(-u1(i1-1,i2-1,i3,n)+u1(i1-1,i2+1,i3,n))/(2.*dr(
     & 1))+(-u1(i1+1,i2-1,i3,n)+u1(i1+1,i2+1,i3,n))/(2.*dr(1)))/(2.*
     & dr(0))
              uuss = (u1(i1,i2-1,i3,n)-2.*u1(i1,i2,i3,n)+u1(i1,i2+1,i3,
     & n))/(dr(1)**2)
              uurrr = (-u1(i1-2,i2,i3,n)+2.*u1(i1-1,i2,i3,n)-2.*u1(i1+
     & 1,i2,i3,n)+u1(i1+2,i2,i3,n))/(2.*dr(0)**3)
              uurrs = ((-u1(i1-1,i2-1,i3,n)+u1(i1-1,i2+1,i3,n))/(2.*dr(
     & 1))-2.*(-u1(i1,i2-1,i3,n)+u1(i1,i2+1,i3,n))/(2.*dr(1))+(-u1(i1+
     & 1,i2-1,i3,n)+u1(i1+1,i2+1,i3,n))/(2.*dr(1)))/(dr(0)**2)
              uurss = (-(u1(i1-1,i2-1,i3,n)-2.*u1(i1-1,i2,i3,n)+u1(i1-
     & 1,i2+1,i3,n))/(dr(1)**2)+(u1(i1+1,i2-1,i3,n)-2.*u1(i1+1,i2,i3,
     & n)+u1(i1+1,i2+1,i3,n))/(dr(1)**2))/(2.*dr(0))
              uusss = (-u1(i1,i2-2,i3,n)+2.*u1(i1,i2-1,i3,n)-2.*u1(i1,
     & i2+1,i3,n)+u1(i1,i2+2,i3,n))/(2.*dr(1)**3)

              t1 = aj2rx**2
              t7 = aj2sx**2
              uxxx = t1*aj2rx*uurrr+3*t1*aj2sx*uurrs+3*aj2rx*t7*uurss+
     & t7*aj2sx*uusss+3*aj2rx*aj2rxx*uurr+(3*aj2sxx*aj2rx+3*aj2sx*
     & aj2rxx)*uurs+3*aj2sxx*aj2sx*uuss+aj2rxxx*uur+aj2sxxx*uus
              t1 = aj2rx**2
              t10 = aj2sx**2
              uxxy = aj2ry*t1*uurrr+(aj2sy*t1+2*aj2ry*aj2sx*aj2rx)*
     & uurrs+(aj2ry*t10+2*aj2sy*aj2sx*aj2rx)*uurss+aj2sy*t10*uusss+(2*
     & aj2rxy*aj2rx+aj2ry*aj2rxx)*uurr+(aj2ry*aj2sxx+2*aj2sx*aj2rxy+2*
     & aj2sxy*aj2rx+aj2sy*aj2rxx)*uurs+(aj2sy*aj2sxx+2*aj2sxy*aj2sx)*
     & uuss+aj2rxxy*uur+aj2sxxy*uus
              t1 = aj2ry**2
              t4 = aj2sy*aj2ry
              t8 = aj2sy*aj2rx+aj2ry*aj2sx
              t16 = aj2sy**2
              uxyy = t1*aj2rx*uurrr+(t4*aj2rx+aj2ry*t8)*uurrs+(t4*
     & aj2sx+aj2sy*t8)*uurss+t16*aj2sx*uusss+(aj2ryy*aj2rx+2*aj2ry*
     & aj2rxy)*uurr+(2*aj2ry*aj2sxy+2*aj2sy*aj2rxy+aj2ryy*aj2sx+
     & aj2syy*aj2rx)*uurs+(aj2syy*aj2sx+2*aj2sy*aj2sxy)*uuss+aj2rxyy*
     & uur+aj2sxyy*uus
              t1 = aj2ry**2
              t7 = aj2sy**2
              uyyy = aj2ry*t1*uurrr+3*t1*aj2sy*uurrs+3*aj2ry*t7*uurss+
     & t7*aj2sy*uusss+3*aj2ry*aj2ryy*uurr+(3*aj2syy*aj2ry+3*aj2sy*
     & aj2ryy)*uurs+3*aj2syy*aj2sy*uuss+aj2ryyy*uur+aj2syyy*uus

              uu = u1(i1,i2,i3,n)
              uur = (-u1(i1-1,i2,i3,n)+u1(i1+1,i2,i3,n))/(2.*dr(0))
              uus = (-u1(i1,i2-1,i3,n)+u1(i1,i2+1,i3,n))/(2.*dr(1))

              uSave(ii,m,1,n)=ux
              uSave(ii,m,2,n)=uy
              uSave(ii,m,3,n)=uxx
              uSave(ii,m,4,n)=uxy
              uSave(ii,m,5,n)=uyy
              uSave(ii,m,6,n)=uxxx
              uSave(ii,m,7,n)=uxxy
              uSave(ii,m,8,n)=uxyy
              uSave(ii,m,9,n)=uyyy


            end do
            ii=ii+1
          end do
          end do


      else
       write(*,'("ERROR: unexpected order of accuracy")')
       stop 1132
      end if


      return
      end
