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
        real t100
        real t104
        real t11
        real t12
        real t122
        real t127
        real t132
        integer t142
        real t158
        real t163
        real t164
        real t166
        real t17
        real t170
        real t171
        integer t186
        real t19
        real t2
        integer t20
        real t205
        real t211
        integer t27
        real t28
        real t31
        integer t33
        real t34
        real t4
        real t48
        integer t5
        real t50
        real t55
        integer t56
        real t57
        real t6
        real t60
        real t63
        real t66
        real t67
        real t70
        real t76
        real t77
        real t78
        real t80
        real t84
        real t85
        real t86
        real t9
        integer t93
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
        t48 = cc * t2
        t50 = (-t48 + t17) * t9
        t55 = t48 / 0.2E1
        t56 = i - 1
        t57 = u(t56,j,n)
        t60 = t4 * (t1 - t57) * t9
        t63 = u(i,t27,n)
        t66 = t4 * (t63 - t1) * t31
        t67 = u(i,t33,n)
        t70 = t4 * (t1 - t67) * t31
        t76 = t19 * ((t10 - t60) * t9 + (t66 - t70) * t31 + src(i,j,nCom
     #p,n)) / 0.4E1
        t77 = ut(t56,j,n)
        t78 = cc * t77
        t80 = (t48 - t78) * t9
        t84 = dx * (t50 / 0.2E1 + t80 / 0.2E1) / 0.4E1
        t85 = t10 + t11 * (t12 - t2) * t9 / 0.2E1 + t17 / 0.2E1 + t19 * 
     #((t4 * (u(t20,j,n) - t6) * t9 - t10) * t9 + (t4 * (t28 - t6) * t31
     # - t4 * (t6 - t34) * t31) * t31 + src(t5,j,nComp,n)) / 0.4E1 - dx 
     #* ((cc * ut(t20,j,n) - t17) * t9 / 0.2E1 + t50 / 0.2E1) / 0.4E1 - 
     #t55 - t76 - t84
        t86 = dt ** 2
        t93 = i - 2
        t100 = u(t56,t27,n)
        t104 = u(t56,t33,n)
        t122 = t60 + t11 * (t2 - t77) * t9 / 0.2E1 + t55 + t76 - t84 - t
     #78 / 0.2E1 - t19 * ((t60 - t4 * (t57 - u(t93,j,n)) * t9) * t9 + (t
     #4 * (t100 - t57) * t31 - t4 * (t57 - t104) * t31) * t31 + src(t56,
     #j,nComp,n)) / 0.4E1 - dx * (t80 / 0.2E1 + (-cc * ut(t93,j,n) + t78
     #) * t9 / 0.2E1) / 0.4E1
        t127 = ut(i,t27,n)
        t132 = cc * t127
        t142 = j + 2
        t158 = (-t48 + t132) * t31
        t163 = ut(i,t33,n)
        t164 = cc * t163
        t166 = (t48 - t164) * t31
        t170 = dy * (t158 / 0.2E1 + t166 / 0.2E1) / 0.4E1
        t171 = t66 + t11 * (t127 - t2) * t31 / 0.2E1 + t132 / 0.2E1 + t1
     #9 * ((t4 * (t28 - t63) * t9 - t4 * (t63 - t100) * t9) * t9 + (t4 *
     # (u(i,t142,n) - t63) * t31 - t66) * t31 + src(i,t27,nComp,n)) / 0.
     #4E1 - dy * ((cc * ut(i,t142,n) - t132) * t31 / 0.2E1 + t158 / 0.2E
     #1) / 0.4E1 - t55 - t76 - t170
        t186 = j - 2
        t205 = t70 + t11 * (t2 - t163) * t31 / 0.2E1 + t55 + t76 - t170 
     #- t164 / 0.2E1 - t19 * ((t4 * (t34 - t67) * t9 - t4 * (t67 - t104)
     # * t9) * t9 + (t70 - t4 * (t67 - u(i,t186,n)) * t31) * t31 + src(i
     #,t33,nComp,n)) / 0.4E1 - dy * (t166 / 0.2E1 + (-cc * ut(i,t186,n) 
     #+ t164) * t31 / 0.2E1) / 0.4E1
        t211 = src(i,j,nComp,n + 1)
        
        unew(i,j) = t1 + dt * t2 + (-t122 * t86 / 0.2E1 + t85 * t86
     # / 0.2E1) * t9 + (t171 * t86 / 0.2E1 - t205 * t86 / 0.2E1) * t31 +
     # t211 * t86 / 0.2E1

        utnew(i,j) = t2 + (-dt * t122 + dt * t85) * t9 + (dt *
     # t171 - dt * t205) * t31 + t211 * dt

        return
      end
