c
c $Header: /usr/gapps/overture/OvertureRepoCVS/overture/Overture/ogshow/cggi.f,v 1.4 2000/10/04 04:11:56 henshaw Exp $
c
cTeX begin \listing cggi.ftex
c=======================================================================
c This file contains
c CGGI   : CG Grid interpolation
c CGLI   : CG Local Interpolation (new (3D) version)
c CGITPL : CG Local Interpolation (old version)
c CGNRST : Find nearest grid point on a component grid - cell vertex
c CGNRSC : Find nearest grid point on a component grid - cell centre
c CGNEAR : Find nearest grid point on a component grid - 2D/3D version
c CGNEAR1
c CGNEAR2
c=======================================================================
cTeX begin \listing cgnrst.ftex
      subroutine cgnrst( ndra,ndrb,ndsa,ndsb,mrsab,xy,x,y, i,j,distmn )
c==================================================================
c
c Find the nearest grid point (i,j) to the point (x,y)
c Input-
c  ndra,ndrb,ndsa,ndsb,mrsab,xy : component grid descriptors
c  (i,j) - initial guess
c Output-
c  (i,j) - nearest point
c  distmn - minimum distance, L1 norm
c==================================================================
cTeX end
      real xy(ndra:ndrb,ndsa:ndsb,2),x,y
      integer mrsab(2,2)
c Statement functions
      dist(x1,y1,x2,y2)=abs(x1-x2)+abs(y1-y2)

      mra=mrsab(1,1)
      mrb=mrsab(1,2)
      msa=mrsab(2,1)
      msb=mrsab(2,2)

c     Get the starting point for a local search.
      if(i.lt.mra .or. i.gt.mrb )then
        i=.5*(mrb+mra)
       endif
      if(j.lt.msa .or. j.gt.msb )then
        j=.5*(msb+msa)
       endif

      do 70 iter=1,2
c       Do a local search for the closest point
        distmn=dist(x,y,xy(i,j,1),xy(i,j,2))
10      continue
          i1=i
          j1=j
          do 30 j0=max0(j1-1,msa),min0(j1+1,msb)
            do 20 i0=max0(i1-1,mra),min0(i1+1,mrb)
              distij=dist(x,y,xy(i0,j0,1),xy(i0,j0,2))
              if(distij.lt.distmn)then
                distmn=distij
                i=i0
                j=j0
               endif
20           continue
30         continue
        if(i.ne.i1.or.j.ne.j1)goto10


        if(iter.eq.1.and.
     +    (i.eq.mra.or.i.eq.mrb.or.j.eq.msa.or.j.eq.msb)
     +    .and.(distmn.ne.0.) )then
c         The closest point was on the boundary so do a global
c         search for the closest point on the boundary and then
c         another local search from that point.
          i=mra
          j=msa
          distmn=dist(x,y,xy(i,j,1),xy(i,j,2))
          do 40 i0=mra,mrb,mrb-mra
            do 40 j0=msa,msb
              distij=dist(x,y,xy(i0,j0,1),xy(i0,j0,2))
              if(distij.lt.distmn)then
                distmn=distij
                i=i0
                j=j0
               endif
            continue
40        continue
          do 50 j0=msa,msb,msb-msa
            do 50 i0=mra,mrb
              distij=dist(x,y,xy(i0,j0,1),xy(i0,j0,2))
              if(distij.lt.distmn)then
                distmn=distij
                i=i0
                j=j0
               endif
            continue
50        continue
        else
          goto 80
        endif
70    continue
80    continue
      return
      end

cTeX begin \listing cgnrsc.ftex
      subroutine cgnrsc( ndra,ndrb,ndsa,ndsb,mrsab,xy,x,y, i,j,distmn )
c==================================================================
c Find the nearest grid point (i,j) to the point (x,y)
c       CELL CENTERED CASE
c Input-
c  (i,j) - initial guess
c Output-
c  (i,j) - nearest point
c  distmn - minimum distance, L1 norm
c==================================================================
cTeX end
      real xy(ndra:ndrb,ndsa:ndsb,2),x,y
      integer mrsab(2,2)

c Statement functions
      dist(x1,y1,x2,y2)=abs(x1-x2)+abs(y1-y2)
      xyc(i,j,l) = .25*(
     &  xy(i,j,l) + xy(i+1,j,l) + xy(i,j+1,l) + xy(i+1,j+1,l))

      mra=mrsab(1,1)
      mrb=mrsab(1,2)
      msa=mrsab(2,1)
      msb=mrsab(2,2)


c     Get the starting point for a local search.
      if(i.lt.mra .or. i.gt.mrb )then
        i=.5*(mrb+mra)
       endif
      if(j.lt.msa .or. j.gt.msb )then
        j=.5*(msb+msa)
       endif

      do 70 iter=1,2
c       Do a local search for the closest point
        distmn=dist(x,y,xyc(i,j,1),xyc(i,j,2))
10      continue
          i1=i
          j1=j
          do 30 j0=max0(j1-1,msa),min0(j1+1,msb)
            do 20 i0=max0(i1-1,mra),min0(i1+1,mrb)
              distij=dist(x,y,xyc(i0,j0,1),xyc(i0,j0,2))
              if(distij.lt.distmn)then
                distmn=distij
                i=i0
                j=j0
               endif
20           continue
30         continue
        if(i.ne.i1.or.j.ne.j1)goto10


        if(iter.eq.1.and.
     +    (i.eq.mra.or.i.eq.mrb.or.j.eq.msa.or.j.eq.msb)
     +    .and.(distmn.ne.0.) )then
c         The closest point was on the boundary so do a global
c         search for the closest point on the boundary and then
c         another local search from that point.
          i=mra
          j=msa
          distmn=dist(x,y,xyc(i,j,1),xyc(i,j,2))
          do 40 i0=mra,mrb,mrb-mra
            do 40 j0=msa,msb
              distij=dist(x,y,xyc(i0,j0,1),xyc(i0,j0,2))
              if(distij.lt.distmn)then
                distmn=distij
                i=i0
                j=j0
               endif
            continue
40        continue
          do 50 j0=msa,msb,msb-msa
            do 50 i0=mra,mrb
              distij=dist(x,y,xyc(i0,j0,1),xyc(i0,j0,2))
              if(distij.lt.distmn)then
                distmn=distij
                i=i0
                j=j0
               endif
            continue
50        continue
        else
          goto 80
        endif
70    continue
80    continue
      return
      end

      subroutine cgnear(k,nd,xv,iv0,ndra,ndrb,ndsa,ndsb
     + ,ndta,ndtb,mrs,xy,distmn )
c================================================================
c
c  Find the nearest point on a component grid
c
c       Arguments on input:
c         k             Grid number
c         xv(3)         Point at which to invert the grid function
c         iv0(3)        Initial guess (if any)
c         mrs(3,2)  *** Note dimensions ***
c
c       Arguments on output:
c         iv0(3)        Closest point
c         distmn        minimum distance, L1 norm
c
c Who to blame
c   Bill Henshaw
c================================================================
        dimension mrs(3,*),xv(nd)
     +   ,iv0(nd),xy(ndra:ndrb,ndsa:ndsb,ndta:ndtb,*)
     +   ,iv(3),nc(-1:1,-1:1,-1:1),ic1(27,-1:1,-1:1,-1:1)
     +   ,ic2(27,-1:1,-1:1,-1:1),ic3(27,-1:1,-1:1,-1:1)
     +   ,is1(8,-1:1,-1:1),is2(8,-1:1,-1:1),ns(-1:1,-1:1)
c*     +   ,k1112(4),k2212(4),k1211(4),k1222(4)
        logical first
        equivalence(i1,iv(1)),(i2,iv(2)),(i3,iv(3))
        save first,nc,ic1,ic2,ic3,ns,is1,is2
        dist1(x1,      x2      )=abs(x1-x2)
        dist2(x1,y1,   x2,y2   )=abs(x1-x2)+abs(y1-y2)
        dist3(x1,y1,z1,x2,y2,z2)=abs(x1-x2)+abs(y1-y2)+abs(z1-z2)
        data is1
     +   /-1,-1,-1, 0, 1, 0, 0, 0
     +   ,-1, 0, 1, 0, 0, 0, 0, 0
     +   ,-1, 0, 1, 1, 1, 0, 0, 0
     +   ,-1,-1,-1, 0, 0, 0, 0, 0
     +   ,-1, 0, 1,-1, 1,-1, 0, 1
     +   , 1, 1, 1, 0, 0, 0, 0, 0
     +   ,-1,-1,-1, 0, 1, 0, 0, 0
     +   ,-1, 0, 1, 0, 0, 0, 0, 0
     +   ,-1, 0, 1, 1, 1, 0, 0, 0/
        data is2
     +   /-1, 0, 1,-1,-1, 0, 0, 0
     +   ,-1,-1,-1, 0, 0, 0, 0, 0
     +   ,-1,-1,-1, 0, 1, 0, 0, 0
     +   ,-1, 0, 1, 0, 0, 0, 0, 0
     +   ,-1,-1,-1, 0, 0, 1, 1, 1
     +   ,-1, 0, 1, 0, 0, 0, 0, 0
     +   ,-1, 0, 1, 1, 1, 0, 0, 0
     +   , 1, 1, 1, 0, 0, 0, 0, 0
     +   , 1, 1,-1, 0, 1, 0, 0, 0/
        data ns/5,3,5,3,8,3,5,3,5/
c*        data k1112,k2212,k1211,k1222/1,1,1,2,2,2,1,2,1,2,1,1,1,2,2,2/
        data first/.true./


c       Get the starting point for a local search.
        iv(3)=mrs(3,1)
        do kd=1,nd
          if( iv0(kd).ge.mrs(kd,1).and.iv0(kd).le.mrs(kd,2) )then
            iv(kd)=iv0(kd)
          else
            iv(kd)=.5*(mrs(kd,1)+mrs(kd,2))
          endif
        end do

c      Dimension of this grid.
        ngd=nd

c       Now find the closest point on the grid to (xv(1),xv(2),xv(3)):
c        (1) For a 1D grid start at the initial guess and look to the le
c         or to the right depending on whether the distance decreases
c         to the left or right.
c        (2) For a 2D grid first do a local search, use the index arrays
c              (is1,is2) "s=square" to indicate which points in the
c            square to check (not all points need be searched as they
c            would have been done on the previous checks). If the local
c            search ends on a boundary then do a global search of all
c            boundary points, followed by another local search.
c        (3) For a 3D grid proceed as in 2D but use the index arrays
c            (ic1,ic2,ic3) "c=cube" to indicate which points in the
c            cube to search.
c
        if(ngd.eq.0)then
c         Zero dimensional grid.  Do nothing.
         elseif(ngd.eq.1)then
c         One dimensional grid.
          x=xv(1)
          distmn=dist1(x,xy(i1,i2,i3,1))
          if(i1+1.le.mrs(1,2))then
            distij=dist1(x,xy(i1+1,i2,i3,1))
            if(distij.lt.distmn)then
              id1=1
              distmn=distij
              i1=i1+1
              i1a=i1+1
              i1b=mrs(1,2)
             else
              id1=-1
              ia1=i1-1
              i1b=mrs(1,1)
             endif
           elseif(i1-1.ge.mrs(1,1))then
            distij=dist1(x,xy(i1-1,i2,i3,1))
            if(distij.lt.distmn)then
              id1=-1
              distmn=dist1(x,xy(i1-1,i2,i3,1))
              i1=i1-1
              i1a=i1-1
              i1b=mrs(1,1)
             else
c             minimum found, exit
              goto40
             endif
           else
c           Only 1 point on the grid - error.
            write(*,*)
     +       'Fatal error CGNEAR:  Grid has only one point'
           endif

          do 30 ii1=i1a,i1b,id1
            distij=dist1(x,xy(ii1,i2,i3,1))
            if(distij.lt.distmn)then
              distmn=distij
              i1=ii1
             else
              goto40
             endif
30         continue
40        continue

         elseif(ngd.eq.2)then
c         Two dimensional grid.
          x=xv(1)
          y=xv(2)

          do 100 iter=1,2
c           Do a local search for the closest point on grid k.
            distmn=dist2(x,y,xy(i1,i2,i3,1),xy(i1,i2,i3,2))
            i1o=i1
            i2o=i2
50          continue
            id1=i1-i1o
            id2=i2-i2o
            i1o=i1
            i2o=i2
            do 60 ks=1,ns(id1,id2)
              i20=i2o+is2(ks,id1,id2)
              if(i20.ge.mrs(2,1).and.i20.le.mrs(2,2))then
                i10=i1o+is1(ks,id1,id2)
                if(i10.ge.mrs(1,1).and.i10.le.mrs(1,2))then
                  distij=dist2(x,y,xy(i10,i20,i3,1)
     +                            ,xy(i10,i20,i3,2))
                  if(distij.lt.distmn)then
                    distmn=distij
                    i1=i10
                    i2=i20
                   endif
                 endif
               endif
60           continue
            if(i1.ne.i1o.or.i2.ne.i2o)goto50

            if(iter.eq.2.or.
     +       (i1.ne.mrs(1,1).and.i1.ne.mrs(1,2).and.
     +        i2.ne.mrs(2,1).and.i2.ne.mrs(2,2)))goto110
c           The closest point was on the boundary so do a global
c           search for the closest point on the boundary and then
c           another local search from that point.
            call cgnear2(ngd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,mrs,
     +       iv,xv,xy,distmn )
c           Distance to the closest point on the boundary:
            im1=i1
            im2=i2
100        continue
110       continue

         elseif(ngd.eq.3)then
c         Three dimensional grid.
          if(first)then
c           First call, compute indices for 3D searches:
            call cgnear1(nc,ic1,ic2,ic3)
            first=.false.
           endif
          x=xv(1)
          y=xv(2)
          z=xv(3)

          do 190 iter=1,2
c           Do a local search for the closest point on grid k.
            distmn=dist3(x,y,z
     +       ,xy(i1,i2,i3,1),xy(i1,i2,i3,2),xy(i1,i2,i3,3))
            i1o=i1
            i2o=i2
            i3o=i3
120         continue
            id1=i1-i1o
            id2=i2-i2o
            id3=i3-i3o
            i1o=i1
            i2o=i2
            i3o=i3
            do 130 kc=1,nc(id1,id2,id3)
              i30=i3o+ic3(kc,id1,id2,id3)
              if(i30.ge.mrs(3,1).and.i30.le.mrs(3,2))then
                i20=i2o+ic2(kc,id1,id2,id3)
                if(i20.ge.mrs(2,1).and.i20.le.mrs(2,2))then
                  i10=i1o+ic1(kc,id1,id2,id3)
                  if(i10.ge.mrs(1,1).and.i10.le.mrs(1,2))then
                    distij=dist3(x,y,z,   xy(i10,i20,i30,1)
     +               ,xy(i10,i20,i30,2),xy(i10,i20,i30,3))
                    if(distij.lt.distmn)then
                      distmn=distij
                      i1=i10
                      i2=i20
                      i3=i30
                     endif
                   endif
                 endif
               endif
130          continue
            if(i1.ne.i1o.or.i2.ne.i2o.or.i3.ne.i3o)goto120

            if(iter.eq.2.or.
     +       (i1.ne.mrs(1,1).and.i1.ne.mrs(1,2).and.
     +        i2.ne.mrs(2,1).and.i2.ne.mrs(2,2).and.
     +        i3.ne.mrs(3,1).and.i3.ne.mrs(3,2)))goto200

c           The closest point was on the boundary so do a global
c           search for the closest point on the boundary and then
c           another local search from that point.
            call cgnear2(ngd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,mrs,
     +       iv,xv,xy,distmn )
c           Distance to the closest point on the boundary:
            im1=i1
            im2=i2
            im3=i3
190        continue
200       continue

        else
         write(*,*) 'Fatal error in CGNEAR:  '/
     +    /'Invalid component grid dimension'
        endif

      do kd=1,nd
        iv0(kd)=iv(kd)
      end do

      end

      subroutine cgnear1(nc,ic1,ic2,ic3)
c
c
c       Compute the index arrays for three dimensional local searches
c
        dimension nc(-1:1,-1:1,-1:1),ic1(27,-1:1,-1:1,-1:1)
     +       ,ic2(27,-1:1,-1:1,-1:1),ic3(27,-1:1,-1:1,-1:1)

        ks=0
        do 60 id3=-1,1
          do 50 id2=-1,1
            do 40 id1=-1,1
              ns=0
              do 30 i3=-1,1
                do 20 i2=-1,1
                  do 10 i1=-1,1
                    if(.not.(
     +               ((id1.eq.-1.and.(i1.eq.0.or.i1.eq.+1)).or.
     +                 id1.eq.0.or.
     +                (id1.eq.+1.and.(i1.eq.0.or.i1.eq.-1))).and.
     +               ((id2.eq.-1.and.(i2.eq.0.or.i2.eq.+1)).or.
     +                 id2.eq.0.or.
     +                (id2.eq.+1.and.(i2.eq.0.or.i2.eq.-1))).and.
     +               ((id3.eq.-1.and.(i3.eq.0.or.i3.eq.+1)).or.
     +                 id3.eq.0.or.
     +                (id3.eq.+1.and.(i3.eq.0.or.i3.eq.-1))))
     +               .or.(id1.eq.0.and.id2.eq.0.and.id3.eq.0
     +                  .and.(i1.ne.0.or.i2.ne.0.or.i3.ne.0)))then
                      ns=ns+1
                      ic1(ns,id1,id2,id3)=i1
                      ic2(ns,id1,id2,id3)=i2
                      ic3(ns,id1,id2,id3)=i3
                     endif
10                 continue
20               continue
30             continue
              ks=ks+1
              nc(id1,id2,id3)=ns
40           continue
50         continue
60       continue
       end

      subroutine cgnear2(ngd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,mrs,
     &  iv,xv,xy,distmn )
c
c       Find the closest boundary point on the grid to the point
c       (xv(1),xv(2),xv(3)) and return the index of this point
c       (iv(1),iv(2),iv(3)) and the minimum distance, distmn.
c
        integer iv(*),kra3(3,2,3,2),mrs(3,2)
        real xy(ndra:ndrb,ndsa:ndsb,ndta:ndtb,*),xv(*)
        dist2(x1,y1,x2,y2)=abs(x1-x2)+abs(y1-y2)
        dist3(x1,y1,z1,x2,y2,z2)=abs(x1-x2)+abs(y1-y2)+abs(z1-z2)
        data kra3
     +   /1,1,1,2,1,1 ,1,1,1,1,2,1 ,1,1,1,1,1,2
     +   ,1,2,2,2,2,2 ,2,1,2,2,2,2 ,2,2,1,2,2,2/

c*** ?? Initialize distmn ?

      do kd=1,ngd
        do ks=1,2
c                 Search the side.

          do i30=mrs(3,kra3(kd,ks,3,1))
     +          ,mrs(3,kra3(kd,ks,3,2))
            do i20=mrs(2,kra3(kd,ks,2,1))
     +            ,mrs(2,kra3(kd,ks,2,2))
              do i10=mrs(1,kra3(kd,ks,1,1))
     +              ,mrs(1,kra3(kd,ks,1,2))
                if(ngd.eq.2)then
                  distij=dist2(xv(1),xv(2),xy(i10,i20,i30,1)
     +                     ,xy(i10,i20,i30,2))
                else
                  distij=dist3(xv(1),xv(2),xv(3)
     +             ,xy(i10,i20,i30,1)
     +             ,xy(i10,i20,i30,2)
     +             ,xy(i10,i20,i30,3))
                endif
                if(distij.lt.distmn)then
                  distmn=distij
                  iv(1)=i10
                  iv(2)=i20
                  iv(3)=i30
                endif
              end do
            end do
          end do
        end do
      end do

      end

