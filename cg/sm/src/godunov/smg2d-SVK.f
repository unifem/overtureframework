      subroutine smg2dn (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                   ds1,ds2,dt,t,xy,rx,det,u,up,f1,f2,ad2,ad2dt,
     *                   ad4,ad4dt,mask,nrprm,rparam,niprm,iparam,
     *                   nrwk,rwk,niwk,iwk,idebug,ier)
c
c
c compute du/dt for 2d nonlinear elasticity (smg => solid mechanics, Godunov)
c
c variables: u(j1,j2,1:2)=velocity
c            u(j1,j2,3:6)=stress (P11,P12,P21,P22)
c            u(j1,j2,7:8)=displacement
c
      implicit real*8 (a-h,o-z)
      dimension xy(*),rx(*),det(*),u(nd1a:nd1b,nd2a:nd2b,m),almax(2),
     *          up(nd1a:nd1b,nd2a:nd2b,m),mask(nd1a:nd1b,nd2a:nd2b),
     *          f1(*),f2(*),rparam(nrprm),iparam(niprm),rwk(nrwk),
     *          iwk(niwk),ad2(*),ad2dt(*),ad4(*),ad4dt(*)
      real*8 diseig,dseigdt,tsdiss,ad2Max,ad4Max
      dimension wtmp(10000)             ! used for testing slope correction
      common / smgdatn / amu,alam,rho0
      common / smrlxn / relaxAlpha,relaxDelta,irelax
      common / smgprmn / method,iorder,ilimit,iupwind,icart,itype,ifrc
      common / tzflow / eptz,itz



c      write(33,332)t
c  332 format(' time =',1pe15.8)
c      do ic=1,8
cc        write(33,334)ic
cc  334   format(' ic =',i2)
c        do j2=n2b+2,n2b-2,-1
c          write(33,333)(u(j1,j2,ic),j1=n1a-2,n1a+2)
c  333     format(5(1x,1pe22.15))
c        end do
c      end do

c
c itype=0 => linear elasicity
c itype=1 => SVK with linear reduction
c itype>1 => SVK
c
c      data ad / .5d0, .5d0, .5d0, .5d0, .5d0, .5d0, .5d0, .5d0 /
c      data ad4 / 0.d0, 0.d0, 0.d0, 0.d0, 0.d0, 0.d0, 0.d0, 0.d0 /
c      if (t.lt.1.d-12) then
c      do i=1,m
c        write(6,*)i,ad(i)
c      end do
c      pause
c      end if

c      do j1=n1a,n1b,n1b-n1a
c      do j2=n2a,n2b,n2b-n2a
c        write(6,231)j1,j2,(u(j1,j2,i),i=3,6)
c  231   format(2(1x,i3),4(1x,1pe9.2))
c      end do
c      end do
c      do j1=n1a,n1b,n1b-n1a
c      do j2=n2a,n2b,n2b-n2a
c        write(6,232)j1,j2,(u(j1,j2,i),i=1,2)
c  232   format(2(1x,i3),2(1x,1pe9.2))
c      end do
c      end do
c      pause
c
c..set error flag
      ier=0
c
c..parameters
      amu=rparam(3)
      alam=rparam(4)
      rho0=rparam(5)
      eptz=rparam(6)
      relaxAlpha=rparam(7)
      relaxDelta=rparam(8)
      tsdiss=rparam(9)
      tsdissdt=rparam(10)

      iorder=iparam(1)
      icart=iparam(2)
      itz=iparam(3)
      method=iparam(4)
      ilimit=iparam(5)
      iupwind=iparam(6)
      itype=iparam(7)
      ifrc=iparam(8)
      irelax=iparam(9)

c
c      write(6,123)t,icart,n1a,n1b,n2a,n2b
c  123 format(1x,1pe10.3,5(1x,i4))
c      pause
c
c      write(6,*)alam,amu,rho0
c      write(6,*)'Message (smg2dn) : itype=',itype
c      pause
c
      ! *wdh* 2014/03/06 -- these were hard coded:
      ! *wdh* tsdiss=1.d0
      ! *wdh* irelax=4

c      write(6,*)relaxAlpha,relaxDelta,tsdiss,iorder,method,irelax
c      write(6,*)'rho0,alam,amu=',rho0,alam,amu
c      pause
c

c       du11min=0.d0
c       do j1=n1a,n1b
c       do j2=n2a,n2b
c         du11=(u(j1+1,j2,7)-u(j1-1,j2,7))/(2.*ds1)
c         du11min=min(du11,du11min)
c       end do
c       end do
c       write(6,*)'t,du11min =',t,du11min

c      j1=n1b
c      do j2=n2a,n2b
c        y=j2*ds2
c        write(66,666)y,t,(u(j1,j2,i),i=1,8)
c  666   format(10(1x,1pe15.8))
c      end do
c
c      j2=n2a
c      do j1=n1a,n1b
c        x=j1*ds1
c        write(77,666)x,t,(u(j1,j2,i),i=1,8)
cc  666   format(10(1x,1pe15.8))
c      end do

c      write(44,442)t
c  442 format('u: t=',1pe15.8)
c      do j2=nd2a,nd2b
c      do j1=nd1a,nd1b
c        write(44,444)j1,j2,(u(j1,j2,i),i=1,8)
cc  444   format(2(1x,i2),8(1x,1pe9.2))
c      end do
c      end do

c      if (icart.ne.1) then
c        write(6,*)t
c        write(6,*)n1a,n1b,n2a,n2b
c        pause
c      end if
c
c..checking zero-traction boundary conditions
c      if (icart.ne.1) then
c        call bcchk (m,nd1a,nd1b,nd2a,nd2b,n1a,n1b,n2a,n2b,
c     *              ds1,ds2,t,rx,u)
c      end if
c
c..printing (x,y) positions
c      if (icart.ne.1) then
c        call prtxy (m,nd1a,nd1b,nd2a,nd2b,n1a,n1b,n2a,n2b,
c     *              t,xy,u)
c      end if
c
c      write(6,*)iorder,icart,method,irelax
c      write(6,*)relaxAlpha,relaxDelta,tsdiss
c      pause
c
c      tsdiss=ad(1)   ! tangent stress dissipation (for now)
c
c      write(6,*)'irelax=',irelax
c      pause
c
c      ifrc=0
c      if( itype.ne.1 .and. itype.ne.2  )then
c         write(*,*) 'smg2d-SVK:ERROR: invalid itype'
c         write(6,*)itype
c         stop 8376
c      end if
c
c check itype
      if (itype.le.0) then
        write(6,*)'Error (smg2dn) : invalid value for itype'
        stop
      end if
c
c     if (m.gt.0.and.t.gt.1.d-2) then
c      if (m.lt.0) then
c        write(44,444)t
c  444   format(1x,1pe15.8)
c        do i=1,8
c          do j1=nd1a,nd1b
c          do j2=nd2a,nd2b
c            write(44,444)u(j1,j2,i)
c          end do
c          end do
c        end do
c        stop
c      end if
c
c      write(6,*)amu,alam,rho0,eptz,iorder,icart,itz
c      if (m.gt.0) stop
c
c      write(6,*)'order =',iorder
c      pause
c
c      iorder=2
c      method=1
c      iorder=2
c      write(6,*)'******* Setting iorder=2 (smg2d) ********'
c      write(6,*)amu,alam,rho0,iorder,icart
c      pause
c      if (m.gt.0) stop
c
c..set array dimensions for parallel
      md1a=max(nd1a,n1a-2)
      md1b=min(nd1b,n1b+2)
      md2a=max(nd2a,n2a-2)
      md2b=min(nd2b,n2b+2)
c
c..sanity check
      if (m.ne.8) then
        write(6,*)'Error (smg2d) : m=8 is assumed'
        stop 1
      end if
c
c..print displacement of the corners of the grid
c      write(50,501)t,u(n1a,n2a,7),u(n1a,n2a,8),
c     *               u(n1b,n2a,7),u(n1b,n2a,8),
c     *               u(n1a,n2b,7),u(n1a,n2b,8),
c     *               u(n1b,n2b,7),u(n1b,n2b,8)
c  501 format(f10.6,8(1x,1pe15.8))
c
c..print displacement on the perimeter
c      call prtprm (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,dt,t,xy,u)
c
c..set boundaries of mapping variables depending on whether
c  the grid is Cartesian (icart=1) or not (icart=0)
c      if (icart.eq.0) then
c        n1bm=nd1b
c        n2bm=nd2b
c      else
c        n1bm=nd1a
c        n2bm=nd2a
c      end if
c
      ngrid=(md1b-md1a+1)
c
c..split up real work space =>
      lw=1
      la1=lw+m*ngrid*10
      laj=la1+8*ngrid
      ldpdf=laj+2*ngrid
      nreq=ldpdf+32*ngrid-1
      if (nreq.gt.nrwk) then
        ier=4
        return
      end if
c
c..filter out underflow in u
      tol=1.d-30
      do j2=md2a,md2b
      do j1=md1a,md1b
        do i=1,m
          if (dabs(u(j1,j2,i)).lt.tol) u(j1,j2,i)=0.d0
        end do
      end do
      end do
c
c..check solution
c      if (t.gt.0.8d0) then
c      call checks (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
c     *             ds1,ds2,t,u,up)
c      end if
c
c..compute a forcing function (old version)
c      call smforcing (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
c     *                ds1,ds2,t,dt,force)
c compute up
c      dt=0.001d0
      call getup2dn (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
     *               md2a,md2b,n2a,n2b,ds1,ds2,dt,t,xy,rx,det,u,
     *               up,f1,f2,mask,almax,rwk(lw),rwk(la1),rwk(laj),
     *               rwk(ldpdf),ier,wtmp)
c
c      call prtui (m,nd1a,nd1b,nd2a,nd2b,n1a,n1b,n2a,n2b,
c     *            t,dt,xy,u,up,mask)
c
c      tol=1.d-12
c      tfinal=1.d0
c      if (dabs(t+dt-tfinal).lt.tol) then
cc        call prtu (m,nd1a,nd1b,nd2a,nd2b,n1a,n1b,n2a,n2b,
cc     *             dt,xy,u,up)
c        call prtu1 (m,nd1a,nd1b,nd2a,nd2b,n1a,n1b,n2a,n2b,
c     *              ds1,ds2,dt,xy,u,up,mask)
c      end if
c
c      call prtu2 (m,nd1a,nd1b,nd2a,nd2b,n1a,n1b,n2a,n2b,
c     *            ds1,ds2,dt,t,xy,u,up,mask)
c
c 2nd and 4th-order dissipation for components of stress on surfaces whose normal is
c tangent to cell faces.
      diseig=0.     ! used for real-part of time-stepping eigenvalue
      diseigdt=0.   ! holds coeff of (1/dt) dissipation eigenvalue 
      dt0=dt
      if( t.lt.0 )then
        ! This is an initialization call, dt is not known yet so do this: (*FIX ME- do better)
        dt0=max(dt,min(ds1,ds2))  
      end if

      if (tsdiss.gt.1.d-12) then
        if (iorder.eq.1) then
          call stressDiss2 (m,nd1a,nd1b,nd2a,nd2b,n1a,n1b,n2a,n2b,
     *            ds1,ds2,tsdiss,tsdissdt,rx,u,up,mask,diseig,diseigdt)
        else
          if( .true. )then
           if( t.lt.2*dt )then
             write(*,'("smgSVK:stressDiss4, tsdiss,tsdissdt=",2e10.2)')
     &           tsdiss,tsdissdt
             write(*,'("smgSVK:stressDiss4, dt,dt0=",2e10.2)') dt,dt0
           end if
           call stressDiss4 (m,nd1a,nd1b,nd2a,nd2b,n1a,n1b,n2a,n2b,
     *        dt0,ds1,ds2,tsdiss,tsdissdt,rx,u,up,mask,diseig,diseigdt)
          else
           ! old: 
          call stressDiss4_0 (m,nd1a,nd1b,nd2a,nd2b,n1a,n1b,n2a,n2b,
     *           ds1,ds2,tsdiss,tsdissdt,rx,u,up,mask,diseig,diseigdt)
         end if
        end if
      end if
c
c add relaxation term to ensure compatibility of stress and position
      if (irelax.ne.0) then
        call stressRelax2dn (m,nd1a,nd1b,nd2a,nd2b,n1a,n1b,n2a,n2b,
     *                       ds1,ds2,dt,t,xy,rx,u,up,mask)
      end if
c
c artificial dissipation (bulk)
      ad2Max=0. ! holds max of scaled ad2(i) 
      ad4Max=0. ! holds max of scaled ad4(i)
      ad2dtMax=0. ! holds max of scaled ad2dt(i) 
      ad4dtMax=0. ! holds max of scaled ad4dt(i)
      do i=1,m
c
c second order

        ! adi=ad2(i)/(2*4*dt0)
        ! adi=ad2(i)  ! ***TEMP 
        adi=( ad2(i)+ad2dt(i)/dt0 )/(2*4)  ! 2=number of dimensions, 4=1+2+1
        if (adi.gt.1.d-12) then
          ad2Max=max(ad2Max,ad2(i))
          ad2dtMax=max(ad2dtMax,ad2dt(i))
          if( iorder.eq.1 .and. i.ge.3 .and. i.le.6 )then
            ! Stresses can have two contributions to the dissipation
             ad2Max=max(ad2Max,diseig+ad2(i))
             ad2dtMax=max(ad2dtMax,diseigdt+ad2dt(i))
          end if
          do j2=n2a,n2b
          do j1=n1a,n1b
            if (mask(j1,j2).ne.0) then
              up(j1,j2,i)=up(j1,j2,i)+adi*(u(j1+1,j2,i)+u(j1-1,j2,i)
     *                    +u(j1,j2+1,i)+u(j1,j2-1,i)-4.d0*u(j1,j2,i))
            end if
          end do
          end do
        end if
c
c fourth order

        ! adi=ad4(i)/(2*16*dt0)
        ! adi=ad4(i)    ! ***TEMP 
        adi = (ad4(i)+ad4dt(i)/dt0 )/(2*16) ! 2=number of dimensions, 16=1+4+6+4+1 
        if (adi.gt.1.d-12) then
          if( t.lt.2*dt )then
            write(*,'(" smgSVK: ad4+ad4t/dt (",i2,")=",e10.2)') i,adi
          end if
          ad4Max=max(ad4Max,ad4(i))
          ad4dtMax=max(ad4dtMax,ad4dt(i))
          if( iorder.eq.2 .and. i.ge.3 .and. i.le.6 )then
            ! Stresses can have two contributions to the dissipation
             ad4Max=max(ad4Max,diseig+ad4(i))
             ad4dtMax=max(ad4dtMax,diseigdt+ad4dt(i))
          end if
          do j2=n2a,n2b
          do j1=n1a,n1b
            if (mask(j1,j2).ne.0) then
              ! *wdh* Use undivided differences: 
              up(j1,j2,i)=up(j1,j2,i)+adi*(
     *                      (-u(j1+2,j2,i)+4*u(j1+1,j2,i)-6*u(j1,j2,i)
     *                                +4*u(j1-1,j2,i)-u(j1-2,j2,i))
     *                     +(-u(j1,j2+2,i)+4*u(j1,j2+1,i)-6*u(j1,j2,i)
     *                                +4*u(j1,j2-1,i)-u(j1,j2-2,i)))
!*wdh              up(j1,j2,i)=up(j1,j2,i)+adi*(
!*wdh     *                      (-u(j1+2,j2,i)+4*u(j1+1,j2,i)-6*u(j1,j2,i)
!*wdh     *                                +4*u(j1-1,j2,i)-u(j1-2,j2,i))/ds1
!*wdh     *                     +(-u(j1,j2+2,i)+4*u(j1,j2+1,i)-6*u(j1,j2,i)
!*wdh     *                                +4*u(j1,j2-1,i)-u(j1,j2-2,i))/ds2)
            end if
          end do
          end do
        end if
      end do

c
c       upmax=0.d0
c       do i=1,m
c         do j2=n2a,n2b
c         do j1=n1a,n1b
c           if (mask(j1,j2).ne.0) then
c             upmax=max(dabs(up(j1,j2,i)),upmax)
c           end if
c         end do
c         end do
c       end do
c       write(6,*)'upmax=',upmax
c       pause

c       do i=1,m
c         do j2=nd2a,nd2b
c         do j1=nd1a,nd1b
c           up(j1,j2,i)=0.d0
c         end do
c         end do
c       end do

c      write(44,443)t
c  443 format('up: t=',1pe15.8)
c      do j2=nd2a,nd2b
c      do j1=nd1a,nd1b
c        write(44,444)j1,j2,(up(j1,j2,i),i=1,8)
c  444   format(2(1x,i2),8(1x,1pe9.2))
c      end do
c      end do
c      pause

c
c compute real and imaginary parts of lambda, where the time stepping
c is interpreted as u'=lambda*u
c
      rparam(1)= max(diseig,ad2Max + ad4Max) 
      rparam(2)=almax(1)/ds1+almax(2)/ds2
      rparam(3)=max( diseigdt,ad2dMax+ad4dtMax)
      ! write(*,'("smg2dn diseigdt=",e10.2)') diseigdt

c      write(50,*)'alam ='
c      write(6,*)ds1,ds2,almax(1),almax(2)
c      pause
c      write(50,302)almax(1),almax(2)
c  302 format(2(1x,1pe10.3))
c
      return
      end
c
c+++++++++++++++++++++++
c
      subroutine prtprm (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                   dt,t,xy,u)
c
      implicit real*8 (a-h,o-z)
      dimension xy(nd1a:nd1b,nd2a:nd2b,2),u(nd1a:nd1b,nd2a:nd2b,m)
c
      if (dt.lt.1.d-8) then
        open(51,file='tdat.m')
        open(52,file='xdat.m')
        open(53,file='ydat.m')
        write(51,510)
        write(52,520)
        write(53,530)
  510   format('t=[];')
  520   format('x=[];')
  530   format('y=[];')
c        write(6,*)xy(n1b,n2b,1),xy(n1a,n2b,2)
c        pause
        return
      end if
c
      write(51,511)t
  511 format('t=[t,',1pe15.8,'];')
c
      write(52,521)
      write(53,531)
  521 format('x1=[')
  531 format('y1=[')
      do j1=n1a,n1b
        write(52,'(1pe15.8)')xy(j1,n2a,1)+u(j1,n2a,7)
        write(53,'(1pe15.8)')xy(j1,n2a,2)+u(j1,n2a,8)
      end do
      do j2=n2a+1,n2b
        write(52,'(1pe15.8)')xy(n1b,j2,1)+u(n1b,j2,7)
        write(53,'(1pe15.8)')xy(n1b,j2,2)+u(n1b,j2,8)
      end do
      do j1=n1b-1,n1a,-1
        write(52,'(1pe15.8)')xy(j1,n2b,1)+u(j1,n2b,7)
        write(53,'(1pe15.8)')xy(j1,n2b,2)+u(j1,n2b,8)
      end do
      do j2=n2b-1,n2a,-1
        write(52,'(1pe15.8)')xy(n1a,j2,1)+u(n1a,j2,7)
        write(53,'(1pe15.8)')xy(n1a,j2,2)+u(n1a,j2,8)
      end do
      write(52,522)
      write(53,532)
  522 format('];  x=[x x1];')
  532 format('];  y=[y y1];')
c
      return
      end
c
c+++++++++++++++++++++++
c
      subroutine checks (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
     *                   ds1,ds2,t,u,ue)
c
      implicit real*8 (a-h,o-z)
      dimension u(nd1a:nd1b,nd2a:nd2b,m),ue(nd1a:nd1b,nd2a:nd2b,m)
c
c hard code some parameters
      pi=4.d0*datan(1.d0)
      omega=2.d0*pi
      x0=0.d0
      y0=0.d0
c
c compute rotation "solution" (assumes a square grid)
      do j1=nd1a,nd1b
      do j2=nd2a,nd2b
        x=j1*ds1
        y=j2*ds2
        u1e= (dcos(omega*t)-1.d0)*(x-x0)+dsin(omega*t)*(y-y0)
        u2e=-dsin(omega*t)*(x-x0)+(dcos(omega*t)-1.d0)*(y-y0)
        v1e=-omega*dsin(omega*t)*(x-x0)+omega*dcos(omega*t)*(y-y0)
        v2e=-omega*dcos(omega*t)*(x-x0)-omega*dsin(omega*t)*(y-y0)
        u1x= (dcos(omega*t)-1.d0)
        u1y= dsin(omega*t)
        u2x=-dsin(omega*t)
        u2y= (dcos(omega*t)-1.d0)
        f11=1.d0+u1x
        f12=     u1y
        f21=     u2x
        f22=1.d0+u2y
        e11=.5d0*(f11*f11+f21*f21-1.d0)
        e12=.5d0*(f11*f12+f21*f22     )
        e22=.5d0*(f12*f12+f22*f22-1.d0)
        trace=e11+e22
        s11=lambda*trace+2.d0*mu*e11
        s12=             2.d0*mu*e12
        s21=s12
        s22=lambda*trace+2.d0*mu*e22
        p11=s11*f11+s12*f12
        p12=s11*f21+s12*f22
        p21=s21*f11+s22*f12
        p22=s21*f21+s22*f22
        ue(j1,j2,1)=v1e
        ue(j1,j2,2)=v2e
        ue(j1,j2,3)=p11
        ue(j1,j2,4)=p12
        ue(j1,j2,5)=p21
        ue(j1,j2,6)=p22
        ue(j1,j2,7)=u1e
        ue(j1,j2,8)=u2e
      end do
      end do
c
c compare solutions
      do i=1,8
        do j1=nd1a,nd1b
        do j2=nd2a,nd2b
          ue(j1,j2,i)=u(j1,j2,i)-ue(j1,j2,i)
        end do
        end do
      end do
c
c zero out error in the interior if t>0
      if (t.gt.1.d-8) then
        do i=1,8
          do j1=n1a+1,n1b-1
          do j2=n2a+1,n2b-1
            ue(j1,j2,i)=0.d0
          end do
          end do
        end do
      end if
c
c print out comparison
      nx=nd1b-nd1a+1
      ny=nd2b-nd2a+1
      write(44,400)nx,ny,t
  400 format(2(1x,i3),1x,1pe15.8)
      do j1=nd1a,nd1b
      do j2=nd2a,nd2b
        x=j1*ds1
        y=j2*ds2
        write(44,410)x,y,(ue(j1,j2,i),i=1,8)
  410   format(2(1x,f10.5),8(1x,1pe10.3))
      end do
      end do
c
c zero out ue
      do i=1,8
        do j1=nd1a,nd1b
        do j2=nd2a,nd2b
          ue(j1,j2,i)=0.d0
        end do
        end do
      end do
c
      return
      end
c
c+++++++++++++++++++++++
c
      subroutine smforcing (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
     *                      ds1,ds2,t,dt,force)
c
      implicit real*8 (a-h,o-z)
      dimension force(nd1a:nd1b,nd2a:nd2b,2,2)
      common / smgdatn / amu,alam,rho0
c
c hard code some parameters
      pi=4.d0*datan(1.d0)
      omega=2.d0*pi           ! omega=1 in the cmd file
      omeg2=omega**2/rho0
      x0=0.d0
      y0=0.d0
      t2=t+dt
c
c zero out forcing
      do j1=nd1a,nd1b
      do j2=nd2a,nd2b
        force(j1,j2,1,1)=0.d0
        force(j1,j2,2,1)=0.d0
        force(j1,j2,1,2)=0.d0
        force(j1,j2,2,2)=0.d0
      end do
      end do
c
c compute rotation "solution" (assumes a square grid)
      if (.false.) then
      do j1=nd1a,nd1b
      do j2=nd2a,nd2b
        x=j1*ds1
        y=j2*ds2
        a1e=(-dcos(omega*t)*(x-x0)-dsin(omega*t)*(y-y0))*omeg2
        a2e=( dsin(omega*t)*(x-x0)-dcos(omega*t)*(y-y0))*omeg2
        force(j1,j2,1,1)=a1e
        force(j1,j2,2,1)=a2e
        a1e=(-dcos(omega*t2)*(x-x0)-dsin(omega*t2)*(y-y0))*omeg2
        a2e=( dsin(omega*t2)*(x-x0)-dcos(omega*t2)*(y-y0))*omeg2
        force(j1,j2,1,2)=a1e
        force(j1,j2,2,2)=a2e
      end do
      end do
      end if
c
      return
      end
c
c+++++++++++++++++++++++
c
      subroutine getup2dn (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
     *                     md2a,md2b,n2a,n2b,ds1,ds2,dt,t,xy,rx,det,
     *                     u,up,f1,f2,mask,almax,w,a1,aj,dpdf,ier,wtmp)
c
      implicit real*8 (a-h,o-z)
      dimension xy(nd1a:nd1b,nd2a:nd2b,2),rx(nd1a:nd1b,nd2a:nd2b,2,2),
     *          det(nd1a:nd1b,nd2a:nd2b),u(nd1a:nd1b,nd2a:nd2b,m),
     *          up(nd1a:nd1b,nd2a:nd2b,m),f1(nd1a:nd1b,nd2a:nd2b,2),
     *          f2(nd1a:nd1b,nd2a:nd2b,2),mask(nd1a:nd1b,nd2a:nd2b),
     *          almax(2),w(m,md1a:md1b,5,2),a1(2,2,md1a:md1b,2),
     *          aj(md1a:md1b,2),dpdf(4,4,md1a:md1b,2)
      dimension wtmp(m,md1a:md1b,5),errw(6,5),dptmp(4,4,-2:12),erd(4,4)
      dimension fxl(6),fxr(6),htz(8),fxtmp(6),errf(6),cml(4,2),cmr(4,2)
      common / smgprmn / method,iorder,ilimit,iupwind,icart,itype,ifrc
      common / smgdatn / amu,alam,rho0
      common / tzflow / eptz,itz
c
c      write(6,*)md1a,md1b
c      pause
c
c      do i=1,6
c        errf(i)=0.d0
c        do k=1,5
c          errw(i,k)=0.d0
c        end do
c      end do
c      do i=1,4
c      do j=1,4
c        erd(i,j)=0.d0
c      end do
c      end do
c
c..initialize almax
      almax(1)=0.d0
      almax(2)=0.d0
c
c..initialize up
      do i=1,6
        do j2=md2a,md2b
        do j1=md1a,md1b
          up(j1,j2,i)=0.d0
        end do
        end do
      end do
c
      if (ifrc.ne.0) then
        do i=1,2
          do j2=md2a,md2b
          do j1=md1a,md1b
            up(j1,j2,i)=.5d0*(f1(j1,j2,i)+f2(j1,j2,i))
          end do
          end do
        end do
      end if
c
      j2=n2a-1
c
c..set grid metrics and velocity (if necessary)
      call smmetrics2dn (nd1a,nd1b,md1a,md1b,nd2a,nd2b,j2,rx,det,
     *                   a1(1,1,md1a,1),aj(md1a,1))
c
c      if (icart.eq.0) then
c        call smslope2dtz (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
      call smslope2dn (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
     *                 j2,ds1,ds2,dt,t,xy,a1(1,1,md1a,1),
     *                 aj(md1a,1),mask(nd1a,j2),u,w(1,md1a,1,1),
     *                 dpdf(1,1,md1a,1),f1)
c      else
c        call smslope2d (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
c     *                  j2,ds1,ds2,dt,t,xy,a1(1,1,md1a,1),
c     *                  aj(md1a,1),mask(nd1a,j2),u,wtmp(1,md1a,1),
c     *                  dptmp(1,1,md1a))
c        call smslope2dtz (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
c        call smslope2d (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
c     *                  j2,ds1,ds2,dt,t,xy,a1(1,1,md1a,1),
c     *                  aj(md1a,1),mask(nd1a,j2),u,w(1,md1a,1,1),
c     *                  dpdf(1,1,md1a,1))
c
c          do j1=n1a-1,n1b+1
c            if (mask(j1,j2).ne.0) then
c              do k=1,5
c              do i=1,6
c                errw(i,k)=max(dabs(w(i,j1,k,1)-wtmp(i,j1,k)),errw(i,k))
c                errf(i)=max(dabs(w(i,j1,k,1)),errf(i))
c              end do
c              end do
c              do i=1,4
c              do j=1,4
c                erd(i,j)=max(dabs(dpdf(i,j,j1,1)-dptmp(i,j,j1)),
c     *                       erd(i,j))
c                if (dabs(dpdf(i,j,j1,1)-dptmp(i,j,j1)).gt.1.d-6) then
c                  write(6,*)'j1,j2,i,j,dp,dp=',
c     *                      j1,j2,i,j,dpdf(i,j,j1,1),dptmp(i,j,j1)
c                  pause
c                end if
c              end do
c              end do
c            end if
c          end do
c
c      end if
c
c..loop over lines j2=n2a:n2b+1
      do j2=n2a,n2b+1
        j2m1=j2-1
c
c..set grid metrics and velocity (if necessary)
        call smmetrics2dn (nd1a,nd1b,md1a,md1b,nd2a,nd2b,j2,rx,det,
     *                     a1(1,1,md1a,2),aj(md1a,2))
c
c..slope correction (top row of cells)
c        if (icart.eq.0) then
c          call smslope2dtz (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
        call smslope2dn (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
     *                   j2,ds1,ds2,dt,t,xy,a1(1,1,md1a,2),
     *                   aj(md1a,2),mask(nd1a,j2),u,w(1,md1a,1,2),
     *                   dpdf(1,1,md1a,2),f1)
c        else
c          call smslope2d (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
c     *                    j2,ds1,ds2,dt,t,xy,a1(1,1,md1a,2),
c     *                    aj(md1a,2),mask(nd1a,j2),u,wtmp(1,md1a,1),
c     *                    dptmp(1,1,md1a))
c          call smslope2dtz (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
c          call smslope2d (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
c     *                    j2,ds1,ds2,dt,t,xy,a1(1,1,md1a,2),
c     *                    aj(md1a,2),mask(nd1a,j2),u,w(1,md1a,1,2),
c     *                    dpdf(1,1,md1a,2))
c
c            do j1=n1a-1,n1b+1
c              if (mask(j1,j2).ne.0) then
cc                write(6,*)'j1=',j1
c                do k=1,5
cc                  write(6,444)k,(w(i,j1,k,2)-wtmp(i,j1,k),i=1,6)
cc  444             format(1x,i1,6(1x,1pe9.2))
c                do i=1,6
c                  errw(i,k)=max(dabs(w(i,j1,k,2)-wtmp(i,j1,k)),
c     *                          errw(i,k))
c                  errf(i)=max(dabs(w(i,j1,k,2)),errf(i))
c                end do
c                end do
c                do i=1,4
cc                  write(6,555)i,(dpdf(i,j,j1,2),dptmp(i,j,j1),j=1,4)
cc  555             format(1x,i1,8(1x,1pe9.2))
c                do j=1,4
c                  erd(i,j)=max(dabs(dpdf(i,j,j1,2)-dptmp(i,j,j1)),
c     *                         erd(i,j))
c                  if (dabs(dpdf(i,j,j1,2)-dptmp(i,j,j1)).gt.1.d-6) then
c                    write(6,*)'j1,j2,i,j,dp,dp=',
c     *                        j1,j2,i,j,dpdf(i,j,j1,2),dptmp(i,j,j1)
c                    pause
c                  end if
c                end do
c                end do
c              end if
cc              pause
c            end do
c
c        end if
c
c..compute s2 flux along j2-1/2, add it to up(j1,j2,.) and up(j1,j2-1,.)
        if (method.eq.0) then
          do j1=n1a,n1b
            if (mask(j1,j2).ne.0.and.mask(j1,j2m1).ne.0) then
              a21=(a1(2,1,j1,2)+a1(2,1,j1,1))/2.d0
              a22=(a1(2,2,j1,2)+a1(2,2,j1,1))/2.d0
              do i=1,4
                cml(i,1)= a1(2,1,j1,1)*dpdf(i,1,j1,1)
     *                   +a1(2,2,j1,1)*dpdf(i,2,j1,1)
                cml(i,2)= a1(2,1,j1,1)*dpdf(i,3,j1,1)
     *                   +a1(2,2,j1,1)*dpdf(i,4,j1,1)
                cmr(i,1)= a1(2,1,j1,2)*dpdf(i,1,j1,2)
     *                   +a1(2,2,j1,2)*dpdf(i,2,j1,2)
                cmr(i,2)= a1(2,1,j1,2)*dpdf(i,3,j1,2)
     *                   +a1(2,2,j1,2)*dpdf(i,4,j1,2)
              end do

c         if (j1.eq.0.and.j2.eq.0.or.
c     *       j1.eq.0.and.j2m1.eq.0) then
c           write(6,*)'cml=',cml
c           write(6,*)'cmr=',cmr
c           pause
c         end if

              call smflux2dn (m,a21,a22,w(1,j1,4,1),w(1,j1,3,2),
     *                        aj(j1,1),aj(j1,2),cml,cmr,fxl,fxr,
     *                        almax(2))
c      write(60,*)'s2 flux  ',j1,j2
c      do i=1,6
c        write(60,601)w(i,j1,4,1),w(i,j1,3,2),fxl(i),fxr(i)
c  601   format(4(1x,1pe10.3))
c      end do

c         if (j1.eq.0.and.j2.eq.0) then
c           write(6,*)'fxr=',fxr
c           pause
c         end if
c         if (j1.eq.0.and.j2m1.eq.0) then
c           write(6,*)'fxl=',fxl
c           pause
c         end if

              do i=1,6
                up(j1,j2  ,i)=up(j1,j2  ,i)+fxr(i)/ds2
                up(j1,j2m1,i)=up(j1,j2m1,i)-fxl(i)/ds2
              end do
            end if
          end do
        elseif (method.eq.1) then
          do j1=n1a,n1b
            if (mask(j1,j2).ne.0.and.mask(j1,j2m1).ne.0) then
              call smflux2dn1 (a1(2,1,j1,1),a1(2,2,j1,1),
     *                         dpdf(1,1,j1,1),w(1,j1,4,1),
     *                         a1(2,1,j1,2),a1(2,2,j1,2),
     *                         dpdf(1,1,j1,2),w(1,j1,3,2),
     *                         fxl,fxr,almax(2))
              do i=1,6
                up(j1,j2  ,i)=up(j1,j2  ,i)+fxr(i)/ds2
                up(j1,j2m1,i)=up(j1,j2m1,i)-fxl(i)/ds2
              end do
            end if
          end do
        else
          write(6,*)'Error (getup2d) : method not supported'
          stop 20
        end if
c
c..if j2.le.n2b, then compute fluxes in the s1 direction
        if (j2.le.n2b) then
c
c..reset metrics and w
          do j1=n1a-1,n1b+1
            aj(j1,1)=aj(j1,2)
            a1(1,1,j1,1)=a1(1,1,j1,2)
            a1(1,2,j1,1)=a1(1,2,j1,2)
            a1(2,1,j1,1)=a1(2,1,j1,2)
            a1(2,2,j1,1)=a1(2,2,j1,2)
            do i=1,4
              do j=1,4
                dpdf(i,j,j1,1)=dpdf(i,j,j1,2)
              end do
            end do
            do k=1,5
              do i=1,m
                w(i,j1,k,1)=w(i,j1,k,2)
              end do
            end do
          end do
c
c..compute s1 flux along j2, add it to up(j1+1,j2,.) and up(j1,j2,.)
          if (method.eq.0) then
            do j1=n1a-1,n1b
              j1p1=j1+1
              if (mask(j1,j2).ne.0.and.mask(j1p1,j2).ne.0) then
                a11=(a1(1,1,j1p1,1)+a1(1,1,j1,1))/2.d0
                a12=(a1(1,2,j1p1,1)+a1(1,2,j1,1))/2.d0
                do i=1,4
                  cml(i,1)= a1(1,1,j1  ,1)*dpdf(i,1,j1  ,1)
     *                     +a1(1,2,j1  ,1)*dpdf(i,2,j1  ,1)
                  cml(i,2)= a1(1,1,j1  ,1)*dpdf(i,3,j1  ,1)
     *                     +a1(1,2,j1  ,1)*dpdf(i,4,j1  ,1)
                  cmr(i,1)= a1(1,1,j1p1,1)*dpdf(i,1,j1p1,1)
     *                     +a1(1,2,j1p1,1)*dpdf(i,2,j1p1,1)
                  cmr(i,2)= a1(1,1,j1p1,1)*dpdf(i,3,j1p1,1)
     *                     +a1(1,2,j1p1,1)*dpdf(i,4,j1p1,1)
                end do
                call smflux2dn (m,a11,a12,w(1,j1,2,1),w(1,j1p1,1,1),
     *                          aj(j1,1),aj(j1p1,1),cml,cmr,fxl,fxr,
     *                          almax(1))
c      write(60,*)'s1 flux  ',j1,j2
c      do i=1,6
c        write(60,602)w(i,j1,2,1),w(i,j1,1,1),fxl(i),fxr(i)
c  602   format(4(1x,1pe10.3))
c      end do
                do i=1,6
                  up(j1p1,j2,i)=up(j1p1,j2,i)+fxr(i)/ds1
                  up(j1  ,j2,i)=up(j1  ,j2,i)-fxl(i)/ds1
                end do
              end if
            end do
          elseif (method.eq.1) then
            do j1=n1a-1,n1b
              j1p1=j1+1
              if (mask(j1,j2).ne.0.and.mask(j1p1,j2).ne.0) then
                call smflux2dn1 (a1(1,1,j1  ,1),a1(1,2,j1  ,1),
     *                           dpdf(1,1,j1  ,1),w(1,j1  ,2,1),
     *                           a1(1,1,j1p1,2),a1(1,2,j1p1,2),
     *                           dpdf(1,1,j1p1,1),w(1,j1p1,1,1),
     *                           fxl,fxr,almax(1))
                do i=1,6
                  up(j1p1,j2,i)=up(j1p1,j2,i)+fxr(i)/ds1
                  up(j1  ,j2,i)=up(j1  ,j2,i)-fxl(i)/ds1
                end do
              end if
            end do
          else
            write(6,*)'Error (getup2d) : method not supported'
            stop 30
          end if
c
c..add free stream correction to up (if a non-Cartesian grid)
          if (icart.eq.0) then
            if (method.eq.0.or.method.eq.3) then
c              if (move.eq.0) then
                do j1=n1a,n1b
                  if (mask(j1,j2).ne.0) then
                    d1p=det(j1+1,j2)+det(j1,j2)
                    d1m=det(j1-1,j2)+det(j1,j2)
                    d2p=det(j1,j2+1)+det(j1,j2)
                    d2m=det(j1,j2-1)+det(j1,j2)
                    do k=1,2
                      da= ((rx(j1+1,j2,1,k)+rx(j1,j2,1,k))*d1p
     *                    -(rx(j1-1,j2,1,k)+rx(j1,j2,1,k))*d1m)
     *                    /(4.d0*ds1)
     *                   +((rx(j1,j2+1,2,k)+rx(j1,j2,2,k))*d2p
     *                    -(rx(j1,j2-1,2,k)+rx(j1,j2,2,k))*d2m)
     *                    /(4.d0*ds2)
                      if (k.eq.1) then
                        fxl(1)=-w(3,j1,5,1)/rho0
                        fxl(2)=-w(4,j1,5,1)/rho0
                      else
                        fxl(1)=-w(5,j1,5,1)/rho0
                        fxl(2)=-w(6,j1,5,1)/rho0
                      end if
c                      do i=1,2
c                        up(j1,j2,i)=up(j1,j2,i)+da*fxl(i)/det(j1,j2)
c                      end do
                    end do
                  end if
                end do
c              else
c moving grid case (not implemented yet)
c              end if
            end if
          end if
c
c..complete up (this should be done after each call to slope for the nonlinear case)
          do i=7,8
            im6=i-6
            do j1=n1a,n1b
c              up(j1,j2,i)=0.d0
              up(j1,j2,i)=w(im6,j1,5,1)
cccc              up(j1,j2,i)=u(j1,j2,im6)
            end do
          end do
c
c..twilight zone flow
          if (itz.ne.0) then
            t1=t
            if (iorder.eq.2) t1=t1+0.5d0*dt
            do j1=n1a,n1b
              call gethtzn (m,xy(j1,j2,1),xy(j1,j2,2),t1,htz)
              do i=1,8
                up(j1,j2,i)=up(j1,j2,i)+htz(i)
              end do
            end do
          end if
c
        end if
c
c..bottom of main loop over lines
      end do
c
c      write(6,*)'dt=',dt
c      do i=1,6
c        write(6,*)'errf =',i,errf(i)
c      end do
c      pause
c
c      write(6,*)'errw ='
c      do i=1,6
c        write(6,231)(errw(i,k),k=1,5),errf(i)
c  231   format(6(1x,1pe9.2))
c      end do
c      write(6,*)'erd ='
c      do i=1,4
c        write(6,432)(erd(i,j),j=1,4)
c  432   format(4(1x,1pe9.2))
c      end do
c      pause
c
      return
      end
c
c++++++++++++++++++++++++++++
c
      subroutine smmetrics2dn (nd1a,nd1b,md1a,md1b,nd2a,nd2b,j2,
     *                         rx,det,a1,aj)
c
      implicit real*8 (a-h,o-z)
      dimension rx(nd1a:nd1b,nd2a:nd2b,2,2),det(nd1a:nd1b,nd2a:nd2b),
     *          a1(2,2,md1a:md1b),aj(md1a:md1b)
      common / smgprmn / method,iorder,ilimit,iupwind,icart,itype,ifrc
c
      if (icart.eq.0) then
c
c non-Cartesian
        do j1=md1a,md1b
          a1(1,1,j1)=rx(j1,j2,1,1)
          a1(1,2,j1)=rx(j1,j2,1,2)
          a1(2,1,j1)=rx(j1,j2,2,1)
          a1(2,2,j1)=rx(j1,j2,2,2)
          aj(j1)=det(j1,j2)
c          write(6,*)j1,a1(1,1,j1),a1(1,2,j1),
c     *              a1(2,1,j1),a1(2,2,j1),aj(j1)
        end do
c        pause
c
      else
c
c Cartesian (for this case, ds1 and ds2 are the physical grid spacings
c so that the mapping is the identity...)
        do j1=md1a,md1b
          a1(1,1,j1)=1.d0
          a1(1,2,j1)=0.d0
          a1(2,1,j1)=0.d0
          a1(2,2,j1)=1.d0
          aj(j1)=1.d0
        end do
c
      end if
c
      return
      end
c
c++++++++++++++++++++
c
      subroutine smslope2dtz (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
     *                        j2,ds1,ds2,dt,t,xy,a1,aj,mask,u,w,dpdf)
c
c slope correction (TZ version)
c
      implicit real*8 (a-h,o-z)
      dimension xy(nd1a:nd1b,nd2a:nd2b,2),a1(2,2,md1a:md1b),
     *          aj(md1a:md1b),mask(nd1a:nd1b),
     *          u(nd1a:nd1b,nd2a:nd2b,m),w(m,md1a:md1b,5),
     *          dpdf(4,4,md1a:md1b),du(2,2),p(2,2),cpar(10)
      dimension htz(8)
      common / smgprmn / method,iorder,ilimit,iupwind,icart,itype,ifrc
      common / smgdatn / amu,alam,rho0
      common / tzflow / eptz,itz
c
c constitutive params
      cpar(1)=alam
      cpar(2)=amu
c
c..copy solution to w
      do k=1,5
        do j1=md1a,md1b
          do i=1,6
            w(i,j1,k)=u(j1,j2,i)
          end do
        end do
      end do
c
      if (iorder.gt.1.and.itz.ne.0) then
c
        r1=.5d0*dt/ds1
        r2=.5d0*dt/ds2
c
        do j1=n1a-1,n1b+1
          if (mask(j1).ne.0) then
c
            do i=1,6
              call ogDeriv (eptz,1,0,0,0,xy(j1,j2,1),xy(j1,j2,2),
     *                      0.d0,t,i-1,ut)
              call ogDeriv (eptz,0,0,0,0,xy(j1-1,j2,1),xy(j1-1,j2,2),
     *                      0.d0,t,i-1,u1m)
              call ogDeriv (eptz,0,0,0,0,xy(j1+1,j2,1),xy(j1+1,j2,2),
     *                      0.d0,t,i-1,u1p)
              call ogDeriv (eptz,0,0,0,0,xy(j1,j2-1,1),xy(j1,j2-1,2),
     *                      0.d0,t,i-1,u2m)
              call ogDeriv (eptz,0,0,0,0,xy(j1,j2+1,1),xy(j1,j2+1,2),
     *                      0.d0,t,i-1,u2p)
              w(i,j1,1)=w(i,j1,1)+.5d0*(ut*dt-.5d0*(u1p-u1m))
              w(i,j1,2)=w(i,j1,2)+.5d0*(ut*dt+.5d0*(u1p-u1m))
              w(i,j1,3)=w(i,j1,3)+.5d0*(ut*dt-.5d0*(u2p-u2m))
              w(i,j1,4)=w(i,j1,4)+.5d0*(ut*dt+.5d0*(u2p-u2m))
              w(i,j1,5)=w(i,j1,5)+.5d0*(ut*dt               )
            end do
c
          end if
        end do
c
      end if
c
c compute K(i,j,k,l)
      do j1=n1a-1,n1b+1
        if (mask(j1).ne.0) then
c
c first task: compute F = I + du/dx
          du1r=(u(j1+1,j2,7)-u(j1-1,j2,7))/(2.d0*ds1)
          du2r=(u(j1+1,j2,8)-u(j1-1,j2,8))/(2.d0*ds1)
          du1s=(u(j1,j2+1,7)-u(j1,j2-1,7))/(2.d0*ds2)
          du2s=(u(j1,j2+1,8)-u(j1,j2-1,8))/(2.d0*ds2)
          if (iorder.gt.1.and.itz.ne.0) then
            call ogDeriv (eptz,1,0,0,0,xy(j1-1,j2,1),xy(j1-1,j2,2),
     *                    0.d0,t,6,ut1rm)
            call ogDeriv (eptz,1,0,0,0,xy(j1+1,j2,1),xy(j1+1,j2,2),
     *                    0.d0,t,6,ut1rp)
            call ogDeriv (eptz,1,0,0,0,xy(j1-1,j2,1),xy(j1-1,j2,2),
     *                    0.d0,t,7,ut2rm)
            call ogDeriv (eptz,1,0,0,0,xy(j1+1,j2,1),xy(j1+1,j2,2),
     *                    0.d0,t,7,ut2rp)
            call ogDeriv (eptz,1,0,0,0,xy(j1,j2-1,1),xy(j1,j2-1,2),
     *                    0.d0,t,6,ut1sm)
            call ogDeriv (eptz,1,0,0,0,xy(j1,j2+1,1),xy(j1,j2+1,2),
     *                    0.d0,t,6,ut1sp)
            call ogDeriv (eptz,1,0,0,0,xy(j1,j2-1,1),xy(j1,j2-1,2),
     *                    0.d0,t,7,ut2sm)
            call ogDeriv (eptz,1,0,0,0,xy(j1,j2+1,1),xy(j1,j2+1,2),
     *                    0.d0,t,7,ut2sp)
            du1r=du1r+.5d0*r1*(ut1rp-ut1rm)
            du2r=du2r+.5d0*r1*(ut2rp-ut2rm)
            du1s=du1s+.5d0*r2*(ut1sp-ut1sm)
            du2s=du2s+.5d0*r2*(ut2sp-ut2sm)
c
c
c            du1r=(u(j1+1,j2,7)-u(j1-1,j2,7)
c     *            +.5d0*dt*(u(j1+1,j2,1)-u(j1-1,j2,1)))/(2.d0*ds1)
c            du2r=(u(j1+1,j2,8)-u(j1-1,j2,8)
c     *            +.5d0*dt*(u(j1+1,j2,2)-u(j1-1,j2,2)))/(2.d0*ds1)
c            du1s=(u(j1,j2+1,7)-u(j1,j2-1,7)
c     *            +.5d0*dt*(u(j1,j2+1,1)-u(j1,j2-1,1)))/(2.d0*ds2)
c            du2s=(u(j1,j2+1,8)-u(j1,j2-1,8)
c     *            +.5d0*dt*(u(j1,j2+1,2)-u(j1,j2-1,2)))/(2.d0*ds2)
c
c            if (itz.ne.0) then
c              call gethtzn (m,xy(j1-1,j2,1),xy(j1-1,j2,2),t,htz)
c              du1r=du1r-.25d0*dt*htz(7)/ds1
c              du2r=du2r-.25d0*dt*htz(8)/ds1
c              call gethtzn (m,xy(j1+1,j2,1),xy(j1+1,j2,2),t,htz)
c              du1r=du1r+.25d0*dt*htz(7)/ds1
c              du2r=du2r+.25d0*dt*htz(8)/ds1
c              call gethtzn (m,xy(j1,j2-1,1),xy(j1,j2-1,2),t,htz)
c              du1s=du1s-.25d0*dt*htz(7)/ds2
c              du2s=du2s-.25d0*dt*htz(8)/ds2
c              call gethtzn (m,xy(j1,j2+1,1),xy(j1,j2+1,2),t,htz)
c              du1s=du1s+.25d0*dt*htz(7)/ds2
c              du2s=du2s+.25d0*dt*htz(8)/ds2
c            end if
c
c
          end if
c
          du(1,1)=a1(1,1,j1)*du1r+a1(2,1,j1)*du1s
          du(1,2)=a1(1,2,j1)*du1r+a1(2,2,j1)*du1s
          du(2,1)=a1(1,1,j1)*du2r+a1(2,1,j1)*du2s
          du(2,2)=a1(1,2,j1)*du2r+a1(2,2,j1)*du2s
c
c now compute dpdf
          ideriv=1
          call smgetdp (du,p,dpdf(1,1,j1),cpar,ideriv,itype)
c
        end if
      end do
c
      return
      end
c
c+++++++++++++++++++
c
      subroutine smslope2dn (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
     *                       j2,ds1,ds2,dt,t,xy,a1,aj,mask,u,w,dpdf,f1)
c
c slope correction
c
      implicit real*8 (a-h,o-z)
      dimension xy(nd1a:nd1b,nd2a:nd2b,2),a1(2,2,md1a:md1b),
     *          aj(md1a:md1b),mask(nd1a:nd1b),
     *          u(nd1a:nd1b,nd2a:nd2b,m),w(m,md1a:md1b,5),
     *          dpdf(4,4,md1a:md1b),f1(nd1a:nd1b,nd2a:nd2b,2),
     *          du(2,2),p(2,2),cpar(10)
c      dimension pp(2,2),dp(4,4)
      dimension al(6),el(6,6),er(6,6),utz(8),htz(8),ptz(4)
      common / smgprmn / method,iorder,ilimit,iupwind,icart,itype,ifrc
      common / smgdatn / amu,alam,rho0
      common / tzflow / eptz,itz
c      data ilimit, iupwind / 0, 0 /   ! setting these values to 1,1 makes the errors smoother but bigger...
      data eps / 1.d-14 /    ! tolerance for zero eigenvalue
c
c testing
c      ilimit=0
c      iupwind=0
c
c      write(6,*)ilimit,iupwind
c      pause
c
c..copy solution to w
      do k=1,5
        do j1=md1a,md1b
          do i=1,6
            w(i,j1,k)=u(j1,j2,i)
          end do
        end do
      end do
c
      cpar(1)=alam
      cpar(2)=amu
c
      if (iorder.eq.1) then
c
        do j1=n1a-1,n1b+1
          if (mask(j1).ne.0) then
c
c compute F = I + du/dx
            du1r=(u(j1+1,j2,7)-u(j1-1,j2,7))/(2.d0*ds1)
            du2r=(u(j1+1,j2,8)-u(j1-1,j2,8))/(2.d0*ds1)
            du1s=(u(j1,j2+1,7)-u(j1,j2-1,7))/(2.d0*ds2)
            du2s=(u(j1,j2+1,8)-u(j1,j2-1,8))/(2.d0*ds2)
            du(1,1)=a1(1,1,j1)*du1r+a1(2,1,j1)*du1s
            du(1,2)=a1(1,2,j1)*du1r+a1(2,2,j1)*du1s
            du(2,1)=a1(1,1,j1)*du2r+a1(2,1,j1)*du2s
            du(2,2)=a1(1,2,j1)*du2r+a1(2,2,j1)*du2s

c            if (j1.eq.0.and.j2.eq.0) then
c              write(6,*)'du =',du
c              pause
c            end if
c
c now compute dpdf, i.e. K(i,j,k,l)
            ideriv=1
            call smgetdp (du,p,dpdf(1,1,j1),cpar,ideriv,itype)

c            if (j1.eq.0.and.j2.eq.0) then
c              write(6,*)'dpdf ='
c              do ii=1,4
c              do jj=1,4
c                write(6,*)dpdf(ii,jj,j1)
c              end do
c              end do
c              pause
c            end if
c
          end if
        end do
c
      else
c
        r1=.5d0*dt/ds1
        r2=.5d0*dt/ds2
c
        do j1=n1a-1,n1b+1
          if (mask(j1).ne.0) then
c
c compute F = I + du/dx
            if (.true.) then

c  centered approximations (old)
            if (.true.) then
            du1r=(u(j1+1,j2,7)-u(j1-1,j2,7)
     *            +.5d0*dt*(u(j1+1,j2,1)-u(j1-1,j2,1)))/(2.d0*ds1)
            du2r=(u(j1+1,j2,8)-u(j1-1,j2,8)
     *            +.5d0*dt*(u(j1+1,j2,2)-u(j1-1,j2,2)))/(2.d0*ds1)
            du1s=(u(j1,j2+1,7)-u(j1,j2-1,7)
     *            +.5d0*dt*(u(j1,j2+1,1)-u(j1,j2-1,1)))/(2.d0*ds2)
            du2s=(u(j1,j2+1,8)-u(j1,j2-1,8)
     *            +.5d0*dt*(u(j1,j2+1,2)-u(j1,j2-1,2)))/(2.d0*ds2)
            else
            du1r=(u(j1+1,j2,7)-u(j1-1,j2,7))/(2.d0*ds1)
            du2r=(u(j1+1,j2,8)-u(j1-1,j2,8))/(2.d0*ds1)
            du1s=(u(j1,j2+1,7)-u(j1,j2-1,7))/(2.d0*ds2)
            du2s=(u(j1,j2+1,8)-u(j1,j2-1,8))/(2.d0*ds2)
            end if
c
            else
c
c  slope-limited approximations
            du1rm=(u(j1,j2,7)-u(j1-1,j2,7)
     *             +.5d0*dt*(u(j1,j2,1)-u(j1-1,j2,1)))/ds1
            du1rp=(u(j1+1,j2,7)-u(j1,j2,7)
     *             +.5d0*dt*(u(j1+1,j2,1)-u(j1,j2,1)))/ds1
            if (du1rm*du1rp.le.0.d0) then
              du1r=0.d0
            else
              if (dabs(du1rm).lt.dabs(du1rp)) then
                du1r=du1rm
              else
                du1r=du1rp
              end if
            end if
c
            du2rm=(u(j1,j2,8)-u(j1-1,j2,8)
     *            +.5d0*dt*(u(j1,j2,2)-u(j1-1,j2,2)))/ds1
            du2rp=(u(j1+1,j2,8)-u(j1,j2,8)
     *            +.5d0*dt*(u(j1+1,j2,2)-u(j1,j2,2)))/ds1
            if (du2rm*du2rp.le.0.d0) then
              du2r=0.d0
            else
              if (dabs(du2rm).lt.dabs(du2rp)) then
                du2r=du2rm
              else
                du2r=du2rp
              end if
            end if
c
            du1sm=(u(j1,j2,7)-u(j1,j2-1,7)
     *            +.5d0*dt*(u(j1,j2,1)-u(j1,j2-1,1)))/ds2
            du1sp=(u(j1,j2+1,7)-u(j1,j2,7)
     *            +.5d0*dt*(u(j1,j2+1,1)-u(j1,j2,1)))/ds2
            if (du1sm*du1sp.le.0.d0) then
              du1s=0.d0
            else
              if (dabs(du1sm).lt.dabs(du1sp)) then
                du1s=du1sm
              else
                du1s=du1sp
              end if
            end if
c
            du2sm=(u(j1,j2,8)-u(j1,j2-1,8)
     *            +.5d0*dt*(u(j1,j2,2)-u(j1,j2-1,2)))/ds2
            du2sp=(u(j1,j2+1,8)-u(j1,j2,8)
     *            +.5d0*dt*(u(j1,j2+1,2)-u(j1,j2,2)))/ds2
            if (du2sm*du2sp.le.0.d0) then
              du2s=0.d0
            else
              if (dabs(du2sm).lt.dabs(du2sp)) then
                du2s=du2sm
              else
                du2s=du2sp
              end if
            end if
c
            end if
c
            if (itz.ne.0) then
              call gethtzn (m,xy(j1-1,j2,1),xy(j1-1,j2,2),t,htz)
              du1r=du1r-.25d0*dt*htz(7)/ds1
              du2r=du2r-.25d0*dt*htz(8)/ds1
              call gethtzn (m,xy(j1+1,j2,1),xy(j1+1,j2,2),t,htz)
              du1r=du1r+.25d0*dt*htz(7)/ds1
              du2r=du2r+.25d0*dt*htz(8)/ds1
              call gethtzn (m,xy(j1,j2-1,1),xy(j1,j2-1,2),t,htz)
              du1s=du1s-.25d0*dt*htz(7)/ds2
              du2s=du2s-.25d0*dt*htz(8)/ds2
              call gethtzn (m,xy(j1,j2+1,1),xy(j1,j2+1,2),t,htz)
              du1s=du1s+.25d0*dt*htz(7)/ds2
              du2s=du2s+.25d0*dt*htz(8)/ds2
            end if
c
            du(1,1)=a1(1,1,j1)*du1r+a1(2,1,j1)*du1s
            du(1,2)=a1(1,2,j1)*du1r+a1(2,2,j1)*du1s
            du(2,1)=a1(1,1,j1)*du2r+a1(2,1,j1)*du2s
            du(2,2)=a1(1,2,j1)*du2r+a1(2,2,j1)*du2s
c
c now compute dpdf, i.e. K(i,j,k,l)
            ideriv=1
            call smgetdp (du,p,dpdf(1,1,j1),cpar,ideriv,itype)
c
c
c build w(1:m,j1,1:5), start with TZ contribution if needed
            if (itz.ne.0) then
              call gethtzn (m,xy(j1,j2,1),xy(j1,j2,2),t,htz)
              do k=1,5
                do i=1,6
                  w(i,j1,k)=w(i,j1,k)+.5d0*dt*htz(i)
                end do
              end do
            else
              if (ifrc.ne.0) then
                do k=1,5
                  do i=1,2
                    w(i,j1,k)=w(i,j1,k)+.5d0*dt*f1(j1,j2,i)
                  end do
                end do
              end if
            end if
c
c..s1 direction
            if (icart.eq.0) then
              call smeig2dn (a1(1,1,j1),a1(1,2,j1),dpdf(1,1,j1),
     *                       al,el,er,ier)
            else
              call smeig2dn1 (dpdf(1,1,j1),al,el,er,ier)
            end if
c
            if (ier.ne.0) then
              write(6,*)'Error (smslope2dn) : du =',du(1,1),du(1,2),
     *                                              du(2,1),du(2,2)
              stop 40
            end if
c
            do j=1,6
              alphal=0.d0
              alphar=0.d0
              do i=1,6
                alphal=alphal+el(j,i)*(u(j1,j2,i)-u(j1-1,j2,i))
                alphar=alphar+el(j,i)*(u(j1+1,j2,i)-u(j1,j2,i))
              end do
c
              if (ilimit.eq.0) then
                alphal=.5d0*(alphal+alphar)
                alphar=alphal
              end if
c
              if (alphal*alphar.gt.0.d0) then
                if (dabs(alphal).lt.dabs(alphar)) then
                  alp=alphal
                else
                  alp=alphar
                end if
c
                if (dabs(al(j)).lt.eps) then
                  do i=1,6
                    w(i,j1,1)=w(i,j1,1)-.5d0*alp*er(i,j)
                    w(i,j1,2)=w(i,j1,2)+.5d0*alp*er(i,j)
                  end do
                else
                  alam0=r1*al(j)
                  tmp=alam0*alp
                  do i=1,6
                    tmp2=tmp*er(i,j)
                    w(i,j1,3)=w(i,j1,3)-tmp2
                    w(i,j1,4)=w(i,j1,4)-tmp2
                    w(i,j1,5)=w(i,j1,5)-tmp2
                    if (iupwind.ne.0) then
                      w(i,j1,1)=w(i,j1,1)
     *                           -(min(alam0,0.d0)+.5d0)*alp*er(i,j)
                      w(i,j1,2)=w(i,j1,2)
     *                           -(max(alam0,0.d0)-.5d0)*alp*er(i,j)
                    else
                      w(i,j1,1)=w(i,j1,1)-(alam0+.5d0)*alp*er(i,j)
                      w(i,j1,2)=w(i,j1,2)-(alam0-.5d0)*alp*er(i,j)
                    end if
                  end do
                end if
c
              end if
            end do
c
c..s2 direction
            if (icart.eq.0) then
              call smeig2dn (a1(2,1,j1),a1(2,2,j1),dpdf(1,1,j1),
     *                       al,el,er,ier)
            else
              call smeig2dn2 (dpdf(1,1,j1),al,el,er,ier)
            end if
c
            if (ier.ne.0) then
              write(6,*)'Error (smslope2dn) : du =',du(1,1),du(1,2),
     *                                              du(2,1),du(2,2)
              stop 50
            end if
c
            do j=1,6
              alphal=0.d0
              alphar=0.d0
              do i=1,6
                alphal=alphal+el(j,i)*(u(j1,j2,i)-u(j1,j2-1,i))
                alphar=alphar+el(j,i)*(u(j1,j2+1,i)-u(j1,j2,i))
              end do
c
              if (ilimit.eq.0) then
                alphal=.5d0*(alphal+alphar)
                alphar=alphal
              end if
c
              if (alphal*alphar.gt.0.d0) then
                if (dabs(alphal).lt.dabs(alphar)) then
                  alp=alphal
                else
                  alp=alphar
                end if
c
                if (dabs(al(j)).lt.eps) then
                  do i=1,6
                    w(i,j1,3)=w(i,j1,3)-.5d0*alp*er(i,j)
                    w(i,j1,4)=w(i,j1,4)+.5d0*alp*er(i,j)
                  end do
                else
                  alam0=r2*al(j)
                  tmp=alam0*alp
                  do i=1,6
                    tmp2=tmp*er(i,j)
                    w(i,j1,1)=w(i,j1,1)-tmp2
                    w(i,j1,2)=w(i,j1,2)-tmp2
                    w(i,j1,5)=w(i,j1,5)-tmp2
                    if (iupwind.ne.0) then
                      w(i,j1,3)=w(i,j1,3)
     *                           -(min(alam0,0.d0)+.5d0)*alp*er(i,j)
                      w(i,j1,4)=w(i,j1,4)
     *                           -(max(alam0,0.d0)-.5d0)*alp*er(i,j)
                    else
                      w(i,j1,3)=w(i,j1,3)-(alam0+.5d0)*alp*er(i,j)
                      w(i,j1,4)=w(i,j1,4)-(alam0-.5d0)*alp*er(i,j)
                    end if
                  end do
                end if
c
              end if
            end do
c
          end if
        end do
c
      end if
c
      return
      end
c
c+++++++++++++++++++
c
      subroutine smgetdp (du,p,dpdf,cpar,ideriv,itype)
c
      implicit real*8 (a-h,o-z)
      dimension du(2,2),f(2,2),s(2,2),p(2,2),dpdf(4,4),cpar(10)
      dimension v(2,2),u(2,2),dh11(4),dh12(4),dh22(4),dd11(4),dd22(4),
     *          ds11(4),ds22(4),dv11(4),dv12(4),dv21(4),dv22(4),
     *          dw11(4),dw22(4),du11(4),du12(4),du21(4),du22(4)
      dimension p1(2,2),dpdf1(4,4)
      data tol / 1.d-14 /
c
      alam=cpar(1)
      amu=cpar(2)
c
c      if (itype.ne.2) then
c        write(6,*)'wrong itype'
c        stop
c      end if
c
      if (itype.eq.1) then
c
c  linear reduction, i.e. f=identity+du, du=tiny
c
        s(1,1)=du(1,1)                         ! this is E(i,j)=(du + du^T)/2, for now
        s(1,2)=.5d0*(du(1,2)+du(2,1))
        s(2,1)=s(1,2)
        s(2,2)=du(2,2)
        trace=s(1,1)+s(2,2)                    ! this is Tr(E)
        s(1,1)=alam*trace+2.d0*amu*s(1,1)      ! this is S(i,j)
        s(1,2)=           2.d0*amu*s(1,2)
        s(2,1)=           2.d0*amu*s(2,1)
        s(2,2)=alam*trace+2.d0*amu*s(2,2)
        p(1,1)=s(1,1)                          ! this is P(i,j) based on the current F(i,j)
        p(1,2)=s(1,2)
        p(2,1)=s(2,1)
        p(2,2)=s(2,2)
c
c        write(6,*)f
c        write(6,*)s
c        write(6,*)p
c        pause
c
        if (ideriv.eq.0) return
c
        dpdf(1,1)=alam+2.d0*amu   ! K(1,1,1,1)
        dpdf(1,2)=0.d0            ! K(1,1,1,2)
        dpdf(1,3)=0.d0            ! K(1,1,2,1)
        dpdf(1,4)=alam            ! K(1,1,2,2)
        dpdf(2,1)=0.d0            ! K(1,2,1,1)
        dpdf(2,2)=amu             ! K(1,2,1,2)
        dpdf(2,3)=amu             ! K(1,2,2,1)
        dpdf(2,4)=0.d0            ! K(1,2,2,2)
        dpdf(3,1)=0.d0            ! K(2,1,1,1)
        dpdf(3,2)=amu             ! K(2,1,1,2)
        dpdf(3,3)=amu             ! K(2,1,2,1)
        dpdf(3,4)=0.d0            ! K(2,1,2,2)
        dpdf(4,1)=alam            ! K(2,2,1,1)
        dpdf(4,2)=0.d0            ! K(2,2,1,2)
        dpdf(4,3)=0.d0            ! K(2,2,2,1)
        dpdf(4,4)=alam+2.d0*amu   ! K(2,2,2,2)
c
      elseif (itype.eq.2) then
c
c full SVK case
c
        f(1,1)=1.d0+du(1,1)
        f(1,2)=     du(1,2)
        f(2,1)=     du(2,1)
        f(2,2)=1.d0+du(2,2)
        s(1,1)=.5d0*(f(1,1)*f(1,1)+f(2,1)*f(2,1)-1.d0)   ! this is E(i,j), for now
        s(1,2)=.5d0*(f(1,1)*f(1,2)+f(2,1)*f(2,2)     )
        s(2,1)=s(1,2)
        s(2,2)=.5d0*(f(1,2)*f(1,2)+f(2,2)*f(2,2)-1.d0)
        trace=s(1,1)+s(2,2)                              ! this is Tr(E)
        s(1,1)=alam*trace+2.d0*amu*s(1,1)                ! this is S(i,j)
        s(1,2)=           2.d0*amu*s(1,2)
        s(2,1)=s(1,2)
        s(2,2)=alam*trace+2.d0*amu*s(2,2)
        p(1,1)=s(1,1)*f(1,1)+s(1,2)*f(1,2)               ! this is P(i,j) based on the current F(i,j)
        p(1,2)=s(1,1)*f(2,1)+s(1,2)*f(2,2)
        p(2,1)=s(2,1)*f(1,1)+s(2,2)*f(1,2)
        p(2,2)=s(2,1)*f(2,1)+s(2,2)*f(2,2)
c
c        write(6,*)f
c        write(6,*)s
c        write(6,*)p
c        pause
c
        if (ideriv.eq.0) return
c
        dpdf(1,1)=alam*f(1,1)*f(1,1)+amu*f(1,1)*f(1,1)   ! K(1,1,1,1)
        dpdf(1,2)=alam*f(1,1)*f(1,2)+amu*f(1,2)*f(1,1)   ! K(1,1,1,2)
        dpdf(1,3)=alam*f(1,1)*f(2,1)+amu*f(1,1)*f(2,1)   ! K(1,1,2,1)
        dpdf(1,4)=alam*f(1,1)*f(2,2)+amu*f(1,2)*f(2,1)   ! K(1,1,2,2)
        dpdf(2,1)=alam*f(2,1)*f(1,1)+amu*f(2,1)*f(1,1)   ! K(1,2,1,1)
        dpdf(2,2)=alam*f(2,1)*f(1,2)+amu*f(2,2)*f(1,1)   ! K(1,2,1,2)
        dpdf(2,3)=alam*f(2,1)*f(2,1)+amu*f(2,1)*f(2,1)   ! K(1,2,2,1)
        dpdf(2,4)=alam*f(2,1)*f(2,2)+amu*f(2,2)*f(2,1)   ! K(1,2,2,2)
        dpdf(3,1)=alam*f(1,2)*f(1,1)+amu*f(1,1)*f(1,2)   ! K(2,1,1,1)
        dpdf(3,2)=alam*f(1,2)*f(1,2)+amu*f(1,2)*f(1,2)   ! K(2,1,1,2)
        dpdf(3,3)=alam*f(1,2)*f(2,1)+amu*f(1,1)*f(2,2)   ! K(2,1,2,1)
        dpdf(3,4)=alam*f(1,2)*f(2,2)+amu*f(1,2)*f(2,2)   ! K(2,1,2,2)
        dpdf(4,1)=alam*f(2,2)*f(1,1)+amu*f(2,1)*f(1,2)   ! K(2,2,1,1)
        dpdf(4,2)=alam*f(2,2)*f(1,2)+amu*f(2,2)*f(1,2)   ! K(2,2,1,2)
        dpdf(4,3)=alam*f(2,2)*f(2,1)+amu*f(2,1)*f(2,2)   ! K(2,2,2,1)
        dpdf(4,4)=alam*f(2,2)*f(2,2)+amu*f(2,2)*f(2,2)   ! K(2,2,2,2)
c
        dpdf(1,1)=dpdf(1,1)+amu*(f(1,1)*f(1,1)+f(1,2)*f(1,2))   ! K(1,1,1,1)
        dpdf(1,3)=dpdf(1,3)+amu*(f(1,1)*f(2,1)+f(1,2)*f(2,2))   ! K(1,1,2,1)
        dpdf(2,1)=dpdf(2,1)+amu*(f(2,1)*f(1,1)+f(2,2)*f(1,2))   ! K(1,2,1,1)
        dpdf(2,3)=dpdf(2,3)+amu*(f(2,1)*f(2,1)+f(2,2)*f(2,2))   ! K(1,2,2,1)
        dpdf(3,2)=dpdf(3,2)+amu*(f(1,1)*f(1,1)+f(1,2)*f(1,2))   ! K(2,1,1,2)
        dpdf(3,4)=dpdf(3,4)+amu*(f(1,1)*f(2,1)+f(1,2)*f(2,2))   ! K(2,1,2,2)
        dpdf(4,2)=dpdf(4,2)+amu*(f(2,1)*f(1,1)+f(2,2)*f(1,2))   ! K(2,2,1,2)
        dpdf(4,4)=dpdf(4,4)+amu*(f(2,1)*f(2,1)+f(2,2)*f(2,2))   ! K(2,2,2,2)
c
        dpdf(1,1)=dpdf(1,1)+s(1,1)   ! K(1,1,1,1)
        dpdf(1,2)=dpdf(1,2)+s(1,2)   ! K(1,1,1,2)
        dpdf(2,3)=dpdf(2,3)+s(1,1)   ! K(1,2,2,1)
        dpdf(2,4)=dpdf(2,4)+s(1,2)   ! K(1,2,2,2)
        dpdf(3,1)=dpdf(3,1)+s(2,1)   ! K(2,1,1,1)
        dpdf(3,2)=dpdf(3,2)+s(2,2)   ! K(2,1,1,2)
        dpdf(4,3)=dpdf(4,3)+s(2,1)   ! K(2,2,2,1)
        dpdf(4,4)=dpdf(4,4)+s(2,2)   ! K(2,2,2,2)
c
      elseif (itype.eq.3) then
c
c rotated linear model
c
        dutol=1.d-6
        dumax=max(dabs(du(1,1)),dabs(du(1,2)),
     *            dabs(du(2,1)),dabs(du(2,2)))
        if (dumax.lt.dutol) then
          p(1,1)=0.d0
          p(1,2)=0.d0
          p(2,1)=0.d0
          p(2,2)=0.d0
          if (ideriv.eq.0) return
          dpdf(1,1)=alam+2.d0*amu   ! K(1,1,1,1)
          dpdf(1,2)=0.d0            ! K(1,1,1,2)
          dpdf(1,3)=0.d0            ! K(1,1,2,1)
          dpdf(1,4)=alam            ! K(1,1,2,2)
          dpdf(2,1)=0.d0            ! K(1,2,1,1)
          dpdf(2,2)=amu             ! K(1,2,1,2)
          dpdf(2,3)=amu             ! K(1,2,2,1)
          dpdf(2,4)=0.d0            ! K(1,2,2,2)
          dpdf(3,1)=0.d0            ! K(2,1,1,1)
          dpdf(3,2)=amu             ! K(2,1,1,2)
          dpdf(3,3)=amu             ! K(2,1,2,1)
          dpdf(3,4)=0.d0            ! K(2,1,2,2)
          dpdf(4,1)=alam            ! K(2,2,1,1)
          dpdf(4,2)=0.d0            ! K(2,2,1,2)
          dpdf(4,3)=0.d0            ! K(2,2,2,1)
          dpdf(4,4)=alam+2.d0*amu   ! K(2,2,2,2)
          return
        end if
c
        f(1,1)=1.d0+du(1,1)    ! deformation gradient tensor
        f(1,2)=     du(1,2)
        f(2,1)=     du(2,1)
        f(2,2)=1.d0+du(2,2)
c
        detf=f(1,1)*f(2,2)-f(1,2)*f(2,1)   ! det(F)
c
        h11=f(1,1)*f(1,1)+f(2,1)*f(2,1)   ! components of H=F'*F
        h12=f(1,1)*f(1,2)+f(2,1)*f(2,2)
        h22=f(1,2)*f(1,2)+f(2,2)*f(2,2)
c
        temp=h22-h11            ! compute Givens rotation to diagonalize H
        fact=dsqrt(temp**2+4.d0*h12**2)
        if (temp.ge.0.d0) then
          denom=temp+fact+tol
          t=-2.d0*h12/denom
        else
          denom=temp-fact-tol
          t=-2.d0*h12/denom
        end if
        cc=1/dsqrt(1+t**2)
        ss=cc*t
c
        d11=h11*cc**2+2.d0*h12*cc*ss+h22*ss**2  ! eigenvalues of H
        d22=h11*ss**2-2.d0*h12*cc*ss+h22*cc**2

        if (d11.lt.-tol) then
          write(6,*)'Error (smgetp0) : d11 negative'
          stop 111
        end if
        if (d22.lt.-tol) then
          write(6,*)'Error (smgetp0) : d22 negative'
          stop 222
        end if
c
        if (d11.ge.d22) then       ! compute S and V in SVD of F
          s11=dsqrt(max(d11,tol))
          s22=dsqrt(max(d22,tol))
          v(1,1)= cc
          v(1,2)=-ss
          v(2,1)= ss
          v(2,2)= cc
        else
          s11=dsqrt(max(d22,tol))
          s22=dsqrt(max(d11,tol))
          v(1,1)=-ss
          v(1,2)=-cc
          v(2,1)= cc
          v(2,2)=-ss
        end if
        if (detf.lt.0.d0) then
          s22=-s22
          do i=1,4
            ds22(i)=-ds22(i)
          end do
        end if
c
        u(1,1)=(f(1,1)*v(1,1)+f(1,2)*v(2,1))/s11  ! compute U in U*S*V'=F
        u(2,1)=(f(2,1)*v(1,1)+f(2,2)*v(2,1))/s11
        u(1,2)=-u(2,1)
        u(2,2)= u(1,1)
c
        trace=s11+s22-2.d0                   ! diagonal components of stress
        w11=alam*trace+2.d0*amu*(s11-1.d0)
        w22=alam*trace+2.d0*amu*(s22-1.d0)
c
c       *wdh* CHECK this: is this P or P' ??
        p(1,1)=w11*v(1,1)*u(1,1)+w22*v(1,2)*u(1,2)   ! nominal stress
        p(1,2)=w11*v(1,1)*u(2,1)+w22*v(1,2)*u(2,2)
        p(2,1)=w11*v(2,1)*u(1,1)+w22*v(2,2)*u(1,2)
        p(2,2)=w11*v(2,1)*u(2,1)+w22*v(2,2)*u(2,2)
c
        if( .true. )then ! *wdh* debugging
          write(*,'(" smgetdp: d11,d22=",2e10.2," s11,s22=",
     & 2e10.2," Phat=",2e10.2)') 
     &      d11,d22,s11,s22,w11,w22
          write(*,'(" du= [",4f6.2,"]")') du 
          write(*,'(" F = [",4f6.2,"]")') f
          write(*,'(" H = [",4f6.2,"]")') h11,h21,h12,h22
          write(*,'(" U = [",4f6.2,"]")') u 
          write(*,'(" V = [",4f6.2,"]")') v 
          write(*,'(" P = [",4f6.2,"]")') p 

        ff11=s11*v(1,1)*u(1,1)+s22*v(1,2)*u(1,2)   ! F = U Sigma V' ? 
        ff12=s11*v(2,1)*u(1,1)+s22*v(2,2)*u(1,2)
        ff21=s11*v(1,1)*u(2,1)+s22*v(1,2)*u(2,2)
        ff22=s11*v(2,1)*u(2,1)+s22*v(2,2)*u(2,2)

          write(*,'(" F=? U*Sigma*V^t = [",4f6.2,"]")') 
     &            ff11,ff21,ff12,ff22
        end if


        if (ideriv.eq.0) return
c
        if (fact.lt.1.d-6.or.dabs(temp).lt.1.d-6) then
          delta=1.d-9   ! numerical derivative for the difficult analytical cases
          do i=1,2
          do j=1,2
            k=2*(i-1)+j
            f(i,j)=f(i,j)+delta
            call smgetp0 (f,p1,alam,amu)
            dpdf(1,k)=(p1(1,1)-p(1,1))/delta
            dpdf(2,k)=(p1(1,2)-p(1,2))/delta
            dpdf(3,k)=(p1(2,1)-p(2,1))/delta
            dpdf(4,k)=(p1(2,2)-p(2,2))/delta
            f(i,j)=f(i,j)-delta
          end do
          end do
          return
        end if
c
        dh11(1)=2.d0*f(1,1)
        dh11(2)=0.d0
        dh11(3)=2.d0*f(2,1)
        dh11(4)=0.d0
        dh12(1)=f(1,2)
        dh12(2)=f(1,1)
        dh12(3)=f(2,2)
        dh12(4)=f(2,1)
        dh22(1)=0.d0
        dh22(2)=2.d0*f(1,2)
        dh22(3)=0.d0
        dh22(4)=2.d0*f(2,2)
c
        if (temp.ge.0.d0) then
          dtdh11=-2.d0*h12*(1.d0+temp/fact)/denom**2
          dtdh22=-dtdh11
          dtdh12=-2.d0/denom+(8.d0*h12**2/fact)/denom**2
        else
          dtdh11=-2.d0*h12*(1.d0-temp/fact)/denom**2
          dtdh22=-dtdh11
          dtdh12=-2.d0/denom-(8.d0*h12**2/fact)/denom**2
        end if
        dc=-t*cc**3
        ds=dc*t+cc
c
        do i=1,4
          dd11(i)=dh11(i)*cc**2+2.d0*dh12(i)*cc*ss+dh22(i)*ss**2
     *            +2.d0*((h11*cc+h12*ss)*dc+(h12*cc+h22*ss)*ds)
     *            *(dtdh11*dh11(i)+dtdh12*dh12(i)+dtdh22*dh22(i))
          dd22(i)=dh11(i)*ss**2-2.d0*dh12(i)*cc*ss+dh22(i)*cc**2
     *            +2.d0*((h11*ss-h12*cc)*ds+(-h12*ss+h22*cc)*dc)
     *            *(dtdh11*dh11(i)+dtdh12*dh12(i)+dtdh22*dh22(i))
        end do
c
        if (d11.ge.d22) then       ! compute S and V in SVD of F
          do i=1,4
            ds11(i)=.5d0*dd11(i)/s11
            ds22(i)=.5d0*dd22(i)/s22
            dv11(i)= dc*(dtdh11*dh11(i)+dtdh12*dh12(i)+dtdh22*dh22(i))
            dv12(i)=-ds*(dtdh11*dh11(i)+dtdh12*dh12(i)+dtdh22*dh22(i))
            dv21(i)=-dv12(i)
            dv22(i)= dv11(i)
          end do
        else
          do i=1,4
            ds11(i)=.5d0*dd22(i)/s11
            ds22(i)=.5d0*dd11(i)/s22
            dv11(i)=-ds*(dtdh11*dh11(i)+dtdh12*dh12(i)+dtdh22*dh22(i))
            dv12(i)=-dc*(dtdh11*dh11(i)+dtdh12*dh12(i)+dtdh22*dh22(i))
            dv21(i)=-dv12(i)
            dv22(i)= dv11(i)
          end do
        end if
        if (detf.lt.0.d0) then
          do i=1,4
            ds22(i)=-ds22(i)
          end do
        end if
c
        du11(1)=v(1,1)/s11
        du11(2)=v(2,1)/s11
        du11(3)=0.d0
        du11(4)=0.d0
        du21(1)=0.d0
        du21(2)=0.d0
        du21(3)=du11(1)
        du21(4)=du11(2)
        do i=1,4
          du11(i)= du11(i)-u(1,1)*ds11(i)/s11
     *            +(f(1,1)*dv11(i)+f(1,2)*dv21(i))/s11
          du21(i)= du21(i)-u(2,1)*ds11(i)/s11
     *            +(f(2,1)*dv11(i)+f(2,2)*dv21(i))/s11
          du12(i)=-du21(i)
          du22(i)= du11(i)
        end do
c
        do i=1,4
          dw11(i)=alam*(ds11(i)+ds22(i))+2.d0*amu*ds11(i)
          dw22(i)=alam*(ds11(i)+ds22(i))+2.d0*amu*ds22(i)
        end do
c
        do i=1,4
          dpdf(1,i)=dw11(i)*v(1,1)*u(1,1)+dw22(i)*v(1,2)*u(1,2)
     *               +w11*dv11(i)*u(1,1)+w22*dv12(i)*u(1,2)
     *               +w11*v(1,1)*du11(i)+w22*v(1,2)*du12(i)
          dpdf(2,i)=dw11(i)*v(1,1)*u(2,1)+dw22(i)*v(1,2)*u(2,2)
     *               +w11*dv11(i)*u(2,1)+w22*dv12(i)*u(2,2)
     *               +w11*v(1,1)*du21(i)+w22*v(1,2)*du22(i)
          dpdf(3,i)=dw11(i)*v(2,1)*u(1,1)+dw22(i)*v(2,2)*u(1,2)
     *               +w11*dv21(i)*u(1,1)+w22*dv22(i)*u(1,2)
     *               +w11*v(2,1)*du11(i)+w22*v(2,2)*du12(i)
          dpdf(4,i)=dw11(i)*v(2,1)*u(2,1)+dw22(i)*v(2,2)*u(2,2)
     *               +w11*dv21(i)*u(2,1)+w22*dv22(i)*u(2,2)
     *               +w11*v(2,1)*du21(i)+w22*v(2,2)*du22(i)
        end do
c
        if( .true. )then ! *wdh* debugging
          write(*,'(" dP/dF = [",4f6.2,"]")') (dpdf(1,j),j=1,4)
          write(*,'(" dP/dF = [",4f6.2,"]")') (dpdf(2,j),j=1,4)
          write(*,'(" dP/dF = [",4f6.2,"]")') (dpdf(3,j),j=1,4)
          write(*,'(" dP/dF = [",4f6.2,"]")') (dpdf(4,j),j=1,4)
        end if


        if (.true.) then ! *WDH* 
          delta=1.d-7    ! check analytical derivative with the numerical one
          do i=1,2
          do j=1,2
            k=2*(i-1)+j
            f(i,j)=f(i,j)+delta
            call smgetp0 (f,p1,alam,amu)
            dpdf1(1,k)=(p1(1,1)-p(1,1))/delta
            dpdf1(2,k)=(p1(1,2)-p(1,2))/delta
            dpdf1(3,k)=(p1(2,1)-p(2,1))/delta
            dpdf1(4,k)=(p1(2,2)-p(2,2))/delta
            f(i,j)=f(i,j)-delta
          end do
          end do
          dmax=0.d0
          do i=1,4
          do j=1,4
            dmax=max(dabs(dpdf1(i,j)-dpdf(i,j)),dmax)
          end do
          end do
          if (dmax.gt.1.d-3) then
            write(6,*)'Error (smgetdp) : derivative mismatch'
            write(6,342)du(1,1),du(1,2),du(2,1),du(2,2)
  342       format(4(1x,1pe15.8))
            do i=1,4
            do j=1,4
              write(6,132)i,j,dpdf(i,j),dpdf1(i,j),
     &               dabs(dpdf1(i,j)-dpdf(i,j))
  132         format(2(1x,i1),2(1x,1pe15.8)," err=",1pe9.2)
            end do
            end do
            ! *wdh* pause is deprecated: pause
          end if
        end if
c
      elseif (itype.eq.4) then
c
c Neo-Hookean
c
        f(1,1)=1.d0+du(1,1)
        f(1,2)=     du(1,2)
        f(2,1)=     du(2,1)
        f(2,2)=1.d0+du(2,2)
        det=f(1,1)*f(2,2)-f(1,2)*f(2,1)                  ! Jacobian
        fact=dlog(det)                                   ! ln(J)
        fact2=(alam*fact-amu)/det
        p(1,1)= fact2*f(2,2)+amu*f(1,1)                  ! this is P(i,j) based on the current F(i,j)
        p(1,2)=-fact2*f(1,2)+amu*f(2,1)
        p(2,1)=-fact2*f(2,1)+amu*f(1,2)
        p(2,2)= fact2*f(1,1)+amu*f(2,2)
c
c        write(6,*)f
c        write(6,*)s
c        write(6,*)p
c        pause
c
        if (ideriv.eq.0) return
c
        fact2=fact2/det
        fact3=alam/det**2
c
        dpdf(1,1)=(-fact2+fact3)*f(2,2)**2+amu
        dpdf(1,2)=( fact2-fact3)*f(2,1)*f(2,2)
        dpdf(1,3)=( fact2-fact3)*f(1,2)*f(2,2)
        dpdf(1,4)=-fact2*f(1,2)*f(2,1)+fact3*f(1,1)*f(2,2)
c
        dpdf(2,1)=( fact2-fact3)*f(1,2)*f(2,2)
        dpdf(2,2)=-fact2*f(1,1)*f(2,2)+fact3*f(1,2)*f(2,1)
        dpdf(2,3)=(-fact2+fact3)*f(1,2)**2+amu
        dpdf(2,4)=( fact2-fact3)*f(1,1)*f(1,2)
c
        dpdf(3,1)=( fact2-fact3)*f(2,1)*f(2,2)
        dpdf(3,2)=(-fact2+fact3)*f(2,1)**2+amu
        dpdf(3,3)=dpdf(2,2)
        dpdf(3,4)=( fact2-fact3)*f(1,1)*f(2,1)
c
        dpdf(4,1)=dpdf(1,4)
        dpdf(4,2)=( fact2-fact3)*f(1,1)*f(2,1)
        dpdf(4,3)=( fact2-fact3)*f(1,1)*f(1,2)
        dpdf(4,4)=(-fact2+fact3)*f(1,1)**2+amu
c
      else
c
        write(6,*)'Error (smgetdp) : invalid itype'
        stop 1234
c
      end if
c
c      write(6,*)'dpdf='
c      do i=1,4
c        write(6,101)(dpdf(i,j),j=1,4)
c  101   format(4(1x,1pe10.3))
c      end do
c      pause
c
      return
      end
c
c++++++++++++++++++++
c
      subroutine smgetp0 (f,p,alam,amu)
c
      implicit real*8 (a-h,o-z)
      dimension f(2,2),v(2,2),u(2,2),p(2,2)
      data tol / 1.d-14 /
c
      detf=f(1,1)*f(2,2)-f(1,2)*f(2,1)   ! det(F)
c
      h11=f(1,1)*f(1,1)+f(2,1)*f(2,1)   ! components of H=F'*F
      h12=f(1,1)*f(1,2)+f(2,1)*f(2,2)
      h22=f(1,2)*f(1,2)+f(2,2)*f(2,2)
c
      temp=h22-h11            ! compute Givens rotation to diagonalize H
      fact=dsqrt(temp**2+4.d0*h12**2)
      if (temp.ge.0.d0) then
        t=-2.d0*h12/(temp+fact+tol)
      else
        t=-2.d0*h12/(temp-fact-tol)
      end if
      c=1/dsqrt(1+t**2)
      s=c*t
c
c
c      if (dabs(h12).lt.tol) then
c        c=1.d0
c        s=0.d0
c      else
c        temp=h22-h11            ! compute Givens rotation to diagonalize H
c        if (temp.ge.0.d0) then
c          t=-2.d0*h12/(temp+dsqrt(temp**2+4.d0*h12**2))
c        else
c          t=-2.d0*h12/(temp-dsqrt(temp**2+4.d0*h12**2))
c        end if
c        c=1/dsqrt(1+t**2)
c        s=c*t
c      end if
c
      d11=h11*c**2+2.d0*h12*c*s+h22*s**2  ! eigenvalues of H
      d22=h11*s**2-2.d0*h12*c*s+h22*c**2
      if (d11.lt.-tol) then
        write(6,*)'Error (smgetp0) : d11 negative'
        stop 111
      end if
      if (d22.lt.-tol) then
        write(6,*)'Error (smgetp0) : d22 negative'
        stop 222
      end if
c
      if (d11.ge.d22) then       ! compute S and V in SVD of F
        s11=dsqrt(max(d11,0.d0))
        s22=dsqrt(max(d22,0.d0))
        v(1,1)= c
        v(1,2)=-s
        v(2,1)= s
        v(2,2)= c
      else
        s11=dsqrt(max(d22,0.d0))
        s22=dsqrt(max(d11,0.d0))
        v(1,1)=-s
        v(1,2)=-c
        v(2,1)= c
        v(2,2)=-s
      end if
      if (detf.lt.0.d0) s22=-s22
c
      u(1,1)=(f(1,1)*v(1,1)+f(1,2)*v(2,1))/s11  ! compute U in U*S*V'=F
      u(2,1)=(f(2,1)*v(1,1)+f(2,2)*v(2,1))/s11
      u(1,2)=-u(2,1)
      u(2,2)= u(1,1)
c
      trace=s11+s22-2.d0                   ! diagonal components of stress
      w11=alam*trace+2.d0*amu*(s11-1.d0)
      w22=alam*trace+2.d0*amu*(s22-1.d0)
c
      p(1,1)=w11*v(1,1)*u(1,1)+w22*v(1,2)*u(1,2)   ! nominal stress
      p(1,2)=w11*v(1,1)*u(2,1)+w22*v(1,2)*u(2,2)
      p(2,1)=w11*v(2,1)*u(1,1)+w22*v(2,2)*u(1,2)
      p(2,2)=w11*v(2,1)*u(2,1)+w22*v(2,2)*u(2,2)
c
      return
      end
c
c++++++++++++++++++++
c
      subroutine smeig2dn (a1,a2,dpdf,al,el,er,ier)
c
      implicit real*8 (a-h,o-z)
      dimension dpdf(4,4),al(6),el(6,6),er(6,6),cm(4,2)
      common / smgdatn / amu,alam,rho0
c
      ier=0
c
c coef matrix
      do i=1,4
        cm(i,1)=a1*dpdf(i,1)+a2*dpdf(i,2)
        cm(i,2)=a1*dpdf(i,3)+a2*dpdf(i,4)
      end do
c
c betas (see notes)
      beta1=(a1*cm(1,1)+a2*cm(3,1))/rho0
      beta2=(a1*cm(1,2)+a2*cm(3,2))/rho0
      beta3=(a1*cm(2,1)+a2*cm(4,1))/rho0
      beta4=(a1*cm(2,2)+a2*cm(4,2))/rho0
c
c wave speeds
      arg=(beta1-beta4)**2+4.d0*beta2*beta3
      if (arg.lt.0.d0) then
        write(6,*)'Error (smeig2d) : complex eigenvalue'
        ier=1
        return
      end if
      fact1=.5d0*(beta1+beta4)
      fact2=.5d0*dsqrt((beta1-beta4)**2+4.d0*beta2*beta3)
      if (fact1-fact2.lt.0.d0) then
        write(6,*)'Error (smeig2d) : imaginary eigenvalue'
        write(6,'(" a1,a2=",2e10.3," fact1,fact2=",2e10.3)') 
     &     a1,a2,fact1,fact2
        write(*,'("Sub matrix of dpdf in direction(a1,a2):")')
        write(*,'(" Beta = [",2f6.2,"]")') beta1,beta2
        write(*,'("      = [",2f6.2,"]")') beta3,beta4
        ier=2
        return
      end if
      c1=dsqrt(fact1+fact2)
      c2=dsqrt(fact1-fact2)
c
c      write(6,*)beta1,beta2,beta3,beta4,c1,c2
c      pause
c
c eigenvalues
      al(1)=-c1
      al(2)=-c2
      al(3)=0.d0
      al(4)=0.d0
      al(5)= c2
      al(6)= c1
c
c right eigenvectors
      do j=1,2
        if (dabs(beta1-al(j)**2).gt.dabs(beta4-al(j)**2)) then
          er(1,j)= beta2
          er(2,j)=-beta1+al(j)**2
        else
          er(1,j)=-beta4+al(j)**2
          er(2,j)= beta3
        end if
      end do
      er(1,5)=er(1,2)
      er(2,5)=er(2,2)
      er(1,6)=er(1,1)
      er(2,6)=er(2,1)
      do i=1,4
        ip2=i+2
        er(ip2,1)= (cm(i,1)*er(1,1)+cm(i,2)*er(2,1))/c1
        er(ip2,2)= (cm(i,1)*er(1,2)+cm(i,2)*er(2,2))/c2
        er(ip2,5)=-(cm(i,1)*er(1,5)+cm(i,2)*er(2,5))/c2
        er(ip2,6)=-(cm(i,1)*er(1,6)+cm(i,2)*er(2,6))/c1
      end do
      er(1,3)= 0.d0
      er(2,3)= 0.d0
      er(3,3)= a2
      er(4,3)= 0.d0
      er(5,3)=-a1
      er(6,3)= 0.d0
      er(1,4)= 0.d0
      er(2,4)= 0.d0
      er(3,4)= 0.d0
      er(4,4)= a2
      er(5,4)= 0.d0
      er(6,4)=-a1
c
c left eigenvectors
      rdet=.5d0/(er(1,1)*er(2,2)-er(2,1)*er(1,2))
c      write(6,*)'rdet=',rdet
c      pause
      el(1,1)= rdet*er(2,2)
      el(1,2)=-rdet*er(1,2)
      el(2,1)=-rdet*er(2,1)
      el(2,2)= rdet*er(1,1)
      do i=1,2
        fact=1.d0/(rho0*al(i))
        el(i,3)=-a1*el(i,1)*fact
        el(i,4)=-a1*el(i,2)*fact
        el(i,5)=-a2*el(i,1)*fact
        el(i,6)=-a2*el(i,2)*fact
      end do
      el(5,1)= el(2,1)
      el(5,2)= el(2,2)
      el(6,1)= el(1,1)
      el(6,2)= el(1,2)
      do i=5,6
        fact=1.d0/(rho0*al(i))
        el(i,3)=-a1*el(i,1)*fact
        el(i,4)=-a1*el(i,2)*fact
        el(i,5)=-a2*el(i,1)*fact
        el(i,6)=-a2*el(i,2)*fact
      end do
      det1=er(3,6)*er(4,5)-er(4,6)*er(3,5)
      det2=er(3,6)*er(6,5)-er(6,6)*er(3,5)
      det3=er(5,6)*er(4,5)-er(4,6)*er(5,5)
      det4=er(5,6)*er(6,5)-er(6,6)*er(5,5)
      det5=er(3,6)*er(5,5)-er(5,6)*er(3,5)
      det6=er(6,6)*er(4,5)-er(4,6)*er(6,5)
      rdet=1.d0/(det1*a1**2+(det2+det3)*a1*a2+det4*a2**2)
      el(3,1)= 0.d0
      el(3,2)= 0.d0
      el(3,3)= rdet*(a1*det3+a2*det4)
      el(3,4)= rdet*(a1*det5        )
      el(3,5)=-rdet*(a1*det1+a2*det2)
      el(3,6)= rdet*(        a2*det5)
      el(4,1)= 0.d0
      el(4,2)= 0.d0
      el(4,3)= rdet*(a1*det6        )
      el(4,4)= rdet*(a1*det2+a2*det4)
      el(4,5)= rdet*(        a2*det6)
      el(4,6)=-rdet*(a1*det1+a2*det3)
c
c check
c      ier=0
c      eps=1.d-13
c      do i=1,6
c      do j=1,6
c        sum=0.d0
c        do k=1,6
c          sum=sum+el(i,k)*er(k,j)
c        end do
c        if (i.eq.j) sum=sum-1.d0
c        if (dabs(sum).gt.eps) then
c          write(6,*)i,j,sum
c          ier=1
c        end if
c      end do
c      end do
c      if (ier.ne.0) then
c        write(6,*)'an1,an2=',an1,an2
c        pause
c      end if
c
      return
      end
c
c++++++++++++++++++++
c
      subroutine smeig2dn1 (dpdf,al,el,er,ier)
c
      implicit real*8 (a-h,o-z)
      dimension dpdf(4,4),al(6),el(6,6),er(6,6),cm(4,2)
      common / smgdatn / amu,alam,rho0
c
      ier=0
c
c coef matrix
      do i=1,4
        cm(i,1)=dpdf(i,1)
        cm(i,2)=dpdf(i,3)
      end do
c
c betas (see notes)
      beta1=cm(1,1)/rho0
      beta2=cm(1,2)/rho0
      beta3=cm(2,1)/rho0
      beta4=cm(2,2)/rho0
c
c wave speeds
      arg=(beta1-beta4)**2+4.d0*beta2*beta3
      if (arg.lt.0.d0) then
        write(6,*)'Error (smeig2d) : complex eigenvalue'
        ier=1
        return
      end if
      fact1=.5d0*(beta1+beta4)
      fact2=.5d0*dsqrt((beta1-beta4)**2+4.d0*beta2*beta3)
      if (fact1-fact2.lt.0.d0) then
        write(6,*)'Error (smeig2d) : imaginary eigenvalue'
        write(*,*)'fact1,fact2=',fact1,fact2
        write(6,*)'dpdf ='
        do i=1,4
          write(6,231)i,(dpdf(i,j),j=1,4)
  231     format(1x,i1,4(1x,1pe15.8))
        end do
        ier=2
        return
      end if
      c1=dsqrt(fact1+fact2)
      c2=dsqrt(fact1-fact2)
c
c      write(6,*)beta1,beta2,beta3,beta4,c1,c2
c      pause
c
c eigenvalues
      al(1)=-c1
      al(2)=-c2
      al(3)=0.d0
      al(4)=0.d0
      al(5)= c2
      al(6)= c1
c
c right eigenvectors
      do j=1,2
        if (dabs(beta1-al(j)**2).gt.dabs(beta4-al(j)**2)) then
          er(1,j)= beta2
          er(2,j)=-beta1+al(j)**2
        else
          er(1,j)=-beta4+al(j)**2
          er(2,j)= beta3
        end if
      end do
      er(1,5)=er(1,2)
      er(2,5)=er(2,2)
      er(1,6)=er(1,1)
      er(2,6)=er(2,1)
      do i=1,4
        ip2=i+2
        er(ip2,1)= (cm(i,1)*er(1,1)+cm(i,2)*er(2,1))/c1
        er(ip2,2)= (cm(i,1)*er(1,2)+cm(i,2)*er(2,2))/c2
        er(ip2,5)=-(cm(i,1)*er(1,5)+cm(i,2)*er(2,5))/c2
        er(ip2,6)=-(cm(i,1)*er(1,6)+cm(i,2)*er(2,6))/c1
      end do
      er(1,3)= 0.d0
      er(2,3)= 0.d0
      er(3,3)= 0.d0
      er(4,3)= 0.d0
      er(5,3)=-1.d0
      er(6,3)= 0.d0
      er(1,4)= 0.d0
      er(2,4)= 0.d0
      er(3,4)= 0.d0
      er(4,4)= 0.d0
      er(5,4)= 0.d0
      er(6,4)=-1.d0
c
c left eigenvectors
      rdet=.5d0/(er(1,1)*er(2,2)-er(2,1)*er(1,2))
c      write(6,*)'rdet=',rdet
c      pause
      el(1,1)= rdet*er(2,2)
      el(1,2)=-rdet*er(1,2)
      el(2,1)=-rdet*er(2,1)
      el(2,2)= rdet*er(1,1)
      do i=1,2
        fact=1.d0/(rho0*al(i))
        el(i,3)=-el(i,1)*fact
        el(i,4)=-el(i,2)*fact
        el(i,5)= 0.d0
        el(i,6)= 0.d0
      end do
      el(5,1)= el(2,1)
      el(5,2)= el(2,2)
      el(6,1)= el(1,1)
      el(6,2)= el(1,2)
      do i=5,6
        fact=1.d0/(rho0*al(i))
        el(i,3)=-el(i,1)*fact
        el(i,4)=-el(i,2)*fact
        el(i,5)= 0.d0
        el(i,6)= 0.d0
      end do
      det1=er(3,6)*er(4,5)-er(4,6)*er(3,5)
      det2=er(3,6)*er(6,5)-er(6,6)*er(3,5)
      det3=er(5,6)*er(4,5)-er(4,6)*er(5,5)
      det5=er(3,6)*er(5,5)-er(5,6)*er(3,5)
      det6=er(6,6)*er(4,5)-er(4,6)*er(6,5)
      rdet=1.d0/det1
      el(3,1)= 0.d0
      el(3,2)= 0.d0
      el(3,3)= rdet*det3
      el(3,4)= rdet*det5
      el(3,5)=-1.d0
      el(3,6)= 0.d0
      el(4,1)= 0.d0
      el(4,2)= 0.d0
      el(4,3)= rdet*det6
      el(4,4)= rdet*det2
      el(4,5)= 0.d0
      el(4,6)=-1.d0
c
c check
c      ier=0
c      eps=1.d-13
c      do i=1,6
c      do j=1,6
c        sum=0.d0
c        do k=1,6
c          sum=sum+el(i,k)*er(k,j)
c        end do
c        if (i.eq.j) sum=sum-1.d0
c        if (dabs(sum).gt.eps) then
c          write(6,*)i,j,sum
c          ier=1
c        end if
c      end do
c      end do
c      if (ier.ne.0) pause
c
      return
      end
c
c++++++++++++++++++++
c
      subroutine smeig2dn2 (dpdf,al,el,er,ier)
c
      implicit real*8 (a-h,o-z)
      dimension dpdf(4,4),al(6),el(6,6),er(6,6),cm(4,2)
      common / smgdatn / amu,alam,rho0
c
      ier=0
c
c coef matrix
      do i=1,4
        cm(i,1)=dpdf(i,2)
        cm(i,2)=dpdf(i,4)
      end do
c
c betas (see notes)
      beta1=cm(3,1)/rho0
      beta2=cm(3,2)/rho0
      beta3=cm(4,1)/rho0
      beta4=cm(4,2)/rho0
c
c wave speeds
      arg=(beta1-beta4)**2+4.d0*beta2*beta3
      if (arg.lt.0.d0) then
        write(6,*)'Error (smeig2d2) : complex eigenvalue'
        ier=1
        return
      end if
      fact1=.5d0*(beta1+beta4)
      fact2=.5d0*dsqrt((beta1-beta4)**2+4.d0*beta2*beta3)
      if (fact1-fact2.lt.0.d0) then
        write(6,*)'Error (smeig2d2) : imaginary eigenvalue'
        write(6,*)'dpdf ='
        do i=1,4
          write(6,231)i,(dpdf(i,j),j=1,4)
  231     format(1x,i1,4(1x,1pe15.8))
        end do
        ier=2
        return
      end if
      c1=dsqrt(fact1+fact2)
      c2=dsqrt(fact1-fact2)
c
c      write(6,*)beta1,beta2,beta3,beta4,c1,c2
c      pause
c
c eigenvalues
      al(1)=-c1
      al(2)=-c2
      al(3)=0.d0
      al(4)=0.d0
      al(5)= c2
      al(6)= c1
c
c right eigenvectors
      do j=1,2
        if (dabs(beta1-al(j)**2).gt.dabs(beta4-al(j)**2)) then
          er(1,j)= beta2
          er(2,j)=-beta1+al(j)**2
        else
          er(1,j)=-beta4+al(j)**2
          er(2,j)= beta3
        end if
      end do
      er(1,5)=er(1,2)
      er(2,5)=er(2,2)
      er(1,6)=er(1,1)
      er(2,6)=er(2,1)
      do i=1,4
        ip2=i+2
        er(ip2,1)= (cm(i,1)*er(1,1)+cm(i,2)*er(2,1))/c1
        er(ip2,2)= (cm(i,1)*er(1,2)+cm(i,2)*er(2,2))/c2
        er(ip2,5)=-(cm(i,1)*er(1,5)+cm(i,2)*er(2,5))/c2
        er(ip2,6)=-(cm(i,1)*er(1,6)+cm(i,2)*er(2,6))/c1
      end do
      er(1,3)= 0.d0
      er(2,3)= 0.d0
      er(3,3)= 1.d0
      er(4,3)= 0.d0
      er(5,3)= 0.d0
      er(6,3)= 0.d0
      er(1,4)= 0.d0
      er(2,4)= 0.d0
      er(3,4)= 0.d0
      er(4,4)= 1.d0
      er(5,4)= 0.d0
      er(6,4)= 0.d0
c
c left eigenvectors
      rdet=.5d0/(er(1,1)*er(2,2)-er(2,1)*er(1,2))
c      write(6,*)'rdet=',rdet
c      pause
      el(1,1)= rdet*er(2,2)
      el(1,2)=-rdet*er(1,2)
      el(2,1)=-rdet*er(2,1)
      el(2,2)= rdet*er(1,1)
      do i=1,2
        fact=1.d0/(rho0*al(i))
        el(i,3)= 0.d0
        el(i,4)= 0.d0
        el(i,5)=-el(i,1)*fact
        el(i,6)=-el(i,2)*fact
      end do
      el(5,1)= el(2,1)
      el(5,2)= el(2,2)
      el(6,1)= el(1,1)
      el(6,2)= el(1,2)
      do i=5,6
        fact=1.d0/(rho0*al(i))
        el(i,3)= 0.d0
        el(i,4)= 0.d0
        el(i,5)=-el(i,1)*fact
        el(i,6)=-el(i,2)*fact
      end do
      det2=er(3,6)*er(6,5)-er(6,6)*er(3,5)
      det3=er(5,6)*er(4,5)-er(4,6)*er(5,5)
      det4=er(5,6)*er(6,5)-er(6,6)*er(5,5)
      det5=er(3,6)*er(5,5)-er(5,6)*er(3,5)
      det6=er(6,6)*er(4,5)-er(4,6)*er(6,5)
      rdet=1.d0/det4
      el(3,1)= 0.d0
      el(3,2)= 0.d0
      el(3,3)= 1.d0
      el(3,4)= 0.d0
      el(3,5)=-rdet*det2
      el(3,6)= rdet*det5
      el(4,1)= 0.d0
      el(4,2)= 0.d0
      el(4,3)= 0.d0
      el(4,4)= 1.d0
      el(4,5)= rdet*det6
      el(4,6)=-rdet*det3
c
c check
c      ier=0
c      eps=1.d-13
c      do i=1,6
c      do j=1,6
c        sum=0.d0
c        do k=1,6
c          sum=sum+el(i,k)*er(k,j)
c        end do
c        if (i.eq.j) sum=sum-1.d0
c        if (dabs(sum).gt.eps) then
c          write(6,*)i,j,sum
c          ier=1
c        end if
c      end do
c      end do
c      if (ier.ne.0) pause
c
      return
      end
c
c++++++++++++++++++++
c
      subroutine smflux2dn (m,a1,a2,wl,wr,ajl,ajr,cml,cmr,
     *                      fxl,fxr,speed)
c
c Godunov flux (method=0)
c
      implicit real*8 (a-h,o-z)
      dimension wl(m),wr(m),cml(4,2),cmr(4,2),fxl(6),fxr(6)
      dimension cm(4,2),el(2,6),er(6,2),al(2),alpha(2),v(2)
      common / smgdatn / amu,alam,rho0
      common / smgprmn / method,iorder,ilimit,iupwind,icart,itype,ifrc
c
c      write(6,*)m,a1,a2,ajl,ajr,cml,cmr
c      pause
c
c average jacobian
      aj=.5d0*(ajl+ajr)
c
c average coef matrix
      do i=1,4
        do j=1,2
          cm(i,j)=.5d0*(cml(i,j)+cmr(i,j))
        end do
      end do
c
c betas (see notes)
      beta1=(a1*cm(1,1)+a2*cm(3,1))/rho0
      beta2=(a1*cm(1,2)+a2*cm(3,2))/rho0
      beta3=(a1*cm(2,1)+a2*cm(4,1))/rho0
      beta4=(a1*cm(2,2)+a2*cm(4,2))/rho0
c
c wave speeds
      arg=(beta1-beta4)**2+4.d0*beta2*beta3
      if (arg.lt.0.d0) then
        write(6,*)'Error (smflux2d) : complex eigenvalue'
        stop 60
      end if
      fact1=.5d0*(beta1+beta4)
      fact2=.5d0*dsqrt((beta1-beta4)**2+4.d0*beta2*beta3)
      if (fact1-fact2.lt.0.d0) then
        write(6,*)'Error (smflux2d) : imaginary eigenvalue'
        stop 61
      end if
      c1=dsqrt(fact1+fact2)
      c2=dsqrt(fact1-fact2)
c
c      write(6,*)beta1,beta2,beta3,beta4,c1,c2
c      pause
c
c first two eigenvalues
      al(1)=-c1
      al(2)=-c2
c
c first two right eigenvectors
      do j=1,2
        if (dabs(beta1-al(j)**2).gt.dabs(beta4-al(j)**2)) then
          er(1,j)= beta2
          er(2,j)=-beta1+al(j)**2
        else
          er(1,j)=-beta4+al(j)**2
          er(2,j)= beta3
        end if
      end do
      do i=1,4
        ip2=i+2
        er(ip2,1)=(cm(i,1)*er(1,1)+cm(i,2)*er(2,1))/c1
        er(ip2,2)=(cm(i,1)*er(1,2)+cm(i,2)*er(2,2))/c2
      end do
c
c first two left eigenvectors
      rdet=.5d0/(er(1,1)*er(2,2)-er(2,1)*er(1,2))
c      write(6,*)'rdet=',rdet
c      pause
      el(1,1)= rdet*er(2,2)
      el(1,2)=-rdet*er(1,2)
      el(2,1)=-rdet*er(2,1)
      el(2,2)= rdet*er(1,1)
      do i=1,2
        fact=1.d0/(rho0*al(i))
        el(i,3)=-a1*el(i,1)*fact
        el(i,4)=-a1*el(i,2)*fact
        el(i,5)=-a2*el(i,1)*fact
        el(i,6)=-a2*el(i,2)*fact
      end do
c
c wave strengths (from the left state)
      do i=1,2
        alpha(i)=0.d0
        do j=1,6
          alpha(i)=alpha(i)+el(i,j)*(wr(j)-wl(j))
        end do
      end do
c
c flux (on the left)
      fxl(1)=aj*(-a1*wl(3)-a2*wl(5))/rho0
      fxl(2)=aj*(-a1*wl(4)-a2*wl(6))/rho0
c
c velocity (on the left)
      v(1)=wl(1)
      v(2)=wl(2)
c
c Godunov flux and middle velocity state (computed from the left)
      do j=1,2
        alp=aj*alpha(j)*al(j)
        do i=1,2
          fxl(i)=fxl(i)+alp*er(i,j)
          v(i)=v(i)+alpha(j)*er(i,j)
        end do
      end do
c
c fixup Godunov flux
      do i=1,2
        fxr(i)=fxl(i)/ajr
        fxl(i)=fxl(i)/ajl
      end do
c
c non-conservative flux for stress
      do i=1,4
        ip2=i+2
        fxl(ip2)=-cml(i,1)*v(1)-cml(i,2)*v(2)
        fxr(ip2)=-cmr(i,1)*v(1)-cmr(i,2)*v(2)
      end do
c
c fastest speed
      speed=max(dabs(al(1)),speed)
c
      return
      end
c
c++++++++++++++++++++
c
      subroutine smflux2dn1 (a1l,a2l,dpdfl,wl,a1r,a2r,dpdfr,wr,
     *                       fxl,fxr,speed)
c
c SVK flux for method=1, similar approach to that of linear elasticity
c with variable coefficients
c
      implicit real*8 (a-h,o-z)
      dimension dpdfl(4,4),wl(6),dpdfr(4,4),wr(6),fxl(6),fxr(6)
      dimension al(6),el(6,6),er(6,6,2),c(4,4),d(4),w0(6)
c
c average metrics
      a1=.5d0*(a1l+a1r)
      a2=.5d0*(a2l+a2r)
c
c eigen-structure on the left
      call smeig2dn (a1,a2,dpdfl,al,el,er(1,1,1),ier)
      if (ier.ne.0) then
        write(6,*)'Error (smflux2dn1) : left state error'
        stop 70
      end if
c
c largest eigenvalue (left)
      speed=max(speed,al(6))
c
c contribution to coefficient matrix from left
      do j=1,2
        c(1,j)=er(1,j,1)
        c(2,j)=er(2,j,1)
        c(3,j)=a1*er(3,j,1)+a2*er(5,j,1)
        c(4,j)=a1*er(4,j,1)+a2*er(6,j,1)
      end do
c
c eigen-structure on the right
      call smeig2dn (a1,a2,dpdfr,al,el,er(1,1,2),ier)
      if (ier.ne.0) then
        write(6,*)'Error (smflux2dn1) : right state error'
        stop 71
      end if
c
c largest eigenvalue (right)
      speed=max(speed,al(6))
c
c contribution to coefficient matrix from right
      do j=3,4
        jp2=j+2
        c(1,j)=er(1,jp2,2)
        c(2,j)=er(2,jp2,2)
        c(3,j)=a1*er(3,jp2,2)+a2*er(5,jp2,2)
        c(4,j)=a1*er(4,jp2,2)+a2*er(6,jp2,2)
      end do
c
c right-hand side vector
      d(1)=wr(1)-wl(1)
      d(2)=wr(2)-wl(2)
      d(3)=a1*(wr(3)-wl(3))+a2*(wr(5)-wl(5))
      d(4)=a1*(wr(4)-wl(4))+a2*(wr(6)-wl(6))
c
c solve the system to compute wave strengths
      call smsolve (c,d,ier)
      if (ier.ne.0) then
        write(6,*)'Error (smflux2dn1) : system may be singular'
        stop 72
      end if
c
c interface state (from the left)
      do i=1,6
        w0(i)=wl(i)+d(1)*er(i,1,1)+d(2)*er(i,2,1)
      end do
c
c get left "flux"
      call smflxn (a1l,a2l,dpdfl,w0,fxl)
c
c interface state (from the right)
      do i=1,6
        w0(i)=wr(i)-d(3)*er(i,5,2)-d(4)*er(i,6,2)
      end do
c
c get right "flux"
      call smflxn (a1r,a2r,dpdfr,w0,fxr)
c
      return
      end
c
c++++++++++++++++++++
c
      subroutine smsolve (dg,g,ier)
c
c 4x4 system solver from cmpduv.f
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
c          write(6,*)'Error (smsolve) : system may be singular'
c          stop 80
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
c++++++++++++++++++++
c
      subroutine smflxn (a1,a2,dpdf,w,fx)
c
      implicit real*8 (a-h,o-z)
      dimension dpdf(4,4),w(6),fx(6),cm(4,2)
      common / smgdatn / amu,alam,rho0
c
      do i=1,4
        cm(i,1)=a1*dpdf(i,1)+a2*dpdf(i,2)
        cm(i,2)=a1*dpdf(i,3)+a2*dpdf(i,4)
      end do
c
      fx(1)=-(a1*w(3)+a2*w(5))/rho0
      fx(2)=-(a1*w(4)+a2*w(6))/rho0
      do i=1,4
        fx(i+2)=-(cm(i,1)*w(1)+cm(i,2)*w(2))
      end do
c
      return
      end
c
c++++++++++++++++++++
c
      subroutine gethtzn (m,x,y,t,htz)
c
c compute tz forcing function
c
      implicit real*8 (a-h,o-z)
      dimension htz(m),ut(8),ux(8),uy(8),u0(2),
     *          du(2,2),p(2,2),dpdf(4,4),cpar(10)
      common / smgprmn / method,iorder,ilimit,iupwind,icart,itype,ifrc
      common / smgdatn / amu,alam,rho0
      common / tzflow / eptz,itz
c
      do i=1,8
        call ogDeriv (eptz,1,0,0,0,x,y,0.d0,t,i-1,ut(i))
      end do
      do i=1,8
        call ogDeriv (eptz,0,1,0,0,x,y,0.d0,t,i-1,ux(i))
        call ogDeriv (eptz,0,0,1,0,x,y,0.d0,t,i-1,uy(i))
      end do
      do i=1,2
        call ogDeriv (eptz,0,0,0,0,x,y,0.d0,t,i-1,u0(i))
      end do
c
      du(1,1)=ux(7)
      du(1,2)=uy(7)
      du(2,1)=ux(8)
      du(2,2)=uy(8)
c
      cpar(1)=alam
      cpar(2)=amu
      ideriv=1
      call smgetdp (du,p,dpdf,cpar,ideriv,itype)
c
      htz(1)=ut(1)-(ux(3)+uy(5))/rho0
      htz(2)=ut(2)-(ux(4)+uy(6))/rho0
      do i=1,4
        ip2=i+2
        htz(ip2)=ut(ip2)-dpdf(i,1)*ux(1)-dpdf(i,3)*ux(2)
     *                  -dpdf(i,2)*uy(1)-dpdf(i,4)*uy(2)
      end do
      htz(7)=ut(7)-u0(1)
      htz(8)=ut(8)-u0(2)
c
c sanity check
c      write(6,101)(htz(i),i=3,8)
c  101 format('htz=',6(1x,1pe9.2))
c      pause
c      err=0.d0
c      tol=1.d-13
c      do i=3,8
c        err=max(dabs(htz(i)),err)
c      end do
c      if (err.gt.tol) then
c        write(6,*)'Warning (gethtzn) : sanity check failed'
c      end if
c
      return
      end
c
c+++++++++++++++++++++++
c
      subroutine smgbcsn (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                    ds1,ds2,t,xy,u,ibc)
c
c variables: u(j1,j2,1:2)=velocity
c            u(j1,j2,3:6)=stress (P11,P12,P21,P22)
c            u(j1,j2,7:8)=displacement
c
      implicit real*8 (a-h,o-z)
      dimension xy(nd1a:nd1b,nd2a:nd2b,2),u(nd1a:nd1b,nd2a:nd2b,m)
      common / smgdatn / amu,alam,rho0
      common / tzflow / eptz,itz
c
c      write(6,*)'smgbcs, t=',t
c
c      write(6,*)itz,eptz,amu,alam,rho0
c      pause
c
c..fill in exact values on the boundary
      if (.true.) then
c
        do i=1,m
        do j1=n1a,n1b
          call ogDeriv (eptz,0,0,0,0,xy(j1,n2a,1),xy(j1,n2a,2),
     *                  0.d0,t,i-1,u(j1,n2a,i))
          call ogDeriv (eptz,0,0,0,0,xy(j1,n2b,1),xy(j1,n2b,2),
     *                  0.d0,t,i-1,u(j1,n2b,i))
        end do
        end do
        do i=1,m
        do j2=n2a,n2b
          call ogDeriv (eptz,0,0,0,0,xy(n1a,j2,1),xy(n1a,j2,2),
     *                  0.d0,t,i-1,u(n1a,j2,i))
          call ogDeriv (eptz,0,0,0,0,xy(n1b,j2,1),xy(n1b,j2,2),
     *                  0.d0,t,i-1,u(n1b,j2,i))
        end do
        end do
c
      end if
c
c..fill in exact values in the first ghost line (without corners)
      if (.true.) then
c
        do i=1,m
        do j1=n1a,n1b
          call ogDeriv (eptz,0,0,0,0,xy(j1,n2a-1,1),xy(j1,n2a-1,2),
     *                  0.d0,t,i-1,u(j1,n2a-1,i))
          call ogDeriv (eptz,0,0,0,0,xy(j1,n2b+1,1),xy(j1,n2b+1,2),
     *                  0.d0,t,i-1,u(j1,n2b+1,i))
        end do
        end do
        do i=1,m
        do j2=n2a,n2b
          call ogDeriv (eptz,0,0,0,0,xy(n1a-1,j2,1),xy(n1a-1,j2,2),
     *                  0.d0,t,i-1,u(n1a-1,j2,i))
          call ogDeriv (eptz,0,0,0,0,xy(n1b+1,j2,1),xy(n1b+1,j2,2),
     *                  0.d0,t,i-1,u(n1b+1,j2,i))
        end do
        end do
c
      end if
c
c..fill in exact values in the corners
      if (.true.) then
c
        do i=1,m
          call ogDeriv (eptz,0,0,0,0,xy(n1a-1,n2a-1,1),
     *                               xy(n1a-1,n2a-1,2),0.d0,t,
     *                            i-1,u(n1a-1,n2a-1,i))
          call ogDeriv (eptz,0,0,0,0,xy(n1a-1,n2b+1,1),
     *                               xy(n1a-1,n2b+1,2),0.d0,t,
     *                            i-1,u(n1a-1,n2b+1,i))
          call ogDeriv (eptz,0,0,0,0,xy(n1b+1,n2a-1,1),
     *                               xy(n1b+1,n2a-1,2),0.d0,t,
     *                            i-1,u(n1b+1,n2a-1,i))
          call ogDeriv (eptz,0,0,0,0,xy(n1b+1,n2b+1,1),
     *                               xy(n1b+1,n2b+1,2),0.d0,t,
     *                            i-1,u(n1b+1,n2b+1,i))
        end do
c
      end if
c
c..fill in exact values in the second ghost line
      if (.true.) then
c
        do i=1,m
        do j1=n1a-2,n1b+2
          call ogDeriv (eptz,0,0,0,0,xy(j1,n2a-2,1),xy(j1,n2a-2,2),
     *                  0.d0,t,i-1,u(j1,n2a-2,i))
          call ogDeriv (eptz,0,0,0,0,xy(j1,n2b+2,1),xy(j1,n2b+2,2),
     *                  0.d0,t,i-1,u(j1,n2b+2,i))
        end do
        do j2=n2a-2,n2b+2
          call ogDeriv (eptz,0,0,0,0,xy(n1a-2,j2,1),xy(n1a-2,j2,2),
     *                  0.d0,t,i-1,u(n1a-2,j2,i))
          call ogDeriv (eptz,0,0,0,0,xy(n1b+2,j2,1),xy(n1b+2,j2,2),
     *                  0.d0,t,i-1,u(n1b+2,j2,i))
        end do
        end do
c
      end if
c
c      write(6,*)'smgbcs (done) ...'
c
      return
      end
c
c+++++++++++++++++++++++
c
      subroutine cornern (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                    ds1,ds2,t,xy,mask,rx,u,icart)
c
c variables: u(j1,j2,1:2)=velocity
c            u(j1,j2,3:6)=stress (S11,S12,S21,S22)
c            u(j1,j2,7:8)=displacement
c
      implicit real*8 (a-h,o-z)
      dimension xy(nd1a:nd1b,nd2a:nd2b,2),mask(nd1a:nd1b,nd2a:nd2b),
     *          rx(nd1a:nd1b,nd2a:nd2b,2,2),u(nd1a:nd1b,nd2a:nd2b,m)
      dimension ue(8)
      common / smgdatn / amu,alam,rho0
      common / tzflow / eptz,itz
c
      return
      end
c
c+++++++++++++++++++++++
c
      subroutine cstressn (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                     ds1,ds2,t,xy,mask,rx,u,icart)
c
c variables: u(j1,j2,1:2)=velocity
c            u(j1,j2,3:6)=stress (S11,S12,S21,S22)
c            u(j1,j2,7:8)=displacement
c
      implicit real*8 (a-h,o-z)
      dimension xy(nd1a:nd1b,nd2a:nd2b,2),mask(nd1a:nd1b,nd2a:nd2b),
     *          rx(nd1a:nd1b,nd2a:nd2b,2,2),u(nd1a:nd1b,nd2a:nd2b,m)
      dimension ue(8),eee(8)
      common / smgdatn / amu,alam,rho0
      common / tzflow / eptz,itz
c
      return
      end
c
c+++++++++++++++++++++++
c
      subroutine smgerrn (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                    ds1,ds2,t,xy,u)
c
c variables: u(j1,j2,1:2)=velocity
c            u(j1,j2,3:6)=stress (S11,S12,S21,S22)
c            u(j1,j2,7:8)=displacement
c
      implicit real*8 (a-h,o-z)
      dimension xy(nd1a:nd1b,nd2a:nd2b,2),u(nd1a:nd1b,nd2a:nd2b,m)
      dimension err(8)
      common / smgdatn / amu,alam,rho0
      common / tzflow / eptz,itz
c
      return
      end
c
c+++++++++++++++++++++++
c
      subroutine stressDiss2 (m,nd1a,nd1b,nd2a,nd2b,n1a,n1b,n2a,n2b,
     *            ds1,ds2,tsdiss,tsdissdt,rx,u,up,mask,diseig,diseigdt)
c
c second-order dissipation on the components of stress belonging to surfaces whose
c normals are tangent to cell faces.  The eigenvalues associated with these components
c are zero and thus the Godunov methods provides no dissipation on its own.
c
c used when iorder=1
c
      implicit real*8 (a-h,o-z)
      dimension rx(nd1a:nd1b,nd2a:nd2b,2,2),u(nd1a:nd1b,nd2a:nd2b,m),
     *          up(nd1a:nd1b,nd2a:nd2b,m),mask(nd1a:nd1b,nd2a:nd2b)
      dimension du2(6)
      common / smgprmn / method,iorder,ilimit,iupwind,icart,itype,ifrc
      common / smgdatn / amu,alam,rho0
      common / tzflow / eptz,itz
c
c wave speed (to provide a scale)
      cp=dsqrt((alam+2.d0*amu)/rho0)
c
c dissipation coeff (scaled by the max wave speed)
      akap=tsdiss*cp
c
      if (icart.eq.1) then
c
c Cartesian case
c
        diseig=akap*max(1.d0/ds1,1.d0/ds2)   ! real part of timestepping eig
        diseigdt=0.   ! for 1/dt dissipation
c
        do j2=n2a,n2b
        do j1=n1a,n1b
          if (mask(j1,j2).ne.0) then
            du2(3)=akap*(u(j1,j2+1,3)-2*u(j1,j2,3)+u(j1,j2-1,3))
     *                  /(4.d0*ds2)
            du2(4)=akap*(u(j1,j2+1,4)-2*u(j1,j2,4)+u(j1,j2-1,4))
     *                  /(4.d0*ds2)
            du2(5)=akap*(u(j1+1,j2,5)-2*u(j1,j2,5)+u(j1-1,j2,5))
     *                  /(4.d0*ds1)
            du2(6)=akap*(u(j1+1,j2,6)-2*u(j1,j2,6)+u(j1-1,j2,6))
     *                  /(4.d0*ds1)
            do i=3,6
              up(j1,j2,i)=up(j1,j2,i)+du2(i)
            end do
          end if
        end do
        end do
c
      else
c
c Curvilinear case
c
        diseig=0.d0   ! real part of timestepping eig (initialize to zero)
        diseigdt=0.   ! for 1/dt dissipation
c
        do j2=n2a,n2b
        do j1=n1a,n1b
          if (mask(j1,j2).ne.0) then
            rad=dsqrt(rx(j1,j2,1,1)**2+rx(j1,j2,1,2)**2)
            fact=akap*rad
            diseig=max(fact/ds1,diseig)
            a1=rx(j1,j2,1,1)/rad
            a2=rx(j1,j2,1,2)/rad
            do i=3,6
              du2(i)=fact*(u(j1+1,j2,i)-2*u(j1,j2,i)+u(j1-1,j2,i))
     *                     /(4.d0*ds1)
            end do
            up(j1,j2,3)=up(j1,j2,3)-a2*(-a2*du2(3)+a1*du2(5))
            up(j1,j2,5)=up(j1,j2,5)+a1*(-a2*du2(3)+a1*du2(5))
            up(j1,j2,4)=up(j1,j2,4)-a2*(-a2*du2(4)+a1*du2(6))
            up(j1,j2,6)=up(j1,j2,6)+a1*(-a2*du2(4)+a1*du2(6))
c original formulas below are equivalent to the simpler ones above
c            alpha=a1*u(j1,j2,3)+a2*u(j1,j2,5)
c            beta= a1*(u(j1,j2,5)+du2(5))-a2*(u(j1,j2,3)+du2(3))
c            up(j1,j2,3)=up(j1,j2,3)+(alpha*a1-beta*a2-u(j1,j2,3))
c            up(j1,j2,5)=up(j1,j2,5)+(a1*beta+a2*alpha-u(j1,j2,5))
c            alpha=a1*u(j1,j2,4)+a2*u(j1,j2,6)
c            beta= a1*(u(j1,j2,6)+du2(6))-a2*(u(j1,j2,4)+du2(4))
c            up(j1,j2,4)=up(j1,j2,4)+(alpha*a1-beta*a2-u(j1,j2,4))
c            up(j1,j2,6)=up(j1,j2,6)+(a1*beta+a2*alpha-u(j1,j2,6))
            rad=dsqrt(rx(j1,j2,2,1)**2+rx(j1,j2,2,2)**2)
            fact=akap*rad
            diseig=max(fact/ds2,diseig)
            a1=rx(j1,j2,2,1)/rad
            a2=rx(j1,j2,2,2)/rad
            do i=3,6
              du2(i)=fact*(u(j1,j2+1,i)-2*u(j1,j2,i)+u(j1,j2-1,i))
     *                     /(4.d0*ds2)
            end do
            up(j1,j2,3)=up(j1,j2,3)-a2*(-a2*du2(3)+a1*du2(5))
            up(j1,j2,5)=up(j1,j2,5)+a1*(-a2*du2(3)+a1*du2(5))
            up(j1,j2,4)=up(j1,j2,4)-a2*(-a2*du2(4)+a1*du2(6))
            up(j1,j2,6)=up(j1,j2,6)+a1*(-a2*du2(4)+a1*du2(6))
c original formulas below are equivalent to the simpler ones above
c            alpha=a1*u(j1,j2,3)+a2*u(j1,j2,5)
c            beta= a1*(u(j1,j2,5)+du2(5))-a2*(u(j1,j2,3)+du2(3))
c            up(j1,j2,3)=up(j1,j2,3)+(alpha*a1-beta*a2-u(j1,j2,3))
c            up(j1,j2,5)=up(j1,j2,5)+(a1*beta+a2*alpha-u(j1,j2,5))
c            alpha=a1*u(j1,j2,4)+a2*u(j1,j2,6)
c            beta= a1*(u(j1,j2,6)+du2(6))-a2*(u(j1,j2,4)+du2(4))
c            up(j1,j2,4)=up(j1,j2,4)+(alpha*a1-beta*a2-u(j1,j2,4))
c            up(j1,j2,6)=up(j1,j2,6)+(a1*beta+a2*alpha-u(j1,j2,6))
          end if
        end do
        end do
c
      end if
c
      return
      end
c
c+++++++++++++++++++++++
c
      subroutine stressDiss4 (m,nd1a,nd1b,nd2a,nd2b,n1a,n1b,n2a,n2b,
     *        dt,ds1,ds2,tsdiss,tsdissdt,rx,u,up,mask,diseig,diseigdt)
c
c fourth-order dissipation on the components of stress belonging to surfaces whose
c normals are tangent to cell faces.  The eigenvalues associated with these components
c are zero and thus the Godunov methods provides no dissipation on its own.
c
c used when iorder=2
c
      implicit real*8 (a-h,o-z)
      dimension rx(nd1a:nd1b,nd2a:nd2b,2,2),u(nd1a:nd1b,nd2a:nd2b,m),
     *          up(nd1a:nd1b,nd2a:nd2b,m),mask(nd1a:nd1b,nd2a:nd2b)
      dimension du4(6)
      common / smgprmn / method,iorder,ilimit,iupwind,icart,itype,ifrc
      common / smgdatn / amu,alam,rho0
      common / tzflow / eptz,itz
c
c dissipation coeff (change 2 to 3 for 3D)

      akap=(tsdiss+tsdissdt/dt)/(2*16.)
      diseig=0.            ! real part of timestepping eig
      diseigdt=tsdiss      ! coeff of 1/dt timestepping eig
c
      if (icart.eq.1) then
c
c Cartesian case
c
        do j2=n2a,n2b
        do j1=n1a,n1b
          if (mask(j1,j2).ne.0) then
            du4(3)=akap*(-u(j1,j2+2,3)+4*u(j1,j2+1,3)
     *                   -u(j1,j2-2,3)+4*u(j1,j2-1,3)-6*u(j1,j2,3))
            du4(4)=akap*(-u(j1,j2+2,4)+4*u(j1,j2+1,4)
     *                   -u(j1,j2-2,4)+4*u(j1,j2-1,4)-6*u(j1,j2,4))
            du4(5)=akap*(-u(j1+2,j2,5)+4*u(j1+1,j2,5)
     *                   -u(j1-2,j2,5)+4*u(j1-1,j2,5)-6*u(j1,j2,5))
            du4(6)=akap*(-u(j1+2,j2,6)+4*u(j1+1,j2,6)
     *                   -u(j1-2,j2,6)+4*u(j1-1,j2,6)-6*u(j1,j2,6))
            do i=3,6
              up(j1,j2,i)=up(j1,j2,i)+du4(i)
            end do
          end if
        end do
        end do
c
      else 
c
c Curvilinear case
c
        do j2=n2a,n2b
        do j1=n1a,n1b
          if (mask(j1,j2).ne.0) then
c
c r1 direction
            rad=dsqrt(rx(j1,j2,1,1)**2+rx(j1,j2,1,2)**2)
            a1=rx(j1,j2,1,1)/rad
            a2=rx(j1,j2,1,2)/rad
            do i=3,6
              du4(i)=akap*(-u(j1+2,j2,i)+4*u(j1+1,j2,i)
     *                     -u(j1-2,j2,i)+4*u(j1-1,j2,i)-6*u(j1,j2,i))
            end do
            up(j1,j2,3)=up(j1,j2,3)-a2*(-a2*du4(3)+a1*du4(5))
            up(j1,j2,5)=up(j1,j2,5)+a1*(-a2*du4(3)+a1*du4(5))
            up(j1,j2,4)=up(j1,j2,4)-a2*(-a2*du4(4)+a1*du4(6))
            up(j1,j2,6)=up(j1,j2,6)+a1*(-a2*du4(4)+a1*du4(6))
c
c r2 direction
            rad=dsqrt(rx(j1,j2,2,1)**2+rx(j1,j2,2,2)**2)
            a1=rx(j1,j2,2,1)/rad
            a2=rx(j1,j2,2,2)/rad
            do i=3,6
              du4(i)=akap*(-u(j1,j2+2,i)+4*u(j1,j2+1,i)
     *                     -u(j1,j2-2,i)+4*u(j1,j2-1,i)-6*u(j1,j2,i))
            end do
            up(j1,j2,3)=up(j1,j2,3)-a2*(-a2*du4(3)+a1*du4(5))
            up(j1,j2,5)=up(j1,j2,5)+a1*(-a2*du4(3)+a1*du4(5))
            up(j1,j2,4)=up(j1,j2,4)-a2*(-a2*du4(4)+a1*du4(6))
            up(j1,j2,6)=up(j1,j2,6)+a1*(-a2*du4(4)+a1*du4(6))
          end if
        end do
        end do
c
      end if
c
      return
      end
c
c+++++++++++++++++++++++
c
      subroutine stressDiss4_0 (m,nd1a,nd1b,nd2a,nd2b,n1a,n1b,n2a,n2b,
     *         ds1,ds2,tsdiss,tsdissdt,rx,u,up,mask,diseig,diseigdt)

c  *** ORIGINAL VERSION ***

c
c fourth-order dissipation on the components of stress belonging to surfaces whose
c normals are tangent to cell faces.  The eigenvalues associated with these components
c are zero and thus the Godunov methods provides no dissipation on its own.
c
c used when iorder=2
c
      implicit real*8 (a-h,o-z)
      dimension rx(nd1a:nd1b,nd2a:nd2b,2,2),u(nd1a:nd1b,nd2a:nd2b,m),
     *          up(nd1a:nd1b,nd2a:nd2b,m),mask(nd1a:nd1b,nd2a:nd2b)
      dimension du4(6)
      common / smgprmn / method,iorder,ilimit,iupwind,icart,itype,ifrc
      common / smgdatn / amu,alam,rho0
      common / tzflow / eptz,itz
c
c wave speed (to provide a scale)
      cp=dsqrt((alam+2.d0*amu)/rho0)
c
c dissipation coeff (scaled by the max wave speed)
      akap=tsdiss*cp
c
      if (icart.eq.1) then
c
c Cartesian case
c
        diseig=akap*max(1.d0/ds1,1.d0/ds2)   ! real part of timestepping eig
        diseigdt=0.                          ! for 1/dt dissipation
c
        do j2=n2a,n2b
        do j1=n1a,n1b
          if (mask(j1,j2).ne.0) then
            du4(3)=akap*(-u(j1,j2+2,3)+4*u(j1,j2+1,3)-6*u(j1,j2,3)
     *                    +4*u(j1,j2-1,3)-u(j1,j2-2,3))/(16.d0*ds2)
            du4(4)=akap*(-u(j1,j2+2,4)+4*u(j1,j2+1,4)-6*u(j1,j2,4)
     *                    +4*u(j1,j2-1,4)-u(j1,j2-2,4))/(16.d0*ds2)
            du4(5)=akap*(-u(j1+2,j2,5)+4*u(j1+1,j2,5)-6*u(j1,j2,5)
     *                    +4*u(j1-1,j2,5)-u(j1-2,j2,5))/(16.d0*ds1)
            du4(6)=akap*(-u(j1+2,j2,6)+4*u(j1+1,j2,6)-6*u(j1,j2,6)
     *                    +4*u(j1-1,j2,6)-u(j1-2,j2,6))/(16.d0*ds1)
            do i=3,6
              up(j1,j2,i)=up(j1,j2,i)+du4(i)
            end do
          end if
        end do
        end do
c
      else
c
c Curvilinear case
c
        diseig=0.d0   ! real part of timestepping eig (initialize to zero)
        diseigdt=0.   ! for 1/dt dissipation
c
        do j2=n2a,n2b
        do j1=n1a,n1b
          if (mask(j1,j2).ne.0) then
            rad=dsqrt(rx(j1,j2,1,1)**2+rx(j1,j2,1,2)**2)
            fact=akap*rad
            diseig=max(fact/ds1,diseig)
            a1=rx(j1,j2,1,1)/rad
            a2=rx(j1,j2,1,2)/rad
            do i=3,6
              du4(i)=fact*(-u(j1+2,j2,i)+4*u(j1+1,j2,i)-6*u(j1,j2,i)
     *                      +4*u(j1-1,j2,i)-u(j1-2,j2,i))/(16.d0*ds1)
            end do
            up(j1,j2,3)=up(j1,j2,3)-a2*(-a2*du4(3)+a1*du4(5))
            up(j1,j2,5)=up(j1,j2,5)+a1*(-a2*du4(3)+a1*du4(5))
            up(j1,j2,4)=up(j1,j2,4)-a2*(-a2*du4(4)+a1*du4(6))
            up(j1,j2,6)=up(j1,j2,6)+a1*(-a2*du4(4)+a1*du4(6))
c original formulas below are equivalent to the simpler ones above
c            alpha=a1*u(j1,j2,3)+a2*u(j1,j2,5)
c            beta= a1*(u(j1,j2,5)+du4(5))-a2*(u(j1,j2,3)+du4(3))
c            up(j1,j2,3)=up(j1,j2,3)+(alpha*a1-beta*a2-u(j1,j2,3))
c            up(j1,j2,5)=up(j1,j2,5)+(a1*beta+a2*alpha-u(j1,j2,5))
c            alpha=a1*u(j1,j2,4)+a2*u(j1,j2,6)
c            beta= a1*(u(j1,j2,6)+du4(6))-a2*(u(j1,j2,4)+du4(4))
c            up(j1,j2,4)=up(j1,j2,4)+(alpha*a1-beta*a2-u(j1,j2,4))
c            up(j1,j2,6)=up(j1,j2,6)+(a1*beta+a2*alpha-u(j1,j2,6))
            rad=dsqrt(rx(j1,j2,2,1)**2+rx(j1,j2,2,2)**2)
            fact=akap*rad
            diseig=max(fact/ds2,diseig)
            a1=rx(j1,j2,2,1)/rad
            a2=rx(j1,j2,2,2)/rad
            do i=3,6
              du4(i)=fact*(-u(j1,j2+2,i)+4*u(j1,j2+1,i)-6*u(j1,j2,i)
     *                      +4*u(j1,j2-1,i)-u(j1,j2-2,i))/(16.d0*ds2)
            end do
            up(j1,j2,3)=up(j1,j2,3)-a2*(-a2*du4(3)+a1*du4(5))
            up(j1,j2,5)=up(j1,j2,5)+a1*(-a2*du4(3)+a1*du4(5))
            up(j1,j2,4)=up(j1,j2,4)-a2*(-a2*du4(4)+a1*du4(6))
            up(j1,j2,6)=up(j1,j2,6)+a1*(-a2*du4(4)+a1*du4(6))
c original formulas below are equivalent to the simpler ones above
c            alpha=a1*u(j1,j2,3)+a2*u(j1,j2,5)
c            beta= a1*(u(j1,j2,5)+du4(5))-a2*(u(j1,j2,3)+du4(3))
c            up(j1,j2,3)=up(j1,j2,3)+(alpha*a1-beta*a2-u(j1,j2,3))
c            up(j1,j2,5)=up(j1,j2,5)+(a1*beta+a2*alpha-u(j1,j2,5))
c            alpha=a1*u(j1,j2,4)+a2*u(j1,j2,6)
c            beta= a1*(u(j1,j2,6)+du4(6))-a2*(u(j1,j2,4)+du4(4))
c            up(j1,j2,4)=up(j1,j2,4)+(alpha*a1-beta*a2-u(j1,j2,4))
c            up(j1,j2,6)=up(j1,j2,6)+(a1*beta+a2*alpha-u(j1,j2,6))
          end if
        end do
        end do
c
      end if
c
      return
      end
c
c+++++++++++++++++++++++
c
      subroutine stressRelax2dn (m,nd1a,nd1b,nd2a,nd2b,n1a,n1b,n2a,n2b,
     *                            ds1,ds2,dt,t,xy,rx,u,up,mask)
c
      implicit real*8 (a-h,o-z)
      dimension xy(nd1a:nd1b,nd2a:nd2b,2),rx(nd1a:nd1b,nd2a:nd2b,2,2),
     *          u(nd1a:nd1b,nd2a:nd2b,m),up(nd1a:nd1b,nd2a:nd2b,m),
     *          mask(nd1a:nd1b,nd2a:nd2b)
      dimension du(2,2),p(2,2),cpar(10)
      common / smgprmn / method,iorder,ilimit,iupwind,icart,itype,ifrc
      common / smrlxn / relaxAlpha,relaxDelta,irelax
      common / smgdatn / amu,alam,rho0
      common / tzflow / eptz,itz
c
c      write(6,*)'irelax,icart =',irelax,icart
c      write(6,*)relaxAlpha,relaxDelta
c      pause
c
c      irelax=2
c      relaxAlpha=1.d0
c      relaxDelta=0.d0
      beta = relaxAlpha+relaxDelta/dt
c
c      e11max=0.d0
c      e12max=0.d0
c      e21max=0.d0
c      e22max=0.d0
c
      cpar(1)=alam
      cpar(2)=amu
c
      if( icart.eq.1 ) then
c
c Cartesian case
        do j2 = n2a,n2b
        do j1 = n1a,n1b
          if( mask(j1,j2).ne.0 ) then
            if( irelax.eq.2 ) then
              u1x = (u(j1+1,j2,7)-u(j1-1,j2,7))/(2.d0*ds1)
              u2x = (u(j1+1,j2,8)-u(j1-1,j2,8))/(2.d0*ds1)
              u1y = (u(j1,j2+1,7)-u(j1,j2-1,7))/(2.d0*ds2)
              u2y = (u(j1,j2+1,8)-u(j1,j2-1,8))/(2.d0*ds2)
            else
              u1x = (-u(j1+2,j2,7)+8*u(j1+1,j2,7)-
     *           8*u(j1-1,j2,7)+u(j1-2,j2,7))/(12.d0*ds1)
              u2x = (-u(j1+2,j2,8)+8*u(j1+1,j2,8)-
     *           8*u(j1-1,j2,8)+u(j1-2,j2,8))/(12.d0*ds1)
              u1y = (-u(j1,j2+2,7)+8*u(j1,j2+1,7)-
     *           8*u(j1,j2-1,7)+u(j1,j2-2,7))/(12.d0*ds2)
              u2y = (-u(j1,j2+2,8)+8*u(j1,j2+1,8)-
     *           8*u(j1,j2-1,8)+u(j1,j2-2,8))/(12.d0*ds2)
            end if
c
            du(1,1)=u1x
            du(1,2)=u1y
            du(2,1)=u2x
            du(2,2)=u2y
c
            ideriv=0
            call smgetdp (du,p,dpdf,cpar,ideriv,itype)
c
            up(j1,j2,3) = up(j1,j2,3)+beta*(-u(j1,j2,3)+p(1,1))
            up(j1,j2,4) = up(j1,j2,4)+beta*(-u(j1,j2,4)+p(1,2))
            up(j1,j2,5) = up(j1,j2,5)+beta*(-u(j1,j2,5)+p(2,1))
            up(j1,j2,6) = up(j1,j2,6)+beta*(-u(j1,j2,6)+p(2,2))
c
c            e11max=max(dabs(p(1,1)-u(j1,j2,3)),e11max)
c            e12max=max(dabs(p(1,2)-u(j1,j2,4)),e12max)
c            e21max=max(dabs(p(2,1)-u(j1,j2,5)),e21max)
c            e22max=max(dabs(p(2,2)-u(j1,j2,6)),e22max)
c
c            if (t.gt.1.d-10) then
c              write(50,500)p(1,1),u(j1,j2,3),p(1,2),u(j1,j2,4),
c     *                     p(2,1),u(j1,j2,5),p(2,2),u(j1,j2,6)
c  500         format(8(1x,1pe15.8))
c            end if
c
          end if
        end do
        end do
c
      else
c
c curvilinear case
        do j2 = n2a,n2b
        do j1 = n1a,n1b
          if( mask(j1,j2).ne.0 ) then
            if( irelax.eq.2 ) then
              u1r = (u(j1+1,j2,7)-u(j1-1,j2,7))/(2.d0*ds1)
              u2r = (u(j1+1,j2,8)-u(j1-1,j2,8))/(2.d0*ds1)
              u1s = (u(j1,j2+1,7)-u(j1,j2-1,7))/(2.d0*ds2)
              u2s = (u(j1,j2+1,8)-u(j1,j2-1,8))/(2.d0*ds2)
            else
              u1r = (-u(j1+2,j2,7)+8*u(j1+1,j2,7)-
     *           8*u(j1-1,j2,7)+u(j1-2,j2,7))/(12.d0*ds1)
              u2r = (-u(j1+2,j2,8)+8*u(j1+1,j2,8)-
     *           8*u(j1-1,j2,8)+u(j1-2,j2,8))/(12.d0*ds1)
              u1s = (-u(j1,j2+2,7)+8*u(j1,j2+1,7)-
     *           8*u(j1,j2-1,7)+u(j1,j2-2,7))/(12.d0*ds2)
              u2s = (-u(j1,j2+2,8)+8*u(j1,j2+1,8)-
     *           8*u(j1,j2-1,8)+u(j1,j2-2,8))/(12.d0*ds2)
            end if
c
            du(1,1) = u1r*rx(j1,j2,1,1)+u1s*rx(j1,j2,2,1)
            du(1,2) = u1r*rx(j1,j2,1,2)+u1s*rx(j1,j2,2,2)
            du(2,1) = u2r*rx(j1,j2,1,1)+u2s*rx(j1,j2,2,1)
            du(2,2) = u2r*rx(j1,j2,1,2)+u2s*rx(j1,j2,2,2)
c
            ideriv=0
            call smgetdp (du,p,dpdf,cpar,ideriv,itype)
c
            up(j1,j2,3) = up(j1,j2,3)+beta*(-u(j1,j2,3)+p(1,1))
            up(j1,j2,4) = up(j1,j2,4)+beta*(-u(j1,j2,4)+p(1,2))
            up(j1,j2,5) = up(j1,j2,5)+beta*(-u(j1,j2,5)+p(2,1))
            up(j1,j2,6) = up(j1,j2,6)+beta*(-u(j1,j2,6)+p(2,2))
c
c            e11max=max(dabs(p(1,1)-u(j1,j2,3)),e11max)
c            e12max=max(dabs(p(1,2)-u(j1,j2,4)),e12max)
c            e21max=max(dabs(p(2,1)-u(j1,j2,5)),e21max)
c            e22max=max(dabs(p(2,2)-u(j1,j2,6)),e22max)
c
c            if (t.gt.1.d-10) then
c              write(50,500)p(1,1),u(j1,j2,3),p(1,2),u(j1,j2,4),
c     *                     p(2,1),u(j1,j2,5),p(2,2),u(j1,j2,6)
c  500         format(8(1x,1pe15.8))
c            end if
c
          end if
        end do
        end do
      end if
c
c      write(6,123)t,e11max,e12max,e21max,e22max
c  123 format('Errors at t =',1pe15.8,/,4(1x,1pe10.3))
cc      pause
c      if (t.gt.1.d-10) then
c        write(6,*)n1a,n1b,n2a,n2b
c        stop
c      end if
c
c
c add twilight zone contribution, if necessary
      if( itz.ne.0 ) then
        iu1c = 6
        iu2c = 7
        is11c = 2
        is12c = 3
        is21c = 4
        is22c = 5
        do j2 = n2a,n2b
        do j1 = n1a,n1b
          if( mask(j1,j2).ne.0 ) then
            x = xy(j1,j2,1)
            y = xy(j1,j2,2)
            call ogDeriv (eptz,0,1,0,0,x,y,0.d0,t,iu1c,u1xt)
            call ogDeriv (eptz,0,0,1,0,x,y,0.d0,t,iu1c,u1yt)
            call ogDeriv (eptz,0,1,0,0,x,y,0.d0,t,iu2c,u2xt)
            call ogDeriv (eptz,0,0,1,0,x,y,0.d0,t,iu2c,u2yt)
            call ogDeriv (eptz,0,0,0,0,x,y,0.d0,t,is11c,s11t)
            call ogDeriv (eptz,0,0,0,0,x,y,0.d0,t,is12c,s12t)
            call ogDeriv (eptz,0,0,0,0,x,y,0.d0,t,is21c,s21t)
            call ogDeriv (eptz,0,0,0,0,x,y,0.d0,t,is22c,s22t)
c
            du(1,1)=u1xt
            du(1,2)=u1yt
            du(2,1)=u2xt
            du(2,2)=u2yt
c
            ideriv=0
            call smgetdp (du,p,dpdf,cpar,ideriv,itype)
c
            up(j1,j2,3) = up(j1,j2,3)-beta*(-s11t+p(1,1))
            up(j1,j2,4) = up(j1,j2,4)-beta*(-s12t+p(1,2))
            up(j1,j2,5) = up(j1,j2,5)-beta*(-s21t+p(2,1))
            up(j1,j2,6) = up(j1,j2,6)-beta*(-s22t+p(2,2))
          end if
        end do
        end do
      end if
c
      return
      end
c
c+++++++++++++++++++++++
c
      subroutine bcchk (m,nd1a,nd1b,nd2a,nd2b,n1a,n1b,n2a,n2b,
     *                  ds1,ds2,t,rx,u)
c
      implicit real*8 (a-h,o-z)
      dimension rx(nd1a:nd1b,nd2a:nd2b,2,2),u(nd1a:nd1b,nd2a:nd2b,m)
      dimension trac(2),f(2,2),p(2,2),s(2,2),fi(2,2),du(2,2),
     *          p1(2,2),dpdf(4,4),pdot(2,2),bcs(8),cpar(10)
      common / smgprmn / method,iorder,ilimit,iupwind,icart,itype,ifrc
      common / smgdatn / amu,alam,rho0
c
c      write(6,*)n1a,n1b,n2a,n2b
c      pause
c
      cpar(1)=alam
      cpar(2)=amu
c
      write(6,100)t
  100 format('** checking bcs at t=',1pe12.5,' **')
c
      do iside=1,2
        j2=n2a+(n2b-n2a)*(iside-1)
        sym=0.d0
        trac(1)=0.d0
        trac(2)=0.d0
        do ii=1,8
          bcs(ii)=0.d0
        end do
        do j1=n1a,n1b
          u1r=(u(j1+1,j2,7)-u(j1-1,j2,7))/(2.d0*ds1)
          u1s=(u(j1,j2+1,7)-u(j1,j2-1,7))/(2.d0*ds2)
          u2r=(u(j1+1,j2,8)-u(j1-1,j2,8))/(2.d0*ds1)
          u2s=(u(j1,j2+1,8)-u(j1,j2-1,8))/(2.d0*ds2)
          u1x=u1r*rx(j1,j2,1,1)+u1s*rx(j1,j2,2,1)
          u1y=u1r*rx(j1,j2,1,2)+u1s*rx(j1,j2,2,2)
          u2x=u2r*rx(j1,j2,1,1)+u2s*rx(j1,j2,2,1)
          u2y=u2r*rx(j1,j2,1,2)+u2s*rx(j1,j2,2,2)
c
          v1r=(u(j1+1,j2,1)-u(j1-1,j2,1))/(2.d0*ds1)
          v1s=(u(j1,j2+1,1)-u(j1,j2-1,1))/(2.d0*ds2)
          v2r=(u(j1+1,j2,2)-u(j1-1,j2,2))/(2.d0*ds1)
          v2s=(u(j1,j2+1,2)-u(j1,j2-1,2))/(2.d0*ds2)
          v1x=v1r*rx(j1,j2,1,1)+v1s*rx(j1,j2,2,1)
          v1y=v1r*rx(j1,j2,1,2)+v1s*rx(j1,j2,2,2)
          v2x=v2r*rx(j1,j2,1,1)+v2s*rx(j1,j2,2,1)
          v2y=v2r*rx(j1,j2,1,2)+v2s*rx(j1,j2,2,2)
c
c          if (j1.eq.n1a) then
c            write(6,555)v1r,v1s,v2r,v2s
c  555       format(4(1x,1pe15.8))
c          end if
c
          du(1,1)=u1x
          du(1,2)=u1y
          du(2,1)=u2x
          du(2,2)=u2y
c
          ideriv=1
          call smgetdp (du,p1,dpdf,cpar,ideriv,itype)
c
c          if (j1.eq.n1a) then
c            write(6,444)u1r,u1s,u2r,u2s,p1(1,1),p1(1,2),p1(2,1),p1(2,2)
c  444       format(2(4(1x,1pe15.8)))
c          end if
c
          pdot(1,1)=  dpdf(1,1)*v1x+dpdf(1,2)*v1y
     *               +dpdf(1,3)*v2x+dpdf(1,4)*v2y
          pdot(1,2)=  dpdf(2,1)*v1x+dpdf(2,2)*v1y
     *               +dpdf(2,3)*v2x+dpdf(2,4)*v2y
          pdot(2,1)=  dpdf(3,1)*v1x+dpdf(3,2)*v1y
     *               +dpdf(3,3)*v2x+dpdf(3,4)*v2y
          pdot(2,2)=  dpdf(4,1)*v1x+dpdf(4,2)*v1y
     *               +dpdf(4,3)*v2x+dpdf(4,4)*v2y
c
          f(1,1)=1.d0+u1x
          f(1,2)=     u1y
          f(2,1)=     u2x
          f(2,2)=1.d0+u2y
          p(1,1)=u(j1,j2,3)
          p(1,2)=u(j1,j2,4)
          p(2,1)=u(j1,j2,5)
          p(2,2)=u(j1,j2,6)
          do i1=1,2
            do i2=1,2
              s(i1,i2)=f(i1,1)*p(1,i2)+f(i1,2)*p(2,i2)
            end do
          end do
          fi(1,1)= f(2,2)
          fi(1,2)=-f(1,2)
          fi(2,1)=-f(2,1)
          fi(2,2)= f(1,1)
          an1=rx(j1,j2,2,1)*fi(1,1)+rx(j1,j2,2,2)*fi(2,1)
          an2=rx(j1,j2,2,1)*fi(1,2)+rx(j1,j2,2,2)*fi(2,2)
          t1=an1*s(1,1)+an2*s(2,1)
          t2=an1*s(1,2)+an2*s(2,2)
          t1=rx(j1,j2,2,1)*p(1,1)+rx(j1,j2,2,2)*p(2,1)
          t2=rx(j1,j2,2,1)*p(1,2)+rx(j1,j2,2,2)*p(2,2)
          t1=rx(j1,j2,2,1)*p1(1,1)+rx(j1,j2,2,2)*p1(2,1)
          t2=rx(j1,j2,2,1)*p1(1,2)+rx(j1,j2,2,2)*p1(2,2)
c          sym=max(dabs(s(1,2)-s(2,1)),sym)
c          trac(1)=max(dabs(t1),trac(1))
c          trac(2)=max(dabs(t2),trac(2))
          bcs(1)=max(dabs(t1),bcs(1))
          bcs(2)=max(dabs(t2),bcs(2))
          tdot1=rx(j1,j2,2,1)*pdot(1,1)+rx(j1,j2,2,2)*pdot(2,1)
          tdot2=rx(j1,j2,2,1)*pdot(1,2)+rx(j1,j2,2,2)*pdot(2,2)
c          write(6,*)j1,tdot1,tdot2
c          if (iside.eq.1.and.j1.eq.23) then
c            write(6,*)'bcchk',(dpdf(4,mc),mc=1,4)
c            write(6,*)'bcchk',v1x,v1y,v2x,v2y
c            write(6,*)'bcchk',pdot(1,1),pdot(1,2),pdot(2,1),pdot(2,2)
c          end if
          bcs(3)=max(dabs(tdot1),bcs(3))
          bcs(4)=max(dabs(tdot2),bcs(4))
          bcs(5)=max(dabs(p1(1,1)-u(j1,j2,3)),bcs(5))
          bcs(6)=max(dabs(p1(1,2)-u(j1,j2,4)),bcs(6))
          bcs(7)=max(dabs(p1(2,1)-u(j1,j2,5)),bcs(7))
          bcs(8)=max(dabs(p1(2,2)-u(j1,j2,6)),bcs(8))
        end do
c        write(6,200)iside,sym,trac(1),trac(2)
c  200   format(1x,i2,3(1x,1pe10.3))
        write(6,200)iside,(bcs(ii),ii=1,8)
  200   format(1x,i2,8(1x,1pe9.2))
      end do
c
c      if (t.gt.1.d-14) pause
c
      return
      end
c
c+++++++++++++++++++++++
c
      subroutine prtxy (m,nd1a,nd1b,nd2a,nd2b,n1a,n1b,n2a,n2b,
     *                  t,xy,u)
c
      implicit real*8 (a-h,o-z)
      dimension xy(nd1a:nd1b,nd2a:nd2b,2),u(nd1a:nd1b,nd2a:nd2b,m)
      dimension j1(3),j2(3),a(3),b(3)
      common / smgprmn / method,iorder,ilimit,iupwind,icart,itype,ifrc
      common / smgdatn / amu,alam,rho0
c
c      i1=(n1b-n1a)/4
c      i2=(n2b-n2a)/4
c      do k=1,3
c        j1(k)=n1a+k*i1
c        j2(k)=n2a+k*i2
c      end do
      j1(1)=n1a
      j1(2)=(n1a+n1b)/2
      j1(3)=n1b
      j2(1)=n2a
      j2(2)=(n2a+n2b)/2
      j2(3)=n2b
c
      do k=1,3
        a(k)=xy(j1(k),j2(2),1)+u(j1(k),j2(2),7)
        b(k)=xy(j1(k),j2(2),2)+u(j1(k),j2(2),8)
      end do
      write(61,100)t,(a(k),b(k),k=1,3)
  100 format(7(1x,1pe22.15))
c
      do k=1,3
        a(k)=xy(j1(2),j2(k),1)+u(j1(2),j2(k),7)
        b(k)=xy(j1(2),j2(k),2)+u(j1(2),j2(k),8)
      end do
      write(62,100)t,(a(k),b(k),k=1,3)
c
      return
      end
c
c+++++++++++++++++++++++
c
      subroutine prtu (m,nd1a,nd1b,nd2a,nd2b,n1a,n1b,n2a,n2b,
     *                 dt,xy,u,up)
c
      implicit real*8 (a-h,o-z)
      dimension xy(nd1a:nd1b,nd2a:nd2b,2),u(nd1a:nd1b,nd2a:nd2b,m),
     *          up(nd1a:nd1b,nd2a:nd2b,m),uout(8)
c
      write(6,*)'** printing solution data on fort.80 **'
c
      n1=n1b-n1a+1
      n2=n2b-n2a+1
      write(80,100)n1,n2
  100 format(2(1x,i4))
c
      do j1=n1a,n1b
      do j2=n2a,n2b
        do i=1,8
          uout(i)=u(j1,j2,i)+dt*up(j1,j2,i)
        end do
        write(80,200)xy(j1,j2,1),xy(j1,j2,2),(uout(k),k=1,8)
  200   format(10(1x,1pe22.15))
      end do
      end do
c
      return
      end
c
c+++++++++++++++++++++++
c
      subroutine prtu1 (m,nd1a,nd1b,nd2a,nd2b,n1a,n1b,n2a,n2b,
     *                  ds1,ds2,dt,xy,u,up,mask)
c
      implicit real*8 (a-h,o-z)
      dimension xy(nd1a:nd1b,nd2a:nd2b,2),u(nd1a:nd1b,nd2a:nd2b,m),
     *          up(nd1a:nd1b,nd2a:nd2b,m),mask(nd1a:nd1b,nd2a:nd2b),
     *          uout(8)
      common / smgprmn / method,iorder,ilimit,iupwind,icart,itype,ifrc
c
      pi=4.d0*datan(1.d0)
c
      if (icart.eq.1) then
        write(6,*)'** printing solution data to solid1.dat **'
        open(80,file='solid1.dat')
        j1a=n1b/2+1
        j1b=n1b
        j1s=1
        j2a=n2b/2
        j2b=j2a
        j2s=1
      else
        write(6,*)'** printing solution data to solid2.dat **'
        open(80,file='solid2.dat')
        j1a=n1a
        j1b=j1a
        j1s=1
        j2a=n2b
        j2b=n2a
        j2s=-1
      end if
c
      do j1=j1a,j1b,j1s
      do j2=j2a,j2b,j2s
        if (mask(j1,j2).ne.0) then
          x=xy(j1,j2,1)
          y=xy(j1,j2,2)
          do i=1,8
            uout(i)=u(j1,j2,i)+dt*up(j1,j2,i)
          end do
          a=x+uout(7)
          b=y+uout(8)
          r=dsqrt(a**2+b**2)
          if (b-a.gt.0.d0) then
            if (b+a.gt.0.d0) then
              phi=.5d0*pi-dasin(a/r)
            else
              phi=pi-dasin(b/r)
            end if
          else
            if (b+a.gt.0.d0) then
              phi=dasin(b/r)
            else
              phi=1.5*pi+dasin(a/r)
            end if
          end if
          rdot=(a*uout(1)+b*uout(2))/r
          p11bar=uout(3)*dcos(phi)+uout(4)*dsin(phi)
          fact=dsqrt(x**2+y**2)/r
          stress=fact*p11bar
          write(80,200)r,rdot,stress
  200     format(3(1x,1pe22.15))
        end if
      end do
      end do
c
      close(80)
c
      return
      end
c
c+++++++++++++++++++++++
c
      subroutine prtui (m,nd1a,nd1b,nd2a,nd2b,n1a,n1b,n2a,n2b,
     *                  t,dt,xy,u,up,mask)
c
      implicit real*8 (a-h,o-z)
      dimension xy(nd1a:nd1b,nd2a:nd2b,2),u(nd1a:nd1b,nd2a:nd2b,m),
     *          up(nd1a:nd1b,nd2a:nd2b,m),mask(nd1a:nd1b,nd2a:nd2b),
     *          uout(8)
      common / smgprmn / method,iorder,ilimit,iupwind,icart,itype,ifrc
c
      pi=4.d0*datan(1.d0)
c
      if (icart.eq.0) then
        j1=n1a
        j2=n2a
        x=xy(j1,j2,1)
        y=xy(j1,j2,2)
        do i=1,8
          uout(i)=u(j1,j2,i)+dt*up(j1,j2,i)
        end do
        a=x+uout(7)
        b=y+uout(8)
        r=dsqrt(a**2+b**2)
        if (b-a.gt.0.d0) then
          if (b+a.gt.0.d0) then
            phi=.5d0*pi-dasin(a/r)
          else
            phi=pi-dasin(b/r)
          end if
        else
          if (b+a.gt.0.d0) then
            phi=dasin(b/r)
          else
            phi=1.5*pi+dasin(a/r)
          end if
        end if
        p11bar=uout(3)*dcos(phi)+uout(4)*dsin(phi)
        r0=dsqrt(x**2+y**2)
        fact=r0/r
        stress=fact*p11bar
        time=t+dt
        write(90,200)time,r-r0,stress
  200   format(3(1x,1pe22.15))
      end if
c
      return
      end
c
c+++++++++++++++++++++++
c
      subroutine prtu2 (m,nd1a,nd1b,nd2a,nd2b,n1a,n1b,n2a,n2b,
     *                  ds1,ds2,dt,t,xy,u,up,mask)
c
      implicit real*8 (a-h,o-z)
      dimension xy(nd1a:nd1b,nd2a:nd2b,2),u(nd1a:nd1b,nd2a:nd2b,m),
     *          up(nd1a:nd1b,nd2a:nd2b,m),mask(nd1a:nd1b,nd2a:nd2b),
     *          uout(8)
      common / smgprmn / method,iorder,ilimit,iupwind,icart,itype,ifrc
c
      pi=4.d0*datan(1.d0)
c
      if (icart.eq.1) then
        iunit=60
        j1a=n1b/2+1
        j1b=n1b
        j1s=1
        j2a=n2b/2
        j2b=j2a
        j2s=1
      else
        iunit=61
        j1a=n1a
        j1b=j1a
        j1s=1
        j2a=n2b
        j2b=n2a
        j2s=-1
      end if
c
      vmin=1.d5
      vmax=-vmin
      do j1=j1a,j1b,j1s
      do j2=j2a,j2b,j2s
        if (mask(j1,j2).ne.0) then
          x=xy(j1,j2,1)
          y=xy(j1,j2,2)
          do i=1,8
            uout(i)=u(j1,j2,i)+dt*up(j1,j2,i)
          end do
          a=x+uout(7)
          b=y+uout(8)
          r=dsqrt(a**2+b**2)
          if (b-a.gt.0.d0) then
            if (b+a.gt.0.d0) then
              phi=.5d0*pi-dasin(a/r)
            else
              phi=pi-dasin(b/r)
            end if
          else
            if (b+a.gt.0.d0) then
              phi=dasin(b/r)
            else
              phi=1.5*pi+dasin(a/r)
            end if
          end if
          rdot=(a*uout(1)+b*uout(2))/r
          vmin=min(rdot,vmin)
          vmax=max(rdot,vmax)
        end if
      end do
      end do
c
      if (dabs(vmin).lt.1.d-6) vmin=0.d0
      if (dabs(vmax).lt.1.d-6) vmax=0.d0
      write(iunit,100)t,vmin,vmax
  100 format(3(1x,1pe15.8))
c
      return
      end
c
c+++++++++++++++++++++++
c
      subroutine smgbcst (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                    ds1,ds2,rx,u)
c
c**** override bcSmOptFOS for zero-traction bcs on an annulus (testing) ****
c
c variables: u(j1,j2,1:2)=velocity
c            u(j1,j2,3:6)=stress (P11,P12,P21,P22)
c            u(j1,j2,7:8)=displacement
c
      implicit real*8 (a-h,o-z)
      dimension rx(nd1a:nd1b,nd2a:nd2b,2,2),u(nd1a:nd1b,nd2a:nd2b,m)
      dimension du(2,2),p(2,2),dpdf(4,4),f(2),a(2,2),pdot(2,2),cpar(10)
      common / smgdatn / amu,alam,rho0
      common / smgprmn / method,iorder,ilimit,iupwind,icart,itype,ifrc
      data itmax, toler / 6, 1.d-13 /
c
      cpar(1)=alam
      cpar(2)=amu
c
c set zero-traction condition on boundaries (axis=2, s=constant)
      do iside=0,1
        j2=n2a+(n2b-n2a)*iside
        do j1=n1a,n1b
          ! (an1,an2) = unit normal
          aNormi=1.d0/dsqrt(rx(j1,j2,2,1)**2+rx(j1,j2,2,2)**2)
          an1=rx(j1,j2,2,1)*aNormi
          an2=rx(j1,j2,2,2)*aNormi
          b1=-(an1*u(j1,j2,3)+an2*u(j1,j2,5))
          b2=-(an1*u(j1,j2,4)+an2*u(j1,j2,6))
          u(j1,j2,3)=u(j1,j2,3)+an1*b1
          u(j1,j2,4)=u(j1,j2,4)+an1*b2
          u(j1,j2,5)=u(j1,j2,5)+an2*b1
          u(j1,j2,6)=u(j1,j2,6)+an2*b2
        end do
      end do
c
c set periodic bcs
      do j2=n2a,n2b
        do i=1,m
          u(n1a-1,j2,i)=u(n1b-1,j2,i)
          u(n1a-2,j2,i)=u(n1b-2,j2,i)
          u(n1b+1,j2,i)=u(n1a+1,j2,i)
          u(n1b+2,j2,i)=u(n1a+2,j2,i)
        end do
      end do
c
c Neumann condition for displacement and velocity, j2=n2a
      j2=n2a
      do j1=n1a,n1b
        ! (an1,an2) = unit normal
        aNormi=1.d0/dsqrt(rx(j1,j2,2,1)**2+rx(j1,j2,2,2)**2)
        an1=rx(j1,j2,2,1)*aNormi
        an2=rx(j1,j2,2,2)*aNormi
c
        u1r=(u(j1+1,j2,7)-u(j1-1,j2,7))/(2.d0*ds1)
        u2r=(u(j1+1,j2,8)-u(j1-1,j2,8))/(2.d0*ds1)
        u1s=(-3.d0*u(j1,j2,7)+4.d0*u(j1,j2+1,7)-u(j1,j2+2,7))
     *       /(2.d0*ds2)
        u2s=(-3.d0*u(j1,j2,8)+4.d0*u(j1,j2+1,8)-u(j1,j2+2,8))
     *       /(2.d0*ds2)
c
        do it=1,itmax
c
          u1x=u1r*rx(j1,j2,1,1)+u1s*rx(j1,j2,2,1)
          u1y=u1r*rx(j1,j2,1,2)+u1s*rx(j1,j2,2,2)
          u2x=u2r*rx(j1,j2,1,1)+u2s*rx(j1,j2,2,1)
          u2y=u2r*rx(j1,j2,1,2)+u2s*rx(j1,j2,2,2)
c
          du(1,1)=u1x
          du(1,2)=u1y
          du(2,1)=u2x
          du(2,2)=u2y
c
          ideriv=1
          call smgetdp (du,p,dpdf,cpar,ideriv,itype)
c
          f(1)=an1*p(1,1)+an2*p(2,1)
          f(2)=an1*p(1,2)+an2*p(2,2)
          fmax=max(dabs(f(1)),dabs(f(2)))

          a(1,1)=an1*(dpdf(1,1)*rx(j1,j2,2,1)+dpdf(1,2)*rx(j1,j2,2,2))
     *          +an2*(dpdf(3,1)*rx(j1,j2,2,1)+dpdf(3,2)*rx(j1,j2,2,2))
          a(1,2)=an1*(dpdf(1,3)*rx(j1,j2,2,1)+dpdf(1,4)*rx(j1,j2,2,2))
     *          +an2*(dpdf(3,3)*rx(j1,j2,2,1)+dpdf(3,4)*rx(j1,j2,2,2))
          a(2,1)=an1*(dpdf(2,1)*rx(j1,j2,2,1)+dpdf(2,2)*rx(j1,j2,2,2))
     *          +an2*(dpdf(4,1)*rx(j1,j2,2,1)+dpdf(4,2)*rx(j1,j2,2,2))
          a(2,2)=an1*(dpdf(2,3)*rx(j1,j2,2,1)+dpdf(2,4)*rx(j1,j2,2,2))
     *          +an2*(dpdf(4,3)*rx(j1,j2,2,1)+dpdf(4,4)*rx(j1,j2,2,2))
c
          det=a(1,1)*a(2,2)-a(1,2)*a(2,1)
          d1=(f(1)*a(2,2)-f(2)*a(1,2))/det
          d2=(f(2)*a(1,1)-f(1)*a(2,1))/det
c
c          if (j1.eq.n1a) then
c            write(6,222)it,fmax,d1,d2
c  222       format(1x,i2,3(1x,1pe10.3))
c          end if
c
          if (fmax.gt.toler) then
            u1s=u1s-d1
            u2s=u2s-d2
          else
            u(j1,j2-1,7)=u(j1,j2+1,7)-2.d0*ds2*u1s
            u(j1,j2-1,8)=u(j1,j2+1,8)-2.d0*ds2*u2s
c
c            if (j1.eq.n1a) then
c              write(6,444)u1r,u1s,u2r,u2s,p(1,1),p(1,2),p(2,1),p(2,2)
c  444         format(2(4(1x,1pe15.8)))
c            end if
c
            v1r=(u(j1+1,j2,1)-u(j1-1,j2,1))/(2.d0*ds1)
            v2r=(u(j1+1,j2,2)-u(j1-1,j2,2))/(2.d0*ds1)
            f(1)=-an1*(
     *           (dpdf(1,1)*rx(j1,j2,1,1)+dpdf(1,2)*rx(j1,j2,1,2))*v1r
     *          +(dpdf(1,3)*rx(j1,j2,1,1)+dpdf(1,4)*rx(j1,j2,1,2))*v2r)
     *           -an2*(
     *           (dpdf(3,1)*rx(j1,j2,1,1)+dpdf(3,2)*rx(j1,j2,1,2))*v1r
     *          +(dpdf(3,3)*rx(j1,j2,1,1)+dpdf(3,4)*rx(j1,j2,1,2))*v2r)
            f(2)=-an1*(
     *           (dpdf(2,1)*rx(j1,j2,1,1)+dpdf(2,2)*rx(j1,j2,1,2))*v1r
     *          +(dpdf(2,3)*rx(j1,j2,1,1)+dpdf(2,4)*rx(j1,j2,1,2))*v2r)
     *           -an2*(
     *           (dpdf(4,1)*rx(j1,j2,1,1)+dpdf(4,2)*rx(j1,j2,1,2))*v1r
     *          +(dpdf(4,3)*rx(j1,j2,1,1)+dpdf(4,4)*rx(j1,j2,1,2))*v2r)
c
            a(1,1)= an1*(
     *              dpdf(1,1)*rx(j1,j2,2,1)+dpdf(1,2)*rx(j1,j2,2,2))
     *             +an2*(
     *              dpdf(3,1)*rx(j1,j2,2,1)+dpdf(3,2)*rx(j1,j2,2,2))
            a(1,2)= an1*(
     *              dpdf(1,3)*rx(j1,j2,2,1)+dpdf(1,4)*rx(j1,j2,2,2))
     *             +an2*(
     *              dpdf(3,3)*rx(j1,j2,2,1)+dpdf(3,4)*rx(j1,j2,2,2))
            a(2,1)= an1*(
     *              dpdf(2,1)*rx(j1,j2,2,1)+dpdf(2,2)*rx(j1,j2,2,2))
     *             +an2*(
     *              dpdf(4,1)*rx(j1,j2,2,1)+dpdf(4,2)*rx(j1,j2,2,2))
            a(2,2)= an1*(
     *              dpdf(2,3)*rx(j1,j2,2,1)+dpdf(2,4)*rx(j1,j2,2,2))
     *             +an2*(
     *              dpdf(4,3)*rx(j1,j2,2,1)+dpdf(4,4)*rx(j1,j2,2,2))
c
c            if (j1.eq.n1a) then
c              write(6,*)a(1,1),a(1,2),a(2,1),a(2,2)
c              write(6,*)f(1),f(2)
c            end if
c
            det=a(1,1)*a(2,2)-a(1,2)*a(2,1)
            v1s=(f(1)*a(2,2)-f(2)*a(1,2))/det
            v2s=(f(2)*a(1,1)-f(1)*a(2,1))/det
            u(j1,j2-1,1)=u(j1,j2+1,1)-2.d0*ds2*v1s
            u(j1,j2-1,2)=u(j1,j2+1,2)-2.d0*ds2*v2s
c
c            if (j1.eq.n1a) then
c              write(6,*)u(j1,j2-1,1),u(j1,j2+1,1),v1s
c              write(6,*)u(j1,j2-1,2),u(j1,j2+1,2),v2s
c            end if
c
c            if (j1.eq.21) then
c              r1=a(1,1)*v1s+a(1,2)*v2s-f(1)
c              r2=a(2,1)*v1s+a(2,2)*v2s-f(2)
c              v1x=v1r*rx(j1,j2,1,1)+v1s*rx(j1,j2,2,1)
c              v1y=v1r*rx(j1,j2,1,2)+v1s*rx(j1,j2,2,2)
c              v2x=v2r*rx(j1,j2,1,1)+v2s*rx(j1,j2,2,1)
c              v2y=v2r*rx(j1,j2,1,2)+v2s*rx(j1,j2,2,2)
c              pdot(1,1)=  dpdf(1,1)*v1x+dpdf(1,2)*v1y
c     *                   +dpdf(1,3)*v2x+dpdf(1,4)*v2y
c              pdot(1,2)=  dpdf(2,1)*v1x+dpdf(2,2)*v1y
c     *                   +dpdf(2,3)*v2x+dpdf(2,4)*v2y
c              pdot(2,1)=  dpdf(3,1)*v1x+dpdf(3,2)*v1y
c     *                   +dpdf(3,3)*v2x+dpdf(3,4)*v2y
c              pdot(2,2)=  dpdf(4,1)*v1x+dpdf(4,2)*v1y
c     *                   +dpdf(4,3)*v2x+dpdf(4,4)*v2y
c              f(1)=an1*pdot(1,1)+an2*pdot(2,1)
c              f(2)=an1*pdot(1,2)+an2*pdot(2,2)
c              write(6,555)v1r,v1s,v2r,v2s,f(1),f(2),r1,r2
c  555         format(8(1x,1pe15.8))
c            end if
c
            goto 5
          end if
c
        end do
c
        write(6,*)'side 1 did not converge, j1=',j1
        stop 90
c
    5   continue
c        if (j1.eq.n1a) pause
      end do
c
c Neumann condition for displacement and velocity, j2=n2b
      j2=n2b
      do j1=n1a,n1b
        ! (an1,an2) = unit normal
        aNormi=1.d0/dsqrt(rx(j1,j2,2,1)**2+rx(j1,j2,2,2)**2)
        an1=rx(j1,j2,2,1)*aNormi
        an2=rx(j1,j2,2,2)*aNormi
c
        u1r=(u(j1+1,j2,7)-u(j1-1,j2,7))/(2.d0*ds1)
        u2r=(u(j1+1,j2,8)-u(j1-1,j2,8))/(2.d0*ds1)
        u1s=( 3.d0*u(j1,j2,7)-4.d0*u(j1,j2-1,7)+u(j1,j2-2,7))
     *       /(2.d0*ds2)
        u2s=( 3.d0*u(j1,j2,8)-4.d0*u(j1,j2-1,8)+u(j1,j2-2,8))
     *       /(2.d0*ds2)
c
        do it=1,itmax
c
          u1x=u1r*rx(j1,j2,1,1)+u1s*rx(j1,j2,2,1)
          u1y=u1r*rx(j1,j2,1,2)+u1s*rx(j1,j2,2,2)
          u2x=u2r*rx(j1,j2,1,1)+u2s*rx(j1,j2,2,1)
          u2y=u2r*rx(j1,j2,1,2)+u2s*rx(j1,j2,2,2)
c
          du(1,1)=u1x
          du(1,2)=u1y
          du(2,1)=u2x
          du(2,2)=u2y
c
          ideriv=1
          call smgetdp (du,p,dpdf,cpar,ideriv,itype)
c
          f(1)=an1*p(1,1)+an2*p(2,1)
          f(2)=an1*p(1,2)+an2*p(2,2)
          fmax=max(dabs(f(1)),dabs(f(2)))

          a(1,1)=an1*(dpdf(1,1)*rx(j1,j2,2,1)+dpdf(1,2)*rx(j1,j2,2,2))
     *          +an2*(dpdf(3,1)*rx(j1,j2,2,1)+dpdf(3,2)*rx(j1,j2,2,2))
          a(1,2)=an1*(dpdf(1,3)*rx(j1,j2,2,1)+dpdf(1,4)*rx(j1,j2,2,2))
     *          +an2*(dpdf(3,3)*rx(j1,j2,2,1)+dpdf(3,4)*rx(j1,j2,2,2))
          a(2,1)=an1*(dpdf(2,1)*rx(j1,j2,2,1)+dpdf(2,2)*rx(j1,j2,2,2))
     *          +an2*(dpdf(4,1)*rx(j1,j2,2,1)+dpdf(4,2)*rx(j1,j2,2,2))
          a(2,2)=an1*(dpdf(2,3)*rx(j1,j2,2,1)+dpdf(2,4)*rx(j1,j2,2,2))
     *          +an2*(dpdf(4,3)*rx(j1,j2,2,1)+dpdf(4,4)*rx(j1,j2,2,2))
c
          det=a(1,1)*a(2,2)-a(1,2)*a(2,1)
          d1=(f(1)*a(2,2)-f(2)*a(1,2))/det
          d2=(f(2)*a(1,1)-f(1)*a(2,1))/det
c
c          if (j1.eq.n1a) then
c            write(6,222)it,fmax,d1,d2
cc  222       format(1x,i2,3(1x,1pe10.3))
c          end if
c
          if (fmax.gt.toler) then
            u1s=u1s-d1
            u2s=u2s-d2
          else
            u(j1,j2+1,7)=u(j1,j2-1,7)+2.d0*ds2*u1s
            u(j1,j2+1,8)=u(j1,j2-1,8)+2.d0*ds2*u2s
c
c            if (j1.eq.n1a) then
c              write(6,444)u1r,u1s,u2r,u2s,p(1,1),p(1,2),p(2,1),p(2,2)
cc  444         format(2(4(1x,1pe15.8)))
c            end if
c
            v1r=(u(j1+1,j2,1)-u(j1-1,j2,1))/(2.d0*ds1)
            v2r=(u(j1+1,j2,2)-u(j1-1,j2,2))/(2.d0*ds1)
            f(1)=-an1*(
     *           (dpdf(1,1)*rx(j1,j2,1,1)+dpdf(1,2)*rx(j1,j2,1,2))*v1r
     *          +(dpdf(1,3)*rx(j1,j2,1,1)+dpdf(1,4)*rx(j1,j2,1,2))*v2r)
     *           -an2*(
     *           (dpdf(3,1)*rx(j1,j2,1,1)+dpdf(3,2)*rx(j1,j2,1,2))*v1r
     *          +(dpdf(3,3)*rx(j1,j2,1,1)+dpdf(3,4)*rx(j1,j2,1,2))*v2r)
            f(2)=-an1*(
     *           (dpdf(2,1)*rx(j1,j2,1,1)+dpdf(2,2)*rx(j1,j2,1,2))*v1r
     *          +(dpdf(2,3)*rx(j1,j2,1,1)+dpdf(2,4)*rx(j1,j2,1,2))*v2r)
     *           -an2*(
     *           (dpdf(4,1)*rx(j1,j2,1,1)+dpdf(4,2)*rx(j1,j2,1,2))*v1r
     *          +(dpdf(4,3)*rx(j1,j2,1,1)+dpdf(4,4)*rx(j1,j2,1,2))*v2r)
c
            a(1,1)= an1*(
     *              dpdf(1,1)*rx(j1,j2,2,1)+dpdf(1,2)*rx(j1,j2,2,2))
     *             +an2*(
     *              dpdf(3,1)*rx(j1,j2,2,1)+dpdf(3,2)*rx(j1,j2,2,2))
            a(1,2)= an1*(
     *              dpdf(1,3)*rx(j1,j2,2,1)+dpdf(1,4)*rx(j1,j2,2,2))
     *             +an2*(
     *              dpdf(3,3)*rx(j1,j2,2,1)+dpdf(3,4)*rx(j1,j2,2,2))
            a(2,1)= an1*(
     *              dpdf(2,1)*rx(j1,j2,2,1)+dpdf(2,2)*rx(j1,j2,2,2))
     *             +an2*(
     *              dpdf(4,1)*rx(j1,j2,2,1)+dpdf(4,2)*rx(j1,j2,2,2))
            a(2,2)= an1*(
     *              dpdf(2,3)*rx(j1,j2,2,1)+dpdf(2,4)*rx(j1,j2,2,2))
     *             +an2*(
     *              dpdf(4,3)*rx(j1,j2,2,1)+dpdf(4,4)*rx(j1,j2,2,2))
c
c            if (j1.eq.n1a) then
c              write(6,*)a(1,1),a(1,2),a(2,1),a(2,2)
c              write(6,*)f(1),f(2)
c            end if
c
            det=a(1,1)*a(2,2)-a(1,2)*a(2,1)
            v1s=(f(1)*a(2,2)-f(2)*a(1,2))/det
            v2s=(f(2)*a(1,1)-f(1)*a(2,1))/det
            u(j1,j2+1,1)=u(j1,j2-1,1)+2.d0*ds2*v1s
            u(j1,j2+1,2)=u(j1,j2-1,2)+2.d0*ds2*v2s
c
c            if (j1.eq.n1a) then
c              write(6,*)u(j1,j2+1,1),u(j1,j2-1,1),v1s
c              write(6,*)u(j1,j2+1,2),u(j1,j2-1,2),v2s
c            end if
c
c            if (j1.eq.21) then
c              r1=a(1,1)*v1s+a(1,2)*v2s-f(1)
c              r2=a(2,1)*v1s+a(2,2)*v2s-f(2)
c              v1x=v1r*rx(j1,j2,1,1)+v1s*rx(j1,j2,2,1)
c              v1y=v1r*rx(j1,j2,1,2)+v1s*rx(j1,j2,2,2)
c              v2x=v2r*rx(j1,j2,1,1)+v2s*rx(j1,j2,2,1)
c              v2y=v2r*rx(j1,j2,1,2)+v2s*rx(j1,j2,2,2)
c              pdot(1,1)=  dpdf(1,1)*v1x+dpdf(1,2)*v1y
c     *                   +dpdf(1,3)*v2x+dpdf(1,4)*v2y
c              pdot(1,2)=  dpdf(2,1)*v1x+dpdf(2,2)*v1y
c     *                   +dpdf(2,3)*v2x+dpdf(2,4)*v2y
c              pdot(2,1)=  dpdf(3,1)*v1x+dpdf(3,2)*v1y
c     *                   +dpdf(3,3)*v2x+dpdf(3,4)*v2y
c              pdot(2,2)=  dpdf(4,1)*v1x+dpdf(4,2)*v1y
c     *                   +dpdf(4,3)*v2x+dpdf(4,4)*v2y
c              f(1)=an1*pdot(1,1)+an2*pdot(2,1)
c              f(2)=an1*pdot(1,2)+an2*pdot(2,2)
c              write(6,555)v1r,v1s,v2r,v2s,f(1),f(2),r1,r2
cc  555         format(8(1x,1pe15.8))
c            end if
c
            goto 6
          end if
c
        end do
c
        write(6,*)'side 2 did not converge, j1=',j1
        stop 100
c
    6   continue
c        if (j1.eq.n1a) pause
      end do
c
c fix up stress (j2=n2a)
      j2=n2a
      do j1=n1a,n1b
c
        u1r=(u(j1+1,j2,7)-u(j1-1,j2,7))/(2.d0*ds1)
        u2r=(u(j1+1,j2,8)-u(j1-1,j2,8))/(2.d0*ds1)
        u1s=(u(j1,j2+1,7)-u(j1,j2-1,7))/(2.d0*ds2)
        u2s=(u(j1,j2+1,8)-u(j1,j2-1,8))/(2.d0*ds2)
c
        u1x=u1r*rx(j1,j2,1,1)+u1s*rx(j1,j2,2,1)
        u1y=u1r*rx(j1,j2,1,2)+u1s*rx(j1,j2,2,2)
        u2x=u2r*rx(j1,j2,1,1)+u2s*rx(j1,j2,2,1)
        u2y=u2r*rx(j1,j2,1,2)+u2s*rx(j1,j2,2,2)
c
        du(1,1)=u1x
        du(1,2)=u1y
        du(2,1)=u2x
        du(2,2)=u2y
c
        ideriv=0
        call smgetdp (du,p,dpdf,cpar,ideriv,itype)
c
        u(j1,j2,3)=p(1,1)
        u(j1,j2,4)=p(1,2)
        u(j1,j2,5)=p(2,1)
        u(j1,j2,6)=p(2,2)
c
        do i=3,6
          u(j1,j2-1,i)=3.d0*u(j1,j2,i)-3.d0*u(j1,j2+1,i)+u(j1,j2+2,i)
        end do
c
      end do
c
c fix up stress (j2=n2b)
      j2=n2b
      do j1=n1a,n1b
c
        u1r=(u(j1+1,j2,7)-u(j1-1,j2,7))/(2.d0*ds1)
        u2r=(u(j1+1,j2,8)-u(j1-1,j2,8))/(2.d0*ds1)
        u1s=(u(j1,j2+1,7)-u(j1,j2-1,7))/(2.d0*ds2)
        u2s=(u(j1,j2+1,8)-u(j1,j2-1,8))/(2.d0*ds2)
c
        u1x=u1r*rx(j1,j2,1,1)+u1s*rx(j1,j2,2,1)
        u1y=u1r*rx(j1,j2,1,2)+u1s*rx(j1,j2,2,2)
        u2x=u2r*rx(j1,j2,1,1)+u2s*rx(j1,j2,2,1)
        u2y=u2r*rx(j1,j2,1,2)+u2s*rx(j1,j2,2,2)
c
        du(1,1)=u1x
        du(1,2)=u1y
        du(2,1)=u2x
        du(2,2)=u2y
c
        ideriv=0
        call smgetdp (du,p,dpdf,cpar,ideriv,itype)
c
        u(j1,j2,3)=p(1,1)
        u(j1,j2,4)=p(1,2)
        u(j1,j2,5)=p(2,1)
        u(j1,j2,6)=p(2,2)
c
        do i=3,6
          u(j1,j2+1,i)=3.d0*u(j1,j2,i)-3.d0*u(j1,j2-1,i)+u(j1,j2-2,i)
        end do
c
      end do
c
c extrapolate to second ghost line (j2=n2a)
      j2=n2a
      do j1=n1a,n1b
        do i=1,m
          u(j1,j2-2,i)=3.d0*u(j1,j2-1,i)-3.d0*u(j1,j2,i)+u(j1,j2+1,i)
        end do
      end do
c
c extrapolate to second ghost line (j2=n2b)
      j2=n2b
      do j1=n1a,n1b
        do i=1,m
          u(j1,j2+2,i)=3.d0*u(j1,j2+1,i)-3.d0*u(j1,j2,i)+u(j1,j2-1,i)
        end do
      end do
c
c set periodic bcs
      do j2=n2a-2,n2b+2
        do i=1,m
          u(n1a-1,j2,i)=u(n1b-1,j2,i)
          u(n1a-2,j2,i)=u(n1b-2,j2,i)
          u(n1b+1,j2,i)=u(n1a+1,j2,i)
          u(n1b+2,j2,i)=u(n1a+2,j2,i)
        end do
      end do
c
      return
      end
