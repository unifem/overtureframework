      subroutine cmfdu (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                  dr,ds1,ds2,r,rx,gv,det,rx2,gv2,det2,xy,
     *                  u,up,mask,ntau,tau,ad,mdat,dat,nrprm,rparam,
     *                  niprm,iparam,nrwk,rwk,niwk,iwk,idebug,pdb,ier)
c
c Compressible Reactive Multifluid (reactive Euler equations with advected scalars)
c
      implicit real*8 (a-h,o-z)
      dimension rx(*),gv(*),det(*),rx2(*),gv2(*),det2(*),xy(*),
     *          u(nd1a:nd1b,nd2a:nd2b,m),up(nd1a:nd1b,nd2a:nd2b,m),
     *          mask(nd1a:nd1b,nd2a:nd2b),tau(ntau),ad(m),
     *          dat(nd1a:nd1b,nd2a:nd2b,*),rparam(nrprm),iparam(niprm),
     *          rwk(nrwk),iwk(niwk)
      dimension ds(2),almax(2)
      logical iplimit
      double precision pdb  ! pointer to data base
      integer ok,getInt,getReal
c
      common / cmftime / tflux,tslope,tsource
      common / axidat / iaxi,j1axi(2),j2axi(2)
      common / rundat / ieos,irxn
      common / tzflow / eptz,itz
      common / srcprm / sigma,pign,npower
      common / filter / tfilter,xfilter,ufilter(4)

      common / gdinfo / igrid,level

c
c limit pressure ?
      data iplimit / .true. /
c
c      write(6,*)'cmfdu start...'
c      write(6,343)n1a,n1b,n2a,n2b,r
c  343 format('** Start cmfdu: ',4(1x,i4),', time =',f15.8)
c
c..set array dimensions for parallel
      md1a=max(nd1a,n1a-2)
      md1b=min(nd1b,n1b+2)
      md2a=max(nd2a,n2a-2)
      md2b=min(nd2b,n2b+2)
c
c..set error flag
      ier=0
c
      if (m.lt.0) then
        do i=1,m
          write(55,411)i
  411     format(i2)
          do j2=nd2a,nd2b
          do j1=nd1a,nd1b
            write(55,412)j1,j2,u(j1,j2,i)
  412       format(2(1x,i2),1x,1pe15.8)
          end do
          end do
        end do
        stop
      end if
c
      if (m.lt.0) then
        small=1.d-8
        if (r.ge.0.056d0-small) then
          do j2=nd2a,nd2b
          do j1=nd1a,nd1b
            write(77,777)(u(j1,j2,i),i=1,6)
  777       format(6(1x,1pe15.8))
          end do
          end do
          write(6,*)'Writing fort.77, stop ...'
          stop
        end if
      end if
c
      if( idebug.gt.0 ) write(6,990)r,dr
c     write(6,990)r,dr
  990 format('** Starting cmfdu...t,dt =',f8.5,1x,1pe9.2)
c     write(6,991)n1a,n1b,n2a,n2b
c 991 format('      Grid bounds =',4(1x,i5))
c
c..mesh spacings
      ds(1)=ds1
      ds(2)=ds2
c
c..zero out timings
      tflux=0.d0
      tslope=0.d0
      tsource=0.d0
c
c..parameters
c
c    iparam(1) =EOS model          (iparam(1)=0 => stiffened gases)
c    iparam(2) =Reaction model     (iparam(2)=0 => no chemical reaction)
c    iparam(3) =move               is the grid moving (=0 => no)
c    iparam(4) =icart              Cartesian grid (=1 => yes)
c    iparam(5) =iorder             order of the method (=1 or 2)
c    iparam(6) =method             method of flux calculation (=0 => exact Riemann solver)
c    iparam(7) =igrid              grid number
c    iparam(8) =level              AMR level for this grid
c    iparam(9) =nstep              number of steps taken
c    iparam(10)=icount             the maximum number of sub-time steps is determined
c    iparam(11)=iaxi               Axisymmetric problem? 0=>no, 1=>axisymmetric about grid line j1=j1axi
c                                                               2=>axisymmetric about grid line j2=j2axi
c    iparam(12)=j1axi(1) or j2axi(1)
c    iparam(13)=j1axi(2) or j2axi(2)
c
c    rparam(1) =real(eigenvalue)   for time stepping
c    rparam(2) =imag(eigenvalue)   for time stepping
c    rparam(3) =viscosity          artificial viscosity
c
c   timings
c    rparam(31)=tflux
c    rparam(32)=tslope
c    rparam(33)=tsourcer
c
      ieos=iparam(1)
      irxn=iparam(2)
c
c check currently supported values for ieos and irxn, and check
c that the corresponding value for m is the expected one.
      if (ieos.eq.4) then
        if (irxn.eq.0) then
          if (m.ne.6) then
            write(6,*)'Error (cmfdu) : m must equal 6 ',
     *                'for stiffened gas with no reaction'
            stop
          end if
        elseif (irxn.eq.9) then
          if (m.ne.10) then
            write(6,*)'Error (cmfdu) : m must equal 10 ',
     *                'for stiffened gas with reaction'
            stop
          end if
        else
          write(6,*)'Error (cmfdu) : reaction rate not supported, ',
     *              'irxn =',irxn
          stop
        end if
      else
        write(6,*)'Error (cmfdu) : EOS not supported, ',
     *            'ieos =',ieos
        stop
      end if
c
c      write(6,*)(iparam(k),k=3,13)
c      pause
c
      move=iparam(3)
      icart=iparam(4)
      iorder=iparam(5)
      method=iparam(6)
      igrid=iparam(7)
      level=iparam(8)
      nstep=iparam(9)
      itz=iparam(14)
      av=rparam(3)
      eptz=rparam(14)
c
c      write(6,*)ieos,irxn,icart,iorder,method
c      pause
c
c      write(6,943)r,nstep,igrid,level
c  943 format('** Start cmfdu: t=',1pe12.5,'  nstep=',i3,'  grid=',i3,
c     *       '  level=',i2)
c
c      write(6,*)'ad =',ad
c      write(6,*)'av =',av
c      pause
c
c     write(6,*)'Warning (cmfdu) : setting iorder=2'
c     iorder=2
c
c..reduce the order for the pre-step to establish the time step
      if (nstep.lt.0) iorder=1
c
c..may want to reduce the order to help smooth out sharp ICs.
c     if (nstep.le.0) then
c       iorder=1
c     end if
c
c..may want to override the input choice for method and iorder.
c     method=0
c     iorder=1
c
c     write(6,*)'iorder=',iorder
c
c get filtering parameters from database
      if (nstep.le.0) then
c
c default values
        tfilter=-1.d4
        xfilter=0.d0
        do i=1,4
          ufilter(i)=0.d0
        end do
c
        ! parameters created in the command file, e.g., 
        !     define real parameter cmfReactionRate   1.0
        ok=getReal(pdb,'PdeParameters/cmfFilterTime',tfilter)
        ok=getReal(pdb,'PdeParameters/cmfFilterPosition',xfilter)
        ok=getReal(pdb,'PdeParameters/cmfFilterDensity',ufilter(1))
        ok=getReal(pdb,'PdeParameters/cmfFilterXMomentum',ufilter(2))
        ok=getReal(pdb,'PdeParameters/cmfFilterYMomentum',ufilter(3))
        ok=getReal(pdb,'PdeParameters/cmfFilterEnergy',ufilter(4))
      end if
c
c
c if irxn.ne.0, then get reaction parameters from database
      if (irxn.ne.0.and.nstep.le.0) then
c
c default values
        sigma=0.d0
        pign=0.d0
        npower=1
c
        ! parameters created in the command file, e.g., 
        !     define real parameter cmfReactionRate   1.0
        ok=getReal(pdb,'PdeParameters/cmfReactionRate',sigma)
        ok=getReal(pdb,'PdeParameters/cmfIgnitionPressure',pign)
        ok=getInt(pdb,'PdeParameters/cmfPressureRatePower',npower)
      end if
c
c..set boundaries of mapping variables depending on whether
c  the grid is Cartesian (icart=1) or not (icart=0)
      if (icart.eq.0) then
        n1bm=nd1b
        n2bm=nd2b
      else
        n1bm=nd1a
        n2bm=nd2a
      end if
c
c      write(6,*)'iaxi=',iparam(11)
c      pause
c
c..axisymmetric problem?
      iaxi=iparam(11)
      if (iaxi.eq.1) then
        j1axi(1)=iparam(12)
        j1axi(2)=iparam(13)
        j2axi(1)=nd2a-1
        j2axi(2)=nd2b+1
      else
        j1axi(1)=nd1a-1
        j1axi(2)=nd1b+1
        j2axi(1)=iparam(12)
        j2axi(2)=iparam(13)
      end if
c      write(6,*)iaxi,j1axi,j2axi
c      pause
c      iaxi=2
c      j2axi(1)=0
c
c..check the mask (if idebug>0)
c    mask=0 => unused point
c    mask>0 => interior discretization point
c    mask<0 => interpolation point
c  ** points with mask>0 cannot be next to points with mask=0 **
      if (idebug.gt.0) then
        do j2=n2a,n2b
        do j1=n1a,n1b
          if (mask(j1,j2).gt.0) then
            do k2=j2-1,j2+1
            do k1=j1-1,j1+1
              if (mask(k1,k2).eq.0) then
                write(6,*)'Error (cmfdu) : inconsistent mask value'
              end if
            end do
            end do
          end if
        end do
        end do
      end if
c
c..monitor data (if idebug>0)
c      if (idebug.gt.0) then
c        call mondat2d (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
c     *                 r,dr,u,rwk,mask)
c      end if
c
      rhoMin=1.d-8
      do j2=md2a,md2b
      do j1=md1a,md1b
        if (mask(j1,j2).ne.0) then
          if (u(j1,j2,1).le.rhoMin) then
            write(6,110)j1,j2
  110       format(' ** Error (cmfdu) : very small density, ',
     *             'j1,j2 =',2(1x,i4))
            u(j1,j2,1)=rhoMin
          end if
        end if
      end do
      end do
c
      ngrid=(md1b-md1a+1)
c
c..split up real work space =>
      lw=1
      lw1=lw+m*ngrid*3
      la0=lw1+m*ngrid*5*2
      la1=la0+4*ngrid
      laj=la1+8*ngrid
      lda0=laj+2*ngrid
      lvaxi=lda0+2*ngrid
      nreq=lvaxi+3*ngrid
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
c copy u to up
      do i=1,m
        do j2=md2a,md2b
        do j1=md1a,md1b
          up(j1,j2,i)=u(j1,j2,i)
        end do
        end do
      end do
c
c compute first source contribution (zero out tau, the maximum
c truncation error estimate returned by cmfsource)
      if (irxn.ne.0.and.r+dr.gt.tfilter) then
        dr2=.5d0*dr
        do k=1,ntau
          tau(k)=0.d0
        end do
        call cmfsource (m,nd1a,nd1b,md1a,md1b,nd2a,nd2b,md2a,md2b,dr2,
     *                  r,xy,up,mask,tau,maxstep1,niwk,iwk,nrwk,rwk)
      end if
c
c zero out almax
      almax(1)=0.d0
      almax(2)=0.d0
c
c      write(6,*)'cmfhydro, start ...'
c
c hydro step (handles slope correction, flux, and free-stream correction)
      call cmfhydro (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,md2a,md2b,
     *               n2a,n2b,dr,ds,r,rx,gv,det,rx2,gv2,det2,xy,up,
     *               mask,almax,rwk(lw),rwk(lw1),rwk(la0),rwk(la1),
     *               rwk(laj),rwk(lda0),rwk(lvaxi),mdat,dat,move,
     *               maxnstep,icart,iorder,method,n1bm,n2bm,ier)
c
      if (ier.ne.0) return
c
c      write(6,*)'cmfhydro, done ...'
c      pause
c
c compute second source contribution
      if (irxn.ne.0.and.r+dr.gt.tfilter) then
        r1=r+dr2
        call cmfsource (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,dr2,
     *                  r1,xy,up,mask,tau,maxstep2,niwk,iwk,nrwk,rwk)
      end if
c
c limit pressure (if necessary)
      if (iplimit) then
        call cmfplimit (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                  up,mask,rwk)
      end if
c
c..apply filter for P and C- blips
      if (iaxi.gt.0.and.nstep.gt.0) then
        if (dabs(r-tfilter).lt.dr) then
          if (m.lt.0) then
            write(6,123)(u(n1a,n2a,i),i=1,4)
  123       format(4(1x,1pe22.15))
            pause
          end if
          call cmffilter (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                    xy,up,mask)
        end if
      end if
c
c compute up
      do i=1,m
        do j1=n1a,n1b
        do j2=n2a,n2b
          up(j1,j2,i)=(up(j1,j2,i)-u(j1,j2,i))/dr
        end do
        end do
      end do
c
c artificial viscosity (and zero up in ghost cells)
c      if (.false.) then
      call cmfvisc (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,md2a,md2b,
     *              n2a,n2b,ds,rx,u,up,mask,rwk,av,vismax,icart,ad,
     *              n1bm,n2bm)
c      end if
c
c compute real and imaginary parts of lambda, where the time stepping
c is interpreted as u'=lambda*u
c
      rparam(1)=4.d0*vismax
      rparam(2)=almax(1)/ds(1)+almax(2)/ds(2)
c
c timings
      rparam(31)=tflux
      rparam(32)=tslope
      rparam(33)=tsource
c
      if (idebug.gt.0) then
        write(6,105)rparam(1),rparam(2),maxnstep
      end if
  105 format('max(lambda) =',2(1pe9.2,1x),',  max(RxnSteps) =',i6)
c
      if( idebug.gt.0 ) write(6,*)'...done: cmfdu'
c     write(6,*)'...done: cmfdu'
c      write(6,944)
c  944 format('** done')
c
c
      return
      end
c
c++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
c
c                      Source step
c
c++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
c
      subroutine cmfsource (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,dt,
     *                      t,xy,u,mask,tau)
c
c No reaction model supported as yet
c
      implicit real*8 (a-h,o-z)
      dimension xy(nd1a:nd1b,nd2a:nd2b,2),u(nd1a:nd1b,nd2a:nd2b,m),
     *          mask(nd1a:nd1b,nd2a:nd2b),tau(nd1a:nd1b,nd2a:nd2b)
      dimension htz(10)
      common / tzflow / eptz,itz
      common / rundat / ieos,irxn
      common / srcprm / sigma,pign,npower
c
c..timings
      common / cmftime / tflux,tslope,tsource
c
      data small / 1.d-10 /
c
      call ovtime (time0)
c
      do j2=n2a,n2b
      do j1=n1a,n1b
c
        if (mask(j1,j2).ne.0) then
c
          alam0=u(j1,j2,5)
          if (alam0.lt.1.d0-small) then
c
c compute pressure
            rho=u(j1,j2,1)
            v1=u(j1,j2,2)/rho
            v2=u(j1,j2,3)/rho
            en=u(j1,j2,4)-.5d0*rho*(v1**2+v2**2)
            amu1=(1.d0-alam0)*u(j1,j2,6)+alam0*u(j1,j2,7)
            amu2=(1.d0-alam0)*u(j1,j2,8)+alam0*u(j1,j2,9)
            amu3=                        alam0*u(j1,j2,10)
            p=(en-amu2-rho*amu3)/amu1
c
c check for ignition
            if (p.gt.pign) then
c
c compute rate and take a half step in lambda
              rate=sigma*(p-pign)**npower
              alam=1.d0-(1.d0-alam0)*dexp(-rate*dt/2.d0)
c
c TZ contribution (if necessary)
              if (itz.ne.0) then
                x=xy(j1,j2,1)
                y=xy(j1,j2,2)
                call ogDeriv (eptz,0,0,0,0,x,y,0.d0,t,0,w0)
                call ogDeriv (eptz,0,0,0,0,x,y,0.d0,t,3,w3)
                call ogDeriv (eptz,0,0,0,0,x,y,0.d0,t+dt/2.d0,4,w4)
                ratetz=sigma*((w0*w3)-pign)**npower
                alam=1.d0+(alam-1.d0)*dexp(ratetz*dt/2.d0)+w4-alam0
              else
c
c Euler step
                alam1=1.d0-(1.d0-alam0)*dexp(-rate*dt)
              end if
c
c re-compute pressure
              amu1=(1.d0-alam)*u(j1,j2,6)+alam*u(j1,j2,7)
              amu2=(1.d0-alam)*u(j1,j2,8)+alam*u(j1,j2,9)
              amu3=                       alam*u(j1,j2,10)
              p=(en-amu2-rho*amu3)/amu1
c
c compute rate at the half step, and take an explicit Midpoint step
              rate=sigma*max(p-pign,0.d0)**npower
              alam2=1.d0-(1.d0-alam0)*dexp(-rate*dt)
c
c TZ contribution (if necessary)
              if (itz.ne.0) then
                tau(j1,j2)=0.d0
                call ogDeriv (eptz,0,0,0,0,x,y,0.d0,t+dt/2.d0,0,w0)
                call ogDeriv (eptz,0,0,0,0,x,y,0.d0,t+dt/2.d0,3,w3)
                call ogDeriv (eptz,0,0,0,0,x,y,0.d0,t+dt,4,w4)
                ratetz=sigma*((w0*w3)-pign)**npower
                alam2=1.d0+(alam2-1.d0)*dexp(ratetz*dt)+w4-alam0
              else
c
c truncation error estimate
                tau(j1,j2)=dabs(alam1-alam2)/dt
              end if
c
              u(j1,j2,5)=alam2
            end if
c
          else
c
c fully reacted
            u(j1,j2,5)=1.d0
c
          end if
c
        end if
c
      end do
      end do
c
      call ovtime (time1)
      tsource=tsource+time1-time0
c
      return
      end
c
c++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
c
c                      artificial viscosity
c
c++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
c
      subroutine cmfvisc (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
     *                    md2a,md2b,n2a,n2b,ds,rx,u,up,mask,div,
     *                    av,vismax,icart,ad,n1bm,n2bm)
c
      implicit real*8 (a-h,o-z)
      dimension rx(nd1a:n1bm,nd2a:n2bm,2,2),u(nd1a:nd1b,nd2a:nd2b,m),
     *          up(nd1a:nd1b,nd2a:nd2b,m),mask(nd1a:nd1b,nd2a:nd2b),
     *          ds(2),div(md1a:md1b,2),ad(m),fx(10)
c
      if (m.gt.10) then
        write(6,*)'Error (cmfvisc) : m>10'
        stop
      end if
c
c      if (m.gt.0) then
c        write(6,*)av,ad
c        stop
c      end if
c
      vismax=0.d0
      adMax=0.d0
      do i=1,m
        adMax=max(adMax,ad(i))
      end do
c
c conserved variables
      do j2=n2a-1,n2b+1
        j2m1=j2-1
        if (icart.eq.0) then
          do j1=n1a-1,n1b+1
            vxm=(rx(j1,j2,1,1)*u(j1-1,j2,2)
     *          +rx(j1,j2,1,2)*u(j1-1,j2,3))/u(j1-1,j2,1)
            vxp=(rx(j1,j2,1,1)*u(j1+1,j2,2)
     *          +rx(j1,j2,1,2)*u(j1+1,j2,3))/u(j1+1,j2,1)
            vym=(rx(j1,j2,2,1)*u(j1,j2-1,2)
     *          +rx(j1,j2,2,2)*u(j1,j2-1,3))/u(j1,j2-1,1)
            vyp=(rx(j1,j2,2,1)*u(j1,j2+1,2)
     *          +rx(j1,j2,2,2)*u(j1,j2+1,3))/u(j1,j2+1,1)
            div(j1,2)= (vxp-vxm)/(2.d0*ds(1))+(vyp-vym)/(2.d0*ds(2))
            if (mask(j1,j2).ne.0) then
              vismax=max(-div(j1,2),vismax)
            end if
          end do
        else
          a11=rx(nd1a,nd2a,1,1)/(2.d0*ds(1))
          a22=rx(nd1a,nd2a,2,2)/(2.d0*ds(2))
          do j1=n1a-1,n1b+1
            vxm=u(j1-1,j2,2)/u(j1-1,j2,1)
            vxp=u(j1+1,j2,2)/u(j1+1,j2,1)
            vym=u(j1,j2-1,3)/u(j1,j2-1,1)
            vyp=u(j1,j2+1,3)/u(j1,j2+1,1)
            div(j1,2)=a11*(vxp-vxm)+a22*(vyp-vym)
            if (mask(j1,j2).ne.0) then
              vismax=max(-div(j1,2),vismax)
            end if
          end do
        end if
        if (j2.ge.n2a) then
          do j1=n1a,n1b
            if (mask(j1,j2).ne.0.and.mask(j1,j2m1).ne.0) then
              vis=av*max(0.d0,-(div(j1,1)+div(j1,2))/2.d0)
              do i=1,4
                fx(i)=(vis+ad(i))*(u(j1,j2,i)-u(j1,j2m1,i))
                up(j1,j2  ,i)=up(j1,j2  ,i)-fx(i)
                up(j1,j2m1,i)=up(j1,j2m1,i)+fx(i)
              end do
            end if
          end do
          if (j2.le.n2b) then
            do j1=n1a-1,n1b
              j1p1=j1+1
              if (mask(j1,j2).ne.0.and.mask(j1p1,j2).ne.0) then
                vis=av*max(0.d0,-(div(j1,2)+div(j1p1,2))/2.d0)
                do i=1,4
                  fx(i)=(vis+ad(i))*(u(j1p1,j2,i)-u(j1,j2,i))
                  up(j1p1,j2,i)=up(j1p1,j2,i)-fx(i)
                  up(j1  ,j2,i)=up(j1  ,j2,i)+fx(i)
                end do
              end if
            end do
          end if
        end if
        do j1=n1a-1,n1b+1
          div(j1,1)=div(j1,2)
        end do
      end do
c
c advected scalars
      do j2=n2a,n2b+1
        j2m1=j2-1
        do j1=n1a,n1b
          if (mask(j1,j2).ne.0.and.mask(j1,j2m1).ne.0) then
            do i=5,m
              fx(i)=ad(i)*(u(j1,j2,i)-u(j1,j2m1,i))
              up(j1,j2  ,i)=up(j1,j2  ,i)-fx(i)
              up(j1,j2m1,i)=up(j1,j2m1,i)+fx(i)
            end do
          end if
        end do
        if (j2.le.n2b) then
          do j1=n1a-1,n1b
            j1p1=j1+1
            if (mask(j1,j2).ne.0.and.mask(j1p1,j2).ne.0) then
              do i=5,m
                fx(i)=ad(i)*(u(j1p1,j2,i)-u(j1,j2,i))
                up(j1p1,j2,i)=up(j1p1,j2,i)-fx(i)
                up(j1  ,j2,i)=up(j1  ,j2,i)+fx(i)
              end do
            end if
          end do
        end if
      end do
c
      vismax=av*vismax+adMax
c
c zero up in ghost cells
      do j2=md2a,md2b
        do j1=md1a,n1a-1
          do i=1,m
            up(j1,j2,i)=0.d0
          end do
        end do
        do j1=n1b+1,md1b
          do i=1,m
            up(j1,j2,i)=0.d0
          end do
        end do
      end do
      do j1=n1a,n1b
        do j2=md2a,n2a-1
          do i=1,m
            up(j1,j2,i)=0.d0
          end do
        end do
        do j2=n2b+1,nd2b
          do i=1,m
            up(j1,j2,i)=0.d0
          end do
        end do
      end do
c
      return
      end
c
c++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
c
c                      Hydro step
c
c++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
c
      subroutine cmfhydro (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
     *                     md2a,md2b,n2a,n2b,dr,ds,r,rx,gv,det,rx2,
     *                     gv2,det2,xy,up,mask,almax,w,w1,a0,a1,aj,
     *                     da0,vaxi,mdat,dat,move,maxnstep,icart,
     *                     iorder,method,n1bm,n2bm,ier)
c
      implicit real*8 (a-h,o-z)
      dimension rx(nd1a:n1bm,nd2a:n2bm,2,2),gv(nd1a:nd1b,nd2a:nd2b,2),
     *          det(nd1a:n1bm,nd2a:n2bm),rx2(nd1a:n1bm,nd2a:n2bm,2,2),
     *          gv2(nd1a:nd1b,nd2a:nd2b,2),det2(nd1a:n1bm,nd2a:n2bm),
     *          xy(nd1a:nd1b,nd2a:nd2b,2),up(nd1a:nd1b,nd2a:nd2b,m),
     *          mask(nd1a:nd1b,nd2a:nd2b),ds(2),almax(2),
     *          w(m,md1a:md1b,3),w1(m,md1a:md1b,5,2),
     *          a0(2,md1a:md1b,2),a1(2,2,md1a:md1b,2),aj(md1a:md1b,2),
     *          da0(md1a:md1b,2),vaxi(md1a:md1b,3),
     *          dat(nd1a:nd1b,nd2a:nd2b,*)
      dimension fl(10),fr(10),htz(10)
c      logical ifilter

      dimension wlsave(10),wrsave(10)

c
      common / axidat / iaxi,j1axi(2),j2axi(2)
      common / rundat / ieos,irxn
      common / tzflow / eptz,itz
c
c..timings
      common / cmftime / tflux,tslope,tsource

      common / gdinfo / igrid,level

c
c..filter out P and C- blips for shock ICs (can only be used if xy is active)
c      data ifilter, tfilter / .true., .98d-2 /
c      data ifilter, tfilter / .false., .98d-2 /
c
c..ratios
      dtds1=dr/ds(1)
      dtds2=dr/ds(2)
c
      if (m.lt.0) then
        write(6,*)det(nd1a,nd2a),rx(nd1a,nd2a,1,1),rx(nd1a,nd2a,1,2),
     *                           rx(nd1a,nd2a,2,1),rx(nd1a,nd2a,2,2)
        pause
      end if
c
      if (m.lt.0) then
        write(6,*)'dt,ds1,ds2=',dr,ds(1),ds(2)
        pause
      end if
c
c
c dat(nd1a:nd1b,nd2a:nd2b,.) is used to save extra grid data,
c if desired, which can then be plotted using plotStuff.
c      if (mdat.gt.0) then
c        do j2=n2a,n2b
c        do j1=n1a,n1b
c          if (mask(j1,j2).eq.0) then
c            dat(j1,j2,1)=0.d0
c          else
c            dat(j1,j2,1)=0.d0    ! set dat() to be whatever
c          end if
c        end do
c        end do
c      end if
c
c..begin hydro step.  Get primitive states along lines j2=n2a-2,n2a-1,n2a
      do k2=1,3
        j2=n2a+k2-3
        call cmfpvs (m,nd1a,nd1b,md1a,md1b,nd2a,nd2b,j2,up,
     *               w(1,md1a,k2))
      end do
c
c..set grid metrics and velocity (if necessary)
      j2=n2a-1
      call mfmetrics (nd1a,nd1b,md1a,md1b,nd2a,nd2b,j2,rx,gv,det,
     *                rx2,gv2,det2,a0(1,md1a,1),a1(1,1,md1a,1),
     *                aj(md1a,1),move,icart,n1bm,n2bm)
c
      call ovtime (time0)
c
c..slope correction (bottom row of cells)
      call cmfslope (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,j2,r,
     *               dr,ds,xy,a0(1,md1a,1),a1(1,1,md1a,1),aj(md1a,1),
     *               mask(nd1a,j2),w,w1(1,md1a,1,1),iorder)
c
c..axisymmetric stuff
      if (iaxi.gt.0) then
        do j1=n1a-1,n1b+1
          if (mask(j1,j2).ne.0) then
            vaxi(j1,2)=w1(3,j1,5,1)
          end if
        end do
      end if
c
      call ovtime (time1)
      tslope=time1-time0
c
c..loop over lines j2=n2a:n2b+1
      do j2=n2a,n2b+1
        j2m1=j2-1
c
c..shift w
        do j1=md1a,md1b
          do i=1,m
            w(i,j1,1)=w(i,j1,2)
            w(i,j1,2)=w(i,j1,3)
          end do
        end do
c
c..get primitive states along line j2+1
        call cmfpvs (m,nd1a,nd1b,md1a,md1b,nd2a,nd2b,j2+1,up,
     *               w(1,md1a,3))
c
c..set grid metrics and velocity (if necessary)
        call mfmetrics (nd1a,nd1b,md1a,md1b,nd2a,nd2b,j2,rx,gv,det,
     *                  rx2,gv2,det2,a0(1,md1a,2),a1(1,1,md1a,2),
     *                  aj(md1a,2),move,icart,n1bm,n2bm)
c
        call ovtime (time0)
c
c..slope correction (top row of cells)
        call cmfslope (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,j2,r,
     *                 dr,ds,xy,a0(1,md1a,2),a1(1,1,md1a,2),aj(md1a,2),
     *                 mask(nd1a,j2),w,w1(1,md1a,1,2),iorder)
c
c..axisymmetric stuff
        if (iaxi.gt.0) then
          do j1=n1a-1,n1b+1
            if (mask(j1,j2).ne.0) then
              vaxi(j1,3)=w1(3,j1,5,2)
            end if
          end do
        end if
c
        call ovtime (time1)
        tslope=tslope+time1-time0
c
c..compute s2 flux along j2-1/2, add it to up(j1,j2,.) and up(j1,j2-1,.)
        do j1=n1a,n1b
          if (mask(j1,j2).ne.0.and.mask(j1,j2m1).ne.0) then
            aj0=(aj(j1,2)+aj(j1,1))/2.d0
            a20=(a0(2,j1,2)+a0(2,j1,1))/2.d0
            a21=(a1(2,1,j1,2)+a1(2,1,j1,1))/2.d0
            a22=(a1(2,2,j1,2)+a1(2,2,j1,1))/2.d0

            do i=1,m
              wlsave(i)=w1(i,j1,4,1)
              wrsave(i)=w1(i,j1,3,2)
            end do

            call cmfflux (m,aj0,a20,a21,a22,w1(1,j1,4,1),
     *                    w1(1,j1,3,2),fl,fr,almax(2),ier)
c            aj0=1.d0
c            a20=0.d0
c            a21=0.d0
c            a22=1.d0
c            call cmfflux (m,aj0,a20,a21,a22,ww(1,j1,j2m1),
c     *                    ww(1,j1,j2),fl,fr,almax(2),ier)
            if (ier.ne.0) then
              if (iorder.gt.1) then
                do i=1,m
                  w1(i,j1,4,1)=w(i,j1,1)
                  w1(i,j1,3,2)=w(i,j1,2)
                end do
                call cmfflux (m,aj0,a20,a21,a22,w1(1,j1,4,1),
     *                        w1(1,j1,3,2),fl,fr,almax(2),ier)
              end if
              if (ier.ne.0) then
                write(6,*)'Error (cmfhydro) : s2 flux, ier=',ier
                write(66,666)igrid,level,r,xy(j1,j2,1),xy(j1,j2,2),
     *                       aj0,a20,a21,a22,(wlsave(i),w(i,j1,1),
     *                       wrsave(i),w(i,j1,2),i=1,m)
  666           format(2(1x,i4),/,3(1x,1pe15.8),/,4(1x,1pe15.8),
     *                 10(/,4(1x,1pe15.8)))
                stop
              end if
            end if
            do i=1,m
              up(j1,j2  ,i)=up(j1,j2  ,i)+dtds2*fr(i)/aj(j1,2)
              up(j1,j2m1,i)=up(j1,j2m1,i)-dtds2*fl(i)/aj(j1,1)
c              up(j1,j2  ,i)=up(j1,j2  ,i)+dtds2*fr(i)
c              up(j1,j2m1,i)=up(j1,j2m1,i)-dtds2*fl(i)
            end do
          end if
        end do
c
c..contribution to free-stream correction for moving, non-Cartesian grids
c  (not implemented)
        if (move.ne.0.and.icart.eq.0.and.m.lt.0) then
          do j1=n1a,n1b
            if (mask(j1,j2).ne.0.and.mask(j1,j2m1).ne.0) then
              fact=(aj(j1,2)+aj(j1,1))*(a0(2,j1,2)+a0(2,j1,1))
     *             /(4.d0*ds(2))
              da0(j1,2)=         -fact
              da0(j1,1)=da0(j1,1)+fact
            end if
          end do
        end if
c
c..add in final second-order contributions to up
        if (j2.gt.n2a.and.iorder.eq.2) then
          do j1=n1a,n1b
            vel1=.5d0*( a1(1,1,j1,1)*(w1(2,j1,1,1)+w1(2,j1,2,1))
     *                 +a1(1,2,j1,1)*(w1(3,j1,1,1)+w1(3,j1,2,1)))
            vel2=.5d0*( a1(2,1,j1,1)*(w1(2,j1,3,1)+w1(2,j1,4,1))
     *                 +a1(2,2,j1,1)*(w1(3,j1,3,1)+w1(3,j1,4,1)))
            do i=5,m
              dphi1=dtds1*(w1(i,j1,2,1)-w1(i,j1,1,1))
              dphi2=dtds2*(w1(i,j1,4,1)-w1(i,j1,3,1))
              up(j1,j2m1,i)=up(j1,j2m1,i)-(vel1*dphi1+vel2*dphi2)
            end do
          end do
        end if
c
c..add axisymmetric contribution, if necessary
        if (j2.gt.n2a.and.iaxi.gt.0) then
            do j1=n1a,n1b
              if (mask(j1,j2m1).ne.0) then
                if (iaxi.eq.1) then
c (revolve about grid line j1)
                  if (j1.eq.j1axi(1).or.j1.eq.j1axi(2)) then
                    fact=.5d0*drds1*a1(1,2,j1,1)
     *                    *(vaxi(j1+1,2)-vaxi(j1-1,2))
                  else
                    fact=dr*vaxi(j1,2)/xy(j1,j2m1,2)
                  end if
                else
c (revolve about grid line j2m1)
                  if (j2m1.eq.j2axi(1).or.j2m1.eq.j2axi(2)) then
                    fact=.5d0*drds2*a1(2,2,j1,1)
     *                   *(vaxi(j1,3)-vaxi(j1,1))
                  else
                    fact=dr*vaxi(j1,2)/xy(j1,j2m1,2)
                  end if
                end if
c (contribution)
                tmp=fact*w1(1,j1,5,1)
                up(j1,j2m1,1)=up(j1,j2m1,1)-tmp
                up(j1,j2m1,2)=up(j1,j2m1,2)-tmp*w1(2,j1,5,1)
                up(j1,j2m1,3)=up(j1,j2m1,3)-tmp*w1(3,j1,5,1)
                enthalpy=w1(4,j1,5,1)
                if (irxn.eq.0) then
                  enthalpy=enthalpy
     *                     +w1(5,j1,5,1)*w1(4,j1,5,1)+w1(6,j1,5,1)
                else
                  alam=w1(5,j1,5,1)
                  amu1=(1.d0-alam)*w1(6,j1,5,1)+alam*w1(7,j1,5,1)
                  amu2=(1.d0-alam)*w1(8,j1,5,1)+alam*w1(9,j1,5,1)
                  amu3=                         alam*w1(10,j1,5,1)
                  enthalpy=enthalpy
     *                     +amu1*w1(4,j1,5,1)+amu2+w1(1,j1,5,1)*amu3
                end if
                tmp=fact*enthalpy+.5d0*tmp*(w1(2,j1,5,1)**2
     *                                     +w1(3,j1,5,1)**2)
                up(j1,j2m1,4)=up(j1,j2m1,4)-tmp
              end if
            end do
        end if
c
        call ovtime (time2)
        tflux=tflux+time2-time1
c
c..if j2.le.n2b, then compute fluxes in the s1 direction
        if (j2.le.n2b) then
c
c..reset metrics and w1
          do j1=n1a-1,n1b+1
            aj(j1,1)=aj(j1,2)
            a0(1,j1,1)=a0(1,j1,2)
            a0(2,j1,1)=a0(2,j1,2)
            a1(1,1,j1,1)=a1(1,1,j1,2)
            a1(1,2,j1,1)=a1(1,2,j1,2)
            a1(2,1,j1,1)=a1(2,1,j1,2)
            a1(2,2,j1,1)=a1(2,2,j1,2)
            do k=1,5
              do i=1,m
                w1(i,j1,k,1)=w1(i,j1,k,2)
              end do
            end do
          end do
c
c..reset da0 (for moving, non-Cartesian grids)
c  (not implemented)
          if (move.ne.0.and.icart.eq.0.and.m.lt.0) then
            do j1=n1a,n1b
              da0(j1,1)=da0(j1,2)
            end do
          end if
c
c..reset vaxi (for axisymmetric problems)
          if (iaxi.gt.0) then
            do j1=n1a-1,n1b+1
              vaxi(j1,1)=vaxi(j1,2)
              vaxi(j1,2)=vaxi(j1,3)
            end do
          end if
c
          call ovtime (time0)
c
c..compute s1 flux along j2, add it to up(j1+1,j2,.) and up(j1,j2,.)
          do j1=n1a-1,n1b
            j1p1=j1+1
            if (mask(j1,j2).ne.0.and.mask(j1p1,j2).ne.0) then
              aj0=(aj(j1p1,1)+aj(j1,1))/2.d0
              a10=(a0(1,j1p1,1)+a0(1,j1,1))/2.d0
              a11=(a1(1,1,j1p1,1)+a1(1,1,j1,1))/2.d0
              a12=(a1(1,2,j1p1,1)+a1(1,2,j1,1))/2.d0

              do i=1,m
                wlsave(i)=w1(i,j1,2,1)
                wrsave(i)=w1(i,j1p1,1,1)
              end do

              call cmfflux (m,aj0,a10,a11,a12,w1(1,j1,2,1),
     *                      w1(1,j1p1,1,1),fl,fr,almax(1),ier)
              if (ier.ne.0) then
                if (iorder.gt.1) then
                  do i=1,m
                    w1(i,j1  ,2,1)=w(i,j1  ,2)
                    w1(i,j1p1,1,1)=w(i,j1p1,2)
                  end do
                  call cmfflux (m,aj0,a10,a11,a12,w1(1,j1,2,1),
     *                          w1(1,j1p1,1,1),fl,fr,almax(1),ier)
                end if
                if (ier.ne.0) then
                  write(6,*)'Error (cmfhydro) : s1 flux, ier=',ier
                  write(66,666)igrid,level,r,xy(j1,j2,1),xy(j1,j2,2),
     *                         aj0,a10,a11,a12,(wlsave(i),w(i,j1,2),
     *                         wrsave(i),w(i,j1p1,2),i=1,m)
                  stop
                end if
              end if
              do i=1,m
                up(j1p1,j2,i)=up(j1p1,j2,i)+dtds1*fr(i)/aj(j1p1,1)
                up(j1  ,j2,i)=up(j1  ,j2,i)-dtds1*fl(i)/aj(j1  ,1)
c                up(j1p1,j2,i)=up(j1p1,j2,i)+dtds1*fr(i)
c                up(j1  ,j2,i)=up(j1  ,j2,i)-dtds1*fl(i)
              end do
            end if
          end do
c
          call ovtime (time1)
          tflux=tflux+time1-time0
c
c..contribution to free-stream correction for moving, non-Cartesian grids
c  (not implemented)
          if (move.ne.0.and.icart.eq.0.and.m.lt.0) then
            do j1=n1a-1,n1b
              j1p1=j1+1
              if (mask(j1,j2).ne.0.and.mask(j1p1,j2).ne.0) then
                fact=(aj(j1p1,1)+aj(j1,1))*(a0(1,j1p1,1)+a0(1,j1,1))
     *                /(4.d0*ds(1))
                da0(j1p1,1)=da0(j1p1,1)-fact
                da0(j1  ,1)=da0(j1  ,1)+fact
              end if
            end do
          end if
c
c..add free stream correction to up (if a non-Cartesian grid)
          if (icart.eq.0) then
            call cmffree (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
     *                    dr,ds,j2,rx,det,rx2,det2,up,mask(nd1a,j2),
     *                    w1(1,md1a,5,1),move)
          end if
c
        end if
c
c..bottom of main loop over lines
      end do
c
c..apply filter for P and C- blips
c      if (iaxi.gt.0.and.icart.ne.0.and.ifilter) then
c        if (dabs(r-tfilter).lt.dr) then
c          call cmffilter (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
c     *                    xy,up,mask)
c        end if
c      end if
c
c..add in TZ forcing (if necessary)
      if (itz.ne.0) then
        t=r
        if (iorder.eq.2) t=t+.5d0*dr
        iprim=0
        do j1=n1a,n1b
        do j2=n2a,n2b
          if (mask(j1,j2).ne.0) then
            call gethtz (m,xy(j1,j2,1),xy(j1,j2,2),t,htz,iprim)
            do i=1,m
              up(j1,j2,i)=up(j1,j2,i)+dr*htz(i)
            end do
          end if
        end do
        end do
      end if
c
      return
      end
c
c+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
c
      subroutine cmffilter (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                      xy,up,mask)
c
c assumes that the mesh is Cartesian and xy is active
c
      implicit real*8 (a-h,o-z)
      dimension xy(nd1a:nd1b,nd2a:nd2b,2),up(nd1a:nd1b,nd2a:nd2b,m),
     *          mask(nd1a:nd1b,nd2a:nd2b)
      common / filter / tfilter,xfilter,ufilter(4)
c
c      data xfilter / .25d0 /
c      data ufilter / 3.0622494d3, 5.9165525d3, 0.d0, 2.28616667d4 /
c
c      if (m.gt.0) then
c        write(6,*)'cmffilter: stop'
c        stop
c      end if
c
      do j2=n2a,n2b
      do j1=n1a,n1b
        if (mask(j1,j2).ne.0) then
c        if (j2.eq.n2a) write(6,*)'j1, x=',j1,xy(j1,j2,1)
          if (xy(j1,j2,1).lt.xfilter) then
            do i=1,4
c              if (j2.eq.n2a) then
c                write(6,100)i,up(j1,j2,i),ufilter(i)
c  100           format(1x,i1,2(1x,1pe15.8))
c              end if
              up(j1,j2,i)=ufilter(i)
            end do
          end if
        end if
      end do
      end do
c      pause
c
      return
      end
c
c+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
c
      subroutine cmfpvs (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,j2,u,w)
c
c convert conservative variables u to primitive variables w
c
      implicit real*8 (a-h,o-z)
      dimension u(nd1a:nd1b,nd2a:nd2b,m),w(m,n1a:n1b)
      common / rundat / ieos,irxn
c
      do j1=n1a,n1b
c
c divide by density
        w(1,j1)=u(j1,j2,1)
        w(2,j1)=u(j1,j2,2)/u(j1,j2,1)
        w(3,j1)=u(j1,j2,3)/u(j1,j2,1)
c
c energy
        en=u(j1,j2,4)-.5d0*w(1,j1)*(w(2,j1)**2+w(3,j1)**2)
c
c mixture pressure
        if (irxn.eq.0) then
          p=(en-u(j1,j2,6))/u(j1,j2,5)
        else
          alam=u(j1,j2,5)
          amu1=(1.d0-alam)*u(j1,j2,6)+alam*u(j1,j2,7)
          amu2=(1.d0-alam)*u(j1,j2,8)+alam*u(j1,j2,9)
          amu3=                       alam*u(j1,j2,10)
          p=(en-amu2-u(j1,j2,1)*amu3)/amu1
        end if
        w(4,j1)=p
c
c copy advected variables
        do i=5,m
          w(i,j1)=u(j1,j2,i)
        end do
c
      end do
c
      return
      end
c
c++++++++++++++++++++++++++++
c
      subroutine mfmetrics (nd1a,nd1b,md1a,md1b,nd2a,nd2b,j2,rx,gv,det,
     *                      rx2,gv2,det2,a0,a1,aj,move,icart,n1bm,n2bm)
c
c grid metrics
c
      implicit real*8 (a-h,o-z)
      dimension rx(nd1a:n1bm,nd2a:n2bm,2,2),gv(nd1a:nd1b,nd2a:nd2b,2),
     *          det(nd1a:n1bm,nd2a:n2bm),rx2(nd1a:n1bm,nd2a:n2bm,2,2),
     *          gv2(nd1a:nd1b,nd2a:nd2b,2),det2(nd1a:n1bm,nd2a:n2bm),
     *          a0(2,md1a:md1b),a1(2,2,md1a:md1b),aj(md1a:md1b)
c
      if (move.ne.0) then
        if (icart.eq.0) then
c
c moving, non-Cartesian grid
          do j1=md1a,md1b
            do k=1,2
              a0(k,j1)=(-rx(j1,j2,k,1)*gv(j1,j2,1)
     *                  -rx(j1,j2,k,2)*gv(j1,j2,2)
     *                  -rx2(j1,j2,k,1)*gv2(j1,j2,1)
     *                  -rx2(j1,j2,k,2)*gv2(j1,j2,2))/2.d0
              do l=1,2
                a1(k,l,j1)=(rx(j1,j2,k,l)+rx2(j1,j2,k,l))/2.d0
              end do
            end do
            aj(j1)=(det(j1,j2)+det2(j1,j2))/2.d0
          end do
c
        else
c
c moving, Cartesian grid
          do j1=md1a,md1b
            do k=1,2
              a0(k,j1)=(-rx(nd1a,nd2a,k,1)*gv(j1,j2,1)
     *                  -rx(nd1a,nd2a,k,2)*gv(j1,j2,2)
     *                  -rx2(nd1a,nd2a,k,1)*gv2(j1,j2,1)
     *                  -rx2(nd1a,nd2a,k,2)*gv2(j1,j2,2))/2.d0
              do l=1,2
                a1(k,l,j1)=(rx(nd1a,nd2a,k,l)
     *                     +rx2(nd1a,nd2a,k,l))/2.d0
              end do
            end do
            aj(j1)=(det(nd1a,nd2a)+det2(nd1a,nd2a))/2.d0
          end do
        end if
c
      else
c
c fixed grid
        do j1=md1a,md1b
          a0(1,j1)=0.d0
          a0(2,j1)=0.d0
        end do
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
          end do
c
        else
c
c Cartesian
          do j1=md1a,md1b
            a1(1,1,j1)=rx(nd1a,nd2a,1,1)
            a1(1,2,j1)=rx(nd1a,nd2a,1,2)
            a1(2,1,j1)=rx(nd1a,nd2a,2,1)
            a1(2,2,j1)=rx(nd1a,nd2a,2,2)
            aj(j1)=det(nd1a,nd2a)
          end do
c
c          write(6,*)a1(1,1,md1a),a1(1,2,md1a),
c     *              a1(2,1,md1a),a1(2,2,md1a),aj(md1a)
c          pause
c
        end if
      end if
c
      return
      end
c
c++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
c
      subroutine cmfslope (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,j2,
     *                     r,dr,ds,xy,a0,a1,aj,mask,w,w1,iorder)
c
c slope correction
c
      implicit real*8 (a-h,o-z)
      dimension ds(2),xy(nd1a:nd1b,nd2a:nd2b,2),a0(2,md1a:md1b),
     *          a1(2,2,md1a:md1b),aj(md1a:md1b),mask(nd1a:nd1b),
     *          w(m,md1a:md1b,3),w1(m,md1a:md1b,5)
      dimension w0(10),dw(10),b(10,2),al(10),fact(5),htz(10)
      logical ilimit
      common / axidat / iaxi,j1axi(2),j2axi(2)
      common / rundat / ieos,irxn
      common / tzflow / eptz,itz
      data c2min / 1.d-6 /
c
c..Jeff's antidiffusion factor designed to sharpen contacts (1.le.adiff.le.2)
      data adiff / 1.d0 /
c
c      write(6,*)iaxi,j1axi,j2axi,j2
c
c..copy solution to w1
      do k=1,5
        do j1=md1a,md1b
          do i=1,m
            w1(i,j1,k)=w(i,j1,2)
          end do
        end do
      end do
c
      if (iorder.le.1.or.m.lt.0) return
c
      dr2=dr/2.d0
c
c..compute slope contribution and add it to w1(i,j1,1:2,1:2)
      do j1=n1a-1,n1b+1

        if (mask(j1).ne.0) then
c
c..primitive state at cell center
          do i=1,m
            w0(i)=w(i,j1,2)
          end do
c
c..sound speed and stuff
          rho=w0(1)
          if (irxn.eq.0) then
            c2=((w0(5)+1.d0)*w0(4)+w0(6))/(rho*w0(5))
          else
            alam=w0(5)
            amu1=(1.d0-alam)*w0(6)+alam*w0(7)
            amu2=(1.d0-alam)*w0(8)+alam*w0(9)
            c2=((amu1+1.d0)*w0(4)+amu2)/(rho*amu1)
          end if
c
          if (c2.lt.c2min) then
            write(6,*)'Error (cmfslope) : c2<c2min'
            stop
          end if
          c=dsqrt(c2)
          rc2=rho*c2
c
c..some constants for Riemann variables
          t0=.5d0/c
          t1=.5d0/rc2
          t2=1.d0/c2
c
c..turn on slope limiter
          ilimit=.true.
c
c..supply TZ forcing to the primitive state (if necessary)
          if (itz.ne.0) then
            ilimit=.false.
            iprim=1
            call gethtz (m,xy(j1,j2,1),xy(j1,j2,2),r,htz,iprim)
            do k=1,5
              do i=1,m
                w1(i,j1,k)=w1(i,j1,k)+dr2*htz(i)
              end do
            end do
          end if
c
c..add axisymmetric contribution, if necessary
          if (iaxi.gt.0) then
            if (iaxi.eq.1) then
c (revolve about grid line j1)
              if (j1.eq.j1axi(1).or.j1.eq.j1axi(2)) then
                vy=a1(1,2,j1)*(w(3,j1+1,2)-w(3,j1-1,2))/(2.d0*ds(1))
              else
                vy=w(3,j1,2)/xy(j1,j2,2)
              end if
            else
c (revolve about grid line j2)
              if (j2.eq.j2axi(1).or.j2.eq.j2axi(2)) then
                vy=a1(2,2,j1)*(w(3,j1,3)-w(3,j1,1))/(2.d0*ds(2))
c                write(6,*)xy(j1,j2,2),w(3,j1,3),w(3,j1,1),ds(2)
              else
                vy=w(3,j1,2)/xy(j1,j2,2)
              end if
            end if
c (contribution)
            afact1=dr2*vy*rho
            afact2=afact1*c2
            do k=1,5
              w1(1,j1,k)=w1(1,j1,k)-afact1
              w1(4,j1,k)=w1(4,j1,k)-afact2
            end do
          end if
c
c..s1 direction
          rad=dsqrt(a1(1,1,j1)**2+a1(1,2,j1)**2)
          an1=a1(1,1,j1)/rad
          an2=a1(1,2,j1)/rad
c
c..differences of Riemann variables
          do ks=1,2
            jkp=j1+ks-1
            jk=jkp-1
            do i=1,m
              dw(i)=w(i,jkp,2)-w(i,jk,2)
            end do
            b(1,ks)=     t0*(-an1*dw(2)-an2*dw(3))+t1*dw(4)
            b(2,ks)=dw(1)                         -t2*dw(4)
            b(3,ks)=         -an2*dw(2)+an1*dw(3)
            b(m,ks)=     t0*( an1*dw(2)+an2*dw(3))+t1*dw(4)
            do i=5,m
              b(i-1,ks)=dw(i)
            end do
          end do
c
c eigenvalues * (dt/ds1) * rad / 2
          tmp=.5d0*rad*dr/ds(1)
          vn=an1*w0(2)+an2*w0(3)
          al(1)=tmp*(vn-c)
          al(2)=tmp*(vn  )
          do i=3,m-1
            al(i)=al(2)
          end do
          al(m)=tmp*(vn+c)
c
c C- contribution
          i=1
          if (ilimit) then
            if (b(i,1)*b(i,2).gt.0.d0) then
              beta=dsign(1.d0,b(i,1))*dmin1(dabs(b(i,1)),dabs(b(i,2)))
              fact(1)=(min(al(i),0.d0)+.5d0)*beta
              fact(2)=(max(al(i),0.d0)-.5d0)*beta
              fact(3)=al(i)*beta
              fact(4)=fact(3)
              fact(5)=fact(3)
              do k=1,5
                w1(1,j1,k)=w1(1,j1,k)-fact(k)*rho
                w1(2,j1,k)=w1(2,j1,k)+fact(k)*c*an1
                w1(3,j1,k)=w1(3,j1,k)+fact(k)*c*an2
                w1(4,j1,k)=w1(4,j1,k)-fact(k)*rc2
              end do
            end if
          else
            beta=.5d0*(b(i,1)+b(i,2))
            fact(1)=(al(i)+.5d0)*beta
            fact(2)=(al(i)-.5d0)*beta
            fact(3)=al(i)*beta
            fact(4)=fact(3)
            fact(5)=fact(3)
            do k=1,5
              w1(1,j1,k)=w1(1,j1,k)-fact(k)*rho
              w1(2,j1,k)=w1(2,j1,k)+fact(k)*c*an1
              w1(3,j1,k)=w1(3,j1,k)+fact(k)*c*an2
              w1(4,j1,k)=w1(4,j1,k)-fact(k)*rc2
            end do
          end if
c
c P contribution
          i=2
          if (ilimit) then
            if (b(i,1)*b(i,2).gt.0.d0 .or. .not.ilimit) then
              beta=dsign(1.d0,b(i,1))*dmin1(dabs(b(i,1)),dabs(b(i,2)))
              fact(1)=(min(al(i),0.d0)+.5d0)*beta
              fact(2)=(max(al(i),0.d0)-.5d0)*beta
              fact(3)=al(i)*beta
              fact(4)=fact(3)
              fact(5)=fact(3)
              do k=1,5
                w1(1,j1,k)=w1(1,j1,k)-fact(k)
              end do
            end if
          else
            beta=.5d0*(b(i,1)+b(i,2))
            fact(1)=(al(i)+.5d0)*beta
            fact(2)=(al(i)-.5d0)*beta
            fact(3)=al(i)*beta
            fact(4)=fact(3)
            fact(5)=fact(3)
            do k=1,5
              w1(1,j1,k)=w1(1,j1,k)-fact(k)
            end do
          end if
c
c P contribution
          i=3
          if (ilimit) then
            if (b(i,1)*b(i,2).gt.0.d0 .or. .not.ilimit) then
              beta=dsign(1.d0,b(i,1))*dmin1(dabs(b(i,1)),dabs(b(i,2)))
              fact(1)=(min(al(i),0.d0)+.5d0)*beta
              fact(2)=(max(al(i),0.d0)-.5d0)*beta
              fact(3)=al(i)*beta
              fact(4)=fact(3)
              fact(5)=fact(3)
              do k=1,5
                w1(2,j1,k)=w1(2,j1,k)+fact(k)*an2
                w1(3,j1,k)=w1(3,j1,k)-fact(k)*an1
              end do
            end if
          else
            beta=.5d0*(b(i,1)+b(i,2))
            fact(1)=(al(i)+.5d0)*beta
            fact(2)=(al(i)-.5d0)*beta
            fact(3)=al(i)*beta
            fact(4)=fact(3)
            fact(5)=fact(3)
            do k=1,5
              w1(2,j1,k)=w1(2,j1,k)+fact(k)*an2
              w1(3,j1,k)=w1(3,j1,k)-fact(k)*an1
            end do
          end if
c
c P contributions (advected scalars)
c          if (.false.) then
          do i=4,m-1
            ip1=i+1
            if (ilimit) then
              if (b(i,1)*b(i,2).gt.0.d0 .or. .not.ilimit) then
                beta=dsign(1.d0,b(i,1))*dmin1(dabs(b(i,1)),dabs(b(i,2)))
                fact(1)=(min(al(i),0.d0)+.5d0)*beta
                fact(2)=(max(al(i),0.d0)-.5d0)*beta
                fact(3)=al(i)*beta
                fact(4)=fact(3)
                fact(5)=fact(3)
                do k=1,5
                  w1(ip1,j1,k)=w1(ip1,j1,k)-adiff*fact(k)
                end do
              end if
            else
              beta=.5d0*(b(i,1)+b(i,2))
              fact(1)=(al(i)+.5d0)*beta
              fact(2)=(al(i)-.5d0)*beta
              fact(3)=al(i)*beta
              fact(4)=fact(3)
              fact(5)=fact(3)
              do k=1,5
                w1(ip1,j1,k)=w1(ip1,j1,k)-fact(k)
              end do
            end if
          end do
c          end if
c
c C+ contribution
          i=m
          if (ilimit) then
            if (b(i,1)*b(i,2).gt.0.d0 .or. .not.ilimit) then
              beta=dsign(1.d0,b(i,1))*dmin1(dabs(b(i,1)),dabs(b(i,2)))
              fact(1)=(min(al(i),0.d0)+.5d0)*beta
              fact(2)=(max(al(i),0.d0)-.5d0)*beta
              fact(3)=al(i)*beta
              fact(4)=fact(3)
              fact(5)=fact(3)
              do k=1,5
                w1(1,j1,k)=w1(1,j1,k)-fact(k)*rho
                w1(2,j1,k)=w1(2,j1,k)-fact(k)*c*an1
                w1(3,j1,k)=w1(3,j1,k)-fact(k)*c*an2
                w1(4,j1,k)=w1(4,j1,k)-fact(k)*rc2
              end do
            end if
          else
            beta=.5d0*(b(i,1)+b(i,2))
            fact(1)=(al(i)+.5d0)*beta
            fact(2)=(al(i)-.5d0)*beta
            fact(3)=al(i)*beta
            fact(4)=fact(3)
            fact(5)=fact(3)
            do k=1,5
              w1(1,j1,k)=w1(1,j1,k)-fact(k)*rho
              w1(2,j1,k)=w1(2,j1,k)-fact(k)*c*an1
              w1(3,j1,k)=w1(3,j1,k)-fact(k)*c*an2
              w1(4,j1,k)=w1(4,j1,k)-fact(k)*rc2
            end do
          end if
c
c
c..s2 direction
          rad=dsqrt(a1(2,1,j1)**2+a1(2,2,j1)**2)
          an1=a1(2,1,j1)/rad
          an2=a1(2,2,j1)/rad
c
c..differences of Riemann variables
          do ks=1,2
            do i=1,m
              dw(i)=w(i,j1,ks+1)-w(i,j1,ks)
            end do
            b(1,ks)=     t0*(-an1*dw(2)-an2*dw(3))+t1*dw(4)
            b(2,ks)=dw(1)                         -t2*dw(4)
            b(3,ks)=         -an2*dw(2)+an1*dw(3)
            b(m,ks)=     t0*( an1*dw(2)+an2*dw(3))+t1*dw(4)
            do i=5,m
              b(i-1,ks)=dw(i)
            end do
          end do
c
c eigenvalues * (dt/ds2) * rad / 2
          tmp=.5d0*rad*dr/ds(2)
          vn=an1*w0(2)+an2*w0(3)
          al(1)=tmp*(vn-c)
          al(2)=tmp*(vn  )
          do i=3,m-1
            al(i)=al(2)
          end do
          al(m)=tmp*(vn+c)
c
c C- contribution
          i=1
          if (ilimit) then
            if (b(i,1)*b(i,2).gt.0.d0 .or. .not.ilimit) then
              beta=dsign(1.d0,b(i,1))*dmin1(dabs(b(i,1)),dabs(b(i,2)))
              fact(1)=al(i)*beta
              fact(2)=fact(1)
              fact(3)=(min(al(i),0.d0)+.5d0)*beta
              fact(4)=(max(al(i),0.d0)-.5d0)*beta
              fact(5)=fact(1)
              do k=1,5
                w1(1,j1,k)=w1(1,j1,k)-fact(k)*rho
                w1(2,j1,k)=w1(2,j1,k)+fact(k)*c*an1
                w1(3,j1,k)=w1(3,j1,k)+fact(k)*c*an2
                w1(4,j1,k)=w1(4,j1,k)-fact(k)*rc2
              end do
            end if
          else
            beta=.5d0*(b(i,1)+b(i,2))
            fact(1)=al(i)*beta
            fact(2)=fact(1)
            fact(3)=(al(i)+.5d0)*beta
            fact(4)=(al(i)-.5d0)*beta
            fact(5)=fact(1)
            do k=1,5
              w1(1,j1,k)=w1(1,j1,k)-fact(k)*rho
              w1(2,j1,k)=w1(2,j1,k)+fact(k)*c*an1
              w1(3,j1,k)=w1(3,j1,k)+fact(k)*c*an2
              w1(4,j1,k)=w1(4,j1,k)-fact(k)*rc2
            end do
          end if
c
c P contribution
          i=2
          if (ilimit) then
            if (b(i,1)*b(i,2).gt.0.d0 .or. .not.ilimit) then
              beta=dsign(1.d0,b(i,1))*dmin1(dabs(b(i,1)),dabs(b(i,2)))
              fact(1)=al(i)*beta
              fact(2)=fact(1)
              fact(3)=(min(al(i),0.d0)+.5d0)*beta
              fact(4)=(max(al(i),0.d0)-.5d0)*beta
              fact(5)=fact(1)
              do k=1,5
                w1(1,j1,k)=w1(1,j1,k)-fact(k)
              end do
            end if
          else
            beta=.5d0*(b(i,1)+b(i,2))
            fact(1)=al(i)*beta
            fact(2)=fact(1)
            fact(3)=(al(i)+.5d0)*beta
            fact(4)=(al(i)-.5d0)*beta
            fact(5)=fact(1)
            do k=1,5
              w1(1,j1,k)=w1(1,j1,k)-fact(k)
            end do
          end if
c
c P contribution
          i=3
          if (ilimit) then
            if (b(i,1)*b(i,2).gt.0.d0 .or. .not.ilimit) then
              beta=dsign(1.d0,b(i,1))*dmin1(dabs(b(i,1)),dabs(b(i,2)))
              fact(1)=al(i)*beta
              fact(2)=fact(1)
              fact(3)=(min(al(i),0.d0)+.5d0)*beta
              fact(4)=(max(al(i),0.d0)-.5d0)*beta
              fact(5)=fact(1)
              do k=1,5
                w1(2,j1,k)=w1(2,j1,k)+fact(k)*an2
                w1(3,j1,k)=w1(3,j1,k)-fact(k)*an1
              end do
            end if
          else
            beta=.5d0*(b(i,1)+b(i,2))
            fact(1)=al(i)*beta
            fact(2)=fact(1)
            fact(3)=(al(i)+.5d0)*beta
            fact(4)=(al(i)-.5d0)*beta
            fact(5)=fact(1)
            do k=1,5
              w1(2,j1,k)=w1(2,j1,k)+fact(k)*an2
              w1(3,j1,k)=w1(3,j1,k)-fact(k)*an1
            end do
          end if
c
c P contributions (advected scalars)
c          if (.false.) then
          do i=4,m-1
            ip1=i+1
            if (ilimit) then
              if (b(i,1)*b(i,2).gt.0.d0 .or. .not.ilimit) then
                beta=dsign(1.d0,b(i,1))*dmin1(dabs(b(i,1)),dabs(b(i,2)))
                fact(1)=al(i)*beta
                fact(2)=fact(1)
                fact(3)=(min(al(i),0.d0)+.5d0)*beta
                fact(4)=(max(al(i),0.d0)-.5d0)*beta
                fact(5)=fact(1)
                do k=1,5
                  w1(ip1,j1,k)=w1(ip1,j1,k)-adiff*fact(k)
                end do
              end if
            else
              beta=.5d0*(b(i,1)+b(i,2))
              fact(1)=al(i)*beta
              fact(2)=fact(1)
              fact(3)=(al(i)+.5d0)*beta
              fact(4)=(al(i)-.5d0)*beta
              fact(5)=fact(1)
              do k=1,5
                w1(ip1,j1,k)=w1(ip1,j1,k)-fact(k)
              end do
            end if
          end do
c          end if
c
c C+ contribution
          i=m
          if (ilimit) then
            if (b(i,1)*b(i,2).gt.0.d0 .or. .not.ilimit) then
              beta=dsign(1.d0,b(i,1))*dmin1(dabs(b(i,1)),dabs(b(i,2)))
              fact(1)=al(i)*beta
              fact(2)=fact(1)
              fact(3)=(min(al(i),0.d0)+.5d0)*beta
              fact(4)=(max(al(i),0.d0)-.5d0)*beta
              fact(5)=fact(1)
              do k=1,5
                w1(1,j1,k)=w1(1,j1,k)-fact(k)*rho
                w1(2,j1,k)=w1(2,j1,k)-fact(k)*c*an1
                w1(3,j1,k)=w1(3,j1,k)-fact(k)*c*an2
                w1(4,j1,k)=w1(4,j1,k)-fact(k)*rc2
              end do
            end if
          else
            beta=.5d0*(b(i,1)+b(i,2))
            fact(1)=al(i)*beta
            fact(2)=fact(1)
            fact(3)=(al(i)+.5d0)*beta
            fact(4)=(al(i)-.5d0)*beta
            fact(5)=fact(1)
            do k=1,5
              w1(1,j1,k)=w1(1,j1,k)-fact(k)*rho
              w1(2,j1,k)=w1(2,j1,k)-fact(k)*c*an1
              w1(3,j1,k)=w1(3,j1,k)-fact(k)*c*an2
              w1(4,j1,k)=w1(4,j1,k)-fact(k)*rc2
            end do
          end if
c
c check for imaginary sound speeds
          do k=1,5
            rho=w1(1,j1,k)
            c2=((w1(5,j1,k)+1.d0)*w1(4,j1,k)+w1(6,j1,k))
     *          /(rho*w1(5,j1,k))
            if (c2.lt.c2min) then
              do i=1,m
                w1(i,j1,k)=w0(i)
              end do
            end if
          end do
c
        end if
      end do
c
      return
      end
c
c++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
c
      subroutine cmffree (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
     *                    dr,ds,j2,rx,det,rx2,det2,up,mask,w1,move)
c
c free-stream correction for non-Cartesian grids
c
      implicit real*8 (a-h,o-z)
      dimension rx(nd1a:nd1b,nd2a:nd2b,2,2),det(nd1a:nd1b,nd2a:nd2b),
     *          rx2(nd1a:nd1b,nd2a:nd2b,2,2),det2(nd1a:nd1b,nd2a:nd2b),
     *          up(nd1a:nd1b,nd2a:nd2b,m),mask(nd1a:nd1b),ds(2),fx(4),
     *          w1(m,md1a:md1b)
      common / rundat / ieos,irxn
c
      if (move.eq.0) then
c
c ratios
        dr1=dr/(4*ds(1))
        dr2=dr/(4*ds(2))
c
        do j1=n1a,n1b
          if (mask(j1).ne.0) then
c
c momenta
            ru=w1(1,j1)*w1(2,j1)
            rv=w1(1,j1)*w1(3,j1)
c
c total enthalpy
            h=w1(4,j1)+.5d0*(ru*w1(2,j1)+rv*w1(3,j1))
            if (irxn.eq.0) then
              h=h+w1(5,j1)*w1(4,j1)+w1(6,j1)
            else
              alam=w1(5,j1)
              amu1=(1.d0-alam)*w1(6,j1)+alam*w1(7,j1)
              amu2=(1.d0-alam)*w1(8,j1)+alam*w1(9,j1)
              amu3=                     alam*w1(10,j1)
              h=h+amu1*w1(4,j1)+amu2+w1(1,j1)*amu3
            end if
c
c average of the determinant
            d1p=det(j1+1,j2)+det(j1,j2)
            d1m=det(j1-1,j2)+det(j1,j2)
            d2p=det(j1,j2+1)+det(j1,j2)
            d2m=det(j1,j2-1)+det(j1,j2)
c
c x contribution
            k=1
            da=(((rx(j1+1,j2,1,k)+rx(j1,j2,1,k))*d1p
     *          -(rx(j1-1,j2,1,k)+rx(j1,j2,1,k))*d1m)*dr1
     *         +((rx(j1,j2+1,2,k)+rx(j1,j2,2,k))*d2p
     *          -(rx(j1,j2-1,2,k)+rx(j1,j2,2,k))*d2m)*dr2)/det(j1,j2)
            fx(1)=ru
            fx(2)=ru*w1(2,j1)+w1(4,j1)
            fx(3)=ru*w1(3,j1)
            fx(4)=h*w1(2,j1)
            do i=1,4
              up(j1,j2,i)=up(j1,j2,i)+da*fx(i)
            end do
c
c y contribution
            k=2
            da=(((rx(j1+1,j2,1,k)+rx(j1,j2,1,k))*d1p
     *          -(rx(j1-1,j2,1,k)+rx(j1,j2,1,k))*d1m)*dr1
     *         +((rx(j1,j2+1,2,k)+rx(j1,j2,2,k))*d2p
     *          -(rx(j1,j2-1,2,k)+rx(j1,j2,2,k))*d2m)*dr2)/det(j1,j2)
            fx(1)=rv
            fx(2)=rv*w1(2,j1)
            fx(3)=rv*w1(3,j1)+w1(4,j1)
            fx(4)=h*w1(3,j1)
            do i=1,4
              up(j1,j2,i)=up(j1,j2,i)+da*fx(i)
            end do
          end if
        end do
c
      else
c
c ratios
        dr1=dr/(16*ds(1))
        dr2=dr/(16*ds(2))
c
        do j1=n1a,n1b
          if (mask(j1).ne.0) then
c
c momenta
            ru=w1(1,j1)*w1(2,j1)
            rv=w1(1,j1)*w1(3,j1)
c
c total enthalpy
            h=w1(4,j1)+.5d0*(ru*w1(2,j1)+rv*w1(3,j1))
            if (irxn.eq.0) then
              h=h+w1(5,j1)*w1(4,j1)+w1(6,j1)
            else
              alam=w1(5,j1)
              amu1=(1.d0-alam)*w1(6,j1)+alam*w1(7,j1)
              amu2=(1.d0-alam)*w1(8,j1)+alam*w1(9,j1)
              amu3=                     alam*w1(10,j1)
              h=h+amu1*w1(4,j1)+amu2+w1(1,j1)*amu3
            end if
c
c average of the determinant
            det0=det(j1,j2)+det2(j1,j2)
            d1p=det(j1+1,j2)+det2(j1+1,j2)+det0
            d1m=det(j1-1,j2)+det2(j1-1,j2)+det0
            d2p=det(j1,j2+1)+det2(j1,j2+1)+det0
            d2m=det(j1,j2-1)+det2(j1,j2-1)+det0
c
c x contribution
            k=1
            rx10=rx(j1,j2,1,k)+rx2(j1,j2,1,k)
            rx20=rx(j1,j2,2,k)+rx2(j1,j2,2,k)
            da=(((rx(j1+1,j2,1,k)+rx2(j1+1,j2,1,k)+rx10)*d1p
     *          -(rx(j1-1,j2,1,k)+rx2(j1-1,j2,1,k)+rx10)*d1m)*dr1
     *         +((rx(j1,j2+1,2,k)+rx2(j1,j2+1,2,k)+rx20)*d2p
     *          -(rx(j1,j2-1,2,k)+rx2(j1,j2-1,2,k)+rx20)*d2m)*dr2)/det0
            fx(1)=ru
            fx(2)=ru*w1(2,j1)+w1(4,j1)
            fx(3)=ru*w1(3,j1)
            fx(4)=h*w1(2,j1)
            do i=1,4
              up(j1,j2,i)=up(j1,j2,i)+da*fx(i)
            end do
c
c y contribution
            k=2
            rx10=rx(j1,j2,1,k)+rx2(j1,j2,1,k)
            rx20=rx(j1,j2,2,k)+rx2(j1,j2,2,k)
            da=(((rx(j1+1,j2,1,k)+rx2(j1+1,j2,1,k)+rx10)*d1p
     *          -(rx(j1-1,j2,1,k)+rx2(j1-1,j2,1,k)+rx10)*d1m)*dr1
     *         +((rx(j1,j2+1,2,k)+rx2(j1,j2+1,2,k)+rx20)*d2p
     *          -(rx(j1,j2-1,2,k)+rx2(j1,j2-1,2,k)+rx20)*d2m)*dr2)/det0
            fx(1)=rv
            fx(2)=rv*w1(2,j1)
            fx(3)=rv*w1(3,j1)+w1(4,j1)
            fx(4)=h*w1(3,j1)
            do i=1,4
              up(j1,j2,i)=up(j1,j2,i)+da*fx(i)
            end do
          end if
        end do
      end if
c
      return
      end
c
c++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
c
c   multifluid flux calculations
c
c++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
c
      subroutine cmfflux (m,aj,a0,a1,a2,wl,wr,fl,fr,speed,ier)
c
c Compute Godunov fluxes for multifluid equations
c
c Input: wl,wr = left and right states in primitive variables
c
c Output: wl,wr = star-state and right-of-contact state
c                 if contact velocity > 0
c               = left-of-contact state and star-state
c                 if contact velocity < 0
c         fl,fr = fluxes including contribution from the contact
c         speed = largest wave speed (for time step calculation)
c
      implicit real*8 (a-h,o-z)
      dimension wl(m),wr(m),fl(m),fr(m),an(2),rm(2)
c      common / junk / iflg
c
c normalize metrics of the mapping
      rad=dsqrt(a1**2+a2**2)
c     an0=a0/rad            ! moving grids not implemented
      an(1)=a1/rad
      an(2)=a2/rad
      aj1=rad*aj
c
c      if (iflg.ne.0) then
c      write(44,*)'in..'
c      write(44,100)(wl(i),i=1,m),(wr(i),i=1,m)
c  100 format(2(6(1x,1pe22.15),/))
c      end if
c
c..compute middle state
      call cmfmiddle (m,an,wl,wr,rm,vm,pm,vmax,ier)
      if (ier.ne.0) return
      speed=max(rad*vmax,speed)
c
c      if (iflg.ne.0) then
c      write(44,*)'after middle'
c      write(44,100)(wl(i),i=1,m),(wr(i),i=1,m)
c      write(44,*)'vm,pm =',vm,pm
c      end if
c
c..compute flux
      if (vm.gt.0.d0) then
c
c    tangential component of the velocity from left state
        vt=an(1)*wl(3)-an(2)*wl(2)
c
c    right-of-contact state (only need velocity)
        wr(2)=an(1)*vm-an(2)*vt
        wr(3)=an(2)*vm+an(1)*vt
c
c   fluxes
        call cmfgetfx (1,m,rm,vm,pm,aj1,an,vt,fl)
        do i=1,4
          fr(i)=fl(i)
        end do
        do i=5,m
          fr(i)=-aj1*vm*(wr(i)-wl(i))
        end do
c
      else
c
c    tangential component of the velocity from right state
        vt=an(1)*wr(3)-an(2)*wr(2)
c
c    left-of-contact state (only need velocity)
        wl(2)=an(1)*vm-an(2)*vt
        wl(3)=an(2)*vm+an(1)*vt
c
c    fluxes
        call cmfgetfx (2,m,rm,vm,pm,aj1,an,vt,fr)
        do i=1,4
          fl(i)=fr(i)
        end do
        do i=5,m
          fl(i)= aj1*vm*(wr(i)-wl(i))
        end do
c
      end if
c
c      if (iflg.ne.0) then
c      write(44,*)'out...'
c      write(44,100)(wl(i),i=1,m),(wr(i),i=1,m)
c      write(44,*)'fluxes...'
c      write(44,100)(fl(i),i=1,m),(fr(i),i=1,m)
c      end if
c
c      pause
c
      return
      end
c
c++++++++++++++++
c
      subroutine cmfmiddle (m,an,wl,wr,rm,vm,pm,vmax,ier)
c
c see Toro, page 128, and algorithm, pages 156-7.
c
      implicit real*8 (a-h,o-z)
      dimension an(2),wl(m),wr(m),rm(2),vdif(2,2),ishock(2)
      common / rundat / ieos,irxn
      common / prmdat / r(2),v(2),p(2),g0(2),p0(2),q0(2),
     *                  a(2),b(2),c(2),gm1(2),gp1(2),em(2),ep(2)
      data ptol / 1.d-6 /
      data tol, itfix, itmax / 1.d-8, 3, 20 /
c
      ier=0
c
c left state
      r(1)=wl(1)
      v(1)=an(1)*wl(2)+an(2)*wl(3)
      p(1)=wl(4)
c
c right state
      r(2)=wr(1)
      v(2)=an(1)*wr(2)+an(2)*wr(3)
      p(2)=wr(4)
c
c gamma, pi and heat release for left and right states
      if (irxn.eq.0) then
        g0(1)=1.d0+1.d0/wl(5)
        p0(1)=wl(6)/(g0(1)*wl(5))
        q0(1)=0.d0
        g0(2)=1.d0+1.d0/wr(5)
        p0(2)=wr(6)/(g0(2)*wr(5))
        q0(2)=0.d0
      else
        alam=wl(5)
        amu1=(1.d0-alam)*wl(6)+alam*wl(7)
        amu2=(1.d0-alam)*wl(8)+alam*wl(9)
        q0(1)=                 alam*wl(10)
        g0(1)=1.d0+1.d0/amu1
        p0(1)=amu2/(g0(1)*amu1)
        alam=wr(5)
        amu1=(1.d0-alam)*wr(6)+alam*wr(7)
        amu2=(1.d0-alam)*wr(8)+alam*wr(9)
        q0(2)=                 alam*wr(10)
        g0(2)=1.d0+1.d0/amu1
        p0(2)=amu2/(g0(2)*amu1)
      end if
c
c loop over sides
      do i=1,2
c
c some constants involving gamma
        gm1(i)=g0(i)-1.d0
        gp1(i)=g0(i)+1.d0
        em(i)=0.5d0*gm1(i)/g0(i)
        ep(i)=0.5d0*gp1(i)/g0(i)
c
c sound speeds and some constants
        a(i)=2.d0/(gp1(i)*r(i))
        b(i)=gm1(i)*(p(i)+p0(i))/gp1(i)+p0(i)
        c2=g0(i)*(p(i)+p0(i))/r(i)
        if (c2.le.0.d0) then
          ier=4
          return
c          write(6,*)'Error (cmfmiddle) : c2.le.0, i =',i
c          stop
        end if
        c(i)=dsqrt(c2)
c
      end do

      pmin=min(p(1),p(2))
      pmax=max(p(1),p(2))
      p0min=min(p0(1),p0(2))
      emmin=min(em(1),em(2))
c
c start with guess based on a linearization
      df1=1.d0/(r(1)*c(1))
      df2=1.d0/(r(2)*c(2))
      ppv=(p(1)*df1+p(2)*df2-v(2)+v(1))/(df1+df2)
c
c the key is to pick the first guess for pstar
c to be less than the true value.  (The minimum
c value for pstar is -p0min which corresponds to
c a vacuum middle state.)
c
      if (pmin.le.ppv.and.pmax.ge.ppv) then
c this case is R-S or S-R
        pstar=ppv
      else
        if (ppv.lt.pmin) then
c this case is R-R
          d1=2.d0*c(1)/(gm1(1)*(p(1)+p0(1))**em(1))
          d2=2.d0*c(2)/(gm1(2)*(p(2)+p0(2))**em(2))
          z0=2.d0*c(1)/gm1(1)+2*c(2)/gm1(2)-(v(2)-v(1))
          if (p0(1).gt.p0(2)) then
            z0=z0-d1*(p0(1)-p0(2))**em(1)
          else
            z0=z0-d2*(p0(2)-p0(1))**em(2)
          end if
          if (z0.le.0.d0) then
            ier=3
            return
c            write(6,*)'Error (cmfmiddle) : vacuum state'
c            stop
          end if
          pstar=(z0/(d1+d2))**(1/emmin)-p0min
        else
c this case is S-S
          gl=dsqrt(a(1)/(ppv+b(1)))
          gr=dsqrt(a(2)/(ppv+b(2)))
          pts=(gl*p(1)+gr*p(2)-v(2)+v(1))/(gl+gr)
c (I think ptol should equal pmax here since
c  pstar is greater than pmax for this case.)
          pstar=max(ptol,pts)
        end if
      end if
c
c set ishock initially
      do i=1,2
        ishock(i)=1
        if (pstar.le.p(i)) ishock(i)=0
      end do
c
c      write(6,*)'  Newton iteration:'
c
c Newton iteration to find pstar, the pressure in the middle state
      it=0
    1 it=it+1
c
c determine velocity difference across a shock or rarefaction
      do i=1,2
        if (ishock(i).eq.1) then
          arg=pstar+b(i)
          if (arg.le.0.d0) then
            ier=5
            return
          end if
          fact=dsqrt(a(i)/arg)
          diff=pstar-p(i)
          vdif(1,i)=fact*diff
          vdif(2,i)=fact*(1.d0-0.5d0*diff/arg)
        else
          arg=(pstar+p0(i))/(p(i)+p0(i))
          if (arg.le.0.d0) then
            ier=6
            return
          end if
          fact=2.d0*c(i)/gm1(i)
          vdif(1,i)=fact*(arg**em(i)-1.d0)
          vdif(2,i)=1.d0/(r(i)*c(i)*arg**ep(i))
        end if
      end do
c
c determine change to pressure in the middle state
      dp=(vdif(1,1)+vdif(1,2)+v(2)-v(1))/(vdif(2,1)+vdif(2,2))
c
c check for convergence
      if (dabs(dp).gt.tol*max(pstar,1.d0)) then
        if (it.lt.itmax) then
c          write(6,100)it,pstar,dp,ishock
c  100     format(4x,' it=',i2,',  pstar=',f12.5,',  dp=',1pe9.2,
c     *           2(2x,i1))
          pstar=pstar-dp
          if (it.le.itfix) then
            do i=1,2
              ishock(i)=1
              if (pstar.le.p(i)) ishock(i)=0
            end do
          end if
          goto 1
        else
          ier=2
          return
c          write(6,*)'Error (cmfmiddle) : itmax exceeded'
c          stop
        end if
      end if
c
c      write(6,101)it,pstar,dp
c  101 format(4x,' it=',i2,',  pstar=',f12.5,',  dp=',1pe9.2,
c     *       '  converged')
c
      pm=pstar
      vm=.5d0*(v(1)-vdif(1,1)+v(2)+vdif(1,2))
c
      vmax=max(c(1)+dabs(v(1)),c(2)+dabs(v(2)))
      do i=1,2
        if (pm.gt.p(i)) then
          rm(i)=r(i)*(gp1(i)*(pm+p0(i))/(p(i)+p0(i))+gm1(i))
     *              /(gm1(i)*(pm+p0(i))/(p(i)+p0(i))+gp1(i))
        else
          rm(i)=r(i)*((pm+p0(i))/(p(i)+p0(i)))**(1.d0/g0(i))
        end if
        cm2=g0(i)*(pm+p0(i))/rm(i)
        if (cm2.lt.0.d0) then
          write(66,667)an(1),an(2),(wl(ii),wr(ii),ii=1,m),
     *                 pm,dp,vm,i,g0(i),p0(i),rm(i),cm2
  667     format(2(1x,1pe15.8),/,6(2(1x,1pe15.8),/),3(1x,1pe15.8),/,
     *           1x,i1,4(1x,1pe15.8))
          ier=1
          return
c          write(6,*)'Error (cmfmiddle) : cm2.le.0, i =',i
c          stop
        end if
        vmax=max(vmax,dsqrt(cm2)+dabs(vm))
      end do
c
      return
      end
c
c+++++++++++++++
c
      subroutine cmfgetfx (i,m,rm,vm,pm,aj,an,vt,fx)
c
c compute flux for i=side
c
      implicit double precision (a-h,o-z)
      dimension rm(2),an(2),wstar(4),fx(m)
      common / prmdat / r(2),v(2),p(2),g0(2),p0(2),q0(2),
     *                  a(2),b(2),c(2),gm1(2),gp1(2),em(2),ep(2)
c
c set isign=1 for i=1 and isign=-1 for i=2
      isgn=3-2*i
c
      if (pm.gt.p(i)) then
c shock cases
        sp=isgn*v(i)-c(i)*dsqrt(ep(i)*(pm+p0(i))/(p(i)+p0(i))+em(i))
        if (sp.ge.0.d0) then
c left (i=1) or right (i=2)
          wstar(1)=r(i)
          wstar(2)=an(1)*v(i)-an(2)*vt
          wstar(3)=an(2)*v(i)+an(1)*vt
          wstar(4)=p(i)
        else
c middle left (i=1) or middle right (i=2)
          wstar(1)=rm(i)
          wstar(2)=an(1)*vm-an(2)*vt
          wstar(3)=an(2)*vm+an(1)*vt
          wstar(4)=pm
        end if
      else
c rarefaction cases
        if (isgn*v(i)-c(i).ge.0.d0) then
c left (i=1) or right (i=2)
          wstar(1)=r(i)
          wstar(2)=an(1)*v(i)-an(2)*vt
          wstar(3)=an(2)*v(i)+an(1)*vt
          wstar(4)=p(i)
        else
c left middle or sonic (i=1) or right middle or sonic (i=2)
          if (isgn*vm-dsqrt(g0(i)*(pm+p0(i))/rm(i)).gt.0.d0) then
c sonic left (i=1) or right (i=2)
            arg=(2.d0+isgn*gm1(i)*v(i)/c(i))/gp1(i)
            vn=c(i)*arg*isgn
            wstar(1)=r(i)*arg**(2.d0/gm1(i))
            wstar(2)=an(1)*vn-an(2)*vt
            wstar(3)=an(2)*vn+an(1)*vt
            wstar(4)=(p(i)+p0(i))*arg**(2.d0*g0(i)/gm1(i))-p0(i)
          else
c middle left (i=1) or right (i=2)
            wstar(1)=rm(i)
            wstar(2)=an(1)*vm-an(2)*vt
            wstar(3)=an(2)*vm+an(1)*vt
            wstar(4)=pm
          end if
        end if
      end if
c
c compute flux
      vn=an(1)*wstar(2)+an(2)*wstar(3)
      fact=wstar(1)*(wstar(2)**2+wstar(3)**2)
      energy=(wstar(4)+g0(i)*p0(i))/gm1(i)+wstar(1)*q0(i)+.5d0*fact
c
      fact=wstar(1)*vn
      fx(1)=aj*fact
      fx(2)=aj*(fact*wstar(2)+an(1)*wstar(4))
      fx(3)=aj*(fact*wstar(3)+an(2)*wstar(4))
      fx(4)=aj*(energy+wstar(4))*vn
      do k=5,m
        fx(k)=0.d0
      end do
c
      return
      end
c
c++++++++++++++++++++
c
      subroutine gethtz (m,x,y,t,htz,iprim)
c
c compute tz forcing function (for hydro part)
c
      implicit real*8 (a-h,o-z)
      dimension htz(m),wt(10),wx(10),wy(10),w0(10)
      common / axidat / iaxi,j1axi(2),j2axi(2)
      common / rundat / ieos,irxn
      common / tzflow / eptz,itz
c
c      write(76,100)x,y,t
c  100 format(' ***',3(1x,1pe15.8))
      do i=1,m
        call ogDeriv (eptz,0,0,0,0,x,y,0.d0,t,i-1,w0(i))
        call ogDeriv (eptz,1,0,0,0,x,y,0.d0,t,i-1,wt(i))
        call ogDeriv (eptz,0,1,0,0,x,y,0.d0,t,i-1,wx(i))
        call ogDeriv (eptz,0,0,1,0,x,y,0.d0,t,i-1,wy(i))
c        write(76,200)i,w0(i),wt(i),wx(i),wy(i)
c  200   format(1x,i2,4(1x,1pe15.8))
      end do
c
c use T=p/rho to convert to p
      wt(4)=w0(1)*wt(4)+wt(1)*w0(4)
      wx(4)=w0(1)*wx(4)+wx(1)*w0(4)
      wy(4)=w0(1)*wy(4)+wy(1)*w0(4)
      w0(4)=w0(1)*w0(4)
c
c variables
      rho=w0(1)
      u=w0(2)
      v=w0(3)
      p=w0(4)
      q2=u**2+v**2
c
c sound speed
      if (irxn.eq.0) then
        c2=((w0(5)+1.d0)*p+w0(6))/(rho*w0(5))
      else
        alam=w0(5)
        amu1=(1.d0-alam)*w0(6)+alam*w0(7)
        amu2=(1.d0-alam)*w0(8)+alam*w0(9)
        c2=((amu1+1.d0)*w0(4)+amu2)/(rho*amu1)
      end if
c
c TZ forcings in primitive form
      do i=1,m
        htz(i)=wt(i)+u*wx(i)+v*wy(i)
      end do
      htz(1)=htz(1)+rho*(wx(2)+wy(3))
      htz(2)=htz(2)+wx(4)/rho
      htz(3)=htz(3)+wy(4)/rho
      htz(4)=htz(4)+rho*c2*(wx(2)+wy(3))
c
c Axisymmetric contribution (if necessary)
      ymin=1.d-8
      if (iaxi.ne.0) then
        if (dabs(y).gt.ymin) then
          vy=v/y
        else
          vy=wy(3)
        end if
        htz(1)=htz(1)+rho*vy
        htz(4)=htz(4)+rho*c2*vy
      end if
c
c TZ forcings in the conservative form (if necessary)
      if (iprim.eq.0) then
        if (irxn.eq.0) then
          htz(4)=.5d0*q2*htz(1)+rho*(u*htz(2)+v*htz(3))
     *             +w0(5)*htz(4)+p*htz(5)+htz(6)
        else
          amu1p=(1.d0-alam)*htz(6)+alam*htz(7)+(w0(7)-w0(6))*htz(5)
          amu2p=(1.d0-alam)*htz(8)+alam*htz(9)+(w0(9)-w0(8))*htz(5)
          amu3=alam*w0(10)
          amu3p=alam*htz(10)+w0(10)*htz(5)
          htz(4)=.5d0*q2*htz(1)+rho*(u*htz(2)+v*htz(3))
     *             +amu1*htz(4)+p*amu1p+amu2p+amu3*htz(1)+rho*amu3p
        end if
        htz(3)=v*htz(1)+rho*htz(3)
        htz(2)=u*htz(1)+rho*htz(2)
      end if
c
      return
      end
c
c++++++++++++++++++++
c
      subroutine cmfplimit (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                      u,mask,w)
c
c limit pressure
c
      implicit real*8 (a-h,o-z)
      dimension u(nd1a:nd1b,nd2a:nd2b,m),mask(nd1a:nd1b,nd2a:nd2b),
     *          w(m,n1a:n1b)
      common / rundat / ieos,irxn
      data plimit / 0.d0 /
c
      do j2=n2a,n2b
c
        call cmfpvs (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,j2,u,w)
c
        do j1=n1a,n1b
c
          p=w(4,j1)
          if (p.lt.plimit) then
            p=plimit
            if (irxn.eq.0) then
              en=p*w(5,j1)+w(6,j1)
            else
              alam=w(5,j1)
              amu1=(1.d0-alam)*w(6,j1)+alam*w(7,j1)
              amu2=(1.d0-alam)*w(8,j1)+alam*w(9,j1)
              amu3=                    alam*w(10,j1)
              en=p*amu1+amu2+w(1,j1)*amu3
            end if
            u(j1,j2,4)=en+.5d0*w(1,j1)*(w(2,j1)**2+w(3,j1)**2)
          end if
c
        end do
      end do
c
      return
      end
