! This file automatically generated from polyFunction.bf with bpp.
! #Include "polyFunction.h"
! ====== This file created by  polyFunction.p =====
! ****** 1 dimensions *********
! ****** degree 0 *********
! ****** degree 1 *********
! ****** degree 2 *********
! ****** degree 3 *********
! ****** degree 4 *********
! ****** degree 5 *********
! ****** degree 6 *********
! ****** 2 dimensions *********
! ****** degree 0 *********
! ****** degree 1 *********
! ****** degree 2 *********
! ****** degree 3 *********
! ****** degree 4 *********
! ****** degree 5 *********
! ****** degree 6 *********
! ****** 3 dimensions *********
! ****** degree 0 *********
! ****** degree 1 *********
! ****** degree 2 *********
! ****** degree 3 *********
! ****** degree 4 *********
! ****** degree 5 *********
! ****** degree 6 *********

c ************************************************************************************
c  Here are the versions for evaluating many points at a time
c ************************************************************************************


! --- evaluate time derivatives of the polynomial ---









! buildFile(polyFunction1D.f,1D0)
! appendFile(polyFunction1D.f,1D1)
! appendFile(polyFunction1D.f,1D2)
! appendFile(polyFunction1D.f,1D3)
! appendFile(polyFunction1D.f,1D4)
! appendFile(polyFunction1D.f,1D5)
! appendFile(polyFunction1D.f,1D6)

! buildFile(polyFunction2D.f,2D0)
! appendFile(polyFunction2D.f,2D1)
! appendFile(polyFunction2D.f,2D2)
! appendFile(polyFunction2D.f,2D3)
! appendFile(polyFunction2D.f,2D4)
! buildFile(polyFunction2D5.f,2D5)
! buildFile(polyFunction2D6.f,2D6)

! buildFile(polyFunction3D.f,3D0)
! appendFile(polyFunction3D.f,3D1)
! appendFile(polyFunction3D.f,3D2)
! appendFile(polyFunction3D.f,3D3)
! buildFile(polyFunction3D4.f,3D4)
! buildFile(polyFunction3D5.f,3D5)
! buildFile(polyFunction3D6.f,3D6)

!     buildFile(polyFunction3D.f,poly3D0)



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
! polyDimension(1)
        if( degree.eq.0 )then
! polyDegree(1,0)
          call polyFunction1 D0(nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,ndrra,
     & ndrrb,ndrsa,ndrsb,ndrta,ndrtb,ndrca,ndrcb,ndc1,ndc2,ndc3,nra,
     & nrb,nsa,nsb,nta,ntb, nca,ncb, nda, degree, degreeTime, t, a,c, 
     & r,xa,ya,za, dx,dy,dz,dt)
        else if( degree.eq.1 )then
! polyDegree(1,1)
          call polyFunction1 D1(nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,ndrra,
     & ndrrb,ndrsa,ndrsb,ndrta,ndrtb,ndrca,ndrcb,ndc1,ndc2,ndc3,nra,
     & nrb,nsa,nsb,nta,ntb, nca,ncb, nda, degree, degreeTime, t, a,c, 
     & r,xa,ya,za, dx,dy,dz,dt)
        else if( degree.eq.2 )then
! polyDegree(1,2)
          call polyFunction1 D2(nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,ndrra,
     & ndrrb,ndrsa,ndrsb,ndrta,ndrtb,ndrca,ndrcb,ndc1,ndc2,ndc3,nra,
     & nrb,nsa,nsb,nta,ntb, nca,ncb, nda, degree, degreeTime, t, a,c, 
     & r,xa,ya,za, dx,dy,dz,dt)
        else if( degree.eq.3 )then
! polyDegree(1,3)
          call polyFunction1 D3(nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,ndrra,
     & ndrrb,ndrsa,ndrsb,ndrta,ndrtb,ndrca,ndrcb,ndc1,ndc2,ndc3,nra,
     & nrb,nsa,nsb,nta,ntb, nca,ncb, nda, degree, degreeTime, t, a,c, 
     & r,xa,ya,za, dx,dy,dz,dt)
        else if( degree.eq.4 )then
! polyDegree(1,4)
          call polyFunction1 D4(nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,ndrra,
     & ndrrb,ndrsa,ndrsb,ndrta,ndrtb,ndrca,ndrcb,ndc1,ndc2,ndc3,nra,
     & nrb,nsa,nsb,nta,ntb, nca,ncb, nda, degree, degreeTime, t, a,c, 
     & r,xa,ya,za, dx,dy,dz,dt)
        else if( degree.eq.5 )then
! polyDegree(1,5)
          call polyFunction1 D5(nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,ndrra,
     & ndrrb,ndrsa,ndrsb,ndrta,ndrtb,ndrca,ndrcb,ndc1,ndc2,ndc3,nra,
     & nrb,nsa,nsb,nta,ntb, nca,ncb, nda, degree, degreeTime, t, a,c, 
     & r,xa,ya,za, dx,dy,dz,dt)
        else if( degree.eq.6 )then
! polyDegree(1,6)
          call polyFunction1 D6(nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,ndrra,
     & ndrrb,ndrsa,ndrsb,ndrta,ndrtb,ndrca,ndrcb,ndc1,ndc2,ndc3,nra,
     & nrb,nsa,nsb,nta,ntb, nca,ncb, nda, degree, degreeTime, t, a,c, 
     & r,xa,ya,za, dx,dy,dz,dt)
        end if
      else if( nd.eq.2 )then
! polyDimension(2)
        if( degree.eq.0 )then
! polyDegree(2,0)
          call polyFunction2 D0(nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,ndrra,
     & ndrrb,ndrsa,ndrsb,ndrta,ndrtb,ndrca,ndrcb,ndc1,ndc2,ndc3,nra,
     & nrb,nsa,nsb,nta,ntb, nca,ncb, nda, degree, degreeTime, t, a,c, 
     & r,xa,ya,za, dx,dy,dz,dt)
        else if( degree.eq.1 )then
! polyDegree(2,1)
          call polyFunction2 D1(nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,ndrra,
     & ndrrb,ndrsa,ndrsb,ndrta,ndrtb,ndrca,ndrcb,ndc1,ndc2,ndc3,nra,
     & nrb,nsa,nsb,nta,ntb, nca,ncb, nda, degree, degreeTime, t, a,c, 
     & r,xa,ya,za, dx,dy,dz,dt)
        else if( degree.eq.2 )then
! polyDegree(2,2)
          call polyFunction2 D2(nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,ndrra,
     & ndrrb,ndrsa,ndrsb,ndrta,ndrtb,ndrca,ndrcb,ndc1,ndc2,ndc3,nra,
     & nrb,nsa,nsb,nta,ntb, nca,ncb, nda, degree, degreeTime, t, a,c, 
     & r,xa,ya,za, dx,dy,dz,dt)
        else if( degree.eq.3 )then
! polyDegree(2,3)
          call polyFunction2 D3(nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,ndrra,
     & ndrrb,ndrsa,ndrsb,ndrta,ndrtb,ndrca,ndrcb,ndc1,ndc2,ndc3,nra,
     & nrb,nsa,nsb,nta,ntb, nca,ncb, nda, degree, degreeTime, t, a,c, 
     & r,xa,ya,za, dx,dy,dz,dt)
        else if( degree.eq.4 )then
! polyDegree(2,4)
          call polyFunction2 D4(nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,ndrra,
     & ndrrb,ndrsa,ndrsb,ndrta,ndrtb,ndrca,ndrcb,ndc1,ndc2,ndc3,nra,
     & nrb,nsa,nsb,nta,ntb, nca,ncb, nda, degree, degreeTime, t, a,c, 
     & r,xa,ya,za, dx,dy,dz,dt)
        else if( degree.eq.5 )then
! polyDegree(2,5)
          call polyFunction2 D5(nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,ndrra,
     & ndrrb,ndrsa,ndrsb,ndrta,ndrtb,ndrca,ndrcb,ndc1,ndc2,ndc3,nra,
     & nrb,nsa,nsb,nta,ntb, nca,ncb, nda, degree, degreeTime, t, a,c, 
     & r,xa,ya,za, dx,dy,dz,dt)
        else if( degree.eq.6 )then
! polyDegree(2,6)
          call polyFunction2 D6(nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,ndrra,
     & ndrrb,ndrsa,ndrsb,ndrta,ndrtb,ndrca,ndrcb,ndc1,ndc2,ndc3,nra,
     & nrb,nsa,nsb,nta,ntb, nca,ncb, nda, degree, degreeTime, t, a,c, 
     & r,xa,ya,za, dx,dy,dz,dt)
        end if
      else
! polyDimension(3)
        if( degree.eq.0 )then
! polyDegree(3,0)
          call polyFunction3 D0(nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,ndrra,
     & ndrrb,ndrsa,ndrsb,ndrta,ndrtb,ndrca,ndrcb,ndc1,ndc2,ndc3,nra,
     & nrb,nsa,nsb,nta,ntb, nca,ncb, nda, degree, degreeTime, t, a,c, 
     & r,xa,ya,za, dx,dy,dz,dt)
        else if( degree.eq.1 )then
! polyDegree(3,1)
          call polyFunction3 D1(nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,ndrra,
     & ndrrb,ndrsa,ndrsb,ndrta,ndrtb,ndrca,ndrcb,ndc1,ndc2,ndc3,nra,
     & nrb,nsa,nsb,nta,ntb, nca,ncb, nda, degree, degreeTime, t, a,c, 
     & r,xa,ya,za, dx,dy,dz,dt)
        else if( degree.eq.2 )then
! polyDegree(3,2)
          call polyFunction3 D2(nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,ndrra,
     & ndrrb,ndrsa,ndrsb,ndrta,ndrtb,ndrca,ndrcb,ndc1,ndc2,ndc3,nra,
     & nrb,nsa,nsb,nta,ntb, nca,ncb, nda, degree, degreeTime, t, a,c, 
     & r,xa,ya,za, dx,dy,dz,dt)
        else if( degree.eq.3 )then
! polyDegree(3,3)
          call polyFunction3 D3(nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,ndrra,
     & ndrrb,ndrsa,ndrsb,ndrta,ndrtb,ndrca,ndrcb,ndc1,ndc2,ndc3,nra,
     & nrb,nsa,nsb,nta,ntb, nca,ncb, nda, degree, degreeTime, t, a,c, 
     & r,xa,ya,za, dx,dy,dz,dt)
        else if( degree.eq.4 )then
! polyDegree(3,4)
          call polyFunction3 D4(nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,ndrra,
     & ndrrb,ndrsa,ndrsb,ndrta,ndrtb,ndrca,ndrcb,ndc1,ndc2,ndc3,nra,
     & nrb,nsa,nsb,nta,ntb, nca,ncb, nda, degree, degreeTime, t, a,c, 
     & r,xa,ya,za, dx,dy,dz,dt)
        else if( degree.eq.5 )then
! polyDegree(3,5)
          call polyFunction3 D5(nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,ndrra,
     & ndrrb,ndrsa,ndrsb,ndrta,ndrtb,ndrca,ndrcb,ndc1,ndc2,ndc3,nra,
     & nrb,nsa,nsb,nta,ntb, nca,ncb, nda, degree, degreeTime, t, a,c, 
     & r,xa,ya,za, dx,dy,dz,dt)
        else if( degree.eq.6 )then
! polyDegree(3,6)
          call polyFunction3 D6(nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,ndrra,
     & ndrrb,ndrsa,ndrsb,ndrta,ndrtb,ndrca,ndrcb,ndc1,ndc2,ndc3,nra,
     & nrb,nsa,nsb,nta,ntb, nca,ncb, nda, degree, degreeTime, t, a,c, 
     & r,xa,ya,za, dx,dy,dz,dt)
        end if
      end if

      return
      end






! ************************************************************************************
!  Here are the versions for calling one point at a time
! ************************************************************************************









!  *** NOTE: we build .F files since the ifort compiler has trouble with some files ***
! buildFile(polyEvaluate1D.F,1D0)
! appendFile(polyEvaluate1D.F,1D1)
! appendFile(polyEvaluate1D.F,1D2)
! appendFile(polyEvaluate1D.F,1D3)
! appendFile(polyEvaluate1D.F,1D4)
! appendFile(polyEvaluate1D.F,1D5)
! appendFile(polyEvaluate1D.F,1D6)

! buildFile(polyEvaluate2D.F,2D0)
! appendFile(polyEvaluate2D.F,2D1)
! appendFile(polyEvaluate2D.F,2D2)
! appendFile(polyEvaluate2D.F,2D3)
! appendFile(polyEvaluate2D.F,2D4)
! appendFile(polyEvaluate2D.F,2D5)
! appendFile(polyEvaluate2D.F,2D6)

! buildFile(polyEvaluate3D.F,3D0)
! appendFile(polyEvaluate3D.F,3D1)
! appendFile(polyEvaluate3D.F,3D2)
! appendFile(polyEvaluate3D.F,3D3)
! appendFile(polyEvaluate3D.F,3D4)
! appendFile(polyEvaluate3D.F,3D5)
! appendFile(polyEvaluate3D.F,3D6)

!     buildFile(polyFunction3D.f,poly3D0)



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
! polyDimension(1)
        if( degree.eq.0 )then
! polyDegree(1,0)
          call polyEvaluate1 D0(r, x,y,z,n,t, ndc1,ndc2,ndc3,nda, 
     & degree, degreeTime, a, c, dx,dy,dz,dt)
        else if( degree.eq.1 )then
! polyDegree(1,1)
          call polyEvaluate1 D1(r, x,y,z,n,t, ndc1,ndc2,ndc3,nda, 
     & degree, degreeTime, a, c, dx,dy,dz,dt)
        else if( degree.eq.2 )then
! polyDegree(1,2)
          call polyEvaluate1 D2(r, x,y,z,n,t, ndc1,ndc2,ndc3,nda, 
     & degree, degreeTime, a, c, dx,dy,dz,dt)
        else if( degree.eq.3 )then
! polyDegree(1,3)
          call polyEvaluate1 D3(r, x,y,z,n,t, ndc1,ndc2,ndc3,nda, 
     & degree, degreeTime, a, c, dx,dy,dz,dt)
        else if( degree.eq.4 )then
! polyDegree(1,4)
          call polyEvaluate1 D4(r, x,y,z,n,t, ndc1,ndc2,ndc3,nda, 
     & degree, degreeTime, a, c, dx,dy,dz,dt)
        else if( degree.eq.5 )then
! polyDegree(1,5)
          call polyEvaluate1 D5(r, x,y,z,n,t, ndc1,ndc2,ndc3,nda, 
     & degree, degreeTime, a, c, dx,dy,dz,dt)
        else if( degree.eq.6 )then
! polyDegree(1,6)
          call polyEvaluate1 D6(r, x,y,z,n,t, ndc1,ndc2,ndc3,nda, 
     & degree, degreeTime, a, c, dx,dy,dz,dt)
        end if
      else if( nd.eq.2 )then
! polyDimension(2)
        if( degree.eq.0 )then
! polyDegree(2,0)
          call polyEvaluate2 D0(r, x,y,z,n,t, ndc1,ndc2,ndc3,nda, 
     & degree, degreeTime, a, c, dx,dy,dz,dt)
        else if( degree.eq.1 )then
! polyDegree(2,1)
          call polyEvaluate2 D1(r, x,y,z,n,t, ndc1,ndc2,ndc3,nda, 
     & degree, degreeTime, a, c, dx,dy,dz,dt)
        else if( degree.eq.2 )then
! polyDegree(2,2)
          call polyEvaluate2 D2(r, x,y,z,n,t, ndc1,ndc2,ndc3,nda, 
     & degree, degreeTime, a, c, dx,dy,dz,dt)
        else if( degree.eq.3 )then
! polyDegree(2,3)
          call polyEvaluate2 D3(r, x,y,z,n,t, ndc1,ndc2,ndc3,nda, 
     & degree, degreeTime, a, c, dx,dy,dz,dt)
        else if( degree.eq.4 )then
! polyDegree(2,4)
          call polyEvaluate2 D4(r, x,y,z,n,t, ndc1,ndc2,ndc3,nda, 
     & degree, degreeTime, a, c, dx,dy,dz,dt)
        else if( degree.eq.5 )then
! polyDegree(2,5)
          call polyEvaluate2 D5(r, x,y,z,n,t, ndc1,ndc2,ndc3,nda, 
     & degree, degreeTime, a, c, dx,dy,dz,dt)
        else if( degree.eq.6 )then
! polyDegree(2,6)
          call polyEvaluate2 D6(r, x,y,z,n,t, ndc1,ndc2,ndc3,nda, 
     & degree, degreeTime, a, c, dx,dy,dz,dt)
        end if
      else
! polyDimension(3)
        if( degree.eq.0 )then
! polyDegree(3,0)
          call polyEvaluate3 D0(r, x,y,z,n,t, ndc1,ndc2,ndc3,nda, 
     & degree, degreeTime, a, c, dx,dy,dz,dt)
        else if( degree.eq.1 )then
! polyDegree(3,1)
          call polyEvaluate3 D1(r, x,y,z,n,t, ndc1,ndc2,ndc3,nda, 
     & degree, degreeTime, a, c, dx,dy,dz,dt)
        else if( degree.eq.2 )then
! polyDegree(3,2)
          call polyEvaluate3 D2(r, x,y,z,n,t, ndc1,ndc2,ndc3,nda, 
     & degree, degreeTime, a, c, dx,dy,dz,dt)
        else if( degree.eq.3 )then
! polyDegree(3,3)
          call polyEvaluate3 D3(r, x,y,z,n,t, ndc1,ndc2,ndc3,nda, 
     & degree, degreeTime, a, c, dx,dy,dz,dt)
        else if( degree.eq.4 )then
! polyDegree(3,4)
          call polyEvaluate3 D4(r, x,y,z,n,t, ndc1,ndc2,ndc3,nda, 
     & degree, degreeTime, a, c, dx,dy,dz,dt)
        else if( degree.eq.5 )then
! polyDegree(3,5)
          call polyEvaluate3 D5(r, x,y,z,n,t, ndc1,ndc2,ndc3,nda, 
     & degree, degreeTime, a, c, dx,dy,dz,dt)
        else if( degree.eq.6 )then
! polyDegree(3,6)
          call polyEvaluate3 D6(r, x,y,z,n,t, ndc1,ndc2,ndc3,nda, 
     & degree, degreeTime, a, c, dx,dy,dz,dt)
        end if
      end if

      return
      end







