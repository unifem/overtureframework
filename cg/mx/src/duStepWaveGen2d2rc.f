      subroutine duStepWaveGen2d2rc( 
     *   nd1a,nd1b,nd2a,nd2b,
     *   n1a,n1b,n2a,n2b,
     *   u,ut,unew,utnew,
     *   dx,dy,dt,cc,
     *   i,j,n )

      implicit none
c
c.. declarations of incoming variables      
      integer nd1a,nd1b,nd2a,nd2b
      integer n1a,n1b,n2a,n2b
      integer i,j,n

      real u    (nd1a:nd1b,nd2a:nd2b,*)
      real ut   (nd1a:nd1b,nd2a:nd2b,*)
      real unew (nd1a:nd1b,nd2a:nd2b)
      real utnew(nd1a:nd1b,nd2a:nd2b)
      real dx,dy,dt,cc
c
c.. generated code to follow
c
        real t1
        real t10
        real t100
        integer t101
        real t103
        real t105
        real t107
        real t109
        real t11
        real t116
        real t117
        real t118
        real t12
        real t131
        real t135
        real t161
        real t166
        real t17
        real t171
        integer t181
        real t19
        real t192
        real t194
        real t197
        real t198
        real t199
        real t2
        integer t20
        real t208
        real t211
        real t212
        real t214
        real t216
        real t218
        real t226
        integer t227
        real t229
        real t231
        real t233
        real t235
        real t242
        real t243
        integer t27
        real t28
        real t284
        real t31
        integer t33
        real t34
        real t4
        real t44
        real t46
        real t48
        integer t5
        real t50
        real t51
        real t52
        real t6
        real t61
        integer t64
        real t65
        real t66
        real t68
        real t70
        real t72
        real t80
        real t81
        real t84
        real t87
        real t9
        real t90
        real t91
        real t94
        real t99
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
        t44 = cc * ut(t20,j,n)
        t46 = (-t17 + t44) * t9
        t48 = cc * t2
        t50 = (-t48 + t17) * t9
        t51 = t50 / 0.2E1
        t52 = dx ** 2
        t61 = (t46 - t50) * t9
        t64 = i - 1
        t65 = ut(t64,j,n)
        t66 = cc * t65
        t68 = (t48 - t66) * t9
        t70 = (t50 - t68) * t9
        t72 = (t61 - t70) * t9
        t80 = t48 / 0.2E1
        t81 = u(t64,j,n)
        t84 = t4 * (t1 - t81) * t9
        t87 = u(i,t27,n)
        t90 = t4 * (t87 - t1) * t31
        t91 = u(i,t33,n)
        t94 = t4 * (t1 - t91) * t31
        t99 = t19 * ((t10 - t84) * t9 + (t90 - t94) * t31) / 0.4E1
        t100 = t68 / 0.2E1
        t101 = i - 2
        t103 = cc * ut(t101,j,n)
        t105 = (-t103 + t66) * t9
        t107 = (t68 - t105) * t9
        t109 = (t70 - t107) * t9
        t116 = dx * (t51 + t100 - t52 * (t72 / 0.2E1 + t109 / 0.2E1) / 0
     #.6E1) / 0.4E1
        t117 = t10 + t11 * (t12 - t2) * t9 / 0.2E1 + t17 / 0.2E1 + t19 *
     # ((t4 * (u(t20,j,n) - t6) * t9 - t10) * t9 + (t4 * (t28 - t6) * t3
     #1 - t4 * (t6 - t34) * t31) * t31) / 0.4E1 - dx * (t46 / 0.2E1 + t5
     #1 - t52 * ((((cc * ut(i + 3,j,n) - t44) * t9 - t46) * t9 - t61) * 
     #t9 / 0.2E1 + t72 / 0.2E1) / 0.6E1) / 0.4E1 - t80 - t99 - t116
        t118 = dt ** 2
        t131 = u(t64,t27,n)
        t135 = u(t64,t33,n)
        t161 = t84 + t11 * (t2 - t65) * t9 / 0.2E1 + t80 + t99 - t116 - 
     #t66 / 0.2E1 - t19 * ((t84 - t4 * (t81 - u(t101,j,n)) * t9) * t9 + 
     #(t4 * (t131 - t81) * t31 - t4 * (t81 - t135) * t31) * t31) / 0.4E1
     # - dx * (t100 + t105 / 0.2E1 - t52 * (t109 / 0.2E1 + (t107 - (t105
     # - (-cc * ut(i - 3,j,n) + t103) * t9) * t9) * t9 / 0.2E1) / 0.6E1)
     # / 0.4E1
        t166 = ut(i,t27,n)
        t171 = cc * t166
        t181 = j + 2
        t192 = cc * ut(i,t181,n)
        t194 = (-t171 + t192) * t31
        t197 = (-t48 + t171) * t31
        t198 = t197 / 0.2E1
        t199 = dy ** 2
        t208 = (t194 - t197) * t31
        t211 = ut(i,t33,n)
        t212 = cc * t211
        t214 = (t48 - t212) * t31
        t216 = (t197 - t214) * t31
        t218 = (t208 - t216) * t31
        t226 = t214 / 0.2E1
        t227 = j - 2
        t229 = cc * ut(i,t227,n)
        t231 = (-t229 + t212) * t31
        t233 = (-t231 + t214) * t31
        t235 = (t216 - t233) * t31
        t242 = dy * (t198 + t226 - t199 * (t218 / 0.2E1 + t235 / 0.2E1) 
     #/ 0.6E1) / 0.4E1
        t243 = t90 + t11 * (t166 - t2) * t31 / 0.2E1 + t171 / 0.2E1 + t1
     #9 * ((t4 * (t28 - t87) * t9 - t4 * (t87 - t131) * t9) * t9 + (t4 *
     # (u(i,t181,n) - t87) * t31 - t90) * t31) / 0.4E1 - dy * (t194 / 0.
     #2E1 + t198 - t199 * ((((cc * ut(i,j + 3,n) - t192) * t31 - t194) *
     # t31 - t208) * t31 / 0.2E1 + t218 / 0.2E1) / 0.6E1) / 0.4E1 - t80 
     #- t99 - t242
        t284 = t94 + t11 * (t2 - t211) * t31 / 0.2E1 + t80 + t99 - t242 
     #- t212 / 0.2E1 - t19 * ((t4 * (t34 - t91) * t9 - t4 * (t91 - t135)
     # * t9) * t9 + (t94 - t4 * (t91 - u(i,t227,n)) * t31) * t31) / 0.4E
     #1 - dy * (t226 + t231 / 0.2E1 - t199 * (t235 / 0.2E1 + (t233 - (t2
     #31 - (-cc * ut(i,j - 3,n) + t229) * t31) * t31) * t31 / 0.2E1) / 0
     #.6E1) / 0.4E1

        unew(i,j) = t1 + dt * t2 + (t117 * t118 / 0.2E1 - t161 * t1
     #18 / 0.2E1) * t9 + (t243 * t118 / 0.2E1 - t284 * t118 / 0.2E1) * t
     #31

        utnew(i,j) = 
     # t2 + (dt * t117 - dt * t161) * t9 + (dt * t243 - dt * t284
     #) * t31

        return
      end
