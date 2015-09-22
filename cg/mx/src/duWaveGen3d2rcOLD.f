      subroutine duWaveGen3d2rcOLD( 
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
              call duStepWaveGen3d2rcOLD( 
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
              call duStepWaveGen3d2rcOLD( 
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
          call getcuu_second3DOLD( cc,dx,dy,dz,dt,cuu )
          call getcuv_second3DOLD( cc,dx,dy,dz,dt,cuv )
          call getcvu_second3DOLD( cc,dx,dy,dz,dt,cvu )
          call getcvv_second3DOLD( cc,dx,dy,dz,dt,cvv )
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
            call duStepWaveGen3d2rc_tzOLD( 
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
            call duStepWaveGen3d2rc_tzOLD( 
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
      subroutine getcuu_second3DOLD( 
     *   cc,dx,dy,dz,dt,cuu )
c
      implicit real (t)
      real cuu(5,5,5)
      real dx,dy,dz,dt,cc
c
      t1 = cc ** 2
      t3 = dt ** 2
      t5 = t1 * cc * t3 * dt
      t6 = dx ** 2
      t9 = sqrt(0.4E1)
      t12 = t5 / t6 / dx * t9 / 0.16E2
      t13 = dy ** 2
      t14 = 0.1E1 / t13
      t15 = t14 * t9
      t16 = 0.1E1 / dx
      t18 = t5 * t15 * t16
      t19 = 0.1E1 / t6
      t20 = t19 * t9
      t21 = 0.1E1 / dy
      t23 = t5 * t20 * t21
      t24 = t18 + t23
      t25 = dz ** 2
      t26 = 0.1E1 / t25
      t27 = t26 * t9
      t29 = t5 * t27 * t16
      t30 = 0.1E1 / dz
      t32 = t5 * t20 * t30
      t33 = t29 + t32
      t35 = t5 * t20 / 0.16E2
      t36 = t1 * t16
      t37 = dt * t1
      t43 = 0.2E1 * dt * (-t1 * t19 - t1 * t14 - t1 * t26)
      t44 = t37 * t19 - t43
      t49 = (-t36 + cc * t44 * t9 / 0.8E1) * t3
      t53 = t23 / 0.8E1
      t54 = t32 / 0.8E1
      t60 = t5 / t13 / dy * t9 / 0.16E2
      t62 = t5 * t27 * t21
      t64 = t5 * t15 * t30
      t65 = t62 + t64
      t66 = t18 / 0.8E1
      t68 = t5 * t15 / 0.16E2
      t69 = t1 * t21
      t71 = t37 * t14 - t43
      t76 = (-t69 + cc * t71 * t9 / 0.8E1) * t3
      t80 = t64 / 0.8E1
      t86 = t5 / t25 / dz * t9 / 0.16E2
      t87 = t29 / 0.8E1
      t88 = t62 / 0.8E1
      t90 = t5 * t27 / 0.16E2
      t91 = t1 * t30
      t93 = t37 * t26 - t43
      t98 = (-t91 + cc * t93 * t9 / 0.8E1) * t3
      t107 = (t36 - cc * t44 * t9 / 0.8E1) * t3
      t114 = (t69 - cc * t71 * t9 / 0.8E1) * t3
      t121 = (t91 - cc * t93 * t9 / 0.8E1) * t3
      cuu(1,1,1) = 0.0E0
      cuu(1,1,2) = 0.0E0
      cuu(1,1,3) = 0.0E0
      cuu(1,1,4) = 0.0E0
      cuu(1,1,5) = 0.0E0
      cuu(1,2,1) = 0.0E0
      cuu(1,2,2) = 0.0E0
      cuu(1,2,3) = 0.0E0
      cuu(1,2,4) = 0.0E0
      cuu(1,2,5) = 0.0E0
      cuu(1,3,1) = 0.0E0
      cuu(1,3,2) = 0.0E0
      cuu(1,3,3) = t12
      cuu(1,3,4) = 0.0E0
      cuu(1,3,5) = 0.0E0
      cuu(1,4,1) = 0.0E0
      cuu(1,4,2) = 0.0E0
      cuu(1,4,3) = 0.0E0
      cuu(1,4,4) = 0.0E0
      cuu(1,4,5) = 0.0E0
      cuu(1,5,1) = 0.0E0
      cuu(1,5,2) = 0.0E0
      cuu(1,5,3) = 0.0E0
      cuu(1,5,4) = 0.0E0
      cuu(1,5,5) = 0.0E0
      cuu(2,1,1) = 0.0E0
      cuu(2,1,2) = 0.0E0
      cuu(2,1,3) = 0.0E0
      cuu(2,1,4) = 0.0E0
      cuu(2,1,5) = 0.0E0
      cuu(2,2,1) = 0.0E0
      cuu(2,2,2) = 0.0E0
      cuu(2,2,3) = t24 / 0.16E2
      cuu(2,2,4) = 0.0E0
      cuu(2,2,5) = 0.0E0
      cuu(2,3,1) = 0.0E0
      cuu(2,3,2) = t33 / 0.16E2
      cuu(2,3,3) = (-t35 - t49 / 0.2E1) * t16 - t53 - t54
      cuu(2,3,4) = t33 / 0.16E2
      cuu(2,3,5) = 0.0E0
      cuu(2,4,1) = 0.0E0
      cuu(2,4,2) = 0.0E0
      cuu(2,4,3) = t24 / 0.16E2
      cuu(2,4,4) = 0.0E0
      cuu(2,4,5) = 0.0E0
      cuu(2,5,1) = 0.0E0
      cuu(2,5,2) = 0.0E0
      cuu(2,5,3) = 0.0E0
      cuu(2,5,4) = 0.0E0
      cuu(2,5,5) = 0.0E0
      cuu(3,1,1) = 0.0E0
      cuu(3,1,2) = 0.0E0
      cuu(3,1,3) = t60
      cuu(3,1,4) = 0.0E0
      cuu(3,1,5) = 0.0E0
      cuu(3,2,1) = 0.0E0
      cuu(3,2,2) = t65 / 0.16E2
      cuu(3,2,3) = -t66 + (-t68 - t76 / 0.2E1) * t21 - t80
      cuu(3,2,4) = t65 / 0.16E2
      cuu(3,2,5) = 0.0E0
      cuu(3,3,1) = t86
      cuu(3,3,2) = -t87 - t88 + (-t90 - t98 / 0.2E1) * t30
      cuu(3,3,3) = 0.1E1 + (t49 - t107) * t16 / 0.2E1 + (t76 - t114) * t
     #21 / 0.2E1 + (t98 - t121) * t30 / 0.2E1
      cuu(3,3,4) = -t87 - t88 + (t121 / 0.2E1 - t90) * t30
      cuu(3,3,5) = t86
      cuu(3,4,1) = 0.0E0
      cuu(3,4,2) = t65 / 0.16E2
      cuu(3,4,3) = -t66 + (t114 / 0.2E1 - t68) * t21 - t80
      cuu(3,4,4) = t65 / 0.16E2
      cuu(3,4,5) = 0.0E0
      cuu(3,5,1) = 0.0E0
      cuu(3,5,2) = 0.0E0
      cuu(3,5,3) = t60
      cuu(3,5,4) = 0.0E0
      cuu(3,5,5) = 0.0E0
      cuu(4,1,1) = 0.0E0
      cuu(4,1,2) = 0.0E0
      cuu(4,1,3) = 0.0E0
      cuu(4,1,4) = 0.0E0
      cuu(4,1,5) = 0.0E0
      cuu(4,2,1) = 0.0E0
      cuu(4,2,2) = 0.0E0
      cuu(4,2,3) = t24 / 0.16E2
      cuu(4,2,4) = 0.0E0
      cuu(4,2,5) = 0.0E0
      cuu(4,3,1) = 0.0E0
      cuu(4,3,2) = t33 / 0.16E2
      cuu(4,3,3) = (t107 / 0.2E1 - t35) * t16 - t53 - t54
      cuu(4,3,4) = t33 / 0.16E2
      cuu(4,3,5) = 0.0E0
      cuu(4,4,1) = 0.0E0
      cuu(4,4,2) = 0.0E0
      cuu(4,4,3) = t24 / 0.16E2
      cuu(4,4,4) = 0.0E0
      cuu(4,4,5) = 0.0E0
      cuu(4,5,1) = 0.0E0
      cuu(4,5,2) = 0.0E0
      cuu(4,5,3) = 0.0E0
      cuu(4,5,4) = 0.0E0
      cuu(4,5,5) = 0.0E0
      cuu(5,1,1) = 0.0E0
      cuu(5,1,2) = 0.0E0
      cuu(5,1,3) = 0.0E0
      cuu(5,1,4) = 0.0E0
      cuu(5,1,5) = 0.0E0
      cuu(5,2,1) = 0.0E0
      cuu(5,2,2) = 0.0E0
      cuu(5,2,3) = 0.0E0
      cuu(5,2,4) = 0.0E0
      cuu(5,2,5) = 0.0E0
      cuu(5,3,1) = 0.0E0
      cuu(5,3,2) = 0.0E0
      cuu(5,3,3) = t12
      cuu(5,3,4) = 0.0E0
      cuu(5,3,5) = 0.0E0
      cuu(5,4,1) = 0.0E0
      cuu(5,4,2) = 0.0E0
      cuu(5,4,3) = 0.0E0
      cuu(5,4,4) = 0.0E0
      cuu(5,4,5) = 0.0E0
      cuu(5,5,1) = 0.0E0
      cuu(5,5,2) = 0.0E0
      cuu(5,5,3) = 0.0E0
      cuu(5,5,4) = 0.0E0
      cuu(5,5,5) = 0.0E0
c
      return
      end
c
c++++++++++++++++
c
      subroutine getcuv_second3DOLD( 
     *   cc,dx,dy,dz,dt,cuv )
c
      implicit real (t)
      real cuv(5,5,5)
      real dx,dy,dz,dt,cc
c
      t1 = sqrt(0.4E1)
      t2 = cc * t1
      t3 = dt ** 2
      t4 = 0.1E1 / dx
      t7 = t2 * t3 * t4 / 0.32E2
      t9 = t2 * t3 / 0.32E2
      t10 = cc ** 2
      t11 = t10 * dt
      t14 = 0.3E1 / 0.16E2 * t2
      t15 = -t11 * t4 / 0.2E1 - t14
      t16 = t15 * t3
      t20 = 0.1E1 / dy
      t23 = t2 * t3 * t20 / 0.32E2
      t26 = -t11 * t20 / 0.2E1 - t14
      t27 = t26 * t3
      t31 = 0.1E1 / dz
      t34 = t2 * t3 * t31 / 0.32E2
      t37 = -t11 * t31 / 0.2E1 - t14
      t38 = t37 * t3
      t42 = -t15 * t3
      t45 = -t26 * t3
      t48 = -t37 * t3
      cuv(1,1,1) = 0.0E0
      cuv(1,1,2) = 0.0E0
      cuv(1,1,3) = 0.0E0
      cuv(1,1,4) = 0.0E0
      cuv(1,1,5) = 0.0E0
      cuv(1,2,1) = 0.0E0
      cuv(1,2,2) = 0.0E0
      cuv(1,2,3) = 0.0E0
      cuv(1,2,4) = 0.0E0
      cuv(1,2,5) = 0.0E0
      cuv(1,3,1) = 0.0E0
      cuv(1,3,2) = 0.0E0
      cuv(1,3,3) = -t7
      cuv(1,3,4) = 0.0E0
      cuv(1,3,5) = 0.0E0
      cuv(1,4,1) = 0.0E0
      cuv(1,4,2) = 0.0E0
      cuv(1,4,3) = 0.0E0
      cuv(1,4,4) = 0.0E0
      cuv(1,4,5) = 0.0E0
      cuv(1,5,1) = 0.0E0
      cuv(1,5,2) = 0.0E0
      cuv(1,5,3) = 0.0E0
      cuv(1,5,4) = 0.0E0
      cuv(1,5,5) = 0.0E0
      cuv(2,1,1) = 0.0E0
      cuv(2,1,2) = 0.0E0
      cuv(2,1,3) = 0.0E0
      cuv(2,1,4) = 0.0E0
      cuv(2,1,5) = 0.0E0
      cuv(2,2,1) = 0.0E0
      cuv(2,2,2) = 0.0E0
      cuv(2,2,3) = 0.0E0
      cuv(2,2,4) = 0.0E0
      cuv(2,2,5) = 0.0E0
      cuv(2,3,1) = 0.0E0
      cuv(2,3,2) = 0.0E0
      cuv(2,3,3) = (t9 - t16 / 0.2E1) * t4
      cuv(2,3,4) = 0.0E0
      cuv(2,3,5) = 0.0E0
      cuv(2,4,1) = 0.0E0
      cuv(2,4,2) = 0.0E0
      cuv(2,4,3) = 0.0E0
      cuv(2,4,4) = 0.0E0
      cuv(2,4,5) = 0.0E0
      cuv(2,5,1) = 0.0E0
      cuv(2,5,2) = 0.0E0
      cuv(2,5,3) = 0.0E0
      cuv(2,5,4) = 0.0E0
      cuv(2,5,5) = 0.0E0
      cuv(3,1,1) = 0.0E0
      cuv(3,1,2) = 0.0E0
      cuv(3,1,3) = -t23
      cuv(3,1,4) = 0.0E0
      cuv(3,1,5) = 0.0E0
      cuv(3,2,1) = 0.0E0
      cuv(3,2,2) = 0.0E0
      cuv(3,2,3) = (t9 - t27 / 0.2E1) * t20
      cuv(3,2,4) = 0.0E0
      cuv(3,2,5) = 0.0E0
      cuv(3,3,1) = -t34
      cuv(3,3,2) = (t9 - t38 / 0.2E1) * t31
      cuv(3,3,3) = dt + (t16 - t42) * t4 / 0.2E1 + (t27 - t45) * t20 / 0
     #.2E1 + (t38 - t48) * t31 / 0.2E1
      cuv(3,3,4) = (t48 / 0.2E1 + t9) * t31
      cuv(3,3,5) = -t34
      cuv(3,4,1) = 0.0E0
      cuv(3,4,2) = 0.0E0
      cuv(3,4,3) = (t45 / 0.2E1 + t9) * t20
      cuv(3,4,4) = 0.0E0
      cuv(3,4,5) = 0.0E0
      cuv(3,5,1) = 0.0E0
      cuv(3,5,2) = 0.0E0
      cuv(3,5,3) = -t23
      cuv(3,5,4) = 0.0E0
      cuv(3,5,5) = 0.0E0
      cuv(4,1,1) = 0.0E0
      cuv(4,1,2) = 0.0E0
      cuv(4,1,3) = 0.0E0
      cuv(4,1,4) = 0.0E0
      cuv(4,1,5) = 0.0E0
      cuv(4,2,1) = 0.0E0
      cuv(4,2,2) = 0.0E0
      cuv(4,2,3) = 0.0E0
      cuv(4,2,4) = 0.0E0
      cuv(4,2,5) = 0.0E0
      cuv(4,3,1) = 0.0E0
      cuv(4,3,2) = 0.0E0
      cuv(4,3,3) = (t42 / 0.2E1 + t9) * t4
      cuv(4,3,4) = 0.0E0
      cuv(4,3,5) = 0.0E0
      cuv(4,4,1) = 0.0E0
      cuv(4,4,2) = 0.0E0
      cuv(4,4,3) = 0.0E0
      cuv(4,4,4) = 0.0E0
      cuv(4,4,5) = 0.0E0
      cuv(4,5,1) = 0.0E0
      cuv(4,5,2) = 0.0E0
      cuv(4,5,3) = 0.0E0
      cuv(4,5,4) = 0.0E0
      cuv(4,5,5) = 0.0E0
      cuv(5,1,1) = 0.0E0
      cuv(5,1,2) = 0.0E0
      cuv(5,1,3) = 0.0E0
      cuv(5,1,4) = 0.0E0
      cuv(5,1,5) = 0.0E0
      cuv(5,2,1) = 0.0E0
      cuv(5,2,2) = 0.0E0
      cuv(5,2,3) = 0.0E0
      cuv(5,2,4) = 0.0E0
      cuv(5,2,5) = 0.0E0
      cuv(5,3,1) = 0.0E0
      cuv(5,3,2) = 0.0E0
      cuv(5,3,3) = -t7
      cuv(5,3,4) = 0.0E0
      cuv(5,3,5) = 0.0E0
      cuv(5,4,1) = 0.0E0
      cuv(5,4,2) = 0.0E0
      cuv(5,4,3) = 0.0E0
      cuv(5,4,4) = 0.0E0
      cuv(5,4,5) = 0.0E0
      cuv(5,5,1) = 0.0E0
      cuv(5,5,2) = 0.0E0
      cuv(5,5,3) = 0.0E0
      cuv(5,5,4) = 0.0E0
      cuv(5,5,5) = 0.0E0
c
      return
      end
c
c++++++++++++++++
c
      subroutine getcvu_second3DOLD( 
     *   cc,dx,dy,dz,dt,cvu )
c
      implicit real (t)
      real cvu(5,5,5)
      real dx,dy,dz,dt,cc
c
      t1 = cc ** 2
      t3 = dt ** 2
      t4 = t1 * cc * t3
      t5 = dx ** 2
      t8 = sqrt(0.4E1)
      t11 = t4 / t5 / dx * t8 / 0.8E1
      t12 = dy ** 2
      t13 = 0.1E1 / t12
      t14 = t13 * t8
      t15 = 0.1E1 / dx
      t17 = t4 * t14 * t15
      t18 = 0.1E1 / t5
      t19 = t18 * t8
      t20 = 0.1E1 / dy
      t22 = t4 * t19 * t20
      t23 = t17 + t22
      t24 = dz ** 2
      t25 = 0.1E1 / t24
      t26 = t25 * t8
      t28 = t4 * t26 * t15
      t29 = 0.1E1 / dz
      t31 = t4 * t19 * t29
      t32 = t28 + t31
      t34 = t4 * t19 / 0.8E1
      t35 = t1 * t15
      t36 = dt * t1
      t42 = 0.2E1 * dt * (-t1 * t18 - t1 * t13 - t1 * t25)
      t43 = t36 * t18 - t42
      t48 = (-t35 + cc * t43 * t8 / 0.8E1) * dt
      t51 = t22 / 0.4E1
      t52 = t31 / 0.4E1
      t58 = t4 / t12 / dy * t8 / 0.8E1
      t60 = t26 * t20 * t4
      t62 = t4 * t14 * t29
      t63 = t60 + t62
      t64 = t17 / 0.4E1
      t66 = t4 * t14 / 0.8E1
      t67 = t1 * t20
      t69 = t36 * t13 - t42
      t74 = (-t67 + cc * t69 * t8 / 0.8E1) * dt
      t77 = t62 / 0.4E1
      t83 = t4 / t24 / dz * t8 / 0.8E1
      t84 = t28 / 0.4E1
      t85 = t60 / 0.4E1
      t87 = t4 * t26 / 0.8E1
      t88 = t1 * t29
      t90 = t36 * t25 - t42
      t95 = (-t88 + cc * t90 * t8 / 0.8E1) * dt
      t103 = (t35 - cc * t43 * t8 / 0.8E1) * dt
      t110 = (t67 - cc * t69 * t8 / 0.8E1) * dt
      t117 = (t88 - cc * t90 * t8 / 0.8E1) * dt
      cvu(1,1,1) = 0.0E0
      cvu(1,1,2) = 0.0E0
      cvu(1,1,3) = 0.0E0
      cvu(1,1,4) = 0.0E0
      cvu(1,1,5) = 0.0E0
      cvu(1,2,1) = 0.0E0
      cvu(1,2,2) = 0.0E0
      cvu(1,2,3) = 0.0E0
      cvu(1,2,4) = 0.0E0
      cvu(1,2,5) = 0.0E0
      cvu(1,3,1) = 0.0E0
      cvu(1,3,2) = 0.0E0
      cvu(1,3,3) = t11
      cvu(1,3,4) = 0.0E0
      cvu(1,3,5) = 0.0E0
      cvu(1,4,1) = 0.0E0
      cvu(1,4,2) = 0.0E0
      cvu(1,4,3) = 0.0E0
      cvu(1,4,4) = 0.0E0
      cvu(1,4,5) = 0.0E0
      cvu(1,5,1) = 0.0E0
      cvu(1,5,2) = 0.0E0
      cvu(1,5,3) = 0.0E0
      cvu(1,5,4) = 0.0E0
      cvu(1,5,5) = 0.0E0
      cvu(2,1,1) = 0.0E0
      cvu(2,1,2) = 0.0E0
      cvu(2,1,3) = 0.0E0
      cvu(2,1,4) = 0.0E0
      cvu(2,1,5) = 0.0E0
      cvu(2,2,1) = 0.0E0
      cvu(2,2,2) = 0.0E0
      cvu(2,2,3) = t23 / 0.8E1
      cvu(2,2,4) = 0.0E0
      cvu(2,2,5) = 0.0E0
      cvu(2,3,1) = 0.0E0
      cvu(2,3,2) = t32 / 0.8E1
      cvu(2,3,3) = (-t34 - t48) * t15 - t51 - t52
      cvu(2,3,4) = t32 / 0.8E1
      cvu(2,3,5) = 0.0E0
      cvu(2,4,1) = 0.0E0
      cvu(2,4,2) = 0.0E0
      cvu(2,4,3) = t23 / 0.8E1
      cvu(2,4,4) = 0.0E0
      cvu(2,4,5) = 0.0E0
      cvu(2,5,1) = 0.0E0
      cvu(2,5,2) = 0.0E0
      cvu(2,5,3) = 0.0E0
      cvu(2,5,4) = 0.0E0
      cvu(2,5,5) = 0.0E0
      cvu(3,1,1) = 0.0E0
      cvu(3,1,2) = 0.0E0
      cvu(3,1,3) = t58
      cvu(3,1,4) = 0.0E0
      cvu(3,1,5) = 0.0E0
      cvu(3,2,1) = 0.0E0
      cvu(3,2,2) = t63 / 0.8E1
      cvu(3,2,3) = -t64 + (-t66 - t74) * t20 - t77
      cvu(3,2,4) = t63 / 0.8E1
      cvu(3,2,5) = 0.0E0
      cvu(3,3,1) = t83
      cvu(3,3,2) = -t84 - t85 + (-t87 - t95) * t29
      cvu(3,3,3) = (t48 - t103) * t15 + (t74 - t110) * t20 + (t95 - t117
     #) * t29
      cvu(3,3,4) = -t84 - t85 + (t117 - t87) * t29
      cvu(3,3,5) = t83
      cvu(3,4,1) = 0.0E0
      cvu(3,4,2) = t63 / 0.8E1
      cvu(3,4,3) = -t64 + (t110 - t66) * t20 - t77
      cvu(3,4,4) = t63 / 0.8E1
      cvu(3,4,5) = 0.0E0
      cvu(3,5,1) = 0.0E0
      cvu(3,5,2) = 0.0E0
      cvu(3,5,3) = t58
      cvu(3,5,4) = 0.0E0
      cvu(3,5,5) = 0.0E0
      cvu(4,1,1) = 0.0E0
      cvu(4,1,2) = 0.0E0
      cvu(4,1,3) = 0.0E0
      cvu(4,1,4) = 0.0E0
      cvu(4,1,5) = 0.0E0
      cvu(4,2,1) = 0.0E0
      cvu(4,2,2) = 0.0E0
      cvu(4,2,3) = t23 / 0.8E1
      cvu(4,2,4) = 0.0E0
      cvu(4,2,5) = 0.0E0
      cvu(4,3,1) = 0.0E0
      cvu(4,3,2) = t32 / 0.8E1
      cvu(4,3,3) = (t103 - t34) * t15 - t51 - t52
      cvu(4,3,4) = t32 / 0.8E1
      cvu(4,3,5) = 0.0E0
      cvu(4,4,1) = 0.0E0
      cvu(4,4,2) = 0.0E0
      cvu(4,4,3) = t23 / 0.8E1
      cvu(4,4,4) = 0.0E0
      cvu(4,4,5) = 0.0E0
      cvu(4,5,1) = 0.0E0
      cvu(4,5,2) = 0.0E0
      cvu(4,5,3) = 0.0E0
      cvu(4,5,4) = 0.0E0
      cvu(4,5,5) = 0.0E0
      cvu(5,1,1) = 0.0E0
      cvu(5,1,2) = 0.0E0
      cvu(5,1,3) = 0.0E0
      cvu(5,1,4) = 0.0E0
      cvu(5,1,5) = 0.0E0
      cvu(5,2,1) = 0.0E0
      cvu(5,2,2) = 0.0E0
      cvu(5,2,3) = 0.0E0
      cvu(5,2,4) = 0.0E0
      cvu(5,2,5) = 0.0E0
      cvu(5,3,1) = 0.0E0
      cvu(5,3,2) = 0.0E0
      cvu(5,3,3) = t11
      cvu(5,3,4) = 0.0E0
      cvu(5,3,5) = 0.0E0
      cvu(5,4,1) = 0.0E0
      cvu(5,4,2) = 0.0E0
      cvu(5,4,3) = 0.0E0
      cvu(5,4,4) = 0.0E0
      cvu(5,4,5) = 0.0E0
      cvu(5,5,1) = 0.0E0
      cvu(5,5,2) = 0.0E0
      cvu(5,5,3) = 0.0E0
      cvu(5,5,4) = 0.0E0
      cvu(5,5,5) = 0.0E0
c
      return
      end
c
c++++++++++++++++
c
      subroutine getcvv_second3DOLD( 
     *   cc,dx,dy,dz,dt,cvv )
c
      implicit real (t)
      real cvv(5,5,5)
      real dx,dy,dz,dt,cc
c
      t1 = sqrt(0.4E1)
      t2 = cc * t1
      t3 = 0.1E1 / dx
      t6 = t2 * dt * t3 / 0.16E2
      t8 = t2 * dt / 0.16E2
      t9 = cc ** 2
      t10 = t9 * dt
      t13 = 0.3E1 / 0.16E2 * t2
      t14 = -t10 * t3 / 0.2E1 - t13
      t15 = t14 * dt
      t18 = 0.1E1 / dy
      t21 = t2 * dt * t18 / 0.16E2
      t24 = -t10 * t18 / 0.2E1 - t13
      t25 = t24 * dt
      t28 = 0.1E1 / dz
      t31 = t2 * dt * t28 / 0.16E2
      t34 = -t10 * t28 / 0.2E1 - t13
      t35 = t34 * dt
      t38 = -t14 * dt
      t41 = -t24 * dt
      t44 = -t34 * dt
      cvv(1,1,1) = 0.0E0
      cvv(1,1,2) = 0.0E0
      cvv(1,1,3) = 0.0E0
      cvv(1,1,4) = 0.0E0
      cvv(1,1,5) = 0.0E0
      cvv(1,2,1) = 0.0E0
      cvv(1,2,2) = 0.0E0
      cvv(1,2,3) = 0.0E0
      cvv(1,2,4) = 0.0E0
      cvv(1,2,5) = 0.0E0
      cvv(1,3,1) = 0.0E0
      cvv(1,3,2) = 0.0E0
      cvv(1,3,3) = -t6
      cvv(1,3,4) = 0.0E0
      cvv(1,3,5) = 0.0E0
      cvv(1,4,1) = 0.0E0
      cvv(1,4,2) = 0.0E0
      cvv(1,4,3) = 0.0E0
      cvv(1,4,4) = 0.0E0
      cvv(1,4,5) = 0.0E0
      cvv(1,5,1) = 0.0E0
      cvv(1,5,2) = 0.0E0
      cvv(1,5,3) = 0.0E0
      cvv(1,5,4) = 0.0E0
      cvv(1,5,5) = 0.0E0
      cvv(2,1,1) = 0.0E0
      cvv(2,1,2) = 0.0E0
      cvv(2,1,3) = 0.0E0
      cvv(2,1,4) = 0.0E0
      cvv(2,1,5) = 0.0E0
      cvv(2,2,1) = 0.0E0
      cvv(2,2,2) = 0.0E0
      cvv(2,2,3) = 0.0E0
      cvv(2,2,4) = 0.0E0
      cvv(2,2,5) = 0.0E0
      cvv(2,3,1) = 0.0E0
      cvv(2,3,2) = 0.0E0
      cvv(2,3,3) = (t8 - t15) * t3
      cvv(2,3,4) = 0.0E0
      cvv(2,3,5) = 0.0E0
      cvv(2,4,1) = 0.0E0
      cvv(2,4,2) = 0.0E0
      cvv(2,4,3) = 0.0E0
      cvv(2,4,4) = 0.0E0
      cvv(2,4,5) = 0.0E0
      cvv(2,5,1) = 0.0E0
      cvv(2,5,2) = 0.0E0
      cvv(2,5,3) = 0.0E0
      cvv(2,5,4) = 0.0E0
      cvv(2,5,5) = 0.0E0
      cvv(3,1,1) = 0.0E0
      cvv(3,1,2) = 0.0E0
      cvv(3,1,3) = -t21
      cvv(3,1,4) = 0.0E0
      cvv(3,1,5) = 0.0E0
      cvv(3,2,1) = 0.0E0
      cvv(3,2,2) = 0.0E0
      cvv(3,2,3) = (t8 - t25) * t18
      cvv(3,2,4) = 0.0E0
      cvv(3,2,5) = 0.0E0
      cvv(3,3,1) = -t31
      cvv(3,3,2) = (t8 - t35) * t28
      cvv(3,3,3) = 0.1E1 + (t15 - t38) * t3 + (t25 - t41) * t18 + (t35 -
     # t44) * t28
      cvv(3,3,4) = (t44 + t8) * t28
      cvv(3,3,5) = -t31
      cvv(3,4,1) = 0.0E0
      cvv(3,4,2) = 0.0E0
      cvv(3,4,3) = (t41 + t8) * t18
      cvv(3,4,4) = 0.0E0
      cvv(3,4,5) = 0.0E0
      cvv(3,5,1) = 0.0E0
      cvv(3,5,2) = 0.0E0
      cvv(3,5,3) = -t21
      cvv(3,5,4) = 0.0E0
      cvv(3,5,5) = 0.0E0
      cvv(4,1,1) = 0.0E0
      cvv(4,1,2) = 0.0E0
      cvv(4,1,3) = 0.0E0
      cvv(4,1,4) = 0.0E0
      cvv(4,1,5) = 0.0E0
      cvv(4,2,1) = 0.0E0
      cvv(4,2,2) = 0.0E0
      cvv(4,2,3) = 0.0E0
      cvv(4,2,4) = 0.0E0
      cvv(4,2,5) = 0.0E0
      cvv(4,3,1) = 0.0E0
      cvv(4,3,2) = 0.0E0
      cvv(4,3,3) = (t38 + t8) * t3
      cvv(4,3,4) = 0.0E0
      cvv(4,3,5) = 0.0E0
      cvv(4,4,1) = 0.0E0
      cvv(4,4,2) = 0.0E0
      cvv(4,4,3) = 0.0E0
      cvv(4,4,4) = 0.0E0
      cvv(4,4,5) = 0.0E0
      cvv(4,5,1) = 0.0E0
      cvv(4,5,2) = 0.0E0
      cvv(4,5,3) = 0.0E0
      cvv(4,5,4) = 0.0E0
      cvv(4,5,5) = 0.0E0
      cvv(5,1,1) = 0.0E0
      cvv(5,1,2) = 0.0E0
      cvv(5,1,3) = 0.0E0
      cvv(5,1,4) = 0.0E0
      cvv(5,1,5) = 0.0E0
      cvv(5,2,1) = 0.0E0
      cvv(5,2,2) = 0.0E0
      cvv(5,2,3) = 0.0E0
      cvv(5,2,4) = 0.0E0
      cvv(5,2,5) = 0.0E0
      cvv(5,3,1) = 0.0E0
      cvv(5,3,2) = 0.0E0
      cvv(5,3,3) = -t6
      cvv(5,3,4) = 0.0E0
      cvv(5,3,5) = 0.0E0
      cvv(5,4,1) = 0.0E0
      cvv(5,4,2) = 0.0E0
      cvv(5,4,3) = 0.0E0
      cvv(5,4,4) = 0.0E0
      cvv(5,4,5) = 0.0E0
      cvv(5,5,1) = 0.0E0
      cvv(5,5,2) = 0.0E0
      cvv(5,5,3) = 0.0E0
      cvv(5,5,4) = 0.0E0
      cvv(5,5,5) = 0.0E0
c
      return
      end
