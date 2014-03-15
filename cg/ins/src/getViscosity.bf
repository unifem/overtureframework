

c ************** Evaluate the (non-linear) viscosity  ******************

c   bpp -I$Overture/include -I/home/henshaw.0/Overture/op/src -I/home/henshaw.0/Overture/op/doc getViscosity.bf
c   bpp -quiet -clean -I$Overture/include -I/home/henshaw.0/Overture/op/src -I/home/henshaw.0/Overture/op/doc getViscosity.bf


c --- See --- op/fortranCoeff/opcoeff.bf 
c             op/include/defineConservative.h
c --- See --- mx/src/interfaceMacros.bf <-- mixes different orders of accuracy 

#Include "viscoPlasticMacros.h"
#Include "lineSolveVP.h"


c -- define bpp macros for coefficient operators (from op/src/stencilCoeff.maple)
#Include opStencilCoeffOrder2.h
! #Include opStencilCoeffOrder4.h
! #Include opStencilCoeffOrder6.h
! #Include opStencilCoeffOrder8.h


c These next include file will define the macros that will define the difference approximations (in op/src)
c Defines getDuDx2(u,aj,ff), getDuDxx2(u,aj,ff), getDuDx3(u,aj,ff), ...  etc. 
#Include "derivMacroDefinitions.h"

c Define 
c    defineParametricDerivativeMacros(u,dr,dx,DIM,ORDER,COMPONENTS,MAXDERIV)
c       defines -> ur2, us2, ux2, uy2, ...            (2D)
c                  ur3, us3, ut3, ux3, uy3, uz3, ...  (3D)
#Include "defineParametricDerivMacros.h"

! defineParametricDerivativeMacros(u,dr,dx,DIM,ORDER,COMPONENTS,MAXDERIV)
! 2D, order=6, components=1
! defineParametricDerivativeMacros(u,dr,dx,DIM,ORDER,COMPONENTS,MAXDERIV)

! defineParametricDerivativeMacros(u,dr,dx,2,2,1,2)
 defineParametricDerivativeMacros(rsxy,dr,dx,3,2,2,2)
!  defineParametricDerivativeMacros(rsxy,dr,dx,3,4,2,2)
!  defineParametricDerivativeMacros(rsxy,dr,dx,3,6,2,2)

 defineParametricDerivativeMacros(u,dr,dx,3,2,1,2)
!  defineParametricDerivativeMacros(ul,dr,dx,3,2,1,2)


! Example to define orders 2,4,6: 
! defineParametricDerivativeMacros(u1,dr1,dx1,2,2,1,6)
! defineParametricDerivativeMacros(u1,dr1,dx1,2,4,1,4)
! defineParametricDerivativeMacros(u1,dr1,dx1,2,6,1,2)

! construct an include file that declares temporary variables:
#beginFile src/getViscosityDeclareTemporaryVariablesOrder2.h
      !  declareTemporaryVariables(DIM,MAXDERIV)   ! I think DIM and MAXDERIV are ignored for now
      declareTemporaryVariables(2,2)
      declareParametricDerivativeVariables(uu,3)   ! declare temp variables uu, uur, uus, ...
      declareParametricDerivativeVariables(vv,3) 
      declareParametricDerivativeVariables(ww,3) 
!      declareParametricDerivativeVariables(pp,3) 
!       declareParametricDerivativeVariables(uul,3)   ! declare temp variables uu, uur, uus, ...
!       declareParametricDerivativeVariables(vvl,3) 
!       declareParametricDerivativeVariables(wwl,3) 
      declareJacobianDerivativeVariables(aj,3)     ! declareJacobianDerivativeVariables(aj,DIM)
#endFile


! define macros for conservative operators: (in op/src)
!  defines getConservativeCoeff( OPERATOR,s,coeff ), OPERATOR=divScalarGrad, ...
! #Include "conservativeCoefficientMacros.h"

c From opcoeff.bf

#beginMacro beginLoops()
 do i3=n3a,n3b
 do i2=n2a,n2b
 do i1=n1a,n1b
  if( mask(i1,i2,i3).ne.0 )then
#endMacro

#beginMacro endLoops()
  end if
 end do
 end do
 end do
#endMacro


! ==========================================================================================
!  Evaluate the Jacobian and it's derivatives (parametric and spatial). 
!    aj     : prefix for the name of the resulting jacobian variables, 
!             e.g. ajrx, ajsy, ajrxx, ajsxy, ...
!    MAXDER : number of derivatives to evaluate.  
! ==========================================================================================
#beginMacro opEvalJacobianDerivatives(aj,MAXDER)

#If $GRIDTYPE eq "curvilinear"
 ! this next call will define the jacobian and its derivatives (parameteric and spatial)
 #peval evalJacobianDerivatives(rsxy,i1,i2,i3,aj,$DIM,$ORDER,MAXDER)

#End

#endMacro 

! ==========================================================================================
!  Evaluate the parametric derivatives of u.
!    u      : evaluate derivatives of this function.
!    uc     : component to evaluate
!    uu     : prefix for the name of the resulting derivatives, e.g. uur, uus, uurr, ...
!    MAXDER : number of derivatives to evaluate.  
! ==========================================================================================
#beginMacro opEvalParametricDerivative(u,uc,uu,MAXDER)
#If $GRIDTYPE eq "curvilinear" 
 #peval evalParametricDerivativesComponents1(u,i1,i2,i3,uc, uu,$DIM,$ORDER,MAXDER)
#Else
 uu=u(i1,i2,i3,uc) ! in the rectangular case just eval the solution
#End
#endMacro


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
#beginMacro getOp(DERIV, u,uc,uu,aj,ud )

 #If $GRIDTYPE eq "curvilinear" 
  #peval getDuD ## DERIV ## $DIM(uu,aj,ud)  ! Note: The perl variables are evaluated when the macro is USED. 
 #Else
  #peval ud = u ## DERIV ## $ORDER(i1,i2,i3,uc)
 #End

#endMacro



c =====================================================================================
c Here is the coefficient of viscosity for the visco plastic model 
c
c   nuT = ( etaVP + (yieldStressVP/esr)*(1.-exp(-exponentVP*esr)) )
c where 
c   esr = effective strain rate = || (2/3)*eDot_ij ||
c
c Macro arguments: 
c   DIM : 2,3 
c   GRIDTYPE: rectangular, curvilinear
c =====================================================================================
#beginMacro getViscosityCoefficients()

! **** to do : optimize this for backward-Euler : fe=0, fi=0 !!
if( pdeModel.eq.viscoPlasticModel )then
 beginLoops()
  ! Evaluate the jacobian derivatives used by the coefficient and forward derivatives:
  ! opEvalJacobianDerivatives(MAXDER) : MAXDER = max number of derivatives to precompute.
  opEvalJacobianDerivatives(aj,0)

  ! evaluate forward derivatives of the current solution: 

  ! First evaluate the parametric derivatives of u, used later by the spatial derivatives. 
  ! MAXDER = max number of parametric derivatives to precompute.
  opEvalParametricDerivative(u,uc,uu,1)
  ! Evaluate the spatial derivatives of u:
  getOp(x, u,uc,uu,aj,u0x)       ! u.x
  getOp(y, u,uc,uu,aj,u0y)       ! u.y

  ! Evaluate the spatial derivatives of v:
  opEvalParametricDerivative(u,vc,vv,1)
  getOp(x, u,vc,vv,aj,v0x)       ! v.x
  getOp(y, u,vc,vv,aj,v0y)       ! v.y 

  esr = strainRate2d(u0x,u0y,v0x,v0y)

 #If $DIM == 3

  getOp(z, u,uc,uu,aj,u0z)       ! u.z
  getOp(z, u,vc,vv,aj,v0z)       ! v.z

  opEvalParametricDerivative(u,wc,ww,1)
  getOp(x, u,wc,ww,aj,w0x)       ! w.x
  getOp(y, u,wc,ww,aj,w0y)       ! w.y
  getOp(z, u,wc,ww,aj,w0z)       ! w.z

  ! finish me for 3d
  stop 3916

 #End


 ! this next macro is in viscoPlasticMacros.h 
 getViscoPlasticViscosity(visc(i1,i2,i3,0),esr)

 endLoops()
else
 write(*,'("getViscosity:ERROR: unknown pdeModel=",i6)') pdeModel
end if

#endMacro


      subroutine getViscosity(nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                       mask,xy,rsxy,  u, visc, gv,dw,  bc, ipar, rpar, ierr )
c======================================================================
c 
c    Compute the coefficient of the (nonlinear) viscosity 
c       for the Incompressible Navier Stokes Equations
c    -----------------------------------------------------
c
c
c nd : number of space dimensions
c nd1a,nd1b,nd2a,nd2b,nd3a,nd3b : array dimensions
c
c mask : 
c xy : 
c rsxy : 
c u : holds the current solution, used to evaluate the viscosity coefficient.
c visc : output, the coefficient of the (nonlinear) viscosity
c gv : gridVelocity for moving grids
c dw : distance to the wall for some turbulence models
c 
c======================================================================
      implicit none
      integer nd, ndc, n1a,n1b,n2a,n2b,n3a,n3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b
      integer nde,nr1a,nr1b,nr2a,nr2b,nr3a,nr3b

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)

      real visc(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)

      real gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      real dw(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer bc(0:1,0:2),indexRange(0:1,0:2),ierr

      integer ipar(0:*)
      real rpar(0:*)
      
c     ---- local variables -----
      integer c,e,i1,i2,i3,m1,m2,m3,j1,j2,j3,ghostLine,n,i1m,i2m,i3m,i1p,i2p,i3p
      integer side,axis,is1,is2,is3,mm,eqnTemp,debug
      integer kd,kd3,orderOfAccuracy,gridIsMoving,orderOfExtrap,orderOfExtrapolation,orderOfExtrapolationForOutflow
      integer numberOfComponentsForCoefficients,stencilSize
      integer gridIsImplicit,implicitOption,implicitMethod,
     & isAxisymmetric,use2ndOrderAD,use4thOrderAD
      integer pc,uc,vc,wc,sc,nc,kc,ec,tc,grid,m,advectPassiveScalar
      real nu,dt,nuPassiveScalar,adcPassiveScalar
      real gravity(0:2), thermalExpansivity, adcBoussinesq,kThermal
      real dxi,dyi,dzi,dri,dsi,dti,dr2i,ds2i,dt2i
      real ad21,ad22,ad41,ad42,cd22,cd42,adc
      real ad21n,ad22n,ad41n,ad42n,cd22n,cd42n
      real yy,yEps

      integer fillCoefficients,evalRightHandSide,evalResidual,evalResidualForBoundaryConditions

      integer equationNumberBase1,equationNumberLength1,equationNumberBase2,equationNumberLength2,\
              equationNumberBase3,equationNumberLength3,equationOffset

      integer gridType
      integer rectangular,curvilinear
      parameter( rectangular=0, curvilinear=1 )

      integer turbulenceModel,noTurbulenceModel
      integer baldwinLomax,spalartAllmaras,kEpsilon,kOmega
      parameter (noTurbulenceModel=0,baldwinLomax=1,kEpsilon=2,kOmega=3,spalartAllmaras=4 )

      integer computeAllTerms,
     &     doNotComputeImplicitTerms,
     &     computeImplicitTermsSeparately,
     &     computeAllWithWeightedImplicit

      parameter( computeAllTerms=0,
     &           doNotComputeImplicitTerms=1,
     &           computeImplicitTermsSeparately=2,
     &           computeAllWithWeightedImplicit=3 )

      real implicitFactor

      integer implicitVariation
      integer implicitViscous, implicitAdvectionAndViscous, implicitFullLinearized
      parameter( implicitViscous=0, 
     &           implicitAdvectionAndViscous=1, 
     &           implicitFullLinearized=2 )

      integer pdeModel,standardModel,BoussinesqModel,viscoPlasticModel
      parameter( standardModel=0,BoussinesqModel=1,viscoPlasticModel=2 )

      real rx,ry,rz,sx,sy,sz,tx,ty,tz
      real aj
      real dr(0:2), dx(0:2)

      integer ncc,halfWidth,halfWidth3

      real u0x,u0y,u0z, v0x,v0y,v0z, w0x,w0y,w0z, p0x,p0y,p0z
      real ulx,uly,ulz, vlx,vly,vlz, wlx,wly,wlz, plx,ply,plz
      real u0xx,u0xy,u0xz,u0yy,u0yz,u0zz
      real v0xx,v0xy,v0xz,v0yy,v0yz,v0zz
      real w0xx,w0xy,w0xz,w0yy,w0yz,w0zz

      ! --- visco plastic variables ---
      declareViscoPlasticVariables()

      ! This include file (created above) declares variables needed by the getDuDx() macros.
      include 'getViscosityDeclareTemporaryVariablesOrder2.h'

c     --- begin statement functions
      rx(i1,i2,i3)=rsxy(i1,i2,i3,0,0)
      ry(i1,i2,i3)=rsxy(i1,i2,i3,0,1)
      rz(i1,i2,i3)=rsxy(i1,i2,i3,0,2)
      sx(i1,i2,i3)=rsxy(i1,i2,i3,1,0)
      sy(i1,i2,i3)=rsxy(i1,i2,i3,1,1)
      sz(i1,i2,i3)=rsxy(i1,i2,i3,1,2)
      tx(i1,i2,i3)=rsxy(i1,i2,i3,2,0)
      ty(i1,i2,i3)=rsxy(i1,i2,i3,2,1)
      tz(i1,i2,i3)=rsxy(i1,i2,i3,2,2)
c     --- end statement functions

      ierr=0

      n1a                =ipar(0)
      n1b                =ipar(1)
      n2a                =ipar(2)
      n2b                =ipar(3)
      n3a                =ipar(4)
      n3b                =ipar(5)

      pc                 =ipar(6)
      uc                 =ipar(7)
      vc                 =ipar(8)
      wc                 =ipar(9)
      nc                 =ipar(10)
      sc                 =ipar(11)
      tc                 =ipar(12)
      grid               =ipar(13)
      orderOfAccuracy    =ipar(14)
      gridIsMoving       =ipar(15) ! *************

      implicitVariation  =ipar(16) ! **new**

      fillCoefficients   =ipar(17) ! new 
      evalRightHandSide  =ipar(18) ! new

      gridIsImplicit     =ipar(19)
      implicitMethod     =ipar(20)
      implicitOption     =ipar(21)
      isAxisymmetric     =ipar(22)
      use2ndOrderAD      =ipar(23)
      use4thOrderAD      =ipar(24)
      advectPassiveScalar=ipar(25)
      gridType           =ipar(26)
      turbulenceModel    =ipar(27)
      pdeModel           =ipar(28)  
      numberOfComponentsForCoefficients =ipar(29) ! number of components for coefficients
      stencilSize        =ipar(30)

      equationOffset        = ipar(31)
      equationNumberBase1   = ipar(32)
      equationNumberLength1 = ipar(33)
      equationNumberBase2   = ipar(34)
      equationNumberLength2 = ipar(35)
      equationNumberBase3   = ipar(36)
      equationNumberLength3 = ipar(37)

      indexRange(0,0)    =ipar(38)
      indexRange(1,0)    =ipar(39)
      indexRange(0,1)    =ipar(40)
      indexRange(1,1)    =ipar(41)
      indexRange(0,2)    =ipar(42)
      indexRange(1,2)    =ipar(43)

      orderOfExtrapolation=ipar(44)
      orderOfExtrapolationForOutflow=ipar(45)

      evalResidual      = ipar(46)
      evalResidualForBoundaryConditions=ipar(47)
      debug             = ipar(48)

      dr(0)             =rpar(0)
      dr(1)             =rpar(1)
      dr(2)             =rpar(2)
      dx(0)             =rpar(3)
      dx(1)             =rpar(4)
      dx(2)             =rpar(5)

      dt                =rpar(6) ! **new**
      implicitFactor    =rpar(7) ! **new**

      nu                =rpar(8)
      ad21              =rpar(9)
      ad22              =rpar(10)
      ad41              =rpar(11)
      ad42              =rpar(12)
      nuPassiveScalar   =rpar(13)
      adcPassiveScalar  =rpar(14)
      ad21n             =rpar(15)
      ad22n             =rpar(16)
      ad41n             =rpar(17)
      ad42n             =rpar(18)
      yEps              =rpar(19) ! for axisymmetric

      gravity(0)        =rpar(20)
      gravity(1)        =rpar(21)
      gravity(2)        =rpar(22)
      thermalExpansivity=rpar(23)
      adcBoussinesq     =rpar(24) ! coefficient of artificial diffusion for Boussinesq T equation 
      kThermal          =rpar(25)

      nuViscoPlastic         =rpar(26)
      etaViscoPlastic        =rpar(27)
      yieldStressViscoPlastic=rpar(28)
      exponentViscoPlastic   =rpar(29)
      epsViscoPlastic        =rpar(30)   ! small parameter used to offset the effective strain rate 

      ! here are the names used by the getViscoPlasticViscosity macro -- what should we do about this ? 
      etaVP=etaViscoPlastic
      yieldStressVP=yieldStressViscoPlastic
      exponentVP=exponentViscoPlastic
      epsVP=epsViscoPlastic

      ncc=numberOfComponentsForCoefficients ! number of components for coefficients

      kc=nc
      ec=kc+1

      if( orderOfAccuracy.ne.2 .and. orderOfAccuracy.ne.4 )then
        write(*,'("insdt:ERROR orderOfAccuracy=",i6)') orderOfAccuracy
        stop 1
      end if
      if( gridType.ne.rectangular .and. gridType.ne.curvilinear )then
        write(*,'("insdt:ERROR gridType=",i6)') gridType
        stop 2
      end if
      if( uc.lt.0 .or. vc.lt.0 .or. (nd.eq.3 .and. wc.lt.0) )then
        write(*,'("insdt:ERROR uc,vc,ws=",3i6)') uc,vc,wc
        stop 4
      end if

c      write(*,'("insdt: turbulenceModel=",2i6)') turbulenceModel
c      write(*,'("insdt: nd,uc,vc,wc,kc=",2i6)') nd,uc,vc,wc,kc

      if( turbulenceModel.eq.kEpsilon .and. (kc.lt.uc+nd .or. kc.gt.1000) )then
        write(*,'("insdt:ERROR in kc: nd,uc,vc,wc,kc=",2i6)') nd,uc,vc,wc,kc
        stop 5
      end if

      ! ==== Assign all possible points ====
      n1a=nd1a+1
      n1b=nd1b-1
      n2a=nd2a+1
      n2b=nd2b-1
      if( nd.gt.2 )then
        n3a=nd3a+1
        n3b=nd3b-1
      else
        n3a=nd3a
        n3b=nd3b
      end if
      if( .true. .or. debug.gt.1 )then
        write(*,'("****** getViscosity etaVP,yieldStressVP=",2e10.2)') etaVP,yieldStressVP
      end if

! Define operator parameters
!   $DIM : number of spatial dimensions
!   $ORDER : order of accuracy of an approximation
!   $GRIDTYPE : rectangular or curvilinear
!   $MATRIX_STENCIL_WIDTH : space in the global coeff matrix was allocated to hold this size stencil
!   $STENCIL_WIDTH : stencil width of the local coeff-matrix (such as xCoeff, yCoeff, lapCoeff, ...)
#perl  $ORDER=2;  $MATRIX_STENCIL_WIDTH=3; $STENCIL_WIDTH=3; 

      if( nd.eq.2 .and. gridType.eq.curvilinear )then
#perl $DIM=2; $GRIDTYPE="curvilinear";

       getViscosityCoefficients()

      else if(  nd.eq.2 .and. gridType.eq.rectangular )then
#perl $DIM=2; $GRIDTYPE="rectangular";

       getViscosityCoefficients()

      else if( nd.eq.3 .and. gridType.eq.curvilinear )then
#perl $DIM=3; $GRIDTYPE="curvilinear";

       getViscosityCoefficients()

      else if(  nd.eq.3 .and. gridType.eq.rectangular )then
#perl $GRIDTYPE="rectangular";

       getViscosityCoefficients()

      else
        stop 1709
      end if

      return
      end


