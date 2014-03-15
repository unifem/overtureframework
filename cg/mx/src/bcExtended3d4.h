 ! results from bcExtended3d4.maple
 ! Assign values on the extended boundary next to two PEC boundaries
 !                                                                  
 ! Here we assume the following are defined                               
 !    c11,c22,c33,c1,c2,c3                                          
 !    urr,uss,utt,ur,us,ut (also for v and w)                       
 !    deltaFu,deltaFv,deltaFw = RHS for Delta(u,v,w)                
 !    g1f,g2f = RHS for extrapolation, a1.D+2^4u(i1,i2-2)=g1f, a2.D+2^4u(i1-2,i2)=g2f,    
 !                                                                  
      DeltaU = c11*urr+c22*uss+c33*utt+c1*ur+c2*us+c3*ut - deltaFu
      DeltaV = c11*vrr+c22*vss+c33*vtt+c1*vr+c2*vs+c3*vt - deltaFv
      DeltaW = c11*wrr+c22*wss+c33*wtt+c1*wr+c2*ws+c3*wt - deltaFw

! ** decompose point u(i1-is1,i2-is2,i3-is3) into components along a1,a2,a3 **
      a11c=A11D3(i1-is1,i2-is2,i3-is3)
      a12c=A12D3(i1-is1,i2-is2,i3-is3)
      a13c=A13D3(i1-is1,i2-is2,i3-is3)
      a21c=A21D3(i1-is1,i2-is2,i3-is3)
      a22c=A22D3(i1-is1,i2-is2,i3-is3)
      a23c=A23D3(i1-is1,i2-is2,i3-is3)
      a31c=A31D3(i1-is1,i2-is2,i3-is3)
      a32c=A32D3(i1-is1,i2-is2,i3-is3)
      a33c=A33D3(i1-is1,i2-is2,i3-is3)

      a1a1=a11c*a11c+a12c*a12c+a13c*a13c
      a1a2=a11c*a21c+a12c*a22c+a13c*a23c
      a1a3=a11c*a31c+a12c*a32c+a13c*a33c
      a2a3=a21c*a31c+a22c*a32c+a23c*a33c
      a3a3=a31c*a31c+a32c*a32c+a33c*a33c
! The tangential component is assumed valid:
      a1Dotu1=a11c*u(i1-is1,i2-is2,i3-is3,ex)+a12c*u(i1-is1,i2-is2,i3-is3,ey)+a13c*u(i1-is1,i2-is2,i3-is3,ez)
      a3Dotu1=a31c*u(i1-is1,i2-is2,i3-is3,ex)+a32c*u(i1-is1,i2-is2,i3-is3,ey)+a33c*u(i1-is1,i2-is2,i3-is3,ez)
 ! u(i1-is1,i2-is2,i3-is3,k) = b1[k]*x1 +g1[k]
      b11 =-a11c*(-a1a2*a3a3+a1a3*a2a3)/(-a1a1*a3a3+a1a3**2)+a21c-a31c*(-a1a1*a2a3+a1a3*a1a2)/(-a1a1*a3a3+a1a3**2)
      b12 =-a12c*(-a1a2*a3a3+a1a3*a2a3)/(-a1a1*a3a3+a1a3**2)+a22c-a32c*(-a1a1*a2a3+a1a3*a1a2)/(-a1a1*a3a3+a1a3**2)
      b13 =-a13c*(-a1a2*a3a3+a1a3*a2a3)/(-a1a1*a3a3+a1a3**2)+a23c-a33c*(-a1a1*a2a3+a1a3*a1a2)/(-a1a1*a3a3+a1a3**2)
      g11 =-(-a11c*a1a3*a3Dotu1+a11c*a1Dotu1*a3a3-a31c*a1a3*a1Dotu1+a31c*a1a1*a3Dotu1)/(-a1a1*a3a3+a1a3**2)
      g12 =-(-a12c*a1a3*a3Dotu1+a12c*a1Dotu1*a3a3-a32c*a1a3*a1Dotu1+a32c*a1a1*a3Dotu1)/(-a1a1*a3a3+a1a3**2)
      g13 =(a13c*a1a3*a3Dotu1-a13c*a1Dotu1*a3a3+a33c*a1a3*a1Dotu1-a33c*a1a1*a3Dotu1)/(-a1a1*a3a3+a1a3**2)

! ** decompose point u(i1-2*is1,i2-2*is2,i3-2*is3) into components along a1,a2,a3 **
      a11c=A11D3(i1-2*is1,i2-2*is2,i3-2*is3)
      a12c=A12D3(i1-2*is1,i2-2*is2,i3-2*is3)
      a13c=A13D3(i1-2*is1,i2-2*is2,i3-2*is3)
      a21c=A21D3(i1-2*is1,i2-2*is2,i3-2*is3)
      a22c=A22D3(i1-2*is1,i2-2*is2,i3-2*is3)
      a23c=A23D3(i1-2*is1,i2-2*is2,i3-2*is3)
      a31c=A31D3(i1-2*is1,i2-2*is2,i3-2*is3)
      a32c=A32D3(i1-2*is1,i2-2*is2,i3-2*is3)
      a33c=A33D3(i1-2*is1,i2-2*is2,i3-2*is3)

      a1a1=a11c*a11c+a12c*a12c+a13c*a13c
      a1a2=a11c*a21c+a12c*a22c+a13c*a23c
      a1a3=a11c*a31c+a12c*a32c+a13c*a33c
      a2a3=a21c*a31c+a22c*a32c+a23c*a33c
      a3a3=a31c*a31c+a32c*a32c+a33c*a33c
! The tangential component is assumed valid:
      a1Dotu2=a11c*u(i1-2*is1,i2-2*is2,i3-2*is3,ex)+a12c*u(i1-2*is1,i2-2*is2,i3-2*is3,ey)+a13c*u(i1-2*is1,i2-2*is2,i3-2*is3,ez)
      a3Dotu2=a31c*u(i1-2*is1,i2-2*is2,i3-2*is3,ex)+a32c*u(i1-2*is1,i2-2*is2,i3-2*is3,ey)+a33c*u(i1-2*is1,i2-2*is2,i3-2*is3,ez)
 ! u(i1-2*is1,i2-2*is2,i3-2*is3,k) = b2[k]*x2 +g2[k]
      b21 =-a11c*(-a1a2*a3a3+a1a3*a2a3)/(-a1a1*a3a3+a1a3**2)+a21c-a31c*(-a1a1*a2a3+a1a3*a1a2)/(-a1a1*a3a3+a1a3**2)
      b22 =-a12c*(-a1a2*a3a3+a1a3*a2a3)/(-a1a1*a3a3+a1a3**2)+a22c-a32c*(-a1a1*a2a3+a1a3*a1a2)/(-a1a1*a3a3+a1a3**2)
      b23 =-a13c*(-a1a2*a3a3+a1a3*a2a3)/(-a1a1*a3a3+a1a3**2)+a23c-a33c*(-a1a1*a2a3+a1a3*a1a2)/(-a1a1*a3a3+a1a3**2)
      g21 =(a11c*a1a3*a3Dotu2-a11c*a1Dotu2*a3a3+a31c*a1a3*a1Dotu2-a31c*a1a1*a3Dotu2)/(-a1a1*a3a3+a1a3**2)
      g22 =(a12c*a1a3*a3Dotu2-a12c*a1Dotu2*a3a3+a32c*a1a3*a1Dotu2-a32c*a1a1*a3Dotu2)/(-a1a1*a3a3+a1a3**2)
      g23 =(a13c*a1a3*a3Dotu2-a13c*a1Dotu2*a3a3+a33c*a1a3*a1Dotu2-a33c*a1a1*a3Dotu2)/(-a1a1*a3a3+a1a3**2)

! ** decompose point u(i1-js1,i2-js2,i3-js3) into components along a1,a2,a3 **
      a11c=A11D3(i1-js1,i2-js2,i3-js3)
      a12c=A12D3(i1-js1,i2-js2,i3-js3)
      a13c=A13D3(i1-js1,i2-js2,i3-js3)
      a21c=A21D3(i1-js1,i2-js2,i3-js3)
      a22c=A22D3(i1-js1,i2-js2,i3-js3)
      a23c=A23D3(i1-js1,i2-js2,i3-js3)
      a31c=A31D3(i1-js1,i2-js2,i3-js3)
      a32c=A32D3(i1-js1,i2-js2,i3-js3)
      a33c=A33D3(i1-js1,i2-js2,i3-js3)

      a2a2=a21c*a21c+a22c*a22c+a23c*a23c
      a1a2=a11c*a21c+a12c*a22c+a13c*a23c
      a1a3=a11c*a31c+a12c*a32c+a13c*a33c
      a2a3=a21c*a31c+a22c*a32c+a23c*a33c
      a3a3=a31c*a31c+a32c*a32c+a33c*a33c
! The tangential component is assumed valid:
      a2Dotu3=a21c*u(i1-js1,i2-js2,i3-js3,ex)+a22c*u(i1-js1,i2-js2,i3-js3,ey)+a23c*u(i1-js1,i2-js2,i3-js3,ez)
      a3Dotu3=a31c*u(i1-js1,i2-js2,i3-js3,ex)+a32c*u(i1-js1,i2-js2,i3-js3,ey)+a33c*u(i1-js1,i2-js2,i3-js3,ez)
 ! u(i1-js1,i2-js2,i3-js3,k) = b3[k]*x3 +g3[k]
      b31 =a11c-a21c/(a2a3**2-a3a3*a2a2)*(-a1a2*a3a3+a1a3*a2a3)+a31c*(-a1a2*a2a3+a2a2*a1a3)/(a2a3**2-a3a3*a2a2)
      b32 =a12c-a22c/(a2a3**2-a3a3*a2a2)*(-a1a2*a3a3+a1a3*a2a3)+a32c*(-a1a2*a2a3+a2a2*a1a3)/(a2a3**2-a3a3*a2a2)
      b33 =a13c-a23c/(a2a3**2-a3a3*a2a2)*(-a1a2*a3a3+a1a3*a2a3)+a33c*(-a1a2*a2a3+a2a2*a1a3)/(a2a3**2-a3a3*a2a2)
      g31 =(-a21c*a3a3*a2Dotu3+a21c*a2a3*a3Dotu3-a31c*a2a2*a3Dotu3+a31c*a2Dotu3*a2a3)/(a2a3**2-a3a3*a2a2)
      g32 =(-a22c*a3a3*a2Dotu3+a22c*a2a3*a3Dotu3-a32c*a2a2*a3Dotu3+a32c*a2Dotu3*a2a3)/(a2a3**2-a3a3*a2a2)
      g33 =(-a23c*a3a3*a2Dotu3+a23c*a2a3*a3Dotu3-a33c*a2a2*a3Dotu3+a33c*a2Dotu3*a2a3)/(a2a3**2-a3a3*a2a2)

! ** decompose point u(i1-2*js1,i2-2*js2,i3-2*js3) into components along a1,a2,a3 **
      a11c=A11D3(i1-2*js1,i2-2*js2,i3-2*js3)
      a12c=A12D3(i1-2*js1,i2-2*js2,i3-2*js3)
      a13c=A13D3(i1-2*js1,i2-2*js2,i3-2*js3)
      a21c=A21D3(i1-2*js1,i2-2*js2,i3-2*js3)
      a22c=A22D3(i1-2*js1,i2-2*js2,i3-2*js3)
      a23c=A23D3(i1-2*js1,i2-2*js2,i3-2*js3)
      a31c=A31D3(i1-2*js1,i2-2*js2,i3-2*js3)
      a32c=A32D3(i1-2*js1,i2-2*js2,i3-2*js3)
      a33c=A33D3(i1-2*js1,i2-2*js2,i3-2*js3)

      a2a2=a21c*a21c+a22c*a22c+a23c*a23c
      a1a2=a11c*a21c+a12c*a22c+a13c*a23c
      a1a3=a11c*a31c+a12c*a32c+a13c*a33c
      a2a3=a21c*a31c+a22c*a32c+a23c*a33c
      a3a3=a31c*a31c+a32c*a32c+a33c*a33c
! The tangential component is assumed valid:
      a2Dotu4=a21c*u(i1-2*js1,i2-2*js2,i3-2*js3,ex)+a22c*u(i1-2*js1,i2-2*js2,i3-2*js3,ey)+a23c*u(i1-2*js1,i2-2*js2,i3-2*js3,ez)
      a3Dotu4=a31c*u(i1-2*js1,i2-2*js2,i3-2*js3,ex)+a32c*u(i1-2*js1,i2-2*js2,i3-2*js3,ey)+a33c*u(i1-2*js1,i2-2*js2,i3-2*js3,ez)
 ! u(i1-2*js1,i2-2*js2,i3-2*js3,k) = b4[k]*x4 +g4[k]
      b41 =a11c-a21c/(a2a3**2-a3a3*a2a2)*(-a1a2*a3a3+a1a3*a2a3)+a31c*(-a1a2*a2a3+a2a2*a1a3)/(a2a3**2-a3a3*a2a2)
      b42 =a12c-a22c/(a2a3**2-a3a3*a2a2)*(-a1a2*a3a3+a1a3*a2a3)+a32c*(-a1a2*a2a3+a2a2*a1a3)/(a2a3**2-a3a3*a2a2)
      b43 =a13c-a23c/(a2a3**2-a3a3*a2a2)*(-a1a2*a3a3+a1a3*a2a3)+a33c*(-a1a2*a2a3+a2a2*a1a3)/(a2a3**2-a3a3*a2a2)
      g41 =-(a21c*a3a3*a2Dotu4-a21c*a3Dotu4*a2a3+a31c*a2a2*a3Dotu4-a31c*a2a3*a2Dotu4)/(a2a3**2-a3a3*a2a2)
      g42 =-(a22c*a3a3*a2Dotu4-a22c*a3Dotu4*a2a3+a32c*a2a2*a3Dotu4-a32c*a2a3*a2Dotu4)/(a2a3**2-a3a3*a2a2)
      g43 =(-a23c*a3a3*a2Dotu4+a23c*a3Dotu4*a2a3-a33c*a2a2*a3Dotu4+a33c*a2a3*a2Dotu4)/(a2a3**2-a3a3*a2a2)

 ! Evaluate a1, a2 and a3 at the corner
      a11=A11D3(i1,i2,i3)
      a12=A12D3(i1,i2,i3)
      a13=A13D3(i1,i2,i3)
      a21=A21D3(i1,i2,i3)
      a22=A22D3(i1,i2,i3)
      a23=A23D3(i1,i2,i3)
      a31=A31D3(i1,i2,i3)
      a32=A32D3(i1,i2,i3)
      a33=A33D3(i1,i2,i3)

      a1DotLu=a11*DeltaU+a12*DeltaV+a13*DeltaW
      a2DotLu=a21*DeltaU+a22*DeltaV+a23*DeltaW

!   a1.Lu = 0 
! e1 := cc11a*u(i1-2,i2,i3)+cc12a*v(i1-2,i2,i3)+cc13a*w(i1-2,i2,i3)
!     + cc14a*u(i1-1,i2,i3)+cc15a*v(i1-1,i2,i3)+cc16a*w(i1-1,i2,i3) 
!     + cc11b*u(i1,i2-2,i3)+cc12b*v(i1,i2-2,i3)+cc13b*w(i1,i2-2,i3)
!     + cc14b*u(i1,i2-1,i3)+cc15b*v(i1,i2-1,i3)+cc16b*w(i1,i2-1,i3) - f1:
!  a2.Lu = 0 :
! e2 := cc21a*u(i1-2,i2,i3)+cc22a*v(i1-2,i2,i3)+cc23a*w(i1-2,i2,i3)
!     + cc24a*u(i1-1,i2,i3)+cc25a*v(i1-1,i2,i3)+cc26a*w(i1-1,i2,i3) 
!     + cc21b*u(i1,i2-2,i3)+cc22b*v(i1,i2-2,i3)+cc23b*w(i1,i2-2,i3) 
!     + cc24b*u(i1,i2-1,i3)+cc25b*v(i1,i2-1,i3)+cc26b*w(i1,i2-1,i3) - f2:
      cc11a=(-1/12.*c11/dra**2+1/12.*c1/dra)*a11
      cc12a=(-1/12.*c11/dra**2+1/12.*c1/dra)*a12
      cc13a=(-1/12.*c11/dra**2+1/12.*c1/dra)*a13
      cc14a=(4/3.*c11/dra**2-2/3.*c1/dra)*a11
      cc15a=(4/3.*c11/dra**2-2/3.*c1/dra)*a12
      cc16a=(4/3.*c11/dra**2-2/3.*c1/dra)*a13
      cc11b=(-1/12.*c22/dsa**2+1/12.*c2/dsa)*a11
      cc12b=(-1/12.*c22/dsa**2+1/12.*c2/dsa)*a12
      cc13b=(-1/12.*c22/dsa**2+1/12.*c2/dsa)*a13
      cc14b=(4/3.*c22/dsa**2-2/3.*c2/dsa)*a11
      cc15b=(4/3.*c22/dsa**2-2/3.*c2/dsa)*a12
      cc16b=(4/3.*c22/dsa**2-2/3.*c2/dsa)*a13
      cc21a=(-1/12.*c11/dra**2+1/12.*c1/dra)*a21
      cc22a=(-1/12.*c11/dra**2+1/12.*c1/dra)*a22
      cc23a=(-1/12.*c11/dra**2+1/12.*c1/dra)*a23
      cc24a=(4/3.*c11/dra**2-2/3.*c1/dra)*a21
      cc25a=(4/3.*c11/dra**2-2/3.*c1/dra)*a22
      cc26a=(4/3.*c11/dra**2-2/3.*c1/dra)*a23
      cc21b=(-1/12.*c22/dsa**2+1/12.*c2/dsa)*a21
      cc22b=(-1/12.*c22/dsa**2+1/12.*c2/dsa)*a22
      cc23b=(-1/12.*c22/dsa**2+1/12.*c2/dsa)*a23
      cc24b=(4/3.*c22/dsa**2-2/3.*c2/dsa)*a21
      cc25b=(4/3.*c22/dsa**2-2/3.*c2/dsa)*a22
      cc26b=(4/3.*c22/dsa**2-2/3.*c2/dsa)*a23

      f1=a1DotLu-cc11a*u(i1-2*is1,i2-2*is2,i3-2*is3,ex)-cc12a*u(i1-2*is1,i2-2*is2,i3-2*is3,ey)-cc13a*u(i1-2*is1,i2-2*is2,i3-2*is3,ez)-cc14a*u(i1-is1,i2-is2,i3-is3,ex)-cc15a*u(i1-is1,i2-is2,i3-is3,ey)-cc16a*u(i1-is1,i2-is2,i3-is3,ez)-cc11b*u(i1-2*js1,i2-2*js2,i3-2*js3,ex)-cc12b*u(i1-2*js1,i2-2*js2,i3-2*js3,ey)-cc13b*u(i1-2*js1,i2-2*js2,i3-2*js3,ez)-cc14b*u(i1-js1,i2-js2,i3-js3,ex)-cc15b*u(i1-js1,i2-js2,i3-js3,ey)-cc16b*u(i1-js1,i2-js2,i3-js3,ez)
      f2=a2DotLu-cc21a*u(i1-2*is1,i2-2*is2,i3-2*is3,ex)-cc22a*u(i1-2*is1,i2-2*is2,i3-2*is3,ey)-cc23a*u(i1-2*is1,i2-2*is2,i3-2*is3,ez)-cc24a*u(i1-is1,i2-is2,i3-is3,ex)-cc25a*u(i1-is1,i2-is2,i3-is3,ey)-cc26a*u(i1-is1,i2-is2,i3-is3,ez)-cc21b*u(i1-2*js1,i2-2*js2,i3-2*js3,ex)-cc22b*u(i1-2*js1,i2-2*js2,i3-2*js3,ey)-cc23b*u(i1-2*js1,i2-2*js2,i3-2*js3,ez)-cc24b*u(i1-js1,i2-js2,i3-js3,ex)-cc25b*u(i1-js1,i2-js2,i3-js3,ey)-cc26b*u(i1-js1,i2-js2,i3-js3,ez)
      f3=6*a21*u(i1,i2,i3,ex)+6*a22*u(i1,i2,i3,ey)+6*a23*u(i1,i2,i3,ez)-4*a21*u(i1+is1,i2+is2,i3+is3,ex)-4*a22*u(i1+is1,i2+is2,i3+is3,ey)-4*a23*u(i1+is1,i2+is2,i3+is3,ez)+a21*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)+a22*u(i1+2*is1,i2+2*is2,i3+2*is3,ey)+a23*u(i1+2*is1,i2+2*is2,i3+2*is3,ez)-g2f
      f4=6*a11*u(i1,i2,i3,ex)+6*a12*u(i1,i2,i3,ey)+6*a13*u(i1,i2,i3,ez)-4*a11*u(i1+js1,i2+js2,i3+js3,ex)-4*a12*u(i1+js1,i2+js2,i3+js3,ey)-4*a13*u(i1+js1,i2+js2,i3+js3,ez)+a11*u(i1+2*js1,i2+2*js2,i3+2*js3,ex)+a12*u(i1+2*js1,i2+2*js2,i3+2*js3,ey)+a13*u(i1+2*js1,i2+2*js2,i3+2*js3,ez)-g1f

 ! Simplfied forms for the 4 equations a1.Lu, a2.Lu, a2.D+r4 u = g2f  a1.D+s4 u = g1f
 ! e1x := dd11*x1+dd12*x2+dd13*x3+dd14*x4+ f1x
 ! e2x := dd21*x1+dd22*x2+dd23*x3+dd24*x4+ f2x
 ! e3x := dd31*x1+dd32*x2+dd33*x3+dd34*x4+ f3x
 ! e4x := dd41*x1+dd42*x2+dd43*x3+dd44*x4+ f4x
      dd11=cc14a*b11+cc15a*b12+cc16a*b13
      dd12=cc11a*b21+cc12a*b22+cc13a*b23
      dd13=cc14b*b31+cc15b*b32+cc16b*b33
      dd14=cc11b*b41+cc12b*b42+cc13b*b43
      dd21=cc24a*b11+cc25a*b12+cc26a*b13
      dd22=cc21a*b21+cc22a*b22+cc23a*b23
      dd23=cc24b*b31+cc25b*b32+cc26b*b33
      dd24=cc21b*b41+cc22b*b42+cc23b*b43
      dd31=-4*a21*b11-4*a22*b12-4*a23*b13
      dd32=a21*b21+a22*b22+a23*b23
      dd33=0
      dd34=0
      dd41=0
      dd42=0
      dd43=-4*a11*b31-4*a12*b32-4*a13*b33
      dd44=a11*b41+a12*b42+a13*b43

      f1x=cc11a*g21+cc12a*g22+cc13a*g23+cc14a*g11+cc15a*g12+cc16a*g13+cc11b*g41+cc12b*g42+cc13b*g43+cc14b*g31+cc15b*g32+cc16b*g33+f1
      f2x=cc21a*g21+cc22a*g22+cc23a*g23+cc24a*g11+cc25a*g12+cc26a*g13+cc21b*g41+cc22b*g42+cc23b*g43+cc24b*g31+cc25b*g32+cc26b*g33+f2
      f3x=a21*g21+a22*g22+a23*g23-4*a21*g11-4*a22*g12-4*a23*g13+f3
      f4x=a11*g41+a12*g42+a13*g43-4*a11*g31-4*a12*g32-4*a13*g33+f4

!  solution x1,x2,x3,x4: 
      det=-dd32*dd43*dd14*dd21-dd32*dd11*dd23*dd44+dd32*dd11*dd43*dd24+dd43*dd14*dd22*dd31-dd13*dd44*dd22*dd31+dd12*dd31*dd23*dd44+dd32*dd13*dd44*dd21-dd12*dd31*dd43*dd24
      x1=(-dd32*f2x*dd13*dd44-dd32*dd43*dd24*f1x+dd32*dd23*dd44*f1x+dd32*dd43*f2x*dd14-dd32*dd23*f4x*dd14+dd32*dd24*dd13*f4x-dd23*dd44*dd12*f3x+dd22*f3x*dd13*dd44-dd43*dd22*f3x*dd14+dd43*dd24*dd12*f3x)/det
      x2=(dd31*f2x*dd13*dd44+dd31*dd43*dd24*f1x-dd31*dd23*dd44*f1x-dd31*dd43*f2x*dd14+dd31*dd23*f4x*dd14-dd31*dd24*dd13*f4x+f3x*dd43*dd14*dd21+f3x*dd11*dd23*dd44-f3x*dd11*dd43*dd24-f3x*dd13*dd44*dd21)/det
      x3=(dd44*dd32*dd11*f2x-dd44*dd12*dd31*f2x+dd44*dd12*f3x*dd21-dd44*dd32*f1x*dd21-dd44*dd11*dd22*f3x+dd44*f1x*dd22*dd31+f4x*dd32*dd14*dd21-f4x*dd32*dd11*dd24-f4x*dd14*dd22*dd31+f4x*dd12*dd31*dd24)/det
      x4=(-dd32*dd13*f4x*dd21-dd32*dd11*dd43*f2x+dd12*dd31*dd43*f2x-dd12*dd31*dd23*f4x-dd43*dd12*f3x*dd21+dd32*dd43*f1x*dd21+dd32*dd11*dd23*f4x+dd13*f4x*dd22*dd31+dd11*dd43*dd22*f3x-dd43*f1x*dd22*dd31)/det

! **** Now assign the extended boundary points **** 
      u(i1-  is1,i2-  is2,i3-  is3,ex) = b11*x1+ g11
      u(i1-  is1,i2-  is2,i3-  is3,ey) = b12*x1+ g12
      u(i1-  is1,i2-  is2,i3-  is3,ez) = b13*x1+ g13
      u(i1-2*is1,i2-2*is2,i3-2*is3,ex) = b21*x2+ g21
      u(i1-2*is1,i2-2*is2,i3-2*is3,ey) = b22*x2+ g22
      u(i1-2*is1,i2-2*is2,i3-2*is3,ez) = b23*x2+ g23
      u(i1-  js1,i2-  js2,i3-  js3,ex) = b31*x3+ g31
      u(i1-  js1,i2-  js2,i3-  js3,ey) = b32*x3+ g32
      u(i1-  js1,i2-  js2,i3-  js3,ez) = b33*x3+ g33
      u(i1-2*js1,i2-2*js2,i3-2*js3,ex) = b41*x4+ g41
      u(i1-2*js1,i2-2*js2,i3-2*js3,ey) = b42*x4+ g42
      u(i1-2*js1,i2-2*js2,i3-2*js3,ez) = b43*x4+ g43
