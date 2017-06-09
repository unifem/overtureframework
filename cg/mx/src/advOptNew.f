! This file automatically generated from advOptNew.bf with bpp.
!
! Optimized advance routines for cgmx
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


! ---------------------------------------------------------------------------
! Macro : beginLoopsMask
! ---------------------------------------------------------------------------

! ---------------------------------------------------------------------------
! Macro : endLoopsMask
! ---------------------------------------------------------------------------







! This macro is used for variable dissipation in 2D

! This macro is used for variable dissipation in 3D

! This macro is used for variable dissipation in 3D



! Optionally add the forcing terms

! Optionally add the forcing terms
! Optionally solve for E or H or both


! Optionally add the dissipation and or forcing terms


! Optionally add the dissipation and or forcing terms
! Optionally solve for E or H or both

! The next macro is used for curvilinear girds where the Laplacian term is precomputed.

! -------------------------------------------------------------------------------------------
! The next macro is used for curvilinear girds where the Laplacian term is precomputed.
! Optionally add dissipation too
! -------------------------------------------------------------------------------------------


! -------------------------------------------------------------------------------------------
! The next macro is used for curvilinear girds where the Laplacian term is precomputed.
! Optionally add dissipation too
! -------------------------------------------------------------------------------------------

! -------------------------------------------------------------------------------------------
! The next macro is used for curvilinear girds where the Laplacian term is precomputed.
! Optionally add dissipation too
! -------------------------------------------------------------------------------------------











! ** evaluate the laplacian on the 9 points centred at (i1,i2,i3)


! ** evaluate the square of the Laplacian for a component ****

! ** evaluate the square of the Laplacian for [ex,ey,hz] ****

! ==== loops for curvilinear, with forcing, dissipation in 2D
! Optionally add the dissipation and or forcing terms

! ==== loops for curvilinear, with forcing, dissipation in 2D
! Optionally add the dissipation and or forcing terms

! ===========================================================================================
! Macro: compute the coefficients in the sosup dissipation for curvilinear grids
! ===========================================================================================


! **********************************************************************************
! Macro ADV_MAXWELL:
!  NAME: name of the subroutine
!  DIM : 2 or 3
!  ORDER : 2 ,4, 6 or 8
!  GRIDTYPE : rectangular, curvilinear
! **********************************************************************************





! -- Most of these are built with advOpt.bf

!      buildFile(advMx3dOrder2r,3,2,rectangular)
!
!      buildFile(advMx3dOrder2c,3,2,curvilinear)
!
!      buildFile(advMx3dOrder4r,3,4,rectangular)
!
!      buildFile(advMx3dOrder4c,3,4,curvilinear)
!
!      buildFile(advMx2dOrder6r,2,6,rectangular)
!      buildFile(advMx3dOrder6r,3,6,rectangular)
!
!       ! build these for testing symmetric operators -- BC's not implemented yet
!      buildFile(advMx2dOrder6c,2,6,curvilinear)
!      buildFile(advMx3dOrder6c,3,6,curvilinear)
!
!      buildFile(advMx2dOrder8r,2,8,rectangular)
!      buildFile(advMx3dOrder8r,3,8,rectangular)
!
!       ! build these for testing symmetric operators -- BC's not implemented yet
!      buildFile(advMx2dOrder8c,2,8,curvilinear)
!      buildFile(advMx3dOrder8c,3,8,curvilinear)



! build an empty version of high order files so we do not have to compile the full version



!      buildFileNull(advMx2dOrder6r,2,6,rectangular)
!      buildFileNull(advMx3dOrder6r,3,6,rectangular)
!
!      buildFileNull(advMx2dOrder6c,2,6,curvilinear)
!      buildFileNull(advMx3dOrder6c,3,6,curvilinear)
!
!      buildFileNull(advMx2dOrder8r,2,8,rectangular)
!      buildFileNull(advMx3dOrder8r,3,8,rectangular)
!
!      buildFileNull(advMx2dOrder8c,2,8,curvilinear)
!      buildFileNull(advMx3dOrder8c,3,8,curvilinear)



      subroutine advMaxwell(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,
     & nd2b,nd3a,nd3b,nd4a,nd4b,mask,rx,  um,u,un,f,fa, v,vvt2,ut3,
     & vvt4,ut5,ut6,ut7, bc, dis, varDis, ipar, rpar, ierr )
!======================================================================
!   Advance a time step for Maxwells eqution
!     OPTIMIZED version for rectangular grids.
! nd : number of space dimensions
!
! ipar(0)  = option : option=0 - Maxwell+Artificial diffusion
!                           =1 - AD only
!======================================================================
      implicit none
      integer nd, n1a,n1b,n2a,n2b,n3a,n3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b

      real um(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real un(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real f(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real fa(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b,0:*)  ! forcings at different times
      real v(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real vvt2(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real ut3(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real vvt4(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real ut5(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real ut6(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real ut7(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real dis(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real varDis(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real rx(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer bc(0:1,0:2),ierr

      integer ipar(0:*)
      real rpar(0:*)

!     ---- local variables -----
      integer c,i1,i2,i3,n,gridType,orderOfAccuracy,orderInTime
      integer addForcing,orderOfDissipation,option
      integer useWhereMask,solveForE,solveForH,grid
      integer ex,ey,ez, hx,hy,hz

      integer rectangular,curvilinear
      parameter( rectangular=0, curvilinear=1 )
!...........end   statement functions


      ! write(*,*) 'Inside advMaxwell...'

      orderOfAccuracy    =ipar(2)
      gridType           =ipar(1)

      if( orderOfAccuracy.eq.2 )then

        if( nd.eq.2 .and. gridType.eq.rectangular ) then
          call advMx2dOrder2r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rx, um,u,un,f,fa, v,vvt2,
     & ut3,vvt4,ut5,ut6,ut7,bc, dis,varDis, ipar, rpar, ierr )
        else if( nd.eq.2 .and. gridType.eq.curvilinear ) then
          call advMx2dOrder2c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rx, um,u,un,f,fa, v,vvt2,
     & ut3,vvt4,ut5,ut6,ut7,bc, dis,varDis, ipar, rpar, ierr )
        else if( nd.eq.3 .and. gridType.eq.rectangular ) then
          call advMx3dOrder2r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rx, um,u,un,f,fa, v,vvt2,
     & ut3,vvt4,ut5,ut6,ut7,bc, dis,varDis, ipar, rpar, ierr )
        else if( nd.eq.3 .and. gridType.eq.curvilinear ) then
          call advMx3dOrder2c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rx, um,u,un,f,fa, v,vvt2,
     & ut3,vvt4,ut5,ut6,ut7,bc, dis,varDis, ipar, rpar, ierr )
        else
          stop 2271
        end if

      else if( orderOfAccuracy.eq.4 ) then
        if( nd.eq.2 .and. gridType.eq.rectangular )then
          call advMx2dOrder4r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rx, um,u,un,f,fa, v,vvt2,
     & ut3,vvt4,ut5,ut6,ut7,bc, dis,varDis, ipar, rpar, ierr )
        else if(nd.eq.2 .and. gridType.eq.curvilinear )then
          call advMx2dOrder4c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rx, um,u,un,f,fa, v,vvt2,
     & ut3,vvt4,ut5,ut6,ut7,bc, dis,varDis, ipar, rpar, ierr )
        else if(  nd.eq.3 .and. gridType.eq.rectangular )then
          call advMx3dOrder4r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rx, um,u,un,f,fa, v,vvt2,
     & ut3,vvt4,ut5,ut6,ut7,bc, dis,varDis, ipar, rpar, ierr )
        else if(  nd.eq.3 .and. gridType.eq.curvilinear )then
          call advMx3dOrder4c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rx, um,u,un,f,fa, v,vvt2,
     & ut3,vvt4,ut5,ut6,ut7,bc, dis,varDis, ipar, rpar, ierr )
       else
         stop 8843
       end if

!
      else if( orderOfAccuracy.eq.6 ) then
        if( nd.eq.2 .and. gridType.eq.rectangular )then
          call advMx2dOrder6r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rx, um,u,un,f,fa, v,vvt2,
     & ut3,vvt4,ut5,ut6,ut7,bc, dis,varDis, ipar, rpar, ierr )
        else if(nd.eq.2 .and. gridType.eq.curvilinear )then
          call advMx2dOrder6c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rx, um,u,un,f,fa, v,vvt2,
     & ut3,vvt4,ut5,ut6,ut7,bc, dis,varDis, ipar, rpar, ierr )
        else if(  nd.eq.3 .and. gridType.eq.rectangular )then
          call advMx3dOrder6r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rx, um,u,un,f,fa, v,vvt2,
     & ut3,vvt4,ut5,ut6,ut7,bc, dis,varDis, ipar, rpar, ierr )
        else if(  nd.eq.3 .and. gridType.eq.curvilinear )then
          call advMx3dOrder6c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rx, um,u,un,f,fa, v,vvt2,
     & ut3,vvt4,ut5,ut6,ut7,bc, dis,varDis, ipar, rpar, ierr )
       else
         stop 8843
       end if

      else if( orderOfAccuracy.eq.8 ) then

        if( nd.eq.2 .and. gridType.eq.rectangular )then
          call advMx2dOrder8r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rx, um,u,un,f,fa, v,vvt2,
     & ut3,vvt4,ut5,ut6,ut7,bc, dis,varDis, ipar, rpar, ierr )
        else if(nd.eq.2 .and. gridType.eq.curvilinear )then
          call advMx2dOrder8c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rx, um,u,un,f,fa, v,vvt2,
     & ut3,vvt4,ut5,ut6,ut7,bc, dis,varDis, ipar, rpar, ierr )
        else if(  nd.eq.3 .and. gridType.eq.rectangular )then
          call advMx3dOrder8r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rx, um,u,un,f,fa, v,vvt2,
     & ut3,vvt4,ut5,ut6,ut7,bc, dis,varDis, ipar, rpar, ierr )
        else if(  nd.eq.3 .and. gridType.eq.curvilinear )then
          call advMx3dOrder8c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rx, um,u,un,f,fa, v,vvt2,
     & ut3,vvt4,ut5,ut6,ut7,bc, dis,varDis, ipar, rpar, ierr )
       else
         stop 8843
       end if

      else
        write(*,'(" advMaxwell:ERROR: un-implemented order of accuracy 
     & =",i6)') orderOfAccuracy
          ! '
        stop 11122
      end if

      return
      end








