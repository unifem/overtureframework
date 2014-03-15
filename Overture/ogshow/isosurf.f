      subroutine isosurf(nt, s, ndd,nd,d,
     *                   i,j,k,
     *                   nc,c,
     *                   npv,px,
     *                   ierr)

c         nt                 the number of tetrahedra to check (kkc 040316)
c         s                  scalar field at vertices
c         ndd                leading dimension of d and px
c         nd                 number of data components,
c         d(nd,0:1,0:1,0:1)            data at vertices
c         i,j,k              corner indices
c         nc                 number of contour levels,
c         c(nc)              contour levels
c         npv(5,nc)          number of vertices for pg_i
c                               npv= 3 triangle
c                               npv= 4 planar quadrilateral
c                               npv=-4 tetrahedron
c                               npv=-1 point
c                               npv=-2 line
c         px(nd,4,5,nc)      vertex list
c         ierr               returns 0 if isosurf was successful, nonzero otherwise

      integer nt
      real    s(0:1,0:1,0:1)
      real    d(ndd,0:1,0:1,0:1)
      integer i,j,k
      real    c(nc)
      integer npv(5,nc)
      real    px(ndd,4,5,nc)
      integer ierr

      integer ifx,ify,ifz,idebug
c       ni = max number of intersections with 1 tetrahedron
c            and 6 edges of each tetrahedra and 2 end points
c         12 = 6*2
      parameter( ni=12 )
      real    s0(18)
      real    s1(18)
      integer iflag(18)
      real    tt(ni)
      real    x(ni),y(ni),z(ni)
      integer ix0(ni),iy0(ni),iz0(ni)
      integer ix1(ni),iy1(ni),iz1(ni)

c
c   iedge(*,i) stores the corner numbers of the endpoints of edge i
c
      integer iedge(2,18)
c
c   ipt(*,i,ifx,ify,ifz) stores the (dx,dy,dz) displacements of corner
c            i ( (ifx,ify,ifz) are "flips")
c
      integer ipt(3,8,0:1,0:1,0:1)
c
c  ite(*,i) stores the six edges of tetrahedron i
c
      integer ite(6,5)
c
c  itp(*,i) stores the four corners of tetrahedron i
c
      integer itp(4,5)

      logical degenerate

      data    iedge/1,2,
     *              2,3,
     *              3,4,
     *              1,4,
     *              2,4,
     *              1,5,
     *              2,6,
     *              3,7,
     *              4,8,
     *              2,5,
     *              2,7,
     *              4,5,
     *              4,7,
     *              5,6,
     *              6,7,
     *              7,8,
     *              8,5,
     *              5,7/
      data    ((ipt(i,j,0,0,0),i=1,3),j=1,8)/
     *            0,0,0,
     *            1,0,0,
     *            1,1,0,
     *            0,1,0,
     *            0,0,1,
     *            1,0,1,
     *            1,1,1,
     *            0,1,1/
      data    ((ipt(i,j,1,0,0),i=1,3),j=1,8)/
     *            1,0,0,
     *            0,0,0,
     *            0,1,0,
     *            1,1,0,
     *            1,0,1,
     *            0,0,1,
     *            0,1,1,
     *            1,1,1/
      data    ((ipt(i,j,0,1,0),i=1,3),j=1,8)/
     *            0,1,0,
     *            1,1,0,
     *            1,0,0,
     *            0,0,0,
     *            0,1,1,
     *            1,1,1,
     *            1,0,1,
     *            0,0,1/
      data    ((ipt(i,j,1,1,0),i=1,3),j=1,8)/
     *            1,1,0,
     *            0,1,0,
     *            0,0,0,
     *            1,0,0,
     *            1,1,1,
     *            0,1,1,
     *            0,0,1,
     *            1,0,1/
      data    ((ipt(i,j,0,0,1),i=1,3),j=1,8)/
     *            0,0,1,
     *            1,0,1,
     *            1,1,1,
     *            0,1,1,
     *            0,0,0,
     *            1,0,0,
     *            1,1,0,
     *            0,1,0/
      data    ((ipt(i,j,1,0,1),i=1,3),j=1,8)/
     *            1,0,1,
     *            0,0,1,
     *            0,1,1,
     *            1,1,1,
     *            1,0,0,
     *            0,0,0,
     *            0,1,0,
     *            1,1,0/
      data    ((ipt(i,j,0,1,1),i=1,3),j=1,8)/
     *            0,1,1,
     *            1,1,1,
     *            1,0,1,
     *            0,0,1,
     *            0,1,0,
     *            1,1,0,
     *            1,0,0,
     *            0,0,0/
      data    ((ipt(i,j,1,1,1),i=1,3),j=1,8)/
     *            1,1,1,
     *            0,1,1,
     *            0,0,1,
     *            1,0,1,
     *            1,1,0,
     *            0,1,0,
     *            0,0,0,
     *            1,0,0/
      data    ite/ 1,    5,   4,     6,   10,   12,
     *             2,    3,   5,    11,    8,   13,
     *            18,   16,   17,   12,   13,    9,
     *            14,   15,   18,   10,    7,   11,
     *            12,   18,   13,    5,   10,   11/

      data    itp/ 1,2,4,5,
     *             2,3,4,7,
     *             4,5,7,8,
     *             5,6,7,2,
     *             2,4,5,7/

      idebug=0
      ierr = 0

      cmin=c(1)
      cmax=c(1)
      do ic=1,nc
         cmax=max(cmax,c(ic))
         cmin=min(cmin,c(ic))
      enddo

      ifz=mod(abs(k),2)
      ify=mod(abs(j),2)
      ifx=mod(abs(i),2)

      smaxcube=s(0,0,0)
      smincube=smaxcube
      do icrnr=1,8
         st=s(ipt(1,icrnr,ifx,ify,ifz),
     *        ipt(2,icrnr,ifx,ify,ifz),
     *        ipt(3,icrnr,ifx,ify,ifz))
         smaxcube=max(smaxcube,st)
         smincube=min(smincube,st)
      enddo

c Check if max of all levels is less than min over cube
c    or if min of all levels is greater than max over cube

      if(cmax.lt.smincube.or.cmin.gt.smaxcube)then
         do ic=1,nc
            do it=1,5
               npv(it,ic)=0
            enddo
         enddo
         return
      endif

c There are a total of 18 edges in the tetrahedra in the cube

      do ie=1,18
          s0(ie)=s(ipt(1,iedge(1,ie),ifx,ify,ifz),
     *             ipt(2,iedge(1,ie),ifx,ify,ifz),
     *             ipt(3,iedge(1,ie),ifx,ify,ifz))
          s1(ie)=s(ipt(1,iedge(2,ie),ifx,ify,ifz),
     *             ipt(2,iedge(2,ie),ifx,ify,ifz),
     *             ipt(3,iedge(2,ie),ifx,ify,ifz))
      enddo
c
c  Loop over each contour level
c
      do 2 ic=1,nc
c
c  Quick Discard of all tetrahedra for level
c
c Check if max of this contour level is less than min over cube
c    or if min of this contour level is greater than smax over cube

         if(c(ic).lt.smincube.or.c(ic).gt.smaxcube)then
            do it=1,5
               npv(it,ic)=0
            enddo
            go to 2
         endif
c
c     Loop over each tetrahedron
c
          do 1 it=1,nt
            npv(it,ic)=0

            ss1=s(ipt(1,itp(1,it),ifx,ify,ifz),
     *            ipt(2,itp(1,it),ifx,ify,ifz),
     *            ipt(3,itp(1,it),ifx,ify,ifz))
            ss2=s(ipt(1,itp(2,it),ifx,ify,ifz),
     *            ipt(2,itp(2,it),ifx,ify,ifz),
     *            ipt(3,itp(2,it),ifx,ify,ifz))
            ss3=s(ipt(1,itp(3,it),ifx,ify,ifz),
     *            ipt(2,itp(3,it),ifx,ify,ifz),
     *            ipt(3,itp(3,it),ifx,ify,ifz))
            ss4=s(ipt(1,itp(4,it),ifx,ify,ifz),
     *            ipt(2,itp(4,it),ifx,ify,ifz),
     *            ipt(3,itp(4,it),ifx,ify,ifz))
            smax=max(ss1,ss2,ss3,ss4)
            smin=min(ss1,ss2,ss3,ss4)

c Check if max of this contour level is less than min over tetrahedron
c   or if min of this contourlevel is greater than smax over tetrahedron

            if(c(ic).lt.smin.or.c(ic).gt.smax)go to 1

            degenerate=.false.
            n=0
c
c        Loop over each edge of tetrahedron, computing crossings
c
            do ien=1,6
               ie=ite(ien,it)
               if(abs(s1(ie)-s0(ie)).gt.1.e-7)then
                  t=(c(ic)-s0(ie))/(s1(ie)-s0(ie))
                  if(abs(t-.5).le..5)then
                     if(abs(t).lt..01)degenerate=.true.
                     if(abs(t-1.).lt..01)degenerate=.true.
                     ic1=iedge(1,ie)
                     ic2=iedge(2,ie)
                     n=n+1
                     if( n.gt.ni )then
                       write(*,'('' ISOSURF:ERROR n ='',i3,'' ni'')') n
c                      kkc 080111 we now return an error instead of overwritting memory
                       ierr = 1
                       return
                     end if
                     tt(n)=t
                     ix0(n)=ipt(1,ic1,ifx,ify,ifz)
                     iy0(n)=ipt(2,ic1,ifx,ify,ifz)
                     iz0(n)=ipt(3,ic1,ifx,ify,ifz)
                     ix1(n)=ipt(1,ic2,ifx,ify,ifz)
                     iy1(n)=ipt(2,ic2,ifx,ify,ifz)
                     iz1(n)=ipt(3,ic2,ifx,ify,ifz)
                     iflag(n)=0
                  endif
               else
                  if(abs(s0(ie)-c(ic)).lt.1.e-7)then
                     degenerate=.true.
                     ic1=iedge(1,ie)
                     ic2=iedge(2,ie)

                     n=n+1
                     if( n.gt.ni )then
                       write(*,'('' ISOSURF:ERROR n ='',i3,'' ni'')') n
c                      kkc 080111 we now return an error instead of overwritting memory
                       ierr = 1
                       return
                     end if
                     tt(n)=0.
                     ix0(n)=ipt(1,ic1,ifx,ify,ifz)
                     iy0(n)=ipt(2,ic1,ifx,ify,ifz)
                     iz0(n)=ipt(3,ic1,ifx,ify,ifz)
                     ix1(n)=ipt(1,ic2,ifx,ify,ifz)
                     iy1(n)=ipt(2,ic2,ifx,ify,ifz)
                     iz1(n)=ipt(3,ic2,ifx,ify,ifz)
                     iflag(n)=0

                     n=n+1
                     if( n.gt.ni )then
                       write(*,'('' ISOSURF:ERROR n ='',i3,'' ni'')') n
c                      kkc 080111 we now return an error instead of overwritting memory
                       ierr = 1
                       return
                     end if
                     tt(n)=1.
                     ix0(n)=ipt(1,ic1,ifx,ify,ifz)
                     iy0(n)=ipt(2,ic1,ifx,ify,ifz)
                     iz0(n)=ipt(3,ic1,ifx,ify,ifz)
                     ix1(n)=ipt(1,ic2,ifx,ify,ifz)
                     iy1(n)=ipt(2,ic2,ifx,ify,ifz)
                     iz1(n)=ipt(3,ic2,ifx,ify,ifz)
                     iflag(n)=0
                  endif
               endif
            enddo
c
c        Process crossings
c

            if(degenerate)then
c
c           Remove duplicates
c
               do i1=1,n-1
                  if(iflag(i1).eq.0)then
                     x1=(1-tt(i1))*ix0(i1)+tt(i1)*ix1(i1)
                     y1=(1-tt(i1))*iy0(i1)+tt(i1)*iy1(i1)
                     z1=(1-tt(i1))*iz0(i1)+tt(i1)*iz1(i1)
                     do i2=i1+1,n
                        x2=(1-tt(i2))*ix0(i2)+tt(i2)*ix1(i2)
                        y2=(1-tt(i2))*iy0(i2)+tt(i2)*iy1(i2)
                        z2=(1-tt(i2))*iz0(i2)+tt(i2)*iz1(i2)
                        if(abs(x1-x2)+abs(y1-y2)+abs(z1-z2).lt.1.e-5)
     *                                                       iflag(i2)=1
                     enddo
                  endif
               enddo
c
c Compress in place
c
               ito=0
               do i1=1,n
                  if(iflag(i1).eq.0)then
                    ito=ito+1
                    tt(ito)=tt(i1)
                    ix0(ito)=ix0(i1)
                    iy0(ito)=iy0(i1)
                    iz0(ito)=iz0(i1)
                    ix1(ito)=ix1(i1)
                    iy1(ito)=iy1(i1)
                    iz1(ito)=iz1(i1)
                    iflag(ito)=iflag(i1)
                  endif
               enddo
               n=ito
            endif
c
c        If there are three it's simple
c
            if(n.eq.3)then
              npv(it,ic)=3
c
c        If there are four there are two cases
c
            elseif(n.eq.4)then
               do in=1,4
                  x(in)=(1.-tt(in))*ix0(in)+tt(in)*ix1(in)
                  y(in)=(1.-tt(in))*iy0(in)+tt(in)*iy1(in)
                  z(in)=(1.-tt(in))*iz0(in)+tt(in)*iz1(in)
               enddo
               a=(x(2)-x(1))*
     *                ((y(3)-y(1))*(z(4)-z(1))-(y(4)-y(1))*(z(3)-z(1)))
     *          -(y(2)-y(1))*
     *                ((x(3)-x(1))*(z(4)-z(1))-(x(4)-x(1))*(z(3)-z(1)))
     *          +(z(2)-z(1))*
     *                ((x(3)-x(1))*(y(4)-y(1))-(x(4)-x(1))*(y(3)-y(1)))
c
c             Not coplanar points, plot faces of tetrahedron
c
               if(abs(a).gt.1.e-5)then
                  npv(it,ic)=-4
               else
c
c             Coplanar points, reorder and draw
c
                  a23a34=
     *                ((y(3)-y(1))*(z(4)-z(1))-(y(4)-y(1))*(z(3)-z(1)))*
     *                ((y(2)-y(1))*(z(3)-z(1))-(y(3)-y(1))*(z(2)-z(1)))
     *               +((x(3)-x(1))*(z(4)-z(1))-(x(4)-x(1))*(z(3)-z(1)))*
     *                ((x(2)-x(1))*(z(3)-z(1))-(x(3)-x(1))*(z(2)-z(1)))
     *               +((x(3)-x(1))*(y(4)-y(1))-(x(4)-x(1))*(y(3)-y(1)))*
     *                ((x(2)-x(1))*(y(3)-y(1))-(x(3)-x(1))*(y(2)-y(1)))

*       write(*,'('' ISOSURF n='',i2,'' a23a34 ='',e12.4)') n,a23a34
                  if(a23a34.lt.0.)then

c  flip intersections 3 and 4

                     ttt=tt(4)
                     tt(4)=tt(3)
                     tt(3)=ttt
                     itt=ix0(4)
                     ix0(4)=ix0(3)
                     ix0(3)=itt
                     itt=iy0(4)
                     iy0(4)=iy0(3)
                     iy0(3)=itt
                     itt=iz0(4)
                     iz0(4)=iz0(3)
                     iz0(3)=itt
                     itt=ix1(4)
                     ix1(4)=ix1(3)
                     ix1(3)=itt
                     itt=iy1(4)
                     iy1(4)=iy1(3)
                     iy1(3)=itt
                     itt=iz1(4)
                     iz1(4)=iz1(3)
                     iz1(3)=itt
                  endif
                  npv(it,ic)=4
               endif
            elseif( n.eq.1 .or.n.eq.2 )then
               npv(it,ic)=-n
            else
              if( idebug.gt.0 )then
                write(*,'('' ISOSURF:ERROR - there are n='',i3,'//
     &         ''' distinct intersections with 1 tetrahedon'')') n
                 npv(it,ic)=0
              end if
c     kkc 080111 we now return an error instead of overwritting memory
              ierr = 1
              return
            endif

            do in=1,n
             do id=1,nd
               px(id,in,it,ic)=(1.-tt(in))*d(id,ix0(in),iy0(in),iz0(in))
     *                            +tt(in) *d(id,ix1(in),iy1(in),iz1(in))
             enddo
            enddo

    1     continue
    2    continue

      return
      end
