      subroutine duStepWaveGen3d2rc_tz( 
     *   nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *   n1a,n1b,n2a,n2b,n3a,n3b,
     *   ndf4a,ndf4b,nComp,
     *   u,ut,unew,utnew,
     *   src,
     *   dx,dy,dz,dt,cc,
     *   i,j,k,n )

      implicit none
c
c.. declarations of incoming variables      
      integer nd1a,nd1b,nd2a,nd2b,nd3a,nd3b
      integer n1a,n1b,n2a,n2b,n3a,n3b
      integer ndf4a,ndf4b,nComp
      integer i,j,k,n

      real u    (nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,*)
      real ut   (nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,*)
      real unew (nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real utnew(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real src  (nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,ndf4a:ndf4b,1:*)
      real dx,dy,dz,dt,cc
c
c.. generated code to follow
c
        real t1
        real t10
        real t100
        real t101
        real t103
        real t107
        real t108
        real t109
        real t11
        integer t116
        real t12
        real t123
        real t127
        real t133
        real t137
        real t155
        real t160
        real t165
        real t17
        integer t175
        real t182
        real t186
        real t19
        real t2
        integer t20
        real t201
        real t206
        real t207
        real t209
        real t213
        real t214
        integer t229
        real t236
        real t240
        real t258
        real t263
        real t268
        integer t27
        real t28
        integer t286
        real t302
        real t307
        real t308
        real t31
        real t310
        real t314
        real t315
        integer t33
        integer t338
        real t34
        real t357
        real t363
        real t4
        integer t40
        real t41
        real t44
        integer t46
        real t47
        integer t5
        real t6
        real t61
        real t63
        real t68
        integer t69
        real t70
        real t73
        real t76
        real t79
        real t80
        real t83
        real t86
        real t89
        real t9
        real t90
        real t93
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
        t61 = cc * t2
        t63 = (-t61 + t17) * t9
        t68 = t61 / 0.2E1
        t69 = i - 1
        t70 = u(t69,j,k,n)
        t73 = t4 * (t1 - t70) * t9
        t76 = u(i,t27,k,n)
        t79 = t4 * (t76 - t1) * t31
        t80 = u(i,t33,k,n)
        t83 = t4 * (t1 - t80) * t31
        t86 = u(i,j,t40,n)
        t89 = t4 * (t86 - t1) * t44
        t90 = u(i,j,t46,n)
        t93 = t4 * (t1 - t90) * t44
        t99 = t19 * ((t10 - t73) * t9 + (t79 - t83) * t31 + (t89 - t93) 
     #* t44 + src(i,j,k,nComp,n)) / 0.4E1
        t100 = ut(t69,j,k,n)
        t101 = cc * t100
        t103 = (t61 - t101) * t9
        t107 = dx * (t63 / 0.2E1 + t103 / 0.2E1) / 0.4E1
        t108 = t10 + t11 * (t12 - t2) * t9 / 0.2E1 + t17 / 0.2E1 + t19 *
     # ((t4 * (u(t20,j,k,n) - t6) * t9 - t10) * t9 + (t4 * (t28 - t6) * 
     #t31 - t4 * (t6 - t34) * t31) * t31 + (t4 * (t41 - t6) * t44 - t4 *
     # (t6 - t47) * t44) * t44 + src(t5,j,k,nComp,n)) / 0.4E1 - dx * ((c
     #c * ut(t20,j,k,n) - t17) * t9 / 0.2E1 + t63 / 0.2E1) / 0.4E1 - t68
     # - t99 - t107
        t109 = dt ** 2
        t116 = i - 2
        t123 = u(t69,t27,k,n)
        t127 = u(t69,t33,k,n)
        t133 = u(t69,j,t40,n)
        t137 = u(t69,j,t46,n)
        t155 = t73 + t11 * (t2 - t100) * t9 / 0.2E1 + t68 + t99 - t107 -
     # t101 / 0.2E1 - t19 * ((t73 - t4 * (t70 - u(t116,j,k,n)) * t9) * t
     #9 + (t4 * (t123 - t70) * t31 - t4 * (t70 - t127) * t31) * t31 + (t
     #4 * (t133 - t70) * t44 - t4 * (t70 - t137) * t44) * t44 + src(t69,
     #j,k,nComp,n)) / 0.4E1 - dx * (t103 / 0.2E1 + (-cc * ut(t116,j,k,n)
     # + t101) * t9 / 0.2E1) / 0.4E1
        t160 = ut(i,t27,k,n)
        t165 = cc * t160
        t175 = j + 2
        t182 = u(i,t27,t40,n)
        t186 = u(i,t27,t46,n)
        t201 = (-t61 + t165) * t31
        t206 = ut(i,t33,k,n)
        t207 = cc * t206
        t209 = (t61 - t207) * t31
        t213 = dy * (t201 / 0.2E1 + t209 / 0.2E1) / 0.4E1
        t214 = t79 + t11 * (t160 - t2) * t31 / 0.2E1 + t165 / 0.2E1 + t1
     #9 * ((t4 * (t28 - t76) * t9 - t4 * (t76 - t123) * t9) * t9 + (t4 *
     # (u(i,t175,k,n) - t76) * t31 - t79) * t31 + (t4 * (t182 - t76) * t
     #44 - t4 * (t76 - t186) * t44) * t44 + src(i,t27,k,nComp,n)) / 0.4E
     #1 - dy * ((cc * ut(i,t175,k,n) - t165) * t31 / 0.2E1 + t201 / 0.2E
     #1) / 0.4E1 - t68 - t99 - t213
        t229 = j - 2
        t236 = u(i,t33,t40,n)
        t240 = u(i,t33,t46,n)
        t258 = t83 + t11 * (t2 - t206) * t31 / 0.2E1 + t68 + t99 - t213 
     #- t207 / 0.2E1 - t19 * ((t4 * (t34 - t80) * t9 - t4 * (t80 - t127)
     # * t9) * t9 + (t83 - t4 * (t80 - u(i,t229,k,n)) * t31) * t31 + (t4
     # * (t236 - t80) * t44 - t4 * (t80 - t240) * t44) * t44 + src(i,t33
     #,k,nComp,n)) / 0.4E1 - dy * (t209 / 0.2E1 + (-cc * ut(i,t229,k,n) 
     #+ t207) * t31 / 0.2E1) / 0.4E1
        t263 = ut(i,j,t40,n)
        t268 = cc * t263
        t286 = k + 2
        t302 = (-t61 + t268) * t44
        t307 = ut(i,j,t46,n)
        t308 = cc * t307
        t310 = (t61 - t308) * t44
        t314 = dy * (t302 / 0.2E1 + t310 / 0.2E1) / 0.4E1
        t315 = t89 + t11 * (t263 - t2) * t44 / 0.2E1 + t268 / 0.2E1 + t1
     #9 * ((t4 * (t41 - t86) * t9 - t4 * (t86 - t133) * t9) * t9 + (t4 *
     # (t182 - t86) * t31 - t4 * (t86 - t236) * t31) * t31 + (t4 * (u(i,
     #j,t286,n) - t86) * t44 - t89) * t44 + src(i,j,t40,nComp,n)) / 0.4E
     #1 - dy * ((cc * ut(i,j,t286,n) - t268) * t44 / 0.2E1 + t302 / 0.2E
     #1) / 0.4E1 - t68 - t99 - t314
        t338 = k - 2
        t357 = t93 + t11 * (t2 - t307) * t44 / 0.2E1 + t68 + t99 - t314 
     #- t308 / 0.2E1 - t19 * ((t4 * (t47 - t90) * t9 - t4 * (t90 - t137)
     # * t9) * t9 + (t4 * (t186 - t90) * t31 - t4 * (t90 - t240) * t31) 
     #* t31 + (t93 - t4 * (t90 - u(i,j,t338,n)) * t44) * t44 + src(i,j,t
     #46,nComp,n)) / 0.4E1 - dy * (t310 / 0.2E1 + (-cc * ut(i,j,t338,n) 
     #+ t308) * t44 / 0.2E1) / 0.4E1
        t363 = src(i,j,k,nComp,n + 1)

        unew(i,j,k) = t1 + dt * t2 + (t108 * t109 / 0.2E1 - t155 * t1
     #09 / 0.2E1) * t9 + (t214 * t109 / 0.2E1 - t258 * t109 / 0.2E1) * t
     #31 + (t315 * t109 / 0.2E1 - t357 * t109 / 0.2E1) * t44 + t363 * t1
     #09 / 0.2E1

        utnew(i,j,k) = 
     #t2 + (dt * t108 - dt * t155) * t9 + (dt * t214 - d
     #t * t258) * t31 + (dt * t315 - dt * t357) * t44 + t363 * dt

        return
      end
