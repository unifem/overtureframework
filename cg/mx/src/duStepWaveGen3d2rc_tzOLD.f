      subroutine duStepWaveGen3d2rc_tzOLD( 
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
        real t101
        real t104
        real t105
        integer t109
        real t11
        real t116
        real t12
        real t120
        real t126
        real t130
        real t14
        real t151
        real t156
        real t158
        integer t169
        integer t17
        real t176
        real t180
        real t197
        real t199
        real t2
        real t203
        real t208
        integer t220
        real t227
        real t231
        integer t24
        real t25
        real t252
        real t257
        real t259
        integer t278
        real t28
        real t296
        real t298
        integer t30
        real t302
        real t307
        real t31
        integer t327
        real t349
        real t355
        integer t37
        real t38
        real t4
        real t41
        integer t43
        real t44
        integer t5
        real t6
        integer t61
        real t62
        real t65
        real t68
        real t71
        real t72
        real t75
        real t78
        real t81
        real t82
        real t85
        real t9
        real t91
        real t92
        real t94
        real t98
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
        t61 = i - 1
        t62 = u(t61,j,k,n)
        t65 = t4 * (t1 - t62) * t9
        t68 = u(i,t24,k,n)
        t71 = t4 * (t68 - t1) * t28
        t72 = u(i,t30,k,n)
        t75 = t4 * (t1 - t72) * t28
        t78 = u(i,j,t37,n)
        t81 = t4 * (t78 - t1) * t41
        t82 = u(i,j,t43,n)
        t85 = t4 * (t1 - t82) * t41
        t91 = dt * ((t10 - t65) * t9 + (t71 - t75) * t28 + (t81 - t85) *
     # t41 + src(i,j,k,nComp,n)) / 0.2E1
        t92 = ut(t61,j,k,n)
        t94 = (t2 - t92) * t9
        t98 = dx * (t14 / 0.2E1 + t94 / 0.2E1) / 0.2E1
        t101 = sqrt(0.4E1)
        t104 = t10 + t11 * t14 / 0.2E1 + cc * (t12 + dt * ((t4 * (u(t17,
     #j,k,n) - t6) * t9 - t10) * t9 + (t4 * (t25 - t6) * t28 - t4 * (t6 
     #- t31) * t28) * t28 + (t4 * (t38 - t6) * t41 - t4 * (t6 - t44) * t
     #41) * t41 + src(t5,j,k,nComp,n)) / 0.2E1 - dx * ((ut(t17,j,k,n) - 
     #t12) * t9 / 0.2E1 + t14 / 0.2E1) / 0.2E1 - t2 - t91 - t98) * t101 
     #/ 0.4E1
        t105 = dt ** 2
        t109 = i - 2
        t116 = u(t61,t24,k,n)
        t120 = u(t61,t30,k,n)
        t126 = u(t61,j,t37,n)
        t130 = u(t61,j,t43,n)
        t151 = t65 + t11 * t94 / 0.2E1 + cc * (t2 + t91 - t98 - t92 - dt
     # * ((t65 - t4 * (t62 - u(t109,j,k,n)) * t9) * t9 + (t4 * (t116 - t
     #62) * t28 - t4 * (t62 - t120) * t28) * t28 + (t4 * (t126 - t62) * 
     #t41 - t4 * (t62 - t130) * t41) * t41 + src(t61,j,k,nComp,n)) / 0.2
     #E1 - dx * (t94 / 0.2E1 + (t92 - ut(t109,j,k,n)) * t9 / 0.2E1) / 0.
     #2E1) * t101 / 0.4E1
        t156 = ut(i,t24,k,n)
        t158 = (t156 - t2) * t28
        t169 = j + 2
        t176 = u(i,t24,t37,n)
        t180 = u(i,t24,t43,n)
        t197 = ut(i,t30,k,n)
        t199 = (t2 - t197) * t28
        t203 = dy * (t158 / 0.2E1 + t199 / 0.2E1) / 0.2E1
        t208 = t71 + t11 * t158 / 0.2E1 + cc * (t156 + dt * ((t4 * (t25 
     #- t68) * t9 - t4 * (t68 - t116) * t9) * t9 + (t4 * (u(i,t169,k,n) 
     #- t68) * t28 - t71) * t28 + (t4 * (t176 - t68) * t41 - t4 * (t68 -
     # t180) * t41) * t41 + src(i,t24,k,nComp,n)) / 0.2E1 - dy * ((ut(i,
     #t169,k,n) - t156) * t28 / 0.2E1 + t158 / 0.2E1) / 0.2E1 - t2 - t91
     # - t203) * t101 / 0.4E1
        t220 = j - 2
        t227 = u(i,t30,t37,n)
        t231 = u(i,t30,t43,n)
        t252 = t75 + t11 * t199 / 0.2E1 + cc * (t2 + t91 - t203 - t197 -
     # dt * ((t4 * (t31 - t72) * t9 - t4 * (t72 - t120) * t9) * t9 + (t7
     #5 - t4 * (t72 - u(i,t220,k,n)) * t28) * t28 + (t4 * (t227 - t72) *
     # t41 - t4 * (t72 - t231) * t41) * t41 + src(i,t30,k,nComp,n)) / 0.
     #2E1 - dy * (t199 / 0.2E1 + (t197 - ut(i,t220,k,n)) * t28 / 0.2E1) 
     #/ 0.2E1) * t101 / 0.4E1
        t257 = ut(i,j,t37,n)
        t259 = (t257 - t2) * t41
        t278 = k + 2
        t296 = ut(i,j,t43,n)
        t298 = (t2 - t296) * t41
        t302 = dz * (t259 / 0.2E1 + t298 / 0.2E1) / 0.2E1
        t307 = t81 + t11 * t259 / 0.2E1 + cc * (t257 + dt * ((t4 * (t38 
     #- t78) * t9 - t4 * (t78 - t126) * t9) * t9 + (t4 * (t176 - t78) * 
     #t28 - t4 * (t78 - t227) * t28) * t28 + (t4 * (u(i,j,t278,n) - t78)
     # * t41 - t81) * t41 + src(i,j,t37,nComp,n)) / 0.2E1 - dz * ((ut(i,
     #j,t278,n) - t257) * t41 / 0.2E1 + t259 / 0.2E1) / 0.2E1 - t2 - t91
     # - t302) * t101 / 0.4E1
        t327 = k - 2
        t349 = t85 + t11 * t298 / 0.2E1 + cc * (t2 + t91 - t302 - t296 -
     # dt * ((t4 * (t44 - t82) * t9 - t4 * (t82 - t130) * t9) * t9 + (t4
     # * (t180 - t82) * t28 - t4 * (t82 - t231) * t28) * t28 + (t85 - t4
     # * (t82 - u(i,j,t327,n)) * t41) * t41 + src(i,j,t43,nComp,n)) / 0.
     #2E1 - dz * (t298 / 0.2E1 + (t296 - ut(i,j,t327,n)) * t41 / 0.2E1) 
     #/ 0.2E1) * t101 / 0.4E1
        t355 = src(i,j,k,nComp,n + 1)

        unew(i,j,k) = t1 + dt * t2 + (t104 * t105 / 0.2E1 - t151 * t1
     #05 / 0.2E1) * t9 + (t208 * t105 / 0.2E1 - t252 * t105 / 0.2E1) * t
     #28 + (t307 * t105 / 0.2E1 - t349 * t105 / 0.2E1) * t41 + t355 * t1
     #05 / 0.2E1

        utnew(i,j,k) = 
     #t2 + (t104 * dt - t151 * dt) * t9 + (t208 * dt - t
     #252 * dt) * t28 + (t307 * dt - t349 * dt) * t41 + t355 * dt


c        blah = array(int(t1 + dt * t2 + (t104 * t105 / 0.2E1 - t151 * t1
c     #05 / 0.2E1) * t9 + (t208 * t105 / 0.2E1 - t252 * t105 / 0.2E1) * t
c     #28 + (t307 * t105 / 0.2E1 - t349 * t105 / 0.2E1) * t41 + t355 * t1
c     #05 / 0.2E1),int(t2 + (t104 * dt - t151 * dt) * t9 + (t208 * dt - t
c     #252 * dt) * t28 + (t307 * dt - t349 * dt) * t41 + t355 * dt))

        return
      end
