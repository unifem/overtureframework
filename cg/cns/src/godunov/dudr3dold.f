      subroutine dudr3dc (md,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                   nd3a,nd3b,n3a,n3b,dr,ds1,ds2,ds3,r,rx,gv,det,
     *                   u,up,mask,ntau,tau,ad,mdat,dat,nrprm,rparam,
     *                   niprm,iparam,nrwk,rwk,niwk,iwk,idebug,ier)
c
      implicit real*8 (a-h,o-z)
      dimension u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,md),
     *          up(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,md),
     *          rx(*),gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,3),
     *          det(*),tau(ntau),rparam(nrprm),iparam(niprm),
     *          ds(3),ad(md),dat(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,*),
     *          rwk(nrwk),iwk(niwk),mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      dimension almax(3),dp(10)
      dimension utest(5)
      common / mvars / mh,mr,me,ieos,irxn
      common / mydata / gam,ht(0:2),at(2),pr(2)
      common / srcdat / nb1a,nb1b,nb2a,nb2b,nb3a,nb3b,icount
      common / timing / tflux,tslope,tsource
c
      write(6,*)'Starting dudr3d...time,dt =',r,dr
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
c    iparam(3) =ichg               is the grid changing with time (=0 => no)
c    iparam(4) =icart              Cartesian grid (=1 => yes)
c    iparam(5) =iorder             order of the method (=1 or 2)
c    iparam(6) =method             method of flux calculation (=0 => Roe solver)
c    iparam(10)=icount             the maximum number of sub-time steps is determined
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
      if (ieos.eq.0) then
c..ideal EOS
        gam=rparam(4)
        me=0
      elseif (ieos.eq.1) then
c..JWL EOS
        gam=rparam(4)
c       many other params
        me=2
      else
        write(6,*)'Error (dudr) : EOS model not supported'
        ier=1
        return
      end if
c
      irxn=iparam(2)
      if (irxn.eq.0) then
c..no reaction model
        mr=0
        ht(0)=0.d0
      elseif (irxn.eq.1) then
c..one-step reaction model
        mr=1
        ht(0)=0.d0
        ht(1)=-rparam(11)
        at(1)=rparam(12)
        pr(1)=rparam(13)
      elseif (irxn.eq.2) then
c..chain-branching reaction model
        mr=2
        ht(0)=0.d0
        ht(1)=-rparam(11)
        ht(2)=-rparam(12)
        at(1)=rparam(13)
        at(2)=rparam(14)
        pr(1)=rparam(15)
        pr(2)=rparam(16)
      elseif (irxn.eq.3) then
c..ignition and growth (this is fake for now)
        mr=1
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
        write(6,*)'Error (dudr) : currently three dimensions is 
     & assumed'
        ier=3
        return
      end if
c
c..sum of hydro and reaction and EOS variables
      m=mh+mr+me
c
      av=rparam(3)
      ichg=iparam(3)
      icart=iparam(4)
      iorder=iparam(5)
      method=iparam(6)
      igrid=iparam(7)
      level=iparam(8)
      nstep=iparam(9)
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
        call mondat3dc (md,m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                 nd3a,nd3b,n3a,n3b,r,dr,u,rwk,mask)
      end if
c
c..check for negative density, limit reacting species
      do j3=nd3a,nd3b
      do j2=nd2a,nd2b
      do j1=nd1a,nd1b
        if (mask(j1,j2,j3).ne.0) then
          rho=u(j1,j2,j3,1)
          if (rho.le.0.d0) then
            write(6,100)j1,j2,j3
  100       format(' ** Error (dudr) : negative density, j1,j2,j3 =',
     *             3(1x,i4))
            u(j1,j2,j3,1)=-u(j1,j2,j3,1)
          end if
          do i=mh+1,mh+mr
            alam=u(j1,j2,j3,i)/u(j1,j2,j3,1)
            if    (alam.lt.0.d0) then
              if (alam.lt.-.01d0) then
                write(6,102)j1,j2,j3,i,alam
  102           format(' ** Error (dudr) : alam < 0, j1,j2,j3,i,alam 
     & =',
     *                 3(1x,i4),1x,i2,1x,1pe10.3)
              end if
              alam=0.d0
            elseif (alam.gt.1.d0) then
              if (alam.gt.1.01d0) then
                write(6,103)j1,j2,j3,i,alam
  103           format(' ** Error (dudr) : alam > 1, j1,j2,j3,i,alam 
     & =',
     *                 3(1x,i4),1x,i2,1x,1pe10.3)
              end if
              alam=1.d0
            end if
            u(j1,j2,j3,i)=u(j1,j2,j3,1)*alam
          end do
          do i=1,md
            rwk(i)=u(j1,j2,j3,i)
          end do
          call getp3dc (md,rwk,pressure,dp,0,ier)
          if (pressure.le.0.d0) then
            write(6,104)nstep,igrid,level,nd1a,nd1b,nd2a,nd2b,nd3a,
     & nd3b,
     *                  j1,j2,j3
  104       format(' ** Error (dudr) : negative pressure',/,
     *             '    nstep,grid,level =',i6,1x,i3,1x,i2,/,
     *             '    nd1a,nd1b,nd2a,nd2b,nd3a,nd3b =',6(1x,i4),/,
     *             '    j1,j2,j3 =',3(1x,i4))
            write(6,*)'u =',(u(j1,j2,j3,i),i=1,md)
            stop
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
      lh=laj+2*ngrid
      leigal=lh+m*ngrid
      leigel=leigal+3*m
      leiger=leigel+3*m*m
      lalpha=leiger+3*m*m
      lu0=lalpha+m
      lfx=lu0+md*ngrid
      ldiv=1                 ! workspace for div can overlap with du
      lrwk1=lfx+m
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
c..set almax=0
      almax(1)=0.d0
      almax(2)=0.d0
      almax(3)=0.d0
c
c..compute dudr
      call dudr3d0c (nd,md,m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *              nd3a,nd3b,n3a,n3b,dr,ds,r,rx,gv,det,u,up,mask,
     *              rwk(ldu),rwk(ldu1),rwk(lul),rwk(lur),rwk(la0),
     *              rwk(la1),rwk(laj),rwk(lh),tau,rwk(leigal),
     *              rwk(leigel),rwk(leiger),rwk(lalpha),rwk(lu0),
     *              rwk(lfx),rwk(ldiv),nrwk1,rwk(lrwk1),niwk1,
     *              iwk(liwk1),almax,mdat,dat,ichg,av,ad,maxnstep,
     *              vismax,icart,iorder,method,n1bm,n2bm,n3bm,ier)
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
      rparam(2)=almax(1)/ds(1)+almax(2)/ds(2)+almax(3)/ds(3)
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
      subroutine dudr3d0c (nd,md,m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     & nd3a,nd3b,n3a,n3b,dr,ds,r,rx,gv,det,u,up,mask,
     & du,du1,ul,ur,a0,a1,aj,h,tau,al,el,er,alpha,u0,
     *                    fx,div,nrwk,rwk,niwk,iwk,almax,mdat,dat,ichg,
     *                    av,ad,maxnstep,vismax,icart,iorder,method,
     *                    n1bm,n2bm,n3bm,ier)
c
      implicit real*8 (a-h,o-z)
      dimension ds(3),rx(nd1a:n1bm,nd2a:n2bm,nd3a:n3bm,3,3),
     *          gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,3),
     *          det(nd1a:n1bm,nd2a:n2bm,nd3a:n3bm),
     *          u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,md),
     *          up(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,md),
     *          mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b),
     *          du(m,nd1a:nd1b,nd2a:nd2b,3,2,2),du1(m,3,2),
     *          ul(md),ur(md),a0(3,nd1a:nd1b,nd2a:nd2b,2),
     *          a1(3,3,nd1a:nd1b,nd2a:nd2b,2),
     *          aj(nd1a:nd1b,nd2a:nd2b,2),h(m,nd1a:nd1b,nd2a:nd2b),
     *          tau(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b),al(m,3),el(m,m,3),
     *          er(m,m,3),alpha(m),u0(md,nd1a:nd1b,nd2a:nd2b),fx(m),
     *          div(nd1a:nd1b,nd2a:nd2b,2),rwk(nrwk),iwk(niwk),
     *          almax(3),dat(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,*),ad(md)
      dimension eye(3,3)
      common / junk / iflag
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
c..method=1 for Roe's Riemann solver
c     data method / 1 /
c
c..iorder=1 or 2
c     data iorder / 2 /
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
c..start with layer at n3a-1
      j3=n3a-1
c
c..compute contributions for a moving grid, if necessary
      if (ichg.ne.0) then
        if (icart.eq.0) then
          do j2=nd2a,nd2b
          do j1=nd1a,nd1b
            do k=1,3
              a0(k,j1,j2,1)=-rx(j1,j2,j3,k,1)*gv(j1,j2,j3,1)
     *                      -rx(j1,j2,j3,k,2)*gv(j1,j2,j3,2)
     *                      -rx(j1,j2,j3,k,3)*gv(j1,j2,j3,3)
            end do
          end do
          end do
        else
          do j2=nd2a,nd2b
          do j1=nd1a,nd1b
            do k=1,3
              a0(k,j1,j2,1)=-rx(nd1a,nd2a,nd3a,k,1)*gv(j1,j2,j3,1)
     *                      -rx(nd1a,nd2a,nd3a,k,2)*gv(j1,j2,j3,2)
     *                      -rx(nd1a,nd2a,nd3a,k,3)*gv(j1,j2,j3,3)
            end do
          end do
          end do
        end if
      else
        do j2=nd2a,nd2b
        do j1=nd1a,nd1b
          a0(1,j1,j2,1)=0.d0
          a0(2,j1,j2,1)=0.d0
          a0(3,j1,j2,1)=0.d0
        end do
        end do
      end if
c
c..mappings
      if (icart.eq.0) then
        do j2=nd2a,nd2b
        do j1=nd1a,nd1b
          a1(1,1,j1,j2,1)=rx(j1,j2,j3,1,1)
          a1(1,2,j1,j2,1)=rx(j1,j2,j3,1,2)
          a1(1,3,j1,j2,1)=rx(j1,j2,j3,1,3)
          a1(2,1,j1,j2,1)=rx(j1,j2,j3,2,1)
          a1(2,2,j1,j2,1)=rx(j1,j2,j3,2,2)
          a1(2,3,j1,j2,1)=rx(j1,j2,j3,2,3)
          a1(3,1,j1,j2,1)=rx(j1,j2,j3,3,1)
          a1(3,2,j1,j2,1)=rx(j1,j2,j3,3,2)
          a1(3,3,j1,j2,1)=rx(j1,j2,j3,3,3)
          aj(j1,j2,1)=det(j1,j2,j3)
        end do
        end do
      else
        do k=1,2
          do j2=nd2a,nd2b
          do j1=nd1a,nd1b
            a1(1,1,j1,j2,k)=rx(nd1a,nd2a,nd3a,1,1)
            a1(1,2,j1,j2,k)=rx(nd1a,nd2a,nd3a,1,2)
            a1(1,3,j1,j2,k)=rx(nd1a,nd2a,nd3a,1,3)
            a1(2,1,j1,j2,k)=rx(nd1a,nd2a,nd3a,2,1)
            a1(2,2,j1,j2,k)=rx(nd1a,nd2a,nd3a,2,2)
            a1(2,3,j1,j2,k)=rx(nd1a,nd2a,nd3a,2,3)
            a1(3,1,j1,j2,k)=rx(nd1a,nd2a,nd3a,3,1)
            a1(3,2,j1,j2,k)=rx(nd1a,nd2a,nd3a,3,2)
            a1(3,3,j1,j2,k)=rx(nd1a,nd2a,nd3a,3,3)
            aj(j1,j2,k)=det(nd1a,nd2a,nd3a)
          end do
          end do
        end do
      end if
c
      call ovtime (time0)
c
c..slope correction
      ier=0
      if (iorder.eq.2) then
        call slope3dc (md,m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                nd3a,nd3b,j3,dr,ds,a0(1,nd1a,nd2a,1),
     *                a1(1,1,nd1a,nd2a,1),aj(nd1a,nd2a,1),u,ul,
     *                mask(nd1a,nd2a,j3),du(1,nd1a,nd2a,1,1,1),
     *                du1,h,tau(nd1a,nd2a,j3),al,el,er,
     *                nrwk,rwk,niwk,iwk,ier)
      else
        do ks=1,2
        do kd=1,3
          do j2=nd2a,nd2b
          do j1=nd1a,nd1b
            do i=1,m
              du(i,j1,j2,kd,ks,1)=0.d0
            end do
          end do
          end do
        end do
        end do
      end if
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
c..compute contributions for a moving grid, if necessary
        if (ichg.ne.0) then
          if (icart.eq.0) then
            do j2=nd2a,nd2b
            do j1=nd1a,nd1b
              do k=1,3
                a0(k,j1,j2,2)=-rx(j1,j2,j3,k,1)*gv(j1,j2,j3,1)
     *                        -rx(j1,j2,j3,k,2)*gv(j1,j2,j3,2)
     *                        -rx(j1,j2,j3,k,3)*gv(j1,j2,j3,3)
              end do
            end do
            end do
          else
            do j2=nd2a,nd2b
            do j1=nd1a,nd1b
              do k=1,3
                a0(k,j1,j2,2)=-rx(nd1a,nd2a,nd3a,k,1)*gv(j1,j2,j3,1)
     *                        -rx(nd1a,nd2a,nd3a,k,2)*gv(j1,j2,j3,2)
     *                        -rx(nd1a,nd2a,nd3a,k,3)*gv(j1,j2,j3,3)
              end do
            end do
            end do
          end if
        else
          do j2=nd2a,nd2b
          do j1=nd1a,nd1b
            a0(1,j1,j2,2)=0.d0
            a0(2,j1,j2,2)=0.d0
            a0(3,j1,j2,2)=0.d0
          end do
          end do
        end if
c
c..mappings
        if (icart.eq.0) then
          do j2=nd2a,nd2b
          do j1=nd1a,nd1b
            a1(1,1,j1,j2,2)=rx(j1,j2,j3,1,1)
            a1(1,2,j1,j2,2)=rx(j1,j2,j3,1,2)
            a1(1,3,j1,j2,2)=rx(j1,j2,j3,1,3)
            a1(2,1,j1,j2,2)=rx(j1,j2,j3,2,1)
            a1(2,2,j1,j2,2)=rx(j1,j2,j3,2,2)
            a1(2,3,j1,j2,2)=rx(j1,j2,j3,2,3)
            a1(3,1,j1,j2,2)=rx(j1,j2,j3,3,1)
            a1(3,2,j1,j2,2)=rx(j1,j2,j3,3,2)
            a1(3,3,j1,j2,2)=rx(j1,j2,j3,3,3)
            aj(j1,j2,2)=det(j1,j2,j3)
          end do
          end do
        end if
c
        call ovtime (time0)
c
c..slope correction
        if (iorder.eq.2) then
          if (m.gt.0) iflag=1
          call slope3dc (md,m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                  nd3a,nd3b,j3,dr,ds,a0(1,nd1a,nd2a,2),
     *                  a1(1,1,nd1a,nd2a,2),aj(nd1a,nd2a,2),u,ul,
     *                  mask(nd1a,nd2a,j3),du(1,nd1a,nd2a,1,1,2),
     *                  du1,h,tau(nd1a,nd2a,j3),al,el,er,
     *                  nrwk,rwk,niwk,iwk,ier)
        else
          do ks=1,2
          do kd=1,3
            do j2=nd2a,nd2b
            do j1=nd1a,nd1b
              do i=1,m
                du(i,j1,j2,kd,ks,2)=0.d0
              end do
            end do
            end do
          end do
          end do
          do j2=nd2a,nd2b
          do j1=nd1a,nd1b
            do i=1,m
              h(i,j1,j2)=0.d0
            end do
          end do
          end do
        end if
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
              ul(i)=u(j1,j2,j3m1,i)
              ur(i)=u(j1,j2,j3  ,i)
            end do
            do i=1,m
              ul(i)=ul(i)+du(i,j1,j2,3,2,1)/aj(j1,j2,1)
              ur(i)=ur(i)+du(i,j1,j2,3,1,2)/aj(j1,j2,2)
            end do
            call gdflux3dc (md,m,aj0,a30,a31,a32,a33,ul,ur,al,el,er,
     *                     alpha,almax(3),fx,method)
            do i=1,m
              up(j1,j2,j3  ,i)=                +fx(i)/ds(3)
              up(j1,j2,j3m1,i)=up(j1,j2,j3m1,i)-fx(i)/ds(3)
            end do
          end if
        end do
        end do
c
        call ovtime (time2)
        tflux=tflux+time2-time1
c
c..final source contribution for the layer j3-1
        if (j3.gt.n3a) then
          do j2=n2a,n2b
          do j1=n1a,n1b
            if (mask(j1,j2,j3m1).ne.0) then
              do i=1,m
                up(j1,j2,j3m1,i)=up(j1,j2,j3m1,i)/aj(j1,j2,1)
                du(i,j1,j2,1,1,1)=up(j1,j2,j3m1,i)
              end do
            end if
          end do
          end do
          call ovtime (time0)
          ipc=1 ! indicates corrector *wdh*
          call source3dc (md,m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                   nd3a,nd3b,j3m1,dr,u,u0,du(1,nd1a,nd2a,1,1,1),
     *                   tau(nd1a,nd2a,j3m1),mask(nd1a,nd2a,j3m1),
     *                   nrwk,rwk,niwk,iwk,maxnstep,ipc)
          call ovtime (time1)
          tsource=tsource+time1-time0
          do j2=n2a,n2b
          do j1=n1a,n1b
            if (mask(j1,j2,j3m1).ne.0) then
              do i=1,m
                up(j1,j2,j3m1,i)=up(j1,j2,j3m1,i)+du(i,j1,j2,1,1,1)
              end do
              do i=m+1,md
                up(j1,j2,j3m1,i)=0.d0
              end do
            else
              do i=1,md
                up(j1,j2,j3m1,i)=0.d0
              end do
            end if
          end do
          end do
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
          end do
          end do
          if (icart.eq.0) then
            do j2=n2a-1,n2b+1
            do j1=n1a-1,n1b+1
              aj(j1,j2,1)=aj(j1,j2,2)
              a1(1,1,j1,j2,1)=a1(1,1,j1,j2,2)
              a1(1,2,j1,j2,1)=a1(1,2,j1,j2,2)
              a1(1,3,j1,j2,1)=a1(1,3,j1,j2,2)
              a1(2,1,j1,j2,1)=a1(2,1,j1,j2,2)
              a1(2,2,j1,j2,1)=a1(2,2,j1,j2,2)
              a1(2,3,j1,j2,1)=a1(2,3,j1,j2,2)
              a1(3,1,j1,j2,1)=a1(3,1,j1,j2,2)
              a1(3,2,j1,j2,1)=a1(3,2,j1,j2,2)
              a1(3,3,j1,j2,1)=a1(3,3,j1,j2,2)
            end do
            end do
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
                  ul(i)=u(j1  ,j2,j3,i)
                  ur(i)=u(j1p1,j2,j3,i)
                end do
                do i=1,m
                  ul(i)=ul(i)+du(i,j1  ,j2,1,2,1)/aj(j1  ,j2,1)
                  ur(i)=ur(i)+du(i,j1p1,j2,1,1,1)/aj(j1p1,j2,1)
                end do
                call gdflux3dc (md,m,aj0,a10,a11,a12,a13,ul,ur,al,el,
     & er,
     *                         alpha,almax(1),fx,method)
                do i=1,m
                  up(j1p1,j2,j3,i)=up(j1p1,j2,j3,i)+fx(i)/ds(1)
                  up(j1  ,j2,j3,i)=up(j1  ,j2,j3,i)-fx(i)/ds(1)
                end do
              end if
            end do
          end do
c
c..compute s2 flux on layer j3, add it to up(.,j1,j2,j3)
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
                  ul(i)=u(j1,j2  ,j3,i)
                  ur(i)=u(j1,j2p1,j3,i)
                end do
                do i=1,m
                  ul(i)=ul(i)+du(i,j1,j2  ,2,2,1)/aj(j1,j2  ,1)
                  ur(i)=ur(i)+du(i,j1,j2p1,2,1,1)/aj(j1,j2p1,1)
                end do
                call gdflux3dc (md,m,aj0,a20,a21,a22,a23,ul,ur,al,el,
     & er,
     *                         alpha,almax(2),fx,method)
                do i=1,m
                  up(j1,j2p1,j3,i)=up(j1,j2p1,j3,i)+fx(i)/ds(2)
                  up(j1,j2  ,j3,i)=up(j1,j2  ,j3,i)-fx(i)/ds(2)
                end do
              end if
            end do
          end do
c
          call ovtime (time1)
          tflux=tflux+time1-time0
c
c..compute and save center update
          do j2=n2a,n2b
          do j1=n1a,n1b
            if (mask(j1,j2,j3).ne.0) then
              do i=1,md
                u0(i,j1,j2)=u(j1,j2,j3,i)
              end do
              do i=1,m
                u0(i,j1,j2)=u0(i,j1,j2)+h(i,j1,j2)/aj(j1,j2,1)
              end do
            end if
          end do
          end do
c
c..add free stream correction to up (if a non-Cartesian grid)
          if (icart.eq.0) then
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
     *                -(rx(j1-1,j2,j3,1,k)+rx(j1,j2,j3,1,k))*d1m)
     *                /(4*ds(1))
     *               +((rx(j1,j2+1,j3,2,k)+rx(j1,j2,j3,2,k))*d2p
     *                -(rx(j1,j2-1,j3,2,k)+rx(j1,j2,j3,2,k))*d2m)
     *                /(4*ds(2))
     *               +((rx(j1,j2,j3+1,3,k)+rx(j1,j2,j3,3,k))*d3p
     *                -(rx(j1,j2,j3-1,3,k)+rx(j1,j2,j3,3,k))*d3m)
     *                /(4*ds(3))
                  call flux3dc (md,m,eye(k,1),eye(k,2),eye(k,3),
     *                         u0(1,j1,j2),fx)
                  do i=1,m
                    up(j1,j2,j3,i)=up(j1,j2,j3,i)+da*fx(i)
                  end do
                end do
              end if
            end do
            end do
          end if
c
        end if
c
c..bottom of main loop over layers
      end do
c
c..add an artificial viscosity
      vismax=0.d0
      adMax=0.d0
      do i=1,m
        adMax=max(adMax,ad(i))
      end do
      do j3=n3a,n3b+1
        j3m1=j3-1
        if (j3.le.n3b) then
          do j2=n2a,n2b
            if (icart.eq.0) then
              do j1=n1a,n1b
                vxm=(rx(j1,j2,j3,1,1)*u(j1-1,j2,j3,2)
     *              +rx(j1,j2,j3,1,2)*u(j1-1,j2,j3,3)
     *              +rx(j1,j2,j3,1,3)*u(j1-1,j2,j3,4))/u(j1-1,j2,j3,1)
                vxp=(rx(j1,j2,j3,1,1)*u(j1+1,j2,j3,2)
     *              +rx(j1,j2,j3,1,2)*u(j1+1,j2,j3,3)
     *              +rx(j1,j2,j3,1,3)*u(j1+1,j2,j3,4))/u(j1+1,j2,j3,1)
                vym=(rx(j1,j2,j3,2,1)*u(j1,j2-1,j3,2)
     *              +rx(j1,j2,j3,2,2)*u(j1,j2-1,j3,3)
     *              +rx(j1,j2,j3,2,3)*u(j1,j2-1,j3,4))/u(j1,j2-1,j3,1)
                vyp=(rx(j1,j2,j3,2,1)*u(j1,j2+1,j3,2)
     *              +rx(j1,j2,j3,2,2)*u(j1,j2+1,j3,3)
     *              +rx(j1,j2,j3,2,3)*u(j1,j2+1,j3,4))/u(j1,j2+1,j3,1)
                vzm=(rx(j1,j2,j3,3,1)*u(j1,j2,j3-1,2)
     *              +rx(j1,j2,j3,3,2)*u(j1,j2,j3-1,3)
     *              +rx(j1,j2,j3,3,3)*u(j1,j2,j3-1,4))/u(j1,j2,j3-1,1)
                vzp=(rx(j1,j2,j3,3,1)*u(j1,j2,j3+1,2)
     *              +rx(j1,j2,j3,3,2)*u(j1,j2,j3+1,3)
     *              +rx(j1,j2,j3,3,3)*u(j1,j2,j3+1,4))/u(j1,j2,j3+1,1)
                div(j1,j2,2)= (vxp-vxm)/(2*ds(1))+(vyp-vym)/(2*ds(2))
     *                       +(vzp-vzm)/(2*ds(3))
                if (mask(j1,j2,j3).ne.0) then
                  vismax=max(-div(j1,j2,2),vismax)
                end if
              end do
            else
              a11=rx(nd1a,nd2a,nd3a,1,1)/(2*ds(1))
              a22=rx(nd1a,nd2a,nd3a,2,2)/(2*ds(2))
              a33=rx(nd1a,nd2a,nd3a,3,3)/(2*ds(3))
              do j1=n1a,n1b
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
            end if
            div(n1a-1,j2,2)=div(n1a,j2,2)
            div(n1b+1,j2,2)=div(n1b,j2,2)
          end do
          do j1=n1a-1,n1b+1
            div(j1,n2a-1,2)=div(j1,n2a,2)
            div(j1,n2b+1,2)=div(j1,n2b,2)
          end do
          if (j3.eq.n3a) then
            do j2=n2a-1,n2b+1
            do j1=n1a-1,n1b+1
              div(j1,j2,1)=div(j1,j2,2)
            end do
            end do
          end if
        end if
        do j2=n2a,n2b
        do j1=n1a,n1b
          if (mask(j1,j2,j3).ne.0.and.mask(j1,j2,j3m1).ne.0) then
            vis=av*max(0.d0,-(div(j1,j2,1)+div(j1,j2,2))/2)
            do i=1,m
              fx(i)=(vis+ad(i))*(u(j1,j2,j3,i)-u(j1,j2,j3m1,i))
              up(j1,j2,j3  ,i)=up(j1,j2,j3  ,i)-fx(i)
              up(j1,j2,j3m1,i)=up(j1,j2,j3m1,i)+fx(i)
            end do
          end if
        end do
        end do
        if (j3.le.n3b) then
          do j2=n2a-1,n2b+1
          do j1=n1a-1,n1b+1
            div(j1,j2,1)=div(j1,j2,2)
          end do
          end do
          do j1=n1a-1,n1b
            j1p1=j1+1
            do j2=n2a,n2b
              if (mask(j1,j2,j3).ne.0.and.mask(j1p1,j2,j3).ne.0) then
                vis=av*max(0.d0,-(div(j1,j2,1)+div(j1p1,j2,1))/2)
                do i=1,m
                  fx(i)=(vis+ad(i))*(u(j1p1,j2,j3,i)-u(j1,j2,j3,i))
                  up(j1p1,j2,j3,i)=up(j1p1,j2,j3,i)-fx(i)
                  up(j1  ,j2,j3,i)=up(j1  ,j2,j3,i)+fx(i)
                end do
              end if
            end do
          end do
          do j2=n2a-1,n2b
            j2p1=j2+1
            do j1=n1a,n1b
              if (mask(j1,j2,j3).ne.0.and.mask(j1,j2p1,j3).ne.0) then
                vis=av*max(0.d0,-(div(j1,j2,1)+div(j1,j2p1,1))/2)
                do i=1,m
                  fx(i)=(vis+ad(i))*(u(j1,j2p1,j3,i)-u(j1,j2,j3,i))
                  up(j1,j2p1,j3,i)=up(j1,j2p1,j3,i)-fx(i)
                  up(j1,j2  ,j3,i)=up(j1,j2  ,j3,i)+fx(i)
                end do
              end if
            end do
          end do
        end if
      end do
      vismax=av*vismax+adMax
c
c..extrapolate to ghost cells
      call upxtrp3dc (md,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *               nd3a,nd3b,n3a,n3b,up)
c
      return
      end
c
c++++++++++++++++
c
      subroutine slope3dc (md,m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                    nd3a,nd3b,j3,dr,ds,a0,a1,aj,u,u0,mask,du,
     *                    du1,h,tau,al,el,er,nrwk,rwk,niwk,iwk,ier)
c
      implicit real*8 (a-h,o-z)
      dimension a0(3,nd1a:nd1b,nd2a:nd2b),a1(3,3,nd1a:nd1b,nd2a:nd2b),
     *          aj(nd1a:nd1b,nd2a:nd2b),mask(nd1a:nd1b,nd2a:nd2b),
     *          du(m,nd1a:nd1b,nd2a:nd2b,3,2),h(m,nd1a:nd1b,nd2a:nd2b),
     *          u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,md),u0(md),du1(m,3,2),
     *          al(m,3),el(m,m,3),tau(nd1a:nd1b,nd2a:nd2b),er(m,m,3),
     *          ds(3),rwk(nrwk),iwk(niwk)
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
            du(i,j1,j2,1,1)=u(j1,j2,j3,i)
          end do
        end do
      end do
      ipc=0 ! indicates predictor *wdh*
      call source3dc (md,m,nd1a,nd1b,n1a-1,n1b+1,nd2a,nd2b,n2a-1,n2b+1,
     *               nd3a,nd3b,j3,c0,u,du(1,nd1a,nd2a,1,1),h,tau,mask,
     *               nrwk,rwk,niwk,iwk,maxnstep,ipc)
c
c..solution differences: s1-direction
      do j2=n2a-1,n2b+1
        do j1=n1a,n1b+1
          do i=1,m
            du(i,j1  ,j2,1,1)=u(j1,j2,j3,i)-u(j1-1,j2,j3,i)
            du(i,j1-1,j2,1,2)=du(i,j1,j2,1,1)
          end do
        end do
        do i=1,m
          du(i,n1a-1,j2,1,1)=u(n1a-1,j2,j3,i)-u(n1a-2,j2,j3,i)
          du(i,n1b+1,j2,1,2)=u(n1b+2,j2,j3,i)-u(n1b+1,j2,j3,i)
c         du(i,n1a-1,j2,1,1)=du(i,n1a,j2,1,1)
c         du(i,n1b+1,j2,1,2)=du(i,n1b,j2,1,2)
        end do
      end do
c
c..solution differences: s2-direction
      do j1=n1a-1,n1b+1
        do j2=n2a,n2b+1
          do i=1,m
            du(i,j1,j2  ,2,1)=u(j1,j2,j3,i)-u(j1,j2-1,j3,i)
            du(i,j1,j2-1,2,2)=du(i,j1,j2,2,1)
          end do
        end do
        do i=1,m
          du(i,j1,n2a-1,2,1)=u(j1,n2a-1,j3,i)-u(j1,n2a-2,j3,i)
          du(i,j1,n2b+1,2,2)=u(j1,n2b+2,j3,i)-u(j1,n2b+1,j3,i)
c         du(i,j1,n2a-1,2,1)=du(i,j1,n2a,2,1)
c         du(i,j1,n2b+1,2,2)=du(i,j1,n2b,2,2)
        end do
      end do
c
c..solution differences: s3-direction
      do j2=n2a-1,n2b+1
      do j1=n1a-1,n1b+1
        do i=1,m
          du(i,j1,j2,3,1)=u(j1,j2,j3  ,i)-u(j1,j2,j3-1,i)
          du(i,j1,j2,3,2)=u(j1,j2,j3+1,i)-u(j1,j2,j3  ,i)
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
            u0(i)=u(j1,j2,j3,i)
          end do
          call eigenv3dc (md,m,a1(1,1,j1,j2),u0,al,el,er,ier)
c
          if (ier.ne.0) then
            write(*,*) 'ERROR return from eigenv for j1,j2,j3=',j1,j2,
     & j3
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
      subroutine eigenv3dc (md,m,a,u,al,el,er,ier)
c
      implicit real*8 (a-h,o-z)
      dimension a(3,3),u(md),al(m,3),el(m,m,3),er(m,m,3)
      dimension an(3),t1(3),t2(3),dp(10)
      common / mvars / mh,mr,me,ieos,irxn
c
      ier=0
c
c..first compute direction-free part
      v1=u(2)/u(1)
      v2=u(3)/u(1)
      v3=u(4)/u(1)
      q2=v1**2+v2**2+v3**2
c
      call getp3dc (md,u,p,dp,mr+2,ier)
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
      c2=dp(1)+(h-q2)*dp(2)+sum
      if (c2.le.0.d0) then
        write(6,*)'Error (eigenv3d) : cannot compute sound speed'
        write(6,*)'u =',(u(i),i=1,m)
        write(6,*)'p =',p
        ier=123
        return
      end if
      c=dsqrt(c2)
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
      subroutine gdflux3dc (md,m,aj,a0,a1,a2,a3,ul,ur,al,el,er,alpha,
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
      dimension ul(md),ur(md),al(m,3),el(m,m),er(m,m),alpha(m),f(m)
      data eps / 1.d-14 /
c
      if     (method.eq.1) then
c
c..Roe's approximate Riemann solver
        include 'roe3d.h'
c
c     elseif (method.eq.2) then
c
c..Saltzman's approximate Riemann solver
c       include 'saltz2.h'
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
      return
      end
c
c+++++++++++++++++++++
c
      subroutine flux3dc (md,m,a1,a2,a3,u,f)
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
      call getp3dc (md,u,p,dp,0,ier)
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
      subroutine roeavg3dc (md,m,a1,a2,a3,ul,ur,al,el,er)
c
c supply eigenvalues and eigenvectors using an appropriate average value
c of u=u(ul,ur) for Roe's Riemann solver
c
      implicit real*8 (a-h,o-z)
      dimension ul(md),ur(md),al(m),el(m,m),er(m,m),alaml(8),alamr(8),
     *          alam(8),an(3),dpl(10),dpr(10),dp(10)
      common / mvars / mh,mr,me,ieos,irxn
      common / junk / iflag
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
      call getp3dc (md,ul,pl,dpl,mr+2,ier)
      hl=(ul(5)+pl)/rhol
c
      rhor=ur(1)
      v1r=ur(2)/rhor
      v2r=ur(3)/rhor
      v3r=ur(4)/rhor
      call getp3dc (md,ur,pr,dpr,mr+2,ier)
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
      if (ieos.eq.0.or.ieos.eq.1) then
        dp(1)=dp(2)*q2/2.d0
      else
        tol=1.d-3
        p1=pl
        do i=1,md
          al(i)=ul(i)
        end do
        delta=ur(1)-ul(1)
        if (dabs(delta).gt.tol) then
          al(1)=ur(1)
          call getp3dc (md,al,p,dpl,0,ier)
          dp(1)=(p-p1)/delta
          p1=p
        end if
        do k=1,mr
          delta=ur(5+k)-ul(5+k)
          if (dabs(delta).gt.tol) then
            al(5+k)=ur(5+k)
            call getp3dc (md,al,p,dpl,0,ier)
            dp(2+k)=(p-p1)/delta
            p1=p
          end if
        end do
        delta=ur(5)-ul(5)-v1*(ur(2)-ul(2))
     *                   -v2*(ur(3)-ul(3))
     *                   -v3*(ur(4)-ul(4))
        if (dabs(delta).gt.tol) then
          dp(2)=(pr-p1)/delta
          if (dabs(dp(2)).lt.tol) then
            write(6,*)'Error (roeavg3d) : dp(2) too small'
            stop
          end if
        end if
      end if
c
      sum=0.d0
      do k=1,mr
        sum=sum+alam(k)*dp(2+k)
      end do
      c2=dp(1)+(h-q2)*dp(2)+sum
c
      if (c2.le.0.d0) then
        write(6,*)'Error (roeavg3d) : c2.le.0'
        stop
      end if
      c=dsqrt(c2)
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
      subroutine eigenr3dc (md,m,a0,a1,a2,a3,ul,ur,all,alr,isign)
c
c compute C- or C+ characteristic for states ul and ur
c
      implicit real*8 (a-h,o-z)
      dimension ul(md),ur(md),an(3),dp(10)
      common / mvars / mh,mr,me,ieos,irxn
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
      call getp3dc (md,ul,p,dp,mr+2,ier)
      h=(ul(5)+p)/ul(1)
      c2=dp(1)+(h-q2)*dp(2)
      do k=1,mr
        c2=c2+ul(5+k)*dp(2+k)/ul(1)
      end do
      all=a0+r*(vn+isign*dsqrt(c2))
c
      v1=ur(2)/ur(1)
      v2=ur(3)/ur(1)
      v3=ur(4)/ur(1)
      q2=v1**2+v2**2+v3**2
      vn=an(1)*v1+an(2)*v2+an(3)*v3
      call getp3dc (md,ur,p,dp,mr+2,ier)
      h=(ur(5)+p)/ur(1)
      c2=dp(1)+(h-q2)*dp(2)
      do k=1,mr
        c2=c2+ur(5+k)*dp(2+k)/ur(1)
      end do
      alr=a0+r*(vn+isign*dsqrt(c2))
c
      return
      end
c
c+++++++++++++++++++++
c
      subroutine source3dc (md,m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                     nd3a,nd3b,j3,dr,u,u0,up2,tau,mask,nrwk,rwk,
     *                     niwk,iwk,maxnstep,ipc)
c
c Requires (5*mr+md+3)*ngrid for real workspace and 3*ngrid for
c integer workspace
c
      implicit real*8 (a-h,o-z)
      dimension u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,md),
     *          u0(md,nd1a:nd1b,nd2a:nd2b),up2(m,nd1a:nd1b,nd2a:nd2b),
     *          tau(nd1a:nd1b,nd2a:nd2b),mask(nd1a:nd1b,nd2a:nd2b),
     *          rwk(nrwk),iwk(niwk)
      common / mvars / mh,mr,me,ieos,irxn
c
      maxnstep=0
c
      if (mr.eq.0) then
c..Euler equations (perhaps with some EOS variables)
        do j2=n2a,n2b
        do j1=n1a,n1b
          do i=1,m
            up2(i,j1,j2)=0.d0
          end do
        end do
        end do
        return
      end if
c
c..Reactive Euler....
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
c zero out error estimate
      do j2=nd2a,nd2b
        do j1=nd1a,nd1b
          tau(j1,j2)=0.d0
        end do
      end do
c
      nlam=mr
      call getsrc3dc (md,m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,nd3a,
     & nd3b,
     *               j3,dr,u,u0,up2,tau,mask,rwk(ltest),rwk(lrk),
     *               rwk(lhk),rwk(luk),rwk(lwk),rwk(lc),rwk(lwpk),
     *               iwk(ljk),iwk(lnstep),maxnstep,nlam,ng,ipc)
c
c     taumax=0.d0
c     do j2=nd2a,nd2b
c       do j1=nd1a,nd1b
c         taumax=max(tau(j1,j2),taumax)
c       end do
c     end do
c     write(6,*)'*** dr, taumax = ',dr,taumax
c     pause
c
      if (me.gt.0) then
        i1=6+mr
        do j2=n2a,n2b
        do j1=n1a,n1b
          do i=i1,m
            up2(i,j1,j2)=0.d0
          end do
        end do
        end do
      end if
c
      return
      end
c
c+++++++++++++++++++++
c
      subroutine getsrc3dc (md,m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                     nd3a,nd3b,j3,dr,u,u0,up2,tau,mask,test,rk,
     *                     hk,uk,wk,c,wpk,jk,nstep,maxnstep,nlam,ng,
     *                     ipc)
c
      implicit real*8 (a-h,o-z)
      dimension u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,md),
     *          u0(md,nd1a:nd1b,nd2a:nd2b),up2(m,nd1a:nd1b,nd2a:nd2b),
     *          tau(nd1a:nd1b,nd2a:nd2b),mask(nd1a:nd1b,nd2a:nd2b),
     *          test(ng),rk(ng),hk(ng),uk(md,ng),wk(nlam,ng),
     *          c(nlam,ng,3),wpk(nlam,ng),jk(2,ng),
     *          nstep(n1a:n1b,n2a:n2b)
      common / srcdat / nb1a,nb1b,nb2a,nb2b,nb3a,nb3b,icount
c     data tol, tiny / 1.d-5, 1.d-14 /
      data tol, tiny / .0001d0, 1.d-14 /
c
c..It is assumed that alam=u(j1,j2,j3,6)/u(j1,j2,j3,1)=1 is an equilibrium value.
c  Thus, if alam is close enough to 1, then set up2(6) so that alam will equal
c  1 and return.
      n=0
      do j2=n2a,n2b
        do j1=n1a,n1b
          nstep(j1,j2)=0
          if (mask(j1,j2).ne.0) then
            alam=u(j1,j2,j3,6)/u(j1,j2,j3,1)
            if (nlam.eq.1.and.alam.gt.1.d0-1.d-6) then
              up2(6,j1,j2)=(u(j1,j2,j3,1)-u(j1,j2,j3,6))/dr
     *                      +up2(1,j1,j2)-up2(6,j1,j2)
              do i=1,5
                up2(i,j1,j2)=0.d0
              end do
            else
              n=n+1
              jk(1,n)=j1
              jk(2,n)=j2
              rk(n)=0.d0
              hk(n)=dr
              do i=1,md
                uk(i,n)=u0(i,j1,j2)
              end do
              do i=1,nlam
                wk(i,n)=u(j1,j2,j3,i+5)
              end do
            end if
          else
            do i=1,m
              up2(i,j1,j2)=0.d0
            end do
          end if
        end do
      end do
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
        call rate3dc (md,nlam,n,uk,wk,wpk)
        do k=1,n
          do i=1,nlam
            c(i,k,1)=hk(k)*wpk(i,k)
            c(i,k,2)=wk(i,k)+.5d0*c(i,k,1)
          end do
        end do
c
        call rate3dc (md,nlam,n,uk,c(1,1,2),wpk)
        do k=1,n
          do i=1,nlam
            c(i,k,2)=hk(k)*wpk(i,k)
            c(i,k,3)=wk(i,k)+.75d0*c(i,k,2)
          end do
        end do
c
        call rate3dc (md,nlam,n,uk,c(1,1,3),wpk)
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
              alam=u(j1,j2,j3,6)/u(j1,j2,j3,1)
              write(*,9000) j1,j2,mask(j1,j2),(u(j1,j2,j3,i),i=1,m)
              write(*,9100) test(k),tol,hk(k),alam,
     &          (u0(i,j1,j2),i=1,m)     
 9100         format(1x,' test=',e9.2,' tol=',e9.2,' hk=',e9.2,
     &               ' alam=',e10.3,' u0=',12(e11.3,1x))
              write(*,9200) (uk(i,k),i=1,5),(wk(i,k),i=1,nlam),
     &                      (c(i,k,1),i=1,nlam),(c(i,k,2),i=1,nlam),
     &                      (c(i,k,3),i=1,nlam)
 9200         format(' uk=',5(e11.2,1x),' wk=',(e11.2,1x),' c = ',
     &             3(e11.3,1x))
              write(*,9300) (up2(i,j1,j2),i=1,5+nlam)
 9300         format(' up2=',2(e11.3,1x))
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
              rho=u(j1,j2,j3,1)+dr*up2(1,j1,j2)
              do i=1,nlam
                upl=(wk(i,k)-u(j1,j2,j3,i+5))/dr
c               alam=(u(j1,j2,j3,i+5)+dr*(up2(i+5,j1,j2)+upl))/rho
c               if (alam.gt.1.d0) then
c                 upl=(u(j1,j2,j3,1)-u(j1,j2,j3,6))/dr
c    *                    +up2(1,j1,j2)-up2(6,j1,j2)
c               end if
                up2(i+5,j1,j2)=upl
              end do              
              do i=1,5
                up2(i,j1,j2)=0.d0
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
      subroutine rate3dc (md,nlam,n,u,w,wp)
c
      implicit real*8 (a-h,o-z)
      dimension u(md,n),w(nlam,n),wp(nlam,n),dp(10)
      common / mvars / mh,mr,me,ieos,irxn
      common / mydata / gam,ht(0:2),at(2),pr(2)
c
      if (nlam.gt.2) then
        write(6,*)'Error (rate) : nlam.gt.2'
        stop
      end if
c
      gm1=gam-1
      if (irxn.eq.1) then
        do k=1,n
          rho=u(1,k)
          u(6,k)=w(1,k)
          call getp3dc (md,u(1,k),p,dp,0,ier)
          temp=p/rho
          prod=w(1,k)/rho
          fuel=1.d0-prod
          ak1=pr(1)*dexp(-at(1)/temp)
          ak1=.01d0
          wp(1,k)=rho*fuel*ak1
        end do
      elseif (irxn.eq.2) then
c ak1=reaction rate for chain-initiation
c ak2=reaction rate for chain-branching
        do k=1,n
          rho=u(1,k)
          prod=w(1,k)/rho
          rdcl=w(2,k)/rho
          fuel=1.d0-prod-rdcl
          u(6,k)=prod
          u(7,k)=rdcl
          call getp3dc (md,u(1,k),p,dp,0,ier)
          temp=p/rho
          ak1=pr(1)*dexp(-at(1)/temp)
          ak2=pr(2)*dexp(-at(2)/temp)
          wp(1,k)=rho*rdcl
          wp(2,k)=rho*(fuel*(ak1+rdcl*ak2)-rdcl)
        end do
      elseif (irxn.eq.3) then
        do k=1,n
          rho=u(1,k)
          u(6,k)=w(1,k)
          call getp3dc (md,u(1,k),p,dp,0,ier)
          temp=p/rho
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
      subroutine getp3dc (md,u,p,dp,ideriv,ier)

c compute pressure p (ideriv=0,default) and derivatives of p (ideriv>0)

      implicit real*8 (a-h,o-z)
      dimension u(md),dp(10)
      common / mvars / mh,mr,me,ieos,irxn
      common / mydata / gam,ht(0:2),at(2),pr(2)
c
      if (mr+2.gt.10) then
        write(6,*)'Error (getp3d) : mr too big'
        stop
      end if
c
      ier=0
      if (ieos.eq.0.or.ieos.eq.1) then
        gm1=gam-1.d0
        q2=(u(2)/u(1))**2+(u(3)/u(1))**2+(u(4)/u(1))**2
        p=u(5)-.5d0*u(1)*q2
        do k=1,mr
          p=p-ht(k)*u(5+k)
        end do
        p=gm1*p
        if (ideriv.gt.0) then
          dp(1)=.5d0*gm1*q2
          dp(2)=gm1
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
      subroutine upxtrp3dc (md,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
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
      subroutine mondat3dc (md,m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                     nd3a,nd3b,n3a,n3b,r,dr,u,u0,mask)
c
      implicit real*8 (a-h,o-z)
      dimension u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,md),umin(9),umax(9),
     *          uavg(9),mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b),dp(10),
     *          u0(md)
      character*10 label(9)
      common / mvars / mh,mr,me,ieos,irxn
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
          call getp3dc (md,u0,p,dp,0,ier)
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
