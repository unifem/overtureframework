      subroutine duWaveGen3d2cc( 
     *   nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *   n1a,n1b,n2a,n2b,n3a,n3b,
     *   ndf4a,ndf4b,nComp,addForcing,
     *   u,ut,unew,utnew,
     *   rx,src,
     *   dx,dy,dz,dt,cc,
     *   useWhereMask,mask )
c
      implicit none
c
c.. declarations of incoming variables
      integer nd1a,nd1b,nd2a,nd2b,nd3a,nd3b
      integer n1a,n1b,n2a,n2b,n3a,n3b
      integer ndf4a,ndf4b,nComp,addForcing
      integer useWhereMask
      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)

      real u    (nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real ut   (nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real unew (nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real utnew(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real rx   (nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:2,0:2)
      real src  (nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,ndf4a:ndf4b,0:*)
      real dx,dy,dz,dt,cc
c
c.. declarations of local variables
      integer i,j,k,n
c
      n = 1
c
      if( n1a-nd1a .lt. 2 ) then
        write(6,*)'Grid not made with enough ghost cells'
        write(6,*)nd1a,n1a
        stop
      end if

      ! second  order, curvilinear, 3D
      if( addForcing.eq.0 )then

        if( useWhereMask.ne.0 ) then
          do k = n3a,n3b
          do j = n2a,n2b
          do i = n1a,n1b
          if( mask(i,j,k).gt.0 ) then
c
            call duStepWaveGen3d2cc( 
     *         nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *         n1a,n1b,n2a,n2b,n3a,n3b,
     *         u,ut,unew,utnew,rx,
     *         dx,dy,dz,dt,cc,
     *         i,j,k,n )
c
          end if
          end do
          end do
          end do
        else
          ! no mask
          do k = n3a,n3b
          do j = n2a,n2b
          do i = n1a,n1b
c   
            call duStepWaveGen3d2cc( 
     *         nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *         n1a,n1b,n2a,n2b,n3a,n3b,
     *         u,ut,unew,utnew,rx,
     *         dx,dy,dz,dt,cc,
     *         i,j,k,n )
c
          end do
          end do
          end do
        end if

      else
        ! add forcing flag is set to true

        if( useWhereMask.ne.0 ) then

          do k = n3a,n3b
          do j = n2a,n2b
          do i = n1a,n1b
          if( mask(i,j,k).gt.0 ) then
c
            call duStepWaveGen3d2cc_tz( 
     *         nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *         n1a,n1b,n2a,n2b,n3a,n3b,
     *         ndf4a,ndf4b,nComp,
     *         u,ut,unew,utnew,
     *         rx,src,
     *         dx,dy,dz,dt,cc,
     *         i,j,k,n )
c
          end if
          end do
          end do
          end do
        else
          ! no mask
          do k = n3a,n3b
          do j = n2a,n2b
          do i = n1a,n1b
c   
            call duStepWaveGen3d2cc_tz( 
     *         nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *         n1a,n1b,n2a,n2b,n3a,n3b,
     *         ndf4a,ndf4b,nComp,
     *         u,ut,unew,utnew,
     *         rx,src,
     *         dx,dy,dz,dt,cc,
     *         i,j,k,n )
c
          end do
          end do
          end do
c
        end if
      end if
c
      return
      end
