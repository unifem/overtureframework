! This file automatically generated from dsg.bf with bpp.
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
      integer laplace,divScalarGrad,derivativeScalarDerivative,
     & divTensorGrad
      parameter(laplace=0,divScalarGrad=1,derivativeScalarDerivative=2,
     & divTensorGrad=3)
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

! defineA23()
      m1a=n1a-1
      m1b=n1b+1
      m2a=n2a-1
      m2b=n2b+1
      m3a=n3a-1
      m3b=n3b+1
      if( averagingType .eq. arithmeticAverage )then
        factor=.5
! GETA23(s(j1,j2,j3,0)*jac(j1,j2,j3),c,factor)
        if( derivOption.eq.laplace )then
          do j3=m3a,m3b
            do j2=m2a,m2b
              do j1=m1a,m1b
                sj = jac(j1,j2,j3)
                a11(j1,j2,j3) = (rx(j1,j2,j3)**2+ry(j1,j2,j3)**2+rz(j1,
     & j2,j3)**2)*sj
                a12(j1,j2,j3) = (rx(j1,j2,j3)*sx(j1,j2,j3)+ry(j1,j2,j3)
     & *sy(j1,j2,j3)+rz(j1,j2,j3)*sz(j1,j2,j3))*sj
                a13(j1,j2,j3) = (rx(j1,j2,j3)*tx(j1,j2,j3)+ry(j1,j2,j3)
     & *ty(j1,j2,j3)+rz(j1,j2,j3)*tz(j1,j2,j3))*sj
                a22(j1,j2,j3) = (sx(j1,j2,j3)**2+sy(j1,j2,j3)**2+sz(j1,
     & j2,j3)**2)*sj
                a23(j1,j2,j3) = (sx(j1,j2,j3)*tx(j1,j2,j3)+sy(j1,j2,j3)
     & *ty(j1,j2,j3)+sz(j1,j2,j3)*tz(j1,j2,j3))*sj
                a33(j1,j2,j3) = (tx(j1,j2,j3)**2+ty(j1,j2,j3)**2+tz(j1,
     & j2,j3)**2)*sj
                a21(j1,j2,j3) = a12(j1,j2,j3)
                a31(j1,j2,j3) = a13(j1,j2,j3)
                a32(j1,j2,j3) = a23(j1,j2,j3)
              end do
            end do
          end do
        else if( derivOption.eq.divScalarGrad )then
          do j3=m3a,m3b
            do j2=m2a,m2b
              do j1=m1a,m1b
                sj = s(j1,j2,j3,0)*jac(j1,j2,j3)
                a11(j1,j2,j3) = (rx(j1,j2,j3)**2+ry(j1,j2,j3)**2+rz(j1,
     & j2,j3)**2)*sj
                a12(j1,j2,j3) = (rx(j1,j2,j3)*sx(j1,j2,j3)+ry(j1,j2,j3)
     & *sy(j1,j2,j3)+rz(j1,j2,j3)*sz(j1,j2,j3))*sj
                a13(j1,j2,j3) = (rx(j1,j2,j3)*tx(j1,j2,j3)+ry(j1,j2,j3)
     & *ty(j1,j2,j3)+rz(j1,j2,j3)*tz(j1,j2,j3))*sj
                a22(j1,j2,j3) = (sx(j1,j2,j3)**2+sy(j1,j2,j3)**2+sz(j1,
     & j2,j3)**2)*sj
                a23(j1,j2,j3) = (sx(j1,j2,j3)*tx(j1,j2,j3)+sy(j1,j2,j3)
     & *ty(j1,j2,j3)+sz(j1,j2,j3)*tz(j1,j2,j3))*sj
                a33(j1,j2,j3) = (tx(j1,j2,j3)**2+ty(j1,j2,j3)**2+tz(j1,
     & j2,j3)**2)*sj
                a21(j1,j2,j3) = a12(j1,j2,j3)
                a31(j1,j2,j3) = a13(j1,j2,j3)
                a32(j1,j2,j3) = a23(j1,j2,j3)
              end do
            end do
          end do
        else if( derivOption.eq.divTensorGrad )then
          do j3=m3a,m3b
            do j2=m2a,m2b
              do j1=m1a,m1b
                sj =jac(j1,j2,j3)
                s11=s(j1,j2,j3,0)
                s21=s(j1,j2,j3,1)
                s31=s(j1,j2,j3,2)
                s12=s(j1,j2,j3,3)
                s22=s(j1,j2,j3,4)
                s32=s(j1,j2,j3,5)
                s13=s(j1,j2,j3,6)
                s23=s(j1,j2,j3,7)
                s33=s(j1,j2,j3,8)
                rxj=rx(j1,j2,j3)
                ryj=ry(j1,j2,j3)
                rzj=rz(j1,j2,j3)
                sxj=sx(j1,j2,j3)
                syj=sy(j1,j2,j3)
                szj=sz(j1,j2,j3)
                txj=tx(j1,j2,j3)
                tyj=ty(j1,j2,j3)
                tzj=tz(j1,j2,j3)
                a11(j1,j2,j3) = (s11*rxj**2+s22*ryj**2+s33*rzj**2+(s12+
     & s21)*rxj*ryj+(s13+s31)*rxj*rzj+(s23+s32)*ryj*rzj)*sj
                a22(j1,j2,j3) = (s11*sxj**2+s22*syj**2+s33*szj**2+(s12+
     & s21)*sxj*syj+(s13+s31)*sxj*szj+(s23+s32)*syj*szj)*sj
                a33(j1,j2,j3) = (s11*txj**2+s22*tyj**2+s33*tzj**2+(s12+
     & s21)*txj*tyj+(s13+s31)*txj*tzj+(s23+s32)*tyj*tzj)*sj
                a12(j1,j2,j3) = (s11*rxj*sxj+s22*ryj*syj+s33*rzj*szj+
     & s12*rxj*syj+s13*rxj*szj+s21*ryj*sxj+s23*ryj*szj+s31*rzj*sxj+
     & s32*rzj*syj)*sj
                a13(j1,j2,j3) = (s11*rxj*txj+s22*ryj*tyj+s33*rzj*tzj+
     & s12*rxj*tyj+s13*rxj*tzj+s21*ryj*txj+s23*ryj*tzj+s31*rzj*txj+
     & s32*rzj*tyj)*sj
                a23(j1,j2,j3) = (s11*sxj*txj+s22*syj*tyj+s33*szj*tzj+
     & s12*sxj*tyj+s13*sxj*tzj+s21*syj*txj+s23*syj*tzj+s31*szj*txj+
     & s32*szj*tyj)*sj
                a21(j1,j2,j3) = (s11*sxj*rxj+s22*syj*ryj+s33*szj*rzj+
     & s12*sxj*ryj+s13*sxj*rzj+s21*syj*rxj+s23*syj*rzj+s31*szj*rxj+
     & s32*szj*ryj)*sj
                a31(j1,j2,j3) = (s11*txj*rxj+s22*tyj*ryj+s33*tzj*rzj+
     & s12*txj*ryj+s13*txj*rzj+s21*tyj*rxj+s23*tyj*rzj+s31*tzj*rxj+
     & s32*tzj*ryj)*sj
                a32(j1,j2,j3) = (s11*txj*sxj+s22*tyj*syj+s33*tzj*szj+
     & s12*txj*syj+s13*txj*szj+s21*tyj*sxj+s23*tyj*szj+s31*tzj*sxj+
     & s32*tzj*syj)*sj
              end do
            end do
          end do
        else if( derivOption.eq.derivativeScalarDerivative )then
          if(      dir1.eq.0 .and. dir2.eq.0 )then
! DXSDY23(x,x,s(j1,j2,j3,0)*jac(j1,j2,j3))
            do j3=m3a,m3b
              do j2=m2a,m2b
                do j1=m1a,m1b
                  sj = s(j1,j2,j3,0)*jac(j1,j2,j3)
                  a11(j1,j2,j3) = (r x (j1,j2,j3)*r x (j1,j2,j3))*sj
                  a12(j1,j2,j3) = (r x (j1,j2,j3)*s x (j1,j2,j3))*sj
                  a13(j1,j2,j3) = (r x (j1,j2,j3)*t x (j1,j2,j3))*sj
                  a22(j1,j2,j3) = (s x (j1,j2,j3)*s x (j1,j2,j3))*sj
                  a23(j1,j2,j3) = (s x (j1,j2,j3)*t x (j1,j2,j3))*sj
                  a33(j1,j2,j3) = (t x (j1,j2,j3)*t x (j1,j2,j3))*sj
! #If "x" == "x"
                  a21(j1,j2,j3) = a12(j1,j2,j3)
                  a31(j1,j2,j3) = a13(j1,j2,j3)
                  a32(j1,j2,j3) = a23(j1,j2,j3)
                end do
              end do
            end do
          else if( dir1.eq.0 .and. dir2.eq.1 )then
! DXSDY23(x,y,s(j1,j2,j3,0)*jac(j1,j2,j3))
            do j3=m3a,m3b
              do j2=m2a,m2b
                do j1=m1a,m1b
                  sj = s(j1,j2,j3,0)*jac(j1,j2,j3)
                  a11(j1,j2,j3) = (r x (j1,j2,j3)*r y (j1,j2,j3))*sj
                  a12(j1,j2,j3) = (r x (j1,j2,j3)*s y (j1,j2,j3))*sj
                  a13(j1,j2,j3) = (r x (j1,j2,j3)*t y (j1,j2,j3))*sj
                  a22(j1,j2,j3) = (s x (j1,j2,j3)*s y (j1,j2,j3))*sj
                  a23(j1,j2,j3) = (s x (j1,j2,j3)*t y (j1,j2,j3))*sj
                  a33(j1,j2,j3) = (t x (j1,j2,j3)*t y (j1,j2,j3))*sj
! #If "x" == "y"
! #Else
                  a21(j1,j2,j3) = (s x (j1,j2,j3)*r y (j1,j2,j3))*sj
                  a31(j1,j2,j3) = (t x (j1,j2,j3)*r y (j1,j2,j3))*sj
                  a32(j1,j2,j3) = (t x (j1,j2,j3)*s y (j1,j2,j3))*sj
                end do
              end do
            end do
          else if( dir1.eq.0 .and. dir2.eq.2 )then
! DXSDY23(x,z,s(j1,j2,j3,0)*jac(j1,j2,j3))
            do j3=m3a,m3b
              do j2=m2a,m2b
                do j1=m1a,m1b
                  sj = s(j1,j2,j3,0)*jac(j1,j2,j3)
                  a11(j1,j2,j3) = (r x (j1,j2,j3)*r z (j1,j2,j3))*sj
                  a12(j1,j2,j3) = (r x (j1,j2,j3)*s z (j1,j2,j3))*sj
                  a13(j1,j2,j3) = (r x (j1,j2,j3)*t z (j1,j2,j3))*sj
                  a22(j1,j2,j3) = (s x (j1,j2,j3)*s z (j1,j2,j3))*sj
                  a23(j1,j2,j3) = (s x (j1,j2,j3)*t z (j1,j2,j3))*sj
                  a33(j1,j2,j3) = (t x (j1,j2,j3)*t z (j1,j2,j3))*sj
! #If "x" == "z"
! #Else
                  a21(j1,j2,j3) = (s x (j1,j2,j3)*r z (j1,j2,j3))*sj
                  a31(j1,j2,j3) = (t x (j1,j2,j3)*r z (j1,j2,j3))*sj
                  a32(j1,j2,j3) = (t x (j1,j2,j3)*s z (j1,j2,j3))*sj
                end do
              end do
            end do
          else if( dir1.eq.1 .and. dir2.eq.0 )then
! DXSDY23(y,x,s(j1,j2,j3,0)*jac(j1,j2,j3))
            do j3=m3a,m3b
              do j2=m2a,m2b
                do j1=m1a,m1b
                  sj = s(j1,j2,j3,0)*jac(j1,j2,j3)
                  a11(j1,j2,j3) = (r y (j1,j2,j3)*r x (j1,j2,j3))*sj
                  a12(j1,j2,j3) = (r y (j1,j2,j3)*s x (j1,j2,j3))*sj
                  a13(j1,j2,j3) = (r y (j1,j2,j3)*t x (j1,j2,j3))*sj
                  a22(j1,j2,j3) = (s y (j1,j2,j3)*s x (j1,j2,j3))*sj
                  a23(j1,j2,j3) = (s y (j1,j2,j3)*t x (j1,j2,j3))*sj
                  a33(j1,j2,j3) = (t y (j1,j2,j3)*t x (j1,j2,j3))*sj
! #If "y" == "x"
! #Else
                  a21(j1,j2,j3) = (s y (j1,j2,j3)*r x (j1,j2,j3))*sj
                  a31(j1,j2,j3) = (t y (j1,j2,j3)*r x (j1,j2,j3))*sj
                  a32(j1,j2,j3) = (t y (j1,j2,j3)*s x (j1,j2,j3))*sj
                end do
              end do
            end do
          else if( dir1.eq.1 .and. dir2.eq.1 )then
! DXSDY23(y,y,s(j1,j2,j3,0)*jac(j1,j2,j3))
            do j3=m3a,m3b
              do j2=m2a,m2b
                do j1=m1a,m1b
                  sj = s(j1,j2,j3,0)*jac(j1,j2,j3)
                  a11(j1,j2,j3) = (r y (j1,j2,j3)*r y (j1,j2,j3))*sj
                  a12(j1,j2,j3) = (r y (j1,j2,j3)*s y (j1,j2,j3))*sj
                  a13(j1,j2,j3) = (r y (j1,j2,j3)*t y (j1,j2,j3))*sj
                  a22(j1,j2,j3) = (s y (j1,j2,j3)*s y (j1,j2,j3))*sj
                  a23(j1,j2,j3) = (s y (j1,j2,j3)*t y (j1,j2,j3))*sj
                  a33(j1,j2,j3) = (t y (j1,j2,j3)*t y (j1,j2,j3))*sj
! #If "y" == "y"
                  a21(j1,j2,j3) = a12(j1,j2,j3)
                  a31(j1,j2,j3) = a13(j1,j2,j3)
                  a32(j1,j2,j3) = a23(j1,j2,j3)
                end do
              end do
            end do
          else if( dir1.eq.1 .and. dir2.eq.2 )then
! DXSDY23(y,z,s(j1,j2,j3,0)*jac(j1,j2,j3))
            do j3=m3a,m3b
              do j2=m2a,m2b
                do j1=m1a,m1b
                  sj = s(j1,j2,j3,0)*jac(j1,j2,j3)
                  a11(j1,j2,j3) = (r y (j1,j2,j3)*r z (j1,j2,j3))*sj
                  a12(j1,j2,j3) = (r y (j1,j2,j3)*s z (j1,j2,j3))*sj
                  a13(j1,j2,j3) = (r y (j1,j2,j3)*t z (j1,j2,j3))*sj
                  a22(j1,j2,j3) = (s y (j1,j2,j3)*s z (j1,j2,j3))*sj
                  a23(j1,j2,j3) = (s y (j1,j2,j3)*t z (j1,j2,j3))*sj
                  a33(j1,j2,j3) = (t y (j1,j2,j3)*t z (j1,j2,j3))*sj
! #If "y" == "z"
! #Else
                  a21(j1,j2,j3) = (s y (j1,j2,j3)*r z (j1,j2,j3))*sj
                  a31(j1,j2,j3) = (t y (j1,j2,j3)*r z (j1,j2,j3))*sj
                  a32(j1,j2,j3) = (t y (j1,j2,j3)*s z (j1,j2,j3))*sj
                end do
              end do
            end do
          else if( dir1.eq.2 .and. dir2.eq.0 )then
! DXSDY23(z,x,s(j1,j2,j3,0)*jac(j1,j2,j3))
            do j3=m3a,m3b
              do j2=m2a,m2b
                do j1=m1a,m1b
                  sj = s(j1,j2,j3,0)*jac(j1,j2,j3)
                  a11(j1,j2,j3) = (r z (j1,j2,j3)*r x (j1,j2,j3))*sj
                  a12(j1,j2,j3) = (r z (j1,j2,j3)*s x (j1,j2,j3))*sj
                  a13(j1,j2,j3) = (r z (j1,j2,j3)*t x (j1,j2,j3))*sj
                  a22(j1,j2,j3) = (s z (j1,j2,j3)*s x (j1,j2,j3))*sj
                  a23(j1,j2,j3) = (s z (j1,j2,j3)*t x (j1,j2,j3))*sj
                  a33(j1,j2,j3) = (t z (j1,j2,j3)*t x (j1,j2,j3))*sj
! #If "z" == "x"
! #Else
                  a21(j1,j2,j3) = (s z (j1,j2,j3)*r x (j1,j2,j3))*sj
                  a31(j1,j2,j3) = (t z (j1,j2,j3)*r x (j1,j2,j3))*sj
                  a32(j1,j2,j3) = (t z (j1,j2,j3)*s x (j1,j2,j3))*sj
                end do
              end do
            end do
          else if( dir1.eq.2 .and. dir2.eq.1 )then
! DXSDY23(z,y,s(j1,j2,j3,0)*jac(j1,j2,j3))
            do j3=m3a,m3b
              do j2=m2a,m2b
                do j1=m1a,m1b
                  sj = s(j1,j2,j3,0)*jac(j1,j2,j3)
                  a11(j1,j2,j3) = (r z (j1,j2,j3)*r y (j1,j2,j3))*sj
                  a12(j1,j2,j3) = (r z (j1,j2,j3)*s y (j1,j2,j3))*sj
                  a13(j1,j2,j3) = (r z (j1,j2,j3)*t y (j1,j2,j3))*sj
                  a22(j1,j2,j3) = (s z (j1,j2,j3)*s y (j1,j2,j3))*sj
                  a23(j1,j2,j3) = (s z (j1,j2,j3)*t y (j1,j2,j3))*sj
                  a33(j1,j2,j3) = (t z (j1,j2,j3)*t y (j1,j2,j3))*sj
! #If "z" == "y"
! #Else
                  a21(j1,j2,j3) = (s z (j1,j2,j3)*r y (j1,j2,j3))*sj
                  a31(j1,j2,j3) = (t z (j1,j2,j3)*r y (j1,j2,j3))*sj
                  a32(j1,j2,j3) = (t z (j1,j2,j3)*s y (j1,j2,j3))*sj
                end do
              end do
            end do
          else if( dir1.eq.2 .and. dir2.eq.2 )then
! DXSDY23(z,z,s(j1,j2,j3,0)*jac(j1,j2,j3))
            do j3=m3a,m3b
              do j2=m2a,m2b
                do j1=m1a,m1b
                  sj = s(j1,j2,j3,0)*jac(j1,j2,j3)
                  a11(j1,j2,j3) = (r z (j1,j2,j3)*r z (j1,j2,j3))*sj
                  a12(j1,j2,j3) = (r z (j1,j2,j3)*s z (j1,j2,j3))*sj
                  a13(j1,j2,j3) = (r z (j1,j2,j3)*t z (j1,j2,j3))*sj
                  a22(j1,j2,j3) = (s z (j1,j2,j3)*s z (j1,j2,j3))*sj
                  a23(j1,j2,j3) = (s z (j1,j2,j3)*t z (j1,j2,j3))*sj
                  a33(j1,j2,j3) = (t z (j1,j2,j3)*t z (j1,j2,j3))*sj
! #If "z" == "z"
                  a21(j1,j2,j3) = a12(j1,j2,j3)
                  a31(j1,j2,j3) = a13(j1,j2,j3)
                  a32(j1,j2,j3) = a23(j1,j2,j3)
                end do
              end do
            end do
          else
            write(*,*) 'ERROR invalid values: dir1=',dir1,' dir2=',dir2
          end if
        end if
        if( derivType.eq.symmetric )then
        ! symmetric case -- do not average a12, a21, a13, z31, etc.  -- could do better
         m1a=n1a
         do j3=m3a,m3b
          do j2=m2a,m2b
            do j1=m1b,m1a,-1 ! go backwards   worry about division by zero
c     sh=s(j1,j2,j3,0)*s(j1-1,j2,j3,0)/(s(j1,j2,j3,0)+s(j1-1,j2,j3,0))
              a11(j1,j2,j3) = factor *d22(1)*(a11(j1,j2,j3)+a11(j1-1,
     & j2,j3))
            end do
          end do
         end do
         m1a=n1a-1
         do j3=m3a,m3b
          do j2=m2a,m2b
            do j1=m1b,m1a,-1
! #If "c" eq "c"
              a12(j1,j2,j3) =         (d12(1)*d12(2))*a12(j1,j2,j3)
              a13(j1,j2,j3) =         (d12(1)*d12(3))*a13(j1,j2,j3)
            end do
          end do
         end do
         m2a=n2a
         do j3=m3a,m3b
          do j2=m2b,m2a,-1
            do j1=m1a,m1b
c     sh=s(j1,j2,j3,0)*s(j1,j2-1,j3,0)/(s(j1,j2,j3,0)+s(j1,j2-1,j3,0)) 
              a22(j1,j2,j3) = factor *d22(2)*(a22(j1,j2,j3)+a22(j1,j2-
     & 1,j3))
            end do
          end do
         end do
         m2a=n2a-1
         do j3=m3a,m3b
          do j2=m2b,m2a,-1
            do j1=m1a,m1b
! #If "c" eq "c"
              a21(j1,j2,j3) =         (d12(1)*d12(2))*a21(j1,j2,j3)
              a23(j1,j2,j3) =         (d12(2)*d12(3))*a23(j1,j2,j3)
            end do
          end do
         end do
         m3a=n3a
         do j3=m3b,m3a,-1
          do j2=m2a,m2b
            do j1=m1a,m1b
c     sh=s(j1,j2,j3,0)*s(j1,j2,j3-1,0)/(s(j1,j2,j3,0)+s(j1,j2,j3-1,0)) 
              a33(j1,j2,j3) = factor *d22(3)*(a33(j1,j2,j3)+a33(j1,j2,
     & j3-1))
            end do
          end do
         end do
         m3a=n3a-1
         do j3=m3b,m3a,-1
          do j2=m2a,m2b
            do j1=m1a,m1b
! #If "c" eq "c"
              a31(j1,j2,j3) =         (d12(1)*d12(3))*a31(j1,j2,j3)
              a32(j1,j2,j3) =         (d12(2)*d12(3))*a32(j1,j2,j3)
            end do
          end do
         end do
           else
         ! ** old way -- average all coefficients
        m1a=n1a
        do j3=m3a,m3b
          do j2=m2a,m2b
            do j1=m1b,m1a,-1 ! go backwards  worry about division by zero
c     sh=s(j1,j2,j3,0)*s(j1-1,j2,j3,0)/(s(j1,j2,j3,0)+s(j1-1,j2,j3,0)) 
              a11(j1,j2,j3) = factor *d22(1)*(a11(j1,j2,j3)+a11(j1-1,
     & j2,j3))
              a12(j1,j2,j3) = factor *(d12(1)*d12(2))*(a12(j1,j2,j3)+
     & a12(j1-1,j2,j3))
              a13(j1,j2,j3) = factor *(d12(1)*d12(3))*(a13(j1,j2,j3)+
     & a13(j1-1,j2,j3))
            end do
          end do
        end do
        m1a=n1a-1
        m2a=n2a
        do j3=m3a,m3b
          do j2=m2b,m2a,-1
            do j1=m1a,m1b
c     sh=s(j1,j2,j3,0)*s(j1,j2-1,j3,0)/(s(j1,j2,j3,0)+s(j1,j2-1,j3,0)) 
              a21(j1,j2,j3) = factor *(d12(1)*d12(2))*(a21(j1,j2,j3)+
     & a21(j1,j2-1,j3))
              a22(j1,j2,j3) = factor *d22(2)*(a22(j1,j2,j3)+a22(j1,j2-
     & 1,j3))
              a23(j1,j2,j3) = factor *(d12(2)*d12(3))*(a23(j1,j2,j3)+
     & a23(j1,j2-1,j3))
            end do
          end do
        end do
        m2a=n2a-1
        m3a=n3a
        do j3=m3b,m3a,-1
          do j2=m2a,m2b
            do j1=m1a,m1b
c     sh=s(j1,j2,j3,0)*s(j1,j2,j3-1,0)/(s(j1,j2,j3,0)+s(j1,j2,j3-1,0)) 
              a31(j1,j2,j3) = factor *(d12(1)*d12(3))*(a31(j1,j2,j3)+
     & a31(j1,j2,j3-1))
              a32(j1,j2,j3) = factor *(d12(2)*d12(3))*(a32(j1,j2,j3)+
     & a32(j1,j2,j3-1))
              a33(j1,j2,j3) = factor *d22(3)*(a33(j1,j2,j3)+a33(j1,j2,
     & j3-1))
            end do
          end do
        end do
        m3a=n3a-1
        end if
      else
c  Harmonic average
        factor=2.
c  do not average in s:  
! GETA23(jac(j1,j2,j3), ,sh)
        if( derivOption.eq.laplace )then
          do j3=m3a,m3b
            do j2=m2a,m2b
              do j1=m1a,m1b
                sj = jac(j1,j2,j3)
                a11(j1,j2,j3) = (rx(j1,j2,j3)**2+ry(j1,j2,j3)**2+rz(j1,
     & j2,j3)**2)*sj
                a12(j1,j2,j3) = (rx(j1,j2,j3)*sx(j1,j2,j3)+ry(j1,j2,j3)
     & *sy(j1,j2,j3)+rz(j1,j2,j3)*sz(j1,j2,j3))*sj
                a13(j1,j2,j3) = (rx(j1,j2,j3)*tx(j1,j2,j3)+ry(j1,j2,j3)
     & *ty(j1,j2,j3)+rz(j1,j2,j3)*tz(j1,j2,j3))*sj
                a22(j1,j2,j3) = (sx(j1,j2,j3)**2+sy(j1,j2,j3)**2+sz(j1,
     & j2,j3)**2)*sj
                a23(j1,j2,j3) = (sx(j1,j2,j3)*tx(j1,j2,j3)+sy(j1,j2,j3)
     & *ty(j1,j2,j3)+sz(j1,j2,j3)*tz(j1,j2,j3))*sj
                a33(j1,j2,j3) = (tx(j1,j2,j3)**2+ty(j1,j2,j3)**2+tz(j1,
     & j2,j3)**2)*sj
                a21(j1,j2,j3) = a12(j1,j2,j3)
                a31(j1,j2,j3) = a13(j1,j2,j3)
                a32(j1,j2,j3) = a23(j1,j2,j3)
              end do
            end do
          end do
        else if( derivOption.eq.divScalarGrad )then
          do j3=m3a,m3b
            do j2=m2a,m2b
              do j1=m1a,m1b
                sj = jac(j1,j2,j3)
                a11(j1,j2,j3) = (rx(j1,j2,j3)**2+ry(j1,j2,j3)**2+rz(j1,
     & j2,j3)**2)*sj
                a12(j1,j2,j3) = (rx(j1,j2,j3)*sx(j1,j2,j3)+ry(j1,j2,j3)
     & *sy(j1,j2,j3)+rz(j1,j2,j3)*sz(j1,j2,j3))*sj
                a13(j1,j2,j3) = (rx(j1,j2,j3)*tx(j1,j2,j3)+ry(j1,j2,j3)
     & *ty(j1,j2,j3)+rz(j1,j2,j3)*tz(j1,j2,j3))*sj
                a22(j1,j2,j3) = (sx(j1,j2,j3)**2+sy(j1,j2,j3)**2+sz(j1,
     & j2,j3)**2)*sj
                a23(j1,j2,j3) = (sx(j1,j2,j3)*tx(j1,j2,j3)+sy(j1,j2,j3)
     & *ty(j1,j2,j3)+sz(j1,j2,j3)*tz(j1,j2,j3))*sj
                a33(j1,j2,j3) = (tx(j1,j2,j3)**2+ty(j1,j2,j3)**2+tz(j1,
     & j2,j3)**2)*sj
                a21(j1,j2,j3) = a12(j1,j2,j3)
                a31(j1,j2,j3) = a13(j1,j2,j3)
                a32(j1,j2,j3) = a23(j1,j2,j3)
              end do
            end do
          end do
        else if( derivOption.eq.divTensorGrad )then
          do j3=m3a,m3b
            do j2=m2a,m2b
              do j1=m1a,m1b
                sj =jac(j1,j2,j3)
                s11=s(j1,j2,j3,0)
                s21=s(j1,j2,j3,1)
                s31=s(j1,j2,j3,2)
                s12=s(j1,j2,j3,3)
                s22=s(j1,j2,j3,4)
                s32=s(j1,j2,j3,5)
                s13=s(j1,j2,j3,6)
                s23=s(j1,j2,j3,7)
                s33=s(j1,j2,j3,8)
                rxj=rx(j1,j2,j3)
                ryj=ry(j1,j2,j3)
                rzj=rz(j1,j2,j3)
                sxj=sx(j1,j2,j3)
                syj=sy(j1,j2,j3)
                szj=sz(j1,j2,j3)
                txj=tx(j1,j2,j3)
                tyj=ty(j1,j2,j3)
                tzj=tz(j1,j2,j3)
                a11(j1,j2,j3) = (s11*rxj**2+s22*ryj**2+s33*rzj**2+(s12+
     & s21)*rxj*ryj+(s13+s31)*rxj*rzj+(s23+s32)*ryj*rzj)*sj
                a22(j1,j2,j3) = (s11*sxj**2+s22*syj**2+s33*szj**2+(s12+
     & s21)*sxj*syj+(s13+s31)*sxj*szj+(s23+s32)*syj*szj)*sj
                a33(j1,j2,j3) = (s11*txj**2+s22*tyj**2+s33*tzj**2+(s12+
     & s21)*txj*tyj+(s13+s31)*txj*tzj+(s23+s32)*tyj*tzj)*sj
                a12(j1,j2,j3) = (s11*rxj*sxj+s22*ryj*syj+s33*rzj*szj+
     & s12*rxj*syj+s13*rxj*szj+s21*ryj*sxj+s23*ryj*szj+s31*rzj*sxj+
     & s32*rzj*syj)*sj
                a13(j1,j2,j3) = (s11*rxj*txj+s22*ryj*tyj+s33*rzj*tzj+
     & s12*rxj*tyj+s13*rxj*tzj+s21*ryj*txj+s23*ryj*tzj+s31*rzj*txj+
     & s32*rzj*tyj)*sj
                a23(j1,j2,j3) = (s11*sxj*txj+s22*syj*tyj+s33*szj*tzj+
     & s12*sxj*tyj+s13*sxj*tzj+s21*syj*txj+s23*syj*tzj+s31*szj*txj+
     & s32*szj*tyj)*sj
                a21(j1,j2,j3) = (s11*sxj*rxj+s22*syj*ryj+s33*szj*rzj+
     & s12*sxj*ryj+s13*sxj*rzj+s21*syj*rxj+s23*syj*rzj+s31*szj*rxj+
     & s32*szj*ryj)*sj
                a31(j1,j2,j3) = (s11*txj*rxj+s22*tyj*ryj+s33*tzj*rzj+
     & s12*txj*ryj+s13*txj*rzj+s21*tyj*rxj+s23*tyj*rzj+s31*tzj*rxj+
     & s32*tzj*ryj)*sj
                a32(j1,j2,j3) = (s11*txj*sxj+s22*tyj*syj+s33*tzj*szj+
     & s12*txj*syj+s13*txj*szj+s21*tyj*sxj+s23*tyj*szj+s31*tzj*sxj+
     & s32*tzj*syj)*sj
              end do
            end do
          end do
        else if( derivOption.eq.derivativeScalarDerivative )then
          if(      dir1.eq.0 .and. dir2.eq.0 )then
! DXSDY23(x,x,jac(j1,j2,j3))
            do j3=m3a,m3b
              do j2=m2a,m2b
                do j1=m1a,m1b
                  sj = jac(j1,j2,j3)
                  a11(j1,j2,j3) = (r x (j1,j2,j3)*r x (j1,j2,j3))*sj
                  a12(j1,j2,j3) = (r x (j1,j2,j3)*s x (j1,j2,j3))*sj
                  a13(j1,j2,j3) = (r x (j1,j2,j3)*t x (j1,j2,j3))*sj
                  a22(j1,j2,j3) = (s x (j1,j2,j3)*s x (j1,j2,j3))*sj
                  a23(j1,j2,j3) = (s x (j1,j2,j3)*t x (j1,j2,j3))*sj
                  a33(j1,j2,j3) = (t x (j1,j2,j3)*t x (j1,j2,j3))*sj
! #If "x" == "x"
                  a21(j1,j2,j3) = a12(j1,j2,j3)
                  a31(j1,j2,j3) = a13(j1,j2,j3)
                  a32(j1,j2,j3) = a23(j1,j2,j3)
                end do
              end do
            end do
          else if( dir1.eq.0 .and. dir2.eq.1 )then
! DXSDY23(x,y,jac(j1,j2,j3))
            do j3=m3a,m3b
              do j2=m2a,m2b
                do j1=m1a,m1b
                  sj = jac(j1,j2,j3)
                  a11(j1,j2,j3) = (r x (j1,j2,j3)*r y (j1,j2,j3))*sj
                  a12(j1,j2,j3) = (r x (j1,j2,j3)*s y (j1,j2,j3))*sj
                  a13(j1,j2,j3) = (r x (j1,j2,j3)*t y (j1,j2,j3))*sj
                  a22(j1,j2,j3) = (s x (j1,j2,j3)*s y (j1,j2,j3))*sj
                  a23(j1,j2,j3) = (s x (j1,j2,j3)*t y (j1,j2,j3))*sj
                  a33(j1,j2,j3) = (t x (j1,j2,j3)*t y (j1,j2,j3))*sj
! #If "x" == "y"
! #Else
                  a21(j1,j2,j3) = (s x (j1,j2,j3)*r y (j1,j2,j3))*sj
                  a31(j1,j2,j3) = (t x (j1,j2,j3)*r y (j1,j2,j3))*sj
                  a32(j1,j2,j3) = (t x (j1,j2,j3)*s y (j1,j2,j3))*sj
                end do
              end do
            end do
          else if( dir1.eq.0 .and. dir2.eq.2 )then
! DXSDY23(x,z,jac(j1,j2,j3))
            do j3=m3a,m3b
              do j2=m2a,m2b
                do j1=m1a,m1b
                  sj = jac(j1,j2,j3)
                  a11(j1,j2,j3) = (r x (j1,j2,j3)*r z (j1,j2,j3))*sj
                  a12(j1,j2,j3) = (r x (j1,j2,j3)*s z (j1,j2,j3))*sj
                  a13(j1,j2,j3) = (r x (j1,j2,j3)*t z (j1,j2,j3))*sj
                  a22(j1,j2,j3) = (s x (j1,j2,j3)*s z (j1,j2,j3))*sj
                  a23(j1,j2,j3) = (s x (j1,j2,j3)*t z (j1,j2,j3))*sj
                  a33(j1,j2,j3) = (t x (j1,j2,j3)*t z (j1,j2,j3))*sj
! #If "x" == "z"
! #Else
                  a21(j1,j2,j3) = (s x (j1,j2,j3)*r z (j1,j2,j3))*sj
                  a31(j1,j2,j3) = (t x (j1,j2,j3)*r z (j1,j2,j3))*sj
                  a32(j1,j2,j3) = (t x (j1,j2,j3)*s z (j1,j2,j3))*sj
                end do
              end do
            end do
          else if( dir1.eq.1 .and. dir2.eq.0 )then
! DXSDY23(y,x,jac(j1,j2,j3))
            do j3=m3a,m3b
              do j2=m2a,m2b
                do j1=m1a,m1b
                  sj = jac(j1,j2,j3)
                  a11(j1,j2,j3) = (r y (j1,j2,j3)*r x (j1,j2,j3))*sj
                  a12(j1,j2,j3) = (r y (j1,j2,j3)*s x (j1,j2,j3))*sj
                  a13(j1,j2,j3) = (r y (j1,j2,j3)*t x (j1,j2,j3))*sj
                  a22(j1,j2,j3) = (s y (j1,j2,j3)*s x (j1,j2,j3))*sj
                  a23(j1,j2,j3) = (s y (j1,j2,j3)*t x (j1,j2,j3))*sj
                  a33(j1,j2,j3) = (t y (j1,j2,j3)*t x (j1,j2,j3))*sj
! #If "y" == "x"
! #Else
                  a21(j1,j2,j3) = (s y (j1,j2,j3)*r x (j1,j2,j3))*sj
                  a31(j1,j2,j3) = (t y (j1,j2,j3)*r x (j1,j2,j3))*sj
                  a32(j1,j2,j3) = (t y (j1,j2,j3)*s x (j1,j2,j3))*sj
                end do
              end do
            end do
          else if( dir1.eq.1 .and. dir2.eq.1 )then
! DXSDY23(y,y,jac(j1,j2,j3))
            do j3=m3a,m3b
              do j2=m2a,m2b
                do j1=m1a,m1b
                  sj = jac(j1,j2,j3)
                  a11(j1,j2,j3) = (r y (j1,j2,j3)*r y (j1,j2,j3))*sj
                  a12(j1,j2,j3) = (r y (j1,j2,j3)*s y (j1,j2,j3))*sj
                  a13(j1,j2,j3) = (r y (j1,j2,j3)*t y (j1,j2,j3))*sj
                  a22(j1,j2,j3) = (s y (j1,j2,j3)*s y (j1,j2,j3))*sj
                  a23(j1,j2,j3) = (s y (j1,j2,j3)*t y (j1,j2,j3))*sj
                  a33(j1,j2,j3) = (t y (j1,j2,j3)*t y (j1,j2,j3))*sj
! #If "y" == "y"
                  a21(j1,j2,j3) = a12(j1,j2,j3)
                  a31(j1,j2,j3) = a13(j1,j2,j3)
                  a32(j1,j2,j3) = a23(j1,j2,j3)
                end do
              end do
            end do
          else if( dir1.eq.1 .and. dir2.eq.2 )then
! DXSDY23(y,z,jac(j1,j2,j3))
            do j3=m3a,m3b
              do j2=m2a,m2b
                do j1=m1a,m1b
                  sj = jac(j1,j2,j3)
                  a11(j1,j2,j3) = (r y (j1,j2,j3)*r z (j1,j2,j3))*sj
                  a12(j1,j2,j3) = (r y (j1,j2,j3)*s z (j1,j2,j3))*sj
                  a13(j1,j2,j3) = (r y (j1,j2,j3)*t z (j1,j2,j3))*sj
                  a22(j1,j2,j3) = (s y (j1,j2,j3)*s z (j1,j2,j3))*sj
                  a23(j1,j2,j3) = (s y (j1,j2,j3)*t z (j1,j2,j3))*sj
                  a33(j1,j2,j3) = (t y (j1,j2,j3)*t z (j1,j2,j3))*sj
! #If "y" == "z"
! #Else
                  a21(j1,j2,j3) = (s y (j1,j2,j3)*r z (j1,j2,j3))*sj
                  a31(j1,j2,j3) = (t y (j1,j2,j3)*r z (j1,j2,j3))*sj
                  a32(j1,j2,j3) = (t y (j1,j2,j3)*s z (j1,j2,j3))*sj
                end do
              end do
            end do
          else if( dir1.eq.2 .and. dir2.eq.0 )then
! DXSDY23(z,x,jac(j1,j2,j3))
            do j3=m3a,m3b
              do j2=m2a,m2b
                do j1=m1a,m1b
                  sj = jac(j1,j2,j3)
                  a11(j1,j2,j3) = (r z (j1,j2,j3)*r x (j1,j2,j3))*sj
                  a12(j1,j2,j3) = (r z (j1,j2,j3)*s x (j1,j2,j3))*sj
                  a13(j1,j2,j3) = (r z (j1,j2,j3)*t x (j1,j2,j3))*sj
                  a22(j1,j2,j3) = (s z (j1,j2,j3)*s x (j1,j2,j3))*sj
                  a23(j1,j2,j3) = (s z (j1,j2,j3)*t x (j1,j2,j3))*sj
                  a33(j1,j2,j3) = (t z (j1,j2,j3)*t x (j1,j2,j3))*sj
! #If "z" == "x"
! #Else
                  a21(j1,j2,j3) = (s z (j1,j2,j3)*r x (j1,j2,j3))*sj
                  a31(j1,j2,j3) = (t z (j1,j2,j3)*r x (j1,j2,j3))*sj
                  a32(j1,j2,j3) = (t z (j1,j2,j3)*s x (j1,j2,j3))*sj
                end do
              end do
            end do
          else if( dir1.eq.2 .and. dir2.eq.1 )then
! DXSDY23(z,y,jac(j1,j2,j3))
            do j3=m3a,m3b
              do j2=m2a,m2b
                do j1=m1a,m1b
                  sj = jac(j1,j2,j3)
                  a11(j1,j2,j3) = (r z (j1,j2,j3)*r y (j1,j2,j3))*sj
                  a12(j1,j2,j3) = (r z (j1,j2,j3)*s y (j1,j2,j3))*sj
                  a13(j1,j2,j3) = (r z (j1,j2,j3)*t y (j1,j2,j3))*sj
                  a22(j1,j2,j3) = (s z (j1,j2,j3)*s y (j1,j2,j3))*sj
                  a23(j1,j2,j3) = (s z (j1,j2,j3)*t y (j1,j2,j3))*sj
                  a33(j1,j2,j3) = (t z (j1,j2,j3)*t y (j1,j2,j3))*sj
! #If "z" == "y"
! #Else
                  a21(j1,j2,j3) = (s z (j1,j2,j3)*r y (j1,j2,j3))*sj
                  a31(j1,j2,j3) = (t z (j1,j2,j3)*r y (j1,j2,j3))*sj
                  a32(j1,j2,j3) = (t z (j1,j2,j3)*s y (j1,j2,j3))*sj
                end do
              end do
            end do
          else if( dir1.eq.2 .and. dir2.eq.2 )then
! DXSDY23(z,z,jac(j1,j2,j3))
            do j3=m3a,m3b
              do j2=m2a,m2b
                do j1=m1a,m1b
                  sj = jac(j1,j2,j3)
                  a11(j1,j2,j3) = (r z (j1,j2,j3)*r z (j1,j2,j3))*sj
                  a12(j1,j2,j3) = (r z (j1,j2,j3)*s z (j1,j2,j3))*sj
                  a13(j1,j2,j3) = (r z (j1,j2,j3)*t z (j1,j2,j3))*sj
                  a22(j1,j2,j3) = (s z (j1,j2,j3)*s z (j1,j2,j3))*sj
                  a23(j1,j2,j3) = (s z (j1,j2,j3)*t z (j1,j2,j3))*sj
                  a33(j1,j2,j3) = (t z (j1,j2,j3)*t z (j1,j2,j3))*sj
! #If "z" == "z"
                  a21(j1,j2,j3) = a12(j1,j2,j3)
                  a31(j1,j2,j3) = a13(j1,j2,j3)
                  a32(j1,j2,j3) = a23(j1,j2,j3)
                end do
              end do
            end do
          else
            write(*,*) 'ERROR invalid values: dir1=',dir1,' dir2=',dir2
          end if
        end if
        if( derivType.eq.symmetric )then
        ! symmetric case -- do not average a12, a21, a13, z31, etc.  -- could do better
         m1a=n1a
         do j3=m3a,m3b
          do j2=m2a,m2b
            do j1=m1b,m1a,-1 ! go backwards   worry about division by zero
             sh=s(j1,j2,j3,0)*s(j1-1,j2,j3,0)/(s(j1,j2,j3,0)+s(j1-1,j2,
     & j3,0))
              a11(j1,j2,j3) = sh *d22(1)*(a11(j1,j2,j3)+a11(j1-1,j2,j3)
     & )
            end do
          end do
         end do
         m1a=n1a-1
         do j3=m3a,m3b
          do j2=m2a,m2b
            do j1=m1b,m1a,-1
! #If "" eq "c"
! #Else
              a12(j1,j2,j3) = s(j1,j2,j3,0)*(d12(1)*d12(2))*a12(j1,j2,
     & j3)
              a13(j1,j2,j3) = s(j1,j2,j3,0)*(d12(1)*d12(3))*a13(j1,j2,
     & j3)
            end do
          end do
         end do
         m2a=n2a
         do j3=m3a,m3b
          do j2=m2b,m2a,-1
            do j1=m1a,m1b
             sh=s(j1,j2,j3,0)*s(j1,j2-1,j3,0)/(s(j1,j2,j3,0)+s(j1,j2-1,
     & j3,0))
              a22(j1,j2,j3) = sh *d22(2)*(a22(j1,j2,j3)+a22(j1,j2-1,j3)
     & )
            end do
          end do
         end do
         m2a=n2a-1
         do j3=m3a,m3b
          do j2=m2b,m2a,-1
            do j1=m1a,m1b
! #If "" eq "c"
! #Else
              a21(j1,j2,j3) = s(j1,j2,j3,0)*(d12(1)*d12(2))*a21(j1,j2,
     & j3)
              a23(j1,j2,j3) = s(j1,j2,j3,0)*(d12(2)*d12(3))*a23(j1,j2,
     & j3)
            end do
          end do
         end do
         m3a=n3a
         do j3=m3b,m3a,-1
          do j2=m2a,m2b
            do j1=m1a,m1b
             sh=s(j1,j2,j3,0)*s(j1,j2,j3-1,0)/(s(j1,j2,j3,0)+s(j1,j2,
     & j3-1,0))
              a33(j1,j2,j3) = sh *d22(3)*(a33(j1,j2,j3)+a33(j1,j2,j3-1)
     & )
            end do
          end do
         end do
         m3a=n3a-1
         do j3=m3b,m3a,-1
          do j2=m2a,m2b
            do j1=m1a,m1b
! #If "" eq "c"
! #Else
              a31(j1,j2,j3) = s(j1,j2,j3,0)*(d12(1)*d12(3))*a31(j1,j2,
     & j3)
              a32(j1,j2,j3) = s(j1,j2,j3,0)*(d12(2)*d12(3))*a32(j1,j2,
     & j3)
            end do
          end do
         end do
           else
         ! ** old way -- average all coefficients
        m1a=n1a
        do j3=m3a,m3b
          do j2=m2a,m2b
            do j1=m1b,m1a,-1 ! go backwards  worry about division by zero
             sh=s(j1,j2,j3,0)*s(j1-1,j2,j3,0)/(s(j1,j2,j3,0)+s(j1-1,j2,
     & j3,0))
              a11(j1,j2,j3) = sh *d22(1)*(a11(j1,j2,j3)+a11(j1-1,j2,j3)
     & )
              a12(j1,j2,j3) = sh *(d12(1)*d12(2))*(a12(j1,j2,j3)+a12(
     & j1-1,j2,j3))
              a13(j1,j2,j3) = sh *(d12(1)*d12(3))*(a13(j1,j2,j3)+a13(
     & j1-1,j2,j3))
            end do
          end do
        end do
        m1a=n1a-1
        m2a=n2a
        do j3=m3a,m3b
          do j2=m2b,m2a,-1
            do j1=m1a,m1b
             sh=s(j1,j2,j3,0)*s(j1,j2-1,j3,0)/(s(j1,j2,j3,0)+s(j1,j2-1,
     & j3,0))
              a21(j1,j2,j3) = sh *(d12(1)*d12(2))*(a21(j1,j2,j3)+a21(
     & j1,j2-1,j3))
              a22(j1,j2,j3) = sh *d22(2)*(a22(j1,j2,j3)+a22(j1,j2-1,j3)
     & )
              a23(j1,j2,j3) = sh *(d12(2)*d12(3))*(a23(j1,j2,j3)+a23(
     & j1,j2-1,j3))
            end do
          end do
        end do
        m2a=n2a-1
        m3a=n3a
        do j3=m3b,m3a,-1
          do j2=m2a,m2b
            do j1=m1a,m1b
             sh=s(j1,j2,j3,0)*s(j1,j2,j3-1,0)/(s(j1,j2,j3,0)+s(j1,j2,
     & j3-1,0))
              a31(j1,j2,j3) = sh *(d12(1)*d12(3))*(a31(j1,j2,j3)+a31(
     & j1,j2,j3-1))
              a32(j1,j2,j3) = sh *(d12(2)*d12(3))*(a32(j1,j2,j3)+a32(
     & j1,j2,j3-1))
              a33(j1,j2,j3) = sh *d22(3)*(a33(j1,j2,j3)+a33(j1,j2,j3-1)
     & )
            end do
          end do
        end do
        m3a=n3a-1
        end if
      end if

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
     & (a11(i1+1,i2  ,i3  )*urr(i1+1,i2  ,i3  ,c) - a11(i1,i2,i3)*urr(
     & i1  ,i2  ,i3  ,c))+
     & (a22(i1  ,i2+1,i3  )*uss(i1  ,i2+1,i3  ,c) - a22(i1,i2,i3)*uss(
     & i1  ,i2  ,i3  ,c))+
     & (a33(i1  ,i2  ,i3+1)*utt(i1  ,i2  ,i3+1,c) - a33(i1,i2,i3)*utt(
     & i1  ,i2  ,i3  ,c))+
     & (a21(i1  ,i2+1,i3  )*D0r(i1  ,i2+1,i3  ,c) - a21(i1,i2-1,i3)*
     & D0r(i1  ,i2-1,i3  ,c) +
     & a12(i1+1,i2  ,i3  )*D0s(i1+1,i2  ,i3  ,c) - a12(i1-1,i2,i3)*
     & D0s(i1-1,i2  ,i3  ,c))+
     & (a31(i1  ,i2  ,i3+1)*D0r(i1  ,i2  ,i3+1,c) - a31(i1,i2,i3-1)*
     & D0r(i1  ,i2  ,i3-1,c) +
     & a13(i1+1,i2  ,i3  )*D0t(i1+1,i2  ,i3  ,c) - a13(i1-1,i2,i3)*
     & D0t(i1-1,i2  ,i3  ,c))+
     & (a32(i1  ,i2  ,i3+1)*D0s(i1  ,i2  ,i3+1,c) - a32(i1,i2,i3-1)*
     & D0s(i1  ,i2  ,i3-1,c) +
     & a23(i1  ,i2+1,i3  )*D0t(i1  ,i2+1,i3  ,c) - a23(i1,i2-1,i3)*
     & D0t(i1  ,i2-1,i3  ,c))
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
     & (a11(i1+1,i2  ,i3  )*urr(i1+1,i2  ,i3  ,c) - a11(i1,i2,i3)*urr(
     & i1,i2,i3,c))+
     & (a22(i1  ,i2+1,i3  )*uss(i1  ,i2+1,i3  ,c) - a22(i1,i2,i3)*uss(
     & i1,i2,i3,c))+
     & (a33(i1  ,i2  ,i3+1)*utt(i1  ,i2  ,i3+1,c) - a33(i1,i2,i3)*utt(
     & i1,i2,i3,c))+
     & (a21(i1  ,i2+1,i3  )*usr(i1  ,i2+1,i3  ,c) - a21(i1,i2,i3)*usr(
     & i1,i2,i3,c) +
     & a12(i1+1,i2  ,i3  )*urs(i1+1,i2  ,i3  ,c) - a12(i1,i2,i3)*urs(
     & i1,i2,i3,c))+
     & (a31(i1  ,i2  ,i3+1)*utr(i1  ,i2  ,i3+1,c) - a31(i1,i2,i3)*utr(
     & i1,i2,i3,c) +
     & a13(i1+1,i2  ,i3  )*urt(i1+1,i2  ,i3  ,c) - a13(i1,i2,i3)*urt(
     & i1,i2,i3,c))+
     & (a32(i1  ,i2  ,i3+1)*uts(i1  ,i2  ,i3+1,c) - a32(i1,i2,i3)*uts(
     & i1,i2,i3,c) +
     & a23(i1  ,i2+1,i3  )*ust(i1  ,i2+1,i3  ,c) - a23(i1,i2,i3)*ust(
     & i1,i2,i3,c))
     &        )/jac(i1,i2,i3)
            end do
          end do
        end do
      end do
      end if

      return
      end
