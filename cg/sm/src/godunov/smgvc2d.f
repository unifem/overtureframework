      subroutine smgvc2d (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                    ds1,ds2,dt,t,xy,rx,det,u,up,f1,f2,ad,
     *                    ndMatProp,matIndex,matValpc,matVal,mask,
     *                    nrprm,rparam,niprm,iparam,nrwk,rwk,niwk,
     *                    iwk,idebug,ier)
c
c compute du/dt for 2d linear elasticity, variable coefficients (smg => solid mechanics, Godunov)
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
c
c arrays for variable material properties --
      integer matIndex(nd1a:nd1b,nd2a:nd2b)
      real*8 matValpc(ndMatProp,0:*)
      real*8 matVal(nd1a:nd1b,nd2a:nd2b,1:*)
c
      common / smgvar / amu,alam,rho0,mformat
      common / smgprm / method,iorder,ilimit,iupwind,icart,ifrc
      common / tzflow / eptz,itz
c
c      data ad / .5d0, .5d0, .5d0, .5d0, .5d0, .5d0, .5d0, .5d0 /
c      do i=1,m
c        ad(i)=0.d0
c      end do
c
c      write(6,*)'Message (smgvc2d) : smgvc2d starting...'
c
c..data dump for debugging
c      call smgdmpv (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
c     *              ds1,ds2,dt,t,xy,rx,det,u,f1,f2,ad,
c     *              mask,nrprm,rparam,niprm,iparam,
c     *              ndMatProp,matIndex,matValpc,matVal,
c     *              nrwk,niwk)
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
        write(*,'(" ++ smgvc2d:INFO: Limiting is ON")') 
      endif 
      iupwind=iparam(6)
      itype=iparam(7)
      ifrc=iparam(8)
      if( itype.ne.0 )then
         write(*,*) 'smgvc2d:ERROR: invalid itype'
         stop 8873
      end if

c stress relaxation
      iRelax=iparam(9)
      relaxAlpha=rparam(7)
      relaxDelta=rparam(8)

c format for material properties
      mformat=iparam(11)

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
c      write(6,*)'******* Setting iorder=1 (smgvc2d) ********'
c       write(6,*)'*** iorder =',iorder
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
      ngrid=(md1b-md1a+1)
c
c..split up real work space =>
      lw=1
      la1=lw+m*ngrid*10
      laj=la1+8*ngrid
      lvc=laj+2*ngrid-1
      nreq=lvc+10*ngrid-1
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
      call getup2dv (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
     *               md2a,md2b,n2a,n2b,ds1,ds2,dt,t,xy,rx,det,
     *               ndMatProp,matIndex,matValpc,matVal,u,up,
     *               f1,f2,mask,almax,rwk(lw),rwk(la1),rwk(laj),
     *               rwk(lvc),ier)
c

c      do j2=n2a,n2b
c      do j1=n1a,n1b
c        write(6,333)j1,j2,(up(j1,j2,i),i=1,6)
c  333   format(2(1x,i3),6(1x,1pe10.3))
c      end do
c      end do
c      pause

c
c add relaxation term to ensure compatibility of stress and position
c   jwb -- 10 Aug 2010
      if( iRelax.ne.0 ) then
c        write(6,*)'Error (smgvc2d) : iRelax not implemented'
c        pause
        call stressRelax2dv (m,nd1a,nd1b,nd2a,nd2b,n1a,n1b,n2a,n2b,
     *                       ds1,ds2,dt,t,xy,rx,u,up,mask,
     *                       ndMatProp,matIndex,matValpc,matVal,
     *                       iRelax,relaxAlpha,relaxDelta)
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
c           write(6,*)'here i am : i,ad=',i,adi
c           pause
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
      subroutine smgdmpv (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                    ds1,ds2,dt,t,xy,rx,det,u,f1,f2,ad,
     *                    mask,nrprm,rparam,niprm,iparam,
     *                    ndMatProp,matIndex,matValpc,matVal,
     *                    nrwk,niwk)
c
      implicit real*8 (a-h,o-z)
      dimension xy(nd1a:nd1b,nd2a:nd2b,2),rx(nd1a:nd1b,nd2a:nd2b,2,2),
     *          det(nd1a:nd1b,nd2a:nd2b),u(nd1a:nd1b,nd2a:nd2b,m),
     *          f1(nd1a:nd1b,nd2a:nd2b,2),f2(nd1a:nd1b,nd2a:nd2b,2),
     *          mask(nd1a:nd1b,nd2a:nd2b),rparam(nrprm),iparam(niprm),
     *          ad(m)
c
c arrays for variable material properties --
      integer matIndex(nd1a:nd1b,nd2a:nd2b)
      real*8 matValpc(ndMatProp,0:*)
      real*8 matVal(nd1a:nd1b,nd2a:nd2b,1:*)
c
c
      write(6,*)'Message (smgdmpv) : writing data for debugging'
c
      write(89,100)m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *             ds1,ds2,dt,t
  100 format(9(1x,i6),/,4(1x,1pe22.15))
c
      write(89,200)nrprm
  200 format(1x,i6)
      do i=1,nrprm
        write(89,210)i,rparam(i)
  210   format(1x,i3,1x,1pe22.15)
      end do
      write(89,300)niprm
  300 format(1x,i6)
      do i=1,niprm
        write(89,310)i,iparam(i)
  310   format(1x,i3,1x,i6)
      end do
c
      write(89,350)nrwk,niwk
  350 format(2(1x,i6))
c
c format for material properties
      mformat=iparam(11)
      maxMatIndex=0
      if (mformat.eq.1) then
        do j1=nd1a,nd1b
        do j2=nd2a,nd2b
          maxMatIndex=max(matIndex(j1,j2),maxMatIndex)
        end do
        end do
      end if
c
      write(89,370)ndMatProp,maxMatIndex
  370 format(2(1x,i3))
c
c      write(6,*)iparam(2),iparam(3),iparam(7),iparam(8)
c      pause
c
      do i=1,m
        write(89,210)i,ad(i)
      end do
c
      do j1=nd1a,nd1b
      do j2=nd2a,nd2b
        if (mask(j1,j2).eq.0) then
          write(89,400)j1,j2
  400     format(2(1x,i6),'  0')
        else
          write(89,401)j1,j2
  401     format(2(1x,i6),'  1')
        end if
        do i=1,m
          write(89,210)i,u(j1,j2,i)
        end do
      end do
      end do
c
      if (iparam(2).eq.0.or.iparam(3).ne.0) then
        do j1=nd1a,nd1b
        do j2=nd2a,nd2b
          write(89,500)(xy(j1,j2,i1),i1=1,2)
  500     format(2(1x,1pe22.15))
        end do
        end do
      end if
c
      if (iparam(2).eq.0) then
        do j1=nd1a,nd1b
        do j2=nd2a,nd2b
          write(89,600)((rx(j1,j2,i1,i2),i1=1,2),i2=1,2),det(j1,j2)
  600     format(5(1x,1pe22.15))
        end do
        end do
      end if
c
      if (iparam(8).ne.0) then
        do j1=nd1a,nd1b
        do j2=nd2a,nd2b
          write(89,700)(f1(j1,j2,i1),i1=1,2),(f2(j1,j2,i1),i1=1,2)
  700     format(4(1x,1pe22.15))
        end do
        end do
      end if
c
      if (mformat.eq.1) then
        do j1=nd1a,nd1b
        do j2=nd2a,nd2b
          write(89,800)matIndex(j1,j2)
  800     format(1x,i3)
        end do
        end do
        do k=0,maxMatIndex
          do i=1,3
            write(89,810)matValpc(i,k)
  810       format(1x,1pe22.15)
          end do
        end do
      elseif (mformat.eq.2) then
        do i=1,3
          do j1=nd1a,nd1b
          do j2=nd2a,nd2b
            write(89,810)matVal(j1,j2,i)
          end do
          end do
        end do
      end if
c
      write(6,*)'Message (smgdmpv) : stopping...'
c
      stop
      end
c
c+++++++++++++++++++++++
c
      subroutine getup2dv (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
     *                     md2a,md2b,n2a,n2b,ds1,ds2,dt,t,xy,rx,det,
     *                     ndMatProp,matIndex,matValpc,matVal,u,up,
     *                     f1,f2,mask,almax,w,a1,aj,vc,ier)
c
      implicit real*8 (a-h,o-z)
      dimension xy(nd1a:nd1b,nd2a:nd2b,2),rx(nd1a:nd1b,nd2a:nd2b,2,2),
     *          det(nd1a:nd1b,nd2a:nd2b),u(nd1a:nd1b,nd2a:nd2b,m),
     *          up(nd1a:nd1b,nd2a:nd2b,m),f1(nd1a:nd1b,nd2a:nd2b,2),
     *          f2(nd1a:nd1b,nd2a:nd2b,2),mask(nd1a:nd1b,nd2a:nd2b),
     *          almax(2),w(m,md1a:md1b,5,2),a1(2,2,md1a:md1b,2),
     *          aj(md1a:md1b,2),vc(5,md1a:md1b,2)
      integer matIndex(nd1a:nd1b,nd2a:nd2b)
      real*8 matValpc(ndMatProp,0:*)
      real*8 matVal(nd1a:nd1b,nd2a:nd2b,1:*)
      dimension fx(6,2),htz(8),fxtmp(6),errf(6),ul(6),ur(6),
     *          du1(6),du2(6),ut(6)
      common / smgprm / method,iorder,ilimit,iupwind,icart,ifrc
      common / tzflow / eptz,itz
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
c      if (iorder.ne.1.and.itz.eq.0.and.
c     *    ilimit.eq.0.and.iupwind.eq.0) then
c        if (icart.ne.0) then
c          c1x=dt/(rho0*ds1)
c          c2x=c1x
c          c3x=(alam+2.d0*amu)*dt/ds1
c          c4x=amu*dt/ds1
c          c5x=c4x
c          c6x=alam*dt/ds1
c          c1y=dt/(rho0*ds2)
c          c2y=c1y
c          c3y=alam*dt/ds2
c          c4y=amu*dt/ds2
c          c5y=c4y
c          c6y=(alam+2.d0*amu)*dt/ds2
c        else
c          c11r=dt/(rho0*ds1)
c          c12r=c11r
c          c21r=c11r
c          c22r=c11r
c          c31r=(alam+2.d0*amu)*dt/ds1
c          c32r=alam*dt/ds1
c          c41r=amu*dt/ds1
c          c42r=c41r
c          c51r=c41r
c          c52r=c41r
c          c61r=c32r
c          c62r=c31r
c          c11s=dt/(rho0*ds2)
c          c12s=c11s
c          c21s=c11s
c          c22s=c11s
c          c31s=(alam+2.d0*amu)*dt/ds2
c          c32s=alam*dt/ds2
c          c41s=amu*dt/ds2
c          c42s=c41s
c          c51s=c41s
c          c52s=c41s
c          c61s=c32s
c          c62s=c31s
c        end if
c      end if
c
      j2=n2a-1
c
c..set grid metrics and velocity
      call smmetrics2dv (nd1a,nd1b,md1a,md1b,nd2a,nd2b,j2,rx,det,
     *                   ndMatProp,matIndex,matValpc,matVal,
     *                   a1(1,1,md1a,1),aj(md1a,1),vc(1,md1a,1))
c
      if (icart.eq.0) then
c        if (.true. .or.iorder.eq.1.or.itz.ne.0.or.
        if (iorder.eq.1.or.itz.ne.0.or.
     *      ilimit.ne.0.or.iupwind.ne.0) then
          call smslope2dv (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
     *                     j2,ds1,ds2,dt,t,xy,a1(1,1,md1a,1),
     *                     aj(md1a,1),vc(1,md1a,1),mask(nd1a,j2),
     *                     u,w(1,md1a,1,1),f1)
        else
          do j1=n1a-1,n1b+1                      ! optimized for ilimit=iupwind=0 (curvilinear case)
            if (mask(j1,j2).ne.0) then
              do i=1,6
                du1(i)=.25d0*(u(j1+1,j2,i)-u(j1-1,j2,i))
                du2(i)=.25d0*(u(j1,j2+1,i)-u(j1,j2-1,i))
              end do
              rho0=vc(1,j1,1)
              amu=vc(2,j1,1)
              alam=vc(3,j1,1)
              akap=alam+2.d0*amu
              ut(1)=u(j1,j2,1)+dt*((rx(j1,j2,1,1)*du1(3)
     *                             +rx(j1,j2,1,2)*du1(5))/ds1
     *                            +(rx(j1,j2,2,1)*du2(3)
     *                             +rx(j1,j2,2,2)*du2(5))/ds2)/rho0
              ut(2)=u(j1,j2,2)+dt*((rx(j1,j2,1,1)*du1(4)
     *                             +rx(j1,j2,1,2)*du1(6))/ds1
     *                            +(rx(j1,j2,2,1)*du2(4)
     *                             +rx(j1,j2,2,2)*du2(6))/ds2)/rho0
              ut(3)=u(j1,j2,3)+dt*((rx(j1,j2,1,1)*akap*du1(1)
     *                             +rx(j1,j2,1,2)*alam*du1(2))/ds1
     *                            +(rx(j1,j2,2,1)*akap*du2(1)
     *                             +rx(j1,j2,2,2)*alam*du2(2))/ds2)
              ut(4)=u(j1,j2,4)+dt*amu*((rx(j1,j2,1,1)*du1(2)
     *                                 +rx(j1,j2,1,2)*du1(1))/ds1
     *                                +(rx(j1,j2,2,1)*du2(2)
     *                                 +rx(j1,j2,2,2)*du2(1))/ds2)
              ut(5)=ut(4)
              ut(6)=u(j1,j2,6)+dt*((rx(j1,j2,1,1)*alam*du1(1)
     *                             +rx(j1,j2,1,2)*akap*du1(2))/ds1
     *                            +(rx(j1,j2,2,1)*alam*du2(1)
     *                             +rx(j1,j2,2,2)*akap*du2(2))/ds2)
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
     *      ilimit.ne.0.or.iupwind.ne.0) then
          call smslope2dvc (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
     *                      j2,ds1,ds2,dt,t,xy,vc(1,md1a,1),
     *                      mask(nd1a,j2),u,w(1,md1a,1,1),f1)
        else
          do j1=n1a-1,n1b+1                      ! optimized for ilimit=iupwind=0 (Cartesian case)
            if (mask(j1,j2).ne.0) then
              do i=1,6
                du1(i)=.25d0*(u(j1+1,j2,i)-u(j1-1,j2,i))
                du2(i)=.25d0*(u(j1,j2+1,i)-u(j1,j2-1,i))
              end do
              rho0=vc(1,j1,1)
              amu=vc(2,j1,1)
              alam=vc(3,j1,1)
              akap=alam+2.d0*amu
              ut(1)=u(j1,j2,1)+dt*(du1(3)/ds1+du2(5)/ds2)/rho0
              ut(2)=u(j1,j2,2)+dt*(du1(4)/ds1+du2(6)/ds2)/rho0
              ut(3)=u(j1,j2,3)+dt*(akap*du1(1)/ds1+alam*du2(2)/ds2)
              ut(4)=u(j1,j2,4)+dt*amu*(du1(2)/ds1+du2(1)/ds2)
              ut(5)=ut(4)
              ut(6)=u(j1,j2,6)+dt*(alam*du1(1)/ds1+akap*du2(2)/ds2)
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
      end if
c
c..loop over lines j2=n2a:n2b+1
      do j2=n2a,n2b+1
        j2m1=j2-1
c
c..set grid metrics and velocity
        call smmetrics2dv (nd1a,nd1b,md1a,md1b,nd2a,nd2b,j2,rx,det,
     *                     ndMatProp,matIndex,matValpc,matVal,
     *                     a1(1,1,md1a,2),aj(md1a,2),vc(1,md1a,2))
c
c..slope correction (top row of cells)
        if (icart.eq.0) then
c          if (.true. .or.iorder.eq.1.or.itz.ne.0.or.
          if (iorder.eq.1.or.itz.ne.0.or.
     *        ilimit.ne.0.or.iupwind.ne.0) then
            call smslope2dv (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
     *                       j2,ds1,ds2,dt,t,xy,a1(1,1,md1a,2),
     *                       aj(md1a,2),vc(1,md1a,2),mask(nd1a,j2),
     *                       u,w(1,md1a,1,2),f1)
          else
            do j1=n1a-1,n1b+1                      ! optimized for ilimit=iupwind=0 (curvilinear case)
              if (mask(j1,j2).ne.0) then
                do i=1,6
                  du1(i)=.25d0*(u(j1+1,j2,i)-u(j1-1,j2,i))
                  du2(i)=.25d0*(u(j1,j2+1,i)-u(j1,j2-1,i))
                end do
                rho0=vc(1,j1,2)
                amu=vc(2,j1,2)
                alam=vc(3,j1,2)
                akap=alam+2.d0*amu
                ut(1)=u(j1,j2,1)+dt*((rx(j1,j2,1,1)*du1(3)
     *                               +rx(j1,j2,1,2)*du1(5))/ds1
     *                              +(rx(j1,j2,2,1)*du2(3)
     *                               +rx(j1,j2,2,2)*du2(5))/ds2)/rho0
                ut(2)=u(j1,j2,2)+dt*((rx(j1,j2,1,1)*du1(4)
     *                               +rx(j1,j2,1,2)*du1(6))/ds1
     *                              +(rx(j1,j2,2,1)*du2(4)
     *                               +rx(j1,j2,2,2)*du2(6))/ds2)/rho0
                ut(3)=u(j1,j2,3)+dt*((rx(j1,j2,1,1)*akap*du1(1)
     *                               +rx(j1,j2,1,2)*alam*du1(2))/ds1
     *                              +(rx(j1,j2,2,1)*akap*du2(1)
     *                               +rx(j1,j2,2,2)*alam*du2(2))/ds2)
                ut(4)=u(j1,j2,4)+dt*amu*((rx(j1,j2,1,1)*du1(2)
     *                                   +rx(j1,j2,1,2)*du1(1))/ds1
     *                                  +(rx(j1,j2,2,1)*du2(2)
     *                                   +rx(j1,j2,2,2)*du2(1))/ds2)
                ut(5)=ut(4)
                ut(6)=u(j1,j2,6)+dt*((rx(j1,j2,1,1)*alam*du1(1)
     *                               +rx(j1,j2,1,2)*akap*du1(2))/ds1
     *                              +(rx(j1,j2,2,1)*alam*du2(1)
     *                               +rx(j1,j2,2,2)*akap*du2(2))/ds2)
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
        else
          if (iorder.eq.1.or.itz.ne.0.or.
     *        ilimit.ne.0.or.iupwind.ne.0) then
            call smslope2dvc (m,nd1a,nd1b,md1a,md1b,n1a,n1b,
     *                        nd2a,nd2b,j2,ds1,ds2,dt,t,xy,
     *                        vc(1,md1a,2),mask(nd1a,j2),u,
     *                        w(1,md1a,1,2),f1)
          else
            do j1=n1a-1,n1b+1                      ! optimized for ilimit=iupwind=0 (Cartesian case)
              if (mask(j1,j2).ne.0) then
                do i=1,6
                  du1(i)=.25d0*(u(j1+1,j2,i)-u(j1-1,j2,i))
                  du2(i)=.25d0*(u(j1,j2+1,i)-u(j1,j2-1,i))
                end do
                rho0=vc(1,j1,2)
                amu=vc(2,j1,2)
                alam=vc(3,j1,2)
                akap=alam+2.d0*amu
                ut(1)=u(j1,j2,1)+dt*(du1(3)/ds1+du2(5)/ds2)/rho0
                ut(2)=u(j1,j2,2)+dt*(du1(4)/ds1+du2(6)/ds2)/rho0
                ut(3)=u(j1,j2,3)+dt*(akap*du1(1)/ds1+alam*du2(2)/ds2)
                ut(4)=u(j1,j2,4)+dt*amu*(du1(2)/ds1+du2(1)/ds2)
                ut(5)=ut(4)
                ut(6)=u(j1,j2,6)+dt*(alam*du1(1)/ds1+akap*du2(2)/ds2)
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
        end if
c
c..compute s2 flux along j2-1/2, add it to up(j1,j2,.) and up(j1,j2-1,.)
        if (icart.eq.0) then
          do j1=n1a,n1b
            if (mask(j1,j2).ne.0.and.mask(j1,j2m1).ne.0) then
              call smflux2dv (a1(2,1,j1,1),a1(2,2,j1,1),
     *                        vc(1,j1,1),w(1,j1,4,1),
     *                        a1(2,1,j1,2),a1(2,2,j1,2),
     *                        vc(1,j1,2),w(1,j1,3,2),
     *                        fx,almax(2))

c              call smflux2d (m,aj(j1,1),a1(2,1,j1,1),a1(2,2,j1,1),
c     *                       w(1,j1,4,1),w(1,j1,3,2),fxtmp,speed0)
c              tol=1.d-13
c              iflag=0
c              do i=1,6
c                errf(i)=0.d0
c                do k=1,2
c                  errf(i)=max(dabs(fx(i,k)-fxtmp(i)),errf(i))
c                  if (dabs(fx(i,k)-fxtmp(i)).gt.tol) iflag=1
c                end do
c              end do
c              if (iflag.eq.1.and. .true. ) then
c                write(6,*)'s2 flux...'
c                do i=1,6
c                  write(6,345)i,fx(i,1),fx(i,2),fxtmp(i),errf(i)
c  345             format(1x,i2,3(1x,1pe15.8),1x,1pe10.3)
c                end do
c                pause
c              end if

c              if (iflag.eq.0) then
              do i=1,6
                up(j1,j2  ,i)=up(j1,j2  ,i)+fx(i,2)/ds2
                up(j1,j2m1,i)=up(j1,j2m1,i)-fx(i,1)/ds2
              end do
c              else
c              do i=1,6
c                up(j1,j2  ,i)=up(j1,j2  ,i)+fxtmp(i)/ds2
c                up(j1,j2m1,i)=up(j1,j2m1,i)-fxtmp(i)/ds2
c              end do
c              end if

            end if
          end do
        else
          do j1=n1a,n1b
            if (mask(j1,j2).ne.0.and.mask(j1,j2m1).ne.0) then
              call smflux2dv2 (vc(1,j1,1),w(1,j1,4,1),
     *                         vc(1,j1,2),w(1,j1,3,2),
     *                         fx,almax(2))
              do i=1,6
                up(j1,j2  ,i)=up(j1,j2  ,i)+fx(i,2)/ds2
                up(j1,j2m1,i)=up(j1,j2m1,i)-fx(i,1)/ds2
              end do
            end if
          end do
        end if
c
c..if j2.le.n2b, then compute fluxes in the s1 direction
        if (j2.le.n2b) then
c
c..reset metrics and w
          if (icart.eq.0) then
            do j1=n1a-1,n1b+1
              aj(j1,1)=aj(j1,2)
              vc(1,j1,1)=vc(1,j1,2)
              vc(2,j1,1)=vc(2,j1,2)
              vc(3,j1,1)=vc(3,j1,2)
              vc(4,j1,1)=vc(4,j1,2)
              vc(5,j1,1)=vc(5,j1,2)
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
              vc(1,j1,1)=vc(1,j1,2)
              vc(2,j1,1)=vc(2,j1,2)
              vc(3,j1,1)=vc(3,j1,2)
              vc(4,j1,1)=vc(4,j1,2)
              vc(5,j1,1)=vc(5,j1,2)
              do k=1,5
                do i=1,m
                  w(i,j1,k,1)=w(i,j1,k,2)
                end do
              end do
            end do
          end if
c
c..compute s1 flux along j2, add it to up(j1+1,j2,.) and up(j1,j2,.)
          if (icart.eq.0) then
            do j1=n1a-1,n1b
              j1p1=j1+1
              if (mask(j1,j2).ne.0.and.mask(j1p1,j2).ne.0) then
                call smflux2dv (a1(1,1,j1  ,1),a1(1,2,j1  ,1),
     *                          vc(1,j1  ,1),w(1,j1  ,2,1),
     *                          a1(1,1,j1p1,1),a1(1,2,j1p1,1),
     *                          vc(1,j1p1,1),w(1,j1p1,1,1),
     *                          fx,almax(1))

c                call smflux2d (m,aj(j1,1),a1(1,1,j1,1),a1(1,2,j1,1),
c     *                         w(1,j1,2,1),w(1,j1p1,1,1),fxtmp,speed0)
c                tol=1.d-13
c                iflag=0
c                do i=1,6
c                  errf(i)=0.d0
c                  do k=1,2
c                    errf(i)=max(dabs(fx(i,k)-fxtmp(i)),errf(i))
c                    if (dabs(fx(i,k)-fxtmp(i)).gt.tol) iflag=1
c                  end do
c                end do
c                if (iflag.eq.1.and. .true. ) then
c                  write(6,*)'s1 flux...'
c                  do i=1,6
c                    write(6,345)i,fx(i,1),fx(i,2),fxtmp(i),errf(i)
cc  345               format(1x,i2,3(1x,1pe15.8),1x,1pe10.3)
c                  end do
c                  pause
c                end if

c                if (iflag.eq.0) then
                do i=1,6
                  up(j1p1,j2,i)=up(j1p1,j2,i)+fx(i,2)/ds1
                  up(j1  ,j2,i)=up(j1  ,j2,i)-fx(i,1)/ds1
                end do
c                else
c                do i=1,6
c                  up(j1p1,j2,i)=up(j1p1,j2,i)+fxtmp(i)/ds1
c                  up(j1  ,j2,i)=up(j1  ,j2,i)-fxtmp(i)/ds1
c                end do
c                end if

              end if
            end do
          else
            do j1=n1a-1,n1b
              j1p1=j1+1
              if (mask(j1,j2).ne.0.and.mask(j1p1,j2).ne.0) then
                call smflux2dv1 (vc(1,j1  ,1),w(1,j1  ,2,1),
     *                           vc(1,j1p1,1),w(1,j1p1,1,1),
     *                           fx,almax(1))
                do i=1,6
                  up(j1p1,j2,i)=up(j1p1,j2,i)+fx(i,2)/ds1
                  up(j1  ,j2,i)=up(j1  ,j2,i)-fx(i,1)/ds1
                end do
              end if
            end do
          end if
c
c..add free stream correction to up (if a non-Cartesian grid)
c    ****** not needed *******  DWS
c
c..complete up
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
              call smGethtzv (m,xy(j1,j2,1),xy(j1,j2,2),t1,htz)
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
c
      return
      end
c
c++++++++++++++++++++++++++++
c
      subroutine smmetrics2dv (nd1a,nd1b,md1a,md1b,nd2a,nd2b,j2,
     *                         rx,det,ndMatProp,matIndex,matValpc,
     *                         matVal,a1,aj,vc)
c
      implicit real*8 (a-h,o-z)
      dimension rx(nd1a:nd1b,nd2a:nd2b,2,2),det(nd1a:nd1b,nd2a:nd2b),
     *          a1(2,2,md1a:md1b),aj(md1a:md1b),vc(5,md1a:md1b)
      integer matIndex(nd1a:nd1b,nd2a:nd2b)
      real*8 matValpc(ndMatProp,0:*)
      real*8 matVal(nd1a:nd1b,nd2a:nd2b,1:*)
      common / smgprm / method,iorder,ilimit,iupwind,icart,ifrc
      common / smgvar / amu,alam,rho0,mformat

      common / smgdat / amu1,alam1,rho01

c
c set grid array for variable coefficients
      if (mformat.eq.0) then
        do j1=md1a,md1b
          vc(1,j1)=rho0
          vc(2,j1)=amu
          vc(3,j1)=alam
        end do
      elseif (mformat.eq.1) then
        do j1=md1a,md1b
          vc(1,j1)=matValpc(1,matIndex(j1,j2))
          vc(2,j1)=matValpc(2,matIndex(j1,j2))
          vc(3,j1)=matValpc(3,matIndex(j1,j2))
        end do
      elseif (mformat.eq.2) then
        do j1=md1a,md1b
          vc(1,j1)=matVal(j1,j2,1)
          vc(2,j1)=matVal(j1,j2,2)
          vc(3,j1)=matVal(j1,j2,3)
        end do
      else
        write(6,*)'Error (smmetrics2dv) : mformat not supported'
        stop
      end if

      rho01=vc(1,md1a)
      amu1 =vc(2,md1a)
      alam1=vc(3,md1a)

c      write(6,*)'material props, j2=',j2
c      do j1=md1a,md1b
c        write(6,111)j1,(vc(i,j1),i=1,3)
c  111   format(1x,i3,3(1x,1pe10.3))
c      end do
c      pause

c
c wave speeds
      do j1=md1a,md1b
        vc(4,j1)=dsqrt((vc(3,j1)+2.d0*vc(2,j1))/vc(1,j1))
        vc(5,j1)=dsqrt(vc(2,j1)/vc(1,j1))
      end do

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
      subroutine smslope2dv (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
     *                       j2,ds1,ds2,dt,t,xy,a1,aj,vc,mask,u,w,f1)
c
c slope correction, variable coefficient case
c
      implicit real*8 (a-h,o-z)
      dimension xy(nd1a:nd1b,nd2a:nd2b,2),a1(2,2,md1a:md1b),
     *          aj(md1a:md1b),vc(5,md1a:md1b),mask(nd1a:nd1b),
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
      r1=.5d0*dt/ds1
      r2=.5d0*dt/ds2
c
      do j1=n1a-1,n1b+1
        if (mask(j1).ne.0) then
c
          if (itz.ne.0) then
            call smGethtzv (m,xy(j1,j2,1),xy(j1,j2,2),t,htz)
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
          call smeig2dv (a1(1,1,j1),a1(1,2,j1),vc(1,j1),al,el,er)
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
          call smeig2dv (a1(2,1,j1),a1(2,2,j1),vc(1,j1),al,el,er)
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
      subroutine smeig2dv (a1,a2,vc,al,el,er)
c
      implicit real*8 (a-h,o-z)
      dimension vc(5),al(6),el(6,6),er(6,6)
c
c..directions
      rad=dsqrt(a1**2+a2**2)
      an1=a1/rad
      an2=a2/rad
      an11=an1*an1
      an12=an1*an2
      an22=an2*an2
c
c..material parameters
      rho0=vc(1)
      amu=vc(2)
      alam=vc(3)
c
c..wave speeds
      c1=vc(4)
      c2=vc(5)
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
      subroutine smslope2dvc (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
     *                        j2,ds1,ds2,dt,t,xy,vc,mask,u,w,f1)
c
c slope correction   ***** optimized for Cartesian grids *****
c
      implicit real*8 (a-h,o-z)
      dimension xy(nd1a:nd1b,nd2a:nd2b,2),vc(5,nd1a:nd1b),
     *          mask(nd1a:nd1b),u(nd1a:nd1b,nd2a:nd2b,m),
     *          w(m,md1a:md1b,5),f1(nd1a:nd1b,nd2a:nd2b,2)
      dimension w1(6),w2(6),w3(6),w4(6),w5(6),htz(8),
     *          du1m(6),du1p(6),du2m(6),du2p(6)
      common / smgprm / method,iorder,ilimit,iupwind,icart,ifrc
      common / smgvar / amu,alam,rho0,mformat
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
        do j1=n1a-1,n1b+1
          if (mask(j1).ne.0) then
c
c..material parameters
            rho0=vc(1,j1)
            amu=vc(2,j1)
            alam=vc(3,j1)
c
c..wave speeds
            c1=vc(4,j1)
            c2=vc(5,j1)
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
c            el33=-1.d0
c            el34=-1.d0
c            el35= 1.d0
            el36= alam/(alam+2*amu)
            el43=-el36
c            el44=-1.d0
c            el45= 1.d0
c            el46= 1.d0
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
              call smGethtzv (m,xy(j1,j2,1),xy(j1,j2,2),t,htz)
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
      subroutine smslope2dvc0 (m,nd1a,nd1b,md1a,md1b,n1a,n1b,nd2a,nd2b,
     *                         j2,ds1,ds2,dt,t,xy,a1,aj,mask,u,w,f1)
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
      common / smgvar / amu,alam,rho0,mformat
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
            call smGethtzv (m,xy(j1,j2,1),xy(j1,j2,2),t,htz)
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
      subroutine smflux2dv1 (vcl,wl,vcr,wr,fx,speed)
c
c Godunov flux (method=0). Cartesian grid, 1-direction
c
      implicit real*8 (a-h,o-z)
      dimension vcl(5),wl(6),vcr(5),wr(6),w0(6),fx(6,2)
c
c normal and tangential velocity differences
      dvn=wr(1)-wl(1)
      dvt=wr(2)-wl(2)
c
c differences of stress in the normal and tangential directions
      dsn=wr(3)-wl(3)
      dst=wr(4)-wl(4)
c
c impedances
      zpl=vcl(1)*vcl(4)
      zsl=vcl(1)*vcl(5)
      zpr=vcr(1)*vcr(4)
      zsr=vcr(1)*vcr(5)
c
c wave strengths
      gam1=(dsn+zpr*dvn)/(vcl(4)*(zpl+zpr))
      gam2=(dst+zsr*dvt)/(vcl(5)*(zsl+zsr))
      gam5=(dst-zsl*dvt)/(vcr(5)*(zsl+zsr))
      gam6=(dsn-zpl*dvn)/(vcr(4)*(zpl+zpr))
c
c interface state (left)
      w0(1)=wl(1)+gam1*vcl(4)
      w0(2)=wl(2)+gam2*vcl(5)
      w0(3)=wl(3)+gam1*zpl*vcl(4)
      w0(4)=wl(4)+gam2*zsl*vcl(5)
      w0(5)=w0(4)
      w0(6)=wl(6)+gam1*(zpl*vcl(4)-2.d0*zsl*vcl(5))
c
c get left "flux"
      fx(1,1)=-w0(3)/vcl(1)
      fx(2,1)=-w0(4)/vcl(1)
      fx(3,1)=-(vcl(3)+2.d0*vcl(2))*w0(1)
      fx(4,1)=-vcl(2)*w0(2)
      fx(5,1)= fx(4,1)
      fx(6,1)=-vcl(3)*w0(1)
c
c interface state (right)
      w0(1)=wr(1)+gam6*vcr(4)
      w0(2)=wr(2)+gam5*vcr(5)
      w0(3)=wr(3)-gam6*zpr*vcr(4)
      w0(4)=wr(4)-gam5*zsr*vcr(5)
      w0(5)=w0(4)
      w0(6)=wr(6)-gam6*(zpr*vcr(4)-2.d0*zsr*vcr(5))
c
c get right "flux"
      fx(1,2)=-w0(3)/vcr(1)
      fx(2,2)=-w0(4)/vcr(1)
      fx(3,2)=-(vcr(3)+2.d0*vcr(2))*w0(1)
      fx(4,2)=-vcr(2)*w0(2)
      fx(5,2)= fx(4,2)
      fx(6,2)=-vcr(3)*w0(1)
c
c fastest speed
      speed=max(vcl(4),vcr(4),speed)
c
      return
      end
c
c++++++++++++++++++++
c
      subroutine smflux2dv2 (vcl,wl,vcr,wr,fx,speed)
c
c Godunov flux (method=0). Cartesian grid, 2-direction
c
      implicit real*8 (a-h,o-z)
      dimension vcl(5),wl(6),vcr(5),wr(6),w0(6),fx(6,2)
c
c normal and tangential velocity differences
      dvn=wr(2)-wl(2)
      dvt=wl(1)-wr(1)
c
c differences of stress in the normal and tangential directions
      dsn=wr(6)-wl(6)
      dst=wl(5)-wr(5)
c
c impedances
      zpl=vcl(1)*vcl(4)
      zsl=vcl(1)*vcl(5)
      zpr=vcr(1)*vcr(4)
      zsr=vcr(1)*vcr(5)
c
c wave strengths
      gam1=(dsn+zpr*dvn)/(vcl(4)*(zpl+zpr))
      gam2=(dst+zsr*dvt)/(vcl(5)*(zsl+zsr))
      gam5=(dst-zsl*dvt)/(vcr(5)*(zsl+zsr))
      gam6=(dsn-zpl*dvn)/(vcr(4)*(zpl+zpr))
c
c interface state (left)
      w0(1)=wl(1)-gam2*vcl(5)
      w0(2)=wl(2)+gam1*vcl(4)
      w0(3)=wl(3)+gam1*(zpl*vcl(4)-2.d0*zsl*vcl(5))
      w0(4)=wl(4)-gam2*zsl*vcl(5)
      w0(5)=w0(4)
      w0(6)=wl(6)+gam1*zpl*vcl(4)
c
c get left "flux"
      fx(1,1)=-w0(5)/vcl(1)
      fx(2,1)=-w0(6)/vcl(1)
      fx(3,1)=-vcl(3)*w0(2)
      fx(4,1)=-vcl(2)*w0(1)
      fx(5,1)= fx(4,1)
      fx(6,1)=-(vcl(3)+2.d0*vcl(2))*w0(2)
c
c interface state (right)
      w0(1)=wr(1)-gam5*vcr(5)
      w0(2)=wr(2)+gam6*vcr(4)
      w0(3)=wr(3)-gam6*(zpr*vcr(4)-2.d0*zsr*vcr(5))
      w0(4)=wr(4)+gam5*zsr*vcr(5)
      w0(5)=w0(4)
      w0(6)=wr(6)-gam6*zpr*vcr(4)
c
c get right "flux"
      fx(1,2)=-w0(5)/vcr(1)
      fx(2,2)=-w0(6)/vcr(1)
      fx(3,2)=-vcr(3)*w0(2)
      fx(4,2)=-vcr(2)*w0(1)
      fx(5,2)= fx(4,2)
      fx(6,2)=-(vcr(3)+2.d0*vcr(2))*w0(2)
c
c fastest speed
      speed=max(vcl(4),vcr(4),speed)
c
      return
      end
c
c++++++++++++++++++++
c
      subroutine smflux2dv (a1l,a2l,vcl,wl,a1r,a2r,vcr,wr,fx,speed)
c
c Godunov flux (method=0)
c
      implicit real*8 (a-h,o-z)
      dimension vcl(5),wl(6),vcr(5),wr(6),w0(6),fx(6,2)
c
c normalize metrics of the mapping
      a1=.5d0*(a1l+a1r)
      a2=.5d0*(a2l+a2r)
      rad=dsqrt(a1**2+a2**2)
      an1=a1/rad
      an2=a2/rad
c
c normal and tangential velocity differences
      dvn=an1*(wr(1)-wl(1))+an2*(wr(2)-wl(2))
      dvt=an1*(wr(2)-wl(2))-an2*(wr(1)-wl(1))
c
c differences of stress in the 1 and 2 directions on a face with n=(an1,an2)
      ds1=an1*(wr(3)-wl(3))+an2*(wr(5)-wl(5))
      ds2=an1*(wr(4)-wl(4))+an2*(wr(6)-wl(6))
c
c differences of stress in the normal and tangential directions
      dsn=an1*ds1+an2*ds2
      dst=an1*ds2-an2*ds1
c
c impedances
      zpl=vcl(1)*vcl(4)
      zsl=vcl(1)*vcl(5)
      zpr=vcr(1)*vcr(4)
      zsr=vcr(1)*vcr(5)
c
c wave strengths
      gam1=(dsn+zpr*dvn)/(vcl(4)*(zpl+zpr))
      gam2=(dst+zsr*dvt)/(vcl(5)*(zsl+zsr))
      gam5=(dst-zsl*dvt)/(vcr(5)*(zsl+zsr))
      gam6=(dsn-zpl*dvn)/(vcr(4)*(zpl+zpr))
c
c interface state (left)
      w0(1)=wl(1)+gam1*(an1*vcl(4))
     *           -gam2*(an2*vcl(5))
      w0(2)=wl(2)+gam1*(an2*vcl(4))
     *           +gam2*(an1*vcl(5))
      w0(3)=wl(3)+gam1*(zpl*vcl(4)-2.d0*zsl*vcl(5)*an2**2)
     *           -gam2*(2.d0*an1*an2*zsl*vcl(5))
      w0(4)=wl(4)+gam1*(2.d0*an1*an2*zsl*vcl(5))
     *           +gam2*(zsl*vcl(5)*(an1**2-an2**2))
      w0(5)=w0(4)
      w0(6)=wl(6)+gam1*(zpl*vcl(4)-2.d0*zsl*vcl(5)*an1**2)
     *           +gam2*(2.d0*an1*an2*zsl*vcl(5))

c      do i=1,6
c        w0(i)=.5d0*(wl(i)+wr(i))
c      end do

c
c get left "flux"
      call smflxv (a1l,a2l,vcl,w0,fx(1,1))
c
c interface state (right)
      w0(1)=wr(1)-gam5*(an2*vcr(5))
     *           +gam6*(an1*vcr(4))
      w0(2)=wr(2)+gam5*(an1*vcr(5))
     *           +gam6*(an2*vcr(4))
      w0(3)=wr(3)+gam5*(2.d0*an1*an2*zsr*vcr(5))
     *           -gam6*(zpr*vcr(4)-2.d0*zsr*vcr(5)*an2**2)
      w0(4)=wr(4)-gam5*(zsr*vcr(5)*(an1**2-an2**2))
     *           -gam6*(2.d0*an1*an2*zsr*vcr(5))
      w0(5)=w0(4)
      w0(6)=wr(6)-gam5*(2.d0*an1*an2*zsr*vcr(5))
     *           -gam6*(zpr*vcr(4)-2.d0*zsr*vcr(5)*an1**2)

c      do i=1,6
c        w0(i)=.5d0*(wl(i)+wr(i))
c      end do

c
c get right "flux"
      call smflxv (a1r,a2r,vcr,w0,fx(1,2))
c
c fastest speed
      speed=max(rad*vcl(4),rad*vcr(4),speed)
c
      return
      end
c
c++++++++++++++++++++
c
      subroutine smflxv (a1,a2,vc,w,fx)
c
c flux
c
      implicit real*8 (a-h,o-z)
      dimension vc(5),w(6),fx(6)
c
      fx(1)=-(a1*w(3)+a2*w(5))/vc(1)
      fx(2)=-(a1*w(4)+a2*w(6))/vc(1)
      fx(3)=-(a1*(vc(3)+2.d0*vc(2))*w(1)+a2*vc(3)*w(2))
      fx(4)=-vc(2)*(a1*w(2)+a2*w(1))
      fx(5)= fx(4)
      fx(6)=-(a1*vc(3)*w(1)+a2*(vc(3)+2.d0*vc(2))*w(2))
c
      return
      end
c
c++++++++++++++++++++
c
      subroutine smGethtzv (m,x,y,t,htz)
c
c compute tz forcing function
c
      implicit real*8 (a-h,o-z)
      dimension htz(m),ut(8),ux(6),uy(6),u0(2)
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
      call ogDeriv (eptz,0,0,0,0,x,y,0.d0,t,8 ,rho0)
      call ogDeriv (eptz,0,0,0,0,x,y,0.d0,t,9 ,amu )
      call ogDeriv (eptz,0,0,0,0,x,y,0.d0,t,10,alam)
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
c      do i=1,6
c        err=max(dabs(htz(i)),err)
c      end do
c      if (err.gt.tol) then
c        write(6,*)'Warning (smGethtz) : sanity check failed'
c      end if
c
      return
      end
c
c++++++++++++++++++++
c
      subroutine stressRelax2dv (m,nd1a,nd1b,nd2a,nd2b,n1a,n1b,n2a,n2b,
     *                           ds1,ds2,dt,t,xy,rx,u,up,mask,
     *                           ndMatProp,matIndex,matValpc,matVal,
     *                           iRelax,relaxAlpha,relaxDelta)
c
      implicit real*8 (a-h,o-z)
      dimension xy(nd1a:nd1b,nd2a:nd2b,2),rx(nd1a:nd1b,nd2a:nd2b,2,2),
     *          u(nd1a:nd1b,nd2a:nd2b,m),
     *          up(nd1a:nd1b,nd2a:nd2b,m),mask(nd1a:nd1b,nd2a:nd2b)
c
c arrays for variable material properties --
      integer matIndex(nd1a:nd1b,nd2a:nd2b)
      real*8 matValpc(ndMatProp,0:*)
      real*8 matVal(nd1a:nd1b,nd2a:nd2b,1:*)
c
      common / smgprm / method,iorder,ilimit,iupwind,icart,ifrc
      common / smgvar / amu,alam,rho0,mformat
      common / tzflow / eptz,itz
c
      beta = relaxAlpha+relaxDelta/dt
      akappa = alam+2.d0*amu
c
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
              u1x = (-u(j1+2,j2,7)+8.d0*(u(j1+1,j2,7)
     *               -u(j1-1,j2,7))+u(j1-2,j2,7))/(12.d0*ds1)
              u2x = (-u(j1+2,j2,8)+8.d0*(u(j1+1,j2,8)
     *               -u(j1-1,j2,8))+u(j1-2,j2,8))/(12.d0*ds1)
              u1y = (-u(j1,j2+2,7)+8.d0*(u(j1,j2+1,7)
     *               -u(j1,j2-1,7))+u(j1,j2-2,7))/(12.d0*ds2)
              u2y = (-u(j1,j2+2,8)+8.d0*(u(j1,j2+1,8)
     *               -u(j1,j2-1,8))+u(j1,j2-2,8))/(12.d0*ds2)
            end if

            if (mformat.eq.1) then
              amu=matValpc(2,matIndex(j1,j2))
              alam=matValpc(3,matIndex(j1,j2))
              akappa=alam+2.d0*amu
            elseif (mformat.eq.2) then
              amu=matVal(j1,j2,2)
              alam=matVal(j1,j2,3)
              akappa=alam+2.d0*amu
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
              u1r = (-u(j1+2,j2,7)+8.d0*(u(j1+1,j2,7)
     *               -u(j1-1,j2,7))+u(j1-2,j2,7))/(12.d0*ds1)
              u2r = (-u(j1+2,j2,8)+8.d0*(u(j1+1,j2,8)
     *               -u(j1-1,j2,8))+u(j1-2,j2,8))/(12.d0*ds1)
              u1s = (-u(j1,j2+2,7)+8.d0*(u(j1,j2+1,7)
     *               -u(j1,j2-1,7))+u(j1,j2-2,7))/(12.d0*ds2)
              u2s = (-u(j1,j2+2,8)+8.d0*(u(j1,j2+1,8)
     *               -u(j1,j2-1,8))+u(j1,j2-2,8))/(12.d0*ds2)
            end if

            u1x = u1r*rx(j1,j2,1,1)+u1s*rx(j1,j2,2,1)
            u2x = u2r*rx(j1,j2,1,1)+u2s*rx(j1,j2,2,1)
            u1y = u1r*rx(j1,j2,1,2)+u1s*rx(j1,j2,2,2)
            u2y = u2r*rx(j1,j2,1,2)+u2s*rx(j1,j2,2,2)

            if (mformat.eq.1) then
              amu=matValpc(2,matIndex(j1,j2))
              alam=matValpc(3,matIndex(j1,j2))
              akappa=alam+2.d0*amu
            elseif (mformat.eq.2) then
              amu=matVal(j1,j2,2)
              alam=matVal(j1,j2,3)
              akappa=alam+2.d0*amu
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
      end if

c add twilight zone contribution, if necessary
      if( itz.ne.0 ) then
        iu1c = 6
        iu2c = 7
        is11c = 2
        is12c = 3
        is21c = 4
        is22c = 5
        imuc = 9
        ilamc = 10
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

            if (mformat.ne.0) then
              call ogDeriv( eptz,0,0,0,0,x,y,0.d0,t,imuc,amu )
              call ogDeriv( eptz,0,0,0,0,x,y,0.d0,t,ilamc,alam )
              akappa=alam+2.d0*amu
            end if

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
