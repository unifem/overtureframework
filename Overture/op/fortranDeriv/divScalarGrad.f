      subroutine divScalarGradFDeriv( nd, 
     &    nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,         
     &    ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b, ! dimensions for u
     &    ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b, ! dimensions for deriv
     &    n1a,n1b,n2a,n2b,n3a,n3b, ca,cb, 
     &    dx, dr,
     &    rsxy, jacobian, u,s, deriv, 
     &    ndw,w,  ! work space
     &    derivative, derivType, gridType, order, averagingType, 
     &    dir1, dir2  )
c======================================================================
c  Discretizations for
c           div( s grad )
c  
c ca,cb : assign components c=ca,..,cb (base 0)
c derivative : 0=laplace, 1=divScalarGrad, 2=derivativeScalarDerivative
c derivType : 0=nonconservative, 1=conservative
c gridType: 0=rectangular, 1=non-rectangular
c order : 2 or 4
c averagingType : arithmeticAverage=0, harmonicAverage=1
c dir1,dir2 : for derivative=derivativeScalarDerivative
c rsxy : not used if rectangular
c dr : 
c 
c======================================================================
      implicit none
      integer nd, 
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b,
     &  n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,
     &  derivative, derivType, gridType, order, averagingType,ndw,
     & dir1,dir2

      real dx(3),dr(3)
      real rsxy(*)
      real jacobian(*)
      real s(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real u(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,ndu4a:ndu4b)
      real deriv(ndd1a:ndd1b,ndd2a:ndd2b,ndd3a:ndd3b,ndd4a:ndd4b)
      real w(0:*)
      
      real h21(3),d22(3),d12(3),h22(3)
      real d24(3),d14(3),h42(3),h41(3)

      integer n,nda
    
      if( derivType.eq.0 .or. order.eq.4 )then   
c       *** non-conservative *** or 4th order -- we only have non-conservative
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

        call divScalarGradNC( nd, 
     &    nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,         
     &    ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b, 
     &    ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b, 
     &    n1a,n1b,n2a,n2b,n3a,n3b, ca,cb, 
     &    h21, d22,d12, h22, d14, d24, h41, h42, 
     &    rsxy, u,s, deriv, 
     &    derivative, gridType, order, averagingType, dir1, dir2 )

      else

c       *** conservative ***
        if( gridType .eq. 0 )then
c         === rectangular ===
          if( order .eq. 2 )then
c           +++ 2nd order +++
            do n=1,3
              h22(n)=1./(dx(n)**2)
            end do
            if( nd.eq.1 )then
c             one-dimension
              nda=(nd1b-nd1a+1)*(nd2b-nd2a+1)*(nd3b-nd3a+1)
              if( ndw .lt. nda )then
                write(*,*) 'divScalarGradFDeriv:ERROR: '
                write(*,*) 'workspace too small: ndw=',ndw,' < ',nda
                return
              end if
              ! write(*,*) '******divScalarGradFDeriv22********'
              call divScalarGradFDeriv21R( nd, 
     &         nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,         
     &         ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b, 
     &         ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b, 
     &         n1a,n1b,n2a,n2b,n3a,n3b, ca,cb, 
     &         h22,
     &         u,s, deriv, 
     &         w(0),
     &         derivative, gridType, order, averagingType, dir1, dir2)
            else if( nd.eq.2 )then
c             two-dimensions
              nda=(nd1b-nd1a+1)*(nd2b-nd2a+1)*(nd3b-nd3a+1)
              if( ndw .lt. 2*nda )then
                write(*,*) 'divScalarGradFDeriv:ERROR: '
                write(*,*) 'workspace too small: ndw=',ndw,' < ',2*nda
                return
              end if
              ! write(*,*) '******divScalarGradFDeriv22********'
              call divScalarGradFDeriv22R( nd, 
     &         nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,         
     &         ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b, 
     &         ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b, 
     &         n1a,n1b,n2a,n2b,n3a,n3b, ca,cb, 
     &         h22,
     &         u,s, deriv, 
     &         w(0),w(nda),
     &         derivative, gridType, order, averagingType, dir1, dir2 )
            else
c             three-dimensions
              nda=(nd1b-nd1a+1)*(nd2b-nd2a+1)*(nd3b-nd3a+1)
              if( ndw .lt. 3*nda )then
                write(*,*) 'divScalarGradFDeriv:ERROR: '
                write(*,*) 'workspace too small: ndw=',ndw,' < ',3*nda
                return
              end if
              call divScalarGradFDeriv23R( nd, 
     &         nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,         
     &         ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b, 
     &         ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b, 
     &         n1a,n1b,n2a,n2b,n3a,n3b, ca,cb, 
     &         h22,
     &         u,s, deriv, 
     &         w(0),w(nda),w(2*nda),
     &         derivative, gridType, order, averagingType, dir1, dir2 )
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
            if( nd.eq.1 )then
c             one-dimension
              nda=(nd1b-nd1a+1)*(nd2b-nd2a+1)*(nd3b-nd3a+1)
              if( ndw .lt. nda )then
                write(*,*) 'divScalarGradFDeriv:ERROR: '
                write(*,*) 'workspace too small: ndw=',ndw,' < ',nda
                return
              end if
              call divScalarGradFDeriv21( nd, 
     &         nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,         
     &         ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b, 
     &         ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b, 
     &         n1a,n1b,n2a,n2b,n3a,n3b, ca,cb, 
     &         d12,d22,
     &         rsxy, jacobian, u,s, deriv, 
     &         w(0),
     &         derivative, gridType, order, averagingType, dir1, dir2)
            else if( nd.eq.2 )then
c             two-dimensions
              nda=(nd1b-nd1a+1)*(nd2b-nd2a+1)*(nd3b-nd3a+1)
              if( ndw .lt. 4*nda )then
                write(*,*) 'divScalarGradFDeriv:ERROR: '
                write(*,*) 'workspace too small: ndw=',ndw,' < ',4*nda
                return
              end if
              call divScalarGradFDeriv22( nd, 
     &         nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,         
     &         ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b, 
     &         ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b, 
     &         n1a,n1b,n2a,n2b,n3a,n3b, ca,cb, 
     &         d12,d22,
     &         rsxy, jacobian, u,s, deriv, 
     &         w(0),w(nda),w(2*nda),w(3*nda),
     &         derivative, gridType, order, averagingType, dir1, dir2)
            else
c             three-dimensions
              nda=(nd1b-nd1a+1)*(nd2b-nd2a+1)*(nd3b-nd3a+1)
              if( ndw .lt. 8*nda )then
                write(*,*) 'divScalarGradFDeriv:ERROR: '
                write(*,*) 'workspace too small: ndw=',ndw,' < ',8*nda
                return
              end if
              call divScalarGradFDeriv23( nd, 
     &         nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,         
     &         ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b, 
     &         ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b, 
     &         n1a,n1b,n2a,n2b,n3a,n3b, ca,cb, 
     &         d12,d22,
     &         rsxy, jacobian, u,s, deriv, 
     &         w(0),w(nda),w(2*nda),w(3*nda),w(4*nda),w(5*nda),
     &         w(6*nda),w(7*nda),w(8*nda),
     &         derivative, gridType, order, averagingType, dir1, dir2)
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
     &    rsxy, jacobian, u,s, deriv, 
     &    a11,  ! work space
     &    derivative, gridType, order, averagingType, dir1, dir2 )
c ===============================================================
c 2nd order, 1D
c Conservative discretization of
c           div( s grad )
c  
c ca,cb : assign components c=ca,..,cb (base 0)
c gridType: 0=rectangular, 1=non-rectangular
c order : 2 or 4
c rsxy : not used if rectangular
c h22 : 1/h**2 : for rectangular  
c ===============================================================

      implicit none
      integer nd, 
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b,
     &  n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,
     &  derivative, gridType, order, averagingType, dir1, dir2

      integer arithmeticAverage,harmonicAverage
      parameter( arithmeticAverage=0,harmonicAverage=1 ) 
      integer laplace,divScalarGrad,derivativeScalarDerivative
      parameter(laplace=0,divScalarGrad=1,derivativeScalarDerivative=2)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,1:nd,1:nd)
      real jacobian(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real s(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
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

      m1a=n1a-1
      m1b=n1b+1
      m2a=n2a
      m2b=n2b
      m3a=n3a
      m3b=n3b

      if( averagingType .eq. arithmeticAverage )then

        factor=.5

        do j3=m3a,m3b
          do j2=m2a,m2b
            do j1=m1a,m1b
              sj = s(j1,j2,j3) * jacobian(j1,j2,j3)
              a11(j1,j2,j3) = (rx(j1,j2,j3)**2)*sj   ! ***** check this -- must simplify
            end do
          end do
        end do
        
        m1a=n1a
        do j3=m3a,m3b
          do j2=m2a,m2b
            do j1=m1b,m1a,-1 ! go backwards since j1-1 appears below
              a11(j1,j2,j3) = factor*(a11(j1,j2,j3)+a11(j1-1,j2,j3))
            end do
          end do
        end do
        m1a=n1a-1
      else    
c       Harmonic average
   
        factor=2.

        do j3=m3a,m3b
          do j2=m2a,m2b
            do j1=m1a,m1b
              sj=jacobian(j1,j2,j3)
              a11(j1,j2,j3) = (rx(j1,j2,j3)**2)*sj
            end do
          end do
        end do

        m1a=n1a
        do j3=m3a,m3b
          do j2=m2a,m2b
            do j1=m1b,m1a,-1
              sh=s(j1,j2,j3)*s(j1-1,j2,j3)/(s(j1,j2,j3)+s(j1-1,j2,j3)) ! worry about division by zero
              a11(j1,j2,j3) = sh*(a11(j1,j2,j3)+a11(j1-1,j2,j3))
            end do
          end do
        end do
        m1a=n1a-1

      end if    

c  NOTE:The spacing on the cross derivative terms is only 1/dx
    
c    Evaluate the derivative
      do c=ca,cb
        do i3=n3a,n3b
          do i2=n2a,n2b
            do i1=n1a,n1b
              deriv(i1,i2,i3,c)=
     &         (
     &         (a11(i1+1,i2  ,i3  )*urr(i1+1,i2  ,i3  ,c) -
     &          a11(i1  ,i2  ,i3  )*urr(i1  ,i2  ,i3  ,c))*d22(1)
     &        )/jacobian(i1,i2,i3)
            end do
          end do
        end do
      end do

      return 
      end



      subroutine divScalarGradFDeriv22( nd, 
     &    nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,         
     &    ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b, ! dimensions for u
     &    ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b, ! dimensions for deriv
     &    n1a,n1b,n2a,n2b,n3a,n3b, ca,cb, 
     &    d12,d22,
     &    rsxy, jacobian, u,s, deriv, 
     &    a11,a12,a21,a22,  ! work space
     &    derivative, gridType, order, averagingType, dir1, dir2 )
c ===============================================================
c 2nd order, 2D
c Conservative discretization of
c           div( s grad )
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
     &  derivative, gridType, order, averagingType, dir1, dir2

      integer arithmeticAverage,harmonicAverage
      parameter( arithmeticAverage=0,harmonicAverage=1 ) 
      integer laplace,divScalarGrad,derivativeScalarDerivative
      parameter(laplace=0,divScalarGrad=1,derivativeScalarDerivative=2)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,1:nd,1:nd)
      real jacobian(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real s(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real u(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,ndu4a:ndu4b)
      real deriv(ndd1a:ndd1b,ndd2a:ndd2b,ndd3a:ndd3b,ndd4a:ndd4b)

      real a11(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real a21(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real a12(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real a22(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)

      real d12(*),d22(*)

c      real rx,ry,rz,sx,sy,sz,tx,ty,tz,factor,sh,sj
c      real urr,uss,urs,usr

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
      ! Estimate D{-r}(i+1/2,j-1/2,k)
      usr(i1,i2,i3,c) = (u(i1+1,i2-1,i3,c) + u(i1+1,i2,i3,c) - 
     &                   u(i1-1,i2-1,i3,c) - u(i1-1,i2,i3,c))
      ! Estimate D{-s}(i,j-1/2,k)
      uss(i1,i2,i3,c) = u(i1,i2,i3,c)-u(i1,i2-1,i3,c)

c.......end statement functions

      kd3=nd

      m1a=n1a-1
      m1b=n1b+1
      m2a=n2a-1
      m2b=n2b+1
      m3a=n3a
      m3b=n3b

      if( averagingType .eq. arithmeticAverage )then

        factor=.5

        do j3=m3a,m3b
          do j2=m2a,m2b
            do j1=m1a,m1b
              sj = s(j1,j2,j3) * jacobian(j1,j2,j3)
              a11(j1,j2,j3) = (rx(j1,j2,j3)**2+ry(j1,j2,j3)**2)*sj
              a12(j1,j2,j3) = (rx(j1,j2,j3)*sx(j1,j2,j3)+
     &                         ry(j1,j2,j3)*sy(j1,j2,j3))*sj 
              a22(j1,j2,j3) = (sx(j1,j2,j3)**2+sy(j1,j2,j3)**2)*sj 
              a21(j1,j2,j3) = a12(j1,j2,j3)
            end do
          end do
        end do
        
        m1a=n1a
        do j3=m3a,m3b
          do j2=m2a,m2b
            do j1=m1b,m1a,-1 ! go backwards since j1-1 appears below
              a11(j1,j2,j3) = factor*(a11(j1,j2,j3)+a11(j1-1,j2,j3))
              a12(j1,j2,j3) = factor*(a12(j1,j2,j3)+a12(j1-1,j2,j3))
            end do
          end do
        end do
        m1a=n1a-1
        m2a=n2a
        do j3=m3a,m3b
          do j2=m2b,m2a,-1
            do j1=m1a,m1b
              a21(j1,j2,j3) = factor*(a21(j1,j2,j3)+a21(j1,j2-1,j3))
              a22(j1,j2,j3) = factor*(a22(j1,j2,j3)+a22(j1,j2-1,j3))
            end do
          end do
        end do
        m2a=n2a-1
      else    
c       Harmonic average
   
        factor=2.

        do j3=m3a,m3b
          do j2=m2a,m2b
            do j1=m1a,m1b
              sj=jacobian(j1,j2,j3)
              a11(j1,j2,j3) = (rx(j1,j2,j3)**2+ry(j1,j2,j3)**2)*sj
              a12(j1,j2,j3) = (rx(j1,j2,j3)*sx(j1,j2,j3)+
     &          ry(j1,j2,j3)*sy(j1,j2,j3))*sj 
              a22(j1,j2,j3) = (sx(j1,j2,j3)**2+sy(j1,j2,j3)**2)*sj 
              a21(j1,j2,j3) = a12(j1,j2,j3)
            end do
          end do
        end do

        m1a=n1a
        do j3=m3a,m3b
          do j2=m2a,m2b
            do j1=m1b,m1a,-1
              sh=s(j1,j2,j3)*s(j1-1,j2,j3)/(s(j1,j2,j3)+s(j1-1,j2,j3)) ! worry about division by zero
              a11(j1,j2,j3) = sh*(a11(j1,j2,j3)+a11(j1-1,j2,j3))
              a12(j1,j2,j3) = sh*(a12(j1,j2,j3)+a12(j1-1,j2,j3))
            end do
          end do
        end do
        m1a=n1a-1
        m2a=n2a
        do j3=m3a,m3b
          do j2=m2b,m2a,-1
            do j1=m1a,m1b
              sh=s(j1,j2,j3)*s(j1,j2-1,j3)/(s(j1,j2,j3)+s(j1,j2-1,j3)) 
              a21(j1,j2,j3) = sh*(a21(j1,j2,j3)+a21(j1,j2-1,j3))
              a22(j1,j2,j3) = sh*(a22(j1,j2,j3)+a22(j1,j2-1,j3))
            end do
          end do
        end do
        m2a=n2a-1

      end if    

c  NOTE:The spacing on the cross derivative terms is only 1/dx
    
c    Evaluate the derivative
      do c=ca,cb
        do i3=n3a,n3b
          do i2=n2a,n2b
            do i1=n1a,n1b

              deriv(i1,i2,i3,c)=
     &         (
     &         (a11(i1+1,i2  ,i3  )*urr(i1+1,i2  ,i3  ,c) -
     &          a11(i1  ,i2  ,i3  )*urr(i1  ,i2  ,i3  ,c))*d22(1)+
     &         (a22(i1  ,i2+1,i3  )*uss(i1  ,i2+1,i3  ,c) - 
     &          a22(i1  ,i2  ,i3  )*uss(i1  ,i2  ,i3  ,c))*d22(2)+
     &         (a21(i1  ,i2+1,i3  )*usr(i1  ,i2+1,i3  ,c) - 
     &          a21(i1  ,i2  ,i3  )*usr(i1  ,i2  ,i3  ,c) +
     &     a12(i1+1,i2  ,i3  )*urs(i1+1,i2  ,i3  ,c) - 
     &     a12(i1  ,i2  ,i3  )*urs(i1  ,i2  ,i3  ,c))*(d12(1)*d12(2))
     &        )/jacobian(i1,i2,i3)
            end do
          end do
        end do
      end do

      return 
      end

      subroutine divScalarGradFDeriv23( nd, 
     &    nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,         
     &    ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b, ! dimensions for u
     &    ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b, ! dimensions for deriv
     &    n1a,n1b,n2a,n2b,n3a,n3b, ca,cb, 
     &    d12,d22,
     &    rsxy, jacobian, u,s, deriv, 
     &    a11,a12,a13,a21,a22,a23,a31,a32,a33,  ! work space
     &    derivative, gridType, order, averagingType, dir1, dir2 )
c ===============================================================
c 2nd order, 3D
c Conservative discretization of
c           div( s grad )
c  
c ca,cb : assign components c=ca,..,cb (base 0)
c gridType: 0=rectangular, 1=non-rectangular
c order : 2 or 4
c rsxy : not used if rectangular
c h22 : 1/h**2 : for rectangular  
c ===============================================================

      implicit none
      integer nd, 
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b,
     &  n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,
     &  derivative, gridType, order, averagingType, dir1, dir2

      integer arithmeticAverage,harmonicAverage
      parameter( arithmeticAverage=0,harmonicAverage=1 ) 
      integer laplace,divScalarGrad,derivativeScalarDerivative
      parameter(laplace=0,divScalarGrad=1,derivativeScalarDerivative=2)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,1:nd,1:nd)
      real jacobian(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real s(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
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

      real rx,ry,rz,sx,sy,sz,tx,ty,tz,factor,sh,sj
      real urr,uss,utt,urs,urt,ust,usr,utr,uts

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

c.......end statement functions

      kd3=nd

c // Cell Spacing
c const RealArray & d12 = mgop.d12;  // 1/ (2 dx)
c const RealArray & d22 = mgop.d22;  // 1/ (dx*dx)
c   
c Index J1 = Range(I1.getBase()-1,I1.getBound()+1);
c Index J2 = Range(I2.getBase()-1,I2.getBound()+1);
c Index J3 = Range(I3.getBase()-1,I3.getBound()+1);

      m1a=n1a-1
      m1b=n1b+1
      m2a=n2a-1
      m2b=n2b+1
      m3a=n3a-1
      m3b=n3b+1

        if( derivative.eq.laplace )then
        else if( derivative.eq.divScalarGrad )then
        else if( derivative.eq.derivativeScalarDerivative )then
        end if       

      if( averagingType .eq. arithmeticAverage )then
        factor=.5

        if( derivative.eq.laplace )then
          do j3=m3a,m3b
          do j2=m2a,m2b
            do j1=m1a,m1b
              a11(j1,j2,j3) = (rx(j1,j2,j3)**2+ry(j1,j2,j3)**2+
     &                         rz(j1,j2,j3)**2)
              a12(j1,j2,j3) = (rx(j1,j2,j3)*sx(j1,j2,j3)+
     &          ry(j1,j2,j3)*sy(j1,j2,j3)+rz(j1,j2,j3)*sz(j1,j2,j3)) 
              a13(j1,j2,j3) = (rx(j1,j2,j3)*tx(j1,j2,j3)+
     &          ry(j1,j2,j3)*ty(j1,j2,j3)+rz(j1,j2,j3)*tz(j1,j2,j3)) 
              a22(j1,j2,j3) = (sx(j1,j2,j3)**2+sy(j1,j2,j3)**2+
     &                sz(j1,j2,j3)**2) 
              a23(j1,j2,j3) = (sx(j1,j2,j3)*tx(j1,j2,j3)+
     &          sy(j1,j2,j3)*ty(j1,j2,j3)+sz(j1,j2,j3)*tz(j1,j2,j3)) 
              a33(j1,j2,j3) = (tx(j1,j2,j3)**2+ty(j1,j2,j3)**2+
     &            tz(j1,j2,j3)**2) 
              a21(j1,j2,j3) = a12(j1,j2,j3)
              a31(j1,j2,j3) = a13(j1,j2,j3)
              a32(j1,j2,j3) = a23(j1,j2,j3)
            end do
          end do
          end do
        else if( derivative.eq.divScalarGrad )then
          do j3=m3a,m3b
          do j2=m2a,m2b
            do j1=m1a,m1b
              sj = s(j1,j2,j3) * jacobian(j1,j2,j3)
              a11(j1,j2,j3) = (rx(j1,j2,j3)**2+ry(j1,j2,j3)**2+
     &                         rz(j1,j2,j3)**2)*sj
              a12(j1,j2,j3) = (rx(j1,j2,j3)*sx(j1,j2,j3)+
     &          ry(j1,j2,j3)*sy(j1,j2,j3)+rz(j1,j2,j3)*sz(j1,j2,j3))*sj 
              a13(j1,j2,j3) = (rx(j1,j2,j3)*tx(j1,j2,j3)+
     &          ry(j1,j2,j3)*ty(j1,j2,j3)+rz(j1,j2,j3)*tz(j1,j2,j3))*sj 
              a22(j1,j2,j3) = (sx(j1,j2,j3)**2+sy(j1,j2,j3)**2+
     &                sz(j1,j2,j3)**2)*sj 
              a23(j1,j2,j3) = (sx(j1,j2,j3)*tx(j1,j2,j3)+
     &          sy(j1,j2,j3)*ty(j1,j2,j3)+sz(j1,j2,j3)*tz(j1,j2,j3))*sj 
              a33(j1,j2,j3) = (tx(j1,j2,j3)**2+ty(j1,j2,j3)**2+
     &            tz(j1,j2,j3)**2)*sj 
              a21(j1,j2,j3) = a12(j1,j2,j3)
              a31(j1,j2,j3) = a13(j1,j2,j3)
              a32(j1,j2,j3) = a23(j1,j2,j3)
            end do
          end do
          end do
        else if( derivative.eq.derivativeScalarDerivative )then
          do j3=m3a,m3b
          do j2=m2a,m2b
            do j1=m1a,m1b
              sj = s(j1,j2,j3) * jacobian(j1,j2,j3)
              a11(j1,j2,j3) = (rx(j1,j2,j3)**2+ry(j1,j2,j3)**2+
     &                         rz(j1,j2,j3)**2)*sj
              a12(j1,j2,j3) = (rx(j1,j2,j3)*sx(j1,j2,j3)+
     &          ry(j1,j2,j3)*sy(j1,j2,j3)+rz(j1,j2,j3)*sz(j1,j2,j3))*sj 
              a13(j1,j2,j3) = (rx(j1,j2,j3)*tx(j1,j2,j3)+
     &          ry(j1,j2,j3)*ty(j1,j2,j3)+rz(j1,j2,j3)*tz(j1,j2,j3))*sj 
              a22(j1,j2,j3) = (sx(j1,j2,j3)**2+sy(j1,j2,j3)**2+
     &                sz(j1,j2,j3)**2)*sj 
              a23(j1,j2,j3) = (sx(j1,j2,j3)*tx(j1,j2,j3)+
     &          sy(j1,j2,j3)*ty(j1,j2,j3)+sz(j1,j2,j3)*tz(j1,j2,j3))*sj 
              a33(j1,j2,j3) = (tx(j1,j2,j3)**2+ty(j1,j2,j3)**2+
     &            tz(j1,j2,j3)**2)*sj 
              a21(j1,j2,j3) = a12(j1,j2,j3)
              a31(j1,j2,j3) = a13(j1,j2,j3)
              a32(j1,j2,j3) = a23(j1,j2,j3)
            end do
          end do
          end do
        end if       
        
        m1a=n1a
        do j3=m3a,m3b
          do j2=m2a,m2b
            do j1=m1b,m1a,-1 ! go backwards
              a11(j1,j2,j3) = factor*(a11(j1,j2,j3)+a11(j1-1,j2,j3))
              a12(j1,j2,j3) = factor*(a12(j1,j2,j3)+a12(j1-1,j2,j3))
              a13(j1,j2,j3) = factor*(a13(j1,j2,j3)+a13(j1-1,j2,j3))
            end do
          end do
        end do
        m1a=n1a-1
        m2a=n2a
        do j3=m3a,m3b
          do j2=m2b,m2a,-1
            do j1=m1a,m1b
              a21(j1,j2,j3) = factor*(a21(j1,j2,j3)+a21(j1,j2-1,j3))
              a22(j1,j2,j3) = factor*(a22(j1,j2,j3)+a22(j1,j2-1,j3))
              a23(j1,j2,j3) = factor*(a23(j1,j2,j3)+a23(j1,j2-1,j3))
            end do
          end do
        end do
        m2a=n2a-1
        m3a=n3a
        do j3=m3b,m3a,-1
          do j2=m2a,m2b
            do j1=m1a,m1b
              a31(j1,j2,j3) = factor*(a31(j1,j2,j3)+a31(j1,j2,j3-1))
              a32(j1,j2,j3) = factor*(a32(j1,j2,j3)+a32(j1,j2,j3-1))
              a33(j1,j2,j3) = factor*(a33(j1,j2,j3)+a33(j1,j2,j3-1))
            end do
          end do
        end do
        m3a=n3a-1
      else    
c       Harmonic average
   
        factor=2.

        do j3=m3a,m3b
          do j2=m2a,m2b
            do j1=m1a,m1b
              sj=jacobian(j1,j2,j3)
              a11(j1,j2,j3) = (rx(j1,j2,j3)**2+ry(j1,j2,j3)**2+
     &                         rz(j1,j2,j3)**2)*sj
              a12(j1,j2,j3) = (rx(j1,j2,j3)*sx(j1,j2,j3)+
     &          ry(j1,j2,j3)*sy(j1,j2,j3)+rz(j1,j2,j3)*sz(j1,j2,j3))*sj 
              a13(j1,j2,j3) = (rx(j1,j2,j3)*tx(j1,j2,j3)+
     &          ry(j1,j2,j3)*ty(j1,j2,j3)+rz(j1,j2,j3)*tz(j1,j2,j3))*sj 
              a22(j1,j2,j3) = (sx(j1,j2,j3)**2+sy(j1,j2,j3)**2+
     &                sz(j1,j2,j3)**2)*sj 
              a23(j1,j2,j3) = (sx(j1,j2,j3)*tx(j1,j2,j3)+
     &          sy(j1,j2,j3)*ty(j1,j2,j3)+sz(j1,j2,j3)*tz(j1,j2,j3))*sj 
              a33(j1,j2,j3) = (tx(j1,j2,j3)**2+ty(j1,j2,j3)**2+
     &            tz(j1,j2,j3)**2)*sj 
              a21(j1,j2,j3) = a12(j1,j2,j3)
              a31(j1,j2,j3) = a13(j1,j2,j3)
              a32(j1,j2,j3) = a23(j1,j2,j3)
            end do
          end do
        end do

        m1a=n1a
        do j3=m3a,m3b
          do j2=m2a,m2b
            do j1=m1b,m1a,-1
              sh=s(j1,j2,j3)*s(j1-1,j2,j3)/(s(j1,j2,j3)+s(j1-1,j2,j3)) ! worry about division by zero
              a11(j1,j2,j3) = sh*(a11(j1,j2,j3)+a11(j1-1,j2,j3))
              a12(j1,j2,j3) = sh*(a12(j1,j2,j3)+a12(j1-1,j2,j3))
              a13(j1,j2,j3) = sh*(a13(j1,j2,j3)+a13(j1-1,j2,j3))
            end do
          end do
        end do
        m1a=n1a-1
        m2a=n2a
        do j3=m3a,m3b
          do j2=m2b,m2a,-1
            do j1=m1a,m1b
              sh=s(j1,j2,j3)*s(j1,j2-1,j3)/(s(j1,j2,j3)+s(j1,j2-1,j3)) 
              a21(j1,j2,j3) = sh*(a21(j1,j2,j3)+a21(j1,j2-1,j3))
              a22(j1,j2,j3) = sh*(a22(j1,j2,j3)+a22(j1,j2-1,j3))
              a23(j1,j2,j3) = sh*(a23(j1,j2,j3)+a23(j1,j2-1,j3))
            end do
          end do
        end do
        m2a=n2a-1
        m3a=n3a
        do j3=m3b,m3a,-1
          do j2=m2a,m2b
            do j1=m1a,m1b
              sh=s(j1,j2,j3)*s(j1,j2,j3-1)/(s(j1,j2,j3)+s(j1,j2,j3-1)) 
              a31(j1,j2,j3) = sh*(a31(j1,j2,j3)+a31(j1,j2,j3-1))
              a32(j1,j2,j3) = sh*(a32(j1,j2,j3)+a32(j1,j2,j3-1))
              a33(j1,j2,j3) = sh*(a33(j1,j2,j3)+a33(j1,j2,j3-1))
            end do
          end do
        end do
        m3a=n3a-1

      end if    

c  NOTE:The spacing on the cross derivative terms is only 1/dx
    
c    Evaluate the derivative
      do c=ca,cb
        do i3=n3a,n3b
          do i2=n2a,n2b
            do i1=n1a,n1b

              deriv(i1,i2,i3,c)=
     &         (
     &         (a11(i1+1,i2  ,i3  )*urr(i1+1,i2  ,i3  ,c) -
     &          a11(i1,i2,i3)*urr(i1,i2,i3,c))*d22(1)+
     &        (a22(i1  ,i2+1,i3  )*uss(i1  ,i2+1,i3  ,c) - 
     &         a22(i1,i2,i3)*uss(i1,i2,i3,c))*d22(2)+
     &        (a33(i1  ,i2  ,i3+1)*utt(i1  ,i2  ,i3+1,c) - 
     &         a33(i1,i2,i3)*utt(i1,i2,i3,c))*d22(3)+
     &        (a21(i1  ,i2+1,i3  )*usr(i1  ,i2+1,i3  ,c) - 
     &         a21(i1,i2,i3)*usr(i1,i2,i3,c) +
     &        a12(i1+1,i2  ,i3  )*urs(i1+1,i2  ,i3  ,c) - 
     &        a12(i1,i2,i3)*urs(i1,i2,i3,c))*(d12(1)*d12(2))+
     &        (a31(i1  ,i2  ,i3+1)*utr(i1  ,i2  ,i3+1,c) - 
     &         a31(i1,i2,i3)*utr(i1,i2,i3,c) +
     &        a13(i1+1,i2  ,i3  )*urt(i1+1,i2  ,i3  ,c) - 
     &        a13(i1,i2,i3)*urt(i1,i2,i3,c))*(d12(1)*d12(3))+
     &        (a32(i1  ,i2  ,i3+1)*uts(i1  ,i2  ,i3+1,c) - 
     &         a32(i1,i2,i3)*uts(i1,i2,i3,c) +
     &        a23(i1  ,i2+1,i3  )*ust(i1  ,i2+1,i3  ,c) - 
     &        a23(i1,i2,i3)*ust(i1,i2,i3,c))*(d12(2)*d12(3))
     &        )/jacobian(i1,i2,i3)
            end do
          end do
        end do
      end do

      return 
      end


      subroutine divScalarGradFDeriv21R( nd, 
     &    nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,         
     &    ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b, ! dimensions for u
     &    ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b, ! dimensions for deriv
     &    n1a,n1b,n2a,n2b,n3a,n3b, ca,cb, 
     &    h22,
     &    u,s, deriv, 
     &    a11,  ! work space
     &    derivative, gridType, order, averagingType, dir1, dir2 )
c ===============================================================
c 2nd order, 3D, rectangular
c Conservative discretization of
c           div( s grad )
c  
c ca,cb : assign components c=ca,..,cb (base 0)
c gridType: 0=rectangular, 1=non-rectangular
c order : 2 or 4
c rsxy : not used if rectangular
c h22 : 1/h**2 : for rectangular  
c ===============================================================

      implicit none
      integer nd, 
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b,
     &  n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,
     &  derivative, gridType, order, averagingType, dir1, dir2

      integer arithmeticAverage,harmonicAverage
      parameter( arithmeticAverage=0,harmonicAverage=1 ) 
      integer laplace,divScalarGrad,derivativeScalarDerivative
      parameter(laplace=0,divScalarGrad=1,derivativeScalarDerivative=2)
      real s(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real u(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,ndu4a:ndu4b)
      real deriv(ndd1a:ndd1b,ndd2a:ndd2b,ndd3a:ndd3b,ndd4a:ndd4b)

      real a11(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real h22(*)
      real factor
      real urr

      integer i1,i2,i3,kd3,c,j1,j2,j3
      integer  m1a,m1b,m2a,m2b,m3a,m3b

c.......statement functions 
      ! Estimate D{-r}(i-1/2,j,k)
      urr(i1,i2,i3,c)=u(i1,i2,i3,c)-u(i1-1,i2,i3,c) 

c.......end statement functions

      kd3=nd

      m1a=n1a-1
      m1b=n1b+1
      m2a=n2a
      m2b=n2b
      m3a=n3a
      m3b=n3b

      if( averagingType .eq. arithmeticAverage )then

        factor=.5

        m1a=n1a
        do j3=m3a,m3b
          do j2=m2a,m2b
            do j1=m1a,m1b 
              a11(j1,j2,j3) = factor*(s(j1,j2,j3)+s(j1-1,j2,j3))
            end do
          end do
        end do
        m1a=n1a-1
      else    
c       Harmonic average
   
        factor=2.
        m1a=n1a
        do j3=m3a,m3b
          do j2=m2a,m2b
            do j1=m1b,m1a,-1
              a11(j1,j2,j3) = s(j1,j2,j3)*s(j1-1,j2,j3)/
     &              (s(j1,j2,j3)+s(j1-1,j2,j3)) ! worry about division by zero
            end do
          end do
        end do
        m1a=n1a-1
      end if    

c  NOTE:The spacing on the cross derivative terms is only 1/dx
    
c    Evaluate the derivative
      do c=ca,cb
        do i3=n3a,n3b
          do i2=n2a,n2b
            do i1=n1a,n1b

              deriv(i1,i2,i3,c)=
     &         (
     &         (a11(i1+1,i2  ,i3  )*urr(i1+1,i2  ,i3  ,c) -
     &          a11(i1,i2,i3)*urr(i1,i2,i3,c))*h22(1)
     &        )
            end do
          end do
        end do
      end do

      return 
      end

      subroutine divScalarGradFDeriv22R( nd, 
     &    nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,         
     &    ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b, ! dimensions for u
     &    ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b, ! dimensions for deriv
     &    n1a,n1b,n2a,n2b,n3a,n3b, ca,cb, 
     &    h22,
     &    u,s, deriv, 
     &    a11,a22,  ! work space
     &    derivative, gridType, order, averagingType, dir1, dir2 )
c ===============================================================
c 2nd order, 3D, rectangular
c Conservative discretization of
c           div( s grad )
c  
c ca,cb : assign components c=ca,..,cb (base 0)
c gridType: 0=rectangular, 1=non-rectangular
c order : 2 or 4
c rsxy : not used if rectangular
c h22 : 1/h**2 : for rectangular  
c ===============================================================

      implicit none
      integer nd, 
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b,
     &  n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,
     &  derivative, gridType, order, averagingType, dir1, dir2

      integer arithmeticAverage,harmonicAverage
      parameter( arithmeticAverage=0,harmonicAverage=1 ) 
      integer laplace,divScalarGrad,derivativeScalarDerivative
      parameter(laplace=0,divScalarGrad=1,derivativeScalarDerivative=2)
      real s(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real u(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,ndu4a:ndu4b)
      real deriv(ndd1a:ndd1b,ndd2a:ndd2b,ndd3a:ndd3b,ndd4a:ndd4b)

      real a11(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real a22(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)

      real h22(*)

      real factor
      real urr,uss

      integer i1,i2,i3,kd3,c,j1,j2,j3
      integer  m1a,m1b,m2a,m2b,m3a,m3b

c.......statement functions 
      ! Estimate D{-r}(i-1/2,j,k)
      urr(i1,i2,i3,c)=u(i1,i2,i3,c)-u(i1-1,i2,i3,c) 
      ! Estimate D{-s}(i,j-1/2,k)
      uss(i1,i2,i3,c) = u(i1,i2,i3,c)-u(i1,i2-1,i3,c)

c.......end statement functions

      kd3=nd

c // Cell Spacing
c const RealArray & d12 = mgop.d12;  // 1/ (2 dx)
c const RealArray & d22 = mgop.d22;  // 1/ (dx*dx)
c   
c Index J1 = Range(I1.getBase()-1,I1.getBound()+1);
c Index J2 = Range(I2.getBase()-1,I2.getBound()+1);
c Index J3 = Range(I3.getBase()-1,I3.getBound()+1);

      m1a=n1a-1
      m1b=n1b+1
      m2a=n2a-1
      m2b=n2b+1
      m3a=n3a
      m3b=n3b

      if( averagingType .eq. arithmeticAverage )then

        factor=.5

        m1a=n1a
        do j3=m3a,m3b
          do j2=m2a,m2b
            do j1=m1a,m1b 
              a11(j1,j2,j3) = factor*(s(j1,j2,j3)+s(j1-1,j2,j3))
            end do
          end do
        end do
        m1a=n1a-1
        m2a=n2a
        do j3=m3a,m3b
          do j2=m2a,m2b
            do j1=m1a,m1b
              a22(j1,j2,j3) = factor*(s(j1,j2,j3)+s(j1,j2-1,j3))
            end do
          end do
        end do
        m2a=n2a-1
      else    
c       Harmonic average
   
        factor=2.
        m1a=n1a
        do j3=m3a,m3b
          do j2=m2a,m2b
            do j1=m1b,m1a,-1
              a11(j1,j2,j3) = s(j1,j2,j3)*s(j1-1,j2,j3)/
     &              (s(j1,j2,j3)+s(j1-1,j2,j3)) ! worry about division by zero
            end do
          end do
        end do
        m1a=n1a-1
        m2a=n2a
        do j3=m3a,m3b
          do j2=m2b,m2a,-1
            do j1=m1a,m1b
              a22(j1,j2,j3) = s(j1,j2,j3)*s(j1,j2-1,j3)/
     &              (s(j1,j2,j3)+s(j1,j2-1,j3)) 
            end do
          end do
        end do
        m2a=n2a-1

      end if    

c  NOTE:The spacing on the cross derivative terms is only 1/dx
    
c    Evaluate the derivative
      do c=ca,cb
        do i3=n3a,n3b
          do i2=n2a,n2b
            do i1=n1a,n1b

              deriv(i1,i2,i3,c)=
     &         (
     &         (a11(i1+1,i2  ,i3  )*urr(i1+1,i2  ,i3  ,c) -
     &          a11(i1,i2,i3)*urr(i1,i2,i3,c))*h22(1)+
     &         (a22(i1  ,i2+1,i3  )*uss(i1  ,i2+1,i3  ,c) - 
     &          a22(i1,i2,i3)*uss(i1,i2,i3,c))*h22(2)
     &        )
            end do
          end do
        end do
      end do

      return 
      end


      subroutine divScalarGradFDeriv23R( nd, 
     &    nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,         
     &    ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b, ! dimensions for u
     &    ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b, ! dimensions for deriv
     &    n1a,n1b,n2a,n2b,n3a,n3b, ca,cb, 
     &    h22,
     &    u,s, deriv, 
     &    a11,a22,a33,  ! work space
     &    derivative, gridType, order, averagingType, dir1, dir2 )
c ===============================================================
c 2nd order, 3D, rectangular
c Conservative discretization of
c           div( s grad )
c  
c ca,cb : assign components c=ca,..,cb (base 0)
c gridType: 0=rectangular, 1=non-rectangular
c order : 2 or 4
c rsxy : not used if rectangular
c h22 : 1/h**2 : for rectangular  
c ===============================================================

      implicit none
      integer nd, 
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b,
     &  n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,
     &  derivative, gridType, order, averagingType, dir1, dir2

      integer arithmeticAverage,harmonicAverage
      parameter( arithmeticAverage=0,harmonicAverage=1 ) 
      integer laplace,divScalarGrad,derivativeScalarDerivative
      parameter(laplace=0,divScalarGrad=1,derivativeScalarDerivative=2)
      real s(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real u(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,ndu4a:ndu4b)
      real deriv(ndd1a:ndd1b,ndd2a:ndd2b,ndd3a:ndd3b,ndd4a:ndd4b)

      real a11(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real a22(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real a33(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)

      real h22(*)

      real factor
      real urr,uss,utt

      integer i1,i2,i3,kd3,c,j1,j2,j3
      integer  m1a,m1b,m2a,m2b,m3a,m3b

c.......statement functions 
      ! Estimate D{-r}(i-1/2,j,k)
      urr(i1,i2,i3,c)=u(i1,i2,i3,c)-u(i1-1,i2,i3,c) 
      ! Estimate D{-t}(i,j,k-1/2)
      utt(i1,i2,i3,c) =u(i1,i2,i3,c)-u(i1,i2,i3-1,c) 
      ! Estimate D{-s}(i,j-1/2,k)
      uss(i1,i2,i3,c) = u(i1,i2,i3,c)-u(i1,i2-1,i3,c)

c.......end statement functions

      kd3=nd

c // Cell Spacing
c const RealArray & d12 = mgop.d12;  // 1/ (2 dx)
c const RealArray & d22 = mgop.d22;  // 1/ (dx*dx)
c   
c Index J1 = Range(I1.getBase()-1,I1.getBound()+1);
c Index J2 = Range(I2.getBase()-1,I2.getBound()+1);
c Index J3 = Range(I3.getBase()-1,I3.getBound()+1);

      m1a=n1a-1
      m1b=n1b+1
      m2a=n2a-1
      m2b=n2b+1
      m3a=n3a-1
      m3b=n3b+1

      if( averagingType .eq. arithmeticAverage )then

        factor=.5

        m1a=n1a
        do j3=m3a,m3b
          do j2=m2a,m2b
            do j1=m1a,m1b 
              a11(j1,j2,j3) = factor*(s(j1,j2,j3)+s(j1-1,j2,j3))
            end do
          end do
        end do
        m1a=n1a-1
        m2a=n2a
        do j3=m3a,m3b
          do j2=m2a,m2b
            do j1=m1a,m1b
              a22(j1,j2,j3) = factor*(s(j1,j2,j3)+s(j1,j2-1,j3))
            end do
          end do
        end do
        m2a=n2a-1
        m3a=n3a
        do j3=m3a,m3b
          do j2=m2a,m2b
            do j1=m1a,m1b
              a33(j1,j2,j3) = factor*(s(j1,j2,j3)+s(j1,j2,j3-1))
            end do
          end do
        end do
        m3a=n3a-1
      else    
c       Harmonic average
   
        factor=2.
        m1a=n1a
        do j3=m3a,m3b
          do j2=m2a,m2b
            do j1=m1b,m1a,-1
              a11(j1,j2,j3) = s(j1,j2,j3)*s(j1-1,j2,j3)/
     &              (s(j1,j2,j3)+s(j1-1,j2,j3)) ! worry about division by zero
            end do
          end do
        end do
        m1a=n1a-1
        m2a=n2a
        do j3=m3a,m3b
          do j2=m2b,m2a,-1
            do j1=m1a,m1b
              a22(j1,j2,j3) = s(j1,j2,j3)*s(j1,j2-1,j3)/
     &              (s(j1,j2,j3)+s(j1,j2-1,j3)) 
            end do
          end do
        end do
        m2a=n2a-1
        m3a=n3a
        do j3=m3b,m3a,-1
          do j2=m2a,m2b
            do j1=m1a,m1b
              a33(j1,j2,j3) = s(j1,j2,j3)*s(j1,j2,j3-1)/
     &              (s(j1,j2,j3)+s(j1,j2,j3-1)) 
            end do
          end do
        end do
        m3a=n3a-1

      end if    

c  NOTE:The spacing on the cross derivative terms is only 1/dx
    
c    Evaluate the derivative
      do c=ca,cb
        do i3=n3a,n3b
          do i2=n2a,n2b
            do i1=n1a,n1b

              deriv(i1,i2,i3,c)=
     &         (
     &         (a11(i1+1,i2  ,i3  )*urr(i1+1,i2  ,i3  ,c) -
     &          a11(i1,i2,i3)*urr(i1,i2,i3,c))*h22(1)+
     &         (a22(i1  ,i2+1,i3  )*uss(i1  ,i2+1,i3  ,c) - 
     &          a22(i1,i2,i3)*uss(i1,i2,i3,c))*h22(2)+
     &         (a33(i1  ,i2  ,i3+1)*utt(i1  ,i2  ,i3+1,c) - 
     &          a33(i1,i2,i3)*utt(i1,i2,i3,c))*h22(3)
     &        )
            end do
          end do
        end do
      end do

      return 
      end


      subroutine divScalarGradNC( nd, 
     &    nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,         
     &    ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b, ! dimensions for u
     &    ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b, ! dimensions for deriv
     &    n1a,n1b,n2a,n2b,n3a,n3b, ca,cb, 
     &    h21, d22,d12, h22, d14, d24, h41, h42, 
     &    rsxy, u,s, deriv, 
     &    derivative, gridType, order, averagingType, dir1, dir2 )
c ===============================================================
c    divScalarGrad -- non-conservative form
c  
c ca,cb : assign components c=ca,..,cb (base 0)
c derivative : 0=laplace, 1=divScalarGrad, 2=derivativeScalarDerivative
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
     &  derivative, gridType, order, averagingType, dir1, dir2

      integer laplace,divScalarGrad,derivativeScalarDerivative
      parameter(laplace=0,divScalarGrad=1,derivativeScalarDerivative=2)

      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,1:nd,1:nd)
      real u(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,ndu4a:ndu4b)
      real s(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b)
      real deriv(ndd1a:ndd1b,ndd2a:ndd2b,ndd3a:ndd3b,ndd4a:ndd4b)
      real h21(*), d22(*),d12(*),h22(*)
      real d24(*),d14(*),h42(*),h41(*)

      real rx,ry,rz,sx,sy,sz,tx,ty,tz
      real urr2,uss2,utt2,urs2,urt2,ust2,ur2,us2,ut2
      real rxx2
      real rxx23,sxx23,txx23
      real rxr2,rxs2,rxt2,sxr2,sxs2,sxt2,txr2,txs2,txt2
      real sxx2,urr,urs,uss,rxx,ur,sxx,us,utt,urt,ust,rxx3,sxx3,txx3
      real rxr,rxs,sxr,sxs,rxt,sxt,txr,txs,txt,ut
      real LAPLACIAN21R, LAPLACIAN21, LAPLACIAN41R, LAPLACIAN41
      real LAPLACIAN22R, LAPLACIAN22, LAPLACIAN42R, LAPLACIAN42
      real LAPLACIAN23R, LAPLACIAN23, LAPLACIAN43R, LAPLACIAN43
      real UX21R, UX21, UX41R, UX41
      real UX22R, UX22, UX42R, UX42
      real UX23R, UX23, UX43R, UX43

      real UY21R, UY21, UY41R, UY41
      real UY22R, UY22, UY42R, UY42
      real UY23R, UY23, UY43R, UY43

      real UZ21R, UZ21, UZ41R, UZ41
      real UZ22R, UZ22, UZ42R, UZ42
      real UZ23R, UZ23, UZ43R, UZ43

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


      sr2(i1,i2,i3)=(s(i1+1,i2,i3)-s(i1-1,i2,i3))*d12(1)
      ss2(i1,i2,i3)=(s(i1,i2+1,i3)-s(i1,i2-1,i3))*d12(2)
      st2(i1,i2,i3)=(s(i1,i2,i3+1)-s(i1,i2,i3-1))*d12(3)

      sx21(i1,i2,i3)= rx(i1,i2,i3)*sr2(i1,i2,i3)

      sx22(i1,i2,i3)= rx(i1,i2,i3)*sr2(i1,i2,i3)
     &                 +sx(i1,i2,i3)*ss2(i1,i2,i3)
      sy22(i1,i2,i3)= ry(i1,i2,i3)*sr2(i1,i2,i3)
     &                 +sy(i1,i2,i3)*ss2(i1,i2,i3)
      sx23(i1,i2,i3)=rx(i1,i2,i3)*sr2(i1,i2,i3)
     &                 +sx(i1,i2,i3)*ss2(i1,i2,i3)
     &                 +tx(i1,i2,i3)*st2(i1,i2,i3)
      sy23(i1,i2,i3)=ry(i1,i2,i3)*sr2(i1,i2,i3)
     &                 +sy(i1,i2,i3)*ss2(i1,i2,i3)
     &                 +ty(i1,i2,i3)*st2(i1,i2,i3)
      sz23(i1,i2,i3)=rz(i1,i2,i3)*sr2(i1,i2,i3)
     &                 +sz(i1,i2,i3)*ss2(i1,i2,i3)
     &                 +tz(i1,i2,i3)*st2(i1,i2,i3)

      sx22r(i1,i2,i3)=(s(i1+1,i2,i3)-s(i1-1,i2,i3))*h21(1)
      sy22r(i1,i2,i3)=(s(i1,i2+1,i3)-s(i1,i2-1,i3))*h21(2)
      sz22r(i1,i2,i3)=(s(i1,i2,i3+1)-s(i1,i2,i3-1))*h21(3)

      sr(i1,i2,i3)=(8.*(s(i1+1,i2,i3)-s(i1-1,i2,i3))
     &                   -(s(i1+2,i2,i3)-s(i1-2,i2,i3)))*d14(1)
      ss(i1,i2,i3)=(8.*(s(i1,i2+1,i3)-s(i1,i2-1,i3))
     &                   -(s(i1,i2+2,i3)-s(i1,i2-2,i3)))*d14(2)
      st(i1,i2,i3)=(8.*(s(i1,i2,i3+1)-s(i1,i2,i3-1))
     &                   -(s(i1,i2,i3+2)-s(i1,i2,i3-2)))*d14(3)

      sx41(i1,i2,i3)= rx(i1,i2,i3)*sr(i1,i2,i3)

      sx42(i1,i2,i3)= rx(i1,i2,i3)*sr(i1,i2,i3)
     &                +sx(i1,i2,i3)*ss(i1,i2,i3)
      sy42(i1,i2,i3)= ry(i1,i2,i3)*sr(i1,i2,i3)
     &                +sy(i1,i2,i3)*ss(i1,i2,i3)
      sx43(i1,i2,i3)=rx(i1,i2,i3)*sr(i1,i2,i3)
     &                +sx(i1,i2,i3)*ss(i1,i2,i3)
     &                +tx(i1,i2,i3)*st(i1,i2,i3)
      sy43(i1,i2,i3)=ry(i1,i2,i3)*sr(i1,i2,i3)
     &                +sy(i1,i2,i3)*ss(i1,i2,i3)
     &                +ty(i1,i2,i3)*st(i1,i2,i3)
      sz43(i1,i2,i3)=rz(i1,i2,i3)*sr(i1,i2,i3)
     &                +sz(i1,i2,i3)*ss(i1,i2,i3)
     &                +tz(i1,i2,i3)*st(i1,i2,i3)

      sx42r(i1,i2,i3)=(8.*(s(i1+1,i2,i3)-s(i1-1,i2,i3))  
     &               -(s(i1+2,i2,i3)-s(i1-2,i2,i3)))*h41(1)
      sy42r(i1,i2,i3)=(8.*(s(i1,i2+1,i3)-s(i1,i2-1,i3))  
     &            -(s(i1,i2+2,i3)-s(i1,i2-2,i3)))*h41(2) 
      sz42r(i1,i2,i3)=(8.*(s(i1,i2,i3+1)-s(i1,i2,i3-1)) 
     &              -(s(i1,i2,i3+2)-s(i1,i2,i3-2)))*h41(3) 

c......end statement function

      kd3=nd

      if( derivative.eq.divScalarGrad )then
c       ****** divScalarGrad ******
        if( nd .eq. 2 )then
c         ************************
c         ******* 2D *************      
c         ************************

          if( gridType .eq. 0 )then
c     rectangular
            if( order.eq.2 )then
              do c=ca,cb
                do i3=n3a,n3b
                  do i2=n2a,n2b
                    do i1=n1a,n1b
                      deriv(i1,i2,i3,c)=
     &                     s(i1,i2,i3)*LAPLACIAN22R(i1,i2,i3,c)
     &                     +SX22R(i1,i2,i3)*UX22R(i1,i2,i3,c)
     &                     +SY22R(i1,i2,i3)*UY22R(i1,i2,i3,c)
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
     &                     s(i1,i2,i3)*LAPLACIAN42R(i1,i2,i3,c)
     &                     +SX42R(i1,i2,i3)*UX42R(i1,i2,i3,c)
     &                     +SY42R(i1,i2,i3)*UY42R(i1,i2,i3,c)
                    end do
                  end do
                end do
              end do

            end if

          else
c           ***** not rectangular *****
            if( order.eq.2 )then
              do c=ca,cb
                do i3=n3a,n3b
                  do i2=n2a,n2b
                    do i1=n1a,n1b
                      deriv(i1,i2,i3,c)=
     &                     s(i1,i2,i3)*LAPLACIAN22(i1,i2,i3,c)
     &                     +SX22(i1,i2,i3)*UX22(i1,i2,i3,c)
     &                     +SY22(i1,i2,i3)*UY22(i1,i2,i3,c)
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
     &                     s(i1,i2,i3)*LAPLACIAN42(i1,i2,i3,c)
     &                     +SX42(i1,i2,i3)*UX42(i1,i2,i3,c)
     &                     +SY42(i1,i2,i3)*UY42(i1,i2,i3,c)
                    end do
                  end do
                end do
              end do

            end if

          endif 
        elseif( nd.eq.3 )then
c         ************************
c         ******* 3D *************      
c         ************************
          if( gridType .eq. 0 )then
c           rectangular
            if( order.eq.2 )then
              do c=ca,cb
                do i3=n3a,n3b
                  do i2=n2a,n2b
                    do i1=n1a,n1b
                      deriv(i1,i2,i3,c)=
     &                     s(i1,i2,i3)*LAPLACIAN23R(i1,i2,i3,c)
     &                     +SX22R(i1,i2,i3)*UX23R(i1,i2,i3,c)
     &                     +SY22R(i1,i2,i3)*UY23R(i1,i2,i3,c)
     &                     +SZ22R(i1,i2,i3)*UZ23R(i1,i2,i3,c)
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
     &                     s(i1,i2,i3)*LAPLACIAN43R(i1,i2,i3,c)
     &                     +SX42R(i1,i2,i3)*UX42R(i1,i2,i3,c)
     &                     +SY42R(i1,i2,i3)*UY42R(i1,i2,i3,c)
     &                     +SZ42R(i1,i2,i3)*UZ42R(i1,i2,i3,c)
                    end do
                  end do
                end do
              end do

            end if

          else
c           ***** not rectangular *****
            if( order.eq.2 )then
              do c=ca,cb
                do i3=n3a,n3b
                  do i2=n2a,n2b
                    do i1=n1a,n1b
                      deriv(i1,i2,i3,c)=
     &                     s(i1,i2,i3)*LAPLACIAN23(i1,i2,i3,c)
     &                     +SX23(i1,i2,i3)*UX23(i1,i2,i3,c)
     &                     +SY23(i1,i2,i3)*UY23(i1,i2,i3,c)
     &                     +SZ23(i1,i2,i3)*UZ23(i1,i2,i3,c)
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
     &                     s(i1,i2,i3)*LAPLACIAN43(i1,i2,i3,c)
     &                     +SX43(i1,i2,i3)*UX43(i1,i2,i3,c)
     &                     +SY43(i1,i2,i3)*UY43(i1,i2,i3,c)
     &                     +SZ43(i1,i2,i3)*UZ43(i1,i2,i3,c)
                    end do
                  end do
                end do
              end do

            end if

          endif 
          

        else
c         ************************
c         ******* 1D *************      
c         ************************

          if( gridType .eq. 0 )then
c           rectangular
            if( order.eq.2 )then
              do c=ca,cb
                do i3=n3a,n3b
                  do i2=n2a,n2b
                    do i1=n1a,n1b
                      deriv(i1,i2,i3,c)=
     &                     s(i1,i2,i3)*LAPLACIAN21R(i1,i2,i3,c)
     &                     +SX22R(i1,i2,i3)*UX22R(i1,i2,i3,c)
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
     &                     s(i1,i2,i3)*LAPLACIAN41R(i1,i2,i3,c)
     &                     +SX42R(i1,i2,i3)*UX42R(i1,i2,i3,c)
                    end do
                  end do
                end do
              end do

            end if

          else
c            ***** not rectangular *****
            if( order.eq.2 )then
              do c=ca,cb
                do i3=n3a,n3b
                  do i2=n2a,n2b
                    do i1=n1a,n1b
                      deriv(i1,i2,i3,c)=
     &                     s(i1,i2,i3)*LAPLACIAN21(i1,i2,i3,c)
     &                     +SX21(i1,i2,i3)*UX21(i1,i2,i3,c)
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
     &                     s(i1,i2,i3)*LAPLACIAN41(i1,i2,i3,c)
     &                     +SX41(i1,i2,i3)*UX41(i1,i2,i3,c)
                    end do
                  end do
                end do
              end do

            end if

          endif 
        end if

      elseif( derivative.eq.laplace )then

c       ****** laplace ******
        if( nd .eq. 2 )then
c         ************************
c         ******* 2D *************      
c         ************************

          if( gridType .eq. 0 )then
c     rectangular
            if( order.eq.2 )then
              do c=ca,cb
                do i3=n3a,n3b
                  do i2=n2a,n2b
                    do i1=n1a,n1b
                      deriv(i1,i2,i3,c)=LAPLACIAN22R(i1,i2,i3,c)
                    end do
                  end do
                end do
              end do
            else
              do c=ca,cb
                do i3=n3a,n3b
                  do i2=n2a,n2b
                    do i1=n1a,n1b
                      deriv(i1,i2,i3,c)=LAPLACIAN42R(i1,i2,i3,c)
                    end do
                  end do
                end do
              end do

            end if

          else
c           ***** not rectangular *****
            if( order.eq.2 )then
              do c=ca,cb
                do i3=n3a,n3b
                  do i2=n2a,n2b
                    do i1=n1a,n1b
                      deriv(i1,i2,i3,c)=LAPLACIAN22(i1,i2,i3,c)
                    end do
                  end do
                end do
              end do
            else
              do c=ca,cb
                do i3=n3a,n3b
                  do i2=n2a,n2b
                    do i1=n1a,n1b
                      deriv(i1,i2,i3,c)=LAPLACIAN42(i1,i2,i3,c)
                    end do
                  end do
                end do
              end do

            end if

          endif 
        elseif( nd.eq.3 )then
c         ************************
c         ******* 3D *************      
c         ************************
          if( gridType .eq. 0 )then
c           rectangular
            if( order.eq.2 )then
              do c=ca,cb
                do i3=n3a,n3b
                  do i2=n2a,n2b
                    do i1=n1a,n1b
                      deriv(i1,i2,i3,c)=LAPLACIAN23R(i1,i2,i3,c)
                    end do
                  end do
                end do
              end do
            else
              do c=ca,cb
                do i3=n3a,n3b
                  do i2=n2a,n2b
                    do i1=n1a,n1b
                      deriv(i1,i2,i3,c)=LAPLACIAN43R(i1,i2,i3,c)
                    end do
                  end do
                end do
              end do

            end if

          else
c           ***** not rectangular *****
            if( order.eq.2 )then
              do c=ca,cb
                do i3=n3a,n3b
                  do i2=n2a,n2b
                    do i1=n1a,n1b
                      deriv(i1,i2,i3,c)=LAPLACIAN23(i1,i2,i3,c)
                    end do
                  end do
                end do
              end do
            else
              do c=ca,cb
                do i3=n3a,n3b
                  do i2=n2a,n2b
                    do i1=n1a,n1b
                      deriv(i1,i2,i3,c)=LAPLACIAN43(i1,i2,i3,c)
                    end do
                  end do
                end do
              end do

            end if

          endif 
          

        else
c         ************************
c         ******* 1D *************      
c         ************************

          if( gridType .eq. 0 )then
c           rectangular
            if( order.eq.2 )then
              do c=ca,cb
                do i3=n3a,n3b
                  do i2=n2a,n2b
                    do i1=n1a,n1b
                      deriv(i1,i2,i3,c)=LAPLACIAN21R(i1,i2,i3,c)
                    end do
                  end do
                end do
              end do
            else
              do c=ca,cb
                do i3=n3a,n3b
                  do i2=n2a,n2b
                    do i1=n1a,n1b
                      deriv(i1,i2,i3,c)=LAPLACIAN41R(i1,i2,i3,c)
                    end do
                  end do
                end do
              end do

            end if

          else
c            ***** not rectangular *****
            if( order.eq.2 )then
              do c=ca,cb
                do i3=n3a,n3b
                  do i2=n2a,n2b
                    do i1=n1a,n1b
                      deriv(i1,i2,i3,c)=LAPLACIAN21(i1,i2,i3,c)
                    end do
                  end do
                end do
              end do
            else
              do c=ca,cb
                do i3=n3a,n3b
                  do i2=n2a,n2b
                    do i1=n1a,n1b
                      deriv(i1,i2,i3,c)=LAPLACIAN41(i1,i2,i3,c)
                    end do
                  end do
                end do
              end do

            end if

          endif 
        end if

      else

c       ****** derivativeScalarDerivative ******
        if( nd .eq. 2 )then
c         ************************
c         ******* 2D *************      
c         ************************

          if( gridType .eq. 0 )then
c           rectangular   ******************** finish this ***********
c       
c           we need   Dx_dir1 ( s Dx_dir2 )
c
c           ---> write a script to do all the cases

            if( order.eq.2 )then
              do c=ca,cb
                do i3=n3a,n3b
                  do i2=n2a,n2b
                    do i1=n1a,n1b
                      deriv(i1,i2,i3,c)=
     &                     s(i1,i2,i3)*LAPLACIAN22R(i1,i2,i3,c)
     &                     +SX22R(i1,i2,i3)*UX22R(i1,i2,i3,c)
     &                     +SY22R(i1,i2,i3)*UY22R(i1,i2,i3,c)
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
     &                     s(i1,i2,i3)*LAPLACIAN42R(i1,i2,i3,c)
     &                     +SX42R(i1,i2,i3)*UX42R(i1,i2,i3,c)
     &                     +SY42R(i1,i2,i3)*UY42R(i1,i2,i3,c)
                    end do
                  end do
                end do
              end do

            end if

          else
c           ***** not rectangular *****
            if( order.eq.2 )then
              do c=ca,cb
                do i3=n3a,n3b
                  do i2=n2a,n2b
                    do i1=n1a,n1b
                      deriv(i1,i2,i3,c)=
     &                     s(i1,i2,i3)*LAPLACIAN22(i1,i2,i3,c)
     &                     +SX22(i1,i2,i3)*UX22(i1,i2,i3,c)
     &                     +SY22(i1,i2,i3)*UY22(i1,i2,i3,c)
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
     &                     s(i1,i2,i3)*LAPLACIAN42(i1,i2,i3,c)
     &                     +SX42(i1,i2,i3)*UX42(i1,i2,i3,c)
     &                     +SY42(i1,i2,i3)*UY42(i1,i2,i3,c)
                    end do
                  end do
                end do
              end do

            end if

          endif 
        elseif( nd.eq.3 )then
c         ************************
c         ******* 3D *************      
c         ************************
          if( gridType .eq. 0 )then
c           rectangular
            if( order.eq.2 )then
              do c=ca,cb
                do i3=n3a,n3b
                  do i2=n2a,n2b
                    do i1=n1a,n1b
                      deriv(i1,i2,i3,c)=
     &                     s(i1,i2,i3)*LAPLACIAN23R(i1,i2,i3,c)
     &                     +SX22R(i1,i2,i3)*UX23R(i1,i2,i3,c)
     &                     +SY22R(i1,i2,i3)*UY23R(i1,i2,i3,c)
     &                     +SZ22R(i1,i2,i3)*UZ23R(i1,i2,i3,c)
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
     &                     s(i1,i2,i3)*LAPLACIAN43R(i1,i2,i3,c)
     &                     +SX42R(i1,i2,i3)*UX42R(i1,i2,i3,c)
     &                     +SY42R(i1,i2,i3)*UY42R(i1,i2,i3,c)
     &                     +SZ42R(i1,i2,i3)*UZ42R(i1,i2,i3,c)
                    end do
                  end do
                end do
              end do

            end if

          else
c           ***** not rectangular *****
            if( order.eq.2 )then
              do c=ca,cb
                do i3=n3a,n3b
                  do i2=n2a,n2b
                    do i1=n1a,n1b
                      deriv(i1,i2,i3,c)=
     &                     s(i1,i2,i3)*LAPLACIAN23(i1,i2,i3,c)
     &                     +SX23(i1,i2,i3)*UX23(i1,i2,i3,c)
     &                     +SY23(i1,i2,i3)*UY23(i1,i2,i3,c)
     &                     +SZ23(i1,i2,i3)*UZ23(i1,i2,i3,c)
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
     &                     s(i1,i2,i3)*LAPLACIAN43(i1,i2,i3,c)
     &                     +SX43(i1,i2,i3)*UX43(i1,i2,i3,c)
     &                     +SY43(i1,i2,i3)*UY43(i1,i2,i3,c)
     &                     +SZ43(i1,i2,i3)*UZ43(i1,i2,i3,c)
                    end do
                  end do
                end do
              end do

            end if

          endif 
          

        else if( nd.eq.1 )then
c         ************************
c         ******* 1D *************      
c         ************************

          if( gridType .eq. 0 )then
c           rectangular
            if( order.eq.2 )then
              do c=ca,cb
                do i3=n3a,n3b
                  do i2=n2a,n2b
                    do i1=n1a,n1b
                      deriv(i1,i2,i3,c)=
     &                     s(i1,i2,i3)*LAPLACIAN21R(i1,i2,i3,c)
     &                     +SX22R(i1,i2,i3)*UX22R(i1,i2,i3,c)
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
     &                     s(i1,i2,i3)*LAPLACIAN41R(i1,i2,i3,c)
     &                     +SX42R(i1,i2,i3)*UX42R(i1,i2,i3,c)
                    end do
                  end do
                end do
              end do

            end if

          else
c            ***** not rectangular *****
            if( order.eq.2 )then
              do c=ca,cb
                do i3=n3a,n3b
                  do i2=n2a,n2b
                    do i1=n1a,n1b
                      deriv(i1,i2,i3,c)=
     &                     s(i1,i2,i3)*LAPLACIAN21(i1,i2,i3,c)
     &                     +SX21(i1,i2,i3)*UX21(i1,i2,i3,c)
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
     &                     s(i1,i2,i3)*LAPLACIAN41(i1,i2,i3,c)
     &                     +SX41(i1,i2,i3)*UX41(i1,i2,i3,c)
                    end do
                  end do
                end do
              end do

            end if

          endif 
        end if
      end if


      if( nd.eq.0 )then
c       *** add these lines to avoid warnings about unused statement functions
        include "cgux2afNoWarnings.h" 
        include "cgux4afNoWarnings.h" 

      end if

      return
      end
