! This file automatically generated from dsg.bf with bpp.
c This file contains:
c
c   divScalarGradFDeriv  : main interface to compute div, Laplace, divScalarGrad, divTensorGrad
c
c   divScalarGradFDeriv21: conservative 2nd-order 1D : Laplace, divScalarGrad, divTensorGrad, derivScalarDeriv
c   divScalarGradFDeriv22: conservative 2nd-order 2D : Laplace, divScalarGrad, divTensorGrad, derivScalarDeriv
c   divScalarGradFDeriv23: conservative 2nd-order 3D : Laplace, divScalarGrad, divTensorGrad, derivScalarDeriv
c       
c   divScalarGradFDeriv21R: rectangular: 
c                           conservative 2nd-order 1D : Laplace, divScalarGrad, divTensorGrad, derivScalarDeriv
c   divScalarGradFDeriv22R: rectangular: 
c                           conservative 2nd-order 2D : Laplace, divScalarGrad, divTensorGrad, derivScalarDeriv
c   divScalarGradFDeriv23R: rectangular: 
c                           conservative 2nd-order 3D : Laplace, divScalarGrad, divTensorGrad, derivScalarDeriv
c
c Macro nonConservativeNew(operator) -->
c   laplaceNCNew : non-conservative
c   divScalarGradNCNew : non-conservative
c   derivativeScalarDerivativeNCNew : non-conservative




c The next include defines the macros for conservative approximations
! #Include "../include/defineConservative.h"
c Define macros for conservative approximations.
c These are used by the forward and inverse operators
c   included in files: dsg.bf, dsgc4.bf, dsgc6.bf and opcoeff.bf


c get coefficients for 1D



c --------------------------------------------------------------------------------------------

c This macro defines Da(sDb) where a=x,y and b=x,y

c =======================================================================
c  Get coefficients for 2D
c =======================================================================

c 


c --------------------------------------------------------------------------------------------


c 
c =======================================================================
c  Get coefficients for 2D
c =======================================================================



c --------------------------------------------------------------------------------------------

c  define a macro






c --------------------------------------------------------------------------------------------

c ===========================================================================================
c Define the coefficients for divScalarGrad, divTensorGrad and derivativeScalarDerivative
c    For 2d rectangular
c============================================================================================

c --------------------------------------------------------------------------------------------


c --------------------------------------------------------------------------------------------


      subroutine divScalarGradFDeriv( nd,
     &    nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &    ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b, ! dimensions for u
     &    ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b, ! dimensions for deriv
     &    n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,
     &    dx, dr,
     &    rsxy, jac, u,s, deriv,
     &    ndw,w,  ! work space
     &    derivOption, derivType, gridType, order, averagingType,
     &    dir1, dir2  )
c======================================================================
c  Discretizations for
c           div          (see div.bf) 
c           Laplace
c           div( s grad )
c           div( tensor Grad )
c           derivativeScalarDerivative
c  
c ca,cb : assign components c=ca,..,cb (base 0)
c derivOption : 0=laplace, 1=divScalarGrad, 2=derivativeScalarDerivative, 3=divTensorGrad
c derivType : 0=nonconservative, 1=conservative, 2=conservative+symmetric
c gridType: 0=rectangular, 1=non-rectangular
c order : 2 or 4
c averagingType : arithmeticAverage=0, harmonicAverage=1
c dir1,dir2 : for derivOption=derivativeScalarDerivative
c rsxy : not used if rectangular
c dr : 
c 
c======================================================================
c      implicit none
      integer nd,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b,
     &  n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,
     &  derivOption, derivType, gridType, order, averagingType,ndw,
     & dir1,dir2

      real dx(3),dr(3)
      real rsxy(*)
      real jac(*)
      real s(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      real u(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,ndu4a:ndu4b)
      real deriv(ndd1a:ndd1b,ndd2a:ndd2b,ndd3a:ndd3b,ndd4a:ndd4b)
      real w(0:*)

      real h21(3),d22(3),d12(3),h22(3)
      real d24(3),d14(3),h42(3),h41(3)

      integer n,nda,ndwMin
      integer laplace,divScalarGrad,derivativeScalarDerivative,
     & divTensorGrad,divergence
      parameter(laplace=0,divScalarGrad=1,derivativeScalarDerivative=2,
     & divTensorGrad=3,divergence=4)

      integer rectangular,curvilinear
      parameter( rectangular=0,curvilinear=1 )

      integer nonConservative,conservative,conservativeAndSymmetric
      parameter( nonConservative=0, conservative=1, 
     & conservativeAndSymmetric=2)




      do n=1,3
        d12(n)=1./(2.*dr(n))
        d22(n)=1./(dr(n)**2)
        d14(n)=1./(12.*dr(n))
        d24(n)=1./(12.*dr(n)**2)

        h21(n)=1./(2.*dx(n))
        h22(n)=1./(dx(n)**2)
        h41(n)=1./(12.*dx(n))
        h42(n)=1./(12.*dx(n)**2)

      end do

      if( derivOption .eq. divergence )then

        ! *new* 051016

        call divergenceFDeriv( nd,
     &    nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &    ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b, ! dimensions for u
     &    ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b, ! dimensions for deriv
     &    n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,
     &    dx, dr,
     &    rsxy, jac, u,s, deriv,
     &    ndw,w,  ! work space
     &    derivOption, derivType, gridType, order, averagingType,
     &    dir1, dir2  )

      else if( derivType.eq.nonConservative )then
c       *** non-conservative *** 

        if( derivOption .eq. laplace )then
          call laplaceNCNew( nd,
     &    nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &    ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     &    ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b,
     &    n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,
     &    dr,dx,
     &    rsxy, u,s, deriv,
     &    derivOption, gridType, order, averagingType, dir1, dir2 )

        else if( derivOption .eq. divScalarGrad .or. derivOption .eq. 
     & divTensorGrad )then

          call divScalarGradNCNew( nd,
     &    nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &    ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     &    ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b,
     &    n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,
     &    dr,dx,
     &    rsxy, u,s, deriv,
     &    derivOption, gridType, order, averagingType, dir1, dir2 )

        else

          call derivativeScalarDerivativeNCNew( nd,
     &    nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &    ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     &    ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b,
     &    n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,
     &    dr,dx,
     &    rsxy, u,s, deriv,
     &    derivOption, gridType, order, averagingType, dir1, dir2 )

        end if

      else if( derivType.eq.conservative .and. order.ge.4 )then

c       ***  conservative higher order ***

        if( derivOption .eq. laplace )then
            ! conservative 4th order laplacian
          if( order.eq.4 )then
            call laplace4Cons( nd,
     &      nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &      ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     &      ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b,
     &      n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,
     &      dr,dx,
     &      rsxy, u,s, deriv,
     &      derivOption, gridType, order, averagingType, dir1, dir2 )
          else if( order.eq.6 )then
            call laplace6Cons( nd,
     &      nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &      ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     &      ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b,
     &      n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,
     &      dr,dx,
     &      rsxy, u,s, deriv,
     &      derivOption, gridType, order, averagingType, dir1, dir2 )
          else if( order.eq.8 )then
            call laplace8Cons( nd,
     &      nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &      ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     &      ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b,
     &      n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,
     &      dr,dx,
     &      rsxy, u,s, deriv,
     &      derivOption, gridType, order, averagingType, dir1, dir2 )
          else
             write(*,*) 'laplaceCons:ERROR: order=',order
             stop 5
          end if

        else if( derivOption .eq. divScalarGrad )then

          if( order.eq.4 )then
            ! conservative 4th order divScalarGrad
           call divScalarGrad4Cons( nd,
     &     nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &     ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     &     ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b,
     &     n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,
     &     dr,dx,
     &     rsxy, u,s, deriv,
     &     derivOption, gridType, order, averagingType, dir1, dir2 )
         else if( order.eq.6 )then
           call divScalarGrad6Cons( nd,
     &     nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &     ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     &     ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b,
     &     n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,
     &     dr,dx,
     &     rsxy, u,s, deriv,
     &     derivOption, gridType, order, averagingType, dir1, dir2 )
         else if( order.eq.8 )then
           call divScalarGrad8Cons( nd,
     &     nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &     ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     &     ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b,
     &     n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,
     &     dr,dx,
     &     rsxy, u,s, deriv,
     &     derivOption, gridType, order, averagingType, dir1, dir2 )
         else
           write(*,*) 'divScalarGrad:ERROR: order=',order
           stop 5
         end if

        else if( derivOption .eq. divTensorGrad )then

          ! *** conservative divTensorGrad ***

         if( order.eq.4 )then
           call divTensorGrad4Cons( nd,
     &     nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &     ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     &     ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b,
     &     n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,
     &     dr,dx,
     &     rsxy, u,s, deriv,
     &     derivOption, gridType, order, averagingType, dir1, dir2 )
         else if( order.eq.6 )then
           call divTensorGrad6Cons( nd,
     &     nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &     ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     &     ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b,
     &     n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,
     &     dr,dx,
     &     rsxy, u,s, deriv,
     &     derivOption, gridType, order, averagingType, dir1, dir2 )
         else if( order.eq.8 )then
           call divTensorGrad8Cons( nd,
     &     nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &     ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     &     ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b,
     &     n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,
     &     dr,dx,
     &     rsxy, u,s, deriv,
     &     derivOption, gridType, order, averagingType, dir1, dir2 )
         else
           write(*,*) 'divTensorGrad:ERROR: order=',order
           stop 5
         end if

        else

          ! *** derivative-scalar-derivative ***

          call derivativeScalarDerivativeNCNew( nd,
     &    nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &    ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     &    ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b,
     &    n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,
     &    dr,dx,
     &    rsxy, u,s, deriv,
     &    derivOption, gridType, order, averagingType, dir1, dir2 )

        end if

      else

c       *** conservative and 2nd-order ***

        if( gridType .eq. rectangular )then
c         === rectangular ===
          if( order .eq. 2 )then
c           +++ 2nd order +++
            nda=(nd1b-nd1a+1)*(nd2b-nd2a+1)*(nd3b-nd3a+1)
            if( derivOption.eq.2 )then
              ndwMin=nda  ! work space size
            else
              ndwMin=nda*nd
            end if
            if( nd.eq.1 )then
c             one-dimension
              if( ndw .lt. nda )then
                write(*,*) 'divScalarGradFDeriv:ERROR:1D:'
                write(*,*) 'workspace too small: ndw=',ndw,' < ',nda
                return
              end if
              ! write(*,*) '******divScalarGradFDeriv22********'
              call divScalarGradFDeriv21R( nd,
     &         nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &         ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     &         ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b,
     &         n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,
     &         dx,
     &         u,s, deriv,
     &         w(0),
     &         derivOption, gridType, order, averagingType, dir1, dir2)
            else if( nd.eq.2 )then
c             two-dimensions
              if( ndw .lt. ndwMin )then
                write(*,*) 'divScalarGradFDeriv:ERROR:2D:'
                write(*,*) 'workspace too small: ndw=',ndw,' < ',ndwMin
                return
              end if
              ! write(*,*) '******divScalarGradFDeriv22********'
              call divScalarGradFDeriv22R( nd,
     &         nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &         ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     &         ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b,
     &         n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,
     &         dx,
     &         u,s, deriv,
     &         w(0),w(nda),w(2*nda),w(3*nda),
     & derivOption, gridType, order, averagingType, dir1, dir2 )
            else
c             three-dimensions
              if( ndw .lt. ndwMin )then
                write(*,*) 'divScalarGradFDeriv:ERROR:3D: '
                write(*,*) 'workspace too small: ndw=',ndw,' < ',ndwMin
                return
              end if
              call divScalarGradFDeriv23R( nd,
     &         nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &         ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     &         ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b,
     &         n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,
     &         dx,
     &         u,s, deriv,
     &         w(0),w(nda),w(2*nda),w(3*nda),w(4*nda),w(5*nda),
     &         w(6*nda),w(7*nda),w(8*nda),
     & derivOption, gridType, order, averagingType, dir1, dir2 )
            end if
          else
c           +++ 4th order +++
            if( nd.eq.1 )then
c             one-dimension
            else if( nd.eq.2 )then
c             two-dimensions
            else
c             three-dimensions
            end if
          end if

        else

c         === non-rectangular ===
          if( order .eq. 2 )then
c           +++ 2nd order +++
            do n=1,3
              d12(n)=1./(2.*dr(n))
              d22(n)=1./(dr(n)**2)
            end do
            nda=(nd1b-nd1a+1)*(nd2b-nd2a+1)*(nd3b-nd3a+1)
            ndwMin=nda*nd*nd  ! min work space size
            if( nd.eq.1 )then
c             one-dimension
              if( ndw .lt. nda )then
                write(*,*) 'divScalarGradFDeriv:ERROR:1D:'
                write(*,*) 'workspace too small: ndw=',ndw,' < ',nda
                return
              end if
              call divScalarGradFDeriv21( nd,
     &         nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &         ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     &         ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b,
     &         n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,
     &         d12,d22,
     &         rsxy, jac, u,s, deriv,
     &         w(0),
     &         derivOption, derivType, gridType, order,
     &         averagingType, dir1, dir2)
            else if( nd.eq.2 )then
c             two-dimensions
              if( ndw .lt. ndwMin )then
                write(*,*) 'divScalarGradFDeriv:ERROR:2D:'
                write(*,*) 'workspace too small: ndw=',ndw,' < ',ndwMin
                return
              end if
              call divScalarGradFDeriv22( nd,
     &         nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &         ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     &         ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b,
     &         n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,
     &         d12,d22,
     &         rsxy, jac, u,s, deriv,
     &         w(0),w(nda),w(2*nda),w(3*nda),
     &         derivOption, derivType, gridType, order,
     &         averagingType, dir1, dir2)
            else
c             three-dimensions
              if( ndw .lt. ndwMin )then
                write(*,*) 'divScalarGradFDeriv:ERROR:3D:'
                write(*,*) 'workspace too small: ndw=',ndw,' < ',ndwMin
                return
              end if
              call divScalarGradFDeriv23( nd,
     &         nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &         ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     &         ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b,
     &         n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,
     &         d12,d22,
     &         rsxy, jac, u,s, deriv,
     &         w(0),w(nda),w(2*nda),w(3*nda),w(4*nda),w(5*nda),
     &         w(6*nda),w(7*nda),w(8*nda),
     &         derivOption, derivType, gridType, order,
     &         averagingType, dir1, dir2)
            end if
          else
c           +++ 4th order +++
            if( nd.eq.1 )then
c             one-dimension
            else if( nd.eq.2 )then
c             two-dimensions
            else
c             three-dimensions
            end if
          end if
        end if
      end if
      return
      end


      subroutine divScalarGradFDeriv21( nd,
     &    nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &    ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b, ! dimensions for u
     &    ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b, ! dimensions for deriv
     &    n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,
     &    d12,d22,
     &    rsxy, jac, u,s, deriv,
     &    a11,  ! work space
     &    derivOption, derivType, gridType, order,
     &    averagingType, dir1, dir2 )
c ===============================================================
c 2nd order, 1D
c Conservative discretization of
c           Laplace
c           div( s grad )
c           div( tensor Grad )
c           derivativeScalarDerivative
c  
c ca,cb : assign components c=ca,..,cb (base 0)
c gridType: 0=rectangular, 1=non-rectangular
c order : 2 or 4
c rsxy : not used if rectangular
c h22 : 1/h**2 : for rectangular  
c ===============================================================

c      implicit none
      integer nd,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b,
     &  n1a,n1b,n2a,n2b,n3a,n3b, ca,cb, derivType,
     &  derivOption, gridType, order, averagingType, dir1, dir2

      integer arithmeticAverage,harmonicAverage
      parameter( arithmeticAverage=0,harmonicAverage=1 )
      integer laplace,divScalarGrad,derivativeScalarDerivative,
     & divTensorGrad,divergence
      parameter(laplace=0,divScalarGrad=1,derivativeScalarDerivative=2,
     & divTensorGrad=3,divergence=4)
      integer symmetric
      parameter( symmetric=2 )
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,1:nd,1:nd)
      real jac(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real s(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      real u(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,ndu4a:ndu4b)
      real deriv(ndd1a:ndd1b,ndd2a:ndd2b,ndd3a:ndd3b,ndd4a:ndd4b)

      real a11(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)

      real d12(*),d22(*)

      real rx,factor,sh,sj
      real urr

      integer i1,i2,i3,kd3,c,j1,j2,j3
      integer  m1a,m1b,m2a,m2b,m3a,m3b

c.......statement functions 
c.......statement functions for jacobian
      rx(i1,i2,i3)=rsxy(i1,i2,i3,  1,  1)

      ! Estimate D{-r}(i-1/2,j,k)
      urr(i1,i2,i3,c)=u(i1,i2,i3,c)-u(i1-1,i2,i3,c)

c.......end statement functions

      kd3=nd

      ! define a11 using macro 
! defineA21()
      m1a=n1a-1
      m1b=n1b+1
      m2a=n2a
      m2b=n2b
      m3a=n3a
      m3b=n3b
      if( averagingType .eq. arithmeticAverage )then
        factor=.5
! GETA21(s(j1,j2,j3,0)*jac(j1,j2,j3),c,factor)
        if( derivOption.eq.laplace )then
          do j3=m3a,m3b
            do j2=m2a,m2b
              do j1=m1a,m1b
                sj = jac(j1,j2,j3)
                a11(j1,j2,j3) = (rx(j1,j2,j3)**2)*sj
              end do
            end do
          end do
        else if( derivOption.eq.divScalarGrad .or. derivOption .eq. 
     & divTensorGrad )then
          do j3=m3a,m3b
            do j2=m2a,m2b
              do j1=m1a,m1b
                sj = s(j1,j2,j3,0)*jac(j1,j2,j3)
                a11(j1,j2,j3) = (rx(j1,j2,j3)**2)*sj
              end do
            end do
          end do
        else if( derivOption.eq.derivativeScalarDerivative )then
          if(      dir1.eq.0 .and. dir2.eq.0 )then
! DXSDY21(x,x,s(j1,j2,j3,0)*jac(j1,j2,j3))
            do j3=m3a,m3b
              do j2=m2a,m2b
                do j1=m1a,m1b
                  sj = s(j1,j2,j3,0)*jac(j1,j2,j3)
                  a11(j1,j2,j3) = (r x (j1,j2,j3)*r x (j1,j2,j3))*sj
                end do
              end do
            end do
          else
            write(*,*) 'ERROR invalid values: dir1=',dir1,' dir2=',dir2
          end if
        end if
        m1a=n1a
        do j3=m3a,m3b
          do j2=m2a,m2b
            do j1=m1b,m1a,-1 ! go backwards ** worry about division by zero
c     sh=s(j1,j2,j3,0)*s(j1-1,j2,j3,0)/(s(j1,j2,j3,0)+s(j1-1,j2,j3,0))
              a11(j1,j2,j3) = factor *d22(1)*(a11(j1,j2,j3)+a11(j1-1,
     & j2,j3))
            end do
          end do
        end do
        m1a=n1a-1
      else
c       Harmonic average
      factor=2.
c       do not average in s:  
! GETA21(jac(j1,j2,j3), ,sh)
      if( derivOption.eq.laplace )then
        do j3=m3a,m3b
          do j2=m2a,m2b
            do j1=m1a,m1b
              sj = jac(j1,j2,j3)
              a11(j1,j2,j3) = (rx(j1,j2,j3)**2)*sj
            end do
          end do
        end do
      else if( derivOption.eq.divScalarGrad .or. derivOption .eq. 
     & divTensorGrad )then
        do j3=m3a,m3b
          do j2=m2a,m2b
            do j1=m1a,m1b
              sj = jac(j1,j2,j3)
              a11(j1,j2,j3) = (rx(j1,j2,j3)**2)*sj
            end do
          end do
        end do
      else if( derivOption.eq.derivativeScalarDerivative )then
        if(      dir1.eq.0 .and. dir2.eq.0 )then
! DXSDY21(x,x,jac(j1,j2,j3))
          do j3=m3a,m3b
            do j2=m2a,m2b
              do j1=m1a,m1b
                sj = jac(j1,j2,j3)
                a11(j1,j2,j3) = (r x (j1,j2,j3)*r x (j1,j2,j3))*sj
              end do
            end do
          end do
        else
          write(*,*) 'ERROR invalid values: dir1=',dir1,' dir2=',dir2
        end if
      end if
      m1a=n1a
      do j3=m3a,m3b
        do j2=m2a,m2b
          do j1=m1b,m1a,-1 ! go backwards ** worry about division by zero
           sh=s(j1,j2,j3,0)*s(j1-1,j2,j3,0)/(s(j1,j2,j3,0)+s(j1-1,j2,
     & j3,0))
            a11(j1,j2,j3) = sh *d22(1)*(a11(j1,j2,j3)+a11(j1-1,j2,j3))
          end do
        end do
      end do
      m1a=n1a-1
      end if

c    Evaluate the derivative
      do c=ca,cb
        do i3=n3a,n3b
          do i2=n2a,n2b
            do i1=n1a,n1b
              deriv(i1,i2,i3,c)=
     &         (
     &         (a11(i1+1,i2  ,i3  )*urr(i1+1,i2  ,i3  ,c) -
     &          a11(i1  ,i2  ,i3  )*urr(i1  ,i2  ,i3  ,c))
     &        )/jac(i1,i2,i3)
            end do
          end do
        end do
      end do

      return
      end




c This next macro defines the operator D_X( s D_Y u )





c =============== NEW VERSION WITH HIGHER ORDER OPERATORS =====================

c This next macro defines the operator D_X( s D_Y u )



c These next include files will define the macros that will define the difference approximations
c The actual macro is called below
! #Include "../src/defineDiffOrder2f.h"
c Use this next macro to declare the statement functions that are defined below
c To include derivatives of rx use OPTION=RX


c Define statement functions for difference approximations of order 2 
c To include derivatives of rx use OPTION=RX
c To include derivatives of rx use OPTION=RX



! #Include "../src/defineDiffOrder4f.h"
c Use this next macro to declare the statement functions that are defined below
c To include derivatives of rx use OPTION=RX


c Define statement functions for difference approximations of order 4 
c To include derivatives of rx use OPTION=RX
c To include derivatives of rx use OPTION=RX
! #Include "../src/defineDiffOrder6f.h"
c Use this next macro to declare the statement functions that are defined below
c To include derivatives of rx use OPTION=RX


c Define statement functions for difference approximations of order 6 
c To include derivatives of rx use OPTION=RX
c To include derivatives of rx use OPTION=RX
! #Include "../src/defineDiffOrder8f.h"
c Use this next macro to declare the statement functions that are defined below
c To include derivatives of rx use OPTION=RX


c Define statement functions for difference approximations of order 8 
c To include derivatives of rx use OPTION=RX
c To include derivatives of rx use OPTION=RX

! Macro to evaluate the divTensorGrad operator in 2D (non-conservative)
! DTYPE: 22R, 22, 42R, ...
!    deriv = SUM  Dm( s(m,n) Dn )


! Macro to evaluate the divTensorGrad operator in 2D (non-conservative)
! DTYPE: 23R, 23, 43R, ...
!    deriv = SUM  Dm( s(m,n) Dn )

! Macro to evaluate the divTensorGrad operator in 1D (non-conservative)
! DTYPE: 21R, 21, 41R, ...
!    deriv = SUM  Dm( s(m,n) Dn )











! buildFileNC(laplaceNC)
! buildFileNC(divScalarGradNC)
! buildFileNC(derivativeScalarDerivativeNC)


