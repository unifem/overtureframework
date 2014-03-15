cTeX begin \listing cgitpl.ftex
      subroutine cgitpl( np,xp,yp,ir,is,kp,up, ng,nv,ndrsab,nrsab,mrsab,
     &          pxy,pkr,prsxy,pu,intopt,rd,id,cgtype,period )
c===================================================================
c
c          Composite Grid Interpolation Routine
c               Component Grid Interpolation
c               ----------------------------
c Input-
c  xp(i),yp(i) i=1,...,np : points to be interpolated
c  ir(i),is(i),kp(i) i=1,...,np : initial guess at nearest point
c  intopt : interpolation options
c       2**1 : use 2nd order interpolation, bi-linear  (only possible)
c       2**3 : use the xyrs array instead of the rsxy array
c  pu(k,n) k=1,ng n=1,nv: pointer to the composite grid functions
c                         to interpolate
c
c Output-
c  up(i,j) i=1,np j=1,nv : interpolated values
c  kp(i) > 0 = component grid from which (xp(i),yp(i)) was interpolated
c        = 0 point could not be interpolated
c        < 0 : point could not be interpolated but was extraploated
c              from component grid -kp(i)
c
c
c Who to Blame:
c   Bill Henshaw April 1988.
c   David Brown  October 1988.
c==================================================================
cTeX end
      parameter( epsi=1.e-3 )
      logical cellvt, cellcn, period(2,ng)
      integer kp(np),pxy(ng),pkr(ng),prsxy(ng),pu(ng,nv),id(*),
     & ndrsab(2,2,ng),nrsab(2,2,ng),ir(np),is(np),mrsab(2,2,ng),
     & cgtype
      real xp(np),yp(np),up(np,nv),rd(*),xyc,rsxyc
c.......local
      logical extrap,dbg
*      data dbg/.false./
      data dbg/.true./
      logical first/.true./
c........start statement functions
      ndr(k)=ndrsab(1,2,k)-ndrsab(1,1,k)+1
      nds(k)=ndrsab(2,2,k)-ndrsab(2,1,k)+1

      ndra(k)=ndrsab(1,1,k)
      ndrb(k)=ndrsab(1,2,k)
      ndsa(k)=ndrsab(2,1,k)
      ndsb(k)=ndrsab(2,2,k)

      nrm(kd,k)   =mrsab(kd,2,k)-mrsab(kd,1,k)+1
      modr(i,kd,k)=mod(i-mrsab(kd,1,k)+nrm(kd,k),nrm(kd,k))
     &                  +mrsab(kd,1,k)

      u(i,j,n,k) =rd(pu(k,n)+i-ndra(k)+ndr(k)*(j-ndsa(k)))
      kr(i,j,k)  =id(pkr(k)+i-ndra(k)+ndr(k)*(j-ndsa(k)))
      xy(i,j,l,k)=rd(pxy(k)+i-ndra(k)+ndr(k)*(j-ndsa(k)
     &                                           +nds(k)*(l-1)))
      xyc(i,j,l,k) = .25*(
     &      xy(i,j,l,k)       + xy(i+1,j,l,k)
     &    + xy(i,j+1,l,k)     + xy(i+1,j+1,l,k))
      rsxy(i,j,l,m,k)=rd(prsxy(k)+i-ndra(k)+ndr(k)*(j-ndsa(k)
     &                          +nds(k)*(l-1+2*(m-1))))
      rsxyc(i,j,l,m,k) = .25*(
     &      rsxy(i,j,l,m,k)     +  rsxy(i+1,j,l,m,k)
     &    + rsxy(i,j+1,l,m,k)   +  rsxy(i+1,j+1,l,m,k))
c........end statement functions

      cellcn = cgtype.eq.2
      cellvt = cgtype.eq.1
      jac=mod(intopt/8,2)
      write(*,'('' CGITPL intopt,jac ='',2i4)') intopt,jac

      if( first )then
        first=.false.
        do k=1,ng
          do i=ndrsab(1,1,k),ndrsab(1,2,k)
            do j=ndrsab(2,1,k),ndrsab(2,2,k)
              write(*,9123) i,j,kr(i,j,k),u(i,j,1,k),
     &         u(i,j,2,k),rsxy(i,j,1,1,k),rsxy(i,j,1,2,k)
            end do
          end do
        end do
      end if
 9123 format(1x,'i,j,kr,u,v,xr,xs = ',3i4,4e8.2)

      ierr=0
      k=min(ng,max(1,kp(1)))
      do 500 ipt=1,np
        ip=ir(ipt)
        jp=is(ipt)
        x=xp(ipt)
        y=yp(ipt)
        kp(ipt)=0
        dist=-1.
        if( dbg ) then
         write(1,*) 'CGITPL Input:'
         write(1,*) '   ipt,xp,yp,=',ipt,x,y
         write(1,*) '   ip,jp,kp  =',ip,jp,kp(ipt)
         write(1,*) '   prsxy     =',(prsxy(kkk),kkk=1,ng)
        end if
c...........find a component grid to interpolate from:
        do 200 kn=1,ng
c         ....find the nearest point, (i,j), to (x,y) on grid k

c         ---Kludge to skip inactive grids
          if( ndra(k).eq.ndrb(k) .and. ndsa(k).eq.ndsb(k) )then
            goto 100  ! unable to interpolate from this grid
          end if

          If (cellvt) Then
            call cgnrst( ndra(k),ndrb(k),ndsa(k),ndsb(k),mrsab(1,1,k),
     &                 rd(pxy(k)),x,y,ip,jp,distmn )
          Else if (cellcn) Then
            call cgnrsc (ndra(k), ndrb(k), ndsa(k), ndsb(k)
     &         , mrsab(1,1,k), rd(pxy(k)), x, y, ip, jp, distmn)
          else
            stop 'CGITPL:ERROR invalid value for cgtype'
          Endif
           if( dbg )then
           write(1,*) 'CGITPL:After CGNRST:'
           write(1,*) '  :ip,jp,k =',ip,jp,k
           write(1,*) '  : kr =',kr(ip,jp,k)
           write(1,*) '  : xy =',xy(ip,jp,1,k),xy(ip,jp,2,k)
           write(1,*) '  : period =',period(1,k),period(2,k)
           end if
          if( kr(ip,jp,k).eq.0 )then
c             ....Unable to interpolate
            goto 100
          end if
c
c.............Iterpolate from the 4 points
c               (ip ,jp1)   (ip1,jp1)
c               (ip ,jp )   (ip1,jp )
c
          If (cellvt) Then
            dx=x-xy(ip,jp,1,k)
            dy=y-xy(ip,jp,2,k)
            if( jac.eq.0 )then
c             ...use rsxy array
              dr=rsxy(ip,jp,1,1,k)*dx+rsxy(ip,jp,1,2,k)*dy
              ds=rsxy(ip,jp,2,1,k)*dx+rsxy(ip,jp,2,2,k)*dy
            else
c             ...rsxy array is really xyrs
              deti=1./(rsxy(ip,jp,1,1,k)*rsxy(ip,jp,2,2,k)-
     &                 rsxy(ip,jp,1,2,k)*rsxy(ip,jp,2,1,k))
              dr=( rsxy(ip,jp,2,2,k)*dx-rsxy(ip,jp,1,2,k)*dy)*deti
              ds=(-rsxy(ip,jp,2,1,k)*dx+rsxy(ip,jp,1,1,k)*dy)*deti
            end if
          Else if (cellcn) Then
            dx=x-xyc(ip,jp,1,k)
            dy=y-xyc(ip,jp,2,k)
            if( jac.eq.0 )then
c             ...use rsxy array
              dr=rsxyc(ip,jp,1,1,k)*dx+rsxyc(ip,jp,1,2,k)*dy
              ds=rsxyc(ip,jp,2,1,k)*dx+rsxyc(ip,jp,2,2,k)*dy
            else
c             ...rsxyc array is really xyrs
              deti=1./(rsxyc(ip,jp,1,1,k)*rsxyc(ip,jp,2,2,k)-
     &                 rsxyc(ip,jp,1,2,k)*rsxyc(ip,jp,2,1,k))
              dr=( rsxyc(ip,jp,2,2,k)*dx-rsxyc(ip,jp,1,2,k)*dy)*deti
              ds=(-rsxyc(ip,jp,2,1,k)*dx+rsxyc(ip,jp,1,1,k)*dy)*deti
            end if
          Else
            write (*,'(''CGITPL: Invalid grid type '',i2)')cgtype
            stop
          Endif
          dr=dr*(nrsab(1,2,k)-nrsab(1,1,k))
          ds=ds*(nrsab(2,2,k)-nrsab(2,1,k))
          dra=min(abs(dr),1.)
          dsa=min(abs(ds),1.)
c...........only use 4 points if dra bigger than epsilon, this lets us
c           interpolate near interpolation boundaries
          if( dra.gt.epsi )then
            ip1=ip+ifix(sign(1.1,dr))
          else
            ip1=ip
          end if
          if( dsa.gt.epsi )then
            jp1=jp+ifix(sign(1.1,ds))
          else
            jp1=jp
          end if
c.............periodic wrap
          if( period(1,k) ) then
            ip1=modr(ip1,1,k)
          end if
          if( period(2,k) ) then
            jp1=modr(jp1,2,k)
          end if

c.............Unable to interpolate if outside the current grid, but
c             extrapolate (to zero order) if this is the closest point
c             so far
          if( ip1.lt.mrsab(1,1,k) .or. ip1.gt.mrsab(1,2,k) .or.
     &        jp1.lt.mrsab(2,1,k) .or. jp1.gt.mrsab(2,2,k) )then
            extrap=.true.
            if( distmn.lt.dist .or.dist.lt.0. )then
              dist=distmn
              if( ip1.lt.mrsab(1,1,k) .or. ip1.gt.mrsab(1,2,k) )then
                ip1=ip
              end if
              if( jp1.lt.mrsab(2,1,k) .or. jp1.gt.mrsab(2,2,k) )then
                jp1=jp
              end if
            else
              goto 100
            end if
          else
            extrap=.false.
          end if

c ... (check to see whether all marked interpolation points are valid)...
          if( kr(ip ,jp,k).eq.0 .or. kr(ip ,jp1,k).eq.0 .or.
     &        kr(ip1,jp,k).eq.0 .or. kr(ip1,jp1,k).eq.0 )then
c           ....Unable to interpolate
            goto 100
          end if

c...........Bi-Linear Interpolation:
          do 50 n=1,nv
            up(ipt,n)=
     &       (1.-dsa)*((1.-dra)*u(ip,jp ,n,k)+dra*u(ip1,jp ,n,k))
     &        +  dsa *((1.-dra)*u(ip,jp1,n,k)+dra*u(ip1,jp1,n,k))
*             if( dbg )then
*             write(1,9000) x,y,up(ipt,n),k,ip,jp,ip1,jp1,dra,dsa,
*      &       u(ip,jp,n,k),u(ip1,jp,n,k),u(ip,jp1,n,k),u(ip1,jp1,n,k)
*             end if
 50       continue
          if( .not.extrap )then
            kp(ipt)=k
          else
            kp(ipt)=-k
          end if
          ir(ipt)=ip
          is(ipt)=jp
 9000 format(2x,'CGITPL: Interpolating x,y, u =',3f10.5,/,
     & 1x,'k,ip,jp,ip1,jp1=',5i4,' dra,dsa =',2f10.4,/,
     & 1x,'u(ip,jp) u(ip1,jp) u(ip,jp1) u(ip1,jp1)=',4f9.5 )
          if( .not.extrap )then
            goto 300
          end if
 100      continue
c         ...try another k
          k=mod(k,ng)+1
 200    continue
c.........Unable to interpolate this point
        if( dbg )then
          write(9,*) 'CGITPL: WARNING unable to interpolate a point'
          write(9,*) '   ipt,xp,yp =',ipt,x,y
        end if
 300   continue
 500  continue
      return
      end
