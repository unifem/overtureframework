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
        real t102
        real t11
        real t119
        real t12
        real t124
        real t129
        integer t139
        real t154
        real t159
        real t160
        real t162
        real t166
        real t167
        real t17
        integer t182
        real t19
        real t2
        integer t20
        real t200
        integer t27
        real t28
        real t31
        integer t33
        real t34
        real t4
        real t47
        real t49
        integer t5
        real t54
        integer t55
        real t56
        real t59
        real t6
        real t62
        real t65
        real t66
        real t69
        real t74
        real t75
        real t76
        real t78
        real t82
        real t83
        real t84
        real t9
        integer t91
        real t98
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
        t47 = cc * t2
        t49 = (-t47 + t17) * t9
        t54 = t47 / 0.2E1
        t55 = i - 1
        t56 = u(t55,j,n)
        t59 = t4 * (t1 - t56) * t9
        t62 = u(i,t27,n)
        t65 = t4 * (t62 - t1) * t31
        t66 = u(i,t33,n)
        t69 = t4 * (t1 - t66) * t31
        t74 = t19 * ((t10 - t59) * t9 + (t65 - t69) * t31) / 0.4E1
        t75 = ut(t55,j,n)
        t76 = cc * t75
        t78 = (t47 - t76) * t9
        t82 = dx * (t49 / 0.2E1 + t78 / 0.2E1) / 0.4E1
        t83 = t10 + t11 * (t12 - t2) * t9 / 0.2E1 + t17 / 0.2E1 + t19 * 
     #((t4 * (u(t20,j,n) - t6) * t9 - t10) * t9 + (t4 * (t28 - t6) * t31
     # - t4 * (t6 - t34) * t31) * t31) / 0.4E1 - dx * ((cc * ut(t20,j,n)
     # - t17) * t9 / 0.2E1 + t49 / 0.2E1) / 0.4E1 - t54 - t74 - t82
        t84 = dt ** 2
        t91 = i - 2
        t98 = u(t55,t27,n)
        t102 = u(t55,t33,n)
        t119 = t59 + t11 * (t2 - t75) * t9 / 0.2E1 + t54 + t74 - t82 - t
     #76 / 0.2E1 - t19 * ((t59 - t4 * (t56 - u(t91,j,n)) * t9) * t9 + (t
     #4 * (t98 - t56) * t31 - t4 * (t56 - t102) * t31) * t31) / 0.4E1 - 
     #dx * (t78 / 0.2E1 + (-cc * ut(t91,j,n) + t76) * t9 / 0.2E1) / 0.4E
     #1
        t124 = ut(i,t27,n)
        t129 = cc * t124
        t139 = j + 2
        t154 = (-t47 + t129) * t31
        t159 = ut(i,t33,n)
        t160 = cc * t159
        t162 = (t47 - t160) * t31
        t166 = dy * (t154 / 0.2E1 + t162 / 0.2E1) / 0.4E1
        t167 = t65 + t11 * (t124 - t2) * t31 / 0.2E1 + t129 / 0.2E1 + t1
     #9 * ((t4 * (t28 - t62) * t9 - t4 * (t62 - t98) * t9) * t9 + (t4 * 
     #(u(i,t139,n) - t62) * t31 - t65) * t31) / 0.4E1 - dy * ((cc * ut(i
     #,t139,n) - t129) * t31 / 0.2E1 + t154 / 0.2E1) / 0.4E1 - t54 - t74
     # - t166
        t182 = j - 2
        t200 = t69 + t11 * (t2 - t159) * t31 / 0.2E1 + t54 + t74 - t166 
     #- t160 / 0.2E1 - t19 * ((t4 * (t34 - t66) * t9 - t4 * (t66 - t102)
     # * t9) * t9 + (t69 - t4 * (t66 - u(i,t182,n)) * t31) * t31) / 0.4E
     #1 - dy * (t162 / 0.2E1 + (-cc * ut(i,t182,n) + t160) * t31 / 0.2E1
     #) / 0.4E1


        unew(i,j) = t1 + dt * t2 + (-t119 * t84 / 0.2E1 + t83 * t84
     # / 0.2E1) * t9 + (t167 * t84 / 0.2E1 - t200 * t84 / 0.2E1) * t31
        
        utnew(i,j) = 
     #t2 + (-dt * t119 + dt * t83) * t9 + (dt * t167 - dt * t200) * 
     #t31

        return
      end
