! This file automatically generated from advSOSUP.bf with bpp.
! 
!  SOSUP: optimized implementations for the second-order-system Upwind Schemes
!
! 2012/09 - Jeff Banks, initial version

! **********************************************************************************
! NAME: name of the subroutine
! DIM : 2 or 3
! ORDER : 2 ,4, 6 or 8
! GRIDTYPE : rectangular, curvilinear
! **********************************************************************************















      subroutine advSOSUP(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,
     & nd2b,nd3a,nd3b,nd4a,nd4b,ndf4a,ndf4b,mask,rx,  u,un,f, bc, 
     & ipar, rpar, ierr )
!======================================================================
!   Advance a time step for Maxwells eqution
!     SOSUP -- second-order-system upwind scheme
!
! nd : number of space dimensions
!
!======================================================================
      implicit none
      integer nd, n1a,n1b,n2a,n2b,n3a,n3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,ndf4a,ndf4b

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real un(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real f(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,ndf4a:ndf4b,0:*)
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


      ! write(*,*) 'Inside advSOSUP...'

      orderOfAccuracy    =ipar(2)
      gridType           =ipar(1)

      if( orderOfAccuracy.eq.2 )then

        if( nd.eq.2 .and. gridType.eq.rectangular ) then
          call advMxSOSUP2dOrder2r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,
     & nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,ndf4a,ndf4b,mask,rx, u,un,f,
     &  bc, ipar, rpar, ierr )
        else if( nd.eq.2 .and. gridType.eq.curvilinear ) then
          call advMxSOSUP2dOrder2c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,
     & nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,ndf4a,ndf4b,mask,rx, u,un,f,
     &  bc, ipar, rpar, ierr )
        else if( nd.eq.3 .and. gridType.eq.rectangular ) then

          call advMxSOSUP3dOrder2r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,
     & nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,ndf4a,ndf4b,mask,rx, u,un,f,
     &  bc, ipar, rpar, ierr )

        else if( nd.eq.3 .and. gridType.eq.curvilinear ) then
           call advMxSOSUP3dOrder2c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,
     & nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,ndf4a,ndf4b,mask,rx, u,un,f,
     &  bc, ipar, rpar, ierr )

        else
          stop 2271
        end if

      else if( orderOfAccuracy.eq.4 ) then
        if( nd.eq.2 .and. gridType.eq.rectangular )then
          call advMxSOSUP2dOrder4r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,
     & nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,ndf4a,ndf4b,mask,rx, u,un,f,
     &  bc, ipar, rpar, ierr )
        else if(nd.eq.2 .and. gridType.eq.curvilinear )then
          call advMxSOSUP2dOrder4c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,
     & nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,ndf4a,ndf4b,mask,rx, u,un,f,
     &  bc, ipar, rpar, ierr )
        else if(  nd.eq.3 .and. gridType.eq.rectangular )then
          call advMxSOSUP3dOrder4r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,
     & nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,ndf4a,ndf4b,mask,rx, u,un,f,
     &  bc, ipar, rpar, ierr )
        else if(  nd.eq.3 .and. gridType.eq.curvilinear )then
          call advMxSOSUP3dOrder4c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,
     & nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,ndf4a,ndf4b,mask,rx, u,un,f,
     &  bc, ipar, rpar, ierr )
        else
          stop 8843
        end if

!
      else if( orderOfAccuracy.eq.6 ) then
         if( nd.eq.2 .and. gridType.eq.rectangular )then
           call advMxSOSUP2dOrder6r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,
     & nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,ndf4a,ndf4b,mask,rx, u,un,f,
     &  bc, ipar, rpar, ierr )
        else if(nd.eq.2 .and. gridType.eq.curvilinear )then
           call advMxSOSUP2dOrder6c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,
     & nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,ndf4a,ndf4b,mask,rx, u,un,f,
     &  bc, ipar, rpar, ierr )
        else if(  nd.eq.3 .and. gridType.eq.rectangular )then
           call advMxSOSUP3dOrder6r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,
     & nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,ndf4a,ndf4b,mask,rx, u,un,f,
     &  bc, ipar, rpar, ierr )
c          stop 2101
        else if(  nd.eq.3 .and. gridType.eq.curvilinear )then
           call advMxSOSUP3dOrder6c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,
     & nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,ndf4a,ndf4b,mask,rx, u,un,f,
     &  bc, ipar, rpar, ierr )
c          stop 2101
        else
          stop 8843
       end if

      else if( orderOfAccuracy.eq.8 ) then

        stop 8843

      else
        write(*,'(" advSOSUP:ERROR: un-implemented order of accuracy 
     & =",i6)') orderOfAccuracy
          ! '
        stop 11122
      end if

      return
      end
