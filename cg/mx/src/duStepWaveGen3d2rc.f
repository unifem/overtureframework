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
        real t102
        real t103
        integer t107
        real t11
        real t114
        real t118
        real t12
        real t124
        real t128
        real t14
        real t148
        real t153
        real t155
        integer t166
        integer t17
        real t173
        real t177
        real t193
        real t195
        real t199
        real t2
        real t204
        integer t216
        real t223
        real t227
        integer t24
        real t247
        real t25
        real t252
        real t254
        integer t273
        real t28
        real t290
        real t292
        real t296
        integer t30
        real t301
        real t31
        integer t321
        real t342
        integer t37
        real t38
        real t4
        real t41
        integer t43
        real t44
        integer t5
        real t6
        integer t60
        real t61
        real t64
        real t67
        real t70
        real t71
        real t74
        real t77
        real t80
        real t81
        real t84
        real t89
        real t9
        real t90
        real t92
        real t96
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
        t14 = (t12 - t2) * t9
        t17 = i + 2
        t24 = j + 1
        t25 = u(t5,t24,k,n)
        t28 = 0.1E1 / dy
        t30 = j - 1
        t31 = u(t5,t30,k,n)
        t37 = k + 1
        t38 = u(t5,j,t37,n)
        t41 = 0.1E1 / dz
        t43 = k - 1
        t44 = u(t5,j,t43,n)
        t60 = i - 1
        t61 = u(t60,j,k,n)
        t64 = t4 * (t1 - t61) * t9
        t67 = u(i,t24,k,n)
        t70 = t4 * (t67 - t1) * t28
        t71 = u(i,t30,k,n)
        t74 = t4 * (t1 - t71) * t28
        t77 = u(i,j,t37,n)
        t80 = t4 * (t77 - t1) * t41
        t81 = u(i,j,t43,n)
        t84 = t4 * (t1 - t81) * t41
        t89 = dt * ((t10 - t64) * t9 + (t70 - t74) * t28 + (t80 - t84) *
     # t41) / 0.2E1
        t90 = ut(t60,j,k,n)
        t92 = (t2 - t90) * t9
        t96 = dx * (t14 / 0.2E1 + t92 / 0.2E1) / 0.2E1
        t99 = sqrt(0.4E1)
        t102 = t10 + t11 * t14 / 0.2E1 + cc * (t12 + dt * ((t4 * (u(t17,
     #j,k,n) - t6) * t9 - t10) * t9 + (t4 * (t25 - t6) * t28 - t4 * (t6 
     #- t31) * t28) * t28 + (t4 * (t38 - t6) * t41 - t4 * (t6 - t44) * t
     #41) * t41) / 0.2E1 - dx * ((ut(t17,j,k,n) - t12) * t9 / 0.2E1 + t1
     #4 / 0.2E1) / 0.2E1 - t2 - t89 - t96) * t99 / 0.4E1
        t103 = dt ** 2
        t107 = i - 2
        t114 = u(t60,t24,k,n)
        t118 = u(t60,t30,k,n)
        t124 = u(t60,j,t37,n)
        t128 = u(t60,j,t43,n)
        t148 = t64 + t11 * t92 / 0.2E1 + cc * (t2 + t89 - t96 - t90 - dt
     # * ((t64 - t4 * (t61 - u(t107,j,k,n)) * t9) * t9 + (t4 * (t114 - t
     #61) * t28 - t4 * (t61 - t118) * t28) * t28 + (t4 * (t124 - t61) * 
     #t41 - t4 * (t61 - t128) * t41) * t41) / 0.2E1 - dx * (t92 / 0.2E1 
     #+ (t90 - ut(t107,j,k,n)) * t9 / 0.2E1) / 0.2E1) * t99 / 0.4E1
        t153 = ut(i,t24,k,n)
        t155 = (t153 - t2) * t28
        t166 = j + 2
        t173 = u(i,t24,t37,n)
        t177 = u(i,t24,t43,n)
        t193 = ut(i,t30,k,n)
        t195 = (t2 - t193) * t28
        t199 = dy * (t155 / 0.2E1 + t195 / 0.2E1) / 0.2E1
        t204 = t70 + t11 * t155 / 0.2E1 + cc * (t153 + dt * ((t4 * (t25 
     #- t67) * t9 - t4 * (t67 - t114) * t9) * t9 + (t4 * (u(i,t166,k,n) 
     #- t67) * t28 - t70) * t28 + (t4 * (t173 - t67) * t41 - t4 * (t67 -
     # t177) * t41) * t41) / 0.2E1 - dy * ((ut(i,t166,k,n) - t153) * t28
     # / 0.2E1 + t155 / 0.2E1) / 0.2E1 - t2 - t89 - t199) * t99 / 0.4E1
        t216 = j - 2
        t223 = u(i,t30,t37,n)
        t227 = u(i,t30,t43,n)
        t247 = t74 + t11 * t195 / 0.2E1 + cc * (t2 + t89 - t199 - t193 -
     # dt * ((t4 * (t31 - t71) * t9 - t4 * (t71 - t118) * t9) * t9 + (t7
     #4 - t4 * (t71 - u(i,t216,k,n)) * t28) * t28 + (t4 * (t223 - t71) *
     # t41 - t4 * (t71 - t227) * t41) * t41) / 0.2E1 - dy * (t195 / 0.2E
     #1 + (t193 - ut(i,t216,k,n)) * t28 / 0.2E1) / 0.2E1) * t99 / 0.4E1
        t252 = ut(i,j,t37,n)
        t254 = (t252 - t2) * t41
        t273 = k + 2
        t290 = ut(i,j,t43,n)
        t292 = (t2 - t290) * t41
        t296 = dz * (t254 / 0.2E1 + t292 / 0.2E1) / 0.2E1
        t301 = t80 + t11 * t254 / 0.2E1 + cc * (t252 + dt * ((t4 * (t38 
     #- t77) * t9 - t4 * (t77 - t124) * t9) * t9 + (t4 * (t173 - t77) * 
     #t28 - t4 * (t77 - t223) * t28) * t28 + (t4 * (u(i,j,t273,n) - t77)
     # * t41 - t80) * t41) / 0.2E1 - dz * ((ut(i,j,t273,n) - t252) * t41
     # / 0.2E1 + t254 / 0.2E1) / 0.2E1 - t2 - t89 - t296) * t99 / 0.4E1
        t321 = k - 2
        t342 = t84 + t11 * t292 / 0.2E1 + cc * (t2 + t89 - t296 - t290 -
     # dt * ((t4 * (t44 - t81) * t9 - t4 * (t81 - t128) * t9) * t9 + (t4
     # * (t177 - t81) * t28 - t4 * (t81 - t227) * t28) * t28 + (t84 - t4
     # * (t81 - u(i,j,t321,n)) * t41) * t41) / 0.2E1 - dz * (t292 / 0.2E
     #1 + (t290 - ut(i,j,t321,n)) * t41 / 0.2E1) / 0.2E1) * t99 / 0.4E1

        unew(i,j,k) = t1 + dt * t2 + (t102 * t103 / 0.2E1 - t148 * t1
     #03 / 0.2E1) * t9 + (t204 * t103 / 0.2E1 - t247 * t103 / 0.2E1) * t
     #28 + (t301 * t103 / 0.2E1 - t342 * t103 / 0.2E1) * t41

        utnew(i,j,k) = t2 + (
     #t102 * dt - t148 * dt) * t9 + (t204 * dt - t247 * dt) * t28 + (t30
     #1 * dt - t342 * dt) * t41
        
c        blah = array(int(t1 + dt * t2 + (t102 * t103 / 0.2E1 - t148 * t1
c     #03 / 0.2E1) * t9 + (t204 * t103 / 0.2E1 - t247 * t103 / 0.2E1) * t
c     #28 + (t301 * t103 / 0.2E1 - t342 * t103 / 0.2E1) * t41),int(t2 + (
c     #t102 * dt - t148 * dt) * t9 + (t204 * dt - t247 * dt) * t28 + (t30
c     #1 * dt - t342 * dt) * t41))

        return
      end
