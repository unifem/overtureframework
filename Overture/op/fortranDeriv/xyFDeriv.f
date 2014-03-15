c gDeriv.f is the generic function used to generate xFDeriv.f, yFDeriv.f, ...
c using the script gDeriv.p

      subroutine xyFDeriv( nd, 
     &    nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,         
     &    ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b, ! dimensions for u
     &    ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b, ! dimensions for deriv
     &    n1a,n1b,n2a,n2b,n3a,n3b, ca,cb, 
     &    h21, d22,d12, h22, d14, d24, h41, h42, 
     &    rsxy, u,deriv, gridType, order )
c ===============================================================
c    derivative
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
     &  gridType, order

      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,1:nd,1:nd)
      real u(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,ndu4a:ndu4b)
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
      real UXY21R, UXY21, UXY41R, UXY41
      real UXY22R, UXY22, UXY42R, UXY42
      real UXY23R, UXY23, UXY43R, UXY43
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

      kd3=nd

      if( nd .eq. 2 )then
c     ************************
c     ******* 2D *************      
c     ************************

        if( gridType .eq. 0 )then
c     rectangular
          if( order.eq.2 )then
            do c=ca,cb
              do i3=n3a,n3b
                do i2=n2a,n2b
                  do i1=n1a,n1b
                    deriv(i1,i2,i3,c)=UXY22R(i1,i2,i3,c)
                  end do
                end do
              end do
            end do
          else
            do c=ca,cb
              do i3=n3a,n3b
                do i2=n2a,n2b
                  do i1=n1a,n1b
                    deriv(i1,i2,i3,c)=UXY42R(i1,i2,i3,c)
                  end do
                end do
              end do
            end do

          end if

        else
c     ***** not rectangular *****
          if( order.eq.2 )then
            do c=ca,cb
              do i3=n3a,n3b
                do i2=n2a,n2b
                  do i1=n1a,n1b
                    deriv(i1,i2,i3,c)=UXY22(i1,i2,i3,c)
                  end do
                end do
              end do
            end do
          else
            do c=ca,cb
              do i3=n3a,n3b
                do i2=n2a,n2b
                  do i1=n1a,n1b
                    deriv(i1,i2,i3,c)=UXY42(i1,i2,i3,c)
                  end do
                end do
              end do
            end do

          end if

        endif 
      elseif( nd.eq.3 )then
c     ************************
c     ******* 3D *************      
c     ************************
        if( gridType .eq. 0 )then
c     rectangular
          if( order.eq.2 )then
            do c=ca,cb
              do i3=n3a,n3b
                do i2=n2a,n2b
                  do i1=n1a,n1b
                    deriv(i1,i2,i3,c)=UXY23R(i1,i2,i3,c)
                  end do
                end do
              end do
            end do
          else
            do c=ca,cb
              do i3=n3a,n3b
                do i2=n2a,n2b
                  do i1=n1a,n1b
                    deriv(i1,i2,i3,c)=UXY43R(i1,i2,i3,c)
                  end do
                end do
              end do
            end do

          end if

        else
c     ***** not rectangular *****
          if( order.eq.2 )then
            do c=ca,cb
              do i3=n3a,n3b
                do i2=n2a,n2b
                  do i1=n1a,n1b
                    deriv(i1,i2,i3,c)=UXY23(i1,i2,i3,c)
                  end do
                end do
              end do
            end do
          else
            do c=ca,cb
              do i3=n3a,n3b
                do i2=n2a,n2b
                  do i1=n1a,n1b
                    deriv(i1,i2,i3,c)=UXY43(i1,i2,i3,c)
                  end do
                end do
              end do
            end do

          end if

        endif 
  

      elseif( nd.eq.1 )then
c       ************************
c       ******* 1D *************      
c       ************************

        if( gridType .eq. 0 )then
c     rectangular
          if( order.eq.2 )then
            do c=ca,cb
              do i3=n3a,n3b
                do i2=n2a,n2b
                  do i1=n1a,n1b
                    deriv(i1,i2,i3,c)=UXY21R(i1,i2,i3,c)
                  end do
                end do
              end do
            end do
          else
            do c=ca,cb
              do i3=n3a,n3b
                do i2=n2a,n2b
                  do i1=n1a,n1b
                    deriv(i1,i2,i3,c)=UXY41R(i1,i2,i3,c)
                  end do
                end do
              end do
            end do

          end if

        else
c     ***** not rectangular *****
          if( order.eq.2 )then
            do c=ca,cb
              do i3=n3a,n3b
                do i2=n2a,n2b
                  do i1=n1a,n1b
                    deriv(i1,i2,i3,c)=UXY21(i1,i2,i3,c)
                  end do
                end do
              end do
            end do
          else
            do c=ca,cb
              do i3=n3a,n3b
                do i2=n2a,n2b
                  do i1=n1a,n1b
                    deriv(i1,i2,i3,c)=UXY41(i1,i2,i3,c)
                  end do
                end do
              end do
            end do

          end if

        endif 
      else if( nd.eq.0 )then
c       *** add these lines to avoid warnings about unused statement functions
        include "cgux2afNoWarnings.h" 
        include "cgux4afNoWarnings.h" 

      end if
      return
      end

