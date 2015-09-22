      subroutine duStepWaveGen2d2rc_tz( 
     *   nd1a,nd1b,nd2a,nd2b,
     *   n1a,n1b,n2a,n2b,
     *   ndf4a,ndf4b,nComp,
     *   u,ut,unew,utnew,
     *   src,
     *   dx,dy,dt,cc,
     *   i,j,n )

      implicit none
c
c.. declarations of incoming variables      
      integer nd1a,nd1b,nd2a,nd2b
      integer n1a,n1b,n2a,n2b
      integer ndf4a,ndf4b,nComp
      integer i,j,n

      real u    (nd1a:nd1b,nd2a:nd2b,*)
      real ut   (nd1a:nd1b,nd2a:nd2b,*)
      real unew (nd1a:nd1b,nd2a:nd2b)
      real utnew(nd1a:nd1b,nd2a:nd2b)
      real src  (nd1a:nd1b,nd2a:nd2b,ndf4a:ndf4b,1:*)
      real dx,dy,dt,cc
c
c.. generated code to follow
c
        real t1
        real t10
        real t101
        real t102
        integer t103
        real t105
        real t107
        real t109
        real t11
        real t111
        real t118
        real t119
        real t12
        real t120
        real t133
        real t137
        real t164
        real t169
        real t17
        real t174
        integer t184
        real t19
        real t196
        real t198
        real t2
        integer t20
        real t201
        real t202
        real t203
        real t212
        real t215
        real t216
        real t218
        real t220
        real t222
        real t230
        integer t231
        real t233
        real t235
        real t237
        real t239
        real t246
        real t247
        integer t27
        real t28
        real t289
        real t295
        real t31
        integer t33
        real t34
        real t4
        real t45
        real t47
        real t49
        integer t5
        real t51
        real t52
        real t53
        real t6
        real t62
        integer t65
        real t66
        real t67
        real t69
        real t71
        real t73
        real t81
        real t82
        real t85
        real t88
        real t9
        real t91
        real t92
        real t95
        t1 = u(i,j,n)
        t2 = ut(i,j,n)
        t4 = cc ** 2
        t5 = i + 1
        t6 = u(t5,j,n)
        t9 = 0.1E1 / dx
        t10 = t4 * (t6 - t1) * t9
        t11 = t4 * dt
        t12 = ut(t5,j,n)
        t17 = cc * t12
        t19 = dt * cc
        t20 = i + 2
        t27 = j + 1
        t28 = u(t5,t27,n)
        t31 = 0.1E1 / dy
        t33 = j - 1
        t34 = u(t5,t33,n)
        t45 = cc * ut(t20,j,n)
        t47 = (-t17 + t45) * t9
        t49 = cc * t2
        t51 = (-t49 + t17) * t9
        t52 = t51 / 0.2E1
        t53 = dx ** 2
        t62 = (t47 - t51) * t9
        t65 = i - 1
        t66 = ut(t65,j,n)
        t67 = cc * t66
        t69 = (t49 - t67) * t9
        t71 = (t51 - t69) * t9
        t73 = (t62 - t71) * t9
        t81 = t49 / 0.2E1
        t82 = u(t65,j,n)
        t85 = t4 * (t1 - t82) * t9
        t88 = u(i,t27,n)
        t91 = t4 * (t88 - t1) * t31
        t92 = u(i,t33,n)
        t95 = t4 * (t1 - t92) * t31
        t101 = t19 * ((t10 - t85) * t9 + (t91 - t95) * t31 + src(i,j,nCo
     #mp,n)) / 0.4E1
        t102 = t69 / 0.2E1
        t103 = i - 2
        t105 = cc * ut(t103,j,n)
        t107 = (-t105 + t67) * t9
        t109 = (t69 - t107) * t9
        t111 = (t71 - t109) * t9
        t118 = dx * (t52 + t102 - t53 * (t73 / 0.2E1 + t111 / 0.2E1) / 0
     #.6E1) / 0.4E1
        t119 = t10 + t11 * (t12 - t2) * t9 / 0.2E1 + t17 / 0.2E1 + t19 *
     # ((t4 * (u(t20,j,n) - t6) * t9 - t10) * t9 + (t4 * (t28 - t6) * t3
     #1 - t4 * (t6 - t34) * t31) * t31 + src(t5,j,nComp,n)) / 0.4E1 - dx
     # * (t47 / 0.2E1 + t52 - t53 * ((((cc * ut(i + 3,j,n) - t45) * t9 -
     # t47) * t9 - t62) * t9 / 0.2E1 + t73 / 0.2E1) / 0.6E1) / 0.4E1 - t
     #81 - t101 - t118
        t120 = dt ** 2
        t133 = u(t65,t27,n)
        t137 = u(t65,t33,n)
        t164 = t85 + t11 * (t2 - t66) * t9 / 0.2E1 + t81 + t101 - t118 -
     # t67 / 0.2E1 - t19 * ((t85 - t4 * (t82 - u(t103,j,n)) * t9) * t9 +
     # (t4 * (t133 - t82) * t31 - t4 * (t82 - t137) * t31) * t31 + src(t
     #65,j,nComp,n)) / 0.4E1 - dx * (t102 + t107 / 0.2E1 - t53 * (t111 /
     # 0.2E1 + (t109 - (t107 - (-cc * ut(i - 3,j,n) + t105) * t9) * t9) 
     #* t9 / 0.2E1) / 0.6E1) / 0.4E1
        t169 = ut(i,t27,n)
        t174 = cc * t169
        t184 = j + 2
        t196 = cc * ut(i,t184,n)
        t198 = (-t174 + t196) * t31
        t201 = (-t49 + t174) * t31
        t202 = t201 / 0.2E1
        t203 = dy ** 2
        t212 = (t198 - t201) * t31
        t215 = ut(i,t33,n)
        t216 = cc * t215
        t218 = (t49 - t216) * t31
        t220 = (t201 - t218) * t31
        t222 = (t212 - t220) * t31
        t230 = t218 / 0.2E1
        t231 = j - 2
        t233 = cc * ut(i,t231,n)
        t235 = (-t233 + t216) * t31
        t237 = (t218 - t235) * t31
        t239 = (t220 - t237) * t31
        t246 = dy * (t202 + t230 - t203 * (t222 / 0.2E1 + t239 / 0.2E1) 
     #/ 0.6E1) / 0.4E1
        t247 = t91 + t11 * (t169 - t2) * t31 / 0.2E1 + t174 / 0.2E1 + t1
     #9 * ((t4 * (t28 - t88) * t9 - t4 * (t88 - t133) * t9) * t9 + (t4 *
     # (u(i,t184,n) - t88) * t31 - t91) * t31 + src(i,t27,nComp,n)) / 0.
     #4E1 - dy * (t198 / 0.2E1 + t202 - t203 * ((((cc * ut(i,j + 3,n) - 
     #t196) * t31 - t198) * t31 - t212) * t31 / 0.2E1 + t222 / 0.2E1) / 
     #0.6E1) / 0.4E1 - t81 - t101 - t246
        t289 = t95 + t11 * (t2 - t215) * t31 / 0.2E1 + t81 + t101 - t246
     # - t216 / 0.2E1 - t19 * ((t4 * (t34 - t92) * t9 - t4 * (t92 - t137
     #) * t9) * t9 + (t95 - t4 * (t92 - u(i,t231,n)) * t31) * t31 + src(
     #i,t33,nComp,n)) / 0.4E1 - dy * (t230 + t235 / 0.2E1 - t203 * (t239
     # / 0.2E1 + (t237 - (t235 - (-cc * ut(i,j - 3,n) + t233) * t31) * t
     #31) * t31 / 0.2E1) / 0.6E1) / 0.4E1
        t295 = src(i,j,nComp,n + 1)

        unew(i,j) = t1 + dt * t2 + (t119 * t120 / 0.2E1 - t164 * t1
     #20 / 0.2E1) * t9 + (t247 * t120 / 0.2E1 - t289 * t120 / 0.2E1) * t
     #31 + t295 * t120 / 0.2E1

        utnew(i,j) = t2 + (dt * t119 - dt * t164) * t9 + 
     #(dt * t247 - dt * t289) * t31 + t295 * dt

        return
      end
