      subroutine duWaveGen2d4rc( 
     *   nd1a,nd1b,nd2a,nd2b,
     *   n1a,n1b,n2a,n2b,
     *   ndf4a,ndf4b,nComp,addForcing,
     *   u,ut,unew,utnew,
     *   src,
     *   dx,dy,dt,cc,beta,
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
      real src (nd1a:nd1b,nd2a:nd2b,ndf4a:ndf4b,0:*)
      real dx,dy,dt,cc,beta
c
c.. declarations of local variables
      integer i,j,ix,jy,n

      integer stencilOpt
c
      real cuu(-3:3,-3:3)
      real cuv(-3:3,-3:3)
      real cvu(-3:3,-3:3)
      real cvv(-3:3,-3:3)
c
      n = 1
      stencilOpt = 1
c
      if( n1a-nd1a .lt. 3 ) then
        write(6,*)'Grid not made with enough ghost cells'
        write(6,*)nd1a,n1a
        stop
      end if

      ! fourth order, cartesian, 2D
      if( addForcing.eq.0 )then

        if( stencilOpt .eq. 0 ) then
          if( useWhereMask.ne.0 ) then
            do j = n2a,n2b
            do i = n1a,n1b
            if( mask(i,j).gt.0 ) then
c
              call duStepWaveGen2d4rc( 
     *           nd1a,nd1b,nd2a,nd2b,
     *           n1a,n1b,n2a,n2b,
     *           u,ut,unew,utnew,
     *           dx,dy,dt,cc,beta,
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
     *           dx,dy,dt,cc,beta,
     *           i,j,n )
c     
            end do
            end do
          end if
        else

          call getcuu_fourth( cc,dx,dy,dt,cuu(-3,-3),beta )
          call getcuv_fourth( cc,dx,dy,dt,cuv(-3,-3),beta )
          call getcvu_fourth( cc,dx,dy,dt,cvu(-3,-3),beta )
          call getcvv_fourth( cc,dx,dy,dt,cvv(-3,-3),beta )

          if( useWhereMask.ne.0 ) then
            do j = n2a,n2b
            do i = n1a,n1b
            if( mask(i,j).gt.0 ) then
c
              unew(i,j) = 0.0
              utnew(i,j) = 0.0
c        
              do jy = -3,3
              do ix = -3+abs(jy),3-abs(jy)
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
              do jy = -3,3
              do ix = -3+abs(jy),3-abs(jy)
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

            call duStepWaveGen2d4rc_tz( 
     *         nd1a,nd1b,nd2a,nd2b,
     *         n1a,n1b,n2a,n2b,
     *         ndf4a,ndf4b,nComp,
     *         u,ut,unew,utnew,
     *         src,
     *         dx,dy,dt,cc,beta,
     *         i,j,n )
c     
          end if
          end do
          end do

        else
          ! no mask
          do j = n2a,n2b
          do i = n1a,n1b
            call duStepWaveGen2d4rc_tz( 
     *         nd1a,nd1b,nd2a,nd2b,
     *         n1a,n1b,n2a,n2b,
     *         ndf4a,ndf4b,nComp,
     *         u,ut,unew,utnew,
     *         src,
     *         dx,dy,dt,cc,beta,
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
      subroutine getcuu_fourth( 
     *   cc,dx,dy,dt,cuu,beta )
c
      implicit real (t)
      real cuu(7,7)
      real dx,dy,dt,cc,beta
c
      cuu(1,4) = beta * dt ** 3 * cc ** 3 * (0.2E1 * beta ** 2 * dt ** 2
     # * cc ** 2 - 0.9E1 * dx ** 2) / dx ** 5 / 0.432E3
      cuu(2,3) = beta * dt ** 3 * cc ** 3 * (0.2E1 * beta ** 2 * dt ** 2
     # * cc ** 2 - 0.3E1 * dx ** 2) * (0.2E1 * dx + dy) / dy ** 2 / dx *
     #* 4 / 0.432E3
      cuu(2,4) = -dt ** 2 * cc ** 2 * (0.4E1 * beta ** 3 * cc ** 3 * dt 
     #** 3 * dx ** 2 + 0.2E1 * beta ** 3 * cc ** 3 * dt ** 3 * dx * dy +
     # 0.6E1 * beta ** 3 * dt ** 3 * cc ** 3 * dy ** 2 - 0.6E1 * beta * 
     #cc * dt * dx ** 4 - 0.3E1 * beta * cc * dt * dx ** 3 * dy - 0.27E2
     # * beta * dt * cc * dy ** 2 * dx ** 2 - 0.9E1 * cc ** 2 * dt ** 2 
     #* dy ** 2 * dx + 0.9E1 * dy ** 2 * dx ** 3) / dy ** 2 / dx ** 5 / 
     #0.216E3
      cuu(2,5) = beta * dt ** 3 * cc ** 3 * (0.2E1 * beta ** 2 * dt ** 2
     # * cc ** 2 - 0.3E1 * dx ** 2) * (0.2E1 * dx + dy) / dy ** 2 / dx *
     #* 4 / 0.432E3
      cuu(3,2) = beta * dt ** 3 * cc ** 3 * (0.2E1 * beta ** 2 * dt ** 2
     # * cc ** 2 - 0.3E1 * dy ** 2) * (dx + 0.2E1 * dy) / dy ** 4 / dx *
     #* 2 / 0.432E3
      cuu(3,3) = -dt ** 3 * cc ** 3 * (0.2E1 * beta ** 3 * cc ** 2 * dt 
     #** 2 * dx ** 3 + 0.4E1 * beta ** 3 * cc ** 2 * dt ** 2 * dx ** 2 *
     # dy + 0.4E1 * beta ** 3 * cc ** 2 * dt ** 2 * dx * dy ** 2 + 0.2E1
     # * beta ** 3 * cc ** 2 * dt ** 2 * dy ** 3 - 0.9E1 * beta * dx ** 
     #3 * dy ** 2 - 0.9E1 * beta * dx ** 2 * dy ** 3 - 0.9E1 * cc * dt *
     # dy ** 2 * dx ** 2) / dy ** 4 / dx ** 4 / 0.108E3
      cuu(3,4) = dt ** 2 * cc ** 2 * (0.12E2 * beta ** 3 * cc ** 3 * dt 
     #** 3 * dx ** 4 + 0.24E2 * beta ** 3 * cc ** 3 * dt ** 3 * dx ** 3 
     #* dy + 0.32E2 * beta ** 3 * cc ** 3 * dt ** 3 * dx ** 2 * dy ** 2 
     #+ 0.16E2 * beta ** 3 * cc ** 3 * dt ** 3 * dx * dy ** 3 + 0.30E2 *
     # beta ** 3 * cc ** 3 * dt ** 3 * dy ** 4 - 0.66E2 * beta * cc * dt
     # * dx ** 4 * dy ** 2 - 0.60E2 * beta * cc * dt * dx ** 3 * dy ** 3
     # - 0.135E3 * beta * dt * cc * dy ** 4 * dx ** 2 - 0.72E2 * cc ** 2
     # * dt ** 2 * dx ** 3 * dy ** 2 - 0.72E2 * cc ** 2 * dt ** 2 * dx *
     # dy ** 4 + 0.288E3 * dy ** 4 * dx ** 3) / dy ** 4 / dx ** 5 / 0.43
     #2E3
      cuu(3,5) = -dt ** 3 * cc ** 3 * (0.2E1 * beta ** 3 * cc ** 2 * dt 
     #** 2 * dx ** 3 + 0.4E1 * beta ** 3 * cc ** 2 * dt ** 2 * dx ** 2 *
     # dy + 0.4E1 * beta ** 3 * cc ** 2 * dt ** 2 * dx * dy ** 2 + 0.2E1
     # * beta ** 3 * cc ** 2 * dt ** 2 * dy ** 3 - 0.9E1 * beta * dx ** 
     #3 * dy ** 2 - 0.9E1 * beta * dx ** 2 * dy ** 3 - 0.9E1 * cc * dt *
     # dy ** 2 * dx ** 2) / dy ** 4 / dx ** 4 / 0.108E3
      cuu(3,6) = beta * dt ** 3 * cc ** 3 * (0.2E1 * beta ** 2 * dt ** 2
     # * cc ** 2 - 0.3E1 * dy ** 2) * (dx + 0.2E1 * dy) / dy ** 4 / dx *
     #* 2 / 0.432E3
      cuu(4,1) = beta * dt ** 3 * cc ** 3 * (0.2E1 * beta ** 2 * dt ** 2
     # * cc ** 2 - 0.9E1 * dy ** 2) / dy ** 5 / 0.432E3
      cuu(4,2) = -dt ** 2 * cc ** 2 * (0.6E1 * beta ** 3 * cc ** 3 * dt 
     #** 3 * dx ** 2 + 0.2E1 * beta ** 3 * cc ** 3 * dt ** 3 * dx * dy +
     # 0.4E1 * beta ** 3 * dt ** 3 * cc ** 3 * dy ** 2 - 0.27E2 * beta *
     # dt * cc * dy ** 2 * dx ** 2 - 0.3E1 * beta * cc * dt * dx * dy **
     # 3 - 0.6E1 * beta * cc * dt * dy ** 4 - 0.9E1 * cc ** 2 * dt ** 2 
     #* dx ** 2 * dy + 0.9E1 * dx ** 2 * dy ** 3) / dx ** 2 / dy ** 5 / 
     #0.216E3
      cuu(4,3) = dt ** 2 * cc ** 2 * (0.30E2 * beta ** 3 * cc ** 3 * dt 
     #** 3 * dx ** 4 + 0.16E2 * beta ** 3 * cc ** 3 * dt ** 3 * dx ** 3 
     #* dy + 0.32E2 * beta ** 3 * cc ** 3 * dt ** 3 * dx ** 2 * dy ** 2 
     #+ 0.24E2 * beta ** 3 * cc ** 3 * dt ** 3 * dx * dy ** 3 + 0.12E2 *
     # beta ** 3 * cc ** 3 * dt ** 3 * dy ** 4 - 0.135E3 * beta * cc * d
     #t * dx ** 4 * dy ** 2 - 0.60E2 * beta * cc * dt * dx ** 3 * dy ** 
     #3 - 0.66E2 * beta * dt * cc * dy ** 4 * dx ** 2 - 0.72E2 * cc ** 2
     # * dt ** 2 * dx ** 4 * dy - 0.72E2 * cc ** 2 * dt ** 2 * dx ** 2 *
     # dy ** 3 + 0.288E3 * dy ** 3 * dx ** 4) / dy ** 5 / dx ** 4 / 0.43
     #2E3
      cuu(4,4) = -(0.10E2 * beta ** 3 * cc ** 5 * dt ** 5 * dx ** 5 + 0.
     #6E1 * beta ** 3 * cc ** 5 * dt ** 5 * dx ** 4 * dy + 0.12E2 * beta
     # ** 3 * cc ** 5 * dt ** 5 * dx ** 3 * dy ** 2 + 0.12E2 * beta ** 3
     # * cc ** 5 * dt ** 5 * dx ** 2 * dy ** 3 + 0.6E1 * beta ** 3 * cc 
     #** 5 * dt ** 5 * dx * dy ** 4 + 0.10E2 * beta ** 3 * cc ** 5 * dt 
     #** 5 * dy ** 5 - 0.45E2 * beta * cc ** 3 * dt ** 3 * dx ** 5 * dy 
     #** 2 - 0.27E2 * beta * cc ** 3 * dt ** 3 * dx ** 4 * dy ** 3 - 0.2
     #7E2 * beta * cc ** 3 * dt ** 3 * dx ** 3 * dy ** 4 - 0.45E2 * beta
     # * cc ** 3 * dt ** 3 * dx ** 2 * dy ** 5 - 0.27E2 * cc ** 4 * dt *
     #* 4 * dx ** 5 * dy - 0.36E2 * cc ** 4 * dt ** 4 * dx ** 3 * dy ** 
     #3 - 0.27E2 * cc ** 4 * dt ** 4 * dx * dy ** 5 + 0.135E3 * cc ** 2 
     #* dt ** 2 * dx ** 5 * dy ** 3 + 0.135E3 * cc ** 2 * dt ** 2 * dx *
     #* 3 * dy ** 5 - 0.108E3 * dy ** 5 * dx ** 5) / dy ** 5 / dx ** 5 /
     # 0.108E3
      cuu(4,5) = dt ** 2 * cc ** 2 * (0.30E2 * beta ** 3 * cc ** 3 * dt 
     #** 3 * dx ** 4 + 0.16E2 * beta ** 3 * cc ** 3 * dt ** 3 * dx ** 3 
     #* dy + 0.32E2 * beta ** 3 * cc ** 3 * dt ** 3 * dx ** 2 * dy ** 2 
     #+ 0.24E2 * beta ** 3 * cc ** 3 * dt ** 3 * dx * dy ** 3 + 0.12E2 *
     # beta ** 3 * cc ** 3 * dt ** 3 * dy ** 4 - 0.135E3 * beta * cc * d
     #t * dx ** 4 * dy ** 2 - 0.60E2 * beta * cc * dt * dx ** 3 * dy ** 
     #3 - 0.66E2 * beta * dt * cc * dy ** 4 * dx ** 2 - 0.72E2 * cc ** 2
     # * dt ** 2 * dx ** 4 * dy - 0.72E2 * cc ** 2 * dt ** 2 * dx ** 2 *
     # dy ** 3 + 0.288E3 * dy ** 3 * dx ** 4) / dy ** 5 / dx ** 4 / 0.43
     #2E3
      cuu(4,6) = -dt ** 2 * cc ** 2 * (0.6E1 * beta ** 3 * cc ** 3 * dt 
     #** 3 * dx ** 2 + 0.2E1 * beta ** 3 * cc ** 3 * dt ** 3 * dx * dy +
     # 0.4E1 * beta ** 3 * dt ** 3 * cc ** 3 * dy ** 2 - 0.27E2 * beta *
     # dt * cc * dy ** 2 * dx ** 2 - 0.3E1 * beta * cc * dt * dx * dy **
     # 3 - 0.6E1 * beta * cc * dt * dy ** 4 - 0.9E1 * cc ** 2 * dt ** 2 
     #* dx ** 2 * dy + 0.9E1 * dx ** 2 * dy ** 3) / dx ** 2 / dy ** 5 / 
     #0.216E3
      cuu(4,7) = beta * dt ** 3 * cc ** 3 * (0.2E1 * beta ** 2 * dt ** 2
     # * cc ** 2 - 0.9E1 * dy ** 2) / dy ** 5 / 0.432E3
      cuu(5,2) = beta * dt ** 3 * cc ** 3 * (0.2E1 * beta ** 2 * dt ** 2
     # * cc ** 2 - 0.3E1 * dy ** 2) * (dx + 0.2E1 * dy) / dy ** 4 / dx *
     #* 2 / 0.432E3
      cuu(5,3) = -dt ** 3 * cc ** 3 * (0.2E1 * beta ** 3 * cc ** 2 * dt 
     #** 2 * dx ** 3 + 0.4E1 * beta ** 3 * cc ** 2 * dt ** 2 * dx ** 2 *
     # dy + 0.4E1 * beta ** 3 * cc ** 2 * dt ** 2 * dx * dy ** 2 + 0.2E1
     # * beta ** 3 * cc ** 2 * dt ** 2 * dy ** 3 - 0.9E1 * beta * dx ** 
     #3 * dy ** 2 - 0.9E1 * beta * dx ** 2 * dy ** 3 - 0.9E1 * cc * dt *
     # dy ** 2 * dx ** 2) / dy ** 4 / dx ** 4 / 0.108E3
      cuu(5,4) = dt ** 2 * cc ** 2 * (0.12E2 * beta ** 3 * cc ** 3 * dt 
     #** 3 * dx ** 4 + 0.24E2 * beta ** 3 * cc ** 3 * dt ** 3 * dx ** 3 
     #* dy + 0.32E2 * beta ** 3 * cc ** 3 * dt ** 3 * dx ** 2 * dy ** 2 
     #+ 0.16E2 * beta ** 3 * cc ** 3 * dt ** 3 * dx * dy ** 3 + 0.30E2 *
     # beta ** 3 * cc ** 3 * dt ** 3 * dy ** 4 - 0.66E2 * beta * cc * dt
     # * dx ** 4 * dy ** 2 - 0.60E2 * beta * cc * dt * dx ** 3 * dy ** 3
     # - 0.135E3 * beta * dt * cc * dy ** 4 * dx ** 2 - 0.72E2 * cc ** 2
     # * dt ** 2 * dx ** 3 * dy ** 2 - 0.72E2 * cc ** 2 * dt ** 2 * dx *
     # dy ** 4 + 0.288E3 * dy ** 4 * dx ** 3) / dy ** 4 / dx ** 5 / 0.43
     #2E3
      cuu(5,5) = -dt ** 3 * cc ** 3 * (0.2E1 * beta ** 3 * cc ** 2 * dt 
     #** 2 * dx ** 3 + 0.4E1 * beta ** 3 * cc ** 2 * dt ** 2 * dx ** 2 *
     # dy + 0.4E1 * beta ** 3 * cc ** 2 * dt ** 2 * dx * dy ** 2 + 0.2E1
     # * beta ** 3 * cc ** 2 * dt ** 2 * dy ** 3 - 0.9E1 * beta * dx ** 
     #3 * dy ** 2 - 0.9E1 * beta * dx ** 2 * dy ** 3 - 0.9E1 * cc * dt *
     # dy ** 2 * dx ** 2) / dy ** 4 / dx ** 4 / 0.108E3
      cuu(5,6) = beta * dt ** 3 * cc ** 3 * (0.2E1 * beta ** 2 * dt ** 2
     # * cc ** 2 - 0.3E1 * dy ** 2) * (dx + 0.2E1 * dy) / dy ** 4 / dx *
     #* 2 / 0.432E3
      cuu(6,3) = beta * dt ** 3 * cc ** 3 * (0.2E1 * beta ** 2 * dt ** 2
     # * cc ** 2 - 0.3E1 * dx ** 2) * (0.2E1 * dx + dy) / dy ** 2 / dx *
     #* 4 / 0.432E3
      cuu(6,4) = -dt ** 2 * cc ** 2 * (0.4E1 * beta ** 3 * cc ** 3 * dt 
     #** 3 * dx ** 2 + 0.2E1 * beta ** 3 * cc ** 3 * dt ** 3 * dx * dy +
     # 0.6E1 * beta ** 3 * dt ** 3 * cc ** 3 * dy ** 2 - 0.6E1 * beta * 
     #cc * dt * dx ** 4 - 0.3E1 * beta * cc * dt * dx ** 3 * dy - 0.27E2
     # * beta * dt * cc * dy ** 2 * dx ** 2 - 0.9E1 * cc ** 2 * dt ** 2 
     #* dy ** 2 * dx + 0.9E1 * dy ** 2 * dx ** 3) / dy ** 2 / dx ** 5 / 
     #0.216E3
      cuu(6,5) = beta * dt ** 3 * cc ** 3 * (0.2E1 * beta ** 2 * dt ** 2
     # * cc ** 2 - 0.3E1 * dx ** 2) * (0.2E1 * dx + dy) / dy ** 2 / dx *
     #* 4 / 0.432E3
      cuu(7,4) = beta * dt ** 3 * cc ** 3 * (0.2E1 * beta ** 2 * dt ** 2
     # * cc ** 2 - 0.9E1 * dx ** 2) / dx ** 5 / 0.432E3


      return
      end
c
c++++++++++++++++
c
      subroutine getcuv_fourth( 
     *   cc,dx,dy,dt,cuv,beta )
c
      implicit real (t)
      real cuv(7,7)
      real dx,dy,dt,cc,beta
c
      cuv(1,4) = -dt ** 2 * cc * (0.4E1 * beta ** 2 * dt ** 2 * cc ** 2 
     #- 0.5E1 * dx ** 2) / dx ** 3 / 0.576E3
      cuv(2,3) = -beta ** 2 * dt ** 4 * cc ** 3 * (0.3E1 * dx + dy) / dy
     # ** 2 / dx ** 2 / 0.576E3
      cuv(2,4) = dt ** 2 * cc * (0.9E1 * beta ** 2 * cc ** 2 * dt ** 2 *
     # dx ** 3 + 0.3E1 * beta ** 2 * dt ** 2 * cc ** 2 * dy * dx ** 2 + 
     #0.36E2 * beta ** 2 * dt ** 2 * cc ** 2 * dy ** 2 * dx + 0.8E1 * cc
     # ** 3 * dt ** 3 * dy ** 2 - 0.12E2 * cc * dt * dy ** 2 * dx ** 2 -
     # 0.45E2 * dy ** 2 * dx ** 3) / dy ** 2 / dx ** 4 / 0.864E3
      cuv(2,5) = -beta ** 2 * dt ** 4 * cc ** 3 * (0.3E1 * dx + dy) / dy
     # ** 2 / dx ** 2 / 0.576E3
      cuv(3,2) = -beta ** 2 * dt ** 4 * cc ** 3 * (dx + 0.3E1 * dy) / dy
     # ** 2 / dx ** 2 / 0.576E3
      cuv(3,3) = dt ** 4 * cc ** 3 * (0.3E1 * beta ** 2 * dx + 0.3E1 * b
     #eta ** 2 * dy + 0.2E1 * dt * cc) / dy ** 2 / dx ** 2 / 0.108E3
      cuv(3,4) = -dt ** 2 * cc * (0.90E2 * beta ** 2 * cc ** 2 * dt ** 2
     # * dx ** 3 + 0.78E2 * beta ** 2 * dt ** 2 * cc ** 2 * dy * dx ** 2
     # + 0.180E3 * beta ** 2 * dt ** 2 * cc ** 2 * dy ** 2 * dx + 0.64E2
     # * cc ** 3 * dt ** 3 * dx ** 2 + 0.64E2 * cc ** 3 * dt ** 3 * dy *
     #* 2 - 0.384E3 * cc * dt * dy ** 2 * dx ** 2 - 0.225E3 * dy ** 2 * 
     #dx ** 3) / dy ** 2 / dx ** 4 / 0.1728E4
      cuv(3,5) = dt ** 4 * cc ** 3 * (0.3E1 * beta ** 2 * dx + 0.3E1 * b
     #eta ** 2 * dy + 0.2E1 * dt * cc) / dy ** 2 / dx ** 2 / 0.108E3
      cuv(3,6) = -beta ** 2 * dt ** 4 * cc ** 3 * (dx + 0.3E1 * dy) / dy
     # ** 2 / dx ** 2 / 0.576E3
      cuv(4,1) = -dt ** 2 * cc * (0.4E1 * beta ** 2 * dt ** 2 * cc ** 2 
     #- 0.5E1 * dy ** 2) / dy ** 3 / 0.576E3
      cuv(4,2) = dt ** 2 * cc * (0.36E2 * beta ** 2 * dt ** 2 * cc ** 2 
     #* dy * dx ** 2 + 0.3E1 * beta ** 2 * dt ** 2 * cc ** 2 * dy ** 2 *
     # dx + 0.9E1 * beta ** 2 * cc ** 2 * dt ** 2 * dy ** 3 + 0.8E1 * cc
     # ** 3 * dt ** 3 * dx ** 2 - 0.12E2 * cc * dt * dy ** 2 * dx ** 2 -
     # 0.45E2 * dx ** 2 * dy ** 3) / dx ** 2 / dy ** 4 / 0.864E3
      cuv(4,3) = -dt ** 2 * cc * (0.180E3 * beta ** 2 * dt ** 2 * cc ** 
     #2 * dy * dx ** 2 + 0.78E2 * beta ** 2 * dt ** 2 * cc ** 2 * dy ** 
     #2 * dx + 0.90E2 * beta ** 2 * cc ** 2 * dt ** 2 * dy ** 3 + 0.64E2
     # * cc ** 3 * dt ** 3 * dx ** 2 + 0.64E2 * cc ** 3 * dt ** 3 * dy *
     #* 2 - 0.384E3 * cc * dt * dy ** 2 * dx ** 2 - 0.225E3 * dx ** 2 * 
     #dy ** 3) / dx ** 2 / dy ** 4 / 0.1728E4
      cuv(4,4) = dt * (0.60E2 * beta ** 2 * cc ** 3 * dt ** 3 * dx ** 4 
     #* dy + 0.36E2 * beta ** 2 * cc ** 3 * dt ** 3 * dx ** 3 * dy ** 2 
     #+ 0.36E2 * beta ** 2 * cc ** 3 * dt ** 3 * dx ** 2 * dy ** 3 + 0.6
     #0E2 * beta ** 2 * cc ** 3 * dt ** 3 * dx * dy ** 4 + 0.24E2 * cc *
     #* 4 * dt ** 4 * dx ** 4 + 0.32E2 * cc ** 4 * dt ** 4 * dx ** 2 * d
     #y ** 2 + 0.24E2 * cc ** 4 * dt ** 4 * dy ** 4 - 0.180E3 * cc ** 2 
     #* dt ** 2 * dx ** 4 * dy ** 2 - 0.180E3 * cc ** 2 * dt ** 2 * dx *
     #* 2 * dy ** 4 - 0.75E2 * cc * dt * dx ** 4 * dy ** 3 - 0.75E2 * cc
     # * dt * dx ** 3 * dy ** 4 + 0.432E3 * dy ** 4 * dx ** 4) / dy ** 4
     # / dx ** 4 / 0.432E3
      cuv(4,5) = -dt ** 2 * cc * (0.180E3 * beta ** 2 * dt ** 2 * cc ** 
     #2 * dy * dx ** 2 + 0.78E2 * beta ** 2 * dt ** 2 * cc ** 2 * dy ** 
     #2 * dx + 0.90E2 * beta ** 2 * cc ** 2 * dt ** 2 * dy ** 3 + 0.64E2
     # * cc ** 3 * dt ** 3 * dx ** 2 + 0.64E2 * cc ** 3 * dt ** 3 * dy *
     #* 2 - 0.384E3 * cc * dt * dy ** 2 * dx ** 2 - 0.225E3 * dx ** 2 * 
     #dy ** 3) / dx ** 2 / dy ** 4 / 0.1728E4
      cuv(4,6) = dt ** 2 * cc * (0.36E2 * beta ** 2 * dt ** 2 * cc ** 2 
     #* dy * dx ** 2 + 0.3E1 * beta ** 2 * dt ** 2 * cc ** 2 * dy ** 2 *
     # dx + 0.9E1 * beta ** 2 * cc ** 2 * dt ** 2 * dy ** 3 + 0.8E1 * cc
     # ** 3 * dt ** 3 * dx ** 2 - 0.12E2 * cc * dt * dy ** 2 * dx ** 2 -
     # 0.45E2 * dx ** 2 * dy ** 3) / dx ** 2 / dy ** 4 / 0.864E3
      cuv(4,7) = -dt ** 2 * cc * (0.4E1 * beta ** 2 * dt ** 2 * cc ** 2 
     #- 0.5E1 * dy ** 2) / dy ** 3 / 0.576E3
      cuv(5,2) = -beta ** 2 * dt ** 4 * cc ** 3 * (dx + 0.3E1 * dy) / dy
     # ** 2 / dx ** 2 / 0.576E3
      cuv(5,3) = dt ** 4 * cc ** 3 * (0.3E1 * beta ** 2 * dx + 0.3E1 * b
     #eta ** 2 * dy + 0.2E1 * dt * cc) / dy ** 2 / dx ** 2 / 0.108E3
      cuv(5,4) = -dt ** 2 * cc * (0.90E2 * beta ** 2 * cc ** 2 * dt ** 2
     # * dx ** 3 + 0.78E2 * beta ** 2 * dt ** 2 * cc ** 2 * dy * dx ** 2
     # + 0.180E3 * beta ** 2 * dt ** 2 * cc ** 2 * dy ** 2 * dx + 0.64E2
     # * cc ** 3 * dt ** 3 * dx ** 2 + 0.64E2 * cc ** 3 * dt ** 3 * dy *
     #* 2 - 0.384E3 * cc * dt * dy ** 2 * dx ** 2 - 0.225E3 * dy ** 2 * 
     #dx ** 3) / dy ** 2 / dx ** 4 / 0.1728E4
      cuv(5,5) = dt ** 4 * cc ** 3 * (0.3E1 * beta ** 2 * dx + 0.3E1 * b
     #eta ** 2 * dy + 0.2E1 * dt * cc) / dy ** 2 / dx ** 2 / 0.108E3
      cuv(5,6) = -beta ** 2 * dt ** 4 * cc ** 3 * (dx + 0.3E1 * dy) / dy
     # ** 2 / dx ** 2 / 0.576E3
      cuv(6,3) = -beta ** 2 * dt ** 4 * cc ** 3 * (0.3E1 * dx + dy) / dy
     # ** 2 / dx ** 2 / 0.576E3
      cuv(6,4) = dt ** 2 * cc * (0.9E1 * beta ** 2 * cc ** 2 * dt ** 2 *
     # dx ** 3 + 0.3E1 * beta ** 2 * dt ** 2 * cc ** 2 * dy * dx ** 2 + 
     #0.36E2 * beta ** 2 * dt ** 2 * cc ** 2 * dy ** 2 * dx + 0.8E1 * cc
     # ** 3 * dt ** 3 * dy ** 2 - 0.12E2 * cc * dt * dy ** 2 * dx ** 2 -
     # 0.45E2 * dy ** 2 * dx ** 3) / dy ** 2 / dx ** 4 / 0.864E3
      cuv(6,5) = -beta ** 2 * dt ** 4 * cc ** 3 * (0.3E1 * dx + dy) / dy
     # ** 2 / dx ** 2 / 0.576E3
      cuv(7,4) = -dt ** 2 * cc * (0.4E1 * beta ** 2 * dt ** 2 * cc ** 2 
     #- 0.5E1 * dx ** 2) / dx ** 3 / 0.576E3

      return
      end
c
c++++++++++++++++
c
      subroutine getcvu_fourth( 
     *   cc,dx,dy,dt,cvu,beta )
c
      implicit real (t)
      real cvu(7,7)
      real dx,dy,dt,cc,beta
c
      cvu(1,4) = beta * dt ** 2 * cc ** 3 * (beta ** 2 * dt ** 2 * cc **
     # 2 - 0.3E1 * dx ** 2) / dx ** 5 / 0.48E2
      cvu(2,3) = beta * dt ** 2 * cc ** 3 * (beta ** 2 * dt ** 2 * cc **
     # 2 - dx ** 2) * (0.2E1 * dx + dy) / dy ** 2 / dx ** 4 / 0.48E2
      cvu(2,4) = -dt * cc ** 2 * (0.2E1 * beta ** 3 * cc ** 3 * dt ** 3 
     #* dx ** 2 + beta ** 3 * cc ** 3 * dt ** 3 * dx * dy + 0.3E1 * beta
     # ** 3 * dt ** 3 * cc ** 3 * dy ** 2 - 0.2E1 * beta * cc * dt * dx 
     #** 4 - beta * cc * dt * dx ** 3 * dy - 0.9E1 * beta * dt * cc * dy
     # ** 2 * dx ** 2 - 0.4E1 * cc ** 2 * dt ** 2 * dy ** 2 * dx + 0.2E1
     # * dy ** 2 * dx ** 3) / dy ** 2 / dx ** 5 / 0.24E2
      cvu(2,5) = beta * dt ** 2 * cc ** 3 * (beta ** 2 * dt ** 2 * cc **
     # 2 - dx ** 2) * (0.2E1 * dx + dy) / dy ** 2 / dx ** 4 / 0.48E2
      cvu(3,2) = beta * dt ** 2 * cc ** 3 * (beta ** 2 * dt ** 2 * cc **
     # 2 - dy ** 2) * (dx + 0.2E1 * dy) / dy ** 4 / dx ** 2 / 0.48E2
      cvu(3,3) = -dt ** 2 * cc ** 3 * (beta ** 3 * cc ** 2 * dt ** 2 * d
     #x ** 3 + 0.2E1 * beta ** 3 * cc ** 2 * dt ** 2 * dx ** 2 * dy + 0.
     #2E1 * beta ** 3 * cc ** 2 * dt ** 2 * dx * dy ** 2 + beta ** 3 * c
     #c ** 2 * dt ** 2 * dy ** 3 - 0.3E1 * beta * dx ** 3 * dy ** 2 - 0.
     #3E1 * beta * dx ** 2 * dy ** 3 - 0.4E1 * cc * dt * dy ** 2 * dx **
     # 2) / dy ** 4 / dx ** 4 / 0.12E2
      cvu(3,4) = dt * cc ** 2 * (0.6E1 * beta ** 3 * cc ** 3 * dt ** 3 *
     # dx ** 4 + 0.12E2 * beta ** 3 * cc ** 3 * dt ** 3 * dx ** 3 * dy +
     # 0.16E2 * beta ** 3 * cc ** 3 * dt ** 3 * dx ** 2 * dy ** 2 + 0.8E
     #1 * beta ** 3 * cc ** 3 * dt ** 3 * dx * dy ** 3 + 0.15E2 * beta *
     #* 3 * cc ** 3 * dt ** 3 * dy ** 4 - 0.22E2 * beta * cc * dt * dx *
     #* 4 * dy ** 2 - 0.20E2 * beta * cc * dt * dx ** 3 * dy ** 3 - 0.45
     #E2 * beta * dt * cc * dy ** 4 * dx ** 2 - 0.32E2 * cc ** 2 * dt **
     # 2 * dx ** 3 * dy ** 2 - 0.32E2 * cc ** 2 * dt ** 2 * dx * dy ** 4
     # + 0.64E2 * dy ** 4 * dx ** 3) / dy ** 4 / dx ** 5 / 0.48E2
      cvu(3,5) = -dt ** 2 * cc ** 3 * (beta ** 3 * cc ** 2 * dt ** 2 * d
     #x ** 3 + 0.2E1 * beta ** 3 * cc ** 2 * dt ** 2 * dx ** 2 * dy + 0.
     #2E1 * beta ** 3 * cc ** 2 * dt ** 2 * dx * dy ** 2 + beta ** 3 * c
     #c ** 2 * dt ** 2 * dy ** 3 - 0.3E1 * beta * dx ** 3 * dy ** 2 - 0.
     #3E1 * beta * dx ** 2 * dy ** 3 - 0.4E1 * cc * dt * dy ** 2 * dx **
     # 2) / dy ** 4 / dx ** 4 / 0.12E2
      cvu(3,6) = beta * dt ** 2 * cc ** 3 * (beta ** 2 * dt ** 2 * cc **
     # 2 - dy ** 2) * (dx + 0.2E1 * dy) / dy ** 4 / dx ** 2 / 0.48E2
      cvu(4,1) = beta * dt ** 2 * cc ** 3 * (beta ** 2 * dt ** 2 * cc **
     # 2 - 0.3E1 * dy ** 2) / dy ** 5 / 0.48E2
      cvu(4,2) = -dt * cc ** 2 * (0.3E1 * beta ** 3 * cc ** 3 * dt ** 3 
     #* dx ** 2 + beta ** 3 * cc ** 3 * dt ** 3 * dx * dy + 0.2E1 * beta
     # ** 3 * dt ** 3 * cc ** 3 * dy ** 2 - 0.9E1 * beta * dt * cc * dy 
     #** 2 * dx ** 2 - beta * cc * dt * dx * dy ** 3 - 0.2E1 * beta * cc
     # * dt * dy ** 4 - 0.4E1 * cc ** 2 * dt ** 2 * dx ** 2 * dy + 0.2E1
     # * dx ** 2 * dy ** 3) / dx ** 2 / dy ** 5 / 0.24E2
      cvu(4,3) = dt * cc ** 2 * (0.15E2 * beta ** 3 * cc ** 3 * dt ** 3 
     #* dx ** 4 + 0.8E1 * beta ** 3 * cc ** 3 * dt ** 3 * dx ** 3 * dy +
     # 0.16E2 * beta ** 3 * cc ** 3 * dt ** 3 * dx ** 2 * dy ** 2 + 0.12
     #E2 * beta ** 3 * cc ** 3 * dt ** 3 * dx * dy ** 3 + 0.6E1 * beta *
     #* 3 * cc ** 3 * dt ** 3 * dy ** 4 - 0.45E2 * beta * cc * dt * dx *
     #* 4 * dy ** 2 - 0.20E2 * beta * cc * dt * dx ** 3 * dy ** 3 - 0.22
     #E2 * beta * dt * cc * dy ** 4 * dx ** 2 - 0.32E2 * cc ** 2 * dt **
     # 2 * dx ** 4 * dy - 0.32E2 * cc ** 2 * dt ** 2 * dx ** 2 * dy ** 3
     # + 0.64E2 * dy ** 3 * dx ** 4) / dy ** 5 / dx ** 4 / 0.48E2
      cvu(4,4) = -dt * cc ** 2 * (0.5E1 * beta ** 3 * cc ** 3 * dt ** 3 
     #* dx ** 5 + 0.3E1 * beta ** 3 * cc ** 3 * dt ** 3 * dx ** 4 * dy +
     # 0.6E1 * beta ** 3 * cc ** 3 * dt ** 3 * dx ** 3 * dy ** 2 + 0.6E1
     # * beta ** 3 * cc ** 3 * dt ** 3 * dx ** 2 * dy ** 3 + 0.3E1 * bet
     #a ** 3 * cc ** 3 * dt ** 3 * dx * dy ** 4 + 0.5E1 * beta ** 3 * cc
     # ** 3 * dt ** 3 * dy ** 5 - 0.15E2 * beta * cc * dt * dx ** 5 * dy
     # ** 2 - 0.9E1 * beta * cc * dt * dx ** 4 * dy ** 3 - 0.9E1 * beta 
     #* cc * dt * dx ** 3 * dy ** 4 - 0.15E2 * beta * cc * dt * dx ** 2 
     #* dy ** 5 - 0.12E2 * cc ** 2 * dt ** 2 * dx ** 5 * dy - 0.16E2 * c
     #c ** 2 * dt ** 2 * dx ** 3 * dy ** 3 - 0.12E2 * cc ** 2 * dt ** 2 
     #* dx * dy ** 5 + 0.30E2 * dx ** 5 * dy ** 3 + 0.30E2 * dx ** 3 * d
     #y ** 5) / dy ** 5 / dx ** 5 / 0.12E2
      cvu(4,5) = dt * cc ** 2 * (0.15E2 * beta ** 3 * cc ** 3 * dt ** 3 
     #* dx ** 4 + 0.8E1 * beta ** 3 * cc ** 3 * dt ** 3 * dx ** 3 * dy +
     # 0.16E2 * beta ** 3 * cc ** 3 * dt ** 3 * dx ** 2 * dy ** 2 + 0.12
     #E2 * beta ** 3 * cc ** 3 * dt ** 3 * dx * dy ** 3 + 0.6E1 * beta *
     #* 3 * cc ** 3 * dt ** 3 * dy ** 4 - 0.45E2 * beta * cc * dt * dx *
     #* 4 * dy ** 2 - 0.20E2 * beta * cc * dt * dx ** 3 * dy ** 3 - 0.22
     #E2 * beta * dt * cc * dy ** 4 * dx ** 2 - 0.32E2 * cc ** 2 * dt **
     # 2 * dx ** 4 * dy - 0.32E2 * cc ** 2 * dt ** 2 * dx ** 2 * dy ** 3
     # + 0.64E2 * dy ** 3 * dx ** 4) / dy ** 5 / dx ** 4 / 0.48E2
      cvu(4,6) = -dt * cc ** 2 * (0.3E1 * beta ** 3 * cc ** 3 * dt ** 3 
     #* dx ** 2 + beta ** 3 * cc ** 3 * dt ** 3 * dx * dy + 0.2E1 * beta
     # ** 3 * dt ** 3 * cc ** 3 * dy ** 2 - 0.9E1 * beta * dt * cc * dy 
     #** 2 * dx ** 2 - beta * cc * dt * dx * dy ** 3 - 0.2E1 * beta * cc
     # * dt * dy ** 4 - 0.4E1 * cc ** 2 * dt ** 2 * dx ** 2 * dy + 0.2E1
     # * dx ** 2 * dy ** 3) / dx ** 2 / dy ** 5 / 0.24E2
      cvu(4,7) = beta * dt ** 2 * cc ** 3 * (beta ** 2 * dt ** 2 * cc **
     # 2 - 0.3E1 * dy ** 2) / dy ** 5 / 0.48E2
      cvu(5,2) = beta * dt ** 2 * cc ** 3 * (beta ** 2 * dt ** 2 * cc **
     # 2 - dy ** 2) * (dx + 0.2E1 * dy) / dy ** 4 / dx ** 2 / 0.48E2
      cvu(5,3) = -dt ** 2 * cc ** 3 * (beta ** 3 * cc ** 2 * dt ** 2 * d
     #x ** 3 + 0.2E1 * beta ** 3 * cc ** 2 * dt ** 2 * dx ** 2 * dy + 0.
     #2E1 * beta ** 3 * cc ** 2 * dt ** 2 * dx * dy ** 2 + beta ** 3 * c
     #c ** 2 * dt ** 2 * dy ** 3 - 0.3E1 * beta * dx ** 3 * dy ** 2 - 0.
     #3E1 * beta * dx ** 2 * dy ** 3 - 0.4E1 * cc * dt * dy ** 2 * dx **
     # 2) / dy ** 4 / dx ** 4 / 0.12E2
      cvu(5,4) = dt * cc ** 2 * (0.6E1 * beta ** 3 * cc ** 3 * dt ** 3 *
     # dx ** 4 + 0.12E2 * beta ** 3 * cc ** 3 * dt ** 3 * dx ** 3 * dy +
     # 0.16E2 * beta ** 3 * cc ** 3 * dt ** 3 * dx ** 2 * dy ** 2 + 0.8E
     #1 * beta ** 3 * cc ** 3 * dt ** 3 * dx * dy ** 3 + 0.15E2 * beta *
     #* 3 * cc ** 3 * dt ** 3 * dy ** 4 - 0.22E2 * beta * cc * dt * dx *
     #* 4 * dy ** 2 - 0.20E2 * beta * cc * dt * dx ** 3 * dy ** 3 - 0.45
     #E2 * beta * dt * cc * dy ** 4 * dx ** 2 - 0.32E2 * cc ** 2 * dt **
     # 2 * dx ** 3 * dy ** 2 - 0.32E2 * cc ** 2 * dt ** 2 * dx * dy ** 4
     # + 0.64E2 * dy ** 4 * dx ** 3) / dy ** 4 / dx ** 5 / 0.48E2
      cvu(5,5) = -dt ** 2 * cc ** 3 * (beta ** 3 * cc ** 2 * dt ** 2 * d
     #x ** 3 + 0.2E1 * beta ** 3 * cc ** 2 * dt ** 2 * dx ** 2 * dy + 0.
     #2E1 * beta ** 3 * cc ** 2 * dt ** 2 * dx * dy ** 2 + beta ** 3 * c
     #c ** 2 * dt ** 2 * dy ** 3 - 0.3E1 * beta * dx ** 3 * dy ** 2 - 0.
     #3E1 * beta * dx ** 2 * dy ** 3 - 0.4E1 * cc * dt * dy ** 2 * dx **
     # 2) / dy ** 4 / dx ** 4 / 0.12E2
      cvu(5,6) = beta * dt ** 2 * cc ** 3 * (beta ** 2 * dt ** 2 * cc **
     # 2 - dy ** 2) * (dx + 0.2E1 * dy) / dy ** 4 / dx ** 2 / 0.48E2
      cvu(6,3) = beta * dt ** 2 * cc ** 3 * (beta ** 2 * dt ** 2 * cc **
     # 2 - dx ** 2) * (0.2E1 * dx + dy) / dy ** 2 / dx ** 4 / 0.48E2
      cvu(6,4) = -dt * cc ** 2 * (0.2E1 * beta ** 3 * cc ** 3 * dt ** 3 
     #* dx ** 2 + beta ** 3 * cc ** 3 * dt ** 3 * dx * dy + 0.3E1 * beta
     # ** 3 * dt ** 3 * cc ** 3 * dy ** 2 - 0.2E1 * beta * cc * dt * dx 
     #** 4 - beta * cc * dt * dx ** 3 * dy - 0.9E1 * beta * dt * cc * dy
     # ** 2 * dx ** 2 - 0.4E1 * cc ** 2 * dt ** 2 * dy ** 2 * dx + 0.2E1
     # * dy ** 2 * dx ** 3) / dy ** 2 / dx ** 5 / 0.24E2
      cvu(6,5) = beta * dt ** 2 * cc ** 3 * (beta ** 2 * dt ** 2 * cc **
     # 2 - dx ** 2) * (0.2E1 * dx + dy) / dy ** 2 / dx ** 4 / 0.48E2
      cvu(7,4) = beta * dt ** 2 * cc ** 3 * (beta ** 2 * dt ** 2 * cc **
     # 2 - 0.3E1 * dx ** 2) / dx ** 5 / 0.48E2

      return
      end
c
c++++++++++++++++
c
      subroutine getcvv_fourth(  
     *   cc,dx,dy,dt,cvv,beta )
c
      implicit real (t)
      real cvv(7,7)
      real dx,dy,dt,cc,beta
c
      cvv(1,4) = -dt * cc * (0.8E1 * beta ** 2 * dt ** 2 * cc ** 2 - 0.5
     #E1 * dx ** 2) / dx ** 3 / 0.288E3
      cvv(2,3) = -beta ** 2 * dt ** 3 * cc ** 3 * (0.3E1 * dx + dy) / dy
     # ** 2 / dx ** 2 / 0.144E3
      cvv(2,4) = dt * cc * (0.6E1 * beta ** 2 * cc ** 2 * dt ** 2 * dx *
     #* 3 + 0.2E1 * beta ** 2 * dt ** 2 * cc ** 2 * dy * dx ** 2 + 0.24E
     #2 * beta ** 2 * dt ** 2 * cc ** 2 * dy ** 2 * dx + 0.6E1 * cc ** 3
     # * dt ** 3 * dy ** 2 - 0.6E1 * cc * dt * dy ** 2 * dx ** 2 - 0.15E
     #2 * dy ** 2 * dx ** 3) / dy ** 2 / dx ** 4 / 0.144E3
      cvv(2,5) = -beta ** 2 * dt ** 3 * cc ** 3 * (0.3E1 * dx + dy) / dy
     # ** 2 / dx ** 2 / 0.144E3
      cvv(3,2) = -beta ** 2 * dt ** 3 * cc ** 3 * (dx + 0.3E1 * dy) / dy
     # ** 2 / dx ** 2 / 0.144E3
      cvv(3,3) = dt ** 3 * cc ** 3 * (0.4E1 * beta ** 2 * dx + 0.4E1 * b
     #eta ** 2 * dy + 0.3E1 * dt * cc) / dy ** 2 / dx ** 2 / 0.36E2
      cvv(3,4) = -dt * cc * (0.60E2 * beta ** 2 * cc ** 2 * dt ** 2 * dx
     # ** 3 + 0.52E2 * beta ** 2 * dt ** 2 * cc ** 2 * dy * dx ** 2 + 0.
     #120E3 * beta ** 2 * dt ** 2 * cc ** 2 * dy ** 2 * dx + 0.48E2 * cc
     # ** 3 * dt ** 3 * dx ** 2 + 0.48E2 * cc ** 3 * dt ** 3 * dy ** 2 -
     # 0.192E3 * cc * dt * dy ** 2 * dx ** 2 - 0.75E2 * dy ** 2 * dx ** 
     #3) / dy ** 2 / dx ** 4 / 0.288E3
      cvv(3,5) = dt ** 3 * cc ** 3 * (0.4E1 * beta ** 2 * dx + 0.4E1 * b
     #eta ** 2 * dy + 0.3E1 * dt * cc) / dy ** 2 / dx ** 2 / 0.36E2
      cvv(3,6) = -beta ** 2 * dt ** 3 * cc ** 3 * (dx + 0.3E1 * dy) / dy
     # ** 2 / dx ** 2 / 0.144E3
      cvv(4,1) = -dt * cc * (0.8E1 * beta ** 2 * dt ** 2 * cc ** 2 - 0.5
     #E1 * dy ** 2) / dy ** 3 / 0.288E3
      cvv(4,2) = dt * cc * (0.24E2 * beta ** 2 * dt ** 2 * cc ** 2 * dy 
     #* dx ** 2 + 0.2E1 * beta ** 2 * dt ** 2 * cc ** 2 * dy ** 2 * dx +
     # 0.6E1 * beta ** 2 * cc ** 2 * dt ** 2 * dy ** 3 + 0.6E1 * cc ** 3
     # * dt ** 3 * dx ** 2 - 0.6E1 * cc * dt * dy ** 2 * dx ** 2 - 0.15E
     #2 * dx ** 2 * dy ** 3) / dx ** 2 / dy ** 4 / 0.144E3
      cvv(4,3) = -dt * cc * (0.120E3 * beta ** 2 * dt ** 2 * cc ** 2 * d
     #y * dx ** 2 + 0.52E2 * beta ** 2 * dt ** 2 * cc ** 2 * dy ** 2 * d
     #x + 0.60E2 * beta ** 2 * cc ** 2 * dt ** 2 * dy ** 3 + 0.48E2 * cc
     # ** 3 * dt ** 3 * dx ** 2 + 0.48E2 * cc ** 3 * dt ** 3 * dy ** 2 -
     # 0.192E3 * cc * dt * dy ** 2 * dx ** 2 - 0.75E2 * dx ** 2 * dy ** 
     #3) / dx ** 2 / dy ** 4 / 0.288E3
      cvv(4,4) = (0.40E2 * beta ** 2 * cc ** 3 * dt ** 3 * dx ** 4 * dy 
     #+ 0.24E2 * beta ** 2 * cc ** 3 * dt ** 3 * dx ** 3 * dy ** 2 + 0.2
     #4E2 * beta ** 2 * cc ** 3 * dt ** 3 * dx ** 2 * dy ** 3 + 0.40E2 *
     # beta ** 2 * cc ** 3 * dt ** 3 * dx * dy ** 4 + 0.18E2 * cc ** 4 *
     # dt ** 4 * dx ** 4 + 0.24E2 * cc ** 4 * dt ** 4 * dx ** 2 * dy ** 
     #2 + 0.18E2 * cc ** 4 * dt ** 4 * dy ** 4 - 0.90E2 * cc ** 2 * dt *
     #* 2 * dx ** 4 * dy ** 2 - 0.90E2 * cc ** 2 * dt ** 2 * dx ** 2 * d
     #y ** 4 - 0.25E2 * cc * dt * dx ** 4 * dy ** 3 - 0.25E2 * cc * dt *
     # dx ** 3 * dy ** 4 + 0.72E2 * dy ** 4 * dx ** 4) / dy ** 4 / dx **
     # 4 / 0.72E2
      cvv(4,5) = -dt * cc * (0.120E3 * beta ** 2 * dt ** 2 * cc ** 2 * d
     #y * dx ** 2 + 0.52E2 * beta ** 2 * dt ** 2 * cc ** 2 * dy ** 2 * d
     #x + 0.60E2 * beta ** 2 * cc ** 2 * dt ** 2 * dy ** 3 + 0.48E2 * cc
     # ** 3 * dt ** 3 * dx ** 2 + 0.48E2 * cc ** 3 * dt ** 3 * dy ** 2 -
     # 0.192E3 * cc * dt * dy ** 2 * dx ** 2 - 0.75E2 * dx ** 2 * dy ** 
     #3) / dx ** 2 / dy ** 4 / 0.288E3
      cvv(4,6) = dt * cc * (0.24E2 * beta ** 2 * dt ** 2 * cc ** 2 * dy 
     #* dx ** 2 + 0.2E1 * beta ** 2 * dt ** 2 * cc ** 2 * dy ** 2 * dx +
     # 0.6E1 * beta ** 2 * cc ** 2 * dt ** 2 * dy ** 3 + 0.6E1 * cc ** 3
     # * dt ** 3 * dx ** 2 - 0.6E1 * cc * dt * dy ** 2 * dx ** 2 - 0.15E
     #2 * dx ** 2 * dy ** 3) / dx ** 2 / dy ** 4 / 0.144E3
      cvv(4,7) = -dt * cc * (0.8E1 * beta ** 2 * dt ** 2 * cc ** 2 - 0.5
     #E1 * dy ** 2) / dy ** 3 / 0.288E3
      cvv(5,2) = -beta ** 2 * dt ** 3 * cc ** 3 * (dx + 0.3E1 * dy) / dy
     # ** 2 / dx ** 2 / 0.144E3
      cvv(5,3) = dt ** 3 * cc ** 3 * (0.4E1 * beta ** 2 * dx + 0.4E1 * b
     #eta ** 2 * dy + 0.3E1 * dt * cc) / dy ** 2 / dx ** 2 / 0.36E2
      cvv(5,4) = -dt * cc * (0.60E2 * beta ** 2 * cc ** 2 * dt ** 2 * dx
     # ** 3 + 0.52E2 * beta ** 2 * dt ** 2 * cc ** 2 * dy * dx ** 2 + 0.
     #120E3 * beta ** 2 * dt ** 2 * cc ** 2 * dy ** 2 * dx + 0.48E2 * cc
     # ** 3 * dt ** 3 * dx ** 2 + 0.48E2 * cc ** 3 * dt ** 3 * dy ** 2 -
     # 0.192E3 * cc * dt * dy ** 2 * dx ** 2 - 0.75E2 * dy ** 2 * dx ** 
     #3) / dy ** 2 / dx ** 4 / 0.288E3
      cvv(5,5) = dt ** 3 * cc ** 3 * (0.4E1 * beta ** 2 * dx + 0.4E1 * b
     #eta ** 2 * dy + 0.3E1 * dt * cc) / dy ** 2 / dx ** 2 / 0.36E2
      cvv(5,6) = -beta ** 2 * dt ** 3 * cc ** 3 * (dx + 0.3E1 * dy) / dy
     # ** 2 / dx ** 2 / 0.144E3
      cvv(6,3) = -beta ** 2 * dt ** 3 * cc ** 3 * (0.3E1 * dx + dy) / dy
     # ** 2 / dx ** 2 / 0.144E3
      cvv(6,4) = dt * cc * (0.6E1 * beta ** 2 * cc ** 2 * dt ** 2 * dx *
     #* 3 + 0.2E1 * beta ** 2 * dt ** 2 * cc ** 2 * dy * dx ** 2 + 0.24E
     #2 * beta ** 2 * dt ** 2 * cc ** 2 * dy ** 2 * dx + 0.6E1 * cc ** 3
     # * dt ** 3 * dy ** 2 - 0.6E1 * cc * dt * dy ** 2 * dx ** 2 - 0.15E
     #2 * dy ** 2 * dx ** 3) / dy ** 2 / dx ** 4 / 0.144E3
      cvv(6,5) = -beta ** 2 * dt ** 3 * cc ** 3 * (0.3E1 * dx + dy) / dy
     # ** 2 / dx ** 2 / 0.144E3
      cvv(7,4) = -dt * cc * (0.8E1 * beta ** 2 * dt ** 2 * cc ** 2 - 0.5
     #E1 * dx ** 2) / dx ** 3 / 0.288E3

      return
      end
c
c++++++++++++++++
c
