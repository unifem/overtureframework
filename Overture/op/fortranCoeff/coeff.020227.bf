      subroutine coeffOperator( nd, 
     &    nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &    n1a,n1b,n2a,n2b,n3a,n3b, 
     &    ndc, nc, ns, ea,eb, ca,cb,
     &    dx, dr,
     &    rsxy, jacobian, coeff,s, 
     &    ndw,w,  ! work space
     &    derivative, derivType, gridType, order, averagingType, 
     &    dir1, dir2, ierr  )
c ===============================================================
c    Build Coefficients for Operators
c  *** This subroutine calls the appropriate sub for a particular operator ***
c  
c  nd : number of range spatial dimensions 
c  nd1a,nd1b : mesh dimensions axis 1
c  nd2a,nd2b : mesh dimensions axis 2
c  nd3a,nd3b : mesh dimensions axis 3
c
c  ndc : number of coefficients/mesh point
c  nc1a,nd1b : coefficient array dimensions axis 1
c  nc2a,nd2b : coefficient array dimensions axis 2
c  nc3a,nd3b : coefficient array dimensions axis 3
c
c  nc1a,nd1b : subset for evaluating operator, axis 1
c  nc2a,nd2b : subset for evaluating operator, axis 2
c  nc3a,nd3b : subset for evaluating operator, axis 3
c
c  nc : number of components
c  ns : stencil size
c  ca,cb : assign components c=ca,..,cb (base 0)
c  ea,eb : assign equations e=ea,..eb   (base 0)
c
c  dx : grid spacing for rectangular grids.
c  dr : unit square spacing
c
c  derivative : specify the derivative 0=xDerivative, ... (from the enum in MappedGridOperators.h)
c  rsxy : jacobian information, not used if rectangular
c  coeff : coefficient matrix
c  gridType: 0=rectangular, 1=non-rectangular
c  order : 2 or 4
c  averagingType : arithmeticAverage=0, harmonicAverage=1, for conservative approximations
c  dir1,dir2 : for derivative=derivativeScalarDerivative
c
c  ndw : sixe of the work space w
c  w   : work space required for some operators. 
c  ierr : error return
c 
c======================================================================
c      implicit none
      integer nd, 
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b,
     &  n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,
     &  derivative, derivType, gridType, order, averagingType,ndw,
     & dir1,dir2,ierr

      real dx(3),dr(3)
      real rsxy(*)
      real jacobian(*)
      real s(*)
      real coeff(1:ndc,nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real w(0:*)

      parameter(
     &    xDerivative=0,
     &    yDerivative=xDerivative+1,
     &    zDerivative=yDerivative+1,
     &    xxDerivative=zDerivative+1,
     &    xyDerivative=xxDerivative+1,
     &    xzDerivative=xyDerivative+1,
     &    yxDerivative=xzDerivative+1,
     &    yyDerivative=yxDerivative+1,
     &    yzDerivative=yyDerivative+1,
     &    zxDerivative=yzDerivative+1,
     &    zyDerivative=zxDerivative+1,
     &    zzDerivative=zyDerivative+1,
     &    laplacianOperator=zzDerivative+1,
     &    r1Derivative=laplacianOperator+1,
     &    r2Derivative=r1Derivative+1,
     &    r3Derivative=r2Derivative+1,
     &    r1r1Derivative=r3Derivative+1,
     &    r1r2Derivative=r1r1Derivative+1,
     &    r1r3Derivative=r1r2Derivative+1,
     &    r2r2Derivative=r1r3Derivative+1,
     &    r2r3Derivative=r2r2Derivative+1,
     &    r3r3Derivative=r2r3Derivative+1,
     &    gradient=r3r3Derivative+1,
     &    divergence=gradient+1,
     &    divergenceScalarGradient=divergence+1,
     &    scalarGradient=divergenceScalarGradient+1,
     &    identityOperator=scalarGradient+1,
     &    vorticityOperator=identityOperator+1,
     &    xDerivativeScalarXDerivative=vorticityOperator+1,
     &    xDerivativeScalarYDerivative=xDerivativeScalarXDerivative+1,
     &    xDerivativeScalarZDerivative=xDerivativeScalarYDerivative+1,
     &    yDerivativeScalarXDerivative=xDerivativeScalarZDerivative+1,
     &    yDerivativeScalarYDerivative=yDerivativeScalarXDerivative+1,
     &    yDerivativeScalarZDerivative=yDerivativeScalarYDerivative+1,
     &    zDerivativeScalarXDerivative=yDerivativeScalarZDerivative+1,
     &    zDerivativeScalarYDerivative=zDerivativeScalarXDerivative+1,
     &    zDerivativeScalarZDerivative=zDerivativeScalarYDerivative+1,
     &    divVectorScalarDerivative=zDerivativeScalarZDerivative+1)

      ierr=0

#beginMacro callOperator(x)
if( order.eq.2 )then
  call x ## Coeff2ndOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a, nd3b,n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,\
   ca,cb, dx,dr, rsxy,coeff, gridType, order )
else
  call x ## Coeff4thOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a, nd3b,n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,\
   ca,cb, dx,dr, rsxy,coeff, gridType, order )
end if
#endMacro

      if( derivative.eq.laplacianOperator )then
        callOperator(laplacian)
      else if( derivative.eq.xDerivative )then
        callOperator(x)
      else if( derivative.eq.yDerivative )then
        callOperator(y)
      else if( derivative.eq.zDerivative )then
        callOperator(z)
      else if( derivative.eq.xxDerivative )then
        callOperator(xx)
      else if( derivative.eq.xyDerivative )then
        callOperator(xy)
      else if( derivative.eq.xzDerivative )then
        callOperator(xz)
      else if( derivative.eq.yyDerivative )then
        callOperator(yy)
      else if( derivative.eq.yzDerivative )then
        callOperator(yz)
      else if( derivative.eq.zzDerivative )then
        callOperator(zz)
      else if( derivative.eq.identityOperator )then
        callOperator(identity)
      else
        ierr=1
        write(*,*) 'coeffOperator:ERROR: unimplemented derivative=',
     &     derivative
      end if

      return 
      end        





#beginMacro loops(arg)
do i3=n3a,n3b
  do i2=n2a,n2b
    do i1=n1a,n1b
      arg
    end do
  end do
end do
#endMacro

#beginMacro beginLoops()
c ***** loop over equations and components *****
do e=ea,eb
do c=ca,cb
ec=ns*(c+nc*e)
c ** it did not affect performance to use an array to index coeff ***
if( nd.eq.2 )then
do i2=-1,1
  do i1=-1,1
   m(i1,i2)=i1+1+3*(i2+1) +1 + ec
  end do
end do
else if( nd.eq.3 )then
do i3=-1,1
  do i2=-1,1
    do i1=-1,1
      m3(i1,i2,i3)=i1+1+3*(i2+1+3*(i3+1)) +1 + ec
    end do
  end do
end do
else
m12=1 + ec
m22=2 + ec
m32=3 + ec
endif

do i3=n3a,n3b
do i2=n2a,n2b
do i1=n1a,n1b
#endMacro

#beginMacro endLoops()
end do
end do
end do
end do
end do
#endMacro


#beginMacro loopBody2ndOrder1d(c0,c1,c2)
 coeff(m12,i1,i2,i3)=c0
 coeff(m22,i1,i2,i3)=c1
 coeff(m32,i1,i2,i3)=c2
#endMacro

#beginMacro loopBody2ndOrder2d(c00,c10,c20,c01,c11,c21,c02,c12,c22)
  coeff(m(-1,-1),i1,i2,i3)=c00
  coeff(m( 0,-1),i1,i2,i3)=c10
  coeff(m(+1,-1),i1,i2,i3)=c20
  coeff(m(-1, 0),i1,i2,i3)=c01
  coeff(m( 0, 0),i1,i2,i3)=c11
  coeff(m(+1, 0),i1,i2,i3)=c21
  coeff(m(-1,+1),i1,i2,i3)=c02
  coeff(m( 0,+1),i1,i2,i3)=c12
  coeff(m(+1,+1),i1,i2,i3)=c22
#endMacro

#beginMacro loopBody2ndOrder3d(c000,c100,c200,c010,c110,c210,c020,c120,c220,c001,c101,c201,c011,c111,c211,c021,c121,c221,c002,c102,c202,c012,c112,c212,c022,c122,c222)
 coeff(m3(-1,-1,-1),i1,i2,i3)=c000
 coeff(m3( 0,-1,-1),i1,i2,i3)=c100
 coeff(m3(+1,-1,-1),i1,i2,i3)=c200
 coeff(m3(-1, 0,-1),i1,i2,i3)=c010
 coeff(m3( 0, 0,-1),i1,i2,i3)=c110
 coeff(m3(+1, 0,-1),i1,i2,i3)=c210
 coeff(m3(-1,+1,-1),i1,i2,i3)=c020
 coeff(m3( 0,+1,-1),i1,i2,i3)=c120
 coeff(m3(+1,+1,-1),i1,i2,i3)=c220
 coeff(m3(-1,-1, 0),i1,i2,i3)=c001
 coeff(m3( 0,-1, 0),i1,i2,i3)=c101
 coeff(m3(+1,-1, 0),i1,i2,i3)=c201
 coeff(m3(-1, 0, 0),i1,i2,i3)=c011
 coeff(m3( 0, 0, 0),i1,i2,i3)=c111
 coeff(m3(+1, 0, 0),i1,i2,i3)=c211
 coeff(m3(-1,+1, 0),i1,i2,i3)=c021
 coeff(m3( 0,+1, 0),i1,i2,i3)=c121
 coeff(m3(+1,+1, 0),i1,i2,i3)=c221
 coeff(m3(-1,-1,+1),i1,i2,i3)=c002
 coeff(m3( 0,-1,+1),i1,i2,i3)=c102
 coeff(m3(+1,-1,+1),i1,i2,i3)=c202
 coeff(m3(-1, 0,+1),i1,i2,i3)=c012
 coeff(m3( 0, 0,+1),i1,i2,i3)=c112
 coeff(m3(+1, 0,+1),i1,i2,i3)=c212
 coeff(m3(-1,+1,+1),i1,i2,i3)=c022
 coeff(m3( 0,+1,+1),i1,i2,i3)=c122
 coeff(m3(+1,+1,+1),i1,i2,i3)=c222
#endMacro

! This macro will switch the roles of x and x in the arguments
#beginMacro loopBody2ndOrder2dSwitchxx(c00,c10,c20,c01,c11,c21,c02,c12,c22)
loopBody2ndOrder2d(c00,c10,c20,c01,c11,c21,c02,c12,c22)
#endMacro

! This macro will switch the roles of x and y in the arguments
#beginMacro loopBody2ndOrder2dSwitchxy(c00,c10,c20,c01,c11,c21,c02,c12,c22)
loopBody2ndOrder2d(c00,c01,c02,c10,c11,c12,c20,c21,c22)
#endMacro

! This macro will switch the roles of x and x in the arguments
#beginMacro loopBody2ndOrder3dSwitchxx(c000,c100,c200,c010,c110,c210,c020,c120,c220,c001,c101,c201,c011,c111,c211,c021,c121,c221,c002,c102,c202,c012,c112,c212,c022,c122,c222)
loopBody2ndOrder3d(c000,c100,c200,c010,c110,c210,c020,c120,c220,c001,c101,c201,c011,c111,c211,c021,c121,c221,c002,c102,c202,c012,c112,c212,c022,c122,c222)
#endMacro

! This macro will switch the roles of x and y in the arguments
#beginMacro loopBody2ndOrder3dSwitchxy(c000,c100,c200,c010,c110,c210,c020,c120,c220,c001,c101,c201,c011,c111,c211,c021,c121,c221,c002,c102,c202,c012,c112,c212,c022,c122,c222)
loopBody2ndOrder3d(c000,c010,c020,c100,c110,c120,c200,c210,c220,c001,c011,c021,c101,c111,c121,c201,c211,c221,c002,c012,c022,c102,c112,c122,c202,c212,c222)
#endMacro

! This macro will switch the roles of x and z in the arguments
#beginMacro loopBody2ndOrder3dSwitchxz(c000,c100,c200,c010,c110,c210,c020,c120,c220,c001,c101,c201,c011,c111,c211,c021,c121,c221,c002,c102,c202,c012,c112,c212,c022,c122,c222)
loopBody2ndOrder3d(c000,c001,c002,c010,c011,c012,c020,c021,c022,c100,c101,c102,c110,c111,c112,c120,c121,c122,c200,c201,c202,c210,c211,c212,c220,c221,c222)
#endMacro

! This macro will switch the roles of y and z in the arguments
#beginMacro loopBody2ndOrder3dSwitchyz(c000,c100,c200,c010,c110,c210,c020,c120,c220,c001,c101,c201,c011,c111,c211,c021,c121,c221,c002,c102,c202,c012,c112,c212,c022,c122,c222)
loopBody2ndOrder3d(c000,c100,c200,c001,c101,c201,c002,c102,c202,c010,c110,c210,c011,c111,c211,c012,c112,c212,c020,c120,c220,c021,c121,c221,c022,c122,c222)
#endMacro


c define a macro for x, y in 2d rectangular
#beginMacro x2ndOrder2dRectangular(x,axis)
loopBody2ndOrder2dSwitchx ## x(0.,0.,0.,-h21(axis),0.,h21(axis),0.,0.,0.)
#endMacro

c define a macro for xx, yy in 2d rectangular
#beginMacro xx2ndOrder2dRectangular(x,axis)
loopBody2ndOrder2dSwitchx ## x(0.,0.,0., h22(axis),-2.*h22(axis),h22(axis), 0.,0.,0.)
#endMacro
c define a macro for x, y in 3d rectangular
#beginMacro x2ndOrder3dRectangular(x,axis)
loopBody2ndOrder3dSwitchx ## x(\
0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,-h21(axis),0.,h21(axis),0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.)
#endMacro

c define a macro for xx, yy in 3d rectangular
#beginMacro xx2ndOrder3dRectangular(x,axis)
loopBody2ndOrder3dSwitchx ## x(\
0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,h22(axis),-2.*h22(axis),h22(axis),0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.)
#endMacro

c define a macro for xy, xz, yz in 3d rectangular
c  for derivative xy use (x,x,1,2)
c  for derivative xz use (y,z,1,3)
c  for derivative yz use (x,z,2,3)
#beginMacro xy2ndOrder3dRectangular(x,y,axis1,axis2)
d=h21(axis1)*h21(axis2)
d8=d*8.
d64=d*64.
loopBody2ndOrder3dSwitch ## x ## y(\
0.,0.,0., 0.,0.,0., 0.,0.,0., d,0.,-d, 0.,0.,0., -d,0.,d, 0.,0.,0., 0.,0.,0., 0.,0.,0.)
#endMacro


c define a macro for x, y in 2d
#beginMacro x2ndOrder2d(x)
rxd = d12(1)*(r x(i1,i2,i3))
sxd = d12(2)*(s x(i1,i2,i3))
loopBody2ndOrder2d(0.,-sxd,0., -rxd,0.,rxd, 0.,sxd,0.)
#endMacro

c define a macro for xx, yy in 2d
#beginMacro xx2ndOrder2d(x)
rxSq=d22(1)*(r x(i1,i2,i3)**2)
rxx =d12(1)*(r x x 2(i1,i2,i3)) 
sxSq=d22(2)*(s x(i1,i2,i3)**2)
sxx =d12(2)*(s x x 2(i1,i2,i3))
rsx =(2.*d12(1)*d12(2))*(r x(i1,i2,i3)*s x(i1,i2,i3))
! check this
loopBody2ndOrder2d(rsx,sxSq -sxx,-rsx,rxSq-rxx,-2.*(rxSq+sxSq),rxSq+rxx,-rsx,sxSq +sxx,rsx)
#endMacro


c define a macro for x, y, z in 3d
#beginMacro x2ndOrder3d(x)
rxd = d12(1)*(r x(i1,i2,i3))
sxd = d12(2)*(s x(i1,i2,i3))
txd = d12(3)*(t x(i1,i2,i3))
loopBody2ndOrder3d(0.,0.,0.,0.,-txd,0.,0.,0.,0.,0.,-sxd,0.,-rxd,0.,rxd,0.,sxd,0.,0.,0.,0.,0.,txd,0.,0.,0.,0.)
#endMacro

c define a macro for xx, yy, zz in 3d
#beginMacro xx2ndOrder3d(x)
rxSq = d22(1)*(r x(i1,i2,i3)**2)
rxx  = d12(1)*(r x x 23(i1,i2,i3))
sxSq = d22(2)*(s x(i1,i2,i3)**2)
sxx  = d12(2)*(s x x 23(i1,i2,i3))
txSq = d22(3)*(t x(i1,i2,i3)**2)
txx  = d12(3)*(t x x 23(i1,i2,i3))
rsx  = (2.*d12(1)*d12(2))*(r x(i1,i2,i3)*s x(i1,i2,i3))
rtx  = (2.*d12(1)*d12(3))*(r x(i1,i2,i3)*t x(i1,i2,i3))
stx  = (2.*d12(2)*d12(3))*(s x(i1,i2,i3)*t x(i1,i2,i3))
loopBody2ndOrder3d(0.,stx,0.,rtx,txSq-txx,-rtx,0.,-stx,0., \
                   rsx,sxSq-sxx,-rsx,rxSq-rxx,-2.*(rxSq+sxSq+txSq),rxSq+rxx,-rsx,sxSq+sxx,rsx, \
                   0.,-stx,0.,-rtx,txSq+txx,rtx,0.,stx,0. )
#endMacro

c define a macro for xy, xz, yz in 3d
#beginMacro xy2ndOrder3d(x,y)
 rxsq = d22(1)*(r x(i1,i2,i3)*r y(i1,i2,i3))
 rxx = d12(1)*(r x y 23(i1,i2,i3))
 sxsq = d22(2)*(s x(i1,i2,i3)*s y(i1,i2,i3))
 sxx = d12(2)*(s x y 23(i1,i2,i3))
 txsq = d22(3)*(t x(i1,i2,i3)*t y(i1,i2,i3))
 txx = d12(3)*(t x y 23(i1,i2,i3))
 rsx = (d12(1)*d12(2))*(r x(i1,i2,i3)*s y(i1,i2,i3)+r y(i1,i2,i3)*s x(i1,i2,i3))
 rtx = (d12(1)*d12(3))*(r x(i1,i2,i3)*t y(i1,i2,i3)+r y(i1,i2,i3)*t x(i1,i2,i3))
 stx = (d12(2)*d12(3))*(s x(i1,i2,i3)*t y(i1,i2,i3)+s y(i1,i2,i3)*t x(i1,i2,i3))

loopBody2ndOrder3d(0.,stx,0., rtx,txSq-txx,-rtx, 0.,-stx,0., \
                   rsx,sxSq-sxx,-rsx, rxSq-rxx,-2.*(rxSq+sxSq+txSq),rxSq+rxx, -rsx,sxSq+sxx,rsx, \
                   0.,-stx,0., -rtx,txSq+txx,rtx, 0.,stx,0.)
#endMacro

#beginMacro coeffOperator2ndOrder(operator)
subroutine operator Coeff2ndOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
    n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb, ca,cb,\
    dx,dr, rsxy,coeff, gridType, order )
c ===============================================================
c  Laplacian Coefficients
c  
c  nd : number of range spatial dimensions 
c  nd1a,nd1b : mesh dimensions axis 1
c  nd2a,nd2b : mesh dimensions axis 2
c  nd3a,nd3b : mesh dimensions axis 3
c
c  ndc : number of coefficients/mesh point
c  nc1a,nd1b : coefficient array dimensions axis 1
c  nc2a,nd2b : coefficient array dimensions axis 2
c  nc3a,nd3b : coefficient array dimensions axis 3
c
c  nc1a,nd1b : subset for evaluating operator, axis 1
c  nc2a,nd2b : subset for evaluating operator, axis 2
c  nc3a,nd3b : subset for evaluating operator, axis 3
c
c  nc : number of components
c  ns : stencil size
c  ca,cb : assign components c=ca,..,cb (base 0)
c  ea,eb : assign equations e=ea,..eb   (base 0)
c
c  d11 : 1/dr
c
c  h11 : 1/h    :  for rectangular   
c
c  rsxy : jacobian information, not used if rectangular
c  coeff : coefficient matrix
c  gridType: 0=rectangular, 1=non-rectangular
c  order : 2 or 4

c nc : number of components
c ns : stencil size
c ca,cb : assign components c=ca,..,cb (base 0)
c ea,eb : assign equations e=ea,..eb   (base 0)
c gridType: 0=rectangular, 1=non-rectangular
c order : 2 or 4
c rsxy : not used if rectangular
c h22 : 1/h**2 : for rectangular  
c ===============================================================

c      implicit none
integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc,ns, ca,cb,ea,eb, gridType, order

real dx(3),dr(3)
real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,1:nd,1:nd)
real coeff(1:ndc,nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
real rx,ry,rz,sx,sy,sz,tx,ty,tz,d
real rxSq,rxx,sxSq,sxx,rsx,rxx2,ryy2,sxx2,syy2
real rxt2,ryt2,rzz23,sxt2,syt2,szz23,txr2,txs2
real txt2,tyr2,tys2,tyt2,tzz23,rzr2,rzs2,rzt2
real szr2,szs2,szt2,tzr2,tzs2,tzt2
real rxr2,rxs2,ryr2,rys2,sxr2,sxs2,syr2,sys2
real txx,txSq,rtx,stx,rxx23,ryy23,sxx23,syy23,txx23,tyy23

c..... added by kkc 1/2/02 for g77 unsatisfied reference
real u(1,1,1,1)

real h21(3),d22(3),d12(3),h22(3)
integer i1,i2,i3,kd3,kd,c,e,ec
integer m12,m22,m32
integer m(-1:1,-1:1),m3(-1:1,-1:1,-1:1)

c.......statement functions for jacobian
rx(i1,i2,i3)=rsxy(i1,i2,i3,  1,  1)
ry(i1,i2,i3)=rsxy(i1,i2,i3,  1,  2)
rz(i1,i2,i3)=rsxy(i1,i2,i3,  1,kd3)
sx(i1,i2,i3)=rsxy(i1,i2,i3,  2,  1)
sy(i1,i2,i3)=rsxy(i1,i2,i3,  2,  2)
sz(i1,i2,i3)=rsxy(i1,i2,i3,  2,kd3)
tx(i1,i2,i3)=rsxy(i1,i2,i3,kd3,  1)
ty(i1,i2,i3)=rsxy(i1,i2,i3,kd3,  2)
tz(i1,i2,i3)=rsxy(i1,i2,i3,kd3,kd3)
rxx1(i1,i2,i3)=rx(i1,i2,i3)*rxr2(i1,i2,i3)


c     u1(i1,i2,i3)=rx(i1,i2,i3)
c     u2(i1,i2,i3)=rx(i1,i2,i3)
c     ur2(i1,i2,i3)=rx(i1,i2,i3)
c     us2(i1,i2,i3)=rx(i1,i2,i3)
c     ut2(i1,i2,i3)=rx(i1,i2,i3)
      
include 'cgux2af.h'

if( order.ne.2 )then
  write(*,*) 'laplacianCoeff:ERROR: order!=2 '
  stop
end if

do n=1,3
  d12(n)=1./(2.*dr(n))
  d22(n)=1./(dr(n)**2)
  h21(n)=1./(2.*dx(n))
  h22(n)=1./(dx(n)**2)
end do


kd3=nd  

if( nd .eq. 2 )then
c       ************************
c       ******* 2D *************      
c       ************************

  #If #operator == "identity"
    loopBody2ndOrder2d(0.,0.,0., 0.,1.,0., 0.,0.,0.)
    return
  #End

  if( gridType .eq. 0 )then
c   rectangular
    beginLoops()
      #If #operator == "laplacian"
       loopBody2ndOrder2d(0.,h22(2),0., h22(1),-2.*(h22(1)+h22(2)),h22(1), 0.,h22(2),0.)
      #Elif #operator == "x"
        x2ndOrder2dRectangular(x,1)
      #Elif #operator == "y"
        x2ndOrder2dRectangular(y,2)
      #Elif #operator == "xx"
        xx2ndOrder2dRectangular(x,1)
      #Elif #operator == "yy"
        xx2ndOrder2dRectangular(y,2)
      #Elif #operator == "xy"
        d=h21(1)*h21(2)
        loopBody2ndOrder2d(d,0.,-d, 0.,0.,0., -d,0.,d)
      #End
    endLoops()
  
  else
c  ***** not rectangular *****
    beginLoops()
      #If #operator == "laplacian"
        rxSq=d22(1)*(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)
        rxx =d12(1)*(rxx2(i1,i2,i3)+ryy2(i1,i2,i3)) 
        sxSq=d22(2)*(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2)
        sxx =d12(2)*(sxx2(i1,i2,i3)+syy2(i1,i2,i3))
        rsx =(2.*d12(1)*d12(2))*(rx(i1,i2,i3)*sx(i1,i2,i3)+ry(i1,i2,i3)*sy(i1,i2,i3))
        loopBody2ndOrder2d(rsx,sxSq -sxx,-rsx,rxSq-rxx,-2.*(rxSq+sxSq),rxSq+rxx,-rsx,sxSq +sxx,rsx)
      #Elif #operator == "x"
        x2ndOrder2d(x)
      #Elif #operator == "y"
        x2ndOrder2d(y)
      #Elif #operator == "xx"
        xx2ndOrder2d(x)
      #Elif #operator == "yy"
        xx2ndOrder2d(y)
      #Elif #operator == "xy"
        rxSq = d22(1)*(rx(i1,i2,i3)*ry(i1,i2,i3))
	rxx  = d12(1)*(rxy2(i1,i2,i3))
        sxsq = d22(2)*(sx(i1,i2,i3)*sy(i1,i2,i3))
	sxx = d12(2)*(sxy2(i1,i2,i3))
	rsx = (d12(1)*d12(2))*(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))
	loopBody2ndOrder2d(rsx,sxSq-sxx,-rsx, rxSq-rxx,-2.*(rxSq+sxSq),rxSq+rxx, -rsx,sxSq+sxx,rsx)
      #End
    endLoops()
  
  endif 
elseif( nd.eq.3 )then
c       ************************
c       ******* 3D *************      
c       ************************
  
  #If #operator == "identity"
    loopBody2ndOrder3d(0.,0.,0.,0.,0.,0.,0.,0.,0., \
                       0.,0.,0.,0.,1.,0.,0.,0.,0., \
                       0.,0.,0.,0.,0.,0.,0.,0.,0. )
    return
  #End

  if( gridType .eq. 0 )then
c   rectangular
    beginLoops()
     #If #operator == "laplacian"
      loopBody2ndOrder3d(0.,0.,0.,0.,h22(3),0.,0.,0.,0., \
                         0.,h22(2),0.,h22(1),-2.*(h22(1)+h22(2)+h22(3)),h22(1),0.,h22(2),0., \
                         0.,0.,0.,0.,h22(3),0.,0.,0.,0. )
     #Elif #operator == "x"
       x2ndOrder3dRectangular(x,1)
     #Elif #operator == "y"
       x2ndOrder3dRectangular(y,2)
     #Elif #operator == "z"
       x2ndOrder3dRectangular(z,3)
     #Elif #operator == "xx"
       xx2ndOrder3dRectangular(x,1)
     #Elif #operator == "yy"
       xx2ndOrder3dRectangular(y,2)
     #Elif #operator == "zz"
       xx2ndOrder3dRectangular(z,3)
     #Elif #operator == "xy"
       xy2ndOrder3dRectangular(x,x,1,2)
     #Elif #operator == "xz"
       xy2ndOrder3dRectangular(y,z,1,3)
     #Elif #operator == "yz"
       xy2ndOrder3dRectangular(x,z,2,3)
     #End
    endLoops()
  
  else
c  ***** not rectangular *****
    beginLoops()
     #If #operator == "laplacian"
      rxSq = d22(1)*(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(i1,i2,i3)**2) 
      rxx  = d12(1)*(rxx23(i1,i2,i3)+ryy23(i1,i2,i3)+rzz23(i1,i2,i3))
      sxSq = d22(2)*(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2+sz(i1,i2,i3)**2) 
      sxx  = d12(2)*(sxx23(i1,i2,i3)+syy23(i1,i2,i3)+szz23(i1,i2,i3))
      txSq = d22(3)*(tx(i1,i2,i3)**2+ty(i1,i2,i3)**2+tz(i1,i2,i3)**2) 
      txx  = d12(3)*(txx23(i1,i2,i3)+tyy23(i1,i2,i3)+tzz23(i1,i2,i3))
      rsx  = (2.*d12(1)*d12(2))*(rx(i1,i2,i3)*sx(i1,i2,i3)+ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,i3)) 
      rtx  = (2.*d12(1)*d12(3))*(rx(i1,i2,i3)*tx(i1,i2,i3)+ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3)) 
      stx  = (2.*d12(2)*d12(3))*(sx(i1,i2,i3)*tx(i1,i2,i3)+sy(i1,i2,i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3)) 
      loopBody2ndOrder3d(0.,stx,0.,rtx,txSq-txx,-rtx,0.,-stx,0., \
                         rsx,sxSq-sxx,-rsx,rxSq-rxx,-2.*(rxSq+sxSq+txSq),rxSq+rxx,-rsx,sxSq+sxx,rsx, \
                         0.,-stx,0.,-rtx,txSq+txx,rtx,0.,stx,0. )
     #Elif #operator == "x"
        x2ndOrder3d(x)
     #Elif #operator == "y"
        x2ndOrder3d(y)
     #Elif #operator == "z"
        x2ndOrder3d(z)
     #Elif #operator == "xx"
       xx2ndOrder3d(x)
     #Elif #operator == "yy"
       xx2ndOrder3d(y)
     #Elif #operator == "zz"
       xx2ndOrder3d(z)
     #Elif #operator == "xy"
       xy2ndOrder3d(x,y)
     #Elif #operator == "xz"
       xy2ndOrder3d(x,z)
     #Elif #operator == "yz"
       xy2ndOrder3d(y,z)
     #End
    endLoops()
  
  end if
  
  
else
c       ************************
c       ******* 1D *************      
c       ************************
  #If #operator == "identity"
    loopBody2ndOrder1d(0.,1.,0.)
    return
  #End

  if( gridType .eq. 0 )then
c   rectangular
    beginLoops()
     #If #operator == "laplacian" || #operator == "xx"
      loopBody2ndOrder1d(h22(1),-2.*h22(1),h22(1))
     #Elif #operator == "x"
      loopBody2ndOrder1d(-h21(1),0.,h21(1))
     #End
    endLoops()
  else
c  ***** not rectangular *****
    beginLoops()
     #If #operator == "laplacian" || #operator == "xx"
      rxSq=d22(1)*rx(i1,i2,i3)**2
      rxx =d12(1)*rxx1(i1,i2,i3)
      loopBody2ndOrder1d(rxSq-rxx,-2.*rxSq,rxSq+rxx)
     #Elif #operator == "x"
      rxd = d12(1)*(rx(i1,i2,i3))
      loopBody2ndOrder1d(-rxd,0.,rxd)
     #End
    endLoops()
  
  end if
  
  end if

return
end

#endMacro // coeffOperator

#beginMacro buildFile(x)
#beginFile x ## Coeff2.f
 coeffOperator2ndOrder(x)
#endFile
#endMacro

      buildFile(identity)
      buildFile(laplacian)
      buildFile(x)
      buildFile(y)
      buildFile(z)
      buildFile(xx)
      buildFile(xy)
      buildFile(xz)
      buildFile(yy)
      buildFile(yz)
      buildFile(zz)




c ****************************************************************************************
c ************************* 4th order ****************************************************
c ****************************************************************************************





#beginMacro beginLoops()
c ***** loop over equations and components *****
do e=ea,eb
do c=ca,cb
ec=ns*(c+nc*e)
c ** it did not affect performance to use an array to index coeff ***
if( nd.eq.2 )then
do i2=-2,2
  do i1=-2,2
   m(i1,i2)=i1+2+5*(i2+2) +1 + ec
  end do
end do
else if( nd.eq.3 )then
do i3=-2,2
  do i2=-2,2
    do i1=-2,2
      m3(i1,i2,i3)=i1+2+5*(i2+2+5*(i3+2)) +1 + ec
    end do
  end do
end do
else
m12=1+ec 
m22=2+ec 
m32=3+ec 
m42=4+ec 
m52=5+ec 
endif

do i3=n3a,n3b
do i2=n2a,n2b
do i1=n1a,n1b
#endMacro

#beginMacro endLoops()
end do
end do
end do
end do
end do
#endMacro


#beginMacro loopBody4thOrder1d(c0,c1,c2,c3,c4)
 coeff(m12,i1,i2,i3)=c0
 coeff(m22,i1,i2,i3)=c1
 coeff(m32,i1,i2,i3)=c2
 coeff(m42,i1,i2,i3)=c3
 coeff(m52,i1,i2,i3)=c4
#endMacro

#beginMacro loopBody4thOrder2d(\
 c00,c10,c20,c30,c40,\
 c01,c11,c21,c31,c41,\
 c02,c12,c22,c32,c42,\
 c03,c13,c23,c33,c43,\
 c04,c14,c24,c34,c44)

 coeff(m(-2,-2),i1,i2,i3)=c00
 coeff(m(-1,-2),i1,i2,i3)=c10
 coeff(m( 0,-2),i1,i2,i3)=c20
 coeff(m( 1,-2),i1,i2,i3)=c30
 coeff(m( 2,-2),i1,i2,i3)=c40
                             
 coeff(m(-2,-1),i1,i2,i3)=c01
 coeff(m(-1,-1),i1,i2,i3)=c11
 coeff(m( 0,-1),i1,i2,i3)=c21
 coeff(m( 1,-1),i1,i2,i3)=c31
 coeff(m( 2,-1),i1,i2,i3)=c41
                             
 coeff(m(-2, 0),i1,i2,i3)=c02
 coeff(m(-1, 0),i1,i2,i3)=c12
 coeff(m( 0, 0),i1,i2,i3)=c22
 coeff(m(+1, 0),i1,i2,i3)=c32
 coeff(m(+2, 0),i1,i2,i3)=c42
                             
 coeff(m(-2, 1),i1,i2,i3)=c03
 coeff(m(-1, 1),i1,i2,i3)=c13
 coeff(m( 0, 1),i1,i2,i3)=c23
 coeff(m( 1, 1),i1,i2,i3)=c33
 coeff(m( 2, 1),i1,i2,i3)=c43
                             
 coeff(m(-2, 2),i1,i2,i3)=c04
 coeff(m(-1, 2),i1,i2,i3)=c14
 coeff(m( 0, 2),i1,i2,i3)=c24
 coeff(m( 1, 2),i1,i2,i3)=c34
 coeff(m( 2, 2),i1,i2,i3)=c44
                              
#endMacro

! This macro will switch the roles of x and x in the arguments
#beginMacro loopBody4thOrder2dSwitchxx(\
 c00,c10,c20,c30,c40,\
 c01,c11,c21,c31,c41,\
 c02,c12,c22,c32,c42,\
 c03,c13,c23,c33,c43,\
 c04,c14,c24,c34,c44)
loopBody4thOrder2d(\
 c00,c10,c20,c30,c40,\
 c01,c11,c21,c31,c41,\
 c02,c12,c22,c32,c42,\
 c03,c13,c23,c33,c43,\
 c04,c14,c24,c34,c44)
#endMacro

! This macro will switch the roles of x and y in the arguments
#beginMacro loopBody4thOrder2dSwitchxy(\
 c00,c10,c20,c30,c40,\
 c01,c11,c21,c31,c41,\
 c02,c12,c22,c32,c42,\
 c03,c13,c23,c33,c43,\
 c04,c14,c24,c34,c44)
loopBody4thOrder2d(\
 c00,c01,c02,c03,c04,\
 c10,c11,c12,c13,c14,\
 c20,c21,c22,c23,c24,\
 c30,c31,c32,c33,c34,\
 c40,c41,c42,c43,c44)
#endMacro


#beginMacro loopBody4thOrder3d(\
 c000,c100,c200,c300,c400,\
 c010,c110,c210,c310,c410,\
 c020,c120,c220,c320,c420,\
 c030,c130,c230,c330,c430,\
 c040,c140,c240,c340,c440,\
 c001,c101,c201,c301,c401,\
 c011,c111,c211,c311,c411,\
 c021,c121,c221,c321,c421,\
 c031,c131,c231,c331,c431,\
 c041,c141,c241,c341,c441,\
 c002,c102,c202,c302,c402,\
 c012,c112,c212,c312,c412,\
 c022,c122,c222,c322,c422,\
 c032,c132,c232,c332,c432,\
 c042,c142,c242,c342,c442,\
 c003,c103,c203,c303,c403,\
 c013,c113,c213,c313,c413,\
 c023,c123,c223,c323,c423,\
 c033,c133,c233,c333,c433,\
 c043,c143,c243,c343,c443,\
 c004,c104,c204,c304,c404,\
 c014,c114,c214,c314,c414,\
 c024,c124,c224,c324,c424,\
 c034,c134,c234,c334,c434,\
 c044,c144,c244,c344,c444)


 coeff(m3(-2,-2,-2),i1,i2,i3)=c000
 coeff(m3(-1,-2,-2),i1,i2,i3)=c100
 coeff(m3( 0,-2,-2),i1,i2,i3)=c200
 coeff(m3( 1,-2,-2),i1,i2,i3)=c300
 coeff(m3( 2,-2,-2),i1,i2,i3)=c400
 
 coeff(m3(-2,-1,-2),i1,i2,i3)=c010
 coeff(m3(-1,-1,-2),i1,i2,i3)=c110
 coeff(m3( 0,-1,-2),i1,i2,i3)=c210
 coeff(m3( 1,-1,-2),i1,i2,i3)=c310
 coeff(m3( 2,-1,-2),i1,i2,i3)=c410
                              
 coeff(m3(-2, 0,-2),i1,i2,i3)=c020
 coeff(m3(-1, 0,-2),i1,i2,i3)=c120
 coeff(m3( 0, 0,-2),i1,i2,i3)=c220
 coeff(m3(+1, 0,-2),i1,i2,i3)=c320
 coeff(m3(+2, 0,-2),i1,i2,i3)=c420
                              
 coeff(m3(-2, 1,-2),i1,i2,i3)=c030
 coeff(m3(-1, 1,-2),i1,i2,i3)=c130
 coeff(m3( 0, 1,-2),i1,i2,i3)=c230
 coeff(m3( 1, 1,-2),i1,i2,i3)=c330
 coeff(m3( 2, 1,-2),i1,i2,i3)=c430
                              
 coeff(m3(-2, 2,-2),i1,i2,i3)=c040
 coeff(m3(-1, 2,-2),i1,i2,i3)=c140
 coeff(m3( 0, 2,-2),i1,i2,i3)=c240
 coeff(m3( 1, 2,-2),i1,i2,i3)=c340
 coeff(m3( 2, 2,-2),i1,i2,i3)=c440
                              
                              
 coeff(m3(-2,-2,-1),i1,i2,i3)=c001
 coeff(m3(-1,-2,-1),i1,i2,i3)=c101
 coeff(m3( 0,-2,-1),i1,i2,i3)=c201
 coeff(m3(+1,-2,-1),i1,i2,i3)=c301
 coeff(m3(+2,-2,-1),i1,i2,i3)=c401
                              
 coeff(m3(-2,-1,-1),i1,i2,i3)=c011
 coeff(m3(-1,-1,-1),i1,i2,i3)=c111
 coeff(m3( 0,-1,-1),i1,i2,i3)=c211
 coeff(m3(+1,-1,-1),i1,i2,i3)=c311
 coeff(m3(+2,-1,-1),i1,i2,i3)=c411
                              
 coeff(m3(-2, 0,-1),i1,i2,i3)=c021
 coeff(m3(-1, 0,-1),i1,i2,i3)=c121
 coeff(m3( 0, 0,-1),i1,i2,i3)=c221
 coeff(m3(+1, 0,-1),i1,i2,i3)=c321
 coeff(m3(+2, 0,-1),i1,i2,i3)=c421
                              
 coeff(m3(-2,+1,-1),i1,i2,i3)=c031
 coeff(m3(-1,+1,-1),i1,i2,i3)=c131
 coeff(m3( 0,+1,-1),i1,i2,i3)=c231
 coeff(m3(+1,+1,-1),i1,i2,i3)=c331
 coeff(m3(+2,+1,-1),i1,i2,i3)=c431
                              
 coeff(m3(-2,+2,-1),i1,i2,i3)=c041
 coeff(m3(-1,+2,-1),i1,i2,i3)=c141
 coeff(m3( 0,+2,-1),i1,i2,i3)=c241
 coeff(m3(+1,+2,-1),i1,i2,i3)=c341
 coeff(m3(+2,+2,-1),i1,i2,i3)=c441
                              
                              
 coeff(m3(-2,-2, 0),i1,i2,i3)=c002
 coeff(m3(-1,-2, 0),i1,i2,i3)=c102
 coeff(m3( 0,-2, 0),i1,i2,i3)=c202
 coeff(m3(+1,-2, 0),i1,i2,i3)=c302
 coeff(m3(+2,-2, 0),i1,i2,i3)=c402
                                 
 coeff(m3(-2,-1, 0),i1,i2,i3)=c012
 coeff(m3(-1,-1, 0),i1,i2,i3)=c112
 coeff(m3( 0,-1, 0),i1,i2,i3)=c212
 coeff(m3(+1,-1, 0),i1,i2,i3)=c312
 coeff(m3(+2,-1, 0),i1,i2,i3)=c412
                                 
 coeff(m3(-2, 0, 0),i1,i2,i3)=c022
 coeff(m3(-1, 0, 0),i1,i2,i3)=c122
 coeff(m3( 0, 0, 0),i1,i2,i3)=c222
 coeff(m3(+1, 0, 0),i1,i2,i3)=c322
 coeff(m3(+2, 0, 0),i1,i2,i3)=c422
                                 
 coeff(m3(-2, 1, 0),i1,i2,i3)=c032
 coeff(m3(-1, 1, 0),i1,i2,i3)=c132
 coeff(m3( 0, 1, 0),i1,i2,i3)=c232
 coeff(m3(+1, 1, 0),i1,i2,i3)=c332
 coeff(m3(+2, 1, 0),i1,i2,i3)=c432
                                 
 coeff(m3(-2, 2, 0),i1,i2,i3)=c042
 coeff(m3(-1, 2, 0),i1,i2,i3)=c142
 coeff(m3( 0, 2, 0),i1,i2,i3)=c242
 coeff(m3(+1, 2, 0),i1,i2,i3)=c342
 coeff(m3(+2, 2, 0),i1,i2,i3)=c442
                                 
                                 
 coeff(m3(-2,-2, 1),i1,i2,i3)=c003
 coeff(m3(-1,-2, 1),i1,i2,i3)=c103
 coeff(m3( 0,-2, 1),i1,i2,i3)=c203
 coeff(m3(+1,-2, 1),i1,i2,i3)=c303
 coeff(m3(+2,-2, 1),i1,i2,i3)=c403
                                 
 coeff(m3(-2,-1, 1),i1,i2,i3)=c013
 coeff(m3(-1,-1, 1),i1,i2,i3)=c113
 coeff(m3( 0,-1, 1),i1,i2,i3)=c213
 coeff(m3(+1,-1, 1),i1,i2,i3)=c313
 coeff(m3(+2,-1, 1),i1,i2,i3)=c413
                                 
 coeff(m3(-2, 0, 1),i1,i2,i3)=c023
 coeff(m3(-1, 0, 1),i1,i2,i3)=c123
 coeff(m3( 0, 0, 1),i1,i2,i3)=c223
 coeff(m3(+1, 0, 1),i1,i2,i3)=c323
 coeff(m3(+2, 0, 1),i1,i2,i3)=c423
                                 
 coeff(m3(-2, 1, 1),i1,i2,i3)=c033
 coeff(m3(-1, 1, 1),i1,i2,i3)=c133
 coeff(m3( 0, 1, 1),i1,i2,i3)=c233
 coeff(m3(+1, 1, 1),i1,i2,i3)=c333
 coeff(m3(+2, 1, 1),i1,i2,i3)=c433
                                 
 coeff(m3(-2, 2, 1),i1,i2,i3)=c043
 coeff(m3(-1, 2, 1),i1,i2,i3)=c143
 coeff(m3( 0, 2, 1),i1,i2,i3)=c243
 coeff(m3(+1, 2, 1),i1,i2,i3)=c343
 coeff(m3(+2, 2, 1),i1,i2,i3)=c443
                              
 coeff(m3(-2,-2, 2),i1,i2,i3)=c004
 coeff(m3(-1,-2, 2),i1,i2,i3)=c104
 coeff(m3( 0,-2, 2),i1,i2,i3)=c204
 coeff(m3(+1,-2, 2),i1,i2,i3)=c304
 coeff(m3(+2,-2, 2),i1,i2,i3)=c404
                                 
 coeff(m3(-2,-1, 2),i1,i2,i3)=c014
 coeff(m3(-1,-1, 2),i1,i2,i3)=c114
 coeff(m3( 0,-1, 2),i1,i2,i3)=c214
 coeff(m3(+1,-1, 2),i1,i2,i3)=c314
 coeff(m3(+2,-1, 2),i1,i2,i3)=c414
                                 
 coeff(m3(-2, 0, 2),i1,i2,i3)=c024
 coeff(m3(-1, 0, 2),i1,i2,i3)=c124
 coeff(m3( 0, 0, 2),i1,i2,i3)=c224
 coeff(m3(+1, 0, 2),i1,i2,i3)=c324
 coeff(m3(+2, 0, 2),i1,i2,i3)=c424
                                 
 coeff(m3(-2,+1, 2),i1,i2,i3)=c034
 coeff(m3(-1,+1, 2),i1,i2,i3)=c134
 coeff(m3( 0,+1, 2),i1,i2,i3)=c234
 coeff(m3(+1,+1, 2),i1,i2,i3)=c334
 coeff(m3(+2,+1, 2),i1,i2,i3)=c434
                                 
 coeff(m3(-2,+2, 2),i1,i2,i3)=c044
 coeff(m3(-1,+2, 2),i1,i2,i3)=c144
 coeff(m3( 0,+2, 2),i1,i2,i3)=c244
 coeff(m3(+1,+2, 2),i1,i2,i3)=c344
 coeff(m3(+2,+2, 2),i1,i2,i3)=c444
#endMacro

! This macro will switch the roles of x and x in the arguments
#beginMacro loopBody4thOrder3dSwitchxx(\
 c000,c100,c200,c300,c400, c010,c110,c210,c310,c410, c020,c120,c220,c320,c420, c030,c130,c230,c330,c430,\
 c040,c140,c240,c340,c440, c001,c101,c201,c301,c401, c011,c111,c211,c311,c411, c021,c121,c221,c321,c421,\
 c031,c131,c231,c331,c431, c041,c141,c241,c341,c441, c002,c102,c202,c302,c402, c012,c112,c212,c312,c412,\
 c022,c122,c222,c322,c422, c032,c132,c232,c332,c432, c042,c142,c242,c342,c442, c003,c103,c203,c303,c403,\
 c013,c113,c213,c313,c413, c023,c123,c223,c323,c423, c033,c133,c233,c333,c433, c043,c143,c243,c343,c443,\
 c004,c104,c204,c304,c404, c014,c114,c214,c314,c414, c024,c124,c224,c324,c424, c034,c134,c234,c334,c434,\
 c044,c144,c244,c344,c444)
loopBody4thOrder3d(\
 c000,c100,c200,c300,c400, c010,c110,c210,c310,c410, c020,c120,c220,c320,c420, c030,c130,c230,c330,c430,\
 c040,c140,c240,c340,c440, c001,c101,c201,c301,c401, c011,c111,c211,c311,c411, c021,c121,c221,c321,c421,\
 c031,c131,c231,c331,c431, c041,c141,c241,c341,c441, c002,c102,c202,c302,c402, c012,c112,c212,c312,c412,\
 c022,c122,c222,c322,c422, c032,c132,c232,c332,c432, c042,c142,c242,c342,c442, c003,c103,c203,c303,c403,\
 c013,c113,c213,c313,c413, c023,c123,c223,c323,c423, c033,c133,c233,c333,c433, c043,c143,c243,c343,c443,\
 c004,c104,c204,c304,c404, c014,c114,c214,c314,c414, c024,c124,c224,c324,c424, c034,c134,c234,c334,c434,\
 c044,c144,c244,c344,c444)
#endMacro
! This macro will switch the roles of x and y in the arguments
#beginMacro loopBody4thOrder3dSwitchxy(\
 c000,c100,c200,c300,c400, c010,c110,c210,c310,c410, c020,c120,c220,c320,c420, c030,c130,c230,c330,c430,\
 c040,c140,c240,c340,c440, c001,c101,c201,c301,c401, c011,c111,c211,c311,c411, c021,c121,c221,c321,c421,\
 c031,c131,c231,c331,c431, c041,c141,c241,c341,c441, c002,c102,c202,c302,c402, c012,c112,c212,c312,c412,\
 c022,c122,c222,c322,c422, c032,c132,c232,c332,c432, c042,c142,c242,c342,c442, c003,c103,c203,c303,c403,\
 c013,c113,c213,c313,c413, c023,c123,c223,c323,c423, c033,c133,c233,c333,c433, c043,c143,c243,c343,c443,\
 c004,c104,c204,c304,c404, c014,c114,c214,c314,c414, c024,c124,c224,c324,c424, c034,c134,c234,c334,c434,\
 c044,c144,c244,c344,c444)
loopBody4thOrder3d(\
 c000,c010,c020,c030,c040, c100,c110,c120,c130,c140, c200,c210,c220,c230,c240, c300,c310,c320,c330,c340,\
 c400,c410,c420,c430,c440, c001,c011,c021,c031,c041, c101,c111,c121,c131,c141, c201,c211,c221,c231,c241,\
 c301,c311,c321,c331,c341, c401,c411,c421,c431,c441, c002,c012,c022,c032,c042, c102,c112,c122,c132,c142,\
 c202,c212,c222,c232,c242, c302,c312,c322,c332,c342, c402,c412,c422,c432,c442, c003,c013,c023,c033,c043,\
 c103,c113,c123,c133,c143, c203,c213,c223,c233,c243, c303,c313,c323,c333,c343, c403,c413,c423,c433,c443,\
 c004,c014,c024,c034,c044, c104,c114,c124,c134,c144, c204,c214,c224,c234,c244, c304,c314,c324,c334,c344,\
 c404,c414,c424,c434,c444)
#endMacro
! This macro will switch the roles of x and z in the arguments
#beginMacro loopBody4thOrder3dSwitchxz(\
 c000,c100,c200,c300,c400, c010,c110,c210,c310,c410, c020,c120,c220,c320,c420, c030,c130,c230,c330,c430,\
 c040,c140,c240,c340,c440, c001,c101,c201,c301,c401, c011,c111,c211,c311,c411, c021,c121,c221,c321,c421,\
 c031,c131,c231,c331,c431, c041,c141,c241,c341,c441, c002,c102,c202,c302,c402, c012,c112,c212,c312,c412,\
 c022,c122,c222,c322,c422, c032,c132,c232,c332,c432, c042,c142,c242,c342,c442, c003,c103,c203,c303,c403,\
 c013,c113,c213,c313,c413, c023,c123,c223,c323,c423, c033,c133,c233,c333,c433, c043,c143,c243,c343,c443,\
 c004,c104,c204,c304,c404, c014,c114,c214,c314,c414, c024,c124,c224,c324,c424, c034,c134,c234,c334,c434,\
 c044,c144,c244,c344,c444)
loopBody4thOrder3d(\
 c000,c001,c002,c003,c004, c010,c011,c012,c013,c014, c020,c021,c022,c023,c024, c030,c031,c032,c033,c034,\
 c040,c041,c042,c043,c044, c100,c101,c102,c103,c104, c110,c111,c112,c113,c114, c120,c121,c122,c123,c124,\
 c130,c131,c132,c133,c134, c140,c141,c142,c143,c144, c200,c201,c202,c203,c204, c210,c211,c212,c213,c214,\
 c220,c221,c222,c223,c224, c230,c231,c232,c233,c234, c240,c241,c242,c243,c244, c300,c301,c302,c303,c304,\
 c310,c311,c312,c313,c314, c320,c321,c322,c323,c324, c330,c331,c332,c333,c334, c340,c341,c342,c343,c344,\
 c400,c401,c402,c403,c404, c410,c411,c412,c413,c414, c420,c421,c422,c423,c424, c430,c431,c432,c433,c434,\
 c440,c441,c442,c443,c444)
#endMacro
! This macro will switch the roles of y and z in the arguments
#beginMacro loopBody4thOrder3dSwitchyz(\
 c000,c100,c200,c300,c400, c010,c110,c210,c310,c410, c020,c120,c220,c320,c420, c030,c130,c230,c330,c430,\
 c040,c140,c240,c340,c440, c001,c101,c201,c301,c401, c011,c111,c211,c311,c411, c021,c121,c221,c321,c421,\
 c031,c131,c231,c331,c431, c041,c141,c241,c341,c441, c002,c102,c202,c302,c402, c012,c112,c212,c312,c412,\
 c022,c122,c222,c322,c422, c032,c132,c232,c332,c432, c042,c142,c242,c342,c442, c003,c103,c203,c303,c403,\
 c013,c113,c213,c313,c413, c023,c123,c223,c323,c423, c033,c133,c233,c333,c433, c043,c143,c243,c343,c443,\
 c004,c104,c204,c304,c404, c014,c114,c214,c314,c414, c024,c124,c224,c324,c424, c034,c134,c234,c334,c434,\
 c044,c144,c244,c344,c444)
loopBody4thOrder3d(\
 c000,c100,c200,c300,c400, c001,c101,c201,c301,c401, c002,c102,c202,c302,c402, c003,c103,c203,c303,c403,\
 c004,c104,c204,c304,c404, c010,c110,c210,c310,c410, c011,c111,c211,c311,c411, c012,c112,c212,c312,c412,\
 c013,c113,c213,c313,c413, c014,c114,c214,c314,c414, c020,c120,c220,c320,c420, c021,c121,c221,c321,c421,\
 c022,c122,c222,c322,c422, c023,c123,c223,c323,c423, c024,c124,c224,c324,c424, c030,c130,c230,c330,c430,\
 c031,c131,c231,c331,c431, c032,c132,c232,c332,c432, c033,c133,c233,c333,c433, c034,c134,c234,c334,c434,\
 c040,c140,c240,c340,c440, c041,c141,c241,c341,c441, c042,c142,c242,c342,c442, c043,c143,c243,c343,c443,\
 c044,c144,c244,c344,c444)
#endMacro

c define a macro for x, y in 2d rectangular
#beginMacro x4thOrder2dRectangular(x,axis)
loopBody4thOrder2dSwitchx ## x(0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
                          h41(axis),-8.*h41(axis),0.,8.* h41(axis),-h41(axis),\
                          0.,0.,0.,0.,0., 0.,0.,0.,0.,0.) 
#endMacro

c define a macro for xx, yy in 2d rectangular
#beginMacro xx4thOrder2dRectangular(x,axis)
loopBody4thOrder2dSwitchx ## x(0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
                          -h42(axis),16.* h42(axis),-30.*h42(axis),16.* h42(axis),-h42(axis),\
                          0.,0.,0.,0.,0., 0.,0.,0.,0.,0.) 
#endMacro

c define a macro for x, y in 2d
#beginMacro x4thOrder2d(x)
rx4 = d14(1)*(r x(i1,i2,i3))
sx4 = d14(2)*(s x(i1,i2,i3))
tx4 = d14(3)*(t x(i1,i2,i3))
loopBody4thOrder2d(\
   0.,0.,sx4,0.,0., 0.,0.,-8.*sx4,0.,0., rx4,-8.*rx4,0.,8.*rx4,-rx4, 0.,0.,8.*sx4,0.,0., 0.,0.,-sx4,0.,0.)
#endMacro

c define a macro for xx, yy in 2d
#beginMacro xx4thOrder2d(x)
rxSq=d24(1)*(rx(i1,i2,i3)**2)
rxxyy=d14(1)*(rxx(i1,i2,i3))
sxSq=d24(2)*(sx(i1,i2,i3)**2)
sxxyy=d14(2)*(sxx(i1,i2,i3))
rsx =(2.*d14(1)*d14(2))*(rx(i1,i2,i3)*sx(i1,i2,i3))
rsx8  = rsx*8. 
rsx64 = rsx*64.

loopBody4thOrder2d(rsx,-rsx8,-sxSq+sxxyy,rsx8,-rsx,\
                   -rsx8,rsx64,16.*sxSq -8.*sxxyy,-  rsx64,rsx8,\
                   -rxSq+rxxyy,16.*rxSq-8.*rxxyy,-30.*(rxSq+sxSq),16.*rxSq+8.*rxxyy,-rxSq-rxxyy,\
                   rsx8,-rsx64,16.*sxSq +8.*sxxyy,rsx64,-rsx8,\
                   -rsx,rsx8,-sxSq-sxxyy,-rsx8,rsx)
#endMacro


c define a macro for x, y in 3d rectangular
#beginMacro x4thOrder3dRectangular(x,axis)
loopBody4thOrder3dSwitchx ## x(\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., h41(axis),-8.*h41(axis),0.,8.* h41(axis),-h41(axis),\
                                               0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.)
#endMacro

c define a macro for xx, yy in 3d rectangular
#beginMacro xx4thOrder3dRectangular(x,axis)
loopBody4thOrder3dSwitchx ## x(\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., -h42(axis),16.* h42(axis),-30.*h42(axis),16.* h42(axis),-h42(axis),\
                                               0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.)
#endMacro

c define a macro for xy, xz, yz in 3d rectangular
c  for derivative xy use (x,x,1,2)
c  for derivative xz use (y,z,1,3)
c  for derivative yz use (x,z,2,3)
#beginMacro xy4thOrder3dRectangular(x,y,axis1,axis2)
d=h41(axis1)*h41(axis2)
d8=d*8.
d64=d*64.
loopBody4thOrder3dSwitch ## x ## y(\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         d,-d8,0.,d8,-d, -d8,d64,0.,-d64,d8, 0.,0.,0.,0.,0., d8,-d64,0.,d64,-d8, -d,d8,0.,-d8,d,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.)
#endMacro

c define a macro for x, y, z in 3d
#beginMacro x4thOrder3d(x)
rx4 = d14(1)*(r x(i1,i2,i3))
sx4 = d14(2)*(s x(i1,i2,i3))
tx4 = d14(3)*(t x(i1,i2,i3))
loopBody4thOrder3d(\
   0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,tx4,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
   0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,-8.*tx4,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
   0.,0.,sx4,0.,0., 0.,0.,-8.*sx4,0.,0., rx4,-8.*rx4,0.,8.*rx4,-rx4, 0.,0.,8.*sx4,0.,0., 0.,0.,-sx4,0.,0.,\
   0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,8.*tx4,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
   0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,-tx4,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.)
#endMacro


c define a macro for xx, yy, zz in 3d
#beginMacro xx4thOrder3d(x)
rxSq = d24(1)*(r x(i1,i2,i3)**2)
rxxyy = d14(1)*(r x x 3(i1,i2,i3))
sxSq = d24(2)*(s x(i1,i2,i3)**2)
sxxyy = d14(2)*(s x x 3(i1,i2,i3))
txSq = d24(3)*(t x(i1,i2,i3)**2)
txxyy = d14(3)*(t x x 3(i1,i2,i3))
rsx  = (2.*d14(1)*d14(2))*(r x(i1,i2,i3)*s x(i1,i2,i3))
rtx  = (2.*d14(1)*d14(3))*(r x(i1,i2,i3)*t x(i1,i2,i3))
stx  = (2.*d14(2)*d14(3))*(s x(i1,i2,i3)*t x(i1,i2,i3))
rsx8  = rsx*8. 
rsx64 = rsx*64.
rtx8  = rtx*8. 
rtx64 = rtx*64.
stx8  = stx*8. 
stx64 = stx*64.
loopBody4thOrder3d(\
    0.,0.,stx,0.,0., 0.,0.,-stx8,0.,0., rtx,-rtx8,-txSq+txxyy,rtx8,-rtx, 0.,0.,stx8,0.,0., 0.,0.,-stx,0.,0.,\
    0.,0.,-stx8,0.,0., 0.,0.,stx64,0.,0., -rtx8,rtx64,16.*txSq-8.*txxyy,-rtx64,rtx8, \
                                                                   0.,0.,-stx64,0.,0., 0.,0.,stx8,0.,0.,\
        rsx,-rsx8,-sxSq+sxxyy,rsx8,-rsx,\
              -rsx8,rsx64,16.*sxSq-8.*sxxyy,-rsx64,rsx8, \
                -rxSq+rxxyy,16.*rxSq-8.*rxxyy,-30.*(rxSq+sxSq+txSq),16.*rxSq+8.*rxxyy,-rxSq-rxxyy, \
                     rsx8,-rsx64,16.*sxSq+8.*sxxyy,rsx64,-rsx8, \
                       -rsx,rsx8,-sxSq-sxxyy,-rsx8,rsx,\
    0.,0.,stx8,0.,0., 0.,0.,-stx64,0.,0., rtx8,-rtx64,16.*txSq+8.*txxyy,rtx64,-rtx8, \
                                                                   0.,0.,stx64,0.,0., 0.,0.,-stx8,0.,0.,\
    0.,0.,-stx,0.,0., 0.,0.,stx8,0.,0., -rtx,rtx8,-txSq-txxyy,-rtx8,rtx, 0.,0.,-stx8,0.,0., 0.,0.,stx,0.,0.)
#endMacro

#beginMacro xy4thOrder3d(x,y)
rxsq = d24(1)*(r x(i1,i2,i3)*r y(i1,i2,i3))
rxxyy=d14(1)*(r x y 3(i1,i2,i3))
sxsq = d24(2)*(s x(i1,i2,i3)*s y(i1,i2,i3))
sxxyy=d14(2)*(s x y 3(i1,i2,i3))
txsq = d24(3)*(t x(i1,i2,i3)*t y(i1,i2,i3))
txxyy=d14(3)*(t x y 3(i1,i2,i3))
rsx = (d14(1)*d14(2))*(r x(i1,i2,i3)*s y(i1,i2,i3)+r y(i1,i2,i3)*s x(i1,i2,i3))
rtx = (d14(1)*d14(3))*(r x(i1,i2,i3)*t y(i1,i2,i3)+r y(i1,i2,i3)*t x(i1,i2,i3))
stx = (d14(2)*d14(3))*(s x(i1,i2,i3)*t y(i1,i2,i3)+s y(i1,i2,i3)*t x(i1,i2,i3))
! check this	
      loopBody4thOrder3d(\
     0.,0.,stx,0.,0., 0.,0.,-8.*stx,0.,0., rtx,-8.*rtx,-txSq+txxyy,8.*rtx,-rtx, 0.,0.,8.*stx,0.,0., 0.,0.,-stx,0.,0.,\
     0.,0.,-8.*stx,0.,0., 0.,64.*stx,0.,0.,0., -8.*rtx,64.*rtx,16.*txSq-8.*txxyy,-64.*rtx,8.*rtx, \
                                                            0.,0.,-64.*stx,0.,0., 0.,0.,8.*stx,0.,0.,\
         rsx,-8.*rsx,-sxSq+sxxyy,8.*rsx,-rsx, -8.*rsx,64.*rsx,16.*sxSq-8.*sxxyy,-64.*rsx,8.*rsx, \
                      -rxSq+rxxyy,16.*rxSq-8.*rxxyy,-30.*(rxSq+sxSq+txSq),16.*rxSq+8.*rxxyy,-rxSq-rxxyy,\
                           8.*rsx,-64.*rsx,16.*sxSq+8.*sxxyy,64.*rsx,-8.*rsx, -rsx,8.*rsx,-sxSq-sxxyy,-8.*rsx,rsx,\
     0.,0.,8.*stx,0.,0., 0.,0.,-64.*stx,0.,0., 8.*rtx,-64.*rtx,16.*txSq+8.*txxyy,64.*rtx,-8.*rtx, \
                                                                0.,0.,64.*stx,0.,0., 0.,0.,8.*stx,0.,0.,\
     0.,0.,-stx,0.,0., 0.,0.,8.*stx,0.,0., -rtx,8.*rtx,-txSq-txxyy,-8.*rtx,rtx, 0.,0.,-8.*stx,0.,0., 0.,0.,stx,0.,0.)
#endMacro


#beginMacro coeffOperator4thOrder(operator)
subroutine operator Coeff4thOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,n1a,n1b,n2a,n2b,n3a,n3b, ndc,nc,ns,ea,eb,ca,cb,\
dx,dr, rsxy,coeff, gridType, order )
c ===============================================================
c  Coefficients - 4th order version
c  
c gridType: 0=rectangular, 1=non-rectangular
c rsxy : not used if rectangular
c h42 : 1/h**2 : for rectangular  
c ===============================================================

c      implicit none
integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb, ca,cb,gridType,order

real dx(3),dr(3)
real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,1:nd,1:nd)
real coeff(1:ndc,nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
real rx,ry,rz,sx,sy,sz,tx,ty,tz,d,d8,d64
real rxSq,rxxyy,sxSq,sxxyy,txxyy,txSq
real rxx,ryy,sxx,syy,rxx3,ryy3,rzz3,sxx3,syy3,szz3,txx3,tyy3,tzz3
real rsx,rtx,stx
real rxt,ryt,sxt,syt,txr,txs
real txt,tyr,tys,tyt,rzr,rzs,rzt
real szr,szs,szt,tzr,tzs,tzt
real rxr,rxs,ryr,rys,sxr,sxs,syr,sys
real rsx8,rsx64,rtx8,rtx64,stx8,stx64

c..... added by kkc 1/2/02 for g77 unsatisfied reference
real u(1,1,1,1)

real d24(3),d14(3),h42(3),h41(3)
integer i1,i2,i3,kd3,kd,kdd,e,c,ec,j
integer m12,m22,m32,m42,m52

integer m(-2:2,-2:2),m3(-2:2,-2:2,-2:2)

c....statement functions for jacobian
rx(i1,i2,i3)=rsxy(i1,i2,i3,  1,  1)
ry(i1,i2,i3)=rsxy(i1,i2,i3,  1,  2)
rz(i1,i2,i3)=rsxy(i1,i2,i3,  1,kd3)
sx(i1,i2,i3)=rsxy(i1,i2,i3,  2,  1)
sy(i1,i2,i3)=rsxy(i1,i2,i3,  2,  2)
sz(i1,i2,i3)=rsxy(i1,i2,i3,  2,kd3)
tx(i1,i2,i3)=rsxy(i1,i2,i3,kd3,  1)
ty(i1,i2,i3)=rsxy(i1,i2,i3,kd3,  2)
tz(i1,i2,i3)=rsxy(i1,i2,i3,kd3,kd3)
rxx1(i1,i2,i3)=rx(i1,i2,i3)*rxr(i1,i2,i3)

include 'cgux4af.h'


if( order.ne.4 )then
  write(*,*) 'laplacianCoeff4:ERROR: order!=4 '
  stop
end if

do n=1,3
  d14(n)=1./(12.*dr(n))
  d24(n)=1./(12.*dr(n)**2)
  h41(n)=1./(12.*dx(n))
  h42(n)=1./(12.*dx(n)**2)
end do

kd3=nd  

if( nd .eq. 2 )then
c       ************************
c       ******* 2D *************      
c       ************************
  #If #operator == "identity"
    loopBody4thOrder2d(0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,1.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.)
    return
  #End

  if( gridType .eq. 0 )then
c   rectangular
    beginLoops()
      #If #operator == "laplacian"
       loopBody4thOrder2d(0.,0.,-h42(2),0.,0., 0.,0.,16.*h42(2),0.,0.,\
                          -h42(1),16.* h42(1),-30.*(h42(1)+h42(2)),16.* h42(1),-h42(1),\
                          0.,0.,16.*h42(2),0.,0., 0.,0.,-h42(2),0.,0.) 
      #Elif #operator == "x"
        x4thOrder2dRectangular(x,1)
      #Elif #operator == "y"
        x4thOrder2dRectangular(y,2)
      #Elif #operator == "xx"
        xx4thOrder2dRectangular(x,1)
      #Elif #operator == "yy"
        xx4thOrder2dRectangular(y,2)
      #Elif #operator == "xy"
        d=h41(axis1)*h41(axis2)
        d8=d*8.
        d64=d*64.
        loopBody4thOrder2d(d,-d8,0.,d8,-d, -d8,d64,0.,-d64,d8, 0.,0.,0.,0.,0., d8,-d64,0.,d64,-d8, -d,d8,0.,-d8,d)
      #End
    endLoops()
  
  else
c  ***** not rectangular *****
    beginLoops()
      #If #operator == "laplacian"
       rxSq=d24(1)*(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)
       rxxyy=d14(1)*(rxx(i1,i2,i3)+ryy(i1,i2,i3)) 
       sxSq=d24(2)*(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2)
       sxxyy=d14(2)*(sxx(i1,i2,i3)+syy(i1,i2,i3))
       rsx =(2.*d14(1)*d14(2))*(rx(i1,i2,i3)*sx(i1,i2,i3)+ry(i1,i2,i3)*sy(i1,i2,i3))
       rsx8  = rsx*8. 
       rsx64 = rsx*64.
       loopBody4thOrder2d(rsx,-rsx8,-sxSq+sxxyy,rsx8,-rsx,\
                          -rsx8,rsx64,16.*sxSq -8.*sxxyy,-  rsx64,rsx8,\
                          -rxSq+rxxyy,16.*rxSq-8.*rxxyy,-30.*(rxSq+sxSq),16.*rxSq+8.*rxxyy,-rxSq-rxxyy,\
                          rsx8,-rsx64,16.*sxSq +8.*sxxyy,rsx64,-rsx8,\
                          -rsx,rsx8,-sxSq-sxxyy,-rsx8,rsx)
      #Elif #operator == "x"
        x4thOrder2d(x)
      #Elif #operator == "y"
        x4thOrder2d(y)
      #Elif #operator == "xx"
        xx4thOrder2d(x)
      #Elif #operator == "yy"
        xx4thOrder2d(y)
      #End
    endLoops()
  
  endif 
elseif( nd.eq.3 )then
c       ************************
c       ******* 3D *************      
c       ************************
  
  #If #operator == "identity"
    loopBody4thOrder3d(\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,1.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.)
    return
  #End
  if( gridType .eq. 0 )then
c   rectangular
    beginLoops()
     #If #operator == "laplacian"
      loopBody4thOrder3d(\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,-h42(3),0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,16.*h42(3),0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,-h42(2),0.,0., 0.,0.,16.*h42(2),0.,0., \
                       -h42(1),16.* h42(1),-30.*(h42(1)+h42(2)+h42(3)),16.* h42(1),-h42(1), \
                                               0.,0.,16.*h42(2),0.,0., 0.,0.,-h42(2),0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,16.*h42(3),0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,-h42(3),0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.)
     #Elif #operator == "x"
       x4thOrder3dRectangular(x,1)
     #Elif #operator == "y"
       x4thOrder3dRectangular(y,2)
     #Elif #operator == "z"
       x4thOrder3dRectangular(z,3)
     #Elif #operator == "xx"
       x4thOrder3dRectangular(x,1)
     #Elif #operator == "yy"
       x4thOrder3dRectangular(y,2)
     #Elif #operator == "zz"
       x4thOrder3dRectangular(z,3)
     #Elif #operator == "xy"
       xy4thOrder3dRectangular(x,x,1,2)
     #Elif #operator == "xz"
       xy4thOrder3dRectangular(y,z,1,3)
     #Elif #operator == "yz"
       xy4thOrder3dRectangular(x,z,2,3)
     #End
    endLoops()
  
  else
c  ***** not rectangular *****
    beginLoops()
     #If #operator == "laplacian"
      rxSq = d24(1)*(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(i1,i2,i3)**2) 
      rxxyy = d14(1)*(rxx3(i1,i2,i3)+ryy3(i1,i2,i3)+rzz3(i1,i2,i3))
      sxSq = d24(2)*(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2+sz(i1,i2,i3)**2) 
      sxxyy = d14(2)*(sxx3(i1,i2,i3)+syy3(i1,i2,i3)+szz3(i1,i2,i3))
      txSq = d24(3)*(tx(i1,i2,i3)**2+ty(i1,i2,i3)**2+tz(i1,i2,i3)**2) 
      txxyy = d14(3)*(txx3(i1,i2,i3)+tyy3(i1,i2,i3)+tzz3(i1,i2,i3))
      rsx  = (2.*d14(1)*d14(2))*(rx(i1,i2,i3)*sx(i1,i2,i3)+ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,i3)) 
      rtx  = (2.*d14(1)*d14(3))*(rx(i1,i2,i3)*tx(i1,i2,i3)+ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3)) 
      stx  = (2.*d14(2)*d14(3))*(sx(i1,i2,i3)*tx(i1,i2,i3)+sy(i1,i2,i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3)) 
      rsx8  = rsx*8. 
      rsx64 = rsx*64.
      rtx8  = rtx*8. 
      rtx64 = rtx*64.
      stx8  = stx*8. 
      stx64 = stx*64.
      loopBody4thOrder3d(\
          0.,0.,stx,0.,0., 0.,0.,-stx8,0.,0., rtx,-rtx8,-txSq+txxyy,rtx8,-rtx, 0.,0.,stx8,0.,0., 0.,0.,-stx,0.,0.,\
          0.,0.,-stx8,0.,0., 0.,0.,stx64,0.,0., -rtx8,rtx64,16.*txSq-8.*txxyy,-rtx64,rtx8, \
                                                                         0.,0.,-stx64,0.,0., 0.,0.,stx8,0.,0.,\
              rsx,-rsx8,-sxSq+sxxyy,rsx8,-rsx,\
                    -rsx8,rsx64,16.*sxSq-8.*sxxyy,-rsx64,rsx8, \
                      -rxSq+rxxyy,16.*rxSq-8.*rxxyy,-30.*(rxSq+sxSq+txSq),16.*rxSq+8.*rxxyy,-rxSq-rxxyy, \
                           rsx8,-rsx64,16.*sxSq+8.*sxxyy,rsx64,-rsx8, \
                             -rsx,rsx8,-sxSq-sxxyy,-rsx8,rsx,\
          0.,0.,stx8,0.,0., 0.,0.,-stx64,0.,0., rtx8,-rtx64,16.*txSq+8.*txxyy,rtx64,-rtx8, \
                                                                         0.,0.,stx64,0.,0., 0.,0.,-stx8,0.,0.,\
          0.,0.,-stx,0.,0., 0.,0.,stx8,0.,0., -rtx,rtx8,-txSq-txxyy,-rtx8,rtx, 0.,0.,-stx8,0.,0., 0.,0.,stx,0.,0.)

     #Elif #operator == "x"
       x4thOrder3d(x)
     #Elif #operator == "y"
       x4thOrder3d(y)
     #Elif #operator == "z"
       x4thOrder3d(z)
     #Elif #operator == "xx"
       xx4thOrder3d(x)
     #Elif #operator == "yy"
       xx4thOrder3d(y)
     #Elif #operator == "zz"
       xx4thOrder3d(z)
     #Elif #operator == "xy"
       xy4thOrder3d(x,y)
     #Elif #operator == "xz"
       xy4thOrder3d(x,y)
     #Elif #operator == "yz"
       xy4thOrder3d(x,y)
     #End
    endLoops()
  
  end if
  
  
else
c       ************************
c       ******* 1D *************      
c       ************************
  #If #operator == "identity"
    loopBody4thOrder1d(0.,0.,1.,0.,0.)
    return
  #End
  if( gridType .eq. 0 )then
c   rectangular
    beginLoops()
     #If #operator == "laplacian" || #operator == "xx"
       loopBody4thOrder1d(-h42(1),16.*h42(1),-30.*h42(1),16.*h42(1),-h42(1))
     #Elif #operator == "x"
       loopBody4thOrder1d(h41(1),-8.*h41(1),0.,8.*h41(1),-h41(1))
     #End
    endLoops()
  else
c  ***** not rectangular *****
    beginLoops()
     #If #operator == "laplacian" || #operator == "xx"
      rxSq=d24(1)*rx(i1,i2,i3)**2
      rxxyy=d14(1)*rxx1(i1,i2,i3)
      loopBody4thOrder1d(-rxSq+rxxyy,16.*rxSq-8.*rxxyy,-30.*rxSq,16.*rxSq+8.*rxxyy,-rxSq-rxxyy)
     #Elif #operator == "x"
      rx4 = d14(1)*(r x(i1,i2,i3))
      loopBody4thOrder1d(rx4,-8.*rx4,0.,8.*rx4,-rx4)
     #End
    endLoops()
  
  end if
  
  end if

return
end
#endMacro



#beginMacro buildFile4(x)
#beginFile x ## Coeff4.f
 coeffOperator4thOrder(x)
#endFile
#endMacro

      buildFile4(identity)
      buildFile4(laplacian)
      buildFile4(x)
      buildFile4(y)
      buildFile4(z)
      buildFile4(xx)
      buildFile4(xy)
      buildFile4(xz)
      buildFile4(yy)
      buildFile4(yz)
      buildFile4(zz)

c done
