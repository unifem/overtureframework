      subroutine smg3d( m,nd1a,nd1b,n1a,n1b,
     *                    nd2a,nd2b,n2a,n2b,
     *                    nd3a,nd3b,n3a,n3b,
     *                    ds1,ds2,ds3,dt,t,xy,rx,det,
     *                    u,up,mask,ad, ! *wdh* 091113
     *                    nrparam,rparam,niparam,iparam,nrwrk,rwrk,
     *                    niwrk,iwrk,idebug,ier )
c
      implicit none
c.. ingoing declarations
      integer nd1a,nd1b,n1a,n1b,m
      integer nd2a,nd2b,n2a,n2b
      integer nd3a,nd3b,n3a,n3b
      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer nrparam,niparam,nrwrk,niwrk
      integer iparam(niparam)
      integer iwrk(niwrk)
      integer idebug,ier
      real ds1,ds2,ds3,dt,t
      real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,3)
      real rx(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,3,3)
      real det(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,m)
      real up(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,m)
      real rparam(nrparam)
      real rwrk(nrwrk)
c
c.. locals
      integer i1,i2,i3,k
      integer md1a,md1b,md2a,md2b,md3a,md3b
      integer ngrid,lw,la1,laj,nreq
      real tol,almax(3),const,diff
      real ad(15),adk,admax,tiny
      integer iRelax
      include 'smupcommons.h'
c
      idebug=iparam(10) ! *wdh* 090905
c
c.. set ad=0 for now
!*wdh*      do k = 1,15
!*wdh*        ad(k) = 0.
!*wdh*        ! ad(k) = 1. ! *wdh* turn on for testing 
!*wdh*      end do
c
      ! write(*,*) 'smg3d: ad=',(ad(i1),i1=1,15)

      if( idebug.gt.0 )then
        write(6,*)'*** Entering smg3d ***'
      end if
c
c.. declarations for these are in smupcommons
      mu     = rparam(3)
      lam    = rparam(4)
      rho0   = rparam(5)
      eptz   = rparam(6)
      iorder = iparam(1)
      icart  = iparam(2)
      itz    = iparam(3)
      ilimit = iparam(5)
      iRelax = iparam(9)
c
c      iorder = 2
c      write(6,*)ilimit
c      pause
c
c.. set array dimensions for parallel
      md1a = max( nd1a,n1a-2 )
      md1b = min( nd1b,n1b+2 )
      md2a = max( nd2a,n2a-2 )
      md2b = min( nd2b,n2b+2 )
      md3a = max( nd3a,n3a-2 )
      md3b = min( nd3b,n3b+2 )
c
c.. sanity check
      if( m.ne.15 ) then
        write(6,*)'Error (smg3d) : m=15 is assumed'
        stop
      end if
c
c.. split up real workspace
      ngrid  = (md2b-md2a+1)*(md1b-md1a+1)
      lw   = 1
      la1  = lw+m*7*ngrid*2
      laj  = la1+9*2*ngrid*2
      nreq = laj+ngrid*2-1
      if( nreq.gt.nrwrk ) then
        ier = 4
        return
      end if
c
c.. filter out underflow
      tol = 1.0e-30
      do k = 1,m
c        const=u(n1a,n2a,n3a,k)
        do i3 = md3a,md3b
        do i2 = md2a,md2b
        do i1 = md1a,md1b
          if( abs( u(i1,i2,i3,k) ).lt.tol ) u(i1,i2,i3,k) = 0.0
c          diff=max(abs(u(i1,i2,i3,k)-const),diff)
        end do
        end do
        end do
c        write(6,*)k,diff
      end do
c      pause
c
c.. make call to compute up
      call smup3d( m,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *               n1a, n1b, n2a, n2b, n3a, n3b,
     *               md1a,md1b,md2a,md2b,md3a,md3b,
     *               ds1,ds2,ds3,dt,t,xy,rx,det,u,up,
     *               mask,almax,rwrk(lw),rwrk(la1),
     *               rwrk(laj),ier )
c
c
c add relaxation term to ensure compatibility of stress and position
c   jwb -- 13 Aug 2010
      if( iRelax.ne.0 ) then
        call stressRelax3d( m,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *                        n1a, n1b, n2a, n2b, n3a, n3b, 
     *                        ds1,ds2,ds3,dt,t,xy,rx,u,up,mask,
     *                        iparam,rparam )
      end if
c
c.. check free streaming
c       do k = 1,m
c         diff = 0.0
c         do i3 = n3a,n3b
c         do i2 = n2a,n2b
c         do i1 = n1a,n1b
c           if( mask(i1,i2,i3).ne.0 )then
c             diff = max(abs(up(i1,i2,i3,k)),diff)
c           end if
c         end do
c         end do
c         end do
c         write(6,*)'k,diff=',k,diff
c       end do
c       pause
c
c.. add an artificial dissipation
       tiny = 1.e-15
       admax = 0.
       do k = 1,m
         adk = ad(k)
         if( adk.gt.tiny )then
           admax = max(adk,admax)
           do i3 = n3a,n3b
           do i2 = n2a,n2b
           do i1 = n1a,n1b
             if( mask(i1,i2,i3).ne.0 )then
               up(i1,i2,i3,k) = up(i1,i2,i3,k)+adk*(-6.*u(i1,i2,i3,k)
     *                               +u(i1+1,i2,i3,k)+u(i1-1,i2,i3,k)
     *                               +u(i1,i2+1,i3,k)+u(i1,i2-1,i3,k)
     *                               +u(i1,i2,i3+1,k)+u(i1,i2,i3-1,k))
             end if
           end do
           end do
           end do
         end if
       end do
c
c.. compute real and imaginary parts of time stepping eigenvalues
      rparam(1) = 6.0*admax
      rparam(2) = almax(1)/ds1+almax(2)/ds2+almax(3)/ds3
      rparam(3)=0. ! for 1/dt dissipation
c
      if( idebug.gt.0 )then
        write(6,*)'*** Leaving smg3d ***'
      end if      
c
      return
      end
c
c+++++++++++++++++++++++
c
      subroutine smup3d( m,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *                     n1a, n1b, n2a, n2b, n3a, n3b,
     *                     md1a,md1b,md2a,md2b,md3a,md3b,
     *                     ds1,ds2,ds3,dt,t,xy,rx,det,u,up,
     *                     mask,almax,w,a1,aj,ier )
c
      implicit none
c.. ingoing declarations
      integer m,ier
      integer nd1a,nd1b,nd2a,nd2b,nd3a,nd3b
      integer n1a, n1b, n2a, n2b, n3a, n3b
      integer md1a,md1b,md2a,md2b,md3a,md3b
      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real ds1,ds2,ds3,dt,t
      real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,3)
      real rx(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,3,3)
      real det(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,m)
      real up(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,m)
      real almax(3)
      real w(m,md1a:md1b,md2a:md2b,7,2)
      real a1(3,3,md1a:md1b,md2a:md2b,2)
      real aj(md1a:md1b,md2a:md2b,2)
      include 'smupcommons.h'
c
c.. local declarations
      integer j,k,i1,i2,i3,i3m1
      integer i1p1,i2p1
      integer s1m,s1p,s2m,s2p,s3m,s3p,scent
      integer lay,laym1
      real met0(3,3),aj0
      real fx(12),htz(15),t1
      real d1p,d1m,d2p,d2m,d3p,d3m,da
c      real fx0(12),errf(12,3)
c      real w0(15,-2:7,-2:7,7,2),errw(12,5)
      real ut(15),ux(12),uy(12),uz(12),u0(3),kap,x,y,z
      real ul(12),ur(12),c1,c2,cf1,cf2,cf3,cf4,df1,df2,df3
      data s1m,s1p,s2m,s2p,s3m,s3p,scent / 1,2,3,4,5,6,7 /
c
c.. initialize max eigenvalues
      almax(1) = 0.0
      almax(2) = 0.0
      almax(3) = 0.0
c
c.. initialize up
      do k = 1,12
        do i3 = n3a,n3b
        do i2 = n2a,n2b
        do i1 = n1a,n1b
          up(i1,i2,i3,k) = 0.0
        end do
        end do
        end do
      end do
c
c.. slope check
c      do k = 1,12
c        errw(k,1) = 0.
c        errw(k,2) = 0.
c        errw(k,3) = 0.
c        errw(k,4) = 0.
c        errw(k,5) = 0.
c      end do
c
c.. flux check
c      do k = 1,12
c        errf(k,1) = 0.
c        errf(k,2) = 0.
c        errf(k,3) = 0.
c      end do
c
c..compute coeff.s for Godunov flux solver (if mesh is Cartesian)
      if( icart.ne.0 )then
        c1 = sqrt((lam+2.*mu)/rho0)
        c2 = sqrt(mu/rho0)
        almax(1) = c1
        almax(2) = c1
        almax(3) = c1
        cf1 = .5/rho0
        cf2 = .5*(lam+2.*mu)
        cf3 = .5*lam
        cf4 = .5*mu
        df1 = .5*c1
        df2 = .5*c2
        df3 = .5*c1*lam/(lam+2.*mu)
      end if
c
      i3 = n3a-1
      lay = 1 
c.. the "lay" variable indicates which layer we are on within w, a1, and aj
c     so that we don't have to do the costly copies to update the layer.
c
c.. set grid metrics and grid velocities
      call smupmetrics3d( nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *                    md1a,md1b,md2a,md2b,md3a,md3b,
     *                    i3,rx,det,a1(1,1,md1a,md2a,lay),
     *                    aj(md1a,md2a,lay) )
      if( icart.eq.1 ) then
        ! if we are doing Cartesian then fill in all layers at once so we
        ! don't have to do it later
        call smupmetrics3d( nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *                      md1a,md1b,md2a,md2b,md3a,md3b,
     *                      i3,rx,det,a1(1,1,md1a,md2a,2),
     *                      aj(md1a,md2a,2) )
      end if
c
c.. set first layer of "slope corrected" values
c      if (.true.) then
      if( iorder.eq.1.or.ilimit.ne.0.or.itz.ne.0 )then
c        write(6,*)'here i am (1)'
        call smupslope3d( m,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *                      md1a,md1b,md2a,md2b,md3a,md3b,
     *                      n1a, n1b, n2a, n2b, n3a, n3b,
     *                      i3,ds1,ds2,ds3,dt,t,xy,
     *                      a1(1,1,md1a,md2a,lay),
     *                      aj(nd1a,md2a,lay), 
     *                      mask(nd1a,nd2a,i3),u,
     *                      w(1,md1a,md2a,1,lay) )
      else
        call smupslope3dO( m,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *                       md1a,md1b,md2a,md2b,md3a,md3b,
     *                       n1a, n1b, n2a, n2b, n3a, n3b,
     *                       i3,ds1,ds2,ds3,dt,rx,det,
     *                       mask(nd1a,nd2a,i3),u,
     *                       w(1,md1a,md2a,1,lay) )
c     *                       w0(1,md1a,md2a,1,lay) )
c        do i2 = n2a-1,n2b+1
c        do i1 = n1a-1,n1b+1
c          if( mask(i1,i2,i3).ne.0 ) then
c            write(1,333)i1,i2,i3
c  333       format(3(1x,i2))
c            do k = 1,12
c              errw(k,1)=max(abs(w(k,i1,i2,1,lay)-w0(k,i1,i2,1,lay)),
c     *                      errw(k,1))
c              errw(k,2)=max(abs(w(k,i1,i2,2,lay)-w0(k,i1,i2,2,lay)),
c     *                      errw(k,2))
c              errw(k,3)=max(abs(w(k,i1,i2,3,lay)-w0(k,i1,i2,3,lay)),
c     *                      errw(k,3))
c              errw(k,4)=max(abs(w(k,i1,i2,4,lay)-w0(k,i1,i2,4,lay)),
c     *                      errw(k,4))
c              errw(k,5)=max(abs(w(k,i1,i2,5,lay)-w0(k,i1,i2,5,lay)),
c     *                      errw(k,5))
c              write(1,444)k,w(k,i1,i2,1,lay),w0(k,i1,i2,1,lay),
c     *                      w(k,i1,i2,2,lay),w0(k,i1,i2,2,lay),
c     *                      w(k,i1,i2,3,lay),w0(k,i1,i2,3,lay),
c     *                      w(k,i1,i2,4,lay),w0(k,i1,i2,4,lay),
c     *                      w(k,i1,i2,5,lay),w0(k,i1,i2,5,lay)
c  444         format(1x,i2,10(1x,1pe9.2))
c            end do
c          end if
c        end do
c        end do
      end if
c
c.. loop over planes i3=n3a,n3b+1
      do i3 = n3a,n3b+1
        i3m1 = i3-1
c
c.. update layer pointer
        laym1 = lay
        lay = lay+1
        if( lay.gt.2 ) lay = 1
c
c.. set metrics if needed
        if( icart.ne.1 ) then
          call smupmetrics3d( nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *                        md1a,md1b,md2a,md2b,md3a,md3b,
     *                        i3,rx,det,a1(1,1,md1a,md2a,lay),
     *                        aj(md1a,md2a,lay) )
        end if
c
c.. slope correction for top layer of cells
c        if (.true.) then
        if( iorder.eq.1.or.ilimit.ne.0.or.itz.ne.0 )then
c        write(6,*)'here i am (2)'
          call smupslope3d( m,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *                        md1a,md1b,md2a,md2b,md3a,md3b,
     *                        n1a, n1b, n2a, n2b, n3a, n3b,
     *                        i3,ds1,ds2,ds3,dt,t,xy,
     *                        a1(1,1,md1a,md2a,lay),
     *                        aj(nd1a,md2a,lay), 
     *                        mask(nd1a,nd2a,i3),u,
     *                        w(1,md1a,md2a,1,lay) )
        else
          call smupslope3dO( m,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *                       md1a,md1b,md2a,md2b,md3a,md3b,
     *                       n1a, n1b, n2a, n2b, n3a, n3b,
     *                       i3,ds1,ds2,ds3,dt,rx,det,
     *                       mask(nd1a,nd2a,i3),u,
     *                       w(1,md1a,md2a,1,lay) )
c     *                       w0(1,md1a,md2a,1,lay) )
c          do i2 = n2a-1,n2b+1
c          do i1 = n1a-1,n1b+1
c            if( mask(i1,i2,i3).ne.0 ) then
c              write(1,333)i1,i2,i3
c              do k = 1,12
c                errw(k,1)=max(abs(w(k,i1,i2,1,lay)-w0(k,i1,i2,1,lay)),
c     *                        errw(k,1))
c                errw(k,2)=max(abs(w(k,i1,i2,2,lay)-w0(k,i1,i2,2,lay)),
c     *                        errw(k,2))
c                errw(k,3)=max(abs(w(k,i1,i2,3,lay)-w0(k,i1,i2,3,lay)),
c     *                        errw(k,3))
c                errw(k,4)=max(abs(w(k,i1,i2,4,lay)-w0(k,i1,i2,4,lay)),
c     *                        errw(k,4))
c                errw(k,5)=max(abs(w(k,i1,i2,5,lay)-w0(k,i1,i2,5,lay)),
c     *                        errw(k,5))
c                write(1,444)k,w(k,i1,i2,1,lay),w0(k,i1,i2,1,lay),
c     *                        w(k,i1,i2,2,lay),w0(k,i1,i2,2,lay),
c     *                        w(k,i1,i2,3,lay),w0(k,i1,i2,3,lay),
c     *                        w(k,i1,i2,4,lay),w0(k,i1,i2,4,lay),
c     *                        w(k,i1,i2,5,lay),w0(k,i1,i2,5,lay)
c              end do
c            end if
c          end do
c          end do
        end if
c
c..compute s3 flux and add it to up
c        if (.true.) then
        if( icart.eq.0 )then
c        write(6,*)'here i am (3)'
c
c  non-Cartesian case
          do i2 = n2a,n2b
          do i1 = n1a,n1b
            if( mask(i1,i2,i3).ne.0 .and. mask(i1,i2,i3m1).ne.0 ) then
              ! average metrics
              aj0 = 0.5*(aj(i1,i2,lay)+aj(i1,i2,laym1))
              met0(1,1) = 0.5*(a1(3,1,i1,i2,lay)+a1(3,1,i1,i2,laym1))
              met0(1,2) = 0.5*(a1(3,2,i1,i2,lay)+a1(3,2,i1,i2,laym1))
              met0(1,3) = 0.5*(a1(3,3,i1,i2,lay)+a1(3,3,i1,i2,laym1))
              met0(2,1) = 0.5*(a1(1,1,i1,i2,lay)+a1(1,1,i1,i2,laym1))
              met0(2,2) = 0.5*(a1(1,2,i1,i2,lay)+a1(1,2,i1,i2,laym1))
              met0(2,3) = 0.5*(a1(1,3,i1,i2,lay)+a1(1,3,i1,i2,laym1))
              met0(3,1) = 0.5*(a1(2,1,i1,i2,lay)+a1(2,1,i1,i2,laym1))
              met0(3,2) = 0.5*(a1(2,2,i1,i2,lay)+a1(2,2,i1,i2,laym1))
              met0(3,3) = 0.5*(a1(2,3,i1,i2,lay)+a1(2,3,i1,i2,laym1))

              ! solve Riemann problem
              call smupflux3d( m,aj0,met0,w(1,i1,i2,s3p,laym1),
     *                         w(1,i1,i2,s3m,lay),fx,almax(3) )
              do k = 1,12
                up(i1,i2,i3,k) = up(i1,i2,i3,k)+
     *             fx(k)/(ds3*aj(i1,i2,lay))
                up(i1,i2,i3m1,k) = up(i1,i2,i3m1,k)-
     *             fx(k)/(ds3*aj(i1,i2,laym1))
              end do
            end if
          end do
          end do
c
        else
c
c  Cartesian case
          do i2 = n2a,n2b
          do i1 = n1a,n1b
            if( mask(i1,i2,i3).ne.0 .and. mask(i1,i2,i3m1).ne.0 ) then
c              aj0 = 0.5*(aj(i1,i2,lay)+aj(i1,i2,laym1))
c              met0(1,1) = 0.5*(a1(3,1,i1,i2,lay)+a1(3,1,i1,i2,laym1))
c              met0(1,2) = 0.5*(a1(3,2,i1,i2,lay)+a1(3,2,i1,i2,laym1))
c              met0(1,3) = 0.5*(a1(3,3,i1,i2,lay)+a1(3,3,i1,i2,laym1))
c              met0(2,1) = 0.5*(a1(1,1,i1,i2,lay)+a1(1,1,i1,i2,laym1))
c              met0(2,2) = 0.5*(a1(1,2,i1,i2,lay)+a1(1,2,i1,i2,laym1))
c              met0(2,3) = 0.5*(a1(1,3,i1,i2,lay)+a1(1,3,i1,i2,laym1))
c              met0(3,1) = 0.5*(a1(2,1,i1,i2,lay)+a1(2,1,i1,i2,laym1))
c              met0(3,2) = 0.5*(a1(2,2,i1,i2,lay)+a1(2,2,i1,i2,laym1))
c              met0(3,3) = 0.5*(a1(2,3,i1,i2,lay)+a1(2,3,i1,i2,laym1))
              do k=1,12
                ul(k) = w(k,i1,i2,s3p,laym1)
                ur(k) = w(k,i1,i2,s3m,lay  )
              end do
              fx( 1) = (-cf1*(ul(10)+ur(10))-df2*(ur(1)-ul(1)))/ds3  ! v1
              fx( 2) = (-cf1*(ul(11)+ur(11))-df2*(ur(2)-ul(2)))/ds3  ! v2
              fx( 3) = (-cf1*(ul(12)+ur(12))-df1*(ur(3)-ul(3)))/ds3  ! v3
              fx(10) = (-cf4*(ul(1)+ur(1))-df2*(ur(10)-ul(10)))/ds3  ! s31
              fx(11) = (-cf4*(ul(2)+ur(2))-df2*(ur(11)-ul(11)))/ds3  ! s32
              fx(12) = (-cf2*(ul(3)+ur(3))-df1*(ur(12)-ul(12)))/ds3  ! s33
              fx( 4) = (-cf3*(ul(3)+ur(3))-df3*(ur(12)-ul(12)))/ds3  ! s11
              fx( 5) = 0.                                            ! s12
              fx( 6) = fx(10)                                        ! s13
              fx( 7) = 0.                                            ! s21
              fx( 8) = fx(4)                                         ! s22
              fx( 9) = fx(11)                                        ! s23
c              call smupflux3d( m,aj0,met0,w(1,i1,i2,s3p,laym1),
c     *                         w(1,i1,i2,s3m,lay),fx0,almax(3) )
              do k = 1,12
                up(i1,i2,i3  ,k) = up(i1,i2,i3  ,k)+fx(k)
                up(i1,i2,i3m1,k) = up(i1,i2,i3m1,k)-fx(k)
c                up(i1,i2,i3  ,k) = up(i1,i2,i3,k)+
c     *             fx0(k)/(ds3*aj(i1,i2,lay  ))
c                up(i1,i2,i3m1,k) = up(i1,i2,i3m1,k)-
c     *             fx0(k)/(ds3*aj(i1,i2,laym1))
c                errf(k,3) = max(abs(fx(k)-fx0(k)/ds3),errf(k,3))
              end do
            end if
          end do
          end do
c
        end if
c
c.. if i3.le.n3b, then compute fluxes in the s1 and s2 directions
        if( i3.le.n3b ) then
c
c.. compute s1 and s2 fluxes
c          if (.true.) then
          if( icart.eq.0 )then
c
c        write(6,*)'here i am (4)'
c.. compute s1 fluxes (non-Cartesian case)
            do i2 = n2a,n2b
            do i1 = n1a-1,n1b
              i1p1 = i1+1
              if( mask(i1,i2,i3).ne.0.and.mask(i1p1,i2,i3).ne.0 ) then
                aj0 = 0.5*(aj(i1p1,i2,lay)+aj(i1,i2,lay))
                met0(1,1) = 0.5*(a1(1,1,i1p1,i2,lay)+a1(1,1,i1,i2,lay))
                met0(1,2) = 0.5*(a1(1,2,i1p1,i2,lay)+a1(1,2,i1,i2,lay))
                met0(1,3) = 0.5*(a1(1,3,i1p1,i2,lay)+a1(1,3,i1,i2,lay))
                met0(2,1) = 0.5*(a1(2,1,i1p1,i2,lay)+a1(2,1,i1,i2,lay))
                met0(2,2) = 0.5*(a1(2,2,i1p1,i2,lay)+a1(2,2,i1,i2,lay))
                met0(2,3) = 0.5*(a1(2,3,i1p1,i2,lay)+a1(2,3,i1,i2,lay))
                met0(3,1) = 0.5*(a1(3,1,i1p1,i2,lay)+a1(3,1,i1,i2,lay))
                met0(3,2) = 0.5*(a1(3,2,i1p1,i2,lay)+a1(3,2,i1,i2,lay))
                met0(3,3) = 0.5*(a1(3,3,i1p1,i2,lay)+a1(3,3,i1,i2,lay))

                ! solve Riemann problem
                call smupflux3d( m,aj0,met0,w(1,i1,i2,s1p,lay),
     *             w(1,i1p1,i2,s1m,lay),fx,almax(1) )
                do k = 1,12
                  up(i1p1,i2,i3,k) = up(i1p1,i2,i3,k)+
     *               fx(k)/(ds1*aj(i1p1,i2,lay))
                  up(i1,i2,i3,k) = up(i1,i2,i3,k)-
     *               fx(k)/(ds1*aj(i1,i2,lay))
                end do
              end if
            end do
            end do
c
c.. compute s2 fluxes (non-Cartesian case)
            do i2 = n2a-1,n2b
            do i1 = n1a,n1b
              i2p1 = i2+1
              if( mask(i1,i2,i3).ne.0.and.mask(i1,i2p1,i3).ne.0 ) then
                aj0 = 0.5*(aj(i1,i2p1,lay)+aj(i1,i2,lay))
                met0(1,1) = 0.5*(a1(2,1,i1,i2p1,lay)+a1(2,1,i1,i2,lay))
                met0(1,2) = 0.5*(a1(2,2,i1,i2p1,lay)+a1(2,2,i1,i2,lay))
                met0(1,3) = 0.5*(a1(2,3,i1,i2p1,lay)+a1(2,3,i1,i2,lay))
                met0(2,1) = 0.5*(a1(3,1,i1,i2p1,lay)+a1(3,1,i1,i2,lay))
                met0(2,2) = 0.5*(a1(3,2,i1,i2p1,lay)+a1(3,2,i1,i2,lay))
                met0(2,3) = 0.5*(a1(3,3,i1,i2p1,lay)+a1(3,3,i1,i2,lay))
                met0(3,1) = 0.5*(a1(1,1,i1,i2p1,lay)+a1(1,1,i1,i2,lay))
                met0(3,2) = 0.5*(a1(1,2,i1,i2p1,lay)+a1(1,2,i1,i2,lay))
                met0(3,3) = 0.5*(a1(1,3,i1,i2p1,lay)+a1(1,3,i1,i2,lay))

                ! solve Riemann problem
                call smupflux3d( m,aj0,met0,w(1,i1,i2,s2p,lay),
     *             w(1,i1,i2p1,s2m,lay),fx,almax(2) )
                do k = 1,12
                  up(i1,i2p1,i3,k) = up(i1,i2p1,i3,k)+
     *               fx(k)/(ds2*aj(i1,i2p1,lay))
                  up(i1,i2,i3,k) = up(i1,i2,i3,k)-
     *               fx(k)/(ds2*aj(i1,i2,lay))
                end do
              end if
            end do
            end do
c
          else
c
c.. compute s1 fluxes (Cartesian case)
            do i2 = n2a,n2b
            do i1 = n1a-1,n1b
              i1p1 = i1+1
              if( mask(i1,i2,i3).ne.0.and.mask(i1p1,i2,i3).ne.0 ) then
c                aj0 = 0.5*(aj(i1p1,i2,lay)+aj(i1,i2,lay))
c                met0(1,1) = 0.5*(a1(1,1,i1p1,i2,lay)+a1(1,1,i1,i2,lay))
c                met0(1,2) = 0.5*(a1(1,2,i1p1,i2,lay)+a1(1,2,i1,i2,lay))
c                met0(1,3) = 0.5*(a1(1,3,i1p1,i2,lay)+a1(1,3,i1,i2,lay))
c                met0(2,1) = 0.5*(a1(2,1,i1p1,i2,lay)+a1(2,1,i1,i2,lay))
c                met0(2,2) = 0.5*(a1(2,2,i1p1,i2,lay)+a1(2,2,i1,i2,lay))
c                met0(2,3) = 0.5*(a1(2,3,i1p1,i2,lay)+a1(2,3,i1,i2,lay))
c                met0(3,1) = 0.5*(a1(3,1,i1p1,i2,lay)+a1(3,1,i1,i2,lay))
c                met0(3,2) = 0.5*(a1(3,2,i1p1,i2,lay)+a1(3,2,i1,i2,lay))
c                met0(3,3) = 0.5*(a1(3,3,i1p1,i2,lay)+a1(3,3,i1,i2,lay))
                do k=1,12
                  ul(k) = w(k,i1  ,i2,s1p,lay)
                  ur(k) = w(k,i1p1,i2,s1m,lay)
                end do
                fx( 1) = (-cf1*(ul(4)+ur(4))-df1*(ur(1)-ul(1)))/ds1  ! v1
                fx( 2) = (-cf1*(ul(5)+ur(5))-df2*(ur(2)-ul(2)))/ds1  ! v2
                fx( 3) = (-cf1*(ul(6)+ur(6))-df2*(ur(3)-ul(3)))/ds1  ! v3
                fx( 4) = (-cf2*(ul(1)+ur(1))-df1*(ur(4)-ul(4)))/ds1  ! s11
                fx( 5) = (-cf4*(ul(2)+ur(2))-df2*(ur(5)-ul(5)))/ds1  ! s12
                fx( 6) = (-cf4*(ul(3)+ur(3))-df2*(ur(6)-ul(6)))/ds1  ! s13
                fx( 7) = fx(5)                                       ! s21
                fx( 8) = (-cf3*(ul(1)+ur(1))-df3*(ur(4)-ul(4)))/ds1  ! s22
                fx( 9) = 0.                                          ! s23
                fx(10) = fx(6)                                       ! s31
                fx(11) = 0.                                          ! s32
                fx(12) = fx(8)                                       ! s33
c                call smupflux3d( m,aj0,met0,w(1,i1,i2,s1p,lay),
c     *             w(1,i1p1,i2,s1m,lay),fx0,almax(1) )
                do k = 1,12
                  up(i1p1,i2,i3,k) = up(i1p1,i2,i3,k)+fx(k)
                  up(i1  ,i2,i3,k) = up(i1  ,i2,i3,k)-fx(k)
c                  up(i1p1,i2,i3,k) = up(i1p1,i2,i3,k)+
c     *               fx0(k)/(ds1*aj(i1p1,i2,lay))
c                  up(i1,i2,i3,k) = up(i1,i2,i3,k)-
c     *               fx0(k)/(ds1*aj(i1,i2,lay))
c                  errf(k,1) = max(abs(fx(k)-fx0(k)/ds1),errf(k,1))
                end do
              end if
            end do
            end do
c
c.. compute s2 fluxes (Cartesian case)
            do i2 = n2a-1,n2b
            do i1 = n1a,n1b
              i2p1 = i2+1
              if( mask(i1,i2,i3).ne.0.and.mask(i1,i2p1,i3).ne.0 ) then
c                aj0 = 0.5*(aj(i1,i2p1,lay)+aj(i1,i2,lay))
c                met0(1,1) = 0.5*(a1(2,1,i1,i2p1,lay)+a1(2,1,i1,i2,lay))
c                met0(1,2) = 0.5*(a1(2,2,i1,i2p1,lay)+a1(2,2,i1,i2,lay))
c                met0(1,3) = 0.5*(a1(2,3,i1,i2p1,lay)+a1(2,3,i1,i2,lay))
c                met0(2,1) = 0.5*(a1(3,1,i1,i2p1,lay)+a1(3,1,i1,i2,lay))
c                met0(2,2) = 0.5*(a1(3,2,i1,i2p1,lay)+a1(3,2,i1,i2,lay))
c                met0(2,3) = 0.5*(a1(3,3,i1,i2p1,lay)+a1(3,3,i1,i2,lay))
c                met0(3,1) = 0.5*(a1(1,1,i1,i2p1,lay)+a1(1,1,i1,i2,lay))
c                met0(3,2) = 0.5*(a1(1,2,i1,i2p1,lay)+a1(1,2,i1,i2,lay))
c                met0(3,3) = 0.5*(a1(1,3,i1,i2p1,lay)+a1(1,3,i1,i2,lay))
                do k=1,12
                  ul(k) = w(k,i1,i2  ,s2p,lay)
                  ur(k) = w(k,i1,i2p1,s2m,lay)
                end do
                fx( 1) = (-cf1*(ul(7)+ur(7))-df2*(ur(1)-ul(1)))/ds2  ! v1
                fx( 2) = (-cf1*(ul(8)+ur(8))-df1*(ur(2)-ul(2)))/ds2  ! v2
                fx( 3) = (-cf1*(ul(9)+ur(9))-df2*(ur(3)-ul(3)))/ds2  ! v3
                fx( 7) = (-cf4*(ul(1)+ur(1))-df2*(ur(7)-ul(7)))/ds2  ! s21
                fx( 8) = (-cf2*(ul(2)+ur(2))-df1*(ur(8)-ul(8)))/ds2  ! s22
                fx( 9) = (-cf4*(ul(3)+ur(3))-df2*(ur(9)-ul(9)))/ds2  ! s23
                fx(10) = 0.                                          ! s31
                fx(11) = fx(9)                                       ! s32
                fx(12) = (-cf3*(ul(2)+ur(2))-df3*(ur(8)-ul(8)))/ds1  ! s33
                fx( 4) = fx(12)                                      ! s11
                fx( 5) = fx(7)                                       ! s12
                fx( 6) = 0.                                          ! s13
c                call smupflux3d( m,aj0,met0,w(1,i1,i2,s2p,lay),
c     *             w(1,i1,i2p1,s2m,lay),fx0,almax(2) )
                do k = 1,12
                  up(i1,i2p1,i3,k) = up(i1,i2p1,i3,k)+fx(k)
                  up(i1,i2  ,i3,k) = up(i1,i2  ,i3,k)-fx(k)
c                  up(i1,i2p1,i3,k) = up(i1,i2p1,i3,k)+
c     *               fx0(k)/(ds2*aj(i1,i2p1,lay))
c                  up(i1,i2,i3,k) = up(i1,i2,i3,k)-
c     *               fx0(k)/(ds2*aj(i1,i2,lay))
c                  errf(k,2) = max(abs(fx(k)-fx0(k)/ds2),errf(k,2))
                end do
              end if
            end do
            end do
c
          end if
c
c.. add free stream correction here ... dws ...
c          if( icart.eq.0 .and. .false. )then
          if( icart.eq.0 )then
            do i2 = n2a,n2b
            do i1 = n1a,n1b
              if( mask(i1,i2,i3).ne.0 )then
                d1p = det(i1+1,i2,i3)+det(i1,i2,i3)
                d1m = det(i1-1,i2,i3)+det(i1,i2,i3)
                d2p = det(i1,i2+1,i3)+det(i1,i2,i3)
                d2m = det(i1,i2-1,i3)+det(i1,i2,i3)
                d3p = det(i1,i2,i3+1)+det(i1,i2,i3)
                d3m = det(i1,i2,i3-1)+det(i1,i2,i3)
                do j = 1,3
                  da = ((rx(i1+1,i2,i3,1,j)+rx(i1,i2,i3,1,j))*d1p
     *                 -(rx(i1-1,i2,i3,1,j)+rx(i1,i2,i3,1,j))*d1m)
     *                 /(4.0*ds1)
     *                +((rx(i1,i2+1,i3,2,j)+rx(i1,i2,i3,2,j))*d2p
     *                 -(rx(i1,i2-1,i3,2,j)+rx(i1,i2,i3,2,j))*d2m)
     *                 /(4.0*ds2)
     *                +((rx(i1,i2,i3+1,3,j)+rx(i1,i2,i3,3,j))*d3p
     *                 -(rx(i1,i2,i3-1,3,j)+rx(i1,i2,i3,3,j))*d3m)
     *                 /(4.0*ds3)
                  call smflux3dfs( j,w(1,i1,i2,scent,lay),fx )
                  do k = 1,12
                    up(i1,i2,i3,k) = up(i1,i2,i3,k)+da*fx(k)
     *                                              /det(i1,i2,i3)
                  end do
                end do
              end if
            end do
            end do
          end if
c
c.. complete up
          do k = 13,15
            do i2 = n2a,n2b
            do i1 = n1a,n1b
              up(i1,i2,i3,k) = w(k-12,i1,i2,scent,lay)
            end do
            end do
          end do
        end if
c
c.. twilight zone
        if( itz.ne.0 ) then
          t1 = t
          if( iorder.eq.2 ) t1 = t1+0.5*dt
          do i2 = n2a,n2b
          do i1 = n1a,n1b
            call smuptz3d( m,xy(i1,i2,i3,1),xy(i1,i2,i3,2),
     *         xy(i1,i2,i3,3),t1,htz )
            do k = 1,15
              up(i1,i2,i3,k) = up(i1,i2,i3,k)+htz(k)
            end do
          end do
          end do
        end if
c
c.. end loop over layers
      end do
c
c.. print slope errors
c      do k=1,12
c        write(6,123)k,errw(k,1),errw(k,2),errw(k,3),errw(k,4),errw(k,5)
c  123   format(1x,i2,5(1x,1pe9.2))
c      end do
c      pause
c
c.. print flux errors
c      do k=1,12
c        write(6,234)k,errf(k,1),errf(k,2),errf(k,3)
c  234   format(1x,i2,3(1x,1pe9.2))
c      end do
c      pause
c
      return
      end
c
c+++++++++++++++++++++++
c
      subroutine smupflux3d( m,aj,met,wl,wr,fx,speed )
c
      implicit none
c.. ingoing declarations
      integer m
      real aj,met(3,3),wl(*),wr(*),fx(*),speed
      include 'smupcommons.h'
c
c.. local declarations
      integer j,k
      real norm(3),tan1(3),tan2(3)
      real al(12),el(12,12),er(12,12)
      real alpha(12),alp
c
c.. metrics
      norm(1) = met(1,1)
      norm(2) = met(1,2)
      norm(3) = met(1,3)
      tan1(1) = met(2,1)
      tan1(2) = met(2,2)
      tan1(3) = met(2,3)
      tan2(1) = met(3,1)
      tan2(2) = met(3,2)
      tan2(3) = met(3,3)

c
c.. get eigen structure. We could get away with part of this structure
c     as was done in smg2d, but in order to reduce coding errors
c     we will just go for the glory for now.      
      call smeig3d( norm,tan1,tan2,al,el,er )
c
c.. wave strengths (from the left state)
      do k = 1,12
        alpha(k) = 0.0
        do j = 1,12
          alpha(k) = alpha(k)+el(k,j)*(wr(j)-wl(j))
        end do
      end do
c
c.. flux (on the left)
c      do k = 1,12
c        wl(k) = 0.5*(wl(k)+wr(k))
c      end do
      call smflux3d( norm,aj,wl,fx )
c
c.. Riemann solution flux (computed from the left)
      do j = 1,12
        if( al(j).lt.0.0 ) then
          alp = aj*alpha(j)*al(j)
          do k = 1,12
            fx(k) = fx(k)+alp*er(k,j)
          end do
        end if
c
c.. fastest wave speed
        speed = max( abs(al(j)),speed )
      end do
c
      return
      end
c
c++++++++++++++++++++
c
      subroutine smupslope3d( m,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *                          md1a,md1b,md2a,md2b,md3a,md3b,
     *                          n1a, n1b, n2a, n2b, n3a, n3b,
     *                          i3,ds1,ds2,ds3,dt,t,xy,
     *                          a1,aj,mask,u,w )
c
      implicit none
c.. ingoing declarations
      integer m,i3
      integer nd1a,nd1b,nd2a,nd2b,nd3a,nd3b
      integer md1a,md1b,md2a,md2b,md3a,md3b
      integer n1a, n1b, n2a, n2b, n3a, n3b
      integer mask(nd1a:nd1b,nd2a:nd2b)
      real ds1,ds2,ds3,dt,t
      real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,3)
      real a1(3,3,md1a:md1b,md2a:md2b)
      real aj(md1a:md1b,md2a:md2b)
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,m)
      real w(m,md1a:md1b,md2a:md2b,7)
      include 'smupcommons.h'
c
c.. local declarations
      integer i1,i2,j,k,dir
      real al(12),el(12,12),er(12,12)
      real norm(3),tan1(3),tan2(3)
      real htz(15)
      real alphal,alphar,alpha,tmp,tmp2,tmp3
      real s1,s2,s3
      real ur1(12),ur2(12),ur3(12),up
      real fr1(12),fr2(12),fr3(12)
c
c.. copy solution into w
      do dir = 1,7
        do k = 1,12
          do i2 = md2a,md2b
          do i1 = md1a,md1b
            w(k,i1,i2,dir) = u(i1,i2,i3,k)
          end do
          end do
        end do
      end do
c
      if( iorder.eq.1 ) return
c
      s1 = 0.5*dt/ds1
      s2 = 0.5*dt/ds2
      s3 = 0.5*dt/ds3
c
c.. loop over space
c
c.. twilight zone stuff
      if( itz.ne.0 ) then
c        pause
        do i2 = n2a-1,n2b+1
        do i1 = n1a-1,n1b+1
          if( mask(i1,i2).ne.0 ) then
            call smuptz3d( m,xy(i1,i2,i3,1),xy(i1,i2,i3,2),
     *                       xy(i1,i2,i3,3),t,htz )
            do dir = 1,7
              do k = 1,12
                w(k,i1,i2,dir) = w(k,i1,i2,dir)+0.5*dt*htz(k)
              end do
            end do
          end if
        end do
        end do
      end if

      do i2 = n2a-1,n2b+1
      do i1 = n1a-1,n1b+1
        if( mask(i1,i2).ne.0 ) then
c
c.. twilight zone stuff
c          if( itz.ne.0 ) then
c            call smuptz3d( m,xy(i1,i2,i3,1),xy(i1,i2,i3,2),
c     *                     xy(i1,i2,i3,3),t,htz )
c            do dir = 1,7
c              do k = 1,12
c                w(k,i1,i2,dir) = w(k,i1,i2,dir)+0.5*dt*htz(k)
c              end do
c            end do
c          end if
c
c          if( .true. ) then
c          if( .false. ) then
          if( ilimit.eq.0 ) then
            do j = 1,12
              ur1(j) = u(i1+1,i2,i3,j)-u(i1-1,i2,i3,j)
              ur2(j) = u(i1,i2+1,i3,j)-u(i1,i2-1,i3,j)
              ur3(j) = u(i1,i2,i3+1,j)-u(i1,i2,i3-1,j)
            end do
            norm(1) = a1(1,1,i1,i2)
            norm(2) = a1(1,2,i1,i2)
            norm(3) = a1(1,3,i1,i2)         
            call smflux3d( norm,1.0,ur1,fr1 )

            norm(1) = a1(2,1,i1,i2)
            norm(2) = a1(2,2,i1,i2)
            norm(3) = a1(2,3,i1,i2)         
            call smflux3d( norm,1.0,ur2,fr2 )

            norm(1) = a1(3,1,i1,i2)
            norm(2) = a1(3,2,i1,i2)
            norm(3) = a1(3,3,i1,i2)         
            call smflux3d( norm,1.0,ur3,fr3 )

            do j = 1,12
              up = -fr1(j)/ds1-fr2(j)/ds2-fr3(j)/ds3
              w(j,i1,i2,1) = w(j,i1,i2,1)+0.25*(-ur1(j)+dt*up)
              w(j,i1,i2,2) = w(j,i1,i2,2)+0.25*( ur1(j)+dt*up)
              w(j,i1,i2,3) = w(j,i1,i2,3)+0.25*(-ur2(j)+dt*up)
              w(j,i1,i2,4) = w(j,i1,i2,4)+0.25*( ur2(j)+dt*up)
              w(j,i1,i2,5) = w(j,i1,i2,5)+0.25*(-ur3(j)+dt*up)
              w(j,i1,i2,6) = w(j,i1,i2,6)+0.25*( ur3(j)+dt*up)
              w(j,i1,i2,7) = w(j,i1,i2,7)+0.25*dt*up
            end do
          else
c
c.. s1 direction
          norm(1) = a1(1,1,i1,i2)
          norm(2) = a1(1,2,i1,i2)
          norm(3) = a1(1,3,i1,i2)
          tan1(1) = a1(2,1,i1,i2)
          tan1(2) = a1(2,2,i1,i2)
          tan1(3) = a1(2,3,i1,i2)
          tan2(1) = a1(3,1,i1,i2)
          tan2(2) = a1(3,2,i1,i2)
          tan2(3) = a1(3,3,i1,i2)
          call smeig3d( norm,tan1,tan2,al,el,er )
c
          do j = 1,12
            alphal = 0.0
            alphar = 0.0
            do k = 1,12
              alphal = alphal+el(j,k)*(u(i1,i2,i3,k)-u(i1-1,i2,i3,k))
              alphar = alphar+el(j,k)*(u(i1+1,i2,i3,k)-u(i1,i2,i3,k))
            end do
c
            if( ilimit.eq.0 ) then
              alphal = 0.5*(alphal+alphar)
              alphar = alphal
            end if
c
            if( alphal*alphar.gt.0.0 ) then
              if( abs(alphal).lt.abs(alphar) ) then
                alpha = alphal
              else
                alpha = alphar
              end if
c
              tmp3 = s1*al(j)
              tmp = tmp3*alpha
              do k = 1,12
                tmp2 = tmp*er(k,j)
                w(k,i1,i2,1) = w(k,i1,i2,1)-(tmp3+0.5)*alpha*er(k,j)
                w(k,i1,i2,2) = w(k,i1,i2,2)-(tmp3-0.5)*alpha*er(k,j)
                w(k,i1,i2,3) = w(k,i1,i2,3)-tmp2
                w(k,i1,i2,4) = w(k,i1,i2,4)-tmp2
                w(k,i1,i2,5) = w(k,i1,i2,5)-tmp2
                w(k,i1,i2,6) = w(k,i1,i2,6)-tmp2
                w(k,i1,i2,7) = w(k,i1,i2,7)-tmp2
              end do
            end if
          end do
c
c.. s2 direction
          norm(1) = a1(2,1,i1,i2)
          norm(2) = a1(2,2,i1,i2)
          norm(3) = a1(2,3,i1,i2)
          tan1(1) = a1(3,1,i1,i2)
          tan1(2) = a1(3,2,i1,i2)
          tan1(3) = a1(3,3,i1,i2)
          tan2(1) = a1(1,1,i1,i2)
          tan2(2) = a1(1,2,i1,i2)
          tan2(3) = a1(1,3,i1,i2)
          call smeig3d( norm,tan1,tan2,al,el,er )
c
          do j = 1,12
            alphal = 0.0
            alphar = 0.0
            do k = 1,12
              alphal = alphal+el(j,k)*(u(i1,i2,i3,k)-u(i1,i2-1,i3,k))
              alphar = alphar+el(j,k)*(u(i1,i2+1,i3,k)-u(i1,i2,i3,k))
            end do
c
            if( ilimit.eq.0 ) then
              alphal = 0.5*(alphal+alphar)
              alphar = alphal
            end if
c
            if( alphal*alphar.gt.0.0 ) then
              if( abs(alphal).lt.abs(alphar) ) then
                alpha = alphal
              else
                alpha = alphar
              end if
c
              tmp3 = s2*al(j)
              tmp = tmp3*alpha
              do k = 1,12
                tmp2 = tmp*er(k,j)
                w(k,i1,i2,1) = w(k,i1,i2,1)-tmp2
                w(k,i1,i2,2) = w(k,i1,i2,2)-tmp2
                w(k,i1,i2,3) = w(k,i1,i2,3)-(tmp3+0.5)*alpha*er(k,j)
                w(k,i1,i2,4) = w(k,i1,i2,4)-(tmp3-0.5)*alpha*er(k,j)
                w(k,i1,i2,5) = w(k,i1,i2,5)-tmp2
                w(k,i1,i2,6) = w(k,i1,i2,6)-tmp2
                w(k,i1,i2,7) = w(k,i1,i2,7)-tmp2
              end do
            end if
          end do
c
c.. s3 direction
          norm(1) = a1(3,1,i1,i2)
          norm(2) = a1(3,2,i1,i2)
          norm(3) = a1(3,3,i1,i2)
          tan1(1) = a1(1,1,i1,i2)
          tan1(2) = a1(1,2,i1,i2)
          tan1(3) = a1(1,3,i1,i2)
          tan2(1) = a1(2,1,i1,i2)
          tan2(2) = a1(2,2,i1,i2)
          tan2(3) = a1(2,3,i1,i2)
          call smeig3d( norm,tan1,tan2,al,el,er )
c
          do j = 1,12
            alphal = 0.0
            alphar = 0.0
            do k = 1,12
              alphal = alphal+el(j,k)*(u(i1,i2,i3,k)-u(i1,i2,i3-1,k))
              alphar = alphar+el(j,k)*(u(i1,i2,i3+1,k)-u(i1,i2,i3,k))
            end do
c
            if( ilimit.eq.0 ) then
              alphal = 0.5*(alphal+alphar)
              alphar = alphal
            end if
c
            if( alphal*alphar.gt.0.0 ) then
              if( abs(alphal).lt.abs(alphar) ) then
                alpha = alphal
              else
                alpha = alphar
              end if
c
              tmp3 = s3*al(j)
              tmp = tmp3*alpha
              do k = 1,12
                tmp2 = tmp*er(k,j)
                w(k,i1,i2,1) = w(k,i1,i2,1)-tmp2
                w(k,i1,i2,2) = w(k,i1,i2,2)-tmp2
                w(k,i1,i2,3) = w(k,i1,i2,3)-tmp2
                w(k,i1,i2,4) = w(k,i1,i2,4)-tmp2
                w(k,i1,i2,5) = w(k,i1,i2,5)-(tmp3+0.5)*alpha*er(k,j)
                w(k,i1,i2,6) = w(k,i1,i2,6)-(tmp3-0.5)*alpha*er(k,j)
                w(k,i1,i2,7) = w(k,i1,i2,7)-tmp2
              end do
            end if
          end do
c
          end if
c
        end if
      end do
      end do
c
      return
      end
c
c++++++++++++++++++++
c
      subroutine smupslope3dO( m,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *                           md1a,md1b,md2a,md2b,md3a,md3b,
     *                           n1a, n1b, n2a, n2b, n3a, n3b,
     *                           i3,ds1,ds2,ds3,dt,rx,det,
     *                           mask,u,w )
c
      implicit none
c.. ingoing declarations
      integer m,i3
      integer nd1a,nd1b,nd2a,nd2b,nd3a,nd3b
      integer md1a,md1b,md2a,md2b,md3a,md3b
      integer n1a, n1b, n2a, n2b, n3a, n3b
      integer mask(nd1a:nd1b,nd2a:nd2b)
      real ds1,ds2,ds3,dt
      real rx(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,3,3)
      real det(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,m)
      real w(m,md1a:md1b,md2a:md2b,7)
      include 'smupcommons.h'
c
c.. local declarations
      integer i1,i2,k
      real du1(12),du2(12),du3(12),ut(12)
      real cx1,cx2,cx3,cx4,cy1,cy2,cy3,cy4,cz1,cz2,cz3,cz4
c
c.. compute constants here for now
      cx1=dt/(rho0*ds1)
      cx2=(lam+2.0*mu)*dt/ds1
      cx3=lam*dt/ds1
      cx4=mu*dt/ds1
      cy1=dt/(rho0*ds2)
      cy2=(lam+2.0*mu)*dt/ds2
      cy3=lam*dt/ds2
      cy4=mu*dt/ds2
      cz1=dt/(rho0*ds3)
      cz2=(lam+2.0*mu)*dt/ds3
      cz3=lam*dt/ds3
      cz4=mu*dt/ds3
c
      if( icart.eq.0) then
c
        do i2 = n2a-1,n2b+1
        do i1 = n1a-1,n1b+1
          if( mask(i1,i2).ne.0 ) then
c
            do k = 1,12
              du1(k) = .25*(u(i1+1,i2,i3,k)-u(i1-1,i2,i3,k))
              du2(k) = .25*(u(i1,i2+1,i3,k)-u(i1,i2-1,i3,k))
              du3(k) = .25*(u(i1,i2,i3+1,k)-u(i1,i2,i3-1,k))
            end do
c
            ut( 1) = u(i1,i2,i3, 1)+cx1*(rx(i1,i2,i3,1,1)*du1(4)
     *                                  +rx(i1,i2,i3,1,2)*du1(7)
     *                                  +rx(i1,i2,i3,1,3)*du1(10))
     *                             +cy1*(rx(i1,i2,i3,2,1)*du2(4)
     *                                  +rx(i1,i2,i3,2,2)*du2(7)
     *                                  +rx(i1,i2,i3,2,3)*du2(10))
     *                             +cz1*(rx(i1,i2,i3,3,1)*du3(4)
     *                                  +rx(i1,i2,i3,3,2)*du3(7)
     *                                  +rx(i1,i2,i3,3,3)*du3(10))
            ut( 2) = u(i1,i2,i3, 2)+cx1*(rx(i1,i2,i3,1,1)*du1(5)
     *                                  +rx(i1,i2,i3,1,2)*du1(8)
     *                                  +rx(i1,i2,i3,1,3)*du1(11))
     *                             +cy1*(rx(i1,i2,i3,2,1)*du2(5)
     *                                  +rx(i1,i2,i3,2,2)*du2(8)
     *                                  +rx(i1,i2,i3,2,3)*du2(11))
     *                             +cz1*(rx(i1,i2,i3,3,1)*du3(5)
     *                                  +rx(i1,i2,i3,3,2)*du3(8)
     *                                  +rx(i1,i2,i3,3,3)*du3(11))
            ut( 3) = u(i1,i2,i3, 3)+cx1*(rx(i1,i2,i3,1,1)*du1(6)
     *                                  +rx(i1,i2,i3,1,2)*du1(9)
     *                                  +rx(i1,i2,i3,1,3)*du1(12))
     *                             +cy1*(rx(i1,i2,i3,2,1)*du2(6)
     *                                  +rx(i1,i2,i3,2,2)*du2(9)
     *                                  +rx(i1,i2,i3,2,3)*du2(12))
     *                             +cz1*(rx(i1,i2,i3,3,1)*du3(6)
     *                                  +rx(i1,i2,i3,3,2)*du3(9)
     *                                  +rx(i1,i2,i3,3,3)*du3(12))
            ut( 4) = u(i1,i2,i3, 4)+cx2* rx(i1,i2,i3,1,1)*du1(1)
     *                             +cx3*(rx(i1,i2,i3,1,2)*du1(2)
     *                                  +rx(i1,i2,i3,1,3)*du1(3))
     *                             +cy2* rx(i1,i2,i3,2,1)*du2(1)
     *                             +cy3*(rx(i1,i2,i3,2,2)*du2(2)
     *                                  +rx(i1,i2,i3,2,3)*du2(3))
     *                             +cz2* rx(i1,i2,i3,3,1)*du3(1)
     *                             +cz3*(rx(i1,i2,i3,3,2)*du3(2)
     *                                  +rx(i1,i2,i3,3,3)*du3(3))
            ut( 5) = u(i1,i2,i3, 5)+cx4*(rx(i1,i2,i3,1,1)*du1(2)
     *                                  +rx(i1,i2,i3,1,2)*du1(1))
     *                             +cy4*(rx(i1,i2,i3,2,1)*du2(2)
     *                                  +rx(i1,i2,i3,2,2)*du2(1))
     *                             +cz4*(rx(i1,i2,i3,3,1)*du3(2)
     *                                  +rx(i1,i2,i3,3,2)*du3(1))
            ut( 6) = u(i1,i2,i3, 6)+cx4*(rx(i1,i2,i3,1,1)*du1(3)
     *                                  +rx(i1,i2,i3,1,3)*du1(1))
     *                             +cy4*(rx(i1,i2,i3,2,1)*du2(3)
     *                                  +rx(i1,i2,i3,2,3)*du2(1))
     *                             +cz4*(rx(i1,i2,i3,3,1)*du3(3)
     *                                  +rx(i1,i2,i3,3,3)*du3(1))
            ut( 7) = u(i1,i2,i3, 7)+cx4*(rx(i1,i2,i3,1,1)*du1(2)
     *                                  +rx(i1,i2,i3,1,2)*du1(1))
     *                             +cy4*(rx(i1,i2,i3,2,1)*du2(2)
     *                                  +rx(i1,i2,i3,2,2)*du2(1))
     *                             +cz4*(rx(i1,i2,i3,3,1)*du3(2)
     *                                  +rx(i1,i2,i3,3,2)*du3(1))
            ut( 8) = u(i1,i2,i3, 8)+cx2* rx(i1,i2,i3,1,2)*du1(2)
     *                             +cx3*(rx(i1,i2,i3,1,3)*du1(3)
     *                                  +rx(i1,i2,i3,1,1)*du1(1))
     *                             +cy2* rx(i1,i2,i3,2,2)*du2(2)
     *                             +cy3*(rx(i1,i2,i3,2,3)*du2(3)
     *                                  +rx(i1,i2,i3,2,1)*du2(1))
     *                             +cz2* rx(i1,i2,i3,3,2)*du3(2)
     *                             +cz3*(rx(i1,i2,i3,3,3)*du3(3)
     *                                  +rx(i1,i2,i3,3,1)*du3(1))
            ut( 9) = u(i1,i2,i3, 9)+cx4*(rx(i1,i2,i3,1,2)*du1(3)
     *                                  +rx(i1,i2,i3,1,3)*du1(2))
     *                             +cy4*(rx(i1,i2,i3,2,2)*du2(3)
     *                                  +rx(i1,i2,i3,2,3)*du2(2))
     *                             +cz4*(rx(i1,i2,i3,3,2)*du3(3)
     *                                  +rx(i1,i2,i3,3,3)*du3(2))
            ut(10) = u(i1,i2,i3,10)+cx4*(rx(i1,i2,i3,1,1)*du1(3)
     *                                  +rx(i1,i2,i3,1,3)*du1(1))
     *                             +cy4*(rx(i1,i2,i3,2,1)*du2(3)
     *                                  +rx(i1,i2,i3,2,3)*du2(1))
     *                             +cz4*(rx(i1,i2,i3,3,1)*du3(3)
     *                                  +rx(i1,i2,i3,3,3)*du3(1))
            ut(11) = u(i1,i2,i3,11)+cx4*(rx(i1,i2,i3,1,2)*du1(3)
     *                                  +rx(i1,i2,i3,1,3)*du1(2))
     *                             +cy4*(rx(i1,i2,i3,2,2)*du2(3)
     *                                  +rx(i1,i2,i3,2,3)*du2(2))
     *                             +cz4*(rx(i1,i2,i3,3,2)*du3(3)
     *                                  +rx(i1,i2,i3,3,3)*du3(2))
            ut(12) = u(i1,i2,i3,12)+cx2* rx(i1,i2,i3,1,3)*du1(3)
     *                             +cx3*(rx(i1,i2,i3,1,1)*du1(1)
     *                                  +rx(i1,i2,i3,1,2)*du1(2))
     *                             +cy2* rx(i1,i2,i3,2,3)*du2(3)
     *                             +cy3*(rx(i1,i2,i3,2,1)*du2(1)
     *                                  +rx(i1,i2,i3,2,2)*du2(2))
     *                             +cz2* rx(i1,i2,i3,3,3)*du3(3)
     *                             +cz3*(rx(i1,i2,i3,3,1)*du3(1)
     *                                  +rx(i1,i2,i3,3,2)*du3(2))
c
            do k = 1,12
              w(k,i1,i2,1) = ut(k)-du1(k)
              w(k,i1,i2,2) = ut(k)+du1(k)
              w(k,i1,i2,3) = ut(k)-du2(k)
              w(k,i1,i2,4) = ut(k)+du2(k)
              w(k,i1,i2,5) = ut(k)-du3(k)
              w(k,i1,i2,6) = ut(k)+du3(k)
              w(k,i1,i2,7) = ut(k)
            end do
          end if
        end do
        end do
c
      else
c
        do i2 = n2a-1,n2b+1
        do i1 = n1a-1,n1b+1
          if( mask(i1,i2).ne.0 ) then
c
            do k = 1,12
              du1(k) = .25*(u(i1+1,i2,i3,k)-u(i1-1,i2,i3,k))
              du2(k) = .25*(u(i1,i2+1,i3,k)-u(i1,i2-1,i3,k))
              du3(k) = .25*(u(i1,i2,i3+1,k)-u(i1,i2,i3-1,k))
            end do
c
            ut( 1) = u(i1,i2,i3, 1)+cx1*du1(4)+cy1*du2(7)+cz1*du3(10)
            ut( 2) = u(i1,i2,i3, 2)+cx1*du1(5)+cy1*du2(8)+cz1*du3(11)
            ut( 3) = u(i1,i2,i3, 3)+cx1*du1(6)+cy1*du2(9)+cz1*du3(12)
            ut( 4) = u(i1,i2,i3, 4)+cx2*du1(1)+cy3*du2(2)+cz3*du3(3)
            ut( 5) = u(i1,i2,i3, 5)+cx4*du1(2)+cy4*du2(1)
            ut( 6) = u(i1,i2,i3, 6)+cx4*du1(3)           +cz4*du3(1)
            ut( 7) = u(i1,i2,i3, 7)+cx4*du1(2)+cy4*du2(1)
            ut( 8) = u(i1,i2,i3, 8)+cx3*du1(1)+cy2*du2(2)+cz3*du3(3)
            ut( 9) = u(i1,i2,i3, 9)           +cy4*du2(3)+cz4*du3(2)
            ut(10) = u(i1,i2,i3,10)+cx4*du1(3)           +cz4*du3(1)
            ut(11) = u(i1,i2,i3,11)           +cy4*du2(3)+cz4*du3(2)
            ut(12) = u(i1,i2,i3,12)+cx3*du1(1)+cy3*du2(2)+cz2*du3(3)
c
            do k = 1,12
              w(k,i1,i2,1) = ut(k)-du1(k)
              w(k,i1,i2,2) = ut(k)+du1(k)
              w(k,i1,i2,3) = ut(k)-du2(k)
              w(k,i1,i2,4) = ut(k)+du2(k)
              w(k,i1,i2,5) = ut(k)-du3(k)
              w(k,i1,i2,6) = ut(k)+du3(k)
              w(k,i1,i2,7) = ut(k)
            end do
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
      subroutine smeig3d( a,s,t,al,el,er )
c
      implicit none
c.. ingoing declarations
      real a(3),s(3),t(3),al(12),el(12,12),er(12,12)
      include 'smupcommons.h'
c
c.. locals
      integer i,j,k,row1,row2,col,ier
      real vm1(12),vm2(12),vm3(12)
      real an(3),sn(3),tn(3)
      real c1,c2,kap,rad,alpha
      real sum,eps,jac
      real mult1,mult2,mult3,mult4,mult5
c
      eps = 1.0e-10
c
      kap = lam+2.0*mu
      c1 = sqrt( kap/rho0 )
      c2 = sqrt( mu/rho0 )
      mult1 = 1.0/(2.0*c1)
      mult2 = 1.0/(2.0*c2)
      mult3 = 1.0/(2.0*kap)
      mult4 = 1.0/(2.0*mu)
      mult5 = 1.0/kap
c
c.. do orthonormalization of a,s,t into an,sn,tn
c     while we are he we set eigenvalues too
      ! normalize an
      rad    = sqrt(a(1)**2+a(2)**2+a(3)**2)
      an(1)  = a(1)/rad
      an(2)  = a(2)/rad
      an(3)  = a(3)/rad
      al(1)  = -rad*c1
      al(2)  = -rad*c2
      al(3)  = -rad*c2
      al(4)  = 0.0
      al(5)  = 0.0
      al(6)  = 0.0
      al(7)  = 0.0
      al(8)  = 0.0
      al(9)  = 0.0
      al(10) = rad*c2
      al(11) = rad*c2
      al(12) = rad*c1
c
      ! set sn to be part of s which is orthoogonal to an
      alpha = an(1)*s(1)+an(2)*s(2)+an(3)*s(3)
      sn(1) = s(1)-alpha*an(1)
      sn(2) = s(2)-alpha*an(2)
      sn(3) = s(3)-alpha*an(3)
      ! normalize sn
      rad   = sqrt(sn(1)**2+sn(2)**2+sn(3)**2)
      sn(1) = sn(1)/rad
      sn(2) = sn(2)/rad
      sn(3) = sn(3)/rad
c
      ! set tn to be part of t which is orthoogonal to an and sn
      alpha = an(1)*t(1)+an(2)*t(2)+an(3)*t(3)
      tn(1) = t(1)-alpha*an(1)
      tn(2) = t(2)-alpha*an(2)
      tn(3) = t(3)-alpha*an(3)
      alpha = sn(1)*tn(1)+sn(2)*tn(2)+sn(3)*tn(3)
      tn(1) = tn(1)-alpha*sn(1)
      tn(2) = tn(2)-alpha*sn(2)
      tn(3) = tn(3)-alpha*sn(3)
      ! normalize tn
      rad   = sqrt(tn(1)**2+tn(2)**2+tn(3)**2)
      tn(1) = tn(1)/rad
      tn(2) = tn(2)/rad
      tn(3) = tn(3)/rad
c
      ! The formulas I use assume that the Jacobian is positive .. make sure this is so
      jac = an(1)*(sn(2)*tn(3)-tn(2)*sn(3))+
     *      an(2)*(sn(3)*tn(1)-sn(1)*tn(3))+
     *      an(3)*(sn(1)*tn(2)-sn(2)*tn(1))
      if( jac.lt.0 ) then
        tn(1) = -tn(1)
        tn(2) = -tn(2)
        tn(3) = -tn(3)
      end if
c
c.. check to make sure these really are orthonormal 
c      if( .true. ) then
      if( .false. ) then
        jac = an(1)*(sn(2)*tn(3)-tn(2)*sn(3))+
     *     an(2)*(sn(3)*tn(1)-sn(1)*tn(3))+
     *     an(3)*(sn(1)*tn(2)-sn(2)*tn(1))
        if( jac.lt.0 ) then
          tn(1) = -tn(1)
          tn(2) = -tn(2)
          tn(3) = -tn(3)
        end if
        jac = an(1)*(sn(2)*tn(3)-tn(2)*sn(3))+
     *     an(2)*(sn(3)*tn(1)-sn(1)*tn(3))+
     *     an(3)*(sn(1)*tn(2)-sn(2)*tn(1))
        if( abs( 1.0-jac ).gt.eps ) then
          write(6,*)'error(smeig3d):orthonormal set not found!!',jac
          write(6,*)a(1),a(2),a(3)
          write(6,*)s(1),s(2),s(3)
          write(6,*)t(1),t(2),t(3)
          stop
        end if
      end if
c
c.. set up v vectors
      vm1(1)  = 0.0
      vm1(2)  = an(3)*c2
      vm1(3)  = -an(2)*c2
      vm1(4)  = 0.0
      vm1(5)  = an(1)*an(3)*mu
      vm1(6)  = -an(1)*an(2)*mu
      vm1(7)  = an(1)*an(3)*mu
      vm1(8)  = 2.0*an(2)*an(3)*mu
      vm1(9)  = mu*(an(3)**2-an(2)**2)
      vm1(10) = -an(1)*an(2)*mu
      vm1(11) = mu*(an(3)**2-an(2)**2)
      vm1(12) = -2.0*an(2)*an(3)*mu
c      
      vm2(1)  = -an(3)*c2
      vm2(2)  = 0.0
      vm2(3)  = an(1)*c2
      vm2(4)  = -2.0*an(1)*an(3)*mu
      vm2(5)  = -an(2)*an(3)*mu
      vm2(6)  = (an(1)**2-an(3)**2)*mu
      vm2(7)  = -an(2)*an(3)*mu
      vm2(8)  = 0.0
      vm2(9)  = an(1)*an(2)*mu
      vm2(10) = (an(1)**2-an(3)**2)*mu
      vm2(11) = an(1)*an(2)*mu
      vm2(12) = 2.0*an(1)*an(3)*mu
c
      vm3(1)  = an(2)*c2
      vm3(2)  = -an(1)*c2
      vm3(3)  = 0.0
      vm3(4)  = 2.0*an(1)*an(2)*mu
      vm3(5)  = -(an(1)**2-an(2)**2)*mu
      vm3(6)  = an(2)*an(3)*mu
      vm3(7)  = -(an(1)**2-an(2)**2)*mu
      vm3(8)  = -2.0*an(1)*an(2)*mu
      vm3(9)  = -an(1)*an(3)*mu
      vm3(10) = an(2)*an(3)*mu
      vm3(11) = -an(1)*an(3)*mu
      vm3(12) = 0.0
c
c.. left eigenvectors ...
      do i = 1,3
        el(1,i)  = mult1*an(i)
        el(2,i)  = mult2*tn(i)
        el(3,i)  = -mult2*sn(i)
        el(4,i)  = 0.0
        el(5,i)  = 0.0
        el(6,i)  = 0.0
        el(7,i)  = 0.0
        el(8,i)  = 0.0
        el(9,i)  = 0.0
        el(10,i) = -mult2*tn(i)
        el(11,i) = mult2*sn(i)
        el(12,i) = -mult1*an(i)
      end do
c
      do i = 1,3
        do j = 1,3
          col = 3*i+j
          el(1,col) = an(i)*an(j)*mult3
          el(2,col) = an(i)*tn(j)*mult4
          el(3,col) = -an(i)*sn(j)*mult4
          do k = 1,3
            el(3+k,col) = -mult5*(an(i)*an(j)*tn(k)*lam+
     *         an(i)*an(k)*tn(j)*kap)
            el(6+k,col) =  mult5*(an(i)*an(j)*sn(k)*lam+
     *         an(i)*an(k)*sn(j)*kap)
          end do
          el(10,col) = el(2,col)
          el(11,col) = el(3,col)
          el(12,col) = el(1,col)
        end do
      end do
c
      do i = 1,3
        row1 = i+3
        row2 = i+6
        do j = 1,3
          col = 3*j+i
          el(row1,col) = el(row1,3*j+i)+tn(j)
          el(row2,col) = el(row2,3*j+i)-sn(j)
        end do
      end do
c
c.. right eigenvectors ... some of this could go above, but this is clearer
      er(1,1)  = an(1)*c1
      er(2,1)  = an(2)*c1
      er(3,1)  = an(3)*c1
      er(4,1)  = 2.0*an(1)**2*mu+lam
      er(5,1)  = 2.0*an(1)*an(2)*mu
      er(6,1)  = 2.0*an(1)*an(3)*mu
      er(7,1)  = 2.0*an(1)*an(2)*mu
      er(8,1)  = 2.0*an(2)**2*mu+lam
      er(9,1)  = 2.0*an(2)*an(3)*mu
      er(10,1) = 2.0*an(1)*an(3)*mu
      er(11,1) = 2.0*an(2)*an(3)*mu
      er(12,1) = 2.0*an(3)**2*mu+lam
c
      do i = 1,12
        er(i,2)  = sn(1)*vm1(i)+sn(2)*vm2(i)+sn(3)*vm3(i)
        er(i,3)  = tn(1)*vm1(i)+tn(2)*vm2(i)+tn(3)*vm3(i)
        er(i,4) = 0.0
        er(i,5) = 0.0
        er(i,6) = 0.0
        er(i,7) = 0.0
        er(i,8) = 0.0
        er(i,9) = 0.0
      end do
c
      er(4,4)  = -sn(2)*an(3)+sn(3)*an(2)
      er(7,4)  =  sn(1)*an(3)-sn(3)*an(1)
      er(10,4) = -sn(1)*an(2)+sn(2)*an(1)
      er(5,5)  = er(4,4)
      er(8,5)  = er(7,4)
      er(11,5) = er(10,4)
      er(6,6)  = er(4,4)
      er(9,6)  = er(7,4)
      er(12,6) = er(10,4)
c
      er(4,7)  = -tn(2)*an(3)+tn(3)*an(2)
      er(7,7)  =  tn(1)*an(3)-tn(3)*an(1)
      er(10,7) = -tn(1)*an(2)+tn(2)*an(1)
      er(5,8)  = er(4,7)
      er(8,8)  = er(7,7)
      er(11,8) = er(10,7)
      er(6,9)  = er(4,7)
      er(9,9)  = er(7,7)
      er(12,9) = er(10,7)
c
      do i = 1,3
        er(i,10) = -er(i,2)
        er(i,11) = -er(i,3)
        er(i,12) = -er(i,1)
      end do
      do i = 4,12
        er(i,10) = er(i,2)
        er(i,11) = er(i,3)
        er(i,12) = er(i,1)
      end do
c
c.. probably want to check that these really are inverses for debugging purposes
c
c      if( .true. ) then
      if( .false. ) then
      ier = 0
      do i = 1,12
        do j = 1,12
          sum = 0.0
          do k = 1,12
            sum = sum+el(i,k)*er(k,j)
          end do
          if( i.eq.j ) sum = sum-1.0
          if( abs( sum ).gt.eps ) then
            write(6,*)'**',i,j,sum
            ier = 1
          end if
        end do
      end do
      if( ier.ne.0 ) then
        write(6,*)'error(smeig3d), metrics below!'
        write(6,*)an(1),an(2),an(3)
        write(6,*)sn(1),sn(2),sn(3)
        write(6,*)tn(1),tn(2),tn(3)
        pause
      end if
      end if
c
      return
      end
c
c+++++++++++++++++++++++
c
      subroutine smflux3d( an,aj,w,f )
c
      implicit none
c.. ingoing declarations
      real an(3),aj,w(12),f(12)
      include 'smupcommons.h'
c
c.. local declarations
      real a1,a2,a3,kap
c
      a1 = an(1)
      a2 = an(2)
      a3 = an(3)
c
      kap = lam+2.0*mu
c
      f(1)  = -aj*(a1*w(4)+a2*w(7)+a3*w(10))/rho0
      f(2)  = -aj*(a1*w(5)+a2*w(8)+a3*w(11))/rho0
      f(3)  = -aj*(a1*w(6)+a2*w(9)+a3*w(12))/rho0
      f(4)  = -aj*(a1*kap*w(1)+lam*(a2*w(2)+a3*w(3)))
      f(5)  = -aj*(mu*(a1*w(2)+a2*w(1)))
      f(6)  = -aj*(mu*(a1*w(3)+a3*w(1)))
      f(7)  = f(5)
      f(8)  = -aj*(lam*(a1*w(1)+a3*w(3))+kap*a2*w(2))
      f(9)  = -aj*(mu*(a2*w(3)+a3*w(2)))
      f(10) = f(6)
      f(11) = f(9)
      f(12) = -aj*(lam*(a1*w(1)+a2*w(2))+kap*a3*w(3))
c
      return
      end
c
c+++++++++++++++++++++++
c
      subroutine smflux3dfs( j,w,f )
c
      implicit none
c.. ingoing declarations
      real w(12),f(12)
      integer j
      include 'smupcommons.h'
c
c.. local declarations
      real kap
c
      kap = lam+2.0*mu
c
      if( j.eq.1 ) then
        f(1)  = -w(4)/rho0
        f(2)  = -w(5)/rho0
        f(3)  = -w(6)/rho0
        f(4)  = -kap*w(1)
        f(5)  = -mu*w(2)
        f(6)  = -mu*w(3)
        f(7)  = f(5)
        f(8)  = -lam*w(1)
        f(9)  = 0.0
        f(10) = f(6)
        f(11) = 0.0
        f(12) = -lam*w(1)
      elseif( j.eq.2 ) then
        f(1)  = -w(7)/rho0
        f(2)  = -w(8)/rho0
        f(3)  = -w(9)/rho0
        f(4)  = -lam*w(2)
        f(5)  = -mu*w(1)
        f(6)  = 0.0
        f(7)  = f(5)
        f(8)  = -kap*w(2)
        f(9)  = -mu*w(3)
        f(10) = 0.0
        f(11) = f(9)
        f(12) = -lam*w(2)
      else
        f(1)  = -w(10)/rho0
        f(2)  = -w(11)/rho0
        f(3)  = -w(12)/rho0
        f(4)  = -lam*w(3)
        f(5)  = 0.0
        f(6)  = -mu*w(1)
        f(7)  = 0.0
        f(8)  = -lam*w(3)
        f(9)  = -mu*w(2)
        f(10) = f(6)
        f(11) = f(9)
        f(12) = -kap*w(3)
      end if
c
      return
      end
c
c+++++++++++++++++++++++
c
      subroutine smupmetrics3d( nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *                          md1a,md1b,md2a,md2b,md3a,md3b,
     *                          i3, rx,det,a1,aj )
c
      implicit none 
c.. ingoing declarations
      integer nd1a,nd1b,nd2a,nd2b,nd3a,nd3b
      integer md1a,md1b,md2a,md2b,md3a,md3b
      integer i3
      real rx(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,3,3)
      real det(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real a1(3,3,md1a:md1b,md2a:md2b)
      real aj(md1a:md1b,md2a:md2b)
      include 'smupcommons.h'
c
c.. local declarations
      integer i,j,i1,i2
c
      if( icart.eq.0 ) then
c
c.. non-Cartesian
        do i2 = md2a,md2b
        do i1 = md1a,md1b
          do i = 1,3
          do j = 1,3
            a1(i,j,i1,i2) = rx(i1,i2,i3,i,j)
          end do
          end do
          aj(i1,i2)=det(i1,i2,i3)
        end do
        end do
c
      else
c
c.. Cartesian
        do i2 = md2a,md2b
        do i1 = md1a,md1b
          do i = 1,3
          do j = 1,3
            if( i.eq.j ) then
              a1(i,j,i1,i2) = 1.0
            else
              a1(i,j,i1,i2) = 0.0
            end if
          end do
          end do
          aj(i1,i2) = 1.0
        end do
        end do
c
      end if
c
      return
      end
c
c++++++++++++++++++++
c
      subroutine smuptz3d( m,x,y,z,t,h )
c
      implicit none
c.. ingoing declarations
      integer m
      real x,y,z,t,h(15)
      include 'smupcommons.h'
c
c.. local declarations
      integer k
      real ut(15),ux(12),uy(12),uz(12),u0(3)
      real kap
c
      kap = lam+2.0*mu
c
      do k = 1,15
        call ogDeriv( eptz,1,0,0,0,x,y,z,t,k-1,ut(k) )
      end do
      do k = 1,12
        call ogDeriv( eptz,0,1,0,0,x,y,z,t,k-1,ux(k) )
        call ogDeriv( eptz,0,0,1,0,x,y,z,t,k-1,uy(k) )
        call ogDeriv( eptz,0,0,0,1,x,y,z,t,k-1,uz(k) )
      end do
      do k = 1,3
        call ogDeriv( eptz,0,0,0,0,x,y,z,t,k-1,u0(k) )
      end do
c
      h(1)  = ut(1) -(ux(4)+uy(7)+uz(10))/rho0
      h(2)  = ut(2) -(ux(5)+uy(8)+uz(11))/rho0
      h(3)  = ut(3) -(ux(6)+uy(9)+uz(12))/rho0
      h(4)  = ut(4) -(kap*ux(1)+lam*(uy(2)+uz(3)))
      h(5)  = ut(5) -mu*(ux(2)+uy(1))
      h(6)  = ut(6) -mu*(ux(3)+uz(1))
      h(7)  = ut(7) -mu*(ux(2)+uy(1))
      h(8)  = ut(8) -(lam*(ux(1)+uz(3))+kap*uy(2))
      h(9)  = ut(9) -mu*(uy(3)+uz(2))
      h(10) = ut(10)-mu*(ux(3)+uz(1))
      h(11) = ut(11)-mu*(uy(3)+uz(2))
      h(12) = ut(12)-(lam*(ux(1)+uy(2))+kap*uz(3))
      h(13) = ut(13)-u0(1)
      h(14) = ut(14)-u0(2)
      h(15) = ut(15)-u0(3)
c
      return
      end
c
c++++++++++++++++++++
c
      subroutine stressRelax3d( m,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *                            n1a, n1b, n2a, n2b, n3a, n3b, 
     *                            ds1,ds2,ds3,dt,t,xy,rx,u,up,mask,
     *                            iparam,rparam )
c
      implicit none
c.. ingoing declarations
      integer nd1a,nd1b,n1a,n1b
      integer nd2a,nd2b,n2a,n2b
      integer nd3a,nd3b,n3a,n3b
      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer m
      integer iRelax
      integer iparam(*)
      real ds1,ds2,ds3,dt,t
      real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,3)
      real rx(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,3,3)
      real det(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,m)
      real up(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,m)
      real rparam(*)
c
c.. locals
      integer i1,i2,i3
      integer iu1c,iu2c,iu3c
      integer is11c,is12c,is13c
      integer is21c,is22c,is23c
      integer is31c,is32c,is33c
      real u1x,u2x,u3x,u1y,u2y,u3y,u1z,u2z,u3z
      real u1r,u2r,u3r,u1s,u2s,u3s,u1t,u2t,u3t
      real u1xt,u2xt,u3xt,u1yt,u2yt,u3yt,u1zt,u2zt,u3zt
      real s11,s12,s13
      real s21,s22,s23
      real s31,s32,s33
      real s11t,s12t,s13t
      real s21t,s22t,s23t
      real s31t,s32t,s33t
      real relaxAlpha,relaxDelta,kappa,beta
      real x,y,z
      include 'smupcommons.h'
c
c      mu         = rparam(3)
c      lam        = rparam(4)
      relaxAlpha = rparam(7)
      relaxDelta = rparam(8)
c      eptz       = rparam(6)

c      icart      = iparam(2)
c      itz        = iparam(3)
      iRelax     = iparam(9)
c
      beta = relaxAlpha+relaxDelta/dt
      kappa = lam+2.0*mu
      if( icart.eq.1 ) then
        do i3 = n3a,n3b
        do i2 = n2a,n2b
        do i1 = n1a,n1b
          if( mask(i1,i2,i3).ne.0 ) then
            if( iRelax.eq.2 ) then
              u1x = (u(i1+1,i2,i3,13)-u(i1-1,i2,i3,13))/(2.d0*ds1)
              u2x = (u(i1+1,i2,i3,14)-u(i1-1,i2,i3,14))/(2.d0*ds1)
              u3x = (u(i1+1,i2,i3,15)-u(i1-1,i2,i3,15))/(2.d0*ds1)
              u1y = (u(i1,i2+1,i3,13)-u(i1,i2-1,i3,13))/(2.d0*ds2)
              u2y = (u(i1,i2+1,i3,14)-u(i1,i2-1,i3,14))/(2.d0*ds2)
              u3y = (u(i1,i2+1,i3,15)-u(i1,i2-1,i3,15))/(2.d0*ds2)
              u1z = (u(i1,i2,i3+1,13)-u(i1,i2,i3-1,13))/(2.d0*ds3)
              u2z = (u(i1,i2,i3+1,14)-u(i1,i2,i3-1,14))/(2.d0*ds3)
              u3z = (u(i1,i2,i3+1,15)-u(i1,i2,i3-1,15))/(2.d0*ds3)
            else
              u1x = (-u(i1+2,i2,i3,13)+8*u(i1+1,i2,i3,13)-
     *           8*u(i1-1,i2,i3,13)+u(i1-2,i2,i3,13))/(12.d0*ds1)
              u2x = (-u(i1+2,i2,i3,14)+8*u(i1+1,i2,i3,14)-
     *           8*u(i1-1,i2,i3,14)+u(i1-2,i2,i3,14))/(12.d0*ds1)
              u3x = (-u(i1+2,i2,i3,15)+8*u(i1+1,i2,i3,15)-
     *           8*u(i1-1,i2,i3,15)+u(i1-2,i2,i3,15))/(12.d0*ds1)
              u1y = (-u(i1,i2+2,i3,13)+8*u(i1,i2+1,i3,13)-
     *           8*u(i1,i2-1,i3,13)+u(i1,i2-2,i3,13))/(12.d0*ds2)
              u2y = (-u(i1,i2+2,i3,14)+8*u(i1,i2+1,i3,14)-
     *           8*u(i1,i2-1,i3,14)+u(i1,i2-2,i3,14))/(12.d0*ds2)
              u3y = (-u(i1,i2+2,i3,15)+8*u(i1,i2+1,i3,15)-
     *           8*u(i1,i2-1,i3,15)+u(i1,i2-2,i3,15))/(12.d0*ds2)
              u1z = (-u(i1,i2,i3+2,13)+8*u(i1,i2,i3+1,13)-
     *           8*u(i1,i2,i3-1,13)+u(i1,i2,i3-2,13))/(12.d0*ds3)
              u2z = (-u(i1,i2,i3+2,14)+8*u(i1,i2,i3+1,14)-
     *           8*u(i1,i2,i3-1,14)+u(i1,i2,i3-2,14))/(12.d0*ds3)
              u3z = (-u(i1,i2,i3+2,15)+8*u(i1,i2,i3+1,15)-
     *           8*u(i1,i2,i3-1,15)+u(i1,i2,i3-2,15))/(12.d0*ds3)
            end if
            
            s11 = kappa*u1x+lam*(u2y+u3z)
            s12 = mu*(u2x+u1y)
            s13 = mu*(u3x+u1z)
            s21 = s12
            s22 = kappa*u2y+lam*(u1x+u3z)
            s23 = mu*(u3y+u2z)
            s31 = s13
            s32 = s23
            s33 = kappa*u3z+lam*(u1x+u2y)
            
            up(i1,i2,i3,4) = up(i1,i2,i3,4)+
     *         beta*(-u(i1,i2,i3,4)+s11)
            up(i1,i2,i3,5) = up(i1,i2,i3,5)+
     *         beta*(-u(i1,i2,i3,5)+s12)
            up(i1,i2,i3,6) = up(i1,i2,i3,6)+
     *         beta*(-u(i1,i2,i3,6)+s13)
            
            up(i1,i2,i3,7) = up(i1,i2,i3,7)+
     *         beta*(-u(i1,i2,i3,7)+s21)
            up(i1,i2,i3,8) = up(i1,i2,i3,8)+
     *         beta*(-u(i1,i2,i3,8)+s22)
            up(i1,i2,i3,9) = up(i1,i2,i3,9)+
     *         beta*(-u(i1,i2,i3,9)+s23)
            
            up(i1,i2,i3,10) = up(i1,i2,i3,10)+
     *         beta*(-u(i1,i2,i3,10)+s31)
            up(i1,i2,i3,11) = up(i1,i2,i3,11)+
     *         beta*(-u(i1,i2,i3,11)+s32)
            up(i1,i2,i3,12) = up(i1,i2,i3,12)+
     *         beta*(-u(i1,i2,i3,12)+s33)
          end if
        end do
        end do
        end do
      else
        do i3 = n3a,n3b
        do i2 = n2a,n2b
        do i1 = n1a,n1b
          if( mask(i1,i2,i3).ne.0 ) then
            if( iRelax.eq.2 ) then
              u1r = (u(i1+1,i2,i3,13)-u(i1-1,i2,i3,13))/(2.d0*ds1)
              u2r = (u(i1+1,i2,i3,14)-u(i1-1,i2,i3,14))/(2.d0*ds1)
              u3r = (u(i1+1,i2,i3,15)-u(i1-1,i2,i3,15))/(2.d0*ds1)
              u1s = (u(i1,i2+1,i3,13)-u(i1,i2-1,i3,13))/(2.d0*ds2)
              u2s = (u(i1,i2+1,i3,14)-u(i1,i2-1,i3,14))/(2.d0*ds2)
              u3s = (u(i1,i2+1,i3,15)-u(i1,i2-1,i3,15))/(2.d0*ds2)
              u1t = (u(i1,i2,i3+1,13)-u(i1,i2,i3-1,13))/(2.d0*ds3)
              u2t = (u(i1,i2,i3+1,14)-u(i1,i2,i3-1,14))/(2.d0*ds3)
              u3t = (u(i1,i2,i3+1,15)-u(i1,i2,i3-1,15))/(2.d0*ds3)
            else
              u1r = (-u(i1+2,i2,i3,13)+8*u(i1+1,i2,i3,13)-
     *           8*u(i1-1,i2,i3,13)+u(i1-2,i2,i3,13))/(12.d0*ds1)
              u2r = (-u(i1+2,i2,i3,14)+8*u(i1+1,i2,i3,14)-
     *           8*u(i1-1,i2,i3,14)+u(i1-2,i2,i3,14))/(12.d0*ds1)
              u3r = (-u(i1+2,i2,i3,15)+8*u(i1+1,i2,i3,15)-
     *           8*u(i1-1,i2,i3,15)+u(i1-2,i2,i3,15))/(12.d0*ds1)
              u1s = (-u(i1,i2+2,i3,13)+8*u(i1,i2+1,i3,13)-
     *           8*u(i1,i2-1,i3,13)+u(i1,i2-2,i3,13))/(12.d0*ds2)
              u2s = (-u(i1,i2+2,i3,14)+8*u(i1,i2+1,i3,14)-
     *           8*u(i1,i2-1,i3,14)+u(i1,i2-2,i3,14))/(12.d0*ds2)
              u3s = (-u(i1,i2+2,i3,15)+8*u(i1,i2+1,i3,15)-
     *           8*u(i1,i2-1,i3,15)+u(i1,i2-2,i3,15))/(12.d0*ds2)
              u1t = (-u(i1,i2,i3+2,13)+8*u(i1,i2,i3+1,13)-
     *           8*u(i1,i2,i3-1,13)+u(i1,i2,i3-2,13))/(12.d0*ds3)
              u2t = (-u(i1,i2,i3+2,14)+8*u(i1,i2,i3+1,14)-
     *           8*u(i1,i2,i3-1,14)+u(i1,i2,i3-2,14))/(12.d0*ds3)
              u3t = (-u(i1,i2,i3+2,15)+8*u(i1,i2,i3+1,15)-
     *           8*u(i1,i2,i3-1,15)+u(i1,i2,i3-2,15))/(12.d0*ds3)
            end if
            
            u1x = u1r*rx(i1,i2,i3,1,1)+u1s*rx(i1,i2,i3,2,1)+
     *         u1t*rx(i1,i2,i3,3,1)
            u2x = u2r*rx(i1,i2,i3,1,1)+u2s*rx(i1,i2,i3,2,1)+
     *         u2t*rx(i1,i2,i3,3,1)
            u3x = u3r*rx(i1,i2,i3,1,1)+u3s*rx(i1,i2,i3,2,1)+
     *         u3t*rx(i1,i2,i3,3,1)
            
            u1y = u1r*rx(i1,i2,i3,1,2)+u1s*rx(i1,i2,i3,2,2)+
     *         u1t*rx(i1,i2,i3,3,2)
            u2y = u2r*rx(i1,i2,i3,1,2)+u2s*rx(i1,i2,i3,2,2)+
     *         u2t*rx(i1,i2,i3,3,2)
            u3y = u3r*rx(i1,i2,i3,1,2)+u3s*rx(i1,i2,i3,2,2)+
     *         u3t*rx(i1,i2,i3,3,2)
            
            u1z = u1r*rx(i1,i2,i3,1,3)+u1s*rx(i1,i2,i3,2,3)+
     *         u1t*rx(i1,i2,i3,3,3)
            u2z = u2r*rx(i1,i2,i3,1,3)+u2s*rx(i1,i2,i3,2,3)+
     *         u2t*rx(i1,i2,i3,3,3)
            u3z = u3r*rx(i1,i2,i3,1,3)+u3s*rx(i1,i2,i3,2,3)+
     *         u3t*rx(i1,i2,i3,3,3)

            s11 = kappa*u1x+lam*(u2y+u3z)
            s12 = mu*(u2x+u1y)
            s13 = mu*(u3x+u1z)
            s21 = s12
            s22 = kappa*u2y+lam*(u1x+u3z)
            s23 = mu*(u3y+u2z)
            s31 = s13
            s32 = s23
            s33 = kappa*u3z+lam*(u1x+u2y)
            
            up(i1,i2,i3,4) = up(i1,i2,i3,4)+
     *         beta*(-u(i1,i2,i3,4)+s11)
            up(i1,i2,i3,5) = up(i1,i2,i3,5)+
     *         beta*(-u(i1,i2,i3,5)+s12)
            up(i1,i2,i3,6) = up(i1,i2,i3,6)+
     *         beta*(-u(i1,i2,i3,6)+s13)
            
            up(i1,i2,i3,7) = up(i1,i2,i3,7)+
     *         beta*(-u(i1,i2,i3,7)+s21)
            up(i1,i2,i3,8) = up(i1,i2,i3,8)+
     *         beta*(-u(i1,i2,i3,8)+s22)
            up(i1,i2,i3,9) = up(i1,i2,i3,9)+
     *         beta*(-u(i1,i2,i3,9)+s23)
            
            up(i1,i2,i3,10) = up(i1,i2,i3,10)+
     *         beta*(-u(i1,i2,i3,10)+s31)
            up(i1,i2,i3,11) = up(i1,i2,i3,11)+
     *         beta*(-u(i1,i2,i3,11)+s32)
            up(i1,i2,i3,12) = up(i1,i2,i3,12)+
     *         beta*(-u(i1,i2,i3,12)+s33)              
          end if
        end do
        end do
        end do
      end if
      ! add twilight zone contribution
      if( itz.ne.0 ) then
        iu1c = 12
        iu2c = 13
        iu3c = 14
        
        is11c = 3
        is12c = 4
        is13c = 5
        
        is21c = 6
        is22c = 7
        is23c = 8
        
        is31c = 9
        is32c = 10
        is33c = 11
        
        do i3 = n3a,n3b
        do i2 = n2a,n2b
        do i1 = n1a,n1b
          if( mask(i1,i2,i3).ne.0 ) then
            x = xy(i1,i2,i3,1)
            y = xy(i1,i2,i3,2)
            z = xy(i1,i2,i3,3)
            
            call ogDeriv( eptz,0,1,0,0,x,y,z,t,iu1c,u1xt )
            call ogDeriv( eptz,0,1,0,0,x,y,z,t,iu2c,u2xt )
            call ogDeriv( eptz,0,1,0,0,x,y,z,t,iu3c,u3xt )
            
            call ogDeriv( eptz,0,0,1,0,x,y,z,t,iu1c,u1yt )
            call ogDeriv( eptz,0,0,1,0,x,y,z,t,iu2c,u2yt )
            call ogDeriv( eptz,0,0,1,0,x,y,z,t,iu3c,u3yt )
            
            call ogDeriv( eptz,0,0,0,1,x,y,z,t,iu1c,u1zt )
            call ogDeriv( eptz,0,0,0,1,x,y,z,t,iu2c,u2zt )
            call ogDeriv( eptz,0,0,0,1,x,y,z,t,iu3c,u3zt )
            
            call ogDeriv( eptz,0,0,0,0,x,y,z,t,is11c,s11t )
            call ogDeriv( eptz,0,0,0,0,x,y,z,t,is12c,s12t )
            call ogDeriv( eptz,0,0,0,0,x,y,z,t,is13c,s13t )
            
            call ogDeriv( eptz,0,0,0,0,x,y,z,t,is21c,s21t )
            call ogDeriv( eptz,0,0,0,0,x,y,z,t,is22c,s22t )
            call ogDeriv( eptz,0,0,0,0,x,y,z,t,is23c,s23t )
            
            call ogDeriv( eptz,0,0,0,0,x,y,z,t,is31c,s31t )
            call ogDeriv( eptz,0,0,0,0,x,y,z,t,is32c,s32t )
            call ogDeriv( eptz,0,0,0,0,x,y,z,t,is33c,s33t )
            
            s11 = kappa*u1xt+lam*(u2yt+u3zt)
            s12 = mu*(u2xt+u1yt)
            s13 = mu*(u3xt+u1zt)
            s21 = s12
            s22 = kappa*u2yt+lam*(u1xt+u3zt)
            s23 = mu*(u3yt+u2zt)
            s31 = s13
            s32 = s23
            s33 = kappa*u3zt+lam*(u1xt+u2yt)
            
            up(i1,i2,i3,4) = up(i1,i2,i3,4)-beta*(-s11t+s11)
            up(i1,i2,i3,5) = up(i1,i2,i3,5)-beta*(-s12t+s12)
            up(i1,i2,i3,6) = up(i1,i2,i3,6)-beta*(-s13t+s13)
            
            up(i1,i2,i3,7) = up(i1,i2,i3,7)-beta*(-s21t+s21)
            up(i1,i2,i3,8) = up(i1,i2,i3,8)-beta*(-s22t+s22)
            up(i1,i2,i3,9) = up(i1,i2,i3,9)-beta*(-s23t+s23)
            
            up(i1,i2,i3,10) = up(i1,i2,i3,10)-beta*(-s31t+s31)
            up(i1,i2,i3,11) = up(i1,i2,i3,11)-beta*(-s32t+s32)
            up(i1,i2,i3,12) = up(i1,i2,i3,12)-beta*(-s33t+s33)
          end if
        end do
        end do
        end do
      end if
c
      return 
      end
c
c++++++++++++++++++++
c
