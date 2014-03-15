

#beginMacro divScalarGradConservativeFourthOrder(operatorName)
subroutine operatorName( nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b, \
  ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b, n1a,n1b,n2a,n2b,n3a,n3b, ca,cb, \
  dr,dx,rsxy,u,s,deriv,derivOption,gridType,order,averagingType,dir1,dir2 )
c ===============================================================
c    Fourth-order Conservative form of the operators:
c  
c           Laplace
c           div( s grad )
c           div( tensor Grad )
c 
c
c  Evaluate:
c     deriv(n1a:n1b,n2a:n2b,n3a:n3b,ca:cb) = Operator()
c
c nd (input) : number of space dimensions
c nd1a,nd1b,nd2a,nd2b,nd3a,nd3b (input) : array dimensions for rsxy
c ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b (input) : array dimensions for u and s
c ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b (input) : array dimensions for deriv
c n1a,n1b,n2a,n2b,n3a,n3b, ca,cb (input): evaluate the operator at these points and components
c dr(1:3),dx(1:3)  (input): dr(axis) = grid spacing on unit square. dx(axis) = grid spacing for Cartesian grids
c rsxy(i1,i2,i3,1:nd,1:nd) (input) : Jacobian derivatives, e.g. rsxy(i1,i2,i3,1,2) = ry (not used if rectangular)
c u(i1,i2,i3,c) (input) : solution to apply the operator to
c s(i1,i2,i3) (input) : scalar coefficient (ignore for Laplace operator)
c deriv(i1,i2,i3,c) (output) : result is returned here
c derivOption : ignored
c gridType: 0=rectangular, 1=non-rectangular
c order : ignored
c averagingType : ignored
c dir1,dir2 : ingnored
c
c NOTES:
c   o This function assumes that there are enough ghost point values to evaluate the operator at all
c      the requested points. It is assumed that some other functions fill in the ghost point values
c      at the boundaries.
c Who to blame:
c    Bill Henshaw
c ===============================================================

implicit none
integer nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,\
  ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b,n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,\
  derivOption, gridType, order, averagingType, dir1, dir2

integer laplace,divScalarGrad,derivativeScalarDerivative,divTensorGrad
parameter(laplace=0,divScalarGrad=1,derivativeScalarDerivative=2,divTensorGrad=3)

real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,1:nd,1:nd)
real u(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,ndu4a:ndu4b)
real s(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,0:*)
real deriv(ndd1a:ndd1b,ndd2a:ndd2b,ndd3a:ndd3b,ndd4a:ndd4b)
real dr(*),dx(*)

c.....local variables
integer i1,i2,i3,kd,c
real drsqi(3),jac,dxsqi(3)

integer rectangular,curvilinear
parameter( rectangular=0,curvilinear=1 )
c.......statement functions 
real rx,ry,rz,sx,sy,sz,tx,ty,tz
real dp1u,dp2u,dz1u,dz2u,dp1Cubedu,dp2Cubedu,dzdpdm1u,dzdpdm2u
real a11,a12,a22,a21, a11r,a12r,a22r,a21r
real a11i,a11m1,a11p1,a11m2,a11p2,a11mh,a11ph,a11p3h,a11m3h,a11p3,a11m3,a11ph2,a11mh2
real a22i,a22m1,a22p1,a22m2,a22p2,a22mh,a22ph,a22p3h,a22m3h,a22p3,a22m3,a22ph2,a22mh2
real ajac

real a33i,a33m1,a33p1,a33m2,a33p2,a33mh,a33ph,a33p3h,a33m3h,a33p3,a33m3,a33ph2,a33mh2
real ajac3d,a113d,a223d,a333d,a123d,a133d,a233d,a213d,a313d,a323d
real a113dr,a223dr,a333dr,a123dr,a133dr,a233dr,a213dr,a313dr,a323dr
real dp3u,dz3u,dp3Cubedu,dzdpdm3u

c.......statement functions for jacobian
rx(i1,i2,i3)=rsxy(i1,i2,i3,1,1)
ry(i1,i2,i3)=rsxy(i1,i2,i3,1,2)
rz(i1,i2,i3)=rsxy(i1,i2,i3,1,3)
sx(i1,i2,i3)=rsxy(i1,i2,i3,2,1)
sy(i1,i2,i3)=rsxy(i1,i2,i3,2,2)
sz(i1,i2,i3)=rsxy(i1,i2,i3,2,3)
tx(i1,i2,i3)=rsxy(i1,i2,i3,3,1)
ty(i1,i2,i3)=rsxy(i1,i2,i3,3,2)
tz(i1,i2,i3)=rsxy(i1,i2,i3,3,3)

dp1u(i1,i2,i3,c)= u(i1+1,i2,i3,c)-u(i1,i2,i3,c) 
dp2u(i1,i2,i3,c)= u(i1,i2+1,i3,c)-u(i1,i2,i3,c) 
dp3u(i1,i2,i3,c)= u(i1,i2,i3+1,c)-u(i1,i2,i3,c) 
dz1u(i1,i2,i3,c)= u(i1+1,i2,i3,c)-u(i1-1,i2,i3,c) 
dz2u(i1,i2,i3,c)= u(i1,i2+1,i3,c)-u(i1,i2-1,i3,c) 
dz3u(i1,i2,i3,c)= u(i1,i2,i3+1,c)-u(i1,i2,i3-1,c) 

dp1Cubedu(i1,i2,i3,c)= u(i1+3,i2,i3,c)-3.*(u(i1+2,i2,i3,c)-u(i1+1,i2,i3,c))-u(i1,i2,i3,c)
dp2Cubedu(i1,i2,i3,c)= u(i1,i2+3,i3,c)-3.*(u(i1,i2+2,i3,c)-u(i1,i2+1,i3,c))-u(i1,i2,i3,c)
dp3Cubedu(i1,i2,i3,c)= u(i1,i2,i3+3,c)-3.*(u(i1,i2,i3+2,c)-u(i1,i2,i3+1,c))-u(i1,i2,i3,c)

dzdpdm1u(i1,i2,i3,c)= u(i1+2,i2,i3,c)-2.*(u(i1+1,i2,i3,c)-u(i1-1,i2,i3,c))-u(i1-2,i2,i3,c)
dzdpdm2u(i1,i2,i3,c)= u(i1,i2+2,i3,c)-2.*(u(i1,i2+1,i3,c)-u(i1,i2-1,i3,c))-u(i1,i2-2,i3,c)
dzdpdm3u(i1,i2,i3,c)= u(i1,i2,i3+2,c)-2.*(u(i1,i2,i3+1,c)-u(i1,i2,i3-1,c))-u(i1,i2,i3-2,c)

ajac(i1,i2,i3)=rx(i1,i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)

#If #operatorName == "laplace4Cons"

a11(i1,i2,i3)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)/ajac(i1,i2,i3)
a22(i1,i2,i3)=(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2)/ajac(i1,i2,i3)
a12(i1,i2,i3)=(rx(i1,i2,i3)*sx(i1,i2,i3)+ry(i1,i2,i3)*sy(i1,i2,i3))/ajac(i1,i2,i3)

#Elif #operatorName == "divScalarGrad4Cons"

a11(i1,i2,i3)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*s(i1,i2,i3,0)/ajac(i1,i2,i3)
a22(i1,i2,i3)=(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2)*s(i1,i2,i3,0)/ajac(i1,i2,i3)
a12(i1,i2,i3)=(rx(i1,i2,i3)*sx(i1,i2,i3)+ry(i1,i2,i3)*sy(i1,i2,i3))*s(i1,i2,i3,0)/ajac(i1,i2,i3)


#Else
! divTensorGrad
a11(i1,i2,i3) = (s(i1,i2,i3,0)*rx(i1,i2,i3)**2+\
                (s(i1,i2,i3,1)+\
                 s(i1,i2,i3,2))*rx(i1,i2,i3)*ry(i1,i2,i3)+\
                 s(i1,i2,i3,3)*ry(i1,i2,i3)**2)/ajac(i1,i2,i3)
a12(i1,i2,i3) = (s(i1,i2,i3,0)*rx(i1,i2,i3)*sx(i1,i2,i3)+\
                 s(i1,i2,i3,1)*ry(i1,i2,i3)*sx(i1,i2,i3)+\
                 s(i1,i2,i3,2)*rx(i1,i2,i3)*sy(i1,i2,i3)+\
                 s(i1,i2,i3,3)*ry(i1,i2,i3)*sy(i1,i2,i3))/ajac(i1,i2,i3) 
a22(i1,i2,i3) = (s(i1,i2,i3,0)*sx(i1,i2,i3)**2+\
                (s(i1,i2,i3,1)+\
                 s(i1,i2,i3,2))*sx(i1,i2,i3)*sy(i1,i2,i3)+\
                 s(i1,i2,i3,3)*sy(i1,i2,i3)**2)/ajac(i1,i2,i3) 
a21(i1,i2,i3) = (s(i1,i2,i3,0)*sx(i1,i2,i3)*rx(i1,i2,i3)+\
                 s(i1,i2,i3,1)*sy(i1,i2,i3)*rx(i1,i2,i3)+\
                 s(i1,i2,i3,2)*sx(i1,i2,i3)*ry(i1,i2,i3)+\
                 s(i1,i2,i3,3)*sy(i1,i2,i3)*ry(i1,i2,i3))/ajac(i1,i2,i3)

! rectangular grid versions
a11r(i1,i2,i3) = s(i1,i2,i3,0)
a21r(i1,i2,i3) = s(i1,i2,i3,1)
a12r(i1,i2,i3) = s(i1,i2,i3,2)
a22r(i1,i2,i3) = s(i1,i2,i3,3)


#End

ajac3d(i1,i2,i3)=(rx(i1,i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3))*tz(i1,i2,i3)+\
                 (ry(i1,i2,i3)*sz(i1,i2,i3)-rz(i1,i2,i3)*sy(i1,i2,i3))*tx(i1,i2,i3)+\
                 (rz(i1,i2,i3)*sx(i1,i2,i3)-rx(i1,i2,i3)*sz(i1,i2,i3))*ty(i1,i2,i3)

#If #operatorName == "laplace4Cons"

a113d(i1,i2,i3)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(i1,i2,i3)**2)/ajac3d(i1,i2,i3)
a223d(i1,i2,i3)=(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2+sz(i1,i2,i3)**2)/ajac3d(i1,i2,i3)
a333d(i1,i2,i3)=(tx(i1,i2,i3)**2+ty(i1,i2,i3)**2+tz(i1,i2,i3)**2)/ajac3d(i1,i2,i3)
a123d(i1,i2,i3)=(rx(i1,i2,i3)*sx(i1,i2,i3)+ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,i3))/ajac3d(i1,i2,i3)
a133d(i1,i2,i3)=(rx(i1,i2,i3)*tx(i1,i2,i3)+ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))/ajac3d(i1,i2,i3)
a233d(i1,i2,i3)=(sx(i1,i2,i3)*tx(i1,i2,i3)+sy(i1,i2,i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3))/ajac3d(i1,i2,i3)

#Elif #operatorName == "divScalarGrad4Cons"

a113d(i1,i2,i3)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(i1,i2,i3)**2)*s(i1,i2,i3,0)/ajac3d(i1,i2,i3)
a223d(i1,i2,i3)=(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2+sz(i1,i2,i3)**2)*s(i1,i2,i3,0)/ajac3d(i1,i2,i3)
a333d(i1,i2,i3)=(tx(i1,i2,i3)**2+ty(i1,i2,i3)**2+tz(i1,i2,i3)**2)*s(i1,i2,i3,0)/ajac3d(i1,i2,i3)
a123d(i1,i2,i3)=(rx(i1,i2,i3)*sx(i1,i2,i3)+ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,i3))\
                                                                 *s(i1,i2,i3,0)/ajac3d(i1,i2,i3)
a133d(i1,i2,i3)=(rx(i1,i2,i3)*tx(i1,i2,i3)+ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))\
                                                                 *s(i1,i2,i3,0)/ajac3d(i1,i2,i3)
a233d(i1,i2,i3)=(sx(i1,i2,i3)*tx(i1,i2,i3)+sy(i1,i2,i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3))\
                                                                 *s(i1,i2,i3,0)/ajac3d(i1,i2,i3)

#Else

! divTensorGrad
a113d(i1,i2,i3)=(s(i1,i2,i3,0)*rx(i1,i2,i3)**2+\
                 s(i1,i2,i3,4)*ry(i1,i2,i3)**2+\
                 s(i1,i2,i3,8)*rz(i1,i2,i3)**2+\
                 (s(i1,i2,i3,3)+s(i1,i2,i3,1))*rx(i1,i2,i3)*ry(i1,i2,i3)+\
                 (s(i1,i2,i3,6)+s(i1,i2,i3,2))*rx(i1,i2,i3)*rz(i1,i2,i3)+\
                 (s(i1,i2,i3,7)+s(i1,i2,i3,5))*ry(i1,i2,i3)*rz(i1,i2,i3))/ajac3d(i1,i2,i3)
a223d(i1,i2,i3)=(s(i1,i2,i3,0)*sx(i1,i2,i3)**2+\
                 s(i1,i2,i3,4)*sy(i1,i2,i3)**2+\
                 s(i1,i2,i3,8)*sz(i1,i2,i3)**2+\
                 (s(i1,i2,i3,3)+s(i1,i2,i3,1))*sx(i1,i2,i3)*sy(i1,i2,i3)+\
                 (s(i1,i2,i3,6)+s(i1,i2,i3,2))*sx(i1,i2,i3)*sz(i1,i2,i3)+\
                 (s(i1,i2,i3,7)+s(i1,i2,i3,5))*sy(i1,i2,i3)*sz(i1,i2,i3))/ajac3d(i1,i2,i3)
a333d(i1,i2,i3)=(s(i1,i2,i3,0)*tx(i1,i2,i3)**2+\
                 s(i1,i2,i3,4)*ty(i1,i2,i3)**2+\
                 s(i1,i2,i3,8)*tz(i1,i2,i3)**2+\
                 (s(i1,i2,i3,3)+s(i1,i2,i3,1))*tx(i1,i2,i3)*ty(i1,i2,i3)+\
                 (s(i1,i2,i3,6)+s(i1,i2,i3,2))*tx(i1,i2,i3)*tz(i1,i2,i3)+\
                 (s(i1,i2,i3,7)+s(i1,i2,i3,5))*ty(i1,i2,i3)*tz(i1,i2,i3))/ajac3d(i1,i2,i3)
a123d(i1,i2,i3)=(s(i1,i2,i3,0)*rx(i1,i2,i3)*sx(i1,i2,i3)+\
                 s(i1,i2,i3,4)*ry(i1,i2,i3)*sy(i1,i2,i3)+\
                 s(i1,i2,i3,8)*rz(i1,i2,i3)*sz(i1,i2,i3)+\
                 s(i1,i2,i3,3)*rx(i1,i2,i3)*sy(i1,i2,i3)+\
                 s(i1,i2,i3,6)*rx(i1,i2,i3)*sz(i1,i2,i3)+\
                 s(i1,i2,i3,1)*ry(i1,i2,i3)*sx(i1,i2,i3)+\
                 s(i1,i2,i3,7)*ry(i1,i2,i3)*sz(i1,i2,i3)+\
                 s(i1,i2,i3,2)*rz(i1,i2,i3)*sx(i1,i2,i3)+\
                 s(i1,i2,i3,5)*rz(i1,i2,i3)*sy(i1,i2,i3))/ajac3d(i1,i2,i3)
a133d(i1,i2,i3)=(s(i1,i2,i3,0)*rx(i1,i2,i3)*tx(i1,i2,i3)+\
                 s(i1,i2,i3,4)*ry(i1,i2,i3)*ty(i1,i2,i3)+\
                 s(i1,i2,i3,8)*rz(i1,i2,i3)*tz(i1,i2,i3)+\
                 s(i1,i2,i3,3)*rx(i1,i2,i3)*ty(i1,i2,i3)+\
                 s(i1,i2,i3,6)*rx(i1,i2,i3)*tz(i1,i2,i3)+\
                 s(i1,i2,i3,1)*ry(i1,i2,i3)*tx(i1,i2,i3)+\
                 s(i1,i2,i3,7)*ry(i1,i2,i3)*tz(i1,i2,i3)+\
                 s(i1,i2,i3,2)*rz(i1,i2,i3)*tx(i1,i2,i3)+\
                 s(i1,i2,i3,5)*rz(i1,i2,i3)*ty(i1,i2,i3))/ajac3d(i1,i2,i3) 
a233d(i1,i2,i3)=(s(i1,i2,i3,0)*sx(i1,i2,i3)*tx(i1,i2,i3)+\
                 s(i1,i2,i3,4)*sy(i1,i2,i3)*ty(i1,i2,i3)+\
                 s(i1,i2,i3,8)*sz(i1,i2,i3)*tz(i1,i2,i3)+\
                 s(i1,i2,i3,3)*sx(i1,i2,i3)*ty(i1,i2,i3)+\
                 s(i1,i2,i3,6)*sx(i1,i2,i3)*tz(i1,i2,i3)+\
                 s(i1,i2,i3,1)*sy(i1,i2,i3)*tx(i1,i2,i3)+\
                 s(i1,i2,i3,7)*sy(i1,i2,i3)*tz(i1,i2,i3)+\
                 s(i1,i2,i3,2)*sz(i1,i2,i3)*tx(i1,i2,i3)+\
                 s(i1,i2,i3,5)*sz(i1,i2,i3)*ty(i1,i2,i3))/ajac3d(i1,i2,i3)  
a213d(i1,i2,i3)=(s(i1,i2,i3,0)*sx(i1,i2,i3)*rx(i1,i2,i3)+\
                 s(i1,i2,i3,4)*sy(i1,i2,i3)*ry(i1,i2,i3)+\
                 s(i1,i2,i3,8)*sz(i1,i2,i3)*rz(i1,i2,i3)+\
                 s(i1,i2,i3,3)*sx(i1,i2,i3)*ry(i1,i2,i3)+\
                 s(i1,i2,i3,6)*sx(i1,i2,i3)*rz(i1,i2,i3)+\
                 s(i1,i2,i3,1)*sy(i1,i2,i3)*rx(i1,i2,i3)+\
                 s(i1,i2,i3,7)*sy(i1,i2,i3)*rz(i1,i2,i3)+\
                 s(i1,i2,i3,2)*sz(i1,i2,i3)*rx(i1,i2,i3)+\
                 s(i1,i2,i3,5)*sz(i1,i2,i3)*ry(i1,i2,i3))/ajac3d(i1,i2,i3) 
a313d(i1,i2,i3)=(s(i1,i2,i3,0)*tx(i1,i2,i3)*rx(i1,i2,i3)+\
                 s(i1,i2,i3,4)*ty(i1,i2,i3)*ry(i1,i2,i3)+\
                 s(i1,i2,i3,8)*tz(i1,i2,i3)*rz(i1,i2,i3)+\
                 s(i1,i2,i3,3)*tx(i1,i2,i3)*ry(i1,i2,i3)+\
                 s(i1,i2,i3,6)*tx(i1,i2,i3)*rz(i1,i2,i3)+\
                 s(i1,i2,i3,1)*ty(i1,i2,i3)*rx(i1,i2,i3)+\
                 s(i1,i2,i3,7)*ty(i1,i2,i3)*rz(i1,i2,i3)+\
                 s(i1,i2,i3,2)*tz(i1,i2,i3)*rx(i1,i2,i3)+\
                 s(i1,i2,i3,5)*tz(i1,i2,i3)*ry(i1,i2,i3))/ajac3d(i1,i2,i3) 
a323d(i1,i2,i3)=(s(i1,i2,i3,0)*tx(i1,i2,i3)*sx(i1,i2,i3)+\
                 s(i1,i2,i3,4)*ty(i1,i2,i3)*sy(i1,i2,i3)+\
                 s(i1,i2,i3,8)*tz(i1,i2,i3)*sz(i1,i2,i3)+\
                 s(i1,i2,i3,3)*tx(i1,i2,i3)*sy(i1,i2,i3)+\
                 s(i1,i2,i3,6)*tx(i1,i2,i3)*sz(i1,i2,i3)+\
                 s(i1,i2,i3,1)*ty(i1,i2,i3)*sx(i1,i2,i3)+\
                 s(i1,i2,i3,7)*ty(i1,i2,i3)*sz(i1,i2,i3)+\
                 s(i1,i2,i3,2)*tz(i1,i2,i3)*sx(i1,i2,i3)+\
                 s(i1,i2,i3,5)*tz(i1,i2,i3)*sy(i1,i2,i3))/ajac3d(i1,i2,i3)

! rectangular grid versions
a113dr(i1,i2,i3) = s(i1,i2,i3,0)
a213dr(i1,i2,i3) = s(i1,i2,i3,1)
a313dr(i1,i2,i3) = s(i1,i2,i3,2)
a123dr(i1,i2,i3) = s(i1,i2,i3,3)
a223dr(i1,i2,i3) = s(i1,i2,i3,4)
a323dr(i1,i2,i3) = s(i1,i2,i3,5)
a133dr(i1,i2,i3) = s(i1,i2,i3,6)
a233dr(i1,i2,i3) = s(i1,i2,i3,7)
a333dr(i1,i2,i3) = s(i1,i2,i3,8)



#End
c........end statement functions


if( gridType.eq.curvilinear )then

  ! ******************************************************************
  ! *************Curvilinear Grid ************************************
  ! ******************************************************************

drsqi(1)=1./dr(1)**2
drsqi(2)=1./dr(2)**2
drsqi(3)=1./dr(3)**2


if( nd.eq.2 )then

! write(*,*) '****INSIDE dsgc4 2d'

do c=ca,cb
do i3=n3a,n3b
do i2=n2a,n2b
do i1=n1a,n1b


 a11i  = a11(i1,i2,i3)
 a11m1 = a11(i1-1,i2,i3)
 a11p1 = a11(i1+1,i2,i3)
 a11m2 = a11(i1-2,i2,i3)
 a11p2 = a11(i1+2,i2,i3)
 
 a11ph  = ((a11p1+a11i)*9.-(a11p2+a11m1))/16.
 a11mh  = ((a11i+a11m1)*9.-(a11p1+a11m2))/16.
 a11ph2 = .5*(a11p1+a11i)  ! we must consistently use the 2nd-order version for the higher order terms
 a11mh2 = .5*(a11m1+a11i)
 a11p3h = .5*(a11p2+a11p1)
 a11m3h = .5*(a11m1+a11m2)

! a11p3= 4.*a11p2 -6.*a11p1 +4.*a11i -a11m1
! a11m3= 4.*a11m2 -6.*a11m1 +4.*a11i -a11p1
! a11p3h  = ((a11p2+a11p1)*9.-(a11p3+a11i))/16.
! a11m3h  = ((a11m2+a11m1)*9.-(a11m3+a11i))/16.
 
 a22i  = a22(i1,i2,i3)
 a22m1 = a22(i1,i2-1,i3)
 a22p1 = a22(i1,i2+1,i3)
 a22m2 = a22(i1,i2-2,i3)
 a22p2 = a22(i1,i2+2,i3)

 a22ph  = ((a22p1+a22i)*9.-(a22p2+a22m1))/16.
 a22mh  = ((a22i+a22m1)*9.-(a22p1+a22m2))/16.
 a22ph2 = .5*(a22p1+a22i)
 a22mh2 = .5*(a22m1+a22i)
 a22p3h = .5*(a22p2+a22p1)
 a22m3h = .5*(a22m1+a22m2)

! a22p3= 4.*a22p2 -6.*a22p1 +4.*a22i -a22m1
! a22m3= 4.*a22m2 -6.*a22m1 +4.*a22i -a22p1
! a22p3h  = ((a22p2+a22p1)*9.-(a22p3+a22i))/16.
! a22m3h  = ((a22m2+a22m1)*9.-(a22m3+a22i))/16.
 
 jac= ajac(i1,i2,i3)
 deriv(i1,i2,i3,c)=\
   (\
     ( a11ph*dp1u(i1,i2,i3,c)-a11mh*dp1u(i1-1,i2,i3,c) \
      -( a11ph2*dp1Cubedu(i1-1,i2,i3,c) - a11mh2*dp1Cubedu(i1-2,i2,i3,c) \
         +a11p3h*dp1u(i1+1,i2,i3,c)-3.*a11ph2*dp1u(i1,i2,i3,c)+3.*a11mh2*dp1u(i1-1,i2,i3,c)-a11m3h*dp1u(i1-2,i2,i3,c) \
       )/24. \
     )*drsqi(1) \
    +(  a22ph*dp2u(i1,i2,i3,c)-a22mh*dp2u(i1,i2-1,i3,c) \
      -( a22ph2*dp2Cubedu(i1,i2-1,i3,c) - a22mh2*dp2Cubedu(i1,i2-2,i3,c) \
         +a22p3h*dp2u(i1,i2+1,i3,c)-3.*a22ph2*dp2u(i1,i2,i3,c)+3.*a22mh2*dp2u(i1,i2-1,i3,c)-a22m3h*dp2u(i1,i2-2,i3,c) \
        )/24. \
     )*drsqi(2) \
    +( a12(i1+1,i2,i3)*( (4./3.)*dz2u(i1+1,i2,i3,c) - dzdpdm2u(i1+1,i2,i3,c)/6. ) \
      -a12(i1-1,i2,i3)*( (4./3.)*dz2u(i1-1,i2,i3,c) - dzdpdm2u(i1-1,i2,i3,c)/6. ) \
      +a12(i1,i2+1,i3)*( (4./3.)*dz1u(i1,i2+1,i3,c) - dzdpdm1u(i1,i2+1,i3,c)/6. ) \
      -a12(i1,i2-1,i3)*( (4./3.)*dz1u(i1,i2-1,i3,c) - dzdpdm1u(i1,i2-1,i3,c)/6. ) \
      -( a12(i1+2,i2,i3)*dz2u(i1+2,i2,i3,c)-a12(i1-2,i2,i3)*dz2u(i1-2,i2,i3,c) \
        +a12(i1,i2+2,i3)*dz1u(i1,i2+2,i3,c)-a12(i1,i2-2,i3)*dz1u(i1,i2-2,i3,c) )/6. \
     )/(4.*dr(1)*dr(2)) \
   )*jac


end do
end do
end do
end do

else if( nd.eq.3 )then

! write(*,*) '****INSIDE dsgc4 3d'

do c=ca,cb
do i3=n3a,n3b
do i2=n2a,n2b
do i1=n1a,n1b


 a11i  = a113d(i1,i2,i3)
 a11m1 = a113d(i1-1,i2,i3)
 a11p1 = a113d(i1+1,i2,i3)
 a11m2 = a113d(i1-2,i2,i3)
 a11p2 = a113d(i1+2,i2,i3)
 
 a11ph  = ((a11p1+a11i)*9.-(a11p2+a11m1))/16.
 a11mh  = ((a11i+a11m1)*9.-(a11p1+a11m2))/16.
 a11ph2 = .5*(a11p1+a11i)
 a11mh2 = .5*(a11m1+a11i)
 a11p3h = .5*(a11p2+a11p1)
 a11m3h = .5*(a11m1+a11m2)
 
 a22i  = a223d(i1,i2,i3)
 a22m1 = a223d(i1,i2-1,i3)
 a22p1 = a223d(i1,i2+1,i3)
 a22m2 = a223d(i1,i2-2,i3)
 a22p2 = a223d(i1,i2+2,i3)
 
 a22ph  = ((a22p1+a22i)*9.-(a22p2+a22m1))/16.
 a22mh  = ((a22i+a22m1)*9.-(a22p1+a22m2))/16.
 a22ph2 = .5*(a22p1+a22i)
 a22mh2 = .5*(a22m1+a22i)
 a22p3h = .5*(a22p2+a22p1)
 a22m3h = .5*(a22m1+a22m2)
 
 a33i  = a333d(i1,i2,i3)
 a33m1 = a333d(i1,i2,i3-1)
 a33p1 = a333d(i1,i2,i3+1)
 a33m2 = a333d(i1,i2,i3-2)
 a33p2 = a333d(i1,i2,i3+2)
 
 a33ph  = ((a33p1+a33i)*9.-(a33p2+a33m1))/16.
 a33mh  = ((a33i+a33m1)*9.-(a33p1+a33m2))/16.
 a33ph2 = .5*(a33p1+a33i)
 a33mh2 = .5*(a33m1+a33i)
 a33p3h = .5*(a33p2+a33p1)
 a33m3h = .5*(a33m1+a33m2)
 
 jac= ajac3d(i1,i2,i3)
 deriv(i1,i2,i3,c)=\
   (\
     ( a11ph*dp1u(i1,i2,i3,c)-a11mh*dp1u(i1-1,i2,i3,c) \
      -( a11ph2*dp1Cubedu(i1-1,i2,i3,c) - a11mh2*dp1Cubedu(i1-2,i2,i3,c) \
         +a11p3h*dp1u(i1+1,i2,i3,c)-3.*a11ph2*dp1u(i1,i2,i3,c)+3.*a11mh2*dp1u(i1-1,i2,i3,c)-a11m3h*dp1u(i1-2,i2,i3,c) \
       )/24. \
     )*drsqi(1) \
    +(  a22ph*dp2u(i1,i2,i3,c)-a22mh*dp2u(i1,i2-1,i3,c) \
      -( a22ph2*dp2Cubedu(i1,i2-1,i3,c) - a22mh2*dp2Cubedu(i1,i2-2,i3,c) \
         +a22p3h*dp2u(i1,i2+1,i3,c)-3.*a22ph2*dp2u(i1,i2,i3,c)+3.*a22mh2*dp2u(i1,i2-1,i3,c)-a22m3h*dp2u(i1,i2-2,i3,c) \
        )/24. \
     )*drsqi(2) \
    +(  a33ph*dp3u(i1,i2,i3,c)-a33mh*dp3u(i1,i2,i3-1,c) \
      -( a33ph2*dp3Cubedu(i1,i2,i3-1,c) - a33mh2*dp3Cubedu(i1,i2,i3-2,c) \
         +a33p3h*dp3u(i1,i2,i3+1,c)-3.*a33ph2*dp3u(i1,i2,i3,c)+3.*a33mh2*dp3u(i1,i2,i3-1,c)-a33m3h*dp3u(i1,i2,i3-2,c) \
        )/24. \
     )*drsqi(3) \
    +( a123d(i1+1,i2,i3)*( (4./3.)*dz2u(i1+1,i2,i3,c) - dzdpdm2u(i1+1,i2,i3,c)/6. ) \
      -a123d(i1-1,i2,i3)*( (4./3.)*dz2u(i1-1,i2,i3,c) - dzdpdm2u(i1-1,i2,i3,c)/6. ) \
      +a123d(i1,i2+1,i3)*( (4./3.)*dz1u(i1,i2+1,i3,c) - dzdpdm1u(i1,i2+1,i3,c)/6. ) \
      -a123d(i1,i2-1,i3)*( (4./3.)*dz1u(i1,i2-1,i3,c) - dzdpdm1u(i1,i2-1,i3,c)/6. ) \
      -( a123d(i1+2,i2,i3)*dz2u(i1+2,i2,i3,c)-a123d(i1-2,i2,i3)*dz2u(i1-2,i2,i3,c) \
        +a123d(i1,i2+2,i3)*dz1u(i1,i2+2,i3,c)-a123d(i1,i2-2,i3)*dz1u(i1,i2-2,i3,c) )/6. \
     )/(4.*dr(1)*dr(2)) \
    +( a133d(i1+1,i2,i3)*( (4./3.)*dz3u(i1+1,i2,i3,c) - dzdpdm3u(i1+1,i2,i3,c)/6. ) \
      -a133d(i1-1,i2,i3)*( (4./3.)*dz3u(i1-1,i2,i3,c) - dzdpdm3u(i1-1,i2,i3,c)/6. ) \
      +a133d(i1,i2,i3+1)*( (4./3.)*dz1u(i1,i2,i3+1,c) - dzdpdm1u(i1,i2,i3+1,c)/6. ) \
      -a133d(i1,i2,i3-1)*( (4./3.)*dz1u(i1,i2,i3-1,c) - dzdpdm1u(i1,i2,i3-1,c)/6. ) \
      -( a133d(i1+2,i2,i3)*dz3u(i1+2,i2,i3,c)-a133d(i1-2,i2,i3)*dz3u(i1-2,i2,i3,c) \
        +a133d(i1,i2,i3+2)*dz1u(i1,i2,i3+2,c)-a133d(i1,i2,i3-2)*dz1u(i1,i2,i3-2,c) )/6. \
     )/(4.*dr(1)*dr(3)) \
    +( a233d(i1,i2+1,i3)*( (4./3.)*dz3u(i1,i2+1,i3,c) - dzdpdm3u(i1,i2+1,i3,c)/6. ) \
      -a233d(i1,i2-1,i3)*( (4./3.)*dz3u(i1,i2-1,i3,c) - dzdpdm3u(i1,i2-1,i3,c)/6. ) \
      +a233d(i1,i2,i3+1)*( (4./3.)*dz2u(i1,i2,i3+1,c) - dzdpdm2u(i1,i2,i3+1,c)/6. ) \
      -a233d(i1,i2,i3-1)*( (4./3.)*dz2u(i1,i2,i3-1,c) - dzdpdm2u(i1,i2,i3-1,c)/6. ) \
      -( a233d(i1,i2+2,i3)*dz3u(i1,i2+2,i3,c)-a233d(i1,i2-2,i3)*dz3u(i1,i2-2,i3,c) \
        +a233d(i1,i2,i3+2)*dz2u(i1,i2,i3+2,c)-a233d(i1,i2,i3-2)*dz2u(i1,i2,i3-2,c) )/6. \
     )/(4.*dr(2)*dr(3)) \
   )*jac

 ! write(*,*) 'i1,i2,i3,jac,deriv=',i1,i2,i3,jac,deriv(i1,i2,i3,c)

end do
end do
end do
end do

else
  write(*,*) 'dsgc4: ERROR invalid nd=',nd
  stop 11
end if

else 

  ! ******************************************************************
  ! *************Rectangular Grid ************************************
  ! ******************************************************************

! This case should only be called for divScalarGrad -- not laplace
#If #operatorName == "laplace4Cons"
write(*,*) 'ERROR: laplace4Cons called for rectangular grids'
stop 20
#End

dxsqi(1)=1./dx(1)**2
dxsqi(2)=1./dx(2)**2
dxsqi(3)=1./dx(3)**2

#If #operatorName == "divScalarGrad4Cons"
!  ************************************************
!  ************divScalarGrad RECTANGULAR **********
!  ************************************************

 if( nd.eq.2 )then
 
 ! write(*,*) '****INSIDE dsgc4 2d rectangular'
 
 do c=ca,cb
 do i3=n3a,n3b
 do i2=n2a,n2b
 do i1=n1a,n1b
 
 
  a11i  = s(i1,i2,i3,0)
  a11m1 = s(i1-1,i2,i3,0)
  a11p1 = s(i1+1,i2,i3,0)
  a11m2 = s(i1-2,i2,i3,0)
  a11p2 = s(i1+2,i2,i3,0)
  
  a11ph  = ((a11p1+a11i)*9.-(a11p2+a11m1))/16.
  a11mh  = ((a11i+a11m1)*9.-(a11p1+a11m2))/16.
  a11ph2 = .5*(a11p1+a11i)  ! we must consistently use the 2nd-order version for the higher order terms
  a11mh2 = .5*(a11m1+a11i)
  a11p3h = .5*(a11p2+a11p1)
  a11m3h = .5*(a11m1+a11m2)
 
  a22i  = s(i1,i2,i3,0)
  a22m1 = s(i1,i2-1,i3,0)
  a22p1 = s(i1,i2+1,i3,0)
  a22m2 = s(i1,i2-2,i3,0)
  a22p2 = s(i1,i2+2,i3,0)
 
  a22ph  = ((a22p1+a22i)*9.-(a22p2+a22m1))/16.
  a22mh  = ((a22i+a22m1)*9.-(a22p1+a22m2))/16.
  a22ph2 = .5*(a22p1+a22i)
  a22mh2 = .5*(a22m1+a22i)
  a22p3h = .5*(a22p2+a22p1)
  a22m3h = .5*(a22m1+a22m2)
 
  deriv(i1,i2,i3,c)=\
    (\
      ( a11ph*dp1u(i1,i2,i3,c)-a11mh*dp1u(i1-1,i2,i3,c) \
       -( a11ph2*dp1Cubedu(i1-1,i2,i3,c) - a11mh2*dp1Cubedu(i1-2,i2,i3,c) \
          +a11p3h*dp1u(i1+1,i2,i3,c)-3.*a11ph2*dp1u(i1,i2,i3,c)+3.*a11mh2*dp1u(i1-1,i2,i3,c)-a11m3h*dp1u(i1-2,i2,i3,c) \
        )/24. \
      )*dxsqi(1) \
     +(  a22ph*dp2u(i1,i2,i3,c)-a22mh*dp2u(i1,i2-1,i3,c) \
       -( a22ph2*dp2Cubedu(i1,i2-1,i3,c) - a22mh2*dp2Cubedu(i1,i2-2,i3,c) \
          +a22p3h*dp2u(i1,i2+1,i3,c)-3.*a22ph2*dp2u(i1,i2,i3,c)+3.*a22mh2*dp2u(i1,i2-1,i3,c)-a22m3h*dp2u(i1,i2-2,i3,c) \
         )/24. \
      )*dxsqi(2) \
    )
 
 end do
 end do
 end do
 end do
 
 else if( nd.eq.3 )then
 
 ! write(*,*) '****INSIDE dsgc4 3d rectangular'
 
 do c=ca,cb
 do i3=n3a,n3b
 do i2=n2a,n2b
 do i1=n1a,n1b
 
 
  a11i  = s(i1,i2,i3,0)
  a11m1 = s(i1-1,i2,i3,0)
  a11p1 = s(i1+1,i2,i3,0)
  a11m2 = s(i1-2,i2,i3,0)
  a11p2 = s(i1+2,i2,i3,0)
  
  a11ph  = ((a11p1+a11i)*9.-(a11p2+a11m1))/16.
  a11mh  = ((a11i+a11m1)*9.-(a11p1+a11m2))/16.
  a11ph2 = .5*(a11p1+a11i)
  a11mh2 = .5*(a11m1+a11i)
  a11p3h = .5*(a11p2+a11p1)
  a11m3h = .5*(a11m1+a11m2)
  
  a22i  = s(i1,i2,i3,0)
  a22m1 = s(i1,i2-1,i3,0)
  a22p1 = s(i1,i2+1,i3,0)
  a22m2 = s(i1,i2-2,i3,0)
  a22p2 = s(i1,i2+2,i3,0)
  
  a22ph  = ((a22p1+a22i)*9.-(a22p2+a22m1))/16.
  a22mh  = ((a22i+a22m1)*9.-(a22p1+a22m2))/16.
  a22ph2 = .5*(a22p1+a22i)
  a22mh2 = .5*(a22m1+a22i)
  a22p3h = .5*(a22p2+a22p1)
  a22m3h = .5*(a22m1+a22m2)
  
  a33i  = s(i1,i2,i3,0)
  a33m1 = s(i1,i2,i3-1,0)
  a33p1 = s(i1,i2,i3+1,0)
  a33m2 = s(i1,i2,i3-2,0)
  a33p2 = s(i1,i2,i3+2,0)
  
  a33ph  = ((a33p1+a33i)*9.-(a33p2+a33m1))/16.
  a33mh  = ((a33i+a33m1)*9.-(a33p1+a33m2))/16.
  a33ph2 = .5*(a33p1+a33i)
  a33mh2 = .5*(a33m1+a33i)
  a33p3h = .5*(a33p2+a33p1)
  a33m3h = .5*(a33m1+a33m2)
  
  deriv(i1,i2,i3,c)=\
    (\
      ( a11ph*dp1u(i1,i2,i3,c)-a11mh*dp1u(i1-1,i2,i3,c) \
       -( a11ph2*dp1Cubedu(i1-1,i2,i3,c) - a11mh2*dp1Cubedu(i1-2,i2,i3,c) \
          +a11p3h*dp1u(i1+1,i2,i3,c)-3.*a11ph2*dp1u(i1,i2,i3,c)+3.*a11mh2*dp1u(i1-1,i2,i3,c)-a11m3h*dp1u(i1-2,i2,i3,c) \
        )/24. \
      )*dxsqi(1) \
     +(  a22ph*dp2u(i1,i2,i3,c)-a22mh*dp2u(i1,i2-1,i3,c) \
       -( a22ph2*dp2Cubedu(i1,i2-1,i3,c) - a22mh2*dp2Cubedu(i1,i2-2,i3,c) \
          +a22p3h*dp2u(i1,i2+1,i3,c)-3.*a22ph2*dp2u(i1,i2,i3,c)+3.*a22mh2*dp2u(i1,i2-1,i3,c)-a22m3h*dp2u(i1,i2-2,i3,c) \
         )/24. \
      )*dxsqi(2) \
     +(  a33ph*dp3u(i1,i2,i3,c)-a33mh*dp3u(i1,i2,i3-1,c) \
       -( a33ph2*dp3Cubedu(i1,i2,i3-1,c) - a33mh2*dp3Cubedu(i1,i2,i3-2,c) \
          +a33p3h*dp3u(i1,i2,i3+1,c)-3.*a33ph2*dp3u(i1,i2,i3,c)+3.*a33mh2*dp3u(i1,i2,i3-1,c)-a33m3h*dp3u(i1,i2,i3-2,c) \
         )/24. \
      )*dxsqi(3) \
    )
 
 end do
 end do
 end do
 end do
 
 else
   write(*,*) 'dsgc4: ERROR invalid nd=',nd
   stop 11
 end if

#Elif #operatorName == "divTensorGrad4Cons"
!  ************************************************
!  ************divTensorGrad RECTANGULAR **********
!  ************************************************
 if( nd.eq.2 )then
 
 ! write(*,*) '****INSIDE dsgc4 2d rectangular'
 
 do c=ca,cb
 do i3=n3a,n3b
 do i2=n2a,n2b
 do i1=n1a,n1b
 
 
 a11i  = a11r(i1,i2,i3)
 a11m1 = a11r(i1-1,i2,i3)
 a11p1 = a11r(i1+1,i2,i3)
 a11m2 = a11r(i1-2,i2,i3)
 a11p2 = a11r(i1+2,i2,i3)
 
 a11ph  = ((a11p1+a11i)*9.-(a11p2+a11m1))/16.
 a11mh  = ((a11i+a11m1)*9.-(a11p1+a11m2))/16.
 a11ph2 = .5*(a11p1+a11i)  ! we must consistently use the 2nd-order version for the higher order terms
 a11mh2 = .5*(a11m1+a11i)
 a11p3h = .5*(a11p2+a11p1)
 a11m3h = .5*(a11m1+a11m2)

 a22i  = a22r(i1,i2,i3)
 a22m1 = a22r(i1,i2-1,i3)
 a22p1 = a22r(i1,i2+1,i3)
 a22m2 = a22r(i1,i2-2,i3)
 a22p2 = a22r(i1,i2+2,i3)

 a22ph  = ((a22p1+a22i)*9.-(a22p2+a22m1))/16.
 a22mh  = ((a22i+a22m1)*9.-(a22p1+a22m2))/16.
 a22ph2 = .5*(a22p1+a22i)
 a22mh2 = .5*(a22m1+a22i)
 a22p3h = .5*(a22p2+a22p1)
 a22m3h = .5*(a22m1+a22m2)

 ! --- new --
 deriv(i1,i2,i3,c)=\
   (\
     ( a11ph*dp1u(i1,i2,i3,c)-a11mh*dp1u(i1-1,i2,i3,c) \
      -( a11ph2*dp1Cubedu(i1-1,i2,i3,c) - a11mh2*dp1Cubedu(i1-2,i2,i3,c) \
         +a11p3h*dp1u(i1+1,i2,i3,c)-3.*a11ph2*dp1u(i1,i2,i3,c)+3.*a11mh2*dp1u(i1-1,i2,i3,c)-a11m3h*dp1u(i1-2,i2,i3,c) \
       )/24. \
     )*dxsqi(1) \
    +(  a22ph*dp2u(i1,i2,i3,c)-a22mh*dp2u(i1,i2-1,i3,c) \
      -( a22ph2*dp2Cubedu(i1,i2-1,i3,c) - a22mh2*dp2Cubedu(i1,i2-2,i3,c) \
         +a22p3h*dp2u(i1,i2+1,i3,c)-3.*a22ph2*dp2u(i1,i2,i3,c)+3.*a22mh2*dp2u(i1,i2-1,i3,c)-a22m3h*dp2u(i1,i2-2,i3,c) \
        )/24. \
     )*dxsqi(2) \
    +( a12r(i1+1,i2,i3)*( (4./3.)*dz2u(i1+1,i2,i3,c) - dzdpdm2u(i1+1,i2,i3,c)/6. ) \
      -a12r(i1-1,i2,i3)*( (4./3.)*dz2u(i1-1,i2,i3,c) - dzdpdm2u(i1-1,i2,i3,c)/6. ) \
      +a12r(i1,i2+1,i3)*( (4./3.)*dz1u(i1,i2+1,i3,c) - dzdpdm1u(i1,i2+1,i3,c)/6. ) \
      -a12r(i1,i2-1,i3)*( (4./3.)*dz1u(i1,i2-1,i3,c) - dzdpdm1u(i1,i2-1,i3,c)/6. ) \
      -( a12r(i1+2,i2,i3)*dz2u(i1+2,i2,i3,c)-a12r(i1-2,i2,i3)*dz2u(i1-2,i2,i3,c) \
        +a12r(i1,i2+2,i3)*dz1u(i1,i2+2,i3,c)-a12r(i1,i2-2,i3)*dz1u(i1,i2-2,i3,c) )/6. \
     )/(4.*dx(1)*dx(2)) \
   )


 end do
 end do
 end do
 end do
 
 else if( nd.eq.3 )then
 
 ! write(*,*) '****INSIDE dsgc4 3d rectangular'
 
 do c=ca,cb
 do i3=n3a,n3b
 do i2=n2a,n2b
 do i1=n1a,n1b
 
 
 a11i  = a113dr(i1,i2,i3)
 a11m1 = a113dr(i1-1,i2,i3)
 a11p1 = a113dr(i1+1,i2,i3)
 a11m2 = a113dr(i1-2,i2,i3)
 a11p2 = a113dr(i1+2,i2,i3)
 
 a11ph  = ((a11p1+a11i)*9.-(a11p2+a11m1))/16.
 a11mh  = ((a11i+a11m1)*9.-(a11p1+a11m2))/16.
 a11ph2 = .5*(a11p1+a11i)
 a11mh2 = .5*(a11m1+a11i)
 a11p3h = .5*(a11p2+a11p1)
 a11m3h = .5*(a11m1+a11m2)
 
 a22i  = a223dr(i1,i2,i3)
 a22m1 = a223dr(i1,i2-1,i3)
 a22p1 = a223dr(i1,i2+1,i3)
 a22m2 = a223dr(i1,i2-2,i3)
 a22p2 = a223dr(i1,i2+2,i3)
 
 a22ph  = ((a22p1+a22i)*9.-(a22p2+a22m1))/16.
 a22mh  = ((a22i+a22m1)*9.-(a22p1+a22m2))/16.
 a22ph2 = .5*(a22p1+a22i)
 a22mh2 = .5*(a22m1+a22i)
 a22p3h = .5*(a22p2+a22p1)
 a22m3h = .5*(a22m1+a22m2)
 
 a33i  = a333dr(i1,i2,i3)
 a33m1 = a333dr(i1,i2,i3-1)
 a33p1 = a333dr(i1,i2,i3+1)
 a33m2 = a333dr(i1,i2,i3-2)
 a33p2 = a333dr(i1,i2,i3+2)
 
 a33ph  = ((a33p1+a33i)*9.-(a33p2+a33m1))/16.
 a33mh  = ((a33i+a33m1)*9.-(a33p1+a33m2))/16.
 a33ph2 = .5*(a33p1+a33i)
 a33mh2 = .5*(a33m1+a33i)
 a33p3h = .5*(a33p2+a33p1)
 a33m3h = .5*(a33m1+a33m2)

! --- new ---

 deriv(i1,i2,i3,c)=\
   (\
     ( a11ph*dp1u(i1,i2,i3,c)-a11mh*dp1u(i1-1,i2,i3,c) \
      -( a11ph2*dp1Cubedu(i1-1,i2,i3,c) - a11mh2*dp1Cubedu(i1-2,i2,i3,c) \
         +a11p3h*dp1u(i1+1,i2,i3,c)-3.*a11ph2*dp1u(i1,i2,i3,c)+3.*a11mh2*dp1u(i1-1,i2,i3,c)-a11m3h*dp1u(i1-2,i2,i3,c) \
       )/24. \
     )*dxsqi(1) \
    +(  a22ph*dp2u(i1,i2,i3,c)-a22mh*dp2u(i1,i2-1,i3,c) \
      -( a22ph2*dp2Cubedu(i1,i2-1,i3,c) - a22mh2*dp2Cubedu(i1,i2-2,i3,c) \
         +a22p3h*dp2u(i1,i2+1,i3,c)-3.*a22ph2*dp2u(i1,i2,i3,c)+3.*a22mh2*dp2u(i1,i2-1,i3,c)-a22m3h*dp2u(i1,i2-2,i3,c) \
        )/24. \
     )*dxsqi(2) \
    +(  a33ph*dp3u(i1,i2,i3,c)-a33mh*dp3u(i1,i2,i3-1,c) \
      -( a33ph2*dp3Cubedu(i1,i2,i3-1,c) - a33mh2*dp3Cubedu(i1,i2,i3-2,c) \
         +a33p3h*dp3u(i1,i2,i3+1,c)-3.*a33ph2*dp3u(i1,i2,i3,c)+3.*a33mh2*dp3u(i1,i2,i3-1,c)-a33m3h*dp3u(i1,i2,i3-2,c) \
        )/24. \
     )*dxsqi(3) \
    +( a123dr(i1+1,i2,i3)*( (4./3.)*dz2u(i1+1,i2,i3,c) - dzdpdm2u(i1+1,i2,i3,c)/6. ) \
      -a123dr(i1-1,i2,i3)*( (4./3.)*dz2u(i1-1,i2,i3,c) - dzdpdm2u(i1-1,i2,i3,c)/6. ) \
      +a123dr(i1,i2+1,i3)*( (4./3.)*dz1u(i1,i2+1,i3,c) - dzdpdm1u(i1,i2+1,i3,c)/6. ) \
      -a123dr(i1,i2-1,i3)*( (4./3.)*dz1u(i1,i2-1,i3,c) - dzdpdm1u(i1,i2-1,i3,c)/6. ) \
      -( a123dr(i1+2,i2,i3)*dz2u(i1+2,i2,i3,c)-a123dr(i1-2,i2,i3)*dz2u(i1-2,i2,i3,c) \
        +a123dr(i1,i2+2,i3)*dz1u(i1,i2+2,i3,c)-a123dr(i1,i2-2,i3)*dz1u(i1,i2-2,i3,c) )/6. \
     )/(4.*dx(1)*dx(2)) \
    +( a133dr(i1+1,i2,i3)*( (4./3.)*dz3u(i1+1,i2,i3,c) - dzdpdm3u(i1+1,i2,i3,c)/6. ) \
      -a133dr(i1-1,i2,i3)*( (4./3.)*dz3u(i1-1,i2,i3,c) - dzdpdm3u(i1-1,i2,i3,c)/6. ) \
      +a133dr(i1,i2,i3+1)*( (4./3.)*dz1u(i1,i2,i3+1,c) - dzdpdm1u(i1,i2,i3+1,c)/6. ) \
      -a133dr(i1,i2,i3-1)*( (4./3.)*dz1u(i1,i2,i3-1,c) - dzdpdm1u(i1,i2,i3-1,c)/6. ) \
      -( a133dr(i1+2,i2,i3)*dz3u(i1+2,i2,i3,c)-a133dr(i1-2,i2,i3)*dz3u(i1-2,i2,i3,c) \
        +a133dr(i1,i2,i3+2)*dz1u(i1,i2,i3+2,c)-a133dr(i1,i2,i3-2)*dz1u(i1,i2,i3-2,c) )/6. \
     )/(4.*dx(1)*dx(3)) \
    +( a233dr(i1,i2+1,i3)*( (4./3.)*dz3u(i1,i2+1,i3,c) - dzdpdm3u(i1,i2+1,i3,c)/6. ) \
      -a233dr(i1,i2-1,i3)*( (4./3.)*dz3u(i1,i2-1,i3,c) - dzdpdm3u(i1,i2-1,i3,c)/6. ) \
      +a233dr(i1,i2,i3+1)*( (4./3.)*dz2u(i1,i2,i3+1,c) - dzdpdm2u(i1,i2,i3+1,c)/6. ) \
      -a233dr(i1,i2,i3-1)*( (4./3.)*dz2u(i1,i2,i3-1,c) - dzdpdm2u(i1,i2,i3-1,c)/6. ) \
      -( a233dr(i1,i2+2,i3)*dz3u(i1,i2+2,i3,c)-a233dr(i1,i2-2,i3)*dz3u(i1,i2-2,i3,c) \
        +a233dr(i1,i2,i3+2)*dz2u(i1,i2,i3+2,c)-a233dr(i1,i2,i3-2)*dz2u(i1,i2,i3-2,c) )/6. \
     )/(4.*dx(2)*dx(3)) \
   )

 end do
 end do
 end do
 end do
 
 else
   write(*,*) 'dsgc4: ERROR invalid nd=',nd
   stop 11
 end if


#Else
   ! unexpected operatorName
   stop 1893
#End


end if


return
end


#endMacro





      divScalarGradConservativeFourthOrder(laplace4Cons)

      divScalarGradConservativeFourthOrder(divScalarGrad4Cons)

      divScalarGradConservativeFourthOrder(divTensorGrad4Cons)
