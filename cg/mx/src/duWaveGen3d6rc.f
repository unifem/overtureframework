      subroutine duWaveGen3d6rc( 
     *   nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *   n1a,n1b,n2a,n2b,n3a,n3b,
     *   ndf4a,ndf4b,nComp,addForcing,
     *   u,ut,unew,utnew,
     *   src,
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
      real src  (nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,ndf4a:ndf4b,-1:*)
      real dx,dy,dz,dt,cc
c
c.. declarations of local variables
      integer i,j,k,n
      integer ix,iy,iz
c
      integer stencilOpt

      real cuu(-4:4,-4:4,-4:4)
      real cuv(-4:4,-4:4,-4:4)
      real cvu(-4:4,-4:4,-4:4)
      real cvv(-4:4,-4:4,-4:4)
c
      n = 1
      stencilOpt = 1
c
      if( n1a-nd1a .lt. 4 ) then
        write(6,*)'Grid not made with enough ghost cells'
        write(6,*)nd1a,n1a
        stop
      end if

      ! sixth order, cartesian, 3D
      if( addForcing.eq.0 )then

        if( stencilOpt .eq. 0 ) then
          if( useWhereMask.ne.0 ) then
            do k = n3a,n3b
            do j = n2a,n2b
            do i = n1a,n1b
            if( mask(i,j,k).gt.0 ) then
c
              call duStepWaveGen3d6rc( 
     *           nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *           n1a,n1b,n2a,n2b,n3a,n3b,
     *           u,ut,unew,utnew,
     *           dx,dy,dz,dt,cc,
     *           i,j,k,n )
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
              call duStepWaveGen3d6rc( 
     *           nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *           n1a,n1b,n2a,n2b,n3a,n3b,
     *           u,ut,unew,utnew,
     *           dx,dy,dz,dt,cc,
     *           i,j,k,n )
c     
            end do
            end do
            end do
          end if
        else
          ! stencil optimized routines
          call getcuu_sixth3D( cc,dx,dy,dz,dt,cuu )
          call getcuv_sixth3D( cc,dx,dy,dz,dt,cuv )
          call getcvu_sixth3D( cc,dx,dy,dz,dt,cvu )
          call getcvv_sixth3D( cc,dx,dy,dz,dt,cvv )
          if( useWhereMask.ne.0 ) then
            do k = n3a,n3b
            do j = n2a,n2b
            do i = n1a,n1b
            if( mask(i,j,k).gt.0 ) then
c
              unew(i,j,k)  = 0.0
              utnew(i,j,k) = 0.0
c
              do iz = -4,4
              do iy = -4+abs(iz),4-abs(iz)
              do ix = -4+abs(iy),4-abs(iy)
                unew(i,j,k) = unew(i,j,k)+
     *             cuu(ix,iy,iz)*u (i+ix,j+iy,k+iz)+
     *             cuv(ix,iy,iz)*ut(i+ix,j+iy,k+iz)
c     
                utnew(i,j,k) = utnew(i,j,k)+
     *             cvu(ix,iy,iz)*u (i+ix,j+iy,k+iz)+
     *             cvv(ix,iy,iz)*ut(i+ix,j+iy,k+iz)
              end do
              end do
              end do
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
              unew(i,j,k)  = 0.0
              utnew(i,j,k) = 0.0
c
              do iz = -4,4
              do iy = -4+abs(iz),4-abs(iz)
              do ix = -4+abs(iy),4-abs(iy)
                unew(i,j,k) = unew(i,j,k)+
     *             cuu(ix,iy,iz)*u (i+ix,j+iy,k+iz)+
     *             cuv(ix,iy,iz)*ut(i+ix,j+iy,k+iz)
c     
                utnew(i,j,k) = utnew(i,j,k)+
     *             cvu(ix,iy,iz)*u (i+ix,j+iy,k+iz)+
     *             cvv(ix,iy,iz)*ut(i+ix,j+iy,k+iz)
              end do
              end do
              end do
c     
            end do
            end do
            end do
          end if
        end if

      else
        ! add forcing flag is set to true

        if( useWhereMask.ne.0 ) then

          do k = n3a,n3b
          do j = n2a,n2b
          do i = n1a,n1b
          if( mask(i,j,k).gt.0 ) then
c
            call duStepWaveGen3d6rc_tz( 
     *         nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *         n1a,n1b,n2a,n2b,n3a,n3b,
     *         ndf4a,ndf4b,nComp,
     *         u,ut,unew,utnew,
     *         src,
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
            call duStepWaveGen3d6rc_tz( 
     *         nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *         n1a,n1b,n2a,n2b,n3a,n3b,
     *         ndf4a,ndf4b,nComp,
     *         u,ut,unew,utnew,
     *         src,
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
c
c++++++++++++++++
c
      subroutine getcuu_sixth3D( 
     *   cc,dx,dy,dz,dt,cuu )
c
      implicit real (t)
      real cuu(1:*)
      real dx,dy,dz,dt,cc
      integer i
c
      do i = 1,9**3
        cuu(i) = 0.0
      end do
c
      cuu(41) = (0.87E2 * cc ** 7 * dt ** 7 * dy ** 7 * dz ** 7 - 0.1200
     #E4 * cc ** 5 * dt ** 5 * dy ** 7 * dz ** 7 * dx ** 2 + 0.4200E4 * 
     #cc ** 3 * dt ** 3 * dy ** 7 * dz ** 7 * dx ** 4) / dx ** 7 / dy **
     # 7 / dz ** 7 / 0.864000E6
      cuu(113) = (0.261E3 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 7 * dx *
     #* 2 + 0.87E2 * cc ** 7 * dt ** 7 * dx * dz ** 7 * dy ** 6 - 0.1800
     #E4 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 7 * dx ** 4 - 0.600E3 * c
     #c ** 5 * dt ** 5 * dx ** 3 * dz ** 7 * dy ** 6 + 0.2400E4 * cc ** 
     #3 * dt ** 3 * dy ** 5 * dz ** 7 * dx ** 6 + 0.800E3 * cc ** 3 * dt
     # ** 3 * dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7
     # / 0.864000E6
      cuu(121) = (0.261E3 * cc ** 7 * dt ** 7 * dy ** 7 * dz ** 5 * dx *
     #* 2 + 0.87E2 * cc ** 7 * dt ** 7 * dx * dy ** 7 * dz ** 6 - 0.1800
     #E4 * cc ** 5 * dt ** 5 * dy ** 7 * dz ** 5 * dx ** 4 - 0.600E3 * c
     #c ** 5 * dt ** 5 * dx ** 3 * dy ** 7 * dz ** 6 + 0.2400E4 * cc ** 
     #3 * dt ** 3 * dy ** 7 * dz ** 5 * dx ** 6 + 0.800E3 * cc ** 3 * dt
     # ** 3 * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7
     # / 0.864000E6
      cuu(122) = (-0.522E3 * cc ** 7 * dt ** 7 * dy ** 7 * dz ** 5 * dx 
     #** 2 - 0.522E3 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 7 * dx ** 2 -
     # 0.174E3 * cc ** 7 * dt ** 7 * dx * dy ** 7 * dz ** 6 - 0.174E3 * 
     #cc ** 7 * dt ** 7 * dx * dz ** 7 * dy ** 6 - 0.696E3 * cc ** 7 * d
     #t ** 7 * dy ** 7 * dz ** 7 + 0.1200E4 * cc ** 6 * dt ** 6 * dy ** 
     #7 * dz ** 7 * dx + 0.3600E4 * cc ** 5 * dt ** 5 * dy ** 7 * dz ** 
     #5 * dx ** 4 + 0.3600E4 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 7 * d
     #x ** 4 + 0.1200E4 * cc ** 5 * dt ** 5 * dx ** 3 * dy ** 7 * dz ** 
     #6 + 0.1200E4 * cc ** 5 * dt ** 5 * dx ** 3 * dz ** 7 * dy ** 6 + 0
     #.9600E4 * cc ** 5 * dt ** 5 * dy ** 7 * dz ** 7 * dx ** 2 - 0.6000
     #E4 * cc ** 4 * dt ** 4 * dy ** 7 * dz ** 7 * dx ** 3 - 0.4800E4 * 
     #cc ** 3 * dt ** 3 * dy ** 7 * dz ** 5 * dx ** 6 - 0.4800E4 * cc **
     # 3 * dt ** 3 * dy ** 5 * dz ** 7 * dx ** 6 - 0.1600E4 * cc ** 3 * 
     #dt ** 3 * dx ** 5 * dy ** 7 * dz ** 6 - 0.1600E4 * cc ** 3 * dt **
     # 3 * dx ** 5 * dz ** 7 * dy ** 6 - 0.33600E5 * cc ** 3 * dt ** 3 *
     # dy ** 7 * dz ** 7 * dx ** 4 + 0.4800E4 * cc ** 2 * dt ** 2 * dy *
     #* 7 * dz ** 7 * dx ** 5) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E
     #6
      cuu(123) = (0.261E3 * cc ** 7 * dt ** 7 * dy ** 7 * dz ** 5 * dx *
     #* 2 + 0.87E2 * cc ** 7 * dt ** 7 * dx * dy ** 7 * dz ** 6 - 0.1800
     #E4 * cc ** 5 * dt ** 5 * dy ** 7 * dz ** 5 * dx ** 4 - 0.600E3 * c
     #c ** 5 * dt ** 5 * dx ** 3 * dy ** 7 * dz ** 6 + 0.2400E4 * cc ** 
     #3 * dt ** 3 * dy ** 7 * dz ** 5 * dx ** 6 + 0.800E3 * cc ** 3 * dt
     # ** 3 * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7
     # / 0.864000E6
      cuu(131) = (0.261E3 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 7 * dx *
     #* 2 + 0.87E2 * cc ** 7 * dt ** 7 * dx * dz ** 7 * dy ** 6 - 0.1800
     #E4 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 7 * dx ** 4 - 0.600E3 * c
     #c ** 5 * dt ** 5 * dx ** 3 * dz ** 7 * dy ** 6 + 0.2400E4 * cc ** 
     #3 * dt ** 3 * dy ** 5 * dz ** 7 * dx ** 6 + 0.800E3 * cc ** 3 * dt
     # ** 3 * dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7
     # / 0.864000E6
      cuu(185) = (0.261E3 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 7 * dx *
     #* 4 + 0.261E3 * cc ** 7 * dt ** 7 * dx ** 3 * dz ** 7 * dy ** 4 - 
     #0.600E3 * cc ** 5 * dt ** 5 * dy ** 3 * dz ** 7 * dx ** 6 - 0.600E
     #3 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 7 * dy ** 4 - 0.600E3 * cc
     # ** 5 * dt ** 5 * dy ** 5 * dz ** 7 * dx ** 4 - 0.600E3 * cc ** 5 
     #* dt ** 5 * dx ** 3 * dz ** 7 * dy ** 6 + 0.1000E4 * cc ** 3 * dt 
     #** 3 * dy ** 5 * dz ** 7 * dx ** 6 + 0.1000E4 * cc ** 3 * dt ** 3 
     #* dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.8
     #64000E6
      cuu(193) = (0.522E3 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 5 * dx *
     #* 4 + 0.261E3 * cc ** 7 * dt ** 7 * dx ** 3 * dz ** 5 * dy ** 6 + 
     #0.261E3 * cc ** 7 * dt ** 7 * dx ** 3 * dy ** 5 * dz ** 6 - 0.1200
     #E4 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 5 * dx ** 6 - 0.600E3 * c
     #c ** 5 * dt ** 5 * dx ** 5 * dz ** 5 * dy ** 6 - 0.600E3 * cc ** 5
     # * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz
     # ** 7 / 0.864000E6
      cuu(194) = (-0.1044E4 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 5 * dx
     # ** 4 - 0.1044E4 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 7 * dx ** 4
     # - 0.522E3 * cc ** 7 * dt ** 7 * dx ** 3 * dz ** 5 * dy ** 6 - 0.5
     #22E3 * cc ** 7 * dt ** 7 * dx ** 3 * dy ** 5 * dz ** 6 - 0.1044E4 
     #* cc ** 7 * dt ** 7 * dx ** 3 * dz ** 7 * dy ** 4 - 0.1566E4 * cc 
     #** 7 * dt ** 7 * dy ** 5 * dz ** 7 * dx ** 2 - 0.522E3 * cc ** 7 *
     # dt ** 7 * dx * dz ** 7 * dy ** 6 + 0.3600E4 * cc ** 6 * dt ** 6 *
     # dy ** 5 * dz ** 7 * dx ** 3 + 0.2400E4 * cc ** 5 * dt ** 5 * dy *
     #* 5 * dz ** 5 * dx ** 6 + 0.2400E4 * cc ** 5 * dt ** 5 * dy ** 3 *
     # dz ** 7 * dx ** 6 + 0.1200E4 * cc ** 5 * dt ** 5 * dx ** 5 * dz *
     #* 5 * dy ** 6 + 0.1200E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 *
     # dz ** 6 + 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 7 * dy *
     #* 4 + 0.13200E5 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 7 * dx ** 4 
     #+ 0.6000E4 * cc ** 5 * dt ** 5 * dx ** 3 * dz ** 7 * dy ** 6 - 0.6
     #000E4 * cc ** 4 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 5 - 0.18400E
     #5 * cc ** 3 * dt ** 3 * dy ** 5 * dz ** 7 * dx ** 6 - 0.8800E4 * c
     #c ** 3 * dt ** 3 * dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 
     #7 / dz ** 7 / 0.864000E6
      cuu(195) = (0.522E3 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 5 * dx *
     #* 4 + 0.261E3 * cc ** 7 * dt ** 7 * dx ** 3 * dz ** 5 * dy ** 6 + 
     #0.261E3 * cc ** 7 * dt ** 7 * dx ** 3 * dy ** 5 * dz ** 6 - 0.1200
     #E4 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 5 * dx ** 6 - 0.600E3 * c
     #c ** 5 * dt ** 5 * dx ** 5 * dz ** 5 * dy ** 6 - 0.600E3 * cc ** 5
     # * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz
     # ** 7 / 0.864000E6
      cuu(201) = (0.261E3 * cc ** 7 * dt ** 7 * dy ** 7 * dz ** 3 * dx *
     #* 4 + 0.261E3 * cc ** 7 * dt ** 7 * dx ** 3 * dy ** 7 * dz ** 4 - 
     #0.600E3 * cc ** 5 * dt ** 5 * dy ** 7 * dz ** 3 * dx ** 6 - 0.600E
     #3 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 7 * dz ** 4 - 0.600E3 * cc
     # ** 5 * dt ** 5 * dy ** 7 * dz ** 5 * dx ** 4 - 0.600E3 * cc ** 5 
     #* dt ** 5 * dx ** 3 * dy ** 7 * dz ** 6 + 0.1000E4 * cc ** 3 * dt 
     #** 3 * dy ** 7 * dz ** 5 * dx ** 6 + 0.1000E4 * cc ** 3 * dt ** 3 
     #* dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.8
     #64000E6
      cuu(202) = (-0.1044E4 * cc ** 7 * dt ** 7 * dy ** 7 * dz ** 3 * dx
     # ** 4 - 0.1044E4 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 5 * dx ** 4
     # - 0.1044E4 * cc ** 7 * dt ** 7 * dx ** 3 * dy ** 7 * dz ** 4 - 0.
     #522E3 * cc ** 7 * dt ** 7 * dx ** 3 * dz ** 5 * dy ** 6 - 0.522E3 
     #* cc ** 7 * dt ** 7 * dx ** 3 * dy ** 5 * dz ** 6 - 0.1566E4 * cc 
     #** 7 * dt ** 7 * dy ** 7 * dz ** 5 * dx ** 2 - 0.522E3 * cc ** 7 *
     # dt ** 7 * dx * dy ** 7 * dz ** 6 + 0.3600E4 * cc ** 6 * dt ** 6 *
     # dy ** 7 * dz ** 5 * dx ** 3 + 0.2400E4 * cc ** 5 * dt ** 5 * dy *
     #* 7 * dz ** 3 * dx ** 6 + 0.2400E4 * cc ** 5 * dt ** 5 * dy ** 5 *
     # dz ** 5 * dx ** 6 + 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy *
     #* 7 * dz ** 4 + 0.1200E4 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 5 *
     # dy ** 6 + 0.1200E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 * dz *
     #* 6 + 0.13200E5 * cc ** 5 * dt ** 5 * dy ** 7 * dz ** 5 * dx ** 4 
     #+ 0.6000E4 * cc ** 5 * dt ** 5 * dx ** 3 * dy ** 7 * dz ** 6 - 0.6
     #000E4 * cc ** 4 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 5 - 0.18400E
     #5 * cc ** 3 * dt ** 3 * dy ** 7 * dz ** 5 * dx ** 6 - 0.8800E4 * c
     #c ** 3 * dt ** 3 * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 
     #7 / dz ** 7 / 0.864000E6
      cuu(203) = (0.1566E4 * cc ** 7 * dt ** 7 * dy ** 7 * dz ** 3 * dx 
     #** 4 + 0.2088E4 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 5 * dx ** 4 
     #+ 0.1566E4 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 7 * dx ** 4 + 0.1
     #566E4 * cc ** 7 * dt ** 7 * dx ** 3 * dy ** 7 * dz ** 4 + 0.1044E4
     # * cc ** 7 * dt ** 7 * dx ** 3 * dz ** 5 * dy ** 6 + 0.1044E4 * cc
     # ** 7 * dt ** 7 * dx ** 3 * dy ** 5 * dz ** 6 + 0.1566E4 * cc ** 7
     # * dt ** 7 * dx ** 3 * dz ** 7 * dy ** 4 + 0.3132E4 * cc ** 7 * dt
     # ** 7 * dy ** 7 * dz ** 5 * dx ** 2 + 0.3132E4 * cc ** 7 * dt ** 7
     # * dy ** 5 * dz ** 7 * dx ** 2 + 0.1044E4 * cc ** 7 * dt ** 7 * dx
     # * dy ** 7 * dz ** 6 + 0.1044E4 * cc ** 7 * dt ** 7 * dx * dz ** 7
     # * dy ** 6 + 0.2436E4 * cc ** 7 * dt ** 7 * dy ** 7 * dz ** 7 - 0.
     #7200E4 * cc ** 6 * dt ** 6 * dy ** 7 * dz ** 5 * dx ** 3 - 0.7200E
     #4 * cc ** 6 * dt ** 6 * dy ** 5 * dz ** 7 * dx ** 3 - 0.7200E4 * c
     #c ** 6 * dt ** 6 * dy ** 7 * dz ** 7 * dx - 0.3600E4 * cc ** 5 * d
     #t ** 5 * dy ** 7 * dz ** 3 * dx ** 6 - 0.4800E4 * cc ** 5 * dt ** 
     #5 * dy ** 5 * dz ** 5 * dx ** 6 - 0.3600E4 * cc ** 5 * dt ** 5 * d
     #y ** 3 * dz ** 7 * dx ** 6 - 0.3600E4 * cc ** 5 * dt ** 5 * dx ** 
     #5 * dy ** 7 * dz ** 4 - 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 5 * d
     #z ** 5 * dy ** 6 - 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 
     #5 * dz ** 6 - 0.3600E4 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 7 * d
     #y ** 4 - 0.25200E5 * cc ** 5 * dt ** 5 * dy ** 7 * dz ** 5 * dx **
     # 4 - 0.25200E5 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 7 * dx ** 4 -
     # 0.10800E5 * cc ** 5 * dt ** 5 * dx ** 3 * dy ** 7 * dz ** 6 - 0.1
     #0800E5 * cc ** 5 * dt ** 5 * dx ** 3 * dz ** 7 * dy ** 6 - 0.33600
     #E5 * cc ** 5 * dt ** 5 * dy ** 7 * dz ** 7 * dx ** 2 + 0.12000E5 *
     # cc ** 4 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 5 + 0.12000E5 * cc 
     #** 4 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 5 + 0.72000E5 * cc ** 4
     # * dt ** 4 * dy ** 7 * dz ** 7 * dx ** 3 + 0.34800E5 * cc ** 3 * d
     #t ** 3 * dy ** 7 * dz ** 5 * dx ** 6 + 0.34800E5 * cc ** 3 * dt **
     # 3 * dy ** 5 * dz ** 7 * dx ** 6 + 0.15600E5 * cc ** 3 * dt ** 3 *
     # dx ** 5 * dy ** 7 * dz ** 6 + 0.15600E5 * cc ** 3 * dt ** 3 * dx 
     #** 5 * dz ** 7 * dy ** 6 + 0.117600E6 * cc ** 3 * dt ** 3 * dy ** 
     #7 * dz ** 7 * dx ** 4 - 0.64800E5 * cc ** 2 * dt ** 2 * dy ** 7 * 
     #dz ** 7 * dx ** 5) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuu(204) = (-0.1044E4 * cc ** 7 * dt ** 7 * dy ** 7 * dz ** 3 * dx
     # ** 4 - 0.1044E4 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 5 * dx ** 4
     # - 0.1044E4 * cc ** 7 * dt ** 7 * dx ** 3 * dy ** 7 * dz ** 4 - 0.
     #522E3 * cc ** 7 * dt ** 7 * dx ** 3 * dz ** 5 * dy ** 6 - 0.522E3 
     #* cc ** 7 * dt ** 7 * dx ** 3 * dy ** 5 * dz ** 6 - 0.1566E4 * cc 
     #** 7 * dt ** 7 * dy ** 7 * dz ** 5 * dx ** 2 - 0.522E3 * cc ** 7 *
     # dt ** 7 * dx * dy ** 7 * dz ** 6 + 0.3600E4 * cc ** 6 * dt ** 6 *
     # dy ** 7 * dz ** 5 * dx ** 3 + 0.2400E4 * cc ** 5 * dt ** 5 * dy *
     #* 7 * dz ** 3 * dx ** 6 + 0.2400E4 * cc ** 5 * dt ** 5 * dy ** 5 *
     # dz ** 5 * dx ** 6 + 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy *
     #* 7 * dz ** 4 + 0.1200E4 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 5 *
     # dy ** 6 + 0.1200E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 * dz *
     #* 6 + 0.13200E5 * cc ** 5 * dt ** 5 * dy ** 7 * dz ** 5 * dx ** 4 
     #+ 0.6000E4 * cc ** 5 * dt ** 5 * dx ** 3 * dy ** 7 * dz ** 6 - 0.6
     #000E4 * cc ** 4 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 5 - 0.18400E
     #5 * cc ** 3 * dt ** 3 * dy ** 7 * dz ** 5 * dx ** 6 - 0.8800E4 * c
     #c ** 3 * dt ** 3 * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 
     #7 / dz ** 7 / 0.864000E6
      cuu(205) = (0.261E3 * cc ** 7 * dt ** 7 * dy ** 7 * dz ** 3 * dx *
     #* 4 + 0.261E3 * cc ** 7 * dt ** 7 * dx ** 3 * dy ** 7 * dz ** 4 - 
     #0.600E3 * cc ** 5 * dt ** 5 * dy ** 7 * dz ** 3 * dx ** 6 - 0.600E
     #3 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 7 * dz ** 4 - 0.600E3 * cc
     # ** 5 * dt ** 5 * dy ** 7 * dz ** 5 * dx ** 4 - 0.600E3 * cc ** 5 
     #* dt ** 5 * dx ** 3 * dy ** 7 * dz ** 6 + 0.1000E4 * cc ** 3 * dt 
     #** 3 * dy ** 7 * dz ** 5 * dx ** 6 + 0.1000E4 * cc ** 3 * dt ** 3 
     #* dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.8
     #64000E6
      cuu(211) = (0.522E3 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 5 * dx *
     #* 4 + 0.261E3 * cc ** 7 * dt ** 7 * dx ** 3 * dz ** 5 * dy ** 6 + 
     #0.261E3 * cc ** 7 * dt ** 7 * dx ** 3 * dy ** 5 * dz ** 6 - 0.1200
     #E4 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 5 * dx ** 6 - 0.600E3 * c
     #c ** 5 * dt ** 5 * dx ** 5 * dz ** 5 * dy ** 6 - 0.600E3 * cc ** 5
     # * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz
     # ** 7 / 0.864000E6
      cuu(212) = (-0.1044E4 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 5 * dx
     # ** 4 - 0.1044E4 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 7 * dx ** 4
     # - 0.522E3 * cc ** 7 * dt ** 7 * dx ** 3 * dz ** 5 * dy ** 6 - 0.5
     #22E3 * cc ** 7 * dt ** 7 * dx ** 3 * dy ** 5 * dz ** 6 - 0.1044E4 
     #* cc ** 7 * dt ** 7 * dx ** 3 * dz ** 7 * dy ** 4 - 0.1566E4 * cc 
     #** 7 * dt ** 7 * dy ** 5 * dz ** 7 * dx ** 2 - 0.522E3 * cc ** 7 *
     # dt ** 7 * dx * dz ** 7 * dy ** 6 + 0.3600E4 * cc ** 6 * dt ** 6 *
     # dy ** 5 * dz ** 7 * dx ** 3 + 0.2400E4 * cc ** 5 * dt ** 5 * dy *
     #* 5 * dz ** 5 * dx ** 6 + 0.2400E4 * cc ** 5 * dt ** 5 * dy ** 3 *
     # dz ** 7 * dx ** 6 + 0.1200E4 * cc ** 5 * dt ** 5 * dx ** 5 * dz *
     #* 5 * dy ** 6 + 0.1200E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 *
     # dz ** 6 + 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 7 * dy *
     #* 4 + 0.13200E5 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 7 * dx ** 4 
     #+ 0.6000E4 * cc ** 5 * dt ** 5 * dx ** 3 * dz ** 7 * dy ** 6 - 0.6
     #000E4 * cc ** 4 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 5 - 0.18400E
     #5 * cc ** 3 * dt ** 3 * dy ** 5 * dz ** 7 * dx ** 6 - 0.8800E4 * c
     #c ** 3 * dt ** 3 * dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 
     #7 / dz ** 7 / 0.864000E6
      cuu(213) = (0.522E3 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 5 * dx *
     #* 4 + 0.261E3 * cc ** 7 * dt ** 7 * dx ** 3 * dz ** 5 * dy ** 6 + 
     #0.261E3 * cc ** 7 * dt ** 7 * dx ** 3 * dy ** 5 * dz ** 6 - 0.1200
     #E4 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 5 * dx ** 6 - 0.600E3 * c
     #c ** 5 * dt ** 5 * dx ** 5 * dz ** 5 * dy ** 6 - 0.600E3 * cc ** 5
     # * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz
     # ** 7 / 0.864000E6
      cuu(221) = (0.261E3 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 7 * dx *
     #* 4 + 0.261E3 * cc ** 7 * dt ** 7 * dx ** 3 * dz ** 7 * dy ** 4 - 
     #0.600E3 * cc ** 5 * dt ** 5 * dy ** 3 * dz ** 7 * dx ** 6 - 0.600E
     #3 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 7 * dy ** 4 - 0.600E3 * cc
     # ** 5 * dt ** 5 * dy ** 5 * dz ** 7 * dx ** 4 - 0.600E3 * cc ** 5 
     #* dt ** 5 * dx ** 3 * dz ** 7 * dy ** 6 + 0.1000E4 * cc ** 3 * dt 
     #** 3 * dy ** 5 * dz ** 7 * dx ** 6 + 0.1000E4 * cc ** 3 * dt ** 3 
     #* dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.8
     #64000E6
      cuu(257) = (0.87E2 * cc ** 7 * dt ** 7 * dy * dz ** 7 * dx ** 6 + 
     #0.261E3 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 7 * dy ** 2 - 0.600E
     #3 * cc ** 5 * dt ** 5 * dy ** 3 * dz ** 7 * dx ** 6 - 0.1800E4 * c
     #c ** 5 * dt ** 5 * dx ** 5 * dz ** 7 * dy ** 4 + 0.800E3 * cc ** 3
     # * dt ** 3 * dy ** 5 * dz ** 7 * dx ** 6 + 0.2400E4 * cc ** 3 * dt
     # ** 3 * dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7
     # / 0.864000E6
      cuu(265) = (0.261E3 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 5 * dx *
     #* 6 + 0.522E3 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 5 * dy ** 4 + 
     #0.261E3 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 3 * dz ** 6 - 0.600E
     #3 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 5 * dx ** 6 - 0.1200E4 * c
     #c ** 5 * dt ** 5 * dx ** 5 * dz ** 5 * dy ** 6 - 0.600E3 * cc ** 5
     # * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz
     # ** 7 / 0.864000E6
      cuu(266) = (-0.522E3 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 5 * dx 
     #** 6 - 0.522E3 * cc ** 7 * dt ** 7 * dy * dz ** 7 * dx ** 6 - 0.10
     #44E4 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 5 * dy ** 4 - 0.522E3 *
     # cc ** 7 * dt ** 7 * dx ** 5 * dy ** 3 * dz ** 6 - 0.1566E4 * cc *
     #* 7 * dt ** 7 * dx ** 5 * dz ** 7 * dy ** 2 - 0.1044E4 * cc ** 7 *
     # dt ** 7 * dy ** 3 * dz ** 7 * dx ** 4 - 0.1044E4 * cc ** 7 * dt *
     #* 7 * dx ** 3 * dz ** 7 * dy ** 4 + 0.3600E4 * cc ** 6 * dt ** 6 *
     # dy ** 3 * dz ** 7 * dx ** 5 + 0.1200E4 * cc ** 5 * dt ** 5 * dy *
     #* 5 * dz ** 5 * dx ** 6 + 0.6000E4 * cc ** 5 * dt ** 5 * dy ** 3 *
     # dz ** 7 * dx ** 6 + 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 5 * dz *
     #* 5 * dy ** 6 + 0.1200E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 *
     # dz ** 6 + 0.13200E5 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 7 * dy 
     #** 4 + 0.2400E4 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 7 * dx ** 4 
     #+ 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 3 * dz ** 7 * dy ** 6 - 0.6
     #000E4 * cc ** 4 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 5 - 0.8800E4
     # * cc ** 3 * dt ** 3 * dy ** 5 * dz ** 7 * dx ** 6 - 0.18400E5 * c
     #c ** 3 * dt ** 3 * dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 
     #7 / dz ** 7 / 0.864000E6
      cuu(267) = (0.261E3 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 5 * dx *
     #* 6 + 0.522E3 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 5 * dy ** 4 + 
     #0.261E3 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 3 * dz ** 6 - 0.600E
     #3 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 5 * dx ** 6 - 0.1200E4 * c
     #c ** 5 * dt ** 5 * dx ** 5 * dz ** 5 * dy ** 6 - 0.600E3 * cc ** 5
     # * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz
     # ** 7 / 0.864000E6
      cuu(273) = (0.261E3 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 3 * dx *
     #* 6 + 0.261E3 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 3 * dy ** 6 + 
     #0.522E3 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 5 * dz ** 4 - 0.600E
     #3 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 5 * dx ** 6 - 0.600E3 * cc
     # ** 5 * dt ** 5 * dx ** 5 * dz ** 5 * dy ** 6 - 0.1200E4 * cc ** 5
     # * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz
     # ** 7 / 0.864000E6
      cuu(274) = (-0.1044E4 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 3 * dx
     # ** 6 - 0.1044E4 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 5 * dx ** 6
     # - 0.1044E4 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 3 * dy ** 6 - 0.
     #2088E4 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 5 * dz ** 4 - 0.2088E
     #4 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 5 * dy ** 4 - 0.1044E4 * c
     #c ** 7 * dt ** 7 * dx ** 5 * dy ** 3 * dz ** 6 - 0.2088E4 * cc ** 
     #7 * dt ** 7 * dy ** 5 * dz ** 5 * dx ** 4 - 0.1044E4 * cc ** 7 * d
     #t ** 7 * dx ** 3 * dz ** 5 * dy ** 6 - 0.1044E4 * cc ** 7 * dt ** 
     #7 * dx ** 3 * dy ** 5 * dz ** 6 + 0.7200E4 * cc ** 6 * dt ** 6 * d
     #y ** 5 * dz ** 5 * dx ** 5 + 0.9600E4 * cc ** 5 * dt ** 5 * dy ** 
     #5 * dz ** 5 * dx ** 6 + 0.9600E4 * cc ** 5 * dt ** 5 * dx ** 5 * d
     #z ** 5 * dy ** 6 + 0.9600E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 
     #5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuu(275) = (0.1566E4 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 3 * dx 
     #** 6 + 0.2088E4 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 5 * dx ** 6 
     #+ 0.1305E4 * cc ** 7 * dt ** 7 * dy * dz ** 7 * dx ** 6 + 0.1566E4
     # * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 3 * dy ** 6 + 0.3132E4 * cc
     # ** 7 * dt ** 7 * dx ** 5 * dy ** 5 * dz ** 4 + 0.4176E4 * cc ** 7
     # * dt ** 7 * dx ** 5 * dz ** 5 * dy ** 4 + 0.2088E4 * cc ** 7 * dt
     # ** 7 * dx ** 5 * dy ** 3 * dz ** 6 + 0.3915E4 * cc ** 7 * dt ** 7
     # * dx ** 5 * dz ** 7 * dy ** 2 + 0.4176E4 * cc ** 7 * dt ** 7 * dy
     # ** 5 * dz ** 5 * dx ** 4 + 0.4176E4 * cc ** 7 * dt ** 7 * dy ** 3
     # * dz ** 7 * dx ** 4 + 0.2088E4 * cc ** 7 * dt ** 7 * dx ** 3 * dz
     # ** 5 * dy ** 6 + 0.2088E4 * cc ** 7 * dt ** 7 * dx ** 3 * dy ** 5
     # * dz ** 6 + 0.4176E4 * cc ** 7 * dt ** 7 * dx ** 3 * dz ** 7 * dy
     # ** 4 + 0.3915E4 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 7 * dx ** 2
     # + 0.1305E4 * cc ** 7 * dt ** 7 * dx * dz ** 7 * dy ** 6 - 0.14400
     #E5 * cc ** 6 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 5 - 0.14400E5 *
     # cc ** 6 * dt ** 6 * dy ** 3 * dz ** 7 * dx ** 5 - 0.14400E5 * cc 
     #** 6 * dt ** 6 * dy ** 5 * dz ** 7 * dx ** 3 - 0.18000E5 * cc ** 5
     # * dt ** 5 * dy ** 5 * dz ** 5 * dx ** 6 - 0.18600E5 * cc ** 5 * d
     #t ** 5 * dy ** 3 * dz ** 7 * dx ** 6 - 0.18000E5 * cc ** 5 * dt **
     # 5 * dx ** 5 * dz ** 5 * dy ** 6 - 0.16800E5 * cc ** 5 * dt ** 5 *
     # dx ** 5 * dy ** 5 * dz ** 6 - 0.36600E5 * cc ** 5 * dt ** 5 * dx 
     #** 5 * dz ** 7 * dy ** 4 - 0.36600E5 * cc ** 5 * dt ** 5 * dy ** 5
     # * dz ** 7 * dx ** 4 - 0.18600E5 * cc ** 5 * dt ** 5 * dx ** 3 * d
     #z ** 7 * dy ** 6 + 0.120000E6 * cc ** 4 * dt ** 4 * dy ** 5 * dz *
     #* 7 * dx ** 5 + 0.64000E5 * cc ** 3 * dt ** 3 * dy ** 5 * dz ** 7 
     #* dx ** 6 + 0.64000E5 * cc ** 3 * dt ** 3 * dx ** 5 * dz ** 7 * dy
     # ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuu(276) = (-0.1044E4 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 3 * dx
     # ** 6 - 0.1044E4 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 5 * dx ** 6
     # - 0.1044E4 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 3 * dy ** 6 - 0.
     #2088E4 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 5 * dz ** 4 - 0.2088E
     #4 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 5 * dy ** 4 - 0.1044E4 * c
     #c ** 7 * dt ** 7 * dx ** 5 * dy ** 3 * dz ** 6 - 0.2088E4 * cc ** 
     #7 * dt ** 7 * dy ** 5 * dz ** 5 * dx ** 4 - 0.1044E4 * cc ** 7 * d
     #t ** 7 * dx ** 3 * dz ** 5 * dy ** 6 - 0.1044E4 * cc ** 7 * dt ** 
     #7 * dx ** 3 * dy ** 5 * dz ** 6 + 0.7200E4 * cc ** 6 * dt ** 6 * d
     #y ** 5 * dz ** 5 * dx ** 5 + 0.9600E4 * cc ** 5 * dt ** 5 * dy ** 
     #5 * dz ** 5 * dx ** 6 + 0.9600E4 * cc ** 5 * dt ** 5 * dx ** 5 * d
     #z ** 5 * dy ** 6 + 0.9600E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 
     #5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuu(277) = (0.261E3 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 3 * dx *
     #* 6 + 0.261E3 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 3 * dy ** 6 + 
     #0.522E3 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 5 * dz ** 4 - 0.600E
     #3 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 5 * dx ** 6 - 0.600E3 * cc
     # ** 5 * dt ** 5 * dx ** 5 * dz ** 5 * dy ** 6 - 0.1200E4 * cc ** 5
     # * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz
     # ** 7 / 0.864000E6
      cuu(281) = (0.87E2 * cc ** 7 * dt ** 7 * dy ** 7 * dz * dx ** 6 + 
     #0.261E3 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 7 * dz ** 2 - 0.600E
     #3 * cc ** 5 * dt ** 5 * dy ** 7 * dz ** 3 * dx ** 6 - 0.1800E4 * c
     #c ** 5 * dt ** 5 * dx ** 5 * dy ** 7 * dz ** 4 + 0.800E3 * cc ** 3
     # * dt ** 3 * dy ** 7 * dz ** 5 * dx ** 6 + 0.2400E4 * cc ** 3 * dt
     # ** 3 * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7
     # / 0.864000E6
      cuu(282) = (-0.522E3 * cc ** 7 * dt ** 7 * dy ** 7 * dz * dx ** 6 
     #- 0.522E3 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 3 * dx ** 6 - 0.15
     #66E4 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 7 * dz ** 2 - 0.522E3 *
     # cc ** 7 * dt ** 7 * dx ** 5 * dz ** 3 * dy ** 6 - 0.1044E4 * cc *
     #* 7 * dt ** 7 * dx ** 5 * dy ** 5 * dz ** 4 - 0.1044E4 * cc ** 7 *
     # dt ** 7 * dy ** 7 * dz ** 3 * dx ** 4 - 0.1044E4 * cc ** 7 * dt *
     #* 7 * dx ** 3 * dy ** 7 * dz ** 4 + 0.3600E4 * cc ** 6 * dt ** 6 *
     # dy ** 7 * dz ** 3 * dx ** 5 + 0.6000E4 * cc ** 5 * dt ** 5 * dy *
     #* 7 * dz ** 3 * dx ** 6 + 0.1200E4 * cc ** 5 * dt ** 5 * dy ** 5 *
     # dz ** 5 * dx ** 6 + 0.13200E5 * cc ** 5 * dt ** 5 * dx ** 5 * dy 
     #** 7 * dz ** 4 + 0.1200E4 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 5 
     #* dy ** 6 + 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 * dz 
     #** 6 + 0.2400E4 * cc ** 5 * dt ** 5 * dy ** 7 * dz ** 5 * dx ** 4 
     #+ 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 3 * dy ** 7 * dz ** 6 - 0.6
     #000E4 * cc ** 4 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 5 - 0.8800E4
     # * cc ** 3 * dt ** 3 * dy ** 7 * dz ** 5 * dx ** 6 - 0.18400E5 * c
     #c ** 3 * dt ** 3 * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 
     #7 / dz ** 7 / 0.864000E6
      cuu(283) = (0.1305E4 * cc ** 7 * dt ** 7 * dy ** 7 * dz * dx ** 6 
     #+ 0.2088E4 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 3 * dx ** 6 + 0.1
     #566E4 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 5 * dx ** 6 + 0.3915E4
     # * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 7 * dz ** 2 + 0.2088E4 * cc
     # ** 7 * dt ** 7 * dx ** 5 * dz ** 3 * dy ** 6 + 0.4176E4 * cc ** 7
     # * dt ** 7 * dx ** 5 * dy ** 5 * dz ** 4 + 0.3132E4 * cc ** 7 * dt
     # ** 7 * dx ** 5 * dz ** 5 * dy ** 4 + 0.1566E4 * cc ** 7 * dt ** 7
     # * dx ** 5 * dy ** 3 * dz ** 6 + 0.4176E4 * cc ** 7 * dt ** 7 * dy
     # ** 7 * dz ** 3 * dx ** 4 + 0.4176E4 * cc ** 7 * dt ** 7 * dy ** 5
     # * dz ** 5 * dx ** 4 + 0.4176E4 * cc ** 7 * dt ** 7 * dx ** 3 * dy
     # ** 7 * dz ** 4 + 0.2088E4 * cc ** 7 * dt ** 7 * dx ** 3 * dz ** 5
     # * dy ** 6 + 0.2088E4 * cc ** 7 * dt ** 7 * dx ** 3 * dy ** 5 * dz
     # ** 6 + 0.3915E4 * cc ** 7 * dt ** 7 * dy ** 7 * dz ** 5 * dx ** 2
     # + 0.1305E4 * cc ** 7 * dt ** 7 * dx * dy ** 7 * dz ** 6 - 0.14400
     #E5 * cc ** 6 * dt ** 6 * dy ** 7 * dz ** 3 * dx ** 5 - 0.14400E5 *
     # cc ** 6 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 5 - 0.14400E5 * cc 
     #** 6 * dt ** 6 * dy ** 7 * dz ** 5 * dx ** 3 - 0.18600E5 * cc ** 5
     # * dt ** 5 * dy ** 7 * dz ** 3 * dx ** 6 - 0.18000E5 * cc ** 5 * d
     #t ** 5 * dy ** 5 * dz ** 5 * dx ** 6 - 0.36600E5 * cc ** 5 * dt **
     # 5 * dx ** 5 * dy ** 7 * dz ** 4 - 0.16800E5 * cc ** 5 * dt ** 5 *
     # dx ** 5 * dz ** 5 * dy ** 6 - 0.18000E5 * cc ** 5 * dt ** 5 * dx 
     #** 5 * dy ** 5 * dz ** 6 - 0.36600E5 * cc ** 5 * dt ** 5 * dy ** 7
     # * dz ** 5 * dx ** 4 - 0.18600E5 * cc ** 5 * dt ** 5 * dx ** 3 * d
     #y ** 7 * dz ** 6 + 0.120000E6 * cc ** 4 * dt ** 4 * dy ** 7 * dz *
     #* 5 * dx ** 5 + 0.64000E5 * cc ** 3 * dt ** 3 * dy ** 7 * dz ** 5 
     #* dx ** 6 + 0.64000E5 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 7 * dz
     # ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuu(284) = (-0.1740E4 * cc ** 7 * dt ** 7 * dy ** 7 * dz * dx ** 6
     # - 0.3132E4 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 3 * dx ** 6 - 0.
     #3132E4 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 5 * dx ** 6 - 0.1740E
     #4 * cc ** 7 * dt ** 7 * dy * dz ** 7 * dx ** 6 - 0.5220E4 * cc ** 
     #7 * dt ** 7 * dx ** 5 * dy ** 7 * dz ** 2 - 0.3132E4 * cc ** 7 * d
     #t ** 7 * dx ** 5 * dz ** 3 * dy ** 6 - 0.6264E4 * cc ** 7 * dt ** 
     #7 * dx ** 5 * dy ** 5 * dz ** 4 - 0.6264E4 * cc ** 7 * dt ** 7 * d
     #x ** 5 * dz ** 5 * dy ** 4 - 0.3132E4 * cc ** 7 * dt ** 7 * dx ** 
     #5 * dy ** 3 * dz ** 6 - 0.5220E4 * cc ** 7 * dt ** 7 * dx ** 5 * d
     #z ** 7 * dy ** 2 - 0.6264E4 * cc ** 7 * dt ** 7 * dy ** 7 * dz ** 
     #3 * dx ** 4 - 0.8352E4 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 5 * d
     #x ** 4 - 0.6264E4 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 7 * dx ** 
     #4 - 0.6264E4 * cc ** 7 * dt ** 7 * dx ** 3 * dy ** 7 * dz ** 4 - 0
     #.4176E4 * cc ** 7 * dt ** 7 * dx ** 3 * dz ** 5 * dy ** 6 - 0.4176
     #E4 * cc ** 7 * dt ** 7 * dx ** 3 * dy ** 5 * dz ** 6 - 0.6264E4 * 
     #cc ** 7 * dt ** 7 * dx ** 3 * dz ** 7 * dy ** 4 - 0.7830E4 * cc **
     # 7 * dt ** 7 * dy ** 7 * dz ** 5 * dx ** 2 - 0.7830E4 * cc ** 7 * 
     #dt ** 7 * dy ** 5 * dz ** 7 * dx ** 2 - 0.2610E4 * cc ** 7 * dt **
     # 7 * dx * dy ** 7 * dz ** 6 - 0.2610E4 * cc ** 7 * dt ** 7 * dx * 
     #dz ** 7 * dy ** 6 - 0.4872E4 * cc ** 7 * dt ** 7 * dy ** 7 * dz **
     # 7 + 0.21600E5 * cc ** 6 * dt ** 6 * dy ** 7 * dz ** 3 * dx ** 5 +
     # 0.28800E5 * cc ** 6 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 5 + 0.2
     #1600E5 * cc ** 6 * dt ** 6 * dy ** 3 * dz ** 7 * dx ** 5 + 0.28800
     #E5 * cc ** 6 * dt ** 6 * dy ** 7 * dz ** 5 * dx ** 3 + 0.28800E5 *
     # cc ** 6 * dt ** 6 * dy ** 5 * dz ** 7 * dx ** 3 + 0.18000E5 * cc 
     #** 6 * dt ** 6 * dy ** 7 * dz ** 7 * dx + 0.26400E5 * cc ** 5 * dt
     # ** 5 * dy ** 7 * dz ** 3 * dx ** 6 + 0.33600E5 * cc ** 5 * dt ** 
     #5 * dy ** 5 * dz ** 5 * dx ** 6 + 0.26400E5 * cc ** 5 * dt ** 5 * 
     #dy ** 3 * dz ** 7 * dx ** 6 + 0.50400E5 * cc ** 5 * dt ** 5 * dx *
     #* 5 * dy ** 7 * dz ** 4 + 0.31200E5 * cc ** 5 * dt ** 5 * dx ** 5 
     #* dz ** 5 * dy ** 6 + 0.31200E5 * cc ** 5 * dt ** 5 * dx ** 5 * dy
     # ** 5 * dz ** 6 + 0.50400E5 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 
     #7 * dy ** 4 + 0.68400E5 * cc ** 5 * dt ** 5 * dy ** 7 * dz ** 5 * 
     #dx ** 4 + 0.68400E5 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 7 * dx *
     #* 4 + 0.32400E5 * cc ** 5 * dt ** 5 * dx ** 3 * dy ** 7 * dz ** 6 
     #+ 0.32400E5 * cc ** 5 * dt ** 5 * dx ** 3 * dz ** 7 * dy ** 6 + 0.
     #67200E5 * cc ** 5 * dt ** 5 * dy ** 7 * dz ** 7 * dx ** 2 - 0.2280
     #00E6 * cc ** 4 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 5 - 0.228000E
     #6 * cc ** 4 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 5 - 0.234000E6 *
     # cc ** 4 * dt ** 4 * dy ** 7 * dz ** 7 * dx ** 3 - 0.112000E6 * cc
     # ** 3 * dt ** 3 * dy ** 7 * dz ** 5 * dx ** 6 - 0.112000E6 * cc **
     # 3 * dt ** 3 * dy ** 5 * dz ** 7 * dx ** 6 - 0.96000E5 * cc ** 3 *
     # dt ** 3 * dx ** 5 * dy ** 7 * dz ** 6 - 0.96000E5 * cc ** 3 * dt 
     #** 3 * dx ** 5 * dz ** 7 * dy ** 6 - 0.235200E6 * cc ** 3 * dt ** 
     #3 * dy ** 7 * dz ** 7 * dx ** 4 + 0.648000E6 * cc ** 2 * dt ** 2 *
     # dy ** 7 * dz ** 7 * dx ** 5) / dx ** 7 / dy ** 7 / dz ** 7 / 0.86
     #4000E6
      cuu(285) = (0.1305E4 * cc ** 7 * dt ** 7 * dy ** 7 * dz * dx ** 6 
     #+ 0.2088E4 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 3 * dx ** 6 + 0.1
     #566E4 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 5 * dx ** 6 + 0.3915E4
     # * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 7 * dz ** 2 + 0.2088E4 * cc
     # ** 7 * dt ** 7 * dx ** 5 * dz ** 3 * dy ** 6 + 0.4176E4 * cc ** 7
     # * dt ** 7 * dx ** 5 * dy ** 5 * dz ** 4 + 0.3132E4 * cc ** 7 * dt
     # ** 7 * dx ** 5 * dz ** 5 * dy ** 4 + 0.1566E4 * cc ** 7 * dt ** 7
     # * dx ** 5 * dy ** 3 * dz ** 6 + 0.4176E4 * cc ** 7 * dt ** 7 * dy
     # ** 7 * dz ** 3 * dx ** 4 + 0.4176E4 * cc ** 7 * dt ** 7 * dy ** 5
     # * dz ** 5 * dx ** 4 + 0.4176E4 * cc ** 7 * dt ** 7 * dx ** 3 * dy
     # ** 7 * dz ** 4 + 0.2088E4 * cc ** 7 * dt ** 7 * dx ** 3 * dz ** 5
     # * dy ** 6 + 0.2088E4 * cc ** 7 * dt ** 7 * dx ** 3 * dy ** 5 * dz
     # ** 6 + 0.3915E4 * cc ** 7 * dt ** 7 * dy ** 7 * dz ** 5 * dx ** 2
     # + 0.1305E4 * cc ** 7 * dt ** 7 * dx * dy ** 7 * dz ** 6 - 0.14400
     #E5 * cc ** 6 * dt ** 6 * dy ** 7 * dz ** 3 * dx ** 5 - 0.14400E5 *
     # cc ** 6 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 5 - 0.14400E5 * cc 
     #** 6 * dt ** 6 * dy ** 7 * dz ** 5 * dx ** 3 - 0.18600E5 * cc ** 5
     # * dt ** 5 * dy ** 7 * dz ** 3 * dx ** 6 - 0.18000E5 * cc ** 5 * d
     #t ** 5 * dy ** 5 * dz ** 5 * dx ** 6 - 0.36600E5 * cc ** 5 * dt **
     # 5 * dx ** 5 * dy ** 7 * dz ** 4 - 0.16800E5 * cc ** 5 * dt ** 5 *
     # dx ** 5 * dz ** 5 * dy ** 6 - 0.18000E5 * cc ** 5 * dt ** 5 * dx 
     #** 5 * dy ** 5 * dz ** 6 - 0.36600E5 * cc ** 5 * dt ** 5 * dy ** 7
     # * dz ** 5 * dx ** 4 - 0.18600E5 * cc ** 5 * dt ** 5 * dx ** 3 * d
     #y ** 7 * dz ** 6 + 0.120000E6 * cc ** 4 * dt ** 4 * dy ** 7 * dz *
     #* 5 * dx ** 5 + 0.64000E5 * cc ** 3 * dt ** 3 * dy ** 7 * dz ** 5 
     #* dx ** 6 + 0.64000E5 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 7 * dz
     # ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuu(286) = (-0.522E3 * cc ** 7 * dt ** 7 * dy ** 7 * dz * dx ** 6 
     #- 0.522E3 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 3 * dx ** 6 - 0.15
     #66E4 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 7 * dz ** 2 - 0.522E3 *
     # cc ** 7 * dt ** 7 * dx ** 5 * dz ** 3 * dy ** 6 - 0.1044E4 * cc *
     #* 7 * dt ** 7 * dx ** 5 * dy ** 5 * dz ** 4 - 0.1044E4 * cc ** 7 *
     # dt ** 7 * dy ** 7 * dz ** 3 * dx ** 4 - 0.1044E4 * cc ** 7 * dt *
     #* 7 * dx ** 3 * dy ** 7 * dz ** 4 + 0.3600E4 * cc ** 6 * dt ** 6 *
     # dy ** 7 * dz ** 3 * dx ** 5 + 0.6000E4 * cc ** 5 * dt ** 5 * dy *
     #* 7 * dz ** 3 * dx ** 6 + 0.1200E4 * cc ** 5 * dt ** 5 * dy ** 5 *
     # dz ** 5 * dx ** 6 + 0.13200E5 * cc ** 5 * dt ** 5 * dx ** 5 * dy 
     #** 7 * dz ** 4 + 0.1200E4 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 5 
     #* dy ** 6 + 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 * dz 
     #** 6 + 0.2400E4 * cc ** 5 * dt ** 5 * dy ** 7 * dz ** 5 * dx ** 4 
     #+ 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 3 * dy ** 7 * dz ** 6 - 0.6
     #000E4 * cc ** 4 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 5 - 0.8800E4
     # * cc ** 3 * dt ** 3 * dy ** 7 * dz ** 5 * dx ** 6 - 0.18400E5 * c
     #c ** 3 * dt ** 3 * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 
     #7 / dz ** 7 / 0.864000E6
      cuu(287) = (0.87E2 * cc ** 7 * dt ** 7 * dy ** 7 * dz * dx ** 6 + 
     #0.261E3 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 7 * dz ** 2 - 0.600E
     #3 * cc ** 5 * dt ** 5 * dy ** 7 * dz ** 3 * dx ** 6 - 0.1800E4 * c
     #c ** 5 * dt ** 5 * dx ** 5 * dy ** 7 * dz ** 4 + 0.800E3 * cc ** 3
     # * dt ** 3 * dy ** 7 * dz ** 5 * dx ** 6 + 0.2400E4 * cc ** 3 * dt
     # ** 3 * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7
     # / 0.864000E6
      cuu(291) = (0.261E3 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 3 * dx *
     #* 6 + 0.261E3 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 3 * dy ** 6 + 
     #0.522E3 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 5 * dz ** 4 - 0.600E
     #3 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 5 * dx ** 6 - 0.600E3 * cc
     # ** 5 * dt ** 5 * dx ** 5 * dz ** 5 * dy ** 6 - 0.1200E4 * cc ** 5
     # * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz
     # ** 7 / 0.864000E6
      cuu(292) = (-0.1044E4 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 3 * dx
     # ** 6 - 0.1044E4 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 5 * dx ** 6
     # - 0.1044E4 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 3 * dy ** 6 - 0.
     #2088E4 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 5 * dz ** 4 - 0.2088E
     #4 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 5 * dy ** 4 - 0.1044E4 * c
     #c ** 7 * dt ** 7 * dx ** 5 * dy ** 3 * dz ** 6 - 0.2088E4 * cc ** 
     #7 * dt ** 7 * dy ** 5 * dz ** 5 * dx ** 4 - 0.1044E4 * cc ** 7 * d
     #t ** 7 * dx ** 3 * dz ** 5 * dy ** 6 - 0.1044E4 * cc ** 7 * dt ** 
     #7 * dx ** 3 * dy ** 5 * dz ** 6 + 0.7200E4 * cc ** 6 * dt ** 6 * d
     #y ** 5 * dz ** 5 * dx ** 5 + 0.9600E4 * cc ** 5 * dt ** 5 * dy ** 
     #5 * dz ** 5 * dx ** 6 + 0.9600E4 * cc ** 5 * dt ** 5 * dx ** 5 * d
     #z ** 5 * dy ** 6 + 0.9600E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 
     #5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuu(293) = (0.1566E4 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 3 * dx 
     #** 6 + 0.2088E4 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 5 * dx ** 6 
     #+ 0.1305E4 * cc ** 7 * dt ** 7 * dy * dz ** 7 * dx ** 6 + 0.1566E4
     # * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 3 * dy ** 6 + 0.3132E4 * cc
     # ** 7 * dt ** 7 * dx ** 5 * dy ** 5 * dz ** 4 + 0.4176E4 * cc ** 7
     # * dt ** 7 * dx ** 5 * dz ** 5 * dy ** 4 + 0.2088E4 * cc ** 7 * dt
     # ** 7 * dx ** 5 * dy ** 3 * dz ** 6 + 0.3915E4 * cc ** 7 * dt ** 7
     # * dx ** 5 * dz ** 7 * dy ** 2 + 0.4176E4 * cc ** 7 * dt ** 7 * dy
     # ** 5 * dz ** 5 * dx ** 4 + 0.4176E4 * cc ** 7 * dt ** 7 * dy ** 3
     # * dz ** 7 * dx ** 4 + 0.2088E4 * cc ** 7 * dt ** 7 * dx ** 3 * dz
     # ** 5 * dy ** 6 + 0.2088E4 * cc ** 7 * dt ** 7 * dx ** 3 * dy ** 5
     # * dz ** 6 + 0.4176E4 * cc ** 7 * dt ** 7 * dx ** 3 * dz ** 7 * dy
     # ** 4 + 0.3915E4 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 7 * dx ** 2
     # + 0.1305E4 * cc ** 7 * dt ** 7 * dx * dz ** 7 * dy ** 6 - 0.14400
     #E5 * cc ** 6 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 5 - 0.14400E5 *
     # cc ** 6 * dt ** 6 * dy ** 3 * dz ** 7 * dx ** 5 - 0.14400E5 * cc 
     #** 6 * dt ** 6 * dy ** 5 * dz ** 7 * dx ** 3 - 0.18000E5 * cc ** 5
     # * dt ** 5 * dy ** 5 * dz ** 5 * dx ** 6 - 0.18600E5 * cc ** 5 * d
     #t ** 5 * dy ** 3 * dz ** 7 * dx ** 6 - 0.18000E5 * cc ** 5 * dt **
     # 5 * dx ** 5 * dz ** 5 * dy ** 6 - 0.16800E5 * cc ** 5 * dt ** 5 *
     # dx ** 5 * dy ** 5 * dz ** 6 - 0.36600E5 * cc ** 5 * dt ** 5 * dx 
     #** 5 * dz ** 7 * dy ** 4 - 0.36600E5 * cc ** 5 * dt ** 5 * dy ** 5
     # * dz ** 7 * dx ** 4 - 0.18600E5 * cc ** 5 * dt ** 5 * dx ** 3 * d
     #z ** 7 * dy ** 6 + 0.120000E6 * cc ** 4 * dt ** 4 * dy ** 5 * dz *
     #* 7 * dx ** 5 + 0.64000E5 * cc ** 3 * dt ** 3 * dy ** 5 * dz ** 7 
     #* dx ** 6 + 0.64000E5 * cc ** 3 * dt ** 3 * dx ** 5 * dz ** 7 * dy
     # ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuu(294) = (-0.1044E4 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 3 * dx
     # ** 6 - 0.1044E4 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 5 * dx ** 6
     # - 0.1044E4 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 3 * dy ** 6 - 0.
     #2088E4 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 5 * dz ** 4 - 0.2088E
     #4 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 5 * dy ** 4 - 0.1044E4 * c
     #c ** 7 * dt ** 7 * dx ** 5 * dy ** 3 * dz ** 6 - 0.2088E4 * cc ** 
     #7 * dt ** 7 * dy ** 5 * dz ** 5 * dx ** 4 - 0.1044E4 * cc ** 7 * d
     #t ** 7 * dx ** 3 * dz ** 5 * dy ** 6 - 0.1044E4 * cc ** 7 * dt ** 
     #7 * dx ** 3 * dy ** 5 * dz ** 6 + 0.7200E4 * cc ** 6 * dt ** 6 * d
     #y ** 5 * dz ** 5 * dx ** 5 + 0.9600E4 * cc ** 5 * dt ** 5 * dy ** 
     #5 * dz ** 5 * dx ** 6 + 0.9600E4 * cc ** 5 * dt ** 5 * dx ** 5 * d
     #z ** 5 * dy ** 6 + 0.9600E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 
     #5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuu(295) = (0.261E3 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 3 * dx *
     #* 6 + 0.261E3 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 3 * dy ** 6 + 
     #0.522E3 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 5 * dz ** 4 - 0.600E
     #3 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 5 * dx ** 6 - 0.600E3 * cc
     # ** 5 * dt ** 5 * dx ** 5 * dz ** 5 * dy ** 6 - 0.1200E4 * cc ** 5
     # * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz
     # ** 7 / 0.864000E6
      cuu(301) = (0.261E3 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 5 * dx *
     #* 6 + 0.522E3 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 5 * dy ** 4 + 
     #0.261E3 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 3 * dz ** 6 - 0.600E
     #3 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 5 * dx ** 6 - 0.1200E4 * c
     #c ** 5 * dt ** 5 * dx ** 5 * dz ** 5 * dy ** 6 - 0.600E3 * cc ** 5
     # * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz
     # ** 7 / 0.864000E6
      cuu(302) = (-0.522E3 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 5 * dx 
     #** 6 - 0.522E3 * cc ** 7 * dt ** 7 * dy * dz ** 7 * dx ** 6 - 0.10
     #44E4 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 5 * dy ** 4 - 0.522E3 *
     # cc ** 7 * dt ** 7 * dx ** 5 * dy ** 3 * dz ** 6 - 0.1566E4 * cc *
     #* 7 * dt ** 7 * dx ** 5 * dz ** 7 * dy ** 2 - 0.1044E4 * cc ** 7 *
     # dt ** 7 * dy ** 3 * dz ** 7 * dx ** 4 - 0.1044E4 * cc ** 7 * dt *
     #* 7 * dx ** 3 * dz ** 7 * dy ** 4 + 0.3600E4 * cc ** 6 * dt ** 6 *
     # dy ** 3 * dz ** 7 * dx ** 5 + 0.1200E4 * cc ** 5 * dt ** 5 * dy *
     #* 5 * dz ** 5 * dx ** 6 + 0.6000E4 * cc ** 5 * dt ** 5 * dy ** 3 *
     # dz ** 7 * dx ** 6 + 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 5 * dz *
     #* 5 * dy ** 6 + 0.1200E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 *
     # dz ** 6 + 0.13200E5 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 7 * dy 
     #** 4 + 0.2400E4 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 7 * dx ** 4 
     #+ 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 3 * dz ** 7 * dy ** 6 - 0.6
     #000E4 * cc ** 4 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 5 - 0.8800E4
     # * cc ** 3 * dt ** 3 * dy ** 5 * dz ** 7 * dx ** 6 - 0.18400E5 * c
     #c ** 3 * dt ** 3 * dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 
     #7 / dz ** 7 / 0.864000E6
      cuu(303) = (0.261E3 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 5 * dx *
     #* 6 + 0.522E3 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 5 * dy ** 4 + 
     #0.261E3 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 3 * dz ** 6 - 0.600E
     #3 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 5 * dx ** 6 - 0.1200E4 * c
     #c ** 5 * dt ** 5 * dx ** 5 * dz ** 5 * dy ** 6 - 0.600E3 * cc ** 5
     # * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz
     # ** 7 / 0.864000E6
      cuu(311) = (0.87E2 * cc ** 7 * dt ** 7 * dy * dz ** 7 * dx ** 6 + 
     #0.261E3 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 7 * dy ** 2 - 0.600E
     #3 * cc ** 5 * dt ** 5 * dy ** 3 * dz ** 7 * dx ** 6 - 0.1800E4 * c
     #c ** 5 * dt ** 5 * dx ** 5 * dz ** 7 * dy ** 4 + 0.800E3 * cc ** 3
     # * dt ** 3 * dy ** 5 * dz ** 7 * dx ** 6 + 0.2400E4 * cc ** 3 * dt
     # ** 3 * dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7
     # / 0.864000E6
      cuu(329) = (0.87E2 * cc ** 7 * dt ** 7 * dx ** 7 * dz ** 7 - 0.120
     #0E4 * cc ** 5 * dt ** 5 * dx ** 7 * dz ** 7 * dy ** 2 + 0.4200E4 *
     # cc ** 3 * dt ** 3 * dx ** 7 * dz ** 7 * dy ** 4) / dx ** 7 / dy *
     #* 7 / dz ** 7 / 0.864000E6
      cuu(337) = (0.261E3 * cc ** 7 * dt ** 7 * dx ** 7 * dz ** 5 * dy *
     #* 2 + 0.87E2 * cc ** 7 * dt ** 7 * dx ** 7 * dy * dz ** 6 - 0.1800
     #E4 * cc ** 5 * dt ** 5 * dx ** 7 * dz ** 5 * dy ** 4 - 0.600E3 * c
     #c ** 5 * dt ** 5 * dx ** 7 * dy ** 3 * dz ** 6 + 0.2400E4 * cc ** 
     #3 * dt ** 3 * dx ** 7 * dz ** 5 * dy ** 6 + 0.800E3 * cc ** 3 * dt
     # ** 3 * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7
     # / 0.864000E6
      cuu(338) = (-0.522E3 * cc ** 7 * dt ** 7 * dx ** 7 * dz ** 5 * dy 
     #** 2 - 0.174E3 * cc ** 7 * dt ** 7 * dx ** 7 * dy * dz ** 6 - 0.69
     #6E3 * cc ** 7 * dt ** 7 * dx ** 7 * dz ** 7 - 0.174E3 * cc ** 7 * 
     #dt ** 7 * dy * dz ** 7 * dx ** 6 - 0.522E3 * cc ** 7 * dt ** 7 * d
     #x ** 5 * dz ** 7 * dy ** 2 + 0.1200E4 * cc ** 6 * dt ** 6 * dx ** 
     #7 * dz ** 7 * dy + 0.3600E4 * cc ** 5 * dt ** 5 * dx ** 7 * dz ** 
     #5 * dy ** 4 + 0.1200E4 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 3 * d
     #z ** 6 + 0.9600E4 * cc ** 5 * dt ** 5 * dx ** 7 * dz ** 7 * dy ** 
     #2 + 0.1200E4 * cc ** 5 * dt ** 5 * dy ** 3 * dz ** 7 * dx ** 6 + 0
     #.3600E4 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 7 * dy ** 4 - 0.6000
     #E4 * cc ** 4 * dt ** 4 * dx ** 7 * dz ** 7 * dy ** 3 - 0.4800E4 * 
     #cc ** 3 * dt ** 3 * dx ** 7 * dz ** 5 * dy ** 6 - 0.1600E4 * cc **
     # 3 * dt ** 3 * dx ** 7 * dy ** 5 * dz ** 6 - 0.33600E5 * cc ** 3 *
     # dt ** 3 * dx ** 7 * dz ** 7 * dy ** 4 - 0.1600E4 * cc ** 3 * dt *
     #* 3 * dy ** 5 * dz ** 7 * dx ** 6 - 0.4800E4 * cc ** 3 * dt ** 3 *
     # dx ** 5 * dz ** 7 * dy ** 6 + 0.4800E4 * cc ** 2 * dt ** 2 * dx *
     #* 7 * dz ** 7 * dy ** 5) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E
     #6
      cuu(339) = (0.261E3 * cc ** 7 * dt ** 7 * dx ** 7 * dz ** 5 * dy *
     #* 2 + 0.87E2 * cc ** 7 * dt ** 7 * dx ** 7 * dy * dz ** 6 - 0.1800
     #E4 * cc ** 5 * dt ** 5 * dx ** 7 * dz ** 5 * dy ** 4 - 0.600E3 * c
     #c ** 5 * dt ** 5 * dx ** 7 * dy ** 3 * dz ** 6 + 0.2400E4 * cc ** 
     #3 * dt ** 3 * dx ** 7 * dz ** 5 * dy ** 6 + 0.800E3 * cc ** 3 * dt
     # ** 3 * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7
     # / 0.864000E6
      cuu(345) = (0.261E3 * cc ** 7 * dt ** 7 * dx ** 7 * dz ** 3 * dy *
     #* 4 + 0.261E3 * cc ** 7 * dt ** 7 * dx ** 7 * dy ** 3 * dz ** 4 - 
     #0.600E3 * cc ** 5 * dt ** 5 * dx ** 7 * dz ** 3 * dy ** 6 - 0.600E
     #3 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 5 * dz ** 4 - 0.600E3 * cc
     # ** 5 * dt ** 5 * dx ** 7 * dz ** 5 * dy ** 4 - 0.600E3 * cc ** 5 
     #* dt ** 5 * dx ** 7 * dy ** 3 * dz ** 6 + 0.1000E4 * cc ** 3 * dt 
     #** 3 * dx ** 7 * dz ** 5 * dy ** 6 + 0.1000E4 * cc ** 3 * dt ** 3 
     #* dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.8
     #64000E6
      cuu(346) = (-0.1044E4 * cc ** 7 * dt ** 7 * dx ** 7 * dz ** 3 * dy
     # ** 4 - 0.1044E4 * cc ** 7 * dt ** 7 * dx ** 7 * dy ** 3 * dz ** 4
     # - 0.1566E4 * cc ** 7 * dt ** 7 * dx ** 7 * dz ** 5 * dy ** 2 - 0.
     #522E3 * cc ** 7 * dt ** 7 * dx ** 7 * dy * dz ** 6 - 0.522E3 * cc 
     #** 7 * dt ** 7 * dy ** 3 * dz ** 5 * dx ** 6 - 0.1044E4 * cc ** 7 
     #* dt ** 7 * dx ** 5 * dz ** 5 * dy ** 4 - 0.522E3 * cc ** 7 * dt *
     #* 7 * dx ** 5 * dy ** 3 * dz ** 6 + 0.3600E4 * cc ** 6 * dt ** 6 *
     # dx ** 7 * dz ** 5 * dy ** 3 + 0.2400E4 * cc ** 5 * dt ** 5 * dx *
     #* 7 * dz ** 3 * dy ** 6 + 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 7 *
     # dy ** 5 * dz ** 4 + 0.13200E5 * cc ** 5 * dt ** 5 * dx ** 7 * dz 
     #** 5 * dy ** 4 + 0.6000E4 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 3 
     #* dz ** 6 + 0.1200E4 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 5 * dx 
     #** 6 + 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 5 * dy ** 6 
     #+ 0.1200E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6 - 0.6
     #000E4 * cc ** 4 * dt ** 4 * dx ** 7 * dz ** 5 * dy ** 5 - 0.18400E
     #5 * cc ** 3 * dt ** 3 * dx ** 7 * dz ** 5 * dy ** 6 - 0.8800E4 * c
     #c ** 3 * dt ** 3 * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 
     #7 / dz ** 7 / 0.864000E6
      cuu(347) = (0.1566E4 * cc ** 7 * dt ** 7 * dx ** 7 * dz ** 3 * dy 
     #** 4 + 0.1566E4 * cc ** 7 * dt ** 7 * dx ** 7 * dy ** 3 * dz ** 4 
     #+ 0.3132E4 * cc ** 7 * dt ** 7 * dx ** 7 * dz ** 5 * dy ** 2 + 0.1
     #044E4 * cc ** 7 * dt ** 7 * dx ** 7 * dy * dz ** 6 + 0.2436E4 * cc
     # ** 7 * dt ** 7 * dx ** 7 * dz ** 7 + 0.1044E4 * cc ** 7 * dt ** 7
     # * dy ** 3 * dz ** 5 * dx ** 6 + 0.1044E4 * cc ** 7 * dt ** 7 * dy
     # * dz ** 7 * dx ** 6 + 0.2088E4 * cc ** 7 * dt ** 7 * dx ** 5 * dz
     # ** 5 * dy ** 4 + 0.1044E4 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 3
     # * dz ** 6 + 0.3132E4 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 7 * dy
     # ** 2 + 0.1566E4 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 7 * dx ** 4
     # + 0.1566E4 * cc ** 7 * dt ** 7 * dx ** 3 * dz ** 7 * dy ** 4 - 0.
     #7200E4 * cc ** 6 * dt ** 6 * dx ** 7 * dz ** 5 * dy ** 3 - 0.7200E
     #4 * cc ** 6 * dt ** 6 * dx ** 7 * dz ** 7 * dy - 0.7200E4 * cc ** 
     #6 * dt ** 6 * dy ** 3 * dz ** 7 * dx ** 5 - 0.3600E4 * cc ** 5 * d
     #t ** 5 * dx ** 7 * dz ** 3 * dy ** 6 - 0.3600E4 * cc ** 5 * dt ** 
     #5 * dx ** 7 * dy ** 5 * dz ** 4 - 0.25200E5 * cc ** 5 * dt ** 5 * 
     #dx ** 7 * dz ** 5 * dy ** 4 - 0.10800E5 * cc ** 5 * dt ** 5 * dx *
     #* 7 * dy ** 3 * dz ** 6 - 0.33600E5 * cc ** 5 * dt ** 5 * dx ** 7 
     #* dz ** 7 * dy ** 2 - 0.2400E4 * cc ** 5 * dt ** 5 * dy ** 5 * dz 
     #** 5 * dx ** 6 - 0.10800E5 * cc ** 5 * dt ** 5 * dy ** 3 * dz ** 7
     # * dx ** 6 - 0.4800E4 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 5 * dy
     # ** 6 - 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6
     # - 0.25200E5 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 7 * dy ** 4 - 0
     #.3600E4 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 7 * dx ** 4 - 0.3600
     #E4 * cc ** 5 * dt ** 5 * dx ** 3 * dz ** 7 * dy ** 6 + 0.12000E5 *
     # cc ** 4 * dt ** 4 * dx ** 7 * dz ** 5 * dy ** 5 + 0.72000E5 * cc 
     #** 4 * dt ** 4 * dx ** 7 * dz ** 7 * dy ** 3 + 0.12000E5 * cc ** 4
     # * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 5 + 0.34800E5 * cc ** 3 * d
     #t ** 3 * dx ** 7 * dz ** 5 * dy ** 6 + 0.15600E5 * cc ** 3 * dt **
     # 3 * dx ** 7 * dy ** 5 * dz ** 6 + 0.117600E6 * cc ** 3 * dt ** 3 
     #* dx ** 7 * dz ** 7 * dy ** 4 + 0.15600E5 * cc ** 3 * dt ** 3 * dy
     # ** 5 * dz ** 7 * dx ** 6 + 0.34800E5 * cc ** 3 * dt ** 3 * dx ** 
     #5 * dz ** 7 * dy ** 6 - 0.64800E5 * cc ** 2 * dt ** 2 * dx ** 7 * 
     #dz ** 7 * dy ** 5) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuu(348) = (-0.1044E4 * cc ** 7 * dt ** 7 * dx ** 7 * dz ** 3 * dy
     # ** 4 - 0.1044E4 * cc ** 7 * dt ** 7 * dx ** 7 * dy ** 3 * dz ** 4
     # - 0.1566E4 * cc ** 7 * dt ** 7 * dx ** 7 * dz ** 5 * dy ** 2 - 0.
     #522E3 * cc ** 7 * dt ** 7 * dx ** 7 * dy * dz ** 6 - 0.522E3 * cc 
     #** 7 * dt ** 7 * dy ** 3 * dz ** 5 * dx ** 6 - 0.1044E4 * cc ** 7 
     #* dt ** 7 * dx ** 5 * dz ** 5 * dy ** 4 - 0.522E3 * cc ** 7 * dt *
     #* 7 * dx ** 5 * dy ** 3 * dz ** 6 + 0.3600E4 * cc ** 6 * dt ** 6 *
     # dx ** 7 * dz ** 5 * dy ** 3 + 0.2400E4 * cc ** 5 * dt ** 5 * dx *
     #* 7 * dz ** 3 * dy ** 6 + 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 7 *
     # dy ** 5 * dz ** 4 + 0.13200E5 * cc ** 5 * dt ** 5 * dx ** 7 * dz 
     #** 5 * dy ** 4 + 0.6000E4 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 3 
     #* dz ** 6 + 0.1200E4 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 5 * dx 
     #** 6 + 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 5 * dy ** 6 
     #+ 0.1200E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6 - 0.6
     #000E4 * cc ** 4 * dt ** 4 * dx ** 7 * dz ** 5 * dy ** 5 - 0.18400E
     #5 * cc ** 3 * dt ** 3 * dx ** 7 * dz ** 5 * dy ** 6 - 0.8800E4 * c
     #c ** 3 * dt ** 3 * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 
     #7 / dz ** 7 / 0.864000E6
      cuu(349) = (0.261E3 * cc ** 7 * dt ** 7 * dx ** 7 * dz ** 3 * dy *
     #* 4 + 0.261E3 * cc ** 7 * dt ** 7 * dx ** 7 * dy ** 3 * dz ** 4 - 
     #0.600E3 * cc ** 5 * dt ** 5 * dx ** 7 * dz ** 3 * dy ** 6 - 0.600E
     #3 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 5 * dz ** 4 - 0.600E3 * cc
     # ** 5 * dt ** 5 * dx ** 7 * dz ** 5 * dy ** 4 - 0.600E3 * cc ** 5 
     #* dt ** 5 * dx ** 7 * dy ** 3 * dz ** 6 + 0.1000E4 * cc ** 3 * dt 
     #** 3 * dx ** 7 * dz ** 5 * dy ** 6 + 0.1000E4 * cc ** 3 * dt ** 3 
     #* dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.8
     #64000E6
      cuu(353) = (0.87E2 * cc ** 7 * dt ** 7 * dx ** 7 * dz * dy ** 6 + 
     #0.261E3 * cc ** 7 * dt ** 7 * dx ** 7 * dy ** 5 * dz ** 2 - 0.600E
     #3 * cc ** 5 * dt ** 5 * dx ** 7 * dz ** 3 * dy ** 6 - 0.1800E4 * c
     #c ** 5 * dt ** 5 * dx ** 7 * dy ** 5 * dz ** 4 + 0.800E3 * cc ** 3
     # * dt ** 3 * dx ** 7 * dz ** 5 * dy ** 6 + 0.2400E4 * cc ** 3 * dt
     # ** 3 * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7
     # / 0.864000E6
      cuu(354) = (-0.522E3 * cc ** 7 * dt ** 7 * dx ** 7 * dz * dy ** 6 
     #- 0.1566E4 * cc ** 7 * dt ** 7 * dx ** 7 * dy ** 5 * dz ** 2 - 0.1
     #044E4 * cc ** 7 * dt ** 7 * dx ** 7 * dz ** 3 * dy ** 4 - 0.1044E4
     # * cc ** 7 * dt ** 7 * dx ** 7 * dy ** 3 * dz ** 4 - 0.522E3 * cc 
     #** 7 * dt ** 7 * dy ** 5 * dz ** 3 * dx ** 6 - 0.522E3 * cc ** 7 *
     # dt ** 7 * dx ** 5 * dz ** 3 * dy ** 6 - 0.1044E4 * cc ** 7 * dt *
     #* 7 * dx ** 5 * dy ** 5 * dz ** 4 + 0.3600E4 * cc ** 6 * dt ** 6 *
     # dx ** 7 * dz ** 3 * dy ** 5 + 0.6000E4 * cc ** 5 * dt ** 5 * dx *
     #* 7 * dz ** 3 * dy ** 6 + 0.13200E5 * cc ** 5 * dt ** 5 * dx ** 7 
     #* dy ** 5 * dz ** 4 + 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 7 * dz 
     #** 5 * dy ** 4 + 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 3 
     #* dz ** 6 + 0.1200E4 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 5 * dx 
     #** 6 + 0.1200E4 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 5 * dy ** 6 
     #+ 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6 - 0.6
     #000E4 * cc ** 4 * dt ** 4 * dx ** 7 * dz ** 5 * dy ** 5 - 0.8800E4
     # * cc ** 3 * dt ** 3 * dx ** 7 * dz ** 5 * dy ** 6 - 0.18400E5 * c
     #c ** 3 * dt ** 3 * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 
     #7 / dz ** 7 / 0.864000E6
      cuu(355) = (0.1305E4 * cc ** 7 * dt ** 7 * dx ** 7 * dz * dy ** 6 
     #+ 0.3915E4 * cc ** 7 * dt ** 7 * dx ** 7 * dy ** 5 * dz ** 2 + 0.4
     #176E4 * cc ** 7 * dt ** 7 * dx ** 7 * dz ** 3 * dy ** 4 + 0.4176E4
     # * cc ** 7 * dt ** 7 * dx ** 7 * dy ** 3 * dz ** 4 + 0.3915E4 * cc
     # ** 7 * dt ** 7 * dx ** 7 * dz ** 5 * dy ** 2 + 0.1305E4 * cc ** 7
     # * dt ** 7 * dx ** 7 * dy * dz ** 6 + 0.2088E4 * cc ** 7 * dt ** 7
     # * dy ** 5 * dz ** 3 * dx ** 6 + 0.2088E4 * cc ** 7 * dt ** 7 * dy
     # ** 3 * dz ** 5 * dx ** 6 + 0.2088E4 * cc ** 7 * dt ** 7 * dx ** 5
     # * dz ** 3 * dy ** 6 + 0.4176E4 * cc ** 7 * dt ** 7 * dx ** 5 * dy
     # ** 5 * dz ** 4 + 0.4176E4 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 5
     # * dy ** 4 + 0.2088E4 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 3 * dz
     # ** 6 + 0.3132E4 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 5 * dx ** 4
     # + 0.1566E4 * cc ** 7 * dt ** 7 * dx ** 3 * dz ** 5 * dy ** 6 + 0.
     #1566E4 * cc ** 7 * dt ** 7 * dx ** 3 * dy ** 5 * dz ** 6 - 0.14400
     #E5 * cc ** 6 * dt ** 6 * dx ** 7 * dz ** 3 * dy ** 5 - 0.14400E5 *
     # cc ** 6 * dt ** 6 * dx ** 7 * dz ** 5 * dy ** 3 - 0.14400E5 * cc 
     #** 6 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 5 - 0.18600E5 * cc ** 5
     # * dt ** 5 * dx ** 7 * dz ** 3 * dy ** 6 - 0.36600E5 * cc ** 5 * d
     #t ** 5 * dx ** 7 * dy ** 5 * dz ** 4 - 0.36600E5 * cc ** 5 * dt **
     # 5 * dx ** 7 * dz ** 5 * dy ** 4 - 0.18600E5 * cc ** 5 * dt ** 5 *
     # dx ** 7 * dy ** 3 * dz ** 6 - 0.16800E5 * cc ** 5 * dt ** 5 * dy 
     #** 5 * dz ** 5 * dx ** 6 - 0.18000E5 * cc ** 5 * dt ** 5 * dx ** 5
     # * dz ** 5 * dy ** 6 - 0.18000E5 * cc ** 5 * dt ** 5 * dx ** 5 * d
     #y ** 5 * dz ** 6 + 0.120000E6 * cc ** 4 * dt ** 4 * dx ** 7 * dz *
     #* 5 * dy ** 5 + 0.64000E5 * cc ** 3 * dt ** 3 * dx ** 7 * dz ** 5 
     #* dy ** 6 + 0.64000E5 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 5 * dz
     # ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuu(356) = (-0.1740E4 * cc ** 7 * dt ** 7 * dx ** 7 * dz * dy ** 6
     # - 0.5220E4 * cc ** 7 * dt ** 7 * dx ** 7 * dy ** 5 * dz ** 2 - 0.
     #6264E4 * cc ** 7 * dt ** 7 * dx ** 7 * dz ** 3 * dy ** 4 - 0.6264E
     #4 * cc ** 7 * dt ** 7 * dx ** 7 * dy ** 3 * dz ** 4 - 0.7830E4 * c
     #c ** 7 * dt ** 7 * dx ** 7 * dz ** 5 * dy ** 2 - 0.2610E4 * cc ** 
     #7 * dt ** 7 * dx ** 7 * dy * dz ** 6 - 0.4872E4 * cc ** 7 * dt ** 
     #7 * dx ** 7 * dz ** 7 - 0.3132E4 * cc ** 7 * dt ** 7 * dy ** 5 * d
     #z ** 3 * dx ** 6 - 0.4176E4 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 
     #5 * dx ** 6 - 0.2610E4 * cc ** 7 * dt ** 7 * dy * dz ** 7 * dx ** 
     #6 - 0.3132E4 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 3 * dy ** 6 - 0
     #.6264E4 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 5 * dz ** 4 - 0.8352
     #E4 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 5 * dy ** 4 - 0.4176E4 * 
     #cc ** 7 * dt ** 7 * dx ** 5 * dy ** 3 * dz ** 6 - 0.7830E4 * cc **
     # 7 * dt ** 7 * dx ** 5 * dz ** 7 * dy ** 2 - 0.6264E4 * cc ** 7 * 
     #dt ** 7 * dy ** 5 * dz ** 5 * dx ** 4 - 0.6264E4 * cc ** 7 * dt **
     # 7 * dy ** 3 * dz ** 7 * dx ** 4 - 0.3132E4 * cc ** 7 * dt ** 7 * 
     #dx ** 3 * dz ** 5 * dy ** 6 - 0.3132E4 * cc ** 7 * dt ** 7 * dx **
     # 3 * dy ** 5 * dz ** 6 - 0.6264E4 * cc ** 7 * dt ** 7 * dx ** 3 * 
     #dz ** 7 * dy ** 4 - 0.5220E4 * cc ** 7 * dt ** 7 * dy ** 5 * dz **
     # 7 * dx ** 2 - 0.1740E4 * cc ** 7 * dt ** 7 * dx * dz ** 7 * dy **
     # 6 + 0.21600E5 * cc ** 6 * dt ** 6 * dx ** 7 * dz ** 3 * dy ** 5 +
     # 0.28800E5 * cc ** 6 * dt ** 6 * dx ** 7 * dz ** 5 * dy ** 3 + 0.1
     #8000E5 * cc ** 6 * dt ** 6 * dx ** 7 * dz ** 7 * dy + 0.28800E5 * 
     #cc ** 6 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 5 + 0.28800E5 * cc *
     #* 6 * dt ** 6 * dy ** 3 * dz ** 7 * dx ** 5 + 0.21600E5 * cc ** 6 
     #* dt ** 6 * dy ** 5 * dz ** 7 * dx ** 3 + 0.26400E5 * cc ** 5 * dt
     # ** 5 * dx ** 7 * dz ** 3 * dy ** 6 + 0.50400E5 * cc ** 5 * dt ** 
     #5 * dx ** 7 * dy ** 5 * dz ** 4 + 0.68400E5 * cc ** 5 * dt ** 5 * 
     #dx ** 7 * dz ** 5 * dy ** 4 + 0.32400E5 * cc ** 5 * dt ** 5 * dx *
     #* 7 * dy ** 3 * dz ** 6 + 0.67200E5 * cc ** 5 * dt ** 5 * dx ** 7 
     #* dz ** 7 * dy ** 2 + 0.31200E5 * cc ** 5 * dt ** 5 * dy ** 5 * dz
     # ** 5 * dx ** 6 + 0.32400E5 * cc ** 5 * dt ** 5 * dy ** 3 * dz ** 
     #7 * dx ** 6 + 0.33600E5 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 5 * 
     #dy ** 6 + 0.31200E5 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 * dz *
     #* 6 + 0.68400E5 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 7 * dy ** 4 
     #+ 0.50400E5 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 7 * dx ** 4 + 0.
     #26400E5 * cc ** 5 * dt ** 5 * dx ** 3 * dz ** 7 * dy ** 6 - 0.2280
     #00E6 * cc ** 4 * dt ** 4 * dx ** 7 * dz ** 5 * dy ** 5 - 0.234000E
     #6 * cc ** 4 * dt ** 4 * dx ** 7 * dz ** 7 * dy ** 3 - 0.228000E6 *
     # cc ** 4 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 5 - 0.112000E6 * cc
     # ** 3 * dt ** 3 * dx ** 7 * dz ** 5 * dy ** 6 - 0.96000E5 * cc ** 
     #3 * dt ** 3 * dx ** 7 * dy ** 5 * dz ** 6 - 0.235200E6 * cc ** 3 *
     # dt ** 3 * dx ** 7 * dz ** 7 * dy ** 4 - 0.96000E5 * cc ** 3 * dt 
     #** 3 * dy ** 5 * dz ** 7 * dx ** 6 - 0.112000E6 * cc ** 3 * dt ** 
     #3 * dx ** 5 * dz ** 7 * dy ** 6 + 0.648000E6 * cc ** 2 * dt ** 2 *
     # dx ** 7 * dz ** 7 * dy ** 5) / dx ** 7 / dy ** 7 / dz ** 7 / 0.86
     #4000E6
      cuu(357) = (0.1305E4 * cc ** 7 * dt ** 7 * dx ** 7 * dz * dy ** 6 
     #+ 0.3915E4 * cc ** 7 * dt ** 7 * dx ** 7 * dy ** 5 * dz ** 2 + 0.4
     #176E4 * cc ** 7 * dt ** 7 * dx ** 7 * dz ** 3 * dy ** 4 + 0.4176E4
     # * cc ** 7 * dt ** 7 * dx ** 7 * dy ** 3 * dz ** 4 + 0.3915E4 * cc
     # ** 7 * dt ** 7 * dx ** 7 * dz ** 5 * dy ** 2 + 0.1305E4 * cc ** 7
     # * dt ** 7 * dx ** 7 * dy * dz ** 6 + 0.2088E4 * cc ** 7 * dt ** 7
     # * dy ** 5 * dz ** 3 * dx ** 6 + 0.2088E4 * cc ** 7 * dt ** 7 * dy
     # ** 3 * dz ** 5 * dx ** 6 + 0.2088E4 * cc ** 7 * dt ** 7 * dx ** 5
     # * dz ** 3 * dy ** 6 + 0.4176E4 * cc ** 7 * dt ** 7 * dx ** 5 * dy
     # ** 5 * dz ** 4 + 0.4176E4 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 5
     # * dy ** 4 + 0.2088E4 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 3 * dz
     # ** 6 + 0.3132E4 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 5 * dx ** 4
     # + 0.1566E4 * cc ** 7 * dt ** 7 * dx ** 3 * dz ** 5 * dy ** 6 + 0.
     #1566E4 * cc ** 7 * dt ** 7 * dx ** 3 * dy ** 5 * dz ** 6 - 0.14400
     #E5 * cc ** 6 * dt ** 6 * dx ** 7 * dz ** 3 * dy ** 5 - 0.14400E5 *
     # cc ** 6 * dt ** 6 * dx ** 7 * dz ** 5 * dy ** 3 - 0.14400E5 * cc 
     #** 6 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 5 - 0.18600E5 * cc ** 5
     # * dt ** 5 * dx ** 7 * dz ** 3 * dy ** 6 - 0.36600E5 * cc ** 5 * d
     #t ** 5 * dx ** 7 * dy ** 5 * dz ** 4 - 0.36600E5 * cc ** 5 * dt **
     # 5 * dx ** 7 * dz ** 5 * dy ** 4 - 0.18600E5 * cc ** 5 * dt ** 5 *
     # dx ** 7 * dy ** 3 * dz ** 6 - 0.16800E5 * cc ** 5 * dt ** 5 * dy 
     #** 5 * dz ** 5 * dx ** 6 - 0.18000E5 * cc ** 5 * dt ** 5 * dx ** 5
     # * dz ** 5 * dy ** 6 - 0.18000E5 * cc ** 5 * dt ** 5 * dx ** 5 * d
     #y ** 5 * dz ** 6 + 0.120000E6 * cc ** 4 * dt ** 4 * dx ** 7 * dz *
     #* 5 * dy ** 5 + 0.64000E5 * cc ** 3 * dt ** 3 * dx ** 7 * dz ** 5 
     #* dy ** 6 + 0.64000E5 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 5 * dz
     # ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuu(358) = (-0.522E3 * cc ** 7 * dt ** 7 * dx ** 7 * dz * dy ** 6 
     #- 0.1566E4 * cc ** 7 * dt ** 7 * dx ** 7 * dy ** 5 * dz ** 2 - 0.1
     #044E4 * cc ** 7 * dt ** 7 * dx ** 7 * dz ** 3 * dy ** 4 - 0.1044E4
     # * cc ** 7 * dt ** 7 * dx ** 7 * dy ** 3 * dz ** 4 - 0.522E3 * cc 
     #** 7 * dt ** 7 * dy ** 5 * dz ** 3 * dx ** 6 - 0.522E3 * cc ** 7 *
     # dt ** 7 * dx ** 5 * dz ** 3 * dy ** 6 - 0.1044E4 * cc ** 7 * dt *
     #* 7 * dx ** 5 * dy ** 5 * dz ** 4 + 0.3600E4 * cc ** 6 * dt ** 6 *
     # dx ** 7 * dz ** 3 * dy ** 5 + 0.6000E4 * cc ** 5 * dt ** 5 * dx *
     #* 7 * dz ** 3 * dy ** 6 + 0.13200E5 * cc ** 5 * dt ** 5 * dx ** 7 
     #* dy ** 5 * dz ** 4 + 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 7 * dz 
     #** 5 * dy ** 4 + 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 3 
     #* dz ** 6 + 0.1200E4 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 5 * dx 
     #** 6 + 0.1200E4 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 5 * dy ** 6 
     #+ 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6 - 0.6
     #000E4 * cc ** 4 * dt ** 4 * dx ** 7 * dz ** 5 * dy ** 5 - 0.8800E4
     # * cc ** 3 * dt ** 3 * dx ** 7 * dz ** 5 * dy ** 6 - 0.18400E5 * c
     #c ** 3 * dt ** 3 * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 
     #7 / dz ** 7 / 0.864000E6
      cuu(359) = (0.87E2 * cc ** 7 * dt ** 7 * dx ** 7 * dz * dy ** 6 + 
     #0.261E3 * cc ** 7 * dt ** 7 * dx ** 7 * dy ** 5 * dz ** 2 - 0.600E
     #3 * cc ** 5 * dt ** 5 * dx ** 7 * dz ** 3 * dy ** 6 - 0.1800E4 * c
     #c ** 5 * dt ** 5 * dx ** 7 * dy ** 5 * dz ** 4 + 0.800E3 * cc ** 3
     # * dt ** 3 * dx ** 7 * dz ** 5 * dy ** 6 + 0.2400E4 * cc ** 3 * dt
     # ** 3 * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7
     # / 0.864000E6
      cuu(361) = (0.87E2 * cc ** 7 * dt ** 7 * dx ** 7 * dy ** 7 - 0.120
     #0E4 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 7 * dz ** 2 + 0.4200E4 *
     # cc ** 3 * dt ** 3 * dx ** 7 * dy ** 7 * dz ** 4) / dx ** 7 / dy *
     #* 7 / dz ** 7 / 0.864000E6
      cuu(362) = (-0.696E3 * cc ** 7 * dt ** 7 * dx ** 7 * dy ** 7 - 0.1
     #74E3 * cc ** 7 * dt ** 7 * dx ** 7 * dz * dy ** 6 - 0.522E3 * cc *
     #* 7 * dt ** 7 * dx ** 7 * dy ** 5 * dz ** 2 - 0.174E3 * cc ** 7 * 
     #dt ** 7 * dy ** 7 * dz * dx ** 6 - 0.522E3 * cc ** 7 * dt ** 7 * d
     #x ** 5 * dy ** 7 * dz ** 2 + 0.1200E4 * cc ** 6 * dt ** 6 * dx ** 
     #7 * dy ** 7 * dz + 0.9600E4 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 
     #7 * dz ** 2 + 0.1200E4 * cc ** 5 * dt ** 5 * dx ** 7 * dz ** 3 * d
     #y ** 6 + 0.3600E4 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 5 * dz ** 
     #4 + 0.1200E4 * cc ** 5 * dt ** 5 * dy ** 7 * dz ** 3 * dx ** 6 + 0
     #.3600E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 7 * dz ** 4 - 0.6000
     #E4 * cc ** 4 * dt ** 4 * dx ** 7 * dy ** 7 * dz ** 3 - 0.33600E5 *
     # cc ** 3 * dt ** 3 * dx ** 7 * dy ** 7 * dz ** 4 - 0.1600E4 * cc *
     #* 3 * dt ** 3 * dx ** 7 * dz ** 5 * dy ** 6 - 0.4800E4 * cc ** 3 *
     # dt ** 3 * dx ** 7 * dy ** 5 * dz ** 6 - 0.1600E4 * cc ** 3 * dt *
     #* 3 * dy ** 7 * dz ** 5 * dx ** 6 - 0.4800E4 * cc ** 3 * dt ** 3 *
     # dx ** 5 * dy ** 7 * dz ** 6 + 0.4800E4 * cc ** 2 * dt ** 2 * dx *
     #* 7 * dy ** 7 * dz ** 5) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E
     #6
      cuu(363) = (0.2436E4 * cc ** 7 * dt ** 7 * dx ** 7 * dy ** 7 + 0.1
     #044E4 * cc ** 7 * dt ** 7 * dx ** 7 * dz * dy ** 6 + 0.3132E4 * cc
     # ** 7 * dt ** 7 * dx ** 7 * dy ** 5 * dz ** 2 + 0.1566E4 * cc ** 7
     # * dt ** 7 * dx ** 7 * dz ** 3 * dy ** 4 + 0.1566E4 * cc ** 7 * dt
     # ** 7 * dx ** 7 * dy ** 3 * dz ** 4 + 0.1044E4 * cc ** 7 * dt ** 7
     # * dy ** 7 * dz * dx ** 6 + 0.1044E4 * cc ** 7 * dt ** 7 * dy ** 5
     # * dz ** 3 * dx ** 6 + 0.3132E4 * cc ** 7 * dt ** 7 * dx ** 5 * dy
     # ** 7 * dz ** 2 + 0.1044E4 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 3
     # * dy ** 6 + 0.2088E4 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 5 * dz
     # ** 4 + 0.1566E4 * cc ** 7 * dt ** 7 * dy ** 7 * dz ** 3 * dx ** 4
     # + 0.1566E4 * cc ** 7 * dt ** 7 * dx ** 3 * dy ** 7 * dz ** 4 - 0.
     #7200E4 * cc ** 6 * dt ** 6 * dx ** 7 * dy ** 7 * dz - 0.7200E4 * c
     #c ** 6 * dt ** 6 * dx ** 7 * dz ** 3 * dy ** 5 - 0.7200E4 * cc ** 
     #6 * dt ** 6 * dy ** 7 * dz ** 3 * dx ** 5 - 0.33600E5 * cc ** 5 * 
     #dt ** 5 * dx ** 7 * dy ** 7 * dz ** 2 - 0.10800E5 * cc ** 5 * dt *
     #* 5 * dx ** 7 * dz ** 3 * dy ** 6 - 0.25200E5 * cc ** 5 * dt ** 5 
     #* dx ** 7 * dy ** 5 * dz ** 4 - 0.3600E4 * cc ** 5 * dt ** 5 * dx 
     #** 7 * dz ** 5 * dy ** 4 - 0.3600E4 * cc ** 5 * dt ** 5 * dx ** 7 
     #* dy ** 3 * dz ** 6 - 0.10800E5 * cc ** 5 * dt ** 5 * dy ** 7 * dz
     # ** 3 * dx ** 6 - 0.2400E4 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 5
     # * dx ** 6 - 0.25200E5 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 7 * d
     #z ** 4 - 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 5 * dy ** 
     #6 - 0.4800E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6 - 0
     #.3600E4 * cc ** 5 * dt ** 5 * dy ** 7 * dz ** 5 * dx ** 4 - 0.3600
     #E4 * cc ** 5 * dt ** 5 * dx ** 3 * dy ** 7 * dz ** 6 + 0.72000E5 *
     # cc ** 4 * dt ** 4 * dx ** 7 * dy ** 7 * dz ** 3 + 0.12000E5 * cc 
     #** 4 * dt ** 4 * dx ** 7 * dz ** 5 * dy ** 5 + 0.12000E5 * cc ** 4
     # * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 5 + 0.117600E6 * cc ** 3 * 
     #dt ** 3 * dx ** 7 * dy ** 7 * dz ** 4 + 0.15600E5 * cc ** 3 * dt *
     #* 3 * dx ** 7 * dz ** 5 * dy ** 6 + 0.34800E5 * cc ** 3 * dt ** 3 
     #* dx ** 7 * dy ** 5 * dz ** 6 + 0.15600E5 * cc ** 3 * dt ** 3 * dy
     # ** 7 * dz ** 5 * dx ** 6 + 0.34800E5 * cc ** 3 * dt ** 3 * dx ** 
     #5 * dy ** 7 * dz ** 6 - 0.64800E5 * cc ** 2 * dt ** 2 * dx ** 7 * 
     #dy ** 7 * dz ** 5) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuu(364) = (-0.4872E4 * cc ** 7 * dt ** 7 * dx ** 7 * dy ** 7 - 0.
     #2610E4 * cc ** 7 * dt ** 7 * dx ** 7 * dz * dy ** 6 - 0.7830E4 * c
     #c ** 7 * dt ** 7 * dx ** 7 * dy ** 5 * dz ** 2 - 0.6264E4 * cc ** 
     #7 * dt ** 7 * dx ** 7 * dz ** 3 * dy ** 4 - 0.6264E4 * cc ** 7 * d
     #t ** 7 * dx ** 7 * dy ** 3 * dz ** 4 - 0.5220E4 * cc ** 7 * dt ** 
     #7 * dx ** 7 * dz ** 5 * dy ** 2 - 0.1740E4 * cc ** 7 * dt ** 7 * d
     #x ** 7 * dy * dz ** 6 - 0.2610E4 * cc ** 7 * dt ** 7 * dy ** 7 * d
     #z * dx ** 6 - 0.4176E4 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 3 * d
     #x ** 6 - 0.3132E4 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 5 * dx ** 
     #6 - 0.7830E4 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 7 * dz ** 2 - 0
     #.4176E4 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 3 * dy ** 6 - 0.8352
     #E4 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 5 * dz ** 4 - 0.6264E4 * 
     #cc ** 7 * dt ** 7 * dx ** 5 * dz ** 5 * dy ** 4 - 0.3132E4 * cc **
     # 7 * dt ** 7 * dx ** 5 * dy ** 3 * dz ** 6 - 0.6264E4 * cc ** 7 * 
     #dt ** 7 * dy ** 7 * dz ** 3 * dx ** 4 - 0.6264E4 * cc ** 7 * dt **
     # 7 * dy ** 5 * dz ** 5 * dx ** 4 - 0.6264E4 * cc ** 7 * dt ** 7 * 
     #dx ** 3 * dy ** 7 * dz ** 4 - 0.3132E4 * cc ** 7 * dt ** 7 * dx **
     # 3 * dz ** 5 * dy ** 6 - 0.3132E4 * cc ** 7 * dt ** 7 * dx ** 3 * 
     #dy ** 5 * dz ** 6 - 0.5220E4 * cc ** 7 * dt ** 7 * dy ** 7 * dz **
     # 5 * dx ** 2 - 0.1740E4 * cc ** 7 * dt ** 7 * dx * dy ** 7 * dz **
     # 6 + 0.18000E5 * cc ** 6 * dt ** 6 * dx ** 7 * dy ** 7 * dz + 0.28
     #800E5 * cc ** 6 * dt ** 6 * dx ** 7 * dz ** 3 * dy ** 5 + 0.21600E
     #5 * cc ** 6 * dt ** 6 * dx ** 7 * dz ** 5 * dy ** 3 + 0.28800E5 * 
     #cc ** 6 * dt ** 6 * dy ** 7 * dz ** 3 * dx ** 5 + 0.28800E5 * cc *
     #* 6 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 5 + 0.21600E5 * cc ** 6 
     #* dt ** 6 * dy ** 7 * dz ** 5 * dx ** 3 + 0.67200E5 * cc ** 5 * dt
     # ** 5 * dx ** 7 * dy ** 7 * dz ** 2 + 0.32400E5 * cc ** 5 * dt ** 
     #5 * dx ** 7 * dz ** 3 * dy ** 6 + 0.68400E5 * cc ** 5 * dt ** 5 * 
     #dx ** 7 * dy ** 5 * dz ** 4 + 0.50400E5 * cc ** 5 * dt ** 5 * dx *
     #* 7 * dz ** 5 * dy ** 4 + 0.26400E5 * cc ** 5 * dt ** 5 * dx ** 7 
     #* dy ** 3 * dz ** 6 + 0.32400E5 * cc ** 5 * dt ** 5 * dy ** 7 * dz
     # ** 3 * dx ** 6 + 0.31200E5 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 
     #5 * dx ** 6 + 0.68400E5 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 7 * 
     #dz ** 4 + 0.31200E5 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 5 * dy *
     #* 6 + 0.33600E5 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6 
     #+ 0.50400E5 * cc ** 5 * dt ** 5 * dy ** 7 * dz ** 5 * dx ** 4 + 0.
     #26400E5 * cc ** 5 * dt ** 5 * dx ** 3 * dy ** 7 * dz ** 6 - 0.2340
     #00E6 * cc ** 4 * dt ** 4 * dx ** 7 * dy ** 7 * dz ** 3 - 0.228000E
     #6 * cc ** 4 * dt ** 4 * dx ** 7 * dz ** 5 * dy ** 5 - 0.228000E6 *
     # cc ** 4 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 5 - 0.235200E6 * cc
     # ** 3 * dt ** 3 * dx ** 7 * dy ** 7 * dz ** 4 - 0.96000E5 * cc ** 
     #3 * dt ** 3 * dx ** 7 * dz ** 5 * dy ** 6 - 0.112000E6 * cc ** 3 *
     # dt ** 3 * dx ** 7 * dy ** 5 * dz ** 6 - 0.96000E5 * cc ** 3 * dt 
     #** 3 * dy ** 7 * dz ** 5 * dx ** 6 - 0.112000E6 * cc ** 3 * dt ** 
     #3 * dx ** 5 * dy ** 7 * dz ** 6 + 0.648000E6 * cc ** 2 * dt ** 2 *
     # dx ** 7 * dy ** 7 * dz ** 5) / dx ** 7 / dy ** 7 / dz ** 7 / 0.86
     #4000E6
      cuu(365) = (0.6090E4 * cc ** 7 * dt ** 7 * dx ** 7 * dy ** 7 + 0.3
     #480E4 * cc ** 7 * dt ** 7 * dx ** 7 * dz * dy ** 6 + 0.10440E5 * c
     #c ** 7 * dt ** 7 * dx ** 7 * dy ** 5 * dz ** 2 + 0.9396E4 * cc ** 
     #7 * dt ** 7 * dx ** 7 * dz ** 3 * dy ** 4 + 0.9396E4 * cc ** 7 * d
     #t ** 7 * dx ** 7 * dy ** 3 * dz ** 4 + 0.10440E5 * cc ** 7 * dt **
     # 7 * dx ** 7 * dz ** 5 * dy ** 2 + 0.3480E4 * cc ** 7 * dt ** 7 * 
     #dx ** 7 * dy * dz ** 6 + 0.6090E4 * cc ** 7 * dt ** 7 * dx ** 7 * 
     #dz ** 7 + 0.3480E4 * cc ** 7 * dt ** 7 * dy ** 7 * dz * dx ** 6 + 
     #0.6264E4 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 3 * dx ** 6 + 0.626
     #4E4 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 5 * dx ** 6 + 0.3480E4 *
     # cc ** 7 * dt ** 7 * dy * dz ** 7 * dx ** 6 + 0.10440E5 * cc ** 7 
     #* dt ** 7 * dx ** 5 * dy ** 7 * dz ** 2 + 0.6264E4 * cc ** 7 * dt 
     #** 7 * dx ** 5 * dz ** 3 * dy ** 6 + 0.12528E5 * cc ** 7 * dt ** 7
     # * dx ** 5 * dy ** 5 * dz ** 4 + 0.12528E5 * cc ** 7 * dt ** 7 * d
     #x ** 5 * dz ** 5 * dy ** 4 + 0.6264E4 * cc ** 7 * dt ** 7 * dx ** 
     #5 * dy ** 3 * dz ** 6 + 0.10440E5 * cc ** 7 * dt ** 7 * dx ** 5 * 
     #dz ** 7 * dy ** 2 + 0.9396E4 * cc ** 7 * dt ** 7 * dy ** 7 * dz **
     # 3 * dx ** 4 + 0.12528E5 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 5 *
     # dx ** 4 + 0.9396E4 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 7 * dx *
     #* 4 + 0.9396E4 * cc ** 7 * dt ** 7 * dx ** 3 * dy ** 7 * dz ** 4 +
     # 0.6264E4 * cc ** 7 * dt ** 7 * dx ** 3 * dz ** 5 * dy ** 6 + 0.62
     #64E4 * cc ** 7 * dt ** 7 * dx ** 3 * dy ** 5 * dz ** 6 + 0.9396E4 
     #* cc ** 7 * dt ** 7 * dx ** 3 * dz ** 7 * dy ** 4 + 0.10440E5 * cc
     # ** 7 * dt ** 7 * dy ** 7 * dz ** 5 * dx ** 2 + 0.10440E5 * cc ** 
     #7 * dt ** 7 * dy ** 5 * dz ** 7 * dx ** 2 + 0.3480E4 * cc ** 7 * d
     #t ** 7 * dx * dy ** 7 * dz ** 6 + 0.3480E4 * cc ** 7 * dt ** 7 * d
     #x * dz ** 7 * dy ** 6 + 0.6090E4 * cc ** 7 * dt ** 7 * dy ** 7 * d
     #z ** 7 - 0.24000E5 * cc ** 6 * dt ** 6 * dx ** 7 * dy ** 7 * dz - 
     #0.43200E5 * cc ** 6 * dt ** 6 * dx ** 7 * dz ** 3 * dy ** 5 - 0.43
     #200E5 * cc ** 6 * dt ** 6 * dx ** 7 * dz ** 5 * dy ** 3 - 0.24000E
     #5 * cc ** 6 * dt ** 6 * dx ** 7 * dz ** 7 * dy - 0.43200E5 * cc **
     # 6 * dt ** 6 * dy ** 7 * dz ** 3 * dx ** 5 - 0.57600E5 * cc ** 6 *
     # dt ** 6 * dy ** 5 * dz ** 5 * dx ** 5 - 0.43200E5 * cc ** 6 * dt 
     #** 6 * dy ** 3 * dz ** 7 * dx ** 5 - 0.43200E5 * cc ** 6 * dt ** 6
     # * dy ** 7 * dz ** 5 * dx ** 3 - 0.43200E5 * cc ** 6 * dt ** 6 * d
     #y ** 5 * dz ** 7 * dx ** 3 - 0.24000E5 * cc ** 6 * dt ** 6 * dy **
     # 7 * dz ** 7 * dx - 0.84000E5 * cc ** 5 * dt ** 5 * dx ** 7 * dy *
     #* 7 * dz ** 2 - 0.45600E5 * cc ** 5 * dt ** 5 * dx ** 7 * dz ** 3 
     #* dy ** 6 - 0.93600E5 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 5 * dz
     # ** 4 - 0.93600E5 * cc ** 5 * dt ** 5 * dx ** 7 * dz ** 5 * dy ** 
     #4 - 0.45600E5 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 3 * dz ** 6 - 
     #0.84000E5 * cc ** 5 * dt ** 5 * dx ** 7 * dz ** 7 * dy ** 2 - 0.45
     #600E5 * cc ** 5 * dt ** 5 * dy ** 7 * dz ** 3 * dx ** 6 - 0.57600E
     #5 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 5 * dx ** 6 - 0.45600E5 * 
     #cc ** 5 * dt ** 5 * dy ** 3 * dz ** 7 * dx ** 6 - 0.93600E5 * cc *
     #* 5 * dt ** 5 * dx ** 5 * dy ** 7 * dz ** 4 - 0.57600E5 * cc ** 5 
     #* dt ** 5 * dx ** 5 * dz ** 5 * dy ** 6 - 0.57600E5 * cc ** 5 * dt
     # ** 5 * dx ** 5 * dy ** 5 * dz ** 6 - 0.93600E5 * cc ** 5 * dt ** 
     #5 * dx ** 5 * dz ** 7 * dy ** 4 - 0.93600E5 * cc ** 5 * dt ** 5 * 
     #dy ** 7 * dz ** 5 * dx ** 4 - 0.93600E5 * cc ** 5 * dt ** 5 * dy *
     #* 5 * dz ** 7 * dx ** 4 - 0.45600E5 * cc ** 5 * dt ** 5 * dx ** 3 
     #* dy ** 7 * dz ** 6 - 0.45600E5 * cc ** 5 * dt ** 5 * dx ** 3 * dz
     # ** 7 * dy ** 6 - 0.84000E5 * cc ** 5 * dt ** 5 * dy ** 7 * dz ** 
     #7 * dx ** 2 + 0.336000E6 * cc ** 4 * dt ** 4 * dx ** 7 * dy ** 7 *
     # dz ** 3 + 0.432000E6 * cc ** 4 * dt ** 4 * dx ** 7 * dz ** 5 * dy
     # ** 5 + 0.336000E6 * cc ** 4 * dt ** 4 * dx ** 7 * dz ** 7 * dy **
     # 3 + 0.432000E6 * cc ** 4 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 5 
     #+ 0.432000E6 * cc ** 4 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 5 + 0
     #.336000E6 * cc ** 4 * dt ** 4 * dy ** 7 * dz ** 7 * dx ** 3 + 0.29
     #4000E6 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 7 * dz ** 4 + 0.16400
     #0E6 * cc ** 3 * dt ** 3 * dx ** 7 * dz ** 5 * dy ** 6 + 0.164000E6
     # * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 5 * dz ** 6 + 0.294000E6 * 
     #cc ** 3 * dt ** 3 * dx ** 7 * dz ** 7 * dy ** 4 + 0.164000E6 * cc 
     #** 3 * dt ** 3 * dy ** 7 * dz ** 5 * dx ** 6 + 0.164000E6 * cc ** 
     #3 * dt ** 3 * dy ** 5 * dz ** 7 * dx ** 6 + 0.164000E6 * cc ** 3 *
     # dt ** 3 * dx ** 5 * dy ** 7 * dz ** 6 + 0.164000E6 * cc ** 3 * dt
     # ** 3 * dx ** 5 * dz ** 7 * dy ** 6 + 0.294000E6 * cc ** 3 * dt **
     # 3 * dy ** 7 * dz ** 7 * dx ** 4 - 0.1176000E7 * cc ** 2 * dt ** 2
     # * dx ** 7 * dy ** 7 * dz ** 5 - 0.1176000E7 * cc ** 2 * dt ** 2 *
     # dx ** 7 * dz ** 7 * dy ** 5 - 0.1176000E7 * cc ** 2 * dt ** 2 * d
     #y ** 7 * dz ** 7 * dx ** 5 + 0.864000E6 * dx ** 7 * dy ** 7 * dz *
     #* 7) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuu(366) = (-0.4872E4 * cc ** 7 * dt ** 7 * dx ** 7 * dy ** 7 - 0.
     #2610E4 * cc ** 7 * dt ** 7 * dx ** 7 * dz * dy ** 6 - 0.7830E4 * c
     #c ** 7 * dt ** 7 * dx ** 7 * dy ** 5 * dz ** 2 - 0.6264E4 * cc ** 
     #7 * dt ** 7 * dx ** 7 * dz ** 3 * dy ** 4 - 0.6264E4 * cc ** 7 * d
     #t ** 7 * dx ** 7 * dy ** 3 * dz ** 4 - 0.5220E4 * cc ** 7 * dt ** 
     #7 * dx ** 7 * dz ** 5 * dy ** 2 - 0.1740E4 * cc ** 7 * dt ** 7 * d
     #x ** 7 * dy * dz ** 6 - 0.2610E4 * cc ** 7 * dt ** 7 * dy ** 7 * d
     #z * dx ** 6 - 0.4176E4 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 3 * d
     #x ** 6 - 0.3132E4 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 5 * dx ** 
     #6 - 0.7830E4 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 7 * dz ** 2 - 0
     #.4176E4 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 3 * dy ** 6 - 0.8352
     #E4 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 5 * dz ** 4 - 0.6264E4 * 
     #cc ** 7 * dt ** 7 * dx ** 5 * dz ** 5 * dy ** 4 - 0.3132E4 * cc **
     # 7 * dt ** 7 * dx ** 5 * dy ** 3 * dz ** 6 - 0.6264E4 * cc ** 7 * 
     #dt ** 7 * dy ** 7 * dz ** 3 * dx ** 4 - 0.6264E4 * cc ** 7 * dt **
     # 7 * dy ** 5 * dz ** 5 * dx ** 4 - 0.6264E4 * cc ** 7 * dt ** 7 * 
     #dx ** 3 * dy ** 7 * dz ** 4 - 0.3132E4 * cc ** 7 * dt ** 7 * dx **
     # 3 * dz ** 5 * dy ** 6 - 0.3132E4 * cc ** 7 * dt ** 7 * dx ** 3 * 
     #dy ** 5 * dz ** 6 - 0.5220E4 * cc ** 7 * dt ** 7 * dy ** 7 * dz **
     # 5 * dx ** 2 - 0.1740E4 * cc ** 7 * dt ** 7 * dx * dy ** 7 * dz **
     # 6 + 0.18000E5 * cc ** 6 * dt ** 6 * dx ** 7 * dy ** 7 * dz + 0.28
     #800E5 * cc ** 6 * dt ** 6 * dx ** 7 * dz ** 3 * dy ** 5 + 0.21600E
     #5 * cc ** 6 * dt ** 6 * dx ** 7 * dz ** 5 * dy ** 3 + 0.28800E5 * 
     #cc ** 6 * dt ** 6 * dy ** 7 * dz ** 3 * dx ** 5 + 0.28800E5 * cc *
     #* 6 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 5 + 0.21600E5 * cc ** 6 
     #* dt ** 6 * dy ** 7 * dz ** 5 * dx ** 3 + 0.67200E5 * cc ** 5 * dt
     # ** 5 * dx ** 7 * dy ** 7 * dz ** 2 + 0.32400E5 * cc ** 5 * dt ** 
     #5 * dx ** 7 * dz ** 3 * dy ** 6 + 0.68400E5 * cc ** 5 * dt ** 5 * 
     #dx ** 7 * dy ** 5 * dz ** 4 + 0.50400E5 * cc ** 5 * dt ** 5 * dx *
     #* 7 * dz ** 5 * dy ** 4 + 0.26400E5 * cc ** 5 * dt ** 5 * dx ** 7 
     #* dy ** 3 * dz ** 6 + 0.32400E5 * cc ** 5 * dt ** 5 * dy ** 7 * dz
     # ** 3 * dx ** 6 + 0.31200E5 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 
     #5 * dx ** 6 + 0.68400E5 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 7 * 
     #dz ** 4 + 0.31200E5 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 5 * dy *
     #* 6 + 0.33600E5 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6 
     #+ 0.50400E5 * cc ** 5 * dt ** 5 * dy ** 7 * dz ** 5 * dx ** 4 + 0.
     #26400E5 * cc ** 5 * dt ** 5 * dx ** 3 * dy ** 7 * dz ** 6 - 0.2340
     #00E6 * cc ** 4 * dt ** 4 * dx ** 7 * dy ** 7 * dz ** 3 - 0.228000E
     #6 * cc ** 4 * dt ** 4 * dx ** 7 * dz ** 5 * dy ** 5 - 0.228000E6 *
     # cc ** 4 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 5 - 0.235200E6 * cc
     # ** 3 * dt ** 3 * dx ** 7 * dy ** 7 * dz ** 4 - 0.96000E5 * cc ** 
     #3 * dt ** 3 * dx ** 7 * dz ** 5 * dy ** 6 - 0.112000E6 * cc ** 3 *
     # dt ** 3 * dx ** 7 * dy ** 5 * dz ** 6 - 0.96000E5 * cc ** 3 * dt 
     #** 3 * dy ** 7 * dz ** 5 * dx ** 6 - 0.112000E6 * cc ** 3 * dt ** 
     #3 * dx ** 5 * dy ** 7 * dz ** 6 + 0.648000E6 * cc ** 2 * dt ** 2 *
     # dx ** 7 * dy ** 7 * dz ** 5) / dx ** 7 / dy ** 7 / dz ** 7 / 0.86
     #4000E6
      cuu(367) = (0.2436E4 * cc ** 7 * dt ** 7 * dx ** 7 * dy ** 7 + 0.1
     #044E4 * cc ** 7 * dt ** 7 * dx ** 7 * dz * dy ** 6 + 0.3132E4 * cc
     # ** 7 * dt ** 7 * dx ** 7 * dy ** 5 * dz ** 2 + 0.1566E4 * cc ** 7
     # * dt ** 7 * dx ** 7 * dz ** 3 * dy ** 4 + 0.1566E4 * cc ** 7 * dt
     # ** 7 * dx ** 7 * dy ** 3 * dz ** 4 + 0.1044E4 * cc ** 7 * dt ** 7
     # * dy ** 7 * dz * dx ** 6 + 0.1044E4 * cc ** 7 * dt ** 7 * dy ** 5
     # * dz ** 3 * dx ** 6 + 0.3132E4 * cc ** 7 * dt ** 7 * dx ** 5 * dy
     # ** 7 * dz ** 2 + 0.1044E4 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 3
     # * dy ** 6 + 0.2088E4 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 5 * dz
     # ** 4 + 0.1566E4 * cc ** 7 * dt ** 7 * dy ** 7 * dz ** 3 * dx ** 4
     # + 0.1566E4 * cc ** 7 * dt ** 7 * dx ** 3 * dy ** 7 * dz ** 4 - 0.
     #7200E4 * cc ** 6 * dt ** 6 * dx ** 7 * dy ** 7 * dz - 0.7200E4 * c
     #c ** 6 * dt ** 6 * dx ** 7 * dz ** 3 * dy ** 5 - 0.7200E4 * cc ** 
     #6 * dt ** 6 * dy ** 7 * dz ** 3 * dx ** 5 - 0.33600E5 * cc ** 5 * 
     #dt ** 5 * dx ** 7 * dy ** 7 * dz ** 2 - 0.10800E5 * cc ** 5 * dt *
     #* 5 * dx ** 7 * dz ** 3 * dy ** 6 - 0.25200E5 * cc ** 5 * dt ** 5 
     #* dx ** 7 * dy ** 5 * dz ** 4 - 0.3600E4 * cc ** 5 * dt ** 5 * dx 
     #** 7 * dz ** 5 * dy ** 4 - 0.3600E4 * cc ** 5 * dt ** 5 * dx ** 7 
     #* dy ** 3 * dz ** 6 - 0.10800E5 * cc ** 5 * dt ** 5 * dy ** 7 * dz
     # ** 3 * dx ** 6 - 0.2400E4 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 5
     # * dx ** 6 - 0.25200E5 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 7 * d
     #z ** 4 - 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 5 * dy ** 
     #6 - 0.4800E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6 - 0
     #.3600E4 * cc ** 5 * dt ** 5 * dy ** 7 * dz ** 5 * dx ** 4 - 0.3600
     #E4 * cc ** 5 * dt ** 5 * dx ** 3 * dy ** 7 * dz ** 6 + 0.72000E5 *
     # cc ** 4 * dt ** 4 * dx ** 7 * dy ** 7 * dz ** 3 + 0.12000E5 * cc 
     #** 4 * dt ** 4 * dx ** 7 * dz ** 5 * dy ** 5 + 0.12000E5 * cc ** 4
     # * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 5 + 0.117600E6 * cc ** 3 * 
     #dt ** 3 * dx ** 7 * dy ** 7 * dz ** 4 + 0.15600E5 * cc ** 3 * dt *
     #* 3 * dx ** 7 * dz ** 5 * dy ** 6 + 0.34800E5 * cc ** 3 * dt ** 3 
     #* dx ** 7 * dy ** 5 * dz ** 6 + 0.15600E5 * cc ** 3 * dt ** 3 * dy
     # ** 7 * dz ** 5 * dx ** 6 + 0.34800E5 * cc ** 3 * dt ** 3 * dx ** 
     #5 * dy ** 7 * dz ** 6 - 0.64800E5 * cc ** 2 * dt ** 2 * dx ** 7 * 
     #dy ** 7 * dz ** 5) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuu(368) = (-0.696E3 * cc ** 7 * dt ** 7 * dx ** 7 * dy ** 7 - 0.1
     #74E3 * cc ** 7 * dt ** 7 * dx ** 7 * dz * dy ** 6 - 0.522E3 * cc *
     #* 7 * dt ** 7 * dx ** 7 * dy ** 5 * dz ** 2 - 0.174E3 * cc ** 7 * 
     #dt ** 7 * dy ** 7 * dz * dx ** 6 - 0.522E3 * cc ** 7 * dt ** 7 * d
     #x ** 5 * dy ** 7 * dz ** 2 + 0.1200E4 * cc ** 6 * dt ** 6 * dx ** 
     #7 * dy ** 7 * dz + 0.9600E4 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 
     #7 * dz ** 2 + 0.1200E4 * cc ** 5 * dt ** 5 * dx ** 7 * dz ** 3 * d
     #y ** 6 + 0.3600E4 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 5 * dz ** 
     #4 + 0.1200E4 * cc ** 5 * dt ** 5 * dy ** 7 * dz ** 3 * dx ** 6 + 0
     #.3600E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 7 * dz ** 4 - 0.6000
     #E4 * cc ** 4 * dt ** 4 * dx ** 7 * dy ** 7 * dz ** 3 - 0.33600E5 *
     # cc ** 3 * dt ** 3 * dx ** 7 * dy ** 7 * dz ** 4 - 0.1600E4 * cc *
     #* 3 * dt ** 3 * dx ** 7 * dz ** 5 * dy ** 6 - 0.4800E4 * cc ** 3 *
     # dt ** 3 * dx ** 7 * dy ** 5 * dz ** 6 - 0.1600E4 * cc ** 3 * dt *
     #* 3 * dy ** 7 * dz ** 5 * dx ** 6 - 0.4800E4 * cc ** 3 * dt ** 3 *
     # dx ** 5 * dy ** 7 * dz ** 6 + 0.4800E4 * cc ** 2 * dt ** 2 * dx *
     #* 7 * dy ** 7 * dz ** 5) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E
     #6
      cuu(369) = (0.87E2 * cc ** 7 * dt ** 7 * dx ** 7 * dy ** 7 - 0.120
     #0E4 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 7 * dz ** 2 + 0.4200E4 *
     # cc ** 3 * dt ** 3 * dx ** 7 * dy ** 7 * dz ** 4) / dx ** 7 / dy *
     #* 7 / dz ** 7 / 0.864000E6
      cuu(371) = (0.87E2 * cc ** 7 * dt ** 7 * dx ** 7 * dz * dy ** 6 + 
     #0.261E3 * cc ** 7 * dt ** 7 * dx ** 7 * dy ** 5 * dz ** 2 - 0.600E
     #3 * cc ** 5 * dt ** 5 * dx ** 7 * dz ** 3 * dy ** 6 - 0.1800E4 * c
     #c ** 5 * dt ** 5 * dx ** 7 * dy ** 5 * dz ** 4 + 0.800E3 * cc ** 3
     # * dt ** 3 * dx ** 7 * dz ** 5 * dy ** 6 + 0.2400E4 * cc ** 3 * dt
     # ** 3 * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7
     # / 0.864000E6
      cuu(372) = (-0.522E3 * cc ** 7 * dt ** 7 * dx ** 7 * dz * dy ** 6 
     #- 0.1566E4 * cc ** 7 * dt ** 7 * dx ** 7 * dy ** 5 * dz ** 2 - 0.1
     #044E4 * cc ** 7 * dt ** 7 * dx ** 7 * dz ** 3 * dy ** 4 - 0.1044E4
     # * cc ** 7 * dt ** 7 * dx ** 7 * dy ** 3 * dz ** 4 - 0.522E3 * cc 
     #** 7 * dt ** 7 * dy ** 5 * dz ** 3 * dx ** 6 - 0.522E3 * cc ** 7 *
     # dt ** 7 * dx ** 5 * dz ** 3 * dy ** 6 - 0.1044E4 * cc ** 7 * dt *
     #* 7 * dx ** 5 * dy ** 5 * dz ** 4 + 0.3600E4 * cc ** 6 * dt ** 6 *
     # dx ** 7 * dz ** 3 * dy ** 5 + 0.6000E4 * cc ** 5 * dt ** 5 * dx *
     #* 7 * dz ** 3 * dy ** 6 + 0.13200E5 * cc ** 5 * dt ** 5 * dx ** 7 
     #* dy ** 5 * dz ** 4 + 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 7 * dz 
     #** 5 * dy ** 4 + 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 3 
     #* dz ** 6 + 0.1200E4 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 5 * dx 
     #** 6 + 0.1200E4 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 5 * dy ** 6 
     #+ 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6 - 0.6
     #000E4 * cc ** 4 * dt ** 4 * dx ** 7 * dz ** 5 * dy ** 5 - 0.8800E4
     # * cc ** 3 * dt ** 3 * dx ** 7 * dz ** 5 * dy ** 6 - 0.18400E5 * c
     #c ** 3 * dt ** 3 * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 
     #7 / dz ** 7 / 0.864000E6
      cuu(373) = (0.1305E4 * cc ** 7 * dt ** 7 * dx ** 7 * dz * dy ** 6 
     #+ 0.3915E4 * cc ** 7 * dt ** 7 * dx ** 7 * dy ** 5 * dz ** 2 + 0.4
     #176E4 * cc ** 7 * dt ** 7 * dx ** 7 * dz ** 3 * dy ** 4 + 0.4176E4
     # * cc ** 7 * dt ** 7 * dx ** 7 * dy ** 3 * dz ** 4 + 0.3915E4 * cc
     # ** 7 * dt ** 7 * dx ** 7 * dz ** 5 * dy ** 2 + 0.1305E4 * cc ** 7
     # * dt ** 7 * dx ** 7 * dy * dz ** 6 + 0.2088E4 * cc ** 7 * dt ** 7
     # * dy ** 5 * dz ** 3 * dx ** 6 + 0.2088E4 * cc ** 7 * dt ** 7 * dy
     # ** 3 * dz ** 5 * dx ** 6 + 0.2088E4 * cc ** 7 * dt ** 7 * dx ** 5
     # * dz ** 3 * dy ** 6 + 0.4176E4 * cc ** 7 * dt ** 7 * dx ** 5 * dy
     # ** 5 * dz ** 4 + 0.4176E4 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 5
     # * dy ** 4 + 0.2088E4 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 3 * dz
     # ** 6 + 0.3132E4 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 5 * dx ** 4
     # + 0.1566E4 * cc ** 7 * dt ** 7 * dx ** 3 * dz ** 5 * dy ** 6 + 0.
     #1566E4 * cc ** 7 * dt ** 7 * dx ** 3 * dy ** 5 * dz ** 6 - 0.14400
     #E5 * cc ** 6 * dt ** 6 * dx ** 7 * dz ** 3 * dy ** 5 - 0.14400E5 *
     # cc ** 6 * dt ** 6 * dx ** 7 * dz ** 5 * dy ** 3 - 0.14400E5 * cc 
     #** 6 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 5 - 0.18600E5 * cc ** 5
     # * dt ** 5 * dx ** 7 * dz ** 3 * dy ** 6 - 0.36600E5 * cc ** 5 * d
     #t ** 5 * dx ** 7 * dy ** 5 * dz ** 4 - 0.36600E5 * cc ** 5 * dt **
     # 5 * dx ** 7 * dz ** 5 * dy ** 4 - 0.18600E5 * cc ** 5 * dt ** 5 *
     # dx ** 7 * dy ** 3 * dz ** 6 - 0.16800E5 * cc ** 5 * dt ** 5 * dy 
     #** 5 * dz ** 5 * dx ** 6 - 0.18000E5 * cc ** 5 * dt ** 5 * dx ** 5
     # * dz ** 5 * dy ** 6 - 0.18000E5 * cc ** 5 * dt ** 5 * dx ** 5 * d
     #y ** 5 * dz ** 6 + 0.120000E6 * cc ** 4 * dt ** 4 * dx ** 7 * dz *
     #* 5 * dy ** 5 + 0.64000E5 * cc ** 3 * dt ** 3 * dx ** 7 * dz ** 5 
     #* dy ** 6 + 0.64000E5 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 5 * dz
     # ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuu(374) = (-0.1740E4 * cc ** 7 * dt ** 7 * dx ** 7 * dz * dy ** 6
     # - 0.5220E4 * cc ** 7 * dt ** 7 * dx ** 7 * dy ** 5 * dz ** 2 - 0.
     #6264E4 * cc ** 7 * dt ** 7 * dx ** 7 * dz ** 3 * dy ** 4 - 0.6264E
     #4 * cc ** 7 * dt ** 7 * dx ** 7 * dy ** 3 * dz ** 4 - 0.7830E4 * c
     #c ** 7 * dt ** 7 * dx ** 7 * dz ** 5 * dy ** 2 - 0.2610E4 * cc ** 
     #7 * dt ** 7 * dx ** 7 * dy * dz ** 6 - 0.4872E4 * cc ** 7 * dt ** 
     #7 * dx ** 7 * dz ** 7 - 0.3132E4 * cc ** 7 * dt ** 7 * dy ** 5 * d
     #z ** 3 * dx ** 6 - 0.4176E4 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 
     #5 * dx ** 6 - 0.2610E4 * cc ** 7 * dt ** 7 * dy * dz ** 7 * dx ** 
     #6 - 0.3132E4 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 3 * dy ** 6 - 0
     #.6264E4 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 5 * dz ** 4 - 0.8352
     #E4 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 5 * dy ** 4 - 0.4176E4 * 
     #cc ** 7 * dt ** 7 * dx ** 5 * dy ** 3 * dz ** 6 - 0.7830E4 * cc **
     # 7 * dt ** 7 * dx ** 5 * dz ** 7 * dy ** 2 - 0.6264E4 * cc ** 7 * 
     #dt ** 7 * dy ** 5 * dz ** 5 * dx ** 4 - 0.6264E4 * cc ** 7 * dt **
     # 7 * dy ** 3 * dz ** 7 * dx ** 4 - 0.3132E4 * cc ** 7 * dt ** 7 * 
     #dx ** 3 * dz ** 5 * dy ** 6 - 0.3132E4 * cc ** 7 * dt ** 7 * dx **
     # 3 * dy ** 5 * dz ** 6 - 0.6264E4 * cc ** 7 * dt ** 7 * dx ** 3 * 
     #dz ** 7 * dy ** 4 - 0.5220E4 * cc ** 7 * dt ** 7 * dy ** 5 * dz **
     # 7 * dx ** 2 - 0.1740E4 * cc ** 7 * dt ** 7 * dx * dz ** 7 * dy **
     # 6 + 0.21600E5 * cc ** 6 * dt ** 6 * dx ** 7 * dz ** 3 * dy ** 5 +
     # 0.28800E5 * cc ** 6 * dt ** 6 * dx ** 7 * dz ** 5 * dy ** 3 + 0.1
     #8000E5 * cc ** 6 * dt ** 6 * dx ** 7 * dz ** 7 * dy + 0.28800E5 * 
     #cc ** 6 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 5 + 0.28800E5 * cc *
     #* 6 * dt ** 6 * dy ** 3 * dz ** 7 * dx ** 5 + 0.21600E5 * cc ** 6 
     #* dt ** 6 * dy ** 5 * dz ** 7 * dx ** 3 + 0.26400E5 * cc ** 5 * dt
     # ** 5 * dx ** 7 * dz ** 3 * dy ** 6 + 0.50400E5 * cc ** 5 * dt ** 
     #5 * dx ** 7 * dy ** 5 * dz ** 4 + 0.68400E5 * cc ** 5 * dt ** 5 * 
     #dx ** 7 * dz ** 5 * dy ** 4 + 0.32400E5 * cc ** 5 * dt ** 5 * dx *
     #* 7 * dy ** 3 * dz ** 6 + 0.67200E5 * cc ** 5 * dt ** 5 * dx ** 7 
     #* dz ** 7 * dy ** 2 + 0.31200E5 * cc ** 5 * dt ** 5 * dy ** 5 * dz
     # ** 5 * dx ** 6 + 0.32400E5 * cc ** 5 * dt ** 5 * dy ** 3 * dz ** 
     #7 * dx ** 6 + 0.33600E5 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 5 * 
     #dy ** 6 + 0.31200E5 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 * dz *
     #* 6 + 0.68400E5 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 7 * dy ** 4 
     #+ 0.50400E5 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 7 * dx ** 4 + 0.
     #26400E5 * cc ** 5 * dt ** 5 * dx ** 3 * dz ** 7 * dy ** 6 - 0.2280
     #00E6 * cc ** 4 * dt ** 4 * dx ** 7 * dz ** 5 * dy ** 5 - 0.234000E
     #6 * cc ** 4 * dt ** 4 * dx ** 7 * dz ** 7 * dy ** 3 - 0.228000E6 *
     # cc ** 4 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 5 - 0.112000E6 * cc
     # ** 3 * dt ** 3 * dx ** 7 * dz ** 5 * dy ** 6 - 0.96000E5 * cc ** 
     #3 * dt ** 3 * dx ** 7 * dy ** 5 * dz ** 6 - 0.235200E6 * cc ** 3 *
     # dt ** 3 * dx ** 7 * dz ** 7 * dy ** 4 - 0.96000E5 * cc ** 3 * dt 
     #** 3 * dy ** 5 * dz ** 7 * dx ** 6 - 0.112000E6 * cc ** 3 * dt ** 
     #3 * dx ** 5 * dz ** 7 * dy ** 6 + 0.648000E6 * cc ** 2 * dt ** 2 *
     # dx ** 7 * dz ** 7 * dy ** 5) / dx ** 7 / dy ** 7 / dz ** 7 / 0.86
     #4000E6
      cuu(375) = (0.1305E4 * cc ** 7 * dt ** 7 * dx ** 7 * dz * dy ** 6 
     #+ 0.3915E4 * cc ** 7 * dt ** 7 * dx ** 7 * dy ** 5 * dz ** 2 + 0.4
     #176E4 * cc ** 7 * dt ** 7 * dx ** 7 * dz ** 3 * dy ** 4 + 0.4176E4
     # * cc ** 7 * dt ** 7 * dx ** 7 * dy ** 3 * dz ** 4 + 0.3915E4 * cc
     # ** 7 * dt ** 7 * dx ** 7 * dz ** 5 * dy ** 2 + 0.1305E4 * cc ** 7
     # * dt ** 7 * dx ** 7 * dy * dz ** 6 + 0.2088E4 * cc ** 7 * dt ** 7
     # * dy ** 5 * dz ** 3 * dx ** 6 + 0.2088E4 * cc ** 7 * dt ** 7 * dy
     # ** 3 * dz ** 5 * dx ** 6 + 0.2088E4 * cc ** 7 * dt ** 7 * dx ** 5
     # * dz ** 3 * dy ** 6 + 0.4176E4 * cc ** 7 * dt ** 7 * dx ** 5 * dy
     # ** 5 * dz ** 4 + 0.4176E4 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 5
     # * dy ** 4 + 0.2088E4 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 3 * dz
     # ** 6 + 0.3132E4 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 5 * dx ** 4
     # + 0.1566E4 * cc ** 7 * dt ** 7 * dx ** 3 * dz ** 5 * dy ** 6 + 0.
     #1566E4 * cc ** 7 * dt ** 7 * dx ** 3 * dy ** 5 * dz ** 6 - 0.14400
     #E5 * cc ** 6 * dt ** 6 * dx ** 7 * dz ** 3 * dy ** 5 - 0.14400E5 *
     # cc ** 6 * dt ** 6 * dx ** 7 * dz ** 5 * dy ** 3 - 0.14400E5 * cc 
     #** 6 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 5 - 0.18600E5 * cc ** 5
     # * dt ** 5 * dx ** 7 * dz ** 3 * dy ** 6 - 0.36600E5 * cc ** 5 * d
     #t ** 5 * dx ** 7 * dy ** 5 * dz ** 4 - 0.36600E5 * cc ** 5 * dt **
     # 5 * dx ** 7 * dz ** 5 * dy ** 4 - 0.18600E5 * cc ** 5 * dt ** 5 *
     # dx ** 7 * dy ** 3 * dz ** 6 - 0.16800E5 * cc ** 5 * dt ** 5 * dy 
     #** 5 * dz ** 5 * dx ** 6 - 0.18000E5 * cc ** 5 * dt ** 5 * dx ** 5
     # * dz ** 5 * dy ** 6 - 0.18000E5 * cc ** 5 * dt ** 5 * dx ** 5 * d
     #y ** 5 * dz ** 6 + 0.120000E6 * cc ** 4 * dt ** 4 * dx ** 7 * dz *
     #* 5 * dy ** 5 + 0.64000E5 * cc ** 3 * dt ** 3 * dx ** 7 * dz ** 5 
     #* dy ** 6 + 0.64000E5 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 5 * dz
     # ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuu(376) = (-0.522E3 * cc ** 7 * dt ** 7 * dx ** 7 * dz * dy ** 6 
     #- 0.1566E4 * cc ** 7 * dt ** 7 * dx ** 7 * dy ** 5 * dz ** 2 - 0.1
     #044E4 * cc ** 7 * dt ** 7 * dx ** 7 * dz ** 3 * dy ** 4 - 0.1044E4
     # * cc ** 7 * dt ** 7 * dx ** 7 * dy ** 3 * dz ** 4 - 0.522E3 * cc 
     #** 7 * dt ** 7 * dy ** 5 * dz ** 3 * dx ** 6 - 0.522E3 * cc ** 7 *
     # dt ** 7 * dx ** 5 * dz ** 3 * dy ** 6 - 0.1044E4 * cc ** 7 * dt *
     #* 7 * dx ** 5 * dy ** 5 * dz ** 4 + 0.3600E4 * cc ** 6 * dt ** 6 *
     # dx ** 7 * dz ** 3 * dy ** 5 + 0.6000E4 * cc ** 5 * dt ** 5 * dx *
     #* 7 * dz ** 3 * dy ** 6 + 0.13200E5 * cc ** 5 * dt ** 5 * dx ** 7 
     #* dy ** 5 * dz ** 4 + 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 7 * dz 
     #** 5 * dy ** 4 + 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 3 
     #* dz ** 6 + 0.1200E4 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 5 * dx 
     #** 6 + 0.1200E4 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 5 * dy ** 6 
     #+ 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6 - 0.6
     #000E4 * cc ** 4 * dt ** 4 * dx ** 7 * dz ** 5 * dy ** 5 - 0.8800E4
     # * cc ** 3 * dt ** 3 * dx ** 7 * dz ** 5 * dy ** 6 - 0.18400E5 * c
     #c ** 3 * dt ** 3 * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 
     #7 / dz ** 7 / 0.864000E6
      cuu(377) = (0.87E2 * cc ** 7 * dt ** 7 * dx ** 7 * dz * dy ** 6 + 
     #0.261E3 * cc ** 7 * dt ** 7 * dx ** 7 * dy ** 5 * dz ** 2 - 0.600E
     #3 * cc ** 5 * dt ** 5 * dx ** 7 * dz ** 3 * dy ** 6 - 0.1800E4 * c
     #c ** 5 * dt ** 5 * dx ** 7 * dy ** 5 * dz ** 4 + 0.800E3 * cc ** 3
     # * dt ** 3 * dx ** 7 * dz ** 5 * dy ** 6 + 0.2400E4 * cc ** 3 * dt
     # ** 3 * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7
     # / 0.864000E6
      cuu(381) = (0.261E3 * cc ** 7 * dt ** 7 * dx ** 7 * dz ** 3 * dy *
     #* 4 + 0.261E3 * cc ** 7 * dt ** 7 * dx ** 7 * dy ** 3 * dz ** 4 - 
     #0.600E3 * cc ** 5 * dt ** 5 * dx ** 7 * dz ** 3 * dy ** 6 - 0.600E
     #3 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 5 * dz ** 4 - 0.600E3 * cc
     # ** 5 * dt ** 5 * dx ** 7 * dz ** 5 * dy ** 4 - 0.600E3 * cc ** 5 
     #* dt ** 5 * dx ** 7 * dy ** 3 * dz ** 6 + 0.1000E4 * cc ** 3 * dt 
     #** 3 * dx ** 7 * dz ** 5 * dy ** 6 + 0.1000E4 * cc ** 3 * dt ** 3 
     #* dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.8
     #64000E6
      cuu(382) = (-0.1044E4 * cc ** 7 * dt ** 7 * dx ** 7 * dz ** 3 * dy
     # ** 4 - 0.1044E4 * cc ** 7 * dt ** 7 * dx ** 7 * dy ** 3 * dz ** 4
     # - 0.1566E4 * cc ** 7 * dt ** 7 * dx ** 7 * dz ** 5 * dy ** 2 - 0.
     #522E3 * cc ** 7 * dt ** 7 * dx ** 7 * dy * dz ** 6 - 0.522E3 * cc 
     #** 7 * dt ** 7 * dy ** 3 * dz ** 5 * dx ** 6 - 0.1044E4 * cc ** 7 
     #* dt ** 7 * dx ** 5 * dz ** 5 * dy ** 4 - 0.522E3 * cc ** 7 * dt *
     #* 7 * dx ** 5 * dy ** 3 * dz ** 6 + 0.3600E4 * cc ** 6 * dt ** 6 *
     # dx ** 7 * dz ** 5 * dy ** 3 + 0.2400E4 * cc ** 5 * dt ** 5 * dx *
     #* 7 * dz ** 3 * dy ** 6 + 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 7 *
     # dy ** 5 * dz ** 4 + 0.13200E5 * cc ** 5 * dt ** 5 * dx ** 7 * dz 
     #** 5 * dy ** 4 + 0.6000E4 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 3 
     #* dz ** 6 + 0.1200E4 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 5 * dx 
     #** 6 + 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 5 * dy ** 6 
     #+ 0.1200E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6 - 0.6
     #000E4 * cc ** 4 * dt ** 4 * dx ** 7 * dz ** 5 * dy ** 5 - 0.18400E
     #5 * cc ** 3 * dt ** 3 * dx ** 7 * dz ** 5 * dy ** 6 - 0.8800E4 * c
     #c ** 3 * dt ** 3 * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 
     #7 / dz ** 7 / 0.864000E6
      cuu(383) = (0.1566E4 * cc ** 7 * dt ** 7 * dx ** 7 * dz ** 3 * dy 
     #** 4 + 0.1566E4 * cc ** 7 * dt ** 7 * dx ** 7 * dy ** 3 * dz ** 4 
     #+ 0.3132E4 * cc ** 7 * dt ** 7 * dx ** 7 * dz ** 5 * dy ** 2 + 0.1
     #044E4 * cc ** 7 * dt ** 7 * dx ** 7 * dy * dz ** 6 + 0.2436E4 * cc
     # ** 7 * dt ** 7 * dx ** 7 * dz ** 7 + 0.1044E4 * cc ** 7 * dt ** 7
     # * dy ** 3 * dz ** 5 * dx ** 6 + 0.1044E4 * cc ** 7 * dt ** 7 * dy
     # * dz ** 7 * dx ** 6 + 0.2088E4 * cc ** 7 * dt ** 7 * dx ** 5 * dz
     # ** 5 * dy ** 4 + 0.1044E4 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 3
     # * dz ** 6 + 0.3132E4 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 7 * dy
     # ** 2 + 0.1566E4 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 7 * dx ** 4
     # + 0.1566E4 * cc ** 7 * dt ** 7 * dx ** 3 * dz ** 7 * dy ** 4 - 0.
     #7200E4 * cc ** 6 * dt ** 6 * dx ** 7 * dz ** 5 * dy ** 3 - 0.7200E
     #4 * cc ** 6 * dt ** 6 * dx ** 7 * dz ** 7 * dy - 0.7200E4 * cc ** 
     #6 * dt ** 6 * dy ** 3 * dz ** 7 * dx ** 5 - 0.3600E4 * cc ** 5 * d
     #t ** 5 * dx ** 7 * dz ** 3 * dy ** 6 - 0.3600E4 * cc ** 5 * dt ** 
     #5 * dx ** 7 * dy ** 5 * dz ** 4 - 0.25200E5 * cc ** 5 * dt ** 5 * 
     #dx ** 7 * dz ** 5 * dy ** 4 - 0.10800E5 * cc ** 5 * dt ** 5 * dx *
     #* 7 * dy ** 3 * dz ** 6 - 0.33600E5 * cc ** 5 * dt ** 5 * dx ** 7 
     #* dz ** 7 * dy ** 2 - 0.2400E4 * cc ** 5 * dt ** 5 * dy ** 5 * dz 
     #** 5 * dx ** 6 - 0.10800E5 * cc ** 5 * dt ** 5 * dy ** 3 * dz ** 7
     # * dx ** 6 - 0.4800E4 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 5 * dy
     # ** 6 - 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6
     # - 0.25200E5 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 7 * dy ** 4 - 0
     #.3600E4 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 7 * dx ** 4 - 0.3600
     #E4 * cc ** 5 * dt ** 5 * dx ** 3 * dz ** 7 * dy ** 6 + 0.12000E5 *
     # cc ** 4 * dt ** 4 * dx ** 7 * dz ** 5 * dy ** 5 + 0.72000E5 * cc 
     #** 4 * dt ** 4 * dx ** 7 * dz ** 7 * dy ** 3 + 0.12000E5 * cc ** 4
     # * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 5 + 0.34800E5 * cc ** 3 * d
     #t ** 3 * dx ** 7 * dz ** 5 * dy ** 6 + 0.15600E5 * cc ** 3 * dt **
     # 3 * dx ** 7 * dy ** 5 * dz ** 6 + 0.117600E6 * cc ** 3 * dt ** 3 
     #* dx ** 7 * dz ** 7 * dy ** 4 + 0.15600E5 * cc ** 3 * dt ** 3 * dy
     # ** 5 * dz ** 7 * dx ** 6 + 0.34800E5 * cc ** 3 * dt ** 3 * dx ** 
     #5 * dz ** 7 * dy ** 6 - 0.64800E5 * cc ** 2 * dt ** 2 * dx ** 7 * 
     #dz ** 7 * dy ** 5) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuu(384) = (-0.1044E4 * cc ** 7 * dt ** 7 * dx ** 7 * dz ** 3 * dy
     # ** 4 - 0.1044E4 * cc ** 7 * dt ** 7 * dx ** 7 * dy ** 3 * dz ** 4
     # - 0.1566E4 * cc ** 7 * dt ** 7 * dx ** 7 * dz ** 5 * dy ** 2 - 0.
     #522E3 * cc ** 7 * dt ** 7 * dx ** 7 * dy * dz ** 6 - 0.522E3 * cc 
     #** 7 * dt ** 7 * dy ** 3 * dz ** 5 * dx ** 6 - 0.1044E4 * cc ** 7 
     #* dt ** 7 * dx ** 5 * dz ** 5 * dy ** 4 - 0.522E3 * cc ** 7 * dt *
     #* 7 * dx ** 5 * dy ** 3 * dz ** 6 + 0.3600E4 * cc ** 6 * dt ** 6 *
     # dx ** 7 * dz ** 5 * dy ** 3 + 0.2400E4 * cc ** 5 * dt ** 5 * dx *
     #* 7 * dz ** 3 * dy ** 6 + 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 7 *
     # dy ** 5 * dz ** 4 + 0.13200E5 * cc ** 5 * dt ** 5 * dx ** 7 * dz 
     #** 5 * dy ** 4 + 0.6000E4 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 3 
     #* dz ** 6 + 0.1200E4 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 5 * dx 
     #** 6 + 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 5 * dy ** 6 
     #+ 0.1200E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6 - 0.6
     #000E4 * cc ** 4 * dt ** 4 * dx ** 7 * dz ** 5 * dy ** 5 - 0.18400E
     #5 * cc ** 3 * dt ** 3 * dx ** 7 * dz ** 5 * dy ** 6 - 0.8800E4 * c
     #c ** 3 * dt ** 3 * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 
     #7 / dz ** 7 / 0.864000E6
      cuu(385) = (0.261E3 * cc ** 7 * dt ** 7 * dx ** 7 * dz ** 3 * dy *
     #* 4 + 0.261E3 * cc ** 7 * dt ** 7 * dx ** 7 * dy ** 3 * dz ** 4 - 
     #0.600E3 * cc ** 5 * dt ** 5 * dx ** 7 * dz ** 3 * dy ** 6 - 0.600E
     #3 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 5 * dz ** 4 - 0.600E3 * cc
     # ** 5 * dt ** 5 * dx ** 7 * dz ** 5 * dy ** 4 - 0.600E3 * cc ** 5 
     #* dt ** 5 * dx ** 7 * dy ** 3 * dz ** 6 + 0.1000E4 * cc ** 3 * dt 
     #** 3 * dx ** 7 * dz ** 5 * dy ** 6 + 0.1000E4 * cc ** 3 * dt ** 3 
     #* dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.8
     #64000E6
      cuu(391) = (0.261E3 * cc ** 7 * dt ** 7 * dx ** 7 * dz ** 5 * dy *
     #* 2 + 0.87E2 * cc ** 7 * dt ** 7 * dx ** 7 * dy * dz ** 6 - 0.1800
     #E4 * cc ** 5 * dt ** 5 * dx ** 7 * dz ** 5 * dy ** 4 - 0.600E3 * c
     #c ** 5 * dt ** 5 * dx ** 7 * dy ** 3 * dz ** 6 + 0.2400E4 * cc ** 
     #3 * dt ** 3 * dx ** 7 * dz ** 5 * dy ** 6 + 0.800E3 * cc ** 3 * dt
     # ** 3 * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7
     # / 0.864000E6
      cuu(392) = (-0.522E3 * cc ** 7 * dt ** 7 * dx ** 7 * dz ** 5 * dy 
     #** 2 - 0.174E3 * cc ** 7 * dt ** 7 * dx ** 7 * dy * dz ** 6 - 0.69
     #6E3 * cc ** 7 * dt ** 7 * dx ** 7 * dz ** 7 - 0.174E3 * cc ** 7 * 
     #dt ** 7 * dy * dz ** 7 * dx ** 6 - 0.522E3 * cc ** 7 * dt ** 7 * d
     #x ** 5 * dz ** 7 * dy ** 2 + 0.1200E4 * cc ** 6 * dt ** 6 * dx ** 
     #7 * dz ** 7 * dy + 0.3600E4 * cc ** 5 * dt ** 5 * dx ** 7 * dz ** 
     #5 * dy ** 4 + 0.1200E4 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 3 * d
     #z ** 6 + 0.9600E4 * cc ** 5 * dt ** 5 * dx ** 7 * dz ** 7 * dy ** 
     #2 + 0.1200E4 * cc ** 5 * dt ** 5 * dy ** 3 * dz ** 7 * dx ** 6 + 0
     #.3600E4 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 7 * dy ** 4 - 0.6000
     #E4 * cc ** 4 * dt ** 4 * dx ** 7 * dz ** 7 * dy ** 3 - 0.4800E4 * 
     #cc ** 3 * dt ** 3 * dx ** 7 * dz ** 5 * dy ** 6 - 0.1600E4 * cc **
     # 3 * dt ** 3 * dx ** 7 * dy ** 5 * dz ** 6 - 0.33600E5 * cc ** 3 *
     # dt ** 3 * dx ** 7 * dz ** 7 * dy ** 4 - 0.1600E4 * cc ** 3 * dt *
     #* 3 * dy ** 5 * dz ** 7 * dx ** 6 - 0.4800E4 * cc ** 3 * dt ** 3 *
     # dx ** 5 * dz ** 7 * dy ** 6 + 0.4800E4 * cc ** 2 * dt ** 2 * dx *
     #* 7 * dz ** 7 * dy ** 5) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E
     #6
      cuu(393) = (0.261E3 * cc ** 7 * dt ** 7 * dx ** 7 * dz ** 5 * dy *
     #* 2 + 0.87E2 * cc ** 7 * dt ** 7 * dx ** 7 * dy * dz ** 6 - 0.1800
     #E4 * cc ** 5 * dt ** 5 * dx ** 7 * dz ** 5 * dy ** 4 - 0.600E3 * c
     #c ** 5 * dt ** 5 * dx ** 7 * dy ** 3 * dz ** 6 + 0.2400E4 * cc ** 
     #3 * dt ** 3 * dx ** 7 * dz ** 5 * dy ** 6 + 0.800E3 * cc ** 3 * dt
     # ** 3 * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7
     # / 0.864000E6
      cuu(401) = (0.87E2 * cc ** 7 * dt ** 7 * dx ** 7 * dz ** 7 - 0.120
     #0E4 * cc ** 5 * dt ** 5 * dx ** 7 * dz ** 7 * dy ** 2 + 0.4200E4 *
     # cc ** 3 * dt ** 3 * dx ** 7 * dz ** 7 * dy ** 4) / dx ** 7 / dy *
     #* 7 / dz ** 7 / 0.864000E6
      cuu(419) = (0.87E2 * cc ** 7 * dt ** 7 * dy * dz ** 7 * dx ** 6 + 
     #0.261E3 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 7 * dy ** 2 - 0.600E
     #3 * cc ** 5 * dt ** 5 * dy ** 3 * dz ** 7 * dx ** 6 - 0.1800E4 * c
     #c ** 5 * dt ** 5 * dx ** 5 * dz ** 7 * dy ** 4 + 0.800E3 * cc ** 3
     # * dt ** 3 * dy ** 5 * dz ** 7 * dx ** 6 + 0.2400E4 * cc ** 3 * dt
     # ** 3 * dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7
     # / 0.864000E6
      cuu(427) = (0.261E3 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 5 * dx *
     #* 6 + 0.522E3 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 5 * dy ** 4 + 
     #0.261E3 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 3 * dz ** 6 - 0.600E
     #3 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 5 * dx ** 6 - 0.1200E4 * c
     #c ** 5 * dt ** 5 * dx ** 5 * dz ** 5 * dy ** 6 - 0.600E3 * cc ** 5
     # * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz
     # ** 7 / 0.864000E6
      cuu(428) = (-0.522E3 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 5 * dx 
     #** 6 - 0.522E3 * cc ** 7 * dt ** 7 * dy * dz ** 7 * dx ** 6 - 0.10
     #44E4 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 5 * dy ** 4 - 0.522E3 *
     # cc ** 7 * dt ** 7 * dx ** 5 * dy ** 3 * dz ** 6 - 0.1566E4 * cc *
     #* 7 * dt ** 7 * dx ** 5 * dz ** 7 * dy ** 2 - 0.1044E4 * cc ** 7 *
     # dt ** 7 * dy ** 3 * dz ** 7 * dx ** 4 - 0.1044E4 * cc ** 7 * dt *
     #* 7 * dx ** 3 * dz ** 7 * dy ** 4 + 0.3600E4 * cc ** 6 * dt ** 6 *
     # dy ** 3 * dz ** 7 * dx ** 5 + 0.1200E4 * cc ** 5 * dt ** 5 * dy *
     #* 5 * dz ** 5 * dx ** 6 + 0.6000E4 * cc ** 5 * dt ** 5 * dy ** 3 *
     # dz ** 7 * dx ** 6 + 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 5 * dz *
     #* 5 * dy ** 6 + 0.1200E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 *
     # dz ** 6 + 0.13200E5 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 7 * dy 
     #** 4 + 0.2400E4 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 7 * dx ** 4 
     #+ 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 3 * dz ** 7 * dy ** 6 - 0.6
     #000E4 * cc ** 4 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 5 - 0.8800E4
     # * cc ** 3 * dt ** 3 * dy ** 5 * dz ** 7 * dx ** 6 - 0.18400E5 * c
     #c ** 3 * dt ** 3 * dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 
     #7 / dz ** 7 / 0.864000E6
      cuu(429) = (0.261E3 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 5 * dx *
     #* 6 + 0.522E3 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 5 * dy ** 4 + 
     #0.261E3 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 3 * dz ** 6 - 0.600E
     #3 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 5 * dx ** 6 - 0.1200E4 * c
     #c ** 5 * dt ** 5 * dx ** 5 * dz ** 5 * dy ** 6 - 0.600E3 * cc ** 5
     # * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz
     # ** 7 / 0.864000E6
      cuu(435) = (0.261E3 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 3 * dx *
     #* 6 + 0.261E3 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 3 * dy ** 6 + 
     #0.522E3 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 5 * dz ** 4 - 0.600E
     #3 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 5 * dx ** 6 - 0.600E3 * cc
     # ** 5 * dt ** 5 * dx ** 5 * dz ** 5 * dy ** 6 - 0.1200E4 * cc ** 5
     # * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz
     # ** 7 / 0.864000E6
      cuu(436) = (-0.1044E4 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 3 * dx
     # ** 6 - 0.1044E4 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 5 * dx ** 6
     # - 0.1044E4 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 3 * dy ** 6 - 0.
     #2088E4 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 5 * dz ** 4 - 0.2088E
     #4 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 5 * dy ** 4 - 0.1044E4 * c
     #c ** 7 * dt ** 7 * dx ** 5 * dy ** 3 * dz ** 6 - 0.2088E4 * cc ** 
     #7 * dt ** 7 * dy ** 5 * dz ** 5 * dx ** 4 - 0.1044E4 * cc ** 7 * d
     #t ** 7 * dx ** 3 * dz ** 5 * dy ** 6 - 0.1044E4 * cc ** 7 * dt ** 
     #7 * dx ** 3 * dy ** 5 * dz ** 6 + 0.7200E4 * cc ** 6 * dt ** 6 * d
     #y ** 5 * dz ** 5 * dx ** 5 + 0.9600E4 * cc ** 5 * dt ** 5 * dy ** 
     #5 * dz ** 5 * dx ** 6 + 0.9600E4 * cc ** 5 * dt ** 5 * dx ** 5 * d
     #z ** 5 * dy ** 6 + 0.9600E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 
     #5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuu(437) = (0.1566E4 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 3 * dx 
     #** 6 + 0.2088E4 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 5 * dx ** 6 
     #+ 0.1305E4 * cc ** 7 * dt ** 7 * dy * dz ** 7 * dx ** 6 + 0.1566E4
     # * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 3 * dy ** 6 + 0.3132E4 * cc
     # ** 7 * dt ** 7 * dx ** 5 * dy ** 5 * dz ** 4 + 0.4176E4 * cc ** 7
     # * dt ** 7 * dx ** 5 * dz ** 5 * dy ** 4 + 0.2088E4 * cc ** 7 * dt
     # ** 7 * dx ** 5 * dy ** 3 * dz ** 6 + 0.3915E4 * cc ** 7 * dt ** 7
     # * dx ** 5 * dz ** 7 * dy ** 2 + 0.4176E4 * cc ** 7 * dt ** 7 * dy
     # ** 5 * dz ** 5 * dx ** 4 + 0.4176E4 * cc ** 7 * dt ** 7 * dy ** 3
     # * dz ** 7 * dx ** 4 + 0.2088E4 * cc ** 7 * dt ** 7 * dx ** 3 * dz
     # ** 5 * dy ** 6 + 0.2088E4 * cc ** 7 * dt ** 7 * dx ** 3 * dy ** 5
     # * dz ** 6 + 0.4176E4 * cc ** 7 * dt ** 7 * dx ** 3 * dz ** 7 * dy
     # ** 4 + 0.3915E4 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 7 * dx ** 2
     # + 0.1305E4 * cc ** 7 * dt ** 7 * dx * dz ** 7 * dy ** 6 - 0.14400
     #E5 * cc ** 6 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 5 - 0.14400E5 *
     # cc ** 6 * dt ** 6 * dy ** 3 * dz ** 7 * dx ** 5 - 0.14400E5 * cc 
     #** 6 * dt ** 6 * dy ** 5 * dz ** 7 * dx ** 3 - 0.18000E5 * cc ** 5
     # * dt ** 5 * dy ** 5 * dz ** 5 * dx ** 6 - 0.18600E5 * cc ** 5 * d
     #t ** 5 * dy ** 3 * dz ** 7 * dx ** 6 - 0.18000E5 * cc ** 5 * dt **
     # 5 * dx ** 5 * dz ** 5 * dy ** 6 - 0.16800E5 * cc ** 5 * dt ** 5 *
     # dx ** 5 * dy ** 5 * dz ** 6 - 0.36600E5 * cc ** 5 * dt ** 5 * dx 
     #** 5 * dz ** 7 * dy ** 4 - 0.36600E5 * cc ** 5 * dt ** 5 * dy ** 5
     # * dz ** 7 * dx ** 4 - 0.18600E5 * cc ** 5 * dt ** 5 * dx ** 3 * d
     #z ** 7 * dy ** 6 + 0.120000E6 * cc ** 4 * dt ** 4 * dy ** 5 * dz *
     #* 7 * dx ** 5 + 0.64000E5 * cc ** 3 * dt ** 3 * dy ** 5 * dz ** 7 
     #* dx ** 6 + 0.64000E5 * cc ** 3 * dt ** 3 * dx ** 5 * dz ** 7 * dy
     # ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuu(438) = (-0.1044E4 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 3 * dx
     # ** 6 - 0.1044E4 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 5 * dx ** 6
     # - 0.1044E4 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 3 * dy ** 6 - 0.
     #2088E4 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 5 * dz ** 4 - 0.2088E
     #4 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 5 * dy ** 4 - 0.1044E4 * c
     #c ** 7 * dt ** 7 * dx ** 5 * dy ** 3 * dz ** 6 - 0.2088E4 * cc ** 
     #7 * dt ** 7 * dy ** 5 * dz ** 5 * dx ** 4 - 0.1044E4 * cc ** 7 * d
     #t ** 7 * dx ** 3 * dz ** 5 * dy ** 6 - 0.1044E4 * cc ** 7 * dt ** 
     #7 * dx ** 3 * dy ** 5 * dz ** 6 + 0.7200E4 * cc ** 6 * dt ** 6 * d
     #y ** 5 * dz ** 5 * dx ** 5 + 0.9600E4 * cc ** 5 * dt ** 5 * dy ** 
     #5 * dz ** 5 * dx ** 6 + 0.9600E4 * cc ** 5 * dt ** 5 * dx ** 5 * d
     #z ** 5 * dy ** 6 + 0.9600E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 
     #5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuu(439) = (0.261E3 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 3 * dx *
     #* 6 + 0.261E3 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 3 * dy ** 6 + 
     #0.522E3 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 5 * dz ** 4 - 0.600E
     #3 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 5 * dx ** 6 - 0.600E3 * cc
     # ** 5 * dt ** 5 * dx ** 5 * dz ** 5 * dy ** 6 - 0.1200E4 * cc ** 5
     # * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz
     # ** 7 / 0.864000E6
      cuu(443) = (0.87E2 * cc ** 7 * dt ** 7 * dy ** 7 * dz * dx ** 6 + 
     #0.261E3 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 7 * dz ** 2 - 0.600E
     #3 * cc ** 5 * dt ** 5 * dy ** 7 * dz ** 3 * dx ** 6 - 0.1800E4 * c
     #c ** 5 * dt ** 5 * dx ** 5 * dy ** 7 * dz ** 4 + 0.800E3 * cc ** 3
     # * dt ** 3 * dy ** 7 * dz ** 5 * dx ** 6 + 0.2400E4 * cc ** 3 * dt
     # ** 3 * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7
     # / 0.864000E6
      cuu(444) = (-0.522E3 * cc ** 7 * dt ** 7 * dy ** 7 * dz * dx ** 6 
     #- 0.522E3 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 3 * dx ** 6 - 0.15
     #66E4 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 7 * dz ** 2 - 0.522E3 *
     # cc ** 7 * dt ** 7 * dx ** 5 * dz ** 3 * dy ** 6 - 0.1044E4 * cc *
     #* 7 * dt ** 7 * dx ** 5 * dy ** 5 * dz ** 4 - 0.1044E4 * cc ** 7 *
     # dt ** 7 * dy ** 7 * dz ** 3 * dx ** 4 - 0.1044E4 * cc ** 7 * dt *
     #* 7 * dx ** 3 * dy ** 7 * dz ** 4 + 0.3600E4 * cc ** 6 * dt ** 6 *
     # dy ** 7 * dz ** 3 * dx ** 5 + 0.6000E4 * cc ** 5 * dt ** 5 * dy *
     #* 7 * dz ** 3 * dx ** 6 + 0.1200E4 * cc ** 5 * dt ** 5 * dy ** 5 *
     # dz ** 5 * dx ** 6 + 0.13200E5 * cc ** 5 * dt ** 5 * dx ** 5 * dy 
     #** 7 * dz ** 4 + 0.1200E4 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 5 
     #* dy ** 6 + 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 * dz 
     #** 6 + 0.2400E4 * cc ** 5 * dt ** 5 * dy ** 7 * dz ** 5 * dx ** 4 
     #+ 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 3 * dy ** 7 * dz ** 6 - 0.6
     #000E4 * cc ** 4 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 5 - 0.8800E4
     # * cc ** 3 * dt ** 3 * dy ** 7 * dz ** 5 * dx ** 6 - 0.18400E5 * c
     #c ** 3 * dt ** 3 * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 
     #7 / dz ** 7 / 0.864000E6
      cuu(445) = (0.1305E4 * cc ** 7 * dt ** 7 * dy ** 7 * dz * dx ** 6 
     #+ 0.2088E4 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 3 * dx ** 6 + 0.1
     #566E4 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 5 * dx ** 6 + 0.3915E4
     # * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 7 * dz ** 2 + 0.2088E4 * cc
     # ** 7 * dt ** 7 * dx ** 5 * dz ** 3 * dy ** 6 + 0.4176E4 * cc ** 7
     # * dt ** 7 * dx ** 5 * dy ** 5 * dz ** 4 + 0.3132E4 * cc ** 7 * dt
     # ** 7 * dx ** 5 * dz ** 5 * dy ** 4 + 0.1566E4 * cc ** 7 * dt ** 7
     # * dx ** 5 * dy ** 3 * dz ** 6 + 0.4176E4 * cc ** 7 * dt ** 7 * dy
     # ** 7 * dz ** 3 * dx ** 4 + 0.4176E4 * cc ** 7 * dt ** 7 * dy ** 5
     # * dz ** 5 * dx ** 4 + 0.4176E4 * cc ** 7 * dt ** 7 * dx ** 3 * dy
     # ** 7 * dz ** 4 + 0.2088E4 * cc ** 7 * dt ** 7 * dx ** 3 * dz ** 5
     # * dy ** 6 + 0.2088E4 * cc ** 7 * dt ** 7 * dx ** 3 * dy ** 5 * dz
     # ** 6 + 0.3915E4 * cc ** 7 * dt ** 7 * dy ** 7 * dz ** 5 * dx ** 2
     # + 0.1305E4 * cc ** 7 * dt ** 7 * dx * dy ** 7 * dz ** 6 - 0.14400
     #E5 * cc ** 6 * dt ** 6 * dy ** 7 * dz ** 3 * dx ** 5 - 0.14400E5 *
     # cc ** 6 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 5 - 0.14400E5 * cc 
     #** 6 * dt ** 6 * dy ** 7 * dz ** 5 * dx ** 3 - 0.18600E5 * cc ** 5
     # * dt ** 5 * dy ** 7 * dz ** 3 * dx ** 6 - 0.18000E5 * cc ** 5 * d
     #t ** 5 * dy ** 5 * dz ** 5 * dx ** 6 - 0.36600E5 * cc ** 5 * dt **
     # 5 * dx ** 5 * dy ** 7 * dz ** 4 - 0.16800E5 * cc ** 5 * dt ** 5 *
     # dx ** 5 * dz ** 5 * dy ** 6 - 0.18000E5 * cc ** 5 * dt ** 5 * dx 
     #** 5 * dy ** 5 * dz ** 6 - 0.36600E5 * cc ** 5 * dt ** 5 * dy ** 7
     # * dz ** 5 * dx ** 4 - 0.18600E5 * cc ** 5 * dt ** 5 * dx ** 3 * d
     #y ** 7 * dz ** 6 + 0.120000E6 * cc ** 4 * dt ** 4 * dy ** 7 * dz *
     #* 5 * dx ** 5 + 0.64000E5 * cc ** 3 * dt ** 3 * dy ** 7 * dz ** 5 
     #* dx ** 6 + 0.64000E5 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 7 * dz
     # ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuu(446) = (-0.1740E4 * cc ** 7 * dt ** 7 * dy ** 7 * dz * dx ** 6
     # - 0.3132E4 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 3 * dx ** 6 - 0.
     #3132E4 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 5 * dx ** 6 - 0.1740E
     #4 * cc ** 7 * dt ** 7 * dy * dz ** 7 * dx ** 6 - 0.5220E4 * cc ** 
     #7 * dt ** 7 * dx ** 5 * dy ** 7 * dz ** 2 - 0.3132E4 * cc ** 7 * d
     #t ** 7 * dx ** 5 * dz ** 3 * dy ** 6 - 0.6264E4 * cc ** 7 * dt ** 
     #7 * dx ** 5 * dy ** 5 * dz ** 4 - 0.6264E4 * cc ** 7 * dt ** 7 * d
     #x ** 5 * dz ** 5 * dy ** 4 - 0.3132E4 * cc ** 7 * dt ** 7 * dx ** 
     #5 * dy ** 3 * dz ** 6 - 0.5220E4 * cc ** 7 * dt ** 7 * dx ** 5 * d
     #z ** 7 * dy ** 2 - 0.6264E4 * cc ** 7 * dt ** 7 * dy ** 7 * dz ** 
     #3 * dx ** 4 - 0.8352E4 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 5 * d
     #x ** 4 - 0.6264E4 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 7 * dx ** 
     #4 - 0.6264E4 * cc ** 7 * dt ** 7 * dx ** 3 * dy ** 7 * dz ** 4 - 0
     #.4176E4 * cc ** 7 * dt ** 7 * dx ** 3 * dz ** 5 * dy ** 6 - 0.4176
     #E4 * cc ** 7 * dt ** 7 * dx ** 3 * dy ** 5 * dz ** 6 - 0.6264E4 * 
     #cc ** 7 * dt ** 7 * dx ** 3 * dz ** 7 * dy ** 4 - 0.7830E4 * cc **
     # 7 * dt ** 7 * dy ** 7 * dz ** 5 * dx ** 2 - 0.7830E4 * cc ** 7 * 
     #dt ** 7 * dy ** 5 * dz ** 7 * dx ** 2 - 0.2610E4 * cc ** 7 * dt **
     # 7 * dx * dy ** 7 * dz ** 6 - 0.2610E4 * cc ** 7 * dt ** 7 * dx * 
     #dz ** 7 * dy ** 6 - 0.4872E4 * cc ** 7 * dt ** 7 * dy ** 7 * dz **
     # 7 + 0.21600E5 * cc ** 6 * dt ** 6 * dy ** 7 * dz ** 3 * dx ** 5 +
     # 0.28800E5 * cc ** 6 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 5 + 0.2
     #1600E5 * cc ** 6 * dt ** 6 * dy ** 3 * dz ** 7 * dx ** 5 + 0.28800
     #E5 * cc ** 6 * dt ** 6 * dy ** 7 * dz ** 5 * dx ** 3 + 0.28800E5 *
     # cc ** 6 * dt ** 6 * dy ** 5 * dz ** 7 * dx ** 3 + 0.18000E5 * cc 
     #** 6 * dt ** 6 * dy ** 7 * dz ** 7 * dx + 0.26400E5 * cc ** 5 * dt
     # ** 5 * dy ** 7 * dz ** 3 * dx ** 6 + 0.33600E5 * cc ** 5 * dt ** 
     #5 * dy ** 5 * dz ** 5 * dx ** 6 + 0.26400E5 * cc ** 5 * dt ** 5 * 
     #dy ** 3 * dz ** 7 * dx ** 6 + 0.50400E5 * cc ** 5 * dt ** 5 * dx *
     #* 5 * dy ** 7 * dz ** 4 + 0.31200E5 * cc ** 5 * dt ** 5 * dx ** 5 
     #* dz ** 5 * dy ** 6 + 0.31200E5 * cc ** 5 * dt ** 5 * dx ** 5 * dy
     # ** 5 * dz ** 6 + 0.50400E5 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 
     #7 * dy ** 4 + 0.68400E5 * cc ** 5 * dt ** 5 * dy ** 7 * dz ** 5 * 
     #dx ** 4 + 0.68400E5 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 7 * dx *
     #* 4 + 0.32400E5 * cc ** 5 * dt ** 5 * dx ** 3 * dy ** 7 * dz ** 6 
     #+ 0.32400E5 * cc ** 5 * dt ** 5 * dx ** 3 * dz ** 7 * dy ** 6 + 0.
     #67200E5 * cc ** 5 * dt ** 5 * dy ** 7 * dz ** 7 * dx ** 2 - 0.2280
     #00E6 * cc ** 4 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 5 - 0.228000E
     #6 * cc ** 4 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 5 - 0.234000E6 *
     # cc ** 4 * dt ** 4 * dy ** 7 * dz ** 7 * dx ** 3 - 0.112000E6 * cc
     # ** 3 * dt ** 3 * dy ** 7 * dz ** 5 * dx ** 6 - 0.112000E6 * cc **
     # 3 * dt ** 3 * dy ** 5 * dz ** 7 * dx ** 6 - 0.96000E5 * cc ** 3 *
     # dt ** 3 * dx ** 5 * dy ** 7 * dz ** 6 - 0.96000E5 * cc ** 3 * dt 
     #** 3 * dx ** 5 * dz ** 7 * dy ** 6 - 0.235200E6 * cc ** 3 * dt ** 
     #3 * dy ** 7 * dz ** 7 * dx ** 4 + 0.648000E6 * cc ** 2 * dt ** 2 *
     # dy ** 7 * dz ** 7 * dx ** 5) / dx ** 7 / dy ** 7 / dz ** 7 / 0.86
     #4000E6
      cuu(447) = (0.1305E4 * cc ** 7 * dt ** 7 * dy ** 7 * dz * dx ** 6 
     #+ 0.2088E4 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 3 * dx ** 6 + 0.1
     #566E4 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 5 * dx ** 6 + 0.3915E4
     # * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 7 * dz ** 2 + 0.2088E4 * cc
     # ** 7 * dt ** 7 * dx ** 5 * dz ** 3 * dy ** 6 + 0.4176E4 * cc ** 7
     # * dt ** 7 * dx ** 5 * dy ** 5 * dz ** 4 + 0.3132E4 * cc ** 7 * dt
     # ** 7 * dx ** 5 * dz ** 5 * dy ** 4 + 0.1566E4 * cc ** 7 * dt ** 7
     # * dx ** 5 * dy ** 3 * dz ** 6 + 0.4176E4 * cc ** 7 * dt ** 7 * dy
     # ** 7 * dz ** 3 * dx ** 4 + 0.4176E4 * cc ** 7 * dt ** 7 * dy ** 5
     # * dz ** 5 * dx ** 4 + 0.4176E4 * cc ** 7 * dt ** 7 * dx ** 3 * dy
     # ** 7 * dz ** 4 + 0.2088E4 * cc ** 7 * dt ** 7 * dx ** 3 * dz ** 5
     # * dy ** 6 + 0.2088E4 * cc ** 7 * dt ** 7 * dx ** 3 * dy ** 5 * dz
     # ** 6 + 0.3915E4 * cc ** 7 * dt ** 7 * dy ** 7 * dz ** 5 * dx ** 2
     # + 0.1305E4 * cc ** 7 * dt ** 7 * dx * dy ** 7 * dz ** 6 - 0.14400
     #E5 * cc ** 6 * dt ** 6 * dy ** 7 * dz ** 3 * dx ** 5 - 0.14400E5 *
     # cc ** 6 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 5 - 0.14400E5 * cc 
     #** 6 * dt ** 6 * dy ** 7 * dz ** 5 * dx ** 3 - 0.18600E5 * cc ** 5
     # * dt ** 5 * dy ** 7 * dz ** 3 * dx ** 6 - 0.18000E5 * cc ** 5 * d
     #t ** 5 * dy ** 5 * dz ** 5 * dx ** 6 - 0.36600E5 * cc ** 5 * dt **
     # 5 * dx ** 5 * dy ** 7 * dz ** 4 - 0.16800E5 * cc ** 5 * dt ** 5 *
     # dx ** 5 * dz ** 5 * dy ** 6 - 0.18000E5 * cc ** 5 * dt ** 5 * dx 
     #** 5 * dy ** 5 * dz ** 6 - 0.36600E5 * cc ** 5 * dt ** 5 * dy ** 7
     # * dz ** 5 * dx ** 4 - 0.18600E5 * cc ** 5 * dt ** 5 * dx ** 3 * d
     #y ** 7 * dz ** 6 + 0.120000E6 * cc ** 4 * dt ** 4 * dy ** 7 * dz *
     #* 5 * dx ** 5 + 0.64000E5 * cc ** 3 * dt ** 3 * dy ** 7 * dz ** 5 
     #* dx ** 6 + 0.64000E5 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 7 * dz
     # ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuu(448) = (-0.522E3 * cc ** 7 * dt ** 7 * dy ** 7 * dz * dx ** 6 
     #- 0.522E3 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 3 * dx ** 6 - 0.15
     #66E4 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 7 * dz ** 2 - 0.522E3 *
     # cc ** 7 * dt ** 7 * dx ** 5 * dz ** 3 * dy ** 6 - 0.1044E4 * cc *
     #* 7 * dt ** 7 * dx ** 5 * dy ** 5 * dz ** 4 - 0.1044E4 * cc ** 7 *
     # dt ** 7 * dy ** 7 * dz ** 3 * dx ** 4 - 0.1044E4 * cc ** 7 * dt *
     #* 7 * dx ** 3 * dy ** 7 * dz ** 4 + 0.3600E4 * cc ** 6 * dt ** 6 *
     # dy ** 7 * dz ** 3 * dx ** 5 + 0.6000E4 * cc ** 5 * dt ** 5 * dy *
     #* 7 * dz ** 3 * dx ** 6 + 0.1200E4 * cc ** 5 * dt ** 5 * dy ** 5 *
     # dz ** 5 * dx ** 6 + 0.13200E5 * cc ** 5 * dt ** 5 * dx ** 5 * dy 
     #** 7 * dz ** 4 + 0.1200E4 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 5 
     #* dy ** 6 + 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 * dz 
     #** 6 + 0.2400E4 * cc ** 5 * dt ** 5 * dy ** 7 * dz ** 5 * dx ** 4 
     #+ 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 3 * dy ** 7 * dz ** 6 - 0.6
     #000E4 * cc ** 4 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 5 - 0.8800E4
     # * cc ** 3 * dt ** 3 * dy ** 7 * dz ** 5 * dx ** 6 - 0.18400E5 * c
     #c ** 3 * dt ** 3 * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 
     #7 / dz ** 7 / 0.864000E6
      cuu(449) = (0.87E2 * cc ** 7 * dt ** 7 * dy ** 7 * dz * dx ** 6 + 
     #0.261E3 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 7 * dz ** 2 - 0.600E
     #3 * cc ** 5 * dt ** 5 * dy ** 7 * dz ** 3 * dx ** 6 - 0.1800E4 * c
     #c ** 5 * dt ** 5 * dx ** 5 * dy ** 7 * dz ** 4 + 0.800E3 * cc ** 3
     # * dt ** 3 * dy ** 7 * dz ** 5 * dx ** 6 + 0.2400E4 * cc ** 3 * dt
     # ** 3 * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7
     # / 0.864000E6
      cuu(453) = (0.261E3 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 3 * dx *
     #* 6 + 0.261E3 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 3 * dy ** 6 + 
     #0.522E3 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 5 * dz ** 4 - 0.600E
     #3 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 5 * dx ** 6 - 0.600E3 * cc
     # ** 5 * dt ** 5 * dx ** 5 * dz ** 5 * dy ** 6 - 0.1200E4 * cc ** 5
     # * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz
     # ** 7 / 0.864000E6
      cuu(454) = (-0.1044E4 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 3 * dx
     # ** 6 - 0.1044E4 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 5 * dx ** 6
     # - 0.1044E4 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 3 * dy ** 6 - 0.
     #2088E4 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 5 * dz ** 4 - 0.2088E
     #4 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 5 * dy ** 4 - 0.1044E4 * c
     #c ** 7 * dt ** 7 * dx ** 5 * dy ** 3 * dz ** 6 - 0.2088E4 * cc ** 
     #7 * dt ** 7 * dy ** 5 * dz ** 5 * dx ** 4 - 0.1044E4 * cc ** 7 * d
     #t ** 7 * dx ** 3 * dz ** 5 * dy ** 6 - 0.1044E4 * cc ** 7 * dt ** 
     #7 * dx ** 3 * dy ** 5 * dz ** 6 + 0.7200E4 * cc ** 6 * dt ** 6 * d
     #y ** 5 * dz ** 5 * dx ** 5 + 0.9600E4 * cc ** 5 * dt ** 5 * dy ** 
     #5 * dz ** 5 * dx ** 6 + 0.9600E4 * cc ** 5 * dt ** 5 * dx ** 5 * d
     #z ** 5 * dy ** 6 + 0.9600E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 
     #5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuu(455) = (0.1566E4 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 3 * dx 
     #** 6 + 0.2088E4 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 5 * dx ** 6 
     #+ 0.1305E4 * cc ** 7 * dt ** 7 * dy * dz ** 7 * dx ** 6 + 0.1566E4
     # * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 3 * dy ** 6 + 0.3132E4 * cc
     # ** 7 * dt ** 7 * dx ** 5 * dy ** 5 * dz ** 4 + 0.4176E4 * cc ** 7
     # * dt ** 7 * dx ** 5 * dz ** 5 * dy ** 4 + 0.2088E4 * cc ** 7 * dt
     # ** 7 * dx ** 5 * dy ** 3 * dz ** 6 + 0.3915E4 * cc ** 7 * dt ** 7
     # * dx ** 5 * dz ** 7 * dy ** 2 + 0.4176E4 * cc ** 7 * dt ** 7 * dy
     # ** 5 * dz ** 5 * dx ** 4 + 0.4176E4 * cc ** 7 * dt ** 7 * dy ** 3
     # * dz ** 7 * dx ** 4 + 0.2088E4 * cc ** 7 * dt ** 7 * dx ** 3 * dz
     # ** 5 * dy ** 6 + 0.2088E4 * cc ** 7 * dt ** 7 * dx ** 3 * dy ** 5
     # * dz ** 6 + 0.4176E4 * cc ** 7 * dt ** 7 * dx ** 3 * dz ** 7 * dy
     # ** 4 + 0.3915E4 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 7 * dx ** 2
     # + 0.1305E4 * cc ** 7 * dt ** 7 * dx * dz ** 7 * dy ** 6 - 0.14400
     #E5 * cc ** 6 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 5 - 0.14400E5 *
     # cc ** 6 * dt ** 6 * dy ** 3 * dz ** 7 * dx ** 5 - 0.14400E5 * cc 
     #** 6 * dt ** 6 * dy ** 5 * dz ** 7 * dx ** 3 - 0.18000E5 * cc ** 5
     # * dt ** 5 * dy ** 5 * dz ** 5 * dx ** 6 - 0.18600E5 * cc ** 5 * d
     #t ** 5 * dy ** 3 * dz ** 7 * dx ** 6 - 0.18000E5 * cc ** 5 * dt **
     # 5 * dx ** 5 * dz ** 5 * dy ** 6 - 0.16800E5 * cc ** 5 * dt ** 5 *
     # dx ** 5 * dy ** 5 * dz ** 6 - 0.36600E5 * cc ** 5 * dt ** 5 * dx 
     #** 5 * dz ** 7 * dy ** 4 - 0.36600E5 * cc ** 5 * dt ** 5 * dy ** 5
     # * dz ** 7 * dx ** 4 - 0.18600E5 * cc ** 5 * dt ** 5 * dx ** 3 * d
     #z ** 7 * dy ** 6 + 0.120000E6 * cc ** 4 * dt ** 4 * dy ** 5 * dz *
     #* 7 * dx ** 5 + 0.64000E5 * cc ** 3 * dt ** 3 * dy ** 5 * dz ** 7 
     #* dx ** 6 + 0.64000E5 * cc ** 3 * dt ** 3 * dx ** 5 * dz ** 7 * dy
     # ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuu(456) = (-0.1044E4 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 3 * dx
     # ** 6 - 0.1044E4 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 5 * dx ** 6
     # - 0.1044E4 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 3 * dy ** 6 - 0.
     #2088E4 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 5 * dz ** 4 - 0.2088E
     #4 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 5 * dy ** 4 - 0.1044E4 * c
     #c ** 7 * dt ** 7 * dx ** 5 * dy ** 3 * dz ** 6 - 0.2088E4 * cc ** 
     #7 * dt ** 7 * dy ** 5 * dz ** 5 * dx ** 4 - 0.1044E4 * cc ** 7 * d
     #t ** 7 * dx ** 3 * dz ** 5 * dy ** 6 - 0.1044E4 * cc ** 7 * dt ** 
     #7 * dx ** 3 * dy ** 5 * dz ** 6 + 0.7200E4 * cc ** 6 * dt ** 6 * d
     #y ** 5 * dz ** 5 * dx ** 5 + 0.9600E4 * cc ** 5 * dt ** 5 * dy ** 
     #5 * dz ** 5 * dx ** 6 + 0.9600E4 * cc ** 5 * dt ** 5 * dx ** 5 * d
     #z ** 5 * dy ** 6 + 0.9600E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 
     #5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuu(457) = (0.261E3 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 3 * dx *
     #* 6 + 0.261E3 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 3 * dy ** 6 + 
     #0.522E3 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 5 * dz ** 4 - 0.600E
     #3 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 5 * dx ** 6 - 0.600E3 * cc
     # ** 5 * dt ** 5 * dx ** 5 * dz ** 5 * dy ** 6 - 0.1200E4 * cc ** 5
     # * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz
     # ** 7 / 0.864000E6
      cuu(463) = (0.261E3 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 5 * dx *
     #* 6 + 0.522E3 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 5 * dy ** 4 + 
     #0.261E3 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 3 * dz ** 6 - 0.600E
     #3 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 5 * dx ** 6 - 0.1200E4 * c
     #c ** 5 * dt ** 5 * dx ** 5 * dz ** 5 * dy ** 6 - 0.600E3 * cc ** 5
     # * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz
     # ** 7 / 0.864000E6
      cuu(464) = (-0.522E3 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 5 * dx 
     #** 6 - 0.522E3 * cc ** 7 * dt ** 7 * dy * dz ** 7 * dx ** 6 - 0.10
     #44E4 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 5 * dy ** 4 - 0.522E3 *
     # cc ** 7 * dt ** 7 * dx ** 5 * dy ** 3 * dz ** 6 - 0.1566E4 * cc *
     #* 7 * dt ** 7 * dx ** 5 * dz ** 7 * dy ** 2 - 0.1044E4 * cc ** 7 *
     # dt ** 7 * dy ** 3 * dz ** 7 * dx ** 4 - 0.1044E4 * cc ** 7 * dt *
     #* 7 * dx ** 3 * dz ** 7 * dy ** 4 + 0.3600E4 * cc ** 6 * dt ** 6 *
     # dy ** 3 * dz ** 7 * dx ** 5 + 0.1200E4 * cc ** 5 * dt ** 5 * dy *
     #* 5 * dz ** 5 * dx ** 6 + 0.6000E4 * cc ** 5 * dt ** 5 * dy ** 3 *
     # dz ** 7 * dx ** 6 + 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 5 * dz *
     #* 5 * dy ** 6 + 0.1200E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 *
     # dz ** 6 + 0.13200E5 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 7 * dy 
     #** 4 + 0.2400E4 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 7 * dx ** 4 
     #+ 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 3 * dz ** 7 * dy ** 6 - 0.6
     #000E4 * cc ** 4 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 5 - 0.8800E4
     # * cc ** 3 * dt ** 3 * dy ** 5 * dz ** 7 * dx ** 6 - 0.18400E5 * c
     #c ** 3 * dt ** 3 * dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 
     #7 / dz ** 7 / 0.864000E6
      cuu(465) = (0.261E3 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 5 * dx *
     #* 6 + 0.522E3 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 5 * dy ** 4 + 
     #0.261E3 * cc ** 7 * dt ** 7 * dx ** 5 * dy ** 3 * dz ** 6 - 0.600E
     #3 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 5 * dx ** 6 - 0.1200E4 * c
     #c ** 5 * dt ** 5 * dx ** 5 * dz ** 5 * dy ** 6 - 0.600E3 * cc ** 5
     # * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz
     # ** 7 / 0.864000E6
      cuu(473) = (0.87E2 * cc ** 7 * dt ** 7 * dy * dz ** 7 * dx ** 6 + 
     #0.261E3 * cc ** 7 * dt ** 7 * dx ** 5 * dz ** 7 * dy ** 2 - 0.600E
     #3 * cc ** 5 * dt ** 5 * dy ** 3 * dz ** 7 * dx ** 6 - 0.1800E4 * c
     #c ** 5 * dt ** 5 * dx ** 5 * dz ** 7 * dy ** 4 + 0.800E3 * cc ** 3
     # * dt ** 3 * dy ** 5 * dz ** 7 * dx ** 6 + 0.2400E4 * cc ** 3 * dt
     # ** 3 * dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7
     # / 0.864000E6
      cuu(509) = (0.261E3 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 7 * dx *
     #* 4 + 0.261E3 * cc ** 7 * dt ** 7 * dx ** 3 * dz ** 7 * dy ** 4 - 
     #0.600E3 * cc ** 5 * dt ** 5 * dy ** 3 * dz ** 7 * dx ** 6 - 0.600E
     #3 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 7 * dy ** 4 - 0.600E3 * cc
     # ** 5 * dt ** 5 * dy ** 5 * dz ** 7 * dx ** 4 - 0.600E3 * cc ** 5 
     #* dt ** 5 * dx ** 3 * dz ** 7 * dy ** 6 + 0.1000E4 * cc ** 3 * dt 
     #** 3 * dy ** 5 * dz ** 7 * dx ** 6 + 0.1000E4 * cc ** 3 * dt ** 3 
     #* dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.8
     #64000E6
      cuu(517) = (0.522E3 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 5 * dx *
     #* 4 + 0.261E3 * cc ** 7 * dt ** 7 * dx ** 3 * dz ** 5 * dy ** 6 + 
     #0.261E3 * cc ** 7 * dt ** 7 * dx ** 3 * dy ** 5 * dz ** 6 - 0.1200
     #E4 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 5 * dx ** 6 - 0.600E3 * c
     #c ** 5 * dt ** 5 * dx ** 5 * dz ** 5 * dy ** 6 - 0.600E3 * cc ** 5
     # * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz
     # ** 7 / 0.864000E6
      cuu(518) = (-0.1044E4 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 5 * dx
     # ** 4 - 0.1044E4 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 7 * dx ** 4
     # - 0.522E3 * cc ** 7 * dt ** 7 * dx ** 3 * dz ** 5 * dy ** 6 - 0.5
     #22E3 * cc ** 7 * dt ** 7 * dx ** 3 * dy ** 5 * dz ** 6 - 0.1044E4 
     #* cc ** 7 * dt ** 7 * dx ** 3 * dz ** 7 * dy ** 4 - 0.1566E4 * cc 
     #** 7 * dt ** 7 * dy ** 5 * dz ** 7 * dx ** 2 - 0.522E3 * cc ** 7 *
     # dt ** 7 * dx * dz ** 7 * dy ** 6 + 0.3600E4 * cc ** 6 * dt ** 6 *
     # dy ** 5 * dz ** 7 * dx ** 3 + 0.2400E4 * cc ** 5 * dt ** 5 * dy *
     #* 5 * dz ** 5 * dx ** 6 + 0.2400E4 * cc ** 5 * dt ** 5 * dy ** 3 *
     # dz ** 7 * dx ** 6 + 0.1200E4 * cc ** 5 * dt ** 5 * dx ** 5 * dz *
     #* 5 * dy ** 6 + 0.1200E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 *
     # dz ** 6 + 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 7 * dy *
     #* 4 + 0.13200E5 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 7 * dx ** 4 
     #+ 0.6000E4 * cc ** 5 * dt ** 5 * dx ** 3 * dz ** 7 * dy ** 6 - 0.6
     #000E4 * cc ** 4 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 5 - 0.18400E
     #5 * cc ** 3 * dt ** 3 * dy ** 5 * dz ** 7 * dx ** 6 - 0.8800E4 * c
     #c ** 3 * dt ** 3 * dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 
     #7 / dz ** 7 / 0.864000E6
      cuu(519) = (0.522E3 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 5 * dx *
     #* 4 + 0.261E3 * cc ** 7 * dt ** 7 * dx ** 3 * dz ** 5 * dy ** 6 + 
     #0.261E3 * cc ** 7 * dt ** 7 * dx ** 3 * dy ** 5 * dz ** 6 - 0.1200
     #E4 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 5 * dx ** 6 - 0.600E3 * c
     #c ** 5 * dt ** 5 * dx ** 5 * dz ** 5 * dy ** 6 - 0.600E3 * cc ** 5
     # * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz
     # ** 7 / 0.864000E6
      cuu(525) = (0.261E3 * cc ** 7 * dt ** 7 * dy ** 7 * dz ** 3 * dx *
     #* 4 + 0.261E3 * cc ** 7 * dt ** 7 * dx ** 3 * dy ** 7 * dz ** 4 - 
     #0.600E3 * cc ** 5 * dt ** 5 * dy ** 7 * dz ** 3 * dx ** 6 - 0.600E
     #3 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 7 * dz ** 4 - 0.600E3 * cc
     # ** 5 * dt ** 5 * dy ** 7 * dz ** 5 * dx ** 4 - 0.600E3 * cc ** 5 
     #* dt ** 5 * dx ** 3 * dy ** 7 * dz ** 6 + 0.1000E4 * cc ** 3 * dt 
     #** 3 * dy ** 7 * dz ** 5 * dx ** 6 + 0.1000E4 * cc ** 3 * dt ** 3 
     #* dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.8
     #64000E6
      cuu(526) = (-0.1044E4 * cc ** 7 * dt ** 7 * dy ** 7 * dz ** 3 * dx
     # ** 4 - 0.1044E4 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 5 * dx ** 4
     # - 0.1044E4 * cc ** 7 * dt ** 7 * dx ** 3 * dy ** 7 * dz ** 4 - 0.
     #522E3 * cc ** 7 * dt ** 7 * dx ** 3 * dz ** 5 * dy ** 6 - 0.522E3 
     #* cc ** 7 * dt ** 7 * dx ** 3 * dy ** 5 * dz ** 6 - 0.1566E4 * cc 
     #** 7 * dt ** 7 * dy ** 7 * dz ** 5 * dx ** 2 - 0.522E3 * cc ** 7 *
     # dt ** 7 * dx * dy ** 7 * dz ** 6 + 0.3600E4 * cc ** 6 * dt ** 6 *
     # dy ** 7 * dz ** 5 * dx ** 3 + 0.2400E4 * cc ** 5 * dt ** 5 * dy *
     #* 7 * dz ** 3 * dx ** 6 + 0.2400E4 * cc ** 5 * dt ** 5 * dy ** 5 *
     # dz ** 5 * dx ** 6 + 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy *
     #* 7 * dz ** 4 + 0.1200E4 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 5 *
     # dy ** 6 + 0.1200E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 * dz *
     #* 6 + 0.13200E5 * cc ** 5 * dt ** 5 * dy ** 7 * dz ** 5 * dx ** 4 
     #+ 0.6000E4 * cc ** 5 * dt ** 5 * dx ** 3 * dy ** 7 * dz ** 6 - 0.6
     #000E4 * cc ** 4 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 5 - 0.18400E
     #5 * cc ** 3 * dt ** 3 * dy ** 7 * dz ** 5 * dx ** 6 - 0.8800E4 * c
     #c ** 3 * dt ** 3 * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 
     #7 / dz ** 7 / 0.864000E6
      cuu(527) = (0.1566E4 * cc ** 7 * dt ** 7 * dy ** 7 * dz ** 3 * dx 
     #** 4 + 0.2088E4 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 5 * dx ** 4 
     #+ 0.1566E4 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 7 * dx ** 4 + 0.1
     #566E4 * cc ** 7 * dt ** 7 * dx ** 3 * dy ** 7 * dz ** 4 + 0.1044E4
     # * cc ** 7 * dt ** 7 * dx ** 3 * dz ** 5 * dy ** 6 + 0.1044E4 * cc
     # ** 7 * dt ** 7 * dx ** 3 * dy ** 5 * dz ** 6 + 0.1566E4 * cc ** 7
     # * dt ** 7 * dx ** 3 * dz ** 7 * dy ** 4 + 0.3132E4 * cc ** 7 * dt
     # ** 7 * dy ** 7 * dz ** 5 * dx ** 2 + 0.3132E4 * cc ** 7 * dt ** 7
     # * dy ** 5 * dz ** 7 * dx ** 2 + 0.1044E4 * cc ** 7 * dt ** 7 * dx
     # * dy ** 7 * dz ** 6 + 0.1044E4 * cc ** 7 * dt ** 7 * dx * dz ** 7
     # * dy ** 6 + 0.2436E4 * cc ** 7 * dt ** 7 * dy ** 7 * dz ** 7 - 0.
     #7200E4 * cc ** 6 * dt ** 6 * dy ** 7 * dz ** 5 * dx ** 3 - 0.7200E
     #4 * cc ** 6 * dt ** 6 * dy ** 5 * dz ** 7 * dx ** 3 - 0.7200E4 * c
     #c ** 6 * dt ** 6 * dy ** 7 * dz ** 7 * dx - 0.3600E4 * cc ** 5 * d
     #t ** 5 * dy ** 7 * dz ** 3 * dx ** 6 - 0.4800E4 * cc ** 5 * dt ** 
     #5 * dy ** 5 * dz ** 5 * dx ** 6 - 0.3600E4 * cc ** 5 * dt ** 5 * d
     #y ** 3 * dz ** 7 * dx ** 6 - 0.3600E4 * cc ** 5 * dt ** 5 * dx ** 
     #5 * dy ** 7 * dz ** 4 - 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 5 * d
     #z ** 5 * dy ** 6 - 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 
     #5 * dz ** 6 - 0.3600E4 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 7 * d
     #y ** 4 - 0.25200E5 * cc ** 5 * dt ** 5 * dy ** 7 * dz ** 5 * dx **
     # 4 - 0.25200E5 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 7 * dx ** 4 -
     # 0.10800E5 * cc ** 5 * dt ** 5 * dx ** 3 * dy ** 7 * dz ** 6 - 0.1
     #0800E5 * cc ** 5 * dt ** 5 * dx ** 3 * dz ** 7 * dy ** 6 - 0.33600
     #E5 * cc ** 5 * dt ** 5 * dy ** 7 * dz ** 7 * dx ** 2 + 0.12000E5 *
     # cc ** 4 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 5 + 0.12000E5 * cc 
     #** 4 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 5 + 0.72000E5 * cc ** 4
     # * dt ** 4 * dy ** 7 * dz ** 7 * dx ** 3 + 0.34800E5 * cc ** 3 * d
     #t ** 3 * dy ** 7 * dz ** 5 * dx ** 6 + 0.34800E5 * cc ** 3 * dt **
     # 3 * dy ** 5 * dz ** 7 * dx ** 6 + 0.15600E5 * cc ** 3 * dt ** 3 *
     # dx ** 5 * dy ** 7 * dz ** 6 + 0.15600E5 * cc ** 3 * dt ** 3 * dx 
     #** 5 * dz ** 7 * dy ** 6 + 0.117600E6 * cc ** 3 * dt ** 3 * dy ** 
     #7 * dz ** 7 * dx ** 4 - 0.64800E5 * cc ** 2 * dt ** 2 * dy ** 7 * 
     #dz ** 7 * dx ** 5) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuu(528) = (-0.1044E4 * cc ** 7 * dt ** 7 * dy ** 7 * dz ** 3 * dx
     # ** 4 - 0.1044E4 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 5 * dx ** 4
     # - 0.1044E4 * cc ** 7 * dt ** 7 * dx ** 3 * dy ** 7 * dz ** 4 - 0.
     #522E3 * cc ** 7 * dt ** 7 * dx ** 3 * dz ** 5 * dy ** 6 - 0.522E3 
     #* cc ** 7 * dt ** 7 * dx ** 3 * dy ** 5 * dz ** 6 - 0.1566E4 * cc 
     #** 7 * dt ** 7 * dy ** 7 * dz ** 5 * dx ** 2 - 0.522E3 * cc ** 7 *
     # dt ** 7 * dx * dy ** 7 * dz ** 6 + 0.3600E4 * cc ** 6 * dt ** 6 *
     # dy ** 7 * dz ** 5 * dx ** 3 + 0.2400E4 * cc ** 5 * dt ** 5 * dy *
     #* 7 * dz ** 3 * dx ** 6 + 0.2400E4 * cc ** 5 * dt ** 5 * dy ** 5 *
     # dz ** 5 * dx ** 6 + 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy *
     #* 7 * dz ** 4 + 0.1200E4 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 5 *
     # dy ** 6 + 0.1200E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 * dz *
     #* 6 + 0.13200E5 * cc ** 5 * dt ** 5 * dy ** 7 * dz ** 5 * dx ** 4 
     #+ 0.6000E4 * cc ** 5 * dt ** 5 * dx ** 3 * dy ** 7 * dz ** 6 - 0.6
     #000E4 * cc ** 4 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 5 - 0.18400E
     #5 * cc ** 3 * dt ** 3 * dy ** 7 * dz ** 5 * dx ** 6 - 0.8800E4 * c
     #c ** 3 * dt ** 3 * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 
     #7 / dz ** 7 / 0.864000E6
      cuu(529) = (0.261E3 * cc ** 7 * dt ** 7 * dy ** 7 * dz ** 3 * dx *
     #* 4 + 0.261E3 * cc ** 7 * dt ** 7 * dx ** 3 * dy ** 7 * dz ** 4 - 
     #0.600E3 * cc ** 5 * dt ** 5 * dy ** 7 * dz ** 3 * dx ** 6 - 0.600E
     #3 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 7 * dz ** 4 - 0.600E3 * cc
     # ** 5 * dt ** 5 * dy ** 7 * dz ** 5 * dx ** 4 - 0.600E3 * cc ** 5 
     #* dt ** 5 * dx ** 3 * dy ** 7 * dz ** 6 + 0.1000E4 * cc ** 3 * dt 
     #** 3 * dy ** 7 * dz ** 5 * dx ** 6 + 0.1000E4 * cc ** 3 * dt ** 3 
     #* dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.8
     #64000E6
      cuu(535) = (0.522E3 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 5 * dx *
     #* 4 + 0.261E3 * cc ** 7 * dt ** 7 * dx ** 3 * dz ** 5 * dy ** 6 + 
     #0.261E3 * cc ** 7 * dt ** 7 * dx ** 3 * dy ** 5 * dz ** 6 - 0.1200
     #E4 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 5 * dx ** 6 - 0.600E3 * c
     #c ** 5 * dt ** 5 * dx ** 5 * dz ** 5 * dy ** 6 - 0.600E3 * cc ** 5
     # * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz
     # ** 7 / 0.864000E6
      cuu(536) = (-0.1044E4 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 5 * dx
     # ** 4 - 0.1044E4 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 7 * dx ** 4
     # - 0.522E3 * cc ** 7 * dt ** 7 * dx ** 3 * dz ** 5 * dy ** 6 - 0.5
     #22E3 * cc ** 7 * dt ** 7 * dx ** 3 * dy ** 5 * dz ** 6 - 0.1044E4 
     #* cc ** 7 * dt ** 7 * dx ** 3 * dz ** 7 * dy ** 4 - 0.1566E4 * cc 
     #** 7 * dt ** 7 * dy ** 5 * dz ** 7 * dx ** 2 - 0.522E3 * cc ** 7 *
     # dt ** 7 * dx * dz ** 7 * dy ** 6 + 0.3600E4 * cc ** 6 * dt ** 6 *
     # dy ** 5 * dz ** 7 * dx ** 3 + 0.2400E4 * cc ** 5 * dt ** 5 * dy *
     #* 5 * dz ** 5 * dx ** 6 + 0.2400E4 * cc ** 5 * dt ** 5 * dy ** 3 *
     # dz ** 7 * dx ** 6 + 0.1200E4 * cc ** 5 * dt ** 5 * dx ** 5 * dz *
     #* 5 * dy ** 6 + 0.1200E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 *
     # dz ** 6 + 0.2400E4 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 7 * dy *
     #* 4 + 0.13200E5 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 7 * dx ** 4 
     #+ 0.6000E4 * cc ** 5 * dt ** 5 * dx ** 3 * dz ** 7 * dy ** 6 - 0.6
     #000E4 * cc ** 4 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 5 - 0.18400E
     #5 * cc ** 3 * dt ** 3 * dy ** 5 * dz ** 7 * dx ** 6 - 0.8800E4 * c
     #c ** 3 * dt ** 3 * dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 
     #7 / dz ** 7 / 0.864000E6
      cuu(537) = (0.522E3 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 5 * dx *
     #* 4 + 0.261E3 * cc ** 7 * dt ** 7 * dx ** 3 * dz ** 5 * dy ** 6 + 
     #0.261E3 * cc ** 7 * dt ** 7 * dx ** 3 * dy ** 5 * dz ** 6 - 0.1200
     #E4 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 5 * dx ** 6 - 0.600E3 * c
     #c ** 5 * dt ** 5 * dx ** 5 * dz ** 5 * dy ** 6 - 0.600E3 * cc ** 5
     # * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz
     # ** 7 / 0.864000E6
      cuu(545) = (0.261E3 * cc ** 7 * dt ** 7 * dy ** 3 * dz ** 7 * dx *
     #* 4 + 0.261E3 * cc ** 7 * dt ** 7 * dx ** 3 * dz ** 7 * dy ** 4 - 
     #0.600E3 * cc ** 5 * dt ** 5 * dy ** 3 * dz ** 7 * dx ** 6 - 0.600E
     #3 * cc ** 5 * dt ** 5 * dx ** 5 * dz ** 7 * dy ** 4 - 0.600E3 * cc
     # ** 5 * dt ** 5 * dy ** 5 * dz ** 7 * dx ** 4 - 0.600E3 * cc ** 5 
     #* dt ** 5 * dx ** 3 * dz ** 7 * dy ** 6 + 0.1000E4 * cc ** 3 * dt 
     #** 3 * dy ** 5 * dz ** 7 * dx ** 6 + 0.1000E4 * cc ** 3 * dt ** 3 
     #* dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.8
     #64000E6
      cuu(599) = (0.261E3 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 7 * dx *
     #* 2 + 0.87E2 * cc ** 7 * dt ** 7 * dx * dz ** 7 * dy ** 6 - 0.1800
     #E4 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 7 * dx ** 4 - 0.600E3 * c
     #c ** 5 * dt ** 5 * dx ** 3 * dz ** 7 * dy ** 6 + 0.2400E4 * cc ** 
     #3 * dt ** 3 * dy ** 5 * dz ** 7 * dx ** 6 + 0.800E3 * cc ** 3 * dt
     # ** 3 * dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7
     # / 0.864000E6
      cuu(607) = (0.261E3 * cc ** 7 * dt ** 7 * dy ** 7 * dz ** 5 * dx *
     #* 2 + 0.87E2 * cc ** 7 * dt ** 7 * dx * dy ** 7 * dz ** 6 - 0.1800
     #E4 * cc ** 5 * dt ** 5 * dy ** 7 * dz ** 5 * dx ** 4 - 0.600E3 * c
     #c ** 5 * dt ** 5 * dx ** 3 * dy ** 7 * dz ** 6 + 0.2400E4 * cc ** 
     #3 * dt ** 3 * dy ** 7 * dz ** 5 * dx ** 6 + 0.800E3 * cc ** 3 * dt
     # ** 3 * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7
     # / 0.864000E6
      cuu(608) = (-0.522E3 * cc ** 7 * dt ** 7 * dy ** 7 * dz ** 5 * dx 
     #** 2 - 0.522E3 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 7 * dx ** 2 -
     # 0.174E3 * cc ** 7 * dt ** 7 * dx * dy ** 7 * dz ** 6 - 0.174E3 * 
     #cc ** 7 * dt ** 7 * dx * dz ** 7 * dy ** 6 - 0.696E3 * cc ** 7 * d
     #t ** 7 * dy ** 7 * dz ** 7 + 0.1200E4 * cc ** 6 * dt ** 6 * dy ** 
     #7 * dz ** 7 * dx + 0.3600E4 * cc ** 5 * dt ** 5 * dy ** 7 * dz ** 
     #5 * dx ** 4 + 0.3600E4 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 7 * d
     #x ** 4 + 0.1200E4 * cc ** 5 * dt ** 5 * dx ** 3 * dy ** 7 * dz ** 
     #6 + 0.1200E4 * cc ** 5 * dt ** 5 * dx ** 3 * dz ** 7 * dy ** 6 + 0
     #.9600E4 * cc ** 5 * dt ** 5 * dy ** 7 * dz ** 7 * dx ** 2 - 0.6000
     #E4 * cc ** 4 * dt ** 4 * dy ** 7 * dz ** 7 * dx ** 3 - 0.4800E4 * 
     #cc ** 3 * dt ** 3 * dy ** 7 * dz ** 5 * dx ** 6 - 0.4800E4 * cc **
     # 3 * dt ** 3 * dy ** 5 * dz ** 7 * dx ** 6 - 0.1600E4 * cc ** 3 * 
     #dt ** 3 * dx ** 5 * dy ** 7 * dz ** 6 - 0.1600E4 * cc ** 3 * dt **
     # 3 * dx ** 5 * dz ** 7 * dy ** 6 - 0.33600E5 * cc ** 3 * dt ** 3 *
     # dy ** 7 * dz ** 7 * dx ** 4 + 0.4800E4 * cc ** 2 * dt ** 2 * dy *
     #* 7 * dz ** 7 * dx ** 5) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E
     #6
      cuu(609) = (0.261E3 * cc ** 7 * dt ** 7 * dy ** 7 * dz ** 5 * dx *
     #* 2 + 0.87E2 * cc ** 7 * dt ** 7 * dx * dy ** 7 * dz ** 6 - 0.1800
     #E4 * cc ** 5 * dt ** 5 * dy ** 7 * dz ** 5 * dx ** 4 - 0.600E3 * c
     #c ** 5 * dt ** 5 * dx ** 3 * dy ** 7 * dz ** 6 + 0.2400E4 * cc ** 
     #3 * dt ** 3 * dy ** 7 * dz ** 5 * dx ** 6 + 0.800E3 * cc ** 3 * dt
     # ** 3 * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7
     # / 0.864000E6
      cuu(617) = (0.261E3 * cc ** 7 * dt ** 7 * dy ** 5 * dz ** 7 * dx *
     #* 2 + 0.87E2 * cc ** 7 * dt ** 7 * dx * dz ** 7 * dy ** 6 - 0.1800
     #E4 * cc ** 5 * dt ** 5 * dy ** 5 * dz ** 7 * dx ** 4 - 0.600E3 * c
     #c ** 5 * dt ** 5 * dx ** 3 * dz ** 7 * dy ** 6 + 0.2400E4 * cc ** 
     #3 * dt ** 3 * dy ** 5 * dz ** 7 * dx ** 6 + 0.800E3 * cc ** 3 * dt
     # ** 3 * dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7
     # / 0.864000E6
      cuu(689) = (0.87E2 * cc ** 7 * dt ** 7 * dy ** 7 * dz ** 7 - 0.120
     #0E4 * cc ** 5 * dt ** 5 * dy ** 7 * dz ** 7 * dx ** 2 + 0.4200E4 *
     # cc ** 3 * dt ** 3 * dy ** 7 * dz ** 7 * dx ** 4) / dx ** 7 / dy *
     #* 7 / dz ** 7 / 0.864000E6

c
      return
      end
c
c++++++++++++++++
c
      subroutine getcuv_sixth3D( 
     *   cc,dx,dy,dz,dt,cuv )
c
      implicit real (t)
      real cuv(1:*)
      real dx,dy,dz,dt,cc
      integer i
c
      do i = 1,9**3
        cuv(i) = 0.0
      end do
c
      cuv(41) = (-0.150E3 * cc ** 5 * dt ** 6 * dy ** 7 * dz ** 7 * dx *
     #* 2 + 0.1325E4 * cc ** 3 * dt ** 4 * dy ** 7 * dz ** 7 * dx ** 4 -
     # 0.1600E4 * cc * dt ** 2 * dy ** 7 * dz ** 7 * dx ** 6) / dx ** 7 
     #/ dy ** 7 / dz ** 7 / 0.864000E6
      cuv(113) = (-0.300E3 * cc ** 5 * dt ** 6 * dy ** 5 * dz ** 7 * dx 
     #** 4 + 0.750E3 * cc ** 3 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 6 +
     # 0.200E3 * cc ** 3 * dt ** 4 * dx ** 5 * dz ** 7 * dy ** 6) / dx *
     #* 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(121) = (-0.300E3 * cc ** 5 * dt ** 6 * dy ** 7 * dz ** 5 * dx 
     #** 4 + 0.750E3 * cc ** 3 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 6 +
     # 0.200E3 * cc ** 3 * dt ** 4 * dx ** 5 * dy ** 7 * dz ** 6) / dx *
     #* 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(122) = (0.174E3 * cc ** 6 * dt ** 7 * dy ** 7 * dz ** 7 * dx +
     # 0.600E3 * cc ** 5 * dt ** 6 * dy ** 7 * dz ** 5 * dx ** 4 + 0.600
     #E3 * cc ** 5 * dt ** 6 * dy ** 5 * dz ** 7 * dx ** 4 + 0.1200E4 * 
     #cc ** 5 * dt ** 6 * dy ** 7 * dz ** 7 * dx ** 2 - 0.1050E4 * cc **
     # 4 * dt ** 5 * dy ** 7 * dz ** 7 * dx ** 3 - 0.1500E4 * cc ** 3 * 
     #dt ** 4 * dy ** 7 * dz ** 5 * dx ** 6 - 0.1500E4 * cc ** 3 * dt **
     # 4 * dy ** 5 * dz ** 7 * dx ** 6 - 0.400E3 * cc ** 3 * dt ** 4 * d
     #x ** 5 * dy ** 7 * dz ** 6 - 0.400E3 * cc ** 3 * dt ** 4 * dx ** 5
     # * dz ** 7 * dy ** 6 - 0.10600E5 * cc ** 3 * dt ** 4 * dy ** 7 * d
     #z ** 7 * dx ** 4 + 0.1600E4 * cc ** 2 * dt ** 3 * dy ** 7 * dz ** 
     #7 * dx ** 5 + 0.12800E5 * cc * dt ** 2 * dy ** 7 * dz ** 7 * dx **
     # 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(123) = (-0.300E3 * cc ** 5 * dt ** 6 * dy ** 7 * dz ** 5 * dx 
     #** 4 + 0.750E3 * cc ** 3 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 6 +
     # 0.200E3 * cc ** 3 * dt ** 4 * dx ** 5 * dy ** 7 * dz ** 6) / dx *
     #* 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(131) = (-0.300E3 * cc ** 5 * dt ** 6 * dy ** 5 * dz ** 7 * dx 
     #** 4 + 0.750E3 * cc ** 3 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 6 +
     # 0.200E3 * cc ** 3 * dt ** 4 * dx ** 5 * dz ** 7 * dy ** 6) / dx *
     #* 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(185) = (-0.150E3 * cc ** 5 * dt ** 6 * dy ** 3 * dz ** 7 * dx 
     #** 6 - 0.150E3 * cc ** 5 * dt ** 6 * dx ** 3 * dz ** 7 * dy ** 6 +
     # 0.375E3 * cc ** 3 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 6 + 0.375
     #E3 * cc ** 3 * dt ** 4 * dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / 
     #dy ** 7 / dz ** 7 / 0.864000E6
      cuv(193) = -cc ** 5 * dt ** 6 / dy ** 2 / dz ** 2 / dx / 0.2880E4
      cuv(194) = (0.522E3 * cc ** 6 * dt ** 7 * dy ** 5 * dz ** 7 * dx *
     #* 3 + 0.600E3 * cc ** 5 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 6 + 
     #0.600E3 * cc ** 5 * dt ** 6 * dy ** 3 * dz ** 7 * dx ** 6 + 0.1800
     #E4 * cc ** 5 * dt ** 6 * dy ** 5 * dz ** 7 * dx ** 4 + 0.600E3 * c
     #c ** 5 * dt ** 6 * dx ** 3 * dz ** 7 * dy ** 6 - 0.1050E4 * cc ** 
     #4 * dt ** 5 * dy ** 5 * dz ** 7 * dx ** 5 - 0.6000E4 * cc ** 3 * d
     #t ** 4 * dy ** 5 * dz ** 7 * dx ** 6 - 0.2700E4 * cc ** 3 * dt ** 
     #4 * dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0
     #.864000E6
      cuv(195) = -cc ** 5 * dt ** 6 / dy ** 2 / dz ** 2 / dx / 0.2880E4
      cuv(201) = (-0.150E3 * cc ** 5 * dt ** 6 * dy ** 7 * dz ** 3 * dx 
     #** 6 - 0.150E3 * cc ** 5 * dt ** 6 * dx ** 3 * dy ** 7 * dz ** 6 +
     # 0.375E3 * cc ** 3 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 6 + 0.375
     #E3 * cc ** 3 * dt ** 4 * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / 
     #dy ** 7 / dz ** 7 / 0.864000E6
      cuv(202) = (0.522E3 * cc ** 6 * dt ** 7 * dy ** 7 * dz ** 5 * dx *
     #* 3 + 0.600E3 * cc ** 5 * dt ** 6 * dy ** 7 * dz ** 3 * dx ** 6 + 
     #0.600E3 * cc ** 5 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 6 + 0.1800
     #E4 * cc ** 5 * dt ** 6 * dy ** 7 * dz ** 5 * dx ** 4 + 0.600E3 * c
     #c ** 5 * dt ** 6 * dx ** 3 * dy ** 7 * dz ** 6 - 0.1050E4 * cc ** 
     #4 * dt ** 5 * dy ** 7 * dz ** 5 * dx ** 5 - 0.6000E4 * cc ** 3 * d
     #t ** 4 * dy ** 7 * dz ** 5 * dx ** 6 - 0.2700E4 * cc ** 3 * dt ** 
     #4 * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0
     #.864000E6
      cuv(203) = (-0.1044E4 * cc ** 6 * dt ** 7 * dy ** 7 * dz ** 5 * dx
     # ** 3 - 0.1044E4 * cc ** 6 * dt ** 7 * dy ** 5 * dz ** 7 * dx ** 3
     # - 0.1044E4 * cc ** 6 * dt ** 7 * dy ** 7 * dz ** 7 * dx - 0.900E3
     # * cc ** 5 * dt ** 6 * dy ** 7 * dz ** 3 * dx ** 6 - 0.1200E4 * cc
     # ** 5 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 6 - 0.900E3 * cc ** 5 
     #* dt ** 6 * dy ** 3 * dz ** 7 * dx ** 6 - 0.3600E4 * cc ** 5 * dt 
     #** 6 * dy ** 7 * dz ** 5 * dx ** 4 - 0.3600E4 * cc ** 5 * dt ** 6 
     #* dy ** 5 * dz ** 7 * dx ** 4 - 0.900E3 * cc ** 5 * dt ** 6 * dx *
     #* 3 * dy ** 7 * dz ** 6 - 0.900E3 * cc ** 5 * dt ** 6 * dx ** 3 * 
     #dz ** 7 * dy ** 6 - 0.4200E4 * cc ** 5 * dt ** 6 * dy ** 7 * dz **
     # 7 * dx ** 2 + 0.2100E4 * cc ** 4 * dt ** 5 * dy ** 7 * dz ** 5 * 
     #dx ** 5 + 0.2100E4 * cc ** 4 * dt ** 5 * dy ** 5 * dz ** 7 * dx **
     # 5 + 0.13500E5 * cc ** 4 * dt ** 5 * dy ** 7 * dz ** 7 * dx ** 3 +
     # 0.11250E5 * cc ** 3 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 6 + 0.1
     #1250E5 * cc ** 3 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 6 + 0.4650E
     #4 * cc ** 3 * dt ** 4 * dx ** 5 * dy ** 7 * dz ** 6 + 0.4650E4 * c
     #c ** 3 * dt ** 4 * dx ** 5 * dz ** 7 * dy ** 6 + 0.37100E5 * cc **
     # 3 * dt ** 4 * dy ** 7 * dz ** 7 * dx ** 4 - 0.21600E5 * cc ** 2 *
     # dt ** 3 * dy ** 7 * dz ** 7 * dx ** 5 - 0.44800E5 * cc * dt ** 2 
     #* dy ** 7 * dz ** 7 * dx ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.8
     #64000E6
      cuv(204) = (0.522E3 * cc ** 6 * dt ** 7 * dy ** 7 * dz ** 5 * dx *
     #* 3 + 0.600E3 * cc ** 5 * dt ** 6 * dy ** 7 * dz ** 3 * dx ** 6 + 
     #0.600E3 * cc ** 5 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 6 + 0.1800
     #E4 * cc ** 5 * dt ** 6 * dy ** 7 * dz ** 5 * dx ** 4 + 0.600E3 * c
     #c ** 5 * dt ** 6 * dx ** 3 * dy ** 7 * dz ** 6 - 0.1050E4 * cc ** 
     #4 * dt ** 5 * dy ** 7 * dz ** 5 * dx ** 5 - 0.6000E4 * cc ** 3 * d
     #t ** 4 * dy ** 7 * dz ** 5 * dx ** 6 - 0.2700E4 * cc ** 3 * dt ** 
     #4 * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0
     #.864000E6
      cuv(205) = (-0.150E3 * cc ** 5 * dt ** 6 * dy ** 7 * dz ** 3 * dx 
     #** 6 - 0.150E3 * cc ** 5 * dt ** 6 * dx ** 3 * dy ** 7 * dz ** 6 +
     # 0.375E3 * cc ** 3 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 6 + 0.375
     #E3 * cc ** 3 * dt ** 4 * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / 
     #dy ** 7 / dz ** 7 / 0.864000E6
      cuv(211) = -cc ** 5 * dt ** 6 / dy ** 2 / dz ** 2 / dx / 0.2880E4
      cuv(212) = (0.522E3 * cc ** 6 * dt ** 7 * dy ** 5 * dz ** 7 * dx *
     #* 3 + 0.600E3 * cc ** 5 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 6 + 
     #0.600E3 * cc ** 5 * dt ** 6 * dy ** 3 * dz ** 7 * dx ** 6 + 0.1800
     #E4 * cc ** 5 * dt ** 6 * dy ** 5 * dz ** 7 * dx ** 4 + 0.600E3 * c
     #c ** 5 * dt ** 6 * dx ** 3 * dz ** 7 * dy ** 6 - 0.1050E4 * cc ** 
     #4 * dt ** 5 * dy ** 5 * dz ** 7 * dx ** 5 - 0.6000E4 * cc ** 3 * d
     #t ** 4 * dy ** 5 * dz ** 7 * dx ** 6 - 0.2700E4 * cc ** 3 * dt ** 
     #4 * dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0
     #.864000E6
      cuv(213) = -cc ** 5 * dt ** 6 / dy ** 2 / dz ** 2 / dx / 0.2880E4
      cuv(221) = (-0.150E3 * cc ** 5 * dt ** 6 * dy ** 3 * dz ** 7 * dx 
     #** 6 - 0.150E3 * cc ** 5 * dt ** 6 * dx ** 3 * dz ** 7 * dy ** 6 +
     # 0.375E3 * cc ** 3 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 6 + 0.375
     #E3 * cc ** 3 * dt ** 4 * dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / 
     #dy ** 7 / dz ** 7 / 0.864000E6
      cuv(257) = (-0.300E3 * cc ** 5 * dt ** 6 * dx ** 5 * dz ** 7 * dy 
     #** 4 + 0.200E3 * cc ** 3 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 6 +
     # 0.750E3 * cc ** 3 * dt ** 4 * dx ** 5 * dz ** 7 * dy ** 6) / dx *
     #* 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(265) = -cc ** 5 * dt ** 6 / dx ** 2 / dz ** 2 / dy / 0.2880E4
      cuv(266) = (0.522E3 * cc ** 6 * dt ** 7 * dy ** 3 * dz ** 7 * dx *
     #* 5 + 0.600E3 * cc ** 5 * dt ** 6 * dy ** 3 * dz ** 7 * dx ** 6 + 
     #0.600E3 * cc ** 5 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 6 + 0.1800
     #E4 * cc ** 5 * dt ** 6 * dx ** 5 * dz ** 7 * dy ** 4 + 0.600E3 * c
     #c ** 5 * dt ** 6 * dx ** 3 * dz ** 7 * dy ** 6 - 0.1050E4 * cc ** 
     #4 * dt ** 5 * dy ** 5 * dz ** 7 * dx ** 5 - 0.2700E4 * cc ** 3 * d
     #t ** 4 * dy ** 5 * dz ** 7 * dx ** 6 - 0.6000E4 * cc ** 3 * dt ** 
     #4 * dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0
     #.864000E6
      cuv(267) = -cc ** 5 * dt ** 6 / dx ** 2 / dz ** 2 / dy / 0.2880E4
      cuv(273) = -cc ** 5 * dt ** 6 / dx ** 2 / dy ** 2 / dz / 0.2880E4
      cuv(274) = (0.1044E4 * cc ** 6 * dt ** 7 * dy ** 5 * dz ** 5 * dx 
     #** 5 + 0.1200E4 * cc ** 5 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 6 
     #+ 0.1200E4 * cc ** 5 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 6 + 0.1
     #200E4 * cc ** 5 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7
     # / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(275) = (-0.2088E4 * cc ** 6 * dt ** 7 * dy ** 5 * dz ** 5 * dx
     # ** 5 - 0.2088E4 * cc ** 6 * dt ** 7 * dy ** 3 * dz ** 7 * dx ** 5
     # - 0.2088E4 * cc ** 6 * dt ** 7 * dy ** 5 * dz ** 7 * dx ** 3 - 0.
     #2400E4 * cc ** 5 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 6 - 0.2400E
     #4 * cc ** 5 * dt ** 6 * dy ** 3 * dz ** 7 * dx ** 6 - 0.2400E4 * c
     #c ** 5 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 6 - 0.1800E4 * cc ** 
     #5 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 6 - 0.4500E4 * cc ** 5 * d
     #t ** 6 * dx ** 5 * dz ** 7 * dy ** 4 - 0.4500E4 * cc ** 5 * dt ** 
     #6 * dy ** 5 * dz ** 7 * dx ** 4 - 0.2400E4 * cc ** 5 * dt ** 6 * d
     #x ** 3 * dz ** 7 * dy ** 6 + 0.22800E5 * cc ** 4 * dt ** 5 * dy **
     # 5 * dz ** 7 * dx ** 5 + 0.20250E5 * cc ** 3 * dt ** 4 * dy ** 5 *
     # dz ** 7 * dx ** 6 + 0.20250E5 * cc ** 3 * dt ** 4 * dx ** 5 * dz 
     #** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(276) = (0.1044E4 * cc ** 6 * dt ** 7 * dy ** 5 * dz ** 5 * dx 
     #** 5 + 0.1200E4 * cc ** 5 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 6 
     #+ 0.1200E4 * cc ** 5 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 6 + 0.1
     #200E4 * cc ** 5 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7
     # / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(277) = -cc ** 5 * dt ** 6 / dx ** 2 / dy ** 2 / dz / 0.2880E4
      cuv(281) = (-0.300E3 * cc ** 5 * dt ** 6 * dx ** 5 * dy ** 7 * dz 
     #** 4 + 0.200E3 * cc ** 3 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 6 +
     # 0.750E3 * cc ** 3 * dt ** 4 * dx ** 5 * dy ** 7 * dz ** 6) / dx *
     #* 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(282) = (0.522E3 * cc ** 6 * dt ** 7 * dy ** 7 * dz ** 3 * dx *
     #* 5 + 0.600E3 * cc ** 5 * dt ** 6 * dy ** 7 * dz ** 3 * dx ** 6 + 
     #0.1800E4 * cc ** 5 * dt ** 6 * dx ** 5 * dy ** 7 * dz ** 4 + 0.600
     #E3 * cc ** 5 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 6 + 0.600E3 * c
     #c ** 5 * dt ** 6 * dx ** 3 * dy ** 7 * dz ** 6 - 0.1050E4 * cc ** 
     #4 * dt ** 5 * dy ** 7 * dz ** 5 * dx ** 5 - 0.2700E4 * cc ** 3 * d
     #t ** 4 * dy ** 7 * dz ** 5 * dx ** 6 - 0.6000E4 * cc ** 3 * dt ** 
     #4 * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0
     #.864000E6
      cuv(283) = (-0.2088E4 * cc ** 6 * dt ** 7 * dy ** 7 * dz ** 3 * dx
     # ** 5 - 0.2088E4 * cc ** 6 * dt ** 7 * dy ** 5 * dz ** 5 * dx ** 5
     # - 0.2088E4 * cc ** 6 * dt ** 7 * dy ** 7 * dz ** 5 * dx ** 3 - 0.
     #2400E4 * cc ** 5 * dt ** 6 * dy ** 7 * dz ** 3 * dx ** 6 - 0.2400E
     #4 * cc ** 5 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 6 - 0.4500E4 * c
     #c ** 5 * dt ** 6 * dx ** 5 * dy ** 7 * dz ** 4 - 0.1800E4 * cc ** 
     #5 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 6 - 0.2400E4 * cc ** 5 * d
     #t ** 6 * dx ** 5 * dy ** 5 * dz ** 6 - 0.4500E4 * cc ** 5 * dt ** 
     #6 * dy ** 7 * dz ** 5 * dx ** 4 - 0.2400E4 * cc ** 5 * dt ** 6 * d
     #x ** 3 * dy ** 7 * dz ** 6 + 0.22800E5 * cc ** 4 * dt ** 5 * dy **
     # 7 * dz ** 5 * dx ** 5 + 0.20250E5 * cc ** 3 * dt ** 4 * dy ** 7 *
     # dz ** 5 * dx ** 6 + 0.20250E5 * cc ** 3 * dt ** 4 * dx ** 5 * dy 
     #** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(284) = (0.3132E4 * cc ** 6 * dt ** 7 * dy ** 7 * dz ** 3 * dx 
     #** 5 + 0.4176E4 * cc ** 6 * dt ** 7 * dy ** 5 * dz ** 5 * dx ** 5 
     #+ 0.3132E4 * cc ** 6 * dt ** 7 * dy ** 3 * dz ** 7 * dx ** 5 + 0.4
     #176E4 * cc ** 6 * dt ** 7 * dy ** 7 * dz ** 5 * dx ** 3 + 0.4176E4
     # * cc ** 6 * dt ** 7 * dy ** 5 * dz ** 7 * dx ** 3 + 0.2610E4 * cc
     # ** 6 * dt ** 7 * dy ** 7 * dz ** 7 * dx + 0.3600E4 * cc ** 5 * dt
     # ** 6 * dy ** 7 * dz ** 3 * dx ** 6 + 0.4800E4 * cc ** 5 * dt ** 6
     # * dy ** 5 * dz ** 5 * dx ** 6 + 0.3600E4 * cc ** 5 * dt ** 6 * dy
     # ** 3 * dz ** 7 * dx ** 6 + 0.6000E4 * cc ** 5 * dt ** 6 * dx ** 5
     # * dy ** 7 * dz ** 4 + 0.3600E4 * cc ** 5 * dt ** 6 * dx ** 5 * dz
     # ** 5 * dy ** 6 + 0.3600E4 * cc ** 5 * dt ** 6 * dx ** 5 * dy ** 5
     # * dz ** 6 + 0.6000E4 * cc ** 5 * dt ** 6 * dx ** 5 * dz ** 7 * dy
     # ** 4 + 0.9000E4 * cc ** 5 * dt ** 6 * dy ** 7 * dz ** 5 * dx ** 4
     # + 0.9000E4 * cc ** 5 * dt ** 6 * dy ** 5 * dz ** 7 * dx ** 4 + 0.
     #3600E4 * cc ** 5 * dt ** 6 * dx ** 3 * dy ** 7 * dz ** 6 + 0.3600E
     #4 * cc ** 5 * dt ** 6 * dx ** 3 * dz ** 7 * dy ** 6 + 0.8400E4 * c
     #c ** 5 * dt ** 6 * dy ** 7 * dz ** 7 * dx ** 2 - 0.43500E5 * cc **
     # 4 * dt ** 5 * dy ** 7 * dz ** 5 * dx ** 5 - 0.43500E5 * cc ** 4 *
     # dt ** 5 * dy ** 5 * dz ** 7 * dx ** 5 - 0.44550E5 * cc ** 4 * dt 
     #** 5 * dy ** 7 * dz ** 7 * dx ** 3 - 0.35500E5 * cc ** 3 * dt ** 4
     # * dy ** 7 * dz ** 5 * dx ** 6 - 0.35500E5 * cc ** 3 * dt ** 4 * d
     #y ** 5 * dz ** 7 * dx ** 6 - 0.30000E5 * cc ** 3 * dt ** 4 * dx **
     # 5 * dy ** 7 * dz ** 6 - 0.30000E5 * cc ** 3 * dt ** 4 * dx ** 5 *
     # dz ** 7 * dy ** 6 - 0.74200E5 * cc ** 3 * dt ** 4 * dy ** 7 * dz 
     #** 7 * dx ** 4 + 0.216000E6 * cc ** 2 * dt ** 3 * dy ** 7 * dz ** 
     #7 * dx ** 5 + 0.89600E5 * cc * dt ** 2 * dy ** 7 * dz ** 7 * dx **
     # 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(285) = (-0.2088E4 * cc ** 6 * dt ** 7 * dy ** 7 * dz ** 3 * dx
     # ** 5 - 0.2088E4 * cc ** 6 * dt ** 7 * dy ** 5 * dz ** 5 * dx ** 5
     # - 0.2088E4 * cc ** 6 * dt ** 7 * dy ** 7 * dz ** 5 * dx ** 3 - 0.
     #2400E4 * cc ** 5 * dt ** 6 * dy ** 7 * dz ** 3 * dx ** 6 - 0.2400E
     #4 * cc ** 5 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 6 - 0.4500E4 * c
     #c ** 5 * dt ** 6 * dx ** 5 * dy ** 7 * dz ** 4 - 0.1800E4 * cc ** 
     #5 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 6 - 0.2400E4 * cc ** 5 * d
     #t ** 6 * dx ** 5 * dy ** 5 * dz ** 6 - 0.4500E4 * cc ** 5 * dt ** 
     #6 * dy ** 7 * dz ** 5 * dx ** 4 - 0.2400E4 * cc ** 5 * dt ** 6 * d
     #x ** 3 * dy ** 7 * dz ** 6 + 0.22800E5 * cc ** 4 * dt ** 5 * dy **
     # 7 * dz ** 5 * dx ** 5 + 0.20250E5 * cc ** 3 * dt ** 4 * dy ** 7 *
     # dz ** 5 * dx ** 6 + 0.20250E5 * cc ** 3 * dt ** 4 * dx ** 5 * dy 
     #** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(286) = (0.522E3 * cc ** 6 * dt ** 7 * dy ** 7 * dz ** 3 * dx *
     #* 5 + 0.600E3 * cc ** 5 * dt ** 6 * dy ** 7 * dz ** 3 * dx ** 6 + 
     #0.1800E4 * cc ** 5 * dt ** 6 * dx ** 5 * dy ** 7 * dz ** 4 + 0.600
     #E3 * cc ** 5 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 6 + 0.600E3 * c
     #c ** 5 * dt ** 6 * dx ** 3 * dy ** 7 * dz ** 6 - 0.1050E4 * cc ** 
     #4 * dt ** 5 * dy ** 7 * dz ** 5 * dx ** 5 - 0.2700E4 * cc ** 3 * d
     #t ** 4 * dy ** 7 * dz ** 5 * dx ** 6 - 0.6000E4 * cc ** 3 * dt ** 
     #4 * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0
     #.864000E6
      cuv(287) = (-0.300E3 * cc ** 5 * dt ** 6 * dx ** 5 * dy ** 7 * dz 
     #** 4 + 0.200E3 * cc ** 3 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 6 +
     # 0.750E3 * cc ** 3 * dt ** 4 * dx ** 5 * dy ** 7 * dz ** 6) / dx *
     #* 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(291) = -cc ** 5 * dt ** 6 / dx ** 2 / dy ** 2 / dz / 0.2880E4
      cuv(292) = (0.1044E4 * cc ** 6 * dt ** 7 * dy ** 5 * dz ** 5 * dx 
     #** 5 + 0.1200E4 * cc ** 5 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 6 
     #+ 0.1200E4 * cc ** 5 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 6 + 0.1
     #200E4 * cc ** 5 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7
     # / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(293) = (-0.2088E4 * cc ** 6 * dt ** 7 * dy ** 5 * dz ** 5 * dx
     # ** 5 - 0.2088E4 * cc ** 6 * dt ** 7 * dy ** 3 * dz ** 7 * dx ** 5
     # - 0.2088E4 * cc ** 6 * dt ** 7 * dy ** 5 * dz ** 7 * dx ** 3 - 0.
     #2400E4 * cc ** 5 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 6 - 0.2400E
     #4 * cc ** 5 * dt ** 6 * dy ** 3 * dz ** 7 * dx ** 6 - 0.2400E4 * c
     #c ** 5 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 6 - 0.1800E4 * cc ** 
     #5 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 6 - 0.4500E4 * cc ** 5 * d
     #t ** 6 * dx ** 5 * dz ** 7 * dy ** 4 - 0.4500E4 * cc ** 5 * dt ** 
     #6 * dy ** 5 * dz ** 7 * dx ** 4 - 0.2400E4 * cc ** 5 * dt ** 6 * d
     #x ** 3 * dz ** 7 * dy ** 6 + 0.22800E5 * cc ** 4 * dt ** 5 * dy **
     # 5 * dz ** 7 * dx ** 5 + 0.20250E5 * cc ** 3 * dt ** 4 * dy ** 5 *
     # dz ** 7 * dx ** 6 + 0.20250E5 * cc ** 3 * dt ** 4 * dx ** 5 * dz 
     #** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(294) = (0.1044E4 * cc ** 6 * dt ** 7 * dy ** 5 * dz ** 5 * dx 
     #** 5 + 0.1200E4 * cc ** 5 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 6 
     #+ 0.1200E4 * cc ** 5 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 6 + 0.1
     #200E4 * cc ** 5 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7
     # / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(295) = -cc ** 5 * dt ** 6 / dx ** 2 / dy ** 2 / dz / 0.2880E4
      cuv(301) = -cc ** 5 * dt ** 6 / dx ** 2 / dz ** 2 / dy / 0.2880E4
      cuv(302) = (0.522E3 * cc ** 6 * dt ** 7 * dy ** 3 * dz ** 7 * dx *
     #* 5 + 0.600E3 * cc ** 5 * dt ** 6 * dy ** 3 * dz ** 7 * dx ** 6 + 
     #0.600E3 * cc ** 5 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 6 + 0.1800
     #E4 * cc ** 5 * dt ** 6 * dx ** 5 * dz ** 7 * dy ** 4 + 0.600E3 * c
     #c ** 5 * dt ** 6 * dx ** 3 * dz ** 7 * dy ** 6 - 0.1050E4 * cc ** 
     #4 * dt ** 5 * dy ** 5 * dz ** 7 * dx ** 5 - 0.2700E4 * cc ** 3 * d
     #t ** 4 * dy ** 5 * dz ** 7 * dx ** 6 - 0.6000E4 * cc ** 3 * dt ** 
     #4 * dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0
     #.864000E6
      cuv(303) = -cc ** 5 * dt ** 6 / dx ** 2 / dz ** 2 / dy / 0.2880E4
      cuv(311) = (-0.300E3 * cc ** 5 * dt ** 6 * dx ** 5 * dz ** 7 * dy 
     #** 4 + 0.200E3 * cc ** 3 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 6 +
     # 0.750E3 * cc ** 3 * dt ** 4 * dx ** 5 * dz ** 7 * dy ** 6) / dx *
     #* 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(329) = (-0.150E3 * cc ** 5 * dt ** 6 * dx ** 7 * dz ** 7 * dy 
     #** 2 + 0.1325E4 * cc ** 3 * dt ** 4 * dx ** 7 * dz ** 7 * dy ** 4 
     #- 0.1600E4 * cc * dt ** 2 * dx ** 7 * dz ** 7 * dy ** 6) / dx ** 7
     # / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(337) = (-0.300E3 * cc ** 5 * dt ** 6 * dx ** 7 * dz ** 5 * dy 
     #** 4 + 0.750E3 * cc ** 3 * dt ** 4 * dx ** 7 * dz ** 5 * dy ** 6 +
     # 0.200E3 * cc ** 3 * dt ** 4 * dx ** 7 * dy ** 5 * dz ** 6) / dx *
     #* 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(338) = (0.174E3 * cc ** 6 * dt ** 7 * dx ** 7 * dz ** 7 * dy +
     # 0.600E3 * cc ** 5 * dt ** 6 * dx ** 7 * dz ** 5 * dy ** 4 + 0.120
     #0E4 * cc ** 5 * dt ** 6 * dx ** 7 * dz ** 7 * dy ** 2 + 0.600E3 * 
     #cc ** 5 * dt ** 6 * dx ** 5 * dz ** 7 * dy ** 4 - 0.1050E4 * cc **
     # 4 * dt ** 5 * dx ** 7 * dz ** 7 * dy ** 3 - 0.1500E4 * cc ** 3 * 
     #dt ** 4 * dx ** 7 * dz ** 5 * dy ** 6 - 0.400E3 * cc ** 3 * dt ** 
     #4 * dx ** 7 * dy ** 5 * dz ** 6 - 0.10600E5 * cc ** 3 * dt ** 4 * 
     #dx ** 7 * dz ** 7 * dy ** 4 - 0.400E3 * cc ** 3 * dt ** 4 * dy ** 
     #5 * dz ** 7 * dx ** 6 - 0.1500E4 * cc ** 3 * dt ** 4 * dx ** 5 * d
     #z ** 7 * dy ** 6 + 0.1600E4 * cc ** 2 * dt ** 3 * dx ** 7 * dz ** 
     #7 * dy ** 5 + 0.12800E5 * cc * dt ** 2 * dx ** 7 * dz ** 7 * dy **
     # 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(339) = (-0.300E3 * cc ** 5 * dt ** 6 * dx ** 7 * dz ** 5 * dy 
     #** 4 + 0.750E3 * cc ** 3 * dt ** 4 * dx ** 7 * dz ** 5 * dy ** 6 +
     # 0.200E3 * cc ** 3 * dt ** 4 * dx ** 7 * dy ** 5 * dz ** 6) / dx *
     #* 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(345) = (-0.150E3 * cc ** 5 * dt ** 6 * dx ** 7 * dz ** 3 * dy 
     #** 6 - 0.150E3 * cc ** 5 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 6 +
     # 0.375E3 * cc ** 3 * dt ** 4 * dx ** 7 * dz ** 5 * dy ** 6 + 0.375
     #E3 * cc ** 3 * dt ** 4 * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / 
     #dy ** 7 / dz ** 7 / 0.864000E6
      cuv(346) = (0.522E3 * cc ** 6 * dt ** 7 * dx ** 7 * dz ** 5 * dy *
     #* 3 + 0.600E3 * cc ** 5 * dt ** 6 * dx ** 7 * dz ** 3 * dy ** 6 + 
     #0.1800E4 * cc ** 5 * dt ** 6 * dx ** 7 * dz ** 5 * dy ** 4 + 0.600
     #E3 * cc ** 5 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 6 + 0.600E3 * c
     #c ** 5 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 6 - 0.1050E4 * cc ** 
     #4 * dt ** 5 * dx ** 7 * dz ** 5 * dy ** 5 - 0.6000E4 * cc ** 3 * d
     #t ** 4 * dx ** 7 * dz ** 5 * dy ** 6 - 0.2700E4 * cc ** 3 * dt ** 
     #4 * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0
     #.864000E6
      cuv(347) = (-0.1044E4 * cc ** 6 * dt ** 7 * dx ** 7 * dz ** 5 * dy
     # ** 3 - 0.1044E4 * cc ** 6 * dt ** 7 * dx ** 7 * dz ** 7 * dy - 0.
     #1044E4 * cc ** 6 * dt ** 7 * dy ** 3 * dz ** 7 * dx ** 5 - 0.900E3
     # * cc ** 5 * dt ** 6 * dx ** 7 * dz ** 3 * dy ** 6 - 0.3600E4 * cc
     # ** 5 * dt ** 6 * dx ** 7 * dz ** 5 * dy ** 4 - 0.900E3 * cc ** 5 
     #* dt ** 6 * dx ** 7 * dy ** 3 * dz ** 6 - 0.4200E4 * cc ** 5 * dt 
     #** 6 * dx ** 7 * dz ** 7 * dy ** 2 - 0.900E3 * cc ** 5 * dt ** 6 *
     # dy ** 3 * dz ** 7 * dx ** 6 - 0.1200E4 * cc ** 5 * dt ** 6 * dx *
     #* 5 * dz ** 5 * dy ** 6 - 0.3600E4 * cc ** 5 * dt ** 6 * dx ** 5 *
     # dz ** 7 * dy ** 4 - 0.900E3 * cc ** 5 * dt ** 6 * dx ** 3 * dz **
     # 7 * dy ** 6 + 0.2100E4 * cc ** 4 * dt ** 5 * dx ** 7 * dz ** 5 * 
     #dy ** 5 + 0.13500E5 * cc ** 4 * dt ** 5 * dx ** 7 * dz ** 7 * dy *
     #* 3 + 0.2100E4 * cc ** 4 * dt ** 5 * dy ** 5 * dz ** 7 * dx ** 5 +
     # 0.11250E5 * cc ** 3 * dt ** 4 * dx ** 7 * dz ** 5 * dy ** 6 + 0.4
     #650E4 * cc ** 3 * dt ** 4 * dx ** 7 * dy ** 5 * dz ** 6 + 0.37100E
     #5 * cc ** 3 * dt ** 4 * dx ** 7 * dz ** 7 * dy ** 4 + 0.4650E4 * c
     #c ** 3 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 6 + 0.11250E5 * cc **
     # 3 * dt ** 4 * dx ** 5 * dz ** 7 * dy ** 6 - 0.21600E5 * cc ** 2 *
     # dt ** 3 * dx ** 7 * dz ** 7 * dy ** 5 - 0.44800E5 * cc * dt ** 2 
     #* dx ** 7 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.8
     #64000E6
      cuv(348) = (0.522E3 * cc ** 6 * dt ** 7 * dx ** 7 * dz ** 5 * dy *
     #* 3 + 0.600E3 * cc ** 5 * dt ** 6 * dx ** 7 * dz ** 3 * dy ** 6 + 
     #0.1800E4 * cc ** 5 * dt ** 6 * dx ** 7 * dz ** 5 * dy ** 4 + 0.600
     #E3 * cc ** 5 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 6 + 0.600E3 * c
     #c ** 5 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 6 - 0.1050E4 * cc ** 
     #4 * dt ** 5 * dx ** 7 * dz ** 5 * dy ** 5 - 0.6000E4 * cc ** 3 * d
     #t ** 4 * dx ** 7 * dz ** 5 * dy ** 6 - 0.2700E4 * cc ** 3 * dt ** 
     #4 * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0
     #.864000E6
      cuv(349) = (-0.150E3 * cc ** 5 * dt ** 6 * dx ** 7 * dz ** 3 * dy 
     #** 6 - 0.150E3 * cc ** 5 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 6 +
     # 0.375E3 * cc ** 3 * dt ** 4 * dx ** 7 * dz ** 5 * dy ** 6 + 0.375
     #E3 * cc ** 3 * dt ** 4 * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / 
     #dy ** 7 / dz ** 7 / 0.864000E6
      cuv(353) = (-0.300E3 * cc ** 5 * dt ** 6 * dx ** 7 * dy ** 5 * dz 
     #** 4 + 0.200E3 * cc ** 3 * dt ** 4 * dx ** 7 * dz ** 5 * dy ** 6 +
     # 0.750E3 * cc ** 3 * dt ** 4 * dx ** 7 * dy ** 5 * dz ** 6) / dx *
     #* 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(354) = (0.522E3 * cc ** 6 * dt ** 7 * dx ** 7 * dz ** 3 * dy *
     #* 5 + 0.600E3 * cc ** 5 * dt ** 6 * dx ** 7 * dz ** 3 * dy ** 6 + 
     #0.1800E4 * cc ** 5 * dt ** 6 * dx ** 7 * dy ** 5 * dz ** 4 + 0.600
     #E3 * cc ** 5 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 6 + 0.600E3 * c
     #c ** 5 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 6 - 0.1050E4 * cc ** 
     #4 * dt ** 5 * dx ** 7 * dz ** 5 * dy ** 5 - 0.2700E4 * cc ** 3 * d
     #t ** 4 * dx ** 7 * dz ** 5 * dy ** 6 - 0.6000E4 * cc ** 3 * dt ** 
     #4 * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0
     #.864000E6
      cuv(355) = (-0.2088E4 * cc ** 6 * dt ** 7 * dx ** 7 * dz ** 3 * dy
     # ** 5 - 0.2088E4 * cc ** 6 * dt ** 7 * dx ** 7 * dz ** 5 * dy ** 3
     # - 0.2088E4 * cc ** 6 * dt ** 7 * dy ** 5 * dz ** 5 * dx ** 5 - 0.
     #2400E4 * cc ** 5 * dt ** 6 * dx ** 7 * dz ** 3 * dy ** 6 - 0.4500E
     #4 * cc ** 5 * dt ** 6 * dx ** 7 * dy ** 5 * dz ** 4 - 0.4500E4 * c
     #c ** 5 * dt ** 6 * dx ** 7 * dz ** 5 * dy ** 4 - 0.2400E4 * cc ** 
     #5 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 6 - 0.1800E4 * cc ** 5 * d
     #t ** 6 * dy ** 5 * dz ** 5 * dx ** 6 - 0.2400E4 * cc ** 5 * dt ** 
     #6 * dx ** 5 * dz ** 5 * dy ** 6 - 0.2400E4 * cc ** 5 * dt ** 6 * d
     #x ** 5 * dy ** 5 * dz ** 6 + 0.22800E5 * cc ** 4 * dt ** 5 * dx **
     # 7 * dz ** 5 * dy ** 5 + 0.20250E5 * cc ** 3 * dt ** 4 * dx ** 7 *
     # dz ** 5 * dy ** 6 + 0.20250E5 * cc ** 3 * dt ** 4 * dx ** 7 * dy 
     #** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(356) = (0.3132E4 * cc ** 6 * dt ** 7 * dx ** 7 * dz ** 3 * dy 
     #** 5 + 0.4176E4 * cc ** 6 * dt ** 7 * dx ** 7 * dz ** 5 * dy ** 3 
     #+ 0.2610E4 * cc ** 6 * dt ** 7 * dx ** 7 * dz ** 7 * dy + 0.4176E4
     # * cc ** 6 * dt ** 7 * dy ** 5 * dz ** 5 * dx ** 5 + 0.4176E4 * cc
     # ** 6 * dt ** 7 * dy ** 3 * dz ** 7 * dx ** 5 + 0.3132E4 * cc ** 6
     # * dt ** 7 * dy ** 5 * dz ** 7 * dx ** 3 + 0.3600E4 * cc ** 5 * dt
     # ** 6 * dx ** 7 * dz ** 3 * dy ** 6 + 0.6000E4 * cc ** 5 * dt ** 6
     # * dx ** 7 * dy ** 5 * dz ** 4 + 0.9000E4 * cc ** 5 * dt ** 6 * dx
     # ** 7 * dz ** 5 * dy ** 4 + 0.3600E4 * cc ** 5 * dt ** 6 * dx ** 7
     # * dy ** 3 * dz ** 6 + 0.8400E4 * cc ** 5 * dt ** 6 * dx ** 7 * dz
     # ** 7 * dy ** 2 + 0.3600E4 * cc ** 5 * dt ** 6 * dy ** 5 * dz ** 5
     # * dx ** 6 + 0.3600E4 * cc ** 5 * dt ** 6 * dy ** 3 * dz ** 7 * dx
     # ** 6 + 0.4800E4 * cc ** 5 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 6
     # + 0.3600E4 * cc ** 5 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 6 + 0.
     #9000E4 * cc ** 5 * dt ** 6 * dx ** 5 * dz ** 7 * dy ** 4 + 0.6000E
     #4 * cc ** 5 * dt ** 6 * dy ** 5 * dz ** 7 * dx ** 4 + 0.3600E4 * c
     #c ** 5 * dt ** 6 * dx ** 3 * dz ** 7 * dy ** 6 - 0.43500E5 * cc **
     # 4 * dt ** 5 * dx ** 7 * dz ** 5 * dy ** 5 - 0.44550E5 * cc ** 4 *
     # dt ** 5 * dx ** 7 * dz ** 7 * dy ** 3 - 0.43500E5 * cc ** 4 * dt 
     #** 5 * dy ** 5 * dz ** 7 * dx ** 5 - 0.35500E5 * cc ** 3 * dt ** 4
     # * dx ** 7 * dz ** 5 * dy ** 6 - 0.30000E5 * cc ** 3 * dt ** 4 * d
     #x ** 7 * dy ** 5 * dz ** 6 - 0.74200E5 * cc ** 3 * dt ** 4 * dx **
     # 7 * dz ** 7 * dy ** 4 - 0.30000E5 * cc ** 3 * dt ** 4 * dy ** 5 *
     # dz ** 7 * dx ** 6 - 0.35500E5 * cc ** 3 * dt ** 4 * dx ** 5 * dz 
     #** 7 * dy ** 6 + 0.216000E6 * cc ** 2 * dt ** 3 * dx ** 7 * dz ** 
     #7 * dy ** 5 + 0.89600E5 * cc * dt ** 2 * dx ** 7 * dz ** 7 * dy **
     # 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(357) = (-0.2088E4 * cc ** 6 * dt ** 7 * dx ** 7 * dz ** 3 * dy
     # ** 5 - 0.2088E4 * cc ** 6 * dt ** 7 * dx ** 7 * dz ** 5 * dy ** 3
     # - 0.2088E4 * cc ** 6 * dt ** 7 * dy ** 5 * dz ** 5 * dx ** 5 - 0.
     #2400E4 * cc ** 5 * dt ** 6 * dx ** 7 * dz ** 3 * dy ** 6 - 0.4500E
     #4 * cc ** 5 * dt ** 6 * dx ** 7 * dy ** 5 * dz ** 4 - 0.4500E4 * c
     #c ** 5 * dt ** 6 * dx ** 7 * dz ** 5 * dy ** 4 - 0.2400E4 * cc ** 
     #5 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 6 - 0.1800E4 * cc ** 5 * d
     #t ** 6 * dy ** 5 * dz ** 5 * dx ** 6 - 0.2400E4 * cc ** 5 * dt ** 
     #6 * dx ** 5 * dz ** 5 * dy ** 6 - 0.2400E4 * cc ** 5 * dt ** 6 * d
     #x ** 5 * dy ** 5 * dz ** 6 + 0.22800E5 * cc ** 4 * dt ** 5 * dx **
     # 7 * dz ** 5 * dy ** 5 + 0.20250E5 * cc ** 3 * dt ** 4 * dx ** 7 *
     # dz ** 5 * dy ** 6 + 0.20250E5 * cc ** 3 * dt ** 4 * dx ** 7 * dy 
     #** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(358) = (0.522E3 * cc ** 6 * dt ** 7 * dx ** 7 * dz ** 3 * dy *
     #* 5 + 0.600E3 * cc ** 5 * dt ** 6 * dx ** 7 * dz ** 3 * dy ** 6 + 
     #0.1800E4 * cc ** 5 * dt ** 6 * dx ** 7 * dy ** 5 * dz ** 4 + 0.600
     #E3 * cc ** 5 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 6 + 0.600E3 * c
     #c ** 5 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 6 - 0.1050E4 * cc ** 
     #4 * dt ** 5 * dx ** 7 * dz ** 5 * dy ** 5 - 0.2700E4 * cc ** 3 * d
     #t ** 4 * dx ** 7 * dz ** 5 * dy ** 6 - 0.6000E4 * cc ** 3 * dt ** 
     #4 * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0
     #.864000E6
      cuv(359) = (-0.300E3 * cc ** 5 * dt ** 6 * dx ** 7 * dy ** 5 * dz 
     #** 4 + 0.200E3 * cc ** 3 * dt ** 4 * dx ** 7 * dz ** 5 * dy ** 6 +
     # 0.750E3 * cc ** 3 * dt ** 4 * dx ** 7 * dy ** 5 * dz ** 6) / dx *
     #* 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(361) = (-0.150E3 * cc ** 5 * dt ** 6 * dx ** 7 * dy ** 7 * dz 
     #** 2 + 0.1325E4 * cc ** 3 * dt ** 4 * dx ** 7 * dy ** 7 * dz ** 4 
     #- 0.1600E4 * cc * dt ** 2 * dx ** 7 * dy ** 7 * dz ** 6) / dx ** 7
     # / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(362) = (0.174E3 * cc ** 6 * dt ** 7 * dx ** 7 * dy ** 7 * dz +
     # 0.1200E4 * cc ** 5 * dt ** 6 * dx ** 7 * dy ** 7 * dz ** 2 + 0.60
     #0E3 * cc ** 5 * dt ** 6 * dx ** 7 * dy ** 5 * dz ** 4 + 0.600E3 * 
     #cc ** 5 * dt ** 6 * dx ** 5 * dy ** 7 * dz ** 4 - 0.1050E4 * cc **
     # 4 * dt ** 5 * dx ** 7 * dy ** 7 * dz ** 3 - 0.10600E5 * cc ** 3 *
     # dt ** 4 * dx ** 7 * dy ** 7 * dz ** 4 - 0.400E3 * cc ** 3 * dt **
     # 4 * dx ** 7 * dz ** 5 * dy ** 6 - 0.1500E4 * cc ** 3 * dt ** 4 * 
     #dx ** 7 * dy ** 5 * dz ** 6 - 0.400E3 * cc ** 3 * dt ** 4 * dy ** 
     #7 * dz ** 5 * dx ** 6 - 0.1500E4 * cc ** 3 * dt ** 4 * dx ** 5 * d
     #y ** 7 * dz ** 6 + 0.1600E4 * cc ** 2 * dt ** 3 * dx ** 7 * dy ** 
     #7 * dz ** 5 + 0.12800E5 * cc * dt ** 2 * dx ** 7 * dy ** 7 * dz **
     # 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(363) = (-0.1044E4 * cc ** 6 * dt ** 7 * dx ** 7 * dy ** 7 * dz
     # - 0.1044E4 * cc ** 6 * dt ** 7 * dx ** 7 * dz ** 3 * dy ** 5 - 0.
     #1044E4 * cc ** 6 * dt ** 7 * dy ** 7 * dz ** 3 * dx ** 5 - 0.4200E
     #4 * cc ** 5 * dt ** 6 * dx ** 7 * dy ** 7 * dz ** 2 - 0.900E3 * cc
     # ** 5 * dt ** 6 * dx ** 7 * dz ** 3 * dy ** 6 - 0.3600E4 * cc ** 5
     # * dt ** 6 * dx ** 7 * dy ** 5 * dz ** 4 - 0.900E3 * cc ** 5 * dt 
     #** 6 * dx ** 7 * dy ** 3 * dz ** 6 - 0.900E3 * cc ** 5 * dt ** 6 *
     # dy ** 7 * dz ** 3 * dx ** 6 - 0.3600E4 * cc ** 5 * dt ** 6 * dx *
     #* 5 * dy ** 7 * dz ** 4 - 0.1200E4 * cc ** 5 * dt ** 6 * dx ** 5 *
     # dy ** 5 * dz ** 6 - 0.900E3 * cc ** 5 * dt ** 6 * dx ** 3 * dy **
     # 7 * dz ** 6 + 0.13500E5 * cc ** 4 * dt ** 5 * dx ** 7 * dy ** 7 *
     # dz ** 3 + 0.2100E4 * cc ** 4 * dt ** 5 * dx ** 7 * dz ** 5 * dy *
     #* 5 + 0.2100E4 * cc ** 4 * dt ** 5 * dy ** 7 * dz ** 5 * dx ** 5 +
     # 0.37100E5 * cc ** 3 * dt ** 4 * dx ** 7 * dy ** 7 * dz ** 4 + 0.4
     #650E4 * cc ** 3 * dt ** 4 * dx ** 7 * dz ** 5 * dy ** 6 + 0.11250E
     #5 * cc ** 3 * dt ** 4 * dx ** 7 * dy ** 5 * dz ** 6 + 0.4650E4 * c
     #c ** 3 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 6 + 0.11250E5 * cc **
     # 3 * dt ** 4 * dx ** 5 * dy ** 7 * dz ** 6 - 0.21600E5 * cc ** 2 *
     # dt ** 3 * dx ** 7 * dy ** 7 * dz ** 5 - 0.44800E5 * cc * dt ** 2 
     #* dx ** 7 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.8
     #64000E6
      cuv(364) = (0.2610E4 * cc ** 6 * dt ** 7 * dx ** 7 * dy ** 7 * dz 
     #+ 0.4176E4 * cc ** 6 * dt ** 7 * dx ** 7 * dz ** 3 * dy ** 5 + 0.3
     #132E4 * cc ** 6 * dt ** 7 * dx ** 7 * dz ** 5 * dy ** 3 + 0.4176E4
     # * cc ** 6 * dt ** 7 * dy ** 7 * dz ** 3 * dx ** 5 + 0.4176E4 * cc
     # ** 6 * dt ** 7 * dy ** 5 * dz ** 5 * dx ** 5 + 0.3132E4 * cc ** 6
     # * dt ** 7 * dy ** 7 * dz ** 5 * dx ** 3 + 0.8400E4 * cc ** 5 * dt
     # ** 6 * dx ** 7 * dy ** 7 * dz ** 2 + 0.3600E4 * cc ** 5 * dt ** 6
     # * dx ** 7 * dz ** 3 * dy ** 6 + 0.9000E4 * cc ** 5 * dt ** 6 * dx
     # ** 7 * dy ** 5 * dz ** 4 + 0.6000E4 * cc ** 5 * dt ** 6 * dx ** 7
     # * dz ** 5 * dy ** 4 + 0.3600E4 * cc ** 5 * dt ** 6 * dx ** 7 * dy
     # ** 3 * dz ** 6 + 0.3600E4 * cc ** 5 * dt ** 6 * dy ** 7 * dz ** 3
     # * dx ** 6 + 0.3600E4 * cc ** 5 * dt ** 6 * dy ** 5 * dz ** 5 * dx
     # ** 6 + 0.9000E4 * cc ** 5 * dt ** 6 * dx ** 5 * dy ** 7 * dz ** 4
     # + 0.3600E4 * cc ** 5 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 6 + 0.
     #4800E4 * cc ** 5 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 6 + 0.6000E
     #4 * cc ** 5 * dt ** 6 * dy ** 7 * dz ** 5 * dx ** 4 + 0.3600E4 * c
     #c ** 5 * dt ** 6 * dx ** 3 * dy ** 7 * dz ** 6 - 0.44550E5 * cc **
     # 4 * dt ** 5 * dx ** 7 * dy ** 7 * dz ** 3 - 0.43500E5 * cc ** 4 *
     # dt ** 5 * dx ** 7 * dz ** 5 * dy ** 5 - 0.43500E5 * cc ** 4 * dt 
     #** 5 * dy ** 7 * dz ** 5 * dx ** 5 - 0.74200E5 * cc ** 3 * dt ** 4
     # * dx ** 7 * dy ** 7 * dz ** 4 - 0.30000E5 * cc ** 3 * dt ** 4 * d
     #x ** 7 * dz ** 5 * dy ** 6 - 0.35500E5 * cc ** 3 * dt ** 4 * dx **
     # 7 * dy ** 5 * dz ** 6 - 0.30000E5 * cc ** 3 * dt ** 4 * dy ** 7 *
     # dz ** 5 * dx ** 6 - 0.35500E5 * cc ** 3 * dt ** 4 * dx ** 5 * dy 
     #** 7 * dz ** 6 + 0.216000E6 * cc ** 2 * dt ** 3 * dx ** 7 * dy ** 
     #7 * dz ** 5 + 0.89600E5 * cc * dt ** 2 * dx ** 7 * dy ** 7 * dz **
     # 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(365) = (-0.3480E4 * cc ** 6 * dt ** 7 * dx ** 7 * dy ** 7 * dz
     # - 0.6264E4 * cc ** 6 * dt ** 7 * dx ** 7 * dz ** 3 * dy ** 5 - 0.
     #6264E4 * cc ** 6 * dt ** 7 * dx ** 7 * dz ** 5 * dy ** 3 - 0.3480E
     #4 * cc ** 6 * dt ** 7 * dx ** 7 * dz ** 7 * dy - 0.6264E4 * cc ** 
     #6 * dt ** 7 * dy ** 7 * dz ** 3 * dx ** 5 - 0.8352E4 * cc ** 6 * d
     #t ** 7 * dy ** 5 * dz ** 5 * dx ** 5 - 0.6264E4 * cc ** 6 * dt ** 
     #7 * dy ** 3 * dz ** 7 * dx ** 5 - 0.6264E4 * cc ** 6 * dt ** 7 * d
     #y ** 7 * dz ** 5 * dx ** 3 - 0.6264E4 * cc ** 6 * dt ** 7 * dy ** 
     #5 * dz ** 7 * dx ** 3 - 0.3480E4 * cc ** 6 * dt ** 7 * dy ** 7 * d
     #z ** 7 * dx - 0.10500E5 * cc ** 5 * dt ** 6 * dx ** 7 * dy ** 7 * 
     #dz ** 2 - 0.5400E4 * cc ** 5 * dt ** 6 * dx ** 7 * dz ** 3 * dy **
     # 6 - 0.12000E5 * cc ** 5 * dt ** 6 * dx ** 7 * dy ** 5 * dz ** 4 -
     # 0.12000E5 * cc ** 5 * dt ** 6 * dx ** 7 * dz ** 5 * dy ** 4 - 0.5
     #400E4 * cc ** 5 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 6 - 0.10500E
     #5 * cc ** 5 * dt ** 6 * dx ** 7 * dz ** 7 * dy ** 2 - 0.5400E4 * c
     #c ** 5 * dt ** 6 * dy ** 7 * dz ** 3 * dx ** 6 - 0.7200E4 * cc ** 
     #5 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 6 - 0.5400E4 * cc ** 5 * d
     #t ** 6 * dy ** 3 * dz ** 7 * dx ** 6 - 0.12000E5 * cc ** 5 * dt **
     # 6 * dx ** 5 * dy ** 7 * dz ** 4 - 0.7200E4 * cc ** 5 * dt ** 6 * 
     #dx ** 5 * dz ** 5 * dy ** 6 - 0.7200E4 * cc ** 5 * dt ** 6 * dx **
     # 5 * dy ** 5 * dz ** 6 - 0.12000E5 * cc ** 5 * dt ** 6 * dx ** 5 *
     # dz ** 7 * dy ** 4 - 0.12000E5 * cc ** 5 * dt ** 6 * dy ** 7 * dz 
     #** 5 * dx ** 4 - 0.12000E5 * cc ** 5 * dt ** 6 * dy ** 5 * dz ** 7
     # * dx ** 4 - 0.5400E4 * cc ** 5 * dt ** 6 * dx ** 3 * dy ** 7 * dz
     # ** 6 - 0.5400E4 * cc ** 5 * dt ** 6 * dx ** 3 * dz ** 7 * dy ** 6
     # - 0.10500E5 * cc ** 5 * dt ** 6 * dy ** 7 * dz ** 7 * dx ** 2 + 0
     #.64200E5 * cc ** 4 * dt ** 5 * dx ** 7 * dy ** 7 * dz ** 3 + 0.828
     #00E5 * cc ** 4 * dt ** 5 * dx ** 7 * dz ** 5 * dy ** 5 + 0.64200E5
     # * cc ** 4 * dt ** 5 * dx ** 7 * dz ** 7 * dy ** 3 + 0.82800E5 * c
     #c ** 4 * dt ** 5 * dy ** 7 * dz ** 5 * dx ** 5 + 0.82800E5 * cc **
     # 4 * dt ** 5 * dy ** 5 * dz ** 7 * dx ** 5 + 0.64200E5 * cc ** 4 *
     # dt ** 5 * dy ** 7 * dz ** 7 * dx ** 3 + 0.92750E5 * cc ** 3 * dt 
     #** 4 * dx ** 7 * dy ** 7 * dz ** 4 + 0.51500E5 * cc ** 3 * dt ** 4
     # * dx ** 7 * dz ** 5 * dy ** 6 + 0.51500E5 * cc ** 3 * dt ** 4 * d
     #x ** 7 * dy ** 5 * dz ** 6 + 0.92750E5 * cc ** 3 * dt ** 4 * dx **
     # 7 * dz ** 7 * dy ** 4 + 0.51500E5 * cc ** 3 * dt ** 4 * dy ** 7 *
     # dz ** 5 * dx ** 6 + 0.51500E5 * cc ** 3 * dt ** 4 * dy ** 5 * dz 
     #** 7 * dx ** 6 + 0.51500E5 * cc ** 3 * dt ** 4 * dx ** 5 * dy ** 7
     # * dz ** 6 + 0.51500E5 * cc ** 3 * dt ** 4 * dx ** 5 * dz ** 7 * d
     #y ** 6 + 0.92750E5 * cc ** 3 * dt ** 4 * dy ** 7 * dz ** 7 * dx **
     # 4 - 0.392000E6 * cc ** 2 * dt ** 3 * dx ** 7 * dy ** 7 * dz ** 5 
     #- 0.392000E6 * cc ** 2 * dt ** 3 * dx ** 7 * dz ** 7 * dy ** 5 - 0
     #.392000E6 * cc ** 2 * dt ** 3 * dy ** 7 * dz ** 7 * dx ** 5 - 0.11
     #2000E6 * cc * dt ** 2 * dx ** 7 * dy ** 7 * dz ** 6 - 0.112000E6 *
     # cc * dt ** 2 * dx ** 7 * dz ** 7 * dy ** 6 - 0.112000E6 * cc * dt
     # ** 2 * dy ** 7 * dz ** 7 * dx ** 6 + 0.864000E6 * dt * dx ** 7 * 
     #dy ** 7 * dz ** 7) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(366) = (0.2610E4 * cc ** 6 * dt ** 7 * dx ** 7 * dy ** 7 * dz 
     #+ 0.4176E4 * cc ** 6 * dt ** 7 * dx ** 7 * dz ** 3 * dy ** 5 + 0.3
     #132E4 * cc ** 6 * dt ** 7 * dx ** 7 * dz ** 5 * dy ** 3 + 0.4176E4
     # * cc ** 6 * dt ** 7 * dy ** 7 * dz ** 3 * dx ** 5 + 0.4176E4 * cc
     # ** 6 * dt ** 7 * dy ** 5 * dz ** 5 * dx ** 5 + 0.3132E4 * cc ** 6
     # * dt ** 7 * dy ** 7 * dz ** 5 * dx ** 3 + 0.8400E4 * cc ** 5 * dt
     # ** 6 * dx ** 7 * dy ** 7 * dz ** 2 + 0.3600E4 * cc ** 5 * dt ** 6
     # * dx ** 7 * dz ** 3 * dy ** 6 + 0.9000E4 * cc ** 5 * dt ** 6 * dx
     # ** 7 * dy ** 5 * dz ** 4 + 0.6000E4 * cc ** 5 * dt ** 6 * dx ** 7
     # * dz ** 5 * dy ** 4 + 0.3600E4 * cc ** 5 * dt ** 6 * dx ** 7 * dy
     # ** 3 * dz ** 6 + 0.3600E4 * cc ** 5 * dt ** 6 * dy ** 7 * dz ** 3
     # * dx ** 6 + 0.3600E4 * cc ** 5 * dt ** 6 * dy ** 5 * dz ** 5 * dx
     # ** 6 + 0.9000E4 * cc ** 5 * dt ** 6 * dx ** 5 * dy ** 7 * dz ** 4
     # + 0.3600E4 * cc ** 5 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 6 + 0.
     #4800E4 * cc ** 5 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 6 + 0.6000E
     #4 * cc ** 5 * dt ** 6 * dy ** 7 * dz ** 5 * dx ** 4 + 0.3600E4 * c
     #c ** 5 * dt ** 6 * dx ** 3 * dy ** 7 * dz ** 6 - 0.44550E5 * cc **
     # 4 * dt ** 5 * dx ** 7 * dy ** 7 * dz ** 3 - 0.43500E5 * cc ** 4 *
     # dt ** 5 * dx ** 7 * dz ** 5 * dy ** 5 - 0.43500E5 * cc ** 4 * dt 
     #** 5 * dy ** 7 * dz ** 5 * dx ** 5 - 0.74200E5 * cc ** 3 * dt ** 4
     # * dx ** 7 * dy ** 7 * dz ** 4 - 0.30000E5 * cc ** 3 * dt ** 4 * d
     #x ** 7 * dz ** 5 * dy ** 6 - 0.35500E5 * cc ** 3 * dt ** 4 * dx **
     # 7 * dy ** 5 * dz ** 6 - 0.30000E5 * cc ** 3 * dt ** 4 * dy ** 7 *
     # dz ** 5 * dx ** 6 - 0.35500E5 * cc ** 3 * dt ** 4 * dx ** 5 * dy 
     #** 7 * dz ** 6 + 0.216000E6 * cc ** 2 * dt ** 3 * dx ** 7 * dy ** 
     #7 * dz ** 5 + 0.89600E5 * cc * dt ** 2 * dx ** 7 * dy ** 7 * dz **
     # 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(367) = (-0.1044E4 * cc ** 6 * dt ** 7 * dx ** 7 * dy ** 7 * dz
     # - 0.1044E4 * cc ** 6 * dt ** 7 * dx ** 7 * dz ** 3 * dy ** 5 - 0.
     #1044E4 * cc ** 6 * dt ** 7 * dy ** 7 * dz ** 3 * dx ** 5 - 0.4200E
     #4 * cc ** 5 * dt ** 6 * dx ** 7 * dy ** 7 * dz ** 2 - 0.900E3 * cc
     # ** 5 * dt ** 6 * dx ** 7 * dz ** 3 * dy ** 6 - 0.3600E4 * cc ** 5
     # * dt ** 6 * dx ** 7 * dy ** 5 * dz ** 4 - 0.900E3 * cc ** 5 * dt 
     #** 6 * dx ** 7 * dy ** 3 * dz ** 6 - 0.900E3 * cc ** 5 * dt ** 6 *
     # dy ** 7 * dz ** 3 * dx ** 6 - 0.3600E4 * cc ** 5 * dt ** 6 * dx *
     #* 5 * dy ** 7 * dz ** 4 - 0.1200E4 * cc ** 5 * dt ** 6 * dx ** 5 *
     # dy ** 5 * dz ** 6 - 0.900E3 * cc ** 5 * dt ** 6 * dx ** 3 * dy **
     # 7 * dz ** 6 + 0.13500E5 * cc ** 4 * dt ** 5 * dx ** 7 * dy ** 7 *
     # dz ** 3 + 0.2100E4 * cc ** 4 * dt ** 5 * dx ** 7 * dz ** 5 * dy *
     #* 5 + 0.2100E4 * cc ** 4 * dt ** 5 * dy ** 7 * dz ** 5 * dx ** 5 +
     # 0.37100E5 * cc ** 3 * dt ** 4 * dx ** 7 * dy ** 7 * dz ** 4 + 0.4
     #650E4 * cc ** 3 * dt ** 4 * dx ** 7 * dz ** 5 * dy ** 6 + 0.11250E
     #5 * cc ** 3 * dt ** 4 * dx ** 7 * dy ** 5 * dz ** 6 + 0.4650E4 * c
     #c ** 3 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 6 + 0.11250E5 * cc **
     # 3 * dt ** 4 * dx ** 5 * dy ** 7 * dz ** 6 - 0.21600E5 * cc ** 2 *
     # dt ** 3 * dx ** 7 * dy ** 7 * dz ** 5 - 0.44800E5 * cc * dt ** 2 
     #* dx ** 7 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.8
     #64000E6
      cuv(368) = (0.174E3 * cc ** 6 * dt ** 7 * dx ** 7 * dy ** 7 * dz +
     # 0.1200E4 * cc ** 5 * dt ** 6 * dx ** 7 * dy ** 7 * dz ** 2 + 0.60
     #0E3 * cc ** 5 * dt ** 6 * dx ** 7 * dy ** 5 * dz ** 4 + 0.600E3 * 
     #cc ** 5 * dt ** 6 * dx ** 5 * dy ** 7 * dz ** 4 - 0.1050E4 * cc **
     # 4 * dt ** 5 * dx ** 7 * dy ** 7 * dz ** 3 - 0.10600E5 * cc ** 3 *
     # dt ** 4 * dx ** 7 * dy ** 7 * dz ** 4 - 0.400E3 * cc ** 3 * dt **
     # 4 * dx ** 7 * dz ** 5 * dy ** 6 - 0.1500E4 * cc ** 3 * dt ** 4 * 
     #dx ** 7 * dy ** 5 * dz ** 6 - 0.400E3 * cc ** 3 * dt ** 4 * dy ** 
     #7 * dz ** 5 * dx ** 6 - 0.1500E4 * cc ** 3 * dt ** 4 * dx ** 5 * d
     #y ** 7 * dz ** 6 + 0.1600E4 * cc ** 2 * dt ** 3 * dx ** 7 * dy ** 
     #7 * dz ** 5 + 0.12800E5 * cc * dt ** 2 * dx ** 7 * dy ** 7 * dz **
     # 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(369) = (-0.150E3 * cc ** 5 * dt ** 6 * dx ** 7 * dy ** 7 * dz 
     #** 2 + 0.1325E4 * cc ** 3 * dt ** 4 * dx ** 7 * dy ** 7 * dz ** 4 
     #- 0.1600E4 * cc * dt ** 2 * dx ** 7 * dy ** 7 * dz ** 6) / dx ** 7
     # / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(371) = (-0.300E3 * cc ** 5 * dt ** 6 * dx ** 7 * dy ** 5 * dz 
     #** 4 + 0.200E3 * cc ** 3 * dt ** 4 * dx ** 7 * dz ** 5 * dy ** 6 +
     # 0.750E3 * cc ** 3 * dt ** 4 * dx ** 7 * dy ** 5 * dz ** 6) / dx *
     #* 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(372) = (0.522E3 * cc ** 6 * dt ** 7 * dx ** 7 * dz ** 3 * dy *
     #* 5 + 0.600E3 * cc ** 5 * dt ** 6 * dx ** 7 * dz ** 3 * dy ** 6 + 
     #0.1800E4 * cc ** 5 * dt ** 6 * dx ** 7 * dy ** 5 * dz ** 4 + 0.600
     #E3 * cc ** 5 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 6 + 0.600E3 * c
     #c ** 5 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 6 - 0.1050E4 * cc ** 
     #4 * dt ** 5 * dx ** 7 * dz ** 5 * dy ** 5 - 0.2700E4 * cc ** 3 * d
     #t ** 4 * dx ** 7 * dz ** 5 * dy ** 6 - 0.6000E4 * cc ** 3 * dt ** 
     #4 * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0
     #.864000E6
      cuv(373) = (-0.2088E4 * cc ** 6 * dt ** 7 * dx ** 7 * dz ** 3 * dy
     # ** 5 - 0.2088E4 * cc ** 6 * dt ** 7 * dx ** 7 * dz ** 5 * dy ** 3
     # - 0.2088E4 * cc ** 6 * dt ** 7 * dy ** 5 * dz ** 5 * dx ** 5 - 0.
     #2400E4 * cc ** 5 * dt ** 6 * dx ** 7 * dz ** 3 * dy ** 6 - 0.4500E
     #4 * cc ** 5 * dt ** 6 * dx ** 7 * dy ** 5 * dz ** 4 - 0.4500E4 * c
     #c ** 5 * dt ** 6 * dx ** 7 * dz ** 5 * dy ** 4 - 0.2400E4 * cc ** 
     #5 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 6 - 0.1800E4 * cc ** 5 * d
     #t ** 6 * dy ** 5 * dz ** 5 * dx ** 6 - 0.2400E4 * cc ** 5 * dt ** 
     #6 * dx ** 5 * dz ** 5 * dy ** 6 - 0.2400E4 * cc ** 5 * dt ** 6 * d
     #x ** 5 * dy ** 5 * dz ** 6 + 0.22800E5 * cc ** 4 * dt ** 5 * dx **
     # 7 * dz ** 5 * dy ** 5 + 0.20250E5 * cc ** 3 * dt ** 4 * dx ** 7 *
     # dz ** 5 * dy ** 6 + 0.20250E5 * cc ** 3 * dt ** 4 * dx ** 7 * dy 
     #** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(374) = (0.3132E4 * cc ** 6 * dt ** 7 * dx ** 7 * dz ** 3 * dy 
     #** 5 + 0.4176E4 * cc ** 6 * dt ** 7 * dx ** 7 * dz ** 5 * dy ** 3 
     #+ 0.2610E4 * cc ** 6 * dt ** 7 * dx ** 7 * dz ** 7 * dy + 0.4176E4
     # * cc ** 6 * dt ** 7 * dy ** 5 * dz ** 5 * dx ** 5 + 0.4176E4 * cc
     # ** 6 * dt ** 7 * dy ** 3 * dz ** 7 * dx ** 5 + 0.3132E4 * cc ** 6
     # * dt ** 7 * dy ** 5 * dz ** 7 * dx ** 3 + 0.3600E4 * cc ** 5 * dt
     # ** 6 * dx ** 7 * dz ** 3 * dy ** 6 + 0.6000E4 * cc ** 5 * dt ** 6
     # * dx ** 7 * dy ** 5 * dz ** 4 + 0.9000E4 * cc ** 5 * dt ** 6 * dx
     # ** 7 * dz ** 5 * dy ** 4 + 0.3600E4 * cc ** 5 * dt ** 6 * dx ** 7
     # * dy ** 3 * dz ** 6 + 0.8400E4 * cc ** 5 * dt ** 6 * dx ** 7 * dz
     # ** 7 * dy ** 2 + 0.3600E4 * cc ** 5 * dt ** 6 * dy ** 5 * dz ** 5
     # * dx ** 6 + 0.3600E4 * cc ** 5 * dt ** 6 * dy ** 3 * dz ** 7 * dx
     # ** 6 + 0.4800E4 * cc ** 5 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 6
     # + 0.3600E4 * cc ** 5 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 6 + 0.
     #9000E4 * cc ** 5 * dt ** 6 * dx ** 5 * dz ** 7 * dy ** 4 + 0.6000E
     #4 * cc ** 5 * dt ** 6 * dy ** 5 * dz ** 7 * dx ** 4 + 0.3600E4 * c
     #c ** 5 * dt ** 6 * dx ** 3 * dz ** 7 * dy ** 6 - 0.43500E5 * cc **
     # 4 * dt ** 5 * dx ** 7 * dz ** 5 * dy ** 5 - 0.44550E5 * cc ** 4 *
     # dt ** 5 * dx ** 7 * dz ** 7 * dy ** 3 - 0.43500E5 * cc ** 4 * dt 
     #** 5 * dy ** 5 * dz ** 7 * dx ** 5 - 0.35500E5 * cc ** 3 * dt ** 4
     # * dx ** 7 * dz ** 5 * dy ** 6 - 0.30000E5 * cc ** 3 * dt ** 4 * d
     #x ** 7 * dy ** 5 * dz ** 6 - 0.74200E5 * cc ** 3 * dt ** 4 * dx **
     # 7 * dz ** 7 * dy ** 4 - 0.30000E5 * cc ** 3 * dt ** 4 * dy ** 5 *
     # dz ** 7 * dx ** 6 - 0.35500E5 * cc ** 3 * dt ** 4 * dx ** 5 * dz 
     #** 7 * dy ** 6 + 0.216000E6 * cc ** 2 * dt ** 3 * dx ** 7 * dz ** 
     #7 * dy ** 5 + 0.89600E5 * cc * dt ** 2 * dx ** 7 * dz ** 7 * dy **
     # 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(375) = (-0.2088E4 * cc ** 6 * dt ** 7 * dx ** 7 * dz ** 3 * dy
     # ** 5 - 0.2088E4 * cc ** 6 * dt ** 7 * dx ** 7 * dz ** 5 * dy ** 3
     # - 0.2088E4 * cc ** 6 * dt ** 7 * dy ** 5 * dz ** 5 * dx ** 5 - 0.
     #2400E4 * cc ** 5 * dt ** 6 * dx ** 7 * dz ** 3 * dy ** 6 - 0.4500E
     #4 * cc ** 5 * dt ** 6 * dx ** 7 * dy ** 5 * dz ** 4 - 0.4500E4 * c
     #c ** 5 * dt ** 6 * dx ** 7 * dz ** 5 * dy ** 4 - 0.2400E4 * cc ** 
     #5 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 6 - 0.1800E4 * cc ** 5 * d
     #t ** 6 * dy ** 5 * dz ** 5 * dx ** 6 - 0.2400E4 * cc ** 5 * dt ** 
     #6 * dx ** 5 * dz ** 5 * dy ** 6 - 0.2400E4 * cc ** 5 * dt ** 6 * d
     #x ** 5 * dy ** 5 * dz ** 6 + 0.22800E5 * cc ** 4 * dt ** 5 * dx **
     # 7 * dz ** 5 * dy ** 5 + 0.20250E5 * cc ** 3 * dt ** 4 * dx ** 7 *
     # dz ** 5 * dy ** 6 + 0.20250E5 * cc ** 3 * dt ** 4 * dx ** 7 * dy 
     #** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(376) = (0.522E3 * cc ** 6 * dt ** 7 * dx ** 7 * dz ** 3 * dy *
     #* 5 + 0.600E3 * cc ** 5 * dt ** 6 * dx ** 7 * dz ** 3 * dy ** 6 + 
     #0.1800E4 * cc ** 5 * dt ** 6 * dx ** 7 * dy ** 5 * dz ** 4 + 0.600
     #E3 * cc ** 5 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 6 + 0.600E3 * c
     #c ** 5 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 6 - 0.1050E4 * cc ** 
     #4 * dt ** 5 * dx ** 7 * dz ** 5 * dy ** 5 - 0.2700E4 * cc ** 3 * d
     #t ** 4 * dx ** 7 * dz ** 5 * dy ** 6 - 0.6000E4 * cc ** 3 * dt ** 
     #4 * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0
     #.864000E6
      cuv(377) = (-0.300E3 * cc ** 5 * dt ** 6 * dx ** 7 * dy ** 5 * dz 
     #** 4 + 0.200E3 * cc ** 3 * dt ** 4 * dx ** 7 * dz ** 5 * dy ** 6 +
     # 0.750E3 * cc ** 3 * dt ** 4 * dx ** 7 * dy ** 5 * dz ** 6) / dx *
     #* 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(381) = (-0.150E3 * cc ** 5 * dt ** 6 * dx ** 7 * dz ** 3 * dy 
     #** 6 - 0.150E3 * cc ** 5 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 6 +
     # 0.375E3 * cc ** 3 * dt ** 4 * dx ** 7 * dz ** 5 * dy ** 6 + 0.375
     #E3 * cc ** 3 * dt ** 4 * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / 
     #dy ** 7 / dz ** 7 / 0.864000E6
      cuv(382) = (0.522E3 * cc ** 6 * dt ** 7 * dx ** 7 * dz ** 5 * dy *
     #* 3 + 0.600E3 * cc ** 5 * dt ** 6 * dx ** 7 * dz ** 3 * dy ** 6 + 
     #0.1800E4 * cc ** 5 * dt ** 6 * dx ** 7 * dz ** 5 * dy ** 4 + 0.600
     #E3 * cc ** 5 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 6 + 0.600E3 * c
     #c ** 5 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 6 - 0.1050E4 * cc ** 
     #4 * dt ** 5 * dx ** 7 * dz ** 5 * dy ** 5 - 0.6000E4 * cc ** 3 * d
     #t ** 4 * dx ** 7 * dz ** 5 * dy ** 6 - 0.2700E4 * cc ** 3 * dt ** 
     #4 * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0
     #.864000E6
      cuv(383) = (-0.1044E4 * cc ** 6 * dt ** 7 * dx ** 7 * dz ** 5 * dy
     # ** 3 - 0.1044E4 * cc ** 6 * dt ** 7 * dx ** 7 * dz ** 7 * dy - 0.
     #1044E4 * cc ** 6 * dt ** 7 * dy ** 3 * dz ** 7 * dx ** 5 - 0.900E3
     # * cc ** 5 * dt ** 6 * dx ** 7 * dz ** 3 * dy ** 6 - 0.3600E4 * cc
     # ** 5 * dt ** 6 * dx ** 7 * dz ** 5 * dy ** 4 - 0.900E3 * cc ** 5 
     #* dt ** 6 * dx ** 7 * dy ** 3 * dz ** 6 - 0.4200E4 * cc ** 5 * dt 
     #** 6 * dx ** 7 * dz ** 7 * dy ** 2 - 0.900E3 * cc ** 5 * dt ** 6 *
     # dy ** 3 * dz ** 7 * dx ** 6 - 0.1200E4 * cc ** 5 * dt ** 6 * dx *
     #* 5 * dz ** 5 * dy ** 6 - 0.3600E4 * cc ** 5 * dt ** 6 * dx ** 5 *
     # dz ** 7 * dy ** 4 - 0.900E3 * cc ** 5 * dt ** 6 * dx ** 3 * dz **
     # 7 * dy ** 6 + 0.2100E4 * cc ** 4 * dt ** 5 * dx ** 7 * dz ** 5 * 
     #dy ** 5 + 0.13500E5 * cc ** 4 * dt ** 5 * dx ** 7 * dz ** 7 * dy *
     #* 3 + 0.2100E4 * cc ** 4 * dt ** 5 * dy ** 5 * dz ** 7 * dx ** 5 +
     # 0.11250E5 * cc ** 3 * dt ** 4 * dx ** 7 * dz ** 5 * dy ** 6 + 0.4
     #650E4 * cc ** 3 * dt ** 4 * dx ** 7 * dy ** 5 * dz ** 6 + 0.37100E
     #5 * cc ** 3 * dt ** 4 * dx ** 7 * dz ** 7 * dy ** 4 + 0.4650E4 * c
     #c ** 3 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 6 + 0.11250E5 * cc **
     # 3 * dt ** 4 * dx ** 5 * dz ** 7 * dy ** 6 - 0.21600E5 * cc ** 2 *
     # dt ** 3 * dx ** 7 * dz ** 7 * dy ** 5 - 0.44800E5 * cc * dt ** 2 
     #* dx ** 7 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.8
     #64000E6
      cuv(384) = (0.522E3 * cc ** 6 * dt ** 7 * dx ** 7 * dz ** 5 * dy *
     #* 3 + 0.600E3 * cc ** 5 * dt ** 6 * dx ** 7 * dz ** 3 * dy ** 6 + 
     #0.1800E4 * cc ** 5 * dt ** 6 * dx ** 7 * dz ** 5 * dy ** 4 + 0.600
     #E3 * cc ** 5 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 6 + 0.600E3 * c
     #c ** 5 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 6 - 0.1050E4 * cc ** 
     #4 * dt ** 5 * dx ** 7 * dz ** 5 * dy ** 5 - 0.6000E4 * cc ** 3 * d
     #t ** 4 * dx ** 7 * dz ** 5 * dy ** 6 - 0.2700E4 * cc ** 3 * dt ** 
     #4 * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0
     #.864000E6
      cuv(385) = (-0.150E3 * cc ** 5 * dt ** 6 * dx ** 7 * dz ** 3 * dy 
     #** 6 - 0.150E3 * cc ** 5 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 6 +
     # 0.375E3 * cc ** 3 * dt ** 4 * dx ** 7 * dz ** 5 * dy ** 6 + 0.375
     #E3 * cc ** 3 * dt ** 4 * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / 
     #dy ** 7 / dz ** 7 / 0.864000E6
      cuv(391) = (-0.300E3 * cc ** 5 * dt ** 6 * dx ** 7 * dz ** 5 * dy 
     #** 4 + 0.750E3 * cc ** 3 * dt ** 4 * dx ** 7 * dz ** 5 * dy ** 6 +
     # 0.200E3 * cc ** 3 * dt ** 4 * dx ** 7 * dy ** 5 * dz ** 6) / dx *
     #* 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(392) = (0.174E3 * cc ** 6 * dt ** 7 * dx ** 7 * dz ** 7 * dy +
     # 0.600E3 * cc ** 5 * dt ** 6 * dx ** 7 * dz ** 5 * dy ** 4 + 0.120
     #0E4 * cc ** 5 * dt ** 6 * dx ** 7 * dz ** 7 * dy ** 2 + 0.600E3 * 
     #cc ** 5 * dt ** 6 * dx ** 5 * dz ** 7 * dy ** 4 - 0.1050E4 * cc **
     # 4 * dt ** 5 * dx ** 7 * dz ** 7 * dy ** 3 - 0.1500E4 * cc ** 3 * 
     #dt ** 4 * dx ** 7 * dz ** 5 * dy ** 6 - 0.400E3 * cc ** 3 * dt ** 
     #4 * dx ** 7 * dy ** 5 * dz ** 6 - 0.10600E5 * cc ** 3 * dt ** 4 * 
     #dx ** 7 * dz ** 7 * dy ** 4 - 0.400E3 * cc ** 3 * dt ** 4 * dy ** 
     #5 * dz ** 7 * dx ** 6 - 0.1500E4 * cc ** 3 * dt ** 4 * dx ** 5 * d
     #z ** 7 * dy ** 6 + 0.1600E4 * cc ** 2 * dt ** 3 * dx ** 7 * dz ** 
     #7 * dy ** 5 + 0.12800E5 * cc * dt ** 2 * dx ** 7 * dz ** 7 * dy **
     # 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(393) = (-0.300E3 * cc ** 5 * dt ** 6 * dx ** 7 * dz ** 5 * dy 
     #** 4 + 0.750E3 * cc ** 3 * dt ** 4 * dx ** 7 * dz ** 5 * dy ** 6 +
     # 0.200E3 * cc ** 3 * dt ** 4 * dx ** 7 * dy ** 5 * dz ** 6) / dx *
     #* 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(401) = (-0.150E3 * cc ** 5 * dt ** 6 * dx ** 7 * dz ** 7 * dy 
     #** 2 + 0.1325E4 * cc ** 3 * dt ** 4 * dx ** 7 * dz ** 7 * dy ** 4 
     #- 0.1600E4 * cc * dt ** 2 * dx ** 7 * dz ** 7 * dy ** 6) / dx ** 7
     # / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(419) = (-0.300E3 * cc ** 5 * dt ** 6 * dx ** 5 * dz ** 7 * dy 
     #** 4 + 0.200E3 * cc ** 3 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 6 +
     # 0.750E3 * cc ** 3 * dt ** 4 * dx ** 5 * dz ** 7 * dy ** 6) / dx *
     #* 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(427) = -cc ** 5 * dt ** 6 / dx ** 2 / dz ** 2 / dy / 0.2880E4
      cuv(428) = (0.522E3 * cc ** 6 * dt ** 7 * dy ** 3 * dz ** 7 * dx *
     #* 5 + 0.600E3 * cc ** 5 * dt ** 6 * dy ** 3 * dz ** 7 * dx ** 6 + 
     #0.600E3 * cc ** 5 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 6 + 0.1800
     #E4 * cc ** 5 * dt ** 6 * dx ** 5 * dz ** 7 * dy ** 4 + 0.600E3 * c
     #c ** 5 * dt ** 6 * dx ** 3 * dz ** 7 * dy ** 6 - 0.1050E4 * cc ** 
     #4 * dt ** 5 * dy ** 5 * dz ** 7 * dx ** 5 - 0.2700E4 * cc ** 3 * d
     #t ** 4 * dy ** 5 * dz ** 7 * dx ** 6 - 0.6000E4 * cc ** 3 * dt ** 
     #4 * dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0
     #.864000E6
      cuv(429) = -cc ** 5 * dt ** 6 / dx ** 2 / dz ** 2 / dy / 0.2880E4
      cuv(435) = -cc ** 5 * dt ** 6 / dx ** 2 / dy ** 2 / dz / 0.2880E4
      cuv(436) = (0.1044E4 * cc ** 6 * dt ** 7 * dy ** 5 * dz ** 5 * dx 
     #** 5 + 0.1200E4 * cc ** 5 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 6 
     #+ 0.1200E4 * cc ** 5 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 6 + 0.1
     #200E4 * cc ** 5 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7
     # / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(437) = (-0.2088E4 * cc ** 6 * dt ** 7 * dy ** 5 * dz ** 5 * dx
     # ** 5 - 0.2088E4 * cc ** 6 * dt ** 7 * dy ** 3 * dz ** 7 * dx ** 5
     # - 0.2088E4 * cc ** 6 * dt ** 7 * dy ** 5 * dz ** 7 * dx ** 3 - 0.
     #2400E4 * cc ** 5 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 6 - 0.2400E
     #4 * cc ** 5 * dt ** 6 * dy ** 3 * dz ** 7 * dx ** 6 - 0.2400E4 * c
     #c ** 5 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 6 - 0.1800E4 * cc ** 
     #5 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 6 - 0.4500E4 * cc ** 5 * d
     #t ** 6 * dx ** 5 * dz ** 7 * dy ** 4 - 0.4500E4 * cc ** 5 * dt ** 
     #6 * dy ** 5 * dz ** 7 * dx ** 4 - 0.2400E4 * cc ** 5 * dt ** 6 * d
     #x ** 3 * dz ** 7 * dy ** 6 + 0.22800E5 * cc ** 4 * dt ** 5 * dy **
     # 5 * dz ** 7 * dx ** 5 + 0.20250E5 * cc ** 3 * dt ** 4 * dy ** 5 *
     # dz ** 7 * dx ** 6 + 0.20250E5 * cc ** 3 * dt ** 4 * dx ** 5 * dz 
     #** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(438) = (0.1044E4 * cc ** 6 * dt ** 7 * dy ** 5 * dz ** 5 * dx 
     #** 5 + 0.1200E4 * cc ** 5 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 6 
     #+ 0.1200E4 * cc ** 5 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 6 + 0.1
     #200E4 * cc ** 5 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7
     # / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(439) = -cc ** 5 * dt ** 6 / dx ** 2 / dy ** 2 / dz / 0.2880E4
      cuv(443) = (-0.300E3 * cc ** 5 * dt ** 6 * dx ** 5 * dy ** 7 * dz 
     #** 4 + 0.200E3 * cc ** 3 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 6 +
     # 0.750E3 * cc ** 3 * dt ** 4 * dx ** 5 * dy ** 7 * dz ** 6) / dx *
     #* 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(444) = (0.522E3 * cc ** 6 * dt ** 7 * dy ** 7 * dz ** 3 * dx *
     #* 5 + 0.600E3 * cc ** 5 * dt ** 6 * dy ** 7 * dz ** 3 * dx ** 6 + 
     #0.1800E4 * cc ** 5 * dt ** 6 * dx ** 5 * dy ** 7 * dz ** 4 + 0.600
     #E3 * cc ** 5 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 6 + 0.600E3 * c
     #c ** 5 * dt ** 6 * dx ** 3 * dy ** 7 * dz ** 6 - 0.1050E4 * cc ** 
     #4 * dt ** 5 * dy ** 7 * dz ** 5 * dx ** 5 - 0.2700E4 * cc ** 3 * d
     #t ** 4 * dy ** 7 * dz ** 5 * dx ** 6 - 0.6000E4 * cc ** 3 * dt ** 
     #4 * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0
     #.864000E6
      cuv(445) = (-0.2088E4 * cc ** 6 * dt ** 7 * dy ** 7 * dz ** 3 * dx
     # ** 5 - 0.2088E4 * cc ** 6 * dt ** 7 * dy ** 5 * dz ** 5 * dx ** 5
     # - 0.2088E4 * cc ** 6 * dt ** 7 * dy ** 7 * dz ** 5 * dx ** 3 - 0.
     #2400E4 * cc ** 5 * dt ** 6 * dy ** 7 * dz ** 3 * dx ** 6 - 0.2400E
     #4 * cc ** 5 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 6 - 0.4500E4 * c
     #c ** 5 * dt ** 6 * dx ** 5 * dy ** 7 * dz ** 4 - 0.1800E4 * cc ** 
     #5 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 6 - 0.2400E4 * cc ** 5 * d
     #t ** 6 * dx ** 5 * dy ** 5 * dz ** 6 - 0.4500E4 * cc ** 5 * dt ** 
     #6 * dy ** 7 * dz ** 5 * dx ** 4 - 0.2400E4 * cc ** 5 * dt ** 6 * d
     #x ** 3 * dy ** 7 * dz ** 6 + 0.22800E5 * cc ** 4 * dt ** 5 * dy **
     # 7 * dz ** 5 * dx ** 5 + 0.20250E5 * cc ** 3 * dt ** 4 * dy ** 7 *
     # dz ** 5 * dx ** 6 + 0.20250E5 * cc ** 3 * dt ** 4 * dx ** 5 * dy 
     #** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(446) = (0.3132E4 * cc ** 6 * dt ** 7 * dy ** 7 * dz ** 3 * dx 
     #** 5 + 0.4176E4 * cc ** 6 * dt ** 7 * dy ** 5 * dz ** 5 * dx ** 5 
     #+ 0.3132E4 * cc ** 6 * dt ** 7 * dy ** 3 * dz ** 7 * dx ** 5 + 0.4
     #176E4 * cc ** 6 * dt ** 7 * dy ** 7 * dz ** 5 * dx ** 3 + 0.4176E4
     # * cc ** 6 * dt ** 7 * dy ** 5 * dz ** 7 * dx ** 3 + 0.2610E4 * cc
     # ** 6 * dt ** 7 * dy ** 7 * dz ** 7 * dx + 0.3600E4 * cc ** 5 * dt
     # ** 6 * dy ** 7 * dz ** 3 * dx ** 6 + 0.4800E4 * cc ** 5 * dt ** 6
     # * dy ** 5 * dz ** 5 * dx ** 6 + 0.3600E4 * cc ** 5 * dt ** 6 * dy
     # ** 3 * dz ** 7 * dx ** 6 + 0.6000E4 * cc ** 5 * dt ** 6 * dx ** 5
     # * dy ** 7 * dz ** 4 + 0.3600E4 * cc ** 5 * dt ** 6 * dx ** 5 * dz
     # ** 5 * dy ** 6 + 0.3600E4 * cc ** 5 * dt ** 6 * dx ** 5 * dy ** 5
     # * dz ** 6 + 0.6000E4 * cc ** 5 * dt ** 6 * dx ** 5 * dz ** 7 * dy
     # ** 4 + 0.9000E4 * cc ** 5 * dt ** 6 * dy ** 7 * dz ** 5 * dx ** 4
     # + 0.9000E4 * cc ** 5 * dt ** 6 * dy ** 5 * dz ** 7 * dx ** 4 + 0.
     #3600E4 * cc ** 5 * dt ** 6 * dx ** 3 * dy ** 7 * dz ** 6 + 0.3600E
     #4 * cc ** 5 * dt ** 6 * dx ** 3 * dz ** 7 * dy ** 6 + 0.8400E4 * c
     #c ** 5 * dt ** 6 * dy ** 7 * dz ** 7 * dx ** 2 - 0.43500E5 * cc **
     # 4 * dt ** 5 * dy ** 7 * dz ** 5 * dx ** 5 - 0.43500E5 * cc ** 4 *
     # dt ** 5 * dy ** 5 * dz ** 7 * dx ** 5 - 0.44550E5 * cc ** 4 * dt 
     #** 5 * dy ** 7 * dz ** 7 * dx ** 3 - 0.35500E5 * cc ** 3 * dt ** 4
     # * dy ** 7 * dz ** 5 * dx ** 6 - 0.35500E5 * cc ** 3 * dt ** 4 * d
     #y ** 5 * dz ** 7 * dx ** 6 - 0.30000E5 * cc ** 3 * dt ** 4 * dx **
     # 5 * dy ** 7 * dz ** 6 - 0.30000E5 * cc ** 3 * dt ** 4 * dx ** 5 *
     # dz ** 7 * dy ** 6 - 0.74200E5 * cc ** 3 * dt ** 4 * dy ** 7 * dz 
     #** 7 * dx ** 4 + 0.216000E6 * cc ** 2 * dt ** 3 * dy ** 7 * dz ** 
     #7 * dx ** 5 + 0.89600E5 * cc * dt ** 2 * dy ** 7 * dz ** 7 * dx **
     # 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(447) = (-0.2088E4 * cc ** 6 * dt ** 7 * dy ** 7 * dz ** 3 * dx
     # ** 5 - 0.2088E4 * cc ** 6 * dt ** 7 * dy ** 5 * dz ** 5 * dx ** 5
     # - 0.2088E4 * cc ** 6 * dt ** 7 * dy ** 7 * dz ** 5 * dx ** 3 - 0.
     #2400E4 * cc ** 5 * dt ** 6 * dy ** 7 * dz ** 3 * dx ** 6 - 0.2400E
     #4 * cc ** 5 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 6 - 0.4500E4 * c
     #c ** 5 * dt ** 6 * dx ** 5 * dy ** 7 * dz ** 4 - 0.1800E4 * cc ** 
     #5 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 6 - 0.2400E4 * cc ** 5 * d
     #t ** 6 * dx ** 5 * dy ** 5 * dz ** 6 - 0.4500E4 * cc ** 5 * dt ** 
     #6 * dy ** 7 * dz ** 5 * dx ** 4 - 0.2400E4 * cc ** 5 * dt ** 6 * d
     #x ** 3 * dy ** 7 * dz ** 6 + 0.22800E5 * cc ** 4 * dt ** 5 * dy **
     # 7 * dz ** 5 * dx ** 5 + 0.20250E5 * cc ** 3 * dt ** 4 * dy ** 7 *
     # dz ** 5 * dx ** 6 + 0.20250E5 * cc ** 3 * dt ** 4 * dx ** 5 * dy 
     #** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(448) = (0.522E3 * cc ** 6 * dt ** 7 * dy ** 7 * dz ** 3 * dx *
     #* 5 + 0.600E3 * cc ** 5 * dt ** 6 * dy ** 7 * dz ** 3 * dx ** 6 + 
     #0.1800E4 * cc ** 5 * dt ** 6 * dx ** 5 * dy ** 7 * dz ** 4 + 0.600
     #E3 * cc ** 5 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 6 + 0.600E3 * c
     #c ** 5 * dt ** 6 * dx ** 3 * dy ** 7 * dz ** 6 - 0.1050E4 * cc ** 
     #4 * dt ** 5 * dy ** 7 * dz ** 5 * dx ** 5 - 0.2700E4 * cc ** 3 * d
     #t ** 4 * dy ** 7 * dz ** 5 * dx ** 6 - 0.6000E4 * cc ** 3 * dt ** 
     #4 * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0
     #.864000E6
      cuv(449) = (-0.300E3 * cc ** 5 * dt ** 6 * dx ** 5 * dy ** 7 * dz 
     #** 4 + 0.200E3 * cc ** 3 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 6 +
     # 0.750E3 * cc ** 3 * dt ** 4 * dx ** 5 * dy ** 7 * dz ** 6) / dx *
     #* 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(453) = -cc ** 5 * dt ** 6 / dx ** 2 / dy ** 2 / dz / 0.2880E4
      cuv(454) = (0.1044E4 * cc ** 6 * dt ** 7 * dy ** 5 * dz ** 5 * dx 
     #** 5 + 0.1200E4 * cc ** 5 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 6 
     #+ 0.1200E4 * cc ** 5 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 6 + 0.1
     #200E4 * cc ** 5 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7
     # / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(455) = (-0.2088E4 * cc ** 6 * dt ** 7 * dy ** 5 * dz ** 5 * dx
     # ** 5 - 0.2088E4 * cc ** 6 * dt ** 7 * dy ** 3 * dz ** 7 * dx ** 5
     # - 0.2088E4 * cc ** 6 * dt ** 7 * dy ** 5 * dz ** 7 * dx ** 3 - 0.
     #2400E4 * cc ** 5 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 6 - 0.2400E
     #4 * cc ** 5 * dt ** 6 * dy ** 3 * dz ** 7 * dx ** 6 - 0.2400E4 * c
     #c ** 5 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 6 - 0.1800E4 * cc ** 
     #5 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 6 - 0.4500E4 * cc ** 5 * d
     #t ** 6 * dx ** 5 * dz ** 7 * dy ** 4 - 0.4500E4 * cc ** 5 * dt ** 
     #6 * dy ** 5 * dz ** 7 * dx ** 4 - 0.2400E4 * cc ** 5 * dt ** 6 * d
     #x ** 3 * dz ** 7 * dy ** 6 + 0.22800E5 * cc ** 4 * dt ** 5 * dy **
     # 5 * dz ** 7 * dx ** 5 + 0.20250E5 * cc ** 3 * dt ** 4 * dy ** 5 *
     # dz ** 7 * dx ** 6 + 0.20250E5 * cc ** 3 * dt ** 4 * dx ** 5 * dz 
     #** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(456) = (0.1044E4 * cc ** 6 * dt ** 7 * dy ** 5 * dz ** 5 * dx 
     #** 5 + 0.1200E4 * cc ** 5 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 6 
     #+ 0.1200E4 * cc ** 5 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 6 + 0.1
     #200E4 * cc ** 5 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7
     # / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(457) = -cc ** 5 * dt ** 6 / dx ** 2 / dy ** 2 / dz / 0.2880E4
      cuv(463) = -cc ** 5 * dt ** 6 / dx ** 2 / dz ** 2 / dy / 0.2880E4
      cuv(464) = (0.522E3 * cc ** 6 * dt ** 7 * dy ** 3 * dz ** 7 * dx *
     #* 5 + 0.600E3 * cc ** 5 * dt ** 6 * dy ** 3 * dz ** 7 * dx ** 6 + 
     #0.600E3 * cc ** 5 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 6 + 0.1800
     #E4 * cc ** 5 * dt ** 6 * dx ** 5 * dz ** 7 * dy ** 4 + 0.600E3 * c
     #c ** 5 * dt ** 6 * dx ** 3 * dz ** 7 * dy ** 6 - 0.1050E4 * cc ** 
     #4 * dt ** 5 * dy ** 5 * dz ** 7 * dx ** 5 - 0.2700E4 * cc ** 3 * d
     #t ** 4 * dy ** 5 * dz ** 7 * dx ** 6 - 0.6000E4 * cc ** 3 * dt ** 
     #4 * dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0
     #.864000E6
      cuv(465) = -cc ** 5 * dt ** 6 / dx ** 2 / dz ** 2 / dy / 0.2880E4
      cuv(473) = (-0.300E3 * cc ** 5 * dt ** 6 * dx ** 5 * dz ** 7 * dy 
     #** 4 + 0.200E3 * cc ** 3 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 6 +
     # 0.750E3 * cc ** 3 * dt ** 4 * dx ** 5 * dz ** 7 * dy ** 6) / dx *
     #* 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(509) = (-0.150E3 * cc ** 5 * dt ** 6 * dy ** 3 * dz ** 7 * dx 
     #** 6 - 0.150E3 * cc ** 5 * dt ** 6 * dx ** 3 * dz ** 7 * dy ** 6 +
     # 0.375E3 * cc ** 3 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 6 + 0.375
     #E3 * cc ** 3 * dt ** 4 * dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / 
     #dy ** 7 / dz ** 7 / 0.864000E6
      cuv(517) = -cc ** 5 * dt ** 6 / dy ** 2 / dz ** 2 / dx / 0.2880E4
      cuv(518) = (0.522E3 * cc ** 6 * dt ** 7 * dy ** 5 * dz ** 7 * dx *
     #* 3 + 0.600E3 * cc ** 5 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 6 + 
     #0.600E3 * cc ** 5 * dt ** 6 * dy ** 3 * dz ** 7 * dx ** 6 + 0.1800
     #E4 * cc ** 5 * dt ** 6 * dy ** 5 * dz ** 7 * dx ** 4 + 0.600E3 * c
     #c ** 5 * dt ** 6 * dx ** 3 * dz ** 7 * dy ** 6 - 0.1050E4 * cc ** 
     #4 * dt ** 5 * dy ** 5 * dz ** 7 * dx ** 5 - 0.6000E4 * cc ** 3 * d
     #t ** 4 * dy ** 5 * dz ** 7 * dx ** 6 - 0.2700E4 * cc ** 3 * dt ** 
     #4 * dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0
     #.864000E6
      cuv(519) = -cc ** 5 * dt ** 6 / dy ** 2 / dz ** 2 / dx / 0.2880E4
      cuv(525) = (-0.150E3 * cc ** 5 * dt ** 6 * dy ** 7 * dz ** 3 * dx 
     #** 6 - 0.150E3 * cc ** 5 * dt ** 6 * dx ** 3 * dy ** 7 * dz ** 6 +
     # 0.375E3 * cc ** 3 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 6 + 0.375
     #E3 * cc ** 3 * dt ** 4 * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / 
     #dy ** 7 / dz ** 7 / 0.864000E6
      cuv(526) = (0.522E3 * cc ** 6 * dt ** 7 * dy ** 7 * dz ** 5 * dx *
     #* 3 + 0.600E3 * cc ** 5 * dt ** 6 * dy ** 7 * dz ** 3 * dx ** 6 + 
     #0.600E3 * cc ** 5 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 6 + 0.1800
     #E4 * cc ** 5 * dt ** 6 * dy ** 7 * dz ** 5 * dx ** 4 + 0.600E3 * c
     #c ** 5 * dt ** 6 * dx ** 3 * dy ** 7 * dz ** 6 - 0.1050E4 * cc ** 
     #4 * dt ** 5 * dy ** 7 * dz ** 5 * dx ** 5 - 0.6000E4 * cc ** 3 * d
     #t ** 4 * dy ** 7 * dz ** 5 * dx ** 6 - 0.2700E4 * cc ** 3 * dt ** 
     #4 * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0
     #.864000E6
      cuv(527) = (-0.1044E4 * cc ** 6 * dt ** 7 * dy ** 7 * dz ** 5 * dx
     # ** 3 - 0.1044E4 * cc ** 6 * dt ** 7 * dy ** 5 * dz ** 7 * dx ** 3
     # - 0.1044E4 * cc ** 6 * dt ** 7 * dy ** 7 * dz ** 7 * dx - 0.900E3
     # * cc ** 5 * dt ** 6 * dy ** 7 * dz ** 3 * dx ** 6 - 0.1200E4 * cc
     # ** 5 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 6 - 0.900E3 * cc ** 5 
     #* dt ** 6 * dy ** 3 * dz ** 7 * dx ** 6 - 0.3600E4 * cc ** 5 * dt 
     #** 6 * dy ** 7 * dz ** 5 * dx ** 4 - 0.3600E4 * cc ** 5 * dt ** 6 
     #* dy ** 5 * dz ** 7 * dx ** 4 - 0.900E3 * cc ** 5 * dt ** 6 * dx *
     #* 3 * dy ** 7 * dz ** 6 - 0.900E3 * cc ** 5 * dt ** 6 * dx ** 3 * 
     #dz ** 7 * dy ** 6 - 0.4200E4 * cc ** 5 * dt ** 6 * dy ** 7 * dz **
     # 7 * dx ** 2 + 0.2100E4 * cc ** 4 * dt ** 5 * dy ** 7 * dz ** 5 * 
     #dx ** 5 + 0.2100E4 * cc ** 4 * dt ** 5 * dy ** 5 * dz ** 7 * dx **
     # 5 + 0.13500E5 * cc ** 4 * dt ** 5 * dy ** 7 * dz ** 7 * dx ** 3 +
     # 0.11250E5 * cc ** 3 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 6 + 0.1
     #1250E5 * cc ** 3 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 6 + 0.4650E
     #4 * cc ** 3 * dt ** 4 * dx ** 5 * dy ** 7 * dz ** 6 + 0.4650E4 * c
     #c ** 3 * dt ** 4 * dx ** 5 * dz ** 7 * dy ** 6 + 0.37100E5 * cc **
     # 3 * dt ** 4 * dy ** 7 * dz ** 7 * dx ** 4 - 0.21600E5 * cc ** 2 *
     # dt ** 3 * dy ** 7 * dz ** 7 * dx ** 5 - 0.44800E5 * cc * dt ** 2 
     #* dy ** 7 * dz ** 7 * dx ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.8
     #64000E6
      cuv(528) = (0.522E3 * cc ** 6 * dt ** 7 * dy ** 7 * dz ** 5 * dx *
     #* 3 + 0.600E3 * cc ** 5 * dt ** 6 * dy ** 7 * dz ** 3 * dx ** 6 + 
     #0.600E3 * cc ** 5 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 6 + 0.1800
     #E4 * cc ** 5 * dt ** 6 * dy ** 7 * dz ** 5 * dx ** 4 + 0.600E3 * c
     #c ** 5 * dt ** 6 * dx ** 3 * dy ** 7 * dz ** 6 - 0.1050E4 * cc ** 
     #4 * dt ** 5 * dy ** 7 * dz ** 5 * dx ** 5 - 0.6000E4 * cc ** 3 * d
     #t ** 4 * dy ** 7 * dz ** 5 * dx ** 6 - 0.2700E4 * cc ** 3 * dt ** 
     #4 * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0
     #.864000E6
      cuv(529) = (-0.150E3 * cc ** 5 * dt ** 6 * dy ** 7 * dz ** 3 * dx 
     #** 6 - 0.150E3 * cc ** 5 * dt ** 6 * dx ** 3 * dy ** 7 * dz ** 6 +
     # 0.375E3 * cc ** 3 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 6 + 0.375
     #E3 * cc ** 3 * dt ** 4 * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / 
     #dy ** 7 / dz ** 7 / 0.864000E6
      cuv(535) = -cc ** 5 * dt ** 6 / dy ** 2 / dz ** 2 / dx / 0.2880E4
      cuv(536) = (0.522E3 * cc ** 6 * dt ** 7 * dy ** 5 * dz ** 7 * dx *
     #* 3 + 0.600E3 * cc ** 5 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 6 + 
     #0.600E3 * cc ** 5 * dt ** 6 * dy ** 3 * dz ** 7 * dx ** 6 + 0.1800
     #E4 * cc ** 5 * dt ** 6 * dy ** 5 * dz ** 7 * dx ** 4 + 0.600E3 * c
     #c ** 5 * dt ** 6 * dx ** 3 * dz ** 7 * dy ** 6 - 0.1050E4 * cc ** 
     #4 * dt ** 5 * dy ** 5 * dz ** 7 * dx ** 5 - 0.6000E4 * cc ** 3 * d
     #t ** 4 * dy ** 5 * dz ** 7 * dx ** 6 - 0.2700E4 * cc ** 3 * dt ** 
     #4 * dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0
     #.864000E6
      cuv(537) = -cc ** 5 * dt ** 6 / dy ** 2 / dz ** 2 / dx / 0.2880E4
      cuv(545) = (-0.150E3 * cc ** 5 * dt ** 6 * dy ** 3 * dz ** 7 * dx 
     #** 6 - 0.150E3 * cc ** 5 * dt ** 6 * dx ** 3 * dz ** 7 * dy ** 6 +
     # 0.375E3 * cc ** 3 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 6 + 0.375
     #E3 * cc ** 3 * dt ** 4 * dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / 
     #dy ** 7 / dz ** 7 / 0.864000E6
      cuv(599) = (-0.300E3 * cc ** 5 * dt ** 6 * dy ** 5 * dz ** 7 * dx 
     #** 4 + 0.750E3 * cc ** 3 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 6 +
     # 0.200E3 * cc ** 3 * dt ** 4 * dx ** 5 * dz ** 7 * dy ** 6) / dx *
     #* 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(607) = (-0.300E3 * cc ** 5 * dt ** 6 * dy ** 7 * dz ** 5 * dx 
     #** 4 + 0.750E3 * cc ** 3 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 6 +
     # 0.200E3 * cc ** 3 * dt ** 4 * dx ** 5 * dy ** 7 * dz ** 6) / dx *
     #* 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(608) = (0.174E3 * cc ** 6 * dt ** 7 * dy ** 7 * dz ** 7 * dx +
     # 0.600E3 * cc ** 5 * dt ** 6 * dy ** 7 * dz ** 5 * dx ** 4 + 0.600
     #E3 * cc ** 5 * dt ** 6 * dy ** 5 * dz ** 7 * dx ** 4 + 0.1200E4 * 
     #cc ** 5 * dt ** 6 * dy ** 7 * dz ** 7 * dx ** 2 - 0.1050E4 * cc **
     # 4 * dt ** 5 * dy ** 7 * dz ** 7 * dx ** 3 - 0.1500E4 * cc ** 3 * 
     #dt ** 4 * dy ** 7 * dz ** 5 * dx ** 6 - 0.1500E4 * cc ** 3 * dt **
     # 4 * dy ** 5 * dz ** 7 * dx ** 6 - 0.400E3 * cc ** 3 * dt ** 4 * d
     #x ** 5 * dy ** 7 * dz ** 6 - 0.400E3 * cc ** 3 * dt ** 4 * dx ** 5
     # * dz ** 7 * dy ** 6 - 0.10600E5 * cc ** 3 * dt ** 4 * dy ** 7 * d
     #z ** 7 * dx ** 4 + 0.1600E4 * cc ** 2 * dt ** 3 * dy ** 7 * dz ** 
     #7 * dx ** 5 + 0.12800E5 * cc * dt ** 2 * dy ** 7 * dz ** 7 * dx **
     # 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(609) = (-0.300E3 * cc ** 5 * dt ** 6 * dy ** 7 * dz ** 5 * dx 
     #** 4 + 0.750E3 * cc ** 3 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 6 +
     # 0.200E3 * cc ** 3 * dt ** 4 * dx ** 5 * dy ** 7 * dz ** 6) / dx *
     #* 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(617) = (-0.300E3 * cc ** 5 * dt ** 6 * dy ** 5 * dz ** 7 * dx 
     #** 4 + 0.750E3 * cc ** 3 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 6 +
     # 0.200E3 * cc ** 3 * dt ** 4 * dx ** 5 * dz ** 7 * dy ** 6) / dx *
     #* 7 / dy ** 7 / dz ** 7 / 0.864000E6
      cuv(689) = (-0.150E3 * cc ** 5 * dt ** 6 * dy ** 7 * dz ** 7 * dx 
     #** 2 + 0.1325E4 * cc ** 3 * dt ** 4 * dy ** 7 * dz ** 7 * dx ** 4 
     #- 0.1600E4 * cc * dt ** 2 * dy ** 7 * dz ** 7 * dx ** 6) / dx ** 7
     # / dy ** 7 / dz ** 7 / 0.864000E6
c
      return
      end
c
c++++++++++++++++
c
      subroutine getcvu_sixth3D( 
     *   cc,dx,dy,dz,dt,cvu )
c
      implicit real (t)
      real cvu(1:*)
      real dx,dy,dz,dt,cc
      integer i
c
      do i = 1,9**3
        cvu(i) = 0.0
      end do
c
      cvu(41) = (0.12E2 * cc ** 7 * dt ** 6 * dy ** 7 * dz ** 7 - 0.120E
     #3 * cc ** 5 * dt ** 4 * dy ** 7 * dz ** 7 * dx ** 2 + 0.252E3 * cc
     # ** 3 * dt ** 2 * dy ** 7 * dz ** 7 * dx ** 4) / dx ** 7 / dy ** 7
     # / dz ** 7 / 0.17280E5
      cvu(113) = (0.36E2 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 7 * dx **
     # 2 + 0.12E2 * cc ** 7 * dt ** 6 * dx * dz ** 7 * dy ** 6 - 0.180E3
     # * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 4 - 0.60E2 * cc *
     #* 5 * dt ** 4 * dx ** 3 * dz ** 7 * dy ** 6 + 0.144E3 * cc ** 3 * 
     #dt ** 2 * dy ** 5 * dz ** 7 * dx ** 6 + 0.48E2 * cc ** 3 * dt ** 2
     # * dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.
     #17280E5
      cvu(121) = (0.36E2 * cc ** 7 * dt ** 6 * dy ** 7 * dz ** 5 * dx **
     # 2 + 0.12E2 * cc ** 7 * dt ** 6 * dx * dy ** 7 * dz ** 6 - 0.180E3
     # * cc ** 5 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 4 - 0.60E2 * cc *
     #* 5 * dt ** 4 * dx ** 3 * dy ** 7 * dz ** 6 + 0.144E3 * cc ** 3 * 
     #dt ** 2 * dy ** 7 * dz ** 5 * dx ** 6 + 0.48E2 * cc ** 3 * dt ** 2
     # * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.
     #17280E5
      cvu(122) = (-0.72E2 * cc ** 7 * dt ** 6 * dy ** 7 * dz ** 5 * dx *
     #* 2 - 0.72E2 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 7 * dx ** 2 - 0
     #.24E2 * cc ** 7 * dt ** 6 * dx * dy ** 7 * dz ** 6 - 0.24E2 * cc *
     #* 7 * dt ** 6 * dx * dz ** 7 * dy ** 6 - 0.96E2 * cc ** 7 * dt ** 
     #6 * dy ** 7 * dz ** 7 + 0.144E3 * cc ** 6 * dt ** 5 * dy ** 7 * dz
     # ** 7 * dx + 0.360E3 * cc ** 5 * dt ** 4 * dy ** 7 * dz ** 5 * dx 
     #** 4 + 0.360E3 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 4 +
     # 0.120E3 * cc ** 5 * dt ** 4 * dx ** 3 * dy ** 7 * dz ** 6 + 0.120
     #E3 * cc ** 5 * dt ** 4 * dx ** 3 * dz ** 7 * dy ** 6 + 0.960E3 * c
     #c ** 5 * dt ** 4 * dy ** 7 * dz ** 7 * dx ** 2 - 0.480E3 * cc ** 4
     # * dt ** 3 * dy ** 7 * dz ** 7 * dx ** 3 - 0.288E3 * cc ** 3 * dt 
     #** 2 * dy ** 7 * dz ** 5 * dx ** 6 - 0.288E3 * cc ** 3 * dt ** 2 *
     # dy ** 5 * dz ** 7 * dx ** 6 - 0.96E2 * cc ** 3 * dt ** 2 * dx ** 
     #5 * dy ** 7 * dz ** 6 - 0.96E2 * cc ** 3 * dt ** 2 * dx ** 5 * dz 
     #** 7 * dy ** 6 - 0.2016E4 * cc ** 3 * dt ** 2 * dy ** 7 * dz ** 7 
     #* dx ** 4 + 0.192E3 * cc ** 2 * dt * dy ** 7 * dz ** 7 * dx ** 5) 
     #/ dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvu(123) = (0.36E2 * cc ** 7 * dt ** 6 * dy ** 7 * dz ** 5 * dx **
     # 2 + 0.12E2 * cc ** 7 * dt ** 6 * dx * dy ** 7 * dz ** 6 - 0.180E3
     # * cc ** 5 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 4 - 0.60E2 * cc *
     #* 5 * dt ** 4 * dx ** 3 * dy ** 7 * dz ** 6 + 0.144E3 * cc ** 3 * 
     #dt ** 2 * dy ** 7 * dz ** 5 * dx ** 6 + 0.48E2 * cc ** 3 * dt ** 2
     # * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.
     #17280E5
      cvu(131) = (0.36E2 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 7 * dx **
     # 2 + 0.12E2 * cc ** 7 * dt ** 6 * dx * dz ** 7 * dy ** 6 - 0.180E3
     # * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 4 - 0.60E2 * cc *
     #* 5 * dt ** 4 * dx ** 3 * dz ** 7 * dy ** 6 + 0.144E3 * cc ** 3 * 
     #dt ** 2 * dy ** 5 * dz ** 7 * dx ** 6 + 0.48E2 * cc ** 3 * dt ** 2
     # * dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.
     #17280E5
      cvu(185) = (0.36E2 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 7 * dx **
     # 4 + 0.36E2 * cc ** 7 * dt ** 6 * dx ** 3 * dz ** 7 * dy ** 4 - 0.
     #60E2 * cc ** 5 * dt ** 4 * dy ** 3 * dz ** 7 * dx ** 6 - 0.60E2 * 
     #cc ** 5 * dt ** 4 * dx ** 5 * dz ** 7 * dy ** 4 - 0.60E2 * cc ** 5
     # * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 4 - 0.60E2 * cc ** 5 * dt *
     #* 4 * dx ** 3 * dz ** 7 * dy ** 6 + 0.60E2 * cc ** 3 * dt ** 2 * d
     #y ** 5 * dz ** 7 * dx ** 6 + 0.60E2 * cc ** 3 * dt ** 2 * dx ** 5 
     #* dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvu(193) = (0.72E2 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 5 * dx **
     # 4 + 0.36E2 * cc ** 7 * dt ** 6 * dx ** 3 * dz ** 5 * dy ** 6 + 0.
     #36E2 * cc ** 7 * dt ** 6 * dx ** 3 * dy ** 5 * dz ** 6 - 0.120E3 *
     # cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx ** 6 - 0.60E2 * cc ** 
     #5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 - 0.60E2 * cc ** 5 * dt 
     #** 4 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 
     #/ 0.17280E5
      cvu(194) = (-0.144E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 5 * dx 
     #** 4 - 0.144E3 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 7 * dx ** 4 -
     # 0.72E2 * cc ** 7 * dt ** 6 * dx ** 3 * dz ** 5 * dy ** 6 - 0.72E2
     # * cc ** 7 * dt ** 6 * dx ** 3 * dy ** 5 * dz ** 6 - 0.144E3 * cc 
     #** 7 * dt ** 6 * dx ** 3 * dz ** 7 * dy ** 4 - 0.216E3 * cc ** 7 *
     # dt ** 6 * dy ** 5 * dz ** 7 * dx ** 2 - 0.72E2 * cc ** 7 * dt ** 
     #6 * dx * dz ** 7 * dy ** 6 + 0.432E3 * cc ** 6 * dt ** 5 * dy ** 5
     # * dz ** 7 * dx ** 3 + 0.240E3 * cc ** 5 * dt ** 4 * dy ** 5 * dz 
     #** 5 * dx ** 6 + 0.240E3 * cc ** 5 * dt ** 4 * dy ** 3 * dz ** 7 *
     # dx ** 6 + 0.120E3 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy **
     # 6 + 0.120E3 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6 + 0
     #.240E3 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 7 * dy ** 4 + 0.1320E
     #4 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 4 + 0.600E3 * cc
     # ** 5 * dt ** 4 * dx ** 3 * dz ** 7 * dy ** 6 - 0.480E3 * cc ** 4 
     #* dt ** 3 * dy ** 5 * dz ** 7 * dx ** 5 - 0.1104E4 * cc ** 3 * dt 
     #** 2 * dy ** 5 * dz ** 7 * dx ** 6 - 0.528E3 * cc ** 3 * dt ** 2 *
     # dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17
     #280E5
      cvu(195) = (0.72E2 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 5 * dx **
     # 4 + 0.36E2 * cc ** 7 * dt ** 6 * dx ** 3 * dz ** 5 * dy ** 6 + 0.
     #36E2 * cc ** 7 * dt ** 6 * dx ** 3 * dy ** 5 * dz ** 6 - 0.120E3 *
     # cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx ** 6 - 0.60E2 * cc ** 
     #5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 - 0.60E2 * cc ** 5 * dt 
     #** 4 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 
     #/ 0.17280E5
      cvu(201) = (0.36E2 * cc ** 7 * dt ** 6 * dy ** 7 * dz ** 3 * dx **
     # 4 + 0.36E2 * cc ** 7 * dt ** 6 * dx ** 3 * dy ** 7 * dz ** 4 - 0.
     #60E2 * cc ** 5 * dt ** 4 * dy ** 7 * dz ** 3 * dx ** 6 - 0.60E2 * 
     #cc ** 5 * dt ** 4 * dx ** 5 * dy ** 7 * dz ** 4 - 0.60E2 * cc ** 5
     # * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 4 - 0.60E2 * cc ** 5 * dt *
     #* 4 * dx ** 3 * dy ** 7 * dz ** 6 + 0.60E2 * cc ** 3 * dt ** 2 * d
     #y ** 7 * dz ** 5 * dx ** 6 + 0.60E2 * cc ** 3 * dt ** 2 * dx ** 5 
     #* dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvu(202) = (-0.144E3 * cc ** 7 * dt ** 6 * dy ** 7 * dz ** 3 * dx 
     #** 4 - 0.144E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 4 -
     # 0.144E3 * cc ** 7 * dt ** 6 * dx ** 3 * dy ** 7 * dz ** 4 - 0.72E
     #2 * cc ** 7 * dt ** 6 * dx ** 3 * dz ** 5 * dy ** 6 - 0.72E2 * cc 
     #** 7 * dt ** 6 * dx ** 3 * dy ** 5 * dz ** 6 - 0.216E3 * cc ** 7 *
     # dt ** 6 * dy ** 7 * dz ** 5 * dx ** 2 - 0.72E2 * cc ** 7 * dt ** 
     #6 * dx * dy ** 7 * dz ** 6 + 0.432E3 * cc ** 6 * dt ** 5 * dy ** 7
     # * dz ** 5 * dx ** 3 + 0.240E3 * cc ** 5 * dt ** 4 * dy ** 7 * dz 
     #** 3 * dx ** 6 + 0.240E3 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 *
     # dx ** 6 + 0.240E3 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 7 * dz **
     # 4 + 0.120E3 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 + 0
     #.120E3 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6 + 0.1320E
     #4 * cc ** 5 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 4 + 0.600E3 * cc
     # ** 5 * dt ** 4 * dx ** 3 * dy ** 7 * dz ** 6 - 0.480E3 * cc ** 4 
     #* dt ** 3 * dy ** 7 * dz ** 5 * dx ** 5 - 0.1104E4 * cc ** 3 * dt 
     #** 2 * dy ** 7 * dz ** 5 * dx ** 6 - 0.528E3 * cc ** 3 * dt ** 2 *
     # dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17
     #280E5
      cvu(203) = (0.216E3 * cc ** 7 * dt ** 6 * dy ** 7 * dz ** 3 * dx *
     #* 4 + 0.288E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 4 + 
     #0.216E3 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 7 * dx ** 4 + 0.216E
     #3 * cc ** 7 * dt ** 6 * dx ** 3 * dy ** 7 * dz ** 4 + 0.144E3 * cc
     # ** 7 * dt ** 6 * dx ** 3 * dz ** 5 * dy ** 6 + 0.144E3 * cc ** 7 
     #* dt ** 6 * dx ** 3 * dy ** 5 * dz ** 6 + 0.216E3 * cc ** 7 * dt *
     #* 6 * dx ** 3 * dz ** 7 * dy ** 4 + 0.432E3 * cc ** 7 * dt ** 6 * 
     #dy ** 7 * dz ** 5 * dx ** 2 + 0.432E3 * cc ** 7 * dt ** 6 * dy ** 
     #5 * dz ** 7 * dx ** 2 + 0.144E3 * cc ** 7 * dt ** 6 * dx * dy ** 7
     # * dz ** 6 + 0.144E3 * cc ** 7 * dt ** 6 * dx * dz ** 7 * dy ** 6 
     #+ 0.336E3 * cc ** 7 * dt ** 6 * dy ** 7 * dz ** 7 - 0.864E3 * cc *
     #* 6 * dt ** 5 * dy ** 7 * dz ** 5 * dx ** 3 - 0.864E3 * cc ** 6 * 
     #dt ** 5 * dy ** 5 * dz ** 7 * dx ** 3 - 0.864E3 * cc ** 6 * dt ** 
     #5 * dy ** 7 * dz ** 7 * dx - 0.360E3 * cc ** 5 * dt ** 4 * dy ** 7
     # * dz ** 3 * dx ** 6 - 0.480E3 * cc ** 5 * dt ** 4 * dy ** 5 * dz 
     #** 5 * dx ** 6 - 0.360E3 * cc ** 5 * dt ** 4 * dy ** 3 * dz ** 7 *
     # dx ** 6 - 0.360E3 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 7 * dz **
     # 4 - 0.240E3 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 - 0
     #.240E3 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6 - 0.360E3
     # * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 7 * dy ** 4 - 0.2520E4 * cc
     # ** 5 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 4 - 0.2520E4 * cc ** 5
     # * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 4 - 0.1080E4 * cc ** 5 * dt
     # ** 4 * dx ** 3 * dy ** 7 * dz ** 6 - 0.1080E4 * cc ** 5 * dt ** 4
     # * dx ** 3 * dz ** 7 * dy ** 6 - 0.3360E4 * cc ** 5 * dt ** 4 * dy
     # ** 7 * dz ** 7 * dx ** 2 + 0.960E3 * cc ** 4 * dt ** 3 * dy ** 7 
     #* dz ** 5 * dx ** 5 + 0.960E3 * cc ** 4 * dt ** 3 * dy ** 5 * dz *
     #* 7 * dx ** 5 + 0.5760E4 * cc ** 4 * dt ** 3 * dy ** 7 * dz ** 7 *
     # dx ** 3 + 0.2088E4 * cc ** 3 * dt ** 2 * dy ** 7 * dz ** 5 * dx *
     #* 6 + 0.2088E4 * cc ** 3 * dt ** 2 * dy ** 5 * dz ** 7 * dx ** 6 +
     # 0.936E3 * cc ** 3 * dt ** 2 * dx ** 5 * dy ** 7 * dz ** 6 + 0.936
     #E3 * cc ** 3 * dt ** 2 * dx ** 5 * dz ** 7 * dy ** 6 + 0.7056E4 * 
     #cc ** 3 * dt ** 2 * dy ** 7 * dz ** 7 * dx ** 4 - 0.2592E4 * cc **
     # 2 * dt * dy ** 7 * dz ** 7 * dx ** 5) / dx ** 7 / dy ** 7 / dz **
     # 7 / 0.17280E5
      cvu(204) = (-0.144E3 * cc ** 7 * dt ** 6 * dy ** 7 * dz ** 3 * dx 
     #** 4 - 0.144E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 4 -
     # 0.144E3 * cc ** 7 * dt ** 6 * dx ** 3 * dy ** 7 * dz ** 4 - 0.72E
     #2 * cc ** 7 * dt ** 6 * dx ** 3 * dz ** 5 * dy ** 6 - 0.72E2 * cc 
     #** 7 * dt ** 6 * dx ** 3 * dy ** 5 * dz ** 6 - 0.216E3 * cc ** 7 *
     # dt ** 6 * dy ** 7 * dz ** 5 * dx ** 2 - 0.72E2 * cc ** 7 * dt ** 
     #6 * dx * dy ** 7 * dz ** 6 + 0.432E3 * cc ** 6 * dt ** 5 * dy ** 7
     # * dz ** 5 * dx ** 3 + 0.240E3 * cc ** 5 * dt ** 4 * dy ** 7 * dz 
     #** 3 * dx ** 6 + 0.240E3 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 *
     # dx ** 6 + 0.240E3 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 7 * dz **
     # 4 + 0.120E3 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 + 0
     #.120E3 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6 + 0.1320E
     #4 * cc ** 5 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 4 + 0.600E3 * cc
     # ** 5 * dt ** 4 * dx ** 3 * dy ** 7 * dz ** 6 - 0.480E3 * cc ** 4 
     #* dt ** 3 * dy ** 7 * dz ** 5 * dx ** 5 - 0.1104E4 * cc ** 3 * dt 
     #** 2 * dy ** 7 * dz ** 5 * dx ** 6 - 0.528E3 * cc ** 3 * dt ** 2 *
     # dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17
     #280E5
      cvu(205) = (0.36E2 * cc ** 7 * dt ** 6 * dy ** 7 * dz ** 3 * dx **
     # 4 + 0.36E2 * cc ** 7 * dt ** 6 * dx ** 3 * dy ** 7 * dz ** 4 - 0.
     #60E2 * cc ** 5 * dt ** 4 * dy ** 7 * dz ** 3 * dx ** 6 - 0.60E2 * 
     #cc ** 5 * dt ** 4 * dx ** 5 * dy ** 7 * dz ** 4 - 0.60E2 * cc ** 5
     # * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 4 - 0.60E2 * cc ** 5 * dt *
     #* 4 * dx ** 3 * dy ** 7 * dz ** 6 + 0.60E2 * cc ** 3 * dt ** 2 * d
     #y ** 7 * dz ** 5 * dx ** 6 + 0.60E2 * cc ** 3 * dt ** 2 * dx ** 5 
     #* dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvu(211) = (0.72E2 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 5 * dx **
     # 4 + 0.36E2 * cc ** 7 * dt ** 6 * dx ** 3 * dz ** 5 * dy ** 6 + 0.
     #36E2 * cc ** 7 * dt ** 6 * dx ** 3 * dy ** 5 * dz ** 6 - 0.120E3 *
     # cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx ** 6 - 0.60E2 * cc ** 
     #5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 - 0.60E2 * cc ** 5 * dt 
     #** 4 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 
     #/ 0.17280E5
      cvu(212) = (-0.144E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 5 * dx 
     #** 4 - 0.144E3 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 7 * dx ** 4 -
     # 0.72E2 * cc ** 7 * dt ** 6 * dx ** 3 * dz ** 5 * dy ** 6 - 0.72E2
     # * cc ** 7 * dt ** 6 * dx ** 3 * dy ** 5 * dz ** 6 - 0.144E3 * cc 
     #** 7 * dt ** 6 * dx ** 3 * dz ** 7 * dy ** 4 - 0.216E3 * cc ** 7 *
     # dt ** 6 * dy ** 5 * dz ** 7 * dx ** 2 - 0.72E2 * cc ** 7 * dt ** 
     #6 * dx * dz ** 7 * dy ** 6 + 0.432E3 * cc ** 6 * dt ** 5 * dy ** 5
     # * dz ** 7 * dx ** 3 + 0.240E3 * cc ** 5 * dt ** 4 * dy ** 5 * dz 
     #** 5 * dx ** 6 + 0.240E3 * cc ** 5 * dt ** 4 * dy ** 3 * dz ** 7 *
     # dx ** 6 + 0.120E3 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy **
     # 6 + 0.120E3 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6 + 0
     #.240E3 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 7 * dy ** 4 + 0.1320E
     #4 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 4 + 0.600E3 * cc
     # ** 5 * dt ** 4 * dx ** 3 * dz ** 7 * dy ** 6 - 0.480E3 * cc ** 4 
     #* dt ** 3 * dy ** 5 * dz ** 7 * dx ** 5 - 0.1104E4 * cc ** 3 * dt 
     #** 2 * dy ** 5 * dz ** 7 * dx ** 6 - 0.528E3 * cc ** 3 * dt ** 2 *
     # dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17
     #280E5
      cvu(213) = (0.72E2 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 5 * dx **
     # 4 + 0.36E2 * cc ** 7 * dt ** 6 * dx ** 3 * dz ** 5 * dy ** 6 + 0.
     #36E2 * cc ** 7 * dt ** 6 * dx ** 3 * dy ** 5 * dz ** 6 - 0.120E3 *
     # cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx ** 6 - 0.60E2 * cc ** 
     #5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 - 0.60E2 * cc ** 5 * dt 
     #** 4 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 
     #/ 0.17280E5
      cvu(221) = (0.36E2 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 7 * dx **
     # 4 + 0.36E2 * cc ** 7 * dt ** 6 * dx ** 3 * dz ** 7 * dy ** 4 - 0.
     #60E2 * cc ** 5 * dt ** 4 * dy ** 3 * dz ** 7 * dx ** 6 - 0.60E2 * 
     #cc ** 5 * dt ** 4 * dx ** 5 * dz ** 7 * dy ** 4 - 0.60E2 * cc ** 5
     # * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 4 - 0.60E2 * cc ** 5 * dt *
     #* 4 * dx ** 3 * dz ** 7 * dy ** 6 + 0.60E2 * cc ** 3 * dt ** 2 * d
     #y ** 5 * dz ** 7 * dx ** 6 + 0.60E2 * cc ** 3 * dt ** 2 * dx ** 5 
     #* dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvu(257) = (0.12E2 * cc ** 7 * dt ** 6 * dy * dz ** 7 * dx ** 6 + 
     #0.36E2 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 7 * dy ** 2 - 0.60E2 
     #* cc ** 5 * dt ** 4 * dy ** 3 * dz ** 7 * dx ** 6 - 0.180E3 * cc *
     #* 5 * dt ** 4 * dx ** 5 * dz ** 7 * dy ** 4 + 0.48E2 * cc ** 3 * d
     #t ** 2 * dy ** 5 * dz ** 7 * dx ** 6 + 0.144E3 * cc ** 3 * dt ** 2
     # * dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.
     #17280E5
      cvu(265) = (0.36E2 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 5 * dx **
     # 6 + 0.72E2 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 4 + 0.
     #36E2 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 3 * dz ** 6 - 0.60E2 * 
     #cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx ** 6 - 0.120E3 * cc ** 
     #5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 - 0.60E2 * cc ** 5 * dt 
     #** 4 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 
     #/ 0.17280E5
      cvu(266) = (-0.72E2 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 5 * dx *
     #* 6 - 0.72E2 * cc ** 7 * dt ** 6 * dy * dz ** 7 * dx ** 6 - 0.144E
     #3 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 4 - 0.72E2 * cc 
     #** 7 * dt ** 6 * dx ** 5 * dy ** 3 * dz ** 6 - 0.216E3 * cc ** 7 *
     # dt ** 6 * dx ** 5 * dz ** 7 * dy ** 2 - 0.144E3 * cc ** 7 * dt **
     # 6 * dy ** 3 * dz ** 7 * dx ** 4 - 0.144E3 * cc ** 7 * dt ** 6 * d
     #x ** 3 * dz ** 7 * dy ** 4 + 0.432E3 * cc ** 6 * dt ** 5 * dy ** 3
     # * dz ** 7 * dx ** 5 + 0.120E3 * cc ** 5 * dt ** 4 * dy ** 5 * dz 
     #** 5 * dx ** 6 + 0.600E3 * cc ** 5 * dt ** 4 * dy ** 3 * dz ** 7 *
     # dx ** 6 + 0.240E3 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy **
     # 6 + 0.120E3 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6 + 0
     #.1320E4 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 7 * dy ** 4 + 0.240E
     #3 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 4 + 0.240E3 * cc
     # ** 5 * dt ** 4 * dx ** 3 * dz ** 7 * dy ** 6 - 0.480E3 * cc ** 4 
     #* dt ** 3 * dy ** 5 * dz ** 7 * dx ** 5 - 0.528E3 * cc ** 3 * dt *
     #* 2 * dy ** 5 * dz ** 7 * dx ** 6 - 0.1104E4 * cc ** 3 * dt ** 2 *
     # dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17
     #280E5
      cvu(267) = (0.36E2 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 5 * dx **
     # 6 + 0.72E2 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 4 + 0.
     #36E2 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 3 * dz ** 6 - 0.60E2 * 
     #cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx ** 6 - 0.120E3 * cc ** 
     #5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 - 0.60E2 * cc ** 5 * dt 
     #** 4 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 
     #/ 0.17280E5
      cvu(273) = (0.36E2 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 3 * dx **
     # 6 + 0.36E2 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 3 * dy ** 6 + 0.
     #72E2 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 4 - 0.60E2 * 
     #cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx ** 6 - 0.60E2 * cc ** 5
     # * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 - 0.120E3 * cc ** 5 * dt 
     #** 4 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 
     #/ 0.17280E5
      cvu(274) = (-0.144E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 3 * dx 
     #** 6 - 0.144E3 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 5 * dx ** 6 -
     # 0.144E3 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 3 * dy ** 6 - 0.288
     #E3 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 4 - 0.288E3 * c
     #c ** 7 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 4 - 0.144E3 * cc ** 7
     # * dt ** 6 * dx ** 5 * dy ** 3 * dz ** 6 - 0.288E3 * cc ** 7 * dt 
     #** 6 * dy ** 5 * dz ** 5 * dx ** 4 - 0.144E3 * cc ** 7 * dt ** 6 *
     # dx ** 3 * dz ** 5 * dy ** 6 - 0.144E3 * cc ** 7 * dt ** 6 * dx **
     # 3 * dy ** 5 * dz ** 6 + 0.864E3 * cc ** 6 * dt ** 5 * dy ** 5 * d
     #z ** 5 * dx ** 5 + 0.960E3 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5
     # * dx ** 6 + 0.960E3 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy 
     #** 6 + 0.960E3 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6) 
     #/ dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvu(275) = (0.216E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 3 * dx *
     #* 6 + 0.288E3 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 5 * dx ** 6 + 
     #0.180E3 * cc ** 7 * dt ** 6 * dy * dz ** 7 * dx ** 6 + 0.216E3 * c
     #c ** 7 * dt ** 6 * dx ** 5 * dz ** 3 * dy ** 6 + 0.432E3 * cc ** 7
     # * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 4 + 0.576E3 * cc ** 7 * dt 
     #** 6 * dx ** 5 * dz ** 5 * dy ** 4 + 0.288E3 * cc ** 7 * dt ** 6 *
     # dx ** 5 * dy ** 3 * dz ** 6 + 0.540E3 * cc ** 7 * dt ** 6 * dx **
     # 5 * dz ** 7 * dy ** 2 + 0.576E3 * cc ** 7 * dt ** 6 * dy ** 5 * d
     #z ** 5 * dx ** 4 + 0.576E3 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 7
     # * dx ** 4 + 0.288E3 * cc ** 7 * dt ** 6 * dx ** 3 * dz ** 5 * dy 
     #** 6 + 0.288E3 * cc ** 7 * dt ** 6 * dx ** 3 * dy ** 5 * dz ** 6 +
     # 0.576E3 * cc ** 7 * dt ** 6 * dx ** 3 * dz ** 7 * dy ** 4 + 0.540
     #E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 7 * dx ** 2 + 0.180E3 * c
     #c ** 7 * dt ** 6 * dx * dz ** 7 * dy ** 6 - 0.1728E4 * cc ** 6 * d
     #t ** 5 * dy ** 5 * dz ** 5 * dx ** 5 - 0.1728E4 * cc ** 6 * dt ** 
     #5 * dy ** 3 * dz ** 7 * dx ** 5 - 0.1728E4 * cc ** 6 * dt ** 5 * d
     #y ** 5 * dz ** 7 * dx ** 3 - 0.1800E4 * cc ** 5 * dt ** 4 * dy ** 
     #5 * dz ** 5 * dx ** 6 - 0.1860E4 * cc ** 5 * dt ** 4 * dy ** 3 * d
     #z ** 7 * dx ** 6 - 0.1800E4 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 
     #5 * dy ** 6 - 0.1680E4 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * d
     #z ** 6 - 0.3660E4 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 7 * dy ** 
     #4 - 0.3660E4 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 4 - 0
     #.1860E4 * cc ** 5 * dt ** 4 * dx ** 3 * dz ** 7 * dy ** 6 + 0.9600
     #E4 * cc ** 4 * dt ** 3 * dy ** 5 * dz ** 7 * dx ** 5 + 0.3840E4 * 
     #cc ** 3 * dt ** 2 * dy ** 5 * dz ** 7 * dx ** 6 + 0.3840E4 * cc **
     # 3 * dt ** 2 * dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / 
     #dz ** 7 / 0.17280E5
      cvu(276) = (-0.144E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 3 * dx 
     #** 6 - 0.144E3 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 5 * dx ** 6 -
     # 0.144E3 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 3 * dy ** 6 - 0.288
     #E3 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 4 - 0.288E3 * c
     #c ** 7 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 4 - 0.144E3 * cc ** 7
     # * dt ** 6 * dx ** 5 * dy ** 3 * dz ** 6 - 0.288E3 * cc ** 7 * dt 
     #** 6 * dy ** 5 * dz ** 5 * dx ** 4 - 0.144E3 * cc ** 7 * dt ** 6 *
     # dx ** 3 * dz ** 5 * dy ** 6 - 0.144E3 * cc ** 7 * dt ** 6 * dx **
     # 3 * dy ** 5 * dz ** 6 + 0.864E3 * cc ** 6 * dt ** 5 * dy ** 5 * d
     #z ** 5 * dx ** 5 + 0.960E3 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5
     # * dx ** 6 + 0.960E3 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy 
     #** 6 + 0.960E3 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6) 
     #/ dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvu(277) = (0.36E2 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 3 * dx **
     # 6 + 0.36E2 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 3 * dy ** 6 + 0.
     #72E2 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 4 - 0.60E2 * 
     #cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx ** 6 - 0.60E2 * cc ** 5
     # * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 - 0.120E3 * cc ** 5 * dt 
     #** 4 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 
     #/ 0.17280E5
      cvu(281) = (0.12E2 * cc ** 7 * dt ** 6 * dy ** 7 * dz * dx ** 6 + 
     #0.36E2 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 7 * dz ** 2 - 0.60E2 
     #* cc ** 5 * dt ** 4 * dy ** 7 * dz ** 3 * dx ** 6 - 0.180E3 * cc *
     #* 5 * dt ** 4 * dx ** 5 * dy ** 7 * dz ** 4 + 0.48E2 * cc ** 3 * d
     #t ** 2 * dy ** 7 * dz ** 5 * dx ** 6 + 0.144E3 * cc ** 3 * dt ** 2
     # * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.
     #17280E5
      cvu(282) = (-0.72E2 * cc ** 7 * dt ** 6 * dy ** 7 * dz * dx ** 6 -
     # 0.72E2 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 3 * dx ** 6 - 0.216E
     #3 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 7 * dz ** 2 - 0.72E2 * cc 
     #** 7 * dt ** 6 * dx ** 5 * dz ** 3 * dy ** 6 - 0.144E3 * cc ** 7 *
     # dt ** 6 * dx ** 5 * dy ** 5 * dz ** 4 - 0.144E3 * cc ** 7 * dt **
     # 6 * dy ** 7 * dz ** 3 * dx ** 4 - 0.144E3 * cc ** 7 * dt ** 6 * d
     #x ** 3 * dy ** 7 * dz ** 4 + 0.432E3 * cc ** 6 * dt ** 5 * dy ** 7
     # * dz ** 3 * dx ** 5 + 0.600E3 * cc ** 5 * dt ** 4 * dy ** 7 * dz 
     #** 3 * dx ** 6 + 0.120E3 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 *
     # dx ** 6 + 0.1320E4 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 7 * dz *
     #* 4 + 0.120E3 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 + 
     #0.240E3 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6 + 0.240E
     #3 * cc ** 5 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 4 + 0.240E3 * cc
     # ** 5 * dt ** 4 * dx ** 3 * dy ** 7 * dz ** 6 - 0.480E3 * cc ** 4 
     #* dt ** 3 * dy ** 7 * dz ** 5 * dx ** 5 - 0.528E3 * cc ** 3 * dt *
     #* 2 * dy ** 7 * dz ** 5 * dx ** 6 - 0.1104E4 * cc ** 3 * dt ** 2 *
     # dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17
     #280E5
      cvu(283) = (0.180E3 * cc ** 7 * dt ** 6 * dy ** 7 * dz * dx ** 6 +
     # 0.288E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 3 * dx ** 6 + 0.216
     #E3 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 5 * dx ** 6 + 0.540E3 * c
     #c ** 7 * dt ** 6 * dx ** 5 * dy ** 7 * dz ** 2 + 0.288E3 * cc ** 7
     # * dt ** 6 * dx ** 5 * dz ** 3 * dy ** 6 + 0.576E3 * cc ** 7 * dt 
     #** 6 * dx ** 5 * dy ** 5 * dz ** 4 + 0.432E3 * cc ** 7 * dt ** 6 *
     # dx ** 5 * dz ** 5 * dy ** 4 + 0.216E3 * cc ** 7 * dt ** 6 * dx **
     # 5 * dy ** 3 * dz ** 6 + 0.576E3 * cc ** 7 * dt ** 6 * dy ** 7 * d
     #z ** 3 * dx ** 4 + 0.576E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 5
     # * dx ** 4 + 0.576E3 * cc ** 7 * dt ** 6 * dx ** 3 * dy ** 7 * dz 
     #** 4 + 0.288E3 * cc ** 7 * dt ** 6 * dx ** 3 * dz ** 5 * dy ** 6 +
     # 0.288E3 * cc ** 7 * dt ** 6 * dx ** 3 * dy ** 5 * dz ** 6 + 0.540
     #E3 * cc ** 7 * dt ** 6 * dy ** 7 * dz ** 5 * dx ** 2 + 0.180E3 * c
     #c ** 7 * dt ** 6 * dx * dy ** 7 * dz ** 6 - 0.1728E4 * cc ** 6 * d
     #t ** 5 * dy ** 7 * dz ** 3 * dx ** 5 - 0.1728E4 * cc ** 6 * dt ** 
     #5 * dy ** 5 * dz ** 5 * dx ** 5 - 0.1728E4 * cc ** 6 * dt ** 5 * d
     #y ** 7 * dz ** 5 * dx ** 3 - 0.1860E4 * cc ** 5 * dt ** 4 * dy ** 
     #7 * dz ** 3 * dx ** 6 - 0.1800E4 * cc ** 5 * dt ** 4 * dy ** 5 * d
     #z ** 5 * dx ** 6 - 0.3660E4 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 
     #7 * dz ** 4 - 0.1680E4 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * d
     #y ** 6 - 0.1800E4 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 
     #6 - 0.3660E4 * cc ** 5 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 4 - 0
     #.1860E4 * cc ** 5 * dt ** 4 * dx ** 3 * dy ** 7 * dz ** 6 + 0.9600
     #E4 * cc ** 4 * dt ** 3 * dy ** 7 * dz ** 5 * dx ** 5 + 0.3840E4 * 
     #cc ** 3 * dt ** 2 * dy ** 7 * dz ** 5 * dx ** 6 + 0.3840E4 * cc **
     # 3 * dt ** 2 * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / 
     #dz ** 7 / 0.17280E5
      cvu(284) = (-0.240E3 * cc ** 7 * dt ** 6 * dy ** 7 * dz * dx ** 6 
     #- 0.432E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 3 * dx ** 6 - 0.43
     #2E3 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 5 * dx ** 6 - 0.240E3 * 
     #cc ** 7 * dt ** 6 * dy * dz ** 7 * dx ** 6 - 0.720E3 * cc ** 7 * d
     #t ** 6 * dx ** 5 * dy ** 7 * dz ** 2 - 0.432E3 * cc ** 7 * dt ** 6
     # * dx ** 5 * dz ** 3 * dy ** 6 - 0.864E3 * cc ** 7 * dt ** 6 * dx 
     #** 5 * dy ** 5 * dz ** 4 - 0.864E3 * cc ** 7 * dt ** 6 * dx ** 5 *
     # dz ** 5 * dy ** 4 - 0.432E3 * cc ** 7 * dt ** 6 * dx ** 5 * dy **
     # 3 * dz ** 6 - 0.720E3 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 7 * d
     #y ** 2 - 0.864E3 * cc ** 7 * dt ** 6 * dy ** 7 * dz ** 3 * dx ** 4
     # - 0.1152E4 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 4 - 0.
     #864E3 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 7 * dx ** 4 - 0.864E3 
     #* cc ** 7 * dt ** 6 * dx ** 3 * dy ** 7 * dz ** 4 - 0.576E3 * cc *
     #* 7 * dt ** 6 * dx ** 3 * dz ** 5 * dy ** 6 - 0.576E3 * cc ** 7 * 
     #dt ** 6 * dx ** 3 * dy ** 5 * dz ** 6 - 0.864E3 * cc ** 7 * dt ** 
     #6 * dx ** 3 * dz ** 7 * dy ** 4 - 0.1080E4 * cc ** 7 * dt ** 6 * d
     #y ** 7 * dz ** 5 * dx ** 2 - 0.1080E4 * cc ** 7 * dt ** 6 * dy ** 
     #5 * dz ** 7 * dx ** 2 - 0.360E3 * cc ** 7 * dt ** 6 * dx * dy ** 7
     # * dz ** 6 - 0.360E3 * cc ** 7 * dt ** 6 * dx * dz ** 7 * dy ** 6 
     #- 0.672E3 * cc ** 7 * dt ** 6 * dy ** 7 * dz ** 7 + 0.2592E4 * cc 
     #** 6 * dt ** 5 * dy ** 7 * dz ** 3 * dx ** 5 + 0.3456E4 * cc ** 6 
     #* dt ** 5 * dy ** 5 * dz ** 5 * dx ** 5 + 0.2592E4 * cc ** 6 * dt 
     #** 5 * dy ** 3 * dz ** 7 * dx ** 5 + 0.3456E4 * cc ** 6 * dt ** 5 
     #* dy ** 7 * dz ** 5 * dx ** 3 + 0.3456E4 * cc ** 6 * dt ** 5 * dy 
     #** 5 * dz ** 7 * dx ** 3 + 0.2160E4 * cc ** 6 * dt ** 5 * dy ** 7 
     #* dz ** 7 * dx + 0.2640E4 * cc ** 5 * dt ** 4 * dy ** 7 * dz ** 3 
     #* dx ** 6 + 0.3360E4 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx 
     #** 6 + 0.2640E4 * cc ** 5 * dt ** 4 * dy ** 3 * dz ** 7 * dx ** 6 
     #+ 0.5040E4 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 7 * dz ** 4 + 0.3
     #120E4 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 + 0.3120E4
     # * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6 + 0.5040E4 * cc
     # ** 5 * dt ** 4 * dx ** 5 * dz ** 7 * dy ** 4 + 0.6840E4 * cc ** 5
     # * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 4 + 0.6840E4 * cc ** 5 * dt
     # ** 4 * dy ** 5 * dz ** 7 * dx ** 4 + 0.3240E4 * cc ** 5 * dt ** 4
     # * dx ** 3 * dy ** 7 * dz ** 6 + 0.3240E4 * cc ** 5 * dt ** 4 * dx
     # ** 3 * dz ** 7 * dy ** 6 + 0.6720E4 * cc ** 5 * dt ** 4 * dy ** 7
     # * dz ** 7 * dx ** 2 - 0.18240E5 * cc ** 4 * dt ** 3 * dy ** 7 * d
     #z ** 5 * dx ** 5 - 0.18240E5 * cc ** 4 * dt ** 3 * dy ** 5 * dz **
     # 7 * dx ** 5 - 0.18720E5 * cc ** 4 * dt ** 3 * dy ** 7 * dz ** 7 *
     # dx ** 3 - 0.6720E4 * cc ** 3 * dt ** 2 * dy ** 7 * dz ** 5 * dx *
     #* 6 - 0.6720E4 * cc ** 3 * dt ** 2 * dy ** 5 * dz ** 7 * dx ** 6 -
     # 0.5760E4 * cc ** 3 * dt ** 2 * dx ** 5 * dy ** 7 * dz ** 6 - 0.57
     #60E4 * cc ** 3 * dt ** 2 * dx ** 5 * dz ** 7 * dy ** 6 - 0.14112E5
     # * cc ** 3 * dt ** 2 * dy ** 7 * dz ** 7 * dx ** 4 + 0.25920E5 * c
     #c ** 2 * dt * dy ** 7 * dz ** 7 * dx ** 5) / dx ** 7 / dy ** 7 / d
     #z ** 7 / 0.17280E5
      cvu(285) = (0.180E3 * cc ** 7 * dt ** 6 * dy ** 7 * dz * dx ** 6 +
     # 0.288E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 3 * dx ** 6 + 0.216
     #E3 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 5 * dx ** 6 + 0.540E3 * c
     #c ** 7 * dt ** 6 * dx ** 5 * dy ** 7 * dz ** 2 + 0.288E3 * cc ** 7
     # * dt ** 6 * dx ** 5 * dz ** 3 * dy ** 6 + 0.576E3 * cc ** 7 * dt 
     #** 6 * dx ** 5 * dy ** 5 * dz ** 4 + 0.432E3 * cc ** 7 * dt ** 6 *
     # dx ** 5 * dz ** 5 * dy ** 4 + 0.216E3 * cc ** 7 * dt ** 6 * dx **
     # 5 * dy ** 3 * dz ** 6 + 0.576E3 * cc ** 7 * dt ** 6 * dy ** 7 * d
     #z ** 3 * dx ** 4 + 0.576E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 5
     # * dx ** 4 + 0.576E3 * cc ** 7 * dt ** 6 * dx ** 3 * dy ** 7 * dz 
     #** 4 + 0.288E3 * cc ** 7 * dt ** 6 * dx ** 3 * dz ** 5 * dy ** 6 +
     # 0.288E3 * cc ** 7 * dt ** 6 * dx ** 3 * dy ** 5 * dz ** 6 + 0.540
     #E3 * cc ** 7 * dt ** 6 * dy ** 7 * dz ** 5 * dx ** 2 + 0.180E3 * c
     #c ** 7 * dt ** 6 * dx * dy ** 7 * dz ** 6 - 0.1728E4 * cc ** 6 * d
     #t ** 5 * dy ** 7 * dz ** 3 * dx ** 5 - 0.1728E4 * cc ** 6 * dt ** 
     #5 * dy ** 5 * dz ** 5 * dx ** 5 - 0.1728E4 * cc ** 6 * dt ** 5 * d
     #y ** 7 * dz ** 5 * dx ** 3 - 0.1860E4 * cc ** 5 * dt ** 4 * dy ** 
     #7 * dz ** 3 * dx ** 6 - 0.1800E4 * cc ** 5 * dt ** 4 * dy ** 5 * d
     #z ** 5 * dx ** 6 - 0.3660E4 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 
     #7 * dz ** 4 - 0.1680E4 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * d
     #y ** 6 - 0.1800E4 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 
     #6 - 0.3660E4 * cc ** 5 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 4 - 0
     #.1860E4 * cc ** 5 * dt ** 4 * dx ** 3 * dy ** 7 * dz ** 6 + 0.9600
     #E4 * cc ** 4 * dt ** 3 * dy ** 7 * dz ** 5 * dx ** 5 + 0.3840E4 * 
     #cc ** 3 * dt ** 2 * dy ** 7 * dz ** 5 * dx ** 6 + 0.3840E4 * cc **
     # 3 * dt ** 2 * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / 
     #dz ** 7 / 0.17280E5
      cvu(286) = (-0.72E2 * cc ** 7 * dt ** 6 * dy ** 7 * dz * dx ** 6 -
     # 0.72E2 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 3 * dx ** 6 - 0.216E
     #3 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 7 * dz ** 2 - 0.72E2 * cc 
     #** 7 * dt ** 6 * dx ** 5 * dz ** 3 * dy ** 6 - 0.144E3 * cc ** 7 *
     # dt ** 6 * dx ** 5 * dy ** 5 * dz ** 4 - 0.144E3 * cc ** 7 * dt **
     # 6 * dy ** 7 * dz ** 3 * dx ** 4 - 0.144E3 * cc ** 7 * dt ** 6 * d
     #x ** 3 * dy ** 7 * dz ** 4 + 0.432E3 * cc ** 6 * dt ** 5 * dy ** 7
     # * dz ** 3 * dx ** 5 + 0.600E3 * cc ** 5 * dt ** 4 * dy ** 7 * dz 
     #** 3 * dx ** 6 + 0.120E3 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 *
     # dx ** 6 + 0.1320E4 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 7 * dz *
     #* 4 + 0.120E3 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 + 
     #0.240E3 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6 + 0.240E
     #3 * cc ** 5 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 4 + 0.240E3 * cc
     # ** 5 * dt ** 4 * dx ** 3 * dy ** 7 * dz ** 6 - 0.480E3 * cc ** 4 
     #* dt ** 3 * dy ** 7 * dz ** 5 * dx ** 5 - 0.528E3 * cc ** 3 * dt *
     #* 2 * dy ** 7 * dz ** 5 * dx ** 6 - 0.1104E4 * cc ** 3 * dt ** 2 *
     # dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17
     #280E5
      cvu(287) = (0.12E2 * cc ** 7 * dt ** 6 * dy ** 7 * dz * dx ** 6 + 
     #0.36E2 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 7 * dz ** 2 - 0.60E2 
     #* cc ** 5 * dt ** 4 * dy ** 7 * dz ** 3 * dx ** 6 - 0.180E3 * cc *
     #* 5 * dt ** 4 * dx ** 5 * dy ** 7 * dz ** 4 + 0.48E2 * cc ** 3 * d
     #t ** 2 * dy ** 7 * dz ** 5 * dx ** 6 + 0.144E3 * cc ** 3 * dt ** 2
     # * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.
     #17280E5
      cvu(291) = (0.36E2 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 3 * dx **
     # 6 + 0.36E2 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 3 * dy ** 6 + 0.
     #72E2 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 4 - 0.60E2 * 
     #cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx ** 6 - 0.60E2 * cc ** 5
     # * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 - 0.120E3 * cc ** 5 * dt 
     #** 4 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 
     #/ 0.17280E5
      cvu(292) = (-0.144E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 3 * dx 
     #** 6 - 0.144E3 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 5 * dx ** 6 -
     # 0.144E3 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 3 * dy ** 6 - 0.288
     #E3 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 4 - 0.288E3 * c
     #c ** 7 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 4 - 0.144E3 * cc ** 7
     # * dt ** 6 * dx ** 5 * dy ** 3 * dz ** 6 - 0.288E3 * cc ** 7 * dt 
     #** 6 * dy ** 5 * dz ** 5 * dx ** 4 - 0.144E3 * cc ** 7 * dt ** 6 *
     # dx ** 3 * dz ** 5 * dy ** 6 - 0.144E3 * cc ** 7 * dt ** 6 * dx **
     # 3 * dy ** 5 * dz ** 6 + 0.864E3 * cc ** 6 * dt ** 5 * dy ** 5 * d
     #z ** 5 * dx ** 5 + 0.960E3 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5
     # * dx ** 6 + 0.960E3 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy 
     #** 6 + 0.960E3 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6) 
     #/ dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvu(293) = (0.216E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 3 * dx *
     #* 6 + 0.288E3 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 5 * dx ** 6 + 
     #0.180E3 * cc ** 7 * dt ** 6 * dy * dz ** 7 * dx ** 6 + 0.216E3 * c
     #c ** 7 * dt ** 6 * dx ** 5 * dz ** 3 * dy ** 6 + 0.432E3 * cc ** 7
     # * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 4 + 0.576E3 * cc ** 7 * dt 
     #** 6 * dx ** 5 * dz ** 5 * dy ** 4 + 0.288E3 * cc ** 7 * dt ** 6 *
     # dx ** 5 * dy ** 3 * dz ** 6 + 0.540E3 * cc ** 7 * dt ** 6 * dx **
     # 5 * dz ** 7 * dy ** 2 + 0.576E3 * cc ** 7 * dt ** 6 * dy ** 5 * d
     #z ** 5 * dx ** 4 + 0.576E3 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 7
     # * dx ** 4 + 0.288E3 * cc ** 7 * dt ** 6 * dx ** 3 * dz ** 5 * dy 
     #** 6 + 0.288E3 * cc ** 7 * dt ** 6 * dx ** 3 * dy ** 5 * dz ** 6 +
     # 0.576E3 * cc ** 7 * dt ** 6 * dx ** 3 * dz ** 7 * dy ** 4 + 0.540
     #E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 7 * dx ** 2 + 0.180E3 * c
     #c ** 7 * dt ** 6 * dx * dz ** 7 * dy ** 6 - 0.1728E4 * cc ** 6 * d
     #t ** 5 * dy ** 5 * dz ** 5 * dx ** 5 - 0.1728E4 * cc ** 6 * dt ** 
     #5 * dy ** 3 * dz ** 7 * dx ** 5 - 0.1728E4 * cc ** 6 * dt ** 5 * d
     #y ** 5 * dz ** 7 * dx ** 3 - 0.1800E4 * cc ** 5 * dt ** 4 * dy ** 
     #5 * dz ** 5 * dx ** 6 - 0.1860E4 * cc ** 5 * dt ** 4 * dy ** 3 * d
     #z ** 7 * dx ** 6 - 0.1800E4 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 
     #5 * dy ** 6 - 0.1680E4 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * d
     #z ** 6 - 0.3660E4 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 7 * dy ** 
     #4 - 0.3660E4 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 4 - 0
     #.1860E4 * cc ** 5 * dt ** 4 * dx ** 3 * dz ** 7 * dy ** 6 + 0.9600
     #E4 * cc ** 4 * dt ** 3 * dy ** 5 * dz ** 7 * dx ** 5 + 0.3840E4 * 
     #cc ** 3 * dt ** 2 * dy ** 5 * dz ** 7 * dx ** 6 + 0.3840E4 * cc **
     # 3 * dt ** 2 * dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / 
     #dz ** 7 / 0.17280E5
      cvu(294) = (-0.144E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 3 * dx 
     #** 6 - 0.144E3 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 5 * dx ** 6 -
     # 0.144E3 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 3 * dy ** 6 - 0.288
     #E3 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 4 - 0.288E3 * c
     #c ** 7 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 4 - 0.144E3 * cc ** 7
     # * dt ** 6 * dx ** 5 * dy ** 3 * dz ** 6 - 0.288E3 * cc ** 7 * dt 
     #** 6 * dy ** 5 * dz ** 5 * dx ** 4 - 0.144E3 * cc ** 7 * dt ** 6 *
     # dx ** 3 * dz ** 5 * dy ** 6 - 0.144E3 * cc ** 7 * dt ** 6 * dx **
     # 3 * dy ** 5 * dz ** 6 + 0.864E3 * cc ** 6 * dt ** 5 * dy ** 5 * d
     #z ** 5 * dx ** 5 + 0.960E3 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5
     # * dx ** 6 + 0.960E3 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy 
     #** 6 + 0.960E3 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6) 
     #/ dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvu(295) = (0.36E2 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 3 * dx **
     # 6 + 0.36E2 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 3 * dy ** 6 + 0.
     #72E2 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 4 - 0.60E2 * 
     #cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx ** 6 - 0.60E2 * cc ** 5
     # * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 - 0.120E3 * cc ** 5 * dt 
     #** 4 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 
     #/ 0.17280E5
      cvu(301) = (0.36E2 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 5 * dx **
     # 6 + 0.72E2 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 4 + 0.
     #36E2 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 3 * dz ** 6 - 0.60E2 * 
     #cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx ** 6 - 0.120E3 * cc ** 
     #5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 - 0.60E2 * cc ** 5 * dt 
     #** 4 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 
     #/ 0.17280E5
      cvu(302) = (-0.72E2 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 5 * dx *
     #* 6 - 0.72E2 * cc ** 7 * dt ** 6 * dy * dz ** 7 * dx ** 6 - 0.144E
     #3 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 4 - 0.72E2 * cc 
     #** 7 * dt ** 6 * dx ** 5 * dy ** 3 * dz ** 6 - 0.216E3 * cc ** 7 *
     # dt ** 6 * dx ** 5 * dz ** 7 * dy ** 2 - 0.144E3 * cc ** 7 * dt **
     # 6 * dy ** 3 * dz ** 7 * dx ** 4 - 0.144E3 * cc ** 7 * dt ** 6 * d
     #x ** 3 * dz ** 7 * dy ** 4 + 0.432E3 * cc ** 6 * dt ** 5 * dy ** 3
     # * dz ** 7 * dx ** 5 + 0.120E3 * cc ** 5 * dt ** 4 * dy ** 5 * dz 
     #** 5 * dx ** 6 + 0.600E3 * cc ** 5 * dt ** 4 * dy ** 3 * dz ** 7 *
     # dx ** 6 + 0.240E3 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy **
     # 6 + 0.120E3 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6 + 0
     #.1320E4 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 7 * dy ** 4 + 0.240E
     #3 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 4 + 0.240E3 * cc
     # ** 5 * dt ** 4 * dx ** 3 * dz ** 7 * dy ** 6 - 0.480E3 * cc ** 4 
     #* dt ** 3 * dy ** 5 * dz ** 7 * dx ** 5 - 0.528E3 * cc ** 3 * dt *
     #* 2 * dy ** 5 * dz ** 7 * dx ** 6 - 0.1104E4 * cc ** 3 * dt ** 2 *
     # dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17
     #280E5
      cvu(303) = (0.36E2 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 5 * dx **
     # 6 + 0.72E2 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 4 + 0.
     #36E2 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 3 * dz ** 6 - 0.60E2 * 
     #cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx ** 6 - 0.120E3 * cc ** 
     #5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 - 0.60E2 * cc ** 5 * dt 
     #** 4 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 
     #/ 0.17280E5
      cvu(311) = (0.12E2 * cc ** 7 * dt ** 6 * dy * dz ** 7 * dx ** 6 + 
     #0.36E2 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 7 * dy ** 2 - 0.60E2 
     #* cc ** 5 * dt ** 4 * dy ** 3 * dz ** 7 * dx ** 6 - 0.180E3 * cc *
     #* 5 * dt ** 4 * dx ** 5 * dz ** 7 * dy ** 4 + 0.48E2 * cc ** 3 * d
     #t ** 2 * dy ** 5 * dz ** 7 * dx ** 6 + 0.144E3 * cc ** 3 * dt ** 2
     # * dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.
     #17280E5
      cvu(329) = (0.12E2 * cc ** 7 * dt ** 6 * dx ** 7 * dz ** 7 - 0.120
     #E3 * cc ** 5 * dt ** 4 * dx ** 7 * dz ** 7 * dy ** 2 + 0.252E3 * c
     #c ** 3 * dt ** 2 * dx ** 7 * dz ** 7 * dy ** 4) / dx ** 7 / dy ** 
     #7 / dz ** 7 / 0.17280E5
      cvu(337) = (0.36E2 * cc ** 7 * dt ** 6 * dx ** 7 * dz ** 5 * dy **
     # 2 + 0.12E2 * cc ** 7 * dt ** 6 * dx ** 7 * dy * dz ** 6 - 0.180E3
     # * cc ** 5 * dt ** 4 * dx ** 7 * dz ** 5 * dy ** 4 - 0.60E2 * cc *
     #* 5 * dt ** 4 * dx ** 7 * dy ** 3 * dz ** 6 + 0.144E3 * cc ** 3 * 
     #dt ** 2 * dx ** 7 * dz ** 5 * dy ** 6 + 0.48E2 * cc ** 3 * dt ** 2
     # * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.
     #17280E5
      cvu(338) = (-0.72E2 * cc ** 7 * dt ** 6 * dx ** 7 * dz ** 5 * dy *
     #* 2 - 0.24E2 * cc ** 7 * dt ** 6 * dx ** 7 * dy * dz ** 6 - 0.96E2
     # * cc ** 7 * dt ** 6 * dx ** 7 * dz ** 7 - 0.24E2 * cc ** 7 * dt *
     #* 6 * dy * dz ** 7 * dx ** 6 - 0.72E2 * cc ** 7 * dt ** 6 * dx ** 
     #5 * dz ** 7 * dy ** 2 + 0.144E3 * cc ** 6 * dt ** 5 * dx ** 7 * dz
     # ** 7 * dy + 0.360E3 * cc ** 5 * dt ** 4 * dx ** 7 * dz ** 5 * dy 
     #** 4 + 0.120E3 * cc ** 5 * dt ** 4 * dx ** 7 * dy ** 3 * dz ** 6 +
     # 0.960E3 * cc ** 5 * dt ** 4 * dx ** 7 * dz ** 7 * dy ** 2 + 0.120
     #E3 * cc ** 5 * dt ** 4 * dy ** 3 * dz ** 7 * dx ** 6 + 0.360E3 * c
     #c ** 5 * dt ** 4 * dx ** 5 * dz ** 7 * dy ** 4 - 0.480E3 * cc ** 4
     # * dt ** 3 * dx ** 7 * dz ** 7 * dy ** 3 - 0.288E3 * cc ** 3 * dt 
     #** 2 * dx ** 7 * dz ** 5 * dy ** 6 - 0.96E2 * cc ** 3 * dt ** 2 * 
     #dx ** 7 * dy ** 5 * dz ** 6 - 0.2016E4 * cc ** 3 * dt ** 2 * dx **
     # 7 * dz ** 7 * dy ** 4 - 0.96E2 * cc ** 3 * dt ** 2 * dy ** 5 * dz
     # ** 7 * dx ** 6 - 0.288E3 * cc ** 3 * dt ** 2 * dx ** 5 * dz ** 7 
     #* dy ** 6 + 0.192E3 * cc ** 2 * dt * dx ** 7 * dz ** 7 * dy ** 5) 
     #/ dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvu(339) = (0.36E2 * cc ** 7 * dt ** 6 * dx ** 7 * dz ** 5 * dy **
     # 2 + 0.12E2 * cc ** 7 * dt ** 6 * dx ** 7 * dy * dz ** 6 - 0.180E3
     # * cc ** 5 * dt ** 4 * dx ** 7 * dz ** 5 * dy ** 4 - 0.60E2 * cc *
     #* 5 * dt ** 4 * dx ** 7 * dy ** 3 * dz ** 6 + 0.144E3 * cc ** 3 * 
     #dt ** 2 * dx ** 7 * dz ** 5 * dy ** 6 + 0.48E2 * cc ** 3 * dt ** 2
     # * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.
     #17280E5
      cvu(345) = (0.36E2 * cc ** 7 * dt ** 6 * dx ** 7 * dz ** 3 * dy **
     # 4 + 0.36E2 * cc ** 7 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 4 - 0.
     #60E2 * cc ** 5 * dt ** 4 * dx ** 7 * dz ** 3 * dy ** 6 - 0.60E2 * 
     #cc ** 5 * dt ** 4 * dx ** 7 * dy ** 5 * dz ** 4 - 0.60E2 * cc ** 5
     # * dt ** 4 * dx ** 7 * dz ** 5 * dy ** 4 - 0.60E2 * cc ** 5 * dt *
     #* 4 * dx ** 7 * dy ** 3 * dz ** 6 + 0.60E2 * cc ** 3 * dt ** 2 * d
     #x ** 7 * dz ** 5 * dy ** 6 + 0.60E2 * cc ** 3 * dt ** 2 * dx ** 7 
     #* dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvu(346) = (-0.144E3 * cc ** 7 * dt ** 6 * dx ** 7 * dz ** 3 * dy 
     #** 4 - 0.144E3 * cc ** 7 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 4 -
     # 0.216E3 * cc ** 7 * dt ** 6 * dx ** 7 * dz ** 5 * dy ** 2 - 0.72E
     #2 * cc ** 7 * dt ** 6 * dx ** 7 * dy * dz ** 6 - 0.72E2 * cc ** 7 
     #* dt ** 6 * dy ** 3 * dz ** 5 * dx ** 6 - 0.144E3 * cc ** 7 * dt *
     #* 6 * dx ** 5 * dz ** 5 * dy ** 4 - 0.72E2 * cc ** 7 * dt ** 6 * d
     #x ** 5 * dy ** 3 * dz ** 6 + 0.432E3 * cc ** 6 * dt ** 5 * dx ** 7
     # * dz ** 5 * dy ** 3 + 0.240E3 * cc ** 5 * dt ** 4 * dx ** 7 * dz 
     #** 3 * dy ** 6 + 0.240E3 * cc ** 5 * dt ** 4 * dx ** 7 * dy ** 5 *
     # dz ** 4 + 0.1320E4 * cc ** 5 * dt ** 4 * dx ** 7 * dz ** 5 * dy *
     #* 4 + 0.600E3 * cc ** 5 * dt ** 4 * dx ** 7 * dy ** 3 * dz ** 6 + 
     #0.120E3 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx ** 6 + 0.240E
     #3 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 + 0.120E3 * cc
     # ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6 - 0.480E3 * cc ** 4 
     #* dt ** 3 * dx ** 7 * dz ** 5 * dy ** 5 - 0.1104E4 * cc ** 3 * dt 
     #** 2 * dx ** 7 * dz ** 5 * dy ** 6 - 0.528E3 * cc ** 3 * dt ** 2 *
     # dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17
     #280E5
      cvu(347) = (0.216E3 * cc ** 7 * dt ** 6 * dx ** 7 * dz ** 3 * dy *
     #* 4 + 0.216E3 * cc ** 7 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 4 + 
     #0.432E3 * cc ** 7 * dt ** 6 * dx ** 7 * dz ** 5 * dy ** 2 + 0.144E
     #3 * cc ** 7 * dt ** 6 * dx ** 7 * dy * dz ** 6 + 0.336E3 * cc ** 7
     # * dt ** 6 * dx ** 7 * dz ** 7 + 0.144E3 * cc ** 7 * dt ** 6 * dy 
     #** 3 * dz ** 5 * dx ** 6 + 0.144E3 * cc ** 7 * dt ** 6 * dy * dz *
     #* 7 * dx ** 6 + 0.288E3 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 5 * 
     #dy ** 4 + 0.144E3 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 3 * dz ** 
     #6 + 0.432E3 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 7 * dy ** 2 + 0.
     #216E3 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 7 * dx ** 4 + 0.216E3 
     #* cc ** 7 * dt ** 6 * dx ** 3 * dz ** 7 * dy ** 4 - 0.864E3 * cc *
     #* 6 * dt ** 5 * dx ** 7 * dz ** 5 * dy ** 3 - 0.864E3 * cc ** 6 * 
     #dt ** 5 * dx ** 7 * dz ** 7 * dy - 0.864E3 * cc ** 6 * dt ** 5 * d
     #y ** 3 * dz ** 7 * dx ** 5 - 0.360E3 * cc ** 5 * dt ** 4 * dx ** 7
     # * dz ** 3 * dy ** 6 - 0.360E3 * cc ** 5 * dt ** 4 * dx ** 7 * dy 
     #** 5 * dz ** 4 - 0.2520E4 * cc ** 5 * dt ** 4 * dx ** 7 * dz ** 5 
     #* dy ** 4 - 0.1080E4 * cc ** 5 * dt ** 4 * dx ** 7 * dy ** 3 * dz 
     #** 6 - 0.3360E4 * cc ** 5 * dt ** 4 * dx ** 7 * dz ** 7 * dy ** 2 
     #- 0.240E3 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx ** 6 - 0.10
     #80E4 * cc ** 5 * dt ** 4 * dy ** 3 * dz ** 7 * dx ** 6 - 0.480E3 *
     # cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 - 0.240E3 * cc **
     # 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6 - 0.2520E4 * cc ** 5 * 
     #dt ** 4 * dx ** 5 * dz ** 7 * dy ** 4 - 0.360E3 * cc ** 5 * dt ** 
     #4 * dy ** 5 * dz ** 7 * dx ** 4 - 0.360E3 * cc ** 5 * dt ** 4 * dx
     # ** 3 * dz ** 7 * dy ** 6 + 0.960E3 * cc ** 4 * dt ** 3 * dx ** 7 
     #* dz ** 5 * dy ** 5 + 0.5760E4 * cc ** 4 * dt ** 3 * dx ** 7 * dz 
     #** 7 * dy ** 3 + 0.960E3 * cc ** 4 * dt ** 3 * dy ** 5 * dz ** 7 *
     # dx ** 5 + 0.2088E4 * cc ** 3 * dt ** 2 * dx ** 7 * dz ** 5 * dy *
     #* 6 + 0.936E3 * cc ** 3 * dt ** 2 * dx ** 7 * dy ** 5 * dz ** 6 + 
     #0.7056E4 * cc ** 3 * dt ** 2 * dx ** 7 * dz ** 7 * dy ** 4 + 0.936
     #E3 * cc ** 3 * dt ** 2 * dy ** 5 * dz ** 7 * dx ** 6 + 0.2088E4 * 
     #cc ** 3 * dt ** 2 * dx ** 5 * dz ** 7 * dy ** 6 - 0.2592E4 * cc **
     # 2 * dt * dx ** 7 * dz ** 7 * dy ** 5) / dx ** 7 / dy ** 7 / dz **
     # 7 / 0.17280E5
      cvu(348) = (-0.144E3 * cc ** 7 * dt ** 6 * dx ** 7 * dz ** 3 * dy 
     #** 4 - 0.144E3 * cc ** 7 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 4 -
     # 0.216E3 * cc ** 7 * dt ** 6 * dx ** 7 * dz ** 5 * dy ** 2 - 0.72E
     #2 * cc ** 7 * dt ** 6 * dx ** 7 * dy * dz ** 6 - 0.72E2 * cc ** 7 
     #* dt ** 6 * dy ** 3 * dz ** 5 * dx ** 6 - 0.144E3 * cc ** 7 * dt *
     #* 6 * dx ** 5 * dz ** 5 * dy ** 4 - 0.72E2 * cc ** 7 * dt ** 6 * d
     #x ** 5 * dy ** 3 * dz ** 6 + 0.432E3 * cc ** 6 * dt ** 5 * dx ** 7
     # * dz ** 5 * dy ** 3 + 0.240E3 * cc ** 5 * dt ** 4 * dx ** 7 * dz 
     #** 3 * dy ** 6 + 0.240E3 * cc ** 5 * dt ** 4 * dx ** 7 * dy ** 5 *
     # dz ** 4 + 0.1320E4 * cc ** 5 * dt ** 4 * dx ** 7 * dz ** 5 * dy *
     #* 4 + 0.600E3 * cc ** 5 * dt ** 4 * dx ** 7 * dy ** 3 * dz ** 6 + 
     #0.120E3 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx ** 6 + 0.240E
     #3 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 + 0.120E3 * cc
     # ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6 - 0.480E3 * cc ** 4 
     #* dt ** 3 * dx ** 7 * dz ** 5 * dy ** 5 - 0.1104E4 * cc ** 3 * dt 
     #** 2 * dx ** 7 * dz ** 5 * dy ** 6 - 0.528E3 * cc ** 3 * dt ** 2 *
     # dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17
     #280E5
      cvu(349) = (0.36E2 * cc ** 7 * dt ** 6 * dx ** 7 * dz ** 3 * dy **
     # 4 + 0.36E2 * cc ** 7 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 4 - 0.
     #60E2 * cc ** 5 * dt ** 4 * dx ** 7 * dz ** 3 * dy ** 6 - 0.60E2 * 
     #cc ** 5 * dt ** 4 * dx ** 7 * dy ** 5 * dz ** 4 - 0.60E2 * cc ** 5
     # * dt ** 4 * dx ** 7 * dz ** 5 * dy ** 4 - 0.60E2 * cc ** 5 * dt *
     #* 4 * dx ** 7 * dy ** 3 * dz ** 6 + 0.60E2 * cc ** 3 * dt ** 2 * d
     #x ** 7 * dz ** 5 * dy ** 6 + 0.60E2 * cc ** 3 * dt ** 2 * dx ** 7 
     #* dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvu(353) = (0.12E2 * cc ** 7 * dt ** 6 * dx ** 7 * dz * dy ** 6 + 
     #0.36E2 * cc ** 7 * dt ** 6 * dx ** 7 * dy ** 5 * dz ** 2 - 0.60E2 
     #* cc ** 5 * dt ** 4 * dx ** 7 * dz ** 3 * dy ** 6 - 0.180E3 * cc *
     #* 5 * dt ** 4 * dx ** 7 * dy ** 5 * dz ** 4 + 0.48E2 * cc ** 3 * d
     #t ** 2 * dx ** 7 * dz ** 5 * dy ** 6 + 0.144E3 * cc ** 3 * dt ** 2
     # * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.
     #17280E5
      cvu(354) = (-0.72E2 * cc ** 7 * dt ** 6 * dx ** 7 * dz * dy ** 6 -
     # 0.216E3 * cc ** 7 * dt ** 6 * dx ** 7 * dy ** 5 * dz ** 2 - 0.144
     #E3 * cc ** 7 * dt ** 6 * dx ** 7 * dz ** 3 * dy ** 4 - 0.144E3 * c
     #c ** 7 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 4 - 0.72E2 * cc ** 7 
     #* dt ** 6 * dy ** 5 * dz ** 3 * dx ** 6 - 0.72E2 * cc ** 7 * dt **
     # 6 * dx ** 5 * dz ** 3 * dy ** 6 - 0.144E3 * cc ** 7 * dt ** 6 * d
     #x ** 5 * dy ** 5 * dz ** 4 + 0.432E3 * cc ** 6 * dt ** 5 * dx ** 7
     # * dz ** 3 * dy ** 5 + 0.600E3 * cc ** 5 * dt ** 4 * dx ** 7 * dz 
     #** 3 * dy ** 6 + 0.1320E4 * cc ** 5 * dt ** 4 * dx ** 7 * dy ** 5 
     #* dz ** 4 + 0.240E3 * cc ** 5 * dt ** 4 * dx ** 7 * dz ** 5 * dy *
     #* 4 + 0.240E3 * cc ** 5 * dt ** 4 * dx ** 7 * dy ** 3 * dz ** 6 + 
     #0.120E3 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx ** 6 + 0.120E
     #3 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 + 0.240E3 * cc
     # ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6 - 0.480E3 * cc ** 4 
     #* dt ** 3 * dx ** 7 * dz ** 5 * dy ** 5 - 0.528E3 * cc ** 3 * dt *
     #* 2 * dx ** 7 * dz ** 5 * dy ** 6 - 0.1104E4 * cc ** 3 * dt ** 2 *
     # dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17
     #280E5
      cvu(355) = (0.180E3 * cc ** 7 * dt ** 6 * dx ** 7 * dz * dy ** 6 +
     # 0.540E3 * cc ** 7 * dt ** 6 * dx ** 7 * dy ** 5 * dz ** 2 + 0.576
     #E3 * cc ** 7 * dt ** 6 * dx ** 7 * dz ** 3 * dy ** 4 + 0.576E3 * c
     #c ** 7 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 4 + 0.540E3 * cc ** 7
     # * dt ** 6 * dx ** 7 * dz ** 5 * dy ** 2 + 0.180E3 * cc ** 7 * dt 
     #** 6 * dx ** 7 * dy * dz ** 6 + 0.288E3 * cc ** 7 * dt ** 6 * dy *
     #* 5 * dz ** 3 * dx ** 6 + 0.288E3 * cc ** 7 * dt ** 6 * dy ** 3 * 
     #dz ** 5 * dx ** 6 + 0.288E3 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 
     #3 * dy ** 6 + 0.576E3 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 5 * dz
     # ** 4 + 0.576E3 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 4 
     #+ 0.288E3 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 3 * dz ** 6 + 0.43
     #2E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 4 + 0.216E3 * 
     #cc ** 7 * dt ** 6 * dx ** 3 * dz ** 5 * dy ** 6 + 0.216E3 * cc ** 
     #7 * dt ** 6 * dx ** 3 * dy ** 5 * dz ** 6 - 0.1728E4 * cc ** 6 * d
     #t ** 5 * dx ** 7 * dz ** 3 * dy ** 5 - 0.1728E4 * cc ** 6 * dt ** 
     #5 * dx ** 7 * dz ** 5 * dy ** 3 - 0.1728E4 * cc ** 6 * dt ** 5 * d
     #y ** 5 * dz ** 5 * dx ** 5 - 0.1860E4 * cc ** 5 * dt ** 4 * dx ** 
     #7 * dz ** 3 * dy ** 6 - 0.3660E4 * cc ** 5 * dt ** 4 * dx ** 7 * d
     #y ** 5 * dz ** 4 - 0.3660E4 * cc ** 5 * dt ** 4 * dx ** 7 * dz ** 
     #5 * dy ** 4 - 0.1860E4 * cc ** 5 * dt ** 4 * dx ** 7 * dy ** 3 * d
     #z ** 6 - 0.1680E4 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx ** 
     #6 - 0.1800E4 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 - 0
     #.1800E4 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6 + 0.9600
     #E4 * cc ** 4 * dt ** 3 * dx ** 7 * dz ** 5 * dy ** 5 + 0.3840E4 * 
     #cc ** 3 * dt ** 2 * dx ** 7 * dz ** 5 * dy ** 6 + 0.3840E4 * cc **
     # 3 * dt ** 2 * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / 
     #dz ** 7 / 0.17280E5
      cvu(356) = (-0.240E3 * cc ** 7 * dt ** 6 * dx ** 7 * dz * dy ** 6 
     #- 0.720E3 * cc ** 7 * dt ** 6 * dx ** 7 * dy ** 5 * dz ** 2 - 0.86
     #4E3 * cc ** 7 * dt ** 6 * dx ** 7 * dz ** 3 * dy ** 4 - 0.864E3 * 
     #cc ** 7 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 4 - 0.1080E4 * cc **
     # 7 * dt ** 6 * dx ** 7 * dz ** 5 * dy ** 2 - 0.360E3 * cc ** 7 * d
     #t ** 6 * dx ** 7 * dy * dz ** 6 - 0.672E3 * cc ** 7 * dt ** 6 * dx
     # ** 7 * dz ** 7 - 0.432E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 3 
     #* dx ** 6 - 0.576E3 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 5 * dx *
     #* 6 - 0.360E3 * cc ** 7 * dt ** 6 * dy * dz ** 7 * dx ** 6 - 0.432
     #E3 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 3 * dy ** 6 - 0.864E3 * c
     #c ** 7 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 4 - 0.1152E4 * cc ** 
     #7 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 4 - 0.576E3 * cc ** 7 * dt
     # ** 6 * dx ** 5 * dy ** 3 * dz ** 6 - 0.1080E4 * cc ** 7 * dt ** 6
     # * dx ** 5 * dz ** 7 * dy ** 2 - 0.864E3 * cc ** 7 * dt ** 6 * dy 
     #** 5 * dz ** 5 * dx ** 4 - 0.864E3 * cc ** 7 * dt ** 6 * dy ** 3 *
     # dz ** 7 * dx ** 4 - 0.432E3 * cc ** 7 * dt ** 6 * dx ** 3 * dz **
     # 5 * dy ** 6 - 0.432E3 * cc ** 7 * dt ** 6 * dx ** 3 * dy ** 5 * d
     #z ** 6 - 0.864E3 * cc ** 7 * dt ** 6 * dx ** 3 * dz ** 7 * dy ** 4
     # - 0.720E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 7 * dx ** 2 - 0.2
     #40E3 * cc ** 7 * dt ** 6 * dx * dz ** 7 * dy ** 6 + 0.2592E4 * cc 
     #** 6 * dt ** 5 * dx ** 7 * dz ** 3 * dy ** 5 + 0.3456E4 * cc ** 6 
     #* dt ** 5 * dx ** 7 * dz ** 5 * dy ** 3 + 0.2160E4 * cc ** 6 * dt 
     #** 5 * dx ** 7 * dz ** 7 * dy + 0.3456E4 * cc ** 6 * dt ** 5 * dy 
     #** 5 * dz ** 5 * dx ** 5 + 0.3456E4 * cc ** 6 * dt ** 5 * dy ** 3 
     #* dz ** 7 * dx ** 5 + 0.2592E4 * cc ** 6 * dt ** 5 * dy ** 5 * dz 
     #** 7 * dx ** 3 + 0.2640E4 * cc ** 5 * dt ** 4 * dx ** 7 * dz ** 3 
     #* dy ** 6 + 0.5040E4 * cc ** 5 * dt ** 4 * dx ** 7 * dy ** 5 * dz 
     #** 4 + 0.6840E4 * cc ** 5 * dt ** 4 * dx ** 7 * dz ** 5 * dy ** 4 
     #+ 0.3240E4 * cc ** 5 * dt ** 4 * dx ** 7 * dy ** 3 * dz ** 6 + 0.6
     #720E4 * cc ** 5 * dt ** 4 * dx ** 7 * dz ** 7 * dy ** 2 + 0.3120E4
     # * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx ** 6 + 0.3240E4 * cc
     # ** 5 * dt ** 4 * dy ** 3 * dz ** 7 * dx ** 6 + 0.3360E4 * cc ** 5
     # * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 + 0.3120E4 * cc ** 5 * dt
     # ** 4 * dx ** 5 * dy ** 5 * dz ** 6 + 0.6840E4 * cc ** 5 * dt ** 4
     # * dx ** 5 * dz ** 7 * dy ** 4 + 0.5040E4 * cc ** 5 * dt ** 4 * dy
     # ** 5 * dz ** 7 * dx ** 4 + 0.2640E4 * cc ** 5 * dt ** 4 * dx ** 3
     # * dz ** 7 * dy ** 6 - 0.18240E5 * cc ** 4 * dt ** 3 * dx ** 7 * d
     #z ** 5 * dy ** 5 - 0.18720E5 * cc ** 4 * dt ** 3 * dx ** 7 * dz **
     # 7 * dy ** 3 - 0.18240E5 * cc ** 4 * dt ** 3 * dy ** 5 * dz ** 7 *
     # dx ** 5 - 0.6720E4 * cc ** 3 * dt ** 2 * dx ** 7 * dz ** 5 * dy *
     #* 6 - 0.5760E4 * cc ** 3 * dt ** 2 * dx ** 7 * dy ** 5 * dz ** 6 -
     # 0.14112E5 * cc ** 3 * dt ** 2 * dx ** 7 * dz ** 7 * dy ** 4 - 0.5
     #760E4 * cc ** 3 * dt ** 2 * dy ** 5 * dz ** 7 * dx ** 6 - 0.6720E4
     # * cc ** 3 * dt ** 2 * dx ** 5 * dz ** 7 * dy ** 6 + 0.25920E5 * c
     #c ** 2 * dt * dx ** 7 * dz ** 7 * dy ** 5) / dx ** 7 / dy ** 7 / d
     #z ** 7 / 0.17280E5
      cvu(357) = (0.180E3 * cc ** 7 * dt ** 6 * dx ** 7 * dz * dy ** 6 +
     # 0.540E3 * cc ** 7 * dt ** 6 * dx ** 7 * dy ** 5 * dz ** 2 + 0.576
     #E3 * cc ** 7 * dt ** 6 * dx ** 7 * dz ** 3 * dy ** 4 + 0.576E3 * c
     #c ** 7 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 4 + 0.540E3 * cc ** 7
     # * dt ** 6 * dx ** 7 * dz ** 5 * dy ** 2 + 0.180E3 * cc ** 7 * dt 
     #** 6 * dx ** 7 * dy * dz ** 6 + 0.288E3 * cc ** 7 * dt ** 6 * dy *
     #* 5 * dz ** 3 * dx ** 6 + 0.288E3 * cc ** 7 * dt ** 6 * dy ** 3 * 
     #dz ** 5 * dx ** 6 + 0.288E3 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 
     #3 * dy ** 6 + 0.576E3 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 5 * dz
     # ** 4 + 0.576E3 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 4 
     #+ 0.288E3 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 3 * dz ** 6 + 0.43
     #2E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 4 + 0.216E3 * 
     #cc ** 7 * dt ** 6 * dx ** 3 * dz ** 5 * dy ** 6 + 0.216E3 * cc ** 
     #7 * dt ** 6 * dx ** 3 * dy ** 5 * dz ** 6 - 0.1728E4 * cc ** 6 * d
     #t ** 5 * dx ** 7 * dz ** 3 * dy ** 5 - 0.1728E4 * cc ** 6 * dt ** 
     #5 * dx ** 7 * dz ** 5 * dy ** 3 - 0.1728E4 * cc ** 6 * dt ** 5 * d
     #y ** 5 * dz ** 5 * dx ** 5 - 0.1860E4 * cc ** 5 * dt ** 4 * dx ** 
     #7 * dz ** 3 * dy ** 6 - 0.3660E4 * cc ** 5 * dt ** 4 * dx ** 7 * d
     #y ** 5 * dz ** 4 - 0.3660E4 * cc ** 5 * dt ** 4 * dx ** 7 * dz ** 
     #5 * dy ** 4 - 0.1860E4 * cc ** 5 * dt ** 4 * dx ** 7 * dy ** 3 * d
     #z ** 6 - 0.1680E4 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx ** 
     #6 - 0.1800E4 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 - 0
     #.1800E4 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6 + 0.9600
     #E4 * cc ** 4 * dt ** 3 * dx ** 7 * dz ** 5 * dy ** 5 + 0.3840E4 * 
     #cc ** 3 * dt ** 2 * dx ** 7 * dz ** 5 * dy ** 6 + 0.3840E4 * cc **
     # 3 * dt ** 2 * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / 
     #dz ** 7 / 0.17280E5
      cvu(358) = (-0.72E2 * cc ** 7 * dt ** 6 * dx ** 7 * dz * dy ** 6 -
     # 0.216E3 * cc ** 7 * dt ** 6 * dx ** 7 * dy ** 5 * dz ** 2 - 0.144
     #E3 * cc ** 7 * dt ** 6 * dx ** 7 * dz ** 3 * dy ** 4 - 0.144E3 * c
     #c ** 7 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 4 - 0.72E2 * cc ** 7 
     #* dt ** 6 * dy ** 5 * dz ** 3 * dx ** 6 - 0.72E2 * cc ** 7 * dt **
     # 6 * dx ** 5 * dz ** 3 * dy ** 6 - 0.144E3 * cc ** 7 * dt ** 6 * d
     #x ** 5 * dy ** 5 * dz ** 4 + 0.432E3 * cc ** 6 * dt ** 5 * dx ** 7
     # * dz ** 3 * dy ** 5 + 0.600E3 * cc ** 5 * dt ** 4 * dx ** 7 * dz 
     #** 3 * dy ** 6 + 0.1320E4 * cc ** 5 * dt ** 4 * dx ** 7 * dy ** 5 
     #* dz ** 4 + 0.240E3 * cc ** 5 * dt ** 4 * dx ** 7 * dz ** 5 * dy *
     #* 4 + 0.240E3 * cc ** 5 * dt ** 4 * dx ** 7 * dy ** 3 * dz ** 6 + 
     #0.120E3 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx ** 6 + 0.120E
     #3 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 + 0.240E3 * cc
     # ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6 - 0.480E3 * cc ** 4 
     #* dt ** 3 * dx ** 7 * dz ** 5 * dy ** 5 - 0.528E3 * cc ** 3 * dt *
     #* 2 * dx ** 7 * dz ** 5 * dy ** 6 - 0.1104E4 * cc ** 3 * dt ** 2 *
     # dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17
     #280E5
      cvu(359) = (0.12E2 * cc ** 7 * dt ** 6 * dx ** 7 * dz * dy ** 6 + 
     #0.36E2 * cc ** 7 * dt ** 6 * dx ** 7 * dy ** 5 * dz ** 2 - 0.60E2 
     #* cc ** 5 * dt ** 4 * dx ** 7 * dz ** 3 * dy ** 6 - 0.180E3 * cc *
     #* 5 * dt ** 4 * dx ** 7 * dy ** 5 * dz ** 4 + 0.48E2 * cc ** 3 * d
     #t ** 2 * dx ** 7 * dz ** 5 * dy ** 6 + 0.144E3 * cc ** 3 * dt ** 2
     # * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.
     #17280E5
      cvu(361) = (0.12E2 * cc ** 7 * dt ** 6 * dx ** 7 * dy ** 7 - 0.120
     #E3 * cc ** 5 * dt ** 4 * dx ** 7 * dy ** 7 * dz ** 2 + 0.252E3 * c
     #c ** 3 * dt ** 2 * dx ** 7 * dy ** 7 * dz ** 4) / dx ** 7 / dy ** 
     #7 / dz ** 7 / 0.17280E5
      cvu(362) = (-0.96E2 * cc ** 7 * dt ** 6 * dx ** 7 * dy ** 7 - 0.24
     #E2 * cc ** 7 * dt ** 6 * dx ** 7 * dz * dy ** 6 - 0.72E2 * cc ** 7
     # * dt ** 6 * dx ** 7 * dy ** 5 * dz ** 2 - 0.24E2 * cc ** 7 * dt *
     #* 6 * dy ** 7 * dz * dx ** 6 - 0.72E2 * cc ** 7 * dt ** 6 * dx ** 
     #5 * dy ** 7 * dz ** 2 + 0.144E3 * cc ** 6 * dt ** 5 * dx ** 7 * dy
     # ** 7 * dz + 0.960E3 * cc ** 5 * dt ** 4 * dx ** 7 * dy ** 7 * dz 
     #** 2 + 0.120E3 * cc ** 5 * dt ** 4 * dx ** 7 * dz ** 3 * dy ** 6 +
     # 0.360E3 * cc ** 5 * dt ** 4 * dx ** 7 * dy ** 5 * dz ** 4 + 0.120
     #E3 * cc ** 5 * dt ** 4 * dy ** 7 * dz ** 3 * dx ** 6 + 0.360E3 * c
     #c ** 5 * dt ** 4 * dx ** 5 * dy ** 7 * dz ** 4 - 0.480E3 * cc ** 4
     # * dt ** 3 * dx ** 7 * dy ** 7 * dz ** 3 - 0.2016E4 * cc ** 3 * dt
     # ** 2 * dx ** 7 * dy ** 7 * dz ** 4 - 0.96E2 * cc ** 3 * dt ** 2 *
     # dx ** 7 * dz ** 5 * dy ** 6 - 0.288E3 * cc ** 3 * dt ** 2 * dx **
     # 7 * dy ** 5 * dz ** 6 - 0.96E2 * cc ** 3 * dt ** 2 * dy ** 7 * dz
     # ** 5 * dx ** 6 - 0.288E3 * cc ** 3 * dt ** 2 * dx ** 5 * dy ** 7 
     #* dz ** 6 + 0.192E3 * cc ** 2 * dt * dx ** 7 * dy ** 7 * dz ** 5) 
     #/ dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvu(363) = (0.336E3 * cc ** 7 * dt ** 6 * dx ** 7 * dy ** 7 + 0.14
     #4E3 * cc ** 7 * dt ** 6 * dx ** 7 * dz * dy ** 6 + 0.432E3 * cc **
     # 7 * dt ** 6 * dx ** 7 * dy ** 5 * dz ** 2 + 0.216E3 * cc ** 7 * d
     #t ** 6 * dx ** 7 * dz ** 3 * dy ** 4 + 0.216E3 * cc ** 7 * dt ** 6
     # * dx ** 7 * dy ** 3 * dz ** 4 + 0.144E3 * cc ** 7 * dt ** 6 * dy 
     #** 7 * dz * dx ** 6 + 0.144E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz *
     #* 3 * dx ** 6 + 0.432E3 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 7 * 
     #dz ** 2 + 0.144E3 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 3 * dy ** 
     #6 + 0.288E3 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 4 + 0.
     #216E3 * cc ** 7 * dt ** 6 * dy ** 7 * dz ** 3 * dx ** 4 + 0.216E3 
     #* cc ** 7 * dt ** 6 * dx ** 3 * dy ** 7 * dz ** 4 - 0.864E3 * cc *
     #* 6 * dt ** 5 * dx ** 7 * dy ** 7 * dz - 0.864E3 * cc ** 6 * dt **
     # 5 * dx ** 7 * dz ** 3 * dy ** 5 - 0.864E3 * cc ** 6 * dt ** 5 * d
     #y ** 7 * dz ** 3 * dx ** 5 - 0.3360E4 * cc ** 5 * dt ** 4 * dx ** 
     #7 * dy ** 7 * dz ** 2 - 0.1080E4 * cc ** 5 * dt ** 4 * dx ** 7 * d
     #z ** 3 * dy ** 6 - 0.2520E4 * cc ** 5 * dt ** 4 * dx ** 7 * dy ** 
     #5 * dz ** 4 - 0.360E3 * cc ** 5 * dt ** 4 * dx ** 7 * dz ** 5 * dy
     # ** 4 - 0.360E3 * cc ** 5 * dt ** 4 * dx ** 7 * dy ** 3 * dz ** 6 
     #- 0.1080E4 * cc ** 5 * dt ** 4 * dy ** 7 * dz ** 3 * dx ** 6 - 0.2
     #40E3 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx ** 6 - 0.2520E4 
     #* cc ** 5 * dt ** 4 * dx ** 5 * dy ** 7 * dz ** 4 - 0.240E3 * cc *
     #* 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 - 0.480E3 * cc ** 5 * 
     #dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6 - 0.360E3 * cc ** 5 * dt ** 
     #4 * dy ** 7 * dz ** 5 * dx ** 4 - 0.360E3 * cc ** 5 * dt ** 4 * dx
     # ** 3 * dy ** 7 * dz ** 6 + 0.5760E4 * cc ** 4 * dt ** 3 * dx ** 7
     # * dy ** 7 * dz ** 3 + 0.960E3 * cc ** 4 * dt ** 3 * dx ** 7 * dz 
     #** 5 * dy ** 5 + 0.960E3 * cc ** 4 * dt ** 3 * dy ** 7 * dz ** 5 *
     # dx ** 5 + 0.7056E4 * cc ** 3 * dt ** 2 * dx ** 7 * dy ** 7 * dz *
     #* 4 + 0.936E3 * cc ** 3 * dt ** 2 * dx ** 7 * dz ** 5 * dy ** 6 + 
     #0.2088E4 * cc ** 3 * dt ** 2 * dx ** 7 * dy ** 5 * dz ** 6 + 0.936
     #E3 * cc ** 3 * dt ** 2 * dy ** 7 * dz ** 5 * dx ** 6 + 0.2088E4 * 
     #cc ** 3 * dt ** 2 * dx ** 5 * dy ** 7 * dz ** 6 - 0.2592E4 * cc **
     # 2 * dt * dx ** 7 * dy ** 7 * dz ** 5) / dx ** 7 / dy ** 7 / dz **
     # 7 / 0.17280E5
      cvu(364) = (-0.672E3 * cc ** 7 * dt ** 6 * dx ** 7 * dy ** 7 - 0.3
     #60E3 * cc ** 7 * dt ** 6 * dx ** 7 * dz * dy ** 6 - 0.1080E4 * cc 
     #** 7 * dt ** 6 * dx ** 7 * dy ** 5 * dz ** 2 - 0.864E3 * cc ** 7 *
     # dt ** 6 * dx ** 7 * dz ** 3 * dy ** 4 - 0.864E3 * cc ** 7 * dt **
     # 6 * dx ** 7 * dy ** 3 * dz ** 4 - 0.720E3 * cc ** 7 * dt ** 6 * d
     #x ** 7 * dz ** 5 * dy ** 2 - 0.240E3 * cc ** 7 * dt ** 6 * dx ** 7
     # * dy * dz ** 6 - 0.360E3 * cc ** 7 * dt ** 6 * dy ** 7 * dz * dx 
     #** 6 - 0.576E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 3 * dx ** 6 -
     # 0.432E3 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 5 * dx ** 6 - 0.108
     #0E4 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 7 * dz ** 2 - 0.576E3 * 
     #cc ** 7 * dt ** 6 * dx ** 5 * dz ** 3 * dy ** 6 - 0.1152E4 * cc **
     # 7 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 4 - 0.864E3 * cc ** 7 * d
     #t ** 6 * dx ** 5 * dz ** 5 * dy ** 4 - 0.432E3 * cc ** 7 * dt ** 6
     # * dx ** 5 * dy ** 3 * dz ** 6 - 0.864E3 * cc ** 7 * dt ** 6 * dy 
     #** 7 * dz ** 3 * dx ** 4 - 0.864E3 * cc ** 7 * dt ** 6 * dy ** 5 *
     # dz ** 5 * dx ** 4 - 0.864E3 * cc ** 7 * dt ** 6 * dx ** 3 * dy **
     # 7 * dz ** 4 - 0.432E3 * cc ** 7 * dt ** 6 * dx ** 3 * dz ** 5 * d
     #y ** 6 - 0.432E3 * cc ** 7 * dt ** 6 * dx ** 3 * dy ** 5 * dz ** 6
     # - 0.720E3 * cc ** 7 * dt ** 6 * dy ** 7 * dz ** 5 * dx ** 2 - 0.2
     #40E3 * cc ** 7 * dt ** 6 * dx * dy ** 7 * dz ** 6 + 0.2160E4 * cc 
     #** 6 * dt ** 5 * dx ** 7 * dy ** 7 * dz + 0.3456E4 * cc ** 6 * dt 
     #** 5 * dx ** 7 * dz ** 3 * dy ** 5 + 0.2592E4 * cc ** 6 * dt ** 5 
     #* dx ** 7 * dz ** 5 * dy ** 3 + 0.3456E4 * cc ** 6 * dt ** 5 * dy 
     #** 7 * dz ** 3 * dx ** 5 + 0.3456E4 * cc ** 6 * dt ** 5 * dy ** 5 
     #* dz ** 5 * dx ** 5 + 0.2592E4 * cc ** 6 * dt ** 5 * dy ** 7 * dz 
     #** 5 * dx ** 3 + 0.6720E4 * cc ** 5 * dt ** 4 * dx ** 7 * dy ** 7 
     #* dz ** 2 + 0.3240E4 * cc ** 5 * dt ** 4 * dx ** 7 * dz ** 3 * dy 
     #** 6 + 0.6840E4 * cc ** 5 * dt ** 4 * dx ** 7 * dy ** 5 * dz ** 4 
     #+ 0.5040E4 * cc ** 5 * dt ** 4 * dx ** 7 * dz ** 5 * dy ** 4 + 0.2
     #640E4 * cc ** 5 * dt ** 4 * dx ** 7 * dy ** 3 * dz ** 6 + 0.3240E4
     # * cc ** 5 * dt ** 4 * dy ** 7 * dz ** 3 * dx ** 6 + 0.3120E4 * cc
     # ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx ** 6 + 0.6840E4 * cc ** 5
     # * dt ** 4 * dx ** 5 * dy ** 7 * dz ** 4 + 0.3120E4 * cc ** 5 * dt
     # ** 4 * dx ** 5 * dz ** 5 * dy ** 6 + 0.3360E4 * cc ** 5 * dt ** 4
     # * dx ** 5 * dy ** 5 * dz ** 6 + 0.5040E4 * cc ** 5 * dt ** 4 * dy
     # ** 7 * dz ** 5 * dx ** 4 + 0.2640E4 * cc ** 5 * dt ** 4 * dx ** 3
     # * dy ** 7 * dz ** 6 - 0.18720E5 * cc ** 4 * dt ** 3 * dx ** 7 * d
     #y ** 7 * dz ** 3 - 0.18240E5 * cc ** 4 * dt ** 3 * dx ** 7 * dz **
     # 5 * dy ** 5 - 0.18240E5 * cc ** 4 * dt ** 3 * dy ** 7 * dz ** 5 *
     # dx ** 5 - 0.14112E5 * cc ** 3 * dt ** 2 * dx ** 7 * dy ** 7 * dz 
     #** 4 - 0.5760E4 * cc ** 3 * dt ** 2 * dx ** 7 * dz ** 5 * dy ** 6 
     #- 0.6720E4 * cc ** 3 * dt ** 2 * dx ** 7 * dy ** 5 * dz ** 6 - 0.5
     #760E4 * cc ** 3 * dt ** 2 * dy ** 7 * dz ** 5 * dx ** 6 - 0.6720E4
     # * cc ** 3 * dt ** 2 * dx ** 5 * dy ** 7 * dz ** 6 + 0.25920E5 * c
     #c ** 2 * dt * dx ** 7 * dy ** 7 * dz ** 5) / dx ** 7 / dy ** 7 / d
     #z ** 7 / 0.17280E5
      cvu(365) = (0.840E3 * cc ** 7 * dt ** 6 * dx ** 7 * dy ** 7 + 0.48
     #0E3 * cc ** 7 * dt ** 6 * dx ** 7 * dz * dy ** 6 + 0.1440E4 * cc *
     #* 7 * dt ** 6 * dx ** 7 * dy ** 5 * dz ** 2 + 0.1296E4 * cc ** 7 *
     # dt ** 6 * dx ** 7 * dz ** 3 * dy ** 4 + 0.1296E4 * cc ** 7 * dt *
     #* 6 * dx ** 7 * dy ** 3 * dz ** 4 + 0.1440E4 * cc ** 7 * dt ** 6 *
     # dx ** 7 * dz ** 5 * dy ** 2 + 0.480E3 * cc ** 7 * dt ** 6 * dx **
     # 7 * dy * dz ** 6 + 0.840E3 * cc ** 7 * dt ** 6 * dx ** 7 * dz ** 
     #7 + 0.480E3 * cc ** 7 * dt ** 6 * dy ** 7 * dz * dx ** 6 + 0.864E3
     # * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 3 * dx ** 6 + 0.864E3 * cc 
     #** 7 * dt ** 6 * dy ** 3 * dz ** 5 * dx ** 6 + 0.480E3 * cc ** 7 *
     # dt ** 6 * dy * dz ** 7 * dx ** 6 + 0.1440E4 * cc ** 7 * dt ** 6 *
     # dx ** 5 * dy ** 7 * dz ** 2 + 0.864E3 * cc ** 7 * dt ** 6 * dx **
     # 5 * dz ** 3 * dy ** 6 + 0.1728E4 * cc ** 7 * dt ** 6 * dx ** 5 * 
     #dy ** 5 * dz ** 4 + 0.1728E4 * cc ** 7 * dt ** 6 * dx ** 5 * dz **
     # 5 * dy ** 4 + 0.864E3 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 3 * d
     #z ** 6 + 0.1440E4 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 7 * dy ** 
     #2 + 0.1296E4 * cc ** 7 * dt ** 6 * dy ** 7 * dz ** 3 * dx ** 4 + 0
     #.1728E4 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 4 + 0.1296
     #E4 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 7 * dx ** 4 + 0.1296E4 * 
     #cc ** 7 * dt ** 6 * dx ** 3 * dy ** 7 * dz ** 4 + 0.864E3 * cc ** 
     #7 * dt ** 6 * dx ** 3 * dz ** 5 * dy ** 6 + 0.864E3 * cc ** 7 * dt
     # ** 6 * dx ** 3 * dy ** 5 * dz ** 6 + 0.1296E4 * cc ** 7 * dt ** 6
     # * dx ** 3 * dz ** 7 * dy ** 4 + 0.1440E4 * cc ** 7 * dt ** 6 * dy
     # ** 7 * dz ** 5 * dx ** 2 + 0.1440E4 * cc ** 7 * dt ** 6 * dy ** 5
     # * dz ** 7 * dx ** 2 + 0.480E3 * cc ** 7 * dt ** 6 * dx * dy ** 7 
     #* dz ** 6 + 0.480E3 * cc ** 7 * dt ** 6 * dx * dz ** 7 * dy ** 6 +
     # 0.840E3 * cc ** 7 * dt ** 6 * dy ** 7 * dz ** 7 - 0.2880E4 * cc *
     #* 6 * dt ** 5 * dx ** 7 * dy ** 7 * dz - 0.5184E4 * cc ** 6 * dt *
     #* 5 * dx ** 7 * dz ** 3 * dy ** 5 - 0.5184E4 * cc ** 6 * dt ** 5 *
     # dx ** 7 * dz ** 5 * dy ** 3 - 0.2880E4 * cc ** 6 * dt ** 5 * dx *
     #* 7 * dz ** 7 * dy - 0.5184E4 * cc ** 6 * dt ** 5 * dy ** 7 * dz *
     #* 3 * dx ** 5 - 0.6912E4 * cc ** 6 * dt ** 5 * dy ** 5 * dz ** 5 *
     # dx ** 5 - 0.5184E4 * cc ** 6 * dt ** 5 * dy ** 3 * dz ** 7 * dx *
     #* 5 - 0.5184E4 * cc ** 6 * dt ** 5 * dy ** 7 * dz ** 5 * dx ** 3 -
     # 0.5184E4 * cc ** 6 * dt ** 5 * dy ** 5 * dz ** 7 * dx ** 3 - 0.28
     #80E4 * cc ** 6 * dt ** 5 * dy ** 7 * dz ** 7 * dx - 0.8400E4 * cc 
     #** 5 * dt ** 4 * dx ** 7 * dy ** 7 * dz ** 2 - 0.4560E4 * cc ** 5 
     #* dt ** 4 * dx ** 7 * dz ** 3 * dy ** 6 - 0.9360E4 * cc ** 5 * dt 
     #** 4 * dx ** 7 * dy ** 5 * dz ** 4 - 0.9360E4 * cc ** 5 * dt ** 4 
     #* dx ** 7 * dz ** 5 * dy ** 4 - 0.4560E4 * cc ** 5 * dt ** 4 * dx 
     #** 7 * dy ** 3 * dz ** 6 - 0.8400E4 * cc ** 5 * dt ** 4 * dx ** 7 
     #* dz ** 7 * dy ** 2 - 0.4560E4 * cc ** 5 * dt ** 4 * dy ** 7 * dz 
     #** 3 * dx ** 6 - 0.5760E4 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 
     #* dx ** 6 - 0.4560E4 * cc ** 5 * dt ** 4 * dy ** 3 * dz ** 7 * dx 
     #** 6 - 0.9360E4 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 7 * dz ** 4 
     #- 0.5760E4 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 - 0.5
     #760E4 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6 - 0.9360E4
     # * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 7 * dy ** 4 - 0.9360E4 * cc
     # ** 5 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 4 - 0.9360E4 * cc ** 5
     # * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 4 - 0.4560E4 * cc ** 5 * dt
     # ** 4 * dx ** 3 * dy ** 7 * dz ** 6 - 0.4560E4 * cc ** 5 * dt ** 4
     # * dx ** 3 * dz ** 7 * dy ** 6 - 0.8400E4 * cc ** 5 * dt ** 4 * dy
     # ** 7 * dz ** 7 * dx ** 2 + 0.26880E5 * cc ** 4 * dt ** 3 * dx ** 
     #7 * dy ** 7 * dz ** 3 + 0.34560E5 * cc ** 4 * dt ** 3 * dx ** 7 * 
     #dz ** 5 * dy ** 5 + 0.26880E5 * cc ** 4 * dt ** 3 * dx ** 7 * dz *
     #* 7 * dy ** 3 + 0.34560E5 * cc ** 4 * dt ** 3 * dy ** 7 * dz ** 5 
     #* dx ** 5 + 0.34560E5 * cc ** 4 * dt ** 3 * dy ** 5 * dz ** 7 * dx
     # ** 5 + 0.26880E5 * cc ** 4 * dt ** 3 * dy ** 7 * dz ** 7 * dx ** 
     #3 + 0.17640E5 * cc ** 3 * dt ** 2 * dx ** 7 * dy ** 7 * dz ** 4 + 
     #0.9840E4 * cc ** 3 * dt ** 2 * dx ** 7 * dz ** 5 * dy ** 6 + 0.984
     #0E4 * cc ** 3 * dt ** 2 * dx ** 7 * dy ** 5 * dz ** 6 + 0.17640E5 
     #* cc ** 3 * dt ** 2 * dx ** 7 * dz ** 7 * dy ** 4 + 0.9840E4 * cc 
     #** 3 * dt ** 2 * dy ** 7 * dz ** 5 * dx ** 6 + 0.9840E4 * cc ** 3 
     #* dt ** 2 * dy ** 5 * dz ** 7 * dx ** 6 + 0.9840E4 * cc ** 3 * dt 
     #** 2 * dx ** 5 * dy ** 7 * dz ** 6 + 0.9840E4 * cc ** 3 * dt ** 2 
     #* dx ** 5 * dz ** 7 * dy ** 6 + 0.17640E5 * cc ** 3 * dt ** 2 * dy
     # ** 7 * dz ** 7 * dx ** 4 - 0.47040E5 * cc ** 2 * dt * dx ** 7 * d
     #y ** 7 * dz ** 5 - 0.47040E5 * cc ** 2 * dt * dx ** 7 * dz ** 7 * 
     #dy ** 5 - 0.47040E5 * cc ** 2 * dt * dy ** 7 * dz ** 7 * dx ** 5) 
     #/ dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvu(366) = (-0.672E3 * cc ** 7 * dt ** 6 * dx ** 7 * dy ** 7 - 0.3
     #60E3 * cc ** 7 * dt ** 6 * dx ** 7 * dz * dy ** 6 - 0.1080E4 * cc 
     #** 7 * dt ** 6 * dx ** 7 * dy ** 5 * dz ** 2 - 0.864E3 * cc ** 7 *
     # dt ** 6 * dx ** 7 * dz ** 3 * dy ** 4 - 0.864E3 * cc ** 7 * dt **
     # 6 * dx ** 7 * dy ** 3 * dz ** 4 - 0.720E3 * cc ** 7 * dt ** 6 * d
     #x ** 7 * dz ** 5 * dy ** 2 - 0.240E3 * cc ** 7 * dt ** 6 * dx ** 7
     # * dy * dz ** 6 - 0.360E3 * cc ** 7 * dt ** 6 * dy ** 7 * dz * dx 
     #** 6 - 0.576E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 3 * dx ** 6 -
     # 0.432E3 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 5 * dx ** 6 - 0.108
     #0E4 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 7 * dz ** 2 - 0.576E3 * 
     #cc ** 7 * dt ** 6 * dx ** 5 * dz ** 3 * dy ** 6 - 0.1152E4 * cc **
     # 7 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 4 - 0.864E3 * cc ** 7 * d
     #t ** 6 * dx ** 5 * dz ** 5 * dy ** 4 - 0.432E3 * cc ** 7 * dt ** 6
     # * dx ** 5 * dy ** 3 * dz ** 6 - 0.864E3 * cc ** 7 * dt ** 6 * dy 
     #** 7 * dz ** 3 * dx ** 4 - 0.864E3 * cc ** 7 * dt ** 6 * dy ** 5 *
     # dz ** 5 * dx ** 4 - 0.864E3 * cc ** 7 * dt ** 6 * dx ** 3 * dy **
     # 7 * dz ** 4 - 0.432E3 * cc ** 7 * dt ** 6 * dx ** 3 * dz ** 5 * d
     #y ** 6 - 0.432E3 * cc ** 7 * dt ** 6 * dx ** 3 * dy ** 5 * dz ** 6
     # - 0.720E3 * cc ** 7 * dt ** 6 * dy ** 7 * dz ** 5 * dx ** 2 - 0.2
     #40E3 * cc ** 7 * dt ** 6 * dx * dy ** 7 * dz ** 6 + 0.2160E4 * cc 
     #** 6 * dt ** 5 * dx ** 7 * dy ** 7 * dz + 0.3456E4 * cc ** 6 * dt 
     #** 5 * dx ** 7 * dz ** 3 * dy ** 5 + 0.2592E4 * cc ** 6 * dt ** 5 
     #* dx ** 7 * dz ** 5 * dy ** 3 + 0.3456E4 * cc ** 6 * dt ** 5 * dy 
     #** 7 * dz ** 3 * dx ** 5 + 0.3456E4 * cc ** 6 * dt ** 5 * dy ** 5 
     #* dz ** 5 * dx ** 5 + 0.2592E4 * cc ** 6 * dt ** 5 * dy ** 7 * dz 
     #** 5 * dx ** 3 + 0.6720E4 * cc ** 5 * dt ** 4 * dx ** 7 * dy ** 7 
     #* dz ** 2 + 0.3240E4 * cc ** 5 * dt ** 4 * dx ** 7 * dz ** 3 * dy 
     #** 6 + 0.6840E4 * cc ** 5 * dt ** 4 * dx ** 7 * dy ** 5 * dz ** 4 
     #+ 0.5040E4 * cc ** 5 * dt ** 4 * dx ** 7 * dz ** 5 * dy ** 4 + 0.2
     #640E4 * cc ** 5 * dt ** 4 * dx ** 7 * dy ** 3 * dz ** 6 + 0.3240E4
     # * cc ** 5 * dt ** 4 * dy ** 7 * dz ** 3 * dx ** 6 + 0.3120E4 * cc
     # ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx ** 6 + 0.6840E4 * cc ** 5
     # * dt ** 4 * dx ** 5 * dy ** 7 * dz ** 4 + 0.3120E4 * cc ** 5 * dt
     # ** 4 * dx ** 5 * dz ** 5 * dy ** 6 + 0.3360E4 * cc ** 5 * dt ** 4
     # * dx ** 5 * dy ** 5 * dz ** 6 + 0.5040E4 * cc ** 5 * dt ** 4 * dy
     # ** 7 * dz ** 5 * dx ** 4 + 0.2640E4 * cc ** 5 * dt ** 4 * dx ** 3
     # * dy ** 7 * dz ** 6 - 0.18720E5 * cc ** 4 * dt ** 3 * dx ** 7 * d
     #y ** 7 * dz ** 3 - 0.18240E5 * cc ** 4 * dt ** 3 * dx ** 7 * dz **
     # 5 * dy ** 5 - 0.18240E5 * cc ** 4 * dt ** 3 * dy ** 7 * dz ** 5 *
     # dx ** 5 - 0.14112E5 * cc ** 3 * dt ** 2 * dx ** 7 * dy ** 7 * dz 
     #** 4 - 0.5760E4 * cc ** 3 * dt ** 2 * dx ** 7 * dz ** 5 * dy ** 6 
     #- 0.6720E4 * cc ** 3 * dt ** 2 * dx ** 7 * dy ** 5 * dz ** 6 - 0.5
     #760E4 * cc ** 3 * dt ** 2 * dy ** 7 * dz ** 5 * dx ** 6 - 0.6720E4
     # * cc ** 3 * dt ** 2 * dx ** 5 * dy ** 7 * dz ** 6 + 0.25920E5 * c
     #c ** 2 * dt * dx ** 7 * dy ** 7 * dz ** 5) / dx ** 7 / dy ** 7 / d
     #z ** 7 / 0.17280E5
      cvu(367) = (0.336E3 * cc ** 7 * dt ** 6 * dx ** 7 * dy ** 7 + 0.14
     #4E3 * cc ** 7 * dt ** 6 * dx ** 7 * dz * dy ** 6 + 0.432E3 * cc **
     # 7 * dt ** 6 * dx ** 7 * dy ** 5 * dz ** 2 + 0.216E3 * cc ** 7 * d
     #t ** 6 * dx ** 7 * dz ** 3 * dy ** 4 + 0.216E3 * cc ** 7 * dt ** 6
     # * dx ** 7 * dy ** 3 * dz ** 4 + 0.144E3 * cc ** 7 * dt ** 6 * dy 
     #** 7 * dz * dx ** 6 + 0.144E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz *
     #* 3 * dx ** 6 + 0.432E3 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 7 * 
     #dz ** 2 + 0.144E3 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 3 * dy ** 
     #6 + 0.288E3 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 4 + 0.
     #216E3 * cc ** 7 * dt ** 6 * dy ** 7 * dz ** 3 * dx ** 4 + 0.216E3 
     #* cc ** 7 * dt ** 6 * dx ** 3 * dy ** 7 * dz ** 4 - 0.864E3 * cc *
     #* 6 * dt ** 5 * dx ** 7 * dy ** 7 * dz - 0.864E3 * cc ** 6 * dt **
     # 5 * dx ** 7 * dz ** 3 * dy ** 5 - 0.864E3 * cc ** 6 * dt ** 5 * d
     #y ** 7 * dz ** 3 * dx ** 5 - 0.3360E4 * cc ** 5 * dt ** 4 * dx ** 
     #7 * dy ** 7 * dz ** 2 - 0.1080E4 * cc ** 5 * dt ** 4 * dx ** 7 * d
     #z ** 3 * dy ** 6 - 0.2520E4 * cc ** 5 * dt ** 4 * dx ** 7 * dy ** 
     #5 * dz ** 4 - 0.360E3 * cc ** 5 * dt ** 4 * dx ** 7 * dz ** 5 * dy
     # ** 4 - 0.360E3 * cc ** 5 * dt ** 4 * dx ** 7 * dy ** 3 * dz ** 6 
     #- 0.1080E4 * cc ** 5 * dt ** 4 * dy ** 7 * dz ** 3 * dx ** 6 - 0.2
     #40E3 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx ** 6 - 0.2520E4 
     #* cc ** 5 * dt ** 4 * dx ** 5 * dy ** 7 * dz ** 4 - 0.240E3 * cc *
     #* 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 - 0.480E3 * cc ** 5 * 
     #dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6 - 0.360E3 * cc ** 5 * dt ** 
     #4 * dy ** 7 * dz ** 5 * dx ** 4 - 0.360E3 * cc ** 5 * dt ** 4 * dx
     # ** 3 * dy ** 7 * dz ** 6 + 0.5760E4 * cc ** 4 * dt ** 3 * dx ** 7
     # * dy ** 7 * dz ** 3 + 0.960E3 * cc ** 4 * dt ** 3 * dx ** 7 * dz 
     #** 5 * dy ** 5 + 0.960E3 * cc ** 4 * dt ** 3 * dy ** 7 * dz ** 5 *
     # dx ** 5 + 0.7056E4 * cc ** 3 * dt ** 2 * dx ** 7 * dy ** 7 * dz *
     #* 4 + 0.936E3 * cc ** 3 * dt ** 2 * dx ** 7 * dz ** 5 * dy ** 6 + 
     #0.2088E4 * cc ** 3 * dt ** 2 * dx ** 7 * dy ** 5 * dz ** 6 + 0.936
     #E3 * cc ** 3 * dt ** 2 * dy ** 7 * dz ** 5 * dx ** 6 + 0.2088E4 * 
     #cc ** 3 * dt ** 2 * dx ** 5 * dy ** 7 * dz ** 6 - 0.2592E4 * cc **
     # 2 * dt * dx ** 7 * dy ** 7 * dz ** 5) / dx ** 7 / dy ** 7 / dz **
     # 7 / 0.17280E5
      cvu(368) = (-0.96E2 * cc ** 7 * dt ** 6 * dx ** 7 * dy ** 7 - 0.24
     #E2 * cc ** 7 * dt ** 6 * dx ** 7 * dz * dy ** 6 - 0.72E2 * cc ** 7
     # * dt ** 6 * dx ** 7 * dy ** 5 * dz ** 2 - 0.24E2 * cc ** 7 * dt *
     #* 6 * dy ** 7 * dz * dx ** 6 - 0.72E2 * cc ** 7 * dt ** 6 * dx ** 
     #5 * dy ** 7 * dz ** 2 + 0.144E3 * cc ** 6 * dt ** 5 * dx ** 7 * dy
     # ** 7 * dz + 0.960E3 * cc ** 5 * dt ** 4 * dx ** 7 * dy ** 7 * dz 
     #** 2 + 0.120E3 * cc ** 5 * dt ** 4 * dx ** 7 * dz ** 3 * dy ** 6 +
     # 0.360E3 * cc ** 5 * dt ** 4 * dx ** 7 * dy ** 5 * dz ** 4 + 0.120
     #E3 * cc ** 5 * dt ** 4 * dy ** 7 * dz ** 3 * dx ** 6 + 0.360E3 * c
     #c ** 5 * dt ** 4 * dx ** 5 * dy ** 7 * dz ** 4 - 0.480E3 * cc ** 4
     # * dt ** 3 * dx ** 7 * dy ** 7 * dz ** 3 - 0.2016E4 * cc ** 3 * dt
     # ** 2 * dx ** 7 * dy ** 7 * dz ** 4 - 0.96E2 * cc ** 3 * dt ** 2 *
     # dx ** 7 * dz ** 5 * dy ** 6 - 0.288E3 * cc ** 3 * dt ** 2 * dx **
     # 7 * dy ** 5 * dz ** 6 - 0.96E2 * cc ** 3 * dt ** 2 * dy ** 7 * dz
     # ** 5 * dx ** 6 - 0.288E3 * cc ** 3 * dt ** 2 * dx ** 5 * dy ** 7 
     #* dz ** 6 + 0.192E3 * cc ** 2 * dt * dx ** 7 * dy ** 7 * dz ** 5) 
     #/ dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvu(369) = (0.12E2 * cc ** 7 * dt ** 6 * dx ** 7 * dy ** 7 - 0.120
     #E3 * cc ** 5 * dt ** 4 * dx ** 7 * dy ** 7 * dz ** 2 + 0.252E3 * c
     #c ** 3 * dt ** 2 * dx ** 7 * dy ** 7 * dz ** 4) / dx ** 7 / dy ** 
     #7 / dz ** 7 / 0.17280E5
      cvu(371) = (0.12E2 * cc ** 7 * dt ** 6 * dx ** 7 * dz * dy ** 6 + 
     #0.36E2 * cc ** 7 * dt ** 6 * dx ** 7 * dy ** 5 * dz ** 2 - 0.60E2 
     #* cc ** 5 * dt ** 4 * dx ** 7 * dz ** 3 * dy ** 6 - 0.180E3 * cc *
     #* 5 * dt ** 4 * dx ** 7 * dy ** 5 * dz ** 4 + 0.48E2 * cc ** 3 * d
     #t ** 2 * dx ** 7 * dz ** 5 * dy ** 6 + 0.144E3 * cc ** 3 * dt ** 2
     # * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.
     #17280E5
      cvu(372) = (-0.72E2 * cc ** 7 * dt ** 6 * dx ** 7 * dz * dy ** 6 -
     # 0.216E3 * cc ** 7 * dt ** 6 * dx ** 7 * dy ** 5 * dz ** 2 - 0.144
     #E3 * cc ** 7 * dt ** 6 * dx ** 7 * dz ** 3 * dy ** 4 - 0.144E3 * c
     #c ** 7 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 4 - 0.72E2 * cc ** 7 
     #* dt ** 6 * dy ** 5 * dz ** 3 * dx ** 6 - 0.72E2 * cc ** 7 * dt **
     # 6 * dx ** 5 * dz ** 3 * dy ** 6 - 0.144E3 * cc ** 7 * dt ** 6 * d
     #x ** 5 * dy ** 5 * dz ** 4 + 0.432E3 * cc ** 6 * dt ** 5 * dx ** 7
     # * dz ** 3 * dy ** 5 + 0.600E3 * cc ** 5 * dt ** 4 * dx ** 7 * dz 
     #** 3 * dy ** 6 + 0.1320E4 * cc ** 5 * dt ** 4 * dx ** 7 * dy ** 5 
     #* dz ** 4 + 0.240E3 * cc ** 5 * dt ** 4 * dx ** 7 * dz ** 5 * dy *
     #* 4 + 0.240E3 * cc ** 5 * dt ** 4 * dx ** 7 * dy ** 3 * dz ** 6 + 
     #0.120E3 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx ** 6 + 0.120E
     #3 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 + 0.240E3 * cc
     # ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6 - 0.480E3 * cc ** 4 
     #* dt ** 3 * dx ** 7 * dz ** 5 * dy ** 5 - 0.528E3 * cc ** 3 * dt *
     #* 2 * dx ** 7 * dz ** 5 * dy ** 6 - 0.1104E4 * cc ** 3 * dt ** 2 *
     # dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17
     #280E5
      cvu(373) = (0.180E3 * cc ** 7 * dt ** 6 * dx ** 7 * dz * dy ** 6 +
     # 0.540E3 * cc ** 7 * dt ** 6 * dx ** 7 * dy ** 5 * dz ** 2 + 0.576
     #E3 * cc ** 7 * dt ** 6 * dx ** 7 * dz ** 3 * dy ** 4 + 0.576E3 * c
     #c ** 7 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 4 + 0.540E3 * cc ** 7
     # * dt ** 6 * dx ** 7 * dz ** 5 * dy ** 2 + 0.180E3 * cc ** 7 * dt 
     #** 6 * dx ** 7 * dy * dz ** 6 + 0.288E3 * cc ** 7 * dt ** 6 * dy *
     #* 5 * dz ** 3 * dx ** 6 + 0.288E3 * cc ** 7 * dt ** 6 * dy ** 3 * 
     #dz ** 5 * dx ** 6 + 0.288E3 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 
     #3 * dy ** 6 + 0.576E3 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 5 * dz
     # ** 4 + 0.576E3 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 4 
     #+ 0.288E3 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 3 * dz ** 6 + 0.43
     #2E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 4 + 0.216E3 * 
     #cc ** 7 * dt ** 6 * dx ** 3 * dz ** 5 * dy ** 6 + 0.216E3 * cc ** 
     #7 * dt ** 6 * dx ** 3 * dy ** 5 * dz ** 6 - 0.1728E4 * cc ** 6 * d
     #t ** 5 * dx ** 7 * dz ** 3 * dy ** 5 - 0.1728E4 * cc ** 6 * dt ** 
     #5 * dx ** 7 * dz ** 5 * dy ** 3 - 0.1728E4 * cc ** 6 * dt ** 5 * d
     #y ** 5 * dz ** 5 * dx ** 5 - 0.1860E4 * cc ** 5 * dt ** 4 * dx ** 
     #7 * dz ** 3 * dy ** 6 - 0.3660E4 * cc ** 5 * dt ** 4 * dx ** 7 * d
     #y ** 5 * dz ** 4 - 0.3660E4 * cc ** 5 * dt ** 4 * dx ** 7 * dz ** 
     #5 * dy ** 4 - 0.1860E4 * cc ** 5 * dt ** 4 * dx ** 7 * dy ** 3 * d
     #z ** 6 - 0.1680E4 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx ** 
     #6 - 0.1800E4 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 - 0
     #.1800E4 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6 + 0.9600
     #E4 * cc ** 4 * dt ** 3 * dx ** 7 * dz ** 5 * dy ** 5 + 0.3840E4 * 
     #cc ** 3 * dt ** 2 * dx ** 7 * dz ** 5 * dy ** 6 + 0.3840E4 * cc **
     # 3 * dt ** 2 * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / 
     #dz ** 7 / 0.17280E5
      cvu(374) = (-0.240E3 * cc ** 7 * dt ** 6 * dx ** 7 * dz * dy ** 6 
     #- 0.720E3 * cc ** 7 * dt ** 6 * dx ** 7 * dy ** 5 * dz ** 2 - 0.86
     #4E3 * cc ** 7 * dt ** 6 * dx ** 7 * dz ** 3 * dy ** 4 - 0.864E3 * 
     #cc ** 7 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 4 - 0.1080E4 * cc **
     # 7 * dt ** 6 * dx ** 7 * dz ** 5 * dy ** 2 - 0.360E3 * cc ** 7 * d
     #t ** 6 * dx ** 7 * dy * dz ** 6 - 0.672E3 * cc ** 7 * dt ** 6 * dx
     # ** 7 * dz ** 7 - 0.432E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 3 
     #* dx ** 6 - 0.576E3 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 5 * dx *
     #* 6 - 0.360E3 * cc ** 7 * dt ** 6 * dy * dz ** 7 * dx ** 6 - 0.432
     #E3 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 3 * dy ** 6 - 0.864E3 * c
     #c ** 7 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 4 - 0.1152E4 * cc ** 
     #7 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 4 - 0.576E3 * cc ** 7 * dt
     # ** 6 * dx ** 5 * dy ** 3 * dz ** 6 - 0.1080E4 * cc ** 7 * dt ** 6
     # * dx ** 5 * dz ** 7 * dy ** 2 - 0.864E3 * cc ** 7 * dt ** 6 * dy 
     #** 5 * dz ** 5 * dx ** 4 - 0.864E3 * cc ** 7 * dt ** 6 * dy ** 3 *
     # dz ** 7 * dx ** 4 - 0.432E3 * cc ** 7 * dt ** 6 * dx ** 3 * dz **
     # 5 * dy ** 6 - 0.432E3 * cc ** 7 * dt ** 6 * dx ** 3 * dy ** 5 * d
     #z ** 6 - 0.864E3 * cc ** 7 * dt ** 6 * dx ** 3 * dz ** 7 * dy ** 4
     # - 0.720E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 7 * dx ** 2 - 0.2
     #40E3 * cc ** 7 * dt ** 6 * dx * dz ** 7 * dy ** 6 + 0.2592E4 * cc 
     #** 6 * dt ** 5 * dx ** 7 * dz ** 3 * dy ** 5 + 0.3456E4 * cc ** 6 
     #* dt ** 5 * dx ** 7 * dz ** 5 * dy ** 3 + 0.2160E4 * cc ** 6 * dt 
     #** 5 * dx ** 7 * dz ** 7 * dy + 0.3456E4 * cc ** 6 * dt ** 5 * dy 
     #** 5 * dz ** 5 * dx ** 5 + 0.3456E4 * cc ** 6 * dt ** 5 * dy ** 3 
     #* dz ** 7 * dx ** 5 + 0.2592E4 * cc ** 6 * dt ** 5 * dy ** 5 * dz 
     #** 7 * dx ** 3 + 0.2640E4 * cc ** 5 * dt ** 4 * dx ** 7 * dz ** 3 
     #* dy ** 6 + 0.5040E4 * cc ** 5 * dt ** 4 * dx ** 7 * dy ** 5 * dz 
     #** 4 + 0.6840E4 * cc ** 5 * dt ** 4 * dx ** 7 * dz ** 5 * dy ** 4 
     #+ 0.3240E4 * cc ** 5 * dt ** 4 * dx ** 7 * dy ** 3 * dz ** 6 + 0.6
     #720E4 * cc ** 5 * dt ** 4 * dx ** 7 * dz ** 7 * dy ** 2 + 0.3120E4
     # * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx ** 6 + 0.3240E4 * cc
     # ** 5 * dt ** 4 * dy ** 3 * dz ** 7 * dx ** 6 + 0.3360E4 * cc ** 5
     # * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 + 0.3120E4 * cc ** 5 * dt
     # ** 4 * dx ** 5 * dy ** 5 * dz ** 6 + 0.6840E4 * cc ** 5 * dt ** 4
     # * dx ** 5 * dz ** 7 * dy ** 4 + 0.5040E4 * cc ** 5 * dt ** 4 * dy
     # ** 5 * dz ** 7 * dx ** 4 + 0.2640E4 * cc ** 5 * dt ** 4 * dx ** 3
     # * dz ** 7 * dy ** 6 - 0.18240E5 * cc ** 4 * dt ** 3 * dx ** 7 * d
     #z ** 5 * dy ** 5 - 0.18720E5 * cc ** 4 * dt ** 3 * dx ** 7 * dz **
     # 7 * dy ** 3 - 0.18240E5 * cc ** 4 * dt ** 3 * dy ** 5 * dz ** 7 *
     # dx ** 5 - 0.6720E4 * cc ** 3 * dt ** 2 * dx ** 7 * dz ** 5 * dy *
     #* 6 - 0.5760E4 * cc ** 3 * dt ** 2 * dx ** 7 * dy ** 5 * dz ** 6 -
     # 0.14112E5 * cc ** 3 * dt ** 2 * dx ** 7 * dz ** 7 * dy ** 4 - 0.5
     #760E4 * cc ** 3 * dt ** 2 * dy ** 5 * dz ** 7 * dx ** 6 - 0.6720E4
     # * cc ** 3 * dt ** 2 * dx ** 5 * dz ** 7 * dy ** 6 + 0.25920E5 * c
     #c ** 2 * dt * dx ** 7 * dz ** 7 * dy ** 5) / dx ** 7 / dy ** 7 / d
     #z ** 7 / 0.17280E5
      cvu(375) = (0.180E3 * cc ** 7 * dt ** 6 * dx ** 7 * dz * dy ** 6 +
     # 0.540E3 * cc ** 7 * dt ** 6 * dx ** 7 * dy ** 5 * dz ** 2 + 0.576
     #E3 * cc ** 7 * dt ** 6 * dx ** 7 * dz ** 3 * dy ** 4 + 0.576E3 * c
     #c ** 7 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 4 + 0.540E3 * cc ** 7
     # * dt ** 6 * dx ** 7 * dz ** 5 * dy ** 2 + 0.180E3 * cc ** 7 * dt 
     #** 6 * dx ** 7 * dy * dz ** 6 + 0.288E3 * cc ** 7 * dt ** 6 * dy *
     #* 5 * dz ** 3 * dx ** 6 + 0.288E3 * cc ** 7 * dt ** 6 * dy ** 3 * 
     #dz ** 5 * dx ** 6 + 0.288E3 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 
     #3 * dy ** 6 + 0.576E3 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 5 * dz
     # ** 4 + 0.576E3 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 4 
     #+ 0.288E3 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 3 * dz ** 6 + 0.43
     #2E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 4 + 0.216E3 * 
     #cc ** 7 * dt ** 6 * dx ** 3 * dz ** 5 * dy ** 6 + 0.216E3 * cc ** 
     #7 * dt ** 6 * dx ** 3 * dy ** 5 * dz ** 6 - 0.1728E4 * cc ** 6 * d
     #t ** 5 * dx ** 7 * dz ** 3 * dy ** 5 - 0.1728E4 * cc ** 6 * dt ** 
     #5 * dx ** 7 * dz ** 5 * dy ** 3 - 0.1728E4 * cc ** 6 * dt ** 5 * d
     #y ** 5 * dz ** 5 * dx ** 5 - 0.1860E4 * cc ** 5 * dt ** 4 * dx ** 
     #7 * dz ** 3 * dy ** 6 - 0.3660E4 * cc ** 5 * dt ** 4 * dx ** 7 * d
     #y ** 5 * dz ** 4 - 0.3660E4 * cc ** 5 * dt ** 4 * dx ** 7 * dz ** 
     #5 * dy ** 4 - 0.1860E4 * cc ** 5 * dt ** 4 * dx ** 7 * dy ** 3 * d
     #z ** 6 - 0.1680E4 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx ** 
     #6 - 0.1800E4 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 - 0
     #.1800E4 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6 + 0.9600
     #E4 * cc ** 4 * dt ** 3 * dx ** 7 * dz ** 5 * dy ** 5 + 0.3840E4 * 
     #cc ** 3 * dt ** 2 * dx ** 7 * dz ** 5 * dy ** 6 + 0.3840E4 * cc **
     # 3 * dt ** 2 * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / 
     #dz ** 7 / 0.17280E5
      cvu(376) = (-0.72E2 * cc ** 7 * dt ** 6 * dx ** 7 * dz * dy ** 6 -
     # 0.216E3 * cc ** 7 * dt ** 6 * dx ** 7 * dy ** 5 * dz ** 2 - 0.144
     #E3 * cc ** 7 * dt ** 6 * dx ** 7 * dz ** 3 * dy ** 4 - 0.144E3 * c
     #c ** 7 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 4 - 0.72E2 * cc ** 7 
     #* dt ** 6 * dy ** 5 * dz ** 3 * dx ** 6 - 0.72E2 * cc ** 7 * dt **
     # 6 * dx ** 5 * dz ** 3 * dy ** 6 - 0.144E3 * cc ** 7 * dt ** 6 * d
     #x ** 5 * dy ** 5 * dz ** 4 + 0.432E3 * cc ** 6 * dt ** 5 * dx ** 7
     # * dz ** 3 * dy ** 5 + 0.600E3 * cc ** 5 * dt ** 4 * dx ** 7 * dz 
     #** 3 * dy ** 6 + 0.1320E4 * cc ** 5 * dt ** 4 * dx ** 7 * dy ** 5 
     #* dz ** 4 + 0.240E3 * cc ** 5 * dt ** 4 * dx ** 7 * dz ** 5 * dy *
     #* 4 + 0.240E3 * cc ** 5 * dt ** 4 * dx ** 7 * dy ** 3 * dz ** 6 + 
     #0.120E3 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx ** 6 + 0.120E
     #3 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 + 0.240E3 * cc
     # ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6 - 0.480E3 * cc ** 4 
     #* dt ** 3 * dx ** 7 * dz ** 5 * dy ** 5 - 0.528E3 * cc ** 3 * dt *
     #* 2 * dx ** 7 * dz ** 5 * dy ** 6 - 0.1104E4 * cc ** 3 * dt ** 2 *
     # dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17
     #280E5
      cvu(377) = (0.12E2 * cc ** 7 * dt ** 6 * dx ** 7 * dz * dy ** 6 + 
     #0.36E2 * cc ** 7 * dt ** 6 * dx ** 7 * dy ** 5 * dz ** 2 - 0.60E2 
     #* cc ** 5 * dt ** 4 * dx ** 7 * dz ** 3 * dy ** 6 - 0.180E3 * cc *
     #* 5 * dt ** 4 * dx ** 7 * dy ** 5 * dz ** 4 + 0.48E2 * cc ** 3 * d
     #t ** 2 * dx ** 7 * dz ** 5 * dy ** 6 + 0.144E3 * cc ** 3 * dt ** 2
     # * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.
     #17280E5
      cvu(381) = (0.36E2 * cc ** 7 * dt ** 6 * dx ** 7 * dz ** 3 * dy **
     # 4 + 0.36E2 * cc ** 7 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 4 - 0.
     #60E2 * cc ** 5 * dt ** 4 * dx ** 7 * dz ** 3 * dy ** 6 - 0.60E2 * 
     #cc ** 5 * dt ** 4 * dx ** 7 * dy ** 5 * dz ** 4 - 0.60E2 * cc ** 5
     # * dt ** 4 * dx ** 7 * dz ** 5 * dy ** 4 - 0.60E2 * cc ** 5 * dt *
     #* 4 * dx ** 7 * dy ** 3 * dz ** 6 + 0.60E2 * cc ** 3 * dt ** 2 * d
     #x ** 7 * dz ** 5 * dy ** 6 + 0.60E2 * cc ** 3 * dt ** 2 * dx ** 7 
     #* dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvu(382) = (-0.144E3 * cc ** 7 * dt ** 6 * dx ** 7 * dz ** 3 * dy 
     #** 4 - 0.144E3 * cc ** 7 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 4 -
     # 0.216E3 * cc ** 7 * dt ** 6 * dx ** 7 * dz ** 5 * dy ** 2 - 0.72E
     #2 * cc ** 7 * dt ** 6 * dx ** 7 * dy * dz ** 6 - 0.72E2 * cc ** 7 
     #* dt ** 6 * dy ** 3 * dz ** 5 * dx ** 6 - 0.144E3 * cc ** 7 * dt *
     #* 6 * dx ** 5 * dz ** 5 * dy ** 4 - 0.72E2 * cc ** 7 * dt ** 6 * d
     #x ** 5 * dy ** 3 * dz ** 6 + 0.432E3 * cc ** 6 * dt ** 5 * dx ** 7
     # * dz ** 5 * dy ** 3 + 0.240E3 * cc ** 5 * dt ** 4 * dx ** 7 * dz 
     #** 3 * dy ** 6 + 0.240E3 * cc ** 5 * dt ** 4 * dx ** 7 * dy ** 5 *
     # dz ** 4 + 0.1320E4 * cc ** 5 * dt ** 4 * dx ** 7 * dz ** 5 * dy *
     #* 4 + 0.600E3 * cc ** 5 * dt ** 4 * dx ** 7 * dy ** 3 * dz ** 6 + 
     #0.120E3 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx ** 6 + 0.240E
     #3 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 + 0.120E3 * cc
     # ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6 - 0.480E3 * cc ** 4 
     #* dt ** 3 * dx ** 7 * dz ** 5 * dy ** 5 - 0.1104E4 * cc ** 3 * dt 
     #** 2 * dx ** 7 * dz ** 5 * dy ** 6 - 0.528E3 * cc ** 3 * dt ** 2 *
     # dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17
     #280E5
      cvu(383) = (0.216E3 * cc ** 7 * dt ** 6 * dx ** 7 * dz ** 3 * dy *
     #* 4 + 0.216E3 * cc ** 7 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 4 + 
     #0.432E3 * cc ** 7 * dt ** 6 * dx ** 7 * dz ** 5 * dy ** 2 + 0.144E
     #3 * cc ** 7 * dt ** 6 * dx ** 7 * dy * dz ** 6 + 0.336E3 * cc ** 7
     # * dt ** 6 * dx ** 7 * dz ** 7 + 0.144E3 * cc ** 7 * dt ** 6 * dy 
     #** 3 * dz ** 5 * dx ** 6 + 0.144E3 * cc ** 7 * dt ** 6 * dy * dz *
     #* 7 * dx ** 6 + 0.288E3 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 5 * 
     #dy ** 4 + 0.144E3 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 3 * dz ** 
     #6 + 0.432E3 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 7 * dy ** 2 + 0.
     #216E3 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 7 * dx ** 4 + 0.216E3 
     #* cc ** 7 * dt ** 6 * dx ** 3 * dz ** 7 * dy ** 4 - 0.864E3 * cc *
     #* 6 * dt ** 5 * dx ** 7 * dz ** 5 * dy ** 3 - 0.864E3 * cc ** 6 * 
     #dt ** 5 * dx ** 7 * dz ** 7 * dy - 0.864E3 * cc ** 6 * dt ** 5 * d
     #y ** 3 * dz ** 7 * dx ** 5 - 0.360E3 * cc ** 5 * dt ** 4 * dx ** 7
     # * dz ** 3 * dy ** 6 - 0.360E3 * cc ** 5 * dt ** 4 * dx ** 7 * dy 
     #** 5 * dz ** 4 - 0.2520E4 * cc ** 5 * dt ** 4 * dx ** 7 * dz ** 5 
     #* dy ** 4 - 0.1080E4 * cc ** 5 * dt ** 4 * dx ** 7 * dy ** 3 * dz 
     #** 6 - 0.3360E4 * cc ** 5 * dt ** 4 * dx ** 7 * dz ** 7 * dy ** 2 
     #- 0.240E3 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx ** 6 - 0.10
     #80E4 * cc ** 5 * dt ** 4 * dy ** 3 * dz ** 7 * dx ** 6 - 0.480E3 *
     # cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 - 0.240E3 * cc **
     # 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6 - 0.2520E4 * cc ** 5 * 
     #dt ** 4 * dx ** 5 * dz ** 7 * dy ** 4 - 0.360E3 * cc ** 5 * dt ** 
     #4 * dy ** 5 * dz ** 7 * dx ** 4 - 0.360E3 * cc ** 5 * dt ** 4 * dx
     # ** 3 * dz ** 7 * dy ** 6 + 0.960E3 * cc ** 4 * dt ** 3 * dx ** 7 
     #* dz ** 5 * dy ** 5 + 0.5760E4 * cc ** 4 * dt ** 3 * dx ** 7 * dz 
     #** 7 * dy ** 3 + 0.960E3 * cc ** 4 * dt ** 3 * dy ** 5 * dz ** 7 *
     # dx ** 5 + 0.2088E4 * cc ** 3 * dt ** 2 * dx ** 7 * dz ** 5 * dy *
     #* 6 + 0.936E3 * cc ** 3 * dt ** 2 * dx ** 7 * dy ** 5 * dz ** 6 + 
     #0.7056E4 * cc ** 3 * dt ** 2 * dx ** 7 * dz ** 7 * dy ** 4 + 0.936
     #E3 * cc ** 3 * dt ** 2 * dy ** 5 * dz ** 7 * dx ** 6 + 0.2088E4 * 
     #cc ** 3 * dt ** 2 * dx ** 5 * dz ** 7 * dy ** 6 - 0.2592E4 * cc **
     # 2 * dt * dx ** 7 * dz ** 7 * dy ** 5) / dx ** 7 / dy ** 7 / dz **
     # 7 / 0.17280E5
      cvu(384) = (-0.144E3 * cc ** 7 * dt ** 6 * dx ** 7 * dz ** 3 * dy 
     #** 4 - 0.144E3 * cc ** 7 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 4 -
     # 0.216E3 * cc ** 7 * dt ** 6 * dx ** 7 * dz ** 5 * dy ** 2 - 0.72E
     #2 * cc ** 7 * dt ** 6 * dx ** 7 * dy * dz ** 6 - 0.72E2 * cc ** 7 
     #* dt ** 6 * dy ** 3 * dz ** 5 * dx ** 6 - 0.144E3 * cc ** 7 * dt *
     #* 6 * dx ** 5 * dz ** 5 * dy ** 4 - 0.72E2 * cc ** 7 * dt ** 6 * d
     #x ** 5 * dy ** 3 * dz ** 6 + 0.432E3 * cc ** 6 * dt ** 5 * dx ** 7
     # * dz ** 5 * dy ** 3 + 0.240E3 * cc ** 5 * dt ** 4 * dx ** 7 * dz 
     #** 3 * dy ** 6 + 0.240E3 * cc ** 5 * dt ** 4 * dx ** 7 * dy ** 5 *
     # dz ** 4 + 0.1320E4 * cc ** 5 * dt ** 4 * dx ** 7 * dz ** 5 * dy *
     #* 4 + 0.600E3 * cc ** 5 * dt ** 4 * dx ** 7 * dy ** 3 * dz ** 6 + 
     #0.120E3 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx ** 6 + 0.240E
     #3 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 + 0.120E3 * cc
     # ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6 - 0.480E3 * cc ** 4 
     #* dt ** 3 * dx ** 7 * dz ** 5 * dy ** 5 - 0.1104E4 * cc ** 3 * dt 
     #** 2 * dx ** 7 * dz ** 5 * dy ** 6 - 0.528E3 * cc ** 3 * dt ** 2 *
     # dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17
     #280E5
      cvu(385) = (0.36E2 * cc ** 7 * dt ** 6 * dx ** 7 * dz ** 3 * dy **
     # 4 + 0.36E2 * cc ** 7 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 4 - 0.
     #60E2 * cc ** 5 * dt ** 4 * dx ** 7 * dz ** 3 * dy ** 6 - 0.60E2 * 
     #cc ** 5 * dt ** 4 * dx ** 7 * dy ** 5 * dz ** 4 - 0.60E2 * cc ** 5
     # * dt ** 4 * dx ** 7 * dz ** 5 * dy ** 4 - 0.60E2 * cc ** 5 * dt *
     #* 4 * dx ** 7 * dy ** 3 * dz ** 6 + 0.60E2 * cc ** 3 * dt ** 2 * d
     #x ** 7 * dz ** 5 * dy ** 6 + 0.60E2 * cc ** 3 * dt ** 2 * dx ** 7 
     #* dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvu(391) = (0.36E2 * cc ** 7 * dt ** 6 * dx ** 7 * dz ** 5 * dy **
     # 2 + 0.12E2 * cc ** 7 * dt ** 6 * dx ** 7 * dy * dz ** 6 - 0.180E3
     # * cc ** 5 * dt ** 4 * dx ** 7 * dz ** 5 * dy ** 4 - 0.60E2 * cc *
     #* 5 * dt ** 4 * dx ** 7 * dy ** 3 * dz ** 6 + 0.144E3 * cc ** 3 * 
     #dt ** 2 * dx ** 7 * dz ** 5 * dy ** 6 + 0.48E2 * cc ** 3 * dt ** 2
     # * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.
     #17280E5
      cvu(392) = (-0.72E2 * cc ** 7 * dt ** 6 * dx ** 7 * dz ** 5 * dy *
     #* 2 - 0.24E2 * cc ** 7 * dt ** 6 * dx ** 7 * dy * dz ** 6 - 0.96E2
     # * cc ** 7 * dt ** 6 * dx ** 7 * dz ** 7 - 0.24E2 * cc ** 7 * dt *
     #* 6 * dy * dz ** 7 * dx ** 6 - 0.72E2 * cc ** 7 * dt ** 6 * dx ** 
     #5 * dz ** 7 * dy ** 2 + 0.144E3 * cc ** 6 * dt ** 5 * dx ** 7 * dz
     # ** 7 * dy + 0.360E3 * cc ** 5 * dt ** 4 * dx ** 7 * dz ** 5 * dy 
     #** 4 + 0.120E3 * cc ** 5 * dt ** 4 * dx ** 7 * dy ** 3 * dz ** 6 +
     # 0.960E3 * cc ** 5 * dt ** 4 * dx ** 7 * dz ** 7 * dy ** 2 + 0.120
     #E3 * cc ** 5 * dt ** 4 * dy ** 3 * dz ** 7 * dx ** 6 + 0.360E3 * c
     #c ** 5 * dt ** 4 * dx ** 5 * dz ** 7 * dy ** 4 - 0.480E3 * cc ** 4
     # * dt ** 3 * dx ** 7 * dz ** 7 * dy ** 3 - 0.288E3 * cc ** 3 * dt 
     #** 2 * dx ** 7 * dz ** 5 * dy ** 6 - 0.96E2 * cc ** 3 * dt ** 2 * 
     #dx ** 7 * dy ** 5 * dz ** 6 - 0.2016E4 * cc ** 3 * dt ** 2 * dx **
     # 7 * dz ** 7 * dy ** 4 - 0.96E2 * cc ** 3 * dt ** 2 * dy ** 5 * dz
     # ** 7 * dx ** 6 - 0.288E3 * cc ** 3 * dt ** 2 * dx ** 5 * dz ** 7 
     #* dy ** 6 + 0.192E3 * cc ** 2 * dt * dx ** 7 * dz ** 7 * dy ** 5) 
     #/ dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvu(393) = (0.36E2 * cc ** 7 * dt ** 6 * dx ** 7 * dz ** 5 * dy **
     # 2 + 0.12E2 * cc ** 7 * dt ** 6 * dx ** 7 * dy * dz ** 6 - 0.180E3
     # * cc ** 5 * dt ** 4 * dx ** 7 * dz ** 5 * dy ** 4 - 0.60E2 * cc *
     #* 5 * dt ** 4 * dx ** 7 * dy ** 3 * dz ** 6 + 0.144E3 * cc ** 3 * 
     #dt ** 2 * dx ** 7 * dz ** 5 * dy ** 6 + 0.48E2 * cc ** 3 * dt ** 2
     # * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.
     #17280E5
      cvu(401) = (0.12E2 * cc ** 7 * dt ** 6 * dx ** 7 * dz ** 7 - 0.120
     #E3 * cc ** 5 * dt ** 4 * dx ** 7 * dz ** 7 * dy ** 2 + 0.252E3 * c
     #c ** 3 * dt ** 2 * dx ** 7 * dz ** 7 * dy ** 4) / dx ** 7 / dy ** 
     #7 / dz ** 7 / 0.17280E5
      cvu(419) = (0.12E2 * cc ** 7 * dt ** 6 * dy * dz ** 7 * dx ** 6 + 
     #0.36E2 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 7 * dy ** 2 - 0.60E2 
     #* cc ** 5 * dt ** 4 * dy ** 3 * dz ** 7 * dx ** 6 - 0.180E3 * cc *
     #* 5 * dt ** 4 * dx ** 5 * dz ** 7 * dy ** 4 + 0.48E2 * cc ** 3 * d
     #t ** 2 * dy ** 5 * dz ** 7 * dx ** 6 + 0.144E3 * cc ** 3 * dt ** 2
     # * dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.
     #17280E5
      cvu(427) = (0.36E2 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 5 * dx **
     # 6 + 0.72E2 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 4 + 0.
     #36E2 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 3 * dz ** 6 - 0.60E2 * 
     #cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx ** 6 - 0.120E3 * cc ** 
     #5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 - 0.60E2 * cc ** 5 * dt 
     #** 4 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 
     #/ 0.17280E5
      cvu(428) = (-0.72E2 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 5 * dx *
     #* 6 - 0.72E2 * cc ** 7 * dt ** 6 * dy * dz ** 7 * dx ** 6 - 0.144E
     #3 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 4 - 0.72E2 * cc 
     #** 7 * dt ** 6 * dx ** 5 * dy ** 3 * dz ** 6 - 0.216E3 * cc ** 7 *
     # dt ** 6 * dx ** 5 * dz ** 7 * dy ** 2 - 0.144E3 * cc ** 7 * dt **
     # 6 * dy ** 3 * dz ** 7 * dx ** 4 - 0.144E3 * cc ** 7 * dt ** 6 * d
     #x ** 3 * dz ** 7 * dy ** 4 + 0.432E3 * cc ** 6 * dt ** 5 * dy ** 3
     # * dz ** 7 * dx ** 5 + 0.120E3 * cc ** 5 * dt ** 4 * dy ** 5 * dz 
     #** 5 * dx ** 6 + 0.600E3 * cc ** 5 * dt ** 4 * dy ** 3 * dz ** 7 *
     # dx ** 6 + 0.240E3 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy **
     # 6 + 0.120E3 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6 + 0
     #.1320E4 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 7 * dy ** 4 + 0.240E
     #3 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 4 + 0.240E3 * cc
     # ** 5 * dt ** 4 * dx ** 3 * dz ** 7 * dy ** 6 - 0.480E3 * cc ** 4 
     #* dt ** 3 * dy ** 5 * dz ** 7 * dx ** 5 - 0.528E3 * cc ** 3 * dt *
     #* 2 * dy ** 5 * dz ** 7 * dx ** 6 - 0.1104E4 * cc ** 3 * dt ** 2 *
     # dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17
     #280E5
      cvu(429) = (0.36E2 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 5 * dx **
     # 6 + 0.72E2 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 4 + 0.
     #36E2 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 3 * dz ** 6 - 0.60E2 * 
     #cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx ** 6 - 0.120E3 * cc ** 
     #5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 - 0.60E2 * cc ** 5 * dt 
     #** 4 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 
     #/ 0.17280E5
      cvu(435) = (0.36E2 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 3 * dx **
     # 6 + 0.36E2 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 3 * dy ** 6 + 0.
     #72E2 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 4 - 0.60E2 * 
     #cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx ** 6 - 0.60E2 * cc ** 5
     # * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 - 0.120E3 * cc ** 5 * dt 
     #** 4 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 
     #/ 0.17280E5
      cvu(436) = (-0.144E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 3 * dx 
     #** 6 - 0.144E3 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 5 * dx ** 6 -
     # 0.144E3 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 3 * dy ** 6 - 0.288
     #E3 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 4 - 0.288E3 * c
     #c ** 7 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 4 - 0.144E3 * cc ** 7
     # * dt ** 6 * dx ** 5 * dy ** 3 * dz ** 6 - 0.288E3 * cc ** 7 * dt 
     #** 6 * dy ** 5 * dz ** 5 * dx ** 4 - 0.144E3 * cc ** 7 * dt ** 6 *
     # dx ** 3 * dz ** 5 * dy ** 6 - 0.144E3 * cc ** 7 * dt ** 6 * dx **
     # 3 * dy ** 5 * dz ** 6 + 0.864E3 * cc ** 6 * dt ** 5 * dy ** 5 * d
     #z ** 5 * dx ** 5 + 0.960E3 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5
     # * dx ** 6 + 0.960E3 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy 
     #** 6 + 0.960E3 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6) 
     #/ dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvu(437) = (0.216E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 3 * dx *
     #* 6 + 0.288E3 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 5 * dx ** 6 + 
     #0.180E3 * cc ** 7 * dt ** 6 * dy * dz ** 7 * dx ** 6 + 0.216E3 * c
     #c ** 7 * dt ** 6 * dx ** 5 * dz ** 3 * dy ** 6 + 0.432E3 * cc ** 7
     # * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 4 + 0.576E3 * cc ** 7 * dt 
     #** 6 * dx ** 5 * dz ** 5 * dy ** 4 + 0.288E3 * cc ** 7 * dt ** 6 *
     # dx ** 5 * dy ** 3 * dz ** 6 + 0.540E3 * cc ** 7 * dt ** 6 * dx **
     # 5 * dz ** 7 * dy ** 2 + 0.576E3 * cc ** 7 * dt ** 6 * dy ** 5 * d
     #z ** 5 * dx ** 4 + 0.576E3 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 7
     # * dx ** 4 + 0.288E3 * cc ** 7 * dt ** 6 * dx ** 3 * dz ** 5 * dy 
     #** 6 + 0.288E3 * cc ** 7 * dt ** 6 * dx ** 3 * dy ** 5 * dz ** 6 +
     # 0.576E3 * cc ** 7 * dt ** 6 * dx ** 3 * dz ** 7 * dy ** 4 + 0.540
     #E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 7 * dx ** 2 + 0.180E3 * c
     #c ** 7 * dt ** 6 * dx * dz ** 7 * dy ** 6 - 0.1728E4 * cc ** 6 * d
     #t ** 5 * dy ** 5 * dz ** 5 * dx ** 5 - 0.1728E4 * cc ** 6 * dt ** 
     #5 * dy ** 3 * dz ** 7 * dx ** 5 - 0.1728E4 * cc ** 6 * dt ** 5 * d
     #y ** 5 * dz ** 7 * dx ** 3 - 0.1800E4 * cc ** 5 * dt ** 4 * dy ** 
     #5 * dz ** 5 * dx ** 6 - 0.1860E4 * cc ** 5 * dt ** 4 * dy ** 3 * d
     #z ** 7 * dx ** 6 - 0.1800E4 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 
     #5 * dy ** 6 - 0.1680E4 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * d
     #z ** 6 - 0.3660E4 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 7 * dy ** 
     #4 - 0.3660E4 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 4 - 0
     #.1860E4 * cc ** 5 * dt ** 4 * dx ** 3 * dz ** 7 * dy ** 6 + 0.9600
     #E4 * cc ** 4 * dt ** 3 * dy ** 5 * dz ** 7 * dx ** 5 + 0.3840E4 * 
     #cc ** 3 * dt ** 2 * dy ** 5 * dz ** 7 * dx ** 6 + 0.3840E4 * cc **
     # 3 * dt ** 2 * dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / 
     #dz ** 7 / 0.17280E5
      cvu(438) = (-0.144E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 3 * dx 
     #** 6 - 0.144E3 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 5 * dx ** 6 -
     # 0.144E3 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 3 * dy ** 6 - 0.288
     #E3 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 4 - 0.288E3 * c
     #c ** 7 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 4 - 0.144E3 * cc ** 7
     # * dt ** 6 * dx ** 5 * dy ** 3 * dz ** 6 - 0.288E3 * cc ** 7 * dt 
     #** 6 * dy ** 5 * dz ** 5 * dx ** 4 - 0.144E3 * cc ** 7 * dt ** 6 *
     # dx ** 3 * dz ** 5 * dy ** 6 - 0.144E3 * cc ** 7 * dt ** 6 * dx **
     # 3 * dy ** 5 * dz ** 6 + 0.864E3 * cc ** 6 * dt ** 5 * dy ** 5 * d
     #z ** 5 * dx ** 5 + 0.960E3 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5
     # * dx ** 6 + 0.960E3 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy 
     #** 6 + 0.960E3 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6) 
     #/ dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvu(439) = (0.36E2 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 3 * dx **
     # 6 + 0.36E2 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 3 * dy ** 6 + 0.
     #72E2 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 4 - 0.60E2 * 
     #cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx ** 6 - 0.60E2 * cc ** 5
     # * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 - 0.120E3 * cc ** 5 * dt 
     #** 4 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 
     #/ 0.17280E5
      cvu(443) = (0.12E2 * cc ** 7 * dt ** 6 * dy ** 7 * dz * dx ** 6 + 
     #0.36E2 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 7 * dz ** 2 - 0.60E2 
     #* cc ** 5 * dt ** 4 * dy ** 7 * dz ** 3 * dx ** 6 - 0.180E3 * cc *
     #* 5 * dt ** 4 * dx ** 5 * dy ** 7 * dz ** 4 + 0.48E2 * cc ** 3 * d
     #t ** 2 * dy ** 7 * dz ** 5 * dx ** 6 + 0.144E3 * cc ** 3 * dt ** 2
     # * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.
     #17280E5
      cvu(444) = (-0.72E2 * cc ** 7 * dt ** 6 * dy ** 7 * dz * dx ** 6 -
     # 0.72E2 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 3 * dx ** 6 - 0.216E
     #3 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 7 * dz ** 2 - 0.72E2 * cc 
     #** 7 * dt ** 6 * dx ** 5 * dz ** 3 * dy ** 6 - 0.144E3 * cc ** 7 *
     # dt ** 6 * dx ** 5 * dy ** 5 * dz ** 4 - 0.144E3 * cc ** 7 * dt **
     # 6 * dy ** 7 * dz ** 3 * dx ** 4 - 0.144E3 * cc ** 7 * dt ** 6 * d
     #x ** 3 * dy ** 7 * dz ** 4 + 0.432E3 * cc ** 6 * dt ** 5 * dy ** 7
     # * dz ** 3 * dx ** 5 + 0.600E3 * cc ** 5 * dt ** 4 * dy ** 7 * dz 
     #** 3 * dx ** 6 + 0.120E3 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 *
     # dx ** 6 + 0.1320E4 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 7 * dz *
     #* 4 + 0.120E3 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 + 
     #0.240E3 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6 + 0.240E
     #3 * cc ** 5 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 4 + 0.240E3 * cc
     # ** 5 * dt ** 4 * dx ** 3 * dy ** 7 * dz ** 6 - 0.480E3 * cc ** 4 
     #* dt ** 3 * dy ** 7 * dz ** 5 * dx ** 5 - 0.528E3 * cc ** 3 * dt *
     #* 2 * dy ** 7 * dz ** 5 * dx ** 6 - 0.1104E4 * cc ** 3 * dt ** 2 *
     # dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17
     #280E5
      cvu(445) = (0.180E3 * cc ** 7 * dt ** 6 * dy ** 7 * dz * dx ** 6 +
     # 0.288E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 3 * dx ** 6 + 0.216
     #E3 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 5 * dx ** 6 + 0.540E3 * c
     #c ** 7 * dt ** 6 * dx ** 5 * dy ** 7 * dz ** 2 + 0.288E3 * cc ** 7
     # * dt ** 6 * dx ** 5 * dz ** 3 * dy ** 6 + 0.576E3 * cc ** 7 * dt 
     #** 6 * dx ** 5 * dy ** 5 * dz ** 4 + 0.432E3 * cc ** 7 * dt ** 6 *
     # dx ** 5 * dz ** 5 * dy ** 4 + 0.216E3 * cc ** 7 * dt ** 6 * dx **
     # 5 * dy ** 3 * dz ** 6 + 0.576E3 * cc ** 7 * dt ** 6 * dy ** 7 * d
     #z ** 3 * dx ** 4 + 0.576E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 5
     # * dx ** 4 + 0.576E3 * cc ** 7 * dt ** 6 * dx ** 3 * dy ** 7 * dz 
     #** 4 + 0.288E3 * cc ** 7 * dt ** 6 * dx ** 3 * dz ** 5 * dy ** 6 +
     # 0.288E3 * cc ** 7 * dt ** 6 * dx ** 3 * dy ** 5 * dz ** 6 + 0.540
     #E3 * cc ** 7 * dt ** 6 * dy ** 7 * dz ** 5 * dx ** 2 + 0.180E3 * c
     #c ** 7 * dt ** 6 * dx * dy ** 7 * dz ** 6 - 0.1728E4 * cc ** 6 * d
     #t ** 5 * dy ** 7 * dz ** 3 * dx ** 5 - 0.1728E4 * cc ** 6 * dt ** 
     #5 * dy ** 5 * dz ** 5 * dx ** 5 - 0.1728E4 * cc ** 6 * dt ** 5 * d
     #y ** 7 * dz ** 5 * dx ** 3 - 0.1860E4 * cc ** 5 * dt ** 4 * dy ** 
     #7 * dz ** 3 * dx ** 6 - 0.1800E4 * cc ** 5 * dt ** 4 * dy ** 5 * d
     #z ** 5 * dx ** 6 - 0.3660E4 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 
     #7 * dz ** 4 - 0.1680E4 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * d
     #y ** 6 - 0.1800E4 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 
     #6 - 0.3660E4 * cc ** 5 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 4 - 0
     #.1860E4 * cc ** 5 * dt ** 4 * dx ** 3 * dy ** 7 * dz ** 6 + 0.9600
     #E4 * cc ** 4 * dt ** 3 * dy ** 7 * dz ** 5 * dx ** 5 + 0.3840E4 * 
     #cc ** 3 * dt ** 2 * dy ** 7 * dz ** 5 * dx ** 6 + 0.3840E4 * cc **
     # 3 * dt ** 2 * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / 
     #dz ** 7 / 0.17280E5
      cvu(446) = (-0.240E3 * cc ** 7 * dt ** 6 * dy ** 7 * dz * dx ** 6 
     #- 0.432E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 3 * dx ** 6 - 0.43
     #2E3 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 5 * dx ** 6 - 0.240E3 * 
     #cc ** 7 * dt ** 6 * dy * dz ** 7 * dx ** 6 - 0.720E3 * cc ** 7 * d
     #t ** 6 * dx ** 5 * dy ** 7 * dz ** 2 - 0.432E3 * cc ** 7 * dt ** 6
     # * dx ** 5 * dz ** 3 * dy ** 6 - 0.864E3 * cc ** 7 * dt ** 6 * dx 
     #** 5 * dy ** 5 * dz ** 4 - 0.864E3 * cc ** 7 * dt ** 6 * dx ** 5 *
     # dz ** 5 * dy ** 4 - 0.432E3 * cc ** 7 * dt ** 6 * dx ** 5 * dy **
     # 3 * dz ** 6 - 0.720E3 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 7 * d
     #y ** 2 - 0.864E3 * cc ** 7 * dt ** 6 * dy ** 7 * dz ** 3 * dx ** 4
     # - 0.1152E4 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 4 - 0.
     #864E3 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 7 * dx ** 4 - 0.864E3 
     #* cc ** 7 * dt ** 6 * dx ** 3 * dy ** 7 * dz ** 4 - 0.576E3 * cc *
     #* 7 * dt ** 6 * dx ** 3 * dz ** 5 * dy ** 6 - 0.576E3 * cc ** 7 * 
     #dt ** 6 * dx ** 3 * dy ** 5 * dz ** 6 - 0.864E3 * cc ** 7 * dt ** 
     #6 * dx ** 3 * dz ** 7 * dy ** 4 - 0.1080E4 * cc ** 7 * dt ** 6 * d
     #y ** 7 * dz ** 5 * dx ** 2 - 0.1080E4 * cc ** 7 * dt ** 6 * dy ** 
     #5 * dz ** 7 * dx ** 2 - 0.360E3 * cc ** 7 * dt ** 6 * dx * dy ** 7
     # * dz ** 6 - 0.360E3 * cc ** 7 * dt ** 6 * dx * dz ** 7 * dy ** 6 
     #- 0.672E3 * cc ** 7 * dt ** 6 * dy ** 7 * dz ** 7 + 0.2592E4 * cc 
     #** 6 * dt ** 5 * dy ** 7 * dz ** 3 * dx ** 5 + 0.3456E4 * cc ** 6 
     #* dt ** 5 * dy ** 5 * dz ** 5 * dx ** 5 + 0.2592E4 * cc ** 6 * dt 
     #** 5 * dy ** 3 * dz ** 7 * dx ** 5 + 0.3456E4 * cc ** 6 * dt ** 5 
     #* dy ** 7 * dz ** 5 * dx ** 3 + 0.3456E4 * cc ** 6 * dt ** 5 * dy 
     #** 5 * dz ** 7 * dx ** 3 + 0.2160E4 * cc ** 6 * dt ** 5 * dy ** 7 
     #* dz ** 7 * dx + 0.2640E4 * cc ** 5 * dt ** 4 * dy ** 7 * dz ** 3 
     #* dx ** 6 + 0.3360E4 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx 
     #** 6 + 0.2640E4 * cc ** 5 * dt ** 4 * dy ** 3 * dz ** 7 * dx ** 6 
     #+ 0.5040E4 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 7 * dz ** 4 + 0.3
     #120E4 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 + 0.3120E4
     # * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6 + 0.5040E4 * cc
     # ** 5 * dt ** 4 * dx ** 5 * dz ** 7 * dy ** 4 + 0.6840E4 * cc ** 5
     # * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 4 + 0.6840E4 * cc ** 5 * dt
     # ** 4 * dy ** 5 * dz ** 7 * dx ** 4 + 0.3240E4 * cc ** 5 * dt ** 4
     # * dx ** 3 * dy ** 7 * dz ** 6 + 0.3240E4 * cc ** 5 * dt ** 4 * dx
     # ** 3 * dz ** 7 * dy ** 6 + 0.6720E4 * cc ** 5 * dt ** 4 * dy ** 7
     # * dz ** 7 * dx ** 2 - 0.18240E5 * cc ** 4 * dt ** 3 * dy ** 7 * d
     #z ** 5 * dx ** 5 - 0.18240E5 * cc ** 4 * dt ** 3 * dy ** 5 * dz **
     # 7 * dx ** 5 - 0.18720E5 * cc ** 4 * dt ** 3 * dy ** 7 * dz ** 7 *
     # dx ** 3 - 0.6720E4 * cc ** 3 * dt ** 2 * dy ** 7 * dz ** 5 * dx *
     #* 6 - 0.6720E4 * cc ** 3 * dt ** 2 * dy ** 5 * dz ** 7 * dx ** 6 -
     # 0.5760E4 * cc ** 3 * dt ** 2 * dx ** 5 * dy ** 7 * dz ** 6 - 0.57
     #60E4 * cc ** 3 * dt ** 2 * dx ** 5 * dz ** 7 * dy ** 6 - 0.14112E5
     # * cc ** 3 * dt ** 2 * dy ** 7 * dz ** 7 * dx ** 4 + 0.25920E5 * c
     #c ** 2 * dt * dy ** 7 * dz ** 7 * dx ** 5) / dx ** 7 / dy ** 7 / d
     #z ** 7 / 0.17280E5
      cvu(447) = (0.180E3 * cc ** 7 * dt ** 6 * dy ** 7 * dz * dx ** 6 +
     # 0.288E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 3 * dx ** 6 + 0.216
     #E3 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 5 * dx ** 6 + 0.540E3 * c
     #c ** 7 * dt ** 6 * dx ** 5 * dy ** 7 * dz ** 2 + 0.288E3 * cc ** 7
     # * dt ** 6 * dx ** 5 * dz ** 3 * dy ** 6 + 0.576E3 * cc ** 7 * dt 
     #** 6 * dx ** 5 * dy ** 5 * dz ** 4 + 0.432E3 * cc ** 7 * dt ** 6 *
     # dx ** 5 * dz ** 5 * dy ** 4 + 0.216E3 * cc ** 7 * dt ** 6 * dx **
     # 5 * dy ** 3 * dz ** 6 + 0.576E3 * cc ** 7 * dt ** 6 * dy ** 7 * d
     #z ** 3 * dx ** 4 + 0.576E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 5
     # * dx ** 4 + 0.576E3 * cc ** 7 * dt ** 6 * dx ** 3 * dy ** 7 * dz 
     #** 4 + 0.288E3 * cc ** 7 * dt ** 6 * dx ** 3 * dz ** 5 * dy ** 6 +
     # 0.288E3 * cc ** 7 * dt ** 6 * dx ** 3 * dy ** 5 * dz ** 6 + 0.540
     #E3 * cc ** 7 * dt ** 6 * dy ** 7 * dz ** 5 * dx ** 2 + 0.180E3 * c
     #c ** 7 * dt ** 6 * dx * dy ** 7 * dz ** 6 - 0.1728E4 * cc ** 6 * d
     #t ** 5 * dy ** 7 * dz ** 3 * dx ** 5 - 0.1728E4 * cc ** 6 * dt ** 
     #5 * dy ** 5 * dz ** 5 * dx ** 5 - 0.1728E4 * cc ** 6 * dt ** 5 * d
     #y ** 7 * dz ** 5 * dx ** 3 - 0.1860E4 * cc ** 5 * dt ** 4 * dy ** 
     #7 * dz ** 3 * dx ** 6 - 0.1800E4 * cc ** 5 * dt ** 4 * dy ** 5 * d
     #z ** 5 * dx ** 6 - 0.3660E4 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 
     #7 * dz ** 4 - 0.1680E4 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * d
     #y ** 6 - 0.1800E4 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 
     #6 - 0.3660E4 * cc ** 5 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 4 - 0
     #.1860E4 * cc ** 5 * dt ** 4 * dx ** 3 * dy ** 7 * dz ** 6 + 0.9600
     #E4 * cc ** 4 * dt ** 3 * dy ** 7 * dz ** 5 * dx ** 5 + 0.3840E4 * 
     #cc ** 3 * dt ** 2 * dy ** 7 * dz ** 5 * dx ** 6 + 0.3840E4 * cc **
     # 3 * dt ** 2 * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / 
     #dz ** 7 / 0.17280E5
      cvu(448) = (-0.72E2 * cc ** 7 * dt ** 6 * dy ** 7 * dz * dx ** 6 -
     # 0.72E2 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 3 * dx ** 6 - 0.216E
     #3 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 7 * dz ** 2 - 0.72E2 * cc 
     #** 7 * dt ** 6 * dx ** 5 * dz ** 3 * dy ** 6 - 0.144E3 * cc ** 7 *
     # dt ** 6 * dx ** 5 * dy ** 5 * dz ** 4 - 0.144E3 * cc ** 7 * dt **
     # 6 * dy ** 7 * dz ** 3 * dx ** 4 - 0.144E3 * cc ** 7 * dt ** 6 * d
     #x ** 3 * dy ** 7 * dz ** 4 + 0.432E3 * cc ** 6 * dt ** 5 * dy ** 7
     # * dz ** 3 * dx ** 5 + 0.600E3 * cc ** 5 * dt ** 4 * dy ** 7 * dz 
     #** 3 * dx ** 6 + 0.120E3 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 *
     # dx ** 6 + 0.1320E4 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 7 * dz *
     #* 4 + 0.120E3 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 + 
     #0.240E3 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6 + 0.240E
     #3 * cc ** 5 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 4 + 0.240E3 * cc
     # ** 5 * dt ** 4 * dx ** 3 * dy ** 7 * dz ** 6 - 0.480E3 * cc ** 4 
     #* dt ** 3 * dy ** 7 * dz ** 5 * dx ** 5 - 0.528E3 * cc ** 3 * dt *
     #* 2 * dy ** 7 * dz ** 5 * dx ** 6 - 0.1104E4 * cc ** 3 * dt ** 2 *
     # dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17
     #280E5
      cvu(449) = (0.12E2 * cc ** 7 * dt ** 6 * dy ** 7 * dz * dx ** 6 + 
     #0.36E2 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 7 * dz ** 2 - 0.60E2 
     #* cc ** 5 * dt ** 4 * dy ** 7 * dz ** 3 * dx ** 6 - 0.180E3 * cc *
     #* 5 * dt ** 4 * dx ** 5 * dy ** 7 * dz ** 4 + 0.48E2 * cc ** 3 * d
     #t ** 2 * dy ** 7 * dz ** 5 * dx ** 6 + 0.144E3 * cc ** 3 * dt ** 2
     # * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.
     #17280E5
      cvu(453) = (0.36E2 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 3 * dx **
     # 6 + 0.36E2 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 3 * dy ** 6 + 0.
     #72E2 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 4 - 0.60E2 * 
     #cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx ** 6 - 0.60E2 * cc ** 5
     # * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 - 0.120E3 * cc ** 5 * dt 
     #** 4 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 
     #/ 0.17280E5
      cvu(454) = (-0.144E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 3 * dx 
     #** 6 - 0.144E3 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 5 * dx ** 6 -
     # 0.144E3 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 3 * dy ** 6 - 0.288
     #E3 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 4 - 0.288E3 * c
     #c ** 7 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 4 - 0.144E3 * cc ** 7
     # * dt ** 6 * dx ** 5 * dy ** 3 * dz ** 6 - 0.288E3 * cc ** 7 * dt 
     #** 6 * dy ** 5 * dz ** 5 * dx ** 4 - 0.144E3 * cc ** 7 * dt ** 6 *
     # dx ** 3 * dz ** 5 * dy ** 6 - 0.144E3 * cc ** 7 * dt ** 6 * dx **
     # 3 * dy ** 5 * dz ** 6 + 0.864E3 * cc ** 6 * dt ** 5 * dy ** 5 * d
     #z ** 5 * dx ** 5 + 0.960E3 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5
     # * dx ** 6 + 0.960E3 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy 
     #** 6 + 0.960E3 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6) 
     #/ dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvu(455) = (0.216E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 3 * dx *
     #* 6 + 0.288E3 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 5 * dx ** 6 + 
     #0.180E3 * cc ** 7 * dt ** 6 * dy * dz ** 7 * dx ** 6 + 0.216E3 * c
     #c ** 7 * dt ** 6 * dx ** 5 * dz ** 3 * dy ** 6 + 0.432E3 * cc ** 7
     # * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 4 + 0.576E3 * cc ** 7 * dt 
     #** 6 * dx ** 5 * dz ** 5 * dy ** 4 + 0.288E3 * cc ** 7 * dt ** 6 *
     # dx ** 5 * dy ** 3 * dz ** 6 + 0.540E3 * cc ** 7 * dt ** 6 * dx **
     # 5 * dz ** 7 * dy ** 2 + 0.576E3 * cc ** 7 * dt ** 6 * dy ** 5 * d
     #z ** 5 * dx ** 4 + 0.576E3 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 7
     # * dx ** 4 + 0.288E3 * cc ** 7 * dt ** 6 * dx ** 3 * dz ** 5 * dy 
     #** 6 + 0.288E3 * cc ** 7 * dt ** 6 * dx ** 3 * dy ** 5 * dz ** 6 +
     # 0.576E3 * cc ** 7 * dt ** 6 * dx ** 3 * dz ** 7 * dy ** 4 + 0.540
     #E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 7 * dx ** 2 + 0.180E3 * c
     #c ** 7 * dt ** 6 * dx * dz ** 7 * dy ** 6 - 0.1728E4 * cc ** 6 * d
     #t ** 5 * dy ** 5 * dz ** 5 * dx ** 5 - 0.1728E4 * cc ** 6 * dt ** 
     #5 * dy ** 3 * dz ** 7 * dx ** 5 - 0.1728E4 * cc ** 6 * dt ** 5 * d
     #y ** 5 * dz ** 7 * dx ** 3 - 0.1800E4 * cc ** 5 * dt ** 4 * dy ** 
     #5 * dz ** 5 * dx ** 6 - 0.1860E4 * cc ** 5 * dt ** 4 * dy ** 3 * d
     #z ** 7 * dx ** 6 - 0.1800E4 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 
     #5 * dy ** 6 - 0.1680E4 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * d
     #z ** 6 - 0.3660E4 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 7 * dy ** 
     #4 - 0.3660E4 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 4 - 0
     #.1860E4 * cc ** 5 * dt ** 4 * dx ** 3 * dz ** 7 * dy ** 6 + 0.9600
     #E4 * cc ** 4 * dt ** 3 * dy ** 5 * dz ** 7 * dx ** 5 + 0.3840E4 * 
     #cc ** 3 * dt ** 2 * dy ** 5 * dz ** 7 * dx ** 6 + 0.3840E4 * cc **
     # 3 * dt ** 2 * dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / 
     #dz ** 7 / 0.17280E5
      cvu(456) = (-0.144E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 3 * dx 
     #** 6 - 0.144E3 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 5 * dx ** 6 -
     # 0.144E3 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 3 * dy ** 6 - 0.288
     #E3 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 4 - 0.288E3 * c
     #c ** 7 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 4 - 0.144E3 * cc ** 7
     # * dt ** 6 * dx ** 5 * dy ** 3 * dz ** 6 - 0.288E3 * cc ** 7 * dt 
     #** 6 * dy ** 5 * dz ** 5 * dx ** 4 - 0.144E3 * cc ** 7 * dt ** 6 *
     # dx ** 3 * dz ** 5 * dy ** 6 - 0.144E3 * cc ** 7 * dt ** 6 * dx **
     # 3 * dy ** 5 * dz ** 6 + 0.864E3 * cc ** 6 * dt ** 5 * dy ** 5 * d
     #z ** 5 * dx ** 5 + 0.960E3 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5
     # * dx ** 6 + 0.960E3 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy 
     #** 6 + 0.960E3 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6) 
     #/ dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvu(457) = (0.36E2 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 3 * dx **
     # 6 + 0.36E2 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 3 * dy ** 6 + 0.
     #72E2 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 4 - 0.60E2 * 
     #cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx ** 6 - 0.60E2 * cc ** 5
     # * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 - 0.120E3 * cc ** 5 * dt 
     #** 4 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 
     #/ 0.17280E5
      cvu(463) = (0.36E2 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 5 * dx **
     # 6 + 0.72E2 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 4 + 0.
     #36E2 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 3 * dz ** 6 - 0.60E2 * 
     #cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx ** 6 - 0.120E3 * cc ** 
     #5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 - 0.60E2 * cc ** 5 * dt 
     #** 4 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 
     #/ 0.17280E5
      cvu(464) = (-0.72E2 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 5 * dx *
     #* 6 - 0.72E2 * cc ** 7 * dt ** 6 * dy * dz ** 7 * dx ** 6 - 0.144E
     #3 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 4 - 0.72E2 * cc 
     #** 7 * dt ** 6 * dx ** 5 * dy ** 3 * dz ** 6 - 0.216E3 * cc ** 7 *
     # dt ** 6 * dx ** 5 * dz ** 7 * dy ** 2 - 0.144E3 * cc ** 7 * dt **
     # 6 * dy ** 3 * dz ** 7 * dx ** 4 - 0.144E3 * cc ** 7 * dt ** 6 * d
     #x ** 3 * dz ** 7 * dy ** 4 + 0.432E3 * cc ** 6 * dt ** 5 * dy ** 3
     # * dz ** 7 * dx ** 5 + 0.120E3 * cc ** 5 * dt ** 4 * dy ** 5 * dz 
     #** 5 * dx ** 6 + 0.600E3 * cc ** 5 * dt ** 4 * dy ** 3 * dz ** 7 *
     # dx ** 6 + 0.240E3 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy **
     # 6 + 0.120E3 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6 + 0
     #.1320E4 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 7 * dy ** 4 + 0.240E
     #3 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 4 + 0.240E3 * cc
     # ** 5 * dt ** 4 * dx ** 3 * dz ** 7 * dy ** 6 - 0.480E3 * cc ** 4 
     #* dt ** 3 * dy ** 5 * dz ** 7 * dx ** 5 - 0.528E3 * cc ** 3 * dt *
     #* 2 * dy ** 5 * dz ** 7 * dx ** 6 - 0.1104E4 * cc ** 3 * dt ** 2 *
     # dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17
     #280E5
      cvu(465) = (0.36E2 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 5 * dx **
     # 6 + 0.72E2 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 5 * dy ** 4 + 0.
     #36E2 * cc ** 7 * dt ** 6 * dx ** 5 * dy ** 3 * dz ** 6 - 0.60E2 * 
     #cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx ** 6 - 0.120E3 * cc ** 
     #5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 - 0.60E2 * cc ** 5 * dt 
     #** 4 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 
     #/ 0.17280E5
      cvu(473) = (0.12E2 * cc ** 7 * dt ** 6 * dy * dz ** 7 * dx ** 6 + 
     #0.36E2 * cc ** 7 * dt ** 6 * dx ** 5 * dz ** 7 * dy ** 2 - 0.60E2 
     #* cc ** 5 * dt ** 4 * dy ** 3 * dz ** 7 * dx ** 6 - 0.180E3 * cc *
     #* 5 * dt ** 4 * dx ** 5 * dz ** 7 * dy ** 4 + 0.48E2 * cc ** 3 * d
     #t ** 2 * dy ** 5 * dz ** 7 * dx ** 6 + 0.144E3 * cc ** 3 * dt ** 2
     # * dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.
     #17280E5
      cvu(509) = (0.36E2 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 7 * dx **
     # 4 + 0.36E2 * cc ** 7 * dt ** 6 * dx ** 3 * dz ** 7 * dy ** 4 - 0.
     #60E2 * cc ** 5 * dt ** 4 * dy ** 3 * dz ** 7 * dx ** 6 - 0.60E2 * 
     #cc ** 5 * dt ** 4 * dx ** 5 * dz ** 7 * dy ** 4 - 0.60E2 * cc ** 5
     # * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 4 - 0.60E2 * cc ** 5 * dt *
     #* 4 * dx ** 3 * dz ** 7 * dy ** 6 + 0.60E2 * cc ** 3 * dt ** 2 * d
     #y ** 5 * dz ** 7 * dx ** 6 + 0.60E2 * cc ** 3 * dt ** 2 * dx ** 5 
     #* dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvu(517) = (0.72E2 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 5 * dx **
     # 4 + 0.36E2 * cc ** 7 * dt ** 6 * dx ** 3 * dz ** 5 * dy ** 6 + 0.
     #36E2 * cc ** 7 * dt ** 6 * dx ** 3 * dy ** 5 * dz ** 6 - 0.120E3 *
     # cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx ** 6 - 0.60E2 * cc ** 
     #5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 - 0.60E2 * cc ** 5 * dt 
     #** 4 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 
     #/ 0.17280E5
      cvu(518) = (-0.144E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 5 * dx 
     #** 4 - 0.144E3 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 7 * dx ** 4 -
     # 0.72E2 * cc ** 7 * dt ** 6 * dx ** 3 * dz ** 5 * dy ** 6 - 0.72E2
     # * cc ** 7 * dt ** 6 * dx ** 3 * dy ** 5 * dz ** 6 - 0.144E3 * cc 
     #** 7 * dt ** 6 * dx ** 3 * dz ** 7 * dy ** 4 - 0.216E3 * cc ** 7 *
     # dt ** 6 * dy ** 5 * dz ** 7 * dx ** 2 - 0.72E2 * cc ** 7 * dt ** 
     #6 * dx * dz ** 7 * dy ** 6 + 0.432E3 * cc ** 6 * dt ** 5 * dy ** 5
     # * dz ** 7 * dx ** 3 + 0.240E3 * cc ** 5 * dt ** 4 * dy ** 5 * dz 
     #** 5 * dx ** 6 + 0.240E3 * cc ** 5 * dt ** 4 * dy ** 3 * dz ** 7 *
     # dx ** 6 + 0.120E3 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy **
     # 6 + 0.120E3 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6 + 0
     #.240E3 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 7 * dy ** 4 + 0.1320E
     #4 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 4 + 0.600E3 * cc
     # ** 5 * dt ** 4 * dx ** 3 * dz ** 7 * dy ** 6 - 0.480E3 * cc ** 4 
     #* dt ** 3 * dy ** 5 * dz ** 7 * dx ** 5 - 0.1104E4 * cc ** 3 * dt 
     #** 2 * dy ** 5 * dz ** 7 * dx ** 6 - 0.528E3 * cc ** 3 * dt ** 2 *
     # dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17
     #280E5
      cvu(519) = (0.72E2 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 5 * dx **
     # 4 + 0.36E2 * cc ** 7 * dt ** 6 * dx ** 3 * dz ** 5 * dy ** 6 + 0.
     #36E2 * cc ** 7 * dt ** 6 * dx ** 3 * dy ** 5 * dz ** 6 - 0.120E3 *
     # cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx ** 6 - 0.60E2 * cc ** 
     #5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 - 0.60E2 * cc ** 5 * dt 
     #** 4 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 
     #/ 0.17280E5
      cvu(525) = (0.36E2 * cc ** 7 * dt ** 6 * dy ** 7 * dz ** 3 * dx **
     # 4 + 0.36E2 * cc ** 7 * dt ** 6 * dx ** 3 * dy ** 7 * dz ** 4 - 0.
     #60E2 * cc ** 5 * dt ** 4 * dy ** 7 * dz ** 3 * dx ** 6 - 0.60E2 * 
     #cc ** 5 * dt ** 4 * dx ** 5 * dy ** 7 * dz ** 4 - 0.60E2 * cc ** 5
     # * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 4 - 0.60E2 * cc ** 5 * dt *
     #* 4 * dx ** 3 * dy ** 7 * dz ** 6 + 0.60E2 * cc ** 3 * dt ** 2 * d
     #y ** 7 * dz ** 5 * dx ** 6 + 0.60E2 * cc ** 3 * dt ** 2 * dx ** 5 
     #* dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvu(526) = (-0.144E3 * cc ** 7 * dt ** 6 * dy ** 7 * dz ** 3 * dx 
     #** 4 - 0.144E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 4 -
     # 0.144E3 * cc ** 7 * dt ** 6 * dx ** 3 * dy ** 7 * dz ** 4 - 0.72E
     #2 * cc ** 7 * dt ** 6 * dx ** 3 * dz ** 5 * dy ** 6 - 0.72E2 * cc 
     #** 7 * dt ** 6 * dx ** 3 * dy ** 5 * dz ** 6 - 0.216E3 * cc ** 7 *
     # dt ** 6 * dy ** 7 * dz ** 5 * dx ** 2 - 0.72E2 * cc ** 7 * dt ** 
     #6 * dx * dy ** 7 * dz ** 6 + 0.432E3 * cc ** 6 * dt ** 5 * dy ** 7
     # * dz ** 5 * dx ** 3 + 0.240E3 * cc ** 5 * dt ** 4 * dy ** 7 * dz 
     #** 3 * dx ** 6 + 0.240E3 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 *
     # dx ** 6 + 0.240E3 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 7 * dz **
     # 4 + 0.120E3 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 + 0
     #.120E3 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6 + 0.1320E
     #4 * cc ** 5 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 4 + 0.600E3 * cc
     # ** 5 * dt ** 4 * dx ** 3 * dy ** 7 * dz ** 6 - 0.480E3 * cc ** 4 
     #* dt ** 3 * dy ** 7 * dz ** 5 * dx ** 5 - 0.1104E4 * cc ** 3 * dt 
     #** 2 * dy ** 7 * dz ** 5 * dx ** 6 - 0.528E3 * cc ** 3 * dt ** 2 *
     # dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17
     #280E5
      cvu(527) = (0.216E3 * cc ** 7 * dt ** 6 * dy ** 7 * dz ** 3 * dx *
     #* 4 + 0.288E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 4 + 
     #0.216E3 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 7 * dx ** 4 + 0.216E
     #3 * cc ** 7 * dt ** 6 * dx ** 3 * dy ** 7 * dz ** 4 + 0.144E3 * cc
     # ** 7 * dt ** 6 * dx ** 3 * dz ** 5 * dy ** 6 + 0.144E3 * cc ** 7 
     #* dt ** 6 * dx ** 3 * dy ** 5 * dz ** 6 + 0.216E3 * cc ** 7 * dt *
     #* 6 * dx ** 3 * dz ** 7 * dy ** 4 + 0.432E3 * cc ** 7 * dt ** 6 * 
     #dy ** 7 * dz ** 5 * dx ** 2 + 0.432E3 * cc ** 7 * dt ** 6 * dy ** 
     #5 * dz ** 7 * dx ** 2 + 0.144E3 * cc ** 7 * dt ** 6 * dx * dy ** 7
     # * dz ** 6 + 0.144E3 * cc ** 7 * dt ** 6 * dx * dz ** 7 * dy ** 6 
     #+ 0.336E3 * cc ** 7 * dt ** 6 * dy ** 7 * dz ** 7 - 0.864E3 * cc *
     #* 6 * dt ** 5 * dy ** 7 * dz ** 5 * dx ** 3 - 0.864E3 * cc ** 6 * 
     #dt ** 5 * dy ** 5 * dz ** 7 * dx ** 3 - 0.864E3 * cc ** 6 * dt ** 
     #5 * dy ** 7 * dz ** 7 * dx - 0.360E3 * cc ** 5 * dt ** 4 * dy ** 7
     # * dz ** 3 * dx ** 6 - 0.480E3 * cc ** 5 * dt ** 4 * dy ** 5 * dz 
     #** 5 * dx ** 6 - 0.360E3 * cc ** 5 * dt ** 4 * dy ** 3 * dz ** 7 *
     # dx ** 6 - 0.360E3 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 7 * dz **
     # 4 - 0.240E3 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 - 0
     #.240E3 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6 - 0.360E3
     # * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 7 * dy ** 4 - 0.2520E4 * cc
     # ** 5 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 4 - 0.2520E4 * cc ** 5
     # * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 4 - 0.1080E4 * cc ** 5 * dt
     # ** 4 * dx ** 3 * dy ** 7 * dz ** 6 - 0.1080E4 * cc ** 5 * dt ** 4
     # * dx ** 3 * dz ** 7 * dy ** 6 - 0.3360E4 * cc ** 5 * dt ** 4 * dy
     # ** 7 * dz ** 7 * dx ** 2 + 0.960E3 * cc ** 4 * dt ** 3 * dy ** 7 
     #* dz ** 5 * dx ** 5 + 0.960E3 * cc ** 4 * dt ** 3 * dy ** 5 * dz *
     #* 7 * dx ** 5 + 0.5760E4 * cc ** 4 * dt ** 3 * dy ** 7 * dz ** 7 *
     # dx ** 3 + 0.2088E4 * cc ** 3 * dt ** 2 * dy ** 7 * dz ** 5 * dx *
     #* 6 + 0.2088E4 * cc ** 3 * dt ** 2 * dy ** 5 * dz ** 7 * dx ** 6 +
     # 0.936E3 * cc ** 3 * dt ** 2 * dx ** 5 * dy ** 7 * dz ** 6 + 0.936
     #E3 * cc ** 3 * dt ** 2 * dx ** 5 * dz ** 7 * dy ** 6 + 0.7056E4 * 
     #cc ** 3 * dt ** 2 * dy ** 7 * dz ** 7 * dx ** 4 - 0.2592E4 * cc **
     # 2 * dt * dy ** 7 * dz ** 7 * dx ** 5) / dx ** 7 / dy ** 7 / dz **
     # 7 / 0.17280E5
      cvu(528) = (-0.144E3 * cc ** 7 * dt ** 6 * dy ** 7 * dz ** 3 * dx 
     #** 4 - 0.144E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 5 * dx ** 4 -
     # 0.144E3 * cc ** 7 * dt ** 6 * dx ** 3 * dy ** 7 * dz ** 4 - 0.72E
     #2 * cc ** 7 * dt ** 6 * dx ** 3 * dz ** 5 * dy ** 6 - 0.72E2 * cc 
     #** 7 * dt ** 6 * dx ** 3 * dy ** 5 * dz ** 6 - 0.216E3 * cc ** 7 *
     # dt ** 6 * dy ** 7 * dz ** 5 * dx ** 2 - 0.72E2 * cc ** 7 * dt ** 
     #6 * dx * dy ** 7 * dz ** 6 + 0.432E3 * cc ** 6 * dt ** 5 * dy ** 7
     # * dz ** 5 * dx ** 3 + 0.240E3 * cc ** 5 * dt ** 4 * dy ** 7 * dz 
     #** 3 * dx ** 6 + 0.240E3 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 *
     # dx ** 6 + 0.240E3 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 7 * dz **
     # 4 + 0.120E3 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 + 0
     #.120E3 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6 + 0.1320E
     #4 * cc ** 5 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 4 + 0.600E3 * cc
     # ** 5 * dt ** 4 * dx ** 3 * dy ** 7 * dz ** 6 - 0.480E3 * cc ** 4 
     #* dt ** 3 * dy ** 7 * dz ** 5 * dx ** 5 - 0.1104E4 * cc ** 3 * dt 
     #** 2 * dy ** 7 * dz ** 5 * dx ** 6 - 0.528E3 * cc ** 3 * dt ** 2 *
     # dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17
     #280E5
      cvu(529) = (0.36E2 * cc ** 7 * dt ** 6 * dy ** 7 * dz ** 3 * dx **
     # 4 + 0.36E2 * cc ** 7 * dt ** 6 * dx ** 3 * dy ** 7 * dz ** 4 - 0.
     #60E2 * cc ** 5 * dt ** 4 * dy ** 7 * dz ** 3 * dx ** 6 - 0.60E2 * 
     #cc ** 5 * dt ** 4 * dx ** 5 * dy ** 7 * dz ** 4 - 0.60E2 * cc ** 5
     # * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 4 - 0.60E2 * cc ** 5 * dt *
     #* 4 * dx ** 3 * dy ** 7 * dz ** 6 + 0.60E2 * cc ** 3 * dt ** 2 * d
     #y ** 7 * dz ** 5 * dx ** 6 + 0.60E2 * cc ** 3 * dt ** 2 * dx ** 5 
     #* dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvu(535) = (0.72E2 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 5 * dx **
     # 4 + 0.36E2 * cc ** 7 * dt ** 6 * dx ** 3 * dz ** 5 * dy ** 6 + 0.
     #36E2 * cc ** 7 * dt ** 6 * dx ** 3 * dy ** 5 * dz ** 6 - 0.120E3 *
     # cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx ** 6 - 0.60E2 * cc ** 
     #5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 - 0.60E2 * cc ** 5 * dt 
     #** 4 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 
     #/ 0.17280E5
      cvu(536) = (-0.144E3 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 5 * dx 
     #** 4 - 0.144E3 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 7 * dx ** 4 -
     # 0.72E2 * cc ** 7 * dt ** 6 * dx ** 3 * dz ** 5 * dy ** 6 - 0.72E2
     # * cc ** 7 * dt ** 6 * dx ** 3 * dy ** 5 * dz ** 6 - 0.144E3 * cc 
     #** 7 * dt ** 6 * dx ** 3 * dz ** 7 * dy ** 4 - 0.216E3 * cc ** 7 *
     # dt ** 6 * dy ** 5 * dz ** 7 * dx ** 2 - 0.72E2 * cc ** 7 * dt ** 
     #6 * dx * dz ** 7 * dy ** 6 + 0.432E3 * cc ** 6 * dt ** 5 * dy ** 5
     # * dz ** 7 * dx ** 3 + 0.240E3 * cc ** 5 * dt ** 4 * dy ** 5 * dz 
     #** 5 * dx ** 6 + 0.240E3 * cc ** 5 * dt ** 4 * dy ** 3 * dz ** 7 *
     # dx ** 6 + 0.120E3 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 5 * dy **
     # 6 + 0.120E3 * cc ** 5 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 6 + 0
     #.240E3 * cc ** 5 * dt ** 4 * dx ** 5 * dz ** 7 * dy ** 4 + 0.1320E
     #4 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 4 + 0.600E3 * cc
     # ** 5 * dt ** 4 * dx ** 3 * dz ** 7 * dy ** 6 - 0.480E3 * cc ** 4 
     #* dt ** 3 * dy ** 5 * dz ** 7 * dx ** 5 - 0.1104E4 * cc ** 3 * dt 
     #** 2 * dy ** 5 * dz ** 7 * dx ** 6 - 0.528E3 * cc ** 3 * dt ** 2 *
     # dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17
     #280E5
      cvu(537) = (0.72E2 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 5 * dx **
     # 4 + 0.36E2 * cc ** 7 * dt ** 6 * dx ** 3 * dz ** 5 * dy ** 6 + 0.
     #36E2 * cc ** 7 * dt ** 6 * dx ** 3 * dy ** 5 * dz ** 6 - 0.120E3 *
     # cc ** 5 * dt ** 4 * dy ** 5 * dz ** 5 * dx ** 6 - 0.60E2 * cc ** 
     #5 * dt ** 4 * dx ** 5 * dz ** 5 * dy ** 6 - 0.60E2 * cc ** 5 * dt 
     #** 4 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 
     #/ 0.17280E5
      cvu(545) = (0.36E2 * cc ** 7 * dt ** 6 * dy ** 3 * dz ** 7 * dx **
     # 4 + 0.36E2 * cc ** 7 * dt ** 6 * dx ** 3 * dz ** 7 * dy ** 4 - 0.
     #60E2 * cc ** 5 * dt ** 4 * dy ** 3 * dz ** 7 * dx ** 6 - 0.60E2 * 
     #cc ** 5 * dt ** 4 * dx ** 5 * dz ** 7 * dy ** 4 - 0.60E2 * cc ** 5
     # * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 4 - 0.60E2 * cc ** 5 * dt *
     #* 4 * dx ** 3 * dz ** 7 * dy ** 6 + 0.60E2 * cc ** 3 * dt ** 2 * d
     #y ** 5 * dz ** 7 * dx ** 6 + 0.60E2 * cc ** 3 * dt ** 2 * dx ** 5 
     #* dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvu(599) = (0.36E2 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 7 * dx **
     # 2 + 0.12E2 * cc ** 7 * dt ** 6 * dx * dz ** 7 * dy ** 6 - 0.180E3
     # * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 4 - 0.60E2 * cc *
     #* 5 * dt ** 4 * dx ** 3 * dz ** 7 * dy ** 6 + 0.144E3 * cc ** 3 * 
     #dt ** 2 * dy ** 5 * dz ** 7 * dx ** 6 + 0.48E2 * cc ** 3 * dt ** 2
     # * dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.
     #17280E5
      cvu(607) = (0.36E2 * cc ** 7 * dt ** 6 * dy ** 7 * dz ** 5 * dx **
     # 2 + 0.12E2 * cc ** 7 * dt ** 6 * dx * dy ** 7 * dz ** 6 - 0.180E3
     # * cc ** 5 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 4 - 0.60E2 * cc *
     #* 5 * dt ** 4 * dx ** 3 * dy ** 7 * dz ** 6 + 0.144E3 * cc ** 3 * 
     #dt ** 2 * dy ** 7 * dz ** 5 * dx ** 6 + 0.48E2 * cc ** 3 * dt ** 2
     # * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.
     #17280E5
      cvu(608) = (-0.72E2 * cc ** 7 * dt ** 6 * dy ** 7 * dz ** 5 * dx *
     #* 2 - 0.72E2 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 7 * dx ** 2 - 0
     #.24E2 * cc ** 7 * dt ** 6 * dx * dy ** 7 * dz ** 6 - 0.24E2 * cc *
     #* 7 * dt ** 6 * dx * dz ** 7 * dy ** 6 - 0.96E2 * cc ** 7 * dt ** 
     #6 * dy ** 7 * dz ** 7 + 0.144E3 * cc ** 6 * dt ** 5 * dy ** 7 * dz
     # ** 7 * dx + 0.360E3 * cc ** 5 * dt ** 4 * dy ** 7 * dz ** 5 * dx 
     #** 4 + 0.360E3 * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 4 +
     # 0.120E3 * cc ** 5 * dt ** 4 * dx ** 3 * dy ** 7 * dz ** 6 + 0.120
     #E3 * cc ** 5 * dt ** 4 * dx ** 3 * dz ** 7 * dy ** 6 + 0.960E3 * c
     #c ** 5 * dt ** 4 * dy ** 7 * dz ** 7 * dx ** 2 - 0.480E3 * cc ** 4
     # * dt ** 3 * dy ** 7 * dz ** 7 * dx ** 3 - 0.288E3 * cc ** 3 * dt 
     #** 2 * dy ** 7 * dz ** 5 * dx ** 6 - 0.288E3 * cc ** 3 * dt ** 2 *
     # dy ** 5 * dz ** 7 * dx ** 6 - 0.96E2 * cc ** 3 * dt ** 2 * dx ** 
     #5 * dy ** 7 * dz ** 6 - 0.96E2 * cc ** 3 * dt ** 2 * dx ** 5 * dz 
     #** 7 * dy ** 6 - 0.2016E4 * cc ** 3 * dt ** 2 * dy ** 7 * dz ** 7 
     #* dx ** 4 + 0.192E3 * cc ** 2 * dt * dy ** 7 * dz ** 7 * dx ** 5) 
     #/ dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvu(609) = (0.36E2 * cc ** 7 * dt ** 6 * dy ** 7 * dz ** 5 * dx **
     # 2 + 0.12E2 * cc ** 7 * dt ** 6 * dx * dy ** 7 * dz ** 6 - 0.180E3
     # * cc ** 5 * dt ** 4 * dy ** 7 * dz ** 5 * dx ** 4 - 0.60E2 * cc *
     #* 5 * dt ** 4 * dx ** 3 * dy ** 7 * dz ** 6 + 0.144E3 * cc ** 3 * 
     #dt ** 2 * dy ** 7 * dz ** 5 * dx ** 6 + 0.48E2 * cc ** 3 * dt ** 2
     # * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.
     #17280E5
      cvu(617) = (0.36E2 * cc ** 7 * dt ** 6 * dy ** 5 * dz ** 7 * dx **
     # 2 + 0.12E2 * cc ** 7 * dt ** 6 * dx * dz ** 7 * dy ** 6 - 0.180E3
     # * cc ** 5 * dt ** 4 * dy ** 5 * dz ** 7 * dx ** 4 - 0.60E2 * cc *
     #* 5 * dt ** 4 * dx ** 3 * dz ** 7 * dy ** 6 + 0.144E3 * cc ** 3 * 
     #dt ** 2 * dy ** 5 * dz ** 7 * dx ** 6 + 0.48E2 * cc ** 3 * dt ** 2
     # * dx ** 5 * dz ** 7 * dy ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.
     #17280E5
      cvu(689) = (0.12E2 * cc ** 7 * dt ** 6 * dy ** 7 * dz ** 7 - 0.120
     #E3 * cc ** 5 * dt ** 4 * dy ** 7 * dz ** 7 * dx ** 2 + 0.252E3 * c
     #c ** 3 * dt ** 2 * dy ** 7 * dz ** 7 * dx ** 4) / dx ** 7 / dy ** 
     #7 / dz ** 7 / 0.17280E5
c
      return
      end
c
c++++++++++++++++
c
      subroutine getcvv_sixth3D( 
     *   cc,dx,dy,dz,dt,cvv )
c
      implicit real (t)
      real cvv(1:*)
      real dx,dy,dz,dt,cc
      integer i
c
      do i = 1,9**3
        cvv(i) = 0.0
      end do
c
      cvv(41) = (-0.18E2 * cc ** 5 * dt ** 5 * dx ** 2 * dy ** 7 * dz **
     # 7 + 0.106E3 * cc ** 3 * dt ** 3 * dx ** 4 * dy ** 7 * dz ** 7 - 0
     #.64E2 * cc * dt * dx ** 6 * dy ** 7 * dz ** 7) / dx ** 7 / dy ** 7
     # / dz ** 7 / 0.17280E5
      cvv(113) = (-0.36E2 * cc ** 5 * dt ** 5 * dx ** 4 * dy ** 5 * dz *
     #* 7 + 0.60E2 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 5 * dz ** 7 + 0
     #.16E2 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 6 * dz ** 7) / dx ** 7
     # / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(121) = (-0.36E2 * cc ** 5 * dt ** 5 * dx ** 4 * dy ** 7 * dz *
     #* 5 + 0.60E2 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 7 * dz ** 5 + 0
     #.16E2 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7
     # / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(122) = (0.24E2 * cc ** 6 * dt ** 6 * dx * dy ** 7 * dz ** 7 + 
     #0.72E2 * cc ** 5 * dt ** 5 * dx ** 4 * dy ** 7 * dz ** 5 + 0.72E2 
     #* cc ** 5 * dt ** 5 * dx ** 4 * dy ** 5 * dz ** 7 + 0.144E3 * cc *
     #* 5 * dt ** 5 * dx ** 2 * dy ** 7 * dz ** 7 - 0.105E3 * cc ** 4 * 
     #dt ** 4 * dx ** 3 * dy ** 7 * dz ** 7 - 0.120E3 * cc ** 3 * dt ** 
     #3 * dx ** 6 * dy ** 7 * dz ** 5 - 0.120E3 * cc ** 3 * dt ** 3 * dx
     # ** 6 * dy ** 5 * dz ** 7 - 0.32E2 * cc ** 3 * dt ** 3 * dx ** 5 *
     # dy ** 7 * dz ** 6 - 0.32E2 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 
     #6 * dz ** 7 - 0.848E3 * cc ** 3 * dt ** 3 * dx ** 4 * dy ** 7 * dz
     # ** 7 + 0.96E2 * cc ** 2 * dt ** 2 * dx ** 5 * dy ** 7 * dz ** 7 +
     # 0.512E3 * cc * dt * dx ** 6 * dy ** 7 * dz ** 7) / dx ** 7 / dy *
     #* 7 / dz ** 7 / 0.17280E5
      cvv(123) = (-0.36E2 * cc ** 5 * dt ** 5 * dx ** 4 * dy ** 7 * dz *
     #* 5 + 0.60E2 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 7 * dz ** 5 + 0
     #.16E2 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7
     # / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(131) = (-0.36E2 * cc ** 5 * dt ** 5 * dx ** 4 * dy ** 5 * dz *
     #* 7 + 0.60E2 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 5 * dz ** 7 + 0
     #.16E2 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 6 * dz ** 7) / dx ** 7
     # / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(185) = (-0.18E2 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 3 * dz *
     #* 7 - 0.18E2 * cc ** 5 * dt ** 5 * dx ** 3 * dy ** 6 * dz ** 7 + 0
     #.30E2 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 5 * dz ** 7 + 0.30E2 *
     # cc ** 3 * dt ** 3 * dx ** 5 * dy ** 6 * dz ** 7) / dx ** 7 / dy *
     #* 7 / dz ** 7 / 0.17280E5
      cvv(193) = -cc ** 5 * dt ** 5 / dy ** 2 / dz ** 2 / dx / 0.480E3
      cvv(194) = (0.72E2 * cc ** 6 * dt ** 6 * dx ** 3 * dy ** 5 * dz **
     # 7 + 0.72E2 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 5 * dz ** 5 + 0.
     #72E2 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 3 * dz ** 7 + 0.216E3 *
     # cc ** 5 * dt ** 5 * dx ** 4 * dy ** 5 * dz ** 7 + 0.72E2 * cc ** 
     #5 * dt ** 5 * dx ** 3 * dy ** 6 * dz ** 7 - 0.105E3 * cc ** 4 * dt
     # ** 4 * dx ** 5 * dy ** 5 * dz ** 7 - 0.480E3 * cc ** 3 * dt ** 3 
     #* dx ** 6 * dy ** 5 * dz ** 7 - 0.216E3 * cc ** 3 * dt ** 3 * dx *
     #* 5 * dy ** 6 * dz ** 7) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(195) = -cc ** 5 * dt ** 5 / dy ** 2 / dz ** 2 / dx / 0.480E3
      cvv(201) = (-0.18E2 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 7 * dz *
     #* 3 - 0.18E2 * cc ** 5 * dt ** 5 * dx ** 3 * dy ** 7 * dz ** 6 + 0
     #.30E2 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 7 * dz ** 5 + 0.30E2 *
     # cc ** 3 * dt ** 3 * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy *
     #* 7 / dz ** 7 / 0.17280E5
      cvv(202) = (0.72E2 * cc ** 6 * dt ** 6 * dx ** 3 * dy ** 7 * dz **
     # 5 + 0.72E2 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 7 * dz ** 3 + 0.
     #72E2 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 5 * dz ** 5 + 0.216E3 *
     # cc ** 5 * dt ** 5 * dx ** 4 * dy ** 7 * dz ** 5 + 0.72E2 * cc ** 
     #5 * dt ** 5 * dx ** 3 * dy ** 7 * dz ** 6 - 0.105E3 * cc ** 4 * dt
     # ** 4 * dx ** 5 * dy ** 7 * dz ** 5 - 0.480E3 * cc ** 3 * dt ** 3 
     #* dx ** 6 * dy ** 7 * dz ** 5 - 0.216E3 * cc ** 3 * dt ** 3 * dx *
     #* 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(203) = (-0.144E3 * cc ** 6 * dt ** 6 * dx ** 3 * dy ** 7 * dz 
     #** 5 - 0.144E3 * cc ** 6 * dt ** 6 * dx ** 3 * dy ** 5 * dz ** 7 -
     # 0.144E3 * cc ** 6 * dt ** 6 * dx * dy ** 7 * dz ** 7 - 0.108E3 * 
     #cc ** 5 * dt ** 5 * dx ** 6 * dy ** 7 * dz ** 3 - 0.144E3 * cc ** 
     #5 * dt ** 5 * dx ** 6 * dy ** 5 * dz ** 5 - 0.108E3 * cc ** 5 * dt
     # ** 5 * dx ** 6 * dy ** 3 * dz ** 7 - 0.432E3 * cc ** 5 * dt ** 5 
     #* dx ** 4 * dy ** 7 * dz ** 5 - 0.432E3 * cc ** 5 * dt ** 5 * dx *
     #* 4 * dy ** 5 * dz ** 7 - 0.108E3 * cc ** 5 * dt ** 5 * dx ** 3 * 
     #dy ** 7 * dz ** 6 - 0.108E3 * cc ** 5 * dt ** 5 * dx ** 3 * dy ** 
     #6 * dz ** 7 - 0.504E3 * cc ** 5 * dt ** 5 * dx ** 2 * dy ** 7 * dz
     # ** 7 + 0.210E3 * cc ** 4 * dt ** 4 * dx ** 5 * dy ** 7 * dz ** 5 
     #+ 0.210E3 * cc ** 4 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 7 + 0.13
     #50E4 * cc ** 4 * dt ** 4 * dx ** 3 * dy ** 7 * dz ** 7 + 0.900E3 *
     # cc ** 3 * dt ** 3 * dx ** 6 * dy ** 7 * dz ** 5 + 0.900E3 * cc **
     # 3 * dt ** 3 * dx ** 6 * dy ** 5 * dz ** 7 + 0.372E3 * cc ** 3 * d
     #t ** 3 * dx ** 5 * dy ** 7 * dz ** 6 + 0.372E3 * cc ** 3 * dt ** 3
     # * dx ** 5 * dy ** 6 * dz ** 7 + 0.2968E4 * cc ** 3 * dt ** 3 * dx
     # ** 4 * dy ** 7 * dz ** 7 - 0.1296E4 * cc ** 2 * dt ** 2 * dx ** 5
     # * dy ** 7 * dz ** 7 - 0.1792E4 * cc * dt * dx ** 6 * dy ** 7 * dz
     # ** 7) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(204) = (0.72E2 * cc ** 6 * dt ** 6 * dx ** 3 * dy ** 7 * dz **
     # 5 + 0.72E2 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 7 * dz ** 3 + 0.
     #72E2 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 5 * dz ** 5 + 0.216E3 *
     # cc ** 5 * dt ** 5 * dx ** 4 * dy ** 7 * dz ** 5 + 0.72E2 * cc ** 
     #5 * dt ** 5 * dx ** 3 * dy ** 7 * dz ** 6 - 0.105E3 * cc ** 4 * dt
     # ** 4 * dx ** 5 * dy ** 7 * dz ** 5 - 0.480E3 * cc ** 3 * dt ** 3 
     #* dx ** 6 * dy ** 7 * dz ** 5 - 0.216E3 * cc ** 3 * dt ** 3 * dx *
     #* 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(205) = (-0.18E2 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 7 * dz *
     #* 3 - 0.18E2 * cc ** 5 * dt ** 5 * dx ** 3 * dy ** 7 * dz ** 6 + 0
     #.30E2 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 7 * dz ** 5 + 0.30E2 *
     # cc ** 3 * dt ** 3 * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy *
     #* 7 / dz ** 7 / 0.17280E5
      cvv(211) = -cc ** 5 * dt ** 5 / dy ** 2 / dz ** 2 / dx / 0.480E3
      cvv(212) = (0.72E2 * cc ** 6 * dt ** 6 * dx ** 3 * dy ** 5 * dz **
     # 7 + 0.72E2 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 5 * dz ** 5 + 0.
     #72E2 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 3 * dz ** 7 + 0.216E3 *
     # cc ** 5 * dt ** 5 * dx ** 4 * dy ** 5 * dz ** 7 + 0.72E2 * cc ** 
     #5 * dt ** 5 * dx ** 3 * dy ** 6 * dz ** 7 - 0.105E3 * cc ** 4 * dt
     # ** 4 * dx ** 5 * dy ** 5 * dz ** 7 - 0.480E3 * cc ** 3 * dt ** 3 
     #* dx ** 6 * dy ** 5 * dz ** 7 - 0.216E3 * cc ** 3 * dt ** 3 * dx *
     #* 5 * dy ** 6 * dz ** 7) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(213) = -cc ** 5 * dt ** 5 / dy ** 2 / dz ** 2 / dx / 0.480E3
      cvv(221) = (-0.18E2 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 3 * dz *
     #* 7 - 0.18E2 * cc ** 5 * dt ** 5 * dx ** 3 * dy ** 6 * dz ** 7 + 0
     #.30E2 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 5 * dz ** 7 + 0.30E2 *
     # cc ** 3 * dt ** 3 * dx ** 5 * dy ** 6 * dz ** 7) / dx ** 7 / dy *
     #* 7 / dz ** 7 / 0.17280E5
      cvv(257) = (-0.36E2 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 4 * dz *
     #* 7 + 0.16E2 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 5 * dz ** 7 + 0
     #.60E2 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 6 * dz ** 7) / dx ** 7
     # / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(265) = -cc ** 5 * dt ** 5 / dx ** 2 / dz ** 2 / dy / 0.480E3
      cvv(266) = (0.72E2 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 3 * dz **
     # 7 + 0.72E2 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 3 * dz ** 7 + 0.
     #72E2 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 6 * dz ** 5 + 0.216E3 *
     # cc ** 5 * dt ** 5 * dx ** 5 * dy ** 4 * dz ** 7 + 0.72E2 * cc ** 
     #5 * dt ** 5 * dx ** 3 * dy ** 6 * dz ** 7 - 0.105E3 * cc ** 4 * dt
     # ** 4 * dx ** 5 * dy ** 5 * dz ** 7 - 0.216E3 * cc ** 3 * dt ** 3 
     #* dx ** 6 * dy ** 5 * dz ** 7 - 0.480E3 * cc ** 3 * dt ** 3 * dx *
     #* 5 * dy ** 6 * dz ** 7) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(267) = -cc ** 5 * dt ** 5 / dx ** 2 / dz ** 2 / dy / 0.480E3
      cvv(273) = -cc ** 5 * dt ** 5 / dx ** 2 / dy ** 2 / dz / 0.480E3
      cvv(274) = (0.144E3 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 5 * dz *
     #* 5 + 0.144E3 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 5 * dz ** 5 + 
     #0.144E3 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 6 * dz ** 5 + 0.144E
     #3 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / d
     #y ** 7 / dz ** 7 / 0.17280E5
      cvv(275) = (-0.288E3 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 5 * dz 
     #** 5 - 0.288E3 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 3 * dz ** 7 -
     # 0.288E3 * cc ** 6 * dt ** 6 * dx ** 3 * dy ** 5 * dz ** 7 - 0.288
     #E3 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 5 * dz ** 5 - 0.288E3 * c
     #c ** 5 * dt ** 5 * dx ** 6 * dy ** 3 * dz ** 7 - 0.288E3 * cc ** 5
     # * dt ** 5 * dx ** 5 * dy ** 6 * dz ** 5 - 0.216E3 * cc ** 5 * dt 
     #** 5 * dx ** 5 * dy ** 5 * dz ** 6 - 0.540E3 * cc ** 5 * dt ** 5 *
     # dx ** 5 * dy ** 4 * dz ** 7 - 0.540E3 * cc ** 5 * dt ** 5 * dx **
     # 4 * dy ** 5 * dz ** 7 - 0.288E3 * cc ** 5 * dt ** 5 * dx ** 3 * d
     #y ** 6 * dz ** 7 + 0.2280E4 * cc ** 4 * dt ** 4 * dx ** 5 * dy ** 
     #5 * dz ** 7 + 0.1620E4 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 5 * d
     #z ** 7 + 0.1620E4 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 6 * dz ** 
     #7) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(276) = (0.144E3 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 5 * dz *
     #* 5 + 0.144E3 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 5 * dz ** 5 + 
     #0.144E3 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 6 * dz ** 5 + 0.144E
     #3 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / d
     #y ** 7 / dz ** 7 / 0.17280E5
      cvv(277) = -cc ** 5 * dt ** 5 / dx ** 2 / dy ** 2 / dz / 0.480E3
      cvv(281) = (-0.36E2 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 7 * dz *
     #* 4 + 0.16E2 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 7 * dz ** 5 + 0
     #.60E2 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7
     # / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(282) = (0.72E2 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 7 * dz **
     # 3 + 0.72E2 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 7 * dz ** 3 + 0.
     #216E3 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 7 * dz ** 4 + 0.72E2 *
     # cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6 + 0.72E2 * cc ** 
     #5 * dt ** 5 * dx ** 3 * dy ** 7 * dz ** 6 - 0.105E3 * cc ** 4 * dt
     # ** 4 * dx ** 5 * dy ** 7 * dz ** 5 - 0.216E3 * cc ** 3 * dt ** 3 
     #* dx ** 6 * dy ** 7 * dz ** 5 - 0.480E3 * cc ** 3 * dt ** 3 * dx *
     #* 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(283) = (-0.288E3 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 7 * dz 
     #** 3 - 0.288E3 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 5 -
     # 0.288E3 * cc ** 6 * dt ** 6 * dx ** 3 * dy ** 7 * dz ** 5 - 0.288
     #E3 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 7 * dz ** 3 - 0.288E3 * c
     #c ** 5 * dt ** 5 * dx ** 6 * dy ** 5 * dz ** 5 - 0.540E3 * cc ** 5
     # * dt ** 5 * dx ** 5 * dy ** 7 * dz ** 4 - 0.216E3 * cc ** 5 * dt 
     #** 5 * dx ** 5 * dy ** 6 * dz ** 5 - 0.288E3 * cc ** 5 * dt ** 5 *
     # dx ** 5 * dy ** 5 * dz ** 6 - 0.540E3 * cc ** 5 * dt ** 5 * dx **
     # 4 * dy ** 7 * dz ** 5 - 0.288E3 * cc ** 5 * dt ** 5 * dx ** 3 * d
     #y ** 7 * dz ** 6 + 0.2280E4 * cc ** 4 * dt ** 4 * dx ** 5 * dy ** 
     #7 * dz ** 5 + 0.1620E4 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 7 * d
     #z ** 5 + 0.1620E4 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 7 * dz ** 
     #6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(284) = (0.432E3 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 7 * dz *
     #* 3 + 0.576E3 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 5 + 
     #0.432E3 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 3 * dz ** 7 + 0.576E
     #3 * cc ** 6 * dt ** 6 * dx ** 3 * dy ** 7 * dz ** 5 + 0.576E3 * cc
     # ** 6 * dt ** 6 * dx ** 3 * dy ** 5 * dz ** 7 + 0.360E3 * cc ** 6 
     #* dt ** 6 * dx * dy ** 7 * dz ** 7 + 0.432E3 * cc ** 5 * dt ** 5 *
     # dx ** 6 * dy ** 7 * dz ** 3 + 0.576E3 * cc ** 5 * dt ** 5 * dx **
     # 6 * dy ** 5 * dz ** 5 + 0.432E3 * cc ** 5 * dt ** 5 * dx ** 6 * d
     #y ** 3 * dz ** 7 + 0.720E3 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 7
     # * dz ** 4 + 0.432E3 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 6 * dz 
     #** 5 + 0.432E3 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6 +
     # 0.720E3 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 4 * dz ** 7 + 0.108
     #0E4 * cc ** 5 * dt ** 5 * dx ** 4 * dy ** 7 * dz ** 5 + 0.1080E4 *
     # cc ** 5 * dt ** 5 * dx ** 4 * dy ** 5 * dz ** 7 + 0.432E3 * cc **
     # 5 * dt ** 5 * dx ** 3 * dy ** 7 * dz ** 6 + 0.432E3 * cc ** 5 * d
     #t ** 5 * dx ** 3 * dy ** 6 * dz ** 7 + 0.1008E4 * cc ** 5 * dt ** 
     #5 * dx ** 2 * dy ** 7 * dz ** 7 - 0.4350E4 * cc ** 4 * dt ** 4 * d
     #x ** 5 * dy ** 7 * dz ** 5 - 0.4350E4 * cc ** 4 * dt ** 4 * dx ** 
     #5 * dy ** 5 * dz ** 7 - 0.4455E4 * cc ** 4 * dt ** 4 * dx ** 3 * d
     #y ** 7 * dz ** 7 - 0.2840E4 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 
     #7 * dz ** 5 - 0.2840E4 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 5 * d
     #z ** 7 - 0.2400E4 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 7 * dz ** 
     #6 - 0.2400E4 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 6 * dz ** 7 - 0
     #.5936E4 * cc ** 3 * dt ** 3 * dx ** 4 * dy ** 7 * dz ** 7 + 0.1296
     #0E5 * cc ** 2 * dt ** 2 * dx ** 5 * dy ** 7 * dz ** 7 + 0.3584E4 *
     # cc * dt * dx ** 6 * dy ** 7 * dz ** 7) / dx ** 7 / dy ** 7 / dz *
     #* 7 / 0.17280E5
      cvv(285) = (-0.288E3 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 7 * dz 
     #** 3 - 0.288E3 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 5 -
     # 0.288E3 * cc ** 6 * dt ** 6 * dx ** 3 * dy ** 7 * dz ** 5 - 0.288
     #E3 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 7 * dz ** 3 - 0.288E3 * c
     #c ** 5 * dt ** 5 * dx ** 6 * dy ** 5 * dz ** 5 - 0.540E3 * cc ** 5
     # * dt ** 5 * dx ** 5 * dy ** 7 * dz ** 4 - 0.216E3 * cc ** 5 * dt 
     #** 5 * dx ** 5 * dy ** 6 * dz ** 5 - 0.288E3 * cc ** 5 * dt ** 5 *
     # dx ** 5 * dy ** 5 * dz ** 6 - 0.540E3 * cc ** 5 * dt ** 5 * dx **
     # 4 * dy ** 7 * dz ** 5 - 0.288E3 * cc ** 5 * dt ** 5 * dx ** 3 * d
     #y ** 7 * dz ** 6 + 0.2280E4 * cc ** 4 * dt ** 4 * dx ** 5 * dy ** 
     #7 * dz ** 5 + 0.1620E4 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 7 * d
     #z ** 5 + 0.1620E4 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 7 * dz ** 
     #6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(286) = (0.72E2 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 7 * dz **
     # 3 + 0.72E2 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 7 * dz ** 3 + 0.
     #216E3 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 7 * dz ** 4 + 0.72E2 *
     # cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6 + 0.72E2 * cc ** 
     #5 * dt ** 5 * dx ** 3 * dy ** 7 * dz ** 6 - 0.105E3 * cc ** 4 * dt
     # ** 4 * dx ** 5 * dy ** 7 * dz ** 5 - 0.216E3 * cc ** 3 * dt ** 3 
     #* dx ** 6 * dy ** 7 * dz ** 5 - 0.480E3 * cc ** 3 * dt ** 3 * dx *
     #* 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(287) = (-0.36E2 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 7 * dz *
     #* 4 + 0.16E2 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 7 * dz ** 5 + 0
     #.60E2 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7
     # / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(291) = -cc ** 5 * dt ** 5 / dx ** 2 / dy ** 2 / dz / 0.480E3
      cvv(292) = (0.144E3 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 5 * dz *
     #* 5 + 0.144E3 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 5 * dz ** 5 + 
     #0.144E3 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 6 * dz ** 5 + 0.144E
     #3 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / d
     #y ** 7 / dz ** 7 / 0.17280E5
      cvv(293) = (-0.288E3 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 5 * dz 
     #** 5 - 0.288E3 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 3 * dz ** 7 -
     # 0.288E3 * cc ** 6 * dt ** 6 * dx ** 3 * dy ** 5 * dz ** 7 - 0.288
     #E3 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 5 * dz ** 5 - 0.288E3 * c
     #c ** 5 * dt ** 5 * dx ** 6 * dy ** 3 * dz ** 7 - 0.288E3 * cc ** 5
     # * dt ** 5 * dx ** 5 * dy ** 6 * dz ** 5 - 0.216E3 * cc ** 5 * dt 
     #** 5 * dx ** 5 * dy ** 5 * dz ** 6 - 0.540E3 * cc ** 5 * dt ** 5 *
     # dx ** 5 * dy ** 4 * dz ** 7 - 0.540E3 * cc ** 5 * dt ** 5 * dx **
     # 4 * dy ** 5 * dz ** 7 - 0.288E3 * cc ** 5 * dt ** 5 * dx ** 3 * d
     #y ** 6 * dz ** 7 + 0.2280E4 * cc ** 4 * dt ** 4 * dx ** 5 * dy ** 
     #5 * dz ** 7 + 0.1620E4 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 5 * d
     #z ** 7 + 0.1620E4 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 6 * dz ** 
     #7) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(294) = (0.144E3 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 5 * dz *
     #* 5 + 0.144E3 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 5 * dz ** 5 + 
     #0.144E3 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 6 * dz ** 5 + 0.144E
     #3 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / d
     #y ** 7 / dz ** 7 / 0.17280E5
      cvv(295) = -cc ** 5 * dt ** 5 / dx ** 2 / dy ** 2 / dz / 0.480E3
      cvv(301) = -cc ** 5 * dt ** 5 / dx ** 2 / dz ** 2 / dy / 0.480E3
      cvv(302) = (0.72E2 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 3 * dz **
     # 7 + 0.72E2 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 3 * dz ** 7 + 0.
     #72E2 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 6 * dz ** 5 + 0.216E3 *
     # cc ** 5 * dt ** 5 * dx ** 5 * dy ** 4 * dz ** 7 + 0.72E2 * cc ** 
     #5 * dt ** 5 * dx ** 3 * dy ** 6 * dz ** 7 - 0.105E3 * cc ** 4 * dt
     # ** 4 * dx ** 5 * dy ** 5 * dz ** 7 - 0.216E3 * cc ** 3 * dt ** 3 
     #* dx ** 6 * dy ** 5 * dz ** 7 - 0.480E3 * cc ** 3 * dt ** 3 * dx *
     #* 5 * dy ** 6 * dz ** 7) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(303) = -cc ** 5 * dt ** 5 / dx ** 2 / dz ** 2 / dy / 0.480E3
      cvv(311) = (-0.36E2 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 4 * dz *
     #* 7 + 0.16E2 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 5 * dz ** 7 + 0
     #.60E2 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 6 * dz ** 7) / dx ** 7
     # / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(329) = (-0.18E2 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 2 * dz *
     #* 7 + 0.106E3 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 4 * dz ** 7 - 
     #0.64E2 * cc * dt * dx ** 7 * dy ** 6 * dz ** 7) / dx ** 7 / dy ** 
     #7 / dz ** 7 / 0.17280E5
      cvv(337) = (-0.36E2 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 4 * dz *
     #* 5 + 0.60E2 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 6 * dz ** 5 + 0
     #.16E2 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7
     # / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(338) = (0.24E2 * cc ** 6 * dt ** 6 * dx ** 7 * dy * dz ** 7 + 
     #0.72E2 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 4 * dz ** 5 + 0.144E3
     # * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 2 * dz ** 7 + 0.72E2 * cc *
     #* 5 * dt ** 5 * dx ** 5 * dy ** 4 * dz ** 7 - 0.105E3 * cc ** 4 * 
     #dt ** 4 * dx ** 7 * dy ** 3 * dz ** 7 - 0.120E3 * cc ** 3 * dt ** 
     #3 * dx ** 7 * dy ** 6 * dz ** 5 - 0.32E2 * cc ** 3 * dt ** 3 * dx 
     #** 7 * dy ** 5 * dz ** 6 - 0.848E3 * cc ** 3 * dt ** 3 * dx ** 7 *
     # dy ** 4 * dz ** 7 - 0.32E2 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 
     #5 * dz ** 7 - 0.120E3 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 6 * dz
     # ** 7 + 0.96E2 * cc ** 2 * dt ** 2 * dx ** 7 * dy ** 5 * dz ** 7 +
     # 0.512E3 * cc * dt * dx ** 7 * dy ** 6 * dz ** 7) / dx ** 7 / dy *
     #* 7 / dz ** 7 / 0.17280E5
      cvv(339) = (-0.36E2 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 4 * dz *
     #* 5 + 0.60E2 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 6 * dz ** 5 + 0
     #.16E2 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7
     # / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(345) = (-0.18E2 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 6 * dz *
     #* 3 - 0.18E2 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 3 * dz ** 6 + 0
     #.30E2 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 6 * dz ** 5 + 0.30E2 *
     # cc ** 3 * dt ** 3 * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy *
     #* 7 / dz ** 7 / 0.17280E5
      cvv(346) = (0.72E2 * cc ** 6 * dt ** 6 * dx ** 7 * dy ** 3 * dz **
     # 5 + 0.72E2 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 6 * dz ** 3 + 0.
     #216E3 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 4 * dz ** 5 + 0.72E2 *
     # cc ** 5 * dt ** 5 * dx ** 7 * dy ** 3 * dz ** 6 + 0.72E2 * cc ** 
     #5 * dt ** 5 * dx ** 5 * dy ** 6 * dz ** 5 - 0.105E3 * cc ** 4 * dt
     # ** 4 * dx ** 7 * dy ** 5 * dz ** 5 - 0.480E3 * cc ** 3 * dt ** 3 
     #* dx ** 7 * dy ** 6 * dz ** 5 - 0.216E3 * cc ** 3 * dt ** 3 * dx *
     #* 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(347) = (-0.144E3 * cc ** 6 * dt ** 6 * dx ** 7 * dy ** 3 * dz 
     #** 5 - 0.144E3 * cc ** 6 * dt ** 6 * dx ** 7 * dy * dz ** 7 - 0.14
     #4E3 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 3 * dz ** 7 - 0.108E3 * 
     #cc ** 5 * dt ** 5 * dx ** 7 * dy ** 6 * dz ** 3 - 0.432E3 * cc ** 
     #5 * dt ** 5 * dx ** 7 * dy ** 4 * dz ** 5 - 0.108E3 * cc ** 5 * dt
     # ** 5 * dx ** 7 * dy ** 3 * dz ** 6 - 0.504E3 * cc ** 5 * dt ** 5 
     #* dx ** 7 * dy ** 2 * dz ** 7 - 0.108E3 * cc ** 5 * dt ** 5 * dx *
     #* 6 * dy ** 3 * dz ** 7 - 0.144E3 * cc ** 5 * dt ** 5 * dx ** 5 * 
     #dy ** 6 * dz ** 5 - 0.432E3 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 
     #4 * dz ** 7 - 0.108E3 * cc ** 5 * dt ** 5 * dx ** 3 * dy ** 6 * dz
     # ** 7 + 0.210E3 * cc ** 4 * dt ** 4 * dx ** 7 * dy ** 5 * dz ** 5 
     #+ 0.1350E4 * cc ** 4 * dt ** 4 * dx ** 7 * dy ** 3 * dz ** 7 + 0.2
     #10E3 * cc ** 4 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 7 + 0.900E3 *
     # cc ** 3 * dt ** 3 * dx ** 7 * dy ** 6 * dz ** 5 + 0.372E3 * cc **
     # 3 * dt ** 3 * dx ** 7 * dy ** 5 * dz ** 6 + 0.2968E4 * cc ** 3 * 
     #dt ** 3 * dx ** 7 * dy ** 4 * dz ** 7 + 0.372E3 * cc ** 3 * dt ** 
     #3 * dx ** 6 * dy ** 5 * dz ** 7 + 0.900E3 * cc ** 3 * dt ** 3 * dx
     # ** 5 * dy ** 6 * dz ** 7 - 0.1296E4 * cc ** 2 * dt ** 2 * dx ** 7
     # * dy ** 5 * dz ** 7 - 0.1792E4 * cc * dt * dx ** 7 * dy ** 6 * dz
     # ** 7) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(348) = (0.72E2 * cc ** 6 * dt ** 6 * dx ** 7 * dy ** 3 * dz **
     # 5 + 0.72E2 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 6 * dz ** 3 + 0.
     #216E3 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 4 * dz ** 5 + 0.72E2 *
     # cc ** 5 * dt ** 5 * dx ** 7 * dy ** 3 * dz ** 6 + 0.72E2 * cc ** 
     #5 * dt ** 5 * dx ** 5 * dy ** 6 * dz ** 5 - 0.105E3 * cc ** 4 * dt
     # ** 4 * dx ** 7 * dy ** 5 * dz ** 5 - 0.480E3 * cc ** 3 * dt ** 3 
     #* dx ** 7 * dy ** 6 * dz ** 5 - 0.216E3 * cc ** 3 * dt ** 3 * dx *
     #* 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(349) = (-0.18E2 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 6 * dz *
     #* 3 - 0.18E2 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 3 * dz ** 6 + 0
     #.30E2 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 6 * dz ** 5 + 0.30E2 *
     # cc ** 3 * dt ** 3 * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy *
     #* 7 / dz ** 7 / 0.17280E5
      cvv(353) = (-0.36E2 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 5 * dz *
     #* 4 + 0.16E2 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 6 * dz ** 5 + 0
     #.60E2 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7
     # / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(354) = (0.72E2 * cc ** 6 * dt ** 6 * dx ** 7 * dy ** 5 * dz **
     # 3 + 0.72E2 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 6 * dz ** 3 + 0.
     #216E3 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 5 * dz ** 4 + 0.72E2 *
     # cc ** 5 * dt ** 5 * dx ** 7 * dy ** 3 * dz ** 6 + 0.72E2 * cc ** 
     #5 * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6 - 0.105E3 * cc ** 4 * dt
     # ** 4 * dx ** 7 * dy ** 5 * dz ** 5 - 0.216E3 * cc ** 3 * dt ** 3 
     #* dx ** 7 * dy ** 6 * dz ** 5 - 0.480E3 * cc ** 3 * dt ** 3 * dx *
     #* 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(355) = (-0.288E3 * cc ** 6 * dt ** 6 * dx ** 7 * dy ** 5 * dz 
     #** 3 - 0.288E3 * cc ** 6 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 5 -
     # 0.288E3 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 5 - 0.288
     #E3 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 6 * dz ** 3 - 0.540E3 * c
     #c ** 5 * dt ** 5 * dx ** 7 * dy ** 5 * dz ** 4 - 0.540E3 * cc ** 5
     # * dt ** 5 * dx ** 7 * dy ** 4 * dz ** 5 - 0.288E3 * cc ** 5 * dt 
     #** 5 * dx ** 7 * dy ** 3 * dz ** 6 - 0.216E3 * cc ** 5 * dt ** 5 *
     # dx ** 6 * dy ** 5 * dz ** 5 - 0.288E3 * cc ** 5 * dt ** 5 * dx **
     # 5 * dy ** 6 * dz ** 5 - 0.288E3 * cc ** 5 * dt ** 5 * dx ** 5 * d
     #y ** 5 * dz ** 6 + 0.2280E4 * cc ** 4 * dt ** 4 * dx ** 7 * dy ** 
     #5 * dz ** 5 + 0.1620E4 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 6 * d
     #z ** 5 + 0.1620E4 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 5 * dz ** 
     #6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(356) = (0.432E3 * cc ** 6 * dt ** 6 * dx ** 7 * dy ** 5 * dz *
     #* 3 + 0.576E3 * cc ** 6 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 5 + 
     #0.360E3 * cc ** 6 * dt ** 6 * dx ** 7 * dy * dz ** 7 + 0.576E3 * c
     #c ** 6 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 5 + 0.576E3 * cc ** 6
     # * dt ** 6 * dx ** 5 * dy ** 3 * dz ** 7 + 0.432E3 * cc ** 6 * dt 
     #** 6 * dx ** 3 * dy ** 5 * dz ** 7 + 0.432E3 * cc ** 5 * dt ** 5 *
     # dx ** 7 * dy ** 6 * dz ** 3 + 0.720E3 * cc ** 5 * dt ** 5 * dx **
     # 7 * dy ** 5 * dz ** 4 + 0.1080E4 * cc ** 5 * dt ** 5 * dx ** 7 * 
     #dy ** 4 * dz ** 5 + 0.432E3 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 
     #3 * dz ** 6 + 0.1008E4 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 2 * d
     #z ** 7 + 0.432E3 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 5 * dz ** 5
     # + 0.432E3 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 3 * dz ** 7 + 0.5
     #76E3 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 6 * dz ** 5 + 0.432E3 *
     # cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6 + 0.1080E4 * cc *
     #* 5 * dt ** 5 * dx ** 5 * dy ** 4 * dz ** 7 + 0.720E3 * cc ** 5 * 
     #dt ** 5 * dx ** 4 * dy ** 5 * dz ** 7 + 0.432E3 * cc ** 5 * dt ** 
     #5 * dx ** 3 * dy ** 6 * dz ** 7 - 0.4350E4 * cc ** 4 * dt ** 4 * d
     #x ** 7 * dy ** 5 * dz ** 5 - 0.4455E4 * cc ** 4 * dt ** 4 * dx ** 
     #7 * dy ** 3 * dz ** 7 - 0.4350E4 * cc ** 4 * dt ** 4 * dx ** 5 * d
     #y ** 5 * dz ** 7 - 0.2840E4 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 
     #6 * dz ** 5 - 0.2400E4 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 5 * d
     #z ** 6 - 0.5936E4 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 4 * dz ** 
     #7 - 0.2400E4 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 5 * dz ** 7 - 0
     #.2840E4 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 6 * dz ** 7 + 0.1296
     #0E5 * cc ** 2 * dt ** 2 * dx ** 7 * dy ** 5 * dz ** 7 + 0.3584E4 *
     # cc * dt * dx ** 7 * dy ** 6 * dz ** 7) / dx ** 7 / dy ** 7 / dz *
     #* 7 / 0.17280E5
      cvv(357) = (-0.288E3 * cc ** 6 * dt ** 6 * dx ** 7 * dy ** 5 * dz 
     #** 3 - 0.288E3 * cc ** 6 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 5 -
     # 0.288E3 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 5 - 0.288
     #E3 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 6 * dz ** 3 - 0.540E3 * c
     #c ** 5 * dt ** 5 * dx ** 7 * dy ** 5 * dz ** 4 - 0.540E3 * cc ** 5
     # * dt ** 5 * dx ** 7 * dy ** 4 * dz ** 5 - 0.288E3 * cc ** 5 * dt 
     #** 5 * dx ** 7 * dy ** 3 * dz ** 6 - 0.216E3 * cc ** 5 * dt ** 5 *
     # dx ** 6 * dy ** 5 * dz ** 5 - 0.288E3 * cc ** 5 * dt ** 5 * dx **
     # 5 * dy ** 6 * dz ** 5 - 0.288E3 * cc ** 5 * dt ** 5 * dx ** 5 * d
     #y ** 5 * dz ** 6 + 0.2280E4 * cc ** 4 * dt ** 4 * dx ** 7 * dy ** 
     #5 * dz ** 5 + 0.1620E4 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 6 * d
     #z ** 5 + 0.1620E4 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 5 * dz ** 
     #6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(358) = (0.72E2 * cc ** 6 * dt ** 6 * dx ** 7 * dy ** 5 * dz **
     # 3 + 0.72E2 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 6 * dz ** 3 + 0.
     #216E3 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 5 * dz ** 4 + 0.72E2 *
     # cc ** 5 * dt ** 5 * dx ** 7 * dy ** 3 * dz ** 6 + 0.72E2 * cc ** 
     #5 * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6 - 0.105E3 * cc ** 4 * dt
     # ** 4 * dx ** 7 * dy ** 5 * dz ** 5 - 0.216E3 * cc ** 3 * dt ** 3 
     #* dx ** 7 * dy ** 6 * dz ** 5 - 0.480E3 * cc ** 3 * dt ** 3 * dx *
     #* 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(359) = (-0.36E2 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 5 * dz *
     #* 4 + 0.16E2 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 6 * dz ** 5 + 0
     #.60E2 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7
     # / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(361) = (-0.18E2 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 7 * dz *
     #* 2 + 0.106E3 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 7 * dz ** 4 - 
     #0.64E2 * cc * dt * dx ** 7 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 
     #7 / dz ** 7 / 0.17280E5
      cvv(362) = (0.24E2 * cc ** 6 * dt ** 6 * dx ** 7 * dy ** 7 * dz + 
     #0.144E3 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 7 * dz ** 2 + 0.72E2
     # * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 5 * dz ** 4 + 0.72E2 * cc *
     #* 5 * dt ** 5 * dx ** 5 * dy ** 7 * dz ** 4 - 0.105E3 * cc ** 4 * 
     #dt ** 4 * dx ** 7 * dy ** 7 * dz ** 3 - 0.848E3 * cc ** 3 * dt ** 
     #3 * dx ** 7 * dy ** 7 * dz ** 4 - 0.32E2 * cc ** 3 * dt ** 3 * dx 
     #** 7 * dy ** 6 * dz ** 5 - 0.120E3 * cc ** 3 * dt ** 3 * dx ** 7 *
     # dy ** 5 * dz ** 6 - 0.32E2 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 
     #7 * dz ** 5 - 0.120E3 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 7 * dz
     # ** 6 + 0.96E2 * cc ** 2 * dt ** 2 * dx ** 7 * dy ** 7 * dz ** 5 +
     # 0.512E3 * cc * dt * dx ** 7 * dy ** 7 * dz ** 6) / dx ** 7 / dy *
     #* 7 / dz ** 7 / 0.17280E5
      cvv(363) = (-0.144E3 * cc ** 6 * dt ** 6 * dx ** 7 * dy ** 7 * dz 
     #- 0.144E3 * cc ** 6 * dt ** 6 * dx ** 7 * dy ** 5 * dz ** 3 - 0.14
     #4E3 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 7 * dz ** 3 - 0.504E3 * 
     #cc ** 5 * dt ** 5 * dx ** 7 * dy ** 7 * dz ** 2 - 0.108E3 * cc ** 
     #5 * dt ** 5 * dx ** 7 * dy ** 6 * dz ** 3 - 0.432E3 * cc ** 5 * dt
     # ** 5 * dx ** 7 * dy ** 5 * dz ** 4 - 0.108E3 * cc ** 5 * dt ** 5 
     #* dx ** 7 * dy ** 3 * dz ** 6 - 0.108E3 * cc ** 5 * dt ** 5 * dx *
     #* 6 * dy ** 7 * dz ** 3 - 0.432E3 * cc ** 5 * dt ** 5 * dx ** 5 * 
     #dy ** 7 * dz ** 4 - 0.144E3 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 
     #5 * dz ** 6 - 0.108E3 * cc ** 5 * dt ** 5 * dx ** 3 * dy ** 7 * dz
     # ** 6 + 0.1350E4 * cc ** 4 * dt ** 4 * dx ** 7 * dy ** 7 * dz ** 3
     # + 0.210E3 * cc ** 4 * dt ** 4 * dx ** 7 * dy ** 5 * dz ** 5 + 0.2
     #10E3 * cc ** 4 * dt ** 4 * dx ** 5 * dy ** 7 * dz ** 5 + 0.2968E4 
     #* cc ** 3 * dt ** 3 * dx ** 7 * dy ** 7 * dz ** 4 + 0.372E3 * cc *
     #* 3 * dt ** 3 * dx ** 7 * dy ** 6 * dz ** 5 + 0.900E3 * cc ** 3 * 
     #dt ** 3 * dx ** 7 * dy ** 5 * dz ** 6 + 0.372E3 * cc ** 3 * dt ** 
     #3 * dx ** 6 * dy ** 7 * dz ** 5 + 0.900E3 * cc ** 3 * dt ** 3 * dx
     # ** 5 * dy ** 7 * dz ** 6 - 0.1296E4 * cc ** 2 * dt ** 2 * dx ** 7
     # * dy ** 7 * dz ** 5 - 0.1792E4 * cc * dt * dx ** 7 * dy ** 7 * dz
     # ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(364) = (0.360E3 * cc ** 6 * dt ** 6 * dx ** 7 * dy ** 7 * dz +
     # 0.576E3 * cc ** 6 * dt ** 6 * dx ** 7 * dy ** 5 * dz ** 3 + 0.432
     #E3 * cc ** 6 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 5 + 0.576E3 * c
     #c ** 6 * dt ** 6 * dx ** 5 * dy ** 7 * dz ** 3 + 0.576E3 * cc ** 6
     # * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 5 + 0.432E3 * cc ** 6 * dt 
     #** 6 * dx ** 3 * dy ** 7 * dz ** 5 + 0.1008E4 * cc ** 5 * dt ** 5 
     #* dx ** 7 * dy ** 7 * dz ** 2 + 0.432E3 * cc ** 5 * dt ** 5 * dx *
     #* 7 * dy ** 6 * dz ** 3 + 0.1080E4 * cc ** 5 * dt ** 5 * dx ** 7 *
     # dy ** 5 * dz ** 4 + 0.720E3 * cc ** 5 * dt ** 5 * dx ** 7 * dy **
     # 4 * dz ** 5 + 0.432E3 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 3 * d
     #z ** 6 + 0.432E3 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 7 * dz ** 3
     # + 0.432E3 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 5 * dz ** 5 + 0.1
     #080E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 7 * dz ** 4 + 0.432E3 
     #* cc ** 5 * dt ** 5 * dx ** 5 * dy ** 6 * dz ** 5 + 0.576E3 * cc *
     #* 5 * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6 + 0.720E3 * cc ** 5 * 
     #dt ** 5 * dx ** 4 * dy ** 7 * dz ** 5 + 0.432E3 * cc ** 5 * dt ** 
     #5 * dx ** 3 * dy ** 7 * dz ** 6 - 0.4455E4 * cc ** 4 * dt ** 4 * d
     #x ** 7 * dy ** 7 * dz ** 3 - 0.4350E4 * cc ** 4 * dt ** 4 * dx ** 
     #7 * dy ** 5 * dz ** 5 - 0.4350E4 * cc ** 4 * dt ** 4 * dx ** 5 * d
     #y ** 7 * dz ** 5 - 0.5936E4 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 
     #7 * dz ** 4 - 0.2400E4 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 6 * d
     #z ** 5 - 0.2840E4 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 5 * dz ** 
     #6 - 0.2400E4 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 7 * dz ** 5 - 0
     #.2840E4 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 7 * dz ** 6 + 0.1296
     #0E5 * cc ** 2 * dt ** 2 * dx ** 7 * dy ** 7 * dz ** 5 + 0.3584E4 *
     # cc * dt * dx ** 7 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz *
     #* 7 / 0.17280E5
      cvv(365) = (-0.480E3 * cc ** 6 * dt ** 6 * dx ** 7 * dy ** 7 * dz 
     #- 0.864E3 * cc ** 6 * dt ** 6 * dx ** 7 * dy ** 5 * dz ** 3 - 0.86
     #4E3 * cc ** 6 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 5 - 0.480E3 * 
     #cc ** 6 * dt ** 6 * dx ** 7 * dy * dz ** 7 - 0.864E3 * cc ** 6 * d
     #t ** 6 * dx ** 5 * dy ** 7 * dz ** 3 - 0.1152E4 * cc ** 6 * dt ** 
     #6 * dx ** 5 * dy ** 5 * dz ** 5 - 0.864E3 * cc ** 6 * dt ** 6 * dx
     # ** 5 * dy ** 3 * dz ** 7 - 0.864E3 * cc ** 6 * dt ** 6 * dx ** 3 
     #* dy ** 7 * dz ** 5 - 0.864E3 * cc ** 6 * dt ** 6 * dx ** 3 * dy *
     #* 5 * dz ** 7 - 0.480E3 * cc ** 6 * dt ** 6 * dx * dy ** 7 * dz **
     # 7 - 0.1260E4 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 7 * dz ** 2 - 
     #0.648E3 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 6 * dz ** 3 - 0.1440
     #E4 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 5 * dz ** 4 - 0.1440E4 * 
     #cc ** 5 * dt ** 5 * dx ** 7 * dy ** 4 * dz ** 5 - 0.648E3 * cc ** 
     #5 * dt ** 5 * dx ** 7 * dy ** 3 * dz ** 6 - 0.1260E4 * cc ** 5 * d
     #t ** 5 * dx ** 7 * dy ** 2 * dz ** 7 - 0.648E3 * cc ** 5 * dt ** 5
     # * dx ** 6 * dy ** 7 * dz ** 3 - 0.864E3 * cc ** 5 * dt ** 5 * dx 
     #** 6 * dy ** 5 * dz ** 5 - 0.648E3 * cc ** 5 * dt ** 5 * dx ** 6 *
     # dy ** 3 * dz ** 7 - 0.1440E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy *
     #* 7 * dz ** 4 - 0.864E3 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 6 * 
     #dz ** 5 - 0.864E3 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 
     #6 - 0.1440E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 4 * dz ** 7 - 0
     #.1440E4 * cc ** 5 * dt ** 5 * dx ** 4 * dy ** 7 * dz ** 5 - 0.1440
     #E4 * cc ** 5 * dt ** 5 * dx ** 4 * dy ** 5 * dz ** 7 - 0.648E3 * c
     #c ** 5 * dt ** 5 * dx ** 3 * dy ** 7 * dz ** 6 - 0.648E3 * cc ** 5
     # * dt ** 5 * dx ** 3 * dy ** 6 * dz ** 7 - 0.1260E4 * cc ** 5 * dt
     # ** 5 * dx ** 2 * dy ** 7 * dz ** 7 + 0.6420E4 * cc ** 4 * dt ** 4
     # * dx ** 7 * dy ** 7 * dz ** 3 + 0.8280E4 * cc ** 4 * dt ** 4 * dx
     # ** 7 * dy ** 5 * dz ** 5 + 0.6420E4 * cc ** 4 * dt ** 4 * dx ** 7
     # * dy ** 3 * dz ** 7 + 0.8280E4 * cc ** 4 * dt ** 4 * dx ** 5 * dy
     # ** 7 * dz ** 5 + 0.8280E4 * cc ** 4 * dt ** 4 * dx ** 5 * dy ** 5
     # * dz ** 7 + 0.6420E4 * cc ** 4 * dt ** 4 * dx ** 3 * dy ** 7 * dz
     # ** 7 + 0.7420E4 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 7 * dz ** 4
     # + 0.4120E4 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 6 * dz ** 5 + 0.
     #4120E4 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 5 * dz ** 6 + 0.7420E
     #4 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 4 * dz ** 7 + 0.4120E4 * c
     #c ** 3 * dt ** 3 * dx ** 6 * dy ** 7 * dz ** 5 + 0.4120E4 * cc ** 
     #3 * dt ** 3 * dx ** 6 * dy ** 5 * dz ** 7 + 0.4120E4 * cc ** 3 * d
     #t ** 3 * dx ** 5 * dy ** 7 * dz ** 6 + 0.4120E4 * cc ** 3 * dt ** 
     #3 * dx ** 5 * dy ** 6 * dz ** 7 + 0.7420E4 * cc ** 3 * dt ** 3 * d
     #x ** 4 * dy ** 7 * dz ** 7 - 0.23520E5 * cc ** 2 * dt ** 2 * dx **
     # 7 * dy ** 7 * dz ** 5 - 0.23520E5 * cc ** 2 * dt ** 2 * dx ** 7 *
     # dy ** 5 * dz ** 7 - 0.23520E5 * cc ** 2 * dt ** 2 * dx ** 5 * dy 
     #** 7 * dz ** 7 - 0.4480E4 * cc * dt * dx ** 7 * dy ** 7 * dz ** 6 
     #- 0.4480E4 * cc * dt * dx ** 7 * dy ** 6 * dz ** 7 - 0.4480E4 * cc
     # * dt * dx ** 6 * dy ** 7 * dz ** 7 + 0.17280E5 * dx ** 7 * dy ** 
     #7 * dz ** 7) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(366) = (0.360E3 * cc ** 6 * dt ** 6 * dx ** 7 * dy ** 7 * dz +
     # 0.576E3 * cc ** 6 * dt ** 6 * dx ** 7 * dy ** 5 * dz ** 3 + 0.432
     #E3 * cc ** 6 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 5 + 0.576E3 * c
     #c ** 6 * dt ** 6 * dx ** 5 * dy ** 7 * dz ** 3 + 0.576E3 * cc ** 6
     # * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 5 + 0.432E3 * cc ** 6 * dt 
     #** 6 * dx ** 3 * dy ** 7 * dz ** 5 + 0.1008E4 * cc ** 5 * dt ** 5 
     #* dx ** 7 * dy ** 7 * dz ** 2 + 0.432E3 * cc ** 5 * dt ** 5 * dx *
     #* 7 * dy ** 6 * dz ** 3 + 0.1080E4 * cc ** 5 * dt ** 5 * dx ** 7 *
     # dy ** 5 * dz ** 4 + 0.720E3 * cc ** 5 * dt ** 5 * dx ** 7 * dy **
     # 4 * dz ** 5 + 0.432E3 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 3 * d
     #z ** 6 + 0.432E3 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 7 * dz ** 3
     # + 0.432E3 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 5 * dz ** 5 + 0.1
     #080E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 7 * dz ** 4 + 0.432E3 
     #* cc ** 5 * dt ** 5 * dx ** 5 * dy ** 6 * dz ** 5 + 0.576E3 * cc *
     #* 5 * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6 + 0.720E3 * cc ** 5 * 
     #dt ** 5 * dx ** 4 * dy ** 7 * dz ** 5 + 0.432E3 * cc ** 5 * dt ** 
     #5 * dx ** 3 * dy ** 7 * dz ** 6 - 0.4455E4 * cc ** 4 * dt ** 4 * d
     #x ** 7 * dy ** 7 * dz ** 3 - 0.4350E4 * cc ** 4 * dt ** 4 * dx ** 
     #7 * dy ** 5 * dz ** 5 - 0.4350E4 * cc ** 4 * dt ** 4 * dx ** 5 * d
     #y ** 7 * dz ** 5 - 0.5936E4 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 
     #7 * dz ** 4 - 0.2400E4 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 6 * d
     #z ** 5 - 0.2840E4 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 5 * dz ** 
     #6 - 0.2400E4 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 7 * dz ** 5 - 0
     #.2840E4 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 7 * dz ** 6 + 0.1296
     #0E5 * cc ** 2 * dt ** 2 * dx ** 7 * dy ** 7 * dz ** 5 + 0.3584E4 *
     # cc * dt * dx ** 7 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz *
     #* 7 / 0.17280E5
      cvv(367) = (-0.144E3 * cc ** 6 * dt ** 6 * dx ** 7 * dy ** 7 * dz 
     #- 0.144E3 * cc ** 6 * dt ** 6 * dx ** 7 * dy ** 5 * dz ** 3 - 0.14
     #4E3 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 7 * dz ** 3 - 0.504E3 * 
     #cc ** 5 * dt ** 5 * dx ** 7 * dy ** 7 * dz ** 2 - 0.108E3 * cc ** 
     #5 * dt ** 5 * dx ** 7 * dy ** 6 * dz ** 3 - 0.432E3 * cc ** 5 * dt
     # ** 5 * dx ** 7 * dy ** 5 * dz ** 4 - 0.108E3 * cc ** 5 * dt ** 5 
     #* dx ** 7 * dy ** 3 * dz ** 6 - 0.108E3 * cc ** 5 * dt ** 5 * dx *
     #* 6 * dy ** 7 * dz ** 3 - 0.432E3 * cc ** 5 * dt ** 5 * dx ** 5 * 
     #dy ** 7 * dz ** 4 - 0.144E3 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 
     #5 * dz ** 6 - 0.108E3 * cc ** 5 * dt ** 5 * dx ** 3 * dy ** 7 * dz
     # ** 6 + 0.1350E4 * cc ** 4 * dt ** 4 * dx ** 7 * dy ** 7 * dz ** 3
     # + 0.210E3 * cc ** 4 * dt ** 4 * dx ** 7 * dy ** 5 * dz ** 5 + 0.2
     #10E3 * cc ** 4 * dt ** 4 * dx ** 5 * dy ** 7 * dz ** 5 + 0.2968E4 
     #* cc ** 3 * dt ** 3 * dx ** 7 * dy ** 7 * dz ** 4 + 0.372E3 * cc *
     #* 3 * dt ** 3 * dx ** 7 * dy ** 6 * dz ** 5 + 0.900E3 * cc ** 3 * 
     #dt ** 3 * dx ** 7 * dy ** 5 * dz ** 6 + 0.372E3 * cc ** 3 * dt ** 
     #3 * dx ** 6 * dy ** 7 * dz ** 5 + 0.900E3 * cc ** 3 * dt ** 3 * dx
     # ** 5 * dy ** 7 * dz ** 6 - 0.1296E4 * cc ** 2 * dt ** 2 * dx ** 7
     # * dy ** 7 * dz ** 5 - 0.1792E4 * cc * dt * dx ** 7 * dy ** 7 * dz
     # ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(368) = (0.24E2 * cc ** 6 * dt ** 6 * dx ** 7 * dy ** 7 * dz + 
     #0.144E3 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 7 * dz ** 2 + 0.72E2
     # * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 5 * dz ** 4 + 0.72E2 * cc *
     #* 5 * dt ** 5 * dx ** 5 * dy ** 7 * dz ** 4 - 0.105E3 * cc ** 4 * 
     #dt ** 4 * dx ** 7 * dy ** 7 * dz ** 3 - 0.848E3 * cc ** 3 * dt ** 
     #3 * dx ** 7 * dy ** 7 * dz ** 4 - 0.32E2 * cc ** 3 * dt ** 3 * dx 
     #** 7 * dy ** 6 * dz ** 5 - 0.120E3 * cc ** 3 * dt ** 3 * dx ** 7 *
     # dy ** 5 * dz ** 6 - 0.32E2 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 
     #7 * dz ** 5 - 0.120E3 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 7 * dz
     # ** 6 + 0.96E2 * cc ** 2 * dt ** 2 * dx ** 7 * dy ** 7 * dz ** 5 +
     # 0.512E3 * cc * dt * dx ** 7 * dy ** 7 * dz ** 6) / dx ** 7 / dy *
     #* 7 / dz ** 7 / 0.17280E5
      cvv(369) = (-0.18E2 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 7 * dz *
     #* 2 + 0.106E3 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 7 * dz ** 4 - 
     #0.64E2 * cc * dt * dx ** 7 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 
     #7 / dz ** 7 / 0.17280E5
      cvv(371) = (-0.36E2 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 5 * dz *
     #* 4 + 0.16E2 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 6 * dz ** 5 + 0
     #.60E2 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7
     # / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(372) = (0.72E2 * cc ** 6 * dt ** 6 * dx ** 7 * dy ** 5 * dz **
     # 3 + 0.72E2 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 6 * dz ** 3 + 0.
     #216E3 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 5 * dz ** 4 + 0.72E2 *
     # cc ** 5 * dt ** 5 * dx ** 7 * dy ** 3 * dz ** 6 + 0.72E2 * cc ** 
     #5 * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6 - 0.105E3 * cc ** 4 * dt
     # ** 4 * dx ** 7 * dy ** 5 * dz ** 5 - 0.216E3 * cc ** 3 * dt ** 3 
     #* dx ** 7 * dy ** 6 * dz ** 5 - 0.480E3 * cc ** 3 * dt ** 3 * dx *
     #* 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(373) = (-0.288E3 * cc ** 6 * dt ** 6 * dx ** 7 * dy ** 5 * dz 
     #** 3 - 0.288E3 * cc ** 6 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 5 -
     # 0.288E3 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 5 - 0.288
     #E3 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 6 * dz ** 3 - 0.540E3 * c
     #c ** 5 * dt ** 5 * dx ** 7 * dy ** 5 * dz ** 4 - 0.540E3 * cc ** 5
     # * dt ** 5 * dx ** 7 * dy ** 4 * dz ** 5 - 0.288E3 * cc ** 5 * dt 
     #** 5 * dx ** 7 * dy ** 3 * dz ** 6 - 0.216E3 * cc ** 5 * dt ** 5 *
     # dx ** 6 * dy ** 5 * dz ** 5 - 0.288E3 * cc ** 5 * dt ** 5 * dx **
     # 5 * dy ** 6 * dz ** 5 - 0.288E3 * cc ** 5 * dt ** 5 * dx ** 5 * d
     #y ** 5 * dz ** 6 + 0.2280E4 * cc ** 4 * dt ** 4 * dx ** 7 * dy ** 
     #5 * dz ** 5 + 0.1620E4 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 6 * d
     #z ** 5 + 0.1620E4 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 5 * dz ** 
     #6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(374) = (0.432E3 * cc ** 6 * dt ** 6 * dx ** 7 * dy ** 5 * dz *
     #* 3 + 0.576E3 * cc ** 6 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 5 + 
     #0.360E3 * cc ** 6 * dt ** 6 * dx ** 7 * dy * dz ** 7 + 0.576E3 * c
     #c ** 6 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 5 + 0.576E3 * cc ** 6
     # * dt ** 6 * dx ** 5 * dy ** 3 * dz ** 7 + 0.432E3 * cc ** 6 * dt 
     #** 6 * dx ** 3 * dy ** 5 * dz ** 7 + 0.432E3 * cc ** 5 * dt ** 5 *
     # dx ** 7 * dy ** 6 * dz ** 3 + 0.720E3 * cc ** 5 * dt ** 5 * dx **
     # 7 * dy ** 5 * dz ** 4 + 0.1080E4 * cc ** 5 * dt ** 5 * dx ** 7 * 
     #dy ** 4 * dz ** 5 + 0.432E3 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 
     #3 * dz ** 6 + 0.1008E4 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 2 * d
     #z ** 7 + 0.432E3 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 5 * dz ** 5
     # + 0.432E3 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 3 * dz ** 7 + 0.5
     #76E3 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 6 * dz ** 5 + 0.432E3 *
     # cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6 + 0.1080E4 * cc *
     #* 5 * dt ** 5 * dx ** 5 * dy ** 4 * dz ** 7 + 0.720E3 * cc ** 5 * 
     #dt ** 5 * dx ** 4 * dy ** 5 * dz ** 7 + 0.432E3 * cc ** 5 * dt ** 
     #5 * dx ** 3 * dy ** 6 * dz ** 7 - 0.4350E4 * cc ** 4 * dt ** 4 * d
     #x ** 7 * dy ** 5 * dz ** 5 - 0.4455E4 * cc ** 4 * dt ** 4 * dx ** 
     #7 * dy ** 3 * dz ** 7 - 0.4350E4 * cc ** 4 * dt ** 4 * dx ** 5 * d
     #y ** 5 * dz ** 7 - 0.2840E4 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 
     #6 * dz ** 5 - 0.2400E4 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 5 * d
     #z ** 6 - 0.5936E4 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 4 * dz ** 
     #7 - 0.2400E4 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 5 * dz ** 7 - 0
     #.2840E4 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 6 * dz ** 7 + 0.1296
     #0E5 * cc ** 2 * dt ** 2 * dx ** 7 * dy ** 5 * dz ** 7 + 0.3584E4 *
     # cc * dt * dx ** 7 * dy ** 6 * dz ** 7) / dx ** 7 / dy ** 7 / dz *
     #* 7 / 0.17280E5
      cvv(375) = (-0.288E3 * cc ** 6 * dt ** 6 * dx ** 7 * dy ** 5 * dz 
     #** 3 - 0.288E3 * cc ** 6 * dt ** 6 * dx ** 7 * dy ** 3 * dz ** 5 -
     # 0.288E3 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 5 - 0.288
     #E3 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 6 * dz ** 3 - 0.540E3 * c
     #c ** 5 * dt ** 5 * dx ** 7 * dy ** 5 * dz ** 4 - 0.540E3 * cc ** 5
     # * dt ** 5 * dx ** 7 * dy ** 4 * dz ** 5 - 0.288E3 * cc ** 5 * dt 
     #** 5 * dx ** 7 * dy ** 3 * dz ** 6 - 0.216E3 * cc ** 5 * dt ** 5 *
     # dx ** 6 * dy ** 5 * dz ** 5 - 0.288E3 * cc ** 5 * dt ** 5 * dx **
     # 5 * dy ** 6 * dz ** 5 - 0.288E3 * cc ** 5 * dt ** 5 * dx ** 5 * d
     #y ** 5 * dz ** 6 + 0.2280E4 * cc ** 4 * dt ** 4 * dx ** 7 * dy ** 
     #5 * dz ** 5 + 0.1620E4 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 6 * d
     #z ** 5 + 0.1620E4 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 5 * dz ** 
     #6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(376) = (0.72E2 * cc ** 6 * dt ** 6 * dx ** 7 * dy ** 5 * dz **
     # 3 + 0.72E2 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 6 * dz ** 3 + 0.
     #216E3 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 5 * dz ** 4 + 0.72E2 *
     # cc ** 5 * dt ** 5 * dx ** 7 * dy ** 3 * dz ** 6 + 0.72E2 * cc ** 
     #5 * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6 - 0.105E3 * cc ** 4 * dt
     # ** 4 * dx ** 7 * dy ** 5 * dz ** 5 - 0.216E3 * cc ** 3 * dt ** 3 
     #* dx ** 7 * dy ** 6 * dz ** 5 - 0.480E3 * cc ** 3 * dt ** 3 * dx *
     #* 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(377) = (-0.36E2 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 5 * dz *
     #* 4 + 0.16E2 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 6 * dz ** 5 + 0
     #.60E2 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7
     # / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(381) = (-0.18E2 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 6 * dz *
     #* 3 - 0.18E2 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 3 * dz ** 6 + 0
     #.30E2 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 6 * dz ** 5 + 0.30E2 *
     # cc ** 3 * dt ** 3 * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy *
     #* 7 / dz ** 7 / 0.17280E5
      cvv(382) = (0.72E2 * cc ** 6 * dt ** 6 * dx ** 7 * dy ** 3 * dz **
     # 5 + 0.72E2 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 6 * dz ** 3 + 0.
     #216E3 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 4 * dz ** 5 + 0.72E2 *
     # cc ** 5 * dt ** 5 * dx ** 7 * dy ** 3 * dz ** 6 + 0.72E2 * cc ** 
     #5 * dt ** 5 * dx ** 5 * dy ** 6 * dz ** 5 - 0.105E3 * cc ** 4 * dt
     # ** 4 * dx ** 7 * dy ** 5 * dz ** 5 - 0.480E3 * cc ** 3 * dt ** 3 
     #* dx ** 7 * dy ** 6 * dz ** 5 - 0.216E3 * cc ** 3 * dt ** 3 * dx *
     #* 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(383) = (-0.144E3 * cc ** 6 * dt ** 6 * dx ** 7 * dy ** 3 * dz 
     #** 5 - 0.144E3 * cc ** 6 * dt ** 6 * dx ** 7 * dy * dz ** 7 - 0.14
     #4E3 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 3 * dz ** 7 - 0.108E3 * 
     #cc ** 5 * dt ** 5 * dx ** 7 * dy ** 6 * dz ** 3 - 0.432E3 * cc ** 
     #5 * dt ** 5 * dx ** 7 * dy ** 4 * dz ** 5 - 0.108E3 * cc ** 5 * dt
     # ** 5 * dx ** 7 * dy ** 3 * dz ** 6 - 0.504E3 * cc ** 5 * dt ** 5 
     #* dx ** 7 * dy ** 2 * dz ** 7 - 0.108E3 * cc ** 5 * dt ** 5 * dx *
     #* 6 * dy ** 3 * dz ** 7 - 0.144E3 * cc ** 5 * dt ** 5 * dx ** 5 * 
     #dy ** 6 * dz ** 5 - 0.432E3 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 
     #4 * dz ** 7 - 0.108E3 * cc ** 5 * dt ** 5 * dx ** 3 * dy ** 6 * dz
     # ** 7 + 0.210E3 * cc ** 4 * dt ** 4 * dx ** 7 * dy ** 5 * dz ** 5 
     #+ 0.1350E4 * cc ** 4 * dt ** 4 * dx ** 7 * dy ** 3 * dz ** 7 + 0.2
     #10E3 * cc ** 4 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 7 + 0.900E3 *
     # cc ** 3 * dt ** 3 * dx ** 7 * dy ** 6 * dz ** 5 + 0.372E3 * cc **
     # 3 * dt ** 3 * dx ** 7 * dy ** 5 * dz ** 6 + 0.2968E4 * cc ** 3 * 
     #dt ** 3 * dx ** 7 * dy ** 4 * dz ** 7 + 0.372E3 * cc ** 3 * dt ** 
     #3 * dx ** 6 * dy ** 5 * dz ** 7 + 0.900E3 * cc ** 3 * dt ** 3 * dx
     # ** 5 * dy ** 6 * dz ** 7 - 0.1296E4 * cc ** 2 * dt ** 2 * dx ** 7
     # * dy ** 5 * dz ** 7 - 0.1792E4 * cc * dt * dx ** 7 * dy ** 6 * dz
     # ** 7) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(384) = (0.72E2 * cc ** 6 * dt ** 6 * dx ** 7 * dy ** 3 * dz **
     # 5 + 0.72E2 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 6 * dz ** 3 + 0.
     #216E3 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 4 * dz ** 5 + 0.72E2 *
     # cc ** 5 * dt ** 5 * dx ** 7 * dy ** 3 * dz ** 6 + 0.72E2 * cc ** 
     #5 * dt ** 5 * dx ** 5 * dy ** 6 * dz ** 5 - 0.105E3 * cc ** 4 * dt
     # ** 4 * dx ** 7 * dy ** 5 * dz ** 5 - 0.480E3 * cc ** 3 * dt ** 3 
     #* dx ** 7 * dy ** 6 * dz ** 5 - 0.216E3 * cc ** 3 * dt ** 3 * dx *
     #* 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(385) = (-0.18E2 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 6 * dz *
     #* 3 - 0.18E2 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 3 * dz ** 6 + 0
     #.30E2 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 6 * dz ** 5 + 0.30E2 *
     # cc ** 3 * dt ** 3 * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7 / dy *
     #* 7 / dz ** 7 / 0.17280E5
      cvv(391) = (-0.36E2 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 4 * dz *
     #* 5 + 0.60E2 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 6 * dz ** 5 + 0
     #.16E2 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7
     # / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(392) = (0.24E2 * cc ** 6 * dt ** 6 * dx ** 7 * dy * dz ** 7 + 
     #0.72E2 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 4 * dz ** 5 + 0.144E3
     # * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 2 * dz ** 7 + 0.72E2 * cc *
     #* 5 * dt ** 5 * dx ** 5 * dy ** 4 * dz ** 7 - 0.105E3 * cc ** 4 * 
     #dt ** 4 * dx ** 7 * dy ** 3 * dz ** 7 - 0.120E3 * cc ** 3 * dt ** 
     #3 * dx ** 7 * dy ** 6 * dz ** 5 - 0.32E2 * cc ** 3 * dt ** 3 * dx 
     #** 7 * dy ** 5 * dz ** 6 - 0.848E3 * cc ** 3 * dt ** 3 * dx ** 7 *
     # dy ** 4 * dz ** 7 - 0.32E2 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 
     #5 * dz ** 7 - 0.120E3 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 6 * dz
     # ** 7 + 0.96E2 * cc ** 2 * dt ** 2 * dx ** 7 * dy ** 5 * dz ** 7 +
     # 0.512E3 * cc * dt * dx ** 7 * dy ** 6 * dz ** 7) / dx ** 7 / dy *
     #* 7 / dz ** 7 / 0.17280E5
      cvv(393) = (-0.36E2 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 4 * dz *
     #* 5 + 0.60E2 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 6 * dz ** 5 + 0
     #.16E2 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 5 * dz ** 6) / dx ** 7
     # / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(401) = (-0.18E2 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 2 * dz *
     #* 7 + 0.106E3 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 4 * dz ** 7 - 
     #0.64E2 * cc * dt * dx ** 7 * dy ** 6 * dz ** 7) / dx ** 7 / dy ** 
     #7 / dz ** 7 / 0.17280E5
      cvv(419) = (-0.36E2 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 4 * dz *
     #* 7 + 0.16E2 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 5 * dz ** 7 + 0
     #.60E2 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 6 * dz ** 7) / dx ** 7
     # / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(427) = -cc ** 5 * dt ** 5 / dx ** 2 / dz ** 2 / dy / 0.480E3
      cvv(428) = (0.72E2 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 3 * dz **
     # 7 + 0.72E2 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 3 * dz ** 7 + 0.
     #72E2 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 6 * dz ** 5 + 0.216E3 *
     # cc ** 5 * dt ** 5 * dx ** 5 * dy ** 4 * dz ** 7 + 0.72E2 * cc ** 
     #5 * dt ** 5 * dx ** 3 * dy ** 6 * dz ** 7 - 0.105E3 * cc ** 4 * dt
     # ** 4 * dx ** 5 * dy ** 5 * dz ** 7 - 0.216E3 * cc ** 3 * dt ** 3 
     #* dx ** 6 * dy ** 5 * dz ** 7 - 0.480E3 * cc ** 3 * dt ** 3 * dx *
     #* 5 * dy ** 6 * dz ** 7) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(429) = -cc ** 5 * dt ** 5 / dx ** 2 / dz ** 2 / dy / 0.480E3
      cvv(435) = -cc ** 5 * dt ** 5 / dx ** 2 / dy ** 2 / dz / 0.480E3
      cvv(436) = (0.144E3 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 5 * dz *
     #* 5 + 0.144E3 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 5 * dz ** 5 + 
     #0.144E3 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 6 * dz ** 5 + 0.144E
     #3 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / d
     #y ** 7 / dz ** 7 / 0.17280E5
      cvv(437) = (-0.288E3 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 5 * dz 
     #** 5 - 0.288E3 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 3 * dz ** 7 -
     # 0.288E3 * cc ** 6 * dt ** 6 * dx ** 3 * dy ** 5 * dz ** 7 - 0.288
     #E3 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 5 * dz ** 5 - 0.288E3 * c
     #c ** 5 * dt ** 5 * dx ** 6 * dy ** 3 * dz ** 7 - 0.288E3 * cc ** 5
     # * dt ** 5 * dx ** 5 * dy ** 6 * dz ** 5 - 0.216E3 * cc ** 5 * dt 
     #** 5 * dx ** 5 * dy ** 5 * dz ** 6 - 0.540E3 * cc ** 5 * dt ** 5 *
     # dx ** 5 * dy ** 4 * dz ** 7 - 0.540E3 * cc ** 5 * dt ** 5 * dx **
     # 4 * dy ** 5 * dz ** 7 - 0.288E3 * cc ** 5 * dt ** 5 * dx ** 3 * d
     #y ** 6 * dz ** 7 + 0.2280E4 * cc ** 4 * dt ** 4 * dx ** 5 * dy ** 
     #5 * dz ** 7 + 0.1620E4 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 5 * d
     #z ** 7 + 0.1620E4 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 6 * dz ** 
     #7) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(438) = (0.144E3 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 5 * dz *
     #* 5 + 0.144E3 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 5 * dz ** 5 + 
     #0.144E3 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 6 * dz ** 5 + 0.144E
     #3 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / d
     #y ** 7 / dz ** 7 / 0.17280E5
      cvv(439) = -cc ** 5 * dt ** 5 / dx ** 2 / dy ** 2 / dz / 0.480E3
      cvv(443) = (-0.36E2 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 7 * dz *
     #* 4 + 0.16E2 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 7 * dz ** 5 + 0
     #.60E2 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7
     # / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(444) = (0.72E2 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 7 * dz **
     # 3 + 0.72E2 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 7 * dz ** 3 + 0.
     #216E3 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 7 * dz ** 4 + 0.72E2 *
     # cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6 + 0.72E2 * cc ** 
     #5 * dt ** 5 * dx ** 3 * dy ** 7 * dz ** 6 - 0.105E3 * cc ** 4 * dt
     # ** 4 * dx ** 5 * dy ** 7 * dz ** 5 - 0.216E3 * cc ** 3 * dt ** 3 
     #* dx ** 6 * dy ** 7 * dz ** 5 - 0.480E3 * cc ** 3 * dt ** 3 * dx *
     #* 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(445) = (-0.288E3 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 7 * dz 
     #** 3 - 0.288E3 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 5 -
     # 0.288E3 * cc ** 6 * dt ** 6 * dx ** 3 * dy ** 7 * dz ** 5 - 0.288
     #E3 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 7 * dz ** 3 - 0.288E3 * c
     #c ** 5 * dt ** 5 * dx ** 6 * dy ** 5 * dz ** 5 - 0.540E3 * cc ** 5
     # * dt ** 5 * dx ** 5 * dy ** 7 * dz ** 4 - 0.216E3 * cc ** 5 * dt 
     #** 5 * dx ** 5 * dy ** 6 * dz ** 5 - 0.288E3 * cc ** 5 * dt ** 5 *
     # dx ** 5 * dy ** 5 * dz ** 6 - 0.540E3 * cc ** 5 * dt ** 5 * dx **
     # 4 * dy ** 7 * dz ** 5 - 0.288E3 * cc ** 5 * dt ** 5 * dx ** 3 * d
     #y ** 7 * dz ** 6 + 0.2280E4 * cc ** 4 * dt ** 4 * dx ** 5 * dy ** 
     #7 * dz ** 5 + 0.1620E4 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 7 * d
     #z ** 5 + 0.1620E4 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 7 * dz ** 
     #6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(446) = (0.432E3 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 7 * dz *
     #* 3 + 0.576E3 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 5 + 
     #0.432E3 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 3 * dz ** 7 + 0.576E
     #3 * cc ** 6 * dt ** 6 * dx ** 3 * dy ** 7 * dz ** 5 + 0.576E3 * cc
     # ** 6 * dt ** 6 * dx ** 3 * dy ** 5 * dz ** 7 + 0.360E3 * cc ** 6 
     #* dt ** 6 * dx * dy ** 7 * dz ** 7 + 0.432E3 * cc ** 5 * dt ** 5 *
     # dx ** 6 * dy ** 7 * dz ** 3 + 0.576E3 * cc ** 5 * dt ** 5 * dx **
     # 6 * dy ** 5 * dz ** 5 + 0.432E3 * cc ** 5 * dt ** 5 * dx ** 6 * d
     #y ** 3 * dz ** 7 + 0.720E3 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 7
     # * dz ** 4 + 0.432E3 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 6 * dz 
     #** 5 + 0.432E3 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6 +
     # 0.720E3 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 4 * dz ** 7 + 0.108
     #0E4 * cc ** 5 * dt ** 5 * dx ** 4 * dy ** 7 * dz ** 5 + 0.1080E4 *
     # cc ** 5 * dt ** 5 * dx ** 4 * dy ** 5 * dz ** 7 + 0.432E3 * cc **
     # 5 * dt ** 5 * dx ** 3 * dy ** 7 * dz ** 6 + 0.432E3 * cc ** 5 * d
     #t ** 5 * dx ** 3 * dy ** 6 * dz ** 7 + 0.1008E4 * cc ** 5 * dt ** 
     #5 * dx ** 2 * dy ** 7 * dz ** 7 - 0.4350E4 * cc ** 4 * dt ** 4 * d
     #x ** 5 * dy ** 7 * dz ** 5 - 0.4350E4 * cc ** 4 * dt ** 4 * dx ** 
     #5 * dy ** 5 * dz ** 7 - 0.4455E4 * cc ** 4 * dt ** 4 * dx ** 3 * d
     #y ** 7 * dz ** 7 - 0.2840E4 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 
     #7 * dz ** 5 - 0.2840E4 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 5 * d
     #z ** 7 - 0.2400E4 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 7 * dz ** 
     #6 - 0.2400E4 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 6 * dz ** 7 - 0
     #.5936E4 * cc ** 3 * dt ** 3 * dx ** 4 * dy ** 7 * dz ** 7 + 0.1296
     #0E5 * cc ** 2 * dt ** 2 * dx ** 5 * dy ** 7 * dz ** 7 + 0.3584E4 *
     # cc * dt * dx ** 6 * dy ** 7 * dz ** 7) / dx ** 7 / dy ** 7 / dz *
     #* 7 / 0.17280E5
      cvv(447) = (-0.288E3 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 7 * dz 
     #** 3 - 0.288E3 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 5 * dz ** 5 -
     # 0.288E3 * cc ** 6 * dt ** 6 * dx ** 3 * dy ** 7 * dz ** 5 - 0.288
     #E3 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 7 * dz ** 3 - 0.288E3 * c
     #c ** 5 * dt ** 5 * dx ** 6 * dy ** 5 * dz ** 5 - 0.540E3 * cc ** 5
     # * dt ** 5 * dx ** 5 * dy ** 7 * dz ** 4 - 0.216E3 * cc ** 5 * dt 
     #** 5 * dx ** 5 * dy ** 6 * dz ** 5 - 0.288E3 * cc ** 5 * dt ** 5 *
     # dx ** 5 * dy ** 5 * dz ** 6 - 0.540E3 * cc ** 5 * dt ** 5 * dx **
     # 4 * dy ** 7 * dz ** 5 - 0.288E3 * cc ** 5 * dt ** 5 * dx ** 3 * d
     #y ** 7 * dz ** 6 + 0.2280E4 * cc ** 4 * dt ** 4 * dx ** 5 * dy ** 
     #7 * dz ** 5 + 0.1620E4 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 7 * d
     #z ** 5 + 0.1620E4 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 7 * dz ** 
     #6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(448) = (0.72E2 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 7 * dz **
     # 3 + 0.72E2 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 7 * dz ** 3 + 0.
     #216E3 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 7 * dz ** 4 + 0.72E2 *
     # cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6 + 0.72E2 * cc ** 
     #5 * dt ** 5 * dx ** 3 * dy ** 7 * dz ** 6 - 0.105E3 * cc ** 4 * dt
     # ** 4 * dx ** 5 * dy ** 7 * dz ** 5 - 0.216E3 * cc ** 3 * dt ** 3 
     #* dx ** 6 * dy ** 7 * dz ** 5 - 0.480E3 * cc ** 3 * dt ** 3 * dx *
     #* 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(449) = (-0.36E2 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 7 * dz *
     #* 4 + 0.16E2 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 7 * dz ** 5 + 0
     #.60E2 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7
     # / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(453) = -cc ** 5 * dt ** 5 / dx ** 2 / dy ** 2 / dz / 0.480E3
      cvv(454) = (0.144E3 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 5 * dz *
     #* 5 + 0.144E3 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 5 * dz ** 5 + 
     #0.144E3 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 6 * dz ** 5 + 0.144E
     #3 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / d
     #y ** 7 / dz ** 7 / 0.17280E5
      cvv(455) = (-0.288E3 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 5 * dz 
     #** 5 - 0.288E3 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 3 * dz ** 7 -
     # 0.288E3 * cc ** 6 * dt ** 6 * dx ** 3 * dy ** 5 * dz ** 7 - 0.288
     #E3 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 5 * dz ** 5 - 0.288E3 * c
     #c ** 5 * dt ** 5 * dx ** 6 * dy ** 3 * dz ** 7 - 0.288E3 * cc ** 5
     # * dt ** 5 * dx ** 5 * dy ** 6 * dz ** 5 - 0.216E3 * cc ** 5 * dt 
     #** 5 * dx ** 5 * dy ** 5 * dz ** 6 - 0.540E3 * cc ** 5 * dt ** 5 *
     # dx ** 5 * dy ** 4 * dz ** 7 - 0.540E3 * cc ** 5 * dt ** 5 * dx **
     # 4 * dy ** 5 * dz ** 7 - 0.288E3 * cc ** 5 * dt ** 5 * dx ** 3 * d
     #y ** 6 * dz ** 7 + 0.2280E4 * cc ** 4 * dt ** 4 * dx ** 5 * dy ** 
     #5 * dz ** 7 + 0.1620E4 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 5 * d
     #z ** 7 + 0.1620E4 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 6 * dz ** 
     #7) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(456) = (0.144E3 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 5 * dz *
     #* 5 + 0.144E3 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 5 * dz ** 5 + 
     #0.144E3 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 6 * dz ** 5 + 0.144E
     #3 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 5 * dz ** 6) / dx ** 7 / d
     #y ** 7 / dz ** 7 / 0.17280E5
      cvv(457) = -cc ** 5 * dt ** 5 / dx ** 2 / dy ** 2 / dz / 0.480E3
      cvv(463) = -cc ** 5 * dt ** 5 / dx ** 2 / dz ** 2 / dy / 0.480E3
      cvv(464) = (0.72E2 * cc ** 6 * dt ** 6 * dx ** 5 * dy ** 3 * dz **
     # 7 + 0.72E2 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 3 * dz ** 7 + 0.
     #72E2 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 6 * dz ** 5 + 0.216E3 *
     # cc ** 5 * dt ** 5 * dx ** 5 * dy ** 4 * dz ** 7 + 0.72E2 * cc ** 
     #5 * dt ** 5 * dx ** 3 * dy ** 6 * dz ** 7 - 0.105E3 * cc ** 4 * dt
     # ** 4 * dx ** 5 * dy ** 5 * dz ** 7 - 0.216E3 * cc ** 3 * dt ** 3 
     #* dx ** 6 * dy ** 5 * dz ** 7 - 0.480E3 * cc ** 3 * dt ** 3 * dx *
     #* 5 * dy ** 6 * dz ** 7) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(465) = -cc ** 5 * dt ** 5 / dx ** 2 / dz ** 2 / dy / 0.480E3
      cvv(473) = (-0.36E2 * cc ** 5 * dt ** 5 * dx ** 5 * dy ** 4 * dz *
     #* 7 + 0.16E2 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 5 * dz ** 7 + 0
     #.60E2 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 6 * dz ** 7) / dx ** 7
     # / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(509) = (-0.18E2 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 3 * dz *
     #* 7 - 0.18E2 * cc ** 5 * dt ** 5 * dx ** 3 * dy ** 6 * dz ** 7 + 0
     #.30E2 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 5 * dz ** 7 + 0.30E2 *
     # cc ** 3 * dt ** 3 * dx ** 5 * dy ** 6 * dz ** 7) / dx ** 7 / dy *
     #* 7 / dz ** 7 / 0.17280E5
      cvv(517) = -cc ** 5 * dt ** 5 / dy ** 2 / dz ** 2 / dx / 0.480E3
      cvv(518) = (0.72E2 * cc ** 6 * dt ** 6 * dx ** 3 * dy ** 5 * dz **
     # 7 + 0.72E2 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 5 * dz ** 5 + 0.
     #72E2 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 3 * dz ** 7 + 0.216E3 *
     # cc ** 5 * dt ** 5 * dx ** 4 * dy ** 5 * dz ** 7 + 0.72E2 * cc ** 
     #5 * dt ** 5 * dx ** 3 * dy ** 6 * dz ** 7 - 0.105E3 * cc ** 4 * dt
     # ** 4 * dx ** 5 * dy ** 5 * dz ** 7 - 0.480E3 * cc ** 3 * dt ** 3 
     #* dx ** 6 * dy ** 5 * dz ** 7 - 0.216E3 * cc ** 3 * dt ** 3 * dx *
     #* 5 * dy ** 6 * dz ** 7) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(519) = -cc ** 5 * dt ** 5 / dy ** 2 / dz ** 2 / dx / 0.480E3
      cvv(525) = (-0.18E2 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 7 * dz *
     #* 3 - 0.18E2 * cc ** 5 * dt ** 5 * dx ** 3 * dy ** 7 * dz ** 6 + 0
     #.30E2 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 7 * dz ** 5 + 0.30E2 *
     # cc ** 3 * dt ** 3 * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy *
     #* 7 / dz ** 7 / 0.17280E5
      cvv(526) = (0.72E2 * cc ** 6 * dt ** 6 * dx ** 3 * dy ** 7 * dz **
     # 5 + 0.72E2 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 7 * dz ** 3 + 0.
     #72E2 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 5 * dz ** 5 + 0.216E3 *
     # cc ** 5 * dt ** 5 * dx ** 4 * dy ** 7 * dz ** 5 + 0.72E2 * cc ** 
     #5 * dt ** 5 * dx ** 3 * dy ** 7 * dz ** 6 - 0.105E3 * cc ** 4 * dt
     # ** 4 * dx ** 5 * dy ** 7 * dz ** 5 - 0.480E3 * cc ** 3 * dt ** 3 
     #* dx ** 6 * dy ** 7 * dz ** 5 - 0.216E3 * cc ** 3 * dt ** 3 * dx *
     #* 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(527) = (-0.144E3 * cc ** 6 * dt ** 6 * dx ** 3 * dy ** 7 * dz 
     #** 5 - 0.144E3 * cc ** 6 * dt ** 6 * dx ** 3 * dy ** 5 * dz ** 7 -
     # 0.144E3 * cc ** 6 * dt ** 6 * dx * dy ** 7 * dz ** 7 - 0.108E3 * 
     #cc ** 5 * dt ** 5 * dx ** 6 * dy ** 7 * dz ** 3 - 0.144E3 * cc ** 
     #5 * dt ** 5 * dx ** 6 * dy ** 5 * dz ** 5 - 0.108E3 * cc ** 5 * dt
     # ** 5 * dx ** 6 * dy ** 3 * dz ** 7 - 0.432E3 * cc ** 5 * dt ** 5 
     #* dx ** 4 * dy ** 7 * dz ** 5 - 0.432E3 * cc ** 5 * dt ** 5 * dx *
     #* 4 * dy ** 5 * dz ** 7 - 0.108E3 * cc ** 5 * dt ** 5 * dx ** 3 * 
     #dy ** 7 * dz ** 6 - 0.108E3 * cc ** 5 * dt ** 5 * dx ** 3 * dy ** 
     #6 * dz ** 7 - 0.504E3 * cc ** 5 * dt ** 5 * dx ** 2 * dy ** 7 * dz
     # ** 7 + 0.210E3 * cc ** 4 * dt ** 4 * dx ** 5 * dy ** 7 * dz ** 5 
     #+ 0.210E3 * cc ** 4 * dt ** 4 * dx ** 5 * dy ** 5 * dz ** 7 + 0.13
     #50E4 * cc ** 4 * dt ** 4 * dx ** 3 * dy ** 7 * dz ** 7 + 0.900E3 *
     # cc ** 3 * dt ** 3 * dx ** 6 * dy ** 7 * dz ** 5 + 0.900E3 * cc **
     # 3 * dt ** 3 * dx ** 6 * dy ** 5 * dz ** 7 + 0.372E3 * cc ** 3 * d
     #t ** 3 * dx ** 5 * dy ** 7 * dz ** 6 + 0.372E3 * cc ** 3 * dt ** 3
     # * dx ** 5 * dy ** 6 * dz ** 7 + 0.2968E4 * cc ** 3 * dt ** 3 * dx
     # ** 4 * dy ** 7 * dz ** 7 - 0.1296E4 * cc ** 2 * dt ** 2 * dx ** 5
     # * dy ** 7 * dz ** 7 - 0.1792E4 * cc * dt * dx ** 6 * dy ** 7 * dz
     # ** 7) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(528) = (0.72E2 * cc ** 6 * dt ** 6 * dx ** 3 * dy ** 7 * dz **
     # 5 + 0.72E2 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 7 * dz ** 3 + 0.
     #72E2 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 5 * dz ** 5 + 0.216E3 *
     # cc ** 5 * dt ** 5 * dx ** 4 * dy ** 7 * dz ** 5 + 0.72E2 * cc ** 
     #5 * dt ** 5 * dx ** 3 * dy ** 7 * dz ** 6 - 0.105E3 * cc ** 4 * dt
     # ** 4 * dx ** 5 * dy ** 7 * dz ** 5 - 0.480E3 * cc ** 3 * dt ** 3 
     #* dx ** 6 * dy ** 7 * dz ** 5 - 0.216E3 * cc ** 3 * dt ** 3 * dx *
     #* 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(529) = (-0.18E2 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 7 * dz *
     #* 3 - 0.18E2 * cc ** 5 * dt ** 5 * dx ** 3 * dy ** 7 * dz ** 6 + 0
     #.30E2 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 7 * dz ** 5 + 0.30E2 *
     # cc ** 3 * dt ** 3 * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7 / dy *
     #* 7 / dz ** 7 / 0.17280E5
      cvv(535) = -cc ** 5 * dt ** 5 / dy ** 2 / dz ** 2 / dx / 0.480E3
      cvv(536) = (0.72E2 * cc ** 6 * dt ** 6 * dx ** 3 * dy ** 5 * dz **
     # 7 + 0.72E2 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 5 * dz ** 5 + 0.
     #72E2 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 3 * dz ** 7 + 0.216E3 *
     # cc ** 5 * dt ** 5 * dx ** 4 * dy ** 5 * dz ** 7 + 0.72E2 * cc ** 
     #5 * dt ** 5 * dx ** 3 * dy ** 6 * dz ** 7 - 0.105E3 * cc ** 4 * dt
     # ** 4 * dx ** 5 * dy ** 5 * dz ** 7 - 0.480E3 * cc ** 3 * dt ** 3 
     #* dx ** 6 * dy ** 5 * dz ** 7 - 0.216E3 * cc ** 3 * dt ** 3 * dx *
     #* 5 * dy ** 6 * dz ** 7) / dx ** 7 / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(537) = -cc ** 5 * dt ** 5 / dy ** 2 / dz ** 2 / dx / 0.480E3
      cvv(545) = (-0.18E2 * cc ** 5 * dt ** 5 * dx ** 6 * dy ** 3 * dz *
     #* 7 - 0.18E2 * cc ** 5 * dt ** 5 * dx ** 3 * dy ** 6 * dz ** 7 + 0
     #.30E2 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 5 * dz ** 7 + 0.30E2 *
     # cc ** 3 * dt ** 3 * dx ** 5 * dy ** 6 * dz ** 7) / dx ** 7 / dy *
     #* 7 / dz ** 7 / 0.17280E5
      cvv(599) = (-0.36E2 * cc ** 5 * dt ** 5 * dx ** 4 * dy ** 5 * dz *
     #* 7 + 0.60E2 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 5 * dz ** 7 + 0
     #.16E2 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 6 * dz ** 7) / dx ** 7
     # / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(607) = (-0.36E2 * cc ** 5 * dt ** 5 * dx ** 4 * dy ** 7 * dz *
     #* 5 + 0.60E2 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 7 * dz ** 5 + 0
     #.16E2 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7
     # / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(608) = (0.24E2 * cc ** 6 * dt ** 6 * dx * dy ** 7 * dz ** 7 + 
     #0.72E2 * cc ** 5 * dt ** 5 * dx ** 4 * dy ** 7 * dz ** 5 + 0.72E2 
     #* cc ** 5 * dt ** 5 * dx ** 4 * dy ** 5 * dz ** 7 + 0.144E3 * cc *
     #* 5 * dt ** 5 * dx ** 2 * dy ** 7 * dz ** 7 - 0.105E3 * cc ** 4 * 
     #dt ** 4 * dx ** 3 * dy ** 7 * dz ** 7 - 0.120E3 * cc ** 3 * dt ** 
     #3 * dx ** 6 * dy ** 7 * dz ** 5 - 0.120E3 * cc ** 3 * dt ** 3 * dx
     # ** 6 * dy ** 5 * dz ** 7 - 0.32E2 * cc ** 3 * dt ** 3 * dx ** 5 *
     # dy ** 7 * dz ** 6 - 0.32E2 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 
     #6 * dz ** 7 - 0.848E3 * cc ** 3 * dt ** 3 * dx ** 4 * dy ** 7 * dz
     # ** 7 + 0.96E2 * cc ** 2 * dt ** 2 * dx ** 5 * dy ** 7 * dz ** 7 +
     # 0.512E3 * cc * dt * dx ** 6 * dy ** 7 * dz ** 7) / dx ** 7 / dy *
     #* 7 / dz ** 7 / 0.17280E5
      cvv(609) = (-0.36E2 * cc ** 5 * dt ** 5 * dx ** 4 * dy ** 7 * dz *
     #* 5 + 0.60E2 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 7 * dz ** 5 + 0
     #.16E2 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 7 * dz ** 6) / dx ** 7
     # / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(617) = (-0.36E2 * cc ** 5 * dt ** 5 * dx ** 4 * dy ** 5 * dz *
     #* 7 + 0.60E2 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 5 * dz ** 7 + 0
     #.16E2 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 6 * dz ** 7) / dx ** 7
     # / dy ** 7 / dz ** 7 / 0.17280E5
      cvv(689) = (-0.18E2 * cc ** 5 * dt ** 5 * dx ** 2 * dy ** 7 * dz *
     #* 7 + 0.106E3 * cc ** 3 * dt ** 3 * dx ** 4 * dy ** 7 * dz ** 7 - 
     #0.64E2 * cc * dt * dx ** 6 * dy ** 7 * dz ** 7) / dx ** 7 / dy ** 
     #7 / dz ** 7 / 0.17280E5
c
      return
      end
