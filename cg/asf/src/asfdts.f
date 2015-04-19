! This file automatically generated from asfdts.bf with bpp.
c
c Compute the time step for the ALL-SPEED compressible NS 
c
c ----------- this file started from cnsdts.bf --------------------
c 


c These next include files will define the macros that will define the difference approximations
c The actual macro is called below
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







c Define macros for the derivatives based on the dimension, order of accuracy and grid-type


c =============================================================
c Compute derivatives of u,v,w 
c =============================================================

c =============================================================
c Compute Spalart-Allmaras quantities
c   This macro assumes u0x,u0y, ... are defined
c =============================================================



c ============== from inspf.bf ***
c Return nuT and it''s first derivatives for SPAL

c ============== from inspf.bf ***
c Return nuT and it''s first derivatives for BL

c ============== from inspf.bf ***
c Return nuT and it''s first derivatives for KE

c ============== from inspf.bf ***

c====================================================================================
c
c SOLVER: ASF, ASFSPAL, ASFBL, ASFKE
c METHOD: GLOBAL, LOCAL  (GLOBAL=fixed time step, LOCAL=local-time stepping -> compute dtVar)
c OPTION: EXPLICIT, IMPLICIT
c ADTYPE: AD2, AD4, AD24 --- NOT USED NOW
c ORDER: 2,4
c DIM: 2,3
c GRIDTYPE: rectangular, curvilinear
c AXISYMMETRIC: notAxisymmetric, or axisymmetric
c====================================================================================

c ====================================================================================
c ====================================================================================







! This is not finished yet..




c ============================================================================================================
c  Define the subroutine that compute the time stepping eigenvalues for a given solver
c ============================================================================================================


      subroutine asfdts(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,
     & nd3a,nd3b,nd4a,nd4b,mask,xy,rsxy,  u,uu, gv,dw, rL, pL, dtVar, 
     & bc, ipar, rpar, pdb, ierr )
c======================================================================
c
c    Determine the time step for the ASF equations.
c    ---------------------------------------------
c
c nd : number of space dimensions
c
c gv : gridVelocity for moving grids
c uu : for moving grids uu is a workspace to hold u-gv, otherwise uu==u
c dw : distance to the wall for some turbulence models
c p : pressure
c dp : work space for Jameson dissipation
c======================================================================
      implicit none
      integer nd, n1a,n1b,n2a,n2b,n3a,n3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real uu(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real dw(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real rL(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)    ! linearized rho
      real pL(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)    ! linearized pressure
      real dtVar(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer bc(0:1,0:2),ierr

      integer ipar(0:*)
      real rpar(0:*)

      double precision pdb  ! pointer to data base

c     ---- local variables -----

      integer pdeModel

      integer turbulenceModel,noTurbulenceModel
      integer baldwinLomax,spalartAllmaras,kEpsilon,kOmega
      parameter (noTurbulenceModel=0,baldwinLomax=1,kEpsilon=2,
     & kOmega=3,spalartAllmaras=4 )


c     --- end statement functions

      ierr=0
      write(*,'("Inside asfdts: nd=",i2)') nd


      pdeModel       =ipar(0)
      turbulenceModel=ipar(1)

      write(*,'("Inside asfdts: pdeModel,turbulenceModel=",2i2)') 
     & pdeModel,turbulenceModel

c     *****************************************************
c     ********DETERMINE THE TIME STEPPING EIGENVALUES *****
c     *****************************************************      

      if( turbulenceModel.eq.noTurbulenceModel )then

        call asfdtsASF(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,
     & nd3a,nd3b,nd4a,nd4b,mask,xy,rsxy,  u,uu, gv,dw, rL, pL, dtVar, 
     & bc, ipar, rpar, pdb, ierr )

      else if( turbulenceModel.eq.spalartAllmaras )then

c       call asfdtsSPAL(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,c         mask,xy,rsxy,  u,uu, gv,dw, rL, pL, dtVar, bc, ipar, rpar, pdb, ierr )

      else if( turbulenceModel.eq.baldwinLomax )then

c       call asfdtsBL(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,c         mask,xy,rsxy,  u,uu, gv,dw, rL, pL, dtVar, bc, ipar, rpar, pdb, ierr )

      else if( turbulenceModel.eq.kEpsilon )then

c       call asfdtsKE(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,c         mask,xy,rsxy,  u,uu, gv,dw, rL, pL, dtVar, bc, ipar, rpar, pdb, ierr )

      else
        stop 33
      end if


      return
      end




c     buildFile(ASFSPAL,asfdtsSPAL)
c     buildFile(ASFBL,asfdtsBL)
c     buildFile(ASFKE,asfdtsKE)

