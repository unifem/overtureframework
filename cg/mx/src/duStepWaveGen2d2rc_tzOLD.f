      subroutine duStepWaveGen2d2rc_tzOLD( 
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
        real t11
        real t118
        real t12
        real t123
        real t125
        integer t136
        real t14
        real t154
        real t156
        real t160
        real t165
        integer t17
        integer t177
        real t199
        real t2
        real t205
        integer t24
        real t25
        real t28
        integer t30
        real t31
        real t4
        integer t48
        real t49
        integer t5
        real t52
        real t55
        real t58
        real t59
        real t6
        real t62
        real t68
        real t69
        real t71
        real t75
        real t78
        real t81
        real t82
        integer t86
        real t9
        real t93
        real t97

        t1 = u(i,j,n)
        t2 = ut(i,j,n)
        t4 = cc ** 2
        t5 = i + 1
        t6 = u(t5,j,n)
        t9 = 0.1E1 / dx
        t10 = t4 * (t6 - t1) * t9
        t11 = t4 * dt
        t12 = ut(t5,j,n)
        t14 = (t12 - t2) * t9
        t17 = i + 2
        t24 = j + 1
        t25 = u(t5,t24,n)
        t28 = 0.1E1 / dy
        t30 = j - 1
        t31 = u(t5,t30,n)
        t48 = i - 1
        t49 = u(t48,j,n)
        t52 = t4 * (t1 - t49) * t9
        t55 = u(i,t24,n)
        t58 = t4 * (t55 - t1) * t28
        t59 = u(i,t30,n)
        t62 = t4 * (t1 - t59) * t28
        t68 = dt * ((t10 - t52) * t9 + (t58 - t62) * t28 + src(i,j,nComp
     #,n)) / 0.2E1
        t69 = ut(t48,j,n)
        t71 = (t2 - t69) * t9
        t75 = dx * (t14 / 0.2E1 + t71 / 0.2E1) / 0.2E1
        t78 = sqrt(0.4E1)
        t81 = t10 + t11 * t14 / 0.2E1 + cc * (t12 + dt * ((t4 * (u(t17,j
     #,n) - t6) * t9 - t10) * t9 + (t4 * (t25 - t6) * t28 - t4 * (t6 - t
     #31) * t28) * t28 + src(t5,j,nComp,n)) / 0.2E1 - dx * ((ut(t17,j,n)
     # - t12) * t9 / 0.2E1 + t14 / 0.2E1) / 0.2E1 - t2 - t68 - t75) * t7
     #8 / 0.4E1
        t82 = dt ** 2
        t86 = i - 2
        t93 = u(t48,t24,n)
        t97 = u(t48,t30,n)
        t118 = t52 + t11 * t71 / 0.2E1 + cc * (t2 + t68 - t75 - t69 - dt
     # * ((t52 - t4 * (t49 - u(t86,j,n)) * t9) * t9 + (t4 * (t93 - t49) 
     #* t28 - t4 * (t49 - t97) * t28) * t28 + src(t48,j,nComp,n)) / 0.2E
     #1 - dx * (t71 / 0.2E1 + (t69 - ut(t86,j,n)) * t9 / 0.2E1) / 0.2E1)
     # * t78 / 0.4E1
        t123 = ut(i,t24,n)
        t125 = (t123 - t2) * t28
        t136 = j + 2
        t154 = ut(i,t30,n)
        t156 = (t2 - t154) * t28
        t160 = dy * (t125 / 0.2E1 + t156 / 0.2E1) / 0.2E1
        t165 = t58 + t11 * t125 / 0.2E1 + cc * (t123 + dt * ((t4 * (t25 
     #- t55) * t9 - t4 * (t55 - t93) * t9) * t9 + (t4 * (u(i,t136,n) - t
     #55) * t28 - t58) * t28 + src(i,t24,nComp,n)) / 0.2E1 - dy * ((ut(i
     #,t136,n) - t123) * t28 / 0.2E1 + t125 / 0.2E1) / 0.2E1 - t2 - t68 
     #- t160) * t78 / 0.4E1
        t177 = j - 2
        t199 = t62 + t11 * t156 / 0.2E1 + cc * (t2 + t68 - t160 - t154 -
     # dt * ((t4 * (t31 - t59) * t9 - t4 * (t59 - t97) * t9) * t9 + (t62
     # - t4 * (t59 - u(i,t177,n)) * t28) * t28 + src(i,t30,nComp,n)) / 0
     #.2E1 - dy * (t156 / 0.2E1 + (t154 - ut(i,t177,n)) * t28 / 0.2E1) /
     # 0.2E1) * t78 / 0.4E1
        t205 = src(i,j,nComp,n + 1)

        unew(i,j) = t1 + dt * t2 + (t81 * t82 / 0.2E1 - t118 * t82 
     #/ 0.2E1) * t9 + (t165 * t82 / 0.2E1 - t199 * t82 / 0.2E1) * t28 + 
     #t205 * t82 / 0.2E1

        utnew(i,j) = t2 + (t81 * dt - t118 * dt) * t9 + (t165 *
     # dt - t199 * dt) * t28 + t205 * dt

c        blah = array(int(t1 + dt * t2 + (t81 * t82 / 0.2E1 - t118 * t82 
c     #/ 0.2E1) * t9 + (t165 * t82 / 0.2E1 - t199 * t82 / 0.2E1) * t28 + 
c     #t205 * t82 / 0.2E1),int(t2 + (t81 * dt - t118 * dt) * t9 + (t165 *
c     # dt - t199 * dt) * t28 + t205 * dt))

        return
      end
