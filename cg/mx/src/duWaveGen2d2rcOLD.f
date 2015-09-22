      subroutine duWaveGen2d2rcOLD( 
     *   nd1a,nd1b,nd2a,nd2b,
     *   n1a,n1b,n2a,n2b,
     *   ndf4a,ndf4b,nComp,addForcing,
     *   u,v,unew,vnew,
     *   src,
     *   dx,dy,dt,c,
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

      real u   (nd1a:nd1b,nd2a:nd2b)
      real v   (nd1a:nd1b,nd2a:nd2b)
      real unew(nd1a:nd1b,nd2a:nd2b)
      real vnew(nd1a:nd1b,nd2a:nd2b)
      real src (nd1a:nd1b,nd2a:nd2b,ndf4a:ndf4b,1:*)
      real dx,dy,dt,c
c
c.. declarations of local variables
      integer i,j,n
      real cx,cy
c
      real t2,t3,t4,t6,t7,t8,t10,t11,t12,t14,t15,t16,t18,t19,t21
      real t22,t23,t24,t25,t28,t29,t32,t38,t39,t41,t44,t45,t55,t69
      real t73,t74,t82,t95,t98,t108,t122
c
      n = 1
c
      cx = c
      cy = c
c
        ! second order, cartesian, 2D
      if( addForcing.eq.0 )then

        if( useWhereMask.ne.0 ) then
          do j = n2a,n2b
          do i = n1a,n1b
          if( mask(i,j).gt.0 ) then

          t2 = (cx ** 2)
          t3 = 1.0 / (dx)
          t4 = (t2) * (t3)
          t12 = 2. * u(i + 1,j)
          t15 = dx ** 2
          t16 = 1.0 / (t15)
          t18 = (cy ** 2)
          t21 = dy ** 2
          t22 = 1. / (t21)
          t28 = 0.3D1 / 0.4D1 * v(i,j)
          t29 = 2. * u(i,j)
          t38 = (dt * (t2 * (u(i + 1,j) - t29 + u(i - 1,j)
     #       ) * t16 + t18 * (u(i,j + 1) - t29 + u(i,j - 1)) * t2
     #       2)) / 0.2D1
          t41 = 0.1D1 / cx
          t44 = ((u(i + 1,j) - u(i,j)) * t3) + (dt) * 
     #       (v(i + 1,j) - v(i,j)) * (t3) / 0.2D1 + (0.3D1 / 
     #       0.4D1 * v(i + 1,j) + (dt * (t2 * (u(i + 2,j) - t
     #       12 + u(i,j)) * t16 + t18 * (u(i + 1,j + 1) - t12 + u
     #       (i + 1,j - 1)) * t22)) / 0.2D1 - v(i + 2,j) / 0.4D1 
     #       - t28 - t38 + v(i - 1,j) / 0.4D1) * t41 / 0.2D1
          t45 = dt ** 2
          t55 = 2 * u(i - 1,j)
          t69 = ((u(i,j) - u(i - 1,j)) * t3) + (dt) * 
     #       (v(i,j) - v(i - 1,j)) * (t3) / 0.2D1 + (t28 + t3
     #       8 - v(i + 1,j) / 0.4D1 - 0.3D1 / 0.4D1 * v(i - 1,j) 
     #       - (dt * (t2 * (u(i,j) - t55 + u(i - 2,j)) * t16 
     #       + t18 * (u(i - 1,j + 1) - t55 + u(i - 1,j - 1)) * t2
     #       2)) / 0.2D1 + v(i - 2,j) / 0.4D1) * t41 / 0.2D1
          t73 = 1. / (dy)
          t74 = t18 * t73
          t82 = 2 * u(i,j + 1)
          t95 = 0.1D1 / cy
          t98 = ((u(i,j + 1) - u(i,j)) * t73) + (dt) *
     #       (v(i,j + 1) - v(i,j)) * (t73) / 0.2D1 + (0.3D1 
     #       / 0.4D1 * v(i,j + 1) + (dt * (t2 * (u(i + 1,j + 1)
     #       - t82 + u(i - 1,j + 1)) * t16 + t18 * (u(i,j + 2)
     #       - t82 + u(i,j)) * t22)) / 0.2D1 - v(i,j + 2) / 0.4D
     #       1 - t28 - t38 + v(i,j - 1) / 0.4D1) * t95 / 0.2D1
          t108 = 2 * u(i,j - 1)
          t122 = ((u(i,j) - u(i,j - 1)) * t73) + (dt) 
     #       * (v(i,j) - v(i,j - 1)) * (t73) / 0.2D1 + (t28 +
     #       t38 - v(i,j + 1) / 0.4D1 - 0.3D1 / 0.4D1 * v(i,j - 1)
     #       - (dt * (t2 * (u(i + 1,j - 1) - t108 + u(i - 1,j - 1))
     #       * t16 + t18 * (u(i,j) - t108 + u(i,j - 2))
     #       * t22)) / 0.2D1 + v(i,j - 2) / 0.4D1) * t95 / 0.2D1
          unew(i,j) = (u(i,j)) + (dt) * v(i,j) + t4 * (t
     #       44 * (t45) - t69 * (t45)) / 0.2D1 + (t74) * (t98 * 
     #       (t45) - t122 * (t45)) / 0.2D1
          vnew(i,j) = v(i,j) + t4 * (t44 * (dt) - t69 * (dt)) +
     #       (t74) * (t98 * (dt) - t122 * (dt))
c     
          end if
          end do
          end do

        else
          ! no mask
          do j = n2a,n2b
          do i = n1a,n1b
          t2 = (cx ** 2)
          t3 = 1.0 / (dx)
          t4 = (t2) * (t3)
          t12 = 2. * u(i + 1,j)
          t15 = dx ** 2
          t16 = 1.0 / (t15)
          t18 = (cy ** 2)
          t21 = dy ** 2
          t22 = 1. / (t21)
          t28 = 0.3D1 / 0.4D1 * v(i,j)
          t29 = 2. * u(i,j)
          t38 = (dt * (t2 * (u(i + 1,j) - t29 + u(i - 1,j)
     #       ) * t16 + t18 * (u(i,j + 1) - t29 + u(i,j - 1)) * t2
     #       2)) / 0.2D1
          t41 = 0.1D1 / cx
          t44 = ((u(i + 1,j) - u(i,j)) * t3) + (dt) * 
     #       (v(i + 1,j) - v(i,j)) * (t3) / 0.2D1 + (0.3D1 / 
     #       0.4D1 * v(i + 1,j) + (dt * (t2 * (u(i + 2,j) - t
     #       12 + u(i,j)) * t16 + t18 * (u(i + 1,j + 1) - t12 + u
     #       (i + 1,j - 1)) * t22)) / 0.2D1 - v(i + 2,j) / 0.4D1 
     #       - t28 - t38 + v(i - 1,j) / 0.4D1) * t41 / 0.2D1
          t45 = dt ** 2
          t55 = 2 * u(i - 1,j)
          t69 = ((u(i,j) - u(i - 1,j)) * t3) + (dt) * 
     #       (v(i,j) - v(i - 1,j)) * (t3) / 0.2D1 + (t28 + t3
     #       8 - v(i + 1,j) / 0.4D1 - 0.3D1 / 0.4D1 * v(i - 1,j) 
     #       - (dt * (t2 * (u(i,j) - t55 + u(i - 2,j)) * t16 
     #       + t18 * (u(i - 1,j + 1) - t55 + u(i - 1,j - 1)) * t2
     #       2)) / 0.2D1 + v(i - 2,j) / 0.4D1) * t41 / 0.2D1
          t73 = 1. / (dy)
          t74 = t18 * t73
          t82 = 2 * u(i,j + 1)
          t95 = 0.1D1 / cy
          t98 = ((u(i,j + 1) - u(i,j)) * t73) + (dt) *
     #       (v(i,j + 1) - v(i,j)) * (t73) / 0.2D1 + (0.3D1 
     #       / 0.4D1 * v(i,j + 1) + (dt * (t2 * (u(i + 1,j + 1)
     #       - t82 + u(i - 1,j + 1)) * t16 + t18 * (u(i,j + 2)
     #       - t82 + u(i,j)) * t22)) / 0.2D1 - v(i,j + 2) / 0.4D
     #       1 - t28 - t38 + v(i,j - 1) / 0.4D1) * t95 / 0.2D1
          t108 = 2 * u(i,j - 1)
          t122 = ((u(i,j) - u(i,j - 1)) * t73) + (dt) 
     #       * (v(i,j) - v(i,j - 1)) * (t73) / 0.2D1 + (t28 +
     #       t38 - v(i,j + 1) / 0.4D1 - 0.3D1 / 0.4D1 * v(i,j - 1)
     #       - (dt * (t2 * (u(i + 1,j - 1) - t108 + u(i - 1,j - 1))
     #       * t16 + t18 * (u(i,j) - t108 + u(i,j - 2))
     #       * t22)) / 0.2D1 + v(i,j - 2) / 0.4D1) * t95 / 0.2D1
          unew(i,j) = (u(i,j)) + (dt) * v(i,j) + t4 * (t
     #       44 * (t45) - t69 * (t45)) / 0.2D1 + (t74) * (t98 * 
     #       (t45) - t122 * (t45)) / 0.2D1
          vnew(i,j) = v(i,j) + t4 * (t44 * (dt) - t69 * (dt)) +
     #       (t74) * (t98 * (dt) - t122 * (dt))
c     
          end do
          end do

        end if
c
      else 
      ! add forcing flag is set to true

        if( useWhereMask.ne.0 ) then
          do j = n2a,n2b
          do i = n1a,n1b
          if( mask(i,j).gt.0 ) then

            call duStepWaveGen2d2rc_tzOLD( 
     *         nd1a,nd1b,nd2a,nd2b,
     *         n1a,n1b,n2a,n2b,
     *         ndf4a,ndf4b,nComp,
     *         u,v,unew,vnew,
     *         src,
     *         dx,dy,dt,c,
     *         i,j,n )
c     
          end if
          end do
          end do

        else
          ! no mask
          do j = n2a,n2b
          do i = n1a,n1b
            call duStepWaveGen2d2rc_tzOLD( 
     *         nd1a,nd1b,nd2a,nd2b,
     *         n1a,n1b,n2a,n2b,
     *         ndf4a,ndf4b,nComp,
     *         u,v,unew,vnew,
     *         src,
     *         dx,dy,dt,c,
     *         i,j,n )
c     
          end do
          end do

        end if
      end if
c
      return
      end
