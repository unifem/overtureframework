      subroutine dudr3d (md,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                   nd3a,nd3b,n3a,n3b,dr,ds1,ds2,ds3,r,rx,gv,det,
     *                   rx2,gv2,det2,u,up,mask,ntau,tau,ad,mdat,dat,
     *                   nrprm,rparam,niprm,iparam,nrwk,rwk,niwk,
     *                   iwk,vertex,idebug,ier)
c
      implicit real*8 (a-h,o-z)
      dimension rx(*),gv(*),det(*),rx2(*),gv2(*),det2(*),
     *          u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,md),
     *          up(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,md),
     *          tau(ntau),rparam(nrprm),iparam(niprm),
     *          ad(md),dat(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,*),
     *          rwk(nrwk),iwk(niwk),mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b),
     *          vertex(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,3)
      dimension ds(3),almax(3),dp(10)
      dimension utest(5)
      integer md1a,md1b,md2a,md2b,md3a,md3b

      include 'mvars.h'
      common / mydata / gam,ht(0:2),at(2),pr(2)
      common / srcdat / nb1a,nb1b,nb2a,nb2b,nb3a,nb3b,icount
      common / timing / tflux,tslope,tsource
c
      common / mpidat / myid
c
c      common / junk2 / almx(3),igrid
c
      include 'eosDefine.h'
      include 'tzcommon.h'
c
c      almx(1)=0.d0
c      almx(2)=0.d0
c      almx(3)=0.d0
c
c      write(6,*)'Starting dudr3d...time,dt =',r,dr
c
      if (md.lt.0) then
      write(50,132)r,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *             nd3a,nd3b,n3a,n3b
  132 format('u at t=',1x,1pe15.8,/,12(1x,i3))
      amx=0.d0
      do j3=nd3a,nd3b
      do j1=nd1a,nd1b
        do j2=nd2a,n2a
          do i=1,md
            u(j1,j2,j3,i)=u(j1,n2a,j3,i)
          end do
        end do
        do j2=n2b,nd2b
          do i=1,md
            u(j1,j2,j3,i)=u(j1,n2b,j3,i)
          end do
        end do
        do j2=nd2a,nd2b
          do i=1,md
            test=u(j1,j2,j3,i)-u(j1,1,j3,i)
            amx=max(dabs(test),amx)
            if (dabs(test).gt.1.d-10) then
              write(6,*)j1,j2,j3,i,test
c              pause
            end if
          end do
        end do
        write(50,133)j1,j3,u(j1,n2a,j3,1),u(j1,n2a,j3,2),
     *                     u(j1,n2a,j3,3),u(j1,n2a,j3,5),
     *                     u(j1,n2a,j3,6)
  133   format(2(1x,i3),5(1x,1pe15.8))
      end do
      end do
      write(6,*)'max layer diff =',amx
      end if
c
      if (md.lt.0) then
        vmax=0.d0
        j1max=nd1a
        j2max=nd2a
        j3max=nd3a
        do j3=nd3a,nd3b
        do j2=nd2a,nd2b
        do j1=nd1a,nd1b
          if (dabs(u(j1,j2,j3,3)).gt.vmax) then
            vmax=dabs(u(j1,j2,j3,3))/u(j1,j2,j3,1)
            j1max=j1
            j2max=j2
            j3max=j3
          end if
        end do
        end do
        end do
        if (vmax.gt.1.d-13) then
          write(21,492)r,n1a,n1b,n2a,n2b,n3a,n3b,
     *                 vmax,j1max,j2max,j3max
  492     format(1x,f8.5,/,6(1x,i4),/,1x,1pe9.2,3(1x,i4),' ***')
        else
          write(21,493)r,n1a,n1b,n2a,n2b,n3a,n3b,
     *                 vmax,j1max,j2max,j3max
  493     format(1x,f8.5,/,6(1x,i4),/,1x,1pe9.2,3(1x,i4))
        end if
      end if
c
      if (md.lt.0) then
        tfinal=.199d0
        if (r+dr.gt.tfinal) then
          do j3=nd3a,nd3b
          do j2=nd2a,nd2b
          do j1=nd1a,nd1b
            write(82,820)(u(j1,j2,j3,i),i=1,md)
  820       format(6(1x,1pe22.15))
          end do
          end do
          end do
          stop
        end if
      end if
c
c..check for nonuniformity in the y-direction and j2 index direction
      if (md.lt.0) then
      istop=0
      tol=1.d-14
      do j1=nd1a,nd1b
      do j3=nd3a,nd3b
        do i=1,5
          utest(i)=u(j1,n2a,j3,i)
        end do
        utest(3)=0.d0
        do j2=nd2a,nd2b
          do i=1,5
            check=dabs(u(j1,j2,j3,i)-utest(i))
            if (i.eq.5) check=check/dabs(utest(i))
            if (check.gt.tol) then
              diff=u(j1,j2,j3,i)-utest(i)
              write(6,554)j1,j2,j3,i,utest(i),diff
  554         format('node =',3(1x,i2),', component =',i2,
     *               ', expected value = ',1pe22.15,
     *               ', diff = ',1pe9.2)
              istop=1
            end if
          end do
        end do
      end do
      end do
      if (istop.eq.1) then
        write(6,*)'** Starting dudr3d, nonuniformity found **'
        write(6,*)'       Printing solution to fort.21'
        write(21,454)r
  454   format(' time = ',1pe10.3)
        do j1=nd1a,nd1b
        do j3=nd3a,nd3b
        do j2=nd2a,nd2b
          write(21,565)j1,j2,j3,(u(j1,j2,j3,i),i=1,5)
  565     format(3(1x,i2),5(1x,f22.15))
        end do
        end do
        end do
        stop
      end if
      end if
c
c..mesh spacings
      ds(1)=ds1
      ds(2)=ds2
      ds(3)=ds3
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
      nb3a=n3a
      nb3b=n3b
      icount=0
c
c..parameters
c
c    iparam(1) =EOS model
c    iparam(2) =Reaction model     (iparam(2)=0 => no reaction model)
c    iparam(3) =move               is the grid changing with time (=0 => no)
c    iparam(4) =icart              Cartesian grid (=1 => yes)
c    iparam(5) =iorder             order of the method (=1 or 2)
c    iparam(6) =method             method of flux calculation (=0 => Roe solver)
c    iparam(10)=icount             the maximum number of sub-time steps is determined
c
c    iparam(17)=myid               process id for parallel runs
c
c    rparam(1) =real(eigenvalue)   for time stepping
c    rparam(2) =imag(eigenvalue)   for time stepping
c    rparam(3) =viscosity          artificial viscosity
c
c   ideal equation of state (iparam(1)=0)
c    rparam(4)=gamma
c
c   JWL equation of state (iparam(1)=1)
c    rparam(4)=gamma
c    many more parameters
c
c   one-step reaction model (iparam(2)=1)
c    rparam(11)=heat release
c    rparam(12)=1/activation energy
c    rparam(13)=prefactor
c
c   chain branching reaction model (iparam(2)=2)
c    rparam(11)=heat release
c    rparam(12)=-absorbed energy
c    rparam(13)=1/activation energy(I)
c    rparam(14)=1/activation energy(B)
c    rparam(15)=prefactor(I)
c    rparam(16)=prefactor(B)
c
c   ignition and growth reaction model (iparam(2)=3)
c    lots of parameters
c
c   timings
c    rparam(31)=tflux
c    rparam(32)=tslope
c    rparam(33)=tsourcer
c
      ieos=iparam(1)
      mr=0
      me=0

      userEOSDataPointer=rparam(89)
      iparEOS(1)=3 ! 3d
      
      if (ieos.eq.idealGasEOS) then
c..ideal EOS
        gam=rparam(4)
      elseif (ieos.eq.jwlEOS) then
c..JWL EOS
        gam=rparam(4)
c       many other params
        me=me+2
      else if( ieos.eq.userDefinedEOS ) then
c..user defined EOS ... do some stuff
        write(*,'("dudr3d: using user defined EOS")')
      else
        write(6,*)'Error (dudr) : EOS model not supported'
        ier=1
        return
      end if
c
      irxn=iparam(2)
      if (irxn.eq.noRxn) then
c..no reaction model
        ht(0)=0.d0
      elseif (irxn.eq.arrhenius) then
c..one-step reaction model
        mr=mr+1
        ht(0)=0.d0
        ht(1)=-rparam(11)
        at(1)=rparam(12)
        pr(1)=rparam(13)
      elseif (irxn.eq.chainAndBranching) then
c..chain-branching reaction model
        mr=mr+2
        ht(0)=0.d0
        ht(1)=-rparam(11)
        ht(2)=-rparam(12)
        at(1)=rparam(13)
        at(2)=rparam(14)
        pr(1)=rparam(15)
        pr(2)=rparam(16)
      elseif (irxn.eq.ignitionAndGrowth) then
c..ignition and growth (this is fake for now)
        mr=mr+1
        ht(0)=0.d0
        ht(1)=-rparam(11)
        at(1)= rparam(12)
        pr(1)= rparam(13)
      else
        write(6,*)'Error (dudr) : Reaction model not supported'
        ier=2
        return
      end if
c
c..number of hydro variables (=number of space dimensions+2)
      mh=md-mr-me
      nd=3
      if (mh-2.ne.nd) then
        write(6,*)'Error (dudr) : currently three dimensions is assumed'
        ier=3
        return
      end if
c
c..sum of hydro and reaction and EOS variables
      m=mh+mr+me
c
      av=rparam(3)
      move=iparam(3)
      icart=iparam(4)
      iorder=iparam(5)
      method=iparam(6)
      igrid=iparam(7)
      level=iparam(8)
      nstep=iparam(9)
      myid=iparam(17)
c      write(100+myid,223)nstep,myid,ieos,r
c  223 format(1x,i4,1x,i2,1x,i4,1x,1pe15.8)
c
c      write(6,*)'igrid=',igrid
c
c      write(6,*)'method,order=',method,iorder
c
c..reduce the order for the pre-step to establish the time step
      if (nstep.lt.0) ioder=1
c
c..set boundaries of mapping variables depending on whether
c  the grid is Cartesian (icart=1) or not (icart=0)
      if (icart.eq.0) then
        n1bm=nd1b
        n2bm=nd2b
        n3bm=nd3b
      else
        n1bm=nd1a
        n2bm=nd2a
        n3bm=nd3a
      end if
c
c..check the mask (if idebug>0)
c    mask=0 => unused point
c    mask>0 => interior discretization point
c    mask<0 => interpolation point
c  ** points with mask>0 cannot be next to points with mask=0 **
      if (idebug.gt.0) then
        do j3=n3a,n3b
        do j2=n2a,n2b
        do j1=n1a,n1b
          if (mask(j1,j2,j3).gt.0) then
            do k3=j3-1,j3+1
            do k2=j2-1,j2+1
            do k1=j1-1,j1+1
              if (mask(k1,k2,k3).eq.0) then
                write(6,*)'Error (dudr) : inconsistent mask value'
              end if
            end do
            end do
            end do
          end if
        end do
        end do
        end do
      end if
c
c..monitor data (if idebug>0)
      if (idebug.gt.0) then
        call mondat3d (md,m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                 nd3a,nd3b,n3a,n3b,r,dr,u,rwk,mask)
      end if
c
c..check for negative density, limit reacting species
      rhoMin=1.d-3   ! *wdh* 100713

      md1a=max(nd1a,n1a-2)  ! *wdh* check 2 ghost points only -- 
      md1b=min(nd1b,n1b+2)
      md2a=max(nd2a,n2a-2)
      md2b=min(nd2b,n2b+2)
      md3a=max(nd3a,n3a-2)
      md3b=min(nd3b,n3b+2)
c
      sum=0.d0
      icnt=0
      do j3=md3a,md3b
      do j2=md2a,md2b
      do j1=md1a,md1b
        if (mask(j1,j2,j3).ne.0) then
          icnt=icnt+md
          do i=1,md
            sum=sum+u(j1,j2,j3,i)**2
          end do
        end if
      end do
      end do
      end do
      unorm2=dsqrt(sum/icnt)
c
      do j3=md3a,md3b
      do j2=md2a,md2b
      do j1=md1a,md1b
        if (mask(j1,j2,j3).ne.0) then
          rho=u(j1,j2,j3,1)
          if (rho.le.rhoMin) then
            ! *wdh* 
            write(6,100) u(j1,j2,j3,1),igrid,mask(j1,j2,j3),j1,j2,j3,
     *                   n1a,n1b,n2a,n2b,n3a,n3b
  100       format(' ** Error (dudr) : very small density=',e10.2,
     *             ' grid=',i3,' mask=',i14,
     *             ' j1,j2,j3 =',3(1x,i4),' n1a,n1b,...=',6i3)
            ! u(j1,j2,j3,1)=-u(j1,j2,j3,1)
            u(j1,j2,j3,1)=rhoMin  !  *wdh* 100713
          end if

          do i=mh+1,mh+mr
            alam=u(j1,j2,j3,i)/u(j1,j2,j3,1)
            if    (alam.lt.0.d0) then
              if (alam.lt.-.01d0) then
                write(6,102)j1,j2,j3,i,alam
  102           format(' ** Error (dudr) : alam < 0, j1,j2,j3,i,alam =',
     *                 3(1x,i4),1x,i2,1x,1pe10.3)
              end if
              alam=0.d0
            elseif (alam.gt.1.d0) then
              if (alam.gt.1.01d0) then
                write(6,103)j1,j2,j3,i,alam
  103           format(' ** Error (dudr) : alam > 1, j1,j2,j3,i,alam =',
     *                 3(1x,i4),1x,i2,1x,1pe10.3)
              end if
              alam=1.d0
            end if
            u(j1,j2,j3,i)=u(j1,j2,j3,1)*alam
          end do
          do i=1,md
            rwk(i)=u(j1,j2,j3,i)
          end do
          ! *wdh* 100713 -- turn this check off as in 2D (it may detect negative p at ghost pts)
          if( .false. )then 
           call getp3d (md,rwk,pp,dp,0,te,ier)
           if (pp.le.0.d0) then
            write(6,104)nstep,igrid,level,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *                  j1,j2,j3
  104       format(' ** Error (dudr) : negative pressure',/,
     *             '    nstep,grid,level =',i6,1x,i3,1x,i2,/,
     *             '    nd1a,nd1b,nd2a,nd2b,nd3a,nd3b =',6(1x,i4),/,
     *             '    j1,j2,j3 =',3(1x,i4))
            write(6,*)'u =',(u(j1,j2,j3,i),i=1,md)
            stop
           end if
          end if
        end if
      end do
      end do
      end do
c
      ngrid=(nd1b-nd1a+1)*(nd2b-nd2a+1)
c
c..split up real work space =>
c    nonreactive: nreq=(13*m+md+26)*ngrid+(6*m+11)*m+2*md
c    reactive   : nreq=nreq+(5*mr+md+3)*ngrid
      ldu=1
      ldu1=ldu+12*m*ngrid
      lul=ldu1+6*m
      lur=lul+md
      la0=lur+md
      la1=la0+6*ngrid
      laj=la1+18*ngrid
      lda0=laj+2*ngrid
      lh=lda0+2*ngrid
      leigal=lh+m*ngrid
      leigel=leigal+3*m
      leiger=leigel+3*m*m
      lalpha=leiger+3*m*m
      lu0=lalpha+m
      lfx=lu0+3*md*ngrid
      lutemp=lfx+m
      ldiv=1                 ! workspace for div can overlap with du
      lrwk1=lutemp+3*ngrid*m
      if (mr.gt.0) then
        nrwk1=(5*mr+md+3)*ngrid
      else
        nrwk1=1
      end if
      nreq=lrwk1+nrwk1-1
      if (nreq.gt.nrwk) then
        ier=4
        return
      end if
c
c..split up integer work space =>
c    nonreactive: nreq=0
c    reactive   : nreq=nreq+3*ngrid
      liwk1=1
      if (mr.gt.0) then
        niwk1=3*ngrid
      else
        niwk1=1
      end if
      nreq=liwk1+niwk1-1
      if (nreq.gt.niwk) then
        ier=5
        return
      end if
c
c..filter out underflow in u
      tol=1.d-30
      do j3=nd3a,nd3b
      do j2=nd2a,nd2b
      do j1=nd1a,nd1b
        do i=1,md
          if (dabs(u(j1,j2,j3,i)).lt.tol) u(j1,j2,j3,i)=0.d0
        end do
      end do
      end do
      end do
c
c..copy u into up. Updates now accumulated on up
      do j3=nd3a,nd3b
      do j2=nd2a,nd2b
      do j1=nd1a,nd1b
        do i=1,m
          up(j1,j2,j3,i)=u(j1,j2,j3,i)
        end do
      end do
      end do
      end do
c
c compute first source contribution (zero out tau, the maximum
c truncation error estimate returned by source)
      if (mr.gt.0) then
        dr2=.5d0*dr
        do k=1,ntau
          tau(k)=0.d0
        end do
        ipc=0
        call rxnsrc3d( md,m,
     *                 nd1a,nd1b,n1a-2,n1b+2,
     *                 nd2a,nd2b,n2a-2,n2b+2,
     *                 nd3a,nd3b,n3a-2,n3b+2,
     *                 dr2,up,
     *                 tau,mask,nrwk1,rwk(lrwk1),
     *                 niwk1,iwk(liwk1),maxnstep,ipc )
      end if
c
c..set almax=0
      almax(1)=0.d0
      almax(2)=0.d0
      almax(3)=0.d0
      rparam(1)=0.d0
      rparam(2)=0.d0
c
c..compute dudr
      call hydro3d( nd,md,m,
     *              nd1a,nd1b,n1a,n1b,
     *              nd2a,nd2b,n2a,n2b,
     *              nd3a,nd3b,n3a,n3b,
     *              dr,ds,r,
     *              rx,gv,det,rx2,gv2,det2,
     *              up,mask,
     *              rwk(ldu),rwk(ldu1),rwk(lul),rwk(lur),rwk(la0),
     *              rwk(la1),rwk(laj),rwk(lda0),rwk(lh),
     *              rwk(leigal),rwk(leigel),rwk(leiger),rwk(lalpha),
     *              rwk(lu0),rwk(lfx),rwk(lutemp),
     *              vertex,almax,mdat,dat,av,ad,move,icart,
     *              iorder,method,n1bm,n2bm,n3bm,rparam,ier )
c
      if (ier.ne.0) return
c      if (igrid.eq.1) stop
c
c..compute second source contribution
      if (mr.gt.0) then
        ipc=1
        call rxnsrc3d( md,m,
     *                 nd1a,nd1b,n1a,n1b,
     *                 nd2a,nd2b,n2a,n2b,
     *                 nd3a,nd3b,n3a,n3b,
     *                 dr2,up,
     *                 tau,mask,nrwk1,rwk(lrwk1),
     *                 niwk1,iwk(liwk1),maxnstep,ipc )
      end if
c
c..up is now accumulated u ... must only return d/dt
      do j3=n3a,n3b
      do j2=n2a,n2b
      do j1=n1a,n1b
        do i=1,m
          up(j1,j2,j3,i)=(up(j1,j2,j3,i)-u(j1,j2,j3,i))/dr
        end do
      end do
      end do
      end do
c
      sum=0.d0
      icnt=0
      do j3=md3a,md3b
      do j2=md2a,md2b
      do j1=md1a,md1b
        if (mask(j1,j2,j3).ne.0) then
          icnt=icnt+md
          do i=1,md
            sum=sum+up(j1,j2,j3,i)**2
          end do
        end if
      end do
      end do
      end do
      upnorm2=dsqrt(sum/icnt)
c
      if( .false. )then ! *wdh* 061101
      write(100+myid,231)nstep,igrid,myid,r,dr,unorm2,upnorm2
  231 format(' nstep=',i4,'  igrid=',i2,'  myid=',i2,'  r=',1pe15.8,
     *       '  dr=',1pe10.3,'  norm(u)=',1pe10.3,
     *       '  norm(up)=',1pe10.3)
c
      end if
      if (md.lt.0) then
      write(50,134)r,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *             nd3a,nd3b,n3a,n3b
  134 format('up at t=',1x,1pe15.8,/,12(1x,i3))
      amx=0.d0
      do j3=n3a,n3b
      do j1=n1a,n1b
        do j2=n2a,n2b
          do i=1,md
            amx=max(dabs(up(j1,j2,j3,i)-up(j1,n2a,j3,i)),amx)
          end do
        end do
        write(50,133)j1,j3,up(j1,n2a,j3,1),up(j1,n2a,j3,2),
     *                     up(j1,n2a,j3,3),up(j1,n2a,j3,5),
     *                     up(j1,n2a,j3,6)
      end do
      end do
      write(6,*)'max layer diff =',amx
      end if
c
c..compute artificial viscosity contribution
      call artvis3d (md,m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *               nd3a,nd3b,n3a,n3b,
     *               dr,ds,rx,u,up,mask,rwk(ldiv),rwk(lfx),
     *               ad,vismax,av,icart,n1bm,n2bm,n3bm)
c
c..compute real and imaginary parts of lambda, where the time stepping
c  is interpreted as u'=lambda*u
c
c      write(6,468)almx(1),almx(2),almx(3),almax(1),almax(2),almax(3)
c  468 format('almax=',3(1x,1pe9.2),/,
c     *       '      ',3(1x,1pe9.2))
c      write(6,*)'rparam(new)=',rparam(1),rparam(2)
c
      iparam(10)=icount
c*wdh      write(23,123)r,n1a,n1b,icount
c*wdh  123 format(1x,f10.6,2(1x,i5),1x,i8)
c
c     param(1)=0.d0
      iold=0  ! set to 1 to use old dt computation
      if (iold.ne.0) then
        rparam(1)=4.d0*vismax
        rparam(2)=almax(1)/ds(1)+almax(2)/ds(2)+almax(3)/ds(3)
c        write(6,*)'rparam(old)=',rparam(1),rparam(2)
      end if
c
      rparam(31)=tflux
      rparam(32)=tslope
      rparam(33)=tsource
c
      if (idebug.gt.0) then
        write(6,105)rparam(1),rparam(2),maxnstep
      end if
  105 format('max(lambda) =',2(1pe9.2,1x),',  max(RxnSteps) =',i6)
c
c..limit lambda (move to seteos???).  Limiting is now done in seteosNew.f
c      do j3=nd3a,nd3b
c      do j2=nd2a,nd2b
c      do j1=nd1a,nd1b
c        if (mask(j1,j2,j3).ne.0) then
c          rho=u(j1,j2,j3,1)+dr*up(j1,j2,j3,1)
c          do i=mh+1,mh+mr
c            alam=(u(j1,j2,j3,i)+dr*up(j1,j2,j3,i))/rho
c            if (alam.gt.1.d0+1.d-14) then
cc             write(6,106)j1,j2,j3,i,alam
cc 106         format(' ** Error (dudr) : lam_out > 1, j1,j2,j3,i,alam =',
cc    *               3(1x,i4),1x,1pe10.3)
c              up(j1,j2,j3,i)=up(j1,j2,j3,1)
c     *                          +(u(j1,j2,j3,1)-u(j1,j2,j3,i))/dr
c            end if
c          end do
c        end if
c      end do
c      end do
c      end do
c
c..check for nonuniformity in the y-direction and j2 index direction
      if (md.lt.0) then
      istop=0
      tol=1.d-14
      do j1=n1a,n1b
      do j3=n3a,n3b
        do i=1,5
          utest(i)=u(j1,n2a,j3,i)+dr*up(j1,n2a,j3,i)
        end do
        utest(3)=0.d0
        do j2=n2a,n2b
          do i=1,5
            unew=u(j1,j2,j3,i)+dr*up(j1,j2,j3,i)
            check=dabs(unew-utest(i))
            if (i.eq.5) check=check/dabs(utest(i))
            if (check.gt.tol) then
              diff=unew-utest(i)
              write(6,554)j1,j2,j3,i,utest(i),diff
              istop=1
            end if
          end do
        end do
      end do
      end do
      if (istop.eq.1) then
        write(6,*)'** Ending dudr3d, nonuniformity found **'
        write(6,*)'      Printing solution to fort.21'
        write(21,454)r
c 454   format(' time = ',1pe10.3)
        do j1=n1a,n1b
        do j3=n3a,n3b
        do j2=n2a,n2b
          do i=1,5
            utest(i)=u(j1,j2,j3,i)+dr*up(j1,j2,j3,i)
          end do
          write(21,565)j1,j2,j3,(utest(i),i=1,5)
c 565     format(3(1x,i2),5(1x,f22.15))
        end do
        end do
        end do
        stop
      end if
      end if
c
c     write(6,*)'dudr3d done.'
c
      return
      end
c
c+++++++++++++++
c
      subroutine hydro3d( nd,md,m,
     *                    nd1a,nd1b,n1a,n1b,
     *                    nd2a,nd2b,n2a,n2b,
     *                    nd3a,nd3b,n3a,n3b,
     *                    dr,ds,r,
     *                    rx,gv,det,rx2,gv2,det2,
     *                    u,mask,
     *                    du,du1,ul,ur,a0,a1,aj,da0,h,
     *                    al,el,er,alpha,u0,fx,utemp,vertex,
     *                    almax,mdat,dat,av,ad,
     *                    move,icart,iorder,method,
     *                    n1bm,n2bm,n3bm,rparam,ier)
c
      implicit real*8 (a-h,o-z)
      dimension ds(3),rx(nd1a:n1bm,nd2a:n2bm,nd3a:n3bm,3,3),
     *          gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,3),
     *          det(nd1a:n1bm,nd2a:n2bm,nd3a:n3bm),
     *          rx2(nd1a:n1bm,nd2a:n2bm,nd3a:n3bm,3,3),
     *          gv2(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,3),
     *          det2(nd1a:n1bm,nd2a:n2bm,nd3a:n3bm),
     *          u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,md),
     *          mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b),
     *          du(m,nd1a:nd1b,nd2a:nd2b,3,2,2),du1(m,3,2),
     *          ul(md),ur(md),a0(3,nd1a:nd1b,nd2a:nd2b,2),
     *          a1(3,3,nd1a:nd1b,nd2a:nd2b,2),
     *          aj(nd1a:nd1b,nd2a:nd2b,2),
     *          da0(nd1a:nd1b,nd2a:nd2b,2),
     *          h(m,nd1a:nd1b,nd2a:nd2b),
     *          al(m,3),el(m,m,3),
     *          er(m,m,3),alpha(m),u0(md,nd1a:nd1b,nd2a:nd2b,3),fx(m),
     *          utemp(nd1a:nd1b,nd2a:nd2b,3,md),
     *          vertex(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,3),
     *          almax(3),dat(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,*),
     *          ad(md),rparam(2)
      dimension eye(3,3)
      common / junk / iflag
      include 'mvars.h'
      include 'tzcommon.h'
c*wdh      data eye / 1.d0,0.d0,0.d0,0.d0,1.d0,0.d0,0.d0,0.d0,1.d0 /
c
c..local variables
c     logical ivar
c
c..timings
      common / timing / tflux,tslope,tsource
c
      data eye / 1.d0,0.d0,0.d0,0.d0,1.d0,0.d0,0.d0,0.d0,1.d0 /
c
c..mapping does not change with r
c     ivar=.false.
c
c dat(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,.) is used to save extra grid data,
c if desired, which can then be plotted using plotStuff
      if (mdat.gt.0) then
        do j3=nd3a,nd3b
        do j2=nd2a,nd2b
        do j1=nd1a,nd1b
          dat(j1,j2,j3,1)=0.d0
        end do
        end do
        end do
      end if
c
      admax=0.d0
      do i=1,md
        admax=max(admax,ad(i))
      end do
c
c      if (iorder.ne.1) then
c        write(6,*)iorder
c        pause
c      end if
c
c..start with layer at n3a-1
      j3=n3a-1
c
c..set grid metrics and grid velocities (if necessary).
c  For this 3-D case we calculate these one layer at a
c  time and store in a1,aj, and a0. With no viscous
c  terms we need two full layers of metrics at any time.
      call metrics3d( nd1a,nd1b,na1,n1b,
     *                nd2a,nd2b,n2a,n2b,
     *                nd3a,nd3b,n3a,n3b,
     *                j3,
     *                rx,gv,det,
     *                rx2,gv2,det2,
     *                a0(1,nd1a,nd2a,1),
     *                a1(1,1,nd1a,nd2a,1),aj(nd1a,nd2a,1),
     *                move,icart,n1bm,n2bm,n3bm )
c
c Jeff put this in but it is not necessary (these metrics are computed later)
c      call metrics3d( nd1a,nd1b,na1,n1b,
c     *                nd2a,nd2b,n2a,n2b,
c     *                nd3a,nd3b,n3a,n3b,
c     *                j3+1,
c     *                rx,gv,det,
c     *                rx2,gv2,det2,
c     *                a0(1,nd1a,nd2a,2),
c     *                a1(1,1,nd1a,nd2a,2),aj(nd1a,nd2a,2),
c     *                move,icart,n1bm,n2bm,n3bm )
c
c..Fill the utemp vector. For now this is simply the 3 layer sliding
c  copy of the solution u. In the future we will add the ability to
c  fill this with conservative or primative quantities.
      call layer3d( md,m,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *               j3-1,1,u,utemp,mask )
c
      call layer3d( md,m,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *               j3,2,u,utemp,mask )
c
      call layer3d( md,m,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *               j3+1,3,u,utemp,mask )
c
      call ovtime (time0)
c
c..slope correction
      ier=0
      if (iorder.eq.2) then
c
        call slope3d( md,m,
     *                nd1a,nd1b,n1a,n1b,
     *                nd2a,nd2b,n2a,n2b,
     *                nd3a,nd3b,j3,dr,ds,a0(1,nd1a,nd2a,1),
     *                a1(1,1,nd1a,nd2a,1),aj(nd1a,nd2a,1),utemp,ul,
     *                mask(nd1a,nd2a,j3),du(1,nd1a,nd2a,1,1,1),
     *                du1,h,al,el,er,av,admax,rparam,ier)
      else
c
        call tmstep3( md,m,
     *                nd1a,nd1b,n1a,n1b,
     *                nd2a,nd2b,n2a,n2b,
     *                nd3a,nd3b,ds,a0(1,nd1a,nd2a,1),
     *                a1(1,1,nd1a,nd2a,1),utemp,ul,
     *                mask(nd1a,nd2a,j3),du(1,nd1a,nd2a,1,1,1),
     *                h,al,el,er,av,admax,rparam,ier)
      end if
c
c..compute and save center update
      do j2=n2a-1,n2b+1
      do j1=n1a-1,n1b+1
        if( mask(j1,j2,j3).ne.0 ) then
          do i=1,m
            u0(i,j1,j2,2)=utemp(j1,j2,2,i)+h(i,j1,j2)/aj(j1,j2,1)
          end do
          do i=m+1,md
            u0(i,j1,j2,2)=utemp(j1,j2,2,i)
          end do
        end if
      end do
      end do
c
      if (ier.ne.0) return
      call ovtime (time1)
      tslope=time1-time0
c
      iflag=0
c..loop over layers j3=n3a:n3b+1
      do j3=n3a,n3b+1
        j3m1=j3-1
c
        call metrics3d( nd1a,nd1b,na1,n1b,
     *                  nd2a,nd2b,n2a,n2b,
     *                  nd3a,nd3b,n3a,n3b,
     *                  j3,
     *                  rx,gv,det,
     *                  rx2,gv2,det2,
     *                  a0(1,nd1a,nd2a,2),
     *                  a1(1,1,nd1a,nd2a,2),aj(nd1a,nd2a,2),
     *                  move,icart,n1bm,n2bm,n3bm )
c
c..slide and then fill the utemp vector (see comments above 
c  regarding the sliding workspace utemp...)
        do j2=nd2a,nd2b
        do j1=nd1a,nd1b
          do i=1,md
            utemp(j1,j2,1,i)=utemp(j1,j2,2,i)
            utemp(j1,j2,2,i)=utemp(j1,j2,3,i)
          end do
        end do
        end do
c
        call layer3d( md,m,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *                j3+1,3,u,utemp,mask )
c
        call ovtime (time0)
c
c..slope correction
        if (iorder.eq.2) then
          if (m.gt.0) iflag=1
          call slope3d( md,m,
     *                  nd1a,nd1b,n1a,n1b,
     *                  nd2a,nd2b,n2a,n2b,
     *                  nd3a,nd3b,j3,dr,ds,a0(1,nd1a,nd2a,2),
     *                  a1(1,1,nd1a,nd2a,2),aj(nd1a,nd2a,2),utemp,ul,
     *                  mask(nd1a,nd2a,j3),du(1,nd1a,nd2a,1,1,2),
     *                  du1,h,al,el,er,av,admax,rparam,ier)
        else
c
          call tmstep3( md,m,
     *                  nd1a,nd1b,n1a,n1b,
     *                  nd2a,nd2b,n2a,n2b,
     *                  nd3a,nd3b,ds,a0(1,nd1a,nd2a,2),
     *                  a1(1,1,nd1a,nd2a,2),utemp,ul,
     *                  mask(nd1a,nd2a,j3),du(1,nd1a,nd2a,1,1,2),
     *                  h,al,el,er,av,admax,rparam,ier)
        end if
c
c..compute center update.
        do j2=n2a-1,n2b+1
        do j1=n1a-1,n1b+1
          if( mask(j1,j2,j3).ne.0 ) then
            do i=1,m
              u0(i,j1,j2,3)=utemp(j1,j2,2,i)+h(i,j1,j2)/aj(j1,j2,2)
            end do
            do i=m+1,md
              u0(i,j1,j2,3)=utemp(j1,j2,2,i)
            end do
          end if
        end do
        end do
c
        if (ier.ne.0) return
        call ovtime (time1)
        tslope=tslope+time1-time0
c
c..compute s3 flux along j3-1/2, add it to up(.,j1,j2,j3) and up(.,j1,j2,j3-1)
        do j2=n2a,n2b
        do j1=n1a,n1b
          if (mask(j1,j2,j3).ne.0.and.mask(j1,j2,j3m1).ne.0) then
            aj0=(aj(j1,j2,2)+aj(j1,j2,1))/2.d0
            a30=(a0(3,j1,j2,2)+a0(3,j1,j2,1))/2.d0
            a31=(a1(3,1,j1,j2,2)+a1(3,1,j1,j2,1))/2.d0
            a32=(a1(3,2,j1,j2,2)+a1(3,2,j1,j2,1))/2.d0
            a33=(a1(3,3,j1,j2,2)+a1(3,3,j1,j2,1))/2.d0
            do i=1,md
              ul(i)=utemp(j1,j2,1,i)
              ur(i)=utemp(j1,j2,2,i)
            end do
            do i=1,m
              du1(i,3,2)=du(i,j1,j2,3,2,1)/aj(j1,j2,1)
              du1(i,3,1)=du(i,j1,j2,3,1,2)/aj(j1,j2,2)
c              ul(i)=ul(i)+du(i,j1,j2,3,2,1)/aj(j1,j2,1)
c              ur(i)=ur(i)+du(i,j1,j2,3,1,2)/aj(j1,j2,2)
            end do
c
            if( itz.eq.1.and.iorder.eq.2 ) then
              call tzsource( nd,iexactp,vertex(j1,j2,j3m1,1),
     *                       vertex(j1,j2,j3m1,2), 
     *                       vertex(j1,j2,j3m1,3), r,
     *                       tzrhsl, mr )
c     
              call tzsource( nd,iexactp,vertex(j1,j2,j3,1),
     *                       vertex(j1,j2,j3,2), 
     *                       vertex(j1,j2,j3,3), r,
     *                       tzrhsr, mr )
            else
              do i=1,m
                tzrhsr(i)=0.d0
                tzrhsl(i)=0.d0
              end do
            end if
c
            call gdflux3d (md,m,aj0,a30,a31,a32,a33,ul,ur,
     *                     du1(1,3,2),du1(1,3,1),al,el,er,
     *                     alpha,almax(3),fx,method)
c          if (j1.eq.6.and.j2.eq.0.and.(j3.eq.4.or.j3m1.eq.4)) then
c            write(6,333)j1,j2,j3,(fx(i),i=1,m)
c  333       format('s3 flux',3(1x,i2),5(1x,1pe15.8))
c            if (j3.eq.4) then
c              write(6,*)'jacobian',aj(j1,j2,2)
c            else
c              write(6,*)'jacobian',aj(j1,j2,1)
c            end if
c          end if
            do i=1,m
              u(j1,j2,j3  ,i)=u(j1,j2,j3,i)
     *             +dr*fx(i)/(ds(3)*aj(j1,j2,2))
              u(j1,j2,j3m1,i)=u(j1,j2,j3m1,i)
     *             -dr*fx(i)/(ds(3)*aj(j1,j2,1))
            end do
          end if
        end do
        end do
c
        if( move.ne.0.and.icart.eq.0 ) then
          do j2=n2a,n2b
          do j1=n1a,n1b
            if( mask(j1,j2,j3).ne.0.and.mask(j1,j2,j3m1).ne.0 ) then
              fact=(aj(j1,j2,2)+aj(j1,j2,1))
     *             *(a0(3,j1,j2,2)+a0(3,j1,j2,1))/(4.d0*ds(3))
              da0(j1,j2,2)=            -fact
              da0(j1,j2,1)=da0(j1,j2,1)+fact
            end if
          end do
          end do
        end if
c
        call ovtime (time2)
        tflux=tflux+time2-time1
c
c..final source contribution for the layer j3-1
        if (j3.gt.n3a) then
          if( icart.eq.0.and.move.ne.0 ) then
c..non-Cartesian, moving
            do j2=n2a,n2b
            do j1=n1a,n1b
              if( mask(j1,j2,j3m1).ne.0 ) then
                do i=1,m
                  u(j1,j2,j3m1,i)=u(j1,j2,j3m1,i)
     *                 +dr*(u0(i,j1,j2,2)*da0(j1,j2,1))/aj(j1,j2,1)
                end do
              end if
            end do
            end do
c          else
c            do j2=n2a,n2b
c            do j1=n1a,n1b
c              if( mask(j1,j2,j3m1).ne.0 ) then
c                do i=1,m
c                  up(j1,j2,j3m1,i)=up(j1,j2,j3m1,i)/aj(j1,j2,1)
c                  du(i,j1,j2,1,1,1)=up(j1,j2,j3m1,i)
c                end do
c              end if
c            end do
c            end do
          end if
c
        end if
c
c..if j3.le.n3b, then compute fluxes in the s1 and s2 directions
        if (j3.le.n3b) then
c
c..reset a0, a1, aj and du
          do j2=n2a-1,n2b+1
          do j1=n1a-1,n1b+1
            do k=1,3
              a0(k,j1,j2,1)=a0(k,j1,j2,2)
              do i=1,md
                du(i,j1,j2,k,1,1)=du(i,j1,j2,k,1,2)
                du(i,j1,j2,k,2,1)=du(i,j1,j2,k,2,2)
              end do
            end do
c
            do i=1,md
              u0(i,j1,j2,1)=u0(i,j1,j2,2)
              u0(i,j1,j2,2)=u0(i,j1,j2,3)
            end do
          end do
          end do
c
          if (icart.eq.0) then
            do j2=n2a-1,n2b+1
            do j1=n1a-1,n1b+1
              aj(j1,j2,1)=aj(j1,j2,2)
              do k=1,3
              do l=1,3
                a1(l,k,j1,j2,1)=a1(l,k,j1,j2,2)
              end do
              end do
            end do
            end do
            if( move.ne.0 ) then
              do j2=n2a,n2b
              do j1=n1a,n1b
                da0(j1,j2,1)=da0(j1,j2,2)
              end do
              end do
            end if
          end if
c
          call ovtime (time0)
c
c..compute s1 flux on layer j3, add it to up(.,j1,j2,j3)
          do j1=n1a-1,n1b
            j1p1=j1+1
            do j2=n2a,n2b
              if (mask(j1,j2,j3).ne.0.and.mask(j1p1,j2,j3).ne.0) then
                aj0=(aj(j1p1,j2,1)+aj(j1,j2,1))/2.d0
                a10=(a0(1,j1p1,j2,1)+a0(1,j1,j2,1))/2.d0
                a11=(a1(1,1,j1p1,j2,1)+a1(1,1,j1,j2,1))/2.d0
                a12=(a1(1,2,j1p1,j2,1)+a1(1,2,j1,j2,1))/2.d0
                a13=(a1(1,3,j1p1,j2,1)+a1(1,3,j1,j2,1))/2.d0
                do i=1,md
                  ul(i)=utemp(j1  ,j2,2,i)
                  ur(i)=utemp(j1p1,j2,2,i)
                end do
                do i=1,m
                  du1(i,1,2)=du(i,j1  ,j2,1,2,1)/aj(j1  ,j2,1)
                  du1(i,1,1)=du(i,j1p1,j2,1,1,1)/aj(j1p1,j2,1)
c                  ul(i)=ul(i)+du(i,j1  ,j2,1,2,1)/aj(j1  ,j2,1)
c                  ur(i)=ur(i)+du(i,j1p1,j2,1,1,1)/aj(j1p1,j2,1)
                end do
c
                if( itz.eq.1.and.iorder.eq.2 ) then
                  call tzsource( nd,iexactp,vertex(j1,j2,j3,1),
     *                           vertex(j1,j2,j3,2), 
     *                           vertex(j1,j2,j3,3), r,
     *                           tzrhsl, mr )
c     
                  call tzsource( nd,iexactp,vertex(j1p1,j2,j3,1),
     *                           vertex(j1p1,j2,j3,2), 
     *                           vertex(j1p1,j2,j3,3), r,
     *                           tzrhsr, mr )
                else
                  do i=1,m
                    tzrhsr(i)=0.d0
                    tzrhsl(i)=0.d0
                  end do
                end if
c
                call gdflux3d (md,m,aj0,a10,a11,a12,a13,ul,ur,
     *                         du1(1,1,2),du1(1,1,1),al,el,er,
     *                         alpha,almax(1),fx,method)
c          if ((j1.eq.6.or.j1p1.eq.6).and.j2.eq.0.and.j3.eq.4) then
c            write(6,334)j1,j2,j3,(fx(i),i=1,m)
c  334       format('s1 flux',3(1x,i2),5(1x,1pe15.8))
c            if (j1.eq.6) then
c              write(6,*)'jacobian',aj(j1,j2,2)
c            else
c              write(6,*)'jacobian',aj(j1p1,j2,2)
c            end if
c          end if
                do i=1,m
                  u(j1p1,j2,j3,i)=u(j1p1,j2,j3,i)
     *                 +dr*fx(i)/(ds(1)*aj(j1p1,j2,2))
                  u(j1,j2,j3,i)=u(j1,j2,j3,i)
     *                 -dr*fx(i)/(ds(1)*aj(j1,j2,2))
                end do
              end if
            end do
          end do
c
          call ovtime( time1 )
          tflux=tflux+time1-time0
c
          if( move.ne.0.and.icart.eq.0 ) then
            do j1=n1a-1,n1b
              j1p1=j1+1
              do j2=n2a,n2b
                if( mask(j1,j2,j3).ne.0.and.mask(j1p1,j2,j3).ne.0 ) then
                  fact=(aj(j1p1,j2,1)+aj(j1,j2,1))
     *                 *(a0(1,j1p1,j2,1)+a0(1,j1,j2,1))/(4.d0*ds(1))
                  da0(j1p1,j2,1)=da0(j1p1,j2,1)-fact
                  da0(j1,  j2,1)=da0(j1,  j2,1)+fact
                end if
              end do
            end do
          end if
c
c..compute s2 flux on layer j3, add it to up(.,j1,j2,j3)
          call ovtime( time0 )
          do j2=n2a-1,n2b
            j2p1=j2+1
            do j1=n1a,n1b
              if (mask(j1,j2,j3).ne.0.and.mask(j1,j2p1,j3).ne.0) then
                aj0=(aj(j1,j2p1,1)+aj(j1,j2,1))/2.d0
                a20=(a0(2,j1,j2p1,1)+a0(2,j1,j2,1))/2.d0
                a21=(a1(2,1,j1,j2p1,1)+a1(2,1,j1,j2,1))/2.d0
                a22=(a1(2,2,j1,j2p1,1)+a1(2,2,j1,j2,1))/2.d0
                a23=(a1(2,3,j1,j2p1,1)+a1(2,3,j1,j2,1))/2.d0
                do i=1,md
                  ul(i)=utemp(j1,j2  ,2,i)
                  ur(i)=utemp(j1,j2p1,2,i)
                end do
                do i=1,m
                  du1(i,2,2)=du(i,j1,j2  ,2,2,1)/aj(j1,j2  ,1)
                  du1(i,2,1)=du(i,j1,j2p1,2,1,1)/aj(j1,j2p1,1)
c                  ul(i)=ul(i)+du(i,j1,j2  ,2,2,1)/aj(j1,j2  ,1)
c                  ur(i)=ur(i)+du(i,j1,j2p1,2,1,1)/aj(j1,j2p1,1)
                end do
c
                if( itz.eq.1.and.iorder.eq.2 ) then
                  call tzsource( nd,iexactp,vertex(j1,j2,j3,1),
     *                           vertex(j1,j2,j3,2),
     *                           vertex(j1,j2,j3,3), r,
     *                           tzrhsl, mr )
c
                  call tzsource( nd,iexactp,vertex(j1,j2p1,j3,1),
     *                           vertex(j1,j2p1,j3,2),
     *                           vertex(j1,j2p1,j3,3), r,
     *                           tzrhsr, mr )
                else
                  do i=1,m
                    tzrhsr(i)=0.d0
                    tzrhsl(i)=0.d0
                  end do
                end if
c
                call gdflux3d (md,m,aj0,a20,a21,a22,a23,ul,ur,
     *                         du1(1,2,2),du1(1,2,1),al,el,er,
     *                         alpha,almax(2),fx,method)
c          if (j1.eq.6.and.(j2.eq.0.or.j2p1.eq.0).and.j3.eq.4) then
c            write(6,335)j1,j2,j3,(fx(i),i=1,m)
c  335       format('s2 flux',3(1x,i2),5(1x,1pe15.8))
c            if (j2.eq.0) then
c              write(6,*)'jacobian',aj(j1,j2,2)
c            else
c              write(6,*)'jacobian',aj(j1,j2p1,2)
c            end if
c          end if
                do i=1,m
                  u(j1,j2p1,j3,i)=u(j1,j2p1,j3,i)
     *                 +dr*fx(i)/(ds(2)*aj(j1,j2p1,2))
                  u(j1,j2,j3,i)=u(j1,j2,j3,i)
     *                 -dr*fx(i)/(ds(2)*aj(j1,j2,2))
                end do
              end if
            end do
          end do
c
          call ovtime (time1)
          tflux=tflux+time1-time0
c
          if( move.ne.0.and.icart.eq.0 ) then
            do j2=n2a-1,n2b
              j2p1=j2+1
              do j1=n1a,n1b
                if( mask(j1,j2,j3).ne.0.and.mask(j1,j2p1,j3).ne.0 ) then
                  fact=(aj(j1,j2p1,1)+aj(j1,j2,1))
     *                 *(a0(2,j1,j2p1,1)+a0(2,j1,j2,1))/(4.d0*ds(2))
                  da0(j1,j2p1,1)=da0(j1,j2p1,1)-fact
                  da0(j1,j2,  1)=da0(j1,j2,  1)+fact
                end if
              end do
            end do
          end if
c
c..add free stream correction to up (if a non-Cartesian grid)
          if (icart.eq.0) then
            if( move.eq.0 ) then
              do j2=n2a,n2b
              do j1=n1a,n1b
                if (mask(j1,j2,j3).ne.0) then
                  d1p=det(j1+1,j2,j3)+det(j1,j2,j3)
                  d1m=det(j1-1,j2,j3)+det(j1,j2,j3)
                  d2p=det(j1,j2+1,j3)+det(j1,j2,j3)
                  d2m=det(j1,j2-1,j3)+det(j1,j2,j3)
                  d3p=det(j1,j2,j3+1)+det(j1,j2,j3)
                  d3m=det(j1,j2,j3-1)+det(j1,j2,j3)
                  do k=1,3
                    da= ((rx(j1+1,j2,j3,1,k)+rx(j1,j2,j3,1,k))*d1p
     *                  -(rx(j1-1,j2,j3,1,k)+rx(j1,j2,j3,1,k))*d1m)
     *                  /(4*ds(1))
     *                 +((rx(j1,j2+1,j3,2,k)+rx(j1,j2,j3,2,k))*d2p
     *                  -(rx(j1,j2-1,j3,2,k)+rx(j1,j2,j3,2,k))*d2m)
     *                  /(4*ds(2))
     *                 +((rx(j1,j2,j3+1,3,k)+rx(j1,j2,j3,3,k))*d3p
     *                  -(rx(j1,j2,j3-1,3,k)+rx(j1,j2,j3,3,k))*d3m)
     *                  /(4*ds(3))
                    call flux3d (md,m,eye(k,1),eye(k,2),eye(k,3),
     *                           u0(1,j1,j2,2),fx)
c                  if (j1.eq.6.and.j2.eq.0.and.j3.eq.4) then
c                    write(6,355)j1,j2,j3,da,(fx(i),i=1,m),aj(j1,j2,2)
c  355               format('free',3(1x,i2),7(1x,1pe15.8))
c                  end if
                    do i=1,m
                      u(j1,j2,j3,i)=u(j1,j2,j3,i)
     *                     +dr*da*fx(i)/aj(j1,j2,2)
                    end do
                  end do
                end if
              end do
              end do
            else
              do j2=n2a,n2b
              do j1=n1a,n1b
                if (mask(j1,j2,j3).ne.0) then
                  det0=det(j1,j2,j3)+det2(j1,j2,j3)
                  d1p=det(j1+1,j2,j3)+det2(j1+1,j2,j3)+det0
                  d1m=det(j1-1,j2,j3)+det2(j1-1,j2,j3)+det0
                  d2p=det(j1,j2+1,j3)+det2(j1,j2+1,j3)+det0
                  d2m=det(j1,j2-1,j3)+det2(j1,j2-1,j3)+det0
                  d3p=det(j1,j2,j3+1)+det2(j1,j2,j3+1)+det0
                  d3m=det(j1,j2,j3-1)+det2(j1,j2,j3-1)+det0
                  do k=1,3
                    rx10=rx(j1,j2,j3,1,k)+rx2(j1,j2,j3,1,k)
                    rx20=rx(j1,j2,j3,2,k)+rx2(j1,j2,j3,2,k)
                    rx30=rx(j1,j2,j3,3,k)+rx2(j1,j2,j3,3,k)
                    da= ((rx(j1+1,j2,j3,1,k)+rx2(j1+1,j2,j3,1,k)+rx10)
     *                   *d1p
     *                  -(rx(j1-1,j2,j3,1,k)+rx2(j1-1,j2,j3,1,k)+rx10)
     *                   *d1m)
     *                  /(16*ds(1))
     *                 +((rx(j1,j2+1,j3,2,k)+rx2(j1,j2+1,j3,2,k)+rx20)
     *                   *d2p
     *                  -(rx(j1,j2-1,j3,2,k)+rx2(j1,j2-1,j3,2,k)+rx20)
     *                   *d2m)
     *                  /(16*ds(2))
     *                 +((rx(j1,j2,j3+1,3,k)+rx2(j1,j2,j3+1,3,k)+rx30)
     *                   *d3p
     *                  -(rx(j1,j2,j3-1,3,k)+rx2(j1,j2,j3-1,3,k)+rx30)
     *                   *d3m)
     *                  /(16*ds(3))
                    call flux3d (md,m,eye(k,1),eye(k,2),eye(k,3),
     *                           u0(1,j1,j2,2),fx)
                    do i=1,m
                      u(j1,j2,j3,i)=u(j1,j2,j3,i)
     *                     +dr*da*fx(i)/aj(j1,j2,2)
                    end do
                  end do
                end if
              end do
              end do
            end if
          end if
c
        end if
c
c..bottom of main loop over layers
      end do
c
c..add twilight zone stuff
      if( itz.eq.1 ) then
        do j3=n3a,n3b
        do j2=n2a,n2b
        do j1=n1a,n1b
          call tzsource( nd,iexactp,vertex(j1,j2,j3,1),
     *         vertex(j1,j2,j3,2), vertex(j1,j2,j3,3),
     *         r+0.5*tzdt,tzrhsl,mr )
          do i=1,m
            u(j1,j2,j3,i)=u(j1,j2,j3,i)+tzdt*tzrhsl(i)
          end do
        end do
        end do
        end do
      end if
c
      return
      end
c
c++++++++++++++++
c
      subroutine tmstep3( md,m,
     *                    nd1a,nd1b,n1a,n1b,
     *                    nd2a,nd2b,n2a,n2b,
     *                    nd3a,nd3b,ds,a0,a1,utemp,u0,mask,
     *                    du,h,al,el,er,av,admax,rparam,ier)
c
      implicit real*8 (a-h,o-z)
      dimension a0(3,nd1a:nd1b,nd2a:nd2b),a1(3,3,nd1a:nd1b,nd2a:nd2b),
     *          mask(nd1a:nd1b,nd2a:nd2b),
     *          du(m,nd1a:nd1b,nd2a:nd2b,3,2),h(m,nd1a:nd1b,nd2a:nd2b),
     *          utemp(nd1a:nd1b,nd2a:nd2b,3,md),u0(md),
     *          al(m,3),el(m,m,3),er(m,m,3),ds(3),rparam(2)
      include 'mvars.h'
c
c..compute time step estimate and zero out du and h
      do j2=n2a-1,n2b+1
      do j1=n1a-1,n1b+1
        if (mask(j1,j2).ne.0) then
c
          do i=1,md
            u0(i)=utemp(j1,j2,2,i)
          end do
          call eigenv3d (md,m,a1(1,1,j1,j2),u0,al,el,er,ier)
c
          vxm=(a1(1,1,j1,j2)*utemp(j1-1,j2,2,2)
     *        +a1(1,2,j1,j2)*utemp(j1-1,j2,2,3)
     *        +a1(1,3,j1,j2)*utemp(j1-1,j2,2,4))/utemp(j1-1,j2,2,1)
          vxp=(a1(1,1,j1,j2)*utemp(j1+1,j2,2,2)
     *        +a1(1,2,j1,j2)*utemp(j1+1,j2,2,3)
     *        +a1(1,3,j1,j2)*utemp(j1+1,j2,2,4))/utemp(j1+1,j2,2,1)
          vym=(a1(2,1,j1,j2)*utemp(j1,j2-1,2,2)
     *        +a1(2,2,j1,j2)*utemp(j1,j2-1,2,3)
     *        +a1(2,3,j1,j2)*utemp(j1,j2-1,2,4))/utemp(j1,j2-1,2,1)
          vyp=(a1(2,1,j1,j2)*utemp(j1,j2+1,2,2)
     *        +a1(2,2,j1,j2)*utemp(j1,j2+1,2,3)
     *        +a1(2,3,j1,j2)*utemp(j1,j2+1,2,4))/utemp(j1,j2+1,2,1)
          vzm=(a1(3,1,j1,j2)*utemp(j1,j2,1,2)
     *        +a1(3,2,j1,j2)*utemp(j1,j2,1,3)
     *        +a1(3,3,j1,j2)*utemp(j1,j2,1,4))/utemp(j1,j2,1,1)
          vzp=(a1(3,1,j1,j2)*utemp(j1,j2,3,2)
     *        +a1(3,2,j1,j2)*utemp(j1,j2,3,3)
     *        +a1(3,3,j1,j2)*utemp(j1,j2,3,4))/utemp(j1,j2,3,1)
          div=.5d0*((vxp-vxm)/ds(1)+(vyp-vym)/ds(2)+(vzp-vzm)/ds(3))
c
c..real and imaginary parts of the time-stepping eigenvalues
          tsreal=4.d0*(av*max(-div,0.d0)+admax)
          tsimag= max(dabs(a0(1,j1,j2)+al(1,1)),
     *                dabs(a0(1,j1,j2)+al(m,1)))/ds(1)
     *           +max(dabs(a0(2,j1,j2)+al(1,2)),
     *                dabs(a0(2,j1,j2)+al(m,2)))/ds(2)
     *           +max(dabs(a0(3,j1,j2)+al(1,3)),
     *                dabs(a0(3,j1,j2)+al(m,3)))/ds(3)
          rparam(1)=max(tsreal,rparam(1))
          rparam(2)=max(tsimag,rparam(2))
c
          if (ier.ne.0) then
            write(*,*) 'ERROR return from eigenv for j1,j2,j3=',j1,j2,j3
            return
          end if
c
        end if
c
        do i=1,m
          h(i,j1,j2)=0.d0
          du(i,j1,j2,1,1)=0.d0
          du(i,j1,j2,1,2)=0.d0
          du(i,j1,j2,2,1)=0.d0
          du(i,j1,j2,2,2)=0.d0
          du(i,j1,j2,3,1)=0.d0
          du(i,j1,j2,3,2)=0.d0
        end do
c
      end do
      end do
c
      return
      end
c
c++++++++++++++++
c
      subroutine slope3d( md,m,
     *                    nd1a,nd1b,n1a,n1b,
     *                    nd2a,nd2b,n2a,n2b,
     *                    nd3a,nd3b,j3,dr,ds,a0,a1,aj,utemp,u0,mask,
     *                    du,du1,h,al,el,er,av,admax,rparam,ier)
c
      implicit real*8 (a-h,o-z)
      dimension a0(3,nd1a:nd1b,nd2a:nd2b),a1(3,3,nd1a:nd1b,nd2a:nd2b),
     *          aj(nd1a:nd1b,nd2a:nd2b),mask(nd1a:nd1b,nd2a:nd2b),
     *          du(m,nd1a:nd1b,nd2a:nd2b,3,2),h(m,nd1a:nd1b,nd2a:nd2b),
     *          utemp(nd1a:nd1b,nd2a:nd2b,3,md),u0(md),du1(m,3,2),
     *          al(m,3),el(m,m,3),er(m,m,3),ds(3),rparam(2)
      include 'mvars.h'
      include 'tzcommon.h'
c
c      common / junk2 / almx(3),igrid
c
      c0=dr/2.d0
      c1=c0/ds(1)
      c2=c0/ds(2)
      c3=c0/ds(3)
c
c..source contribution
      do j2=n2a-1,n2b+1
      do j1=n1a-1,n1b+1
        do i=1,m
          h(i,j1,j2)=0.d0
        end do
      end do
      end do
c
c..solution differences: s1-direction
      do j2=n2a-1,n2b+1
      do j1=n1a,n1b+1
        do i=1,m
          du(i,j1  ,j2,1,1)=utemp(j1,j2,2,i)-utemp(j1-1,j2,2,i)
          du(i,j1-1,j2,1,2)=du(i,j1,j2,1,1)
        end do
      end do
      do i=1,m
        du(i,n1a-1,j2,1,1)=utemp(n1a-1,j2,2,i)-utemp(n1a-2,j2,2,i)
        du(i,n1b+1,j2,1,2)=utemp(n1b+2,j2,2,i)-utemp(n1b+1,j2,2,i)
      end do
      end do
c
c..solution differences: s2-direction
      do j1=n1a-1,n1b+1
      do j2=n2a,n2b+1
        do i=1,m
          du(i,j1,j2  ,2,1)=utemp(j1,j2,2,i)-utemp(j1,j2-1,2,i)
          du(i,j1,j2-1,2,2)=du(i,j1,j2,2,1)
        end do
      end do
      do i=1,m
        du(i,j1,n2a-1,2,1)=utemp(j1,n2a-1,2,i)-utemp(j1,n2a-2,2,i)
        du(i,j1,n2b+1,2,2)=utemp(j1,n2b+2,2,i)-utemp(j1,n2b+1,2,i)
      end do
      end do
c
c..solution differences: s3-direction
      do j2=n2a-1,n2b+1
      do j1=n1a-1,n1b+1
        do i=1,m
          du(i,j1,j2,3,1)=utemp(j1,j2,2,i)-utemp(j1,j2,1,i)
          du(i,j1,j2,3,2)=utemp(j1,j2,3,i)-utemp(j1,j2,2,i)
        end do
      end do
      end do
c
c..compute slope contribution and add it to source contribution
      do j2=n2a-1,n2b+1
      do j1=n1a-1,n1b+1
        if (mask(j1,j2).ne.0) then
c
          do i=1,md
            u0(i)=utemp(j1,j2,2,i)
          end do
          call eigenv3d (md,m,a1(1,1,j1,j2),u0,al,el,er,ier)
c
          vxm=(a1(1,1,j1,j2)*utemp(j1-1,j2,2,2)
     *        +a1(1,2,j1,j2)*utemp(j1-1,j2,2,3)
     *        +a1(1,3,j1,j2)*utemp(j1-1,j2,2,4))/utemp(j1-1,j2,2,1)
          vxp=(a1(1,1,j1,j2)*utemp(j1+1,j2,2,2)
     *        +a1(1,2,j1,j2)*utemp(j1+1,j2,2,3)
     *        +a1(1,3,j1,j2)*utemp(j1+1,j2,2,4))/utemp(j1+1,j2,2,1)
          vym=(a1(2,1,j1,j2)*utemp(j1,j2-1,2,2)
     *        +a1(2,2,j1,j2)*utemp(j1,j2-1,2,3)
     *        +a1(2,3,j1,j2)*utemp(j1,j2-1,2,4))/utemp(j1,j2-1,2,1)
          vyp=(a1(2,1,j1,j2)*utemp(j1,j2+1,2,2)
     *        +a1(2,2,j1,j2)*utemp(j1,j2+1,2,3)
     *        +a1(2,3,j1,j2)*utemp(j1,j2+1,2,4))/utemp(j1,j2+1,2,1)
          vzm=(a1(3,1,j1,j2)*utemp(j1,j2,1,2)
     *        +a1(3,2,j1,j2)*utemp(j1,j2,1,3)
     *        +a1(3,3,j1,j2)*utemp(j1,j2,1,4))/utemp(j1,j2,1,1)
          vzp=(a1(3,1,j1,j2)*utemp(j1,j2,3,2)
     *        +a1(3,2,j1,j2)*utemp(j1,j2,3,3)
     *        +a1(3,3,j1,j2)*utemp(j1,j2,3,4))/utemp(j1,j2,3,1)
          div=.5d0*((vxp-vxm)/ds(1)+(vyp-vym)/ds(2)+(vzp-vzm)/ds(3))
c
c..real and imaginary parts of the time-stepping eigenvalues
          tsreal=4.d0*(av*max(-div,0.d0)+admax)
          tsimag= max(dabs(a0(1,j1,j2)+al(1,1)),
     *                dabs(a0(1,j1,j2)+al(m,1)))/ds(1)
     *           +max(dabs(a0(2,j1,j2)+al(1,2)),
     *                dabs(a0(2,j1,j2)+al(m,2)))/ds(2)
     *           +max(dabs(a0(3,j1,j2)+al(1,3)),
     *                dabs(a0(3,j1,j2)+al(m,3)))/ds(3)
          rparam(1)=max(tsreal,rparam(1))
          rparam(2)=max(tsimag,rparam(2))
c
c          do k=1,3
c            almx(k)=max(dabs(a0(k,j1,j2)+al(1,k)),
c     *                  dabs(a0(k,j1,j2)+al(m,k)),almx(k))
c          end do
c
c          if (igrid.eq.1) then
c          write(44,333)j1,j2,j3,al(1,2),al(m,2)
c  333     format(3(1x,i3),2(1x,1pe10.3))
c          end if
c
          if (ier.ne.0) then
            write(*,*) 'ERROR return from eigenv for j1,j2,j3=',j1,j2,j3
            return
          end if
c
          tmp=c0*aj(j1,j2)
          do i=1,m
            h(i,j1,j2)=tmp*h(i,j1,j2)
            do kd=1,3
              do ks=1,2
                du1(i,kd,ks)=du(i,j1,j2,kd,ks)
                du(i,j1,j2,kd,ks)=h(i,j1,j2)
              end do
            end do
          end do
c
          do j=1,m
            alphal=0.d0
            alphar=0.d0
            do i=1,m
              alphal=alphal+el(j,i,1)*du1(i,1,1)
              alphar=alphar+el(j,i,1)*du1(i,1,2)
            end do
c
            if( itz.eq.1 ) then
              alphal=0.5*(alphal+alphar)
              alphar=alphal
            end if
            if (alphal*alphar.gt.0.d0) then
              if (dabs(alphal).lt.dabs(alphar)) then
                alpha=alphal
              else
                alpha=alphar
              end if
              alpha=alpha*aj(j1,j2)
              alam=c1*(a0(1,j1,j2)+al(j,1))
              tmp=alam*alpha
              do i=1,m
                tmp2=tmp*er(i,j,1)
                h(i,j1,j2)=h(i,j1,j2)-tmp2
                du(i,j1,j2,3,1)=du(i,j1,j2,3,1)-tmp2
                du(i,j1,j2,3,2)=du(i,j1,j2,3,2)-tmp2
                du(i,j1,j2,2,1)=du(i,j1,j2,2,1)-tmp2
                du(i,j1,j2,2,2)=du(i,j1,j2,2,2)-tmp2
                du(i,j1,j2,1,1)=du(i,j1,j2,1,1)
     *                           -(min(alam,0.d0)+.5d0)*alpha*er(i,j,1)
                du(i,j1,j2,1,2)=du(i,j1,j2,1,2)
     *                           -(max(alam,0.d0)-.5d0)*alpha*er(i,j,1)
              end do
            end if
          end do
c
          do j=1,m
            alphal=0.d0
            alphar=0.d0
            do i=1,m
              alphal=alphal+el(j,i,2)*du1(i,2,1)
              alphar=alphar+el(j,i,2)*du1(i,2,2)
            end do
c
            if( itz.eq.1 ) then
              alphal=0.5*(alphal+alphar)
              alphar=alphal
            end if
            if (alphal*alphar.gt.0.d0) then
              if (dabs(alphal).lt.dabs(alphar)) then
                alpha=alphal
              else
                alpha=alphar
              end if
              alpha=alpha*aj(j1,j2)
              alam=c2*(a0(2,j1,j2)+al(j,2))
              tmp=alam*alpha
              do i=1,m
                tmp2=tmp*er(i,j,2)
                h(i,j1,j2)=h(i,j1,j2)-tmp2
                du(i,j1,j2,1,1)=du(i,j1,j2,1,1)-tmp2
                du(i,j1,j2,1,2)=du(i,j1,j2,1,2)-tmp2
                du(i,j1,j2,3,1)=du(i,j1,j2,3,1)-tmp2
                du(i,j1,j2,3,2)=du(i,j1,j2,3,2)-tmp2
                du(i,j1,j2,2,1)=du(i,j1,j2,2,1)
     *                           -(min(alam,0.d0)+.5d0)*alpha*er(i,j,2)
                du(i,j1,j2,2,2)=du(i,j1,j2,2,2)
     *                           -(max(alam,0.d0)-.5d0)*alpha*er(i,j,2)
              end do
            end if
          end do
c
          do j=1,m
            alphal=0.d0
            alphar=0.d0
            do i=1,m
              alphal=alphal+el(j,i,3)*du1(i,3,1)
              alphar=alphar+el(j,i,3)*du1(i,3,2)
            end do
c
            if( itz.eq.1 ) then
              alphal=0.5*(alphal+alphar)
              alphar=alphal
            end if
            if (alphal*alphar.gt.0.d0) then
              if (dabs(alphal).lt.dabs(alphar)) then
                alpha=alphal
              else
                alpha=alphar
              end if
              alpha=alpha*aj(j1,j2)
              alam=c3*(a0(3,j1,j2)+al(j,3))
              tmp=alam*alpha
              do i=1,m
                tmp2=tmp*er(i,j,3)
                h(i,j1,j2)=h(i,j1,j2)-tmp2
                du(i,j1,j2,2,1)=du(i,j1,j2,2,1)-tmp2
                du(i,j1,j2,2,2)=du(i,j1,j2,2,2)-tmp2
                du(i,j1,j2,1,1)=du(i,j1,j2,1,1)-tmp2
                du(i,j1,j2,1,2)=du(i,j1,j2,1,2)-tmp2
                du(i,j1,j2,3,1)=du(i,j1,j2,3,1)
     *                           -(min(alam,0.d0)+.5d0)*alpha*er(i,j,3)
                du(i,j1,j2,3,2)=du(i,j1,j2,3,2)
     *                           -(max(alam,0.d0)-.5d0)*alpha*er(i,j,3)
              end do
            end if
          end do
c
        end if
      end do
      end do
c
      return
      end
c
c++++++++++++++++
c
      subroutine eigenv3d (md,m,a,u,al,el,er,ier)
c
      implicit real*8 (a-h,o-z)
      dimension a(3,3),u(md),al(m,3),el(m,m,3),er(m,m,3)
      dimension an(3),t1(3),t2(3),dp(10)
c
      include 'mvars.h'
c      common / mvars / mh,mr,me,ieos,irxn
      data c2min, c2eps / -1.d-2, 1.d-6 /
c
      ier=0
c
c..first compute direction-free part
      v1=u(2)/u(1)
      v2=u(3)/u(1)
      v3=u(4)/u(1)
      q2=v1**2+v2**2+v3**2
c
      call getp3d (md,u,p,dp,mr+2,te,ier)
      if (ier.ne.0) then
        write(6,*)'Error (eigenv3d) : getp3d, ier=',ier
        write(6,*)'u =',(u(i),i=1,m)
        ier=123
        return
      end if
      h=(u(5)+p)/u(1)
      sum=0.d0
      do k=1,mr
        sum=sum+u(5+k)*dp(2+k)/u(1)
      end do
      c2=dp(1)+(h-.5d0*q2)*dp(2)+sum
c      if (c2.le.0.d0) then
c        write(6,*)'Error (eigenv3d) : cannot compute sound speed'
c        write(6,*)'u =',(u(i),i=1,m)
c        write(6,*)'p =',p
c        ier=123
c        return
c      end if
c      c=dsqrt(c2)
      c=dsqrt(max(c2,c2eps))
c
      tmp1=dp(2)/(2.d0*c2)
      tmp2=2.d0*tmp1
      tmp3=h-q2+sum/dp(2)
      tmp4=.5d0/c
c
c begin with nonreactive contributions to el and er
      el(1,1,1)=-tmp1*tmp3+.5d0
      el(1,2,1)=-tmp1*v1
      el(1,3,1)=-tmp1*v2
      el(1,4,1)=-tmp1*v3
      el(1,5,1)= tmp1
      el(4,1,1)= tmp2*tmp3
      el(4,2,1)= tmp2*v1
      el(4,3,1)= tmp2*v2
      el(4,4,1)= tmp2*v3
      el(4,5,1)=-tmp2
      el(m,1,1)= el(1,1,1)
      el(m,2,1)= el(1,2,1)
      el(m,3,1)= el(1,3,1)
      el(m,4,1)= el(1,4,1)
      el(m,5,1)= tmp1
c
      er(1,1,1)=1.d0
      er(2,1,1)=v1
      er(3,1,1)=v2
      er(4,1,1)=v3
      er(5,1,1)=h
      er(1,4,1)=1.d0
      er(2,4,1)=v1
      er(3,4,1)=v2
      er(4,4,1)=v3
      er(5,4,1)=h+(sum-c2)/dp(2)
      er(1,m,1)=1.d0
      er(2,m,1)=v1
      er(3,m,1)=v2
      er(4,m,1)=v3
      er(5,m,1)=h
c
c add on reactive and EOS contributions
      do i=1,mr+me
        ip4=i+4
        ip5=i+5
        heat=0.d0
        if (i.le.mr) heat=-dp(2+i)/dp(2)
        el(1,ip5,1)=-tmp1*heat
        el(2,ip5,1)= 0.d0
        el(3,ip5,1)= 0.d0
        el(4,ip5,1)= tmp2*heat
        el(m,ip5,1)= el(1,ip5,1)
        do j=1,m
          er(j,ip4,1)=0.d0
        end do
        er(5  ,ip4,1)=heat
        er(ip5,ip4,1)=1.d0
      end do
c
      do i=1,mr+me
        ip4=i+4
        ip5=i+5
        alam=u(ip5)/u(1)
        er(ip5,1,1)= alam
        er(ip5,2,1)= 0.d0
        er(ip5,3,1)= 0.d0
        er(ip5,4,1)= 0.d0
        er(ip5,m,1)= alam
        do j=1,m
          el(ip4,j,1)=alam*el(4,j,1)
        end do
        el(ip4,  1,1)=el(ip4,  1,1)-alam
        el(ip4,ip5,1)=el(ip4,ip5,1)+1.d0
      end do
c
c..make copies
      do i=1,m
        do j=1,m
          el(i,j,2)=el(i,j,1)
          el(i,j,3)=el(i,j,1)
          er(i,j,2)=er(i,j,1)
          er(i,j,3)=er(i,j,1)
        end do
      end do
c
c..add on directional part
      do k=1,3
        r=dsqrt(a(k,1)**2+a(k,2)**2+a(k,3)**2)
        an(1)=a(k,1)/r
        an(2)=a(k,2)/r
        an(3)=a(k,3)/r
        if (dabs(an(1)).gt.dabs(an(2)).and.
     *      dabs(an(1)).gt.dabs(an(3))) then
          if (dabs(an(2)).lt.dabs(an(3))) then
            t1(1)=-an(2)
            t1(2)= an(1)
            t1(3)= 0.d0
          else
            t1(1)=-an(3)
            t1(2)= 0.d0
            t1(3)= an(1)
          end if
        elseif (dabs(an(2)).gt.dabs(an(3))) then
          if (dabs(an(1)).lt.dabs(an(3))) then
            t1(1)= an(2)
            t1(2)=-an(1)
            t1(3)= 0.d0
          else
            t1(1)= 0.d0
            t1(2)=-an(3)
            t1(3)= an(2)
          end if
        else
          if (dabs(an(1)).lt.dabs(an(2))) then
            t1(1)= an(3)
            t1(2)= 0.d0
            t1(3)=-an(1)
          else
            t1(1)= 0.d0
            t1(2)= an(3)
            t1(3)=-an(2)
          end if
        end if
        fact=1.d0/dsqrt(t1(1)**2+t1(2)**2+t1(3)**2)
        t1(1)=fact*t1(1)
        t1(2)=fact*t1(2)
        t1(3)=fact*t1(3)
        t2(1)=an(2)*t1(3)-an(3)*t1(2)
        t2(2)=an(3)*t1(1)-an(1)*t1(3)
        t2(3)=an(1)*t1(2)-an(2)*t1(1)
c
        vn=an(1)*v1+an(2)*v2+an(3)*v3
        vt1=t1(1)*v1+t1(2)*v2+t1(3)*v3
        vt2=t2(1)*v1+t2(2)*v2+t2(3)*v3
c
        al(1,k)=r*(vn-c)
        do i=2,m-1
          al(i,k)=r*vn
        end do
        al(m,k)=r*(vn+c)
c
        el(1,1,k)=el(1,1,k)+tmp4*vn
        el(1,2,k)=el(1,2,k)-tmp4*an(1)
        el(1,3,k)=el(1,3,k)-tmp4*an(2)
        el(1,4,k)=el(1,4,k)-tmp4*an(3)
        el(2,1,k)=-vt1
        el(2,2,k)= t1(1)
        el(2,3,k)= t1(2)
        el(2,4,k)= t1(3)
        el(2,5,k)= 0.d0
        el(3,1,k)=-vt2
        el(3,2,k)= t2(1)
        el(3,3,k)= t2(2)
        el(3,4,k)= t2(3)
        el(3,5,k)= 0.d0
        el(m,1,k)=el(m,1,k)-tmp4*vn
        el(m,2,k)=el(m,2,k)+tmp4*an(1)
        el(m,3,k)=el(m,3,k)+tmp4*an(2)
        el(m,4,k)=el(m,4,k)+tmp4*an(3)
c
        er(2,1,k)=er(2,1,k)-an(1)*c
        er(3,1,k)=er(3,1,k)-an(2)*c
        er(4,1,k)=er(4,1,k)-an(3)*c
        er(5,1,k)=er(5,1,k)-vn*c
        er(1,2,k)=0.d0
        er(2,2,k)=t1(1)
        er(3,2,k)=t1(2)
        er(4,2,k)=t1(3)
        er(5,2,k)=vt1
        er(1,3,k)=0.d0
        er(2,3,k)=t2(1)
        er(3,3,k)=t2(2)
        er(4,3,k)=t2(3)
        er(5,3,k)=vt2
        er(2,m,k)=er(2,m,k)+an(1)*c
        er(3,m,k)=er(3,m,k)+an(2)*c
        er(4,m,k)=er(4,m,k)+an(3)*c
        er(5,m,k)=er(5,m,k)+vn*c
c
      end do
c
      if (m.lt.0) then
        tol=1.d-14
        iflag=0
        do k=1,3
          do i1=1,m
          do i2=1,m
            sum=0.d0
            do j=1,m
              sum=sum+el(i1,j,k)*er(j,i2,k)
            end do
            if (i1.eq.i2) then
              if (dabs(sum-1.d0).gt.tol) then
                iflag=1
                write(6,*)'Error (eigenv3d) : i1,i2,k,sum=',i1,i2,k,sum
              end if
            else
              if (dabs(sum).gt.tol) then
                iflag=1
                write(6,*)'Error (eigenv3d) : i1,i2,k,sum=',i1,i2,k,sum
              end if
            end if
          end do
          end do
        end do
        if (iflag.ne.0) stop
      end if
c
      return
      end
c
c+++++++++++++++++++
c
      subroutine gdflux3d (md,m,aj,a0,a1,a2,a3,ul,ur,
     *                     dul,dur,al,el,er,alpha,
     *                     almax,f,method)
c
c calculate numerical flux given solutions on the left and right,
c ul and ur.
c
c al(k), el(k,.), er(.,k), k=1,2,..,m are the kth eigenvalue, left
c eigenvector and right eigenvector, respectively.
c
c a1, a2, and a3 are the mapping transformations.
c
      implicit real*8 (a-h,o-z)
      dimension ul(md),ur(md),al(m,3),el(m,m),er(m,m),alpha(m),f(m),
     *          dul(m),dur(m)
      include 'tzcommon.h'
c
      include 'mvars.h'
c      common / mvars / mh,mr,me,ieos,irxn,imult,islope,ifix
c*wdh* put data statements last
      data eps / 1.d-14 /
c
c..add on possible slope corrections
      do i=1,m
        ul(i)=ul(i)+dul(i)
        ur(i)=ur(i)+dur(i)
      end do
c
c..add on twilight zone stuff
      if( itz.eq.1 ) then
        do i=1,m
          ul(i)=ul(i)+0.5*tzdt*tzrhsl(i)
          ur(i)=ur(i)+0.5*tzdt*tzrhsr(i)
        end do
      end if
c
      if (method.eq.0) then
c
c..exact Riemann solver (ideal gas only)
        include 'Rsolve3d.h'
c
      elseif (method.eq.1) then
c
c..Roe's approximate Riemann solver
        include 'roe3d.h'
c
c     elseif (method.eq.2) then
c
c..Saltzman's approximate Riemann solver
c       include 'saltz2.h'
c
      elseif (method.eq.3) then
c
c..HLL approximate Riemann solver
        include 'hll3d.h'
c
      else
c
        write(6,*)'Error (gdflux) : invalid value for method'
        stop
c
      end if
c
c     write(6,*)'gdflux =>'
c     write(6,123)(f(i),i=1,m)
c 123 format(5(1x,1pe15.8))
c
c      write(6,*)f(1),f(2),f(5)
      return
      end
c
c+++++++++++++++++++++
c
      subroutine flux3d (md,m,a1,a2,a3,u,f)
c
c Compute flux, a1*f1(u)+a2*f2(u)+a3*f3(u), for 3D Reactive Euler equations,
c where u(1)=density, u(2),u(3),u(4)=momenta, u(5)=total energy, and
c u(5+i)=density*lambda(i) (lambda=mass fraction of species i)
c
      implicit real*8 (a-h,o-z)
      dimension u(md),f(m),dp(10)
c
      v1=u(2)/u(1)
      v2=u(3)/u(1)
      v3=u(4)/u(1)
      call getp3d (md,u,p,dp,0,te,ier)
c
      v=a1*v1+a2*v2+a3*v3
      do i=1,m
        f(i)=u(i)*v
      end do
      f(2)=f(2)+a1*p
      f(3)=f(3)+a2*p
      f(4)=f(4)+a3*p
      f(5)=f(5)+p*v
c
      return
      end
c
c+++++++++++++++++++++
c
      subroutine roeavg3d (md,m,a1,a2,a3,ul,ur,al,el,er)
c
c supply eigenvalues and eigenvectors using an appropriate average value
c of u=u(ul,ur) for Roe's Riemann solver
c
      implicit real*8 (a-h,o-z)
      dimension ul(md),ur(md),al(m),el(m,m),er(m,m),alaml(8),alamr(8),
     *          alam(8),an(3),dpl(10),dpr(10),dp(10),d(10)
      include 'mvars.h'
c      common / mvars / mh,mr,me,ieos,irxn
      common / junk / iflag
      ! *wdh* 100713 data c2eps / 1.d-6 /
      data c2eps,rhoMin / 1.d-6,1.d-3 /
c
      rad=dsqrt(a1**2+a2**2+a3**2)
      an(1)=a1/rad
      an(2)=a2/rad
      an(3)=a3/rad
c
c compute left and right states, and compute Roe average
      ! *wdh* 100713 rhol=ul(1)
      rhol=max(ul(1),rhoMin)
      v1l=ul(2)/rhol
      v2l=ul(3)/rhol
      v3l=ul(4)/rhol
      q2l=v1l**2+v2l**2+v3l**2
      call getp3d (md,ul,pl,dpl,mr+2,te,ier)
      hl=(ul(5)+pl)/rhol
c
      ! *wdh* 100713 rhor=ur(1)
      rhor=max(ur(1),rhoMin)
      v1r=ur(2)/rhor
      v2r=ur(3)/rhor
      v3r=ur(4)/rhor
      q2r=v1r**2+v2r**2+v3r**2
      call getp3d (md,ur,pr,dpr,mr+2,te,ier)
      hr=(ur(5)+pr)/rhor
c
      rl=dsqrt(rhol)
      rr=dsqrt(rhor)
      r=rl+rr
      v1=(rl*v1l+rr*v1r)/r
      v2=(rl*v2l+rr*v2r)/r
      v3=(rl*v3l+rr*v3r)/r
      h=(rl*hl+rr*hr)/r
      q2=v1**2+v2**2+v3**2
      vn=an(1)*v1+an(2)*v2+an(3)*v3
c
      do k=1,mr+me
        alaml(k)=ul(k+5)/rhol
        alamr(k)=ur(k+5)/rhor
        alam(k)=(rl*alaml(k)+rr*alamr(k))/r
      end do
c
      do k=1,mr+2
        dp(k)=.5d0*(dpl(k)+dpr(k))
      end do
c
c get derivatives (Glaister if ieos.ne.0)
c      if( ieos.ne.idealGasEOS ) then
c        tol=1.d-3
c        d(1)=ur(1)-ul(1)
c        d(2)=ur(5)-.5d0*rhor*q2r-(ul(5)-.5d0*rhol*q2l)
c        arg=d(1)**2+d(2)**2
c        do k=1,mr
c          d(k+2)=ur(k+5)-ul(k+5)
c          arg=arg+d(k+2)**2
c        end do
c        if (arg.gt.tol) then
c          theta=pr-pl-dp(1)*d(1)-dp(2)*d(2)
c          do k=1,mr
c            theta=theta-dp(k+2)*d(k+2)
c          end do
c          theta=theta/arg
c          do k=1,mr+2
c            dp(k)=dp(k)+d(k)*theta
c          end do
c        end if
c      end if
c
      sum=0.d0
      do k=1,mr
        sum=sum+alam(k)*dp(2+k)
      end do
      c2=dp(1)+(h-.5d0*q2)*dp(2)+sum
      c=dsqrt(max(c2,c2eps))
c
c eigenvalues
      al(1)=rad*(vn-c)
      do i=2,m-1
        al(i)=rad*vn
      end do
      al(m)=rad*(vn+c)
      if (iflag.ne.0.and.m.lt.0) write(99,111)rad,vn,c
  111 format(3(1x,1pe22.15))
c
c eigenvectors
      tmp1=dp(2)/(2.d0*c2)
      tmp3=h-q2+sum/dp(2)
      tmp4=.5d0/c
c
c begin with nonreactive contributions to el and er
      el(1,1)=-tmp1*tmp3+(c+vn)/(2*c)
      el(1,2)=-tmp1*v1-tmp4*an(1)
      el(1,3)=-tmp1*v2-tmp4*an(2)
      el(1,4)=-tmp1*v3-tmp4*an(3)
      el(1,5)= tmp1
      el(m,1)=-tmp1*tmp3+(c-vn)/(2*c)
      el(m,2)=-tmp1*v1+tmp4*an(1)
      el(m,3)=-tmp1*v2+tmp4*an(2)
      el(m,4)=-tmp1*v3+tmp4*an(3)
      el(m,5)= tmp1
c
      er(1,1)=1.d0
      er(2,1)=v1-an(1)*c
      er(3,1)=v2-an(2)*c
      er(4,1)=v3-an(3)*c
      er(5,1)=h-vn*c
      er(1,m)=1.d0
      er(2,m)=v1+an(1)*c
      er(3,m)=v2+an(2)*c
      er(4,m)=v3+an(3)*c
      er(5,m)=h+vn*c
c
c add on reactive contributions
      do i=1,mr+me
        ip5=i+5
        heat=0.d0
        if (i.le.mr) heat=-dp(2+i)/dp(2)
        el(1,ip5)=-tmp1*heat
        el(m,ip5)= el(1,ip5)
        er(ip5,1)= alam(i)
        er(ip5,m)= alam(i)
      end do
c
      return
      end
c
c+++++++++++++++++++++
c
      subroutine eigenr3d (md,m,a0,a1,a2,a3,ul,ur,all,alr,isign)
c
c compute C- or C+ characteristic for states ul and ur
c
      implicit real*8 (a-h,o-z)
      dimension ul(md),ur(md),an(3),dp(10)
      include 'mvars.h'
c      common / mvars / mh,mr,me,ieos,irxn
c
      r=dsqrt(a1**2+a2**2+a3**2)
      an(1)=a1/r
      an(2)=a2/r
      an(3)=a3/r
c
      v1=ul(2)/ul(1)
      v2=ul(3)/ul(1)
      v3=ul(4)/ul(1)
      q2=v1**2+v2**2+v3**2
      vn=an(1)*v1+an(2)*v2+an(3)*v3
      call getp3d (md,ul,p,dp,mr+2,te,ier)
      h=(ul(5)+p)/ul(1)
      c2=dp(1)+(h-.5d0*q2)*dp(2)
      do k=1,mr
        c2=c2+ul(5+k)*dp(2+k)/ul(1)
      end do
      if( c2.gt.0.d0 ) then
        all=a0+r*(vn+isign*dsqrt(c2))
      else
        all=0.d0
        alr=0.d0
        return
      end if
c
      v1=ur(2)/ur(1)
      v2=ur(3)/ur(1)
      v3=ur(4)/ur(1)
      q2=v1**2+v2**2+v3**2
      vn=an(1)*v1+an(2)*v2+an(3)*v3
      call getp3d (md,ur,p,dp,mr+2,te,ier)
      h=(ur(5)+p)/ur(1)
      c2=dp(1)+(h-.5d0*q2)*dp(2)
      do k=1,mr
        c2=c2+ur(5+k)*dp(2+k)/ur(1)
      end do
      if( c2.gt.0.d0 ) then
        alr=a0+r*(vn+isign*dsqrt(c2))
      else
        all=0.d0
        alr=0.d0
      end if
c
      return
      end
c
c+++++++++++++++++++++
c
      subroutine roespd3d (md,m,a1,a2,a3,ul,ur,spl,spr,ier)
c
c supply eigenvalues and eigenvectors using an approximate average value
c of u=u(ul,ur) for Roe's Riemann solver
c
      implicit real*8 (a-h,o-z)
      dimension ul(md),ur(md),alaml(8),alamr(8),
     *          alam(8),an(3),dpl(10),dpr(10),dp(10),d(10)
      include 'mvars.h'
      data c2eps / 1.d-6 /
c
      rad=dsqrt(a1**2+a2**2+a3**2)
      an(1)=a1/rad
      an(2)=a2/rad
      an(3)=a3/rad
c
c compute left and right states, and compute Roe average
      rhol=ul(1)
      v1l=ul(2)/rhol
      v2l=ul(3)/rhol
      v3l=ul(4)/rhol
      q2l=v1l**2+v2l**2+v3l**2
      call getp3d (md,ul,pl,dpl,mr+2,te,ier)
      hl=(ul(5)+pl)/rhol
c
      rhor=ur(1)
      v1r=ur(2)/rhor
      v2r=ur(3)/rhor
      v3r=ur(4)/rhor
      q2r=v1r**2+v2r**2+v3r**2
      call getp3d (md,ur,pr,dpr,mr+2,te,ier)
      hr=(ur(5)+pr)/rhor
c
      rl=dsqrt(rhol)
      rr=dsqrt(rhor)
      r=rl+rr
      v1=(rl*v1l+rr*v1r)/r
      v2=(rl*v2l+rr*v2r)/r
      v3=(rl*v3l+rr*v3r)/r
      h=(rl*hl+rr*hr)/r
      q2=v1**2+v2**2+v3**2
      vn=an(1)*v1+an(2)*v2+an(3)*v3
c
      suml=0.d0
      sumr=0.d0
      do k=1,mr
        alaml(k)=ul(k+5)/rhol
        alamr(k)=ur(k+5)/rhor
        suml=suml+alaml(k)*dpl(2+k)
        sumr=sumr+alamr(k)*dpr(2+k)
        alam(k)=(rl*alaml(k)+rr*alamr(k))/r
      end do
c
      c2l=c2l+suml
      c2r=c2r+sumr
      cl=dsqrt(max(c2l,c2eps))
      cr=dsqrt(max(c2r,c2eps))
c
      do k=1,mr+2
        dp(k)=.5d0*(dpl(k)+dpr(k))
      end do
c
c get derivatives (Glaister type averging for non-ideal EOS)
c      if (ieos.ne.idealGasEOS) then
c        tol=1.d-3
c        d(1)=ur(1)-ul(1)
c        d(2)=ur(4)-.5d0*rhor*q2r-(ul(4)-.5d0*rhol*q2l)
c        sqr=d(1)**2+d(2)**2
c        do k=1,mr
c          d(k+2)=ur(k+4)-ul(k+4)
c          sqr=sqr+d(k+2)**2
c        end do
c        if (sqr.gt.tol) then
c          theta=pr-pl-(dp(1)*d(1)+dp(2)*d(2))
c          do k=1,mr
c            theta=theta-dp(k+2)*d(k+2)
c          end do
c          theta=theta/sqr
c          do k=1,mr+2
c            dp(k)=dp(k)+d(k)*theta
c          end do
c        end if
c      end if
c
      sum=0.d0
      do k=1,mr
        sum=sum+alam(k)*dp(2+k)
      end do
      c2=dp(1)+(h-.5d0*q2)*dp(2)+sum
      c=dsqrt(max(c2,c2eps))
c
c approximate wave speeds
c      if (ieos.eq.idealGasEOS.or.
c     *   (ieos.eq.idealGasEOS.and.imult.eq.1)) then
c        beta=dsqrt(.5d0*dp(2)/(dp(2)+1.d0))
c      else if( ieos.eq.jwlEOS ) then
c        beta=1.d0
c      else if( ieos.eq.mieGruneisenEOS )then
c        ! *wdh* what should this be?
c        beta=1.d0 ! dsqrt(.5d0*dp(2)/(dp(2)+1.d0))
c      else
c        write(*,'("ERROR: ieos")')
c      end if
c      beta=dsqrt(.5d0*0.4d0/1.4d0)
      beta=.5d0
c      beta=1.d0
      spl=rad*min(vn-c,vnl-beta*cl)
      spr=rad*max(vn+c,vnr+beta*cr)
c
      return
      end
c
c+++++++++++++++++++++
c
      subroutine prim3d (md,ul,ur,an1,an2,an3)
c
c compute the primitive variables for the left and right states
c assuming an ideal gas
c
      implicit real*8 (a-h,o-z)
      dimension ul(md),ur(md),dp(10)
      include 'eosDefine.h'     ! define the EOS names, idealGasEOS, jwlEOS, mieGruneisenEOS, userDefinedEOS
      include 'mvars.h'
      common / mydata / gam,ht(0:2),at(2),prxn(2)
      common / prmdat / r(2),v(2),p(2),c(2),a(2),b(2),
     *                  gamma,gm1,gp1,em,ep
c
      ier=0
      ideriv=0
c
      if( ieos.ne.idealGasEOS )then
        write(6,*)'Error (prim3d): exact Riemann solver not implemented
     *     for desired EOS'
        stop
      end if
c
c some constants involving gamma
      gamma=gam
      gm1=gamma-1.d0
      gp1=gamma+1.d0
      em=0.5d0*gm1/gamma
      ep=0.5d0*gp1/gamma
c
c left state
      r(1)=ul(1)
      v(1)=(an1*ul(2)+an2*ul(3)+an3*ul(4))/ul(1)
      call getp3d (md,ul,p(1),dp,ideriv,te,ier)
c
c right state
      r(2)=ur(1)
      v(2)=(an1*ur(2)+an2*ur(3)+an3*ur(4))/ur(1)
      call getp3d (md,ur,p(2),dp,ideriv,te,ier)
c
c sound speeds and some constants
      do k=1,2
        a(k)=2.d0/(gp1*r(k))
        b(k)=gm1*p(k)/gp1
        c2=gamma*p(k)/r(k)
        if (c2.le.0.d0) then
          write(6,*)'Error (prim3d) : c2.le.0, k =',k
          write(6,*)'ul =',ul
          write(6,*)'ur =',ur
          stop
        end if
        c(k)=dsqrt(c2)
      end do
c
c check for vacuum state
      if (2.d0*(c(1)+c(2))/gm1.le.v(2)-v(1)) then
        write(6,*)'Error (prim3d) : vacuum state found'
        stop
      end if
c
      return
      end
c
c+++++++++++++++
c
      subroutine upstar3d (md,u,pm,vm,an0,an1,an2,an3,ustar,k)
c
      implicit real*8 (a-h,o-z)
      dimension u(md),ustar(md)
      include 'mvars.h'
c      common / mvars / mh,mr,me,ieos,irxn,imult,islope,ifix
      common / mydata / gam,ht(0:2),at(2),pr(2)
      common / prmdat / r(2),v(2),p(2),c(2),a(2),b(2),
     *                  gamma,gm1,gp1,em,ep
c
c set isign=1 for k=1 and isign=-1 for k=2
      isign=3-2*k
c
      if (pm.gt.p(k)) then
c shock cases
        sp=isign*(an0+v(k))-c(k)*dsqrt(ep*pm/p(k)+em)
        if (sp.ge.0.d0) then
c left (k=1) or right (k=2)
          do i=1,md
            ustar(i)=u(i)
          end do
          return
        else
c middle left (k=1) or middle right (k=2)
          rm=r(k)*(gp1*pm/p(k)+gm1)/(gm1*pm/p(k)+gp1)
        end if
      else
c rarefaction cases
        if (isign*(an0+v(k))-c(k).ge.0.d0) then
c left (k=1) or right (k=2)
          do i=1,md
            ustar(i)=u(i)
          end do
          return
        else
c left middle or sonic (k=1) or right middle or sonic (k=2)
          rm=r(k)*(pm/p(k))**(1.d0/gamma)
          if (isign*(an0+vm)-dsqrt(gamma*pm/rm).gt.0.d0) then
c sonic left (k=1) or right (k=2)
            arg=(2.d0+isign*gm1*(an0+v(k))/c(k))/gp1
            rm=r(k)*arg**(2.d0/gm1)
            vm=c(k)*arg*isign-an0
            pm=p(k)*arg**(2.d0*gamma/gm1)
          end if
        end if
      end if
c
c convert to conserved variables
      un=an1*u(2)+an2*u(3)+an3*u(4)
      vt1=(u(2)-un*an1)/u(1)
      vt2=(u(3)-un*an2)/u(1)
      vt3=(u(4)-un*an3)/u(1)
      q2=vm**2+(vt1**2+vt2**2+vt3**2)
      sum=0.d0
      do kr=1,mr
        ustar(5+kr)=rm*u(5+kr)/u(1)
        sum=sum+ht(kr)*ustar(5+kr)
      end do
      ustar(1)=rm
      ustar(2)=rm*(an1*vm+vt1)
      ustar(3)=rm*(an2*vm+vt2)
      ustar(4)=rm*(an3*vm+vt3)
      ustar(5)=pm/gm1+.5d0*rm*q2+sum
c
      return
      end
c
c+++++++++++++++++++++++
c
      subroutine rxnsrc3d( md,m,
     *                     nd1a,nd1b,n1a,n1b,
     *                     nd2a,nd2b,n2a,n2b,
     *                     nd3a,nd3b,n3a,n3b,
     *                     dr,u,tau,mask,nrwk,rwk,
     *                     niwk,iwk,maxnstep,ipc )
c
c Requires (5*mr+md+3)*ngrid for real workspace and 3*ngrid for
c integer workspace
c
      implicit real*8 (a-h,o-z)
      dimension u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,md),
     *          tau(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b),
     *          mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b),
     *          rwk(nrwk),iwk(niwk)
      include 'mvars.h'
c
      maxnstep=0
c
      ng=(n1b-n1a+1)*(n2b-n2a+1)
      if ((5*mr+md+3)*ng.gt.nrwk.or.3*ng.gt.niwk) then
        write(6,*)'Error (source) : nrwk or niwk too small'
        stop
      end if
c
      ltest=1
      lrk=ltest+ng
      lhk=lrk+ng
      luk=lhk+ng
      lwk=luk+md*ng
      lc=lwk+mr*ng
      lwpk=lc+3*mr*ng
c
      ljk=1
      lnstep=ljk+2*ng
c
      irc=6
      nlam=mr
c
c..loop over all layers
      do j3=n3a,n3b
        call rxnlayer( md,m,
     *                 nd1a,nd1b,n1a,n1b,
     *                 nd2a,nd2b,n2a,n2b,
     *                 nd3a,nd3b,
     *                 j3,dr,u,tau(nd1a,nd2a,j3),
     *                 mask(nd1a,nd2a,j3),rwk(ltest),rwk(lrk),
     *                 rwk(lhk),rwk(luk),rwk(lwk),rwk(lc),rwk(lwpk),
     *                 iwk(ljk),iwk(lnstep),maxnstep,irc,nlam,ng,ipc)
      end do
c
      return
      end
c
c+++++++++++++++++++++
c
      subroutine rxnlayer( md,m,
     *                     nd1a,nd1b,n1a,n1b,
     *                     nd2a,nd2b,n2a,n2b,
     *                     nd3a,nd3b,
     *                     j3,dr,u,tau,mask,test,rk,
     *                     hk,uk,wk,c,wpk,jk,nstep,maxnstep,
     *                     irc,nlam,ng,ipc)
c
      implicit real*8 (a-h,o-z)
      dimension u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,md),
     *          tau(nd1a:nd1b,nd2a:nd2b),mask(nd1a:nd1b,nd2a:nd2b),
     *          test(ng),rk(ng),hk(ng),uk(md,ng),wk(nlam,ng),
     *          c(nlam,ng,3),wpk(nlam,ng),jk(2,ng),
     *          nstep(n1a:n1b,n2a:n2b)
      common / srcdat / nb1a,nb1b,nb2a,nb2b,nb3a,nb3b,icount
c     data tol, tiny / 1.d-5, 1.d-14 /
      data tol, tiny / .0001d0, 1.d-14 /
c
      if( nlam.eq.1 ) then
        n=0
        do j2=n2a,n2b
        do j1=n1a,n1b
          nstep(j1,j2)=0
          if (mask(j1,j2).ne.0) then
            alam=u(j1,j2,j3,irc)/u(j1,j2,j3,1)
            if( alam.gt.1.d0-1.d-6 ) then
              u(j1,j2,j3,irc)=u(j1,j2,j3,1)
            else
              n=n+1
              jk(1,n)=j1
              jk(2,n)=j2
              rk(n)=0.d0
              hk(n)=dr
              do i=1,md
                uk(i,n)=u(j1,j2,j3,i)
              end do
              do i=1,nlam
                wk(i,n)=u(j1,j2,j3,i+irc-1)
              end do
            end if
          end if
        end do
        end do
      else
        n=0
        do j2=n2a,n2b
        do j1=n1a,n1b
          nstep(j1,j2)=0
          if( mask(j1,j2).ne.0 ) then
            n=n+1
            jk(1,n)=j1
            jk(2,n)=j2
            rk(n)=0.d0
            hk(n)=dr
            do i=1,md
              uk(i,n)=u(j1,j2,j3,i)
            end do
            do i=1,nlam
              wk(i,n)=u(j1,j2,j3,i+irc-1)
            end do
          end if
        end do
        end do
      end if
c
      if (n.eq.0) return
c
c..if more than one species or if away from equilibrium, then compute source
c  by integrating from r=0 to r=dr holding the density, velocities, and the
c  total energy fixed.
      itmax=100000
      hmin=dr/10000
c
      do it=1,itmax
c
        call rate3d (md,irc,nlam,n,uk,wk,wpk)
        do k=1,n
          do i=1,nlam
            c(i,k,1)=hk(k)*wpk(i,k)
            c(i,k,2)=wk(i,k)+.5d0*c(i,k,1)
          end do
        end do
c
        call rate3d (md,irc,nlam,n,uk,c(1,1,2),wpk)
        do k=1,n
          do i=1,nlam
            c(i,k,2)=hk(k)*wpk(i,k)
            c(i,k,3)=wk(i,k)+.75d0*c(i,k,2)
          end do
        end do
c
        call rate3d (md,irc,nlam,n,uk,c(1,1,3),wpk)
        do k=1,n
          j1=jk(1,k)
          j2=jk(2,k)
          test(k)=0.d0
          do i=1,nlam
            c(i,k,3)=hk(k)*wpk(i,k)
            test(k)=max(dabs(2*c(i,k,1)-6*c(i,k,2)+4*c(i,k,3)),test(k))
          end do
          test(k)=test(k)/(9*hk(k))
          alam=u(j1,j2,j3,6)/u(j1,j2,j3,1)
          if (alam.lt.0.99d0) then
            tau(j1,j2)=max(test(k),tau(j1,j2))
          end if
c icount determines the maximum number of sub-time steps taken for the
c source calculation.  The 26th bit in the mask array determines whether
c the grid point is covered by a finer grid.
          if (.not.btest(mask(j1,j2),26)) then
            if (j1.ge.nb1a.and.j1.le.nb1b.and.
     *          j2.ge.nb2a.and.j2.le.nb2b.and.
     *          j3.ge.nb3a.and.j3.le.nb3b) then
              icount=max(it,icount)
            end if
          end if
        end do
c
        n1=0
        do k=1,n
          if (test(k).lt.tol.or.hk(k).le.hmin .or. it.gt.500 ) then
c*wdh
c           we have converged or reached the min step size
            if( it.gt.500 )then
              write(*,*) 'WARNING (getsrc) number of iterations=',it
              if( ipc.eq.0 )then
                write(*,*) 'This error is at the predictor step'
              else
                write(*,*) 'This error is at the corrector step'
              end if
              j1=jk(1,k)
              j2=jk(2,k)
              alam=u(j1,j2,j3,irc)/u(j1,j2,j3,1)
              write(*,9000) j1,j2,mask(j1,j2),(u(j1,j2,j3,i),i=1,m)
c              write(*,9100) test(k),tol,hk(k),alam,
c     &          (u0(i,j1,j2),i=1,m)     
c 9100         format(1x,' test=',e9.2,' tol=',e9.2,' hk=',e9.2,
c     &               ' alam=',e10.3,' u0=',12(e11.3,1x))
              write(*,9200) (uk(i,k),i=1,5),(wk(i,k),i=1,nlam),
     &                      (c(i,k,1),i=1,nlam),(c(i,k,2),i=1,nlam),
     &                      (c(i,k,3),i=1,nlam)
 9200         format(' uk=',5(e11.2,1x),' wk=',(e11.2,1x),' c = ',
     &             3(e11.3,1x))
              if (.not.btest(mask(j1,j2),26)) then
                 write(*,*) ' Above pt is not hidden by refinement'              
              else
                 write(*,*) ' Above pt is hidden by refinement'              
              end if
              if( mask(j1,j2).lt.0 ) then
                 write(*,*) ' mask(j1,j2)<0'
              end if
              if (.not.btest(mask(j1,j2),31)) then
                 write(*,*) ' Above pt is not an overlap interp pt'              
              else
                 write(*,*) ' Above pt is an overlap interp pt' 
              end if
              stop
            end if
c*wdh
            rk(k)=rk(k)+hk(k)
            do i=1,nlam
              wk(i,k)=wk(i,k)+(2*c(i,k,1)+3*c(i,k,2)+4*c(i,k,3))/9.d0
            end do
            j1=jk(1,k)
            j2=jk(2,k)
            nstep(j1,j2)=nstep(j1,j2)+1
            maxnstep=max(nstep(j1,j2),maxnstep)
            if (rk(k).lt.dr-tiny) then
              n1=n1+1
              test(n1)=test(k)
              jk(1,n1)=jk(1,k)
              jk(2,n1)=jk(2,k)
              rk(n1)=rk(k)
              hk(n1)=hk(k)
              do i=1,md
                uk(i,n1)=uk(i,k)
              end do
              do i=1,nlam
                wk(i,n1)=wk(i,k)
              end do
            else
              j1=jk(1,k)
              j2=jk(2,k)
              do i=1,nlam
                u(j1,j2,j3,i+irc-1)=wk(i,k)
              end do              
            end if
          else
            n1=n1+1
            test(n1)=test(k)
            jk(1,n1)=jk(1,k)
            jk(2,n1)=jk(2,k)
            rk(n1)=rk(k)
            hk(n1)=hk(k)
            do i=1,md
              uk(i,n1)=uk(i,k)
            end do
            do i=1,nlam
              wk(i,n1)=wk(i,k)
            end do
          end if
        end do
c
        n=n1
        if (n.eq.0) return
c
        do k=1,n
          q=min(4.d0,max(0.1d0,dsqrt(tol/(2*max(test(k),tiny)))))
          hk(k)=min(dr-rk(k)+tiny,max(q*hk(k),hmin))
        end do
c
      end do
c
      write(6,*)'Error (getsrc) : itmax exceeded, n =',n
      do k=1,n
        j1=jk(1,k)
        j2=jk(2,k)
        write(*,9000) j1,j2,mask(j1,j2),(u(j1,j2,j3,i),i=1,md)
 9000   format(1x,' -> j1,j2=',i4,i4,' mask=',i15,' u=',10(e9.2,1x))
      end do
      stop
      end
c
c+++++++++++++++++++++
c
      subroutine rate3d (md,irc,nlam,n,u,w,wp)
c
      implicit real*8 (a-h,o-z)
      dimension u(md,n),w(nlam,n),wp(nlam,n),dp(10)
      include 'mvars.h'
      common / mydata / gam,ht(0:2),at(2),pr(2)
c
      if (nlam.gt.2) then
        write(6,*)'Error (rate) : nlam.gt.2'
        stop
      end if
c
      gm1=gam-1
      if (irxn.eq.arrhenius) then
        do k=1,n
          rho=u(1,k)
          u(irc,k)=w(1,k)
          call getp3d (md,u(1,k),p,dp,0,temp,ier)
c          temp=p/rho
          prod=w(1,k)/rho
          fuel=1.d0-prod
          ak1=pr(1)*dexp(-at(1)/temp)
c          ak1=.01d0
          wp(1,k)=rho*fuel*ak1
        end do
      elseif (irxn.eq.chainAndBranching) then
c ak1=reaction rate for chain-initiation
c ak2=reaction rate for chain-branching
        do k=1,n
          rho=u(1,k)
          prod=w(1,k)/rho
          rdcl=w(2,k)/rho
          fuel=1.d0-prod-rdcl
          u(irc,k)=prod
          u(irc+1,k)=rdcl
          call getp3d (md,u(1,k),p,dp,0,temp,ier)
c          temp=p/rho
          ak1=pr(1)*dexp(-at(1)/temp)
          ak2=pr(2)*dexp(-at(2)/temp)
          wp(1,k)=rho*rdcl
          wp(2,k)=rho*(fuel*(ak1+rdcl*ak2)-rdcl)
        end do
      elseif (irxn.eq.ignitionAndGrowth) then
        do k=1,n
          rho=u(1,k)
          u(irc,k)=w(1,k)
          call getp3d (md,u(1,k),p,dp,0,temp,ier)
c          temp=p/rho
          prod=w(1,k)/rho
          fuel=1.d0-prod
          ak1=pr(1)*dexp(-at(1)/temp)
          ak1=.01d0
          wp(1,k)=rho*fuel*ak1
        end do
      else
        write(6,*)'Error (rate) : irxm not supported'
        stop
      end if
c
      return
      end
c
c++++++++++++++++
c
      subroutine getp3d (md,u,p,dp,ideriv,te,ier)

c compute pressure p (ideriv=0,default) and derivatives of p (ideriv>0)
c also return temperature, te

      implicit real*8 (a-h,o-z)
      dimension u(md),dp(10)
      real*8 rho,e,p,q2
      integer ierr
c
      include 'eosDefine.h'  ! define the EOS names, idealGasEOS, jwlEOS, mieGruneisenEOS, userDefinedEOS
      include 'mvars.h'
      ! user defined EOS class pointer is here:
      include 'eosUserDefined.h'

c      common / mvars / mh,mr,me,ieos,irxn
      common / mydata / gam,ht(0:2),at(2),pr(2)
c
      common / mpidat / myid
c
      if (mr+2.gt.10) then
        write(6,*)'Error (getp3d) : mr too big'
        stop
      end if
c
      if (ieos.eq.idealGasEOS) then
        ier=0
        gm1=gam-1.d0
        q2=(u(2)/u(1))**2+(u(3)/u(1))**2+(u(4)/u(1))**2
        p=u(5)-.5d0*u(1)*q2
        do k=1,mr
          p=p-ht(k)*u(5+k)
        end do
        p=gm1*p
        te=p/u(1)   ! this is the temperature
        if (ideriv.gt.0) then
          dp(1)=0.d0
          dp(2)=gm1
          do k=1,mr
            dp(2+k)=-gm1*ht(k)
          end do
        end if

      else if( ieos.eq.userDefinedEOS ) then
        ! --- user defined EOS ---

        r  = u(1)
        ! u(2) = rho*u, u(3)=rho*v, u(4)=rho*w
        ! q2 = u^2+v^2+w^2 
        q2 = ( u(2)**2 + u(3)**2 + u(4)**2 )/(r**2)
        !  E = rho*e + .5*rho*( u^2+v^2+w^2 ) 
        e = u(5)/r -.5*q2 

        ierr = 0
        eosOption=1 ! get p=p(r,e)
        if (ideriv.gt.0) then
          eosDerivOption=1  ! compute dp/dr and dp/de
        else
          eosDerivOption=0 ! no derivatives needed
        end if

        ! derivOption=1: 
        ! dp(1) = dp/dr with rho*e=constant
        ! dp(2) = dp/d(rho*e) with rho=const
        iparEOS(1)=3 ! 3D
        call getUserDefinedEOS( r,e,p,dp, eosOption, eosDerivOption, u, 
     &                      iparEOS,rparEOS, userEOSDataPointer, ierr )

        te=p/u(1)   ! this is the temperature
        if (ideriv.gt.0) then
          do k=1,mr
            dp(2+k)=-gm1*ht(k)
          end do
        end if

      else
        write(6,*)'Error (getp3d) : ieos not supported'
        stop
      end if
c
      return
      end
c
c++++++++++++++++
c
      subroutine getcon3d( md,uprim,ucon,ier )
c
c conversion from primitive to conservative variables
c
      implicit real*8(a-h,o-z)
      dimension ucon(md), uprim(md)

      ! user defined EOS class pointer is here:
      include 'eosUserDefined.h'
      integer ierr
      real*8 dp(2)
c

      common / mydata / gam,ht(0:2),at(2),pr(2)
      include 'mvars.h'
      include 'eosDefine.h'
c
      ucon(1)=uprim(1)
      q2=uprim(2)**2+uprim(3)**2+uprim(4)**2
      do i=2,4
        ucon(i)=uprim(1)*uprim(i)
      end do

c
c..ideal single component 
      if( ieos.eq.idealGasEOS ) then
        gm1=gam-1.d0
        ucon(5)=uprim(5)/gm1
        do k=1,mr
          ucon(5)=ucon(5)+ht(k)*ucon(1)*uprim(5+k)
        end do
        ucon(5)=ucon(5)+.5d0*ucon(1)*q2
c
        do k=6,md-me
          ucon(k)=uprim(k)*uprim(1)
        end do
        ier=0
      else if( ieos.eq.userDefinedEOS ) then
        ! --- user defined EOS ---
        ! *** CHECK ME ***
        r = uprim(1)
        p = uprim(5)
        ierr = 0
        eosOption=0 ! get e=e(r,e)
        eosDerivOption=0 ! no derivatives needed
        iparEOS(1)=3 ! 3D
        call getUserDefinedEOS( r,e,p,dp, eosOption, eosDerivOption, u, 
     &                      iparEOS,rparEOS, userEOSDataPointer, ierr )


        ucon(5)=r*(e+0.5*q2)
        do i = 6,md
          ucon(i) = rho*uprim(i)
        end do

      else
        write(6,*)'Error (getcon3d): ieos not surpported'
      end if
c
      return
      end
c
c+++++++++++++++++++++
c
      subroutine upxtrp3d (md,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                     nd3a,nd3b,n3a,n3b,up)
c
c extrapolate up into ghost cells.
c
      implicit real*8 (a-h,o-z)
      dimension up(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,md)
c
c sides
      do j3=n3a,n3b
      do j2=n2a,n2b
        do i=1,md
          up(n1a-1,j2,j3,i)=3*up(n1a,j2,j3,i)-3*up(n1a+1,j2,j3,i)
     *                       +up(n1a+2,j2,j3,i)
          up(n1b+1,j2,j3,i)=3*up(n1b,j2,j3,i)-3*up(n1b-1,j2,j3,i)
     *                       +up(n1b-2,j2,j3,i)
          up(n1a-2,j2,j3,i)=2*up(n1a-1,j2,j3,i)-up(n1a,j2,j3,i)
          up(n1b+2,j2,j3,i)=2*up(n1b+1,j2,j3,i)-up(n1b,j2,j3,i)
        end do
      end do
      end do
c
c sides
      do j3=n3a,n3b
      do j1=nd1a,nd1b
        do i=1,md
          up(j1,n2a-1,j3,i)=3*up(j1,n2a,j3,i)-3*up(j1,n2a+1,j3,i)
     *                       +up(j1,n2a+2,j3,i)
          up(j1,n2b+1,j3,i)=3*up(j1,n2b,j3,i)-3*up(j1,n2b-1,j3,i)
     *                       +up(j1,n2b-2,j3,i)
          up(j1,n2a-2,j3,i)=2*up(j1,n2a-1,j3,i)-up(j1,n2a,j3,i)
          up(j1,n2b+2,j3,i)=2*up(j1,n2b+1,j3,i)-up(j1,n2b,j3,i)
        end do
      end do
      end do
c
c sides
      do j2=nd2a,nd2b
      do j1=nd1a,nd1b
        do i=1,md
          up(j1,j2,n3a-1,i)=3*up(j1,j2,n3a,i)-3*up(j1,j2,n3a+1,i)
     *                       +up(j1,j2,n3a+2,i)
          up(j1,j2,n3b+1,i)=3*up(j1,j2,n3b,i)-3*up(j1,j2,n3b-1,i)
     *                       +up(j1,j2,n3b-2,i)
          up(j1,j2,n3a-2,i)=2*up(j1,j2,n3a-1,i)-up(j1,j2,n3a,i)
          up(j1,j2,n3b+2,i)=2*up(j1,j2,n3b+1,i)-up(j1,j2,n3b,i)
        end do
      end do
      end do
c
      return
      end
c
c+++++++++++++++++++++
c
      subroutine mondat3d (md,m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                     nd3a,nd3b,n3a,n3b,r,dr,u,u0,mask)
c
      implicit real*8 (a-h,o-z)
      dimension u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,md),umin(9),umax(9),
     *          uavg(9),mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b),dp(10),
     *          u0(md)
      character*10 label(9)
      include 'mvars.h'
c      common / mvars / mh,mr,me,ieos,irxn
c
c can only handle 4 reacting species unless the "9" is made bigger
      nlam=min(mr,4)
c
      do i=1,m
        uavg(i)=0.d0
      end do
c
      icnt=0
      do j3=n3a,n3b
      do j2=n2a,n2b
      do j1=n1a,n1b
        if (mask(j1,j2,j3).gt.0) then
          icnt=icnt+1
          do i=1,md
            u0(i)=u(j1,j2,j3,i)
          end do
          rho=u0(1)
          v1=u0(2)/rho
          v2=u0(3)/rho
          v3=u0(4)/rho
          call getp3d (md,u0,p,dp,0,te,ier)
          if (icnt.eq.1) then
            umin(1)=rho
            umax(1)=rho
            umin(2)=v1
            umax(2)=v1
            umin(3)=v2
            umax(3)=v2
            umin(4)=v3
            umin(4)=v3
            umin(5)=p
            umax(5)=p
            do i=1,nlam
              alam=u0(5+i)/rho
              umin(5+i)=alam
              umax(5+i)=alam
            end do
          end if
          umin(1)=min(rho,umin(1))
          umax(1)=max(rho,umax(1))
          uavg(1)=uavg(1)+rho
          umin(2)=min(v1,umin(2))
          umax(2)=max(v1,umax(2))
          uavg(2)=uavg(2)+v1
          umin(3)=min(v2,umin(3))
          umax(3)=max(v2,umax(3))
          uavg(3)=uavg(3)+v2
          umin(4)=min(v3,umin(4))
          umax(4)=max(v3,umax(4))
          uavg(4)=uavg(4)+v3
          umin(5)=min(p,umin(5))
          umax(5)=max(p,umax(5))
          uavg(5)=uavg(5)+p
          do i=1,nlam
            alam=u0(5+i)/rho
            umin(5+i)=min(alam,umin(5+i))
            umax(5+i)=max(alam,umax(5+i))
            uavg(5+i)=uavg(5+i)+alam
          end do
        end if
      end do
      end do
      end do
c
      ng=(n1b-n1a+1)*(n2b-n2a+1)*(n3b-n3a+1)
      do i=1,m
        uavg(i)=uavg(i)/ng
      end do
c
      label(1)='density   '
      label(2)='x-velocity'
      label(3)='y-velocity'
      label(4)='z-velocity'
      label(5)='pressure  '
      do i=1,nlam
        write(label(5+i),100)i
  100   format('lambda(',i1,') ')
      end do
c
      write(6,200)r,dr
  200 format('=> r =',f10.6,',  dr =',1pe10.3)
      write(6,201)(label(i),i=1,m)
  201 format('       ',9(1x,a10))
      write(6,202)(umin(i),i=1,m)
  202 format(' min =',9(1x,1pe10.3))
      write(6,203)(uavg(i),i=1,m)
  203 format(' avg =',9(1x,1pe10.3))
      write(6,204)(umax(i),i=1,m)
  204 format(' max =',9(1x,1pe10.3))
c
      return
      end
c
c++++++++++++++++++++++++++++
c
      subroutine metrics3d (nd1a,nd1b,na1,n1b,nd2a,nd2b,n2a,n2b,
     *                    nd3a,nd3b,n3a,n3b,j3,rx,gv,det,
     *                    rx2,gv2,det2,a0,a1,aj,
     *                    move,icart,n1bm,n2bm,n3bm)
c
      implicit real*8 (a-h,o-z)
      dimension rx(nd1a:n1bm,nd2a:n2bm,nd3a:n3bm,3,3),
     *          gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,3),
     *          det(nd1a:n1bm,nd2a:n2bm,nd3a:n3bm),
     *          rx2(nd1a:n1bm,nd2a:n2bm,nd3a:n3bm,3,3),
     *          gv2(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,3),
     *          det2(nd1a:n1bm,nd2a:n2bm,nd3a:n3bm),
     *          a0(3,nd1a:nd1b,nd2a:nd2b),
     *          a1(3,3,nd1a:nd1b,nd2a:nd2b),
     *          aj(nd1a:nd1b,nd2a:nd2b)
c
      if (move.ne.0) then
        if (icart.eq.0) then
c
c moving, non-Cartesian grid
          do j2=nd2a,nd2b
          do j1=nd1a,nd1b
            do k=1,3
              a0(k,j1,j2)=0.d0
              do l=1,3
                a0(k,j1,j2)=a0(k,j1,j2)
     *               -rx(j1,j2,j3,k,l)*gv(j1,j2,j3,l)
     *               -rx2(j1,j2,j3,k,l)*gv2(j1,j2,j3,l)
                a1(k,l,j1,j2)=(rx(j1,j2,j3,k,l)+rx2(j1,j2,j3,k,l))/2.d0
              end do
              a0(k,j1,j2)=a0(k,j1,j2)/2.d0
            end do
            aj(j1,j2)=(det(j1,j2,j3)+det2(j1,j2,j3))/2.d0
          end do
          end do
c
        else
c
c moving, Cartesian Grid
          do j2=nd2a,nd2b
          do j1=nd1a,nd1b
            do k=1,3
              a0(k,j1,j2)=0.d0
              do l=1,3
                a0(k,j1,j2)=a0(k,j1,j2)
     *                -rx(nd1a,nd2a,nd3a,k,l)*gv(j1,j2,j3,l)
     *               -rx2(nd1a,nd2a,nd3a,k,l)*gv2(j1,j2,j3,l)
                a1(k,l,j1,j2)=(rx(nd1a,nd2a,nd3a,k,l)
     *                       +rx2(nd1a,nd2a,nd3a,k,l))/2.d0
              end do
              a0(k,j1,j2)=a0(k,j1,j2)/2.d0
            end do
            aj(j1,j2)=(det(nd1a,nd2a,nd3a)+det2(nd1a,nd2a,nd3a))/2.d0
          end do
          end do
        end if
c
      else
c
c fixed grid
        do j2=nd2a,nd2b
        do j1=nd1a,nd1b
          do k=1,3
            a0(k,j1,j2)=0.d0
          end do
        end do
        end do
c
        if( icart.eq.0 ) then
c
c non-Cartesian
          do j2=nd2a,nd2b
          do j1=nd1a,nd1b
            do k=1,3
            do l=1,3
              a1(k,l,j1,j2)=rx(j1,j2,j3,k,l)
            end do
            end do
            aj(j1,j2)=det(j1,j2,j3)
          end do
          end do
c
        else
c
c Cartesian
          do j2=nd2a,nd2b
          do j1=nd1a,nd1b
            do k=1,3
            do l=1,3
              a1(k,l,j1,j2)=rx(nd1a,nd2a,nd3a,k,l)
            end do
            end do
            aj(j1,j2)=det(nd1a,nd2a,nd3a)
          end do
          end do
c 
        end if
      end if
c
      return
      end
c
c++++++++++++++++++++++++++++
c
      subroutine layer3d( md,m,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *                    j3,j3temp,u,utemp,mask )
c
      implicit real*8 (a-h,o-z)
c
      dimension u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,md),
     *          utemp(nd1a:nd1b,nd2a:nd2b,3,md),
     *          mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
c
c..Since we only allow conservative correction for now this is trivial
      do j2=nd2a,nd2b
      do j1=nd1a,nd1b
        do i=1,md
          utemp(j1,j2,j3temp,i)=u(j1,j2,j3,i)
        end do
      end do
      end do
c
      return
      end
c
c++++++++++++++++++++++++++++
c
c 060304 -- fixed by dws to work in parallel case, removed   div(n1a-1,j2,2)=div(n1a,j2,2) etc.
c
      subroutine artvis3d( md,m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *     nd3a,nd3b,n3a,n3b,dr,ds,rx,u,up,mask,div,fx,ad,vismax,
     *     av,icart,n1bm,n2bm,n3bm)
c
      implicit real*8 (a-h,o-z)
      dimension rx(nd1a:n1bm,nd2a:n2bm,nd3a:n3bm,3,3),
     *     u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,md),
     *     up(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,md),
     *     mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b),
     *     div(nd1a:nd1b,nd2a:nd2b,2),fx(m),ad(md),ds(3)
c
c..add an artificial viscosity
      vismax=0.d0
      adMax=0.d0
      do i=1,m
        adMax=max(adMax,ad(i))
      end do
      do j3=n3a-1,n3b+1
        j3m1=j3-1
        if (icart.eq.0) then
          do j1=n1a-1,n1b+1
          do j2=n2a-1,n2b+1
            vxm=(rx(j1,j2,j3,1,1)*u(j1-1,j2,j3,2)
     *          +rx(j1,j2,j3,1,2)*u(j1-1,j2,j3,3)
     *          +rx(j1,j2,j3,1,3)*u(j1-1,j2,j3,4))/u(j1-1,j2,j3,1)
            vxp=(rx(j1,j2,j3,1,1)*u(j1+1,j2,j3,2)
     *          +rx(j1,j2,j3,1,2)*u(j1+1,j2,j3,3)
     *          +rx(j1,j2,j3,1,3)*u(j1+1,j2,j3,4))/u(j1+1,j2,j3,1)
            vym=(rx(j1,j2,j3,2,1)*u(j1,j2-1,j3,2)
     *          +rx(j1,j2,j3,2,2)*u(j1,j2-1,j3,3)
     *          +rx(j1,j2,j3,2,3)*u(j1,j2-1,j3,4))/u(j1,j2-1,j3,1)
            vyp=(rx(j1,j2,j3,2,1)*u(j1,j2+1,j3,2)
     *          +rx(j1,j2,j3,2,2)*u(j1,j2+1,j3,3)
     *          +rx(j1,j2,j3,2,3)*u(j1,j2+1,j3,4))/u(j1,j2+1,j3,1)
            vzm=(rx(j1,j2,j3,3,1)*u(j1,j2,j3-1,2)
     *          +rx(j1,j2,j3,3,2)*u(j1,j2,j3-1,3)
     *          +rx(j1,j2,j3,3,3)*u(j1,j2,j3-1,4))/u(j1,j2,j3-1,1)
            vzp=(rx(j1,j2,j3,3,1)*u(j1,j2,j3+1,2)
     *          +rx(j1,j2,j3,3,2)*u(j1,j2,j3+1,3)
     *          +rx(j1,j2,j3,3,3)*u(j1,j2,j3+1,4))/u(j1,j2,j3+1,1)
            div(j1,j2,2)= (vxp-vxm)/(2*ds(1))+(vyp-vym)/(2*ds(2))
     *                   +(vzp-vzm)/(2*ds(3))
            if (mask(j1,j2,j3).ne.0) then
              vismax=max(-div(j1,j2,2),vismax)
            end if
          end do
          end do
        else
          a11=rx(nd1a,nd2a,nd3a,1,1)/(2*ds(1))
          a22=rx(nd1a,nd2a,nd3a,2,2)/(2*ds(2))
          a33=rx(nd1a,nd2a,nd3a,3,3)/(2*ds(3))
          do j1=n1a-1,n1b+1
          do j2=n2a-1,n2b+1
            vxm=u(j1-1,j2,j3,2)/u(j1-1,j2,j3,1)
            vxp=u(j1+1,j2,j3,2)/u(j1+1,j2,j3,1)
            vym=u(j1,j2-1,j3,3)/u(j1,j2-1,j3,1)
            vyp=u(j1,j2+1,j3,3)/u(j1,j2+1,j3,1)
            vzm=u(j1,j2,j3-1,4)/u(j1,j2,j3-1,1)
            vzp=u(j1,j2,j3+1,4)/u(j1,j2,j3+1,1)
            div(j1,j2,2)=a11*(vxp-vxm)+a22*(vyp-vym)+a33*(vzp-vzm)
            if (mask(j1,j2,j3).ne.0) then
              vismax=max(-div(j1,j2,2),vismax)
            end if
          end do
          end do
        end if
        if (j3.ge.n3a) then
          do j2=n2a,n2b
          do j1=n1a,n1b
            if (mask(j1,j2,j3).ne.0.and.mask(j1,j2,j3m1).ne.0) then
              vis=av*max(0.d0,-(div(j1,j2,1)+div(j1,j2,2))/2)
              do i=1,m
                fx(i)=(vis+ad(i))*(u(j1,j2,j3,i)-u(j1,j2,j3m1,i))
                up(j1,j2,j3  ,i)=up(j1,j2,j3  ,i)-fx(i)
                up(j1,j2,j3m1,i)=up(j1,j2,j3m1,i)+fx(i)
              end do
c           if (j1.eq.6.and.j2.eq.0.and.(j3.eq.4.or.j3m1.eq.4)) then
c             write(6,340)j1,j3,(fx(i),i=1,m)
c  340        format('art3',2(1x,i2),5(1x,1pe15.8))
c           end if
            end if
          end do
          end do
          if (j3.le.n3b) then
            do j1=n1a-1,n1b
              j1p1=j1+1
              do j2=n2a,n2b
                if (mask(j1,j2,j3).ne.0.and.mask(j1p1,j2,j3).ne.0) then
                  vis=av*max(0.d0,-(div(j1,j2,2)+div(j1p1,j2,2))/2)
                  do i=1,m
                    fx(i)=(vis+ad(i))*(u(j1p1,j2,j3,i)-u(j1,j2,j3,i))
                    up(j1p1,j2,j3,i)=up(j1p1,j2,j3,i)-fx(i)
                    up(j1  ,j2,j3,i)=up(j1  ,j2,j3,i)+fx(i)
                  end do
c           if ((j1.eq.6.or.j1p1.eq.6).and.j2.eq.0.and.j3.eq.4) then
c             write(6,341)j1,j3,(fx(i),i=1,m)
c  341        format('art1',2(1x,i2),5(1x,1pe15.8))
c             write(6,*)vis,ad(5),u(j1p1,j2,j3,5),u(j1,j2,j3,5)
c           end if
                end if
              end do
            end do
            do j2=n2a-1,n2b
              j2p1=j2+1
              do j1=n1a,n1b
                if (mask(j1,j2,j3).ne.0.and.mask(j1,j2p1,j3).ne.0) then
                  vis=av*max(0.d0,-(div(j1,j2,2)+div(j1,j2p1,2))/2)
                  do i=1,m
                    fx(i)=(vis+ad(i))*(u(j1,j2p1,j3,i)-u(j1,j2,j3,i))
                    up(j1,j2p1,j3,i)=up(j1,j2p1,j3,i)-fx(i)
                    up(j1,j2  ,j3,i)=up(j1,j2  ,j3,i)+fx(i)
                  end do
c           if (j1.eq.6.and.(j2.eq.0.or.j2p1.eq.0).and.j3.eq.4) then
c             write(6,342)j1,j3,(fx(i),i=1,m)
c  342        format('art2',2(1x,i2),5(1x,1pe15.8))
c           end if
                end if
              end do
            end do
          end if
        end if
        do j1=n1a-1,n1b+1
        do j2=n2a-1,n2b+1
          div(j1,j2,1)=div(j1,j2,2)
        end do
        end do
      end do
      vismax=av*vismax+adMax
c
      return
      end
