      subroutine cmpdu (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                  dr,ds1,ds2,r,rx,gv,det,rx2,gv2,det2,xy,
     *                  u,up,mask,ntau,tau,ad,mdat,dat,nrprm,rparam,
     *                  niprm,iparam,nrwk,rwk,niwk,iwk,idebug,ier)
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
      common / junk / ijunk(9,-5:300)
c
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0
      common / srcprm / delta,rmuc,htrans,cratio,abmin,abmax,isrc
c
      common / srcdat / nb1a,nb1b,nb2a,nb2b,icount
      common / timing / tflux,tslope,tsource
      common / axidat / iaxi,j1axi(2),j2axi(2)
c
c     data ic / 9,1,2,4,5,6,8 /
      data ic / 9,1,3,4,5,7,8 /
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
      if (m.lt.0) then
        call chkp (m,nd1a,nd1b,md1a,md1b,nd2a,nd2b,md2a,md2b,u,rwk)
      end if
c
      if (m.lt.0) then
      write(55,434)dr,ds1,ds2
  434 format('dr,ds1,ds2=',3(1x,1pe10.3),/,
     *       'cmpdu start, u=')
c     do j1=n1a,n1b
c       write(55,234)j1,(u(j1,n2a,ic(k)),k=1,7)
c 234   format(1x,i1,7(/,1x,1pe15.8))
c     end do
      do j2=n2a,n2b
        write(55,234)j2,(u(n1a,j2,ic(k)),k=1,7)
  234   format(1x,i1,7(/,1x,1pe15.8))
      end do
      write(6,*)dr
      pause
      if (m.lt.0) stop
      end if
c
c..set error flag
      ier=0
c
c..sanity check
      if (m.ne.9) then
        write(6,*)'Error (cmpdu) : m=9 is assumed'
        stop
      end if
c
      if (m.lt.0) then
       write(6,*)'cmpdu(in)'
       write(6,'(9(1x,i1,1x,f15.8,/))')(i,u(0,0,i),i=1,9)
       write(6,*)gam,gm1
       pause
      end if
c
c     if( idebug.gt.0 ) write(6,990)r,dr
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
      tflux=0.d0
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
      if (nstep.lt.0) ioder=1
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
                write(6,*)'Error (dudr) : inconsistent mask value'
              end if
            end do
            end do
          end if
        end do
        end do
      end if
c
      if (m.gt.0) then
        call chks (m,nd1a,nd1b,md1a,md1b,nd2a,nd2b,md2a,md2b,
     *             u,mask,rwk,ier,1)
        if (ier.ne.0) stop
      end if
c
c..monitor data (if idebug>0)
c      if (idebug.gt.0) then
c        call mondat2d (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
c     *                 r,dr,u,rwk,mask)
c      end if
c
c..limit alphaSolid and check for negative densities
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
      if (m.gt.0) then
        call chks (m,nd1a,nd1b,md1a,md1b,nd2a,nd2b,md2a,md2b,
     *             u,mask,rwk,ier,2)
        if (ier.ne.0) stop
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
          call prts (m,nd1a,nd1b,md1a,md1b,nd2a,nd2b,md2a,md2b,
     *               u,rwk,60)
        end if
      end if
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
      if (m.lt.0.and.r.gt.1.482d0) then
        call prts (m,nd1a,nd1b,md1a,md1b,nd2a,nd2b,md2a,md2b,
     *             up,rwk,58)
      end if
c
c hydro step (handles slope correction, flux, and free-stream correction)
      call cmphydro (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,md2a,md2b,
     *               n2a,n2b,dr,ds,r,rx,gv,det,rx2,gv2,det2,xy,up,
     *               mask,almax,rwk(lw),rwk(lw1),rwk(la0),rwk(la1),
     *               rwk(laj),rwk(lda0),rwk(lvaxi),mdat,dat,move,
     *               maxnstep,icart,iorder,method,n1bm,n2bm,ier)
c
      if (m.lt.0.and.r.gt.1.482d0) then
        call prts (m,nd1a,nd1b,md1a,md1b,nd2a,nd2b,md2a,md2b,
     *             up,rwk,59)
      end if
c
      if (ier.ne.0) return
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
          write(6,*)'r =',r
c          call prts (m,nd1a,nd1b,md1a,md1b,nd2a,nd2b,md2a,md2b,
c     *               u,rwk,61)
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
      if (m.lt.0.and.nstep.ge.0) then
        write(55,453)r
  453   format('** t=',f12.4)
        do j1=n1a,n1b
          write(55,678)j1,up(j1,n2a,1),up(j1,n2a,9),(ijunk(i,j1),i=1,9)
  678     format(1x,i3,2(1x,1pe22.15),9(1x,i2))
        end do
      end if
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
          call prts (m,nd1a,nd1b,md1a,md1b,nd2a,nd2b,md2a,md2b,
     *               u,rwk,62)
          stop
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
      rparam(31)=tflux
      rparam(32)=tslope
      rparam(33)=tsource
c
      if (idebug.gt.0) then
        write(6,105)rparam(1),rparam(2),maxnstep
      end if
  105 format('max(lambda) =',2(1pe9.2,1x),',  max(RxnSteps) =',i6)
c
      if( idebug.gt.0 ) write(6,*)'...done: cmpdu'
c     write(6,*)'...done: cmpdu'
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
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0
      data c2min / 1.d-6 /
c
      ier=0
      do j2=md2a,md2b
        call cmppvs (m,nd1a,nd1b,md1a,md1b,nd2a,nd2b,j2,u,w)
        do j1=md1a,md1b
          if (mask(j1,j2).ne.0) then
            rb=w(1,j1)
            gpb=gam(1)*(w(4,j1)+ps0)
            cb2=gpb/rb
            if (cb2.lt.c2min) then
              ier=1
              goto 1
            end if
            r=w(5,j1)
            gp=gam(2)*w(8,j1)
            c2=gp/r
            if (c2.lt.c2min) then
              ier=2
              goto 1
            end if
          end if
        end do
      end do
c
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
      ltk=1
      lhk=ltk+nd
      luk=lhk+nd
      lqk=luk+nd*m
      lwk=lqk+nd*4
      lupk=lwk+nd*m
      luk1=lupk+nd*15
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
     *                rwk(luk1),rwk(ltest),iwk(lnstep),maxstep,ier)
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
c
c     write(6,*)'cmpsrc1'
c
      do j2=n2a,n2b
      do j1=n1a,n1b
        if (mask(j1,j2).ne.0) then
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
     *                    test,nstep,maxstep,ier)
c
c source step: compaction, heat transfer, chemical reaction
c
      implicit real*8 (a-h,o-z)
      dimension u(nd1a:nd1b,nd2a:nd2b,m),mask(nd1a:nd1b),
     *          tau(nd1a:nd1b),jk(nd),tk(nd),hk(nd),uk(nd,m),
     *          qk(nd,4),wk(m,nd),upk(5,nd,3),uk1(nd,m),test(nd),
     *          nstep(nd),scale(5)
      common / cmpsrc / tol,qmax,qmin,itmax
      data abmin1, tiny / 1.d-3, 1.d-14 /
c
c     write(6,*)'cmpsrc2'
c
      ier=0
      maxstep=0
      hmin=.5d0*dt/itmax
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
        call cmprate (m,nd,n,uk,wk,upk(1,1,1))
c
        do k=1,n
          do i=1,4
            upk(i,k,1)=hk(k)*upk(i,k,1)
            uk1(k,i  )=uk(k,i)+.5d0*upk(i,k,1)
            uk1(k,i+4)=qk(k,i)-uk1(k,i)
          end do
          upk(5,k,1)=hk(k)*upk(5,k,1)
          uk1(k,9)=uk(k,9)+.5d0*upk(5,k,1)
        end do
c
        call cmprate (m,nd,n,uk1,wk,upk(1,1,2))
c
        do k=1,n
          do i=1,4
            upk(i,k,2)=hk(k)*upk(i,k,2)
            uk1(k,i  )=uk(k,i)+.75d0*upk(i,k,2)
            uk1(k,i+4)=qk(k,i)-uk1(k,i)
          end do
          upk(5,k,2)=hk(k)*upk(5,k,2)
          uk1(k,9)=uk(k,9)+.75d0*upk(5,k,2)
        end do
c
        call cmprate (m,nd,n,uk1,wk,upk(1,1,3))
c
        do k=1,n
          j1=jk(k)
          test(k)=0.d0
          scale(1)=qk(k,1)
          scale(2)=qk(k,1)*wk(2,k)
          scale(3)=qk(k,1)*wk(3,k)
          scale(4)=qk(k,4)
          scale(5)=1.d0
          do i=1,5
            upk(i,k,3)=hk(k)*upk(i,k,3)
            trunc=dabs(2*upk(i,k,1)-6*upk(i,k,2)+4*upk(i,k,3))/scale(i)
            test(k)=max(trunc,test(k))
          end do
          tau(j1)=max(test(k),tau(j1))
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
              do i=1,4
                uk(k,i)=uk(k,i)
     *                  +(2*upk(i,k,1)+3*upk(i,k,2)+4*upk(i,k,3))/9.d0
                uk(k,i+4)=qk(k,i)-uk(k,i)
              end do
              uk(k,9)=uk(k,9)
     *                +(2*upk(5,k,1)+3*upk(5,k,2)+4*upk(5,k,3))/9.d0
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
            if (it.lt.itmax) then
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
            do i=1,4
              u(j1,j2,i)=uk(k,i)
     *                   +(2*upk(i,k,1)+3*upk(i,k,2)+4*upk(i,k,3))/9.d0
              u(j1,j2,i+4)=qk(k,i)-u(j1,j2,i)
            end do
            u(j1,j2,9)=uk(k,9)
     *                 +(2*upk(5,k,1)+3*upk(5,k,2)+4*upk(5,k,3))/9.d0
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
      subroutine cmprate (m,nd,n,uk,wk,upk)
c
      implicit real*8 (a-h,o-z)
      dimension uk(nd,m),wk(m,nd),upk(5,nd)
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0
      common / srcprm / delta,rmuc,htrans,cratio,abmin,abmax,isrc
      common / cmprxn / sigma,pgi,anu
      data c2min / 1.d-12 /
c
c     rmuc=.5d0
c     htrans=0.d0
c     sigma=0.d0
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
        h=htrans*(wk(8,k)/(gm1(2)*wk(5,k))
     *    -cratio*(wk(4,k)+ps0)/(gm1(1)*wk(1,k)))
c
c chemical reaction
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
c save acoustic speeds (for later use in the scales for the
c truncation error estimate)
        c2=gam(2)*wk(8,k)/wk(5,k)
        cb2=gam(1)*(wk(4,k)+ps0)/wk(1,k)
        c=dsqrt(max(c2,c2min))
        cb=dsqrt(max(cb2,c2min))
        wk(2,k)=max(dabs(wk(2,k))+cb,dabs(wk(6,k))+c)
        wk(3,k)=max(dabs(wk(3,k))+cb,dabs(wk(7,k))+c)
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
      dimension rx(md1a:n1bm,md2a:n2bm,2,2),u(nd1a:nd1b,nd2a:nd2b,m),
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
            div(j1,2)= (vxp-vxm)/(2*ds(1))+(vyp-vym)/(2*ds(2))
            if (mask(j1,j2).ne.0) then
              vismax=max(-div(j1,2),vismax)
            end if
          end do
        else
          a11=rx(nd1a,nd2a,1,1)/(2*ds(1))
          a22=rx(nd1a,nd2a,2,2)/(2*ds(2))
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
              vis=av*max(0.d0,-(div(j1,1)+div(j1,2))/2)
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
                vis=av*max(0.d0,-(div(j1,2)+div(j1p1,2))/2)
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
            div(j1,2)= (vxp-vxm)/(2*ds(1))+(vyp-vym)/(2*ds(2))
            if (mask(j1,j2).ne.0) then
              vismax=max(-div(j1,2),vismax)
            end if
          end do
        else
          a11=rx(nd1a,nd2a,1,1)/(2*ds(1))
          a22=rx(nd1a,nd2a,2,2)/(2*ds(2))
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
              vis=av*max(0.d0,-(div(j1,1)+div(j1,2))/2)
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
                vis=av*max(0.d0,-(div(j1,2)+div(j1p1,2))/2)
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
c volume fraction
      i=9
      do j2=n2a,n2b+1
        j2m1=j2-1
        do j1=n1a,n1b
          if (mask(j1,j2).ne.0.and.mask(j1,j2m1).ne.0) then
            fx(i)=ad(i)*(u(j1,j2,i)-u(j1,j2m1,i))
            up(j1,j2  ,i)=up(j1,j2  ,i)-fx(i)
            up(j1,j2m1,i)=up(j1,j2m1,i)+fx(i)
          end if
        end do
        if (j2.le.n2b) then
          do j1=n1a-1,n1b
            j1p1=j1+1
            if (mask(j1,j2).ne.0.and.mask(j1p1,j2).ne.0) then
              fx(i)=ad(i)*(u(j1p1,j2,i)-u(j1,j2,i))
              up(j1p1,j2,i)=up(j1p1,j2,i)-fx(i)
              up(j1  ,j2,i)=up(j1  ,j2,i)+fx(i)
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
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0
c
c..timings
      common / timing / tflux,tslope,tsource
c
      common / test / iflg
c
c     data ic / 9,1,2,4,5,6,8 /
      data ic / 9,1,3,4,5,7,8 /
c
      iflg=0
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
      call second (time0)
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
      call second (time1)
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
        call second (time0)
c
        if (j2.eq.n2a) iflg=1
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
        call second (time1)
        tslope=tslope+time1-time0
c
c..compute s2 flux along j2-1/2, add it to up(j1,j2,.) and up(j1,j2-1,.)
        do j1=n1a,n1b
          if (mask(j1,j2).ne.0.and.mask(j1,j2m1).ne.0) then
            aj0=(aj(j1,2)+aj(j1,1))/2.d0
            a20=(a0(2,j1,2)+a0(2,j1,1))/2.d0
            a21=(a1(2,1,j1,2)+a1(2,1,j1,1))/2.d0
            a22=(a1(2,2,j1,2)+a1(2,2,j1,1))/2.d0
            if (j1.eq.n1a.and.m.lt.0) then
            write(55,444)j2,aj0,a20,a21,a22,
     *                   (w1(ic(k),j1,4,1),w1(ic(k),j1,3,2),k=1,7)
            end if
            call cmpflux (m,aj0,a20,a21,a22,w1(1,j1,4,1),
     *                    w1(1,j1,3,2),fl,fr,almax(2),method)
            if (j1.eq.n1a.and.m.lt.0) then
            write(55,445)j2,dtds2,(fl(ic(k)),fr(ic(k)),
     *                   w1(ic(k),j1,4,1),w1(ic(k),j1,3,2),k=1,7)
            end if
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
        call second (time2)
        tflux=tflux+time2-time1
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
            aint= (a1(1,1,j1,1)*(w1(2,j1,1,1)+w1(2,j1,2,1))
     *            +a1(1,2,j1,1)*(w1(3,j1,1,1)+w1(3,j1,2,1)))*da1
     *           +(a1(2,1,j1,1)*(w1(2,j1,3,1)+w1(2,j1,4,1))
     *            +a1(2,2,j1,1)*(w1(3,j1,3,1)+w1(3,j1,4,1)))*da2
            up(j1,j2m1,9)=up(j1,j2m1,9)-aint
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
          call second (time0)
c
c..compute s1 flux along j2, add it to up(j1+1,j2,.) and up(j1,j2,.)
          do j1=n1a-1,n1b
            j1p1=j1+1
            if (mask(j1,j2).ne.0.and.mask(j1p1,j2).ne.0) then
              aj0=(aj(j1p1,1)+aj(j1,1))/2.d0
              a10=(a0(1,j1p1,1)+a0(1,j1,1))/2.d0
              a11=(a1(1,1,j1p1,1)+a1(1,1,j1,1))/2.d0
              a12=(a1(1,2,j1p1,1)+a1(1,2,j1,1))/2.d0
              if (j2.eq.n2a.and.m.lt.0) then
              write(55,444)j1,aj0,a10,a11,a12,
     *                     (w1(ic(k),j1,2,1),w1(ic(k),j1p1,1,1),k=1,7)
  444         format(1x,i2,4(1x,f7.2),7(/,2(1x,1pe15.8)))
              end if
c             if (j1.eq.100.and.j2.eq.n2a) then
c               write(6,*)'flux'
c             end if
              do i=1,m
                wsavel(i)=w1(i,j1,2,1)
                wsaver(i)=w1(i,j1p1,1,1)
              end do
              call cmpflux (m,aj0,a10,a11,a12,w1(1,j1,2,1),
     *                      w1(1,j1p1,1,1),fl,fr,almax(1),method)
              if (m.lt.0)then
              if (r.gt.1.482d0.and.j1.eq.21.and.j2.eq.2) then
                write(57,321)nd1a,nd1b,nd2a,nd2b,j1,j2,
     *                       (wsavel(i),i=1,9),(wsaver(i),i=1,9),
     *                       (fl(i),i=1,9),(fr(i),i=1,9),
     *                       aj0,a10,a11,a12
  321           format(6(1x,i4),/,4(9(1x,1pe22.15),/),4(1x,1pe15.8))
              end if
              end if
              if (j2.eq.n2a.and.m.lt.0) then
              write(55,445)j1,dtds1,(fl(ic(k)),fr(ic(k)),
     *                     w1(ic(k),j1,2,1),w1(ic(k),j1p1,1,1),k=1,7)
  445         format(1x,i2,1x,1pe10.3,7(/,4(1x,1pe15.8)))
              end if
              do i=1,m
                up(j1p1,j2,i)=up(j1p1,j2,i)+dtds1*fr(i)/aj(j1p1,1)
                up(j1  ,j2,i)=up(j1  ,j2,i)-dtds1*fl(i)/aj(j1  ,1)
              end do
              if (up(j1,j2,1).lt.0.d0.or.up(j1p1,j2,1).lt.0.d0) then
                write(6,*)'oops:fl=',fl
                write(6,*)'oops:fr=',fr
                write(6,*)'wl=',wsavel
                write(6,*)'wr=',wsaver
                write(6,*)'as=',aj0,a10,a11,a12
                pause
              end if
            end if
          end do
          if (m.lt.0) stop
c
          call second (time1)
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
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0
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
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0

      common / junk / ijunk(9,-5:300)
      common / test / iflg

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
c     if (iflg.eq.1) then
c       write(6,*)'slope'
c     end if
c
c..compute slope contribution and add it to w1(i,j1,1:2,1:2)
      do j1=n1a-1,n1b+1

c       do i=1,9
c         ijunk(i,j1)=0
c       end do

c       if (iflg.eq.1.and.j1.eq.101) then
c         write(6,*)'here i am'
c         iflg=0
c       end if

        if (mask(j1).ne.0) then
c
          isolid=.true.
          if (w(9,j1,2).lt.amin) isolid=.false.
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
          gp=gam(2)*w(8,j1,2)
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
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0
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
            hs=gam(1)*(w1(4,j1)+ps0)/gm1(1)
     *         +.5d0*(rus*w1(2,j1)+rvs*w1(3,j1))
     *                    +w1(1,j1)*compac(as,0)
            hg=gam(2)*w1(8,j1)/gm1(2)
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
            hs=gam(1)*(w1(4,j1)+ps0)/gm1(1)
     *         +.5d0*(rus*w1(2,j1)+rvs*w1(3,j1))
     *                    +w1(1,j1)*compac(as,0)
            hg=gam(2)*w1(8,j1)/gm1(2)
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
      implicit real*8 (a-h,o-z)
      dimension wl(m),wr(m),fl(m),fr(m)
      dimension an(2),pm(2,2),vm(2,2),dv(2,2)
      common / prmdat / alpha(2,2),r(2,2),v(2,2),p(2,2),
     *                  a(2,2),b(2,2),c(2,2)
      common / cntdat / icnt(4)
c
c      do i=1,m
c        if (dabs(wl(i)-wr(i)).gt.1.d-13) then
c          write(6,*)'left and right states do not agree'
c          pause
c        end if
c      end do
c
      if (m.lt.0) then
      write(6,*)'cmpflux(in)'
      write(6,*)'aj,a0,a1,a2=',aj,a0,a1,a2
      write(6,123)(i,wl(i),i=1,9),(i,wr(i),i=1,9)
  123 format('wl=',/,9(1x,i1,1x,f15.8,/),
     *       'wr=',/,9(1x,i1,1x,f15.8,/))
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
c      an0=a0/rad            ! moving grids not implemented
      an(1)=a1/rad
      an(2)=a2/rad
c
c..compute decoupled middle state for each phase
      do j=1,2
        k=4*j-3
        call cmpmiddle (j,an,wl(k),wr(k),pm(1,j),vm(1,j),dv(1,j),ier)
        if (ier.ne.0) then
          write(6,100)ier,wl,wr
  100     format('Error (cmpflux) : unable to compute middle state, ',
     *           'ier=',i1,/,'wl=',9(1x,1pe15.8),/,'wr=',9(1x,1pe15.8))
          stop
        end if
      end do
c
c..compute coupled middle state and fluxes, or decoupled fluxes
      aj1=rad*aj
      call cmpcouple (m,an,aj1,pm,vm,dv,wl,wr,fl,fr,method,ier)
      if (ier.ne.0) then
        write(6,*)'Error (cmpflux) : unable to compute coupled state,',
     *            ' ier=',ier
        stop
      end if
c
c..compute maximum wave speed
      call maxspd (rad,pm,speed)
c
      if (m.lt.0) then
      write(6,*)'cmpflux(out)'
      write(6,*)'speed=',speed
      write(6,124)(i,fl(i),i=1,9),(i,fr(i),i=1,9)
  124 format('fl=',/,9(1x,i1,1x,f15.8,/),
     *       'fr=',/,9(1x,i1,1x,f15.8,/))
      pause
      end if
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
      implicit real*8 (a-h,o-z)
      dimension an(2),wl(4),wr(4),pm(2),vm(2),dv(2)
      dimension vdif(2,2)
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0
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
      implicit real*8 (a-h,o-z)
      dimension an(2),pm(2,2),vm(2,2),dv(2,2),wl(m),wr(m),fl(m),fr(m)
      dimension pm0(2,2),vm0(2,2),dummy(2,2)
c*wdh
      dimension g(4)
c*wdh
      logical ctest
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0
      common / prmdat / alpha(2,2),r(2,2),v(2,2),p(2,2),
     *                  a(2,2),b(2,2),c(2,2)
      common / scales / pscale,vscale,gfact(4)
      common / cntdat / icnt(4)
      data sig, frac, tol / 10.d0, .99d0, 1.d-10 /
      data abdif, abmin / 1.d-14, 1.d-3 /
      data pmin, c2min / 1.d-6, 1.d-12 /
c
      ier=0
c
c perform a coupled flux calculation only if the difference between
c the left and right values of alpha_bar is greater than a tolerance
c and if there is a minimum amount of solid left in both states
      if (dabs(wr(9)-wl(9)).gt.abdif.and.
     *     max(wl(9),wr(9)).gt.abmin) then
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
          call getfx2 (2,alpha(2,2),pm(1,2),vm(2,2),vt,
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
      implicit real*8 (a-h,o-z)
      dimension an(2),wstar(4),fx(4)
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0
      common / prmdat / alpha(2,2),r(2,2),v(2,2),p(2,2),
     *                  a(2,2),b(2,2),c(2,2)
c
c set isign=1 for i=1 and isign=-1 for i=2
      isign=3-2*i
c
      if (j.eq.1) then
        p0=ps0
      else
        p0=0.d0
      end if
c
      if (pm.gt.p(i,j)) then
c shock cases
        sp=isign*v(i,j)-c(i,j)*dsqrt(ep(j)*(pm+p0)/(p(i,j)+p0)+em(j))
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
        if (isign*v(i,j)-c(i,j).ge.0.d0) then
c left (i=1) or right (i=2)
          wstar(1)=r(i,j)
          wstar(2)=an(1)*v(i,j)-an(2)*vt
          wstar(3)=an(2)*v(i,j)+an(1)*vt
          wstar(4)=p(i,j)
        else
c left middle or sonic (i=1) or right middle or sonic (i=2)
          rm=r(i,j)*((pm+p0)/(p(i,j)+p0))**(1.d0/gam(j))
          if (isign*vm-dsqrt(gam(j)*(pm+p0)/rm).gt.0.d0) then
c sonic left (i=1) or right (i=2)
            arg=(2.d0+isign*gm1(j)*v(i,j)/c(i,j))/gp1(j)
            vn=c(i,j)*arg*isign
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
      implicit real*8 (a-h,o-z)
      dimension pm(2),an(2),wstar(4),fx(4)
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0
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
      subroutine newton (pm,vm,ctest)
c
      implicit real*8 (a-h,o-z)
      dimension pm(2,2),vm(2,2),rm(2),g(4),dg(4,4)
      logical ctest
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0
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
      implicit real*8 (a-h,o-z)
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
      implicit real*8 (a-h,o-z)
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
      implicit real*8 (a-h,o-z)
      dimension pm(2,2),vm(2,2)
      logical ctest
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0
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
      implicit real*8 (a-h,o-z)
      dimension pm(2,2),vm(2,2),sp(3,2)
      logical ctest
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0
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
      implicit real*8 (a-h,o-z)
      dimension pm(2,2),vm(2,2),rm(2),g(4)
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0
      common / prmdat / alpha(2,2),r(2,2),v(2,2),p(2,2),
     *                  a(2,2),b(2,2),c(2,2)
      common / scales / pscale,vscale,gfact(4)
c
c compute velocities across acoustic fields
      j=1
      do i=1,2
        isign=2*i-3
        if (pm(i,j).gt.p(i,j)) then
          arg=pm(i,j)+b(i,j)
          fact=dsqrt(a(i,j)/arg)
          vm(i,j)=v(i,j)+isign*fact*(pm(i,j)-p(i,j))
        else
          arg=(pm(i,j)+ps0)/(p(i,j)+ps0)
          fact=2.d0*c(i,j)/gm1(j)
          vm(i,j)=v(i,j)+isign*fact*(arg**em(j)-1.d0)
        end if
      end do
c
      j=2
      do i=1,2
        isign=2*i-3
        if (pm(i,j).gt.p(i,j)) then
          arg=pm(i,j)+b(i,j)
          fact=dsqrt(a(i,j)/arg)
          vm(i,j)=v(i,j)+isign*fact*(pm(i,j)-p(i,j))
        else
          arg=pm(i,j)/p(i,j)
          fact=2.d0*c(i,j)/gm1(j)
          vm(i,j)=v(i,j)+isign*fact*(arg**em(j)-1.d0)
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
      implicit real*8 (a-h,o-z)
      dimension dg(4,4),pm(2,2),vm(2,2),rm(2)
      dimension fp(2,2),gk(2),gp(2),drdp(2,2)
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0
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
      implicit real*8 (a-h,o-z)
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
      implicit real*8 (a-h,o-z)
      dimension pm(2,2)
      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0
      common / prmdat / alpha(2,2),r(2,2),v(2,2),p(2,2),
     *                  a(2,2),b(2,2),c(2,2)
c
      j=1
      do i=1,2
        isign=3-2*i
        if (pm(i,j).gt.p(i,j)) then
          sp=isign*v(i,j)-c(i,j)*dsqrt(ep(j)*(pm(i,j)+ps0)
     *                                       /(p(i,j)+ps0)+em(j))
        else
          sp=isign*v(i,j)-c(i,j)
        end if
        speed=max(rad*dabs(sp),speed)
      end do
c
      j=2
      do i=1,2
        isign=3-2*i
        if (pm(i,j).gt.p(i,j)) then
          sp=isign*v(i,j)-c(i,j)*dsqrt(ep(j)*pm(i,j)/p(i,j)+em(j))
        else
          sp=isign*v(i,j)-c(i,j)
        end if
        speed=max(rad*dabs(sp),speed)
      end do
c
      return
      end

