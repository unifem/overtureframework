!
!  This function is used to compute the "exact" solution to an FSI problem consisting
! of a elastic disk (governed by the SVK model) and a compressible inviscid fluid
!
! Author: Don Schwendeman, 2013
!
      subroutine rotatingDiskInFluid (nSolid,uSolid,nFluid,uFluid,
     *                                tfinal,param,nrwk,rwk)
c
c Compute the exact solution to the FSI problem involving a rotating
c elastic disk (SVK model) and a surrounding inviscid compressible fluid
c
      implicit real*8 (a-h,o-z)
      dimension uSolid(nSolid,0:8),uFluid(nFluid,0:3),param(10),
     *          rwk(nrwk)
c
c number of grid cells in the solid domain
      ns=nSolid-1
c
c split up workspace for solid
      lu=1
      lp=lu+ns+3
      lum=lp+ns+3
      lpm=lum+ns+1
      lup=lpm+ns+1
      lpp=lup+ns+1
      la=lpp+ns+1
      lb=la+ns+2
      lc=lb+ns+2
      ld=lc+ns+1
c
c number of grid cells in the fluid domain
      nf=nFluid-1
c
c split up workspace for fluid
      lrf=ld+ns+1
      lvf=lrf+nf+3
      lpf=lvf+nf+3
      lrfp=lpf+nf+3
      lvfp=lrfp+nf+1
      lpfp=lvfp+nf+1
      lrst=lpfp+nf+1
      lvst=lrst+nf+2
      lpst=lvst+nf+2
      nreq=lpst+nf+2-1
c
      if (nreq.gt.nrwk) then
        write(6,*)'Error (rotatingDiskInFluid) : nrwk too small'
        stop
      end if
c
c get solution
      call getFSISoln (nSolid,uSolid,nFluid,uFluid,tfinal,param,
     *                 ns,rwk(lu),rwk(lp),rwk(lum),rwk(lpm),rwk(lup),
     *                 rwk(lpp),rwk(la),rwk(lb),rwk(lc),rwk(ld),
     *                 nf,rwk(lrf),rwk(lvf),rwk(lpf),rwk(lrfp),
     *                 rwk(lvfp),rwk(lpfp),rwk(lrst),rwk(lvst),
     *                 rwk(lpst))
c
      return
      end
c
c++++++++++++++++++
c
      subroutine getFSISoln (nSolid,uSolid,nFluid,uFluid,tfinal,param,
     *                       ns,u,p,um,pm,up,pp,a,b,c,d,nf,rf,vf,pf,
     *                       rfp,vfp,pfp,rstar,vstar,pstar)
c
      implicit real*8 (a-h,o-z)
      dimension uSolid(nSolid,0:8),uFluid(nFluid,0:3),param(10)
      dimension u(-1:ns+1),p(-1:ns+1),um(0:ns),pm(0:ns),up(0:ns),
     *          pp(0:ns),a(-1:ns),b(-1:ns),c(0:ns),d(0:ns)
      dimension rf(-1:nf+1),vf(-1:nf+1),pf(-1:nf+1),rfp(0:nf),
     *          vfp(0:nf),pfp(0:nf),rstar(0:nf+1),vstar(0:nf+1),
     *          pstar(0:nf+1)
      common / solidParams / r0,alam,amu,akap,j0
      common / fluidParams / r1,r2,gamma,pOffset
c
c      write(6,*)tfinal,nSolid,n
c      write(6,*)param
c      pause
c
c parameters (rhoSolid=1 by assumption)
      r0=param(1)
      r1=param(2)
      omega0=param(3)
      alam=param(4)
      amu=param(5)
      r2=param(6)
      rho0=param(7)
      pOffset=param(8)
      gamma=param(9)

      ! write(*,'("getFSISoln: omega0,r0,r1=",3f5.1)') omega0,r0,r1
c
c solid sound speed
      akap=alam+2.d0*amu
      cp=dsqrt(akap)
c
c fluid sound speed
      cf=dsqrt(gamma*pOffset/rho0)
c
c some grid parameters
      cfl=.4d0
      dx=1.d0/nf
      drs=(r1-r0)/ns
      drf=(r2-r1)*dx
      dt=cfl*min(drs/cp,drf/cf)
      nstep=tfinal/dt+1
      dt=tfinal/nstep
c
c initial conditions
      if (tfinal.lt.1.d-14) then

c set initial conditions in the solid disk and fluid annulus, and then return
        do j=0,ns
          jp1=j+1
          r=r0+j*drs
          uSolid(jp1,0)=r
          uSolid(jp1,1)=0.d0
          uSolid(jp1,2)=0.d0
          uSolid(jp1,3)=0.d0
          uSolid(jp1,4)=omegaRDIF(r,r0,r1,omega0)
          uSolid(jp1,5)=0.d0
          uSolid(jp1,6)=0.d0
          uSolid(jp1,7)=0.d0
          uSolid(jp1,8)=0.d0
        end do
        do j=0,nf
          jp1=j+1
          r=r1+j*drf
          uFluid(jp1,0)=r
          uFluid(jp1,1)=rho0
          uFluid(jp1,2)=0.d0
          uFluid(jp1,3)=pOffset
        end do
        return

      else

c set initial conditions in the solid and fluid for time stepping
        do j=-1,ns+1
          u(j)=0.d0
          p(j)=0.d0
        end do
        do j=0,ns
          r=r0+j*drs
          dp0=dt*omegaRDIF(r,r0,r1,omega0)
          um(j)=.5d0*r*dp0**2
          pm(j)=-dp0
        end do
        do j=-1,nf+1
          rf(j)=rho0
          vf(j)=0.d0
          pf(j)=pOffset
        end do

      end if
c
c check whether r0=0
      if (r0.gt.1.d-14) then
        j0=0
      else
        j0=1
      end if
c
c save interface velocity and stress to a file
      open(10,file='uInterface.m')
      write(10,*)'ui=['
c
      t=0.d0
c
c time stepping
      do m=1,nstep+1
c
c take a time step in the solid domain
        call solidStep (ns,drs,dt,u,p,um,pm,up,pp,a,b,c,d)
c
c set uSolid and uFluid and return
        if (m.gt.nstep) then
          write(10,*)'];'
          close(10)
          call getSoln (ns,drs,dt,u,p,um,pm,up,pp,nf,dx,rf,vf,pf,
     *                  nSolid,uSolid,nFluid,uFluid)
          return
        end if
c
c interface displacement and velocity at time t
        u1=u(ns)
        ut1=(up(ns)-um(ns))/(2.d0*dt)
c
c interface displacement and velocity at time t+dt/2
        u1p=.5d0*(u(ns)+up(ns))
        ut1p=(up(ns)-u(ns))/dt
c
c take a time step in the fluid domain
        call fluidStep (nf,dx,dt,rf,vf,pf,rfp,vfp,pfp,
     *                  rstar,vstar,pstar,u1,u1p,ut1,ut1p)
c
c compute solid interface velocity and stress at time t+dt
        udr=up(ns)/r1
        udrp1=udr+1.d0
        ur=(3.d0*up(ns)-4.d0*up(ns-1)+up(ns-2))/(2.d0*drs)
        urp1=ur+1.d0
        rpr=r1*(3.d0*pp(ns)-4.d0*pp(ns-1)+pp(ns-2))/(2.d0*drs)
        e11=.5d0*((2.d0+ur)*ur+(udrp1*rpr)**2)
        e22=.5d0*(2.d0+udr)*udr
        s11=akap*e11+alam*e22
        p11=urp1*s11
        vels=(3.d0*up(ns)-4.d0*u(ns)+um(ns))/(2.d0*dt)
        sigs=r1*p11/(r1+up(ns))
c
c fluid interface velocity and stress at time t+dt
        velf=vfp(0)
        sigf=pOffset-pfp(0)
c
c impedances (assuming solid density=1)
        zs=cp
        zf=rfp(0)*dsqrt(gamma*pfp(0)/rfp(0))
c
c projection
        veli=(zf*velf+zs*vels       +sigf-sigs )/(zs+zf)
        sigi=(zs*sigf+zf*sigs+zs*zf*(velf-vels))/(zs+zf)
c
c adjust solid displacement and nominal stress at the interface
        up(ns)=(2.d0*dt*veli+4.d0*u(ns)-um(ns))/3.d0
        p11=(r1+up(ns))*sigi/r1
c
c adjust fluid velocity and pressure at the interface
        vfp(0)=veli
        pfp(0)=pOffset-sigi
c
c apply solid bcs
        call solidBCs (ns,drs,u,p,um,pm,up,pp,p11)
c
c apply fluid bcs
        call fluidBCs (nf,rf,vf,pf,rfp,vfp,pfp)
c
        t=t+dt
c        write(10,100)t,veli,sigi
        write(10,100)t,u(ns),veli,sigi
  100   format(4(1x,1pe15.8))
c
      end do
c
      return
      end
c
c+++++++++++++++++++++
c
      double precision function omegaRDIF (r,r0,r1,omega0)
c
      ! implicit real*8 (a-h,o-z)
      implicit none
      real*8 r,r0,r1,omega0
      real*8 radialVariation
c
      ! variable rotation
      ! radialVariation= (4.d0*(r-r0)*(r1-r)/(r0+r1)**2)**2
      ! radialVariation= (4.d0*(r-r0)*(r1-r)/(r0+r1)**2)**3
      radialVariation= (4.d0*(r-r0)*(r1-r)/(r0+r1)**2)**4
      ! radialVariation= (4.d0*(r-r0)*(r1-r)/(r0+r1)**2)**6

      ! radialVariation=(1.-r/r1)**2  ! wdh - for r0=0. 
      ! radialVariation=(1.-r/r1)**4  ! wdh - more derivatives zero at r=1
      omegaRDIF=omega0*radialVariation

       ! constant rotation:
c      omegaRDIF=omega0
c
      return 
      end
c
c++++++++++++++++++
c
      subroutine solidStep (n,dr,dt,u,p,um,pm,up,pp,a,b,c,d)
c
c take a time step in the solid domain.  The code takes (u,p) at time t
c and (um,pm) at time t-dt and computes (up,pp) at time t+dt.
c
      implicit real*8 (a-h,o-z)
      dimension u(-1:n+1),p(-1:n+1),um(0:n),pm(0:n),up(0:n),pp(0:n)
      dimension a(-1:n),b(-1:n),c(0:n),d(0:n)
      common / solidParams / r0,alam,amu,akap,j0
      data tol, itmax / 1.d-12, 10 /
c
c compute a=r*(\bar P_{11}) and b=r*(\bar P_{12}) at cell boundaries
      do j=j0-1,n
        r=r0+(j+.5d0)*dr
        udr=.5d0*(u(j)+u(j+1))/r
        udrp1=udr+1.d0
        ur=(u(j+1)-u(j))/dr
        urp1=ur+1.d0
        rpr=r*(p(j+1)-p(j))/dr
        e11=.5d0*((2.d0+ur)*ur+(udrp1*rpr)**2)
        e12=.5d0*rpr*udrp1**2
        e22=.5d0*(2.d0+udr)*udr
        s11=akap*e11+alam*e22
        s12=2.d0*amu*e12
        a(j)=r*urp1*s11
        b(j)=r*udrp1*(rpr*s11+s12)
      end do
c
c compute (c,d), the right-hand sides of the wave equations for u and p
      do j=j0,n
        r=r0+j*dr
        udr=u(j)/r
        udrp1=udr+1.d0
        ur=(u(j+1)-u(j-1))/(2.d0*dr)
        urp1=ur+1.d0
        rpr=r*(p(j+1)-p(j-1))/(2.d0*dr)
        e11=.5d0*((2.d0+ur)*ur+(udrp1*rpr)**2)
        e12=.5d0*rpr*udrp1**2
        e22=.5d0*(2.d0+udr)*udr
        s11=akap*e11+alam*e22
        s12=2.d0*amu*e12
        s21=s12
        s22=alam*e11+akap*e22
        p11=urp1*s11
        p12=udrp1*(rpr*s11+s12)
        p21=urp1*s21
        p22=udrp1*(rpr*s21+s22)
        c(j)=((a(j)-a(j-1))/dr-rpr*p12-p22)/r
        d(j)=((b(j)-b(j-1))/dr+rpr*p11+p21)/r
      end do
c
c compute RHS/(r+u) of the wave equation for p (at r=0, if needed)
      if (j0.eq.1) then
        ur=(u(1)-u(-1))/(2.d0*dr)
        urp1=ur+1.d0
        prr=(p(1)-2.d0*p(0)+p(-1))/dr**2
        d(0)=4.d0*((alam+akap)*(1.d0+.5d0*ur)*ur
     *                           +2.d0*amu*urp1**2)*prr
      end if
c
c advance u and p
      do j=j0,n
        r=r0+j*dr
        up(j)=2.d0*u(j)-um(j)
        pp(j)=2.d0*p(j)-pm(j)
        do it=1,itmax
          ut=(up(j)-um(j))/(2.d0*dt)
          pt=(pp(j)-pm(j))/(2.d0*dt)
          utt=(up(j)-2.d0*u(j)+um(j))/dt**2
          ptt=(pp(j)-2.d0*p(j)+pm(j))/dt**2
          b1=utt-(r+u(j))*pt**2-c(j)
          b2=(r+u(j))*ptt+2.d0*ut*pt-d(j)
          a11=1.d0/dt**2
          a12=-(r+u(j))*pt/dt
          a21=pt/dt
          a22=(r+u(j))/dt**2+ut/dt
          det=a11*a22-a12*a21
          du=(b1*a22-b2*a12)/det
          dp=(b2*a11-b1*a21)/det
          up(j)=up(j)-du
          pp(j)=pp(j)-dp
          if (max(dabs(du),dabs(dp)).lt.tol) then
            goto 1
          else
            if (it.eq.itmax) then
              write(6,*)'Error (solidStep) : did not converge'
              stop
            end if
          end if
        end do
    1   continue
      end do
c
c advance p for r=0 (u is equal to zero at r=0), if needed
      if (j0.eq.1) then
        urt=(up(1)-um(1))/(2.d0*dt*dr)
        a1=1.d0+dt*urt/urp1
        b1=2.d0*p(0)-(1.d0-dt*urt/urp1)*pm(0)+d(0)*dt**2
        pp(0)=b1/a1
        up(0)=0.d0
      end if
c
      return
      end
c
c++++++++++++++++++
c
      subroutine fluidStep (n,dx,dt,rf,vf,pf,rfp,vfp,pfp,
     *                      rstar,vstar,pstar,u1,u1p,ut1,ut1p)
c
c take a time step in the fluid domain using Lax-Wendroff 2-step.
c The code takes (rf,vf,pf) at time t and computes (rfp,vfp,pfp)
c at time t+dt.  (u1,ut1) are the displacement and velocity of
c the left boundary at time t and (u1p,ut1p) are these quantities
c at time t+dt/2.
c
      implicit real*8 (a-h,o-z)
      dimension rf(-1:n+1),vf(-1:n+1),pf(-1:n+1),rfp(0:n),vfp(0:n),
     *          pfp(0:n),rstar(0:n+1),vstar(0:n+1),pstar(0:n+1)
      common / fluidParams / r1,r2,gamma,pOffset
c
c first half step
      d=r2-(r1+u1)
      dr=d*dx
      do j=0,n+1
        x=(j-.5d0)*dx
        r=(r1+u1)+d*x
        gdot=ut1*(1.d0-x)
        rfr=(rf(j)-rf(j-1))/dr
        vfr=(vf(j)-vf(j-1))/dr
        pfr=(pf(j)-pf(j-1))/dr
        rfa=.5d0*(rf(j-1)+rf(j))
        vfa=.5d0*(vf(j-1)+vf(j))
        pfa=.5d0*(pf(j-1)+pf(j))
        rstar(j)=rfa-.5d0*dt*((vfa-gdot)*rfr+rfa*(vfr+vfa/r))
        vstar(j)=vfa-.5d0*dt*((vfa-gdot)*vfr+pfr/rfa)
        pstar(j)=pfa-.5d0*dt*((vfa-gdot)*pfr+gamma*pfa*(vfr+vfa/r))
      end do
c
c full time step
      d=r2-(r1+u1p)
      dr=d*dx
      do j=0,n
        x=j*dx
        r=(r1+u1p)+d*x
        gdot=ut1p*(1.d0-x)
        rfr=(rstar(j+1)-rstar(j))/dr
        vfr=(vstar(j+1)-vstar(j))/dr
        pfr=(pstar(j+1)-pstar(j))/dr
        rfa=.5d0*(rstar(j)+rstar(j+1))
        vfa=.5d0*(vstar(j)+vstar(j+1))
        pfa=.5d0*(pstar(j)+pstar(j+1))
        rfp(j)=rf(j)-dt*((vfa-gdot)*rfr+rfa*(vfr+vfa/r))
        vfp(j)=vf(j)-dt*((vfa-gdot)*vfr+pfr/rfa)
        pfp(j)=pf(j)-dt*((vfa-gdot)*pfr+gamma*pfa*(vfr+vfa/r))
      end do
c
      return
      end
c
c++++++++++++++++++
c
      subroutine solidBCs (n,dr,u,p,um,pm,up,pp,p11)
c
c reset (u,p), (um,pm) and (up,pp) and apply boundary conditions
c to set ghost points
c
      implicit real*8 (a-h,o-z)
      dimension u(-1:n+1),p(-1:n+1),um(0:n),pm(0:n),up(0:n),pp(0:n)
      common / solidParams / r0,alam,amu,akap,j0
      data tol, itmax / 1.d-12, 10 /
c
c initial guess for ur at r=r1 (interface)
      ur=(u(n+1)-u(n-1))/(2.d0*dr)
c
c reset: (u,p)->(um,pm) and (up,pp)->(u,p)
      do j=0,n
        um(j)=u(j)
        pm(j)=p(j)
        u(j)=up(j)
        p(j)=pp(j)
      end do
c
c boundary conditions at r=r0 (no traction if j0=0 or regularity if j0=1)
      if (j0.eq.0) then
        udr=u(0)/r0
        ak=alam*udr*(2.d0+udr)/akap
        if (ak.ge.1.d0) then
          write(6,*)'Error (solidBCs) : cannot resolve bc0'
          stop
        end if
        ur=-ak/(1.d0+dsqrt(1.d0-ak))
        u(-1)=u(1)-2.d0*dr*ur
        p(-1)=p(1)
      else
        u(-1)=-u(1)
        p(-1)= p(1)
      end if
c
c boundary conditions at r=r1 (p12=0 and p11=given)
      r1=r0+n*dr
      udr=u(n)/r1
      alp=alam*udr*(1.d0+.5d0*udr)
      do it=1,itmax
        q=(akap*ur*(1.d0+.5d0*ur)+alp)
        f=(1.d0+ur)*q-p11
        fp=q+akap*(1.d0+ur)**2
        dur=f/fp
        ur=ur-dur
        if (dabs(dur).lt.tol) then
          u(n+1)=u(n-1)+2.d0*dr*ur
          p(n+1)=p(n-1)
          return
        else
          if (it.eq.itmax) then
            write(6,*)'Error (solidBCs) : cannot resolve bc1'
            stop
          end if
        end if
      end do
c
      return
      end
c
c++++++++++++++++++
c
      subroutine fluidBCs (n,rf,vf,pf,rfp,vfp,pfp)
c
c reset fluid variables and use BCs to set ghost points
c
      implicit real*8 (a-h,o-z)
      dimension rf(-1:n+1),vf(-1:n+1),pf(-1:n+1),rfp(0:n),vfp(0:n),
     *          pfp(0:n)
c
c reset: (rfp,vfp,pfp)->(rf,vf,pf)
      do j=0,n
        rf(j)=rfp(j)
        vf(j)=vfp(j)
        pf(j)=pfp(j)
      end do
c
c extrapolation at the interface
      rf(-1)=3.d0*(rf(0)-rf(1))+rf(2)
      vf(-1)=3.d0*(vf(0)-vf(1))+vf(2)
      pf(-1)=3.d0*(pf(0)-pf(1))+pf(2)
c
c no-flow at r=r2 (fixed solid wall)
      vf(n)=0.d0
      rf(n+1)=3.d0*(rf(n)-rf(n-1))+rf(n-2)
      vf(n+1)=3.d0*(vf(n)-vf(n-1))+vf(n-2)
      pf(n+1)=pf(n-1)
c
      return
      end
c
c+++++++++++++++++++++
c
      subroutine getSoln (n,dr,dt,u,p,um,pm,up,pp,nf,dx,rf,vf,pf,
     *                    nSolid,uSolid,nFluid,uFluid)
c
      implicit real*8 (a-h,o-z)
      dimension u(-1:n+1),p(-1:n+1),um(0:n),pm(0:n),up(0:n),pp(0:n)
      dimension rf(-1:nf+1),vf(-1:nf+1),pf(-1:nf+1)
      dimension uSolid(nSolid,0:8),uFluid(nFluid,0:3)
      common / solidParams / r0,alam,amu,akap,j0
      common / fluidParams / r1,r2,gamma,pOffset
c
      do j=0,n
        jp1=j+1
        r=r0+j*dr
        ut=(up(j)-um(j))/(2.d0*dt)
        pt=(pp(j)-pm(j))/(2.d0*dt)
        ur=(u(j+1)-u(j-1))/(2.d0*dr)
        urp1=ur+1.d0
        if (j.gt.0.or.j0.eq.0) then
          udr=u(j)/r
        else
          udr=ur
        end if
        udrp1=udr+1.d0
        rpr=r*(p(j+1)-p(j-1))/(2.d0*dr)
        e11=.5d0*((2.d0+ur)*ur+(udrp1*rpr)**2)
        e12=.5d0*rpr*udrp1**2
        e22=.5d0*(2.d0+udr)*udr
        s11=akap*e11+alam*e22
        s12=2.d0*amu*e12
        s21=s12
        s22=alam*e11+akap*e22
        p11=urp1*s11
        p12=udrp1*(rpr*s11+s12)
        p21=urp1*s21
        p22=udrp1*(rpr*s21+s22)
        uSolid(jp1,0)=r+u(j)   ! physical radius
        uSolid(jp1,1)=u(j)     ! radial displacement
        uSolid(jp1,2)=p(j)     ! angular displacement
        uSolid(jp1,3)=ut       ! radial velocity
        uSolid(jp1,4)=pt       ! angular velocity
        uSolid(jp1,5)=p11      ! components of bar-P (see notes)
        uSolid(jp1,6)=p12
        uSolid(jp1,7)=p21
        uSolid(jp1,8)=p22
c        write(51,300)r,u(j),p(j),ut,pt,p11,p12,p21,p22
c  300   format(9(1x,1pe22.15))
      end do
c
      r1u=r1+u(n)
      d=r2-r1u
      do j=0,nf
        jp1=j+1
        x=j*dx
        r=r1u+d*x
        uFluid(jp1,0)=r        ! physical radius
        uFluid(jp1,1)=rf(j)    ! density
        uFluid(jp1,2)=vf(j)    ! radial velocity
        uFluid(jp1,3)=pf(j)    ! pressure
      end do
c
c filter
      eps=1.d-16
      do j=1,nSolid
        do k=0,8
          if (dabs(uSolid(j,k)).lt.eps) then
            uSolid(j,k)=0.d0
          end if
        end do
      end do
      do j=1,nFluid
        do k=0,3
          if (dabs(uFluid(j,k)).lt.eps) then
            uFluid(j,k)=0.d0
          end if
        end do
      end do
c
      return
      end
