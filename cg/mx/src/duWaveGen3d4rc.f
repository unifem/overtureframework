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
      t5 = t2 + t4
      t6 = t2 - t4
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
      t41 = t28 * t38
      t42 = t41 * t9
      t43 = t42 * t23
      t44 = t14 * t38
      t45 = t44 * beta
      t46 = t45 * dt
      t47 = t46 * t8
      t48 = t30 * t47
      t49 = t3 * t43
      t50 = t48 - t49
      t51 = t33 * t3 * t24 * t19 * t26
      t52 = -t51 + t30
      t53 = t6 * (-t1 * t50 + t46 * t52)
      t54 = 0.1E1 / 0.24E2
      t31 = t18 * t54 - t31
      t55 = t6 * (-t1 * t31 + t17 * (-t34 + t54))
      t56 = -t26 * (t2 * t55 + t4 * t31)
      t57 = -t26 * (t2 * t53 + t4 * t50) * t7
      t58 = t56 * t37
      t59 = t57 + t58
      t60 = 0.1E1 / dz
      t61 = t60 ** 2
      t62 = t61 ** 2
      t63 = t60 * t61
      t64 = t28 * t61 * t9
      t65 = t64 * t23
      t66 = t14 * t61
      t67 = t66 * beta
      t68 = t67 * dt
      t69 = t68 * t8
      t70 = t30 * t69
      t71 = t3 * t65
      t72 = t70 - t71
      t52 = t6 * (-t1 * t72 + t68 * t52)
      t73 = -t26 * (t2 * t52 + t4 * t72) * t7
      t56 = t56 * t60
      t74 = t56 + t73
      t75 = (t9 + t61 + t38) * t12
      t76 = 0.2E1 * t75
      t77 = cc * (t76 + t33)
      t78 = t7 * (t15 - t77)
      t79 = t61 + t38
      t80 = 0.2E1 * t12 * t79 + 0.3E1 * t33
      t81 = t12 * t7
      t82 = t13 * t9
      t79 = 0.2E1 * t82 * t79
      t83 = -t7 * t81 * (t80 + t33) - t79
      t84 = t7 * (t15 + t77)
      t85 = t22 - t20
      t86 = -t22 + t20
      t10 = t13 * t10
      t87 = t27 * cc
      t88 = beta * dt
      t89 = t88 * dx
      t90 = t21 - t19
      t91 = t13 * t26
      t92 = 0.13E2 / 0.24E2
      t93 = t2 * t91 * t11 * t90
      t94 = t92 * t18
      t95 = t29 * t89 * t78 * t8 + t30 * t87 * (t10 * t85 + t83 * t86) +
     # t54 * t89 * t84 * t8 - t93 - t94
      t96 = cc * t7
      t97 = t2 * t13 * t6 * dt
      t11 = t97 * t11
      t16 = t16 * t92
      t98 = beta * dx
      t99 = t30 * cc * (t27 * (t10 - t83) * t20 + t96) + t6 * (dt * (-t1
     #6 + t98 * (t29 * t78 + t54 * t84) - t11) - t1 * t95)
      t100 = t1 * t26
      t101 = t6 * (t1 * t31 + t17 * (t34 - t54))
      t55 = (t37 + t60) * (0.2E1 * t3 * t100 * t31 - t2 * t26 * (-t55 + 
     #t101))
      t35 = t7 * (-t2 * t26 * (-t35 + t99) - t3 * t100 * (-t32 + t95)) +
     # t55
      t31 = t26 * (t101 * t2 - t4 * t31)
      t101 = t31 * t60
      t73 = t101 + t73
      t31 = t31 * t37
      t57 = t31 + t57
      t102 = t30 * t28 * t39 * t23
      t103 = t47 * t54 - t102
      t104 = t12 * t38
      t105 = t104 * t30 * t24 * t19 * t26
      t106 = t6 * (-t1 * t103 + t46 * (-t105 + t54))
      t107 = -t26 * (t4 * t103 + t106 * t2)
      t108 = t30 * t18
      t109 = t108 - t49
      t110 = t104 * t3 * t24 * t19 * t26
      t111 = -t110 + t30
      t112 = t6 * (-t1 * t109 + t17 * t111)
      t113 = t107 * t7
      t114 = -t26 * (t4 * t109 + t112 * t2) * t37
      t115 = t114 + t113
      t41 = t41 * t61
      t116 = t41 * t23
      t41 = t41 * t19
      t117 = t116 * t1
      t117 = t26 * (t30 * t6 * (t41 - t117) + t117 / 0.36E2)
      t64 = t64 * t19
      t65 = t65 * t1
      t65 = t26 * (t30 * t6 * (-t65 + t64) + t65 / 0.36E2)
      t43 = t43 * t1
      t42 = t26 * (t30 * t6 * (t42 * t19 - t43) + t43 / 0.36E2)
      t43 = t117 * t7
      t118 = t42 * t60
      t119 = t65 * t37
      t120 = t118 + t119 + t43
      t121 = t9 + t61
      t122 = 0.2E1 * t12 * t121 + 0.3E1 * t104
      t123 = t12 * t37
      t124 = t13 * t38
      t121 = 0.2E1 * t124 * t121
      t125 = -t37 * t123 * (t122 + t104) - t121
      t126 = t91 * t38
      t127 = t30 * t87 * t125 * t23
      t128 = t2 * t126 * t7 * t90
      t129 = 0.5E1 / 0.12E2 * t47
      t130 = -t128 - t129 - t127 + t49
      t131 = t44 * t2 * t6 * dt
      t132 = t131 * t7
      t133 = 0.5E1 / 0.12E2 * t104 * beta
      t134 = t82 * t3 * t25 * t19 * t26
      t135 = t134 * t38
      t136 = t125 * t30
      t137 = t136 * t25 * t19 * t26
      t138 = dt * cc
      t139 = t6 * (-t1 * t130 + t138 * (-t137 - t132 - t133 + t135))
      t82 = t82 * t26
      t140 = t30 * t87 * t83 * t23
      t141 = -t2 * t82 * t37 * t90
      t142 = 0.5E1 / 0.12E2 * t18
      t143 = t141 - t142 - t140 + t49
      t144 = t15 * t2 * t6 * dt
      t145 = t144 * t37
      t146 = 0.5E1 / 0.12E2 * t33 * beta
      t147 = t30 * t25 * t19 * t26
      t148 = t147 * t83
      t149 = t6 * (-t1 * t143 + t138 * (-t148 - t145 - t146 + t135))
      t150 = -0.2E1 * t42 * t60
      t151 = t37 * (-t2 * t26 * (-t112 + t149) - t3 * t100 * (-t109 + t1
     #43)) + t7 * (-t2 * t26 * (-t53 + t139) - t3 * t100 * (-t50 + t130)
     #) + t150
      t42 = t42 * t60
      t152 = t42 + t119 + t43
      t153 = t54 * t69
      t28 = t30 * t28 * t62 * t23
      t154 = -t28 + t153
      t155 = t12 * t61
      t156 = t155 * t30 * t24 * t19 * t26
      t157 = t6 * (-t1 * t154 + t68 * (-t156 + t54))
      t158 = -t26 * (t4 * t154 + t157 * t2)
      t108 = t108 - t71
      t159 = -t155 * t3 * t24 * t19 * t26 + t30
      t160 = t6 * (-t1 * t108 + t17 * t159)
      t161 = t158 * t7
      t162 = -t26 * (t4 * t108 + t160 * t2) * t60
      t163 = t162 + t161
      t9 = t9 + t38
      t164 = 0.2E1 * t12 * t9 + 0.3E1 * t155
      t165 = t12 * t60
      t9 = 0.2E1 * t13 * t61 * t9
      t166 = -t60 * t165 * (t155 + t164) - t9
      t167 = t91 * t61
      t168 = t30 * t87 * t166 * t23
      t169 = t2 * t167 * t7 * t90
      t170 = 0.5E1 / 0.12E2 * t69
      t171 = -t169 - t170 - t168 + t71
      t134 = t134 * t61
      t172 = t66 * t2 * t6 * dt
      t173 = t172 * t7
      t174 = t147 * t166
      t175 = 0.5E1 / 0.12E2 * t155 * beta
      t176 = t6 * (-t1 * t171 + t138 * (-t175 + t134 - t173 - t174))
      t177 = dy ** 2
      t178 = t177 * t61
      t179 = t54 * t178 * t18
      t180 = t29 * t18 * dy * t60
      t82 = t2 * t82 * t60 * t90
      t181 = -0.7E1 / 0.12E2 * t18
      t140 = t181 + t179 - t140 + t180 - t82 + t71
      t182 = t54 * t60
      t144 = t144 * t60
      t183 = t33 * beta * (dy * t60 * (t182 * dy + t29) - 0.7E1 / 0.12E2
     #)
      t184 = t6 * (-t1 * t140 + t138 * (-t148 - t144 + t183 + t134))
      t185 = -0.2E1 * t65 * t37
      t186 = t60 * (-t2 * t26 * (-t160 + t184) - t3 * t100 * (-t108 + t1
     #40)) + t7 * (-t2 * t26 * (-t52 + t176) - t3 * t100 * (-t72 + t171)
     #) + t185
      t187 = -0.2E1 * t37 * t123 * t122 - 0.2E1 * t60 * t165 * t164 - 0.
     #2E1 * t7 * t81 * t80
      t188 = -0.2E1 * t7 * t77
      t189 = t5 - t6
      t190 = -t5 + t6
      t191 = t81 * t26
      t192 = t88 * cc
      t18 = 0.2E1 / 0.3E1 * t18
      t193 = -0.5E1 / 0.4E1 * t192 * t75 * t8
      t194 = t29 * t89 * t78 * t189 + t2 * t191 * t80 * t90 + t30 * t87 
     #* (t20 * (-t83 - t187) + t22 * (t83 + t187)) + t54 * t89 * (t188 *
     # t189 + t190 * t84) + t18 - t193
      t195 = t89 * t6
      t196 = t88 * t6
      t75 = t196 * t75
      t197 = t87 * t20
      t198 = 0.2E1 / 0.3E1 * t96 * beta
      t199 = t2 * dt * t6
      t200 = t81 * dt
      t201 = 0.5E1 / 0.4E1 * cc * (-t96 + t75)
      t78 = t29 * t195 * t78 + t30 * t197 * (t83 + t187) - t54 * t195 * 
     #(t84 - t188) + t6 * (-t1 * t194 + t200 * (t199 * t80 + t198)) + t2
     #01
      t84 = t6 * (t1 * t143 + t138 * (t148 + t145 + t146 - t135))
      t21 = beta * t189 + t138 * (t21 - t19) * t60
      t148 = t15 * dt
      t202 = t178 * t33
      t203 = t202 * t190
      t24 = t24 * t26
      t83 = t24 * t83
      t204 = t2 * t148 * t21
      t205 = t30 * t192 * (t83 * t85 + t203) - t71 + t204
      t206 = t138 * t6 * t60
      t207 = t206 + beta
      t208 = t192 * t6
      t64 = t64 * t3
      t148 = t2 * t148 * t6 * t207
      t83 = t30 * t208 * (t83 * t19 - t202) + t6 * (-t1 * t205 - t64) + 
     #t148
      t95 = t37 * (0.2E1 * t3 * t100 * t143 - t2 * t26 * (-t149 + t84)) 
     #+ t60 * (-t2 * t26 * (-t184 + t83) - t3 * t100 * (-t140 + t205)) +
     # t7 * (-t2 * t26 * (-t99 + t78) - t3 * t100 * (-t95 + t194))
      t9 = -t60 * t165 * (t155 + t164) - t9
      t99 = t30 * t87 * t9 * t23
      t140 = -t169 - t170 - t99 + t71
      t149 = t147 * t9
      t169 = t6 * (-t1 * t140 + t138 * (-t175 + t134 - t173 - t149))
      t184 = t179 - t180 + t71
      t209 = t29 * dy
      t210 = t6 * (-t1 * t184 + t17 * t60 * (t60 * (t24 * t12 * t19 * t3
     # + t177 * t54) - t209))
      t52 = t60 * (-t2 * t26 * (-t83 + t210) - t3 * t100 * (-t205 + t184
     #)) + t7 * (-t2 * t26 * (-t52 + t169) - t3 * t100 * (-t72 + t140)) 
     #+ t185
      t83 = t26 * (t4 * t184 + t2 * t210) * t60
      t65 = t65 * t37
      t121 = -t37 * t123 * (t122 + t104) - t121
      t205 = t30 * t87 * t121 * t23
      t128 = -t128 - t129 - t205 + t49
      t129 = t147 * t121
      t211 = t6 * (-t1 * t128 + t138 * (-t129 - t132 - t133 + t135))
      t212 = t110 - t30
      t213 = t6 * (t1 * t109 + t17 * t212)
      t53 = t37 * (-t2 * t26 * (-t84 + t213) - t3 * t100 * (-t109 + t143
     #)) + t7 * (-t2 * t26 * (-t53 + t211) - t3 * t100 * (-t50 + t128)) 
     #+ t150
      t84 = t26 * (-t4 * t109 + t2 * t213) * t37
      t102 = t29 * t47 - t102
      t143 = t6 * (-t1 * t102 + t46 * (-t105 + t29))
      t214 = -t26 * (t4 * t102 + t143 * t2)
      t116 = t3 * t116
      t70 = -t116 + t70
      t111 = t6 * (-t1 * t70 + t68 * t111)
      t107 = t107 * t60
      t215 = -t26 * (t111 * t2 + t4 * t70) * t37
      t216 = t6 * (t1 * t103 + t46 * (t105 - t54))
      t104 = cc * (t76 + t104)
      t217 = t37 * (t44 + t104)
      t218 = t37 * (t44 - t104)
      t39 = t13 * t39
      t219 = t39 * t85
      t220 = t88 * dy
      t221 = -t2 * t91 * t40 * t90
      t222 = t92 * t47
      t223 = t29 * t220 * t218 * t8 + t30 * t87 * (t125 * t86 + t219) + 
     #t54 * t220 * t217 * t8 + t221 - t222
      t224 = cc * t37
      t40 = t97 * t40
      t45 = t45 * t92
      t225 = beta * dy
      t226 = t30 * cc * (t27 * (t39 - t125) * t20 + t224) + t6 * (dt * (
     #-t40 - t45 + t225 * (t217 * t54 + t218 * t29)) - t1 * t223)
      t106 = (t60 + t7) * (0.2E1 * t3 * t100 * t103 - t2 * t26 * (-t106 
     #+ t216))
      t143 = t37 * (-t2 * t26 * (-t143 + t226) + t3 * t100 * (t102 - t22
     #3)) + t106
      t103 = t26 * (-t4 * t103 + t2 * t216)
      t216 = t103 * t60
      t48 = t48 - t116
      t159 = t6 * (-t1 * t48 + t46 * t159)
      t227 = -t26 * (t159 * t2 + t4 * t48) * t60
      t158 = t158 * t37
      t228 = t227 + t158
      t167 = -t2 * t167 * t37 * t90
      t168 = t167 + t116 - t170 - t168
      t229 = t126 * t3 * t25 * t19 * t61
      t172 = t172 * t37
      t230 = t6 * (-t1 * t168 + t138 * (-t175 + t229 - t172 - t174))
      t231 = t29 * t88 * t37 * t14 * t60 * t8
      t126 = t2 * t126 * t60 * t90
      t232 = -0.7E1 / 0.12E2 * t47
      t127 = t116 - t127 - t126 + t232 + t231 + t153
      t233 = t29 * t37
      t61 = t124 * t61 * t3
      t124 = t131 * t60
      t12 = t12 * beta * (t60 * (t182 + t233) - 0.7E1 / 0.12E2 * t38)
      t38 = t6 * (-t1 * t127 + t138 * (t25 * (-t136 + t61) * t19 * t26 -
     # t124 + t12))
      t131 = -0.2E1 * t117 * t7
      t136 = t37 * (-t2 * t26 * (-t111 + t230) - t3 * t100 * (-t70 + t16
     #8)) + t60 * (-t2 * t26 * (-t159 + t38) + t3 * t100 * (t48 - t127))
     # + t131
      t137 = t6 * (t1 * t130 + t138 * (t137 + t132 + t133 - t135))
      t234 = -0.2E1 * t37 * t104
      t235 = t123 * t26
      t47 = 0.2E1 / 0.3E1 * t47
      t236 = t2 * t235 * t122 * t90 + t29 * t220 * (t218 * t5 - t6 * t21
     #8) + t30 * t87 * (t20 * (-t125 - t187) + t22 * (t125 + t187)) + t5
     #4 * t220 * (t5 * (-t217 + t234) + t6 * (t217 - t234)) - t193 + t47
      t237 = t196 * dy
      t238 = 0.2E1 / 0.3E1 * t224 * beta
      t123 = t123 * dt
      t239 = 0.5E1 / 0.4E1 * cc * (-t75 + t224)
      t217 = t29 * t237 * t218 + t30 * t197 * (t125 + t187) + t54 * t237
     # * (-t217 + t234) + t6 * (-t1 * t236 + t123 * (t199 * t122 + t238)
     #) - t239
      t218 = t44 * dt
      t240 = t155 * t190
      t241 = t24 * t85
      t21 = t2 * t218 * t21
      t242 = t30 * t192 * (t241 * t125 + t240) - t116 + t21
      t24 = t24 * t19
      t41 = t41 * t3
      t207 = t2 * t6 * t218 * t207
      t125 = t30 * t208 * (t24 * t125 - t155) + t6 * (-t1 * t242 - t41) 
     #+ t207
      t38 = t37 * (-t2 * t26 * (-t226 + t217) - t3 * t100 * (-t223 + t23
     #6)) + t60 * (-t2 * t26 * (-t38 + t125) - t3 * t100 * (-t127 + t242
     #)) + t7 * (0.2E1 * t3 * t100 * t130 - t2 * t26 * (-t139 + t137))
      t99 = t167 + t116 - t170 - t99
      t127 = t6 * (-t1 * t99 + t138 * (-t175 + t229 - t172 - t149))
      t139 = t116 - t231 + t153
      t167 = t88 * t60
      t110 = t6 * (-t1 * t139 + t167 * t14 * (t60 * (t110 + t54) - t233)
     #)
      t111 = t37 * (-t2 * t26 * (-t111 + t127) - t3 * t100 * (-t70 + t99
     #)) + t60 * (-t2 * t26 * (-t125 + t110) - t3 * t100 * (-t242 + t139
     #)) + t131
      t125 = t26 * (t110 * t2 + t4 * t139) * t60
      t170 = t29 * t69 - t28
      t218 = t6 * (-t1 * t170 + t68 * (-t156 + t29))
      t223 = -t26 * (t4 * t170 + t2 * t218)
      t156 = t6 * (t1 * t154 + t68 * (t156 - t54))
      t76 = cc * (t155 + t76)
      t226 = t60 * (t66 - t76)
      t233 = t60 * (t66 + t76)
      t242 = dz * t233
      t243 = dz * t226
      t13 = t13 * t62
      t244 = -0.17E2 / 0.24E2
      t91 = t2 * t91 * t63 * t90
      t245 = t29 * t88 * (t189 * t63 * dy * t14 + t243 * t189) + t30 * t
     #87 * (t13 * t85 + t166 * t86) + t54 * t88 * (t189 * t62 * t177 * t
     #14 + t242 * t189) + t69 * t244 - t91
      t246 = cc * t60
      t62 = t29 * t196 * (dy * t14 * t63 + t243) + t30 * cc * (t27 * (t1
     #3 - t166) * t20 + t246) + t54 * t196 * (t177 * t14 * t62 + t242) +
     # t6 * (-t1 * t245 + t66 * dt * (beta * t244 - t206 * t2))
      t157 = (t37 + t7) * (0.2E1 * t3 * t100 * t154 - t2 * t26 * (-t157 
     #+ t156))
      t170 = t60 * (-t2 * t26 * (-t218 + t62) - t3 * t100 * (-t170 + t24
     #5)) + t157
      t206 = t6 * (t1 * t171 + t138 * (t175 - t134 + t173 + t174))
      t174 = t6 * (t1 * t168 + t138 * (t175 - t229 + t172 + t174))
      t218 = -0.2E1 * t60 * t76
      t242 = dz * t218
      t243 = t189 * t60
      t226 = dy * t226
      t244 = t165 * t26
      t247 = 0.2E1 / 0.3E1 * t69
      t248 = t2 * t244 * t164 * t90 + t29 * t88 * t226 * t189 + t30 * t8
     #7 * (t20 * (-t166 - t187) + t22 * (t166 + t187)) - t54 * t88 * (t2
     #43 * t233 * t177 + t242 * t190) - t193 + t247
      t249 = 0.2E1 / 0.3E1 * t246 * beta
      t250 = t165 * dt
      t75 = 0.5E1 / 0.4E1 * cc * (t75 - t246)
      t166 = t29 * t196 * t226 + t30 * t197 * (t166 + t187) + t54 * t196
     # * (-t233 * t177 * t60 + t242) + t6 * (-t1 * t248 + t250 * (t199 *
     # t164 + t249)) + t75
      t62 = t37 * (0.2E1 * t3 * t100 * t168 - t2 * t26 * (-t230 + t174))
     # + t60 * (-t2 * t26 * (-t62 + t166) - t3 * t100 * (-t245 + t248)) 
     #+ t7 * (0.2E1 * t3 * t100 * t171 - t2 * t26 * (-t176 + t206))
      t176 = t7 * (t15 - t77)
      t33 = -t7 * t81 * (t80 + t33) - t79
      t15 = t7 * (t15 + t77)
      t18 = -t2 * t191 * t80 * t90 + t29 * t89 * (-t5 * t176 + t176 * t6
     #) - t30 * t87 * (t20 * (-t187 - t33) + t22 * (t187 + t33)) - t54 *
     # t89 * (t5 * (t188 - t15) + t6 * (-t188 + t15)) - t18 + t193
      t77 = -t29 * t195 * t176 - t30 * t197 * (t187 + t33) + t54 * t195 
     #* (-t188 + t15) + t6 * (-t1 * t18 + t200 * (-t199 * t80 - t198)) -
     # t201
      t79 = t37 * (t44 + t104)
      t44 = t37 * (t44 - t104)
      t47 = -t2 * t235 * t122 * t90 + t29 * t220 * (-t5 * t44 + t44 * t6
     #) - t30 * t87 * (t20 * (-t187 - t121) + t22 * (t187 + t121)) + t54
     # * t220 * (t5 * (-t234 + t79) + t6 * (t234 - t79)) + t193 - t47
      t80 = -t29 * t237 * t44 - t30 * t197 * (t187 + t121) + t54 * t237 
     #* (-t234 + t79) + t6 * (-t1 * t47 + t123 * (-t199 * t122 - t238)) 
     #+ t239
      t81 = t60 * (t66 - t76)
      t66 = t60 * (t66 + t76)
      t76 = dz * t66
      t104 = dz * t81
      t22 = -t2 * t244 * t164 * t90 - t29 * t88 * t104 * t189 - t30 * t8
     #7 * (t20 * (-t187 - t9) + t22 * (t187 + t9)) - t54 * t88 * (t243 *
     # t218 * t177 + t76 * t190) + t193 - t247
      t75 = -t29 * t196 * t104 - t30 * t197 * (t187 + t9) + t54 * t196 *
     # (-t218 * t177 * t60 + t76) + t6 * (-t1 * t22 + t250 * (-t199 * t1
     #64 - t249)) - t75
      t76 = t37 * (-t2 * t26 * (-t217 + t80) - t3 * t100 * (-t236 + t47)
     #) + t60 * (-t2 * t26 * (-t166 + t75) - t3 * t100 * (-t248 + t22)) 
     #+ t7 * (-t2 * t26 * (-t78 + t77) - t3 * t100 * (-t194 + t18)) + 0.
     #1E1
      t78 = t6 * (t1 * t140 + t138 * (t175 - t134 + t173 + t149))
      t90 = t6 * (t1 * t99 + t138 * (t175 - t229 + t172 + t149))
      t69 = -t54 * t167 * t177 * t66 * t8 - t29 * t220 * t81 * t8 + t30 
     #* t87 * (t13 * t86 + t85 * t9) + t69 * t92 + t91
      t9 = -t30 * cc * (t27 * (t13 - t9) * t20 + t246) + t6 * (dt * (-t1
     #82 * beta * t177 * t66 - t225 * t29 * t81 + t97 * t63 + t67 * t92)
     # - t1 * t69)
      t13 = t37 * (0.2E1 * t3 * t100 * t99 - t2 * t26 * (-t127 + t90)) +
     # t60 * (-t2 * t26 * (-t75 + t9) - t3 * t100 * (-t22 + t69)) + t7 *
     # (0.2E1 * t3 * t100 * t140 - t2 * t26 * (-t169 + t78))
      t5 = -t29 * t220 * t14 * t63 * t8 + t54 * t68 * (t178 * t189 - t5 
     #+ t6) + t28
      t14 = t54 * t68 * t6 * (-0.1E1 + t178) + t6 * (-t1 * t5 + t88 * t1
     #4 * t63 * (t24 * t165 * t30 - t209))
      t9 = t60 * (t2 * t26 * (t9 - t14) - t3 * t100 * (-t69 + t5)) + t15
     #7
      t22 = t26 * (-t4 * t154 + t156 * t2)
      t28 = t22 * t37
      t63 = -t70
      t66 = t6 * (-t1 * t63 + t68 * t212)
      t67 = t116 - t205 - t126 + t232 + t231 + t153
      t12 = t6 * (-t1 * t67 + t138 * (t25 * (-t121 * t30 + t61) * t19 * 
     #t26 - t124 + t12))
      t19 = t37 * (-t2 * t26 * (-t174 + t66) - t3 * t100 * (t168 + t63))
     # + t60 * (-t2 * t26 * (-t159 + t12) - t3 * t100 * (-t48 + t67)) + 
     #t131
      t25 = t6 * (t1 * t128 + t138 * (t129 + t132 + t133 - t135))
      t48 = -t29 * t220 * t44 * t8 - t54 * t220 * t79 * t8 - t30 * t87 *
     # (t121 * t86 + t219) - t221 + t222
      t39 = -t30 * cc * (t27 * (t39 - t121) * t20 + t224) + t6 * (dt * (
     #t40 - t225 * (t29 * t44 + t54 * t79) + t45) - t1 * t48)
      t21 = t30 * t192 * (t241 * t121 + t240) - t116 + t21
      t40 = t30 * t208 * (t24 * t121 - t155) + t6 * (-t1 * t21 - t41) + 
     #t207
      t12 = t37 * (-t2 * t26 * (-t80 + t39) - t3 * t100 * (-t47 + t48)) 
     #+ t60 * (-t2 * t26 * (-t12 + t40) - t3 * t100 * (-t67 + t21)) + t7
     # * (0.2E1 * t3 * t100 * t128 - t2 * t26 * (-t211 + t25))
      t21 = t37 * (-t2 * t26 * (-t90 + t66) - t3 * t100 * (t99 + t63)) +
     # t60 * (t2 * t26 * (-t110 + t40) + t3 * t100 * (-t139 + t21)) + t1
     #31
      t40 = t26 * (t66 * t2 + t4 * t63) * t37
      t41 = -t102
      t44 = t6 * (-t1 * t41 + t46 * (t105 - t29))
      t39 = t37 * (-t2 * t26 * (-t39 + t44) - t3 * t100 * (-t48 + t41)) 
     #+ t106
      t45 = t103 * t7
      t47 = t117 * t7
      t48 = -t50
      t50 = t51 - t30
      t46 = t6 * (-t1 * t48 + t46 * t50)
      t23 = t30 * t87 * t33 * t23
      t49 = t141 - t142 - t23 + t49
      t51 = t147 * t33
      t61 = t6 * (-t1 * t49 + t138 * (t135 - t145 - t146 - t51))
      t63 = t37 * (-t2 * t26 * (-t112 + t61) - t3 * t100 * (-t109 + t49)
     #) + t7 * (-t2 * t26 * (-t137 + t46) - t3 * t100 * (t130 + t48)) + 
     #t150
      t22 = t22 * t7
      t66 = -t72
      t50 = t6 * (-t1 * t66 + t68 * t50)
      t23 = t181 - t23 + t179 + t180 - t82 + t71
      t67 = t6 * (-t1 * t23 + t138 * (-t51 - t144 + t183 + t134))
      t68 = t60 * (-t2 * t26 * (-t160 + t67) - t3 * t100 * (-t108 + t23)
     #) + t7 * (-t2 * t26 * (-t206 + t50) - t3 * t100 * (t171 + t66)) + 
     #t185
      t8 = -t29 * t89 * t176 * t8 + t30 * t87 * (t10 * t86 + t33 * t85) 
     #- t54 * t89 * t15 * t8 + t93 + t94
      t10 = t30 * cc * (t27 * (-t10 + t33) * t20 - t96) + t6 * (dt * (-t
     #98 * (t15 * t54 + t176 * t29) + t16 + t11) - t1 * t8)
      t11 = t6 * (t1 * t49 + t138 * (-t135 + t145 + t146 + t51))
      t15 = t30 * t192 * (t241 * t33 + t203) + t204 - t71
      t16 = t30 * t208 * (t24 * t33 - t202) + t6 * (-t1 * t15 - t64) + t
     #148
      t18 = t37 * (0.2E1 * t3 * t100 * t49 - t2 * t26 * (-t61 + t11)) + 
     #t60 * (t2 * t26 * (t67 - t16) - t3 * t100 * (-t23 + t15)) + t7 * (
     #-t2 * t26 * (-t77 + t10) - t3 * t100 * (-t18 + t8))
      t15 = t60 * (t2 * t26 * (-t210 + t16) + t3 * t100 * (-t184 + t15))
     # + t7 * (-t2 * t26 * (-t78 + t50) - t3 * t100 * (t140 + t66)) + t1
     #85
      t11 = t37 * (t2 * t26 * (-t213 + t11) + t3 * t100 * (t109 - t49)) 
     #+ t7 * (-t2 * t26 * (-t25 + t46) - t3 * t100 * (t128 + t48)) + t15
     #0
      t16 = t26 * (t2 * t46 + t4 * t48) * t7
      t20 = t26 * (t2 * t50 + t4 * t66) * t7
      t23 = t56 + t20
      t24 = -t32
      t1 = t6 * (-t1 * t24 + t17 * (t34 - t29))
      t3 = t7 * (-t2 * t26 * (-t10 + t1) - t3 * t100 * (-t8 + t24)) + t5
     #5
      t1 = t26 * (t1 * t2 + t4 * t24)
      cuu(25) = t36 * t7
      cuu(67) = t59
      cuu(73) = t74
      cuu(74) = t35
      cuu(75) = t73
      cuu(81) = t57
      cuu(109) = t115
      cuu(115) = t120
      cuu(116) = t151
      cuu(117) = t152
      cuu(121) = t163
      cuu(122) = t186
      cuu(123) = t95
      cuu(124) = t52
      cuu(125) = t83 + t161
      cuu(129) = t118 + t43 + t65
      cuu(130) = t53
      cuu(131) = t42 + t43 + t65
      cuu(137) = t113 + t84
      cuu(151) = t214 * t37
      cuu(157) = t107 + t215
      cuu(158) = t143
      cuu(159) = t216 + t215
      cuu(163) = t228
      cuu(164) = t136
      cuu(165) = t38
      cuu(166) = t111
      cuu(167) = t125 + t158
      cuu(169) = t223 * t60
      cuu(170) = t170
      cuu(171) = t62
      cuu(172) = t76
      cuu(173) = t13
      cuu(174) = t9
      cuu(175) = t26 * (t14 * t2 + t4 * t5) * t60
      cuu(177) = t227 + t28
      cuu(178) = t19
      cuu(179) = t12
      cuu(180) = t21
      cuu(181) = t125 + t28
      cuu(185) = t107 + t40
      cuu(186) = t39
      cuu(187) = t216 + t40
      cuu(193) = t26 * (t44 * t2 + t4 * t41) * t37
      cuu(207) = t114 + t45
      cuu(213) = t118 + t119 + t47
      cuu(214) = t63
      cuu(215) = t42 + t119 + t47
      cuu(219) = t162 + t22
      cuu(220) = t68
      cuu(221) = t18
      cuu(222) = t15
      cuu(223) = t83 + t22
      cuu(227) = t118 + t47 + t65
      cuu(228) = t11
      cuu(229) = t42 + t47 + t65
      cuu(235) = t45 + t84
      cuu(263) = t16 + t58
      cuu(269) = t23
      cuu(270) = t3
      cuu(271) = t101 + t20
      cuu(277) = t31 + t16
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
      t5 = t2 + t4
      t4 = t2 - t4
      t6 = 0.1E1 / dx
      t7 = t4 ** 2
      t8 = t4 * t7
      t9 = t5 ** 2
      t10 = t9 - t7
      t11 = beta ** 2
      t12 = dt ** 2
      t13 = dt * t12
      t14 = t6 ** 2
      t15 = cc ** 2
      t16 = cc * t15
      t17 = t16 * t14
      t18 = t17 * t11 * t12
      t19 = t18 * t10
      t18 = t18 * t4
      t20 = t19 * t1
      t21 = t4 * (-t20 + t18)
      t22 = -0.5E1 / 0.288E3 * cc
      t23 = t21 / 0.12E2 + t22
      t24 = t19 / 0.72E2
      t25 = t24 * t1
      t26 = -t12 * (t2 * t23 + t25)
      t27 = 0.1E1 / dy
      t28 = t27 ** 2
      t29 = t16 * t28
      t30 = t29 * t11 * t12
      t31 = t30 * t10
      t30 = t4 * (-t1 * t31 + t30 * t4)
      t32 = t30 / 0.32E2
      t33 = t31 / 0.96E2
      t34 = -t12 * (t33 * t1 + t32)
      t20 = -t12 * (t21 / 0.96E2 + t20 / 0.288E3)
      t35 = t34 * t6
      t36 = t20 * t27
      t37 = 0.1E1 / dz
      t38 = t37 ** 2
      t39 = t37 * t38
      t40 = t16 * t38
      t41 = t40 * t11 * t12
      t42 = t41 * t10
      t43 = t4 * (-t1 * t42 + t41 * t4)
      t44 = t43 / 0.32E2
      t45 = t42 / 0.96E2
      t46 = -t12 * (t45 * t1 + t44)
      t47 = t46 * t6
      t48 = t20 * t37
      t49 = (t38 + t14 + t28) * t15
      t50 = 0.2E1 * t49
      t51 = t15 * t14
      t52 = cc * (t50 + t51)
      t53 = t6 * (-t52 + t17)
      t54 = t11 * t12
      t55 = t54 * dx
      t56 = t5 - t4
      t57 = t15 * dt
      t58 = t57 * t6
      t59 = t58 * t56
      t5 = t5 * t9 - t8
      t60 = t15 ** 2 * t13
      t61 = t60 * t6 * t14
      t62 = 0.7E1 / 0.24E2
      t63 = 0.1E1 / 0.16E2
      t64 = t62 * t19
      t65 = t59 / 0.12E2
      t66 = t3 * t61 * t5
      t67 = t10 * t53 * t55 * t63 - t64 + t65 - t66
      t68 = t17 * t62
      t61 = t61 * t3 * t4
      t69 = t58 / 0.12E2
      t70 = 0.25E2 / 0.288E3 * cc
      t71 = t4 * (-t1 * t67 + t4 * (-t61 + t54 * (dx * t53 * t63 - t68))
     # + t69) + t70
      t72 = t1 * t12
      t73 = -0.2E1 * (t27 + t37) * t20
      t74 = t6 * (-t2 * t12 * (-t23 + t71) + t72 * (-t3 * t67 + t24)) + 
     #t73
      t75 = t20 * t37
      t76 = t20 * t27
      t77 = t34 / 0.3E1
      t20 = 0.3E1 * t20
      t78 = t77 * t6
      t79 = t20 * t27
      t80 = t60 * t28
      t81 = 0.13E2 / 0.48E2 * t31
      t82 = -t3 * t80 * t6 * t5 - t81
      t83 = 0.13E2 / 0.48E2 * t11
      t84 = t3 * cc
      t85 = t84 * t4 * dt
      t86 = t85 * t6 + t83
      t87 = t29 * t4 * t12
      t88 = t87 * t86
      t89 = t4 * (-t1 * t82 - t88)
      t90 = t60 * t14
      t91 = 0.13E2 / 0.48E2 * t19
      t92 = -t3 * t90 * t27 * t5 - t91
      t93 = t85 * t27
      t94 = t83 + t93
      t95 = t17 * t4 * t12
      t96 = t95 * t94
      t97 = t4 * (-t1 * t92 - t96)
      t98 = t19 / 0.96E2
      t21 = t21 / 0.32E2
      t99 = t12 * (t1 * (-t3 * t82 + t33) - t2 * t89 + t32) * t6
      t100 = t12 * (t1 * (-t3 * t92 + t98) - t2 * t97 + t21) * t27
      t101 = t46 / 0.3E1
      t102 = t101 * t6
      t103 = t20 * t37
      t104 = t60 * t38
      t105 = 0.13E2 / 0.48E2 * t42
      t106 = -t3 * t104 * t6 * t5 - t105
      t107 = t40 * t4 * t12
      t86 = t107 * t86
      t108 = t4 * (-t1 * t106 - t86)
      t109 = t19 * dy * t37
      t90 = t3 * t90 * t37 * t5
      t19 = t19 / 0.3E1
      t110 = t109 * t63 - t19 - t90
      t111 = t11 * t63
      t11 = t11 / 0.3E1
      t112 = t4 * (-t1 * t110 + t95 * (t37 * (t111 * dy - t85) - t11))
      t113 = t12 * (t1 * (-t106 * t3 + t45) - t108 * t2 + t44) * t6
      t114 = t12 * (t1 * (-t110 * t3 + t98) - t112 * t2 + t21) * t37
      t115 = 0.2E1 * t15 * (t38 + t28) + 0.3E1 * t51
      t116 = t15 * t13
      t117 = t116 * t115 * t6
      t118 = t54 * cc
      t59 = 0.5E1 / 0.4E1 * t59
      t119 = 0.5E1 / 0.8E1 * t118 * t49 * t10
      t120 = t3 * t117 * t5 - t63 * t55 * (t53 * t7 - t9 * t53) + t19 - 
     #t59 + t119
      t121 = t55 * t7
      t49 = 0.5E1 / 0.8E1 * t49
      t51 = t118 * (t51 / 0.3E1 + t49)
      t58 = 0.5E1 / 0.4E1 * t58
      t122 = 0.25E2 / 0.144E3 * cc
      t53 = t63 * t121 * t53 + t4 * (-t1 * t120 + t4 * (t117 * t3 * t4 +
     # t51) - t58) - t122
      t96 = t4 * (t1 * t92 + t96)
      t90 = t91 + t90
      t83 = t85 * t37 + t83
      t85 = t4 * (-t1 * t90 + t95 * t83)
      t91 = (-t2 * t12 * (-t112 + t85) - t3 * t72 * (-t110 + t90)) * t37
      t95 = (0.2E1 * t3 * t72 * t92 - t2 * t12 * (-t97 + t96)) * t27
      t67 = t6 * (-t2 * t12 * (-t71 + t53) - t3 * t72 * (-t67 + t120)) +
     # t91 + t95
      t71 = t109 / 0.96E2
      t18 = t4 * (t18 * dy * t37 - t1 * t109) / 0.32E2
      t85 = t12 * (t1 * (t3 * t90 + t71) + t2 * t85 + t18) * t37
      t18 = -t12 * (t71 * t1 + t18) * t37
      t21 = t12 * (t1 * (-t3 * t92 + t98) + t2 * t96 + t21) * t27
      t71 = t21 + t99
      t20 = t20 * t27
      t22 = t30 / 0.12E2 + t22
      t30 = t31 / 0.72E2
      t90 = t30 * t1
      t92 = -t12 * (t2 * t22 + t90)
      t96 = t77 * t37
      t97 = t46 * t27
      t98 = t15 * t28
      t109 = cc * (t50 + t98)
      t110 = t27 * (t29 - t109)
      t112 = t54 * dy
      t117 = t57 * t27
      t123 = -t117 * t56
      t124 = t60 * t27 * t28
      t125 = t62 * t31
      t126 = t123 / 0.12E2
      t127 = -t3 * t124 * t5
      t128 = t63 * t112 * t110 * t10 - t125 - t126 + t127
      t129 = t29 * t62
      t124 = t124 * t3 * t4
      t130 = t117 / 0.12E2
      t131 = t4 * (-t1 * t128 + t4 * (-t124 + t54 * (dy * t110 * t63 - t
     #129)) + t130) + t70
      t132 = -0.2E1 / 0.3E1 * (t37 + t6) * t34
      t133 = t27 * (-t2 * t12 * (-t22 + t131) + t72 * (-t128 * t3 + t30)
     #) + t132
      t134 = t77 * t37
      t135 = t34 * t37
      t136 = t101 * t27
      t137 = t136 + t135
      t104 = -t3 * t104 * t27 * t5 - t105
      t94 = t107 * t94
      t105 = t4 * (-t1 * t104 - t94)
      t107 = t54 * t27 * t16 * t37
      t138 = t107 * t10
      t80 = t3 * t80 * t37 * t5
      t31 = t31 / 0.3E1
      t139 = t138 * t63 - t31 - t80
      t140 = t27 * t4
      t11 = t4 * (-t1 * t139 + t140 * t12 * t16 * (t37 * (t111 - t93) - 
     #t11 * t27))
      t93 = t12 * (t1 * (-t139 * t3 + t33) - t11 * t2 + t32) * t37
      t111 = t12 * (t1 * (-t104 * t3 + t45) - t105 * t2 + t44) * t27
      t88 = t4 * (t1 * t82 + t88)
      t141 = 0.2E1 * t15 * (t38 + t14) + 0.3E1 * t98
      t142 = -t9 + t7
      t143 = t9 - t7
      t144 = t116 * t141
      t123 = 0.5E1 / 0.4E1 * t123
      t145 = t63 * t112 * t110 * t143 + t3 * t144 * t27 * t5 + t119 + t1
     #23 + t31
      t146 = t112 * t7
      t98 = t118 * (t98 / 0.3E1 + t49)
      t117 = 0.5E1 / 0.4E1 * t117
      t110 = t63 * t146 * t110 + t4 * (-t1 * t145 + t4 * (t144 * t140 * 
     #t3 + t98) - t117) - t122
      t80 = t81 + t80
      t81 = t4 * (-t1 * t80 + t87 * t83)
      t83 = (0.2E1 * t3 * t72 * t82 + t2 * t12 * (t89 - t88)) * t6
      t11 = (t2 * t12 * (t11 - t81) - t3 * t72 * (-t139 + t80)) * t37
      t87 = t27 * (t2 * t12 * (t131 - t110) - t3 * t72 * (-t128 + t145))
     # + t83 + t11
      t89 = t138 / 0.96E2
      t107 = t4 * (-t1 * t138 + t107 * t4) / 0.32E2
      t80 = t12 * (t1 * (t3 * t80 + t89) + t2 * t81 + t107) * t37
      t81 = -t12 * (t89 * t1 + t107) * t37
      t89 = 0.1E1 / 0.48E2
      t107 = dy * t37
      t43 = (0.1E1 / 0.288E3 - t107 * t89) * cc + t43 / 0.12E2
      t128 = t42 / 0.72E2
      t131 = -t12 * (t1 * t128 + t2 * t43)
      t138 = t15 * t38
      t50 = cc * (t138 + t50)
      t139 = t37 * (t40 - t50)
      t57 = t57 * t37
      t56 = t57 * t56
      t60 = t60 * t39
      t144 = dz * t139
      t147 = t56 / 0.12E2
      t148 = t3 * t60 * t5
      t149 = -0.17E2 / 0.48E2 * t42 + t63 * t54 * (t143 * t39 * dy * t16
     # + t144 * t143) + t147 - t148
      t150 = t54 * t7
      t151 = dy ** 2
      t152 = t151 * cc * t38 / 0.288E3
      t41 = -cc / 0.18E2 + t4 * (-t1 * t149 + t57 / 0.12E2) + t63 * t150
     # * (dy * t16 * t39 + t144) - t152 + 0.7E1 / 0.48E2 * t107 * cc - t
     #60 * t3 * t8 - 0.17E2 / 0.48E2 * t41 * t7
      t57 = -0.2E1 / 0.3E1 * (t27 + t6) * t46
      t43 = t37 * (t2 * t12 * (t43 - t41) + t72 * (-t149 * t3 + t128)) +
     # t57
      t60 = t4 * (t1 * t106 + t86)
      t86 = t4 * (t1 * t104 + t94)
      t14 = 0.2E1 * t15 * (t14 + t28) + 0.3E1 * t138
      t28 = dy * t139
      t56 = 0.5E1 / 0.4E1 * t56
      t94 = t42 / 0.3E1
      t128 = t3 * t116 * t14 * t37 * t5 + t63 * t54 * t28 * t143 + t119 
     #- t56 + t94
      t138 = cc * t8 * t13
      t139 = cc * t37
      t142 = t4 * dt * t37
      t49 = t49 * t150
      t118 = (-0.5E1 / 0.4E1 * t142 + t118 * t7 * t38 / 0.3E1) * cc
      t144 = t151 * t38
      t28 = (-0.19E2 / 0.48E2 + t118 + t49 + t144 / 0.18E2) * cc + t3 * 
     #t139 * (t138 * t14 + dy) + t63 * t150 * t28 - t1 * t128 * t4
      t108 = (0.2E1 * t3 * t72 * t106 - t2 * t12 * (-t108 + t60)) * t6
      t105 = (0.2E1 * t3 * t72 * t104 + t2 * t12 * (t105 - t86)) * t27
      t41 = t37 * (-t2 * t12 * (-t41 + t28) - t3 * t72 * (-t149 + t128))
     # + t108 + t105
      t17 = t6 * (-t52 + t17)
      t52 = -t116 * t115 * t6
      t19 = t3 * t52 * t5 - t63 * t55 * (-t7 * t17 + t17 * t9) - t119 - 
     #t19 + t59
      t51 = -t63 * t121 * t17 + t4 * (-t19 * t1 + t4 * (t52 * t3 * t4 - 
     #t51) + t58) + t122
      t29 = t27 * (t29 - t109)
      t52 = -t116 * t141
      t7 = t3 * t52 * t27 * t5 + t63 * t112 * (t29 * t7 - t9 * t29) - t1
     #19 - t123 - t31
      t9 = -t63 * t146 * t29 + t4 * (-t1 * t7 + t4 * (t52 * t140 * t3 - 
     #t98) + t117) + t122
      t14 = -t14
      t31 = t37 * (t40 - t50)
      t40 = dz * t31
      t5 = t3 * t116 * t14 * t37 * t5 - t63 * t54 * t40 * t143 - t119 + 
     #t56 - t94
      t14 = (-0.5E1 / 0.48E2 * t144 + 0.4E1 / 0.9E1 - t118 - t49) * cc +
     # t3 * t139 * (t138 * t14 - dy) - t63 * t150 * t40 - t5 * t1 * t4
      t28 = t27 * (-t2 * t12 * (-t110 + t9) - t3 * t72 * (-t145 + t7)) +
     # t37 * (-t2 * t12 * (-t28 + t14) - t3 * t72 * (-t128 + t5)) + t6 *
     # (-t2 * t12 * (-t53 + t51) - t3 * t72 * (-t120 + t19)) + dt
      t31 = t112 * t31
      t40 = -t63 * t31 * t10 + t42 * t62 - t147 + t148
      t8 = (((t84 * t13 * t8 * t39 + t150 * t38 * t62) * cc - t142 / 0.1
     #2E2) * cc + t107 * (t107 / 0.18E2 - 0.7E1 / 0.48E2) + 0.1E1 / 0.28
     #8E3) * cc - t4 * (t31 * t63 * t4 + t1 * t40)
      t5 = t37 * (-t2 * t12 * (-t14 + t8) + t3 * t72 * (t5 - t40)) + t10
     #5 + t108
      t13 = -t63 * t112 * t16 * t39 * t10 - t42 * t89
      t14 = t13 * t1
      t15 = t89 * t139 * (-t150 * t15 * t37 + dy) - t152 - t14 * t4 - t1
     #46 * t63 * t16 * t39
      t8 = t37 * (t2 * t12 * (t8 - t15) - t3 * t72 * (-t40 + t13)) + t57
      t13 = t101 * t27
      t16 = t12 * (t1 * (-t104 * t3 + t45) + t2 * t86 + t44) * t27
      t31 = -t63 * t112 * t29 * t10 + t125 + t126 - t127
      t29 = t4 * (-t1 * t31 + t4 * (t124 + t54 * (-dy * t29 * t63 + t129
     #)) - t130) - t70
      t7 = t27 * (-t2 * t12 * (-t9 + t29) - t3 * t72 * (-t7 + t31)) + t1
     #1 + t83
      t9 = t46 * t27
      t11 = t27 * (t2 * t12 * (t29 + t22) + t72 * (t3 * t31 + t30)) + t1
     #32
      t22 = t12 * (-t2 * t22 - t90)
      t29 = t77 * t6
      t30 = t12 * (t1 * (-t3 * t82 + t33) + t2 * t88 + t32) * t6
      t31 = t101 * t6
      t32 = t12 * (t1 * (-t106 * t3 + t45) + t2 * t60 + t44) * t6
      t10 = -t63 * t55 * t17 * t10 + t64 - t65 + t66
      t1 = t4 * (-t1 * t10 + t4 * (t54 * (-dx * t17 * t63 + t68) + t61) 
     #- t69) - t70
      t4 = t6 * (-t2 * t12 * (-t51 + t1) - t3 * t72 * (-t19 + t10)) + t9
     #1 + t95
      t17 = t34 * t6
      t19 = t46 * t6
      t1 = t6 * (t2 * t12 * (t1 + t23) + t72 * (t10 * t3 + t24)) + t73
      t10 = t12 * (-t2 * t23 - t25)
      cuv(25) = t26 * t6
      cuv(67) = t35 + t36
      cuv(73) = t47 + t48
      cuv(74) = t74
      cuv(75) = t47 + t75
      cuv(81) = t35 + t76
      cuv(109) = t78 + t79
      cuv(116) = t100 + t99
      cuv(121) = t103 + t102
      cuv(122) = t114 + t113
      cuv(123) = t67
      cuv(124) = t113 + t85
      cuv(125) = t102 + t18
      cuv(130) = t71
      cuv(137) = t78 + t20
      cuv(151) = t92 * t27
      cuv(157) = t96 + t97
      cuv(158) = t133
      cuv(159) = t134 + t97
      cuv(163) = t137
      cuv(164) = t93 + t111
      cuv(165) = t87
      cuv(166) = t111 + t80
      cuv(167) = t136 + t81
      cuv(169) = t131 * t37
      cuv(170) = t43
      cuv(171) = t41
      cuv(172) = t28
      cuv(173) = t5
      cuv(174) = t8
      cuv(175) = t12 * (t14 * t3 + t15 * t2) * t37
      cuv(177) = t135 + t13
      cuv(178) = t93 + t16
      cuv(179) = t7
      cuv(180) = t16 + t80
      cuv(181) = t13 + t81
      cuv(185) = t96 + t9
      cuv(186) = t11
      cuv(187) = t134 + t9
      cuv(193) = t22 * t27
      cuv(207) = t29 + t79
      cuv(214) = t30 + t100
      cuv(219) = t31 + t103
      cuv(220) = t32 + t114
      cuv(221) = t4
      cuv(222) = t32 + t85
      cuv(223) = t31 + t18
      cuv(228) = t30 + t21
      cuv(235) = t29 + t20
      cuv(263) = t17 + t36
      cuv(269) = t19 + t48
      cuv(270) = t1
      cuv(271) = t19 + t75
      cuv(277) = t17 + t76
      cuv(319) = t10 * t6
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
      t33 = t15 * (-t32 + t27)
      t34 = t30 * t1
      t34 = -t34 * t2 * dt + dt * (t34 * t3 - t33)
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
      t46 = t28 * t42
      t47 = t45 / 0.6E1
      t48 = t46 - t47
      t49 = -t31 * t22 * t19 * t24 / 0.6E1 + t28
      t41 = t41 * t3
      t50 = t41 * t49
      t51 = t48 * t1
      t52 = 0.1E1 / 0.24E2
      t29 = t16 * t52 - t29
      t32 = t15 * (t52 - t32)
      t53 = t29 * t1
      t53 = -t53 * t2 * dt + dt * (t53 * t3 - t32)
      t51 = (dt * (t51 * t3 - t50) - t51 * t2 * dt) * t5
      t54 = t53 * t35
      t55 = t51 + t54
      t56 = 0.1E1 / dz
      t57 = t56 ** 2
      t58 = t57 ** 2
      t59 = t56 * t57
      t60 = t12 * t57
      t61 = t60 * beta
      t62 = t61 * dt
      t63 = t62 * t6
      t64 = t26 * t57 * t7
      t65 = t64 * t21
      t66 = t28 * t63
      t67 = t65 / 0.6E1
      t68 = -t67 + t66
      t69 = t62 * t3
      t49 = t69 * t49
      t70 = t68 * t1
      t70 = (dt * (t70 * t3 - t49) - t70 * t2 * dt) * t5
      t53 = t53 * t56
      t71 = t53 + t70
      t72 = (t36 + t7 + t57) * t10
      t73 = 0.2E1 * t72
      t74 = cc * (t73 + t31)
      t75 = t5 * (-t74 + t13)
      t76 = t36 + t57
      t77 = 0.2E1 * t10 * t76 + 0.3E1 * t31
      t78 = t10 * t5
      t79 = t11 * t7
      t76 = 0.2E1 * t79 * t76
      t80 = -t5 * t78 * (t77 + t31) - t76
      t81 = t5 * (t74 + t13)
      t82 = -t19 + t17
      t83 = t11 * t24
      t84 = beta * dt
      t85 = t84 * dx
      t86 = t20 - t18
      t87 = -t20 + t18
      t8 = t11 * t8
      t88 = t8 * t86
      t89 = t87 * t80
      t90 = t25 * cc
      t91 = 0.13E2 / 0.24E2
      t92 = t2 * t83 * t9 * t82
      t93 = t91 * t16
      t94 = t27 * t85 * t75 * t6 - t28 * t90 * (t89 + t88) + t52 * t85 *
     # t81 * t6 - t92 - t93
      t95 = cc * t5
      t14 = t14 * t91
      t96 = t3 * dt
      t97 = t96 * t2
      t98 = t97 * t11
      t9 = t98 * t9
      t99 = beta * dx
      t100 = -t28 * cc * (t25 * (-t8 + t80) * t20 - t95) + t96 * (-t14 -
     # t9 + t99 * (t27 * t75 + t52 * t81))
      t101 = t1 * t3
      t102 = t1 * dt
      t103 = -t29
      t29 = (t56 + t35) * (-t2 * t102 * (t103 - t29) - dt * (t101 * (t29
     # - t103) - 0.2E1 * t32))
      t104 = t5 * (-t2 * t102 * (t94 - t30) - dt * (t101 * (t30 - t94) -
     # t33 + t100)) + t29
      t105 = t102 * t2
      t32 = -dt * (t101 * t103 + t32) + t105 * t103
      t103 = t32 * t56
      t70 = t103 + t70
      t32 = t32 * t35
      t51 = t51 + t32
      t106 = -t28 * t26 * t37 * t21
      t107 = t42 * t52 + t106
      t108 = t10 * t36
      t109 = t108 * t28 * t22 * t19 * t24
      t110 = t41 * (t52 - t109)
      t111 = dt * (t101 * t107 - t110) - t105 * t107
      t112 = t28 * t16
      t113 = -t47 + t112
      t114 = t108 * t22 * t19 * t24 / 0.6E1
      t115 = t28 - t114
      t116 = t15 * t115
      t117 = t111 * t5
      t118 = (dt * (t101 * t113 - t116) - t105 * t113) * t35
      t119 = t118 + t117
      t43 = t43 * t57
      t120 = t43 * t21
      t121 = t120 * t1
      t122 = t102 * t28
      t123 = -t96 * (-t43 * t19 + t121) / 0.6E1 + t122 * t120
      t124 = t65 * t1
      t65 = -t96 * (-t64 * t19 + t124) / 0.6E1 + t122 * t65
      t1 = t45 * t1
      t45 = -t96 * (-t44 * t19 + t1) / 0.6E1 + t122 * t45
      t122 = t123 * t5
      t125 = t45 * t56
      t126 = t65 * t35
      t127 = t125 + t122 + t126
      t128 = t7 + t57
      t129 = 0.2E1 * t10 * t128 + 0.3E1 * t108
      t130 = t10 * t35
      t131 = t11 * t36
      t128 = 0.2E1 * t131 * t128
      t132 = -t35 * t130 * (t129 + t108) - t128
      t133 = t83 * t36
      t134 = t28 * t90 * t132 * t21
      t135 = t2 * t133 * t5 * t82
      t136 = -0.5E1 / 0.12E2 * t42
      t137 = t47 - t135 + t136 - t134
      t138 = t79 * t23 * t19 * t24 / 0.6E1
      t139 = t138 * t36
      t140 = -0.5E1 / 0.12E2 * t108 * beta
      t141 = t97 * t39
      t142 = t141 * t5
      t143 = t132 * t28
      t144 = t96 * cc
      t145 = t144 * (-t143 * t23 * t19 * t24 + t139 + t140 - t142)
      t79 = t79 * t24
      t146 = t28 * t90 * t80 * t21
      t147 = -t2 * t79 * t35 * t82
      t148 = -0.5E1 / 0.12E2 * t16
      t149 = t47 + t147 + t148 - t146
      t150 = -0.5E1 / 0.12E2 * t31 * beta
      t151 = t28 * t23 * t19 * t24
      t152 = t151 * t80
      t153 = t97 * t13
      t154 = t153 * t35
      t155 = t144 * (-t154 + t139 + t150 - t152)
      t156 = t3 / 0.3E1 - 0.1E1 / 0.6E1
      t1 = dt * (t1 * t156 - t44 * t20 / 0.3E1) * t56
      t44 = t35 * (-t2 * t102 * (t149 - t113) - dt * (t101 * (t113 - t14
     #9) - t116 + t155)) + t5 * (-t2 * t102 * (t137 - t48) - dt * (t101 
     #* (t48 - t137) - t50 + t145)) + t1
      t45 = t45 * t56
      t157 = t45 + t122 + t126
      t158 = t52 * t63
      t26 = t28 * t26 * t58 * t21
      t159 = -t26 + t158
      t160 = t10 * t57
      t161 = t160 * t28 * t22 * t19 * t24
      t162 = t69 * (-t161 + t52)
      t163 = dt * (t101 * t159 - t162) - t105 * t159
      t112 = -t67 + t112
      t164 = t28 - t160 * t22 * t19 * t24 / 0.6E1
      t165 = t15 * t164
      t166 = t163 * t5
      t167 = (dt * (t101 * t112 - t165) - t105 * t112) * t56
      t168 = t167 + t166
      t7 = t36 + t7
      t169 = 0.2E1 * t10 * t7 + 0.3E1 * t160
      t170 = t10 * t56
      t7 = 0.2E1 * t11 * t57 * t7
      t171 = -t56 * t170 * (t160 + t169) - t7
      t172 = t83 * t57
      t173 = t28 * t90 * t171 * t21
      t174 = t2 * t172 * t5 * t82
      t175 = -0.5E1 / 0.12E2 * t63
      t176 = -t174 + t67 + t175 - t173
      t138 = t138 * t57
      t177 = -0.5E1 / 0.12E2 * t160 * beta
      t178 = t97 * t60
      t179 = t178 * t5
      t180 = t151 * t171
      t181 = t144 * (-t180 + t138 + t177 - t179)
      t182 = dy ** 2
      t183 = t182 * t57
      t184 = t52 * t183 * t16
      t185 = t27 * t16 * dy * t56
      t79 = t2 * t79 * t56 * t82
      t186 = -0.7E1 / 0.12E2 * t16
      t146 = t67 + t185 - t79 + t186 - t146 + t184
      t187 = t52 * t56
      t188 = t31 * beta * (dy * t56 * (t187 * dy + t27) - 0.7E1 / 0.12E2
     #)
      t153 = t153 * t56
      t152 = t144 * (-t153 + t138 + t188 - t152)
      t124 = dt * (t124 * t156 - t64 * t20 / 0.3E1) * t35
      t189 = t5 * (t2 * t102 * (t68 - t176) - dt * (t101 * (t68 - t176) 
     #- t49 + t181)) + t56 * (t2 * t102 * (t112 - t146) - dt * (t101 * (
     #t112 - t146) - t165 + t152)) + t124
      t190 = 0.2E1 * t35 * t130 * t129 + 0.2E1 * t56 * t170 * t169 + 0.2
     #E1 * t5 * t78 * t77
      t191 = -0.2E1 * t5 * t74
      t192 = t4 - t3
      t193 = -t4 + t3
      t194 = t86 * t190
      t195 = t78 * t24
      t196 = t84 * cc
      t16 = 0.2E1 / 0.3E1 * t16
      t197 = 0.5E1 / 0.4E1 * t196 * t72 * t6
      t198 = t27 * t85 * t75 * t192 + t2 * t195 * t77 * t82 + t28 * t90 
     #* (t89 + t194) - t52 * t85 * (t191 * t193 + t192 * t81) + t16 + t1
     #97
      t199 = t85 * t3
      t200 = t90 * t20
      t201 = t84 * t3
      t72 = t201 * t72
      t202 = 0.2E1 / 0.3E1 * t95 * beta
      t203 = 0.5E1 / 0.4E1 * cc * (t95 - t72)
      t204 = t96 * t78
      t75 = t27 * t199 * t75 + t28 * t200 * (t80 - t190) - t52 * t199 * 
     #(t81 - t191) - t203 + t204 * (t97 * t77 + t202)
      t81 = t183 * t31
      t205 = t81 * t193
      t206 = cc * t56
      t17 = beta * t192 + t206 * (-t19 + t17) * dt
      t207 = t2 * t13 * dt * t17
      t89 = t28 * t196 * (t89 * t22 * t24 + t205) - t67 + t207
      t208 = t22 * t19 * t24
      t209 = t196 * t3
      t210 = t144 * t56 + beta
      t64 = t64 * t20 / 0.6E1
      t211 = t2 * t96 * t13 * t210
      t80 = t28 * t209 * (t208 * t80 - t81) - t64 + t211
      t94 = t35 * (0.2E1 * t2 * t102 * t149 + dt * (-0.2E1 * t101 * t149
     # + 0.2E1 * t155)) + t5 * (t2 * t102 * (t94 - t198) - dt * (t101 * 
     #(t94 - t198) - t100 + t75)) + t56 * (-t2 * t102 * (-t146 + t89) + 
     #dt * (t101 * (-t146 + t89) + t152 - t80))
      t7 = -t56 * t170 * (t160 + t169) - t7
      t100 = t28 * t90 * t7 * t21
      t146 = -t174 + t67 + t175 - t100
      t152 = t151 * t7
      t174 = t144 * (-t152 + t138 + t177 - t179)
      t179 = t67 - t185 + t184
      t212 = t27 * dy
      t15 = t15 * t56 * (t56 * (t182 * t52 + t208 * t10 / 0.6E1) - t212)
      t80 = t5 * (t2 * t102 * (t68 - t146) - dt * (t101 * (t68 - t146) -
     # t49 + t174)) + t56 * (t2 * t102 * (t89 - t179) - dt * (t101 * (t8
     #9 - t179) - t80 + t15)) + t124
      t89 = (-dt * (t101 * t179 - t15) + t105 * t179) * t56
      t65 = t65 * t35
      t128 = -t35 * t130 * (t129 + t108) - t128
      t213 = t28 * t90 * t128 * t21
      t135 = t47 - t135 + t136 - t213
      t136 = t144 * (-t151 * t128 + t139 + t140 - t142)
      t140 = -t113
      t142 = t35 * (-t2 * t102 * (t140 + t149) - dt * (t101 * (-t149 - t
     #140) + t155 - t116)) + t5 * (t2 * t102 * (t48 - t135) - dt * (t101
     # * (t48 - t135) - t50 + t136)) + t1
      t149 = (-dt * (t101 * t140 + t116) + t105 * t140) * t35
      t117 = t149 + t117
      t106 = t27 * t42 + t106
      t109 = t41 * (t27 - t109)
      t155 = dt * (t101 * t106 - t109) - t105 * t106
      t120 = t120 / 0.6E1
      t66 = -t120 + t66
      t115 = t69 * t115
      t111 = t111 * t56
      t214 = (dt * (t101 * t66 - t115) - t105 * t66) * t35
      t215 = -t107
      t108 = cc * (t108 + t73)
      t216 = t35 * (t39 + t108)
      t217 = t35 * (t39 - t108)
      t37 = t11 * t37
      t218 = t37 * t86
      t219 = t87 * t132
      t220 = t84 * dy
      t221 = -t2 * t83 * t38 * t82
      t222 = t91 * t42
      t223 = t27 * t220 * t217 * t6 - t28 * t90 * (t219 + t218) + t52 * 
     #t220 * t216 * t6 + t221 - t222
      t224 = cc * t35
      t40 = t40 * t91
      t38 = t98 * t38
      t225 = beta * dy
      t226 = t28 * cc * (t25 * (t37 - t132) * t20 + t224) + t96 * (-t40 
     #- t38 + t225 * (t216 * t52 + t217 * t27))
      t107 = (t56 + t5) * (t2 * t102 * (t107 - t215) - dt * (t101 * (t10
     #7 - t215) - 0.2E1 * t110))
      t227 = t35 * (t2 * t102 * (t106 - t223) - dt * (t101 * (t106 - t22
     #3) - t109 + t226)) + t107
      t110 = -dt * (t101 * t215 + t110) + t105 * t215
      t215 = t110 * t56
      t46 = t46 - t120
      t41 = t41 * t164
      t164 = (dt * (t101 * t46 - t41) - t105 * t46) * t56
      t163 = t163 * t35
      t172 = -t2 * t172 * t35 * t82
      t173 = t120 + t172 + t175 - t173
      t178 = t178 * t35
      t228 = t133 * t23 * t19 * t57 / 0.6E1
      t180 = t144 * (-t180 + t177 - t178 + t228)
      t229 = t27 * t84 * t35 * t12 * t56 * t6
      t133 = t2 * t133 * t56 * t82
      t230 = -0.7E1 / 0.12E2 * t42
      t134 = t120 - t133 + t229 + t230 - t134 + t158
      t231 = t27 * t35
      t57 = t131 * t57 / 0.6E1
      t131 = t141 * t56
      t10 = t10 * beta * (t56 * (t187 + t231) - 0.7E1 / 0.12E2 * t36)
      t36 = t144 * (t23 * (-t143 + t57) * t19 * t24 - t131 + t10)
      t121 = dt * (t121 * t156 - t43 * t20 / 0.3E1) * t5
      t141 = t35 * (-t2 * t102 * (-t66 + t173) - dt * (t101 * (t66 - t17
     #3) - t115 + t180)) + t56 * (t2 * t102 * (t46 - t134) - dt * (t101 
     #* (t46 - t134) - t41 + t36)) + t121
      t143 = -0.2E1 * t35 * t108
      t156 = t130 * t24
      t42 = 0.2E1 / 0.3E1 * t42
      t194 = t2 * t156 * t129 * t82 + t27 * t220 * (-t3 * t217 + t217 * 
     #t4) + t28 * t90 * (t219 + t194) + t52 * t220 * (t3 * (t216 - t143)
     # + t4 * (-t216 + t143)) + t197 + t42
      t232 = t201 * dy
      t233 = 0.2E1 / 0.3E1 * t224 * beta
      t234 = 0.5E1 / 0.4E1 * cc * (-t72 + t224)
      t130 = t130 * t96
      t216 = t27 * t232 * t217 + t28 * t200 * (t132 - t190) + t52 * t232
     # * (-t216 + t143) - t234 + t130 * (t97 * t129 + t233)
      t17 = t2 * t39 * dt * t17
      t217 = t28 * t196 * (t219 * t24 * t22 + t160 * t193) - t120 + t17
      t43 = t43 * t20 / 0.6E1
      t210 = t2 * t96 * t39 * t210
      t132 = t28 * t209 * (t208 * t132 - t160) - t43 + t210
      t36 = t35 * (-t2 * t102 * (t194 - t223) - dt * (t101 * (t223 - t19
     #4) - t226 + t216)) + t5 * (0.2E1 * t2 * t102 * t137 - dt * (0.2E1 
     #* t101 * t137 - 0.2E1 * t145)) + t56 * (-t2 * t102 * (-t134 + t217
     #) - dt * (t101 * (t134 - t217) - t36 + t132))
      t100 = t120 + t172 + t175 - t100
      t134 = t144 * (-t152 + t177 - t178 + t228)
      t152 = t120 - t229 + t158
      t114 = t201 * t56 * t12 * (t56 * (t114 + t52) - t231)
      t132 = t35 * (-t2 * t102 * (-t66 + t100) - dt * (t101 * (t66 - t10
     #0) - t115 + t134)) + t56 * (t2 * t102 * (t217 - t152) - dt * (t101
     # * (t217 - t152) - t132 + t114)) + t121
      t172 = (dt * (-t101 * t152 + t114) + t105 * t152) * t56
      t175 = t27 * t63 - t26
      t161 = t69 * (-t161 + t27)
      t177 = -dt * (-t101 * t175 + t161) - t105 * t175
      t178 = -t159
      t73 = cc * (t160 + t73)
      t217 = t56 * (t73 - t60)
      t219 = t56 * (t73 + t60)
      t223 = dz * t217
      t226 = dz * t219
      t11 = t11 * t58
      t228 = -0.17E2 / 0.24E2
      t83 = t2 * t83 * t59 * t82
      t231 = -t27 * t84 * (t193 * t59 * dy * t12 + t223 * t192) + t28 * 
     #t90 * (t11 * t87 + t171 * t86) + t52 * t84 * (t192 * t58 * t182 * 
     #t12 + t226 * t192) + t63 * t228 - t83
      t58 = -t27 * t201 * (-dy * t12 * t59 + t223) + t28 * cc * (t25 * (
     #t11 - t171) * t20 + t206) + t52 * t201 * (t182 * t12 * t58 + t226)
     # + t96 * t60 * (beta * t228 - t206 * t97)
      t159 = (t5 + t35) * (t2 * t102 * (t159 - t178) - dt * (t101 * (t15
     #9 - t178) - 0.2E1 * t162))
      t161 = t56 * (t2 * t102 * (t175 - t231) - dt * (t101 * (t175 - t23
     #1) - t161 + t58)) + t159
      t175 = -0.2E1 * t56 * t73
      t217 = dy * t217
      t223 = dz * t175
      t226 = t192 * t56
      t228 = t170 * t24
      t235 = 0.2E1 / 0.3E1 * t63
      t193 = -t27 * t84 * t217 * t192 + t2 * t228 * t169 * t82 - t28 * t
     #90 * (t18 * (-t171 + t190) + t20 * (t171 - t190)) - t52 * t84 * (t
     #226 * t219 * t182 + t223 * t193) + t197 + t235
      t236 = 0.2E1 / 0.3E1 * t206 * beta
      t72 = 0.5E1 / 0.4E1 * cc * (t206 - t72)
      t237 = t170 * t96
      t171 = -t27 * t201 * t217 - t28 * t200 * (-t171 + t190) - t52 * t2
     #01 * (t219 * t182 * t56 - t223) - t72 + t237 * (t97 * t169 + t236)
      t58 = t35 * (0.2E1 * t2 * t102 * t173 + dt * (-0.2E1 * t101 * t173
     # + 0.2E1 * t180)) + t5 * (0.2E1 * t2 * t102 * t176 + dt * (-0.2E1 
     #* t101 * t176 + 0.2E1 * t181)) + t56 * (-t2 * t102 * (-t231 + t193
     #) + dt * (t101 * (-t231 + t193) + t58 - t171))
      t217 = t5 * (-t74 + t13)
      t31 = -t5 * t78 * (t77 + t31) - t76
      t13 = t5 * (t74 + t13)
      t16 = -t2 * t195 * t77 * t82 - t27 * t85 * (-t3 * t217 + t217 * t4
     #) - t28 * t90 * (t18 * (-t190 + t31) + t20 * (t190 - t31)) + t52 *
     # t85 * (t3 * (t191 - t13) + t4 * (-t191 + t13)) - t16 - t197
      t74 = -t27 * t199 * t217 - t28 * t200 * (-t190 + t31) + t52 * t199
     # * (-t191 + t13) + t203 + t204 * (-t97 * t77 - t202)
      t76 = t35 * (t39 + t108)
      t39 = t35 * (t39 - t108)
      t42 = -t2 * t156 * t129 * t82 + t27 * t220 * (t3 * t39 - t4 * t39)
     # - t28 * t90 * (t18 * (-t190 + t128) + t20 * (t190 - t128)) - t52 
     #* t220 * (t3 * (-t143 + t76) + t4 * (t143 - t76)) - t197 - t42
      t77 = -t27 * t232 * t39 - t28 * t200 * (-t190 + t128) - t52 * t232
     # * (t143 - t76) + t234 + t130 * (-t97 * t129 - t233)
      t78 = t56 * (-t73 + t60)
      t60 = t56 * (-t73 - t60)
      t73 = dz * t78
      t108 = dz * t60
      t18 = -t2 * t228 * t169 * t82 - t27 * t84 * t73 * t192 - t28 * t90
     # * (t18 * (-t190 + t7) + t20 * (t190 - t7)) - t52 * t84 * (t226 * 
     #t175 * t182 + t108 * t192) - t197 - t235
      t72 = -t27 * t201 * t73 - t28 * t200 * (-t190 + t7) - t52 * t201 *
     # (t175 * t182 * t56 + t108) + t72 + t237 * (-t97 * t169 - t236)
      t73 = t35 * (-t2 * t102 * (-t194 + t42) - dt * (t101 * (t194 - t42
     #) - t216 + t77)) + t5 * (-t2 * t102 * (-t198 + t16) + dt * (t101 *
     # (-t198 + t16) + t75 - t74)) + t56 * (-t2 * t102 * (t18 - t193) - 
     #dt * (t101 * (t193 - t18) - t171 + t72))
      t63 = t52 * t84 * t182 * t60 * t56 * t6 - t27 * t220 * t78 * t6 + 
     #t28 * t90 * (t11 * t86 + t7 * t87) + t63 * t91 + t83
      t7 = t28 * cc * (t25 * (-t11 + t7) * t20 - t206) + t96 * (t187 * b
     #eta * t182 * t60 - t225 * t27 * t78 + t98 * t59 + t61 * t91)
      t11 = t35 * (0.2E1 * t2 * t102 * t100 - dt * (0.2E1 * t101 * t100 
     #- 0.2E1 * t134)) + t5 * (0.2E1 * t2 * t102 * t146 - dt * (0.2E1 * 
     #t101 * t146 - 0.2E1 * t174)) + t56 * (t2 * t102 * (-t63 + t18) + d
     #t * (t101 * (-t18 + t63) + t72 - t7))
      t3 = -t27 * t220 * t12 * t59 * t6 + t52 * t62 * (t183 * t192 + t3 
     #- t4) + t26
      t4 = t52 * t69 * (-0.1E1 + t183) + t201 * t12 * t59 * (t208 * t170
     # * t28 - t212)
      t7 = t56 * (t2 * t102 * (t63 - t3) + dt * (t101 * (-t63 + t3) + t7
     # - t4)) + t159
      t12 = dt * (-t101 * t178 - t162) + t105 * t178
      t18 = t12 * t35
      t26 = -t66
      t59 = t120 - t133 + t229 + t230 - t213 + t158
      t10 = t144 * (t23 * (-t128 * t28 + t57) * t19 * t24 - t131 + t10)
      t19 = t35 * (-t2 * t102 * (t173 + t26) - dt * (t101 * (-t173 - t26
     #) + t180 - t115)) + t56 * (-t2 * t102 * (-t46 + t59) - dt * (t101 
     #* (t46 - t59) - t41 + t10)) + t121
      t23 = -t27 * t220 * t39 * t6 + t28 * t90 * (t128 * t87 + t218) - t
     #52 * t220 * t76 * t6 - t221 + t222
      t37 = -t28 * cc * (t25 * (t37 - t128) * t20 + t224) + t96 * (t40 +
     # t38 - t225 * (t27 * t39 + t52 * t76))
      t17 = -t28 * t196 * (t86 * t128 * t24 * t22 + t160 * t192) - t120 
     #+ t17
      t38 = -t28 * t209 * (-t208 * t128 + t160) + t210 - t43
      t10 = t35 * (-t2 * t102 * (-t42 + t23) + dt * (t101 * (-t42 + t23)
     # + t77 - t37)) + t5 * (0.2E1 * t2 * t102 * t135 - dt * (0.2E1 * t1
     #01 * t135 - 0.2E1 * t136)) + t56 * (t2 * t102 * (t59 - t17) - dt *
     # (t101 * (t59 - t17) - t10 + t38))
      t17 = t35 * (t2 * t102 * (-t100 - t26) + dt * (t101 * (t100 + t26)
     # - t134 + t115)) + t56 * (t2 * t102 * (-t152 + t17) - dt * (t101 *
     # (-t152 + t17) + t114 - t38)) + t121
      t26 = (-dt * (t101 * t26 + t115) + t105 * t26) * t35
      t38 = -t106
      t23 = t35 * (-t2 * t102 * (-t23 + t38) + dt * (t101 * (-t23 + t38)
     # + t37 + t109)) + t107
      t37 = -dt * (t101 * t38 + t109) + t105 * t38
      t38 = t110 * t5
      t39 = t123 * t5
      t40 = -t48
      t21 = t28 * t90 * t31 * t21
      t41 = t47 + t147 + t148 - t21
      t42 = t151 * t31
      t43 = t144 * (-t42 + t139 + t150 - t154)
      t46 = t35 * (t2 * t102 * (t113 - t41) - dt * (t101 * (t113 - t41) 
     #- t116 + t43)) + t5 * (-t2 * t102 * (t137 + t40) + dt * (t101 * (t
     #137 + t40) - t145 + t50)) + t1
      t12 = t12 * t5
      t47 = -t68
      t21 = t67 + t185 - t79 + t186 - t21 + t184
      t42 = t144 * (-t42 + t138 + t188 - t153)
      t48 = t5 * (-t2 * t102 * (t47 + t176) - dt * (t101 * (-t176 - t47)
     # + t181 - t49)) + t56 * (t2 * t102 * (t112 - t21) - dt * (t101 * (
     #t112 - t21) - t165 + t42)) + t124
      t57 = t31 * t87
      t6 = -t27 * t85 * t217 * t6 + t28 * t90 * (t57 + t88) - t52 * t85 
     #* t13 * t6 + t92 + t93
      t8 = t28 * cc * (t25 * (-t8 + t31) * t20 - t95) + t96 * (t14 + t9 
     #- t99 * (t13 * t52 + t217 * t27))
      t9 = t28 * t196 * (t57 * t22 * t24 + t205) + t207 - t67
      t13 = t28 * t209 * (t208 * t31 - t81) + t211 - t64
      t14 = t35 * (0.2E1 * t2 * t102 * t41 - dt * (0.2E1 * t101 * t41 - 
     #0.2E1 * t43)) + t5 * (-t2 * t102 * (t6 - t16) - dt * (t101 * (t16 
     #- t6) - t74 + t8)) + t56 * (-t2 * t102 * (-t21 + t9) - dt * (t101 
     #* (t21 - t9) - t42 + t13))
      t9 = t5 * (-t2 * t102 * (t146 + t47) + dt * (t101 * (t146 + t47) -
     # t174 + t49)) + t56 * (t2 * t102 * (-t179 + t9) + dt * (t101 * (t1
     #79 - t9) - t15 + t13)) + t124
      t1 = t35 * (t2 * t102 * (-t41 - t140) + dt * (t101 * (t140 + t41) 
     #+ t116 - t43)) + t5 * (-t2 * t102 * (t135 + t40) + dt * (t101 * (t
     #135 + t40) - t136 + t50)) + t1
      t13 = (-dt * (t101 * t40 + t50) + t105 * t40) * t5
      t15 = (-dt * (t101 * t47 + t49) + t105 * t47) * t5
      t16 = -t30
      t2 = t5 * (-t2 * t102 * (t16 - t6) + dt * (t101 * (t16 - t6) + t8 
     #+ t33)) + t29
      t6 = -dt * (t101 * t16 + t33) + t105 * t16
      cvu(25) = t34 * t5
      cvu(67) = t55
      cvu(73) = t71
      cvu(74) = t104
      cvu(75) = t70
      cvu(81) = t51
      cvu(109) = t119
      cvu(115) = t127
      cvu(116) = t44
      cvu(117) = t157
      cvu(121) = t168
      cvu(122) = t189
      cvu(123) = t94
      cvu(124) = t80
      cvu(125) = t89 + t166
      cvu(129) = t125 + t65 + t122
      cvu(130) = t142
      cvu(131) = t45 + t65 + t122
      cvu(137) = t117
      cvu(151) = t155 * t35
      cvu(157) = t111 + t214
      cvu(158) = t227
      cvu(159) = t215 + t214
      cvu(163) = t164 + t163
      cvu(164) = t141
      cvu(165) = t36
      cvu(166) = t132
      cvu(167) = t172 + t163
      cvu(169) = t177 * t56
      cvu(170) = t161
      cvu(171) = t58
      cvu(172) = t73
      cvu(173) = t11
      cvu(174) = t7
      cvu(175) = (-dt * (t101 * t3 - t4) + t105 * t3) * t56
      cvu(177) = t18 + t164
      cvu(178) = t19
      cvu(179) = t10
      cvu(180) = t17
      cvu(181) = t18 + t172
      cvu(185) = t111 + t26
      cvu(186) = t23
      cvu(187) = t215 + t26
      cvu(193) = t37 * t35
      cvu(207) = t38 + t118
      cvu(213) = t125 + t39 + t126
      cvu(214) = t46
      cvu(215) = t45 + t39 + t126
      cvu(219) = t167 + t12
      cvu(220) = t48
      cvu(221) = t14
      cvu(222) = t9
      cvu(223) = t89 + t12
      cvu(227) = t125 + t65 + t39
      cvu(228) = t1
      cvu(229) = t45 + t65 + t39
      cvu(235) = t38 + t149
      cvu(263) = t13 + t54
      cvu(269) = t53 + t15
      cvu(270) = t2
      cvu(271) = t103 + t15
      cvu(277) = t13 + t32
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
      t7 = t5 ** 2
      t8 = t4 ** 2
      t9 = t4 * t8
      t10 = -t8 + t7
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
      t21 = cc * (t20 * t11 * t8 * t13 / 0.12E2 - 0.5E1 / 0.288E3)
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
      t35 = t34 * t33 * (-t30 * t4 + t32)
      t36 = t31 / 0.32E2
      t37 = -t36 * t1 * dt + t35
      t38 = t18 * t4
      t39 = t33 * (-t38 + t24)
      t40 = 0.1E1 / 0.48E2
      t41 = t39 * t40 - t24 * dt / 0.96E2
      t42 = t41 * t27
      t43 = t37 * t6
      t44 = 0.1E1 / dz
      t45 = t44 ** 2
      t46 = t44 * t45
      t47 = t16 * t45
      t48 = t47 * t11 * t13
      t49 = t48 * t10
      t50 = t49 * t1
      t51 = t34 * t33 * (-t48 * t4 + t50)
      t52 = t49 / 0.32E2
      t53 = -t52 * t1 * dt + t51
      t54 = t41 * t44
      t55 = t53 * t6
      t56 = (t28 + t45 + t12) * t15
      t57 = 0.2E1 * t56
      t58 = cc * (t20 + t57)
      t59 = t6 * (t17 - t58)
      t60 = t5 - t4
      t61 = t15 * dt
      t62 = t61 * t6 * t60
      t5 = t5 * t7 - t9
      t63 = t15 ** 2 * t14
      t64 = t63 * t6 * t12
      t65 = t11 * t13
      t66 = t65 * dx
      t67 = 0.7E1 / 0.24E2
      t68 = t62 / 0.12E2
      t69 = t3 * t64 * t5
      t70 = t67 * t19
      t71 = t34 * t66 * t59 * t10 + t68 - t69 - t70
      t72 = t17 * t67
      t73 = t33 / 0.12E2
      t74 = t73 * t15
      t75 = t74 * t6
      t64 = t64 * t3 * t9
      t76 = 0.25E2 / 0.288E3 * cc
      t77 = t65 * (dx * t59 * t34 - t72) * t8 + t76 - t64 + t75
      t78 = t1 * t4
      t22 = -t22 + 0.1E1 / 0.24E2
      t79 = t22 * t19
      t80 = t1 * dt
      t81 = -t4 / 0.24E2 + t40
      t18 = (t27 + t44) * dt * (t24 * t81 + t18 * t8 / 0.24E2)
      t24 = t6 * (t80 * (-t2 * t71 + t79) + (t78 * t71 + t21 - t77) * dt
     #) + t18
      t82 = t41 * t44
      t83 = t41 * t27
      t84 = t37 / 0.3E1
      t41 = 0.3E1 * t41
      t85 = t41 * t27
      t86 = t84 * t6
      t87 = t63 * t28
      t88 = 0.13E2 / 0.48E2 * t31
      t89 = -t3 * t87 * t6 * t5 - t88
      t90 = 0.13E2 / 0.48E2 * t11
      t91 = t33 * t3 * cc
      t92 = t91 * t6 + t90
      t93 = t29 * t8 * t13
      t94 = -t93 * t92
      t95 = t63 * t12
      t96 = 0.13E2 / 0.48E2 * t19
      t97 = -t3 * t95 * t27 * t5 - t96
      t98 = t91 * t27
      t99 = t98 + t90
      t100 = t17 * t8 * t13
      t101 = -t100 * t99
      t102 = t19 / 0.32E2
      t39 = t34 * t39
      t103 = (t80 * (-t2 * t97 + t102) - t39 + dt * (t78 * t97 - t101)) 
     #* t27
      t104 = (t80 * (-t2 * t89 + t36) - t35 + dt * (t78 * t89 - t94)) * 
     #t6
      t105 = t53 / 0.3E1
      t106 = t41 * t44
      t107 = t105 * t6
      t108 = t63 * t45
      t109 = 0.13E2 / 0.48E2 * t49
      t110 = -t3 * t108 * t6 * t5 - t109
      t111 = t47 * t8 * t13
      t92 = -t111 * t92
      t112 = t19 * dy * t44
      t95 = t3 * t95 * t44 * t5
      t19 = t19 / 0.3E1
      t113 = t112 * t34 - t19 - t95
      t114 = t11 * t34
      t115 = t11 / 0.3E1
      t116 = t100 * (t44 * (t114 * dy - t91) - t115)
      t117 = (t80 * (-t113 * t2 + t102) - t39 + dt * (t78 * t113 - t116)
     #) * t44
      t118 = (t80 * (-t110 * t2 + t52) - t51 + dt * (t78 * t110 - t92)) 
     #* t6
      t119 = 0.2E1 * t15 * (t28 + t45) + 0.3E1 * t20
      t120 = t15 * t14
      t62 = 0.5E1 / 0.4E1 * t62
      t121 = 0.5E1 / 0.8E1 * t65 * cc * t56 * t10
      t122 = t3 * t120 * t119 * t6 * t5 - t34 * t66 * (-t7 * t59 + t59 *
     # t8) + t19 - t62 + t121
      t123 = t66 * t8
      t56 = 0.5E1 / 0.8E1 * t56
      t20 = t20 / 0.3E1
      t124 = 0.5E1 / 0.4E1 * cc
      t125 = t3 * cc * t8 * t13
      t11 = t33 * t11
      t59 = t34 * t123 * t59 + cc * (t33 * (t125 * t119 * t6 - t124 * t6
     # + t11 * (t20 + t56)) - 0.25E2 / 0.144E3)
      t95 = t96 + t95
      t90 = t91 * t44 + t90
      t91 = t100 * t90
      t96 = (-t2 * t80 * (-t113 + t95) + dt * (t78 * (-t113 + t95) + t11
     #6 - t91)) * t44
      t100 = (0.2E1 * t2 * t80 * t97 - dt * (0.2E1 * t78 * t97 - 0.2E1 *
     # t101)) * t27
      t71 = t6 * (t2 * t80 * (t71 - t122) + dt * (t78 * (-t71 + t122) + 
     #t77 - t59)) + t96 + t100
      t77 = t112 / 0.32E2
      t38 = t34 * t33 * (-t38 * dy * t44 + t1 * t112)
      t91 = (t80 * (t2 * t95 + t77) - t38 - dt * (t78 * t95 - t91)) * t4
     #4
      t38 = (-t77 * t80 + t38) * t44
      t77 = t107 + t38
      t39 = (t80 * (-t2 * t97 + t102) - t39 - dt * (-t78 * t97 + t101)) 
     #* t27
      t41 = t41 * t27
      t95 = t15 * t28
      t97 = cc * (t95 * t65 * t8 / 0.12E2 - 0.5E1 / 0.288E3)
      t101 = t32 * t23
      t102 = dt * (t101 - t97)
      t112 = t84 * t44
      t113 = t53 * t27
      t116 = cc * (t95 + t57)
      t126 = t27 * (t29 - t116)
      t127 = -t61 * t27 * t60
      t128 = t63 * t27 * t28
      t129 = t65 * dy
      t130 = t127 / 0.12E2
      t131 = -t3 * t128 * t5
      t132 = t67 * t31
      t133 = t34 * t129 * t126 * t10 - t130 + t131 - t132
      t134 = t29 * t67
      t135 = t74 * t27
      t128 = t128 * t3 * t9
      t136 = t65 * (dy * t126 * t34 - t134) * t8 + t76 - t128 + t135
      t137 = t22 * t31
      t30 = (t44 + t6) * dt * (t32 * t81 + t30 * t8 / 0.24E2)
      t32 = t27 * (t80 * (-t133 * t2 + t137) + (t78 * t133 - t136 + t97)
     # * dt) + t30
      t138 = t84 * t44
      t139 = t37 * t44
      t140 = t105 * t27
      t141 = t140 + t139
      t108 = -t3 * t108 * t27 * t5 - t109
      t99 = -t111 * t99
      t109 = t65 * t27 * t16 * t44
      t111 = t109 * t10
      t87 = t3 * t87 * t44 * t5
      t31 = t31 / 0.3E1
      t142 = t111 * t34 - t31 - t87
      t98 = t27 * t8 * t13 * t16 * (t44 * (t114 - t98) - t115 * t27)
      t114 = (t80 * (-t142 * t2 + t36) - t35 + dt * (t78 * t142 - t98)) 
     #* t44
      t115 = (t80 * (-t108 * t2 + t52) - t51 + dt * (t78 * t108 - t99)) 
     #* t27
      t143 = 0.2E1 * t15 * (t45 + t12) + 0.3E1 * t95
      t127 = 0.5E1 / 0.4E1 * t127
      t144 = t3 * t120 * t143 * t27 * t5 + t34 * t129 * (t126 * t7 - t8 
     #* t126) + t121 + t31 + t127
      t145 = t129 * t8
      t95 = t95 / 0.3E1
      t125 = t34 * t145 * t126 + cc * (t33 * (t125 * t143 * t27 - t124 *
     # t27 + t11 * (t95 + t56)) - 0.25E2 / 0.144E3)
      t87 = t88 + t87
      t88 = t93 * t90
      t90 = (t2 * t80 * (t142 - t87) - dt * (t78 * (t142 - t87) - t98 + 
     #t88)) * t44
      t93 = (0.2E1 * t2 * t80 * t89 + dt * (-0.2E1 * t78 * t89 + 0.2E1 *
     # t94)) * t6
      t98 = t27 * (-t2 * t80 * (-t133 + t144) - dt * (t78 * (t133 - t144
     #) - t136 + t125)) + t90 + t93
      t126 = t111 / 0.32E2
      t1 = t34 * t33 * (t1 * t111 - t109 * t4)
      t4 = (t80 * (t2 * t87 + t126) - t1 - dt * (t78 * t87 - t88)) * t44
      t1 = (-t126 * t80 + t1) * t44
      t87 = t65 * t15 * t8
      t88 = cc * (t44 * (-dy * t40 + t87 * t44 / 0.12E2) + 0.1E1 / 0.288
     #E3)
      t23 = dt * (t50 * t23 - t88)
      t109 = t15 * t45
      t57 = cc * (t109 + t57)
      t111 = t44 * (t47 - t57)
      t60 = t61 * t44 * t60
      t61 = t63 * t46
      t63 = -t8 + t7
      t126 = dz * t111
      t133 = t60 / 0.12E2
      t136 = t3 * t61 * t5
      t142 = t34 * t65 * (t63 * t46 * dy * t16 + t126 * t63) - 0.17E2 / 
     #0.48E2 * t49 + t133 - t136
      t146 = t65 * t8
      t147 = dy / 0.288E3
      t73 = cc * (t44 * (dy * (-t147 * t44 + 0.7E1 / 0.48E2) + t73 * cc 
     #- t47 * t3 * t9 * t14 - 0.17E2 / 0.48E2 * t87 * t44) - 0.1E1 / 0.1
     #8E2) + t34 * t146 * (dy * t16 * t46 + t126)
      t48 = (t27 + t6) * dt * (t50 * t81 + t48 * t8 / 0.24E2)
      t22 = t44 * (t80 * (-t142 * t2 + t22 * t49) + (t78 * t142 - t73 + 
     #t88) * dt) + t48
      t12 = 0.2E1 * t15 * (t28 + t12) + 0.3E1 * t109
      t15 = dy * t111
      t28 = t49 / 0.3E1
      t50 = 0.5E1 / 0.4E1 * t60
      t60 = t3 * t120 * t12 * t44 * t5 + t34 * t65 * t15 * t63 + t121 + 
     #t28 - t50
      t14 = cc * t9 * t14
      t81 = cc * t44
      t88 = dy ** 2
      t111 = t124 * t33 * t44
      t109 = t146 * (t109 / 0.3E1 + t56)
      t15 = cc * (t88 * t45 / 0.18E2 + t109 - t111 - 0.19E2 / 0.48E2) + 
     #t3 * t81 * (t14 * t12 + dy) + t34 * t146 * t15
      t124 = (0.2E1 * t2 * t80 * t108 + dt * (-0.2E1 * t78 * t108 + 0.2E
     #1 * t99)) * t27
      t126 = (0.2E1 * t2 * t80 * t110 + dt * (-0.2E1 * t78 * t110 + 0.2E
     #1 * t92)) * t6
      t73 = t44 * (-t2 * t80 * (-t142 + t60) + dt * (t78 * (-t142 + t60)
     # + t73 - t15)) + t124 + t126
      t119 = -t119
      t17 = t6 * (t17 - t58)
      t19 = t3 * t120 * t119 * t6 * t5 - t34 * t66 * (t17 * t7 - t17 * t
     #8) - t121 - t19 + t62
      t13 = t8 * t13
      t56 = t56 * t11
      t20 = -t34 * t123 * t17 + cc * (t33 * (-t20 * t11 + (t13 * t119 * 
     #t3 + 0.5E1 / 0.4E1) * t6 * cc - t56) + 0.25E2 / 0.144E3)
      t29 = t27 * (t29 - t116)
      t58 = -t143
      t7 = t3 * t120 * t58 * t27 * t5 - t34 * t129 * (t29 * t7 - t29 * t
     #8) - t121 - t127 - t31
      t8 = -t34 * t145 * t29 + cc * (t33 * (-t95 * t11 + (t13 * t58 * t3
     # + 0.5E1 / 0.4E1) * t27 * cc - t56) + 0.25E2 / 0.144E3)
      t11 = -t12
      t12 = t44 * (t47 - t57)
      t13 = dz * t12
      t5 = t3 * t120 * t11 * t44 * t5 - t34 * t65 * t13 * t63 - t121 - t
     #28 + t50
      t11 = cc * (-0.5E1 / 0.48E2 * t88 * t45 - t109 + t111 + 0.4E1 / 0.
     #9E1) + t3 * t81 * (t14 * t11 - dy) - t34 * t146 * t13
      t13 = t27 * (-t2 * t80 * (-t144 + t7) + dt * (t78 * (-t144 + t7) +
     # t125 - t8)) + t44 * (-t2 * t80 * (-t60 + t5) - dt * (t78 * (t60 -
     # t5) - t15 + t11)) + t6 * (-t2 * t80 * (-t122 + t19) + dt * (t78 *
     # (-t122 + t19) + t59 - t20)) + 0.1E1
      t14 = -t34 * t129 * t12 * t10 + t49 * t67 - t133 + t136
      t15 = dy * t44
      t3 = cc * (t15 * (t15 / 0.18E2 - 0.7E1 / 0.48E2) + 0.1E1 / 0.288E3
     #) + t61 * t3 * t9 - t74 * t44 + t146 * (-dy * t12 * t34 + t47 * t6
     #7)
      t5 = t44 * (t2 * t80 * (t5 - t14) + dt * (t78 * (-t5 + t14) + t11 
     #- t3)) + t124 + t126
      t9 = -t34 * t129 * t16 * t46 * t10 - t40 * t49
      t11 = t87 * t44
      t11 = t40 * t81 * (-t11 + dy) - dy * cc * t45 * (t11 * t34 + t147)
      t3 = t44 * (t2 * t80 * (t14 - t9) - dt * (t78 * (t14 - t9) - t3 + 
     #t11)) + t48
      t12 = t105 * t27
      t14 = t12 + t139
      t15 = (t80 * (-t108 * t2 + t52) - t51 - dt * (-t78 * t108 + t99)) 
     #* t27
      t16 = -t34 * t129 * t29 * t10 + t130 - t131 + t132
      t28 = t146 * (-dy * t29 * t34 + t134) - t76 + t128 - t135
      t7 = t27 * (-t2 * t80 * (-t7 + t16) + dt * (t78 * (-t7 + t16) + t8
     # - t28)) + t90 + t93
      t8 = t53 * t27
      t29 = -t97
      t16 = t27 * (t80 * (t16 * t2 + t137) - (t78 * t16 - t28 + t29) * d
     #t) + t30
      t28 = dt * (t101 + t29)
      t29 = t84 * t6
      t30 = (t80 * (-t2 * t89 + t36) - t35 - dt * (-t78 * t89 + t94)) * 
     #t6
      t31 = t105 * t6
      t33 = (t80 * (-t110 * t2 + t52) - t51 - dt * (-t78 * t110 + t92)) 
     #* t6
      t10 = -t34 * t66 * t17 * t10 - t68 + t69 + t70
      t17 = t146 * (-dx * t17 * t34 + t72) - t76 + t64 - t75
      t19 = t6 * (-t2 * t80 * (-t19 + t10) + dt * (t78 * (-t19 + t10) + 
     #t20 - t17)) + t100 + t96
      t20 = t30 + t39
      t34 = t37 * t6
      t35 = t53 * t6
      t21 = -t21
      t10 = t6 * (t80 * (t10 * t2 + t79) - (t78 * t10 - t17 + t21) * dt)
     # + t18
      t17 = dt * (t25 + t21)
      cvv(25) = t26 * t6
      cvv(67) = t43 + t42
      cvv(73) = t55 + t54
      cvv(74) = t24
      cvv(75) = t55 + t82
      cvv(81) = t43 + t83
      cvv(109) = t86 + t85
      cvv(116) = t104 + t103
      cvv(121) = t107 + t106
      cvv(122) = t118 + t117
      cvv(123) = t71
      cvv(124) = t118 + t91
      cvv(125) = t77
      cvv(130) = t104 + t39
      cvv(137) = t86 + t41
      cvv(151) = t102 * t27
      cvv(157) = t113 + t112
      cvv(158) = t32
      cvv(159) = t113 + t138
      cvv(163) = t141
      cvv(164) = t115 + t114
      cvv(165) = t98
      cvv(166) = t115 + t4
      cvv(167) = t1 + t140
      cvv(169) = t23 * t44
      cvv(170) = t22
      cvv(171) = t73
      cvv(172) = t13
      cvv(173) = t5
      cvv(174) = t3
      cvv(175) = (t80 * t2 * t9 - dt * (t78 * t9 - t11)) * t44
      cvv(177) = t14
      cvv(178) = t15 + t114
      cvv(179) = t7
      cvv(180) = t15 + t4
      cvv(181) = t12 + t1
      cvv(185) = t8 + t112
      cvv(186) = t16
      cvv(187) = t8 + t138
      cvv(193) = t27 * t28
      cvv(207) = t29 + t85
      cvv(214) = t30 + t103
      cvv(219) = t31 + t106
      cvv(220) = t33 + t117
      cvv(221) = t19
      cvv(222) = t33 + t91
      cvv(223) = t31 + t38
      cvv(228) = t20
      cvv(235) = t29 + t41
      cvv(263) = t34 + t42
      cvv(269) = t35 + t54
      cvv(270) = t10
      cvv(271) = t35 + t82
      cvv(277) = t34 + t83
      cvv(319) = t17 * t6
c
      return
      end
