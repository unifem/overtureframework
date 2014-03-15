      subroutine cmpdu (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                  dr,ds1,ds2,r,rx,gv,det,rx2,gv2,det2,xy,
     *                  u,up,mask,ntau,tau,ad,mdat,dat,nrprm,rparam,
     *                  niprm,iparam,nrwk,rwk,niwk,iwk,idebug,ier)
c
c Multi-phase flow, BN equations, with advected scalars
c
      implicit real*8 (a-h,o-z)
      dimension rx(*),gv(*),det(*),rx2(*),gv2(*),det2(*),xy(*),
     *          u(nd1a:nd1b,nd2a:nd2b,m),up(nd1a:nd1b,nd2a:nd2b,m),
     *          mask(nd1a:nd1b,nd2a:nd2b),tau(ntau),ad(m),
     *          dat(nd1a:nd1b,nd2a:nd2b,*),rparam(nrprm),iparam(niprm),
     *          rwk(nrwk),iwk(niwk)
      dimension ds(2),almax(2)
c
      dimension ic(7)
c      common / junk / ijunk(9,-5:300)
c
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0,bgas
      common / srcprm / delta,rmuc,htrans,cratio,abmin,abmax,isrc
c
      common / srcdat / nb1a,nb1b,nb2a,nb2b,icount
      common / timing / tflux(10),tslope,tsource
      common / axidat / iaxi,j1axi(2),j2axi(2)
      common / cutdat / abmin1
c
      data ic / 9,1,2,4,5,6,8 /
c     data ic / 9,1,3,4,5,7,8 /
c
c     write(6,*)'cmpdu start...'
c      write(6,343)n1a,n1b,n2a,n2b,r
c  343 format('** Start cmpdu: ',4(1x,i4),', time =',f15.8)
c
c..set array dimensions for parallel
      md1a=max(nd1a,n1a-2)
      md1b=min(nd1b,n1b+2)
      md2a=max(nd2a,n2a-2)
      md2b=min(nd2b,n2b+2)
c
c cutoff for alpha_bar.  If alpha_bar is less than abmin1, then
c the solid is considered to be gone.  In this case, the source
c calculation is turned off and the flux is taken to be decoupled.
c Since the alpha_bar asymptotes to abmin, abmin1 should be
c somewhat bigger
      abmin1=5.d0*abmin
c
      if (m.lt.0) then
        call chkp (m,nd1a,nd1b,md1a,md1b,nd2a,nd2b,md2a,md2b,u,rwk)
      end if
c
      if (m.lt.0.and.iparam(9).ge.0) then
        write(55,434)dr,ds1,ds2
  434   format('dr,ds1,ds2=',3(1x,1pe10.3),/,
     *         'cmpdu start, u=')
        do j1=n1a,n1b
          write(55,234)j1,(u(j1,n2a,ic(k)),k=1,7)
  234     format(1x,i3,7(1x,1pe15.8))
        end do
c        do j2=n2a,n2b
c          write(55,234)j2,(u(n1a,j2,ic(k)),k=1,7)
c  234     format(1x,i3,7(1x,1pe15.8))
c        end do
c        write(6,*)dr
c        pause
c        if (m.lt.0) stop
      end if
c
c..set error flag
      ier=0
c
c..sanity check
      if (m.lt.9) then
        write(6,*)'Error (cmpdu) : m.ge.9 is assumed'
        stop
      else
        if (iparam(9).le.0) then
          nscalar=m-9
          write(6,*)'Info (cmpdu) : advected scalars = ',nscalar
        end if
      end if
c
      if (m.lt.0) then
        write(6,*)'cmpdu(in)'
        write(6,'(9(1x,i1,1x,f15.8,/))')(i,u(0,0,i),i=1,9)
        write(6,*)gam,gm1
        pause
      end if
c
      if( idebug.gt.0 ) write(6,990)r,dr
c     write(6,990)r,dr
  990 format('** Starting cmpdu...t,dt =',f8.5,1x,1pe9.2)
c     write(6,991)n1a,n1b,n2a,n2b
c 991 format('      Grid bounds =',4(1x,i5))
c
c..mesh spacings
      ds(1)=ds1
      ds(2)=ds2
c
c..zero out timings
      do i=1,10
        tflux(i)=0.d0
      end do
      tslope=0.d0
      tsource=0.d0
c
c..set boundary dimensions for counting source sub-time steps
c  and zero out counter
      nb1a=n1a
      nb1b=n1b
      nb2a=n2a
      nb2b=n2b
      icount=0
c
c..parameters
c
c    iparam(1) =EOS model          (iparam(1)=0 => stiffened solid, ideal gas)
c    iparam(2) =Reaction model     (iparam(2)=0 => no chemical reaction)
c    iparam(3) =move               is the grid moving (=0 => no)
c    iparam(4) =icart              Cartesian grid (=1 => yes)
c    iparam(5) =iorder             order of the method (=1 or 2)
c    iparam(6) =method             method of flux calculation (=0 => adaptive MP)
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
      if (irxn.ne.0) then
        write(6,*)'Error (cmpdu) : irxn not supported'
        stop
      end if
c
      move=iparam(3)
      icart=iparam(4)
      iorder=iparam(5)
      method=iparam(6)
      igrid=iparam(7)
      level=iparam(8)
      nstep=iparam(9)
      av=rparam(3)
c
c low pressure-density fix
      call cmplowfix (m,nd1a,nd1b,md1a,md1b,nd2a,nd2b,md2a,md2b,
     *                u,mask,rwk,nstep,igrid,level)
c
c      write(6,943)r,nstep,igrid,level
c  943 format('** Start cmpdu: t=',1pe12.5,'  nstep=',i3,'  grid=',i3,
c     *       '  level=',i2)
c
      if (nstep.ge.0.and.m.lt.0) then
        write(44,201)r
  201   format('time = ',f15.7)
        do j2=n2a-2,n2b+2
          do j1=n1a-2,n1b+2
            write(44,301)j1,j2,(u(j1,j2,i),u(j1,j2,i+4),i=1,4)
  301       format(2(1x,i3),4(/,3x,2(1x,1pe22.15)))
          end do
        end do
      end if
c
c
c      write(6,*)'ad =',ad
c      write(6,*)'av =',av
c      pause
c
c     write(6,*)'Warning (cmpdu) : setting iorder=2'
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
c..axisymmetric problem?  *** CHECK limits here for parallel ***
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
                write(6,*)'Error (cmpdu) : inconsistent mask value'
              end if
            end do
            end do
          end if
        end do
        end do
      end if
c
      if (m.lt.0) then
        call chks (m,nd1a,nd1b,md1a,md1b,nd2a,nd2b,md2a,md2b,
     *             u,mask,rwk,ier,1)
        if (ier.ne.0) then
          write(6,*)'Error (cmpdu) : error return from chks (1)'
          stop
        end if
      end if
c
c..monitor data (if idebug>0)
c      if (idebug.gt.0) then
c        call mondat2d (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
c     *                 r,dr,u,rwk,mask)
c      end if
c
c..limit alphaSolid and check for negative densities

      ! *wdh* 
c      write(*,'(" cmpduv: grid=",i3," nd1a,...=",4i5)') igrid,
c     & nd1a,nd1b,nd2a,nd2b

      abtiny=1.d-8
      rhoMin=1.d-8
      do j2=md2a,md2b
      do j1=md1a,md1b
        if (mask(j1,j2).ne.0) then
          u(j1,j2,9)=min(1.d0-abtiny,max(abtiny,u(j1,j2,9)))
          rs=u(j1,j2,1)/u(j1,j2,9)
          if (rs.le.rhoMin) then
            write(6,100)j1,j2
  100       format(' ** Error (cmpdu) : very small solid density, ',
     *             'j1,j2 =',2(1x,i4))
            u(j1,j2,1)=rhoMin*u(j1,j2,9)
          end if
          rg=u(j1,j2,5)/(1.d0-u(j1,j2,9))
          if (rg.le.rhoMin) then
            write(6,110)j1,j2
  110       format(' ** Error (cmpdu) : very small gas density, ',
     *             'j1,j2 =',2(1x,i4))
            u(j1,j2,5)=rhoMin*(1.d0-u(j1,j2,9))
          end if
        end if
      end do
      end do
c
      if (m.lt.0) then
        call chks (m,nd1a,nd1b,md1a,md1b,nd2a,nd2b,md2a,md2b,
     *             u,mask,rwk,ier,2)
        if (ier.ne.0) then
          write(6,*)'Error (cmpdu) : error return from chks (2)'
          stop
        end if
      end if
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
c truncation error estimate returned by cmpsource)
      if (isrc.ne.0) then
        dr2=.5d0*dr
        do k=1,ntau
          tau(k)=0.d0
        end do
        call cmpsource (m,nd1a,nd1b,md1a,md1b,nd2a,nd2b,md2a,md2b,dr2,
     *                  up,mask,tau,maxstep1,niwk,iwk,nrwk,rwk)
      end if
c
      if (m.lt.0) then
        call chks (m,nd1a,nd1b,md1a,md1b,nd2a,nd2b,md2a,md2b,
     *             up,mask,rwk,ier,3)
        if (ier.ne.0) then
          write(6,*)'Error (cmpdu) : error return from chks (3)'
          call prts (m,nd1a,nd1b,md1a,md1b,nd2a,nd2b,md2a,md2b,
     *               u,rwk,60)
        end if
      end if
c
c low pressure-density fix
c      call cmplowfix (m,nd1a,nd1b,md1a,md1b,nd2a,nd2b,md2a,md2b,
c     *                up,mask,rwk)
c
c zero out almax
      almax(1)=0.d0
      almax(2)=0.d0
c
      if (m.lt.0) then
        call chkp (m,nd1a,nd1b,md1a,md1b,nd2a,nd2b,md2a,md2b,u,rwk)
      end if
c
c      esym=0.d0
c      do j1=nd1a,nd1b
c      do j2=nd2a,nd2b
c        esym=max(dabs(up(j1,j2,1)-up(j2,j1,1)),esym)
c        esym=max(dabs(up(j1,j2,2)-up(j2,j1,3)),esym)
c        esym=max(dabs(up(j1,j2,3)-up(j2,j1,2)),esym)
c        esym=max(dabs(up(j1,j2,4)-up(j2,j1,4)),esym)
c        esym=max(dabs(up(j1,j2,5)-up(j2,j1,5)),esym)
c        esym=max(dabs(up(j1,j2,6)-up(j2,j1,7)),esym)
c        esym=max(dabs(up(j1,j2,7)-up(j2,j1,6)),esym)
c        esym=max(dabs(up(j1,j2,8)-up(j2,j1,8)),esym)
c        esym=max(dabs(up(j1,j2,9)-up(j2,j1,9)),esym)
c      end do
c      end do
c      write(6,452)esym
c  452 format('   error(symmetry)=',1pe9.2)
c
c hydro step (handles slope correction, flux, and free-stream correction)
      call cmphydro (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,md2a,md2b,
     *               n2a,n2b,dr,ds,r,rx,gv,det,rx2,gv2,det2,xy,up,
     *               mask,almax,rwk(lw),rwk(lw1),rwk(la0),rwk(la1),
     *               rwk(laj),rwk(lda0),rwk(lvaxi),mdat,dat,move,
     *               maxnstep,icart,iorder,method,n1bm,n2bm,ier)
c
      if (ier.ne.0) return
c
c low pressure-density fix
c      call cmplowfix (m,nd1a,nd1b,md1a,md1b,nd2a,nd2b,md2a,md2b,
c     *                up,mask,rwk)
c
c      esym=0.d0
c      do j1=nd1a,nd1b
c      do j2=nd2a,nd2b
c        esym=max(dabs(up(j1,j2,1)-up(j2,j1,1)),esym)
c        esym=max(dabs(up(j1,j2,2)-up(j2,j1,3)),esym)
c        esym=max(dabs(up(j1,j2,3)-up(j2,j1,2)),esym)
c        esym=max(dabs(up(j1,j2,4)-up(j2,j1,4)),esym)
c        esym=max(dabs(up(j1,j2,5)-up(j2,j1,5)),esym)
c        esym=max(dabs(up(j1,j2,6)-up(j2,j1,7)),esym)
c        esym=max(dabs(up(j1,j2,7)-up(j2,j1,6)),esym)
c        esym=max(dabs(up(j1,j2,8)-up(j2,j1,8)),esym)
c        esym=max(dabs(up(j1,j2,9)-up(j2,j1,9)),esym)
c      end do
c      end do
c      write(6,452)esym
c
      if (m.lt.0) then
        call chks (m,nd1a,nd1b,md1a,md1b,nd2a,nd2b,md2a,md2b,
     *             up,mask,rwk,ier,4)
        if (ier.ne.0) then
          write(6,*)'Error (cmpdu) : error return from chks (4)'
c         write(6,*)'r =',r
          call prts (m,nd1a,nd1b,md1a,md1b,nd2a,nd2b,md2a,md2b,
     *               u,rwk,61)
          stop
        end if
      end if
c
      if (m.lt.0) then
        write(55,*)'up ='
c       do j1=n1a,n1b
c         write(55,333)j1,(up(j1,n2a,ic(k)),k=1,7)
c 333     format(1x,i2,7(/,1x,1pe15.8))
c       end do
        do j2=n2a,n2b
          write(55,333)j2,(up(n1a,j2,ic(k)),k=1,7)
  333     format(1x,i2,7(/,1x,1pe15.8))
        end do
c       stop
      end if
c
c      if (m.lt.0.and.nstep.ge.0) then
c        write(55,453)r
c  453   format('** t=',f12.4)
c        do j1=n1a,n1b
c          write(55,678)j1,up(j1,n2a,1),up(j1,n2a,9),(ijunk(i,j1),i=1,9)
c  678     format(1x,i3,2(1x,1pe22.15),9(1x,i2))
c        end do
c      end if
c
c compute second source contribution
      if (isrc.ne.0) then
        call cmpsource (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,dr2,
     *                  up,mask,tau,maxstep2,niwk,iwk,nrwk,rwk)
c       write(6,*)'** cmpSource : maximum time steps = ',
c    *            max(maxstep1,maxstep2)
        call setdat (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
     *               md2a,md2b,n2a,n2b,tau,mdat,dat)
      end if
c
      if (m.lt.0) then
        call chks (m,nd1a,nd1b,md1a,md1b,nd2a,nd2b,md2a,md2b,
     *             up,mask,rwk,ier,5)
        if (ier.ne.0) then
          write(6,*)'Error (cmpdu) : error return from chks (5)'
c          call prts (m,nd1a,nd1b,md1a,md1b,nd2a,nd2b,md2a,md2b,
c     *               u,rwk,62)
          stop
        end if
      end if
c
      if (m.lt.0.and.iparam(9).ge.0) then
        write(55,*)'up ='
        do j1=n1a,n1b
          write(55,339)j1,(up(j1,n2a,ic(k)),k=1,7)
  339     format(1x,i3,7(1x,1pe15.8))
        end do
c        do j2=n2a,n2b
c          write(55,339)j2,(up(n1a,j2,ic(k)),k=1,7)
c  339     format(1x,i3,7(1x,1pe15.8))
c        end do
        stop
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
c low pressure-density fix
c      call cmplowfix (m,nd1a,nd1b,md1a,md1b,nd2a,nd2b,md2a,md2b,
c     *                up,mask,rwk)
c
c artificial viscosity (and zero up in ghost cells)
      call cmpvisc (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,md2a,md2b,
     *              n2a,n2b,ds,rx,u,up,mask,rwk,av,vismax,icart,ad,
     *              n1bm,n2bm)
c
c      esym=0.d0
c      do j1=nd1a,nd1b
c      do j2=nd2a,nd2b
c        esym=max(dabs(up(j1,j2,1)-up(j2,j1,1)),esym)
c        esym=max(dabs(up(j1,j2,2)-up(j2,j1,3)),esym)
c        esym=max(dabs(up(j1,j2,3)-up(j2,j1,2)),esym)
c        esym=max(dabs(up(j1,j2,4)-up(j2,j1,4)),esym)
c        esym=max(dabs(up(j1,j2,5)-up(j2,j1,5)),esym)
c        esym=max(dabs(up(j1,j2,6)-up(j2,j1,7)),esym)
c        esym=max(dabs(up(j1,j2,7)-up(j2,j1,6)),esym)
c        esym=max(dabs(up(j1,j2,8)-up(j2,j1,8)),esym)
c        esym=max(dabs(up(j1,j2,9)-up(j2,j1,9)),esym)
c      end do
c      end do
c      write(6,452)esym
c
c**** TEMPORARY ****
c      do i=1,4
c        do j1=n1a,n1b
c        do j2=n2a,n2b
c          up(j1,j2,i+4)=up(j1,j2,i)
c        end do
c        end do
c      end do
c
c compute real and imaginary parts of lambda, where the time stepping
c is interpreted as u'=lambda*u
c
      iparam(10)=icount
      rparam(1)=4.d0*vismax
      rparam(2)=almax(1)/ds(1)+almax(2)/ds(2)
c
c timings
      rparam(31)=tflux(1)
      rparam(32)=tslope
      rparam(33)=tsource
      rparam(34)=tflux(2)
      rparam(35)=tflux(3)
      rparam(36)=tflux(4)
      rparam(37)=tflux(5)
c
      if (idebug.gt.0) then
        write(6,105)rparam(1),rparam(2),maxnstep
      end if
  105 format('max(lambda) =',2(1pe9.2,1x),',  max(RxnSteps) =',i6)
c
      if( idebug.gt.0 ) write(6,*)'...done: cmpdu'
c     write(6,*)'...done: cmpdu'
c      write(6,944)
c  944 format('** done')
c
c
      return
      end
c
c***************
c
      subroutine cmplowfix0 (m,nd1a,nd1b,md1a,md1b,nd2a,nd2b,
     *                      md2a,md2b,u,w)
c
c original version - add mask to argument list or do not use ****
c
      implicit real*8 (a-h,o-z)
      dimension u(nd1a:nd1b,nd2a:nd2b,m),w(m,md1a:md1b)
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0,bgas
      data psmin / 0.d0 /
c
      do j2=md2a,md2b
        call cmppvs (m,nd1a,nd1b,md1a,md1b,nd2a,nd2b,j2,u,w)
        do j1=md1a,md1b
          if (w(4,j1).le.psmin) then
            u(j1,j2,4)=w(9,j1)*((psmin+gam(1)*ps0)/(gam(1)-1.d0)
     *                 +w(1,j1)*(.5d0*(w(2,j1)**2+w(3,j1)**2)
     *                          +compac(w(9,j1),0)))
          end if
        end do
      end do
c
      return
      end
c
c***************
c
      subroutine cmplowfix (m,nd1a,nd1b,md1a,md1b,nd2a,nd2b,
     *                      md2a,md2b,u,mask,w,nstep,igrid,level)
c
c new version: checks for low pressures and densities
c
      implicit real*8 (a-h,o-z)
      dimension u(nd1a:nd1b,nd2a:nd2b,m),mask(nd1a:nd1b,nd2a:nd2b),
     *          w(m,md1a:md1b)
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0,bgas
      data rbmin, pbmin, rmin, pmin / 1.d-2, 0.d0, 1.d-6, 1.d-2 /
c
      iflag=0
      do j2=md2a,md2b
        call cmppvs (m,nd1a,nd1b,md1a,md1b,nd2a,nd2b,j2,u,w)
        do j1=md1a,md1b
          if (mask(j1,j2).ne.0.and.w(9,j1).lt.1.d-3) then
            ichg=0
            if (w(1,j1).le.rbmin) then
c              write(6,*)'Limiting rho_solid=',j1,j2,w(1,j1)
              w(1,j1)=rbmin
              ichg=1
            end if
            if (w(4,j1).le.pbmin) then
c              write(6,*)'Limiting p_solid=',j1,j2,w(4,j1)
              w(4,j1)=pbmin
              ichg=1
            end if
            if (w(5,j1).le.rmin) then
c              write(6,*)'Limiting rho_gas=',j1,j2,w(5,j1)
              w(5,j1)=rmin
              ichg=1
            end if
            if (w(8,j1).le.pmin) then
c              write(6,*)'Limiting p_gas=',j1,j2,w(8,j1)
              w(8,j1)=pmin
              ichg=1
            end if
            if (ichg.ne.0) then
              iflag=1
              u(j1,j2,1)=w(9,j1)*w(1,j1)
              u(j1,j2,2)=u(j1,j2,1)*w(2,j1)
              u(j1,j2,3)=u(j1,j2,1)*w(3,j1)
              en=(w(4,j1)+gam(1)*ps0)/gm1(1)
     *            +.5d0*w(1,j1)*(w(2,j1)**2+w(3,j1)**2)
              u(j1,j2,4)=w(9,j1)*(en+w(1,j1)*compac(w(9,j1),0))
              u(j1,j2,5)=(1.d0-w(9,j1))*w(5,j1)
              u(j1,j2,6)=u(j1,j2,5)*w(6,j1)
              u(j1,j2,7)=u(j1,j2,5)*w(7,j1)
              en=w(8,j1)/(gm1(2)*(1.d0+bgas*w(5,j1)))
     *           +.5d0*w(5,j1)*(w(6,j1)**2+w(7,j1)**2)
              u(j1,j2,8)=(1.d0-w(9,j1))*en
              u(j1,j2,9)=w(9,j1)
            end if
          end if
        end do
      end do
c
c     if (iflag.ne.0) then
c       write(6,100)nstep,igrid,level
c 100   format('** limiting occured at nstep=',i8,',  grid=',i5,
c    *         ',  level=',i2)
c     end if
c
      return
      end
c
c***************
c
      subroutine chkp (m,nd1a,nd1b,md1a,md1b,nd2a,nd2b,md2a,md2b,u,w)
c
      implicit real*8 (a-h,o-z)
      dimension u(nd1a:nd1b,nd2a:nd2b,m),w(m,md1a:md1b)
c
      do j2=md2a,md2b
        call cmppvs (m,nd1a,nd1b,md1a,md1b,nd2a,nd2b,j2,u,w)
        do j1=md1a,md1b
          if (w(4,j1).le.0.d0.or.w(8,j1).le.0.d0) then
            write(6,100)nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                  j1,j2,(i,w(i,j1),i=1,m)
  100       format('** negative pressure found **',/,
     *             6(1x,i5),9(/,1x,i2,1x,1pe22.15))
            pause
          end if
        end do
      end do
c
      return
      end
c
c***************
c
      subroutine chks (m,nd1a,nd1b,md1a,md1b,nd2a,nd2b,md2a,md2b,
     *                 u,mask,w,ier,loc)
c
      implicit real*8 (a-h,o-z)
      dimension u(nd1a:nd1b,nd2a:nd2b,m),mask(nd1a:nd1b,nd2a:nd2b),
     *          w(m,md1a:md1b)
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0,bgas
      data c2min / 1.d-6 /
c
      ier=0
      do j2=md2a,md2b
        call cmppvs (m,nd1a,nd1b,md1a,md1b,nd2a,nd2b,j2,u,w)
        do j1=md1a,md1b
          if (mask(j1,j2).ne.0) then
            cb2=gam(1)*(w(4,j1)+ps0)/w(1,j1)
            if (cb2.lt.c2min) then
              ier=1
              write(6,*)'j1,j2,cb2=',j1,j2,cb2
c             goto 1
            end if
            fact=1.d0+bgas*w(5,j1)
            c2=w(8,j1)*(gam(2)/w(5,j1)
     *           +bgas*(gm1(2)*fact+1.d0)/fact)
            if (c2.lt.c2min) then
              ier=2
              write(6,*)'j1,j2,c2=',j1,j2,c2
c             goto 1
            end if
          end if
        end do
      end do
c
      if (ier.ne.0) goto 1
      return
c
    1 write(55,100)loc,m,nd1a,nd1b,md1a,md1b,nd2a,nd2b,md2a,md2b
  100 format('location =',i2,/,1x,i2,8(1x,i4))
      do j2=md2a,md2b
        call cmppvs (m,nd1a,nd1b,md1a,md1b,nd2a,nd2b,j2,u,w)
        do j1=md1a,md1b
          write(55,200)j1,j2,(w(i,j1),i=1,9)
  200     format(2(1x,i4),9(1x,1pe22.15))
        end do
      end do
c
      ier=1
      return
      end
c
c***************
c
      subroutine prts (m,nd1a,nd1b,md1a,md1b,nd2a,nd2b,md2a,md2b,
     *                 u,w,iunit)
c
      implicit real*8 (a-h,o-z)
      dimension u(nd1a:nd1b,nd2a:nd2b,m),w(m,md1a:md1b)
c
      write(iunit,100)m,nd1a,nd1b,md1a,md1b,nd2a,nd2b,md2a,md2b
  100 format(1x,i2,8(1x,i4))
      do j2=md2a,md2b
        call cmppvs (m,nd1a,nd1b,md1a,md1b,nd2a,nd2b,j2,u,w)
        do j1=md1a,md1b
          write(iunit,200)j1,j2,(w(i,j1),i=1,9)
  200     format(2(1x,i4),9(1x,1pe22.15))
        end do
      end do
c
      return
      end
c
c++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
c
      subroutine setdat (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
     *                   md2a,md2b,n2a,n2b,tau,mdat,dat)
c
      implicit real*8 (a-h,o-z)
      dimension tau(nd1a:nd1b,nd2a:nd2b),dat(nd1a:nd1b,nd2a:nd2b,*)
c
c dat(nd1a:nd1b,nd2a:nd2b,.) is used to save extra grid data,
c if desired, which can then be plotted using plotStuff.
      if (mdat.gt.0) then
        do j2=n2a,n2b
        do j1=n1a,n1b
c          if (mask(j1,j2).eq.0) then
c            dat(j1,j2,1)=0.d0
c          else
            dat(j1,j2,1)=tau(j1,j2)
c          end if
        end do
        end do
      end if
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
      subroutine cmpsource (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,dt,
     *                      u,mask,tau,maxstep1,niwk,iwk,nrwk,rwk)
c
c Solid EOS includes a stiffening pressure and compaction potential.
c
c source contribution for drag, compaction, heat transfer and chemical
c reaction.  The drag coefficient is delta, the compaction viscosity is
c 1/rmuc and the heat transfer coefficient is htrans.  The ratio of
c specific heats (gas/solid) is cratio.  Integrate the source term from
c t=0 to t=dt.
c
      implicit real*8 (a-h,o-z)
      dimension u(nd1a:nd1b,nd2a:nd2b,m),mask(nd1a:nd1b,nd2a:nd2b),
     *          tau(nd1a:nd1b,nd2a:nd2b),iwk(niwk),rwk(nrwk)
c
c..timings
      common / timing / tflux(10),tslope,tsource
c
      call ovtime (time0)
c
c zero out truncation error estimate
      do j2=nd2a,nd2b
      do j1=nd1a,nd1b
        tau(j1,j2)=0.d0
      end do
      end do
c
c quarter step: drag only
      dt2=.5d0*dt
      call cmpsrc1 (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,dt2,
     *              u,mask)
c
c split up real workspace
      nd=n1b-n1a+1
      mpk=m-4
      ltk=1
      lhk=ltk+nd
      luk=lhk+nd
      lqk=luk+nd*m
      lwk=lqk+nd*4
      lupk=lwk+nd*m
      luk1=lupk+nd*3*mpk
      ltest=luk1+nd*m
      nreq=ltest+nd-1
      if (nreq.gt.nrwk) then
        write(6,*)'Error (cmpsource) : not enough real workspace'
        stop
      end if
c
c split up integer workspace
      ljk=1
      lnstep=ljk+nd
      nreq=lnstep+nd-1
      if (nreq.gt.niwk) then
        write(6,*)'Error (cmpsource) : not enough integer workspace'
        stop
      end if
c
c half step: compaction, heat transfer and chemical reaction
      ier1=0
      maxstep1=0
      do j2=n2a,n2b
        call cmpsrc2 (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,j2,dt,u,
     *                mask(nd1a,j2),tau(nd1a,j2),nd,iwk(ljk),rwk(ltk),
     *                rwk(lhk),rwk(luk),rwk(lqk),rwk(lwk),rwk(lupk),
     *                rwk(luk1),rwk(ltest),iwk(lnstep),mpk,maxstep,
     *                ier)
        ier1=max(ier,ier1)
        maxstep1=max(maxstep,maxstep1)
      end do
c
c      write(6,*)'  ** cmpsource, maxstep =',maxstep1
c
      if (ier1.ne.0) then
        write(6,*)'Warning (cmpsource) : ier1 =',ier1
      end if
c
c quarter step: drag only
      call cmpsrc1 (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,dt2,
     *              u,mask)
c
c set tau (for debugging)
c      do j2=nd2a,nd2b
c      do j1=nd1a,nd1b
c        tau(j1,j2)=1.d0
c      end do
c      end do
c
      call ovtime (time1)
      tsource=tsource+time1-time0
c
      return
      end
c
c++++++++++++++++++++++++++++++++++++++++++++++++++++++
c
      subroutine cmpsrc1 (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,dt,
     *                    u,mask)
c
c source step: drag only
c
      implicit real*8 (a-h,o-z)
      dimension u(nd1a:nd1b,nd2a:nd2b,m),mask(nd1a:nd1b,nd2a:nd2b)
      common / srcprm / delta,rmuc,htrans,cratio,abmin,abmax,isrc
      common / cutdat / abmin1
c
c     write(6,*)'cmpsrc1'
c
      do j2=n2a,n2b
      do j1=n1a,n1b
        if (mask(j1,j2).ne.0.and.u(j1,j2,9).gt.abmin1) then
c
c mass fractions
          fs=u(j1,j2,1)
          fg=u(j1,j2,5)
c
c current velocities
          us0=u(j1,j2,2)/fs
          vs0=u(j1,j2,3)/fs
          ug0=u(j1,j2,6)/fg
          vg0=u(j1,j2,7)/fg
          du0=ug0-us0
          dv0=vg0-vs0
c
c new velocities
          d=fg/(fs+fg)
          c=1.d0/fs+1.d0/fg
          b=d*(du0**2+dv0**2)
          a=us0*du0+vs0*dv0+b
          expn=dexp(-delta*c*dt)
          us=us0+du0*d*(1.d0-expn)
          vs=vs0+dv0*d*(1.d0-expn)
          ug=us+du0*expn
          vg=vs+dv0*expn
c
c update conserved variables
          u(j1,j2,2)=fs*us
          u(j1,j2,3)=fs*vs
          u(j1,j2,6)=fg*ug
          u(j1,j2,7)=fg*vg
          de=(a*(1.d0-expn)-.5d0*b*(1.d0-expn**2))/c
          u(j1,j2,4)=u(j1,j2,4)+de
          u(j1,j2,8)=u(j1,j2,8)-de
c
        end if
      end do
      end do
c
      return
      end
c
c++++++++++++++++++++++++++++++++++++++++++++++++++++++
c
      subroutine cmpsrc2 (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,j2,dt,u,
     *                    mask,tau,nd,jk,tk,hk,uk,qk,wk,upk,uk1,
     *                    test,nstep,mpk,maxstep,ier)
c
c source step: compaction, heat transfer, chemical reaction
c
      implicit real*8 (a-h,o-z)
      dimension u(nd1a:nd1b,nd2a:nd2b,m),mask(nd1a:nd1b),
     *          tau(nd1a:nd1b),jk(nd),tk(nd),hk(nd),uk(nd,m),
     *          qk(nd,4),wk(m,nd),upk(mpk,nd,3),uk1(nd,m),test(nd),
     *          nstep(nd),scale(5)
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0,bgas
      common / cmpsrc / tol,qmax,qmin,itmax
      common / cutdat / abmin1
      data tiny, c2min / 1.d-14, 1.d-12 /
c
c     write(6,*)'cmpsrc2'
c
      ier=0
      maxstep=0
      hmin=2.d0*dt/itmax
c
c accumulate unmasked points
      n=0
      do j1=n1a,n1b
        if (mask(j1).ne.0.and.u(j1,j2,9).gt.abmin1) then
          n=n+1
          jk(n)=j1
          tk(n)=0.d0
          hk(n)=dt
          do i=1,m
            uk(n,i)=u(j1,j2,i)
          end do
          do i=1,4
            qk(n,i)=uk(n,i)+uk(n,i+4)
          end do
          nstep(n)=0
        end if
      end do
c
c if there are no unmasked points, then return
      if (n.eq.0) return
c
c time steps for t=0 to t=dt
      do it=1,itmax
c
        call cmprate (m,nd,n,uk,wk,upk(1,1,1),mpk)
c
        do k=1,n
          do i=1,4                              ! update conserved solid and gas quantities
            upk(i,k,1)=hk(k)*upk(i,k,1)
            uk1(k,i  )=uk(k,i)+.5d0*upk(i,k,1)
            uk1(k,i+4)=qk(k,i)-uk1(k,i)
          end do
          do i=5,mpk                            ! update solid alpha + advected variables
            upk(i,k,1)=hk(k)*upk(i,k,1)
            uk1(k,i+4)=uk(k,i+4)+.5d0*upk(i,k,1)
          end do
        end do
c
        call cmprate (m,nd,n,uk1,wk,upk(1,1,2),mpk)
c
        do k=1,n
          do i=1,4                              ! update conserved solid and gas quantities
            upk(i,k,2)=hk(k)*upk(i,k,2)
            uk1(k,i  )=uk(k,i)+.75d0*upk(i,k,2)
            uk1(k,i+4)=qk(k,i)-uk1(k,i)
          end do
          do i=5,mpk                            ! update solid alpha + advected variables
            upk(i,k,2)=hk(k)*upk(i,k,2)
            uk1(k,i+4)=uk(k,i+4)+.75d0*upk(i,k,2)
          end do
        end do
c
        call cmprate (m,nd,n,uk1,wk,upk(1,1,3),mpk)
c
        do k=1,n
          j1=jk(k)
          test(k)=0.d0
          fact=1.d0+bgas*wk(5,k)
          c2=wk(8,k)*(gam(2)/wk(5,k)
     *         +bgas*(gm1(2)*fact+1.d0)/fact)
          c=dsqrt(max(c2,c2min))
          cb2=gam(1)*(wk(4,k)+ps0)/wk(1,k)
          cb=dsqrt(max(cb2,c2min))
          scale(1)=qk(k,1)
          scale(2)=qk(k,1)*max(dabs(wk(2,k))+cb,dabs(wk(6,k))+c)
          scale(3)=qk(k,1)*max(dabs(wk(3,k))+cb,dabs(wk(7,k))+c)
          scale(4)=qk(k,4)
          scale(5)=1.d0
          do i=1,5                              ! update conserved solid and solid alpha
            upk(i,k,3)=hk(k)*upk(i,k,3)
            trunc=dabs(2*upk(i,k,1)-6*upk(i,k,2)+4*upk(i,k,3))/scale(i)
            test(k)=max(trunc,test(k))
          end do
          test(k)=test(k)/(9*hk(k))
          tau(j1)=max(test(k),tau(j1))
          do i=6,mpk                            ! update advected variables (if necessary)
            upk(i,k,3)=hk(k)*upk(i,k,3)
          end do
        end do
c
        n1=0
        if (it.lt.itmax) then
          do k=1,n
            if (test(k).lt.tol.or.hk(k).le.hmin) then
c
c step succeeded: advance uk
              tk(k)=tk(k)+hk(k)
              nstep(k)=nstep(k)+1
              do i=1,4                          ! advance conserved solid and gas quantities
                uk(k,i)=uk(k,i)
     *                  +(2*upk(i,k,1)+3*upk(i,k,2)+4*upk(i,k,3))/9.d0
                uk(k,i+4)=qk(k,i)-uk(k,i)
              end do
              do i=5,mpk                        ! advance solid alpha + advected variables
                uk(k,i+4)=uk(k,i+4)
     *                  +(2*upk(i,k,1)+3*upk(i,k,2)+4*upk(i,k,3))/9.d0
              end do
c
c check whether or not time has reached dt
              if (tk(k).lt.dt-tiny) then
                n1=n1+1
                test(n1)=test(k)
                jk(n1)=jk(k)
                tk(n1)=tk(k)
                hk(n1)=hk(k)
                do i=1,m
                  uk(n1,i)=uk(k,i)
                end do
                do i=1,4
                  qk(n1,i)=qk(k,i)
                end do
                nstep(n1)=nstep(k)
              else
                j1=jk(k)
                do i=1,m
                  u(j1,j2,i)=uk(k,i)
                end do
                maxstep=max(nstep(k),maxstep)
              end if
c
            else
c
c step failed
              n1=n1+1
              test(n1)=test(k)
              jk(n1)=jk(k)
              tk(n1)=tk(k)
              hk(n1)=hk(k)
              do i=1,m
                uk(n1,i)=uk(k,i)
              end do
              do i=1,4
                qk(n1,i)=qk(k,i)
              end do
              nstep(n1)=nstep(k)
            end if
          end do
c
          n=n1
          if (n.eq.0) then
            return
          else
            if (it.lt.itmax-1) then
              do k=1,n
                q=min(qmax,max(qmin,dsqrt(tol/(2*max(test(k),tiny)))))
                hk(k)=min(dt-tk(k)+tiny,max(q*hk(k),hmin))
              end do
            else
              do k=1,n
                hk(k)=dt-tk(k)
              end do
            end if
          end if
c
        else
c
c no more time steps allowed, just finish off
          do k=1,n
            j1=jk(k)
            do i=1,4                            ! advance conserved solid and gas quantities
              u(j1,j2,i)=uk(k,i)
     *                   +(2*upk(i,k,1)+3*upk(i,k,2)+4*upk(i,k,3))/9.d0
              u(j1,j2,i+4)=qk(k,i)-u(j1,j2,i)
            end do
            do i=5,mpk                          ! advance solid alpha + advected variables
              u(j1,j2,i+4)=uk(k,i+4)
     *                   +(2*upk(i,k,1)+3*upk(i,k,2)+4*upk(i,k,3))/9.d0
            end do
            maxstep=max(nstep(k)+1,maxstep)
          end do
c
        end if
c
      end do
c
c one or more points may have integrated to dt without tolerance being met
      ier=1
c
      return
      end
c
c++++++++++++++++++++++++++++++++++++++++++++++++++++++
c
      subroutine cmprate (m,nd,n,uk,wk,upk,mpk)
c
      implicit real*8 (a-h,o-z)
      dimension uk(nd,m),wk(m,nd),upk(mpk,nd)
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0,bgas
      common / srcprm / delta,rmuc,htrans,cratio,abmin,abmax,isrc
      common / cmprxn / sigma,pgi,anu
      common / cmpign / sigmai,pfref,ab0,anui,phieps,phimin
c
! *wdh* test:      if( m.ne.9) then
      if( .false. .and. m.ne.9) then
        write(6,*)'Error (cmprate) : burn-only rate assumed, m=9'
        stop
      end if
c
c get primitive state
      call cmppvs (m,1,nd,1,n,1,1,1,uk,wk)
c
c get rate
      do k=1,n
c
c compaction
        beta=uk(k,1)*compac(wk(9,k),1)
        f=rmuc*(abmax-wk(9,k))*(wk(9,k)-abmin)*(wk(4,k)-wk(8,k)-beta)
c
c heat transfer
        h=htrans*(wk(8,k)/(gm1(2)*wk(5,k)*(1.d0+bgas*wk(5,k)))
     *    -cratio*(wk(4,k)+ps0)/(gm1(1)*wk(1,k)))
c
c chemical reaction, "burn" reaction only
        if (wk(8,k).gt.pgi) then
          c=-sigma*(wk(9,k)-abmin)*wk(1,k)*(wk(8,k)-pgi)**anu
        else
          c=0.d0
        end if
c
c source
        upk(1,k)=c
        upk(2,k)=.5d0*c*(wk(2,k)+wk(6,k))
        upk(3,k)=.5d0*c*(wk(3,k)+wk(7,k))
        upk(4,k)=(uk(k,4)/uk(k,1)+beta/wk(1,k)
     *            +.5d0*(wk(2,k)*(wk(6,k)-wk(2,k))
     *                  +wk(3,k)*(wk(7,k)-wk(3,k))))*c
     *           +h-wk(8,k)*f
        upk(5,k)=f+c/wk(1,k)
c
      end do
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
      subroutine cmpvisc (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
     *                    md2a,md2b,n2a,n2b,ds,rx,u,up,mask,div,
     *                    av,vismax,icart,ad,n1bm,n2bm)
c
      implicit real*8 (a-h,o-z)
      dimension rx(nd1a:n1bm,nd2a:n2bm,2,2),u(nd1a:nd1b,nd2a:nd2b,m),
     *          up(nd1a:nd1b,nd2a:nd2b,m),mask(nd1a:nd1b,nd2a:nd2b),
     *          ds(2),div(md1a:md1b,2),ad(m),fx(9)
c
      vismax=0.d0
      adMax=0.d0
      do i=1,m
        adMax=max(adMax,ad(i))
      end do
c
c solid phase
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
c gas phase
      do j2=n2a-1,n2b+1
        j2m1=j2-1
        if (icart.eq.0) then
          do j1=n1a-1,n1b+1
            vxm=(rx(j1,j2,1,1)*u(j1-1,j2,6)
     *          +rx(j1,j2,1,2)*u(j1-1,j2,7))/u(j1-1,j2,5)
            vxp=(rx(j1,j2,1,1)*u(j1+1,j2,6)
     *          +rx(j1,j2,1,2)*u(j1+1,j2,7))/u(j1+1,j2,5)
            vym=(rx(j1,j2,2,1)*u(j1,j2-1,6)
     *          +rx(j1,j2,2,2)*u(j1,j2-1,7))/u(j1,j2-1,5)
            vyp=(rx(j1,j2,2,1)*u(j1,j2+1,6)
     *          +rx(j1,j2,2,2)*u(j1,j2+1,7))/u(j1,j2+1,5)
            div(j1,2)= (vxp-vxm)/(2.d0*ds(1))+(vyp-vym)/(2.d0*ds(2))
            if (mask(j1,j2).ne.0) then
              vismax=max(-div(j1,2),vismax)
            end if
          end do
        else
          a11=rx(nd1a,nd2a,1,1)/(2.d0*ds(1))
          a22=rx(nd1a,nd2a,2,2)/(2.d0*ds(2))
          do j1=n1a-1,n1b+1
            vxm=u(j1-1,j2,6)/u(j1-1,j2,5)
            vxp=u(j1+1,j2,6)/u(j1+1,j2,5)
            vym=u(j1,j2-1,7)/u(j1,j2-1,5)
            vyp=u(j1,j2+1,7)/u(j1,j2+1,5)
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
              do i=5,8
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
                do i=5,8
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
c volume fraction + advected scalars
      do j2=n2a,n2b+1
        j2m1=j2-1
        do j1=n1a,n1b
          if (mask(j1,j2).ne.0.and.mask(j1,j2m1).ne.0) then
            do i=9,m
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
              do i=9,m
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
      subroutine cmphydro (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
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
c
      dimension ic(7),wsavel(9),wsaver(9)
c
      dimension fl(9),fr(9)
      common / axidat / iaxi,j1axi(2),j2axi(2)
c
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0,bgas
c
c..timings
      common / timing / tflux(10),tslope,tsource
c
c     data ic / 9,1,2,4,5,6,8 /
      data ic / 9,1,3,4,5,7,8 /
c
c..ratios
      dtds1=dr/ds(1)
      dtds2=dr/ds(2)
c
      if (m.lt.0) then
        write(6,*)((rx(n1a,n2a,i,j),j=1,2),i=1,2),det(n1a,n2a)
        pause
      end if
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
        call cmppvs (m,nd1a,nd1b,md1a,md1b,nd2a,nd2b,j2,up,
     *               w(1,md1a,k2))
      end do
c
      if (m.lt.0) then
        write(6,*)'cmpdu(after cmppvs)'
        write(6,'(9(1x,i1,1x,f15.8,/))')(i,w(i,0,2),i=1,9)
        pause
      end if
c
c..set grid metrics and velocity (if necessary)
      j2=n2a-1
      call mpmetrics (nd1a,nd1b,md1a,md1b,nd2a,nd2b,j2,rx,gv,det,
     *                rx2,gv2,det2,a0(1,md1a,1),a1(1,1,md1a,1),
     *                aj(md1a,1),move,icart,n1bm,n2bm)
c
      call ovtime (time0)
c
c..slope correction (bottom row of cells)
      call cmpslope (m,nd1a,nd1b,md1a,md1b,n1a,n1b,dr,ds,
     *               a0(1,md1a,1),a1(1,1,md1a,1),aj(md1a,1),
     *               mask(nd1a,j2),w,w1(1,md1a,1,1),iorder)
c
c..axisymmetric stuff (not implemented)
c      if (iorder.eq.2.and.m.lt.0) then
c        if (iaxi.gt.0) then
c          write(6,*)'Error (cmpdu0) : axi.gt.0'
c          if (iaxi.gt.0) stop
c          do j1=n1a-1,n1b+1
c            if (mask(j1,j2).ne.0) then
c              vaxi(j1,2)= (u(j1,j2,3)+h(3,j1)/aj(j1,1))
c     *                   /(u(j1,j2,1)+h(1,j1)/aj(j1,1))
c            end if
c          end do
c        end if
c      else
c        if (iaxi.gt.0) then
c          write(6,*)'Error (cmpdu0) : axi.gt.0'
c          if (iaxi.gt.0) stop
c          do j1=n1a-1,n1b+1
c            if (mask(j1,j2).ne.0) then
c              vaxi(j1,2)=u(j1,j2,3)/u(j1,j2,1)
c            end if
c          end do
c        end if
c      end if
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
        call cmppvs (m,nd1a,nd1b,md1a,md1b,nd2a,nd2b,j2+1,up,
     *               w(1,md1a,3))
c
c..set grid metrics and velocity (if necessary)
        call mpmetrics (nd1a,nd1b,md1a,md1b,nd2a,nd2b,j2,rx,gv,det,
     *                  rx2,gv2,det2,a0(1,md1a,2),a1(1,1,md1a,2),
     *                  aj(md1a,2),move,icart,n1bm,n2bm)
c
        call ovtime (time0)
c
c..slope correction (top row of cells)
        call cmpslope (m,nd1a,nd1b,md1a,md1b,n1a,n1b,dr,ds,
     *                 a0(1,md1a,2),a1(1,1,md1a,2),aj(md1a,2),
     *                 mask(nd1a,j2),w,w1(1,md1a,1,2),iorder)
c
c..axisymmetric stuff (not implemented)
c        if (iorder.eq.2.and.m.lt.0) then
c          if (iaxi.gt.0) then
c            write(6,*)'Error (cmpdu0) : axi.gt.0'
c            if (iaxi.gt.0) stop
c            do j1=n1a-1,n1b+1
c              if (mask(j1,j2).ne.0) then
c                vaxi(j1,3)= (u(j1,j2,3)+h(3,j1)/aj(j1,2))
c     *                     /(u(j1,j2,1)+h(1,j1)/aj(j1,2))
c              end if
c            end do
c          end if
c        else
c          if (iaxi.gt.0) then
c            write(6,*)'Error (cmpdu0) : axi.gt.0'
c            if (iaxi.gt.0) stop
c            do j1=n1a-1,n1b+1
c              if (mask(j1,j2).ne.0) then
c                vaxi(j1,3)=u(j1,j2,3)/u(j1,j2,1)
c              end if
c            end do
c          end if
c        end if
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

c            if (j1.eq.n1a.and.m.lt.0) then
c            write(55,444)j2,aj0,a20,a21,a22,
c     *                   (w1(ic(k),j1,4,1),w1(ic(k),j1,3,2),k=1,7)
c            end if

c            if (m.lt.0) then
c              do i=1,m
c                wsavel(i)=w1(i,j1,4,1)
c                wsaver(i)=w1(i,j1,3,2)
c              end do
c            end if

            call cmpflux (m,aj0,a20,a21,a22,w1(1,j1,4,1),
     *                    w1(1,j1,3,2),fl,fr,almax(2),method)

c            if (m.lt.0.and.j1.eq.n1a.and.dr.gt.1.d-3) then
cc              write(66,432)j1,r
cc  432         format(1x,i3,1x,f15.8)
c              call chkflux (m,aj0,a20,a21,a22,wsavel,wsaver,
c     *                      w1(1,j1,4,1),w1(1,j1,3,2),fl,fr,
c     *                      almax(2),method,errfl,errfr,
c     *                      errwl,errwr)
c              if (max(errfl,errfr,errwl,errwr).gt.1.d-12) then
c                write(66,437)j1,r,errfl,errfr,errwl,errwr
c  437           format(1x,i3,1x,f15.8,4(1x,1pe9.2))
c              end if
c            end if


c            if (j1.eq.n1a.and.m.lt.0) then
c            write(55,445)j2,dtds2,(fl(ic(k)),fr(ic(k)),
c     *                   w1(ic(k),j1,4,1),w1(ic(k),j1,3,2),k=1,7)
c            end if

            do i=1,m
              up(j1,j2  ,i)=up(j1,j2  ,i)+dtds2*fr(i)/aj(j1,2)
              up(j1,j2m1,i)=up(j1,j2m1,i)-dtds2*fl(i)/aj(j1,1)
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
        call ovtime (time2)
        tflux(1)=tflux(1)+time2-time1
c
c..add in final second-order contributions to up
        if (j2.gt.n2a.and.iorder.eq.2) then
          do j1=n1a,n1b
            da1=.5d0*dtds1*(w1(9,j1,2,1)-w1(9,j1,1,1))
            da2=.5d0*dtds2*(w1(9,j1,4,1)-w1(9,j1,3,1))
            aint= a1(1,1,j1,1)*(w1(8,j1,1,1)+w1(8,j1,2,1))*da1
     *           +a1(2,1,j1,1)*(w1(8,j1,3,1)+w1(8,j1,4,1))*da2
            up(j1,j2m1,2)=up(j1,j2m1,2)+aint
            up(j1,j2m1,6)=up(j1,j2m1,6)-aint
            aint= a1(1,2,j1,1)*(w1(8,j1,1,1)+w1(8,j1,2,1))*da1
     *           +a1(2,2,j1,1)*(w1(8,j1,3,1)+w1(8,j1,4,1))*da2
            up(j1,j2m1,3)=up(j1,j2m1,3)+aint
            up(j1,j2m1,7)=up(j1,j2m1,7)-aint
            aint=((a1(1,1,j1,1)*w1(2,j1,1,1)
     *            +a1(1,2,j1,1)*w1(3,j1,1,1))*w1(8,j1,1,1)
     *           +(a1(1,1,j1,1)*w1(2,j1,2,1)
     *            +a1(1,2,j1,1)*w1(3,j1,2,1))*w1(8,j1,2,1))*da1
     *          +((a1(2,1,j1,1)*w1(2,j1,3,1)
     *            +a1(2,2,j1,1)*w1(3,j1,3,1))*w1(8,j1,3,1)
     *           +(a1(2,1,j1,1)*w1(2,j1,4,1)
     *            +a1(2,2,j1,1)*w1(3,j1,4,1))*w1(8,j1,4,1))*da2
            up(j1,j2m1,4)=up(j1,j2m1,4)+aint
            up(j1,j2m1,8)=up(j1,j2m1,8)-aint
            vel1= a1(1,1,j1,1)*(w1(2,j1,1,1)+w1(2,j1,2,1))
     *           +a1(1,2,j1,1)*(w1(3,j1,1,1)+w1(3,j1,2,1))
            vel2= a1(2,1,j1,1)*(w1(2,j1,3,1)+w1(2,j1,4,1))
     *           +a1(2,2,j1,1)*(w1(3,j1,3,1)+w1(3,j1,4,1))
            aint=vel1*da1+vel2*da2
            up(j1,j2m1,9)=up(j1,j2m1,9)-aint
            do i=10,m
              dphi1=.5d0*dtds1*(w1(i,j1,2,1)-w1(i,j1,1,1))
              dphi2=.5d0*dtds2*(w1(i,j1,4,1)-w1(i,j1,3,1))
              aint=vel1*dphi1+vel2*dphi2
              up(j1,j2m1,i)=up(j1,j2m1,i)-aint
            end do
          end do
        end if
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
          if (move.ne.0.and.icart.eq.0.and.m.lt.0) then
            do j1=n1a,n1b
              da0(j1,1)=da0(j1,2)
            end do
          end if
c
c..reset vaxi (for axisymmetric problems)
c          if (iaxi.gt.0) then
c            do j1=n1a-1,n1b+1
c              vaxi(j1,1)=vaxi(j1,2)
c              vaxi(j1,2)=vaxi(j1,3)
c            end do
c          end if
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

c              if (j2.eq.n2a.and.m.lt.0) then
c                write(55,444)j1,aj0,a10,a11,a12,
c     *                     (w1(ic(k),j1,2,1),w1(ic(k),j1p1,1,1),k=1,7)
c  444           format(1x,i2,4(1x,f7.2),7(/,2(1x,1pe15.8)))
c              end if

c              if (m.lt.0) then
c                do i=1,m
c                  wsavel(i)=w1(i,j1,2,1)
c                  wsaver(i)=w1(i,j1p1,1,1)
c                end do
c              end if

              call cmpflux (m,aj0,a10,a11,a12,w1(1,j1,2,1),
     *                      w1(1,j1p1,1,1),fl,fr,almax(1),method)


c              if (m.lt.0.and.j2.eq.n2a.and.dr.gt.1.d-3) then
cc                write(66,432)j1,r
cc  432           format(1x,i3,1x,f15.8)
c                call chkflux (m,aj0,a10,a11,a12,wsavel,wsaver,
c     *                        w1(1,j1,2,1),w1(1,j1p1,1,1),fl,fr,
c     *                        almax(1),method,errfl,errfr,
c     *                        errwl,errwr)
c                if (max(errfl,errfr,errwl,errwr).gt.1.d-12) then
c                  write(66,436)j1,r,errfl,errfr,errwl,errwr
c  436             format(1x,i3,1x,f15.8,4(1x,1pe9.2))
c                end if
c              end if

c              if (j2.eq.n2a.and.m.lt.0) then
c                write(55,445)j1,dtds1,(fl(ic(k)),fr(ic(k)),
c     *                     w1(ic(k),j1,2,1),w1(ic(k),j1p1,1,1),k=1,7)
c  445           format(1x,i2,1x,1pe10.3,7(/,4(1x,1pe15.8)))
c              end if


              do i=1,m
                up(j1p1,j2,i)=up(j1p1,j2,i)+dtds1*fr(i)/aj(j1p1,1)
                up(j1  ,j2,i)=up(j1  ,j2,i)-dtds1*fl(i)/aj(j1  ,1)
              end do
            end if
          end do
c
          call ovtime (time1)
          tflux(1)=tflux(1)+time1-time0
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
            call cmpfree (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
     *                    dr,ds,j2,rx,det,rx2,det2,up,mask(nd1a,j2),
     *                    w1(1,md1a,5,1),move)
          end if
c
        end if
c
c..bottom of main loop over lines
      end do
c
      return
      end
c
c+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
c
      subroutine cmppvs (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,j2,u,w)
c
c convert conservative u to primitive variables w
c
      implicit real*8 (a-h,o-z)
      dimension u(nd1a:nd1b,nd2a:nd2b,m),w(m,n1a:n1b)
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0,bgas
c
      do j1=n1a,n1b
        w(9,j1)=u(j1,j2,9)
        w(1,j1)=u(j1,j2,1)/w(9,j1)
        w(2,j1)=u(j1,j2,2)/u(j1,j2,1)
        w(3,j1)=u(j1,j2,3)/u(j1,j2,1)
        w(4,j1)=u(j1,j2,4)/u(j1,j2,1)
        w(4,j1)=gm1(1)*w(1,j1)*(w(4,j1)-.5d0*(w(2,j1)**2+w(3,j1)**2)
     *                         -compac(w(9,j1),0))-gam(1)*ps0
        w(5,j1)=u(j1,j2,5)/(1.d0-w(9,j1))
        w(6,j1)=u(j1,j2,6)/u(j1,j2,5)
        w(7,j1)=u(j1,j2,7)/u(j1,j2,5)
        w(8,j1)=u(j1,j2,8)/u(j1,j2,5)
        w(8,j1)=gm1(2)*w(5,j1)*(w(8,j1)-.5d0*(w(6,j1)**2+w(7,j1)**2))
     *           *(1.d0+bgas*w(5,j1))
        do i=10,m
          w(i,j1)=u(j1,j2,i)
        end do
      end do
c
      return
      end
c
c++++++++++++++++++++++++++++++++++++++++++++++++++
c
      double precision function compac0 (as,ideriv)
c
c compute compaction potential (if ideriv=0) or its first
c derivative (if ideriv=1) for the configuration pressure
c
      implicit real*8 (a-h,o-z)
c
      compac0=0.d0
c
      return
      end
c
c+++++++++++++++++++++++++++++++++++++++++++++++++
c
      double precision function compac (as,ideriv)
c
c compute compaction potential (if ideriv=0) or its first
c derivative (if ideriv=1) for the configuration pressure
c
      implicit real*8 (a-h,o-z)
      common / comdat / cfact1,cfact2,heat
c
c      as0=.73d0
c      ps0=7.6d0
c      pg0=0.252d0
c      rs0=1900.d0
c      cfact1=(pg0-ps0)*(2.d0-as0)**2/(as0*rs0*dlog(1.d0-as0))
c      cfact2=(2.d0-as0)/(1.d0-as0)**((1.d0-as0)/(2.d0-as0))
c
      c1=1.d0-as
      c2=1.d0/(2.d0-as)
c
      if (ideriv.eq.0) then
        compac= cfact1*dlog(cfact2*c2*c1**(c1*c2))+heat
      else
        compac=-cfact1*dlog(c1)*c2**2
      end if
c
      return
      end
c
c++++++++++++++++++++++++++++
c
      subroutine mpmetrics (nd1a,nd1b,md1a,md1b,nd2a,nd2b,j2,rx,gv,det,
     *                      rx2,gv2,det2,a0,a1,aj,move,icart,n1bm,n2bm)
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
        end if
      end if
c
      return
      end
c
c++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
c
      subroutine cmpslope (m,nd1a,nd1b,md1a,md1b,n1a,n1b,dr,ds,
     *                     a0,a1,aj,mask,w,w1,iorder)
c
c slope correction
c
      implicit real*8 (a-h,o-z)
      dimension ds(2),a0(2,md1a:md1b),a1(2,2,md1a:md1b),aj(md1a:md1b),
     *          mask(nd1a:nd1b),w(m,md1a:md1b,3),w1(m,md1a:md1b,5)
      dimension dw(9),b(9,2),al(9),fact(5)
      logical isolid,igas
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0,bgas
      common / cutdat / abmin1

c      common / junk / ijunk(9,-5:300)

      data amin,amax,c2min,abtiny / 1.d-2, 0.99d0, 1.d-6, 1.d-8 /
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
      if (iorder.le.1) return
c
c..compute slope contribution and add it to w1(i,j1,1:2,1:2)
      do j1=n1a-1,n1b+1

c       do i=1,9
c         ijunk(i,j1)=0
c       end do

        if (mask(j1).ne.0) then
c
          isolid=.true.
          if (w(9,j1,2).lt.abmin1) isolid=.false.
          igas=.true.
          if (w(9,j1,2).gt.amax) igas=.false.
c
          rb=w(1,j1,2)
          gpb=gam(1)*(w(4,j1,2)+ps0)
          cb2=gpb/rb
          if (cb2.lt.c2min) then
            write(6,*)'Error (cmpslope) : cb2<c2min'
            write(6,*)j1,(w(i,j1,2),i=1,9)
            stop
          end if
          cb=dsqrt(cb2)
c
          r=w(5,j1,2)
          bfact=1.d0+bgas*r
          gp=w(8,j1,2)*(gam(2)+bgas*r*(gm1(2)*bfact+1.d0)/bfact)
          c2=gp/r
          if (c2.lt.c2min) then
            write(6,*)'Error (cmpslope) : c2<c2min'
            stop
          end if
          c=dsqrt(c2)
c
          dp=w(8,j1,2)-w(4,j1,2)
          dpda=dp/max(w(9,j1,2),amin)
c
c..s1 direction
          rad=dsqrt(a1(1,1,j1)**2+a1(1,2,j1)**2)
          an1=a1(1,1,j1)/rad
          an2=a1(1,2,j1)/rad
c
          vnb=an1*w(2,j1,2)+an2*w(3,j1,2)
          vn =an1*w(6,j1,2)+an2*w(7,j1,2)
          dv=vn-vnb
          dvda=dv/(1.d0-min(w(9,j1,2),amax))
          dvp=dv+c
          dvm=dv-c
          sig=dvp*dvm
c
          t0=1.d0/sig
          t1=.5d0/gpb
          t2=rb*cb
          t3=1.d0/cb2
          t4=.5d0/gp
          t5=gp*dvda/dvm
          t6=r*c
          t7=1.d0/c2
          t8=gp*dvda/dvp
c
c..differences of Riemann variables
          do ks=1,2
            jkp=j1+ks-1
            jk=jkp-1
            do i=1,m
              dw(i)=w(i,jkp,2)-w(i,jk,2)
            end do
c solid C-
            b(1,ks)=t1*(-t2*(an1*dw(2)+an2*dw(3))+dw(4)-dpda*dw(9))
c solid P
            b(2,ks)=t3*(cb2*dw(1)-dw(4)+dpda*dw(9))
            b(3,ks)=-an2*dw(2)+an1*dw(3)
c solid C+
            b(4,ks)=t1*( t2*(an1*dw(2)+an2*dw(3))+dw(4)-dpda*dw(9))
c gas C-
            b(5,ks)=t4*(-t6*(an1*dw(6)+an2*dw(7))+dw(8)-t5*dw(9))
c gas P
            b(6,ks)=t7*(c2*dw(5)-dw(8))
            b(7,ks)=-an2*dw(6)+an1*dw(7)
c gas C+
            b(8,ks)=t4*( t6*(an1*dw(6)+an2*dw(7))+dw(8)-t8*dw(9))
c solid P
            b(9,ks)=-t0*dw(9)
          end do
c
c eigenvalues * (dt/ds1) * rad / 2
          tmp=.5d0*rad*dr/ds(1)
          al(1)=tmp*(vnb-cb)
          al(2)=tmp*(vnb   )
          al(3)=al(2)
          al(4)=tmp*(vnb+cb)
          al(5)=tmp*(vn -c )
          al(6)=tmp*(vn    )
          al(7)=al(6)
          al(8)=tmp*(vn +c )
          al(9)=al(2)
c
          if (isolid) then
c
c C- contribution
            i=1
            if (b(i,1)*b(i,2).gt.0.d0.and.m.gt.0) then

c             ijunk(i,j1)=1

              beta=dsign(1.d0,b(i,1))*dmin1(dabs(b(i,1)),dabs(b(i,2)))
              fact(1)=(min(al(i),0.d0)+.5d0)*beta
              fact(2)=(max(al(i),0.d0)-.5d0)*beta
              fact(3)=al(i)*beta
              fact(4)=fact(3)
              fact(5)=fact(3)
              do k=1,5
                w1(1,j1,k)=w1(1,j1,k)-fact(k)*rb
                w1(2,j1,k)=w1(2,j1,k)+fact(k)*cb*an1
                w1(3,j1,k)=w1(3,j1,k)+fact(k)*cb*an2
                w1(4,j1,k)=w1(4,j1,k)-fact(k)*gpb
              end do
            end if
c
c P contribution
            i=2
            if (b(i,1)*b(i,2).gt.0.d0.and.m.gt.0) then

c             ijunk(i,j1)=1

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
c
c P contribution
            i=3
            if (b(i,1)*b(i,2).gt.0.d0.and.m.gt.0) then

c             ijunk(i,j1)=1

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
c
c C+ contribution
            i=4
            if (b(i,1)*b(i,2).gt.0.d0.and.m.gt.0) then

c             ijunk(i,j1)=1

              beta=dsign(1.d0,b(i,1))*dmin1(dabs(b(i,1)),dabs(b(i,2)))
              fact(1)=(min(al(i),0.d0)+.5d0)*beta
              fact(2)=(max(al(i),0.d0)-.5d0)*beta
              fact(3)=al(i)*beta
              fact(4)=fact(3)
              fact(5)=fact(3)
              do k=1,5
                w1(1,j1,k)=w1(1,j1,k)-fact(k)*rb
                w1(2,j1,k)=w1(2,j1,k)-fact(k)*cb*an1
                w1(3,j1,k)=w1(3,j1,k)-fact(k)*cb*an2
                w1(4,j1,k)=w1(4,j1,k)-fact(k)*gpb
              end do
            end if
c
          end if
c
          if (igas) then
c
c C- contribution
            i=5
            if (b(i,1)*b(i,2).gt.0.d0.and.m.gt.0) then

c             ijunk(i,j1)=1

              beta=dsign(1.d0,b(i,1))*dmin1(dabs(b(i,1)),dabs(b(i,2)))
              fact(1)=(min(al(i),0.d0)+.5d0)*beta
              fact(2)=(max(al(i),0.d0)-.5d0)*beta
              fact(3)=al(i)*beta
              fact(4)=fact(3)
              fact(5)=fact(3)
              do k=1,5
                w1(5,j1,k)=w1(5,j1,k)-fact(k)*r
                w1(6,j1,k)=w1(6,j1,k)+fact(k)*c*an1
                w1(7,j1,k)=w1(7,j1,k)+fact(k)*c*an2
                w1(8,j1,k)=w1(8,j1,k)-fact(k)*gp
              end do
            end if
c
c P contribution
            i=6
            if (b(i,1)*b(i,2).gt.0.d0.and.m.gt.0) then

c             ijunk(i,j1)=1

              beta=dsign(1.d0,b(i,1))*dmin1(dabs(b(i,1)),dabs(b(i,2)))
              fact(1)=(min(al(i),0.d0)+.5d0)*beta
              fact(2)=(max(al(i),0.d0)-.5d0)*beta
              fact(3)=al(i)*beta
              fact(4)=fact(3)
              fact(5)=fact(3)
              do k=1,5
                w1(5,j1,k)=w1(5,j1,k)-fact(k)
              end do
            end if
c
c P contribution
            i=7
            if (b(i,1)*b(i,2).gt.0.d0.and.m.gt.0) then

c             ijunk(i,j1)=1

              beta=dsign(1.d0,b(i,1))*dmin1(dabs(b(i,1)),dabs(b(i,2)))
              fact(1)=(min(al(i),0.d0)+.5d0)*beta
              fact(2)=(max(al(i),0.d0)-.5d0)*beta
              fact(3)=al(i)*beta
              fact(4)=fact(3)
              fact(5)=fact(3)
              do k=1,5
                w1(6,j1,k)=w1(6,j1,k)+fact(k)*an2
                w1(7,j1,k)=w1(7,j1,k)-fact(k)*an1
              end do
            end if
c
c C+ contribution
            i=8
            if (b(i,1)*b(i,2).gt.0.d0.and.m.gt.0) then

c             ijunk(i,j1)=1

              beta=dsign(1.d0,b(i,1))*dmin1(dabs(b(i,1)),dabs(b(i,2)))
              fact(1)=(min(al(i),0.d0)+.5d0)*beta
              fact(2)=(max(al(i),0.d0)-.5d0)*beta
              fact(3)=al(i)*beta
              fact(4)=fact(3)
              fact(5)=fact(3)
              do k=1,5
                w1(5,j1,k)=w1(5,j1,k)-fact(k)*r
                w1(6,j1,k)=w1(6,j1,k)-fact(k)*c*an1
                w1(7,j1,k)=w1(7,j1,k)-fact(k)*c*an2
                w1(8,j1,k)=w1(8,j1,k)-fact(k)*gp
              end do
            end if
c
          end if
c
c solid P contribution
          i=9
          if (b(i,1)*b(i,2).gt.0.d0.and.m.gt.0) then

c             ijunk(i,j1)=1

            beta=dsign(1.d0,b(i,1))*dmin1(dabs(b(i,1)),dabs(b(i,2)))
            fact(1)=(min(al(i),0.d0)+.5d0)*beta
            fact(2)=(max(al(i),0.d0)-.5d0)*beta
            fact(3)=al(i)*beta
            fact(4)=fact(3)
            fact(5)=fact(3)
            if (isolid) then
              do k=1,5
                w1(4,j1,k)=w1(4,j1,k)+fact(k)*sig*dpda
              end do
            end if
            if (igas) then
              do k=1,5
                w1(5,j1,k)=w1(5,j1,k)+fact(k)*dvda*r*dv
                w1(6,j1,k)=w1(6,j1,k)-fact(k)*dvda*c2*an1
                w1(7,j1,k)=w1(7,j1,k)-fact(k)*dvda*c2*an2
                w1(8,j1,k)=w1(8,j1,k)+fact(k)*dvda*gp*dv
              end do
            end if
            do k=1,5
              w1(9,j1,k)=w1(9,j1,k)+fact(k)*sig
            end do
          end if
c
c solid P contribution (advected scalars)
          if (isolid) then
            m0=100
            if (m.gt.9) m0=10
            do i=m0,m
              b1=w(i,j1,2)-w(i,j1-1,2)
              b2=w(i,j1+1,2)-w(i,j1,2)
              if (b1*b2.gt.0.d0.and.m.gt.0) then
                beta=dsign(1.d0,b1)*dmin1(dabs(b1),dabs(b2))
                fact(1)=(min(al(2),0.d0)+.5d0)*beta
                fact(2)=(max(al(2),0.d0)-.5d0)*beta
                fact(3)=al(2)*beta
                fact(4)=fact(3)
                fact(5)=fact(3)
                do k=1,5
                  w1(i,j1,k)=w1(i,j1,k)+fact(k)
                end do
              end if
            end do
          end if
c
c
c..s2 direction
          rad=dsqrt(a1(2,1,j1)**2+a1(2,2,j1)**2)
          an1=a1(2,1,j1)/rad
          an2=a1(2,2,j1)/rad
c
          vnb=an1*w(2,j1,2)+an2*w(3,j1,2)
          vn =an1*w(6,j1,2)+an2*w(7,j1,2)
          dv=vn-vnb
          dvda=dv/(1.d0-min(w(9,j1,2),amax))
          dvp=dv+c
          dvm=dv-c
          sig=dvp*dvm
c
          t0=1.d0/sig
          t1=.5d0/gpb
          t2=rb*cb
          t3=1.d0/cb2
          t4=.5d0/gp
          t5=gp*dvda/dvm
          t6=r*c
          t7=1.d0/c2
          t8=gp*dvda/dvp
c
c..differences of Riemann variables
          do ks=1,2
            do i=1,m
              dw(i)=w(i,j1,ks+1)-w(i,j1,ks)
            end do
c solid C-
            b(1,ks)=t1*(-t2*(an1*dw(2)+an2*dw(3))+dw(4)-dpda*dw(9))
c solid P
            b(2,ks)=t3*(cb2*dw(1)-dw(4)+dpda*dw(9))
            b(3,ks)=-an2*dw(2)+an1*dw(3)
c solid C+
            b(4,ks)=t1*( t2*(an1*dw(2)+an2*dw(3))+dw(4)-dpda*dw(9))
c gas C-
            b(5,ks)=t4*(-t6*(an1*dw(6)+an2*dw(7))+dw(8)-t5*dw(9))
c gas P
            b(6,ks)=t7*(c2*dw(5)-dw(8))
            b(7,ks)=-an2*dw(6)+an1*dw(7)
c gas C+
            b(8,ks)=t4*( t6*(an1*dw(6)+an2*dw(7))+dw(8)-t8*dw(9))
c solid P
            b(9,ks)=-t0*dw(9)
          end do
c
c eigenvalues * (dt/ds2) * rad / 2
          tmp=.5d0*rad*dr/ds(2)
          al(1)=tmp*(vnb-cb)
          al(2)=tmp*(vnb   )
          al(3)=al(2)
          al(4)=tmp*(vnb+cb)
          al(5)=tmp*(vn -c )
          al(6)=tmp*(vn    )
          al(7)=al(6)
          al(8)=tmp*(vn +c )
          al(9)=al(2)
c
          if (isolid) then
c
c C- contribution
            i=1
            if (b(i,1)*b(i,2).gt.0.d0.and.m.gt.0) then
              beta=dsign(1.d0,b(i,1))*dmin1(dabs(b(i,1)),dabs(b(i,2)))
              fact(1)=al(i)*beta
              fact(2)=fact(1)
              fact(3)=(min(al(i),0.d0)+.5d0)*beta
              fact(4)=(max(al(i),0.d0)-.5d0)*beta
              fact(5)=fact(1)
              do k=1,5
                w1(1,j1,k)=w1(1,j1,k)-fact(k)*rb
                w1(2,j1,k)=w1(2,j1,k)+fact(k)*cb*an1
                w1(3,j1,k)=w1(3,j1,k)+fact(k)*cb*an2
                w1(4,j1,k)=w1(4,j1,k)-fact(k)*gpb
              end do
            end if
c
c P contribution
            i=2
            if (b(i,1)*b(i,2).gt.0.d0.and.m.gt.0) then
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
c
c P contribution
            i=3
            if (b(i,1)*b(i,2).gt.0.d0.and.m.gt.0) then
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
c
c C+ contribution
            i=4
            if (b(i,1)*b(i,2).gt.0.d0.and.m.gt.0) then
              beta=dsign(1.d0,b(i,1))*dmin1(dabs(b(i,1)),dabs(b(i,2)))
              fact(1)=al(i)*beta
              fact(2)=fact(1)
              fact(3)=(min(al(i),0.d0)+.5d0)*beta
              fact(4)=(max(al(i),0.d0)-.5d0)*beta
              fact(5)=fact(1)
              do k=1,5
                w1(1,j1,k)=w1(1,j1,k)-fact(k)*rb
                w1(2,j1,k)=w1(2,j1,k)-fact(k)*cb*an1
                w1(3,j1,k)=w1(3,j1,k)-fact(k)*cb*an2
                w1(4,j1,k)=w1(4,j1,k)-fact(k)*gpb
              end do
            end if
c
          end if
c
          if (igas) then
c
c C- contribution
            i=5
            if (b(i,1)*b(i,2).gt.0.d0.and.m.gt.0) then
              beta=dsign(1.d0,b(i,1))*dmin1(dabs(b(i,1)),dabs(b(i,2)))
              fact(1)=al(i)*beta
              fact(2)=fact(1)
              fact(3)=(min(al(i),0.d0)+.5d0)*beta
              fact(4)=(max(al(i),0.d0)-.5d0)*beta
              fact(5)=fact(1)
              do k=1,5
                w1(5,j1,k)=w1(5,j1,k)-fact(k)*r
                w1(6,j1,k)=w1(6,j1,k)+fact(k)*c*an1
                w1(7,j1,k)=w1(7,j1,k)+fact(k)*c*an2
                w1(8,j1,k)=w1(8,j1,k)-fact(k)*gp
              end do
            end if
c
c P contribution
            i=6
            if (b(i,1)*b(i,2).gt.0.d0.and.m.gt.0) then
              beta=dsign(1.d0,b(i,1))*dmin1(dabs(b(i,1)),dabs(b(i,2)))
              fact(1)=al(i)*beta
              fact(2)=fact(1)
              fact(3)=(min(al(i),0.d0)+.5d0)*beta
              fact(4)=(max(al(i),0.d0)-.5d0)*beta
              fact(5)=fact(1)
              do k=1,5
                w1(5,j1,k)=w1(5,j1,k)-fact(k)
              end do
            end if
c
c P contribution
            i=7
            if (b(i,1)*b(i,2).gt.0.d0.and.m.gt.0) then
              beta=dsign(1.d0,b(i,1))*dmin1(dabs(b(i,1)),dabs(b(i,2)))
              fact(1)=al(i)*beta
              fact(2)=fact(1)
              fact(3)=(min(al(i),0.d0)+.5d0)*beta
              fact(4)=(max(al(i),0.d0)-.5d0)*beta
              fact(5)=fact(1)
              do k=1,5
                w1(6,j1,k)=w1(6,j1,k)+fact(k)*an2
                w1(7,j1,k)=w1(7,j1,k)-fact(k)*an1
              end do
            end if
c
c C+ contribution
            i=8
            if (b(i,1)*b(i,2).gt.0.d0.and.m.gt.0) then
              beta=dsign(1.d0,b(i,1))*dmin1(dabs(b(i,1)),dabs(b(i,2)))
              fact(1)=al(i)*beta
              fact(2)=fact(1)
              fact(3)=(min(al(i),0.d0)+.5d0)*beta
              fact(4)=(max(al(i),0.d0)-.5d0)*beta
              fact(5)=fact(1)
              do k=1,5
                w1(5,j1,k)=w1(5,j1,k)-fact(k)*r
                w1(6,j1,k)=w1(6,j1,k)-fact(k)*c*an1
                w1(7,j1,k)=w1(7,j1,k)-fact(k)*c*an2
                w1(8,j1,k)=w1(8,j1,k)-fact(k)*gp
              end do
            end if
c
          end if
c
c solid P contribution
          i=9
          if (b(i,1)*b(i,2).gt.0.d0.and.m.gt.0) then
            beta=dsign(1.d0,b(i,1))*dmin1(dabs(b(i,1)),dabs(b(i,2)))
            fact(1)=al(i)*beta
            fact(2)=fact(1)
            fact(3)=(min(al(i),0.d0)+.5d0)*beta
            fact(4)=(max(al(i),0.d0)-.5d0)*beta
            fact(5)=fact(1)
            if (isolid) then
              do k=1,5
                w1(4,j1,k)=w1(4,j1,k)+fact(k)*sig*dpda
              end do
            end if
            if (igas) then
              do k=1,5
                w1(5,j1,k)=w1(5,j1,k)+fact(k)*dvda*r*dv
                w1(6,j1,k)=w1(6,j1,k)-fact(k)*dvda*c2*an1
                w1(7,j1,k)=w1(7,j1,k)-fact(k)*dvda*c2*an2
                w1(8,j1,k)=w1(8,j1,k)+fact(k)*dvda*gp*dv
              end do
            end if
            do k=1,5
              w1(9,j1,k)=w1(9,j1,k)+fact(k)*sig
            end do
          end if
c
c solid P contribution (advected scalars)
          if (isolid) then
            m0=100
            if (m.gt.9) m0=10
            do i=m0,m
              b1=w(i,j1,2)-w(i,j1,1)
              b2=w(i,j1,3)-w(i,j1,2)
              if (b1*b2.gt.0.d0.and.m.gt.0) then
                beta=dsign(1.d0,b1)*dmin1(dabs(b1),dabs(b2))
                fact(1)=al(2)*beta
                fact(2)=fact(1)
                fact(3)=(min(al(2),0.d0)+.5d0)*beta
                fact(4)=(max(al(2),0.d0)-.5d0)*beta
                fact(5)=fact(1)
                do k=1,5
                  w1(i,j1,k)=w1(i,j1,k)+fact(k)
                end do
              end if
            end do
          end if
c
c         dar=w(9,j1+1,2)-w(9,j1-1,2)
c         das=w(9,j1,3)-w(9,j1,1)
c         utilde=dr*(a1(1,1,j1)*w(2,j1,2)+a1(1,2,j1)*w(3,j1,2))/ds(1)
c         vtilde=dr*(a1(2,1,j1)*w(2,j1,2)+a1(2,2,j1)*w(3,j1,2))/ds(2)
c         w1(9,j1,1)=w(9,j1,2)-.25d0*((utilde+1.d0)*dar+vtilde*das)
c         w1(9,j1,2)=w(9,j1,2)-.25d0*((utilde-1.d0)*dar+vtilde*das)
c         w1(9,j1,3)=w(9,j1,2)-.25d0*(utilde*dar+(vtilde+1.d0)*das)
c         w1(9,j1,4)=w(9,j1,2)-.25d0*(utilde*dar+(vtilde-1.d0)*das)
c         w1(9,j1,5)=w(9,j1,2)-.25d0*(utilde*dar+vtilde*das)
c
          do k=1,4
            if (w1(1,j1,k).le.0.d0.or.w1(5,j1,k).le.0.d0.or.
     *          w1(4,j1,k).le.p0  .or.w1(8,j1,k).le.0.d0) then
              do i=1,m
                w1(i,j1,k)=w(i,j1,2)
              end do
            else
              w1(9,j1,k)=max(min(w1(9,j1,k),1.d0-abtiny),abtiny)
            end if
          end do
          w1(9,j1,5)=max(min(w1(9,j1,5),1.d0-abtiny),abtiny)
c
        end if
      end do
c
      return
      end
c
c++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
c
      subroutine cmpfree (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
     *                    dr,ds,j2,rx,det,rx2,det2,up,mask,w1,move)
c
c free-stream correction for non-Cartesian grids
c
      implicit real*8 (a-h,o-z)
      dimension rx(nd1a:nd1b,nd2a:nd2b,2,2),det(nd1a:nd1b,nd2a:nd2b),
     *          rx2(nd1a:nd1b,nd2a:nd2b,2,2),det2(nd1a:nd1b,nd2a:nd2b),
     *          up(nd1a:nd1b,nd2a:nd2b,m),mask(nd1a:nd1b),ds(2),fx(9),
     *          w1(m,md1a:md1b)
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0,bgas
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
c solid and gas volume fractions
            as=w1(9,j1)
            ag=1.d0-as
c
c solid and gas momenta
            rus=w1(1,j1)*w1(2,j1)
            rvs=w1(1,j1)*w1(3,j1)
            rug=w1(5,j1)*w1(6,j1)
            rvg=w1(5,j1)*w1(7,j1)
c
c solid and gas total enthalpies
            hs=gam(1)*(w1(4,j1)+ps0)/gm1(1)+w1(1,j1)*compac(as,0)
     *         +.5d0*(rus*w1(2,j1)+rvs*w1(3,j1))
            hg=w1(8,j1)*(1.d0+1.d0/(gm1(2)*(1.d0+bgas*w1(5,j1))))
     *         +.5d0*(rug*w1(6,j1)+rvg*w1(7,j1))
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
            fx(1)=as*rus
            fx(2)=as*(rus*w1(2,j1)+w1(4,j1))
            fx(3)=as*rus*w1(3,j1)
            fx(4)=as*hs*w1(2,j1)
            fx(5)=ag*rug
            fx(6)=ag*(rug*w1(6,j1)+w1(8,j1))
            fx(7)=ag*rug*w1(7,j1)
            fx(8)=ag*hg*w1(6,j1)
            do i=1,8
              up(j1,j2,i)=up(j1,j2,i)+da*fx(i)
            end do
c
c y contribution
            k=2
            da=(((rx(j1+1,j2,1,k)+rx(j1,j2,1,k))*d1p
     *          -(rx(j1-1,j2,1,k)+rx(j1,j2,1,k))*d1m)*dr1
     *         +((rx(j1,j2+1,2,k)+rx(j1,j2,2,k))*d2p
     *          -(rx(j1,j2-1,2,k)+rx(j1,j2,2,k))*d2m)*dr2)/det(j1,j2)
            fx(1)=as*rvs
            fx(2)=as*rvs*w1(2,j1)
            fx(3)=as*(rvs*w1(3,j1)+w1(4,j1))
            fx(4)=as*hs*w1(3,j1)
            fx(5)=ag*rvg
            fx(6)=ag*rvg*w1(6,j1)
            fx(7)=ag*(rvg*w1(7,j1)+w1(8,j1))
            fx(8)=ag*hg*w1(7,j1)
            do i=1,8
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
c solid and gas volume fractions
            as=w1(9,j1)
            ag=1.d0-as
c
c solid and gas momenta
            rus=w1(1,j1)*w1(2,j1)
            rvs=w1(1,j1)*w1(3,j1)
            rug=w1(5,j1)*w1(6,j1)
            rvg=w1(5,j1)*w1(7,j1)
c
c solid and gas total enthalpies
            hs=gam(1)*(w1(4,j1)+ps0)/gm1(1)+w1(1,j1)*compac(as,0)
     *         +.5d0*(rus*w1(2,j1)+rvs*w1(3,j1))
            hg=w1(8,j1)*(1.d0+1.d0/(gm1(2)*(1.d0+bgas*w1(5,j1))))
     *         +.5d0*(rug*w1(6,j1)+rvg*w1(7,j1))
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
            fx(1)=as*rus
            fx(2)=as*(rus*w1(2,j1)+w1(4,j1))
            fx(3)=as*rus*w1(3,j1)
            fx(4)=as*hs*w1(2,j1)
            fx(5)=ag*rug
            fx(6)=ag*(rug*w1(6,j1)+w1(8,j1))
            fx(7)=ag*rug*w1(7,j1)
            fx(8)=ag*hg*w1(6,j1)
            do i=1,8
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
            fx(1)=as*rvs
            fx(2)=as*rvs*w1(2,j1)
            fx(3)=as*(rvs*w1(3,j1)+w1(4,j1))
            fx(4)=as*hs*w1(3,j1)
            fx(5)=ag*rvg
            fx(6)=ag*rvg*w1(6,j1)
            fx(7)=ag*(rvg*w1(7,j1)+w1(8,j1))
            fx(8)=ag*hg*w1(7,j1)
            do i=1,8
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
c   multiphase flux calculations
c
c++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
c
      subroutine chkflux (m,aj,a0,a1,a2,wl,wr,wlout,wrout,fl,fr,
     *                    speed,method,errfl,errfr,errwl,errwr)
c
c probably only works if m=9, i.e. no advected scalars (I did not try
c to modify this subroutine for the case m>9)
c
      implicit double precision (a-h,o-z)
      dimension wl(m),wr(m),wlout(m),wrout(m),fl(m),fr(m)
      dimension wl1d(7),wr1d(7),fl1d(7),fr1d(7),ic(7)
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0,bgas
      common / cutdat / abmin1
      common / gasdat1d / gam1d(2),gm11d(2),gp11d(2),
     *                    em1d(2),ep1d(2),p01d(2),bgas1d
      common / cutdat1d / abmin11d
      common / flxdat1d / iflux1d
      common / comdat / cfact1,cfact2,heat
c
c      data ic / 9, 1, 2, 4, 5, 6, 8 /
      data ic / 9, 1, 3, 4, 5, 7, 8 /
c
      do j=1,2
        gam1d(j)=gam(j)
        gm11d(j)=gm1(j)
        gp11d(j)=gp1(j)
        em1d(j)=em(j)
        ep1d(j)=ep(j)
      end do
      p01d(1)=ps0
      p01d(2)=0.d0
      bgas1d=bgas
      abmin11d=abmin1
      iflux1d=1
c
      m1d=7
      do i=1,m1d
        wl1d(i)=wl(ic(i))
        wr1d(i)=wr(ic(i))
      end do
c
      if (dabs(wr1d(1)).lt.1.d-6) then
        write(6,*)'wl =',wl
        write(6,*)'wr =',wr
        pause
      end if
c
      speed1d=0.d0
      method1d=method
      call cmpflux1d (m1d,wl1d,wr1d,fl1d,fr1d,speed1d,method1d)
c
      rad=dsqrt(a1**2+a2**2)
      aj1=rad*aj
      errfl=0.d0
      errfr=0.d0
      errwl=0.d0
      errwr=0.d0
      do i=1,m1d
        errfl=max(dabs(fl1d(i)-fl(ic(i))/aj1),errfl)
        errfr=max(dabs(fr1d(i)-fr(ic(i))/aj1),errfr)
        errwl=max(dabs(wl1d(i)-wlout(ic(i))),errwl)
        errwr=max(dabs(wr1d(i)-wrout(ic(i))),errwr)
      end do
c      write(66,100)errfl,errfr,errwl,errwr
c  100 format(4(1x,1pe9.2))
c
c      if (errwl.gt.1.d-14) then
      if (max(errfl,errfr).gt.1.d-4) then
        write(6,*)'writing gdflux.out ...'
        open (40,file='gdflux.out')
        write(40,400)m,(wl(i),i=1,m),(wr(i),i=1,m)
  400   format(1x,i2,2(/,9(1x,1pe22.15)))
        do i=1,2
          write(40,401)gam(i),gm1(i),gp1(i),em(i),ep(i)
  401     format(5(1x,1pe22.15))
        end do
        write(40,402)ps0,bgas,abmin1,method,aj,a0,a1,a2,
     *               cfact1,cfact2,heat
  402   format(1x,3(1pe22.15,1x),i2,/,4(1x,1pe22.15),/,
     *         3(1x,1pe22.15))
        stop
      end if
c
      return
      end
c
c+++++++++++++++++++++++++++++++++++++++++++++++++++
c
      subroutine cmpflux (m,aj,a0,a1,a2,wl,wr,fl,fr,speed,method)
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
      dimension wl(m),wr(m),fl(m),fr(m)
      dimension an(2),pm(2,2),vm(2,2),dv(2,2),rm(2),fp(2,2),gp(2)
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0,bgas
      common / prmdat / alpha(2,2),r(2,2),v(2,2),p(2,2),
     *                  a(2,2),b(2,2),c(2,2)
      common / cntdat / icnt(4)
c..timings
      common / timing / tflux(10),tslope,tsource
c
      if (m.lt.0) then
        write(6,*)'cmpflux(in)'
        write(6,*)'aj,a0,a1,a2=',aj,a0,a1,a2
        write(6,123)(i,wl(i),i=1,9),(i,wr(i),i=1,9)
  123   format('wl=',/,9(1x,i1,1x,f15.8,/),
     *         'wr=',/,9(1x,i1,1x,f15.8,/))
        pause
      end if
c
c icnt(1) counts the total number of flux calculations
      icnt(1)=icnt(1)+1
c
c assign volume fractions
      alpha(1,1)=wl(9)
      alpha(1,2)=1.d0-wl(9)
      alpha(2,1)=wr(9)
      alpha(2,2)=1.d0-wr(9)
c
c normalize metrics of the mapping
      rad=dsqrt(a1**2+a2**2)
c     an0=a0/rad            ! moving grids not implemented
      an(1)=a1/rad
      an(2)=a2/rad
c
c assume virial EOS if bgas>tiny, otherwise use the old ideal EOS code
      if (bgas.gt.1.d-14) then
c
        call ovtime (time0)
c
c compute solid middle state based on stiffened ideal EOS
        call cmpideals (1,an,wl(1),wr(1),pm(1,1),vm(1,1),fp(1,1),ier)
        if (ier.ne.0) then
          write(6,110)ier,wl,wr
  110     format('Error (cmpflux) : unable to compute solid middle ',
     *           'state, ier=',i1,/,'wl=',9(1x,1pe15.8),/,
     *                              'wr=',9(1x,1pe15.8))
          stop
        end if
c
        call ovtime (time1)
        tflux(2)=tflux(2)+time1-time0
c
c compute gas middle state based on virial EOS
        call cmpvirial (2,an,wl(5),wr(5),pm(1,2),vm(1,2),fp(1,2),
     *                  gp,rm,ier)
        if (ier.ne.0) then
          write(6,120)ier,wl,wr,an,bgas
  120     format('Error (cmpflux) : unable to compute gas middle ',
     *           'state, ier=',i1,/,'wl=',9(1x,1pe15.8),/,
     *                              'wr=',9(1x,1pe15.8),/,
     *           'an=',2(1pe15.8),'  bgas=',1pe15.8)
          stop
        end if
c
        call ovtime (time2)
        tflux(3)=tflux(3)+time2-time1
c
c compute coupled middle state and flux
        aj1=rad*aj
        call cmpcouplev (m,an,aj1,wl,wr,rm,vm,pm,fp,gp,fl,fr,
     *                   method,ier)
        if (ier.ne.0) then
          write(6,*)'Error (cmpflux) : unable to compute coupled ',
     *              'state, ier=',ier
          stop
        end if
c
c..compute maximum wave speed
        call maxspdv (rad,speed)
c
      else
c
c..compute decoupled middle state for each phase
        do j=1,2
          k=4*j-3
          call cmpmiddle (j,an,wl(k),wr(k),pm(1,j),vm(1,j),dv(1,j),ier)
          if (ier.ne.0) then
            write(6,100)ier,wl,wr
  100       format('Error (cmpflux) : unable to compute middle ',
     *             'state, ier=',i1,/,'wl=',9(1x,1pe15.8),/,
     *                                'wr=',9(1x,1pe15.8))
            stop
          end if
        end do
c
c..compute coupled middle state and fluxes, or decoupled fluxes
        aj1=rad*aj
        call cmpcouple (m,an,aj1,pm,vm,dv,wl,wr,fl,fr,
     *                  method,ier)
        if (ier.ne.0) then
          write(6,*)'Error (cmpflux) : unable to compute coupled ',
     *              'state, ier=',ier
          stop
        end if
c
c..compute maximum wave speed
        call maxspd (rad,pm,speed)
c
      end if
c
      if (m.lt.0) then
        write(6,*)'cmpflux(out)'
        write(6,*)'speed=',speed
        write(6,124)(i,fl(i),i=1,9),(i,fr(i),i=1,9)
  124   format('fl=',/,9(1x,i1,1x,f15.8,/),
     *         'fr=',/,9(1x,i1,1x,f15.8,/))
        pause
      end if
c
      return
      end
c
c+++++++++++++++++++++++++++++++++++++++++++++++++++
c
      subroutine cmpideals (j,an,wl,wr,pm,vm,fp,ier)
c
c Middle states for the Riemann problem for the stiffened ideal EOS
c
      implicit double precision (a-h,o-z)
      dimension an(2),wl(4),wr(4),pm(2),vm(2),fp(2)
      dimension dv(2)
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0,bgas
      common / prmdat / alpha(2,2),r(2,2),v(2,2),p(2,2),
     *                  a(2,2),b(2,2),c(2,2)
c      common / cmpmid / toli,tolv,itmax
      data toli, itmax / 1.d-3, 6 /
c     data pratio, ptol / 2.d0, 1.d-6 /
      data pratio, pfact / 2.d0, 1.d-4 /
c
      ier=0
c
c minimum pressure tolerance
c      ptol=pfact*min(wl(3),wr(3))
c
c minimum pressure tolerance, assumes p0(2)=0
      if (j.eq.1) then
        p0=ps0
        ptol=-(1.d0-pfact)*ps0
      else
        p0=0.d0
        ptol=pfact*min(wl(4),wr(4))
      end if
c
c left primitive state variables
      r(1,j)=wl(1)
      v(1,j)=an(1)*wl(2)+an(2)*wl(3)
      p(1,j)=wl(4)
c
c right primitive state variables
      r(2,j)=wr(1)
      v(2,j)=an(1)*wr(2)+an(2)*wr(3)
      p(2,j)=wr(4)
c
c sound speeds and some constants
      do i=1,2
        a(i,j)=2.d0/(gp1(j)*r(i,j))
        b(i,j)=gm1(j)*(p(i,j)+p0)/gp1(j)+p0
        c2=gam(j)*(p(i,j)+p0)/r(i,j)
        if (c2.le.0.d0) then
          write(6,*)'Error (cmpideals) : c2.le.0, i,j =',i,j
          ier=1
          return
        end if
        c(i,j)=dsqrt(c2)
      end do
c
c check for vacuum state (in the decoupled phases)
      if (2.d0*(c(1,j)+c(2,j))/gm1(j).le.v(2,j)-v(1,j)) then
        write(6,*)'Error (cmpideals) : vacuum found, j=',j
        ier=2
        return
      end if
c
c compute min/max pressures
c      pmin=min(p(1,j),p(2,j))
c      pmax=max(p(1,j),p(2,j))
      pmin=min(p(1,j)+p0,p(2,j)+p0)
      pmax=max(p(1,j)+p0,p(2,j)+p0)
c
c start with guess based on a linearization
      ppv=.5d0*(p(1,j)+p(2,j))
     *    -.125d0*(v(2,j)-v(1,j))*(r(1,j)+r(2,j))*(c(1,j)+c(2,j))
c      ppv=max(ppv,0.d0)
      ppv=max(ppv+p0,0.d0)
c
c      if ((pmax+p0)/(pmin+p0).le.pratio
c     *   .and.pmin.le.ppv.and.pmax.ge.ppv) then
      if (pmax/pmin.le.pratio
     *   .and.pmin.le.ppv.and.pmax.ge.ppv) then
c        pstar=ppv
        pstar=ppv-p0
      else
        if (ppv.lt.pmin) then
c guess based on two rarefaction solution
          arg1=c(1,j)/((p(1,j)+p0)**em(j))
          arg2=c(2,j)/((p(2,j)+p0)**em(j))
          arg3=(c(1,j)+c(2,j)-.5d0*gm1(j)*(v(2,j)-v(1,j)))/(arg1+arg2)
          pstar=arg3**(1.d0/em(j))-p0
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
            arg=(pstar+p0)/(p(i,j)+p0)
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
        if (dabs(dp)/(pstar+p0).lt.toli) goto 1
c
      end do
c
c print warning if middle pstar state is not converged
      write(6,*)'Warning (cmpideals) : pstar not converged'
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
c+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
c
      subroutine cmpvirial (j,an,wl,wr,pm,vm,fp,gp,rm,ier)
c
c middle states of the Riemann problem for the virial EOS
c
      implicit real*8 (a-h,o-z)
      dimension an(2),wl(4),wr(4),pm(2),vm(2),fp(2),gp(2),rm(2)
      dimension dv(2),drm(2),esave(100)
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0,bgas
      common / prmdat / alpha(2,2),r(2,2),v(2,2),p(2,2),
     *                  a(2,2),b(2,2),c(2,2)
      common / romdat / aint0(2),r0(2),c0(2)
c      common / cmpmid / toli,tolv,itmax
      data pratio, pfact / 2.d0, 1.d-4 /
      data tolv, itmax / 1.d-3, 6 /
      data rmin / 1.d-4 /     ! minimum value of gas density to avoid negative densities during iteration
c
      ier=0
c
c minimum pressure tolerance
      ptol=pfact*min(wl(4),wr(4))
c
c left and right states
      r(1,j)=wl(1)
      v(1,j)=an(1)*wl(2)+an(2)*wl(3)
      p(1,j)=wl(4)
      r(2,j)=wr(1)
      v(2,j)=an(1)*wr(2)+an(2)*wr(3)
      p(2,j)=wr(4)
c
c compute sound speeds
      do i=1,2
        c2=p(i,j)*(gam(j)/r(i,j)+bgas*(gm1(j)+1.d0/(1.d0+bgas*r(i,j))))
        if (c2.lt.0.d0) then
          write(6,*)'Error (cmpvirial) : c2.lt.0, i=',i
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
        write(6,*)'Error (cmpvirial) : vacuum found, j=',j
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
          write(6,*)'Error (cmpvirial) : initial density is negative'
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
              write(6,*)'Error (cmpvirial) : two-shock solution failed'
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
      tolv=1.d-5
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
              write(6,*)'Error (cmpvirial) : cm2.lt.0, i=',i
              ier=3
              return
            end if
            cm=dsqrt(cm2)
            if (rm(i).le.0.d0) then
              write(6,*)'Error (cmpvirial) : rm(i).le.0'
              write(6,*)'iguess=',iguess
              pause
              ier=4
              return
            end if
            dv(i)=getdv(i,rm(i),cm)
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
              write(6,*)'Error (cmpvirial) : denom.le.0'
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
c        rm(1)=rm(1)-drm(1)
c        rm(2)=rm(2)-drm(2)
        rm(1)=max(rm(1)-drm(1),rmin)
        rm(2)=max(rm(2)-drm(2),rmin)
c
        if (rm(1).le.0.d0.or.rm(2).le.0.d0) then
          write(6,*)'Error (cmpvirial) : negative densities'
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
        if (err.lt.tolv) then
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
      write(6,*)'Error (cmpvirial) : iteration failed to converge'
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
c++++++++++++++++++++++++++++++++++++++++++++++
c
      subroutine cmpcouplev (m,an,aj,wl,wr,rm,vm,pm,fp,gp,fl,fr,
     *                       method,ier)
c
      implicit double precision (a-h,o-z)
      dimension an(2),wl(m),wr(m),rm(2),pm(2,2),vm(2,2),fp(2,2),gp(2),
     *          fl(m),fr(m),g(4),rmsave(2),vmsave(2,2),pmsave(2,2)
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0,bgas
      common / prmdat / alpha(2,2),r(2,2),v(2,2),p(2,2),
     *                  a(2,2),b(2,2),c(2,2)
      common / scalev / rm0save,rscale,pscale,vscale,gfact(4)
      common / cutdat / abmin1
      common / cmpflx / rtol, lcont
c..timings
      common / timing / tflux(10),tslope,tsource
c
      data sig, frac / 10.d0, .95d0 /   ! this is what Chris likes.
c      data sig, frac / 10.d0, .99d0 /
c
      ier=0
c
      call ovtime (time0)
c
      if (dabs(wr(9)-wl(9)).gt.1.d-14.and.
     *     min(wl(9),wr(9)).gt.abmin1) then
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
        dalp=wr(9)-wl(9)
c
    1   if (dabs(wl(9)-.5d0).lt.dabs(wr(9)-.5d0)) then
          alpha(2,1)=wl(9)+dalp
          alpha(2,2)=1.d0-alpha(2,1)
        else
          alpha(1,1)=wr(9)-dalp
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
            write(6,*)'Error (cmpcouplev) : cm2.le.0 (L1)'
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
            write(6,*)'Error (cmpcouplev) : cm2.le.0 (L2)'
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
c check whether to accept the linearized jump solution
        if (lcont.eq.0) then
c
          write(6,*)'lcont =',lcont
          pause
c
c compute residual of the jump conditions
c          iconf=0
          rm0save=rm0
          do i=1,2
            rmsave(i)=rm(i)
            do j=1,2
              vmsave(i,j)=vm(i,j)
              pmsave(i,j)=pm(i,j)
            end do
          end do
c
          call getgv (rm0,rm,vm,pm,g,resid,iconf,ier)
c
c          write(57,200)resid,(g(i),i=1,5)
c  200     format('   * residuals for linearize state, resid=',1pe9.2,/,
c     *           4x,5(1x,1pe9.2))
c
          itmax=4
c now specified in a common block
c         rtol=1.d-10
          if (resid.gt.rtol.or.ier.ne.0) then
            if (ier.eq.0) then
              call newtonv (rm0,rm,vm,pm,g,iconf,ier)
            end if
            if (ier.ne.0) then
              fact=bgas*max(r(1,2),r(2,2))
c              write(55,223)xpos,tpos,fact,lv
c  223         format(3(1x,1pe15.8),1x,i2,' 2')
              do it=1,itmax
                dalp=dalp/2.d0
c                write(6,*)'it,dalp=',it,dalp
                if (dabs(wl(9)-.5d0).lt.dabs(wr(9)-.5d0)) then
                  alpha(2,1)=wl(9)+dalp
                  alpha(2,2)=1.d0-alpha(2,1)
                else
                  alpha(1,1)=wr(9)-dalp
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
                call getgv (rm0,rm,vm,pm,g,resid,iconf,ier)
                if (ier.eq.0) then
                  call newtonv (rm0,rm,vm,pm,g,iconf,ier)
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
    2 alpha(1,1)=wl(9)
      alpha(1,2)=1.d0-wl(9)
      alpha(2,1)=wr(9)
      alpha(2,2)=1.d0-wr(9)
c
      call ovtime (time1)
      tflux(4)=tflux(4)+time1-time0
c
c-------------------------------------
c
c          compute fluxes
c
c-------------------------------------
c
c solid contact source contributions
      source =pm(2,1)*alpha(2,1)-pm(1,1)*alpha(1,1)
      source2=vm(1,1)*source
c
c check if solid contact is to the right or left of x=0
      if (vm(1,1).gt.0.d0) then
c
c tangential component of the solid velocity from left state
        vt=an(1)*wl(3)-an(2)*wl(2)
c
c right-of-solid-contact state (only need solid velocity and gas pressure)
        wr(2)=an(1)*vm(1,1)-an(2)*vt
        wr(3)=an(2)*vm(1,1)+an(1)*vt
        wr(8)=pm(2,2)
c
c solid phase:
        call getfx (1,1,alpha(1,1),pm(1,1),vm(1,1),vt,
     *              an,aj,wl(1),fl(1))
c
c add on solid contact source contributions
        fr(1)=fl(1)
        fr(2)=fl(2)+aj*an(1)*source
        fr(3)=fl(3)+aj*an(2)*source
        fr(4)=fl(4)+aj*source2
c
c gas phase:
        if (vm(1,2).ge.0.d0) then
c gas contact (and solid contact) to the right
c compute tangential component of the gas velocity from left state
          vt=an(1)*wl(7)-an(2)*wl(6)
          call getfxv (1,alpha(1,2),rm(1),vm(1,2),vt,pm(1,2),
     *                 an,aj,wl(5),fl(5),ier)
          if (ier.ne.0) return
        else
c gas contact to the left, solid contact to the right
c compute tangential component of the gas velocity from right state
          vt=an(1)*wr(7)-an(2)*wr(6)
          call getfxv2 (1,alpha(1,2),rm0,vm(1,2),vt,pm(1,2),
     *                  an,aj,wl(5),fl(5))
        end if
c
c add on solid contact source contributions
        fr(5)=fl(5)
        fr(6)=fl(6)-aj*an(1)*source
        fr(7)=fl(7)-aj*an(2)*source
        fr(8)=fl(8)-aj*source2
c
c solid volume fraction flux
        fl(9)= 0.d0
        fr(9)=-aj*vm(1,1)*(alpha(2,1)-alpha(1,1))
c
c flux for advected variables
        do i=10,m
          fl(i)= 0.d0
          fr(i)=-aj*vm(1,1)*(wr(i)-wl(i))
        end do
c
      else
c
c tangential component of the solid velocity from right state
        vt=an(1)*wr(3)-an(2)*wr(2)
c
c left-of-solid-contact state (only need solid velocity and gas pressure)
        wl(2)=an(1)*vm(1,1)-an(2)*vt
        wl(3)=an(2)*vm(1,1)+an(1)*vt
        wl(8)=pm(1,2)
c
c solid phase:
        call getfx (2,1,alpha(2,1),pm(2,1),vm(2,1),vt,
     *              an,aj,wr(1),fr(1))
c
c add on solid contact source contributions
        fl(1)=fr(1)
        fl(2)=fr(2)-aj*an(1)*source
        fl(3)=fr(3)-aj*an(2)*source
        fl(4)=fr(4)-aj*source2
c
c gas phase:
        if (vm(2,2).le.0.d0) then
c gas contact (and solid contact) to the left
c compute tangential component of the gas velocity from right state
          vt=an(1)*wr(7)-an(2)*wr(6)
          call getfxv (2,alpha(2,2),rm(2),vm(2,2),vt,pm(2,2),
     *                 an,aj,wr(5),fr(5),ier)
          if (ier.ne.0) return
        else
c gas contact to the right, solid contact to the left
c compute tangential component of the gas velocity from left state
          vt=an(1)*wl(7)-an(2)*wl(6)
          call getfxv2 (2,alpha(2,2),rm0,vm(2,2),vt,pm(2,2),
     *                  an,aj,wr(5),fr(5))
        end if
c
c add on solid contact source contributions
        fl(5)=fr(5)
        fl(6)=fr(6)+aj*an(1)*source
        fl(7)=fr(7)+aj*an(2)*source
        fl(8)=fr(8)+aj*source2
c
c solid volume fraction flux
        fl(9)= aj*vm(1,1)*(alpha(2,1)-alpha(1,1))
        fr(9)= 0.d0
c
c flux for advected variables
        do i=10,m
          fl(i)= aj*vm(1,1)*(wr(i)-wl(i))
          fr(i)= 0.d0
        end do
c
      end if
c
      call ovtime (time2)
      tflux(5)=tflux(5)+time2-time1
c
      return
      end
c
c++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
c
      subroutine getfxv (i,alfa,rm,vm,vt,pm,an,aj,wstar,fx,ier)
c
c compute solid or gas flux for i=side and j=phase
c
      implicit double precision (a-h,o-z)
      dimension an(2),wstar(4),fx(4)
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0,bgas
      common / prmdat / alpha(2,2),r(2,2),v(2,2),p(2,2),
     *                  a(2,2),b(2,2),c(2,2)
c
      ier=0
c
c set j=2 (virial gas)
      j=2
c
c set isgn=1 for i=1 and isgn=-1 for i=2
      isgn=3-2*i
c
      if (rm.gt.r(i,j)) then
c shock cases
        sp=isgn*v(i,j)-dsqrt((rm/r(i,j))*(pm-p(i,j))/(rm-r(i,j)))
        if (sp.ge.0.d0) then
c left (i=1) or right (i=2)
          wstar(1)=r(i,j)
          wstar(2)=an(1)*v(i,j)-an(2)*vt
          wstar(3)=an(2)*v(i,j)+an(1)*vt
          wstar(4)=p(i,j)
        else
c middle left (i=1) or middle right (i=2)
          wstar(1)=rm
          wstar(2)=an(1)*vm-an(2)*vt
          wstar(3)=an(2)*vm+an(1)*vt
          wstar(4)=pm
        end if
      else
c rarefaction cases
        if (isgn*v(i,j)-c(i,j).ge.0.d0) then
c left (i=1) or right (i=2)
          wstar(1)=r(i,j)
          wstar(2)=an(1)*v(i,j)-an(2)*vt
          wstar(3)=an(2)*v(i,j)+an(1)*vt
          wstar(4)=p(i,j)
        else
c left middle or sonic (i=1) or right middle or sonic (i=2)
          fact=1.d0+bgas*rm
          cm2=pm*(gam(2)/rm+bgas*(gm1(2)+1.d0/fact))
          if (cm2.le.0.d0) then
            write(6,*)'Error (getfxv) : cm2.le.0'
            ier=1
            return
          end if
          if (isgn*vm-dsqrt(cm2).gt.0.d0) then
c sonic left (i=1) or right (i=2)
c            call sonicv (i,isgn,r(i,j),rm,an,vt,wstar)
            call sonicv2 (i,isgn,rm,vm,cm2,an,vt,wstar)
          else
c middle left (i=1) or right (i=2)
            wstar(1)=rm
            wstar(2)=an(1)*vm-an(2)*vt
            wstar(3)=an(2)*vm+an(1)*vt
            wstar(4)=pm
          end if
        end if
      end if
c
c compute flux
      q2=wstar(2)**2+wstar(3)**2
      vn=an(1)*wstar(2)+an(2)*wstar(3)
      energy=wstar(4)/(gm1(2)*(1.d0+bgas*wstar(1)))+.5d0*wstar(1)*q2
c
      fact1=alfa*aj
      fact2=wstar(1)*vn
      fx(1)=fact1*fact2
      fx(2)=fact1*(fact2*wstar(2)+an(1)*wstar(4))
      fx(3)=fact1*(fact2*wstar(3)+an(2)*wstar(4))
      fx(4)=fact1*(energy+wstar(4))*vn
c
      return
      end
c
c++++++++++++++++++++++++++++++++++++++++++++++++++
c
      subroutine sonicv (i,isgn,ra,rb,an,vt,wstar)
c
c use bisection to find the sonic point => VERY SLOW
c
      implicit real*8 (a-h,o-z)
      dimension an(2),wstar(4)
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0,bgas
      common / prmdat / alpha(2,2),r(2,2),v(2,2),p(2,2),
     *                  a(2,2),b(2,2),c(2,2)
      data tol, itbig / 1.d-4, 20 /
c
      arg=dabs(rb-ra)/(tol*max(ra,rb))
      itmax=dlog(max(arg,1.d-15))/dlog(2.d0)
      itmax=max(itmax,1)
c
      if (itmax.gt.itbig) then
        write(6,*)'Warning (sonicv) : itmax.gt.itbig, itmax=',itmax
      end if
c
      j=2
      facti=1.d0+bgas*r(i,j)
c
      do it=1,itmax
c
        rm=.5d0*(ra+rb)
        factm=1.d0+bgas*rm
        pm=p(i,j)*((rm/r(i,j))**gam(j))*factm/facti
     *             *dexp(gm1(j)*bgas*(rm-r(i,j)))
        cm2=pm*(gam(j)/rm+bgas*(gm1(j)+1.d0/factm))
        if (cm2.lt.0.d0) then
          write(6,*)'Error (sonicv) : cm2.lt.0, i=',i
          stop
        end if
        cm=dsqrt(cm2)
        if (rm.le.0.d0) then
          write(6,*)'Error (sonicv) : rm.le.0'
          stop
        end if
        dv=getdv(i,rm,cm)
c
        vm=v(i,j)-isgn*dv
        if (isgn*vm-cm.gt.0.d0) then
          rb=rm
        else
          ra=rm
        end if
c
      end do
c
c      write(6,*)'sonic: ra,rb =',ra,rb
      wstar(1)=rm
      wstar(2)=an(1)*vm-an(2)*vt
      wstar(3)=an(2)*vm+an(1)*vt
      wstar(4)=pm
c
      return
      end
c
c++++++++++++++++++++++++++++++++++++++++++++++++++
c
      subroutine sonicv2 (i,isgn,rm,vm,cm2,an,vt,wstar)
c
c Use just one step of false position (i.e. a linear fit), much faster.
c
      implicit real*8 (a-h,o-z)
      dimension an(2),wstar(4)
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
          write(6,*)'Error (sonicv2) : cm2.lt.0, i=',i
          stop
        end if
        cm=dsqrt(cm2)
        vm=cm/isgn
        wstar(1)=rm
        wstar(2)=an(1)*vm-an(2)*vt
        wstar(3)=an(2)*vm+an(1)*vt
        wstar(4)=pm
      else
        write(6,*)'Error (sonicv2) : no root found'
      end if
c
      return
      end
c
c+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
c
      subroutine getfxv2 (i,alfa,rm,vm,vt,pm,an,aj,wstar,fx)
c
c compute gas flux for the case when solid contact and gas contact
c are on either side of x=0.  If i=1, then solid contact is to the
c right of x=0, and if i=2, then solid contact is to the left.
c
      implicit double precision (a-h,o-z)
      dimension an(2),wstar(4),fx(4)
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0,bgas
c
      wstar(1)=rm
      wstar(2)=an(1)*vm-an(2)*vt
      wstar(3)=an(2)*vm+an(1)*vt
      wstar(4)=pm
c
c compute flux
      q2=wstar(2)**2+wstar(3)**2
      vn=an(1)*wstar(2)+an(2)*wstar(3)
      energy=wstar(4)/(gm1(2)*(1.d0+bgas*wstar(1)))+.5d0*wstar(1)*q2
c
      fact1=alfa*aj
      fact2=wstar(1)*vn
      fx(1)=fact1*fact2
      fx(2)=fact1*(fact2*wstar(2)+an(1)*wstar(4))
      fx(3)=fact1*(fact2*wstar(3)+an(2)*wstar(4))
      fx(4)=fact1*(energy+wstar(4))*vn
c
      return
      end
c
c+++++++++++++++++++++++++++++++++++++++++++++++++++
c
      subroutine cmpmiddle (j,an,wl,wr,pm,vm,dv,ier)
c
c Approximate-state Riemann solver, see Toro
c
      implicit double precision (a-h,o-z)
      dimension an(2),wl(4),wr(4),pm(2),vm(2),dv(2)
      dimension vdif(2,2)
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0,bgas
      common / prmdat / alpha(2,2),r(2,2),v(2,2),p(2,2),
     *                  a(2,2),b(2,2),c(2,2)
      data pratio, pfact / 2.d0, 1.d-4 /
      data tol, itmax / 1.d-3, 6 /
c
      ier=0
c
c minimum pressure tolerance
      if (j.eq.1) then
        p0=ps0
        ptol=-(1.d0-pfact)*ps0
      else
        p0=0.d0
        ptol=pfact*min(wl(4),wr(4))
      end if
c
c left primitive state variables
      r(1,j)=wl(1)
      v(1,j)=an(1)*wl(2)+an(2)*wl(3)
      p(1,j)=wl(4)
c
c right primitive state variables
      r(2,j)=wr(1)
      v(2,j)=an(1)*wr(2)+an(2)*wr(3)
      p(2,j)=wr(4)
c
c sound speeds and some constants
      do i=1,2
        a(i,j)=2.d0/(gp1(j)*r(i,j))
        b(i,j)=gm1(j)*(p(i,j)+p0)/gp1(j)+p0
        c2=gam(j)*(p(i,j)+p0)/r(i,j)
        if (c2.le.0.d0) then
          write(6,*)'Error (cmpmiddle) : c2.le.0, i,j =',i,j
          ier=1
          return
        end if
        c(i,j)=dsqrt(c2)
      end do
c
c check for vacuum state (in the decoupled phases)
      if (2.d0*(c(1,j)+c(2,j))/gm1(j).le.v(2,j)-v(1,j)) then
        write(6,*)'Error (cmpmiddle) : vacuum found, j=',j
        ier=2
        return
      end if
c
c compute min/max pressures
      pmin=min(p(1,j)+p0,p(2,j)+p0)
      pmax=max(p(1,j)+p0,p(2,j)+p0)
c
c start with guess based on a linearization
      ppv=.5d0*(p(1,j)+p(2,j))
     *    -.125d0*(v(2,j)-v(1,j))*(r(1,j)+r(2,j))*(c(1,j)+c(2,j))
      ppv=max(ppv+p0,0.d0)
c
      if (pmax/pmin.le.pratio
     *   .and.pmin.le.ppv.and.pmax.ge.ppv) then
        pstar=ppv-p0
      else
        if (ppv.lt.pmin) then
c guess based on two rarefaction solution
          arg1=c(1,j)/((p(1,j)+p0)**em(j))
          arg2=c(2,j)/((p(2,j)+p0)**em(j))
          arg3=(c(1,j)+c(2,j)-.5d0*gm1(j)*(v(2,j)-v(1,j)))/(arg1+arg2)
          pstar=arg3**(1.d0/em(j))-p0
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
            arg=(pstar+p0)/(p(i,j)+p0)
            fact=2.d0*c(i,j)/gm1(j)
            vdif(1,i)=fact*(arg**em(j)-1.d0)
            vdif(2,i)=1.d0/(r(i,j)*c(i,j)*arg**ep(j))
          end if
        end do
c
c determine change to pressure in the middle state
        dp=(vdif(1,1)+vdif(1,2)+v(2,j)-v(1,j))/(vdif(2,1)+vdif(2,2))
        pstar=pstar-dp
c
        if (dabs(dp)/(pstar+p0).lt.tol) goto 1
c
      end do
c
c print warning if middle pstar state is not converged
      write(6,*)'Warning (cmpmiddle) : pstar not converged'
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
c++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
c
      subroutine cmpcouple (m,an,aj,pm,vm,dv,wl,wr,fl,fr,method,ier)
c
      implicit double precision (a-h,o-z)
      dimension an(2),pm(2,2),vm(2,2),dv(2,2),wl(m),wr(m),fl(m),fr(m)
      dimension g(4),pm0(2,2),vm0(2,2),dummy(2,2)
      logical ctest
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0,bgas
      common / prmdat / alpha(2,2),r(2,2),v(2,2),p(2,2),
     *                  a(2,2),b(2,2),c(2,2)
      common / scales / pscale,vscale,gfact(4)
      common / cntdat / icnt(4)
      common / cutdat / abmin1
      data sig, frac, tol / 10.d0, .99d0, 1.d-10 /
      data pmin, c2min / 1.d-6, 1.d-12 /
c
      ier=0
c
c perform a coupled flux calculation only if the difference between
c the left and right values of alpha_bar is greater than a tolerance
c and if there is a minimum amount of solid left in both states
      if (dabs(wr(9)-wl(9)).gt.1.d-14.and.
     *     max(wl(9),wr(9)).gt.abmin1) then
c
c icnt(2) counts the total number of coupled flux calculations
        icnt(2)=icnt(2)+1
c
c compute gas density
        dvm=vm(1,2)-vm(1,1)
        if (dvm.ge.0.d0) then
          if (pm(1,2).gt.p(1,2)) then
            rm=r(1,2)*(gp1(2)*pm(1,2)/p(1,2)+gm1(2))
     *               /(gm1(2)*pm(1,2)/p(1,2)+gp1(2))
          else
            rm=r(1,2)*(pm(1,2)/p(1,2))**(1.d0/gam(2))
          end if
        else
          if (pm(2,2).gt.p(2,2)) then
            rm=r(2,2)*(gp1(2)*pm(2,2)/p(2,2)+gm1(2))
     *               /(gm1(2)*pm(2,2)/p(2,2)+gp1(2))
          else
            rm=r(2,2)*(pm(2,2)/p(2,2))**(1.d0/gam(2))
          end if
        end if
c
c compute pressure and velocity scales
        pscale=max(pm(1,1),pm(1,2),pmin)
        vscale=dsqrt(max(gam(2)*pm(1,2)/rm,c2min))
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
        cm=frac*dsqrt(max(gam(2)*pm(1,2)/rm,c2min))
        arg1=sig*(1.d0+dvm/cm)
        arg2=sig*(1.d0-dvm/cm)
        dvm=.5d0*cm*log(cosh(arg1)/cosh(arg2))/sig
c
c linear update for middle gas pressures
        del=gam(2)*pm(1,2)*dvm*eps
     *      /(agm*(gam(2)*pm(1,2)-rm*dvm**2)*(dv(1,2)+dv(2,2)))
        pm(1,2)=pm(1,2)+(rm*dvm*dv(2,2)+1.d0)*del
        pm(2,2)=pm(2,2)-(rm*dvm*dv(1,2)-1.d0)*del
c
c compute residual of the jump conditions
        call getg (pm,vm,dummy,g,resid)
c
c if residual is too big, then try Newton
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
          ctest=.true.  ! *wdh* 110121
          call newton (pm,vm,ctest)
c
c reset middle pressures
          if (.not.ctest) then
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
c-------------------------------------
c
c          compute fluxes
c
c-------------------------------------
c
c solid contact source contributions
      source =pm(2,1)*alpha(2,1)-pm(1,1)*alpha(1,1)
      source2=vm(1,1)*source
c
c check if solid contact is to the right or left of x=0
      if (vm(1,1).gt.0.d0) then
c
c tangential component of the solid velocity from left state
        vt=an(1)*wl(3)-an(2)*wl(2)
c
c right-of-solid-contact state (only need solid velocity and gas pressure)
        wr(2)=an(1)*vm(1,1)-an(2)*vt
        wr(3)=an(2)*vm(1,1)+an(1)*vt
        wr(8)=pm(2,2)
c
c solid phase:
        call getfx (1,1,alpha(1,1),pm(1,1),vm(1,1),vt,
     *              an,aj,wl(1),fl(1))
c
c add on solid contact source contributions
        fr(1)=fl(1)
        fr(2)=fl(2)+aj*an(1)*source
        fr(3)=fl(3)+aj*an(2)*source
        fr(4)=fl(4)+aj*source2
c
c gas phase:
        if (vm(1,2).ge.0.d0) then
c gas contact (and solid contact) to the right
c compute tangential component of the gas velocity from left state
          vt=an(1)*wl(7)-an(2)*wl(6)
          call getfx (1,2,alpha(1,2),pm(1,2),vm(1,2),vt,
     *                an,aj,wl(5),fl(5))
        else
c gas contact to the left, solid contact to the right
c compute tangential component of the gas velocity from right state
          vt=an(1)*wr(7)-an(2)*wr(6)
          call getfx2 (1,alpha(1,2),pm(1,2),vm(1,2),vt,
     *                 an,aj,wl(5),fl(5))
        end if
c
c add on solid contact source contributions
        fr(5)=fl(5)
        fr(6)=fl(6)-aj*an(1)*source
        fr(7)=fl(7)-aj*an(2)*source
        fr(8)=fl(8)-aj*source2
c
c solid volume fraction flux
        fl(9)= 0.d0
        fr(9)=-aj*vm(1,1)*(alpha(2,1)-alpha(1,1))
c
c flux for advected variables
        do i=10,m
          fl(i)= 0.d0
          fr(i)=-aj*vm(1,1)*(wr(i)-wl(i))
        end do
c
      else
c
c tangential component of the solid velocity from right state
        vt=an(1)*wr(3)-an(2)*wr(2)
c
c left-of-solid-contact state (only need solid velocity and gas pressure)
        wl(2)=an(1)*vm(1,1)-an(2)*vt
        wl(3)=an(2)*vm(1,1)+an(1)*vt
        wl(8)=pm(1,2)
c
c solid phase:
        call getfx (2,1,alpha(2,1),pm(2,1),vm(2,1),vt,
     *              an,aj,wr(1),fr(1))
c
c add on solid contact source contributions
        fl(1)=fr(1)
        fl(2)=fr(2)-aj*an(1)*source
        fl(3)=fr(3)-aj*an(2)*source
        fl(4)=fr(4)-aj*source2
c
c gas phase:
        if (vm(2,2).le.0.d0) then
c gas contact (and solid contact) to the left
c compute tangential component of the gas velocity from right state
          vt=an(1)*wr(7)-an(2)*wr(6)
          call getfx (2,2,alpha(2,2),pm(2,2),vm(2,2),vt,
     *                an,aj,wr(5),fr(5))
        else
c gas contact to the right, solid contact to the left
c compute tangential component of the gas velocity from left state
          vt=an(1)*wl(7)-an(2)*wl(6)
          call getfx2 (2,alpha(2,2),pm(2,2),vm(2,2),vt,
     *                 an,aj,wr(5),fr(5))
        end if
c
c add on solid contact source contributions
        fl(5)=fr(5)
        fl(6)=fr(6)+aj*an(1)*source
        fl(7)=fr(7)+aj*an(2)*source
        fl(8)=fr(8)+aj*source2
c
c solid volume fraction flux
        fl(9)= aj*vm(1,1)*(alpha(2,1)-alpha(1,1))
        fr(9)= 0.d0
c
c flux for advected variables
        do i=10,m
          fl(i)= aj*vm(1,1)*(wr(i)-wl(i))
          fr(i)= 0.d0
        end do
c
      end if
c
      return
      end
c
c++++++++++++++++++++++++++++++++++++++++++++++++++++++++
c
      subroutine getfx (i,j,alfa,pm,vm,vt,an,aj,wstar,fx)
c
c compute solid or gas flux for i=side and j=phase
c
      implicit double precision (a-h,o-z)
      dimension an(2),wstar(4),fx(4)
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0,bgas
      common / prmdat / alpha(2,2),r(2,2),v(2,2),p(2,2),
     *                  a(2,2),b(2,2),c(2,2)
c
c set isgn=1 for i=1 and isgn=-1 for i=2
      isgn=3-2*i
c
      if (j.eq.1) then
        p0=ps0
      else
        p0=0.d0
      end if
c
      if (pm.gt.p(i,j)) then
c shock cases
        sp=isgn*v(i,j)-c(i,j)*dsqrt(ep(j)*(pm+p0)/(p(i,j)+p0)+em(j))
        if (sp.ge.0.d0) then
c left (i=1) or right (i=2)
          wstar(1)=r(i,j)
          wstar(2)=an(1)*v(i,j)-an(2)*vt
          wstar(3)=an(2)*v(i,j)+an(1)*vt
          wstar(4)=p(i,j)
        else
c middle left (i=1) or middle right (i=2)
          wstar(1)=r(i,j)*(gp1(j)*(pm+p0)/(p(i,j)+p0)+gm1(j))
     *                   /(gm1(j)*(pm+p0)/(p(i,j)+p0)+gp1(j))
          wstar(2)=an(1)*vm-an(2)*vt
          wstar(3)=an(2)*vm+an(1)*vt
          wstar(4)=pm
        end if
      else
c rarefaction cases
        if (isgn*v(i,j)-c(i,j).ge.0.d0) then
c left (i=1) or right (i=2)
          wstar(1)=r(i,j)
          wstar(2)=an(1)*v(i,j)-an(2)*vt
          wstar(3)=an(2)*v(i,j)+an(1)*vt
          wstar(4)=p(i,j)
        else
c left middle or sonic (i=1) or right middle or sonic (i=2)
          rm=r(i,j)*((pm+p0)/(p(i,j)+p0))**(1.d0/gam(j))
          if (isgn*vm-dsqrt(gam(j)*(pm+p0)/rm).gt.0.d0) then
c sonic left (i=1) or right (i=2)
            arg=(2.d0+isgn*gm1(j)*v(i,j)/c(i,j))/gp1(j)
            vn=c(i,j)*arg*isgn
            wstar(1)=r(i,j)*arg**(2.d0/gm1(j))
            wstar(2)=an(1)*vn-an(2)*vt
            wstar(3)=an(2)*vn+an(1)*vt
            wstar(4)=(p(i,j)+p0)*arg**(2.d0*gam(j)/gm1(j))-p0
          else
c middle left (i=1) or right (i=2)
            wstar(1)=rm
            wstar(2)=an(1)*vm-an(2)*vt
            wstar(3)=an(2)*vm+an(1)*vt
            wstar(4)=pm
          end if
        end if
      end if
c
c compute flux
      q2=wstar(2)**2+wstar(3)**2
      vn=an(1)*wstar(2)+an(2)*wstar(3)
      if (j.eq.1) then
        energy=(wstar(4)+gam(1)*ps0)/gm1(1)
     *          +wstar(1)*(compac(alfa,0)+.5d0*q2)
      else
        energy=wstar(4)/gm1(2)+.5d0*wstar(1)*q2
      end if
c
      fact1=alfa*aj
      fact2=wstar(1)*vn
      fx(1)=fact1*fact2
      fx(2)=fact1*(fact2*wstar(2)+an(1)*wstar(4))
      fx(3)=fact1*(fact2*wstar(3)+an(2)*wstar(4))
      fx(4)=fact1*(energy+wstar(4))*vn
c
      return
      end
c
c+++++++++++++++++++++++++++++++++++++++++++++++++++++++
c
      subroutine getfx2 (i,alfa,pm,vm,vt,an,aj,wstar,fx)
c
c compute gas flux for the case when solid contact and gas contact
c are on either side of x=0.  If i=1, then solid contact is to the
c right of x=0, and if i=2, then solid contact is to the left.
c
      implicit double precision (a-h,o-z)
      dimension pm(2),an(2),wstar(4),fx(4)
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0,bgas
      common / prmdat / alpha(2,2),r(2,2),v(2,2),p(2,2),
     *                  a(2,2),b(2,2),c(2,2)
c
c if i=1, then k=2, and if i=2, then k=1
      k=3-i
c
      if (pm(k).gt.p(k,2)) then
c shock middle left (k=1) or middle right (k=2)
        rm=r(k,2)*(gp1(2)*pm(k)/p(k,2)+gm1(2))
     *           /(gm1(2)*pm(k)/p(k,2)+gp1(2))
        wstar(1)=rm*(pm(i)/pm(k))**(1.d0/gam(2))
      else
c rarefaction middle left (k=1) or right (k=2)
        wstar(1)=r(k,2)*(pm(i)/p(k,2))**(1.d0/gam(2))
      end if
c
      wstar(2)=an(1)*vm-an(2)*vt
      wstar(3)=an(2)*vm+an(1)*vt
      wstar(4)=pm(i)
c
c compute flux
      q2=wstar(2)**2+wstar(3)**2
      vn=an(1)*wstar(2)+an(2)*wstar(3)
      energy=wstar(4)/gm1(2)+.5d0*wstar(1)*q2
c
      fact1=alfa*aj
      fact2=wstar(1)*vn
      fx(1)=fact1*fact2
      fx(2)=fact1*(fact2*wstar(2)+an(1)*wstar(4))
      fx(3)=fact1*(fact2*wstar(3)+an(2)*wstar(4))
      fx(4)=fact1*(energy+wstar(4))*vn
c
      return
      end
c
c++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
c
c   support subroutines for the coupled phase calculation
c             (unchanged from the 1d version)
c
c++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
c
      double precision function getdv (i,rm,cm)
c
c evaluate FL(rho) or FR(rho) for virial gas (essentially integrate
c C+ or C- characteristic equation numerically)
c
      implicit real*8 (a-h,o-z)
      parameter (nmax=20)
      dimension aint(2,nmax),err(nmax)
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0,bgas
      common / prmdat / alpha(2,2),r(2,2),v(2,2),p(2,2),
     *                  a(2,2),b(2,2),c(2,2)
      common / romdat / aint0(2),r0(2),c0(2)
      common / cmprom / atol, nrmax
c
      j=2
      facti=1.d0+bgas*r(i,j)
      factm=1.d0+bgas*rm
c
c Trapezoidal rule
      dr=rm-r0(i)
      aint(1,1)=.5d0*(cm/rm+c0(i)/r0(i))*dr
c
      if (nrmax.le.1) then
        getdv=aint(1,1)+aint0(i)
        return
      else
        ndmax=min(nrmax,nmax)
      end if
c
c Romberg integration
c      atol=1.d-10
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
          getdv=aint0(i)
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
      getdv=aint(2,n)+aint0(i)
c
      return
      end
c
c++++++++++++++++++++++++++++++++++++++++++++++++++++++
c
      subroutine getgv (rm0,rm,vm,pm,g,resid,iconf,ier)
c
      implicit double precision (a-h,o-z)
c      parameter (nmax=20)
c      dimension aint(2,nmax)
      dimension ii(2,2)
      dimension rm(2),vm(2,2),pm(2,2),g(4),dv(2)
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0,bgas
      common / prmdat / alpha(2,2),r(2,2),v(2,2),p(2,2),
     *                  a(2,2),b(2,2),c(2,2)
      common / scalev / rm0save,rscale,pscale,vscale,gfact(4)
      common / romdat / aint0(2),r0(2),c0(2)
c
      ier=0
c
c compute solid velocities across acoustic fields
      j=1
      do i=1,2
        isgn=2*i-3
        if (pm(i,j).gt.p(i,j)) then
          arg=pm(i,j)+b(i,j)
          fact=dsqrt(a(i,j)/arg)
          vm(i,j)=v(i,j)+isgn*fact*(pm(i,j)-p(i,j))
          ii(i,j)=1
        else
          arg=(pm(i,j)+ps0)/(p(i,j)+ps0)
          fact=2.d0*c(i,j)/gm1(j)
          vm(i,j)=v(i,j)+isgn*fact*(arg**em(j)-1.d0)
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
          dv(i)=getdv(i,rm(i),cm)
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
c++++++++++++++++++++++++++++++++++++++++++++++++++
c
      subroutine newtonv (rm0,rm,vm,pm,g,iconf,ier)
c
      implicit double precision (a-h,o-z)
      dimension rm(2),vm(2,2),pm(2,2),g(4),dg(4,4),dg0(4,4)
c      logical ctest
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0,bgas
      common / prmdat / alpha(2,2),r(2,2),v(2,2),p(2,2),
     *                  a(2,2),b(2,2),c(2,2)
      common / scalev / rm0save,rscale,pscale,vscale,gfact(4)
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
c        call getdgfv (rm0,rm,vm,pm,g,dg,iconf,ier)
c analytic
        call getdgav (rm0,rm,vm,pm,dg,iconf,ier)
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
        call solve (dg,g,ier)
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
          call getgv (rm0,rm,vm,pm,g,resid,iconf,ier)
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
c+++++++++++++++++++++++++++++++++++++++++++++++++++++
c
      subroutine getdgfv (rm0,rm,vm,pm,g,dg,iconf,ier)
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
        call getgv (rm0p,rmp,vmp,pmp,gp,resid,iconf,ier)
        if (ier.ne.0) return
        do i=1,5
          dg(i,k)=(gp(i)-g(i))/(rm(k)*delta)
        end do
        rmp(k)=rm(k)
      end do
c
      do k=1,2
        pmp(k,1)=pm(k,1)*(1.d0+delta)
        call getgv (rm0p,rmp,vmp,pmp,gp,resid,iconf,ier)
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
c+++++++++++++++++++++++++++++++++++++++++++++++++++
c
      subroutine getdgav (rm0,rm,vm,pm,dg,iconf,ier)
c
      implicit double precision (a-h,o-z)
      dimension rm(2),vm(2,2),pm(2,2),dg(4,4)
      dimension fp(2,2),gp(2),dv(2)
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0,bgas
      common / prmdat / alpha(2,2),r(2,2),v(2,2),p(2,2),
     *                  a(2,2),b(2,2),c(2,2)
      common / scalev / rm0save,rscale,pscale,vscale,gfact(4)
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
          arg=(pm(i,j)+ps0)/(p(i,j)+ps0)
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
            write(6,*)'Error (getdgav) : cm2.le.0'
            ier=102
            return
          end if
          cm=dsqrt(cm2)
          dv(i)=getdv (i,rm(i),cm)
          gp(i)=cm2
          fp(i,j)=cm/rm(i)
        else
          z=r(i,j)/rm(i)
          z2=1.d0-z
          z3=.5d0*gm1(j)*facti*z2
          ratio=facti/factm
          denom=ratio*z-z3
          if (denom.le.1d-14) then
            write(6,*)'Error (getdgav) : denom.le.0'
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
c++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
c
      subroutine newton (pm,vm,ctest)
c
      implicit double precision (a-h,o-z)
      dimension pm(2,2),vm(2,2),rm(2),g(4),dg(4,4)
      logical ctest
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0,bgas
      common / prmdat / alpha(2,2),r(2,2),v(2,2),p(2,2),
     *                  a(2,2),b(2,2),c(2,2)
      common / scales / pscale,vscale,gfact(4)
      data pfact, tol, itmax / 1.d-4, 1.d-4, 15 /
c
c solid pressure is used to compute source
      source0=pm(2,1)*alpha(2,1)-pm(1,1)*alpha(1,1)
c
c Newton iteration to find pm
      it=0
    1 it=it+1
c
c get right-hand-side vector
        call getg (pm,vm,rm,g,resid)
c
c get Jacobian matrix (analytic)
        call getdga (dg,pm,vm,rm)
c
c get Jacobian matrix (finite difference)
c        call getdgf (pm,dg,g)
c
c compute correction, overwrite
        call solve (dg,g,ier)
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
        err1=dabs(source-source0)/min(pm(1,1)+ps0,pm(2,1)+ps0)
c
c relative error in the solid pressures
        err2=max(dabs(g(1))/(pm(1,1)+ps0),dabs(g(2))/(pm(2,1)+ps0))
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
      call soltest (pm,vm,ctest)
c
      return
      end
c
c++++++++++++++++++++++++++++++++
c
      subroutine solve (dg,g,ier)
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
c++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
c
      subroutine getguess (m,alpha,pm,pm0,vm,alphi,alphp,admin)
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
        call newton (pm,vm,ctest)
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
c++++++++++++++++++++++++++++++++++++++
c
      subroutine soltest1 (pm,vm,ctest)
c
      implicit double precision (a-h,o-z)
      dimension pm(2,2),vm(2,2)
      logical ctest
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0,bgas
      common / prmdat / alpha(2,2),r(2,2),v(2,2),p(2,2),
     *                  a(2,2),b(2,2),c(2,2)
c
      return
      end
c
c+++++++++++++++++++++++++++++++++++++
c
      subroutine soltest (pm,vm,ctest)
c
      implicit double precision (a-h,o-z)
      dimension pm(2,2),vm(2,2),sp(3,2)
      logical ctest
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0,bgas
      common / prmdat / alpha(2,2),r(2,2),v(2,2),p(2,2),
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
        sp(1,j)=v(1,j)-c(1,j)*dsqrt(ep(j)*(pm(1,j)+ps0)
     *                                    /(p(1,j)+ps0)+em(j))
        if (sp(1,j)-tiny.gt.v(1,j)-c(1,j)) then
          ctest=.false.
          return
        end if
      else
        rm=r(1,j)*((pm(1,j)+ps0)/(p(1,j)+ps0))**(1.d0/gam(j))
        sp(1,j)=vm(1,j)-dsqrt(gam(j)*(pm(1,j)+ps0)/rm)
        if (sp(1,j)+tiny.lt.v(1,j)-c(1,j)) then
          ctest=.false.
          return
        end if
      end if
      if (pm(2,j).gt.p(2,j)) then
        sp(3,j)=v(2,j)+c(2,j)*dsqrt(ep(j)*(pm(2,j)+ps0)
     *                                    /(p(2,j)+ps0)+em(j))
        if (sp(3,j)+tiny.lt.v(2,j)+c(2,j)) then
          ctest=.false.
          return
        end if
      else
        rm=r(2,j)*((pm(2,j)+ps0)/(p(2,j)+ps0))**(1.d0/gam(j))
        sp(3,j)=vm(2,j)+dsqrt(gam(j)*(pm(2,j)+ps0)/rm)
        if (sp(3,j)-tiny.gt.v(2,j)+c(2,j)) then
          ctest=.false.
          return
        end if
      end if
c
      j=2
      if (pm(1,j).gt.p(1,j)) then
        sp(1,j)=v(1,j)-c(1,j)*dsqrt(ep(j)*pm(1,j)/p(1,j)+em(j))
        if (sp(1,j)-tiny.gt.v(1,j)-c(1,j)) then
          ctest=.false.
          return
        end if
      else
        rm=r(1,j)*(pm(1,j)/p(1,j))**(1.d0/gam(j))
        sp(1,j)=vm(1,j)-dsqrt(gam(j)*pm(1,j)/rm)
        if (sp(1,j)+tiny.lt.v(1,j)-c(1,j)) then
          ctest=.false.
          return
        end if
      end if
      if (pm(2,j).gt.p(2,j)) then
        sp(3,j)=v(2,j)+c(2,j)*dsqrt(ep(j)*pm(2,j)/p(2,j)+em(j))
        if (sp(3,j)+tiny.lt.v(2,j)+c(2,j)) then
          ctest=.false.
          return
        end if
      else
        rm=r(2,j)*(pm(2,j)/p(2,j))**(1.d0/gam(j))
        sp(3,j)=vm(2,j)+dsqrt(gam(j)*pm(2,j)/rm)
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
c+++++++++++++++++++++++++++++++++++++++
c
      subroutine getg (pm,vm,rm,g,resid)
c
      implicit double precision (a-h,o-z)
      dimension pm(2,2),vm(2,2),rm(2),g(4)
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0,bgas
      common / prmdat / alpha(2,2),r(2,2),v(2,2),p(2,2),
     *                  a(2,2),b(2,2),c(2,2)
      common / scales / pscale,vscale,gfact(4)
c
c compute velocities across acoustic fields
      j=1
      do i=1,2
        isgn=2*i-3
        if (pm(i,j).gt.p(i,j)) then
          arg=pm(i,j)+b(i,j)
          fact=dsqrt(a(i,j)/arg)
          vm(i,j)=v(i,j)+isgn*fact*(pm(i,j)-p(i,j))
        else
          arg=(pm(i,j)+ps0)/(p(i,j)+ps0)
          fact=2.d0*c(i,j)/gm1(j)
          vm(i,j)=v(i,j)+isgn*fact*(arg**em(j)-1.d0)
        end if
      end do
c
      j=2
      do i=1,2
        isgn=2*i-3
        if (pm(i,j).gt.p(i,j)) then
          arg=pm(i,j)+b(i,j)
          fact=dsqrt(a(i,j)/arg)
          vm(i,j)=v(i,j)+isgn*fact*(pm(i,j)-p(i,j))
        else
          arg=pm(i,j)/p(i,j)
          fact=2.d0*c(i,j)/gm1(j)
          vm(i,j)=v(i,j)+isgn*fact*(arg**em(j)-1.d0)
        end if
      end do
c
c check whether gas contact is to the left or
c to the right of solid contact, compute gas densities
      if (vm(1,1).gt.vm(1,2)) then
c       if (vm(2,1).gt.vm(2,2)) then
c gas contact on the left
          if (pm(2,2).gt.p(2,2)) then
            rm(2)=r(2,2)*(gp1(2)*pm(2,2)/p(2,2)+gm1(2))
     *                  /(gm1(2)*pm(2,2)/p(2,2)+gp1(2))
          else
            rm(2)=r(2,2)*(pm(2,2)/p(2,2))**(1.d0/gam(2))
          end if
          rm(1)=rm(2)*(pm(1,2)/pm(2,2))**(1.d0/gam(2))
c       else
c         write(6,*)'Error (getg) : inconsistent velocities'
c         stop
c       end if
      else
c       if (vm(2,1).le.vm(2,2)) then
c gas contact on the right
          if (pm(1,2).gt.p(1,2)) then
            rm(1)=r(1,2)*(gp1(2)*pm(1,2)/p(1,2)+gm1(2))
     *                  /(gm1(2)*pm(1,2)/p(1,2)+gp1(2))
          else
            rm(1)=r(1,2)*(pm(1,2)/p(1,2))**(1.d0/gam(2))
          end if
          rm(2)=rm(1)*(pm(2,2)/pm(1,2))**(1.d0/gam(2))
c       else
c         write(6,*)'Error (getg) : inconsistent velocities'
c         stop
c       end if
      end if
c
c compute constraints across solid particle path
      v1=vm(1,2)-vm(1,1)
      v2=vm(2,2)-vm(2,1)
      h1=gam(2)*pm(1,2)/(gm1(2)*rm(1))
      h2=gam(2)*pm(2,2)/(gm1(2)*rm(2))
      g(1)=vm(2,1)-vm(1,1)
      g(2)= alpha(2,2)*(pm(2,2)**(1.d0/gam(2)))*v2
     *     -alpha(1,2)*(pm(1,2)**(1.d0/gam(2)))*v1
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
c++++++++++++++++++++++++++++++++++++
c
      subroutine getdga (dg,pm,vm,rm)
c
      implicit double precision (a-h,o-z)
      dimension dg(4,4),pm(2,2),vm(2,2),rm(2)
      dimension fp(2,2),gk(2),gp(2),drdp(2,2)
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0,bgas
      common / prmdat / alpha(2,2),r(2,2),v(2,2),p(2,2),
     *                  a(2,2),b(2,2),c(2,2)
      common / scales / pscale,vscale,gfact(4)
c
      j=1
      do i=1,2
        if (pm(i,j).gt.p(i,j)) then
          fp(i,j)=(gp1(j)*(pm(i,j)+ps0)+(3.d0*gam(j)-1.d0)*
     *             (p(i,j)+ps0))/dsqrt(2.d0*r(i,j)*(gp1(j)*
     *             (pm(i,j)+ps0)+gm1(j)*(p(i,j)+ps0))**3)
        else
          fp(i,j)=(((pm(i,j)+ps0)/(p(i,j)+ps0))**(-ep(j)))/
     *                                (r(i,j)*c(i,j))
        end if
      end do
c
      j=2
      do i=1,2
        if (pm(i,j).gt.p(i,j)) then
          fp(i,j)=(gp1(j)*pm(i,j)+(3.d0*gam(j)-1.d0)*p(i,j))
     *             /dsqrt(2.d0*r(i,j)*(gp1(j)*pm(i,j)
     *            +gm1(j)*p(i,j))**3)
          gk(i)=r(i,2)*(gp1(2)*pm(i,2)+gm1(2)*p(i,2))/
     *                 (gm1(2)*pm(i,2)+gp1(2)*p(i,2))
          gp(i)=4.0d0*gam(2)*r(i,2)*p(i,2)/
     *                ((gm1(2)*pm(i,2)+gp1(2)*p(i,2))**2)
        else
          fp(i,j)=((pm(i,j)/p(i,j))**(-ep(j)))/(r(i,j)*c(i,j))
          gk(i)=r(i,2)*(pm(i,2)/p(i,2))**(1/gam(2))
          gp(i)=gk(i)/(gam(2)*pm(i,2))
        end if
      end do
c
      gi=1.d0/gam(2)
      rat=(pm(1,2)/pm(2,2))**gi
      if (vm(1,1).gt.vm(1,2)) then
        drdp(1,1)=gk(2)*gi/pm(1,2)*rat
        drdp(1,2)=(gp(2)-gk(2)*gi/pm(2,2))*rat
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
      dg(4,3)=-gam(2)*pm(2,2)/gm1(2)/(rm(2)**2)*drdp(2,1)+dv1*fp(1,2)-
     *                gam(2)*(rm(1)-pm(1,2)*drdp(1,1))/gm1(2)/(rm(1)**2)
      dg(4,4)=gam(2)*pm(1,2)/gm1(2)/(rm(1)**2)*drdp(1,2)+dv2*fp(2,2)+
     *                gam(2)*(rm(2)-pm(2,2)*drdp(2,2))/gm1(2)/(rm(2)**2)
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
c+++++++++++++++++++++++++++++++++
c
      subroutine getdgf (pm,dg,g0)
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
          call getg (pm,vm,rm,g,resid)
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
c++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
c
c  end of support subroutines for the coupled phase calculation
c
c++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
c
      subroutine maxspd (rad,pm,speed)
c
c compute maximum wave speed
c
      implicit double precision (a-h,o-z)
      dimension pm(2,2)
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0,bgas
      common / prmdat / alpha(2,2),r(2,2),v(2,2),p(2,2),
     *                  a(2,2),b(2,2),c(2,2)
c
      j=1
      do i=1,2
        isgn=3-2*i
        if (pm(i,j).gt.p(i,j)) then
          sp=isgn*v(i,j)-c(i,j)*dsqrt(ep(j)*(pm(i,j)+ps0)
     *                                       /(p(i,j)+ps0)+em(j))
        else
          sp=isgn*v(i,j)-c(i,j)
        end if
        speed=max(rad*dabs(sp),speed)
      end do
c
      j=2
      do i=1,2
        isgn=3-2*i
        if (pm(i,j).gt.p(i,j)) then
          sp=isgn*v(i,j)-c(i,j)*dsqrt(ep(j)*pm(i,j)/p(i,j)+em(j))
        else
          sp=isgn*v(i,j)-c(i,j)
        end if
        speed=max(rad*dabs(sp),speed)
      end do
c
      return
      end
c
c++++++++++++++++++++++++++++++++++++++
c
      subroutine maxspdv (rad,speed)
c
c compute maximum wave speed
c
      implicit double precision (a-h,o-z)
      common / prmdat / alpha(2,2),r(2,2),v(2,2),p(2,2),
     *                  a(2,2),b(2,2),c(2,2)
c
      j=1
      do i=1,2
        isgn=3-2*i
        do j=1,2
          sp=isgn*v(i,j)-c(i,j)
          speed=max(rad*dabs(sp),speed)
        end do
      end do
c
      return
      end
