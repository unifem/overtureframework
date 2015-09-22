      subroutine duWaveGen3d2rc( 
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
      real src  (nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,ndf4a:ndf4b,0:*)
      real dx,dy,dz,dt,cc
c
c.. declarations of local variables
      integer i,j,k,n
      integer ix,iy,iz
c
      integer stencilOpt

      real cuu(-2:2,-2:2,-2:2)
      real cuv(-2:2,-2:2,-2:2)
      real cvu(-2:2,-2:2,-2:2)
      real cvv(-2:2,-2:2,-2:2)
c
      n = 1
      stencilOpt = 1
c
      if( n1a-nd1a .lt. 2 ) then
        write(6,*)'Grid not made with enough ghost cells'
        write(6,*)nd1a,n1a
        stop
      end if

      ! second order, cartesian, 3D
      if( addForcing.eq.0 )then

        if( stencilOpt .eq. 0 ) then
          if( useWhereMask.ne.0 ) then
            do k = n3a,n3b
            do j = n2a,n2b
            do i = n1a,n1b
            if( mask(i,j,k).gt.0 ) then
c
              call duStepWaveGen3d2rc( 
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
              call duStepWaveGen3d2rc( 
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
          call getcuu_second3D( cc,dx,dy,dz,dt,cuu )
          call getcuv_second3D( cc,dx,dy,dz,dt,cuv )
          call getcvu_second3D( cc,dx,dy,dz,dt,cvu )
          call getcvv_second3D( cc,dx,dy,dz,dt,cvv )
          if( useWhereMask.ne.0 ) then
            do k = n3a,n3b
            do j = n2a,n2b
            do i = n1a,n1b
            if( mask(i,j,k).gt.0 ) then
c
              unew(i,j,k)  = 0.0
              utnew(i,j,k) = 0.0
c
              do iz = -2,2
              do iy = -2+abs(iz),2-abs(iz)
              do ix = -2+abs(iy),2-abs(iy)
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
              do iz = -2,2
              do iy = -2+abs(iz),2-abs(iz)
              do ix = -2+abs(iy),2-abs(iy)
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
            call duStepWaveGen3d2rc_tz( 
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
            call duStepWaveGen3d2rc_tz( 
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
      subroutine getcuu_second3D( 
     *   cc,dx,dy,dz,dt,cuu )
c
      implicit real (t)
      real cuu(0:*)
      real dx,dy,dz,dt,cc
      integer i
c
      do i = 1,5**3
        cuu(i) = 0.0
      end do
c
      t1 = 0.1E1 / dy
      t2 = 0.1E1 / dx
      t3 = t1 + t2
      t4 = dt ** 2
      t5 = cc ** 2
      t6 = cc * t5
      t7 = dt * t4 * t6
      t8 = t7 * t2
      t9 = 0.1E1 / dz
      t10 = t9 + t2
      t11 = t9 ** 2
      t12 = t1 ** 2
      t13 = t2 ** 2
      t14 = 0.1E1 / 0.2E1
      t15 = t14 * dt * (t13 + t11 + t12) * t5
      t5 = dt * t5 / 0.4E1
      t16 = cc * (-cc * t2 + t5 * t13 + t15)
      t6 = dt * t6 / 0.8E1
      t17 = t6 * t13
      t18 = t9 + t1
      t19 = t7 * t13 * t18 / 0.4E1
      t20 = cc * (-cc * t1 + t5 * t12 + t15)
      t21 = t6 * t12
      t22 = t7 * t12 * t10 / 0.4E1
      t5 = cc * (-cc * t9 + t5 * t11 + t15)
      t6 = t6 * t11
      t15 = t7 * t11 * t3 / 0.4E1
      t3 = t8 * t1 * t3 / 0.8E1
      t8 = t8 * t9 * t10 / 0.8E1
      t10 = t7 * t1 * t9 * t18 / 0.8E1
      t7 = t7 / 0.8E1
      t12 = t7 * t1 * t12
      t11 = t7 * t9 * t11
      t7 = t7 * t2 * t13
      cuu(13) = t7
      cuu(33) = t3
      cuu(37) = t8
      cuu(38) = -t2 * t4 * (t14 * t16 + t17) - t19
      cuu(39) = t8
      cuu(43) = t3
      cuu(53) = t12
      cuu(57) = t10
      cuu(58) = -t1 * t4 * (t14 * t20 + t21) - t22
      cuu(59) = t10
      cuu(61) = t11
      cuu(62) = -t9 * t4 * (t14 * t5 + t6) - t15
      cuu(63) = t14 * (0.2E1 * t1 * t4 * t20 + 0.2E1 * t2 * t4 * t16 + 0
     #.2E1 * t9 * t4 * t5) + 0.1E1
      cuu(64) = t9 * t4 * (-t14 * t5 - t6) - t15
      cuu(65) = t11
      cuu(67) = t10
      cuu(68) = t1 * t4 * (-t14 * t20 - t21) - t22
      cuu(69) = t10
      cuu(73) = t12
      cuu(83) = t3
      cuu(87) = t8
      cuu(88) = t2 * t4 * (-t14 * t16 - t17) - t19
      cuu(89) = t8
      cuu(93) = t3
      cuu(113) = t7
c
      return
      end
c
c++++++++++++++++
c
      subroutine getcuv_second3D( 
     *   cc,dx,dy,dz,dt,cuv )
c
      implicit real (t)
      real cuv(0:*)
      real dx,dy,dz,dt,cc
      integer i
c
      do i = 1,5**3
        cuv(i) = 0.0
      end do
c
      t1 = 0.1E1 / dx
      t2 = dt * cc / 0.2E1
      t3 = cc * (-t2 * t1 - 0.3E1 / 0.8E1)
      t4 = dt ** 2
      t5 = cc / 0.16E2
      t6 = 0.1E1 / dy
      t2 = cc * (-t2 * t6 - 0.3E1 / 0.8E1)
      t7 = 0.1E1 / dz
      t8 = -cc * (dt * cc * t7 + 0.1E1) / 0.2E1 + dy * cc * t7 / 0.8E1
      t9 = t5 * dy
      t10 = t9 * t7
      t9 = t9 * t7 ** 2 * t4
      t11 = t5 * t4
      t12 = t11 * t1
      t11 = t11 * t6
      cuv(13) = -t12
      cuv(38) = t4 * (-t3 / 0.2E1 + t5) * t1
      cuv(53) = -t11
      cuv(58) = t4 * (-t2 / 0.2E1 + t5) * t6
      cuv(61) = -t9
      cuv(62) = t4 * (-t8 / 0.2E1 + t10) * t7
      cuv(63) = t1 * t4 * t3 + t6 * t4 * t2 + t7 * t4 * t8 + dt
      cuv(64) = t4 * (-t8 / 0.2E1 + t10) * t7
      cuv(65) = -t9
      cuv(68) = t4 * (-t2 / 0.2E1 + t5) * t6
      cuv(73) = -t11
      cuv(88) = t4 * (-t3 / 0.2E1 + t5) * t1
      cuv(113) = -t12
c
      return
      end
c
c++++++++++++++++
c
      subroutine getcvu_second3D( 
     *   cc,dx,dy,dz,dt,cvu )
c
      implicit real (t)
      real cvu(0:*)
      real dx,dy,dz,dt,cc
      integer i
c
      do i = 1,5**3
        cvu(i) = 0.0
      end do
c
      t1 = 0.1E1 / dy
      t2 = 0.1E1 / dx
      t3 = t1 + t2
      t4 = cc ** 2
      t5 = cc * t4
      t6 = dt ** 2 * t5
      t7 = t6 * t2
      t8 = 0.1E1 / dz
      t9 = t8 + t2
      t10 = t8 ** 2
      t11 = t1 ** 2
      t12 = t2 ** 2
      t13 = dt * (t12 + t10 + t11) * t4 / 0.2E1
      t14 = dt / 0.4E1
      t4 = t14 * t4
      t15 = cc * (-cc * t2 + t4 * t12 + t13)
      t5 = t14 * t5
      t14 = t5 * t12
      t16 = t8 + t1
      t17 = t6 * t12 * t16 / 0.2E1
      t18 = cc * (-cc * t1 + t4 * t11 + t13)
      t19 = t5 * t11
      t20 = t6 * t11 * t9 / 0.2E1
      t4 = cc * (-cc * t8 + t4 * t10 + t13)
      t5 = t5 * t10
      t13 = t6 * t10 * t3 / 0.2E1
      t21 = t6 / 0.4E1
      t12 = t21 * t2 * t12
      t11 = t21 * t1 * t11
      t10 = t21 * t8 * t10
      t3 = t7 * t1 * t3 / 0.4E1
      t7 = t7 * t8 * t9 / 0.4E1
      t6 = t6 * t1 * t8 * t16 / 0.4E1
      cvu(13) = t12
      cvu(33) = t3
      cvu(37) = t7
      cvu(38) = -t2 * dt * (t14 + t15) - t17
      cvu(39) = t7
      cvu(43) = t3
      cvu(53) = t11
      cvu(57) = t6
      cvu(58) = -t1 * dt * (t19 + t18) - t20
      cvu(59) = t6
      cvu(61) = t10
      cvu(62) = -t8 * dt * (t5 + t4) - t13
      cvu(63) = 0.2E1 * t1 * dt * t18 + 0.2E1 * t2 * dt * t15 + 0.2E1 * 
     #t8 * dt * t4
      cvu(64) = t8 * dt * (-t5 - t4) - t13
      cvu(65) = t10
      cvu(67) = t6
      cvu(68) = t1 * dt * (-t19 - t18) - t20
      cvu(69) = t6
      cvu(73) = t11
      cvu(83) = t3
      cvu(87) = t7
      cvu(88) = t2 * dt * (-t14 - t15) - t17
      cvu(89) = t7
      cvu(93) = t3
      cvu(113) = t12
c
      return
      end
c
c++++++++++++++++
c
      subroutine getcvv_second3D( 
     *   cc,dx,dy,dz,dt,cvv )
c
      implicit real (t)
      real cvv(0:*)
      real dx,dy,dz,dt,cc
      integer i
c
      do i = 1,5**3
        cvv(i) = 0.0
      end do
c
      t1 = 0.1E1 / dx
      t2 = -dt * cc / 0.2E1
      t3 = cc * (-0.3E1 / 0.8E1 + t2 * t1)
      t4 = cc / 0.8E1
      t5 = 0.1E1 / dy
      t2 = cc * (-0.3E1 / 0.8E1 + t2 * t5)
      t6 = 0.1E1 / dz
      t7 = dt * cc
      t8 = t4 * dy * t6
      t9 = -cc * (t7 * t6 + 0.1E1) / 0.2E1 + t8
      t7 = t7 / 0.8E1
      t10 = t7 * dy * t6 ** 2
      t11 = t7 * t5
      t7 = t7 * t1
      cvv(13) = -t7
      cvv(38) = dt * (-t3 + t4) * t1
      cvv(53) = -t11
      cvv(58) = dt * (-t2 + t4) * t5
      cvv(61) = -t10
      cvv(62) = dt * (-t9 + t8) * t6
      cvv(63) = 0.2E1 * t1 * dt * t3 + 0.2E1 * t5 * dt * t2 + 0.2E1 * t6
     # * dt * t9 + 0.1E1
      cvv(64) = dt * (-t9 + t8) * t6
      cvv(65) = -t10
      cvv(68) = dt * (-t2 + t4) * t5
      cvv(73) = -t11
      cvv(88) = dt * (-t3 + t4) * t1
      cvv(113) = -t7
c
      return
      end
