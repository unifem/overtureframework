      subroutine duWaveGen2d2rc( 
     *   nd1a,nd1b,nd2a,nd2b,
     *   n1a,n1b,n2a,n2b,
     *   ndf4a,ndf4b,nComp,addForcing,
     *   u,ut,unew,utnew,
     *   src,
     *   dx,dy,dt,cc,
     *   useWhereMask,mask )
c
      implicit none
c
c.. declarations of incoming variables
      integer nd1a,nd1b,nd2a,nd2b
      integer n1a,n1b,n2a,n2b
      integer ndf4a,ndf4b,nComp,addForcing
      integer useWhereMask
      integer mask(nd1a:nd1b,nd2a:nd2b)

      real u(nd1a:nd1b,nd2a:nd2b)
      real ut(nd1a:nd1b,nd2a:nd2b)
      real unew(nd1a:nd1b,nd2a:nd2b)
      real utnew(nd1a:nd1b,nd2a:nd2b)
      real src (nd1a:nd1b,nd2a:nd2b,ndf4a:ndf4b,1:*)
      real dx,dy,dt,cc
c
c.. declarations of local variables
      integer i,j,ix,jy,n

      integer stencilOpt
c
      real cuu(-2:2,-2:2)
      real cuv(-2:2,-2:2)
      real cvu(-2:2,-2:2)
      real cvv(-2:2,-2:2)
c
      n = 1
      stencilOpt = 1
c
      if( n1a-nd1a .lt. 2 ) then
        write(6,*)'Grid not made with enough ghost cells'
        write(6,*)nd1a,n1a
        stop
      end if

      ! second order, cartesian, 2D
      if( addForcing.eq.0 )then

        if( stencilOpt .eq. 0 ) then
          if( useWhereMask.ne.0 ) then
            do j = n2a,n2b
            do i = n1a,n1b
            if( mask(i,j).gt.0 ) then
c
              call duStepWaveGen2d2rc( 
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
             call duStepWaveGen2d4rc( 
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

          call getcuu_second( cc,dx,dy,dt,cuu(-3,-3) )
          call getcuv_second( cc,dx,dy,dt,cuv(-3,-3) )
          call getcvu_second( cc,dx,dy,dt,cvu(-3,-3) )
          call getcvv_second( cc,dx,dy,dt,cvv(-3,-3) )

          if( useWhereMask.ne.0 ) then
            do j = n2a,n2b
            do i = n1a,n1b
            if( mask(i,j).gt.0 ) then
c
              unew(i,j) = 0.0
              utnew(i,j) = 0.0
c        
              do jy = -2,2
              do ix = -2+abs(jy),2-abs(jy)
                unew(i,j) = unew(i,j)+
     *             cuu(ix,jy)*u(i+ix,j+jy)+
     *             cuv(ix,jy)*ut(i+ix,j+jy)
c     
                utnew(i,j) = utnew(i,j)+
     *             cvu(ix,jy)*u(i+ix,j+jy)+
     *             cvv(ix,jy)*ut(i+ix,j+jy)
              end do
              end do
            
            end if
            end do
            end do

          else
            ! no mask
            do j = n2a,n2b
            do i = n1a,n1b
c   
              unew(i,j) = 0.0
              utnew(i,j) = 0.0
c     
              do jy = -2,2
              do ix = -2+abs(jy),2-abs(jy)
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

      else
        ! add forcing flag is set to true

        if( useWhereMask.ne.0 ) then
          do j = n2a,n2b
          do i = n1a,n1b
          if( mask(i,j).gt.0 ) then

            call duStepWaveGen2d2rc_tz( 
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
            call duStepWaveGen2d2rc_tz( 
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
      subroutine getcuu_second( 
     *   cc,dx,dy,dt,cuu )
c
      implicit real (t)
      real cuu(5,5)
      real dx,dy,dt,cc
c
      cuu(1,3) = dt ** 3 * cc ** 3 / dx ** 3 / 0.8E1
      cuu(2,2) = dt ** 3 * cc ** 3 * (dx + dy) / dy ** 2 / dx ** 2 / 0.8
     #E1
      cuu(2,3) = -dt ** 2 * cc ** 2 * (dt * cc * dx ** 2 + dt * cc * dx 
     #* dy + 0.2E1 * cc * dt * dy ** 2 - 0.2E1 * dy ** 2 * dx) / dy ** 2
     # / dx ** 3 / 0.4E1
      cuu(2,4) = dt ** 3 * cc ** 3 * (dx + dy) / dy ** 2 / dx ** 2 / 0.8
     #E1
      cuu(3,1) = dt ** 3 * cc ** 3 / dy ** 3 / 0.8E1
      cuu(3,2) = -dt ** 2 * cc ** 2 * (0.2E1 * dt * cc * dx ** 2 + dt * 
     #cc * dx * dy + cc * dt * dy ** 2 - 0.2E1 * dx ** 2 * dy) / dy ** 3
     # / dx ** 2 / 0.4E1
      cuu(3,3) = (0.3E1 * cc ** 3 * dt ** 3 * dx ** 3 + 0.2E1 * cc ** 3 
     #* dt ** 3 * dx ** 2 * dy + 0.2E1 * cc ** 3 * dt ** 3 * dx * dy ** 
     #2 + 0.3E1 * cc ** 3 * dt ** 3 * dy ** 3 - 0.4E1 * cc ** 2 * dt ** 
     #2 * dx ** 3 * dy - 0.4E1 * cc ** 2 * dt ** 2 * dx * dy ** 3 + 0.4E
     #1 * dx ** 3 * dy ** 3) / dy ** 3 / dx ** 3 / 0.4E1
      cuu(3,4) = -dt ** 2 * cc ** 2 * (0.2E1 * dt * cc * dx ** 2 + dt * 
     #cc * dx * dy + cc * dt * dy ** 2 - 0.2E1 * dx ** 2 * dy) / dy ** 3
     # / dx ** 2 / 0.4E1
      cuu(3,5) = dt ** 3 * cc ** 3 / dy ** 3 / 0.8E1
      cuu(4,2) = dt ** 3 * cc ** 3 * (dx + dy) / dy ** 2 / dx ** 2 / 0.8
     #E1
      cuu(4,3) = -dt ** 2 * cc ** 2 * (dt * cc * dx ** 2 + dt * cc * dx 
     #* dy + 0.2E1 * cc * dt * dy ** 2 - 0.2E1 * dy ** 2 * dx) / dy ** 2
     # / dx ** 3 / 0.4E1
      cuu(4,4) = dt ** 3 * cc ** 3 * (dx + dy) / dy ** 2 / dx ** 2 / 0.8
     #E1
      cuu(5,3) = dt ** 3 * cc ** 3 / dx ** 3 / 0.8E1

      return
      end
c
c++++++++++++++++
c
      subroutine getcuv_second( 
     *   cc,dx,dy,dt,cuv )
c
      implicit real (t)
      real cuv(5,5)
      real dx,dy,dt,cc
c

      cuv(1,3) = -cc * dt ** 2 / dx / 0.12E2
      cuv(2,3) = cc * dt ** 2 * (0.24E2 * dt * cc + 0.23E2 * dx) / dx **
     # 2 / 0.96E2
      cuv(3,1) = -cc * dt ** 2 / dy / 0.12E2
      cuv(3,2) = cc * dt ** 2 * (0.24E2 * dt * cc + 0.23E2 * dy) / dy **
     # 2 / 0.96E2
      cuv(3,3) = -dt * (0.3E1 * cc ** 2 * dt ** 2 * dx ** 2 + 0.3E1 * cc
     # ** 2 * dt ** 2 * dy ** 2 + 0.2E1 * cc * dt * dx ** 2 * dy + 0.2E1
     # * cc * dt * dx * dy ** 2 - 0.6E1 * dx ** 2 * dy ** 2) / dy ** 2 /
     # dx ** 2 / 0.6E1
      cuv(3,4) = cc * dt ** 2 * (0.24E2 * dt * cc + 0.23E2 * dy) / dy **
     # 2 / 0.96E2
      cuv(3,5) = -cc * dt ** 2 / dy / 0.12E2
      cuv(4,3) = cc * dt ** 2 * (0.24E2 * dt * cc + 0.23E2 * dx) / dx **
     # 2 / 0.96E2
      cuv(5,3) = -cc * dt ** 2 / dx / 0.12E2

      return
      end
c
c++++++++++++++++
c
      subroutine getcvu_second( 
     *   cc,dx,dy,dt,cvu )
c
      implicit real (t)
      real cvu(5,5)
      real dx,dy,dt,cc
c
      cvu(1,3) = dt ** 2 * cc ** 3 / dx ** 3 / 0.4E1
      cvu(2,2) = dt ** 2 * cc ** 3 * (dx + dy) / dy ** 2 / dx ** 2 / 0.4
     #E1
      cvu(2,3) = -cc ** 2 * dt * (dt * cc * dx ** 2 + dt * cc * dx * dy 
     #+ 0.2E1 * cc * dt * dy ** 2 - 0.2E1 * dy ** 2 * dx) / dy ** 2 / dx
     # ** 3 / 0.2E1
      cvu(2,4) = dt ** 2 * cc ** 3 * (dx + dy) / dy ** 2 / dx ** 2 / 0.4
     #E1
      cvu(3,1) = dt ** 2 * cc ** 3 / dy ** 3 / 0.4E1
      cvu(3,2) = -cc ** 2 * dt * (0.2E1 * dt * cc * dx ** 2 + dt * cc * 
     #dx * dy + cc * dt * dy ** 2 - 0.2E1 * dx ** 2 * dy) / dy ** 3 / dx
     # ** 2 / 0.2E1
      cvu(3,3) = cc ** 2 * dt * (0.3E1 * cc * dt * dx ** 3 + 0.2E1 * cc 
     #* dt * dx ** 2 * dy + 0.2E1 * cc * dt * dx * dy ** 2 + 0.3E1 * cc 
     #* dt * dy ** 3 - 0.4E1 * dx ** 3 * dy - 0.4E1 * dx * dy ** 3) / dy
     # ** 3 / dx ** 3 / 0.2E1
      cvu(3,4) = -cc ** 2 * dt * (0.2E1 * dt * cc * dx ** 2 + dt * cc * 
     #dx * dy + cc * dt * dy ** 2 - 0.2E1 * dx ** 2 * dy) / dy ** 3 / dx
     # ** 2 / 0.2E1
      cvu(3,5) = dt ** 2 * cc ** 3 / dy ** 3 / 0.4E1
      cvu(4,2) = dt ** 2 * cc ** 3 * (dx + dy) / dy ** 2 / dx ** 2 / 0.4
     #E1
      cvu(4,3) = -cc ** 2 * dt * (dt * cc * dx ** 2 + dt * cc * dx * dy 
     #+ 0.2E1 * cc * dt * dy ** 2 - 0.2E1 * dy ** 2 * dx) / dy ** 2 / dx
     # ** 3 / 0.2E1
      cvu(4,4) = dt ** 2 * cc ** 3 * (dx + dy) / dy ** 2 / dx ** 2 / 0.4
     #E1
      cvu(5,3) = dt ** 2 * cc ** 3 / dx ** 3 / 0.4E1


      return
      end
c
c++++++++++++++++
c
      subroutine getcvv_second(  
     *   cc,dx,dy,dt,cvv )
c
      implicit real (t)
      real cvv(5,5)
      real dx,dy,dt,cc
c
      cvv(1,3) = -dt * cc / dx / 0.6E1
      cvv(2,3) = dt * cc * (0.24E2 * dt * cc + 0.23E2 * dx) / dx ** 2 / 
     #0.48E2
      cvv(3,1) = -dt * cc / dy / 0.6E1
      cvv(3,2) = dt * cc * (0.24E2 * dt * cc + 0.23E2 * dy) / dy ** 2 / 
     #0.48E2
      cvv(3,3) = -(0.3E1 * cc ** 2 * dt ** 2 * dx ** 2 + 0.3E1 * cc ** 2
     # * dt ** 2 * dy ** 2 + 0.2E1 * cc * dt * dx ** 2 * dy + 0.2E1 * cc
     # * dt * dx * dy ** 2 - 0.3E1 * dx ** 2 * dy ** 2) / dy ** 2 / dx *
     #* 2 / 0.3E1
      cvv(3,4) = dt * cc * (0.24E2 * dt * cc + 0.23E2 * dy) / dy ** 2 / 
     #0.48E2
      cvv(3,5) = -dt * cc / dy / 0.6E1
      cvv(4,3) = dt * cc * (0.24E2 * dt * cc + 0.23E2 * dx) / dx ** 2 / 
     #0.48E2
      cvv(5,3) = -dt * cc / dx / 0.6E1


      return
      end
c
c++++++++++++++++
c
