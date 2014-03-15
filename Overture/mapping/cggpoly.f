      subroutine cprg( r,s,x,y,xr,xs,yr,ys,
     &                 ndiwk,iwk, ndrwk,rwk, ierr )
c==========================================================
c    Grid on a Curved Polygonal Region
c    ---------------------------------
c  Input -
c   r,s   : evaluate grid at these values
c   ndiwk,iwk,ndrwk,rwk : arrays initialized by cprgi
c  Output -
c   x,y,xr,xs,yr,ys
c
c Bill Henshaw, March 1988.
c==========================================================
      integer iwk(ndiwk)
      real rwk(ndrwk)

      ierr=0
c........evaluate:
      ndi=iwk(1)
      ndr=iwk(2)
      if( ndi.le.0 .or. ndr.le.0 )then
        stop 'CPRG:ERROR: invalid values in iwk(1) or iwk(2)'
      end if
      call cprs( r,s,x,y,xr,xs,yr,ys, iwk(3),rwk(iwk(4)),rwk(iwk(4)),
     &   ndi,iwk(5+2*ndi),iwk(5+3*ndi),iwk(5+4*ndi),
     &   ndr,rwk(1+2*ndr),rwk(1+3*ndr),rwk(1+4*ndr),
     &   rwk(1+5*ndr) )
      return
      end



c====================================================================
c       CPR - Grids for Curved Polygonal Regions
c       ----------------------------------------
c
c  This file contains subroutines to generate a boundary fitted
c  grid next to a curve which is a polygon with smooth out corners
c
c  CPINIT - initialization of a curved polygonal region
c  CPR    - evaluate the grid at (r,s)
c  CPRDS  - called by cpr
c  CP     - called by cpr
c  CPRSI  - initialize the spline which is fitted to the grid
c  CPRS   - evaluate the spline fitted grid at (r,s)
c
c-------------------------------------------------------------------

      subroutine cpinit( ndi,iwx,iwy,iwr,iws,iwr1,
     &                   ndr,rwx,rwy,rwr,rws,rwr1,ccor,
     &  nc,sc,xc,yc,bv,  sab, r12b, nsr,sr, ndwk,wk,
     &  x00,x01,x10,x11, y00,y01,y10,y11,iccor,per )
c==========================================================
c   Initialization Routine for CPR
c
c
c INPUT -
c  iwx,iwy,iwr,iws,iwr1 : Integer work arrays of dimension ndi,
c  ndi                  : ndi >= 5
c  rwx,rwy,rwr,rws,rwr1 : Real work arrays of dimension ndr,
c  ndr                  : ndr >= 3*nc
c  ccor                 : real array of dimension 20
c  per   : 0= curve not periodic in s
c          1= derivative periodic in s
c          2= curve is periodic in s
c OUTPUT -
c  everything : To be used in calls to CPR and CPRSI
c
c Who to blame: Bill Henshaw
c==========================================================
      integer iwx(ndi),iwy(ndi),iwr(ndi),iws(ndi),iwr1(ndi),per
      real rwx(ndr),rwy(ndr),rwr(ndr),rws(ndr),rwr1(ndi)
      real ccor(20)
      real xc(nc),yc(nc),bv(nc),sc(nc)
      real sab(2,nc),r12b(3,nc),sr(3,nsr)
      real wk(ndwk)
      real*4 r1mach
      real*8 d1mach

      if( ndi .lt. 5 )then
        write(*,'('' CPINIT:ERROR ndi < 5'')')
        stop
      end if
      if( ndr .lt. 3*nc )then
        write(*,'('' CPINIT:ERROR ndr < 3*nc '')')
        stop
      end if
c.........compute normalized arclengths
      sc(1)=0.
      do 100 i=2,nc
        sc(i)=sc(i-1)+sqrt((xc(i)-xc(i-1))**2+(yc(i)-yc(i-1))**2)
 100  continue
      do 200 i=2,nc
        sc(i)=sc(i)/sc(nc)
 200  continue

c.......assign parameters for stretching functions
      ioptx=3  !  iopt=3 : (=1+2) do not compute a spline; specify r0,r1
      iwx(1)=0           ! no layer functions
      iwx(2)=nc-1        ! number of ramp functions
      iwx(3)=min(1,per)  ! 1=periodic
      iwx(4)=0
      do 300 i=1,nc-1
        rwx(3*i-2)=(xc(i+1)-xc(i))/(sc(i+1)-sc(i))   ! slope of ramp
        rwx(3*i-1)=bv(i)                             ! transition expon.
        rwx(3*i  )=sc(i)                             ! start of slope
 300  continue
c.........Boundary conditions:
      if( per.eq.0 )then
c       non-periodic
        rwx(3)=-1.               ! f(1) : first ramp starts at s=-1
        rwx(3*(nc-1)+1)=xc(1)    ! r0 - origin
        rwx(3*(nc-1)+2)=1.       ! r1 - scale factor
        rwx(3*(nc-1)+3)=2.       ! f(nc-1) : last ramp ends at s=2
      else
        rwx(3)=0.                ! f(1) : first ramp starts at s=-1
        rwx(3*(nc-1)+1)=xc(1)    ! r0 - origin
        rwx(3*(nc-1)+2)=1.       ! r1 - scale factor
        rwx(3*(nc-1)+3)=1.       ! f(nc-1) :
      end if

      iopty=3  !  iopt=3 : (=1+2) do not compute a spline; specify r0,r1
      iwy(1)=0
      iwy(2)=nc-1
      iwy(3)=min(1,per)  ! 1=periodic
      iwy(4)=0
      do 400 i=1,nc-1
        rwy(3*i-2)=(yc(i+1)-yc(i))/(sc(i+1)-sc(i))
        rwy(3*i-1)=bv(i)
        rwy(3*i  )=sc(i)
 400  continue
c.........BC's
      if( per.eq.0 )then
c       non-periodic
        rwy(3)=-1.
        rwy(3*(nc-1)+1)=yc(1)
        rwy(3*(nc-1)+2)=1.
        rwy(3*(nc-1)+3)=2.
      else
        rwy(3)=0.
        rwy(3*(nc-1)+1)=yc(1)
        rwy(3*(nc-1)+2)=1.
        rwy(3*(nc-1)+3)=1.
      end if

c*wdh write(1,9100) nc,(i,sc(i),bv(i),
c*wdh&                rwy(3*i-2),rwy(3*i-1),rwy(3*i),i=1,nc)
 9100 format(1x,'CPINIT: nc=',i3,/,
     & 1x,' i    sc(i)   bv(i)   rwy(3*i-2)  rwy(3*i-1)  rwy(3*i)',/,
     & (1x,i3,5f11.4))

c..........stretching in s direction around corners
      ic0=2
      ic1=nc-1
      if( sab(1, 1).ne.0. .and. sab(2, 1).ne.0. )then
        ic0=1
      end if
      if( sab(1,nc).ne.0. .and. sab(2,nc).ne.0. )then
        ic1=nc
      end if
      nnc=ic1-ic0+1
      iws(1)=nnc
      iws(2)=0
      iws(3)=min(1,per)  ! 1=periodic
      iws(4)=0
      do 500 i=ic0,ic1
        rws(3*(i-ic0)+1)=sab(1,i)
        rws(3*(i-ic0)+2)=sab(2,i)
        rws(3*(i-ic0)+3)=sc(i)
 500  continue
      rws(3*(ic1+1-ic0)+1)=0.
      rws(3*(ic1+1-ic0)+2)=0.
      rws(3*(ic1+1-ic0)+3)=0.

c     ---stretching of radius
      if( per.eq.0 )then
        ntanh=nc-2
      else
        ntanh=nc-1
        r12b(1,nc)=r12b(1,1)
      end if
      ioptr=3
      iwr(1)=ntanh  ! number of tanh's
      iwr(2)=nc-1   ! number of ramps
      iwr(3)=min(1,per)  ! 1=periodic
      iwr(4)=0
      do i=1,ntanh
        rwr(3*i-2)=r12b(1,i+1)-r12b(2,i)
        rwr(3*i-1)=r12b(3,i)
        rwr(3*i  )=sc(i+1)
      end do
      j=ntanh
      do i=1,nc-1
        j=j+1
        rwr(3*j-2)=(r12b(2,i)-r12b(1,i))/(sc(i+1)-sc(i))
        rwr(3*j-1)=r12b(3,i)
        rwr(3*j  )=sc(i)
      end do
c     ...BC's:
      if( per.eq.0 )then
c       non-periodic
c***    rwr(3*(nc-1))=-1.     ! first ramp starts here
        rwr(3*(ntanh+1))=-1.     ! first ramp starts here
        rwr(3*j+1)=r12b(1,1)  ! r0: radius at vertex 1
        rwr(3*j+2)=1.         ! r1: scale factor
        rwr(3*j+3)=2.         ! last ramp ends here
      else
        rwr(3*(ntanh+1))=0.
        rwr(3*j+1)=r12b(1,1)
        rwr(3*j+2)=1.
        rwr(3*j+3)=1.
      end if

c..........stretching in r direction
      iwr1(1)=nsr
      iwr1(2)=0
      iwr1(3)=0
      iwr1(4)=0
      do 800 i=1,nsr
        rwr1(3*i-2)=sr(1,i)
        rwr1(3*i-1)=sr(2,i)
        rwr1(3*i  )=sr(3,i)
 800  continue
      rwr1(3*(nsr+1)-2)=0.
      rwr1(3*(nsr+1)-1)=0.
      rwr1(3*(nsr+1)  )=0.

c.......initialize stretching functions
      call stinit( ndi,iwr,ndr,rwr,ioptr,ndwk,wk,ierr )
      if( ierr.gt.0 )then
        stop 'CPINIT: Error return from STINIT (r)'
      end if
      call stinit( ndi,iws,ndr,rws,0,ndwk,wk,ierr )
      if( ierr.gt.0 )then
        stop 'CPINIT: Error return from STINIT (s)'
      end if
      call stinit( ndi,iwx,ndr,rwx,ioptx,ndwk,wk,ierr )
      if( ierr.gt.0 )then
        stop 'CPINIT: Error return from STINIT (x)'
      end if
      call stinit( ndi,iwy,ndr,rwy,iopty,ndwk,wk,ierr )
      if( ierr.gt.0 )then
        stop 'CPINIT: Error return from STINIT (y)'
      end if
      call stinit( ndi,iwr1,ndr,rwr1,0,ndwk,wk,ierr )
      if( ierr.gt.0 )then
        stop 'CPINIT: Error return from STINIT (r1)'
      end if

      do 900 ic=1,20
        ccor(ic)=0.
 900  continue

      ccor(18)=iccor
      if( iccor.eq.1 )then
c       Correct for corners:
c       Add a mapping which puts the corners where they "should" be
c       x_new(r,s) = x_old(r,s) + ccor(1)*r+ccor(2)*s+ccor(3)*r*s+ccor(4)
c       y_new(r,s) = y_old(r,s) + ccor(5)*r+ccor(6)*s+ccor(7)*r*s+ccor(8)
c
c       Choose ccor(k) k=1,...,8 so that
c          x_new(0,0)=x00 x_new(0,1)=x01 x_new(1,0)=x10 x_new(1,1)=x11
c          y_new(0,0)=y00 y_new(0,1)=y01 y_new(1,0)=y10 y_new(1,1)=y11
c
        call cpr( 0.,0.,xc00,yc00,xr,xs,yr,ys,ndi,iwx,iwy,iwr,iws,iwr1,
     &                ndr,rwx,rwy,rwr,rws,rwr1,ccor )
        call cpr( 0.,1.,xc01,yc01,xr,xs,yr,ys,ndi,iwx,iwy,iwr,iws,iwr1,
     &                ndr,rwx,rwy,rwr,rws,rwr1,ccor )
        call cpr( 1.,0.,xc10,yc10,xr,xs,yr,ys,ndi,iwx,iwy,iwr,iws,iwr1,
     &                ndr,rwx,rwy,rwr,rws,rwr1,ccor )
        call cpr( 1.,1.,xc11,yc11,xr,xs,yr,ys,ndi,iwx,iwy,iwr,iws,iwr1,
     &                ndr,rwx,rwy,rwr,rws,rwr1,ccor )
        ccor(4)=x00-xc00
        ccor(8)=y00-yc00
        ccor(2)=x01-xc01-ccor(4)
        ccor(6)=y01-yc01-ccor(8)
        ccor(1)=x10-xc10-ccor(4)
        ccor(5)=y10-yc10-ccor(8)
        ccor(3)=x11-xc11-(ccor(1)+ccor(2)+ccor(4))
        ccor(7)=y11-yc11-(ccor(5)+ccor(6)+ccor(8))
c.........save the corners too
        ccor(10)=x00
        ccor(11)=x01
        ccor(12)=x10
        ccor(13)=x11
        ccor(14)=y00
        ccor(15)=y01
        ccor(16)=y10
        ccor(17)=y11

c.........save a the machine epsilon * 100
c     .... get eps=r1mach(3) = smallest relative spacing
c      IBM Single Prec.: eps=.596046E-07
      eps=r1mach(3)
c** The following fudge tries to detect whether the code has been
c   compiled with an automatic double precision option in which
c   case we want to call D1MACH
c      IBM Double Prec.:   eps=.1388-16
        oneps=1.+eps/100.
        if( oneps.ne.1. )then
          eps=d1mach(3)
        end if

        ccor(9)=eps*100.
      end if
      return
      end

      subroutine cpr( r,s,x,y,xr,xs,yr,ys, ndi,iwx,iwy,iwr,iws,iwr1,
     &                ndr,rwx,rwy,rwr,rws,rwr1,ccor )
c==========================================================
c      Curved Polygonal Region
c      -----------------------
c Evaluate the grid defined with CPINIT
c
c INPUT
c  r,s  : evalute the grid at r,s
c  ndi,iwx,iwy,... : parameters and arrays generated by CPINIT
c OUTPUT
c  x,y, xr,xs,yr,ys : position and derivatives at (r,s)
c
c WARNING: xs and ys are not computed that accurately
c
c Who to blame: Bill Henshaw
c==========================================================
      integer iwx(ndi),iwy(ndi),iwr(ndi),iws(ndi)
      real rwx(ndr),rwy(ndr),rwr(ndr),rws(ndr),ccor(*)

      call strt( s,s0,s0s, iws,rws,ierr )
      call sttr( s0,r0,r0s0, iwr,rwr,ierr )
      call strt( r,r1,r1r, iwr1,rwr1,ierr )
      call cp( s0,x0,y0,x0s0,y0s0, ndi,iwx,iwy,ndr,rwx,rwy )
      x0s=x0s0*s0s
      y0s=y0s0*s0s
      rr0=r1*r0
      rr0s=r1*r0s0*s0s
      d=sqrt(x0s**2+y0s**2)
*       x=x0+rr0*y0s/d + ccor(1)*r+ccor(2)*s+ccor(3)*r*s+ccor(4)
*       y=y0-rr0*x0s/d + ccor(5)*r+ccor(6)*s+ccor(7)*r*s+ccor(8)
      x=x0-rr0*y0s/d + ccor(1)*r+ccor(2)*s+ccor(3)*r*s+ccor(4)
      y=y0+rr0*x0s/d + ccor(5)*r+ccor(6)*s+ccor(7)*r*s+ccor(8)
c.........need xss and yss
      ds=.001
c.......c4 commented out 4'th order approximation
c4    call cprds( r,s-2.*ds,x0sm2,y0sm2, ndi,iwx,iwy,iwr,iws,iwr1,
c4   &                ndr,rwx,rwy,rwr,rws,rwr1 )
      call cprds( r,s-ds,x0sm1,y0sm1, ndi,iwx,iwy,iwr,iws,iwr1,
     &                ndr,rwx,rwy,rwr,rws,rwr1 )
      call cprds( r,s+ds,x0sp1,y0sp1, ndi,iwx,iwy,iwr,iws,iwr1,
     &                ndr,rwx,rwy,rwr,rws,rwr1 )
c4    call cprds( r,s+2.*ds,x0sp2,y0sp2, ndi,iwx,iwy,iwr,iws,iwr1,
c4   &                ndr,rwx,rwy,rwr,rws,rwr1 )
c4    x0ss=(-x0sp2+8.*x0sp1-8.*x0sm1+x0sm2)/(12.*ds)
c4    y0ss=(-y0sp2+8.*y0sp1-8.*y0sm1+y0sm2)/(12.*ds)
      x0ss=(x0sp1-x0sm1)/(2.*ds)
      y0ss=(y0sp1-y0sm1)/(2.*ds)
      tmp=(x0s*y0ss-y0s*x0ss)/d**3

      if( ccor(18).gt. .5 )then
*         xs=x0s+rr0*x0s*tmp+rr0s*y0s/d  + ccor(2)+ccor(3)*r
*         ys=y0s+rr0*y0s*tmp-rr0s*x0s/d  + ccor(6)+ccor(7)*r
*         xr= r1r*r0*y0s/d               + ccor(1)+ccor(3)*s
*         yr=-r1r*r0*x0s/d               + ccor(5)+ccor(7)*s
        xs=x0s-rr0*x0s*tmp+rr0s*y0s/d  + ccor(2)+ccor(3)*r
        ys=y0s-rr0*y0s*tmp-rr0s*x0s/d  + ccor(6)+ccor(7)*r
        xr=-r1r*r0*y0s/d               + ccor(1)+ccor(3)*s
        yr=+r1r*r0*x0s/d               + ccor(5)+ccor(7)*s
      else
*         xs=x0s+rr0*x0s*tmp+rr0s*y0s/d
*         ys=y0s+rr0*y0s*tmp-rr0s*x0s/d
*         xr= r1r*r0*y0s/d
*         yr=-r1r*r0*x0s/d
        xs=x0s-rr0*x0s*tmp+rr0s*y0s/d
        ys=y0s-rr0*y0s*tmp-rr0s*x0s/d
        xr=-r1r*r0*y0s/d
        yr=+r1r*r0*x0s/d
      end if

      return
      end

      subroutine cprds( r,s,x0s,y0s, ndi,iwx,iwy,iwr,iws,iwr1,
     &                ndr,rwx,rwy,rwr,rws,rwr1 )
c==========================================================
c      Called by CPR
c      --------------
c Return x0s = dx/ds at r=0 and y0s = dy/ds at r=0
c==========================================================
      integer iwx(ndi),iwy(ndi),iwr(ndi),iws(ndi)
      real rwx(ndr),rwy(ndr),rwr(ndr),rws(ndr)
      call strt( s,s0,s0s, iws,rws,ierr )
      call sttr( s0,r0,r0s0, iwr,rwr,ierr )
      call strt( r,r1,r1r, iwr1,rwr1,ierr )
      call cp( s0,x0,y0,x0s0,y0s0, ndi,iwx,iwy,ndr,rwx,rwy )
      x0s=x0s0*s0s
      y0s=y0s0*s0s

      return
      end

      subroutine cp( r,x,y,xr,yr, ndi,iwx,iwy,ndr,rwx,rwy )
c==========================================================
c      Called by CPR
c==========================================================
      integer iwx(ndi),iwy(ndi)
      real rwx(ndr),rwy(ndr)
      call sttr( r,x,xr, iwx,rwx,ierr )
      call sttr( r,y,yr, iwy,rwy,ierr )
      return
      end

      subroutine cprsi( nr,ns,per,ndw,w,iw,
     &   nwdi,iwx,iwy,iwr,iws,iwr1,nwdr,rwx,rwy,rwr,rws,rwr1,ccor )
c================================================================
c  Curved Polygonal Region defined by Splines - Initialization
c
c INPUT-
c  nr,ns : number of points on the splines in the r and s directions
c  w,iw  : work arrays (same array) of dimension ndw
c  per   : 0= curve not periodic in s
c          1= derivative periodic in s
c          2= curve is periodic in s
c  ndw : size of real work space wk()
c          ndw > 9+ns*9
c  nwdi,iwx,iwy,... : arrays genereated by CPINIT
c
c OUTPUT -
c  ndw,w,iw,...  : arrays to be used in calls to CPRS
c
c Who to blame: Bill Henshaw
c================================================================
      integer iw(ndw),rptr,sptr,xptr,yptr,bxptr,byptr,per
      integer iwx(nwdi),iwy(nwdi),iwr(nwdi),iws(nwdi)
      real w(ndw)
      real rwx(nwdr),rwy(nwdr),rwr(nwdr),rws(nwdr),ccor(*)
     
      ierr=0
      iw(1)=nr
      iw(2)=ns
c assign pointers
      rptr=0
      sptr=9
      xptr =sptr+ns
      yptr =xptr+ns
      bxptr=yptr+ns
      byptr=bxptr+ns*3
      if( byptr+3*ns .gt.ndw )then
        ierr=1
        write(6,*) 'CPRSI: not enough work space, ndw <',byptr+3*ns
        return
      end if
      iw(3)=rptr
      iw(4)=sptr
      iw(5)=xptr
      iw(6)=yptr
      iw(7)=bxptr
      iw(8)=byptr
      r=0.
      do 100 j=1,ns
        w(sptr+j-1)=float(j-1)/float(ns-1)
        call cpr( r,w(sptr+j-1),w(xptr+j-1),w(yptr+j-1),xr,xs,yr,ys,
     &    nwdi,iwx,iwy,iwr,iws,iwr1,nwdr,rwx,rwy,rwr,rws,rwr1,ccor )
 100  continue

      if( per.gt.0 )then
c       ...periodic case:
        iopt=1
        if( per.eq.2 )then
c         ...make sure end points coincide
          w(xptr+ns-1)=w(xptr)
          w(yptr+ns-1)=w(yptr)
        end if
      else
c       ..non-periodic
        iopt=0
      end if

      call csgen (ns, w(sptr),w(xptr),w(bxptr), iopt  )
      call csgen (ns, w(sptr),w(yptr),w(byptr), iopt  )

c*wdh write(1,9000) (i,w(sptr+i-1),w(xptr+i-1),w(bxptr+3*(i-1)),
c*wdh&  w(bxptr+3*(i-1)+1),w(bxptr+3*(i-1)+2),i=1,ns-1)
 9000 format(1x,' i     s      x     b(1,.)    b(2,.)    b(3,.)',/,
     &        (1x,i3,5f11.4))
c*wdh write(1,9100) (i,w(sptr+i-1),w(yptr+i-1),w(byptr+3*(i-1)),
c*wdh&  w(byptr+3*(i-1)+1),w(byptr+3*(i-1)+2),i=1,ns-1)
 9100 format(1x,' i     s      y     b(1,.)    b(2,.)    b(3,.)',/,
     &        (1x,i3,5f11.4))
      return
      end

      subroutine cprs( r,s,x,y,xr,xs,yr,ys, ndw,w,iw,
     &                 ndi,iwr,iws,iwr1,ndr,rwr,rws,rwr1,ccor )
c================================================================
c      Curved Polygonal Region defined by Splines
c      ------------------------------------------
c Evaluate the grid defined with CPRSI
c
c INPUT
c  r,s  : evalute the grid at r,s
c  ndw,w,ndi,iwr,... : parameters and arrays generated by CPRSI
c OUTPUT
c  x,y, xr,xs,yr,ys : position and derivatives at (r,s)
c
c Who to blame: Bill Henshaw
c================================================================
      real w(ndw),ccor(20)
      integer iw(ndw)
c local
      integer bxptr,byptr
c.......start statement functions
      bx(k)=w(bxptr+k)
      by(k)=w(byptr+k)
c.......end statement functions
      nsm1=iw(2)-1
      j=s*nsm1+1
      j=max(min(j,nsm1),1)
      ds=s-(j-1)/float(nsm1)
      bxptr=iw(7)+3*(j-1)
      x0  =w(iw(5)+j-1)+ds*(bx(0)+ds*(bx(1)+ds*bx(2)))
      x0s =bx(0)+ds*(2.*bx(1)+ds*3.*bx(2))
      x0ss=2.*(bx(1)+ds*3.*bx(2))
      byptr=iw(8)+3*(j-1)
      y0  =w(iw(6)+j-1)+ds*(by(0)+ds*(by(1)+ds*by(2)))
      y0s =by(0)+ds*(2.*by(1)+ds*3.*by(2))
      y0ss=2.*(by(1)+ds*3.*by(2))

c.......stretching in s direction (near corners etc.)
      call strt( s,s0,s0s,   iws,rws,ierr )
c.......radial width varies in s direction
      call sttr( s0,r0,r0s0, iwr,rwr,ierr )
c........lines stretched in r direction
      call strt( r,r1,r1r, iwr1,rwr1,ierr )
      rr0=r1*r0
      rr0s=r1*r0s0*s0s
      d=1./sqrt(x0s**2+y0s**2)

      x=x0-rr0*y0s*d
      y=y0+rr0*x0s*d
      tmp=-(x0s*y0ss-y0s*x0ss)*d**3
      xs=x0s+rr0*x0s*tmp-rr0s*y0s*d
      ys=y0s+rr0*y0s*tmp+rr0s*x0s*d
      xr=-r1r*r0*y0s*d
      yr=+r1r*r0*x0s*d

c.....Now project onto boundary curves s=0 and s=1

      if( ccor(18).gt.0.5 )then
        epss=ccor(9)
        if( abs(s).lt.epss )then
          x=ccor(10)+r1*(ccor(12)-ccor(10))
          y=ccor(14)+r1*(ccor(16)-ccor(14))
          xr=(ccor(12)-ccor(10))*r1r
          yr=(ccor(16)-ccor(14))*r1r
        elseif( abs(s-1.).lt.epss )then
          x=ccor(11)+r1*(ccor(13)-ccor(11))
          y=ccor(15)+r1*(ccor(17)-ccor(15))
          xr=(ccor(13)-ccor(11))*r1r
          yr=(ccor(17)-ccor(15))*r1r
        end if
      end if

      return
      end
