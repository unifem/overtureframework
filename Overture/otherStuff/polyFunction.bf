#Include "polyFunction.h"

c ************************************************************************************
c  Here are the versions for evaluating many points at a time
c ************************************************************************************

#beginMacro defineTime()
if( dt.eq.0 )then
 if( degreeTime.eq.0 )then
  time=a(0,n)
 else if( degreeTime.eq.1 )then
  time=a(0,n)+t*(a(1,n))
 else if( degreeTime.eq.2 )then
  time=a(0,n)+t*(a(1,n)+t*(a(2,n)))
 else if( degreeTime.eq.3 )then
  time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))))
 else if( degreeTime.eq.4 )then
  time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))))
 else if( degreeTime.eq.5 )then
  time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n))))))
 else if( degreeTime.eq.6 )then
  time=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+t*(a(6,n)))))))
 else
  write(*,*) 'ERROR invalid degreeTime'
  stop
 end if
else if( dt.eq.1 )then
 ! --- first time derivative ---
 if( degreeTime.eq.0 )then
  time=0.
 else if( degreeTime.eq.1 )then
  time=a(1,n)
 else if( degreeTime.eq.2 )then
  time=a(1,n)+t*(2.*a(2,n))
 else if( degreeTime.eq.3 )then
  time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
 else if( degreeTime.eq.4 )then
  time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
 else if( degreeTime.eq.5 )then
  time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)))))
 else if( degreeTime.eq.6 )then
  time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)+t*(6.*a(6,n))))))
 else
  write(*,*) 'PolyFunction:ERROR invalid degreeTime'
  stop 1834
 end if
else if( dt.eq.2 )then
 ! --- 2nd time derivative ---
 if( degreeTime.eq.0 )then
  time=0.
 else if( degreeTime.eq.1 )then
  time=0.
 else if( degreeTime.eq.2 )then
  time=2.*a(2,n)
 else if( degreeTime.eq.3 )then
  time=2.*a(2,n)+t*(6.*a(3,n))
 else if( degreeTime.eq.4 )then
  time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
 else if( degreeTime.eq.5 )then
  time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
 else if( degreeTime.eq.6 )then
  time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*a(6,n)))))
 else
  write(*,*) 'PolyFunction:ERROR invalid degreeTime'
  stop 1835
 end if
else if( dt.eq.3 )then
 ! --- 3rd time derivative ---
 if( degreeTime.eq.0 )then
  time=0.
 else if( degreeTime.eq.1 )then
  time=0.
 else if( degreeTime.eq.2 )then
  time=0.
 else if( degreeTime.eq.3 )then
  time=6.*a(3,n)
 else if( degreeTime.eq.4 )then
  time=6.*a(3,n)+t*(24.*a(4,n))
 else if( degreeTime.eq.5 )then
  time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
 else if( degreeTime.eq.6 )then
  time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
 else
  write(*,*) 'PolyFunction:ERROR invalid degreeTime'
  stop 1836
 end if
else if( dt.eq.4 )then
 ! --- 4th time derivative ---
 if( degreeTime.eq.0 )then
  time=0.
 else if( degreeTime.eq.1 )then
  time=0.
 else if( degreeTime.eq.2 )then
  time=0.
 else if( degreeTime.eq.3 )then
  time=0.
 else if( degreeTime.eq.4 )then
  time=24.*a(4,n)
 else if( degreeTime.eq.5 )then
  time=24.*a(4,n)+t*(120.*a(5,n))
 else if( degreeTime.eq.6 )then
  time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
 else
  write(*,*) 'PolyFunction:ERROR invalid degreeTime'
  stop 1837
 end if
else
  write(*,*) 'PolyFunction:ERROR: too many time derivatives'
  stop 1838
end if
#endMacro

! --- evaluate time derivatives of the polynomial ---
#beginMacro defineTimeDerivative()
if( dt.eq.1 )then
 ! --- first time derivative ---
 if( degreeTime.eq.0 )then
  time=0.
 else if( degreeTime.eq.1 )then
  time=a(1,n)
 else if( degreeTime.eq.2 )then
  time=a(1,n)+t*(2.*a(2,n))
 else if( degreeTime.eq.3 )then
  time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)))
 else if( degreeTime.eq.4 )then
  time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))))
 else if( degreeTime.eq.5 )then
  time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)))))
 else if( degreeTime.eq.6 )then
  time=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)+t*(6.*a(6,n))))))
 else
  write(*,*) 'PolyFunction:ERROR invalid degreeTime'
  stop 1834
 end if
else if( dt.eq.2 )then
 ! --- 2nd time derivative ---
 if( degreeTime.eq.0 )then
  time=0.
 else if( degreeTime.eq.1 )then
  time=0.
 else if( degreeTime.eq.2 )then
  time=2.*a(2,n)
 else if( degreeTime.eq.3 )then
  time=2.*a(2,n)+t*(6.*a(3,n))
 else if( degreeTime.eq.4 )then
  time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)))
 else if( degreeTime.eq.5 )then
  time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n))))
 else if( degreeTime.eq.6 )then
  time=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*a(6,n)))))
 else
  write(*,*) 'PolyFunction:ERROR invalid degreeTime'
  stop 1835
 end if
else if( dt.eq.3 )then
 ! --- 3rd time derivative ---
 if( degreeTime.eq.0 )then
  time=0.
 else if( degreeTime.eq.1 )then
  time=0.
 else if( degreeTime.eq.2 )then
  time=0.
 else if( degreeTime.eq.3 )then
  time=6.*a(3,n)
 else if( degreeTime.eq.4 )then
  time=6.*a(3,n)+t*(24.*a(4,n))
 else if( degreeTime.eq.5 )then
  time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)))
 else if( degreeTime.eq.6 )then
  time=6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n))))
 else
  write(*,*) 'PolyFunction:ERROR invalid degreeTime'
  stop 1836
 end if
else if( dt.eq.4 )then
 ! --- 4th time derivative ---
 if( degreeTime.eq.0 )then
  time=0.
 else if( degreeTime.eq.1 )then
  time=0.
 else if( degreeTime.eq.2 )then
  time=0.
 else if( degreeTime.eq.3 )then
  time=0.
 else if( degreeTime.eq.4 )then
  time=24.*a(4,n)
 else if( degreeTime.eq.5 )then
  time=24.*a(4,n)+t*(120.*a(5,n))
 else if( degreeTime.eq.6 )then
  time=24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n)))
 else
  write(*,*) 'PolyFunction:ERROR invalid degreeTime'
  stop 1837
 end if
else
  write(*,*) 'PolyFunction:ERROR: too many time derivatives'
  stop 1838
end if
#endMacro


#beginMacro beginLoops(arg)
do n=nca,ncb
arg
do i3=nta,ntb
do i2=nsa,nsb
do i1=nra,nrb
#endMacro
#beginMacro endLoops()
end do
end do
end do
end do
#endMacro


#beginMacro polyFun(type)
subroutine polyFunction ## type (nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,\
 ndrra,ndrrb,ndrsa,ndrsb,ndrta,ndrtb,ndrca,ndrcb,ndc1,ndc2,ndc3,\
 nra,nrb,nsa,nsb,nta,ntb, nca,ncb, nda, degree, degreeTime, t, \
 a,c, r,xa,ya,za, dx,dy,dz,dt)
!==========================================================================
!   *** Define a polynomial function and it's derivatives ***
! nd : number of space dimensions
! nra,nrb,nsa,nsb,nta,ntb : return result in this array
! c : array of polynomial coefficients
! r  : return result in this array
! degree: degree of the polynomial
! dx,dy,dz: compute this derivative
!==========================================================================
integer nca,ncb,dx,dy,dz,dt,degree,degreeTime
real xa(ndra:ndrb,ndsa:ndsb,ndta:ndtb)
real ya(ndra:ndrb,ndsa:ndsb,ndta:ndtb)
real za(ndra:ndrb,ndsa:ndsb,ndta:ndtb)
! real r(ndrra:ndrrb,ndrsa:ndrsb,ndrta:ndrtb,nca:ncb)
real r(ndrra:ndrrb,ndrsa:ndrsb,ndrta:ndrtb,ndrca:ndrcb)
real c(0:ndc1-1,0:ndc2-1,0:ndc3-1,0:*)
real a(0:nda-1,0:*)
real t

integer n,i1,i2,i3,laplace
real time

laplace=0
if( dx.eq.-2 )then
laplace=1
end if

poly ## type(x1=xa(i1,i2,i3),y1=ya(i1,i2,i3),z1=za(i1,i2,i3),r(i1,i2,i3,n))

return
end
#endMacro


#beginMacro buildFile(file,type)
#beginFile file
polyFun(type)
#endFile
#endMacro


#beginMacro appendFile(file,type)
#appendFile file
polyFun(type)
#endFile
#endMacro

      buildFile(polyFunction1D.f,1D0)
      appendFile(polyFunction1D.f,1D1)
      appendFile(polyFunction1D.f,1D2)
      appendFile(polyFunction1D.f,1D3)
      appendFile(polyFunction1D.f,1D4)
      appendFile(polyFunction1D.f,1D5)
      appendFile(polyFunction1D.f,1D6)

      buildFile(polyFunction2D.f,2D0)
      appendFile(polyFunction2D.f,2D1)
      appendFile(polyFunction2D.f,2D2)
      appendFile(polyFunction2D.f,2D3)
      appendFile(polyFunction2D.f,2D4)
      buildFile(polyFunction2D5.f,2D5)
      buildFile(polyFunction2D6.f,2D6)

      buildFile(polyFunction3D.f,3D0)
      appendFile(polyFunction3D.f,3D1)
      appendFile(polyFunction3D.f,3D2)
      appendFile(polyFunction3D.f,3D3)
      buildFile(polyFunction3D4.f,3D4)
      buildFile(polyFunction3D5.f,3D5)
      buildFile(polyFunction3D6.f,3D6)

!     buildFile(polyFunction3D.f,poly3D0)

#beginMacro polyDegree(ND,DEGREE)
 call polyFunction ## ND D ## DEGREE(nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,\
 ndrra,ndrrb,ndrsa,ndrsb,ndrta,ndrtb,ndrca,ndrcb,ndc1,ndc2,ndc3,\
 nra,nrb,nsa,nsb,nta,ntb, nca,ncb, nda, degree, degreeTime, t, \
 a,c, r,xa,ya,za, dx,dy,dz,dt)
#endMacro

#beginMacro polyDimension(ND)
if( degree.eq.0 )then
 polyDegree(ND,0)
else if( degree.eq.1 )then
 polyDegree(ND,1)
else if( degree.eq.2 )then
 polyDegree(ND,2)
else if( degree.eq.3 )then
 polyDegree(ND,3)
else if( degree.eq.4 )then
 polyDegree(ND,4)
else if( degree.eq.5 )then
 polyDegree(ND,5)
else if( degree.eq.6 )then
 polyDegree(ND,6)
end if
#endMacro

      subroutine polyFunction(nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,
     &  ndrra,ndrrb,ndrsa,ndrsb,ndrta,ndrtb,ndrca,ndrcb,ndc1,ndc2,ndc3,
     &  nra,nrb,nsa,nsb,nta,ntb, nca,ncb, nda, degree, degreeTime, t, 
     &  a, c, r,xa,ya,za, dx,dy,dz,dt)
!==========================================================================
!   *** Define a polynomial function and it's derivatives ***
! nd : number of space dimensions
! nra,nrb,nsa,nsb,nta,ntb : return result in this array
! c : array of polynomial coefficients
! r  : return result in this array
! degree: degree of the polynomial
! dx,dy,dz,dt: compute this derivative (set dx==-2 to get laplace operator)
!==========================================================================
      integer nca,ncb,dx,dy,dz,dt,degree,degreeTime
      real xa(ndra:ndrb,ndsa:ndsb,ndta:ndtb)
      real ya(ndra:ndrb,ndsa:ndsb,ndta:ndtb)
      real za(ndra:ndrb,ndsa:ndsb,ndta:ndtb)
      real r(ndrra:ndrrb,ndrsa:ndrsb,ndrta:ndrtb,ndrca:ndrcb)
      real c(0:ndc1-1,0:ndc2-1,0:ndc3-1,0:*)
      real a(0:nda-1,0:*)
      real t

      if( nd.eq.1 )then
        polyDimension(1)
      else if( nd.eq.2 )then
        polyDimension(2)
      else
        polyDimension(3)
      end if

      return
      end






! ************************************************************************************
!  Here are the versions for calling one point at a time
! ************************************************************************************


#beginMacro beginLoops(arg)
arg
#endMacro
#beginMacro endLoops()
#endMacro


#beginMacro polyFun(type)
subroutine polyEvaluate ## type (r, x1,y1,z1,n,t, ndc1,ndc2,ndc3,nda, \
 degree, degreeTime, a, c, dx,dy,dz,dt)
!==========================================================================
!==========================================================================
integer ndc1,ndc2,ndc3,nda
integer n,dx,dy,dz,dt,degree,degreeTime
real x1,y1,z1,r
real c(0:ndc1-1,0:ndc2-1,0:ndc3-1,0:*)
real a(0:nda-1,0:*)
real t

integer laplace
real time

laplace=0
if( dx.eq.-2 )then
laplace=1
end if

#If #type eq "3D5" || #type eq "3D6"
#ifndef IFORT_COMPILER
#End

poly ## type(c,c,c,r)

#If #type eq "3D5" || #type eq "3D6"
#else
write(*,'(''ERROR: poly ## type not available for ifort'')')
#endif
#End

return
end
#endMacro


#beginMacro buildFile(file,type)
#beginFile file
polyFun(type)
#endFile
#endMacro


#beginMacro appendFile(file,type)
#appendFile file
polyFun(type)
#endFile
#endMacro

!  *** NOTE: we build .F files since the ifort compiler has trouble with some files ***
      buildFile(polyEvaluate1D.F,1D0)
      appendFile(polyEvaluate1D.F,1D1)
      appendFile(polyEvaluate1D.F,1D2)
      appendFile(polyEvaluate1D.F,1D3)
      appendFile(polyEvaluate1D.F,1D4)
      appendFile(polyEvaluate1D.F,1D5)
      appendFile(polyEvaluate1D.F,1D6)

      buildFile(polyEvaluate2D.F,2D0)
      appendFile(polyEvaluate2D.F,2D1)
      appendFile(polyEvaluate2D.F,2D2)
      appendFile(polyEvaluate2D.F,2D3)
      appendFile(polyEvaluate2D.F,2D4)
      appendFile(polyEvaluate2D.F,2D5)
      appendFile(polyEvaluate2D.F,2D6)

      buildFile(polyEvaluate3D.F,3D0)
      appendFile(polyEvaluate3D.F,3D1)
      appendFile(polyEvaluate3D.F,3D2)
      appendFile(polyEvaluate3D.F,3D3)
      appendFile(polyEvaluate3D.F,3D4)
      appendFile(polyEvaluate3D.F,3D5)
      appendFile(polyEvaluate3D.F,3D6)

!     buildFile(polyFunction3D.f,poly3D0)

#beginMacro polyDegree(ND,DEGREE)
 call polyEvaluate ## ND D ## DEGREE(r, x,y,z,n,t, ndc1,ndc2,ndc3,nda, \
  degree, degreeTime, a, c, dx,dy,dz,dt)
#endMacro

#beginMacro polyDimension(ND)
if( degree.eq.0 )then
 polyDegree(ND,0)
else if( degree.eq.1 )then
 polyDegree(ND,1)
else if( degree.eq.2 )then
 polyDegree(ND,2)
else if( degree.eq.3 )then
 polyDegree(ND,3)
else if( degree.eq.4 )then
 polyDegree(ND,4)
else if( degree.eq.5 )then
 polyDegree(ND,5)
else if( degree.eq.6 )then
 polyDegree(ND,6)
end if
#endMacro

      subroutine polyEvaluate(nd, r, x,y,z,n,t, ndc1,ndc2,ndc3,nda, 
     & degree, degreeTime, a, c, dx,dy,dz,dt)
!==========================================================================
!   *** Define a polynomial function and it's derivatives ***
! nd : number of space dimensions
! nra,nrb,nsa,nsb,nta,ntb : return result in this array
! c : array of polynomial coefficients
! r  : return result in this array
! degree: degree of the polynomial
! dx,dy,dz,dt: compute this derivative (set dx==-2 to get laplace operator)
!==========================================================================
      integer nd,ndc1,ndc2,ndc3,nda
      integer n,dx,dy,dz,dt,degree,degreeTime
      real x,y,z,r
      real c(0:ndc1-1,0:ndc2-1,0:ndc3-1,0:*)
      real a(0:nda-1,0:*)
      real t

      if( nd.eq.1 )then
        polyDimension(1)
      else if( nd.eq.2 )then
        polyDimension(2)
      else
        polyDimension(3)
      end if

      return
      end







