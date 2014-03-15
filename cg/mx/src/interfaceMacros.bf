c *******************************************************************************
c   This file defines the include files
c
c  evaluateJacobianDerivativesOrder6.h
c  evaluateCoefficientsOrder6.h
c  declareTemporaryVariablesOrder6.h
c   
c
c *******************************************************************************

c These next include files will define the macros that will define the difference approximations
#Include "derivMacroDefinitions.h"

#Include "defineParametricDerivMacros.h"

! defineParametricDerivativeMacros(u,dr,dx,DIM,ORDER,COMPONENTS,MAXDERIV)
! 2D, order=6, components=1

defineParametricDerivativeMacros(u1,dr1,dx1,2,2,1,6)
defineParametricDerivativeMacros(u1,dr1,dx1,2,4,1,4)
defineParametricDerivativeMacros(u1,dr1,dx1,2,6,1,2)

defineParametricDerivativeMacros(u2,dr2,dx2,2,2,1,6)
defineParametricDerivativeMacros(u2,dr2,dx2,2,4,1,4)
defineParametricDerivativeMacros(u2,dr2,dx2,2,6,1,2)

! 2D, order=6, components=2
defineParametricDerivativeMacros(rsxy1,dr1,dx1,2,6,2,1)
defineParametricDerivativeMacros(rsxy1,dr1,dx1,2,4,2,3)
defineParametricDerivativeMacros(rsxy1,dr1,dx1,2,2,2,5)

defineParametricDerivativeMacros(rsxy2,dr2,dx2,2,6,2,1)
defineParametricDerivativeMacros(rsxy2,dr2,dx2,2,4,2,3)
defineParametricDerivativeMacros(rsxy2,dr2,dx2,2,2,2,5)


c *** Here are macros that define the coefficients of the ghost points
#Include "derivStencilCoeff.h"


c MAT: 1 or 2 for material 1 or 2
c DIR: 0 or 1 for r or s direction
#beginMacro evalCoeffOrder6(MAT,DIR,dr,ds)

#If DIR == 0 
 c ## MAT ##x6(1) = xCoeff2dOrder6Ghost10(a ## MAT ##j6,dr,ds)
 c ## MAT ##x6(2) = xCoeff2dOrder6Ghost20(a ## MAT ##j6,dr,ds)
 c ## MAT ##x6(3) = xCoeff2dOrder6Ghost30(a ## MAT ##j6,dr,ds)

 c ## MAT ##y6(1) = yCoeff2dOrder6Ghost10(a ## MAT ##j6,dr,ds)
 c ## MAT ##y6(2) = yCoeff2dOrder6Ghost20(a ## MAT ##j6,dr,ds)
 c ## MAT ##y6(3) = yCoeff2dOrder6Ghost30(a ## MAT ##j6,dr,ds)

 c ## MAT ##Lap6(1) = lapCoeff2dOrder6Ghost10(a ## MAT ##j6,dr,ds)
 c ## MAT ##Lap6(2) = lapCoeff2dOrder6Ghost20(a ## MAT ##j6,dr,ds)
 c ## MAT ##Lap6(3) = lapCoeff2dOrder6Ghost30(a ## MAT ##j6,dr,ds)

 ! 4th order values:

 c ## MAT ##xLap4(1) = xLapCoeff2dOrder4Ghost10(a ## MAT ##j4,dr,ds)
 c ## MAT ##xLap4(2) = xLapCoeff2dOrder4Ghost20(a ## MAT ##j4,dr,ds)
 c ## MAT ##xLap4(3) = xLapCoeff2dOrder4Ghost30(a ## MAT ##j4,dr,ds)

 c ## MAT ##yLap4(1) = yLapCoeff2dOrder4Ghost10(a ## MAT ##j4,dr,ds)
 c ## MAT ##yLap4(2) = yLapCoeff2dOrder4Ghost20(a ## MAT ##j4,dr,ds)
 c ## MAT ##yLap4(3) = yLapCoeff2dOrder4Ghost30(a ## MAT ##j4,dr,ds)

 c ## MAT ##LapSq4(1) = lapSqCoeff2dOrder4Ghost10(a ## MAT ##j4,dr,ds)
 c ## MAT ##LapSq4(2) = lapSqCoeff2dOrder4Ghost20(a ## MAT ##j4,dr,ds)
 c ## MAT ##LapSq4(3) = lapSqCoeff2dOrder4Ghost30(a ## MAT ##j4,dr,ds)

 ! for fixup: 
 c ## MAT ##xxx4(1) = xxxCoeff2dOrder4Ghost10(a ## MAT ##j4,dr,ds)
 c ## MAT ##xxx4(2) = xxxCoeff2dOrder4Ghost20(a ## MAT ##j4,dr,ds)
 c ## MAT ##xxx4(3) = xxxCoeff2dOrder4Ghost30(a ## MAT ##j4,dr,ds)

 c ## MAT ##xxy4(1) = xxyCoeff2dOrder4Ghost10(a ## MAT ##j4,dr,ds)
 c ## MAT ##xxy4(2) = xxyCoeff2dOrder4Ghost20(a ## MAT ##j4,dr,ds)
 c ## MAT ##xxy4(3) = xxyCoeff2dOrder4Ghost30(a ## MAT ##j4,dr,ds)

 c ## MAT ##xyy4(1) = xyyCoeff2dOrder4Ghost10(a ## MAT ##j4,dr,ds)
 c ## MAT ##xyy4(2) = xyyCoeff2dOrder4Ghost20(a ## MAT ##j4,dr,ds)
 c ## MAT ##xyy4(3) = xyyCoeff2dOrder4Ghost30(a ## MAT ##j4,dr,ds)

 c ## MAT ##yyy4(1) = yyyCoeff2dOrder4Ghost10(a ## MAT ##j4,dr,ds)
 c ## MAT ##yyy4(2) = yyyCoeff2dOrder4Ghost20(a ## MAT ##j4,dr,ds)
 c ## MAT ##yyy4(3) = yyyCoeff2dOrder4Ghost30(a ## MAT ##j4,dr,ds)

 ! 2nd order values:

 c ## MAT ##xLapSq2(1) = xLapSqCoeff2dOrder2Ghost10(a ## MAT ##j2,dr,ds)
 c ## MAT ##xLapSq2(2) = xLapSqCoeff2dOrder2Ghost20(a ## MAT ##j2,dr,ds)
 c ## MAT ##xLapSq2(3) = xLapSqCoeff2dOrder2Ghost30(a ## MAT ##j2,dr,ds)

 c ## MAT ##yLapSq2(1) = yLapSqCoeff2dOrder2Ghost10(a ## MAT ##j2,dr,ds)
 c ## MAT ##yLapSq2(2) = yLapSqCoeff2dOrder2Ghost20(a ## MAT ##j2,dr,ds)
 c ## MAT ##yLapSq2(3) = yLapSqCoeff2dOrder2Ghost30(a ## MAT ##j2,dr,ds)

 c ## MAT ##LapCubed2(1) = lapCubedCoeff2dOrder2Ghost10(a ## MAT ##j2,dr,ds)
 c ## MAT ##LapCubed2(2) = lapCubedCoeff2dOrder2Ghost20(a ## MAT ##j2,dr,ds)
 c ## MAT ##LapCubed2(3) = lapCubedCoeff2dOrder2Ghost30(a ## MAT ##j2,dr,ds)

 !  for fixup:
 c ## MAT ##xxxxy2(1) = xxxxyCoeff2dOrder2Ghost10(a ## MAT ##j2,dr,ds)
 c ## MAT ##xxxxy2(2) = xxxxyCoeff2dOrder2Ghost20(a ## MAT ##j2,dr,ds)
 c ## MAT ##xxxxy2(3) = xxxxyCoeff2dOrder2Ghost30(a ## MAT ##j2,dr,ds)
                                                                                                
 c ## MAT ##xyyyy2(1) = xyyyyCoeff2dOrder2Ghost10(a ## MAT ##j2,dr,ds)
 c ## MAT ##xyyyy2(2) = xyyyyCoeff2dOrder2Ghost20(a ## MAT ##j2,dr,ds)
 c ## MAT ##xyyyy2(3) = xyyyyCoeff2dOrder2Ghost30(a ## MAT ##j2,dr,ds)

 c ## MAT ##xxyyy2(1) = xxyyyCoeff2dOrder2Ghost10(a ## MAT ##j2,dr,ds)
 c ## MAT ##xxyyy2(2) = xxyyyCoeff2dOrder2Ghost20(a ## MAT ##j2,dr,ds)
 c ## MAT ##xxyyy2(3) = xxyyyCoeff2dOrder2Ghost30(a ## MAT ##j2,dr,ds)
                                                                                                
 c ## MAT ##xxxyy2(1) = xxxyyCoeff2dOrder2Ghost10(a ## MAT ##j2,dr,ds)
 c ## MAT ##xxxyy2(2) = xxxyyCoeff2dOrder2Ghost20(a ## MAT ##j2,dr,ds)
 c ## MAT ##xxxyy2(3) = xxxyyCoeff2dOrder2Ghost30(a ## MAT ##j2,dr,ds)

 c ## MAT ##yyyyy2(1) = yyyyyCoeff2dOrder2Ghost10(a ## MAT ##j2,dr,ds)
 c ## MAT ##yyyyy2(2) = yyyyyCoeff2dOrder2Ghost20(a ## MAT ##j2,dr,ds)
 c ## MAT ##yyyyy2(3) = yyyyyCoeff2dOrder2Ghost30(a ## MAT ##j2,dr,ds)


#Elif DIR == 1

 c ## MAT ##x6(1) = xCoeff2dOrder6Ghost01(a ## MAT ##j6,dr,ds)
 c ## MAT ##x6(2) = xCoeff2dOrder6Ghost02(a ## MAT ##j6,dr,ds)
 c ## MAT ##x6(3) = xCoeff2dOrder6Ghost03(a ## MAT ##j6,dr,ds)

 c ## MAT ##y6(1) = yCoeff2dOrder6Ghost01(a ## MAT ##j6,dr,ds)
 c ## MAT ##y6(2) = yCoeff2dOrder6Ghost02(a ## MAT ##j6,dr,ds)
 c ## MAT ##y6(3) = yCoeff2dOrder6Ghost03(a ## MAT ##j6,dr,ds)

 c ## MAT ##Lap6(1) = lapCoeff2dOrder6Ghost01(a ## MAT ##j6,dr,ds)
 c ## MAT ##Lap6(2) = lapCoeff2dOrder6Ghost02(a ## MAT ##j6,dr,ds)
 c ## MAT ##Lap6(3) = lapCoeff2dOrder6Ghost03(a ## MAT ##j6,dr,ds)

 ! 4th order values:

 c ## MAT ##xLap4(1) = xLapCoeff2dOrder4Ghost01(a ## MAT ##j4,dr,ds)
 c ## MAT ##xLap4(2) = xLapCoeff2dOrder4Ghost02(a ## MAT ##j4,dr,ds)
 c ## MAT ##xLap4(3) = xLapCoeff2dOrder4Ghost03(a ## MAT ##j4,dr,ds)

 c ## MAT ##yLap4(1) = yLapCoeff2dOrder4Ghost01(a ## MAT ##j4,dr,ds)
 c ## MAT ##yLap4(2) = yLapCoeff2dOrder4Ghost02(a ## MAT ##j4,dr,ds)
 c ## MAT ##yLap4(3) = yLapCoeff2dOrder4Ghost03(a ## MAT ##j4,dr,ds)

 c ## MAT ##LapSq4(1) = lapSqCoeff2dOrder4Ghost01(a ## MAT ##j4,dr,ds)
 c ## MAT ##LapSq4(2) = lapSqCoeff2dOrder4Ghost02(a ## MAT ##j4,dr,ds)
 c ## MAT ##LapSq4(3) = lapSqCoeff2dOrder4Ghost03(a ## MAT ##j4,dr,ds)

 ! for fixup: 
 c ## MAT ##xxx4(1) = xxxCoeff2dOrder4Ghost01(a ## MAT ##j4,dr,ds)
 c ## MAT ##xxx4(2) = xxxCoeff2dOrder4Ghost02(a ## MAT ##j4,dr,ds)
 c ## MAT ##xxx4(3) = xxxCoeff2dOrder4Ghost03(a ## MAT ##j4,dr,ds)

 c ## MAT ##xxy4(1) = xxyCoeff2dOrder4Ghost01(a ## MAT ##j4,dr,ds)
 c ## MAT ##xxy4(2) = xxyCoeff2dOrder4Ghost02(a ## MAT ##j4,dr,ds)
 c ## MAT ##xxy4(3) = xxyCoeff2dOrder4Ghost03(a ## MAT ##j4,dr,ds)
                                                                                        
 c ## MAT ##xyy4(1) = xyyCoeff2dOrder4Ghost01(a ## MAT ##j4,dr,ds)
 c ## MAT ##xyy4(2) = xyyCoeff2dOrder4Ghost02(a ## MAT ##j4,dr,ds)
 c ## MAT ##xyy4(3) = xyyCoeff2dOrder4Ghost03(a ## MAT ##j4,dr,ds)
                                                                                        
 c ## MAT ##yyy4(1) = yyyCoeff2dOrder4Ghost01(a ## MAT ##j4,dr,ds)
 c ## MAT ##yyy4(2) = yyyCoeff2dOrder4Ghost02(a ## MAT ##j4,dr,ds)
 c ## MAT ##yyy4(3) = yyyCoeff2dOrder4Ghost03(a ## MAT ##j4,dr,ds)

 ! 2nd order values:

 c ## MAT ##xLapSq2(1) = xLapSqCoeff2dOrder2Ghost01(a ## MAT ##j2,dr,ds)
 c ## MAT ##xLapSq2(2) = xLapSqCoeff2dOrder2Ghost02(a ## MAT ##j2,dr,ds)
 c ## MAT ##xLapSq2(3) = xLapSqCoeff2dOrder2Ghost03(a ## MAT ##j2,dr,ds)

 c ## MAT ##yLapSq2(1) = yLapSqCoeff2dOrder2Ghost01(a ## MAT ##j2,dr,ds)
 c ## MAT ##yLapSq2(2) = yLapSqCoeff2dOrder2Ghost02(a ## MAT ##j2,dr,ds)
 c ## MAT ##yLapSq2(3) = yLapSqCoeff2dOrder2Ghost03(a ## MAT ##j2,dr,ds)

 c ## MAT ##LapCubed2(1) = lapCubedCoeff2dOrder2Ghost01(a ## MAT ##j2,dr,ds)
 c ## MAT ##LapCubed2(2) = lapCubedCoeff2dOrder2Ghost02(a ## MAT ##j2,dr,ds)
 c ## MAT ##LapCubed2(3) = lapCubedCoeff2dOrder2Ghost03(a ## MAT ##j2,dr,ds)

 !  for fixup:
 c ## MAT ##xxxxy2(1) = xxxxyCoeff2dOrder2Ghost01(a ## MAT ##j2,dr,ds)
 c ## MAT ##xxxxy2(2) = xxxxyCoeff2dOrder2Ghost02(a ## MAT ##j2,dr,ds)
 c ## MAT ##xxxxy2(3) = xxxxyCoeff2dOrder2Ghost03(a ## MAT ##j2,dr,ds)
                                                                                                
 c ## MAT ##xyyyy2(1) = xyyyyCoeff2dOrder2Ghost01(a ## MAT ##j2,dr,ds)
 c ## MAT ##xyyyy2(2) = xyyyyCoeff2dOrder2Ghost02(a ## MAT ##j2,dr,ds)
 c ## MAT ##xyyyy2(3) = xyyyyCoeff2dOrder2Ghost03(a ## MAT ##j2,dr,ds)
                                                                                                
 c ## MAT ##xxyyy2(1) = xxyyyCoeff2dOrder2Ghost01(a ## MAT ##j2,dr,ds)
 c ## MAT ##xxyyy2(2) = xxyyyCoeff2dOrder2Ghost02(a ## MAT ##j2,dr,ds)
 c ## MAT ##xxyyy2(3) = xxyyyCoeff2dOrder2Ghost03(a ## MAT ##j2,dr,ds)
                                                                                                
 c ## MAT ##xxxyy2(1) = xxxyyCoeff2dOrder2Ghost01(a ## MAT ##j2,dr,ds)
 c ## MAT ##xxxyy2(2) = xxxyyCoeff2dOrder2Ghost02(a ## MAT ##j2,dr,ds)
 c ## MAT ##xxxyy2(3) = xxxyyCoeff2dOrder2Ghost03(a ## MAT ##j2,dr,ds)
                                                                                                
 c ## MAT ##yyyyy2(1) = yyyyyCoeff2dOrder2Ghost01(a ## MAT ##j2,dr,ds)
 c ## MAT ##yyyyy2(2) = yyyyyCoeff2dOrder2Ghost02(a ## MAT ##j2,dr,ds)
 c ## MAT ##yyyyy2(3) = yyyyyCoeff2dOrder2Ghost03(a ## MAT ##j2,dr,ds)

#Else
  stop 8843
#End

#endMacro


c Evaluate the BC equations and fill in f(i) 
#beginMacro evaluateEquationsOrder6()

 ! ******************* 6th order ******************
 evalParametricDerivativesComponents1(u1,i1,i2,i3,ex, uu1,2,6,2)
 evalParametricDerivativesComponents1(u1,i1,i2,i3,ey, vv1,2,6,2)

 evalParametricDerivativesComponents1(u2,j1,j2,j3,ex, uu2,2,6,2)
 evalParametricDerivativesComponents1(u2,j1,j2,j3,ey, vv2,2,6,2)

 ! 1st derivatives, 6th order
 getDuDx2(uu1,a1j6,uu1x6)
 getDuDy2(uu1,a1j6,uu1y6)
 getDuDx2(vv1,a1j6,vv1x6)
 getDuDy2(vv1,a1j6,vv1y6)

 getDuDx2(uu2,a2j6,uu2x6)
 getDuDy2(uu2,a2j6,uu2y6)
 getDuDx2(vv2,a2j6,vv2x6)
 getDuDy2(vv2,a2j6,vv2y6)

 ! 2nd derivatives, 6th order
 getDuDxx2(uu1,a1j6,uu1xx6)
 getDuDyy2(uu1,a1j6,uu1yy6)
 getDuDxx2(vv1,a1j6,vv1xx6)
 getDuDyy2(vv1,a1j6,vv1yy6)

 getDuDxx2(uu2,a2j6,uu2xx6)
 getDuDyy2(uu2,a2j6,uu2yy6)
 getDuDxx2(vv2,a2j6,vv2xx6)
 getDuDyy2(vv2,a2j6,vv2yy6)

 ulap1=uu1xx6+uu1yy6
 vlap1=vv1xx6+vv1yy6

 ulap2=uu2xx6+uu2yy6
 vlap2=vv2xx6+vv2yy6

 ! ****** fourth order ******

 evalParametricDerivativesComponents1(u1,i1,i2,i3,ex, uu1,2,4,4)
 evalParametricDerivativesComponents1(u1,i1,i2,i3,ey, vv1,2,4,4)

 evalParametricDerivativesComponents1(u2,j1,j2,j3,ex, uu2,2,4,4)
 evalParametricDerivativesComponents1(u2,j1,j2,j3,ey, vv2,2,4,4)

 ! 3rd derivatives, 4th order
 getDuDxxx2(uu1,a1j4,uu1xxx4)
 getDuDxxy2(uu1,a1j4,uu1xxy4)
 getDuDxyy2(uu1,a1j4,uu1xyy4)
 getDuDyyy2(uu1,a1j4,uu1yyy4)

 getDuDxxx2(vv1,a1j4,vv1xxx4)
 getDuDxxy2(vv1,a1j4,vv1xxy4)
 getDuDxyy2(vv1,a1j4,vv1xyy4)
 getDuDyyy2(vv1,a1j4,vv1yyy4)

 getDuDxxx2(uu2,a2j4,uu2xxx4)
 getDuDxxy2(uu2,a2j4,uu2xxy4)
 getDuDxyy2(uu2,a2j4,uu2xyy4)
 getDuDyyy2(uu2,a2j4,uu2yyy4)

 getDuDxxx2(vv2,a2j4,vv2xxx4)
 getDuDxxy2(vv2,a2j4,vv2xxy4)
 getDuDxyy2(vv2,a2j4,vv2xyy4)
 getDuDyyy2(vv2,a2j4,vv2yyy4)

 ! 4th derivatives, 4th order
 getDuDxxxx2(uu1,a1j4,uu1xxxx4)
 getDuDxxyy2(uu1,a1j4,uu1xxyy4)
 getDuDyyyy2(uu1,a1j4,uu1yyyy4)

 getDuDxxxx2(vv1,a1j4,vv1xxxx4)
 getDuDxxyy2(vv1,a1j4,vv1xxyy4)
 getDuDyyyy2(vv1,a1j4,vv1yyyy4)

 getDuDxxxx2(uu2,a2j4,uu2xxxx4)
 getDuDxxyy2(uu2,a2j4,uu2xxyy4)
 getDuDyyyy2(uu2,a2j4,uu2yyyy4)

 getDuDxxxx2(vv2,a2j4,vv2xxxx4)
 getDuDxxyy2(vv2,a2j4,vv2xxyy4)
 getDuDyyyy2(vv2,a2j4,vv2yyyy4)



 ulapSq1=uu1xxxx4+2.*uu1xxyy4+uu1yyyy4
 vlapSq1=vv1xxxx4+2.*vv1xxyy4+vv1yyyy4

 ulapSq2=uu2xxxx4+2.*uu2xxyy4+uu2yyyy4
 vlapSq2=vv2xxxx4+2.*vv2xxyy4+vv2yyyy4


 ! ****** 2nd order ******

 evalParametricDerivativesComponents1(u1,i1,i2,i3,ex, uu1,2,2,6)
 evalParametricDerivativesComponents1(u1,i1,i2,i3,ey, vv1,2,2,6)

 evalParametricDerivativesComponents1(u2,j1,j2,j3,ex, uu2,2,2,6)
 evalParametricDerivativesComponents1(u2,j1,j2,j3,ey, vv2,2,2,6)

 ! 5th derivatives, 2nd order
 getDuDxxxxx2(uu1,a1j2,uu1xxxxx2)
 getDuDxxxxy2(uu1,a1j2,uu1xxxxy2)
 getDuDxxxyy2(uu1,a1j2,uu1xxxyy2)
 getDuDxxyyy2(uu1,a1j2,uu1xxyyy2)
 getDuDxyyyy2(uu1,a1j2,uu1xyyyy2)
 getDuDyyyyy2(uu1,a1j2,uu1yyyyy2)

 getDuDxxxxx2(vv1,a1j2,vv1xxxxx2)
 getDuDxxxxy2(vv1,a1j2,vv1xxxxy2)
 getDuDxxxyy2(vv1,a1j2,vv1xxxyy2)
 getDuDxxyyy2(vv1,a1j2,vv1xxyyy2)
 getDuDxyyyy2(vv1,a1j2,vv1xyyyy2)
 getDuDyyyyy2(vv1,a1j2,vv1yyyyy2)

 getDuDxxxxx2(uu2,a2j2,uu2xxxxx2)
 getDuDxxxxy2(uu2,a2j2,uu2xxxxy2)
 getDuDxxxyy2(uu2,a2j2,uu2xxxyy2)
 getDuDxxyyy2(uu2,a2j2,uu2xxyyy2)
 getDuDxyyyy2(uu2,a2j2,uu2xyyyy2)
 getDuDyyyyy2(uu2,a2j2,uu2yyyyy2)

 getDuDxxxxx2(vv2,a2j2,vv2xxxxx2)
 getDuDxxxxy2(vv2,a2j2,vv2xxxxy2)
 getDuDxxxyy2(vv2,a2j2,vv2xxxyy2)
 getDuDxxyyy2(vv2,a2j2,vv2xxyyy2)
 getDuDxyyyy2(vv2,a2j2,vv2xyyyy2)
 getDuDyyyyy2(vv2,a2j2,vv2yyyyy2)

 ! 6th derivatives, 2nd order
 getDuDxxxxxx2(uu1,a1j2,uu1xxxxxx2)
 getDuDxxxxyy2(uu1,a1j2,uu1xxxxyy2)
 getDuDxxyyyy2(uu1,a1j2,uu1xxyyyy2)
 getDuDyyyyyy2(uu1,a1j2,uu1yyyyyy2)

 getDuDxxxxxx2(vv1,a1j2,vv1xxxxxx2)
 getDuDxxxxyy2(vv1,a1j2,vv1xxxxyy2)
 getDuDxxyyyy2(vv1,a1j2,vv1xxyyyy2)
 getDuDyyyyyy2(vv1,a1j2,vv1yyyyyy2)

 getDuDxxxxxx2(uu2,a2j2,uu2xxxxxx2)
 getDuDxxxxyy2(uu2,a2j2,uu2xxxxyy2)
 getDuDxxyyyy2(uu2,a2j2,uu2xxyyyy2)
 getDuDyyyyyy2(uu2,a2j2,uu2yyyyyy2)

 getDuDxxxxxx2(vv2,a2j2,vv2xxxxxx2)
 getDuDxxxxyy2(vv2,a2j2,vv2xxxxyy2)
 getDuDxxyyyy2(vv2,a2j2,vv2xxyyyy2)
 getDuDyyyyyy2(vv2,a2j2,vv2yyyyyy2)

 ulapCubed1=uu1xxxxxx2+3.*(uu1xxxxyy2+uu1xxyyyy2)+uu1yyyyyy2
 vlapCubed1=vv1xxxxxx2+3.*(vv1xxxxyy2+vv1xxyyyy2)+vv1yyyyyy2

 ulapCubed2=uu2xxxxxx2+3.*(uu2xxxxyy2+uu2xxyyyy2)+uu2yyyyyy2
 vlapCubed2=vv2xxxxxx2+3.*(vv2xxxxyy2+vv2xxyyyy2)+vv2yyyyyy2


 ! first evaluate the equations we want to solve with the wrong values at the ghost points:
 f(0)=(uu1x6+vv1y6)  - \
      (uu2x6+vv2y6)

 f(1)=(an1*ulap1+an2*vlap1) - \
      (an1*ulap2+an2*vlap2)

 f(2)=(vv1x6-uu1y6) - \
      (vv2x6-uu2y6)
 
 f(3)=(tau1*ulap1+tau2*vlap1)/eps1 - \
      (tau1*ulap2+tau2*vlap2)/eps2

 ! These next we can do to 4th order 
 !     also subtract off f(3)_tau = (tau.Lap(uv))_tau/eps to eliminate vxxy term
 f(4)=( (uu1xxx4         + vv1xxy4        ) - ( tau1a*(uu1xxy4+uu1yyy4)+tau2a*(vv1xxy4+vv1yyy4) ) )/eps1 - \
      ( (uu2xxx4         + vv2xxy4        ) - ( tau1a*(uu2xxy4+uu2yyy4)+tau2a*(vv2xxy4+vv2yyy4) ) )/eps2
 !f(4)=( (uu1xxx4+uu1xyy4 + vv1xxy4+vv1yyy4) - ( tau1a*(uu1xxy4+uu1yyy4)+tau2a*(vv1xxy4+vv1yyy4) ) )/eps1 - \
 !     ( (uu2xxx4+uu2xyy4 + vv2xxy4+vv2yyy4) - ( tau1a*(uu2xxy4+uu2yyy4)+tau2a*(vv2xxy4+vv2yyy4) ) )/eps2
 !f(4)=( (uu1xxx4+uu1xyy4 + vv1xxy4+vv1yyy4) )/eps1 - \
 !     ( (uu2xxx4+uu2xyy4 + vv2xxy4+vv2yyy4) )/eps2

 f(5)=(an1*ulapSq1 + an2*vlapSq1)/eps1 - \
      (an1*ulapSq2 + an2*vlapSq2)/eps2

 ! also subtract ...
 f(6)=( ((vv1xxx4+vv1xyy4)-(uu1xxy4+uu1yyy4)) +(uu1xxy4+vv1xyy4) )/eps1 - \
      ( ((vv2xxx4+vv2xyy4)-(uu2xxy4+uu2yyy4)) +(uu2xxy4+vv2xyy4) )/eps2

 ! f(6)=( ((vv1xxx4+vv1xyy4)-(uu1xxy4+uu1yyy4)) )/eps1 - \
 !      ( ((vv2xxx4+vv2xyy4)-(uu2xxy4+uu2yyy4)) )/eps2

 f(7)=(tau1*ulapSq1 + tau2*vlapSq1)/eps1**2 - \
      (tau1*ulapSq2 + tau2*vlapSq2)/eps2**2
 

 ! These last we do to 2nd order
 f(8)=((uu1xxxxx2+2.*uu1xxxyy2+uu1xyyyy2)+(vv1xxxxy2+2.*vv1xxyyy2+vv1yyyyy2) - \
       (tau1a*(uu1xxxxy2+2.*uu1xxyyy2+uu1yyyyy2)+tau2a*(vv1xxxxy2+2.*vv1xxyyy2+ vv1yyyyy2)) )/eps1**2 - \
      ((uu2xxxxx2+2.*uu2xxxyy2+uu2xyyyy2)+(vv2xxxxy2+2.*vv2xxyyy2+vv2yyyyy2)- \
       (tau1a*(uu2xxxxy2+2.*uu2xxyyy2+uu2yyyyy2)+tau2a*(vv2xxxxy2+2.*vv2xxyyy2+ vv2yyyyy2)) )/eps2**2

 ! f(8)=((uu1xxxxx2+2.*uu1xxxyy2+uu1xyyyy2)+(vv1xxxxy2+2.*vv1xxyyy2+vv1yyyyy2) \
 !                                       )/eps1**2 - \
 !      ((uu2xxxxx2+2.*uu2xxxyy2+uu2xyyyy2)+(vv2xxxxy2+2.*vv2xxyyy2+vv2yyyyy2) \
 !                                       )/eps2**2

 f(9) =(an1*ulapCubed1+an2*vlapCubed1)/eps1**2 - \
       (an1*ulapCubed2+an2*vlapCubed2)/eps2**2

 ! add on extra terms to cancel odd y-derivative terms
 f(10)=( ((vv1xxxxx2+2.*vv1xxxyy2+vv1xyyyy2)-(uu1xxxxy2+2.*uu1xxyyy2+uu1yyyyy2)) + \
               ((uu1xxxxy2+vv1xxxyy2) +2.*(uu1xxyyy2+vv1xyyyy2))               )/eps1**2 - \
       ( ((vv2xxxxx2+2.*vv2xxxyy2+vv2xyyyy2)-(uu2xxxxy2+2.*uu2xxyyy2+uu2yyyyy2)) + \
               ((uu2xxxxy2+vv2xxxyy2) +2.*(uu2xxyyy2+vv2xyyyy2))               )/eps2**2

 ! f(10)=( ((vv1xxxxx2+2.*vv1xxxyy2+vv1xyyyy2)-(uu1xxxxy2+2.*uu1xxyyy2+uu1yyyyy2))  \
 !                             )/eps1**2 - \
 !       ( ((vv2xxxxx2+2.*vv2xxxyy2+vv2xyyyy2)-(uu2xxxxy2+2.*uu2xxyyy2+uu2yyyyy2))  \
 !                            )/eps2**2

 f(11)=(tau1*ulapCubed1+tau2*vlapCubed1)/eps1**3 - \
       (tau1*ulapCubed2+tau2*vlapCubed2)/eps2**3

#endMacro


#beginFile evaluateJacobianDerivativesOrder6.h

      evalJacobianDerivatives(rsxy1,i1,i2,i3,a1j6,2,6,1)
      evalJacobianDerivatives(rsxy1,i1,i2,i3,a1j4,2,4,3)
      evalJacobianDerivatives(rsxy1,i1,i2,i3,a1j2,2,2,5)

      evalJacobianDerivatives(rsxy2,j1,j2,j3,a2j6,2,6,1)
      evalJacobianDerivatives(rsxy2,j1,j2,j3,a2j4,2,4,3)
      evalJacobianDerivatives(rsxy2,j1,j2,j3,a2j2,2,2,5)
#endFile

#beginFile evaluateEquationsOrder6.h
      evaluateEquationsOrder6()
#endFile

#beginFile evaluateCoefficientsOrder6.h
      ! Get coefficients for material 1
      dr1a=dr1(0)
      ds1a=dr1(1)
      if( axis1.eq.0 )then
        dr1a=dr1(0)*is
        ! macro: evalCoeffOrder6(MAT,DIR,dr,ds)
        evalCoeffOrder6(1,0,dr1a,ds1a)
      else
        ds1a=dr1(1)*is
        evalCoeffOrder6(1,1,dr1a,ds1a)
      end if
      
      ! Get coefficients for material 2
      dr2a=dr2(0)
      ds2a=dr2(1)
      if( axis2.eq.0 )then
        dr2a=dr2(0)*js
        ! macro: evalCoeffOrder6(MATERIAL,DIR,dr,ds)
        evalCoeffOrder6(2,0,dr2a,ds2a)
      else
        ds2a=dr2(1)*js
        evalCoeffOrder6(2,1,dr2a,ds2a)
      end if
#endFile


#beginFile declareTemporaryVariablesOrder6.h
!  declareTemporaryVariables(DIM,MAXDERIV)
      declareTemporaryVariables(2,8)

! declareParametricDerivativeVariables(v,DIM)
      declareParametricDerivativeVariables(uu1,2)
      declareParametricDerivativeVariables(vv1,2)
      declareParametricDerivativeVariables(ww1,2)
      declareJacobianDerivativeVariables(a1j2,2)
      declareJacobianDerivativeVariables(a1j4,2)
      declareJacobianDerivativeVariables(a1j6,2)
     
      declareParametricDerivativeVariables(uu2,2)
      declareParametricDerivativeVariables(vv2,2)
      declareParametricDerivativeVariables(ww2,2)
      declareJacobianDerivativeVariables(a2j2,2)
      declareJacobianDerivativeVariables(a2j4,2)
      declareJacobianDerivativeVariables(a2j6,2)
#endFile
