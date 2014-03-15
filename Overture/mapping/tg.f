*--------------------------------------------------------------
      integer function findnose(n,z)
	  
	  implicit none
	  real*8	PI
	  parameter (PI = 3.14159265358979)
	  integer	n
	  complex*16	z(n)
	  
	  integer	i, imin
	  real*8	xmin
	  
	  imin = 0
	  xmin = 1e8
	  do i = 1,n
	    if (dble(z(i)) .lt. xmin) then
		  xmin = dble(z(i))
		  imin = i
		end if
	  end do
	  findnose = imin

	  return
	  end
*--------------------------------------------------------------

      real*8 function arg(z)
	  
	  implicit none
	  real*8	PI
	  parameter (PI = 3.14159265358979)
	  complex*16 z
	  
	  arg = datan2(dimag(z),dble(z))
          if (arg.lt.0.0) arg=2*PI+arg
	  
	  return
	  end
*--------------------------------------------------------------

      real*8 function argdir(z,direction)

* arg(z) is kept continous by switching Riemann-sheet
* direction=0 -> first point -pi < arg(z) <= pi
* direction=1 -> argdir(z) increases monotonously
* direction=-1 -> argdir(z) decreases monotonously

      implicit none
      real*8	PI
      parameter (PI = 3.14159265358979)
 
	  complex*16 z
      integer direction

      real*8 oldarg,arg
      integer sheet,dsheet
      save oldarg,sheet
	  
      arg = atan2(dimag(z),dble(z))
      dsheet=0
	  if (nint(sign(1.0d0,arg-oldarg)).ne.direction) then
        dsheet=1
      end if
      oldarg=arg
      sheet =direction*(sheet+dsheet)
      argdir=arg+2*PI*sheet
	  
      return
      end
*--------------------------------------------------------------

      complex*16 function curvature(z1,z2,z3)
	  
	  implicit none
	  real*8	PI
	  parameter (PI = 3.14159265358979)
	  complex*16 z1,z2,z3
	  
	  curvature = ((z3-z2)/cdabs(z3-z2)-(z2-z1)/cdabs(z2-z1))/
     -				cdabs(z3-z1)*2
	  return
	  end
*--------------------------------------------------------------
	  
c This version does something very specialized for 
c Karmann-Trefftz transformations.
c
	  integer function branch(z0,z1,zi,old_zi)
	  
	  implicit none
	  real*8	PI,zero,one
	  parameter (PI = 3.14159265358979,zero=0.0d0,one=1.0d0)
	  complex*16	z0,z1,zi,old_zi
	  
	  integer	isign
      real*8	sign
      real*8	a1xb1
      real*8	a0r,a1r,b0r,b1r
      real*8	a0i,a1i,b0i,b1i
      real*8	s,t
	  integer	br
	  save br
	  data br /0/
	  
	  a0r = dble(z0)
	  a0i = dimag(z0)
	  a1r = dble(z1-z0)
	  a1i = dimag(z1-z0)
	  b0r = dble(old_zi)
	  b0i = dimag(old_zi)
	  b1r = dble(zi-old_zi)
	  b1i = dimag(zi-old_zi)
	  
	  a1xb1 = a1r*b1i-a1i*b1r
	  if (a1xb1.eq.0.0d0) goto 1000

	  isign = nint(sign(1.0d0,a1xb1))
	  
	  t=(b0i-a0i-(b0r-a0r)*a1i/a1r)/(-b1i+b1r*a1i/a1r)
	  s=(b0r-a0r+b1r*t)/a1r
	  
	  br = br+isign*(t.gt.zero.and.t.lt.one.
     1           and.s.gt.zero.and.s.lt.one)
	 
1000  branch = br
	  
c	  if (dimag(dz).eq.0.0) goto 1000
c	  xint=dble(old_zi)+dble(dz)*(dimag(z0)-dimag(old_zi))/dimag(dz)
c	  y = dimag(zi) - dimag(z0)
c	  yo = dimag(old_zi) - dimag(z0)
c	  crosscut = (sign(1,yo).ne.sign(1,y))
c	  if (xint.lt.dble(z0).and.crosscut) then
c	    old_branch = old_branch + int(sign(1,yo-y))
c	  end if
c	  xint=dble(old_zi)+dble(dz)*(dimag(z1)-dimag(old_zi))/dimag(dz)
c	  y = dimag(zi) - dimag(z1)
c	  yo = dimag(old_zi) - dimag(z1)
c	  ytemp = sign(1,yo)
c	  ytemp = sign(1,y)
c	  crosscut = (sign(1,yo).ne.sign(1,y))
c	  if (xint.lt.dble(z1).and.crosscut) then
c	    old_branch = old_branch - int(sign(1,yo-y))
c	  end if
c1000  old_zi = zi
c	  branch = old_branch
	  
	  return
	  end
*--------------------------------------------------------------

      function dumexp( a, b )
      complex*16 dumexp, a
      real*8 b
      if (dble(a) .eq. 0.0d0 .and. dimag(a) .eq. 0.0d0) then
        dumexp = 0.0
      else
        dumexp = a**b
      endif
      return
      end

      subroutine invkarmanntrefftz(z,n,beta,z0,z1,rte,dtau)
	  
	  implicit none
	  real*8	PI
	  parameter (PI = 3.14159265358979)
	  
	  integer	n
	  complex*16	z(n)
	  real*8	beta
	  complex*16	z0, z1
	  real*8	rte,dtau
	  
	  real*8	arg
	  complex*16	angle
	  integer	branch
	  complex*16	curv, curvature
	  integer	findnose
	  integer	i
	  real*8	tau
	  complex*16	old_zi, den, dumexp
          external dumexp
	  
	  tau = arg(z(n-1)-z(n)) - arg(z(2)-z(1))+dtau
	  beta = 1/(2-tau/PI)
	  z0 = z(1)-dcmplx(rte,0)
	  i = findnose(n,z)
	  curv = curvature(z(i-1),z(i),z(i+1))
	  z1 = curv/cdabs(curv)**2 / 2
	  old_zi = z(1)
	  do i = 1,n
	    angle = branch(z0,z1,z(i),old_zi)*dcmplx(0,2)*beta*PI
            den = -1 + dumexp( (z(i)-z0)/(z(i) - z1), beta) * 
     +           cdexp(angle)
	    z(i) = beta*z1+(-(beta*z0)+beta*z1)/ den
	  end do
	  
	  return
	  end
	  
*--------------------------------------------------------------

      subroutine karmanntrefftz(z,n,beta,z0,z1)
	  
	  implicit none
	  real*8	PI
	  parameter (PI = 3.14159265358979)
	  
	  integer	n
	  complex*16	z(n)
	  real*8	beta
	  complex*16	z0, z1
	  
	  real*8	betai
	  integer	i
	  
	  betai = 1/beta
	  do i = 1,n
	    z(i) = (-((z(i) - beta*z0)**betai*z1) + 
     -			z0*(z(i) - beta*z1)**betai)/
     -			(-((z(i) - beta*z0)**betai) + 
     -			(z(i) - beta*z1)**betai)
	  end do
	  return
	  end
	  
*--------------------------------------------------------------

      subroutine theodorsengarrick(z, nz, a, b, nc)
	  
	  implicit none
	  real*8	PI
	  parameter (PI = 3.14159265358979)
	  
	  integer	nz, nc
	  complex*16	z(nz)
	  real*8	a(nc),b(nc)
	  
	  integer	i,j
	  complex*16	sum
	  	  
	  do i = 1,nz
	    sum = dcmplx(0,0)
		do j = 1,nc
		  sum = sum+dcmplx(a(j),b(j))*z(i)**(1-j)
		end do
		z(i) = z(i)*cdexp(sum)
	  end do
	  
	  return
	  end
	  
*--------------------------------------------------------------
* For closed curves only...

      subroutine cg(c, n, z)
	  
	  implicit none
	  real*8	PI
	  parameter (PI = 3.14159265358979)
	  
	  complex*16	c
	  integer	n
	  complex*16	z(n)
	  
	  
	  real*8	ds
	  integer	i
	  complex*16	m
	  integer	next
	  integer	prev
	  real*8	s
	  
	  m = dcmplx(0.0d0,0.0d0)
	  s = 0.0d0
	  do i = 1,n-1
	    next = i+1 		! because z(n) = z(1)
	    prev = n-1-mod(n-i,n-1)
	    ds = (cdabs(z(i)-z(next))+cdabs(z(i)-z(prev)))/2
	    s = s + ds
	    m = m + ds*z(i)
	  end do
	  c = m/s
	  
	  return
	  end
*--------------------------------------------------------------

      subroutine translate(n, z, t)
	  
	  implicit none
	  real*8	PI
	  parameter (PI = 3.14159265358979)
	  
	  integer	n
	  complex*16	z(n)
	  complex*16	t
	  
	  
	  integer	i
	  
	  do i = 1,n
	    z(i) = z(i) + t
	  end do
	  
	  return
	  end

*--------------------------------------------------------------

c      subroutine kttgjacobian(z,n,beta,z0,z1)
	  
c	  implicit none
c	  real*8*8	PI
c	  parameter (PI = 3.14159265358979)
c	  
c	  integer	nz, nc
c	  complex*16	z(nz)
c	  real*8	a(nc),b(nc)
c	  
c	  integer	i,j
c	  complex*16*16	sum
c	  	  
c	  do i = 1,nz
c	    sum = dcmplx(0,0)
c		do j = 1,nc
c		  sum = sum+dcmplx(a(j),b(j))*z(i)**(1-j)
c		end do
c		z(i) = z(i)*cdexp(sum)
c	  end do
c	  
c	  return
c	  end
	  
*--------------------------------------------------------------
c(1 + dsum)*esum*zq**(-1 + 1/beta)*(-z0 + z1)**2/
c     -  ((-1 + zq**(1/beta))**2*(-c - esum*z + beta*z0)**2)
