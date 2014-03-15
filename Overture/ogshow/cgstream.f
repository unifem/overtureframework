      subroutine cgstrn2(id,rd,pu,pv,pkr,pxy,bc,ndrsab,nrsab,mrsab,
     & ng,wdir,iset,iframe,pxyab,prsxy,intopt,cgtype,cgdir,period,
     & nxg,nyg,nga,grids,ip,rp )
c==================================================================
c  Plot Streamlines of (u,v)... lower level for cgstrm
c Variables of interest
c==================================================================
      integer id(*),nrsab(2,2,ng),mrsab(2,2,ng),ndrsab(2,2,ng),
     & pu(ng),pv(ng),pkr(ng),pxy(ng),bc(2,2,ng),prsxy(ng),
     & pxyab,cgtype,cgdir,wdir,grids(ng),ip(*)
      real rp(*),rd(*)
      logical period(ng)
c..........local
      parameter( ndg=10000,ngd=50 )
      real xyabl(2,2)
      integer ig(ndg),dskloc,pcgp,dim(2),puv(4*ngd)
      logical plot
      parameter( nip2=20 )
      integer ip2(nip2)
      real rp2(nip2)
c.......start statement functions
      ndra(k)=ndrsab(1,1,k)
      ndrb(k)=ndrsab(1,2,k)
      ndsa(k)=ndrsab(2,1,k)
      ndsb(k)=ndrsab(2,2,k)
      xyab(m,n)=rd(pxyab+m-1+2*(n-1))
      ndr(k)=ndrsab(1,2,k)-ndrsab(1,1,k)+1
      nds(k)=ndrsab(2,2,k)-ndrsab(2,1,k)+1
      kr(i,j,k)=id(pkr(k)+i-ndrsab(1,1,k)+ndr(k)*(j-ndrsab(2,1,k)))
      u(i,j,k) =rd( pu(k)+i-ndrsab(1,1,k)+ndr(k)*(j-ndrsab(2,1,k)))
      v(i,j,k) =rd( pv(k)+i-ndrsab(1,1,k)+ndr(k)*(j-ndrsab(2,1,k)))
c.......end statement functions

      if( ng.gt.ngd )then
        stop 'CGSTRM2: ng > ngd'
      end if

c      ---set plotting bounds
      if( iset.eq.0 )then
        if( ip(6).eq.0 )then
          xyabl(1,1)=xyab(1,1)
          xyabl(1,2)=xyab(1,2)
          xyabl(2,1)=xyab(2,1)
          xyabl(2,2)=xyab(2,2)
        else
          xyabl(1,1)=rp(1)  ! set bounds based on values in rp
          xyabl(2,1)=rp(2)
          xyabl(1,2)=rp(3)
          xyabl(2,2)=rp(4)
        end if
        xa=xyabl(1,1)
        xb=xyabl(1,2)
        ya=xyabl(2,1)
        yb=xyabl(2,2)
        if( yb-ya .ge. xb-xa )then
          dxy=.4*(xb-xa)/(yb-ya)
          rag=.5-dxy
          rbg=.5+dxy
          sag=.1
          sbg=.9
        else
          dxy=.4*(yb-ya)/(xb-xa)
          rag=.1
          rbg=.9
          sag=.5-dxy
          sbg=.5+dxy
        end if
        call set(rag,rbg,sag,sbg,xa,xb,ya,yb,1)
      else
c       --get plotting bounds
        call getset( rag,rbg,sag,sbg,xyabl(1,1),xyabl(1,2),
     &                               xyabl(2,1),xyabl(2,2),idum )
      end if
*      write(6,9900) rag,rbg,sag,sbg,xyab(1,1),xyab(2,1),
*     & xyab(1,2),xyab(2,2),xyabl
* 9900 format(1x,' rag,rbg,sag,sbg =',4i5,/,
*     &       1x,' xyab =',4f12.4,/,
*     &       1x,' xyabl=',4f12.4)
      xa=xyabl(1,1)
      xb=xyabl(1,2)
      xba=xb-xa
      ya=xyabl(2,1)
      yb=xyabl(2,2)
      yba=yb-ya

c.......Plot boundary curves of the grid
      pcgp =dskloc(id,wdir,'cgp')
      if( pcgp.eq.0 )then
        dim(1)=1
        dim(2)=2
        call dskdef(id,wdir,'cgp','I',dim,pcgp,ierr)
        id(pcgp)  =0
        id(pcgp+1)=0
      end if
c       save cgp values so they can be reset
      icgp1=id(pcgp  )
      icgp2=id(pcgp+1)
c                     ! plot only boundary curves
      id(pcgp)  =15
      id(pcgp+1)=0
      ip2(1)=wdir
      ip2(2)=1       ! iset=1
      ip2(3)=iframe
      call cggp( id,cgdir,ip2,rp2,ierr )
      call dskrel( id,wdir,'cgp',' ' ,ierr )

c***
      call set(rag,rbg,sag,sbg,xa,xb,ya,yb,1)

      nrsmx=0
      do 50 k=1,ng
        nrsmx=max(nrsmx,nrsab(1,2,k)-nrsab(1,1,k),
     &                  nrsab(2,2,k)-nrsab(2,1,k))
 50   continue
      dwx=(rbg-rag)
      dwy=(sbg-sag)
      aspect=(xyabl(2,2)-xyabl(2,1))/(xyabl(1,2)-xyabl(1,1))*dwx/dwy
      if( nxg.le.0 .or. nyg.le.0 )then
        if( dwy.gt.dwx )then
          nyg=min(60,nrsmx)
          nxg=max(.3*nyg,nyg*dwx/dwy)
        else
          nxg=min(60,nrsmx)
          nyg=max(.3*nxg,nxg*dwy/dwx)
        end if
      end if

c*    write(6,*) ' nxg,nyg,aspect =',nxg,nyg,aspect
c     write(6,*) 'Enter new nxg,nyg'
c*    read(5,*) nxg,nyg

      if( nxg*nyg .gt.ndg )then
        ifact=sqrt( real(ngd/nxg*nyg) )
        nxg=nxg*ifact-1
        nyg=nyg*ifact-1
      end if

c............Plot Streamlines on each component grid............
c       the array ig(ixg,iyg) is set >= 1 if a streamline
c       passes through the cell (ixg,iyg)
      do 100 j=1,nyg
        do 100 i=1,nxg
          ig(i+nxg*(j-1))=0
        continue
 100  continue
      do 200 k=1,ng
        puv(k   )=pu(k)
        puv(k+ng)=pv(k)
 200  continue
c.......determine max and min value of u**2+v**2
      uvmx=0.
      uvmn=1.e20
      do k0=1,nga
        k=grids(k0)  ! active grids
        do i=mrsab(1,1,k),mrsab(1,2,k)
          do j=mrsab(2,1,k),mrsab(2,2,k)
            if( kr(i,j,k).ne.0 )then
              uv=u(i,j,k)**2+v(i,j,k)**2
              uvmx=max(uvmx,uv)
              uvmn=min(uvmn,uv)
            end if
          end do
        end do
      end do
c*    write(*,*) ' uvmn,uvmx = ',uvmn,uvmx
c*wdh
c     write(*,*) '     : pu  =',pu
c     write(*,*) '     : pv  =',pv
c     write(*,*) '     : pkr =',pkr
c     write(*,*) '     : pxy =',pxy
c     write(*,*) '     : nrsab =',nrsab
c     write(*,*) '     : ndrsab =',ndrsab
c     write(*,*) ' kr(.,.,1) =',(((kr(i,j,k),i=nrsab(1,1,k),
c    &  nrsab(1,2,k)),j=nrsab(2,1,k),nrsab(2,2,k)),k=1,1)
c     write(*,*) ' u(.,.,1) =',(((u(i,j,k),i=nrsab(1,1,k),
c    &  nrsab(1,2,k)),j=nrsab(2,1,k),nrsab(2,2,k)),k=1,1)
c     write(*,*) ' v(.,.,1) =',(((v(i,j,k),i=nrsab(1,1,k),
c    &  nrsab(1,2,k)),j=nrsab(2,1,k),nrsab(2,2,k)),k=1,1)
c*wdh


      cfl=.05
      plot=.true.
      do 400 i=1,nxg
        do 400 j=1,nyg
          if( ig(i+nxg*(j-1)).eq.0 )then
c                                      ! starting point for streamline
            xtp=xa+xba*(i-1+.5)/nxg
            ytp=ya+yba*(j-1+.5)/nyg
c             first integrate backwards in time to get starting
c             point for streamline
            plot=.false.
c*wdh       cfl=-.25
            cfl=-.5
            xts=xtp
            yts=ytp
            call cgstr1( xtp,ytp,itp,jtp,ktp, ng,ndrsab,nrsab,
     &       mrsab,period,intopt,cgtype,pxy,pkr,prsxy,puv,
     &       nxg,nxg,nyg,ig,xyabl,id,rd,cfl,plot,aspect,uvmx,uvmn )
c           write(6,9000) xts,yts,xtp,ytp
c9000 format(1x,'CGSTRM2: (xts,yts)=',2f10.3,' (xtp,ytp)=',2f10.3 )
c            Now plot the streamline
            plot=.true.
c*wdh       cfl= .25
            cfl= .5
            call cgstr1( xtp,ytp,itp,jtp,ktp, ng,ndrsab,nrsab,
     &       mrsab,period,intopt,cgtype,pxy,pkr,prsxy,puv,
     &       nxg,nxg,nyg,ig,xyabl,id,rd,cfl,plot,aspect,uvmx,uvmn )
          end if
        continue
 400  continue

c       reset cgp values
      id(pcgp  )=icgp1
      id(pcgp+1)=icgp2
      return
      end

      subroutine cgstr1( xtp,ytp,itp,jtp,ktp, ng,ndrsab,nrsab,mrsab,
     &  period,intopt,cgtype,pxy,pkr,prsxy,puv,ndg,nxg,nyg,ig,xyab,
     &  id,rd,cfl,plot,aspect,uvmx,uvmn )
c================================================================
c          Composite Grid - Streamline Routine
c          --------------   ------------------
c Purpose -
c Integrate the streamline whose initial position is (xtp,ytp)
c and plot the streamline if plot=.true.
c If Plot=.true. mark the cells in the array ig(i,j) i=1,nxg j=1,nyg
c which the streamline passes through and stop plotting the
c streamline if the streamline enters a cell with ig(i,j) > 1
c Plot arrows on the streamline when the streamline enters a
c cell (i,j) satisfying mod(i,lax)=0 and mod(j,lay)=0 where
c (lax,lay) are assign below
c  Colour the contours according to the value of u**2+v**2.
c
c  Who to Blame: Bill Henshaw  1989.
c================================================================
      real xtp,ytp,rd(*),xyab(2,2),uvmx,uvmn
      integer pxy(ng),pkr(ng),prsxy(ng),puv(ng,2),id(*),
     &  itp,jtp,ktp,ndrsab(2,2,ng),nrsab(2,2,ng),cgtype,
     &  ig(ndg,*),mrsab(2,2,ng)
      logical period(2,ng),plot
c........local
      real ui(4)
c*wdh for colour contours
      parameter ( ncol=11 )
      character colour(-1:ncol-1)*20
      data colour/'black','blue violet','blue','turquoise','sea green',
     &  'green','yellow green','yellow',
     & 'orange','orange red','red','violet red'/
      include 'ctable.h'
c........
c              ! max(2,nxg/5)
      lax=5
      lay=5

      dxmx=max(xyab(1,2)-xyab(1,1),xyab(2,2)-xyab(2,1))
      nrsmx=0
      do 100 k=1,ng
        nrsmx=max(nrsmx,nrsab(1,2,k)-nrsab(1,1,k),
     &                  nrsab(2,2,k)-nrsab(2,1,k))
 100  continue
c.......for colour streamlines
      icol1=-2
      icol =0
      if( uvmx.ne.uvmn )then
        uvfact=ncol/(uvmx-uvmn)
      else
        uvfact=ncol
      end if


c              ! maximum number of time steps
      nt=1000
      t=0.
      cfla=abs(cfl)*dxmx/nrsmx
      dtmx=cfla*2.
      xa=xyab(1,1)
      ya=xyab(2,1)
      xgf=nxg/(xyab(1,2)-xyab(1,1))
      ygf=nyg/(xyab(2,2)-xyab(2,1))
      nv=2
      ni=1
      itp=1
      jtp=1
      ktp=1
      x=xtp
      y=ytp
      xi=x
      yi=y
c         interpolate velocity (ui(1),ui(2)) at (x,y)
      call cgitpl( ni,x,y,itp,jtp,ktp,ui,ng,nv,ndrsab,nrsab,mrsab,
     & pxy,pkr,prsxy,puv,intopt,rd,id,cgtype,period )
      if( ktp.le.0 ) then
c       ....unable to interpolate
        goto 900
      end if
      if( plot )then
        call frstpt( x,y )
      end if
      ixg0=0
      iyg0=0
      ixg1=(x-xa)*xgf+1
      iyg1=(y-ya)*ygf+1
      dpath=0.
c...........Take time steps it=2,3,...,nt  (t=dt,2dt,...,(nt-1)*dt)
      do 200 it=2,nt
c.............get: ui(1)=u(k1) ui(2)=v(k1) ui(3)=u(k1p) ui(4)=v(k1p)
        xi=x
        yi=y
        uiabs=abs(ui(1))+abs(ui(2))
        if( uiabs.lt.1.e-4*uvmx )then
c*wdh   if( uiabs.lt.1.e-3*uvmx )then
c          flow is too slow here to move anywhere
          goto 900
        end if
c*wdh950214: fix for slow flows??     dts=ign(min(dtmx,cfla/uiabs),cfl)
c*wdh        dt=sign(cfla/uiabs,cfl)
        dt=sign(min(dtmx,cfla)/uiabs,cfl)
        t=t+dt
        xs=x+dt*ui(1)
        ys=y+dt*ui(2)
        if( xs.lt.xyab(1,1) .or. xs.gt.xyab(1,2) .or.
     &      ys.lt.xyab(2,1) .or. ys.gt.xyab(2,2) )then
c         ....outside plotting bounds:
          goto 900
        end if
c         interpolate velocity (ui(1),ui(2)) at (xs,ys)
        call cgitpl( ni,xs,ys,itp,jtp,ktp,ui,ng,nv,ndrsab,nrsab,mrsab,
     &    pxy,pkr,prsxy,puv,intopt,rd,id,cgtype,period )
        if( ktp.le.0 )then
c         ....unable to interpolate
          goto 900
        end if
c                                ! trapezoidal rule
        uiabs=abs(ui(1))+abs(ui(2))
        dt=sign(min(dtmx,cfla)/uiabs,cfl)
        x=.5*(xs + x+dt*ui(1))
        y=.5*(ys + y+dt*ui(2))

c**     write(*,*) ' CGSTR1: x,y,ui =',x,y,ui(1),ui(2)

        if( plot )then
          if( mod(icgst,2).eq.1 )then
            icol=-1
          else
            icol=(ui(1)**2+ui(2)**2-uvmn)*uvfact
            icol=max(0,min(icol,ncol-1))
          end if
          if( icol.ne.icol1 )then
            call g1cci( colour(icol),icola,ierr )
            call gpplci( icola )
            icol1=icol
          end if
          call vector( x,y )
        end if
c         mark cell as being passed through
        ixg=(x-xa)*xgf+1
        iyg=(y-ya)*ygf+1
        if( ixg.ge.1 .and. ixg.le.nxg .and.
     &      iyg.ge.1 .and. iyg.le.nyg     )then
          if( plot )then
            if( ixg.ne.ixg0 .or. iyg.ne.iyg0 )then
c                                          ! only 2 streamlines allowed
              if( ig(ixg,iyg).ge.2 )then
                goto 900
              end if
              ig(ixg,iyg)=ig(ixg,iyg)+1
              ixg0=ixg
              iyg0=iyg
              if( ig(ixg,iyg).eq.1 .and.
     &            mod(ixg,lax).eq.0 .and. mod(iyg,lay).eq.0 )then
c                                                   ! draw arrow
                call cgtrar( ui(1)*aspect,ui(2) )
              end if
            end if
          elseif( ig(ixg,iyg).gt.0 )then
            goto 900
          end if
        end if
c         try and check for closed loops
        dpath=dpath+abs(x-xi)+abs(y-yi)
c       if( it.gt.25 .and. abs(x-xtp)+abs(y-ytp).lt. .25*cfla )then
c       if( it.gt.25 .and. ixg.eq.ixg1 .and. iyg.eq.iyg1 )then
        if( it.gt.25 .and. ixg.eq.ixg1 .and. iyg.eq.iyg1 .and.
     &      dpath.gt. 0.05*dxmx  )then
          goto 900
        end if
 200  continue
 900  continue
      xtp=xi
      ytp=yi
      return
      end

