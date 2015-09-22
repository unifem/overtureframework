      subroutine duWaveGen2d6rc( 
     *   nd1a,nd1b,nd2a,nd2b,
     *   n1a,n1b,n2a,n2b,
     *   ndf4a,ndf4b,nComp,addForcing,
     *   u,ut,unew,utnew,
     *   src,
     *   dx,dy,dt,cc,
     *   useWhereMask,mask )
c
      implicit none

c.. declarations of incoming variables
      integer nd1a,nd1b,nd2a,nd2b
      integer n1a,n1b,n2a,n2b
      integer ndf4a,ndf4b,nComp,addForcing
      integer useWhereMask
      integer mask(nd1a:nd1b,nd2a:nd2b)

      real u    (nd1a:nd1b,nd2a:nd2b)
      real ut   (nd1a:nd1b,nd2a:nd2b)
      real unew (nd1a:nd1b,nd2a:nd2b)
      real utnew(nd1a:nd1b,nd2a:nd2b)
      real src (nd1a:nd1b,nd2a:nd2b,ndf4a:ndf4b,-1:*)
      real dx,dy,dt,cc
c
c.. declarations of local variables
      integer i,j,ix,jy,n
c
      integer stencilOpt
c
      real cuu(-4:4,-4:4)
      real cuv(-4:4,-4:4)
      real cvu(-4:4,-4:4)
      real cvv(-4:4,-4:4)
c
      n = 1
      stencilOpt = 1
c
      if( n1a-nd1a .lt. 4 ) then
        write(6,*)'Grid not made with enough ghost cells'
        write(6,*)nd1a,n1a
        stop
      end if

      if( addForcing.eq.0 )then
        ! sixth order, cartesian, 2D
        if( stencilOpt .eq. 0 ) then
          if( useWhereMask.ne.0 ) then
            do j = n2a,n2b
            do i = n1a,n1b
            if( mask(i,j).gt.0 ) then
c
              call duStepWaveGen2d6rc( 
     *           nd1a,nd1b,nd2a,nd2b,
     *           n1a,n1b,n2a,n2b,
     *           u,ut,unew,utnew,
     *           dx,dy,dt,cc,
     *           i,j,n )
c
            end if
            end do
            end do
          else
            ! no mask
            do j = n2a,n2b
            do i = n1a,n1b
c
              call duStepWaveGen2d6rc( 
     *           nd1a,nd1b,nd2a,nd2b,
     *           n1a,n1b,n2a,n2b,
     *           u,ut,unew,utnew,
     *           dx,dy,dt,cc,
     *           i,j,n )
c     
            end do
            end do
          end if
        else
          ! stencil optimized routines
          call getcuu_sixth2D( cc,dx,dy,dt,cuu(-4,-4) )
          call getcuv_sixth2D( cc,dx,dy,dt,cuv(-4,-4) )
          call getcvu_sixth2D( cc,dx,dy,dt,cvu(-4,-4) )
          call getcvv_sixth2D( cc,dx,dy,dt,cvv(-4,-4) )

          if( useWhereMask.ne.0 ) then
            do j = n2a,n2b
            do i = n1a,n1b
            if( mask(i,j).gt.0 ) then
c     
              unew(i,j)  = 0.0
              utnew(i,j) = 0.0  

              do jy = -4,4
              do ix = -4+abs(jy),4-abs(jy)
                unew(i,j) = unew(i,j)+
     *             cuu(ix,jy)*u(i+ix,j+jy)+
     *             cuv(ix,jy)*ut(i+ix,j+jy)
c     
                utnew(i,j) = utnew(i,j)+
     *             cvu(ix,jy)*u(i+ix,j+jy)+
     *             cvv(ix,jy)*ut(i+ix,j+jy)
              end do
              end do
c     
            end if
            end do
            end do

          else
            ! no mask
            do j = n2a,n2b
            do i = n1a,n1b
c   
              unew(i,j)  = 0.0
              utnew(i,j) = 0.0  
c     
              do jy = -4,4
              do ix = -4+abs(jy),4-abs(jy)
                unew(i,j) = unew(i,j)+
     *             cuu(ix,jy)*u(i+ix,j+jy)+
     *             cuv(ix,jy)*ut(i+ix,j+jy)
c     
                utnew(i,j) = utnew(i,j)+
     *             cvu(ix,jy)*u(i+ix,j+jy)+
     *             cvv(ix,jy)*ut(i+ix,j+jy)
              end do
              end do

            end do
            end do
          end if
        end if
c
      else
        ! add forcing flag is set to true

        if( useWhereMask.ne.0 ) then
          do j = n2a,n2b
          do i = n1a,n1b
          if( mask(i,j).gt.0 ) then

            call duStepWaveGen2d6rc_tz( 
     *         nd1a,nd1b,nd2a,nd2b,
     *         n1a,n1b,n2a,n2b,
     *         ndf4a,ndf4b,nComp,
     *         u,ut,unew,utnew,
     *         src,
     *         dx,dy,dt,cc,
     *         i,j,n )
c     
          end if
          end do
          end do

        else
          ! no mask
          do j = n2a,n2b
          do i = n1a,n1b
            call duStepWaveGen2d6rc_tz( 
     *         nd1a,nd1b,nd2a,nd2b,
     *         n1a,n1b,n2a,n2b,
     *         ndf4a,ndf4b,nComp,
     *         u,ut,unew,utnew,
     *         src,
     *         dx,dy,dt,cc,
     *         i,j,n )
c     
          end do
          end do

        end if
      end if
c
      return
      end
c
c++++++++++++++++
c
      subroutine getcuu_sixth2D( 
     *   cc,dx,dy,dt,cuu )
c
      implicit real (t)
      real cuu(9,9)
      real dx,dy,dt,cc
c
      cuu(1,5) = dt ** 3 * cc ** 3 * (0.29E2 * dt ** 4 * cc ** 4 - 0.400
     #E3 * cc ** 2 * dt ** 2 * dx ** 2 + 0.1400E4 * dx ** 4) / dx ** 7 /
     # 0.288000E6
      cuu(2,4) = dt ** 3 * cc ** 3 * (0.87E2 * dt ** 4 * cc ** 4 - 0.600
     #E3 * cc ** 2 * dt ** 2 * dx ** 2 + 0.800E3 * dx ** 4) * (0.3E1 * d
     #x + dy) / dx ** 6 / dy ** 2 / 0.864000E6
      cuu(2,5) = -dt ** 2 * cc ** 2 * (0.261E3 * dt ** 5 * cc ** 5 * dx 
     #** 2 + 0.87E2 * cc ** 5 * dt ** 5 * dx * dy + 0.348E3 * cc ** 5 * 
     #dt ** 5 * dy ** 2 - 0.600E3 * cc ** 4 * dt ** 4 * dx * dy ** 2 - 0
     #.1800E4 * cc ** 3 * dt ** 3 * dx ** 4 - 0.600E3 * cc ** 3 * dt ** 
     #3 * dx ** 3 * dy - 0.4800E4 * dt ** 3 * cc ** 3 * dx ** 2 * dy ** 
     #2 + 0.3000E4 * cc ** 2 * dt ** 2 * dx ** 3 * dy ** 2 + 0.2400E4 * 
     #cc * dt * dx ** 6 + 0.800E3 * cc * dt * dx ** 5 * dy + 0.16800E5 *
     # cc * dt * dx ** 4 * dy ** 2 - 0.2400E4 * dx ** 5 * dy ** 2) / dx 
     #** 7 / dy ** 2 / 0.432000E6
      cuu(2,6) = dt ** 3 * cc ** 3 * (0.87E2 * dt ** 4 * cc ** 4 - 0.600
     #E3 * cc ** 2 * dt ** 2 * dx ** 2 + 0.800E3 * dx ** 4) * (0.3E1 * d
     #x + dy) / dx ** 6 / dy ** 2 / 0.864000E6
      cuu(3,3) = dt ** 3 * cc ** 3 * (0.261E3 * dt ** 4 * cc ** 4 - 0.60
     #0E3 * cc ** 2 * dt ** 2 * dx ** 2 - 0.600E3 * cc ** 2 * dt ** 2 * 
     #dy ** 2 + 0.1000E4 * dx ** 2 * dy ** 2) * (dx + dy) / dy ** 4 / dx
     # ** 4 / 0.864000E6
      cuu(3,4) = -dt ** 3 * cc ** 3 * (0.522E3 * cc ** 4 * dt ** 4 * dx 
     #** 3 + 0.522E3 * cc ** 4 * dt ** 4 * dx ** 2 * dy + 0.783E3 * cc *
     #* 4 * dt ** 4 * dx * dy ** 2 + 0.261E3 * cc ** 4 * dt ** 4 * dy **
     # 3 - 0.1800E4 * dt ** 3 * cc ** 3 * dx ** 2 * dy ** 2 - 0.1200E4 *
     # cc ** 2 * dt ** 2 * dx ** 5 - 0.1200E4 * cc ** 2 * dt ** 2 * dx *
     #* 4 * dy - 0.6600E4 * cc ** 2 * dt ** 2 * dx ** 3 * dy ** 2 - 0.30
     #00E4 * cc ** 2 * dt ** 2 * dx ** 2 * dy ** 3 + 0.3000E4 * cc * dt 
     #* dx ** 4 * dy ** 2 + 0.9200E4 * dx ** 5 * dy ** 2 + 0.4400E4 * dx
     # ** 4 * dy ** 3) / dx ** 6 / dy ** 4 / 0.432000E6
      cuu(3,5) = cc ** 2 * dt ** 2 * (0.261E3 * cc ** 5 * dt ** 5 * dx *
     #* 4 + 0.261E3 * cc ** 5 * dt ** 5 * dx ** 3 * dy + 0.522E3 * cc **
     # 5 * dt ** 5 * dx ** 2 * dy ** 2 + 0.174E3 * cc ** 5 * dt ** 5 * d
     #x * dy ** 3 + 0.406E3 * cc ** 5 * dt ** 5 * dy ** 4 - 0.1200E4 * c
     #c ** 4 * dt ** 4 * dx ** 3 * dy ** 2 - 0.1200E4 * cc ** 4 * dt ** 
     #4 * dx * dy ** 4 - 0.600E3 * cc ** 3 * dt ** 3 * dx ** 6 - 0.600E3
     # * cc ** 3 * dt ** 3 * dx ** 5 * dy - 0.4200E4 * cc ** 3 * dt ** 3
     # * dx ** 4 * dy ** 2 - 0.1800E4 * cc ** 3 * dt ** 3 * dx ** 3 * dy
     # ** 3 - 0.5600E4 * cc ** 3 * dt ** 3 * dx ** 2 * dy ** 4 + 0.2000E
     #4 * cc ** 2 * dt ** 2 * dx ** 5 * dy ** 2 + 0.12000E5 * cc ** 2 * 
     #dt ** 2 * dx ** 3 * dy ** 4 + 0.5800E4 * cc * dt * dx ** 6 * dy **
     # 2 + 0.2600E4 * cc * dt * dx ** 5 * dy ** 3 + 0.19600E5 * dt * cc 
     #* dx ** 4 * dy ** 4 - 0.10800E5 * dx ** 5 * dy ** 4) / dx ** 7 / d
     #y ** 4 / 0.144000E6
      cuu(3,6) = -dt ** 3 * cc ** 3 * (0.522E3 * cc ** 4 * dt ** 4 * dx 
     #** 3 + 0.522E3 * cc ** 4 * dt ** 4 * dx ** 2 * dy + 0.783E3 * cc *
     #* 4 * dt ** 4 * dx * dy ** 2 + 0.261E3 * cc ** 4 * dt ** 4 * dy **
     # 3 - 0.1800E4 * dt ** 3 * cc ** 3 * dx ** 2 * dy ** 2 - 0.1200E4 *
     # cc ** 2 * dt ** 2 * dx ** 5 - 0.1200E4 * cc ** 2 * dt ** 2 * dx *
     #* 4 * dy - 0.6600E4 * cc ** 2 * dt ** 2 * dx ** 3 * dy ** 2 - 0.30
     #00E4 * cc ** 2 * dt ** 2 * dx ** 2 * dy ** 3 + 0.3000E4 * cc * dt 
     #* dx ** 4 * dy ** 2 + 0.9200E4 * dx ** 5 * dy ** 2 + 0.4400E4 * dx
     # ** 4 * dy ** 3) / dx ** 6 / dy ** 4 / 0.432000E6
      cuu(3,7) = dt ** 3 * cc ** 3 * (0.261E3 * dt ** 4 * cc ** 4 - 0.60
     #0E3 * cc ** 2 * dt ** 2 * dx ** 2 - 0.600E3 * cc ** 2 * dt ** 2 * 
     #dy ** 2 + 0.1000E4 * dx ** 2 * dy ** 2) * (dx + dy) / dy ** 4 / dx
     # ** 4 / 0.864000E6
      cuu(4,2) = dt ** 3 * cc ** 3 * (0.87E2 * dt ** 4 * cc ** 4 - 0.600
     #E3 * cc ** 2 * dt ** 2 * dy ** 2 + 0.800E3 * dy ** 4) * (dx + 0.3E
     #1 * dy) / dy ** 6 / dx ** 2 / 0.864000E6
      cuu(4,3) = -dt ** 3 * cc ** 3 * (0.261E3 * cc ** 4 * dt ** 4 * dx 
     #** 3 + 0.783E3 * cc ** 4 * dt ** 4 * dx ** 2 * dy + 0.522E3 * cc *
     #* 4 * dt ** 4 * dx * dy ** 2 + 0.522E3 * cc ** 4 * dt ** 4 * dy **
     # 3 - 0.1800E4 * dt ** 3 * cc ** 3 * dx ** 2 * dy ** 2 - 0.3000E4 *
     # cc ** 2 * dt ** 2 * dx ** 3 * dy ** 2 - 0.6600E4 * cc ** 2 * dt *
     #* 2 * dx ** 2 * dy ** 3 - 0.1200E4 * dt ** 2 * cc ** 2 * dy ** 4 *
     # dx - 0.1200E4 * cc ** 2 * dt ** 2 * dy ** 5 + 0.3000E4 * cc * dt 
     #* dx ** 2 * dy ** 4 + 0.4400E4 * dx ** 3 * dy ** 4 + 0.9200E4 * dx
     # ** 2 * dy ** 5) / dy ** 6 / dx ** 4 / 0.432000E6
      cuu(4,4) = dt ** 3 * cc ** 3 * (0.1305E4 * cc ** 4 * dt ** 4 * dx 
     #** 5 + 0.3915E4 * cc ** 4 * dt ** 4 * dx ** 4 * dy + 0.4176E4 * cc
     # ** 4 * dt ** 4 * dx ** 3 * dy ** 2 + 0.4176E4 * cc ** 4 * dt ** 4
     # * dx ** 2 * dy ** 3 + 0.3915E4 * cc ** 4 * dt ** 4 * dx * dy ** 4
     # + 0.1305E4 * cc ** 4 * dt ** 4 * dy ** 5 - 0.14400E5 * cc ** 3 * 
     #dt ** 3 * dx ** 4 * dy ** 2 - 0.14400E5 * cc ** 3 * dt ** 3 * dx *
     #* 2 * dy ** 4 - 0.18600E5 * cc ** 2 * dt ** 2 * dx ** 5 * dy ** 2 
     #- 0.36600E5 * cc ** 2 * dt ** 2 * dx ** 4 * dy ** 3 - 0.36600E5 * 
     #cc ** 2 * dt ** 2 * dx ** 3 * dy ** 4 - 0.18600E5 * cc ** 2 * dt *
     #* 2 * dx ** 2 * dy ** 5 + 0.120000E6 * dt * cc * dx ** 4 * dy ** 4
     # + 0.64000E5 * dx ** 5 * dy ** 4 + 0.64000E5 * dy ** 5 * dx ** 4) 
     #/ dy ** 6 / dx ** 6 / 0.864000E6
      cuu(4,5) = -cc ** 2 * dt ** 2 * (0.870E3 * cc ** 5 * dt ** 5 * dx 
     #** 6 + 0.2610E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy + 0.3132E4 * cc
     # ** 5 * dt ** 5 * dx ** 4 * dy ** 2 + 0.3132E4 * cc ** 5 * dt ** 5
     # * dx ** 3 * dy ** 3 + 0.3915E4 * cc ** 5 * dt ** 5 * dx ** 2 * dy
     # ** 4 + 0.1305E4 * cc ** 5 * dt ** 5 * dx * dy ** 5 + 0.2436E4 * c
     #c ** 5 * dt ** 5 * dy ** 6 - 0.10800E5 * cc ** 4 * dt ** 4 * dx **
     # 5 * dy ** 2 - 0.14400E5 * cc ** 4 * dt ** 4 * dx ** 3 * dy ** 4 -
     # 0.9000E4 * cc ** 4 * dt ** 4 * dx * dy ** 6 - 0.13200E5 * cc ** 3
     # * dt ** 3 * dx ** 6 * dy ** 2 - 0.25200E5 * cc ** 3 * dt ** 3 * d
     #x ** 5 * dy ** 3 - 0.34200E5 * cc ** 3 * dt ** 3 * dx ** 4 * dy **
     # 4 - 0.16200E5 * cc ** 3 * dt ** 3 * dx ** 3 * dy ** 5 - 0.33600E5
     # * cc ** 3 * dt ** 3 * dx ** 2 * dy ** 6 + 0.114000E6 * cc ** 2 * 
     #dt ** 2 * dx ** 5 * dy ** 4 + 0.117000E6 * cc ** 2 * dt ** 2 * dx 
     #** 3 * dy ** 6 + 0.56000E5 * dt * cc * dx ** 6 * dy ** 4 + 0.48000
     #E5 * cc * dt * dx ** 5 * dy ** 5 + 0.117600E6 * cc * dt * dx ** 4 
     #* dy ** 6 - 0.324000E6 * dx ** 5 * dy ** 6) / dx ** 7 / dy ** 6 / 
     #0.432000E6
      cuu(4,6) = dt ** 3 * cc ** 3 * (0.1305E4 * cc ** 4 * dt ** 4 * dx 
     #** 5 + 0.3915E4 * cc ** 4 * dt ** 4 * dx ** 4 * dy + 0.4176E4 * cc
     # ** 4 * dt ** 4 * dx ** 3 * dy ** 2 + 0.4176E4 * cc ** 4 * dt ** 4
     # * dx ** 2 * dy ** 3 + 0.3915E4 * cc ** 4 * dt ** 4 * dx * dy ** 4
     # + 0.1305E4 * cc ** 4 * dt ** 4 * dy ** 5 - 0.14400E5 * cc ** 3 * 
     #dt ** 3 * dx ** 4 * dy ** 2 - 0.14400E5 * cc ** 3 * dt ** 3 * dx *
     #* 2 * dy ** 4 - 0.18600E5 * cc ** 2 * dt ** 2 * dx ** 5 * dy ** 2 
     #- 0.36600E5 * cc ** 2 * dt ** 2 * dx ** 4 * dy ** 3 - 0.36600E5 * 
     #cc ** 2 * dt ** 2 * dx ** 3 * dy ** 4 - 0.18600E5 * cc ** 2 * dt *
     #* 2 * dx ** 2 * dy ** 5 + 0.120000E6 * dt * cc * dx ** 4 * dy ** 4
     # + 0.64000E5 * dx ** 5 * dy ** 4 + 0.64000E5 * dy ** 5 * dx ** 4) 
     #/ dy ** 6 / dx ** 6 / 0.864000E6
      cuu(4,7) = -dt ** 3 * cc ** 3 * (0.261E3 * cc ** 4 * dt ** 4 * dx 
     #** 3 + 0.783E3 * cc ** 4 * dt ** 4 * dx ** 2 * dy + 0.522E3 * cc *
     #* 4 * dt ** 4 * dx * dy ** 2 + 0.522E3 * cc ** 4 * dt ** 4 * dy **
     # 3 - 0.1800E4 * dt ** 3 * cc ** 3 * dx ** 2 * dy ** 2 - 0.3000E4 *
     # cc ** 2 * dt ** 2 * dx ** 3 * dy ** 2 - 0.6600E4 * cc ** 2 * dt *
     #* 2 * dx ** 2 * dy ** 3 - 0.1200E4 * dt ** 2 * cc ** 2 * dy ** 4 *
     # dx - 0.1200E4 * cc ** 2 * dt ** 2 * dy ** 5 + 0.3000E4 * cc * dt 
     #* dx ** 2 * dy ** 4 + 0.4400E4 * dx ** 3 * dy ** 4 + 0.9200E4 * dx
     # ** 2 * dy ** 5) / dy ** 6 / dx ** 4 / 0.432000E6
      cuu(4,8) = dt ** 3 * cc ** 3 * (0.87E2 * dt ** 4 * cc ** 4 - 0.600
     #E3 * cc ** 2 * dt ** 2 * dy ** 2 + 0.800E3 * dy ** 4) * (dx + 0.3E
     #1 * dy) / dy ** 6 / dx ** 2 / 0.864000E6
      cuu(5,1) = dt ** 3 * cc ** 3 * (0.29E2 * dt ** 4 * cc ** 4 - 0.400
     #E3 * cc ** 2 * dt ** 2 * dy ** 2 + 0.1400E4 * dy ** 4) / dy ** 7 /
     # 0.288000E6
      cuu(5,2) = -dt ** 2 * cc ** 2 * (0.348E3 * dt ** 5 * cc ** 5 * dx 
     #** 2 + 0.87E2 * cc ** 5 * dt ** 5 * dx * dy + 0.261E3 * cc ** 5 * 
     #dt ** 5 * dy ** 2 - 0.600E3 * cc ** 4 * dt ** 4 * dx ** 2 * dy - 0
     #.4800E4 * dt ** 3 * cc ** 3 * dx ** 2 * dy ** 2 - 0.600E3 * cc ** 
     #3 * dt ** 3 * dx * dy ** 3 - 0.1800E4 * cc ** 3 * dt ** 3 * dy ** 
     #4 + 0.3000E4 * cc ** 2 * dt ** 2 * dx ** 2 * dy ** 3 + 0.16800E5 *
     # cc * dt * dx ** 2 * dy ** 4 + 0.800E3 * cc * dt * dx * dy ** 5 + 
     #0.2400E4 * cc * dt * dy ** 6 - 0.2400E4 * dx ** 2 * dy ** 5) / dy 
     #** 7 / dx ** 2 / 0.432000E6
      cuu(5,3) = cc ** 2 * dt ** 2 * (0.406E3 * cc ** 5 * dt ** 5 * dx *
     #* 4 + 0.174E3 * cc ** 5 * dt ** 5 * dx ** 3 * dy + 0.522E3 * cc **
     # 5 * dt ** 5 * dx ** 2 * dy ** 2 + 0.261E3 * cc ** 5 * dt ** 5 * d
     #x * dy ** 3 + 0.261E3 * cc ** 5 * dt ** 5 * dy ** 4 - 0.1200E4 * c
     #c ** 4 * dt ** 4 * dx ** 4 * dy - 0.1200E4 * cc ** 4 * dt ** 4 * d
     #x ** 2 * dy ** 3 - 0.5600E4 * cc ** 3 * dt ** 3 * dx ** 4 * dy ** 
     #2 - 0.1800E4 * cc ** 3 * dt ** 3 * dx ** 3 * dy ** 3 - 0.4200E4 * 
     #cc ** 3 * dt ** 3 * dx ** 2 * dy ** 4 - 0.600E3 * cc ** 3 * dt ** 
     #3 * dx * dy ** 5 - 0.600E3 * cc ** 3 * dt ** 3 * dy ** 6 + 0.12000
     #E5 * cc ** 2 * dt ** 2 * dx ** 4 * dy ** 3 + 0.2000E4 * cc ** 2 * 
     #dt ** 2 * dx ** 2 * dy ** 5 + 0.19600E5 * dt * cc * dx ** 4 * dy *
     #* 4 + 0.2600E4 * cc * dt * dx ** 3 * dy ** 5 + 0.5800E4 * cc * dt 
     #* dx ** 2 * dy ** 6 - 0.10800E5 * dy ** 5 * dx ** 4) / dy ** 7 / d
     #x ** 4 / 0.144000E6
      cuu(5,4) = -cc ** 2 * dt ** 2 * (0.2436E4 * cc ** 5 * dt ** 5 * dx
     # ** 6 + 0.1305E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy + 0.3915E4 * c
     #c ** 5 * dt ** 5 * dx ** 4 * dy ** 2 + 0.3132E4 * cc ** 5 * dt ** 
     #5 * dx ** 3 * dy ** 3 + 0.3132E4 * cc ** 5 * dt ** 5 * dx ** 2 * d
     #y ** 4 + 0.2610E4 * cc ** 5 * dt ** 5 * dx * dy ** 5 + 0.870E3 * c
     #c ** 5 * dt ** 5 * dy ** 6 - 0.9000E4 * cc ** 4 * dt ** 4 * dx ** 
     #6 * dy - 0.14400E5 * cc ** 4 * dt ** 4 * dx ** 4 * dy ** 3 - 0.108
     #00E5 * cc ** 4 * dt ** 4 * dx ** 2 * dy ** 5 - 0.33600E5 * cc ** 3
     # * dt ** 3 * dx ** 6 * dy ** 2 - 0.16200E5 * cc ** 3 * dt ** 3 * d
     #x ** 5 * dy ** 3 - 0.34200E5 * cc ** 3 * dt ** 3 * dx ** 4 * dy **
     # 4 - 0.25200E5 * cc ** 3 * dt ** 3 * dx ** 3 * dy ** 5 - 0.13200E5
     # * cc ** 3 * dt ** 3 * dx ** 2 * dy ** 6 + 0.117000E6 * cc ** 2 * 
     #dt ** 2 * dx ** 6 * dy ** 3 + 0.114000E6 * cc ** 2 * dt ** 2 * dx 
     #** 4 * dy ** 5 + 0.117600E6 * dt * cc * dx ** 6 * dy ** 4 + 0.4800
     #0E5 * cc * dt * dx ** 5 * dy ** 5 + 0.56000E5 * cc * dt * dx ** 4 
     #* dy ** 6 - 0.324000E6 * dx ** 6 * dy ** 5) / dx ** 6 / dy ** 7 / 
     #0.432000E6
      cuu(5,5) = (0.3045E4 * cc ** 7 * dt ** 7 * dx ** 7 + 0.1740E4 * cc
     # ** 7 * dt ** 7 * dx ** 6 * dy + 0.5220E4 * cc ** 7 * dt ** 7 * dx
     # ** 5 * dy ** 2 + 0.4698E4 * cc ** 7 * dt ** 7 * dx ** 4 * dy ** 3
     # + 0.4698E4 * cc ** 7 * dt ** 7 * dx ** 3 * dy ** 4 + 0.5220E4 * c
     #c ** 7 * dt ** 7 * dx ** 2 * dy ** 5 + 0.1740E4 * cc ** 7 * dt ** 
     #7 * dx * dy ** 6 + 0.3045E4 * cc ** 7 * dt ** 7 * dy ** 7 - 0.1200
     #0E5 * cc ** 6 * dt ** 6 * dx ** 7 * dy - 0.21600E5 * cc ** 6 * dt 
     #** 6 * dx ** 5 * dy ** 3 - 0.21600E5 * cc ** 6 * dt ** 6 * dx ** 3
     # * dy ** 5 - 0.12000E5 * cc ** 6 * dt ** 6 * dx * dy ** 7 - 0.4200
     #0E5 * cc ** 5 * dt ** 5 * dx ** 7 * dy ** 2 - 0.22800E5 * cc ** 5 
     #* dt ** 5 * dx ** 6 * dy ** 3 - 0.46800E5 * cc ** 5 * dt ** 5 * dx
     # ** 5 * dy ** 4 - 0.46800E5 * cc ** 5 * dt ** 5 * dx ** 4 * dy ** 
     #5 - 0.22800E5 * cc ** 5 * dt ** 5 * dx ** 3 * dy ** 6 - 0.42000E5 
     #* cc ** 5 * dt ** 5 * dx ** 2 * dy ** 7 + 0.168000E6 * cc ** 4 * d
     #t ** 4 * dx ** 7 * dy ** 3 + 0.216000E6 * cc ** 4 * dt ** 4 * dx *
     #* 5 * dy ** 5 + 0.168000E6 * cc ** 4 * dt ** 4 * dx ** 3 * dy ** 7
     # + 0.147000E6 * cc ** 3 * dt ** 3 * dx ** 7 * dy ** 4 + 0.82000E5 
     #* cc ** 3 * dt ** 3 * dx ** 6 * dy ** 5 + 0.82000E5 * cc ** 3 * dt
     # ** 3 * dx ** 5 * dy ** 6 + 0.147000E6 * cc ** 3 * dt ** 3 * dx **
     # 4 * dy ** 7 - 0.588000E6 * cc ** 2 * dt ** 2 * dx ** 7 * dy ** 5 
     #- 0.588000E6 * cc ** 2 * dt ** 2 * dx ** 5 * dy ** 7 + 0.432000E6 
     #* dx ** 7 * dy ** 7) / dx ** 7 / dy ** 7 / 0.432000E6
      cuu(5,6) = -cc ** 2 * dt ** 2 * (0.2436E4 * cc ** 5 * dt ** 5 * dx
     # ** 6 + 0.1305E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy + 0.3915E4 * c
     #c ** 5 * dt ** 5 * dx ** 4 * dy ** 2 + 0.3132E4 * cc ** 5 * dt ** 
     #5 * dx ** 3 * dy ** 3 + 0.3132E4 * cc ** 5 * dt ** 5 * dx ** 2 * d
     #y ** 4 + 0.2610E4 * cc ** 5 * dt ** 5 * dx * dy ** 5 + 0.870E3 * c
     #c ** 5 * dt ** 5 * dy ** 6 - 0.9000E4 * cc ** 4 * dt ** 4 * dx ** 
     #6 * dy - 0.14400E5 * cc ** 4 * dt ** 4 * dx ** 4 * dy ** 3 - 0.108
     #00E5 * cc ** 4 * dt ** 4 * dx ** 2 * dy ** 5 - 0.33600E5 * cc ** 3
     # * dt ** 3 * dx ** 6 * dy ** 2 - 0.16200E5 * cc ** 3 * dt ** 3 * d
     #x ** 5 * dy ** 3 - 0.34200E5 * cc ** 3 * dt ** 3 * dx ** 4 * dy **
     # 4 - 0.25200E5 * cc ** 3 * dt ** 3 * dx ** 3 * dy ** 5 - 0.13200E5
     # * cc ** 3 * dt ** 3 * dx ** 2 * dy ** 6 + 0.117000E6 * cc ** 2 * 
     #dt ** 2 * dx ** 6 * dy ** 3 + 0.114000E6 * cc ** 2 * dt ** 2 * dx 
     #** 4 * dy ** 5 + 0.117600E6 * dt * cc * dx ** 6 * dy ** 4 + 0.4800
     #0E5 * cc * dt * dx ** 5 * dy ** 5 + 0.56000E5 * cc * dt * dx ** 4 
     #* dy ** 6 - 0.324000E6 * dx ** 6 * dy ** 5) / dx ** 6 / dy ** 7 / 
     #0.432000E6
      cuu(5,7) = cc ** 2 * dt ** 2 * (0.406E3 * cc ** 5 * dt ** 5 * dx *
     #* 4 + 0.174E3 * cc ** 5 * dt ** 5 * dx ** 3 * dy + 0.522E3 * cc **
     # 5 * dt ** 5 * dx ** 2 * dy ** 2 + 0.261E3 * cc ** 5 * dt ** 5 * d
     #x * dy ** 3 + 0.261E3 * cc ** 5 * dt ** 5 * dy ** 4 - 0.1200E4 * c
     #c ** 4 * dt ** 4 * dx ** 4 * dy - 0.1200E4 * cc ** 4 * dt ** 4 * d
     #x ** 2 * dy ** 3 - 0.5600E4 * cc ** 3 * dt ** 3 * dx ** 4 * dy ** 
     #2 - 0.1800E4 * cc ** 3 * dt ** 3 * dx ** 3 * dy ** 3 - 0.4200E4 * 
     #cc ** 3 * dt ** 3 * dx ** 2 * dy ** 4 - 0.600E3 * cc ** 3 * dt ** 
     #3 * dx * dy ** 5 - 0.600E3 * cc ** 3 * dt ** 3 * dy ** 6 + 0.12000
     #E5 * cc ** 2 * dt ** 2 * dx ** 4 * dy ** 3 + 0.2000E4 * cc ** 2 * 
     #dt ** 2 * dx ** 2 * dy ** 5 + 0.19600E5 * dt * cc * dx ** 4 * dy *
     #* 4 + 0.2600E4 * cc * dt * dx ** 3 * dy ** 5 + 0.5800E4 * cc * dt 
     #* dx ** 2 * dy ** 6 - 0.10800E5 * dy ** 5 * dx ** 4) / dy ** 7 / d
     #x ** 4 / 0.144000E6
      cuu(5,8) = -dt ** 2 * cc ** 2 * (0.348E3 * dt ** 5 * cc ** 5 * dx 
     #** 2 + 0.87E2 * cc ** 5 * dt ** 5 * dx * dy + 0.261E3 * cc ** 5 * 
     #dt ** 5 * dy ** 2 - 0.600E3 * cc ** 4 * dt ** 4 * dx ** 2 * dy - 0
     #.4800E4 * dt ** 3 * cc ** 3 * dx ** 2 * dy ** 2 - 0.600E3 * cc ** 
     #3 * dt ** 3 * dx * dy ** 3 - 0.1800E4 * cc ** 3 * dt ** 3 * dy ** 
     #4 + 0.3000E4 * cc ** 2 * dt ** 2 * dx ** 2 * dy ** 3 + 0.16800E5 *
     # cc * dt * dx ** 2 * dy ** 4 + 0.800E3 * cc * dt * dx * dy ** 5 + 
     #0.2400E4 * cc * dt * dy ** 6 - 0.2400E4 * dx ** 2 * dy ** 5) / dy 
     #** 7 / dx ** 2 / 0.432000E6
      cuu(5,9) = dt ** 3 * cc ** 3 * (0.29E2 * dt ** 4 * cc ** 4 - 0.400
     #E3 * cc ** 2 * dt ** 2 * dy ** 2 + 0.1400E4 * dy ** 4) / dy ** 7 /
     # 0.288000E6
      cuu(6,2) = dt ** 3 * cc ** 3 * (0.87E2 * dt ** 4 * cc ** 4 - 0.600
     #E3 * cc ** 2 * dt ** 2 * dy ** 2 + 0.800E3 * dy ** 4) * (dx + 0.3E
     #1 * dy) / dy ** 6 / dx ** 2 / 0.864000E6
      cuu(6,3) = -dt ** 3 * cc ** 3 * (0.261E3 * cc ** 4 * dt ** 4 * dx 
     #** 3 + 0.783E3 * cc ** 4 * dt ** 4 * dx ** 2 * dy + 0.522E3 * cc *
     #* 4 * dt ** 4 * dx * dy ** 2 + 0.522E3 * cc ** 4 * dt ** 4 * dy **
     # 3 - 0.1800E4 * dt ** 3 * cc ** 3 * dx ** 2 * dy ** 2 - 0.3000E4 *
     # cc ** 2 * dt ** 2 * dx ** 3 * dy ** 2 - 0.6600E4 * cc ** 2 * dt *
     #* 2 * dx ** 2 * dy ** 3 - 0.1200E4 * dt ** 2 * cc ** 2 * dy ** 4 *
     # dx - 0.1200E4 * cc ** 2 * dt ** 2 * dy ** 5 + 0.3000E4 * cc * dt 
     #* dx ** 2 * dy ** 4 + 0.4400E4 * dx ** 3 * dy ** 4 + 0.9200E4 * dx
     # ** 2 * dy ** 5) / dy ** 6 / dx ** 4 / 0.432000E6
      cuu(6,4) = dt ** 3 * cc ** 3 * (0.1305E4 * cc ** 4 * dt ** 4 * dx 
     #** 5 + 0.3915E4 * cc ** 4 * dt ** 4 * dx ** 4 * dy + 0.4176E4 * cc
     # ** 4 * dt ** 4 * dx ** 3 * dy ** 2 + 0.4176E4 * cc ** 4 * dt ** 4
     # * dx ** 2 * dy ** 3 + 0.3915E4 * cc ** 4 * dt ** 4 * dx * dy ** 4
     # + 0.1305E4 * cc ** 4 * dt ** 4 * dy ** 5 - 0.14400E5 * cc ** 3 * 
     #dt ** 3 * dx ** 4 * dy ** 2 - 0.14400E5 * cc ** 3 * dt ** 3 * dx *
     #* 2 * dy ** 4 - 0.18600E5 * cc ** 2 * dt ** 2 * dx ** 5 * dy ** 2 
     #- 0.36600E5 * cc ** 2 * dt ** 2 * dx ** 4 * dy ** 3 - 0.36600E5 * 
     #cc ** 2 * dt ** 2 * dx ** 3 * dy ** 4 - 0.18600E5 * cc ** 2 * dt *
     #* 2 * dx ** 2 * dy ** 5 + 0.120000E6 * dt * cc * dx ** 4 * dy ** 4
     # + 0.64000E5 * dx ** 5 * dy ** 4 + 0.64000E5 * dy ** 5 * dx ** 4) 
     #/ dy ** 6 / dx ** 6 / 0.864000E6
      cuu(6,5) = -cc ** 2 * dt ** 2 * (0.870E3 * cc ** 5 * dt ** 5 * dx 
     #** 6 + 0.2610E4 * cc ** 5 * dt ** 5 * dx ** 5 * dy + 0.3132E4 * cc
     # ** 5 * dt ** 5 * dx ** 4 * dy ** 2 + 0.3132E4 * cc ** 5 * dt ** 5
     # * dx ** 3 * dy ** 3 + 0.3915E4 * cc ** 5 * dt ** 5 * dx ** 2 * dy
     # ** 4 + 0.1305E4 * cc ** 5 * dt ** 5 * dx * dy ** 5 + 0.2436E4 * c
     #c ** 5 * dt ** 5 * dy ** 6 - 0.10800E5 * cc ** 4 * dt ** 4 * dx **
     # 5 * dy ** 2 - 0.14400E5 * cc ** 4 * dt ** 4 * dx ** 3 * dy ** 4 -
     # 0.9000E4 * cc ** 4 * dt ** 4 * dx * dy ** 6 - 0.13200E5 * cc ** 3
     # * dt ** 3 * dx ** 6 * dy ** 2 - 0.25200E5 * cc ** 3 * dt ** 3 * d
     #x ** 5 * dy ** 3 - 0.34200E5 * cc ** 3 * dt ** 3 * dx ** 4 * dy **
     # 4 - 0.16200E5 * cc ** 3 * dt ** 3 * dx ** 3 * dy ** 5 - 0.33600E5
     # * cc ** 3 * dt ** 3 * dx ** 2 * dy ** 6 + 0.114000E6 * cc ** 2 * 
     #dt ** 2 * dx ** 5 * dy ** 4 + 0.117000E6 * cc ** 2 * dt ** 2 * dx 
     #** 3 * dy ** 6 + 0.56000E5 * dt * cc * dx ** 6 * dy ** 4 + 0.48000
     #E5 * cc * dt * dx ** 5 * dy ** 5 + 0.117600E6 * cc * dt * dx ** 4 
     #* dy ** 6 - 0.324000E6 * dx ** 5 * dy ** 6) / dx ** 7 / dy ** 6 / 
     #0.432000E6
      cuu(6,6) = dt ** 3 * cc ** 3 * (0.1305E4 * cc ** 4 * dt ** 4 * dx 
     #** 5 + 0.3915E4 * cc ** 4 * dt ** 4 * dx ** 4 * dy + 0.4176E4 * cc
     # ** 4 * dt ** 4 * dx ** 3 * dy ** 2 + 0.4176E4 * cc ** 4 * dt ** 4
     # * dx ** 2 * dy ** 3 + 0.3915E4 * cc ** 4 * dt ** 4 * dx * dy ** 4
     # + 0.1305E4 * cc ** 4 * dt ** 4 * dy ** 5 - 0.14400E5 * cc ** 3 * 
     #dt ** 3 * dx ** 4 * dy ** 2 - 0.14400E5 * cc ** 3 * dt ** 3 * dx *
     #* 2 * dy ** 4 - 0.18600E5 * cc ** 2 * dt ** 2 * dx ** 5 * dy ** 2 
     #- 0.36600E5 * cc ** 2 * dt ** 2 * dx ** 4 * dy ** 3 - 0.36600E5 * 
     #cc ** 2 * dt ** 2 * dx ** 3 * dy ** 4 - 0.18600E5 * cc ** 2 * dt *
     #* 2 * dx ** 2 * dy ** 5 + 0.120000E6 * dt * cc * dx ** 4 * dy ** 4
     # + 0.64000E5 * dx ** 5 * dy ** 4 + 0.64000E5 * dy ** 5 * dx ** 4) 
     #/ dy ** 6 / dx ** 6 / 0.864000E6
      cuu(6,7) = -dt ** 3 * cc ** 3 * (0.261E3 * cc ** 4 * dt ** 4 * dx 
     #** 3 + 0.783E3 * cc ** 4 * dt ** 4 * dx ** 2 * dy + 0.522E3 * cc *
     #* 4 * dt ** 4 * dx * dy ** 2 + 0.522E3 * cc ** 4 * dt ** 4 * dy **
     # 3 - 0.1800E4 * dt ** 3 * cc ** 3 * dx ** 2 * dy ** 2 - 0.3000E4 *
     # cc ** 2 * dt ** 2 * dx ** 3 * dy ** 2 - 0.6600E4 * cc ** 2 * dt *
     #* 2 * dx ** 2 * dy ** 3 - 0.1200E4 * dt ** 2 * cc ** 2 * dy ** 4 *
     # dx - 0.1200E4 * cc ** 2 * dt ** 2 * dy ** 5 + 0.3000E4 * cc * dt 
     #* dx ** 2 * dy ** 4 + 0.4400E4 * dx ** 3 * dy ** 4 + 0.9200E4 * dx
     # ** 2 * dy ** 5) / dy ** 6 / dx ** 4 / 0.432000E6
      cuu(6,8) = dt ** 3 * cc ** 3 * (0.87E2 * dt ** 4 * cc ** 4 - 0.600
     #E3 * cc ** 2 * dt ** 2 * dy ** 2 + 0.800E3 * dy ** 4) * (dx + 0.3E
     #1 * dy) / dy ** 6 / dx ** 2 / 0.864000E6
      cuu(7,3) = dt ** 3 * cc ** 3 * (0.261E3 * dt ** 4 * cc ** 4 - 0.60
     #0E3 * cc ** 2 * dt ** 2 * dx ** 2 - 0.600E3 * cc ** 2 * dt ** 2 * 
     #dy ** 2 + 0.1000E4 * dx ** 2 * dy ** 2) * (dx + dy) / dy ** 4 / dx
     # ** 4 / 0.864000E6
      cuu(7,4) = -dt ** 3 * cc ** 3 * (0.522E3 * cc ** 4 * dt ** 4 * dx 
     #** 3 + 0.522E3 * cc ** 4 * dt ** 4 * dx ** 2 * dy + 0.783E3 * cc *
     #* 4 * dt ** 4 * dx * dy ** 2 + 0.261E3 * cc ** 4 * dt ** 4 * dy **
     # 3 - 0.1800E4 * dt ** 3 * cc ** 3 * dx ** 2 * dy ** 2 - 0.1200E4 *
     # cc ** 2 * dt ** 2 * dx ** 5 - 0.1200E4 * cc ** 2 * dt ** 2 * dx *
     #* 4 * dy - 0.6600E4 * cc ** 2 * dt ** 2 * dx ** 3 * dy ** 2 - 0.30
     #00E4 * cc ** 2 * dt ** 2 * dx ** 2 * dy ** 3 + 0.3000E4 * cc * dt 
     #* dx ** 4 * dy ** 2 + 0.9200E4 * dx ** 5 * dy ** 2 + 0.4400E4 * dx
     # ** 4 * dy ** 3) / dx ** 6 / dy ** 4 / 0.432000E6
      cuu(7,5) = cc ** 2 * dt ** 2 * (0.261E3 * cc ** 5 * dt ** 5 * dx *
     #* 4 + 0.261E3 * cc ** 5 * dt ** 5 * dx ** 3 * dy + 0.522E3 * cc **
     # 5 * dt ** 5 * dx ** 2 * dy ** 2 + 0.174E3 * cc ** 5 * dt ** 5 * d
     #x * dy ** 3 + 0.406E3 * cc ** 5 * dt ** 5 * dy ** 4 - 0.1200E4 * c
     #c ** 4 * dt ** 4 * dx ** 3 * dy ** 2 - 0.1200E4 * cc ** 4 * dt ** 
     #4 * dx * dy ** 4 - 0.600E3 * cc ** 3 * dt ** 3 * dx ** 6 - 0.600E3
     # * cc ** 3 * dt ** 3 * dx ** 5 * dy - 0.4200E4 * cc ** 3 * dt ** 3
     # * dx ** 4 * dy ** 2 - 0.1800E4 * cc ** 3 * dt ** 3 * dx ** 3 * dy
     # ** 3 - 0.5600E4 * cc ** 3 * dt ** 3 * dx ** 2 * dy ** 4 + 0.2000E
     #4 * cc ** 2 * dt ** 2 * dx ** 5 * dy ** 2 + 0.12000E5 * cc ** 2 * 
     #dt ** 2 * dx ** 3 * dy ** 4 + 0.5800E4 * cc * dt * dx ** 6 * dy **
     # 2 + 0.2600E4 * cc * dt * dx ** 5 * dy ** 3 + 0.19600E5 * dt * cc 
     #* dx ** 4 * dy ** 4 - 0.10800E5 * dx ** 5 * dy ** 4) / dx ** 7 / d
     #y ** 4 / 0.144000E6
      cuu(7,6) = -dt ** 3 * cc ** 3 * (0.522E3 * cc ** 4 * dt ** 4 * dx 
     #** 3 + 0.522E3 * cc ** 4 * dt ** 4 * dx ** 2 * dy + 0.783E3 * cc *
     #* 4 * dt ** 4 * dx * dy ** 2 + 0.261E3 * cc ** 4 * dt ** 4 * dy **
     # 3 - 0.1800E4 * dt ** 3 * cc ** 3 * dx ** 2 * dy ** 2 - 0.1200E4 *
     # cc ** 2 * dt ** 2 * dx ** 5 - 0.1200E4 * cc ** 2 * dt ** 2 * dx *
     #* 4 * dy - 0.6600E4 * cc ** 2 * dt ** 2 * dx ** 3 * dy ** 2 - 0.30
     #00E4 * cc ** 2 * dt ** 2 * dx ** 2 * dy ** 3 + 0.3000E4 * cc * dt 
     #* dx ** 4 * dy ** 2 + 0.9200E4 * dx ** 5 * dy ** 2 + 0.4400E4 * dx
     # ** 4 * dy ** 3) / dx ** 6 / dy ** 4 / 0.432000E6
      cuu(7,7) = dt ** 3 * cc ** 3 * (0.261E3 * dt ** 4 * cc ** 4 - 0.60
     #0E3 * cc ** 2 * dt ** 2 * dx ** 2 - 0.600E3 * cc ** 2 * dt ** 2 * 
     #dy ** 2 + 0.1000E4 * dx ** 2 * dy ** 2) * (dx + dy) / dy ** 4 / dx
     # ** 4 / 0.864000E6
      cuu(8,4) = dt ** 3 * cc ** 3 * (0.87E2 * dt ** 4 * cc ** 4 - 0.600
     #E3 * cc ** 2 * dt ** 2 * dx ** 2 + 0.800E3 * dx ** 4) * (0.3E1 * d
     #x + dy) / dx ** 6 / dy ** 2 / 0.864000E6
      cuu(8,5) = -dt ** 2 * cc ** 2 * (0.261E3 * dt ** 5 * cc ** 5 * dx 
     #** 2 + 0.87E2 * cc ** 5 * dt ** 5 * dx * dy + 0.348E3 * cc ** 5 * 
     #dt ** 5 * dy ** 2 - 0.600E3 * cc ** 4 * dt ** 4 * dx * dy ** 2 - 0
     #.1800E4 * cc ** 3 * dt ** 3 * dx ** 4 - 0.600E3 * cc ** 3 * dt ** 
     #3 * dx ** 3 * dy - 0.4800E4 * dt ** 3 * cc ** 3 * dx ** 2 * dy ** 
     #2 + 0.3000E4 * cc ** 2 * dt ** 2 * dx ** 3 * dy ** 2 + 0.2400E4 * 
     #cc * dt * dx ** 6 + 0.800E3 * cc * dt * dx ** 5 * dy + 0.16800E5 *
     # cc * dt * dx ** 4 * dy ** 2 - 0.2400E4 * dx ** 5 * dy ** 2) / dx 
     #** 7 / dy ** 2 / 0.432000E6
      cuu(8,6) = dt ** 3 * cc ** 3 * (0.87E2 * dt ** 4 * cc ** 4 - 0.600
     #E3 * cc ** 2 * dt ** 2 * dx ** 2 + 0.800E3 * dx ** 4) * (0.3E1 * d
     #x + dy) / dx ** 6 / dy ** 2 / 0.864000E6
      cuu(9,5) = dt ** 3 * cc ** 3 * (0.29E2 * dt ** 4 * cc ** 4 - 0.400
     #E3 * cc ** 2 * dt ** 2 * dx ** 2 + 0.1400E4 * dx ** 4) / dx ** 7 /
     # 0.288000E6
c
      return
      end
c
c++++++++++++++++
c
      subroutine getcuv_sixth2D( 
     *   cc,dx,dy,dt,cuv )
c
      implicit real (t)
      real cuv(9,9)
      real dx,dy,dt,cc
c
      cuv(1,5) = -cc * dt ** 2 * (0.6E1 * dt ** 4 * cc ** 4 - 0.53E2 * c
     #c ** 2 * dt ** 2 * dx ** 2 + 0.64E2 * dx ** 4) / dx ** 5 / 0.34560
     #E5
      cuv(2,4) = -dt ** 4 * cc ** 3 * (0.6E1 * dt ** 2 * cc ** 2 - 0.15E
     #2 * dx ** 2 - 0.4E1 * dx * dy) / dx ** 3 / dy ** 2 / 0.17280E5
      cuv(2,5) = cc * dt ** 2 * (0.87E2 * cc ** 5 * dt ** 5 * dy ** 2 + 
     #0.300E3 * cc ** 4 * dt ** 4 * dx ** 3 + 0.600E3 * cc ** 4 * dt ** 
     #4 * dx * dy ** 2 - 0.525E3 * dt ** 3 * cc ** 3 * dx ** 2 * dy ** 2
     # - 0.750E3 * cc ** 2 * dt ** 2 * dx ** 5 - 0.200E3 * cc ** 2 * dt 
     #** 2 * dx ** 4 * dy - 0.5300E4 * cc ** 2 * dt ** 2 * dx ** 3 * dy 
     #** 2 + 0.800E3 * cc * dt * dx ** 4 * dy ** 2 + 0.6400E4 * dx ** 5 
     #* dy ** 2) / dx ** 6 / dy ** 2 / 0.432000E6
      cuv(2,6) = -dt ** 4 * cc ** 3 * (0.6E1 * dt ** 2 * cc ** 2 - 0.15E
     #2 * dx ** 2 - 0.4E1 * dx * dy) / dx ** 3 / dy ** 2 / 0.17280E5
      cuv(3,3) = -dt ** 4 * cc ** 3 * (0.2E1 * cc ** 2 * dt ** 2 * dx **
     # 3 + 0.2E1 * cc ** 2 * dt ** 2 * dy ** 3 - 0.5E1 * dx ** 3 * dy **
     # 2 - 0.5E1 * dx ** 2 * dy ** 3) / dy ** 4 / dx ** 4 / 0.11520E5
      cuv(3,4) = dt ** 4 * cc ** 3 * (0.87E2 * cc ** 3 * dt ** 3 * dy **
     # 2 + 0.100E3 * cc ** 2 * dt ** 2 * dx ** 3 + 0.300E3 * dt ** 2 * c
     #c ** 2 * dx * dy ** 2 + 0.100E3 * cc ** 2 * dt ** 2 * dy ** 3 - 0.
     #175E3 * cc * dt * dx ** 2 * dy ** 2 - 0.1000E4 * dx ** 3 * dy ** 2
     # - 0.450E3 * dx ** 2 * dy ** 3) / dx ** 4 / dy ** 4 / 0.144000E6
      cuv(3,5) = -cc * dt ** 2 * (0.522E3 * cc ** 5 * dt ** 5 * dx ** 2 
     #* dy ** 2 + 0.522E3 * cc ** 5 * dt ** 5 * dy ** 4 + 0.450E3 * cc *
     #* 4 * dt ** 4 * dx ** 5 + 0.1800E4 * cc ** 4 * dt ** 4 * dx ** 3 *
     # dy ** 2 + 0.450E3 * cc ** 4 * dt ** 4 * dx ** 2 * dy ** 3 + 0.210
     #0E4 * cc ** 4 * dt ** 4 * dx * dy ** 4 - 0.1050E4 * cc ** 3 * dt *
     #* 3 * dx ** 4 * dy ** 2 - 0.6750E4 * cc ** 3 * dt ** 3 * dx ** 2 *
     # dy ** 4 - 0.5625E4 * cc ** 2 * dt ** 2 * dx ** 5 * dy ** 2 - 0.23
     #25E4 * cc ** 2 * dt ** 2 * dx ** 4 * dy ** 3 - 0.18550E5 * cc ** 2
     # * dt ** 2 * dx ** 3 * dy ** 4 + 0.10800E5 * dt * cc * dx ** 4 * d
     #y ** 4 + 0.22400E5 * dx ** 5 * dy ** 4) / dx ** 6 / dy ** 4 / 0.43
     #2000E6
      cuv(3,6) = dt ** 4 * cc ** 3 * (0.87E2 * cc ** 3 * dt ** 3 * dy **
     # 2 + 0.100E3 * cc ** 2 * dt ** 2 * dx ** 3 + 0.300E3 * dt ** 2 * c
     #c ** 2 * dx * dy ** 2 + 0.100E3 * cc ** 2 * dt ** 2 * dy ** 3 - 0.
     #175E3 * cc * dt * dx ** 2 * dy ** 2 - 0.1000E4 * dx ** 3 * dy ** 2
     # - 0.450E3 * dx ** 2 * dy ** 3) / dx ** 4 / dy ** 4 / 0.144000E6
      cuv(3,7) = -dt ** 4 * cc ** 3 * (0.2E1 * cc ** 2 * dt ** 2 * dx **
     # 3 + 0.2E1 * cc ** 2 * dt ** 2 * dy ** 3 - 0.5E1 * dx ** 3 * dy **
     # 2 - 0.5E1 * dx ** 2 * dy ** 3) / dy ** 4 / dx ** 4 / 0.11520E5
      cuv(4,2) = -dt ** 4 * cc ** 3 * (0.6E1 * dt ** 2 * cc ** 2 - 0.4E1
     # * dx * dy - 0.15E2 * dy ** 2) / dx ** 2 / dy ** 3 / 0.17280E5
      cuv(4,3) = dt ** 4 * cc ** 3 * (0.87E2 * cc ** 3 * dt ** 3 * dx **
     # 2 + 0.100E3 * cc ** 2 * dt ** 2 * dx ** 3 + 0.300E3 * cc ** 2 * d
     #t ** 2 * dx ** 2 * dy + 0.100E3 * cc ** 2 * dt ** 2 * dy ** 3 - 0.
     #175E3 * cc * dt * dx ** 2 * dy ** 2 - 0.450E3 * dx ** 3 * dy ** 2 
     #- 0.1000E4 * dx ** 2 * dy ** 3) / dx ** 4 / dy ** 4 / 0.144000E6
      cuv(4,4) = -dt ** 4 * cc ** 3 * (0.348E3 * cc ** 3 * dt ** 3 * dx 
     #** 2 + 0.348E3 * cc ** 3 * dt ** 3 * dy ** 2 + 0.400E3 * cc ** 2 *
     # dt ** 2 * dx ** 3 + 0.750E3 * cc ** 2 * dt ** 2 * dx ** 2 * dy + 
     #0.750E3 * dt ** 2 * cc ** 2 * dx * dy ** 2 + 0.400E3 * cc ** 2 * d
     #t ** 2 * dy ** 3 - 0.3800E4 * cc * dt * dx ** 2 * dy ** 2 - 0.3375
     #E4 * dx ** 3 * dy ** 2 - 0.3375E4 * dx ** 2 * dy ** 3) / dy ** 4 /
     # dx ** 4 / 0.144000E6
      cuv(4,5) = cc * dt ** 2 * (0.1566E4 * cc ** 5 * dt ** 5 * dx ** 4 
     #+ 0.2088E4 * cc ** 5 * dt ** 5 * dx ** 2 * dy ** 2 + 0.1305E4 * cc
     # ** 5 * dt ** 5 * dy ** 4 + 0.1800E4 * cc ** 4 * dt ** 4 * dx ** 5
     # + 0.3000E4 * cc ** 4 * dt ** 4 * dx ** 4 * dy + 0.4500E4 * cc ** 
     #4 * dt ** 4 * dx ** 3 * dy ** 2 + 0.1800E4 * cc ** 4 * dt ** 4 * d
     #x ** 2 * dy ** 3 + 0.4200E4 * cc ** 4 * dt ** 4 * dx * dy ** 4 - 0
     #.21750E5 * cc ** 3 * dt ** 3 * dx ** 4 * dy ** 2 - 0.22275E5 * cc 
     #** 3 * dt ** 3 * dx ** 2 * dy ** 4 - 0.17750E5 * cc ** 2 * dt ** 2
     # * dx ** 5 * dy ** 2 - 0.15000E5 * cc ** 2 * dt ** 2 * dx ** 4 * d
     #y ** 3 - 0.37100E5 * cc ** 2 * dt ** 2 * dx ** 3 * dy ** 4 + 0.108
     #000E6 * dt * cc * dx ** 4 * dy ** 4 + 0.44800E5 * dx ** 5 * dy ** 
     #4) / dx ** 6 / dy ** 4 / 0.432000E6
      cuv(4,6) = -dt ** 4 * cc ** 3 * (0.348E3 * cc ** 3 * dt ** 3 * dx 
     #** 2 + 0.348E3 * cc ** 3 * dt ** 3 * dy ** 2 + 0.400E3 * cc ** 2 *
     # dt ** 2 * dx ** 3 + 0.750E3 * cc ** 2 * dt ** 2 * dx ** 2 * dy + 
     #0.750E3 * dt ** 2 * cc ** 2 * dx * dy ** 2 + 0.400E3 * cc ** 2 * d
     #t ** 2 * dy ** 3 - 0.3800E4 * cc * dt * dx ** 2 * dy ** 2 - 0.3375
     #E4 * dx ** 3 * dy ** 2 - 0.3375E4 * dx ** 2 * dy ** 3) / dy ** 4 /
     # dx ** 4 / 0.144000E6
      cuv(4,7) = dt ** 4 * cc ** 3 * (0.87E2 * cc ** 3 * dt ** 3 * dx **
     # 2 + 0.100E3 * cc ** 2 * dt ** 2 * dx ** 3 + 0.300E3 * cc ** 2 * d
     #t ** 2 * dx ** 2 * dy + 0.100E3 * cc ** 2 * dt ** 2 * dy ** 3 - 0.
     #175E3 * cc * dt * dx ** 2 * dy ** 2 - 0.450E3 * dx ** 3 * dy ** 2 
     #- 0.1000E4 * dx ** 2 * dy ** 3) / dx ** 4 / dy ** 4 / 0.144000E6
      cuv(4,8) = -dt ** 4 * cc ** 3 * (0.6E1 * dt ** 2 * cc ** 2 - 0.4E1
     # * dx * dy - 0.15E2 * dy ** 2) / dx ** 2 / dy ** 3 / 0.17280E5
      cuv(5,1) = -cc * dt ** 2 * (0.6E1 * dt ** 4 * cc ** 4 - 0.53E2 * c
     #c ** 2 * dt ** 2 * dy ** 2 + 0.64E2 * dy ** 4) / dy ** 5 / 0.34560
     #E5
      cuv(5,2) = cc * dt ** 2 * (0.87E2 * dt ** 5 * cc ** 5 * dx ** 2 + 
     #0.600E3 * cc ** 4 * dt ** 4 * dx ** 2 * dy + 0.300E3 * cc ** 4 * d
     #t ** 4 * dy ** 3 - 0.525E3 * dt ** 3 * cc ** 3 * dx ** 2 * dy ** 2
     # - 0.5300E4 * cc ** 2 * dt ** 2 * dx ** 2 * dy ** 3 - 0.200E3 * dt
     # ** 2 * cc ** 2 * dy ** 4 * dx - 0.750E3 * cc ** 2 * dt ** 2 * dy 
     #** 5 + 0.800E3 * cc * dt * dx ** 2 * dy ** 4 + 0.6400E4 * dx ** 2 
     #* dy ** 5) / dy ** 6 / dx ** 2 / 0.432000E6
      cuv(5,3) = -cc * dt ** 2 * (0.522E3 * cc ** 5 * dt ** 5 * dx ** 4 
     #+ 0.522E3 * cc ** 5 * dt ** 5 * dx ** 2 * dy ** 2 + 0.2100E4 * cc 
     #** 4 * dt ** 4 * dx ** 4 * dy + 0.450E3 * cc ** 4 * dt ** 4 * dx *
     #* 3 * dy ** 2 + 0.1800E4 * cc ** 4 * dt ** 4 * dx ** 2 * dy ** 3 +
     # 0.450E3 * cc ** 4 * dt ** 4 * dy ** 5 - 0.6750E4 * cc ** 3 * dt *
     #* 3 * dx ** 4 * dy ** 2 - 0.1050E4 * cc ** 3 * dt ** 3 * dx ** 2 *
     # dy ** 4 - 0.18550E5 * cc ** 2 * dt ** 2 * dx ** 4 * dy ** 3 - 0.2
     #325E4 * cc ** 2 * dt ** 2 * dx ** 3 * dy ** 4 - 0.5625E4 * cc ** 2
     # * dt ** 2 * dx ** 2 * dy ** 5 + 0.10800E5 * dt * cc * dx ** 4 * d
     #y ** 4 + 0.22400E5 * dy ** 5 * dx ** 4) / dy ** 6 / dx ** 4 / 0.43
     #2000E6
      cuv(5,4) = cc * dt ** 2 * (0.1305E4 * cc ** 5 * dt ** 5 * dx ** 4 
     #+ 0.2088E4 * cc ** 5 * dt ** 5 * dx ** 2 * dy ** 2 + 0.1566E4 * cc
     # ** 5 * dt ** 5 * dy ** 4 + 0.4200E4 * cc ** 4 * dt ** 4 * dx ** 4
     # * dy + 0.1800E4 * cc ** 4 * dt ** 4 * dx ** 3 * dy ** 2 + 0.4500E
     #4 * cc ** 4 * dt ** 4 * dx ** 2 * dy ** 3 + 0.3000E4 * cc ** 4 * d
     #t ** 4 * dx * dy ** 4 + 0.1800E4 * cc ** 4 * dt ** 4 * dy ** 5 - 0
     #.22275E5 * cc ** 3 * dt ** 3 * dx ** 4 * dy ** 2 - 0.21750E5 * cc 
     #** 3 * dt ** 3 * dx ** 2 * dy ** 4 - 0.37100E5 * cc ** 2 * dt ** 2
     # * dx ** 4 * dy ** 3 - 0.15000E5 * cc ** 2 * dt ** 2 * dx ** 3 * d
     #y ** 4 - 0.17750E5 * cc ** 2 * dt ** 2 * dx ** 2 * dy ** 5 + 0.108
     #000E6 * dt * cc * dx ** 4 * dy ** 4 + 0.44800E5 * dy ** 5 * dx ** 
     #4) / dy ** 6 / dx ** 4 / 0.432000E6
      cuv(5,5) = -dt * (0.1740E4 * cc ** 6 * dt ** 6 * dx ** 6 + 0.3132E
     #4 * cc ** 6 * dt ** 6 * dx ** 4 * dy ** 2 + 0.3132E4 * cc ** 6 * d
     #t ** 6 * dx ** 2 * dy ** 4 + 0.1740E4 * cc ** 6 * dt ** 6 * dy ** 
     #6 + 0.5250E4 * cc ** 5 * dt ** 5 * dx ** 6 * dy + 0.2700E4 * cc **
     # 5 * dt ** 5 * dx ** 5 * dy ** 2 + 0.6000E4 * cc ** 5 * dt ** 5 * 
     #dx ** 4 * dy ** 3 + 0.6000E4 * cc ** 5 * dt ** 5 * dx ** 3 * dy **
     # 4 + 0.2700E4 * cc ** 5 * dt ** 5 * dx ** 2 * dy ** 5 + 0.5250E4 *
     # cc ** 5 * dt ** 5 * dx * dy ** 6 - 0.32100E5 * cc ** 4 * dt ** 4 
     #* dx ** 6 * dy ** 2 - 0.41400E5 * cc ** 4 * dt ** 4 * dx ** 4 * dy
     # ** 4 - 0.32100E5 * cc ** 4 * dt ** 4 * dx ** 2 * dy ** 6 - 0.4637
     #5E5 * cc ** 3 * dt ** 3 * dx ** 6 * dy ** 3 - 0.25750E5 * cc ** 3 
     #* dt ** 3 * dx ** 5 * dy ** 4 - 0.25750E5 * cc ** 3 * dt ** 3 * dx
     # ** 4 * dy ** 5 - 0.46375E5 * cc ** 3 * dt ** 3 * dx ** 3 * dy ** 
     #6 + 0.196000E6 * cc ** 2 * dt ** 2 * dx ** 6 * dy ** 4 + 0.196000E
     #6 * cc ** 2 * dt ** 2 * dx ** 4 * dy ** 6 + 0.56000E5 * cc * dt * 
     #dx ** 6 * dy ** 5 + 0.56000E5 * cc * dt * dx ** 5 * dy ** 6 - 0.43
     #2000E6 * dx ** 6 * dy ** 6) / dx ** 6 / dy ** 6 / 0.432000E6
      cuv(5,6) = cc * dt ** 2 * (0.1305E4 * cc ** 5 * dt ** 5 * dx ** 4 
     #+ 0.2088E4 * cc ** 5 * dt ** 5 * dx ** 2 * dy ** 2 + 0.1566E4 * cc
     # ** 5 * dt ** 5 * dy ** 4 + 0.4200E4 * cc ** 4 * dt ** 4 * dx ** 4
     # * dy + 0.1800E4 * cc ** 4 * dt ** 4 * dx ** 3 * dy ** 2 + 0.4500E
     #4 * cc ** 4 * dt ** 4 * dx ** 2 * dy ** 3 + 0.3000E4 * cc ** 4 * d
     #t ** 4 * dx * dy ** 4 + 0.1800E4 * cc ** 4 * dt ** 4 * dy ** 5 - 0
     #.22275E5 * cc ** 3 * dt ** 3 * dx ** 4 * dy ** 2 - 0.21750E5 * cc 
     #** 3 * dt ** 3 * dx ** 2 * dy ** 4 - 0.37100E5 * cc ** 2 * dt ** 2
     # * dx ** 4 * dy ** 3 - 0.15000E5 * cc ** 2 * dt ** 2 * dx ** 3 * d
     #y ** 4 - 0.17750E5 * cc ** 2 * dt ** 2 * dx ** 2 * dy ** 5 + 0.108
     #000E6 * dt * cc * dx ** 4 * dy ** 4 + 0.44800E5 * dy ** 5 * dx ** 
     #4) / dy ** 6 / dx ** 4 / 0.432000E6
      cuv(5,7) = -cc * dt ** 2 * (0.522E3 * cc ** 5 * dt ** 5 * dx ** 4 
     #+ 0.522E3 * cc ** 5 * dt ** 5 * dx ** 2 * dy ** 2 + 0.2100E4 * cc 
     #** 4 * dt ** 4 * dx ** 4 * dy + 0.450E3 * cc ** 4 * dt ** 4 * dx *
     #* 3 * dy ** 2 + 0.1800E4 * cc ** 4 * dt ** 4 * dx ** 2 * dy ** 3 +
     # 0.450E3 * cc ** 4 * dt ** 4 * dy ** 5 - 0.6750E4 * cc ** 3 * dt *
     #* 3 * dx ** 4 * dy ** 2 - 0.1050E4 * cc ** 3 * dt ** 3 * dx ** 2 *
     # dy ** 4 - 0.18550E5 * cc ** 2 * dt ** 2 * dx ** 4 * dy ** 3 - 0.2
     #325E4 * cc ** 2 * dt ** 2 * dx ** 3 * dy ** 4 - 0.5625E4 * cc ** 2
     # * dt ** 2 * dx ** 2 * dy ** 5 + 0.10800E5 * dt * cc * dx ** 4 * d
     #y ** 4 + 0.22400E5 * dy ** 5 * dx ** 4) / dy ** 6 / dx ** 4 / 0.43
     #2000E6
      cuv(5,8) = cc * dt ** 2 * (0.87E2 * dt ** 5 * cc ** 5 * dx ** 2 + 
     #0.600E3 * cc ** 4 * dt ** 4 * dx ** 2 * dy + 0.300E3 * cc ** 4 * d
     #t ** 4 * dy ** 3 - 0.525E3 * dt ** 3 * cc ** 3 * dx ** 2 * dy ** 2
     # - 0.5300E4 * cc ** 2 * dt ** 2 * dx ** 2 * dy ** 3 - 0.200E3 * dt
     # ** 2 * cc ** 2 * dy ** 4 * dx - 0.750E3 * cc ** 2 * dt ** 2 * dy 
     #** 5 + 0.800E3 * cc * dt * dx ** 2 * dy ** 4 + 0.6400E4 * dx ** 2 
     #* dy ** 5) / dy ** 6 / dx ** 2 / 0.432000E6
      cuv(5,9) = -cc * dt ** 2 * (0.6E1 * dt ** 4 * cc ** 4 - 0.53E2 * c
     #c ** 2 * dt ** 2 * dy ** 2 + 0.64E2 * dy ** 4) / dy ** 5 / 0.34560
     #E5
      cuv(6,2) = -dt ** 4 * cc ** 3 * (0.6E1 * dt ** 2 * cc ** 2 - 0.4E1
     # * dx * dy - 0.15E2 * dy ** 2) / dx ** 2 / dy ** 3 / 0.17280E5
      cuv(6,3) = dt ** 4 * cc ** 3 * (0.87E2 * cc ** 3 * dt ** 3 * dx **
     # 2 + 0.100E3 * cc ** 2 * dt ** 2 * dx ** 3 + 0.300E3 * cc ** 2 * d
     #t ** 2 * dx ** 2 * dy + 0.100E3 * cc ** 2 * dt ** 2 * dy ** 3 - 0.
     #175E3 * cc * dt * dx ** 2 * dy ** 2 - 0.450E3 * dx ** 3 * dy ** 2 
     #- 0.1000E4 * dx ** 2 * dy ** 3) / dx ** 4 / dy ** 4 / 0.144000E6
      cuv(6,4) = -dt ** 4 * cc ** 3 * (0.348E3 * cc ** 3 * dt ** 3 * dx 
     #** 2 + 0.348E3 * cc ** 3 * dt ** 3 * dy ** 2 + 0.400E3 * cc ** 2 *
     # dt ** 2 * dx ** 3 + 0.750E3 * cc ** 2 * dt ** 2 * dx ** 2 * dy + 
     #0.750E3 * dt ** 2 * cc ** 2 * dx * dy ** 2 + 0.400E3 * cc ** 2 * d
     #t ** 2 * dy ** 3 - 0.3800E4 * cc * dt * dx ** 2 * dy ** 2 - 0.3375
     #E4 * dx ** 3 * dy ** 2 - 0.3375E4 * dx ** 2 * dy ** 3) / dy ** 4 /
     # dx ** 4 / 0.144000E6
      cuv(6,5) = cc * dt ** 2 * (0.1566E4 * cc ** 5 * dt ** 5 * dx ** 4 
     #+ 0.2088E4 * cc ** 5 * dt ** 5 * dx ** 2 * dy ** 2 + 0.1305E4 * cc
     # ** 5 * dt ** 5 * dy ** 4 + 0.1800E4 * cc ** 4 * dt ** 4 * dx ** 5
     # + 0.3000E4 * cc ** 4 * dt ** 4 * dx ** 4 * dy + 0.4500E4 * cc ** 
     #4 * dt ** 4 * dx ** 3 * dy ** 2 + 0.1800E4 * cc ** 4 * dt ** 4 * d
     #x ** 2 * dy ** 3 + 0.4200E4 * cc ** 4 * dt ** 4 * dx * dy ** 4 - 0
     #.21750E5 * cc ** 3 * dt ** 3 * dx ** 4 * dy ** 2 - 0.22275E5 * cc 
     #** 3 * dt ** 3 * dx ** 2 * dy ** 4 - 0.17750E5 * cc ** 2 * dt ** 2
     # * dx ** 5 * dy ** 2 - 0.15000E5 * cc ** 2 * dt ** 2 * dx ** 4 * d
     #y ** 3 - 0.37100E5 * cc ** 2 * dt ** 2 * dx ** 3 * dy ** 4 + 0.108
     #000E6 * dt * cc * dx ** 4 * dy ** 4 + 0.44800E5 * dx ** 5 * dy ** 
     #4) / dx ** 6 / dy ** 4 / 0.432000E6
      cuv(6,6) = -dt ** 4 * cc ** 3 * (0.348E3 * cc ** 3 * dt ** 3 * dx 
     #** 2 + 0.348E3 * cc ** 3 * dt ** 3 * dy ** 2 + 0.400E3 * cc ** 2 *
     # dt ** 2 * dx ** 3 + 0.750E3 * cc ** 2 * dt ** 2 * dx ** 2 * dy + 
     #0.750E3 * dt ** 2 * cc ** 2 * dx * dy ** 2 + 0.400E3 * cc ** 2 * d
     #t ** 2 * dy ** 3 - 0.3800E4 * cc * dt * dx ** 2 * dy ** 2 - 0.3375
     #E4 * dx ** 3 * dy ** 2 - 0.3375E4 * dx ** 2 * dy ** 3) / dy ** 4 /
     # dx ** 4 / 0.144000E6
      cuv(6,7) = dt ** 4 * cc ** 3 * (0.87E2 * cc ** 3 * dt ** 3 * dx **
     # 2 + 0.100E3 * cc ** 2 * dt ** 2 * dx ** 3 + 0.300E3 * cc ** 2 * d
     #t ** 2 * dx ** 2 * dy + 0.100E3 * cc ** 2 * dt ** 2 * dy ** 3 - 0.
     #175E3 * cc * dt * dx ** 2 * dy ** 2 - 0.450E3 * dx ** 3 * dy ** 2 
     #- 0.1000E4 * dx ** 2 * dy ** 3) / dx ** 4 / dy ** 4 / 0.144000E6
      cuv(6,8) = -dt ** 4 * cc ** 3 * (0.6E1 * dt ** 2 * cc ** 2 - 0.4E1
     # * dx * dy - 0.15E2 * dy ** 2) / dx ** 2 / dy ** 3 / 0.17280E5
      cuv(7,3) = -dt ** 4 * cc ** 3 * (0.2E1 * cc ** 2 * dt ** 2 * dx **
     # 3 + 0.2E1 * cc ** 2 * dt ** 2 * dy ** 3 - 0.5E1 * dx ** 3 * dy **
     # 2 - 0.5E1 * dx ** 2 * dy ** 3) / dy ** 4 / dx ** 4 / 0.11520E5
      cuv(7,4) = dt ** 4 * cc ** 3 * (0.87E2 * cc ** 3 * dt ** 3 * dy **
     # 2 + 0.100E3 * cc ** 2 * dt ** 2 * dx ** 3 + 0.300E3 * dt ** 2 * c
     #c ** 2 * dx * dy ** 2 + 0.100E3 * cc ** 2 * dt ** 2 * dy ** 3 - 0.
     #175E3 * cc * dt * dx ** 2 * dy ** 2 - 0.1000E4 * dx ** 3 * dy ** 2
     # - 0.450E3 * dx ** 2 * dy ** 3) / dx ** 4 / dy ** 4 / 0.144000E6
      cuv(7,5) = -cc * dt ** 2 * (0.522E3 * cc ** 5 * dt ** 5 * dx ** 2 
     #* dy ** 2 + 0.522E3 * cc ** 5 * dt ** 5 * dy ** 4 + 0.450E3 * cc *
     #* 4 * dt ** 4 * dx ** 5 + 0.1800E4 * cc ** 4 * dt ** 4 * dx ** 3 *
     # dy ** 2 + 0.450E3 * cc ** 4 * dt ** 4 * dx ** 2 * dy ** 3 + 0.210
     #0E4 * cc ** 4 * dt ** 4 * dx * dy ** 4 - 0.1050E4 * cc ** 3 * dt *
     #* 3 * dx ** 4 * dy ** 2 - 0.6750E4 * cc ** 3 * dt ** 3 * dx ** 2 *
     # dy ** 4 - 0.5625E4 * cc ** 2 * dt ** 2 * dx ** 5 * dy ** 2 - 0.23
     #25E4 * cc ** 2 * dt ** 2 * dx ** 4 * dy ** 3 - 0.18550E5 * cc ** 2
     # * dt ** 2 * dx ** 3 * dy ** 4 + 0.10800E5 * dt * cc * dx ** 4 * d
     #y ** 4 + 0.22400E5 * dx ** 5 * dy ** 4) / dx ** 6 / dy ** 4 / 0.43
     #2000E6
      cuv(7,6) = dt ** 4 * cc ** 3 * (0.87E2 * cc ** 3 * dt ** 3 * dy **
     # 2 + 0.100E3 * cc ** 2 * dt ** 2 * dx ** 3 + 0.300E3 * dt ** 2 * c
     #c ** 2 * dx * dy ** 2 + 0.100E3 * cc ** 2 * dt ** 2 * dy ** 3 - 0.
     #175E3 * cc * dt * dx ** 2 * dy ** 2 - 0.1000E4 * dx ** 3 * dy ** 2
     # - 0.450E3 * dx ** 2 * dy ** 3) / dx ** 4 / dy ** 4 / 0.144000E6
      cuv(7,7) = -dt ** 4 * cc ** 3 * (0.2E1 * cc ** 2 * dt ** 2 * dx **
     # 3 + 0.2E1 * cc ** 2 * dt ** 2 * dy ** 3 - 0.5E1 * dx ** 3 * dy **
     # 2 - 0.5E1 * dx ** 2 * dy ** 3) / dy ** 4 / dx ** 4 / 0.11520E5
      cuv(8,4) = -dt ** 4 * cc ** 3 * (0.6E1 * dt ** 2 * cc ** 2 - 0.15E
     #2 * dx ** 2 - 0.4E1 * dx * dy) / dx ** 3 / dy ** 2 / 0.17280E5
      cuv(8,5) = cc * dt ** 2 * (0.87E2 * cc ** 5 * dt ** 5 * dy ** 2 + 
     #0.300E3 * cc ** 4 * dt ** 4 * dx ** 3 + 0.600E3 * cc ** 4 * dt ** 
     #4 * dx * dy ** 2 - 0.525E3 * dt ** 3 * cc ** 3 * dx ** 2 * dy ** 2
     # - 0.750E3 * cc ** 2 * dt ** 2 * dx ** 5 - 0.200E3 * cc ** 2 * dt 
     #** 2 * dx ** 4 * dy - 0.5300E4 * cc ** 2 * dt ** 2 * dx ** 3 * dy 
     #** 2 + 0.800E3 * cc * dt * dx ** 4 * dy ** 2 + 0.6400E4 * dx ** 5 
     #* dy ** 2) / dx ** 6 / dy ** 2 / 0.432000E6
      cuv(8,6) = -dt ** 4 * cc ** 3 * (0.6E1 * dt ** 2 * cc ** 2 - 0.15E
     #2 * dx ** 2 - 0.4E1 * dx * dy) / dx ** 3 / dy ** 2 / 0.17280E5
      cuv(9,5) = -cc * dt ** 2 * (0.6E1 * dt ** 4 * cc ** 4 - 0.53E2 * c
     #c ** 2 * dt ** 2 * dx ** 2 + 0.64E2 * dx ** 4) / dx ** 5 / 0.34560
     #E5
c
      return
      end
c
c++++++++++++++++
c
      subroutine getcvu_sixth2D( 
     *   cc,dx,dy,dt,cvu )
c
      implicit real (t)
      real cvu(9,9)
      real dx,dy,dt,cc
c
      cvu(1,5) = dt ** 2 * cc ** 3 * (dt ** 4 * cc ** 4 - 0.10E2 * cc **
     # 2 * dt ** 2 * dx ** 2 + 0.21E2 * dx ** 4) / dx ** 7 / 0.1440E4
      cvu(2,4) = dt ** 2 * cc ** 3 * (dt ** 4 * cc ** 4 - 0.5E1 * cc ** 
     #2 * dt ** 2 * dx ** 2 + 0.4E1 * dx ** 4) * (0.3E1 * dx + dy) / dx 
     #** 6 / dy ** 2 / 0.1440E4
      cvu(2,5) = -dt * cc ** 2 * (0.3E1 * dt ** 5 * cc ** 5 * dx ** 2 + 
     #cc ** 5 * dt ** 5 * dx * dy + 0.4E1 * cc ** 5 * dt ** 5 * dy ** 2 
     #- 0.6E1 * cc ** 4 * dt ** 4 * dx * dy ** 2 - 0.15E2 * cc ** 3 * dt
     # ** 3 * dx ** 4 - 0.5E1 * cc ** 3 * dt ** 3 * dx ** 3 * dy - 0.40E
     #2 * dt ** 3 * cc ** 3 * dx ** 2 * dy ** 2 + 0.20E2 * cc ** 2 * dt 
     #** 2 * dx ** 3 * dy ** 2 + 0.12E2 * cc * dt * dx ** 6 + 0.4E1 * cc
     # * dt * dx ** 5 * dy + 0.84E2 * cc * dt * dx ** 4 * dy ** 2 - 0.8E
     #1 * dx ** 5 * dy ** 2) / dx ** 7 / dy ** 2 / 0.720E3
      cvu(2,6) = dt ** 2 * cc ** 3 * (dt ** 4 * cc ** 4 - 0.5E1 * cc ** 
     #2 * dt ** 2 * dx ** 2 + 0.4E1 * dx ** 4) * (0.3E1 * dx + dy) / dx 
     #** 6 / dy ** 2 / 0.1440E4
      cvu(3,3) = dt ** 2 * cc ** 3 * (0.3E1 * dt ** 4 * cc ** 4 - 0.5E1 
     #* cc ** 2 * dt ** 2 * dx ** 2 - 0.5E1 * cc ** 2 * dt ** 2 * dy ** 
     #2 + 0.5E1 * dx ** 2 * dy ** 2) * (dx + dy) / dy ** 4 / dx ** 4 / 0
     #.1440E4
      cvu(3,4) = -dt ** 2 * cc ** 3 * (0.6E1 * cc ** 4 * dt ** 4 * dx **
     # 3 + 0.6E1 * cc ** 4 * dt ** 4 * dx ** 2 * dy + 0.9E1 * cc ** 4 * 
     #dt ** 4 * dx * dy ** 2 + 0.3E1 * cc ** 4 * dt ** 4 * dy ** 3 - 0.1
     #8E2 * dt ** 3 * cc ** 3 * dx ** 2 * dy ** 2 - 0.10E2 * cc ** 2 * d
     #t ** 2 * dx ** 5 - 0.10E2 * cc ** 2 * dt ** 2 * dx ** 4 * dy - 0.5
     #5E2 * cc ** 2 * dt ** 2 * dx ** 3 * dy ** 2 - 0.25E2 * cc ** 2 * d
     #t ** 2 * dx ** 2 * dy ** 3 + 0.20E2 * cc * dt * dx ** 4 * dy ** 2 
     #+ 0.46E2 * dx ** 5 * dy ** 2 + 0.22E2 * dx ** 4 * dy ** 3) / dx **
     # 6 / dy ** 4 / 0.720E3
      cvu(3,5) = cc ** 2 * dt * (0.9E1 * cc ** 5 * dt ** 5 * dx ** 4 + 0
     #.9E1 * cc ** 5 * dt ** 5 * dx ** 3 * dy + 0.18E2 * cc ** 5 * dt **
     # 5 * dx ** 2 * dy ** 2 + 0.6E1 * cc ** 5 * dt ** 5 * dx * dy ** 3 
     #+ 0.14E2 * cc ** 5 * dt ** 5 * dy ** 4 - 0.36E2 * cc ** 4 * dt ** 
     #4 * dx ** 3 * dy ** 2 - 0.36E2 * cc ** 4 * dt ** 4 * dx * dy ** 4 
     #- 0.15E2 * cc ** 3 * dt ** 3 * dx ** 6 - 0.15E2 * cc ** 3 * dt ** 
     #3 * dx ** 5 * dy - 0.105E3 * cc ** 3 * dt ** 3 * dx ** 4 * dy ** 2
     # - 0.45E2 * cc ** 3 * dt ** 3 * dx ** 3 * dy ** 3 - 0.140E3 * cc *
     #* 3 * dt ** 3 * dx ** 2 * dy ** 4 + 0.40E2 * cc ** 2 * dt ** 2 * d
     #x ** 5 * dy ** 2 + 0.240E3 * cc ** 2 * dt ** 2 * dx ** 3 * dy ** 4
     # + 0.87E2 * cc * dt * dx ** 6 * dy ** 2 + 0.39E2 * cc * dt * dx **
     # 5 * dy ** 3 + 0.294E3 * dt * cc * dx ** 4 * dy ** 4 - 0.108E3 * d
     #x ** 5 * dy ** 4) / dx ** 7 / dy ** 4 / 0.720E3
      cvu(3,6) = -dt ** 2 * cc ** 3 * (0.6E1 * cc ** 4 * dt ** 4 * dx **
     # 3 + 0.6E1 * cc ** 4 * dt ** 4 * dx ** 2 * dy + 0.9E1 * cc ** 4 * 
     #dt ** 4 * dx * dy ** 2 + 0.3E1 * cc ** 4 * dt ** 4 * dy ** 3 - 0.1
     #8E2 * dt ** 3 * cc ** 3 * dx ** 2 * dy ** 2 - 0.10E2 * cc ** 2 * d
     #t ** 2 * dx ** 5 - 0.10E2 * cc ** 2 * dt ** 2 * dx ** 4 * dy - 0.5
     #5E2 * cc ** 2 * dt ** 2 * dx ** 3 * dy ** 2 - 0.25E2 * cc ** 2 * d
     #t ** 2 * dx ** 2 * dy ** 3 + 0.20E2 * cc * dt * dx ** 4 * dy ** 2 
     #+ 0.46E2 * dx ** 5 * dy ** 2 + 0.22E2 * dx ** 4 * dy ** 3) / dx **
     # 6 / dy ** 4 / 0.720E3
      cvu(3,7) = dt ** 2 * cc ** 3 * (0.3E1 * dt ** 4 * cc ** 4 - 0.5E1 
     #* cc ** 2 * dt ** 2 * dx ** 2 - 0.5E1 * cc ** 2 * dt ** 2 * dy ** 
     #2 + 0.5E1 * dx ** 2 * dy ** 2) * (dx + dy) / dy ** 4 / dx ** 4 / 0
     #.1440E4
      cvu(4,2) = dt ** 2 * cc ** 3 * (dt ** 4 * cc ** 4 - 0.5E1 * cc ** 
     #2 * dt ** 2 * dy ** 2 + 0.4E1 * dy ** 4) * (dx + 0.3E1 * dy) / dy 
     #** 6 / dx ** 2 / 0.1440E4
      cvu(4,3) = -dt ** 2 * cc ** 3 * (0.3E1 * cc ** 4 * dt ** 4 * dx **
     # 3 + 0.9E1 * cc ** 4 * dt ** 4 * dx ** 2 * dy + 0.6E1 * cc ** 4 * 
     #dt ** 4 * dx * dy ** 2 + 0.6E1 * cc ** 4 * dt ** 4 * dy ** 3 - 0.1
     #8E2 * dt ** 3 * cc ** 3 * dx ** 2 * dy ** 2 - 0.25E2 * cc ** 2 * d
     #t ** 2 * dx ** 3 * dy ** 2 - 0.55E2 * cc ** 2 * dt ** 2 * dx ** 2 
     #* dy ** 3 - 0.10E2 * dt ** 2 * cc ** 2 * dy ** 4 * dx - 0.10E2 * c
     #c ** 2 * dt ** 2 * dy ** 5 + 0.20E2 * cc * dt * dx ** 2 * dy ** 4 
     #+ 0.22E2 * dx ** 3 * dy ** 4 + 0.46E2 * dx ** 2 * dy ** 5) / dy **
     # 6 / dx ** 4 / 0.720E3
      cvu(4,4) = dt ** 2 * cc ** 3 * (0.15E2 * cc ** 4 * dt ** 4 * dx **
     # 5 + 0.45E2 * cc ** 4 * dt ** 4 * dx ** 4 * dy + 0.48E2 * cc ** 4 
     #* dt ** 4 * dx ** 3 * dy ** 2 + 0.48E2 * cc ** 4 * dt ** 4 * dx **
     # 2 * dy ** 3 + 0.45E2 * cc ** 4 * dt ** 4 * dx * dy ** 4 + 0.15E2 
     #* cc ** 4 * dt ** 4 * dy ** 5 - 0.144E3 * cc ** 3 * dt ** 3 * dx *
     #* 4 * dy ** 2 - 0.144E3 * cc ** 3 * dt ** 3 * dx ** 2 * dy ** 4 - 
     #0.155E3 * cc ** 2 * dt ** 2 * dx ** 5 * dy ** 2 - 0.305E3 * cc ** 
     #2 * dt ** 2 * dx ** 4 * dy ** 3 - 0.305E3 * cc ** 2 * dt ** 2 * dx
     # ** 3 * dy ** 4 - 0.155E3 * cc ** 2 * dt ** 2 * dx ** 2 * dy ** 5 
     #+ 0.800E3 * dt * cc * dx ** 4 * dy ** 4 + 0.320E3 * dx ** 5 * dy *
     #* 4 + 0.320E3 * dy ** 5 * dx ** 4) / dy ** 6 / dx ** 6 / 0.1440E4
      cvu(4,5) = -cc ** 2 * dt * (0.10E2 * cc ** 5 * dt ** 5 * dx ** 6 +
     # 0.30E2 * cc ** 5 * dt ** 5 * dx ** 5 * dy + 0.36E2 * cc ** 5 * dt
     # ** 5 * dx ** 4 * dy ** 2 + 0.36E2 * cc ** 5 * dt ** 5 * dx ** 3 *
     # dy ** 3 + 0.45E2 * cc ** 5 * dt ** 5 * dx ** 2 * dy ** 4 + 0.15E2
     # * cc ** 5 * dt ** 5 * dx * dy ** 5 + 0.28E2 * cc ** 5 * dt ** 5 *
     # dy ** 6 - 0.108E3 * cc ** 4 * dt ** 4 * dx ** 5 * dy ** 2 - 0.144
     #E3 * cc ** 4 * dt ** 4 * dx ** 3 * dy ** 4 - 0.90E2 * cc ** 4 * dt
     # ** 4 * dx * dy ** 6 - 0.110E3 * cc ** 3 * dt ** 3 * dx ** 6 * dy 
     #** 2 - 0.210E3 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 3 - 0.285E3 *
     # cc ** 3 * dt ** 3 * dx ** 4 * dy ** 4 - 0.135E3 * cc ** 3 * dt **
     # 3 * dx ** 3 * dy ** 5 - 0.280E3 * cc ** 3 * dt ** 3 * dx ** 2 * d
     #y ** 6 + 0.760E3 * cc ** 2 * dt ** 2 * dx ** 5 * dy ** 4 + 0.780E3
     # * cc ** 2 * dt ** 2 * dx ** 3 * dy ** 6 + 0.280E3 * dt * cc * dx 
     #** 6 * dy ** 4 + 0.240E3 * cc * dt * dx ** 5 * dy ** 5 + 0.588E3 *
     # cc * dt * dx ** 4 * dy ** 6 - 0.1080E4 * dx ** 5 * dy ** 6) / dx 
     #** 7 / dy ** 6 / 0.720E3
      cvu(4,6) = dt ** 2 * cc ** 3 * (0.15E2 * cc ** 4 * dt ** 4 * dx **
     # 5 + 0.45E2 * cc ** 4 * dt ** 4 * dx ** 4 * dy + 0.48E2 * cc ** 4 
     #* dt ** 4 * dx ** 3 * dy ** 2 + 0.48E2 * cc ** 4 * dt ** 4 * dx **
     # 2 * dy ** 3 + 0.45E2 * cc ** 4 * dt ** 4 * dx * dy ** 4 + 0.15E2 
     #* cc ** 4 * dt ** 4 * dy ** 5 - 0.144E3 * cc ** 3 * dt ** 3 * dx *
     #* 4 * dy ** 2 - 0.144E3 * cc ** 3 * dt ** 3 * dx ** 2 * dy ** 4 - 
     #0.155E3 * cc ** 2 * dt ** 2 * dx ** 5 * dy ** 2 - 0.305E3 * cc ** 
     #2 * dt ** 2 * dx ** 4 * dy ** 3 - 0.305E3 * cc ** 2 * dt ** 2 * dx
     # ** 3 * dy ** 4 - 0.155E3 * cc ** 2 * dt ** 2 * dx ** 2 * dy ** 5 
     #+ 0.800E3 * dt * cc * dx ** 4 * dy ** 4 + 0.320E3 * dx ** 5 * dy *
     #* 4 + 0.320E3 * dy ** 5 * dx ** 4) / dy ** 6 / dx ** 6 / 0.1440E4
      cvu(4,7) = -dt ** 2 * cc ** 3 * (0.3E1 * cc ** 4 * dt ** 4 * dx **
     # 3 + 0.9E1 * cc ** 4 * dt ** 4 * dx ** 2 * dy + 0.6E1 * cc ** 4 * 
     #dt ** 4 * dx * dy ** 2 + 0.6E1 * cc ** 4 * dt ** 4 * dy ** 3 - 0.1
     #8E2 * dt ** 3 * cc ** 3 * dx ** 2 * dy ** 2 - 0.25E2 * cc ** 2 * d
     #t ** 2 * dx ** 3 * dy ** 2 - 0.55E2 * cc ** 2 * dt ** 2 * dx ** 2 
     #* dy ** 3 - 0.10E2 * dt ** 2 * cc ** 2 * dy ** 4 * dx - 0.10E2 * c
     #c ** 2 * dt ** 2 * dy ** 5 + 0.20E2 * cc * dt * dx ** 2 * dy ** 4 
     #+ 0.22E2 * dx ** 3 * dy ** 4 + 0.46E2 * dx ** 2 * dy ** 5) / dy **
     # 6 / dx ** 4 / 0.720E3
      cvu(4,8) = dt ** 2 * cc ** 3 * (dt ** 4 * cc ** 4 - 0.5E1 * cc ** 
     #2 * dt ** 2 * dy ** 2 + 0.4E1 * dy ** 4) * (dx + 0.3E1 * dy) / dy 
     #** 6 / dx ** 2 / 0.1440E4
      cvu(5,1) = dt ** 2 * cc ** 3 * (dt ** 4 * cc ** 4 - 0.10E2 * cc **
     # 2 * dt ** 2 * dy ** 2 + 0.21E2 * dy ** 4) / dy ** 7 / 0.1440E4
      cvu(5,2) = -dt * cc ** 2 * (0.4E1 * dt ** 5 * cc ** 5 * dx ** 2 + 
     #cc ** 5 * dt ** 5 * dx * dy + 0.3E1 * cc ** 5 * dt ** 5 * dy ** 2 
     #- 0.6E1 * cc ** 4 * dt ** 4 * dx ** 2 * dy - 0.40E2 * dt ** 3 * cc
     # ** 3 * dx ** 2 * dy ** 2 - 0.5E1 * cc ** 3 * dt ** 3 * dx * dy **
     # 3 - 0.15E2 * cc ** 3 * dt ** 3 * dy ** 4 + 0.20E2 * cc ** 2 * dt 
     #** 2 * dx ** 2 * dy ** 3 + 0.84E2 * cc * dt * dx ** 2 * dy ** 4 + 
     #0.4E1 * cc * dt * dx * dy ** 5 + 0.12E2 * cc * dt * dy ** 6 - 0.8E
     #1 * dx ** 2 * dy ** 5) / dy ** 7 / dx ** 2 / 0.720E3
      cvu(5,3) = dt * cc ** 2 * (0.14E2 * cc ** 5 * dt ** 5 * dx ** 4 + 
     #0.6E1 * cc ** 5 * dt ** 5 * dx ** 3 * dy + 0.18E2 * cc ** 5 * dt *
     #* 5 * dx ** 2 * dy ** 2 + 0.9E1 * cc ** 5 * dt ** 5 * dx * dy ** 3
     # + 0.9E1 * cc ** 5 * dt ** 5 * dy ** 4 - 0.36E2 * cc ** 4 * dt ** 
     #4 * dx ** 4 * dy - 0.36E2 * cc ** 4 * dt ** 4 * dx ** 2 * dy ** 3 
     #- 0.140E3 * cc ** 3 * dt ** 3 * dx ** 4 * dy ** 2 - 0.45E2 * cc **
     # 3 * dt ** 3 * dx ** 3 * dy ** 3 - 0.105E3 * cc ** 3 * dt ** 3 * d
     #x ** 2 * dy ** 4 - 0.15E2 * cc ** 3 * dt ** 3 * dx * dy ** 5 - 0.1
     #5E2 * cc ** 3 * dt ** 3 * dy ** 6 + 0.240E3 * cc ** 2 * dt ** 2 * 
     #dx ** 4 * dy ** 3 + 0.40E2 * cc ** 2 * dt ** 2 * dx ** 2 * dy ** 5
     # + 0.294E3 * dt * cc * dx ** 4 * dy ** 4 + 0.39E2 * cc * dt * dx *
     #* 3 * dy ** 5 + 0.87E2 * cc * dt * dx ** 2 * dy ** 6 - 0.108E3 * d
     #y ** 5 * dx ** 4) / dy ** 7 / dx ** 4 / 0.720E3
      cvu(5,4) = -dt * cc ** 2 * (0.28E2 * cc ** 5 * dt ** 5 * dx ** 6 +
     # 0.15E2 * cc ** 5 * dt ** 5 * dx ** 5 * dy + 0.45E2 * cc ** 5 * dt
     # ** 5 * dx ** 4 * dy ** 2 + 0.36E2 * cc ** 5 * dt ** 5 * dx ** 3 *
     # dy ** 3 + 0.36E2 * cc ** 5 * dt ** 5 * dx ** 2 * dy ** 4 + 0.30E2
     # * cc ** 5 * dt ** 5 * dx * dy ** 5 + 0.10E2 * cc ** 5 * dt ** 5 *
     # dy ** 6 - 0.90E2 * cc ** 4 * dt ** 4 * dx ** 6 * dy - 0.144E3 * c
     #c ** 4 * dt ** 4 * dx ** 4 * dy ** 3 - 0.108E3 * cc ** 4 * dt ** 4
     # * dx ** 2 * dy ** 5 - 0.280E3 * cc ** 3 * dt ** 3 * dx ** 6 * dy 
     #** 2 - 0.135E3 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 3 - 0.285E3 *
     # cc ** 3 * dt ** 3 * dx ** 4 * dy ** 4 - 0.210E3 * cc ** 3 * dt **
     # 3 * dx ** 3 * dy ** 5 - 0.110E3 * cc ** 3 * dt ** 3 * dx ** 2 * d
     #y ** 6 + 0.780E3 * cc ** 2 * dt ** 2 * dx ** 6 * dy ** 3 + 0.760E3
     # * cc ** 2 * dt ** 2 * dx ** 4 * dy ** 5 + 0.588E3 * dt * cc * dx 
     #** 6 * dy ** 4 + 0.240E3 * cc * dt * dx ** 5 * dy ** 5 + 0.280E3 *
     # cc * dt * dx ** 4 * dy ** 6 - 0.1080E4 * dx ** 6 * dy ** 5) / dx 
     #** 6 / dy ** 7 / 0.720E3
      cvu(5,5) = cc ** 2 * dt * (0.35E2 * cc ** 5 * dt ** 5 * dx ** 7 + 
     #0.20E2 * cc ** 5 * dt ** 5 * dx ** 6 * dy + 0.60E2 * cc ** 5 * dt 
     #** 5 * dx ** 5 * dy ** 2 + 0.54E2 * cc ** 5 * dt ** 5 * dx ** 4 * 
     #dy ** 3 + 0.54E2 * cc ** 5 * dt ** 5 * dx ** 3 * dy ** 4 + 0.60E2 
     #* cc ** 5 * dt ** 5 * dx ** 2 * dy ** 5 + 0.20E2 * cc ** 5 * dt **
     # 5 * dx * dy ** 6 + 0.35E2 * cc ** 5 * dt ** 5 * dy ** 7 - 0.120E3
     # * cc ** 4 * dt ** 4 * dx ** 7 * dy - 0.216E3 * cc ** 4 * dt ** 4 
     #* dx ** 5 * dy ** 3 - 0.216E3 * cc ** 4 * dt ** 4 * dx ** 3 * dy *
     #* 5 - 0.120E3 * cc ** 4 * dt ** 4 * dx * dy ** 7 - 0.350E3 * cc **
     # 3 * dt ** 3 * dx ** 7 * dy ** 2 - 0.190E3 * cc ** 3 * dt ** 3 * d
     #x ** 6 * dy ** 3 - 0.390E3 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 4
     # - 0.390E3 * cc ** 3 * dt ** 3 * dx ** 4 * dy ** 5 - 0.190E3 * cc 
     #** 3 * dt ** 3 * dx ** 3 * dy ** 6 - 0.350E3 * cc ** 3 * dt ** 3 *
     # dx ** 2 * dy ** 7 + 0.1120E4 * cc ** 2 * dt ** 2 * dx ** 7 * dy *
     #* 3 + 0.1440E4 * cc ** 2 * dt ** 2 * dx ** 5 * dy ** 5 + 0.1120E4 
     #* cc ** 2 * dt ** 2 * dx ** 3 * dy ** 7 + 0.735E3 * cc * dt * dx *
     #* 7 * dy ** 4 + 0.410E3 * cc * dt * dx ** 6 * dy ** 5 + 0.410E3 * 
     #cc * dt * dx ** 5 * dy ** 6 + 0.735E3 * cc * dt * dx ** 4 * dy ** 
     #7 - 0.1960E4 * dx ** 7 * dy ** 5 - 0.1960E4 * dx ** 5 * dy ** 7) /
     # dx ** 7 / dy ** 7 / 0.720E3
      cvu(5,6) = -dt * cc ** 2 * (0.28E2 * cc ** 5 * dt ** 5 * dx ** 6 +
     # 0.15E2 * cc ** 5 * dt ** 5 * dx ** 5 * dy + 0.45E2 * cc ** 5 * dt
     # ** 5 * dx ** 4 * dy ** 2 + 0.36E2 * cc ** 5 * dt ** 5 * dx ** 3 *
     # dy ** 3 + 0.36E2 * cc ** 5 * dt ** 5 * dx ** 2 * dy ** 4 + 0.30E2
     # * cc ** 5 * dt ** 5 * dx * dy ** 5 + 0.10E2 * cc ** 5 * dt ** 5 *
     # dy ** 6 - 0.90E2 * cc ** 4 * dt ** 4 * dx ** 6 * dy - 0.144E3 * c
     #c ** 4 * dt ** 4 * dx ** 4 * dy ** 3 - 0.108E3 * cc ** 4 * dt ** 4
     # * dx ** 2 * dy ** 5 - 0.280E3 * cc ** 3 * dt ** 3 * dx ** 6 * dy 
     #** 2 - 0.135E3 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 3 - 0.285E3 *
     # cc ** 3 * dt ** 3 * dx ** 4 * dy ** 4 - 0.210E3 * cc ** 3 * dt **
     # 3 * dx ** 3 * dy ** 5 - 0.110E3 * cc ** 3 * dt ** 3 * dx ** 2 * d
     #y ** 6 + 0.780E3 * cc ** 2 * dt ** 2 * dx ** 6 * dy ** 3 + 0.760E3
     # * cc ** 2 * dt ** 2 * dx ** 4 * dy ** 5 + 0.588E3 * dt * cc * dx 
     #** 6 * dy ** 4 + 0.240E3 * cc * dt * dx ** 5 * dy ** 5 + 0.280E3 *
     # cc * dt * dx ** 4 * dy ** 6 - 0.1080E4 * dx ** 6 * dy ** 5) / dx 
     #** 6 / dy ** 7 / 0.720E3
      cvu(5,7) = dt * cc ** 2 * (0.14E2 * cc ** 5 * dt ** 5 * dx ** 4 + 
     #0.6E1 * cc ** 5 * dt ** 5 * dx ** 3 * dy + 0.18E2 * cc ** 5 * dt *
     #* 5 * dx ** 2 * dy ** 2 + 0.9E1 * cc ** 5 * dt ** 5 * dx * dy ** 3
     # + 0.9E1 * cc ** 5 * dt ** 5 * dy ** 4 - 0.36E2 * cc ** 4 * dt ** 
     #4 * dx ** 4 * dy - 0.36E2 * cc ** 4 * dt ** 4 * dx ** 2 * dy ** 3 
     #- 0.140E3 * cc ** 3 * dt ** 3 * dx ** 4 * dy ** 2 - 0.45E2 * cc **
     # 3 * dt ** 3 * dx ** 3 * dy ** 3 - 0.105E3 * cc ** 3 * dt ** 3 * d
     #x ** 2 * dy ** 4 - 0.15E2 * cc ** 3 * dt ** 3 * dx * dy ** 5 - 0.1
     #5E2 * cc ** 3 * dt ** 3 * dy ** 6 + 0.240E3 * cc ** 2 * dt ** 2 * 
     #dx ** 4 * dy ** 3 + 0.40E2 * cc ** 2 * dt ** 2 * dx ** 2 * dy ** 5
     # + 0.294E3 * dt * cc * dx ** 4 * dy ** 4 + 0.39E2 * cc * dt * dx *
     #* 3 * dy ** 5 + 0.87E2 * cc * dt * dx ** 2 * dy ** 6 - 0.108E3 * d
     #y ** 5 * dx ** 4) / dy ** 7 / dx ** 4 / 0.720E3
      cvu(5,8) = -dt * cc ** 2 * (0.4E1 * dt ** 5 * cc ** 5 * dx ** 2 + 
     #cc ** 5 * dt ** 5 * dx * dy + 0.3E1 * cc ** 5 * dt ** 5 * dy ** 2 
     #- 0.6E1 * cc ** 4 * dt ** 4 * dx ** 2 * dy - 0.40E2 * dt ** 3 * cc
     # ** 3 * dx ** 2 * dy ** 2 - 0.5E1 * cc ** 3 * dt ** 3 * dx * dy **
     # 3 - 0.15E2 * cc ** 3 * dt ** 3 * dy ** 4 + 0.20E2 * cc ** 2 * dt 
     #** 2 * dx ** 2 * dy ** 3 + 0.84E2 * cc * dt * dx ** 2 * dy ** 4 + 
     #0.4E1 * cc * dt * dx * dy ** 5 + 0.12E2 * cc * dt * dy ** 6 - 0.8E
     #1 * dx ** 2 * dy ** 5) / dy ** 7 / dx ** 2 / 0.720E3
      cvu(5,9) = dt ** 2 * cc ** 3 * (dt ** 4 * cc ** 4 - 0.10E2 * cc **
     # 2 * dt ** 2 * dy ** 2 + 0.21E2 * dy ** 4) / dy ** 7 / 0.1440E4
      cvu(6,2) = dt ** 2 * cc ** 3 * (dt ** 4 * cc ** 4 - 0.5E1 * cc ** 
     #2 * dt ** 2 * dy ** 2 + 0.4E1 * dy ** 4) * (dx + 0.3E1 * dy) / dy 
     #** 6 / dx ** 2 / 0.1440E4
      cvu(6,3) = -dt ** 2 * cc ** 3 * (0.3E1 * cc ** 4 * dt ** 4 * dx **
     # 3 + 0.9E1 * cc ** 4 * dt ** 4 * dx ** 2 * dy + 0.6E1 * cc ** 4 * 
     #dt ** 4 * dx * dy ** 2 + 0.6E1 * cc ** 4 * dt ** 4 * dy ** 3 - 0.1
     #8E2 * dt ** 3 * cc ** 3 * dx ** 2 * dy ** 2 - 0.25E2 * cc ** 2 * d
     #t ** 2 * dx ** 3 * dy ** 2 - 0.55E2 * cc ** 2 * dt ** 2 * dx ** 2 
     #* dy ** 3 - 0.10E2 * dt ** 2 * cc ** 2 * dy ** 4 * dx - 0.10E2 * c
     #c ** 2 * dt ** 2 * dy ** 5 + 0.20E2 * cc * dt * dx ** 2 * dy ** 4 
     #+ 0.22E2 * dx ** 3 * dy ** 4 + 0.46E2 * dx ** 2 * dy ** 5) / dy **
     # 6 / dx ** 4 / 0.720E3
      cvu(6,4) = dt ** 2 * cc ** 3 * (0.15E2 * cc ** 4 * dt ** 4 * dx **
     # 5 + 0.45E2 * cc ** 4 * dt ** 4 * dx ** 4 * dy + 0.48E2 * cc ** 4 
     #* dt ** 4 * dx ** 3 * dy ** 2 + 0.48E2 * cc ** 4 * dt ** 4 * dx **
     # 2 * dy ** 3 + 0.45E2 * cc ** 4 * dt ** 4 * dx * dy ** 4 + 0.15E2 
     #* cc ** 4 * dt ** 4 * dy ** 5 - 0.144E3 * cc ** 3 * dt ** 3 * dx *
     #* 4 * dy ** 2 - 0.144E3 * cc ** 3 * dt ** 3 * dx ** 2 * dy ** 4 - 
     #0.155E3 * cc ** 2 * dt ** 2 * dx ** 5 * dy ** 2 - 0.305E3 * cc ** 
     #2 * dt ** 2 * dx ** 4 * dy ** 3 - 0.305E3 * cc ** 2 * dt ** 2 * dx
     # ** 3 * dy ** 4 - 0.155E3 * cc ** 2 * dt ** 2 * dx ** 2 * dy ** 5 
     #+ 0.800E3 * dt * cc * dx ** 4 * dy ** 4 + 0.320E3 * dx ** 5 * dy *
     #* 4 + 0.320E3 * dy ** 5 * dx ** 4) / dy ** 6 / dx ** 6 / 0.1440E4
      cvu(6,5) = -cc ** 2 * dt * (0.10E2 * cc ** 5 * dt ** 5 * dx ** 6 +
     # 0.30E2 * cc ** 5 * dt ** 5 * dx ** 5 * dy + 0.36E2 * cc ** 5 * dt
     # ** 5 * dx ** 4 * dy ** 2 + 0.36E2 * cc ** 5 * dt ** 5 * dx ** 3 *
     # dy ** 3 + 0.45E2 * cc ** 5 * dt ** 5 * dx ** 2 * dy ** 4 + 0.15E2
     # * cc ** 5 * dt ** 5 * dx * dy ** 5 + 0.28E2 * cc ** 5 * dt ** 5 *
     # dy ** 6 - 0.108E3 * cc ** 4 * dt ** 4 * dx ** 5 * dy ** 2 - 0.144
     #E3 * cc ** 4 * dt ** 4 * dx ** 3 * dy ** 4 - 0.90E2 * cc ** 4 * dt
     # ** 4 * dx * dy ** 6 - 0.110E3 * cc ** 3 * dt ** 3 * dx ** 6 * dy 
     #** 2 - 0.210E3 * cc ** 3 * dt ** 3 * dx ** 5 * dy ** 3 - 0.285E3 *
     # cc ** 3 * dt ** 3 * dx ** 4 * dy ** 4 - 0.135E3 * cc ** 3 * dt **
     # 3 * dx ** 3 * dy ** 5 - 0.280E3 * cc ** 3 * dt ** 3 * dx ** 2 * d
     #y ** 6 + 0.760E3 * cc ** 2 * dt ** 2 * dx ** 5 * dy ** 4 + 0.780E3
     # * cc ** 2 * dt ** 2 * dx ** 3 * dy ** 6 + 0.280E3 * dt * cc * dx 
     #** 6 * dy ** 4 + 0.240E3 * cc * dt * dx ** 5 * dy ** 5 + 0.588E3 *
     # cc * dt * dx ** 4 * dy ** 6 - 0.1080E4 * dx ** 5 * dy ** 6) / dx 
     #** 7 / dy ** 6 / 0.720E3
      cvu(6,6) = dt ** 2 * cc ** 3 * (0.15E2 * cc ** 4 * dt ** 4 * dx **
     # 5 + 0.45E2 * cc ** 4 * dt ** 4 * dx ** 4 * dy + 0.48E2 * cc ** 4 
     #* dt ** 4 * dx ** 3 * dy ** 2 + 0.48E2 * cc ** 4 * dt ** 4 * dx **
     # 2 * dy ** 3 + 0.45E2 * cc ** 4 * dt ** 4 * dx * dy ** 4 + 0.15E2 
     #* cc ** 4 * dt ** 4 * dy ** 5 - 0.144E3 * cc ** 3 * dt ** 3 * dx *
     #* 4 * dy ** 2 - 0.144E3 * cc ** 3 * dt ** 3 * dx ** 2 * dy ** 4 - 
     #0.155E3 * cc ** 2 * dt ** 2 * dx ** 5 * dy ** 2 - 0.305E3 * cc ** 
     #2 * dt ** 2 * dx ** 4 * dy ** 3 - 0.305E3 * cc ** 2 * dt ** 2 * dx
     # ** 3 * dy ** 4 - 0.155E3 * cc ** 2 * dt ** 2 * dx ** 2 * dy ** 5 
     #+ 0.800E3 * dt * cc * dx ** 4 * dy ** 4 + 0.320E3 * dx ** 5 * dy *
     #* 4 + 0.320E3 * dy ** 5 * dx ** 4) / dy ** 6 / dx ** 6 / 0.1440E4
      cvu(6,7) = -dt ** 2 * cc ** 3 * (0.3E1 * cc ** 4 * dt ** 4 * dx **
     # 3 + 0.9E1 * cc ** 4 * dt ** 4 * dx ** 2 * dy + 0.6E1 * cc ** 4 * 
     #dt ** 4 * dx * dy ** 2 + 0.6E1 * cc ** 4 * dt ** 4 * dy ** 3 - 0.1
     #8E2 * dt ** 3 * cc ** 3 * dx ** 2 * dy ** 2 - 0.25E2 * cc ** 2 * d
     #t ** 2 * dx ** 3 * dy ** 2 - 0.55E2 * cc ** 2 * dt ** 2 * dx ** 2 
     #* dy ** 3 - 0.10E2 * dt ** 2 * cc ** 2 * dy ** 4 * dx - 0.10E2 * c
     #c ** 2 * dt ** 2 * dy ** 5 + 0.20E2 * cc * dt * dx ** 2 * dy ** 4 
     #+ 0.22E2 * dx ** 3 * dy ** 4 + 0.46E2 * dx ** 2 * dy ** 5) / dy **
     # 6 / dx ** 4 / 0.720E3
      cvu(6,8) = dt ** 2 * cc ** 3 * (dt ** 4 * cc ** 4 - 0.5E1 * cc ** 
     #2 * dt ** 2 * dy ** 2 + 0.4E1 * dy ** 4) * (dx + 0.3E1 * dy) / dy 
     #** 6 / dx ** 2 / 0.1440E4
      cvu(7,3) = dt ** 2 * cc ** 3 * (0.3E1 * dt ** 4 * cc ** 4 - 0.5E1 
     #* cc ** 2 * dt ** 2 * dx ** 2 - 0.5E1 * cc ** 2 * dt ** 2 * dy ** 
     #2 + 0.5E1 * dx ** 2 * dy ** 2) * (dx + dy) / dy ** 4 / dx ** 4 / 0
     #.1440E4
      cvu(7,4) = -dt ** 2 * cc ** 3 * (0.6E1 * cc ** 4 * dt ** 4 * dx **
     # 3 + 0.6E1 * cc ** 4 * dt ** 4 * dx ** 2 * dy + 0.9E1 * cc ** 4 * 
     #dt ** 4 * dx * dy ** 2 + 0.3E1 * cc ** 4 * dt ** 4 * dy ** 3 - 0.1
     #8E2 * dt ** 3 * cc ** 3 * dx ** 2 * dy ** 2 - 0.10E2 * cc ** 2 * d
     #t ** 2 * dx ** 5 - 0.10E2 * cc ** 2 * dt ** 2 * dx ** 4 * dy - 0.5
     #5E2 * cc ** 2 * dt ** 2 * dx ** 3 * dy ** 2 - 0.25E2 * cc ** 2 * d
     #t ** 2 * dx ** 2 * dy ** 3 + 0.20E2 * cc * dt * dx ** 4 * dy ** 2 
     #+ 0.46E2 * dx ** 5 * dy ** 2 + 0.22E2 * dx ** 4 * dy ** 3) / dx **
     # 6 / dy ** 4 / 0.720E3
      cvu(7,5) = cc ** 2 * dt * (0.9E1 * cc ** 5 * dt ** 5 * dx ** 4 + 0
     #.9E1 * cc ** 5 * dt ** 5 * dx ** 3 * dy + 0.18E2 * cc ** 5 * dt **
     # 5 * dx ** 2 * dy ** 2 + 0.6E1 * cc ** 5 * dt ** 5 * dx * dy ** 3 
     #+ 0.14E2 * cc ** 5 * dt ** 5 * dy ** 4 - 0.36E2 * cc ** 4 * dt ** 
     #4 * dx ** 3 * dy ** 2 - 0.36E2 * cc ** 4 * dt ** 4 * dx * dy ** 4 
     #- 0.15E2 * cc ** 3 * dt ** 3 * dx ** 6 - 0.15E2 * cc ** 3 * dt ** 
     #3 * dx ** 5 * dy - 0.105E3 * cc ** 3 * dt ** 3 * dx ** 4 * dy ** 2
     # - 0.45E2 * cc ** 3 * dt ** 3 * dx ** 3 * dy ** 3 - 0.140E3 * cc *
     #* 3 * dt ** 3 * dx ** 2 * dy ** 4 + 0.40E2 * cc ** 2 * dt ** 2 * d
     #x ** 5 * dy ** 2 + 0.240E3 * cc ** 2 * dt ** 2 * dx ** 3 * dy ** 4
     # + 0.87E2 * cc * dt * dx ** 6 * dy ** 2 + 0.39E2 * cc * dt * dx **
     # 5 * dy ** 3 + 0.294E3 * dt * cc * dx ** 4 * dy ** 4 - 0.108E3 * d
     #x ** 5 * dy ** 4) / dx ** 7 / dy ** 4 / 0.720E3
      cvu(7,6) = -dt ** 2 * cc ** 3 * (0.6E1 * cc ** 4 * dt ** 4 * dx **
     # 3 + 0.6E1 * cc ** 4 * dt ** 4 * dx ** 2 * dy + 0.9E1 * cc ** 4 * 
     #dt ** 4 * dx * dy ** 2 + 0.3E1 * cc ** 4 * dt ** 4 * dy ** 3 - 0.1
     #8E2 * dt ** 3 * cc ** 3 * dx ** 2 * dy ** 2 - 0.10E2 * cc ** 2 * d
     #t ** 2 * dx ** 5 - 0.10E2 * cc ** 2 * dt ** 2 * dx ** 4 * dy - 0.5
     #5E2 * cc ** 2 * dt ** 2 * dx ** 3 * dy ** 2 - 0.25E2 * cc ** 2 * d
     #t ** 2 * dx ** 2 * dy ** 3 + 0.20E2 * cc * dt * dx ** 4 * dy ** 2 
     #+ 0.46E2 * dx ** 5 * dy ** 2 + 0.22E2 * dx ** 4 * dy ** 3) / dx **
     # 6 / dy ** 4 / 0.720E3
      cvu(7,7) = dt ** 2 * cc ** 3 * (0.3E1 * dt ** 4 * cc ** 4 - 0.5E1 
     #* cc ** 2 * dt ** 2 * dx ** 2 - 0.5E1 * cc ** 2 * dt ** 2 * dy ** 
     #2 + 0.5E1 * dx ** 2 * dy ** 2) * (dx + dy) / dy ** 4 / dx ** 4 / 0
     #.1440E4
      cvu(8,4) = dt ** 2 * cc ** 3 * (dt ** 4 * cc ** 4 - 0.5E1 * cc ** 
     #2 * dt ** 2 * dx ** 2 + 0.4E1 * dx ** 4) * (0.3E1 * dx + dy) / dx 
     #** 6 / dy ** 2 / 0.1440E4
      cvu(8,5) = -dt * cc ** 2 * (0.3E1 * dt ** 5 * cc ** 5 * dx ** 2 + 
     #cc ** 5 * dt ** 5 * dx * dy + 0.4E1 * cc ** 5 * dt ** 5 * dy ** 2 
     #- 0.6E1 * cc ** 4 * dt ** 4 * dx * dy ** 2 - 0.15E2 * cc ** 3 * dt
     # ** 3 * dx ** 4 - 0.5E1 * cc ** 3 * dt ** 3 * dx ** 3 * dy - 0.40E
     #2 * dt ** 3 * cc ** 3 * dx ** 2 * dy ** 2 + 0.20E2 * cc ** 2 * dt 
     #** 2 * dx ** 3 * dy ** 2 + 0.12E2 * cc * dt * dx ** 6 + 0.4E1 * cc
     # * dt * dx ** 5 * dy + 0.84E2 * cc * dt * dx ** 4 * dy ** 2 - 0.8E
     #1 * dx ** 5 * dy ** 2) / dx ** 7 / dy ** 2 / 0.720E3
      cvu(8,6) = dt ** 2 * cc ** 3 * (dt ** 4 * cc ** 4 - 0.5E1 * cc ** 
     #2 * dt ** 2 * dx ** 2 + 0.4E1 * dx ** 4) * (0.3E1 * dx + dy) / dx 
     #** 6 / dy ** 2 / 0.1440E4
      cvu(9,5) = dt ** 2 * cc ** 3 * (dt ** 4 * cc ** 4 - 0.10E2 * cc **
     # 2 * dt ** 2 * dx ** 2 + 0.21E2 * dx ** 4) / dx ** 7 / 0.1440E4
c
      return
      end
c
c++++++++++++++++
c
      subroutine getcvv_sixth2D( 
     *   cc,dx,dy,dt,cvv )
c
      implicit real (t)
      real cvv(9,9)
      real dx,dy,dt,cc
c
      cvv(1,5) = -cc * dt * (0.9E1 * dt ** 4 * cc ** 4 - 0.53E2 * cc ** 
     #2 * dt ** 2 * dx ** 2 + 0.32E2 * dx ** 4) / dx ** 5 / 0.8640E4
      cvv(2,4) = -dt ** 3 * cc ** 3 * (0.9E1 * dt ** 2 * cc ** 2 - 0.15E
     #2 * dx ** 2 - 0.4E1 * dx * dy) / dx ** 3 / dy ** 2 / 0.4320E4
      cvv(2,5) = cc * dt * (0.24E2 * cc ** 5 * dt ** 5 * dy ** 2 + 0.72E
     #2 * cc ** 4 * dt ** 4 * dx ** 3 + 0.144E3 * cc ** 4 * dt ** 4 * dx
     # * dy ** 2 - 0.105E3 * dt ** 3 * cc ** 3 * dx ** 2 * dy ** 2 - 0.1
     #20E3 * cc ** 2 * dt ** 2 * dx ** 5 - 0.32E2 * cc ** 2 * dt ** 2 * 
     #dx ** 4 * dy - 0.848E3 * cc ** 2 * dt ** 2 * dx ** 3 * dy ** 2 + 0
     #.96E2 * cc * dt * dx ** 4 * dy ** 2 + 0.512E3 * dx ** 5 * dy ** 2)
     # / dx ** 6 / dy ** 2 / 0.17280E5
      cvv(2,6) = -dt ** 3 * cc ** 3 * (0.9E1 * dt ** 2 * cc ** 2 - 0.15E
     #2 * dx ** 2 - 0.4E1 * dx * dy) / dx ** 3 / dy ** 2 / 0.4320E4
      cvv(3,3) = -dt ** 3 * cc ** 3 * (0.3E1 * cc ** 2 * dt ** 2 * dx **
     # 3 + 0.3E1 * cc ** 2 * dt ** 2 * dy ** 3 - 0.5E1 * dx ** 3 * dy **
     # 2 - 0.5E1 * dx ** 2 * dy ** 3) / dy ** 4 / dx ** 4 / 0.2880E4
      cvv(3,4) = dt ** 3 * cc ** 3 * (0.24E2 * cc ** 3 * dt ** 3 * dy **
     # 2 + 0.24E2 * cc ** 2 * dt ** 2 * dx ** 3 + 0.72E2 * dt ** 2 * cc 
     #** 2 * dx * dy ** 2 + 0.24E2 * cc ** 2 * dt ** 2 * dy ** 3 - 0.35E
     #2 * cc * dt * dx ** 2 * dy ** 2 - 0.160E3 * dx ** 3 * dy ** 2 - 0.
     #72E2 * dx ** 2 * dy ** 3) / dx ** 4 / dy ** 4 / 0.5760E4
      cvv(3,5) = -cc * dt * (0.72E2 * cc ** 5 * dt ** 5 * dx ** 2 * dy *
     #* 2 + 0.72E2 * cc ** 5 * dt ** 5 * dy ** 4 + 0.54E2 * cc ** 4 * dt
     # ** 4 * dx ** 5 + 0.216E3 * cc ** 4 * dt ** 4 * dx ** 3 * dy ** 2 
     #+ 0.54E2 * cc ** 4 * dt ** 4 * dx ** 2 * dy ** 3 + 0.252E3 * cc **
     # 4 * dt ** 4 * dx * dy ** 4 - 0.105E3 * cc ** 3 * dt ** 3 * dx ** 
     #4 * dy ** 2 - 0.675E3 * cc ** 3 * dt ** 3 * dx ** 2 * dy ** 4 - 0.
     #450E3 * cc ** 2 * dt ** 2 * dx ** 5 * dy ** 2 - 0.186E3 * cc ** 2 
     #* dt ** 2 * dx ** 4 * dy ** 3 - 0.1484E4 * cc ** 2 * dt ** 2 * dx 
     #** 3 * dy ** 4 + 0.648E3 * dt * cc * dx ** 4 * dy ** 4 + 0.896E3 *
     # dx ** 5 * dy ** 4) / dx ** 6 / dy ** 4 / 0.8640E4
      cvv(3,6) = dt ** 3 * cc ** 3 * (0.24E2 * cc ** 3 * dt ** 3 * dy **
     # 2 + 0.24E2 * cc ** 2 * dt ** 2 * dx ** 3 + 0.72E2 * dt ** 2 * cc 
     #** 2 * dx * dy ** 2 + 0.24E2 * cc ** 2 * dt ** 2 * dy ** 3 - 0.35E
     #2 * cc * dt * dx ** 2 * dy ** 2 - 0.160E3 * dx ** 3 * dy ** 2 - 0.
     #72E2 * dx ** 2 * dy ** 3) / dx ** 4 / dy ** 4 / 0.5760E4
      cvv(3,7) = -dt ** 3 * cc ** 3 * (0.3E1 * cc ** 2 * dt ** 2 * dx **
     # 3 + 0.3E1 * cc ** 2 * dt ** 2 * dy ** 3 - 0.5E1 * dx ** 3 * dy **
     # 2 - 0.5E1 * dx ** 2 * dy ** 3) / dy ** 4 / dx ** 4 / 0.2880E4
      cvv(4,2) = -dt ** 3 * cc ** 3 * (0.9E1 * dt ** 2 * cc ** 2 - 0.4E1
     # * dx * dy - 0.15E2 * dy ** 2) / dx ** 2 / dy ** 3 / 0.4320E4
      cvv(4,3) = dt ** 3 * cc ** 3 * (0.24E2 * cc ** 3 * dt ** 3 * dx **
     # 2 + 0.24E2 * cc ** 2 * dt ** 2 * dx ** 3 + 0.72E2 * cc ** 2 * dt 
     #** 2 * dx ** 2 * dy + 0.24E2 * cc ** 2 * dt ** 2 * dy ** 3 - 0.35E
     #2 * cc * dt * dx ** 2 * dy ** 2 - 0.72E2 * dx ** 3 * dy ** 2 - 0.1
     #60E3 * dx ** 2 * dy ** 3) / dx ** 4 / dy ** 4 / 0.5760E4
      cvv(4,4) = -dt ** 3 * cc ** 3 * (0.24E2 * cc ** 3 * dt ** 3 * dx *
     #* 2 + 0.24E2 * cc ** 3 * dt ** 3 * dy ** 2 + 0.24E2 * cc ** 2 * dt
     # ** 2 * dx ** 3 + 0.45E2 * cc ** 2 * dt ** 2 * dx ** 2 * dy + 0.45
     #E2 * dt ** 2 * cc ** 2 * dx * dy ** 2 + 0.24E2 * cc ** 2 * dt ** 2
     # * dy ** 3 - 0.190E3 * cc * dt * dx ** 2 * dy ** 2 - 0.135E3 * dx 
     #** 3 * dy ** 2 - 0.135E3 * dx ** 2 * dy ** 3) / dy ** 4 / dx ** 4 
     #/ 0.1440E4
      cvv(4,5) = cc * dt * (0.432E3 * cc ** 5 * dt ** 5 * dx ** 4 + 0.57
     #6E3 * cc ** 5 * dt ** 5 * dx ** 2 * dy ** 2 + 0.360E3 * cc ** 5 * 
     #dt ** 5 * dy ** 4 + 0.432E3 * cc ** 4 * dt ** 4 * dx ** 5 + 0.720E
     #3 * cc ** 4 * dt ** 4 * dx ** 4 * dy + 0.1080E4 * cc ** 4 * dt ** 
     #4 * dx ** 3 * dy ** 2 + 0.432E3 * cc ** 4 * dt ** 4 * dx ** 2 * dy
     # ** 3 + 0.1008E4 * cc ** 4 * dt ** 4 * dx * dy ** 4 - 0.4350E4 * c
     #c ** 3 * dt ** 3 * dx ** 4 * dy ** 2 - 0.4455E4 * cc ** 3 * dt ** 
     #3 * dx ** 2 * dy ** 4 - 0.2840E4 * cc ** 2 * dt ** 2 * dx ** 5 * d
     #y ** 2 - 0.2400E4 * cc ** 2 * dt ** 2 * dx ** 4 * dy ** 3 - 0.5936
     #E4 * cc ** 2 * dt ** 2 * dx ** 3 * dy ** 4 + 0.12960E5 * dt * cc *
     # dx ** 4 * dy ** 4 + 0.3584E4 * dx ** 5 * dy ** 4) / dx ** 6 / dy 
     #** 4 / 0.17280E5
      cvv(4,6) = -dt ** 3 * cc ** 3 * (0.24E2 * cc ** 3 * dt ** 3 * dx *
     #* 2 + 0.24E2 * cc ** 3 * dt ** 3 * dy ** 2 + 0.24E2 * cc ** 2 * dt
     # ** 2 * dx ** 3 + 0.45E2 * cc ** 2 * dt ** 2 * dx ** 2 * dy + 0.45
     #E2 * dt ** 2 * cc ** 2 * dx * dy ** 2 + 0.24E2 * cc ** 2 * dt ** 2
     # * dy ** 3 - 0.190E3 * cc * dt * dx ** 2 * dy ** 2 - 0.135E3 * dx 
     #** 3 * dy ** 2 - 0.135E3 * dx ** 2 * dy ** 3) / dy ** 4 / dx ** 4 
     #/ 0.1440E4
      cvv(4,7) = dt ** 3 * cc ** 3 * (0.24E2 * cc ** 3 * dt ** 3 * dx **
     # 2 + 0.24E2 * cc ** 2 * dt ** 2 * dx ** 3 + 0.72E2 * cc ** 2 * dt 
     #** 2 * dx ** 2 * dy + 0.24E2 * cc ** 2 * dt ** 2 * dy ** 3 - 0.35E
     #2 * cc * dt * dx ** 2 * dy ** 2 - 0.72E2 * dx ** 3 * dy ** 2 - 0.1
     #60E3 * dx ** 2 * dy ** 3) / dx ** 4 / dy ** 4 / 0.5760E4
      cvv(4,8) = -dt ** 3 * cc ** 3 * (0.9E1 * dt ** 2 * cc ** 2 - 0.4E1
     # * dx * dy - 0.15E2 * dy ** 2) / dx ** 2 / dy ** 3 / 0.4320E4
      cvv(5,1) = -cc * dt * (0.9E1 * dt ** 4 * cc ** 4 - 0.53E2 * cc ** 
     #2 * dt ** 2 * dy ** 2 + 0.32E2 * dy ** 4) / dy ** 5 / 0.8640E4
      cvv(5,2) = dt * cc * (0.24E2 * dt ** 5 * cc ** 5 * dx ** 2 + 0.144
     #E3 * cc ** 4 * dt ** 4 * dx ** 2 * dy + 0.72E2 * cc ** 4 * dt ** 4
     # * dy ** 3 - 0.105E3 * dt ** 3 * cc ** 3 * dx ** 2 * dy ** 2 - 0.8
     #48E3 * cc ** 2 * dt ** 2 * dx ** 2 * dy ** 3 - 0.32E2 * dt ** 2 * 
     #cc ** 2 * dy ** 4 * dx - 0.120E3 * cc ** 2 * dt ** 2 * dy ** 5 + 0
     #.96E2 * cc * dt * dx ** 2 * dy ** 4 + 0.512E3 * dx ** 2 * dy ** 5)
     # / dy ** 6 / dx ** 2 / 0.17280E5
      cvv(5,3) = -dt * cc * (0.72E2 * cc ** 5 * dt ** 5 * dx ** 4 + 0.72
     #E2 * cc ** 5 * dt ** 5 * dx ** 2 * dy ** 2 + 0.252E3 * cc ** 4 * d
     #t ** 4 * dx ** 4 * dy + 0.54E2 * cc ** 4 * dt ** 4 * dx ** 3 * dy 
     #** 2 + 0.216E3 * cc ** 4 * dt ** 4 * dx ** 2 * dy ** 3 + 0.54E2 * 
     #cc ** 4 * dt ** 4 * dy ** 5 - 0.675E3 * cc ** 3 * dt ** 3 * dx ** 
     #4 * dy ** 2 - 0.105E3 * cc ** 3 * dt ** 3 * dx ** 2 * dy ** 4 - 0.
     #1484E4 * cc ** 2 * dt ** 2 * dx ** 4 * dy ** 3 - 0.186E3 * cc ** 2
     # * dt ** 2 * dx ** 3 * dy ** 4 - 0.450E3 * cc ** 2 * dt ** 2 * dx 
     #** 2 * dy ** 5 + 0.648E3 * dt * cc * dx ** 4 * dy ** 4 + 0.896E3 *
     # dy ** 5 * dx ** 4) / dy ** 6 / dx ** 4 / 0.8640E4
      cvv(5,4) = dt * cc * (0.360E3 * cc ** 5 * dt ** 5 * dx ** 4 + 0.57
     #6E3 * cc ** 5 * dt ** 5 * dx ** 2 * dy ** 2 + 0.432E3 * cc ** 5 * 
     #dt ** 5 * dy ** 4 + 0.1008E4 * cc ** 4 * dt ** 4 * dx ** 4 * dy + 
     #0.432E3 * cc ** 4 * dt ** 4 * dx ** 3 * dy ** 2 + 0.1080E4 * cc **
     # 4 * dt ** 4 * dx ** 2 * dy ** 3 + 0.720E3 * cc ** 4 * dt ** 4 * d
     #x * dy ** 4 + 0.432E3 * cc ** 4 * dt ** 4 * dy ** 5 - 0.4455E4 * c
     #c ** 3 * dt ** 3 * dx ** 4 * dy ** 2 - 0.4350E4 * cc ** 3 * dt ** 
     #3 * dx ** 2 * dy ** 4 - 0.5936E4 * cc ** 2 * dt ** 2 * dx ** 4 * d
     #y ** 3 - 0.2400E4 * cc ** 2 * dt ** 2 * dx ** 3 * dy ** 4 - 0.2840
     #E4 * cc ** 2 * dt ** 2 * dx ** 2 * dy ** 5 + 0.12960E5 * dt * cc *
     # dx ** 4 * dy ** 4 + 0.3584E4 * dy ** 5 * dx ** 4) / dy ** 6 / dx 
     #** 4 / 0.17280E5
      cvv(5,5) = -(0.120E3 * cc ** 6 * dt ** 6 * dx ** 6 + 0.216E3 * cc 
     #** 6 * dt ** 6 * dx ** 4 * dy ** 2 + 0.216E3 * cc ** 6 * dt ** 6 *
     # dx ** 2 * dy ** 4 + 0.120E3 * cc ** 6 * dt ** 6 * dy ** 6 + 0.315
     #E3 * cc ** 5 * dt ** 5 * dx ** 6 * dy + 0.162E3 * cc ** 5 * dt ** 
     #5 * dx ** 5 * dy ** 2 + 0.360E3 * cc ** 5 * dt ** 5 * dx ** 4 * dy
     # ** 3 + 0.360E3 * cc ** 5 * dt ** 5 * dx ** 3 * dy ** 4 + 0.162E3 
     #* cc ** 5 * dt ** 5 * dx ** 2 * dy ** 5 + 0.315E3 * cc ** 5 * dt *
     #* 5 * dx * dy ** 6 - 0.1605E4 * cc ** 4 * dt ** 4 * dx ** 6 * dy *
     #* 2 - 0.2070E4 * cc ** 4 * dt ** 4 * dx ** 4 * dy ** 4 - 0.1605E4 
     #* cc ** 4 * dt ** 4 * dx ** 2 * dy ** 6 - 0.1855E4 * cc ** 3 * dt 
     #** 3 * dx ** 6 * dy ** 3 - 0.1030E4 * cc ** 3 * dt ** 3 * dx ** 5 
     #* dy ** 4 - 0.1030E4 * cc ** 3 * dt ** 3 * dx ** 4 * dy ** 5 - 0.1
     #855E4 * cc ** 3 * dt ** 3 * dx ** 3 * dy ** 6 + 0.5880E4 * cc ** 2
     # * dt ** 2 * dx ** 6 * dy ** 4 + 0.5880E4 * cc ** 2 * dt ** 2 * dx
     # ** 4 * dy ** 6 + 0.1120E4 * cc * dt * dx ** 6 * dy ** 5 + 0.1120E
     #4 * cc * dt * dx ** 5 * dy ** 6 - 0.4320E4 * dx ** 6 * dy ** 6) / 
     #dx ** 6 / dy ** 6 / 0.4320E4
      cvv(5,6) = dt * cc * (0.360E3 * cc ** 5 * dt ** 5 * dx ** 4 + 0.57
     #6E3 * cc ** 5 * dt ** 5 * dx ** 2 * dy ** 2 + 0.432E3 * cc ** 5 * 
     #dt ** 5 * dy ** 4 + 0.1008E4 * cc ** 4 * dt ** 4 * dx ** 4 * dy + 
     #0.432E3 * cc ** 4 * dt ** 4 * dx ** 3 * dy ** 2 + 0.1080E4 * cc **
     # 4 * dt ** 4 * dx ** 2 * dy ** 3 + 0.720E3 * cc ** 4 * dt ** 4 * d
     #x * dy ** 4 + 0.432E3 * cc ** 4 * dt ** 4 * dy ** 5 - 0.4455E4 * c
     #c ** 3 * dt ** 3 * dx ** 4 * dy ** 2 - 0.4350E4 * cc ** 3 * dt ** 
     #3 * dx ** 2 * dy ** 4 - 0.5936E4 * cc ** 2 * dt ** 2 * dx ** 4 * d
     #y ** 3 - 0.2400E4 * cc ** 2 * dt ** 2 * dx ** 3 * dy ** 4 - 0.2840
     #E4 * cc ** 2 * dt ** 2 * dx ** 2 * dy ** 5 + 0.12960E5 * dt * cc *
     # dx ** 4 * dy ** 4 + 0.3584E4 * dy ** 5 * dx ** 4) / dy ** 6 / dx 
     #** 4 / 0.17280E5
      cvv(5,7) = -dt * cc * (0.72E2 * cc ** 5 * dt ** 5 * dx ** 4 + 0.72
     #E2 * cc ** 5 * dt ** 5 * dx ** 2 * dy ** 2 + 0.252E3 * cc ** 4 * d
     #t ** 4 * dx ** 4 * dy + 0.54E2 * cc ** 4 * dt ** 4 * dx ** 3 * dy 
     #** 2 + 0.216E3 * cc ** 4 * dt ** 4 * dx ** 2 * dy ** 3 + 0.54E2 * 
     #cc ** 4 * dt ** 4 * dy ** 5 - 0.675E3 * cc ** 3 * dt ** 3 * dx ** 
     #4 * dy ** 2 - 0.105E3 * cc ** 3 * dt ** 3 * dx ** 2 * dy ** 4 - 0.
     #1484E4 * cc ** 2 * dt ** 2 * dx ** 4 * dy ** 3 - 0.186E3 * cc ** 2
     # * dt ** 2 * dx ** 3 * dy ** 4 - 0.450E3 * cc ** 2 * dt ** 2 * dx 
     #** 2 * dy ** 5 + 0.648E3 * dt * cc * dx ** 4 * dy ** 4 + 0.896E3 *
     # dy ** 5 * dx ** 4) / dy ** 6 / dx ** 4 / 0.8640E4
      cvv(5,8) = dt * cc * (0.24E2 * dt ** 5 * cc ** 5 * dx ** 2 + 0.144
     #E3 * cc ** 4 * dt ** 4 * dx ** 2 * dy + 0.72E2 * cc ** 4 * dt ** 4
     # * dy ** 3 - 0.105E3 * dt ** 3 * cc ** 3 * dx ** 2 * dy ** 2 - 0.8
     #48E3 * cc ** 2 * dt ** 2 * dx ** 2 * dy ** 3 - 0.32E2 * dt ** 2 * 
     #cc ** 2 * dy ** 4 * dx - 0.120E3 * cc ** 2 * dt ** 2 * dy ** 5 + 0
     #.96E2 * cc * dt * dx ** 2 * dy ** 4 + 0.512E3 * dx ** 2 * dy ** 5)
     # / dy ** 6 / dx ** 2 / 0.17280E5
      cvv(5,9) = -cc * dt * (0.9E1 * dt ** 4 * cc ** 4 - 0.53E2 * cc ** 
     #2 * dt ** 2 * dy ** 2 + 0.32E2 * dy ** 4) / dy ** 5 / 0.8640E4
      cvv(6,2) = -dt ** 3 * cc ** 3 * (0.9E1 * dt ** 2 * cc ** 2 - 0.4E1
     # * dx * dy - 0.15E2 * dy ** 2) / dx ** 2 / dy ** 3 / 0.4320E4
      cvv(6,3) = dt ** 3 * cc ** 3 * (0.24E2 * cc ** 3 * dt ** 3 * dx **
     # 2 + 0.24E2 * cc ** 2 * dt ** 2 * dx ** 3 + 0.72E2 * cc ** 2 * dt 
     #** 2 * dx ** 2 * dy + 0.24E2 * cc ** 2 * dt ** 2 * dy ** 3 - 0.35E
     #2 * cc * dt * dx ** 2 * dy ** 2 - 0.72E2 * dx ** 3 * dy ** 2 - 0.1
     #60E3 * dx ** 2 * dy ** 3) / dx ** 4 / dy ** 4 / 0.5760E4
      cvv(6,4) = -dt ** 3 * cc ** 3 * (0.24E2 * cc ** 3 * dt ** 3 * dx *
     #* 2 + 0.24E2 * cc ** 3 * dt ** 3 * dy ** 2 + 0.24E2 * cc ** 2 * dt
     # ** 2 * dx ** 3 + 0.45E2 * cc ** 2 * dt ** 2 * dx ** 2 * dy + 0.45
     #E2 * dt ** 2 * cc ** 2 * dx * dy ** 2 + 0.24E2 * cc ** 2 * dt ** 2
     # * dy ** 3 - 0.190E3 * cc * dt * dx ** 2 * dy ** 2 - 0.135E3 * dx 
     #** 3 * dy ** 2 - 0.135E3 * dx ** 2 * dy ** 3) / dy ** 4 / dx ** 4 
     #/ 0.1440E4
      cvv(6,5) = cc * dt * (0.432E3 * cc ** 5 * dt ** 5 * dx ** 4 + 0.57
     #6E3 * cc ** 5 * dt ** 5 * dx ** 2 * dy ** 2 + 0.360E3 * cc ** 5 * 
     #dt ** 5 * dy ** 4 + 0.432E3 * cc ** 4 * dt ** 4 * dx ** 5 + 0.720E
     #3 * cc ** 4 * dt ** 4 * dx ** 4 * dy + 0.1080E4 * cc ** 4 * dt ** 
     #4 * dx ** 3 * dy ** 2 + 0.432E3 * cc ** 4 * dt ** 4 * dx ** 2 * dy
     # ** 3 + 0.1008E4 * cc ** 4 * dt ** 4 * dx * dy ** 4 - 0.4350E4 * c
     #c ** 3 * dt ** 3 * dx ** 4 * dy ** 2 - 0.4455E4 * cc ** 3 * dt ** 
     #3 * dx ** 2 * dy ** 4 - 0.2840E4 * cc ** 2 * dt ** 2 * dx ** 5 * d
     #y ** 2 - 0.2400E4 * cc ** 2 * dt ** 2 * dx ** 4 * dy ** 3 - 0.5936
     #E4 * cc ** 2 * dt ** 2 * dx ** 3 * dy ** 4 + 0.12960E5 * dt * cc *
     # dx ** 4 * dy ** 4 + 0.3584E4 * dx ** 5 * dy ** 4) / dx ** 6 / dy 
     #** 4 / 0.17280E5
      cvv(6,6) = -dt ** 3 * cc ** 3 * (0.24E2 * cc ** 3 * dt ** 3 * dx *
     #* 2 + 0.24E2 * cc ** 3 * dt ** 3 * dy ** 2 + 0.24E2 * cc ** 2 * dt
     # ** 2 * dx ** 3 + 0.45E2 * cc ** 2 * dt ** 2 * dx ** 2 * dy + 0.45
     #E2 * dt ** 2 * cc ** 2 * dx * dy ** 2 + 0.24E2 * cc ** 2 * dt ** 2
     # * dy ** 3 - 0.190E3 * cc * dt * dx ** 2 * dy ** 2 - 0.135E3 * dx 
     #** 3 * dy ** 2 - 0.135E3 * dx ** 2 * dy ** 3) / dy ** 4 / dx ** 4 
     #/ 0.1440E4
      cvv(6,7) = dt ** 3 * cc ** 3 * (0.24E2 * cc ** 3 * dt ** 3 * dx **
     # 2 + 0.24E2 * cc ** 2 * dt ** 2 * dx ** 3 + 0.72E2 * cc ** 2 * dt 
     #** 2 * dx ** 2 * dy + 0.24E2 * cc ** 2 * dt ** 2 * dy ** 3 - 0.35E
     #2 * cc * dt * dx ** 2 * dy ** 2 - 0.72E2 * dx ** 3 * dy ** 2 - 0.1
     #60E3 * dx ** 2 * dy ** 3) / dx ** 4 / dy ** 4 / 0.5760E4
      cvv(6,8) = -dt ** 3 * cc ** 3 * (0.9E1 * dt ** 2 * cc ** 2 - 0.4E1
     # * dx * dy - 0.15E2 * dy ** 2) / dx ** 2 / dy ** 3 / 0.4320E4
      cvv(7,3) = -dt ** 3 * cc ** 3 * (0.3E1 * cc ** 2 * dt ** 2 * dx **
     # 3 + 0.3E1 * cc ** 2 * dt ** 2 * dy ** 3 - 0.5E1 * dx ** 3 * dy **
     # 2 - 0.5E1 * dx ** 2 * dy ** 3) / dy ** 4 / dx ** 4 / 0.2880E4
      cvv(7,4) = dt ** 3 * cc ** 3 * (0.24E2 * cc ** 3 * dt ** 3 * dy **
     # 2 + 0.24E2 * cc ** 2 * dt ** 2 * dx ** 3 + 0.72E2 * dt ** 2 * cc 
     #** 2 * dx * dy ** 2 + 0.24E2 * cc ** 2 * dt ** 2 * dy ** 3 - 0.35E
     #2 * cc * dt * dx ** 2 * dy ** 2 - 0.160E3 * dx ** 3 * dy ** 2 - 0.
     #72E2 * dx ** 2 * dy ** 3) / dx ** 4 / dy ** 4 / 0.5760E4
      cvv(7,5) = -cc * dt * (0.72E2 * cc ** 5 * dt ** 5 * dx ** 2 * dy *
     #* 2 + 0.72E2 * cc ** 5 * dt ** 5 * dy ** 4 + 0.54E2 * cc ** 4 * dt
     # ** 4 * dx ** 5 + 0.216E3 * cc ** 4 * dt ** 4 * dx ** 3 * dy ** 2 
     #+ 0.54E2 * cc ** 4 * dt ** 4 * dx ** 2 * dy ** 3 + 0.252E3 * cc **
     # 4 * dt ** 4 * dx * dy ** 4 - 0.105E3 * cc ** 3 * dt ** 3 * dx ** 
     #4 * dy ** 2 - 0.675E3 * cc ** 3 * dt ** 3 * dx ** 2 * dy ** 4 - 0.
     #450E3 * cc ** 2 * dt ** 2 * dx ** 5 * dy ** 2 - 0.186E3 * cc ** 2 
     #* dt ** 2 * dx ** 4 * dy ** 3 - 0.1484E4 * cc ** 2 * dt ** 2 * dx 
     #** 3 * dy ** 4 + 0.648E3 * dt * cc * dx ** 4 * dy ** 4 + 0.896E3 *
     # dx ** 5 * dy ** 4) / dx ** 6 / dy ** 4 / 0.8640E4
      cvv(7,6) = dt ** 3 * cc ** 3 * (0.24E2 * cc ** 3 * dt ** 3 * dy **
     # 2 + 0.24E2 * cc ** 2 * dt ** 2 * dx ** 3 + 0.72E2 * dt ** 2 * cc 
     #** 2 * dx * dy ** 2 + 0.24E2 * cc ** 2 * dt ** 2 * dy ** 3 - 0.35E
     #2 * cc * dt * dx ** 2 * dy ** 2 - 0.160E3 * dx ** 3 * dy ** 2 - 0.
     #72E2 * dx ** 2 * dy ** 3) / dx ** 4 / dy ** 4 / 0.5760E4
      cvv(7,7) = -dt ** 3 * cc ** 3 * (0.3E1 * cc ** 2 * dt ** 2 * dx **
     # 3 + 0.3E1 * cc ** 2 * dt ** 2 * dy ** 3 - 0.5E1 * dx ** 3 * dy **
     # 2 - 0.5E1 * dx ** 2 * dy ** 3) / dy ** 4 / dx ** 4 / 0.2880E4
      cvv(8,4) = -dt ** 3 * cc ** 3 * (0.9E1 * dt ** 2 * cc ** 2 - 0.15E
     #2 * dx ** 2 - 0.4E1 * dx * dy) / dx ** 3 / dy ** 2 / 0.4320E4
      cvv(8,5) = cc * dt * (0.24E2 * cc ** 5 * dt ** 5 * dy ** 2 + 0.72E
     #2 * cc ** 4 * dt ** 4 * dx ** 3 + 0.144E3 * cc ** 4 * dt ** 4 * dx
     # * dy ** 2 - 0.105E3 * dt ** 3 * cc ** 3 * dx ** 2 * dy ** 2 - 0.1
     #20E3 * cc ** 2 * dt ** 2 * dx ** 5 - 0.32E2 * cc ** 2 * dt ** 2 * 
     #dx ** 4 * dy - 0.848E3 * cc ** 2 * dt ** 2 * dx ** 3 * dy ** 2 + 0
     #.96E2 * cc * dt * dx ** 4 * dy ** 2 + 0.512E3 * dx ** 5 * dy ** 2)
     # / dx ** 6 / dy ** 2 / 0.17280E5
      cvv(8,6) = -dt ** 3 * cc ** 3 * (0.9E1 * dt ** 2 * cc ** 2 - 0.15E
     #2 * dx ** 2 - 0.4E1 * dx * dy) / dx ** 3 / dy ** 2 / 0.4320E4
      cvv(9,5) = -cc * dt * (0.9E1 * dt ** 4 * cc ** 4 - 0.53E2 * cc ** 
     #2 * dt ** 2 * dx ** 2 + 0.32E2 * dx ** 4) / dx ** 5 / 0.8640E4
c
      return
      end
c
c++++++++++++++++
c

