      subroutine smg2d (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                  ds1,ds2,dt,t,xy,rx,det,u,up,f1,f2,ad,mask,
     *                  nrprm,rparam,niprm,iparam,nrwk,rwk,niwk,
     *                  iwk,idebug,ier)
c
c compute du/dt for 2d linear elasticity (smg => solid mechanics, Godunov)
c
c variables: u(j1,j2,1:2)=velocity
c            u(j1,j2,3:6)=stress (S11,S12,S21,S22)
c            u(j1,j2,7:8)=displacement
c
      implicit real*8 (a-h,o-z)
      dimension xy(nd1a:nd1b,nd2a:nd2b,2),rx(nd1a:nd1b,nd2a:nd2b,2,2),
     *          det(*),u(nd1a:nd1b,nd2a:nd2b,m),almax(2),
     *          up(nd1a:nd1b,nd2a:nd2b,m),mask(nd1a:nd1b,nd2a:nd2b),
     *          f1(*),f2(*),rparam(nrprm),iparam(niprm),rwk(nrwk),
     *          iwk(niwk),ad(8)
      dimension wtmp(10000)             ! used for testing slope correction
      common / smgdat / amu,alam,rho0
      common / smgprm / method,iorder,ilimit,iupwind,icart,ifrc
      common / tzflow / eptz,itz
c
c      data ad / .5d0, .5d0, .5d0, .5d0, .5d0, .5d0, .5d0, .5d0 /
c      do i=1,m
c        ad(i)=0.d0
c      end do
c
c      write(6,*)'smg2d, t=',t
c      write(6,123)(u(0,0,i),i=3,6)
c      write(6,123)(u(30,0,i),i=3,6)
c      write(6,123)(u(0,20,i),i=3,6)
c      write(6,123)(u(30,20,i),i=3,6)
c  123 format(4(1x,1pe15.8))
c      write(6,*)n1a,n1b,n2a,n2b
c      write(6,*)nd1a,nd1b,nd2a,nd2b
c      pause
c
c      call chku (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,t,u)
c
c..set error flag
      ier=0
c
c      do i=1,8
c        ad(i)=1.d0
c      end do
c      ad(7)=2.d0
c      ad(8)=2.d0
c       write(6,*)ad
c       pause
c
c..parameters
      amu=rparam(3)
      alam=rparam(4)
      rho0=rparam(5)
      eptz=rparam(6)
      iorder=iparam(1)
      icart=iparam(2)
      itz=iparam(3)
      method=iparam(4)
      ilimit=iparam(5)
      if( ilimit.ne.0 .and. t.lt.dt )then
        write(*,'(" ++ smg2d:INFO: Limiting is ON")') 
      endif 
      iupwind=iparam(6)
      itype=iparam(7)
      ifrc=iparam(8)
      if( itype.ne.0 )then
         write(*,*) 'smg2d:ERROR: invalid itype'
         stop 8873
      end if

      iRelax = iparam(9)
c      ifrc=0
c
c      write(6,*)amu,alam,rho0,eptz,iorder,icart,itz
c      if (m.gt.0) stop
c
c      write(6,*)'order =',iorder
c      pause
c
c      iorder=2
c      method=0
c      iorder=1
c      write(6,*)'******* Setting iorder=1 (smg2d) ********'
c      write(6,*)amu,alam,rho0,iorder,icart
c      if (m.gt.0) stop
c
c..set array dimensions for parallel
      md1a=max(nd1a,n1a-2)
      md1b=min(nd1b,n1b+2)
      md2a=max(nd2a,n2a-2)
      md2b=min(nd2b,n2b+2)
c
c      er=0.d0
c      do i=1,m
c        do j2=n2a-2,n2b+2
c          do k=-2,2
c            er=max(dabs(u(n1a+k,j2,i)-u(n1b+k,j2,i)),er)
c          end do
c        end do
c      end do
c      write(6,*)'err =',er
c      pause
c
c..sanity check
      if (m.ne.8) then
        write(6,*)'Error (smg2d) : m=8 is assumed'
        stop
      end if
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
c compute stresses
c      do j2=n2a-1,n2b+1
c        do j1=n1a-1,n1b+1
c          du11=(u(j1+1,j2,7)-u(j1-1,j2,7))/(2*ds1)
c          du12=(u(j1,j2+1,7)-u(j1,j2-1,7))/(2*ds2)
c          du21=(u(j1+1,j2,8)-u(j1-1,j2,8))/(2*ds1)
c          du22=(u(j1,j2+1,8)-u(j1,j2-1,8))/(2*ds2)
c          u(j1,j2,3)=alam*(du11+du22)+2*amu*du11
c          u(j1,j2,4)=amu*(du12+du21)
c          u(j1,j2,5)=u(j1,j2,4)
c          u(j1,j2,6)=alam*(du11+du22)+2*amu*du22
c        end do
c      end do
cc extrapolate to the ghost points
c      do i=3,6
c        do j2=n2a-1,n2b+1
cc          u(n1a-1,j2,i)=2*u(n1a,j2,i)-u(n1a+1,j2,i)
c          u(n1a-2,j2,i)=2*u(n1a-1,j2,i)-u(n1a,j2,i)
cc          u(n1b+1,j2,i)=2*u(n1b,j2,i)-u(n1b-1,j2,i)
c          u(n1b+2,j2,i)=2*u(n1b+1,j2,i)-u(n1b,j2,i)
c        end do
c      end do
c      do i=3,6
c        do j1=md1a,md1b
cc          u(j1,n2a-1,i)=2*u(j1,n2a,i)-u(j1,n2a+1,i)
c          u(j1,n2a-2,i)=2*u(j1,n2a-1,i)-u(j1,n2a,i)
cc          u(j1,n2b+1,i)=2*u(j1,n2b,i)-u(j1,n2b-1,i)
c          u(j1,n2b+2,i)=2*u(j1,n2b+1,i)-u(j1,n2b,i)
c        end do
c      end do
c
c..apply boundary conditions (temporary) *** no longer works here, but
c  can be called by bcOptSmFOS
cc      call smgbcs (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,ds1,ds2,u)
c
c..assign corner values (temporary)
cc      call corner (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,ds1,ds2,
cc     *             t,xy,u)
c
      ngrid=(md1b-md1a+1)
c
c..split up real work space =>
      lw=1
      la1=lw+m*ngrid*10
      laj=la1+8*ngrid
      nreq=laj+2*ngrid-1
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

c      write(50,*)'u ='
c      do i=1,m
c        write(50,300)i
c  300   format('i=',i2)
c        do j2=md2a,md2b
c        do j1=md1a,md1b
c          msk=0
c          if (mask(j1,j2).ne.0) msk=1
c          write(50,301)j1,j2,u(j1,j2,i),msk
c  301     format(2(1x,i2),1x,1pe15.8,1x,i1)
c        end do
c        end do
c      end do
c
c       err=0.d0
c       tiny=1.d-12
c       do i=1,m
c         u0=u(n1a,n1b,i)
c         do j1=nd1a,nd1b
c         do j2=nd2a,nd2b
c           err=max(dabs(u(j1,j2,i)-u0),err)
c         end do
c         end do
c       end do
c       if (err.gt.tiny) then
c         write(6,*)'not uniform'
c         pause
c       else
c         write(6,*)'uniform'
c         pause
c       end if
c
c      if (m.gt.0) stop
c
c compute up
c      dt=0.001d0
      call getup2d (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
     *              md2a,md2b,n2a,n2b,ds1,ds2,dt,t,xy,rx,det,u,
     *              up,f1,f2,mask,almax,rwk(lw),rwk(la1),rwk(laj),
     *              ier,wtmp)
c
c
c add relaxation term to ensure compatibility of stress and position
c   jwb -- 10 Aug 2010
      if( iRelax.ne.0 ) then
        call stressRelax2d( m,nd1a,nd1b,nd2a,nd2b,
     *                        n1a, n1b, n2a, n2b,
     *                        ds1,ds2,dt,t,xy,rx,u,up,mask,
     *                        iparam,rparam )
      end if
c
c      do j2=n2a,n2b
c        write(55,555)j2,(up(0,j2,i),i=1,8)
c  555   format(1x,i2,8(1x,1pe10.3))
c      end do
c      do j2=n2a,n2b
c        write(56,555)j2,(up(30,j2,i),i=1,8)
c      end do
c      if (m.gt.0) stop
c
c artificial dissipation
       tiny=1.d-15
       admax=0.d0
       do i=1,m
         adi=ad(i)
         if (adi.gt.tiny) then
           admax=max(adi,admax)
           do j2=n2a,n2b
           do j1=n1a,n1b
             if (mask(j1,j2).ne.0) then
               up(j1,j2,i)=up(j1,j2,i)+adi*(u(j1+1,j2,i)+u(j1-1,j2,i)
     *                     +u(j1,j2+1,i)+u(j1,j2-1,i)-4.d0*u(j1,j2,i))
             end if
           end do
           end do
         end if
       end do

c      write(50,*)'icart=',icart
c      write(6,*)'icart=',icart
c      pause
c      do i=1,m
c        write(50,300)i
c        do j2=n2a,n2b
c        do j1=n1a,n1b
c        do j2=nd2a,nd2b
c        do j1=nd1a,nd1b
c          up(j1,j2,i)=0.d0
c          msk=0
c          if (mask(j1,j2).ne.0) msk=1
c          write(50,301)j1,j2,up(j1,j2,i),msk
c        end do
c        end do
c      end do
c
c      do j2=n2a,n2b
c      do j1=n1a,n1b
c        write(1,100)(up(j1,j2,i),i=1,8)
c  100   format(8(1x,1pe10.3))
c      end do
c      end do
c      if (m.gt.0) stop
c
c compute real and imaginary parts of lambda, where the time stepping
c is interpreted as u'=lambda*u
c
      rparam(1)=4.d0*admax
      rparam(2)=almax(1)/ds1+almax(2)/ds2

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
      subroutine chku (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,t,u)
c
      implicit real*8 (a-h,o-z)
      dimension u(nd1a:nd1b,nd2a:nd2b,m),ue(8),emax(8)
c
      pi=4.d0*datan(1.d0)
      dth=2.d0*pi/n1b
      dr=.5d0/n2b
c
      p0=1.d0
      p1=2.d0
      r0=.5d0
      r1=1.d0
      alam=1.d0
      amu=1.d0
      a=(p1*r1*r1-p0*r0*r0)/(r0*r0-r1*r1)
      b=(p1-p0)*r0*r0*r1*r1/(r0*r0-r1*r1)
      au=a/(2.d0*(alam+amu))
      bu=b/(2.d0*amu)
c
      do i=1,8
        emax(i)=0.d0
      end do
c
      do j2=nd2a,nd2b
      do j1=nd1a,nd1b
        th=j1*dth
        r=r0+j2*dr
        x=r*dcos(th)
        y=r*dsin(th)
        ue(1)=0.d0
        ue(2)=0.d0
        ue(3)=a-b*dcos(2.d0*th)/(r*r)
        ue(4)= -b*dsin(2.d0*th)/(r*r)
        ue(5)=ue(4)
        ue(6)=a+b*dcos(2.d0*th)/(r*r)
        ue(7)=(au*r+bu/r)*dcos(th)
        ue(8)=(au*r+bu/r)*dsin(th)
c        write(1,100)x,y,(u(j1,j2,i)-ue(i),i=3,8)
c  100   format(2(1x,1pe10.3),6(1x,1pe15.8))
c        do i=1,8
c          emax(i)=max(dabs(u(j1,j2,i)-ue(i)),emax(i))
c        end do
c        if (j2.le.n2a.or.j2.ge.n2b) then
        if (j2.lt.n2a.or.j2.gt.n2b) then
          do i=1,2
            u(j1,j2,i)=ue(i)
          end do
c        else
c          if (j1.lt.n1a) then
c            do i=1,8
c              u(j1,j2,i)=u(j1+n1b,j2,i)
c            end do
c          else
c            if (j1.gt.n1b) then
c              do i=1,8
c                u(j1,j2,i)=u(j1-n1b,j2,i)
c              end do
c            end if
c          end if
        end if
        do i=1,8
          emax(i)=max(dabs(u(j1,j2,i)-ue(i)),emax(i))
        end do
      end do
      end do
c
c      write(6,200)(emax(i),i=1,8)
c  200 format('Errors :',8(1x,1pe9.2))
c
c      if (m.gt.0) stop
c
c      do j2=nd2a,nd2b
c      do j1=nd1a,nd1b
c        do i=1,8
c          u(j1,j2,i)=i
c        end do
c      end do
c      end do
c
      return
      end
c
c+++++++++++++++++++++++
c
      subroutine getup2d (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
     *                    md2a,md2b,n2a,n2b,ds1,ds2,dt,t,xy,rx,det,
     *                    u,up,f1,f2,mask,almax,w,a1,aj,ier,wtmp)
c
      implicit real*8 (a-h,o-z)
      dimension xy(nd1a:nd1b,nd2a:nd2b,2),rx(nd1a:nd1b,nd2a:nd2b,2,2),
     *          det(nd1a:nd1b,nd2a:nd2b),u(nd1a:nd1b,nd2a:nd2b,m),
     *          up(nd1a:nd1b,nd2a:nd2b,m),f1(nd1a:nd1b,nd2a:nd2b,2),
     *          f2(nd1a:nd1b,nd2a:nd2b,2),mask(nd1a:nd1b,nd2a:nd2b),
     *          almax(2),w(m,md1a:md1b,5,2),a1(2,2,md1a:md1b,2),
     *          aj(md1a:md1b,2)
c      dimension wtmp(m,md1a:md1b,5),errw(6,5)
      dimension fx(6),htz(8),fxtmp(6),errf(6),ul(6),ur(6),
     *          du1(6),du2(6),ut(6)
      common / smgprm / method,iorder,ilimit,iupwind,icart,ifrc
      common / smgdat / amu,alam,rho0
      common / tzflow / eptz,itz

c      do i=1,6
c        errf(i)=0.d0
c        do k=1,5
c          errw(i,k)=0.d0
c        end do
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
c..add body force (if necessary)
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
c..compute coeff.s for slope correction (if necessary)
      if (iorder.ne.1.and.itz.eq.0.and.
     *    ilimit.eq.0.and.iupwind.eq.0) then
        if (icart.ne.0) then
          c1x=dt/(rho0*ds1)
          c2x=c1x
          c3x=(alam+2.d0*amu)*dt/ds1
          c4x=amu*dt/ds1
          c5x=c4x
          c6x=alam*dt/ds1
          c1y=dt/(rho0*ds2)
          c2y=c1y
          c3y=alam*dt/ds2
          c4y=amu*dt/ds2
          c5y=c4y
          c6y=(alam+2.d0*amu)*dt/ds2
        else
          c11r=dt/(rho0*ds1)
          c12r=c11r
          c21r=c11r
          c22r=c11r
          c31r=(alam+2.d0*amu)*dt/ds1
          c32r=alam*dt/ds1
          c41r=amu*dt/ds1
          c42r=c41r
          c51r=c41r
          c52r=c41r
          c61r=c32r
          c62r=c31r
          c11s=dt/(rho0*ds2)
          c12s=c11s
          c21s=c11s
          c22s=c11s
          c31s=(alam+2.d0*amu)*dt/ds2
          c32s=alam*dt/ds2
          c41s=amu*dt/ds2
          c42s=c41s
          c51s=c41s
          c52s=c41s
          c61s=c32s
          c62s=c31s
        end if
      end if
c
c..compute coeff.s for Godunov flux solver (if mesh is Cartesian)
      if (icart.ne.0.and.method.eq.0) then
        c1=dsqrt((alam+2.d0*amu)/rho0)
        c2=dsqrt(amu/rho0)
        almax(1)=c1
        almax(2)=c1
        cf1=.5d0/rho0
        cf2=.5d0*(alam+2.d0*amu)
        cf3=.5d0*alam
        cf4=.5d0*amu
        df1=.5d0*c1
        df2=.5d0*c2
        df3=.5d0*c1*alam/(alam+2.d0*amu)
      end if
c
      j2=n2a-1
c
c..set grid metrics and velocity (if necessary)
      if (icart.eq.0.or.method.ne.0) then
        call smmetrics2d (nd1a,nd1b,md1a,md1b,nd2a,nd2b,j2,rx,det,
     *                    a1(1,1,md1a,1),aj(md1a,1))
      end if
c
      if (method.ne.2) then
        if (icart.eq.0) then
c          if (.true. .or.iorder.eq.1.or.itz.ne.0.or.
          if (iorder.eq.1.or.itz.ne.0.or.
     *        ilimit.ne.0.or.iupwind.ne.0) then
            call smslope2d (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
     *                      j2,ds1,ds2,dt,t,xy,a1(1,1,md1a,1),
     *                      aj(md1a,1),mask(nd1a,j2),u,
     *                      w(1,md1a,1,1),f1)
          else
            do j1=n1a-1,n1b+1                      ! optimized for ilimit=iupwind=0 (curvilinear case)
              if (mask(j1,j2).ne.0) then
                do i=1,6
                  du1(i)=.25d0*(u(j1+1,j2,i)-u(j1-1,j2,i))
                  du2(i)=.25d0*(u(j1,j2+1,i)-u(j1,j2-1,i))
                end do
                ut(1)=u(j1,j2,1)+rx(j1,j2,1,1)*c11r*du1(3)
     *                          +rx(j1,j2,1,2)*c12r*du1(5)
     *                          +rx(j1,j2,2,1)*c11s*du2(3)
     *                          +rx(j1,j2,2,2)*c12s*du2(5)
                ut(2)=u(j1,j2,2)+rx(j1,j2,1,1)*c21r*du1(4)
     *                          +rx(j1,j2,1,2)*c22r*du1(6)
     *                          +rx(j1,j2,2,1)*c21s*du2(4)
     *                          +rx(j1,j2,2,2)*c22s*du2(6)
                ut(3)=u(j1,j2,3)+rx(j1,j2,1,1)*c31r*du1(1)
     *                          +rx(j1,j2,1,2)*c32r*du1(2)
     *                          +rx(j1,j2,2,1)*c31s*du2(1)
     *                          +rx(j1,j2,2,2)*c32s*du2(2)
                ut(4)=u(j1,j2,4)+rx(j1,j2,1,1)*c41r*du1(2)
     *                          +rx(j1,j2,1,2)*c42r*du1(1)
     *                          +rx(j1,j2,2,1)*c41s*du2(2)
     *                          +rx(j1,j2,2,2)*c42s*du2(1)
                ut(5)=u(j1,j2,5)+rx(j1,j2,1,1)*c51r*du1(2)
     *                          +rx(j1,j2,1,2)*c52r*du1(1)
     *                          +rx(j1,j2,2,1)*c51s*du2(2)
     *                          +rx(j1,j2,2,2)*c52s*du2(1)
                ut(6)=u(j1,j2,6)+rx(j1,j2,1,1)*c61r*du1(1)
     *                          +rx(j1,j2,1,2)*c62r*du1(2)
     *                          +rx(j1,j2,2,1)*c61s*du2(1)
     *                          +rx(j1,j2,2,2)*c62s*du2(2)
                do i=1,6
                  w(i,j1,1,1)=ut(i)-du1(i)
                  w(i,j1,2,1)=ut(i)+du1(i)
                  w(i,j1,3,1)=ut(i)-du2(i)
                  w(i,j1,4,1)=ut(i)+du2(i)
                  w(i,j1,5,1)=ut(i)
                end do
              end if
            end do
            if (ifrc.ne.0) then
              do j1=n1a-1,n1b+1
                if (mask(j1,j2).ne.0) then
                  do i=1,2
                    tmp=.5d0*dt*f1(j1,j2,i)
                    do k=1,5
                      w(i,j1,k,1)=w(i,j1,k,1)+tmp
                    end do
                  end do
                end if
              end do
            end if
          end if
        else
          if (iorder.eq.1.or.itz.ne.0.or.
     *        ilimit.ne.0.or.iupwind.ne.0) then
c            call smslope2d (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
c     *                      j2,ds1,ds2,dt,t,xy,a1(1,1,md1a,1),
c     *                      aj(md1a,1),mask(nd1a,j2),u,wtmp)
            call smslope2dc (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
     *                       j2,ds1,ds2,dt,t,xy,mask(nd1a,j2),u,
     *                       w(1,md1a,1,1),f1)
          else
            do j1=n1a-1,n1b+1                      ! optimized for ilimit=iupwind=0 (Cartesian case)
              if (mask(j1,j2).ne.0) then
                do i=1,6
                  du1(i)=.25d0*(u(j1+1,j2,i)-u(j1-1,j2,i))
                  du2(i)=.25d0*(u(j1,j2+1,i)-u(j1,j2-1,i))
                end do
                ut(1)=u(j1,j2,1)+c1x*du1(3)+c1y*du2(5)
                ut(2)=u(j1,j2,2)+c2x*du1(4)+c2y*du2(6)
                ut(3)=u(j1,j2,3)+c3x*du1(1)+c3y*du2(2)
                ut(4)=u(j1,j2,4)+c4x*du1(2)+c4y*du2(1)
                ut(5)=u(j1,j2,5)+c5x*du1(2)+c5y*du2(1)
                ut(6)=u(j1,j2,6)+c6x*du1(1)+c6y*du2(2)
                do i=1,6
                  w(i,j1,1,1)=ut(i)-du1(i)
                  w(i,j1,2,1)=ut(i)+du1(i)
                  w(i,j1,3,1)=ut(i)-du2(i)
                  w(i,j1,4,1)=ut(i)+du2(i)
                  w(i,j1,5,1)=ut(i)
                end do
              end if
            end do
            if (ifrc.ne.0) then
              do j1=n1a-1,n1b+1
                if (mask(j1,j2).ne.0) then
                  do i=1,2
                    tmp=.5d0*dt*f1(j1,j2,i)
                    do k=1,5
                      w(i,j1,k,1)=w(i,j1,k,1)+tmp
                    end do
                  end do
                end if
              end do
            end if
          end if
c          do j1=n1a-1,n1b+1
c            if (mask(j1,j2).ne.0) then
c              do k=1,5
c              do i=1,6
c                errw(i,k)=max(dabs(w(i,j1,k,1)-wtmp(i,j1,k)),errw(i,k))
c              end do
c              end do
c            end if
c          end do
        end if
      end if
c
c..loop over lines j2=n2a:n2b+1
      do j2=n2a,n2b+1
        j2m1=j2-1
c
c..set grid metrics and velocity (if necessary)
        if (icart.eq.0.or.method.ne.0) then
          call smmetrics2d (nd1a,nd1b,md1a,md1b,nd2a,nd2b,j2,rx,det,
     *                      a1(1,1,md1a,2),aj(md1a,2))
        end if
c
c..slope correction (top row of cells)
        if (method.ne.2) then
          if (icart.eq.0) then
c            if (.true. .or.iorder.eq.1.or.itz.ne.0.or.
            if (iorder.eq.1.or.itz.ne.0.or.
     *          ilimit.ne.0.or.iupwind.ne.0) then
c              call smslope2d (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
c     *                        j2,ds1,ds2,dt,t,xy,a1(1,1,md1a,2),
c     *                        aj(md1a,2),mask(nd1a,j2),u,
c     *                        wtmp,f1)
              call smslope2d (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
     *                        j2,ds1,ds2,dt,t,xy,a1(1,1,md1a,2),
     *                        aj(md1a,2),mask(nd1a,j2),u,
     *                        w(1,md1a,1,2),f1)
            else
              do j1=n1a-1,n1b+1                      ! optimized for ilimit=iupwind=0 (curvilinear case)
                if (mask(j1,j2).ne.0) then
                  do i=1,6
                    du1(i)=.25d0*(u(j1+1,j2,i)-u(j1-1,j2,i))
                    du2(i)=.25d0*(u(j1,j2+1,i)-u(j1,j2-1,i))
                  end do
                  ut(1)=u(j1,j2,1)+rx(j1,j2,1,1)*c11r*du1(3)
     *                            +rx(j1,j2,1,2)*c12r*du1(5)
     *                            +rx(j1,j2,2,1)*c11s*du2(3)
     *                            +rx(j1,j2,2,2)*c12s*du2(5)
                  ut(2)=u(j1,j2,2)+rx(j1,j2,1,1)*c21r*du1(4)
     *                            +rx(j1,j2,1,2)*c22r*du1(6)
     *                            +rx(j1,j2,2,1)*c21s*du2(4)
     *                            +rx(j1,j2,2,2)*c22s*du2(6)
                  ut(3)=u(j1,j2,3)+rx(j1,j2,1,1)*c31r*du1(1)
     *                            +rx(j1,j2,1,2)*c32r*du1(2)
     *                            +rx(j1,j2,2,1)*c31s*du2(1)
     *                            +rx(j1,j2,2,2)*c32s*du2(2)
                  ut(4)=u(j1,j2,4)+rx(j1,j2,1,1)*c41r*du1(2)
     *                            +rx(j1,j2,1,2)*c42r*du1(1)
     *                            +rx(j1,j2,2,1)*c41s*du2(2)
     *                            +rx(j1,j2,2,2)*c42s*du2(1)
                  ut(5)=u(j1,j2,5)+rx(j1,j2,1,1)*c51r*du1(2)
     *                            +rx(j1,j2,1,2)*c52r*du1(1)
     *                            +rx(j1,j2,2,1)*c51s*du2(2)
     *                            +rx(j1,j2,2,2)*c52s*du2(1)
                  ut(6)=u(j1,j2,6)+rx(j1,j2,1,1)*c61r*du1(1)
     *                            +rx(j1,j2,1,2)*c62r*du1(2)
     *                            +rx(j1,j2,2,1)*c61s*du2(1)
     *                            +rx(j1,j2,2,2)*c62s*du2(2)
                  do i=1,6
                    w(i,j1,1,2)=ut(i)-du1(i)
                    w(i,j1,2,2)=ut(i)+du1(i)
                    w(i,j1,3,2)=ut(i)-du2(i)
                    w(i,j1,4,2)=ut(i)+du2(i)
                    w(i,j1,5,2)=ut(i)
                  end do
                end if
              end do
              if (ifrc.ne.0) then
                do j1=n1a-1,n1b+1
                  if (mask(j1,j2).ne.0) then
                    do i=1,2
                      tmp=.5d0*dt*f1(j1,j2,i)
                      do k=1,5
                        w(i,j1,k,2)=w(i,j1,k,2)+tmp
                      end do
                    end do
                  end if
                end do
              end if
            end if
c            do j1=n1a-1,n1b+1
c              if (mask(j1,j2).ne.0) then
c                do k=1,5
c                do i=1,6
c                  errw(i,k)=max(dabs(w(i,j1,k,2)-wtmp(i,j1,k)),
c     *                          errw(i,k))
c                end do
c                end do
c              end if
c            end do
          else
            if (iorder.eq.1.or.itz.ne.0.or.
     *          ilimit.ne.0.or.iupwind.ne.0) then
c              call smslope2d (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
c     *                        j2,ds1,ds2,dt,t,xy,a1(1,1,md1a,2),
c     *                        aj(md1a,2),mask(nd1a,j2),u,wtmp)
c              call smslope2dc0 (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
c     *                         j2,ds1,ds2,dt,t,xy,a1(1,1,md1a,2),
c     *                         aj(md1a,2),mask(nd1a,j2),u,
c     *                         wtmp,f1)
c              call smslope2dc (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
c     *                         j2,ds1,ds2,dt,t,xy,mask(nd1a,j2),u,
c     *                         wtmp,f1)
              call smslope2dc (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
     *                         j2,ds1,ds2,dt,t,xy,mask(nd1a,j2),u,
     *                         w(1,md1a,1,2),f1)
            else
              do j1=n1a-1,n1b+1                      ! optimized for ilimit=iupwind=0 (Cartesian case)
                if (mask(j1,j2).ne.0) then
                  do i=1,6
                    du1(i)=.25d0*(u(j1+1,j2,i)-u(j1-1,j2,i))
                    du2(i)=.25d0*(u(j1,j2+1,i)-u(j1,j2-1,i))
                  end do
                  ut(1)=u(j1,j2,1)+c1x*du1(3)+c1y*du2(5)
                  ut(2)=u(j1,j2,2)+c2x*du1(4)+c2y*du2(6)
                  ut(3)=u(j1,j2,3)+c3x*du1(1)+c3y*du2(2)
                  ut(4)=u(j1,j2,4)+c4x*du1(2)+c4y*du2(1)
                  ut(5)=u(j1,j2,5)+c5x*du1(2)+c5y*du2(1)
                  ut(6)=u(j1,j2,6)+c6x*du1(1)+c6y*du2(2)
                  do i=1,6
                    w(i,j1,1,2)=ut(i)-du1(i)
                    w(i,j1,2,2)=ut(i)+du1(i)
                    w(i,j1,3,2)=ut(i)-du2(i)
                    w(i,j1,4,2)=ut(i)+du2(i)
                    w(i,j1,5,2)=ut(i)
                  end do
                end if
              end do
              if (ifrc.ne.0) then
                do j1=n1a-1,n1b+1
                  if (mask(j1,j2).ne.0) then
                    do i=1,2
                      tmp=.5d0*dt*f1(j1,j2,i)
                      do k=1,5
                        w(i,j1,k,2)=w(i,j1,k,2)+tmp
                      end do
                    end do
                  end if
                end do
              end if
            end if
c            do j1=n1a-1,n1b+1
c              if (mask(j1,j2).ne.0) then
c                do k=1,5
c                do i=1,6
c                  errw(i,k)=max(dabs(w(i,j1,k,2)-wtmp(i,j1,k)),
c     *                          errw(i,k))
c                end do
c                end do
c              end if
c            end do
          end if
        end if
c
c..compute s2 flux along j2-1/2, add it to up(j1,j2,.) and up(j1,j2-1,.)
        if (method.eq.0) then
          if (icart.eq.0) then
            do j1=n1a,n1b
              if (mask(j1,j2).ne.0.and.mask(j1,j2m1).ne.0) then
                aj0=(aj(j1,2)+aj(j1,1))/2.d0
                a21=(a1(2,1,j1,2)+a1(2,1,j1,1))/2.d0
                a22=(a1(2,2,j1,2)+a1(2,2,j1,1))/2.d0
                call smflux2d (m,aj0,a21,a22,w(1,j1,4,1),w(1,j1,3,2),
     *                         fx,almax(2))
c                call smfx2dlw (m,dt,ds2,ds1,nd1a,nd1b,nd2a,nd2b,
c     *                         j1,j2m1,2,det,rx,u,fxtmp,dum)
                do i=1,6
c                  fx(i)=fxtmp(i)
c                  errf(i)=max(dabs(fxtmp(i)-fx(i)),errf(i))
                  up(j1,j2  ,i)=up(j1,j2  ,i)+fx(i)/(ds2*aj(j1,2))
                  up(j1,j2m1,i)=up(j1,j2m1,i)-fx(i)/(ds2*aj(j1,1))
                end do
              end if
            end do
          else
            do j1=n1a,n1b
              if (mask(j1,j2).ne.0.and.mask(j1,j2m1).ne.0) then
                do i=1,6
                  ul(i)=w(i,j1,4,1)
                  ur(i)=w(i,j1,3,2)
                end do
                fx(1)=(-cf1*(ul(5)+ur(5))-df2*(ur(1)-ul(1)))/ds2
                fx(2)=(-cf1*(ul(6)+ur(6))-df1*(ur(2)-ul(2)))/ds2
                fx(3)=(-cf3*(ul(2)+ur(2))-df3*(ur(6)-ul(6)))/ds2
                fx(4)=(-cf4*(ul(1)+ur(1))-df2*(ur(5)-ul(5)))/ds2
                fx(5)=fx(4)
                fx(6)=(-cf2*(ul(2)+ur(2))-df1*(ur(6)-ul(6)))/ds2
c                aj0=(aj(j1,2)+aj(j1,1))/2.d0
c                a21=(a1(2,1,j1,2)+a1(2,1,j1,1))/2.d0
c                a22=(a1(2,2,j1,2)+a1(2,2,j1,1))/2.d0
c                call smflux2d (m,aj0,a21,a22,w(1,j1,4,1),w(1,j1,3,2),
c     *                         fxtmp,dum)
                do i=1,6
c                  if (dabs(fx(i)-fxtmp(i)/ds2).gt.1.d-13) then
c                    write(6,*)'oops(y),i=',i
c                    stop
c                  end if
                  up(j1,j2  ,i)=up(j1,j2  ,i)+fx(i)
                  up(j1,j2m1,i)=up(j1,j2m1,i)-fx(i)
                end do
              end if
            end do
          end if
        elseif (method.eq.3) then
          do j1=n1a,n1b
            if (mask(j1,j2).ne.0.and.mask(j1,j2m1).ne.0) then
              aj0=(aj(j1,2)+aj(j1,1))/2.d0
              a21=(a1(2,1,j1,2)+a1(2,1,j1,1))/2.d0
              a22=(a1(2,2,j1,2)+a1(2,2,j1,1))/2.d0
              call smfx2dHLL (m,aj0,a21,a22,w(1,j1,4,1),w(1,j1,3,2),
     *                        fx,almax(2))
              do i=1,6
                up(j1,j2  ,i)=up(j1,j2  ,i)+fx(i)/(ds2*aj(j1,2))
                up(j1,j2m1,i)=up(j1,j2m1,i)-fx(i)/(ds2*aj(j1,1))
              end do
            end if
          end do
        elseif (method.eq.1) then
          do j1=n1a,n1b
            if (mask(j1,j2).ne.0.and.mask(j1,j2m1).ne.0) then
              aj0=(aj(j1,2)+aj(j1,1))/2.d0
              a21=(a1(2,1,j1,2)+a1(2,1,j1,1))/2.d0
              a22=(a1(2,2,j1,2)+a1(2,2,j1,1))/2.d0
              call smfx2dlf (m,dt,ds2,aj0,a21,a22,w(1,j1,4,1),
     *                       w(1,j1,3,2),fx,almax(2))
c              call smfx2dlf (m,dt,ds2,aj(j1,1),a1(2,1,j1,1),
c     *                       a1(2,2,j1,1),aj(j1,2),a1(2,1,j1,2),
c     *                       a1(2,2,j1,2),w(1,j1,4,1),w(1,j1,3,2),
c     *                       fx,almax(2))
              do i=1,6
                up(j1,j2  ,i)=up(j1,j2  ,i)+fx(i)/(ds2*aj(j1,2))
                up(j1,j2m1,i)=up(j1,j2m1,i)-fx(i)/(ds2*aj(j1,1))
              end do
            end if
          end do
        elseif (method.eq.2) then
          do j1=n1a,n1b
            if (mask(j1,j2).ne.0.and.mask(j1,j2m1).ne.0) then
              call smfx2dlw (m,dt,ds2,ds1,nd1a,nd1b,nd2a,nd2b,
     *                       j1,j2m1,2,det,rx,u,fx,almax(2))
              do i=1,6
                up(j1,j2  ,i)=up(j1,j2  ,i)+fx(i)/(ds2*aj(j1,2))
                up(j1,j2m1,i)=up(j1,j2m1,i)-fx(i)/(ds2*aj(j1,1))
              end do
            end if
          end do
        else
          write(6,*)'Error (getup2d) : method not supported'
          stop
        end if
c
c..if j2.le.n2b, then compute fluxes in the s1 direction
        if (j2.le.n2b) then
c
c..reset metrics and w
          if (icart.eq.0.or.method.ne.0) then
            do j1=n1a-1,n1b+1
              aj(j1,1)=aj(j1,2)
              a1(1,1,j1,1)=a1(1,1,j1,2)
              a1(1,2,j1,1)=a1(1,2,j1,2)
              a1(2,1,j1,1)=a1(2,1,j1,2)
              a1(2,2,j1,1)=a1(2,2,j1,2)
              do k=1,5
                do i=1,m
                  w(i,j1,k,1)=w(i,j1,k,2)
                end do
              end do
            end do
          else
            do j1=n1a-1,n1b+1
              do k=1,5
                do i=1,m
                  w(i,j1,k,1)=w(i,j1,k,2)
                end do
              end do
            end do
          end if
c
c..compute s1 flux along j2, add it to up(j1+1,j2,.) and up(j1,j2,.)
          if (method.eq.0) then
            if (icart.eq.0) then
              do j1=n1a-1,n1b
                j1p1=j1+1
                if (mask(j1,j2).ne.0.and.mask(j1p1,j2).ne.0) then
                  aj0=(aj(j1p1,1)+aj(j1,1))/2.d0
                  a11=(a1(1,1,j1p1,1)+a1(1,1,j1,1))/2.d0
                  a12=(a1(1,2,j1p1,1)+a1(1,2,j1,1))/2.d0
                  call smflux2d (m,aj0,a11,a12,w(1,j1,2,1),
     *                           w(1,j1p1,1,1),fx,almax(1))
c                  call smfx2dlw (m,dt,ds1,ds2,nd1a,nd1b,nd2a,nd2b,
c     *                           j1,j2,1,det,rx,u,fxtmp,dum)
                  do i=1,6
c                    fx(i)=fxtmp(i)
c                    errf(i)=max(dabs(fxtmp(i)-fx(i)),errf(i))
                    up(j1p1,j2,i)=up(j1p1,j2,i)+fx(i)/(ds1*aj(j1p1,1))
                    up(j1  ,j2,i)=up(j1  ,j2,i)-fx(i)/(ds1*aj(j1  ,1))
                  end do
                end if
              end do
            else
              do j1=n1a-1,n1b
                j1p1=j1+1
                if (mask(j1,j2).ne.0.and.mask(j1p1,j2).ne.0) then
                  do i=1,6
                    ul(i)=w(i,j1,2,1)
                    ur(i)=w(i,j1p1,1,1)
                  end do
                  fx(1)=(-cf1*(ul(3)+ur(3))-df1*(ur(1)-ul(1)))/ds1
                  fx(2)=(-cf1*(ul(4)+ur(4))-df2*(ur(2)-ul(2)))/ds1
                  fx(3)=(-cf2*(ul(1)+ur(1))-df1*(ur(3)-ul(3)))/ds1
                  fx(4)=(-cf4*(ul(2)+ur(2))-df2*(ur(4)-ul(4)))/ds1
                  fx(5)=fx(4)
                  fx(6)=(-cf3*(ul(1)+ur(1))-df3*(ur(3)-ul(3)))/ds1
c                  aj0=(aj(j1p1,1)+aj(j1,1))/2.d0
c                  a11=(a1(1,1,j1p1,1)+a1(1,1,j1,1))/2.d0
c                  a12=(a1(1,2,j1p1,1)+a1(1,2,j1,1))/2.d0
c                  call smflux2d (m,aj0,a11,a12,w(1,j1,2,1),
c     *                           w(1,j1p1,1,1),fxtmp,dum)
                  do i=1,6
c                    if (dabs(fx(i)-fxtmp(i)/ds1).gt.1.d-10) then
c                      write(6,*)'oops(x),i=',i
c                      stop
c                    end if
                    up(j1p1,j2,i)=up(j1p1,j2,i)+fx(i)
                    up(j1  ,j2,i)=up(j1  ,j2,i)-fx(i)
                  end do
                end if
              end do
            end if
          elseif (method.eq.3) then
            do j1=n1a-1,n1b
              j1p1=j1+1
              if (mask(j1,j2).ne.0.and.mask(j1p1,j2).ne.0) then
                aj0=(aj(j1p1,1)+aj(j1,1))/2.d0
                a11=(a1(1,1,j1p1,1)+a1(1,1,j1,1))/2.d0
                a12=(a1(1,2,j1p1,1)+a1(1,2,j1,1))/2.d0
                call smfx2dHLL (m,aj0,a11,a12,w(1,j1,2,1),w(1,j1p1,1,1),
     *                          fx,almax(1))
                do i=1,6
                  up(j1p1,j2,i)=up(j1p1,j2,i)+fx(i)/(ds1*aj(j1p1,1))
                  up(j1  ,j2,i)=up(j1  ,j2,i)-fx(i)/(ds1*aj(j1  ,1))
                end do
              end if
            end do
          elseif (method.eq.1) then
            do j1=n1a-1,n1b
              j1p1=j1+1
              if (mask(j1,j2).ne.0.and.mask(j1p1,j2).ne.0) then
                aj0=(aj(j1p1,1)+aj(j1,1))/2.d0
                a11=(a1(1,1,j1p1,1)+a1(1,1,j1,1))/2.d0
                a12=(a1(1,2,j1p1,1)+a1(1,2,j1,1))/2.d0
                call smfx2dlf (m,dt,ds1,aj0,a11,a12,w(1,j1,2,1),
     *                         w(1,j1p1,1,1),fx,almax(1))
c                call smfx2dlf (m,dt,ds1,aj(j1,1),a1(1,1,j1,1),
c     *                         a1(1,2,j1,1),aj(j1p1,1),a1(1,1,j1p1,1),
c     *                         a1(1,2,j1p1,1),w(1,j1,2,1),
c     *                         w(1,j1p1,1,1),fx,almax(1))
                do i=1,6
                  up(j1p1,j2,i)=up(j1p1,j2,i)+fx(i)/(ds1*aj(j1p1,1))
                  up(j1  ,j2,i)=up(j1  ,j2,i)-fx(i)/(ds1*aj(j1  ,1))
                end do
              end if
            end do
          elseif (method.eq.2) then
            do j1=n1a-1,n1b
              j1p1=j1+1
              if (mask(j1,j2).ne.0.and.mask(j1p1,j2).ne.0) then
                call smfx2dlw (m,dt,ds1,ds2,nd1a,nd1b,nd2a,nd2b,
     *                         j1,j2,1,det,rx,u,fx,almax(1))
                do i=1,6
                  up(j1p1,j2,i)=up(j1p1,j2,i)+fx(i)/(ds1*aj(j1p1,1))
                  up(j1  ,j2,i)=up(j1  ,j2,i)-fx(i)/(ds1*aj(j1  ,1))
                end do
              end if
            end do
          else
            write(6,*)'Error (getup2d) : method not supported'
            stop
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
c                        fx(1)=-w(3,j1,5,1)/rho0
c                        fx(2)=-w(4,j1,5,1)/rho0
c                        fx(3)=-(alam+2*amu)*w(1,j1,5,1)
c                        fx(4)=-amu*w(2,j1,5,1)
c                        fx(5)= fx(4)
c                        fx(6)=-alam*w(1,j1,5,1)
                        call smflx (m,1.d0,1.d0,0.d0,w(1,j1,5,1),fx)
                      else
c                        fx(1)=-w(5,j1,5,1)/rho0
c                        fx(2)=-w(6,j1,5,1)/rho0
c                        fx(3)=-alam*w(2,j1,5,1)
c                        fx(4)=-amu*w(1,j1,5,1)
c                        fx(5)= fx(4)
c                        fx(6)=-(alam+2*amu)*w(2,j1,5,1)
                        call smflx (m,1.d0,0.d0,1.d0,w(1,j1,5,1),fx)
                      end if
                      do i=1,6
                       up(j1,j2,i)=up(j1,j2,i)+da*fx(i)/det(j1,j2)
                      end do
                    end do
                  end if
                end do
c              else
c moving grid case (not implemented yet)
c              end if
            end if
          end if
c
c  ***********  should include mask in the displacement update  ***************
c
          if (method.eq.2) then
            call smdisp (m,dt,ds1,ds2,nd1a,nd1b,nd2a,nd2b,n1a,n1b,j2,
     *                   det,rx,u,md1a,md1b,w(1,md1a,5,1))
          end if
c
c..complete up (this should be done after each call to slope for the nonlinear case)
          do i=7,8
            im6=i-6
            do j1=n1a,n1b
              up(j1,j2,i)=w(im6,j1,5,1)
c              up(j1,j2,i)=u(j1,j2,im6)
            end do
          end do
c
c..twilight zone flow
          if (itz.ne.0) then
            t1=t
            if (iorder.eq.2) t1=t1+0.5d0*dt
            do j1=n1a,n1b
              call smGethtz (m,xy(j1,j2,1),xy(j1,j2,2),t1,htz)
              do i=1,8
                up(j1,j2,i)=up(j1,j2,i)+htz(i)
              end do
c              do i=7,8
c                up(j1,j2,i)=u(j1,j2,i-6)+.5d0*dt*htz(i-6)+htz(i)
c              end do
            end do
          end if
c
        end if
c
c..bottom of main loop over lines
      end do

c      write(6,*)'dt=',dt
c      do i=1,6
c        write(6,*)'errf =',i,errf(i)
c      end do
c      pause

c      write(6,*)'errw ='
c      do i=1,6
c        write(6,231)(errw(i,k),k=1,5)
c  231   format(5(1x,1pe9.2))
c      end do
c      pause
c
      return
      end
c
c++++++++++++++++++++++++++++
c
      subroutine smmetrics2d (nd1a,nd1b,md1a,md1b,nd2a,nd2b,j2,
     *                        rx,det,a1,aj)
c
      implicit real*8 (a-h,o-z)
      dimension rx(nd1a:nd1b,nd2a:nd2b,2,2),det(nd1a:nd1b,nd2a:nd2b),
     *          a1(2,2,md1a:md1b),aj(md1a:md1b)
      common / smgprm / method,iorder,ilimit,iupwind,icart,ifrc
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
      subroutine smslope2d (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
     *                      j2,ds1,ds2,dt,t,xy,a1,aj,mask,u,w,f1)
c
c slope correction
c
      implicit real*8 (a-h,o-z)
      dimension xy(nd1a:nd1b,nd2a:nd2b,2),a1(2,2,md1a:md1b),
     *          aj(md1a:md1b),mask(nd1a:nd1b),
     *          u(nd1a:nd1b,nd2a:nd2b,m),w(m,md1a:md1b,5),
     *          f1(nd1a:nd1b,nd2a:nd2b,2)
      dimension al(6),el(6,6),er(6,6),utz(8),htz(8)
c      dimension aa(6,6),bb(6,6)
      common / smgprm / method,iorder,ilimit,iupwind,icart,ifrc
      common / tzflow / eptz,itz
c      common / junk / dx,dy,nx,ny
c      data ilimit, iupwind / 0, 0 /   ! setting these values to 1,1 makes the errors smoother but bigger...
      data eps / 1.d-14 /    ! tolerance for zero eigenvalue
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
      if (iorder.eq.1) return
c
c      call getab (aa,bb)
c
      r1=.5d0*dt/ds1
      r2=.5d0*dt/ds2
c
      do j1=n1a-1,n1b+1
        if (mask(j1).ne.0) then
c
          if (itz.ne.0) then
            call smGethtz (m,xy(j1,j2,1),xy(j1,j2,2),t,htz)
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
c
c          n2a=0
c          x=(j1-n1a)*dx
c          y=(j2-n2a)*dy
c          call evaltz (x,y,t,utz,htz)
c          do k=1,5
c            do i=1,2
c              w(i,j1,k)=w(i,j1,k)+.5d0*dt*htz(i)
c            end do
c          end do
c
c exact slope corrections
c          do i=1,6
c            suma=0.d0
c            sumb=0.d0
c            do k=1,6
c              suma=suma+aa(i,k)*(u(j1+1,j2,k)-u(j1-1,j2,k))
c              sumb=sumb+bb(i,k)*(u(j1,j2+1,k)-u(j1,j2-1,k))
c            end do
c            tmp=w(i,j1,5)-.25d0*dt*(suma/dx+sumb/dy)
c            w(i,j1,1)=tmp-.25d0*(u(j1+1,j2,i)-u(j1-1,j2,i))
c            w(i,j1,2)=tmp+.25d0*(u(j1+1,j2,i)-u(j1-1,j2,i))
c            w(i,j1,3)=tmp-.25d0*(u(j1,j2+1,i)-u(j1,j2-1,i))
c            w(i,j1,4)=tmp+.25d0*(u(j1,j2+1,i)-u(j1,j2-1,i))
c            w(i,j1,5)=tmp
c          end do
c
c..s1 direction
          call smeig2d (a1(1,1,j1),a1(1,2,j1),al,el,er)
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
                alam=r1*al(j)
                tmp=alam*alp
                do i=1,6
                  tmp2=tmp*er(i,j)
                  w(i,j1,3)=w(i,j1,3)-tmp2
                  w(i,j1,4)=w(i,j1,4)-tmp2
                  w(i,j1,5)=w(i,j1,5)-tmp2
                  if (iupwind.ne.0) then
                    w(i,j1,1)=w(i,j1,1)
     *                         -(min(alam,0.d0)+.5d0)*alp*er(i,j)
                    w(i,j1,2)=w(i,j1,2)
     *                         -(max(alam,0.d0)-.5d0)*alp*er(i,j)
                  else
                    w(i,j1,1)=w(i,j1,1)-(alam+.5d0)*alp*er(i,j)
                    w(i,j1,2)=w(i,j1,2)-(alam-.5d0)*alp*er(i,j)
                  end if
                end do
              end if
c
            end if
          end do
c
c..s2 direction
          call smeig2d (a1(2,1,j1),a1(2,2,j1),al,el,er)
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
                alam=r2*al(j)
                tmp=alam*alp
                do i=1,6
                  tmp2=tmp*er(i,j)
                  w(i,j1,1)=w(i,j1,1)-tmp2
                  w(i,j1,2)=w(i,j1,2)-tmp2
                  w(i,j1,5)=w(i,j1,5)-tmp2
                  if (iupwind.ne.0) then
                    w(i,j1,3)=w(i,j1,3)
     *                         -(min(alam,0.d0)+.5d0)*alp*er(i,j)
                    w(i,j1,4)=w(i,j1,4)
     *                         -(max(alam,0.d0)-.5d0)*alp*er(i,j)
                  else
                    w(i,j1,3)=w(i,j1,3)-(alam+.5d0)*alp*er(i,j)
                    w(i,j1,4)=w(i,j1,4)-(alam-.5d0)*alp*er(i,j)
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
      return
      end
c
c++++++++++++++++++++
c
      subroutine getab (aa,bb)
c
      implicit real*8 (a-h,o-z)
      dimension aa(6,6),bb(6,6)
      common / smgdat / amu,alam,rho0
c
      do i=1,6
        do j=1,6
          aa(i,j)=0.d0
          bb(i,j)=0.d0
        end do
      end do
c
      aa(1,3)=-1.d0/rho0
      aa(2,4)=-1.d0/rho0
      aa(3,1)=-(alam+2*amu)
      aa(4,2)=-amu
      aa(5,2)=-amu
      aa(6,1)=-alam
c
      bb(1,5)=-1.d0/rho0
      bb(2,6)=-1.d0/rho0
      bb(3,2)=-alam
      bb(4,1)=-amu
      bb(5,1)=-amu
      bb(6,2)=-(alam+2*amu)
c
      return
      end
c
c++++++++++++++++++++
c
      subroutine smeig2d (a1,a2,al,el,er)
c
      implicit real*8 (a-h,o-z)
      dimension al(6),el(6,6),er(6,6)
      common / smgdat / amu,alam,rho0
c
c..directions
      rad=dsqrt(a1**2+a2**2)
      an1=a1/rad
      an2=a2/rad
      an11=an1*an1
      an12=an1*an2
      an22=an2*an2
c
c..wave speeds
      c1=dsqrt((alam+2*amu)/rho0)
      c2=dsqrt(amu/rho0)
c
c..eigenvalues
      al(1)=-rad*c1
      al(2)=-rad*c2
      al(3)= 0.d0
      al(4)= 0.d0
      al(5)=-al(2)
      al(6)=-al(1)
c
c..left eigenvector
      el(1,1)= .5d0*an1/c1
      el(1,2)= .5d0*an2/c1
      el(1,3)= .5d0*an11/(alam+2*amu)
      el(1,4)= .5d0*an12/(alam+2*amu)
      el(1,5)= el(1,4)
      el(1,6)= .5d0*an22/(alam+2*amu)
      el(2,1)=-.5d0*an2/c2
      el(2,2)= .5d0*an1/c2
      el(2,3)=-.5d0*an12/amu
      el(2,4)= .5d0*an11/amu
      el(2,5)=-.5d0*an22/amu
      el(2,6)=-el(2,3)
      el(3,1)= 0.d0
      el(3,2)= 0.d0
      el(3,3)=-an2*(((an22-an11)*alam+2*an22*amu)/(alam+2*amu))
      el(3,4)=-an1*(((an11-an22)*alam+2*an11*amu)/(alam+2*amu))
      el(3,5)= an1*(1.d0+2*an22*(alam+amu)/(alam+2*amu))
      el(3,6)=-an2*(((an11-an22)*alam+2*an11*amu)/(alam+2*amu))
      el(4,1)= 0.d0
      el(4,2)= 0.d0
      el(4,3)= an1*(((an22-an11)*alam+2*an22*amu)/(alam+2*amu))
      el(4,4)=-an2*(1.d0+2*an11*(alam+amu)/(alam+2*amu))
      el(4,5)= an2*(((an22-an11)*alam+2*an22*amu)/(alam+2*amu))
      el(4,6)= an1*(((an11-an22)*alam+2*an11*amu)/(alam+2*amu))
      el(5,1)=-el(2,1)
      el(5,2)=-el(2,2)
      el(5,3)= el(2,3)
      el(5,4)= el(2,4)
      el(5,5)= el(2,5)
      el(5,6)= el(2,6)
      el(6,1)=-el(1,1)
      el(6,2)=-el(1,2)
      el(6,3)= el(1,3)
      el(6,4)= el(1,4)
      el(6,5)= el(1,5)
      el(6,6)= el(1,6)
c
c..right eigenvector,ifrc
      er(1,1)= an1*c1
      er(2,1)= an2*c1
      er(3,1)= alam+2*an11*amu
      er(4,1)= 2*an12*amu
      er(5,1)= er(4,1)
      er(6,1)= alam+2*an22*amu
      er(1,2)=-an2*c2
      er(2,2)= an1*c2
      er(3,2)=-2*an12*amu
      er(4,2)= (an11-an22)*amu
      er(5,2)= er(4,2)
      er(6,2)=-er(3,2)
      er(1,3)= 0.d0
      er(2,3)= 0.d0
      er(3,3)=-an2
      er(4,3)= 0.d0
      er(5,3)= an1
      er(6,3)= 0.d0
      er(1,4)= 0.d0
      er(2,4)= 0.d0
      er(3,4)= 0.d0
      er(4,4)=-an2
      er(5,4)= 0.d0
      er(6,4)= an1
      er(1,5)=-er(1,2)
      er(2,5)=-er(2,2)
      er(3,5)= er(3,2)
      er(4,5)= er(4,2)
      er(5,5)= er(5,2)
      er(6,5)= er(6,2)
      er(1,6)=-er(1,1)
      er(2,6)=-er(2,1)
      er(3,6)= er(3,1)
      er(4,6)= er(4,1)
      er(5,6)= er(5,1)
      er(6,6)= er(6,1)
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
      subroutine smslope2dc (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
     *                       j2,ds1,ds2,dt,t,xy,mask,u,w,f1)
c
c slope correction   ***** optimized for Cartesian grids *****
c
      implicit real*8 (a-h,o-z)
      dimension xy(nd1a:nd1b,nd2a:nd2b,2),mask(nd1a:nd1b),
     *          u(nd1a:nd1b,nd2a:nd2b,m),w(m,md1a:md1b,5),
     *          f1(nd1a:nd1b,nd2a:nd2b,2)
      dimension w1(6),w2(6),w3(6),w4(6),w5(6),htz(8),
     *          du1m(6),du1p(6),du2m(6),du2p(6)
      common / smgprm / method,iorder,ilimit,iupwind,icart,ifrc
      common / smgdat / amu,alam,rho0
      common / tzflow / eptz,itz
c      data ilimit, iupwind / 0, 0 /   ! setting these values to 1,1 makes the errors smoother but bigger...
c
c      write(6,*)ilimit,iupwind
c      pause
c
      if (iorder.eq.1) then
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
      else
c
        r1=.5d0*dt/ds1
        r2=.5d0*dt/ds2
c
c..wave speeds
        c1=dsqrt((alam+2*amu)/rho0)
        c2=dsqrt(amu/rho0)
c
c..eigenvalue contributions
        eig11=-r1*c1
        eig12=-r1*c2
        eig15=-eig12
        eig16=-eig11
        eig21=-r2*c1
        eig22=-r2*c2
        eig25=-eig22
        eig26=-eig21
c
c..nonzero components of R\sp{-1}
        el11= .5d0/c1
        el12= el11
        el13= .5d0/(alam+2*amu)
        el16= el13
        el21=-.5d0/c2
        el22=-el21
        el24= .5d0/amu
        el25=-el24
c        el33=-1.d0
c        el34=-1.d0
c        el35= 1.d0
        el36= alam/(alam+2*amu)
        el43=-el36
c        el44=-1.d0
c        el45= 1.d0
c        el46= 1.d0
        el51=-el21
        el52= el21
        el54= el24
        el55=-el24
        el61=-el11
        el62=-el11
        el63= el13
        el66= el13
c
c..nonzero components of R
        er111= c1
        er131= alam+2*amu
        er161= alam
        er122= c2
        er142= amu
        er152= amu
        er125=-c2
        er145= amu
        er155= amu
        er116=-c1
        er136= alam+2*amu
        er166= alam
        er221= c1
        er231= alam
        er261= alam+2*amu
        er212=-c2
        er242=-amu
        er252=-amu
        er215= c2
        er245=-amu
        er255=-amu
        er226=-c1
        er236= alam
        er266= alam+2*amu
c
        do j1=n1a-1,n1b+1
          if (mask(j1).ne.0) then
c
            do i=1,6
              w1(i)=u(j1,j2,i)
              w2(i)=u(j1,j2,i)
              w3(i)=u(j1,j2,i)
              w4(i)=u(j1,j2,i)
              w5(i)=u(j1,j2,i)
            end do
c
            if (itz.eq.0) then
              if (ifrc.ne.0) then
                do i=1,2
                  w1(i)=w1(i)+.5d0*dt*f1(j1,j2,i)
                  w2(i)=w1(i)
                  w3(i)=w1(i)
                  w4(i)=w1(i)
                  w5(i)=w1(i)
                end do
              end if
            else
              call smGethtz (m,xy(j1,j2,1),xy(j1,j2,2),t,htz)
              do i=1,6
                w1(i)=w1(i)+.5d0*dt*htz(i)
                w2(i)=w1(i)
                w3(i)=w1(i)
                w4(i)=w1(i)
                w5(i)=w1(i)
              end do
            end if
c
            do i=1,6
              du1m(i)=u(j1,j2,i)-u(j1-1,j2,i)
              du1p(i)=u(j1+1,j2,i)-u(j1,j2,i)
              du2m(i)=u(j1,j2,i)-u(j1,j2-1,i)
              du2p(i)=u(j1,j2+1,i)-u(j1,j2,i)
            end do
c
c..s1 direction, j=1
c            el11= .5d0/c1
c            el13= .5d0/(alam+2*amu)
            alphal=el11*du1m(1)+el13*du1m(3)
            alphar=el11*du1p(1)+el13*du1p(3)
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
              tmp=eig11*alp
c
              tmp11=tmp*er111
              tmp31=tmp*er131
              tmp61=tmp*er161
c
              w3(1)=w3(1)-tmp11
              w4(1)=w4(1)-tmp11
              w5(1)=w5(1)-tmp11
              w3(3)=w3(3)-tmp31
              w4(3)=w4(3)-tmp31
              w5(3)=w5(3)-tmp31
              w3(6)=w3(6)-tmp61
              w4(6)=w4(6)-tmp61
              w5(6)=w5(6)-tmp61
c
              if (iupwind.ne.0) then
                w1(1)=w1(1)-(eig11+.5d0)*alp*er111
                w2(1)=w2(1)-(     -.5d0)*alp*er111
                w1(3)=w1(3)-(eig11+.5d0)*alp*er131
                w2(3)=w2(3)-(     -.5d0)*alp*er131
                w1(6)=w1(6)-(eig11+.5d0)*alp*er161
                w2(6)=w2(6)-(     -.5d0)*alp*er161
              else
                w1(1)=w1(1)-(eig11+.5d0)*alp*er111
                w2(1)=w2(1)-(eig11-.5d0)*alp*er111
                w1(3)=w1(3)-(eig11+.5d0)*alp*er131
                w2(3)=w2(3)-(eig11-.5d0)*alp*er131
                w1(6)=w1(6)-(eig11+.5d0)*alp*er161
                w2(6)=w2(6)-(eig11-.5d0)*alp*er161
              end if
            end if
c
c..s1 direction, j=2
c            el22= .5d0/c2
c            el24= .5d0/amu
            alphal=el22*du1m(2)+el24*du1m(4)
            alphar=el22*du1p(2)+el24*du1p(4)
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
              tmp=eig12*alp
c
              tmp22=tmp*er122
              tmp42=tmp*er142
              tmp52=tmp*er152
c
              w3(2)=w3(2)-tmp22
              w4(2)=w4(2)-tmp22
              w5(2)=w5(2)-tmp22
              w3(4)=w3(4)-tmp42
              w4(4)=w4(4)-tmp42
              w5(4)=w5(4)-tmp42
              w3(5)=w3(5)-tmp52
              w4(5)=w4(5)-tmp52
              w5(5)=w5(5)-tmp52
c
              if (iupwind.ne.0) then
                w1(2)=w1(2)-(eig12+.5d0)*alp*er122
                w2(2)=w2(2)-(     -.5d0)*alp*er122
                w1(4)=w1(4)-(eig12+.5d0)*alp*er142
                w2(4)=w2(4)-(     -.5d0)*alp*er142
                w1(5)=w1(5)-(eig12+.5d0)*alp*er152
                w2(5)=w2(5)-(     -.5d0)*alp*er152
              else
                w1(2)=w1(2)-(eig12+.5d0)*alp*er122
                w2(2)=w2(2)-(eig12-.5d0)*alp*er122
                w1(4)=w1(4)-(eig12+.5d0)*alp*er142
                w2(4)=w2(4)-(eig12-.5d0)*alp*er142
                w1(5)=w1(5)-(eig12+.5d0)*alp*er152
                w2(5)=w2(5)-(eig12-.5d0)*alp*er152
              end if
            end if
c
c..s1 direction, j=3
c            el34=-1.d0
c            el35= 1.d0
            alphal=-du1m(4)+du1m(5)
            alphar=-du1p(4)+du1p(5)
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
              w1(5)=w1(5)-.5d0*alp
              w2(5)=w2(5)+.5d0*alp
            end if
c
c..s1 direction, j=4
c            el43=-alam/(alam+2*amu)
c            el46= 1.d0
            alphal=el43*du1m(3)+du1m(6)
            alphar=el43*du1p(3)+du1p(6)
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
              w1(6)=w1(6)-.5d0*alp
              w2(6)=w2(6)+.5d0*alp
            end if
c
c..s1 direction, j=5
c            el52=-.5d0/c2
c            el54= .5d0/amu
            alphal=el52*du1m(2)+el54*du1m(4)
            alphar=el52*du1p(2)+el54*du1p(4)
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
              tmp=eig15*alp
c
              tmp25=tmp*er125
              tmp45=tmp*er145
              tmp55=tmp*er155

              w3(2)=w3(2)-tmp25
              w4(2)=w4(2)-tmp25
              w5(2)=w5(2)-tmp25
              w3(4)=w3(4)-tmp45
              w4(4)=w4(4)-tmp45
              w5(4)=w5(4)-tmp45
              w3(5)=w3(5)-tmp55
              w4(5)=w4(5)-tmp55
              w5(5)=w5(5)-tmp55
c
              if (iupwind.ne.0) then
                w1(2)=w1(2)-(      .5d0)*alp*er125
                w2(2)=w2(2)-(eig15-.5d0)*alp*er125
                w1(4)=w1(4)-(      .5d0)*alp*er145
                w2(4)=w2(4)-(eig15-.5d0)*alp*er145
                w1(5)=w1(5)-(      .5d0)*alp*er155
                w2(5)=w2(5)-(eig15-.5d0)*alp*er155
              else
                w1(2)=w1(2)-(eig15+.5d0)*alp*er125
                w2(2)=w2(2)-(eig15-.5d0)*alp*er125
                w1(4)=w1(4)-(eig15+.5d0)*alp*er145
                w2(4)=w2(4)-(eig15-.5d0)*alp*er145
                w1(5)=w1(5)-(eig15+.5d0)*alp*er155
                w2(5)=w2(5)-(eig15-.5d0)*alp*er155
              end if
            end if
c
c..s1 direction, j=6
c            el61=-.5d0/c1
c            el63= .5d0/(alam+2*amu)
            alphal=el61*du1m(1)+el63*du1m(3)
            alphar=el61*du1p(1)+el63*du1p(3)
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
              tmp=eig16*alp
c
              tmp16=tmp*er116
              tmp66=tmp*er166
              tmp36=tmp*er136
c
              w3(1)=w3(1)-tmp16
              w4(1)=w4(1)-tmp16
              w5(1)=w5(1)-tmp16
              w3(3)=w3(3)-tmp36
              w4(3)=w4(3)-tmp36
              w5(3)=w5(3)-tmp36
              w3(6)=w3(6)-tmp66
              w4(6)=w4(6)-tmp66
              w5(6)=w5(6)-tmp66
c
              if (iupwind.ne.0) then
                w1(1)=w1(1)-(      .5d0)*alp*er116
                w2(1)=w2(1)-(eig16-.5d0)*alp*er116
                w1(3)=w1(3)-(      .5d0)*alp*er136
                w2(3)=w2(3)-(eig16-.5d0)*alp*er136
                w1(6)=w1(6)-(      .5d0)*alp*er166
                w2(6)=w2(6)-(eig16-.5d0)*alp*er166
              else
                w1(1)=w1(1)-(eig16+.5d0)*alp*er116
                w2(1)=w2(1)-(eig16-.5d0)*alp*er116
                w1(3)=w1(3)-(eig16+.5d0)*alp*er136
                w2(3)=w2(3)-(eig16-.5d0)*alp*er136
                w1(6)=w1(6)-(eig16+.5d0)*alp*er166
                w2(6)=w2(6)-(eig16-.5d0)*alp*er166
              end if
            end if
c
c..s2 direction, j=1
c            el12= .5d0/c1
c            el16= .5d0/(alam+2*amu)
            alphal=el12*du2m(2)+el16*du2m(6)
            alphar=el12*du2p(2)+el16*du2p(6)
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
              tmp=eig21*alp
c
              tmp21=tmp*er221
              tmp31=tmp*er231
              tmp61=tmp*er261
c
              w1(2)=w1(2)-tmp21
              w2(2)=w2(2)-tmp21
              w5(2)=w5(2)-tmp21
              w1(3)=w1(3)-tmp31
              w2(3)=w2(3)-tmp31
              w5(3)=w5(3)-tmp31
              w1(6)=w1(6)-tmp61
              w2(6)=w2(6)-tmp61
              w5(6)=w5(6)-tmp61
c
              if (iupwind.ne.0) then
                w3(2)=w3(2)-(eig21+.5d0)*alp*er221
                w4(2)=w4(2)-(     -.5d0)*alp*er221
                w3(3)=w3(3)-(eig21+.5d0)*alp*er231
                w4(3)=w4(3)-(     -.5d0)*alp*er231
                w3(6)=w3(6)-(eig21+.5d0)*alp*er261
                w4(6)=w4(6)-(     -.5d0)*alp*er261
              else
                w3(2)=w3(2)-(eig21+.5d0)*alp*er221
                w4(2)=w4(2)-(eig21-.5d0)*alp*er221
                w3(3)=w3(3)-(eig21+.5d0)*alp*er231
                w4(3)=w4(3)-(eig21-.5d0)*alp*er231
                w3(6)=w3(6)-(eig21+.5d0)*alp*er261
                w4(6)=w4(6)-(eig21-.5d0)*alp*er261
              end if
            end if
c
c..s2 direction, j=2
c            el21=-.5d0/c2
c            el25=-.5d0/amu
            alphal=el21*du2m(1)+el25*du2m(5)
            alphar=el21*du2p(1)+el25*du2p(5)
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
              tmp=eig22*alp
c
              tmp12=tmp*er212
              tmp42=tmp*er242
              tmp52=tmp*er252
c
              w1(1)=w1(1)-tmp12
              w2(1)=w2(1)-tmp12
              w5(1)=w5(1)-tmp12
              w1(4)=w1(4)-tmp42
              w2(4)=w2(4)-tmp42
              w5(4)=w5(4)-tmp42
              w1(5)=w1(5)-tmp52
              w2(5)=w2(5)-tmp52
              w5(5)=w5(5)-tmp52
c
              if (iupwind.ne.0) then
                w3(1)=w3(1)-(eig22+.5d0)*alp*er212
                w4(1)=w4(1)-(     -.5d0)*alp*er212
                w3(4)=w3(4)-(eig22+.5d0)*alp*er242
                w4(4)=w4(4)-(     -.5d0)*alp*er242
                w3(5)=w3(5)-(eig22+.5d0)*alp*er252
                w4(5)=w4(5)-(     -.5d0)*alp*er252
              else
                w3(1)=w3(1)-(eig22+.5d0)*alp*er212
                w4(1)=w4(1)-(eig22-.5d0)*alp*er212
                w3(4)=w3(4)-(eig22+.5d0)*alp*er242
                w4(4)=w4(4)-(eig22-.5d0)*alp*er242
                w3(5)=w3(5)-(eig22+.5d0)*alp*er252
                w4(5)=w4(5)-(eig22-.5d0)*alp*er252
              end if
            end if
c
c..s2 direction, j=3
c            el33=-1.d0
c            el36= alam/(alam+2*amu)
            alphal=-du2m(3)+el36*du2m(6)
            alphar=-du2p(3)+el36*du2p(6)
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
              w3(3)=w3(3)+.5d0*alp
              w4(3)=w4(3)-.5d0*alp
            end if
c
c..s2 direction, j=4
c            el44=-1.d0
c            el45= 1.d0
            alphal=-du2m(4)+du2m(5)
            alphar=-du2p(4)+du2p(5)
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
              w3(4)=w3(4)+.5d0*alp
              w4(4)=w4(4)-.5d0*alp
            end if
c
c..s2 direction, j=5
c            el51= .5d0/c2
c            el55=-.5d0/amu
            alphal=el51*du2m(1)+el55*du2m(5)
            alphar=el51*du2p(1)+el55*du2p(5)
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
              tmp=eig25*alp
c
              tmp15=tmp*er215
              tmp45=tmp*er245
              tmp55=tmp*er255
c
              w1(1)=w1(1)-tmp15
              w2(1)=w2(1)-tmp15
              w5(1)=w5(1)-tmp15
              w1(4)=w1(4)-tmp45
              w2(4)=w2(4)-tmp45
              w5(4)=w5(4)-tmp45
              w1(5)=w1(5)-tmp55
              w2(5)=w2(5)-tmp55
              w5(5)=w5(5)-tmp55
c
              if (iupwind.ne.0) then
                w3(1)=w3(1)-(      .5d0)*alp*er215
                w4(1)=w4(1)-(eig25-.5d0)*alp*er215
                w3(4)=w3(4)-(      .5d0)*alp*er245
                w4(4)=w4(4)-(eig25-.5d0)*alp*er245
                w3(5)=w3(5)-(      .5d0)*alp*er255
                w4(5)=w4(5)-(eig25-.5d0)*alp*er255
              else
                w3(1)=w3(1)-(eig25+.5d0)*alp*er215
                w4(1)=w4(1)-(eig25-.5d0)*alp*er215
                w3(4)=w3(4)-(eig25+.5d0)*alp*er245
                w4(4)=w4(4)-(eig25-.5d0)*alp*er245
                w3(5)=w3(5)-(eig25+.5d0)*alp*er255
                w4(5)=w4(5)-(eig25-.5d0)*alp*er255
              end if
            end if
c
c..s2 direction, j=6
c            el62=-.5d0/c1
c            el66= .5d0/(alam+2*amu)
            alphal=el62*du2m(2)+el66*du2m(6)
            alphar=el62*du2p(2)+el66*du2p(6)
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
              tmp=eig26*alp
c
              tmp26=tmp*er226
              tmp36=tmp*er236
              tmp66=tmp*er266
c
              w1(2)=w1(2)-tmp26
              w2(2)=w2(2)-tmp26
              w5(2)=w5(2)-tmp26
              w1(3)=w1(3)-tmp36
              w2(3)=w2(3)-tmp36
              w5(3)=w5(3)-tmp36
              w1(6)=w1(6)-tmp66
              w2(6)=w2(6)-tmp66
              w5(6)=w5(6)-tmp66
c
              if (iupwind.ne.0) then
                w3(2)=w3(2)-(      .5d0)*alp*er226
                w4(2)=w4(2)-(eig26-.5d0)*alp*er226
                w3(3)=w3(3)-(      .5d0)*alp*er236
                w4(3)=w4(3)-(eig26-.5d0)*alp*er236
                w3(6)=w3(6)-(      .5d0)*alp*er266
                w4(6)=w4(6)-(eig26-.5d0)*alp*er266
              else
                w3(2)=w3(2)-(eig26+.5d0)*alp*er226
                w4(2)=w4(2)-(eig26-.5d0)*alp*er226
                w3(3)=w3(3)-(eig26+.5d0)*alp*er236
                w4(3)=w4(3)-(eig26-.5d0)*alp*er236
                w3(6)=w3(6)-(eig26+.5d0)*alp*er266
                w4(6)=w4(6)-(eig26-.5d0)*alp*er266
              end if
            end if
c
            do i=1,6
              w(i,j1,1)=w1(i)
              w(i,j1,2)=w2(i)
              w(i,j1,3)=w3(i)
              w(i,j1,4)=w4(i)
              w(i,j1,5)=w5(i)
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
c++++++++++++++++++++
c
      subroutine smslope2dc0 (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
     *                       j2,ds1,ds2,dt,t,xy,a1,aj,mask,u,w,f1)
c
c slope correction   ***** optimized for Cartesian grids *****
c
      implicit real*8 (a-h,o-z)
      dimension xy(nd1a:nd1b,nd2a:nd2b,2),a1(2,2,md1a:md1b),
     *          aj(md1a:md1b),mask(nd1a:nd1b),
     *          u(nd1a:nd1b,nd2a:nd2b,m),w(m,md1a:md1b,5),
     *          f1(nd1a:nd1b,nd2a:nd2b,2)
      dimension htz(8)
      common / smgprm / method,iorder,ilimit,iupwind,icart,ifrc
      common / smgdat / amu,alam,rho0
      common / tzflow / eptz,itz
c      data ilimit, iupwind / 0, 0 /   ! setting these values to 1,1 makes the errors smoother but bigger...
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
      if (iorder.eq.1) return
c
      r1=.5d0*dt/ds1
      r2=.5d0*dt/ds2
c
c..wave speeds
      c1=dsqrt((alam+2*amu)/rho0)
      c2=dsqrt(amu/rho0)
c
      do j1=n1a-1,n1b+1
        if (mask(j1).ne.0) then
c
          if (itz.ne.0) then
            call smGethtz (m,xy(j1,j2,1),xy(j1,j2,2),t,htz)
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
c..s1 direction, j=1
          el11= .5d0/c1
          el13= .5d0/(alam+2*amu)
          alphal= el11*(u(j1,j2,1)-u(j1-1,j2,1))
     *           +el13*(u(j1,j2,3)-u(j1-1,j2,3))
          alphar= el11*(u(j1+1,j2,1)-u(j1,j2,1))
     *           +el13*(u(j1+1,j2,3)-u(j1,j2,3))
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
            eig=-r1*c1
            tmp=eig*alp
c
            i=1
            er11= c1
            tmp2=tmp*er11
            w(i,j1,3)=w(i,j1,3)-tmp2
            w(i,j1,4)=w(i,j1,4)-tmp2
            w(i,j1,5)=w(i,j1,5)-tmp2
            if (iupwind.ne.0) then
              w(i,j1,1)=w(i,j1,1)-(min(eig,0.d0)+.5d0)*alp*er11
              w(i,j1,2)=w(i,j1,2)-(max(eig,0.d0)-.5d0)*alp*er11
            else
              w(i,j1,1)=w(i,j1,1)-(eig+.5d0)*alp*er11
              w(i,j1,2)=w(i,j1,2)-(eig-.5d0)*alp*er11
            end if
c
            i=3
            er31=alam+2*amu
            tmp2=tmp*er31
            w(i,j1,3)=w(i,j1,3)-tmp2
            w(i,j1,4)=w(i,j1,4)-tmp2
            w(i,j1,5)=w(i,j1,5)-tmp2
            if (iupwind.ne.0) then
              w(i,j1,1)=w(i,j1,1)-(min(eig,0.d0)+.5d0)*alp*er31
              w(i,j1,2)=w(i,j1,2)-(max(eig,0.d0)-.5d0)*alp*er31
            else
              w(i,j1,1)=w(i,j1,1)-(eig+.5d0)*alp*er31
              w(i,j1,2)=w(i,j1,2)-(eig-.5d0)*alp*er31
            end if
c
            i=6
            er61=alam
            tmp2=tmp*er61
            w(i,j1,3)=w(i,j1,3)-tmp2
            w(i,j1,4)=w(i,j1,4)-tmp2
            w(i,j1,5)=w(i,j1,5)-tmp2
            if (iupwind.ne.0) then
              w(i,j1,1)=w(i,j1,1)-(min(eig,0.d0)+.5d0)*alp*er61
              w(i,j1,2)=w(i,j1,2)-(max(eig,0.d0)-.5d0)*alp*er61
            else
              w(i,j1,1)=w(i,j1,1)-(eig+.5d0)*alp*er61
              w(i,j1,2)=w(i,j1,2)-(eig-.5d0)*alp*er61
            end if
          end if
c
c..s1 direction, j=2
          el22= .5d0/c2
          el24= .5d0/amu
          alphal= el22*(u(j1,j2,2)-u(j1-1,j2,2))
     *           +el24*(u(j1,j2,4)-u(j1-1,j2,4))
          alphar= el22*(u(j1+1,j2,2)-u(j1,j2,2))
     *           +el24*(u(j1+1,j2,4)-u(j1,j2,4))
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
            eig=-r1*c2
            tmp=eig*alp
c
            i=2
            er22= c2
            tmp2=tmp*er22
            w(i,j1,3)=w(i,j1,3)-tmp2
            w(i,j1,4)=w(i,j1,4)-tmp2
            w(i,j1,5)=w(i,j1,5)-tmp2
            if (iupwind.ne.0) then
              w(i,j1,1)=w(i,j1,1)-(min(eig,0.d0)+.5d0)*alp*er22
              w(i,j1,2)=w(i,j1,2)-(max(eig,0.d0)-.5d0)*alp*er22
            else
              w(i,j1,1)=w(i,j1,1)-(eig+.5d0)*alp*er22
              w(i,j1,2)=w(i,j1,2)-(eig-.5d0)*alp*er22
            end if
c
            i=4
            er42=amu
            tmp2=tmp*er42
            w(i,j1,3)=w(i,j1,3)-tmp2
            w(i,j1,4)=w(i,j1,4)-tmp2
            w(i,j1,5)=w(i,j1,5)-tmp2
            if (iupwind.ne.0) then
              w(i,j1,1)=w(i,j1,1)-(min(eig,0.d0)+.5d0)*alp*er42
              w(i,j1,2)=w(i,j1,2)-(max(eig,0.d0)-.5d0)*alp*er42
            else
              w(i,j1,1)=w(i,j1,1)-(eig+.5d0)*alp*er42
              w(i,j1,2)=w(i,j1,2)-(eig-.5d0)*alp*er42
            end if
c
            i=5
            er52=amu
            tmp2=tmp*er52
            w(i,j1,3)=w(i,j1,3)-tmp2
            w(i,j1,4)=w(i,j1,4)-tmp2
            w(i,j1,5)=w(i,j1,5)-tmp2
            if (iupwind.ne.0) then
              w(i,j1,1)=w(i,j1,1)-(min(eig,0.d0)+.5d0)*alp*er52
              w(i,j1,2)=w(i,j1,2)-(max(eig,0.d0)-.5d0)*alp*er52
            else
              w(i,j1,1)=w(i,j1,1)-(eig+.5d0)*alp*er52
              w(i,j1,2)=w(i,j1,2)-(eig-.5d0)*alp*er52
            end if
          end if
c
c..s1 direction, j=3
          el34=-1.d0
          el35= 1.d0
          alphal=-(u(j1,j2,4)-u(j1-1,j2,4))
     *           +(u(j1,j2,5)-u(j1-1,j2,5))
          alphar=-(u(j1+1,j2,4)-u(j1,j2,4))
     *           +(u(j1+1,j2,5)-u(j1,j2,5))
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
            i=5
            er53=1.d0
            w(i,j1,1)=w(i,j1,1)-.5d0*alp
            w(i,j1,2)=w(i,j1,2)+.5d0*alp
          end if
c
c..s1 direction, j=4
          el43=-alam/(alam+2*amu)
          el46= 1.d0
          alphal=el43*(u(j1,j2,3)-u(j1-1,j2,3))
     *               +(u(j1,j2,6)-u(j1-1,j2,6))
          alphar=el43*(u(j1+1,j2,3)-u(j1,j2,3))
     *               +(u(j1+1,j2,6)-u(j1,j2,6))
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
            i=6
            er64=1.d0
            w(i,j1,1)=w(i,j1,1)-.5d0*alp
            w(i,j1,2)=w(i,j1,2)+.5d0*alp
          end if
c
c..s1 direction, j=5
          el52=-.5d0/c2
          el54= .5d0/amu
          alphal= el52*(u(j1,j2,2)-u(j1-1,j2,2))
     *           +el54*(u(j1,j2,4)-u(j1-1,j2,4))
          alphar= el52*(u(j1+1,j2,2)-u(j1,j2,2))
     *           +el54*(u(j1+1,j2,4)-u(j1,j2,4))
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
            eig= r1*c2
            tmp=eig*alp
c
            i=2
            er25=-c2
            tmp2=tmp*er25
            w(i,j1,3)=w(i,j1,3)-tmp2
            w(i,j1,4)=w(i,j1,4)-tmp2
            w(i,j1,5)=w(i,j1,5)-tmp2
            if (iupwind.ne.0) then
              w(i,j1,1)=w(i,j1,1)-(min(eig,0.d0)+.5d0)*alp*er25
              w(i,j1,2)=w(i,j1,2)-(max(eig,0.d0)-.5d0)*alp*er25
            else
              w(i,j1,1)=w(i,j1,1)-(eig+.5d0)*alp*er25
              w(i,j1,2)=w(i,j1,2)-(eig-.5d0)*alp*er25
            end if
c
            i=4
            er45=amu
            tmp2=tmp*er45
            w(i,j1,3)=w(i,j1,3)-tmp2
            w(i,j1,4)=w(i,j1,4)-tmp2
            w(i,j1,5)=w(i,j1,5)-tmp2
            if (iupwind.ne.0) then
              w(i,j1,1)=w(i,j1,1)-(min(eig,0.d0)+.5d0)*alp*er45
              w(i,j1,2)=w(i,j1,2)-(max(eig,0.d0)-.5d0)*alp*er45
            else
              w(i,j1,1)=w(i,j1,1)-(eig+.5d0)*alp*er45
              w(i,j1,2)=w(i,j1,2)-(eig-.5d0)*alp*er45
            end if
c
            i=5
            er55=amu
            tmp2=tmp*er55
            w(i,j1,3)=w(i,j1,3)-tmp2
            w(i,j1,4)=w(i,j1,4)-tmp2
            w(i,j1,5)=w(i,j1,5)-tmp2
            if (iupwind.ne.0) then
              w(i,j1,1)=w(i,j1,1)-(min(eig,0.d0)+.5d0)*alp*er55
              w(i,j1,2)=w(i,j1,2)-(max(eig,0.d0)-.5d0)*alp*er55
            else
              w(i,j1,1)=w(i,j1,1)-(eig+.5d0)*alp*er55
              w(i,j1,2)=w(i,j1,2)-(eig-.5d0)*alp*er55
            end if
          end if
c
c..s1 direction, j=6
          el61=-.5d0/c1
          el63= .5d0/(alam+2*amu)
          alphal= el61*(u(j1,j2,1)-u(j1-1,j2,1))
     *           +el63*(u(j1,j2,3)-u(j1-1,j2,3))
          alphar= el61*(u(j1+1,j2,1)-u(j1,j2,1))
     *           +el63*(u(j1+1,j2,3)-u(j1,j2,3))
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
            eig= r1*c1
            tmp=eig*alp
c
            i=1
            er16=-c1
            tmp2=tmp*er16
            w(i,j1,3)=w(i,j1,3)-tmp2
            w(i,j1,4)=w(i,j1,4)-tmp2
            w(i,j1,5)=w(i,j1,5)-tmp2
            if (iupwind.ne.0) then
              w(i,j1,1)=w(i,j1,1)-(min(eig,0.d0)+.5d0)*alp*er16
              w(i,j1,2)=w(i,j1,2)-(max(eig,0.d0)-.5d0)*alp*er16
            else
              w(i,j1,1)=w(i,j1,1)-(eig+.5d0)*alp*er16
              w(i,j1,2)=w(i,j1,2)-(eig-.5d0)*alp*er16
            end if
c
            i=3
            er36=alam+2*amu
            tmp2=tmp*er36
            w(i,j1,3)=w(i,j1,3)-tmp2
            w(i,j1,4)=w(i,j1,4)-tmp2
            w(i,j1,5)=w(i,j1,5)-tmp2
            if (iupwind.ne.0) then
              w(i,j1,1)=w(i,j1,1)-(min(eig,0.d0)+.5d0)*alp*er36
              w(i,j1,2)=w(i,j1,2)-(max(eig,0.d0)-.5d0)*alp*er36
            else
              w(i,j1,1)=w(i,j1,1)-(eig+.5d0)*alp*er36
              w(i,j1,2)=w(i,j1,2)-(eig-.5d0)*alp*er36
            end if
c
            i=6
            er66=alam
            tmp2=tmp*er66
            w(i,j1,3)=w(i,j1,3)-tmp2
            w(i,j1,4)=w(i,j1,4)-tmp2
            w(i,j1,5)=w(i,j1,5)-tmp2
            if (iupwind.ne.0) then
              w(i,j1,1)=w(i,j1,1)-(min(eig,0.d0)+.5d0)*alp*er66
              w(i,j1,2)=w(i,j1,2)-(max(eig,0.d0)-.5d0)*alp*er66
            else
              w(i,j1,1)=w(i,j1,1)-(eig+.5d0)*alp*er66
              w(i,j1,2)=w(i,j1,2)-(eig-.5d0)*alp*er66
            end if
          end if
c
c..s2 direction, j=1
          el12= .5d0/c1
          el16= .5d0/(alam+2*amu)
          alphal= el12*(u(j1,j2,2)-u(j1,j2-1,2))
     *           +el16*(u(j1,j2,6)-u(j1,j2-1,6))
          alphar= el12*(u(j1,j2+1,2)-u(j1,j2,2))
     *           +el16*(u(j1,j2+1,6)-u(j1,j2,6))
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
            eig=-r2*c1
            tmp=eig*alp
c
            i=2
            er21= c1
            tmp2=tmp*er21
            w(i,j1,1)=w(i,j1,1)-tmp2
            w(i,j1,2)=w(i,j1,2)-tmp2
            w(i,j1,5)=w(i,j1,5)-tmp2
            if (iupwind.ne.0) then
              w(i,j1,3)=w(i,j1,3)-(min(eig,0.d0)+.5d0)*alp*er21
              w(i,j1,4)=w(i,j1,4)-(max(eig,0.d0)-.5d0)*alp*er21
            else
              w(i,j1,3)=w(i,j1,3)-(eig+.5d0)*alp*er21
              w(i,j1,4)=w(i,j1,4)-(eig-.5d0)*alp*er21
            end if
c
            i=3
            er31= alam
            tmp2=tmp*er31
            w(i,j1,1)=w(i,j1,1)-tmp2
            w(i,j1,2)=w(i,j1,2)-tmp2
            w(i,j1,5)=w(i,j1,5)-tmp2
            if (iupwind.ne.0) then
              w(i,j1,3)=w(i,j1,3)-(min(eig,0.d0)+.5d0)*alp*er31
              w(i,j1,4)=w(i,j1,4)-(max(eig,0.d0)-.5d0)*alp*er31
            else
              w(i,j1,3)=w(i,j1,3)-(eig+.5d0)*alp*er31
              w(i,j1,4)=w(i,j1,4)-(eig-.5d0)*alp*er31
            end if
c
            i=6
            er61= alam+2*amu
            tmp2=tmp*er61
            w(i,j1,1)=w(i,j1,1)-tmp2
            w(i,j1,2)=w(i,j1,2)-tmp2
            w(i,j1,5)=w(i,j1,5)-tmp2
            if (iupwind.ne.0) then
              w(i,j1,3)=w(i,j1,3)-(min(eig,0.d0)+.5d0)*alp*er61
              w(i,j1,4)=w(i,j1,4)-(max(eig,0.d0)-.5d0)*alp*er61
            else
              w(i,j1,3)=w(i,j1,3)-(eig+.5d0)*alp*er61
              w(i,j1,4)=w(i,j1,4)-(eig-.5d0)*alp*er61
            end if
          end if
c
c..s2 direction, j=2
          el21=-.5d0/c2
          el25=-.5d0/amu
          alphal= el21*(u(j1,j2,1)-u(j1,j2-1,1))
     *           +el25*(u(j1,j2,5)-u(j1,j2-1,5))
          alphar= el21*(u(j1,j2+1,1)-u(j1,j2,1))
     *           +el25*(u(j1,j2+1,5)-u(j1,j2,5))
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
            eig=-r2*c2
            tmp=eig*alp
c
            i=1
            er12=-c2
            tmp2=tmp*er12
            w(i,j1,1)=w(i,j1,1)-tmp2
            w(i,j1,2)=w(i,j1,2)-tmp2
            w(i,j1,5)=w(i,j1,5)-tmp2
            if (iupwind.ne.0) then
              w(i,j1,3)=w(i,j1,3)-(min(eig,0.d0)+.5d0)*alp*er12
              w(i,j1,4)=w(i,j1,4)-(max(eig,0.d0)-.5d0)*alp*er12
            else
              w(i,j1,3)=w(i,j1,3)-(eig+.5d0)*alp*er12
              w(i,j1,4)=w(i,j1,4)-(eig-.5d0)*alp*er12
            end if
c
            i=4
            er42=-amu
            tmp2=tmp*er42
            w(i,j1,1)=w(i,j1,1)-tmp2
            w(i,j1,2)=w(i,j1,2)-tmp2
            w(i,j1,5)=w(i,j1,5)-tmp2
            if (iupwind.ne.0) then
              w(i,j1,3)=w(i,j1,3)-(min(eig,0.d0)+.5d0)*alp*er42
              w(i,j1,4)=w(i,j1,4)-(max(eig,0.d0)-.5d0)*alp*er42
            else
              w(i,j1,3)=w(i,j1,3)-(eig+.5d0)*alp*er42
              w(i,j1,4)=w(i,j1,4)-(eig-.5d0)*alp*er42
            end if
c
            i=5
            er52=-amu
            tmp2=tmp*er52
            w(i,j1,1)=w(i,j1,1)-tmp2
            w(i,j1,2)=w(i,j1,2)-tmp2
            w(i,j1,5)=w(i,j1,5)-tmp2
            if (iupwind.ne.0) then
              w(i,j1,3)=w(i,j1,3)-(min(eig,0.d0)+.5d0)*alp*er52
              w(i,j1,4)=w(i,j1,4)-(max(eig,0.d0)-.5d0)*alp*er52
            else
              w(i,j1,3)=w(i,j1,3)-(eig+.5d0)*alp*er52
              w(i,j1,4)=w(i,j1,4)-(eig-.5d0)*alp*er52
            end if
          end if
c
c..s2 direction, j=3
          el33=-1.d0
          el36= alam/(alam+2*amu)
          alphal=     -(u(j1,j2,3)-u(j1,j2-1,3))
     *           +el36*(u(j1,j2,6)-u(j1,j2-1,6))
          alphar=     -(u(j1,j2+1,3)-u(j1,j2,3))
     *           +el36*(u(j1,j2+1,6)-u(j1,j2,6))
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
            i=3
            er33=-1.d0
            w(i,j1,3)=w(i,j1,3)+.5d0*alp
            w(i,j1,4)=w(i,j1,4)-.5d0*alp
          end if
c
c..s2 direction, j=4
          el44=-1.d0
          el45= 1.d0
          alphal=-(u(j1,j2,4)-u(j1,j2-1,4))
     *           +(u(j1,j2,5)-u(j1,j2-1,5))
          alphar=-(u(j1,j2+1,4)-u(j1,j2,4))
     *           +(u(j1,j2+1,5)-u(j1,j2,5))
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
            i=4
            er44=-1.d0
            w(i,j1,3)=w(i,j1,3)+.5d0*alp
            w(i,j1,4)=w(i,j1,4)-.5d0*alp
          end if
c
c..s2 direction, j=5
          el51= .5d0/c2
          el55=-.5d0/amu
          alphal= el51*(u(j1,j2,1)-u(j1,j2-1,1))
     *           +el55*(u(j1,j2,5)-u(j1,j2-1,5))
          alphar= el51*(u(j1,j2+1,1)-u(j1,j2,1))
     *           +el55*(u(j1,j2+1,5)-u(j1,j2,5))
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
            eig= r2*c2
            tmp=eig*alp
c
            i=1
            er15= c2
            tmp2=tmp*er15
            w(i,j1,1)=w(i,j1,1)-tmp2
            w(i,j1,2)=w(i,j1,2)-tmp2
            w(i,j1,5)=w(i,j1,5)-tmp2
            if (iupwind.ne.0) then
              w(i,j1,3)=w(i,j1,3)-(min(eig,0.d0)+.5d0)*alp*er15
              w(i,j1,4)=w(i,j1,4)-(max(eig,0.d0)-.5d0)*alp*er15
            else
              w(i,j1,3)=w(i,j1,3)-(eig+.5d0)*alp*er15
              w(i,j1,4)=w(i,j1,4)-(eig-.5d0)*alp*er15
            end if
c
            i=4
            er45=-amu
            tmp2=tmp*er45
            w(i,j1,1)=w(i,j1,1)-tmp2
            w(i,j1,2)=w(i,j1,2)-tmp2
            w(i,j1,5)=w(i,j1,5)-tmp2
            if (iupwind.ne.0) then
              w(i,j1,3)=w(i,j1,3)-(min(eig,0.d0)+.5d0)*alp*er45
              w(i,j1,4)=w(i,j1,4)-(max(eig,0.d0)-.5d0)*alp*er45
            else
              w(i,j1,3)=w(i,j1,3)-(eig+.5d0)*alp*er45
              w(i,j1,4)=w(i,j1,4)-(eig-.5d0)*alp*er45
            end if
c
            i=5
            er55=-amu
            tmp2=tmp*er55
            w(i,j1,1)=w(i,j1,1)-tmp2
            w(i,j1,2)=w(i,j1,2)-tmp2
            w(i,j1,5)=w(i,j1,5)-tmp2
            if (iupwind.ne.0) then
              w(i,j1,3)=w(i,j1,3)-(min(eig,0.d0)+.5d0)*alp*er55
              w(i,j1,4)=w(i,j1,4)-(max(eig,0.d0)-.5d0)*alp*er55
            else
              w(i,j1,3)=w(i,j1,3)-(eig+.5d0)*alp*er55
              w(i,j1,4)=w(i,j1,4)-(eig-.5d0)*alp*er55
            end if
          end if
c
c..s2 direction, j=6
          el62=-.5d0/c1
          el66= .5d0/(alam+2*amu)
          alphal= el62*(u(j1,j2,2)-u(j1,j2-1,2))
     *           +el66*(u(j1,j2,6)-u(j1,j2-1,6))
          alphar= el62*(u(j1,j2+1,2)-u(j1,j2,2))
     *           +el66*(u(j1,j2+1,6)-u(j1,j2,6))
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
            eig= r2*c1
            tmp=eig*alp
c
            i=2
            er26=-c1
            tmp2=tmp*er26
            w(i,j1,1)=w(i,j1,1)-tmp2
            w(i,j1,2)=w(i,j1,2)-tmp2
            w(i,j1,5)=w(i,j1,5)-tmp2
            if (iupwind.ne.0) then
              w(i,j1,3)=w(i,j1,3)-(min(eig,0.d0)+.5d0)*alp*er26
              w(i,j1,4)=w(i,j1,4)-(max(eig,0.d0)-.5d0)*alp*er26
            else
              w(i,j1,3)=w(i,j1,3)-(eig+.5d0)*alp*er26
              w(i,j1,4)=w(i,j1,4)-(eig-.5d0)*alp*er26
            end if
c
            i=3
            er36= alam
            tmp2=tmp*er36
            w(i,j1,1)=w(i,j1,1)-tmp2
            w(i,j1,2)=w(i,j1,2)-tmp2
            w(i,j1,5)=w(i,j1,5)-tmp2
            if (iupwind.ne.0) then
              w(i,j1,3)=w(i,j1,3)-(min(eig,0.d0)+.5d0)*alp*er36
              w(i,j1,4)=w(i,j1,4)-(max(eig,0.d0)-.5d0)*alp*er36
            else
              w(i,j1,3)=w(i,j1,3)-(eig+.5d0)*alp*er36
              w(i,j1,4)=w(i,j1,4)-(eig-.5d0)*alp*er36
            end if
c
            i=6
            er66= alam+2*amu
            tmp2=tmp*er66
            w(i,j1,1)=w(i,j1,1)-tmp2
            w(i,j1,2)=w(i,j1,2)-tmp2
            w(i,j1,5)=w(i,j1,5)-tmp2
            if (iupwind.ne.0) then
              w(i,j1,3)=w(i,j1,3)-(min(eig,0.d0)+.5d0)*alp*er66
              w(i,j1,4)=w(i,j1,4)-(max(eig,0.d0)-.5d0)*alp*er66
            else
              w(i,j1,3)=w(i,j1,3)-(eig+.5d0)*alp*er66
              w(i,j1,4)=w(i,j1,4)-(eig-.5d0)*alp*er66
            end if
          end if
c
        end if
      end do
c
      return
      end
c
c++++++++++++++++++++
c
      subroutine smeig2d1 (a1,a2,al,el,er)
c
      implicit real*8 (a-h,o-z)
      dimension al(6),el(6,6),er(6,6)
      common / smgdat / amu,alam,rho0
c
c..directions
      rad=dsqrt(a1**2+a2**2)    ! 1
      an1=a1/rad                ! 1
      an2=a2/rad                ! 0
      an11=an1*an1              ! 1
      an12=an1*an2              ! 0
      an22=an2*an2              ! 0
c
c..wave speeds
      c1=dsqrt((alam+2*amu)/rho0)
      c2=dsqrt(amu/rho0)
c
c..eigenvalues
      al(1)=-c1
      al(2)=-c2
      al(3)= 0.d0
      al(4)= 0.d0
      al(5)=-al(2)
      al(6)=-al(1)
c
      do i=1,6
      do j=1,6
        el(i,j)=0.d0
        er(i,j)=0.d0
      end do
      end do
c
c..left eigenvector
      el(1,1)= .5d0/c1
      el(1,3)= .5d0/(alam+2*amu)
      el(2,2)= .5d0/c2
      el(2,4)= .5d0/amu
      el(3,4)=-1.d0
      el(3,5)= 1.d0
      el(4,3)=-alam/(alam+2*amu)
      el(4,6)= 1.d0
      el(5,2)=-.5d0/c2
      el(5,4)= .5d0/amu
      el(6,1)=-.5d0/c1
      el(6,3)= .5d0/(alam+2*amu)
c
c..right eigenvector
      er(1,1)= c1
      er(3,1)= alam+2*amu
      er(6,1)= alam
      er(2,2)= c2
      er(4,2)= amu
      er(5,2)= amu
      er(5,3)= 1.d0
      er(6,4)= 1.d0
      er(2,5)=-c2
      er(4,5)= amu
      er(5,5)= amu
      er(1,6)=-c1
      er(3,6)= alam+2*amu
      er(6,6)= alam
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
      subroutine smeig2d2 (a1,a2,al,el,er)
c
      implicit real*8 (a-h,o-z)
      dimension al(6),el(6,6),er(6,6)
      common / smgdat / amu,alam,rho0
c
c..directions
      rad=dsqrt(a1**2+a2**2)     ! 1
      an1=a1/rad                 ! 0
      an2=a2/rad                 ! 1
      an11=an1*an1               ! 0
      an12=an1*an2               ! 0
      an22=an2*an2               ! 1
c
c..wave speeds
      c1=dsqrt((alam+2*amu)/rho0)
      c2=dsqrt(amu/rho0)
c
c..eigenvalues
      al(1)=-c1
      al(2)=-c2
      al(3)= 0.d0
      al(4)= 0.d0
      al(5)=-al(2)
      al(6)=-al(1)
c
      do i=1,6
      do j=1,6
        el(i,j)=0.d0
        er(i,j)=0.d0
      end do
      end do
c
c..left eigenvector
      el(1,2)= .5d0/c1
      el(1,6)= .5d0/(alam+2*amu)
      el(2,1)=-.5d0/c2
      el(2,5)=-.5d0/amu
      el(3,3)=-1.d0
      el(3,6)= alam/(alam+2*amu)
      el(4,4)=-1.d0
      el(4,5)= 1.d0
      el(5,1)= .5d0/c2
      el(5,5)=-.5d0/amu
      el(6,2)=-.5d0/c1
      el(6,6)= .5d0/(alam+2*amu)
c
c..right eigenvector
      er(2,1)= c1
      er(3,1)= alam
      er(6,1)= alam+2*amu
      er(1,2)=-c2
      er(4,2)=-amu
      er(5,2)=-amu
      er(3,3)=-1.d0
      er(4,4)=-1.d0
      er(1,5)= c2
      er(4,5)=-amu
      er(5,5)=-amu
      er(2,6)=-c1
      er(3,6)= alam
      er(6,6)= alam+2*amu
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
      subroutine smflux2d (m,aj,a1,a2,wl,wr,fx,speed)
c
c Godunov flux (method=0)
c
      implicit real*8 (a-h,o-z)
      dimension wl(m),wr(m),fx(6)
      dimension an(2),el(2,6),er(6,2),al(2),alpha(2)
      common / smgdat / amu,alam,rho0
      common / smgprm / method,iorder,ilimit,iupwind,icart,ifrc
c
c normalize metrics of the mapping
      rad=dsqrt(a1**2+a2**2)
      an(1)=a1/rad
      an(2)=a2/rad
c
c wave speeds
      c1=dsqrt((alam+2*amu)/rho0)
      c2=dsqrt(amu/rho0)
c
c first two left eigenvectors
      el(1,1)= .5d0*an(1)/c1
      el(1,2)= .5d0*an(2)/c1
      el(1,3)= .5d0*an(1)**2/(alam+2*amu)
      el(1,4)= .5d0*an(1)*an(2)/(alam+2*amu)
      el(1,5)= el(1,4)
      el(1,6)= .5d0*an(2)**2/(alam+2*amu)
      el(2,1)=-.5d0*an(2)/c2
      el(2,2)= .5d0*an(1)/c2
      el(2,3)=-.5d0*an(1)*an(2)/amu
      el(2,4)= .5d0*an(1)**2/amu
      el(2,5)=-.5d0*an(2)**2/amu
      el(2,6)=-el(2,3)
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
      call smflx (m,aj,a1,a2,wl,fx)
c      fx(1)=aj*(-a1*wl(3)-a2*wl(5))/rho0
c      fx(2)=aj*(-a1*wl(4)-a2*wl(6))/rho0
c      fx(3)=aj*(-a1*(alam+2*amu)*wl(1)-a2*alam*wl(2))
c      fx(4)=aj*(-amu*(a1*wl(2)+a2*wl(1)))
c      fx(5)=fx(4)
c      fx(6)=aj*(-a1*alam*wl(1)-a2*(alam+2*amu)*wl(2))
c
c first two eigenvalues
      al(1)=-rad*c1
      al(2)=-rad*c2
c
c first two right eigenvectors
      er(1,1)= an(1)*c1
      er(2,1)= an(2)*c1
c      er(2,1)= an(2)*c2      correction from Jeff
      er(3,1)= alam+2*amu*an(1)**2
      er(4,1)= 2*amu*an(1)*an(2)
      er(5,1)= er(4,1)
      er(6,1)= alam+2*amu*an(2)**2
      er(1,2)=-an(2)*c2
      er(2,2)= an(1)*c2
      er(3,2)=-2*amu*an(1)*an(2)
      er(4,2)= amu*(an(1)**2-an(2)**2)
      er(5,2)= er(4,2)
      er(6,2)=-er(3,2)
c
c Godunov flux (computed from the left)
      do j=1,2
        alp=aj*alpha(j)*al(j)
        do i=1,6
          fx(i)=fx(i)+alp*er(i,j)
        end do
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
      subroutine smfx2dHLL (m,aj,a1,a2,wl,wr,fx,speed)
c
c HLL flux (method=3)
c
      implicit real*8 (a-h,o-z)
      dimension wl(m),wr(m),fx(6)
      dimension fl(6),fr(6)
      common / smgdat / amu,alam,rho0
      common / smgprm / method,iorder,ilimit,iupwind,icart,ifrc
c
c fastest wave speed
      rad=dsqrt(a1**2+a2**2)
      c1=rad*dsqrt((alam+2*amu)/rho0)
      speed=max(c1,speed)
c
c left and right fluxes
      call smflx (m,aj,a1,a2,wl,fl)
      call smflx (m,aj,a1,a2,wr,fr)
c
c compute HLL flux
      fact=c1*aj
      do i=1,6
        fx(i)=.5d0*(fl(i)+fr(i)-fact*(wr(i)-wl(i)))
      end do
c
      return
      end
c
c++++++++++++++++++++
c
      subroutine smfx2dlf (m,dt,ds,aj,a1,a2,wl,wr,fx,speed)
c
c Lax-Friedrichs flux (method=1)
c
      implicit real*8 (a-h,o-z)
      dimension wl(m),wr(m),fx(6),w(6)
      common / smgdat / amu,alam,rho0
      common / smgprm / method,iorder,ilimit,iupwind,icart,ifrc
c
c averages
      do i=1,6
        w(i)=.5d0*(wl(i)+wr(i))
      end do
c
c centered part
      call smflx (m,aj,a1,a2,w,fx)
c
c dissipation correction
      sig=.25d0*ds/dt
      do i=1,6
        fx(i)=fx(i)-sig*aj*(wr(i)-wl(i))
      end do
c
c max wave speed
      almax=dsqrt((a1**2+a2**2)*(alam+2*amu)/rho0)
      speed=max(almax,speed)
c
      return
      end
c
c++++++++++++++++++++
c
      subroutine smfx2dlfOLD (m,dt,ds,ajl,a1l,a2l,ajr,a1r,a2r,wl,wr,fx,
     *                        speed)
c
c Lax-Friedrichs flux (method=1)
c
      implicit real*8 (a-h,o-z)
      dimension wl(m),wr(m),fx(6),w(6)
      common / smgdat / amu,alam,rho0
      common / smgprm / method,iorder,ilimit,iupwind,icart,ifrc
c
c averages
      aj=.5d0*(ajl+ajr)
      a1=.5d0*(a1l+a1r)
      a2=.5d0*(a2l+a2r)
      do i=1,6
        w(i)=.5d0*(wl(i)+wr(i))
      end do
c
c centered part
      call smflx (m,aj,a1,a2,w,fx)
c
c dissipation correction
      sig=.25d0*ds/dt
      do i=1,6
c        fx(i)=fx(i)-sig*(ajr*wr(i)-ajl*wl(i))
        fx(i)=fx(i)-sig*aj*(wr(i)-wl(i))
      end do
c
c max wave speed
      almax=dsqrt((a1**2+a2**2)*(alam+2*amu)/rho0)
      speed=max(almax,speed)
c
      return
      end
c
c++++++++++++++++++++
c
      subroutine smfx2dlw (m,dt,ds,ds2,nd1a,nd1b,nd2a,nd2b,j1,j2,
     *                     is,det,rx,u,fx,speed)
c
c Lax-Wendroff flux (method=2)
c
      implicit real*8 (a-h,o-z)
      dimension det(nd1a:nd1b,nd2a:nd2b),rx(nd1a:nd1b,nd2a:nd2b,2,2),
     *          u(nd1a:nd1b,nd2a:nd2b,m)
      dimension al(2),ar(2),apl(2),apr(2),aml(2),amr(2),fx(6)
      dimension w(6),wl(6),wr(6),wp(6),wm(6),fl(6),fr(6),fp(6),fm(6)
      common / smgdat / amu,alam,rho0
      common / smgprm / method,iorder,ilimit,iupwind,icart,ifrc
c
      dt2=.5d0*dt
      ds22=2.d0*ds2
c
      if (is.eq.1) then
c index positions (left)
        j1l=j1
        j2l=j2
        j1lp=j1l
        j2lp=j2l+1
        j1lm=j1l
        j2lm=j2l-1
c index positions (right)
        j1r=j1+1
        j2r=j2
        j1rp=j1r
        j2rp=j2r+1
        j1rm=j1r
        j2rm=j2r-1
        if (icart.eq.0) then
c
c left and right values       ***** non-Cartesian case, side=1 ******
          ajl=det(j1l,j2l)
          a1l=rx(j1l,j2l,1,1)
          a2l=rx(j1l,j2l,1,2)
          ajr=det(j1r,j2r)
          a1r=rx(j1r,j2r,1,1)
          a2r=rx(j1r,j2r,1,2)
          do i=1,6
            wl(i)=u(j1l,j2l,i)
            wr(i)=u(j1r,j2r,i)
          end do
c
c averages
          aj=.5d0*(ajl+ajr)
          a1=.5d0*(a1l+a1r)
          a2=.5d0*(a2l+a2r)
          ajp=.5d0*(det(j1lp,j2lp)+det(j1rp,j2rp))
          a1p=.5d0*(rx(j1lp,j2lp,2,1)+rx(j1rp,j2rp,2,1))
          a2p=.5d0*(rx(j1lp,j2lp,2,2)+rx(j1rp,j2rp,2,2))
          ajm=.5d0*(det(j1lm,j2lm)+det(j1rm,j2rm))
          a1m=.5d0*(rx(j1lm,j2lm,2,1)+rx(j1rm,j2rm,2,1))
          a2m=.5d0*(rx(j1lm,j2lm,2,2)+rx(j1rm,j2rm,2,2))
          do i=1,6
            w(i)=.5d0*(wl(i)+wr(i))
            wp(i)=.5d0*(u(j1lp,j2lp,i)+u(j1rp,j2rp,i))
            wm(i)=.5d0*(u(j1lm,j2lm,i)+u(j1rm,j2rm,i))
          end do
c
c first-order update
          call smflx (m,ajr,a1r,a2r,wr,fr)
          call smflx (m,ajl,a1l,a2l,wl,fl)
          call smflx (m,ajp,a1p,a2p,wp,fp)
          call smflx (m,ajm,a1m,a2m,wm,fm)
          do i=1,6
            w(i)=w(i)-dt2*((fr(i)-fl(i))/ds+(fp(i)-fm(i))/ds22)/aj
          end do
c
c second-order flux
          call smflx (m,aj,a1,a2,w,fx)
c
c max wave speed
          almax=dsqrt((a1**2+a2**2)*(alam+2*amu)/rho0)
          speed=max(almax,speed)
c
        else
c
c left and right values       ***** Cartesian case, side=1 ******
          do i=1,6
            wl(i)=u(j1l,j2l,i)
            wr(i)=u(j1r,j2r,i)
          end do
c
c averages
          do i=1,6
            w(i)=.5d0*(wl(i)+wr(i))
            wp(i)=.5d0*(u(j1lp,j2lp,i)+u(j1rp,j2rp,i))
            wm(i)=.5d0*(u(j1lm,j2lm,i)+u(j1rm,j2rm,i))
          end do
c
c first-order update
          call smflx (m,1.d0,1.d0,0.d0,wr,fr)
          call smflx (m,1.d0,1.d0,0.d0,wl,fl)
          call smflx (m,1.d0,0.d0,1.d0,wp,fp)
          call smflx (m,1.d0,0.d0,1.d0,wm,fm)
          do i=1,6
            w(i)=w(i)-dt2*((fr(i)-fl(i))/ds+(fp(i)-fm(i))/ds22)
          end do
c
c second-order flux
          call smflx (m,1.d0,1.d0,0.d0,w,fx)
c
c max wave speed
          almax=dsqrt((alam+2*amu)/rho0)
          speed=max(almax,speed)
c
        end if
      elseif (is.eq.2) then
c index positions (left)
        j1l=j1
        j2l=j2
        j1lp=j1l+1
        j2lp=j2l
        j1lm=j1l-1
        j2lm=j2l
c index positions (right)
        j1r=j1
        j2r=j2+1
        j1rp=j1r+1
        j2rp=j2r
        j1rm=j1r-1
        j2rm=j2r
        if (icart.eq.0) then
c
c left and right values       ***** non-Cartesian case, side=2 ******
          ajl=det(j1l,j2l)
          a1l=rx(j1l,j2l,2,1)
          a2l=rx(j1l,j2l,2,2)
          ajr=det(j1r,j2r)
          a1r=rx(j1r,j2r,2,1)
          a2r=rx(j1r,j2r,2,2)
          do i=1,6
            wl(i)=u(j1l,j2l,i)
            wr(i)=u(j1r,j2r,i)
          end do
c
c averages
          aj=.5d0*(ajl+ajr)
          a1=.5d0*(a1l+a1r)
          a2=.5d0*(a2l+a2r)
          ajp=.5d0*(det(j1lp,j2lp)+det(j1rp,j2rp))
          a1p=.5d0*(rx(j1lp,j2lp,1,1)+rx(j1rp,j2rp,1,1))
          a2p=.5d0*(rx(j1lp,j2lp,1,2)+rx(j1rp,j2rp,1,2))
          ajm=.5d0*(det(j1lm,j2lm)+det(j1rm,j2rm))
          a1m=.5d0*(rx(j1lm,j2lm,1,1)+rx(j1rm,j2rm,1,1))
          a2m=.5d0*(rx(j1lm,j2lm,1,2)+rx(j1rm,j2rm,1,2))
          do i=1,6
            w(i)=.5d0*(wl(i)+wr(i))
            wp(i)=.5d0*(u(j1lp,j2lp,i)+u(j1rp,j2rp,i))
            wm(i)=.5d0*(u(j1lm,j2lm,i)+u(j1rm,j2rm,i))
          end do
c
c first-order update
          call smflx (m,ajr,a1r,a2r,wr,fr)
          call smflx (m,ajl,a1l,a2l,wl,fl)
          call smflx (m,ajp,a1p,a2p,wp,fp)
          call smflx (m,ajm,a1m,a2m,wm,fm)
          do i=1,6
            w(i)=w(i)-dt2*((fr(i)-fl(i))/ds+(fp(i)-fm(i))/ds22)/aj
          end do
c
c second-order flux
          call smflx (m,aj,a1,a2,w,fx)
c
c max wave speed
          almax=dsqrt((a1**2+a2**2)*(alam+2*amu)/rho0)
          speed=max(almax,speed)
c
        else
c
c left and right values       ***** Cartesian case, side=2 ******
          do i=1,6
            wl(i)=u(j1l,j2l,i)
            wr(i)=u(j1r,j2r,i)
          end do
c
c averages
          do i=1,6
            w(i)=.5d0*(wl(i)+wr(i))
            wp(i)=.5d0*(u(j1lp,j2lp,i)+u(j1rp,j2rp,i))
            wm(i)=.5d0*(u(j1lm,j2lm,i)+u(j1rm,j2rm,i))
          end do
c
c first-order update
          call smflx (m,1.d0,0.d0,1.d0,wr,fr)
          call smflx (m,1.d0,0.d0,1.d0,wl,fl)
          call smflx (m,1.d0,1.d0,0.d0,wp,fp)
          call smflx (m,1.d0,1.d0,0.d0,wm,fm)
          do i=1,6
            w(i)=w(i)-dt2*((fr(i)-fl(i))/ds+(fp(i)-fm(i))/ds22)
          end do
c
c second-order flux
          call smflx (m,1.d0,0.d0,1.d0,w,fx)
c
c max wave speed
          almax=dsqrt((alam+2*amu)/rho0)
          speed=max(almax,speed)
c
        end if
      else
        write(6,*)'Error (smfx2dlw) : invalid value for is'
        stop
      end if
c
      return
      end
c
c++++++++++++++++++++
c
      subroutine smdisp (m,dt,ds1,ds2,nd1a,nd1b,nd2a,nd2b,n1a,n1b,j2,
     *                   det,rx,u,md1a,md1b,w)
c
c Lax-Wendroff flux (compute velocities to update displacement)
c
      implicit real*8 (a-h,o-z)
      dimension det(nd1a:nd1b,nd2a:nd2b),rx(nd1a:nd1b,nd2a:nd2b,2,2),
     *          u(nd1a:nd1b,nd2a:nd2b,m),w(m,md1a:md1b)
      dimension wrp(6),wrm(6),wsp(6),wsm(6),frp(6),frm(6),fsp(6),fsm(6)
      common / smgprm / method,iorder,ilimit,iupwind,icart,ifrc
c
      dt4=.25d0*dt
c
      if (icart.eq.0) then
        do j1=n1a,n1b         ! ***** non-Cartesian case *****
          do i=1,6
            wrp(i)=u(j1+1,j2,i)
            wrm(i)=u(j1-1,j2,i)
            wsp(i)=u(j1,j2+1,i)
            wsm(i)=u(j1,j2-1,i)
          end do
          call smflx (m,det(j1+1,j2),rx(j1+1,j2,1,1),rx(j1+1,j2,1,2),
     *                wrp,frp)
          call smflx (m,det(j1-1,j2),rx(j1-1,j2,1,1),rx(j1-1,j2,1,2),
     *                wrm,frm)
          call smflx (m,det(j1,j2+1),rx(j1,j2+1,2,1),rx(j1,j2+1,2,2),
     *                wsp,fsp)
          call smflx (m,det(j1,j2-1),rx(j1,j2-1,2,1),rx(j1,j2-1,2,2),
     *                wsm,fsm)
          do i=1,2
            w(i,j1)=u(j1,j2,i)-dt4*((frp(i)-frm(i))/ds1
     *                             +(fsp(i)-fsm(i))/ds2)/det(j1,j2)
          end do
        end do
      else
        do j1=n1a,n1b         ! ***** Cartesian case *****
          do i=1,6
            wrp(i)=u(j1+1,j2,i)
            wrm(i)=u(j1-1,j2,i)
            wsp(i)=u(j1,j2+1,i)
            wsm(i)=u(j1,j2-1,i)
          end do
          call smflx (m,1.d0,1.d0,0.d0,wrp,frp)
          call smflx (m,1.d0,1.d0,0.d0,wrm,frm)
          call smflx (m,1.d0,0.d0,1.d0,wsp,fsp)
          call smflx (m,1.d0,0.d0,1.d0,wsm,fsm)
          do i=1,2
            w(i,j1)=u(j1,j2,i)-dt4*((frp(i)-frm(i))/ds1
     *                             +(fsp(i)-fsm(i))/ds2)
          end do
        end do
      end if
c
      return
      end
c
c++++++++++++++++++++
c
      subroutine smflx (m,aj,a1,a2,w,fx)
c
c flux
c
      implicit real*8 (a-h,o-z)
      dimension w(6),fx(6)
      common / smgdat / amu,alam,rho0
c
      fx(1)=-aj*(a1*w(3)+a2*w(5))/rho0
      fx(2)=-aj*(a1*w(4)+a2*w(6))/rho0
      fx(3)=-aj*(a1*(alam+2.d0*amu)*w(1)+a2*alam*w(2))
      fx(4)=-aj*amu*(a1*w(2)+a2*w(1))
      fx(5)= fx(4)
      fx(6)=-aj*(a1*alam*w(1)+a2*(alam+2.d0*amu)*w(2))
c
      return
      end
c
c++++++++++++++++++++
c
      subroutine smGethtz (m,x,y,t,htz)
c
c compute tz forcing function
c
      implicit real*8 (a-h,o-z)
      dimension htz(m),ut(8),ux(6),uy(6),u0(2)
      common / smgdat / amu,alam,rho0
      common / tzflow / eptz,itz
c
c      htz(1)=v1t-(s11x+s21y)/rho0
c      htz(2)=v2t-(s12x+s22y)/rho0
c      htz(3)=s11t-alam*(v1x+v2y)-2.d0*amu*v1x
c      htz(4)=s12t-amu*(v1y+v2x)
c      htz(5)=s21t-amu*(v1y+v2x)
c      htz(6)=s22t-alam*(v1x+v2y)-2.d0*amu*v2y
c      htz(7)=u1t-v1
c      htz(8)=u2t-v2
c
      do i=1,8
        call ogDeriv (eptz,1,0,0,0,x,y,0.d0,t,i-1,ut(i))
      end do
      do i=1,6
        call ogDeriv (eptz,0,1,0,0,x,y,0.d0,t,i-1,ux(i))
        call ogDeriv (eptz,0,0,1,0,x,y,0.d0,t,i-1,uy(i))
      end do
      do i=1,2
        call ogDeriv (eptz,0,0,0,0,x,y,0.d0,t,i-1,u0(i))
      end do
c
      htz(1)=ut(1)-(ux(3)+uy(5))/rho0
      htz(2)=ut(2)-(ux(4)+uy(6))/rho0
      htz(3)=ut(3)-alam*(ux(1)+uy(2))-2.d0*amu*ux(1)
      htz(4)=ut(4)-amu*(uy(1)+ux(2))
      htz(5)=ut(5)-amu*(uy(1)+ux(2))
      htz(6)=ut(6)-alam*(ux(1)+uy(2))-2.d0*amu*uy(2)
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
c        write(6,*)'Warning (smGethtz) : sanity check failed'
c      end if
c
      return
      end
c
c+++++++++++++++++++++++
c
      subroutine smgbcs (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                   ds1,ds2,t,xy,u,ibc)
c
c variables: u(j1,j2,1:2)=velocity
c            u(j1,j2,3:6)=stress (S11,S12,S21,S22)
c            u(j1,j2,7:8)=displacement
c
      implicit real*8 (a-h,o-z)
      parameter (md1a=-5,md1b=210,md2a=-5,md2b=210)
      dimension ub1(md1a:md1b,md2a:md2b,8),ub2(md1a:md1b,md2a:md2b,8)
      dimension xy(nd1a:nd1b,nd2a:nd2b,2),u(nd1a:nd1b,nd2a:nd2b,m)
      common / smgdat / amu,alam,rho0
      common / tzflow / eptz,itz
c
c      write(6,*)'smgbcs, t=',t
c
c      write(6,*)itz,eptz,amu,alam,rho0
c      pause
c
      j1dif=n1b-n1a
      j2dif=n2b-n2a
c
      if (itz.eq.0) then
        do i=1,m
          do j2=n2a,n2b
            ub1(n1a,j2,i)=0.d0
            ub1(n1b,j2,i)=0.d0
            ub1(n1a-1,j2,i)=0.d0
            ub1(n1b+1,j2,i)=0.d0
          end do
          do j1=n1a,n1b
            ub2(j1,n2a,i)=0.d0
            ub2(j1,n2b,i)=0.d0
            ub2(j1,n2a-1,i)=0.d0
            ub2(j1,n2b+1,i)=0.d0
          end do
        end do
        do j1=n1a,n1b,j1dif
        do j2=n2a,n2b,j2dif
          do i=3,6
            ub1(j1,j2,i)=0.d0
          end do
        end do
        end do
      else
        do j2=n2a,n2b
          call ogDeriv (eptz,0,0,0,0,xy(n1a,j2,1),xy(n1a,j2,2),0.d0,t,
     *                  6,ub1(n1a,j2,7))
          call ogDeriv (eptz,0,0,0,0,xy(n1b,j2,1),xy(n1b,j2,2),0.d0,t,
     *                  6,ub1(n1b,j2,7))
          call ogDeriv (eptz,0,0,0,0,xy(n1a,j2,1),xy(n1a,j2,2),0.d0,t,
     *                  7,ub1(n1a,j2,8))
          call ogDeriv (eptz,0,0,0,0,xy(n1b,j2,1),xy(n1b,j2,2),0.d0,t,
     *                  7,ub1(n1b,j2,8))
          call ogDeriv (eptz,0,0,0,0,xy(n1a,j2,1),xy(n1a,j2,2),0.d0,t,
     *                  0,ub1(n1a,j2,1))
          call ogDeriv (eptz,0,0,0,0,xy(n1b,j2,1),xy(n1b,j2,2),0.d0,t,
     *                  0,ub1(n1b,j2,1))
          call ogDeriv (eptz,0,0,0,0,xy(n1a,j2,1),xy(n1a,j2,2),0.d0,t,
     *                  1,ub1(n1a,j2,2))
          call ogDeriv (eptz,0,0,0,0,xy(n1b,j2,1),xy(n1b,j2,2),0.d0,t,
     *                  1,ub1(n1b,j2,2))
          call ogDeriv (eptz,0,1,0,0,xy(n1a,j2,1),xy(n1a,j2,2),0.d0,t,
     *                  2,s11x)
          call ogDeriv (eptz,0,1,0,0,xy(n1a,j2,1),xy(n1a,j2,2),0.d0,t,
     *                  3,s12x)
          call ogDeriv (eptz,0,0,1,0,xy(n1a,j2,1),xy(n1a,j2,2),0.d0,t,
     *                  4,s21y)
          call ogDeriv (eptz,0,0,1,0,xy(n1a,j2,1),xy(n1a,j2,2),0.d0,t,
     *                  5,s22y)
          ub1(n1a-1,j2,3)=s11x+s21y
          ub1(n1a-1,j2,4)=s12x+s22y
          call ogDeriv (eptz,0,1,0,0,xy(n1b,j2,1),xy(n1b,j2,2),0.d0,t,
     *                  2,s11x)
          call ogDeriv (eptz,0,1,0,0,xy(n1b,j2,1),xy(n1b,j2,2),0.d0,t,
     *                  3,s12x)
          call ogDeriv (eptz,0,0,1,0,xy(n1b,j2,1),xy(n1b,j2,2),0.d0,t,
     *                  4,s21y)
          call ogDeriv (eptz,0,0,1,0,xy(n1b,j2,1),xy(n1b,j2,2),0.d0,t,
     *                  5,s22y)
          ub1(n1b+1,j2,3)=s11x+s21y
          ub1(n1b+1,j2,4)=s12x+s22y
        end do
        do j1=n1a,n1b
          call ogDeriv (eptz,0,0,0,0,xy(j1,n2a,1),xy(j1,n2a,2),0.d0,t,
     *                  6,ub2(j1,n2a,7))
          call ogDeriv (eptz,0,0,0,0,xy(j1,n2b,1),xy(j1,n2b,2),0.d0,t,
     *                  6,ub2(j1,n2b,7))
          call ogDeriv (eptz,0,0,0,0,xy(j1,n2a,1),xy(j1,n2a,2),0.d0,t,
     *                  7,ub2(j1,n2a,8))
          call ogDeriv (eptz,0,0,0,0,xy(j1,n2b,1),xy(j1,n2b,2),0.d0,t,
     *                  7,ub2(j1,n2b,8))
          call ogDeriv (eptz,0,0,0,0,xy(j1,n2a,1),xy(j1,n2a,2),0.d0,t,
     *                  0,ub2(j1,n2a,1))
          call ogDeriv (eptz,0,0,0,0,xy(j1,n2b,1),xy(j1,n2b,2),0.d0,t,
     *                  0,ub2(j1,n2b,1))
          call ogDeriv (eptz,0,0,0,0,xy(j1,n2a,1),xy(j1,n2a,2),0.d0,t,
     *                  1,ub2(j1,n2a,2))
          call ogDeriv (eptz,0,0,0,0,xy(j1,n2b,1),xy(j1,n2b,2),0.d0,t,
     *                  1,ub2(j1,n2b,2))
          call ogDeriv (eptz,0,1,0,0,xy(j1,n2a,1),xy(j1,n2a,2),0.d0,t,
     *                  2,s11x)
          call ogDeriv (eptz,0,1,0,0,xy(j1,n2a,1),xy(j1,n2a,2),0.d0,t,
     *                  3,s12x)
          call ogDeriv (eptz,0,0,1,0,xy(j1,n2a,1),xy(j1,n2a,2),0.d0,t,
     *                  4,s21y)
          call ogDeriv (eptz,0,0,1,0,xy(j1,n2a,1),xy(j1,n2a,2),0.d0,t,
     *                  5,s22y)
          ub2(j1,n2a-1,5)=s11x+s21y
          ub2(j1,n2a-1,6)=s12x+s22y
          call ogDeriv (eptz,0,1,0,0,xy(j1,n2b,1),xy(j1,n2b,2),0.d0,t,
     *                  2,s11x)
          call ogDeriv (eptz,0,1,0,0,xy(j1,n2b,1),xy(j1,n2b,2),0.d0,t,
     *                  3,s12x)
          call ogDeriv (eptz,0,0,1,0,xy(j1,n2b,1),xy(j1,n2b,2),0.d0,t,
     *                  4,s21y)
          call ogDeriv (eptz,0,0,1,0,xy(j1,n2b,1),xy(j1,n2b,2),0.d0,t,
     *                  5,s22y)
          ub2(j1,n2b+1,5)=s11x+s21y
          ub2(j1,n2b+1,6)=s12x+s22y
        end do
        do j1=n1a,n1b,j1dif
        do j2=n2a,n2b,j2dif
          do i=3,6
            call ogDeriv (eptz,0,0,0,0,xy(j1,j2,1),xy(j1,j2,2),0.d0,t,
     *                    i-1,ub1(j1,j2,i))
          end do
          call ogDeriv (eptz,0,1,0,0,xy(j1,j2,1),xy(j1,j2,2),0.d0,t,
     *                  6,u1x)
          call ogDeriv (eptz,0,1,0,0,xy(j1,j2,1),xy(j1,j2,2),0.d0,t,
     *                  7,u2x)
          call ogDeriv (eptz,0,0,1,0,xy(j1,j2,1),xy(j1,j2,2),0.d0,t,
     *                  6,u1y)
          call ogDeriv (eptz,0,0,1,0,xy(j1,j2,1),xy(j1,j2,2),0.d0,t,
     *                  7,u2y)
          ub1(j1,j2,3)=ub1(j1,j2,3)-(alam*(u1x+u2y)+2.d0*amu*u1x)
          ub1(j1,j2,4)=ub1(j1,j2,4)-(amu*(u1y+u2x))
          ub1(j1,j2,5)=ub1(j1,j2,5)-(amu*(u1y+u2x))
          ub1(j1,j2,6)=ub1(j1,j2,6)-(alam*(u1x+u2y)+2.d0*amu*u2y)
        end do
        end do
      end if
c
c      do j2=n2a,n2b
c        write(6,453)n1a,j2,ub1(n1a,j2,7),ub1(n1a,j2,8),ub1(n1a,j2,1),
c     *              ub1(n1a,j2,2),ub1(n1a-1,j2,3),ub1(n1a-1,j2,4)
c  453   format(2(1x,i2),6(1x,1pe12.5))
c      end do
c      do j2=n2a,n2b
c        write(6,453)n1b,j2,ub1(n1b,j2,7),ub1(n1b,j2,8),ub1(n1b,j2,1),
c     *              ub1(n1b,j2,2),ub1(n1b+1,j2,3),ub1(n1b+1,j2,4)
c      end do
c      do j1=n1a,n1b
c        write(6,453)j1,n2a,ub2(j1,n2a,7),ub2(j1,n2a,8),ub2(j1,n2a,1),
c     *              ub2(j1,n2a,2),ub2(j1,n2a-1,5),ub2(j1,n2a-1,6)
c      end do
c      do j1=n1a,n1b
c        write(6,453)j1,n2b,ub2(j1,n2b,7),ub2(j1,n2b,8),ub2(j1,n2b,1),
c     *              ub2(j1,n2b,2),ub2(j1,n2b+1,5),ub2(j1,n2b+1,6)
c      end do
c      pause
c
c
c      if (m.gt.0) return
c
c..step 0: Dirichlet bcs for displacement and velocity
      do j2=n2a,n2b
        u(n1a,j2,7)=ub1(n1a,j2,7)
        u(n1b,j2,7)=ub1(n1b,j2,7)
        u(n1a,j2,8)=ub1(n1a,j2,8)
        u(n1b,j2,8)=ub1(n1b,j2,8)
        u(n1a,j2,1)=ub1(n1a,j2,1)
        u(n1b,j2,1)=ub1(n1b,j2,1)
        u(n1a,j2,2)=ub1(n1a,j2,2)
        u(n1b,j2,2)=ub1(n1b,j2,2)
      end do
      do j1=n1a,n1b
        u(j1,n2a,7)=ub2(j1,n2a,7)
        u(j1,n2b,7)=ub2(j1,n2b,7)
        u(j1,n2a,8)=ub2(j1,n2a,8)
        u(j1,n2b,8)=ub2(j1,n2b,8)
        u(j1,n2a,1)=ub2(j1,n2a,1)
        u(j1,n2b,1)=ub2(j1,n2b,1)
        u(j1,n2a,2)=ub2(j1,n2a,2)
        u(j1,n2b,2)=ub2(j1,n2b,2)
      end do
c
c..exact values for the stresses in the corners
      if (.false.) then
        do i=3,6
          call ogDeriv (eptz,0,0,0,0,xy(n1a,n2a,1),xy(n1a,n2a,2),0.d0,
     *                  t,i-1,u(n1a,n2a,i))
          call ogDeriv (eptz,0,0,0,0,xy(n1a,n2b,1),xy(n1a,n2b,2),0.d0,
     *                  t,i-1,u(n1a,n2b,i))
          call ogDeriv (eptz,0,0,0,0,xy(n1b,n2a,1),xy(n1b,n2a,2),0.d0,
     *                  t,i-1,u(n1b,n2a,i))
          call ogDeriv (eptz,0,0,0,0,xy(n1b,n2b,1),xy(n1b,n2b,2),0.d0,
     *                  t,i-1,u(n1b,n2b,i))
        end do
      end if
c
c..step 1: extrapolate to first ghost cells
      do i=1,m
        do j2=n2a,n2b
c          u(n1a-1,j2,i)=2.d0*u(n1a,j2,i)-u(n1a+1,j2,i)
c          u(n1b+1,j2,i)=2.d0*u(n1b,j2,i)-u(n1b-1,j2,i)
          u(n1a-1,j2,i)=3.d0*(u(n1a,j2,i)-u(n1a+1,j2,i))+u(n1a+2,j2,i)
          u(n1b+1,j2,i)=3.d0*(u(n1b,j2,i)-u(n1b-1,j2,i))+u(n1b-2,j2,i)
        end do
        do j1=n1a-1,n1b+1
c          u(j1,n2a-1,i)=2.d0*u(j1,n2a,i)-u(j1,n2a+1,i)
c          u(j1,n2b+1,i)=2.d0*u(j1,n2b,i)-u(j1,n2b-1,i)
          u(j1,n2a-1,i)=3.d0*(u(j1,n2a,i)-u(j1,n2a+1,i))+u(j1,n2a+2,i)
          u(j1,n2b+1,i)=3.d0*(u(j1,n2b,i)-u(j1,n2b-1,i))+u(j1,n2b-2,i)
        end do
      end do
c
c..fill in exact values in the first ghost lines
      if (.false.) then
        do i=1,m
        do j1=n1a,n1b
          call ogDeriv (eptz,0,0,0,0,xy(j1,n2a-1,1),xy(j1,n2a-1,2),
     *                  0.d0,t,i-1,u(j1,n2a-1,i))
          call ogDeriv (eptz,0,0,0,0,xy(j1,n2b+1,1),xy(j1,n2b+1,2),
     *                  0.d0,t,i-1,u(j1,n2b+1,i))
        end do
        do j2=n2a-1,n2b+1
          call ogDeriv (eptz,0,0,0,0,xy(n1a-1,j2,1),xy(n1a-1,j2,2),
     *                  0.d0,t,i-1,u(n1a-1,j2,i))
          call ogDeriv (eptz,0,0,0,0,xy(n1b+1,j2,1),xy(n1b+1,j2,2),
     *                  0.d0,t,i-1,u(n1b+1,j2,i))
        end do
        end do
      end if
c
c..Dirichlet bcs for stress in the corner
      do j1=n1a,n1b,j1dif
      do j2=n2a,n2b,j2dif
        u1x=(u(j1+1,j2,7)-u(j1-1,j2,7))/(2.d0*ds1)
        u2x=(u(j1+1,j2,8)-u(j1-1,j2,8))/(2.d0*ds1)
        u1y=(u(j1,j2+1,7)-u(j1,j2-1,7))/(2.d0*ds2)
        u2y=(u(j1,j2+1,8)-u(j1,j2-1,8))/(2.d0*ds2)
        u(j1,j2,3)=alam*(u1x+u2y)+2.d0*amu*u1x+ub1(j1,j2,3)
        u(j1,j2,4)=amu*(u1y+u2x)              +ub1(j1,j2,4)
        u(j1,j2,5)=amu*(u1y+u2x)              +ub1(j1,j2,5)
        u(j1,j2,6)=alam*(u1x+u2y)+2.d0*amu*u2y+ub1(j1,j2,6)
      end do
      end do
c
c..step 2: Neumann bcs for stress
c      do j2=n2a,n2b
      do j2=n2a+1,n2b-1
        u(n1a-1,j2,3)=u(n1a+1,j2,3)-2.d0*ds1*(ub1(n1a-1,j2,3)
     *                 -(u(n1a,j2+1,5)-u(n1a,j2-1,5))/(2.d0*ds2))
        u(n1b+1,j2,3)=u(n1b-1,j2,3)+2.d0*ds1*(ub1(n1b+1,j2,3)
     *                 -(u(n1b,j2+1,5)-u(n1b,j2-1,5))/(2.d0*ds2))
        u(n1a-1,j2,4)=u(n1a+1,j2,4)-2.d0*ds1*(ub1(n1a-1,j2,4)
     *                 -(u(n1a,j2+1,6)-u(n1a,j2-1,6))/(2.d0*ds2))
        u(n1b+1,j2,4)=u(n1b-1,j2,4)+2.d0*ds1*(ub1(n1b+1,j2,4)
     *                 -(u(n1b,j2+1,6)-u(n1b,j2-1,6))/(2.d0*ds2))
c        u(n1a-1,j2,5)=u(n1a-1,j2,4)
c        u(n1b+1,j2,5)=u(n1b+1,j2,4)
      end do
c      do j1=n1a,n1b
      do j1=n1a+1,n1b-1
        u(j1,n2a-1,5)=u(j1,n2a+1,5)-2.d0*ds2*(ub2(j1,n2a-1,5)
     *                 -(u(j1+1,n2a,3)-u(j1-1,n2a,3))/(2.d0*ds1))
        u(j1,n2b+1,5)=u(j1,n2b-1,5)+2.d0*ds2*(ub2(j1,n2b+1,5)
     *                 -(u(j1+1,n2b,3)-u(j1-1,n2b,3))/(2.d0*ds1))
        u(j1,n2a-1,6)=u(j1,n2a+1,6)-2.d0*ds2*(ub2(j1,n2a-1,6)
     *                 -(u(j1+1,n2a,4)-u(j1-1,n2a,4))/(2.d0*ds1))
        u(j1,n2b+1,6)=u(j1,n2b-1,6)+2.d0*ds2*(ub2(j1,n2b+1,6)
     *                 -(u(j1+1,n2b,4)-u(j1-1,n2b,4))/(2.d0*ds1))
c        u(j1,n2a-1,4)=u(j1,n2a-1,5)
c        u(j1,n2b+1,4)=u(j1,n2b+1,5)
      end do
c
c..step 3: assign ghost values for displacement
      fact1=1.d0/(alam+2.d0*amu)
      fact2=1.d0/amu
c      do j2=n2a,n2b
c        u(n1a-1,j2,7)=u(n1a+1,j2,7)-2.d0*ds1*fact1*(u(n1a,j2,3)
c     *                 -alam*(u(n1a,j2+1,8)-u(n1a,j2-1,8))/(2.d0*ds2))
c        u(n1b+1,j2,7)=u(n1b-1,j2,7)+2.d0*ds1*fact1*(u(n1b,j2,3)
c     *                 -alam*(u(n1b,j2+1,8)-u(n1b,j2-1,8))/(2.d0*ds2))
c        u(n1a-1,j2,8)=u(n1a+1,j2,8)-2.d0*ds1*fact2*(u(n1a,j2,4)
c     *                 -amu*(u(n1a,j2+1,7)-u(n1a,j2-1,7))/(2.d0*ds2))
c        u(n1b+1,j2,8)=u(n1b-1,j2,8)+2.d0*ds1*fact2*(u(n1b,j2,4)
c     *                 -amu*(u(n1b,j2+1,7)-u(n1b,j2-1,7))/(2.d0*ds2))
c      end do
c      do j1=n1a,n1b
c        u(j1,n2a-1,7)=u(j1,n2a+1,7)-2.d0*ds2*fact2*(u(j1,n2a,5)
c     *                 -amu*(u(j1+1,n2a,8)-u(j1-1,n2a,8))/(2.d0*ds1))
c        u(j1,n2b+1,7)=u(j1,n2b-1,7)+2.d0*ds2*fact2*(u(j1,n2b,5)
c     *                 -amu*(u(j1+1,n2b,8)-u(j1-1,n2b,8))/(2.d0*ds1))
c        u(j1,n2a-1,8)=u(j1,n2a+1,8)-2.d0*ds2*fact1*(u(j1,n2a,6)
c     *                 -alam*(u(j1+1,n2a,7)-u(j1-1,n2a,7))/(2.d0*ds1))
c        u(j1,n2b+1,8)=u(j1,n2b-1,8)+2.d0*ds2*fact1*(u(j1,n2b,6)
c     *                 -alam*(u(j1+1,n2b,7)-u(j1-1,n2b,7))/(2.d0*ds1))
c      end do
c
c..step 5: assign ghost value for velocity (temporarily place the computed
c          value in the second ghost line).
      u1ttt=0.d0
      u2ttt=0.d0
      ds11=ds1**2
      ds12=ds1*ds2
      ds22=ds2**2
c      do j2=n2a,n2b
c        u(n1a-2,j2,1)=2.d0*u(n1a,j2,1)-u(n1a+1,j2,1)+ds11*fact1
c     *                *(rho0*u1ttt-amu*(u(n1a,j2+1,1)
c     *                 -2.d0*u(n1a,j2,1)+u(n1a,j2-1,1))/ds22
c     *                 -(alam+amu)*(u(n1a+1,j2+1,2)-u(n1a-1,j2+1,2)
c     *                  -u(n1a+1,j2-1,2)+u(n1a-1,j2-1,2))/(4.d0*ds12))
c        u(n1b+2,j2,1)=2.d0*u(n1b,j2,1)-u(n1b-1,j2,1)+ds11*fact1
c     *                *(rho0*u1ttt-amu*(u(n1b,j2+1,1)
c     *                 -2.d0*u(n1b,j2,1)+u(n1b,j2-1,1))/ds22
c     *                 -(alam+amu)*(u(n1b+1,j2+1,2)-u(n1b-1,j2+1,2)
c     *                  -u(n1b+1,j2-1,2)+u(n1b-1,j2-1,2))/(4.d0*ds12))
c        u(n1a-2,j2,2)=2.d0*u(n1a,j2,2)-u(n1a+1,j2,2)+ds11*fact2
c     *                *(rho0*u2ttt-(alam+2.d0*amu)*(u(n1a,j2+1,2)
c     *                 -2.d0*u(n1a,j2,2)+u(n1a,j2-1,2))/ds22
c     *                 -(alam+amu)*(u(n1a+1,j2+1,1)-u(n1a-1,j2+1,1)
c     *                  -u(n1a+1,j2-1,1)+u(n1a-1,j2-1,1))/(4.d0*ds12))
c        u(n1b+2,j2,2)=2.d0*u(n1b,j2,2)-u(n1b-1,j2,2)+ds11*fact2
c     *                *(rho0*u2ttt-(alam+2.d0*amu)*(u(n1b,j2+1,2)
c     *                 -2.d0*u(n1b,j2,2)+u(n1b,j2-1,2))/ds22
c     *                 -(alam+amu)*(u(n1b+1,j2+1,1)-u(n1b-1,j2+1,1)
c     *                  -u(n1b+1,j2-1,1)+u(n1b-1,j2-1,1))/(4.d0*ds12))
c      end do
c      do j1=n1a,n1b
c        u(j1,n2a-2,1)=2.d0*u(j1,n2a,1)-u(j1,n2a+1,1)+ds22*fact2
c     *                *(rho0*u1ttt-(alam+2.d0*amu)*(u(j1+1,n2a,1)
c     *                 -2.d0*u(j1,n2a,1)+u(j1-1,n2a,1))/ds11
c     *                 -(alam+amu)*(u(j1+1,n2a+1,2)-u(j1-1,n2a+1,2)
c     *                  -u(j1+1,n2a-1,2)+u(j1-1,n2a-1,2))/(4.d0*ds12))
c        u(j1,n2b+2,1)=2.d0*u(j1,n2b,1)-u(j1,n2b+1,1)+ds22*fact2
c     *                *(rho0*u1ttt-(alam+2.d0*amu)*(u(j1+1,n2b,1)
c     *                 -2.d0*u(j1,n2b,1)+u(j1-1,n2b,1))/ds11
c     *                 -(alam+amu)*(u(j1+1,n2b+1,2)-u(j1-1,n2b+1,2)
c     *                  -u(j1+1,n2b-1,2)+u(j1-1,n2b-1,2))/(4.d0*ds12))
c        u(j1,n2a-2,2)=2.d0*u(j1,n2a,2)-u(j1,n2a+1,2)+ds22*fact1
c     *                *(rho0*u2ttt-amu*(u(j1+1,n2a,2)
c     *                 -2.d0*u(j1,n2a,2)+u(j1-1,n2a,2))/ds11
c     *                 -(alam+amu)*(u(j1+1,n2a+1,1)-u(j1-1,n2a+1,1)
c     *                  -u(j1+1,n2a-1,1)+u(j1-1,n2a-1,1))/(4.d0*ds12))
c        u(j1,n2b+2,2)=2.d0*u(j1,n2b,2)-u(j1,n2b-1,2)+ds22*fact1
c     *                *(rho0*u2ttt-amu*(u(j1+1,n2b,2)
c     *                 -2.d0*u(j1,n2b,2)+u(j1-1,n2b,2))/ds11
c     *                 -(alam+amu)*(u(j1+1,n2b+1,1)-u(j1-1,n2b+1,1)
c     *                  -u(j1+1,n2b-1,1)+u(j1-1,n2b-1,1))/(4.d0*ds12))
c      end do
c
c..last step: move second-ghost line velocities back to the first ghost-line positions
c             and extrapolate the second ghost line
c      do i=1,2
c        do j2=n2a,n2b
c          u(n1a-1,j2,i)=u(n1a-2,j2,i)
c          u(n1b+1,j2,i)=u(n1b+2,j2,i)
c        end do
c        do j1=n1a,n1b
c          u(j1,n2a-1,i)=u(j1,n2a-2,i)
c          u(j1,n2b+1,i)=u(j1,n2b+2,i)
c        end do
c      end do
      do i=1,m
        do j2=n2a-1,n2b+1
c          u(n1a-2,j2,i)=2.d0*u(n1a-1,j2,i)-u(n1a,j2,i)
c          u(n1b+2,j2,i)=2.d0*u(n1b+1,j2,i)-u(n1b,j2,i)
          u(n1a-2,j2,i)=3.d0*(u(n1a-1,j2,i)-u(n1a,j2,i))+u(n1a+1,j2,i)
          u(n1b+2,j2,i)=3.d0*(u(n1b+1,j2,i)-u(n1b,j2,i))+u(n1b-1,j2,i)
        end do
        do j1=n1a-2,n1b+2
c          u(j1,n2a-2,i)=2.d0*u(j1,n2a-1,i)-u(j1,n2a,i)
c          u(j1,n2b+2,i)=2.d0*u(j1,n2b+1,i)-u(j1,n2b,i)
          u(j1,n2a-2,i)=3.d0*(u(j1,n2a-1,i)-u(j1,n2a,i))+u(j1,n2a+1,i)
          u(j1,n2b+2,i)=3.d0*(u(j1,n2b+1,i)-u(j1,n2b,i))+u(j1,n2b-1,i)
        end do
      end do
c
c      call corner (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
c     *             ds1,ds2,t,xy,u)
c
      if (dabs(t-.5d0).lt.1.d-8) then
        call smgerr (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *               ds1,ds2,t,xy,u)
      end if
c
      return
      end      
c
c+++++++++++++++++++++++
c
      subroutine corner0 (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                   ds1,ds2,t,xy,u)
c
c variables: u(j1,j2,1:2)=velocity
c            u(j1,j2,3:6)=stress (S11,S12,S21,S22)
c            u(j1,j2,7:8)=displacement
c
      implicit real*8 (a-h,o-z)
      dimension xy(nd1a:nd1b,nd2a:nd2b,2),u(nd1a:nd1b,nd2a:nd2b,m)
      common / smgdat / amu,alam,rho0
      common / tzflow / eptz,itz
c
      j1dif=n1b-n1a+2
      j2dif=n2b-n2a+2
c      write(6,*)j1dif,j2dif
      do j1=n1a-1,n1b+1,j1dif
      do j2=n2a-1,n2b+1,j2dif
c        write(6,*)j1,j2,xy(j1,j2,1),xy(j1,j2,2),t
c        pause
        do i=1,m
          call ogDeriv (eptz,0,0,0,0,xy(j1,j2,1),xy(j1,j2,2),0.d0,t,
     *                  i-1,u(j1,j2,i))
        end do
      end do
      end do
c
      return
      end      
c
c+++++++++++++++++++++++
c
      subroutine cstress (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                    ds1,ds2,t,xy,mask,rx,u,icart)
c
c variables: u(j1,j2,1:2)=velocity
c            u(j1,j2,3:6)=stress (S11,S12,S21,S22)
c            u(j1,j2,7:8)=displacement
c
      implicit real*8 (a-h,o-z)
      dimension xy(nd1a:nd1b,nd2a:nd2b,2),mask(nd1a:nd1b,nd2a:nd2b),
     *          rx(nd1a:nd1b,nd2a:nd2b,2,2),u(nd1a:nd1b,nd2a:nd2b,m)
      dimension ue(8),eee(8)
      common / smgdat / amu,alam,rho0
      common / tzflow / eptz,itz
c
c..set components of stress in the corners
      if (.true.) then
        j1dif=n1b-n1a
        j2dif=n2b-n2a
        if (icart.eq.1) then
          do j1=n1a,n1b,j1dif
          do j2=n2a,n2b,j2dif
            if (mask(j1,j2).ne.0) then
              u1x=(u(j1+1,j2,7)-u(j1-1,j2,7))/(2.d0*ds1)
              u2x=(u(j1+1,j2,8)-u(j1-1,j2,8))/(2.d0*ds1)
              u1y=(u(j1,j2+1,7)-u(j1,j2-1,7))/(2.d0*ds2)
              u2y=(u(j1,j2+1,8)-u(j1,j2-1,8))/(2.d0*ds2)
              u(j1,j2,3)=alam*(u1x+u2y)+2.d0*amu*u1x
              u(j1,j2,4)=amu*(u1y+u2x)
              u(j1,j2,5)=amu*(u1y+u2x)
              u(j1,j2,6)=alam*(u1x+u2y)+2.d0*amu*u2y
            end if
          end do
          end do
        else
          do j1=n1a,n1b,j1dif
          do j2=n2a,n2b,j2dif
            if (mask(j1,j2).ne.0) then
              u1r=(u(j1+1,j2,7)-u(j1-1,j2,7))/(2.d0*ds1)
              u2r=(u(j1+1,j2,8)-u(j1-1,j2,8))/(2.d0*ds1)
              u1s=(u(j1,j2+1,7)-u(j1,j2-1,7))/(2.d0*ds2)
              u2s=(u(j1,j2+1,8)-u(j1,j2-1,8))/(2.d0*ds2)
c         write(6,222)j1,j2,u(j1-1,j2,7),u(j1+1,j2,7),u(j1,j2-1,7),
c     * u(j1,j2+1,7),u(j1-1,j2,8),u(j1+1,j2,8),u(j1,j2-1,8),u(j1,j2+1,8)
c  222 format(2(1x,i2),4(1x,1pe15.8),/,6x,4(1x,1pe15.8))
              u1x=u1r*rx(j1,j2,1,1)+u1s*rx(j1,j2,2,1)
              u2x=u2r*rx(j1,j2,1,1)+u2s*rx(j1,j2,2,1)
              u1y=u1r*rx(j1,j2,1,2)+u1s*rx(j1,j2,2,2)
              u2y=u2r*rx(j1,j2,1,2)+u2s*rx(j1,j2,2,2)
c          write(6,333)u1x,u1y,u2x,u2y
c  333 format(6x,4(1x,1pe15.8))
              u(j1,j2,3)=alam*(u1x+u2y)+2.d0*amu*u1x
              u(j1,j2,4)=amu*(u1y+u2x)
              u(j1,j2,5)=amu*(u1y+u2x)
              u(j1,j2,6)=alam*(u1x+u2y)+2.d0*amu*u2y
            end if
          end do
          end do
c      pause
        end if
        if (itz.ne.0) then
          do i=3,6
            eee(i)=0.d0
          end do
          do j1=n1a,n1b,j1dif
          do j2=n2a,n2b,j2dif
            if (mask(j1,j2).ne.0) then
              do i=3,6
                call ogDeriv (eptz,0,0,0,0,xy(j1,j2,1),xy(j1,j2,2),
     *                        0.d0,t,i-1,ue(i))
              end do
              call ogDeriv (eptz,0,1,0,0,xy(j1,j2,1),xy(j1,j2,2),0.d0,
     *                      t,6,u1x)
              call ogDeriv (eptz,0,1,0,0,xy(j1,j2,1),xy(j1,j2,2),0.d0,
     *                      t,7,u2x)
              call ogDeriv (eptz,0,0,1,0,xy(j1,j2,1),xy(j1,j2,2),0.d0,
     *                      t,6,u1y)
              call ogDeriv (eptz,0,0,1,0,xy(j1,j2,1),xy(j1,j2,2),0.d0,
     *                      t,7,u2y)
c          write(6,333)u1x,u1y,u2x,u2y
c      eee(3)=max(dabs(u(j1,j2,3)-(alam*(u1x+u2y)+2.d0*amu*u1x)),eee(3))
c      eee(4)=max(dabs(u(j1,j2,4)-(amu*(u1y+u2x))),eee(4))
c      eee(5)=max(dabs(u(j1,j2,5)-(amu*(u1y+u2x))),eee(5))
c      eee(6)=max(dabs(u(j1,j2,6)-(alam*(u1x+u2y)+2.d0*amu*u2y)),eee(6))
              u(j1,j2,3)=u(j1,j2,3)+ue(3)-(alam*(u1x+u2y)+2.d0*amu*u1x)
              u(j1,j2,4)=u(j1,j2,4)+ue(4)-(amu*(u1y+u2x))
              u(j1,j2,5)=u(j1,j2,5)+ue(5)-(amu*(u1y+u2x))
              u(j1,j2,6)=u(j1,j2,6)+ue(6)-(alam*(u1x+u2y)+2.d0*amu*u2y)
c              u(j1,j2,3)=ue(3)
c              u(j1,j2,4)=ue(4)
c              u(j1,j2,5)=ue(5)
c              u(j1,j2,6)=ue(6)
            end if
          end do
          end do
c          write(6,*)(eee(i),i=3,6)
c          pause
        end if
      end if
c
      return
      end      
c
c+++++++++++++++++++++++
c
      subroutine corner (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                   ds1,ds2,t,xy,mask,rx,u,icart)
c
c variables: u(j1,j2,1:2)=velocity
c            u(j1,j2,3:6)=stress (S11,S12,S21,S22)
c            u(j1,j2,7:8)=displacement
c
      implicit real*8 (a-h,o-z)
      dimension xy(nd1a:nd1b,nd2a:nd2b,2),mask(nd1a:nd1b,nd2a:nd2b),
     *          rx(nd1a:nd1b,nd2a:nd2b,2,2),u(nd1a:nd1b,nd2a:nd2b,m)
      dimension ue(8)
      common / smgdat / amu,alam,rho0
      common / tzflow / eptz,itz
c
c..set components of stress in the corners
      if (.false.) then
        j1dif=n1b-n1a
        j2dif=n2b-n2a
        if (icart.eq.1) then
          do j1=n1a,n1b,j1dif
          do j2=n2a,n2b,j2dif
            if (mask(j1,j2).ne.0) then
              u1x=(u(j1+1,j2,7)-u(j1-1,j2,7))/(2.d0*ds1)
              u2x=(u(j1+1,j2,8)-u(j1-1,j2,8))/(2.d0*ds1)
              u1y=(u(j1,j2+1,7)-u(j1,j2-1,7))/(2.d0*ds2)
              u2y=(u(j1,j2+1,8)-u(j1,j2-1,8))/(2.d0*ds2)
              u(j1,j2,3)=alam*(u1x+u2y)+2.d0*amu*u1x
              u(j1,j2,4)=amu*(u1y+u2x)
              u(j1,j2,5)=amu*(u1y+u2x)
              u(j1,j2,6)=alam*(u1x+u2y)+2.d0*amu*u2y
            end if
          end do
          end do
        else
          do j1=n1a,n1b,j1dif
          do j2=n2a,n2b,j2dif
            if (mask(j1,j2).ne.0) then
              u1r=(u(j1+1,j2,7)-u(j1-1,j2,7))/(2.d0*ds1)
              u2r=(u(j1+1,j2,8)-u(j1-1,j2,8))/(2.d0*ds1)
              u1s=(u(j1,j2+1,7)-u(j1,j2-1,7))/(2.d0*ds2)
              u2s=(u(j1,j2+1,8)-u(j1,j2-1,8))/(2.d0*ds2)
              u1x=u1r*rx(j1,j2,1,1)+u1s*rx(j1,j2,2,1)
              u2x=u2r*rx(j1,j2,1,1)+u2s*rx(j1,j2,2,1)
              u1y=u1r*rx(j1,j2,1,2)+u1s*rx(j1,j2,2,2)
              u2y=u2r*rx(j1,j2,1,2)+u2s*rx(j1,j2,2,2)
              u(j1,j2,3)=alam*(u1x+u2y)+2.d0*amu*u1x
              u(j1,j2,4)=amu*(u1y+u2x)
              u(j1,j2,5)=amu*(u1y+u2x)
              u(j1,j2,6)=alam*(u1x+u2y)+2.d0*amu*u2y
            end if
          end do
          end do
        end if
        if (itz.ne.0) then
          do j1=n1a,n1b,j1dif
          do j2=n2a,n2b,j2dif
            if (mask(j1,j2).ne.0) then
              do i=3,6
                call ogDeriv (eptz,0,0,0,0,xy(j1,j2,1),xy(j1,j2,2),
     *                        0.d0,t,i-1,ue(i))
              end do
              call ogDeriv (eptz,0,1,0,0,xy(j1,j2,1),xy(j1,j2,2),0.d0,
     *                      t,6,u1x)
              call ogDeriv (eptz,0,1,0,0,xy(j1,j2,1),xy(j1,j2,2),0.d0,
     *                      t,7,u2x)
              call ogDeriv (eptz,0,0,1,0,xy(j1,j2,1),xy(j1,j2,2),0.d0,
     *                      t,6,u1y)
              call ogDeriv (eptz,0,0,1,0,xy(j1,j2,1),xy(j1,j2,2),0.d0,
     *                      t,7,u2y)
              u(j1,j2,3)=u(j1,j2,3)+ue(3)-(alam*(u1x+u2y)+2.d0*amu*u1x)
              u(j1,j2,4)=u(j1,j2,4)+ue(4)-(amu*(u1y+u2x))
              u(j1,j2,5)=u(j1,j2,5)+ue(5)-(amu*(u1y+u2x))
              u(j1,j2,6)=u(j1,j2,6)+ue(6)-(alam*(u1x+u2y)+2.d0*amu*u2y)
              u(j1,j2,3)=ue(3)
              u(j1,j2,4)=ue(4)
              u(j1,j2,5)=ue(5)
              u(j1,j2,6)=ue(6)
            end if
          end do
          end do
        end if
      end if
c
c..extrapolate corners
      if (.false.) then
        do i=1,m
          u(n1a-1,n2a-1,i)=3.d0*(u(n1a,n2a,i)-u(n1a+1,n2a+1,i))
     *                      +u(n1a+2,n2a+2,i)
          u(n1a-1,n2b+1,i)=3.d0*(u(n1a,n2b,i)-u(n1a+1,n2b-1,i))
     *                      +u(n1a+2,n2b-2,i)
          u(n1b+1,n2a-1,i)=3.d0*(u(n1b,n2a,i)-u(n1b-1,n2a+1,i))
     *                      +u(n1b-2,n2a+2,i)
          u(n1b+1,n2b+1,i)=3.d0*(u(n1b,n2b,i)-u(n1b-1,n2b-1,i))
     *                      +u(n1b-2,n2b-2,i)
        end do
      end if
c
c..extrapolate near corner stresses
      if (.false.) then
        j1dif=n1b-n1a
        j2dif=n2b-n2a
        do i=3,6
          do j2=n2a,n2b,j2dif
c            u(n1a-1,j2,i)=2.d0*u(n1a,j2,i)-u(n1a+1,j2,i)
c            u(n1b+1,j2,i)=2.d0*u(n1b,j2,i)-u(n1b-1,j2,i)
            u(n1a-1,j2,i)=3.d0*(u(n1a,j2,i)-u(n1a+1,j2,i))+u(n1a+2,j2,i)
            u(n1b+1,j2,i)=3.d0*(u(n1b,j2,i)-u(n1b-1,j2,i))+u(n1b-2,j2,i)
          end do
          do j1=n1a,n1b,j1dif
c            u(j1,n2a-1,i)=2.d0*u(j1,n2a,i)-u(j1,n2a+1,i)
c            u(j1,n2b+1,i)=2.d0*u(j1,n2b,i)-u(j1,n2b-1,i)
            u(j1,n2a-1,i)=3.d0*(u(j1,n2a,i)-u(j1,n2a+1,i))+u(j1,n2a+2,i)
            u(j1,n2b+1,i)=3.d0*(u(j1,n2b,i)-u(j1,n2b-1,i))+u(j1,n2b-2,i)
          end do
        end do
      end if
c
c..extrapolate second ghost lines
      if (.false.) then
        do i=1,m
        do j1=n1a-1,n1b+1
          u(j1,n2a-2,i)=3.d0*(u(j1,n2a-1,i)-u(j1,n2a,i))+u(j1,n2a+1,i)
          u(j1,n2b+2,i)=3.d0*(u(j1,n2b+1,i)-u(j1,n2b,i))+u(j1,n2b-1,i)
        end do
        do j2=n2a-2,n2b+2
          u(n1a-2,j2,i)=3.d0*(u(n1a-1,j2,i)-u(n1a,j2,i))+u(n1a+1,j2,i)
          u(n1b+2,j2,i)=3.d0*(u(n1b+1,j2,i)-u(n1b,j2,i))+u(n1b-1,j2,i)
        end do
        end do
      end if
c
c..fill in exact values for displacement and velocity on the boundary
      if (.false.) then
        do i=1,2
        do j1=n1a,n1b
          call ogDeriv (eptz,0,0,0,0,xy(j1,n2a,1),xy(j1,n2a,2),
     *                  0.d0,t,i-1,u(j1,n2a,i))
          call ogDeriv (eptz,0,0,0,0,xy(j1,n2b,1),xy(j1,n2b,2),
     *                  0.d0,t,i-1,u(j1,n2b,i))
        end do
        end do
        do i=7,8
        do j1=n1a,n1b
          call ogDeriv (eptz,0,0,0,0,xy(j1,n2a,1),xy(j1,n2a,2),
     *                  0.d0,t,i-1,u(j1,n2a,i))
          call ogDeriv (eptz,0,0,0,0,xy(j1,n2b,1),xy(j1,n2b,2),
     *                  0.d0,t,i-1,u(j1,n2b,i))
        end do
        end do
        do i=1,2
        do j2=n2a,n2b
          call ogDeriv (eptz,0,0,0,0,xy(n1a,j2,1),xy(n1a,j2,2),
     *                  0.d0,t,i-1,u(n1a,j2,i))
          call ogDeriv (eptz,0,0,0,0,xy(n1b,j2,1),xy(n1b,j2,2),
     *                  0.d0,t,i-1,u(n1b,j2,i))
        end do
        end do
        do i=7,8
        do j2=n2a,n2b
          call ogDeriv (eptz,0,0,0,0,xy(n1a,j2,1),xy(n1a,j2,2),
     *                  0.d0,t,i-1,u(n1a,j2,i))
          call ogDeriv (eptz,0,0,0,0,xy(n1b,j2,1),xy(n1b,j2,2),
     *                  0.d0,t,i-1,u(n1b,j2,i))
        end do
        end do
      end if
c
c..fill in exact values in the first ghost lines (without corners)
      if (.false.) then
        do i=1,m
c        if (i.ne.5.and.i.ne.6) then
        do j1=n1a,n1b
          call ogDeriv (eptz,0,0,0,0,xy(j1,n2a-1,1),xy(j1,n2a-1,2),
     *                  0.d0,t,i-1,u(j1,n2a-1,i))
          call ogDeriv (eptz,0,0,0,0,xy(j1,n2b+1,1),xy(j1,n2b+1,2),
     *                  0.d0,t,i-1,u(j1,n2b+1,i))
        end do
c        end if
        end do
        do i=1,m
c        if (i.ne.3.and.i.ne.4) then
        do j2=n2a,n2b
          call ogDeriv (eptz,0,0,0,0,xy(n1a-1,j2,1),xy(n1a-1,j2,2),
     *                  0.d0,t,i-1,u(n1a-1,j2,i))
          call ogDeriv (eptz,0,0,0,0,xy(n1b+1,j2,1),xy(n1b+1,j2,2),
     *                  0.d0,t,i-1,u(n1b+1,j2,i))
        end do
c        end if
        end do
      end if
c
c..fill in exact values in the corners
      if (.true.) then
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
      end if
c
c..fill in exact values in the second ghost line
      if (.true.) then
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
      end if
c
      return
      end      
c
c+++++++++++++++++++++++
c
      subroutine corner1 (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                   ds1,ds2,t,xy,u)
c
c this version was the latest one used with smgbcs
c
c variables: u(j1,j2,1:2)=velocity
c            u(j1,j2,3:6)=stress (S11,S12,S21,S22)
c            u(j1,j2,7:8)=displacement
c
      implicit real*8 (a-h,o-z)
      dimension xy(nd1a:nd1b,nd2a:nd2b,2),u(nd1a:nd1b,nd2a:nd2b,m)
      common / smgdat / amu,alam,rho0
      common / tzflow / eptz,itz
c
c..extrapolate corners
      if (.false.) then
c
        do i=1,m
          u(n1a-1,n2a-1,i)=3.d0*(u(n1a,n2a,i)-u(n1a+1,n2a+1,i))
     *                      +u(n1a+2,n2a+2,i)
          u(n1a-1,n2b+1,i)=3.d0*(u(n1a,n2b,i)-u(n1a+1,n2b-1,i))
     *                      +u(n1a+2,n2b-2,i)
          u(n1b+1,n2a-1,i)=3.d0*(u(n1b,n2a,i)-u(n1b-1,n2a+1,i))
     *                      +u(n1b-2,n2a+2,i)
          u(n1b+1,n2b+1,i)=3.d0*(u(n1b,n2b,i)-u(n1b-1,n2b-1,i))
     *                      +u(n1b-2,n2b-2,i)
        end do
c
      end if
c
c..extrapolate second ghost lines
      if (.false.) then
c
        do i=1,m
        do j1=n1a-1,n1b+1
          u(j1,n2a-2,i)=3.d0*(u(j1,n2a-1,i)-u(j1,n2a,i))+u(j1,n2a+1,i)
          u(j1,n2b+2,i)=3.d0*(u(j1,n2b+1,i)-u(j1,n2b,i))+u(j1,n2b-1,i)
        end do
        do j2=n2a-2,n2b+2
          u(n1a-2,j2,i)=3.d0*(u(n1a-1,j2,i)-u(n1a,j2,i))+u(n1a+1,j2,i)
          u(n1b+2,j2,i)=3.d0*(u(n1b+1,j2,i)-u(n1b,j2,i))+u(n1b-1,j2,i)
        end do
        end do
c
      end if
c
c..fill in exact values in the first ghost lines (without corners)
      if (.false.) then
c
        do i=1,m
c        if (i.ne.5.and.i.ne.6) then
        do j1=n1a,n1b
          call ogDeriv (eptz,0,0,0,0,xy(j1,n2a-1,1),xy(j1,n2a-1,2),
     *                  0.d0,t,i-1,u(j1,n2a-1,i))
          call ogDeriv (eptz,0,0,0,0,xy(j1,n2b+1,1),xy(j1,n2b+1,2),
     *                  0.d0,t,i-1,u(j1,n2b+1,i))
        end do
c        end if
        end do
c
        do i=1,m
c        if (i.ne.3.and.i.ne.4) then
        do j2=n2a,n2b
          call ogDeriv (eptz,0,0,0,0,xy(n1a-1,j2,1),xy(n1a-1,j2,2),
     *                  0.d0,t,i-1,u(n1a-1,j2,i))
          call ogDeriv (eptz,0,0,0,0,xy(n1b+1,j2,1),xy(n1b+1,j2,2),
     *                  0.d0,t,i-1,u(n1b+1,j2,i))
        end do
c        end if
        end do
c
      end if
c
c..fill in exact values in the corners
      if (.false.) then
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
      if (.false.) then
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
      return
      end      
c
c+++++++++++++++++++++++
c
      subroutine smgerr (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                   ds1,ds2,t,xy,u)
c
c variables: u(j1,j2,1:2)=velocity
c            u(j1,j2,3:6)=stress (S11,S12,S21,S22)
c            u(j1,j2,7:8)=displacement
c
      implicit real*8 (a-h,o-z)
      dimension xy(nd1a:nd1b,nd2a:nd2b,2),u(nd1a:nd1b,nd2a:nd2b,m)
      dimension err(8)
      common / smgdat / amu,alam,rho0
      common / tzflow / eptz,itz
c
      if (m.ne.8) then
        write(6,*)'Error (smgerr) : m.ne.8'
        stop
      end if
c
      do i=1,m
        err(i)=0.d0
        do j1=n1a,n1b
        do j2=n2a,n2b
          call ogDeriv (eptz,0,0,0,0,xy(j1,j2,1),xy(j1,j2,2),0.d0,t,
     *                  i-1,ue)
          err(i)=max(dabs(u(j1,j2,i)-ue),err(i))
        end do
        end do
      end do
c
      write(6,100)(err(i),i=1,m)
  100 format('** smgerr:  errors=[',8(1pe10.3,1x),'] **')
c
      return
      end
c
c+++++++++++++++++++++++
c
      subroutine corner2 (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                   ds1,ds2,t,xy,u)
c
c variables: u(j1,j2,1:2)=velocity
c            u(j1,j2,3:6)=stress (S11,S12,S21,S22)
c            u(j1,j2,7:8)=displacement
c
      implicit real*8 (a-h,o-z)
      dimension xy(nd1a:nd1b,nd2a:nd2b,2),u(nd1a:nd1b,nd2a:nd2b,m)
      common / smgdat / amu,alam,rho0
      common / tzflow / eptz,itz
c
      if (m.ne.8) then
        write(6,*)'Error (corner) : m.ne.8'
        stop
      end if
c
      do i=1,m
        do j1=n1a-2,n1b+2
        do j2=n2a-2,n2a
          call ogDeriv (eptz,0,0,0,0,xy(j1,j2,1),xy(j1,j2,2),0.d0,t,
     *                  i-1,u(j1,j2,i))
        end do
        end do
        do j1=n1a-2,n1b+2
        do j2=n2b,n2b+2
          call ogDeriv (eptz,0,0,0,0,xy(j1,j2,1),xy(j1,j2,2),0.d0,t,
     *                  i-1,u(j1,j2,i))
        end do
        end do
        do j1=n1a-2,n1a
        do j2=n2a-2,n2b+2
          call ogDeriv (eptz,0,0,0,0,xy(j1,j2,1),xy(j1,j2,2),0.d0,t,
     *                  i-1,u(j1,j2,i))
        end do
        end do
        do j1=n1b,n1b+2
        do j2=n2a-2,n2b+2
          call ogDeriv (eptz,0,0,0,0,xy(j1,j2,1),xy(j1,j2,2),0.d0,t,
     *                  i-1,u(j1,j2,i))
        end do
        end do
      end do
c
      return
      end
c
c+++++++++++++++++++++++
c
      subroutine stressRelax2d( m,nd1a,nd1b,nd2a,nd2b,
     *                            n1a, n1b, n2a, n2b, 
     *                            ds1,ds2,dt,t,xy,rx,u,up,mask,
     *                            iparam,rparam )
c
      implicit real*8 (a-h,o-z)
      dimension xy(nd1a:nd1b,nd2a:nd2b,2),rx(nd1a:nd1b,nd2a:nd2b,2,2),
     *          u(nd1a:nd1b,nd2a:nd2b,m),
     *          up(nd1a:nd1b,nd2a:nd2b,m),mask(nd1a:nd1b,nd2a:nd2b),
     *          rparam(*),iparam(*)
      common / smgprm / method,iorder,ilimit,iupwind,icart,ifrc
      common / tzflow / eptz,itz
c
      iRelax = iparam(9)

      amu        = rparam(3)
      alam       = rparam(4)
      relaxAlpha = rparam(7)
      relaxDelta = rparam(8)

      beta = relaxAlpha+relaxDelta/dt
      akappa = alam+2.0*amu
      if( icart.eq.1 ) then
        do j2 = n2a,n2b
        do j1 = n1a,n1b
          if( mask(j1,j2).ne.0 ) then
            if( iRelax.eq.2 ) then
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
            
            s11 = akappa*u1x+alam*u2y
            s12 = amu*(u2x+u1y)
            s21 = s12
            s22 = alam*u1x+akappa*u2y
            
            up(j1,j2,3) = up(j1,j2,3)+beta*(-u(j1,j2,3)+s11)
            up(j1,j2,4) = up(j1,j2,4)+beta*(-u(j1,j2,4)+s12)
            up(j1,j2,5) = up(j1,j2,5)+beta*(-u(j1,j2,5)+s21)
            up(j1,j2,6) = up(j1,j2,6)+beta*(-u(j1,j2,6)+s22)
          end if
        end do
        end do
      else
        do j2 = n2a,n2b
        do j1 = n1a,n1b
          if( mask(j1,j2).ne.0 ) then
            if( iRelax.eq.2 ) then
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
            
            u1x = u1r*rx(j1,j2,1,1)+u1s*rx(j1,j2,2,1)
            u2x = u2r*rx(j1,j2,1,1)+u2s*rx(j1,j2,2,1)
            u1y = u1r*rx(j1,j2,1,2)+u1s*rx(j1,j2,2,2)
            u2y = u2r*rx(j1,j2,1,2)+u2s*rx(j1,j2,2,2)
            
            s11 = akappa*u1x+alam*u2y
            s12 = amu*(u2x+u1y)
            s21 = s12
            s22 = alam*u1x+akappa*u2y
            
            up(j1,j2,3) = up(j1,j2,3)+beta*(-u(j1,j2,3)+s11)
            up(j1,j2,4) = up(j1,j2,4)+beta*(-u(j1,j2,4)+s12)
            up(j1,j2,5) = up(j1,j2,5)+beta*(-u(j1,j2,5)+s21)
            up(j1,j2,6) = up(j1,j2,6)+beta*(-u(j1,j2,6)+s22)
          end if
        end do
        end do
      end if
      ! add twilight zone contribution
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
            call ogDeriv( eptz,0,1,0,0,x,y,0.d0,t,iu1c,u1xt )
            call ogDeriv( eptz,0,0,1,0,x,y,0.d0,t,iu1c,u1yt )
            call ogDeriv( eptz,0,1,0,0,x,y,0.d0,t,iu2c,u2xt )
            call ogDeriv( eptz,0,0,1,0,x,y,0.d0,t,iu2c,u2yt )
            
            call ogDeriv( eptz,0,0,0,0,x,y,0.d0,t,is11c,s11t )
            call ogDeriv( eptz,0,0,0,0,x,y,0.d0,t,is12c,s12t )
            call ogDeriv( eptz,0,0,0,0,x,y,0.d0,t,is21c,s21t )
            call ogDeriv( eptz,0,0,0,0,x,y,0.d0,t,is22c,s22t )
            
            s11 = akappa*u1xt+alam*u2yt
            s12 = amu*(u2xt+u1yt)
            s21 = s12
            s22 = alam*u1xt+akappa*u2yt
            
            up(j1,j2,3) = up(j1,j2,3)-beta*(-s11t+s11)
            up(j1,j2,4) = up(j1,j2,4)-beta*(-s12t+s12)
            up(j1,j2,5) = up(j1,j2,5)-beta*(-s21t+s21)
            up(j1,j2,6) = up(j1,j2,6)-beta*(-s22t+s22)
          end if
        end do
        end do
      end if
c
      return
      end
