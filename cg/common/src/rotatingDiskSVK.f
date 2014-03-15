      subroutine rotatingDiskSVK (tfinal,nDisk,uDisk,param,nrwk,rwk)
c
c Compute the exact solution to the rotating disk (SVK model)
c
      implicit real*8 (a-h,o-z)
      dimension uDisk(nDisk,8),param(10),rwk(nrwk)
c
c number of grid cells
      n=nDisk-1
c
c split up workspace
      lu=1
      lp=lu+n+3
      lum=lp+n+3
      lpm=lum+n+1
      la=lpm+n+1
      lb=la+n+2
      lc=lb+n+2
      ld=lc+n+1
      lut=ld+n+1
      lpt=lut+n+1
      nreq=lpt+n
c
      if (nreq.gt.nrwk) then
        write(6,*)'Error (rotatingDiskSVK) : nrwk too small'
        stop
      end if
c
c get solution
      call getRotatingSoln (tfinal,nDisk,uDisk,param,n,rwk(lu),rwk(lp),
     *                      rwk(lum),rwk(lpm),rwk(la),rwk(lb),rwk(lc),
     *                      rwk(ld),rwk(lut),rwk(lpt))
c
      return
      end
c
c++++++++++++++++++
c
      subroutine getRotatingSoln (tfinal,nDisk,uDisk,param,n,u,p,
     *                            um,pm,a,b,c,d,ut,pt)
c
      implicit real*8 (a-h,o-z)
      dimension uDisk(nDisk,8),param(10)
      dimension u(-1:n+1),p(-1:n+1),um(0:n),pm(0:n),a(-1:n),b(-1:n),
     *          c(0:n),d(0:n),ut(0:n),pt(0:n)
      data tol, itmax / 1.d-12, 10 /
c
c      write(6,*)tfinal,nDisk,n
c      write(6,*)param
c      pause
c
c parameters
      r0=param(1)
      r1=param(2)
      omega0=param(3)
      alam=param(4)
      amu=param(5)
c
c combination of Lame parameters
      akap=alam+2.d0*amu
c
c some grid parameters
      fact=.2d0
      dr=(r1-r0)/n
      dt=fact*dr
      nstep=tfinal/dt+1
      dt=tfinal/nstep
      dt2=dt**2
c
c initial conditions
      if (tfinal.lt.1.d-14) then
        do j=0,n
          jp1=j+1
          r=r0+j*dr
          uDisk(jp1,1)=0.d0
          uDisk(jp1,2)=0.d0
          uDisk(jp1,3)=0.d0
          uDisk(jp1,4)=omega(r,r0,r1,omega0)
          uDisk(jp1,5)=0.d0
          uDisk(jp1,6)=0.d0
          uDisk(jp1,7)=0.d0
          uDisk(jp1,8)=0.d0
        end do
        return
      else
        do j=0,n
          r=r0+j*dr
          dp0=dt*omega(r,r0,r1,omega0)
          u(j)=0.d0
          p(j)=0.d0
          um(j)=.5d0*r*dp0**2
          pm(j)=-dp0
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
      t=0.d0
c
c      write(52,200)nstep,n,tfinal,r0,r1
c  200 format(2(1x,i5),3(1x,1pe15.8))
c      do j=0,n
c        write(52,250)u(j),p(j)
c  250   format(2(1x,1pe15.8))
c      end do
c
      do m=1,nstep+1
c
c boundary conditions at r=r0
        if (j0.eq.0) then
          udr=u(0)/r0
          ak=alam*udr*(2.d0+udr)/akap
          if (ak.ge.1.d0) then
            write(6,*)'Error (getRotatingSoln) : cannot resolve bc0'
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
c boundary conditions at r=r1
        udr=u(n)/r1
        ak=alam*udr*(2.d0+udr)/akap
        if (ak.ge.1.d0) then
          write(6,*)'Error (getRotatingSoln) : cannot resolve bc1'
          stop
        end if
        ur=-ak/(1.d0+dsqrt(1.d0-ak))
        u(n+1)=u(n-1)+2.d0*dr*ur
        p(n+1)=p(n-1)
c
c compute a=r\bar P_{11} and b=r\bar P_{12} at cell boundaries
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
c compute RHS's of wave equations (c,d) for u and p
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
c compute RHS/(r+u) of wave equation (d) for p (at r=0, if needed)
        if (j0.eq.1) then
          ur=(u(1)-u(-1))/(2.d0*dr)
          urp1=ur+1.d0
          prr=(p(1)-2.d0*p(0)+p(-1))/dr**2
          d(0)=4.d0*((alam+akap)*(1.d0+.5d0*ur)*ur
     *                             +2.d0*amu*urp1**2)*prr
        end if
c
c advance u and p
        do j=j0,n
          r=r0+j*dr
          up=2.d0*u(j)-um(j)
          pp=2.d0*p(j)-pm(j)
          do it=1,itmax
            utj=(up-um(j))/(2.d0*dt)
            ptj=(pp-pm(j))/(2.d0*dt)
            utt=(up-2.d0*u(j)+um(j))/dt2
            ptt=(pp-2.d0*p(j)+pm(j))/dt2
            b1=utt-(r+u(j))*ptj**2-c(j)
            b2=(r+u(j))*ptt+2.d0*utj*ptj-d(j)
            a11=1.d0/dt2
            a12=-(r+u(j))*ptj/dt
            a21=ptj/dt
            a22=(r+u(j))/dt2+utj/dt
            det=a11*a22-a12*a21
            du=(b1*a22-b2*a12)/det
            dp=(b2*a11-b1*a21)/det
            up=up-du
            pp=pp-dp
            if (max(dabs(du),dabs(dp)).lt.tol) then
              ut(j)=(up-um(j))/(2.d0*dt)
              pt(j)=(pp-pm(j))/(2.d0*dt)
              goto 1
            else
              if (it.eq.itmax) then
                write(6,*)'Error (getRotatingSoln) : did not converge'
                stop
              end if
            end if
          end do
    1     if (m.le.nstep) then
            um(j)=u(j)
            pm(j)=p(j)
            u(j)=up
            p(j)=pp
          end if
        end do
c
c advance p for r=0 (u is equal to zero at r=0), if needed
        if (j0.eq.1) then
          urt=ut(1)/dr
          a1=1.d0/dt2+urt/(urp1*dt)
          b1=(2.d0*p(0)-pm(0))/dt2+urt*pm(0)/dt+d(0)
          pp=b1/a1
          ut(0)=0.d0
          pt(0)=(pp-pm(0))/(2.d0*dt)
          if (m.le.nstep) then
            pm(0)=p(0)
            p(0)=pp
          end if
        end if
c
        t=t+dt
c
c        if (m.le.nstep) then
c          do j=0,n
c            write(52,250)u(j),p(j)
c          end do
c        end if
c
      end do
c
      do j=0,n
        jp1=j+1
        r=r0+j*dr
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
        uDisk(jp1,1)=u(j)
        uDisk(jp1,2)=p(j)
        uDisk(jp1,3)=ut(j)
        uDisk(jp1,4)=pt(j)
        uDisk(jp1,5)=p11
        uDisk(jp1,6)=p12
        uDisk(jp1,7)=p21
        uDisk(jp1,8)=p22
c        write(51,300)r,u(j),p(j),ut(j),pt(j),p11,p12,p21,p22
c  300   format(9(1x,1pe22.15))
      end do
c
      return
      end
c
c+++++++++++++++++++++
c
      double precision function omega (r,r0,r1,omega0)
c
      implicit real*8 (a-h,o-z)
c
      quad=4.d0*(r-r0)*(r1-r)/(r0+r1)**2
      omega=omega0*quad**2
c      omega=omega0
c
      return
      end

