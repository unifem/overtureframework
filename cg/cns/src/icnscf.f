! This file automatically generated from icnscf.bf with bpp.










c *wdh* 081214 -- xlf does not like +- --> change O1 to (O1) etc. below








      subroutine icnscf(igd, igi,xyz,rx, jac, mask, iprm,rprm,u,coeff)
c
c     060306 kkc Initial version
c     
c     icnscf computes the coefficient matrix for the linearized compressible
c     Navier-Stokes equations on a curvilinear grid.  It should work for
c     both Cartesian and (2D) cylindrical coordinate systems.  The method discretizes
c     the non-dimensionalized, primitive-variable, and constant coefficient 
c     (viscosity, thermal conduction) version of the equations.
c
c     Notes: 
c          - only 2D is supported right now
c          - a linear third order dissipation is added to the system if av4>0
      implicit none

c     INPUT 
      integer igd(2,*) ! grid dimensions
      integer igi(2,*) ! interior grid dimensions (disc points)
      integer iprm(*) ! integer solver parameters
      double precision xyz(igd(1,1):igd(2,1),
     &                     igd(1,2):igd(2,2),
     &                     igd(1,3):igd(2,3), *) ! grid vertices
      double precision rx(igd(1,1):igd(2,1),
     &                    igd(1,2):igd(2,2),
     &                    igd(1,3):igd(2,3), iprm(1), *) ! mapping derivatives
      double precision jac(igd(1,1):igd(2,1),
     &                     igd(1,2):igd(2,2),
     &                     igd(1,3):igd(2,3)) ! det mapping jacobian
      integer mask(igd(1,1):igd(2,1),
     &             igd(1,2):igd(2,2),
     &             igd(1,3):igd(2,3))   ! grid mask
      double precision rprm(*) ! real solver parameters
      double precision u(igd(1,1):igd(2,1),
     &                   igd(1,2):igd(2,2),
     &                   igd(1,3):igd(2,3), *) ! state to linearize about

c     OUTPUT
      double precision coeff(0:iprm(11)*iprm(2)**2-1,igd(1,1):igd(2,1),
     &                                           igd(1,2):igd(2,2),
     &                                           igd(1,3):igd(2,3))

c     LOCAL
      integer i1,i2,i3,d,e,c,ndim,ncmp,cc,is
      integer rc,uc,vc,wc,tc,gmove,isaxi,isswirl,eqn
      integer isten_size,width,hwidth,width3
      double precision ren,prn,man,gam,gm1,impfac
      double precision t0,dt,axifac, gm2, oren, oprn, f43, f13
      double precision fac,jaci,drar,lur,lus,av4,av2,orad,drad
      logical usestrik,oldstrik,avgmomforcing
      double precision alpha,dr(3),dri(3),dri2(3)
      double precision alpha_r, alpha_s
c     STATEMENT FUNCTIONS
      include 'icnssfdec.h'
      include 'icnssf.h'

      off(1) = 0
      off(2) = 0
      off(3) = 0
      occ = -1
      oce = -1

      usestrik = .true.
c      oldstrik = .true.
      oldstrik = .false.
c      usestrik = .false.
c      alpha = 0d0
      alpha = 1d0/6d0
c      alpha = 1d0/7d0
c      alpha = 1d0/24d0
c      alpha = 1d0/60d0
c      alpha = 1d0/600d0
      avgmomforcing = .false.

      ndim = iprm(1)
      ncmp = iprm(2)
      rc   = iprm(3)+1
      uc   = iprm(4)+1
      vc   = iprm(5)+1
      wc   = iprm(6)+1
      tc   = iprm(7)+1
      gmove= iprm(8)
      isaxi= iprm(9)
      isswirl= iprm(10)
      isten_size= iprm(11)
      width = iprm(12)
      hwidth= iprm(13)
      width3 = 0

      if ( (isswirl.eq.0) ) wc=tc

      if ( ndim.eq.3 ) width3 = width

      ren = rprm(1)
      prn = rprm(2)
      man = rprm(3)
      gam =rprm(4)
      impfac=rprm(5)
      dr(1) = rprm(6)
      dr(2) = rprm(7)
      dr(3) = rprm(8)
      t0    = rprm(9)
      dt    = rprm(10)
      av2 = rprm(13)
      av4 = rprm(14)

      alpha = rprm(18)

      f43 = 4d0/3d0
      f13 = 1d0/3d0
      oren = 1d0/ren
      oprn = 1d0/prn
      gm1 = gam-1d0
      gm2 = 1d0/(gam*man*man)
      do d=1,ndim
         dri(d) = 1d0/dr(d)
         dri2(d) = .5d0*dri(d)
      end do

      if ( isaxi.ne.0 ) then
         axifac = 1d0
      else
         axifac = 0d0
      endif

      if ( .false. ) then
      write (*,*) "INSIDE ICNSCF"
      write (*,'("grid bounds : ",2(1x,i4))') igd(1,1), igd(2,1)
      write (*,'("              ",2(1x,i4))') igd(1,2), igd(2,2)
      write (*,'("              ",2(1x,i4))') igd(1,3), igd(2,3)

      write (*,'("disc bounds : ",2(1x,i4))') igi(1,1), igi(2,1)
      write (*,'("              ",2(1x,i4))') igi(1,2), igi(2,2)
      write (*,'("              ",2(1x,i4))') igi(1,3), igi(2,3)

      write (*,'("stencil size : ",i4))') iprm(11)
      write (*,'("width        : ",i4))') iprm(12)
      write (*,'("hwidth       : ",i4))') iprm(13)
      write (*,'("num coeff.   : ",i4))') iprm(2)
      write (*,'("num dim.     : ",i4))') iprm(1)
      write (*,'("isaxi        : ",i2))') iprm(9)
      write (*,'("isswirl      : ",i2))') iprm(10)

      write (*,'("Reynold`s #  : ",f10.5)') rprm(1)
      write (*,'("Prandtl   #  : ",f10.5)') rprm(2)
      write (*,'("Mach      #  : ",f10.5)') rprm(3)
      write (*,'("gamma        : ",f10.5)') rprm(4)
      write (*,'("dr(*)        : ",3(2x,f10.5))') rprm(6),rprm(7),rprm(
     & 8)
      write (*,'("dt        : ",f10.5)') dt
      endif

c      print *,"av2= ",av2,", av4= ",av4
c     scan and fix negative densities
      if ( .false. ) then
      do i3=igi(1,3), igi(2,3)
      do i2=igi(1,2), igi(2,2)
      do i1=igi(1,1), igi(2,1)
c            u(i1,i2,i3,rc)  = u(i1,i2,i3,rc) + 1e-7
         if ( mask(i1,i2,i3).gt.0 .and. u(i1,i2,i3,rc).lt.0d0 ) then
      write(*,*)"WARNING : icnscf : negative density at ",
     &           i1,", ",i2," : ",u(i1,i2,i3,rc)
c         u(i1,i2,i3,rc) = abs(u(i1,i2,i3,rc))
c            u(i1,i2,i3,rc) = .25d0 * ( abs(u(i1+1,i2,i3,rc)) + 
c     &                                 abs(u(i1-1,i2,i3,rc)) +
c     &                                 abs(u(i1,i2+1,i3,rc)) +
c     &                                 abs(u(i1,i2-1,i3,rc)) )
         endif
      end do
      end do
      end do
      endif

      do i3=igi(1,3), igi(2,3)
      do i2=igi(1,2), igi(2,2)
      do i1=igi(1,1), igi(2,1)

         if ( mask(i1,i2,i3).gt.0 ) then

c
c     CONTINUITY DISCRETIZED WITH A CONSERVATIVE FINITE VOLUME METHOD
c     
            eqn = rc

            jaci = 1d0/jac(i1,i2,i3)
            do d=uc,vc

               cc = d-uc+1
c           linearized D_{o\xi} (\rho a_{11}u + \rho a_{12}v)/J
c      linearize terms like -jaci*rx(i1-1,i2,i3,1,cc)*jac(i1-1,i2,i3)*dri2(1) * u(rc)* u(d) at the grid point offset by -1, 0, 0
                     coeff(icf((-1),(0),(0),eqn,rc),i1,i2,i3) = coeff(
     & icf((-1),(0),(0),eqn,rc),i1,i2,i3) + (-jaci*rx(i1-1,i2,i3,1,cc)
     & *jac(i1-1,i2,i3)*dri2(1))*u(i1+(-1)+off(1),i2+(0)+off(2),i3+(0)
     & +off(3),d)
                     coeff(icf((-1),(0),(0),eqn,d),i1,i2,i3) = coeff(
     & icf((-1),(0),(0),eqn,d),i1,i2,i3) + (-jaci*rx(i1-1,i2,i3,1,cc)*
     & jac(i1-1,i2,i3)*dri2(1))*u(i1+(-1)+off(1),i2+(0)+off(2),i3+(0)+
     & off(3),rc)
c      linearize terms like jaci*rx(i1+1,i2,i3,1,cc)*jac(i1+1,i2,i3)*dri2(1) * u(rc)* u(d) at the grid point offset by 1, 0, 0
                     coeff(icf((1),(0),(0),eqn,rc),i1,i2,i3) = coeff(
     & icf((1),(0),(0),eqn,rc),i1,i2,i3) + (jaci*rx(i1+1,i2,i3,1,cc)*
     & jac(i1+1,i2,i3)*dri2(1))*u(i1+(1)+off(1),i2+(0)+off(2),i3+(0)+
     & off(3),d)
                     coeff(icf((1),(0),(0),eqn,d),i1,i2,i3) = coeff(
     & icf((1),(0),(0),eqn,d),i1,i2,i3) + (jaci*rx(i1+1,i2,i3,1,cc)*
     & jac(i1+1,i2,i3)*dri2(1))*u(i1+(1)+off(1),i2+(0)+off(2),i3+(0)+
     & off(3),rc)

c           linearized D_{o\eta} (\rho a_{21}u + \rho a_{22}v)/J
c      linearize terms like -jaci*rx(i1,i2-1,i3,2,cc)*jac(i1,i2-1,i3)*dri2(2) * u(rc)* u(d) at the grid point offset by 0, -1, 0
                     coeff(icf((0),(-1),(0),eqn,rc),i1,i2,i3) = coeff(
     & icf((0),(-1),(0),eqn,rc),i1,i2,i3) + (-jaci*rx(i1,i2-1,i3,2,cc)
     & *jac(i1,i2-1,i3)*dri2(2))*u(i1+(0)+off(1),i2+(-1)+off(2),i3+(0)
     & +off(3),d)
                     coeff(icf((0),(-1),(0),eqn,d),i1,i2,i3) = coeff(
     & icf((0),(-1),(0),eqn,d),i1,i2,i3) + (-jaci*rx(i1,i2-1,i3,2,cc)*
     & jac(i1,i2-1,i3)*dri2(2))*u(i1+(0)+off(1),i2+(-1)+off(2),i3+(0)+
     & off(3),rc)
c      linearize terms like jaci*rx(i1,i2+1,i3,2,cc)*jac(i1,i2+1,i3)*dri2(2) * u(rc)* u(d) at the grid point offset by 0, 1, 0
                     coeff(icf((0),(1),(0),eqn,rc),i1,i2,i3) = coeff(
     & icf((0),(1),(0),eqn,rc),i1,i2,i3) + (jaci*rx(i1,i2+1,i3,2,cc)*
     & jac(i1,i2+1,i3)*dri2(2))*u(i1+(0)+off(1),i2+(1)+off(2),i3+(0)+
     & off(3),d)
                     coeff(icf((0),(1),(0),eqn,d),i1,i2,i3) = coeff(
     & icf((0),(1),(0),eqn,d),i1,i2,i3) + (jaci*rx(i1,i2+1,i3,2,cc)*
     & jac(i1,i2+1,i3)*dri2(2))*u(i1+(0)+off(1),i2+(1)+off(2),i3+(0)+
     & off(3),rc)

               if ( usestrik ) then

                  if ( oldstrik ) then
c     coefficients for a 3rd order  difference approximation to 1d0 * u[eqn] u[d]_X
c                 u[eqn]^{n+1} * u[d]^n_X
                           coeff(icf(0,0,0,eqn,eqn),i1,i2,i3) = coeff(
     & icf(0,0,0,eqn,eqn),i1,i2,i3) + (1d0) * ux4p(i1,i2,i3,d,cc)
c                  (r_X u[d]_r^{n+1} + s_X u[d]_s^{n+1} )
c     coefficients for a 3rd order  difference approximation to (1d0)*u(i1+off(1),i2+off(2),i3+off(3),eqn) * u[d]_X
c                  (r_X u[d]_r^{n+1} + s_X u[d]_s^{n+1} )
c                 r - part
                                 coeff(icf(+1,0,0,eqn,d),i1,i2,i3) =  
     & coeff(icf(+1,0,0,eqn,d),i1,i2,i3) - ((1d0)*u(i1+off(1),i2+off(
     & 2),i3+off(3),eqn)) * (alpha-.5d0)    *rx(i1+off(1),i2+off(2),
     & i3+off(3),1,cc)*dri(1)
                                 coeff(icf( 0,0,0,eqn,d),i1,i2,i3) =  
     & coeff(icf( 0,0,0,eqn,d),i1,i2,i3) + ((1d0)*u(i1+off(1),i2+off(
     & 2),i3+off(3),eqn)) * (3d0*alpha)     *rx(i1+off(1),i2+off(2),
     & i3+off(3),1,cc)*dri(1)
                                 coeff(icf(-1,0,0,eqn,d),i1,i2,i3) =  
     & coeff(icf(-1,0,0,eqn,d),i1,i2,i3) - ((1d0)*u(i1+off(1),i2+off(
     & 2),i3+off(3),eqn)) * (.5d0+3d0*alpha)*rx(i1+off(1),i2+off(2),
     & i3+off(3),1,cc)*dri(1)
                                 coeff(icf(-2,0,0,eqn,d),i1,i2,i3) =  
     & coeff(icf(-2,0,0,eqn,d),i1,i2,i3) + ((1d0)*u(i1+off(1),i2+off(
     & 2),i3+off(3),eqn)) * (alpha)         *rx(i1+off(1),i2+off(2),
     & i3+off(3),1,cc)*dri(1)
c                 s - part
                                 coeff(icf(0,+1,0,eqn,d),i1,i2,i3) =  
     & coeff(icf(0,+1,0,eqn,d),i1,i2,i3) - ((1d0)*u(i1+off(1),i2+off(
     & 2),i3+off(3),eqn)) * (alpha-.5d0)    *rx(i1+off(1),i2+off(2),
     & i3+off(3),2,cc)*dri(2)
                                 coeff(icf( 0,0,0,eqn,d),i1,i2,i3) =  
     & coeff(icf(0,0,0,eqn,d),i1,i2,i3)  + ((1d0)*u(i1+off(1),i2+off(
     & 2),i3+off(3),eqn)) * (3d0*alpha)     *rx(i1+off(1),i2+off(2),
     & i3+off(3),2,cc)*dri(2)
                                 coeff(icf(0,-1,0,eqn,d),i1,i2,i3) =  
     & coeff(icf(0,-1,0,eqn,d),i1,i2,i3) - ((1d0)*u(i1+off(1),i2+off(
     & 2),i3+off(3),eqn)) * (.5d0+3d0*alpha)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,cc)*dri(2)
                                 coeff(icf(0,-2,0,eqn,d),i1,i2,i3) =  
     & coeff(icf(0,-2,0,eqn,d),i1,i2,i3) + ((1d0)*u(i1+off(1),i2+off(
     & 2),i3+off(3),eqn)) * (alpha)         *rx(i1+off(1),i2+off(2),
     & i3+off(3),2,cc)*dri(2)
c                 r - part
c      coeff(icf(+1,0,0,eqn,d),i1,i2,i3) =  coeff(icf(+1,0,0,eqn,d),i1,i2,i3) - (1d0) * (alpha-.5d0)    *rx(i1+off(1),i2+off(2),i3+off(3),1,cc)*u(i1+off(1),i2+off(2),i3+off(3),eqn)*dri(1)
c      coeff(icf( 0,0,0,eqn,d),i1,i2,i3) =  coeff(icf( 0,0,0,eqn,d),i1,i2,i3) + (1d0) * (3d0*alpha)     *rx(i1+off(1),i2+off(2),i3+off(3),1,cc)*u(i1+off(1),i2+off(2),i3+off(3),eqn)*dri(1)
c      coeff(icf(-1,0,0,eqn,d),i1,i2,i3) =  coeff(icf(-1,0,0,eqn,d),i1,i2,i3) - (1d0) * (.5d0+3d0*alpha)*rx(i1+off(1),i2+off(2),i3+off(3),1,cc)*u(i1+off(1),i2+off(2),i3+off(3),eqn)*dri(1)
c      coeff(icf(-2,0,0,eqn,d),i1,i2,i3) =  coeff(icf(-2,0,0,eqn,d),i1,i2,i3) + (1d0) * (alpha)         *rx(i1+off(1),i2+off(2),i3+off(3),1,cc)*u(i1+off(1),i2+off(2),i3+off(3),eqn)*dri(1)
c                 s - part
c      coeff(icf(0,+1,0,eqn,d),i1,i2,i3) =  coeff(icf(0,+1,0,eqn,d),i1,i2,i3) - (1d0) * (alpha-.5d0)    *rx(i1+off(1),i2+off(2),i3+off(3),2,cc)*u(i1+off(1),i2+off(2),i3+off(3),eqn)*dri(2)
c      coeff(icf( 0,0,0,eqn,d),i1,i2,i3) =  coeff(icf(0,0,0,eqn,d),i1,i2,i3)  + (1d0) * (3d0*alpha)     *rx(i1+off(1),i2+off(2),i3+off(3),2,cc)*u(i1+off(1),i2+off(2),i3+off(3),eqn)*dri(2)
c      coeff(icf(0,-1,0,eqn,d),i1,i2,i3) =  coeff(icf(0,-1,0,eqn,d),i1,i2,i3) - (1d0) * (.5d0+3d0*alpha)*rx(i1+off(1),i2+off(2),i3+off(3),2,cc)*u(i1+off(1),i2+off(2),i3+off(3),eqn)*dri(2)
c      coeff(icf(0,-2,0,eqn,d),i1,i2,i3) =  coeff(icf(0,-2,0,eqn,d),i1,i2,i3) + (1d0) * (alpha)         *rx(i1+off(1),i2+off(2),i3+off(3),2,cc)*u(i1+off(1),i2+off(2),i3+off(3),eqn)*dri(2)
!     subtract off the part we don't need
c     coefficients for a central difference approximation to (-1d0) * u[eqn] u[d]_X
c                 u[eqn]^{n+1} * u[d]^n_X
                                       coeff(icf(0,0,0,eqn,eqn),i1,i2,
     & i3) = coeff(icf(0,0,0,eqn,eqn),i1,i2,i3) + ((-1d0)) * ux(i1,i2,
     & i3,d,cc)
c                 u[eqn]^n * (r_X u[d]_r^{n+1} + s_X u[d]_s^{n+1} )
c                 r - part
                                       coeff(icf(+1,0,0,eqn,d),i1,i2,
     & i3) =  coeff(icf(+1,0,0,eqn,d),i1,i2,i3) + ((-1d0)) * rx(i1+
     & off(1),i2+off(2),i3+off(3),1,cc)*u(i1+off(1),i2+off(2),i3+off(
     & 3),eqn)*dri2(1)
                                       coeff(icf(-1,0,0,eqn,d),i1,i2,
     & i3) =  coeff(icf(-1,0,0,eqn,d),i1,i2,i3) - ((-1d0)) * rx(i1+
     & off(1),i2+off(2),i3+off(3),1,cc)*u(i1+off(1),i2+off(2),i3+off(
     & 3),eqn)*dri2(1)
c                 s - part
                                       coeff(icf(0,+1,0,eqn,d),i1,i2,
     & i3) =  coeff(icf(0,+1,0,eqn,d),i1,i2,i3) + ((-1d0)) * rx(i1+
     & off(1),i2+off(2),i3+off(3),2,cc)*u(i1+off(1),i2+off(2),i3+off(
     & 3),eqn)*dri2(2)
                                       coeff(icf(0,-1,0,eqn,d),i1,i2,
     & i3) =  coeff(icf(0,-1,0,eqn,d),i1,i2,i3) - ((-1d0)) * rx(i1+
     & off(1),i2+off(2),i3+off(3),2,cc)*u(i1+off(1),i2+off(2),i3+off(
     & 3),eqn)*dri2(2)
                  else
c                     if ( i1.gt.igi(1,1) ) then
                     drar = alpha*jaci*dri(1)
                     fac =     -drar*rx(i1+1,i2,i3,1,cc)*jac(i1+1,i2,
     & i3)
c                     print *,i1,i2,drar,fac,jac(i1+1,i2,i3)
c      linearize terms like fac * u(eqn)* u(d) at the grid point offset by +1, 0, 0
                           coeff(icf((+1),(0),(0),eqn,eqn),i1,i2,i3) = 
     & coeff(icf((+1),(0),(0),eqn,eqn),i1,i2,i3) + (fac)*u(i1+(+1)+
     & off(1),i2+(0)+off(2),i3+(0)+off(3),d)
                           coeff(icf((+1),(0),(0),eqn,d),i1,i2,i3) = 
     & coeff(icf((+1),(0),(0),eqn,d),i1,i2,i3) + (fac)*u(i1+(+1)+off(
     & 1),i2+(0)+off(2),i3+(0)+off(3),eqn)
                     fac =  3d0*drar*rx(i1  ,i2,i3,1,cc)*jac(i1  ,i2,
     & i3)
c      linearize terms like fac * u(eqn)* u(d) at the grid point offset by 0, 0, 0
                           coeff(icf((0),(0),(0),eqn,eqn),i1,i2,i3) = 
     & coeff(icf((0),(0),(0),eqn,eqn),i1,i2,i3) + (fac)*u(i1+(0)+off(
     & 1),i2+(0)+off(2),i3+(0)+off(3),d)
                           coeff(icf((0),(0),(0),eqn,d),i1,i2,i3) = 
     & coeff(icf((0),(0),(0),eqn,d),i1,i2,i3) + (fac)*u(i1+(0)+off(1),
     & i2+(0)+off(2),i3+(0)+off(3),eqn)
                     fac = -3d0*drar*rx(i1-1,i2,i3,1,cc)*jac(i1-1,i2,
     & i3)
c      linearize terms like fac * u(eqn)* u(d) at the grid point offset by -1, 0, 0
                           coeff(icf((-1),(0),(0),eqn,eqn),i1,i2,i3) = 
     & coeff(icf((-1),(0),(0),eqn,eqn),i1,i2,i3) + (fac)*u(i1+(-1)+
     & off(1),i2+(0)+off(2),i3+(0)+off(3),d)
                           coeff(icf((-1),(0),(0),eqn,d),i1,i2,i3) = 
     & coeff(icf((-1),(0),(0),eqn,d),i1,i2,i3) + (fac)*u(i1+(-1)+off(
     & 1),i2+(0)+off(2),i3+(0)+off(3),eqn)
                     fac =      drar*rx(i1-2,i2,i3,1,cc)*jac(i1-2,i2,
     & i3)
c      linearize terms like fac * u(eqn)* u(d) at the grid point offset by -2, 0, 0
                           coeff(icf((-2),(0),(0),eqn,eqn),i1,i2,i3) = 
     & coeff(icf((-2),(0),(0),eqn,eqn),i1,i2,i3) + (fac)*u(i1+(-2)+
     & off(1),i2+(0)+off(2),i3+(0)+off(3),d)
                           coeff(icf((-2),(0),(0),eqn,d),i1,i2,i3) = 
     & coeff(icf((-2),(0),(0),eqn,d),i1,i2,i3) + (fac)*u(i1+(-2)+off(
     & 1),i2+(0)+off(2),i3+(0)+off(3),eqn)
c                     endif
c                     if ( i2.gt.igi(1,2) ) then
                     drar = alpha*jaci*dri(2)
                     fac =     -drar*rx(i1,i2+1,i3,2,cc)*jac(i1,i2+1,
     & i3)
c      linearize terms like fac * u(eqn)* u(d) at the grid point offset by 0, +1, 0
                           coeff(icf((0),(+1),(0),eqn,eqn),i1,i2,i3) = 
     & coeff(icf((0),(+1),(0),eqn,eqn),i1,i2,i3) + (fac)*u(i1+(0)+off(
     & 1),i2+(+1)+off(2),i3+(0)+off(3),d)
                           coeff(icf((0),(+1),(0),eqn,d),i1,i2,i3) = 
     & coeff(icf((0),(+1),(0),eqn,d),i1,i2,i3) + (fac)*u(i1+(0)+off(1)
     & ,i2+(+1)+off(2),i3+(0)+off(3),eqn)
                     fac =  3d0*drar*rx(i1,i2  ,i3,2,cc)*jac(i1,i2,i3)
c      linearize terms like fac * u(eqn)* u(d) at the grid point offset by 0, 0, 0
                           coeff(icf((0),(0),(0),eqn,eqn),i1,i2,i3) = 
     & coeff(icf((0),(0),(0),eqn,eqn),i1,i2,i3) + (fac)*u(i1+(0)+off(
     & 1),i2+(0)+off(2),i3+(0)+off(3),d)
                           coeff(icf((0),(0),(0),eqn,d),i1,i2,i3) = 
     & coeff(icf((0),(0),(0),eqn,d),i1,i2,i3) + (fac)*u(i1+(0)+off(1),
     & i2+(0)+off(2),i3+(0)+off(3),eqn)
                     fac = -3d0*drar*rx(i1,i2-1,i3,2,cc)*jac(i1,i2-1,
     & i3)
c      linearize terms like fac * u(eqn)* u(d) at the grid point offset by 0, -1, 0
                           coeff(icf((0),(-1),(0),eqn,eqn),i1,i2,i3) = 
     & coeff(icf((0),(-1),(0),eqn,eqn),i1,i2,i3) + (fac)*u(i1+(0)+off(
     & 1),i2+(-1)+off(2),i3+(0)+off(3),d)
                           coeff(icf((0),(-1),(0),eqn,d),i1,i2,i3) = 
     & coeff(icf((0),(-1),(0),eqn,d),i1,i2,i3) + (fac)*u(i1+(0)+off(1)
     & ,i2+(-1)+off(2),i3+(0)+off(3),eqn)
                     fac =     drar*rx(i1,i2-2,i3,2,cc)*jac(i1,i2-2,i3)
c      linearize terms like fac * u(eqn)* u(d) at the grid point offset by 0, -2, 0
                           coeff(icf((0),(-2),(0),eqn,eqn),i1,i2,i3) = 
     & coeff(icf((0),(-2),(0),eqn,eqn),i1,i2,i3) + (fac)*u(i1+(0)+off(
     & 1),i2+(-2)+off(2),i3+(0)+off(3),d)
                           coeff(icf((0),(-2),(0),eqn,d),i1,i2,i3) = 
     & coeff(icf((0),(-2),(0),eqn,d),i1,i2,i3) + (fac)*u(i1+(0)+off(1)
     & ,i2+(-2)+off(2),i3+(0)+off(3),eqn)
c                     endif

                  endif

               endif

            end do


c     freestream correction
            if ( .true. ) then

               do d=1,ndim
                  cc = uc+d-1
                  fac = jaci*(
     &                 (jac(i1+1,i2,i3)*rx(i1+1,i2,i3,1,d)-
     &                 jac(i1-1,i2,i3)*rx(i1-1,i2,i3,1,d))*dri2(1) +
     &                 (jac(i1,i2+1,i3)*rx(i1,i2+1,i3,2,d)-
     &                 jac(i1,i2-1,i3)*rx(i1,i2-1,i3,2,d))*dri2(2))
c      linearize terms like -fac * u(rc)* u(cc) at the grid point offset by 0, 0, 0
                        coeff(icf((0),(0),(0),eqn,rc),i1,i2,i3) = 
     & coeff(icf((0),(0),(0),eqn,rc),i1,i2,i3) + (-fac)*u(i1+(0)+off(
     & 1),i2+(0)+off(2),i3+(0)+off(3),cc)
                        coeff(icf((0),(0),(0),eqn,cc),i1,i2,i3) = 
     & coeff(icf((0),(0),(0),eqn,cc),i1,i2,i3) + (-fac)*u(i1+(0)+off(
     & 1),i2+(0)+off(2),i3+(0)+off(3),rc)

                  if ( usestrik .and. .not.oldstrik) then
                     drar = alpha*jaci*dri(1)
                     fac =
     &                    -drar*rx(i1+1,i2,i3,1,d)*jac(i1+1,i2,i3)
     &                +3d0*drar*rx(i1  ,i2,i3,1,d)*jac(i1,i2,i3)
     &                -3d0*drar*rx(i1-1,i2,i3,1,d)*jac(i1-1,i2,i3)
     &                    +drar*rx(i1-2,i2,i3,1,d)*jac(i1-2,i2,i3)
                     drar = alpha*jaci*dri(2)
                     fac = fac
     &                    -drar*rx(i1,i2+1,i3,2,d)*jac(i1,i2+1,i3)
     &                +3d0*drar*rx(i1,i2  ,i3,2,d)*jac(i1,i2,i3)
     &                -3d0*drar*rx(i1,i2-1,i3,2,d)*jac(i1,i2-1,i3)
     &                    +drar*rx(i1,i2-2,i3,2,d)*jac(i1,i2-2,i3)

c      linearize terms like -fac * u(rc)* u(cc) at the grid point offset by 0, 0, 0
                           coeff(icf((0),(0),(0),eqn,rc),i1,i2,i3) = 
     & coeff(icf((0),(0),(0),eqn,rc),i1,i2,i3) + (-fac)*u(i1+(0)+off(
     & 1),i2+(0)+off(2),i3+(0)+off(3),cc)
                           coeff(icf((0),(0),(0),eqn,cc),i1,i2,i3) = 
     & coeff(icf((0),(0),(0),eqn,cc),i1,i2,i3) + (-fac)*u(i1+(0)+off(
     & 1),i2+(0)+off(2),i3+(0)+off(3),rc)
                  endif

               enddo


            endif

c     axisymmetric forcing
            if ( isaxi.gt.0 .and. xyz(i1,i2,i3,2).gt.0d0 ) then

               do d=1,ndim
                  cc = uc+d-1

                  if ( .true. ) then
                           if ( .true. ) then
                           drar = (1)*dri2(1)*jaci*jac(i1+off(1)-1,i2+
     & off(2),i3+off(3))*(xyz(i1+off(1),i2+off(2),i3+off(3),2)-xyz(i1+
     & off(1)-1,i2+off(2),i3+off(3),2))*rx(i1+off(1)-1,i2+off(2),i3+
     & off(3),1,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
c      linearize terms like drar * u(rc)* u(cc) at the grid point offset by -1, 0, 0
                                 coeff(icf((-1),(0),(0),eqn,rc),i1,i2,
     & i3) = coeff(icf((-1),(0),(0),eqn,rc),i1,i2,i3) + (drar)*u(i1+(-
     & 1)+off(1),i2+(0)+off(2),i3+(0)+off(3),cc)
                                 coeff(icf((-1),(0),(0),eqn,cc),i1,i2,
     & i3) = coeff(icf((-1),(0),(0),eqn,cc),i1,i2,i3) + (drar)*u(i1+(-
     & 1)+off(1),i2+(0)+off(2),i3+(0)+off(3),rc)
                           drar = (1)*dri2(1)*jaci*jac(i1+off(1)+1,i2+
     & off(2),i3+off(3))*(xyz(i1+off(1)+1,i2+off(2),i3+off(3),2)-xyz(
     & i1+off(1),i2+off(2),i3+off(3),2))*rx(i1+off(1)+1,i2+off(2),i3+
     & off(3),1,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
c      linearize terms like drar * u(rc)* u(cc) at the grid point offset by +1, 0, 0
                                 coeff(icf((+1),(0),(0),eqn,rc),i1,i2,
     & i3) = coeff(icf((+1),(0),(0),eqn,rc),i1,i2,i3) + (drar)*u(i1+(+
     & 1)+off(1),i2+(0)+off(2),i3+(0)+off(3),cc)
                                 coeff(icf((+1),(0),(0),eqn,cc),i1,i2,
     & i3) = coeff(icf((+1),(0),(0),eqn,cc),i1,i2,i3) + (drar)*u(i1+(+
     & 1)+off(1),i2+(0)+off(2),i3+(0)+off(3),rc)
                           drar = (1)*dri2(2)*jaci*jac(i1+off(1),i2+
     & off(2)-1,i3+off(3))*(xyz(i1+off(1),i2+off(2),i3+off(3),2)-xyz(
     & i1+off(1),i2+off(2)-1,i3+off(3),2))*rx(i1+off(1),i2+off(2)-1,
     & i3+off(3),2,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
c      linearize terms like drar * u(rc)* u(cc) at the grid point offset by 0, -1, 0
                                 coeff(icf((0),(-1),(0),eqn,rc),i1,i2,
     & i3) = coeff(icf((0),(-1),(0),eqn,rc),i1,i2,i3) + (drar)*u(i1+(
     & 0)+off(1),i2+(-1)+off(2),i3+(0)+off(3),cc)
                                 coeff(icf((0),(-1),(0),eqn,cc),i1,i2,
     & i3) = coeff(icf((0),(-1),(0),eqn,cc),i1,i2,i3) + (drar)*u(i1+(
     & 0)+off(1),i2+(-1)+off(2),i3+(0)+off(3),rc)
                           drar = (1)*dri2(2)*jaci*jac(i1+off(1),i2+
     & off(2)+1,i3+off(3))*(xyz(i1+off(1),i2+off(2)+1,i3+off(3),2)-
     & xyz(i1+off(1),i2+off(2),i3+off(3),2))*rx(i1+off(1),i2+off(2)+1,
     & i3+off(3),2,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
c      linearize terms like drar * u(rc)* u(cc) at the grid point offset by 0, +1, 0
                                 coeff(icf((0),(+1),(0),eqn,rc),i1,i2,
     & i3) = coeff(icf((0),(+1),(0),eqn,rc),i1,i2,i3) + (drar)*u(i1+(
     & 0)+off(1),i2+(+1)+off(2),i3+(0)+off(3),cc)
                                 coeff(icf((0),(+1),(0),eqn,cc),i1,i2,
     & i3) = coeff(icf((0),(+1),(0),eqn,cc),i1,i2,i3) + (drar)*u(i1+(
     & 0)+off(1),i2+(+1)+off(2),i3+(0)+off(3),rc)
                           else if ( .false. ) then
                              drar = (1)*.25d0
c      linearize terms like drar/xyz(i1-1,i2,i3,2) * u(rc)* u(cc) at the grid point offset by -1, 0, 0
                                   coeff(icf((-1),(0),(0),eqn,rc),i1,
     & i2,i3) = coeff(icf((-1),(0),(0),eqn,rc),i1,i2,i3) + (drar/xyz(
     & i1-1,i2,i3,2))*u(i1+(-1)+off(1),i2+(0)+off(2),i3+(0)+off(3),cc)
                                   coeff(icf((-1),(0),(0),eqn,cc),i1,
     & i2,i3) = coeff(icf((-1),(0),(0),eqn,cc),i1,i2,i3) + (drar/xyz(
     & i1-1,i2,i3,2))*u(i1+(-1)+off(1),i2+(0)+off(2),i3+(0)+off(3),rc)
c      linearize terms like drar/xyz(i1+1,i2,i3,2) * u(rc)* u(cc) at the grid point offset by +1, 0, 0
                                   coeff(icf((+1),(0),(0),eqn,rc),i1,
     & i2,i3) = coeff(icf((+1),(0),(0),eqn,rc),i1,i2,i3) + (drar/xyz(
     & i1+1,i2,i3,2))*u(i1+(+1)+off(1),i2+(0)+off(2),i3+(0)+off(3),cc)
                                   coeff(icf((+1),(0),(0),eqn,cc),i1,
     & i2,i3) = coeff(icf((+1),(0),(0),eqn,cc),i1,i2,i3) + (drar/xyz(
     & i1+1,i2,i3,2))*u(i1+(+1)+off(1),i2+(0)+off(2),i3+(0)+off(3),rc)
c      linearize terms like drar/xyz(i1,i2-1,i3,2) * u(rc)* u(cc) at the grid point offset by 0, -1, 0
                                   coeff(icf((0),(-1),(0),eqn,rc),i1,
     & i2,i3) = coeff(icf((0),(-1),(0),eqn,rc),i1,i2,i3) + (drar/xyz(
     & i1,i2-1,i3,2))*u(i1+(0)+off(1),i2+(-1)+off(2),i3+(0)+off(3),cc)
                                   coeff(icf((0),(-1),(0),eqn,cc),i1,
     & i2,i3) = coeff(icf((0),(-1),(0),eqn,cc),i1,i2,i3) + (drar/xyz(
     & i1,i2-1,i3,2))*u(i1+(0)+off(1),i2+(-1)+off(2),i3+(0)+off(3),rc)
c      linearize terms like drar/xyz(i1,i2+1,i3,2) * u(rc)* u(cc) at the grid point offset by 0, +1, 0
                                   coeff(icf((0),(+1),(0),eqn,rc),i1,
     & i2,i3) = coeff(icf((0),(+1),(0),eqn,rc),i1,i2,i3) + (drar/xyz(
     & i1,i2+1,i3,2))*u(i1+(0)+off(1),i2+(+1)+off(2),i3+(0)+off(3),cc)
                                   coeff(icf((0),(+1),(0),eqn,cc),i1,
     & i2,i3) = coeff(icf((0),(+1),(0),eqn,cc),i1,i2,i3) + (drar/xyz(
     & i1,i2+1,i3,2))*u(i1+(0)+off(1),i2+(+1)+off(2),i3+(0)+off(3),rc)
                           else
c      linearize terms like ((1)/(xyz(i1,i2,i3,2))) * u(rc)* u(cc) at the grid point offset by 0, 0, 0
                                    coeff(icf((0),(0),(0),eqn,rc),i1,
     & i2,i3) = coeff(icf((0),(0),(0),eqn,rc),i1,i2,i3) + (((1)/(xyz(
     & i1,i2,i3,2))))*u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)+off(3),cc)
                                    coeff(icf((0),(0),(0),eqn,cc),i1,
     & i2,i3) = coeff(icf((0),(0),(0),eqn,cc),i1,i2,i3) + (((1)/(xyz(
     & i1,i2,i3,2))))*u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)+off(3),rc)
                           endif

                  else
                  drar = dri2(1)*jaci*jac(i1-1,i2,i3)*
     & (xyz(i1,i2,i3,2)-xyz(i1-1,i2,i3,2))*rx(i1-1,i2,i3,1,d)/
     &                           xyz(i1,i2,i3,2)
c      linearize terms like drar * u(rc)* u(cc) at the grid point offset by -1, 0, 0
                        coeff(icf((-1),(0),(0),eqn,rc),i1,i2,i3) = 
     & coeff(icf((-1),(0),(0),eqn,rc),i1,i2,i3) + (drar)*u(i1+(-1)+
     & off(1),i2+(0)+off(2),i3+(0)+off(3),cc)
                        coeff(icf((-1),(0),(0),eqn,cc),i1,i2,i3) = 
     & coeff(icf((-1),(0),(0),eqn,cc),i1,i2,i3) + (drar)*u(i1+(-1)+
     & off(1),i2+(0)+off(2),i3+(0)+off(3),rc)

                  drar = dri2(1)*jaci*jac(i1+1,i2,i3)*
     & (xyz(i1+1,i2,i3,2)-xyz(i1,i2,i3,2))*rx(i1+1,i2,i3,1,d)/
     &                             xyz(i1,i2,i3,2)
c      linearize terms like drar * u(rc)* u(cc) at the grid point offset by +1, 0, 0
                        coeff(icf((+1),(0),(0),eqn,rc),i1,i2,i3) = 
     & coeff(icf((+1),(0),(0),eqn,rc),i1,i2,i3) + (drar)*u(i1+(+1)+
     & off(1),i2+(0)+off(2),i3+(0)+off(3),cc)
                        coeff(icf((+1),(0),(0),eqn,cc),i1,i2,i3) = 
     & coeff(icf((+1),(0),(0),eqn,cc),i1,i2,i3) + (drar)*u(i1+(+1)+
     & off(1),i2+(0)+off(2),i3+(0)+off(3),rc)

                  drar = dri2(2)*jaci*jac(i1,i2-1,i3)*
     & (xyz(i1,i2,i3,2)-xyz(i1,i2-1,i3,2))*rx(i1,i2-1,i3,2,d)/
     &                           xyz(i1,i2,i3,2)
c      linearize terms like drar * u(rc)* u(cc) at the grid point offset by 0, -1, 0
                        coeff(icf((0),(-1),(0),eqn,rc),i1,i2,i3) = 
     & coeff(icf((0),(-1),(0),eqn,rc),i1,i2,i3) + (drar)*u(i1+(0)+off(
     & 1),i2+(-1)+off(2),i3+(0)+off(3),cc)
                        coeff(icf((0),(-1),(0),eqn,cc),i1,i2,i3) = 
     & coeff(icf((0),(-1),(0),eqn,cc),i1,i2,i3) + (drar)*u(i1+(0)+off(
     & 1),i2+(-1)+off(2),i3+(0)+off(3),rc)

                  drar = dri2(2)*jaci*jac(i1,i2+1,i3)*
     & (xyz(i1,i2+1,i3,2)-xyz(i1,i2,i3,2))*rx(i1,i2+1,i3,2,d)/
     &                           xyz(i1,i2,i3,2)
c      linearize terms like drar * u(rc)* u(cc) at the grid point offset by 0, +1, 0
                        coeff(icf((0),(+1),(0),eqn,rc),i1,i2,i3) = 
     & coeff(icf((0),(+1),(0),eqn,rc),i1,i2,i3) + (drar)*u(i1+(0)+off(
     & 1),i2+(+1)+off(2),i3+(0)+off(3),cc)
                        coeff(icf((0),(+1),(0),eqn,cc),i1,i2,i3) = 
     & coeff(icf((0),(+1),(0),eqn,cc),i1,i2,i3) + (drar)*u(i1+(0)+off(
     & 1),i2+(+1)+off(2),i3+(0)+off(3),rc)

                  endif ! non-macro code
c                  if ( .false. ) then
                  if ( usestrik.and. .not. oldstrik ) then
                     drar = dri(1)*jaci*jac(i1+1,i2,i3)*
     & (xyz(i1+1,i2,i3,2)-xyz(i1,i2,i3,2))*rx(i1+1,i2,i3,1,d)/
     &                    xyz(i1,i2,i3,2)
c      linearize terms like -alpha*drar * u(rc)* u(cc) at the grid point offset by +1, 0, 0
                           coeff(icf((+1),(0),(0),eqn,rc),i1,i2,i3) = 
     & coeff(icf((+1),(0),(0),eqn,rc),i1,i2,i3) + (-alpha*drar)*u(i1+(
     & +1)+off(1),i2+(0)+off(2),i3+(0)+off(3),cc)
                           coeff(icf((+1),(0),(0),eqn,cc),i1,i2,i3) = 
     & coeff(icf((+1),(0),(0),eqn,cc),i1,i2,i3) + (-alpha*drar)*u(i1+(
     & +1)+off(1),i2+(0)+off(2),i3+(0)+off(3),rc)
                     drar = dri(1)*jaci*jac(i1-1,i2,i3)*
     & (xyz(i1-1,i2,i3,2)-xyz(i1,i2,i3,2))*rx(i1-1,i2,i3,1,d)/
     &                    xyz(i1,i2,i3,2)
c      linearize terms like -3d0*alpha*drar * u(rc)* u(cc) at the grid point offset by -1, 0, 0
                           coeff(icf((-1),(0),(0),eqn,rc),i1,i2,i3) = 
     & coeff(icf((-1),(0),(0),eqn,rc),i1,i2,i3) + (-3d0*alpha*drar)*u(
     & i1+(-1)+off(1),i2+(0)+off(2),i3+(0)+off(3),cc)
                           coeff(icf((-1),(0),(0),eqn,cc),i1,i2,i3) = 
     & coeff(icf((-1),(0),(0),eqn,cc),i1,i2,i3) + (-3d0*alpha*drar)*u(
     & i1+(-1)+off(1),i2+(0)+off(2),i3+(0)+off(3),rc)
                     drar = dri(1)*jaci*jac(i1-2,i2,i3)*
     & (xyz(i1-2,i2,i3,2)-xyz(i1,i2,i3,2))*rx(i1-2,i2,i3,1,d)/
     &                    xyz(i1,i2,i3,2)
c      linearize terms like alpha*drar * u(rc)* u(cc) at the grid point offset by -2, 0, 0
                           coeff(icf((-2),(0),(0),eqn,rc),i1,i2,i3) = 
     & coeff(icf((-2),(0),(0),eqn,rc),i1,i2,i3) + (alpha*drar)*u(i1+(-
     & 2)+off(1),i2+(0)+off(2),i3+(0)+off(3),cc)
                           coeff(icf((-2),(0),(0),eqn,cc),i1,i2,i3) = 
     & coeff(icf((-2),(0),(0),eqn,cc),i1,i2,i3) + (alpha*drar)*u(i1+(-
     & 2)+off(1),i2+(0)+off(2),i3+(0)+off(3),rc)

                     drar = dri(2)*jaci*jac(i1,i2+1,i3)*
     & (xyz(i1,i2+1,i3,2)-xyz(i1,i2,i3,2))*rx(i1,i2+1,i3,2,d)/
     &                    xyz(i1,i2,i3,2)
c      linearize terms like -alpha*drar * u(rc)* u(cc) at the grid point offset by 0, +1, 0
                           coeff(icf((0),(+1),(0),eqn,rc),i1,i2,i3) = 
     & coeff(icf((0),(+1),(0),eqn,rc),i1,i2,i3) + (-alpha*drar)*u(i1+(
     & 0)+off(1),i2+(+1)+off(2),i3+(0)+off(3),cc)
                           coeff(icf((0),(+1),(0),eqn,cc),i1,i2,i3) = 
     & coeff(icf((0),(+1),(0),eqn,cc),i1,i2,i3) + (-alpha*drar)*u(i1+(
     & 0)+off(1),i2+(+1)+off(2),i3+(0)+off(3),rc)
                     drar = dri(2)*jaci*jac(i1,i2-1,i3)*
     & (xyz(i1,i2-1,i3,2)-xyz(i1,i2,i3,2))*rx(i1,i2-1,i3,2,d)/
     &                    xyz(i1,i2,i3,2)
c      linearize terms like -3d0*alpha*drar * u(rc)* u(cc) at the grid point offset by 0, -1, 0
                           coeff(icf((0),(-1),(0),eqn,rc),i1,i2,i3) = 
     & coeff(icf((0),(-1),(0),eqn,rc),i1,i2,i3) + (-3d0*alpha*drar)*u(
     & i1+(0)+off(1),i2+(-1)+off(2),i3+(0)+off(3),cc)
                           coeff(icf((0),(-1),(0),eqn,cc),i1,i2,i3) = 
     & coeff(icf((0),(-1),(0),eqn,cc),i1,i2,i3) + (-3d0*alpha*drar)*u(
     & i1+(0)+off(1),i2+(-1)+off(2),i3+(0)+off(3),rc)
                     drar = dri(2)*jaci*jac(i1,i2-2,i3)*
     & (xyz(i1,i2-2,i3,2)-xyz(i1,i2,i3,2))*rx(i1,i2-2,i3,2,d)/
     &                    xyz(i1,i2,i3,2)
c      linearize terms like alpha*drar * u(rc)* u(cc) at the grid point offset by 0, -2, 0
                           coeff(icf((0),(-2),(0),eqn,rc),i1,i2,i3) = 
     & coeff(icf((0),(-2),(0),eqn,rc),i1,i2,i3) + (alpha*drar)*u(i1+(
     & 0)+off(1),i2+(-2)+off(2),i3+(0)+off(3),cc)
                           coeff(icf((0),(-2),(0),eqn,cc),i1,i2,i3) = 
     & coeff(icf((0),(-2),(0),eqn,cc),i1,i2,i3) + (alpha*drar)*u(i1+(
     & 0)+off(1),i2+(-2)+off(2),i3+(0)+off(3),rc)

                  endif
c                  endif

               end do ! ndim

            endif ! axi for continuity

c
c     MOMENTUM EQUATIONS
c

            if ( .true. .or.
     &           (i1.gt.igi(1,1) .and. i1.lt.igi(2,1) .and.
     &           i2.gt.igi(1,2) .and. i2.lt.igi(2,2)) ) then
c
c     LINEARIZED CONVECTIVE DERIVATIVE (U\dot\grad v) for u,v,w,T
c
c      the general (nonlinear) form is :
c          u[uc] u[eqn]_x + u[vc] u[eqn]_y 
c             - or in (r,s) -
c          u[uc] ( r_x u[eqn]_r + s_x u[eqn]_s ) + u[vc] ( r_y u[eqn]_r + s_y u[eqn]_s )
c             - or in the loop -
c          += u[cc] ( r_{cc-uc+1} u[eqn]_r + s_{cc-uc+1} u[eqn]_s )
c       
c          all the linearizations will contribute terms to the RHS
            do eqn=uc,tc

               do cc=uc, uc+ndim-1
                  d = cc-uc+1

c                 u[cc]^{n+1} * D_d (u[eqn]^n)
c                  coeff(icf(0,0,0,eqn,cc),i1,i2,i3) =
c     &                 coeff(icf(0,0,0,eqn,cc),i1,i2,i3) + 
c     &                    ux(i1,i2,i3,eqn,d)
c     &                 rx(i1,i2,i3,1,d)*ur + rx(i1,i2,i3,2,d)*us

c                 u[cc]^n * (r_d u[eqn]_r^{n+1} + s_d u[eqn]_s^{n+1} )
c                  UXC(d, (u(i1,i2,i3,cc)), eqn)
c     coefficients for a central difference approximation to 1d0 * u[cc] u[eqn]_X
c                 u[cc]^{n+1} * u[eqn]^n_X
                                    coeff(icf(0,0,0,eqn,cc),i1,i2,i3) 
     & = coeff(icf(0,0,0,eqn,cc),i1,i2,i3) + (1d0) * ux(i1,i2,i3,eqn,
     & d)
c                 u[cc]^n * (r_X u[eqn]_r^{n+1} + s_X u[eqn]_s^{n+1} )
c                 r - part
                                    coeff(icf(+1,0,0,eqn,eqn),i1,i2,i3)
     &  =  coeff(icf(+1,0,0,eqn,eqn),i1,i2,i3) + (1d0) * rx(i1+off(1),
     & i2+off(2),i3+off(3),1,d)*u(i1+off(1),i2+off(2),i3+off(3),cc)*
     & dri2(1)
                                    coeff(icf(-1,0,0,eqn,eqn),i1,i2,i3)
     &  =  coeff(icf(-1,0,0,eqn,eqn),i1,i2,i3) - (1d0) * rx(i1+off(1),
     & i2+off(2),i3+off(3),1,d)*u(i1+off(1),i2+off(2),i3+off(3),cc)*
     & dri2(1)
c                 s - part
                                    coeff(icf(0,+1,0,eqn,eqn),i1,i2,i3)
     &  =  coeff(icf(0,+1,0,eqn,eqn),i1,i2,i3) + (1d0) * rx(i1+off(1),
     & i2+off(2),i3+off(3),2,d)*u(i1+off(1),i2+off(2),i3+off(3),cc)*
     & dri2(2)
                                    coeff(icf(0,-1,0,eqn,eqn),i1,i2,i3)
     &  =  coeff(icf(0,-1,0,eqn,eqn),i1,i2,i3) - (1d0) * rx(i1+off(1),
     & i2+off(2),i3+off(3),2,d)*u(i1+off(1),i2+off(2),i3+off(3),cc)*
     & dri2(2)
               end do ! cc (each vel. component)

            end do ! eqn

c
c     PRESSURE GRADIENT FOR MOMENTUM EQS
c
c     The pressure derivative looks like 
c         (u[rc] u[tc])_x/(u[rc] gamma M^2) 
c     where x is any one of the coordinate (physical) directions.
c     Note that this relies on the Mach number having been set to some reasonable value consistent
c     with whatever nondimensionalization one chooses.  Expanding this equation gets us
c         u[tc]_x/(gamma M^2) + u[tc] u[rc]_x/(u[rc] gamma M^2)
c           - or in (r,s) coordinates -
c         1/(gamma M^2) ( r_x u[tc]_r + s_x u[tc]_s + u[tc] ( r_x u[rc]_r + s_x u[rc]_s)/u[rc] )
c
c
            do eqn=uc,uc+ndim-1
ctest            do eqn=vc,uc+ndim-1

               cc = eqn - uc + 1 ! coordinate direction index
c           linear part:
c           gm2 * (r_c u[tc]_r + s_c u[tc]_s)
c     
               if ( .false. .and.
     &              usestrik ) then
c     coefficients for a 3rd order  difference approximation to gm2 * u[tc]_X
c                  (r_X u[tc]_r^{n+1} + s_X u[tc]_s^{n+1} )
c                 r - part
                           coeff(icf(-1,0,0,eqn,tc),i1,i2,i3) =  coeff(
     & icf(-1,0,0,eqn,tc),i1,i2,i3) + (gm2) * (alpha-.5d0)    *rx(i1+
     & off(1),i2+off(2),i3+off(3),1,cc)*dri(1)
                           coeff(icf( 0,0,0,eqn,tc),i1,i2,i3) =  coeff(
     & icf( 0,0,0,eqn,tc),i1,i2,i3) - (gm2) * (3d0*alpha)     *rx(i1+
     & off(1),i2+off(2),i3+off(3),1,cc)*dri(1)
                           coeff(icf(+1,0,0,eqn,tc),i1,i2,i3) =  coeff(
     & icf(+1,0,0,eqn,tc),i1,i2,i3) + (gm2) * (.5d0+3d0*alpha)*rx(i1+
     & off(1),i2+off(2),i3+off(3),1,cc)*dri(1)
                           coeff(icf(+2,0,0,eqn,tc),i1,i2,i3) =  coeff(
     & icf(+2,0,0,eqn,tc),i1,i2,i3) - (gm2) * (alpha)         *rx(i1+
     & off(1),i2+off(2),i3+off(3),1,cc)*dri(1)
c                 s - part
                           coeff(icf(0 ,-1,0,eqn,tc),i1,i2,i3) =  
     & coeff(icf(0,-1,0,eqn,tc),i1,i2,i3) + (gm2) * (alpha-.5d0)    *
     & rx(i1+off(1),i2+off(2),i3+off(3),2,cc)*dri(2)
                           coeff(icf(0 , 0,0,eqn,tc),i1,i2,i3) =  
     & coeff(icf(0, 0,0,eqn,tc),i1,i2,i3) - (gm2) * (3d0*alpha)     *
     & rx(i1+off(1),i2+off(2),i3+off(3),2,cc)*dri(2)
                           coeff(icf(0 ,+1,0,eqn,tc),i1,i2,i3) =  
     & coeff(icf(0,+1,0,eqn,tc),i1,i2,i3) + (gm2) * (.5d0+3d0*alpha)*
     & rx(i1+off(1),i2+off(2),i3+off(3),2,cc)*dri(2)
                           coeff(icf(0 ,+2,0,eqn,tc),i1,i2,i3) =  
     & coeff(icf(0,+2,0,eqn,tc),i1,i2,i3) - (gm2) * (alpha)         *
     & rx(i1+off(1),i2+off(2),i3+off(3),2,cc)*dri(2)
               else
c     coefficients for a  difference approximation to gm2 * u[tc]_X
c     r-part
                        coeff(icf(+1,0,0,eqn,tc),i1,i2,i3) = coeff(icf(
     & +1,0,0,eqn,tc),i1,i2,i3) + (gm2)*rx(i1+off(1),i2+off(2),i3+off(
     & 3),1,cc)*dri2(1)
                        coeff(icf(-1,0,0,eqn,tc),i1,i2,i3) = coeff(icf(
     & -1,0,0,eqn,tc),i1,i2,i3) - (gm2)*rx(i1+off(1),i2+off(2),i3+off(
     & 3),1,cc)*dri2(1)
c     s-part
                        coeff(icf(0,+1,0,eqn,tc),i1,i2,i3) = coeff(icf(
     & 0,+1,0,eqn,tc),i1,i2,i3) + (gm2)*rx(i1+off(1),i2+off(2),i3+off(
     & 3),2,cc)*dri2(2)
                        coeff(icf(0,-1,0,eqn,tc),i1,i2,i3) = coeff(icf(
     & 0,-1,0,eqn,tc),i1,i2,i3) - (gm2)*rx(i1+off(1),i2+off(2),i3+off(
     & 3),2,cc)*dri2(2)
               endif

               if ( .false. ) then
c
c           nonlinear part:
c          gm2 * u[tc] * ( r_c u[rc]_r + s_c u[rc]_s)/u[rc]
c          each triple like u[tc] u[rc]_r/u[rc] will contribute 3 terms in the linearization :
c              (u[tc]^{n+1} u[rc]^n_r + u[tc]^n u[rc]^{n+1}_r - u[rc]^{n+1} u[tc]^n u[rc]^{n}_r/u[rc]^n )/u[rc]^n
c              There will be no entries from this linearization into the RHS
c     
c              gm2 * u[tc]^{n+1}*( r_c u[rc]^n_r + s_c u[rc]^n_s )/u[rc]^n
               coeff(icf(0,0,0,eqn,tc),i1,i2,i3) =
     &           coeff(icf(0,0,0,eqn,tc),i1,i2,i3) +
     &           gm2*ux(i1,i2,i3,rc,cc)/u(i1,i2,i3,rc)

c              gm2 * u[tc]^{n}*( r_c u[rc]^{n+1}_r + s_c u[rc]^{n+1}_s )/u[rc]^n
c     coefficients for a  difference approximation to (u(i1,i2,i3,tc)*gm2/u(i1,i2,i3,rc)) * u[rc]_X
c     r-part
                     coeff(icf(+1,0,0,eqn,rc),i1,i2,i3) = coeff(icf(+1,
     & 0,0,eqn,rc),i1,i2,i3) + ((u(i1,i2,i3,tc)*gm2/u(i1,i2,i3,rc)))*
     & rx(i1+off(1),i2+off(2),i3+off(3),1,cc)*dri2(1)
                     coeff(icf(-1,0,0,eqn,rc),i1,i2,i3) = coeff(icf(-1,
     & 0,0,eqn,rc),i1,i2,i3) - ((u(i1,i2,i3,tc)*gm2/u(i1,i2,i3,rc)))*
     & rx(i1+off(1),i2+off(2),i3+off(3),1,cc)*dri2(1)
c     s-part
                     coeff(icf(0,+1,0,eqn,rc),i1,i2,i3) = coeff(icf(0,+
     & 1,0,eqn,rc),i1,i2,i3) + ((u(i1,i2,i3,tc)*gm2/u(i1,i2,i3,rc)))*
     & rx(i1+off(1),i2+off(2),i3+off(3),2,cc)*dri2(2)
                     coeff(icf(0,-1,0,eqn,rc),i1,i2,i3) = coeff(icf(0,-
     & 1,0,eqn,rc),i1,i2,i3) - ((u(i1,i2,i3,tc)*gm2/u(i1,i2,i3,rc)))*
     & rx(i1+off(1),i2+off(2),i3+off(3),2,cc)*dri2(2)

c           - gm2*u[rc]^{n+1} u[tc]^n ( r_c u[rc]^n_r + s_c u[rc]^n_s)/u[rc]^n /[rc]^n
               coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &           coeff(icf(0,0,0,eqn,rc),i1,i2,i3) -
     & gm2*u(i1,i2,i3,tc)*ux(i1,i2,i3,rc,cc)/(u(i1,i2,i3,rc)**2)

               else ! discretize the gradient of log(rho)

                  if ( oldstrik ) then
c          we linearize a discretized version of gm2*u[tc]*log(u[rc])_c
                     lur = (dlog(dabs(u(i1+1,i2,i3,rc)))-dlog(dabs(u(
     & i1-1,i2,i3,rc))))
                     lus = (dlog(dabs(u(i1,i2+1,i3,rc)))-dlog(dabs(u(
     & i1,i2-1,i3,rc))))

c           gm2*u[tc]^{n+1}*log(u[rc]^n)_c
                     coeff(icf(0,0,0,eqn,tc),i1,i2,i3) =
     &                    coeff(icf(0,0,0,eqn,tc),i1,i2,i3) +
     &                    gm2*( rx(i1,i2,i3,1,cc)*dri2(1)*lur +
     &                    rx(i1,i2,i3,2,cc)*dri2(2)*lus )

c           the n+1 part of the
c            gm2*u[tc]^{n}*log(u[rc]^{n+1})_c
c           linearization is
c            gm2*u[tc]^{n}*( rx(1,cc)*dri2*( u[rc]^{n+1}_{i+1}/u[rc]^n{i+1} - 
c                                            u[rc]^{n+1}_{i-1}/u[rc]^n{i-1} ) +
c                            rx(2,cc)*dri2*( u[rc]^{n+1}_{j+1}/u[rc]^n{j+1} - 
c                                            u[rc]^{n+1}_{j-1}/u[rc]^n{j-1} ) )
c     
                     coeff(icf(+1,0,0,eqn,rc),i1,i2,i3) =
     &                    coeff(icf(+1,0,0,eqn,rc),i1,i2,i3) +
     &                    gm2*u(i1,i2,i3,tc)*dri2(1)*rx(i1,i2,i3,1,cc)/
     &                    u(i1+1,i2,i3,rc)

                     coeff(icf(-1,0,0,eqn,rc),i1,i2,i3) =
     &                    coeff(icf(-1,0,0,eqn,rc),i1,i2,i3) -
     &                    gm2*u(i1,i2,i3,tc)*dri2(1)*rx(i1,i2,i3,1,cc)/
     &                    u(i1-1,i2,i3,rc)

                     coeff(icf(0,+1,0,eqn,rc),i1,i2,i3) =
     &                    coeff(icf(0,+1,0,eqn,rc),i1,i2,i3) +
     &                    gm2*u(i1,i2,i3,tc)*dri2(2)*rx(i1,i2,i3,2,cc)/
     &                    u(i1,i2+1,i3,rc)

                     coeff(icf(0,-1,0,eqn,rc),i1,i2,i3) =
     &                    coeff(icf(0,-1,0,eqn,rc),i1,i2,i3) -
     &                    gm2*u(i1,i2,i3,tc)*dri2(2)*rx(i1,i2,i3,2,cc)/
     &                    u(i1,i2-1,i3,rc)

                     if ( usestrik ) then
c     strikwerda-like correction term
c     coefficients for a 3rd order  difference approximation to (gm2/u(i1,i2,i3,rc)) * u[tc] u[rc]_X
c                 u[tc]^{n+1} * u[rc]^n_X
                              coeff(icf(0,0,0,eqn,tc),i1,i2,i3) = 
     & coeff(icf(0,0,0,eqn,tc),i1,i2,i3) + ((gm2/u(i1,i2,i3,rc))) * 
     & ux4m(i1,i2,i3,rc,cc)
c                 u[tc]^n * (r_X u[rc]_r^{n+1} + s_X u[rc]_s^{n+1} )
c     coefficients for a 3rd order  difference approximation to ((gm2/u(i1,i2,i3,rc)))*u(i1+off(1),i2+off(2),i3+off(3),tc) * u[rc]_X
c                  (r_X u[rc]_r^{n+1} + s_X u[rc]_s^{n+1} )
c                 r - part
                                    coeff(icf(-1,0,0,eqn,rc),i1,i2,i3) 
     & =  coeff(icf(-1,0,0,eqn,rc),i1,i2,i3) + (((gm2/u(i1,i2,i3,rc)))
     & *u(i1+off(1),i2+off(2),i3+off(3),tc)) * (alpha-.5d0)    *rx(i1+
     & off(1),i2+off(2),i3+off(3),1,cc)*dri(1)
                                    coeff(icf( 0,0,0,eqn,rc),i1,i2,i3) 
     & =  coeff(icf( 0,0,0,eqn,rc),i1,i2,i3) - (((gm2/u(i1,i2,i3,rc)))
     & *u(i1+off(1),i2+off(2),i3+off(3),tc)) * (3d0*alpha)     *rx(i1+
     & off(1),i2+off(2),i3+off(3),1,cc)*dri(1)
                                    coeff(icf(+1,0,0,eqn,rc),i1,i2,i3) 
     & =  coeff(icf(+1,0,0,eqn,rc),i1,i2,i3) + (((gm2/u(i1,i2,i3,rc)))
     & *u(i1+off(1),i2+off(2),i3+off(3),tc)) * (.5d0+3d0*alpha)*rx(i1+
     & off(1),i2+off(2),i3+off(3),1,cc)*dri(1)
                                    coeff(icf(+2,0,0,eqn,rc),i1,i2,i3) 
     & =  coeff(icf(+2,0,0,eqn,rc),i1,i2,i3) - (((gm2/u(i1,i2,i3,rc)))
     & *u(i1+off(1),i2+off(2),i3+off(3),tc)) * (alpha)         *rx(i1+
     & off(1),i2+off(2),i3+off(3),1,cc)*dri(1)
c                 s - part
                                    coeff(icf(0 ,-1,0,eqn,rc),i1,i2,i3)
     &  =  coeff(icf(0,-1,0,eqn,rc),i1,i2,i3) + (((gm2/u(i1,i2,i3,rc))
     & )*u(i1+off(1),i2+off(2),i3+off(3),tc)) * (alpha-.5d0)    *rx(
     & i1+off(1),i2+off(2),i3+off(3),2,cc)*dri(2)
                                    coeff(icf(0 , 0,0,eqn,rc),i1,i2,i3)
     &  =  coeff(icf(0, 0,0,eqn,rc),i1,i2,i3) - (((gm2/u(i1,i2,i3,rc))
     & )*u(i1+off(1),i2+off(2),i3+off(3),tc)) * (3d0*alpha)     *rx(
     & i1+off(1),i2+off(2),i3+off(3),2,cc)*dri(2)
                                    coeff(icf(0 ,+1,0,eqn,rc),i1,i2,i3)
     &  =  coeff(icf(0,+1,0,eqn,rc),i1,i2,i3) + (((gm2/u(i1,i2,i3,rc))
     & )*u(i1+off(1),i2+off(2),i3+off(3),tc)) * (.5d0+3d0*alpha)*rx(
     & i1+off(1),i2+off(2),i3+off(3),2,cc)*dri(2)
                                    coeff(icf(0 ,+2,0,eqn,rc),i1,i2,i3)
     &  =  coeff(icf(0,+2,0,eqn,rc),i1,i2,i3) - (((gm2/u(i1,i2,i3,rc))
     & )*u(i1+off(1),i2+off(2),i3+off(3),tc)) * (alpha)         *rx(
     & i1+off(1),i2+off(2),i3+off(3),2,cc)*dri(2)
c                 r - part
c      coeff(icf(-1,0,0,eqn,rc),i1,i2,i3) =  coeff(icf(-1,0,0,eqn,rc),i1,i2,i3) + ((gm2/u(i1,i2,i3,rc))) * (alpha-.5d0)    *rx(i1+off(1),i2+off(2),i3+off(3),1,cc)*u(i1+off(1),i2+off(2),i3+off(3),tc)*dri(1)
c      coeff(icf( 0,0,0,eqn,rc),i1,i2,i3) =  coeff(icf( 0,0,0,eqn,rc),i1,i2,i3) - ((gm2/u(i1,i2,i3,rc))) * (3d0*alpha)     *rx(i1+off(1),i2+off(2),i3+off(3),1,cc)*u(i1+off(1),i2+off(2),i3+off(3),tc)*dri(1)
c      coeff(icf(+1,0,0,eqn,rc),i1,i2,i3) =  coeff(icf(+1,0,0,eqn,rc),i1,i2,i3) + ((gm2/u(i1,i2,i3,rc))) * (.5d0+3d0*alpha)*rx(i1+off(1),i2+off(2),i3+off(3),1,cc)*u(i1+off(1),i2+off(2),i3+off(3),tc)*dri(1)
c      coeff(icf(+2,0,0,eqn,rc),i1,i2,i3) =  coeff(icf(+2,0,0,eqn,rc),i1,i2,i3) - ((gm2/u(i1,i2,i3,rc))) * (alpha)         *rx(i1+off(1),i2+off(2),i3+off(3),1,cc)*u(i1+off(1),i2+off(2),i3+off(3),tc)*dri(1)
c                 s - part
c      coeff(icf(0,-1,0,eqn,rc),i1,i2,i3) =  coeff(icf(0,-1,0,eqn,rc),i1,i2,i3) + ((gm2/u(i1,i2,i3,rc))) * (alpha-.5d0)    *rx(i1+off(1),i2+off(2),i3+off(3),2,cc)*u(i1+off(1),i2+off(2),i3+off(3),tc)*dri(2)
c      coeff(icf( 0,0,0,eqn,rc),i1,i2,i3) =  coeff(icf(0,0,0,eqn,rc),i1,i2,i3)  - ((gm2/u(i1,i2,i3,rc))) * (3d0*alpha)     *rx(i1+off(1),i2+off(2),i3+off(3),2,cc)*u(i1+off(1),i2+off(2),i3+off(3),tc)*dri(2)
c      coeff(icf(0,+1,0,eqn,rc),i1,i2,i3) =  coeff(icf(0,+1,0,eqn,rc),i1,i2,i3) + ((gm2/u(i1,i2,i3,rc))) * (.5d0+3d0*alpha)*rx(i1+off(1),i2+off(2),i3+off(3),2,cc)*u(i1+off(1),i2+off(2),i3+off(3),tc)*dri(2)
c      coeff(icf(0,+2,0,eqn,rc),i1,i2,i3) =  coeff(icf(0,+2,0,eqn,rc),i1,i2,i3) - ((gm2/u(i1,i2,i3,rc))) * (alpha)         *rx(i1+off(1),i2+off(2),i3+off(3),2,cc)*u(i1+off(1),i2+off(2),i3+off(3),tc)*dri(2)
                        coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &                       coeff(icf(0,0,0,eqn,rc),i1,i2,i3) -
     & gm2*(ux4m(i1,i2,i3,rc,cc)-ux(i1,i2,i3,rc,cc))*
     & u(i1,i2,i3,tc)/(u(i1,i2,i3,rc)*u(i1,i2,i3,rc))

c     subtract off the part we don't need (done above with log(rho))
c     coefficients for a central difference approximation to (-gm2/u(i1,i2,i3,rc)) * u[tc] u[rc]_X
c                 u[tc]^{n+1} * u[rc]^n_X
                                          coeff(icf(0,0,0,eqn,tc),i1,
     & i2,i3) = coeff(icf(0,0,0,eqn,tc),i1,i2,i3) + ((-gm2/u(i1,i2,i3,
     & rc))) * ux(i1,i2,i3,rc,cc)
c                 u[tc]^n * (r_X u[rc]_r^{n+1} + s_X u[rc]_s^{n+1} )
c                 r - part
                                          coeff(icf(+1,0,0,eqn,rc),i1,
     & i2,i3) =  coeff(icf(+1,0,0,eqn,rc),i1,i2,i3) + ((-gm2/u(i1,i2,
     & i3,rc))) * rx(i1+off(1),i2+off(2),i3+off(3),1,cc)*u(i1+off(1),
     & i2+off(2),i3+off(3),tc)*dri2(1)
                                          coeff(icf(-1,0,0,eqn,rc),i1,
     & i2,i3) =  coeff(icf(-1,0,0,eqn,rc),i1,i2,i3) - ((-gm2/u(i1,i2,
     & i3,rc))) * rx(i1+off(1),i2+off(2),i3+off(3),1,cc)*u(i1+off(1),
     & i2+off(2),i3+off(3),tc)*dri2(1)
c                 s - part
                                          coeff(icf(0,+1,0,eqn,rc),i1,
     & i2,i3) =  coeff(icf(0,+1,0,eqn,rc),i1,i2,i3) + ((-gm2/u(i1,i2,
     & i3,rc))) * rx(i1+off(1),i2+off(2),i3+off(3),2,cc)*u(i1+off(1),
     & i2+off(2),i3+off(3),tc)*dri2(2)
                                          coeff(icf(0,-1,0,eqn,rc),i1,
     & i2,i3) =  coeff(icf(0,-1,0,eqn,rc),i1,i2,i3) - ((-gm2/u(i1,i2,
     & i3,rc))) * rx(i1+off(1),i2+off(2),i3+off(3),2,cc)*u(i1+off(1),
     & i2+off(2),i3+off(3),tc)*dri2(2)
                     endif

                  else

c     coefficients for a 3rd order  difference approximation to gm2 *  u[tc]*(log(u[rc]))_X
c                 u[tc]^{n+1} * u[rc]^n_X
                         coeff(icf(0,0,0,eqn,tc),i1,i2,i3) = coeff(icf(
     & 0,0,0,eqn,tc),i1,i2,i3) + (gm2) * logux4m(i1+off(1),i2+off(2),
     & i3+off(3),rc,cc)
c                 r - part
                         coeff(icf(-1,0,0,eqn,rc),i1,i2,i3) =  coeff(
     & icf(-1,0,0,eqn,rc),i1,i2,i3) + (gm2) * (alpha-.5d0)    *rx(i1+
     & off(1),i2+off(2),i3+off(3),1,cc)*u(i1+off(1),i2+off(2),i3+off(
     & 3),tc)*dri(1)/u(i1-1+off(1),i2+off(2),i3+off(3),rc)
                         coeff(icf( 0,0,0,eqn,rc),i1,i2,i3) =  coeff(
     & icf( 0,0,0,eqn,rc),i1,i2,i3) - (gm2) * (3d0*alpha)     *rx(i1+
     & off(1),i2+off(2),i3+off(3),1,cc)*u(i1+off(1),i2+off(2),i3+off(
     & 3),tc)*dri(1)/u(i1  +off(1),  i2+off(2),i3+off(3),rc)
                         coeff(icf(+1,0,0,eqn,rc),i1,i2,i3) =  coeff(
     & icf(+1,0,0,eqn,rc),i1,i2,i3) + (gm2) * (.5d0+3d0*alpha)*rx(i1+
     & off(1),i2+off(2),i3+off(3),1,cc)*u(i1+off(1),i2+off(2),i3+off(
     & 3),tc)*dri(1)/u(i1+1+off(1),i2+off(2),i3+off(3),rc)
                         if ( usestrik ) then
                         coeff(icf(+2,0,0,eqn,rc),i1,i2,i3) =  coeff(
     & icf(+2,0,0,eqn,rc),i1,i2,i3) - (gm2) * (alpha)         *rx(i1+
     & off(1),i2+off(2),i3+off(3),1,cc)*u(i1+off(1),i2+off(2),i3+off(
     & 3),tc)*dri(1)/u(i1+2+off(1),i2+off(2),i3+off(3),rc)
                         endif
c                 s - part
                         coeff(icf(0,-1,0,eqn,rc),i1,i2,i3) =  coeff(
     & icf(0,-1,0,eqn,rc),i1,i2,i3) + (gm2) * (alpha-.5d0)    *rx(i1+
     & off(1),i2+off(2),i3+off(3),2,cc)*u(i1+off(1),i2+off(2),i3+off(
     & 3),tc)*dri(2)/u(i1+off(1),i2-1+off(2),i3+off(3),rc)
                         coeff(icf( 0,0,0,eqn,rc),i1,i2,i3) =  coeff(
     & icf(0,0,0,eqn,rc),i1,i2,i3)  - (gm2) * (3d0*alpha)     *rx(i1+
     & off(1),i2+off(2),i3+off(3),2,cc)*u(i1+off(1),i2+off(2),i3+off(
     & 3),tc)*dri(2)/u(i1+off(1),i2  +off(2),i3+off(3),rc)
                         coeff(icf(0,+1,0,eqn,rc),i1,i2,i3) =  coeff(
     & icf(0,+1,0,eqn,rc),i1,i2,i3) + (gm2) * (.5d0+3d0*alpha)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,cc)*u(i1+off(1),i2+off(2),i3+off(
     & 3),tc)*dri(2)/u(i1+off(1),i2+1+off(2),i3+off(3),rc)
                         if ( usestrik ) then
                         coeff(icf(0,+2,0,eqn,rc),i1,i2,i3) =  coeff(
     & icf(0,+2,0,eqn,rc),i1,i2,i3) - (gm2) * (alpha)         *rx(i1+
     & off(1),i2+off(2),i3+off(3),2,cc)*u(i1+off(1),i2+off(2),i3+off(
     & 3),tc)*dri(2)/u(i1+off(1),i2+2+off(2),i3+off(3),rc)
                         endif
c                     ULOGUX4PC(cc,gm2,tc,rc)
c                     if ( i1.eq.5 ) then
c                  print *, i1,i2,logux4m(i1,i2,i3,rc,cc)/
c     &                    (2d0*xyz(i1,i2,i3,2))
c                  print *, i1,i2,logux(i1,i2,i3,rc,cc)/
c     &                    (2d0*xyz(i1,i2,i3,2))
c                  print *, i1,i2,u(i1,i2,i3,wc)**2/
c     &                    (2d0*xyz(i1,i2,i3,2)**2)
c                     endif
                  endif         ! oldstrik
               endif


            end do ! pressure derivative for eqn

c
c     MOMENTUM EQUATION AXISYMMETRIC FORCING TERMS (NON-VISCOUS)
c
            if ( isaxi.gt.0 .and. xyz(i1,i2,i3,2).gt.0d0 ) then

c      we only have these terms in the momentum equation if we have swirl

               if ( isswirl.gt.0 ) then
                  if ( avgmomforcing ) then
                           if ( .false. ) then
                           if ( .false. ) then
c         do d=2,2
                                 eqn=vc
                                       if ( .true. ) then
                                       drar = (-1d0)*dri2(1)*jaci*jac(
     & i1+off(1)-1,i2+off(2),i3+off(3))*(xyz(i1+off(1),i2+off(2),i3+
     & off(3),2)-xyz(i1+off(1)-1,i2+off(2),i3+off(3),2))*rx(i1+off(1)-
     & 1,i2+off(2),i3+off(3),1,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
c      linearize terms like drar * u(wc)* u(wc) at the grid point offset by -1, 0, 0
                                             coeff(icf((-1),(0),(0),
     & eqn,wc),i1,i2,i3) = coeff(icf((-1),(0),(0),eqn,wc),i1,i2,i3) + 
     & (drar)*u(i1+(-1)+off(1),i2+(0)+off(2),i3+(0)+off(3),wc)
                                             coeff(icf((-1),(0),(0),
     & eqn,wc),i1,i2,i3) = coeff(icf((-1),(0),(0),eqn,wc),i1,i2,i3) + 
     & (drar)*u(i1+(-1)+off(1),i2+(0)+off(2),i3+(0)+off(3),wc)
                                       drar = (-1d0)*dri2(1)*jaci*jac(
     & i1+off(1)+1,i2+off(2),i3+off(3))*(xyz(i1+off(1)+1,i2+off(2),i3+
     & off(3),2)-xyz(i1+off(1),i2+off(2),i3+off(3),2))*rx(i1+off(1)+1,
     & i2+off(2),i3+off(3),1,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
c      linearize terms like drar * u(wc)* u(wc) at the grid point offset by +1, 0, 0
                                             coeff(icf((+1),(0),(0),
     & eqn,wc),i1,i2,i3) = coeff(icf((+1),(0),(0),eqn,wc),i1,i2,i3) + 
     & (drar)*u(i1+(+1)+off(1),i2+(0)+off(2),i3+(0)+off(3),wc)
                                             coeff(icf((+1),(0),(0),
     & eqn,wc),i1,i2,i3) = coeff(icf((+1),(0),(0),eqn,wc),i1,i2,i3) + 
     & (drar)*u(i1+(+1)+off(1),i2+(0)+off(2),i3+(0)+off(3),wc)
                                       drar = (-1d0)*dri2(2)*jaci*jac(
     & i1+off(1),i2+off(2)-1,i3+off(3))*(xyz(i1+off(1),i2+off(2),i3+
     & off(3),2)-xyz(i1+off(1),i2+off(2)-1,i3+off(3),2))*rx(i1+off(1),
     & i2+off(2)-1,i3+off(3),2,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
c      linearize terms like drar * u(wc)* u(wc) at the grid point offset by 0, -1, 0
                                             coeff(icf((0),(-1),(0),
     & eqn,wc),i1,i2,i3) = coeff(icf((0),(-1),(0),eqn,wc),i1,i2,i3) + 
     & (drar)*u(i1+(0)+off(1),i2+(-1)+off(2),i3+(0)+off(3),wc)
                                             coeff(icf((0),(-1),(0),
     & eqn,wc),i1,i2,i3) = coeff(icf((0),(-1),(0),eqn,wc),i1,i2,i3) + 
     & (drar)*u(i1+(0)+off(1),i2+(-1)+off(2),i3+(0)+off(3),wc)
                                       drar = (-1d0)*dri2(2)*jaci*jac(
     & i1+off(1),i2+off(2)+1,i3+off(3))*(xyz(i1+off(1),i2+off(2)+1,i3+
     & off(3),2)-xyz(i1+off(1),i2+off(2),i3+off(3),2))*rx(i1+off(1),
     & i2+off(2)+1,i3+off(3),2,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
c      linearize terms like drar * u(wc)* u(wc) at the grid point offset by 0, +1, 0
                                             coeff(icf((0),(+1),(0),
     & eqn,wc),i1,i2,i3) = coeff(icf((0),(+1),(0),eqn,wc),i1,i2,i3) + 
     & (drar)*u(i1+(0)+off(1),i2+(+1)+off(2),i3+(0)+off(3),wc)
                                             coeff(icf((0),(+1),(0),
     & eqn,wc),i1,i2,i3) = coeff(icf((0),(+1),(0),eqn,wc),i1,i2,i3) + 
     & (drar)*u(i1+(0)+off(1),i2+(+1)+off(2),i3+(0)+off(3),wc)
                                       else if ( .false. ) then
                                          drar = (-1d0)*.25d0
c      linearize terms like drar/xyz(i1-1,i2,i3,2) * u(wc)* u(wc) at the grid point offset by -1, 0, 0
                                               coeff(icf((-1),(0),(0),
     & eqn,wc),i1,i2,i3) = coeff(icf((-1),(0),(0),eqn,wc),i1,i2,i3) + 
     & (drar/xyz(i1-1,i2,i3,2))*u(i1+(-1)+off(1),i2+(0)+off(2),i3+(0)+
     & off(3),wc)
                                               coeff(icf((-1),(0),(0),
     & eqn,wc),i1,i2,i3) = coeff(icf((-1),(0),(0),eqn,wc),i1,i2,i3) + 
     & (drar/xyz(i1-1,i2,i3,2))*u(i1+(-1)+off(1),i2+(0)+off(2),i3+(0)+
     & off(3),wc)
c      linearize terms like drar/xyz(i1+1,i2,i3,2) * u(wc)* u(wc) at the grid point offset by +1, 0, 0
                                               coeff(icf((+1),(0),(0),
     & eqn,wc),i1,i2,i3) = coeff(icf((+1),(0),(0),eqn,wc),i1,i2,i3) + 
     & (drar/xyz(i1+1,i2,i3,2))*u(i1+(+1)+off(1),i2+(0)+off(2),i3+(0)+
     & off(3),wc)
                                               coeff(icf((+1),(0),(0),
     & eqn,wc),i1,i2,i3) = coeff(icf((+1),(0),(0),eqn,wc),i1,i2,i3) + 
     & (drar/xyz(i1+1,i2,i3,2))*u(i1+(+1)+off(1),i2+(0)+off(2),i3+(0)+
     & off(3),wc)
c      linearize terms like drar/xyz(i1,i2-1,i3,2) * u(wc)* u(wc) at the grid point offset by 0, -1, 0
                                               coeff(icf((0),(-1),(0),
     & eqn,wc),i1,i2,i3) = coeff(icf((0),(-1),(0),eqn,wc),i1,i2,i3) + 
     & (drar/xyz(i1,i2-1,i3,2))*u(i1+(0)+off(1),i2+(-1)+off(2),i3+(0)+
     & off(3),wc)
                                               coeff(icf((0),(-1),(0),
     & eqn,wc),i1,i2,i3) = coeff(icf((0),(-1),(0),eqn,wc),i1,i2,i3) + 
     & (drar/xyz(i1,i2-1,i3,2))*u(i1+(0)+off(1),i2+(-1)+off(2),i3+(0)+
     & off(3),wc)
c      linearize terms like drar/xyz(i1,i2+1,i3,2) * u(wc)* u(wc) at the grid point offset by 0, +1, 0
                                               coeff(icf((0),(+1),(0),
     & eqn,wc),i1,i2,i3) = coeff(icf((0),(+1),(0),eqn,wc),i1,i2,i3) + 
     & (drar/xyz(i1,i2+1,i3,2))*u(i1+(0)+off(1),i2+(+1)+off(2),i3+(0)+
     & off(3),wc)
                                               coeff(icf((0),(+1),(0),
     & eqn,wc),i1,i2,i3) = coeff(icf((0),(+1),(0),eqn,wc),i1,i2,i3) + 
     & (drar/xyz(i1,i2+1,i3,2))*u(i1+(0)+off(1),i2+(+1)+off(2),i3+(0)+
     & off(3),wc)
                                       else
c      linearize terms like ((-1d0)/(xyz(i1,i2,i3,2))) * u(wc)* u(wc) at the grid point offset by 0, 0, 0
                                                coeff(icf((0),(0),(0),
     & eqn,wc),i1,i2,i3) = coeff(icf((0),(0),(0),eqn,wc),i1,i2,i3) + (
     & ((-1d0)/(xyz(i1,i2,i3,2))))*u(i1+(0)+off(1),i2+(0)+off(2),i3+(
     & 0)+off(3),wc)
                                                coeff(icf((0),(0),(0),
     & eqn,wc),i1,i2,i3) = coeff(icf((0),(0),(0),eqn,wc),i1,i2,i3) + (
     & ((-1d0)/(xyz(i1,i2,i3,2))))*u(i1+(0)+off(1),i2+(0)+off(2),i3+(
     & 0)+off(3),wc)
                                       endif
                                 eqn = wc
                                       if ( .true. ) then
                                       drar = (1d0)*dri2(1)*jaci*jac(
     & i1+off(1)-1,i2+off(2),i3+off(3))*(xyz(i1+off(1),i2+off(2),i3+
     & off(3),2)-xyz(i1+off(1)-1,i2+off(2),i3+off(3),2))*rx(i1+off(1)-
     & 1,i2+off(2),i3+off(3),1,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
c      linearize terms like drar * u(vc)* u(wc) at the grid point offset by -1, 0, 0
                                             coeff(icf((-1),(0),(0),
     & eqn,vc),i1,i2,i3) = coeff(icf((-1),(0),(0),eqn,vc),i1,i2,i3) + 
     & (drar)*u(i1+(-1)+off(1),i2+(0)+off(2),i3+(0)+off(3),wc)
                                             coeff(icf((-1),(0),(0),
     & eqn,wc),i1,i2,i3) = coeff(icf((-1),(0),(0),eqn,wc),i1,i2,i3) + 
     & (drar)*u(i1+(-1)+off(1),i2+(0)+off(2),i3+(0)+off(3),vc)
                                       drar = (1d0)*dri2(1)*jaci*jac(
     & i1+off(1)+1,i2+off(2),i3+off(3))*(xyz(i1+off(1)+1,i2+off(2),i3+
     & off(3),2)-xyz(i1+off(1),i2+off(2),i3+off(3),2))*rx(i1+off(1)+1,
     & i2+off(2),i3+off(3),1,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
c      linearize terms like drar * u(vc)* u(wc) at the grid point offset by +1, 0, 0
                                             coeff(icf((+1),(0),(0),
     & eqn,vc),i1,i2,i3) = coeff(icf((+1),(0),(0),eqn,vc),i1,i2,i3) + 
     & (drar)*u(i1+(+1)+off(1),i2+(0)+off(2),i3+(0)+off(3),wc)
                                             coeff(icf((+1),(0),(0),
     & eqn,wc),i1,i2,i3) = coeff(icf((+1),(0),(0),eqn,wc),i1,i2,i3) + 
     & (drar)*u(i1+(+1)+off(1),i2+(0)+off(2),i3+(0)+off(3),vc)
                                       drar = (1d0)*dri2(2)*jaci*jac(
     & i1+off(1),i2+off(2)-1,i3+off(3))*(xyz(i1+off(1),i2+off(2),i3+
     & off(3),2)-xyz(i1+off(1),i2+off(2)-1,i3+off(3),2))*rx(i1+off(1),
     & i2+off(2)-1,i3+off(3),2,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
c      linearize terms like drar * u(vc)* u(wc) at the grid point offset by 0, -1, 0
                                             coeff(icf((0),(-1),(0),
     & eqn,vc),i1,i2,i3) = coeff(icf((0),(-1),(0),eqn,vc),i1,i2,i3) + 
     & (drar)*u(i1+(0)+off(1),i2+(-1)+off(2),i3+(0)+off(3),wc)
                                             coeff(icf((0),(-1),(0),
     & eqn,wc),i1,i2,i3) = coeff(icf((0),(-1),(0),eqn,wc),i1,i2,i3) + 
     & (drar)*u(i1+(0)+off(1),i2+(-1)+off(2),i3+(0)+off(3),vc)
                                       drar = (1d0)*dri2(2)*jaci*jac(
     & i1+off(1),i2+off(2)+1,i3+off(3))*(xyz(i1+off(1),i2+off(2)+1,i3+
     & off(3),2)-xyz(i1+off(1),i2+off(2),i3+off(3),2))*rx(i1+off(1),
     & i2+off(2)+1,i3+off(3),2,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
c      linearize terms like drar * u(vc)* u(wc) at the grid point offset by 0, +1, 0
                                             coeff(icf((0),(+1),(0),
     & eqn,vc),i1,i2,i3) = coeff(icf((0),(+1),(0),eqn,vc),i1,i2,i3) + 
     & (drar)*u(i1+(0)+off(1),i2+(+1)+off(2),i3+(0)+off(3),wc)
                                             coeff(icf((0),(+1),(0),
     & eqn,wc),i1,i2,i3) = coeff(icf((0),(+1),(0),eqn,wc),i1,i2,i3) + 
     & (drar)*u(i1+(0)+off(1),i2+(+1)+off(2),i3+(0)+off(3),vc)
                                       else if ( .false. ) then
                                          drar = (1d0)*.25d0
c      linearize terms like drar/xyz(i1-1,i2,i3,2) * u(vc)* u(wc) at the grid point offset by -1, 0, 0
                                               coeff(icf((-1),(0),(0),
     & eqn,vc),i1,i2,i3) = coeff(icf((-1),(0),(0),eqn,vc),i1,i2,i3) + 
     & (drar/xyz(i1-1,i2,i3,2))*u(i1+(-1)+off(1),i2+(0)+off(2),i3+(0)+
     & off(3),wc)
                                               coeff(icf((-1),(0),(0),
     & eqn,wc),i1,i2,i3) = coeff(icf((-1),(0),(0),eqn,wc),i1,i2,i3) + 
     & (drar/xyz(i1-1,i2,i3,2))*u(i1+(-1)+off(1),i2+(0)+off(2),i3+(0)+
     & off(3),vc)
c      linearize terms like drar/xyz(i1+1,i2,i3,2) * u(vc)* u(wc) at the grid point offset by +1, 0, 0
                                               coeff(icf((+1),(0),(0),
     & eqn,vc),i1,i2,i3) = coeff(icf((+1),(0),(0),eqn,vc),i1,i2,i3) + 
     & (drar/xyz(i1+1,i2,i3,2))*u(i1+(+1)+off(1),i2+(0)+off(2),i3+(0)+
     & off(3),wc)
                                               coeff(icf((+1),(0),(0),
     & eqn,wc),i1,i2,i3) = coeff(icf((+1),(0),(0),eqn,wc),i1,i2,i3) + 
     & (drar/xyz(i1+1,i2,i3,2))*u(i1+(+1)+off(1),i2+(0)+off(2),i3+(0)+
     & off(3),vc)
c      linearize terms like drar/xyz(i1,i2-1,i3,2) * u(vc)* u(wc) at the grid point offset by 0, -1, 0
                                               coeff(icf((0),(-1),(0),
     & eqn,vc),i1,i2,i3) = coeff(icf((0),(-1),(0),eqn,vc),i1,i2,i3) + 
     & (drar/xyz(i1,i2-1,i3,2))*u(i1+(0)+off(1),i2+(-1)+off(2),i3+(0)+
     & off(3),wc)
                                               coeff(icf((0),(-1),(0),
     & eqn,wc),i1,i2,i3) = coeff(icf((0),(-1),(0),eqn,wc),i1,i2,i3) + 
     & (drar/xyz(i1,i2-1,i3,2))*u(i1+(0)+off(1),i2+(-1)+off(2),i3+(0)+
     & off(3),vc)
c      linearize terms like drar/xyz(i1,i2+1,i3,2) * u(vc)* u(wc) at the grid point offset by 0, +1, 0
                                               coeff(icf((0),(+1),(0),
     & eqn,vc),i1,i2,i3) = coeff(icf((0),(+1),(0),eqn,vc),i1,i2,i3) + 
     & (drar/xyz(i1,i2+1,i3,2))*u(i1+(0)+off(1),i2+(+1)+off(2),i3+(0)+
     & off(3),wc)
                                               coeff(icf((0),(+1),(0),
     & eqn,wc),i1,i2,i3) = coeff(icf((0),(+1),(0),eqn,wc),i1,i2,i3) + 
     & (drar/xyz(i1,i2+1,i3,2))*u(i1+(0)+off(1),i2+(+1)+off(2),i3+(0)+
     & off(3),vc)
                                       else
c      linearize terms like ((1d0)/(xyz(i1,i2,i3,2))) * u(vc)* u(wc) at the grid point offset by 0, 0, 0
                                                coeff(icf((0),(0),(0),
     & eqn,vc),i1,i2,i3) = coeff(icf((0),(0),(0),eqn,vc),i1,i2,i3) + (
     & ((1d0)/(xyz(i1,i2,i3,2))))*u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)
     & +off(3),wc)
                                                coeff(icf((0),(0),(0),
     & eqn,wc),i1,i2,i3) = coeff(icf((0),(0),(0),eqn,wc),i1,i2,i3) + (
     & ((1d0)/(xyz(i1,i2,i3,2))))*u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)
     & +off(3),vc)
                                       endif
c         enddo
                           else if ( .false. ) then
                              eqn = vc
c     -u[wc]^2/r 
c     will contribute to RHS
c      linearize terms like -1d0/xyz(i1,i2,i3,2) * u(wc)* u(wc) at the grid point offset by 0, 0, 0
                                    coeff(icf((0),(0),(0),eqn,wc),i1,
     & i2,i3) = coeff(icf((0),(0),(0),eqn,wc),i1,i2,i3) + (-1d0/xyz(
     & i1,i2,i3,2))*u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)+off(3),wc)
                                    coeff(icf((0),(0),(0),eqn,wc),i1,
     & i2,i3) = coeff(icf((0),(0),(0),eqn,wc),i1,i2,i3) + (-1d0/xyz(
     & i1,i2,i3,2))*u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)+off(3),wc)
                              eqn = wc
c     +u[vc] u[wc]/r
c     will contribute to RHS
c      linearize terms like 1d0/xyz(i1,i2,i3,2) * u(wc)* u(vc) at the grid point offset by 0, 0, 0
                                    coeff(icf((0),(0),(0),eqn,wc),i1,
     & i2,i3) = coeff(icf((0),(0),(0),eqn,wc),i1,i2,i3) + (1d0/xyz(i1,
     & i2,i3,2))*u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)+off(3),vc)
                                    coeff(icf((0),(0),(0),eqn,vc),i1,
     & i2,i3) = coeff(icf((0),(0),(0),eqn,vc),i1,i2,i3) + (1d0/xyz(i1,
     & i2,i3,2))*u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)+off(3),wc)
                           endif
                           if ( usestrik.and. .not. oldstrik .and. 
     & .false.) then
                              do d=2,2
                                 eqn = vc
                                 drar = -dri(1)*(xyz(i1-1,i2,i3,2)-xyz(
     & i1,i2,i3,2))*rx(i1,i2,i3,1,d)/xyz(i1,i2,i3,2)
c      linearize terms like alpha*drar * u(wc)* u(wc) at the grid point offset by -1, 0, 0
                                       coeff(icf((-1),(0),(0),eqn,wc),
     & i1,i2,i3) = coeff(icf((-1),(0),(0),eqn,wc),i1,i2,i3) + (alpha*
     & drar)*u(i1+(-1)+off(1),i2+(0)+off(2),i3+(0)+off(3),wc)
                                       coeff(icf((-1),(0),(0),eqn,wc),
     & i1,i2,i3) = coeff(icf((-1),(0),(0),eqn,wc),i1,i2,i3) + (alpha*
     & drar)*u(i1+(-1)+off(1),i2+(0)+off(2),i3+(0)+off(3),wc)
                                 drar = -dri(1)*(xyz(i1+1,i2,i3,2)-xyz(
     & i1,i2,i3,2))*rx(i1,i2,i3,1,d)/xyz(i1,i2,i3,2)
c      linearize terms like 3d0*alpha*drar * u(wc)* u(wc) at the grid point offset by +1, 0, 0
                                       coeff(icf((+1),(0),(0),eqn,wc),
     & i1,i2,i3) = coeff(icf((+1),(0),(0),eqn,wc),i1,i2,i3) + (3d0*
     & alpha*drar)*u(i1+(+1)+off(1),i2+(0)+off(2),i3+(0)+off(3),wc)
                                       coeff(icf((+1),(0),(0),eqn,wc),
     & i1,i2,i3) = coeff(icf((+1),(0),(0),eqn,wc),i1,i2,i3) + (3d0*
     & alpha*drar)*u(i1+(+1)+off(1),i2+(0)+off(2),i3+(0)+off(3),wc)
                                 drar = -dri(1)*(xyz(i1+2,i2,i3,2)-xyz(
     & i1,i2,i3,2))*rx(i1,i2,i3,1,d)/xyz(i1,i2,i3,2)
c      linearize terms like alpha*drar * u(wc)* u(wc) at the grid point offset by +2, 0, 0
                                       coeff(icf((+2),(0),(0),eqn,wc),
     & i1,i2,i3) = coeff(icf((+2),(0),(0),eqn,wc),i1,i2,i3) + (alpha*
     & drar)*u(i1+(+2)+off(1),i2+(0)+off(2),i3+(0)+off(3),wc)
                                       coeff(icf((+2),(0),(0),eqn,wc),
     & i1,i2,i3) = coeff(icf((+2),(0),(0),eqn,wc),i1,i2,i3) + (alpha*
     & drar)*u(i1+(+2)+off(1),i2+(0)+off(2),i3+(0)+off(3),wc)
                                 drar = -dri(2)*(xyz(i1,i2-1,i3,2)-xyz(
     & i1,i2,i3,2))*rx(i1,i2,i3,2,d)/xyz(i1,i2,i3,2)
c      linearize terms like alpha*drar * u(wc)* u(wc) at the grid point offset by 0, -1, 0
                                       coeff(icf((0),(-1),(0),eqn,wc),
     & i1,i2,i3) = coeff(icf((0),(-1),(0),eqn,wc),i1,i2,i3) + (alpha*
     & drar)*u(i1+(0)+off(1),i2+(-1)+off(2),i3+(0)+off(3),wc)
                                       coeff(icf((0),(-1),(0),eqn,wc),
     & i1,i2,i3) = coeff(icf((0),(-1),(0),eqn,wc),i1,i2,i3) + (alpha*
     & drar)*u(i1+(0)+off(1),i2+(-1)+off(2),i3+(0)+off(3),wc)
                                 drar = -dri(2)*(xyz(i1,i2+1,i3,2)-xyz(
     & i1,i2,i3,2))*rx(i1,i2,i3,2,d)/xyz(i1,i2,i3,2)
c      linearize terms like 3d0*alpha*drar * u(wc)* u(wc) at the grid point offset by 0, +1, 0
                                       coeff(icf((0),(+1),(0),eqn,wc),
     & i1,i2,i3) = coeff(icf((0),(+1),(0),eqn,wc),i1,i2,i3) + (3d0*
     & alpha*drar)*u(i1+(0)+off(1),i2+(+1)+off(2),i3+(0)+off(3),wc)
                                       coeff(icf((0),(+1),(0),eqn,wc),
     & i1,i2,i3) = coeff(icf((0),(+1),(0),eqn,wc),i1,i2,i3) + (3d0*
     & alpha*drar)*u(i1+(0)+off(1),i2+(+1)+off(2),i3+(0)+off(3),wc)
                                 drar = -dri(2)*(xyz(i1,i2+2,i3,2)-xyz(
     & i1,i2,i3,2))*rx(i1,i2,i3,2,d)/xyz(i1,i2,i3,2)
c      linearize terms like alpha*drar * u(wc)* u(wc) at the grid point offset by 0, +2, 0
                                       coeff(icf((0),(+2),(0),eqn,wc),
     & i1,i2,i3) = coeff(icf((0),(+2),(0),eqn,wc),i1,i2,i3) + (alpha*
     & drar)*u(i1+(0)+off(1),i2+(+2)+off(2),i3+(0)+off(3),wc)
                                       coeff(icf((0),(+2),(0),eqn,wc),
     & i1,i2,i3) = coeff(icf((0),(+2),(0),eqn,wc),i1,i2,i3) + (alpha*
     & drar)*u(i1+(0)+off(1),i2+(+2)+off(2),i3+(0)+off(3),wc)
                                 eqn = wc
                                 drar = dri(1)*(xyz(i1-1,i2,i3,2)-xyz(
     & i1,i2,i3,2))*rx(i1,i2,i3,1,d)/xyz(i1,i2,i3,2)
c      linearize terms like alpha*drar * u(wc)* u(vc) at the grid point offset by -1, 0, 0
                                       coeff(icf((-1),(0),(0),eqn,wc),
     & i1,i2,i3) = coeff(icf((-1),(0),(0),eqn,wc),i1,i2,i3) + (alpha*
     & drar)*u(i1+(-1)+off(1),i2+(0)+off(2),i3+(0)+off(3),vc)
                                       coeff(icf((-1),(0),(0),eqn,vc),
     & i1,i2,i3) = coeff(icf((-1),(0),(0),eqn,vc),i1,i2,i3) + (alpha*
     & drar)*u(i1+(-1)+off(1),i2+(0)+off(2),i3+(0)+off(3),wc)
                                 drar = dri(1)*(xyz(i1+1,i2,i3,2)-xyz(
     & i1,i2,i3,2))*rx(i1,i2,i3,1,d)/xyz(i1,i2,i3,2)
c      linearize terms like 3d0*alpha*drar * u(wc)* u(vc) at the grid point offset by +1, 0, 0
                                       coeff(icf((+1),(0),(0),eqn,wc),
     & i1,i2,i3) = coeff(icf((+1),(0),(0),eqn,wc),i1,i2,i3) + (3d0*
     & alpha*drar)*u(i1+(+1)+off(1),i2+(0)+off(2),i3+(0)+off(3),vc)
                                       coeff(icf((+1),(0),(0),eqn,vc),
     & i1,i2,i3) = coeff(icf((+1),(0),(0),eqn,vc),i1,i2,i3) + (3d0*
     & alpha*drar)*u(i1+(+1)+off(1),i2+(0)+off(2),i3+(0)+off(3),wc)
                                 drar = dri(1)*(xyz(i1+2,i2,i3,2)-xyz(
     & i1,i2,i3,2))*rx(i1,i2,i3,1,d)/xyz(i1,i2,i3,2)
c      linearize terms like alpha*drar * u(wc)* u(vc) at the grid point offset by +2, 0, 0
                                       coeff(icf((+2),(0),(0),eqn,wc),
     & i1,i2,i3) = coeff(icf((+2),(0),(0),eqn,wc),i1,i2,i3) + (alpha*
     & drar)*u(i1+(+2)+off(1),i2+(0)+off(2),i3+(0)+off(3),vc)
                                       coeff(icf((+2),(0),(0),eqn,vc),
     & i1,i2,i3) = coeff(icf((+2),(0),(0),eqn,vc),i1,i2,i3) + (alpha*
     & drar)*u(i1+(+2)+off(1),i2+(0)+off(2),i3+(0)+off(3),wc)
                                 drar = dri(2)*(xyz(i1,i2-1,i3,2)-xyz(
     & i1,i2,i3,2))*rx(i1,i2,i3,2,d)/xyz(i1,i2,i3,2)
c      linearize terms like alpha*drar * u(wc)* u(vc) at the grid point offset by 0, -1, 0
                                       coeff(icf((0),(-1),(0),eqn,wc),
     & i1,i2,i3) = coeff(icf((0),(-1),(0),eqn,wc),i1,i2,i3) + (alpha*
     & drar)*u(i1+(0)+off(1),i2+(-1)+off(2),i3+(0)+off(3),vc)
                                       coeff(icf((0),(-1),(0),eqn,vc),
     & i1,i2,i3) = coeff(icf((0),(-1),(0),eqn,vc),i1,i2,i3) + (alpha*
     & drar)*u(i1+(0)+off(1),i2+(-1)+off(2),i3+(0)+off(3),wc)
                                 drar = dri(2)*(xyz(i1,i2+1,i3,2)-xyz(
     & i1,i2,i3,2))*rx(i1,i2,i3,2,d)/xyz(i1,i2,i3,2)
c      linearize terms like 3d0*alpha*drar * u(wc)* u(vc) at the grid point offset by 0, +1, 0
                                       coeff(icf((0),(+1),(0),eqn,wc),
     & i1,i2,i3) = coeff(icf((0),(+1),(0),eqn,wc),i1,i2,i3) + (3d0*
     & alpha*drar)*u(i1+(0)+off(1),i2+(+1)+off(2),i3+(0)+off(3),vc)
                                       coeff(icf((0),(+1),(0),eqn,vc),
     & i1,i2,i3) = coeff(icf((0),(+1),(0),eqn,vc),i1,i2,i3) + (3d0*
     & alpha*drar)*u(i1+(0)+off(1),i2+(+1)+off(2),i3+(0)+off(3),wc)
                                 drar = dri(2)*(xyz(i1,i2+2,i3,2)-xyz(
     & i1,i2,i3,2))*rx(i1,i2,i3,2,d)/xyz(i1,i2,i3,2)
c      linearize terms like alpha*drar * u(wc)* u(vc) at the grid point offset by 0, +2, 0
                                       coeff(icf((0),(+2),(0),eqn,wc),
     & i1,i2,i3) = coeff(icf((0),(+2),(0),eqn,wc),i1,i2,i3) + (alpha*
     & drar)*u(i1+(0)+off(1),i2+(+2)+off(2),i3+(0)+off(3),vc)
                                       coeff(icf((0),(+2),(0),eqn,vc),
     & i1,i2,i3) = coeff(icf((0),(+2),(0),eqn,vc),i1,i2,i3) + (alpha*
     & drar)*u(i1+(0)+off(1),i2+(+2)+off(2),i3+(0)+off(3),wc)
                              enddo
                           endif
                           endif

                  else

                  eqn = vc
c                 -u[wc]^2/r 
c                 will contribute to RHS

                  coeff(icf(0,0,0,eqn,wc),i1,i2,i3) =
     &                 coeff(icf(0,0,0,eqn,wc),i1,i2,i3) -
     &                 2d0 * u(i1,i2,i3,wc)/xyz(i1,i2,i3,2)

                  eqn = wc
c                 +u[vc] u[wc]/r
c                 will contribute to RHS
                  coeff(icf(0,0,0,eqn,wc),i1,i2,i3) =
     &                 coeff(icf(0,0,0,eqn,wc),i1,i2,i3) +
     &                 u(i1,i2,i3,vc)/xyz(i1,i2,i3,2)

                  coeff(icf(0,0,0,eqn,vc),i1,i2,i3) =
     &                 coeff(icf(0,0,0,eqn,vc),i1,i2,i3) +
     &                 u(i1,i2,i3,wc)/xyz(i1,i2,i3,2)

                  endif
               endif

            endif ! non-viscous axi forcing terms
c
c     MOMENTUM EQUATION VISCOUS TERMS
c

c           u-momentum equation, the terms are
c           (4 u[uc]_xx/3 + u[uc]_yy + u[vc]_xy/3 + axifac*(u[uc]_y/y + u[vc]_x/y/3))/u[rc]/ren
c
            eqn = uc
c           UXYC is a bpp macro that fills in the 9 point stencil for the second derivatives (see top of file)

c           u[uc]_xx/uc[rc] linearization (will have RHS)
c     9 point curvilinear grid second derivative stencil:  (-f43*oren/u(i1,i2,i3,rc)) * u[uc]_{XY}
                        coeff(icf(-1,-1,0,eqn,uc),i1,i2,i3) =  coeff(
     & icf(-1,-1,0,eqn,uc),i1,i2,i3) + ( (-f43*oren/u(i1,i2,i3,rc)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,1) +  rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,1))*dri2(1)*dri2(2)
                        coeff(icf( 0,-1,0,eqn,uc),i1,i2,i3) =  coeff(
     & icf( 0,-1,0,eqn,uc),i1,i2,i3) + ( (-f43*oren/u(i1,i2,i3,rc)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),2,1)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,1)*dri(2)*dri(2) - rxx(i1,i2,i3,2,1,1)*dri2(2))
                        coeff(icf(+1,-1,0,eqn,uc),i1,i2,i3) =  coeff(
     & icf(+1,-1,0,eqn,uc),i1,i2,i3) + (-(-f43*oren/u(i1,i2,i3,rc)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,1) +  rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,1))*dri2(1)*dri2(2)
                        coeff(icf(-1, 0,0,eqn,uc),i1,i2,i3) =  coeff(
     & icf(-1, 0,0,eqn,uc),i1,i2,i3) + ( (-f43*oren/u(i1,i2,i3,rc)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2),
     & i3+off(3),1,1)*dri(1)*dri(1) - rxx(i1,i2,i3,1,1,1)*dri2(1))
                        coeff(icf( 0, 0,0,eqn,uc),i1,i2,i3) =  coeff(
     & icf( 0, 0,0,eqn,uc),i1,i2,i3) + (-2d0*(-f43*oren/u(i1,i2,i3,rc)
     & ))*(rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(
     & 2),i3+off(3),1,1)*dri(1)*dri(1) + rx(i1+off(1),i2+off(2),i3+
     & off(3),2,1)*rx(i1+off(1),i2+off(2),i3+off(3),2,1)*dri(2)*dri(2)
     & )
                        coeff(icf(+1, 0,0,eqn,uc),i1,i2,i3) =  coeff(
     & icf(+1, 0,0,eqn,uc),i1,i2,i3) + ( (-f43*oren/u(i1,i2,i3,rc)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2),
     & i3+off(3),1,1)*dri(1)*dri(1) + rxx(i1,i2,i3,1,1,1)*dri2(1))
                        coeff(icf(-1,+1,0,eqn,uc),i1,i2,i3) =  coeff(
     & icf(-1,+1,0,eqn,uc),i1,i2,i3) + (-(-f43*oren/u(i1,i2,i3,rc)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,1) + rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,1))*dri2(1)*dri2(2)
                        coeff(icf( 0,+1,0,eqn,uc),i1,i2,i3) =  coeff(
     & icf( 0,+1,0,eqn,uc),i1,i2,i3) + ( (-f43*oren/u(i1,i2,i3,rc)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),2,1)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,1)*dri(2)*dri(2) + rxx(i1,i2,i3,2,1,1)*dri2(2))
                        coeff(icf(+1,+1,0,eqn,uc),i1,i2,i3) =  coeff(
     & icf(+1,+1,0,eqn,uc),i1,i2,i3) + ( (-f43*oren/u(i1,i2,i3,rc)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,1) + rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,1))*dri2(1)*dri2(2)
            coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &           coeff(icf(0,0,0,eqn,rc),i1,i2,i3) -
     &           (-f43*oren*uxx(i1,i2,i3,uc,1,1))/
     &              (u(i1,i2,i3,rc)**2)

c           u[uc]_yy/uc[rc] linearization (will have RHS)
c     9 point curvilinear grid second derivative stencil:  (-oren/u(i1,i2,i3,rc)) * u[uc]_{XY}
                        coeff(icf(-1,-1,0,eqn,uc),i1,i2,i3) =  coeff(
     & icf(-1,-1,0,eqn,uc),i1,i2,i3) + ( (-oren/u(i1,i2,i3,rc)))*(rx(
     & i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2),i3+
     & off(3),2,2) +  rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(
     & 1),i2+off(2),i3+off(3),2,2))*dri2(1)*dri2(2)
                        coeff(icf( 0,-1,0,eqn,uc),i1,i2,i3) =  coeff(
     & icf( 0,-1,0,eqn,uc),i1,i2,i3) + ( (-oren/u(i1,i2,i3,rc)))*(rx(
     & i1+off(1),i2+off(2),i3+off(3),2,2)*rx(i1+off(1),i2+off(2),i3+
     & off(3),2,2)*dri(2)*dri(2) - rxx(i1,i2,i3,2,2,2)*dri2(2))
                        coeff(icf(+1,-1,0,eqn,uc),i1,i2,i3) =  coeff(
     & icf(+1,-1,0,eqn,uc),i1,i2,i3) + (-(-oren/u(i1,i2,i3,rc)))*(rx(
     & i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2),i3+
     & off(3),2,2) +  rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(
     & 1),i2+off(2),i3+off(3),2,2))*dri2(1)*dri2(2)
                        coeff(icf(-1, 0,0,eqn,uc),i1,i2,i3) =  coeff(
     & icf(-1, 0,0,eqn,uc),i1,i2,i3) + ( (-oren/u(i1,i2,i3,rc)))*(rx(
     & i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2),i3+
     & off(3),1,2)*dri(1)*dri(1) - rxx(i1,i2,i3,1,2,2)*dri2(1))
                        coeff(icf( 0, 0,0,eqn,uc),i1,i2,i3) =  coeff(
     & icf( 0, 0,0,eqn,uc),i1,i2,i3) + (-2d0*(-oren/u(i1,i2,i3,rc)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2),
     & i3+off(3),1,2)*dri(1)*dri(1) + rx(i1+off(1),i2+off(2),i3+off(3)
     & ,2,2)*rx(i1+off(1),i2+off(2),i3+off(3),2,2)*dri(2)*dri(2))
                        coeff(icf(+1, 0,0,eqn,uc),i1,i2,i3) =  coeff(
     & icf(+1, 0,0,eqn,uc),i1,i2,i3) + ( (-oren/u(i1,i2,i3,rc)))*(rx(
     & i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2),i3+
     & off(3),1,2)*dri(1)*dri(1) + rxx(i1,i2,i3,1,2,2)*dri2(1))
                        coeff(icf(-1,+1,0,eqn,uc),i1,i2,i3) =  coeff(
     & icf(-1,+1,0,eqn,uc),i1,i2,i3) + (-(-oren/u(i1,i2,i3,rc)))*(rx(
     & i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2),i3+
     & off(3),2,2) + rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(
     & 1),i2+off(2),i3+off(3),2,2))*dri2(1)*dri2(2)
                        coeff(icf( 0,+1,0,eqn,uc),i1,i2,i3) =  coeff(
     & icf( 0,+1,0,eqn,uc),i1,i2,i3) + ( (-oren/u(i1,i2,i3,rc)))*(rx(
     & i1+off(1),i2+off(2),i3+off(3),2,2)*rx(i1+off(1),i2+off(2),i3+
     & off(3),2,2)*dri(2)*dri(2) + rxx(i1,i2,i3,2,2,2)*dri2(2))
                        coeff(icf(+1,+1,0,eqn,uc),i1,i2,i3) =  coeff(
     & icf(+1,+1,0,eqn,uc),i1,i2,i3) + ( (-oren/u(i1,i2,i3,rc)))*(rx(
     & i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2),i3+
     & off(3),2,2) + rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(
     & 1),i2+off(2),i3+off(3),2,2))*dri2(1)*dri2(2)
            coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &           coeff(icf(0,0,0,eqn,rc),i1,i2,i3) -
     &           (-oren*uxx(i1,i2,i3,uc,2,2))/
     &              (u(i1,i2,i3,rc)**2)


c           u[vc]_xy/uc[rc] linearization (will have RHS)
c     9 point curvilinear grid second derivative stencil:  (-f13*oren/u(i1,i2,i3,rc)) * u[vc]_{XY}
                        coeff(icf(-1,-1,0,eqn,vc),i1,i2,i3) =  coeff(
     & icf(-1,-1,0,eqn,vc),i1,i2,i3) + ( (-f13*oren/u(i1,i2,i3,rc)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,2) +  rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,1))*dri2(1)*dri2(2)
                        coeff(icf( 0,-1,0,eqn,vc),i1,i2,i3) =  coeff(
     & icf( 0,-1,0,eqn,vc),i1,i2,i3) + ( (-f13*oren/u(i1,i2,i3,rc)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),2,1)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,2)*dri(2)*dri(2) - rxx(i1,i2,i3,2,1,2)*dri2(2))
                        coeff(icf(+1,-1,0,eqn,vc),i1,i2,i3) =  coeff(
     & icf(+1,-1,0,eqn,vc),i1,i2,i3) + (-(-f13*oren/u(i1,i2,i3,rc)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,2) +  rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,1))*dri2(1)*dri2(2)
                        coeff(icf(-1, 0,0,eqn,vc),i1,i2,i3) =  coeff(
     & icf(-1, 0,0,eqn,vc),i1,i2,i3) + ( (-f13*oren/u(i1,i2,i3,rc)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2),
     & i3+off(3),1,2)*dri(1)*dri(1) - rxx(i1,i2,i3,1,1,2)*dri2(1))
                        coeff(icf( 0, 0,0,eqn,vc),i1,i2,i3) =  coeff(
     & icf( 0, 0,0,eqn,vc),i1,i2,i3) + (-2d0*(-f13*oren/u(i1,i2,i3,rc)
     & ))*(rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(
     & 2),i3+off(3),1,2)*dri(1)*dri(1) + rx(i1+off(1),i2+off(2),i3+
     & off(3),2,1)*rx(i1+off(1),i2+off(2),i3+off(3),2,2)*dri(2)*dri(2)
     & )
                        coeff(icf(+1, 0,0,eqn,vc),i1,i2,i3) =  coeff(
     & icf(+1, 0,0,eqn,vc),i1,i2,i3) + ( (-f13*oren/u(i1,i2,i3,rc)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2),
     & i3+off(3),1,2)*dri(1)*dri(1) + rxx(i1,i2,i3,1,1,2)*dri2(1))
                        coeff(icf(-1,+1,0,eqn,vc),i1,i2,i3) =  coeff(
     & icf(-1,+1,0,eqn,vc),i1,i2,i3) + (-(-f13*oren/u(i1,i2,i3,rc)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,2) + rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,1))*dri2(1)*dri2(2)
                        coeff(icf( 0,+1,0,eqn,vc),i1,i2,i3) =  coeff(
     & icf( 0,+1,0,eqn,vc),i1,i2,i3) + ( (-f13*oren/u(i1,i2,i3,rc)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),2,1)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,2)*dri(2)*dri(2) + rxx(i1,i2,i3,2,1,2)*dri2(2))
                        coeff(icf(+1,+1,0,eqn,vc),i1,i2,i3) =  coeff(
     & icf(+1,+1,0,eqn,vc),i1,i2,i3) + ( (-f13*oren/u(i1,i2,i3,rc)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,2) + rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,1))*dri2(1)*dri2(2)
            coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &           coeff(icf(0,0,0,eqn,rc),i1,i2,i3) -
     &           (-f13*oren*uxx(i1,i2,i3,vc,1,2))/
     &              (u(i1,i2,i3,rc)**2)

            if ( isaxi.gt.0 .and. xyz(i1,i2,i3,2).gt.0d0 ) then

c           u[uc]_y/y/u[rc] linearization (will have RHS)
               coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &          coeff(icf(0,0,0,eqn,rc),i1,i2,i3) +
     & oren*ux(i1,i2,i3,uc,2)/xyz(i1,i2,i3,2)/(u(i1,i2,i3,rc)**2)

c     coefficients for a  difference approximation to (-oren/xyz(i1,i2,i3,2)/u(i1,i2,i3,rc)) * u[uc]_X
c     r-part
                     coeff(icf(+1,0,0,eqn,uc),i1,i2,i3) = coeff(icf(+1,
     & 0,0,eqn,uc),i1,i2,i3) + ((-oren/xyz(i1,i2,i3,2)/u(i1,i2,i3,rc))
     & )*rx(i1+off(1),i2+off(2),i3+off(3),1,2)*dri2(1)
                     coeff(icf(-1,0,0,eqn,uc),i1,i2,i3) = coeff(icf(-1,
     & 0,0,eqn,uc),i1,i2,i3) - ((-oren/xyz(i1,i2,i3,2)/u(i1,i2,i3,rc))
     & )*rx(i1+off(1),i2+off(2),i3+off(3),1,2)*dri2(1)
c     s-part
                     coeff(icf(0,+1,0,eqn,uc),i1,i2,i3) = coeff(icf(0,+
     & 1,0,eqn,uc),i1,i2,i3) + ((-oren/xyz(i1,i2,i3,2)/u(i1,i2,i3,rc))
     & )*rx(i1+off(1),i2+off(2),i3+off(3),2,2)*dri2(2)
                     coeff(icf(0,-1,0,eqn,uc),i1,i2,i3) = coeff(icf(0,-
     & 1,0,eqn,uc),i1,i2,i3) - ((-oren/xyz(i1,i2,i3,2)/u(i1,i2,i3,rc))
     & )*rx(i1+off(1),i2+off(2),i3+off(3),2,2)*dri2(2)

c          u[vc]_x/y/uc[rc] linearization (will have RHS)
               coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &          coeff(icf(0,0,0,eqn,rc),i1,i2,i3) +
     & f13*oren*ux(i1,i2,i3,vc,1)/xyz(i1,i2,i3,2)/(u(i1,i2,i3,rc)**2)

c     coefficients for a  difference approximation to (-f13*oren/xyz(i1,i2,i3,2)/u(i1,i2,i3,rc)) * u[vc]_X
c     r-part
                     coeff(icf(+1,0,0,eqn,vc),i1,i2,i3) = coeff(icf(+1,
     & 0,0,eqn,vc),i1,i2,i3) + ((-f13*oren/xyz(i1,i2,i3,2)/u(i1,i2,i3,
     & rc)))*rx(i1+off(1),i2+off(2),i3+off(3),1,1)*dri2(1)
                     coeff(icf(-1,0,0,eqn,vc),i1,i2,i3) = coeff(icf(-1,
     & 0,0,eqn,vc),i1,i2,i3) - ((-f13*oren/xyz(i1,i2,i3,2)/u(i1,i2,i3,
     & rc)))*rx(i1+off(1),i2+off(2),i3+off(3),1,1)*dri2(1)
c     s-part
                     coeff(icf(0,+1,0,eqn,vc),i1,i2,i3) = coeff(icf(0,+
     & 1,0,eqn,vc),i1,i2,i3) + ((-f13*oren/xyz(i1,i2,i3,2)/u(i1,i2,i3,
     & rc)))*rx(i1+off(1),i2+off(2),i3+off(3),2,1)*dri2(2)
                     coeff(icf(0,-1,0,eqn,vc),i1,i2,i3) = coeff(icf(0,-
     & 1,0,eqn,vc),i1,i2,i3) - ((-f13*oren/xyz(i1,i2,i3,2)/u(i1,i2,i3,
     & rc)))*rx(i1+off(1),i2+off(2),i3+off(3),2,1)*dri2(2)

            endif

c           v-momentum equation, the terms are
c           (4 u[vc]_yy/3 + u[vc]_xx + u[uc]_xy/3 + axifac*(4/3)*(u[vc]_y/y - u[vc]/y^2))/u[rc]/ren
c
            eqn = vc
c           u[vc]_yy/uc[rc] linearization (will have RHS)
c     9 point curvilinear grid second derivative stencil:  (-f43*oren/u(i1,i2,i3,rc)) * u[vc]_{XY}
                        coeff(icf(-1,-1,0,eqn,vc),i1,i2,i3) =  coeff(
     & icf(-1,-1,0,eqn,vc),i1,i2,i3) + ( (-f43*oren/u(i1,i2,i3,rc)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,2) +  rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,2))*dri2(1)*dri2(2)
                        coeff(icf( 0,-1,0,eqn,vc),i1,i2,i3) =  coeff(
     & icf( 0,-1,0,eqn,vc),i1,i2,i3) + ( (-f43*oren/u(i1,i2,i3,rc)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),2,2)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,2)*dri(2)*dri(2) - rxx(i1,i2,i3,2,2,2)*dri2(2))
                        coeff(icf(+1,-1,0,eqn,vc),i1,i2,i3) =  coeff(
     & icf(+1,-1,0,eqn,vc),i1,i2,i3) + (-(-f43*oren/u(i1,i2,i3,rc)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,2) +  rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,2))*dri2(1)*dri2(2)
                        coeff(icf(-1, 0,0,eqn,vc),i1,i2,i3) =  coeff(
     & icf(-1, 0,0,eqn,vc),i1,i2,i3) + ( (-f43*oren/u(i1,i2,i3,rc)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2),
     & i3+off(3),1,2)*dri(1)*dri(1) - rxx(i1,i2,i3,1,2,2)*dri2(1))
                        coeff(icf( 0, 0,0,eqn,vc),i1,i2,i3) =  coeff(
     & icf( 0, 0,0,eqn,vc),i1,i2,i3) + (-2d0*(-f43*oren/u(i1,i2,i3,rc)
     & ))*(rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(
     & 2),i3+off(3),1,2)*dri(1)*dri(1) + rx(i1+off(1),i2+off(2),i3+
     & off(3),2,2)*rx(i1+off(1),i2+off(2),i3+off(3),2,2)*dri(2)*dri(2)
     & )
                        coeff(icf(+1, 0,0,eqn,vc),i1,i2,i3) =  coeff(
     & icf(+1, 0,0,eqn,vc),i1,i2,i3) + ( (-f43*oren/u(i1,i2,i3,rc)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2),
     & i3+off(3),1,2)*dri(1)*dri(1) + rxx(i1,i2,i3,1,2,2)*dri2(1))
                        coeff(icf(-1,+1,0,eqn,vc),i1,i2,i3) =  coeff(
     & icf(-1,+1,0,eqn,vc),i1,i2,i3) + (-(-f43*oren/u(i1,i2,i3,rc)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,2) + rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,2))*dri2(1)*dri2(2)
                        coeff(icf( 0,+1,0,eqn,vc),i1,i2,i3) =  coeff(
     & icf( 0,+1,0,eqn,vc),i1,i2,i3) + ( (-f43*oren/u(i1,i2,i3,rc)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),2,2)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,2)*dri(2)*dri(2) + rxx(i1,i2,i3,2,2,2)*dri2(2))
                        coeff(icf(+1,+1,0,eqn,vc),i1,i2,i3) =  coeff(
     & icf(+1,+1,0,eqn,vc),i1,i2,i3) + ( (-f43*oren/u(i1,i2,i3,rc)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,2) + rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,2))*dri2(1)*dri2(2)
            coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &           coeff(icf(0,0,0,eqn,rc),i1,i2,i3) -
     &           (-f43*oren*uxx(i1,i2,i3,vc,2,2))/
     &              (u(i1,i2,i3,rc)**2)

c           u[vc]_xx/uc[rc] linearization (will have RHS)
c     9 point curvilinear grid second derivative stencil:  (-oren/u(i1,i2,i3,rc)) * u[vc]_{XY}
                        coeff(icf(-1,-1,0,eqn,vc),i1,i2,i3) =  coeff(
     & icf(-1,-1,0,eqn,vc),i1,i2,i3) + ( (-oren/u(i1,i2,i3,rc)))*(rx(
     & i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2),i3+
     & off(3),2,1) +  rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(
     & 1),i2+off(2),i3+off(3),2,1))*dri2(1)*dri2(2)
                        coeff(icf( 0,-1,0,eqn,vc),i1,i2,i3) =  coeff(
     & icf( 0,-1,0,eqn,vc),i1,i2,i3) + ( (-oren/u(i1,i2,i3,rc)))*(rx(
     & i1+off(1),i2+off(2),i3+off(3),2,1)*rx(i1+off(1),i2+off(2),i3+
     & off(3),2,1)*dri(2)*dri(2) - rxx(i1,i2,i3,2,1,1)*dri2(2))
                        coeff(icf(+1,-1,0,eqn,vc),i1,i2,i3) =  coeff(
     & icf(+1,-1,0,eqn,vc),i1,i2,i3) + (-(-oren/u(i1,i2,i3,rc)))*(rx(
     & i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2),i3+
     & off(3),2,1) +  rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(
     & 1),i2+off(2),i3+off(3),2,1))*dri2(1)*dri2(2)
                        coeff(icf(-1, 0,0,eqn,vc),i1,i2,i3) =  coeff(
     & icf(-1, 0,0,eqn,vc),i1,i2,i3) + ( (-oren/u(i1,i2,i3,rc)))*(rx(
     & i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2),i3+
     & off(3),1,1)*dri(1)*dri(1) - rxx(i1,i2,i3,1,1,1)*dri2(1))
                        coeff(icf( 0, 0,0,eqn,vc),i1,i2,i3) =  coeff(
     & icf( 0, 0,0,eqn,vc),i1,i2,i3) + (-2d0*(-oren/u(i1,i2,i3,rc)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2),
     & i3+off(3),1,1)*dri(1)*dri(1) + rx(i1+off(1),i2+off(2),i3+off(3)
     & ,2,1)*rx(i1+off(1),i2+off(2),i3+off(3),2,1)*dri(2)*dri(2))
                        coeff(icf(+1, 0,0,eqn,vc),i1,i2,i3) =  coeff(
     & icf(+1, 0,0,eqn,vc),i1,i2,i3) + ( (-oren/u(i1,i2,i3,rc)))*(rx(
     & i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2),i3+
     & off(3),1,1)*dri(1)*dri(1) + rxx(i1,i2,i3,1,1,1)*dri2(1))
                        coeff(icf(-1,+1,0,eqn,vc),i1,i2,i3) =  coeff(
     & icf(-1,+1,0,eqn,vc),i1,i2,i3) + (-(-oren/u(i1,i2,i3,rc)))*(rx(
     & i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2),i3+
     & off(3),2,1) + rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(
     & 1),i2+off(2),i3+off(3),2,1))*dri2(1)*dri2(2)
                        coeff(icf( 0,+1,0,eqn,vc),i1,i2,i3) =  coeff(
     & icf( 0,+1,0,eqn,vc),i1,i2,i3) + ( (-oren/u(i1,i2,i3,rc)))*(rx(
     & i1+off(1),i2+off(2),i3+off(3),2,1)*rx(i1+off(1),i2+off(2),i3+
     & off(3),2,1)*dri(2)*dri(2) + rxx(i1,i2,i3,2,1,1)*dri2(2))
                        coeff(icf(+1,+1,0,eqn,vc),i1,i2,i3) =  coeff(
     & icf(+1,+1,0,eqn,vc),i1,i2,i3) + ( (-oren/u(i1,i2,i3,rc)))*(rx(
     & i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2),i3+
     & off(3),2,1) + rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(
     & 1),i2+off(2),i3+off(3),2,1))*dri2(1)*dri2(2)
            coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &           coeff(icf(0,0,0,eqn,rc),i1,i2,i3) -
     &           (-oren*uxx(i1,i2,i3,vc,1,1))/
     &              (u(i1,i2,i3,rc)**2)


c           u[uc]_xy/uc[rc] linearization (will have RHS)
c     9 point curvilinear grid second derivative stencil:  (-f13*oren/u(i1,i2,i3,rc)) * u[uc]_{XY}
                        coeff(icf(-1,-1,0,eqn,uc),i1,i2,i3) =  coeff(
     & icf(-1,-1,0,eqn,uc),i1,i2,i3) + ( (-f13*oren/u(i1,i2,i3,rc)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,1) +  rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,2))*dri2(1)*dri2(2)
                        coeff(icf( 0,-1,0,eqn,uc),i1,i2,i3) =  coeff(
     & icf( 0,-1,0,eqn,uc),i1,i2,i3) + ( (-f13*oren/u(i1,i2,i3,rc)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),2,2)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,1)*dri(2)*dri(2) - rxx(i1,i2,i3,2,2,1)*dri2(2))
                        coeff(icf(+1,-1,0,eqn,uc),i1,i2,i3) =  coeff(
     & icf(+1,-1,0,eqn,uc),i1,i2,i3) + (-(-f13*oren/u(i1,i2,i3,rc)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,1) +  rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,2))*dri2(1)*dri2(2)
                        coeff(icf(-1, 0,0,eqn,uc),i1,i2,i3) =  coeff(
     & icf(-1, 0,0,eqn,uc),i1,i2,i3) + ( (-f13*oren/u(i1,i2,i3,rc)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2),
     & i3+off(3),1,1)*dri(1)*dri(1) - rxx(i1,i2,i3,1,2,1)*dri2(1))
                        coeff(icf( 0, 0,0,eqn,uc),i1,i2,i3) =  coeff(
     & icf( 0, 0,0,eqn,uc),i1,i2,i3) + (-2d0*(-f13*oren/u(i1,i2,i3,rc)
     & ))*(rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(
     & 2),i3+off(3),1,1)*dri(1)*dri(1) + rx(i1+off(1),i2+off(2),i3+
     & off(3),2,2)*rx(i1+off(1),i2+off(2),i3+off(3),2,1)*dri(2)*dri(2)
     & )
                        coeff(icf(+1, 0,0,eqn,uc),i1,i2,i3) =  coeff(
     & icf(+1, 0,0,eqn,uc),i1,i2,i3) + ( (-f13*oren/u(i1,i2,i3,rc)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2),
     & i3+off(3),1,1)*dri(1)*dri(1) + rxx(i1,i2,i3,1,2,1)*dri2(1))
                        coeff(icf(-1,+1,0,eqn,uc),i1,i2,i3) =  coeff(
     & icf(-1,+1,0,eqn,uc),i1,i2,i3) + (-(-f13*oren/u(i1,i2,i3,rc)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,1) + rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,2))*dri2(1)*dri2(2)
                        coeff(icf( 0,+1,0,eqn,uc),i1,i2,i3) =  coeff(
     & icf( 0,+1,0,eqn,uc),i1,i2,i3) + ( (-f13*oren/u(i1,i2,i3,rc)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),2,2)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,1)*dri(2)*dri(2) + rxx(i1,i2,i3,2,2,1)*dri2(2))
                        coeff(icf(+1,+1,0,eqn,uc),i1,i2,i3) =  coeff(
     & icf(+1,+1,0,eqn,uc),i1,i2,i3) + ( (-f13*oren/u(i1,i2,i3,rc)))*(
     & rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,1) + rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,2))*dri2(1)*dri2(2)
            coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &           coeff(icf(0,0,0,eqn,rc),i1,i2,i3) -
     &           (-f13*oren*uxx(i1,i2,i3,uc,2,1))/
     &              (u(i1,i2,i3,rc)**2)

            if ( isaxi.gt.0 .and. xyz(i1,i2,i3,2).gt.0d0 ) then

c           u[vc]_y/y/u[rc] linearization (will have RHS)
               coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &          coeff(icf(0,0,0,eqn,rc),i1,i2,i3) +
     &          f43*oren*ux(i1,i2,i3,vc,2)/xyz(i1,i2,i3,2)/
     &              (u(i1,i2,i3,rc)**2)

c     coefficients for a  difference approximation to (-f43*oren/u(i1,i2,i3,rc)/xyz(i1,i2,i3,2)) * u[vc]_X
c     r-part
                     coeff(icf(+1,0,0,eqn,vc),i1,i2,i3) = coeff(icf(+1,
     & 0,0,eqn,vc),i1,i2,i3) + ((-f43*oren/u(i1,i2,i3,rc)/xyz(i1,i2,
     & i3,2)))*rx(i1+off(1),i2+off(2),i3+off(3),1,2)*dri2(1)
                     coeff(icf(-1,0,0,eqn,vc),i1,i2,i3) = coeff(icf(-1,
     & 0,0,eqn,vc),i1,i2,i3) - ((-f43*oren/u(i1,i2,i3,rc)/xyz(i1,i2,
     & i3,2)))*rx(i1+off(1),i2+off(2),i3+off(3),1,2)*dri2(1)
c     s-part
                     coeff(icf(0,+1,0,eqn,vc),i1,i2,i3) = coeff(icf(0,+
     & 1,0,eqn,vc),i1,i2,i3) + ((-f43*oren/u(i1,i2,i3,rc)/xyz(i1,i2,
     & i3,2)))*rx(i1+off(1),i2+off(2),i3+off(3),2,2)*dri2(2)
                     coeff(icf(0,-1,0,eqn,vc),i1,i2,i3) = coeff(icf(0,-
     & 1,0,eqn,vc),i1,i2,i3) - ((-f43*oren/u(i1,i2,i3,rc)/xyz(i1,i2,
     & i3,2)))*rx(i1+off(1),i2+off(2),i3+off(3),2,2)*dri2(2)

c          -u[vc]/y^2/u[rc] linearization (will have RHS)
               coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &              coeff(icf(0,0,0,eqn,rc),i1,i2,i3) -
     &              f43*oren*u(i1,i2,i3,vc)/(xyz(i1,i2,i3,2)**2)/
     &                       (u(i1,i2,i3,rc)**2)
               coeff(icf(0,0,0,eqn,vc),i1,i2,i3) =
     &              coeff(icf(0,0,0,eqn,vc),i1,i2,i3) +
     &              f43*oren/(xyz(i1,i2,i3,2)**2)/u(i1,i2,i3,rc)
            endif

            if ( isswirl.gt.0 ) then
c            w-momentum equation, the terms are
c            (u[wc]_xx + u[wc]_yy + axifac*(u[wc]_y/y - u[wc]/y^2))/u[rc]/ren
c
               eqn = wc
c           u[wc]_xx/uc[rc] linearization (will have RHS)
c     9 point curvilinear grid second derivative stencil:  (-oren/u(i1,i2,i3,rc)) * u[wc]_{XY}
                           coeff(icf(-1,-1,0,eqn,wc),i1,i2,i3) =  
     & coeff(icf(-1,-1,0,eqn,wc),i1,i2,i3) + ( (-oren/u(i1,i2,i3,rc)))
     & *(rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,1) +  rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,1))*dri2(1)*dri2(2)
                           coeff(icf( 0,-1,0,eqn,wc),i1,i2,i3) =  
     & coeff(icf( 0,-1,0,eqn,wc),i1,i2,i3) + ( (-oren/u(i1,i2,i3,rc)))
     & *(rx(i1+off(1),i2+off(2),i3+off(3),2,1)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,1)*dri(2)*dri(2) - rxx(i1,i2,i3,2,1,1)*dri2(2))
                           coeff(icf(+1,-1,0,eqn,wc),i1,i2,i3) =  
     & coeff(icf(+1,-1,0,eqn,wc),i1,i2,i3) + (-(-oren/u(i1,i2,i3,rc)))
     & *(rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,1) +  rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,1))*dri2(1)*dri2(2)
                           coeff(icf(-1, 0,0,eqn,wc),i1,i2,i3) =  
     & coeff(icf(-1, 0,0,eqn,wc),i1,i2,i3) + ( (-oren/u(i1,i2,i3,rc)))
     & *(rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2),
     & i3+off(3),1,1)*dri(1)*dri(1) - rxx(i1,i2,i3,1,1,1)*dri2(1))
                           coeff(icf( 0, 0,0,eqn,wc),i1,i2,i3) =  
     & coeff(icf( 0, 0,0,eqn,wc),i1,i2,i3) + (-2d0*(-oren/u(i1,i2,i3,
     & rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+
     & off(2),i3+off(3),1,1)*dri(1)*dri(1) + rx(i1+off(1),i2+off(2),
     & i3+off(3),2,1)*rx(i1+off(1),i2+off(2),i3+off(3),2,1)*dri(2)*
     & dri(2))
                           coeff(icf(+1, 0,0,eqn,wc),i1,i2,i3) =  
     & coeff(icf(+1, 0,0,eqn,wc),i1,i2,i3) + ( (-oren/u(i1,i2,i3,rc)))
     & *(rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2),
     & i3+off(3),1,1)*dri(1)*dri(1) + rxx(i1,i2,i3,1,1,1)*dri2(1))
                           coeff(icf(-1,+1,0,eqn,wc),i1,i2,i3) =  
     & coeff(icf(-1,+1,0,eqn,wc),i1,i2,i3) + (-(-oren/u(i1,i2,i3,rc)))
     & *(rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,1) + rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,1))*dri2(1)*dri2(2)
                           coeff(icf( 0,+1,0,eqn,wc),i1,i2,i3) =  
     & coeff(icf( 0,+1,0,eqn,wc),i1,i2,i3) + ( (-oren/u(i1,i2,i3,rc)))
     & *(rx(i1+off(1),i2+off(2),i3+off(3),2,1)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,1)*dri(2)*dri(2) + rxx(i1,i2,i3,2,1,1)*dri2(2))
                           coeff(icf(+1,+1,0,eqn,wc),i1,i2,i3) =  
     & coeff(icf(+1,+1,0,eqn,wc),i1,i2,i3) + ( (-oren/u(i1,i2,i3,rc)))
     & *(rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,1) + rx(i1+off(1),i2+off(2),i3+off(3),1,1)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,1))*dri2(1)*dri2(2)
               coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &              coeff(icf(0,0,0,eqn,rc),i1,i2,i3) -
     &              (-oren*uxx(i1,i2,i3,wc,1,1))/
     &              (u(i1,i2,i3,rc)**2)

c           u[wc]_yy/uc[rc] linearization (will have RHS)
c     9 point curvilinear grid second derivative stencil:  (-oren/u(i1,i2,i3,rc)) * u[wc]_{XY}
                           coeff(icf(-1,-1,0,eqn,wc),i1,i2,i3) =  
     & coeff(icf(-1,-1,0,eqn,wc),i1,i2,i3) + ( (-oren/u(i1,i2,i3,rc)))
     & *(rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,2) +  rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,2))*dri2(1)*dri2(2)
                           coeff(icf( 0,-1,0,eqn,wc),i1,i2,i3) =  
     & coeff(icf( 0,-1,0,eqn,wc),i1,i2,i3) + ( (-oren/u(i1,i2,i3,rc)))
     & *(rx(i1+off(1),i2+off(2),i3+off(3),2,2)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,2)*dri(2)*dri(2) - rxx(i1,i2,i3,2,2,2)*dri2(2))
                           coeff(icf(+1,-1,0,eqn,wc),i1,i2,i3) =  
     & coeff(icf(+1,-1,0,eqn,wc),i1,i2,i3) + (-(-oren/u(i1,i2,i3,rc)))
     & *(rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,2) +  rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,2))*dri2(1)*dri2(2)
                           coeff(icf(-1, 0,0,eqn,wc),i1,i2,i3) =  
     & coeff(icf(-1, 0,0,eqn,wc),i1,i2,i3) + ( (-oren/u(i1,i2,i3,rc)))
     & *(rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2),
     & i3+off(3),1,2)*dri(1)*dri(1) - rxx(i1,i2,i3,1,2,2)*dri2(1))
                           coeff(icf( 0, 0,0,eqn,wc),i1,i2,i3) =  
     & coeff(icf( 0, 0,0,eqn,wc),i1,i2,i3) + (-2d0*(-oren/u(i1,i2,i3,
     & rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+
     & off(2),i3+off(3),1,2)*dri(1)*dri(1) + rx(i1+off(1),i2+off(2),
     & i3+off(3),2,2)*rx(i1+off(1),i2+off(2),i3+off(3),2,2)*dri(2)*
     & dri(2))
                           coeff(icf(+1, 0,0,eqn,wc),i1,i2,i3) =  
     & coeff(icf(+1, 0,0,eqn,wc),i1,i2,i3) + ( (-oren/u(i1,i2,i3,rc)))
     & *(rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2),
     & i3+off(3),1,2)*dri(1)*dri(1) + rxx(i1,i2,i3,1,2,2)*dri2(1))
                           coeff(icf(-1,+1,0,eqn,wc),i1,i2,i3) =  
     & coeff(icf(-1,+1,0,eqn,wc),i1,i2,i3) + (-(-oren/u(i1,i2,i3,rc)))
     & *(rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,2) + rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,2))*dri2(1)*dri2(2)
                           coeff(icf( 0,+1,0,eqn,wc),i1,i2,i3) =  
     & coeff(icf( 0,+1,0,eqn,wc),i1,i2,i3) + ( (-oren/u(i1,i2,i3,rc)))
     & *(rx(i1+off(1),i2+off(2),i3+off(3),2,2)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,2)*dri(2)*dri(2) + rxx(i1,i2,i3,2,2,2)*dri2(2))
                           coeff(icf(+1,+1,0,eqn,wc),i1,i2,i3) =  
     & coeff(icf(+1,+1,0,eqn,wc),i1,i2,i3) + ( (-oren/u(i1,i2,i3,rc)))
     & *(rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+off(1),i2+off(2),
     & i3+off(3),2,2) + rx(i1+off(1),i2+off(2),i3+off(3),1,2)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,2))*dri2(1)*dri2(2)
               coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &              coeff(icf(0,0,0,eqn,rc),i1,i2,i3) -
     &              (-oren*uxx(i1,i2,i3,wc,2,2))/
     &              (u(i1,i2,i3,rc)**2)

c           u[wc]_y/y/u[rc] linearization (will have RHS)
                  coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &                 coeff(icf(0,0,0,eqn,rc),i1,i2,i3) +
     &                 oren*ux(i1,i2,i3,wc,2)/xyz(i1,i2,i3,2)/
     &                 (u(i1,i2,i3,rc)**2)

c     coefficients for a  difference approximation to (-oren/xyz(i1,i2,i3,2)/u(i1,i2,i3,rc)) * u[wc]_X
c     r-part
                        coeff(icf(+1,0,0,eqn,wc),i1,i2,i3) = coeff(icf(
     & +1,0,0,eqn,wc),i1,i2,i3) + ((-oren/xyz(i1,i2,i3,2)/u(i1,i2,i3,
     & rc)))*rx(i1+off(1),i2+off(2),i3+off(3),1,2)*dri2(1)
                        coeff(icf(-1,0,0,eqn,wc),i1,i2,i3) = coeff(icf(
     & -1,0,0,eqn,wc),i1,i2,i3) - ((-oren/xyz(i1,i2,i3,2)/u(i1,i2,i3,
     & rc)))*rx(i1+off(1),i2+off(2),i3+off(3),1,2)*dri2(1)
c     s-part
                        coeff(icf(0,+1,0,eqn,wc),i1,i2,i3) = coeff(icf(
     & 0,+1,0,eqn,wc),i1,i2,i3) + ((-oren/xyz(i1,i2,i3,2)/u(i1,i2,i3,
     & rc)))*rx(i1+off(1),i2+off(2),i3+off(3),2,2)*dri2(2)
                        coeff(icf(0,-1,0,eqn,wc),i1,i2,i3) = coeff(icf(
     & 0,-1,0,eqn,wc),i1,i2,i3) - ((-oren/xyz(i1,i2,i3,2)/u(i1,i2,i3,
     & rc)))*rx(i1+off(1),i2+off(2),i3+off(3),2,2)*dri2(2)

c          -u[wc]/y^2/u[rc] linearization (will have RHS)
                  coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &                 coeff(icf(0,0,0,eqn,rc),i1,i2,i3) -
     &                 oren*u(i1,i2,i3,wc)/(xyz(i1,i2,i3,2)**2)/
     &                 (u(i1,i2,i3,rc)*u(i1,i2,i3,rc))
                  coeff(icf(0,0,0,eqn,wc),i1,i2,i3) =
     &                 coeff(icf(0,0,0,eqn,wc),i1,i2,i3) +
     &                 oren/(xyz(i1,i2,i3,2)**2)/u(i1,i2,i3,rc)

            endif ! has swirl component

c
c     ENERGY/TEMPERATURE EQUATION
c
            eqn = tc
c
c     ENERGY EQUATION, PRESSURE WORK 
c
c     gm1 * u[tc] ( u[uc]_x + u[vc]_y + u[vc]/y )
c     each term will contribute to the RHS
c     we can do the Cartesian part generically:
            do cc=uc, uc+ndim-1
               d = cc-uc+1
               if ( .true. ) then
c              += gm1 u[tc]^{n+1} u[cc]_d^n 
               coeff(icf(0,0,0,eqn,tc),i1,i2,i3) =
     &         coeff(icf(0,0,0,eqn,tc),i1,i2,i3) +
     &         gm1*ux(i1,i2,i3,cc,d )

c             += gm1 u[tc]^n u[cc]_d^{n+1}
c     coefficients for a  difference approximation to (gm1*u(i1,i2,i3,tc)) * u[cc]_X
c     r-part
                     coeff(icf(+1,0,0,eqn,cc),i1,i2,i3) = coeff(icf(+1,
     & 0,0,eqn,cc),i1,i2,i3) + ((gm1*u(i1,i2,i3,tc)))*rx(i1+off(1),i2+
     & off(2),i3+off(3),1,d)*dri2(1)
                     coeff(icf(-1,0,0,eqn,cc),i1,i2,i3) = coeff(icf(-1,
     & 0,0,eqn,cc),i1,i2,i3) - ((gm1*u(i1,i2,i3,tc)))*rx(i1+off(1),i2+
     & off(2),i3+off(3),1,d)*dri2(1)
c     s-part
                     coeff(icf(0,+1,0,eqn,cc),i1,i2,i3) = coeff(icf(0,+
     & 1,0,eqn,cc),i1,i2,i3) + ((gm1*u(i1,i2,i3,tc)))*rx(i1+off(1),i2+
     & off(2),i3+off(3),2,d)*dri2(2)
                     coeff(icf(0,-1,0,eqn,cc),i1,i2,i3) = coeff(icf(0,-
     & 1,0,eqn,cc),i1,i2,i3) - ((gm1*u(i1,i2,i3,tc)))*rx(i1+off(1),i2+
     & off(2),i3+off(3),2,d)*dri2(2)
               else
c     coefficients for a 3rd order  difference approximation to gm1 * u[tc] u[cc]_X
c                 u[tc]^{n+1} * u[cc]^n_X
                        coeff(icf(0,0,0,eqn,tc),i1,i2,i3) = coeff(icf(
     & 0,0,0,eqn,tc),i1,i2,i3) + (gm1) * ux4p(i1,i2,i3,cc,d)
c                  (r_X u[cc]_r^{n+1} + s_X u[cc]_s^{n+1} )
c     coefficients for a 3rd order  difference approximation to (gm1)*u(i1+off(1),i2+off(2),i3+off(3),tc) * u[cc]_X
c                  (r_X u[cc]_r^{n+1} + s_X u[cc]_s^{n+1} )
c                 r - part
                              coeff(icf(+1,0,0,eqn,cc),i1,i2,i3) =  
     & coeff(icf(+1,0,0,eqn,cc),i1,i2,i3) - ((gm1)*u(i1+off(1),i2+off(
     & 2),i3+off(3),tc)) * (alpha-.5d0)    *rx(i1+off(1),i2+off(2),i3+
     & off(3),1,d)*dri(1)
                              coeff(icf( 0,0,0,eqn,cc),i1,i2,i3) =  
     & coeff(icf( 0,0,0,eqn,cc),i1,i2,i3) + ((gm1)*u(i1+off(1),i2+off(
     & 2),i3+off(3),tc)) * (3d0*alpha)     *rx(i1+off(1),i2+off(2),i3+
     & off(3),1,d)*dri(1)
                              coeff(icf(-1,0,0,eqn,cc),i1,i2,i3) =  
     & coeff(icf(-1,0,0,eqn,cc),i1,i2,i3) - ((gm1)*u(i1+off(1),i2+off(
     & 2),i3+off(3),tc)) * (.5d0+3d0*alpha)*rx(i1+off(1),i2+off(2),i3+
     & off(3),1,d)*dri(1)
                              coeff(icf(-2,0,0,eqn,cc),i1,i2,i3) =  
     & coeff(icf(-2,0,0,eqn,cc),i1,i2,i3) + ((gm1)*u(i1+off(1),i2+off(
     & 2),i3+off(3),tc)) * (alpha)         *rx(i1+off(1),i2+off(2),i3+
     & off(3),1,d)*dri(1)
c                 s - part
                              coeff(icf(0,+1,0,eqn,cc),i1,i2,i3) =  
     & coeff(icf(0,+1,0,eqn,cc),i1,i2,i3) - ((gm1)*u(i1+off(1),i2+off(
     & 2),i3+off(3),tc)) * (alpha-.5d0)    *rx(i1+off(1),i2+off(2),i3+
     & off(3),2,d)*dri(2)
                              coeff(icf( 0,0,0,eqn,cc),i1,i2,i3) =  
     & coeff(icf(0,0,0,eqn,cc),i1,i2,i3)  + ((gm1)*u(i1+off(1),i2+off(
     & 2),i3+off(3),tc)) * (3d0*alpha)     *rx(i1+off(1),i2+off(2),i3+
     & off(3),2,d)*dri(2)
                              coeff(icf(0,-1,0,eqn,cc),i1,i2,i3) =  
     & coeff(icf(0,-1,0,eqn,cc),i1,i2,i3) - ((gm1)*u(i1+off(1),i2+off(
     & 2),i3+off(3),tc)) * (.5d0+3d0*alpha)*rx(i1+off(1),i2+off(2),i3+
     & off(3),2,d)*dri(2)
                              coeff(icf(0,-2,0,eqn,cc),i1,i2,i3) =  
     & coeff(icf(0,-2,0,eqn,cc),i1,i2,i3) + ((gm1)*u(i1+off(1),i2+off(
     & 2),i3+off(3),tc)) * (alpha)         *rx(i1+off(1),i2+off(2),i3+
     & off(3),2,d)*dri(2)
c                 r - part
c      coeff(icf(+1,0,0,eqn,cc),i1,i2,i3) =  coeff(icf(+1,0,0,eqn,cc),i1,i2,i3) - (gm1) * (alpha-.5d0)    *rx(i1+off(1),i2+off(2),i3+off(3),1,d)*u(i1+off(1),i2+off(2),i3+off(3),tc)*dri(1)
c      coeff(icf( 0,0,0,eqn,cc),i1,i2,i3) =  coeff(icf( 0,0,0,eqn,cc),i1,i2,i3) + (gm1) * (3d0*alpha)     *rx(i1+off(1),i2+off(2),i3+off(3),1,d)*u(i1+off(1),i2+off(2),i3+off(3),tc)*dri(1)
c      coeff(icf(-1,0,0,eqn,cc),i1,i2,i3) =  coeff(icf(-1,0,0,eqn,cc),i1,i2,i3) - (gm1) * (.5d0+3d0*alpha)*rx(i1+off(1),i2+off(2),i3+off(3),1,d)*u(i1+off(1),i2+off(2),i3+off(3),tc)*dri(1)
c      coeff(icf(-2,0,0,eqn,cc),i1,i2,i3) =  coeff(icf(-2,0,0,eqn,cc),i1,i2,i3) + (gm1) * (alpha)         *rx(i1+off(1),i2+off(2),i3+off(3),1,d)*u(i1+off(1),i2+off(2),i3+off(3),tc)*dri(1)
c                 s - part
c      coeff(icf(0,+1,0,eqn,cc),i1,i2,i3) =  coeff(icf(0,+1,0,eqn,cc),i1,i2,i3) - (gm1) * (alpha-.5d0)    *rx(i1+off(1),i2+off(2),i3+off(3),2,d)*u(i1+off(1),i2+off(2),i3+off(3),tc)*dri(2)
c      coeff(icf( 0,0,0,eqn,cc),i1,i2,i3) =  coeff(icf(0,0,0,eqn,cc),i1,i2,i3)  + (gm1) * (3d0*alpha)     *rx(i1+off(1),i2+off(2),i3+off(3),2,d)*u(i1+off(1),i2+off(2),i3+off(3),tc)*dri(2)
c      coeff(icf(0,-1,0,eqn,cc),i1,i2,i3) =  coeff(icf(0,-1,0,eqn,cc),i1,i2,i3) - (gm1) * (.5d0+3d0*alpha)*rx(i1+off(1),i2+off(2),i3+off(3),2,d)*u(i1+off(1),i2+off(2),i3+off(3),tc)*dri(2)
c      coeff(icf(0,-2,0,eqn,cc),i1,i2,i3) =  coeff(icf(0,-2,0,eqn,cc),i1,i2,i3) + (gm1) * (alpha)         *rx(i1+off(1),i2+off(2),i3+off(3),2,d)*u(i1+off(1),i2+off(2),i3+off(3),tc)*dri(2)
               endif

            end do

            if ( isaxi.gt.0 .and. xyz(i1,i2,i3,2).gt.0d0 ) then
               if ( .true. ) then
               d=2
                     drar = (gm1)*dri2(1)*jaci*jac(i1+off(1)-1,i2+off(
     & 2),i3+off(3))*(xyz(i1+off(1),i2+off(2),i3+off(3),2)-xyz(i1+off(
     & 1)-1,i2+off(2),i3+off(3),2))*rx(i1+off(1)-1,i2+off(2),i3+off(3)
     & ,1,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
c      linearize terms like drar * u(tc)* u(vc) at the grid point offset by O1, O2, O3
                           coeff(icf((0),(0),(0),eqn,tc),i1,i2,i3) = 
     & coeff(icf((0),(0),(0),eqn,tc),i1,i2,i3) + (drar)*u(i1+(-1)+off(
     & 1),i2+(0)+off(2),i3+(0)+off(3),vc)
                           coeff(icf((-1),(0),(0),eqn,vc),i1,i2,i3) = 
     & coeff(icf((-1),(0),(0),eqn,vc),i1,i2,i3) + (drar)*u(i1+(0)+off(
     & 1),i2+(0)+off(2),i3+(0)+off(3),tc)
                     drar = (gm1)*dri2(1)*jaci*jac(i1+off(1)+1,i2+off(
     & 2),i3+off(3))*(xyz(i1+off(1)+1,i2+off(2),i3+off(3),2)-xyz(i1+
     & off(1),i2+off(2),i3+off(3),2))*rx(i1+off(1)+1,i2+off(2),i3+off(
     & 3),1,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
c      linearize terms like drar * u(tc)* u(vc) at the grid point offset by O1, O2, O3
                           coeff(icf((0),(0),(0),eqn,tc),i1,i2,i3) = 
     & coeff(icf((0),(0),(0),eqn,tc),i1,i2,i3) + (drar)*u(i1+(+1)+off(
     & 1),i2+(0)+off(2),i3+(0)+off(3),vc)
                           coeff(icf((+1),(0),(0),eqn,vc),i1,i2,i3) = 
     & coeff(icf((+1),(0),(0),eqn,vc),i1,i2,i3) + (drar)*u(i1+(0)+off(
     & 1),i2+(0)+off(2),i3+(0)+off(3),tc)
                     drar = (gm1)*dri2(2)*jaci*jac(i1+off(1),i2+off(2)-
     & 1,i3+off(3))*(xyz(i1+off(1),i2+off(2),i3+off(3),2)-xyz(i1+off(
     & 1),i2+off(2)-1,i3+off(3),2))*rx(i1+off(1),i2+off(2)-1,i3+off(3)
     & ,2,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
c      linearize terms like drar * u(tc)* u(vc) at the grid point offset by O1, O2, O3
                           coeff(icf((0),(0),(0),eqn,tc),i1,i2,i3) = 
     & coeff(icf((0),(0),(0),eqn,tc),i1,i2,i3) + (drar)*u(i1+(0)+off(
     & 1),i2+(-1)+off(2),i3+(0)+off(3),vc)
                           coeff(icf((0),(-1),(0),eqn,vc),i1,i2,i3) = 
     & coeff(icf((0),(-1),(0),eqn,vc),i1,i2,i3) + (drar)*u(i1+(0)+off(
     & 1),i2+(0)+off(2),i3+(0)+off(3),tc)
                     drar = (gm1)*dri2(2)*jaci*jac(i1+off(1),i2+off(2)+
     & 1,i3+off(3))*(xyz(i1+off(1),i2+off(2)+1,i3+off(3),2)-xyz(i1+
     & off(1),i2+off(2),i3+off(3),2))*rx(i1+off(1),i2+off(2)+1,i3+off(
     & 3),2,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
c      linearize terms like drar * u(tc)* u(vc) at the grid point offset by O1, O2, O3
                           coeff(icf((0),(0),(0),eqn,tc),i1,i2,i3) = 
     & coeff(icf((0),(0),(0),eqn,tc),i1,i2,i3) + (drar)*u(i1+(0)+off(
     & 1),i2+(+1)+off(2),i3+(0)+off(3),vc)
                           coeff(icf((0),(+1),(0),eqn,vc),i1,i2,i3) = 
     & coeff(icf((0),(+1),(0),eqn,vc),i1,i2,i3) + (drar)*u(i1+(0)+off(
     & 1),i2+(0)+off(2),i3+(0)+off(3),tc)
               else
c              gm1 u[tc]^n u[vc]^{n+1}/y
               coeff(icf(0,0,0,eqn,vc),i1,i2,i3) =
     &         coeff(icf(0,0,0,eqn,vc),i1,i2,i3) +
     &              gm1*u(i1,i2,i3,tc)/xyz(i1,i2,i3,2)

c              gm1 u[tc]^{n+1} u[vc]^n/y
               coeff(icf(0,0,0,eqn,tc),i1,i2,i3) =
     &         coeff(icf(0,0,0,eqn,tc),i1,i2,i3) +
     &              gm1*u(i1,i2,i3,vc)/xyz(i1,i2,i3,2)
               endif

            endif

c
c     ENERGY EQUATION, THERMAL CONDUCTIVITY
c
c     -gam * ( u[tc]_{xx} + u[tc]_{yy} + u[tc]_y/y )/(u[rc] Re Pr)
c     do the Cartesian terms generically
c     each term will contribute to the RHS
            do d=1,ndim

c            += -gam*u[tc]_{dd}^{n+1}/u[rc]^n/Re/Pr
c     9 point curvilinear grid second derivative stencil:  (-gam*oprn*oren/u(i1,i2,i3,rc)) * u[tc]_{XY}
                           coeff(icf(-1,-1,0,eqn,tc),i1,i2,i3) =  
     & coeff(icf(-1,-1,0,eqn,tc),i1,i2,i3) + ( (-gam*oprn*oren/u(i1,
     & i2,i3,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,d)*rx(i1+off(1)
     & ,i2+off(2),i3+off(3),2,d) +  rx(i1+off(1),i2+off(2),i3+off(3),
     & 1,d)*rx(i1+off(1),i2+off(2),i3+off(3),2,d))*dri2(1)*dri2(2)
                           coeff(icf( 0,-1,0,eqn,tc),i1,i2,i3) =  
     & coeff(icf( 0,-1,0,eqn,tc),i1,i2,i3) + ( (-gam*oprn*oren/u(i1,
     & i2,i3,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),2,d)*rx(i1+off(1)
     & ,i2+off(2),i3+off(3),2,d)*dri(2)*dri(2) - rxx(i1,i2,i3,2,d,d)*
     & dri2(2))
                           coeff(icf(+1,-1,0,eqn,tc),i1,i2,i3) =  
     & coeff(icf(+1,-1,0,eqn,tc),i1,i2,i3) + (-(-gam*oprn*oren/u(i1,
     & i2,i3,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,d)*rx(i1+off(1)
     & ,i2+off(2),i3+off(3),2,d) +  rx(i1+off(1),i2+off(2),i3+off(3),
     & 1,d)*rx(i1+off(1),i2+off(2),i3+off(3),2,d))*dri2(1)*dri2(2)
                           coeff(icf(-1, 0,0,eqn,tc),i1,i2,i3) =  
     & coeff(icf(-1, 0,0,eqn,tc),i1,i2,i3) + ( (-gam*oprn*oren/u(i1,
     & i2,i3,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,d)*rx(i1+off(1)
     & ,i2+off(2),i3+off(3),1,d)*dri(1)*dri(1) - rxx(i1,i2,i3,1,d,d)*
     & dri2(1))
                           coeff(icf( 0, 0,0,eqn,tc),i1,i2,i3) =  
     & coeff(icf( 0, 0,0,eqn,tc),i1,i2,i3) + (-2d0*(-gam*oprn*oren/u(
     & i1,i2,i3,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,d)*rx(i1+
     & off(1),i2+off(2),i3+off(3),1,d)*dri(1)*dri(1) + rx(i1+off(1),
     & i2+off(2),i3+off(3),2,d)*rx(i1+off(1),i2+off(2),i3+off(3),2,d)*
     & dri(2)*dri(2))
                           coeff(icf(+1, 0,0,eqn,tc),i1,i2,i3) =  
     & coeff(icf(+1, 0,0,eqn,tc),i1,i2,i3) + ( (-gam*oprn*oren/u(i1,
     & i2,i3,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,d)*rx(i1+off(1)
     & ,i2+off(2),i3+off(3),1,d)*dri(1)*dri(1) + rxx(i1,i2,i3,1,d,d)*
     & dri2(1))
                           coeff(icf(-1,+1,0,eqn,tc),i1,i2,i3) =  
     & coeff(icf(-1,+1,0,eqn,tc),i1,i2,i3) + (-(-gam*oprn*oren/u(i1,
     & i2,i3,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,d)*rx(i1+off(1)
     & ,i2+off(2),i3+off(3),2,d) + rx(i1+off(1),i2+off(2),i3+off(3),1,
     & d)*rx(i1+off(1),i2+off(2),i3+off(3),2,d))*dri2(1)*dri2(2)
                           coeff(icf( 0,+1,0,eqn,tc),i1,i2,i3) =  
     & coeff(icf( 0,+1,0,eqn,tc),i1,i2,i3) + ( (-gam*oprn*oren/u(i1,
     & i2,i3,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),2,d)*rx(i1+off(1)
     & ,i2+off(2),i3+off(3),2,d)*dri(2)*dri(2) + rxx(i1,i2,i3,2,d,d)*
     & dri2(2))
                           coeff(icf(+1,+1,0,eqn,tc),i1,i2,i3) =  
     & coeff(icf(+1,+1,0,eqn,tc),i1,i2,i3) + ( (-gam*oprn*oren/u(i1,
     & i2,i3,rc)))*(rx(i1+off(1),i2+off(2),i3+off(3),1,d)*rx(i1+off(1)
     & ,i2+off(2),i3+off(3),2,d) + rx(i1+off(1),i2+off(2),i3+off(3),1,
     & d)*rx(i1+off(1),i2+off(2),i3+off(3),2,d))*dri2(1)*dri2(2)

c            += gam* u[rc]^{n+1} u[tc]_{dd}^n/u[rc]^n/u[rc]^n/Re/Pr
               coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &         coeff(icf(0,0,0,eqn,rc),i1,i2,i3) +
     &         oprn*oren*gam*uxx(i1,i2,i3,tc,d,d)/(u(i1,i2,i3,rc)**2)
            enddo

            if ( isaxi.gt.0 .and. xyz(i1,i2,i3,2).gt.0d0 ) then
c              gam u[tc]_y^{n+1}/u[rc]^n/y/Re/Pr

c     coefficients for a  difference approximation to (-oprn*oren*gam/(u(i1,i2,i3,rc)*xyz(i1,i2,i3,2))) * u[tc]_X
c     r-part
                   coeff(icf(+1,0,0,eqn,tc),i1,i2,i3) = coeff(icf(+1,0,
     & 0,eqn,tc),i1,i2,i3) + ((-oprn*oren*gam/(u(i1,i2,i3,rc)*xyz(i1,
     & i2,i3,2))))*rx(i1+off(1),i2+off(2),i3+off(3),1,2)*dri2(1)
                   coeff(icf(-1,0,0,eqn,tc),i1,i2,i3) = coeff(icf(-1,0,
     & 0,eqn,tc),i1,i2,i3) - ((-oprn*oren*gam/(u(i1,i2,i3,rc)*xyz(i1,
     & i2,i3,2))))*rx(i1+off(1),i2+off(2),i3+off(3),1,2)*dri2(1)
c     s-part
                   coeff(icf(0,+1,0,eqn,tc),i1,i2,i3) = coeff(icf(0,+1,
     & 0,eqn,tc),i1,i2,i3) + ((-oprn*oren*gam/(u(i1,i2,i3,rc)*xyz(i1,
     & i2,i3,2))))*rx(i1+off(1),i2+off(2),i3+off(3),2,2)*dri2(2)
                   coeff(icf(0,-1,0,eqn,tc),i1,i2,i3) = coeff(icf(0,-1,
     & 0,eqn,tc),i1,i2,i3) - ((-oprn*oren*gam/(u(i1,i2,i3,rc)*xyz(i1,
     & i2,i3,2))))*rx(i1+off(1),i2+off(2),i3+off(3),2,2)*dri2(2)

c              gam u[rc]^{n+1}u[tc]_y^n/u[rc]^n/u[rc]^n/y/Re/Pr
               coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &         coeff(icf(0,0,0,eqn,rc),i1,i2,i3) +
     & gam*oprn*oren*ux(i1,i2,i3,tc,2)/(u(i1,i2,i3,rc)**2)/
     &                  xyz(i1,i2,i3,2)

            end if

c
c     ENERGY EQUATION, VISCOUS DISSIPATION
c
c     Since the viscous dissipation terms consist of products of three
c      solution variables, no contribution to the RHS will be made.
c     There are a bunch of terms that look like u[uc]_x^2/u[rc] which will
c      be linearized and added to the matrix by the bpp macro UX2OR.
            fac = -gam*gm1*oren*man*man
c     linearize and add terms like (f43*fac) * u[uc]_X^2/u[rc] to the matrix
c     (f43*fac) * u[uc]_{1}^2/u[rc] --> (f43*fac) * ( 2 u[uc]_X^n u[uc]_X^{n+1} - uc[rc]^{n+1} u[uc]_X^n/uc[rc]^n)/uc[rc]^n
c     (f43*fac) * ( 2 u[uc]_X^n u[uc]_X^{n+1} )/u[rc]^n
c     r-part
                  coeff(icf(+1,0,0,eqn,uc),i1,i2,i3) = coeff(icf(+1,0,
     & 0,eqn,uc),i1,i2,i3) + ((f43*fac))*2d0*ux(i1,i2,i3,uc,1)*rx(i1+
     & off(1),i2+off(2),i3+off(3),1,1)*dri2(1)/u(i1+off(1),i2+off(2),
     & i3+off(3),rc)
                  coeff(icf(-1,0,0,eqn,uc),i1,i2,i3) = coeff(icf(-1,0,
     & 0,eqn,uc),i1,i2,i3) - ((f43*fac))*2d0*ux(i1,i2,i3,uc,1)*rx(i1+
     & off(1),i2+off(2),i3+off(3),1,1)*dri2(1)/u(i1+off(1),i2+off(2),
     & i3+off(3),rc)
c     s-part
                  coeff(icf(0,+1,0,eqn,uc),i1,i2,i3) = coeff(icf(0,+1,
     & 0,eqn,uc),i1,i2,i3) + ((f43*fac))*2d0*ux(i1,i2,i3,uc,1)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,1)*dri2(2)/u(i1+off(1),i2+off(2),
     & i3+off(3),rc)
                  coeff(icf(0,-1,0,eqn,uc),i1,i2,i3) = coeff(icf(0,-1,
     & 0,eqn,uc),i1,i2,i3) - ((f43*fac))*2d0*ux(i1,i2,i3,uc,1)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,1)*dri2(2)/u(i1+off(1),i2+off(2),
     & i3+off(3),rc)
c     - (f43*fac) uc[rc]^{n+1} (u[uc]_X^n)^2/uc[rc]^n/uc[rc]^n
                  coeff(icf(0,0,0,eqn,rc),i1,i2,i3) = coeff(icf(0,0,0,
     & eqn,rc),i1,i2,i3) - ((f43*fac)) * ux(i1,i2,i3,uc,1)**2/(u(i1+
     & off(1),i2+off(2),i3+off(3),rc)**2)
c     linearize and add terms like (f43*fac) * u[vc]_X^2/u[rc] to the matrix
c     (f43*fac) * u[vc]_{2}^2/u[rc] --> (f43*fac) * ( 2 u[vc]_X^n u[vc]_X^{n+1} - uc[rc]^{n+1} u[vc]_X^n/uc[rc]^n)/uc[rc]^n
c     (f43*fac) * ( 2 u[vc]_X^n u[vc]_X^{n+1} )/u[rc]^n
c     r-part
                  coeff(icf(+1,0,0,eqn,vc),i1,i2,i3) = coeff(icf(+1,0,
     & 0,eqn,vc),i1,i2,i3) + ((f43*fac))*2d0*ux(i1,i2,i3,vc,2)*rx(i1+
     & off(1),i2+off(2),i3+off(3),1,2)*dri2(1)/u(i1+off(1),i2+off(2),
     & i3+off(3),rc)
                  coeff(icf(-1,0,0,eqn,vc),i1,i2,i3) = coeff(icf(-1,0,
     & 0,eqn,vc),i1,i2,i3) - ((f43*fac))*2d0*ux(i1,i2,i3,vc,2)*rx(i1+
     & off(1),i2+off(2),i3+off(3),1,2)*dri2(1)/u(i1+off(1),i2+off(2),
     & i3+off(3),rc)
c     s-part
                  coeff(icf(0,+1,0,eqn,vc),i1,i2,i3) = coeff(icf(0,+1,
     & 0,eqn,vc),i1,i2,i3) + ((f43*fac))*2d0*ux(i1,i2,i3,vc,2)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,2)*dri2(2)/u(i1+off(1),i2+off(2),
     & i3+off(3),rc)
                  coeff(icf(0,-1,0,eqn,vc),i1,i2,i3) = coeff(icf(0,-1,
     & 0,eqn,vc),i1,i2,i3) - ((f43*fac))*2d0*ux(i1,i2,i3,vc,2)*rx(i1+
     & off(1),i2+off(2),i3+off(3),2,2)*dri2(2)/u(i1+off(1),i2+off(2),
     & i3+off(3),rc)
c     - (f43*fac) uc[rc]^{n+1} (u[vc]_X^n)^2/uc[rc]^n/uc[rc]^n
                  coeff(icf(0,0,0,eqn,rc),i1,i2,i3) = coeff(icf(0,0,0,
     & eqn,rc),i1,i2,i3) - ((f43*fac)) * ux(i1,i2,i3,vc,2)**2/(u(i1+
     & off(1),i2+off(2),i3+off(3),rc)**2)
c     linearize and add terms like fac * u[uc]_X^2/u[rc] to the matrix
c     fac * u[uc]_{2}^2/u[rc] --> fac * ( 2 u[uc]_X^n u[uc]_X^{n+1} - uc[rc]^{n+1} u[uc]_X^n/uc[rc]^n)/uc[rc]^n
c     fac * ( 2 u[uc]_X^n u[uc]_X^{n+1} )/u[rc]^n
c     r-part
                  coeff(icf(+1,0,0,eqn,uc),i1,i2,i3) = coeff(icf(+1,0,
     & 0,eqn,uc),i1,i2,i3) + (fac)*2d0*ux(i1,i2,i3,uc,2)*rx(i1+off(1),
     & i2+off(2),i3+off(3),1,2)*dri2(1)/u(i1+off(1),i2+off(2),i3+off(
     & 3),rc)
                  coeff(icf(-1,0,0,eqn,uc),i1,i2,i3) = coeff(icf(-1,0,
     & 0,eqn,uc),i1,i2,i3) - (fac)*2d0*ux(i1,i2,i3,uc,2)*rx(i1+off(1),
     & i2+off(2),i3+off(3),1,2)*dri2(1)/u(i1+off(1),i2+off(2),i3+off(
     & 3),rc)
c     s-part
                  coeff(icf(0,+1,0,eqn,uc),i1,i2,i3) = coeff(icf(0,+1,
     & 0,eqn,uc),i1,i2,i3) + (fac)*2d0*ux(i1,i2,i3,uc,2)*rx(i1+off(1),
     & i2+off(2),i3+off(3),2,2)*dri2(2)/u(i1+off(1),i2+off(2),i3+off(
     & 3),rc)
                  coeff(icf(0,-1,0,eqn,uc),i1,i2,i3) = coeff(icf(0,-1,
     & 0,eqn,uc),i1,i2,i3) - (fac)*2d0*ux(i1,i2,i3,uc,2)*rx(i1+off(1),
     & i2+off(2),i3+off(3),2,2)*dri2(2)/u(i1+off(1),i2+off(2),i3+off(
     & 3),rc)
c     - fac uc[rc]^{n+1} (u[uc]_X^n)^2/uc[rc]^n/uc[rc]^n
                  coeff(icf(0,0,0,eqn,rc),i1,i2,i3) = coeff(icf(0,0,0,
     & eqn,rc),i1,i2,i3) - (fac) * ux(i1,i2,i3,uc,2)**2/(u(i1+off(1),
     & i2+off(2),i3+off(3),rc)**2)
c     linearize and add terms like fac * u[vc]_X^2/u[rc] to the matrix
c     fac * u[vc]_{1}^2/u[rc] --> fac * ( 2 u[vc]_X^n u[vc]_X^{n+1} - uc[rc]^{n+1} u[vc]_X^n/uc[rc]^n)/uc[rc]^n
c     fac * ( 2 u[vc]_X^n u[vc]_X^{n+1} )/u[rc]^n
c     r-part
                  coeff(icf(+1,0,0,eqn,vc),i1,i2,i3) = coeff(icf(+1,0,
     & 0,eqn,vc),i1,i2,i3) + (fac)*2d0*ux(i1,i2,i3,vc,1)*rx(i1+off(1),
     & i2+off(2),i3+off(3),1,1)*dri2(1)/u(i1+off(1),i2+off(2),i3+off(
     & 3),rc)
                  coeff(icf(-1,0,0,eqn,vc),i1,i2,i3) = coeff(icf(-1,0,
     & 0,eqn,vc),i1,i2,i3) - (fac)*2d0*ux(i1,i2,i3,vc,1)*rx(i1+off(1),
     & i2+off(2),i3+off(3),1,1)*dri2(1)/u(i1+off(1),i2+off(2),i3+off(
     & 3),rc)
c     s-part
                  coeff(icf(0,+1,0,eqn,vc),i1,i2,i3) = coeff(icf(0,+1,
     & 0,eqn,vc),i1,i2,i3) + (fac)*2d0*ux(i1,i2,i3,vc,1)*rx(i1+off(1),
     & i2+off(2),i3+off(3),2,1)*dri2(2)/u(i1+off(1),i2+off(2),i3+off(
     & 3),rc)
                  coeff(icf(0,-1,0,eqn,vc),i1,i2,i3) = coeff(icf(0,-1,
     & 0,eqn,vc),i1,i2,i3) - (fac)*2d0*ux(i1,i2,i3,vc,1)*rx(i1+off(1),
     & i2+off(2),i3+off(3),2,1)*dri2(2)/u(i1+off(1),i2+off(2),i3+off(
     & 3),rc)
c     - fac uc[rc]^{n+1} (u[vc]_X^n)^2/uc[rc]^n/uc[rc]^n
                  coeff(icf(0,0,0,eqn,rc),i1,i2,i3) = coeff(icf(0,0,0,
     & eqn,rc),i1,i2,i3) - (fac) * ux(i1,i2,i3,vc,1)**2/(u(i1+off(1),
     & i2+off(2),i3+off(3),rc)**2)

c        -f43*fac*u[vc]_y u[uc]_x/u[rc]
c     coefficients for a  difference approximation to (-f43*fac*ux(i1,i2,i3,uc,1)/u(i1,i2,i3,rc)) * u[vc]_X
c     r-part
                  coeff(icf(+1,0,0,eqn,vc),i1,i2,i3) = coeff(icf(+1,0,
     & 0,eqn,vc),i1,i2,i3) + ((-f43*fac*ux(i1,i2,i3,uc,1)/u(i1,i2,i3,
     & rc)))*rx(i1+off(1),i2+off(2),i3+off(3),1,2)*dri2(1)
                  coeff(icf(-1,0,0,eqn,vc),i1,i2,i3) = coeff(icf(-1,0,
     & 0,eqn,vc),i1,i2,i3) - ((-f43*fac*ux(i1,i2,i3,uc,1)/u(i1,i2,i3,
     & rc)))*rx(i1+off(1),i2+off(2),i3+off(3),1,2)*dri2(1)
c     s-part
                  coeff(icf(0,+1,0,eqn,vc),i1,i2,i3) = coeff(icf(0,+1,
     & 0,eqn,vc),i1,i2,i3) + ((-f43*fac*ux(i1,i2,i3,uc,1)/u(i1,i2,i3,
     & rc)))*rx(i1+off(1),i2+off(2),i3+off(3),2,2)*dri2(2)
                  coeff(icf(0,-1,0,eqn,vc),i1,i2,i3) = coeff(icf(0,-1,
     & 0,eqn,vc),i1,i2,i3) - ((-f43*fac*ux(i1,i2,i3,uc,1)/u(i1,i2,i3,
     & rc)))*rx(i1+off(1),i2+off(2),i3+off(3),2,2)*dri2(2)
c     coefficients for a  difference approximation to (-f43*fac*ux(i1,i2,i3,vc,2)/u(i1,i2,i3,rc)) * u[uc]_X
c     r-part
                  coeff(icf(+1,0,0,eqn,uc),i1,i2,i3) = coeff(icf(+1,0,
     & 0,eqn,uc),i1,i2,i3) + ((-f43*fac*ux(i1,i2,i3,vc,2)/u(i1,i2,i3,
     & rc)))*rx(i1+off(1),i2+off(2),i3+off(3),1,1)*dri2(1)
                  coeff(icf(-1,0,0,eqn,uc),i1,i2,i3) = coeff(icf(-1,0,
     & 0,eqn,uc),i1,i2,i3) - ((-f43*fac*ux(i1,i2,i3,vc,2)/u(i1,i2,i3,
     & rc)))*rx(i1+off(1),i2+off(2),i3+off(3),1,1)*dri2(1)
c     s-part
                  coeff(icf(0,+1,0,eqn,uc),i1,i2,i3) = coeff(icf(0,+1,
     & 0,eqn,uc),i1,i2,i3) + ((-f43*fac*ux(i1,i2,i3,vc,2)/u(i1,i2,i3,
     & rc)))*rx(i1+off(1),i2+off(2),i3+off(3),2,1)*dri2(2)
                  coeff(icf(0,-1,0,eqn,uc),i1,i2,i3) = coeff(icf(0,-1,
     & 0,eqn,uc),i1,i2,i3) - ((-f43*fac*ux(i1,i2,i3,vc,2)/u(i1,i2,i3,
     & rc)))*rx(i1+off(1),i2+off(2),i3+off(3),2,1)*dri2(2)
            coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &      coeff(icf(0,0,0,eqn,rc),i1,i2,i3) -
     &        (-f43*fac*ux(i1,i2,i3,uc,1)*ux(i1,i2,i3,vc,2))/
     &                  (u(i1,i2,i3,rc)**2)

c        2*fac*u[vc]_x u[uc]_y/u[rc]
c     coefficients for a  difference approximation to (2d0*fac*ux(i1,i2,i3,uc,2)/u(i1,i2,i3,rc)) * u[vc]_X
c     r-part
                  coeff(icf(+1,0,0,eqn,vc),i1,i2,i3) = coeff(icf(+1,0,
     & 0,eqn,vc),i1,i2,i3) + ((2d0*fac*ux(i1,i2,i3,uc,2)/u(i1,i2,i3,
     & rc)))*rx(i1+off(1),i2+off(2),i3+off(3),1,1)*dri2(1)
                  coeff(icf(-1,0,0,eqn,vc),i1,i2,i3) = coeff(icf(-1,0,
     & 0,eqn,vc),i1,i2,i3) - ((2d0*fac*ux(i1,i2,i3,uc,2)/u(i1,i2,i3,
     & rc)))*rx(i1+off(1),i2+off(2),i3+off(3),1,1)*dri2(1)
c     s-part
                  coeff(icf(0,+1,0,eqn,vc),i1,i2,i3) = coeff(icf(0,+1,
     & 0,eqn,vc),i1,i2,i3) + ((2d0*fac*ux(i1,i2,i3,uc,2)/u(i1,i2,i3,
     & rc)))*rx(i1+off(1),i2+off(2),i3+off(3),2,1)*dri2(2)
                  coeff(icf(0,-1,0,eqn,vc),i1,i2,i3) = coeff(icf(0,-1,
     & 0,eqn,vc),i1,i2,i3) - ((2d0*fac*ux(i1,i2,i3,uc,2)/u(i1,i2,i3,
     & rc)))*rx(i1+off(1),i2+off(2),i3+off(3),2,1)*dri2(2)
c     coefficients for a  difference approximation to (2d0*fac*ux(i1,i2,i3,vc,1)/u(i1,i2,i3,rc)) * u[uc]_X
c     r-part
                  coeff(icf(+1,0,0,eqn,uc),i1,i2,i3) = coeff(icf(+1,0,
     & 0,eqn,uc),i1,i2,i3) + ((2d0*fac*ux(i1,i2,i3,vc,1)/u(i1,i2,i3,
     & rc)))*rx(i1+off(1),i2+off(2),i3+off(3),1,2)*dri2(1)
                  coeff(icf(-1,0,0,eqn,uc),i1,i2,i3) = coeff(icf(-1,0,
     & 0,eqn,uc),i1,i2,i3) - ((2d0*fac*ux(i1,i2,i3,vc,1)/u(i1,i2,i3,
     & rc)))*rx(i1+off(1),i2+off(2),i3+off(3),1,2)*dri2(1)
c     s-part
                  coeff(icf(0,+1,0,eqn,uc),i1,i2,i3) = coeff(icf(0,+1,
     & 0,eqn,uc),i1,i2,i3) + ((2d0*fac*ux(i1,i2,i3,vc,1)/u(i1,i2,i3,
     & rc)))*rx(i1+off(1),i2+off(2),i3+off(3),2,2)*dri2(2)
                  coeff(icf(0,-1,0,eqn,uc),i1,i2,i3) = coeff(icf(0,-1,
     & 0,eqn,uc),i1,i2,i3) - ((2d0*fac*ux(i1,i2,i3,vc,1)/u(i1,i2,i3,
     & rc)))*rx(i1+off(1),i2+off(2),i3+off(3),2,2)*dri2(2)
            coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &      coeff(icf(0,0,0,eqn,rc),i1,i2,i3) -
     &        (2d0*fac*ux(i1,i2,i3,uc,2)*ux(i1,i2,i3,vc,1))/
     &                  (u(i1,i2,i3,rc)**2)

            if ( isaxi.gt.0 .and. xyz(i1,i2,i3,2).gt.0d0 ) then

c           f43*fac*u[vc]^2/y^2/u[rc]
               coeff(icf(0,0,0,eqn,vc),i1,i2,i3) =
     &         coeff(icf(0,0,0,eqn,vc),i1,i2,i3) +
     &         2d0*f43*fac*u(i1,i2,i3,vc)/u(i1,i2,i3,rc)/
     &                   (xyz(i1,i2,i3,2)**2)

               coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &         coeff(icf(0,0,0,eqn,rc),i1,i2,i3) -
     &         f43*fac*u(i1,i2,i3,vc)**2/(u(i1,i2,i3,rc)**2)/
     &                    (xyz(i1,i2,i3,2)**2)

c           -f43*fac*u[vc]*u[uc]_x/y/u[rc]
c     coefficients for a central difference approximation to (-f43*fac/xyz(i1,i2,i3,2)/u(i1,i2,i3,rc)) * u[vc] u[uc]_X
c                 u[vc]^{n+1} * u[uc]^n_X
                                 coeff(icf(0,0,0,eqn,vc),i1,i2,i3) = 
     & coeff(icf(0,0,0,eqn,vc),i1,i2,i3) + ((-f43*fac/xyz(i1,i2,i3,2)
     & /u(i1,i2,i3,rc))) * ux(i1,i2,i3,uc,1)
c                 u[vc]^n * (r_X u[uc]_r^{n+1} + s_X u[uc]_s^{n+1} )
c                 r - part
                                 coeff(icf(+1,0,0,eqn,uc),i1,i2,i3) =  
     & coeff(icf(+1,0,0,eqn,uc),i1,i2,i3) + ((-f43*fac/xyz(i1,i2,i3,2)
     & /u(i1,i2,i3,rc))) * rx(i1+off(1),i2+off(2),i3+off(3),1,1)*u(i1+
     & off(1),i2+off(2),i3+off(3),vc)*dri2(1)
                                 coeff(icf(-1,0,0,eqn,uc),i1,i2,i3) =  
     & coeff(icf(-1,0,0,eqn,uc),i1,i2,i3) - ((-f43*fac/xyz(i1,i2,i3,2)
     & /u(i1,i2,i3,rc))) * rx(i1+off(1),i2+off(2),i3+off(3),1,1)*u(i1+
     & off(1),i2+off(2),i3+off(3),vc)*dri2(1)
c                 s - part
                                 coeff(icf(0,+1,0,eqn,uc),i1,i2,i3) =  
     & coeff(icf(0,+1,0,eqn,uc),i1,i2,i3) + ((-f43*fac/xyz(i1,i2,i3,2)
     & /u(i1,i2,i3,rc))) * rx(i1+off(1),i2+off(2),i3+off(3),2,1)*u(i1+
     & off(1),i2+off(2),i3+off(3),vc)*dri2(2)
                                 coeff(icf(0,-1,0,eqn,uc),i1,i2,i3) =  
     & coeff(icf(0,-1,0,eqn,uc),i1,i2,i3) - ((-f43*fac/xyz(i1,i2,i3,2)
     & /u(i1,i2,i3,rc))) * rx(i1+off(1),i2+off(2),i3+off(3),2,1)*u(i1+
     & off(1),i2+off(2),i3+off(3),vc)*dri2(2)
               coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &         coeff(icf(0,0,0,eqn,rc),i1,i2,i3) -
     &         (-f43*fac/xyz(i1,i2,i3,2)/u(i1,i2,i3,rc)) *
     &         u(i1,i2,i3,vc)*ux(i1,i2,i3,uc,1)/u(i1,i2,i3,rc)

c          -f43*fac*u[vc]*u[vc]_y/y/u[rc]
c     coefficients for a central difference approximation to (-f43*fac/xyz(i1,i2,i3,2)/u(i1,i2,i3,rc)) * u[vc] u[vc]_X
c                 u[vc]^{n+1} * u[vc]^n_X
                                 coeff(icf(0,0,0,eqn,vc),i1,i2,i3) = 
     & coeff(icf(0,0,0,eqn,vc),i1,i2,i3) + ((-f43*fac/xyz(i1,i2,i3,2)
     & /u(i1,i2,i3,rc))) * ux(i1,i2,i3,vc,2)
c                 u[vc]^n * (r_X u[vc]_r^{n+1} + s_X u[vc]_s^{n+1} )
c                 r - part
                                 coeff(icf(+1,0,0,eqn,vc),i1,i2,i3) =  
     & coeff(icf(+1,0,0,eqn,vc),i1,i2,i3) + ((-f43*fac/xyz(i1,i2,i3,2)
     & /u(i1,i2,i3,rc))) * rx(i1+off(1),i2+off(2),i3+off(3),1,2)*u(i1+
     & off(1),i2+off(2),i3+off(3),vc)*dri2(1)
                                 coeff(icf(-1,0,0,eqn,vc),i1,i2,i3) =  
     & coeff(icf(-1,0,0,eqn,vc),i1,i2,i3) - ((-f43*fac/xyz(i1,i2,i3,2)
     & /u(i1,i2,i3,rc))) * rx(i1+off(1),i2+off(2),i3+off(3),1,2)*u(i1+
     & off(1),i2+off(2),i3+off(3),vc)*dri2(1)
c                 s - part
                                 coeff(icf(0,+1,0,eqn,vc),i1,i2,i3) =  
     & coeff(icf(0,+1,0,eqn,vc),i1,i2,i3) + ((-f43*fac/xyz(i1,i2,i3,2)
     & /u(i1,i2,i3,rc))) * rx(i1+off(1),i2+off(2),i3+off(3),2,2)*u(i1+
     & off(1),i2+off(2),i3+off(3),vc)*dri2(2)
                                 coeff(icf(0,-1,0,eqn,vc),i1,i2,i3) =  
     & coeff(icf(0,-1,0,eqn,vc),i1,i2,i3) - ((-f43*fac/xyz(i1,i2,i3,2)
     & /u(i1,i2,i3,rc))) * rx(i1+off(1),i2+off(2),i3+off(3),2,2)*u(i1+
     & off(1),i2+off(2),i3+off(3),vc)*dri2(2)
               coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &         coeff(icf(0,0,0,eqn,rc),i1,i2,i3) -
     &         (-f43*fac/xyz(i1,i2,i3,2)/u(i1,i2,i3,rc)) *
     &         u(i1,i2,i3,vc)*ux(i1,i2,i3,vc,2)/u(i1,i2,i3,rc)

               if ( isswirl.gt.0 ) then

c               fac*u[wc]_x^2/u[rc]
c     linearize and add terms like fac * u[wc]_X^2/u[rc] to the matrix
c     fac * u[wc]_{1}^2/u[rc] --> fac * ( 2 u[wc]_X^n u[wc]_X^{n+1} - uc[rc]^{n+1} u[wc]_X^n/uc[rc]^n)/uc[rc]^n
c     fac * ( 2 u[wc]_X^n u[wc]_X^{n+1} )/u[rc]^n
c     r-part
                     coeff(icf(+1,0,0,eqn,wc),i1,i2,i3) = coeff(icf(+1,
     & 0,0,eqn,wc),i1,i2,i3) + (fac)*2d0*ux(i1,i2,i3,wc,1)*rx(i1+off(
     & 1),i2+off(2),i3+off(3),1,1)*dri2(1)/u(i1+off(1),i2+off(2),i3+
     & off(3),rc)
                     coeff(icf(-1,0,0,eqn,wc),i1,i2,i3) = coeff(icf(-1,
     & 0,0,eqn,wc),i1,i2,i3) - (fac)*2d0*ux(i1,i2,i3,wc,1)*rx(i1+off(
     & 1),i2+off(2),i3+off(3),1,1)*dri2(1)/u(i1+off(1),i2+off(2),i3+
     & off(3),rc)
c     s-part
                     coeff(icf(0,+1,0,eqn,wc),i1,i2,i3) = coeff(icf(0,+
     & 1,0,eqn,wc),i1,i2,i3) + (fac)*2d0*ux(i1,i2,i3,wc,1)*rx(i1+off(
     & 1),i2+off(2),i3+off(3),2,1)*dri2(2)/u(i1+off(1),i2+off(2),i3+
     & off(3),rc)
                     coeff(icf(0,-1,0,eqn,wc),i1,i2,i3) = coeff(icf(0,-
     & 1,0,eqn,wc),i1,i2,i3) - (fac)*2d0*ux(i1,i2,i3,wc,1)*rx(i1+off(
     & 1),i2+off(2),i3+off(3),2,1)*dri2(2)/u(i1+off(1),i2+off(2),i3+
     & off(3),rc)
c     - fac uc[rc]^{n+1} (u[wc]_X^n)^2/uc[rc]^n/uc[rc]^n
                     coeff(icf(0,0,0,eqn,rc),i1,i2,i3) = coeff(icf(0,0,
     & 0,eqn,rc),i1,i2,i3) - (fac) * ux(i1,i2,i3,wc,1)**2/(u(i1+off(1)
     & ,i2+off(2),i3+off(3),rc)**2)

c            fac*u[wc]_y^2/u[rc]
c     linearize and add terms like fac * u[wc]_X^2/u[rc] to the matrix
c     fac * u[wc]_{2}^2/u[rc] --> fac * ( 2 u[wc]_X^n u[wc]_X^{n+1} - uc[rc]^{n+1} u[wc]_X^n/uc[rc]^n)/uc[rc]^n
c     fac * ( 2 u[wc]_X^n u[wc]_X^{n+1} )/u[rc]^n
c     r-part
                     coeff(icf(+1,0,0,eqn,wc),i1,i2,i3) = coeff(icf(+1,
     & 0,0,eqn,wc),i1,i2,i3) + (fac)*2d0*ux(i1,i2,i3,wc,2)*rx(i1+off(
     & 1),i2+off(2),i3+off(3),1,2)*dri2(1)/u(i1+off(1),i2+off(2),i3+
     & off(3),rc)
                     coeff(icf(-1,0,0,eqn,wc),i1,i2,i3) = coeff(icf(-1,
     & 0,0,eqn,wc),i1,i2,i3) - (fac)*2d0*ux(i1,i2,i3,wc,2)*rx(i1+off(
     & 1),i2+off(2),i3+off(3),1,2)*dri2(1)/u(i1+off(1),i2+off(2),i3+
     & off(3),rc)
c     s-part
                     coeff(icf(0,+1,0,eqn,wc),i1,i2,i3) = coeff(icf(0,+
     & 1,0,eqn,wc),i1,i2,i3) + (fac)*2d0*ux(i1,i2,i3,wc,2)*rx(i1+off(
     & 1),i2+off(2),i3+off(3),2,2)*dri2(2)/u(i1+off(1),i2+off(2),i3+
     & off(3),rc)
                     coeff(icf(0,-1,0,eqn,wc),i1,i2,i3) = coeff(icf(0,-
     & 1,0,eqn,wc),i1,i2,i3) - (fac)*2d0*ux(i1,i2,i3,wc,2)*rx(i1+off(
     & 1),i2+off(2),i3+off(3),2,2)*dri2(2)/u(i1+off(1),i2+off(2),i3+
     & off(3),rc)
c     - fac uc[rc]^{n+1} (u[wc]_X^n)^2/uc[rc]^n/uc[rc]^n
                     coeff(icf(0,0,0,eqn,rc),i1,i2,i3) = coeff(icf(0,0,
     & 0,eqn,rc),i1,i2,i3) - (fac) * ux(i1,i2,i3,wc,2)**2/(u(i1+off(1)
     & ,i2+off(2),i3+off(3),rc)**2)

c            fac*u[wc]^2/y^2/u[rc]
               coeff(icf(0,0,0,eqn,wc),i1,i2,i3) =
     &         coeff(icf(0,0,0,eqn,wc),i1,i2,i3) +
     &         2d0*fac*u(i1,i2,i3,wc)/u(i1,i2,i3,rc)/
     &                   (xyz(i1,i2,i3,2)**2)

               coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &         coeff(icf(0,0,0,eqn,rc),i1,i2,i3) -
     &         fac*u(i1,i2,i3,wc)**2/(u(i1,i2,i3,rc)**2)/
     &                    (xyz(i1,i2,i3,2)**2)

c            -2*fac*u[wc]*u[wc]_y/y/u[rc]
c     coefficients for a central difference approximation to (-2d0*fac/xyz(i1,i2,i3,2)/u(i1,i2,i3,rc)) * u[wc] u[wc]_X
c                 u[wc]^{n+1} * u[wc]^n_X
                                 coeff(icf(0,0,0,eqn,wc),i1,i2,i3) = 
     & coeff(icf(0,0,0,eqn,wc),i1,i2,i3) + ((-2d0*fac/xyz(i1,i2,i3,2)
     & /u(i1,i2,i3,rc))) * ux(i1,i2,i3,wc,2)
c                 u[wc]^n * (r_X u[wc]_r^{n+1} + s_X u[wc]_s^{n+1} )
c                 r - part
                                 coeff(icf(+1,0,0,eqn,wc),i1,i2,i3) =  
     & coeff(icf(+1,0,0,eqn,wc),i1,i2,i3) + ((-2d0*fac/xyz(i1,i2,i3,2)
     & /u(i1,i2,i3,rc))) * rx(i1+off(1),i2+off(2),i3+off(3),1,2)*u(i1+
     & off(1),i2+off(2),i3+off(3),wc)*dri2(1)
                                 coeff(icf(-1,0,0,eqn,wc),i1,i2,i3) =  
     & coeff(icf(-1,0,0,eqn,wc),i1,i2,i3) - ((-2d0*fac/xyz(i1,i2,i3,2)
     & /u(i1,i2,i3,rc))) * rx(i1+off(1),i2+off(2),i3+off(3),1,2)*u(i1+
     & off(1),i2+off(2),i3+off(3),wc)*dri2(1)
c                 s - part
                                 coeff(icf(0,+1,0,eqn,wc),i1,i2,i3) =  
     & coeff(icf(0,+1,0,eqn,wc),i1,i2,i3) + ((-2d0*fac/xyz(i1,i2,i3,2)
     & /u(i1,i2,i3,rc))) * rx(i1+off(1),i2+off(2),i3+off(3),2,2)*u(i1+
     & off(1),i2+off(2),i3+off(3),wc)*dri2(2)
                                 coeff(icf(0,-1,0,eqn,wc),i1,i2,i3) =  
     & coeff(icf(0,-1,0,eqn,wc),i1,i2,i3) - ((-2d0*fac/xyz(i1,i2,i3,2)
     & /u(i1,i2,i3,rc))) * rx(i1+off(1),i2+off(2),i3+off(3),2,2)*u(i1+
     & off(1),i2+off(2),i3+off(3),wc)*dri2(2)
               coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &         coeff(icf(0,0,0,eqn,rc),i1,i2,i3) -
     &         (-2d0*fac/xyz(i1,i2,i3,2)/u(i1,i2,i3,rc)) *
     &         u(i1,i2,i3,wc)*ux(i1,i2,i3,wc,2)/u(i1,i2,i3,rc)


               end if

            end  if

            if ( (av4.gt.0d0 .or. av2.gt.0d0)
     &          .and. .true.
     &           ) then
c       add some artificial viscosity
c               drar = av4*(rx(i1,i2,i3,1,1)**2+rx(i1,i2,i3,2,2)**2)*
c     &                       (dri(1)+dri(2))
c               if ( i1.eq.30 ) write(*,*) i2,drar,rx(i1,i2,i3,2,2)

               if ( .true. ) then

               do eqn=uc,vc
                  drar = av2
                  coeff(icf(1,0,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(1,0,0,eqn,eqn),i1,i2,i3) -drar
                  coeff(icf(-1,0,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(-1,0,0,eqn,eqn),i1,i2,i3) -drar
                  coeff(icf(0,0,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(0,0,0,eqn,eqn),i1,i2,i3) + 4d0*drar
                  coeff(icf(0,1,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(0,1,0,eqn,eqn),i1,i2,i3) -drar
                  coeff(icf(0,-1,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(0,-1,0,eqn,eqn),i1,i2,i3) -drar

c                  drar = av4*(rx(i1,i2,i3,1,1)**4+rx(i1,i2,i3,1,2)**4)
c     &                 *(dri(1))
                  drar = av4!*abs(rx(i1,i2,i3,1,1)+rx(i1,i2,i3,2,2))!*(rx(i1,i2,i3,1,1)**4+rx(i1,i2,i3,2,2)**4)
c     &                 *(dri(1))

                  coeff(icf(2,0,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(2,0,0,eqn,eqn),i1,i2,i3) +drar
                  coeff(icf(-2,0,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(-2,0,0,eqn,eqn),i1,i2,i3) +drar
                  coeff(icf(1,0,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(1,0,0,eqn,eqn),i1,i2,i3) -4d0*drar
                  coeff(icf(-1,0,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(-1,0,0,eqn,eqn),i1,i2,i3) -4d0*drar

                  coeff(icf(0,0,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(0,0,0,eqn,eqn),i1,i2,i3) + 6d0*drar

                  drar = av4!*(rx(i1,i2,i3,2,2)**4+rx(i1,i2,i3,2,1)**4)
c     &                 *(dri(2))

                  coeff(icf(0,0,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(0,0,0,eqn,eqn),i1,i2,i3) + 6d0*drar

                  coeff(icf(0,1,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(0,1,0,eqn,eqn),i1,i2,i3) -4d0*drar
                  coeff(icf(0,-1,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(0,-1,0,eqn,eqn),i1,i2,i3) -4d0*drar
                  coeff(icf(0,2,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(0,2,0,eqn,eqn),i1,i2,i3) +drar
                  coeff(icf(0,-2,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(0,-2,0,eqn,eqn),i1,i2,i3) +drar

               end do
               endif
               if ( .true. ) then

                  drar = av4    !*(rx(i1,i2,i3,1,1)**4+rx(i1,i2,i3,2,2)**4)
c     &                       *(dri(1)+dri(2))
c                  do eqn=wc,tc
                  if ( isswirl.gt.0 ) then
                     d=wc
                  else
                     d=tc
                  endif
                do eqn=d,tc
c                  eqn = wc
C     coeff(icf(1,0,0,eqn,eqn),i1,i2,i3) =
C      &            coeff(icf(1,0,0,eqn,eqn),i1,i2,i3) -drar
C                   coeff(icf(-1,0,0,eqn,eqn),i1,i2,i3) =
C      &            coeff(icf(-1,0,0,eqn,eqn),i1,i2,i3) -drar
C                   coeff(icf(0,0,0,eqn,eqn),i1,i2,i3) =
C      &            coeff(icf(0,0,0,eqn,eqn),i1,i2,i3) + 4d0*drar
C                   coeff(icf(0,1,0,eqn,eqn),i1,i2,i3) =
C      &            coeff(icf(0,1,0,eqn,eqn),i1,i2,i3) -drar
C                   coeff(icf(0,-1,0,eqn,eqn),i1,i2,i3) =
C      &            coeff(icf(0,-1,0,eqn,eqn),i1,i2,i3) -drar
                  drar = av2
                  coeff(icf(1,0,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(1,0,0,eqn,eqn),i1,i2,i3) -drar
                  coeff(icf(-1,0,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(-1,0,0,eqn,eqn),i1,i2,i3) -drar
                  coeff(icf(0,0,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(0,0,0,eqn,eqn),i1,i2,i3) + 4d0*drar
                  coeff(icf(0,1,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(0,1,0,eqn,eqn),i1,i2,i3) -drar
                  coeff(icf(0,-1,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(0,-1,0,eqn,eqn),i1,i2,i3) -drar


                  drar = av4!*2d0/(dr(1)+dr(2))    !*(rx(i1,i2,i3,1,1)**4+rx(i1,i2,i3,2,2)**4)
                     coeff(icf(2,0,0,eqn,eqn),i1,i2,i3) =
     &               coeff(icf(2,0,0,eqn,eqn),i1,i2,i3) +drar
                     coeff(icf(-2,0,0,eqn,eqn),i1,i2,i3) =
     &               coeff(icf(-2,0,0,eqn,eqn),i1,i2,i3) +drar
                     coeff(icf(1,0,0,eqn,eqn),i1,i2,i3) =
     &               coeff(icf(1,0,0,eqn,eqn),i1,i2,i3) -4d0*drar
                     coeff(icf(-1,0,0,eqn,eqn),i1,i2,i3) =
     &               coeff(icf(-1,0,0,eqn,eqn),i1,i2,i3) -4d0*drar

                     coeff(icf(0,0,0,eqn,eqn),i1,i2,i3) =
     &               coeff(icf(0,0,0,eqn,eqn),i1,i2,i3) + 12d0*drar

                     coeff(icf(0,1,0,eqn,eqn),i1,i2,i3) =
     &               coeff(icf(0,1,0,eqn,eqn),i1,i2,i3) -4d0*drar
                     coeff(icf(0,-1,0,eqn,eqn),i1,i2,i3) =
     &               coeff(icf(0,-1,0,eqn,eqn),i1,i2,i3) -4d0*drar
                     coeff(icf(0,2,0,eqn,eqn),i1,i2,i3) =
     &               coeff(icf(0,2,0,eqn,eqn),i1,i2,i3) +drar
                     coeff(icf(0,-2,0,eqn,eqn),i1,i2,i3) =
     &               coeff(icf(0,-2,0,eqn,eqn),i1,i2,i3) +drar

                  end do

               endif

               eqn = rc
               if ( .false. ) then

                  drar = av2*dabs(u(i1,i2,i3,rc))
                  coeff(icf(1,0,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(1,0,0,eqn,eqn),i1,i2,i3) -
     &                 drar/u(i1+1,i2,i3,rc)
                  coeff(icf(-1,0,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(-1,0,0,eqn,eqn),i1,i2,i3) -
     &                 drar/u(i1-1,i2,i3,rc)

                  coeff(icf(0,0,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(0,0,0,eqn,eqn),i1,i2,i3) +
     &                 2d0*drar/u(i1,i2,i3,rc)

                  drar = av2
                  coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &            coeff(icf(0,0,0,eqn,rc),i1,i2,i3) -
     &                 drar*dlog(dabs(u(i1+1,i2,i3,rc)))
                  coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &            coeff(icf(0,0,0,eqn,rc),i1,i2,i3) -
     &                 drar*dlog(dabs(u(i1-1,i2,i3,rc)))

                  coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &            coeff(icf(0,0,0,eqn,rc),i1,i2,i3) +
     &                 2d0*drar*dlog(dabs(u(i1,i2,i3,rc)))

c                  drar = av4*(rx(i1,i2,i3,1,1)**4+rx(i1,i2,i3,2,2)**4)
c     &                 *(dri(1)+dri(2))
                  drar = av4!*dabs(rx(i1,i2,i3,1,1))!*(rx(i1,i2,i3,1,1)**4)
c     &                 *(dri(1))
                  drar = drar * dabs(u(i1,i2,i3,rc))

                  coeff(icf(2,0,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(2,0,0,eqn,eqn),i1,i2,i3) +
     &                 drar/u(i1+2,i2,i3,rc)
                  coeff(icf(-2,0,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(-2,0,0,eqn,eqn),i1,i2,i3) +
     &                 drar/u(i1-2,i2,i3,rc)
                  coeff(icf(1,0,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(1,0,0,eqn,eqn),i1,i2,i3) -
     &                 4d0*drar/u(i1+1,i2,i3,rc)
                  coeff(icf(-1,0,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(-1,0,0,eqn,eqn),i1,i2,i3) -
     &                 4d0*drar/u(i1-1,i2,i3,rc)

                  coeff(icf(0,0,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(0,0,0,eqn,eqn),i1,i2,i3) +
     &                 6d0*drar/u(i1,i2,i3,rc)
c                  drar = av4*(rx(i1,i2,i3,1,1)**4+rx(i1,i2,i3,2,2)**4)
c     &                       *(dri(1)+dri(2))

                  drar = av4!*dabs(rx(i1,i2,i3,1,1))!*(rx(i1,i2,i3,1,1)**4)
c     &                 *(dri(1))

                  coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &            coeff(icf(0,0,0,eqn,rc),i1,i2,i3) +
     &                 drar*dlog(dabs(u(i1+2,i2,i3,rc)))
                  coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &            coeff(icf(0,0,0,eqn,rc),i1,i2,i3) +
     &                 drar*dlog(dabs(u(i1-2,i2,i3,rc)))
                  coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &            coeff(icf(0,0,0,eqn,rc),i1,i2,i3) -
     &                 4d0*drar*dlog(dabs(u(i1+1,i2,i3,rc)))
                  coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &            coeff(icf(0,0,0,eqn,rc),i1,i2,i3) -
     &                 4d0*drar*dlog(dabs(u(i1-1,i2,i3,rc)))

                  coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &            coeff(icf(0,0,0,eqn,rc),i1,i2,i3) +
     &                 6d0*drar*dlog(dabs(u(i1,i2,i3,rc)))


                  drar = av2*dabs(u(i1,i2,i3,rc))
                  coeff(icf(0,0,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(0,0,0,eqn,eqn),i1,i2,i3) +
     &                 2d0*drar/u(i1,i2,i3,rc)

                  coeff(icf(0,1,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(0,1,0,eqn,eqn),i1,i2,i3) -
     &                 drar/u(i1,i2+1,i3,rc)
                  coeff(icf(0,-1,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(0,-1,0,eqn,eqn),i1,i2,i3) -
     &                 drar/u(i1,i2-1,i3,rc)

                  drar = av2
                  coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &            coeff(icf(0,0,0,eqn,rc),i1,i2,i3) +
     &                 2d0*drar*dlog(dabs(u(i1,i2,i3,rc)))
                  coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &            coeff(icf(0,0,0,eqn,rc),i1,i2,i3) -
     &                 drar*dlog(dabs(u(i1,i2+1,i3,rc)))
                  coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &            coeff(icf(0,0,0,eqn,rc),i1,i2,i3) -
     &                 drar*dlog(dabs(u(i1,i2-1,i3,rc)))

                  drar = av4!*dabs(rx(i1,i2,i3,2,2))!*(rx(i1,i2,i3,2,2)**4)
c     &                 *(dri(2))
                  drar = drar * dabs(u(i1,i2,i3,rc))

                  coeff(icf(0,0,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(0,0,0,eqn,eqn),i1,i2,i3) +
     &                 6d0*drar/u(i1,i2,i3,rc)

                  coeff(icf(0,1,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(0,1,0,eqn,eqn),i1,i2,i3) -
     &                 4d0*drar/u(i1,i2+1,i3,rc)
                  coeff(icf(0,-1,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(0,-1,0,eqn,eqn),i1,i2,i3) -
     &                 4d0*drar/u(i1,i2-1,i3,rc)
                  coeff(icf(0,2,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(0,2,0,eqn,eqn),i1,i2,i3) +
     &                 drar/u(i1,i2+2,i3,rc)
                  coeff(icf(0,-2,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(0,-2,0,eqn,eqn),i1,i2,i3) +
     &                 drar/u(i1,i2-2,i3,rc)
c                  drar = av4*(rx(i1,i2,i3,1,1)**4+rx(i1,i2,i3,2,2)**4)
c     &                       *(dri(1)+dri(2))

                  drar = av4!*dabs(rx(i1,i2,i3,2,2))!*(rx(i1,i2,i3,2,2)**4)
c     &                 *(dri(2))
                  coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &            coeff(icf(0,0,0,eqn,rc),i1,i2,i3) +
     &                 6d0*drar*dlog(dabs(u(i1,i2,i3,rc)))

                  coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &            coeff(icf(0,0,0,eqn,rc),i1,i2,i3) +
     &                 drar*dlog(dabs(u(i1,i2+2,i3,rc)))
                  coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &            coeff(icf(0,0,0,eqn,rc),i1,i2,i3) +
     &                 drar*dlog(dabs(u(i1,i2-2,i3,rc)))
                  coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &            coeff(icf(0,0,0,eqn,rc),i1,i2,i3) -
     &                 4d0*drar*dlog(dabs(u(i1,i2+1,i3,rc)))
                  coeff(icf(0,0,0,eqn,rc),i1,i2,i3) =
     &            coeff(icf(0,0,0,eqn,rc),i1,i2,i3) -
     &                 4d0*drar*dlog(dabs(u(i1,i2-1,i3,rc)))


               else  if ( .true.) then
                  eqn = rc

                  drar = av2
                  coeff(icf(1,0,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(1,0,0,eqn,eqn),i1,i2,i3) -drar
                  coeff(icf(-1,0,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(-1,0,0,eqn,eqn),i1,i2,i3) -drar
                  coeff(icf(0,0,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(0,0,0,eqn,eqn),i1,i2,i3) + 4d0*drar
                  coeff(icf(0,1,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(0,1,0,eqn,eqn),i1,i2,i3) -drar
                  coeff(icf(0,-1,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(0,-1,0,eqn,eqn),i1,i2,i3) -drar


                  drar = av4
                  coeff(icf(2,0,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(2,0,0,eqn,eqn),i1,i2,i3) +drar
                  coeff(icf(-2,0,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(-2,0,0,eqn,eqn),i1,i2,i3) +drar
                  coeff(icf(1,0,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(1,0,0,eqn,eqn),i1,i2,i3) -4d0*drar
                  coeff(icf(-1,0,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(-1,0,0,eqn,eqn),i1,i2,i3) -4d0*drar

                  coeff(icf(0,0,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(0,0,0,eqn,eqn),i1,i2,i3) + 12d0*drar

                  coeff(icf(0,1,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(0,1,0,eqn,eqn),i1,i2,i3) -4d0*drar
                  coeff(icf(0,-1,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(0,-1,0,eqn,eqn),i1,i2,i3) -4d0*drar
                  coeff(icf(0,2,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(0,2,0,eqn,eqn),i1,i2,i3) +drar
                  coeff(icf(0,-2,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(0,-2,0,eqn,eqn),i1,i2,i3) +drar

               else if ( .false. ) then
                  drar = jaci*av4/xyz(i1,i2,i3,2)

                  coeff(icf(2,0,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(2,0,0,eqn,eqn),i1,i2,i3) +drar*
     &                 xyz(i1+2,i2,i3,2)*jac(i1+2,i2,i3)
                  coeff(icf(-2,0,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(-2,0,0,eqn,eqn),i1,i2,i3) + drar*
     &                 xyz(i1-2,i2,i3,2)*jac(i1-2,i2,i3)
                  coeff(icf(1,0,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(1,0,0,eqn,eqn),i1,i2,i3) -4d0*drar*
     &                 xyz(i1+1,i2,i3,2)*jac(i1+1,i2,i3)
                  coeff(icf(-1,0,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(-1,0,0,eqn,eqn),i1,i2,i3) -4d0*drar*
     &                 xyz(i1-1,i2,i3,2)*jac(i1-1,i2,i3)

                  coeff(icf(0,0,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(0,0,0,eqn,eqn),i1,i2,i3) + 12d0*drar*
     &                 xyz(i1,i2,i3,2)*jac(i1,i2,i3)

                  coeff(icf(0,1,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(0,1,0,eqn,eqn),i1,i2,i3) -4d0*drar*
     &                 xyz(i1,i2+1,i3,2)*jac(i1,i2+1,i3)
                  coeff(icf(0,-1,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(0,-1,0,eqn,eqn),i1,i2,i3) -4d0*drar*
     &                 xyz(i1,i2-1,i3,2)*jac(i1,i2-1,i3)
                  coeff(icf(0,2,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(0,2,0,eqn,eqn),i1,i2,i3) +drar*
     &                 xyz(i1,i2+2,i3,2)*jac(i1,i2+2,i3)
                  coeff(icf(0,-2,0,eqn,eqn),i1,i2,i3) =
     &            coeff(icf(0,-2,0,eqn,eqn),i1,i2,i3) +drar*
     &                 xyz(i1,i2-2,i3,2)*jac(i1,i2-2,i3)

               endif

            endif

c            do cc=0,iprm(11)*iprm(2)**2-1
c               coeff(cc,i1,i2,i3) = u(i1,i2,i3,rc)*coeff(cc,i1,i2,i3)
c            end do

         endif ! is interior
         endif ! mask.gt.0
      end do ! i1
      end do ! i2
      end do ! i3

      return
      end

      subroutine icnsrhs(igd, igi, xyz,rx,jac, mask, iprm,rprm,u, rhs)
c
c     060306 kkc Initial version
c     
c     icnsrhs computes the right hand side vector for the linearized compressible
c     Navier-Stokes equations on a curvilinear grid.  It should work for
c     both Cartesian and (2D) cylindrical coordinate systems.  The method discretizes
c     the non-dimensionalized, primitive-variable, and constant coefficient 
c     (viscosity, thermal conduction) version of the equations.
c
c     Notes: 
c          - only 2D is supported right now
c          - artificial dissipation could be added to the rhs using the
c            Jameson method's subroutine
c          - any forcing terms are NOT added here, they are done elsewhere in cns.C
c
      implicit none

c     INPUT 
      integer igd(2,*) ! grid dimensions
      integer igi(2,*) ! interior grid dimensions (disc points)
      integer iprm(*) ! integer solver parameters
      double precision xyz(igd(1,1):igd(2,1),
     &                     igd(1,2):igd(2,2),
     &                     igd(1,3):igd(2,3), *) ! grid vertices
      double precision rx(igd(1,1):igd(2,1),
     &                    igd(1,2):igd(2,2),
     &                    igd(1,3):igd(2,3), iprm(1), *) ! mapping derivatives
      double precision jac(igd(1,1):igd(2,1),
     &                     igd(1,2):igd(2,2),
     &                     igd(1,3):igd(2,3)) ! det mapping jacobian
      integer mask(igd(1,1):igd(2,1),
     &             igd(1,2):igd(2,2),
     &             igd(1,3):igd(2,3))   ! grid mask
      double precision rprm(*) ! real solver parameters
      double precision u(igd(1,1):igd(2,1),
     &                   igd(1,2):igd(2,2),
     &                   igd(1,3):igd(2,3), *) ! state to linearize about

c     OUTPUT
      double precision rhs(igd(1,1):igd(2,1),
     &                     igd(1,2):igd(2,2),
     &                     igd(1,3):igd(2,3), *)

c     LOCAL
      integer i1,i2,i3,d,e,c,ndim,ncmp,cc
      integer rc,uc,vc,wc,tc,gmove,isaxi,isswirl,eqn
      integer isten_size,width,hwidth,width3
      double precision ren,prn,man,gam,gm1,impfac,dr(3),dri(3),dri2(3)
      double precision t0,dt,axifac, ur,us, gm2, oren, oprn, f43, f13
      double precision fac,jaci,drar,lur,lus,avd,rfloor,av2,av4
      double precision orad,drad
      logical usestrik,oldstrik,avgmomforcing
      double precision alpha

c     STATEMENT FUNCTIONS
      include 'icnssfdec.h'
      include 'icnssf.h'
c     END OF STATEMENT FUNCTIONS

      if ( .false. ) then
      write (*,*) "INSIDE ICNSRHS"
      write (*,'("grid bounds : ",2(1x,i4))') igd(1,1), igd(2,1)
      write (*,'("              ",2(1x,i4))') igd(1,2), igd(2,2)
      write (*,'("              ",2(1x,i4))') igd(1,3), igd(2,3)

      write (*,'("disc bounds : ",2(1x,i4))') igi(1,1), igi(2,1)
      write (*,'("              ",2(1x,i4))') igi(1,2), igi(2,2)
      write (*,'("              ",2(1x,i4))') igi(1,3), igi(2,3)

      write (*,'("num dim.     : ",i4))') iprm(1)
      write (*,'("isaxi        : ",i2))') iprm(9)
      write (*,'("isswirl      : ",i2))') iprm(10)

      write (*,'("Reynold`s #  : ",f10.5)') rprm(1)
      write (*,'("Prandtl   #  : ",f10.5)') rprm(2)
      write (*,'("Mach      #  : ",f10.5)') rprm(3)
      write (*,'("gamma        : ",f10.5)') rprm(4)
      write (*,'("dr(*)        : ",3(2x,f10.5))') rprm(6),rprm(7),rprm(
     & 8)
      endif

      off(1) = 0
      off(2) = 0
      off(3) = 0
      occ = -1
      oce = -1

      usestrik = .true.
      oldstrik = .false.
c      oldstrik = .true.

c      usestrik = .false.
      alpha = 0d0
      alpha = 1d0/6d0
c      alpha = 1d0/7d0
c      alpha = 1d0/24d0
c      alpha = 1d0/60d0
c      alpha = 1d0/600d0
      avgmomforcing = .false.

      ndim = iprm(1)
      ncmp = iprm(2)
      rc   = iprm(3)+1
      uc   = iprm(4)+1
      vc   = iprm(5)+1
      wc   = iprm(6)+1
      tc   = iprm(7)+1
      gmove= iprm(8)
      isaxi= iprm(9)
      isswirl= iprm(10)

      ren = rprm(1)
      prn = rprm(2)
      man = rprm(3)
      gam =rprm(4)
      impfac=rprm(5)
      dr(1) = rprm(6)
      dr(2) = rprm(7)
      dr(3) = rprm(8)
c      t0    = rprm(9)
      avd = rprm(9)
      dt    = rprm(10)
      av4= avd
      av2=0d0

      alpha = rprm(18)

      f43 = 4d0/3d0
      f13 = 1d0/3d0
      oren = 1d0/ren
      oprn = 1d0/prn
      gm1 = gam-1d0
      gm2 = 1d0/(gam*man*man)
      do d=1,ndim
         dri(d) = 1d0/dr(d)
         dri2(d) = .5d0*dri(d)
      end do

      if ( isaxi.ne.0 ) then
         axifac = 1d0
      else
         axifac = 0d0
      endif

      do i3=igi(1,3), igi(2,3)
      do i2=igi(1,2), igi(2,2)
      do i1=igi(1,1), igi(2,1)

         if ( mask(i1,i2,i3).gt.0 ) then
c
c     CONTINUITY EQUATION
c     
            eqn = rc
            jaci = 1d0/jac(i1,i2,i3)
            do d=uc,vc
               cc = d-uc+1

               if ( .true. ) then
c           linearized D_{o\xi} (\rho a_{11}u + \rho a_{12}v)/J
c      linearize terms like -jaci*rx(i1-1,i2,i3,1,cc)*jac(i1-1,i2,i3)*dri2(1) * u(rc)* u(d) at the grid point offset by -1, 0, 0
                     rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (-jaci*rx(
     & i1-1,i2,i3,1,cc)*jac(i1-1,i2,i3)*dri2(1))*u(i1+(-1)+off(1),i2+(
     & 0)+off(2),i3+(0)+off(3),rc)*u(i1+(-1)+off(1),i2+(0)+off(2),i3+(
     & 0)+off(3),d)
c      linearize terms like jaci*rx(i1+1,i2,i3,1,cc)*jac(i1+1,i2,i3)*dri2(1) * u(rc)* u(d) at the grid point offset by 1, 0, 0
                     rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (jaci*rx(
     & i1+1,i2,i3,1,cc)*jac(i1+1,i2,i3)*dri2(1))*u(i1+(1)+off(1),i2+(
     & 0)+off(2),i3+(0)+off(3),rc)*u(i1+(1)+off(1),i2+(0)+off(2),i3+(
     & 0)+off(3),d)

c           linearized D_{o\eta} (\rho a_{21}u + \rho a_{22}v)/J
c      linearize terms like -jaci*rx(i1,i2-1,i3,2,cc)*jac(i1,i2-1,i3)*dri2(2) * u(rc)* u(d) at the grid point offset by 0, -1, 0
                     rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (-jaci*rx(
     & i1,i2-1,i3,2,cc)*jac(i1,i2-1,i3)*dri2(2))*u(i1+(0)+off(1),i2+(-
     & 1)+off(2),i3+(0)+off(3),rc)*u(i1+(0)+off(1),i2+(-1)+off(2),i3+(
     & 0)+off(3),d)
c      linearize terms like jaci*rx(i1,i2+1,i3,2,cc)*jac(i1,i2+1,i3)*dri2(2) * u(rc)* u(d) at the grid point offset by 0, 1, 0
                     rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (jaci*rx(
     & i1,i2+1,i3,2,cc)*jac(i1,i2+1,i3)*dri2(2))*u(i1+(0)+off(1),i2+(
     & 1)+off(2),i3+(0)+off(3),rc)*u(i1+(0)+off(1),i2+(1)+off(2),i3+(
     & 0)+off(3),d)
               endif
            if ( usestrik ) then

               if ( oldstrik ) then
                 rhs(i1,i2,i3,rc) = rhs(i1,i2,i3,rc) +
     & u(i1,i2,i3,rc)*(ux4p(i1,i2,i3,d,cc)-ux(i1,i2,i3,d,cc))

               else
c                  if ( i1.gt.igi(1,1) ) then
                  drar = alpha*jaci*dri(1)
                  fac =     -drar*rx(i1+1,i2,i3,1,cc)*jac(i1+1,i2,i3)
c      linearize terms like fac * u(eqn)* u(d) at the grid point offset by +1, 0, 0
                        rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (fac)*
     & u(i1+(+1)+off(1),i2+(0)+off(2),i3+(0)+off(3),eqn)*u(i1+(+1)+
     & off(1),i2+(0)+off(2),i3+(0)+off(3),d)
                  fac =  3d0*drar*rx(i1  ,i2,i3,1,cc)*jac(i1  ,i2,i3)
c      linearize terms like fac * u(eqn)* u(d) at the grid point offset by 0, 0, 0
                        rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (fac)*
     & u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)+off(3),eqn)*u(i1+(0)+off(
     & 1),i2+(0)+off(2),i3+(0)+off(3),d)
                  fac = -3d0*drar*rx(i1-1,i2,i3,1,cc)*jac(i1-1,i2,i3)
c      linearize terms like fac * u(eqn)* u(d) at the grid point offset by -1, 0, 0
                        rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (fac)*
     & u(i1+(-1)+off(1),i2+(0)+off(2),i3+(0)+off(3),eqn)*u(i1+(-1)+
     & off(1),i2+(0)+off(2),i3+(0)+off(3),d)
                  fac =      drar*rx(i1-2,i2,i3,1,cc)*jac(i1-2,i2,i3)
c      linearize terms like fac * u(eqn)* u(d) at the grid point offset by -2, 0, 0
                        rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (fac)*
     & u(i1+(-2)+off(1),i2+(0)+off(2),i3+(0)+off(3),eqn)*u(i1+(-2)+
     & off(1),i2+(0)+off(2),i3+(0)+off(3),d)
c                  endif
c                  if ( i2.gt.igi(1,2) ) then 
                  drar = alpha*jaci*dri(2)
                  fac =     -drar*rx(i1,i2+1,i3,2,cc)*jac(i1,i2+1,i3)
c      linearize terms like fac * u(eqn)* u(d) at the grid point offset by 0, +1, 0
                        rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (fac)*
     & u(i1+(0)+off(1),i2+(+1)+off(2),i3+(0)+off(3),eqn)*u(i1+(0)+off(
     & 1),i2+(+1)+off(2),i3+(0)+off(3),d)
                  fac =  3d0*drar*rx(i1,i2  ,i3,2,cc)*jac(i1,i2,i3)
c      linearize terms like fac * u(eqn)* u(d) at the grid point offset by 0, 0, 0
                        rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (fac)*
     & u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)+off(3),eqn)*u(i1+(0)+off(
     & 1),i2+(0)+off(2),i3+(0)+off(3),d)
                  fac = -3d0*drar*rx(i1,i2-1,i3,2,cc)*jac(i1,i2-1,i3)
c      linearize terms like fac * u(eqn)* u(d) at the grid point offset by 0, -1, 0
                        rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (fac)*
     & u(i1+(0)+off(1),i2+(-1)+off(2),i3+(0)+off(3),eqn)*u(i1+(0)+off(
     & 1),i2+(-1)+off(2),i3+(0)+off(3),d)
                  fac =     drar*rx(i1,i2-2,i3,2,cc)*jac(i1,i2-2,i3)
c      linearize terms like fac * u(eqn)* u(d) at the grid point offset by 0, -2, 0
                        rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (fac)*
     & u(i1+(0)+off(1),i2+(-2)+off(2),i3+(0)+off(3),eqn)*u(i1+(0)+off(
     & 1),i2+(-2)+off(2),i3+(0)+off(3),d)
c                  endif
               endif
            endif

            end do

c     freestream correction
            if ( .true. ) then
               do d=1,ndim
                  cc = uc+d-1
                  fac = jaci*(
     &                 (jac(i1+1,i2,i3)*rx(i1+1,i2,i3,1,d)-
     &                 jac(i1-1,i2,i3)*rx(i1-1,i2,i3,1,d))*dri2(1) +
     &                 (jac(i1,i2+1,i3)*rx(i1,i2+1,i3,2,d)-
     &                 jac(i1,i2-1,i3)*rx(i1,i2-1,i3,2,d))*dri2(2))
c      linearize terms like -fac * u(rc)* u(cc) at the grid point offset by 0, 0, 0
                        rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (-fac)*
     & u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)+off(3),rc)*u(i1+(0)+off(1)
     & ,i2+(0)+off(2),i3+(0)+off(3),cc)

                  if ( .not. oldstrik ) then
                     drar = alpha*jaci*dri(1)
                     fac =
     &                    -drar*rx(i1+1,i2,i3,1,d)*jac(i1+1,i2,i3)
     &                +3d0*drar*rx(i1  ,i2,i3,1,d)*jac(i1,i2,i3)
     &                -3d0*drar*rx(i1-1,i2,i3,1,d)*jac(i1-1,i2,i3)
     &                    +drar*rx(i1-2,i2,i3,1,d)*jac(i1-2,i2,i3)
                     drar = alpha*jaci*dri(2)
                     fac = fac
     &                    -drar*rx(i1,i2+1,i3,2,d)*jac(i1,i2+1,i3)
     &                +3d0*drar*rx(i1,i2  ,i3,2,d)*jac(i1,i2,i3)
     &                -3d0*drar*rx(i1,i2-1,i3,2,d)*jac(i1,i2-1,i3)
     &                    +drar*rx(i1,i2-2,i3,2,d)*jac(i1,i2-2,i3)
c      linearize terms like -fac * u(rc)* u(cc) at the grid point offset by 0, 0, 0
                           rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (-
     & fac)*u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)+off(3),rc)*u(i1+(0)+
     & off(1),i2+(0)+off(2),i3+(0)+off(3),cc)
                  endif

               enddo
            endif

c     axisymmetric forcing
            if ( isaxi.gt.0 .and.
     &           xyz(i1,i2,i3,2).gt.0d0 ) then

               do d=1,ndim
                  cc = uc+d-1

                  if ( .true. ) then
                           if ( .true. ) then
                           drar = (1)*dri2(1)*jaci*jac(i1+off(1)-1,i2+
     & off(2),i3+off(3))*(xyz(i1+off(1),i2+off(2),i3+off(3),2)-xyz(i1+
     & off(1)-1,i2+off(2),i3+off(3),2))*rx(i1+off(1)-1,i2+off(2),i3+
     & off(3),1,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
c      linearize terms like drar * u(rc)* u(cc) at the grid point offset by -1, 0, 0
                                 rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) 
     & + (drar)*u(i1+(-1)+off(1),i2+(0)+off(2),i3+(0)+off(3),rc)*u(i1+
     & (-1)+off(1),i2+(0)+off(2),i3+(0)+off(3),cc)
                           drar = (1)*dri2(1)*jaci*jac(i1+off(1)+1,i2+
     & off(2),i3+off(3))*(xyz(i1+off(1)+1,i2+off(2),i3+off(3),2)-xyz(
     & i1+off(1),i2+off(2),i3+off(3),2))*rx(i1+off(1)+1,i2+off(2),i3+
     & off(3),1,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
c      linearize terms like drar * u(rc)* u(cc) at the grid point offset by +1, 0, 0
                                 rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) 
     & + (drar)*u(i1+(+1)+off(1),i2+(0)+off(2),i3+(0)+off(3),rc)*u(i1+
     & (+1)+off(1),i2+(0)+off(2),i3+(0)+off(3),cc)
                           drar = (1)*dri2(2)*jaci*jac(i1+off(1),i2+
     & off(2)-1,i3+off(3))*(xyz(i1+off(1),i2+off(2),i3+off(3),2)-xyz(
     & i1+off(1),i2+off(2)-1,i3+off(3),2))*rx(i1+off(1),i2+off(2)-1,
     & i3+off(3),2,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
c      linearize terms like drar * u(rc)* u(cc) at the grid point offset by 0, -1, 0
                                 rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) 
     & + (drar)*u(i1+(0)+off(1),i2+(-1)+off(2),i3+(0)+off(3),rc)*u(i1+
     & (0)+off(1),i2+(-1)+off(2),i3+(0)+off(3),cc)
                           drar = (1)*dri2(2)*jaci*jac(i1+off(1),i2+
     & off(2)+1,i3+off(3))*(xyz(i1+off(1),i2+off(2)+1,i3+off(3),2)-
     & xyz(i1+off(1),i2+off(2),i3+off(3),2))*rx(i1+off(1),i2+off(2)+1,
     & i3+off(3),2,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
c      linearize terms like drar * u(rc)* u(cc) at the grid point offset by 0, +1, 0
                                 rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) 
     & + (drar)*u(i1+(0)+off(1),i2+(+1)+off(2),i3+(0)+off(3),rc)*u(i1+
     & (0)+off(1),i2+(+1)+off(2),i3+(0)+off(3),cc)
                           else if ( .false. ) then
                              drar = (1)*.25d0
c      linearize terms like drar/xyz(i1-1,i2,i3,2) * u(rc)* u(cc) at the grid point offset by -1, 0, 0
                                   rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,
     & eqn) + (drar/xyz(i1-1,i2,i3,2))*u(i1+(-1)+off(1),i2+(0)+off(2),
     & i3+(0)+off(3),rc)*u(i1+(-1)+off(1),i2+(0)+off(2),i3+(0)+off(3),
     & cc)
c      linearize terms like drar/xyz(i1+1,i2,i3,2) * u(rc)* u(cc) at the grid point offset by +1, 0, 0
                                   rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,
     & eqn) + (drar/xyz(i1+1,i2,i3,2))*u(i1+(+1)+off(1),i2+(0)+off(2),
     & i3+(0)+off(3),rc)*u(i1+(+1)+off(1),i2+(0)+off(2),i3+(0)+off(3),
     & cc)
c      linearize terms like drar/xyz(i1,i2-1,i3,2) * u(rc)* u(cc) at the grid point offset by 0, -1, 0
                                   rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,
     & eqn) + (drar/xyz(i1,i2-1,i3,2))*u(i1+(0)+off(1),i2+(-1)+off(2),
     & i3+(0)+off(3),rc)*u(i1+(0)+off(1),i2+(-1)+off(2),i3+(0)+off(3),
     & cc)
c      linearize terms like drar/xyz(i1,i2+1,i3,2) * u(rc)* u(cc) at the grid point offset by 0, +1, 0
                                   rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,
     & eqn) + (drar/xyz(i1,i2+1,i3,2))*u(i1+(0)+off(1),i2+(+1)+off(2),
     & i3+(0)+off(3),rc)*u(i1+(0)+off(1),i2+(+1)+off(2),i3+(0)+off(3),
     & cc)
                           else
c      linearize terms like ((1)/(xyz(i1,i2,i3,2))) * u(rc)* u(cc) at the grid point offset by 0, 0, 0
                                    rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,
     & eqn) + (((1)/(xyz(i1,i2,i3,2))))*u(i1+(0)+off(1),i2+(0)+off(2),
     & i3+(0)+off(3),rc)*u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)+off(3),
     & cc)
                           endif
                  else
                  drar = dri2(1)*jaci*jac(i1-1,i2,i3)*
     & (xyz(i1,i2,i3,2)-xyz(i1-1,i2,i3,2))*rx(i1-1,i2,i3,1,d)/
     &                           xyz(i1,i2,i3,2)
c      linearize terms like drar * u(rc)* u(cc) at the grid point offset by -1, 0, 0
                        rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (drar)*
     & u(i1+(-1)+off(1),i2+(0)+off(2),i3+(0)+off(3),rc)*u(i1+(-1)+off(
     & 1),i2+(0)+off(2),i3+(0)+off(3),cc)

                  drar = dri2(1)*jaci*jac(i1+1,i2,i3)*
     & (xyz(i1+1,i2,i3,2)-xyz(i1,i2,i3,2))*rx(i1+1,i2,i3,1,d)/
     &                             xyz(i1,i2,i3,2)
c      linearize terms like drar * u(rc)* u(cc) at the grid point offset by +1, 0, 0
                        rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (drar)*
     & u(i1+(+1)+off(1),i2+(0)+off(2),i3+(0)+off(3),rc)*u(i1+(+1)+off(
     & 1),i2+(0)+off(2),i3+(0)+off(3),cc)

                  drar = dri2(2)*jaci*jac(i1,i2-1,i3)*
     & (xyz(i1,i2,i3,2)-xyz(i1,i2-1,i3,2))*rx(i1,i2-1,i3,2,d)/
     &                           xyz(i1,i2,i3,2)
c      linearize terms like drar * u(rc)* u(cc) at the grid point offset by 0, -1, 0
                        rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (drar)*
     & u(i1+(0)+off(1),i2+(-1)+off(2),i3+(0)+off(3),rc)*u(i1+(0)+off(
     & 1),i2+(-1)+off(2),i3+(0)+off(3),cc)

                  drar = dri2(2)*jaci*jac(i1,i2+1,i3)*
     & (xyz(i1,i2+1,i3,2)-xyz(i1,i2,i3,2))*rx(i1,i2+1,i3,2,d)/
     &                           xyz(i1,i2,i3,2)
c      linearize terms like drar * u(rc)* u(cc) at the grid point offset by 0, +1, 0
                        rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (drar)*
     & u(i1+(0)+off(1),i2+(+1)+off(2),i3+(0)+off(3),rc)*u(i1+(0)+off(
     & 1),i2+(+1)+off(2),i3+(0)+off(3),cc)

                  endif !non-macro code
c                  if ( .false. ) then
                  if ( usestrik .and. .not. oldstrik ) then
                     drar = dri(1)*jaci*jac(i1+1,i2,i3)*
     & (xyz(i1+1,i2,i3,2)-xyz(i1,i2,i3,2))*rx(i1+1,i2,i3,1,d)/
     &                    xyz(i1,i2,i3,2)
c      linearize terms like -alpha*drar * u(rc)* u(cc) at the grid point offset by +1, 0, 0
                           rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (-
     & alpha*drar)*u(i1+(+1)+off(1),i2+(0)+off(2),i3+(0)+off(3),rc)*u(
     & i1+(+1)+off(1),i2+(0)+off(2),i3+(0)+off(3),cc)
                     drar = dri(1)*jaci*jac(i1-1,i2,i3)*
     & (xyz(i1-1,i2,i3,2)-xyz(i1,i2,i3,2))*rx(i1-1,i2,i3,1,d)/
     &                    xyz(i1,i2,i3,2)
c      linearize terms like -3d0*alpha*drar * u(rc)* u(cc) at the grid point offset by -1, 0, 0
                           rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (-
     & 3d0*alpha*drar)*u(i1+(-1)+off(1),i2+(0)+off(2),i3+(0)+off(3),
     & rc)*u(i1+(-1)+off(1),i2+(0)+off(2),i3+(0)+off(3),cc)
                     drar = dri(1)*jaci*jac(i1-2,i2,i3)*
     & (xyz(i1-2,i2,i3,2)-xyz(i1,i2,i3,2))*rx(i1-2,i2,i3,1,d)/
     &                    xyz(i1,i2,i3,2)
c      linearize terms like alpha*drar * u(rc)* u(cc) at the grid point offset by -2, 0, 0
                           rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (
     & alpha*drar)*u(i1+(-2)+off(1),i2+(0)+off(2),i3+(0)+off(3),rc)*u(
     & i1+(-2)+off(1),i2+(0)+off(2),i3+(0)+off(3),cc)

                     drar = dri(2)*jaci*jac(i1,i2+1,i3)*
     & (xyz(i1,i2+1,i3,2)-xyz(i1,i2,i3,2))*rx(i1,i2+1,i3,2,d)/
     &                    xyz(i1,i2,i3,2)
c      linearize terms like -alpha*drar * u(rc)* u(cc) at the grid point offset by 0, +1, 0
                           rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (-
     & alpha*drar)*u(i1+(0)+off(1),i2+(+1)+off(2),i3+(0)+off(3),rc)*u(
     & i1+(0)+off(1),i2+(+1)+off(2),i3+(0)+off(3),cc)
                     drar = dri(2)*jaci*jac(i1,i2-1,i3)*
     & (xyz(i1,i2-1,i3,2)-xyz(i1,i2,i3,2))*rx(i1,i2-1,i3,2,d)/
     &                    xyz(i1,i2,i3,2)
c      linearize terms like -3d0*alpha*drar * u(rc)* u(cc) at the grid point offset by 0, -1, 0
                           rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (-
     & 3d0*alpha*drar)*u(i1+(0)+off(1),i2+(-1)+off(2),i3+(0)+off(3),
     & rc)*u(i1+(0)+off(1),i2+(-1)+off(2),i3+(0)+off(3),cc)
                     drar = dri(2)*jaci*jac(i1,i2-2,i3)*
     & (xyz(i1,i2-2,i3,2)-xyz(i1,i2,i3,2))*rx(i1,i2-2,i3,2,d)/
     &                    xyz(i1,i2,i3,2)
c      linearize terms like alpha*drar * u(rc)* u(cc) at the grid point offset by 0, -2, 0
                           rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (
     & alpha*drar)*u(i1+(0)+off(1),i2+(-2)+off(2),i3+(0)+off(3),rc)*u(
     & i1+(0)+off(1),i2+(-2)+off(2),i3+(0)+off(3),cc)
                  endif
c                  endif
               end do ! ndim

            end if

c
c     MOMENTUM EQUATION
c
            if ( .true. .or.
     &           (i1.gt.igi(1,1) .and. i1.lt.igi(2,1) .and.
     &           i2.gt.igi(1,2) .and. i2.lt.igi(2,2)) ) then
c
c     CONVECTIVE DERIVATIVES
c
c          += u[cc] ( r_{cc-uc+1} u[eqn]_r + s_{cc-uc+1} u[eqn]_s )
c       
c          all the linearizations will contribute terms to the RHS
            do eqn=uc,tc

               do cc=uc, uc+ndim-1
                  d = cc-uc+1
                  rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) +
     &                 u(i1,i2,i3,cc)*ux(i1,i2,i3,eqn,d)
               end do
            end do

c
c     PRESSURE GRADIENT
c
            if ( .false. ) then ! corresponds to .false. in coeff construction
c     all the terms are either linear or triple products so there are no contributions to
c       the right hand side
            else

c            gm2*u[tc]^{n}*log(u[rc]^{n})_c

c                  rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) +
c     &               gm2*u(i1,i2,i3,tc)*(rx(i1,i2,i3,1,cc)*dri2(1)*lur +
c     &                                   rx(i1,i2,i3,2,cc)*dri2(2)*lus )
c               end do

            endif

c
c     MOMENTUM EQUATION AXISYMMETRIC FORCING TERMS (NON-VISCOUS)
c
            if ( isaxi.gt.0 .and. xyz(i1,i2,i3,2).gt.0d0 ) then
               if ( isswirl.gt.0 ) then
                  if ( avgmomforcing ) then
                           if ( .false. ) then
                           if ( .false. ) then
c         do d=2,2
                                 eqn=vc
                                       if ( .true. ) then
                                       drar = (-1d0)*dri2(1)*jaci*jac(
     & i1+off(1)-1,i2+off(2),i3+off(3))*(xyz(i1+off(1),i2+off(2),i3+
     & off(3),2)-xyz(i1+off(1)-1,i2+off(2),i3+off(3),2))*rx(i1+off(1)-
     & 1,i2+off(2),i3+off(3),1,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
c      linearize terms like drar * u(wc)* u(wc) at the grid point offset by -1, 0, 0
                                             rhs(i1,i2,i3,eqn) = rhs(
     & i1,i2,i3,eqn) + (drar)*u(i1+(-1)+off(1),i2+(0)+off(2),i3+(0)+
     & off(3),wc)*u(i1+(-1)+off(1),i2+(0)+off(2),i3+(0)+off(3),wc)
                                       drar = (-1d0)*dri2(1)*jaci*jac(
     & i1+off(1)+1,i2+off(2),i3+off(3))*(xyz(i1+off(1)+1,i2+off(2),i3+
     & off(3),2)-xyz(i1+off(1),i2+off(2),i3+off(3),2))*rx(i1+off(1)+1,
     & i2+off(2),i3+off(3),1,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
c      linearize terms like drar * u(wc)* u(wc) at the grid point offset by +1, 0, 0
                                             rhs(i1,i2,i3,eqn) = rhs(
     & i1,i2,i3,eqn) + (drar)*u(i1+(+1)+off(1),i2+(0)+off(2),i3+(0)+
     & off(3),wc)*u(i1+(+1)+off(1),i2+(0)+off(2),i3+(0)+off(3),wc)
                                       drar = (-1d0)*dri2(2)*jaci*jac(
     & i1+off(1),i2+off(2)-1,i3+off(3))*(xyz(i1+off(1),i2+off(2),i3+
     & off(3),2)-xyz(i1+off(1),i2+off(2)-1,i3+off(3),2))*rx(i1+off(1),
     & i2+off(2)-1,i3+off(3),2,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
c      linearize terms like drar * u(wc)* u(wc) at the grid point offset by 0, -1, 0
                                             rhs(i1,i2,i3,eqn) = rhs(
     & i1,i2,i3,eqn) + (drar)*u(i1+(0)+off(1),i2+(-1)+off(2),i3+(0)+
     & off(3),wc)*u(i1+(0)+off(1),i2+(-1)+off(2),i3+(0)+off(3),wc)
                                       drar = (-1d0)*dri2(2)*jaci*jac(
     & i1+off(1),i2+off(2)+1,i3+off(3))*(xyz(i1+off(1),i2+off(2)+1,i3+
     & off(3),2)-xyz(i1+off(1),i2+off(2),i3+off(3),2))*rx(i1+off(1),
     & i2+off(2)+1,i3+off(3),2,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
c      linearize terms like drar * u(wc)* u(wc) at the grid point offset by 0, +1, 0
                                             rhs(i1,i2,i3,eqn) = rhs(
     & i1,i2,i3,eqn) + (drar)*u(i1+(0)+off(1),i2+(+1)+off(2),i3+(0)+
     & off(3),wc)*u(i1+(0)+off(1),i2+(+1)+off(2),i3+(0)+off(3),wc)
                                       else if ( .false. ) then
                                          drar = (-1d0)*.25d0
c      linearize terms like drar/xyz(i1-1,i2,i3,2) * u(wc)* u(wc) at the grid point offset by -1, 0, 0
                                               rhs(i1,i2,i3,eqn) = rhs(
     & i1,i2,i3,eqn) + (drar/xyz(i1-1,i2,i3,2))*u(i1+(-1)+off(1),i2+(
     & 0)+off(2),i3+(0)+off(3),wc)*u(i1+(-1)+off(1),i2+(0)+off(2),i3+(
     & 0)+off(3),wc)
c      linearize terms like drar/xyz(i1+1,i2,i3,2) * u(wc)* u(wc) at the grid point offset by +1, 0, 0
                                               rhs(i1,i2,i3,eqn) = rhs(
     & i1,i2,i3,eqn) + (drar/xyz(i1+1,i2,i3,2))*u(i1+(+1)+off(1),i2+(
     & 0)+off(2),i3+(0)+off(3),wc)*u(i1+(+1)+off(1),i2+(0)+off(2),i3+(
     & 0)+off(3),wc)
c      linearize terms like drar/xyz(i1,i2-1,i3,2) * u(wc)* u(wc) at the grid point offset by 0, -1, 0
                                               rhs(i1,i2,i3,eqn) = rhs(
     & i1,i2,i3,eqn) + (drar/xyz(i1,i2-1,i3,2))*u(i1+(0)+off(1),i2+(-
     & 1)+off(2),i3+(0)+off(3),wc)*u(i1+(0)+off(1),i2+(-1)+off(2),i3+(
     & 0)+off(3),wc)
c      linearize terms like drar/xyz(i1,i2+1,i3,2) * u(wc)* u(wc) at the grid point offset by 0, +1, 0
                                               rhs(i1,i2,i3,eqn) = rhs(
     & i1,i2,i3,eqn) + (drar/xyz(i1,i2+1,i3,2))*u(i1+(0)+off(1),i2+(+
     & 1)+off(2),i3+(0)+off(3),wc)*u(i1+(0)+off(1),i2+(+1)+off(2),i3+(
     & 0)+off(3),wc)
                                       else
c      linearize terms like ((-1d0)/(xyz(i1,i2,i3,2))) * u(wc)* u(wc) at the grid point offset by 0, 0, 0
                                                rhs(i1,i2,i3,eqn) = 
     & rhs(i1,i2,i3,eqn) + (((-1d0)/(xyz(i1,i2,i3,2))))*u(i1+(0)+off(
     & 1),i2+(0)+off(2),i3+(0)+off(3),wc)*u(i1+(0)+off(1),i2+(0)+off(
     & 2),i3+(0)+off(3),wc)
                                       endif
                                 eqn = wc
                                       if ( .true. ) then
                                       drar = (1d0)*dri2(1)*jaci*jac(
     & i1+off(1)-1,i2+off(2),i3+off(3))*(xyz(i1+off(1),i2+off(2),i3+
     & off(3),2)-xyz(i1+off(1)-1,i2+off(2),i3+off(3),2))*rx(i1+off(1)-
     & 1,i2+off(2),i3+off(3),1,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
c      linearize terms like drar * u(vc)* u(wc) at the grid point offset by -1, 0, 0
                                             rhs(i1,i2,i3,eqn) = rhs(
     & i1,i2,i3,eqn) + (drar)*u(i1+(-1)+off(1),i2+(0)+off(2),i3+(0)+
     & off(3),vc)*u(i1+(-1)+off(1),i2+(0)+off(2),i3+(0)+off(3),wc)
                                       drar = (1d0)*dri2(1)*jaci*jac(
     & i1+off(1)+1,i2+off(2),i3+off(3))*(xyz(i1+off(1)+1,i2+off(2),i3+
     & off(3),2)-xyz(i1+off(1),i2+off(2),i3+off(3),2))*rx(i1+off(1)+1,
     & i2+off(2),i3+off(3),1,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
c      linearize terms like drar * u(vc)* u(wc) at the grid point offset by +1, 0, 0
                                             rhs(i1,i2,i3,eqn) = rhs(
     & i1,i2,i3,eqn) + (drar)*u(i1+(+1)+off(1),i2+(0)+off(2),i3+(0)+
     & off(3),vc)*u(i1+(+1)+off(1),i2+(0)+off(2),i3+(0)+off(3),wc)
                                       drar = (1d0)*dri2(2)*jaci*jac(
     & i1+off(1),i2+off(2)-1,i3+off(3))*(xyz(i1+off(1),i2+off(2),i3+
     & off(3),2)-xyz(i1+off(1),i2+off(2)-1,i3+off(3),2))*rx(i1+off(1),
     & i2+off(2)-1,i3+off(3),2,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
c      linearize terms like drar * u(vc)* u(wc) at the grid point offset by 0, -1, 0
                                             rhs(i1,i2,i3,eqn) = rhs(
     & i1,i2,i3,eqn) + (drar)*u(i1+(0)+off(1),i2+(-1)+off(2),i3+(0)+
     & off(3),vc)*u(i1+(0)+off(1),i2+(-1)+off(2),i3+(0)+off(3),wc)
                                       drar = (1d0)*dri2(2)*jaci*jac(
     & i1+off(1),i2+off(2)+1,i3+off(3))*(xyz(i1+off(1),i2+off(2)+1,i3+
     & off(3),2)-xyz(i1+off(1),i2+off(2),i3+off(3),2))*rx(i1+off(1),
     & i2+off(2)+1,i3+off(3),2,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
c      linearize terms like drar * u(vc)* u(wc) at the grid point offset by 0, +1, 0
                                             rhs(i1,i2,i3,eqn) = rhs(
     & i1,i2,i3,eqn) + (drar)*u(i1+(0)+off(1),i2+(+1)+off(2),i3+(0)+
     & off(3),vc)*u(i1+(0)+off(1),i2+(+1)+off(2),i3+(0)+off(3),wc)
                                       else if ( .false. ) then
                                          drar = (1d0)*.25d0
c      linearize terms like drar/xyz(i1-1,i2,i3,2) * u(vc)* u(wc) at the grid point offset by -1, 0, 0
                                               rhs(i1,i2,i3,eqn) = rhs(
     & i1,i2,i3,eqn) + (drar/xyz(i1-1,i2,i3,2))*u(i1+(-1)+off(1),i2+(
     & 0)+off(2),i3+(0)+off(3),vc)*u(i1+(-1)+off(1),i2+(0)+off(2),i3+(
     & 0)+off(3),wc)
c      linearize terms like drar/xyz(i1+1,i2,i3,2) * u(vc)* u(wc) at the grid point offset by +1, 0, 0
                                               rhs(i1,i2,i3,eqn) = rhs(
     & i1,i2,i3,eqn) + (drar/xyz(i1+1,i2,i3,2))*u(i1+(+1)+off(1),i2+(
     & 0)+off(2),i3+(0)+off(3),vc)*u(i1+(+1)+off(1),i2+(0)+off(2),i3+(
     & 0)+off(3),wc)
c      linearize terms like drar/xyz(i1,i2-1,i3,2) * u(vc)* u(wc) at the grid point offset by 0, -1, 0
                                               rhs(i1,i2,i3,eqn) = rhs(
     & i1,i2,i3,eqn) + (drar/xyz(i1,i2-1,i3,2))*u(i1+(0)+off(1),i2+(-
     & 1)+off(2),i3+(0)+off(3),vc)*u(i1+(0)+off(1),i2+(-1)+off(2),i3+(
     & 0)+off(3),wc)
c      linearize terms like drar/xyz(i1,i2+1,i3,2) * u(vc)* u(wc) at the grid point offset by 0, +1, 0
                                               rhs(i1,i2,i3,eqn) = rhs(
     & i1,i2,i3,eqn) + (drar/xyz(i1,i2+1,i3,2))*u(i1+(0)+off(1),i2+(+
     & 1)+off(2),i3+(0)+off(3),vc)*u(i1+(0)+off(1),i2+(+1)+off(2),i3+(
     & 0)+off(3),wc)
                                       else
c      linearize terms like ((1d0)/(xyz(i1,i2,i3,2))) * u(vc)* u(wc) at the grid point offset by 0, 0, 0
                                                rhs(i1,i2,i3,eqn) = 
     & rhs(i1,i2,i3,eqn) + (((1d0)/(xyz(i1,i2,i3,2))))*u(i1+(0)+off(1)
     & ,i2+(0)+off(2),i3+(0)+off(3),vc)*u(i1+(0)+off(1),i2+(0)+off(2),
     & i3+(0)+off(3),wc)
                                       endif
c         enddo
                           else if ( .false. ) then
                              eqn = vc
c     -u[wc]^2/r 
c     will contribute to RHS
c      linearize terms like -1d0/xyz(i1,i2,i3,2) * u(wc)* u(wc) at the grid point offset by 0, 0, 0
                                    rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,
     & eqn) + (-1d0/xyz(i1,i2,i3,2))*u(i1+(0)+off(1),i2+(0)+off(2),i3+
     & (0)+off(3),wc)*u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)+off(3),wc)
                              eqn = wc
c     +u[vc] u[wc]/r
c     will contribute to RHS
c      linearize terms like 1d0/xyz(i1,i2,i3,2) * u(wc)* u(vc) at the grid point offset by 0, 0, 0
                                    rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,
     & eqn) + (1d0/xyz(i1,i2,i3,2))*u(i1+(0)+off(1),i2+(0)+off(2),i3+(
     & 0)+off(3),wc)*u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)+off(3),vc)
                           endif
                           if ( usestrik.and. .not. oldstrik .and. 
     & .false.) then
                              do d=2,2
                                 eqn = vc
                                 drar = -dri(1)*(xyz(i1-1,i2,i3,2)-xyz(
     & i1,i2,i3,2))*rx(i1,i2,i3,1,d)/xyz(i1,i2,i3,2)
c      linearize terms like alpha*drar * u(wc)* u(wc) at the grid point offset by -1, 0, 0
                                       rhs(i1,i2,i3,eqn) = rhs(i1,i2,
     & i3,eqn) + (alpha*drar)*u(i1+(-1)+off(1),i2+(0)+off(2),i3+(0)+
     & off(3),wc)*u(i1+(-1)+off(1),i2+(0)+off(2),i3+(0)+off(3),wc)
                                 drar = -dri(1)*(xyz(i1+1,i2,i3,2)-xyz(
     & i1,i2,i3,2))*rx(i1,i2,i3,1,d)/xyz(i1,i2,i3,2)
c      linearize terms like 3d0*alpha*drar * u(wc)* u(wc) at the grid point offset by +1, 0, 0
                                       rhs(i1,i2,i3,eqn) = rhs(i1,i2,
     & i3,eqn) + (3d0*alpha*drar)*u(i1+(+1)+off(1),i2+(0)+off(2),i3+(
     & 0)+off(3),wc)*u(i1+(+1)+off(1),i2+(0)+off(2),i3+(0)+off(3),wc)
                                 drar = -dri(1)*(xyz(i1+2,i2,i3,2)-xyz(
     & i1,i2,i3,2))*rx(i1,i2,i3,1,d)/xyz(i1,i2,i3,2)
c      linearize terms like alpha*drar * u(wc)* u(wc) at the grid point offset by +2, 0, 0
                                       rhs(i1,i2,i3,eqn) = rhs(i1,i2,
     & i3,eqn) + (alpha*drar)*u(i1+(+2)+off(1),i2+(0)+off(2),i3+(0)+
     & off(3),wc)*u(i1+(+2)+off(1),i2+(0)+off(2),i3+(0)+off(3),wc)
                                 drar = -dri(2)*(xyz(i1,i2-1,i3,2)-xyz(
     & i1,i2,i3,2))*rx(i1,i2,i3,2,d)/xyz(i1,i2,i3,2)
c      linearize terms like alpha*drar * u(wc)* u(wc) at the grid point offset by 0, -1, 0
                                       rhs(i1,i2,i3,eqn) = rhs(i1,i2,
     & i3,eqn) + (alpha*drar)*u(i1+(0)+off(1),i2+(-1)+off(2),i3+(0)+
     & off(3),wc)*u(i1+(0)+off(1),i2+(-1)+off(2),i3+(0)+off(3),wc)
                                 drar = -dri(2)*(xyz(i1,i2+1,i3,2)-xyz(
     & i1,i2,i3,2))*rx(i1,i2,i3,2,d)/xyz(i1,i2,i3,2)
c      linearize terms like 3d0*alpha*drar * u(wc)* u(wc) at the grid point offset by 0, +1, 0
                                       rhs(i1,i2,i3,eqn) = rhs(i1,i2,
     & i3,eqn) + (3d0*alpha*drar)*u(i1+(0)+off(1),i2+(+1)+off(2),i3+(
     & 0)+off(3),wc)*u(i1+(0)+off(1),i2+(+1)+off(2),i3+(0)+off(3),wc)
                                 drar = -dri(2)*(xyz(i1,i2+2,i3,2)-xyz(
     & i1,i2,i3,2))*rx(i1,i2,i3,2,d)/xyz(i1,i2,i3,2)
c      linearize terms like alpha*drar * u(wc)* u(wc) at the grid point offset by 0, +2, 0
                                       rhs(i1,i2,i3,eqn) = rhs(i1,i2,
     & i3,eqn) + (alpha*drar)*u(i1+(0)+off(1),i2+(+2)+off(2),i3+(0)+
     & off(3),wc)*u(i1+(0)+off(1),i2+(+2)+off(2),i3+(0)+off(3),wc)
                                 eqn = wc
                                 drar = dri(1)*(xyz(i1-1,i2,i3,2)-xyz(
     & i1,i2,i3,2))*rx(i1,i2,i3,1,d)/xyz(i1,i2,i3,2)
c      linearize terms like alpha*drar * u(wc)* u(vc) at the grid point offset by -1, 0, 0
                                       rhs(i1,i2,i3,eqn) = rhs(i1,i2,
     & i3,eqn) + (alpha*drar)*u(i1+(-1)+off(1),i2+(0)+off(2),i3+(0)+
     & off(3),wc)*u(i1+(-1)+off(1),i2+(0)+off(2),i3+(0)+off(3),vc)
                                 drar = dri(1)*(xyz(i1+1,i2,i3,2)-xyz(
     & i1,i2,i3,2))*rx(i1,i2,i3,1,d)/xyz(i1,i2,i3,2)
c      linearize terms like 3d0*alpha*drar * u(wc)* u(vc) at the grid point offset by +1, 0, 0
                                       rhs(i1,i2,i3,eqn) = rhs(i1,i2,
     & i3,eqn) + (3d0*alpha*drar)*u(i1+(+1)+off(1),i2+(0)+off(2),i3+(
     & 0)+off(3),wc)*u(i1+(+1)+off(1),i2+(0)+off(2),i3+(0)+off(3),vc)
                                 drar = dri(1)*(xyz(i1+2,i2,i3,2)-xyz(
     & i1,i2,i3,2))*rx(i1,i2,i3,1,d)/xyz(i1,i2,i3,2)
c      linearize terms like alpha*drar * u(wc)* u(vc) at the grid point offset by +2, 0, 0
                                       rhs(i1,i2,i3,eqn) = rhs(i1,i2,
     & i3,eqn) + (alpha*drar)*u(i1+(+2)+off(1),i2+(0)+off(2),i3+(0)+
     & off(3),wc)*u(i1+(+2)+off(1),i2+(0)+off(2),i3+(0)+off(3),vc)
                                 drar = dri(2)*(xyz(i1,i2-1,i3,2)-xyz(
     & i1,i2,i3,2))*rx(i1,i2,i3,2,d)/xyz(i1,i2,i3,2)
c      linearize terms like alpha*drar * u(wc)* u(vc) at the grid point offset by 0, -1, 0
                                       rhs(i1,i2,i3,eqn) = rhs(i1,i2,
     & i3,eqn) + (alpha*drar)*u(i1+(0)+off(1),i2+(-1)+off(2),i3+(0)+
     & off(3),wc)*u(i1+(0)+off(1),i2+(-1)+off(2),i3+(0)+off(3),vc)
                                 drar = dri(2)*(xyz(i1,i2+1,i3,2)-xyz(
     & i1,i2,i3,2))*rx(i1,i2,i3,2,d)/xyz(i1,i2,i3,2)
c      linearize terms like 3d0*alpha*drar * u(wc)* u(vc) at the grid point offset by 0, +1, 0
                                       rhs(i1,i2,i3,eqn) = rhs(i1,i2,
     & i3,eqn) + (3d0*alpha*drar)*u(i1+(0)+off(1),i2+(+1)+off(2),i3+(
     & 0)+off(3),wc)*u(i1+(0)+off(1),i2+(+1)+off(2),i3+(0)+off(3),vc)
                                 drar = dri(2)*(xyz(i1,i2+2,i3,2)-xyz(
     & i1,i2,i3,2))*rx(i1,i2,i3,2,d)/xyz(i1,i2,i3,2)
c      linearize terms like alpha*drar * u(wc)* u(vc) at the grid point offset by 0, +2, 0
                                       rhs(i1,i2,i3,eqn) = rhs(i1,i2,
     & i3,eqn) + (alpha*drar)*u(i1+(0)+off(1),i2+(+2)+off(2),i3+(0)+
     & off(3),wc)*u(i1+(0)+off(1),i2+(+2)+off(2),i3+(0)+off(3),vc)
                              enddo
                           endif
                           endif
                  else
                     rhs(i1,i2,i3,vc) = rhs(i1,i2,i3,vc) -
     &                    u(i1,i2,i3,wc)**2/xyz(i1,i2,i3,2)
                     rhs(i1,i2,i3,wc) = rhs(i1,i2,i3,wc) +
     &                    u(i1,i2,i3,wc)*u(i1,i2,i3,vc)/xyz(i1,i2,i3,2)
                  endif
               endif
            end if
c
c     MOMENTUM EQUATION VISCOUS TERMS
c
c           u-momentum equation, the terms are
c           (4 u[uc]_xx/3 + u[uc]_yy + u[vc]_xy/3 + axifac*(u[uc]_y/y + u[vc]_x/y/3))/u[rc]/ren
            rhs(i1,i2,i3,uc) = rhs(i1,i2,i3,uc) +
     &      oren*( f43*uxx(i1,i2,i3,uc,1,1) + uxx(i1,i2,i3,uc,2,2) +
     &             f13*uxx(i1,i2,i3,vc,1,2) )/u(i1,i2,i3,rc)

            if ( isaxi.gt.0 .and. xyz(i1,i2,i3,2).gt.0d0 ) then
               rhs(i1,i2,i3,uc) = rhs(i1,i2,i3,uc) +
     &  oren*(ux(i1,i2,i3,uc,2) + f13*ux(i1,i2,i3,vc,1))/
     &               (u(i1,i2,i3,rc)*xyz(i1,i2,i3,2))
            end if

c           v-momentum equation, the terms are
c           (4 u[vc]_yy/3 + u[vc]_xx + u[uc]_xy/3 + axifac*(4/3)*(u[vc]_y/y - u[vc]/y^2))/u[rc]/ren
            rhs(i1,i2,i3,vc) = rhs(i1,i2,i3,vc) +
     &      oren*( f43*uxx(i1,i2,i3,vc,2,2) + uxx(i1,i2,i3,vc,1,1) +
     &             f13*uxx(i1,i2,i3,uc,2,1) )/u(i1,i2,i3,rc)

            if ( isaxi.gt.0 .and. xyz(i1,i2,i3,2).gt.0d0 ) then
               rhs(i1,i2,i3,vc) = rhs(i1,i2,i3,vc) +
     & oren*(f43*(ux(i1,i2,i3,vc,2)-u(i1,i2,i3,vc)/xyz(i1,i2,i3,2))/
     &                          (u(i1,i2,i3,rc)*xyz(i1,i2,i3,2)))

            endif

            if (  isswirl.gt.0 ) then
c            w-momentum equation, the terms are
c            (u[wc]_xx + u[wc]_yy + axifac*(u[wc]_y/y - u[wc]/y^2))/u[rc]/ren
               if ( .true. ) then
               rhs(i1,i2,i3,wc) = rhs(i1,i2,i3,wc) +
     &   oren*(uxx(i1,i2,i3,wc,1,1)+uxx(i1,i2,i3,wc,2,2) +
     &              (ux(i1,i2,i3,wc,2)-u(i1,i2,i3,wc)/xyz(i1,i2,i3,2))/
     &                 xyz(i1,i2,i3,2))/u(i1,i2,i3,rc)
               else

               rhs(i1,i2,i3,wc) = rhs(i1,i2,i3,wc) +
     & oren*(uxx(i1,i2,i3,wc,1,1)+uxx(i1,i2,i3,wc,2,2))/u(i1,i2,i3,rc)

               rhs(i1,i2,i3,wc) = rhs(i1,i2,i3,wc) +
     &   oren*(
     &              (ux(i1,i2,i3,wc,2)-u(i1,i2,i3,wc)/xyz(i1,i2,i3,2))/
     &                 xyz(i1,i2,i3,2))/u(i1,i2,i3,rc)

               endif
            endif

c
c     ENERGY EQUATION, PRESSURE WORK 
c
c     gm1 * u[tc] ( u[uc]_x + u[vc]_y + u[vc]/y )
c     each term will contribute to the RHS
c     we can do the Cartesian part generically:
            do cc=uc, uc+ndim-1
               d = cc-uc+1
               if ( .true. ) then
               rhs(i1,i2,i3,tc) = rhs(i1,i2,i3,tc) +
     &                 gm1*u(i1,i2,i3,tc)*ux(i1,i2,i3,cc,d)
               else
               rhs(i1,i2,i3,tc) = rhs(i1,i2,i3,tc) +
     &                 gm1*u(i1,i2,i3,tc)*ux4p(i1,i2,i3,cc,d)

               endif
            end do

            if ( isaxi.gt.0 .and. xyz(i1,i2,i3,2).gt.0d0 ) then
               if ( .true. ) then
                  eqn = tc
               d=2
                     drar = (gm1)*dri2(1)*jaci*jac(i1+off(1)-1,i2+off(
     & 2),i3+off(3))*(xyz(i1+off(1),i2+off(2),i3+off(3),2)-xyz(i1+off(
     & 1)-1,i2+off(2),i3+off(3),2))*rx(i1+off(1)-1,i2+off(2),i3+off(3)
     & ,1,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
c      linearize terms like drar * u(tc)* u(vc) at the grid point offset by O1, O2, O3
                           rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (
     & drar)*u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)+off(3),tc)*u(i1+(-1)
     & +off(1),i2+(0)+off(2),i3+(0)+off(3),vc)
                     drar = (gm1)*dri2(1)*jaci*jac(i1+off(1)+1,i2+off(
     & 2),i3+off(3))*(xyz(i1+off(1)+1,i2+off(2),i3+off(3),2)-xyz(i1+
     & off(1),i2+off(2),i3+off(3),2))*rx(i1+off(1)+1,i2+off(2),i3+off(
     & 3),1,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
c      linearize terms like drar * u(tc)* u(vc) at the grid point offset by O1, O2, O3
                           rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (
     & drar)*u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)+off(3),tc)*u(i1+(+1)
     & +off(1),i2+(0)+off(2),i3+(0)+off(3),vc)
                     drar = (gm1)*dri2(2)*jaci*jac(i1+off(1),i2+off(2)-
     & 1,i3+off(3))*(xyz(i1+off(1),i2+off(2),i3+off(3),2)-xyz(i1+off(
     & 1),i2+off(2)-1,i3+off(3),2))*rx(i1+off(1),i2+off(2)-1,i3+off(3)
     & ,2,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
c      linearize terms like drar * u(tc)* u(vc) at the grid point offset by O1, O2, O3
                           rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (
     & drar)*u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)+off(3),tc)*u(i1+(0)+
     & off(1),i2+(-1)+off(2),i3+(0)+off(3),vc)
                     drar = (gm1)*dri2(2)*jaci*jac(i1+off(1),i2+off(2)+
     & 1,i3+off(3))*(xyz(i1+off(1),i2+off(2)+1,i3+off(3),2)-xyz(i1+
     & off(1),i2+off(2),i3+off(3),2))*rx(i1+off(1),i2+off(2)+1,i3+off(
     & 3),2,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
c      linearize terms like drar * u(tc)* u(vc) at the grid point offset by O1, O2, O3
                           rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (
     & drar)*u(i1+(0)+off(1),i2+(0)+off(2),i3+(0)+off(3),tc)*u(i1+(0)+
     & off(1),i2+(+1)+off(2),i3+(0)+off(3),vc)
               else
               rhs(i1,i2,i3,tc) = rhs(i1,i2,i3,tc) +
     &              gm1*u(i1,i2,i3,tc)*u(i1,i2,i3,vc)/xyz(i1,i2,i3,2)
               endif
            endif

c     
c     ENERGY EQUATION, THERMAL CONDUCTIVITY
c
c     +gam * ( u[tc]_{xx} + u[tc]_{yy} + u[tc]_y/y )/(u[rc] Re Pr)
c     do the Cartesian terms generically
c     each term will contribute to the RHS
            do d=1,ndim
               rhs(i1,i2,i3,tc) = rhs(i1,i2,i3,tc) +
     &            gam*oren*oprn*(uxx(i1,i2,i3,tc,d,d))/u(i1,i2,i3,rc)
            end do

            if ( isaxi.gt.0 .and. xyz(i1,i2,i3,2).gt.0d0 ) then
               rhs(i1,i2,i3,tc) = rhs(i1,i2,i3,tc) +
     &            gam*oren*oprn*ux(i1,i2,i3,tc,2)/
     &               (xyz(i1,i2,i3,2)*u(i1,i2,i3,rc))
            endif
c
c     ENERGY EQUATION, VISCOUS DISSIPATION
c
c     Since the viscous dissipation terms consist of products of three
c      solution variables, no contribution to the RHS will be made.

            if ( .false. ) then
c       add some artificial viscosity
              rhs(i1,i2,i3,rc) = rhs(i1,i2,i3,rc) +
     &        av4*(
     &        u(i1-2,i2,i3,rc) + u(i1+2,i2,i3,rc) -
     & 4d0*(u(i1-1,i2,i3,rc)+u(i1+1,i2,i3,rc)+u(i1+2,i2,i3,rc)) +
     &        6d0*u(i1,i2,i3,rc)+
     &        u(i1,i2-2,i3,rc) + u(i1,i2+2,i3,rc) -
     & 4d0*(u(i1,i2-1,i3,rc)+u(i1,i2+1,i3,rc)+u(i1,i2+2,i3,rc)) +
     &        6d0*u(i1,i2,i3,rc))
           endif
c            do cc=uc,tc
c               rhs(i1,i2,i3,cc) = rhs(i1,i2,i3,cc)*u(i1,i2,i3,rc)
c            end do

         end if ! is interior
         end if ! if not masked

      enddo ! i1
      enddo ! i2
      enddo ! i3

      return
      end
