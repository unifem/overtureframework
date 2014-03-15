      subroutine cmpflux1d (m,wl,wr,fl,fr,speed,method)
c
c Compute Godunov fluxes for BN equations
c
c Input: wl,wr = left and right states in primitive variables
c
c Output: wl,wr = star-state and right-of-solid-contact state
c                 if solid contact velocity > 0
c               = left-of-solid-contact state and star-state
c                 if solid contact velocity < 0
c         fl,fr = fluxes including contribution from the solid
c                 contact layer
c         speed = largest wave speed (for time step calculation)
c
      implicit double precision (a-h,o-z)
      dimension wl(m),wr(m),fl(m),fr(m),wvl(7),wvr(7),fvl(7),fvr(7)
      dimension pm(2,2),vm(2,2),dv(2,2),rm(2),fp(2,2),gp(2)
      common / gasdat1d / gam(2),gm1(2),gp1(2),em(2),ep(2),p0(2),bgas
      common / prmdat1d / alpha(2,2),r(2,2),v(2,2),p(2,2),
     *                  a(2,2),b(2,2),c(2,2)
      common / cntdat1d / icnt(4)
      common / errdat1d / err
      common / flxdat1d / iflux
      common / xtdat1d / xpos,tpos,lv
c
c if iflux=0, then use ideal EOS with local gamma
c    iflux=1, then use virial EOS unless an error occurs in which case use ideal EOS
c    iflux>1, then use virial EOS and stop if an error occurs
c
c if iflux<0, compute virial and ideal EOS, but use ideal EOS with local gamma
c
      if (iflux.ne.0) then
c
c compute flux using virial EOS equations directly
        if (m.eq.7) then
          do i=1,m
            wvl(i)=wl(i)
            wvr(i)=wr(i)
          end do
        else
          write(6,*)'Error (gdflux) : m.ne.7'
          stop
        end if
c
c solid middle state based on stiffened ideal EOS
        call ideals1d (1,wvl(2),wvr(2),pm(1,1),vm(1,1),fp(1,1),ier)
        if (ier.ne.0) then
          write(6,*)'ideals, ier=',ier
c
c   if ideals fails, then there is no available fix...
c
c          pause
c          if (iflux.eq.1) goto 1
c          write(6,*)'Error (gdflux) : error return from ideals'
          goto 2
        end if
c
c gas middle state based on virial EOS
        call virial21d (2,wvl(5),wvr(5),pm(1,2),vm(1,2),fp(1,2),
     *                  gp,rm,ier)
        if (ier.ne.0) then
          write(6,*)'virial, ier=',ier
c          if (ier.eq.7) pause
c          pause
          fact=bgas*max(wvl(5),wvr(5))
c          write(55,222)xpos,tpos,fact,lv
c  222     format(3(1x,1pe15.8),1x,i2,' 0')
          if (iflux.eq.1) goto 1
          write(6,*)'Error (gdflux) : error return from virial'
          goto 2
        end if
c
c coupled middle state and flux
        call couple31d (m,wvl,wvr,rm,vm,pm,fp,gp,fvl,fvr,method,ier)
        if (ier.ne.0) then
          write(6,*)'couple3, ier=',ier
c          pause
          fact=bgas*max(wvl(5),wvr(5))
c          write(55,223)xpos,tpos,fact,lv
c  223     format(3(1x,1pe15.8),1x,i2,' 1')
          if (iflux.eq.1) goto 1
          write(6,*)'Error (gdflux) : error return from couple2'
          goto 2
        end if
c
        if (iflux.gt.0) then
          do i=1,m
            wl(i)=wvl(i)
            wr(i)=wvr(i)
            fl(i)=fvl(i)
            fr(i)=fvr(i)
          end do
cc          call maxspd2 (rm,pm,speed)
          return
        end if
c
      end if
c
c..save current gamma_gas and re-calculate gamma and its related forms
c  based on an effective virial EOS factor using the average gas density
    1 gamgas=gam(2)
      fact=1.d0+bgas*(wl(5)+wr(5))/2.d0
c      gam(2)=gm1(2)*fact+1
      gam(2)=gam(2)*fact
      gm1(2)=gam(2)-1.d0
      gp1(2)=gam(2)+1.d0
      em(2)=0.5d0*gm1(2)/gam(2)
      ep(2)=0.5d0*gp1(2)/gam(2)
c
c icnt(1) counts the total number of flux calculations
      icnt(1)=icnt(1)+1
c
c..assign volume fractions
      alpha(1,1)=wl(1)
      alpha(1,2)=1.d0-wl(1)
      alpha(2,1)=wr(1)
      alpha(2,2)=1.d0-wr(1)
c
c..compute decoupled middle state for each phase
      do j=1,2
        k=3*j-1
        call middle1d (j,wl(k),wr(k),pm(1,j),vm(1,j),dv(1,j),ier)
        if (ier.ne.0) then
          write(6,100)ier,wl,wr
 100      format('Error (gdflux) : error return, ier=',i1,/,
     *             'wl=',7(1x,1pe15.8),/,'wr=',7(1x,1pe15.8))
          stop
        end if
      end do
c
c..compute coupled middle state and fluxes, or decoupled fluxes
      call couple1d (m,pm,vm,dv,wl,wr,fl,fr,method,ier)
c
      if (ier.ne.0) then
        write(6,*)'Error (gdflux) : error return, ier=',ier
        stop
      end if
c
c        do i=1,7
c          fvl(i)=max(dabs(fl(i)-fvl(i)),dabs(fr(i)-fvr(i)))
c        end do
c        err=fvl(1)/(.5d0*(c(1,1)+c(2,1)))
c        err=max(fvl(2)/(.5d0*(r(1,1)*c(1,1)+r(2,1)*c(2,1))),err)
c        err=max(fvl(3)/(.5d0*(p(1,1)+p(2,1))+p0(1)),err)
c        err=max(fvl(4)/(.5d0*((p(1,1)+p0(1))*c(1,1)
c     *                       +(p(2,1)+p0(1))*c(2,1))),err)
c        err=max(fvl(5)/(.5d0*(r(1,2)*c(1,2)+r(2,2)*c(2,2))),err)
c        err=max(fvl(6)/(.5d0*(p(1,2)+p(2,2))+p0(1)),err)
c        err=max(fvl(7)/(.5d0*(p(1,2)*c(1,2)+p(2,2)*c(2,2))),err)
c
c..compute maximum wave speed
c      call maxspd (pm,speed)
c
c..return gamma_gas to its original value
      gam(2)=gamgas
      gm1(2)=gam(2)-1.d0
      gp1(2)=gam(2)+1.d0
      em(2)=0.5d0*gm1(2)/gam(2)
      ep(2)=0.5d0*gp1(2)/gam(2)
c
      return
c
    2 write(6,*)'writing gdflux.out ...'
      open (40,file='gdflux.out')
      write(40,400)m,(wl(i),i=1,m),(wr(i),i=1,m)
  400 format(1x,i2,2(/,7(1x,1pe22.15)))
      do i=1,2
        write(40,401)gam(i),gm1(i),gp1(i),em(i),ep(i),p0(i)
  401   format(6(1x,1pe22.15))
      end do
      write(40,402)bgas,method
  402 format(1x,1pe22.15,1x,i2)
      close (1)
c
      stop
      end
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
      subroutine ideals1d (j,wl,wr,pm,vm,fp,ier)
c
c Middle states for the Riemann problem for the stiffened ideal EOS
c
      implicit double precision (a-h,o-z)
      dimension wl(3),wr(3),pm(2),vm(2),fp(2)
      dimension dv(2)
      common / gasdat1d / gam(2),gm1(2),gp1(2),em(2),ep(2),p0(2),bgas
      common / prmdat1d / alpha(2,2),r(2,2),v(2,2),p(2,2),
     *                  a(2,2),b(2,2),c(2,2)
c     data pratio, ptol / 2.d0, 1.d-6 /
      data pratio, pfact / 2.d0, 1.d-4 /
      data tol, itmax / 1.d-3, 6 /
c
      ier=0
c
c minimum pressure tolerance
c      ptol=pfact*min(wl(3),wr(3))
c
c minimum pressure tolerance, assumes p0(2)=0
      if (j.eq.1) then
        ptol=-(1.d0-pfact)*p0(1)
      else
        ptol=pfact*min(wl(3),wr(3))
      end if
c
c left primitive state variables
      r(1,j)=wl(1)
      v(1,j)=wl(2)
      p(1,j)=wl(3)
c
c right primitive state variables
      r(2,j)=wr(1)
      v(2,j)=wr(2)
      p(2,j)=wr(3)
c
c sound speeds and some constants
      do i=1,2
        a(i,j)=2.d0/(gp1(j)*r(i,j))
        b(i,j)=gm1(j)*(p(i,j)+p0(j))/gp1(j)+p0(j)
        c2=gam(j)*(p(i,j)+p0(j))/r(i,j)
        if (c2.le.0.d0) then
          write(6,*)'Error (ideals) : c2.le.0, i,j =',i,j
          ier=1
          return
        end if
        c(i,j)=dsqrt(c2)
      end do
c
c check for vacuum state (in the decoupled phases)
      if (2.d0*(c(1,j)+c(2,j))/gm1(j).le.v(2,j)-v(1,j)) then
        write(6,*)'Error (ideals) : vacuum found, j=',j
        ier=2
        return
      end if
c
c compute min/max pressures
c      pmin=min(p(1,j),p(2,j))
c      pmax=max(p(1,j),p(2,j))
      pmin=min(p(1,j)+p0(j),p(2,j)+p0(j))
      pmax=max(p(1,j)+p0(j),p(2,j)+p0(j))
c
c start with guess based on a linearization
      ppv=.5d0*(p(1,j)+p(2,j))
     *    -.125d0*(v(2,j)-v(1,j))*(r(1,j)+r(2,j))*(c(1,j)+c(2,j))
c      ppv=max(ppv,0.d0)
      ppv=max(ppv+p0(j),0.d0)
c
c      if ((pmax+p0(j))/(pmin+p0(j)).le.pratio
c     *   .and.pmin.le.ppv.and.pmax.ge.ppv) then
      if (pmax/pmin.le.pratio
     *   .and.pmin.le.ppv.and.pmax.ge.ppv) then
c        pstar=ppv
        pstar=ppv-p0(j)
      else
        if (ppv.lt.pmin) then
c guess based on two rarefaction solution
          arg1=c(1,j)/((p(1,j)+p0(j))**em(j))
          arg2=c(2,j)/((p(2,j)+p0(j))**em(j))
          arg3=(c(1,j)+c(2,j)-.5d0*gm1(j)*(v(2,j)-v(1,j)))/(arg1+arg2)
          pstar=arg3**(1.d0/em(j))-p0(j)
        else
c guess based on two shock approximate solution
          gl=dsqrt(a(1,j)/(ppv+b(1,j)))
          gr=dsqrt(a(2,j)/(ppv+b(2,j)))
          pts=(gl*p(1,j)+gr*p(2,j)-v(2,j)+v(1,j))/(gl+gr)
          pstar=max(ptol,pts)
        end if
      end if
c
c get middle pstar state
      do it=1,itmax
c
c determine velocity difference across a shock or rarefaction
        do i=1,2
          if (pstar.gt.p(i,j)) then
            arg=pstar+b(i,j)
            fact=dsqrt(a(i,j)/arg)
            diff=pstar-p(i,j)
            dv(i)=fact*diff
            fp(i)=fact*(1.d0-0.5d0*diff/arg)
          else
            arg=(pstar+p0(j))/(p(i,j)+p0(j))
            fact=2.d0*c(i,j)/gm1(j)
            dv(i)=fact*(arg**em(j)-1.d0)
            fp(i)=1.d0/(r(i,j)*c(i,j)*arg**ep(j))
          end if
        end do
c
c   determine change to pressure in the middle state
        vm(1)=v(1,j)-dv(1)
        vm(2)=v(2,j)+dv(2)
        dp=(vm(2)-vm(1))/(fp(1)+fp(2))
        pstar=pstar-dp
c
        if (dabs(dp)/(pstar+p0(j)).lt.tol) goto 1
c
      end do
c
c print warning if middle pstar state is not converged
      write(6,*)'Warning (ideals) : pstar not converged'
      ier=2
c
c assign middle pressures
    1 pm(1)=pstar
      pm(2)=pm(1)
c
c assign middle velocity
      vm(1)=.5d0*(vm(1)+vm(2))
      vm(2)=vm(1)
c
      return
      end
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
      subroutine virial21d (j,wl,wr,pm,vm,fp,gp,rm,ier)
c
c middle states of the Riemann problem for the virial EOS
c
      implicit real*8 (a-h,o-z)
      dimension wl(3),wr(3),pm(2),vm(2),fp(2),gp(2),rm(2)
      dimension dv(2),drm(2),esave(100)
      common / gasdat1d / gam(2),gm1(2),gp1(2),em(2),ep(2),p0(2),bgas
      common / prmdat1d / alpha(2,2),r(2,2),v(2,2),p(2,2),
     *                  a(2,2),b(2,2),c(2,2)
      common / romdat1d / aint0(2),r0(2),c0(2)
      data pratio, pfact / 2.d0, 1.d-4 /
      data tol, itmax / 1.d-3, 6 /
c
      ier=0
c
c minimum pressure tolerance
      ptol=pfact*min(wl(3),wr(3))
c
c left and right states
      r(1,j)=wl(1)
      v(1,j)=wl(2)
      p(1,j)=wl(3)
      r(2,j)=wr(1)
      v(2,j)=wr(2)
      p(2,j)=wr(3)
c
c compute sound speeds
      do i=1,2
        c2=p(i,j)*(gam(j)/r(i,j)+bgas*(gm1(j)+1.d0/(1.d0+bgas*r(i,j))))
        if (c2.lt.0.d0) then
          write(6,*)'Error (virial2) : c2.lt.0, i=',i
          ier=1
          return
        end if
        c(i,j)=dsqrt(c2)
      end do
c
c set new gamma
      gamma=gam(j)
      fact=1.d0+bgas*(r(1,j)+r(2,j))/2.d0
      gam(j)=gam(j)*fact
      gm1(j)=gam(j)-1.d0
      gp1(j)=gam(j)+1.d0
      em(j)=0.5d0*gm1(j)/gam(j)
      ep(j)=0.5d0*gp1(j)/gam(j)
c
c check for vacuum state (in the decoupled phases)
      if (2.d0*(c(1,j)+c(2,j))/gm1(j).le.v(2,j)-v(1,j)) then
        write(6,*)'Error (virial2) : vacuum found, j=',j
        ier=2
        return
      end if
c
c compute min/max pressures
      pmin=min(p(1,j),p(2,j))
      pmax=max(p(1,j),p(2,j))
c
c compute average density and sound speed
      rma=.5d0*(r(1,j)+r(2,j))
      cma=.5d0*(c(1,j)+c(2,j))
c
c start with guess based on a linearization
      ppv=.5d0*(p(1,j)+p(2,j))
     *    -.125d0*(v(2,j)-v(1,j))*(r(1,j)+r(2,j))*(c(1,j)+c(2,j))
      ppv=max(ppv,0.d0)
c
      if (pmax/pmin.le.pratio
     *   .and.pmin.le.ppv.and.pmax.ge.ppv) then
        rm(1)=r(1,j)-(rma*cma*(v(2,j)-v(1,j))-p(2,j)+p(1,j))
     *               /(2.d0*cma**2)
        rm(2)=r(2,j)-(rma*cma*(v(2,j)-v(1,j))-p(1,j)+p(2,j))
     *               /(2.d0*cma**2)
        if (rm(1).le.0.d0.or.rm(2).le.0.d0) then
          write(6,*)'Error (virial2) : initial density is negative'
          ier=2
          return
        end if
        iguess=0
      else
        if (ppv.lt.pmin) then
c guess based on two rarefaction solution
          arg1=c(1,j)/(p(1,j)**em(j))
          arg2=c(2,j)/(p(2,j)**em(j))
          arg3=(c(1,j)+c(2,j)-.5d0*gm1(j)*(v(2,j)-v(1,j)))/(arg1+arg2)
          pstar=arg3**(1.d0/em(j))
          rm(1)=r(1,j)*(pstar/p(1,j))**(1.d0/gam(j))
          rm(2)=r(2,j)*(pstar/p(2,j))**(1.d0/gam(j))
          iguess=-1
        else
c guess based on two shock approximate solution
          do i=1,2
            a(i,j)=2.d0/(gp1(j)*r(i,j))
            b(i,j)=gm1(j)*p(i,j)/gp1(j)
          end do
          gl=dsqrt(a(1,j)/(ppv+b(1,j)))
          gr=dsqrt(a(2,j)/(ppv+b(2,j)))
          pts=(gl*p(1,j)+gr*p(2,j)-v(2,j)+v(1,j))/(gl+gr)
          pstar=max(ptol,pts)
          do i=1,2
            z=pstar/p(i,j)-1.d0
            fact=1.d0+bgas*r(i,j)
            ftm=fact*(gamma-1.d0)/2.d0
            ftp=fact*(gamma+1.d0)/2.d0
            a2=ftp*(z+1.d0)+ftm
            a1=-(2.d0+ftm)*fact*(z+1.d0)-ftm*fact+1.d0
            a0=z*fact
            arg=a1**2-4.d0*a0*a2
            if (arg.lt.0.d0) then
              write(6,*)'Error (virial2) : two-shock solution failed'
              ier=8
              return
            end if
            vol=(-a1-dsqrt(arg))/(2.d0*a2)
            rm(i)=r(i,j)/(1.d0-vol)
          end do
          iguess=1
        end if
      end if
c
c initialize Romberg integration
      do i=1,2
        aint0(i)=0.d0
        r0(i)=r(i,j)
        c0(i)=c(i,j)
      end do
c
c reset gamma
      gam(j)=gamma
      gm1(j)=gam(j)-1.d0
      gp1(j)=gam(j)+1.d0
      em(j)=0.5d0*gm1(j)/gam(j)
      ep(j)=0.5d0*gp1(j)/gam(j)
c
c Newton iteration to find middle states
      itmax=10
      tol=1.d-5
      do it=1,itmax
c
c evaluate rarefaction or shock from each side
        do i=1,2
c
          facti=1.d0+bgas*r(i,j)
          factm=1.d0+bgas*rm(i)
c
          if (rm(i).le.r(i,j)) then
c
c rarefaction solution
            pm(i)=p(i,j)*((rm(i)/r(i,j))**gam(j))*factm/facti
     *                *dexp(gm1(j)*bgas*(rm(i)-r(i,j)))
            cm2=pm(i)*(gam(j)/rm(i)+bgas*(gm1(j)+1.d0/factm))
            if (cm2.lt.0.d0) then
              write(6,*)'Error (virial) : cm2.lt.0, i=',i
              ier=3
              return
            end if
            cm=dsqrt(cm2)
            if (rm(i).le.0.d0) then
              write(6,*)'Error (virial) : rm(i).le.0'
              write(6,*)'iguess=',iguess
              pause
              ier=4
              return
            end if
            dv(i)=getdv1d(i,rm(i),cm)
c
c derivatives
            gp(i)=cm2
            fp(i)=cm/rm(i)
c
          else
c
c shock solution
            z=r(i,j)/rm(i)
            z2=1.d0-z
            z3=.5d0*gm1(j)*facti*z2
            ratio=facti/factm
            denom=ratio*z-z3
            if (denom.le.0.d0) then
              write(6,*)'Error (virial) : denom.le.0'
              ier=7
              return
            end if
            pm(i)=p(i,j)*(1.d0+z3)/denom
            dv(i)=dsqrt(p(i,j)*(pm(i)/p(i,j)-1.d0)*z2/r(i,j))
c
c derivatives
            gp(i)=ratio*(z+bgas*r(i,j)/factm)+.5d0*gm1(j)*facti*z
     *            *(1.d0+ratio+bgas*rm(i)*ratio*z2/factm)
            gp(i)=(p(i,j)/rm(i))*gp(i)/denom**2
            fp(i)=.5d0*dv(i)*(gp(i)/(pm(i)-p(i,j))+z/(rm(i)-r(i,j)))
c
          end if
c
        end do
c
c compute middle velocities
        vm(1)=v(1,j)-dv(1)
        vm(2)=v(2,j)+dv(2)
c
c compute density increments
        det=fp(1)*gp(2)+fp(2)*gp(1)
        drm(1)=(gp(2)*(vm(2)-vm(1))-fp(2)*(pm(2)-pm(1)))/det
        drm(2)=(gp(1)*(vm(2)-vm(1))+fp(1)*(pm(2)-pm(1)))/det
c
c update middle densities
        rm(1)=rm(1)-drm(1)
        rm(2)=rm(2)-drm(2)
c
        if (rm(1).le.0.d0.or.rm(2).le.0.d0) then
          write(6,*)'Error (virial) : negative densities'
          ier=5
          return
        end if
c
        err=max(dabs(drm(1)),dabs(drm(2)))/rma
        esave(it)=err
c        write(56,100)it,(rm(i),drm(i),i=1,2),err
c  100   format(' * it=',i2,' : rm(1)=',1pe15.8,', drm(1)=',1pe9.2,/,
c     *             '           rm(2)=',1pe15.8,', drm(2)=',1pe9.2,
c     *         ', err=',1pe9.2)
c
        if (err.lt.tol) then
          pm(1)=.5d0*(pm(1)+pm(2))
          vm(1)=.5d0*(vm(1)+vm(2))
          pm(2)=pm(1)
          vm(2)=vm(1)
c          write(56,200)pm(1),vm(1)
c  200     format(' * converged :',/,
c     *           '      pstar=',1pe15.8,', vstar=',1pe15.8)
          return
        end if
c
      end do
c
      write(6,*)'Error (virial2) : iteration failed to converge'
      ier=6
      write(6,*)'iguess=',iguess
      do i=1,itmax
        write(6,*)'it,err=',i,esave(i)
      end do
      pause
c      write(56,300)
c  300 format(' * iteration failed')
c
      return
      end
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
      double precision function getdv1d (i,rm,cm)
c
c evaluate FL(rho) or FR(rho) for virial gas (essentially integrate
c C+ or C- characteristic equation numerically)
c
      implicit real*8 (a-h,o-z)
      parameter (nmax=20)
      dimension aint(2,nmax),err(nmax)
      common / gasdat1d / gam(2),gm1(2),gp1(2),em(2),ep(2),p0(2),bgas
      common / prmdat1d / alpha(2,2),r(2,2),v(2,2),p(2,2),
     *                  a(2,2),b(2,2),c(2,2)
      common / romdat1d / aint0(2),r0(2),c0(2)
c
      j=2
      facti=1.d0+bgas*r(i,j)
      factm=1.d0+bgas*rm
c
c Romberg integration
      atol=1.d-10
      dr=rm-r0(i)
      aint(1,1)=.5d0*(cm/rm+c0(i)/r0(i))*dr
      do n=2,nmax
        nm1=n-1
        sum=0.d0
        kmax=2**(n-2)
        do k=1,kmax
          rk=r0(i)+(k-.5d0)*dr
          factk=1.d0+bgas*rk
          pk=p(i,j)*((rk/r(i,j))**gam(j))*factk/facti
     *              *dexp(gm1(j)*bgas*(rk-r(i,j)))
          ck=dsqrt(pk*(gam(j)/rk+bgas*(gm1(j)+1.d0/factk)))
          sum=sum+ck/rk
        end do
        aint(2,1)=.5d0*(aint(1,1)+dr*sum)
        do l=1,nm1
          aint(2,l+1)=aint(2,l)+(aint(2,l)-aint(1,l))/(4**l-1.d0)
        end do
c        write(56,*)n,aint(1,nm1),aint(2,n)
        err(n)=dabs(aint(2,n)-aint(1,nm1))/c(i,j)
        if (err(n).lt.atol) then
          aint0(i)=aint(2,n)+aint0(i)
          r0(i)=rm
          c0(i)=cm
          getdv1d=aint0(i)
          return
        end if
        dr=.5d0*dr
        do l=1,n
          aint(1,l)=aint(2,l)
        end do
      end do
c
      write(6,100)
  100 format('** Warning (getdv) : Romberg did not converge')
      do n=2,nmax
        write(6,101)n,err(n)
  101   format('    it=',i2,',  error=',1pe9.2)
      end do
      pause
c
      getdv1d=aint(2,n)+aint0(i)
c
      return
      end
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
      subroutine couple31d (m,wl,wr,rm,vm,pm,fp,gp,fl,fr,method,ier)
c
      implicit double precision (a-h,o-z)
      dimension wl(m),wr(m),rm(2),pm(2,2),vm(2,2),fp(2,2),gp(2),
     *          fl(m),fr(m),g(4),rmsave(2),vmsave(2,2),pmsave(2,2)
      common / gasdat1d / gam(2),gm1(2),gp1(2),em(2),ep(2),p0(2),bgas
      common / prmdat1d / alpha(2,2),r(2,2),v(2,2),p(2,2),
     *                  a(2,2),b(2,2),c(2,2)
      common / scale31d / rm0save,rscale,pscale,vscale,gfact(4)
      common / xtdat1d / xpos,tpos,lv
      common / cutdat1d / abmin1
      data sig, frac / 10.d0, .95d0 /   ! this is what Chris likes.
c      data sig, frac / 10.d0, .99d0 /
c
      ier=0
c
c..assign volume fractions
      alpha(1,1)=wl(1)
      alpha(1,2)=1.d0-wl(1)
      alpha(2,1)=wr(1)
      alpha(2,2)=1.d0-wr(1)
c
      if (dabs(wr(1)-wl(1)).gt.1.d-14
     *      .and.min(wl(1),wr(1)).gt.abmin1) then
c
c        write(57,100)(wl(i),wr(i),i=1,7)
c  100   format(' * coupled states: wl,wr=',7(/,5x,2(1x,1pe15.8)))
c
c compute pressure and velocity scales
        rscale=max(rm(1),rm(2))
        pscale=max(pm(1,1),pm(1,2))
        vscale=.5d0*(c(1,2)+c(2,2))
        gfact(1)=1.d0/vscale
        gfact(2)=gfact(1)/rscale
        gfact(3)=1.d0/pscale
        gfact(4)=gfact(1)/vscale
c
        do i=1,2
          rmsave(i)=rm(i)
          do j=1,2
            vmsave(i,j)=vm(i,j)
            pmsave(i,j)=pm(i,j)
          end do
        end do
c
        it=0
        itmax=4
        dalp=wr(1)-wl(1)
c
    1   if (dabs(wl(1)-.5d0).lt.dabs(wr(1)-.5d0)) then
          alpha(2,1)=wl(1)+dalp
          alpha(2,2)=1.d0-alpha(2,1)
        else
          alpha(1,1)=wr(1)-dalp
          alpha(1,2)=1.d0-alpha(1,1)
        end if
c
c "middle" value for alpha to linearize about
c       asm=.5d0*(alpha(1,1)+alpha(2,1))
        if (alpha(1,1).gt.0.5d0) then
          if (alpha(2,1).lt.0.5d0) then
            asm=.5d0
          else
            asm=min(alpha(1,1),alpha(2,1))
          end if
        else
          if (alpha(2,1).gt.0.5d0) then
            asm=.5d0
          else
            asm=max(alpha(1,1),alpha(2,1))
          end if
        end if
        agm=1.d0-asm
c
c linear update for middle solid pressures
        eps=alpha(2,1)-alpha(1,1)
        del=(pm(1,2)-pm(1,1))*eps/(asm*(fp(1,1)+fp(2,1)))
        pm(1,1)=pm(1,1)-fp(2,1)*del
        pm(2,1)=pm(2,1)+fp(1,1)*del
c
c linear update for middle gas densities
        dvm=vm(1,2)-vm(1,1)
c        write(6,*)dvm
        if (dvm.lt.0.d0) then
          iconf=-1
          rm0=rm(2)
          fact=1.d0+bgas*rm0
          cm2=pm(2,2)*(gam(2)/rm0+bgas*(gm1(2)+1.d0/fact))
          if (cm2.le.0.d0) then
            write(6,*)'Error (couple3) : cm2.le.0 (L1)'
            ier=1
            return
          end if
c limit velocity difference
          cm=frac*dsqrt(cm2)
          arg1=sig*(1.d0+dvm/cm)
          arg2=sig*(1.d0-dvm/cm)
          dvm=.5d0*cm*log(cosh(arg1)/cosh(arg2))/sig
c          write(6,*)dvm
          eps=alpha(2,2)-alpha(1,2)
          del=dvm*eps/(agm*(1.d0-dvm**2/cm2)
     *                    *(fp(1,2)*gp(2)+fp(2,2)*gp(1)))
          fact=rm0*dvm
          rm(1)=rm(1)-(gp(2)+fact*fp(2,2))*del
          rm(2)=rm(2)-(gp(1)-fact*fp(1,2))*del
          rm0=rm0-(fact*fp(1,2)*(gp(2)-cm2)
     *           +(fact*fp(2,2)+cm2)*gp(1))*del/cm2
        else
          iconf=+1
          rm0=rm(1)
          fact=1.d0+bgas*rm0
          cm2=pm(1,2)*(gam(2)/rm0+bgas*(gm1(2)+1.d0/fact))
          if (cm2.le.0.d0) then
            write(6,*)'Error (couple3) : cm2.le.0 (L2)'
            ier=3
            return
          end if
c limit velocity difference
          cm=frac*dsqrt(cm2)
          arg1=sig*(1.d0+dvm/cm)
          arg2=sig*(1.d0-dvm/cm)
          dvm=.5d0*cm*log(cosh(arg1)/cosh(arg2))/sig
c          write(6,*)dvm
          eps=alpha(2,2)-alpha(1,2)
          del=dvm*eps/(agm*(1.d0-dvm**2/cm2)
     *                    *(fp(1,2)*gp(2)+fp(2,2)*gp(1)))
          fact=rm0*dvm
          rm(1)=rm(1)-(gp(2)+fact*fp(2,2))*del
          rm(2)=rm(2)-(gp(1)-fact*fp(1,2))*del
          rm0=rm0+(fact*fp(2,2)*(gp(1)-cm2)
     *           +(fact*fp(1,2)-cm2)*gp(2))*del/cm2
        end if
c        write(6,*)rm0,rm(1),rm(2)
c        pause
c
c check linearized state
        if (rm(1).le.0.d0.or.rm(2).le.0.d0.or.rm0.le.0.d0) then
          do i=1,2
            rm(i)=rmsave(i)
            do j=1,2
              vm(i,j)=vmsave(i,j)
              pm(i,j)=pmsave(i,j)
            end do
          end do
          it=it+1
          if (it.gt.itmax) then
            if (vm(1,2).gt.vm(1,1)) then
              rm0=rm(1)
            else
              rm0=rm(2)
            end if
            goto 2
          end if
          dalp=dalp/2.d0
          goto 1
        end if
c
c compute residual of the jump conditions
c        iconf=0
        rm0save=rm0
        do i=1,2
          rmsave(i)=rm(i)
          do j=1,2
            vmsave(i,j)=vm(i,j)
            pmsave(i,j)=pm(i,j)
          end do
        end do
c
        call getg31d (rm0,rm,vm,pm,g,resid,iconf,ier)
c
c        write(57,200)resid,(g(i),i=1,5)
c  200   format('   * residuals for linearize state, resid=',1pe9.2,/,
c     *         4x,5(1x,1pe9.2))
c
        itmax=4
        rtol=1.d-10
        if (resid.gt.rtol.or.ier.ne.0) then
          if (ier.eq.0) then
            call newton31d (rm0,rm,vm,pm,g,iconf,ier)
          end if
          if (ier.ne.0) then
            fact=bgas*max(r(1,2),r(2,2))
c            write(55,223)xpos,tpos,fact,lv
c  223       format(3(1x,1pe15.8),1x,i2,' 2')
            do it=1,itmax
              dalp=dalp/2.d0
c              write(6,*)'it,dalp=',it,dalp
              if (dabs(wl(1)-.5d0).lt.dabs(wr(1)-.5d0)) then
                alpha(2,1)=wl(1)+dalp
                alpha(2,2)=1.d0-alpha(2,1)
              else
                alpha(1,1)=wr(1)-dalp
                alpha(1,2)=1.d0-alpha(1,1)
              end if
              rm0=rm0save
              do i=1,2
                rm(i)=rmsave(i)
                do j=1,2
                  vm(i,j)=vmsave(i,j)
                  pm(i,j)=pmsave(i,j)
                end do
              end do
              call getg31d (rm0,rm,vm,pm,g,resid,iconf,ier)
              if (ier.eq.0) then
                call newton31d (rm0,rm,vm,pm,g,iconf,ier)
              end if
              if (ier.eq.0) goto 2
            end do
            ier=0
            rm0=rm0save
            do i=1,2
              rm(i)=rmsave(i)
              do j=1,2
                vm(i,j)=vmsave(i,j)
                pm(i,j)=pmsave(i,j)
              end do
            end do
          end if
        end if
c
      else
c
        if (vm(1,2).gt.vm(1,1)) then
          rm0=rm(1)
        else
          rm0=rm(2)
        end if
c
      end if
c
    2 alpha(1,1)=wl(1)
      alpha(1,2)=1.d0-wl(1)
      alpha(2,1)=wr(1)
      alpha(2,2)=1.d0-wr(1)
c
c+++++++++++++++++++++++++++++++
c       now compute fluxes
c+++++++++++++++++++++++++++++++
c
c solid contact source contributions
      if (method.eq.0) then
        source=pm(2,1)*alpha(2,1)-pm(1,1)*alpha(1,1)
      else
        source=0.d0
      end if
      source2=vm(1,1)*source
c
c check if solid contact is to the right or left of x=0
      if (vm(1,1).gt.0.d0) then
c
c right-of-solid-contact state (only need solid velocity and gas pressure)
        wr(3)=vm(1,1)
        wr(7)=pm(2,2)
c
c solid volume fraction flux
        if (method.eq.0) then
          fl(1)= 0.d0
          fr(1)=-vm(1,1)*(alpha(2,1)-alpha(1,1))
        else
          fl(1)=0.d0
          fr(1)=0.d0
        end if
c
c solid phase:
        call getfx1d (1,1,alpha(1,1),pm(1,1),vm(1,1),wl(2),fl(2))
c
c add on solid contact source contributions
        fr(2)=fl(2)
        fr(3)=fl(3)+source
        fr(4)=fl(4)+source2
c
c gas phase:
        if (vm(1,2).ge.0.d0) then
c gas contact (and solid contact) to the right
          call getfxv1d (1,alpha(1,2),rm(1),vm(1,2),pm(1,2),wl(5),
     *                 fl(5),ier)
          if (ier.ne.0) return
        else
c gas contact to the left, solid contact to the right
          call getfxv21d (1,alpha(1,2),rm0,vm(1,2),pm(1,2),wl(5),fl(5))
        end if
c
c add on solid contact source contributions
        fr(5)=fl(5)
        fr(6)=fl(6)-source
        fr(7)=fl(7)-source2
c
        if (method.ne.0) then
          do i=1,m
            wr(i)=wl(i)
          end do
        end if
c
      else
c
c left-of-solid-contact state (only need solid velocity and gas pressure)
        wl(3)=vm(1,1)
        wl(7)=pm(1,2)
c
c solid volume fraction flux
        if (method.eq.0) then
          fl(1)= vm(1,1)*(alpha(2,1)-alpha(1,1))
          fr(1)= 0.d0
        else
          fl(1)=0.d0
          fr(1)=0.d0
        end if
c
c solid phase:
        call getfx1d (2,1,alpha(2,1),pm(2,1),vm(2,1),wr(2),fr(2))
c
c add on solid contact source contributions
        fl(2)=fr(2)
        fl(3)=fr(3)-source
        fl(4)=fr(4)-source2
c
c gas phase:
        if (vm(2,2).le.0.d0) then
c gas contact (and solid contact) to the left
          call getfxv1d (2,alpha(2,2),rm(2),vm(2,2),pm(2,2),wr(5),
     *                 fr(5),ier)
          if (ier.ne.0) return
        else
c gas contact to the right, solid contact to the left
          call getfxv21d (2,alpha(2,2),rm0,vm(2,2),pm(2,2),wr(5),fr(5))
        end if
c
c add on solid contact source contributions
        fl(5)=fr(5)
        fl(6)=fr(6)+source
        fl(7)=fr(7)+source2
c
        if (method.ne.0) then
          do i=1,m
            wl(i)=wr(i)
          end do
        end if
c
      end if
c
      return
      end
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
      subroutine getfxv1d (i,alfa,rm,vm,pm,wstar,fx,ier)
c
c compute solid or gas flux for i=side and j=phase
c
      implicit double precision (a-h,o-z)
      dimension wstar(3),fx(3)
      common / gasdat1d / gam(2),gm1(2),gp1(2),em(2),ep(2),p0(2),bgas
      common / prmdat1d / alpha(2,2),r(2,2),v(2,2),p(2,2),
     *                  a(2,2),b(2,2),c(2,2)
c
      ier=0
c
c set j=2 (virial gas)
      j=2
c
c set isign=1 for i=1 and isign=-1 for i=2
      isign=3-2*i
c
      if (rm.gt.r(i,j)) then
c shock cases
        sp=isign*v(i,j)-dsqrt((rm/r(i,j))*(pm-p(i,j))/(rm-r(i,j)))
        if (sp.ge.0.d0) then
c left (i=1) or right (i=2)
          wstar(1)=r(i,j)
          wstar(2)=v(i,j)
          wstar(3)=p(i,j)
        else
c middle left (i=1) or middle right (i=2)
          wstar(1)=rm
          wstar(2)=vm
          wstar(3)=pm
        end if
      else
c rarefaction cases
        if (isign*v(i,j)-c(i,j).ge.0.d0) then
c left (i=1) or right (i=2)
          wstar(1)=r(i,j)
          wstar(2)=v(i,j)
          wstar(3)=p(i,j)
        else
c left middle or sonic (i=1) or right middle or sonic (i=2)
          fact=1.d0+bgas*rm
          cm2=pm*(gam(2)/rm+bgas*(gm1(2)+1.d0/fact))
          if (cm2.le.0.d0) then
            write(6,*)'Error (getfxv) : cm2.le.0'
            ier=1
            return
          end if
          if (isign*vm-dsqrt(cm2).gt.0.d0) then
c sonic left (i=1) or right (i=2)
c            call sonic21d (i,isign,r(i,j),rm,wstar)
            call sonic31d (i,isign,rm,vm,cm2,wstar)
          else
c middle left (i=1) or right (i=2)
            wstar(1)=rm
            wstar(2)=vm
            wstar(3)=pm
          end if
        end if
      end if
c
c compute flux
      fact=wstar(1)*wstar(2)**2
      energy=wstar(3)/(gm1(2)*(1.d0+bgas*wstar(1)))+.5d0*fact
      fx(1)=alfa*wstar(1)*wstar(2)
      fx(2)=alfa*(fact+wstar(3))
      fx(3)=alfa*(energy+wstar(3))*wstar(2)
c
      return
      end
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
      subroutine sonic21d (i,isign,ra,rb,wstar)
c
      implicit real*8 (a-h,o-z)
c      parameter (nmax=20)
c      dimension aint(2,nmax)
      dimension wstar(3)
      common / gasdat1d / gam(2),gm1(2),gp1(2),em(2),ep(2),p0(2),bgas
      common / prmdat1d / alpha(2,2),r(2,2),v(2,2),p(2,2),
     *                  a(2,2),b(2,2),c(2,2)
c      common / romdat / aint0(2),r0(2),c0(2)
c
      data tol / 1.d-4 /
c
      arg=dabs(rb-ra)/(tol*max(ra,rb))
      itmax=dlog(max(arg,1.d-15))/dlog(2.d0)
      itmax=max(itmax,1)
c
      j=2
      facti=1.d0+bgas*r(i,j)
c
c      itmax=50
      do it=1,itmax
c
        rm=.5d0*(ra+rb)
        factm=1.d0+bgas*rm
        pm=p(i,j)*((rm/r(i,j))**gam(j))*factm/facti
     *             *dexp(gm1(j)*bgas*(rm-r(i,j)))
        cm2=pm*(gam(j)/rm+bgas*(gm1(j)+1.d0/factm))
        if (cm2.lt.0.d0) then
          write(6,*)'Error (sonic2) : cm2.lt.0, i=',i
          stop
        end if
        cm=dsqrt(cm2)
        if (rm.le.0.d0) then
          write(6,*)'Error (sonic2) : rm.le.0'
          stop
        end if
        dv=getdv1d(i,rm,cm)
c
c Romberg integration
c        atol=1.d-10
c        dr=rm-r0(i)
c        aint(1,1)=.5d0*(cm/rm+c0(i)/r0(i))*dr
c        do n=2,nmax
c          nm1=n-1
c          sum=0.d0
c          kmax=2**(n-2)
c          do k=1,kmax
c            rk=r0(i)+(k-.5d0)*dr
c            factk=1.d0+bgas*rk
c            pk=p(i,j)*((rk/r(i,j))**gam(j))*factk/facti
c     *                 *dexp(gm1(j)*bgas*(rk-r(i,j)))
c            ck=dsqrt(pk*(gam(j)/rk+bgas*(gm1(j)+1.d0/factk)))
c            sum=sum+ck/rk
c          end do
c          aint(2,1)=.5d0*(aint(1,1)+dr*sum)
c          do l=1,nm1
c            aint(2,l+1)=aint(2,l)+(aint(2,l)-aint(1,l))/(4**l-1.d0)
c          end do
cc          write(56,*)n,aint(1,nm1),aint(2,n)
c          if (dabs(aint(2,n)-aint(1,nm1))/c(i,j).lt.atol) goto 1
c          dr=.5d0*dr
c          do l=1,n
c            aint(1,l)=aint(2,l)
c          end do
c        end do
c
c        write(6,*)'Warning (sonic2) : Romberg did not converge'
c    1   dv=aint(2,n)+aint0(i)
c        aint0(i)=dv
c        r0(i)=rm
c        c0(i)=cm
c
        vm=v(i,j)-isign*dv
        if (isign*vm-cm.gt.0.d0) then
          rb=rm
        else
          ra=rm
        end if
c
      end do
c
c      write(6,*)'sonic: ra,rb =',ra,rb
      wstar(1)=rm
      wstar(2)=vm
      wstar(3)=pm
c
      return
      end
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
      subroutine sonic31d (i,isgn,rm,vm,cm2,wstar)
c
c Use just one step of false position (i.e. a linear fit), much faster.
c
      implicit real*8 (a-h,o-z)
      dimension wstar(3)
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0,bgas
      common / prmdat / alpha(2,2),r(2,2),v(2,2),p(2,2),
     *                  a(2,2),b(2,2),c(2,2)
c
      j=2
      alam0=isgn*v(i,j)-c(i,j)
      alam1=isgn*vm-dsqrt(cm2)
      if (alam0.lt.0.d0.and.alam1.gt.0.d0) then
        rm=r(i,j)-alam0*(rm-r(i,j))/(alam1-alam0)
        facti=1.d0+bgas*r(i,j)
        factm=1.d0+bgas*rm
        pm=p(i,j)*((rm/r(i,j))**gam(j))*factm/facti
     *             *dexp(gm1(j)*bgas*(rm-r(i,j)))
        cm2=pm*(gam(j)/rm+bgas*(gm1(j)+1.d0/factm))
        if (cm2.lt.0.d0) then
          write(6,*)'Error (sonic31d) : cm2.lt.0, i=',i
          stop
        end if
        cm=dsqrt(cm2)
        vm=cm/isgn
        wstar(1)=rm
        wstar(2)=vm
        wstar(3)=pm
      else
        write(6,*)'Error (sonic31d) : no root found'
      end if
c
      return
      end
c
c+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
c
      subroutine getfxv21d (i,alfa,rm,vm,pm,wstar,fx)
c
c compute gas flux for the case when solid contact and gas contact
c are on either side of x=0.  If i=1, then solid contact is to the
c right of x=0, and if i=2, then solid contact is to the left.
c
      implicit double precision (a-h,o-z)
      dimension wstar(3),fx(3)
      common / gasdat1d / gam(2),gm1(2),gp1(2),em(2),ep(2),p0(2),bgas
c
      wstar(1)=rm
      wstar(2)=vm
      wstar(3)=pm
c
c compute flux
      fact=wstar(1)*wstar(2)**2
      energy=wstar(3)/(gm1(2)*(1.d0+bgas*wstar(1)))+.5d0*fact
      fx(1)=alfa*wstar(1)*wstar(2)
      fx(2)=alfa*(fact+wstar(3))
      fx(3)=alfa*(energy+wstar(3))*wstar(2)
c
      return
      end
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
      subroutine getg31d (rm0,rm,vm,pm,g,resid,iconf,ier)
c
      implicit double precision (a-h,o-z)
c      parameter (nmax=20)
c      dimension aint(2,nmax)
      dimension ii(2,2)
      dimension rm(2),vm(2,2),pm(2,2),g(4),dv(2)
      common / gasdat1d / gam(2),gm1(2),gp1(2),em(2),ep(2),p0(2),bgas
      common / prmdat1d / alpha(2,2),r(2,2),v(2,2),p(2,2),
     *                  a(2,2),b(2,2),c(2,2)
      common / scale31d / rm0save,rscale,pscale,vscale,gfact(4)
      common / romdat1d / aint0(2),r0(2),c0(2)
c
      ier=0
c
c compute solid velocities across acoustic fields
      j=1
      do i=1,2
        isign=2*i-3
        if (pm(i,j).gt.p(i,j)) then
          arg=pm(i,j)+b(i,j)
          fact=dsqrt(a(i,j)/arg)
          vm(i,j)=v(i,j)+isign*fact*(pm(i,j)-p(i,j))
          ii(i,j)=1
        else
          arg=(pm(i,j)+p0(j))/(p(i,j)+p0(j))
          fact=2.d0*c(i,j)/gm1(j)
          vm(i,j)=v(i,j)+isign*fact*(arg**em(j)-1.d0)
          ii(i,j)=2
        end if
      end do
c
c      write(6,145)rm0,rm(1),rm(2),vm(1,1),pm(1,1),vm(2,1),pm(2,1)
c  145 format(6(1x,1pe22.15,/),1x,1pe22.15)
c      pause
c
c compute gas velocities and pressures across acoustic fields
      j=2
      do i=1,2
c
        facti=1.d0+bgas*r(i,j)
        factm=1.d0+bgas*rm(i)
c
        if (rm(i).le.r(i,j)) then
          ii(i,j)=2
c
c rarefaction solution
          pm(i,j)=p(i,j)*((rm(i)/r(i,j))**gam(j))*factm/facti
     *                  *dexp(gm1(j)*bgas*(rm(i)-r(i,j)))
          cm2=pm(i,j)*(gam(j)/rm(i)+bgas*(gm1(j)+1.d0/factm))
          if (cm2.lt.0.d0) then
c            write(6,*)'Error (getg3) : cm2.lt.0, i=',i
            ier=1
            return
          end if
          cm=dsqrt(cm2)
          if (rm(i).le.0.d0) then
c            write(6,*)'Error (getg3) : rm(i).le.0'
            ier=2
            return
          end if
          dv(i)=getdv1d(i,rm(i),cm)
c
        else
          ii(i,j)=1
c
c shock solution
          z=r(i,j)/rm(i)
          z2=1.d0-z
          z3=.5d0*gm1(j)*facti*z2
          ratio=facti/factm
          denom=ratio*z-z3
          if (denom.le.0.d0) then
c            write(6,*)'Error (getg3) : denom.le.0'
            ier=5
            return
          end if
          pm(i,j)=p(i,j)*(1.d0+z3)/denom
          dv(i)=dsqrt(p(i,j)*(pm(i,j)/p(i,j)-1.d0)*z2/r(i,j))
c
        end if
c
      end do
c
      vm(1,j)=v(1,j)-dv(1)
      vm(2,j)=v(2,j)+dv(2)
c      write(6,543)vm(1,j),vm(2,j)
c  543 format(2(1x,1pe22.15),/)
c
      v1=vm(1,2)-vm(1,1)
      v2=vm(2,2)-vm(2,1)
      if (iconf.eq.0) then
        if (vm(1,1).gt.vm(1,2)) then
          iconf=-1
        else
          iconf= 1
        end if
      end if
c
c check whether gas contact is to the left or right of solid contact,
c compute constraints across solid contact
      if (iconf.lt.0) then
        jj=1
c gas contact on the left
        const=dexp(gm1(2)*bgas*(rm0save-rm(2)))*(1.d0+bgas*rm0save)
     *        /(1.d0+bgas*rm(2))
        rm0=rm(2)*(pm(1,2)/(const*pm(2,2)))**(1.d0/gam(2))
        fact1=1.d0+bgas*rm0
        fact2=1.d0+bgas*rm(2)
        h1=pm(1,2)*(1.d0+1.d0/(gm1(2)*fact1))/rm0
        h2=pm(2,2)*(1.d0+1.d0/(gm1(2)*fact2))/rm(2)
        g(1)=vm(2,1)-vm(1,1)
        g(2)=alpha(2,2)*rm(2)*v2-alpha(1,2)*rm0*v1
        g(3)= alpha(2,1)*pm(2,1)+alpha(2,2)*(pm(2,2)+rm(2)*v2**2)
     *       -alpha(1,1)*pm(1,1)-alpha(1,2)*(pm(1,2)+rm0*v1**2)
        g(4)=.5d0*v2**2+h2-.5d0*v1**2-h1
      else
        jj=2
c gas contact on the right
        const=dexp(gm1(2)*bgas*(rm0save-rm(1)))*(1.d0+bgas*rm0save)
     *        /(1.d0+bgas*rm(1))
        rm0=rm(1)*(pm(2,2)/(const*pm(1,2)))**(1.d0/gam(2))
        fact1=1.d0+bgas*rm(1)
        fact2=1.d0+bgas*rm0
        h1=pm(1,2)*(1.d0+1.d0/(gm1(2)*fact1))/rm(1)
        h2=pm(2,2)*(1.d0+1.d0/(gm1(2)*fact2))/rm0
        g(1)=vm(2,1)-vm(1,1)
        g(2)=alpha(2,2)*rm0*v2-alpha(1,2)*rm(1)*v1
        g(3)= alpha(2,1)*pm(2,1)+alpha(2,2)*(pm(2,2)+rm0*v2**2)
     *       -alpha(1,1)*pm(1,1)-alpha(1,2)*(pm(1,2)+rm(1)*v1**2)
        g(4)=.5d0*v2**2+h2-.5d0*v1**2-h1
      end if
c
c      write(6,567)ii(1,1),ii(2,1),ii(1,2),ii(2,2),jj
c  567 format('** structure=',5(1x,i1))
c      write(6,345)(g(i),i=1,5)
c  345 format('** g=',5(/,1x,1pe22.15))
c      pause
c
c scale residual
      resid=0.d0
      do i=1,4
        g(i)=gfact(i)*g(i)
        resid=max(dabs(g(i)),resid)
      end do
c
      return
      end
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
      subroutine newton31d (rm0,rm,vm,pm,g,iconf,ier)
c
      implicit double precision (a-h,o-z)
      dimension rm(2),vm(2,2),pm(2,2),g(4),dg(4,4),dg0(4,4)
c      logical ctest
      common / gasdat1d / gam(2),gm1(2),gp1(2),em(2),ep(2),p0(2),bgas
      common / prmdat1d / alpha(2,2),r(2,2),v(2,2),p(2,2),
     *                  a(2,2),b(2,2),c(2,2)
      common / scale31d / rm0save,rscale,pscale,vscale,gfact(4)
      data pfact, tol, itmax / 1.d-4, 1.d-10, 15 /
c
      ier=0
c
c Newton iteration to find rm0, rm (gas densities) and pm(:,1) (solid pressures)
      it=0
    1 it=it+1
c
c get Jacobian matrix
        if (it.le.100) then
c finite differences
c        call getdg31d (rm0,rm,vm,pm,g,dg,iconf,ier)
c analytic
        call getdga31d (rm0,rm,vm,pm,dg,iconf,ier)
        if (ier.ne.0) then
c          write(6,*)'Error (newton3) : cannot compute jacobian'
          ier=9
          return
        end if
c analytic
c       call not available
        do i=1,4
          do j=1,4
            dg0(i,j)=dg(i,j)
          end do
        end do
        else
        do i=1,4
          do j=1,4
            dg(i,j)=dg0(i,j)
          end do
        end do
        end if
c
c compute correction, overwrite
        call solve1d (dg,g,ier)
        if (ier.ne.0) then
c          write(6,*)'Error (newton3) : linear system not solvable'
          ier=1
          return
        end if
c
c update
cc        write(6,320)it,rm0,g(1),rm(1),g(2),rm(2),g(3),
cc     *              pm(1,1),g(4),pm(2,1),g(5)
cc  320   format(' it =',i2,5(/,4x,2(1x,1pe15.8)))
        rm(1)=rm(1)-g(1)
        rm(2)=rm(2)-g(2)
        pm(1,1)=pm(1,1)-g(3)
        pm(2,1)=pm(2,1)-g(4)
c
        if (rm(1).le.0.d0.or.rm(2).le.0.d0) then
c          write(6,*)'Error (newton3) : negative gas densities'
          ier=3
          return
        end if
c
        if (pm(1,1).le.0.d0.or.pm(2,1).le.0.d0) then
c          write(6,*)'Error (newton3) : negative solid pressures'
          ier=4
          return
        end if
c
c error
        err=0.d0
        do k=1,2
          err=max(dabs(g(k))/rscale,err)
        end do
        do k=3,4
          err=max(dabs(g(k))/pscale,err)
        end do
c
c        write(57,300)it,err,(g(i),i=1,5)
c  300   format('     * errors for Newton iteration, it,err=',i2,
c     *         1x,1pe9.2,/,4x,5(1x,1pe9.2))
c
c check for convergence
      if (err.gt.tol) then
        if (it.lt.itmax) then
          call getg31d (rm0,rm,vm,pm,g,resid,iconf,ier)
          if (ier.ne.0) return
          goto 1
        else
c          write(6,*)'Error (newton3) : iteration did not converge'
          ier=2
          return
        end if
      end if
c
c check for NaNs
      zero=0.d0
      if (zero*rm0.ne.zero) goto 2
      do i=1,2
        if (zero*rm(i).ne.zero) goto 2
        do j=1,2
          if (zero*vm(i,j).ne.zero.or.zero*pm(i,j).ne.zero) goto 2
        end do
      end do
c
c check the solution configuration.
      if (iconf.lt.0) then
c
c gas contact on the left
        fact=1.d0+bgas*rm(2)
        cm2=pm(2,2)*(gam(2)/rm(2)+bgas*(gm1(2)+1.d0/fact))
        if (cm2.le.0.d0) then
c          write(6,*)'Error (newton3) : cm2.le.0'
          ier=11
          return
        end if
        if (vm(2,1)-vm(2,2).gt.dsqrt(cm2)) then
c          write(6,*)'Error (newton3) : supersonic solution found'
          ier=12
          return
        end if
        fact=1.d0+bgas*rm0
        cm2=pm(1,2)*(gam(2)/rm0+bgas*(gm1(2)+1.d0/fact))
        if (cm2.le.0.d0) then
c          write(6,*)'Error (newton3) : cm2.le.0'
          ier=13
          return
        end if
        if (vm(1,1)-vm(1,2).gt.dsqrt(cm2)) then
c          write(6,*)'Error (newton3) : supersonic solution found'
          ier=14
          return
        end if
c
      else
c
c gas contact on the right
        fact=1.d0+bgas*rm0
        cm2=pm(2,2)*(gam(2)/rm0+bgas*(gm1(2)+1.d0/fact))
        if (cm2.le.0.d0) then
c          write(6,*)'Error (newton3) : cm2.le.0'
          ier=21
          return
        end if
        if (vm(2,2)-vm(2,1).gt.dsqrt(cm2)) then
c          write(6,*)'Error (newton3) : supersonic solution found'
          ier=22
          return
        end if
        fact=1.d0+bgas*rm(1)
        cm2=pm(1,2)*(gam(2)/rm(1)+bgas*(gm1(2)+1.d0/fact))
        if (cm2.le.0.d0) then
c          write(6,*)'Error (newton3) : cm2.le.0'
          ier=23
          return
        end if
        if (vm(1,2)-vm(1,1).gt.dsqrt(cm2)) then
c          write(6,*)'Error (newton3) : supersonic solution found'
          ier=24
          return
        end if
c
      end if
c
      return
c
    2 ier=5
c      write(6,*)'Error (newton3) : NaN detected'
c
      return
      end
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
      subroutine getdg31d (rm0,rm,vm,pm,g,dg,iconf,ier)
c
      implicit double precision (a-h,o-z)
      dimension rm(2),vm(2,2),pm(2,2),g(4),dg(4,4)
      dimension rmp(2),vmp(2,2),pmp(2,2),gp(4)
c      data delta / 1.d-4 /
      data delta / 1.d-8 /      ! Chris says that this is best
c
      ier=0
c
      do i=1,2
        rmp(i)=rm(i)
        do j=1,2
          vmp(i,j)=vm(i,j)
          pmp(i,j)=pm(i,j)
        end do
      end do
c
      do k=1,2
        rmp(k)=rm(k)*(1.d0+delta)
        call getg31d (rm0p,rmp,vmp,pmp,gp,resid,iconf,ier)
        if (ier.ne.0) return
        do i=1,5
          dg(i,k)=(gp(i)-g(i))/(rm(k)*delta)
        end do
        rmp(k)=rm(k)
      end do
c
      do k=1,2
        pmp(k,1)=pm(k,1)*(1.d0+delta)
        call getg31d (rm0p,rmp,vmp,pmp,gp,resid,iconf,ier)
        if (ier.ne.0) return
        do i=1,5
          dg(i,k+2)=(gp(i)-g(i))/(pm(k,1)*delta)
        end do
        pmp(k,1)=pm(k,1)
      end do
c
      return
      end
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
      subroutine getdga31d (rm0,rm,vm,pm,dg,iconf,ier)
c
      implicit double precision (a-h,o-z)
      dimension rm(2),vm(2,2),pm(2,2),dg(4,4)
      dimension fp(2,2),gp(2),dv(2)
      common / gasdat1d / gam(2),gm1(2),gp1(2),em(2),ep(2),p0(2),bgas
      common / prmdat1d / alpha(2,2),r(2,2),v(2,2),p(2,2),
     *                  a(2,2),b(2,2),c(2,2)
      common / scale31d / rm0save,rscale,pscale,vscale,gfact(4)
c
c compute solid quantities (vm and fp)
      j=1
      do i=1,2
        if (pm(i,j).gt.p(i,j)) then
          arg=pm(i,j)+b(i,j)
          fact=dsqrt(a(i,j)/arg)
          diff=pm(i,j)-p(i,j)
          dv(i)=fact*diff
          fp(i,j)=fact*(1.d0-0.5d0*diff/arg)
        else
          arg=(pm(i,j)+p0(j))/(p(i,j)+p0(j))
          fact=2.d0*c(i,j)/gm1(j)
          dv(i)=fact*(arg**em(j)-1.d0)
          fp(i,j)=1.d0/(r(i,j)*c(i,j)*arg**ep(j))
        end if
      end do
      vm(1,j)=v(1,j)-dv(1)
      vm(2,j)=v(2,j)+dv(2)
c
c compute gas quantities (pm, vm, fp, and gp)
      j=2
      do i=1,2
        facti=1.d0+bgas*r(i,j)
        factm=1.d0+bgas*rm(i)
        if (rm(i).le.r(i,j)+1.d-8) then
          pm(i,j)=p(i,j)*((rm(i)/r(i,j))**gam(j))*factm/facti
     *                            *dexp(gm1(j)*(factm-facti))
          cm2=pm(i,j)*(gam(j)/rm(i)+bgas*(gm1(j)+1.d0/factm))
          if (cm2.le.0.d0) then
            write(6,*)'Error (getdga3) : cm2.le.0'
            ier=102
            return
          end if
          cm=dsqrt(cm2)
          dv(i)=getdv1d(i,rm(i),cm)
          gp(i)=cm2
          fp(i,j)=cm/rm(i)
        else
          z=r(i,j)/rm(i)
          z2=1.d0-z
          z3=.5d0*gm1(j)*facti*z2
          ratio=facti/factm
          denom=ratio*z-z3
          if (denom.le.1d-14) then
            write(6,*)'Error (getdga3) : denom.le.0'
            ier=103
            return
          end if
          pm(i,j)=p(i,j)*(1.d0+z3)/denom
          dv(i)=dsqrt(p(i,j)*(pm(i,j)/p(i,j)-1.d0)*z2/r(i,j))
          gp(i)=ratio*(z+bgas*r(i,j)/factm)+.5d0*gm1(j)*facti*z
     *            *(1.d0+ratio+bgas*rm(i)*ratio*z2/factm)
          gp(i)=(p(i,j)/rm(i))*gp(i)/denom**2
          fp(i,j)=.5d0*dv(i)*(gp(i)/(pm(i,j)-p(i,j))+z/(rm(i)-r(i,j)))
        end if
      end do
      vm(1,j)=v(1,j)-dv(1)
      vm(2,j)=v(2,j)+dv(2)
c
c define some convenient quantities
      facts=1.d0+bgas*rm0save
      do i=1,2
        dv(i)=vm(i,2)-vm(i,1)
      end do
c
c compute the elements of the Jacobian matrix
      dg(1,1)=0.d0
      dg(1,2)=0.d0
      dg(1,3)=fp(1,1)
      dg(1,4)=fp(2,1)
      dg(4,3)=-dv(1)*fp(1,1)
      dg(4,4)=-dv(2)*fp(2,1)
c
c      if (vm(1,2).gt.vm(1,1)) then
      if (iconf.gt.0) then
        factm=1.d0+bgas*rm(1)
        frho=dexp(gm1(2)*(facts-factm))*facts/factm
        r0=rm(1)*(pm(2,2)/(frho*pm(1,2)))**(1.d0/gam(2))
        fact0=1.d0+bgas*r0
        f0=gam(2)+gm1(2)*bgas*r0
        fm=gam(2)+gm1(2)*bgas*rm(1)
        q1=1.d0/rm(1)-frho*(gp(1)-bgas*fm*pm(1,2)/factm)/
     *                                      (gam(2)*frho*pm(1,2))
        q2=dv(2)*gp(2)/(gam(2)*pm(2,2))
        c0sq=pm(2,2)*(gam(2)/r0+bgas*(gm1(2)+1.d0/fact0))
        cmsq=pm(1,2)*(gam(2)/rm(1)+bgas*(gm1(2)+1.d0/factm))
c
        dg(2,1)=alpha(2,2)*r0*dv(2)*q1-alpha(1,2)*(dv(1)-rm(1)*fp(1,2))
        dg(2,2)=alpha(2,2)*r0*(fp(2,2)+q2)
        dg(2,3)=-alpha(1,2)*rm(1)*fp(1,1)
        dg(2,4)=-alpha(2,2)*r0*fp(2,1)
        dg(3,1)=alpha(2,2)*r0*(dv(2)**2.d0)*q1-
     *          alpha(1,2)*(gp(1)+dv(1)*(dv(1)-2.d0*rm(1)*fp(1,2)))
        dg(3,2)=alpha(2,2)*(gp(2)+r0*dv(2)*(2.d0*fp(2,2)+q2))
        dg(3,3)=-alpha(1,1)-2.d0*alpha(1,2)*rm(1)*dv(1)*fp(1,1)
        dg(3,4)=alpha(2,1)-2.d0*alpha(2,2)*r0*dv(2)*fp(2,1)
        dg(4,1)=dv(1)*fp(1,2)-c0sq*q1/(gm1(2)*fact0)-
     *          (fm*gp(1)-cmsq)/(gm1(2)*rm(1)*factm)
        dg(4,2)=dv(2)*fp(2,2)+(f0-r0*c0sq/
     *          (gam(2)*pm(2,2)))*gp(2)/(gm1(2)*r0*fact0)
      else
        factm=1.d0+bgas*rm(2)
        frho=dexp(gm1(2)*(facts-factm))*facts/factm
        r0=rm(2)*(pm(1,2)/(frho*pm(2,2)))**(1.d0/gam(2))
        fact0=1.d0+bgas*r0
        f0=gam(2)+gm1(2)*bgas*r0
        fm=gam(2)+gm1(2)*bgas*rm(2)
        q1=1.d0/rm(2)-frho*(gp(2)-bgas*fm*pm(2,2)/factm)/
     *                                      (gam(2)*frho*pm(2,2))
        q2=dv(1)*gp(1)/(gam(2)*pm(1,2))
        c0sq=pm(1,2)*(gam(2)/r0+bgas*(gm1(2)+1.d0/fact0))
        cmsq=pm(2,2)*(gam(2)/rm(2)+bgas*(gm1(2)+1.d0/factm))
c
        dg(2,1)=alpha(1,2)*r0*(fp(1,2)-q2)
        dg(2,2)=-alpha(1,2)*r0*dv(1)*q1+alpha(2,2)*(dv(2)+rm(2)*fp(2,2))
        dg(2,3)=-alpha(1,2)*r0*fp(1,1)
        dg(2,4)=-alpha(2,2)*rm(2)*fp(2,1)
        dg(3,1)=alpha(1,2)*(-gp(1)+r0*dv(1)*(2.d0*fp(1,2)-q2))
        dg(3,2)=-alpha(1,2)*r0*(dv(1)**2.d0)*q1+
     *          alpha(2,2)*(gp(2)+dv(2)*(dv(2)+2.d0*rm(2)*fp(2,2)))
        dg(3,3)=-alpha(1,1)-2.d0*alpha(1,2)*r0*dv(1)*fp(1,1)
        dg(3,4)=alpha(2,1)-2.d0*alpha(2,2)*rm(2)*dv(2)*fp(2,1)
        dg(4,1)=dv(1)*fp(1,2)-(f0-r0*c0sq/
     *          (gam(2)*pm(1,2)))*gp(1)/(gm1(2)*r0*fact0)
        dg(4,2)=dv(2)*fp(2,2)+c0sq*q1/(gm1(2)*fact0)+
     *          (fm*gp(2)-cmsq)/(gm1(2)*rm(2)*factm)
      end if
c
      do i=1,4
        do j=1,4
          dg(i,j)=gfact(i)*dg(i,j)
        end do
      end do
c
      return
      end
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
      subroutine middle1d (j,wl,wr,pm,vm,dv,ier)
c
c Approximate-state Riemann solver, see Toro
c
      implicit double precision (a-h,o-z)
      dimension wl(3),wr(3),pm(2),vm(2),dv(2)
      dimension vdif(2,2)
      common / gasdat1d / gam(2),gm1(2),gp1(2),em(2),ep(2),p0(2),bgas
      common / prmdat1d / alpha(2,2),r(2,2),v(2,2),p(2,2),
     *                  a(2,2),b(2,2),c(2,2)
c     data pratio, ptol / 2.d0, 1.d-6 /
      data pratio, pfact / 2.d0, 1.d-4 /
      data tol, itmax / 1.d-3, 6 /
c
      ier=0
c
c minimum pressure tolerance
c      ptol=pfact*min(wl(3),wr(3))
c
c minimum pressure tolerance, assumes p0(2)=0
      if (j.eq.1) then
        ptol=-(1.d0-pfact)*p0(1)
      else
        ptol=pfact*min(wl(3),wr(3))
      end if
c
c left primitive state variables
      r(1,j)=wl(1)
      v(1,j)=wl(2)
      p(1,j)=wl(3)
c
c right primitive state variables
      r(2,j)=wr(1)
      v(2,j)=wr(2)
      p(2,j)=wr(3)
c
c sound speeds and some constants
      do i=1,2
        a(i,j)=2.d0/(gp1(j)*r(i,j))
        b(i,j)=gm1(j)*(p(i,j)+p0(j))/gp1(j)+p0(j)
        c2=gam(j)*(p(i,j)+p0(j))/r(i,j)
        if (c2.le.0.d0) then
          write(6,*)'Error (middle) : c2.le.0, i,j =',i,j
          ier=1
          return
        end if
        c(i,j)=dsqrt(c2)
      end do
c
c check for vacuum state (in the decoupled phases)
      if (2.d0*(c(1,j)+c(2,j))/gm1(j).le.v(2,j)-v(1,j)) then
        write(6,*)'Error (middle) : vacuum found, j=',j
        ier=2
        return
      end if
c
c compute min/max pressures
c      pmin=min(p(1,j),p(2,j))
c      pmax=max(p(1,j),p(2,j))
      pmin=min(p(1,j)+p0(j),p(2,j)+p0(j))
      pmax=max(p(1,j)+p0(j),p(2,j)+p0(j))
c
c start with guess based on a linearization
      ppv=.5d0*(p(1,j)+p(2,j))
     *    -.125d0*(v(2,j)-v(1,j))*(r(1,j)+r(2,j))*(c(1,j)+c(2,j))
c      ppv=max(ppv,0.d0)
      ppv=max(ppv+p0(j),0.d0)
c
c      if ((pmax+p0(j))/(pmin+p0(j)).le.pratio
c     *   .and.pmin.le.ppv.and.pmax.ge.ppv) then
      if (pmax/pmin.le.pratio
     *   .and.pmin.le.ppv.and.pmax.ge.ppv) then
c        pstar=ppv
        pstar=ppv-p0(j)
      else
        if (ppv.lt.pmin) then
c guess based on two rarefaction solution
          arg1=c(1,j)/((p(1,j)+p0(j))**em(j))
          arg2=c(2,j)/((p(2,j)+p0(j))**em(j))
          arg3=(c(1,j)+c(2,j)-.5d0*gm1(j)*(v(2,j)-v(1,j)))/(arg1+arg2)
          pstar=arg3**(1.d0/em(j))-p0(j)
        else
c guess based on two shock approximate solution
          gl=dsqrt(a(1,j)/(ppv+b(1,j)))
          gr=dsqrt(a(2,j)/(ppv+b(2,j)))
          pts=(gl*p(1,j)+gr*p(2,j)-v(2,j)+v(1,j))/(gl+gr)
          pstar=max(ptol,pts)
        end if
      end if
c
c get middle pstar state
      do it=1,itmax
c
c determine velocity difference across a shock or rarefaction
        do i=1,2
          if (pstar.gt.p(i,j)) then
            arg=pstar+b(i,j)
            fact=dsqrt(a(i,j)/arg)
            diff=pstar-p(i,j)
            vdif(1,i)=fact*diff
            vdif(2,i)=fact*(1.d0-0.5d0*diff/arg)
          else
            arg=(pstar+p0(j))/(p(i,j)+p0(j))
            fact=2.d0*c(i,j)/gm1(j)
            vdif(1,i)=fact*(arg**em(j)-1.d0)
            vdif(2,i)=1.d0/(r(i,j)*c(i,j)*arg**ep(j))
          end if
        end do
c
c   determine change to pressure in the middle state
        dp=(vdif(1,1)+vdif(1,2)+v(2,j)-v(1,j))/(vdif(2,1)+vdif(2,2))
        pstar=pstar-dp
c
        if (dabs(dp)/(pstar+p0(j)).lt.tol) goto 1
c
      end do
c
c print warning if middle pstar state is not converged
      write(6,*)'Warning (middle) : pstar not converged'
c
c assign middle pressures
    1 pm(1)=pstar
      pm(2)=pm(1)
c
c assign middle velocity
      vm(1)=.5d0*(v(1,j)-vdif(1,1)+v(2,j)+vdif(1,2))
      vm(2)=vm(1)
c
c assign dv/dp
      dv(1)=vdif(2,1)
      dv(2)=vdif(2,2)
c
      return
      end
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
      subroutine couple1d (m,pm,vm,dv,wl,wr,fl,fr,method,ier)
c
      implicit double precision (a-h,o-z)
      dimension pm(2,2),vm(2,2),dv(2,2),wl(m),wr(m),fl(m),fr(m)
      dimension g(4),pm0(2,2),vm0(2,2),dummy(2,2)
      logical ctest
      common / gasdat1d / gam(2),gm1(2),gp1(2),em(2),ep(2),p0(2),bgas
      common / prmdat1d / alpha(2,2),r(2,2),v(2,2),p(2,2),
     *                  a(2,2),b(2,2),c(2,2)
      common / scales1d / pscale,vscale,gfact(4)
      common / cntdat1d / icnt(4)
c     data sig, frac, tol / 10.d0, .99d0, 1.d-6 /
      data sig, frac, tol / 10.d0, .99d0, 1.d-10 /
c
      ier=0
c
      if (dabs(wr(1)-wl(1)).gt.1.d-14) then
c
c icnt(2) counts the total number of coupled flux calculations
        icnt(2)=icnt(2)+1
c
c compute gas density
        dvm=vm(1,2)-vm(1,1)
        if (dvm.ge.0.d0) then
          if (pm(1,2).gt.p(1,2)) then
            rm=r(1,2)*(gp1(2)*(pm(1,2)+p0(2))/(p(1,2)+p0(2))+gm1(2))
     *           /(gm1(2)*(pm(1,2)+p0(2))/(p(1,2)+p0(2))+gp1(2))
          else
            rm=r(1,2)*((pm(1,2)+p0(2))/(p(1,2)+p0(2)))**(1.d0/gam(2))
          end if
        else
          if (pm(2,2).gt.p(2,2)) then
            rm=r(2,2)*(gp1(2)*(pm(2,2)+p0(2))/(p(2,2)+p0(2))+gm1(2))
     *               /(gm1(2)*(pm(2,2)+p0(2))/(p(2,2)+p0(2))+gp1(2))
          else
            rm=r(2,2)*((pm(2,2)+p0(2))/(p(2,2)+p0(2)))**(1.d0/gam(2))
          end if
        end if
c
c compute pressure and velocity scales
        pscale=max(pm(1,1),pm(1,2))
        vscale=dsqrt(gam(2)*(pm(1,2)+p0(2))/rm)
        gfact(1)=1.d0/vscale
        gfact(2)=gfact(1)/pscale**(1.d0/gam(2))
        gfact(3)=1.d0/pscale
        gfact(4)=gfact(1)**2
c
c "middle" value for alpha to linearize about
c       asm=.5d0*(alpha(1,1)+alpha(2,1))
        if (alpha(1,1).gt.0.5d0) then
          if (alpha(2,1).lt.0.5d0) then
            asm=.5d0
          else
            asm=min(alpha(1,1),alpha(2,1))
          end if
        else
          if (alpha(2,1).gt.0.5d0) then
            asm=.5d0
          else
            asm=max(alpha(1,1),alpha(2,1))
          end if
        end if
        agm=1.d0-asm
c
c linear update for middle solid pressures
        eps=alpha(2,1)-alpha(1,1)
        del=(pm(1,2)-pm(1,1))*eps/(asm*(dv(1,1)+dv(2,1)))
        pm(1,1)=pm(1,1)-dv(2,1)*del
        pm(2,1)=pm(2,1)+dv(1,1)*del
c
c limit velocity difference
        cm=frac*dsqrt(gam(2)*pm(1,2)/rm)
        arg1=sig*(1.d0+dvm/cm)
        arg2=sig*(1.d0-dvm/cm)
        dvm=.5d0*cm*log(cosh(arg1)/cosh(arg2))/sig
c
c linear update for middle gas pressures
        del=gam(2)*(pm(1,2)+p0(2))*dvm*eps
     *      /(agm*(gam(2)*(pm(1,2)+p0(2))-rm*dvm**2)*(dv(1,2)+dv(2,2)))
        pm(1,2)=pm(1,2)+(rm*dvm*dv(2,2)+1.d0)*del
        pm(2,2)=pm(2,2)-(rm*dvm*dv(1,2)-1.d0)*del
c
c compute residual of the jump conditions
        call getg1d (pm,vm,dummy,g,resid)
c
c if residual is too big, then do Newton
        if (resid.gt.tol) then
c
c save current middle pressures and velocities
          do i=1,2
            do j=1,2
              pm0(i,j)=pm(i,j)
              vm0(i,j)=vm(i,j)
            end do
          end do
c
          call newton1d (pm,vm,ctest)
c
c reset middle pressures
          if (.not.ctest) then
c            write(6,*)'ctest is false'
c            pause
            do i=1,2
              do j=1,2
                pm(i,j)=pm0(i,j)
                vm(i,j)=vm0(i,j)
              end do
            end do
          end if
c
        end if
c
      end if
c
c now compute fluxes
c
c solid contact source contributions
      if (method.eq.0) then
        source=pm(2,1)*alpha(2,1)-pm(1,1)*alpha(1,1)
      else
        source=0.d0
      end if
      source2=vm(1,1)*source
c
c check if solid contact is to the right or left of x=0
      if (vm(1,1).gt.0.d0) then
c
c right-of-solid-contact state (only need solid velocity and gas pressure)
        wr(3)=vm(1,1)
        wr(7)=pm(2,2)
c
c solid volume fraction flux
        if (method.eq.0) then
          fl(1)= 0.d0
          fr(1)=-vm(1,1)*(alpha(2,1)-alpha(1,1))
        else
          fl(1)=0.d0
          fr(1)=0.d0
        end if
c
c solid phase:
        call getfx1d (1,1,alpha(1,1),pm(1,1),vm(1,1),wl(2),fl(2))
c
c add on solid contact source contributions
        fr(2)=fl(2)
        fr(3)=fl(3)+source
        fr(4)=fl(4)+source2
c
c gas phase:
        if (vm(1,2).ge.0.d0) then
c gas contact (and solid contact) to the right
          call getfx1d (1,2,alpha(1,2),pm(1,2),vm(1,2),wl(5),fl(5))
        else
c gas contact to the left, solid contact to the right
          call getfx21d (1,alpha(1,2),pm(1,2),vm(1,2),wl(5),fl(5))
        end if
c
c add on solid contact source contributions
        fr(5)=fl(5)
        fr(6)=fl(6)-source
        fr(7)=fl(7)-source2
c
        if (method.ne.0) then
          do i=1,m
            wr(i)=wl(i)
          end do
        end if
c
      else
c
c left-of-solid-contact state (only need solid velocity and gas pressure)
        wl(3)=vm(1,1)
        wl(7)=pm(1,2)
c
c solid volume fraction flux
        if (method.eq.0) then
          fl(1)= vm(1,1)*(alpha(2,1)-alpha(1,1))
          fr(1)= 0.d0
        else
          fl(1)=0.d0
          fr(1)=0.d0
        end if
c
c solid phase:
        call getfx1d (2,1,alpha(2,1),pm(2,1),vm(2,1),wr(2),fr(2))
c
c add on solid contact source contributions
        fl(2)=fr(2)
        fl(3)=fr(3)-source
        fl(4)=fr(4)-source2
c
c gas phase:
        if (vm(2,2).le.0.d0) then
c gas contact (and solid contact) to the left
          call getfx1d (2,2,alpha(2,2),pm(2,2),vm(2,2),wr(5),fr(5))
        else
c gas contact to the right, solid contact to the left
          call getfx21d (2,alpha(2,2),pm(1,2),vm(2,2),wr(5),fr(5))
        end if
c
c add on solid contact source contributions
        fl(5)=fr(5)
        fl(6)=fr(6)+source
        fl(7)=fr(7)+source2
c
        if (method.ne.0) then
          do i=1,m
            wl(i)=wr(i)
          end do
        end if
c
      end if
c
      return
      end
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
      subroutine newton1d (pm,vm,ctest)
c
      implicit double precision (a-h,o-z)
      dimension pm(2,2),vm(2,2),rm(2),g(4),dg(4,4)
      logical ctest
      common / gasdat1d / gam(2),gm1(2),gp1(2),em(2),ep(2),p0(2),bgas
      common / prmdat1d / alpha(2,2),r(2,2),v(2,2),p(2,2),
     *                  a(2,2),b(2,2),c(2,2)
      common / scales1d / pscale,vscale,gfact(4)
      data pfact, tol, itmax / 1.d-4, 1.d-10, 15 /
c
c solid pressure is used to compute source
      source0=pm(2,1)*alpha(2,1)-pm(1,1)*alpha(1,1)
c
c Newton iteration to find pm
      it=0
    1 it=it+1
c
c get right-hand-side vector
        call getg1d (pm,vm,rm,g,resid)
c
c get Jacobian matrix (analytic)
        call getdga1d (dg,pm,vm,rm)
c
c get Jacobian matrix (finite difference)
c        call getdgf1d (pm,dg,g)
c
c compute correction, overwrite
        call solve1d (dg,g,ier)
        if (ier.ne.0) then
          ctest=.false.
          return
        end if
c
c update pm
        pm(1,1)=pm(1,1)-g(1)
        pm(2,1)=pm(2,1)-g(2)
c
c solid pressures may be negative so these first two are
c commented out - might re-think this...
c        pm(1,1)=dmax1(pm(1,1)-g(1),pfact*pm(1,1))
c        pm(2,1)=dmax1(pm(2,1)-g(2),pfact*pm(2,1))
        pm(1,2)=dmax1(pm(1,2)-g(3),pfact*pm(1,2))
        pm(2,2)=dmax1(pm(2,2)-g(4),pfact*pm(2,2))
c
c relative error in the source calculation
        source=pm(2,1)*alpha(2,1)-pm(1,1)*alpha(1,1)
        err1=dabs(source-source0)/min(pm(1,1)+p0(1),pm(2,1)+p0(1))
c
c relative error in the solid pressures
        err2=max(dabs(g(1))/(pm(1,1)+p0(1)),dabs(g(2))/(pm(2,1)+p0(1)))
c
c relative error in the gas pressures
        err3=max(dabs(g(3))/pm(1,2),dabs(g(4))/pm(2,2))
c
c       write(6,*)'it,err1,err2,err3=',it,err1,err2,err3
c
c check for convergence
      if (max(err1,err2,err3).gt.tol) then
        if (it.lt.itmax) then
          source0=source
          goto 1
        else
          ctest=.false.
          return
        end if
      end if
c
c if converged, then check solution
      call soltest1d (pm,vm,ctest)
c
      return
      end
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
      subroutine solve1d (dg,g,ier)
c
      implicit double precision (a-h,o-z)
      dimension dg(4,4),g(4),a(4,5)
      data tol / 1.d-12 /
c
      ier=0
c
c set up augmented matrix a=[dg,g] and compute Frobenius norm of dg
      anorm=0.d0
      do i=1,4
        do j=1,4
          a(i,j)=dg(i,j)
          anorm=anorm+dg(i,j)**2
        end do
        a(i,5)=g(i)
      end do
      anorm=dsqrt(anorm)
c
c tolerance for singular system
      atol=anorm*tol
c
c Gaussian elimination with partial pivoting
      do k=1,3
        kpiv=k
        apiv=dabs(a(k,k))
        do i=k+1,4
          if (dabs(a(i,k)).gt.apiv) then
            kpiv=i
            apiv=dabs(a(i,k))
          end if
        end do
        if (kpiv.ne.k) then
          do j=k,5
            atmp=a(k,j)
            a(k,j)=a(kpiv,j)
            a(kpiv,j)=atmp
          end do
        end if
        if (dabs(a(k,k)).lt.atol) then
          ier=1
          return
        end if
        do i=k+1,4
          fact=a(i,k)/a(k,k)
          do j=k+1,5
            a(i,j)=a(i,j)-fact*a(k,j)
          end do
        end do
      end do
c
c backward substitution
      g(4)=a(4,5)/a(4,4)
      do i=3,1,-1
        sum=a(i,5)
        do j=i+1,4
          sum=sum-a(i,j)*g(j)
        end do
        g(i)=sum/a(i,i)
      end do
c
      return
      end
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
      subroutine getguess1d (m,alpha,pm,pm0,vm,alphi,alphp,admin)
c
c     improves the initial guess by a continuous extension
c
      implicit double precision (a-h,o-z)
      dimension alpha(2,2),pm(2,2),pm0(2,2),vm(2,2)
      logical ctest
c
      kc=1
      if (dabs(alpha(2,1)-5.0d-1).gt.dabs(alpha(1,1)-5.0d-1)) kc=2
      alfa=alpha(kc,1)
c
      ctest=.false.
      do while (.not.ctest)
        ctest=.true.
c      
        alpha(kc,1)=alphi
        alpha(kc,2)=1.0d0-alphi
c
        call newton1d (pm,vm,ctest)
c
        if (.not.ctest) then
          do i=1,2
            do j=1,2
              pm(i,j)=pm0(i,j)
            end do
          end do
          alphi=(alphp+alphi)/2.0d0
          alpha(kc,1)=alphi
          alpha(kc,2)=1.0d0-alphi
          if (dabs(alphi-alphp).lt.admin) then
            ctest=.true.
            return
          endif
        end if
c          
      end do
c
      do i=1,2
        do j=1,2
          pm0(i,j)=pm(i,j)
        end do
      end do
c
      alpha(kc,1)=alfa
      alpha(kc,2)=1.0d0-alfa
c
      alphp=alphi
      alphi=(alphi+alfa)/2.0d0
c
      return
      end
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
      subroutine soltest1d (pm,vm,ctest)
c
      implicit double precision (a-h,o-z)
      dimension pm(2,2),vm(2,2),sp(3,2)
      logical ctest
      common / gasdat1d / gam(2),gm1(2),gp1(2),em(2),ep(2),p0(2),bgas
      common / prmdat1d / alpha(2,2),r(2,2),v(2,2),p(2,2),
     *                  a(2,2),b(2,2),c(2,2)
      data tiny / 1.d-8 /
c
      if (vm(1,1).lt.vm(1,2)) then
        sp(2,2)=vm(2,2)
      else
        sp(2,2)=vm(1,2)
      end if
      sp(2,1)=vm(1,1)
c
      j=1
      if (pm(1,j).gt.p(1,j)) then
        sp(1,j)=v(1,j)-c(1,j)*dsqrt(ep(j)*(pm(1,j)+p0(j))
     *                                    /(p(1,j)+p0(j))+em(j))
        if (sp(1,j)-tiny.gt.v(1,j)-c(1,j)) then
          ctest=.false.
          return
        end if
      else
        rm=r(1,j)*((pm(1,j)+p0(j))/(p(1,j)+p0(j)))**(1.d0/gam(j))
        sp(1,j)=vm(1,j)-dsqrt(gam(j)*(pm(1,j)+p0(j))/rm)
        if (sp(1,j)+tiny.lt.v(1,j)-c(1,j)) then
          ctest=.false.
          return
        end if
      end if
      if (pm(2,j).gt.p(2,j)) then
        sp(3,j)=v(2,j)+c(2,j)*dsqrt(ep(j)*(pm(2,j)+p0(j))
     *                                    /(p(2,j)+p0(j))+em(j))
        if (sp(3,j)+tiny.lt.v(2,j)+c(2,j)) then
          ctest=.false.
          return
        end if
      else
        rm=r(2,j)*((pm(2,j)+p0(j))/(p(2,j)+p0(j)))**(1.d0/gam(j))
        sp(3,j)=vm(2,j)+dsqrt(gam(j)*(pm(2,j)+p0(j))/rm)
        if (sp(3,j)-tiny.gt.v(2,j)+c(2,j)) then
          ctest=.false.
          return
        end if
      end if
c
      j=2
      if (pm(1,j).gt.p(1,j)) then
        sp(1,j)=v(1,j)-c(1,j)*dsqrt(ep(j)*(pm(1,j)+p0(j))
     *                                    /(p(1,j)+p0(j))+em(j))
        if (sp(1,j)-tiny.gt.v(1,j)-c(1,j)) then
          ctest=.false.
          return
        end if
      else
        rm=r(1,j)*((pm(1,j)+p0(j))/(p(1,j)+p0(j)))**(1.d0/gam(j))
        sp(1,j)=vm(1,j)-dsqrt(gam(j)*(pm(1,j)+p0(j))/rm)
        if (sp(1,j)+tiny.lt.v(1,j)-c(1,j)) then
          ctest=.false.
          return
        end if
      end if
      if (pm(2,j).gt.p(2,j)) then
        sp(3,j)=v(2,j)+c(2,j)*dsqrt(ep(j)*(pm(2,j)+p0(j))
     *                                    /(p(2,j)+p0(j))+em(j))
        if (sp(3,j)+tiny.lt.v(2,j)+c(2,j)) then
          ctest=.false.
          return
        end if
      else
        rm=r(2,j)*((pm(2,j)+p0(j))/(p(2,j)+p0(j)))**(1.d0/gam(j))
        sp(3,j)=vm(2,j)+dsqrt(gam(j)*(pm(2,j)+p0(j))/rm)
        if (sp(3,j)-tiny.gt.v(2,j)+c(2,j)) then
          ctest=.false.
          return
        end if
      end if
c
      do j=1,2
        if (sp(2,1)+tiny.lt.sp(1,j).or.
     *      sp(2,1)-tiny.gt.sp(3,j)) then
          ctest=.false.
          return
        end if
        if (sp(2,j)+tiny.lt.sp(1,j).or.
     *      sp(2,j)-tiny.gt.sp(3,j)) then
          ctest=.false.
          return
        end if
      end do
c
      return
      end
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
      subroutine getg1d (pm,vm,rm,g,resid)
c
      implicit double precision (a-h,o-z)
      dimension pm(2,2),vm(2,2),rm(2),g(4)
      common / gasdat1d / gam(2),gm1(2),gp1(2),em(2),ep(2),p0(2),bgas
      common / prmdat1d / alpha(2,2),r(2,2),v(2,2),p(2,2),
     *                  a(2,2),b(2,2),c(2,2)
      common / scales1d / pscale,vscale,gfact(4)
c
c compute velocities across acoustic fields
      do j=1,2
        do i=1,2
          isign=2*i-3
          if (pm(i,j).gt.p(i,j)) then
            arg=pm(i,j)+b(i,j)
            fact=dsqrt(a(i,j)/arg)
            vm(i,j)=v(i,j)+isign*fact*(pm(i,j)-p(i,j))
          else
            arg=(pm(i,j)+p0(j))/(p(i,j)+p0(j))
            fact=2.d0*c(i,j)/gm1(j)
            vm(i,j)=v(i,j)+isign*fact*(arg**em(j)-1.d0)
          end if
        end do
      enddo
c
c check whether gas contact is to the left or
c to the right of solid contact, compute gas densities
      if (vm(1,1).gt.vm(1,2)) then
c gas contact on the left
        if (pm(2,2).gt.p(2,2)) then
          rm(2)=r(2,2)*(gp1(2)*(pm(2,2)+p0(2))/(p(2,2)+p0(2))+gm1(2))
     *                /(gm1(2)*(pm(2,2)+p0(2))/(p(2,2)+p0(2))+gp1(2))
        else
          rm(2)=r(2,2)*((pm(2,2)+p0(2))/(p(2,2)+p0(2)))**(1.d0/gam(2))
        end if
        rm(1)=rm(2)*((pm(1,2)+p0(2))/(pm(2,2)+p0(2)))**(1.d0/gam(2))
      else
c gas contact on the right
        if (pm(1,2).gt.p(1,2)) then
          rm(1)=r(1,2)*(gp1(2)*(pm(1,2)+p0(2))/(p(1,2)+p0(2))+gm1(2))
     *                /(gm1(2)*(pm(1,2)+p0(2))/(p(1,2)+p0(2))+gp1(2))
        else
          rm(1)=r(1,2)*((pm(1,2)+p0(2))/(p(1,2)+p0(2)))**(1.d0/gam(2))
        end if
        rm(2)=rm(1)*((pm(2,2)+p0(2))/(pm(1,2)+p0(2)))**(1.d0/gam(2))
      end if
c
c compute constraints across solid particle path
      v1=vm(1,2)-vm(1,1)
      v2=vm(2,2)-vm(2,1)
      h1=gam(2)*(pm(1,2)+p0(2))/(gm1(2)*rm(1))
      h2=gam(2)*(pm(2,2)+p0(2))/(gm1(2)*rm(2))
      g(1)=vm(2,1)-vm(1,1)
      g(2)= alpha(2,2)*((pm(2,2)+p0(2))**(1.d0/gam(2)))*v2
     *     -alpha(1,2)*((pm(1,2)+p0(2))**(1.d0/gam(2)))*v1
      g(3)= alpha(2,1)*pm(2,1)+alpha(2,2)*(pm(2,2)+rm(2)*v2**2)
     *     -alpha(1,1)*pm(1,1)-alpha(1,2)*(pm(1,2)+rm(1)*v1**2)
      g(4)=.5d0*v2**2+h2-.5d0*v1**2-h1
c
c scale residual
      do i=1,4
        g(i)=gfact(i)*g(i)
      end do
c
c compute residual
      resid=max(dabs(g(1)),dabs(g(2)),dabs(g(3)),dabs(g(4)))
c
      return
      end
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
      subroutine getdga1d (dg,pm,vm,rm)
c
      implicit double precision (a-h,o-z)
      dimension dg(4,4),pm(2,2),vm(2,2),rm(2)
      dimension fp(2,2),gk(2),gp(2),drdp(2,2)
      common / gasdat1d / gam(2),gm1(2),gp1(2),em(2),ep(2),p0(2),bgas
      common / prmdat1d / alpha(2,2),r(2,2),v(2,2),p(2,2),
     *                  a(2,2),b(2,2),c(2,2)
      common / scales1d / pscale,vscale,gfact(4)
c
      j=1
      do i=1,2
        if (pm(i,j).gt.p(i,j)) then
          fp(i,j)=(gp1(j)*(pm(i,j)+p0(j))+(3.d0*gam(j)-1.d0)*
     *             (p(i,j)+p0(j)))/dsqrt(2.d0*r(i,j)*(gp1(j)*
     *             (pm(i,j)+p0(j))+gm1(j)*(p(i,j)+p0(j)))**3)
        else
          fp(i,j)=(((pm(i,j)+p0(j))/(p(i,j)+p0(j)))**(-ep(j)))/
     *                                (r(i,j)*c(i,j))
        end if
      end do
c
      j=2
      do i=1,2
        if (pm(i,j).gt.p(i,j)) then
          fp(i,j)=(gp1(j)*(pm(i,j)+p0(j))+(3.d0*gam(j)-1.d0)*
     *             (p(i,j)+p0(j)))/dsqrt(2.d0*r(i,j)*(gp1(j)*
     *             (pm(i,j)+p0(j))+gm1(j)*(p(i,j)+p0(j)))**3)
          gk(i)=r(i,2)*(gp1(2)*(pm(i,2)+p0(2))+gm1(2)*(p(i,2)+p0(2)))/
     *                 (gm1(2)*(pm(i,2)+p0(2))+gp1(2)*(p(i,2)+p0(2)))
          gp(i)=4.0d0*gam(2)*r(i,2)*(p(i,2)+p0(2))/
     *          ((gm1(2)*(pm(i,2)+p0(2))+gp1(2)*(p(i,2)+p0(2)))**2)
        else
          fp(i,j)=((pm(i,j)/p(i,j))**(-ep(j)))/(r(i,j)*c(i,j))
          gk(i)=r(i,2)*((pm(i,2)+p0(2))/(p(i,2)+p0(2)))**(1/gam(2))
          gp(i)=gk(i)/(gam(2)*(pm(i,2)+p0(2)))
        end if
      end do
c
      gi=1.d0/gam(2)
      rat=((pm(1,2)+p0(2))/(pm(2,2)+p0(2)))**gi
      if (vm(1,1).gt.vm(1,2)) then
        drdp(1,1)=gk(2)*gi*rat/(pm(1,2)+p0(2))
        drdp(1,2)=(gp(2)-gk(2)*gi/(pm(2,2)+p0(2)))*rat
        drdp(2,1)=0.d0
        drdp(2,2)=gp(2)
      else
        drdp(1,1)=gp(1)
        drdp(1,2)=0.d0
        drdp(2,1)=(gp(1)-gk(1)*gi/pm(1,2))/rat
        drdp(2,2)=gk(1)*gi/pm(2,2)/rat
      end if
c
      dv1=vm(1,2)-vm(1,1)
      dv2=vm(2,2)-vm(2,1)
c
      dg(1,1)=fp(1,1)
      dg(1,2)=fp(2,1)
      dg(1,3)=0.d0
      dg(1,4)=0.d0
      dg(2,1)=-alpha(1,2)*(pm(1,2)**gi)*fp(1,1)
      dg(2,2)=-alpha(2,2)*(pm(2,2)**gi)*fp(2,1)
      dg(2,3)=alpha(1,2)*(pm(1,2)**gi)*(fp(1,2)-dv1*gi/pm(1,2))
      dg(2,4)=alpha(2,2)*(pm(2,2)**gi)*(fp(2,2)+dv2*gi/pm(2,2))
      dg(3,1)=-alpha(1,1)-2.d0*alpha(1,2)*rm(1)*dv1*fp(1,1)
      dg(3,2)=alpha(2,1)-2.d0*alpha(2,2)*rm(2)*dv2*fp(2,1)
      dg(3,3)=alpha(2,2)*drdp(2,1)*(dv2**2)-alpha(1,2)*
     *                 (1.d0+drdp(1,1)*(dv1**2)-2.d0*rm(1)*dv1*fp(1,2))
      dg(3,4)=-alpha(1,2)*drdp(1,2)*(dv1**2)+alpha(2,2)*
     *                 (1.d0+drdp(2,2)*(dv2**2)+2.d0*rm(2)*dv2*fp(2,2))
      dg(4,1)=-dv1*fp(1,1)
      dg(4,2)=-dv2*fp(2,1)
      dg(4,3)=dv1*fp(1,2)-gam(2)/gm1(2)*(1.0d0/rm(1)+
     *                          drdp(2,1)*(pm(2,2)+p0(2))/(rm(2)**2)-
     *                          drdp(1,1)*(pm(1,2)+p0(2))/(rm(1)**2))
      dg(4,4)=dv2*fp(2,2)+gam(2)/gm1(2)*(1.0d0/rm(2)+
     *                          drdp(1,2)*(pm(1,2)+p0(2))/(rm(1)**2)-
     *                          drdp(2,2)*(pm(2,2)+p0(2))/(rm(2)**2))
c
      do i=1,4
        do j=1,4
          dg(i,j)=gfact(i)*dg(i,j)
        end do
      end do
c
      return
      end
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
      subroutine getdgf1d (pm,dg,g0)
c
      implicit double precision (a-h,o-z)
      dimension pm(2,2),vm(2,2),rm(2),dg(4,4),g0(4),g(4)
      data delta / 1.d-6 /
c
c finite difference approximation of the jacobian
      do i=1,2
        do j=1,2
          n=2*(j-1)+i
          dpm=delta*pm(i,j)
          pm(i,j)=pm(i,j)+dpm
          call getg1d (pm,vm,rm,g,resid)
          do k=1,4
            dg(k,n)=(g(k)-g0(k))/dpm
          end do
          pm(i,j)=pm(i,j)-dpm
        end do
      end do
c
      return
      end
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
      subroutine getfx1d (i,j,alfa,pm,vm,wstar,fx)
c
c compute solid or gas flux for i=side and j=phase
c
      implicit double precision (a-h,o-z)
      dimension wstar(3),fx(3)
      common / gasdat1d / gam(2),gm1(2),gp1(2),em(2),ep(2),p0(2),bgas
      common / prmdat1d / alpha(2,2),r(2,2),v(2,2),p(2,2),
     *                  a(2,2),b(2,2),c(2,2)
c
c set isign=1 for i=1 and isign=-1 for i=2
      isign=3-2*i
c
      if (pm.gt.p(i,j)) then
c shock cases
        sp=isign*v(i,j)-c(i,j)*dsqrt(ep(j)*(pm+p0(j))/(p(i,j)+p0(j))
     *                                                         +em(j))
        if (sp.ge.0.d0) then
c left (i=1) or right (i=2)
          wstar(1)=r(i,j)
          wstar(2)=v(i,j)
          wstar(3)=p(i,j)
        else
c middle left (i=1) or middle right (i=2)
          wstar(1)=r(i,j)*(gp1(j)*(pm+p0(j))/(p(i,j)+p0(j))+gm1(j))
     *                   /(gm1(j)*(pm+p0(j))/(p(i,j)+p0(j))+gp1(j))
          wstar(2)=vm
          wstar(3)=pm
        end if
      else
c rarefaction cases
        if (isign*v(i,j)-c(i,j).ge.0.d0) then
c left (i=1) or right (i=2)
          wstar(1)=r(i,j)
          wstar(2)=v(i,j)
          wstar(3)=p(i,j)
        else
c left middle or sonic (i=1) or right middle or sonic (i=2)
          rm=r(i,j)*((pm+p0(j))/(p(i,j)+p0(j)))**(1.d0/gam(j))
          if (isign*vm-dsqrt(gam(j)*(pm+p0(j))/rm).gt.0.d0) then
c sonic left (i=1) or right (i=2)
            arg=(2.d0+isign*gm1(j)*v(i,j)/c(i,j))/gp1(j)
            wstar(1)=r(i,j)*arg**(2.d0/gm1(j))
            wstar(2)=c(i,j)*arg*isign
            wstar(3)=(p(i,j)+p0(j))*arg**(2.d0*gam(j)/gm1(j))-p0(j)
          else
c middle left (i=1) or right (i=2)
            wstar(1)=rm
            wstar(2)=vm
            wstar(3)=pm
          end if
        end if
      end if
c
c compute flux
      fact=wstar(1)*wstar(2)**2
      if (j.eq.1) then
        energy=(wstar(3)+gam(1)*p0(j))/gm1(1)
     *          +wstar(1)*compac(alfa,0)+.5d0*fact
      else
        energy=wstar(3)/gm1(2)+.5d0*fact
      end if
      fx(1)=alfa*wstar(1)*wstar(2)
      fx(2)=alfa*(fact+wstar(3))
      fx(3)=alfa*(energy+wstar(3))*wstar(2)
c
      return
      end
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
      subroutine getfx21d (i,alfa,pm,vm,wstar,fx)
c
c compute gas flux for the case when solid contact and gas contact
c are on either side of x=0.  If i=1, then solid contact is to the
c right of x=0, and if i=2, then solid contact is to the left.
c
      implicit double precision (a-h,o-z)
      dimension pm(2),wstar(3),fx(3)
      common / gasdat1d / gam(2),gm1(2),gp1(2),em(2),ep(2),p0(2),bgas
      common / prmdat1d / alpha(2,2),r(2,2),v(2,2),p(2,2),
     *                  a(2,2),b(2,2),c(2,2)
c
c if i=1, then k=2, and if i=2, then k=1
      k=3-i
c
      if (pm(k).gt.p(k,2)) then
c shock middle left (k=1) or middle right (k=2)
        rm=r(k,2)*(gp1(2)*(pm(k)+p0(2))/(p(k,2)+p0(2))+gm1(2))
     *           /(gm1(2)*(pm(k)+p0(2))/(p(k,2)+p0(2))+gp1(2))
        wstar(1)=rm*((pm(i)+p0(2))/(pm(k)+p0(2)))**(1.d0/gam(2))
      else
c rarefaction middle left (k=1) or right (k=2)
        wstar(1)=r(k,2)*((pm(i)+p0(2))/(p(k,2)+p0(2)))**(1.d0/gam(2))
      end if
c
      wstar(2)=vm
      wstar(3)=pm(i)
c
c compute flux
      fact=wstar(1)*wstar(2)**2
      energy=(wstar(3)+p0(2))/gm1(2)+.5d0*fact
      fx(1)=alfa*wstar(1)*wstar(2)
      fx(2)=alfa*(fact+wstar(3))
      fx(3)=alfa*(energy+wstar(3))*wstar(2)
c
      return
      end
