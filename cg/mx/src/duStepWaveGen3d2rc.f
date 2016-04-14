      subroutine duStepWaveGen3d2rc( 
     *   nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *   n1a,n1b,n2a,n2b,n3a,n3b,
     *   u,ut,unew,utnew,
     *   dx,dy,dz,dt,cc,
     *   i,j,k,n )

      implicit none
c
c.. declarations of incoming variables      
      integer nd1a,nd1b,nd2a,nd2b,nd3a,nd3b
      integer n1a,n1b,n2a,n2b,n3a,n3b
      integer i,j,k,n

      real u    (nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,*)
      real ut   (nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,*)
      real unew (nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real utnew(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real dx,dy,dz,dt,cc
c
c.. generated code to follow
c
        real t1
        real t10
        real t101
        real t105
        real t106
        real t107
        real t11
        integer t114
        real t12
        real t121
        real t125
        real t131
        real t135
        real t152
        real t157
        real t162
        real t17
        integer t172
        real t179
        real t183
        real t19
        real t197
        real t2
        integer t20
        real t202
        real t203
        real t205
        real t209
        real t210
        integer t225
        real t232
        real t236
        real t253
        real t258
        real t263
        integer t27
        real t28
        integer t281
        real t296
        real t301
        real t302
        real t304
        real t308
        real t309
        real t31
        integer t33
        integer t332
        real t34
        real t350
        real t4
        integer t40
        real t41
        real t44
        integer t46
        real t47
        integer t5
        real t6
        real t60
        real t62
        real t67
        integer t68
        real t69
        real t72
        real t75
        real t78
        real t79
        real t82
        real t85
        real t88
        real t89
        real t9
        real t92
        real t97
        real t98
        real t99
        t1 = u(i,j,k,n)
        t2 = ut(i,j,k,n)
        t4 = cc ** 2
        t5 = i + 1
        t6 = u(t5,j,k,n)
        t9 = 0.1E1 / dx
        t10 = t4 * (t6 - t1) * t9
        t11 = t4 * dt
        t12 = ut(t5,j,k,n)
        t17 = cc * t12
        t19 = dt * cc
        t20 = i + 2
        t27 = j + 1
        t28 = u(t5,t27,k,n)
        t31 = 0.1E1 / dy
        t33 = j - 1
        t34 = u(t5,t33,k,n)
        t40 = k + 1
        t41 = u(t5,j,t40,n)
        t44 = 0.1E1 / dz
        t46 = k - 1
        t47 = u(t5,j,t46,n)
        t60 = cc * t2
        t62 = (-t60 + t17) * t9
        t67 = t60 / 0.2E1
        t68 = i - 1
        t69 = u(t68,j,k,n)
        t72 = t4 * (t1 - t69) * t9
        t75 = u(i,t27,k,n)
        t78 = t4 * (t75 - t1) * t31
        t79 = u(i,t33,k,n)
        t82 = t4 * (t1 - t79) * t31
        t85 = u(i,j,t40,n)
        t88 = t4 * (t85 - t1) * t44
        t89 = u(i,j,t46,n)
        t92 = t4 * (t1 - t89) * t44
        t97 = t19 * ((t10 - t72) * t9 + (t78 - t82) * t31 + (t88 - t92) 
     #* t44) / 0.4E1
        t98 = ut(t68,j,k,n)
        t99 = cc * t98
        t101 = (t60 - t99) * t9
        t105 = dx * (t62 / 0.2E1 + t101 / 0.2E1) / 0.4E1
        t106 = t10 + t11 * (t12 - t2) * t9 / 0.2E1 + t17 / 0.2E1 + t19 *
     # ((t4 * (u(t20,j,k,n) - t6) * t9 - t10) * t9 + (t4 * (t28 - t6) * 
     #t31 - t4 * (t6 - t34) * t31) * t31 + (t4 * (t41 - t6) * t44 - t4 *
     # (t6 - t47) * t44) * t44) / 0.4E1 - dx * ((cc * ut(t20,j,k,n) - t1
     #7) * t9 / 0.2E1 + t62 / 0.2E1) / 0.4E1 - t67 - t97 - t105
        t107 = dt ** 2
        t114 = i - 2
        t121 = u(t68,t27,k,n)
        t125 = u(t68,t33,k,n)
        t131 = u(t68,j,t40,n)
        t135 = u(t68,j,t46,n)
        t152 = t72 + t11 * (t2 - t98) * t9 / 0.2E1 + t67 + t97 - t105 - 
     #t99 / 0.2E1 - t19 * ((t72 - t4 * (t69 - u(t114,j,k,n)) * t9) * t9 
     #+ (t4 * (t121 - t69) * t31 - t4 * (t69 - t125) * t31) * t31 + (t4 
     #* (t131 - t69) * t44 - t4 * (t69 - t135) * t44) * t44) / 0.4E1 - d
     #x * (t101 / 0.2E1 + (-cc * ut(t114,j,k,n) + t99) * t9 / 0.2E1) / 0
     #.4E1
        t157 = ut(i,t27,k,n)
        t162 = cc * t157
        t172 = j + 2
        t179 = u(i,t27,t40,n)
        t183 = u(i,t27,t46,n)
        t197 = (-t60 + t162) * t31
        t202 = ut(i,t33,k,n)
        t203 = cc * t202
        t205 = (t60 - t203) * t31
        t209 = dy * (t197 / 0.2E1 + t205 / 0.2E1) / 0.4E1
        t210 = t78 + t11 * (t157 - t2) * t31 / 0.2E1 + t162 / 0.2E1 + t1
     #9 * ((t4 * (t28 - t75) * t9 - t4 * (t75 - t121) * t9) * t9 + (t4 *
     # (u(i,t172,k,n) - t75) * t31 - t78) * t31 + (t4 * (t179 - t75) * t
     #44 - t4 * (t75 - t183) * t44) * t44) / 0.4E1 - dy * ((cc * ut(i,t1
     #72,k,n) - t162) * t31 / 0.2E1 + t197 / 0.2E1) / 0.4E1 - t67 - t97 
     #- t209
        t225 = j - 2
        t232 = u(i,t33,t40,n)
        t236 = u(i,t33,t46,n)
        t253 = t82 + t11 * (t2 - t202) * t31 / 0.2E1 + t67 + t97 - t209 
     #- t203 / 0.2E1 - t19 * ((t4 * (t34 - t79) * t9 - t4 * (t79 - t125)
     # * t9) * t9 + (t82 - t4 * (t79 - u(i,t225,k,n)) * t31) * t31 + (t4
     # * (t232 - t79) * t44 - t4 * (t79 - t236) * t44) * t44) / 0.4E1 - 
     #dy * (t205 / 0.2E1 + (-cc * ut(i,t225,k,n) + t203) * t31 / 0.2E1) 
     #/ 0.4E1
        t258 = ut(i,j,t40,n)
        t263 = cc * t258
        t281 = k + 2
        t296 = (-t60 + t263) * t44
        t301 = ut(i,j,t46,n)
        t302 = cc * t301
        t304 = (t60 - t302) * t44
        t308 = dz * (t296 / 0.2E1 + t304 / 0.2E1) / 0.4E1
        t309 = t88 + t11 * (t258 - t2) * t44 / 0.2E1 + t263 / 0.2E1 + t1
     #9 * ((t4 * (t41 - t85) * t9 - t4 * (t85 - t131) * t9) * t9 + (t4 *
     # (t179 - t85) * t31 - t4 * (t85 - t232) * t31) * t31 + (t4 * (u(i,
     #j,t281,n) - t85) * t44 - t88) * t44) / 0.4E1 - dz * ((cc * ut(i,j,
     #t281,n) - t263) * t44 / 0.2E1 + t296 / 0.2E1) / 0.4E1 - t67 - t97 
     #- t308
        t332 = k - 2
        t350 = t92 + t11 * (t2 - t301) * t44 / 0.2E1 + t67 + t97 - t308 
     #- t302 / 0.2E1 - t19 * ((t4 * (t47 - t89) * t9 - t4 * (t89 - t135)
     # * t9) * t9 + (t4 * (t183 - t89) * t31 - t4 * (t89 - t236) * t31) 
     #* t31 + (t92 - t4 * (t89 - u(i,j,t332,n)) * t44) * t44) / 0.4E1 - 
     #dz * (t304 / 0.2E1 + (-cc * ut(i,j,t332,n) + t302) * t44 / 0.2E1) 
     #/ 0.4E1
        unew(i,j,k) = t1 + dt * t2 + (t106 * t107 / 0.2E1 - t152 * t1
     #07 / 0.2E1) * t9 + (t210 * t107 / 0.2E1 - t253 * t107 / 0.2E1) * t
     #31 + (t309 * t107 / 0.2E1 - t350 * t107 / 0.2E1) * t44

        utnew(i,j,k) = t2 + (
     #dt * t106 - dt * t152) * t9 + (dt * t210 - dt * t253) * t31 + (dt 
     #* t309 - dt * t350) * t44


        return
      end
