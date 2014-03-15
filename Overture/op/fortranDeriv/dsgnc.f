! This file automatically generated from dsg.bf with bpp.

      subroutine divScalarGradNC( nd,
     &    nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &    ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b, ! dimensions for u
     &    ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b, ! dimensions for deriv
     &    n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,
     &    h21, d22,d12, h22, d14, d24, h41, h42,
     &    rsxy, u,s, deriv,
     &    derivOption, gridType, order, averagingType, dir1, dir2 )
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
      integer nd,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b,
     &  n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,
     &  derivOption, gridType, order, averagingType, dir1, dir2

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

      sy21(i1,i2,i3)=0.
      sz21(i1,i2,i3)=0.
      sz42(i1,i2,i3)=0.
      sz22(i1,i2,i3)=0.
      sz41(i1,i2,i3)=0.
      sy41(i1,i2,i3)=0.

c      uzx23(i1,i2,i3,c) =uxz23(i1,i2,i3,c)
c      uzx23r(i1,i2,i3,c)=uxz23r(i1,i2,i3,c)
c      uzy23 (i1,i2,i3,c)=uyz23 (i1,i2,i3,c)
c      uzy23r(i1,i2,i3,c)=uyz23r(i1,i2,i3,c)
c      uzx43r(i1,i2,i3,c)=uxz43r(i1,i2,i3,c)
c      uzx43 (i1,i2,i3,c)=uxz43 (i1,i2,i3,c)
c      uzy43 (i1,i2,i3,c)=uyz43 (i1,i2,i3,c)
c      uzy43r(i1,i2,i3,c)=uyz43r(i1,i2,i3,c)

c     These appear but should never be called
c      uzx21 (i1,i2,i3,c)=0.
c      uzx21r(i1,i2,i3,c)=0.
c      uzy21r(i1,i2,i3,c)=0.
c      uzy22r(i1,i2,i3,c)=0.
c      uzy22(i1,i2,i3,c)=0.
c      uzy41r(i1,i2,i3,c)=0.
c      uzx42r(i1,i2,i3,c)=0.
c      uzx42 (i1,i2,i3,c)=0.
c      uzx41 (i1,i2,i3,c)=0.
c      uzy41 (i1,i2,i3,c)=0.
c      uzy42 (i1,i2,i3,c)=0.
c      uzy42r(i1,i2,i3,c)=0.
c      uzy21 (i1,i2,i3,c)=0.
c      uzx22 (i1,i2,i3,c)=0.
c      uzx22r(i1,i2,i3,c)=0.
c      uzx41r(i1,i2,i3,c)=0.

c......end statement function


c This next macro defines the operator D_X( s D_Y u )

      kd3=nd

      if( derivOption.eq.divScalarGrad )then
c       ****** divScalarGrad ******

        if( nd .eq. 2 )then
c         ******* 2D *************      
          if( gridType .eq. 0 )then
c           rectangular
            if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*LAPLACIAN22R(i1,i2,i3,c)+SX22R(i1,i2,i3)*UX22R(i1,i2,i3,c)+SY22R(i1,i2,i3)*UY22R(i1,i2,i3,c))
              do c=ca,cb
                do i3=n3a,n3b
                  do i2=n2a,n2b
                    do i1=n1a,n1b
                      deriv(i1,i2,i3,c)=s(i1,i2,i3)*LAPLACIAN22R(i1,i2,
     & i3,c)+SX22R(i1,i2,i3)*UX22R(i1,i2,i3,c)+SY22R(i1,i2,i3)*UY22R(
     & i1,i2,i3,c)
                    end do
                  end do
                end do
              end do
            else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*LAPLACIAN42R(i1,i2,i3,c) +SX42R(i1,i2,i3)*UX42R(i1,i2,i3,c)+SY42R(i1,i2,i3)*UY42R(i1,i2,i3,c))
              do c=ca,cb
                do i3=n3a,n3b
                  do i2=n2a,n2b
                    do i1=n1a,n1b
                      deriv(i1,i2,i3,c)=s(i1,i2,i3)*LAPLACIAN42R(i1,i2,
     & i3,c)+SX42R(i1,i2,i3)*UX42R(i1,i2,i3,c)+SY42R(i1,i2,i3)*UY42R(
     & i1,i2,i3,c)
                    end do
                  end do
                end do
              end do
            end if
          else
c           ***** not rectangular *****
            if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*LAPLACIAN22(i1,i2,i3,c)+SX22(i1,i2,i3)*UX22(i1,i2,i3,c)+SY22(i1,i2,i3)*UY22(i1,i2,i3,c))
              do c=ca,cb
                do i3=n3a,n3b
                  do i2=n2a,n2b
                    do i1=n1a,n1b
                      deriv(i1,i2,i3,c)=s(i1,i2,i3)*LAPLACIAN22(i1,i2,
     & i3,c)+SX22(i1,i2,i3)*UX22(i1,i2,i3,c)+SY22(i1,i2,i3)*UY22(i1,
     & i2,i3,c)
                    end do
                  end do
                end do
              end do
            else
! loopsDSG(deriv(i1,i2,i3,c)= s(i1,i2,i3)*LAPLACIAN42(i1,i2,i3,c)+SX42(i1,i2,i3)*UX42(i1,i2,i3,c)+SY42(i1,i2,i3)*UY42(i1,i2,i3,c))
              do c=ca,cb
                do i3=n3a,n3b
                  do i2=n2a,n2b
                    do i1=n1a,n1b
                      deriv(i1,i2,i3,c)=s(i1,i2,i3)*LAPLACIAN42(i1,i2,
     & i3,c)+SX42(i1,i2,i3)*UX42(i1,i2,i3,c)+SY42(i1,i2,i3)*UY42(i1,
     & i2,i3,c)
                    end do
                  end do
                end do
              end do
            end if
          endif
        elseif( nd.eq.3 )then
c         ******* 3D *************      
          if( gridType .eq. 0 )then
c           rectangular
            if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*LAPLACIAN23R(i1,i2,i3,c)+SX22R(i1,i2,i3)*UX23R(i1,i2,i3,c)+SY22R(i1,i2,i3)*UY23R(i1,i2,i3,c)+SZ22R(i1,i2,i3)*UZ23R(i1,i2,i3,c))
              do c=ca,cb
                do i3=n3a,n3b
                  do i2=n2a,n2b
                    do i1=n1a,n1b
                      deriv(i1,i2,i3,c)=s(i1,i2,i3)*LAPLACIAN23R(i1,i2,
     & i3,c)+SX22R(i1,i2,i3)*UX23R(i1,i2,i3,c)+SY22R(i1,i2,i3)*UY23R(
     & i1,i2,i3,c)+SZ22R(i1,i2,i3)*UZ23R(i1,i2,i3,c)
                    end do
                  end do
                end do
              end do
            else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*LAPLACIAN43R(i1,i2,i3,c)+SX42R(i1,i2,i3)*UX42R(i1,i2,i3,c)+SY42R(i1,i2,i3)*UY42R(i1,i2,i3,c)+SZ42R(i1,i2,i3)*UZ42R(i1,i2,i3,c))
              do c=ca,cb
                do i3=n3a,n3b
                  do i2=n2a,n2b
                    do i1=n1a,n1b
                      deriv(i1,i2,i3,c)=s(i1,i2,i3)*LAPLACIAN43R(i1,i2,
     & i3,c)+SX42R(i1,i2,i3)*UX42R(i1,i2,i3,c)+SY42R(i1,i2,i3)*UY42R(
     & i1,i2,i3,c)+SZ42R(i1,i2,i3)*UZ42R(i1,i2,i3,c)
                    end do
                  end do
                end do
              end do
            end if
          else
c           ***** not rectangular *****
            if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*LAPLACIAN23(i1,i2,i3,c)+SX23(i1,i2,i3)*UX23(i1,i2,i3,c)+SY23(i1,i2,i3)*UY23(i1,i2,i3,c)+SZ23(i1,i2,i3)*UZ23(i1,i2,i3,c))
              do c=ca,cb
                do i3=n3a,n3b
                  do i2=n2a,n2b
                    do i1=n1a,n1b
                      deriv(i1,i2,i3,c)=s(i1,i2,i3)*LAPLACIAN23(i1,i2,
     & i3,c)+SX23(i1,i2,i3)*UX23(i1,i2,i3,c)+SY23(i1,i2,i3)*UY23(i1,
     & i2,i3,c)+SZ23(i1,i2,i3)*UZ23(i1,i2,i3,c)
                    end do
                  end do
                end do
              end do
            else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*LAPLACIAN43(i1,i2,i3,c)+SX43(i1,i2,i3)*UX43(i1,i2,i3,c)+SY43(i1,i2,i3)*UY43(i1,i2,i3,c)+SZ43(i1,i2,i3)*UZ43(i1,i2,i3,c))
              do c=ca,cb
                do i3=n3a,n3b
                  do i2=n2a,n2b
                    do i1=n1a,n1b
                      deriv(i1,i2,i3,c)=s(i1,i2,i3)*LAPLACIAN43(i1,i2,
     & i3,c)+SX43(i1,i2,i3)*UX43(i1,i2,i3,c)+SY43(i1,i2,i3)*UY43(i1,
     & i2,i3,c)+SZ43(i1,i2,i3)*UZ43(i1,i2,i3,c)
                    end do
                  end do
                end do
              end do
            end if
          endif

        else
c         ******* 1D *************      
          if( gridType .eq. 0 )then
c           rectangular
            if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*LAPLACIAN21R(i1,i2,i3,c)+SX22R(i1,i2,i3)*UX22R(i1,i2,i3,c))
              do c=ca,cb
                do i3=n3a,n3b
                  do i2=n2a,n2b
                    do i1=n1a,n1b
                      deriv(i1,i2,i3,c)=s(i1,i2,i3)*LAPLACIAN21R(i1,i2,
     & i3,c)+SX22R(i1,i2,i3)*UX22R(i1,i2,i3,c)
                    end do
                  end do
                end do
              end do
            else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*LAPLACIAN41R(i1,i2,i3,c)+SX42R(i1,i2,i3)*UX42R(i1,i2,i3,c))
              do c=ca,cb
                do i3=n3a,n3b
                  do i2=n2a,n2b
                    do i1=n1a,n1b
                      deriv(i1,i2,i3,c)=s(i1,i2,i3)*LAPLACIAN41R(i1,i2,
     & i3,c)+SX42R(i1,i2,i3)*UX42R(i1,i2,i3,c)
                    end do
                  end do
                end do
              end do
            end if

          else
c            ***** not rectangular *****
            if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*LAPLACIAN21(i1,i2,i3,c)+SX21(i1,i2,i3)*UX21(i1,i2,i3,c))
              do c=ca,cb
                do i3=n3a,n3b
                  do i2=n2a,n2b
                    do i1=n1a,n1b
                      deriv(i1,i2,i3,c)=s(i1,i2,i3)*LAPLACIAN21(i1,i2,
     & i3,c)+SX21(i1,i2,i3)*UX21(i1,i2,i3,c)
                    end do
                  end do
                end do
              end do
            else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*LAPLACIAN41(i1,i2,i3,c)+SX41(i1,i2,i3)*UX41(i1,i2,i3,c))
              do c=ca,cb
                do i3=n3a,n3b
                  do i2=n2a,n2b
                    do i1=n1a,n1b
                      deriv(i1,i2,i3,c)=s(i1,i2,i3)*LAPLACIAN41(i1,i2,
     & i3,c)+SX41(i1,i2,i3)*UX41(i1,i2,i3,c)
                    end do
                  end do
                end do
              end do
            end if

          endif
        end if

      elseif( derivOption.eq.laplace )then

c       ****** laplace ******

        if( nd .eq. 2 )then
c         ******* 2D *************      
          if( gridType .eq. 0 )then
c           rectangular
            if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=LAPLACIAN22R(i1,i2,i3,c))
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
! loopsDSG(deriv(i1,i2,i3,c)=LAPLACIAN42R(i1,i2,i3,c))
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
! loopsDSG(deriv(i1,i2,i3,c)=LAPLACIAN22(i1,i2,i3,c))
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
! loopsDSG(deriv(i1,i2,i3,c)=LAPLACIAN42(i1,i2,i3,c))
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
c         ******* 3D *************      
          if( gridType .eq. 0 )then
c           rectangular
            if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=LAPLACIAN23R(i1,i2,i3,c))
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
! loopsDSG(deriv(i1,i2,i3,c)=LAPLACIAN43R(i1,i2,i3,c))
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
! loopsDSG(deriv(i1,i2,i3,c)=LAPLACIAN23(i1,i2,i3,c))
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
! loopsDSG(deriv(i1,i2,i3,c)=LAPLACIAN43(i1,i2,i3,c))
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
c         ******* 1D *************      
          if( gridType .eq. 0 )then
c           rectangular
            if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=LAPLACIAN21R(i1,i2,i3,c))
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
! loopsDSG(deriv(i1,i2,i3,c)=LAPLACIAN41R(i1,i2,i3,c))
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
! loopsDSG(deriv(i1,i2,i3,c)=LAPLACIAN21(i1,i2,i3,c))
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
! loopsDSG(deriv(i1,i2,i3,c)=LAPLACIAN41(i1,i2,i3,c))
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

        if(      dir1.eq.0 .and. dir2.eq.0 )then
! DXDY(XX,X,X)
          if( nd .eq. 2 )then
c ******* 2D *************      
            if( gridType .eq. 0 )then
c   rectangular
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XX 22R(i1,i2,i3,c)+S X 22R(i1,i2,i3)*U X 22R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXX22R(i1,i2,i3,
     & c)+SX22R(i1,i2,i3)*UX22R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XX 42R(i1,i2,i3,c)+S X 42R(i1,i2,i3)*U X 42R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXX42R(i1,i2,i3,
     & c)+SX42R(i1,i2,i3)*UX42R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if
            else
c   ***** not rectangular *****
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XX 22(i1,i2,i3,c)+S X 22(i1,i2,i3)*U X 22(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXX22(i1,i2,i3,c)
     & +SX22(i1,i2,i3)*UX22(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)= s(i1,i2,i3)*U XX 42(i1,i2,i3,c)+S X 42(i1,i2,i3)*U X 42(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXX42(i1,i2,i3,c)
     & +SX42(i1,i2,i3)*UX42(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if
            endif
          elseif( nd.eq.3 )then
c ******* 3D *************      
            if( gridType .eq. 0 )then
c   rectangular
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XX 23R(i1,i2,i3,c)+S X 22R(i1,i2,i3)*U X 23R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXX23R(i1,i2,i3,
     & c)+SX22R(i1,i2,i3)*UX23R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XX 43R(i1,i2,i3,c)+S X 42R(i1,i2,i3)*U X 42R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXX43R(i1,i2,i3,
     & c)+SX42R(i1,i2,i3)*UX42R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if
            else
c   ***** not rectangular *****
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XX 23(i1,i2,i3,c)+S X 23(i1,i2,i3)*U X 23(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXX23(i1,i2,i3,c)
     & +SX23(i1,i2,i3)*UX23(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XX 43(i1,i2,i3,c)+S X 43(i1,i2,i3)*U X 43(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXX43(i1,i2,i3,c)
     & +SX43(i1,i2,i3)*UX43(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if
            endif

           else
c   ******* 1D *************      
            if( gridType .eq. 0 )then
c   rectangular
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XX 21R(i1,i2,i3,c)+S X 22R(i1,i2,i3)*U X 22R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXX21R(i1,i2,i3,
     & c)+SX22R(i1,i2,i3)*UX22R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XX 41R(i1,i2,i3,c)+S X 42R(i1,i2,i3)*U X 42R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXX41R(i1,i2,i3,
     & c)+SX42R(i1,i2,i3)*UX42R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if

            else
c    ***** not rectangular *****
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XX 21(i1,i2,i3,c)+S X 21(i1,i2,i3)*U X 21(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXX21(i1,i2,i3,c)
     & +SX21(i1,i2,i3)*UX21(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XX 41(i1,i2,i3,c)+S X 41(i1,i2,i3)*U X 41(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXX41(i1,i2,i3,c)
     & +SX41(i1,i2,i3)*UX41(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if

            endif
          end if
        else if( dir1.eq.0 .and. dir2.eq.1 )then
! DXDY(XY,X,Y)
          if( nd .eq. 2 )then
c ******* 2D *************      
            if( gridType .eq. 0 )then
c   rectangular
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XY 22R(i1,i2,i3,c)+S X 22R(i1,i2,i3)*U Y 22R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXY22R(i1,i2,i3,
     & c)+SX22R(i1,i2,i3)*UY22R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XY 42R(i1,i2,i3,c)+S X 42R(i1,i2,i3)*U Y 42R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXY42R(i1,i2,i3,
     & c)+SX42R(i1,i2,i3)*UY42R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if
            else
c   ***** not rectangular *****
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XY 22(i1,i2,i3,c)+S X 22(i1,i2,i3)*U Y 22(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXY22(i1,i2,i3,c)
     & +SX22(i1,i2,i3)*UY22(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)= s(i1,i2,i3)*U XY 42(i1,i2,i3,c)+S X 42(i1,i2,i3)*U Y 42(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXY42(i1,i2,i3,c)
     & +SX42(i1,i2,i3)*UY42(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if
            endif
          elseif( nd.eq.3 )then
c ******* 3D *************      
            if( gridType .eq. 0 )then
c   rectangular
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XY 23R(i1,i2,i3,c)+S X 22R(i1,i2,i3)*U Y 23R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXY23R(i1,i2,i3,
     & c)+SX22R(i1,i2,i3)*UY23R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XY 43R(i1,i2,i3,c)+S X 42R(i1,i2,i3)*U Y 42R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXY43R(i1,i2,i3,
     & c)+SX42R(i1,i2,i3)*UY42R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if
            else
c   ***** not rectangular *****
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XY 23(i1,i2,i3,c)+S X 23(i1,i2,i3)*U Y 23(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXY23(i1,i2,i3,c)
     & +SX23(i1,i2,i3)*UY23(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XY 43(i1,i2,i3,c)+S X 43(i1,i2,i3)*U Y 43(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXY43(i1,i2,i3,c)
     & +SX43(i1,i2,i3)*UY43(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if
            endif

           else
c   ******* 1D *************      
            if( gridType .eq. 0 )then
c   rectangular
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XY 21R(i1,i2,i3,c)+S X 22R(i1,i2,i3)*U Y 22R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXY21R(i1,i2,i3,
     & c)+SX22R(i1,i2,i3)*UY22R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XY 41R(i1,i2,i3,c)+S X 42R(i1,i2,i3)*U Y 42R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXY41R(i1,i2,i3,
     & c)+SX42R(i1,i2,i3)*UY42R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if

            else
c    ***** not rectangular *****
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XY 21(i1,i2,i3,c)+S X 21(i1,i2,i3)*U Y 21(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXY21(i1,i2,i3,c)
     & +SX21(i1,i2,i3)*UY21(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XY 41(i1,i2,i3,c)+S X 41(i1,i2,i3)*U Y 41(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXY41(i1,i2,i3,c)
     & +SX41(i1,i2,i3)*UY41(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if

            endif
          end if
        else if( dir1.eq.0 .and. dir2.eq.2 )then
! DXDY(XZ,X,Z)
          if( nd .eq. 2 )then
c ******* 2D *************      
            if( gridType .eq. 0 )then
c   rectangular
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XZ 22R(i1,i2,i3,c)+S X 22R(i1,i2,i3)*U Z 22R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXZ22R(i1,i2,i3,
     & c)+SX22R(i1,i2,i3)*UZ22R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XZ 42R(i1,i2,i3,c)+S X 42R(i1,i2,i3)*U Z 42R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXZ42R(i1,i2,i3,
     & c)+SX42R(i1,i2,i3)*UZ42R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if
            else
c   ***** not rectangular *****
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XZ 22(i1,i2,i3,c)+S X 22(i1,i2,i3)*U Z 22(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXZ22(i1,i2,i3,c)
     & +SX22(i1,i2,i3)*UZ22(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)= s(i1,i2,i3)*U XZ 42(i1,i2,i3,c)+S X 42(i1,i2,i3)*U Z 42(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXZ42(i1,i2,i3,c)
     & +SX42(i1,i2,i3)*UZ42(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if
            endif
          elseif( nd.eq.3 )then
c ******* 3D *************      
            if( gridType .eq. 0 )then
c   rectangular
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XZ 23R(i1,i2,i3,c)+S X 22R(i1,i2,i3)*U Z 23R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXZ23R(i1,i2,i3,
     & c)+SX22R(i1,i2,i3)*UZ23R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XZ 43R(i1,i2,i3,c)+S X 42R(i1,i2,i3)*U Z 42R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXZ43R(i1,i2,i3,
     & c)+SX42R(i1,i2,i3)*UZ42R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if
            else
c   ***** not rectangular *****
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XZ 23(i1,i2,i3,c)+S X 23(i1,i2,i3)*U Z 23(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXZ23(i1,i2,i3,c)
     & +SX23(i1,i2,i3)*UZ23(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XZ 43(i1,i2,i3,c)+S X 43(i1,i2,i3)*U Z 43(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXZ43(i1,i2,i3,c)
     & +SX43(i1,i2,i3)*UZ43(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if
            endif

           else
c   ******* 1D *************      
            if( gridType .eq. 0 )then
c   rectangular
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XZ 21R(i1,i2,i3,c)+S X 22R(i1,i2,i3)*U Z 22R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXZ21R(i1,i2,i3,
     & c)+SX22R(i1,i2,i3)*UZ22R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XZ 41R(i1,i2,i3,c)+S X 42R(i1,i2,i3)*U Z 42R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXZ41R(i1,i2,i3,
     & c)+SX42R(i1,i2,i3)*UZ42R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if

            else
c    ***** not rectangular *****
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XZ 21(i1,i2,i3,c)+S X 21(i1,i2,i3)*U Z 21(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXZ21(i1,i2,i3,c)
     & +SX21(i1,i2,i3)*UZ21(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XZ 41(i1,i2,i3,c)+S X 41(i1,i2,i3)*U Z 41(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXZ41(i1,i2,i3,c)
     & +SX41(i1,i2,i3)*UZ41(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if

            endif
          end if
        else if( dir1.eq.1 .and. dir2.eq.0 )then
! DXDY(XY,Y,X)
          if( nd .eq. 2 )then
c ******* 2D *************      
            if( gridType .eq. 0 )then
c   rectangular
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XY 22R(i1,i2,i3,c)+S Y 22R(i1,i2,i3)*U X 22R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXY22R(i1,i2,i3,
     & c)+SY22R(i1,i2,i3)*UX22R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XY 42R(i1,i2,i3,c)+S Y 42R(i1,i2,i3)*U X 42R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXY42R(i1,i2,i3,
     & c)+SY42R(i1,i2,i3)*UX42R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if
            else
c   ***** not rectangular *****
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XY 22(i1,i2,i3,c)+S Y 22(i1,i2,i3)*U X 22(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXY22(i1,i2,i3,c)
     & +SY22(i1,i2,i3)*UX22(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)= s(i1,i2,i3)*U XY 42(i1,i2,i3,c)+S Y 42(i1,i2,i3)*U X 42(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXY42(i1,i2,i3,c)
     & +SY42(i1,i2,i3)*UX42(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if
            endif
          elseif( nd.eq.3 )then
c ******* 3D *************      
            if( gridType .eq. 0 )then
c   rectangular
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XY 23R(i1,i2,i3,c)+S Y 22R(i1,i2,i3)*U X 23R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXY23R(i1,i2,i3,
     & c)+SY22R(i1,i2,i3)*UX23R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XY 43R(i1,i2,i3,c)+S Y 42R(i1,i2,i3)*U X 42R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXY43R(i1,i2,i3,
     & c)+SY42R(i1,i2,i3)*UX42R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if
            else
c   ***** not rectangular *****
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XY 23(i1,i2,i3,c)+S Y 23(i1,i2,i3)*U X 23(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXY23(i1,i2,i3,c)
     & +SY23(i1,i2,i3)*UX23(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XY 43(i1,i2,i3,c)+S Y 43(i1,i2,i3)*U X 43(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXY43(i1,i2,i3,c)
     & +SY43(i1,i2,i3)*UX43(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if
            endif

           else
c   ******* 1D *************      
            if( gridType .eq. 0 )then
c   rectangular
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XY 21R(i1,i2,i3,c)+S Y 22R(i1,i2,i3)*U X 22R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXY21R(i1,i2,i3,
     & c)+SY22R(i1,i2,i3)*UX22R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XY 41R(i1,i2,i3,c)+S Y 42R(i1,i2,i3)*U X 42R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXY41R(i1,i2,i3,
     & c)+SY42R(i1,i2,i3)*UX42R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if

            else
c    ***** not rectangular *****
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XY 21(i1,i2,i3,c)+S Y 21(i1,i2,i3)*U X 21(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXY21(i1,i2,i3,c)
     & +SY21(i1,i2,i3)*UX21(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XY 41(i1,i2,i3,c)+S Y 41(i1,i2,i3)*U X 41(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXY41(i1,i2,i3,c)
     & +SY41(i1,i2,i3)*UX41(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if

            endif
          end if
        else if( dir1.eq.1 .and. dir2.eq.1 )then
! DXDY(YY,Y,Y)
          if( nd .eq. 2 )then
c ******* 2D *************      
            if( gridType .eq. 0 )then
c   rectangular
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U YY 22R(i1,i2,i3,c)+S Y 22R(i1,i2,i3)*U Y 22R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UYY22R(i1,i2,i3,
     & c)+SY22R(i1,i2,i3)*UY22R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U YY 42R(i1,i2,i3,c)+S Y 42R(i1,i2,i3)*U Y 42R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UYY42R(i1,i2,i3,
     & c)+SY42R(i1,i2,i3)*UY42R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if
            else
c   ***** not rectangular *****
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U YY 22(i1,i2,i3,c)+S Y 22(i1,i2,i3)*U Y 22(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UYY22(i1,i2,i3,c)
     & +SY22(i1,i2,i3)*UY22(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)= s(i1,i2,i3)*U YY 42(i1,i2,i3,c)+S Y 42(i1,i2,i3)*U Y 42(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UYY42(i1,i2,i3,c)
     & +SY42(i1,i2,i3)*UY42(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if
            endif
          elseif( nd.eq.3 )then
c ******* 3D *************      
            if( gridType .eq. 0 )then
c   rectangular
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U YY 23R(i1,i2,i3,c)+S Y 22R(i1,i2,i3)*U Y 23R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UYY23R(i1,i2,i3,
     & c)+SY22R(i1,i2,i3)*UY23R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U YY 43R(i1,i2,i3,c)+S Y 42R(i1,i2,i3)*U Y 42R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UYY43R(i1,i2,i3,
     & c)+SY42R(i1,i2,i3)*UY42R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if
            else
c   ***** not rectangular *****
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U YY 23(i1,i2,i3,c)+S Y 23(i1,i2,i3)*U Y 23(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UYY23(i1,i2,i3,c)
     & +SY23(i1,i2,i3)*UY23(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U YY 43(i1,i2,i3,c)+S Y 43(i1,i2,i3)*U Y 43(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UYY43(i1,i2,i3,c)
     & +SY43(i1,i2,i3)*UY43(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if
            endif

           else
c   ******* 1D *************      
            if( gridType .eq. 0 )then
c   rectangular
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U YY 21R(i1,i2,i3,c)+S Y 22R(i1,i2,i3)*U Y 22R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UYY21R(i1,i2,i3,
     & c)+SY22R(i1,i2,i3)*UY22R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U YY 41R(i1,i2,i3,c)+S Y 42R(i1,i2,i3)*U Y 42R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UYY41R(i1,i2,i3,
     & c)+SY42R(i1,i2,i3)*UY42R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if

            else
c    ***** not rectangular *****
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U YY 21(i1,i2,i3,c)+S Y 21(i1,i2,i3)*U Y 21(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UYY21(i1,i2,i3,c)
     & +SY21(i1,i2,i3)*UY21(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U YY 41(i1,i2,i3,c)+S Y 41(i1,i2,i3)*U Y 41(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UYY41(i1,i2,i3,c)
     & +SY41(i1,i2,i3)*UY41(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if

            endif
          end if
        else if( dir1.eq.1 .and. dir2.eq.2 )then
! DXDY(YZ,Y,Z)
          if( nd .eq. 2 )then
c ******* 2D *************      
            if( gridType .eq. 0 )then
c   rectangular
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U YZ 22R(i1,i2,i3,c)+S Y 22R(i1,i2,i3)*U Z 22R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UYZ22R(i1,i2,i3,
     & c)+SY22R(i1,i2,i3)*UZ22R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U YZ 42R(i1,i2,i3,c)+S Y 42R(i1,i2,i3)*U Z 42R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UYZ42R(i1,i2,i3,
     & c)+SY42R(i1,i2,i3)*UZ42R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if
            else
c   ***** not rectangular *****
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U YZ 22(i1,i2,i3,c)+S Y 22(i1,i2,i3)*U Z 22(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UYZ22(i1,i2,i3,c)
     & +SY22(i1,i2,i3)*UZ22(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)= s(i1,i2,i3)*U YZ 42(i1,i2,i3,c)+S Y 42(i1,i2,i3)*U Z 42(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UYZ42(i1,i2,i3,c)
     & +SY42(i1,i2,i3)*UZ42(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if
            endif
          elseif( nd.eq.3 )then
c ******* 3D *************      
            if( gridType .eq. 0 )then
c   rectangular
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U YZ 23R(i1,i2,i3,c)+S Y 22R(i1,i2,i3)*U Z 23R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UYZ23R(i1,i2,i3,
     & c)+SY22R(i1,i2,i3)*UZ23R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U YZ 43R(i1,i2,i3,c)+S Y 42R(i1,i2,i3)*U Z 42R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UYZ43R(i1,i2,i3,
     & c)+SY42R(i1,i2,i3)*UZ42R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if
            else
c   ***** not rectangular *****
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U YZ 23(i1,i2,i3,c)+S Y 23(i1,i2,i3)*U Z 23(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UYZ23(i1,i2,i3,c)
     & +SY23(i1,i2,i3)*UZ23(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U YZ 43(i1,i2,i3,c)+S Y 43(i1,i2,i3)*U Z 43(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UYZ43(i1,i2,i3,c)
     & +SY43(i1,i2,i3)*UZ43(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if
            endif

           else
c   ******* 1D *************      
            if( gridType .eq. 0 )then
c   rectangular
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U YZ 21R(i1,i2,i3,c)+S Y 22R(i1,i2,i3)*U Z 22R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UYZ21R(i1,i2,i3,
     & c)+SY22R(i1,i2,i3)*UZ22R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U YZ 41R(i1,i2,i3,c)+S Y 42R(i1,i2,i3)*U Z 42R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UYZ41R(i1,i2,i3,
     & c)+SY42R(i1,i2,i3)*UZ42R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if

            else
c    ***** not rectangular *****
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U YZ 21(i1,i2,i3,c)+S Y 21(i1,i2,i3)*U Z 21(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UYZ21(i1,i2,i3,c)
     & +SY21(i1,i2,i3)*UZ21(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U YZ 41(i1,i2,i3,c)+S Y 41(i1,i2,i3)*U Z 41(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UYZ41(i1,i2,i3,c)
     & +SY41(i1,i2,i3)*UZ41(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if

            endif
          end if
        else if( dir1.eq.2 .and. dir2.eq.0 )then
! DXDY(XZ,Z,X)
          if( nd .eq. 2 )then
c ******* 2D *************      
            if( gridType .eq. 0 )then
c   rectangular
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XZ 22R(i1,i2,i3,c)+S Z 22R(i1,i2,i3)*U X 22R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXZ22R(i1,i2,i3,
     & c)+SZ22R(i1,i2,i3)*UX22R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XZ 42R(i1,i2,i3,c)+S Z 42R(i1,i2,i3)*U X 42R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXZ42R(i1,i2,i3,
     & c)+SZ42R(i1,i2,i3)*UX42R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if
            else
c   ***** not rectangular *****
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XZ 22(i1,i2,i3,c)+S Z 22(i1,i2,i3)*U X 22(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXZ22(i1,i2,i3,c)
     & +SZ22(i1,i2,i3)*UX22(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)= s(i1,i2,i3)*U XZ 42(i1,i2,i3,c)+S Z 42(i1,i2,i3)*U X 42(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXZ42(i1,i2,i3,c)
     & +SZ42(i1,i2,i3)*UX42(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if
            endif
          elseif( nd.eq.3 )then
c ******* 3D *************      
            if( gridType .eq. 0 )then
c   rectangular
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XZ 23R(i1,i2,i3,c)+S Z 22R(i1,i2,i3)*U X 23R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXZ23R(i1,i2,i3,
     & c)+SZ22R(i1,i2,i3)*UX23R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XZ 43R(i1,i2,i3,c)+S Z 42R(i1,i2,i3)*U X 42R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXZ43R(i1,i2,i3,
     & c)+SZ42R(i1,i2,i3)*UX42R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if
            else
c   ***** not rectangular *****
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XZ 23(i1,i2,i3,c)+S Z 23(i1,i2,i3)*U X 23(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXZ23(i1,i2,i3,c)
     & +SZ23(i1,i2,i3)*UX23(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XZ 43(i1,i2,i3,c)+S Z 43(i1,i2,i3)*U X 43(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXZ43(i1,i2,i3,c)
     & +SZ43(i1,i2,i3)*UX43(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if
            endif

           else
c   ******* 1D *************      
            if( gridType .eq. 0 )then
c   rectangular
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XZ 21R(i1,i2,i3,c)+S Z 22R(i1,i2,i3)*U X 22R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXZ21R(i1,i2,i3,
     & c)+SZ22R(i1,i2,i3)*UX22R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XZ 41R(i1,i2,i3,c)+S Z 42R(i1,i2,i3)*U X 42R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXZ41R(i1,i2,i3,
     & c)+SZ42R(i1,i2,i3)*UX42R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if

            else
c    ***** not rectangular *****
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XZ 21(i1,i2,i3,c)+S Z 21(i1,i2,i3)*U X 21(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXZ21(i1,i2,i3,c)
     & +SZ21(i1,i2,i3)*UX21(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U XZ 41(i1,i2,i3,c)+S Z 41(i1,i2,i3)*U X 41(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UXZ41(i1,i2,i3,c)
     & +SZ41(i1,i2,i3)*UX41(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if

            endif
          end if
        else if( dir1.eq.2 .and. dir2.eq.1 )then
! DXDY(YZ,Z,Y)
          if( nd .eq. 2 )then
c ******* 2D *************      
            if( gridType .eq. 0 )then
c   rectangular
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U YZ 22R(i1,i2,i3,c)+S Z 22R(i1,i2,i3)*U Y 22R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UYZ22R(i1,i2,i3,
     & c)+SZ22R(i1,i2,i3)*UY22R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U YZ 42R(i1,i2,i3,c)+S Z 42R(i1,i2,i3)*U Y 42R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UYZ42R(i1,i2,i3,
     & c)+SZ42R(i1,i2,i3)*UY42R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if
            else
c   ***** not rectangular *****
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U YZ 22(i1,i2,i3,c)+S Z 22(i1,i2,i3)*U Y 22(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UYZ22(i1,i2,i3,c)
     & +SZ22(i1,i2,i3)*UY22(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)= s(i1,i2,i3)*U YZ 42(i1,i2,i3,c)+S Z 42(i1,i2,i3)*U Y 42(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UYZ42(i1,i2,i3,c)
     & +SZ42(i1,i2,i3)*UY42(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if
            endif
          elseif( nd.eq.3 )then
c ******* 3D *************      
            if( gridType .eq. 0 )then
c   rectangular
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U YZ 23R(i1,i2,i3,c)+S Z 22R(i1,i2,i3)*U Y 23R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UYZ23R(i1,i2,i3,
     & c)+SZ22R(i1,i2,i3)*UY23R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U YZ 43R(i1,i2,i3,c)+S Z 42R(i1,i2,i3)*U Y 42R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UYZ43R(i1,i2,i3,
     & c)+SZ42R(i1,i2,i3)*UY42R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if
            else
c   ***** not rectangular *****
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U YZ 23(i1,i2,i3,c)+S Z 23(i1,i2,i3)*U Y 23(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UYZ23(i1,i2,i3,c)
     & +SZ23(i1,i2,i3)*UY23(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U YZ 43(i1,i2,i3,c)+S Z 43(i1,i2,i3)*U Y 43(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UYZ43(i1,i2,i3,c)
     & +SZ43(i1,i2,i3)*UY43(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if
            endif

           else
c   ******* 1D *************      
            if( gridType .eq. 0 )then
c   rectangular
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U YZ 21R(i1,i2,i3,c)+S Z 22R(i1,i2,i3)*U Y 22R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UYZ21R(i1,i2,i3,
     & c)+SZ22R(i1,i2,i3)*UY22R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U YZ 41R(i1,i2,i3,c)+S Z 42R(i1,i2,i3)*U Y 42R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UYZ41R(i1,i2,i3,
     & c)+SZ42R(i1,i2,i3)*UY42R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if

            else
c    ***** not rectangular *****
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U YZ 21(i1,i2,i3,c)+S Z 21(i1,i2,i3)*U Y 21(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UYZ21(i1,i2,i3,c)
     & +SZ21(i1,i2,i3)*UY21(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U YZ 41(i1,i2,i3,c)+S Z 41(i1,i2,i3)*U Y 41(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UYZ41(i1,i2,i3,c)
     & +SZ41(i1,i2,i3)*UY41(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if

            endif
          end if
        else if( dir1.eq.2 .and. dir2.eq.2 )then
! DXDY(ZZ,Z,Z)
          if( nd .eq. 2 )then
c ******* 2D *************      
            if( gridType .eq. 0 )then
c   rectangular
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U ZZ 22R(i1,i2,i3,c)+S Z 22R(i1,i2,i3)*U Z 22R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UZZ22R(i1,i2,i3,
     & c)+SZ22R(i1,i2,i3)*UZ22R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U ZZ 42R(i1,i2,i3,c)+S Z 42R(i1,i2,i3)*U Z 42R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UZZ42R(i1,i2,i3,
     & c)+SZ42R(i1,i2,i3)*UZ42R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if
            else
c   ***** not rectangular *****
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U ZZ 22(i1,i2,i3,c)+S Z 22(i1,i2,i3)*U Z 22(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UZZ22(i1,i2,i3,c)
     & +SZ22(i1,i2,i3)*UZ22(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)= s(i1,i2,i3)*U ZZ 42(i1,i2,i3,c)+S Z 42(i1,i2,i3)*U Z 42(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UZZ42(i1,i2,i3,c)
     & +SZ42(i1,i2,i3)*UZ42(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if
            endif
          elseif( nd.eq.3 )then
c ******* 3D *************      
            if( gridType .eq. 0 )then
c   rectangular
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U ZZ 23R(i1,i2,i3,c)+S Z 22R(i1,i2,i3)*U Z 23R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UZZ23R(i1,i2,i3,
     & c)+SZ22R(i1,i2,i3)*UZ23R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U ZZ 43R(i1,i2,i3,c)+S Z 42R(i1,i2,i3)*U Z 42R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UZZ43R(i1,i2,i3,
     & c)+SZ42R(i1,i2,i3)*UZ42R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if
            else
c   ***** not rectangular *****
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U ZZ 23(i1,i2,i3,c)+S Z 23(i1,i2,i3)*U Z 23(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UZZ23(i1,i2,i3,c)
     & +SZ23(i1,i2,i3)*UZ23(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U ZZ 43(i1,i2,i3,c)+S Z 43(i1,i2,i3)*U Z 43(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UZZ43(i1,i2,i3,c)
     & +SZ43(i1,i2,i3)*UZ43(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if
            endif

           else
c   ******* 1D *************      
            if( gridType .eq. 0 )then
c   rectangular
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U ZZ 21R(i1,i2,i3,c)+S Z 22R(i1,i2,i3)*U Z 22R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UZZ21R(i1,i2,i3,
     & c)+SZ22R(i1,i2,i3)*UZ22R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U ZZ 41R(i1,i2,i3,c)+S Z 42R(i1,i2,i3)*U Z 42R(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UZZ41R(i1,i2,i3,
     & c)+SZ42R(i1,i2,i3)*UZ42R(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if

            else
c    ***** not rectangular *****
              if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U ZZ 21(i1,i2,i3,c)+S Z 21(i1,i2,i3)*U Z 21(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UZZ21(i1,i2,i3,c)
     & +SZ21(i1,i2,i3)*UZ21(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3)*U ZZ 41(i1,i2,i3,c)+S Z 41(i1,i2,i3)*U Z 41(i1,i2,i3,c))
                do c=ca,cb
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        deriv(i1,i2,i3,c)=s(i1,i2,i3)*UZZ41(i1,i2,i3,c)
     & +SZ41(i1,i2,i3)*UZ41(i1,i2,i3,c)
                      end do
                    end do
                  end do
                end do
              end if

            endif
          end if
        else
          write(*,*) 'ERROR invalid values: dir1=',dir1,' dir2=',dir2
        end if
      end if

      if( nd.eq.0 )then
c       *** add these lines to avoid warnings about unused statement functions
        include "cgux2afNoWarnings.h"
        include "cgux4afNoWarnings.h"
      end if

      return
      end

