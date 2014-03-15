! This file automatically generated from insdts.bf with bpp.
!
! Compute the time step dt for the incompressible NS on rectangular AND curvilinear grids
!

! These next include files will define the macros that will define the difference approximations
! The actual macro is called below
c Use this next macro to declare the statement functions that are defined below
c To include derivatives of rx use OPTION=RX


c Define statement functions for difference approximations of order 2 
c To include derivatives of rx use OPTION=RX
c To include derivatives of rx use OPTION=RX



c Use this next macro to declare the statement functions that are defined below
c To include derivatives of rx use OPTION=RX


c Define statement functions for difference approximations of order 4 
c To include derivatives of rx use OPTION=RX
c To include derivatives of rx use OPTION=RX





! Define macros for the derivatives based on the dimension, order of accuracy and grid-type


! =============================================================
! Compute derivatives of u,v,w 
! =============================================================

! =============================================================
! Compute Spalart-Allmaras quantities
!   This macro assumes u0x,u0y, ... are defined
! =============================================================



! ============== from inspf.bf ***
! Return nuT and it's first derivatives for SPAL

! ============== from inspf.bf ***
! Return nuT and it's first derivatives for BL

! ============== from inspf.bf ***
! Return nuT and it's first derivatives for KE

! ============== from inspf.bf ***
! Return the visco-plastic viscosity and it's first derivatives for BL


!====================================================================================
!
! SOLVER: INS, INSSPAL, INSBL, INSKE
! METHOD: GLOBAL, LOCAL  (GLOBAL=fixed time step, LOCAL=local-time stepping -> compute dtVar)
! OPTION: EXPLICIT, IMPLICIT
! ADTYPE: AD2, AD4, AD24 --- NOT USED NOW
! ORDER: 2,4
! DIM: 2,3
! GRIDTYPE: rectangular, curvilinear
! AXISYMMETRIC: notAxisymmetric, or axisymmetric
!====================================================================================

! ====================================================================================
! ====================================================================================







! This is not finished yet..


! ============================================================================================================
!  Define the subroutine that compute the eigenvalues for a given solver
! ============================================================================================================


      subroutine insdts(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,
     & nd3a,nd3b,nd4a,nd4b,mask,xy,rsxy,  u,uu, gv,dw, divDamping, 
     & dtVar, ndMatProp,matIndex,matValpc,matVal, bc, ipar, rpar, pdb,
     &  ierr )
!======================================================================
!
!    Determine the time step for the INS equations.
!    ---------------------------------------------
!
! nd : number of space dimensions
!
! gv : gridVelocity for moving grids
! uu : for moving grids uu is a workspace to hold u-gv, otherwise uu==u
! dw : distance to the wall for some turbulence models
!======================================================================
      implicit none
      integer nd, n1a,n1b,n2a,n2b,n3a,n3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real uu(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real dw(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real divDamping(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real dtVar(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer bc(0:1,0:2),ierr

      integer ipar(0:*)
      real rpar(0:*)

      double precision pdb  ! pointer to data base

      ! -- arrays for variable material properties --
      integer materialFormat,ndMatProp
      integer matIndex(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real matValpc(0:ndMatProp-1,0:*)
      real matVal(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)

      integer turbulenceModel,noTurbulenceModel
      integer baldwinLomax,spalartAllmaras,kEpsilon,kOmega,
     & largeEddySimulation
      parameter (noTurbulenceModel=0,baldwinLomax=1,kEpsilon=2,
     & kOmega=3,spalartAllmaras=4,largeEddySimulation=5 )

      integer pdeModel,standardModel,BoussinesqModel,viscoPlasticModel,
     & twoPhaseFlowModel
      parameter( standardModel=0,BoussinesqModel=1,viscoPlasticModel=2,
     & twoPhaseFlowModel=3 )

!     --- end statement functions

      ierr=0
      ! write(*,'("Inside insdts: gridType=",i2)') gridType

!$$$      pc                 =ipar(0)
!$$$      uc                 =ipar(1)
!$$$      vc                 =ipar(2)
!$$$      wc                 =ipar(3)
!$$$      nc                 =ipar(4)
!$$$      sc                 =ipar(5)
!$$$      grid               =ipar(6)
!$$$      orderOfAccuracy    =ipar(7)
!$$$      gridIsMoving       =ipar(8)
!$$$      useWhereMask       =ipar(9)
!$$$      gridIsImplicit     =ipar(10)
!$$$      implicitMethod     =ipar(11)
!$$$      implicitOption     =ipar(12)
!$$$      isAxisymmetric     =ipar(13)
!$$$      use2ndOrderAD      =ipar(14)
!$$$      use4thOrderAD      =ipar(15)
!$$$      advectPassiveScalar=ipar(16)
!$$$      gridType           =ipar(17)
      turbulenceModel    =ipar(18)
!$$$      useLocalTimeStepping=ipar(19)

      pdeModel           =ipar(23)

!     *****************************************************
!     ********DETERMINE THE TIME STEPPING EIGENVALUES *****
!     *****************************************************      

      if( turbulenceModel.eq.noTurbulenceModel )then

        if( pdeModel.eq.standardModel .or. pdeModel.eq.BoussinesqModel 
     & )then
          call insdtsINS(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,
     & nd2b,nd3a,nd3b,nd4a,nd4b,mask,xy,rsxy,  u,uu, gv,dw, 
     & divDamping, dtVar, ndMatProp,matIndex,matValpc,matVal, bc, 
     & ipar, rpar, pdb, ierr )
        else if( pdeModel.eq.viscoPlasticModel )then
          call insdtsVP(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,
     & nd3a,nd3b,nd4a,nd4b,mask,xy,rsxy,  u,uu, gv,dw, divDamping, 
     & dtVar, ndMatProp,matIndex,matValpc,matVal,bc, ipar, rpar, pdb, 
     & ierr )
        else if( pdeModel.eq.twoPhaseFlowModel )then
          call insdtsTP(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,
     & nd3a,nd3b,nd4a,nd4b,mask,xy,rsxy,  u,uu, gv,dw, divDamping, 
     & dtVar, ndMatProp,matIndex,matValpc,matVal,bc, ipar, rpar, pdb, 
     & ierr )
        else
          write(*,'("insdts:ERROR::pdeModel=",i2)') pdeModel
          stop 45
        end if

      else if( turbulenceModel.eq.spalartAllmaras )then

        call insdtsSPAL(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,
     & nd3a,nd3b,nd4a,nd4b,mask,xy,rsxy,  u,uu, gv,dw, divDamping, 
     & dtVar, ndMatProp,matIndex,matValpc,matVal,bc, ipar, rpar, pdb, 
     & ierr )

      else if( turbulenceModel.eq.baldwinLomax )then

        call insdtsBL(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,
     & nd3a,nd3b,nd4a,nd4b,mask,xy,rsxy,  u,uu, gv,dw, divDamping, 
     & dtVar, ndMatProp,matIndex,matValpc,matVal,bc, ipar, rpar, pdb, 
     & ierr )

      else if( turbulenceModel.eq.kEpsilon )then

        call insdtsKE(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,
     & nd3a,nd3b,nd4a,nd4b,mask,xy,rsxy,  u,uu, gv,dw, divDamping, 
     & dtVar, ndMatProp,matIndex,matValpc,matVal,bc, ipar, rpar, pdb, 
     & ierr )

      else if( turbulenceModel.eq.largeEddySimulation )then
        ! use the VP model to get the time step here
        call insdtsVP(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,
     & nd3a,nd3b,nd4a,nd4b,mask,xy,rsxy,  u,uu, gv,dw, divDamping, 
     & dtVar, ndMatProp,matIndex,matValpc,matVal,bc, ipar, rpar, pdb, 
     & ierr )
      else
        stop 33
      end if


      return
      end

! 
! : empty version for linking when we don't want an option
!





