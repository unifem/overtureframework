! -*- mode: F90 -*-
! The next include defines the macros for conservative approximations
#Include "../include/defineConservative.h"

      subroutine coeffOperator( nd, 
     &    nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &    ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,
     &    nds1a,nds1b,nds2a,nds2b,nds3a,nds3b,
     &    n1a,n1b,n2a,n2b,n3a,n3b, 
     &    ndc, nc, ns, ea,eb, ca,cb,
     &    dx, dr,
     &    rsxy, jac, coeff,s, 
     &    ndw,w,  ! work space
     &    derivative, derivType, gridType, order, averagingType, 
     &    dir1, dir2, ierr  )
! ===============================================================
!    Build Coefficients for Operators
!  *** This subroutine calls the appropriate sub for a particular operator ***
!  
!  nd : number of range spatial dimensions 
!  nd1a,nd1b : mesh dimensions axis 1 (dimensions for rsxy and jac)
!  nd2a,nd2b : mesh dimensions axis 2
!  nd3a,nd3b : mesh dimensions axis 3
!
!  ndc : number of coefficients/mesh point
!  ndc1a,ndc1b : coefficient array dimensions axis 1 (dimensions for coeff)
!  ndc2a,ndc2b : coefficient array dimensions axis 2
!  ndc3a,ndc3b : coefficient array dimensions axis 3
!
!  nds1a,nds1b,nds2a,nds2b,nds3a,nds3b : dimensions for s
!
!  n1a,n1b : subset for evaluating operator, axis 1
!  n2a,n2b : subset for evaluating operator, axis 2
!  n3a,n3b : subset for evaluating operator, axis 3
!
!  nc : number of components
!  ns : stencil size
!  ca,cb : assign components c=ca,..,cb (base 0)
!  ea,eb : assign equations e=ea,..eb   (base 0)
!
!  dx : grid spacing for rectangular grids.
!  dr : unit square spacing
!
!  derivative : specify the derivative 0=xDerivative, ... (from the enum in MappedGridOperators.h)
!  rsxy : jacobian information, not used if rectangular
!  coeff : coefficient matrix
!  derivType : 0=non-conservative, 1=conservative
!  gridType: 0=rectangular, 1=non-rectangular
!  order : 2 or 4
!  averagingType : arithmeticAverage=0, harmonicAverage=1, for conservative approximations
!  dir1,dir2 : for derivative=derivativeScalarDerivative
!
!  ndw : size of the work space w
!  w   : work space required for some operators. 
!  ierr : error return
!         ierr=2 : not enough space in ndw
!======================================================================
!      implicit none
      integer nd, 
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b, 
     & nds1a,nds1b,nds2a,nds2b,nds3a,nds3b, 
     &  n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,
     &  derivative, derivType, gridType, order, averagingType,ndw,
     & dir1,dir2,ierr

      real dx(3),dr(3)
      real rsxy(*)
      real jac(*)
      real s(*)
      real coeff(1:ndc,ndc1a:ndc1b,ndc2a:ndc2b,ndc3a:ndc3b)
      real w(0:*)

      integer derivOption,nda,ndwMin
      integer laplace,divScalarGrad,derivativeScalarDerivative
      parameter(laplace=0,divScalarGrad=1,derivativeScalarDerivative=2)
      integer conservative, nonConservative
      parameter(nonConservative=0,conservative=1)
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
  call x ## Coeff2ndOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,\
   nds1a,nds1b,nds2a,nds2b,nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,\
   ca,cb, dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s, jac, averagingType, dir1, dir2,\
   w(0),w(nda),w(2*nda),w(3*nda),w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda) )
else if( order.eq.4 )then
  call x ## Coeff4thOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,\
   nds1a,nds1b,nds2a,nds2b,nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,\
   ca,cb, dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s, jac, averagingType, dir1, dir2,\
   w(0),w(nda),w(2*nda),w(3*nda),w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda) )
else if( order.eq.6 )then
  call x ## Coeff6thOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,\
   nds1a,nds1b,nds2a,nds2b,nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,\
   ca,cb, dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s, jac, averagingType, dir1, dir2,\
   w(0),w(nda),w(2*nda),w(3*nda),w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda) )
else
  write(*,'(" opcoeff: not implemented for order=",i6)') order
  stop 8272
end if
#endMacro

! The next call assumes that only 3 work space arrays are needed, a11,a22,a33
#beginMacro callOperatorRectangular(x)
if( order.eq.2 )then
  call x ## Coeff2ndOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,\
   nds1a,nds1b,nds2a,nds2b,nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,\
   ca,cb, dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s, jac, averagingType, dir1, dir2,\
   w(0),w(nda),w(2*nda),w(2*nda),w(2*nda),w(2*nda), w(2*nda),w(2*nda),w(2*nda) )
else if( order.eq.4 )then
  call x ## Coeff4thOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,\
   nds1a,nds1b,nds2a,nds2b,nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,\
   ca,cb, dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s, jac, averagingType, dir1, dir2,\
   w(0),w(nda),w(2*nda),w(2*nda),w(2*nda),w(2*nda), w(2*nda),w(2*nda),w(2*nda) )
else if( order.eq.6 )then
  call x ## Coeff6thOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,\
   nds1a,nds1b,nds2a,nds2b,nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,\
   ca,cb, dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s, jac, averagingType, dir1, dir2,\
   w(0),w(nda),w(2*nda),w(2*nda),w(2*nda),w(2*nda), w(2*nda),w(2*nda),w(2*nda) )
else
  write(*,'(" opcoeff: not implemented for order=",i6)') order
  stop 8272
end if
#endMacro

      if( derivative.eq.laplacianOperator )then
        derivOption=laplace
      else if( derivative.eq.divergenceScalarGradient )then
        derivOption=divScalarGrad
      else if( derivative.ge.xDerivativeScalarXDerivative .and. derivative.le.zDerivativeScalarZDerivative )then
        derivOption=derivativeScalarDerivative
      else
        derivOption=-1
      end if
      ! for work space
      if( derivOption.ge.0 )then
        nda=(nd1b-nd1a+1)*(nd2b-nd2a+1)*(nd3b-nd3a+1)
      else
        nda=0
      end if
      if( derivative.eq.laplacianOperator .and. derivType.eq.nonConservative )then
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
      else if( derivative.eq.divergenceScalarGradient .or. derivative.eq.laplacianOperator .or.
     &        (derivative.ge.xDerivativeScalarXDerivative .and. derivative.le.zDerivativeScalarZDerivative)) then
        ! this next call can do divScalarGrad, derivativeScalarDerivative and conservative laplacian

        ! check work space size
        if( derivative.eq.laplacianOperator .or. derivative.eq.divergenceScalarGradient )then
          if( gridType.eq.0 )then
            ndwMin=nda*nd
          else
            ndwMin=nda*nd*nd
          end if
        else
          if( gridType.eq.0 )then
            ndwMin=nda
          else
            ndwMin=nda*nd*nd
          end if
        end if
        if( ndw.lt.ndwMin )then
          write(*,*) 'coeffOperator:ERROR: ndw=',ndw,' is too small'
          write(*,*) '  ndw should be at least ',ndwMin
          ierr=2
          return
        end if
        if( (derivative.eq.laplacianOperator .or. derivative.eq.divergenceScalarGradient)
     &      .and. gridType.eq.0 )then
          ! rectangular grid requires work arrays a11,a22,a33
          callOperatorRectangular(divScalarGrad)
        else
          callOperator(divScalarGrad)
        endif
      else if( derivative.eq.r1Derivative )then
        callOperator(r)
      else if( derivative.eq.r2Derivative )then
        callOperator(s)
      else if( derivative.eq.r3Derivative )then
        callOperator(t)
      else if( derivative.eq.r1r1Derivative )then
        callOperator(rr)
      else if( derivative.eq.r1r2Derivative )then
        callOperator(rs)
      else if( derivative.eq.r1r3Derivative )then
        callOperator(rt)
      else if( derivative.eq.r2r2Derivative )then
        callOperator(ss)
      else if( derivative.eq.r2r3Derivative )then
        callOperator(st)
      else if( derivative.eq.r3r3Derivative )then
        callOperator(tt)
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
! ***** loop over equations and components *****
do e=ea,eb
do c=ca,cb
ec=ns*(c+nc*e)
! ** it did not affect performance to use an array to index coeff ***
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

! This macro will switch the roles of y and y in the arguments
#beginMacro loopBody2ndOrder2dSwitchyy(c00,c10,c20,c01,c11,c21,c02,c12,c22)
loopBody2ndOrder2d(c00,c10,c20,c01,c11,c21,c02,c12,c22)
#endMacro

! This macro will switch the roles of x and y in the arguments
#beginMacro loopBody2ndOrder2dSwitchxy(c00,c10,c20,c01,c11,c21,c02,c12,c22)
loopBody2ndOrder2d(c00,c01,c02,c10,c11,c12,c20,c21,c22)
#endMacro
! This macro will switch the roles of x and y in the arguments
#beginMacro loopBody2ndOrder2dSwitchyx(c00,c10,c20,c01,c11,c21,c02,c12,c22)
loopBody2ndOrder2d(c00,c01,c02,c10,c11,c12,c20,c21,c22)
#endMacro

! This macro will switch the roles of x and x in the arguments
#beginMacro loopBody2ndOrder3dSwitchxx(c000,c100,c200,c010,c110,c210,c020,c120,c220,c001,c101,c201,c011,c111,c211,c021,c121,c221,c002,c102,c202,c012,c112,c212,c022,c122,c222)
loopBody2ndOrder3d(c000,c100,c200,c010,c110,c210,c020,c120,c220,c001,c101,c201,c011,c111,c211,c021,c121,c221,c002,c102,c202,c012,c112,c212,c022,c122,c222)
#endMacro

! This macro will switch the roles of y and y in the arguments
#beginMacro loopBody2ndOrder3dSwitchyy(c000,c100,c200,c010,c110,c210,c020,c120,c220,c001,c101,c201,c011,c111,c211,c021,c121,c221,c002,c102,c202,c012,c112,c212,c022,c122,c222)
loopBody2ndOrder3d(c000,c100,c200,c010,c110,c210,c020,c120,c220,c001,c101,c201,c011,c111,c211,c021,c121,c221,c002,c102,c202,c012,c112,c212,c022,c122,c222)
#endMacro

! This macro will switch the roles of z and z in the arguments
#beginMacro loopBody2ndOrder3dSwitchzz(c000,c100,c200,c010,c110,c210,c020,c120,c220,c001,c101,c201,c011,c111,c211,c021,c121,c221,c002,c102,c202,c012,c112,c212,c022,c122,c222)
loopBody2ndOrder3d(c000,c100,c200,c010,c110,c210,c020,c120,c220,c001,c101,c201,c011,c111,c211,c021,c121,c221,c002,c102,c202,c012,c112,c212,c022,c122,c222)
#endMacro

! This macro will switch the roles of x and y in the arguments
#beginMacro loopBody2ndOrder3dSwitchxy(c000,c100,c200,c010,c110,c210,c020,c120,c220,c001,c101,c201,c011,c111,c211,c021,c121,c221,c002,c102,c202,c012,c112,c212,c022,c122,c222)
loopBody2ndOrder3d(c000,c010,c020,c100,c110,c120,c200,c210,c220,c001,c011,c021,c101,c111,c121,c201,c211,c221,c002,c012,c022,c102,c112,c122,c202,c212,c222)
#endMacro

! This macro will switch the roles of y and x in the arguments
#beginMacro loopBody2ndOrder3dSwitchyx(c000,c100,c200,c010,c110,c210,c020,c120,c220,c001,c101,c201,c011,c111,c211,c021,c121,c221,c002,c102,c202,c012,c112,c212,c022,c122,c222)
loopBody2ndOrder3d(c000,c010,c020,c100,c110,c120,c200,c210,c220,c001,c011,c021,c101,c111,c121,c201,c211,c221,c002,c012,c022,c102,c112,c122,c202,c212,c222)
#endMacro

! This macro will switch the roles of x and z in the arguments
#beginMacro loopBody2ndOrder3dSwitchxz(c000,c100,c200,c010,c110,c210,c020,c120,c220,c001,c101,c201,c011,c111,c211,c021,c121,c221,c002,c102,c202,c012,c112,c212,c022,c122,c222)
loopBody2ndOrder3d(c000,c001,c002,c010,c011,c012,c020,c021,c022,c100,c101,c102,c110,c111,c112,c120,c121,c122,c200,c201,c202,c210,c211,c212,c220,c221,c222)
#endMacro
! This macro will switch the roles of z and x in the arguments
#beginMacro loopBody2ndOrder3dSwitchzx(c000,c100,c200,c010,c110,c210,c020,c120,c220,c001,c101,c201,c011,c111,c211,c021,c121,c221,c002,c102,c202,c012,c112,c212,c022,c122,c222)
loopBody2ndOrder3d(c000,c001,c002,c010,c011,c012,c020,c021,c022,c100,c101,c102,c110,c111,c112,c120,c121,c122,c200,c201,c202,c210,c211,c212,c220,c221,c222)
#endMacro

! This macro will switch the roles of y and z in the arguments
#beginMacro loopBody2ndOrder3dSwitchyz(c000,c100,c200,c010,c110,c210,c020,c120,c220,c001,c101,c201,c011,c111,c211,c021,c121,c221,c002,c102,c202,c012,c112,c212,c022,c122,c222)
loopBody2ndOrder3d(c000,c100,c200,c001,c101,c201,c002,c102,c202,c010,c110,c210,c011,c111,c211,c012,c112,c212,c020,c120,c220,c021,c121,c221,c022,c122,c222)
#endMacro
! This macro will switch the roles of z and y in the arguments
#beginMacro loopBody2ndOrder3dSwitchzy(c000,c100,c200,c010,c110,c210,c020,c120,c220,c001,c101,c201,c011,c111,c211,c021,c121,c221,c002,c102,c202,c012,c112,c212,c022,c122,c222)
loopBody2ndOrder3d(c000,c100,c200,c001,c101,c201,c002,c102,c202,c010,c110,c210,c011,c111,c211,c012,c112,c212,c020,c120,c220,c021,c121,c221,c022,c122,c222)
#endMacro


! define a macro for x, y in 2d rectangular
#beginMacro x2ndOrder2dRectangular(x,axis)
loopBody2ndOrder2dSwitchx ## x(0.,0.,0.,-h21(axis),0.,h21(axis),0.,0.,0.)
#endMacro

! define a macro for xx, yy in 2d rectangular
#beginMacro xx2ndOrder2dRectangular(x,axis)
loopBody2ndOrder2dSwitchx ## x(0.,0.,0., h22(axis),-2.*h22(axis),h22(axis), 0.,0.,0.)
#endMacro
! define a macro for x, y in 3d rectangular
#beginMacro x2ndOrder3dRectangular(x,axis)
loopBody2ndOrder3dSwitchx ## x(\
0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,-h21(axis),0.,h21(axis),0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.)
#endMacro

! define a macro for xx, yy in 3d rectangular
#beginMacro xx2ndOrder3dRectangular(x,axis)
loopBody2ndOrder3dSwitchx ## x(\
0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,h22(axis),-2.*h22(axis),h22(axis),0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.)
#endMacro

! define a macro for xy, xz, yz in 3d rectangular
!  for derivative xy use (x,x,1,2)
!  for derivative xz use (y,z,1,3)
!  for derivative yz use (x,z,2,3)
#beginMacro xy2ndOrder3dRectangular(xd,yd,axis1,axis2)
d=h21(axis1)*h21(axis2)
d8=d*8.
d64=d*64.
loopBody2ndOrder3dSwitch ## xd ## yd(\
0.,0.,0., 0.,0.,0., 0.,0.,0., d,0.,-d, 0.,0.,0., -d,0.,d, 0.,0.,0., 0.,0.,0., 0.,0.,0.)
#endMacro


! define a macro for r, s in 2d rectangular
#beginMacro r2ndOrder2dRectangular(x,axis)
loopBody2ndOrder2dSwitchx ## x(0.,0.,0.,-d12(axis),0.,d12(axis),0.,0.,0.)
#endMacro

! define a macro for rr, ss in 2d rectangular
#beginMacro rr2ndOrder2dRectangular(x,axis)
loopBody2ndOrder2dSwitchx ## x(0.,0.,0., d22(axis),-2.*d22(axis),d22(axis), 0.,0.,0.)
#endMacro

! define a macro for r, s, t in 3d rectangular
#beginMacro r2ndOrder3dRectangular(x,axis)
loopBody2ndOrder3dSwitchx ## x(\
0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,-d12(axis),0.,d12(axis),0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.)
#endMacro

! define a macro for rr, ss, tt in 3d rectangular
#beginMacro rr2ndOrder3dRectangular(x,axis)
loopBody2ndOrder3dSwitchx ## x(\
0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,d22(axis),-2.*d22(axis),d22(axis),0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.)
#endMacro

! define a macro for xy, xz, yz in 3d rectangular
!  for derivative rs use (x,x,1,2)
!  for derivative rt use (y,z,1,3)
!  for derivative st use (x,z,2,3)
#beginMacro rs2ndOrder3dRectangular(xd,yd,axis1,axis2)
d=d12(axis1)*d12(axis2)
d8=d*8.
d64=d*64.
loopBody2ndOrder3dSwitch ## xd ## yd(\
0.,0.,0., 0.,0.,0., 0.,0.,0., d,0.,-d, 0.,0.,0., -d,0.,d, 0.,0.,0., 0.,0.,0., 0.,0.,0.)
#endMacro


! define a macro for x, y in 2d
#beginMacro x2ndOrder2d(x)
rxd = d12(1)*(r x(i1,i2,i3))
sxd = d12(2)*(s x(i1,i2,i3))
loopBody2ndOrder2d(0.,-sxd,0., -rxd,0.,rxd, 0.,sxd,0.)
#endMacro

! define a macro for xx, yy in 2d
#beginMacro xx2ndOrder2d(x)
rxSq=d22(1)*(r x(i1,i2,i3)**2)
rxx =d12(1)*(r x x 2(i1,i2,i3)) 
sxSq=d22(2)*(s x(i1,i2,i3)**2)
sxx =d12(2)*(s x x 2(i1,i2,i3))
rsx =(2.*d12(1)*d12(2))*(r x(i1,i2,i3)*s x(i1,i2,i3))
! check this
loopBody2ndOrder2d(rsx,sxSq -sxx,-rsx,rxSq-rxx,-2.*(rxSq+sxSq),rxSq+rxx,-rsx,sxSq +sxx,rsx)
#endMacro


! define a macro for x, y, z in 3d
#beginMacro x2ndOrder3d(x)
rxd = d12(1)*(r x(i1,i2,i3))
sxd = d12(2)*(s x(i1,i2,i3))
txd = d12(3)*(t x(i1,i2,i3))
loopBody2ndOrder3d(0.,0.,0.,0.,-txd,0.,0.,0.,0.,0.,-sxd,0.,-rxd,0.,rxd,0.,sxd,0.,0.,0.,0.,0.,txd,0.,0.,0.,0.)
#endMacro

! define a macro for xx, yy, zz in 3d
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

! define a macro for xy, xz, yz in 3d
#beginMacro xy2ndOrder3d(xd,yd)
 rxsq = d22(1)*(r xd(i1,i2,i3)*r yd(i1,i2,i3))
 rxx = d12(1)*(r xd yd 23(i1,i2,i3))
 sxsq = d22(2)*(s xd(i1,i2,i3)*s yd(i1,i2,i3))
 sxx = d12(2)*(s xd yd 23(i1,i2,i3))
 txsq = d22(3)*(t xd(i1,i2,i3)*t yd(i1,i2,i3))
 txx = d12(3)*(t xd yd 23(i1,i2,i3))
 rsx = (d12(1)*d12(2))*(r xd(i1,i2,i3)*s yd(i1,i2,i3)+r yd(i1,i2,i3)*s xd(i1,i2,i3))
 rtx = (d12(1)*d12(3))*(r xd(i1,i2,i3)*t yd(i1,i2,i3)+r yd(i1,i2,i3)*t xd(i1,i2,i3))
 stx = (d12(2)*d12(3))*(s xd(i1,i2,i3)*t yd(i1,i2,i3)+s yd(i1,i2,i3)*t xd(i1,i2,i3))

loopBody2ndOrder3d(0.,stx,0., rtx,txSq-txx,-rtx, 0.,-stx,0., \
                   rsx,sxSq-sxx,-rsx, rxSq-rxx,-2.*(rxSq+sxSq+txSq),rxSq+rxx, -rsx,sxSq+sxx,rsx, \
                   0.,-stx,0., -rtx,txSq+txx,rtx, 0.,stx,0.)
#endMacro


#beginMacro coeffOperator2ndOrder(operator)
subroutine operator ## Coeff2ndOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
    ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b, nds1a,nds1b,nds2a,nds2b,nds3a,nds3b,\
    n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb, ca,cb,\
    dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s, jac, averagingType, dir1, dir2,\
    a11,a22,a12,a21,a33,a13,a23,a31,a32 )
! ===============================================================
!  Derivative Coefficients
!  
!  nd : number of range spatial dimensions 
!  nd1a,nd1b : mesh dimensions axis 1
!  nd2a,nd2b : mesh dimensions axis 2
!  nd3a,nd3b : mesh dimensions axis 3
!
!  ndc : number of coefficients/mesh point
!  nc1a,nd1b : coefficient array dimensions axis 1
!  nc2a,nd2b : coefficient array dimensions axis 2
!  nc3a,nd3b : coefficient array dimensions axis 3
!
!  nc1a,nd1b : subset for evaluating operator, axis 1
!  nc2a,nd2b : subset for evaluating operator, axis 2
!  nc3a,nd3b : subset for evaluating operator, axis 3
!
!  nc : number of components
!  ns : stencil size
!  ca,cb : assign components c=ca,..,cb (base 0)
!  ea,eb : assign equations e=ea,..eb   (base 0)
!
!  d11 : 1/dr
!
!  h11 : 1/h    :  for rectangular   
!
!  rsxy : jacobian information, not used if rectangular
!  coeff : coefficient matrix
!  gridType: 0=rectangular, 1=non-rectangular
!  order : 2 or 4

! nc : number of components
! ns : stencil size
! ca,cb : assign components c=ca,..,cb (base 0)
! ea,eb : assign equations e=ea,..eb   (base 0)
! gridType: 0=rectangular, 1=non-rectangular
! order : 2 or 4
! rsxy : not used if rectangular
! ===============================================================

!      implicit none
integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc,ns, ca,cb,ea,eb, gridType, order
integer ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,nds2b,nds3a,nds3b
integer derivOption, derivType, averagingType, dir1, dir2 
real dx(3),dr(3)
real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,1:nd,1:nd)
real coeff(1:ndc,ndc1a:ndc1b,ndc2a:ndc2b,ndc3a:ndc3b)
! *wdh* 2016/08/27 real s(nds1a:nds1b,nds2a:nds2b,nds3a:nds3b)
real s(nds1a:nds1b,nds2a:nds2b,nds3a:nds3b,0:*)
real jac(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
real a11(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
real a12(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
real a13(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
real a21(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
real a22(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
real a23(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
real a31(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
real a32(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
real a33(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
! real rx,ry,rz,sx,sy,sz,tx,ty,tz,d
! real rxSq,rxx,sxSq,sxx,rsx,rxx2,ryy2,sxx2,syy2
! real rxt2,ryt2,rzz23,sxt2,syt2,szz23,txr2,txs2
! real txt2,tyr2,tys2,tyt2,tzz23,rzr2,rzs2,rzt2
! real szr2,szs2,szt2,tzr2,tzs2,tzt2
! real rxr2,rxs2,ryr2,rys2,sxr2,sxs2,syr2,sys2
! real txx,txSq,rtx,stx,rxx23,ryy23,sxx23,syy23,txx23,tyy23

!..... added by kkc 1/2/02 for g77 unsatisfied reference
real u(1,1,1,1)

real h21(3),d22(3),d12(3),h22(3)
integer i1,i2,i3,kd3,kd,c,e,ec
integer m12,m22,m32
integer m(-1:1,-1:1),m3(-1:1,-1:1,-1:1)

integer laplace,divScalarGrad,derivativeScalarDerivative
parameter(laplace=0,divScalarGrad=1,derivativeScalarDerivative=2)
integer arithmeticAverage,harmonicAverage
parameter( arithmeticAverage=0,harmonicAverage=1 ) 
integer symmetric
parameter( symmetric=2 )

!.......statement functions for jacobian
rx(i1,i2,i3)=rsxy(i1,i2,i3,  1,  1)
ry(i1,i2,i3)=rsxy(i1,i2,i3,  1,  2)
rz(i1,i2,i3)=rsxy(i1,i2,i3,  1,kd3)
sx(i1,i2,i3)=rsxy(i1,i2,i3,  2,  1)
sy(i1,i2,i3)=rsxy(i1,i2,i3,  2,  2)
sz(i1,i2,i3)=rsxy(i1,i2,i3,  2,kd3)
tx(i1,i2,i3)=rsxy(i1,i2,i3,kd3,  1)
ty(i1,i2,i3)=rsxy(i1,i2,i3,kd3,  2)
tz(i1,i2,i3)=rsxy(i1,i2,i3,kd3,kd3)

include 'cgux2af.h'
rxx1(i1,i2,i3)=rx(i1,i2,i3)*rxr2(i1,i2,i3)

!.....end statement functions

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
!       ************************
!       ******* 2D *************      
!       ************************

  #If #operator == "identity"
    beginLoops()
      loopBody2ndOrder2d(0.,0.,0., 0.,1.,0., 0.,0.,0.)
    endLoops()
    return
  #Elif #operator == "r"
    beginLoops()
      r2ndOrder2dRectangular(x,1)
    endLoops()
    return
  #Elif #operator == "s"
    beginLoops()
      r2ndOrder2dRectangular(y,2)
    endLoops()
    return
  #Elif #operator == "rr"
    beginLoops()
      rr2ndOrder2dRectangular(x,1)
    endLoops()
    return
  #Elif #operator == "ss"
    beginLoops()
      rr2ndOrder2dRectangular(y,2)
    endLoops()
    return
  #Elif #operator == "rs"
    beginLoops()
      d=d12(1)*d12(2)
      loopBody2ndOrder2d(d,0.,-d, 0.,0.,0., -d,0.,d)
    endLoops()
    return
  #End

  if( gridType .eq. 0 )then
!   rectangular
    #If #operator == "divScalarGrad"
      defineA22R()
      if( derivOption.eq.divScalarGrad )then
       beginLoops()
       loopBody2ndOrder2d(0.,a22(i1,i2,i3),0., a11(i1,i2,i3),\
           -(a11(i1+1,i2,i3)+a11(i1,i2,i3)+a22(i1,i2,i3)+a22(i1,i2+1,i3)), \
                  a11(i1+1,i2,i3),  0.,a22(i1,i2+1,i3),0.)
       endLoops()
      else if( dir1.eq.0 .and. dir2.eq.0 )then
        beginLoops()
         loopBody2ndOrder2d(0.,0.,0., a11(i1,i2,i3), -(a11(i1+1,i2,i3)+a11(i1,i2,i3)), a11(i1+1,i2,i3), 0.,0.,0.)
        endLoops()
      else if( dir1.eq.0 .and. dir2.eq.1 )then
        beginLoops()
         loopBody2ndOrder2d(a11(i1,i2,i3),-a11(i1+1,i2,i3)+a11(i1,i2,i3),-a11(i1+1,i2,i3),0,0,0,-a11(i1,i2,i3), \
        a11(i1+1,i2,i3)-a11(i1,i2,i3),a11(i1+1,i2,i3))
        endLoops()
      else if( dir1.eq.1 .and. dir2.eq.0 )then
        beginLoops()
         loopBody2ndOrder2d(a11(i1,i2,i3),0,-a11(i1,i2,i3),-a11(i1,i2+1,i3)+a11(i1,i2,i3),0, \
        a11(i1,i2+1,i3)-a11(i1,i2,i3),-a11(i1,i2+1,i3),0,a11(i1,i2+1,i3))
        endLoops()
      else if( dir1.eq.1 .and. dir2.eq.1 )then
        beginLoops()
         loopBody2ndOrder2d(0,a11(i1,i2,i3),0,0,-a11(i1,i2+1,i3)-a11(i1,i2,i3),0,0,a11(i1,i2+1,i3),0)
        endLoops()
      end if
    #Else
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
    #End
  else
!  ***** not rectangular *****
    #If #operator == "divScalarGrad"
!       Here we define divScalarGrad as well as laplacian and DxSDy etc
      defineA22()
!       This was generated by dd.m
      beginLoops()
        loopBody2ndOrder2d( \
        (a12(i1,i2,i3)+a21(i1,i2,i3))/jac(i1,i2,i3), \
        (a12(i1,i2,i3)+a22(i1,i2,i3)-a12(i1+1,i2,i3))/jac(i1,i2,i3), \
        -(a21(i1,i2,i3)+a12(i1+1,i2,i3))/jac(i1,i2,i3), \
        (a11(i1,i2,i3)+a21(i1,i2,i3)-a21(i1,i2+1,i3))/jac(i1,i2,i3), \
        -(a11(i1+1,i2,i3)+a11(i1,i2,i3)+a22(i1,i2+1,i3)+a22(i1,i2,i3))/jac(i1,i2,i3), \
        -(-a11(i1+1,i2,i3)-a21(i1,i2+1,i3)+a21(i1,i2,i3))/jac(i1,i2,i3), \
        -(a21(i1,i2+1,i3)+a12(i1,i2,i3))/jac(i1,i2,i3), \
        -(a12(i1,i2,i3)-a22(i1,i2+1,i3)-a12(i1+1,i2,i3))/jac(i1,i2,i3), \
        (a21(i1,i2+1,i3)+a12(i1+1,i2,i3))/jac(i1,i2,i3) )
      endLoops()
    #Else
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
    #End
  
  endif 
elseif( nd.eq.3 )then
!       ************************
!       ******* 3D *************      
!       ************************
  
  #If #operator == "identity"
    beginLoops()
      loopBody2ndOrder3d(0.,0.,0.,0.,0.,0.,0.,0.,0., \
                         0.,0.,0.,0.,1.,0.,0.,0.,0., \
                         0.,0.,0.,0.,0.,0.,0.,0.,0. )
    endLoops()
    return
   #Elif #operator == "r"
    beginLoops()
      r2ndOrder3dRectangular(x,1)
    endLoops()
    return
   #Elif #operator == "s"
    beginLoops()
      r2ndOrder3dRectangular(y,2)
    endLoops()
    return
   #Elif #operator == "t"
    beginLoops()
      r2ndOrder3dRectangular(z,3)
    endLoops()
    return
   #Elif #operator == "rr"
    beginLoops()
      rr2ndOrder3dRectangular(x,1)
    endLoops()
    return
   #Elif #operator == "ss"
    beginLoops()
      rr2ndOrder3dRectangular(y,2)
    endLoops()
    return
   #Elif #operator == "tt"
    beginLoops()
      rr2ndOrder3dRectangular(z,3)
    endLoops()
    return
   #Elif #operator == "rs"
    beginLoops()
      rs2ndOrder3dRectangular(x,x,1,2)
    endLoops()
    return
   #Elif #operator == "rt"
    beginLoops()
      rs2ndOrder3dRectangular(y,z,1,3)
    endLoops()
    return
   #Elif #operator == "st"
    beginLoops()
      rs2ndOrder3dRectangular(x,z,2,3)
    endLoops()
    return
  #End

  if( gridType .eq. 0 )then
!   rectangular
    #If #operator == "divScalarGrad"
      defineA23R()
      if( derivOption.eq.divScalarGrad )then
       beginLoops()
!       This was generated by dd.m
        loopBody2ndOrder3d(\
        0,0,0,0,a33(i1,i2,i3),0,0,0,0,0,a22(i1,i2,i3),0,a11(i1,i2,i3), \
        -a11(i1+1,i2,i3)-a11(i1,i2,i3)-a22(i1,i2+1,i3)-a22(i1,i2,i3)-a33(i1,i2,i3+1)-a33(i1,i2,i3), \
        a11(i1+1,i2,i3),0,a22(i1,i2+1,i3),0,0,0,0,0,a33(i1,i2,i3+1),0,0,0,0 )
       endLoops()
      else if( dir1.eq.0 .and. dir2.eq.0 )then
        beginLoops()
         loopBody2ndOrder3d(0,0,0,0,0,0,0,0,0,0,0,0,a11(i1,i2,i3),-a11(i1+1,i2,i3)-a11(i1,i2,i3),a11(i1+1,i2,i3),0,0,0,0,0,0,0,0,0,0,0,0 )
        endLoops()
      else if( dir1.eq.0 .and. dir2.eq.1 )then
        beginLoops()
         loopBody2ndOrder3d(\
          0,0,0,0,0,0,0,0,0,a11(i1,i2,i3),-a11(i1+1,i2,i3)+a11(i1,i2,i3),-a11(i1+1,i2,i3),0,0,0,-a11(i1,i2,i3), \
          a11(i1+1,i2,i3)-a11(i1,i2,i3),a11(i1+1,i2,i3),0,0,0,0,0,0,0,0,0 )
        endLoops()
      else if( dir1.eq.0 .and. dir2.eq.2 )then
        beginLoops()
         loopBody2ndOrder3d(\
        0,0,0,a11(i1,i2,i3),-a11(i1+1,i2,i3)+a11(i1,i2,i3),-a11(i1+1,i2,i3),0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, \
        -a11(i1,i2,i3),a11(i1+1,i2,i3)-a11(i1,i2,i3),a11(i1+1,i2,i3),0,0,0 )
        endLoops()
      else if( dir1.eq.1 .and. dir2.eq.0 )then
        beginLoops()
         loopBody2ndOrder3d(\
        0,0,0,0,0,0,0,0,0,a11(i1,i2,i3),0,-a11(i1,i2,i3),-a11(i1,i2+1,i3)+a11(i1,i2,i3),0, \
        a11(i1,i2+1,i3)-a11(i1,i2,i3),-a11(i1,i2+1,i3),0,a11(i1,i2+1,i3),0,0,0,0,0,0,0,0,0 )
        endLoops()
      else if( dir1.eq.1 .and. dir2.eq.1 )then
        beginLoops()
         loopBody2ndOrder3d(\
        0,0,0,0,0,0,0,0,0,0,a11(i1,i2,i3),0,0,-a11(i1,i2+1,i3)-a11(i1,i2,i3),0,0,a11(i1,i2+1,i3),0,0,0,0,0,0,0,0,0,0 )
        endLoops()
      else if( dir1.eq.1 .and. dir2.eq.2 )then
        beginLoops()
        loopBody2ndOrder3d(\
        0,a11(i1,i2,i3),0,0,-a11(i1,i2+1,i3)+a11(i1,i2,i3),0,0,-a11(i1,i2+1,i3),0,0,0,0,0,0,0,0,0,0,0,-a11(i1,i2,i3),0, \
        0,a11(i1,i2+1,i3)-a11(i1,i2,i3),0,0,a11(i1,i2+1,i3),0 )

        endLoops()
      else if( dir1.eq.2 .and. dir2.eq.0 )then
        beginLoops()
         loopBody2ndOrder3d(\
        0,0,0,a11(i1,i2,i3),0,-a11(i1,i2,i3),0,0,0,0,0,0,-a11(i1,i2,i3+1)+a11(i1,i2,i3),0, \
        a11(i1,i2,i3+1)-a11(i1,i2,i3),0,0,0,0,0,0,-a11(i1,i2,i3+1),0,a11(i1,i2,i3+1),0,0,0 )
        endLoops()
      else if( dir1.eq.2 .and. dir2.eq.1 )then
        beginLoops()
         loopBody2ndOrder3d(\
        0,a11(i1,i2,i3),0,0,0,0,0,-a11(i1,i2,i3),0,0,-a11(i1,i2,i3+1)+a11(i1,i2,i3),0,0,0,0,0, \
        a11(i1,i2,i3+1)-a11(i1,i2,i3),0,0,-a11(i1,i2,i3+1),0,0,0,0,0,a11(i1,i2,i3+1),0 )
        endLoops()
      else if( dir1.eq.2 .and. dir2.eq.2 )then
        beginLoops()
         loopBody2ndOrder3d(\
        0,0,0,0,a11(i1,i2,i3),0,0,0,0,0,0,0,0,-a11(i1,i2,i3+1)-a11(i1,i2,i3),0,0,0,0,0,0,0,0,a11(i1,i2,i3+1),0,0,0,0 )
        endLoops()
      end if
    #Else
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
       xy2ndOrder3dRectangular(x,y,1,2)
     #Elif #operator == "xz"
       xy2ndOrder3dRectangular(x,z,1,3)
     #Elif #operator == "yz"
       xy2ndOrder3dRectangular(y,z,2,3)
     #End
     endLoops()
    #End  
  else
!  ***** not rectangular *****
    #If #operator == "divScalarGrad"
      defineA23()
!       This was generated by dd.m
      beginLoops()
        loopBody2ndOrder3d(\
        0,(a23(i1,i2,i3)+a32(i1,i2,i3))/jac(i1,i2,i3),0,(a13(i1,i2,i3)+a31(i1,i2,i3))/jac(i1,i2,i3), \
        -(a13(i1+1,i2,i3)-a33(i1,i2,i3)-a13(i1,i2,i3)-a23(i1,i2,i3)+a23(i1,i2+1,i3))/jac(i1,i2,i3), \
        -(a13(i1+1,i2,i3)+a31(i1,i2,i3))/jac(i1,i2,i3),0, \
        -(a23(i1,i2+1,i3)+a32(i1,i2,i3))/jac(i1,i2,i3),0,(a12(i1,i2,i3)+a21(i1,i2,i3))/jac(i1,i2,i3), \
        (-a32(i1,i2,i3+1)-a12(i1+1,i2,i3)+a32(i1,i2,i3)+a22(i1,i2,i3)+a12(i1,i2,i3))/jac(i1,i2,i3), \
        -(a21(i1,i2,i3)+a12(i1+1,i2,i3))/jac(i1,i2,i3), \
        (-a31(i1,i2,i3+1)+a11(i1,i2,i3)+a31(i1,i2,i3)-a21(i1,i2+1,i3)+a21(i1,i2,i3))/jac(i1,i2,i3), \
        -(a11(i1+1,i2,i3)+a11(i1,i2,i3)+a22(i1,i2+1,i3)+a22(i1,i2,i3)+a33(i1,i2,i3+1)+a33(i1,i2,i3))/jac(i1,i2,i3), \
        -(-a11(i1+1,i2,i3)+a21(i1,i2,i3)+a31(i1,i2,i3)-a21(i1,i2+1,i3)-a31(i1,i2,i3+1))/jac(i1,i2,i3), \
        -(a21(i1,i2+1,i3)+a12(i1,i2,i3))/jac(i1,i2,i3), \
        -(-a32(i1,i2,i3+1)-a12(i1+1,i2,i3)-a22(i1,i2+1,i3)+a12(i1,i2,i3)+a32(i1,i2,i3))/jac(i1,i2,i3), \
        (a21(i1,i2+1,i3)+a12(i1+1,i2,i3))/jac(i1,i2,i3),0, \
        -(a32(i1,i2,i3+1)+a23(i1,i2,i3))/jac(i1,i2,i3),0, \
        -(a13(i1,i2,i3)+a31(i1,i2,i3+1))/jac(i1,i2,i3), \
        -(-a13(i1+1,i2,i3)+a13(i1,i2,i3)-a33(i1,i2,i3+1)-a23(i1,i2+1,i3)+a23(i1,i2,i3))/jac(i1,i2,i3), \
        (a13(i1+1,i2,i3)+a31(i1,i2,i3+1))/jac(i1,i2,i3),0, \
        (a23(i1,i2+1,i3)+a32(i1,i2,i3+1))/jac(i1,i2,i3),0 )
      endLoops()
    #Else
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
    #End
  end if
  
  
elseif( nd.eq.1 )then
!       ************************
!       ******* 1D *************      
!       ************************
  #If #operator == "identity"
    beginLoops()
      loopBody2ndOrder1d(0.,1.,0.)
    endLoops()
    return
  #Elif #operator == "rr"
    beginLoops()
      loopBody2ndOrder1d(d22(1),-2.*d22(1),d22(1))
    endLoops()
    return
  #Elif #operator == "r"
    beginLoops()
      loopBody2ndOrder1d(-d12(1),0.,d12(1))
    endLoops()
    return
  #End

  if( gridType .eq. 0 )then
!   rectangular
    #If #operator == "divScalarGrad"
      defineA21R()
        beginLoops()
        loopBody2ndOrder1d(a11(i1,i2,i3),-(a11(i1+1,i2,i3)+a11(i1,i2,i3)),a11(i1+1,i2,i3))
        endLoops()
    #Else
     beginLoops()
     #If #operator == "laplacian" || #operator == "xx"
      loopBody2ndOrder1d(h22(1),-2.*h22(1),h22(1))
     #Elif #operator == "x"
      loopBody2ndOrder1d(-h21(1),0.,h21(1))
     #End
     endLoops()
    #End
  else
!  ***** not rectangular *****
    #If #operator == "divScalarGrad"
      defineA21()
!      This was generated by dd.m
      beginLoops()
       loopBody2ndOrder1d(\
        (a11(i1,i2,i3))/jac(i1,i2,i3), \
       -(a11(i1+1,i2,i3)+a11(i1,i2,i3))/jac(i1,i2,i3), \
        (a11(i1+1,i2,i3))/jac(i1,i2,i3) )
       endLoops()
    #Else
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
    #End
  end if
  
  else if( nd.eq.0 )then
!       *** add these lines to avoid warnings about unused statement functions
    include "cgux2afNoWarnings.h" 
    temp=rxx1(i1,i2,i3)
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
      buildFile(divScalarGrad)


      buildFile(r)
      buildFile(s)
      buildFile(t)
      buildFile(rr)
      buildFile(rs)
      buildFile(rt)
      buildFile(ss)
      buildFile(st)
      buildFile(tt)



! ****************************************************************************************
! ************************* 4th order ****************************************************
! ****************************************************************************************

#beginMacro beginLoops4()
! ***** loop over equations and components *****
do e=ea,eb
do c=ca,cb
ec=ns*(c+nc*e)
! ** it did not affect performance to use an array to index coeff ***
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

! define a macro for x, y in 2d rectangular
#beginMacro x4thOrder2dRectangular(x,axis)
loopBody4thOrder2dSwitchx ## x(0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
                          h41(axis),-8.*h41(axis),0.,8.* h41(axis),-h41(axis),\
                          0.,0.,0.,0.,0., 0.,0.,0.,0.,0.) 
#endMacro

! define a macro for xx, yy in 2d rectangular
#beginMacro xx4thOrder2dRectangular(x,axis)
loopBody4thOrder2dSwitchx ## x(0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
                          -h42(axis),16.* h42(axis),-30.*h42(axis),16.* h42(axis),-h42(axis),\
                          0.,0.,0.,0.,0., 0.,0.,0.,0.,0.) 
#endMacro

! define a macro for r, s in 2d rectangular
#beginMacro r4thOrder2dRectangular(x,axis)
loopBody4thOrder2dSwitchx ## x(0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
                          d14(axis),-8.*d14(axis),0.,8.* d14(axis),-d14(axis),\
                          0.,0.,0.,0.,0., 0.,0.,0.,0.,0.) 
#endMacro

! define a macro for rr, ss in 2d rectangular
#beginMacro rr4thOrder2dRectangular(x,axis)
loopBody4thOrder2dSwitchx ## x(0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
                          -d24(axis),16.* d24(axis),-30.*d24(axis),16.* d24(axis),-d24(axis),\
                          0.,0.,0.,0.,0., 0.,0.,0.,0.,0.) 
#endMacro
! define a macro for x, y in 2d
#beginMacro x4thOrder2d(x)
rx4 = d14(1)*(r x(i1,i2,i3))
sx4 = d14(2)*(s x(i1,i2,i3))
tx4 = d14(3)*(t x(i1,i2,i3))
loopBody4thOrder2d(\
   0.,0.,sx4,0.,0., 0.,0.,-8.*sx4,0.,0., rx4,-8.*rx4,0.,8.*rx4,-rx4, 0.,0.,8.*sx4,0.,0., 0.,0.,-sx4,0.,0.)
#endMacro

! define a macro for xx, yy in 2d
#beginMacro xx4thOrder2d(x)
rxSq=d24(1)*(r x(i1,i2,i3)**2)
rxxyy=d14(1)*(r x x(i1,i2,i3))
sxSq=d24(2)*(s x(i1,i2,i3)**2)
sxxyy=d14(2)*(s x x(i1,i2,i3))
rsx =(2.*d14(1)*d14(2))*(r x(i1,i2,i3)*s x(i1,i2,i3))
rsx8  = rsx*8. 
rsx64 = rsx*64.

loopBody4thOrder2d(rsx,-rsx8,-sxSq+sxxyy,rsx8,-rsx,\
                   -rsx8,rsx64,16.*sxSq -8.*sxxyy,-  rsx64,rsx8,\
                   -rxSq+rxxyy,16.*rxSq-8.*rxxyy,-30.*(rxSq+sxSq),16.*rxSq+8.*rxxyy,-rxSq-rxxyy,\
                   rsx8,-rsx64,16.*sxSq +8.*sxxyy,rsx64,-rsx8,\
                   -rsx,rsx8,-sxSq-sxxyy,-rsx8,rsx)
#endMacro


#beginMacro xy4thOrder2d(X,Y)
rxsq = d24(1)*(r X(i1,i2,i3)*r Y(i1,i2,i3))
rxxyy= d14(1)*(r X Y (i1,i2,i3))
sxsq = d24(2)*(s X(i1,i2,i3)*s Y(i1,i2,i3))
sxxyy=d14(2)*(s X Y (i1,i2,i3))
rsx = (d14(1)*d14(2))*(r X(i1,i2,i3)*s Y(i1,i2,i3)+r Y(i1,i2,i3)*s X(i1,i2,i3))
! check this	
loopBody4thOrder2d(\
  rsx,-8.*rsx,-sxSq+sxxyy,8.*rsx,-rsx, -8.*rsx,64.*rsx,16.*sxSq-8.*sxxyy,-64.*rsx,8.*rsx, \
   -rxSq+rxxyy,16.*rxSq-8.*rxxyy,-30.*(rxSq+sxSq),16.*rxSq+8.*rxxyy,-rxSq-rxxyy,\
      8.*rsx,-64.*rsx,16.*sxSq+8.*sxxyy,64.*rsx,-8.*rsx, -rsx,8.*rsx,-sxSq-sxxyy,-8.*rsx,rsx)
#endMacro



! define a macro for x, y in 3d rectangular
#beginMacro x4thOrder3dRectangular(x,axis)
loopBody4thOrder3dSwitchx ## x(\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., h41(axis),-8.*h41(axis),0.,8.* h41(axis),-h41(axis),\
                                               0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.)
#endMacro

! define a macro for xx, yy in 3d rectangular
#beginMacro xx4thOrder3dRectangular(x,axis)
loopBody4thOrder3dSwitchx ## x(\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., -h42(axis),16.* h42(axis),-30.*h42(axis),16.* h42(axis),-h42(axis),\
                                               0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.)
#endMacro

! define a macro for xy, xz, yz in 3d rectangular
!  for derivative xy use (x,x,1,2)
!  for derivative xz use (y,z,1,3)
!  for derivative yz use (x,z,2,3)
#beginMacro xy4thOrder3dRectangular(xd,yd,axis1,axis2)
d=h41(axis1)*h41(axis2)
d8=d*8.
d64=d*64.
loopBody4thOrder3dSwitch ## xd ## yd(\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         d,-d8,0.,d8,-d, -d8,d64,0.,-d64,d8, 0.,0.,0.,0.,0., d8,-d64,0.,d64,-d8, -d,d8,0.,-d8,d,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.)
#endMacro


! define a macro for r, s, t in 3d rectangular
#beginMacro r4thOrder3dRectangular(x,axis)
loopBody4thOrder3dSwitchx ## x(\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., d14(axis),-8.*d14(axis),0.,8.* d14(axis),-d14(axis),\
                                               0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.)
#endMacro

! define a macro for rr, ss, tt in 3d rectangular
#beginMacro rr4thOrder3dRectangular(x,axis)
loopBody4thOrder3dSwitchx ## x(\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., -d24(axis),16.* d24(axis),-30.*d24(axis),16.* d24(axis),-d24(axis),\
                                               0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.)
#endMacro

! define a macro for rs, rt, st in 3d rectangular
!  for derivative rs use (x,x,1,2)
!  for derivative rt use (y,z,1,3)
!  for derivative st use (x,z,2,3)
#beginMacro rs4thOrder3dRectangular(xd,yd,axis1,axis2)
d=d14(axis1)*d14(axis2)
d8=d*8.
d64=d*64.
loopBody4thOrder3dSwitch ## xd ## yd(\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         d,-d8,0.,d8,-d, -d8,d64,0.,-d64,d8, 0.,0.,0.,0.,0., d8,-d64,0.,d64,-d8, -d,d8,0.,-d8,d,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.)
#endMacro



! define a macro for x, y, z in 3d
#beginMacro x4thOrder3d(x)
rx4 = d14(1)*(r x(i1,i2,i3))
sx4 = d14(2)*(s x(i1,i2,i3))
tx4 = d14(3)*(t x(i1,i2,i3))
loopBody4thOrder3d(\
   0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,tx4,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
   0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,-8.*tx4,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
   0.,0.,sx4,0.,0., 0.,0.,-8.*sx4,0.,0., rx4,-8.*rx4,0.,8.*rx4,-rx4, 0.,0.,8.*sx4,0.,0., 0.,0.,-sx4,0.,0.,\
   0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,8.*tx4,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
   0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,-tx4,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.)
#endMacro


! define a macro for xx, yy, zz in 3d
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

#beginMacro xy4thOrder3d(X,Y)
rxsq = d24(1)*(r X(i1,i2,i3)*r Y(i1,i2,i3))
rxxyy=d14(1)*(r X Y 3(i1,i2,i3))
sxsq = d24(2)*(s X(i1,i2,i3)*s Y(i1,i2,i3))
sxxyy=d14(2)*(s X Y 3(i1,i2,i3))
txsq = d24(3)*(t X(i1,i2,i3)*t Y(i1,i2,i3))
txxyy=d14(3)*(t X Y 3(i1,i2,i3))
rsx = (d14(1)*d14(2))*(r X(i1,i2,i3)*s Y(i1,i2,i3)+r Y(i1,i2,i3)*s X(i1,i2,i3))
rtx = (d14(1)*d14(3))*(r X(i1,i2,i3)*t Y(i1,i2,i3)+r Y(i1,i2,i3)*t X(i1,i2,i3))
stx = (d14(2)*d14(3))*(s X(i1,i2,i3)*t Y(i1,i2,i3)+s Y(i1,i2,i3)*t X(i1,i2,i3))
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
!! check this	
!      loopBody4thOrder3d(\
!     0.,0.,stx,0.,0., 0.,0.,-8.*stx,0.,0., rtx,-8.*rtx,-txSq+txxyy,8.*rtx,-rtx, 0.,0.,8.*stx,0.,0., 0.,0.,-stx,0.,0.,\
!     0.,0.,-8.*stx,0.,0., 0.,64.*stx,0.,0.,0., -8.*rtx,64.*rtx,16.*txSq-8.*txxyy,-64.*rtx,8.*rtx, \
!                                                            0.,0.,-64.*stx,0.,0., 0.,0.,8.*stx,0.,0.,\
!         rsx,-8.*rsx,-sxSq+sxxyy,8.*rsx,-rsx, -8.*rsx,64.*rsx,16.*sxSq-8.*sxxyy,-64.*rsx,8.*rsx, \
!                      -rxSq+rxxyy,16.*rxSq-8.*rxxyy,-30.*(rxSq+sxSq+txSq),16.*rxSq+8.*rxxyy,-rxSq-rxxyy,\
!                           8.*rsx,-64.*rsx,16.*sxSq+8.*sxxyy,64.*rsx,-8.*rsx, -rsx,8.*rsx,-sxSq-sxxyy,-8.*rsx,rsx,\
!     0.,0.,8.*stx,0.,0., 0.,0.,-64.*stx,0.,0., 8.*rtx,-64.*rtx,16.*txSq+8.*txxyy,64.*rtx,-8.*rtx, \
!                                                                0.,0.,64.*stx,0.,0., 0.,0.,8.*stx,0.,0.,\
!     0.,0.,-stx,0.,0., 0.,0.,8.*stx,0.,0., -rtx,8.*rtx,-txSq-txxyy,-8.*rtx,rtx, 0.,0.,-8.*stx,0.,0., 0.,0.,stx,0.,0.)
#endMacro

!        loopBody4thOrder3d(\
!        0,0,sx*ty+sy*tx,0,0,0,0,-8.*sx*ty-8.*sy*tx,0,0,rx*ty+ry*tx,-8.*rx*ty-8.*ry*tx,txy43-1.*tx*ty, \
!        8.*rx*ty+8.*ry*tx,-1.*rx*ty-1.*ry*tx,0,0,8.*sx*ty+8.*sy*tx,0,0,0,0,-1.*sx*ty-1.*sy*tx,0,0,0,0, \
!        -8.*sx*ty-8.*sy*tx,0,0,0,0,64.*sx*ty+64.*sy*tx,0,0,-8.*rx*ty-8.*ry*tx,64.*rx*ty+64.*ry*tx, \
!        -8.*txy43+16.*tx*ty,-64.*rx*ty-64.*ry*tx,8.*rx*ty+8.*ry*tx,0,0,-64.*sx*ty-64.*sy*tx,0,0,0,0, \
!        8.*sx*ty+8.*sy*tx,0,0,rx*sy+ry*sx,-8.*rx*sy-8.*ry*sx,sxy43-1.*sx*sy,8.*rx*sy+8.*ry*sx, \
!        -1.*rx*sy-1.*ry*sx,-8.*rx*sy-8.*ry*sx,64.*rx*sy+64.*ry*sx,-8.*sxy43+16.*sx*sy, \
!        -64.*rx*sy-64.*ry*sx,8.*rx*sy+8.*ry*sx,-1.*rx*ry+rxy43,16.*rx*ry-8.*rxy43, \
!        -30.*rx*ry-30.*sx*sy-30.*tx*ty,16.*rx*ry+8.*rxy43,-1.*rx*ry-1.*rxy43,8.*rx*sy+8.*ry*sx, \
!        -64.*rx*sy-64.*ry*sx,8.*sxy43+16.*sx*sy,64.*rx*sy+64.*ry*sx,-8.*rx*sy-8.*ry*sx, \
!        -1.*rx*sy-1.*ry*sx,8.*rx*sy+8.*ry*sx,-1.*sxy43-1.*sx*sy,-8.*rx*sy-8.*ry*sx,rx*sy+ry*sx,0,0, \
!        8.*sx*ty+8.*sy*tx,0,0,0,0,-64.*sx*ty-64.*sy*tx,0,0,8.*rx*ty+8.*ry*tx,-64.*rx*ty-64.*ry*tx, \
!        8.*txy43+16.*tx*ty,64.*rx*ty+64.*ry*tx,-8.*rx*ty-8.*ry*tx,0,0,64.*sx*ty+64.*sy*tx,0,0,0,0, \
!        -8.*sx*ty-8.*sy*tx,0,0,0,0,-1.*sx*ty-1.*sy*tx,0,0,0,0,8.*sx*ty+8.*sy*tx,0,0,-1.*rx*ty-1.*ry*tx, \
!        8.*rx*ty+8.*ry*tx,-1.*txy43-1.*tx*ty,-8.*rx*ty-8.*ry*tx,rx*ty+ry*tx,0,0,-8.*sx*ty-8.*sy*tx,0,0,0,0, \
!        sx*ty+sy*tx,0,0 )


#beginMacro coeffOperator4thOrder(operator)
subroutine operator ## Coeff4thOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,\
 nds1a,nds1b,nds2a,nds2b,nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc,nc,ns,ea,eb,ca,cb,\
 dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s, jac, averagingType, dir1, dir2,\
 a11,a22,a12,a21,a33,a13,a23,a31,a32 )
! ===============================================================
!  Derivative Coefficients - 4th order version
!  
! gridType: 0=rectangular, 1=non-rectangular
! rsxy : not used if rectangular
! h42 : 1/h**2 : for rectangular  
! ===============================================================

!      implicit none
integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb, ca,cb,gridType,order
integer ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,nds2b,nds3a,nds3b
integer derivOption, derivType, averagingType, dir1, dir2 
real dx(3),dr(3)
real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,1:nd,1:nd)
real coeff(1:ndc,ndc1a:ndc1b,ndc2a:ndc2b,ndc3a:ndc3b)
! *wdh* 2016/08/27 real s(nds1a:nds1b,nds2a:nds2b,nds3a:nds3b)
real s(nds1a:nds1b,nds2a:nds2b,nds3a:nds3b,0:*)
real jac(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
real a11(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
real a12(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
real a13(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
real a21(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
real a22(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
real a23(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
real a31(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
real a32(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
real a33(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
! real rx,ry,rz,sx,sy,sz,tx,ty,tz,d,d8,d64
! real rxSq,rxxyy,sxSq,sxxyy,txxyy,txSq
! real rxx,ryy,sxx,syy,rxx3,ryy3,rzz3,sxx3,syy3,szz3,txx3,tyy3,tzz3
! real rsx,rtx,stx
! real rxt,ryt,sxt,syt,txr,txs
! real txt,tyr,tys,tyt,rzr,rzs,rzt
! real szr,szs,szt,tzr,tzs,tzt
! real rxr,rxs,ryr,rys,sxr,sxs,syr,sys
! real rsx8,rsx64,rtx8,rtx64,stx8,stx64

!..... added by kkc 1/2/02 for g77 unsatisfied reference
real u(1,1,1,1)

real d24(3),d14(3),h42(3),h41(3)
integer i1,i2,i3,kd3,kd,kdd,e,c,ec
integer m12,m22,m32,m42,m52

integer m(-2:2,-2:2),m3(-2:2,-2:2,-2:2)

integer laplace,divScalarGrad,derivativeScalarDerivative
parameter(laplace=0,divScalarGrad=1,derivativeScalarDerivative=2)
integer arithmeticAverage,harmonicAverage
parameter( arithmeticAverage=0,harmonicAverage=1 ) 
integer symmetric
parameter( symmetric=2 )

!....statement functions for jacobian
rx(i1,i2,i3)=rsxy(i1,i2,i3,  1,  1)
ry(i1,i2,i3)=rsxy(i1,i2,i3,  1,  2)
rz(i1,i2,i3)=rsxy(i1,i2,i3,  1,kd3)
sx(i1,i2,i3)=rsxy(i1,i2,i3,  2,  1)
sy(i1,i2,i3)=rsxy(i1,i2,i3,  2,  2)
sz(i1,i2,i3)=rsxy(i1,i2,i3,  2,kd3)
tx(i1,i2,i3)=rsxy(i1,i2,i3,kd3,  1)
ty(i1,i2,i3)=rsxy(i1,i2,i3,kd3,  2)
tz(i1,i2,i3)=rsxy(i1,i2,i3,kd3,kd3)

include 'cgux4af.h'
rxx1(i1,i2,i3)=rx(i1,i2,i3)*rxr(i1,i2,i3)

!.....end statement functions


if( order.ne.4 )then
  write(*,*) 'laplacianCoeff4:ERROR: order!=4 '
  stop
end if

#If #operator == "divScalarGrad"
  write(*,*) 'divScalarGradCoeff4:ERROR: not implemented'
  write(*,*) '  The requested 4th order conservative'
  write(*,*) '  approximation is not implemented.'
  stop
#End

do n=1,3
  d14(n)=1./(12.*dr(n))
  d24(n)=1./(12.*dr(n)**2)
  h41(n)=1./(12.*dx(n))
  h42(n)=1./(12.*dx(n)**2)
end do

kd3=nd  

if( nd .eq. 2 )then
!       ************************
!       ******* 2D *************      
!       ************************
  #If #operator == "identity"
    beginLoops4()
      loopBody4thOrder2d(0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,1.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.)
    endLoops()
    return
  #Elif #operator == "r"
    beginLoops4()
     r4thOrder2dRectangular(x,1)
    endLoops()
    return
  #Elif #operator == "s"
    beginLoops4()
     r4thOrder2dRectangular(y,2)
    endLoops()
    return
  #Elif #operator == "rr"
    beginLoops4()
     rr4thOrder2dRectangular(x,1)
    endLoops()
    return
  #Elif #operator == "ss"
    beginLoops4()
     rr4thOrder2dRectangular(y,2)
    endLoops()
    return
  #Elif #operator == "rs"
    beginLoops4()
     d=d14(1)*d14(2)
     d8=d*8.
     d64=d*64.
     loopBody4thOrder2d(d,-d8,0.,d8,-d, -d8,d64,0.,-d64,d8, 0.,0.,0.,0.,0., d8,-d64,0.,d64,-d8, -d,d8,0.,-d8,d)
    endLoops()
    return
  #End

  if( gridType .eq. 0 )then
!   rectangular
    beginLoops4()
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
        d=h41(1)*h41(2)
        d8=d*8.
        d64=d*64.
        loopBody4thOrder2d(d,-d8,0.,d8,-d, -d8,d64,0.,-d64,d8, 0.,0.,0.,0.,0., d8,-d64,0.,d64,-d8, -d,d8,0.,-d8,d)
      #End
    endLoops()
  
  else
!  ***** not rectangular *****
    beginLoops4()
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
     #Elif #operator == "xy"
       xy4thOrder2d(x,y)
      #End
    endLoops()
  
  endif 
elseif( nd.eq.3 )then
!       ************************
!       ******* 3D *************      
!       ************************
  
  #If #operator == "identity"
    beginLoops4()
      loopBody4thOrder3d(\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,1.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.)
      endLoops()
    return
  #Elif #operator == "r"
    beginLoops4()
     r4thOrder3dRectangular(x,1)
    endLoops()
    return
  #Elif #operator == "s"
    beginLoops4()
     r4thOrder3dRectangular(y,2)
    endLoops()
    return
  #Elif #operator == "t"
    beginLoops4()
     r4thOrder3dRectangular(z,3)
    endLoops()
    return
  #Elif #operator == "rr"
    beginLoops4()
     rr4thOrder3dRectangular(x,1)
    endLoops()
    return
  #Elif #operator == "ss"
    beginLoops4()
     rr4thOrder3dRectangular(y,2)
    endLoops()
    return
  #Elif #operator == "tt"
    beginLoops4()
     rr4thOrder3dRectangular(z,3)
    endLoops()
    return
  #Elif #operator == "rs"
    beginLoops4()
     rs4thOrder3dRectangular(x,x,1,2)
    endLoops()
    return
  #Elif #operator == "rt"
    beginLoops4()
     rs4thOrder3dRectangular(y,z,1,3)
    endLoops()
    return
  #Elif #operator == "st"
    beginLoops4()
     rs4thOrder3dRectangular(x,z,2,3)
    endLoops()
    return
  #End
  if( gridType .eq. 0 )then
!   rectangular
    beginLoops4()
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
       xx4thOrder3dRectangular(x,1)
     #Elif #operator == "yy"
       xx4thOrder3dRectangular(y,2)
     #Elif #operator == "zz"
       xx4thOrder3dRectangular(z,3)
     #Elif #operator == "xy"
       xy4thOrder3dRectangular(x,y,1,2)
     #Elif #operator == "xz"
       xy4thOrder3dRectangular(x,z,1,3)
     #Elif #operator == "yz"
       xy4thOrder3dRectangular(y,z,2,3)
     #End
    endLoops()
  
  else
!  ***** not rectangular *****
    beginLoops4()
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
       xy4thOrder3d(x,z)
     #Elif #operator == "yz"
       xy4thOrder3d(y,z)
     #End
    endLoops()
  
  end if
  
  
elseif( nd.eq.1 )then
!       ************************
!       ******* 1D *************      
!       ************************
  #If #operator == "identity"
    beginLoops4()
      loopBody4thOrder1d(0.,0.,1.,0.,0.)
    endLoops()
    return
  #Elif #operator == "rr"
    beginLoops4()
      loopBody4thOrder1d(-d24(1),16.* d24(1),-30.*d24(1),16.* d24(1),-d24(1))
    endLoops()
    return
  #Elif #operator == "r"
    beginLoops4()
      loopBody4thOrder1d(d14(1),-8.*d14(1),0.,8.* d14(1),-d14(1))
    endLoops()
    return
  #End
  if( gridType .eq. 0 )then
!   rectangular
    beginLoops4()
     #If #operator == "laplacian" || #operator == "xx"
       loopBody4thOrder1d(-h42(1),16.*h42(1),-30.*h42(1),16.*h42(1),-h42(1))
     #Elif #operator == "x"
       loopBody4thOrder1d(h41(1),-8.*h41(1),0.,8.*h41(1),-h41(1))
     #End
    endLoops()
  else
!  ***** not rectangular *****
    beginLoops4()
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
  
  else if( nd.eq.0 )then
!       *** add these lines to avoid warnings about unused statement functions
    include "cgux4afNoWarnings.h" 
    temp=rxx1(i1,i2,i3)
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
      buildFile4(divScalarGrad)

      buildFile4(r)
      buildFile4(s)
      buildFile4(t)
      buildFile4(rr)
      buildFile4(rs)
      buildFile4(rt)
      buildFile4(ss)
      buildFile4(st)
      buildFile4(tt)

! done 4th order






! ****************************************************************************************
! ************************* 6th order ****************************************************
! ****************************************************************************************

!  wdh: *new* Aug 27, 2016 -- **finish me**

#beginMacro beginLoops6()
! ***** loop over equations and components *****
do e=ea,eb
do c=ca,cb
ec=ns*(c+nc*e)
! ** it did not affect performance to use an array to index coeff ***
if( nd.eq.2 )then
do i2=-halfWidth,halfWidth
  do i1=-halfWidth,halfWidth
   m(i1,i2)=i1+halfWidth+width*(i2+halfWidth) +1 + ec
  end do
end do
else if( nd.eq.3 )then
do i3=-halfWidth,halfWidth
  do i2=-halfWidth,halfWidth
    do i1=-halfWidth,halfWidth
      m3(i1,i2,i3)=i1+halfWidth+width*(i2+halfWidth+width*(i3+halfWidth)) +1 + ec
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


#beginMacro loopBody6thOrder1d(c0,c1,c2,c3,c4)
 coeff(m12,i1,i2,i3)=c0
 coeff(m22,i1,i2,i3)=c1
 coeff(m32,i1,i2,i3)=c2
 coeff(m42,i1,i2,i3)=c3
 coeff(m52,i1,i2,i3)=c4
#endMacro

#beginMacro loopBody6thOrder2d(\
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
#beginMacro loopBody6thOrder2dSwitchxx(\
 c00,c10,c20,c30,c40,\
 c01,c11,c21,c31,c41,\
 c02,c12,c22,c32,c42,\
 c03,c13,c23,c33,c43,\
 c04,c14,c24,c34,c44)
loopBody6thOrder2d(\
 c00,c10,c20,c30,c40,\
 c01,c11,c21,c31,c41,\
 c02,c12,c22,c32,c42,\
 c03,c13,c23,c33,c43,\
 c04,c14,c24,c34,c44)
#endMacro

! This macro will switch the roles of x and y in the arguments
#beginMacro loopBody6thOrder2dSwitchxy(\
 c00,c10,c20,c30,c40,\
 c01,c11,c21,c31,c41,\
 c02,c12,c22,c32,c42,\
 c03,c13,c23,c33,c43,\
 c04,c14,c24,c34,c44)
loopBody6thOrder2d(\
 c00,c01,c02,c03,c04,\
 c10,c11,c12,c13,c14,\
 c20,c21,c22,c23,c24,\
 c30,c31,c32,c33,c34,\
 c40,c41,c42,c43,c44)
#endMacro


#beginMacro loopBody6thOrder3d(\
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
#beginMacro loopBody6thOrder3dSwitchxx(\
 c000,c100,c200,c300,c400, c010,c110,c210,c310,c410, c020,c120,c220,c320,c420, c030,c130,c230,c330,c430,\
 c040,c140,c240,c340,c440, c001,c101,c201,c301,c401, c011,c111,c211,c311,c411, c021,c121,c221,c321,c421,\
 c031,c131,c231,c331,c431, c041,c141,c241,c341,c441, c002,c102,c202,c302,c402, c012,c112,c212,c312,c412,\
 c022,c122,c222,c322,c422, c032,c132,c232,c332,c432, c042,c142,c242,c342,c442, c003,c103,c203,c303,c403,\
 c013,c113,c213,c313,c413, c023,c123,c223,c323,c423, c033,c133,c233,c333,c433, c043,c143,c243,c343,c443,\
 c004,c104,c204,c304,c404, c014,c114,c214,c314,c414, c024,c124,c224,c324,c424, c034,c134,c234,c334,c434,\
 c044,c144,c244,c344,c444)
loopBody6thOrder3d(\
 c000,c100,c200,c300,c400, c010,c110,c210,c310,c410, c020,c120,c220,c320,c420, c030,c130,c230,c330,c430,\
 c040,c140,c240,c340,c440, c001,c101,c201,c301,c401, c011,c111,c211,c311,c411, c021,c121,c221,c321,c421,\
 c031,c131,c231,c331,c431, c041,c141,c241,c341,c441, c002,c102,c202,c302,c402, c012,c112,c212,c312,c412,\
 c022,c122,c222,c322,c422, c032,c132,c232,c332,c432, c042,c142,c242,c342,c442, c003,c103,c203,c303,c403,\
 c013,c113,c213,c313,c413, c023,c123,c223,c323,c423, c033,c133,c233,c333,c433, c043,c143,c243,c343,c443,\
 c004,c104,c204,c304,c404, c014,c114,c214,c314,c414, c024,c124,c224,c324,c424, c034,c134,c234,c334,c434,\
 c044,c144,c244,c344,c444)
#endMacro
! This macro will switch the roles of x and y in the arguments
#beginMacro loopBody6thOrder3dSwitchxy(\
 c000,c100,c200,c300,c400, c010,c110,c210,c310,c410, c020,c120,c220,c320,c420, c030,c130,c230,c330,c430,\
 c040,c140,c240,c340,c440, c001,c101,c201,c301,c401, c011,c111,c211,c311,c411, c021,c121,c221,c321,c421,\
 c031,c131,c231,c331,c431, c041,c141,c241,c341,c441, c002,c102,c202,c302,c402, c012,c112,c212,c312,c412,\
 c022,c122,c222,c322,c422, c032,c132,c232,c332,c432, c042,c142,c242,c342,c442, c003,c103,c203,c303,c403,\
 c013,c113,c213,c313,c413, c023,c123,c223,c323,c423, c033,c133,c233,c333,c433, c043,c143,c243,c343,c443,\
 c004,c104,c204,c304,c404, c014,c114,c214,c314,c414, c024,c124,c224,c324,c424, c034,c134,c234,c334,c434,\
 c044,c144,c244,c344,c444)
loopBody6thOrder3d(\
 c000,c010,c020,c030,c040, c100,c110,c120,c130,c140, c200,c210,c220,c230,c240, c300,c310,c320,c330,c340,\
 c400,c410,c420,c430,c440, c001,c011,c021,c031,c041, c101,c111,c121,c131,c141, c201,c211,c221,c231,c241,\
 c301,c311,c321,c331,c341, c401,c411,c421,c431,c441, c002,c012,c022,c032,c042, c102,c112,c122,c132,c142,\
 c202,c212,c222,c232,c242, c302,c312,c322,c332,c342, c402,c412,c422,c432,c442, c003,c013,c023,c033,c043,\
 c103,c113,c123,c133,c143, c203,c213,c223,c233,c243, c303,c313,c323,c333,c343, c403,c413,c423,c433,c443,\
 c004,c014,c024,c034,c044, c104,c114,c124,c134,c144, c204,c214,c224,c234,c244, c304,c314,c324,c334,c344,\
 c404,c414,c424,c434,c444)
#endMacro
! This macro will switch the roles of x and z in the arguments
#beginMacro loopBody6thOrder3dSwitchxz(\
 c000,c100,c200,c300,c400, c010,c110,c210,c310,c410, c020,c120,c220,c320,c420, c030,c130,c230,c330,c430,\
 c040,c140,c240,c340,c440, c001,c101,c201,c301,c401, c011,c111,c211,c311,c411, c021,c121,c221,c321,c421,\
 c031,c131,c231,c331,c431, c041,c141,c241,c341,c441, c002,c102,c202,c302,c402, c012,c112,c212,c312,c412,\
 c022,c122,c222,c322,c422, c032,c132,c232,c332,c432, c042,c142,c242,c342,c442, c003,c103,c203,c303,c403,\
 c013,c113,c213,c313,c413, c023,c123,c223,c323,c423, c033,c133,c233,c333,c433, c043,c143,c243,c343,c443,\
 c004,c104,c204,c304,c404, c014,c114,c214,c314,c414, c024,c124,c224,c324,c424, c034,c134,c234,c334,c434,\
 c044,c144,c244,c344,c444)
loopBody6thOrder3d(\
 c000,c001,c002,c003,c004, c010,c011,c012,c013,c014, c020,c021,c022,c023,c024, c030,c031,c032,c033,c034,\
 c040,c041,c042,c043,c044, c100,c101,c102,c103,c104, c110,c111,c112,c113,c114, c120,c121,c122,c123,c124,\
 c130,c131,c132,c133,c134, c140,c141,c142,c143,c144, c200,c201,c202,c203,c204, c210,c211,c212,c213,c214,\
 c220,c221,c222,c223,c224, c230,c231,c232,c233,c234, c240,c241,c242,c243,c244, c300,c301,c302,c303,c304,\
 c310,c311,c312,c313,c314, c320,c321,c322,c323,c324, c330,c331,c332,c333,c334, c340,c341,c342,c343,c344,\
 c400,c401,c402,c403,c404, c410,c411,c412,c413,c414, c420,c421,c422,c423,c424, c430,c431,c432,c433,c434,\
 c440,c441,c442,c443,c444)
#endMacro
! This macro will switch the roles of y and z in the arguments
#beginMacro loopBody6thOrder3dSwitchyz(\
 c000,c100,c200,c300,c400, c010,c110,c210,c310,c410, c020,c120,c220,c320,c420, c030,c130,c230,c330,c430,\
 c040,c140,c240,c340,c440, c001,c101,c201,c301,c401, c011,c111,c211,c311,c411, c021,c121,c221,c321,c421,\
 c031,c131,c231,c331,c431, c041,c141,c241,c341,c441, c002,c102,c202,c302,c402, c012,c112,c212,c312,c412,\
 c022,c122,c222,c322,c422, c032,c132,c232,c332,c432, c042,c142,c242,c342,c442, c003,c103,c203,c303,c403,\
 c013,c113,c213,c313,c413, c023,c123,c223,c323,c423, c033,c133,c233,c333,c433, c043,c143,c243,c343,c443,\
 c004,c104,c204,c304,c404, c014,c114,c214,c314,c414, c024,c124,c224,c324,c424, c034,c134,c234,c334,c434,\
 c044,c144,c244,c344,c444)
loopBody6thOrder3d(\
 c000,c100,c200,c300,c400, c001,c101,c201,c301,c401, c002,c102,c202,c302,c402, c003,c103,c203,c303,c403,\
 c004,c104,c204,c304,c404, c010,c110,c210,c310,c410, c011,c111,c211,c311,c411, c012,c112,c212,c312,c412,\
 c013,c113,c213,c313,c413, c014,c114,c214,c314,c414, c020,c120,c220,c320,c420, c021,c121,c221,c321,c421,\
 c022,c122,c222,c322,c422, c023,c123,c223,c323,c423, c024,c124,c224,c324,c424, c030,c130,c230,c330,c430,\
 c031,c131,c231,c331,c431, c032,c132,c232,c332,c432, c033,c133,c233,c333,c433, c034,c134,c234,c334,c434,\
 c040,c140,c240,c340,c440, c041,c141,c241,c341,c441, c042,c142,c242,c342,c442, c043,c143,c243,c343,c443,\
 c044,c144,c244,c344,c444)
#endMacro

! define a macro for x, y in 2d rectangular
#beginMacro x6thOrder2dRectangular(x,axis)
loopBody6thOrder2dSwitchx ## x(0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
                          h41(axis),-8.*h41(axis),0.,8.* h41(axis),-h41(axis),\
                          0.,0.,0.,0.,0., 0.,0.,0.,0.,0.) 
#endMacro

! define a macro for xx, yy in 2d rectangular
#beginMacro xx6thOrder2dRectangular(x,axis)
loopBody6thOrder2dSwitchx ## x(0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
                          -h42(axis),16.* h42(axis),-30.*h42(axis),16.* h42(axis),-h42(axis),\
                          0.,0.,0.,0.,0., 0.,0.,0.,0.,0.) 
#endMacro

! define a macro for r, s in 2d rectangular
#beginMacro r6thOrder2dRectangular(x,axis)
loopBody6thOrder2dSwitchx ## x(0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
                          d14(axis),-8.*d14(axis),0.,8.* d14(axis),-d14(axis),\
                          0.,0.,0.,0.,0., 0.,0.,0.,0.,0.) 
#endMacro

! define a macro for rr, ss in 2d rectangular
#beginMacro rr6thOrder2dRectangular(x,axis)
loopBody6thOrder2dSwitchx ## x(0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
                          -d24(axis),16.* d24(axis),-30.*d24(axis),16.* d24(axis),-d24(axis),\
                          0.,0.,0.,0.,0., 0.,0.,0.,0.,0.) 
#endMacro
! define a macro for x, y in 2d
#beginMacro x6thOrder2d(x)
rx4 = d14(1)*(r x(i1,i2,i3))
sx4 = d14(2)*(s x(i1,i2,i3))
tx4 = d14(3)*(t x(i1,i2,i3))
loopBody6thOrder2d(\
   0.,0.,sx4,0.,0., 0.,0.,-8.*sx4,0.,0., rx4,-8.*rx4,0.,8.*rx4,-rx4, 0.,0.,8.*sx4,0.,0., 0.,0.,-sx4,0.,0.)
#endMacro

! define a macro for xx, yy in 2d
#beginMacro xx6thOrder2d(x)
rxSq=d24(1)*(r x(i1,i2,i3)**2)
rxxyy=d14(1)*(r x x(i1,i2,i3))
sxSq=d24(2)*(s x(i1,i2,i3)**2)
sxxyy=d14(2)*(s x x(i1,i2,i3))
rsx =(2.*d14(1)*d14(2))*(r x(i1,i2,i3)*s x(i1,i2,i3))
rsx8  = rsx*8. 
rsx64 = rsx*64.

loopBody6thOrder2d(rsx,-rsx8,-sxSq+sxxyy,rsx8,-rsx,\
                   -rsx8,rsx64,16.*sxSq -8.*sxxyy,-  rsx64,rsx8,\
                   -rxSq+rxxyy,16.*rxSq-8.*rxxyy,-30.*(rxSq+sxSq),16.*rxSq+8.*rxxyy,-rxSq-rxxyy,\
                   rsx8,-rsx64,16.*sxSq +8.*sxxyy,rsx64,-rsx8,\
                   -rsx,rsx8,-sxSq-sxxyy,-rsx8,rsx)
#endMacro


#beginMacro xy6thOrder2d(X,Y)
rxsq = d24(1)*(r X(i1,i2,i3)*r Y(i1,i2,i3))
rxxyy= d14(1)*(r X Y (i1,i2,i3))
sxsq = d24(2)*(s X(i1,i2,i3)*s Y(i1,i2,i3))
sxxyy=d14(2)*(s X Y (i1,i2,i3))
rsx = (d14(1)*d14(2))*(r X(i1,i2,i3)*s Y(i1,i2,i3)+r Y(i1,i2,i3)*s X(i1,i2,i3))
! check this	
loopBody6thOrder2d(\
  rsx,-8.*rsx,-sxSq+sxxyy,8.*rsx,-rsx, -8.*rsx,64.*rsx,16.*sxSq-8.*sxxyy,-64.*rsx,8.*rsx, \
   -rxSq+rxxyy,16.*rxSq-8.*rxxyy,-30.*(rxSq+sxSq),16.*rxSq+8.*rxxyy,-rxSq-rxxyy,\
      8.*rsx,-64.*rsx,16.*sxSq+8.*sxxyy,64.*rsx,-8.*rsx, -rsx,8.*rsx,-sxSq-sxxyy,-8.*rsx,rsx)
#endMacro



! define a macro for x, y in 3d rectangular
#beginMacro x6thOrder3dRectangular(x,axis)
loopBody6thOrder3dSwitchx ## x(\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., h41(axis),-8.*h41(axis),0.,8.* h41(axis),-h41(axis),\
                                               0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.)
#endMacro

! define a macro for xx, yy in 3d rectangular
#beginMacro xx6thOrder3dRectangular(x,axis)
loopBody6thOrder3dSwitchx ## x(\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., -h42(axis),16.* h42(axis),-30.*h42(axis),16.* h42(axis),-h42(axis),\
                                               0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.)
#endMacro

! define a macro for xy, xz, yz in 3d rectangular
!  for derivative xy use (x,x,1,2)
!  for derivative xz use (y,z,1,3)
!  for derivative yz use (x,z,2,3)
#beginMacro xy6thOrder3dRectangular(xd,yd,axis1,axis2)
d=h41(axis1)*h41(axis2)
d8=d*8.
d64=d*64.
loopBody6thOrder3dSwitch ## xd ## yd(\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         d,-d8,0.,d8,-d, -d8,d64,0.,-d64,d8, 0.,0.,0.,0.,0., d8,-d64,0.,d64,-d8, -d,d8,0.,-d8,d,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.)
#endMacro


! define a macro for r, s, t in 3d rectangular
#beginMacro r6thOrder3dRectangular(x,axis)
loopBody6thOrder3dSwitchx ## x(\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., d14(axis),-8.*d14(axis),0.,8.* d14(axis),-d14(axis),\
                                               0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.)
#endMacro

! define a macro for rr, ss, tt in 3d rectangular
#beginMacro rr6thOrder3dRectangular(x,axis)
loopBody6thOrder3dSwitchx ## x(\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., -d24(axis),16.* d24(axis),-30.*d24(axis),16.* d24(axis),-d24(axis),\
                                               0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.)
#endMacro

! define a macro for rs, rt, st in 3d rectangular
!  for derivative rs use (x,x,1,2)
!  for derivative rt use (y,z,1,3)
!  for derivative st use (x,z,2,3)
#beginMacro rs6thOrder3dRectangular(xd,yd,axis1,axis2)
d=d14(axis1)*d14(axis2)
d8=d*8.
d64=d*64.
loopBody6thOrder3dSwitch ## xd ## yd(\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         d,-d8,0.,d8,-d, -d8,d64,0.,-d64,d8, 0.,0.,0.,0.,0., d8,-d64,0.,d64,-d8, -d,d8,0.,-d8,d,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.)
#endMacro



! define a macro for x, y, z in 3d
#beginMacro x6thOrder3d(x)
rx4 = d14(1)*(r x(i1,i2,i3))
sx4 = d14(2)*(s x(i1,i2,i3))
tx4 = d14(3)*(t x(i1,i2,i3))
loopBody6thOrder3d(\
   0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,tx4,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
   0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,-8.*tx4,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
   0.,0.,sx4,0.,0., 0.,0.,-8.*sx4,0.,0., rx4,-8.*rx4,0.,8.*rx4,-rx4, 0.,0.,8.*sx4,0.,0., 0.,0.,-sx4,0.,0.,\
   0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,8.*tx4,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
   0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,-tx4,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.)
#endMacro


! define a macro for xx, yy, zz in 3d
#beginMacro xx6thOrder3d(x)
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
loopBody6thOrder3d(\
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

#beginMacro xy6thOrder3d(X,Y)
rxsq = d24(1)*(r X(i1,i2,i3)*r Y(i1,i2,i3))
rxxyy=d14(1)*(r X Y 3(i1,i2,i3))
sxsq = d24(2)*(s X(i1,i2,i3)*s Y(i1,i2,i3))
sxxyy=d14(2)*(s X Y 3(i1,i2,i3))
txsq = d24(3)*(t X(i1,i2,i3)*t Y(i1,i2,i3))
txxyy=d14(3)*(t X Y 3(i1,i2,i3))
rsx = (d14(1)*d14(2))*(r X(i1,i2,i3)*s Y(i1,i2,i3)+r Y(i1,i2,i3)*s X(i1,i2,i3))
rtx = (d14(1)*d14(3))*(r X(i1,i2,i3)*t Y(i1,i2,i3)+r Y(i1,i2,i3)*t X(i1,i2,i3))
stx = (d14(2)*d14(3))*(s X(i1,i2,i3)*t Y(i1,i2,i3)+s Y(i1,i2,i3)*t X(i1,i2,i3))
rsx8  = rsx*8. 
rsx64 = rsx*64.
rtx8  = rtx*8. 
rtx64 = rtx*64.
stx8  = stx*8. 
stx64 = stx*64.
loopBody6thOrder3d(\
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



#beginMacro coeffOperator6thOrder(operator)
subroutine operator ## Coeff6thOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,\
 nds1a,nds1b,nds2a,nds2b,nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc,nc,ns,ea,eb,ca,cb,\
 dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s, jac, averagingType, dir1, dir2,\
 a11,a22,a12,a21,a33,a13,a23,a31,a32 )
! ===============================================================
!  Derivative Coefficients - 6th order version
!  
! gridType: 0=rectangular, 1=non-rectangular
! rsxy : not used if rectangular
! h42 : 1/h**2 : for rectangular  
! ===============================================================

!      implicit none
integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb, ca,cb,gridType,order
integer ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,nds2b,nds3a,nds3b
integer derivOption, derivType, averagingType, dir1, dir2 
real dx(3),dr(3)
real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,1:nd,1:nd)
real coeff(1:ndc,ndc1a:ndc1b,ndc2a:ndc2b,ndc3a:ndc3b)
! *wdh* 2016/08/27 real s(nds1a:nds1b,nds2a:nds2b,nds3a:nds3b)
real s(nds1a:nds1b,nds2a:nds2b,nds3a:nds3b,0:*)
real jac(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
real a11(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
real a12(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
real a13(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
real a21(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
real a22(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
real a23(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
real a31(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
real a32(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
real a33(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)

!..... added by kkc 1/2/02 for g77 unsatisfied reference
real u(1,1,1,1)

real d24(3),d14(3),h42(3),h41(3)
real d26(3),d16(3),h62(3),h61(3)
integer i1,i2,i3,kd3,kd,kdd,e,c,ec,j1,j2,j3
integer m12,m22,m32,m42,m52
integer width,halfWidth

integer m(-3:3,-3:3),m3(-3:3,-3:3,-3:3)

integer laplace,divScalarGrad,derivativeScalarDerivative
parameter(laplace=0,divScalarGrad=1,derivativeScalarDerivative=2)
integer arithmeticAverage,harmonicAverage
parameter( arithmeticAverage=0,harmonicAverage=1 ) 
integer symmetric
parameter( symmetric=2 )

!....statement functions for jacobian
rx(i1,i2,i3)=rsxy(i1,i2,i3,  1,  1)
ry(i1,i2,i3)=rsxy(i1,i2,i3,  1,  2)
rz(i1,i2,i3)=rsxy(i1,i2,i3,  1,kd3)
sx(i1,i2,i3)=rsxy(i1,i2,i3,  2,  1)
sy(i1,i2,i3)=rsxy(i1,i2,i3,  2,  2)
sz(i1,i2,i3)=rsxy(i1,i2,i3,  2,kd3)
tx(i1,i2,i3)=rsxy(i1,i2,i3,kd3,  1)
ty(i1,i2,i3)=rsxy(i1,i2,i3,kd3,  2)
tz(i1,i2,i3)=rsxy(i1,i2,i3,kd3,kd3)

include 'cgux4af.h'
rxx1(i1,i2,i3)=rx(i1,i2,i3)*rxr(i1,i2,i3)

!.....end statement functions


#If #operator == "laplacian"
  if( gridType .eq. 0 )then
    ! This case is implemented
  else
    write(*,*) 'opcoeff: order=6  finish me!'
    stop 1189
  end if
#Else
  if( .true. )then
    write(*,*) 'opcoeff: order=6  finish me!'
    stop 1190
  end if
#End


if( order.ne.6 )then
  write(*,*) 'opcoeff: ERROR: order!=6 '
  stop 1191
end if

! stencil width and "half-width"
width=7
halfWidth=3

#If #operator == "divScalarGrad"
  write(*,*) 'divScalarGradCoeff6:ERROR: not implemented'
  write(*,*) '  The requested 6th order conservative'
  write(*,*) '  approximation is not implemented.'
  stop
#End

! keep d14, d24, etc. for now ... while converting to order=6
do n=1,3
  d14(n)=1./(12.*dr(n))
  d24(n)=1./(12.*dr(n)**2)
  h41(n)=1./(12.*dx(n))
  h42(n)=1./(12.*dx(n)**2)

  h62(n)=1./(180.*dx(n)**2)
end do

kd3=nd  

if( nd .eq. 2 )then
!       ************************
!       ******* 2D *************      
!       ************************
  #If #operator == "identity"
    beginLoops6()
      loopBody6thOrder2d(0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,1.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.)
    endLoops()
    return
  #Elif #operator == "r"
    beginLoops6()
     r6thOrder2dRectangular(x,1)
    endLoops()
    return
  #Elif #operator == "s"
    beginLoops6()
     r6thOrder2dRectangular(y,2)
    endLoops()
    return
  #Elif #operator == "rr"
    beginLoops6()
     rr6thOrder2dRectangular(x,1)
    endLoops()
    return
  #Elif #operator == "ss"
    beginLoops6()
     rr6thOrder2dRectangular(y,2)
    endLoops()
    return
  #Elif #operator == "rs"
    beginLoops6()
     d=d14(1)*d14(2)
     d8=d*8.
     d64=d*64.
     loopBody6thOrder2d(d,-d8,0.,d8,-d, -d8,d64,0.,-d64,d8, 0.,0.,0.,0.,0., d8,-d64,0.,d64,-d8, -d,d8,0.,-d8,d)
    endLoops()
    return
  #End

  if( gridType .eq. 0 )then
!   rectangular
    beginLoops6()
      #If #operator == "laplacian"
       ! loopBody6thOrder2d(0.,0.,-h42(2),0.,0., 0.,0.,16.*h42(2),0.,0.,\
       !                   -h42(1),16.* h42(1),-30.*(h42(1)+h42(2)),16.* h42(1),-h42(1),\
       !                   0.,0.,16.*h42(2),0.,0., 0.,0.,-h42(2),0.,0.) 
  
      ! do this for now *wdh* 2016/08/27
      do j1=-halfWidth,halfWidth
      do j2=-halfWidth,halfWidth
        coeff(m(j1,j2),i1,i2,i3)=0.
      end do
      end do

      coeff(m( 0,-3),i1,i2,i3)=                    2.*h62(2)
      coeff(m( 0,-2),i1,i2,i3)=                  -27.*h62(2)
      coeff(m( 0,-1),i1,i2,i3)=                  270.*h62(2)
      
      coeff(m(-3, 0),i1,i2,i3)=   2.*h62(1)
      coeff(m(-2, 0),i1,i2,i3)= -27.*h62(1)
      coeff(m(-1, 0),i1,i2,i3)= 270.*h62(1)
      coeff(m( 0, 0),i1,i2,i3)=-490.*(h62(1)     +h62(2))
      coeff(m( 1, 0),i1,i2,i3)= 270.*h62(1)
      coeff(m( 2, 0),i1,i2,i3)= -27.*h62(1)
      coeff(m( 3, 0),i1,i2,i3)=   2.*h62(1)
      
      coeff(m( 0, 1),i1,i2,i3)=                 270.*h62(2)
      coeff(m( 0, 2),i1,i2,i3)=                 -27.*h62(2)
      coeff(m( 0, 3),i1,i2,i3)=                   2.*h62(2)


      #Elif #operator == "x"
        x6thOrder2dRectangular(x,1)
      #Elif #operator == "y"
        x6thOrder2dRectangular(y,2)
      #Elif #operator == "xx"
        xx6thOrder2dRectangular(x,1)
      #Elif #operator == "yy"
        xx6thOrder2dRectangular(y,2)
      #Elif #operator == "xy"
        d=h41(1)*h41(2)
        d8=d*8.
        d64=d*64.
        loopBody6thOrder2d(d,-d8,0.,d8,-d, -d8,d64,0.,-d64,d8, 0.,0.,0.,0.,0., d8,-d64,0.,d64,-d8, -d,d8,0.,-d8,d)
      #End
    endLoops()
  
  else
!  ***** not rectangular *****
    beginLoops6()
      #If #operator == "laplacian"
       rxSq=d24(1)*(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)
       rxxyy=d14(1)*(rxx(i1,i2,i3)+ryy(i1,i2,i3)) 
       sxSq=d24(2)*(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2)
       sxxyy=d14(2)*(sxx(i1,i2,i3)+syy(i1,i2,i3))
       rsx =(2.*d14(1)*d14(2))*(rx(i1,i2,i3)*sx(i1,i2,i3)+ry(i1,i2,i3)*sy(i1,i2,i3))
       rsx8  = rsx*8. 
       rsx64 = rsx*64.
       loopBody6thOrder2d(rsx,-rsx8,-sxSq+sxxyy,rsx8,-rsx,\
                          -rsx8,rsx64,16.*sxSq -8.*sxxyy,-  rsx64,rsx8,\
                          -rxSq+rxxyy,16.*rxSq-8.*rxxyy,-30.*(rxSq+sxSq),16.*rxSq+8.*rxxyy,-rxSq-rxxyy,\
                          rsx8,-rsx64,16.*sxSq +8.*sxxyy,rsx64,-rsx8,\
                          -rsx,rsx8,-sxSq-sxxyy,-rsx8,rsx)
      #Elif #operator == "x"
        x6thOrder2d(x)
      #Elif #operator == "y"
        x6thOrder2d(y)
      #Elif #operator == "xx"
        xx6thOrder2d(x)
      #Elif #operator == "yy"
        xx6thOrder2d(y)
     #Elif #operator == "xy"
       xy6thOrder2d(x,y)
      #End
    endLoops()
  
  endif 
elseif( nd.eq.3 )then
!       ************************
!       ******* 3D *************      
!       ************************
  
  #If #operator == "identity"
    beginLoops6()
      loopBody6thOrder3d(\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,1.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.)
      endLoops()
    return
  #Elif #operator == "r"
    beginLoops6()
     r6thOrder3dRectangular(x,1)
    endLoops()
    return
  #Elif #operator == "s"
    beginLoops6()
     r6thOrder3dRectangular(y,2)
    endLoops()
    return
  #Elif #operator == "t"
    beginLoops6()
     r6thOrder3dRectangular(z,3)
    endLoops()
    return
  #Elif #operator == "rr"
    beginLoops6()
     rr6thOrder3dRectangular(x,1)
    endLoops()
    return
  #Elif #operator == "ss"
    beginLoops6()
     rr6thOrder3dRectangular(y,2)
    endLoops()
    return
  #Elif #operator == "tt"
    beginLoops6()
     rr6thOrder3dRectangular(z,3)
    endLoops()
    return
  #Elif #operator == "rs"
    beginLoops6()
     rs6thOrder3dRectangular(x,x,1,2)
    endLoops()
    return
  #Elif #operator == "rt"
    beginLoops6()
     rs6thOrder3dRectangular(y,z,1,3)
    endLoops()
    return
  #Elif #operator == "st"
    beginLoops6()
     rs6thOrder3dRectangular(x,z,2,3)
    endLoops()
    return
  #End
  if( gridType .eq. 0 )then
!   rectangular
    beginLoops6()
     #If #operator == "laplacian"
!      loopBody6thOrder3d(\
!         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,-h42(3),0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
!         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,16.*h42(3),0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
!         0.,0.,-h42(2),0.,0., 0.,0.,16.*h42(2),0.,0., \
!                       -h42(1),16.* h42(1),-30.*(h42(1)+h42(2)+h42(3)),16.* h42(1),-h42(1), \
!                                               0.,0.,16.*h42(2),0.,0., 0.,0.,-h42(2),0.,0.,\
!         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,16.*h42(3),0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,\
!         0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,-h42(3),0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.)

      ! do this for now *wdh* 2016/08/27
      do j3=-halfWidth,halfWidth
      do j2=-halfWidth,halfWidth
      do j1=-halfWidth,halfWidth
        coeff(m3(j1,j2,j3),i1,i2,i3)=0.
      end do
      end do
      end do

      coeff(m3( 0, 0,-3),i1,i2,i3)=                                   2.*h62(3)
      coeff(m3( 0, 0,-2),i1,i2,i3)=                                 -27.*h62(3)
      coeff(m3( 0, 0,-1),i1,i2,i3)=                                 270.*h62(3)
      
      coeff(m3( 0,-3, 0),i1,i2,i3)=                   2.*h62(2)
      coeff(m3( 0,-2, 0),i1,i2,i3)=                 -27.*h62(2)
      coeff(m3( 0,-1, 0),i1,i2,i3)=                 270.*h62(2)
      
      coeff(m3(-3, 0, 0),i1,i2,i3)=   2.*h62(1)
      coeff(m3(-2, 0, 0),i1,i2,i3)= -27.*h62(1)
      coeff(m3(-1, 0, 0),i1,i2,i3)= 270.*h62(1)
      coeff(m3( 0, 0, 0),i1,i2,i3)=-490.*(h62(1)     +h62(2)     +h62(3))
      coeff(m3( 1, 0, 0),i1,i2,i3)= 270.*h62(1)
      coeff(m3( 2, 0, 0),i1,i2,i3)= -27.*h62(1)
      coeff(m3( 3, 0, 0),i1,i2,i3)=   2.*h62(1)
      
      coeff(m3( 0, 1, 0),i1,i2,i3)=                 270.*h62(2)
      coeff(m3( 0, 2, 0),i1,i2,i3)=                 -27.*h62(2)
      coeff(m3( 0, 3, 0),i1,i2,i3)=                   2.*h62(2)
      
      coeff(m3( 0, 0, 1),i1,i2,i3)=                                 270.*h62(3)
      coeff(m3( 0, 0, 2),i1,i2,i3)=                                 -27.*h62(3)
      coeff(m3( 0, 0, 3),i1,i2,i3)=                                   2.*h62(3)


     #Elif #operator == "x"
       x6thOrder3dRectangular(x,1)
     #Elif #operator == "y"
       x6thOrder3dRectangular(y,2)
     #Elif #operator == "z"
       x6thOrder3dRectangular(z,3)
     #Elif #operator == "xx"
       xx6thOrder3dRectangular(x,1)
     #Elif #operator == "yy"
       xx6thOrder3dRectangular(y,2)
     #Elif #operator == "zz"
       xx6thOrder3dRectangular(z,3)
     #Elif #operator == "xy"
       xy6thOrder3dRectangular(x,y,1,2)
     #Elif #operator == "xz"
       xy6thOrder3dRectangular(x,z,1,3)
     #Elif #operator == "yz"
       xy6thOrder3dRectangular(y,z,2,3)
     #End
    endLoops()
  
  else
!  ***** not rectangular *****
    beginLoops6()
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
      loopBody6thOrder3d(\
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
       x6thOrder3d(x)
     #Elif #operator == "y"
       x6thOrder3d(y)
     #Elif #operator == "z"
       x6thOrder3d(z)
     #Elif #operator == "xx"
       xx6thOrder3d(x)
     #Elif #operator == "yy"
       xx6thOrder3d(y)
     #Elif #operator == "zz"
       xx6thOrder3d(z)
     #Elif #operator == "xy"
       xy6thOrder3d(x,y)
     #Elif #operator == "xz"
       xy6thOrder3d(x,z)
     #Elif #operator == "yz"
       xy6thOrder3d(y,z)
     #End
    endLoops()
  
  end if
  
  
elseif( nd.eq.1 )then
!       ************************
!       ******* 1D *************      
!       ************************
  #If #operator == "identity"
    beginLoops6()
      loopBody6thOrder1d(0.,0.,1.,0.,0.)
    endLoops()
    return
  #Elif #operator == "rr"
    beginLoops6()
      loopBody6thOrder1d(-d24(1),16.* d24(1),-30.*d24(1),16.* d24(1),-d24(1))
    endLoops()
    return
  #Elif #operator == "r"
    beginLoops6()
      loopBody6thOrder1d(d14(1),-8.*d14(1),0.,8.* d14(1),-d14(1))
    endLoops()
    return
  #End
  if( gridType .eq. 0 )then
!   rectangular
    beginLoops6()
     #If #operator == "laplacian" || #operator == "xx"
       ! loopBody6thOrder1d(-h42(1),16.*h42(1),-30.*h42(1),16.*h42(1),-h42(1))
      j2=0
      do j1=-halfWidth,halfWidth
        coeff(m(j1,j2),i1,i2,i3)=0.
      end do

      coeff(m(-3, 0),i1,i2,i3)=   2.*h62(1)
      coeff(m(-2, 0),i1,i2,i3)= -27.*h62(1)
      coeff(m(-1, 0),i1,i2,i3)= 270.*h62(1)
      coeff(m( 0, 0),i1,i2,i3)=-490.*(h62(1)     +h62(2))
      coeff(m( 1, 0),i1,i2,i3)= 270.*h62(1)
      coeff(m( 2, 0),i1,i2,i3)= -27.*h62(1)
      coeff(m( 3, 0),i1,i2,i3)=   2.*h62(1)
      
     #Elif #operator == "x"
       loopBody6thOrder1d(h41(1),-8.*h41(1),0.,8.*h41(1),-h41(1))
     #End
    endLoops()
  else
!  ***** not rectangular *****
    beginLoops6()
     #If #operator == "laplacian" || #operator == "xx"
      rxSq=d24(1)*rx(i1,i2,i3)**2
      rxxyy=d14(1)*rxx1(i1,i2,i3)
      loopBody6thOrder1d(-rxSq+rxxyy,16.*rxSq-8.*rxxyy,-30.*rxSq,16.*rxSq+8.*rxxyy,-rxSq-rxxyy)
     #Elif #operator == "x"
      rx4 = d14(1)*(r x(i1,i2,i3))
      loopBody6thOrder1d(rx4,-8.*rx4,0.,8.*rx4,-rx4)
     #End
    endLoops()
  
  end if
  
  else if( nd.eq.0 )then
!       *** add these lines to avoid warnings about unused statement functions
    include "cgux4afNoWarnings.h" 
    temp=rxx1(i1,i2,i3)
  end if

return
end
#endMacro



#beginMacro buildFile6(x)
#beginFile x ## Coeff6.f
 coeffOperator6thOrder(x)
#endFile
#endMacro

      buildFile6(identity)
      buildFile6(laplacian)
      buildFile6(x)
      buildFile6(y)
      buildFile6(z)
      buildFile6(xx)
      buildFile6(xy)
      buildFile6(xz)
      buildFile6(yy)
      buildFile6(yz)
      buildFile6(zz)
      buildFile6(divScalarGrad)

      buildFile6(r)
      buildFile6(s)
      buildFile6(t)
      buildFile6(rr)
      buildFile6(rs)
      buildFile6(rt)
      buildFile6(ss)
      buildFile6(st)
      buildFile6(tt)

! done
