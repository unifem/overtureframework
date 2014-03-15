c
c $Header: /usr/gapps/overture/OvertureRepoCVS/overture/Overture/mapping/cs.f,v 1.4 2003/11/01 21:27:15 henshaw Exp $
c
      subroutine cseval(n, x, y, bcd, u, s, sp )
c================================================================
c              Cubic Spline Evaluator
c              ----------------------
c Adapted from the program SEVAL of "Computer Methods for Mathematical
c Computations" by Forsythe, Malcolm and Moler. Bill Henshaw 12/12/86
c Modified by Geoff Chesshire 1/6/87 to allow spline evaluation outside
c the interval of definition.
c
c  this subroutine evaluates the cubic spline function
c
c  s=y(i)+bcd(1,i)*(u-x(i))+bcd(2,i)*(u-x(i))**2+bcd(3,i)*(u-x(i))**3
c
c  and its derivative
c
c  sp=bcd(1,i)+2*bcd(2,i)*(u-x(i))+3*bcd(3,i)*(u-x(i))**2
c
c  where  x(i) .lt. u .lt. x(i+1)
c
c  input..
c
c    n = the number of data points
c    x,y = the arrays of data abscissas and ordinates
c    bcd = array of spline coefficients (such as computed by csgen)
c    u = the abscissa at which to evaluate the spline
c
c  output..
c   s = spline evaluated at u
c   sp = derivative of spline evaluated at u
c
c  if  u  is not in the same interval as the previous call, then a
c  binary search is performed to determine the proper interval.
c
      integer n

      real x(n), y(n), bcd(3,n)
      integer i, j, k
      real dx
      save i
      data i/1/
      if( u.lt.x(1) )then
        i=1
        goto30
      else if( u.gt.x(n) )then
        i=n
        goto30
      end if
      if ( i .ge. n ) i = 1
      if ( u .lt. x(i) ) go to 10
      if ( u .le. x(i+1) ) go to 30
c
c  binary search
c
   10 i = 1
      j = n+1
   20 k = (i+j)/2
      if ( u .lt. x(k) ) j = k
      if ( u .ge. x(k) ) i = k
      if ( j .gt. i+1 ) go to 20
c
c  evaluate spline
c
   30 dx = u - x(i)
      s  = y(i) + dx*(bcd(1,i) + dx*(bcd(2,i) + dx*bcd(3,i)))
      sp = bcd(1,i) + dx*(2.*bcd(2,i) + 3.*dx*bcd(3,i))
      return
      end

      subroutine csevl2(n, x, y, bcd, u, s, sp, spp )
c================================================================
c              Cubic Spline Evaluator
c              ----------------------
c Adapted from the program SEVAL of "Computer Methods for Mathematical
c Computations" by Forsythe, Malcolm and Moler. Bill Henshaw 12/12/86
c Modified by Geoff Chesshire 1/6/87 to allow spline evaluation outside
c the interval of definition.
c
c  this subroutine evaluates the cubic spline function
c
c  s=y(i)+bcd(1,i)*(u-x(i))+bcd(2,i)*(u-x(i))**2+bcd(3,i)*(u-x(i))**3
c
c  and its derivative
c
c  sp=bcd(1,i)+2*bcd(2,i)*(u-x(i))+3*bcd(3,i)*(u-x(i))**2
c
c  where  x(i) .lt. u .lt. x(i+1)
c
c  input..
c
c    n = the number of data points
c    x,y = the arrays of data abscissas and ordinates
c    bcd = array of spline coefficients (such as computed by csgen)
c    u = the abscissa at which to evaluate the spline
c
c  output..
c   s = spline evaluated at u
c   sp = derivative of spline evaluated at u
c
c  if  u  is not in the same interval as the previous call, then a
c  binary search is performed to determine the proper interval.
c
      integer n

      real x(n), y(n), bcd(3,n)
      integer i, j, k
      real dx
      save i
      data i/1/
      if( u.lt.x(1) )then
        i=1
        goto30
      else if( u.gt.x(n) )then
        i=n
        goto30
      end if
      if ( i .ge. n ) i = 1
      if ( u .lt. x(i) ) go to 10
      if ( u .le. x(i+1) ) go to 30
c
c  binary search
c
   10 i = 1
      j = n+1
   20 k = (i+j)/2
      if ( u .lt. x(k) ) j = k
      if ( u .ge. x(k) ) i = k
      if ( j .gt. i+1 ) go to 20
c
c  evaluate spline
c
   30 dx = u - x(i)
      s   = y(i) + dx*(bcd(1,i) + dx*(bcd(2,i) + dx*bcd(3,i)))
      sp  = bcd(1,i) + dx*(2.*bcd(2,i) + 3.*dx*bcd(3,i))
      spp = 2.*bcd(2,i) + 6.*dx*bcd(3,i)
      return
      end

      subroutine csgen (n, x, y, bcd, iopt  )
c===============================================================
c               Cubic Spline Generator
c               ----------------------
c Adapted from the program SPLINE of "Computer Methods for Mathematical
c Computations" by Forsythe, Malcolm and Moler, with changes to allow
c for periodic splines..........................Bill Henshaw 12/12/86
c
c  the coefficients b(i)=bcd(1,i), c(i)=bcd(2,i), and d(i)=bcd(3,i)
c  i=1,2,...,n are computed for a cubic interpolating spline
c
c    s(x) = y(i) + b(i)*(x-x(i)) + c(i)*(x-x(i))**2 + d(i)*(x-x(i))**3
c
c    for  x(i) .le. x .le. x(i+1)
c
c  input..
c
c    n = the number of data points or knots (n.ge.2)
c        also equals the number of points in the arrays x,y,b,c,d
c    x = the abscissas of the knots in strictly increasing order
c    y = the ordinates of the knots
c    iopt =  0 for nonperiodic spline
c         <> 0 for periodic spline : If y(1).ne.y(n) then the periodic
c              spline is fit in the following manner:
c         (1) subtract out the linear function
c                yl(x)=y(1)+(x-x(1))*(y(n)-y(1))/(x(n)-x(1))
c                i.e. y(i) <- y(i) - yl(x(i))
c         (2) Fit the periodic spline to the resulting points (x(i),y(i))
c         (3) Add back the linear function yl(x)
c
c  output..
c
c    bcd  = arrays of spline coefficients as defined above.
c           bcd of dimension bcd(3,n)
c
c  using  p  to denote differentiation,
c
c    y(i) = s(x(i))
c    b(i) = sp(x(i))
c    c(i) = spp(x(i))/2
c    d(i) = sppp(x(i))/6  (derivative from the right)
c
c  the accompanying subroutine cseval can be used
c  to evaluate the spline.
c
c===============================================================
      integer n
      real x(n), y(n), bcd(3,n)
      integer nm1, ib, i
      real t
      h(i)=x(i+1)-x(i)
      yl(i)=y1+slope*(x(i)-x(1))
c
      nm1 = n-1
      if ( n .lt. 2 ) return
      if ( n .lt. 3 ) go to 50

      if( iopt.ne.0 ) goto 100
c...................Nonperiodic Case .........................

c
c  set up tridiagonal system
c
c  b = diagonal, d = offdiagonal, c = right hand side.
c
      bcd(3,1) = x(2) - x(1)
      bcd(2,2) = (y(2) - y(1))/bcd(3,1)
      do 10 i = 2, nm1
         bcd(3,i) = x(i+1) - x(i)
         bcd(1,i) = 2.*(bcd(3,i-1) + bcd(3,i))
         bcd(2,i+1) = (y(i+1) - y(i))/bcd(3,i)
         bcd(2,i) = bcd(2,i+1) - bcd(2,i)
   10 continue
c
c  end conditions.  third derivatives at  x(1)  and  x(n)
c  obtained from divided differences
c
      bcd(1,1) = -bcd(3,1)
      bcd(1,n) = -bcd(3,n-1)
      bcd(2,1) = 0.
      bcd(2,n) = 0.
      if ( n .eq. 3 ) go to 15
      bcd(2,1) = bcd(2,3)/(x(4)-x(2)) - bcd(2,2)/(x(3)-x(1))
      bcd(2,n) = bcd(2,n-1)/(x(n)-x(n-2)) - bcd(2,n-2)/(x(n-1)-x(n-3))
      bcd(2,1) = bcd(2,1)*bcd(3,1)**2/(x(4)-x(1))
      bcd(2,n) = -bcd(2,n)*bcd(3,n-1)**2/(x(n)-x(n-3))
c
c  forward elimination
c
   15 do 20 i = 2, n
         t = bcd(3,i-1)/bcd(1,i-1)
         bcd(1,i) = bcd(1,i) - t*bcd(3,i-1)
         bcd(2,i) = bcd(2,i) - t*bcd(2,i-1)
   20 continue
c
c  back substitution
c
      bcd(2,n) = bcd(2,n)/bcd(1,n)
      do 30 ib = 1, nm1
         i = n-ib
         bcd(2,i) = (bcd(2,i) - bcd(3,i)*bcd(2,i+1))/bcd(1,i)
   30 continue
c
c  bcd(2,i) is now the sigma(i) of the text
c
c  compute polynomial coefficients
c
      bcd(1,n)=(y(n)-y(nm1))/bcd(3,nm1)+bcd(3,nm1)*
     &         (bcd(2,nm1)+2.*bcd(2,n))
      do 40 i = 1, nm1
         bcd(1,i)=(y(i+1)-y(i))/bcd(3,i)-bcd(3,i)*
     &            (bcd(2,i+1)+2.*bcd(2,i))
         bcd(3,i) = (bcd(2,i+1) - bcd(2,i))/bcd(3,i)
         bcd(2,i) = 3.*bcd(2,i)
   40 continue
      bcd(2,n) = 3.*bcd(2,n)
      bcd(3,n) = bcd(3,n-1)
      return
c  Spline with 2 points:
   50 bcd(1,1) = (y(2)-y(1))/(x(2)-x(1))
      bcd(2,1) = 0.
      bcd(3,1) = 0.
      bcd(1,2) = bcd(1,1)
      bcd(2,2) = 0.
      bcd(3,2) = 0.
      return
 100  continue
c............Periodic Case with n>=3...........................
c     Subtract off the "linear" part of the function
      y1=y(1)
      slope=(y(n)-y(1))/(x(n)-x(1))
      do 110 i=1,n
        y(i)=y(i)-yl(i)
 110  continue

      nm1=n-1
      nm2=n-2
      im1=nm1
c  Compute diagonal and right hand side of matrix
      do 200 i=1,nm1
        bcd(1,i)=2.*(h(im1)+h(i))
        bcd(2,i)=(y(i+1)-y(i))/h(i)-(y(i)-y(im1))/h(im1)
        im1=i
 200  continue
c  Solve the periodic "tridiagonal" system
c
c        | b1  h1                   hnm1 |
c        | h1  b2 h2                     |
c        | 0   h2 b3 h3                  |
c        | 0   0  h3 b4 h4               |
c        | 0                             |
c        | 0                             |
c        | 0                             |
c        |                               |
c        | 0               hnm3 bnm2 hnm2|
c        | hnm1             0   hnm2 bnm1|
c
c
c

      bn=bcd(1,nm1)         ! bottom-right element of matrix
c*wdh 031028   an=h(1)
      an=h(nm1)             ! bottom lower corner
      rn=bcd(2,nm1)         ! last element of rhs
      bcd(3,1)=h(nm1)       ! upper right element
      do 300 i=2,n-2
        tmp=h(i-1)/bcd(1,i-1)
        bcd(1,i)=bcd(1,i)-tmp*h(i-1)        ! diagonal
        bcd(2,i)=bcd(2,i)-tmp*bcd(2,i-1)    ! RHS
        bcd(3,i)=    -tmp*bcd(3,i-1)        ! right-most column
        tmp=an/bcd(1,i-1)
        rn=rn-tmp*bcd(2,i-1)
        an=  -tmp*h(i-1)                    ! bottom column
        bn=bn-tmp*bcd(3,i-1)
 300  continue
      bcd(3,nm2)=h(nm2)+bcd(3,nm2)        ! correct last step
c*wdh 031028  an=(an+h(nm1))/bcd(1,nm2)
      an=(an+h(nm2))/bcd(1,nm2)           ! correct last step and start next step
      bcd(1,nm1)=bn-an*bcd(3,nm2)
      rn=(rn-an*bcd(2,nm2))/bcd(1,nm1)
      bcd(2,nm1)=rn
      bcd(2,nm2)=(bcd(2,nm2)-bcd(3,nm2)*rn)/bcd(1,nm2)
      do 400 i=n-3,1,-1
        bcd(2,i)=(bcd(2,i)-h(i)*bcd(2,i+1)-bcd(3,i)*rn)/bcd(1,i)
 400  continue
      bcd(2,n)=bcd(2,1)
c
c  bcd(2,i) is now the sigma(i) of the text
c
c  compute polynomial coefficients
c
      do 500 i = 1, nm1
         bcd(1,i)=(y(i+1)-y(i))/h(i)-h(i)*(bcd(2,i+1)+2.*bcd(2,i))
         bcd(3,i) = (bcd(2,i+1) - bcd(2,i))/h(i)
         bcd(2,i) = 3.*bcd(2,i)
 500  continue
      bcd(1,n) = bcd(1,1)
      bcd(2,n) = bcd(2,1)
      bcd(3,n) = bcd(3,1)
c     Now add back the linear part and change the spline coefficients
c     corresponding to the first derivative.
      do 510 i=1,n
        y(i)=y(i)+yl(i)
        bcd(1,i)=bcd(1,i)+slope
 510  continue
      return
      end
