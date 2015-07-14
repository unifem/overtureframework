! This file automatically generated from bcOptSmFOS.bf with bpp.
c *******************************************************************************
c  Cgsm: solid Mechanics boundary conditions for the First-order system
c *******************************************************************************

c These next include files will define the macros that will define the difference approximations
c The actual macro is called below
c #Include "defineDiffNewerOrder2f.h"
c #Include "defineDiffNewerOrder4f.h"







! loop on the boundary but include ghost points in tangential directions





















c ************************************************************************************************
c  This macro is used for looping over the faces of a grid to assign booundary conditions
c
c extra: extra points to assign
c          Case 1: extra=numberOfGhostPoints -- for assigning extended boundaries
c          Case 2: extra=-1 -- for assigning ghost points but not including extended boundaries
c numberOfGhostPoints : number of ghost points (1 for 2nd order, 2 for fourth-order ...)
c
c
c Output:
c  n1a,n1b,n2a,n2b,n3a,n3b : from gridIndexRange
c  nn1a,nn1b,nn2a,nn2b,nn3a,nn3b : includes "extra" points
c 
c ***********************************************************************************************


! ====================================================================
! Look up an integer parameter from the data base
! ====================================================================

! ====================================================================
! Look up a real parameter from the data base
! ====================================================================

! ==========================================================================================================
! This macro will pin the solution at specified corners or edges.
! ==========================================================================================================


! ==================================================================================
!  Use this next macro to fill in values to the boundary forcing array 
!  (We cannot use the statement function bcf on the LHS of an expression)
! ==================================================================================



      subroutine bcOptSmFOS( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     & gridIndexRange, u, mask,rx, xy, ndMatProp,matIndex,matValpc,
     & matVal, det, boundaryCondition, addBoundaryForcing, 
     & interfaceType, dim, bcf00,bcf10,bcf01,bcf11,bcf02,bcf12,bcf0,
     & bcOffset, ndpin, pinbc, ndpv, pinValues, ipar, rpar, pdb, ierr 
     & )
c ===================================================================================
c  Boundary conditions for solid mechanics : First Order System
c
c  gridType : 0=rectangular, 1=curvilinear
c
c  c2= mu/rho, c1=(mu+lambda)/rho;
c 
c The forcing for the boundary conditions can be accessed in two ways. One can either 
c use the arrays: 
c       bcf00(i1,i2,i3,m), bcf10(i1,i2,i3,m), bcf01(i1,i2,i3,m), bcf11(i1,i2,i3,m), 
c       bcf02(i1,i2,i3,m), bcf12(i1,i2,i3,m)
c which provide values for the 6 different faces in 6 different arrays. One can also
c access the same values using the single statement function
c         bcf(side,axis,i1,i2,i3,m)
c which is defined below. 
c ===================================================================================

      implicit none

      integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,ndpin,ndpv, ierr

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real rx(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      real det(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer gridIndexRange(0:1,0:2),boundaryCondition(0:1,0:2)

      integer addBoundaryForcing(0:1,0:2)
      integer interfaceType(0:1,0:2,0:*)
      integer dim(0:1,0:2,0:1,0:2)
      integer pinbc(0:ndpin-1,0:*)
      real pinValues(0:ndpv-1,0:*)

      real bcf00(dim(0,0,0,0):dim(1,0,0,0), dim(0,1,0,0):dim(1,1,0,0), 
     & dim(0,2,0,0):dim(1,2,0,0),0:*)
      real bcf10(dim(0,0,1,0):dim(1,0,1,0), dim(0,1,1,0):dim(1,1,1,0), 
     & dim(0,2,1,0):dim(1,2,1,0),0:*)
      real bcf01(dim(0,0,0,1):dim(1,0,0,1), dim(0,1,0,1):dim(1,1,0,1), 
     & dim(0,2,0,1):dim(1,2,0,1),0:*)
      real bcf11(dim(0,0,1,1):dim(1,0,1,1), dim(0,1,1,1):dim(1,1,1,1), 
     & dim(0,2,1,1):dim(1,2,1,1),0:*)
      real bcf02(dim(0,0,0,2):dim(1,0,0,2), dim(0,1,0,2):dim(1,1,0,2), 
     & dim(0,2,0,2):dim(1,2,0,2),0:*)
      real bcf12(dim(0,0,1,2):dim(1,0,1,2), dim(0,1,1,2):dim(1,1,1,2), 
     & dim(0,2,1,2):dim(1,2,1,2),0:*)

      real bcf0(0:*)
      integer*8 bcOffset(0:1,0:2)

      integer ipar(0:*)
      real rpar(0:*)

      ! -- Declare arrays for variable material properties --
      include 'declareVarMatProp.h'

      double precision pdb  ! pointer to data base

      if( nd.eq.2 )then
        call bcSmFOS2d( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     & gridIndexRange, u, mask,rx, xy, ndMatProp,matIndex,matValpc,
     & matVal, det, boundaryCondition, addBoundaryForcing, 
     & interfaceType, dim, bcf00,bcf10,bcf01,bcf11,bcf02,bcf12,bcf0,
     & bcOffset, ndpin, pinbc, ndpv, pinValues, ipar, rpar, pdb, ierr 
     & )

      else
        call bcSmFOS3d( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     & gridIndexRange, u, mask,rx, xy, ndMatProp,matIndex,matValpc,
     & matVal, det, boundaryCondition, addBoundaryForcing, 
     & interfaceType, dim, bcf00,bcf10,bcf01,bcf11,bcf02,bcf12,bcf0,
     & bcOffset, ndpin, pinbc, ndpv, pinValues, ipar, rpar, pdb, ierr 
     & )
      end if

      return
      end


! ===========================================================================================
!  -- Fill in forcing arrays if they are not provided ---
!
!   Fill-in the boundary forcing array:
!                bcfa(side,axis,i1,i2,i3,uc:*)
! ===========================================================================================


!========================================================================
!******* Primary Dirichlet boundary conditions ***********
!========================================================================


!============================================================================================
! Macro: 
!    -- Extrapolate to the first ghost cells (only for physical sides) 
!
!   Note: symmetry BC's are not done here
!============================================================================================


!============================================================================================
! Macro: 
!    -- Assign the symmetry BC on a given ghost line 
! Input:
!  ghostLine : 1 or 2 
!============================================================================================


!=========================================================================================
!******* Fix up components of stress in the corners (such as n1a,n2a) ********
!=========================================================================================




!=============================================================================================
!******* Secondary Neumann boundary conditions (compatibility conditions) ********
!=============================================================================================

! ==========================================================================================================
! ******* Secondary Dirichlet conditions for the tangential components of stress (tractionBC only) ********
! ==========================================================================================================

!========================================================================================
! ******* Secondary Dirichlet conditions for stress (slipWall only) ********
!========================================================================================

! ===================================================================================================
! ******* Extrapolation of stress to the first ghost line (for the tractionBC case only) ********
! ===================================================================================================

! ========================================================================================
!******* Extrapolation to the second ghost line ******** 
! ========================================================================================

! =======================================================================================
! Set the solution to be the exact (TZ) solution in corners
! =======================================================================================

!  ======================================================================================
! Debug routine to print the solution
!  ======================================================================================



!  ======================================================================================
!    Macro to define the 2d and 3d BC routines
!
!  NAME : name of the subroutine
!  DIM  : 2 or 3 for 2d or 3d
!  ======================================================================================

! ---- Here we create the 2d and 3d subroutines in different files 




c++++++++++++++++++++++++++++++++++++++++++++++++

      subroutine smbcsdp (ux,uy,vx,vy,lambda,mu,p,dpdf,ideriv)

      implicit none
      integer ideriv
      real ux,uy,vx,vy,lambda,mu,p(2,2),dpdf(4,4)

c local variables
      real f(2,2),s(2,2),trace

      write(6,*)'Error (smbcsdp) : no longer used'
      pause

      f(1,1)=1.0+ux
      f(1,2)=    uy
      f(2,1)=    vx
      f(2,2)=1.0+vy
      s(1,1)=0.5*(f(1,1)*f(1,1)+f(2,1)*f(2,1)-1.0)   ! this is E(i,j), for now, symmetric
      s(1,2)=0.5*(f(1,1)*f(1,2)+f(2,1)*f(2,2)    )
      s(2,2)=0.5*(f(1,2)*f(1,2)+f(2,2)*f(2,2)-1.0)
      trace=s(1,1)+s(2,2)                            ! this is Tr(E)
      s(1,1)=lambda*trace+2.0*mu*s(1,1)              ! this is S(i,j), symmetric
      s(1,2)=             2.0*mu*s(1,2)
      s(2,2)=lambda*trace+2.0*mu*s(2,2)
      p(1,1)=s(1,1)*f(1,1)+s(1,2)*f(1,2)             ! this is P(i,j) based on the current F(i,j)
      p(1,2)=s(1,1)*f(2,1)+s(1,2)*f(2,2)
      p(2,1)=s(1,2)*f(1,1)+s(2,2)*f(1,2)
      p(2,2)=s(1,2)*f(2,1)+s(2,2)*f(2,2)

      if (ideriv.eq.0) return

      dpdf(1,1)=lambda*f(1,1)*f(1,1)+mu*f(1,1)*f(1,1)        ! K(1,1,1,1)
      dpdf(1,2)=lambda*f(1,1)*f(1,2)+mu*f(1,2)*f(1,1)        ! K(1,1,1,2)
      dpdf(1,3)=lambda*f(1,1)*f(2,1)+mu*f(1,1)*f(2,1)        ! K(1,1,2,1)
      dpdf(1,4)=lambda*f(1,1)*f(2,2)+mu*f(1,2)*f(2,1)        ! K(1,1,2,2)
      dpdf(2,1)=lambda*f(2,1)*f(1,1)+mu*f(2,1)*f(1,1)        ! K(1,2,1,1)
      dpdf(2,2)=lambda*f(2,1)*f(1,2)+mu*f(2,2)*f(1,1)        ! K(1,2,1,2)
      dpdf(2,3)=lambda*f(2,1)*f(2,1)+mu*f(2,1)*f(2,1)        ! K(1,2,2,1)
      dpdf(2,4)=lambda*f(2,1)*f(2,2)+mu*f(2,2)*f(2,1)        ! K(1,2,2,2)
      dpdf(3,1)=lambda*f(1,2)*f(1,1)+mu*f(1,1)*f(1,2)        ! K(2,1,1,1)
      dpdf(3,2)=lambda*f(1,2)*f(1,2)+mu*f(1,2)*f(1,2)        ! K(2,1,1,2)
      dpdf(3,3)=lambda*f(1,2)*f(2,1)+mu*f(1,1)*f(2,2)        ! K(2,1,2,1)
      dpdf(3,4)=lambda*f(1,2)*f(2,2)+mu*f(1,2)*f(2,2)        ! K(2,1,2,2)
      dpdf(4,1)=lambda*f(2,2)*f(1,1)+mu*f(2,1)*f(1,2)        ! K(2,2,1,1)
      dpdf(4,2)=lambda*f(2,2)*f(1,2)+mu*f(2,2)*f(1,2)        ! K(2,2,1,2)
      dpdf(4,3)=lambda*f(2,2)*f(2,1)+mu*f(2,1)*f(2,2)        ! K(2,2,2,1)
      dpdf(4,4)=lambda*f(2,2)*f(2,2)+mu*f(2,2)*f(2,2)        ! K(2,2,2,2)

      dpdf(1,1)=dpdf(1,1)+mu*(f(1,1)*f(1,1)+f(1,2)*f(1,2))   ! K(1,1,1,1)
      dpdf(1,3)=dpdf(1,3)+mu*(f(1,1)*f(2,1)+f(1,2)*f(2,2))   ! K(1,1,2,1)
      dpdf(2,1)=dpdf(2,1)+mu*(f(2,1)*f(1,1)+f(2,2)*f(1,2))   ! K(1,2,1,1)
      dpdf(2,3)=dpdf(2,3)+mu*(f(2,1)*f(2,1)+f(2,2)*f(2,2))   ! K(1,2,2,1)
      dpdf(3,2)=dpdf(3,2)+mu*(f(1,1)*f(1,1)+f(1,2)*f(1,2))   ! K(2,1,1,2)
      dpdf(3,4)=dpdf(3,4)+mu*(f(1,1)*f(2,1)+f(1,2)*f(2,2))   ! K(2,1,2,2)
      dpdf(4,2)=dpdf(4,2)+mu*(f(2,1)*f(1,1)+f(2,2)*f(1,2))   ! K(2,2,1,2)
      dpdf(4,4)=dpdf(4,4)+mu*(f(2,1)*f(2,1)+f(2,2)*f(2,2))   ! K(2,2,2,2)

      dpdf(1,1)=dpdf(1,1)+s(1,1)                             ! K(1,1,1,1)
      dpdf(1,2)=dpdf(1,2)+s(1,2)                             ! K(1,1,1,2)
      dpdf(2,3)=dpdf(2,3)+s(1,1)                             ! K(1,2,2,1)
      dpdf(2,4)=dpdf(2,4)+s(1,2)                             ! K(1,2,2,2)
      dpdf(3,1)=dpdf(3,1)+s(1,2)                             ! K(2,1,1,1)
      dpdf(3,2)=dpdf(3,2)+s(2,2)                             ! K(2,1,1,2)
      dpdf(4,3)=dpdf(4,3)+s(1,2)                             ! K(2,2,2,1)
      dpdf(4,4)=dpdf(4,4)+s(2,2)                             ! K(2,2,2,2)

      return
      end

