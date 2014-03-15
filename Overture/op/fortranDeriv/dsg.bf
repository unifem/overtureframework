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
#Include "../include/defineConservative.h"

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
      integer laplace,divScalarGrad,derivativeScalarDerivative,divTensorGrad,divergence
      parameter(laplace=0,divScalarGrad=1,derivativeScalarDerivative=2,divTensorGrad=3,divergence=4)
    
      integer rectangular,curvilinear
      parameter( rectangular=0,curvilinear=1 )

      integer nonConservative,conservative,conservativeAndSymmetric
      parameter( nonConservative=0, conservative=1, conservativeAndSymmetric=2)


      

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

        else if( derivOption .eq. divScalarGrad .or. derivOption .eq. divTensorGrad )then

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
     &         derivOption, gridType, order, averagingType, dir1, dir2 )
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
     &         derivOption, gridType, order, averagingType, dir1, dir2 )
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
      integer laplace,divScalarGrad,derivativeScalarDerivative,divTensorGrad,divergence
      parameter(laplace=0,divScalarGrad=1,derivativeScalarDerivative=2,divTensorGrad=3,divergence=4)
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
      defineA21()

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

#beginFile dsg2.f

      subroutine divScalarGradFDeriv22( nd, 
     &    nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,         
     &    ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b, ! dimensions for u
     &    ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b, ! dimensions for deriv
     &    n1a,n1b,n2a,n2b,n3a,n3b, ca,cb, 
     &    d12,d22,
     &    rsxy, jac, u,s, deriv, 
     &    a11,a12,a21,a22,  ! work space
     &    derivOption, derivType, gridType, order, 
     &    averagingType, dir1, dir2 )
c ===============================================================
c 2nd order, 2D
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
      integer laplace,divScalarGrad,derivativeScalarDerivative,divTensorGrad
      parameter(laplace=0,divScalarGrad=1,derivativeScalarDerivative=2,divTensorGrad=3)
      integer symmetric
      parameter( symmetric=2 )
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,1:nd,1:nd)
      real jac(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real s(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      real u(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,ndu4a:ndu4b)
      real deriv(ndd1a:ndd1b,ndd2a:ndd2b,ndd3a:ndd3b,ndd4a:ndd4b)

      real a11(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real a21(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real a12(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real a22(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)

      real d12(*),d22(*)

      real s11,s21,s31,s12,s22,s32,s13,s23,s33
      real rxj,ryj,rzj,sxj,syj,szj,txj,tyj,tzj
c      real rx,ry,rz,sx,sy,sz,tx,ty,tz,factor,sh,sj
c      real urr,uss,urs,usr

      integer i1,i2,i3,kd3,c,j1,j2,j3
      integer  m1a,m1b,m2a,m2b,m3a,m3b

c.......statement functions 
c.......statement functions for jacobian
      rx(i1,i2,i3)=rsxy(i1,i2,i3,  1,  1)
      ry(i1,i2,i3)=rsxy(i1,i2,i3,  1,  2)
c      rz(i1,i2,i3)=rsxy(i1,i2,i3,  1,kd3)
      sx(i1,i2,i3)=rsxy(i1,i2,i3,  2,  1)
      sy(i1,i2,i3)=rsxy(i1,i2,i3,  2,  2)
c      sz(i1,i2,i3)=rsxy(i1,i2,i3,  2,kd3)
c      tx(i1,i2,i3)=rsxy(i1,i2,i3,kd3,  1)
c      ty(i1,i2,i3)=rsxy(i1,i2,i3,kd3,  2)
c      tz(i1,i2,i3)=rsxy(i1,i2,i3,kd3,kd3)

      ! Estimate D{-r}(i-1/2,j,k)
      urr(i1,i2,i3,c)=u(i1,i2,i3,c)-u(i1-1,i2,i3,c) 
      ! Estimate D{-s}(i-1/2,j+1/2,k)
      urs(i1,i2,i3,c)=(u(i1-1,i2+1,i3,c)+u(i1,i2+1,i3,c)-
     &                 u(i1-1,i2-1,i3,c)-u(i1,i2-1,i3,c))
      ! Estimate D{-r}(i+1/2,j-1/2,k)
      usr(i1,i2,i3,c) = (u(i1+1,i2-1,i3,c) + u(i1+1,i2,i3,c) - 
     &                   u(i1-1,i2-1,i3,c) - u(i1-1,i2,i3,c))
      ! Estimate D{-s}(i,j-1/2,k)
      uss(i1,i2,i3,c) = u(i1,i2,i3,c)-u(i1,i2-1,i3,c)

      D0r(i1,i2,i3,c)=u(i1+1,i2,i3,c)-u(i1-1,i2,i3,c)
      D0s(i1,i2,i3,c)=u(i1,i2+1,i3,c)-u(i1,i2-1,i3,c)

c.......end statement functions

c       for now only use the symmetric formula in these cases
      if( derivOption.eq.laplace .or. 
     &    derivOption.eq.divScalarGrad )then
        derivType=symmetric 
      end if

      kd3=nd

      ! define a11,a12,a21,a22 using a macro 
      defineA22()

c     Evaluate the derivative
      if( derivType.eq.symmetric )then
c       ** here is the new symmetric formula ***
        do c=ca,cb
          do i3=n3a,n3b
            do i2=n2a,n2b
              do i1=n1a,n1b
                deriv(i1,i2,i3,c)=
     &              (
     &              (a11(i1+1,i2  ,i3  )*urr(i1+1,i2  ,i3  ,c) -
     &               a11(i1  ,i2  ,i3  )*urr(i1  ,i2  ,i3  ,c))+
     &              (a22(i1  ,i2+1,i3  )*uss(i1  ,i2+1,i3  ,c) - 
     &               a22(i1  ,i2  ,i3  )*uss(i1  ,i2  ,i3  ,c))
     &             +(a21(i1  ,i2+1,i3  )*D0r(i1  ,i2+1,i3  ,c) - 
     &               a21(i1  ,i2-1,i3  )*D0r(i1  ,i2-1,i3  ,c) +
     &               a12(i1+1,i2  ,i3  )*D0s(i1+1,i2  ,i3  ,c) - 
     &               a12(i1-1,i2  ,i3  )*D0s(i1-1,i2  ,i3  ,c))
     &               )/jac(i1,i2,i3)
              end do
            end do
          end do
        end do
      else
        do c=ca,cb
          do i3=n3a,n3b
            do i2=n2a,n2b
              do i1=n1a,n1b
                deriv(i1,i2,i3,c)=
     &               (
     &               (a11(i1+1,i2  ,i3  )*urr(i1+1,i2  ,i3  ,c) -
     &               a11(i1  ,i2  ,i3  )*urr(i1  ,i2  ,i3  ,c))+
     &               (a22(i1  ,i2+1,i3  )*uss(i1  ,i2+1,i3  ,c) - 
     &               a22(i1  ,i2  ,i3  )*uss(i1  ,i2  ,i3  ,c))+
     &               (a21(i1  ,i2+1,i3  )*usr(i1  ,i2+1,i3  ,c) - 
     &               a21(i1  ,i2  ,i3  )*usr(i1  ,i2  ,i3  ,c) +
     &               a12(i1+1,i2  ,i3  )*urs(i1+1,i2  ,i3  ,c) - 
     &               a12(i1  ,i2  ,i3  )*urs(i1  ,i2  ,i3  ,c))
     &               )/jac(i1,i2,i3)
              end do
            end do
          end do
        end do
      end if
      return 
      end

#endFile

#beginFile dsg3.f
      subroutine divScalarGradFDeriv23( nd, 
     &    nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,         
     &    ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b, ! dimensions for u
     &    ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b, ! dimensions for deriv
     &    n1a,n1b,n2a,n2b,n3a,n3b, ca,cb, 
     &    d12,d22,
     &    rsxy, jac, u,s, deriv, 
     &    a11,a12,a13,a21,a22,a23,a31,a32,a33,  ! work space
     &    derivOption, derivType, gridType, order, 
     &    averagingType, dir1, dir2 )
c ===============================================================
c 2nd order, 3D
c     Conservative discretization of
c           Laplace
c           div( s grad )
c           div( tensor Grad )
c           derivativeScalarDerivative
c  
c ca,cb : assign components c=ca,..,cb (base 0)
c derivOption : 0=laplace, 1=divScalarGrad, 2=derivativeScalarDerivative
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
      integer laplace,divScalarGrad,derivativeScalarDerivative,divTensorGrad
      parameter(laplace=0,divScalarGrad=1,derivativeScalarDerivative=2,divTensorGrad=3)
      integer symmetric
      parameter( symmetric=2 )
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,1:nd,1:nd)
      real jac(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real s(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      real u(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,ndu4a:ndu4b)
      real deriv(ndd1a:ndd1b,ndd2a:ndd2b,ndd3a:ndd3b,ndd4a:ndd4b)

      real a11(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real a21(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real a31(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real a12(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real a22(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real a32(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real a13(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real a23(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real a33(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)

      real d12(*),d22(*)

      real s11,s21,s31,s12,s22,s32,s13,s23,s33
      real rxj,ryj,rzj,sxj,syj,szj,txj,tyj,tzj

c      real rx,ry,rz,sx,sy,sz,tx,ty,tz,factor,sh,sj
c      real urr,uss,utt,urs,urt,ust,usr,utr,uts

      integer i1,i2,i3,kd3,c,j1,j2,j3
      integer  m1a,m1b,m2a,m2b,m3a,m3b

c.......statement functions 
c.......statement functions for jacobian
      rx(i1,i2,i3)=rsxy(i1,i2,i3,  1,  1)
      ry(i1,i2,i3)=rsxy(i1,i2,i3,  1,  2)
      rz(i1,i2,i3)=rsxy(i1,i2,i3,  1,kd3)
      sx(i1,i2,i3)=rsxy(i1,i2,i3,  2,  1)
      sy(i1,i2,i3)=rsxy(i1,i2,i3,  2,  2)
      sz(i1,i2,i3)=rsxy(i1,i2,i3,  2,kd3)
      tx(i1,i2,i3)=rsxy(i1,i2,i3,kd3,  1)
      ty(i1,i2,i3)=rsxy(i1,i2,i3,kd3,  2)
      tz(i1,i2,i3)=rsxy(i1,i2,i3,kd3,kd3)

      ! Estimate D{-r}(i-1/2,j,k)
      urr(i1,i2,i3,c)=u(i1,i2,i3,c)-u(i1-1,i2,i3,c) 
      ! Estimate D{-s}(i-1/2,j+1/2,k)
      urs(i1,i2,i3,c)=(u(i1-1,i2+1,i3,c)+u(i1,i2+1,i3,c)-
     &     u(i1-1,i2-1,i3,c)-u(i1,i2-1,i3,c))
      ! Estimate D{-t}(i-1/2,j,k+1/2)
      urt(i1,i2,i3,c) = (u(i1-1,i2,i3+1,c) + u(i1,i2,i3+1,c) - 
     &                   u(i1-1,i2,i3-1,c) - u(i1,i2,i3-1,c))   
      ! Estimate D{-r}(i+1/2,j,k-1/2)
      utr(i1,i2,i3,c) = (u(i1+1,i2,i3-1,c) + u(i1+1,i2,i3,c) - 
     &                   u(i1-1,i2,i3-1,c) - u(i1-1,i2,i3,c))   
      ! Estimate D{-s}(i,j+1/2,k-1/2)
      uts(i1,i2,i3,c) = (u(i1,i2+1,i3-1,c) + u(i1,i2+1,i3,c) - 
     &                   u(i1,i2-1,i3-1,c) - u(i1,i2-1,i3,c))
      ! Estimate D{-t}(i,j,k-1/2)
      utt(i1,i2,i3,c) =u(i1,i2,i3,c)-u(i1,i2,i3-1,c) 
      ! Estimate D{-r}(i+1/2,j-1/2,k)
      usr(i1,i2,i3,c) = (u(i1+1,i2-1,i3,c) + u(i1+1,i2,i3,c) - 
     &                   u(i1-1,i2-1,i3,c) - u(i1-1,i2,i3,c))
      ! Estimate D{-s}(i,j-1/2,k)
      uss(i1,i2,i3,c) = u(i1,i2,i3,c)-u(i1,i2-1,i3,c)
      ! Estimate D{-t}(i,j-1/2,k+1/2)
      ust(i1,i2,i3,c) = (u(i1,i2-1,i3+1,c) + u(i1,i2,i3+1,c) - 
     &                   u(i1,i2-1,i3-1,c) - u(i1,i2,i3-1,c))

      D0r(i1,i2,i3,c)=u(i1+1,i2,i3,c)-u(i1-1,i2,i3,c)
      D0s(i1,i2,i3,c)=u(i1,i2+1,i3,c)-u(i1,i2-1,i3,c)
      D0t(i1,i2,i3,c)=u(i1,i2,i3+1,c)-u(i1,i2,i3-1,c)

c.......end statement functions

      kd3=nd
c       for now only use the symmetric formula in these cases
      if( derivOption.eq.laplace .or. 
     &    derivOption.eq.divScalarGrad )then
        derivType=symmetric 
      end if

      defineA23()

c     Evaluate the derivative
c     Evaluate the derivative
      if( derivType.eq.symmetric )then
c       ** here is the new symmetric formula ***
        do c=ca,cb
          do i3=n3a,n3b
            do i2=n2a,n2b
              do i1=n1a,n1b
              deriv(i1,i2,i3,c)=
     &         (
     &         (a11(i1+1,i2  ,i3  )*urr(i1+1,i2  ,i3  ,c) - a11(i1,i2,i3)*urr(i1  ,i2  ,i3  ,c))+
     &         (a22(i1  ,i2+1,i3  )*uss(i1  ,i2+1,i3  ,c) - a22(i1,i2,i3)*uss(i1  ,i2  ,i3  ,c))+
     &         (a33(i1  ,i2  ,i3+1)*utt(i1  ,i2  ,i3+1,c) - a33(i1,i2,i3)*utt(i1  ,i2  ,i3  ,c))+
     &         (a21(i1  ,i2+1,i3  )*D0r(i1  ,i2+1,i3  ,c) - a21(i1,i2-1,i3)*D0r(i1  ,i2-1,i3  ,c) +
     &          a12(i1+1,i2  ,i3  )*D0s(i1+1,i2  ,i3  ,c) - a12(i1-1,i2,i3)*D0s(i1-1,i2  ,i3  ,c))+
     &         (a31(i1  ,i2  ,i3+1)*D0r(i1  ,i2  ,i3+1,c) - a31(i1,i2,i3-1)*D0r(i1  ,i2  ,i3-1,c) +
     &          a13(i1+1,i2  ,i3  )*D0t(i1+1,i2  ,i3  ,c) - a13(i1-1,i2,i3)*D0t(i1-1,i2  ,i3  ,c))+
     &         (a32(i1  ,i2  ,i3+1)*D0s(i1  ,i2  ,i3+1,c) - a32(i1,i2,i3-1)*D0s(i1  ,i2  ,i3-1,c) +
     &          a23(i1  ,i2+1,i3  )*D0t(i1  ,i2+1,i3  ,c) - a23(i1,i2-1,i3)*D0t(i1  ,i2-1,i3  ,c))
     &        )/jac(i1,i2,i3)
              end do
            end do
          end do
        end do
      else
      do c=ca,cb
        do i3=n3a,n3b
          do i2=n2a,n2b
            do i1=n1a,n1b
              deriv(i1,i2,i3,c)=
     &         (
     &         (a11(i1+1,i2  ,i3  )*urr(i1+1,i2  ,i3  ,c) - a11(i1,i2,i3)*urr(i1,i2,i3,c))+
     &         (a22(i1  ,i2+1,i3  )*uss(i1  ,i2+1,i3  ,c) - a22(i1,i2,i3)*uss(i1,i2,i3,c))+
     &         (a33(i1  ,i2  ,i3+1)*utt(i1  ,i2  ,i3+1,c) - a33(i1,i2,i3)*utt(i1,i2,i3,c))+
     &         (a21(i1  ,i2+1,i3  )*usr(i1  ,i2+1,i3  ,c) - a21(i1,i2,i3)*usr(i1,i2,i3,c) +
     &          a12(i1+1,i2  ,i3  )*urs(i1+1,i2  ,i3  ,c) - a12(i1,i2,i3)*urs(i1,i2,i3,c))+
     &         (a31(i1  ,i2  ,i3+1)*utr(i1  ,i2  ,i3+1,c) - a31(i1,i2,i3)*utr(i1,i2,i3,c) +
     &          a13(i1+1,i2  ,i3  )*urt(i1+1,i2  ,i3  ,c) - a13(i1,i2,i3)*urt(i1,i2,i3,c))+
     &         (a32(i1  ,i2  ,i3+1)*uts(i1  ,i2  ,i3+1,c) - a32(i1,i2,i3)*uts(i1,i2,i3,c) +
     &          a23(i1  ,i2+1,i3  )*ust(i1  ,i2+1,i3  ,c) - a23(i1,i2,i3)*ust(i1,i2,i3,c))
     &        )/jac(i1,i2,i3)
            end do
          end do
        end do
      end do
      end if

      return 
      end
#endFile

#beginFile dsgr.f


      subroutine divScalarGradFDeriv21R( nd, 
     &    nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,         
     &    ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b, ! dimensions for u
     &    ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b, ! dimensions for deriv
     &    n1a,n1b,n2a,n2b,n3a,n3b, ca,cb, 
     &    dx,
     &    u,s, deriv, 
     &    a11,  ! work space
     &    derivOption, gridType, order, averagingType, dir1, dir2 )
c ===============================================================
c 2nd order, 1D, rectangular
c Conservative discretization of
c           div( s grad )
c           div( tensor grad )
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
     &  n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,
     &  derivOption, gridType, order, averagingType, dir1, dir2

      integer arithmeticAverage,harmonicAverage
      parameter( arithmeticAverage=0,harmonicAverage=1 ) 
      integer laplace,divScalarGrad,derivativeScalarDerivative,divTensorGrad
      parameter(laplace=0,divScalarGrad=1,derivativeScalarDerivative=2,divTensorGrad=3)
      real s(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      real u(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,ndu4a:ndu4b)
      real deriv(ndd1a:ndd1b,ndd2a:ndd2b,ndd3a:ndd3b,ndd4a:ndd4b)

      real a11(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real dx(1),h22(1),h21(1)

      real factor
      real urr

      integer i1,i2,i3,kd3,c,j1,j2,j3,n
      integer  m1a,m1b,m2a,m2b,m3a,m3b

c.......statement functions 
      ! Estimate D{-r}(i-1/2,j,k)
      urr(i1,i2,i3,c)=u(i1,i2,i3,c)-u(i1-1,i2,i3,c) 

c.......end statement functions

      kd3=nd
      do n=1,nd
        h22(n)=1./dx(n)**2
        h21(n)=.5/dx(n)
      end do

      defineA21R()

c      Evaluate the derivative
      loopsDSG(deriv(i1,i2,i3,c)=(a11(i1+1,i2,i3)*urr(i1+1,i2,i3,c)-a11(i1,i2,i3)*urr(i1,i2,i3,c)))

      return 
      end


      subroutine divScalarGradFDeriv22R( nd, 
     &    nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,         
     &    ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b, ! dimensions for u
     &    ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b, ! dimensions for deriv
     &    n1a,n1b,n2a,n2b,n3a,n3b, ca,cb, 
     &    dx, ! *****************
     &    u,s, deriv, 
     &    a11,a22,a12,a21,  ! work space
     &    derivOption, gridType, order, averagingType, dir1, dir2 )
c ===============================================================
c 2nd order, 3D, rectangular
c Conservative discretization of
c           div( s grad )
c           div( tensor grad )
c           derivativeScalarDerivative
c  
c ca,cb : assign components c=ca,..,cb (base 0)
c derivOption : 0=laplace, 1=divScalarGrad, 2=derivativeScalarDerivative
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
     &  n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,
     &  derivOption, gridType, order, averagingType, dir1, dir2

      integer arithmeticAverage,harmonicAverage
      parameter( arithmeticAverage=0,harmonicAverage=1 ) 
      integer laplace,divScalarGrad,derivativeScalarDerivative,divTensorGrad
      parameter(laplace=0,divScalarGrad=1,derivativeScalarDerivative=2,divTensorGrad=3)
      real s(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      real u(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,ndu4a:ndu4b)
      real deriv(ndd1a:ndd1b,ndd2a:ndd2b,ndd3a:ndd3b,ndd4a:ndd4b)

      real a11(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real a12(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real a21(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real a22(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)

      real dx(2),h22(2),h21(2)

      real factor
      real urr,uss,urs,usr,hh

      integer i1,i2,i3,kd3,c,j1,j2,j3,n
      integer  m1a,m1b,m2a,m2b,m3a,m3b

c.......statement functions 
      ! Estimate D{-r}(i-1/2,j,k)
      urr(i1,i2,i3,c)=u(i1,i2,i3,c)-u(i1-1,i2,i3,c) 
      ! Estimate D{-s}(i-1/2,j+1/2,k)
      urs(i1,i2,i3,c)=(u(i1-1,i2+1,i3,c)+u(i1,i2+1,i3,c)-
     &     u(i1-1,i2-1,i3,c)-u(i1,i2-1,i3,c))
      ! Estimate D{-r}(i+1/2,j-1/2,k)
      usr(i1,i2,i3,c) = (u(i1+1,i2-1,i3,c) + u(i1+1,i2,i3,c) - 
     &                   u(i1-1,i2-1,i3,c) - u(i1-1,i2,i3,c))
      ! Estimate D{-s}(i,j-1/2,k)
      uss(i1,i2,i3,c) = u(i1,i2,i3,c)-u(i1,i2-1,i3,c)
      ! Estimate D{-t}(i,j-1/2,k+1/2)
c      ust(i1,i2,i3,c) = (u(i1,i2-1,i3+1,c) + u(i1,i2,i3+1,c) - 
c     &                   u(i1,i2-1,i3-1,c) - u(i1,i2,i3-1,c))

      D0r(i1,i2,i3,c)=u(i1+1,i2,i3,c)-u(i1-1,i2,i3,c)
      D0s(i1,i2,i3,c)=u(i1,i2+1,i3,c)-u(i1,i2-1,i3,c)
      D0t(i1,i2,i3,c)=u(i1,i2,i3+1,c)-u(i1,i2,i3-1,c)

c.......end statement functions

      kd3=nd
      do n=1,nd
        h22(n)=1./dx(n)**2
        h21(n)=.5/dx(n)
      end do


      defineA22R()
    
c     Evaluate the derivative
      if( derivOption.eq.divScalarGrad )then
        loopsDSG(deriv(i1,i2,i3,c)=(  \
              (a11(i1+1,i2  ,i3  )*urr(i1+1,i2  ,i3  ,c) - a11(i1,i2,i3)*urr(i1,i2,i3,c))+  \
              (a22(i1  ,i2+1,i3  )*uss(i1  ,i2+1,i3  ,c) - a22(i1,i2,i3)*uss(i1,i2,i3,c))  \
             ))
      else if( derivOption.eq.divTensorGrad )then

c       ** here is the new symmetric formula ***
        do c=ca,cb
          do i3=n3a,n3b
            do i2=n2a,n2b
              do i1=n1a,n1b
                deriv(i1,i2,i3,c)=
     &              (
     &              (a11(i1+1,i2  ,i3  )*urr(i1+1,i2  ,i3  ,c) -
     &               a11(i1  ,i2  ,i3  )*urr(i1  ,i2  ,i3  ,c))+
     &              (a22(i1  ,i2+1,i3  )*uss(i1  ,i2+1,i3  ,c) - 
     &               a22(i1  ,i2  ,i3  )*uss(i1  ,i2  ,i3  ,c))
     &             +(a21(i1  ,i2+1,i3  )*D0r(i1  ,i2+1,i3  ,c) - 
     &               a21(i1  ,i2-1,i3  )*D0r(i1  ,i2-1,i3  ,c) +
     &               a12(i1+1,i2  ,i3  )*D0s(i1+1,i2  ,i3  ,c) - 
     &               a12(i1-1,i2  ,i3  )*D0s(i1-1,i2  ,i3  ,c))
     &               )
              end do
            end do
          end do
        end do

      else 
c       derivative scalar derivative
        if( dir1.eq.0 .and. dir2.eq.0 )then
          loopsDSG(deriv(i1,i2,i3,c)=(a11(i1+1,i2,i3)*urr(i1+1,i2,i3,c)-a11(i1,i2,i3)*urr(i1,i2,i3,c)))
c          c=ca
c          i1=3
c          i2=3
c          i3=0
c          write(*,*) ' dsg: ca,cb,i1,i2,a11,urr=',ca,cb,i1,i2,a11(i1,i2,i3),urr(i1,i2,i3,c)
c          i1=i1+1
c          write(*,*) ' dsg: i1,i2,a11,urr=',i1,i2,a11(i1,i2,i3),urr(i1,i2,i3,c)

        else if( dir1.eq.0 .and. dir2.eq.1 )then
          loopsDSG(deriv(i1,i2,i3,c)=(a11(i1+1,i2,i3)*urs(i1+1,i2,i3,c)-a11(i1,i2,i3)*urs(i1,i2,i3,c)))

        else if( dir1.eq.1 .and. dir2.eq.0 )then
          loopsDSG(deriv(i1,i2,i3,c)=(a11(i1,i2+1,i3)*usr(i1,i2+1,i3,c)-a11(i1,i2,i3)*usr(i1,i2,i3,c)))
        else if( dir1.eq.1 .and. dir2.eq.1 )then
          loopsDSG(deriv(i1,i2,i3,c)=(a11(i1,i2+1,i3)*uss(i1,i2+1,i3,c)-a11(i1,i2,i3)*uss(i1,i2,i3,c)))

        end if
      end if

      return 
      end


      subroutine divScalarGradFDeriv23R( nd, 
     &    nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,         
     &    ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b, ! dimensions for u
     &    ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b, ! dimensions for deriv
     &    n1a,n1b,n2a,n2b,n3a,n3b, ca,cb, 
     &    dx,   ! ********* this was changed ************
     &    u,s, deriv, 
     &    a11,a22,a33, a12,a13,a21,a23,a31,a32,  ! work space
     &    derivOption, gridType, order, averagingType, dir1, dir2 )
c ===============================================================
c 2nd order, 3D, rectangular
c Conservative discretization of
c           div( s grad )
c           div( tensor Grad)
c           derivativeScalarDerivative - D_x1( s D_x2(u) )
c  
c ca,cb : assign components c=ca,..,cb (base 0)
c derivOption : 0=laplace, 1=divScalarGrad, 2=derivativeScalarDerivative
c gridType: 0=rectangular, 1=non-rectangular
c order : 2 or 4
c rsxy : not used if rectangular
c dx : grid spacing 
c a11,a22,a33 : only a11 is needed for derivOption==derivativeScalarDerivative
c ===============================================================

c      implicit none
      integer nd, 
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b,
     &  n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,
     &  derivOption, gridType, order, averagingType, dir1, dir2

      integer arithmeticAverage,harmonicAverage
      parameter( arithmeticAverage=0,harmonicAverage=1 ) 
      integer laplace,divScalarGrad,derivativeScalarDerivative,divTensorGrad
      parameter(laplace=0,divScalarGrad=1,derivativeScalarDerivative=2,divTensorGrad=3)
      real s(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      real u(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,ndu4a:ndu4b)
      real deriv(ndd1a:ndd1b,ndd2a:ndd2b,ndd3a:ndd3b,ndd4a:ndd4b)

      real a11(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real a21(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real a31(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real a12(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real a22(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real a32(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real a13(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real a23(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real a33(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)

      real dx(3),h22(3),h21(3)

      real factor,hh
      real urr,uss,utt,urs,usr,urt,utr,ust,uts

      integer i1,i2,i3,kd3,c,j1,j2,j3,n
      integer  m1a,m1b,m2a,m2b,m3a,m3b

c.......statement functions 
      ! Estimate D{-r}(i-1/2,j,k)
      urr(i1,i2,i3,c)=u(i1,i2,i3,c)-u(i1-1,i2,i3,c) 
      ! Estimate D{-s}(i-1/2,j+1/2,k)
      urs(i1,i2,i3,c)=(u(i1-1,i2+1,i3,c)+u(i1,i2+1,i3,c)-
     &     u(i1-1,i2-1,i3,c)-u(i1,i2-1,i3,c))
      ! Estimate D{-t}(i-1/2,j,k+1/2)
      urt(i1,i2,i3,c) = (u(i1-1,i2,i3+1,c) + u(i1,i2,i3+1,c) - 
     &                   u(i1-1,i2,i3-1,c) - u(i1,i2,i3-1,c))   
      ! Estimate D{-r}(i+1/2,j,k-1/2)
      utr(i1,i2,i3,c) = (u(i1+1,i2,i3-1,c) + u(i1+1,i2,i3,c) - 
     &                   u(i1-1,i2,i3-1,c) - u(i1-1,i2,i3,c))   
      ! Estimate D{-s}(i,j+1/2,k-1/2)
      uts(i1,i2,i3,c) = (u(i1,i2+1,i3-1,c) + u(i1,i2+1,i3,c) - 
     &                   u(i1,i2-1,i3-1,c) - u(i1,i2-1,i3,c))
      ! Estimate D{-t}(i,j,k-1/2)
      utt(i1,i2,i3,c) =u(i1,i2,i3,c)-u(i1,i2,i3-1,c) 
      ! Estimate D{-r}(i+1/2,j-1/2,k)
      usr(i1,i2,i3,c) = (u(i1+1,i2-1,i3,c) + u(i1+1,i2,i3,c) - 
     &                   u(i1-1,i2-1,i3,c) - u(i1-1,i2,i3,c))
      ! Estimate D{-s}(i,j-1/2,k)
      uss(i1,i2,i3,c) = u(i1,i2,i3,c)-u(i1,i2-1,i3,c)
      ! Estimate D{-t}(i,j-1/2,k+1/2)
      ust(i1,i2,i3,c) = (u(i1,i2-1,i3+1,c) + u(i1,i2,i3+1,c) - 
     &                   u(i1,i2-1,i3-1,c) - u(i1,i2,i3-1,c))

      D0r(i1,i2,i3,c)=u(i1+1,i2,i3,c)-u(i1-1,i2,i3,c)
      D0s(i1,i2,i3,c)=u(i1,i2+1,i3,c)-u(i1,i2-1,i3,c)
      D0t(i1,i2,i3,c)=u(i1,i2,i3+1,c)-u(i1,i2,i3-1,c)

c.......end statement functions


      if( derivOption.eq.laplace )then
         write(*,*) 'ERROR: divScalarGradFDeriv23R should not be' // 
     &    ' called for laplace'
         return
      end if

      kd3=nd
      do n=1,nd
        h22(n)=1./dx(n)**2
        h21(n)=.5/dx(n)
      end do


      defineA23R()

c      Evaluate the derivative
      if( derivOption.eq.divScalarGrad )then
        loopsDSG(deriv(i1,i2,i3,c)=(  \
              (a11(i1+1,i2  ,i3  )*urr(i1+1,i2  ,i3  ,c) -  \
               a11(i1,i2,i3)*urr(i1,i2,i3,c))+  \
              (a22(i1  ,i2+1,i3  )*uss(i1  ,i2+1,i3  ,c) -   \
               a22(i1,i2,i3)*uss(i1,i2,i3,c))+  \
              (a33(i1  ,i2  ,i3+1)*utt(i1  ,i2  ,i3+1,c) -   \
               a33(i1,i2,i3)*utt(i1,i2,i3,c)) \
             ))
      else if( derivOption.eq.divTensorGrad )then
c       ** here is the new symmetric formula ***
        do c=ca,cb
          do i3=n3a,n3b
            do i2=n2a,n2b
              do i1=n1a,n1b
              deriv(i1,i2,i3,c)=
     &         (
     &         (a11(i1+1,i2  ,i3  )*urr(i1+1,i2  ,i3  ,c) - a11(i1,i2,i3)*urr(i1  ,i2  ,i3  ,c))+
     &         (a22(i1  ,i2+1,i3  )*uss(i1  ,i2+1,i3  ,c) - a22(i1,i2,i3)*uss(i1  ,i2  ,i3  ,c))+
     &         (a33(i1  ,i2  ,i3+1)*utt(i1  ,i2  ,i3+1,c) - a33(i1,i2,i3)*utt(i1  ,i2  ,i3  ,c))+
     &         (a21(i1  ,i2+1,i3  )*D0r(i1  ,i2+1,i3  ,c) - a21(i1,i2-1,i3)*D0r(i1  ,i2-1,i3  ,c) +
     &          a12(i1+1,i2  ,i3  )*D0s(i1+1,i2  ,i3  ,c) - a12(i1-1,i2,i3)*D0s(i1-1,i2  ,i3  ,c))+
     &         (a31(i1  ,i2  ,i3+1)*D0r(i1  ,i2  ,i3+1,c) - a31(i1,i2,i3-1)*D0r(i1  ,i2  ,i3-1,c) +
     &          a13(i1+1,i2  ,i3  )*D0t(i1+1,i2  ,i3  ,c) - a13(i1-1,i2,i3)*D0t(i1-1,i2  ,i3  ,c))+
     &         (a32(i1  ,i2  ,i3+1)*D0s(i1  ,i2  ,i3+1,c) - a32(i1,i2,i3-1)*D0s(i1  ,i2  ,i3-1,c) +
     &          a23(i1  ,i2+1,i3  )*D0t(i1  ,i2+1,i3  ,c) - a23(i1,i2-1,i3)*D0t(i1  ,i2-1,i3  ,c))
     &        )
              end do
            end do
          end do
        end do
      else 
c       derivative scalar derivative
        if( dir1.eq.0 .and. dir2.eq.0 )then
          loopsDSG(deriv(i1,i2,i3,c)=(a11(i1+1,i2,i3)*urr(i1+1,i2,i3,c)-a11(i1,i2,i3)*urr(i1,i2,i3,c)))
        else if( dir1.eq.0 .and. dir2.eq.1 )then
          loopsDSG(deriv(i1,i2,i3,c)=(a11(i1+1,i2,i3)*urs(i1+1,i2,i3,c)-a11(i1,i2,i3)*urs(i1,i2,i3,c)))
        else if( dir1.eq.0 .and. dir2.eq.2 )then
          loopsDSG(deriv(i1,i2,i3,c)=(a11(i1+1,i2,i3)*urt(i1+1,i2,i3,c)-a11(i1,i2,i3)*urt(i1,i2,i3,c)))

        else if( dir1.eq.1 .and. dir2.eq.0 )then
          loopsDSG(deriv(i1,i2,i3,c)=(a11(i1,i2+1,i3)*usr(i1,i2+1,i3,c)-a11(i1,i2,i3)*usr(i1,i2,i3,c)))
        else if( dir1.eq.1 .and. dir2.eq.1 )then
          loopsDSG(deriv(i1,i2,i3,c)=(a11(i1,i2+1,i3)*uss(i1,i2+1,i3,c)-a11(i1,i2,i3)*uss(i1,i2,i3,c)))
        else if( dir1.eq.1 .and. dir2.eq.2 )then
          loopsDSG(deriv(i1,i2,i3,c)=(a11(i1,i2+1,i3)*ust(i1,i2+1,i3,c)-a11(i1,i2,i3)*ust(i1,i2,i3,c)))

        else if( dir1.eq.2 .and. dir2.eq.0 )then
          loopsDSG(deriv(i1,i2,i3,c)=(a11(i1,i2,i3+1)*utr(i1,i2,i3+1,c)-a11(i1,i2,i3)*utr(i1,i2,i3,c)))
        else if( dir1.eq.2 .and. dir2.eq.1 )then
          loopsDSG(deriv(i1,i2,i3,c)=(a11(i1,i2,i3+1)*uts(i1,i2,i3+1,c)-a11(i1,i2,i3)*uts(i1,i2,i3,c)))
        else if( dir1.eq.2 .and. dir2.eq.2 )then
          loopsDSG(deriv(i1,i2,i3,c)=(a11(i1,i2,i3+1)*utt(i1,i2,i3+1,c)-a11(i1,i2,i3)*utt(i1,i2,i3,c)))

        end if
      end if

      return 
      end

#endFile

c This next macro defines the operator D_X( s D_Y u )
#beginMacro DXDY(XY,X1,Y1)
if( nd .eq. 2 )then
c ******* 2D *************      
  if( gridType .eq. 0 )then
c   rectangular
    if( order.eq.2 )then
      loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*U XY 22R(i1,i2,i3,c)+S X1 22R(i1,i2,i3)*U Y1 22R(i1,i2,i3,c))
    else
      loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*U XY 42R(i1,i2,i3,c)+S X1 42R(i1,i2,i3)*U Y1 42R(i1,i2,i3,c))
    end if
  else
c   ***** not rectangular *****
    if( order.eq.2 )then
      loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*U XY 22(i1,i2,i3,c)+S X1 22(i1,i2,i3)*U Y1 22(i1,i2,i3,c))
    else
      loopsDSG(deriv(i1,i2,i3,c)= s(i1,i2,i3,0)*U XY 42(i1,i2,i3,c)+S X1 42(i1,i2,i3)*U Y1 42(i1,i2,i3,c))
    end if
  endif 
elseif( nd.eq.3 )then
c ******* 3D *************      
  if( gridType .eq. 0 )then
c   rectangular
    if( order.eq.2 )then
      loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*U XY 23R(i1,i2,i3,c)+S X1 22R(i1,i2,i3)*U Y1 23R(i1,i2,i3,c))
    else
      loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*U XY 43R(i1,i2,i3,c)+S X1 42R(i1,i2,i3)*U Y1 42R(i1,i2,i3,c))
    end if
  else
c   ***** not rectangular *****
    if( order.eq.2 )then
      loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*U XY 23(i1,i2,i3,c)+S X1 23(i1,i2,i3)*U Y1 23(i1,i2,i3,c))
    else
      loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*U XY 43(i1,i2,i3,c)+S X1 43(i1,i2,i3)*U Y1 43(i1,i2,i3,c))
    end if
  endif 
  
 else
c   ******* 1D *************      
  if( gridType .eq. 0 )then
c   rectangular
    if( order.eq.2 )then
      loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*U XY 21R(i1,i2,i3,c)+S X1 22R(i1,i2,i3)*U Y1 22R(i1,i2,i3,c))
    else
      loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*U XY 41R(i1,i2,i3,c)+S X1 42R(i1,i2,i3)*U Y1 42R(i1,i2,i3,c))
    end if
  
  else
c    ***** not rectangular *****
    if( order.eq.2 )then
      loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*U XY 21(i1,i2,i3,c)+S X1 21(i1,i2,i3)*U Y1 21(i1,i2,i3,c))
    else
      loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*U XY 41(i1,i2,i3,c)+S X1 41(i1,i2,i3)*U Y1 41(i1,i2,i3,c))
    end if
  
  endif 
end if
#endMacro


#beginMacro nonConservative(operator)
subroutine operator( nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b, \
  ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b, n1a,n1b,n2a,n2b,n3a,n3b, ca,cb, \
  h21,d22,d12,h22,d14,d24,h41,h42,rsxy,u,s,deriv,derivOption,gridType,order,averagingType,dir1,dir2 )
c ===============================================================
c    divScalarGrad -- non-conservative form
c  
c ca,cb : assign components c=ca,..,cb (base 0)
c derivOption : 0=laplace, 1=divScalarGrad, 2=derivativeScalarDerivative
c gridType: 0=rectangular, 1=non-rectangular
c order : 2 or 4
c rsxy : not used if rectangular
c h22 : 1/h**2 : for rectangular  
c ===============================================================

c      implicit none
integer nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,\
  ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b,n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,\
  derivOption, gridType, order, averagingType, dir1, dir2

integer laplace,divScalarGrad,derivativeScalarDerivative,divTensorGrad
parameter(laplace=0,divScalarGrad=1,derivativeScalarDerivative=2,divTensorGrad=3)

real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,1:nd,1:nd)
real u(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,ndu4a:ndu4b)
real s(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,0:*)
real deriv(ndd1a:ndd1b,ndd2a:ndd2b,ndd3a:ndd3b,ndd4a:ndd4b)
real h21(*), d22(*),d12(*),h22(*)
real d24(*),d14(*),h42(*),h41(*)

real LAPLACIAN21R, LAPLACIAN21, LAPLACIAN41R, LAPLACIAN41
real LAPLACIAN22R, LAPLACIAN22, LAPLACIAN42R, LAPLACIAN42
real LAPLACIAN23R, LAPLACIAN23, LAPLACIAN43R, LAPLACIAN43

real sr2,ss2,st2,sx22,sx23,sy23,sz23,sx22r,sy22r,sz22r
real sr,ss,st,sx42,sy42,sx43,sy43,sz43,sx42r,sy42r,sz42r
integer i1,i2,i3,kd3,kd,c,kdd

c.......statement functions 
c.......statement functions for jacobian
rx(i1,i2,i3)=rsxy(i1,i2,i3,  1,  1)
ry(i1,i2,i3)=rsxy(i1,i2,i3,  1,  2)
rz(i1,i2,i3)=rsxy(i1,i2,i3,  1,kd3)
sx(i1,i2,i3)=rsxy(i1,i2,i3,  2,  1)
sy(i1,i2,i3)=rsxy(i1,i2,i3,  2,  2)
sz(i1,i2,i3)=rsxy(i1,i2,i3,  2,kd3)
tx(i1,i2,i3)=rsxy(i1,i2,i3,kd3,  1)
ty(i1,i2,i3)=rsxy(i1,i2,i3,kd3,  2)
tz(i1,i2,i3)=rsxy(i1,i2,i3,kd3,kd3)

include 'cgux2af.h'
include 'cgux4af.h'


sr2(i1,i2,i3)=(s(i1+1,i2,i3,0)-s(i1-1,i2,i3,0))*d12(1)
ss2(i1,i2,i3)=(s(i1,i2+1,i3,0)-s(i1,i2-1,i3,0))*d12(2)
st2(i1,i2,i3)=(s(i1,i2,i3+1,0)-s(i1,i2,i3-1,0))*d12(3)

sx21(i1,i2,i3)= rx(i1,i2,i3)*sr2(i1,i2,i3)

sx22(i1,i2,i3)= rx(i1,i2,i3)*sr2(i1,i2,i3)+sx(i1,i2,i3)*ss2(i1,i2,i3)
sy22(i1,i2,i3)= ry(i1,i2,i3)*sr2(i1,i2,i3)+sy(i1,i2,i3)*ss2(i1,i2,i3)
sx23(i1,i2,i3)=rx(i1,i2,i3)*sr2(i1,i2,i3)+sx(i1,i2,i3)*ss2(i1,i2,i3)+tx(i1,i2,i3)*st2(i1,i2,i3)
sy23(i1,i2,i3)=ry(i1,i2,i3)*sr2(i1,i2,i3)+sy(i1,i2,i3)*ss2(i1,i2,i3)+ty(i1,i2,i3)*st2(i1,i2,i3)
sz23(i1,i2,i3)=rz(i1,i2,i3)*sr2(i1,i2,i3)+sz(i1,i2,i3)*ss2(i1,i2,i3)+tz(i1,i2,i3)*st2(i1,i2,i3)

sx22r(i1,i2,i3)=(s(i1+1,i2,i3,0)-s(i1-1,i2,i3,0))*h21(1)
sy22r(i1,i2,i3)=(s(i1,i2+1,i3,0)-s(i1,i2-1,i3,0))*h21(2)
sz22r(i1,i2,i3)=(s(i1,i2,i3+1,0)-s(i1,i2,i3-1,0))*h21(3)

sr(i1,i2,i3)=(8.*(s(i1+1,i2,i3,0)-s(i1-1,i2,i3,0))-(s(i1+2,i2,i3,0)-s(i1-2,i2,i3,0)))*d14(1)
ss(i1,i2,i3)=(8.*(s(i1,i2+1,i3,0)-s(i1,i2-1,i3,0))-(s(i1,i2+2,i3,0)-s(i1,i2-2,i3,0)))*d14(2)
st(i1,i2,i3)=(8.*(s(i1,i2,i3+1,0)-s(i1,i2,i3-1,0))-(s(i1,i2,i3+2,0)-s(i1,i2,i3-2,0)))*d14(3)

sx41(i1,i2,i3)= rx(i1,i2,i3)*sr(i1,i2,i3)

sx42(i1,i2,i3)= rx(i1,i2,i3)*sr(i1,i2,i3)+sx(i1,i2,i3)*ss(i1,i2,i3)
sy42(i1,i2,i3)= ry(i1,i2,i3)*sr(i1,i2,i3) +sy(i1,i2,i3)*ss(i1,i2,i3)
sx43(i1,i2,i3)=rx(i1,i2,i3)*sr(i1,i2,i3)+sx(i1,i2,i3)*ss(i1,i2,i3)+tx(i1,i2,i3)*st(i1,i2,i3)
 sy43(i1,i2,i3)=ry(i1,i2,i3)*sr(i1,i2,i3)+sy(i1,i2,i3)*ss(i1,i2,i3)+ty(i1,i2,i3)*st(i1,i2,i3)
sz43(i1,i2,i3)=rz(i1,i2,i3)*sr(i1,i2,i3)+sz(i1,i2,i3)*ss(i1,i2,i3)+tz(i1,i2,i3)*st(i1,i2,i3)

sx42r(i1,i2,i3)=(8.*(s(i1+1,i2,i3,0)-s(i1-1,i2,i3,0))-(s(i1+2,i2,i3,0)-s(i1-2,i2,i3,0)))*h41(1)
sy42r(i1,i2,i3)=(8.*(s(i1,i2+1,i3,0)-s(i1,i2-1,i3,0))-(s(i1,i2+2,i3,0)-s(i1,i2-2,i3,0)))*h41(2) 
sz42r(i1,i2,i3)=(8.*(s(i1,i2,i3+1,0)-s(i1,i2,i3-1,0))-(s(i1,i2,i3+2,0)-s(i1,i2,i3-2,0)))*h41(3) 

sy21(i1,i2,i3)=0.
sz21(i1,i2,i3)=0.
sz42(i1,i2,i3)=0.
sz22(i1,i2,i3)=0.
sz41(i1,i2,i3)=0.
sy41(i1,i2,i3)=0.

c......end statement function



kd3=nd

#If #operator == "divScalarGradNC"
c       ****** divScalarGrad ******

if( nd .eq. 2 )then
c         ******* 2D *************      
  if( gridType .eq. 0 )then
c           rectangular
    if( order.eq.2 )then
      loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*LAPLACIAN22R(i1,i2,i3,c)\
                          +SX22R(i1,i2,i3)*UX22R(i1,i2,i3,c)+SY22R(i1,i2,i3)*UY22R(i1,i2,i3,c))
    else
      loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*LAPLACIAN42R(i1,i2,i3,c) \
                   +SX42R(i1,i2,i3)*UX42R(i1,i2,i3,c)+SY42R(i1,i2,i3)*UY42R(i1,i2,i3,c))
    end if
  else
c           ***** not rectangular *****
    if( order.eq.2 )then
      loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*LAPLACIAN22(i1,i2,i3,c)\
                      +SX22(i1,i2,i3)*UX22(i1,i2,i3,c)+SY22(i1,i2,i3)*UY22(i1,i2,i3,c))
    else
      loopsDSG(deriv(i1,i2,i3,c)= s(i1,i2,i3,0)*LAPLACIAN42(i1,i2,i3,c)\
                      +SX42(i1,i2,i3)*UX42(i1,i2,i3,c)+SY42(i1,i2,i3)*UY42(i1,i2,i3,c))
    end if
  endif 
elseif( nd.eq.3 )then
c         ******* 3D *************      
  if( gridType .eq. 0 )then
c           rectangular
    if( order.eq.2 )then
      loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*LAPLACIAN23R(i1,i2,i3,c)\
                   +SX22R(i1,i2,i3)*UX23R(i1,i2,i3,c)\
                   +SY22R(i1,i2,i3)*UY23R(i1,i2,i3,c)\
                   +SZ22R(i1,i2,i3)*UZ23R(i1,i2,i3,c))
    else
      loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*LAPLACIAN43R(i1,i2,i3,c)\
                   +SX42R(i1,i2,i3)*UX42R(i1,i2,i3,c)\
                   +SY42R(i1,i2,i3)*UY42R(i1,i2,i3,c)\
                   +SZ42R(i1,i2,i3)*UZ42R(i1,i2,i3,c))
    end if
  else
c           ***** not rectangular *****
    if( order.eq.2 )then
      loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*LAPLACIAN23(i1,i2,i3,c)\
                   +SX23(i1,i2,i3)*UX23(i1,i2,i3,c)\
                   +SY23(i1,i2,i3)*UY23(i1,i2,i3,c)\
                   +SZ23(i1,i2,i3)*UZ23(i1,i2,i3,c))
    else
      loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*LAPLACIAN43(i1,i2,i3,c)\
                   +SX43(i1,i2,i3)*UX43(i1,i2,i3,c)\
                   +SY43(i1,i2,i3)*UY43(i1,i2,i3,c)\
                   +SZ43(i1,i2,i3)*UZ43(i1,i2,i3,c))
    end if
  endif 

else
c         ******* 1D *************      
  if( gridType .eq. 0 )then
c           rectangular
    if( order.eq.2 )then
      loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*LAPLACIAN21R(i1,i2,i3,c)+SX22R(i1,i2,i3)*UX22R(i1,i2,i3,c))
    else
      loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*LAPLACIAN41R(i1,i2,i3,c)+SX42R(i1,i2,i3)*UX42R(i1,i2,i3,c))
    end if

  else
c            ***** not rectangular *****
    if( order.eq.2 )then
      loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*LAPLACIAN21(i1,i2,i3,c)+SX21(i1,i2,i3)*UX21(i1,i2,i3,c))
    else
      loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*LAPLACIAN41(i1,i2,i3,c)+SX41(i1,i2,i3)*UX41(i1,i2,i3,c))
    end if

  endif 
end if

#Elif #operator == "laplaceNC"
c       ****** laplace ******

if( nd .eq. 2 )then
c         ******* 2D *************      
  if( gridType .eq. 0 )then
c           rectangular
    if( order.eq.2 )then
      loopsDSG(deriv(i1,i2,i3,c)=LAPLACIAN22R(i1,i2,i3,c))
    else
      loopsDSG(deriv(i1,i2,i3,c)=LAPLACIAN42R(i1,i2,i3,c))
    end if
  else
c           ***** not rectangular *****
    if( order.eq.2 )then
      loopsDSG(deriv(i1,i2,i3,c)=LAPLACIAN22(i1,i2,i3,c))
    else
      loopsDSG(deriv(i1,i2,i3,c)=LAPLACIAN42(i1,i2,i3,c))
    end if
  endif 
elseif( nd.eq.3 )then
c         ******* 3D *************      
  if( gridType .eq. 0 )then
c           rectangular
    if( order.eq.2 )then
      loopsDSG(deriv(i1,i2,i3,c)=LAPLACIAN23R(i1,i2,i3,c))
    else
      loopsDSG(deriv(i1,i2,i3,c)=LAPLACIAN43R(i1,i2,i3,c))
    end if
  else
c           ***** not rectangular *****
    if( order.eq.2 )then
      loopsDSG(deriv(i1,i2,i3,c)=LAPLACIAN23(i1,i2,i3,c))
    else
      loopsDSG(deriv(i1,i2,i3,c)=LAPLACIAN43(i1,i2,i3,c))
    end if
  endif 

else
c         ******* 1D *************      
  if( gridType .eq. 0 )then
c           rectangular
    if( order.eq.2 )then
      loopsDSG(deriv(i1,i2,i3,c)=LAPLACIAN21R(i1,i2,i3,c))
    else
      loopsDSG(deriv(i1,i2,i3,c)=LAPLACIAN41R(i1,i2,i3,c))
    end if

  else
c            ***** not rectangular *****
    if( order.eq.2 )then
      loopsDSG(deriv(i1,i2,i3,c)=LAPLACIAN21(i1,i2,i3,c))
    else
      loopsDSG(deriv(i1,i2,i3,c)=LAPLACIAN41(i1,i2,i3,c))
    end if

  endif 
end if

#Elif #operator == "derivativeScalarDerivativeNC"
c       ****** derivativeScalarDerivative ******

if(      dir1.eq.0 .and. dir2.eq.0 )then
  DXDY(XX,X,X)
else if( dir1.eq.0 .and. dir2.eq.1 )then
  DXDY(XY,X,Y)
else if( dir1.eq.0 .and. dir2.eq.2 )then
  DXDY(XZ,X,Z)
else if( dir1.eq.1 .and. dir2.eq.0 )then
  DXDY(XY,Y,X)
else if( dir1.eq.1 .and. dir2.eq.1 )then
  DXDY(YY,Y,Y)
else if( dir1.eq.1 .and. dir2.eq.2 )then
  DXDY(YZ,Y,Z)
else if( dir1.eq.2 .and. dir2.eq.0 )then
  DXDY(XZ,Z,X)
else if( dir1.eq.2 .and. dir2.eq.1 )then
  DXDY(YZ,Z,Y)
else if( dir1.eq.2 .and. dir2.eq.2 )then
  DXDY(ZZ,Z,Z)
else
  write(*,*) 'ERROR invalid values: dir1=',dir1,' dir2=',dir2
end if
#End
if( nd.eq.0 )then
c       *** add these lines to avoid warnings about unused statement functions
  include "cgux2afNoWarnings.h" 
  include "cgux4afNoWarnings.h" 
end if

return
end

#endMacro



c =============== NEW VERSION WITH HIGHER ORDER OPERATORS =====================

c This next macro defines the operator D_X( s D_Y u )
#beginMacro DXDYNEW(XY,X1,Y1)
if( nd .eq. 2 )then
c ******* 2D *************      
  if( gridType .eq. 0 )then
c   rectangular
    if( order.eq.2 )then
      loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*U XY 22R(i1,i2,i3,c)+SC X1 22R(i1,i2,i3,0)*U Y1 22R(i1,i2,i3,c))
    else if( order.eq.4 )then
      loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*U XY 42R(i1,i2,i3,c)+SC X1 42R(i1,i2,i3,0)*U Y1 42R(i1,i2,i3,c))
    else if( order.eq.6 )then
      loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*U XY 62R(i1,i2,i3,c)+SC X1 62R(i1,i2,i3,0)*U Y1 62R(i1,i2,i3,c))
    else if( order.eq.8 )then
      loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*U XY 82R(i1,i2,i3,c)+SC X1 82R(i1,i2,i3,0)*U Y1 82R(i1,i2,i3,c))
    else
      write(*,*) 'ERROR:divScalarGradNC:order=',order
      stop 43
    end if
  else
c   ***** not rectangular *****
    if( order.eq.2 )then
      loopsDSG(deriv(i1,i2,i3,c)= sc(i1,i2,i3,0)*U XY 22(i1,i2,i3,c)+SC X1 22(i1,i2,i3,0)*U Y1 22(i1,i2,i3,c))
    else if( order.eq.4 )then
      loopsDSG(deriv(i1,i2,i3,c)= sc(i1,i2,i3,0)*U XY 42(i1,i2,i3,c)+SC X1 42(i1,i2,i3,0)*U Y1 42(i1,i2,i3,c))
    else if( order.eq.6 )then
      loopsDSG(deriv(i1,i2,i3,c)= sc(i1,i2,i3,0)*U XY 62(i1,i2,i3,c)+SC X1 62(i1,i2,i3,0)*U Y1 62(i1,i2,i3,c))
    else if( order.eq.8 )then
      loopsDSG(deriv(i1,i2,i3,c)= sc(i1,i2,i3,0)*U XY 82(i1,i2,i3,c)+SC X1 82(i1,i2,i3,0)*U Y1 82(i1,i2,i3,c))
    else
      write(*,*) 'ERROR:divScalarGradNC:order=',order
      stop 43
    end if
  endif 
elseif( nd.eq.3 )then
c ******* 3D *************      
  if( gridType .eq. 0 )then
c   rectangular
    if( order.eq.2 )then
      loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*U XY 23R(i1,i2,i3,c)+SC X1 22R(i1,i2,i3,0)*U Y1 23R(i1,i2,i3,c))
    else if( order.eq.4 )then
      loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*U XY 43R(i1,i2,i3,c)+SC X1 42R(i1,i2,i3,0)*U Y1 42R(i1,i2,i3,c))
    else if( order.eq.6 )then
      loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*U XY 63R(i1,i2,i3,c)+SC X1 62R(i1,i2,i3,0)*U Y1 62R(i1,i2,i3,c))
    else if( order.eq.8 )then
      loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*U XY 83R(i1,i2,i3,c)+SC X1 82R(i1,i2,i3,0)*U Y1 82R(i1,i2,i3,c))
    else
      write(*,*) 'ERROR:divScalarGradNC:order=',order
      stop 43
    end if
  else
c   ***** not rectangular *****
    if( order.eq.2 )then
      loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*U XY 23(i1,i2,i3,c)+SC X1 23(i1,i2,i3,0)*U Y1 23(i1,i2,i3,c))
    else if( order.eq.4 )then
      loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*U XY 43(i1,i2,i3,c)+SC X1 43(i1,i2,i3,0)*U Y1 43(i1,i2,i3,c))
    else if( order.eq.6 )then
      loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*U XY 63(i1,i2,i3,c)+SC X1 63(i1,i2,i3,0)*U Y1 63(i1,i2,i3,c))
    else if( order.eq.8 )then
      loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*U XY 83(i1,i2,i3,c)+SC X1 83(i1,i2,i3,0)*U Y1 83(i1,i2,i3,c))
    else
      write(*,*) 'ERROR:divScalarGradNC:order=',order
      stop 43
    end if
  endif 
  
 else
c   ******* 1D *************      
  if( gridType .eq. 0 )then
c   rectangular
    if( order.eq.2 )then
      loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*U XY 21R(i1,i2,i3,c)+SC X1 22R(i1,i2,i3,0)*U Y1 22R(i1,i2,i3,c))
    else if( order.eq.4 )then
      loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*U XY 41R(i1,i2,i3,c)+SC X1 42R(i1,i2,i3,0)*U Y1 42R(i1,i2,i3,c))
    else if( order.eq.6 )then
      loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*U XY 61R(i1,i2,i3,c)+SC X1 62R(i1,i2,i3,0)*U Y1 62R(i1,i2,i3,c))
    else if( order.eq.8 )then
      loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*U XY 81R(i1,i2,i3,c)+SC X1 82R(i1,i2,i3,0)*U Y1 82R(i1,i2,i3,c))
    else
      write(*,*) 'ERROR:divScalarGradNC:order=',order
      stop 43
    end if
  
  else
c    ***** not rectangular *****
    if( order.eq.2 )then
      loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*U XY 21(i1,i2,i3,c)+SC X1 21(i1,i2,i3,0)*U Y1 21(i1,i2,i3,c))
    else if( order.eq.4 )then
      loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*U XY 41(i1,i2,i3,c)+SC X1 41(i1,i2,i3,0)*U Y1 41(i1,i2,i3,c))
    else if( order.eq.6 )then
      loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*U XY 61(i1,i2,i3,c)+SC X1 61(i1,i2,i3,0)*U Y1 61(i1,i2,i3,c))
    else if( order.eq.8 )then
      loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*U XY 81(i1,i2,i3,c)+SC X1 81(i1,i2,i3,0)*U Y1 81(i1,i2,i3,c))
    else
      write(*,*) 'ERROR:divScalarGradNC:order=',order
      stop 43
    end if
  
  endif 
end if
#endMacro



c These next include files will define the macros that will define the difference approximations
c The actual macro is called below
#Include "../src/defineDiffOrder2f.h"
#Include "../src/defineDiffOrder4f.h"
#Include "../src/defineDiffOrder6f.h"
#Include "../src/defineDiffOrder8f.h"

! Macro to evaluate the divTensorGrad operator in 2D (non-conservative)
! DTYPE: 22R, 22, 42R, ...
!    deriv = SUM  Dm( s(m,n) Dn )
#beginMacro divTensorGrad2d(DTYPE)
do c=ca,cb
do i3=n3a,n3b
do i2=n2a,n2b
do i1=n1a,n1b

 uxc = UX ## DTYPE(i1,i2,i3,c)
 uyc = UY ## DTYPE(i1,i2,i3,c)
 uxxc= UXX ## DTYPE(i1,i2,i3,c)
 uxyc= UXY ## DTYPE(i1,i2,i3,c)
 uyyc= UYY ## DTYPE(i1,i2,i3,c)

 deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*uxxc +SCX ## DTYPE(i1,i2,i3,0)*uxc+ \
                   sc(i1,i2,i3,1)*uxyc +SCY ## DTYPE(i1,i2,i3,1)*uxc+ \
                   sc(i1,i2,i3,2)*uxyc +SCX ## DTYPE(i1,i2,i3,2)*uyc+ \
                   sc(i1,i2,i3,3)*uyyc +SCY ## DTYPE(i1,i2,i3,3)*uyc

end do
end do
end do
end do

#endMacro


! Macro to evaluate the divTensorGrad operator in 2D (non-conservative)
! DTYPE: 23R, 23, 43R, ...
!    deriv = SUM  Dm( s(m,n) Dn )
#beginMacro divTensorGrad3d(DTYPE)
do c=ca,cb
do i3=n3a,n3b
do i2=n2a,n2b
do i1=n1a,n1b

 uxc = UX ## DTYPE(i1,i2,i3,c)
 uyc = UY ## DTYPE(i1,i2,i3,c)
 uzc = UZ ## DTYPE(i1,i2,i3,c)

 uxxc= UXX ## DTYPE(i1,i2,i3,c)
 uxyc= UXY ## DTYPE(i1,i2,i3,c)
 uxzc= UXZ ## DTYPE(i1,i2,i3,c)

 uyxc=uxyc
 uyyc= UYY ## DTYPE(i1,i2,i3,c)
 uyzc= UYZ ## DTYPE(i1,i2,i3,c)

 uzxc=uxzc
 uzyc=uyzc
 uzzc= UZZ ## DTYPE(i1,i2,i3,c)

 deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*uxxc +SCX ## DTYPE(i1,i2,i3,0)*uxc+ \
                   sc(i1,i2,i3,1)*uyxc +SCY ## DTYPE(i1,i2,i3,1)*uxc+ \
                   sc(i1,i2,i3,2)*uzxc +SCZ ## DTYPE(i1,i2,i3,2)*uxc+ \
                   sc(i1,i2,i3,3)*uxyc +SCX ## DTYPE(i1,i2,i3,3)*uyc+ \
                   sc(i1,i2,i3,4)*uyyc +SCY ## DTYPE(i1,i2,i3,4)*uyc+ \
                   sc(i1,i2,i3,5)*uzyc +SCZ ## DTYPE(i1,i2,i3,5)*uyc+ \
                   sc(i1,i2,i3,6)*uxzc +SCX ## DTYPE(i1,i2,i3,6)*uzc+ \
                   sc(i1,i2,i3,7)*uyzc +SCY ## DTYPE(i1,i2,i3,7)*uzc+ \
                   sc(i1,i2,i3,8)*uzzc +SCZ ## DTYPE(i1,i2,i3,8)*uzc

end do
end do
end do
end do

#endMacro

! Macro to evaluate the divTensorGrad operator in 1D (non-conservative)
! DTYPE: 21R, 21, 41R, ...
!    deriv = SUM  Dm( s(m,n) Dn )
#beginMacro divTensorGrad1d(DTYPE)
do c=ca,cb
do i3=n3a,n3b
do i2=n2a,n2b
do i1=n1a,n1b

 uxc = UX ## DTYPE(i1,i2,i3,c)
 uxxc= UXX ## DTYPE(i1,i2,i3,c)

 deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*uxxc +SCX ## DTYPE(i1,i2,i3,0)*uxc

end do
end do
end do
end do

#endMacro



#beginMacro nonConservativeNew(operator)
subroutine operator ## New( nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b, \
  ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b, n1a,n1b,n2a,n2b,n3a,n3b, ca,cb, \
  dr,dx,rsxy,u,sc,deriv,derivOption,gridType,order,averagingType,dir1,dir2 )
c ===============================================================
c Non-conservative form of the operators:
c           Laplace
c           div( s grad )
c           div( tensor Grad )
c           derivativeScalarDerivative
c  
c ca,cb : assign components c=ca,..,cb (base 0)
c derivOption : 0=laplace, 1=divScalarGrad, 2=derivativeScalarDerivative
c gridType: 0=rectangular, 1=non-rectangular
c order : 2 or 4
c rsxy : not used if rectangular
c ===============================================================

c      implicit none
integer nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,\
  ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b,n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,\
  derivOption, gridType, order, averagingType, dir1, dir2

integer laplace,divScalarGrad,derivativeScalarDerivative,divTensorGrad
parameter(laplace=0,divScalarGrad=1,derivativeScalarDerivative=2,divTensorGrad=3)

real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,1:nd,1:nd)
real u(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,ndu4a:ndu4b)
real sc(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,0:*)
real deriv(ndd1a:ndd1b,ndd2a:ndd2b,ndd3a:ndd3b,ndd4a:ndd4b)
real dr(0:*),dx(0:*)

integer i1,i2,i3,kd3,kd,c,kdd

c.......statement functions 
real rx,ry,rz,sx,sy,sz,tx,ty,tz

real uxc,uyc,uzc,uxxc,uxyc,uxzc,uyxc,uyyc,uyzc,uzxc,uzyc,uzzc

 include 'declareDiffOrder2f.h'
 include 'declareDiffOrder4f.h'
 include 'declareDiffOrder6f.h'
 include 'declareDiffOrder8f.h'
c.......statement functions for jacobian
rx(i1,i2,i3)=rsxy(i1,i2,i3,  1,  1)
ry(i1,i2,i3)=rsxy(i1,i2,i3,  1,  2)
rz(i1,i2,i3)=rsxy(i1,i2,i3,  1,kd3)
sx(i1,i2,i3)=rsxy(i1,i2,i3,  2,  1)
sy(i1,i2,i3)=rsxy(i1,i2,i3,  2,  2)
sz(i1,i2,i3)=rsxy(i1,i2,i3,  2,kd3)
tx(i1,i2,i3)=rsxy(i1,i2,i3,kd3,  1)
ty(i1,i2,i3)=rsxy(i1,i2,i3,kd3,  2)
tz(i1,i2,i3)=rsxy(i1,i2,i3,kd3,kd3)

c     The next macro call will define the difference approximation statement functions
 defineDifferenceOrder2Components1(u,RX)
 defineDifferenceOrder4Components1(u,RX)
 defineDifferenceOrder6Components1(u,RX)
 defineDifferenceOrder8Components1(u,RX)
c we also need derivatives of the scalar "sc"
 defineDifferenceOrder2Components1(sc,)
 defineDifferenceOrder4Components1(sc,)
 defineDifferenceOrder6Components1(sc,)
 defineDifferenceOrder8Components1(sc,)

c......end statement functions

kd3=nd

#If #operator == "divScalarGradNC"
c       ****** divScalarGrad ******

if( derivOption .eq. divScalarGrad )then
 ! **********************************************
 ! ************divScalarGrad*********************
 ! **********************************************
 if( nd .eq. 2 )then
c         ******* 2D *************      
  if( gridType .eq. 0 )then
c           rectangular
    if( order.eq.2 )then
      loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN22R(i1,i2,i3,c)\
                          +SCX22R(i1,i2,i3,0)*UX22R(i1,i2,i3,c)+SCY22R(i1,i2,i3,0)*UY22R(i1,i2,i3,c))
    else if( order.eq.4 )then
      loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN42R(i1,i2,i3,c) \
                   +SCX42R(i1,i2,i3,0)*UX42R(i1,i2,i3,c)+SCY42R(i1,i2,i3,0)*UY42R(i1,i2,i3,c))
    else if( order.eq.6 )then
      loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN62R(i1,i2,i3,c) \
                   +SCX62R(i1,i2,i3,0)*UX62R(i1,i2,i3,c)+SCY62R(i1,i2,i3,0)*UY62R(i1,i2,i3,c))
    else if( order.eq.8 )then
      loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN82R(i1,i2,i3,c) \
                   +SCX82R(i1,i2,i3,0)*UX82R(i1,i2,i3,c)+SCY82R(i1,i2,i3,0)*UY82R(i1,i2,i3,c))
    else
      write(*,*) 'ERROR:divScalarGradNC:order=',order
      stop 43
    end if
  else
c           ***** not rectangular *****
    if( order.eq.2 )then
      loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN22(i1,i2,i3,c)\
                      +SCX22(i1,i2,i3,0)*UX22(i1,i2,i3,c)+SCY22(i1,i2,i3,0)*UY22(i1,i2,i3,c))
    else if( order.eq.4 )then
      loopsDSG(deriv(i1,i2,i3,c)= sc(i1,i2,i3,0)*ULAPLACIAN42(i1,i2,i3,c)\
                      +SCX42(i1,i2,i3,0)*UX42(i1,i2,i3,c)+SCY42(i1,i2,i3,0)*UY42(i1,i2,i3,c))
    else if( order.eq.6 )then
      loopsDSG(deriv(i1,i2,i3,c)= sc(i1,i2,i3,0)*ULAPLACIAN62(i1,i2,i3,c)\
                      +SCX62(i1,i2,i3,0)*UX62(i1,i2,i3,c)+SCY62(i1,i2,i3,0)*UY62(i1,i2,i3,c))
    else if( order.eq.8 )then
      loopsDSG(deriv(i1,i2,i3,c)= sc(i1,i2,i3,0)*ULAPLACIAN82(i1,i2,i3,c)\
                      +SCX82(i1,i2,i3,0)*UX82(i1,i2,i3,c)+SCY82(i1,i2,i3,0)*UY82(i1,i2,i3,c))
    else
      write(*,*) 'ERROR:divScalarGradNC:order=',order
      stop 43
    end if
  endif 
 elseif( nd.eq.3 )then
c         ******* 3D *************      
  if( gridType .eq. 0 )then
c           rectangular
    if( order.eq.2 )then
      loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN23R(i1,i2,i3,c)\
                   +SCX22R(i1,i2,i3,0)*UX23R(i1,i2,i3,c)\
                   +SCY22R(i1,i2,i3,0)*UY23R(i1,i2,i3,c)\
                   +SCZ22R(i1,i2,i3,0)*UZ23R(i1,i2,i3,c))
    else if( order.eq.4 )then
      loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN43R(i1,i2,i3,c)\
                   +SCX42R(i1,i2,i3,0)*UX42R(i1,i2,i3,c)\
                   +SCY42R(i1,i2,i3,0)*UY42R(i1,i2,i3,c)\
                   +SCZ42R(i1,i2,i3,0)*UZ42R(i1,i2,i3,c))
    else if( order.eq.6 )then
      loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN63R(i1,i2,i3,c)\
                   +SCX62R(i1,i2,i3,0)*UX62R(i1,i2,i3,c)\
                   +SCY62R(i1,i2,i3,0)*UY62R(i1,i2,i3,c)\
                   +SCZ62R(i1,i2,i3,0)*UZ62R(i1,i2,i3,c))
    else if( order.eq.8 )then
      loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN83R(i1,i2,i3,c)\
                   +SCX82R(i1,i2,i3,0)*UX82R(i1,i2,i3,c)\
                   +SCY82R(i1,i2,i3,0)*UY82R(i1,i2,i3,c)\
                   +SCZ82R(i1,i2,i3,0)*UZ82R(i1,i2,i3,c))
    end if
  else
c           ***** not rectangular *****
    if( order.eq.2 )then
      loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN23(i1,i2,i3,c)\
                   +SCX23(i1,i2,i3,0)*UX23(i1,i2,i3,c)\
                   +SCY23(i1,i2,i3,0)*UY23(i1,i2,i3,c)\
                   +SCZ23(i1,i2,i3,0)*UZ23(i1,i2,i3,c))
    else if( order.eq.4 )then
      loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN43(i1,i2,i3,c)\
                   +SCX43(i1,i2,i3,0)*UX43(i1,i2,i3,c)\
                   +SCY43(i1,i2,i3,0)*UY43(i1,i2,i3,c)\
                   +SCZ43(i1,i2,i3,0)*UZ43(i1,i2,i3,c))
    else if( order.eq.6 )then
      loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN63(i1,i2,i3,c)\
                   +SCX63(i1,i2,i3,0)*UX63(i1,i2,i3,c)\
                   +SCY63(i1,i2,i3,0)*UY63(i1,i2,i3,c)\
                   +SCZ63(i1,i2,i3,0)*UZ63(i1,i2,i3,c))
    else if( order.eq.8 )then
      loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN83(i1,i2,i3,c)\
                   +SCX83(i1,i2,i3,0)*UX83(i1,i2,i3,c)\
                   +SCY83(i1,i2,i3,0)*UY83(i1,i2,i3,c)\
                   +SCZ83(i1,i2,i3,0)*UZ83(i1,i2,i3,c))
    else
      write(*,*) 'ERROR:divScalarGradNC:order=',order
      stop 43
    end if
  endif 

 else
c         ******* 1D *************      
  if( gridType .eq. 0 )then
c           rectangular
    if( order.eq.2 )then
      loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN21R(i1,i2,i3,c)+SCX22R(i1,i2,i3,0)*UX22R(i1,i2,i3,c))
    else if( order.eq.4 )then
      loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN41R(i1,i2,i3,c)+SCX42R(i1,i2,i3,0)*UX42R(i1,i2,i3,c))
    else if( order.eq.6 )then
      loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN61R(i1,i2,i3,c)+SCX62R(i1,i2,i3,0)*UX62R(i1,i2,i3,c))
    else if( order.eq.8 )then
      loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN81R(i1,i2,i3,c)+SCX82R(i1,i2,i3,0)*UX82R(i1,i2,i3,c))
    else
      write(*,*) 'ERROR:divScalarGradNC:order=',order
      stop 43
    end if

  else
c            ***** not rectangular *****
    if( order.eq.2 )then
      loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN21(i1,i2,i3,c)+SCX21(i1,i2,i3,0)*UX21(i1,i2,i3,c))
    else if( order.eq.4 )then
      loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN41(i1,i2,i3,c)+SCX41(i1,i2,i3,0)*UX41(i1,i2,i3,c))
    else if( order.eq.6 )then
      loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN61(i1,i2,i3,c)+SCX61(i1,i2,i3,0)*UX61(i1,i2,i3,c))
    else if( order.eq.8 )then
      loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN81(i1,i2,i3,c)+SCX81(i1,i2,i3,0)*UX81(i1,i2,i3,c))
    else
      write(*,*) 'ERROR:divScalarGradNC:order=',order
      stop 43
    end if

  endif 
 end if

else if( derivOption .eq. divTensorGrad )then

 ! **********************************************
 ! ************divTensorGrad*********************
 ! **********************************************
 if( nd .eq. 2 )then
c         ******* 2D *************      
  if( gridType .eq. 0 )then
c           rectangular
    if( order.eq.2 )then
      divTensorGrad2d(22R)
    else if( order.eq.4 )then
      divTensorGrad2d(42R)
    else if( order.eq.6 )then
      divTensorGrad2d(62R)
    else if( order.eq.8 )then
      divTensorGrad2d(82R)
    else
      write(*,*) 'ERROR:divTensorGradNC:order=',order
      stop 43
    end if
  else
c           ***** not rectangular *****
    if( order.eq.2 )then
      divTensorGrad2d(22)
    else if( order.eq.4 )then
      divTensorGrad2d(42)
    else if( order.eq.6 )then
      divTensorGrad2d(62)
    else if( order.eq.8 )then
      divTensorGrad2d(82)
    else
      write(*,*) 'ERROR:divTensorGradNC:order=',order
      stop 43
    end if
  endif 
 elseif( nd.eq.3 )then
c         ******* 3D *************      
  if( gridType .eq. 0 )then
c           rectangular
    if( order.eq.2 )then
      divTensorGrad3d(23R)
    else if( order.eq.4 )then
      divTensorGrad3d(43R)
    else if( order.eq.6 )then
      divTensorGrad3d(63R)
    else if( order.eq.8 )then
      divTensorGrad3d(83R)
    else
      write(*,*) 'ERROR:divTensorGradNC:order=',order
      stop 43
    end if
  else
c           ***** not rectangular *****
    if( order.eq.2 )then
      divTensorGrad3d(23)
    else if( order.eq.4 )then
      divTensorGrad3d(43)
    else if( order.eq.6 )then
      divTensorGrad3d(63)
    else if( order.eq.8 )then
      divTensorGrad3d(83)
    else
      write(*,*) 'ERROR:divTensorGradNC:order=',order
      stop 43
    end if
  endif 

 else
c         ******* 1D *************      
  if( gridType .eq. 0 )then
c           rectangular
    if( order.eq.2 )then
      divTensorGrad1d(21R)
    else if( order.eq.4 )then
      divTensorGrad1d(41R)
    else if( order.eq.6 )then
      divTensorGrad1d(61R)
    else if( order.eq.8 )then
      divTensorGrad1d(81R)
    else
      write(*,*) 'ERROR:divTensorGradNC:order=',order
      stop 41
    end if
  else
c           ***** not rectangular *****
    if( order.eq.2 )then
      divTensorGrad1d(21)
    else if( order.eq.4 )then
      divTensorGrad1d(41)
    else if( order.eq.6 )then
      divTensorGrad1d(61)
    else if( order.eq.8 )then
      divTensorGrad1d(81)
    else
      write(*,*) 'ERROR:divTensorGradNC:order=',order
      stop 41
    end if
  endif 

 end if ! end nd.eq.1
else
  write(*,'(" Unexpected value for derivOption=",i6)') derivOption
  stop 4523
end if


#Elif #operator == "laplaceNC"
c       ****** laplace ******

if( nd .eq. 2 )then
c         ******* 2D *************      
  if( gridType .eq. 0 )then
c           rectangular
    if( order.eq.2 )then
      loopsDSG(deriv(i1,i2,i3,c)=ULAPLACIAN22R(i1,i2,i3,c))
    else if( order.eq.4 )then
      loopsDSG(deriv(i1,i2,i3,c)=ULAPLACIAN42R(i1,i2,i3,c))
    else if( order.eq.6 )then
      loopsDSG(deriv(i1,i2,i3,c)=ULAPLACIAN62R(i1,i2,i3,c))
    else if( order.eq.8 )then
      loopsDSG(deriv(i1,i2,i3,c)=ULAPLACIAN82R(i1,i2,i3,c))
    else
      write(*,*) 'ERROR:divScalarGradNC:order=',order
      stop 43
    end if
  else
c           ***** not rectangular *****
    if( order.eq.2 )then
      loopsDSG(deriv(i1,i2,i3,c)=ULAPLACIAN22(i1,i2,i3,c))
    else if( order.eq.4 )then
      loopsDSG(deriv(i1,i2,i3,c)=ULAPLACIAN42(i1,i2,i3,c))
    else if( order.eq.6 )then
      loopsDSG(deriv(i1,i2,i3,c)=ULAPLACIAN62(i1,i2,i3,c))
    else if( order.eq.8 )then
      loopsDSG(deriv(i1,i2,i3,c)=ULAPLACIAN82(i1,i2,i3,c))
    else
      write(*,*) 'ERROR:divScalarGradNC:order=',order
      stop 43
    end if
  endif 
elseif( nd.eq.3 )then
c         ******* 3D *************      
  if( gridType .eq. 0 )then
c           rectangular
    if( order.eq.2 )then
      loopsDSG(deriv(i1,i2,i3,c)=ULAPLACIAN23R(i1,i2,i3,c))
    else if( order.eq.4 )then
      loopsDSG(deriv(i1,i2,i3,c)=ULAPLACIAN43R(i1,i2,i3,c))
    else if( order.eq.6 )then
      loopsDSG(deriv(i1,i2,i3,c)=ULAPLACIAN63R(i1,i2,i3,c))
    else if( order.eq.8 )then
      loopsDSG(deriv(i1,i2,i3,c)=ULAPLACIAN83R(i1,i2,i3,c))
    else
      write(*,*) 'ERROR:divScalarGradNC:order=',order
      stop 43
    end if
  else
c           ***** not rectangular *****
    if( order.eq.2 )then
      loopsDSG(deriv(i1,i2,i3,c)=ULAPLACIAN23(i1,i2,i3,c))
    else if( order.eq.4 )then
      loopsDSG(deriv(i1,i2,i3,c)=ULAPLACIAN43(i1,i2,i3,c))
    else if( order.eq.6 )then
      loopsDSG(deriv(i1,i2,i3,c)=ULAPLACIAN63(i1,i2,i3,c))
    else if( order.eq.8 )then
      loopsDSG(deriv(i1,i2,i3,c)=ULAPLACIAN83(i1,i2,i3,c))
    else
      write(*,*) 'ERROR:divScalarGradNC:order=',order
      stop 43
    end if
  endif 

else
c         ******* 1D *************      
  if( gridType .eq. 0 )then
c           rectangular
    if( order.eq.2 )then
      loopsDSG(deriv(i1,i2,i3,c)=ULAPLACIAN21R(i1,i2,i3,c))
    else if( order.eq.4 )then
      loopsDSG(deriv(i1,i2,i3,c)=ULAPLACIAN41R(i1,i2,i3,c))
    else if( order.eq.6 )then
      loopsDSG(deriv(i1,i2,i3,c)=ULAPLACIAN61R(i1,i2,i3,c))
    else if( order.eq.8 )then
      loopsDSG(deriv(i1,i2,i3,c)=ULAPLACIAN81R(i1,i2,i3,c))
    else
      write(*,*) 'ERROR:divScalarGradNC:order=',order
      stop 43
    end if

  else
c            ***** not rectangular *****
    if( order.eq.2 )then
      loopsDSG(deriv(i1,i2,i3,c)=ULAPLACIAN21(i1,i2,i3,c))
    else if( order.eq.4 )then
      loopsDSG(deriv(i1,i2,i3,c)=ULAPLACIAN41(i1,i2,i3,c))
    else if( order.eq.6 )then
      loopsDSG(deriv(i1,i2,i3,c)=ULAPLACIAN61(i1,i2,i3,c))
    else if( order.eq.8 )then
      loopsDSG(deriv(i1,i2,i3,c)=ULAPLACIAN81(i1,i2,i3,c))
    else
      write(*,*) 'ERROR:divScalarGradNC:order=',order
      stop 43
    end if

  endif 
end if

#Elif #operator == "derivativeScalarDerivativeNC"
c       ****** derivativeScalarDerivative ******

if(      dir1.eq.0 .and. dir2.eq.0 )then
  DXDYNEW(XX,X,X)
else if( dir1.eq.0 .and. dir2.eq.1 )then
  DXDYNEW(XY,X,Y)
else if( dir1.eq.0 .and. dir2.eq.2 )then
  DXDYNEW(XZ,X,Z)
else if( dir1.eq.1 .and. dir2.eq.0 )then
  DXDYNEW(XY,Y,X)
else if( dir1.eq.1 .and. dir2.eq.1 )then
  DXDYNEW(YY,Y,Y)
else if( dir1.eq.1 .and. dir2.eq.2 )then
  DXDYNEW(YZ,Y,Z)
else if( dir1.eq.2 .and. dir2.eq.0 )then
  DXDYNEW(XZ,Z,X)
else if( dir1.eq.2 .and. dir2.eq.1 )then
  DXDYNEW(YZ,Z,Y)
else if( dir1.eq.2 .and. dir2.eq.2 )then
  DXDYNEW(ZZ,Z,Z)
else
  write(*,*) 'ERROR invalid values: dir1=',dir1,' dir2=',dir2
end if
#End
if( nd.eq.0 )then
c       *** add these lines to avoid warnings about unused statement functions
end if

return
end

#endMacro






#beginMacro buildFileNC(x)
#beginFile x ## .f
 nonConservative(x)
 nonConservativeNew(x)
#endFile
#endMacro


      buildFileNC(laplaceNC)
      buildFileNC(divScalarGradNC)
      buildFileNC(derivativeScalarDerivativeNC)


