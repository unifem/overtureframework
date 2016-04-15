      subroutine duWaveGen3d4rc( 
     *   nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *   n1a,n1b,n2a,n2b,n3a,n3b,
     *   ndf4a,ndf4b,nComp,addForcing,
     *   u,ut,unew,utnew,
     *   src,
     *   dx,dy,dz,dt,cc,beta,
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
      real dx,dy,dz,dt,cc,beta
c
c.. declarations of local variables
      integer i,j,k,n
      integer ix,iy,iz
c
      integer stencilOpt

      real cuu(-3:3,-3:3,-3:3)
      real cuv(-3:3,-3:3,-3:3)
      real cvu(-3:3,-3:3,-3:3)
      real cvv(-3:3,-3:3,-3:3)
c
      n = 1
      stencilOpt = 1
c
      if( n1a-nd1a .lt. 3 ) then
        write(6,*)'Grid not made with enough ghost cells'
        write(6,*)nd1a,n1a
        stop
      end if

      ! fourth order, cartesian, 3D
      if( addForcing.eq.0 )then

        if( stencilOpt .eq. 0 ) then
          if( useWhereMask.ne.0 ) then
            do k = n3a,n3b
            do j = n2a,n2b
            do i = n1a,n1b
            if( mask(i,j,k).gt.0 ) then
c
              call duStepWaveGen3d4rc( 
     *           nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *           n1a,n1b,n2a,n2b,n3a,n3b,
     *           u,ut,unew,utnew,
     *           dx,dy,dz,dt,cc,beta,
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
              call duStepWaveGen3d4rc( 
     *           nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *           n1a,n1b,n2a,n2b,n3a,n3b,
     *           u,ut,unew,utnew,
     *           dx,dy,dz,dt,cc,beta,
     *           i,j,k,n )
c     
            end do
            end do
            end do
          end if
        else
          ! stencil optimized routines
          call getcuu_fourth3D( cc,dx,dy,dz,dt,cuu,beta )
          call getcuv_fourth3D( cc,dx,dy,dz,dt,cuv,beta )
          call getcvu_fourth3D( cc,dx,dy,dz,dt,cvu,beta )
          call getcvv_fourth3D( cc,dx,dy,dz,dt,cvv,beta )
          if( useWhereMask.ne.0 ) then
            do k = n3a,n3b
            do j = n2a,n2b
            do i = n1a,n1b
            if( mask(i,j,k).gt.0 ) then
c
              unew(i,j,k)  = 0.0
              utnew(i,j,k) = 0.0
c
              do iz = -3,3
              do iy = -3+abs(iz),3-abs(iz)
              do ix = -3+abs(iy),3-abs(iy)
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
              do iz = -3,3
              do iy = -3+abs(iz),3-abs(iz)
              do ix = -3+abs(iy),3-abs(iy)
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
            call duStepWaveGen3d4rc_tz( 
     *         nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *         n1a,n1b,n2a,n2b,n3a,n3b,
     *         ndf4a,ndf4b,nComp,
     *         u,ut,unew,utnew,
     *         src,
     *         dx,dy,dz,dt,cc,beta,
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
            call duStepWaveGen3d4rc_tz( 
     *         nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *         n1a,n1b,n2a,n2b,n3a,n3b,
     *         ndf4a,ndf4b,nComp,
     *         u,ut,unew,utnew,
     *         src,
     *         dx,dy,dz,dt,cc,beta,
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
      subroutine getcuu_fourth3D( 
     *   cc,dx,dy,dz,dt,cuu,beta )
c
      implicit real (t)
      real cuu(1:*)
      real dx,dy,dz,dt,cc,beta
      integer i
c
      do i = 1,7**3
        cuu(i) = 0.0
      end do
c 
      t1 = sqrt(0.3E1)
      t2 = 0.1E1 / 0.2E1
      t3 = 0.1E1 / 0.6E1
      t4 = t3 * t1
      t5 = t4 + t2
      t6 = -t4 + t2
      t7 = 0.1E1 / dx
      t8 = t5 - t6
      t9 = t7 ** 2
      t10 = t9 ** 2
      t11 = t7 * t9
      t12 = cc ** 2
      t13 = t12 ** 2
      t14 = cc * t12
      t15 = t14 * t9
      t16 = t15 * beta
      t17 = t16 * dt
      t18 = t17 * t8
      t19 = t6 ** 2
      t20 = t6 * t19
      t21 = t5 ** 2
      t22 = t5 * t21
      t23 = t22 - t20
      t24 = beta ** 2
      t25 = beta * t24
      t26 = dt ** 2
      t27 = t25 * dt * t26
      t28 = t27 * cc * t13
      t29 = 0.1E1 / 0.8E1
      t30 = 0.1E1 / 0.12E2
      t31 = t30 * t28 * t10 * t23
      t32 = t18 * t29 - t31
      t33 = t12 * t9
      t34 = t33 * t30 * t24 * t19 * t26
      t35 = t6 * (-t1 * t32 + t17 * (-t34 + t29))
      t36 = -t26 * (t2 * t35 + t4 * t32)
      t37 = 0.1E1 / dy
      t38 = t37 ** 2
      t39 = t38 ** 2
      t40 = t37 * t38
      t41 = t14 * t38
      t42 = t41 * beta
      t43 = t42 * dt
      t44 = t43 * t8
      t45 = t28 * t38
      t46 = t45 * t9
      t47 = t46 * t23
      t48 = t3 * t47
      t49 = t30 * t44
      t50 = t49 - t48
      t51 = t33 * t3 * t24 * t19 * t26
      t52 = -t51 + t30
      t53 = t6 * (-t1 * t50 + t43 * t52)
      t54 = 0.1E1 / 0.24E2
      t31 = t18 * t54 - t31
      t55 = t6 * (-t1 * t31 + t17 * (-t34 + t54))
      t56 = -t26 * (t2 * t55 + t4 * t31)
      t57 = t56 * t37
      t58 = -t26 * (t2 * t53 + t4 * t50) * t7
      t59 = t58 + t57
      t60 = 0.1E1 / dz
      t61 = t60 ** 2
      t62 = t61 ** 2
      t63 = t60 * t61
      t64 = t28 * t61 * t9
      t65 = t64 * t23
      t14 = t14 * t61
      t66 = t14 * beta
      t67 = t66 * dt
      t68 = t67 * t8
      t69 = t3 * t65
      t70 = t30 * t68
      t71 = -t69 + t70
      t52 = t6 * (-t1 * t71 + t67 * t52)
      t56 = t56 * t60
      t72 = -t26 * (t2 * t52 + t4 * t71) * t7
      t73 = t56 + t72
      t74 = (t38 + t61 + t9) * t12
      t75 = 0.2E1 * t74
      t76 = cc * (t75 + t33)
      t77 = t7 * (t76 - t15)
      t78 = t38 + t61
      t79 = 0.2E1 * t12 * t78 + 0.3E1 * t33
      t80 = t12 * t7
      t81 = t13 * t9
      t78 = 0.2E1 * t81 * t78
      t82 = -t7 * t80 * (t79 + t33) - t78
      t83 = t7 * (t76 + t15)
      t84 = t22 - t20
      t85 = -t22 + t20
      t86 = t85 * t82
      t10 = t13 * t10
      t87 = t10 * t84
      t88 = t27 * cc
      t89 = beta * dt
      t90 = t89 * dx
      t21 = t19 - t21
      t91 = t13 * t26
      t92 = t2 * t91 * t11 * t21
      t93 = 0.13E2 / 0.24E2 * t18
      t94 = -t29 * t90 * t77 * t8 + t30 * t88 * (t87 + t86) + t54 * t90 
     #* t83 * t8 + t92 - t93
      t95 = cc * t7
      t96 = t2 * t13 * t6 * dt
      t11 = t96 * t11
      t16 = 0.13E2 / 0.24E2 * t16
      t97 = beta * dx
      t98 = t30 * cc * (t27 * (t10 - t82) * t20 + t95) + t6 * (dt * (-t1
     #6 - t11 + t97 * (-t29 * t77 + t54 * t83)) - t1 * t94)
      t99 = t1 * t26
      t100 = t6 * (t1 * t31 + t17 * (t34 - t54))
      t55 = (t60 + t37) * (t2 * t26 * (t55 - t100) + 0.2E1 * t3 * t99 * 
     #t31)
      t35 = t7 * (-t2 * t26 * (-t35 + t98) + t3 * t99 * (t32 - t94)) + t
     #55
      t31 = t26 * (t100 * t2 - t4 * t31)
      t100 = t31 * t60
      t72 = t100 + t72
      t31 = t31 * t37
      t58 = t31 + t58
      t101 = t30 * t28 * t39 * t23
      t102 = t44 * t54 - t101
      t103 = t12 * t38
      t104 = t103 * t30 * t24 * t19 * t26
      t105 = t6 * (-t1 * t102 + t43 * (-t104 + t54))
      t106 = -t26 * (t4 * t102 + t105 * t2)
      t107 = t30 * t18
      t108 = t107 - t48
      t109 = t103 * t3 * t24 * t19 * t26
      t110 = -t109 + t30
      t111 = t6 * (-t1 * t108 + t17 * t110)
      t112 = -t26 * (t4 * t108 + t111 * t2) * t37
      t113 = t106 * t7
      t114 = t113 + t112
      t45 = t45 * t61
      t115 = t45 * t23
      t116 = t115 * t1
      t45 = t26 * (t30 * t6 * (t45 * t19 - t116) + t116 / 0.36E2)
      t65 = t65 * t1
      t64 = t26 * (t30 * t6 * (t64 * t19 - t65) + t65 / 0.36E2)
      t47 = t47 * t1
      t46 = t26 * (t30 * t6 * (t46 * t19 - t47) + t47 / 0.36E2)
      t47 = t64 * t37
      t65 = t46 * t60
      t116 = t45 * t7
      t117 = t65 + t116 + t47
      t118 = t61 + t9
      t119 = 0.2E1 * t12 * t118 + 0.3E1 * t103
      t120 = t12 * t37
      t118 = 0.2E1 * t13 * t38 * t118
      t121 = -t37 * t120 * (t119 + t103) - t118
      t122 = t91 * t38
      t123 = t30 * t88 * t121 * t23
      t124 = t2 * t122 * t7 * t21
      t125 = 0.5E1 / 0.12E2 * t44
      t126 = -t123 + t124 + t48 - t125
      t127 = t30 * t25 * t19 * t26
      t128 = t127 * t121
      t129 = 0.5E1 / 0.12E2 * t103 * beta
      t130 = t41 * t2 * t6 * dt
      t131 = t130 * t7
      t132 = t81 * t3 * t25 * t19 * t26
      t133 = t132 * t38
      t134 = dt * cc
      t135 = t6 * (-t1 * t126 + t134 * (t133 - t128 - t129 - t131))
      t81 = t81 * t26
      t136 = t30 * t88 * t82 * t23
      t137 = t2 * t81 * t37 * t21
      t138 = 0.5E1 / 0.12E2 * t18
      t139 = -t136 + t137 + t48 - t138
      t140 = t127 * t82
      t141 = 0.5E1 / 0.12E2 * t33 * beta
      t142 = t15 * t2 * t6 * dt
      t143 = t142 * t37
      t144 = t6 * (-t1 * t139 + t134 * (t133 - t140 - t141 - t143))
      t145 = -0.2E1 * t46 * t60
      t146 = t37 * (-t2 * t26 * (-t111 + t144) + t3 * t99 * (t108 - t139
     #)) + t7 * (-t2 * t26 * (-t53 + t135) + t3 * t99 * (t50 - t126)) + 
     #t145
      t46 = t46 * t60
      t147 = t46 + t116 + t47
      t28 = t30 * t28 * t62 * t23
      t148 = t54 * t68 - t28
      t149 = t12 * t61
      t150 = t149 * t30 * t24 * t19 * t26
      t151 = t6 * (-t1 * t148 + t67 * (-t150 + t54))
      t152 = -t26 * (t4 * t148 + t151 * t2)
      t107 = t107 - t69
      t24 = t149 * t3 * t24 * t19 * t26
      t153 = -t24 + t30
      t154 = t6 * (-t1 * t107 + t17 * t153)
      t155 = -t26 * (t4 * t107 + t154 * t2) * t60
      t156 = t152 * t7
      t157 = t155 + t156
      t9 = t38 + t9
      t38 = 0.2E1 * t12 * t9 + 0.3E1 * t149
      t12 = t12 * t60
      t9 = 0.2E1 * t13 * t61 * t9
      t158 = -t60 * t12 * (t149 + t38) - t9
      t159 = t91 * t61
      t160 = t30 * t88 * t158 * t23
      t161 = t2 * t159 * t7 * t21
      t162 = 0.5E1 / 0.12E2 * t68
      t163 = -t160 - t162 + t69 + t161
      t164 = t127 * t158
      t165 = 0.5E1 / 0.12E2 * t149 * beta
      t166 = t14 * t2 * t6 * dt
      t167 = t166 * t7
      t132 = t132 * t61
      t168 = t6 * (-t1 * t163 + t134 * (t132 - t164 - t165 - t167))
      t81 = t2 * t81 * t60 * t21
      t136 = -t136 + t69 - t138 + t81
      t142 = t142 * t60
      t169 = t6 * (-t1 * t136 + t134 * (t132 - t140 - t141 - t142))
      t170 = -0.2E1 * t64 * t37
      t171 = t60 * (-t2 * t26 * (-t154 + t169) + t3 * t99 * (t107 - t136
     #)) + t7 * (t2 * t26 * (t52 - t168) + t3 * t99 * (t71 - t163)) + t1
     #70
      t172 = -t79
      t173 = 0.2E1 * t37 * t120 * t119 + 0.2E1 * t60 * t12 * t38 + t7 * 
     #t80 * (t79 - t172)
      t174 = 0.2E1 * t7 * t76
      t175 = t84 * t173
      t176 = t80 * t26
      t18 = 0.2E1 / 0.3E1 * t18
      t177 = -0.5E1 / 0.4E1 * t89 * cc * t74 * t8
      t86 = -t2 * t176 * t79 * t21 - t29 * t90 * (t5 * t77 - t6 * t77) -
     # t30 * t88 * (t86 + t175) - t54 * t90 * (t5 * (t83 + t174) + t6 * 
     #(-t83 - t174)) + t18 - t177
      t178 = t89 * t6
      t74 = t178 * t74
      t179 = t90 * t6
      t180 = t88 * t20
      t181 = 0.2E1 / 0.3E1 * t95 * beta
      t182 = t2 * dt * t6
      t183 = t80 * dt
      t184 = 0.5E1 / 0.4E1 * cc * (t74 - t95)
      t77 = -t29 * t179 * t77 - t30 * t180 * (-t82 + t173) - t54 * t179 
     #* (t83 + t174) + t6 * (-t1 * t86 + t183 * (t182 * t79 + t181)) + t
     #184
      t79 = t6 * (t1 * t139 + t134 * (-t133 + t140 + t141 + t143))
      t82 = t6 * (t1 * t136 + t134 * (-t132 + t140 + t141 + t142))
      t83 = t37 * (t2 * t26 * (t144 - t79) + 0.2E1 * t3 * t99 * t139) + 
     #t60 * (t2 * t26 * (t169 - t82) + 0.2E1 * t3 * t99 * t136) + t7 * (
     #t2 * t26 * (t98 - t77) - t3 * t99 * (-t94 + t86))
      t9 = -t60 * t12 * (t149 + t38) - t9
      t94 = t30 * t88 * t9 * t23
      t98 = -t94 - t162 + t69 + t161
      t140 = t127 * t9
      t144 = t6 * (-t1 * t98 + t134 * (t132 - t140 - t165 - t167))
      t24 = t24 - t30
      t161 = t6 * (t1 * t107 + t17 * t24)
      t52 = t60 * (t2 * t26 * (t82 - t161) + t3 * t99 * (t107 - t136)) +
     # t7 * (t2 * t26 * (t52 - t144) - t3 * t99 * (-t71 + t98)) + t170
      t82 = t26 * (-t4 * t107 + t161 * t2) * t60
      t64 = t64 * t37
      t118 = -t37 * t120 * (t119 + t103) - t118
      t136 = t30 * t88 * t118 * t23
      t124 = -t136 + t124 + t48 - t125
      t169 = t127 * t118
      t185 = t6 * (-t1 * t124 + t134 * (t133 - t169 - t129 - t131))
      t109 = t109 - t30
      t186 = t6 * (t1 * t108 + t17 * t109)
      t53 = t37 * (-t2 * t26 * (-t79 + t186) + t3 * t99 * (t108 - t139))
     # + t7 * (-t2 * t26 * (-t53 + t185) + t3 * t99 * (t50 - t124)) + t1
     #45
      t79 = t26 * (-t4 * t108 + t186 * t2) * t37
      t101 = t29 * t44 - t101
      t139 = t6 * (-t1 * t101 + t43 * (-t104 + t29))
      t187 = -t26 * (t4 * t101 + t139 * t2)
      t115 = t3 * t115
      t70 = -t115 + t70
      t110 = t6 * (-t1 * t70 + t67 * t110)
      t188 = -t26 * (t110 * t2 + t4 * t70) * t37
      t106 = t106 * t60
      t189 = t6 * (t1 * t102 + t43 * (t104 - t54))
      t103 = cc * (t75 + t103)
      t190 = t37 * (t103 + t41)
      t191 = t37 * (t103 - t41)
      t192 = t89 * dy
      t193 = t85 * t121
      t39 = t13 * t39
      t194 = t2 * t91 * t40 * t21
      t195 = 0.13E2 / 0.24E2 * t44
      t196 = t54 * t192 * t190 * t8 - t29 * t192 * t191 * t8 + t30 * t88
     # * (t39 * t84 + t193) + t194 - t195
      t197 = cc * t37
      t40 = t96 * t40
      t42 = 0.13E2 / 0.24E2 * t42
      t198 = beta * dy
      t199 = t30 * cc * (t27 * (t39 - t121) * t20 + t197) + t6 * (dt * (
     #-t42 + t198 * (t190 * t54 - t191 * t29) - t40) - t1 * t196)
      t105 = (t60 + t7) * (t2 * t26 * (t105 - t189) + 0.2E1 * t3 * t99 *
     # t102)
      t139 = t37 * (-t2 * t26 * (-t139 + t199) + t3 * t99 * (t101 - t196
     #)) + t105
      t102 = t26 * (-t4 * t102 + t189 * t2)
      t189 = t102 * t60
      t49 = t49 - t115
      t153 = t6 * (-t1 * t49 + t43 * t153)
      t200 = -t26 * (t153 * t2 + t4 * t49) * t60
      t152 = t152 * t37
      t201 = t152 + t200
      t159 = t2 * t159 * t37 * t21
      t160 = -t160 - t162 + t115 + t159
      t166 = t166 * t37
      t19 = t122 * t3 * t25 * t19 * t61
      t25 = t6 * (-t1 * t160 + t134 * (t19 - t164 - t165 - t166))
      t61 = t2 * t122 * t60 * t21
      t122 = -t123 + t115 - t125 + t61
      t123 = t130 * t60
      t130 = t6 * (-t1 * t122 + t134 * (t19 - t128 - t129 - t123))
      t202 = -0.2E1 * t45 * t7
      t203 = t37 * (-t2 * t26 * (-t110 + t25) + t3 * t99 * (t70 - t160))
     # + t60 * (t2 * t26 * (t153 - t130) - t3 * t99 * (-t49 + t122)) + t
     #202
      t204 = t6 * (t1 * t126 + t134 * (-t133 + t128 + t129 + t131))
      t205 = 0.2E1 * t37 * t103
      t206 = t120 * t26
      t44 = 0.2E1 / 0.3E1 * t44
      t175 = -t2 * t206 * t119 * t21 + t29 * t192 * (-t5 * t191 + t191 *
     # t6) - t30 * t88 * (t193 + t175) + t54 * t192 * (t5 * (-t190 - t20
     #5) + t6 * (t190 + t205)) - t177 + t44
      t193 = t178 * dy
      t207 = 0.2E1 / 0.3E1 * t197 * beta
      t120 = t120 * dt
      t208 = 0.5E1 / 0.4E1 * cc * (-t197 + t74)
      t121 = -t29 * t193 * t191 - t30 * t180 * (-t121 + t173) - t54 * t1
     #93 * (t190 + t205) + t6 * (-t1 * t175 + t120 * (t182 * t119 + t207
     #)) + t208
      t128 = t6 * (t1 * t122 + t134 * (-t19 + t128 + t129 + t123))
      t130 = t37 * (t2 * t26 * (t199 - t121) - t3 * t99 * (-t196 + t175)
     #) + t60 * (0.2E1 * t3 * t99 * t122 - t2 * t26 * (-t130 + t128)) + 
     #t7 * (0.2E1 * t3 * t99 * t126 + t2 * t26 * (t135 - t204))
      t94 = -t94 - t162 + t115 + t159
      t135 = t6 * (-t1 * t94 + t134 * (t19 - t140 - t165 - t166))
      t24 = t6 * (t1 * t49 + t43 * t24)
      t110 = t37 * (t2 * t26 * (t110 - t135) - t3 * t99 * (-t70 + t94)) 
     #+ t60 * (t2 * t26 * (t128 - t24) + t3 * t99 * (-t122 + t49)) + t20
     #2
      t122 = t26 * (t2 * t24 - t4 * t49) * t60
      t28 = t29 * t68 - t28
      t128 = t6 * (-t1 * t28 + t67 * (-t150 + t29))
      t159 = -t26 * (t128 * t2 + t4 * t28)
      t162 = t6 * (t1 * t148 + t67 * (t150 - t54))
      t75 = cc * (t75 + t149)
      t149 = t60 * (t75 - t14)
      t190 = t60 * (t75 + t14)
      t13 = t13 * t62
      t62 = t89 * dz
      t89 = t2 * t91 * t63 * t21
      t91 = 0.13E2 / 0.24E2 * t68
      t191 = -t29 * t62 * t149 * t8 + t30 * t88 * (t13 * t84 + t158 * t8
     #5) + t54 * t62 * t190 * t8 + t89 - t91
      t196 = cc * t60
      t63 = t96 * t63
      t66 = 0.13E2 / 0.24E2 * t66
      t96 = beta * dz
      t199 = t30 * cc * (t27 * (t13 - t158) * t20 + t196) + t6 * (dt * (
     #-t66 + t96 * (-t149 * t29 + t190 * t54) - t63) - t1 * t191)
      t151 = (t7 + t37) * (t2 * t26 * (t151 - t162) + 0.2E1 * t3 * t99 *
     # t148)
      t128 = t60 * (-t2 * t26 * (-t128 + t199) + t3 * t99 * (t28 - t191)
     #) + t151
      t209 = t6 * (t1 * t163 + t134 * (-t132 + t164 + t165 + t167))
      t164 = t6 * (t1 * t160 + t134 * (-t19 + t164 + t165 + t166))
      t210 = 0.2E1 * t60 * t75
      t211 = t12 * t26
      t68 = 0.2E1 / 0.3E1 * t68
      t212 = -t2 * t211 * t38 * t21 - t29 * t62 * (t149 * t5 - t6 * t149
     #) - t30 * t88 * (t20 * (t158 - t173) + t22 * (-t158 + t173)) - t54
     # * t62 * (t5 * (t190 + t210) + t6 * (-t190 - t210)) - t177 + t68
      t178 = t178 * dz
      t213 = 0.2E1 / 0.3E1 * t196 * beta
      t12 = t12 * dt
      t74 = 0.5E1 / 0.4E1 * cc * (t74 - t196)
      t149 = -t29 * t178 * t149 - t30 * t180 * (-t158 + t173) - t54 * t1
     #78 * (t190 + t210) + t6 * (-t1 * t212 + t12 * (t182 * t38 + t213))
     # + t74
      t25 = t37 * (-t2 * t26 * (-t25 + t164) + 0.2E1 * t3 * t99 * t160) 
     #+ t60 * (t2 * t26 * (t199 - t149) + t3 * t99 * (t191 - t212)) + t7
     # * (-t2 * t26 * (-t168 + t209) + 0.2E1 * t3 * t99 * t163)
      t158 = t7 * (-t76 + t15)
      t33 = t7 * t80 * (t172 - t33) - t78
      t15 = t7 * (-t76 - t15)
      t18 = -t2 * t176 * t172 * t21 - t29 * t90 * (t158 * t5 - t6 * t158
     #) + t30 * t88 * (t20 * (-t173 + t33) + t22 * (t173 - t33)) + t54 *
     # t90 * (t5 * (t174 - t15) + t6 * (-t174 + t15)) + t177 - t18
      t76 = -t29 * t179 * t158 + t30 * t180 * (t173 - t33) + t54 * t179 
     #* (t174 - t15) + t6 * (-t1 * t18 + t183 * (t182 * t172 - t181)) - 
     #t184
      t78 = t37 * (t103 + t41)
      t41 = t37 * (-t103 + t41)
      t44 = t2 * t206 * t119 * t21 + t29 * t192 * (-t5 * t41 + t41 * t6)
     # + t30 * t88 * (t20 * (-t173 + t118) + t22 * (t173 - t118)) - t54 
     #* t192 * (t5 * (-t205 - t78) + t6 * (t205 + t78)) + t177 - t44
      t80 = -t29 * t193 * t41 + t30 * t180 * (t173 - t118) + t54 * t193 
     #* (t205 + t78) + t6 * (-t1 * t44 + t120 * (-t182 * t119 - t207)) -
     # t208
      t103 = t60 * (-t75 + t14)
      t14 = t60 * (-t75 - t14)
      t5 = t2 * t211 * t38 * t21 - t29 * t62 * (t103 * t5 - t6 * t103) +
     # t30 * t88 * (t20 * (-t173 + t9) + t22 * (t173 - t9)) - t54 * t62 
     #* (t5 * (-t210 + t14) + t6 * (t210 - t14)) + t177 - t68
      t12 = -t29 * t178 * t103 + t30 * t180 * (t173 - t9) - t54 * t178 *
     # (-t210 + t14) + t6 * (-t1 * t5 + t12 * (-t182 * t38 - t213)) - t7
     #4
      t21 = t37 * (t2 * t26 * (t121 - t80) + t3 * t99 * (t175 - t44)) + 
     #t60 * (-t2 * t26 * (-t149 + t12) - t3 * t99 * (-t212 + t5)) + t7 *
     # (t2 * t26 * (t77 - t76) + t3 * t99 * (t86 - t18)) + 0.1E1
      t22 = t6 * (t1 * t98 + t134 * (-t132 + t140 + t165 + t167))
      t38 = t6 * (t1 * t94 + t134 * (-t19 + t140 + t165 + t166))
      t62 = -t29 * t62 * t103 * t8 + t30 * t88 * (t13 * t85 + t84 * t9) 
     #+ t54 * t62 * t14 * t8 - t89 + t91
      t9 = t30 * cc * (t27 * (-t13 + t9) * t20 - t196) + t6 * (dt * (t66
     # + t96 * (-t103 * t29 + t14 * t54) + t63) - t1 * t62)
      t5 = t37 * (0.2E1 * t3 * t99 * t94 - t2 * t26 * (-t135 + t38)) + t
     #60 * (t2 * t26 * (t12 - t9) - t3 * t99 * (-t5 + t62)) + t7 * (0.2E
     #1 * t3 * t99 * t98 - t2 * t26 * (-t144 + t22))
      t12 = t6 * (t1 * t28 + t67 * (t150 - t29))
      t9 = t60 * (t2 * t26 * (t9 - t12) + t3 * t99 * (t62 + t28)) + t151
      t13 = t26 * (-t4 * t148 + t162 * t2)
      t14 = t13 * t37
      t62 = t6 * (t1 * t70 + t67 * t109)
      t61 = -t136 + t115 - t125 + t61
      t63 = t6 * (-t1 * t61 + t134 * (t19 - t169 - t129 - t123))
      t66 = t37 * (t2 * t26 * (t164 - t62) - t3 * t99 * (t160 - t70)) + 
     #t60 * (t2 * t26 * (t153 - t63) - t3 * t99 * (-t49 + t61)) + t202
      t68 = t6 * (t1 * t124 + t134 * (-t133 + t169 + t129 + t131))
      t74 = -t29 * t192 * t41 * t8 + t30 * t88 * (t118 * t84 + t39 * t85
     #) - t54 * t192 * t78 * t8 - t194 + t195
      t39 = t30 * cc * (t27 * (-t39 + t118) * t20 - t197) + t6 * (dt * (
     #-t198 * (t29 * t41 + t54 * t78) + t42 + t40) - t1 * t74)
      t19 = t6 * (t1 * t61 + t134 * (-t19 + t169 + t129 + t123))
      t40 = t37 * (-t2 * t26 * (-t80 + t39) + t3 * t99 * (t44 - t74)) + 
     #t60 * (0.2E1 * t3 * t99 * t61 + t2 * t26 * (t63 - t19)) + t7 * (t2
     # * t26 * (t185 - t68) + 0.2E1 * t3 * t99 * t124)
      t19 = t37 * (t2 * t26 * (t38 - t62) + t3 * t99 * (-t94 + t70)) + t
     #60 * (-t2 * t26 * (t24 - t19) + t3 * t99 * (t49 - t61)) + t202
      t24 = t26 * (t2 * t62 - t4 * t70) * t37
      t38 = t6 * (t1 * t101 + t43 * (t104 - t29))
      t39 = t37 * (t2 * t26 * (t39 - t38) + t3 * t99 * (t74 + t101)) + t
     #105
      t41 = t102 * t7
      t42 = t45 * t7
      t44 = t51 - t30
      t43 = t6 * (t1 * t50 + t43 * t44)
      t23 = -t30 * t88 * t33 * t23
      t45 = t23 + t137 + t48 - t138
      t48 = t127 * t33
      t49 = t6 * (-t1 * t45 + t134 * (t133 - t141 - t48 - t143))
      t51 = t37 * (t2 * t26 * (t111 - t49) + t3 * t99 * (t108 - t45)) + 
     #t7 * (-t2 * t26 * (-t204 + t43) + t3 * t99 * (t50 - t126)) + t145
      t13 = t13 * t7
      t44 = t6 * (t1 * t71 + t67 * t44)
      t23 = t23 + t69 - t138 + t81
      t61 = t6 * (-t1 * t23 + t134 * (t132 - t141 - t48 - t142))
      t62 = t60 * (t2 * t26 * (t154 - t61) + t3 * t99 * (t107 - t23)) + 
     #t7 * (t2 * t26 * (t209 - t44) + t3 * t99 * (t71 - t163)) + t170
      t8 = -t29 * t90 * t158 * t8 - t30 * t88 * (t33 * t85 + t87) + t54 
     #* t90 * t15 * t8 - t92 + t93
      t10 = -t30 * cc * (t27 * (t10 - t33) * t20 + t95) + t6 * (dt * (t9
     #7 * (t15 * t54 - t158 * t29) + t16 + t11) - t1 * t8)
      t11 = t6 * (t1 * t45 + t134 * (-t133 + t141 + t48 + t143))
      t15 = t6 * (t1 * t23 + t134 * (-t132 + t141 + t48 + t142))
      t16 = t37 * (0.2E1 * t3 * t99 * t45 + t2 * t26 * (t49 - t11)) + t6
     #0 * (0.2E1 * t3 * t99 * t23 + t2 * t26 * (t61 - t15)) + t7 * (-t2 
     #* t26 * (-t76 + t10) + t3 * t99 * (t18 - t8))
      t15 = t60 * (-t2 * t26 * (t161 - t15) + t3 * t99 * (t107 - t23)) +
     # t7 * (t2 * t26 * (t22 - t44) + t3 * t99 * (-t98 + t71)) + t170
      t11 = t37 * (-t2 * t26 * (t186 - t11) + t3 * t99 * (t108 - t45)) +
     # t7 * (-t2 * t26 * (-t68 + t43) + t3 * t99 * (t50 - t124)) + t145
      t18 = t26 * (t2 * t43 - t4 * t50) * t7
      t20 = t26 * (t2 * t44 - t4 * t71) * t7
      t22 = -t32
      t1 = t6 * (-t1 * t22 + t17 * (t34 - t29))
      t3 = t7 * (t2 * t26 * (t10 - t1) - t3 * t99 * (-t8 + t22)) + t55
      t1 = t26 * (t1 * t2 + t4 * t22)
      cuu(25) = t36 * t7
      cuu(67) = t59
      cuu(73) = t73
      cuu(74) = t35
      cuu(75) = t72
      cuu(81) = t58
      cuu(109) = t114
      cuu(115) = t117
      cuu(116) = t146
      cuu(117) = t147
      cuu(121) = t157
      cuu(122) = t171
      cuu(123) = t83
      cuu(124) = t52
      cuu(125) = t82 + t156
      cuu(129) = t64 + t65 + t116
      cuu(130) = t53
      cuu(131) = t46 + t64 + t116
      cuu(137) = t79 + t113
      cuu(151) = t187 * t37
      cuu(157) = t106 + t188
      cuu(158) = t139
      cuu(159) = t188 + t189
      cuu(163) = t201
      cuu(164) = t203
      cuu(165) = t130
      cuu(166) = t110
      cuu(167) = t152 + t122
      cuu(169) = t159 * t60
      cuu(170) = t128
      cuu(171) = t25
      cuu(172) = t21
      cuu(173) = t5
      cuu(174) = t9
      cuu(175) = t26 * (t12 * t2 - t4 * t28) * t60
      cuu(177) = t200 + t14
      cuu(178) = t66
      cuu(179) = t40
      cuu(180) = t19
      cuu(181) = t122 + t14
      cuu(185) = t106 + t24
      cuu(186) = t39
      cuu(187) = t189 + t24
      cuu(193) = t26 * (-t4 * t101 + t2 * t38) * t37
      cuu(207) = t41 + t112
      cuu(213) = t42 + t65 + t47
      cuu(214) = t51
      cuu(215) = t46 + t42 + t47
      cuu(219) = t155 + t13
      cuu(220) = t62
      cuu(221) = t16
      cuu(222) = t15
      cuu(223) = t82 + t13
      cuu(227) = t64 + t42 + t65
      cuu(228) = t11
      cuu(229) = t46 + t64 + t42
      cuu(235) = t79 + t41
      cuu(263) = t18 + t57
      cuu(269) = t56 + t20
      cuu(270) = t3
      cuu(271) = t100 + t20
      cuu(277) = t31 + t18
      cuu(319) = t1 * t7
c
      return
      end
c
c++++++++++++++++
c
      subroutine getcuv_fourth3D( 
     *   cc,dx,dy,dz,dt,cuv,beta )
c
      implicit real (t)
      real cuv(1:*)
      real dx,dy,dz,dt,cc,beta
      integer i
c
      do i = 1,7**3
        cuv(i) = 0.0
      end do
c

      t1 = sqrt(0.3E1)
      t2 = 0.1E1 / 0.2E1
      t3 = 0.1E1 / 0.6E1
      t4 = t3 * t1
      t5 = t4 + t2
      t4 = -t4 + t2
      t6 = 0.1E1 / dx
      t7 = t5 ** 2
      t8 = t4 ** 2
      t9 = -t8 + t7
      t10 = beta ** 2
      t11 = dt ** 2
      t12 = dt * t11
      t13 = cc ** 2
      t14 = cc * t13
      t15 = t6 ** 2
      t16 = t14 * t15
      t17 = t16 * t10 * t11
      t18 = t17 * t9
      t19 = t18 * t1
      t17 = t4 * (-t17 * t4 + t19)
      t20 = -0.5E1 / 0.288E3 * cc
      t21 = -t17 / 0.12E2 + t20
      t22 = t18 / 0.72E2
      t23 = t22 * t1
      t24 = -t11 * (t2 * t21 + t23)
      t25 = 0.1E1 / dy
      t26 = t25 ** 2
      t27 = t14 * t26
      t28 = t27 * t10 * t11
      t29 = t28 * t9
      t28 = t4 * (t1 * t29 - t28 * t4)
      t30 = t28 / 0.32E2
      t31 = t29 / 0.96E2
      t32 = t11 * (-t31 * t1 + t30)
      t19 = t11 * (t17 / 0.96E2 - t19 / 0.288E3)
      t33 = t19 * t25
      t34 = t32 * t6
      t35 = 0.1E1 / dz
      t36 = t35 ** 2
      t14 = t14 * t36
      t37 = t14 * t10 * t11
      t38 = t37 * t9
      t37 = t4 * (t1 * t38 - t37 * t4)
      t39 = t37 / 0.32E2
      t40 = t38 / 0.96E2
      t41 = t11 * (-t40 * t1 + t39)
      t42 = t19 * t35
      t43 = t41 * t6
      t44 = (t26 + t15 + t36) * t13
      t45 = 0.2E1 * t44
      t46 = t13 * t15
      t47 = cc * (t46 + t45)
      t48 = t6 * (t16 - t47)
      t49 = t5 - t4
      t50 = dt * t6
      t51 = t50 * t13
      t52 = t51 * t49
      t5 = t4 * t8 - t5 * t7
      t53 = t13 ** 2 * t12
      t54 = t53 * t6 * t15
      t55 = t10 * t11
      t56 = t55 * dx
      t57 = 0.1E1 / 0.16E2
      t58 = 0.7E1 / 0.24E2 * t18
      t59 = t52 / 0.12E2
      t60 = t3 * t54 * t5
      t61 = t48 * t56 * t57 * t9 - t58 + t59 + t60
      t62 = 0.7E1 / 0.24E2 * t16
      t54 = t54 * t3 * t4
      t63 = t51 / 0.12E2
      t64 = 0.25E2 / 0.288E3 * cc
      t65 = t4 * (-t1 * t61 + t4 * (t55 * (dx * t48 * t57 - t62) - t54) 
     #+ t63) + t64
      t66 = t1 * t11
      t67 = -0.2E1 * (t35 + t25) * t19
      t68 = t6 * (t2 * t11 * (t21 - t65) + t66 * (-t3 * t61 + t22)) + t6
     #7
      t69 = t19 * t35
      t70 = t19 * t25
      t71 = t32 / 0.3E1
      t19 = 0.3E1 * t19
      t72 = t19 * t25
      t73 = t71 * t6
      t74 = t53 * t26
      t75 = 0.13E2 / 0.48E2 * t29
      t76 = t3 * t74 * t6 * t5 - t75
      t10 = 0.13E2 / 0.48E2 * t10
      t50 = t50 * t3 * cc * t4 + t10
      t77 = t27 * t4 * t11
      t78 = t77 * t50
      t79 = t4 * (-t1 * t76 - t78)
      t80 = t53 * t15
      t81 = 0.13E2 / 0.48E2 * t18
      t82 = t3 * t80 * t25 * t5 - t81
      t83 = t3 * cc * t4 * dt
      t84 = t83 * t25 + t10
      t85 = t16 * t4 * t11
      t86 = t85 * t84
      t87 = t4 * (-t1 * t82 - t86)
      t88 = t18 / 0.96E2
      t17 = t17 / 0.32E2
      t89 = t11 * (t1 * (-t3 * t82 + t88) - t2 * t87 - t17) * t25
      t90 = t11 * (t1 * (-t3 * t76 + t31) - t2 * t79 - t30) * t6
      t91 = t41 / 0.3E1
      t92 = t19 * t35
      t93 = t91 * t6
      t94 = t53 * t36
      t95 = 0.13E2 / 0.48E2 * t38
      t96 = t3 * t94 * t6 * t5 - t95
      t97 = t14 * t4 * t11
      t50 = t97 * t50
      t98 = t4 * (-t1 * t96 - t50)
      t80 = t3 * t80 * t35 * t5 - t81
      t10 = t83 * t35 + t10
      t81 = t85 * t10
      t83 = t4 * (-t1 * t80 - t81)
      t85 = t11 * (t1 * (-t3 * t80 + t88) - t2 * t83 - t17) * t35
      t99 = t11 * (t1 * (-t3 * t96 + t40) - t2 * t98 - t39) * t6
      t100 = 0.2E1 * t13 * (t26 + t36) + 0.3E1 * t46
      t101 = t55 * cc
      t12 = t13 * t12
      t102 = t12 * t100 * t6
      t52 = 0.5E1 / 0.4E1 * t52
      t103 = 0.5E1 / 0.8E1 * t101 * t44 * t9
      t18 = t18 / 0.3E1
      t104 = -t3 * t102 * t5 - t57 * t56 * (-t7 * t48 + t48 * t8) - t52 
     #+ t103 + t18
      t105 = t56 * t8
      t44 = 0.5E1 / 0.8E1 * t44
      t46 = t101 * (t46 / 0.3E1 + t44)
      t51 = 0.5E1 / 0.4E1 * t51
      t106 = 0.25E2 / 0.144E3 * cc
      t48 = t57 * t105 * t48 + t4 * (-t1 * t104 + t4 * (t102 * t3 * t4 +
     # t46) - t51) - t106
      t86 = t4 * (t1 * t82 + t86)
      t81 = t4 * (t1 * t80 + t81)
      t87 = (t2 * t11 * (t87 - t86) + 0.2E1 * t3 * t66 * t82) * t25
      t83 = (0.2E1 * t3 * t66 * t80 - t2 * t11 * (-t83 + t81)) * t35
      t61 = t6 * (-t2 * t11 * (-t65 + t48) - t3 * t66 * (-t61 + t104)) +
     # t87 + t83
      t65 = t11 * (t1 * (-t3 * t80 + t88) + t2 * t81 - t17) * t35
      t80 = t19 * t35
      t17 = t11 * (t1 * (-t3 * t82 + t88) + t2 * t86 - t17) * t25
      t19 = t19 * t25
      t28 = -t28 / 0.12E2 + t20
      t81 = t29 / 0.72E2
      t82 = t81 * t1
      t86 = -t11 * (t2 * t28 + t82)
      t88 = t71 * t35
      t102 = t41 * t25
      t107 = t13 * t26
      t108 = cc * (t107 + t45)
      t109 = t25 * (t27 - t108)
      t110 = t55 * dy
      t111 = dt * t13
      t112 = t111 * t25
      t113 = t112 * t49
      t114 = t53 * t25 * t26
      t115 = 0.7E1 / 0.24E2 * t29
      t116 = t113 / 0.12E2
      t117 = t3 * t114 * t5
      t118 = t57 * t110 * t109 * t9 - t115 + t116 + t117
      t119 = 0.7E1 / 0.24E2 * t27
      t114 = t114 * t3 * t4
      t120 = t112 / 0.12E2
      t121 = t4 * (-t1 * t118 + t4 * (t55 * (dy * t109 * t57 - t119) - t
     #114) + t120) + t64
      t122 = -0.2E1 / 0.3E1 * (t35 + t6) * t32
      t123 = t25 * (-t2 * t11 * (-t28 + t121) + t66 * (-t118 * t3 + t81)
     #) + t122
      t124 = t71 * t35
      t125 = t102 + t124
      t126 = t32 * t35
      t127 = t91 * t25
      t94 = t3 * t94 * t25 * t5 - t95
      t84 = t97 * t84
      t95 = t4 * (-t1 * t94 - t84)
      t74 = t3 * t74 * t35 * t5 - t75
      t10 = t77 * t10
      t75 = t4 * (-t1 * t74 - t10)
      t77 = t11 * (t1 * (-t3 * t74 + t31) - t2 * t75 - t30) * t35
      t97 = t11 * (t1 * (-t3 * t94 + t40) - t2 * t95 - t39) * t25
      t78 = t4 * (t1 * t76 + t78)
      t128 = 0.2E1 * t13 * (t15 + t36) + 0.3E1 * t107
      t129 = t8 - t7
      t129 = t12 * t128 * t25
      t113 = 0.5E1 / 0.4E1 * t113
      t29 = t29 / 0.3E1
      t130 = -t3 * t129 * t5 + t57 * t110 * t109 * (-t8 + t7) + t103 - t
     #113 + t29
      t131 = t110 * t8
      t107 = t101 * (t107 / 0.3E1 + t44)
      t112 = 0.5E1 / 0.4E1 * t112
      t109 = t57 * t131 * t109 + t4 * (-t1 * t130 + t4 * (t129 * t3 * t4
     # + t107) - t112) - t106
      t10 = t4 * (t1 * t74 + t10)
      t79 = (0.2E1 * t3 * t66 * t76 + t2 * t11 * (t79 - t78)) * t6
      t75 = (0.2E1 * t3 * t66 * t74 + t2 * t11 * (t75 - t10)) * t35
      t118 = t25 * (t2 * t11 * (t121 - t109) + t3 * t66 * (t118 - t130))
     # + t79 + t75
      t10 = t11 * (t1 * (-t3 * t74 + t31) + t10 * t2 - t30) * t35
      t74 = t32 * t35
      t20 = -t37 / 0.12E2 + t20
      t37 = t38 / 0.72E2
      t121 = t37 * t1
      t129 = -t11 * (t2 * t20 + t121)
      t132 = t13 * t36
      t45 = cc * (t132 + t45)
      t133 = t35 * (-t14 + t45)
      t111 = t111 * t35
      t49 = t111 * t49
      t36 = t53 * t35 * t36
      t53 = t55 * dz
      t134 = 0.7E1 / 0.24E2 * t38
      t135 = t49 / 0.12E2
      t136 = t3 * t36 * t5
      t137 = -t57 * t53 * t133 * t9 - t134 + t135 + t136
      t138 = 0.7E1 / 0.24E2 * t14
      t36 = t36 * t3 * t4
      t139 = t111 / 0.12E2
      t140 = t4 * (-t1 * t137 + t4 * (-t55 * (dz * t133 * t57 + t138) - 
     #t36) + t139) + t64
      t141 = -0.2E1 / 0.3E1 * (t6 + t25) * t41
      t142 = t35 * (-t2 * t11 * (-t20 + t140) + t66 * (-t137 * t3 + t37)
     #) + t141
      t50 = t4 * (t1 * t96 + t50)
      t84 = t4 * (t1 * t94 + t84)
      t13 = 0.2E1 * t13 * (t26 + t15) + 0.3E1 * t132
      t15 = t13 * t12 * t35
      t26 = 0.5E1 / 0.4E1 * t49
      t38 = t38 / 0.3E1
      t49 = -t3 * t15 * t5 - t57 * t53 * (t133 * t7 - t8 * t133) + t103 
     #- t26 + t38
      t143 = t53 * t8
      t44 = t101 * (t132 / 0.3E1 + t44)
      t101 = 0.5E1 / 0.4E1 * t111
      t15 = -t57 * t143 * t133 + t4 * (-t1 * t49 + t4 * (t15 * t3 * t4 +
     # t44) - t101) - t106
      t98 = (t2 * t11 * (t98 - t50) + 0.2E1 * t3 * t66 * t96) * t6
      t95 = (0.2E1 * t3 * t66 * t94 + t2 * t11 * (t95 - t84)) * t25
      t111 = t35 * (t2 * t11 * (t140 - t15) + t3 * t66 * (t137 - t49)) +
     # t98 + t95
      t16 = t6 * (t16 - t47)
      t47 = -t12 * t100 * t6
      t18 = -t3 * t47 * t5 + t57 * t56 * (-t7 * t16 + t16 * t8) - t103 -
     # t18 + t52
      t46 = -t57 * t105 * t16 + t4 * (-t18 * t1 + t4 * (t47 * t3 * t4 - 
     #t46) + t51) + t106
      t27 = t25 * (t27 - t108)
      t47 = -t12 * t128 * t25
      t29 = -t3 * t47 * t5 + t57 * t110 * (-t7 * t27 + t27 * t8) - t103 
     #+ t113 - t29
      t47 = -t57 * t131 * t27 + t4 * (-t1 * t29 + t4 * (t47 * t3 * t4 - 
     #t107) + t112) + t106
      t14 = t35 * (t14 - t45)
      t12 = -t13 * t12 * t35
      t5 = -t3 * t12 * t5 - t57 * t53 * (t14 * t7 - t8 * t14) - t103 + t
     #26 - t38
      t7 = -t57 * t143 * t14 + t4 * (-t1 * t5 + t4 * (t12 * t3 * t4 - t4
     #4) + t101) + t106
      t8 = t25 * (-t2 * t11 * (-t109 + t47) - t3 * t66 * (-t130 + t29)) 
     #+ t35 * (-t2 * t11 * (-t15 + t7) + t3 * t66 * (t49 - t5)) + t6 * (
     #-t2 * t11 * (-t48 + t46) + t3 * t66 * (t104 - t18)) + dt
      t12 = -t57 * t53 * t14 * t9 + t134 - t135 - t136
      t13 = t4 * (-t1 * t12 + t4 * (t55 * (-dz * t14 * t57 + t138) + t36
     #) - t139) - t64
      t5 = t35 * (-t2 * t11 * (-t7 + t13) + t3 * t66 * (t5 - t12)) + t95
     # + t98
      t7 = t35 * (t2 * t11 * (t13 + t20) + t66 * (t12 * t3 + t37)) + t14
     #1
      t12 = t91 * t25
      t13 = t11 * (t1 * (-t3 * t94 + t40) + t2 * t84 - t39) * t25
      t14 = -t57 * t110 * t27 * t9 + t115 - t116 - t117
      t15 = t4 * (-t1 * t14 + t4 * (t55 * (-dy * t27 * t57 + t119) + t11
     #4) - t120) - t64
      t26 = t25 * (-t2 * t11 * (-t47 + t15) + t3 * t66 * (t29 - t14)) + 
     #t75 + t79
      t27 = t41 * t25
      t14 = t25 * (t2 * t11 * (t15 + t28) + t66 * (t14 * t3 + t81)) + t1
     #22
      t15 = t11 * (-t2 * t28 - t82)
      t28 = t71 * t6
      t29 = t11 * (t1 * (-t3 * t76 + t31) + t2 * t78 - t30) * t6
      t30 = t91 * t6
      t31 = t11 * (t1 * (-t3 * t96 + t40) + t2 * t50 - t39) * t6
      t9 = -t57 * t56 * t16 * t9 + t58 - t59 - t60
      t1 = t4 * (-t1 * t9 + t4 * (t55 * (-dx * t16 * t57 + t62) + t54) -
     # t63) - t64
      t4 = t6 * (-t2 * t11 * (-t46 + t1) + t3 * t66 * (t18 - t9)) + t83 
     #+ t87
      t16 = t32 * t6
      t18 = t41 * t6
      t1 = t6 * (t2 * t11 * (t1 + t21) + t66 * (t3 * t9 + t22)) + t67
      t3 = t11 * (-t2 * t21 - t23)
      cuv(25) = t24 * t6
      cuv(67) = t34 + t33
      cuv(73) = t43 + t42
      cuv(74) = t68
      cuv(75) = t43 + t69
      cuv(81) = t34 + t70
      cuv(109) = t73 + t72
      cuv(116) = t90 + t89
      cuv(121) = t93 + t92
      cuv(122) = t99 + t85
      cuv(123) = t61
      cuv(124) = t99 + t65
      cuv(125) = t93 + t80
      cuv(130) = t90 + t17
      cuv(137) = t73 + t19
      cuv(151) = t86 * t25
      cuv(157) = t102 + t88
      cuv(158) = t123
      cuv(159) = t125
      cuv(163) = t127 + t126
      cuv(164) = t97 + t77
      cuv(165) = t118
      cuv(166) = t97 + t10
      cuv(167) = t127 + t74
      cuv(169) = t129 * t35
      cuv(170) = t142
      cuv(171) = t111
      cuv(172) = t8
      cuv(173) = t5
      cuv(174) = t7
      cuv(175) = t11 * (-t2 * t20 - t121) * t35
      cuv(177) = t12 + t126
      cuv(178) = t13 + t77
      cuv(179) = t26
      cuv(180) = t13 + t10
      cuv(181) = t12 + t74
      cuv(185) = t27 + t88
      cuv(186) = t14
      cuv(187) = t27 + t124
      cuv(193) = t15 * t25
      cuv(207) = t72 + t28
      cuv(214) = t89 + t29
      cuv(219) = t92 + t30
      cuv(220) = t85 + t31
      cuv(221) = t4
      cuv(222) = t65 + t31
      cuv(223) = t80 + t30
      cuv(228) = t17 + t29
      cuv(235) = t19 + t28
      cuv(263) = t16 + t33
      cuv(269) = t42 + t18
      cuv(270) = t1
      cuv(271) = t69 + t18
      cuv(277) = t16 + t70
      cuv(319) = t3 * t6
c
      return
      end
c
c++++++++++++++++
c
      subroutine getcvu_fourth3D( 
     *   cc,dx,dy,dz,dt,cvu,beta )
c
      implicit real (t)
      real cvu(1:*)
      real dx,dy,dz,dt,cc,beta
      integer i
c
      do i = 1,7**3
        cvu(i) = 0.0
      end do
c

      t1 = sqrt(0.3E1)
      t2 = 0.1E1 / 0.2E1
      t3 = t1 / 0.6E1
      t4 = t2 + t3
      t3 = t2 - t3
      t5 = 0.1E1 / dx
      t6 = t4 - t3
      t7 = t5 ** 2
      t8 = t7 ** 2
      t9 = t5 * t7
      t10 = cc ** 2
      t11 = t10 ** 2
      t12 = cc * t10
      t13 = t12 * t7
      t14 = t13 * beta
      t15 = t14 * dt
      t16 = t15 * t6
      t17 = t4 ** 2
      t18 = t4 * t17
      t19 = t3 ** 2
      t20 = t3 * t19
      t21 = -t20 + t18
      t22 = beta ** 2
      t23 = beta * t22
      t24 = dt ** 2
      t25 = t23 * dt * t24
      t26 = t25 * cc * t11
      t27 = 0.1E1 / 0.8E1
      t28 = 0.1E1 / 0.12E2
      t29 = t28 * t26 * t8 * t21
      t30 = t16 * t27 - t29
      t31 = t10 * t7
      t32 = t31 * t28 * t22 * t19 * t24
      t15 = t15 * t3
      t33 = t15 * (t27 - t32)
      t34 = t30 * t1
      t34 = dt * (t34 * t3 - t33) - t34 * t2 * dt
      t35 = 0.1E1 / dy
      t36 = t35 ** 2
      t37 = t36 ** 2
      t38 = t35 * t36
      t39 = t12 * t36
      t40 = t39 * beta
      t41 = t40 * dt
      t42 = t41 * t6
      t43 = t26 * t36
      t44 = t43 * t7
      t45 = t44 * t21
      t46 = t45 / 0.6E1
      t47 = t28 * t42
      t48 = -t46 + t47
      t49 = t28 - t31 * t22 * t19 * t24 / 0.6E1
      t41 = t41 * t3
      t50 = t41 * t49
      t51 = t48 * t1
      t52 = 0.1E1 / 0.24E2
      t29 = t16 * t52 - t29
      t32 = t15 * (t52 - t32)
      t53 = t29 * t1
      t53 = dt * (t53 * t3 - t32) - t53 * t2 * dt
      t51 = (dt * (t51 * t3 - t50) - t51 * t2 * dt) * t5
      t54 = t53 * t35
      t55 = t54 + t51
      t56 = 0.1E1 / dz
      t57 = t56 ** 2
      t58 = t57 ** 2
      t59 = t56 * t57
      t60 = t26 * t57 * t7
      t61 = t60 * t21
      t12 = t12 * t57
      t62 = t12 * beta
      t63 = t62 * dt
      t64 = t63 * t6
      t65 = t61 / 0.6E1
      t66 = t28 * t64
      t67 = -t65 + t66
      t63 = t63 * t3
      t49 = t63 * t49
      t68 = t67 * t1
      t53 = t53 * t56
      t68 = (dt * (t68 * t3 - t49) - t68 * t2 * dt) * t5
      t69 = t68 + t53
      t70 = (t36 + t57 + t7) * t10
      t71 = 0.2E1 * t70
      t72 = cc * (t31 + t71)
      t73 = t5 * (t72 - t13)
      t74 = t36 + t57
      t75 = 0.2E1 * t10 * t74 + 0.3E1 * t31
      t76 = t10 * t5
      t77 = t11 * t7
      t74 = 0.2E1 * t77 * t74
      t78 = -t5 * t76 * (t75 + t31) - t74
      t79 = t5 * (t72 + t13)
      t80 = t20 - t18
      t81 = -t20 + t18
      t8 = t11 * t8
      t82 = t8 * t81
      t83 = t25 * cc
      t17 = t19 - t17
      t84 = t11 * t24
      t85 = beta * dt
      t86 = t85 * dx
      t87 = 0.13E2 / 0.24E2 * t16
      t88 = t2 * t84 * t9 * t17
      t89 = -t27 * t86 * t73 * t6 + t28 * t83 * (t78 * t80 + t82) + t52 
     #* t86 * t79 * t6 - t87 + t88
      t90 = cc * t5
      t91 = t2 * t11 * t3 * dt
      t9 = t91 * t9
      t14 = 0.13E2 / 0.24E2 * t14
      t92 = beta * dx
      t93 = t3 * dt
      t94 = t28 * cc * (t25 * (t8 - t78) * t20 + t90) + t93 * (t92 * (-t
     #27 * t73 + t52 * t79) - t14 - t9)
      t95 = t1 * dt
      t96 = t1 * t3
      t97 = -t29
      t29 = (t35 + t56) * (t2 * t95 * (t29 - t97) - dt * (t96 * (t29 - t
     #97) - 0.2E1 * t32))
      t98 = t5 * (t2 * t95 * (t30 - t89) - dt * (t96 * (t30 - t89) - t33
     # + t94)) + t29
      t99 = t95 * t2
      t32 = dt * (-t96 * t97 - t32) + t99 * t97
      t97 = t32 * t56
      t68 = t68 + t97
      t32 = t32 * t35
      t51 = t32 + t51
      t100 = t28 * t26 * t37 * t21
      t101 = t42 * t52 - t100
      t102 = t10 * t36
      t103 = t102 * t28 * t22 * t19 * t24
      t104 = t41 * (t52 - t103)
      t105 = -dt * (-t96 * t101 + t104) - t99 * t101
      t106 = t28 * t16
      t107 = -t46 + t106
      t108 = t28 - t102 * t22 * t19 * t24 / 0.6E1
      t109 = t15 * t108
      t110 = t105 * t5
      t111 = (-dt * (-t96 * t107 + t109) - t99 * t107) * t35
      t112 = t111 + t110
      t43 = t43 * t57
      t113 = t43 * t21
      t114 = t113 * t1
      t115 = t95 * t28
      t116 = t93 * (t43 * t19 - t114) / 0.6E1 + t115 * t113
      t117 = t61 * t1
      t61 = t93 * (t60 * t19 - t117) / 0.6E1 + t115 * t61
      t1 = t45 * t1
      t45 = t93 * (t44 * t19 - t1) / 0.6E1 + t115 * t45
      t115 = t45 * t56
      t118 = t116 * t5
      t119 = t61 * t35
      t120 = t119 + t118 + t115
      t121 = t57 + t7
      t122 = 0.2E1 * t10 * t121 + 0.3E1 * t102
      t123 = t10 * t35
      t121 = 0.2E1 * t11 * t36 * t121
      t124 = -t35 * t123 * (t122 + t102) - t121
      t125 = t84 * t36
      t126 = -0.5E1 / 0.12E2 * t42
      t127 = t2 * t125 * t5 * t17
      t128 = t28 * t83 * t124 * t21
      t129 = t127 + t126 + t46 - t128
      t130 = t93 * t39 * t2
      t131 = t130 * t5
      t132 = t77 * t23 * t19 * t24 / 0.6E1
      t133 = t132 * t36
      t134 = t28 * t23 * t19 * t24
      t135 = t134 * t124
      t136 = -0.5E1 / 0.12E2 * t102 * beta
      t137 = t93 * cc
      t138 = t137 * (t136 - t131 + t133 - t135)
      t77 = t77 * t24
      t139 = -0.5E1 / 0.12E2 * t16
      t140 = t2 * t77 * t35 * t17
      t141 = t28 * t83 * t78 * t21
      t142 = -t141 + t140 + t139 + t46
      t143 = t93 * t13 * t2
      t144 = t143 * t35
      t145 = t134 * t78
      t146 = -0.5E1 / 0.12E2 * t31 * beta
      t147 = t137 * (t146 - t144 + t133 - t145)
      t148 = t3 / 0.3E1 - 0.1E1 / 0.6E1
      t1 = dt * (t1 * t148 - t44 * t20 / 0.3E1) * t56
      t44 = t35 * (t2 * t95 * (-t142 + t107) + dt * (t96 * (-t107 + t142
     #) + t109 - t147)) + t5 * (t2 * t95 * (t48 - t129) - dt * (t96 * (t
     #48 - t129) - t50 + t138)) + t1
      t45 = t45 * t56
      t149 = t119 + t118 + t45
      t26 = t28 * t26 * t58 * t21
      t150 = t52 * t64 - t26
      t151 = t10 * t57
      t152 = t151 * t28 * t22 * t19 * t24
      t153 = t63 * (t52 - t152)
      t154 = dt * (t96 * t150 - t153) - t99 * t150
      t106 = -t65 + t106
      t22 = t28 - t151 * t22 * t19 * t24 / 0.6E1
      t15 = t15 * t22
      t155 = (dt * (t96 * t106 - t15) - t99 * t106) * t56
      t156 = t154 * t5
      t157 = t156 + t155
      t7 = t36 + t7
      t36 = 0.2E1 * t10 * t7 + 0.3E1 * t151
      t10 = t10 * t56
      t7 = 0.2E1 * t11 * t57 * t7
      t158 = -t56 * t10 * (t36 + t151) - t7
      t159 = t84 * t57
      t160 = -0.5E1 / 0.12E2 * t64
      t161 = t2 * t159 * t5 * t17
      t162 = t28 * t83 * t158 * t21
      t163 = t161 + t160 + t65 - t162
      t164 = t93 * t12 * t2
      t165 = t164 * t5
      t132 = t132 * t57
      t166 = t134 * t158
      t167 = -0.5E1 / 0.12E2 * t151 * beta
      t168 = t137 * (t167 - t165 + t132 - t166)
      t77 = t2 * t77 * t56 * t17
      t141 = -t141 + t77 + t139 + t65
      t143 = t143 * t56
      t145 = t137 * (t146 - t143 + t132 - t145)
      t60 = dt * (t117 * t148 - t60 * t20 / 0.3E1) * t35
      t117 = t5 * (-t2 * t95 * (-t67 + t163) + dt * (t96 * (-t67 + t163)
     # + t49 - t168)) + t56 * (t2 * t95 * (t106 - t141) - dt * (t96 * (t
     #106 - t141) - t15 + t145)) + t60
      t169 = -t75
      t170 = -0.2E1 * t35 * t123 * t122 + t5 * t76 * (t169 - t75) - 0.2E
     #1 * t56 * t10 * t36
      t171 = 0.2E1 * t5 * t72
      t172 = t76 * t24
      t173 = -0.5E1 / 0.4E1 * t85 * cc * t70 * t6
      t16 = 0.2E1 / 0.3E1 * t16
      t174 = -t2 * t172 * t75 * t17 - t27 * t86 * (-t3 * t73 + t4 * t73)
     # + t28 * t83 * (t18 * (t78 + t170) + t20 * (-t78 - t170)) - t52 * 
     #t86 * (t3 * (-t79 - t171) + t4 * (t79 + t171)) - t173 + t16
      t175 = t85 * t3
      t70 = t175 * t70
      t176 = t86 * t3
      t177 = t83 * t20
      t178 = 0.2E1 / 0.3E1 * t90 * beta
      t179 = t93 * t2
      t180 = 0.5E1 / 0.4E1 * cc * (t70 - t90)
      t181 = t93 * t76
      t73 = -t27 * t176 * t73 + t28 * t177 * (t78 + t170) - t52 * t176 *
     # (t79 + t171) + t180 + t181 * (t179 * t75 + t178)
      t75 = -t142
      t78 = -t141
      t79 = t35 * (-t2 * t95 * (t75 - t142) - dt * (t96 * (t142 - t75) -
     # 0.2E1 * t147)) + t5 * (-t2 * t95 * (-t89 + t174) + dt * (t96 * (-
     #t89 + t174) + t94 - t73)) + t56 * (-t2 * t95 * (t78 - t141) - dt *
     # (t96 * (t141 - t78) - 0.2E1 * t145))
      t7 = -t56 * t10 * (t36 + t151) - t7
      t89 = t28 * t83 * t7 * t21
      t94 = -t89 + t161 + t160 + t65
      t141 = t134 * t7
      t142 = t137 * (t167 - t165 + t132 - t141)
      t161 = -t15
      t78 = t5 * (-t2 * t95 * (-t67 + t94) + dt * (t96 * (-t67 + t94) + 
     #t49 - t142)) + t56 * (t2 * t95 * (t78 + t106) - dt * (t96 * (t78 +
     # t106) + t145 + t161)) + t60
      t145 = (dt * (t96 * t106 + t161) - t99 * t106) * t56
      t61 = t61 * t35
      t121 = -t35 * t123 * (t122 + t102) - t121
      t165 = t28 * t83 * t121 * t21
      t127 = -t165 + t127 + t126 + t46
      t182 = t134 * t121
      t131 = t137 * (t136 - t131 + t133 - t182)
      t75 = t35 * (t2 * t95 * (t75 + t107) - dt * (t96 * (t75 + t107) + 
     #t147 - t109)) + t5 * (t2 * t95 * (t48 - t127) - dt * (t96 * (t48 -
     # t127) - t50 + t131)) + t1
      t147 = (dt * (t96 * t107 - t109) - t99 * t107) * t35
      t110 = t147 + t110
      t100 = t27 * t42 - t100
      t103 = t41 * (t27 - t103)
      t183 = -dt * (-t96 * t100 + t103) - t99 * t100
      t113 = t113 / 0.6E1
      t66 = -t113 + t66
      t108 = t63 * t108
      t184 = (-t99 * t66 - dt * (-t96 * t66 + t108)) * t35
      t105 = t105 * t56
      t185 = -t101
      t102 = cc * (t102 + t71)
      t186 = t35 * (t102 + t39)
      t187 = t35 * (t102 - t39)
      t37 = t11 * t37
      t188 = t37 * t81
      t189 = t85 * dy
      t190 = 0.13E2 / 0.24E2 * t42
      t191 = t2 * t84 * t38 * t17
      t192 = -t27 * t189 * t187 * t6 + t28 * t83 * (t124 * t80 + t188) +
     # t52 * t189 * t186 * t6 - t190 + t191
      t193 = cc * t35
      t38 = t91 * t38
      t40 = 0.13E2 / 0.24E2 * t40
      t194 = beta * dy
      t195 = t28 * cc * (t25 * (t37 - t124) * t20 + t193) + t93 * (t194 
     #* (t186 * t52 - t187 * t27) - t38 - t40)
      t101 = (t5 + t56) * (t2 * t95 * (t101 - t185) - dt * (t96 * (t101 
     #- t185) - 0.2E1 * t104))
      t196 = t35 * (t2 * t95 * (t100 - t192) - dt * (t96 * (t100 - t192)
     # - t103 + t195)) + t101
      t104 = t99 * t185 + dt * (-t96 * t185 - t104)
      t185 = t104 * t56
      t47 = -t113 + t47
      t22 = t41 * t22
      t41 = t154 * t35
      t154 = (-t99 * t47 + dt * (t96 * t47 - t22)) * t56
      t159 = t2 * t159 * t35 * t17
      t162 = t159 + t113 + t160 - t162
      t164 = t164 * t35
      t19 = t125 * t23 * t19 * t57 / 0.6E1
      t23 = t137 * (t167 - t164 + t19 - t166)
      t57 = t2 * t125 * t56 * t17
      t125 = t57 + t113 + t126 - t128
      t128 = t130 * t56
      t130 = t137 * (t136 - t128 + t19 - t135)
      t43 = dt * (t114 * t148 - t43 * t20 / 0.3E1) * t5
      t114 = t35 * (-t2 * t95 * (-t66 + t162) + dt * (t96 * (-t66 + t162
     #) + t108 - t23)) + t56 * (t2 * t95 * (t47 - t125) - dt * (t96 * (t
     #47 - t125) - t22 + t130)) + t43
      t135 = -t129
      t148 = 0.2E1 * t35 * t102
      t166 = t123 * t24
      t42 = 0.2E1 / 0.3E1 * t42
      t197 = -t2 * t166 * t122 * t17 + t27 * t189 * (t187 * t3 - t4 * t1
     #87) + t28 * t83 * (t18 * (t124 + t170) + t20 * (-t124 - t170)) + t
     #52 * t189 * (t3 * (t186 + t148) + t4 * (-t186 - t148)) - t173 + t4
     #2
      t198 = t175 * dy
      t199 = 0.2E1 / 0.3E1 * t193 * beta
      t200 = 0.5E1 / 0.4E1 * cc * (t70 - t193)
      t123 = t123 * t93
      t124 = -t27 * t198 * t187 + t28 * t177 * (t124 + t170) - t52 * t19
     #8 * (t186 + t148) + t200 + t123 * (t179 * t122 + t199)
      t186 = -t125
      t125 = t35 * (-t2 * t95 * (-t192 + t197) + dt * (t96 * (-t192 + t1
     #97) + t195 - t124)) + t5 * (-t2 * t95 * (-t129 + t135) + dt * (t96
     # * (-t129 + t135) + 0.2E1 * t138)) + t56 * (t2 * t95 * (t125 - t18
     #6) - dt * (t96 * (t125 - t186) - 0.2E1 * t130))
      t89 = -t89 + t159 + t113 + t160
      t129 = t137 * (t167 - t164 + t19 - t141)
      t141 = -t47
      t130 = t35 * (-t2 * t95 * (-t66 + t89) + dt * (t96 * (-t66 + t89) 
     #+ t108 - t129)) + t56 * (t2 * t95 * (t186 - t141) - dt * (t96 * (t
     #186 - t141) + t130 - t22)) + t43
      t159 = (t99 * t141 - dt * (t96 * t141 + t22)) * t56
      t26 = t27 * t64 - t26
      t63 = t63 * (t27 - t152)
      t152 = -t99 * t26 + dt * (t96 * t26 - t63)
      t160 = -t150
      t71 = cc * (t151 + t71)
      t151 = t56 * (t71 - t12)
      t164 = t56 * (t71 + t12)
      t85 = t85 * dz
      t11 = t11 * t58
      t58 = 0.13E2 / 0.24E2 * t64
      t84 = t2 * t84 * t59 * t17
      t167 = -t27 * t85 * t151 * t6 + t52 * t85 * t164 * t6 + t28 * t83 
     #* (t11 * t81 + t158 * t80) - t58 + t84
      t186 = cc * t56
      t59 = t91 * t59
      t62 = 0.13E2 / 0.24E2 * t62
      t91 = beta * dz
      t187 = t28 * cc * (t25 * (t11 - t158) * t20 + t186) + t93 * (t91 *
     # (-t151 * t27 + t164 * t52) - t62 - t59)
      t150 = (t35 + t5) * (t2 * t95 * (t150 - t160) - dt * (t96 * (t150 
     #- t160) - 0.2E1 * t153))
      t192 = t56 * (-t2 * t95 * (-t26 + t167) + dt * (t96 * (-t26 + t167
     #) + t63 - t187)) + t150
      t195 = -t163
      t201 = -0.2E1 * t56 * t71
      t24 = t10 * t24
      t64 = 0.2E1 / 0.3E1 * t64
      t202 = -t2 * t24 * t36 * t17 - t27 * t85 * (-t3 * t151 + t151 * t4
     #) + t28 * t83 * (t18 * (t158 + t170) + t20 * (-t158 - t170)) - t52
     # * t85 * (t3 * (-t164 + t201) + t4 * (t164 - t201)) - t173 + t64
      t175 = t175 * dz
      t203 = 0.2E1 / 0.3E1 * t186 * beta
      t70 = 0.5E1 / 0.4E1 * cc * (t70 - t186)
      t10 = t10 * t93
      t151 = -t27 * t175 * t151 + t28 * t177 * (t158 + t170) + t52 * t17
     #5 * (-t164 + t201) + t70 + t10 * (t179 * t36 + t203)
      t158 = t35 * (0.2E1 * t2 * t95 * t162 + dt * (-0.2E1 * t96 * t162 
     #+ 0.2E1 * t23)) + t5 * (-t2 * t95 * (t195 - t163) - dt * (t96 * (t
     #163 - t195) - 0.2E1 * t168)) + t56 * (t2 * t95 * (t167 - t202) - d
     #t * (t96 * (t167 - t202) - t187 + t151))
      t163 = t5 * (-t72 + t13)
      t31 = t5 * t76 * (t169 - t31) - t74
      t13 = t5 * (-t72 - t13)
      t16 = -t2 * t172 * t169 * t17 - t27 * t86 * (-t3 * t163 + t163 * t
     #4) - t28 * t83 * (t18 * (t170 + t31) + t20 * (-t170 - t31)) + t52 
     #* t86 * (t3 * (-t171 + t13) + t4 * (t171 - t13)) - t16 + t173
      t72 = -t27 * t176 * t163 - t28 * t177 * (t170 + t31) + t52 * t176 
     #* (t171 - t13) - t180 + t181 * (t179 * t169 - t178)
      t74 = t35 * (t102 + t39)
      t39 = t35 * (-t102 + t39)
      t42 = t2 * t166 * t122 * t17 + t27 * t189 * (t3 * t39 - t4 * t39) 
     #- t28 * t83 * (t18 * (t170 + t121) + t20 * (-t170 - t121)) - t52 *
     # t189 * (t3 * (t148 + t74) + t4 * (-t148 - t74)) + t173 - t42
      t76 = -t27 * t198 * t39 - t28 * t177 * (t170 + t121) + t52 * t198 
     #* (t148 + t74) - t200 + t123 * (-t179 * t122 - t199)
      t102 = t56 * (-t71 + t12)
      t12 = t56 * (-t71 - t12)
      t3 = t2 * t24 * t36 * t17 - t27 * t85 * (-t3 * t102 + t102 * t4) -
     # t28 * t83 * (t18 * (t170 + t7) + t20 * (-t170 - t7)) - t52 * t85 
     #* (t3 * (-t201 - t12) + t4 * (t201 + t12)) + t173 - t64
      t4 = -t27 * t175 * t102 - t28 * t177 * (t170 + t7) - t52 * t175 * 
     #(t201 + t12) - t70 + t10 * (-t179 * t36 - t203)
      t10 = t35 * (-t2 * t95 * (t42 - t197) - dt * (t96 * (t197 - t42) -
     # t124 + t76)) + t5 * (t2 * t95 * (t174 - t16) - dt * (t96 * (t174 
     #- t16) - t73 + t72)) + t56 * (t2 * t95 * (t202 - t3) - dt * (t96 *
     # (t202 - t3) - t151 + t4))
      t17 = -t94
      t18 = -t89
      t24 = -t27 * t85 * t102 * t6 + t52 * t85 * t12 * t6 + t28 * t83 * 
     #(t11 * t80 + t7 * t81) + t58 - t84
      t7 = t28 * cc * (t25 * (-t11 + t7) * t20 - t186) + t93 * (t91 * (-
     #t102 * t27 + t12 * t52) + t62 + t59)
      t3 = t35 * (-t2 * t95 * (-t89 + t18) + dt * (t96 * (-t89 + t18) + 
     #0.2E1 * t129)) + t5 * (t2 * t95 * (t94 - t17) - dt * (t96 * (t94 -
     # t17) - 0.2E1 * t142)) + t56 * (t2 * t95 * (-t24 + t3) + dt * (t96
     # * (-t3 + t24) + t4 - t7))
      t4 = t56 * (t2 * t95 * (t24 + t26) - dt * (t96 * (t24 + t26) - t7 
     #- t63)) + t150
      t7 = t99 * t160 + dt * (-t96 * t160 - t153)
      t11 = t7 * t35
      t12 = -t165 + t57 + t113 + t126
      t19 = t137 * (t136 - t128 + t19 - t182)
      t23 = t35 * (t2 * t95 * (t66 - t162) + dt * (t96 * (-t66 + t162) +
     # t108 - t23)) + t56 * (t2 * t95 * (-t12 + t47) + dt * (t96 * (-t47
     # + t12) + t22 - t19)) + t43
      t24 = -t27 * t189 * t39 * t6 - t28 * t83 * (t121 * t80 + t188) - t
     #52 * t189 * t74 * t6 + t190 - t191
      t36 = -t28 * cc * (t25 * (t37 - t121) * t20 + t193) + t93 * (-t194
     # * (t27 * t39 + t52 * t74) + t38 + t40)
      t37 = t35 * (t2 * t95 * (-t24 + t42) + dt * (t96 * (-t42 + t24) + 
     #t76 - t36)) + t5 * (0.2E1 * t2 * t95 * t127 + dt * (-0.2E1 * t96 *
     # t127 + 0.2E1 * t131)) + t56 * (0.2E1 * t2 * t95 * t12 - dt * (0.2
     #E1 * t96 * t12 - 0.2E1 * t19))
      t12 = t35 * (t2 * t95 * (t18 + t66) - dt * (t96 * (t18 + t66) + t1
     #29 - t108)) + t56 * (-t2 * t95 * (t141 + t12) + dt * (t96 * (t141 
     #+ t12) + t22 - t19)) + t43
      t18 = (-t99 * t66 - dt * (-t96 * t66 + t108)) * t35
      t19 = -t100
      t22 = t35 * (t2 * t95 * (-t19 + t24) + dt * (t96 * (-t24 + t19) + 
     #t36 + t103)) + t101
      t19 = t99 * t19 + dt * (-t96 * t19 - t103)
      t24 = t104 * t5
      t36 = t116 * t5
      t38 = -t50
      t21 = t28 * t83 * t31 * t21
      t39 = -t21 + t140 + t139 + t46
      t40 = t134 * t31
      t42 = t137 * (t146 - t144 + t133 - t40)
      t43 = t35 * (-t2 * t95 * (t39 - t107) - dt * (t96 * (t107 - t39) -
     # t109 + t42)) + t5 * (t2 * t95 * (t135 + t48) - dt * (t96 * (t135 
     #+ t48) + t138 + t38)) + t1
      t7 = t5 * t7
      t46 = -t49
      t21 = -t21 + t77 + t139 + t65
      t40 = t137 * (t146 - t143 + t132 - t40)
      t15 = t5 * (t2 * t95 * (t195 + t67) - dt * (t96 * (t195 + t67) + t
     #168 + t46)) + t56 * (t2 * t95 * (t106 - t21) - dt * (t96 * (t106 -
     # t21) - t15 + t40)) + t60
      t6 = -t27 * t86 * t163 * t6 - t28 * t83 * (t31 * t80 + t82) + t52 
     #* t86 * t13 * t6 + t87 - t88
      t8 = -t28 * cc * (t25 * (t8 - t31) * t20 + t90) + t93 * (t92 * (t1
     #3 * t52 - t163 * t27) + t14 + t9)
      t9 = t35 * (0.2E1 * t2 * t95 * t39 + dt * (-0.2E1 * t96 * t39 + 0.
     #2E1 * t42)) + t5 * (t2 * t95 * (t16 - t6) - dt * (t96 * (t16 - t6)
     # - t72 + t8)) + t56 * (0.2E1 * t2 * t95 * t21 + dt * (-0.2E1 * t96
     # * t21 + 0.2E1 * t40))
      t13 = t5 * (t2 * t95 * (t17 + t67) - dt * (t96 * (t17 + t67) + t14
     #2 + t46)) + t56 * (-t2 * t95 * (t21 - t106) - dt * (t96 * (t106 - 
     #t21) + t161 + t40)) + t60
      t1 = t35 * (t2 * t95 * (t107 - t39) + dt * (t96 * (t39 - t107) + t
     #109 - t42)) + t5 * (-t2 * t95 * (-t48 + t127) - dt * (t96 * (t48 -
     # t127) + t131 + t38)) + t1
      t14 = (-t99 * t48 + dt * (t96 * t48 + t38)) * t5
      t16 = (-t99 * t67 + dt * (t96 * t67 + t46)) * t5
      t17 = -t30
      t2 = t5 * (-t2 * t95 * (-t6 + t17) + dt * (t96 * (-t6 + t17) + t8 
     #+ t33)) + t29
      t6 = t99 * t17 - dt * (t96 * t17 + t33)
      cvu(25) = t34 * t5
      cvu(67) = t55
      cvu(73) = t69
      cvu(74) = t98
      cvu(75) = t68
      cvu(81) = t51
      cvu(109) = t112
      cvu(115) = t120
      cvu(116) = t44
      cvu(117) = t149
      cvu(121) = t157
      cvu(122) = t117
      cvu(123) = t79
      cvu(124) = t78
      cvu(125) = t156 + t145
      cvu(129) = t61 + t118 + t115
      cvu(130) = t75
      cvu(131) = t61 + t118 + t45
      cvu(137) = t110
      cvu(151) = t183 * t35
      cvu(157) = t184 + t105
      cvu(158) = t196
      cvu(159) = t184 + t185
      cvu(163) = t41 + t154
      cvu(164) = t114
      cvu(165) = t125
      cvu(166) = t130
      cvu(167) = t41 + t159
      cvu(169) = t152 * t56
      cvu(170) = t192
      cvu(171) = t158
      cvu(172) = t10
      cvu(173) = t3
      cvu(174) = t4
      cvu(175) = (-t99 * t26 - dt * (-t96 * t26 + t63)) * t56
      cvu(177) = t11 + t154
      cvu(178) = t23
      cvu(179) = t37
      cvu(180) = t12
      cvu(181) = t11 + t159
      cvu(185) = t18 + t105
      cvu(186) = t22
      cvu(187) = t18 + t185
      cvu(193) = t19 * t35
      cvu(207) = t111 + t24
      cvu(213) = t119 + t36 + t115
      cvu(214) = t43
      cvu(215) = t119 + t36 + t45
      cvu(219) = t7 + t155
      cvu(220) = t15
      cvu(221) = t9
      cvu(222) = t13
      cvu(223) = t7 + t145
      cvu(227) = t61 + t36 + t115
      cvu(228) = t1
      cvu(229) = t61 + t36 + t45
      cvu(235) = t147 + t24
      cvu(263) = t14 + t54
      cvu(269) = t16 + t53
      cvu(270) = t2
      cvu(271) = t16 + t97
      cvu(277) = t14 + t32
      cvu(319) = t6 * t5
c
      return
      end
c
c++++++++++++++++
c
      subroutine getcvv_fourth3D( 
     *   cc,dx,dy,dz,dt,cvv,beta )
c
      implicit real (t)
      real cvv(1:*)
      real dx,dy,dz,dt,cc,beta
      integer i
c
      do i = 1,7**3
        cvv(i) = 0.0
      end do
c 
      t1 = sqrt(0.3E1)
      t2 = 0.1E1 / 0.2E1
      t3 = 0.1E1 / 0.6E1
      t4 = t3 * t1
      t5 = t4 + t2
      t4 = -t4 + t2
      t6 = 0.1E1 / dx
      t7 = t4 ** 2
      t8 = t4 * t7
      t9 = t5 ** 2
      t10 = t9 - t7
      t11 = beta ** 2
      t12 = t6 ** 2
      t13 = dt ** 2
      t14 = dt * t13
      t15 = cc ** 2
      t16 = cc * t15
      t17 = t16 * t12
      t18 = t17 * t11 * t13
      t19 = t18 * t10
      t20 = t15 * t12
      t21 = cc * (-0.5E1 / 0.288E3 + t20 * t11 * t7 * t13 / 0.12E2)
      t22 = t4 / 0.12E2
      t23 = t22 - 0.1E1 / 0.24E2
      t24 = t19 * t1
      t25 = t24 * t23
      t26 = dt * (t25 - t21)
      t27 = 0.1E1 / dy
      t28 = t27 ** 2
      t29 = t16 * t28
      t30 = t29 * t11 * t13
      t31 = t30 * t10
      t32 = t31 * t1
      t33 = t4 * dt
      t34 = 0.1E1 / 0.16E2
      t35 = t34 * t33 * (t30 * t4 - t32)
      t36 = t31 / 0.32E2
      t37 = -t36 * t1 * dt - t35
      t38 = t33 * (t18 * t4 - t24)
      t39 = -t38 / 0.48E2 - t24 * dt / 0.96E2
      t40 = t37 * t6
      t41 = t39 * t27
      t42 = 0.1E1 / dz
      t43 = t42 ** 2
      t16 = t16 * t43
      t44 = t16 * t11 * t13
      t45 = t44 * t10
      t46 = t45 * t1
      t47 = t34 * t33 * (-t44 * t4 + t46)
      t48 = t45 / 0.32E2
      t49 = -t48 * t1 * dt + t47
      t50 = t39 * t42
      t51 = t49 * t6
      t52 = (t28 + t43 + t12) * t15
      t53 = 0.2E1 * t52
      t54 = cc * (t53 + t20)
      t55 = t6 * (t17 - t54)
      t56 = t11 * t13
      t57 = t56 * dx
      t58 = t5 - t4
      t59 = dt * t6 * t15 * t58
      t5 = -t5 * t9 + t8
      t60 = t15 ** 2 * t14
      t61 = t60 * t6 * t12
      t62 = 0.7E1 / 0.24E2 * t19
      t63 = t3 * t61 * t5
      t64 = t59 / 0.12E2
      t65 = t34 * t57 * t55 * t10 - t62 + t63 + t64
      t66 = 0.7E1 / 0.24E2 * t17
      t67 = t33 / 0.12E2
      t68 = t67 * t6 * t15
      t61 = t61 * t3 * t8
      t69 = 0.25E2 / 0.288E3 * cc
      t70 = t69 - t61 + t68 + t56 * (dx * t55 * t34 - t66) * t7
      t71 = t1 * t4
      t22 = -t22 + 0.1E1 / 0.24E2
      t72 = t22 * t19
      t1 = t1 * dt
      t4 = -t4 / 0.24E2 + 0.1E1 / 0.48E2
      t18 = (t42 + t27) * dt * (t24 * t4 + t18 * t7 / 0.24E2)
      t24 = t6 * ((t71 * t65 + t21 - t70) * dt + t1 * (-t2 * t65 + t72))
     # + t18
      t73 = t39 * t42
      t74 = t39 * t27
      t75 = t37 / 0.3E1
      t39 = 0.3E1 * t39
      t76 = t75 * t6
      t77 = t39 * t27
      t78 = t60 * t28
      t79 = -0.13E2 / 0.48E2 * t31
      t80 = t3 * t78 * t6 * t5 + t79
      t81 = -0.13E2 / 0.48E2 * t11
      t82 = t33 * t3 * cc
      t83 = -t82 * t6 + t81
      t84 = t29 * t7 * t13
      t85 = t84 * t83
      t86 = t60 * t12
      t87 = -0.13E2 / 0.48E2 * t19
      t88 = t3 * t86 * t27 * t5 + t87
      t89 = -t82 * t27 + t81
      t90 = t17 * t7 * t13
      t91 = t90 * t89
      t92 = t19 / 0.32E2
      t38 = t34 * t38
      t93 = (t35 + dt * (t71 * t80 - t85) + t1 * (-t2 * t80 + t36)) * t6
      t94 = (t38 + dt * (t71 * t88 - t91) + t1 * (-t2 * t88 + t92)) * t2
     #7
      t95 = t49 / 0.3E1
      t96 = t39 * t42
      t97 = t95 * t6
      t98 = t60 * t43
      t99 = -0.13E2 / 0.48E2 * t45
      t100 = t3 * t98 * t6 * t5 + t99
      t101 = t16 * t7 * t13
      t83 = t101 * t83
      t86 = t3 * t86 * t42 * t5 + t87
      t81 = -t82 * t42 + t81
      t82 = t90 * t81
      t87 = (t38 + dt * (t71 * t86 - t82) + t1 * (-t2 * t86 + t92)) * t4
     #2
      t90 = (-t47 + dt * (t71 * t100 - t83) + t1 * (-t100 * t2 + t48)) *
     # t6
      t102 = 0.2E1 * t15 * (t28 + t43) + 0.3E1 * t20
      t14 = t15 * t14
      t59 = 0.5E1 / 0.4E1 * t59
      t103 = 0.5E1 / 0.8E1 * t56 * cc * t52 * t10
      t19 = t19 / 0.3E1
      t104 = -t3 * t14 * t102 * t6 * t5 + t34 * t57 * (-t7 * t55 + t55 *
     # t9) - t59 + t103 + t19
      t105 = t57 * t7
      t52 = 0.5E1 / 0.8E1 * t52
      t20 = t20 / 0.3E1
      t106 = t3 * cc * t7 * t13
      t11 = t33 * t11
      t55 = t34 * t105 * t55 + cc * (-0.25E2 / 0.144E3 + t33 * (t11 * (t
     #52 + t20) + t106 * t102 * t6 - 0.5E1 / 0.4E1 * t6 * cc))
      t107 = (0.2E1 * t2 * t1 * t86 - dt * (0.2E1 * t71 * t86 - 0.2E1 * 
     #t82)) * t42
      t108 = (0.2E1 * t2 * t1 * t88 - dt * (0.2E1 * t71 * t88 - 0.2E1 * 
     #t91)) * t27
      t65 = t6 * (-t2 * t1 * (-t65 + t104) - dt * (t71 * (t65 - t104) - 
     #t70 + t55)) + t107 + t108
      t70 = (t38 - dt * (-t71 * t86 + t82) + t1 * (-t2 * t86 + t92)) * t
     #42
      t82 = t39 * t42
      t38 = (t38 - dt * (-t71 * t88 + t91) + t1 * (-t2 * t88 + t92)) * t
     #27
      t39 = t39 * t27
      t86 = t39 + t76
      t88 = t15 * t28
      t91 = cc * (-0.5E1 / 0.288E3 + t88 * t56 * t7 / 0.12E2)
      t92 = t32 * t23
      t109 = dt * (t92 - t91)
      t110 = t75 * t42
      t111 = t49 * t27
      t112 = cc * (t53 + t88)
      t113 = t27 * (t29 - t112)
      t114 = t56 * dy
      t115 = dt * t15
      t116 = t115 * t27 * t58
      t117 = t60 * t27 * t28
      t118 = 0.7E1 / 0.24E2 * t31
      t119 = t3 * t117 * t5
      t120 = t116 / 0.12E2
      t121 = t34 * t114 * t113 * t10 - t118 + t119 + t120
      t122 = 0.7E1 / 0.24E2 * t29
      t117 = t117 * t3 * t8
      t67 = t67 * t15
      t123 = t67 * t27
      t124 = t69 + t123 - t117 + t56 * (dy * t113 * t34 - t122) * t7
      t125 = t22 * t31
      t30 = (t42 + t6) * dt * (t32 * t4 + t30 * t7 / 0.24E2)
      t32 = t27 * ((t71 * t121 - t124 + t91) * dt + t1 * (-t121 * t2 + t
     #125)) + t30
      t126 = t75 * t42
      t127 = t37 * t42
      t128 = t95 * t27
      t98 = t3 * t98 * t27 * t5 + t99
      t89 = t101 * t89
      t78 = t3 * t78 * t42 * t5 + t79
      t79 = t84 * t81
      t81 = (t35 + dt * (t71 * t78 - t79) + t1 * (-t2 * t78 + t36)) * t4
     #2
      t84 = (-t47 + dt * (t71 * t98 - t89) + t1 * (-t2 * t98 + t48)) * t
     #27
      t99 = 0.2E1 * t15 * (t43 + t12) + 0.3E1 * t88
      t101 = -t9 + t7
      t101 = 0.5E1 / 0.4E1 * t116
      t31 = t31 / 0.3E1
      t116 = -t3 * t14 * t99 * t27 * t5 + t34 * t114 * t113 * (t9 - t7) 
     #+ t103 - t101 + t31
      t129 = t114 * t7
      t88 = t88 / 0.3E1
      t130 = 0.5E1 / 0.4E1 * cc
      t113 = t34 * t129 * t113 + cc * (-0.25E2 / 0.144E3 + t33 * (t106 *
     # t99 * t27 - t130 * t27 + t11 * (t52 + t88)))
      t131 = (0.2E1 * t2 * t1 * t78 - dt * (0.2E1 * t71 * t78 - 0.2E1 * 
     #t79)) * t42
      t132 = (0.2E1 * t2 * t1 * t80 - dt * (0.2E1 * t71 * t80 - 0.2E1 * 
     #t85)) * t6
      t121 = t27 * (-t2 * t1 * (-t121 + t116) - dt * (t71 * (t121 - t116
     #) - t124 + t113)) + t131 + t132
      t78 = (t35 - dt * (-t71 * t78 + t79) + t1 * (-t2 * t78 + t36)) * t
     #42
      t79 = t37 * t42
      t124 = t128 + t79
      t133 = t15 * t43
      t134 = cc * (-0.5E1 / 0.288E3 + t133 * t56 * t7 / 0.12E2)
      t23 = t46 * t23
      t135 = dt * (t23 - t134)
      t53 = cc * (t53 + t133)
      t136 = t42 * (-t16 + t53)
      t137 = t56 * dz
      t58 = t115 * t42 * t58
      t43 = t60 * t42 * t43
      t60 = 0.7E1 / 0.24E2 * t45
      t115 = t3 * t43 * t5
      t138 = t58 / 0.12E2
      t139 = -t34 * t137 * t136 * t10 + t115 + t138 - t60
      t140 = 0.7E1 / 0.24E2 * t16
      t67 = t67 * t42
      t8 = t43 * t3 * t8
      t43 = t69 - t8 + t67 - t56 * (dz * t136 * t34 + t140) * t7
      t22 = t22 * t45
      t4 = (t6 + t27) * dt * (t46 * t4 + t44 * t7 / 0.24E2)
      t44 = t42 * ((t71 * t139 + t134 - t43) * dt + t1 * (-t139 * t2 + t
     #22)) + t4
      t12 = 0.2E1 * t15 * (t28 + t12) + 0.3E1 * t133
      t15 = 0.5E1 / 0.4E1 * t58
      t28 = t45 / 0.3E1
      t45 = -t3 * t14 * t12 * t42 * t5 - t34 * t137 * (-t7 * t136 + t136
     # * t9) + t103 - t15 + t28
      t46 = t137 * t7
      t58 = t133 / 0.3E1
      t106 = -t34 * t46 * t136 + cc * (-0.25E2 / 0.144E3 + t33 * (t106 *
     # t12 * t42 - t130 * t42 + t11 * (t52 + t58)))
      t130 = (0.2E1 * t2 * t1 * t100 + dt * (-0.2E1 * t71 * t100 + 0.2E1
     # * t83)) * t6
      t133 = (0.2E1 * t2 * t1 * t98 + dt * (-0.2E1 * t71 * t98 + 0.2E1 *
     # t89)) * t27
      t43 = t42 * (t2 * t1 * (t139 - t45) - dt * (t71 * (t139 - t45) - t
     #43 + t106)) + t130 + t133
      t17 = t6 * (t17 - t54)
      t54 = -t102
      t19 = -t3 * t14 * t54 * t6 * t5 - t34 * t57 * (-t17 * t7 + t17 * t
     #9) - t103 - t19 + t59
      t13 = t7 * t13
      t52 = t52 * t11
      t20 = -t34 * t105 * t17 + cc * (0.25E2 / 0.144E3 + t33 * (-t52 + (
     #0.5E1 / 0.4E1 + t13 * t54 * t3) * t6 * cc - t20 * t11))
      t29 = t27 * (t29 - t112)
      t54 = -t99
      t31 = -t3 * t14 * t54 * t27 * t5 + t34 * t114 * (t29 * t7 - t9 * t
     #29) + t101 - t103 - t31
      t54 = -t34 * t129 * t29 + cc * (0.25E2 / 0.144E3 + t33 * (-t52 - t
     #88 * t11 + (0.5E1 / 0.4E1 + t13 * t54 * t3) * t27 * cc))
      t16 = t42 * (t16 - t53)
      t12 = -t12
      t5 = -t3 * t14 * t12 * t42 * t5 - t34 * t137 * (-t16 * t7 + t16 * 
     #t9) - t103 + t15 - t28
      t3 = -t34 * t46 * t16 + cc * (0.25E2 / 0.144E3 + t33 * (-t52 - t58
     # * t11 + (0.5E1 / 0.4E1 + t13 * t12 * t3) * t42 * cc))
      t9 = t27 * (-t2 * t1 * (-t116 + t31) - dt * (t71 * (t116 - t31) - 
     #t113 + t54)) + t42 * (-t2 * t1 * (-t45 + t5) - dt * (t71 * (t45 - 
     #t5) - t106 + t3)) + t6 * (-t2 * t1 * (-t104 + t19) + dt * (t71 * (
     #-t104 + t19) + t55 - t20)) + 0.1E1
      t11 = -t34 * t137 * t16 * t10 - t115 - t138 + t60
      t8 = -t69 + t8 - t67 + t56 * (-dz * t16 * t34 + t140) * t7
      t3 = t42 * (t2 * t1 * (t5 - t11) - dt * (t71 * (t5 - t11) - t3 + t
     #8)) + t130 + t133
      t5 = -t134
      t4 = t42 * (-(t71 * t11 + t5 - t8) * dt + t1 * (t11 * t2 + t22)) +
     # t4
      t8 = t95 * t27
      t11 = (-t47 - dt * (-t71 * t98 + t89) + t1 * (-t2 * t98 + t48)) * 
     #t27
      t12 = -t34 * t114 * t29 * t10 + t118 - t119 - t120
      t13 = -t69 - t123 + t117 + t56 * (-dy * t29 * t34 + t122) * t7
      t14 = t27 * (-t2 * t1 * (-t31 + t12) - dt * (t71 * (t31 - t12) - t
     #54 + t13)) + t131 + t132
      t15 = t49 * t27
      t16 = -t91
      t12 = t27 * (-(t71 * t12 - t13 + t16) * dt + t1 * (t12 * t2 + t125
     #)) + t30
      t13 = t75 * t6
      t22 = (t35 - dt * (-t71 * t80 + t85) + t1 * (-t2 * t80 + t36)) * t
     #6
      t28 = t95 * t6
      t29 = (-t47 - dt * (-t71 * t100 + t83) + t1 * (-t100 * t2 + t48)) 
     #* t6
      t10 = -t34 * t57 * t17 * t10 + t62 - t63 - t64
      t7 = -t69 + t61 - t68 + t56 * (-dx * t17 * t34 + t66) * t7
      t17 = t6 * (-t2 * t1 * (-t19 + t10) - dt * (t71 * (t19 - t10) - t2
     #0 + t7)) + t107 + t108
      t19 = t37 * t6
      t20 = t49 * t6
      t21 = -t21
      t1 = t6 * (-(t71 * t10 + t21 - t7) * dt + t1 * (t10 * t2 + t72)) +
     # t18
      cvv(25) = t26 * t6
      cvv(67) = t41 + t40
      cvv(73) = t51 + t50
      cvv(74) = t24
      cvv(75) = t51 + t73
      cvv(81) = t74 + t40
      cvv(109) = t77 + t76
      cvv(116) = t94 + t93
      cvv(121) = t97 + t96
      cvv(122) = t90 + t87
      cvv(123) = t65
      cvv(124) = t90 + t70
      cvv(125) = t97 + t82
      cvv(130) = t38 + t93
      cvv(137) = t86
      cvv(151) = t109 * t27
      cvv(157) = t111 + t110
      cvv(158) = t32
      cvv(159) = t111 + t126
      cvv(163) = t128 + t127
      cvv(164) = t84 + t81
      cvv(165) = t121
      cvv(166) = t84 + t78
      cvv(167) = t124
      cvv(169) = t135 * t42
      cvv(170) = t44
      cvv(171) = t43
      cvv(172) = t9
      cvv(173) = t3
      cvv(174) = t4
      cvv(175) = dt * (t23 + t5) * t42
      cvv(177) = t8 + t127
      cvv(178) = t11 + t81
      cvv(179) = t14
      cvv(180) = t11 + t78
      cvv(181) = t8 + t79
      cvv(185) = t15 + t110
      cvv(186) = t12
      cvv(187) = t15 + t126
      cvv(193) = dt * (t92 + t16) * t27
      cvv(207) = t77 + t13
      cvv(214) = t94 + t22
      cvv(219) = t28 + t96
      cvv(220) = t29 + t87
      cvv(221) = t17
      cvv(222) = t29 + t70
      cvv(223) = t28 + t82
      cvv(228) = t38 + t22
      cvv(235) = t39 + t13
      cvv(263) = t41 + t19
      cvv(269) = t20 + t50
      cvv(270) = t1
      cvv(271) = t20 + t73
      cvv(277) = t74 + t19
      cvv(319) = dt * (t25 + t21) * t6
c
      return
      end
