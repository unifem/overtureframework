      subroutine smg3dNL( m,nd1a,nd1b,n1a,n1b,
     *                    nd2a,nd2b,n2a,n2b,
     *                    nd3a,nd3b,n3a,n3b,
     *                    ds1,ds2,ds3,dt,t,xy,rx,det,
     *                    u,up,f1,f2,mask,ad,
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
      real f1(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,3)
      real f2(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,3)
      real rparam(nrparam)
      real rwrk(nrwrk)
c
c.. locals
      integer i1,i2,i3,k
      integer md1a,md1b,md2a,md2b,md3a,md3b
      integer ngrid,lw,la1,laj,ldpdf,nreq
      real tol,almax(3),const,diff
      real ad(15),adk,admax,tiny
      integer iRelax
      real tsdiss,diseig
      include 'smupNLcommons.h'
c
      idebug = iparam(10)
c
      if( idebug.gt.0 )then
        write(6,*)'*** Entering smg3dNL ***'
      end if
c
c.. declarations for these are in smupNLcommons
      mu     = rparam(3)
      lambda = rparam(4)
      rho0   = rparam(5)
      eptz   = rparam(6)
      tsdiss = rparam(9)
 
      iorder = iparam(1)
      icart  = iparam(2)
      itz    = iparam(3)
      ilimit = iparam(5)
      itype  = iparam(7)
      ifrc   = iparam(8)
      iRelax = iparam(9)

      ifrc = 0
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
        write(6,*)'Error (smg3dNL) : m=15 is assumed'
        stop
      end if
c
c.. split up real workspace
      ngrid  = (md2b-md2a+1)*(md1b-md1a+1)
      lw     = 1
      la1    = lw+m*7*ngrid*2
      laj    = la1+9*2*ngrid*2
      ldpdf  = laj+ngrid*2
      nreq   = ldpdf+81*2*ngrid-1
      if( nreq.gt.nrwrk ) then
        ier = 4
        return
      end if
c
c.. filter out underflow
      tol = 1.0e-30
      do k = 1,m
        do i3 = md3a,md3b
        do i2 = md2a,md2b
        do i1 = md1a,md1b
          if( abs( u(i1,i2,i3,k) ).lt.tol ) u(i1,i2,i3,k) = 0.0
        end do
        end do
        end do
      end do
c
c.. make call to compute up
      call smup3dNL( m,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *               n1a, n1b, n2a, n2b, n3a, n3b,
     *               md1a,md1b,md2a,md2b,md3a,md3b,
     *               ds1,ds2,ds3,dt,t,xy,rx,det,u,up,
     *               f1,f2,mask,almax,rwrk(lw),rwrk(la1),
     *               rwrk(laj),rwrk(ldpdf),ier )
c
c..need to add dissipation for zero eigenvalues ... FIX ME!!
c
c.. 2nd and 4th-order dissipation for components of stress on surfaces whose normal is
c     tangent to cell faces. Recall that almax has an estimate for max wave speed.
      if( tsdiss.gt.1.d-12 ) then
        if( iorder.eq.1 ) then
          call stressDiss3d2( 
     *       m,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *       n1a,n1b,n2a,n2b,n3a,n3b,
     *       ds1,ds2,ds3,tsdiss,rx,u,up,mask,diseig )
        else
          call stressDiss3d4 (
     *       m,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *       n1a,n1b,n2a,n2b,n3a,n3b,
     *       ds1,ds2,ds3,tsdiss,rx,u,up,mask,diseig )
        end if
      end if
c
c add relaxation term to ensure compatibility of stress and position
      if( iRelax.ne.0 ) then
        call stressRelax3dNL( m,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *                        n1a, n1b, n2a, n2b, n3a, n3b, 
     *                        ds1,ds2,ds3,dt,t,xy,rx,u,up,mask,
     *                        iparam,rparam )
      end if
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
c       write(6,*)almax(1),almax(2),almax(3)
c.. compute real and imaginary parts of time stepping eigenvalues
      rparam(1) = 6.0*admax+diseig
      rparam(2) = almax(1)/ds1+almax(2)/ds2+almax(3)/ds3
      rparam(3)=0. ! for 1/dt dissipation

c
      if( idebug.gt.0 )then
        write(6,*)'*** Leaving smg3dNL ***'
      end if      
c
      return
      end
c
c+++++++++++++++++++++++
c
      subroutine smup3dNL( m,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *                     n1a, n1b, n2a, n2b, n3a, n3b,
     *                     md1a,md1b,md2a,md2b,md3a,md3b,
     *                     ds1,ds2,ds3,dt,t,xy,rx,det,u,up,
     *                     f1,f2,mask,almax,w,a1,aj,dpdf,ier )
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
      real f1(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,3)
      real f2(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,3)
      real almax(3)
      real w(m,md1a:md1b,md2a:md2b,7,2)
      real a1(3,3,md1a:md1b,md2a:md2b,2)
      real aj(md1a:md1b,md2a:md2b,2)
      real dpdf(3,3,3,3,md1a:md1b,md2a:md2b,2)
      include 'smupNLcommons.h'
c
c.. local declarations
      integer i,j,k,l,i1,i2,i3,i3m1
      integer i1p1,i2p1
      integer s1m,s1p,s2m,s2p,s3m,s3p,scent
      integer lay,laym1
      real met0(3,3),aj0
      real metl(3,3),metr(3,3)
      real fx(12),htz(15),t1
      real fxl(15),fxr(15)
      real d1p,d1m,d2p,d2m,d3p,d3m,da
      real ut(15),ux(12),uy(12),uz(12),u0(3),kap,x,y,z
      real ul(12),ur(12),c1,c2,cf1,cf2,cf3,cf4,df1,df2,df3
      real dpdfl(3,3,3,3),dpdfr(3,3,3,3)
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
c.. add external forcing if needed
      if( ifrc.ne.0 ) then
        do k = 1,2
          do i3 = n3a,n3b
          do i2 = n2a,n2b
          do i1 = n1a,n1b
            up(i1,i2,i3,k) = 0.5*(f1(i1,i2,i3,k)+f2(i1,i2,i3,k))
            write(6,*)f1(i1,i2,i3,k),f2(i1,i2,i3,k)
          end do
          end do
          end do
        end do
      end if
c
      i3 = n3a-1
      lay = 1 
c.. the "lay" variable indicates which layer we are on within w, a1, aj, and dpdf
c     so that we don't have to do the costly copies to update the layer.
c
c.. set grid metrics and grid velocities
      call smupmetrics3dNL( nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *                      md1a,md1b,md2a,md2b,md3a,md3b,
     *                      i3,rx,det,a1(1,1,md1a,md2a,lay),
     *                      aj(md1a,md2a,lay) )
      if( icart.eq.1 ) then
        ! if we are doing Cartesian then fill in all layers at once so we
        ! don't have to do it later
        call smupmetrics3dNL( nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *                        md1a,md1b,md2a,md2b,md3a,md3b,
     *                        i3,rx,det,a1(1,1,md1a,md2a,2),
     *                        aj(md1a,md2a,2) )
      end if
c
c.. set first layer of "slope corrected" values
      call smupslope3dNL( m,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *                    md1a,md1b,md2a,md2b,md3a,md3b,
     *                    n1a, n1b, n2a, n2b, n3a, n3b,
     *                    i3,ds1,ds2,ds3,dt,t,xy,
     *                    a1(1,1,md1a,md2a,lay),
     *                    aj(nd1a,md2a,lay), 
     *                    mask(nd1a,nd2a,i3),u,
     *                    w(1,md1a,md2a,1,lay),
     *                    dpdf(1,1,1,1,md1a,md2a,lay),f1 )
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
          call smupmetrics3dNL( nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *                          md1a,md1b,md2a,md2b,md3a,md3b,
     *                          i3,rx,det,a1(1,1,md1a,md2a,lay),
     *                          aj(md1a,md2a,lay) )
        end if
c
c.. slope correction for top layer of cells
        call smupslope3dNL( m,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *                      md1a,md1b,md2a,md2b,md3a,md3b,
     *                      n1a, n1b, n2a, n2b, n3a, n3b,
     *                      i3,ds1,ds2,ds3,dt,t,xy,
     *                      a1(1,1,md1a,md2a,lay),
     *                      aj(nd1a,md2a,lay), 
     *                      mask(nd1a,nd2a,i3),u,
     *                      w(1,md1a,md2a,1,lay),
     *                      dpdf(1,1,1,1,md1a,md2a,lay),f1 )
c
c..compute s3 flux and add it to up
        do i2 = n2a,n2b
        do i1 = n1a,n1b
          if( mask(i1,i2,i3).ne.0 .and. mask(i1,i2,i3m1).ne.0 ) then
            ! average metrics
            !aj0 = 0.5*(aj(i1,i2,lay)+aj(i1,i2,laym1))
c            met0(1,1) = 0.5*(a1(3,1,i1,i2,lay)+a1(3,1,i1,i2,laym1))
c            met0(1,2) = 0.5*(a1(3,2,i1,i2,lay)+a1(3,2,i1,i2,laym1))
c            met0(1,3) = 0.5*(a1(3,3,i1,i2,lay)+a1(3,3,i1,i2,laym1))
c            met0(2,1) = 0.5*(a1(1,1,i1,i2,lay)+a1(1,1,i1,i2,laym1))
c            met0(2,2) = 0.5*(a1(1,2,i1,i2,lay)+a1(1,2,i1,i2,laym1))
c            met0(2,3) = 0.5*(a1(1,3,i1,i2,lay)+a1(1,3,i1,i2,laym1))
c            met0(3,1) = 0.5*(a1(2,1,i1,i2,lay)+a1(2,1,i1,i2,laym1))
c            met0(3,2) = 0.5*(a1(2,2,i1,i2,lay)+a1(2,2,i1,i2,laym1))
c            met0(3,3) = 0.5*(a1(2,3,i1,i2,lay)+a1(2,3,i1,i2,laym1))

            metl(1,1) = a1(3,1,i1,i2,laym1)
            metl(1,2) = a1(3,2,i1,i2,laym1)
            metl(1,3) = a1(3,3,i1,i2,laym1)
            metl(2,1) = a1(1,1,i1,i2,laym1)
            metl(2,2) = a1(1,2,i1,i2,laym1)
            metl(2,3) = a1(1,3,i1,i2,laym1)
            metl(3,1) = a1(2,1,i1,i2,laym1)
            metl(3,2) = a1(2,2,i1,i2,laym1)
            metl(3,3) = a1(2,3,i1,i2,laym1)

            metr(1,1) = a1(3,1,i1,i2,lay)
            metr(1,2) = a1(3,2,i1,i2,lay)
            metr(1,3) = a1(3,3,i1,i2,lay)
            metr(2,1) = a1(1,1,i1,i2,lay)
            metr(2,2) = a1(1,2,i1,i2,lay)
            metr(2,3) = a1(1,3,i1,i2,lay)
            metr(3,1) = a1(2,1,i1,i2,lay)
            metr(3,2) = a1(2,2,i1,i2,lay)
            metr(3,3) = a1(2,3,i1,i2,lay)

            ! get fluxes
            call smupflux3dNL( 
     *         m,aj(i1,i2,laym1),aj(i1,i2,lay),metl,metr,
     *         dpdf(1,1,1,1,i1,i2,laym1),dpdf(1,1,1,1,i1,i2,lay),
     *         w(1,i1,i2,s3p,laym1),w(1,i1,i2,s3m,lay),
     *         fxl,fxr,almax(3) )

            do k = 1,12
              up(i1,i2,i3,k) = up(i1,i2,i3,k)+fxr(k)/ds3
              up(i1,i2,i3m1,k) = up(i1,i2,i3m1,k)-fxl(k)/ds3
            end do
          end if
        end do
        end do
c
c.. if i3.le.n3b, then compute fluxes in the s1 and s2 directions
        if( i3.le.n3b ) then
c
c.. compute s1 and s2 fluxes
c
c.. compute s1 fluxes 
          do i2 = n2a,n2b
          do i1 = n1a-1,n1b
            i1p1 = i1+1
            if( mask(i1,i2,i3).ne.0.and.mask(i1p1,i2,i3).ne.0 ) then
              !aj0 = 0.5*(aj(i1p1,i2,lay)+aj(i1,i2,lay))
c              met0(1,1) = 0.5*(a1(1,1,i1p1,i2,lay)+a1(1,1,i1,i2,lay))
c              met0(1,2) = 0.5*(a1(1,2,i1p1,i2,lay)+a1(1,2,i1,i2,lay))
c              met0(1,3) = 0.5*(a1(1,3,i1p1,i2,lay)+a1(1,3,i1,i2,lay))
c              met0(2,1) = 0.5*(a1(2,1,i1p1,i2,lay)+a1(2,1,i1,i2,lay))
c              met0(2,2) = 0.5*(a1(2,2,i1p1,i2,lay)+a1(2,2,i1,i2,lay))
c              met0(2,3) = 0.5*(a1(2,3,i1p1,i2,lay)+a1(2,3,i1,i2,lay))
c              met0(3,1) = 0.5*(a1(3,1,i1p1,i2,lay)+a1(3,1,i1,i2,lay))
c              met0(3,2) = 0.5*(a1(3,2,i1p1,i2,lay)+a1(3,2,i1,i2,lay))
c              met0(3,3) = 0.5*(a1(3,3,i1p1,i2,lay)+a1(3,3,i1,i2,lay))

              metl(1,1) = a1(1,1,i1,i2,lay)
              metl(1,2) = a1(1,2,i1,i2,lay)
              metl(1,3) = a1(1,3,i1,i2,lay)
              metl(2,1) = a1(2,1,i1,i2,lay)
              metl(2,2) = a1(2,2,i1,i2,lay)
              metl(2,3) = a1(2,3,i1,i2,lay)
              metl(3,1) = a1(3,1,i1,i2,lay)
              metl(3,2) = a1(3,2,i1,i2,lay)
              metl(3,3) = a1(3,3,i1,i2,lay)
                                      
              metr(1,1) = a1(1,1,i1p1,i2,lay)
              metr(1,2) = a1(1,2,i1p1,i2,lay)
              metr(1,3) = a1(1,3,i1p1,i2,lay)
              metr(2,1) = a1(2,1,i1p1,i2,lay)
              metr(2,2) = a1(2,2,i1p1,i2,lay)
              metr(2,3) = a1(2,3,i1p1,i2,lay)
              metr(3,1) = a1(3,1,i1p1,i2,lay)
              metr(3,2) = a1(3,2,i1p1,i2,lay)
              metr(3,3) = a1(3,3,i1p1,i2,lay)

              ! get fluxes
              call smupflux3dNL( 
     *           m,aj(i1,i2,lay),aj(i1p1,i2,lay),metl,metr,
     *           dpdf(1,1,1,1,i1,i2,lay),dpdf(1,1,1,1,i1p1,i2,lay),
     *           w(1,i1,i2,s1p,lay),w(1,i1p1,i2,s1m,lay),
     *           fxl,fxr,almax(1) )
              do k = 1,12
                up(i1p1,i2,i3,k) = up(i1p1,i2,i3,k)+fxr(k)/ds1
                up(i1,i2,i3,k) = up(i1,i2,i3,k)-fxl(k)/ds1
              end do
            end if
          end do
          end do
c
c.. compute s2 fluxes 
          do i2 = n2a-1,n2b
          do i1 = n1a,n1b
            i2p1 = i2+1
            if( mask(i1,i2,i3).ne.0.and.mask(i1,i2p1,i3).ne.0 ) then
              !aj0 = 0.5*(aj(i1,i2p1,lay)+aj(i1,i2,lay))
c              met0(1,1) = 0.5*(a1(2,1,i1,i2p1,lay)+a1(2,1,i1,i2,lay))
c              met0(1,2) = 0.5*(a1(2,2,i1,i2p1,lay)+a1(2,2,i1,i2,lay))
c              met0(1,3) = 0.5*(a1(2,3,i1,i2p1,lay)+a1(2,3,i1,i2,lay))
c              met0(2,1) = 0.5*(a1(3,1,i1,i2p1,lay)+a1(3,1,i1,i2,lay))
c              met0(2,2) = 0.5*(a1(3,2,i1,i2p1,lay)+a1(3,2,i1,i2,lay))
c              met0(2,3) = 0.5*(a1(3,3,i1,i2p1,lay)+a1(3,3,i1,i2,lay))
c              met0(3,1) = 0.5*(a1(1,1,i1,i2p1,lay)+a1(1,1,i1,i2,lay))
c              met0(3,2) = 0.5*(a1(1,2,i1,i2p1,lay)+a1(1,2,i1,i2,lay))
c              met0(3,3) = 0.5*(a1(1,3,i1,i2p1,lay)+a1(1,3,i1,i2,lay))

              metl(1,1) = a1(2,1,i1,i2,lay)
              metl(1,2) = a1(2,2,i1,i2,lay)
              metl(1,3) = a1(2,3,i1,i2,lay)
              metl(2,1) = a1(3,1,i1,i2,lay)
              metl(2,2) = a1(3,2,i1,i2,lay)
              metl(2,3) = a1(3,3,i1,i2,lay)
              metl(3,1) = a1(1,1,i1,i2,lay)
              metl(3,2) = a1(1,2,i1,i2,lay)
              metl(3,3) = a1(1,3,i1,i2,lay)
                          
              metr(1,1) = a1(2,1,i1,i2p1,lay)
              metr(1,2) = a1(2,2,i1,i2p1,lay)
              metr(1,3) = a1(2,3,i1,i2p1,lay)
              metr(2,1) = a1(3,1,i1,i2p1,lay)
              metr(2,2) = a1(3,2,i1,i2p1,lay)
              metr(2,3) = a1(3,3,i1,i2p1,lay)
              metr(3,1) = a1(1,1,i1,i2p1,lay)
              metr(3,2) = a1(1,2,i1,i2p1,lay)
              metr(3,3) = a1(1,3,i1,i2p1,lay)

              ! get fluxes
              call smupflux3dNL(
     *           m,aj(i1,i2,lay),aj(i1,i2p1,lay),metl,metr,
     *           dpdf(1,1,1,1,i1,i2,lay),dpdf(1,1,1,1,i1,i2p1,lay),
     *           w(1,i1,i2,s2p,lay),w(1,i1,i2p1,s2m,lay),
     *           fxl,fxr,almax(2) )
              do k = 1,12
                up(i1,i2p1,i3,k) = up(i1,i2p1,i3,k)+fxr(k)/ds2
                up(i1,i2,i3,k) = up(i1,i2,i3,k)-fxl(k)/ds2
              end do
            end if
          end do
        end do
c
c.. free stream correction
        if( icart.eq.0 )then
c        if( .false. )then
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
     *               -(rx(i1-1,i2,i3,1,j)+rx(i1,i2,i3,1,j))*d1m)
     *               /(4.0*ds1)
     *              +((rx(i1,i2+1,i3,2,j)+rx(i1,i2,i3,2,j))*d2p
     *               -(rx(i1,i2-1,i3,2,j)+rx(i1,i2,i3,2,j))*d2m)
     *               /(4.0*ds2)
     *              +((rx(i1,i2,i3+1,3,j)+rx(i1,i2,i3,3,j))*d3p
     *               -(rx(i1,i2,i3-1,3,j)+rx(i1,i2,i3,3,j))*d3m)
     *               /(4.0*ds3)
                if( j.eq.1 ) then
                  fx(1) = -w(4,i1,i2,scent,lay)/rho0
                  fx(2) = -w(5,i1,i2,scent,lay)/rho0
                  fx(3) = -w(6,i1,i2,scent,lay)/rho0
                elseif( j.eq.2 ) then
                  fx(1) = -w(7,i1,i2,scent,lay)/rho0
                  fx(2) = -w(8,i1,i2,scent,lay)/rho0
                  fx(3) = -w(9,i1,i2,scent,lay)/rho0
                else
                  fx(1) = -w(10,i1,i2,scent,lay)/rho0
                  fx(2) = -w(11,i1,i2,scent,lay)/rho0
                  fx(3) = -w(12,i1,i2,scent,lay)/rho0
                end if
                do k = 1,3
                  up(i1,i2,i3,k) = up(i1,i2,i3,k)
     *               +da*fx(k)/det(i1,i2,i3)
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
          call smuptz3dNL( m,xy(i1,i2,i3,1),xy(i1,i2,i3,2),
     *       xy(i1,i2,i3,3),t1,htz )
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
      return
      end
c
c+++++++++++++++++++++++
c
      subroutine smupflux3dNL( 
     *   m,ajl,ajr,metl,metr,
     *   dpdfl,dpdfr,
     *   wl,wr,fxl,fxr,speed )
c
      implicit none
c.. ingoing declarations
      integer m
      real ajl,ajr,metl(3,3),metr(3,3)
      real wl(*),wr(*),fxl(*),fxr(*),speed
      real dpdfl(3,3,3,3),dpdfr(3,3,3,3)
      include 'smupNLcommons.h'
c
c.. local declarations
      integer i,j,k,l,ier,iflux
      real norm(3),tan1(3),tan2(3)
      real aj
      real al(12),el(12,12),er(12,12)
      real alpha(12),alp
      real v(3)
      real dpdf0(3,3,3,3),cml(9,3),cmr(9,3)
c
c.. average "Ks"
      do k = 1,3
      do j = 1,3
      do i = 1,3
        do l = 1,3
          dpdf0(i,j,k,l) = 0.5*(dpdfl(i,j,k,l)+dpdfr(i,j,k,l))
        end do
        cml(i+(j-1)*3,k) = metl(1,1)*dpdfl(i,j,k,1)+
     *                     metl(1,2)*dpdfl(i,j,k,2)+
     *                     metl(1,3)*dpdfl(i,j,k,3)
        cmr(i+(j-1)*3,k) = metr(1,1)*dpdfr(i,j,k,1)+
     *                     metr(1,2)*dpdfr(i,j,k,2)+
     *                     metr(1,3)*dpdfr(i,j,k,3)
      end do
      end do
      end do
c
c.. metrics
      norm(1) = 0.5*(metl(1,1)+metr(1,1))
      norm(2) = 0.5*(metl(1,2)+metr(1,2))
      norm(3) = 0.5*(metl(1,3)+metr(1,3))
      tan1(1) = 0.5*(metl(2,1)+metr(2,1))
      tan1(2) = 0.5*(metl(2,2)+metr(2,2))
      tan1(3) = 0.5*(metl(2,3)+metr(2,3))
      tan2(1) = 0.5*(metl(3,1)+metr(3,1))
      tan2(2) = 0.5*(metl(3,2)+metr(3,2))
      tan2(3) = 0.5*(metl(3,3)+metr(3,3))
      
      aj = 0.5*(ajl+ajr)
c
c.. get eigen structure. Note that the iflux=1 flag indicates we only want the
c     first 3 eigenvalues/eigenvectors 
      iflux = 1
      call smeig3dNL( norm,tan1,tan2,dpdf0,al,el,er,iflux,ier )
      if( ier.ne.0 ) then
        write(6,*)'Error (smupflux3dNL): error in call'
        write(6,*)'  to eigenvalue decomposition'
        stop
      end if

c      write(6,*)
c      do j=1,12
c        write(6,*)el(1,j),el(2,j),el(3,j)
c      end do
c      write(6,*)
c
c      write(6,*)al(1),al(2),al(3)
c      write(6,*)
c      do j=1,12
c        write(6,*)er(j,1),er(j,2),er(j,3)
c      end do
c      write(6,*)
c
c      write(6,*)norm(1),norm(2),norm(3)
c      write(6,*)norm(1),tan1(1),tan2(1)
c      write(6,*)norm(2),tan1(2),tan2(2)
c      write(6,*)norm(3),tan1(3),tan2(3)
c      write(6,*)
c
c.. wave strengths from the left state (only need first 3)
      do k = 1,3
        alpha(k) = 0.0
        do j = 1,12
          alpha(k) = alpha(k)+el(k,j)*(wr(j)-wl(j))
        end do
c        write(6,*)wl(k),wr(k),alpha(k)
      end do
c
c.. conservative flux on the left
      fxl(1) = -aj*(norm(1)*wl(4)+norm(2)*wl(7)+norm(3)*wl(10))/rho0
      fxl(2) = -aj*(norm(1)*wl(5)+norm(2)*wl(8)+norm(3)*wl(11))/rho0
      fxl(3) = -aj*(norm(1)*wl(6)+norm(2)*wl(9)+norm(3)*wl(12))/rho0      
c      write(6,*)
c      write(6,*)fxl(1),fxl(2),fxl(3)
c      write(6,*)
c
c.. velocities on the left
      v(1) = wl(1)
      v(2) = wl(2)
      v(3) = wl(3)
c
c.. conservative flux and middle velocity state (computed from the left)
c     notice we could check the sign of al(j) and do a generic loop
c     but we assume the eigenvalues are ordered with the first 3 
c     less than 0
      do j = 1,3
        alp = aj*alpha(j)*al(j)
        do k = 1,3
          fxl(k) = fxl(k)+alp*er(k,j)
          v(k)   = v(k)+alpha(j)*er(k,j)
        end do
      end do
c
c.. account for Jacobian in the conservative fluxes
      do j = 1,3
        fxr(j) = fxl(j)/ajr
        fxl(j) = fxl(j)/ajl
      end do
c
c.. nonconservative flux for stresses
c      v(1) = 0.5*(wl(1)+wr(1))
c      v(2) = 0.5*(wl(2)+wr(2))
c      v(3) = 0.5*(wl(3)+wr(3))
      do j = 1,9
        fxl(j+3) = -cml(j,1)*v(1)-cml(j,2)*v(2)-cml(j,3)*v(3)
        fxr(j+3) = -cmr(j,1)*v(1)-cmr(j,2)*v(2)-cmr(j,3)*v(3)
      end do
c
c      if( abs(norm(1)).gt.1 ) then
c      if( .false. ) then
c        write(6,*)norm(1),norm(2),norm(3)
c        do j = 1,12
c          write(6,*)fxl(j)*ajl,fxr(j)*ajr
c        end do
c        write(6,*)
cc
c        do j = 1,9
c          write(6,*)cml(j,1),cml(j,2),cml(j,3)
c        end do
c        write(6,*)
c        do j = 1,9
c          write(6,*)cmr(j,1),cmr(j,2),cmr(j,3)
c        end do
c        write(6,*)
c        do j = 1,9
c          write(6,*)cmr(j,1)-cml(j,1),cmr(j,2)-cml(j,2),
c     *       cmr(j,3)-cml(j,3)
c        end do
c        write(6,*)
c        pause
c      end if
c
c      do j = 1,12
c        write(6,*)fxl(j)*ajl,fxr(j)*ajr
c      end do
c      write(6,*)
c      pause
c
c.. fastest wave speed
      speed = max( abs(al(1)),speed )
      speed = max( abs(al(2)),speed )
      speed = max( abs(al(3)),speed )
c
      return
      end
c
c++++++++++++++++++++
c
      subroutine smupslope3dNL( m,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *                          md1a,md1b,md2a,md2b,md3a,md3b,
     *                          n1a, n1b, n2a, n2b, n3a, n3b,
     *                          i3,ds1,ds2,ds3,dt,t,xy,
     *                          a1,aj,mask,u,w,dpdf,f1 )
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
      real f1(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,3)
      real w(m,md1a:md1b,md2a:md2b,7)
      real dpdf(3,3,3,3,md1a:md1b,md2a:md2b)
      include 'smupNLcommons.h'
c
c.. local declarations
      integer i1,i2,j,k,dir,iflux
      integer ideriv,ier
      real al(12),el(12,12),er(12,12)
      real norm(3),tan1(3),tan2(3)
      real htz(15)
      real alphal,alphar,alpha,tmp,tmp2,tmp3
      real s1,s2,s3
      real ur1(12),ur2(12),ur3(12),up
      real fr1(12),fr2(12),fr3(12)
      real u1r,u2r,u3r
      real u1s,u2s,u3s
      real u1t,u2t,u3t
      real du(3,3),p(3,3)
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
      if( iorder.eq.1 ) then
c
c.. compute dpdf to 1st order
        do i2 = n2a-1,n2b+1
        do i1 = n1a-1,n1b+1
          u1r = (u(i1+1,i2,i3,13)-u(i1-1,i2,i3,13))/(2.d0*ds1)
          u2r = (u(i1+1,i2,i3,14)-u(i1-1,i2,i3,14))/(2.d0*ds1)
          u3r = (u(i1+1,i2,i3,15)-u(i1-1,i2,i3,15))/(2.d0*ds1)
          u1s = (u(i1,i2+1,i3,13)-u(i1,i2-1,i3,13))/(2.d0*ds2)
          u2s = (u(i1,i2+1,i3,14)-u(i1,i2-1,i3,14))/(2.d0*ds2)
          u3s = (u(i1,i2+1,i3,15)-u(i1,i2-1,i3,15))/(2.d0*ds2)
          u1t = (u(i1,i2,i3+1,13)-u(i1,i2,i3-1,13))/(2.d0*ds3)
          u2t = (u(i1,i2,i3+1,14)-u(i1,i2,i3-1,14))/(2.d0*ds3)
          u3t = (u(i1,i2,i3+1,15)-u(i1,i2,i3-1,15))/(2.d0*ds3)

          du(1,1) = u1r*a1(1,1,i1,i2)+u1s*a1(2,1,i1,i2)
     *       +u1t*a1(3,1,i1,i2)
          du(1,2) = u1r*a1(1,2,i1,i2)+u1s*a1(2,2,i1,i2)
     *       +u1t*a1(3,2,i1,i2)
          du(1,3) = u1r*a1(1,3,i1,i2)+u1s*a1(2,3,i1,i2)
     *       +u1t*a1(3,3,i1,i2)
          
          du(2,1) = u2r*a1(1,1,i1,i2)+u2s*a1(2,1,i1,i2)
     *       +u2t*a1(3,1,i1,i2)
          du(2,2) = u2r*a1(1,2,i1,i2)+u2s*a1(2,2,i1,i2)
     *       +u2t*a1(3,2,i1,i2)
          du(2,3) = u2r*a1(1,3,i1,i2)+u2s*a1(2,3,i1,i2)
     *       +u2t*a1(3,3,i1,i2)

          du(3,1) = u3r*a1(1,1,i1,i2)+u3s*a1(2,1,i1,i2)
     *       +u3t*a1(3,1,i1,i2)
          du(3,2) = u3r*a1(1,2,i1,i2)+u3s*a1(2,2,i1,i2)
     *       +u3t*a1(3,2,i1,i2)
          du(3,3) = u3r*a1(1,3,i1,i2)+u3s*a1(2,3,i1,i2)
     *       +u3t*a1(3,3,i1,i2)

          ideriv = 1
          call sm3NLgetp( du,p,dpdf(1,1,1,1,i1,i2),ideriv )
        end do
        end do

      else
        ! 2nd order or TVD so we need slope correction
c
        s1 = 0.5*dt/ds1
        s2 = 0.5*dt/ds2
        s3 = 0.5*dt/ds3
c
c.. loop over space
        do i2 = n2a-1,n2b+1
        do i1 = n1a-1,n1b+1
        if( mask(i1,i2).ne.0 ) then
c
c.. compute dpdf to 2nd order
          u1r = (u(i1+1,i2,i3,13)-u(i1-1,i2,i3,13)
     *       +0.5*dt*(u(i1+1,i2,i3,1)-u(i1-1,i2,i3,1)))/(2.d0*ds1)
          u2r = (u(i1+1,i2,i3,14)-u(i1-1,i2,i3,14)
     *       +0.5*dt*(u(i1+1,i2,i3,2)-u(i1-1,i2,i3,2)))/(2.d0*ds1)
          u3r = (u(i1+1,i2,i3,15)-u(i1-1,i2,i3,15)
     *       +0.5*dt*(u(i1+1,i2,i3,3)-u(i1-1,i2,i3,3)))/(2.d0*ds1)
          u1s = (u(i1,i2+1,i3,13)-u(i1,i2-1,i3,13)
     *       +0.5*dt*(u(i1,i2+1,i3,1)-u(i1,i2-1,i3,1)))/(2.d0*ds2)
          u2s = (u(i1,i2+1,i3,14)-u(i1,i2-1,i3,14)
     *       +0.5*dt*(u(i1,i2+2,i3,1)-u(i1,i2-1,i3,2)))/(2.d0*ds2)
          u3s = (u(i1,i2+1,i3,15)-u(i1,i2-1,i3,15)
     *       +0.5*dt*(u(i1,i2+1,i3,3)-u(i1,i2-1,i3,3)))/(2.d0*ds2)
          u1t = (u(i1,i2,i3+1,13)-u(i1,i2,i3-1,13)
     *       +0.5*dt*(u(i1,i2,i3+1,1)-u(i1,i2,i3-1,1)))/(2.d0*ds3)
          u2t = (u(i1,i2,i3+1,14)-u(i1,i2,i3-1,14)
     *       +0.5*dt*(u(i1,i2,i3+1,2)-u(i1,i2,i3-1,2)))/(2.d0*ds3)
          u3t = (u(i1,i2,i3+1,15)-u(i1,i2,i3-1,15)
     *       +0.5*dt*(u(i1,i2,i3+1,3)-u(i1,i2,i3-1,3)))/(2.d0*ds3)

          if( itz.ne.0 ) then
            call smuptz3dNL( m,xy(i1-1,i2,i3,1),xy(i1-1,i2,i3,2),
     *                       xy(i1-1,i2,i3,3),t,htz )
            u1r = u1r-0.25*dt*htz(13)/ds1
            u2r = u2r-0.25*dt*htz(14)/ds1
            u3r = u3r-0.25*dt*htz(15)/ds1
            call smuptz3dNL( m,xy(i1+1,i2,i3,1),xy(i1+1,i2,i3,2),
     *                       xy(i1+1,i2,i3,3),t,htz )
            u1r = u1r+0.25*dt*htz(13)/ds1
            u2r = u2r+0.25*dt*htz(14)/ds1
            u3r = u3r+0.25*dt*htz(15)/ds1

            call smuptz3dNL( m,xy(i1,i2-1,i3,1),xy(i1,i2-1,i3,2),
     *                       xy(i1,i2-1,i3,3),t,htz )
            u1s = u1s-0.25*dt*htz(13)/ds2
            u2s = u2s-0.25*dt*htz(14)/ds2
            u3s = u3s-0.25*dt*htz(15)/ds2
            call smuptz3dNL( m,xy(i1,i2+1,i3,1),xy(i1,i2+1,i3,2),
     *                       xy(i1,i2+1,i3,3),t,htz )
            u1s = u1s+0.25*dt*htz(13)/ds2
            u2s = u2s+0.25*dt*htz(14)/ds2
            u3s = u3s+0.25*dt*htz(15)/ds2
            
            call smuptz3dNL( m,xy(i1,i2,i3-1,1),xy(i1,i2,i3-1,2),
     *                       xy(i1,i2,i3-1,3),t,htz )
            u1t = u1t-0.25*dt*htz(13)/ds3
            u2t = u2t-0.25*dt*htz(14)/ds3
            u3t = u3t-0.25*dt*htz(15)/ds3
            call smuptz3dNL( m,xy(i1,i2,i3+1,1),xy(i1,i2,i3+1,2),
     *                       xy(i1,i2,i3+1,3),t,htz )
            u1t = u1t+0.25*dt*htz(13)/ds3
            u2t = u2t+0.25*dt*htz(14)/ds3
            u3t = u3t+0.25*dt*htz(15)/ds3
          end if

          du(1,1) = u1r*a1(1,1,i1,i2)+u1s*a1(2,1,i1,i2)
     *       +u1t*a1(3,1,i1,i2)
          du(1,2) = u1r*a1(1,2,i1,i2)+u1s*a1(2,2,i1,i2)
     *       +u1t*a1(3,2,i1,i2)
          du(1,3) = u1r*a1(1,3,i1,i2)+u1s*a1(2,3,i1,i2)
     *       +u1t*a1(3,3,i1,i2)


          du(2,1) = u2r*a1(1,1,i1,i2)+u2s*a1(2,1,i1,i2)
     *       +u2t*a1(3,1,i1,i2)
          du(2,2) = u2r*a1(1,2,i1,i2)+u2s*a1(2,2,i1,i2)
     *       +u2t*a1(3,2,i1,i2)
          du(2,3) = u2r*a1(1,3,i1,i2)+u2s*a1(2,3,i1,i2)
     *       +u2t*a1(3,3,i1,i2)


          du(3,1) = u3r*a1(1,1,i1,i2)+u3s*a1(2,1,i1,i2)
     *       +u3t*a1(3,1,i1,i2)
          du(3,2) = u3r*a1(1,2,i1,i2)+u3s*a1(2,2,i1,i2)
     *       +u3t*a1(3,2,i1,i2)
          du(3,3) = u3r*a1(1,3,i1,i2)+u3s*a1(2,3,i1,i2)
     *       +u3t*a1(3,3,i1,i2)

          ideriv = 1
          call sm3NLgetp( du,p,dpdf(1,1,1,1,i1,i2),ideriv )
c
c.. twilight zone 
          if( itz.ne.0 ) then
            call smuptz3dNL( m,xy(i1,i2,i3,1),xy(i1,i2,i3,2),
     *                       xy(i1,i2,i3,3),t,htz )
            do dir = 1,7
              do k = 1,12
                w(k,i1,i2,dir) = w(k,i1,i2,dir)+0.5*dt*htz(k)
              end do
            end do
c
c.. body forcing 
          elseif( ifrc.ne.0 ) then
            do dir = 1,7
              do k = 1,3
                w(k,i1,i2,dir) = w(k,i1,i2,dir)+0.5*dt*f1(i1,i2,i3,k)
              end do
            end do
          end if
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
          iflux = 0
          call smeig3dNL( norm,tan1,tan2,dpdf(1,1,1,1,i1,i2),
     *       al,el,er,iflux,ier )
c
          if( ier.ne.0 ) then
            write(6,*)'Error (smupslope3dNL): error in call
     *         to eigenvalue decomposition s1 direction'
            stop
          end if

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
          iflux = 0
          call smeig3dNL( norm,tan1,tan2,dpdf(1,1,1,1,i1,i2),
     *       al,el,er,iflux,ier )
c
          if( ier.ne.0 ) then
            write(6,*)'Error (smupslope3dNL): error in call
     *         to eigenvalue decomposition s2 direction'
            stop
          end if
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
          iflux = 0
          call smeig3dNL( norm,tan1,tan2,dpdf(1,1,1,1,i1,i2),
     *       al,el,er,iflux,ier )
c
          if( ier.ne.0 ) then
            write(6,*)'Error (smupslope3dNL): error in call
     *         to eigenvalue decomposition s3 direction'
            stop
          end if
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
        end if ! mask
        end do ! i1
        end do ! i2
c
      end if ! second-order
c
      return
      end
c
c+++++++++++++++++++++++
c
      subroutine smeig3dNL( a,s,t,dpdf,al,el,er,iflux,ier )
c
      implicit none
c.. ingoing declarations
      integer ier,iflux
      real a(3),s(3),t(3),al(12),el(12,12),er(12,12)
      real dpdf(3,3,3,3)
      include 'smupNLcommons.h'
c
c.. locals
      integer i,j,k,info
      real an(3),sn(3),tn(3)
      real cm(9,3),bm(3,3)
      real lamReal(3),lamIm(3),junk,work(18),VR(3,3)
      real rhs(3,9)

      real sum,eps,jac,det,rad,alpha
c
      eps = 1.0e-10
c
c.. do orthonormalization of a,s,t into an,sn,tn
      ! normalize an
      rad    = sqrt(a(1)**2+a(2)**2+a(3)**2)
      an(1)  = a(1)/rad
      an(2)  = a(2)/rad
      an(3)  = a(3)/rad
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
c.. fill in coefficient matrix
      do k = 1,3
      do j = 1,3
      do i = 1,3
        cm(i+(j-1)*3,k) = an(1)*dpdf(i,j,k,1)
     *                   +an(2)*dpdf(i,j,k,2)
     *                   +an(3)*dpdf(i,j,k,3)
      end do
      end do
      end do
c
c      write(6,*)an(1),an(2),an(3)
c      write(6,*)sn(1),sn(2),sn(3)
c      write(6,*)tn(1),tn(2),tn(3)
c      write(6,*)
c      do k = 1,9
c        write(6,*)cm(k,1),cm(k,2),cm(k,3)
c      end do
c      write(6,*)
c
c.. create "B" matrix
      bm(1,1) = an(1)*cm(1,1)+an(2)*cm(4,1)+an(3)*cm(7,1)
      bm(1,2) = an(1)*cm(1,2)+an(2)*cm(4,2)+an(3)*cm(7,2)
      bm(1,3) = an(1)*cm(1,3)+an(2)*cm(4,3)+an(3)*cm(7,3)

      bm(2,1) = an(1)*cm(2,1)+an(2)*cm(5,1)+an(3)*cm(8,1)
      bm(2,2) = an(1)*cm(2,2)+an(2)*cm(5,2)+an(3)*cm(8,2)
      bm(2,3) = an(1)*cm(2,3)+an(2)*cm(5,3)+an(3)*cm(8,3)

      bm(3,1) = an(1)*cm(3,1)+an(2)*cm(6,1)+an(3)*cm(9,1)
      bm(3,2) = an(1)*cm(3,2)+an(2)*cm(6,2)+an(3)*cm(9,2)
      bm(3,3) = an(1)*cm(3,3)+an(2)*cm(6,3)+an(3)*cm(9,3)

c      do i = 1,9
c        write(6,*)cm(1,i),cm(2,i),cm(3,i)
c      end do
c      write(6,*)bm(1,1),bm(1,2),bm(1,3)
c      write(6,*)bm(2,1),bm(2,2),bm(2,3)
c      write(6,*)bm(3,1),bm(3,2),bm(3,3)
c      write(6,*)

      ! call LAPACK for eigenvalues (only a 3x3 system, I believe for many (all?) 
      !    material models this matrix is symmetric)
      if( abs(bm(1,2)-bm(2,1)).lt.eps .and.
     *    abs(bm(1,3)-bm(3,1)).lt.eps .and.
     *    abs(bm(2,3)-bm(3,2)).lt.eps ) then
        ! the matrix is symmetric
        call dsyev( 'V','U',3,bm,3,lamReal,work,18,info )
        if( info.ne.0 ) then
          write(6,*)
     *       'Error(smeig3dNL): symmetric eigenvalue solver failed'
          stop
        end if
c        write(6,*)info,rad
c        write(6,*)bm(1,1),bm(1,2),bm(1,3)
c        write(6,*)bm(2,1),bm(2,2),bm(2,3)
c        write(6,*)bm(3,1),bm(3,2),bm(3,3)
c        write(6,*)lamReal(1),lamReal(2),lamReal(3)
c        stop

        ! pack the result into VR
        VR(1,1) = bm(1,1)
        VR(2,1) = bm(2,1)
        VR(3,1) = bm(3,1)

        VR(1,2) = bm(1,2)
        VR(2,2) = bm(2,2)
        VR(3,2) = bm(3,2)

        VR(1,3) = bm(1,3)
        VR(2,3) = bm(2,3)
        VR(3,3) = bm(3,3)
      else
        ! the matrix is not symmetric
        write(6,*)'(INFO) using nonsymmetric eigenvalue solver'
        call dgeev( 'N','V',3,bm,3,lamReal,lamIm,
     *     junk,1,VR,3,work,18,info )
        if( info.ne.0 ) then
          write(6,*)
     *       'Error(smeig3dNL): general eigenvalue solver failed'
          stop
        end if

c        write(6,*)VR(1,1),VR(1,2),VR(1,3)
c        write(6,*)VR(2,1),VR(2,2),VR(2,3)
c        write(6,*)VR(3,1),VR(3,2),VR(3,3)
c        write(6,*)lamReal(1),lamReal(2),lamReal(3)
c        write(6,*)lamIm(1),lamIm(2),lamIm(3)
c        write(6,*)
      end if

      if( abs(lamIm(1)).gt.eps .or.
     *    abs(lamIm(2)).gt.eps .or.
     *    abs(lamIm(3)).gt.eps ) then
        write(6,*)'Error(smeig3dNL): complex eigenvalue'
        ier = 1
        return
      end if

      if( lamReal(1).lt.0.0 .or.
     *    lamReal(2).lt.0.0 .or.
     *    lamReal(3).lt.0.0 ) then
        write(6,*)'Error(smeig3dNL): complex eigenvalue'
        write(6,*)'    (negative square ev)'
        ier = 1
        return
      end if

      al(1)  = -sqrt(abs(lamReal(1))/rho0)
      al(2)  = -sqrt(abs(lamReal(2))/rho0)
      al(3)  = -sqrt(abs(lamReal(3))/rho0)
      al(4)  = 0.0
      al(5)  = 0.0
      al(6)  = 0.0
      al(7)  = 0.0
      al(8)  = 0.0
      al(9)  = 0.0
      al(10) = -al(3)
      al(11) = -al(2)
      al(12) = -al(1)
c
c.. right eigenvectors. We do the first 3 eigenvectors first because if iflux=1
c     then we return after computing those three.
      er(1,1) = VR(1,1)
      er(2,1) = VR(2,1)
      er(3,1) = VR(3,1)

      er(1,2) = VR(1,2)
      er(2,2) = VR(2,2)
      er(3,2) = VR(3,2)

      er(1,3) = VR(1,3)
      er(2,3) = VR(2,3)
      er(3,3) = VR(3,3)

      do j = 4,12
        er(j,1) = (-cm(j-3,1)*VR(1,1)
     *             -cm(j-3,2)*VR(2,1)
     *             -cm(j-3,3)*VR(3,1))/al(1)

        er(j,2) = (-cm(j-3,1)*VR(1,2)
     *             -cm(j-3,2)*VR(2,2)
     *             -cm(j-3,3)*VR(3,2))/al(2)

        er(j,3) = (-cm(j-3,1)*VR(1,3)
     *             -cm(j-3,2)*VR(2,3)
     *             -cm(j-3,3)*VR(3,3))/al(3)
      end do

      det = 
     *    VR(1,1)*VR(2,2)*VR(3,3)
     *   -VR(1,1)*VR(3,2)*VR(2,3)
     *   -VR(2,1)*VR(1,2)*VR(3,3)
     *   +VR(2,1)*VR(3,2)*VR(1,3)
     *   +VR(3,1)*VR(1,2)*VR(2,3)
     *   -VR(3,1)*VR(2,2)*VR(1,3)

      el(1,1) = (VR(2,2)*VR(3,3)-VR(3,2)*VR(2,3))/(2.*det)
      el(2,1) = (VR(3,1)*VR(2,3)-VR(2,1)*VR(3,3))/(2.*det)
      el(3,1) = (VR(2,1)*VR(3,2)-VR(3,1)*VR(2,2))/(2.*det)
      
      el(1,2) = (VR(3,2)*VR(1,3)-VR(1,2)*VR(3,3))/(2.*det)
      el(2,2) = (VR(1,1)*VR(3,3)-VR(3,1)*VR(1,3))/(2.*det)
      el(3,2) = (VR(3,1)*VR(1,2)-VR(1,1)*VR(3,2))/(2.*det)

      el(1,3) = (VR(1,2)*VR(2,3)-VR(2,2)*VR(1,3))/(2.*det)
      el(2,3) = (VR(2,1)*VR(1,3)-VR(1,1)*VR(2,3))/(2.*det)
      el(3,3) = (VR(1,1)*VR(2,2)-VR(2,1)*VR(1,2))/(2.*det)

      do j = 1,3
        el(j,4) = -an(1)*el(j,1)/(rho0*al(j))
        el(j,5) = -an(1)*el(j,2)/(rho0*al(j))
        el(j,6) = -an(1)*el(j,3)/(rho0*al(j))

        el(j,7) = -an(2)*el(j,1)/(rho0*al(j))
        el(j,8) = -an(2)*el(j,2)/(rho0*al(j))
        el(j,9) = -an(2)*el(j,3)/(rho0*al(j))

        el(j,10) = -an(3)*el(j,1)/(rho0*al(j))
        el(j,11) = -an(3)*el(j,2)/(rho0*al(j))
        el(j,12) = -an(3)*el(j,3)/(rho0*al(j))
      end do
c
c.. if iflux = 1, then we do only the first 3 right and left eigenvectors
      if( iflux.eq.1 ) then
        rad = sqrt(a(1)**2+a(2)**2+a(3)**2)
        do j = 1,12
          al(j) = rad*al(j)
        end do
        ier = 0
        return
      end if
c
c.. now iflux.ne.1 so we finish the remaining bits of the eigenstructure
      er(1,10) = VR(1,3)
      er(2,10) = VR(2,3)
      er(3,10) = VR(3,3)

      er(1,11) = VR(1,2)
      er(2,11) = VR(2,2)
      er(3,11) = VR(3,2)

      er(1,12) = VR(1,1)
      er(2,12) = VR(2,1)
      er(3,12) = VR(3,1)
      do j = 4,12
        er(j,10) = -er(j,3)
        er(j,11) = -er(j,2)
        er(j,12) = -er(j,1)
      end do

      er(4,4)  = -sn(2)*an(3)+sn(3)*an(2)
      er(7,4)  =  sn(1)*an(3)-sn(3)*an(1)
      er(10,4) = -sn(1)*an(2)+sn(2)*an(1)

      er(5,5)  =  er(4,4)
      er(8,5)  =  er(7,4)
      er(11,5) =  er(10,4)

      er(6,6)  =  er(4,4)
      er(9,6)  =  er(7,4)
      er(12,6) =  er(10,4)

      er(4,7)  = -tn(2)*an(3)+tn(3)*an(2)
      er(7,7)  =  tn(1)*an(3)-tn(3)*an(1)
      er(10,7) = -tn(1)*an(2)+tn(2)*an(1)

      er(5,8)  =  er(4,7)
      er(8,8)  =  er(7,7)
      er(11,8) =  er(10,7)
      
      er(6,9)  =  er(4,7)
      er(9,9)  =  er(7,7)
      er(12,9) =  er(10,7)
c
c.. remaining left eigenvectors ...
      el(10,1) = el(3,1)
      el(10,2) = el(3,2)
      el(10,3) = el(3,3)

      el(11,1) = el(2,1)
      el(11,2) = el(2,2)
      el(11,3) = el(2,3)

      el(12,1) = el(1,1)
      el(12,2) = el(1,2)
      el(12,3) = el(1,3)

c      do j = 4,12
c        el(10,j) = -el(3,j)
c        el(11,j) = -el(2,j)
c        el(12,j) = -el(1,j)
c      end do
      do j = 10,12
        el(j,4) = -an(1)*el(j,1)/(rho0*al(j))
        el(j,5) = -an(1)*el(j,2)/(rho0*al(j))
        el(j,6) = -an(1)*el(j,3)/(rho0*al(j))
        
        el(j,7) = -an(2)*el(j,1)/(rho0*al(j))
        el(j,8) = -an(2)*el(j,2)/(rho0*al(j))
        el(j,9) = -an(2)*el(j,3)/(rho0*al(j))
        
        el(j,10) = -an(3)*el(j,1)/(rho0*al(j))
        el(j,11) = -an(3)*el(j,2)/(rho0*al(j))
        el(j,12) = -an(3)*el(j,3)/(rho0*al(j))
      end do

      
c rows 4 and 7
      do j = 1,9
        rhs(1,j) = -2*(el(1,j+3)*er(4,1) 
     *     +el(2,j+3)*er(4,2) +el(3,j+3)*er(4,3))
        rhs(2,j) = -2*(el(1,j+3)*er(7,1) 
     *     +el(2,j+3)*er(7,2) +el(3,j+3)*er(7,3))
        rhs(3,j) = -2*(el(1,j+3)*er(10,1)
     *     +el(2,j+3)*er(10,2)+el(3,j+3)*er(10,3))
      end do
      rhs(1,1) = rhs(1,1)+1.
      rhs(2,4) = rhs(2,4)+1.
      rhs(3,7) = rhs(3,7)+1.

      do j = 1,9
        el(4,j+3) = er(4,4)*rhs(1,j)+er(7,4)*rhs(2,j)+er(10,4)*rhs(3,j)
        el(7,j+3) = er(4,7)*rhs(1,j)+er(7,7)*rhs(2,j)+er(10,7)*rhs(3,j)
      end do

c rows 5 and 8
      do j = 1,9
        rhs(1,j) = -2*(el(1,j+3)*er(5,1) 
     *     +el(2,j+3)*er(5,2) +el(3,j+3)*er(5,3))
        rhs(2,j) = -2*(el(1,j+3)*er(8,1) 
     *     +el(2,j+3)*er(8,2) +el(3,j+3)*er(8,3))
        rhs(3,j) = -2*(el(1,j+3)*er(11,1)
     *     +el(2,j+3)*er(11,2)+el(3,j+3)*er(11,3))
      end do
      rhs(1,2) = rhs(1,2)+1.
      rhs(2,5) = rhs(2,5)+1.
      rhs(3,8) = rhs(3,8)+1.

      do j = 1,9
        el(5,j+3) = er(5,5)*rhs(1,j)+er(8,5)*rhs(2,j)+er(11,5)*rhs(3,j)
        el(8,j+3) = er(5,8)*rhs(1,j)+er(8,8)*rhs(2,j)+er(11,8)*rhs(3,j)
      end do


c rows 6 and 9
      do j = 1,9
        rhs(1,j) = -2*(el(1,j+3)*er(6,1) 
     *     +el(2,j+3)*er(6,2) +el(3,j+3)*er(6,3))
        rhs(2,j) = -2*(el(1,j+3)*er(9,1) 
     *     +el(2,j+3)*er(9,2) +el(3,j+3)*er(9,3))
        rhs(3,j) = -2*(el(1,j+3)*er(12,1)
     *     +el(2,j+3)*er(12,2)+el(3,j+3)*er(12,3))
      end do
      rhs(1,3) = rhs(1,3)+1.
      rhs(2,6) = rhs(2,6)+1.
      rhs(3,9) = rhs(3,9)+1.

      do j = 1,9
        el(6,j+3) = er(6,6)*rhs(1,j)+er(9,6)*rhs(2,j)+er(12,6)*rhs(3,j)
        el(9,j+3) = er(6,9)*rhs(1,j)+er(9,9)*rhs(2,j)+er(12,9)*rhs(3,j)
      end do
c
c.. finally we must scale the eigenvalues
      rad    = sqrt(a(1)**2+a(2)**2+a(3)**2)
      do j = 1,12
        al(j) = rad*al(j)
      end do

c
c.. may want to check that these really are inverses for debugging purposes
c     make sure to turn this off for real runs ... FIX ME!!!
      if( .true. ) then
c      if( .false. ) then
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
      ier = 0
c
      return
      end
c
c+++++++++++++++++++++++
c
      subroutine smflux3dNL( an,aj,w,f )
c
      implicit none
c.. ingoing declarations
      real an(3),aj,w(12),f(12)
      include 'smupNLcommons.h'
c
c.. local declarations
      real a1,a2,a3,kap
c
      a1 = an(1)
      a2 = an(2)
      a3 = an(3)
c
      kap = lambda+2.0*mu
c
      f(1)  = -aj*(a1*w(4)+a2*w(7)+a3*w(10))/rho0
      f(2)  = -aj*(a1*w(5)+a2*w(8)+a3*w(11))/rho0
      f(3)  = -aj*(a1*w(6)+a2*w(9)+a3*w(12))/rho0
      f(4)  = -aj*(a1*kap*w(1)+lambda*(a2*w(2)+a3*w(3)))
      f(5)  = -aj*(mu*(a1*w(2)+a2*w(1)))
      f(6)  = -aj*(mu*(a1*w(3)+a3*w(1)))
      f(7)  = f(5)
      f(8)  = -aj*(lambda*(a1*w(1)+a3*w(3))+kap*a2*w(2))
      f(9)  = -aj*(mu*(a2*w(3)+a3*w(2)))
      f(10) = f(6)
      f(11) = f(9)
      f(12) = -aj*(lambda*(a1*w(1)+a2*w(2))+kap*a3*w(3))
c
      return
      end
c
c+++++++++++++++++++++++
c
      subroutine smupmetrics3dNL( nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
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
      include 'smupNLcommons.h'
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
      subroutine smuptz3dNL( m,x,y,z,t,h )
c
      implicit none
c.. ingoing declarations
      integer m
      real x,y,z,t,h(15)
      include 'smupNLcommons.h'
c
c.. local declarations
      integer ideriv,i,j,k,l
      real ut(15),ux(12),uy(12),uz(12),u0(3)
      real du(3,3),dpdf(3,3,3,3),p(3,3)
c
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
      if( .true. )then
        ! *wdh* 2015/09/23 -- Something is wrong below as ux(13) etc. have not been set!
        write(*,'("ERROR: smg3d-NL: FIX ME")') 
        stop 31415
      end if
      du(1,1) = ux(13)
      du(1,2) = uy(13)
      du(1,3) = uz(13)

      du(2,1) = ux(14)
      du(2,2) = uy(14)
      du(2,3) = uz(14)
      
      du(3,1) = ux(15)
      du(3,2) = uy(15)
      du(3,3) = uz(15)
      ideriv = 1
      call sm3NLgetp( du,p,dpdf,ideriv )
c
      h(1)  = ut(1) -(ux(4)+uy(7)+uz(10))/rho0
      h(2)  = ut(2) -(ux(5)+uy(8)+uz(11))/rho0
      h(3)  = ut(3) -(ux(6)+uy(9)+uz(12))/rho0
c
      do i = 4,12
        h(i) = ut(i)
      end do
c
      do i = 1,3
      do j = 1,3
        do k = 1,3
          h(i+(j-1)*3+3) = h(i+(j-1)*3+3)
     *       -dpdf(i,j,k,1)*ux(k)
     *       -dpdf(i,j,k,2)*uy(k)
     *       -dpdf(i,j,k,3)*uz(k)
        end do
      end do
      end do

      h(13) = ut(13)-u0(1)
      h(14) = ut(14)-u0(2)
      h(15) = ut(15)-u0(3)
c
      return
      end
c
c++++++++++++++++++++
c
      subroutine stressRelax3dNL( m,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
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
      real s11t,s12t,s13t
      real s21t,s22t,s23t
      real s31t,s32t,s33t
      real relaxAlpha,relaxDelta,beta
      real x,y,z
      real dp(3,3),p(3,3),du(3,3),dpdf
      integer ideriv
      include 'smupNLcommons.h'
c
      iRelax     = iparam(9)
c
      ! *wdh* 2015/08/23 -- we now pass in alpha+delta/dt (to support MOL schemes)
      beta       = rparam(7)
      ! *wdh*relaxAlpha = rparam(7)
      ! *wdh*relaxDelta = rparam(8)
      ! *wdh*beta = relaxAlpha+relaxDelta/dt

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

            du(1,1) = u1x
            du(1,2) = u1y
            du(1,3) = u1z

            du(2,1) = u2x
            du(2,2) = u2y
            du(2,3) = u2z

            du(3,1) = u3x
            du(3,2) = u3y
            du(3,3) = u3z
            ideriv = 0
            call sm3NLgetp( du,p,dpdf,ideriv )
            
            up(i1,i2,i3,4) = up(i1,i2,i3,4)+
     *         beta*(-u(i1,i2,i3,4)+p(1,1))
            up(i1,i2,i3,5) = up(i1,i2,i3,5)+
     *         beta*(-u(i1,i2,i3,5)+p(1,2))
            up(i1,i2,i3,6) = up(i1,i2,i3,6)+
     *         beta*(-u(i1,i2,i3,6)+p(1,3))
            
            up(i1,i2,i3,7) = up(i1,i2,i3,7)+
     *         beta*(-u(i1,i2,i3,7)+p(2,1))
            up(i1,i2,i3,8) = up(i1,i2,i3,8)+
     *         beta*(-u(i1,i2,i3,8)+p(2,2))
            up(i1,i2,i3,9) = up(i1,i2,i3,9)+
     *         beta*(-u(i1,i2,i3,9)+p(2,3))
            
            up(i1,i2,i3,10) = up(i1,i2,i3,10)+
     *         beta*(-u(i1,i2,i3,10)+p(3,1))
            up(i1,i2,i3,11) = up(i1,i2,i3,11)+
     *         beta*(-u(i1,i2,i3,11)+p(3,2))
            up(i1,i2,i3,12) = up(i1,i2,i3,12)+
     *         beta*(-u(i1,i2,i3,12)+p(3,3))
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

            du(1,1) = u1x
            du(1,2) = u1y
            du(1,3) = u1z

            du(2,1) = u2x
            du(2,2) = u2y
            du(2,3) = u2z

            du(3,1) = u3x
            du(3,2) = u3y
            du(3,3) = u3z
            ideriv = 0
            call sm3NLgetp( du,p,dpdf,ideriv )

            up(i1,i2,i3,4) = up(i1,i2,i3,4)+
     *         beta*(-u(i1,i2,i3,4)+p(1,1))
            up(i1,i2,i3,5) = up(i1,i2,i3,5)+
     *         beta*(-u(i1,i2,i3,5)+p(1,2))
            up(i1,i2,i3,6) = up(i1,i2,i3,6)+
     *         beta*(-u(i1,i2,i3,6)+p(1,3))
            
            up(i1,i2,i3,7) = up(i1,i2,i3,7)+
     *         beta*(-u(i1,i2,i3,7)+p(2,1))
            up(i1,i2,i3,8) = up(i1,i2,i3,8)+
     *         beta*(-u(i1,i2,i3,8)+p(2,2))
            up(i1,i2,i3,9) = up(i1,i2,i3,9)+
     *         beta*(-u(i1,i2,i3,9)+p(2,3))
            
            up(i1,i2,i3,10) = up(i1,i2,i3,10)+
     *         beta*(-u(i1,i2,i3,10)+p(3,1))
            up(i1,i2,i3,11) = up(i1,i2,i3,11)+
     *         beta*(-u(i1,i2,i3,11)+p(3,2))
            up(i1,i2,i3,12) = up(i1,i2,i3,12)+
     *         beta*(-u(i1,i2,i3,12)+p(3,3))      
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

            du(1,1) = u1xt
            du(1,2) = u1yt
            du(1,3) = u1zt

            du(2,1) = u2xt
            du(2,2) = u2yt
            du(2,3) = u2zt

            du(3,1) = u3xt
            du(3,2) = u3yt
            du(3,3) = u3zt
            ideriv = 0
            call sm3NLgetp( du,p,dpdf,ideriv )
            
            up(i1,i2,i3,4) = up(i1,i2,i3,4)-beta*(-s11t+p(1,1))
            up(i1,i2,i3,5) = up(i1,i2,i3,5)-beta*(-s12t+p(1,2))
            up(i1,i2,i3,6) = up(i1,i2,i3,6)-beta*(-s13t+p(1,3))
            
            up(i1,i2,i3,7) = up(i1,i2,i3,7)-beta*(-s21t+p(2,1))
            up(i1,i2,i3,8) = up(i1,i2,i3,8)-beta*(-s22t+p(2,2))
            up(i1,i2,i3,9) = up(i1,i2,i3,9)-beta*(-s23t+p(2,3))
            
            up(i1,i2,i3,10) = up(i1,i2,i3,10)-beta*(-s31t+p(3,1))
            up(i1,i2,i3,11) = up(i1,i2,i3,11)-beta*(-s32t+p(3,2))
            up(i1,i2,i3,12) = up(i1,i2,i3,12)-beta*(-s33t+p(3,3))
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
      subroutine sm3NLgetp( du,p,dpdf,ideriv )
c
      implicit none
c.. ingoing declarations
      integer ideriv
      real du(3,3),p(3,3),dpdf(3,3,3,3)
c
c.. locals
      integer i1,i2
      real f(3,3)
      include 'smupNLcommons.h'
c
c.. temporaries for generated code
      real t1
      real t2
      real t3
      real t4
      real t5
      real t6
      real t7
      real t8
      real t9
      real t10
      real t11
      real t12
      real t13
      real t14
      real t15
      real t16
      real t17
      real t18
      real t19
      real t20
      real t21
      real t22
      real t23
      real t24
      real t25
      real t26
      real t27
      real t28
      real t29
      real t30
      real t31
      real t32
      real t33
      real t34
      real t35
      real t36
      real t37
      real t38
      real t39
      real t40
      real t41
      real t42
      real t43
      real t44
      real t45
      real t46
      real t47
      real t48
      real t49
      real t50
      real t51
      real t52
      real t53
      real t54
      real t55
      real t56
      real t57
      real t58
      real t59
      real t60
      real t61
      real t62
      real t63
      real t64
      real t65
      real t66
      real t67
      real t68
      real t69
      real t70
      real t71
      real t72
      real t73
      real t74
      real t75
      real t76
      real t77
      real t78
      real t79
      real t80
      real t81
      real t82
      real t83
      real t84
      real t85
      real t86
      real t87
      real t88
      real t89
      real t90
      real t91
      real t92
      real t93
      real t94
      real t95
      real t96
      real t97
      real t98
      real t99
      real t100
      real t101
      real t102
      real t103
      real t104
      real t105
      real t106
      real t107
      real t108
      real t109
      real t110
      real t111
      real t112
      real t113
      real t114
      real t115
      real t116
      real t117
      real t118
      real t119
      real t120
      real t121
      real t122
      real t123
      real t124
      real t125
      real t126
      real t127
      real t128
      real t129
      real t130
      real t131
      real t132
      real t133
      real t134
      real t135
      real t136
      real t137
      real t138
      real t139
      real t140
      real t141
      real t142
      real t143
      real t144
      real t145
      real t146
      real t147
      real t148
      real t149
      real t150
      real t151
      real t152
      real t153
      real t154
      real t155
      real t156
      real t157
      real t158
      real t159
      real t160
      real t161
      real t162
      real t163
      real t164
      real t165
      real t166
      real t167
      real t168
      real t169
      real t170
      real t171
      real t172
      real t173
      real t174
      real t175
      real t176
      real t177
      real t178
      real t179
      real t180
      real t181
      real t182
      real t183
      real t184
      real t185
      real t186
      real t187
      real t188
      real t189
      real t190
      real t191
      real t192
      real t193
      real t194
      real t195
      real t196
      real t197
      real t198
      real t199
      real t200
      real t201
      real t202
      real t203
      real t204
      real t205
      real t206
      real t207
      real t208
      real t209
      real t210
      real t211
      real t212
      real t213
      real t214
      real t215
      real t216
      real t217
      real t218
      real t219
      real t220
      real t221
      real t222
      real t223
      real t224
      real t225
      real t226
      real t227
      real t228
      real t229
      real t230
      real t231
      real t232
      real t233
      real t234
      real t235
      real t236
      real t237
      real t238
      real t239
      real t240
      real t241
      real t242
      real t243
      real t244
      real t245
      real t246
      real t247
      real t248
      real t249
      real t250
c
      integer i,j,k,l

      do i1 = 1,3
      do i2 = 1,3
        f(i1,i2) = du(i1,i2)
      end do
      f(i1,i1) = f(i1,i1)+1.0
      end do

      if( itype.eq.1 ) then !linear reduction
        include 'LIN3D_press.h'

        if( ideriv.gt.0 ) then
          ! return derivatives as well
          include 'LIN3D_derivs.h'
        end if 


c        do j = 1,3
c        do k = 1,3
c        do l = 1,3
c          write(6,*),dpdf(i,j,k,l)
c        end do
c        end do
c        end do
c        end do

      else if( itype.eq.2 ) then !SVK model

        include 'SVK3D_press.h'

        if( ideriv.gt.0 ) then
          ! return derivatives as well
          include 'SVK3D_derivs.h'
        end if 

      else
        write(6,*)'ERROR: material model not implemented',itype
        stop

      end if ! itype

      return
      end
c
c++++++++++++++++++++
c
      subroutine stressDiss3d2( 
     *   m,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *   n1a,n1b,n2a,n2b,n3a,n3b,
     *   ds1,ds2,ds3,
     *   tsdiss,rx,u,up,mask,diseig )
c     
c second-order dissipation on the components of stress belonging to surfaces whose
c normals are tangent to cell faces.  The eigenvalues associated with these components
c are zero and thus the Godunov methods provides no dissipation on its own.
c
c used when iorder=1
c
      implicit none
c.. ingoing declarations
      integer nd1a,nd1b,n1a,n1b,m
      integer nd2a,nd2b,n2a,n2b
      integer nd3a,nd3b,n3a,n3b
      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real ds1,ds2,ds3
      real rx(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,3,3)
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,m)
      real up(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,m)
      real tsdiss,diseig
c
c .. locals
      integer i1,i2,i3,k
      real cmax,akap
      real du2(9)
      real du2s1(9),du2s2(9),du2s3(9)
      real an(3)
      real rad,rad1,rad2,rad3
      include 'smupNLcommons.h'
c
c approximate wave speed (to provide a scale)
      cmax = sqrt((lambda+2.*mu)/rho0)
c
c dissipation coeff (scaled by the approximate wave speed)
      akap = tsdiss*cmax
c
      if( icart.eq.1 ) then
c
c Cartesian case
c
        diseig = akap/ds1
        diseig = max(akap/ds2,diseig)
        diseig = max(akap/ds3,diseig)
c
        do i3 = n3a,n3b
        do i2 = n2a,n2b
        do i1 = n1a,n1b
          if( mask(i1,i2,i3).ne.0 ) then
            do k = 4,6
              du2(k) = akap*( 
     *    (u(i1,i2+1,i3,k)-2.*u(i1,i2,i3,k)+u(i1,i2-1,i3,k))/(4.*ds2)
     *   +(u(i1,i2,i3+1,k)-2.*u(i1,i2,i3,k)+u(i1,i2,i3-1,k))/(4.*ds3))
            end do

            do k = 7,9
              du2(k) = akap*( 
     *    (u(i1+1,i2,i3,k)-2.*u(i1,i2,i3,k)+u(i1-1,i2,i3,k))/(4.*ds1)
     *   +(u(i1,i2,i3+1,k)-2.*u(i1,i2,i3,k)+u(i1,i2,i3-1,k))/(4.*ds3))
            end do

            do k = 10,12
              du2(k) = akap*( 
     *    (u(i1+1,i2,i3,k)-2.*u(i1,i2,i3,k)+u(i1-1,i2,i3,k))/(4.*ds1)
     *   +(u(i1,i2+1,i3,k)-2.*u(i1,i2,i3,k)+u(i1,i2-1,i3,k))/(4.*ds2))
            end do
c
            do k = 4,12
              up(i1,i2,i3,k) = up(i1,i2,i3,k)+du2(k)
            end do
          end if
        end do
        end do
        end do
c
      else
c
c Curvilinear case
c
        diseig = 0.
c
        do i3 = n3a,n3b
        do i2 = n2a,n2b
        do i1 = n1a,n1b
          if( mask(i1,i2,i3).ne.0 ) then
            ! first add operator in all directions
            ! loop over components
            rad1 = sqrt(rx(i1,i2,i3,1,1)**2
     *         +rx(i1,i2,i3,1,2)**2+rx(i1,i2,i3,1,3)**2)
            rad2 = sqrt(rx(i1,i2,i3,2,1)**2
     *         +rx(i1,i2,i3,2,2)**2+rx(i1,i2,i3,2,3)**2)
            rad3 = sqrt(rx(i1,i2,i3,3,1)**2
     *         +rx(i1,i2,i3,3,2)**2+rx(i1,i2,i3,3,3)**2)

            diseig = max(akap*rad1/ds1,diseig)
            diseig = max(akap*rad2/ds2,diseig)
            diseig = max(akap*rad3/ds3,diseig)
            do k = 4,12
              du2s1(k) = akap*rad1*(u(i1+1,i2,i3,k)
     *                  -2.*u(i1,i2,i3,k)+u(i1-1,i2,i3,k))/(4.*ds1)
              du2s2(k) = akap*rad2*(u(i1,i2+1,i3,k)
     *                  -2.*u(i1,i2,i3,k)+u(i1,i2-1,i3,k))/(4.*ds2)
              du2s3(k) = akap*rad3*(u(i1,i2,i3+1,k)
     *                  -2.*u(i1,i2,i3,k)+u(i1,i2,i3-1,k))/(4.*ds3)

              up(i1,i2,i3,k) = up(i1,i2,i3,k)
     *           +du2s1(k)+du2s2(k)+du2s3(k)
            end do

            ! remove dissipation in s1-dir
            an(1) = rx(i1,i2,i3,1,1)
            an(2) = rx(i1,i2,i3,1,2)
            an(3) = rx(i1,i2,i3,1,3)
            an(1) = an(1)/rad1
            an(2) = an(2)/rad1
            an(3) = an(3)/rad1
            do k = 4,6
              up(i1,i2,i3,k) = up(i1,i2,i3,k)
     *           -(an(1)*du2s1(k)+an(2)*du2s2(k)+an(3)*du2s3(k))
            end do

            ! remove dissipation in s2-dir
            an(1) = rx(i1,i2,i3,2,1)
            an(2) = rx(i1,i2,i3,2,2)
            an(3) = rx(i1,i2,i3,2,3)
            an(1) = an(1)/rad2
            an(2) = an(2)/rad2
            an(3) = an(3)/rad2
            do k = 7,9
              up(i1,i2,i3,k) = up(i1,i2,i3,k)
     *           -(an(1)*du2s1(k)+an(2)*du2s2(k)+an(3)*du2s3(k))
            end do

            ! remove dissipation in s3-dir
            an(1) = rx(i1,i2,i3,3,1)
            an(2) = rx(i1,i2,i3,3,2)
            an(3) = rx(i1,i2,i3,3,3)
            an(1) = an(1)/rad3
            an(2) = an(2)/rad3
            an(3) = an(3)/rad3
            do k = 10,12
              up(i1,i2,i3,k) = up(i1,i2,i3,k)
     *           -(an(1)*du2s1(k)+an(2)*du2s2(k)+an(3)*du2s3(k))
            end do
c
          end if
        end do
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
      subroutine stressDiss3d4 (
     *   m,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *   n1a,n1b,n2a,n2b,n3a,n3b,
     *   ds1,ds2,ds3,
     *   tsdiss,rx,u,up,mask,diseig )
c
c fourth-order dissipation on the components of stress belonging to surfaces whose
c normals are tangent to cell faces.  The eigenvalues associated with these components
c are zero and thus the Godunov methods provides no dissipation on its own.
c
c used when iorder=2
c
      implicit none
c.. ingoing declarations
      integer nd1a,nd1b,n1a,n1b,m
      integer nd2a,nd2b,n2a,n2b
      integer nd3a,nd3b,n3a,n3b
      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real ds1,ds2,ds3
      real rx(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,3,3)
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,m)
      real up(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,m)
      real tsdiss,diseig
c
c .. locals
      integer i1,i2,i3,k
      real cmax,akap
      real du4(9)
      real du4s1(9),du4s2(9),du4s3(9)
      real an(3)
      real rad,rad1,rad2,rad3
      include 'smupNLcommons.h'
c
c
c approximate wave speed (to provide a scale)
      cmax = sqrt((lambda+2.*mu)/rho0)
c
c dissipation coeff (scaled by the approximate wave speed)
      akap = tsdiss*cmax
c
      if (icart.eq.1) then
c
c Cartesian case
c
        diseig = akap/ds1
        diseig = max(akap/ds2,diseig)
        diseig = max(akap/ds3,diseig)
c
        do i3 = n3a,n3b
        do i2 = n2a,n2b
        do i1 = n1a,n1b
          if( mask(i1,i2,i3).ne.0 ) then
            do k = 4,6
              du4(k) = akap*( 
     *         (-u(i1,i2+2,i3,k)+4.0*u(i1,i2+1,i3,k)-6.*u(i1,i2,i3,k)
     *             +4.0*u(i1,i2-1,i3,k)-u(i1,i2-2,i3,k))/(16.*ds2)
     *        +(-u(i1,i2,i3+2,k)+4.0*u(i1,i2,i3+1,k)-6.*u(i1,i2,i3,k)
     *             +4.0*u(i1,i2,i3-1,k)-u(i1,i2,i3-2,k))/(16.*ds3))
            end do

            do k = 7,9
              du4(k) = akap*( 
     *         (-u(i1+2,i2,i3,k)+4.0*u(i1+1,i2,i3,k)-6.*u(i1,i2,i3,k)
     *             +4.0*u(i1-1,i2,i3,k)-u(i1-2,i2,i3,k))/(16.*ds1)
     *        +(-u(i1,i2,i3+2,k)+4.0*u(i1,i2,i3+1,k)-6.*u(i1,i2,i3,k)
     *             +4.0*u(i1,i2,i3-1,k)-u(i1,i2,i3-2,k))/(16.*ds3))
            end do

            do k = 10,12
              du4(k) = akap*( 
     *         (-u(i1+2,i2,i3,k)+4.0*u(i1+1,i2,i3,k)-6.*u(i1,i2,i3,k)
     *             +4.0*u(i1-1,i2,i3,k)-u(i1-2,i2,i3,k))/(16.*ds1)
     *        +(-u(i1,i2+2,i3,k)+4.0*u(i1,i2+1,i3,k)-6.*u(i1,i2,i3,k)
     *             +4.0*u(i1,i2-1,i3,k)-u(i1,i2-2,i3,k))/(16.*ds2))
            end do
c
            do k = 4,12
              up(i1,i2,i3,k) = up(i1,i2,i3,k)+du4(k)
            end do
          end if
        end do
        end do
        end do
c
      else
c
c Curvilinear case
c
        diseig = 0.
        do i3 = n3a,n3b
        do i2 = n2a,n2b
        do i1 = n1a,n1b
          if( mask(i1,i2,i3).ne.0 ) then
            ! first add operator in all directions
            ! loop over components
            rad1 = sqrt(rx(i1,i2,i3,1,1)**2
     *         +rx(i1,i2,i3,1,2)**2+rx(i1,i2,i3,1,3)**2)
            rad2 = sqrt(rx(i1,i2,i3,2,1)**2
     *         +rx(i1,i2,i3,2,2)**2+rx(i1,i2,i3,2,3)**2)
            rad3 = sqrt(rx(i1,i2,i3,3,1)**2
     *         +rx(i1,i2,i3,3,2)**2+rx(i1,i2,i3,3,3)**2)

            diseig = max(akap*rad1/ds1,diseig)
            diseig = max(akap*rad2/ds2,diseig)
            diseig = max(akap*rad3/ds3,diseig)
            do k = 4,12
              du4s1(k) = akap*rad1*(
     *           -u(i1+2,i2,i3,k)+4.0*u(i1+1,i2,i3,k)-6.*u(i1,i2,i3,k)
     *             +4.0*u(i1-1,i2,i3,k)-u(i1-2,i2,i3,k))/(16.*ds1)
              du4s2(k) = akap*rad2*(
     *           -u(i1,i2+2,i3,k)+4.0*u(i1,i2+1,i3,k)-6.*u(i1,i2,i3,k)
     *             +4.0*u(i1,i2-1,i3,k)-u(i1,i2-2,i3,k))/(16.*ds2)
              du4s3(k) = akap*rad3*(
     *           -u(i1,i2,i3+2,k)+4.0*u(i1,i2,i3+1,k)-6.*u(i1,i2,i3,k)
     *             +4.0*u(i1,i2,i3-1,k)-u(i1,i2,i3-2,k))/(16.*ds3)

              up(i1,i2,i3,k) = up(i1,i2,i3,k)
     *           +du4s1(k)+du4s2(k)+du4s3(k)
            end do

            ! remove dissipation in s1-dir
            an(1) = rx(i1,i2,i3,1,1)
            an(2) = rx(i1,i2,i3,1,2)
            an(3) = rx(i1,i2,i3,1,3)
            an(1) = an(1)/rad1
            an(2) = an(2)/rad1
            an(3) = an(3)/rad1
            do k = 4,6
              up(i1,i2,i3,k) = up(i1,i2,i3,k)
     *           -(an(1)*du4s1(k)+an(2)*du4s2(k)+an(3)*du4s3(k))
            end do

            ! remove dissipation in s2-dir
            an(1) = rx(i1,i2,i3,2,1)
            an(2) = rx(i1,i2,i3,2,2)
            an(3) = rx(i1,i2,i3,2,3)
            an(1) = an(1)/rad2
            an(2) = an(2)/rad2
            an(3) = an(3)/rad2
            do k = 7,9
              up(i1,i2,i3,k) = up(i1,i2,i3,k)
     *           -(an(1)*du4s1(k)+an(2)*du4s2(k)+an(3)*du4s3(k))
            end do

            ! remove dissipation in s3-dir
            an(1) = rx(i1,i2,i3,3,1)
            an(2) = rx(i1,i2,i3,3,2)
            an(3) = rx(i1,i2,i3,3,3)
            an(1) = an(1)/rad3
            an(2) = an(2)/rad3
            an(3) = an(3)/rad3
            do k = 10,12
              up(i1,i2,i3,k) = up(i1,i2,i3,k)
     *           -(an(1)*du4s1(k)+an(2)*du4s2(k)+an(3)*du4s3(k))
            end do
c
          end if
        end do
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
