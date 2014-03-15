! =============================================================================================
! bpp macros defining macros for conservative coefficient matrix approximations to
!              laplace
!              div(scalar grad)
!              div( tensor.grad )
!              Dx( scalar Dy )
! =============================================================================================

! include macros that define  getDivTensorGradCoeffOrder2Dim2() etc.
#Include "conservativeCoefficientMatrixMacros.h"


! ===============================================================================================
! Declare variables needed by the conservative coefficients 
!  ORDER : maximum order of accuracy to be used. 
! ===============================================================================================
#beginMacro declareConservativeCoefficientVariables(ORDER)
! real dp1u,dp2u,dz1u,dz2u,dp1Cubedu,dp2Cubedu,dzdpdm1u,dzdpdm2u
! real a11,a12,a22,a21, a11r,a12r,a22r,a21r
real a11i,a11m1,a11p1,a11m2,a11p2,a11mh,a11ph,a11p3h,a11m3h,a11p3,a11m3,a11ph2,a11mh2
real a11ph6,a11mh6,a11p3h4,a11ph4,a11mh4,a11m3h4,a11p3h2,a11m3h2,a11p5h2,a11m5h2
real a22i,a22m1,a22p1,a22m2,a22p2,a22mh,a22ph,a22p3h,a22m3h,a22p3,a22m3,a22ph2,a22mh2
real a22ph6,a22mh6,a22p3h4,a22ph4,a22mh4,a22m3h4,a22p3h2,a22m3h2,a22p5h2,a22m5h2
real a33i,a33m1,a33p1,a33m2,a33p2,a33mh,a33ph,a33p3h,a33m3h,a33p3,a33m3,a33ph2,a33mh2
real a33ph6,a33mh6,a33p3h4,a33ph4,a33mh4,a33m3h4,a33p3h2,a33m3h2,a33p5h2,a33m5h2
! real a113dr,a223dr,a333dr,a123dr,a133dr,a233dr,a213dr,a313dr,a323dr

real a12m1zz,a12p1zz,a12zm1z,a12zp1z,a12zzm1,a12zzp1
real a12m2zz,a12p2zz,a12zm2z,a12zp2z,a12zzm2,a12zzp2
real a12m3zz,a12p3zz,a12zm3z,a12zp3z,a12zzm3,a12zzp3
real a12m4zz,a12p4zz,a12zm4z,a12zp4z,a12zzm4,a12zzp4

real a13m1zz,a13p1zz,a13zm1z,a13zp1z,a13zzm1,a13zzp1
real a13m2zz,a13p2zz,a13zm2z,a13zp2z,a13zzm2,a13zzp2
real a13m3zz,a13p3zz,a13zm3z,a13zp3z,a13zzm3,a13zzp3
real a13m4zz,a13p4zz,a13zm4z,a13zp4z,a13zzm4,a13zzp4

real a23m1zz,a23p1zz,a23zm1z,a23zp1z,a23zzm1,a23zzp1
real a23m2zz,a23p2zz,a23zm2z,a23zp2z,a23zzm2,a23zzp2
real a23m3zz,a23p3zz,a23zm3z,a23zp3z,a23zzm3,a23zzp3
real a23m4zz,a23p4zz,a23zm4z,a23zp4z,a23zzm4,a23zzp4

#If #ORDER == "8"
real a11m4,a11p4,a11ph8,a11mh8,a11p3h6,a11m3h6,a11p5h4,a11m5h4,a11p7h2,a11m7h2
real a22m4,a22p4,a22ph8,a22mh8,a22p3h6,a22m3h6,a22p5h4,a22m5h4,a22p7h2,a22m7h2
real a33m4,a33p4,a33ph8,a33mh8,a33p3h6,a33m3h6,a33p5h4,a33m5h4,a33p7h2,a33m7h2
#End
real jac

! real ajac3d,a113d,a223d,a333d,a123d,a133d,a233d,a213d,a313d,a323d
! real dp1r,dp1s,dp1t,dzr,dzs,dzt,dp3r,dp3s,dp3t,dp5r,dp5s,dp5t,dzpmr,dzpms,dzpmt,dp7r,dp7s,dp7t
#endMacro



! ===============================================================================================
! Declare inline macros needed by the conservative coefficients 
!  OPERATOR: 
!          divTensorGrad
!          divScalarGrad
!          laplacian
!          DxScalarDy, ... TODO ...
!          
! Implied arguments:
!  $DIM: 2 or 3 
!  $ORDER : 2,4,6,8
!  $GRIDTYPE : "curvilinear", "rectangular"
! ===============================================================================================
#beginMacro defineConservativeCoefficients(OPERATOR,s)

#defineMacro ajac(i1,i2,i3) (rx(i1,i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3))

#If #OPERATOR == "laplacian"

#defineMacro a11(i1,i2,i3) ((rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)/ajac(i1,i2,i3))
#defineMacro a22(i1,i2,i3) ((sx(i1,i2,i3)**2+sy(i1,i2,i3)**2)/ajac(i1,i2,i3))
#defineMacro a12(i1,i2,i3) ((rx(i1,i2,i3)*sx(i1,i2,i3)+ry(i1,i2,i3)*sy(i1,i2,i3))/ajac(i1,i2,i3))

! rectangular grid versions
#defineMacro a11r(i1,i2,i3) 1.
#defineMacro a21r(i1,i2,i3) 0.
#defineMacro a12r(i1,i2,i3) 0.
#defineMacro a22r(i1,i2,i3) 1.

#Elif #OPERATOR == "divScalarGrad"

#defineMacro a11(i1,i2,i3) ((rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*s(i1,i2,i3,0)/ajac(i1,i2,i3))
#defineMacro a22(i1,i2,i3) ((sx(i1,i2,i3)**2+sy(i1,i2,i3)**2)*s(i1,i2,i3,0)/ajac(i1,i2,i3))
#defineMacro a12(i1,i2,i3) ((rx(i1,i2,i3)*sx(i1,i2,i3)+ry(i1,i2,i3)*sy(i1,i2,i3))*s(i1,i2,i3,0)/ajac(i1,i2,i3))

! rectangular grid versions
#defineMacro a11r(i1,i2,i3) s(i1,i2,i3,0)
#defineMacro a21r(i1,i2,i3) 0.
#defineMacro a12r(i1,i2,i3) 0.
#defineMacro a22r(i1,i2,i3) s(i1,i2,i3,0)

#Elif #OPERATOR == "divTensorGrad"

#defineMacro a11(i1,i2,i3) ( (s(i1,i2,i3,0)*rx(i1,i2,i3)**2+\
                (s(i1,i2,i3,1)+\
                 s(i1,i2,i3,2))*rx(i1,i2,i3)*ry(i1,i2,i3)+\
                 s(i1,i2,i3,3)*ry(i1,i2,i3)**2)/ajac(i1,i2,i3) )
#defineMacro a12(i1,i2,i3) ( (s(i1,i2,i3,0)*rx(i1,i2,i3)*sx(i1,i2,i3)+\
                 s(i1,i2,i3,1)*ry(i1,i2,i3)*sx(i1,i2,i3)+\
                 s(i1,i2,i3,2)*rx(i1,i2,i3)*sy(i1,i2,i3)+\
                 s(i1,i2,i3,3)*ry(i1,i2,i3)*sy(i1,i2,i3))/ajac(i1,i2,i3) )
#defineMacro a22(i1,i2,i3) ( (s(i1,i2,i3,0)*sx(i1,i2,i3)**2+\
                (s(i1,i2,i3,1)+\
                 s(i1,i2,i3,2))*sx(i1,i2,i3)*sy(i1,i2,i3)+\
                 s(i1,i2,i3,3)*sy(i1,i2,i3)**2)/ajac(i1,i2,i3) )
#defineMacro a21(i1,i2,i3) ( (s(i1,i2,i3,0)*sx(i1,i2,i3)*rx(i1,i2,i3)+\
                 s(i1,i2,i3,1)*sy(i1,i2,i3)*rx(i1,i2,i3)+\
                 s(i1,i2,i3,2)*sx(i1,i2,i3)*ry(i1,i2,i3)+\
                 s(i1,i2,i3,3)*sy(i1,i2,i3)*ry(i1,i2,i3))/ajac(i1,i2,i3) )

! rectangular grid versions
#defineMacro a11r(i1,i2,i3) s(i1,i2,i3,0)
#defineMacro a21r(i1,i2,i3) s(i1,i2,i3,1)
#defineMacro a12r(i1,i2,i3) s(i1,i2,i3,2)
#defineMacro a22r(i1,i2,i3) s(i1,i2,i3,3)
#Else
  ERROR
#End

#defineMacro ajac3d(i1,i2,i3) ((rx(i1,i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3))*tz(i1,i2,i3)+\
                 (ry(i1,i2,i3)*sz(i1,i2,i3)-rz(i1,i2,i3)*sy(i1,i2,i3))*tx(i1,i2,i3)+\
                 (rz(i1,i2,i3)*sx(i1,i2,i3)-rx(i1,i2,i3)*sz(i1,i2,i3))*ty(i1,i2,i3))

#If #OPERATOR == "laplacian"

#defineMacro a113d(i1,i2,i3) ((rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(i1,i2,i3)**2)/ajac3d(i1,i2,i3))
#defineMacro a223d(i1,i2,i3) ((sx(i1,i2,i3)**2+sy(i1,i2,i3)**2+sz(i1,i2,i3)**2)/ajac3d(i1,i2,i3))
#defineMacro a333d(i1,i2,i3) ((tx(i1,i2,i3)**2+ty(i1,i2,i3)**2+tz(i1,i2,i3)**2)/ajac3d(i1,i2,i3))
#defineMacro a123d(i1,i2,i3) ((rx(i1,i2,i3)*sx(i1,i2,i3)+ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,i3))/ajac3d(i1,i2,i3))
#defineMacro a133d(i1,i2,i3) ((rx(i1,i2,i3)*tx(i1,i2,i3)+ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))/ajac3d(i1,i2,i3))
#defineMacro a233d(i1,i2,i3) ((sx(i1,i2,i3)*tx(i1,i2,i3)+sy(i1,i2,i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3))/ajac3d(i1,i2,i3))

! rectangular grid versions
#defineMacro a113dr(i1,i2,i3)  1.
#defineMacro a213dr(i1,i2,i3)  0.
#defineMacro a313dr(i1,i2,i3)  0.
#defineMacro a123dr(i1,i2,i3)  0.
#defineMacro a223dr(i1,i2,i3)  1. 
#defineMacro a323dr(i1,i2,i3)  0.
#defineMacro a133dr(i1,i2,i3)  0.
#defineMacro a233dr(i1,i2,i3)  0.
#defineMacro a333dr(i1,i2,i3)  1. 

#Elif #OPERATOR == "divScalarGrad"

#defineMacro a113d(i1,i2,i3) ((rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(i1,i2,i3)**2)*s(i1,i2,i3,0)/ajac3d(i1,i2,i3))
#defineMacro a223d(i1,i2,i3) ((sx(i1,i2,i3)**2+sy(i1,i2,i3)**2+sz(i1,i2,i3)**2)*s(i1,i2,i3,0)/ajac3d(i1,i2,i3))
#defineMacro a333d(i1,i2,i3) ((tx(i1,i2,i3)**2+ty(i1,i2,i3)**2+tz(i1,i2,i3)**2)*s(i1,i2,i3,0)/ajac3d(i1,i2,i3))
#defineMacro a123d(i1,i2,i3) ((rx(i1,i2,i3)*sx(i1,i2,i3)+ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,i3))\
                                                                 *s(i1,i2,i3,0)/ajac3d(i1,i2,i3))
#defineMacro a133d(i1,i2,i3) ((rx(i1,i2,i3)*tx(i1,i2,i3)+ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))\
                                                                 *s(i1,i2,i3,0)/ajac3d(i1,i2,i3))
#defineMacro a233d(i1,i2,i3) ((sx(i1,i2,i3)*tx(i1,i2,i3)+sy(i1,i2,i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3))\
                                                                 *s(i1,i2,i3,0)/ajac3d(i1,i2,i3))
! rectangular grid versions
#defineMacro a113dr(i1,i2,i3)  s(i1,i2,i3,0)
#defineMacro a213dr(i1,i2,i3)  0.
#defineMacro a313dr(i1,i2,i3)  0.
#defineMacro a123dr(i1,i2,i3)  0.
#defineMacro a223dr(i1,i2,i3)  s(i1,i2,i3,0)
#defineMacro a323dr(i1,i2,i3)  0.
#defineMacro a133dr(i1,i2,i3)  0.
#defineMacro a233dr(i1,i2,i3)  0.
#defineMacro a333dr(i1,i2,i3)  s(i1,i2,i3,0)

#Elif #OPERATOR == "divTensorGrad"

#defineMacro a113d(i1,i2,i3) ((s(i1,i2,i3,0)*rx(i1,i2,i3)**2+\
                 s(i1,i2,i3,4)*ry(i1,i2,i3)**2+\
                 s(i1,i2,i3,8)*rz(i1,i2,i3)**2+\
                 (s(i1,i2,i3,3)+s(i1,i2,i3,1))*rx(i1,i2,i3)*ry(i1,i2,i3)+\
                 (s(i1,i2,i3,6)+s(i1,i2,i3,2))*rx(i1,i2,i3)*rz(i1,i2,i3)+\
                 (s(i1,i2,i3,7)+s(i1,i2,i3,5))*ry(i1,i2,i3)*rz(i1,i2,i3))/ajac3d(i1,i2,i3))
#defineMacro a223d(i1,i2,i3) ((s(i1,i2,i3,0)*sx(i1,i2,i3)**2+\
                 s(i1,i2,i3,4)*sy(i1,i2,i3)**2+\
                 s(i1,i2,i3,8)*sz(i1,i2,i3)**2+\
                 (s(i1,i2,i3,3)+s(i1,i2,i3,1))*sx(i1,i2,i3)*sy(i1,i2,i3)+\
                 (s(i1,i2,i3,6)+s(i1,i2,i3,2))*sx(i1,i2,i3)*sz(i1,i2,i3)+\
                 (s(i1,i2,i3,7)+s(i1,i2,i3,5))*sy(i1,i2,i3)*sz(i1,i2,i3))/ajac3d(i1,i2,i3))
#defineMacro a333d(i1,i2,i3) ((s(i1,i2,i3,0)*tx(i1,i2,i3)**2+\
                 s(i1,i2,i3,4)*ty(i1,i2,i3)**2+\
                 s(i1,i2,i3,8)*tz(i1,i2,i3)**2+\
                 (s(i1,i2,i3,3)+s(i1,i2,i3,1))*tx(i1,i2,i3)*ty(i1,i2,i3)+\
                 (s(i1,i2,i3,6)+s(i1,i2,i3,2))*tx(i1,i2,i3)*tz(i1,i2,i3)+\
                 (s(i1,i2,i3,7)+s(i1,i2,i3,5))*ty(i1,i2,i3)*tz(i1,i2,i3))/ajac3d(i1,i2,i3))
#defineMacro a123d(i1,i2,i3) ((s(i1,i2,i3,0)*rx(i1,i2,i3)*sx(i1,i2,i3)+\
                 s(i1,i2,i3,4)*ry(i1,i2,i3)*sy(i1,i2,i3)+\
                 s(i1,i2,i3,8)*rz(i1,i2,i3)*sz(i1,i2,i3)+\
                 s(i1,i2,i3,3)*rx(i1,i2,i3)*sy(i1,i2,i3)+\
                 s(i1,i2,i3,6)*rx(i1,i2,i3)*sz(i1,i2,i3)+\
                 s(i1,i2,i3,1)*ry(i1,i2,i3)*sx(i1,i2,i3)+\
                 s(i1,i2,i3,7)*ry(i1,i2,i3)*sz(i1,i2,i3)+\
                 s(i1,i2,i3,2)*rz(i1,i2,i3)*sx(i1,i2,i3)+\
                 s(i1,i2,i3,5)*rz(i1,i2,i3)*sy(i1,i2,i3))/ajac3d(i1,i2,i3))
#defineMacro a133d(i1,i2,i3) ((s(i1,i2,i3,0)*rx(i1,i2,i3)*tx(i1,i2,i3)+\
                 s(i1,i2,i3,4)*ry(i1,i2,i3)*ty(i1,i2,i3)+\
                 s(i1,i2,i3,8)*rz(i1,i2,i3)*tz(i1,i2,i3)+\
                 s(i1,i2,i3,3)*rx(i1,i2,i3)*ty(i1,i2,i3)+\
                 s(i1,i2,i3,6)*rx(i1,i2,i3)*tz(i1,i2,i3)+\
                 s(i1,i2,i3,1)*ry(i1,i2,i3)*tx(i1,i2,i3)+\
                 s(i1,i2,i3,7)*ry(i1,i2,i3)*tz(i1,i2,i3)+\
                 s(i1,i2,i3,2)*rz(i1,i2,i3)*tx(i1,i2,i3)+\
                 s(i1,i2,i3,5)*rz(i1,i2,i3)*ty(i1,i2,i3))/ajac3d(i1,i2,i3))
#defineMacro a233d(i1,i2,i3) ((s(i1,i2,i3,0)*sx(i1,i2,i3)*tx(i1,i2,i3)+\
                 s(i1,i2,i3,4)*sy(i1,i2,i3)*ty(i1,i2,i3)+\
                 s(i1,i2,i3,8)*sz(i1,i2,i3)*tz(i1,i2,i3)+\
                 s(i1,i2,i3,3)*sx(i1,i2,i3)*ty(i1,i2,i3)+\
                 s(i1,i2,i3,6)*sx(i1,i2,i3)*tz(i1,i2,i3)+\
                 s(i1,i2,i3,1)*sy(i1,i2,i3)*tx(i1,i2,i3)+\
                 s(i1,i2,i3,7)*sy(i1,i2,i3)*tz(i1,i2,i3)+\
                 s(i1,i2,i3,2)*sz(i1,i2,i3)*tx(i1,i2,i3)+\
                 s(i1,i2,i3,5)*sz(i1,i2,i3)*ty(i1,i2,i3))/ajac3d(i1,i2,i3)) 
#defineMacro a213d(i1,i2,i3) ((s(i1,i2,i3,0)*sx(i1,i2,i3)*rx(i1,i2,i3)+\
                 s(i1,i2,i3,4)*sy(i1,i2,i3)*ry(i1,i2,i3)+\
                 s(i1,i2,i3,8)*sz(i1,i2,i3)*rz(i1,i2,i3)+\
                 s(i1,i2,i3,3)*sx(i1,i2,i3)*ry(i1,i2,i3)+\
                 s(i1,i2,i3,6)*sx(i1,i2,i3)*rz(i1,i2,i3)+\
                 s(i1,i2,i3,1)*sy(i1,i2,i3)*rx(i1,i2,i3)+\
                 s(i1,i2,i3,7)*sy(i1,i2,i3)*rz(i1,i2,i3)+\
                 s(i1,i2,i3,2)*sz(i1,i2,i3)*rx(i1,i2,i3)+\
                 s(i1,i2,i3,5)*sz(i1,i2,i3)*ry(i1,i2,i3))/ajac3d(i1,i2,i3))
#defineMacro a313d(i1,i2,i3) ((s(i1,i2,i3,0)*tx(i1,i2,i3)*rx(i1,i2,i3)+\
                 s(i1,i2,i3,4)*ty(i1,i2,i3)*ry(i1,i2,i3)+\
                 s(i1,i2,i3,8)*tz(i1,i2,i3)*rz(i1,i2,i3)+\
                 s(i1,i2,i3,3)*tx(i1,i2,i3)*ry(i1,i2,i3)+\
                 s(i1,i2,i3,6)*tx(i1,i2,i3)*rz(i1,i2,i3)+\
                 s(i1,i2,i3,1)*ty(i1,i2,i3)*rx(i1,i2,i3)+\
                 s(i1,i2,i3,7)*ty(i1,i2,i3)*rz(i1,i2,i3)+\
                 s(i1,i2,i3,2)*tz(i1,i2,i3)*rx(i1,i2,i3)+\
                 s(i1,i2,i3,5)*tz(i1,i2,i3)*ry(i1,i2,i3))/ajac3d(i1,i2,i3)) 
#defineMacro a323d(i1,i2,i3) ((s(i1,i2,i3,0)*tx(i1,i2,i3)*sx(i1,i2,i3)+\
                 s(i1,i2,i3,4)*ty(i1,i2,i3)*sy(i1,i2,i3)+\
                 s(i1,i2,i3,8)*tz(i1,i2,i3)*sz(i1,i2,i3)+\
                 s(i1,i2,i3,3)*tx(i1,i2,i3)*sy(i1,i2,i3)+\
                 s(i1,i2,i3,6)*tx(i1,i2,i3)*sz(i1,i2,i3)+\
                 s(i1,i2,i3,1)*ty(i1,i2,i3)*sx(i1,i2,i3)+\
                 s(i1,i2,i3,7)*ty(i1,i2,i3)*sz(i1,i2,i3)+\
                 s(i1,i2,i3,2)*tz(i1,i2,i3)*sx(i1,i2,i3)+\
                 s(i1,i2,i3,5)*tz(i1,i2,i3)*sy(i1,i2,i3))/ajac3d(i1,i2,i3))


! rectangular grid versions
#defineMacro a113dr(i1,i2,i3)  s(i1,i2,i3,0)
#defineMacro a213dr(i1,i2,i3)  s(i1,i2,i3,1)
#defineMacro a313dr(i1,i2,i3)  s(i1,i2,i3,2)
#defineMacro a123dr(i1,i2,i3)  s(i1,i2,i3,3)
#defineMacro a223dr(i1,i2,i3)  s(i1,i2,i3,4)
#defineMacro a323dr(i1,i2,i3)  s(i1,i2,i3,5)
#defineMacro a133dr(i1,i2,i3)  s(i1,i2,i3,6)
#defineMacro a233dr(i1,i2,i3)  s(i1,i2,i3,7)
#defineMacro a333dr(i1,i2,i3)  s(i1,i2,i3,8)

#Else
  ERROR
#End

#endMacro

! ==================================================================================================
! Define the values of the coefficients at x+ m*h/2  for m=+1,-1,+2,-2,...
!
!  OPERATOR: 
!          divTensorGrad
!          divScalarGrad
!          laplacian
!          DxScalarDy, 
!          
! old: 
!          DSG = divScalarGrad
!          LAPLACE = laplace
!          DSGR = divScalarGrad (rectangular)
!

! Implied arguments:
!  $DIM: 2 or 3 
!  $ORDER : 2,4,6,8
!  $GRIDTYPE : "curvilinear", "rectangular"
!
!  (originally from op/fortranDeriv/dsgc6.bf)
! ==================================================================================================
#beginMacro defineMidValueCoefficients(OPERATOR,s)

#If $GRIDTYPE == "curvilinear"

 #If $DIM == 2 

  a11i  = a11(i1,i2,i3)
  a11m1 = a11(i1-1,i2,i3)
  a11p1 = a11(i1+1,i2,i3)

  a22i  = a22(i1,i2,i3)
  a22m1 = a22(i1,i2-1,i3)
  a22p1 = a22(i1,i2+1,i3)

  a12m1zz = a12(i1-1,i2,i3)
  a12p1zz = a12(i1+1,i2,i3)
  a12zm1z = a12(i1,i2-1,i3)
  a12zp1z = a12(i1,i2+1,i3)

 #If $ORDER > 2
  a11m2 = a11(i1-2,i2,i3)
  a11p2 = a11(i1+2,i2,i3)
  a22m2 = a22(i1,i2-2,i3)
  a22p2 = a22(i1,i2+2,i3)

  a12m2zz = a12(i1-2,i2,i3)
  a12p2zz = a12(i1+2,i2,i3)
  a12zm2z = a12(i1,i2-2,i3)
  a12zp2z = a12(i1,i2+2,i3)

 #End
 #If $ORDER > 4 
  a11m3 = a11(i1-3,i2,i3)
  a11p3 = a11(i1+3,i2,i3)
  a22m3 = a22(i1,i2-3,i3)
  a22p3 = a22(i1,i2+3,i3)

  a12m3zz = a12(i1-3,i2,i3)
  a12p3zz = a12(i1+3,i2,i3)
  a12zm3z = a12(i1,i2-3,i3)
  a12zp3z = a12(i1,i2+3,i3)

 #End
  
 #If $ORDER > 6
  a11m4 = a11(i1-4,i2,i3)
  a11p4 = a11(i1+4,i2,i3)
  a22m4 = a22(i1,i2-4,i3)
  a22p4 = a22(i1,i2+4,i3)

  a12m4zz = a12(i1-4,i2,i3)
  a12p4zz = a12(i1+4,i2,i3)
  a12zm4z = a12(i1,i2-4,i3)
  a12zp4z = a12(i1,i2+4,i3)

 #End

 #Else ! *** 3D ***

  a11i  = a113d(i1  ,i2,i3)
  a11m1 = a113d(i1-1,i2,i3)
  a11p1 = a113d(i1+1,i2,i3)
  a22i  = a223d(i1,i2  ,i3)
  a22m1 = a223d(i1,i2-1,i3)
  a22p1 = a223d(i1,i2+1,i3)
  a33i  = a333d(i1,i2,i3  )
  a33m1 = a333d(i1,i2,i3-1)
  a33p1 = a333d(i1,i2,i3+1)

  a12m1zz = a123d(i1-1,i2,i3)
  a12p1zz = a123d(i1+1,i2,i3)
  a12zm1z = a123d(i1,i2-1,i3)
  a12zp1z = a123d(i1,i2+1,i3)

  a13m1zz = a133d(i1-1,i2,i3)
  a13p1zz = a133d(i1+1,i2,i3)
  a13zm1z = a133d(i1,i2-1,i3)
  a13zp1z = a133d(i1,i2+1,i3)

  a23m1zz = a233d(i1-1,i2,i3)
  a23p1zz = a233d(i1+1,i2,i3)
  a23zm1z = a233d(i1,i2-1,i3)
  a23zp1z = a233d(i1,i2+1,i3)

 #If $ORDER > 2
  a11m2 = a113d(i1-2,i2,i3)
  a11p2 = a113d(i1+2,i2,i3)
  a22m2 = a223d(i1,i2-2,i3)
  a22p2 = a223d(i1,i2+2,i3)
  a33m2 = a333d(i1,i2,i3-2)
  a33p2 = a333d(i1,i2,i3+2)

  a12m2zz = a123d(i1-2,i2,i3)
  a12p2zz = a123d(i1+2,i2,i3)
  a12zm2z = a123d(i1,i2-2,i3)
  a12zp2z = a123d(i1,i2+2,i3)

  a13m2zz = a133d(i1-2,i2,i3)
  a13p2zz = a133d(i1+2,i2,i3)
  a13zm2z = a133d(i1,i2-2,i3)
  a13zp2z = a133d(i1,i2+2,i3)

  a23m2zz = a233d(i1-2,i2,i3)
  a23p2zz = a233d(i1+2,i2,i3)
  a23zm2z = a233d(i1,i2-2,i3)
  a23zp2z = a233d(i1,i2+2,i3)

 #End

 #If $ORDER > 4
  a11m3 = a113d(i1-3,i2,i3)
  a11p3 = a113d(i1+3,i2,i3)
  a22m3 = a223d(i1,i2-3,i3)
  a22p3 = a223d(i1,i2+3,i3)
  a33m3 = a333d(i1,i2,i3-3)
  a33p3 = a333d(i1,i2,i3+3)

  a12m3zz = a123d(i1-3,i2,i3)
  a12p3zz = a123d(i1+3,i2,i3)
  a12zm3z = a123d(i1,i2-3,i3)
  a12zp3z = a123d(i1,i2+3,i3)

  a13m3zz = a133d(i1-3,i2,i3)
  a13p3zz = a133d(i1+3,i2,i3)
  a13zm3z = a133d(i1,i2-3,i3)
  a13zp3z = a133d(i1,i2+3,i3)

  a23m3zz = a233d(i1-3,i2,i3)
  a23p3zz = a233d(i1+3,i2,i3)
  a23zm3z = a233d(i1,i2-3,i3)
  a23zp3z = a233d(i1,i2+3,i3)

 #End
  

 #If $ORDER > 6
  a11m4 = a113d(i1-4,i2,i3)
  a11p4 = a113d(i1+4,i2,i3)
  a22m4 = a223d(i1,i2-4,i3)
  a22p4 = a223d(i1,i2+4,i3)
  a33m4 = a333d(i1,i2,i3-4)
  a33p4 = a333d(i1,i2,i3+4)

  a12m4zz = a123d(i1-4,i2,i3)
  a12p4zz = a123d(i1+4,i2,i3)
  a12zm4z = a123d(i1,i2-4,i3)
  a12zp4z = a123d(i1,i2+4,i3)

  a13m4zz = a133d(i1-4,i2,i3)
  a13p4zz = a133d(i1+4,i2,i3)
  a13zm4z = a133d(i1,i2-4,i3)
  a13zp4z = a133d(i1,i2+4,i3)

  a23m4zz = a233d(i1-4,i2,i3)
  a23p4zz = a233d(i1+4,i2,i3)
  a23zm4z = a233d(i1,i2-4,i3)
  a23zp4z = a233d(i1,i2+4,i3)

 #End

 #End ! DIM

#Elif $GRIDTYPE == "rectangular"


 #If $DIM == 2 

  a11i  = a11r(i1,i2,i3)
  a11m1 = a11r(i1-1,i2,i3)
  a11p1 = a11r(i1+1,i2,i3)

  a22i  = a22r(i1,i2,i3)
  a22m1 = a22r(i1,i2-1,i3)
  a22p1 = a22r(i1,i2+1,i3)

  a12m1zz = a12r(i1-1,i2,i3)
  a12p1zz = a12r(i1+1,i2,i3)
  a12zm1z = a12r(i1,i2-1,i3)
  a12zp1z = a12r(i1,i2+1,i3)

 #If $ORDER > 2
  a11m2 = a11r(i1-2,i2,i3)
  a11p2 = a11r(i1+2,i2,i3)
  a22m2 = a22r(i1,i2-2,i3)
  a22p2 = a22r(i1,i2+2,i3)

  a12m2zz = a12r(i1-2,i2,i3)
  a12p2zz = a12r(i1+2,i2,i3)
  a12zm2z = a12r(i1,i2-2,i3)
  a12zp2z = a12r(i1,i2+2,i3)

 #End
 #If $ORDER > 4 
  a11m3 = a11r(i1-3,i2,i3)
  a11p3 = a11r(i1+3,i2,i3)
  a22m3 = a22r(i1,i2-3,i3)
  a22p3 = a22r(i1,i2+3,i3)

  a12m3zz = a12r(i1-3,i2,i3)
  a12p3zz = a12r(i1+3,i2,i3)
  a12zm3z = a12r(i1,i2-3,i3)
  a12zp3z = a12r(i1,i2+3,i3)

 #End
  
 #If $ORDER > 6
  a11m4 = a11r(i1-4,i2,i3)
  a11p4 = a11r(i1+4,i2,i3)
  a22m4 = a22r(i1,i2-4,i3)
  a22p4 = a22r(i1,i2+4,i3)

  a12m4zz = a12r(i1-4,i2,i3)
  a12p4zz = a12r(i1+4,i2,i3)
  a12zm4z = a12r(i1,i2-4,i3)
  a12zp4z = a12r(i1,i2+4,i3)

 #End

 #Else ! *** 3D ***

  a11i  = a113dr(i1  ,i2,i3)
  a11m1 = a113dr(i1-1,i2,i3)
  a11p1 = a113dr(i1+1,i2,i3)
  a22i  = a223dr(i1,i2  ,i3)
  a22m1 = a223dr(i1,i2-1,i3)
  a22p1 = a223dr(i1,i2+1,i3)
  a33i  = a333dr(i1,i2,i3  )
  a33m1 = a333dr(i1,i2,i3-1)
  a33p1 = a333dr(i1,i2,i3+1)

  a12m1zz = a123dr(i1-1,i2,i3)
  a12p1zz = a123dr(i1+1,i2,i3)
  a12zm1z = a123dr(i1,i2-1,i3)
  a12zp1z = a123dr(i1,i2+1,i3)

  a13m1zz = a133dr(i1-1,i2,i3)
  a13p1zz = a133dr(i1+1,i2,i3)
  a13zm1z = a133dr(i1,i2-1,i3)
  a13zp1z = a133dr(i1,i2+1,i3)

  a23m1zz = a233dr(i1-1,i2,i3)
  a23p1zz = a233dr(i1+1,i2,i3)
  a23zm1z = a233dr(i1,i2-1,i3)
  a23zp1z = a233dr(i1,i2+1,i3)

 #If $ORDER > 2
  a11m2 = a113dr(i1-2,i2,i3)
  a11p2 = a113dr(i1+2,i2,i3)
  a22m2 = a223dr(i1,i2-2,i3)
  a22p2 = a223dr(i1,i2+2,i3)
  a33m2 = a333dr(i1,i2,i3-2)
  a33p2 = a333dr(i1,i2,i3+2)

  a12m2zz = a123dr(i1-2,i2,i3)
  a12p2zz = a123dr(i1+2,i2,i3)
  a12zm2z = a123dr(i1,i2-2,i3)
  a12zp2z = a123dr(i1,i2+2,i3)

  a13m2zz = a133dr(i1-2,i2,i3)
  a13p2zz = a133dr(i1+2,i2,i3)
  a13zm2z = a133dr(i1,i2-2,i3)
  a13zp2z = a133dr(i1,i2+2,i3)

  a23m2zz = a233dr(i1-2,i2,i3)
  a23p2zz = a233dr(i1+2,i2,i3)
  a23zm2z = a233dr(i1,i2-2,i3)
  a23zp2z = a233dr(i1,i2+2,i3)

 #End

 #If $ORDER > 4
  a11m3 = a113dr(i1-3,i2,i3)
  a11p3 = a113dr(i1+3,i2,i3)
  a22m3 = a223dr(i1,i2-3,i3)
  a22p3 = a223dr(i1,i2+3,i3)
  a33m3 = a333dr(i1,i2,i3-3)
  a33p3 = a333dr(i1,i2,i3+3)

  a12m3zz = a123dr(i1-3,i2,i3)
  a12p3zz = a123dr(i1+3,i2,i3)
  a12zm3z = a123dr(i1,i2-3,i3)
  a12zp3z = a123dr(i1,i2+3,i3)

  a13m3zz = a133dr(i1-3,i2,i3)
  a13p3zz = a133dr(i1+3,i2,i3)
  a13zm3z = a133dr(i1,i2-3,i3)
  a13zp3z = a133dr(i1,i2+3,i3)

  a23m3zz = a233dr(i1-3,i2,i3)
  a23p3zz = a233dr(i1+3,i2,i3)
  a23zm3z = a233dr(i1,i2-3,i3)
  a23zp3z = a233dr(i1,i2+3,i3)

 #End
  

 #If $ORDER > 6
  a11m4 = a113dr(i1-4,i2,i3)
  a11p4 = a113dr(i1+4,i2,i3)
  a22m4 = a223dr(i1,i2-4,i3)
  a22p4 = a223dr(i1,i2+4,i3)
  a33m4 = a333dr(i1,i2,i3-4)
  a33p4 = a333dr(i1,i2,i3+4)

  a12m4zz = a123dr(i1-4,i2,i3)
  a12p4zz = a123dr(i1+4,i2,i3)
  a12zm4z = a123dr(i1,i2-4,i3)
  a12zp4z = a123dr(i1,i2+4,i3)

  a13m4zz = a133dr(i1-4,i2,i3)
  a13p4zz = a133dr(i1+4,i2,i3)
  a13zm4z = a133dr(i1,i2-4,i3)
  a13zp4z = a133dr(i1,i2+4,i3)

  a23m4zz = a233dr(i1-4,i2,i3)
  a23p4zz = a233dr(i1+4,i2,i3)
  a23zm4z = a233dr(i1,i2-4,i3)
  a23zp4z = a233dr(i1,i2+4,i3)

 #End

 #End ! DIM

#Else 
  ! Is this last section used for anything ??

 a11i  = s(i1  ,i2,i3,0)
 a11m1 = s(i1-1,i2,i3,0)
 a11p1 = s(i1+1,i2,i3,0)
 a11m2 = s(i1-2,i2,i3,0)
 a11p2 = s(i1+2,i2,i3,0)
 a11m3 = s(i1-3,i2,i3,0)
 a11p3 = s(i1+3,i2,i3,0)
 
 a22i  = s(i1,i2,i3  ,0)
 a22m1 = s(i1,i2-1,i3,0)
 a22p1 = s(i1,i2+1,i3,0)
 a22m2 = s(i1,i2-2,i3,0)
 a22p2 = s(i1,i2+2,i3,0)
 a22m3 = s(i1,i2-3,i3,0)
 a22p3 = s(i1,i2+3,i3,0)
 
 #If $ORDER == 8
  a11m4 = s(i1-4,i2,i3,0)
  a11p4 = s(i1+4,i2,i3,0)
  a22m4 = s(i1,i2-4,i3,0)
  a22p4 = s(i1,i2+4,i3,0)
 #End

 #If $DIM == 3 
  a33i  = s(i1,i2,i3  ,0)
  a33m1 = s(i1,i2,i3-1,0)
  a33p1 = s(i1,i2,i3+1,0)
  a33m2 = s(i1,i2,i3-2,0)
  a33p2 = s(i1,i2,i3+2,0)
  a33m3 = s(i1,i2,i3-3,0)
  a33p3 = s(i1,i2,i3+3,0)
  #If $ORDER == 8
   a33m4 = s(i1,i2,i3-4,0)
   a33p4 = s(i1,i2,i3+4,0)
  #End
 #End

#End

 #If $ORDER > 4 
  a11ph6 = (150.*(a11p1+a11i)-25.*(a11p2+a11m1)+3.*(a11p3+a11m2))/256.  ! 6th-order approx
  a11mh6 = (150.*(a11i+a11m1)-25.*(a11p1+a11m2)+3.*(a11p2+a11m3))/256.
 
  a11ph4 = ((a11p1+a11i )*9.-(a11p2+a11m1))/16.                        
  a11mh4 = ((a11i +a11m1)*9.-(a11p1+a11m2))/16.
  a11p3h4= ((a11p2+a11p1)*9.-(a11p3+a11i ))/16.                          ! 4th-order approx
  a11m3h4= ((a11m1+a11m2)*9.-(a11i +a11m3))/16.
 
  a11ph2 = .5*(a11p1+a11i)  ! 2nd-order approx
  a11mh2 = .5*(a11m1+a11i)
  a11p3h2= .5*(a11p2+a11p1)
  a11m3h2= .5*(a11m1+a11m2)
  a11p5h2= .5*(a11p3+a11p2)
  a11m5h2= .5*(a11m2+a11m3)
 
  a22ph6 = (150.*(a22p1+a22i)-25.*(a22p2+a22m1)+3.*(a22p3+a22m2))/256.  ! 6th-order approx
  a22mh6 = (150.*(a22i+a22m1)-25.*(a22p1+a22m2)+3.*(a22p2+a22m3))/256.
 
  a22ph4 = ((a22p1+a22i )*9.-(a22p2+a22m1))/16.                        
  a22mh4 = ((a22i +a22m1)*9.-(a22p1+a22m2))/16.
  a22p3h4= ((a22p2+a22p1)*9.-(a22p3+a22i ))/16.                          ! 4th-order approx
  a22m3h4= ((a22m1+a22m2)*9.-(a22i +a22m3))/16.
 
  a22ph2 = .5*(a22p1+a22i)  ! 2nd-order approx
  a22mh2 = .5*(a22m1+a22i)
  a22p3h2= .5*(a22p2+a22p1)
  a22m3h2= .5*(a22m1+a22m2)
  a22p5h2= .5*(a22p3+a22p2)
  a22m5h2= .5*(a22m2+a22m3)
 #End

 #If $ORDER > 6 

  a11ph8 = (1225.*(a11p1+a11i)-245.*(a11p2+a11m1)+49.*(a11p3+a11m2)-5.*(a11p4+a11m3))/2048. ! 8th-order approx
  a11mh8 = (1225.*(a11m1+a11i)-245.*(a11m2+a11p1)+49.*(a11m3+a11p2)-5.*(a11m4+a11p3))/2048.
c a11ph8 = a11ph6
c a11mh8 = a11mh6

  a11p3h6 = (150.*(a11p2+a11p1)-25.*(a11p3+a11i )+3.*(a11p4+a11m1))/256.  ! 6th-order approx
  a11m3h6 = (150.*(a11m2+a11m1)-25.*(a11m3+a11i )+3.*(a11m4+a11p1))/256.

  a11p5h4= ((a11p3+a11p2)*9.-(a11p4+a11p1))/16.                          ! 4th-order approx
  a11m5h4= ((a11m3+a11m2)*9.-(a11m4+a11m1))/16.

  a11p7h2= .5*(a11p4+a11p3) ! 2nd order approx
  a11m7h2= .5*(a11m4+a11m3)

  a22ph8 = (1225.*(a22p1+a22i)-245.*(a22p2+a22m1)+49.*(a22p3+a22m2)-5.*(a22p4+a22m3))/2048. ! 8th-order approx
  a22mh8 = (1225.*(a22m1+a22i)-245.*(a22m2+a22p1)+49.*(a22m3+a22p2)-5.*(a22m4+a22p3))/2048.
c a22ph8 = a22ph6
c a22mh8 = a22mh6

  a22p3h6 = (150.*(a22p2+a22p1)-25.*(a22p3+a22i )+3.*(a22p4+a22m1))/256.  ! 6th-order approx
  a22m3h6 = (150.*(a22m2+a22m1)-25.*(a22m3+a22i )+3.*(a22m4+a22p1))/256.

  a22p5h4= ((a22p3+a22p2)*9.-(a22p4+a22p1))/16.                          ! 4th-order approx
  a22m5h4= ((a22m3+a22m2)*9.-(a22m4+a22m1))/16.

  a22p7h2= .5*(a22p4+a22p3) ! 2nd order approx
  a22m7h2= .5*(a22m4+a22m3)

 #End

#If $DIM == 3 

 #If $ORDER > 4 
  a33ph6 = (150.*(a33p1+a33i)-25.*(a33p2+a33m1)+3.*(a33p3+a33m2))/256.  ! 6th-order approx
  a33mh6 = (150.*(a33i+a33m1)-25.*(a33p1+a33m2)+3.*(a33p2+a33m3))/256.
 
  a33p3h4= ((a33p2+a33p1)*9.-(a33p3+a33i ))/16.                          ! 4th-order approx
  a33ph4 = ((a33p1+a33i )*9.-(a33p2+a33m1))/16.                        
  a33mh4 = ((a33i +a33m1)*9.-(a33p1+a33m2))/16.
  a33m3h4= ((a33m1+a33m2)*9.-(a33i +a33m3))/16.
 
  a33ph2 = .5*(a33p1+a33i)  ! 2nd-order approx
  a33mh2 = .5*(a33m1+a33i)
  a33p3h2= .5*(a33p2+a33p1)
  a33m3h2= .5*(a33m1+a33m2)
  a33p5h2= .5*(a33p3+a33p2)
  a33m5h2= .5*(a33m2+a33m3)
 #End


 #If $ORDER > 6 
  a33ph8 = (1225.*(a33p1+a33i)-245.*(a33p2+a33m1)+49.*(a33p3+a33m2)-5.*(a33p4+a33m3))/2048. ! 8th-order approx
  a33mh8 = (1225.*(a33m1+a33i)-245.*(a33m2+a33p1)+49.*(a33m3+a33p2)-5.*(a33m4+a33p3))/2048.

  a33p3h6 = (150.*(a33p2+a33p1)-25.*(a33p3+a33i )+3.*(a33p4+a33m1))/256.  ! 6th-order approx
  a33m3h6 = (150.*(a33m2+a33m1)-25.*(a33m3+a33i )+3.*(a33m4+a33p1))/256.

  a33p5h4= ((a33p3+a33p2)*9.-(a33p4+a33p1))/16.                          ! 4th-order approx
  a33m5h4= ((a33m3+a33m2)*9.-(a33m4+a33m1))/16.

  a33p7h2= .5*(a33p4+a33p3) ! 2nd order approx
  a33m7h2= .5*(a33m4+a33m3)

 #End

#End

#endMacro


! ====================================================================================================
! Get the coefficient matrix for a conservative approximation. 
!
!  OPERATOR : divScalarGrad
!             divTensorGrad
!             laplacian
!             DxDx, DxDy, 
!             DxScalarDx, DxScalarDy
! Implied arguments:
!    (i1,i2,i3) : assign the coefficients at this grid point.
!    $DIM, $ORDER : dimension and order of accuracy.
!    $GRIDTYPE : "curvilinear", "rectangular"
! ====================================================================================================
#beginMacro getConservativeCoeff( OPERATOR,s,coeff )

! Define inline macros for a11, a12, a22, ...
defineConservativeCoefficients(OPERATOR,s)

defineMidValueCoefficients(OPERATOR,s)

#If $GRIDTYPE == "curvilinear"
 #If $DIM == 2
  jac= ajac(i1,i2,i3)
 #Else
  jac = ajac3d(i1,i2,i3)
 #End
#Else
  jac=1.
#End

! call the macro to fill in coeff
#If $GRIDTYPE == "curvilinear"
  #peval getDivTensorGradCoeffOrder$ORDER\Dim$DIM(coeff,dr)
#Else
  #peval getDivTensorGradCoeffOrder$ORDER\Dim$DIM(coeff,dx)
#End

#endMacro
