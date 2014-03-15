      subroutine dudr2dc (md,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                   dr,ds1,ds2,r,rx,gv,det,rx2,gv2,det2,xy,u,
     *                   up,mask,ntau,tau,ad,mdat,dat,nrprm,rparam,
     *                   niprm,iparam,nrwk,rwk,niwk,iwk,idebug,ier)
c
      implicit real*8 (a-h,o-z)
      dimension rx(*),gv(*),det(*),rx2(*),gv2(*),det2(*),xy(*),
     *          u(nd1a:nd1b,nd2a:nd2b,md),up(nd1a:nd1b,nd2a:nd2b,md),
     *          mask(nd1a:nd1b,nd2a:nd2b),tau(ntau),ad(md),
     *          dat(nd1a:nd1b,nd2a:nd2b,*),rparam(nrprm),iparam(niprm),
     *          rwk(nrwk),iwk(niwk)
      dimension ds(2),almax(2),dp(10)
c
      include 'eosDefine.h'  ! define the EOS names, idealGasEOS, jwlEOS, mieGruneisenEOS
c
      common / mvars / mh,mr,me,ieos,irxn,imult,islope,ifix

      common / mydata / gam,ht(0:2),at(2),pr(2)
      common / srcdat / nb1a,nb1b,nb2a,nb2b,icount
      common / timing / tflux,tslope,tsource
      common / axidat / iaxi,j1axi(2),j2axi(2)
      include 'eosdat.h'
c      common / eosdat / omeg(2),ajwl(2,2),rjwl(2,2),vs0,ts0,
c     *                  fsvs0,zsvs0,vg0,fgvg0,zgvg0,cgcs,heat
      common / igdat / ra,eb,ex,ec,ed,ey,ee,eg,ez,al0,al1,al2,
     *                 ai,ag1,ag2
c
      include 'multiDat.h'
c      common / muldat / gami, gamr
c
c     if( idebug.gt.0 ) write(6,990)r,dr
c     write(6,990)r,dr
  990 format('** Starting dudr2d...t,dt =',f8.5,1x,1pe9.2)
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
c    iparam(1) =EOS model
c    iparam(2) =Reaction model     (iparam(2)=0 => no reaction model)
c    iparam(3) =move               is the grid moving (=0 => no)
c    iparam(4) =icart              Cartesian grid (=1 => yes)
c    iparam(5) =iorder             order of the method (=1 or 2)
c    iparam(6) =method             method of flux calculation (=0 => Roe solver)
c    iparam(7) =igrid              grid number
c    iparam(8) =level              AMR level for this grid
c    iparam(9) =nstep              number of steps taken
c    iparam(10)=icount             the maximum number of sub-time steps is determined
c    iparam(11)=iaxi               Axisymmetric problem? 0=>no, 1=>axisymmetric about grid line j1=j1axi
c                                                               2=>axisymmetric about grid line j2=j2axi
c    iparam(12)=j1axi(1) or j2axi(1)
c    iparam(13)=j1axi(2) or j2axi(2)
c    iparam(18)=imult             multicomponent calculation flag. 1=>yes, 0=>no
c    iparam(19)=islope             slope correction flag. 1=>primative, 0=>conservative
c    iparam(20)=ifix               flag to turn on pressure correction. 1=>on, 0=>off
c
c    rparam(1) =real(eigenvalue)   for time stepping
c    rparam(2) =imag(eigenvalue)   for time stepping
c    rparam(3) =viscosity          artificial viscosity
c
c   ideal equation of state (iparam(1)=0)
c    rparam(4)=gamma
c
c   JWL equation of state (iparam(1)=1)
c    rparam(51) to rparam(64) => mixture JWL EOS params
c
c   one-step reaction model (iparam(2)=1)
c    rparam(11)=heat release
c    rparam(12)=1/activation energy
c    rparam(13)=prefactor
c
c   chain branching reaction model (ipam(2)=2)
c    rparam(11)=heat release
c    rparam(12)=-absorbed energy
c    rparam(13)=1/activation energy(I)
c    rparam(14)=1/activation energy(B)
c    rparam(15)=prefactor(I)
c    rparam(16)=prefactor(B)
c
c   ignition and growth reaction model (iparam(2)=3)
c    rparam(11) to rparam(25) => IG rate params
c
c   timings
c    rparam(31)=tflux
c    rparam(32)=tslope
c    rparam(33)=tsourcer
c
c   multicomponent
c    rparam(41)=gammai
c    rparam(42)=gammar
c    rparam(43)=cvi (if <0 then we assume using trivial mixing rules ... )
c    rparam(44)=cvr (... otherwise we assume temperature and pressure equilibrium)
c
      ieos=iparam(1)
      imult=iparam(18)
      islope=iparam(19)
      mr=0  ! number of multicomponent species?
            ! NO this is number of "reactive" species. These are advected.
      me=0  ! number of extra species ?
            ! like vs, vg, etc
      if (ieos.eq.idealGasEOS.and.imult.eq.0) then
c..ideal EOS
        gam=rparam(4)
      elseif (ieos.eq.idealGasEOS.and.iparam(18).eq.1) then
c..ideal gas with multicomponent
        gami=rparam(41)
        gamr=rparam(42)
        cvi=rparam(43)
        cvr=rparam(44)
        mr=mr+1
      elseif (ieos.eq.jwlEOS) then
c..JWL EOS
        omeg(1)=rparam(51)
        ajwl(1,1)=rparam(52)
        ajwl(2,1)=rparam(53)
        rjwl(1,1)=rparam(54)
        rjwl(2,1)=rparam(55)
        omeg(2)=rparam(56)
        ajwl(1,2)=rparam(57)
        ajwl(2,2)=rparam(58)
        rjwl(1,2)=rparam(59)
        rjwl(2,2)=rparam(60)
        vs0=rparam(61)
        ts0=1.d0
        zsvs0=vs0*(ajwl(1,1)*dexp(-rjwl(1,1)*vs0)
     *            +ajwl(2,1)*dexp(-rjwl(2,1)*vs0))/omeg(1)
        fsvs0=zsvs0-ajwl(1,1)*dexp(-rjwl(1,1)*vs0)/rjwl(1,1)
     *            -ajwl(2,1)*dexp(-rjwl(2,1)*vs0)/rjwl(2,1)
        vg0=rparam(62)
        zgvg0=vg0*(ajwl(1,2)*dexp(-rjwl(1,2)*vg0)
     *            +ajwl(2,2)*dexp(-rjwl(2,2)*vg0))/omeg(2)
        fgvg0=zgvg0-ajwl(1,2)*dexp(-rjwl(1,2)*vg0)/rjwl(1,2)
     *             -ajwl(2,2)*dexp(-rjwl(2,2)*vg0)/rjwl(2,2)
        cgcs=rparam(63)
        heat=rparam(64)
        me=me+2
      else if( ieos.eq.mieGruneisenEOS )then
        gam=rparam(4)
        me=0
        ! other parameters should have been set
        if( .false. )then
          write(*,'(" dudr2d: mieGruneisen: alpha,beta,v0=",3f6.2)') 
     &      eospar(1),eospar(2),eospar(3)
        end if
      else
        write(6,*)'Error (dudr) : EOS model not supported'
        ier=1
        return
      end if
c
      irxn=iparam(2)
      if (irxn.eq.0) then
c..no reaction model
        ht(0)=0.d0
      elseif (irxn.eq.1.or.irxn.eq.7) then
c..one-step reaction model (irxn=7 for pressure dependent rate law)
        mr=mr+1
        ht(0)=0.d0
        ht(1)=-rparam(11)
        at(1)= rparam(12)
        pr(1)= rparam(13)
      elseif (irxn.eq.2) then
c..chain-branching reaction model
        mr=mr+2
        ht(0)=0.d0
        ht(1)=-rparam(11)
        ht(2)=-rparam(12)
        at(1)= rparam(13)
        at(2)= rparam(14)
        pr(1)= rparam(15)
        pr(2)= rparam(16)
      elseif (irxn.eq.3) then
c..ignition and growth
        mr=mr+1
        ra=rparam(11)
        eb=rparam(12)
        ex=rparam(13)
        ec=rparam(14)
        ed=rparam(15)
        ey=rparam(16)
        ee=rparam(17)
        eg=rparam(18)
        ez=rparam(19)
        al0=rparam(20)
        al1=rparam(21)
        al2=rparam(22)
        ai=rparam(23)
        ag1=rparam(24)
        ag2=rparam(25)
      else
        write(6,*)'Error (dudr) : Reaction model not supported'
        ier=2
        return
      end if
c
c..number of hydro variables (=number of space dimensions+2)
      mh=md-mr-me
      nd=2
      if (mh-2.ne.nd) then
        write(6,*)'Error (dudr) : currently two dimensions is assumed'
        ier=3
        return
      end if
c
c..sum of hydro and reaction and EOS variables (EOS variables are
c  being advected too???)
      m=mh+mr+me
c
c..this doesn't make much sense.  According to the calculation above m=md.
c  It appears that the EOS variables are being advected...
c
      av=rparam(3)
      move=iparam(3)
      icart=iparam(4)
      iorder=iparam(5)
      method=iparam(6)
      igrid=iparam(7)
      level=iparam(8)
      nstep=iparam(9)
      ifix=iparam(20)
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
c..monitor data (if idebug>0)
      if (idebug.gt.0) then
        call mondat2dc (md,m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                 r,dr,u,rwk,mask)
      end if
c
c..check for negative density, limit reacting species (probably not necessary)
      rhoMin=1.d-3
c*wdh      do j2=nd2a,nd2b
c*wdh      do j1=nd1a,nd1b
      md1a=max(nd1a,n1a-2)
      md1b=min(nd1b,n1b+2)
      md2a=max(nd2a,n2a-2)
      md2b=min(nd2b,n2b+2)

      do j2=md2a,md2b    ! for parallel -- only check 2 ghost lines
      do j1=md1a,md1b   
        if (mask(j1,j2).ne.0) then
          rho=u(j1,j2,1)
          if (rho.le.rhoMin) then
            write(6,100)j1,j2
  100       format(' ** Error (dudr) : very small density, j1,j2 =',
     *             2(1x,i4))
            u(j1,j2,1)=rhoMin
          end if
          do i=mh+1,mh+mr
            alam=u(j1,j2,i)/u(j1,j2,1)
            if    (alam.lt.0.d0) then
              if (alam.lt.-.01d0) then
                write(6,102)j1,j2,i,alam
  102           format(' ** Error (dudr) : alam < 0, j1,j2,i,alam =',
     *                 2(1x,i4),1x,i2,1x,1pe10.3)
              end if
              alam=0.d0
            elseif (alam.gt.1.d0) then
              if (alam.gt.1.01d0) then
                write(6,103)j1,j2,i,alam
  103           format(' ** Error (dudr) : alam > 1, j1,j2,i,alam =',
     *                 2(1x,i4),1x,i2,1x,1pe10.3)
              end if
              alam=1.d0
            end if
            u(j1,j2,i)=u(j1,j2,1)*alam
          end do
          do i=1,md
            rwk(i)=u(j1,j2,i)
          end do
          call getc2c (md,rwk,c2,ier)
          if (c2.lt.0.d0) then
            write(6,104)j1,j2
  104       format(' ** Error (dudr) : negative c^2, j1,j2 =',2(1x,i4))
            write(6,*)(rwk(i),i=1,m)
          end if

c          if (ieos.eq.0) then
c            do i=1,md
c              rwk(i)=u(j1,j2,i)
c            end do
c            ier=0
c            call getp2dc (md,rwk,pressure,dp,0,te,ier)
c            if (pressure.le.0.d0) then
c              write(6,104)j1,j2
c  104         format(' ** Error (dudr) : negative pressure, j1,j2 =',
c     *               2(1x,i4))
c              write(6,*)(u(j1,j2,i),i=1,md)
c              stop
c            end if
c          end if

        end if
      end do
      end do
c
      ngrid=(nd1b-nd1a+1)
c
c..split up real work space =>
c    nonreactive: nreq=(9*m+md+19)*ngrid+(4*m+9)*m+2*md
c    reactive   : nreq=nreq+(5*mr+md+3)*ngrid
      ldu=1
      ldu1=ldu+8*m*ngrid
      lul=ldu1+4*m
      lur=lul+md
      la0=lur+md
      la1=la0+4*ngrid
      laj=la1+8*ngrid
      lda0=laj+2*ngrid
      lvaxi=lda0+2*ngrid
      lh=lvaxi+3*ngrid
      leigal=lh+m*ngrid
      leigel=leigal+3*m
      leiger=leigel+2*m*m
      lalpha=leiger+2*m*m
      lu0=lalpha+m
      lfx=lu0+3*md*ngrid  ! *wdh* 050116 keep 3 levels
      lutemp=lfx+m
      lwrkp=lutemp+3*ngrid*m
      ldufix=lwrkp+3*ngrid*(mr+3)
      lfxfxl=ldufix+2*ngrid*m
      lfxfxr=lfxfxl+m
      lunew=lfxfxr+m
      ldiv=1                 ! workspace for div can overlap with du
      lrwk1=lunew+m
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
c    reactive   : nreq=nreq+2*ngrid
      liwk1=1
      if (mr.gt.0) then
        niwk1=2*ngrid
      else
        niwk1=1
      end if
      nreq=liwk1+niwk1-1
      if (nreq.gt.niwk) then
        ier=5
        return
      end if
c
c zero out tau, the maximum truncation error estimate returned by source
      if (mr.gt.0) then
        do k=1,ntau
          tau(k)=0.d0
        end do
      end if
c
c..filter out underflow in u
      tol=1.d-30
      do j2=nd2a,nd2b
      do j1=nd1a,nd1b
        do i=1,md
          if (dabs(u(j1,j2,i)).lt.tol) u(j1,j2,i)=0.d0
        end do
      end do
      end do
c
c..set almax=0
      almax(1)=0.d0
      almax(2)=0.d0
c
c..compute dudr
      call dudr2d0c (nd,md,m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *              dr,ds,r,rx,gv,det,rx2,gv2,det2,xy,u,up,mask,
     *              rwk(ldu),rwk(ldu1),rwk(lul),rwk(lur),rwk(la0),
     *              rwk(la1),rwk(laj),rwk(lda0),rwk(lvaxi),rwk(lh),
     *              tau,rwk(leigal),rwk(leigel),rwk(leiger),
     *              rwk(lalpha),rwk(lu0),rwk(lfx),rwk(ldiv),
     *              rwk(lutemp),rwk(lwrkp),rwk(ldufix),
     *              rwk(lfxfxl),rwk(lfxfxr),rwk(lunew),
     *              nrwk1,rwk(lrwk1),niwk1,iwk(liwk1),almax,
     *              mdat,dat,move,av,ad,maxnstep,vismax,icart,
     *              iorder,method,n1bm,n2bm,ier)
c
      if (ier.ne.0) return
c
c..compute real and imaginary parts of lambda, where the time stepping
c  is interpreted as u'=lambda*u
c
      iparam(10)=icount
c*wdh      write(23,123)r,n1a,n1b,icount
c*wdh  123 format(1x,f10.6,2(1x,i5),1x,i8)
c
c     param(1)=0.d0
      rparam(1)=4.d0*vismax
      rparam(2)=almax(1)/ds(1)+almax(2)/ds(2)
c     rparam(1)=min(max(4.d0*vismax,0.d0),1.d2)
c     rparam(2)=min(max(almax(1)/ds(1)+almax(2)/ds(2),0.d0),1.d2)
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
      if( idebug.gt.0 ) write(6,*)'...done: dudr2d'
c     write(6,*)'...done: dudr2d'
c
      return
      end
c
c+++++++++++++++
c
c
      subroutine dudr2d0c (nd,md,m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                    dr,ds,r,rx,gv,det,rx2,gv2,det2,xy,u,up,mask,
     *                    du,du1,ul,ur,a0,a1,aj,da0,vaxi,h,tau,al,el,
     *                    er,alpha,u0,fx,div,utemp,wrkp,dufix,fxfixl,
     *                    fxfixr,unew,nrwk,rwk,niwk,iwk,almax,
     *                    mdat,dat,move,av,ad,maxnstep,vismax,icart,
     *                    iorder,method,n1bm,n2bm,ier)
c
      implicit real*8 (a-h,o-z)
      dimension rx(nd1a:n1bm,nd2a:n2bm,2,2),gv(nd1a:nd1b,nd2a:nd2b,2),
     *          det(nd1a:n1bm,nd2a:n2bm),rx2(nd1a:n1bm,nd2a:n2bm,2,2),
     *          gv2(nd1a:nd1b,nd2a:nd2b,2),det2(nd1a:n1bm,nd2a:n2bm),
     *          xy(nd1a:nd1b,nd2a:nd2b,2),u(nd1a:nd1b,nd2a:nd2b,md),
     *          up(nd1a:nd1b,nd2a:nd2b,md),mask(nd1a:nd1b,nd2a:nd2b),
     *          du(m,nd1a:nd1b,2,2,2),du1(m,2,2),ul(md),ur(md),
     *          a0(2,nd1a:nd1b,2),a1(2,2,nd1a:nd1b,2),aj(nd1a:nd1b,2),
     *          vaxi(nd1a:nd1b,3),h(m,nd1a:nd1b),
     *          tau(nd1a:nd1b,nd2a:nd2b),al(m,3),el(m,m,2),er(m,m,2),
     & alpha(m),u0(md,nd1a:nd1b,3),almax(2),fx(m),ds(2),ad(md),
     *          div(nd1a:nd1b,2),rwk(nrwk),iwk(niwk),da0(nd1a:nd1b,2),
     *          dat(nd1a:nd1b,nd2a:nd2b,*),
     *          utemp(nd1a:nd1b,3,md),wrkp(nd1a:nd1b,3,mr+3),
     *          dufix(nd1a:nd1b,2,md),fxfixl(md),fxfixr(md),unew(md)
      dimension eye(2,2),dp(10)
      common / mvars / mh,mr,me,ieos,irxn,imult,islope,ifix
      common / axidat / iaxi,j1axi(2),j2axi(2)
      common / mydata / gam,ht(0:2),at(2),pr(2)

c..timings
      common / timing / tflux,tslope,tsource
c
c 2x2 identity
      data eye / 1.d0,0.d0,0.d0,1.d0 /
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c      open(unit = 666,file = 'E.dat',status='append')
c      j2=n2a+1
c      rener=0.d0
c      do j1=n1a,n1b
c         rener=rener+u(j1,j2,4)
c      end do
c      write(666,*)rener
c      close(unit = 666)
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c dat(nd1a:nd1b,nd2a:nd2b,.) is used to save extra grid data,
c if desired, which can then be plotted using plotStuff.  For
c example, here we compute c2, the sound speed squared.
      if (mdat.gt.0) then
        do j2=n2a,n2b
        do j1=n1a,n1b
          if (mask(j1,j2).eq.0) then
            dat(j1,j2,1)=0.d0
          else
            do i=1,md
              ul(i)=u(j1,j2,i)
            end do
            ier=0
            call getc2c (md,ul,c2,ier)
            dat(j1,j2,1)=c2
          end if
        end do
        end do
      end if
c
c..start with line at n2a-1
      j2=n2a-1
c
c..compute contributions for a moving grid, if necessary
      if (move.ne.0) then
        if (icart.eq.0) then
          do j1=nd1a,nd1b
            do k=1,2
              a0(k,j1,1)=(-rx(j1,j2,k,1)*gv(j1,j2,1)
     *                    -rx(j1,j2,k,2)*gv(j1,j2,2)
     *                    -rx2(j1,j2,k,1)*gv2(j1,j2,1)
     *                    -rx2(j1,j2,k,2)*gv2(j1,j2,2))/2.d0
              do l=1,2
                a1(k,l,j1,1)=(rx(j1,j2,k,l)+rx2(j1,j2,k,l))/2.d0
              end do
            end do
            aj(j1,1)=(det(j1,j2)+det2(j1,j2))/2.d0
          end do
        else
          do j1=nd1a,nd1b
            do k=1,2
              a0(k,j1,1)=(-rx(nd1a,nd2a,k,1)*gv(j1,j2,1)
     *                    -rx(nd1a,nd2a,k,2)*gv(j1,j2,2)
     *                    -rx2(nd1a,nd2a,k,1)*gv2(j1,j2,1)
     *                    -rx2(nd1a,nd2a,k,2)*gv2(j1,j2,2))/2.d0
              do l=1,2
                a1(k,l,j1,1)=(rx(nd1a,nd2a,k,l)
     *                       +rx2(nd1a,nd2a,k,l))/2.d0
              end do
            end do
            aj(j1,1)=(det(nd1a,nd2a)+det2(nd1a,nd2a))/2.d0
          end do
        end if
      else
        do j1=nd1a,nd1b
          a0(1,j1,1)=0.d0
          a0(2,j1,1)=0.d0
        end do
        if (icart.eq.0) then
          do j1=nd1a,nd1b
            a1(1,1,j1,1)=rx(j1,j2,1,1)
            a1(1,2,j1,1)=rx(j1,j2,1,2)
            a1(2,1,j1,1)=rx(j1,j2,2,1)
            a1(2,2,j1,1)=rx(j1,j2,2,2)
            aj(j1,1)=det(j1,j2)
          end do
        else
          do k=1,2
            do j1=nd1a,nd1b
              a1(1,1,j1,k)=rx(nd1a,nd2a,1,1)
              a1(1,2,j1,k)=rx(nd1a,nd2a,1,2)
              a1(2,1,j1,k)=rx(nd1a,nd2a,2,1)
              a1(2,2,j1,k)=rx(nd1a,nd2a,2,2)
              aj(j1,k)=det(nd1a,nd2a)
            end do
          end do
        end if
      end if
c
c..fill the utemp vector (utemp is a sliding copy of u, the conserved
c  variables if islope=0, else utemp is a sliding copy of w, the primitive
c  variables corresponding to u)
      if( islope.eq.0 ) then
        ! Here we fill with conserved quantities
        do j1=nd1a,nd1b
          do k2=1,2
            do i=1,md
              dufix(j1,k2,i)=0.d0 ! *wdh* from *jwb*
            end do
          end do
          do k2=-1,1
            do i=1,md
              utemp(j1,k2+2,i)=u(j1,j2+k2,i)
            end do
          end do
        end do
      else
        ! Here we fill with primative quantities
        do j1=nd1a,nd1b
          do k2=1,2
            ! *wdh* from *jwb* :
            do i=1,md
              dufix(j1,k2,i)=0.d0
            end do
          end do
          do k2=-1,1

            utemp(j1,k2+2,1)=u(j1,j2+k2,1)
            ul(1)=u(j1,j2+k2,1)
            do i=2,md
              utemp(j1,k2+2,i)=u(j1,j2+k2,i)/u(j1,j2+k2,1)
              ul(i)=u(j1,j2+k2,i)
            end do
            ! now we must fix pressure, and fill sound speed and 
            !   pressure partials workspace
            ier=0
            call getp2dc( md,ul,utemp(j1,k2+2,4),dp,mr+2,te,ier )
            if (ier.ne.0) then
              write(17,*)'Error (dudr2d0) : getp2d, ier=',ier
              write(17,*)'u =',(ul(i),i=1,m)
              ier=123
              return
            end if
            q2=utemp(j1,k2+2,2)**2+utemp(j1,k2+2,3)**2
            enth=(ul(4)+utemp(j1,k2+2,4))/ul(1)
            sum=0.d0
            do k=1,mr
              sum=sum+ul(4+k)*dp(2+k)/ul(1)
              wrkp(j1,k2+2,3+k)=dp(2+k)
            end do
c
c..sliding copy of c^2 and all dp's are held in the workspace wrkp
            wrkp(j1,k2+2,1)=dp(1)+(enth-.5d0*q2)*dp(2)+sum
            wrkp(j1,k2+2,2)=dp(1)
            wrkp(j1,k2+2,3)=dp(2)
          end do
        end do
      end if
c
      call ovtime (time0)
c
c..slope correction (now may be done in conservative or primitive variables)
      if (iorder.eq.2) then
        call slope2dc (md,m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,j2,dr,ds,
     *                a0(1,nd1a,1),a1(1,1,nd1a,1),aj(nd1a,1),xy,
     *                utemp,
     *                u,ul,mask(nd1a,j2),du(1,nd1a,1,1,1),du1,h,
     *                tau(nd1a,j2),al,el,er,nrwk,rwk,niwk,iwk,ier,
     *                wrkp)
        if (iaxi.gt.0) then
          do j1=n1a-1,n1b+1
            if (mask(j1,j2).ne.0) then
              vaxi(j1,2)= (u(j1,j2,3)+h(3,j1)/aj(j1,1))
     *                   /(u(j1,j2,1)+h(1,j1)/aj(j1,1))
            end if
          end do
        end if
      else
        do ks=1,2
        do kd=1,2
          do j1=nd1a,nd1b
            do i=1,m
              du(i,j1,kd,ks,1)=0.d0
            end do
          end do
        end do
        end do
        if (iaxi.gt.0) then
          do j1=n1a-1,n1b+1
            if (mask(j1,j2).ne.0) then
              vaxi(j1,2)=u(j1,j2,3)/u(j1,j2,1)
            end if
          end do
        end if
      end if
c

c**************** *wdh* 050116 start
c
c..compute and save center update (if islope=1, then utemp is primitive
c  and then the center update must be converted to conservative variables
c  for use in source2d)
      do j1=n1a-1,n1b+1
        if (mask(j1,j2).ne.0) then
          do i=1,md
c            u0(i,j1,2)=utemp(j1,j2,i)
            ul(i)=utemp(j1,2,i)
          end do
          do i=1,m
c            u0(i,j1,2)=u0(i,j1,2)+h(i,j1)/aj(j1,1)
            ul(i)=ul(i)+h(i,j1)/aj(j1,1)
          end do
          if( islope.eq.1 ) then
            ! we must do primative to conservative conversion
            call getconc( md,ul,ul,ier )
            if( ier.ne.0) then
              write(17,*)'Error (dudr2d0): getcon'
              stop
            end if
          end if
          do i=1,md
            u0(i,j1,2)=ul(i)
          end do
        end if
      end do
c**************** *wdh* 050116 end
c

      if (ier.ne.0) return
      call ovtime (time1)
      tslope=time1-time0
c
c..loop over lines j2=n2a:n2b+1
      do j2=n2a,n2b+1
        j2m1=j2-1
c
c..compute contributions for a moving grid, if necessary
        if (move.ne.0) then
          if (icart.eq.0) then
            do j1=nd1a,nd1b
              do k=1,2
                a0(k,j1,2)=(-rx(j1,j2,k,1)*gv(j1,j2,1)
     *                      -rx(j1,j2,k,2)*gv(j1,j2,2)
     *                      -rx2(j1,j2,k,1)*gv2(j1,j2,1)
     *                      -rx2(j1,j2,k,2)*gv2(j1,j2,2))/2.d0
                do l=1,2
                  a1(k,l,j1,2)=(rx(j1,j2,k,l)+rx2(j1,j2,k,l))/2.d0
                end do
              end do
              aj(j1,2)=(det(j1,j2)+det2(j1,j2))/2.d0
            end do
          else
            do j1=nd1a,nd1b
              do k=1,2
                a0(k,j1,2)=(-rx(nd1a,nd2a,k,1)*gv(j1,j2,1)
     *                      -rx(nd1a,nd2a,k,2)*gv(j1,j2,2)
     *                      -rx2(nd1a,nd2a,k,1)*gv2(j1,j2,1)
     *                      -rx2(nd1a,nd2a,k,2)*gv2(j1,j2,2))/2.d0
                do l=1,2
                  a1(k,l,j1,2)=(rx(nd1a,nd2a,k,l)
     *                         +rx2(nd1a,nd2a,k,l))/2.d0
                end do
              end do
              aj(j1,2)=(det(nd1a,nd2a)+det2(nd1a,nd2a))/2.d0
            end do
          end if
        else
          do j1=nd1a,nd1b
            a0(1,j1,2)=0.d0
            a0(2,j1,2)=0.d0
          end do
          if (icart.eq.0) then
            do j1=nd1a,nd1b
              a1(1,1,j1,2)=rx(j1,j2,1,1)
              a1(1,2,j1,2)=rx(j1,j2,1,2)
              a1(2,1,j1,2)=rx(j1,j2,2,1)
              a1(2,2,j1,2)=rx(j1,j2,2,2)
              aj(j1,2)=det(j1,j2)
            end do
          end if
        end if
c
c..fill the utemp vector (see comments above regarding the sliding
c  workspace utemp...)
        if( islope.eq.0 ) then
          ! Here we fill with conserved quantities
          do j1=nd1a,nd1b
            do k2=-1,1
              do i=1,md
                utemp(j1,k2+2,i)=u(j1,j2+k2,i)
c                utemp(j1,3,i)=u(j1,j2+1,i)
              end do
            end do
          end do
        else
          ! Here we fill with primative quantities
          do j1=nd1a,nd1b
            ! first update utemp amd wrkp vectors
            do i=1,md
              utemp(j1,1,i)=utemp(j1,2,i)
              utemp(j1,2,i)=utemp(j1,3,i)
            end do
            do k=1,mr+3
              wrkp(j1,1,k)=wrkp(j1,2,k)
              wrkp(j1,2,k)=wrkp(j1,3,k)
            end do
            ! now fill the vacant spots in utemp and wrk
            utemp(j1,3,1)=u(j1,j2+1,1)
            ul(1)=u(j1,j2+1,1)
            do i=2,md
              utemp(j1,3,i)=u(j1,j2+1,i)/u(j1,j2+1,1)
              ul(i)=u(j1,j2+1,i)
            end do
            ! now we must fix pressure and fill sound speed and
            !   pressure partials workspace
            ier=0
            call getp2dc( md,ul,utemp(j1,3,4),dp,mr+2,te,ier )
            if (ier.ne.0) then
              write(17,*)'Error (dudr2d0) : getp2d, ier=',ier
              write(17,*)'u =',(ul(i),i=1,m)
              ier=123
              return
            end if
            q2=utemp(j1,3,2)**2+utemp(j1,3,3)**2
            enth=(ul(4)+utemp(j1,3,4))/ul(1)
            sum=0.d0
            do k=1,mr
              sum=sum+ul(4+k)*dp(2+k)/ul(1)
              wrkp(j1,3,3+k)=dp(2+k)
            end do
            wrkp(j1,3,1)=dp(1)+(enth-.5d0*q2)*dp(2)+sum            
            wrkp(j1,3,2)=dp(1)
            wrkp(j1,3,3)=dp(2)
          end do
        end if
        call ovtime (time0)
c
c..slope correction (now may be done in conservative or primitive variables)
        if (iorder.eq.2) then
          call slope2dc (md,m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,j2,dr,ds,
     *                  a0(1,nd1a,2),a1(1,1,nd1a,2),aj(nd1a,2),xy,
     *                  utemp,
     *                  u,ul,mask(nd1a,j2),du(1,nd1a,1,1,2),du1,h,
     *                  tau(nd1a,j2),al,el,er,nrwk,rwk,niwk,iwk,ier,
     *                  wrkp)
          if (iaxi.gt.0) then
            do j1=n1a-1,n1b+1
              if (mask(j1,j2).ne.0) then
                vaxi(j1,3)= (u(j1,j2,3)+h(3,j1)/aj(j1,2))
     *                     /(u(j1,j2,1)+h(1,j1)/aj(j1,2))
              end if
            end do
          end if
        else
          do ks=1,2
          do kd=1,2
            do j1=nd1a,nd1b
              do i=1,m
                du(i,j1,kd,ks,2)=0.d0
              end do
            end do
          end do
          end do
          do j1=nd1a,nd1b
            do i=1,m
              h(i,j1)=0.d0
            end do
          end do
          if (iaxi.gt.0) then
            do j1=n1a-1,n1b+1
              if (mask(j1,j2).ne.0) then
                vaxi(j1,3)=u(j1,j2,3)/u(j1,j2,1)
              end if
            end do
          end if
        end if
c

c******************** *wdh* 050116 start
c
c..compute and save center update (if islope=1, then utemp is primitive
c  and then the center update must be converted to conservative variables
c  for use in source2d)
        do j1=n1a-1,n1b+1
          if (mask(j1,j2).ne.0) then
            do i=1,md
c              u0(i,j1,3)=utemp(j1,j2,i)
              ul(i)=utemp(j1,2,i)
            end do
            do i=1,m
c              u0(i,j1,3)=u0(i,j1,3)+h(i,j1)/aj(j1,2)
              ul(i)=ul(i)+h(i,j1)/aj(j1,2)
            end do
            if( islope.eq.1 ) then
              ! we must do primative to conservative conversion
              call getconc( md,ul,ul,ier )
              if( ier.ne.0) then
                write(17,*)'Error (dudr2d0): getcon'
                stop
              end if
            end if
            do i=1,md
              u0(i,j1,3)=ul(i)
            end do
          end if
        end do
c
c******************** *wdh* 050116 end


        if (ier.ne.0) return
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
            do i=1,md
c              ul(i)=u(j1,j2m1,i)
c              ur(i)=u(j1,j2  ,i)
              ul(i)=utemp(j1,1,i)
              ur(i)=utemp(j1,2,i)
            end do
            do i=1,m
              du1(i,2,2)=du(i,j1,2,2,1)/aj(j1,1)
              du1(i,2,1)=du(i,j1,2,1,2)/aj(j1,2)
            end do
c
c..gdflux2d handles input in conservative or primitive variables (depending on islope)
            call gdflux2dc (md,m,aj0,a20,a21,a22,ul,ur,du1(1,2,2),
     *                     du1(1,2,1),al,el,er,alpha,almax(2),fx,
     *                     method,ier)
            if (ier.ne.0) then
              write(17,*)'Error (dudr2d0) : flux2 : j1,j2 =',j1,j2
              stop
            end if
c
c..This is where the pressure fix comes in
            if( ifix.eq.1 ) then
              do i=1,md
                ul(i)=utemp(j1,1,i)
                ur(i)=utemp(j1,2,i)
              end do
              call fluxfixc( md,m,aj0,a20,a21,a22,ul,ur,du1(1,2,2),
     *                      du1(1,2,1),fxfixl,fxfixr,method,ier )
              if( ier.ne.0) then
                write(17,*)'Error (dudr2d0): fluxfix, ier=', ier
                ier=123
                return
              end if
            else
              do i=1,m
                fxfixr(i)=0.d0
                fxfixl(i)=0.d0
              end do
            end if
c
            do i=1,m
              dufix(j1,2,i)=fxfixr(i)/ds(2)
              dufix(j1,1,i)=dufix(j1,1,i)-fxfixl(i)/ds(2)
              up(j1,j2  ,i)=             +fx(i)/ds(2)
              up(j1,j2m1,i)=up(j1,j2m1,i)-fx(i)/ds(2)
            end do
          end if
        end do
c
        if (move.ne.0.and.icart.eq.0) then
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
        tflux=tflux+time2-time1
c
c..final source contribution for the line j2-1
        if (j2.gt.n2a) then
          if (icart.eq.0.and.move.ne.0) then
            do j1=n1a,n1b
              if (mask(j1,j2m1).ne.0) then
                do i=1,m
                  dufix(j1,1,i)=(dufix(j1,1,i)+u0(i,j1,2)*da0(j1,1))
     *                          /aj(j1,1)
                  up(j1,j2m1,i)=(up(j1,j2m1,i)+u0(i,j1,2)*da0(j1,1))
     *                          /aj(j1,1)
                  du(i,j1,1,1,1)=up(j1,j2m1,i)
                end do
              end if
            end do
          else
            do j1=n1a,n1b
              if (mask(j1,j2m1).ne.0) then
                do i=1,m
                  dufix(j1,1,i)=dufix(j1,1,i)/aj(j1,1)
                  up(j1,j2m1,i)=up(j1,j2m1,i)/aj(j1,1)
                  du(i,j1,1,1,1)=up(j1,j2m1,i)
                end do
              end if
            end do
          end if
          call ovtime (time0)
          ipc=1 ! indicates corrector *wdh*, also tau is computed only when ipc=1 *dws*
                ! *jwb* wants his initials in this code as well!!!
c
c..add axisymmetric contribution, if necessary
          if (iaxi.gt.0) then
            do j1=n1a,n1b
              if (mask(j1,j2m1).ne.0) then
                if (iaxi.eq.1) then
c (revolve about grid line j1)
                  if (j1.eq.j1axi(1).or.j1.eq.j1axi(2)) then
                    fact=a1(1,2,j1,1)*(vaxi(j1+1,2)-vaxi(j1-1,2))
     *                                /(2*ds(1))
                  else
                    fact=vaxi(j1,2)/xy(j1,j2m1,2)
                  end if
                else
c (revolve about grid line j2m1)
                  if (j2m1.eq.j2axi(1).or.j2m1.eq.j2axi(2)) then
                    fact=a1(2,2,j1,1)*(vaxi(j1,3)-vaxi(j1,1))
     *                                /(2*ds(2))
                  else
                    fact=vaxi(j1,2)/xy(j1,j2m1,2)
                  end if
                end if
c (contribution - could combine with free stream corrections, be careful about jacobian)
                ier=0
                call getp2dc (md,u0(1,j1,2),p,dp,0,te,ier)
                if (ier.ne.0) then
                  write(17,*)'Error (dudr2d0) : axisymmetry'
                  write(17,*)'j1,j2m1 =',j1,j2m1
                  write(17,*)'u =',(u0(i,j1,2),i=1,md)
                  stop
                end if
                up(j1,j2m1,4)=up(j1,j2m1,4)-fact*p
                do i=1,m
                  up(j1,j2m1,i)=up(j1,j2m1,i)-fact*u0(i,j1,2)
                  du(i,j1,1,1,1)=up(j1,j2m1,i)
                end do
              end if
            end do
          end if
c
c..Compute the energy fix and add it to up.
          if( ifix.eq.1 ) then
            do j1=n1a,n1b
              do i=1,m
                ul(i)=u(j1,j2m1,i)+dr*dufix(j1,1,i)
                ur(i)=u(j1,j2m1,i)
                unew(i)=u(j1,j2m1,i)+dr*up(j1,j2m1,i)
              end do
              call ecorrectc( md,unew,ur,ul,dr,ecorr,ier )
              if( ier.ne.0 ) then
                write(17,*)'Error (dudr2d0): ecorrect'
                stop
              end if
              up(j1,j2m1,4)=up(j1,j2m1,4)+ecorr
            end do
          end if
c
          call source2dc (md,m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,j2m1,dr,
     & u,u0(1,nd1a,2),du(1,nd1a,1,1,1),tau(nd1a,j2m1),
     *                   mask(nd1a,j2m1),nrwk,rwk,niwk,iwk,
     *                   maxnstep,ipc)
c
          call ovtime (time1)
          tsource=tsource+time1-time0
          do j1=n1a,n1b
            if (mask(j1,j2m1).ne.0) then
              do i=1,m
                up(j1,j2m1,i)=up(j1,j2m1,i)+du(i,j1,1,1,1)
              end do
              do i=m+1,md
                up(j1,j2m1,i)=0.d0
              end do
            else
              do i=1,md
                up(j1,j2m1,i)=0.d0
              end do
            end if
          end do
        end if
c
c..if j2.le.n2b, then compute fluxes in the s1 direction
        if (j2.le.n2b) then
c
c..reset a0, a1, aj and du and u0
          do j1=n1a-1,n1b+1
            do k=1,2
              a0(k,j1,1)=a0(k,j1,2)
              do i=1,md
                du(i,j1,k,1,1)=du(i,j1,k,1,2)
                du(i,j1,k,2,1)=du(i,j1,k,2,2)
              end do
            end do
c ************* *wdh* 050116 start
            do i=1,md
              u0(i,j1,1)=u0(i,j1,2)
              u0(i,j1,2)=u0(i,j1,3)
            end do
c ************* *wdh* 050116 end

          end do
          if (icart.eq.0) then
            do j1=n1a-1,n1b+1
              aj(j1,1)=aj(j1,2)
              a1(1,1,j1,1)=a1(1,1,j1,2)
              a1(1,2,j1,1)=a1(1,2,j1,2)
              a1(2,1,j1,1)=a1(2,1,j1,2)
              a1(2,2,j1,1)=a1(2,2,j1,2)
            end do
            if (move.ne.0) then
              do j1=n1a,n1b
                da0(j1,1)=da0(j1,2)
              end do
            end if
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
c            *wdh* from *jwb* :
          do k2=1,2
            do i=1,md
              dufix(n1a-1,k2,i)=0.d0
              dufix(n1b+1,k2,i)=0.d0
            end do
          end do
c
          do j1=n1a-1,n1b
            j1p1=j1+1
            if (mask(j1,j2).ne.0.and.mask(j1p1,j2).ne.0) then
              aj0=(aj(j1p1,1)+aj(j1,1))/2.d0
              a10=(a0(1,j1p1,1)+a0(1,j1,1))/2.d0
              a11=(a1(1,1,j1p1,1)+a1(1,1,j1,1))/2.d0
              a12=(a1(1,2,j1p1,1)+a1(1,2,j1,1))/2.d0
              do i=1,md
c                ul(i)=u(j1  ,j2,i)
c                ur(i)=u(j1p1,j2,i)
                ul(i)=utemp(j1  ,2,i)
                ur(i)=utemp(j1p1,2,i)
              end do
              do i=1,m
                du1(i,1,2)=du(i,j1  ,1,2,1)/aj(j1  ,1)
                du1(i,1,1)=du(i,j1p1,1,1,1)/aj(j1p1,1)
              end do
c
c..gdflux2d handles input in conservative or primitive variables (depending on islope)
              call gdflux2dc (md,m,aj0,a10,a11,a12,ul,ur,du1(1,1,2),
     *                       du1(1,1,1),al,el,er,alpha,almax(1),fx,
     *                       method,ier)
              if (ier.ne.0) then
                write(17,*)'Error (dudr2d0) : flux1 : j1,j2 =',j1,j2
                stop
              end if
c
              if( ifix.eq.1 ) then
                do i=1,md
                  ul(i)=utemp(j1  ,2,i)
                  ur(i)=utemp(j1p1,2,i)
                end do
                call fluxfixc( md,m,aj0,a10,a11,a12,ul,ur,du1(1,1,2),
     *                       du1(1,1,1),fxfixl,fxfixr,method,ier )
                if( ier.ne.0) then
                  write(17,*)'Error (dudr2d0): fluxfix, ier=', ier
                  ier=123
                  return
                end if
              else
                do i=1,m
                  fxfixr(i)=0.d0
                  fxfixl(i)=0.d0
                end do
              end if
              do i=1,m
                dufix(j1p1,2,i)=dufix(j1p1,2,i)+fxfixr(i)/ds(1)
                dufix(j1  ,2,i)=dufix(j1  ,2,i)-fxfixl(i)/ds(1)
                up(j1p1,j2,i)=up(j1p1,j2,i)+fx(i)/ds(1)
                up(j1  ,j2,i)=up(j1  ,j2,i)-fx(i)/ds(1)
              end do
            end if
          end do
c
          call ovtime (time1)
          tflux=tflux+time1-time0

          if (move.ne.0.and.icart.eq.0) then
c           write(6,*)'flux(1), j2=',j2
            do j1=n1a-1,n1b
              j1p1=j1+1
              if (mask(j1,j2).ne.0.and.mask(j1p1,j2).ne.0) then
                fact=(aj(j1p1,1)+aj(j1,1))*(a0(1,j1p1,1)+a0(1,j1,1))
     *                /(4.d0*ds(1))
c               write(6,*)'j1, fact(1) =',j1,fact
                da0(j1p1,1)=da0(j1p1,1)-fact
                da0(j1  ,1)=da0(j1  ,1)+fact
              end if
            end do
          end if

c**** *wdh* 050116 start
c* c
c* c..compute and save center update (if islope=1, then utemp is primitive
c* c  and then the center update must be converted to conservative variables
c* c  for use in source2d)
c*           do j1=n1a,n1b
c*             if (mask(j1,j2).ne.0) then
c*               do i=1,md
c* c                u0(i,j1)=utemp(j1,j2,i)
c*                 ul(i)=utemp(j1,2,i)
c*               end do
c*               do i=1,m                
c* c                u0(i,j1)=u0(i,j1)+h(i,j1)/aj(j1,1)
c*                 ul(i)=ul(i)+h(i,j1)/aj(j1,1)
c*               end do
c*               if( islope.eq.1 ) then
c*                 ! we must do primative to conservative conversion
c*                 call getconc( md,ul,ul,ier )
c*                 if( ier.ne.0) then
c*                   write(17,*)'Error (dudr2d0): getcon'
c*                   stop
c*                 end if
c*               end if
c*               do i=1,md
c*                 u0(i,j1)=ul(i)
c*               end do
c*             end if
c*           end do
c**** *wdh* 050116 end


c
c..add free stream correction to up (if a non-Cartesian grid)
          if (icart.eq.0) then
            if (move.eq.0) then
              do j1=n1a,n1b
                if (mask(j1,j2).ne.0) then
                  d1p=det(j1+1,j2)+det(j1,j2)
                  d1m=det(j1-1,j2)+det(j1,j2)
                  d2p=det(j1,j2+1)+det(j1,j2)
                  d2m=det(j1,j2-1)+det(j1,j2)
                  ier=0
                  call getp2dc (md,u0(1,j1,2),p,dp,0,te,ier)
                  if (ier.ne.0) then
                    write(17,*)'Error (dudr2d0) : free stream'
                    write(17,*)'j1,j2 =',j1,j2
                    write(17,*)'u =',(u0(i,j1,2),i=1,md)
                    stop
                  end if
                  do k=1,2
                    da= ((rx(j1+1,j2,1,k)+rx(j1,j2,1,k))*d1p
     *                  -(rx(j1-1,j2,1,k)+rx(j1,j2,1,k))*d1m)
     *                  /(4*ds(1))
     *                 +((rx(j1,j2+1,2,k)+rx(j1,j2,2,k))*d2p
     *                  -(rx(j1,j2-1,2,k)+rx(j1,j2,2,k))*d2m)
     *                  /(4*ds(2))
                    call flux2dc (md,m,eye(k,1),eye(k,2),u0(1,j1,2),p,
     & fx)
                    do i=1,m
                      dufix(j1,2,i)=dufix(j1,2,i)+da*fx(i)
                      up(j1,j2,i)=up(j1,j2,i)+da*fx(i)
                    end do
                  end do
                end if
              end do
            else
              do j1=n1a,n1b
                if (mask(j1,j2).ne.0) then
                  det0=det(j1,j2)+det2(j1,j2)
                  d1p=det(j1+1,j2)+det2(j1+1,j2)+det0
                  d1m=det(j1-1,j2)+det2(j1-1,j2)+det0
                  d2p=det(j1,j2+1)+det2(j1,j2+1)+det0
                  d2m=det(j1,j2-1)+det2(j1,j2-1)+det0
                  ier=0
                  call getp2dc (md,u0(1,j1,2),p,dp,0,te,ier)
                  if (ier.ne.0) then
                    write(17,*)'Error (dudr2d0) : free stream'
                    write(17,*)'j1,j2 =',j1,j2
                    write(17,*)'u =',(u0(i,j1,2),i=1,md)
                    stop
                  end if
                  do k=1,2
                    rx10=rx(j1,j2,1,k)+rx2(j1,j2,1,k)
                    rx20=rx(j1,j2,2,k)+rx2(j1,j2,2,k)
                    da= ((rx(j1+1,j2,1,k)+rx2(j1+1,j2,1,k)+rx10)*d1p
     *                  -(rx(j1-1,j2,1,k)+rx2(j1-1,j2,1,k)+rx10)*d1m)
     *                  /(16*ds(1))
     *                 +((rx(j1,j2+1,2,k)+rx2(j1,j2+1,2,k)+rx20)*d2p
     *                  -(rx(j1,j2-1,2,k)+rx2(j1,j2-1,2,k)+rx20)*d2m)
     *                  /(16*ds(2))
                    call flux2dc (md,m,eye(k,1),eye(k,2),u0(1,j1,2),p,
     & fx)
                    do i=1,m
                      dufix(j1,2,i)=dufix(j1,2,i)+da*fx(i)
                      up(j1,j2,i)=up(j1,j2,i)+da*fx(i)
                    end do
                  end do
                end if
              end do
            end if
          end if
c
        end if
c
c..update the dufix vectors
        do j1=nd1a,nd1b
          do i=1,m
            dufix(j1,1,i)=dufix(j1,2,i)
          end do
        end do
c     
c..bottom of main loop over lines
      end do
c
c..add an artificial viscosity
      vismax=0.d0
      adMax=0.d0
      do i=1,m
        adMax=max(adMax,ad(i))
      end do
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
              do i=1,m
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
                do i=1,m
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
      vismax=av*vismax+adMax

      ! *wdh* add viscous terms
      nwkViscous=m*(nd1b-nd1a+1)*8  ! work space available in du -- may need to fix this
      nda=min(nd1a,nd2b)
      ndb=max(nd1b,nd2b)
c      call addViscousTermsc(nd,md,m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,
c     & n2a,n2b,n1bm,n2bm,nda,ndb,dr,ds,rx,gv,det,rx2,gv2,det2,
c     & xy,u,up,mask,icart,  du,nwkViscous )

c
c..extrapolate to ghost cells
c     call upxtrp2dc (md,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,up)
      call zerobd2dc (md,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,up)
c
      return
      end
c
c++++++++++++++++
c
      subroutine slope2dc (md,m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,j2,dr,ds,
     *                    a0,a1,aj,xy,utemp,
     *                    u,u0,mask,du,du1,h,tau,al,el,er,
     *                    nrwk,rwk,niwk,iwk,ier,wrkp)
c
      implicit real*8 (a-h,o-z)
      dimension a0(2,nd1a:nd1b),a1(2,2,nd1a:nd1b),aj(nd1a:nd1b),
     *          mask(nd1a:nd1b),du(m,nd1a:nd1b,2,2),h(m,nd1a:nd1b),
     *          u(nd1a:nd1b,nd2a:nd2b,md),u0(md),du1(m,2,2),al(m,2),
     *          el(m,m,2),tau(nd1a:nd1b),er(m,m,2),ds(2),
     *          xy(nd1a:nd1b,nd2a:nd2b,2),rwk(nrwk),iwk(niwk),
     *          utemp(nd1a:nd1b,3,md),wrkp(nd1a:nd1b,3,mr+3)
      common / axidat / iaxi,j1axi(2),j2axi(2)
      common / mvars / mh,mr,me,ieos,irxn,imult,islope,ifix
c
      c0=dr/2.d0
      c1=c0/ds(1)
      c2=c0/ds(2)
c
c..source contribution
      do j1=n1a-1,n1b+1
        do i=1,m
          h(i,j1)=0.d0
          du(i,j1,1,1)=u(j1,j2,i)
c          du(i,j1,1,1)=utemp(j1,2,i)
        end do
      end do
      ipc=0 ! indicates predictor *wdh*, also tau is computed only when ipc=1 *dws*
      call source2dc (md,m,nd1a,nd1b,n1a-1,n1b+1,nd2a,nd2b,j2,c0,u,
     *               du(1,nd1a,1,1),h,tau,mask,nrwk,rwk,niwk,iwk,
     *               maxnstep,ipc)
c
c..if slope correction in primative variables and rxn is on we must convert h
      if( islope.eq.1.and.(irxn.eq.1.or.irxn.eq.7) ) then
        do j1=n1a-1,n1b+1
          h(4,j1)=h(6,j1)*wrkp(j1,2,mr+3) ! (mr+2)+1
          h(6,j1)=h(6,j1)/utemp(j1,2,1)
        end do
      end if
c
c..solution differences: s1-direction
      do j1=n1a-1,n1b+1
        do i=1,m
          du(i,j1,1,1)=utemp(j1,2,i)
        end do
      end do
      do j1=n1a,n1b+1
        do i=1,m
c          du(i,j1  ,1,1)=u(j1,j2,i)-u(j1-1,j2,i)
          du(i,j1  ,1,1)=utemp(j1,2,i)-utemp(j1-1,2,i)
          du(i,j1-1,1,2)=du(i,j1,1,1)
        end do
      end do
      do i=1,m
c        du(i,n1a-1,1,1)=u(n1a-1,j2,i)-u(n1a-2,j2,i)
c        du(i,n1b+1,1,2)=u(n1b+2,j2,i)-u(n1b+1,j2,i)
        du(i,n1a-1,1,1)=utemp(n1a-1,2,i)-utemp(n1a-2,2,i)
        du(i,n1b+1,1,2)=utemp(n1b+2,2,i)-utemp(n1b+1,2,i)
      end do
c
c..solution differences: s2-direction
      do j1=n1a-1,n1b+1
        do i=1,m
c          du(i,j1,2,1)=u(j1,j2  ,i)-u(j1,j2-1,i)
c          du(i,j1,2,2)=u(j1,j2+1,i)-u(j1,j2  ,i)
          du(i,j1,2,1)=utemp(j1,2,i)-utemp(j1,1,i)
          du(i,j1,2,2)=utemp(j1,3,i)-utemp(j1,2,i)
        end do
      end do
c
c..compute slope contribution and add it to source contribution
      do j1=n1a-1,n1b+1
        if (mask(j1).ne.0) then
c
          do i=1,md
c            u0(i)=u(j1,j2,i)
            u0(i)=utemp(j1,2,i)
          end do
          call eigenv2dc (md,m,a1(1,1,j1),u0,p,al,el,er,wrkp(j1,2,1),
     & ier)
c
          if (ier.ne.0) then
            write(17,*)'Error (slope2d) : j1,j2 =',j1,j2
            write(17,*)'u =',(u0(i),i=1,md)
            stop
          end if
c
c..add axisymmetric contribution, if necessary
c..for now this will NOT work with the primative variables slope correction!!
          if (iaxi.gt.0) then
            if (iaxi.eq.1) then
c (revolve about grid line j1)
              if (j1.eq.j1axi(1).or.j1.eq.j1axi(2)) then
                fact=a1(1,2,j1)*(u(j1+1,j2,3)/u(j1+1,j2,1)
     *                          -u(j1-1,j2,3)/u(j1-1,j2,1))/(2*ds(1))
              else
                fact=u(j1,j2,3)/(u(j1,j2,1)*xy(j1,j2,2))
              end if
            else
c (revolve about grid line j2)
              if (j2.eq.j2axi(1).or.j2.eq.j2axi(2)) then
                fact=a1(2,2,j1)*(u(j1,j2+1,3)/u(j1,j2+1,1)
     *                          -u(j1,j2-1,3)/u(j1,j2-1,1))/(2*ds(2))
              else
                fact=u(j1,j2,3)/(u(j1,j2,1)*xy(j1,j2,2))
              end if
            end if
c (contribution)
            h(4,j1)=h(4,j1)-fact*p
            do i=1,m
              h(i,j1)=h(i,j1)-fact*u(j1,j2,i)
            end do
          end if
c
c scale h
          tmp=c0*aj(j1)
          do i=1,m
            h(i,j1)=tmp*h(i,j1)
            do kd=1,2
              do ks=1,2
                du1(i,kd,ks)=du(i,j1,kd,ks)
                du(i,j1,kd,ks)=h(i,j1)
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
            if (alphal*alphar.gt.0.d0) then
              if (dabs(alphal).lt.dabs(alphar)) then
                alpha=alphal
              else
                alpha=alphar
              end if
              alpha=alpha*aj(j1)
              alam=c1*(a0(1,j1)+al(j,1))
              tmp=alam*alpha
              do i=1,m
                tmp2=tmp*er(i,j,1)
                h(i,j1)=h(i,j1)-tmp2
                du(i,j1,2,1)=du(i,j1,2,1)-tmp2
                du(i,j1,2,2)=du(i,j1,2,2)-tmp2
                du(i,j1,1,1)=du(i,j1,1,1)
     *                        -(min(alam,0.d0)+.5d0)*alpha*er(i,j,1)
                du(i,j1,1,2)=du(i,j1,1,2)
     *                        -(max(alam,0.d0)-.5d0)*alpha*er(i,j,1)
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
            if (alphal*alphar.gt.0.d0) then
              if (dabs(alphal).lt.dabs(alphar)) then
                alpha=alphal
              else
                alpha=alphar
              end if
              alpha=alpha*aj(j1)
              alam=c2*(a0(2,j1)+al(j,2))
              tmp=alam*alpha
              do i=1,m
                tmp2=tmp*er(i,j,2)
                h(i,j1)=h(i,j1)-tmp2
                du(i,j1,1,1)=du(i,j1,1,1)-tmp2
                du(i,j1,1,2)=du(i,j1,1,2)-tmp2
                du(i,j1,2,1)=du(i,j1,2,1)
     *                        -(min(alam,0.d0)+.5d0)*alpha*er(i,j,2)
                du(i,j1,2,2)=du(i,j1,2,2)
     *                        -(max(alam,0.d0)-.5d0)*alpha*er(i,j,2)
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
c++++++++++++++++
c
      subroutine eigenv2dc (md,m,a,u,p,al,el,er,c2,ier)
c
      implicit real*8 (a-h,o-z)
      dimension a(2,2),u(md),al(m,2),el(m,m,2),er(m,m,2)
      dimension an(2),at(2),dp(10),ucon(10)
      common / mvars / mh,mr,me,ieos,irxn,imult,islope,ifix
c
      include 'multiDat.h'
c      common / muldat / gami, gamr
      data c2min, c2eps / -1.d-2, 1.d-6 /
c
      ier=0
c
      if( islope.eq.1 ) then
        ! fill matricies for primative variables
        c=dsqrt(max(c2,c2eps))
c
c.. first fill with zeros
        do i=1,m
          do j=1,m
            er(i,j,1)=0.d0
            el(i,j,1)=0.d0
          end do
        end do
c.. now fill in nonzero components
c.. begin with direction-free part
        er(1,1,1)=u(1)
        er(4,1,1)=c2*u(1)
        er(1,2,1)=1.d0
        er(1,m,1)=u(1)
        er(4,m,1)=c2*u(1)
        do k=1,mr+me
          er(4+k,3+k,1)=1.d0
        end do
        tmp1=.5d0/(c2*u(1))
        el(1,4,1)=tmp1
        el(m,4,1)=tmp1
        el(2,1,1)=1.d0
        el(2,4,1)=-1.d0/c2
        do k=1,mr+me
          el(3+k,4+k,1)=1.d0
        end do
c     
c..make a copy
        do i=1,m
          do j=1,m
            el(i,j,2)=el(i,j,1)
            er(i,j,2)=er(i,j,1)
          end do
        end do
c     
c..add on directional part        
        do k=1,2
          r=dsqrt(a(k,1)**2+a(k,2)**2)
          an(1)=a(k,1)/r
          an(2)=a(k,2)/r
          v1=u(2)
          v2=u(3)
          vn=an(1)*v1+an(2)*v2
c
c..set eigenvalues
          al(1,k)=r*(vn-c)
          do i=2,m-1
            al(i,k)=r*vn
          end do
          al(m,k)=r*(vn+c)
c
          er(2,1,k)=-an(1)*c
          er(3,1,k)=-an(2)*c
          er(2,3,k)=-an(2)
          er(3,3,k)=an(1)
          er(2,m,k)=an(1)*c
          er(3,m,k)=an(2)*c
c
          tmp1=.5d0/c
          el(1,2,k)=-an(1)*tmp1
          el(3,2,k)=-an(2)
          el(m,2,k)=an(1)*tmp1
          el(1,3,k)=-an(2)*tmp1
          el(3,3,k)=an(1)
          el(m,3,k)=an(2)*tmp1
        end do
c
      else
        ! fill matricies for conservative variables
c..first compute direction-free part
        v1=u(2)/u(1)
        v2=u(3)/u(1)
        q2=v1**2+v2**2
c
        ier=0
        call getp2dc (md,u,p,dp,mr+2,te,ier)
        if (ier.ne.0) then
          write(17,*)'Error (eigenv2d) : getp2d, ier=',ier
          write(17,*)'u =',(u(i),i=1,m)
          ier=123
          return
        end if
        h=(u(4)+p)/u(1)
        sum=0.d0
        do k=1,mr
          sum=sum+u(4+k)*dp(2+k)/u(1)
        end do
        c2=dp(1)+(h-.5d0*q2)*dp(2)+sum
c     if (c2.lt.c2min*q2) then
cc      if (c2.lt.-1.d-1) then
cc        write(6,*)'Error (eigenv2d) : cannot compute sound speed'
cc        write(6,*)'u =',(u(i),i=1,m)
cc        write(6,*)'p, c2 =',p,c2
cc        ier=123
cc        return
cc      end if
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
        el(1,4,1)= tmp1
        el(3,1,1)= tmp2*tmp3
        el(3,2,1)= tmp2*v1
        el(3,3,1)= tmp2*v2
        el(3,4,1)=-tmp2
        el(m,1,1)= el(1,1,1)
        el(m,2,1)= el(1,2,1)
        el(m,3,1)= el(1,3,1)
        el(m,4,1)= tmp1
c
        er(1,1,1)=1.d0
        er(2,1,1)=v1
        er(3,1,1)=v2
        er(4,1,1)=h
        er(1,3,1)=1.d0
        er(2,3,1)=v1
        er(3,3,1)=v2
        er(4,3,1)=h+(sum-c2)/dp(2)
        er(1,m,1)=1.d0
        er(2,m,1)=v1
        er(3,m,1)=v2
        er(4,m,1)=h
c
c add on reactive and EOS contributions
        do i=1,mr+me
          ip3=i+3
          ip4=i+4
          heat=0.d0
          if (i.le.mr) heat=-dp(2+i)/dp(2)
          el(1,ip4,1)=-tmp1*heat
          el(2,ip4,1)= 0.d0
          el(3,ip4,1)= tmp2*heat
          el(m,ip4,1)= el(1,ip4,1)
          do j=1,m
            er(j,ip3,1)=0.d0
          end do
          er(4  ,ip3,1)=heat
          er(ip4,ip3,1)=1.d0
        end do
c
        do i=1,mr+me
          ip3=i+3
          ip4=i+4
          alam=u(ip4)/u(1)
          er(ip4,1,1)= alam
          er(ip4,2,1)= 0.d0
          er(ip4,3,1)= 0.d0
          er(ip4,m,1)= alam
          do j=1,m
            el(ip3,j,1)=alam*el(3,j,1)
          end do
          el(ip3,  1,1)=el(ip3,  1,1)-alam
          el(ip3,ip4,1)=el(ip3,ip4,1)+1.d0
        end do
c     
c..make a copy
        do i=1,m
          do j=1,m
            el(i,j,2)=el(i,j,1)
            er(i,j,2)=er(i,j,1)
          end do
        end do
c     
c..add on directional part
        do k=1,2
          r=dsqrt(a(k,1)**2+a(k,2)**2)
          an(1)=a(k,1)/r
          an(2)=a(k,2)/r
          vn=an(1)*v1+an(2)*v2
          at(1)= an(2)
          at(2)=-an(1)
          vt=at(1)*v1+at(2)*v2
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
          el(2,1,k)=-vt
          el(2,2,k)= at(1)
          el(2,3,k)= at(2)
          el(2,4,k)= 0.d0
          el(m,1,k)=el(m,1,k)-tmp4*vn
          el(m,2,k)=el(m,2,k)+tmp4*an(1)
          el(m,3,k)=el(m,3,k)+tmp4*an(2)
c     
          er(2,1,k)=er(2,1,k)-an(1)*c
          er(3,1,k)=er(3,1,k)-an(2)*c
          er(4,1,k)=er(4,1,k)-vn*c
          er(1,2,k)=0.d0
          er(2,2,k)=at(1)
          er(3,2,k)=at(2)
          er(4,2,k)= vt
          er(2,m,k)=er(2,m,k)+an(1)*c
          er(3,m,k)=er(3,m,k)+an(2)*c
          er(4,m,k)=er(4,m,k)+vn*c
c
        end do
c
c..may want to check the eigenvectors (for debugging)
        if (m.lt.0) then
          tol=1.d-14
          iflag=0
          do k=1,2
          do i1=1,m
          do i2=1,m
            sum=0.d0
            do j=1,m
              sum=sum+el(i1,j,k)*er(j,i2,k)
            end do
            if (i1.eq.i2) then
              if (dabs(sum-1.d0).gt.tol) then
                iflag=1
                write(6,*)'Error (eigenv2d) : i1,i2,k,sum=',i1,i2,k,sum
              end if
            else
              if (dabs(sum).gt.tol) then
                iflag=1
                write(6,*)'Error (eigenv2d) : i1,i2,k,sum=',i1,i2,k,sum
              end if
            end if
          end do
          end do
          end do
          if (iflag.ne.0) stop
        end if
      endif
c
      return
      end
c
c++++++++++++++++
c
      subroutine getc2c (md,u,c2,ier)
c
      implicit real*8 (a-h,o-z)
      dimension u(md),dp(10),dp1(10)

      include 'eosDefine.h'  ! define the EOS names, idealGasEOS, jwlEOS, mieGruneisenEOS
      common / mvars / mh,mr,me,ieos,irxn,imult,islope,ifix
c
      ier=0
c
c..first compute direction-free part
      v1=u(2)/u(1)
      v2=u(3)/u(1)
      q2=v1**2+v2**2
c
      ier=0
      call getp2dc (md,u,p,dp,mr+2,te,ier)
      if (ier.ne.0) then
        write(17,*)'Error (getc2) : getp2d, ier=',ier
        write(17,*)'u =',(u(i),i=1,md)
        ier=123
        return
      end if
      h=(u(4)+p)/u(1)
      sum=0.d0
      do k=1,mr
        sum=sum+u(4+k)*dp(2+k)/u(1)
      end do
      c2=dp(1)+(h-.5d0*q2)*dp(2)+sum
c

c check derivatives dp(.) and calculation of sound speed
      if (ieos.eq.jwlEOS.and.md.lt.0) then
        delta=1.d-7
        tol=1.d-5
        r=u(1)
        e=u(4)-.5d0*(u(2)**2+u(3)**2)/r
        y=u(5)
        vs=u(6)
        vg=u(7)
        iform=0
        r=r+delta
        call geteos (r,e,y,vs,vg,p1,dp1,iform,ier)
c       write(55,*)'Info (getc2) : dp(1),diff =',
c    *              dp(1),dabs((p1-p)/delta-dp(1))
        if (dabs((p1-p)/delta-dp(1)).gt.tol) then
          write(6,*)'Info (getc2) : (p1-p)/delta, dp(1) =',
     *              (p1-p)/delta,dp(1)
          pause
        end if
        r=r-delta
        e=e+delta
        call geteos (r,e,y,vs,vg,p1,dp1,iform,ier)
c       write(55,*)'Info (getc2) : dp(2),diff =',
c    *              dp(2),dabs((p1-p)/delta-dp(2))
        if (dabs((p1-p)/delta-dp(2)).gt.tol) then
          write(6,*)'Info (getc2) : (p1-p)/delta, dp(2) =',
     *              (p1-p)/delta,dp(2)
          pause
        end if
        e=e-delta
        y=y+delta
        call geteos (r,e,y,vs,vg,p1,dp1,iform,ier)
c       write(55,*)'Info (getc2) : dp(3),diff =',
c    *              dp(3),dabs((p1-p)/delta-dp(3))
        if (dabs((p1-p)/delta-dp(3)).gt.tol) then
          write(6,*)'Info (getc2) : (p1-p)/delta, dp(3) =',
     *              (p1-p)/delta,dp(3)
          pause
        end if
        y=y-delta
        e0=e/r
        alam=y/r
        r=r+delta
        y=r*alam
        iform=-1
        call geteos (r,e1,y,vs,vg,p,dp1,iform,ier)
        dedr=(e1/r-e0)/delta
        r=r-delta
        y=r*alam
        p=p+delta
        call geteos (r,e1,y,vs,vg,p,dp1,iform,ier)
        dedp=(e1/r-e0)/delta
        p=p-delta
        c2prim=(p/r**2-dedr)/dedp
c       write(55,*)c2,dabs(c2prim-c2)
        if (dabs(c2prim-c2).gt.tol) then
          write(6,*)'Info (getc2) : c2_prim, c2 =',
     *              c2prim,c2
          pause
        end if
      end if
c

      return
      end
c
c+++++++++++++++++++
c
      subroutine gdflux2dc (md,m,aj,a0,a1,a2,ul,ur,dul,dur,al,el,er,
     *                     alpha,almax,f,method,ier)
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
      dimension ul(md),ur(md),dul(m),dur(m),al(m,3),el(m,m),er(m,m),
     *          alpha(m),f(m),dpl(10),dpr(10)
c
      include 'eosDefine.h'  ! define the EOS names, idealGasEOS, jwlEOS, mieGruneisenEOS
c
      common / mvars / mh,mr,me,ieos,irxn,imult,islope,ifix
c
      include 'multiDat.h'
c      common / muldat / gami, gamr
      data eps / 1.d-14 /
c
c these are possible slope corrections
      do i=1,m
        ul(i)=ul(i)+dul(i)
        ur(i)=ur(i)+dur(i)
      end do
      if( islope.eq.1 ) then
        ! We must perform a conversion back to conservative variables
        call getconc( md,ul,ul,ier1 )
        call getconc( md,ur,ur,ier2 )
        if( ier1.ne.0 ) then
          write(17,*)'Error (gdflux): getcon'
          stop
        end if
        if( ier2.ne.0 ) then
          write(17,*)'Error (gdflux): getcon'
          stop
        end if
      endif
c
c update eos for left and right states
      ier1=-1
      call getp2dc (md,ul,pl,dpl,mr+2,te,ier1)
      ier2=-1
      call getp2dc (md,ur,pr,dpr,mr+2,te,ier2)
c
c check if update failed in which case don't use slope correction
c This logic needs to be looked over by dws and jwb
c   because it is WRONG for multicomponent case
      if (ier1.ne.0.or.ier2.ne.0) then
c
        if (islope.eq.1) then
          write(6,*)'Error (gdflux) : conservative-primitive mismatch'
          stop
        end if
c
        do i=1,m
          ul(i)=ul(i)-dul(i)
          ur(i)=ur(i)-dur(i)
        end do
        ier=0
        call getp2dc (md,ul,pl,dpl,mr+2,te,ier)
        if (ier.ne.0) then
          write(17,*)'Error (gdflux2d) : computing pl'
          return
        end if
        ier=0
        call getp2dc (md,ur,pr,dpr,mr+2,te,ier)
        if (ier.ne.0) then
          write(17,*)'Error (gdflux2d) : computing pr'
          return
        end if
      end if
c
c..At this stage, the flux is computed with left and right states
c  in conservative variables whether the slope correction was done
c  primitive variables or not.
c
      if     (method.eq.0) then
c
c..exact Riemann solver (ideal gas only)
        if (ieos.eq.idealGasEOS.and.mr.le.2.and.imult.ne.1) then
          include 'Rsolve2d.h'
        else
          write(6,*)'Error (gdflux) : exact Riemann solver not',
     *              ' supported'
        end if
c
      elseif (method.eq.1) then
c
c..Roe's approximate Riemann solver
        include 'roe2d.h'
c
c     elseif (method.eq.2) then
c
c..Saltzman's approximate Riemann solver
c       include 'saltz2.h'
c
      elseif (method.eq.3) then
c
c..HLL approximate Riemann solver
        include 'hll2d.h'
c
      else
c
        write(6,*)'Error (gdflux) : invalid value for method'
        stop
c
      end if
c
      return
      end
c
c+++++++++++++++++++++
c
      subroutine flux2dc (md,m,a1,a2,u,p,f)
c
c Compute flux, a1*f1(u)+a2*f2(u), for 2D Reactive Euler equations,
c where u(1)=density, u(2),u(3)=momenta, u(4)=total energy, and
c u(4+i)=density*lambda(i) (lambda=mass fraction of species i)
c
      implicit real*8 (a-h,o-z)
      dimension u(md),f(m),dp(10)
c
      v1=u(2)/u(1)
      v2=u(3)/u(1)
c
      v=a1*v1+a2*v2
      do i=1,m
        f(i)=u(i)*v
      end do
      f(2)=f(2)+a1*p
      f(3)=f(3)+a2*p
      f(4)=f(4)+p*v
c
      return
      end
c
c+++++++++++++++++++++
c
      subroutine roeavg2dc (md,m,a1,a2,ul,ur,al,el,er,pl,pr,
     *                     dpl,dpr,ier)
c
c supply eigenvalues and eigenvectors using an appropriate average value
c of u=u(ul,ur) for Roe's Riemann solver
c
      implicit real*8 (a-h,o-z)
      dimension ul(md),ur(md),al(m),el(m,m),er(m,m),alaml(8),alamr(8),
     *          alam(8),an(2),dpl(10),dpr(10),dp(10),d(10)
c
      include 'eosDefine.h'  ! define the EOS names, idealGasEOS, jwlEOS, mieGruneisenEOS
      common / mvars / mh,mr,me,ieos,irxn,imult,islope,ifix
      data c2eps / 1.d-6 /
c

      rad=dsqrt(a1**2+a2**2)
      an(1)=a1/rad
      an(2)=a2/rad
c
c compute left and right states, and compute Roe average
      rhol=ul(1)
      v1l=ul(2)/rhol
      v2l=ul(3)/rhol
      q2l=v1l**2+v2l**2
      hl=(ul(4)+pl)/rhol
c
      rhor=ur(1)
      v1r=ur(2)/rhor
      v2r=ur(3)/rhor
      q2r=v1r**2+v2r**2
      hr=(ur(4)+pr)/rhor
c
      rl=dsqrt(rhol)
      rr=dsqrt(rhor)
      r=rl+rr
      v1=(rl*v1l+rr*v1r)/r
      v2=(rl*v2l+rr*v2r)/r
      h=(rl*hl+rr*hr)/r
      q2=v1**2+v2**2
      vn=an(1)*v1+an(2)*v2
c
      do k=1,mr
        alaml(k)=ul(k+4)/rhol
        alamr(k)=ur(k+4)/rhor
        alam(k)=(rl*alaml(k)+rr*alamr(k))/r
      end do
c
      do k=mr+1,mr+me
        alaml(k)=ul(k+4)
        alamr(k)=ur(k+4)
        alam(k)=(rl*alaml(k)+rr*alamr(k))/r
      end do
c
      do k=1,mr+2
        dp(k)=.5d0*(dpl(k)+dpr(k))
      end do
c
cx
      if (iflg.eq.1) then
        write(6,*)'dpl,dpr,dp=',(dpl(i),i=1,3),
     *             (dpr(i),i=1,3),(dp(i),i=1,3)
      end if
cx
c
c get derivatives (Glaister type averaging for non-ideal EOS)
      if (ieos.ne.idealGasEOS.or.
     *   (ieos.eq.idealGasEOS.and.imult.eq.1)) then

        tol=1.d-3
        d(1)=ur(1)-ul(1)
        d(2)=ur(4)-.5d0*rhor*q2r-(ul(4)-.5d0*rhol*q2l)
        arg=d(1)**2+d(2)**2
        do k=1,mr
          d(k+2)=ur(k+4)-ul(k+4)
          arg=arg+d(k+2)**2
        end do
        if (arg.gt.tol) then
          theta=pr-pl-dp(1)*d(1)-dp(2)*d(2)
          do k=1,mr
            theta=theta-dp(k+2)*d(k+2)
          end do
          theta=theta/arg
          do k=1,mr+2
            dp(k)=dp(k)+d(k)*theta
          end do
        end if
      end if
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
      el(1,4)= tmp1
      el(m,1)=-tmp1*tmp3+(c-vn)/(2*c)
      el(m,2)=-tmp1*v1+tmp4*an(1)
      el(m,3)=-tmp1*v2+tmp4*an(2)
      el(m,4)= tmp1
c
      er(1,1)=1.d0
      er(2,1)=v1-an(1)*c
      er(3,1)=v2-an(2)*c
      er(4,1)=h-vn*c
      er(1,m)=1.d0
      er(2,m)=v1+an(1)*c
      er(3,m)=v2+an(2)*c
      er(4,m)=h+vn*c
c
c add on reactive contributions
      do i=1,mr+me
        ip4=i+4
        heat=0.d0
        if (i.le.mr) heat=-dp(2+i)/dp(2)
        el(1,ip4)=-tmp1*heat
        el(m,ip4)= el(1,ip4)
        er(ip4,1)= alam(i)
        er(ip4,m)= alam(i)
      end do
c
      return
      end
c
c+++++++++++++++++++++
c
      subroutine eigenr2dc (md,m,a0,a1,a2,ul,ur,pl,pr,dpl,dpr,
     *                     all,alr,isign)
c
c compute C- or C+ characteristic for states ul and ur
c
      implicit real*8 (a-h,o-z)
      dimension ul(md),ur(md),an(2),dpl(10),dpr(10)
      common / mvars / mh,mr,me,ieos,irxn,imult,islope,ifix
      r=dsqrt(a1**2+a2**2)
      an(1)=a1/r
      an(2)=a2/r
c
      v1=ul(2)/ul(1)
      v2=ul(3)/ul(1)
      q2=v1**2+v2**2
      vn=an(1)*v1+an(2)*v2
      h=(ul(4)+pl)/ul(1)
      c2=dpl(1)+(h-.5d0*q2)*dpl(2)
      do k=1,mr
        c2=c2+ul(4+k)*dpl(2+k)/ul(1)
      end do
      if (c2.gt.0.d0) then
        all=a0+r*(vn+isign*dsqrt(c2))
      else
        all=0.d0
        alr=0.d0
        return
      end if
c
      v1=ur(2)/ur(1)
      v2=ur(3)/ur(1)
      q2=v1**2+v2**2
      vn=an(1)*v1+an(2)*v2
      h=(ur(4)+pr)/ur(1)
      c2=dpr(1)+(h-.5d0*q2)*dpr(2)
      do k=1,mr
        c2=c2+ur(4+k)*dpr(2+k)/ur(1)
      end do
      if (c2.gt.0.d0) then
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
      subroutine roespd2dc (md,m,a1,a2,ul,ur,pl,pr,
     *                     dpl,dpr,spl,spr,ier)
c
c supply eigenvalues and eigenvectors using an approximate average value
c of u=u(ul,ur) for Roe's Riemann solver
c
      implicit real*8 (a-h,o-z)
      dimension ul(md),ur(md),alaml(8),alamr(8),
     *          alam(8),an(2),dpl(10),dpr(10),dp(10),d(10)
c
      include 'eosDefine.h'  ! define the EOS names, idealGasEOS, jwlEOS, mieGruneisenEOS
      common / mvars / mh,mr,me,ieos,irxn,imult,islope,ifix
      data c2min, c2eps / -1.d-2, 1.d-6 /
c
      rad=dsqrt(a1**2+a2**2)
      an(1)=a1/rad
      an(2)=a2/rad
c
c compute left and right states, and compute Roe average
      rhol=ul(1)
      v1l=ul(2)/rhol
      v2l=ul(3)/rhol
      q2l=v1l**2+v2l**2
      hl=(ul(4)+pl)/rhol
      vnl=an(1)*v1l+an(2)*v2l
      c2l=dpl(1)+(hl-.5d0*q2l)*dpl(2)
c
      rhor=ur(1)
      v1r=ur(2)/rhor
      v2r=ur(3)/rhor
      q2r=v1r**2+v2r**2
      hr=(ur(4)+pr)/rhor
      vnr=an(1)*v1r+an(2)*v2r
      c2r=dpr(1)+(hr-.5d0*q2r)*dpr(2)
c
      rl=dsqrt(rhol)
      rr=dsqrt(rhor)
      r=rl+rr
      v1=(rl*v1l+rr*v1r)/r
      v2=(rl*v2l+rr*v2r)/r
      h=(rl*hl+rr*hr)/r
      q2=v1**2+v2**2
      vn=an(1)*v1+an(2)*v2
c
      suml=0.d0
      sumr=0.d0
      do k=1,mr
        alaml(k)=ul(k+4)/rhol
        alamr(k)=ur(k+4)/rhor
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
      if (ieos.ne.idealGasEOS.or.
     *   (ieos.eq.idealGasEOS.and.imult.eq.1)) then
        tol=1.d-3
        d(1)=ur(1)-ul(1)
        d(2)=ur(4)-.5d0*rhor*q2r-(ul(4)-.5d0*rhol*q2l)
        sqr=d(1)**2+d(2)**2                
        do k=1,mr
          d(k+2)=ur(k+4)-ul(k+4)
          sqr=sqr+d(k+2)**2
        end do
        if (sqr.gt.tol) then
          theta=pr-pl-(dp(1)*d(1)+dp(2)*d(2))
          do k=1,mr
            theta=theta-dp(k+2)*d(k+2)
          end do
          theta=theta/sqr
          do k=1,mr+2
            dp(k)=dp(k)+d(k)*theta
          end do
        end if
      end if
c
      sum=0.d0
      do k=1,mr
        sum=sum+alam(k)*dp(2+k)
      end do
      c2=dp(1)+(h-.5d0*q2)*dp(2)+sum
      c=dsqrt(max(c2,c2eps))
c
c approximate wave speeds
      if (ieos.eq.idealGasEOS.or.
     *   (ieos.eq.idealGasEOS.and.imult.eq.1)) then
        beta=dsqrt(.5d0*dp(2)/(dp(2)+1.d0))
      else if( ieos.eq.jwlEOS ) then
        beta=1.d0
      else if( ieos.eq.mieGruneisenEOS )then
        ! *wdh* what should this be?
        beta=1.d0 ! dsqrt(.5d0*dp(2)/(dp(2)+1.d0))
      else
        write(*,'("ERROR: ieos")')
      end if
      beta=dsqrt(.5d0*0.4d0/1.4d0)
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
      subroutine prim2dc (md,ul,ur,pl,pr,an1,an2)
c
c compute the primitive variables for the left and right states
c assuming an ideal gas
c
      implicit real*8 (a-h,o-z)
      dimension ul(md),ur(md)
      common / mydata / gam,ht(0:2),at(2),prxn(2)
      common / prmdat / r(2),v(2),p(2),c(2),a(2),b(2),
     *                  gamma,gm1,gp1,em,ep
c
c some constants involving gamma
      gamma=gam
      gm1=gamma-1.d0
      gp1=gamma+1.d0
      em=0.5d0*gm1/gamma
      ep=0.5d0*gp1/gamma
c
c left state
      p(1)=pl
      r(1)=ul(1)
      v(1)=(an1*ul(2)+an2*ul(3))/ul(1)
c
c right state
      p(2)=pr
      r(2)=ur(1)
      v(2)=(an1*ur(2)+an2*ur(3))/ur(1)
c
c sound speeds and some constants
      do k=1,2
        a(k)=2.d0/(gp1*r(k))
        b(k)=gm1*p(k)/gp1
        c2=gamma*p(k)/r(k)
        if (c2.le.0.d0) then
          write(6,*)'Error (prim) : c2.le.0, k =',k
          write(6,*)'ul =',ul
          write(6,*)'ur =',ur
          stop
        end if
        c(k)=dsqrt(c2)
      end do
c
c check for vacuum state
      if (2.d0*(c(1)+c(2))/gm1.le.v(2)-v(1)) then
        write(6,*)'Error (prim2d) : vacuum state found'
        stop
      end if
c
      return
      end
c
c+++++++++++++++
c
      subroutine middlec (pm,vm,spl,spr)
c
c see Toro, page 128, and algorithm, pages 156-7.
c
      implicit real*8 (a-h,o-z)
      logical ishock
      dimension vdif(2,2),ishock(2)
      common / prmdat / r(2),v(2),p(2),c(2),a(2),b(2),
     *                  gamma,gm1,gp1,em,ep
      data pratio, ptol / 2.d0, 1.d-6 /
      data tol, itfix, itmax / 1.d-8, 3, 10 /
c
      pmin=min(p(1),p(2))
      pmax=max(p(1),p(2))
c
c start with guess based on a linearization
      ppv=.5d0*(p(1)+p(2))-.125d0*(v(2)-v(1))*(r(1)+r(2))*(c(1)+c(2))
      ppv=max(ppv,0.d0)
c
      if (pmax/pmin.le.pratio.and.pmin.le.ppv.and.pmax.ge.ppv) then
        pm=ppv
      else
        if (ppv.lt.pmin) then
c guess based on two rarefaction solution
          arg1=c(1)/(p(1)**em)
          arg2=c(2)/(p(2)**em)
          arg3=(c(1)+c(2)-.5d0*gm1*(v(2)-v(1)))/(arg1+arg2)
          pm=arg3**(1.d0/em)
        else
c guess based on two shock approximate solution
          gl=dsqrt(a(1)/(ppv+b(1)))
          gr=dsqrt(a(2)/(ppv+b(2)))
          pts=(gl*p(1)+gr*p(2)-v(2)+v(1))/(gl+gr)
          pm=max(ptol,pts)
        end if
      end if
c
c set ishock initially
      do k=1,2
        ishock(k)=.true.
        if (pm.le.p(k)) ishock(k)=.false.
      end do
c
c Newton iteration to find pm, the pressure in the middle state
      it=0
    1 it=it+1
c
c   determine velocity difference across a shock or rarefaction
        do k=1,2
          if (ishock(k)) then
            arg=pm+b(k)
            fact=dsqrt(a(k)/arg)
            diff=pm-p(k)
            vdif(1,k)=fact*diff
            vdif(2,k)=fact*(1.d0-0.5d0*diff/arg)
          else
            arg=pm/p(k)
            fact=2.d0*c(k)/gm1
            vdif(1,k)=fact*(arg**em-1.d0)
            vdif(2,k)=1.d0/(r(k)*c(k)*arg**ep)
          end if
        end do
c
c   determine change to pressure in the middle state
        dpm=(vdif(1,1)+vdif(1,2)+v(2)-v(1))/(vdif(2,1)+vdif(2,2))
c
c check for convergence
      if (dabs(dpm).gt.tol) then
        if (it.lt.itmax) then
c         write(6,100)it,pm,dpm,ishock
c 100     format(' it=',i2,',  pm=',f12.5,',  dpm=',1pe9.2,2(2x,i1))
          pm=pm-dpm
          if (it.le.itfix) then
            do k=1,2
              ishock(k)=.true.
              if (pm.le.p(k)) ishock(k)=.false.
            end do
          end if
          goto 1
        else
          write(6,*)'Error (middle) : itmax exceeded'
          stop
        end if
      end if
c
c     write(6,101)it,pm,dpm
c 101 format(' it=',i2,',  pm=',f12.5,',  dpm=',1pe9.2,'  converged')
c
c compute velocity
      vm=.5d0*(v(1)+v(2)+vdif(1,2)-vdif(1,1))
c
c compute min and max wave speeds
      if (ishock(1)) then
        spl=v(1)-c(1)*dsqrt(ep*pm/p(1)+em)
      else
        spl=v(1)-c(1)
      end if
      if (ishock(2)) then
        spr=v(2)+c(2)*dsqrt(ep*pm/p(2)+em)
      else
        spr=v(2)+c(2)
      end if
c
      return
      end
c
c+++++++++++++++
c
      subroutine upstarc (md,u,pm,vm,an0,an1,an2,ustar,pstar,k)
c
      implicit real*8 (a-h,o-z)
      dimension u(md),ustar(md)
      common / mvars / mh,mr,me,ieos,irxn,imult,islope,ifix
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
          pstar=p(k)
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
          pstar=p(k)
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
      vt=(an1*u(3)-an2*u(2))/u(1)
      q2=vm**2+vt**2
      sum=0.d0
      do kr=1,mr
        u(4+kr)=rm*u(4+kr)/u(1)
        sum=sum+ht(kr)*u(4+kr)
      end do
      ustar(1)=rm
      ustar(2)=rm*(an1*vm-an2*vt)
      ustar(3)=rm*(an1*vt+an2*vm)
      ustar(4)=pm/gm1+.5d0*rm*q2+sum
      pstar=pm
c
      return
      end
c
c+++++++++++++++++++++++
c
      subroutine source2dc (md,m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,j2,dr,
     *                     u,u0,up2,tau,mask,nrwk,rwk,niwk,iwk,
     *                     maxnstep,ipc)
c
c Requires (5*mr+md+3)*ngrid for real workspace and 3*ngrid for
c integer workspace
c
      implicit real*8 (a-h,o-z)
      dimension u(nd1a:nd1b,nd2a:nd2b,md),u0(md,nd1a:nd1b),
     *          up2(m,nd1a:nd1b),tau(nd1a:nd1b),mask(nd1a:nd1b),
     *          rwk(nrwk),iwk(niwk)
      common / mvars / mh,mr,me,ieos,irxn,imult,islope,ifix
c
      maxnstep=0
c
      if (mr.eq.0.or.(mr.eq.1.and.imult.eq.1)) then
c..Euler equations (perhaps with some EOS variables)
        do j1=n1a,n1b
          do i=1,m
            up2(i,j1)=0.d0
          end do
        end do
        return
      end if
c
c..Reactive Euler....
c
      ng=(n1b-n1a+1)
      if ((5*mr+md+3)*ng.gt.nrwk.or.2*ng.gt.niwk) then
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
      lnstep=ljk+ng
c
      irc=5
      nlam=mr
      if (imult.eq.1) then
        irc=6
        nlam=nlam-1
      end if
c
      call getsrc2dc (md,m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,j2,dr,u,u0,
     *               up2,tau,mask,rwk(ltest),rwk(lrk),rwk(lhk),
     *               rwk(luk),rwk(lwk),rwk(lc),rwk(lwpk),iwk(ljk),
     *               iwk(lnstep),maxnstep,irc,nlam,ng,ipc)
c
      if (me.gt.0) then
        i1=5+mr
        do j1=n1a,n1b
          do i=i1,m
            up2(i,j1)=0.d0
          end do
        end do
      end if
c
      return
      end
c
c+++++++++++++++++++++
c
      subroutine getsrc2dc (md,m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,j2,dr,u,
     *                     u0,up2,tau,mask,test,rk,hk,uk,wk,c,wpk,jk,
     *                     nstep,maxnstep,irc,nlam,ng,ipc)
c
      implicit real*8 (a-h,o-z)
      dimension u(nd1a:nd1b,nd2a:nd2b,md),u0(md,nd1a:nd1b),
     *          up2(m,nd1a:nd1b),tau(nd1a:nd1b),mask(nd1a:nd1b),
     *          test(ng),rk(ng),hk(ng),uk(md,ng),wk(nlam,ng),
     *          c(nlam,ng,3),wpk(nlam,ng),jk(ng),nstep(n1a:n1b)
      common / srcdat / nb1a,nb1b,nb2a,nb2b,icount
c
c     data tol, tiny / 1.d-5, 1.d-14 /
      data tol, tiny / .0001d0, 1.d-14 /
c
c..It is assumed that alam=(u(j1,j2,irc)+dr*up2(j1,j2,irc)/(u(j1,j2,1)+dr*up2(j1,j2,1))=1
c  (the advected lambda) is an equilibrium value.  Thus, if alam is close enough to 1,
c  then set up2(icr) so that alam will equal 1 and return.
c
      if (nlam.eq.1) then
        n=0
        do j1=n1a,n1b
          nstep(j1)=0
          if (mask(j1).ne.0) then
c this was the old alam (dws)
c           alam=u(j1,j2,irc)/u(j1,j2,1)
c this is the new alam (dws)
            alam=(u(j1,j2,irc)+dr*up2(irc,j1))/(u(j1,j2,1)+dr*up2(1,j1)
     & )
            if (alam.gt.1.d0-1.d-6) then
              up2(irc,j1)=(u(j1,j2,1)-u(j1,j2,irc))/dr
     *                      +up2(1,j1)-up2(irc,j1)
              do i=1,irc-1
                up2(i,j1)=0.d0
              end do
            else
              n=n+1
              jk(n)=j1
              rk(n)=0.d0
              hk(n)=dr
              do i=1,md
                uk(i,n)=u0(i,j1)
              end do
              do i=1,nlam
                wk(i,n)=u(j1,j2,i+irc-1)
              end do
            end if
          else
            do i=1,m
              up2(i,j1)=0.d0
            end do
          end if
        end do
      else
        n=0
        do j1=n1a,n1b
          nstep(j1)=0
          if (mask(j1).ne.0) then
            n=n+1
            jk(n)=j1
            rk(n)=0.d0
            hk(n)=dr
            do i=1,md
              uk(i,n)=u0(i,j1)
            end do
            do i=1,nlam
              wk(i,n)=u(j1,j2,i+irc-1)
            end do
          else
            do i=1,m
              up2(i,j1)=0.d0
            end do
          end if
        end do
      end if
c
      if (n.eq.0) return
c
c..if more than one species or if away from equilibrium, then compute source
c  by integrating from r=0 to r=dr holding the density, velocities, and the
c  total energy fixed.
      itmax=1000
      hmin=dr/900
c
      do it=1,itmax
c
        call rate2dc (md,irc,nlam,n,uk,wk,wpk)
        do k=1,n
          do i=1,nlam
            c(i,k,1)=hk(k)*wpk(i,k)
            c(i,k,2)=wk(i,k)+.5d0*c(i,k,1)
          end do
        end do
c
        call rate2dc (md,irc,nlam,n,uk,c(1,1,2),wpk)
        do k=1,n
          do i=1,nlam
            c(i,k,2)=hk(k)*wpk(i,k)
            c(i,k,3)=wk(i,k)+.75d0*c(i,k,2)
          end do
        end do
c
        call rate2dc (md,irc,nlam,n,uk,c(1,1,3),wpk)
        do k=1,n
          j1=jk(k)
          test(k)=0.d0
          do i=1,nlam
            c(i,k,3)=hk(k)*wpk(i,k)
            test(k)=max(dabs(2*c(i,k,1)-6*c(i,k,2)+4*c(i,k,3)),test(k))
          end do
          test(k)=test(k)/(9*hk(k))
          alam=u(j1,j2,irc)/u(j1,j2,1)
          if (alam.lt.0.99d0.and.ipc.eq.1) then
            tau(j1)=max(test(k),tau(j1))
          end if
c icount determines the maximum number of sub-time steps taken for the
c source calculation.  The 26th bit in the mask array determines whether
c the grid point is covered by a finer grid.
c         if (.not.btest(mask(j1),26)) then
c           if (j1.ge.nb1a.and.j1.le.nb1b.and.
c    *          j2.ge.nb2a.and.j2.le.nb2b) then
c             icount=max(it,icount)
c             if (nb1a.eq.0.and.nb2a.eq.0) then
c               if ((nb1b.eq.495.and.nb2b.eq.400).or.
c    *              (nb1b.eq.306.and.nb2b.eq.5)) then
c                 if (it.ge.2) then
c                   write(6,556)it,j1,j2,nb1b,nb2b,test(k)
c 556               format('it=',i6,'  j1,j2=',2(1x,i5),
c    *                     '  n1b,n2b=',2(1x,i5),'  test=',1pe9.2)
c                 end if
c               end if
c             end if
c           end if
c         end if
        end do
c
        n1=0
        do k=1,n
          if (test(k).lt.tol.or.hk(k).le.hmin .or. it.gt.500 ) then
c*wdh
c           we have converged or reached the min step size
            if( it.gt.500 .and..false.)then
              write(*,*) 'WARNING (getsrc) number of iterations=',it
              if( ipc.eq.0 )then
                write(*,*) 'This error is at the predictor step'
              else
                write(*,*) 'This error is at the corrector step'
              end if
              j1=jk(k)
              write(*,9000) j1,mask(j1),(u(j1,j2,i),i=1,m)
              write(*,9100) test(k),tol,hk(k),u(j1,j2,5)/u(j1,j2,1),
     &          (u0(i,j1),i=1,m)     
 9100         format(1x,' test=',e9.2,' tol=',e9.2,' hk=',e9.2,
     &               ' alam=',e10.3,' u0=',12(e11.3,1x))
              write(*,9200) (uk(i,k),i=1,4),(wk(i,k),i=1,nlam),
     &                      (c(i,k,1),i=1,nlam),(c(i,k,2),i=1,nlam),
     &                      (c(i,k,3),i=1,nlam)
 9200         format(' uk=',4(e11.2,1x),' wk=',(e11.2,1x),' c = ',
     &             3(e11.3,1x))
              write(*,9300) (up2(i,j1),i=1,4+nlam)
 9300         format(' up2=',2(e11.3,1x))
              if (.not.btest(mask(j1),26)) then
                 write(*,*) ' Above pt is not hidden by refinement'
              else
                 write(*,*) ' Above pt is hidden by refinement'
              end if
              if( mask(j1).lt.0 ) then
                 write(*,*) ' mask(j1)<0'
              end if
              if (.not.btest(mask(j1),31)) then
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
            j1=jk(k)
            nstep(j1)=nstep(j1)+1
            maxnstep=max(nstep(j1),maxnstep)
            if (.not.btest(mask(j1),26)) then
              if (j1.ge.nb1a.and.j1.le.nb1b.and.
     *            j2.ge.nb2a.and.j2.le.nb2b) then
                icount=max(nstep(j1),icount)
c               if (nb1a.eq.0.and.nb2a.eq.0) then
c                 if ((nb1b.eq.495.and.nb2b.eq.400).or.
c    *                (nb1b.eq.306.and.nb2b.eq.5)) then
c                   if (nstep(j1).ge.2) then
c                     write(6,556)nstep(j1),j1,j2,nb1b,nb2b,test(k),
c    *                            hk(k),hmin
c 556                 format('nstep=',i3,'  j1,j2=',2(1x,i5),
c    *                       '  n1b,n2b=',2(1x,i5),'  test=',1pe9.2,
c    *                       '  h,hmin=',2(1pe9.2))
c                   end if
c                 end if
c               end if
              end if
            end if
            if (rk(k).lt.dr-tiny) then
              n1=n1+1
              test(n1)=test(k)
              jk(n1)=jk(k)
              rk(n1)=rk(k)
              hk(n1)=hk(k)
              do i=1,md
                uk(i,n1)=uk(i,k)
              end do
              do i=1,nlam
                wk(i,n1)=wk(i,k)
              end do
            else
              j1=jk(k)
              rho=u(j1,j2,1)+dr*up2(1,j1)
              do i=1,nlam
                upl=(wk(i,k)-u(j1,j2,i+irc-1))/dr
c               alam=(u(j1,j2,i+irc-1)+dr*(up2(i+irc-1,j1)+upl))/rho
c               if (alam.gt.1.d0) then
c                 upl=(u(j1,j2,1)-u(j1,j2,irc))/dr
c    *                    +up2(1,j1)-up2(irc,j1)
c               end if
                up2(i+irc-1,j1)=upl
              end do              
              do i=1,irc-1
                up2(i,j1)=0.d0
              end do
            end if
          else
            n1=n1+1
            test(n1)=test(k)
            jk(n1)=jk(k)
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
        j1=jk(k)
        write(*,9000) j1,mask(j1),(u(j1,j2,i),i=1,m)
 9000   format(1x,' -> j1=',i4,' mask=',i15,' u=',10(e9.2,1x))
      end do
      stop
      end
c
c+++++++++++++++++++++
c
      subroutine rate2dc (md,irc,nlam,n,u,w,wp)
c
      implicit real*8 (a-h,o-z)
      dimension u(md,n),w(nlam,n),wp(nlam,n),dp(10)
      dimension omeg(2),ajwl(2,2),rjwl(2,2)
      common / mvars / mh,mr,me,ieos,irxn,imult,islope,ifix
      common / mydata / gam,ht(0:2),at(2),pr(2)
      common / igdat / ra,eb,ex,ec,ed,ey,ee,eg,ez,al0,al1,al2,
     *                 ai,ag1,ag2
      data eps, pmin / 1.d-15, 1.d-3 /
c
      if (nlam.gt.2) then
        write(6,*)'Error (rate) : nlam.gt.2'
        stop
      end if
c
      if (irxn.eq.1.or.irxn.eq.7) then
        do k=1,n
          rho=u(1,k)
          u(irc,k)=w(1,k)
          ier=0
          call getp2dc (md,u(1,k),p,dp,0,temp,ier)
          if (ier.ne.0) then
            write(17,101)(u(i,k),i=1,md)
  101       format(' Error (rate) : u(:,k)=',/,
     *             5(1x,1pe15.8))
            stop
          end if
          ! *wdh* temp=p/rho ! this is now computed above in getp2d
ccccccccc JWB added temporarily for Ioan ... "temperature kinetic model"
c          if( temp >= 0.26 ) then
cc            ak1 = 1000000.d0
c            ak1 = 10.d0
c          else
c            ak1 = 0.d0
c         end if
c         wp(1,k)=rho*fuel*ak1
ccccccc
ccccccc JWB added for pressure dependent rate law
          prod=w(1,k)/rho
          if( irxn.eq.1 ) then
            fuel=1.d0-prod
            ak1=pr(1)*dexp(-at(1)/temp)
            wp(1,k)=rho*fuel*ak1
          elseif( irxn.eq.7 ) then
c            fuel=1.d0-prod*(u(5,k)/u(1,k))
            fuel=1.d0-prod
            ak1=pr(1)*p**(at(1))
            wp(1,k)=rho*dsqrt(max(fuel,eps))*ak1
          endif
        end do
      elseif (irxn.eq.2) then
c ak1=reaction rate for chain-initiation
c ak2=reaction rate for chain-branching
        do k=1,n
          rho=u(1,k)
          prod=w(1,k)/rho
          rdcl=w(2,k)/rho
          fuel=1.d0-prod-rdcl
          u(irc  ,k)=prod
          u(irc+1,k)=rdcl
          ier=0
          call getp2dc (md,u(1,k),p,dp,0,temp,ier)
          if (ier.ne.0) then
            write(17,102)(u(i,k),i=1,md)
  102       format(' Error (rate) : u(:,k)=',/,
     *             6(1x,1pe15.8))
            stop
          end if
          ! *wdh* temp=p/rho ! this is now computed above in getp2d
          ak1=pr(1)*dexp(-at(1)/temp)
          ak2=pr(2)*dexp(-at(2)/temp)
          wp(1,k)=rho*rdcl
          wp(2,k)=rho*(fuel*(ak1+rdcl*ak2)-rdcl)
        end do
      elseif (irxn.eq.3) then
        do k=1,n
          rho=u(1,k)
          u(irc,k)=w(1,k)
          ier=0
          call getp2dc (md,u(1,k),p,dp,0,te,ier)
          if (ier.ne.0) then
            write(17,103)(u(i,k),i=1,md)
  103       format(' Error (rate) : u(:,k)=',/,
     *             7(1x,1pe15.8))
            stop
          end if
          p=max(p,eps)
          prod=max(w(1,k)/rho,eps)
          fuel=max(1.d0-prod,eps)
          wp(1,k)=0.d0
          arg=rho-1.d0-ra
          if (arg.gt.0.d0) then
            if (prod.lt.al0 ) then
              wp(1,k)=wp(1,k)+ai*(fuel**eb)*(arg**ex)
            end if
          end if
          if (p.gt.pmin) then
            if (prod.lt.al1) then
              wp(1,k)=wp(1,k)+ag1*(fuel**ec)*(prod**ed)*(p**ey)
            end if
            if (prod.gt.al2) then
              wp(1,k)=wp(1,k)+ag2*(fuel**ee)*(prod**eg)*(p**ez)
            end if
          end if
          wp(1,k)=rho*wp(1,k)
        end do
      else
        write(6,*)'Error (rate) : irxm not supported'
        write(6,*)irxn
        stop
      end if
c
      return
      end
c
c+++++++++++++++++++
c
      subroutine fluxfixc (md,m,aj,a0,a1,a2,ul,ur,dul,dur,
     *                    fl,fr,method,ier)
c
c a1, a2, and a3 are the mapping transformations.
c
      implicit real*8 (a-h,o-z)
      dimension ul(md),ur(md),dul(md),dur(md),
     *          fl(md),fr(md),dpl(10),dpr(10),conl(10),conr(10)
      common / mvars / mh,mr,me,ieos,irxn,imult,islope,ifix
      data eps / 1.d-14 /
c
c.. get infector velocities and pressures
      if( islope.eq.1 ) then
        v1l=ul(2)
        v1r=ur(2)
        v2l=ul(3)
        v2r=ur(3)
        rpl=ul(4)
        rpr=ur(4)
      else
        v1l=ul(2)/ul(1)
        v1r=ur(2)/ur(1)
        v2l=ul(3)/ul(1)
        v2r=ur(3)/ur(1)
        ier1=0
        ier2=0
        call getp2dc( md,ul,rpl,dpl,0,te,ier1 )
        call getp2dc( md,ur,rpr,dpr,0,te,ier2 )
        if( ier1.ne.0.or.ier2.ne.0 ) then
          write(17,*)'Error (fluxfix): getp2d'
          stop
        end if
      end if
      rad=dsqrt(a1**2+a2**2)
c     
c first step is to infect right cell with left cell pressure and velocities.
c This infection gives the flux out of the left cell
c Begin by filling con vectors with primative quantities and then convert
c them to conservative quantities
      conl(1)=ul(1)+dul(1)
      conr(1)=ur(1)+dur(1)
      conl(2)=v1l
      conr(2)=v1l
      conl(3)=v2l
      conr(3)=v2l
      conl(4)=rpl
      conr(4)=rpl
      if( islope.eq.1 ) then
        factl=1.d0
        factr=1.d0
      else
        factl=conl(1)
        factr=conr(1)
      end if
      do i=5,m
        conl(i)=(ul(i)+dul(i))/factl
        conr(i)=(ur(i)+dur(i))/factr
      end do
      call getconc( md,conl,conl,ier1 )
      call getconc( md,conr,conr,ier2 )
      if( ier1.ne.0.or.ier2.ne.0 ) then
          write(17,*)'Error (fluxfix): getcon'
          stop
        end if
      velng=a1*v1l+a2*v2l      
      vel=velng-a0    !could be wrong sign ... find out later!!!
c     
c.. different methods can be inserted here
      if( method.eq.1 ) then
        ! Roe solver
        absvel=dabs(vel)
        ! We can now calculate the fluxes
        do i=1,m
          fl(i)=.5d0*aj*(vel*(conr(i)+conl(i))
     *                    -absvel*(conr(i)-conl(i)))
        end do
        fl(4)=fl(4)+.5d0*aj*velng*2.d0*rpl
      elseif( method.eq.3 ) then
        ! HLL solver
        ier1=0
        ier2=0
        call getp2dc( md,conl,rpl,dpl,mr+2,te,ier1 )
        call getp2dc( md,conr,rpl,dpr,mr+2,te,ier2 )
        if( ier1.ne.0.or.ier2.ne.0 ) then
          write(17,*)'Error (fluxfix): getp2d'
          stop
        end if
        call roespd2dc( md,md,a1,a2,conl,conr,
     *                 rpl,rpl,dpl,dpr,sl,sr,ier )
        if( ier.ne.0 ) then
          write(17,*)'Error (fluxfix): roespd2d'
          stop
        end if
        sl=aj*(sl+a0)
        sr=aj*(sr+a0)
        do i=1,m
          fl(i)=1.d0/(sr-sl)*(aj*vel*(sr*conl(i)-sl*conr(i))
     *          +sl*sr*(conr(i)-conl(i)))
        end do
        fl(4)=fl(4)+aj*velng*rpl
      else
        write(6,*)'Error(fluxfix): method not supported in pressure fix'
        stop
      end if
c
c second step is to infect left cell with right cell 
      conl(2)=v1r
      conr(2)=v1r
      conl(3)=v2r
      conr(3)=v2r
      conl(4)=rpr
      conr(4)=rpr
      velng=a1*v1r+a2*v2r    
      vel=velng-a0  !could be wrong sign ... find out later!!!
      if( islope.eq.1 ) then
        factl=1.d0
        factr=1.d0
      else
        factl=conl(1)
        factr=conr(1)
      end if
      do i=5,m
        conl(i)=(ul(i)+dul(i))/factl
        conr(i)=(ur(i)+dur(i))/factr
      end do
      call getconc( md,conl,conl,ier1 )
      call getconc( md,conr,conr,ier2 )
      if( ier1.ne.0.or.ier2.ne.0 ) then
          write(17,*)'Error (fluxfix): getcon'
          stop
        end if
      ! We can now calculate the fluxes
      if( method.eq.1 ) then
        absvel=dabs(vel)
        do i=1,m
          fr(i)=.5d0*aj*(vel*(conr(i)+conl(i))
     *                  -absvel*(conr(i)-conl(i)))
        end do
        fr(4)=fr(4)+.5d0*aj*velng*2.d0*rpr
      elseif( method.eq.3 ) then
        ier1=0
        ier2=0
        call getp2dc( md,conl,rpr,dpl,mr+2,te,ier1 )
        call getp2dc( md,conr,rpr,dpr,mr+2,te,ier2 )
        if( ier1.ne.0.or.ier2.ne.0 ) then
          write(17,*)'Error (fluxfix): getp2d'
          stop
        end if
        call roespd2dc( md,md,a1,a2,conl,conr,
     *                 rpr,rpr,dpl,dpr,sl,sr,ier )
        if( ier.ne.0 ) then
          write(17,*)'Error (fluxfix): roespd2d'
          stop
        end if
        sl=aj*(sl+a0)
        sr=aj*(sr+a0)
        do i=1,m
          fr(i)=1.d0/(sr-sl)*(aj*vel*(sr*conl(i)-sl*conr(i))
     *          +sl*sr*(conr(i)-conl(i)))
        end do
        fr(4)=fr(4)+aj*velng*rpr
      else
        write(6,*)'Error(fluxfix): method not supported in pressure fix'
        stop
      end if
c
      return
      end
c
c+++++++++++++++++
c
      subroutine ecorrectc( md,unew,uold,utild,dr,ecorr,ier )
c
      implicit real*8(a-h,o-z)
      dimension unew(md),uold(md),utild(md),dp(10),
     *          prim(10),con(10)
      common / mvars / mh,mr,me,ieos,irxn,imult,islope,ifix
c
      include 'multiDat.h'
c      common / muldat / gami, gamr
      common / mydata / gam,ht(0:2),at(2),pr(2)
c
c..Determine old and tilde pressures
      ier1=0
      ier2=0
      call getp2dc( md,uold,pold,dp,0,te,ier1 )
      call getp2dc( md,utild,ptild,dp,0,te,ier2 )
      if( ier1.ne.0.or.ier2.ne.0 ) then
        write(17,*)'Error (ecorrect): getp2d'
        stop
      end if
      rdp=ptild-pold

      prim(1)=unew(1)
      prim(2)=unew(2)/unew(1)
      prim(3)=unew(3)/unew(1)
      ier=0
      call getp2dc( md,unew,pnew,dp,0,te,ier )
      if( ier.ne.0 ) then
        write(17,*)'Error (ecorrect): getp2d'
        stop
      end if
      prim(4)=pnew+rdp
      do k=5,md
        prim(k)=unew(k)/unew(1)
      end do
      call getconc( md,prim,con,ier )
      if( ier.ne.0 ) then
        write(17,*)'Error (ecorrect): getcon'
        stop
      end if
      ecorr=(unew(4)-con(4))/dr
      ier=0
c
      return
      end
c
c++++++++++++++++
c
      subroutine getp2dc (md,u,p,dp,ideriv,te,ier)

c compute pressure p (ideriv=0,default) and derivatives of p (ideriv>0)
c *wdh* also return the temperature, te

c Note that ier is an input and an output parameter for this routine

      implicit real*8 (a-h,o-z)
      dimension u(md),dp(10)
      include 'eosDefine.h'  ! define the EOS names, idealGasEOS, jwlEOS, mieGruneisenEOS
      real*8 mgp1,mgp2,v0

      common / mvars / mh,mr,me,ieos,irxn,imult,islope,ifix
      common / mydata / gam,ht(0:2),at(2),pr(2) 
c
      include 'multiDat.h'
c      common / muldat / gami, gamr
      include 'eosdat.h'
      data emin / -1.d10 /
c     data emin / 0.01d0 /
c
c
      if (mr+2.gt.10) then
        write(6,*)'Error (getp3d) : mr too big'
        stop
      end if
c
      if (ieos.eq.idealGasEOS.and.imult.eq.0) then         ! ideal eos
        ier=0
        gm1=gam-1.d0
        q2=(u(2)/u(1))**2+(u(3)/u(1))**2
        p=u(4)-.5d0*u(1)*q2
        do k=1,mr
          p=p-ht(k)*u(4+k)
        end do
        p=gm1*p
        te=p/u(1) ! Temperature
        if (ideriv.gt.0) then
          dp(1)=0.d0
          dp(2)=gm1
          do k=1,mr
            dp(2+k)=-gm1*ht(k)
          end do
        end if
      elseif( ieos.eq.idealGasEOS.and.imult.eq.1 ) then    ! multicomponent ideal eos
        ier=0
        rmu=u(5)/u(1)
        if( irxn.eq.1.or.irxn.eq.7 ) then
          rlam=u(6)/u(1)
        else
          rlam=0.d0
        endif
c
        ! get gamma function and its derivatives
        if( cvi.lt.0.d0 ) then
          ! here we use the mixing rules with no cvs
          t1=(gamr-1.d0)*(gami-1.d0)
          t2=rmu*(gami-1.d0)+(1.d0-rmu)*(gamr-1.d0)
          gm1=t1/t2
          dgm=-t1*(gami-gamr)/(t2**2)
        else
          t1=rmu*cvr*gamr+(1.d0-rmu)*cvi*gami
          t2=rmu*cvr+(1.d0-rmu)*cvi
          gm1=t1/t2-1.d0
          t1=cvi*cvr*(gamr-gami)
          t2=rmu*cvr+cvi-rmu*cvi
          dgm=t1/(t2*t2)
        endif
        !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
c
        q2=(u(2)/u(1))**2+(u(3)/u(1))**2
        en=u(4)/u(1)-.5d0*q2
        p=u(1)*en
        if( irxn.eq.1.or.irxn.eq.7 ) then
          p=p-u(1)*rmu*rlam*ht(1)
        endif
        p=p*gm1

        te=p/u(1) ! Temperature  *wdh* 050116 -- is this correct?
                  ! hahaha ... *jwb* 050207 -- yes this is correct.

        if( ideriv.gt.0 ) then
          t1=-dgm*en
          if( irxn.eq.1.or.irxn.eq.7 ) then
            t1=t1+rlam*ht(1)*(dgm*rmu+gm1)
            dp(4)=-gm1*rmu*ht(1)
          endif
          dp(1)=t1*rmu
          dp(2)=gm1
          dp(3)=-t1
        endif
      elseif (ieos.eq.jwlEOS) then     ! mixture JWL eos
        r=u(1)
        e=u(4)-.5d0*(u(2)**2+u(3)**2)/r
        y=u(5)
        vs=u(6)
        vg=u(7)
        iform=0
        if (ideriv.gt.0) iform=1
        e=max(e,emin)
        call geteos (r,e,y,vs,vg,p,dp,iform,ier)
        te=p/u(1) ! fake Temperature
        if (ier.ne.0) then
          if (ier.gt.0) then
            write(17,101)(u(i),i=1,md)
  101       format('Error (getdp) : error return from geteos',/,
     *             'u =',7(1x,1pe15.8))
          end if
          return
        end if
        u(6)=vs
        u(7)=vg

      else if( ieos.eq.mieGruneisenEOS )then

        ! define Mie-Gruneisen EOS
        !  p = (gamma-1)*( E - .5*rho*u^2 - E_c ) + alpha*(vn-1) + beta*(vn-1)**2
        !  E_c = (alpha/2)*( (vn-1)^2/vn ) + (beta/3)*( (vn-1)^3/vn )
        ier=0
        gm1=gam-1.d0
        q2=(u(2)/u(1))**2+(u(3)/u(1))**2
        p=u(4)-.5d0*u(1)*q2                  ! "p" = E - .5*rho*U^2 
        do k=1,mr
          p=p-ht(k)*u(4+k)
        end do
        ! alpha = mgp1
        ! beta = mgp2
        ! V0 = mgp3
        mgp1 = eosPar(1)
        mgp2 = eosPar(2)
        v0   = eosPar(3)
        vn=1./(u(1)*v0)   ! V/V0
        p=gm1*p

        ! Add MieG correction in two steps -- so we can compute T
        ! *wdh* 050108 change sign of Ec p = p - gm1*( .5*mgp1 + (mgp2/3.)*(vn-1.) )*(vn-1.)**2/vn
        p = p + gm1*( .5*mgp1 + (mgp2/3.)*(vn-1.) )*(vn-1.)**2/vn 

        ! kappa=eosPar(4): Cp = Cv + kappa*R   (kappa=1 for ideal gas)

        ! write(*,'(" getp2d: eosPar(4)=kappa=",e10.3)') eosPar(4)

        te=p/(eosPar(4)*u(1)) ! Temperature = (gamma-1)*( rho*e - E_c )/(kappa*rho*R) = (p-F)/(kappa*rho*Rg)

        ! p = p - ( .5*mgp1 + mgp2/3.*(vn-1.) )*(vn-1.)**2/vn
        !      + mgp1*(V-V0)/V0 + mgp2*(V-V0)**2/V0**2 
        p = p + ( mgp1 + mgp2*(vn-1.) )*(vn-1.)

        if (ideriv.gt.0) then
          ! dp/d(rho) = -V^2 dp/dV   
c *wdh* 050108          dp(1) = ( -vn**2*(mgp1 + 2.*mgp2*(vn-1.) ) +
c     &      gm1*( .5*mgp1*(vn**2-1.)+(mgp2/3.)*(vn-1.)**2*(2.*vn+1.)) 
c     &            )/v0
          dp(1) = ( -vn**2*(mgp1 + 2.*mgp2*(vn-1.) ) -
     &      gm1*( .5*mgp1*(vn**2-1.)+(mgp2/3.)*(vn-1.)**2*(2.*vn+1.)) 
     &            )/v0
c           dp(1) = ( -vn**2*(mgp1 + 2.*mgp2*(vn-1.) ) )/v0
          ! dp/d(rho*e) :
          dp(2)=gm1
          do k=1,mr
            dp(2+k)=-gm1*ht(k)
          end do
          ! write(*,'(" gam,mgp1,...",4e12.4)') gam,mgp1,mgp2,v0
          ! write(*,'(" vn,dp=",e9.2,1x,3e10.2)') vn,dp(1),dp(2),dp(3)
        end if

      else
        write(6,*)'Error (getp2d) : ieos not supported'
        stop
      end if
c
      return
      end
c
c+++++++++++++++++++++
c
      subroutine upxtrp2dc (md,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,up)
c
c extrapolate up into ghost cells.
c
      implicit real*8 (a-h,o-z)
      dimension up(nd1a:nd1b,nd2a:nd2b,md)
c
c sides
      do j2=n2a,n2b
        do i=1,md
          up(n1a-1,j2,i)=3*up(n1a,j2,i)-3*up(n1a+1,j2,i)+up(n1a+2,j2,i)
          up(n1b+1,j2,i)=3*up(n1b,j2,i)-3*up(n1b-1,j2,i)+up(n1b-2,j2,i)
          up(n1a-2,j2,i)=2*up(n1a-1,j2,i)-up(n1a,j2,i)
          up(n1b+2,j2,i)=2*up(n1b+1,j2,i)-up(n1b,j2,i)
        end do
      end do
c
c sides
      do j1=nd1a,nd1b
        do i=1,md
          up(j1,n2a-1,i)=3*up(j1,n2a,i)-3*up(j1,n2a+1,i)+up(j1,n2a+2,i)
          up(j1,n2b+1,i)=3*up(j1,n2b,i)-3*up(j1,n2b-1,i)+up(j1,n2b-2,i)
          up(j1,n2a-2,i)=2*up(j1,n2a-1,i)-up(j1,n2a,i)
          up(j1,n2b+2,i)=2*up(j1,n2b+1,i)-up(j1,n2b,i)
        end do
      end do
c
      return
      end
c
c+++++++++++++++++++++
c
      subroutine zerobd2dc (md,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,up)
c
c zero up at ghost cells.
c
      implicit real*8 (a-h,o-z)
      dimension up(nd1a:nd1b,nd2a:nd2b,md)
c
      do i=1,md
        do j2=n2a,n2b
          do j1=nd1a,n1a-1
            up(j1,j2,i)=0.d0
          end do
          do j1=n1b+1,nd1b
            up(j1,j2,i)=0.d0
          end do
        end do
        do j1=nd1a,nd1b
          do j2=nd2a,n2a-1
            up(j1,j2,i)=0.d0
          end do
          do j2=n2b+1,nd2b
            up(j1,j2,i)=0.d0
          end do
        end do
      end do
c
      return
      end
c
c+++++++++++++++++++++
c
      subroutine mondat2dc (md,m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                     r,dr,u,u0,mask)
c
      implicit real*8 (a-h,o-z)
      dimension u(nd1a:nd1b,nd2a:nd2b,md),u0(md),umin(9),umax(9),
     *          uavg(9),mask(nd1a:nd1b,nd2a:nd2b),dp(10)
      character*10 label(9)
      common / mvars / mh,mr,me,ieos,irxn,imult,islope,ifix
c
c can only handle 5 reacting + eos species unless the "9" is made bigger
      nlam=min(mr+me,5)
c
      do i=1,m
        uavg(i)=0.d0
      end do
c
      icnt=0
      do j2=n2a,n2b
      do j1=n1a,n1b
        if (mask(j1,j2).gt.0) then
          icnt=icnt+1
          do i=1,md
            u0(i)=u(j1,j2,i)
          end do
          rho=u0(1)
          v1=u0(2)/rho
          v2=u0(3)/rho
          ier=0
          call getp2dc (md,u0,p,dp,0,te,ier)
          if (ier.ne.0) then
            write(17,*)'Error (mondat2d) : error return from getp2d'
            write(17,*)'j1,j2 =',j1,j2
            write(17,*)'u =',(u0(i),i=1,md)
            stop
          end if
          if (icnt.eq.1) then
            umin(1)=rho
            umax(1)=rho
            umin(2)=v1
            umax(2)=v1
            umin(3)=v2
            umax(3)=v2
            umin(4)=p
            umax(4)=p
            do i=1,nlam
              alam=u0(4+i)/rho
              umin(4+i)=alam
              umax(4+i)=alam
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
          umin(4)=min(p,umin(4))
          umax(4)=max(p,umax(4))
          uavg(4)=uavg(4)+p
          do i=1,nlam
            alam=u0(4+i)/rho
            umin(4+i)=min(alam,umin(4+i))
            umax(4+i)=max(alam,umax(4+i))
            uavg(4+i)=uavg(4+i)+alam
          end do
        end if
      end do
      end do
c
      ng=(n1b-n1a+1)*(n2b-n2a+1)
      do i=1,m
        uavg(i)=uavg(i)/ng
      end do
c
      label(1)='density   '
      label(2)='x-velocity'
      label(3)='y-velocity'
      label(4)='pressure  '
      do i=1,nlam
        write(label(4+i),100)i
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
c++++++++++++++++
c
      subroutine addpsic (nd1a,nd1b,nd2a,nd2b,fact,rho,u)
c
      implicit real*8 (a-h,o-z)
      dimension rho(nd1a:nd1b,nd2a:nd2b),u(nd1a:nd1b,nd2a:nd2b)
c
      if (fact.gt.0) then
        do j1=nd1a,nd1b
          do j2=nd2a,nd2b
            u(j1,j2)=u(j1,j2)+rho(j1,j2)*psi(rho(j1,j2),0)
          end do
        end do
      else
        do j1=nd1a,nd1b
          do j2=nd2a,nd2b
            u(j1,j2)=u(j1,j2)+fact*psi(rho(j1,j2),0)
          end do
        end do
      end if
c
      return
      end
c
c++++++++++++++++
c
      subroutine getconc( md,uprim,ucon,ier )
c
c conversion from primitive to conservative variables (needed for
c the multi-component version)
c
      implicit real*8(a-h,o-z)
      dimension ucon(md), uprim(md)
      common / mvars / mh,mr,me,ieos,irxn,imult,islope,ifix
c
      include 'multiDat.h'
c      common / muldat / gami, gamr
      common / mydata / gam,ht(0:2),at(2),pr(2)
c
      ucon(1)=uprim(1)
      q2=uprim(2)**2+uprim(3)**2
      do i=2,3
        ucon(i)=uprim(1)*uprim(i)
      end do
      if( imult.eq.1 ) then
        rmu=uprim(5)
        if( cvi.lt.0.d0 ) then
          t1=(gamr-1.d0)*(gami-1.d0)
          t2=rmu*(gami-1.d0)+(1.d0-rmu)*(gamr-1.d0)
          gm1=t1/t2
        else
          t1=rmu*cvr*gamr+(1.d0-rmu)*cvi*gami
          t2=rmu*cvr+(1.d0-rmu)*cvi
          gm1=t1/t2-1.d0
        endif
        ucon(4)=uprim(4)/gm1
        if( irxn.eq.1.or.irxn.eq.7 ) then
          rlam=uprim(6)
          ucon(4)=ucon(4)+ucon(1)*rmu*rlam*ht(1)
        end if
        ucon(4)=ucon(4)+.5d0*ucon(1)*q2
      else
        gm1=gam-1.d0
        ucon(4)=uprim(4)/gm1
        do k=1,mr
          ucon(4)=ucon(4)+ht(k)*ucon(1)*uprim(4+k)
        end do
        ucon(4)=ucon(4)+.5d0*ucon(1)*q2
      end if
      do k=5,md
        ucon(k)=uprim(k)*uprim(1)
      end do
      ier=0
c
      return
      end
c
c++++++++++++++++
c
      double precision function psic (rho,ideriv)
c
c
      implicit real*8 (a-h,o-z)
      common / rxndat / gamma,heat(0:2),pre(2),act(2)
c
c..ideal gas
      if (ideriv.eq.0) then
        psic=-heat(0)
      else
        psic=0.d0
      end if
c
      return
      end

