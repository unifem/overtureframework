! This file automatically generated from opcoeff.bf with bpp.
c The next include defines the macros for conservative approximations
! #Include "../include/defineConservative.h"
c Define macros for conservative approximations.
c These are used by the forward and inverse operators
c   includefd in files: dsg.bf and opcoeff.bf


c 



c --------------------------------------------------------------------------------------------

c This macro defines Da(sDb) where a=x,y and b=x,y

c 



c --------------------------------------------------------------------------------------------


c 



c --------------------------------------------------------------------------------------------

c  define a macro






c --------------------------------------------------------------------------------------------


c --------------------------------------------------------------------------------------------


c --------------------------------------------------------------------------------------------


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
c ===============================================================
c    Build Coefficients for Operators
c  *** This subroutine calls the appropriate sub for a particular operator ***
c  
c  nd : number of range spatial dimensions 
c  nd1a,nd1b : mesh dimensions axis 1 (dimensions for rsxy and jac)
c  nd2a,nd2b : mesh dimensions axis 2
c  nd3a,nd3b : mesh dimensions axis 3
c
c  ndc : number of coefficients/mesh point
c  ndc1a,ndc1b : coefficient array dimensions axis 1 (dimensions for coeff)
c  ndc2a,ndc2b : coefficient array dimensions axis 2
c  ndc3a,ndc3b : coefficient array dimensions axis 3
c
c  nds1a,nds1b,nds2a,nds2b,nds3a,nds3b : dimensions for s
c
c  n1a,n1b : subset for evaluating operator, axis 1
c  n2a,n2b : subset for evaluating operator, axis 2
c  n3a,n3b : subset for evaluating operator, axis 3
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
c  derivType : 0=non-conservative, 1=conservative
c  gridType: 0=rectangular, 1=non-rectangular
c  order : 2 or 4
c  averagingType : arithmeticAverage=0, harmonicAverage=1, for conservative approximations
c  dir1,dir2 : for derivative=derivativeScalarDerivative
c
c  ndw : size of the work space w
c  w   : work space required for some operators. 
c  ierr : error return
c         ierr=2 : not enough space in ndw
c======================================================================
c      implicit none
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


c The next call assumes that only 3 work space arrays are needed, a11,a22,a33

      if( derivative.eq.laplacianOperator )then
        derivOption=laplace
      else if( derivative.eq.divergenceScalarGradient )then
        derivOption=divScalarGrad
      else if( derivative.ge.xDerivativeScalarXDerivative .and. 
     & derivative.le.zDerivativeScalarZDerivative )then
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
      if( derivative.eq.laplacianOperator .and. 
     & derivType.eq.nonConservative )then
! callOperator(laplacian)
        if( order.eq.2 )then
          call laplacianCoeff2ndOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,
     & nd3b, ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,
     & nds2b,nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,
     & ca,cb, dx,dr, rsxy,coeff, derivOption, derivType, gridType, 
     & order, s, jac, averagingType, dir1, dir2,w(0),w(nda),w(2*nda),
     & w(3*nda),w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda) )
        else
          call laplacianCoeff4thOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,
     & nd3b, ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,
     & nds2b,nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,
     & ca,cb, dx,dr, rsxy,coeff, derivOption, derivType, gridType, 
     & order, s, jac, averagingType, dir1, dir2,w(0),w(nda),w(2*nda),
     & w(3*nda),w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda) )
        end if
      else if( derivative.eq.xDerivative )then
! callOperator(x)
        if( order.eq.2 )then
          call xCoeff2ndOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,nds2b,
     & nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,ca,cb,
     &  dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s,
     &  jac, averagingType, dir1, dir2,w(0),w(nda),w(2*nda),w(3*nda),
     & w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda) )
        else
          call xCoeff4thOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,nds2b,
     & nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,ca,cb,
     &  dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s,
     &  jac, averagingType, dir1, dir2,w(0),w(nda),w(2*nda),w(3*nda),
     & w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda) )
        end if
      else if( derivative.eq.yDerivative )then
! callOperator(y)
        if( order.eq.2 )then
          call yCoeff2ndOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,nds2b,
     & nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,ca,cb,
     &  dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s,
     &  jac, averagingType, dir1, dir2,w(0),w(nda),w(2*nda),w(3*nda),
     & w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda) )
        else
          call yCoeff4thOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,nds2b,
     & nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,ca,cb,
     &  dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s,
     &  jac, averagingType, dir1, dir2,w(0),w(nda),w(2*nda),w(3*nda),
     & w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda) )
        end if
      else if( derivative.eq.zDerivative )then
! callOperator(z)
        if( order.eq.2 )then
          call zCoeff2ndOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,nds2b,
     & nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,ca,cb,
     &  dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s,
     &  jac, averagingType, dir1, dir2,w(0),w(nda),w(2*nda),w(3*nda),
     & w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda) )
        else
          call zCoeff4thOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,nds2b,
     & nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,ca,cb,
     &  dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s,
     &  jac, averagingType, dir1, dir2,w(0),w(nda),w(2*nda),w(3*nda),
     & w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda) )
        end if
      else if( derivative.eq.xxDerivative )then
! callOperator(xx)
        if( order.eq.2 )then
          call xxCoeff2ndOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,nds2b,
     & nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,ca,cb,
     &  dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s,
     &  jac, averagingType, dir1, dir2,w(0),w(nda),w(2*nda),w(3*nda),
     & w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda) )
        else
          call xxCoeff4thOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,nds2b,
     & nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,ca,cb,
     &  dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s,
     &  jac, averagingType, dir1, dir2,w(0),w(nda),w(2*nda),w(3*nda),
     & w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda) )
        end if
      else if( derivative.eq.xyDerivative )then
! callOperator(xy)
        if( order.eq.2 )then
          call xyCoeff2ndOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,nds2b,
     & nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,ca,cb,
     &  dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s,
     &  jac, averagingType, dir1, dir2,w(0),w(nda),w(2*nda),w(3*nda),
     & w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda) )
        else
          call xyCoeff4thOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,nds2b,
     & nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,ca,cb,
     &  dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s,
     &  jac, averagingType, dir1, dir2,w(0),w(nda),w(2*nda),w(3*nda),
     & w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda) )
        end if
      else if( derivative.eq.xzDerivative )then
! callOperator(xz)
        if( order.eq.2 )then
          call xzCoeff2ndOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,nds2b,
     & nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,ca,cb,
     &  dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s,
     &  jac, averagingType, dir1, dir2,w(0),w(nda),w(2*nda),w(3*nda),
     & w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda) )
        else
          call xzCoeff4thOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,nds2b,
     & nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,ca,cb,
     &  dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s,
     &  jac, averagingType, dir1, dir2,w(0),w(nda),w(2*nda),w(3*nda),
     & w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda) )
        end if
      else if( derivative.eq.yyDerivative )then
! callOperator(yy)
        if( order.eq.2 )then
          call yyCoeff2ndOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,nds2b,
     & nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,ca,cb,
     &  dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s,
     &  jac, averagingType, dir1, dir2,w(0),w(nda),w(2*nda),w(3*nda),
     & w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda) )
        else
          call yyCoeff4thOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,nds2b,
     & nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,ca,cb,
     &  dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s,
     &  jac, averagingType, dir1, dir2,w(0),w(nda),w(2*nda),w(3*nda),
     & w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda) )
        end if
      else if( derivative.eq.yzDerivative )then
! callOperator(yz)
        if( order.eq.2 )then
          call yzCoeff2ndOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,nds2b,
     & nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,ca,cb,
     &  dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s,
     &  jac, averagingType, dir1, dir2,w(0),w(nda),w(2*nda),w(3*nda),
     & w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda) )
        else
          call yzCoeff4thOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,nds2b,
     & nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,ca,cb,
     &  dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s,
     &  jac, averagingType, dir1, dir2,w(0),w(nda),w(2*nda),w(3*nda),
     & w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda) )
        end if
      else if( derivative.eq.zzDerivative )then
! callOperator(zz)
        if( order.eq.2 )then
          call zzCoeff2ndOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,nds2b,
     & nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,ca,cb,
     &  dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s,
     &  jac, averagingType, dir1, dir2,w(0),w(nda),w(2*nda),w(3*nda),
     & w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda) )
        else
          call zzCoeff4thOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,nds2b,
     & nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,ca,cb,
     &  dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s,
     &  jac, averagingType, dir1, dir2,w(0),w(nda),w(2*nda),w(3*nda),
     & w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda) )
        end if
      else if( derivative.eq.identityOperator )then
! callOperator(identity)
        if( order.eq.2 )then
          call identityCoeff2ndOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,
     & nd3b, ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,
     & nds2b,nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,
     & ca,cb, dx,dr, rsxy,coeff, derivOption, derivType, gridType, 
     & order, s, jac, averagingType, dir1, dir2,w(0),w(nda),w(2*nda),
     & w(3*nda),w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda) )
        else
          call identityCoeff4thOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,
     & nd3b, ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,
     & nds2b,nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,
     & ca,cb, dx,dr, rsxy,coeff, derivOption, derivType, gridType, 
     & order, s, jac, averagingType, dir1, dir2,w(0),w(nda),w(2*nda),
     & w(3*nda),w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda) )
        end if
      else if( derivative.eq.divergenceScalarGradient .or. 
     & derivative.eq.laplacianOperator .or.
     & (derivative.ge.xDerivativeScalarXDerivative .and. 
     & derivative.le.zDerivativeScalarZDerivative)) then
        ! this next call can do divScalarGrad, derivativeScalarDerivative and conservative laplacian

        ! check work space size
        if( derivative.eq.laplacianOperator .or. 
     & derivative.eq.divergenceScalarGradient )then
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
        if( (derivative.eq.laplacianOperator .or. 
     & derivative.eq.divergenceScalarGradient)
     &      .and. gridType.eq.0 )then
          ! rectangular grid requires work arrays a11,a22,a33
! callOperatorRectangular(divScalarGrad)
          if( order.eq.2 )then
            call divScalarGradCoeff2ndOrder( nd, nd1a,nd1b,nd2a,nd2b,
     & nd3a,nd3b, ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,
     & nds2a,nds2b,nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, 
     & ea,eb,ca,cb, dx,dr, rsxy,coeff, derivOption, derivType, 
     & gridType, order, s, jac, averagingType, dir1, dir2,w(0),w(nda),
     & w(2*nda),w(2*nda),w(2*nda),w(2*nda), w(2*nda),w(2*nda),w(2*nda)
     &  )
          else
            call divScalarGradCoeff4thOrder( nd, nd1a,nd1b,nd2a,nd2b,
     & nd3a,nd3b, ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,
     & nds2a,nds2b,nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, 
     & ea,eb,ca,cb, dx,dr, rsxy,coeff, derivOption, derivType, 
     & gridType, order, s, jac, averagingType, dir1, dir2,w(0),w(nda),
     & w(2*nda),w(2*nda),w(2*nda),w(2*nda), w(2*nda),w(2*nda),w(2*nda)
     &  )
          end if
        else
! callOperator(divScalarGrad)
          if( order.eq.2 )then
            call divScalarGradCoeff2ndOrder( nd, nd1a,nd1b,nd2a,nd2b,
     & nd3a,nd3b, ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,
     & nds2a,nds2b,nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, 
     & ea,eb,ca,cb, dx,dr, rsxy,coeff, derivOption, derivType, 
     & gridType, order, s, jac, averagingType, dir1, dir2,w(0),w(nda),
     & w(2*nda),w(3*nda),w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda)
     &  )
          else
            call divScalarGradCoeff4thOrder( nd, nd1a,nd1b,nd2a,nd2b,
     & nd3a,nd3b, ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,
     & nds2a,nds2b,nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, 
     & ea,eb,ca,cb, dx,dr, rsxy,coeff, derivOption, derivType, 
     & gridType, order, s, jac, averagingType, dir1, dir2,w(0),w(nda),
     & w(2*nda),w(3*nda),w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda)
     &  )
          end if
        endif
      else if( derivative.eq.r1Derivative )then
! callOperator(r)
        if( order.eq.2 )then
          call rCoeff2ndOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,nds2b,
     & nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,ca,cb,
     &  dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s,
     &  jac, averagingType, dir1, dir2,w(0),w(nda),w(2*nda),w(3*nda),
     & w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda) )
        else
          call rCoeff4thOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,nds2b,
     & nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,ca,cb,
     &  dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s,
     &  jac, averagingType, dir1, dir2,w(0),w(nda),w(2*nda),w(3*nda),
     & w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda) )
        end if
      else if( derivative.eq.r2Derivative )then
! callOperator(s)
        if( order.eq.2 )then
          call sCoeff2ndOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,nds2b,
     & nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,ca,cb,
     &  dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s,
     &  jac, averagingType, dir1, dir2,w(0),w(nda),w(2*nda),w(3*nda),
     & w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda) )
        else
          call sCoeff4thOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,nds2b,
     & nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,ca,cb,
     &  dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s,
     &  jac, averagingType, dir1, dir2,w(0),w(nda),w(2*nda),w(3*nda),
     & w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda) )
        end if
      else if( derivative.eq.r3Derivative )then
! callOperator(t)
        if( order.eq.2 )then
          call tCoeff2ndOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,nds2b,
     & nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,ca,cb,
     &  dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s,
     &  jac, averagingType, dir1, dir2,w(0),w(nda),w(2*nda),w(3*nda),
     & w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda) )
        else
          call tCoeff4thOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,nds2b,
     & nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,ca,cb,
     &  dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s,
     &  jac, averagingType, dir1, dir2,w(0),w(nda),w(2*nda),w(3*nda),
     & w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda) )
        end if
      else if( derivative.eq.r1r1Derivative )then
! callOperator(rr)
        if( order.eq.2 )then
          call rrCoeff2ndOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,nds2b,
     & nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,ca,cb,
     &  dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s,
     &  jac, averagingType, dir1, dir2,w(0),w(nda),w(2*nda),w(3*nda),
     & w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda) )
        else
          call rrCoeff4thOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,nds2b,
     & nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,ca,cb,
     &  dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s,
     &  jac, averagingType, dir1, dir2,w(0),w(nda),w(2*nda),w(3*nda),
     & w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda) )
        end if
      else if( derivative.eq.r1r2Derivative )then
! callOperator(rs)
        if( order.eq.2 )then
          call rsCoeff2ndOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,nds2b,
     & nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,ca,cb,
     &  dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s,
     &  jac, averagingType, dir1, dir2,w(0),w(nda),w(2*nda),w(3*nda),
     & w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda) )
        else
          call rsCoeff4thOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,nds2b,
     & nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,ca,cb,
     &  dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s,
     &  jac, averagingType, dir1, dir2,w(0),w(nda),w(2*nda),w(3*nda),
     & w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda) )
        end if
      else if( derivative.eq.r1r3Derivative )then
! callOperator(rt)
        if( order.eq.2 )then
          call rtCoeff2ndOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,nds2b,
     & nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,ca,cb,
     &  dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s,
     &  jac, averagingType, dir1, dir2,w(0),w(nda),w(2*nda),w(3*nda),
     & w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda) )
        else
          call rtCoeff4thOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,nds2b,
     & nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,ca,cb,
     &  dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s,
     &  jac, averagingType, dir1, dir2,w(0),w(nda),w(2*nda),w(3*nda),
     & w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda) )
        end if
      else if( derivative.eq.r2r2Derivative )then
! callOperator(ss)
        if( order.eq.2 )then
          call ssCoeff2ndOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,nds2b,
     & nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,ca,cb,
     &  dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s,
     &  jac, averagingType, dir1, dir2,w(0),w(nda),w(2*nda),w(3*nda),
     & w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda) )
        else
          call ssCoeff4thOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,nds2b,
     & nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,ca,cb,
     &  dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s,
     &  jac, averagingType, dir1, dir2,w(0),w(nda),w(2*nda),w(3*nda),
     & w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda) )
        end if
      else if( derivative.eq.r2r3Derivative )then
! callOperator(st)
        if( order.eq.2 )then
          call stCoeff2ndOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,nds2b,
     & nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,ca,cb,
     &  dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s,
     &  jac, averagingType, dir1, dir2,w(0),w(nda),w(2*nda),w(3*nda),
     & w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda) )
        else
          call stCoeff4thOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,nds2b,
     & nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,ca,cb,
     &  dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s,
     &  jac, averagingType, dir1, dir2,w(0),w(nda),w(2*nda),w(3*nda),
     & w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda) )
        end if
      else if( derivative.eq.r3r3Derivative )then
! callOperator(tt)
        if( order.eq.2 )then
          call ttCoeff2ndOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,nds2b,
     & nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,ca,cb,
     &  dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s,
     &  jac, averagingType, dir1, dir2,w(0),w(nda),w(2*nda),w(3*nda),
     & w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda) )
        else
          call ttCoeff4thOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,nds2b,
     & nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb,ca,cb,
     &  dx,dr, rsxy,coeff, derivOption, derivType, gridType, order, s,
     &  jac, averagingType, dir1, dir2,w(0),w(nda),w(2*nda),w(3*nda),
     & w(4*nda),w(5*nda), w(6*nda),w(7*nda),w(8*nda) )
        end if
      else
        ierr=1
        write(*,*) 'coeffOperator:ERROR: unimplemented derivative=',
     &     derivative
      end if

      return
      end












! This macro will switch the roles of x and x in the arguments

! This macro will switch the roles of y and y in the arguments

! This macro will switch the roles of x and y in the arguments
! This macro will switch the roles of x and y in the arguments

! This macro will switch the roles of x and x in the arguments

! This macro will switch the roles of y and y in the arguments

! This macro will switch the roles of z and z in the arguments

! This macro will switch the roles of x and y in the arguments

! This macro will switch the roles of y and x in the arguments

! This macro will switch the roles of x and z in the arguments
! This macro will switch the roles of z and x in the arguments

! This macro will switch the roles of y and z in the arguments
! This macro will switch the roles of z and y in the arguments


c define a macro for x, y in 2d rectangular

c define a macro for xx, yy in 2d rectangular
c define a macro for x, y in 3d rectangular

c define a macro for xx, yy in 3d rectangular

c define a macro for xy, xz, yz in 3d rectangular
c  for derivative xy use (x,x,1,2)
c  for derivative xz use (y,z,1,3)
c  for derivative yz use (x,z,2,3)


c define a macro for r, s in 2d rectangular

c define a macro for rr, ss in 2d rectangular

c define a macro for r, s, t in 3d rectangular

c define a macro for rr, ss, tt in 3d rectangular

c define a macro for xy, xz, yz in 3d rectangular
c  for derivative rs use (x,x,1,2)
c  for derivative rt use (y,z,1,3)
c  for derivative st use (x,z,2,3)


c define a macro for x, y in 2d

c define a macro for xx, yy in 2d


c define a macro for x, y, z in 3d

c define a macro for xx, yy, zz in 3d

c define a macro for xy, xz, yz in 3d




! buildFile(identity)
! buildFile(laplacian)
! buildFile(x)
! buildFile(y)
! buildFile(z)
! buildFile(xx)
! buildFile(xy)
! buildFile(xz)
! buildFile(yy)
! buildFile(yz)
! buildFile(zz)
! buildFile(divScalarGrad)


! buildFile(r)
! buildFile(s)
! buildFile(t)
! buildFile(rr)
! buildFile(rs)
! buildFile(rt)
! buildFile(ss)
! buildFile(st)
! buildFile(tt)

c ****************************************************************************************
c ************************* 4th order ****************************************************
c ****************************************************************************************










! This macro will switch the roles of x and x in the arguments

! This macro will switch the roles of x and y in the arguments



! This macro will switch the roles of x and x in the arguments
! This macro will switch the roles of x and y in the arguments
! This macro will switch the roles of x and z in the arguments
! This macro will switch the roles of y and z in the arguments

c define a macro for x, y in 2d rectangular

c define a macro for xx, yy in 2d rectangular

c define a macro for r, s in 2d rectangular

c define a macro for rr, ss in 2d rectangular
c define a macro for x, y in 2d

c define a macro for xx, yy in 2d





c define a macro for x, y in 3d rectangular

c define a macro for xx, yy in 3d rectangular

c define a macro for xy, xz, yz in 3d rectangular
c  for derivative xy use (x,x,1,2)
c  for derivative xz use (y,z,1,3)
c  for derivative yz use (x,z,2,3)


c define a macro for r, s, t in 3d rectangular

c define a macro for rr, ss, tt in 3d rectangular

c define a macro for rs, rt, st in 3d rectangular
c  for derivative rs use (x,x,1,2)
c  for derivative rt use (y,z,1,3)
c  for derivative st use (x,z,2,3)



c define a macro for x, y, z in 3d


c define a macro for xx, yy, zz in 3d


c        loopBody4thOrder3d(c        0,0,sx*ty+sy*tx,0,0,0,0,-8.*sx*ty-8.*sy*tx,0,0,rx*ty+ry*tx,-8.*rx*ty-8.*ry*tx,txy43-1.*tx*ty, c        8.*rx*ty+8.*ry*tx,-1.*rx*ty-1.*ry*tx,0,0,8.*sx*ty+8.*sy*tx,0,0,0,0,-1.*sx*ty-1.*sy*tx,0,0,0,0, c        -8.*sx*ty-8.*sy*tx,0,0,0,0,64.*sx*ty+64.*sy*tx,0,0,-8.*rx*ty-8.*ry*tx,64.*rx*ty+64.*ry*tx, c        -8.*txy43+16.*tx*ty,-64.*rx*ty-64.*ry*tx,8.*rx*ty+8.*ry*tx,0,0,-64.*sx*ty-64.*sy*tx,0,0,0,0, c        8.*sx*ty+8.*sy*tx,0,0,rx*sy+ry*sx,-8.*rx*sy-8.*ry*sx,sxy43-1.*sx*sy,8.*rx*sy+8.*ry*sx, c        -1.*rx*sy-1.*ry*sx,-8.*rx*sy-8.*ry*sx,64.*rx*sy+64.*ry*sx,-8.*sxy43+16.*sx*sy, c        -64.*rx*sy-64.*ry*sx,8.*rx*sy+8.*ry*sx,-1.*rx*ry+rxy43,16.*rx*ry-8.*rxy43, c        -30.*rx*ry-30.*sx*sy-30.*tx*ty,16.*rx*ry+8.*rxy43,-1.*rx*ry-1.*rxy43,8.*rx*sy+8.*ry*sx, c        -64.*rx*sy-64.*ry*sx,8.*sxy43+16.*sx*sy,64.*rx*sy+64.*ry*sx,-8.*rx*sy-8.*ry*sx, c        -1.*rx*sy-1.*ry*sx,8.*rx*sy+8.*ry*sx,-1.*sxy43-1.*sx*sy,-8.*rx*sy-8.*ry*sx,rx*sy+ry*sx,0,0, c        8.*sx*ty+8.*sy*tx,0,0,0,0,-64.*sx*ty-64.*sy*tx,0,0,8.*rx*ty+8.*ry*tx,-64.*rx*ty-64.*ry*tx, c        8.*txy43+16.*tx*ty,64.*rx*ty+64.*ry*tx,-8.*rx*ty-8.*ry*tx,0,0,64.*sx*ty+64.*sy*tx,0,0,0,0, c        -8.*sx*ty-8.*sy*tx,0,0,0,0,-1.*sx*ty-1.*sy*tx,0,0,0,0,8.*sx*ty+8.*sy*tx,0,0,-1.*rx*ty-1.*ry*tx, c        8.*rx*ty+8.*ry*tx,-1.*txy43-1.*tx*ty,-8.*rx*ty-8.*ry*tx,rx*ty+ry*tx,0,0,-8.*sx*ty-8.*sy*tx,0,0,0,0, c        sx*ty+sy*tx,0,0 )






! buildFile4(identity)
! buildFile4(laplacian)
! buildFile4(x)
! buildFile4(y)
! buildFile4(z)
! buildFile4(xx)
! buildFile4(xy)
! buildFile4(xz)
! buildFile4(yy)
! buildFile4(yz)
! buildFile4(zz)
! buildFile4(divScalarGrad)

! buildFile4(r)
! buildFile4(s)
! buildFile4(t)
! buildFile4(rr)
! buildFile4(rs)
! buildFile4(rt)
! buildFile4(ss)
! buildFile4(st)
! buildFile4(tt)

c done
