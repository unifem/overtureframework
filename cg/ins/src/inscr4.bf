c ************* NEW version *****************

      subroutine inscr4( kd1,ks1,kd2,ks2,nd,gridIndexRange,bc,
     & ndra,ndrb,ndsa,ndsb,ndta,ndtb,ipar,rpar, u,t,d14,d24,ajs,xy,rsxy,gridType )
c======================================================================
c      Get Values for u outside corners in 2D or Edges in 3D
c
c  Input -
c   (kd1,ks1),(kd2,ks2) : defines the corner or edge
c    u :
c
c NOTE: This approximation is 4th order accurate but NOT exact for 4th degree polynomials
c NEW NOTE: the new version is exact for 4th degree polynomials
c
c  Corners are labelled (in 2d) as (kd,ks)=
c
c           (1,2)          (2,2)
c                +--------+
c                |        |
c                |        |
c                +--------+
c           (1,1)          (2,1)
c
c  To get the value at the corner use:
c u(r,s) = u(0) + r*u.r(0) + s*u.s(0) + .5*r**2*u.rr(0) + ...
c  which implies
c u(r,s)+u(-r,-s) = 2u(0) + r**2*u.rr+2r*s*u.rs(0)+s**2*u.ss(0)+O(h**4)
c  At a corner we know u(0), and all non mixed derivatives, u.r, u.s,
c u.rr, u.ss, ...
c   To get u.rs and v.rs we use u.x+v.y=0 and (u.x+v.y).r=0 and
c   (u.x+v.y).s=0, which gives
c
c (r.y*s.x-r.x*s.y)u.rs + r.y*r.x*u.rr - s.y*s.x*u.ss
c       +r.y**2*v.rr - s.y**2*v.ss + r.y a_1 - s.y a_2 = 0
c (r.x*s.y-r.y*s.x)v.rs + r.x*r.x*u.rr - s.x*s.x*u.ss
c       +r.x*r.y*v.rr - s.x*s.y*v.ss + r.x a_1 - s.x a_2 = 0
c  a_1 = (r.x).r*u.r + (s.x).r*u.s + (r.y).r*v.r + (s.y).r*v.s
c  a_2 = (r.x).s*u.r + (s.x).s*u.s + (r.y).s*v.r + (s.y).s*v.s
c
c
c  In 3D, for an edge parallel to "t", we use
c
c u(r,s)+u(-r,-s) = 2u(0) + r**2*u.rr+2r*s*u.rs(0)+s**2*u.ss(0)+O(h**4)
c
c and to get (u,v,w)_rs we use
c
c       (u_x+v_y+w_z)_r=0
c       (u_x+v_y+w_z)_s=0
c       Extrapolate( (t_1,t_2,t_3).(u,v,w))
c where (t_1,t_2,t_3) is the tangent to the edge.
c
c
c======================================================================
      implicit none
      integer kd1,ks1,kd2,ks2,nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb
      real t,ajs
      real u(ndra:ndrb,ndsa:ndsb,ndta:ndtb,0:*),d14(3),d24(3),
     &    xy(ndra:ndrb,ndsa:ndsb,ndta:ndtb,nd),
     &  rsxy(ndra:ndrb,ndsa:ndsb,ndta:ndtb,nd,nd)
      integer gridIndexRange(2,3), bc(2,3),gridType
      integer ipar(0:*)
      real rpar(0:*)
c.......local
      real vr(3,3),vrr(3,3,3),drs(3)
      integer iv(3),is(3)
      logical period,oldway
      integer nrsab,nrs,numberOfProcessors
      integer kd,kdd,kd3,i1,i2,i3,is1,is2,is3,kdn,ks,j1,j2,j3,
     & i11,i12,i21,i22,i31,i32,js3

      real uc,uc0,uc33,uc32,uc31,ubr,ubs,ubt,ubrr,ubss,ubtt,ubrs,ubrt,
     & ubst,uv3,rx,ry,rz,sx,sy,sz,tx,ty,tz
      real taylor2d1,taylor2d2,uc2d11,uc2d21,uc2d12,uc2d22
      real taylor3d3e1,taylor3d3e2,taylor3d2e1,taylor3d2e2,
     &     taylor3d1e1,taylor3d1e2,uc3d3e11,uc3d3e22,
     &     uc3d2e11,uc3d2e22,uc3d1e11,uc3d1e22,
     &     taylor3d1,taylor3d2,uc3d111,uc3d222

c.......start statement functions
c       equation to get values outside corners:
c       corner = (i1,i2), point=(i1-is1,i2-is2)
      nrsab(kd,ks) = gridIndexRange(ks,kd)
      nrs(kd,ks)=gridIndexRange(ks,kd)

      period(kd) = bc(1,kd).lt.0

      uc(is1,is2,i1,i2,i3,kd)=
     & 2.*u(i1,i2,i3,kd)-u(i1+is1,i2+is2,i3,kd)
     &            +(is1*drs(1))**2*vrr(kd,1,1)
     & +2.*(is1*drs(1)*is2*drs(2))*vrr(kd,1,2)
     &            +(is2*drs(2))**2*vrr(kd,2,2)

c Here are more accurate expressions for 2D -- exact for 4th order polys
      taylor2d1(is1,is2,i1,i2,i3,kd)=(is1*drs(1))*vr(kd,1)
     &                       +(is2*drs(2))*vr(kd,2)
      taylor2d2(is1,is2,i1,i2,i3,kd)=.5*(is1*drs(1))**2*vrr(kd,1,1)
     &                  +(is1*drs(1)*is2*drs(2))*vrr(kd,1,2)
     &                       +.5*(is2*drs(2))**2*vrr(kd,2,2)

      uc2d11(is1,is2,i1,i2,i3,kd)= ! for u(-1,-1) 
     &   3.75*u(i1,i2,i3,kd)  
     &    -3.*u(i1+is1,i2+is2,i3,kd)
     &   +.25*u(i1+2*is1,i2+2*is2,i3,kd)
     &  +1.5*taylor2d1(is1,is2,i1,i2,i3,kd)
     &  + 3.*taylor2d2(is1,is2,i1,i2,i3,kd)

      uc2d21(is1,is2,i1,i2,i3,kd)=uc2d11(2*is1,  is2,i1,i2,i3,kd) ! for u(-2,-1) 
      uc2d12(is1,is2,i1,i2,i3,kd)=uc2d11(  is1,2*is2,i1,i2,i3,kd) ! for u(-2,-1) 

      uc2d22(is1,is2,i1,i2,i3,kd)= ! for u(-2,-2) 
     &    30.*u(i1,i2,i3,kd)  
     &   -32.*u(i1+is1,i2+is2,i3,kd)
     &    +3.*u(i1+2*is1,i2+2*is2,i3,kd)
     &  +24.*taylor2d1(is1,is2,i1,i2,i3,kd)
     &  +24.*taylor2d2(is1,is2,i1,i2,i3,kd)

      uc0(is1,is2,i1,i2,i3,kd)=
     & 2.*u(i1,i2,i3,kd)-u(i1+is1,i2+is2,i3,kd)

c     --- old 3d ----
      uc33(is1,is2,i1,i2,i3,kd)=
     & 2.*u(i1,i2,i3,kd)-u(i1+is1,i2+is2,i3,kd)
     &            +(is1*drs(1))**2*vrr(kd,1,1)
     & +2.*(is1*drs(1)*is2*drs(2))*vrr(kd,1,2)
     &            +(is2*drs(2))**2*vrr(kd,2,2)
      uc32(is1,is3,i1,i2,i3,kd)=
     & 2.*u(i1,i2,i3,kd)-u(i1+is1,i2,i3+is3,kd)
     &            +(is1*drs(1))**2*vrr(kd,1,1)
     & +2.*(is1*drs(1)*is3*drs(3))*vrr(kd,1,3)
     &            +(is3*drs(3))**2*vrr(kd,3,3)
      uc31(is2,is3,i1,i2,i3,kd)=
     & 2.*u(i1,i2,i3,kd)-u(i1,i2+is2,i3+is3,kd)
     &            +(is2*drs(2))**2*vrr(kd,2,2)
     & +2.*(is2*drs(2)*is3*drs(3))*vrr(kd,2,3)
     &            +(is3*drs(3))**2*vrr(kd,3,3)

c  Here are more accurate expressions for 3D -- exact for 4th order polynomials
      taylor3d3e1(is1,is2,i1,i2,i3,kd)=       ! 3d, edge along direction 3, 1st derivative term in Taylor series
     &     (is1*drs(1))*vr(kd,1)
     &    +(is2*drs(2))*vr(kd,2)              
      taylor3d3e2(is1,is2,i1,i2,i3,kd)=       ! 3d, edge along direction 3, 2nd derivative term in Taylor series
     &          .5*(is1*drs(1))**2*vrr(kd,1,1)
     &    +(is1*drs(1)*is2*drs(2))*vrr(kd,1,2)
     &         +.5*(is2*drs(2))**2*vrr(kd,2,2)

      taylor3d2e1(is1,is3,i1,i2,i3,kd)=       ! 3d, edge along direction 2, 1st derivative term in Taylor series
     &     (is1*drs(1))*vr(kd,1)
     &    +(is3*drs(3))*vr(kd,3)              
      taylor3d2e2(is1,is3,i1,i2,i3,kd)=       ! 3d, edge along direction 2, 2nd derivative term in Taylor series
     &          .5*(is1*drs(1))**2*vrr(kd,1,1)
     &    +(is1*drs(1)*is3*drs(3))*vrr(kd,1,3)
     &         +.5*(is3*drs(3))**2*vrr(kd,3,3)

      taylor3d1e1(is2,is3,i1,i2,i3,kd)=       ! 3d, edge along direction 1, 1st derivative term in Taylor series
     &     (is2*drs(2))*vr(kd,2)
     &    +(is3*drs(3))*vr(kd,3)              
      taylor3d1e2(is2,is3,i1,i2,i3,kd)=       ! 3d, edge along direction 1, 2nd derivative term in Taylor series
     &          .5*(is2*drs(2))**2*vrr(kd,2,2)
     &    +(is2*drs(2)*is3*drs(3))*vrr(kd,2,3)
     &         +.5*(is3*drs(3))**2*vrr(kd,3,3)

      uc3d3e11(is1,is2,i1,i2,i3,kd)=           !  3d, edge along direction 3,for u(-1,-1,*) 
     &   3.75*u(i1,i2,i3,kd)  
     &    -3.*u(i1+is1,i2+is2,i3,kd)
     &   +.25*u(i1+2*is1,i2+2*is2,i3,kd)
     &  +1.5*taylor3d3e1(is1,is2,i1,i2,i3,kd)
     &  + 3.*taylor3d3e2(is1,is2,i1,i2,i3,kd)

      uc3d3e22(is1,is2,i1,i2,i3,kd)=           ! 3d, edge along direction 3, for u(-2,-2,*) 
     &    30.*u(i1,i2,i3,kd)  
     &   -32.*u(i1+is1,i2+is2,i3,kd)
     &    +3.*u(i1+2*is1,i2+2*is2,i3,kd)
     &  +24.*taylor3d3e1(is1,is2,i1,i2,i3,kd)
     &  +24.*taylor3d3e2(is1,is2,i1,i2,i3,kd)

      uc3d2e11(is1,is3,i1,i2,i3,kd)=           !  3d, edge along direction 2,for u(-1,*,-1) 
     &   3.75*u(i1,i2,i3,kd)  
     &    -3.*u(i1+is1,i2,i3+is3,kd)
     &   +.25*u(i1+2*is1,i2,i3+2*is3,kd)
     &  +1.5*taylor3d2e1(is1,is3,i1,i2,i3,kd)
     &  + 3.*taylor3d2e2(is1,is3,i1,i2,i3,kd)

      uc3d2e22(is1,is3,i1,i2,i3,kd)=           ! 3d, edge along direction 2, for u(-2,*,-2) 
     &    30.*u(i1,i2,i3,kd)  
     &   -32.*u(i1+is1,i2,i3+is3,kd)
     &    +3.*u(i1+2*is1,i2,i3+2*is3,kd)
     &  +24.*taylor3d2e1(is1,is3,i1,i2,i3,kd)
     &  +24.*taylor3d2e2(is1,is3,i1,i2,i3,kd)

      uc3d1e11(is2,is3,i1,i2,i3,kd)=           !  3d, edge along direction 1,for u(*,-1,-1) 
     &   3.75*u(i1,i2,i3,kd)  
     &    -3.*u(i1,i2+is2,i3+is3,kd)
     &   +.25*u(i1,i2+2*is2,i3+2*is3,kd)
     &  +1.5*taylor3d1e1(is2,is3,i1,i2,i3,kd)
     &  + 3.*taylor3d1e2(is2,is3,i1,i2,i3,kd)

      uc3d1e22(is2,is3,i1,i2,i3,kd)=           ! 3d, edge along direction 1, for u(*,-2,-2) 
     &    30.*u(i1,i2,i3,kd)  
     &   -32.*u(i1,i2+is2,i3+is3,kd)
     &    +3.*u(i1,i2+2*is2,i3+2*is3,kd)
     &  +24.*taylor3d1e1(is2,is3,i1,i2,i3,kd)
     &  +24.*taylor3d1e2(is2,is3,i1,i2,i3,kd)




c.......parametric derivatives on the boundary used by uv3(is1,is2,...)
      ubr(i1,i2,i3,kd)=(8.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))
     &                    -(u(i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd)))*d14(1)
      ubs(i1,i2,i3,kd)=(8.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))
     &                    -(u(i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd)))*d14(2)
      ubt(i1,i2,i3,kd)=(8.*(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))
     &                    -(u(i1,i2,i3+2,kd)-u(i1,i2,i3-2,kd)))*d14(3)
      ubrr(i1,i2,i3,kd)=
     & ( -30.*u(i1,i2,i3,kd)
     &  +16.*(u(i1+1,i2,i3,kd)+u(i1-1,i2,i3,kd))
     &      -(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)) )*d24(1)
      ubss(i1,i2,i3,kd)=
     &+( -30.*u(i1,i2,i3,kd)
     &  +16.*(u(i1,i2+1,i3,kd)+u(i1,i2-1,i3,kd))
     &      -(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )*d24(2)
      ubtt(i1,i2,i3,kd)=
     &+( -30.*u(i1,i2,i3,kd)
     &  +16.*(u(i1,i2,i3+1,kd)+u(i1,i2,i3-1,kd))
     &      -(u(i1,i2,i3+2,kd)+u(i1,i2,i3-2,kd)) )*d24(3)
      ubrs(i1,i2,i3,kd)=
     &   (8.*(ubs(i1+1,i2,i3,kd)-ubs(i1-1,i2,i3,kd))
     &      -(ubs(i1+2,i2,i3,kd)-ubs(i1-2,i2,i3,kd)))*d14(1)
      ubrt(i1,i2,i3,kd)=
     &   (8.*(ubt(i1+1,i2,i3,kd)-ubt(i1-1,i2,i3,kd))
     &      -(ubt(i1+2,i2,i3,kd)-ubt(i1-2,i2,i3,kd)))*d14(1)
      ubst(i1,i2,i3,kd)=
     &   (8.*(ubt(i1,i2+1,i3,kd)-ubt(i1,i2-1,i3,kd))
     &      -(ubt(i1,i2+2,i3,kd)-ubt(i1,i2-2,i3,kd)))*d14(2)
c.........................................................
c        Values outside of a vertex in 3D:
c.........................................................
c    ** old **
      uv3(is1,is2,is3,i1,i2,i3,kd)=
     & 2.*u(i1,i2,i3,kd)-u(i1+is1,i2+is2,i3+is3,kd)
     &            +(is1*drs(1))**2*ubrr(i1,i2,i3,kd)
     &            +(is2*drs(2))**2*ubss(i1,i2,i3,kd)
     &            +(is3*drs(3))**2*ubtt(i1,i2,i3,kd)
     & +2.*(is1*drs(1)*is2*drs(2))*ubrs(i1,i2,i3,kd)
     & +2.*(is1*drs(1)*is3*drs(3))*ubrt(i1,i2,i3,kd)
     & +2.*(is2*drs(2)*is3*drs(3))*ubst(i1,i2,i3,kd)

c   ** new **
      taylor3d1(is1,is2,is3,i1,i2,i3,kd)=       ! 3d, full 1st derivative term in Taylor series
     &     (is1*drs(1))*ubr(i1,i2,i3,kd)
     &    +(is2*drs(2))*ubs(i1,i2,i3,kd)
     &    +(is3*drs(3))*ubt(i1,i2,i3,kd)
      taylor3d2(is1,is2,is3,i1,i2,i3,kd)=           ! 3d, full 2nd derivative in Taylor series
     &          .5*(is1*drs(1))**2*ubrr(i1,i2,i3,kd)
     &         +.5*(is2*drs(2))**2*ubss(i1,i2,i3,kd)
     &         +.5*(is3*drs(3))**2*ubtt(i1,i2,i3,kd)
     &    +(is1*drs(1)*is2*drs(2))*ubrs(i1,i2,i3,kd)
     &    +(is1*drs(1)*is3*drs(3))*ubrt(i1,i2,i3,kd)
     &    +(is2*drs(2)*is3*drs(3))*ubst(i1,i2,i3,kd)

      uc3d111(is1,is2,is3,i1,i2,i3,kd)= ! for u(-1,-1,-1), u(-2,-1,-1), u(-1,-2,-1), u(-1,-1,-2)
     &   3.75*u(i1,i2,i3,kd)  
     &    -3.*u(i1+is1,i2+is2,i3+is3,kd)
     &   +.25*u(i1+2*is1,i2+2*is2,i3+2*is3,kd)
     &  +1.5*taylor3d1(is1,is2,is3,i1,i2,i3,kd)
     &  + 3.*taylor3d2(is1,is2,is3,i1,i2,i3,kd)

      uc3d222(is1,is2,is3,i1,i2,i3,kd)= !   3d for u(-2,-2,-2) 
     &    30.*u(i1,i2,i3,kd)  
     &   -32.*u(i1+is1,i2+is2,i3+is3,kd)
     &    +3.*u(i1+2*is1,i2+2*is2,i3+2*is3,kd)
     &  +24.*taylor3d1(is1,is2,is3,i1,i2,i3,kd)
     &  +24.*taylor3d2(is1,is2,is3,i1,i2,i3,kd)


c.......end  statement functions
c........Interpolate corners of u
c  (i1,i2) is the corner
c  (is1,is2) is in the normal direction into the domain
c
c                |  |  |
c                |  |  |
c                |  |  X------
c                |  +------        X=(i1,i2)
c                +---------
c

      kd3=min(nd,3)

      oldway=.false. ! .true.

      numberOfProcessors=ipar(17)

c*** do this some where else ****
      do kdd=1,nd
c**        drs(kdd)=1./(nrsab(kdd,2)-nrsab(kdd,1))
        drs(kdd)=1./(12.*d14(kdd))
      end do

      if( nd.eq.2 )then
c         here we assume (kd1,kd2)=(1,2)
        i1=nrsab(kd1,ks1)
        i2=nrsab(kd2,ks2)
        i3=nrsab(3,1)
        is1=3-2*ks1
        is2=3-2*ks2
c       ...get derivatives at corner
c          u.r,u.s,u.t;  u.rr, u.ss, u.tt, u.rs
          call insbv4( i1,i2,i3,kd1,kd2,kdn,is1,is2,is3,t,vr,vrr,
     &     d14,d24,drs,nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb, xy,rsxy,u,gridType )
          do kdd=1,nd
             if( oldway )then
             u(i1-  is1,i2-  is2,i3,kdd)=uc(  is1,  is2,i1,i2,i3,kdd)
             u(i1-2*is1,i2-  is2,i3,kdd)=uc(2*is1,  is2,i1,i2,i3,kdd)
             u(i1-  is1,i2-2*is2,i3,kdd)=uc(  is1,2*is2,i1,i2,i3,kdd)
             u(i1-2*is1,i2-2*is2,i3,kdd)=uc(2*is1,2*is2,i1,i2,i3,kdd)
c           here is the new, more accurate way:
            else
            u(i1-  is1,i2-  is2,i3,kdd)=uc2d11(is1,is2,i1,i2,i3,kdd)
            u(i1-2*is1,i2-  is2,i3,kdd)=uc2d21(is1,is2,i1,i2,i3,kdd)
            u(i1-  is1,i2-2*is2,i3,kdd)=uc2d12(is1,is2,i1,i2,i3,kdd)
            u(i1-2*is1,i2-2*is2,i3,kdd)=uc2d22(is1,is2,i1,i2,i3,kdd)
            end if
          end do

      else
c       ************* 3D ************
        iv(1)=0
        iv(2)=0
        iv(3)=0
        iv(kd1)=nrsab(kd1,ks1)
        iv(kd2)=nrsab(kd2,ks2)
        if( kd1+kd2.eq.5 )then
          kdn=1
        elseif( kd1+kd2.eq.4 )then
          kdn=2
        else
          kdn=3
        end if
        is(kd1)=3-2*ks1
        is(kd2)=3-2*ks2
        is(kdn)=0
        i1=iv(1)
        i2=iv(2)
        i3=iv(3)
        is1=is(1)
        is2=is(2)
        is3=is(3)
*         write(*,*) 'INSCR: kdn,is1,is2,is3=',kdn,is1,is2,is3
        if( kdn.eq.3 )then
c           kdn=3 is the direction tangential to the edge
          do 320 i3=nrs(3,1),nrs(3,2)
c       ...get derivatives along an edge
c          u.r,u.s,u.t;  u.rr, u.ss, u.tt, u.rs
             call insbv4( i1,i2,i3,kd1,kd2,kdn,is1,is2,is3,t,
     &        vr,vrr,d14,d24,drs,nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,
     &        xy,rsxy,u,gridType )
            do 320 kdd=1,nd
              if( oldway )then
               u(i1-  is1,i2-  is2,i3,kdd)=uc33(  is1,  is2,i1,i2,i3,kdd)
               u(i1-2*is1,i2-  is2,i3,kdd)=uc33(2*is1,  is2,i1,i2,i3,kdd)
               u(i1-  is1,i2-2*is2,i3,kdd)=uc33(  is1,2*is2,i1,i2,i3,kdd)
               u(i1-2*is1,i2-2*is2,i3,kdd)=uc33(2*is1,2*is2,i1,i2,i3,kdd)
              else
c              new:
              u(i1-  is1,i2-  is2,i3,kdd)=
     &                    uc3d3e11(  is1,  is2,i1,i2,i3,kdd)
              u(i1-2*is1,i2-  is2,i3,kdd)=
     &                    uc3d3e11(2*is1,  is2,i1,i2,i3,kdd)
              u(i1-  is1,i2-2*is2,i3,kdd)=
     &                    uc3d3e11(  is1,2*is2,i1,i2,i3,kdd)
              u(i1-2*is1,i2-2*is2,i3,kdd)=
     &                    uc3d3e22(  is1,  is2,i1,i2,i3,kdd)
              end if
            continue
 320      continue
          if( period(3) )then
c           ...swap periodic edges
            if( numberOfProcessors.le.1 )then
             ! in parallel we do this in the calling routine
             i31=nrsab(3,1)
             i32=nrsab(3,2)
             do 340 j2=i2-2*is2,i2-is2,is2
              do 340 j1=i1-2*is1,i1-is1,is1
                do 340 kd=1,nd
                  u(j1,j2,i31-1,kd)=u(j1,j2,i32-1,kd)
                  u(j1,j2,i31-2,kd)=u(j1,j2,i32-2,kd)
                  u(j1,j2,i32  ,kd)=u(j1,j2,i31  ,kd)
                  u(j1,j2,i32+1,kd)=u(j1,j2,i31+1,kd)
                  u(j1,j2,i32+2,kd)=u(j1,j2,i31+1,kd)
                continue
              continue
 340         continue
            end if
          else
c
c           ...assign values outside vertices in 3D
c              use Taylor series (derivatives u.rr, u.rs ... are known)
c                u(-r)=2*u(0)-u(r)+ r**2u.rr+...
c
            do ks=1,2
              if( bc(ks,3).gt.0 )then
                i3=nrsab(3,ks)
                if( .not.oldway )then
                is3=3-2*ks
                do kdd=1,nd
                  u(i1-  is1,i2-  is2,i3-  is3,kdd)=
     &                    uc3d111(  is1,  is2,  is3,i1,i2,i3,kdd)
                  u(i1-2*is1,i2-  is2,i3-  is3,kdd)=
     &                    uc3d111(2*is1,  is2,  is3,i1,i2,i3,kdd)
                  u(i1-  is1,i2-2*is2,i3-  is3,kdd)=
     &                    uc3d111(  is1,2*is2,  is3,i1,i2,i3,kdd)
                  u(i1-  is1,i2-  is2,i3-2*is3,kdd)=
     &                    uc3d111(  is1,  is2,2*is3,i1,i2,i3,kdd)
                  u(i1-2*is1,i2-2*is2,i3-  is3,kdd)=
     &                    uc3d111(2*is1,2*is2,  is3,i1,i2,i3,kdd)
                  u(i1-  is1,i2-2*is2,i3-2*is3,kdd)=
     &                    uc3d111(  is1,2*is2,2*is3,i1,i2,i3,kdd)
                  u(i1-2*is1,i2-  is2,i3-2*is3,kdd)=
     &                    uc3d111(2*is1,  is2,2*is3,i1,i2,i3,kdd)
                  u(i1-2*is1,i2-2*is2,i3-2*is3,kdd)=
     &                    uc3d222(  is1,  is2,  is3,i1,i2,i3,kdd)
                end do
                else
                  ! old way
                 js3=3-2*ks
                 do j3=i3-2*js3,i3-js3,js3
                 do j2=i2-2*is2,i2-is2,is2
                 do j1=i1-2*is1,i1-is1,is1
                 do kdd=1,nd
                   u(j1,j2,j3,kdd)=uv3(i1-j1,i2-j2,i3-j3,i1,i2,i3,kdd)
                 end do
                 end do
                 end do
                 end do
                 end if
              end if
            end do
          end if
*             if( i3.gt.nrsab(3,1).and.i3.lt.nrsab(3,2) )then
*               write(*,9500) j1,j2,j3,(u(j1,j2,j3,kdd),kdd=1,nd),
*      &         ue(j1,j2,j3),ve(j1,j2,j3),we(j1,j2,j3)
*             end if
*  9500 format(' j1,j2,j3=',3i3,' uc=',3e8.2,' ue=',3e8.2)

        elseif( kdn.eq.2 )then

          do 420 i2=nrs(2,1),nrs(2,2)
            call insbv4( i1,i2,i3,kd1,kd2,kdn,is1,is2,is3,t,
     &        vr,vrr,d14,d24,drs,nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,
     &        xy,rsxy,u,gridType )
            do 420 kdd=1,nd
              if( oldway )then
              u(i1-  is1,i2,i3-  is3,kdd)=uc32(  is1,  is3,i1,i2,i3,kdd)
              u(i1-2*is1,i2,i3-  is3,kdd)=uc32(2*is1,  is3,i1,i2,i3,kdd)
              u(i1-  is1,i2,i3-2*is3,kdd)=uc32(  is1,2*is3,i1,i2,i3,kdd)
              u(i1-2*is1,i2,i3-2*is3,kdd)=uc32(2*is1,2*is3,i1,i2,i3,kdd)
              else
c              new
              u(i1-  is1,i2,i3-  is3,kdd)=
     &                    uc3d2e11(  is1,  is3,i1,i2,i3,kdd)
              u(i1-2*is1,i2,i3-  is3,kdd)=
     &                    uc3d2e11(2*is1,  is3,i1,i2,i3,kdd)
              u(i1-  is1,i2,i3-2*is3,kdd)=
     &                    uc3d2e11(  is1,2*is3,i1,i2,i3,kdd)
              u(i1-2*is1,i2,i3-2*is3,kdd)=
     &                    uc3d2e22(  is1,  is3,i1,i2,i3,kdd)
            end if
            continue
 420      continue
          if( period(2) )then
c           ...swap periodic edges
            if( numberOfProcessors.le.1 )then
             ! in parallel we do this in the calling routine
             i21=nrsab(2,1)
             i22=nrsab(2,2)
             do 440 j3=i3-2*is3,i3-is3,is3
              do 440 j1=i1-2*is1,i1-is1,is1
                do 440 kd=1,nd
                  u(j1,i21-1,j3,kd)=u(j1,i22-1,j3,kd)
                  u(j1,i21-2,j3,kd)=u(j1,i22-2,j3,kd)
                  u(j1,i22  ,j3,kd)=u(j1,i21  ,j3,kd)
                  u(j1,i22+1,j3,kd)=u(j1,i21+1,j3,kd)
                  u(j1,i22+2,j3,kd)=u(j1,i21+1,j3,kd)
 440         continue
            end if
          end if

        else ! kdn==1

          do 520 i1=nrs(1,1),nrs(1,2)
            call insbv4( i1,i2,i3,kd1,kd2,kdn,is1,is2,is3,t,
     &        vr,vrr,d14,d24,drs,nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,
     &        xy,rsxy,u,gridType )
            do 520 kdd=1,nd
              if( oldway )then
              u(i1,i2-  is2,i3-  is3,kdd)=uc31(  is2,  is3,i1,i2,i3,kdd)
              u(i1,i2-2*is2,i3-  is3,kdd)=uc31(2*is2,  is3,i1,i2,i3,kdd)
              u(i1,i2-  is2,i3-2*is3,kdd)=uc31(  is2,2*is3,i1,i2,i3,kdd)
              u(i1,i2-2*is2,i3-2*is3,kdd)=uc31(2*is2,2*is3,i1,i2,i3,kdd)
              else
c              new
              u(i1,i2-  is2,i3-  is3,kdd)=
     &                    uc3d1e11(  is2,  is3,i1,i2,i3,kdd)
              u(i1,i2-2*is2,i3-  is3,kdd)=
     &                    uc3d1e11(2*is2,  is3,i1,i2,i3,kdd)
              u(i1,i2-  is2,i3-2*is3,kdd)=
     &                    uc3d1e11(  is2,2*is3,i1,i2,i3,kdd)
              u(i1,i2-2*is2,i3-2*is3,kdd)=
     &                    uc3d1e22(  is2,  is3,i1,i2,i3,kdd)
              endif
            continue
 520      continue
          if( period(1) )then
c           ...swap periodic edges
            if( numberOfProcessors.le.1 )then
             ! in parallel we do this in the calling routine
             i11=nrsab(1,1)
             i12=nrsab(1,2)
             do 540 j3=i3-2*is3,i3-is3,is3
              do 540 j2=i2-2*is2,i2-is2,is2
                do 540 kd=1,nd
                  u(i11-1,j2,j3,kd)=u(i12-1,j2,j3,kd)
                  u(i11-2,j2,j3,kd)=u(i12-2,j2,j3,kd)
                  u(i12  ,j2,j3,kd)=u(i11  ,j2,j3,kd)
                  u(i12+1,j2,j3,kd)=u(i11+1,j2,j3,kd)
                  u(i12+2,j2,j3,kd)=u(i11+1,j2,j3,kd)
 540         continue
            end if
          end if
        end if
      end if

      return
      end

      subroutine insbv4( i1,i2,i3,kd1,kd2,kdn,is1,is2,is3,t,
     & vr,vrr,d14,d24,drs,nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb, xy,rsxy,u,gridType )
c======================================================================
c        Return Tangential and Mixed Derivatives
c          at a Corner in 2D or and edge in 3D
c
c Input
c  u  : solution with correct boundary values
c Output -
c  vr(.,.), vrr(.,.,.)
c======================================================================
      implicit none
      integer ndra,ndrb,ndsa,ndsb,ndta,ndtb,nd
      real t,vr(3,3),vrr(3,3,3),d14(3),d24(3),drs(3),
     &    u(ndra:ndrb,ndsa:ndsb,ndta:ndtb,0:*),
     &   xy(*),
     & rsxy(*)
      integer i1,i2,i3,kd1,kd2,kdn,is1,is2,is3,gridType
c... local variables
      integer rectangular,curvilinear
      parameter( rectangular=0, curvilinear=1 )

      if( gridType.eq.rectangular )then
        call insbv4r( i1,i2,i3,kd1,kd2,kdn,is1,is2,is3,t,
     & vr,vrr,d14,d24,drs,nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb, xy,rsxy,u,gridType )
      else
        call insbv4c( i1,i2,i3,kd1,kd2,kdn,is1,is2,is3,t,
     & vr,vrr,d14,d24,drs,nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb, xy,rsxy,u,gridType )
      end if
      return 
      end  

c Define the rectangular and curvilinear versions
#beginMacro insCornerDerivatives(name,TYPE)
      subroutine name( i1,i2,i3,kd1,kd2,kdn,is1,is2,is3,t,\
      vr,vrr,d14,d24,drs,nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb, xy,rsxy,u,gridType )
c======================================================================
c        Return Tangential and Mixed Derivatives
c          at a Corner in 2D or and edge in 3D
c
c Input
c  u  : solution with correct boundary values
c Output -
c  vr(.,.), vrr(.,.,.)
c======================================================================
 implicit none
 integer ndra,ndrb,ndsa,ndsb,ndta,ndtb,nd
 real t,vr(3,3),vrr(3,3,3),d14(3),d24(3),drs(3),\
     u(ndra:ndrb,ndsa:ndsb,ndta:ndtb,0:*),\
    xy(ndra:ndrb,ndsa:ndsb,ndta:ndtb,nd),\
  rsxy(ndra:ndrb,ndsa:ndsb,ndta:ndtb,nd,nd)
 integer i1,i2,i3,kd1,kd2,kdn,is1,is2,is3,gridType
c.......local
 real a(3,3),b(3),tn(3),delta(3,3)
 logical debug,oldway

 integer kd,kdd,kdp1,kdp2,kd3,m1,m2,m3,j1,j2,j3
 integer n1,n2,n3
 real det,deti,ajac,a1,a2,a3
 real rx,ry,rz,sx,sy,sz,tx,ty,tz,ubr,ubs,ubt,ubrr,ubss,ubtt,\
  ubrs,ubrt,ubst,rx3,rxr3,rxs3,rxt3,divr0,divs0,divt0,\
  uc31,uc32,uc33,ux6m,rsxyr,rsxys,rsxyt,trsi
 real rxr,rxs,rxt, ryr,rys,ryt, rzr,rzs,rzt,\
      sxr,sxs,sxt, syr,sys,syt, szr,szs,szt,\
      txr,txs,txt, tyr,tys,tyt, tzr,tzs,tzt

 real taylor3d3e1,taylor3d3e2,taylor3d2e1,taylor3d2e2,\
  taylor3d1e1,taylor3d1e2,uc3d1e11,uc3d2e11,uc3d3e11

 integer rectangular,curvilinear
 parameter( rectangular=0, curvilinear=1 )

c.......start statement functions

#If #TYPE == "curvilinear"
 rx(i1,i2,i3)=rsxy(i1,i2,i3,  1,  1)
 ry(i1,i2,i3)=rsxy(i1,i2,i3,  1,  2)
 rz(i1,i2,i3)=rsxy(i1,i2,i3,  1,kd3)
 sx(i1,i2,i3)=rsxy(i1,i2,i3,  2,  1)
 sy(i1,i2,i3)=rsxy(i1,i2,i3,  2,  2)
 sz(i1,i2,i3)=rsxy(i1,i2,i3,  2,kd3)
 tx(i1,i2,i3)=rsxy(i1,i2,i3,kd3,  1)
 ty(i1,i2,i3)=rsxy(i1,i2,i3,kd3,  2)
 tz(i1,i2,i3)=rsxy(i1,i2,i3,kd3,kd3)
#Elif #TYPE == "rectangular"
 rx(i1,i2,i3)=1.
 ry(i1,i2,i3)=0.
 rz(i1,i2,i3)=0.
 sx(i1,i2,i3)=0.
 sy(i1,i2,i3)=1.
 sz(i1,i2,i3)=0.
 tx(i1,i2,i3)=0.
 ty(i1,i2,i3)=0.
 tz(i1,i2,i3)=1.
#Else
 write(*,*) 'ERROR'
 stop 8
#End
c.......
 ubr(i1,i2,i3,kd)=(8.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd)) \
                     -(u(i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd)))*d14(1)
 ubs(i1,i2,i3,kd)=(8.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd)) \
                     -(u(i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd)))*d14(2)
 ubt(i1,i2,i3,kd)=(8.*(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd)) \
                     -(u(i1,i2,i3+2,kd)-u(i1,i2,i3-2,kd)))*d14(3)
 ubrr(i1,i2,i3,kd)= \
  ( -30.*u(i1,i2,i3,kd) \
   +16.*(u(i1+1,i2,i3,kd)+u(i1-1,i2,i3,kd)) \
       -(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)) )*d24(1)
 ubss(i1,i2,i3,kd)= \
 +( -30.*u(i1,i2,i3,kd) \
   +16.*(u(i1,i2+1,i3,kd)+u(i1,i2-1,i3,kd)) \
       -(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )*d24(2)
 ubtt(i1,i2,i3,kd)= \
 +( -30.*u(i1,i2,i3,kd) \
   +16.*(u(i1,i2,i3+1,kd)+u(i1,i2,i3-1,kd)) \
       -(u(i1,i2,i3+2,kd)+u(i1,i2,i3-2,kd)) )*d24(3)
 ubrs(i1,i2,i3,kd)= \
    (8.*(ubs(i1+1,i2,i3,kd)-ubs(i1-1,i2,i3,kd)) \
       -(ubs(i1+2,i2,i3,kd)-ubs(i1-2,i2,i3,kd)))*d14(1)
 ubrt(i1,i2,i3,kd)= \
    (8.*(ubt(i1+1,i2,i3,kd)-ubt(i1-1,i2,i3,kd)) \
       -(ubt(i1+2,i2,i3,kd)-ubt(i1-2,i2,i3,kd)))*d14(1)
 ubst(i1,i2,i3,kd)= \
    (8.*(ubt(i1,i2+1,i3,kd)-ubt(i1,i2-1,i3,kd)) \
       -(ubt(i1,i2+2,i3,kd)-ubt(i1,i2-2,i3,kd)))*d14(2)
c     ...short forms for jacobian entries and derivatives

#If #TYPE == "curvilinear"
 rsxyr(i1,i2,i3,m1,m2)=(8.*(rsxy(i1+1,i2,i3,m1,m2)-rsxy(i1-1,i2,i3,m1,m2)) \
                          -(rsxy(i1+2,i2,i3,m1,m2)-rsxy(i1-2,i2,i3,m1,m2)))*d14(1)
 rsxys(i1,i2,i3,m1,m2)=(8.*(rsxy(i1,i2+1,i3,m1,m2)-rsxy(i1,i2-1,i3,m1,m2)) \
                          -(rsxy(i1,i2+2,i3,m1,m2)-rsxy(i1,i2-2,i3,m1,m2)))*d14(2)
 rsxyt(i1,i2,i3,m1,m2)=(8.*(rsxy(i1,i2,i3+1,m1,m2)-rsxy(i1,i2,i3-1,m1,m2)) \
                          -(rsxy(i1,i2,i3+2,m1,m2)-rsxy(i1,i2,i3-2,m1,m2)))*d14(3)
#Else
 rsxyr(i1,i2,i3,m1,m2)=0.
 rsxys(i1,i2,i3,m1,m2)=0.
 rsxyt(i1,i2,i3,m1,m2)=0.
#End
 rxr(i1,i2,i3)=rsxyr(i1,i2,i3,1,1)
 sxr(i1,i2,i3)=rsxyr(i1,i2,i3,2,1)
 txr(i1,i2,i3)=rsxyr(i1,i2,i3,3,1)
 ryr(i1,i2,i3)=rsxyr(i1,i2,i3,1,2)
 syr(i1,i2,i3)=rsxyr(i1,i2,i3,2,2)
 tyr(i1,i2,i3)=rsxyr(i1,i2,i3,3,2)
 rzr(i1,i2,i3)=rsxyr(i1,i2,i3,1,3)
 szr(i1,i2,i3)=rsxyr(i1,i2,i3,2,3)
 tzr(i1,i2,i3)=rsxyr(i1,i2,i3,3,3)

 rxs(i1,i2,i3)=rsxys(i1,i2,i3,1,1)
 sxs(i1,i2,i3)=rsxys(i1,i2,i3,2,1)
 txs(i1,i2,i3)=rsxys(i1,i2,i3,3,1)
 rys(i1,i2,i3)=rsxys(i1,i2,i3,1,2)
 sys(i1,i2,i3)=rsxys(i1,i2,i3,2,2)
 tys(i1,i2,i3)=rsxys(i1,i2,i3,3,2)
 rzs(i1,i2,i3)=rsxys(i1,i2,i3,1,3)
 szs(i1,i2,i3)=rsxys(i1,i2,i3,2,3)
 tzs(i1,i2,i3)=rsxys(i1,i2,i3,3,3)

 rxt(i1,i2,i3)=rsxyt(i1,i2,i3,1,1)
 sxt(i1,i2,i3)=rsxyt(i1,i2,i3,2,1)
 txt(i1,i2,i3)=rsxyt(i1,i2,i3,3,1)
 ryt(i1,i2,i3)=rsxyt(i1,i2,i3,1,2)
 syt(i1,i2,i3)=rsxyt(i1,i2,i3,2,2)
 tyt(i1,i2,i3)=rsxyt(i1,i2,i3,3,2)
 rzt(i1,i2,i3)=rsxyt(i1,i2,i3,1,3)
 szt(i1,i2,i3)=rsxyt(i1,i2,i3,2,3)
 tzt(i1,i2,i3)=rsxyt(i1,i2,i3,3,3)

#If #TYPE == "curvilinear"
 rx3(m1,m2) =rsxy (i1,i2,i3,m1,m2)
#Else
 rx3(m1,m2) = delta(m1,m2)
#End
 rxr3(m1,m2)=rsxyr(i1,i2,i3,m1,m2)
 rxs3(m1,m2)=rsxys(i1,i2,i3,m1,m2)
 rxt3(m1,m2)=rsxyt(i1,i2,i3,m1,m2)
c       rhs for div.r=0, div.s=0 and div.t=0
 divr0(m1,m2,m3)= \
    rxr3(m1,1)*vr(1,m1)+rxr3(m1,2)*vr(2,m1)+rxr3(m1,3)*vr(3,m1) \
  + rxr3(m2,1)*vr(1,m2)+rxr3(m2,2)*vr(2,m2)+rxr3(m2,3)*vr(3,m2) \
  + rxr3(m3,1)*vr(1,m3)+rxr3(m3,2)*vr(2,m3)+rxr3(m3,3)*vr(3,m3) \
  + rx3(m1,1)*vrr(1,m1,m1)+rx3(m1,2)*vrr(2,m1,m1) \
                          +rx3(m1,3)*vrr(3,m1,m1) \
  + rx3(m3,1)*vrr(1,m1,m3)+rx3(m3,2)*vrr(2,m1,m3) \
                          +rx3(m3,3)*vrr(3,m1,m3)
 divs0(m1,m2,m3)= \
    rxs3(m1,1)*vr(1,m1)+rxs3(m1,2)*vr(2,m1)+rxs3(m1,3)*vr(3,m1) \
  + rxs3(m2,1)*vr(1,m2)+rxs3(m2,2)*vr(2,m2)+rxs3(m2,3)*vr(3,m2) \
  + rxs3(m3,1)*vr(1,m3)+rxs3(m3,2)*vr(2,m3)+rxs3(m3,3)*vr(3,m3) \
  + rx3(m1,1)*vrr(1,m1,m1)+rx3(m1,2)*vrr(2,m1,m1) \
                          +rx3(m1,3)*vrr(3,m1,m1) \
  + rx3(m3,1)*vrr(1,m1,m3)+rx3(m3,2)*vrr(2,m1,m3) \
                          +rx3(m3,3)*vrr(3,m1,m3)
 divt0(m1,m2,m3)= \
    rxt3(m1,1)*vr(1,m1)+rxt3(m1,2)*vr(2,m1)+rxt3(m1,3)*vr(3,m1) \
  + rxt3(m2,1)*vr(1,m2)+rxt3(m2,2)*vr(2,m2)+rxt3(m2,3)*vr(3,m2) \
  + rxt3(m3,1)*vr(1,m3)+rxt3(m3,2)*vr(2,m3)+rxt3(m3,3)*vr(3,m3) \
  + rx3(m1,1)*vrr(1,m1,m1)+rx3(m1,2)*vrr(2,m1,m1) \
                          +rx3(m1,3)*vrr(3,m1,m1) \
  + rx3(m3,1)*vrr(1,m1,m3)+rx3(m3,2)*vrr(2,m1,m3) \
                          +rx3(m3,3)*vrr(3,m1,m3)
 uc33(is1,is2,i1,i2,i3,kd)= \
  2.*u(i1,i2,i3,kd)-u(i1+is1,i2+is2,i3,kd) \
             +(is1*drs(1))**2*vrr(kd,1,1) \
             +(is2*drs(2))**2*vrr(kd,2,2)
 uc32(is1,is3,i1,i2,i3,kd)= \
  2.*u(i1,i2,i3,kd)-u(i1+is1,i2,i3+is3,kd) \
             +(is1*drs(1))**2*vrr(kd,1,1) \
             +(is3*drs(3))**2*vrr(kd,3,3)
 uc31(is2,is3,i1,i2,i3,kd)= \
  2.*u(i1,i2,i3,kd)-u(i1,i2+is2,i3+is3,kd) \
             +(is2*drs(2))**2*vrr(kd,2,2) \
             +(is3*drs(3))**2*vrr(kd,3,3)

c  Here are more accurate expressions for 3D -- exact for 4th order polynomials
 ! 3d, edge along direction 3, 1st derivative term in Taylor series
 taylor3d3e1(is1,is2,i1,i2,i3,kd)= \
      (is1*drs(1))*vr(kd,1) \
     +(is2*drs(2))*vr(kd,2)              
 ! 3d, edge along direction 3, 2nd derivative term in Taylor series
 taylor3d3e2(is1,is2,i1,i2,i3,kd)=    \
           .5*(is1*drs(1))**2*vrr(kd,1,1) \
          +.5*(is2*drs(2))**2*vrr(kd,2,2)

 ! 3d, edge along direction 2, 1st derivative term in Taylor series
 taylor3d2e1(is1,is3,i1,i2,i3,kd)= \
      (is1*drs(1))*vr(kd,1) \
     +(is3*drs(3))*vr(kd,3)              
 ! 3d, edge along direction 2, 2nd derivative term in Taylor series
 taylor3d2e2(is1,is3,i1,i2,i3,kd)= \
           .5*(is1*drs(1))**2*vrr(kd,1,1) \
          +.5*(is3*drs(3))**2*vrr(kd,3,3)

 ! 3d, edge along direction 1, 1st derivative term in Taylor series
 taylor3d1e1(is2,is3,i1,i2,i3,kd)= \
      (is2*drs(2))*vr(kd,2) \
     +(is3*drs(3))*vr(kd,3)              
 ! 3d, edge along direction 1, 2nd derivative term in Taylor series
 taylor3d1e2(is2,is3,i1,i2,i3,kd)=   \
           .5*(is2*drs(2))**2*vrr(kd,2,2) \
          +.5*(is3*drs(3))**2*vrr(kd,3,3)

 !  3d, edge along direction 3,for u(-1,-1,*) 
 uc3d3e11(is1,is2,i1,i2,i3,kd)= \
    3.75*u(i1,i2,i3,kd)   \
     -3.*u(i1+is1,i2+is2,i3,kd) \
    +.25*u(i1+2*is1,i2+2*is2,i3,kd) \
   +1.5*taylor3d3e1(is1,is2,i1,i2,i3,kd) \
   + 3.*taylor3d3e2(is1,is2,i1,i2,i3,kd)

 !  3d, edge along direction 2,for u(-1,*,-1) 
 uc3d2e11(is1,is3,i1,i2,i3,kd)= \
    3.75*u(i1,i2,i3,kd)   \
     -3.*u(i1+is1,i2,i3+is3,kd) \
    +.25*u(i1+2*is1,i2,i3+2*is3,kd) \
   +1.5*taylor3d2e1(is1,is3,i1,i2,i3,kd) \
   + 3.*taylor3d2e2(is1,is3,i1,i2,i3,kd)

 !  3d, edge along direction 1,for u(*,-1,-1) 
 uc3d1e11(is2,is3,i1,i2,i3,kd)= \
    3.75*u(i1,i2,i3,kd)   \
     -3.*u(i1,i2+is2,i3+is3,kd) \
    +.25*u(i1,i2+2*is2,i3+2*is3,kd) \
   +1.5*taylor3d1e1(is2,is3,i1,i2,i3,kd) \
   + 3.*taylor3d1e2(is2,is3,i1,i2,i3,kd)

 ! ...extrapolate velocity in 3D (extrap u(i1-n1,i2-n2,i3-n3)
 ux6m(n1,n2,n3,kd)= \
    + 6.*u(i1     ,i2     ,i3     ,kd) \
    -15.*u(i1+  n1,i2+  n2,i3+  n3,kd) \
    +20.*u(i1+2*n1,i2+2*n2,i3+2*n3,kd) \
    -15.*u(i1+3*n1,i2+3*n2,i3+3*n3,kd) \
    + 6.*u(i1+4*n1,i2+4*n2,i3+4*n3,kd) \
    -    u(i1+5*n1,i2+5*n2,i3+5*n3,kd) 
c.......end statement functions

 oldway=.false. ! .true.

 debug=.false.
 kd3=min(nd,3)

 do kd=1,3
 do kdd=1,3
   if( kd.eq.kdd )then
     delta(kd,kdd)=1.
   else
     delta(kd,kdd)=0.
   end if
 end do
 end do

 do kd=1,nd
   vr(kd,1)   =ubr(i1,i2,i3,kd)
   vr(kd,2)   =ubs(i1,i2,i3,kd)
   vrr(kd,1,1)=ubrr(i1,i2,i3,kd)
   vrr(kd,2,2)=ubss(i1,i2,i3,kd)
   if( nd.eq.3 )then
     vr(kd,3)   =ubt(i1,i2,i3,kd)
     vrr(kd,3,3)=ubtt(i1,i2,i3,kd)
   end if
 end do

 if( nd.eq.2 )then
c     ...get mixed derivatives at corners
  if( gridType.eq.curvilinear )then
   ajac=rx(i1,i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)
   a1=rxr(i1,i2,i3)*vr(1,1)+sxr(i1,i2,i3)*vr(1,2) \
     +ryr(i1,i2,i3)*vr(2,1)+syr(i1,i2,i3)*vr(2,2)
   a2=rxs(i1,i2,i3)*vr(1,1)+sxs(i1,i2,i3)*vr(1,2) \
     +rys(i1,i2,i3)*vr(2,1)+sys(i1,i2,i3)*vr(2,2)
   vrr(1,1,2)=(+ry(i1,i2,i3)*rx(i1,i2,i3)*vrr(1,1,1) \
               -sy(i1,i2,i3)*sx(i1,i2,i3)*vrr(1,2,2) \
               +ry(i1,i2,i3)*ry(i1,i2,i3)*vrr(2,1,1) \
               -sy(i1,i2,i3)*sy(i1,i2,i3)*vrr(2,2,2) \
               +ry(i1,i2,i3)*a1-sy(i1,i2,i3)*a2)/ajac
   vrr(2,1,2)=(-rx(i1,i2,i3)*rx(i1,i2,i3)*vrr(1,1,1) \
               +sx(i1,i2,i3)*sx(i1,i2,i3)*vrr(1,2,2) \
               -rx(i1,i2,i3)*ry(i1,i2,i3)*vrr(2,1,1) \
               +sx(i1,i2,i3)*sy(i1,i2,i3)*vrr(2,2,2) \
               -rx(i1,i2,i3)*a1+sx(i1,i2,i3)*a2)/ajac
  else ! rectangular
   vrr(1,1,2)=-ubss(i1,i2,i3,2)  ! uxy = -v.yy
   vrr(1,2,1)=vrr(1,1,2)
   vrr(2,1,2)=-ubrr(i1,i2,i3,1)  ! vxy = -uxx
   vrr(2,2,1)=vrr(2,1,2)
  end if
 else
c       ...3D
c         ...tangent in the direction kdn

c       if( gridType.eq.rectangular )then
c         ! u.xz = -v.yz - w.zz
c         if( kdn.eq.3 )then
c           ! z derivatives are tangential derivatives and known
c           do kd=1,3
c             vrr(kd,1,3)=ubrt(i1,i2,i3,kd)
c             vrr(kd,3,1)=vrr(kd,1,3)
c             vrr(kd,2,3)=ubst(i1,i2,i3,kd)
c             vrr(kd,3,2)=vrr(kd,2,3)
c           end do
c
c           vrr(1,1,2)=-ubss(i1,i2,i3,2)- ubst(i1,i2,i3,3) ! u.xy = -v.yy - w.yz
c           vrr(1,2,1)=vrr(1,1,2)
c
c           vrr(2,1,2)=-ubrr(i1,i2,i3,1)- ubrt(i1,i2,i3,3) ! v.xy = -u.xx - w.xz
c           vrr(2,2,1)=vrr(2,1,2)
c
c           vrr(3,1,2)=-ubrr(i1,i2,i3,1)- ubrt(i1,i2,i3,3) ! w.xy = 
c           vrr(3,2,1)=vrr(3,1,2)
c
c         else if( kdn.eq.2 )then
c         else
c         end if
c       else

   tn(1)=rx3(kd1,2)*rx3(kd2,3)-rx3(kd1,3)*rx3(kd2,2)
   tn(2)=rx3(kd1,3)*rx3(kd2,1)-rx3(kd1,1)*rx3(kd2,3)
   tn(3)=rx3(kd1,1)*rx3(kd2,2)-rx3(kd1,2)*rx3(kd2,1)
c if( debug .and.kdn.eq.1 )then
c   write(1,9100) i1,i2,i3,is1,is2,is3,kd1,kd2,kdn,tn, \
c  (vrr(kdd,1,1),kdd=1,nd),(vrr(kdd,2,2),kdd=1,nd), \
c  (vrr(kdd,3,3),kdd=1,nd)
c end if
c 9100 format(' INSBV4: i1,i2,i3 =',3i3,' is1,is2,is3=',3i3, \
c      ' kd1,kd2,kdn=',3i3,/,' tn =',3e10.2,' u.rr=',3e10.2,/, \
c      ' v.ss=',3e10.2,' v.tt=',3e10.2)
   if( kdn.eq.3 )then
c          direction 3 derivatives are also known:
c****** watch out here for i3 near boundaries****
     do kd=1,nd
       vrr(kd,1,3)=ubrt(i1,i2,i3,kd)
       vrr(kd,2,3)=ubst(i1,i2,i3,kd)
       vrr(kd,3,1)=vrr(kd,1,3)
       vrr(kd,3,2)=vrr(kd,2,3)
     end do
     b(1)=-divr0(kd1,kd2,kdn)
     b(2)=-divs0(kd2,kd1,kdn)
c         ...determine the rhs from the extrapolation condition
     if( oldway )then
     trsi=1./(2.*(is1*drs(kd1)*is2*drs(kd2)))
     b(3)= \
        tn(1)*(ux6m(is1,is2,0,1)-uc33(is1,is2,i1,i2,i3,1))*trsi \
       +tn(2)*(ux6m(is1,is2,0,2)-uc33(is1,is2,i1,i2,i3,2))*trsi \
       +tn(3)*(ux6m(is1,is2,0,3)-uc33(is1,is2,i1,i2,i3,3))*trsi
     else
     trsi=1./(3.*(is1*drs(kd1)*is2*drs(kd2)))
     b(3)= \
        tn(1)*(ux6m(is1,is2,0,1)-uc3d3e11(is1,is2,i1,i2,i3,1))*trsi \
       +tn(2)*(ux6m(is1,is2,0,2)-uc3d3e11(is1,is2,i1,i2,i3,2))*trsi \
       +tn(3)*(ux6m(is1,is2,0,3)-uc3d3e11(is1,is2,i1,i2,i3,3))*trsi
     end if
   elseif( kdn.eq.2 )then
     do kd=1,nd
       vrr(kd,1,2)=ubrs(i1,i2,i3,kd)
       vrr(kd,2,3)=ubst(i1,i2,i3,kd)
       vrr(kd,2,1)=vrr(kd,1,2)
       vrr(kd,3,2)=vrr(kd,2,3)
     end do
     b(1)=-divr0(kd1,kd2,kdn)
     b(2)=-divt0(kd2,kd1,kdn)
c         ...determine the rhs from the extrapolation condition
     if( oldway )then
     trsi=1./(2.*(is1*drs(kd1)*is3*drs(kd2)))
     b(3)= \
        tn(1)*(ux6m(is1,0,is3,1)-uc32(is1,is3,i1,i2,i3,1))*trsi \
       +tn(2)*(ux6m(is1,0,is3,2)-uc32(is1,is3,i1,i2,i3,2))*trsi \
       +tn(3)*(ux6m(is1,0,is3,3)-uc32(is1,is3,i1,i2,i3,3))*trsi
     else
     trsi=1./(3.*(is1*drs(kd1)*is3*drs(kd2)))
     b(3)= \
        tn(1)*(ux6m(is1,0,is3,1)-uc3d2e11(is1,is3,i1,i2,i3,1))*trsi \
       +tn(2)*(ux6m(is1,0,is3,2)-uc3d2e11(is1,is3,i1,i2,i3,2))*trsi \
       +tn(3)*(ux6m(is1,0,is3,3)-uc3d2e11(is1,is3,i1,i2,i3,3))*trsi
     end if
   elseif( kdn.eq.1 )then
     do kd=1,nd
       vrr(kd,1,2)=ubrs(i1,i2,i3,kd)
       vrr(kd,1,3)=ubrt(i1,i2,i3,kd)
       vrr(kd,2,1)=vrr(kd,1,2)
       vrr(kd,3,1)=vrr(kd,1,3)
     end do
     b(1)=-divs0(kd1,kd2,kdn)
     b(2)=-divt0(kd2,kd1,kdn)
c         ...determine the rhs from the extrapolation condition
     if( oldway )then
     trsi=1./(2.*(is2*drs(kd1)*is3*drs(kd2)))
     b(3)= \
        tn(1)*(ux6m(0,is2,is3,1)-uc31(is2,is3,i1,i2,i3,1))*trsi \
       +tn(2)*(ux6m(0,is2,is3,2)-uc31(is2,is3,i1,i2,i3,2))*trsi \
       +tn(3)*(ux6m(0,is2,is3,3)-uc31(is2,is3,i1,i2,i3,3))*trsi
     else
     trsi=1./(3.*(is2*drs(kd1)*is3*drs(kd2)))
     b(3)= \
        tn(1)*(ux6m(0,is2,is3,1)-uc3d1e11(is2,is3,i1,i2,i3,1))*trsi \
       +tn(2)*(ux6m(0,is2,is3,2)-uc3d1e11(is2,is3,i1,i2,i3,2))*trsi \
       +tn(3)*(ux6m(0,is2,is3,3)-uc3d1e11(is2,is3,i1,i2,i3,3))*trsi
     end if
c      if( debug )then
c        j2=i2-is2
c        j3=i3-is3
c        write(1,9700) ux6m(0,is2,is3,1),uc31(is2,is3,i1,i2,i3,1),
c     &   u0(xy(i1,j2,j3,1),xy(i1,j2,j3,2),xy(i1,j2,j3,3),t)
c      end if
c 9700 format(' ux6m(0,is2,is3,1)=',e12.4,' uc31 =',e12.4,' u0=',e12.4)
   else
     stop 'INSBV4: Invalid value for kdn'
   end if

   a(1,1)=rx3(kd2,1)
   a(1,2)=rx3(kd2,2)
   a(1,3)=rx3(kd2,3)
   a(2,1)=rx3(kd1,1)
   a(2,2)=rx3(kd1,2)
   a(2,3)=rx3(kd1,3)
   a(3,1)=tn(1)
   a(3,2)=tn(2)
   a(3,3)=tn(3)
   det=a(1,1)*(a(2,2)*a(3,3)-a(3,2)*a(2,3)) \
      +a(2,1)*(a(3,2)*a(1,3)-a(1,2)*a(3,3)) \
      +a(3,1)*(a(1,2)*a(2,3)-a(2,2)*a(1,3))
   if( det.eq.0. )then
     stop 'INSBV4: det=0'
   end if
   deti=1./det
   do kd=1,nd
     kdp1=mod(kd  ,nd)+1
     kdp2=mod(kd+1,nd)+1
     vrr(kd,kd1,kd2)= deti* \
      (  b(1)*(a(2,kdp1)*a(3,kdp2)-a(3,kdp1)*a(2,kdp2)) \
        +b(2)*(a(3,kdp1)*a(1,kdp2)-a(1,kdp1)*a(3,kdp2)) \
        +b(3)*(a(1,kdp1)*a(2,kdp2)-a(2,kdp1)*a(1,kdp2)) )
     vrr(kd,kd2,kd1)=vrr(kd,kd1,kd2)
   end do
c if( debug .and.kdn.eq.1 )then
c   write(1,9200) a,b, \
c   kd1,kd2, (vrr(kd,kd1,kd2),kd=1,nd),(ubst(i1,i2,i3,kd),kd=1,nd)
c end if
c 9200 format(' a=',9e9.1,/,' b=',3e12.4,/, \
c             ' kd1,kd2=',2i2,' vrr(kd1,kd2)=',3e12.4,/, \
c             '             ubrr=',3e12.4)

c **   end if ! curvilinear
 end if

 return
 end
#endMacro


      insCornerDerivatives(insbv4c,curvilinear)
      insCornerDerivatives(insbv4r,rectangular)
      
