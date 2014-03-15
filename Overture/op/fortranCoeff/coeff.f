      subroutine laplacianCoeff( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &    n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb, ca,cb, 
     &    d22,d12, h22, rsxy,coeff, gridType, order )
c ===============================================================
c  Laplacian Coefficients
c  
c nc : number of components
c ns : stencil size
c ca,cb : assign components c=ca,..,cb (base 0)
c ea,eb : assign equations e=ea,..eb   (base 0)
c gridType: 0=rectangular, 1=non-rectangular
c order : 2 or 4
c rsxy : not used if rectangular
c h22 : 1/h**2 : for rectangular  
c ===============================================================

c      implicit none
      integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &  n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc,ns, ca,cb,ea,eb,
     &  gridType, order

      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,1:nd,1:nd)
      real coeff(1:ndc,nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real d22(*),d12(*),h22(*)
      real rx,ry,rz,sx,sy,sz,tx,ty,tz
      real rxSq,rxx,sxSq,sxx,rsx,rxx2,ryy2,sxx2,syy2
      real rxt2,ryt2,rzz23,sxt2,syt2,szz23,txr2,txs2
      real txt2,tyr2,tys2,tyt2,tzz23,rzr2,rzs2,rzt2
      real szr2,szs2,szt2,tzr2,tzs2,tzt2
      real rxr2,rxs2,ryr2,rys2,sxr2,sxs2,syr2,sys2
      real txx,txSq,rtx,stx,rxx23,ryy23,sxx23,syy23,txx23,tyy23

c..... added by kkc 1/2/02 for g77 unsatisfied reference
      real u(1,1,1,1),h21(3)

      integer i1,i2,i3,kd3,kd,c,e,ec
      integer m12,m22,m32
      integer m(-1:1,-1:1),m3(-1:1,-1:1,-1:1)

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
      rxx1(i1,i2,i3)=rx(i1,i2,i3)*rxr2(i1,i2,i3)

      if( order.ne.2 )then
        write(*,*) 'laplacianCoeff:ERROR: order!=2 '
        stop
      end if

      kd3=nd  

c     ***** loop over equations and components *****
      do e=ea,eb
        do c=ca,cb
          ec=ns*(c+nc*e)


      if( nd .eq. 2 )then
c       ************************
c       ******* 2D *************      
c       ************************

      do i2=-1,1
        do i1=-1,1
         m(i1,i2)=i1+1+3*(i2+1) +1 + ec
        end do
      end do

      if( gridType .eq. 0 )then
c       rectangular

        do i3=n3a,n3b
        do i2=n2a,n2b
        do i1=n1a,n1b

c         ** it did not affect performance to use an array to index coeff ***
          coeff(m(-1,-1),i1,i2,i3)=       0.
          coeff(m( 0,-1),i1,i2,i3)=            h22(2)
          coeff(m(+1,-1),i1,i2,i3)=       0.
          coeff(m(-1, 0),i1,i2,i3)=     h22(1)
          coeff(m( 0, 0),i1,i2,i3)=-2.*(h22(1)+h22(2))
          coeff(m(+1, 0),i1,i2,i3)=     h22(1)
          coeff(m(-1,+1),i1,i2,i3)=       0.
          coeff(m( 0,+1),i1,i2,i3)=            h22(2)
          coeff(m(+1,+1),i1,i2,i3)=       0.

        end do
        end do
        end do


      else
       

c      ***** not rectangular *****
        do i3=n3a,n3b
        do i2=n2a,n2b
        do i1=n1a,n1b

          rxSq=d22(1)*(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)
          rxx =d12(1)*(rxx2(i1,i2,i3)+ryy2(i1,i2,i3)) 
          sxSq=d22(2)*(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2)
          sxx =d12(2)*(sxx2(i1,i2,i3)+syy2(i1,i2,i3))
          rsx =(2.*d12(1)*d12(2))*(rx(i1,i2,i3)*sx(i1,i2,i3)+
     &                               ry(i1,i2,i3)*sy(i1,i2,i3))

          coeff(m(-1,-1),i1,i2,i3)=                              rsx
          coeff(m( 0,-1),i1,i2,i3)=                    sxSq -sxx
          coeff(m(+1,-1),i1,i2,i3)=                             -rsx 
          coeff(m(-1, 0),i1,i2,i3)=     rxSq      -rxx
          coeff(m( 0, 0),i1,i2,i3)=-2.*(rxSq+sxSq)
          coeff(m(+1, 0),i1,i2,i3)=     rxSq      +rxx
          coeff(m(-1,+1),i1,i2,i3)=                             -rsx 
          coeff(m( 0,+1),i1,i2,i3)=                    sxSq +sxx
          coeff(m(+1,+1),i1,i2,i3)=                              rsx 

        end do
        end do
        end do

      endif 
      elseif( nd.eq.3 )then
c       ************************
c       ******* 3D *************      
c       ************************
  
        do i3=-1,1
          do i2=-1,1
            do i1=-1,1
              m3(i1,i2,i3)=i1+1+3*(i2+1+3*(i3+1)) +1 + ec
            end do
          end do
        end do


      if( gridType .eq. 0 )then
c       rectangular
        do i3=n3a,n3b
        do i2=n2a,n2b
        do i1=n1a,n1b

          coeff(m3(-1,-1,-1),i1,i2,i3)=0.
          coeff(m3( 0,-1,-1),i1,i2,i3)=0.
          coeff(m3(+1,-1,-1),i1,i2,i3)=0.
          coeff(m3(-1, 0,-1),i1,i2,i3)=0.
          coeff(m3( 0, 0,-1),i1,i2,i3)=                     h22(3)
          coeff(m3(+1, 0,-1),i1,i2,i3)=0.
          coeff(m3(-1,+1,-1),i1,i2,i3)=0.
          coeff(m3( 0,+1,-1),i1,i2,i3)=0.
          coeff(m3(+1,+1,-1),i1,i2,i3)=0.
          coeff(m3(-1,-1, 0),i1,i2,i3)=0.
          coeff(m3( 0,-1, 0),i1,i2,i3)=                h22(2)
          coeff(m3(+1,-1, 0),i1,i2,i3)=0.
          coeff(m3(-1, 0, 0),i1,i2,i3)=     h22(1)
          coeff(m3( 0, 0, 0),i1,i2,i3)=-2.*(h22(1)+h22(2)+h22(3))
          coeff(m3(+1, 0, 0),i1,i2,i3)=     h22(1)
          coeff(m3(-1,+1, 0),i1,i2,i3)=0.
          coeff(m3( 0,+1, 0),i1,i2,i3)=                h22(2)
          coeff(m3(+1,+1, 0),i1,i2,i3)=0.
          coeff(m3(-1,-1,+1),i1,i2,i3)=0.
          coeff(m3( 0,-1,+1),i1,i2,i3)=0.
          coeff(m3(+1,-1,+1),i1,i2,i3)=0.
          coeff(m3(-1, 0,+1),i1,i2,i3)=0.
          coeff(m3( 0, 0,+1),i1,i2,i3)=                     h22(3)
          coeff(m3(+1, 0,+1),i1,i2,i3)=0.
          coeff(m3(-1,+1,+1),i1,i2,i3)=0.
          coeff(m3( 0,+1,+1),i1,i2,i3)=0.
          coeff(m3(+1,+1,+1),i1,i2,i3)=0.

        end do
        end do
        end do


      else
       

c      ***** not rectangular *****
        do i3=n3a,n3b
        do i2=n2a,n2b
        do i1=n1a,n1b

          rxSq = d22(1)*(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+
     &             rz(i1,i2,i3)**2) 
          rxx  = d12(1)*(rxx23(i1,i2,i3)+ryy23(i1,i2,i3)+
     &              rzz23(i1,i2,i3))
          
          sxSq = d22(2)*(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2+
     &            sz(i1,i2,i3)**2) 
          sxx  = d12(2)*(sxx23(i1,i2,i3)+syy23(i1,i2,i3)+
     &             szz23(i1,i2,i3))
          
          txSq = d22(3)*(tx(i1,i2,i3)**2+ty(i1,i2,i3)**2+
     &         tz(i1,i2,i3)**2) 
          txx  = d12(3)*(txx23(i1,i2,i3)+tyy23(i1,i2,i3)+
     &          tzz23(i1,i2,i3))
          
          rsx  = (2.*d12(1)*d12(2))*(rx(i1,i2,i3)*sx(i1,i2,i3)+
     &         ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,i3)) 
          rtx  = (2.*d12(1)*d12(3))*(rx(i1,i2,i3)*tx(i1,i2,i3)+
     &         ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3)) 
          stx  = (2.*d12(2)*d12(3))*(sx(i1,i2,i3)*tx(i1,i2,i3)+
     &         sy(i1,i2,i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3)) 

	
          coeff(m3(-1,-1,-1),i1,i2,i3)= 0.
          coeff(m3( 0,-1,-1),i1,i2,i3)=                          stx
          coeff(m3(+1,-1,-1),i1,i2,i3)= 0.
          coeff(m3(-1, 0,-1),i1,i2,i3)=                          rtx
          coeff(m3( 0, 0,-1),i1,i2,i3)=               txSq -txx
          coeff(m3(+1, 0,-1),i1,i2,i3)=                          -rtx
          coeff(m3(-1,+1,-1),i1,i2,i3)= 0.
          coeff(m3( 0,+1,-1),i1,i2,i3)=                          -stx
          coeff(m3(+1,+1,-1),i1,i2,i3)= 0.
          coeff(m3(-1,-1, 0),i1,i2,i3)=                           rsx
          coeff(m3( 0,-1, 0),i1,i2,i3)=                    sxSq -sxx
          coeff(m3(+1,-1, 0),i1,i2,i3)=                          -rsx 
          coeff(m3(-1, 0, 0),i1,i2,i3)=     rxSq      -rxx
          coeff(m3( 0, 0, 0),i1,i2,i3)=-2.*(rxSq+sxSq+txSq)
          coeff(m3(+1, 0, 0),i1,i2,i3)=     rxSq      +rxx
          coeff(m3(-1,+1, 0),i1,i2,i3)=                          -rsx 
          coeff(m3( 0,+1, 0),i1,i2,i3)=                    sxSq +sxx
          coeff(m3(+1,+1, 0),i1,i2,i3)=                           rsx 
          coeff(m3(-1,-1,+1),i1,i2,i3)= 0.
          coeff(m3( 0,-1,+1),i1,i2,i3)=                          -stx
          coeff(m3(+1,-1,+1),i1,i2,i3)= 0.
          coeff(m3(-1, 0,+1),i1,i2,i3)=                          -rtx
          coeff(m3( 0, 0,+1),i1,i2,i3)=               txSq +txx
          coeff(m3(+1, 0,+1),i1,i2,i3)=                           rtx
          coeff(m3(-1,+1,+1),i1,i2,i3)= 0.
          coeff(m3( 0,+1,+1),i1,i2,i3)=                           stx
          coeff(m3(+1,+1,+1),i1,i2,i3)= 0.


        end do
        end do
        end do

      end if


      else
c       ************************
c       ******* 1D *************      
c       ************************


      m12=1 + ec
      m22=2 + ec
      m32=3 + ec

      

      if( gridType .eq. 0 )then
c       rectangular
        do i3=n3a,n3b
        do i2=n2a,n2b
        do i1=n1a,n1b

          coeff(m12,i1,i2,i3)=    h22(1)
          coeff(m22,i1,i2,i3)=-2.*h22(1)
          coeff(m32,i1,i2,i3)=    h22(1)

        end do
        end do
        end do

      else

c      ***** not rectangular *****
        do i3=n3a,n3b
        do i2=n2a,n2b
        do i1=n1a,n1b

          rxSq=d22(1)*rx(i1,i2,i3)**2
          rxx =d12(1)*rxx1(i1,i2,i3)

          coeff(m12,i1,i2,i3)=    rxSq      -rxx
          coeff(m22,i1,i2,i3)=-2.*rxSq
          coeff(m32,i1,i2,i3)=    rxSq      +rxx

        end do
        end do
        end do

      end if

      end if

      end do  ! end c
      end do  ! end e


      if( nd.eq.0 )then
c       *** add these lines to avoid warnings about unused statement functions
        include "cgux2afNoWarnings.h" 
      end if

      return
      end

      subroutine laplacianCoeff4( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &    n1a,n1b,n2a,n2b,n3a,n3b, ndc,  nc, ns, ea,eb, ca,cb, 
     &    d24,d14, h42, rsxy,coeff, gridType, order )
c ===============================================================
c  Laplacian Coefficients - 4th order version
c  
c gridType: 0=rectangular, 1=non-rectangular
c rsxy : not used if rectangular
c h42 : 1/h**2 : for rectangular  
c ===============================================================

c      implicit none
      integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &        n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb, ca,cb,
     &    gridType,order

      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,1:nd,1:nd)
      real coeff(1:ndc,nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real d24(*),d14(*),h42(*)
      real rx,ry,rz,sx,sy,sz,tx,ty,tz
      real rxSq,rxxyy,sxSq,sxxyy,txxyy,txSq
      real rxx,ryy,sxx,syy,rxx3,ryy3,rzz3,sxx3,syy3,szz3,txx3,tyy3,tzz3
      real rsx,rtx,stx
      real rxt,ryt,sxt,syt,txr,txs
      real txt,tyr,tys,tyt,rzr,rzs,rzt
      real szr,szs,szt,tzr,tzs,tzt
      real rxr,rxs,ryr,rys,sxr,sxs,syr,sys
      real rsx8,rsx64,rtx8,rtx64,stx8,stx64

c..... added by kkc 1/2/02 for g77 unsatisfied reference
      real u(1,1,1,1),h41(3)

      integer i1,i2,i3,kd3,kd,kdd,e,c,ec,j
      integer m12,m22,m32,m42,m52

      integer m(-2:2,-2:2),m3(-2:2,-2:2,-2:2)

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

      include 'cgux4af.h'
      rxx1(i1,i2,i3)=rx(i1,i2,i3)*rxr(i1,i2,i3)

      kd3=nd  

c     ***** loop over equations and components *****
      do e=ea,eb
        do c=ca,cb
          ec=ns*(c+nc*e)


      if( nd .eq. 2 )then
c       ************************
c       ******* 2D *************      
c       ************************


      do i2=-2,2
        do i1=-2,2
         m(i1,i2)=i1+2+5*(i2+2) +1 + ec
        end do
      end do

      if( gridType .eq. 0 )then
c       rectangular
        do i3=n3a,n3b
        do i2=n2a,n2b
        do i1=n1a,n1b

          do j=ec+1,ec+25
            coeff(j,i1,i2,i3)=0.
          end do

          coeff(m( 0,-2),i1,i2,i3)=             -h42(2)
          coeff(m( 0,-1),i1,i2,i3)=          16.*h42(2)
          coeff(m(-2, 0),i1,i2,i3)=     -h42(1)
          coeff(m(-1, 0),i1,i2,i3)= 16.* h42(1)
          coeff(m( 0, 0),i1,i2,i3)=-30.*(h42(1) +h42(2) )
          coeff(m( 1, 0),i1,i2,i3)= 16.* h42(1)
          coeff(m( 2, 0),i1,i2,i3)=     -h42(1)
          coeff(m( 0, 1),i1,i2,i3)=          16.*h42(2)
          coeff(m( 0, 2),i1,i2,i3)=             -h42(2)

        end do
        end do
        end do


      else
       

c      ***** not rectangular *****
        do i3=n3a,n3b
        do i2=n2a,n2b
        do i1=n1a,n1b

          rxSq=d24(1)*(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)
          rxxyy=d14(1)*(rxx(i1,i2,i3)+ryy(i1,i2,i3)) 
          sxSq=d24(2)*(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2)
          sxxyy=d14(2)*(sxx(i1,i2,i3)+syy(i1,i2,i3))
          rsx =(2.*d14(1)*d14(2))*(rx(i1,i2,i3)*sx(i1,i2,i3)+
     &                             ry(i1,i2,i3)*sy(i1,i2,i3))

          rsx8  = rsx*8. 
          rsx64 = rsx*64.

          coeff(m(-2,-2),i1,i2,i3)=     rsx
          coeff(m(-1,-2),i1,i2,i3)= -  rsx8 
          coeff(m( 0,-2),i1,i2,i3)=               -sxSq    +sxxyy
          coeff(m(+1,-2),i1,i2,i3)=    rsx8 
          coeff(m(+2,-2),i1,i2,i3)=    -rsx
                       
          coeff(m(-2,-1),i1,i2,i3)= -  rsx8
          coeff(m(-1,-1),i1,i2,i3)=   rsx64 
          coeff(m( 0,-1),i1,i2,i3)=            16.*sxSq -8.*sxxyy
          coeff(m(+1,-1),i1,i2,i3)=-  rsx64 
          coeff(m(+2,-1),i1,i2,i3)=    rsx8
                       
          coeff(m(-2, 0),i1,i2,i3)=    -rxSq    +rxxyy
          coeff(m(-1, 0),i1,i2,i3)= 16.*rxSq -8.*rxxyy
          coeff(m( 0, 0),i1,i2,i3)=-30.*(rxSq   +sxSq  )
          coeff(m(+1, 0),i1,i2,i3)= 16.*rxSq +8.*rxxyy
          coeff(m(+2, 0),i1,i2,i3)=    -rxSq    -rxxyy
                       
          coeff(m(-2, 1),i1,i2,i3)=    rsx8
          coeff(m(-1, 1),i1,i2,i3)=-  rsx64 
          coeff(m( 0, 1),i1,i2,i3)=            16.*sxSq +8.*sxxyy
          coeff(m(+1, 1),i1,i2,i3)=   rsx64 
          coeff(m(+2, 1),i1,i2,i3)= -  rsx8
                       
          coeff(m(-2, 2),i1,i2,i3)=    -rsx
          coeff(m(-1, 2),i1,i2,i3)=    rsx8 
          coeff(m( 0, 2),i1,i2,i3)=               -sxSq    -sxxyy
          coeff(m(+1, 2),i1,i2,i3)= -  rsx8 
          coeff(m(+2, 2),i1,i2,i3)=     rsx




        end do
        end do
        end do

      endif 
      elseif( nd.eq.3 )then
c       ************************
c       ******* 3D *************      
c       ************************
        do i3=-2,2
          do i2=-2,2
            do i1=-2,2
              m3(i1,i2,i3)=i1+2+5*(i2+2+5*(i3+2)) +1 + ec
            end do
          end do
        end do


      if( gridType .eq. 0 )then
c       rectangular
        do i3=n3a,n3b
        do i2=n2a,n2b
        do i1=n1a,n1b

          do j=ec+1,ec+125
            coeff(j,i1,i2,i3)=0.
          end do

          coeff(m3( 0, 0,-2),i1,i2,i3)=                      -h42(3)
          coeff(m3( 0, 0,-1),i1,i2,i3)=                   16.*h42(3)
          coeff(m3( 0,-2, 0),i1,i2,i3)=             -h42(2)
          coeff(m3( 0,-1, 0),i1,i2,i3)=          16.*h42(2)
          coeff(m3(-2, 0, 0),i1,i2,i3)=     -h42(1)
          coeff(m3(-1, 0, 0),i1,i2,i3)= 16.* h42(1)
          coeff(m3( 0, 0, 0),i1,i2,i3)=-30.*(h42(1) +h42(2)  +h42(3))
          coeff(m3( 1, 0, 0),i1,i2,i3)= 16.* h42(1)
          coeff(m3( 2, 0, 0),i1,i2,i3)=     -h42(1)
          coeff(m3( 0, 1, 0),i1,i2,i3)=          16.*h42(2)
          coeff(m3( 0, 2, 0),i1,i2,i3)=             -h42(2)
          coeff(m3( 0, 0, 1),i1,i2,i3)=                    16.*h42(3)
          coeff(m3( 0, 0, 2),i1,i2,i3)=                       -h42(3)

        end do
        end do
        end do


      else
       

c      ***** not rectangular *****
        do i3=n3a,n3b
        do i2=n2a,n2b
        do i1=n1a,n1b

          rxSq = d24(1)*(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+
     &             rz(i1,i2,i3)**2) 
          rxxyy = d14(1)*(rxx3(i1,i2,i3)+ryy3(i1,i2,i3)+
     &              rzz3(i1,i2,i3))
          
          sxSq = d24(2)*(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2+
     &            sz(i1,i2,i3)**2) 
          sxxyy = d14(2)*(sxx3(i1,i2,i3)+syy3(i1,i2,i3)+
     &             szz3(i1,i2,i3))
          
          txSq = d24(3)*(tx(i1,i2,i3)**2+ty(i1,i2,i3)**2+
     &         tz(i1,i2,i3)**2) 
          txxyy = d14(3)*(txx3(i1,i2,i3)+tyy3(i1,i2,i3)+
     &          tzz3(i1,i2,i3))
          
          rsx  = (2.*d14(1)*d14(2))*(rx(i1,i2,i3)*sx(i1,i2,i3)+
     &         ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,i3)) 
          rtx  = (2.*d14(1)*d14(3))*(rx(i1,i2,i3)*tx(i1,i2,i3)+
     &         ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3)) 
          stx  = (2.*d14(2)*d14(3))*(sx(i1,i2,i3)*tx(i1,i2,i3)+
     &         sy(i1,i2,i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3)) 

	
          rsx8  = rsx*8. 
          rsx64 = rsx*64.
          rtx8  = rtx*8. 
          rtx64 = rtx*64.
          stx8  = stx*8. 
          stx64 = stx*64.

          coeff(m3(-2,-2,-2),i1,i2,i3)=0.
          coeff(m3(-1,-2,-2),i1,i2,i3)=0.
          coeff(m3( 0,-2,-2),i1,i2,i3)=     stx
          coeff(m3( 1,-2,-2),i1,i2,i3)=0.
          coeff(m3( 2,-2,-2),i1,i2,i3)=0.

          coeff(m3(-2,-1,-2),i1,i2,i3)=0.
          coeff(m3(-1,-1,-2),i1,i2,i3)=0.
          coeff(m3( 0,-1,-2),i1,i2,i3)=  - stx8 
          coeff(m3( 1,-1,-2),i1,i2,i3)=0.
          coeff(m3( 2,-1,-2),i1,i2,i3)=0.

          coeff(m3(-2, 0,-2),i1,i2,i3)=     rtx
          coeff(m3(-1, 0,-2),i1,i2,i3)= -  rtx8 
          coeff(m3( 0, 0,-2),i1,i2,i3)=         -txSq    +txxyy
          coeff(m3(+1, 0,-2),i1,i2,i3)=    rtx8 
          coeff(m3(+2, 0,-2),i1,i2,i3)=    -rtx

          coeff(m3(-2, 1,-2),i1,i2,i3)=0.
          coeff(m3(-1, 1,-2),i1,i2,i3)=0.
          coeff(m3( 0, 1,-2),i1,i2,i3)=    stx8 
          coeff(m3( 1, 1,-2),i1,i2,i3)=0.
          coeff(m3( 2, 1,-2),i1,i2,i3)=0.

          coeff(m3(-2, 2,-2),i1,i2,i3)=0.
          coeff(m3(-1, 2,-2),i1,i2,i3)=0.
          coeff(m3( 0, 2,-2),i1,i2,i3)=    -stx
          coeff(m3( 1, 2,-2),i1,i2,i3)=0.
          coeff(m3( 2, 2,-2),i1,i2,i3)=0.


          coeff(m3(-2,-2,-1),i1,i2,i3)= 0.
          coeff(m3(-1,-2,-1),i1,i2,i3)= 0.
          coeff(m3( 0,-2,-1),i1,i2,i3)= -  stx8
          coeff(m3(+1,-2,-1),i1,i2,i3)= 0.
          coeff(m3(+2,-2,-1),i1,i2,i3)= 0.

          coeff(m3(-2,-1,-1),i1,i2,i3)= 0.
          coeff(m3(-1,-1,-1),i1,i2,i3)= 0.
          coeff(m3( 0,-1,-1),i1,i2,i3)=   stx64 
          coeff(m3(+1,-1,-1),i1,i2,i3)= 0.
          coeff(m3(+2,-1,-1),i1,i2,i3)= 0.

          coeff(m3(-2, 0,-1),i1,i2,i3)= -  rtx8
          coeff(m3(-1, 0,-1),i1,i2,i3)=   rtx64 
          coeff(m3( 0, 0,-1),i1,i2,i3)=           16.*txSq -8.*txxyy
          coeff(m3(+1, 0,-1),i1,i2,i3)=-  rtx64 
          coeff(m3(+2, 0,-1),i1,i2,i3)=    rtx8

          coeff(m3(-2,+1,-1),i1,i2,i3)= 0.
          coeff(m3(-1,+1,-1),i1,i2,i3)= 0.
          coeff(m3( 0,+1,-1),i1,i2,i3)=-  stx64 
          coeff(m3(+1,+1,-1),i1,i2,i3)= 0.
          coeff(m3(+2,+1,-1),i1,i2,i3)= 0.


          coeff(m3(-2,+2,-1),i1,i2,i3)= 0.
          coeff(m3(-1,+2,-1),i1,i2,i3)= 0.
          coeff(m3( 0,+2,-1),i1,i2,i3)=    stx8
          coeff(m3(+1,+2,-1),i1,i2,i3)= 0.
          coeff(m3(+2,+2,-1),i1,i2,i3)= 0.


          coeff(m3(-2,-2, 0),i1,i2,i3)=     rsx
          coeff(m3(-1,-2, 0),i1,i2,i3)= -  rsx8 
          coeff(m3( 0,-2, 0),i1,i2,i3)=               -sxSq    +sxxyy
          coeff(m3(+1,-2, 0),i1,i2,i3)=    rsx8 
          coeff(m3(+2,-2, 0),i1,i2,i3)=    -rsx


          coeff(m3(-2,-1, 0),i1,i2,i3)= -  rsx8
          coeff(m3(-1,-1, 0),i1,i2,i3)=   rsx64 
          coeff(m3( 0,-1, 0),i1,i2,i3)=            16.*sxSq -8.*sxxyy
          coeff(m3(+1,-1, 0),i1,i2,i3)=-  rsx64 
          coeff(m3(+2,-1, 0),i1,i2,i3)=    rsx8

          coeff(m3(-2, 0, 0),i1,i2,i3)=    -rxSq    +rxxyy
          coeff(m3(-1, 0, 0),i1,i2,i3)= 16.*rxSq -8.*rxxyy
          coeff(m3( 0, 0, 0),i1,i2,i3)=-30.*(rxSq   +sxSq  +   txSq )
          coeff(m3(+1, 0, 0),i1,i2,i3)= 16.*rxSq +8.*rxxyy
          coeff(m3(+2, 0, 0),i1,i2,i3)=    -rxSq    -rxxyy

          coeff(m3(-2, 1, 0),i1,i2,i3)=    rsx8
          coeff(m3(-1, 1, 0),i1,i2,i3)=-  rsx64 
          coeff(m3( 0, 1, 0),i1,i2,i3)=            16.*sxSq +8.*sxxyy
          coeff(m3(+1, 1, 0),i1,i2,i3)=   rsx64 
          coeff(m3(+2, 1, 0),i1,i2,i3)= -  rsx8

          coeff(m3(-2, 2, 0),i1,i2,i3)=    -rsx
          coeff(m3(-1, 2, 0),i1,i2,i3)=    rsx8 
          coeff(m3( 0, 2, 0),i1,i2,i3)=               -sxSq    -sxxyy
          coeff(m3(+1, 2, 0),i1,i2,i3)= -  rsx8 
          coeff(m3(+2, 2, 0),i1,i2,i3)=     rsx


          coeff(m3(-2,-2, 1),i1,i2,i3)= 0.
          coeff(m3(-1,-2, 1),i1,i2,i3)= 0.
          coeff(m3( 0,-2, 1),i1,i2,i3)=    stx8
          coeff(m3(+1,-2, 1),i1,i2,i3)= 0.
          coeff(m3(+2,-2, 1),i1,i2,i3)= 0.

          coeff(m3(-2,-1, 1),i1,i2,i3)= 0.
          coeff(m3(-1,-1, 1),i1,i2,i3)= 0.
          coeff(m3( 0,-1, 1),i1,i2,i3)=-  stx64 
          coeff(m3(+1,-1, 1),i1,i2,i3)= 0.
          coeff(m3(+2,-1, 1),i1,i2,i3)= 0.

          coeff(m3(-2, 0, 1),i1,i2,i3)=    rtx8
          coeff(m3(-1, 0, 1),i1,i2,i3)=-  rtx64 
          coeff(m3( 0, 0, 1),i1,i2,i3)=            16.*txSq +8.*txxyy
          coeff(m3(+1, 0, 1),i1,i2,i3)=   rtx64 
          coeff(m3(+2, 0, 1),i1,i2,i3)= -  rtx8

          coeff(m3(-2, 1, 1),i1,i2,i3)= 0.
          coeff(m3(-1, 1, 1),i1,i2,i3)= 0.
          coeff(m3( 0, 1, 1),i1,i2,i3)=   stx64 
          coeff(m3(+1, 1, 1),i1,i2,i3)= 0.
          coeff(m3(+2, 1, 1),i1,i2,i3)= 0.

          coeff(m3(-2, 2, 1),i1,i2,i3)= 0.
          coeff(m3(-1, 2, 1),i1,i2,i3)= 0.
          coeff(m3( 0, 2, 1),i1,i2,i3)= -  stx8
          coeff(m3(+1, 2, 1),i1,i2,i3)= 0.
          coeff(m3(+2, 2, 1),i1,i2,i3)= 0.


          coeff(m3(-2,-2, 2),i1,i2,i3)= 0.
          coeff(m3(-1,-2, 2),i1,i2,i3)= 0.
          coeff(m3( 0,-2, 2),i1,i2,i3)=    -stx
          coeff(m3(+1,-2, 2),i1,i2,i3)= 0.
          coeff(m3(+2,-2, 2),i1,i2,i3)= 0.

          coeff(m3(-2,-1, 2),i1,i2,i3)= 0.
          coeff(m3(-1,-1, 2),i1,i2,i3)= 0.
          coeff(m3( 0,-1, 2),i1,i2,i3)=    stx8 
          coeff(m3(+1,-1, 2),i1,i2,i3)= 0.
          coeff(m3(+2,-1, 2),i1,i2,i3)= 0.

          coeff(m3(-2, 0, 2),i1,i2,i3)=    -rtx
          coeff(m3(-1, 0, 2),i1,i2,i3)=    rtx8 
          coeff(m3( 0, 0, 2),i1,i2,i3)=              -txSq    -txxyy
          coeff(m3(+1, 0, 2),i1,i2,i3)= -  rtx8 
          coeff(m3(+2, 0, 2),i1,i2,i3)=     rtx

          coeff(m3(-2,+1, 2),i1,i2,i3)= 0.
          coeff(m3(-1,+1, 2),i1,i2,i3)= 0.
          coeff(m3( 0,+1, 2),i1,i2,i3)= -  stx8 
          coeff(m3(+1,+1, 2),i1,i2,i3)= 0.
          coeff(m3(+2,+1, 2),i1,i2,i3)= 0.

          coeff(m3(-2,+2, 2),i1,i2,i3)= 0.
          coeff(m3(-1,+2, 2),i1,i2,i3)= 0.
          coeff(m3( 0,+2, 2),i1,i2,i3)=     stx
          coeff(m3(+1,+2, 2),i1,i2,i3)= 0.
          coeff(m3(+2,+2, 2),i1,i2,i3)= 0.

        end do
        end do
        end do

      end if


      else
c       ************************
c       ******* 1D *************      
c       ************************


      m12=1+ec 
      m22=2+ec 
      m32=3+ec 
      m42=4+ec 
      m52=5+ec 

      if( gridType .eq. 0 )then
c       rectangular
        do i3=n3a,n3b
        do i2=n2a,n2b
        do i1=n1a,n1b

          coeff(m12,i1,i2,i3)=    -h42(1)
          coeff(m22,i1,i2,i3)= 16.*h42(1)
          coeff(m32,i1,i2,i3)=-30.*h42(1)
          coeff(m42,i1,i2,i3)= 16.*h42(1)
          coeff(m52,i1,i2,i3)=    -h42(1)

        end do
        end do
        end do

      else

c      ***** not rectangular *****
        do i3=n3a,n3b
        do i2=n2a,n2b
        do i1=n1a,n1b

          rxSq=d24(1)*rx(i1,i2,i3)**2
          rxxyy=d14(1)*rxx1(i1,i2,i3)

          coeff(m12,i1,i2,i3)=    -rxSq    +rxxyy
          coeff(m22,i1,i2,i3)= 16.*rxSq -8.*rxxyy
          coeff(m32,i1,i2,i3)=-30.*(rxSq )
          coeff(m42,i1,i2,i3)= 16.*rxSq +8.*rxxyy
          coeff(m52,i1,i2,i3)=    -rxSq    -rxxyy

        end do
        end do
        end do

      end if

      end if

      end do  ! end c
      end do  ! end e


      if( nd.eq.0 )then
c       *** add these lines to avoid warnings about unused statement functions
        include "cgux4afNoWarnings.h" 

      end if

      return
      end






      subroutine identityCoeff( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &    n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb, ca,cb, coeff )
c ===============================================================
c  Identity operator
c  
c ===============================================================

      implicit none
      integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &        n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, ea,eb, ca,cb

      real coeff(1:ndc,nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)

      integer i1,i2,i3,e,c,ec
      integer m11,m12,m13,m21,m22,m23,m31,m32,m33
      integer m111,m211,m311,m121,m221,m321,m131,m231,m331
      integer m112,m212,m312,m122,m222,m322,m132,m232,m332
      integer m113,m213,m313,m123,m223,m323,m133,m233,m333

c     ***** loop over equations and components *****
      do e=ea,eb
        do c=ca,cb
          ec=ns*(c+nc*e)

c      write(*,*) 'identityCoeff: e,c,ec=',e,c,ec
      if( nd .eq. 2 )then
c       ************************
c       ******* 2D *************      
c       ************************


      m11=1+ec ! MCE(-1,-1, 0)
      m21=2+ec ! MCE( 0,-1, 0)
      m31=3+ec ! MCE(+1,-1, 0)
      m12=4+ec ! MCE(-1, 0, 0)
      m22=5+ec ! MCE( 0, 0, 0)
      m32=6+ec ! MCE(+1, 0, 0)
      m13=7+ec ! MCE(-1,+1, 0)
      m23=8+ec ! MCE( 0,+1, 0)
      m33=9+ec ! MCE(+1,+1, 0)
      

      do i3=n3a,n3b
      do i2=n2a,n2b
      do i1=n1a,n1b

        coeff(m11,i1,i2,i3)=0.
        coeff(m21,i1,i2,i3)=0.
        coeff(m31,i1,i2,i3)=0.
        coeff(m12,i1,i2,i3)=0.
        coeff(m22,i1,i2,i3)=1. 
        coeff(m32,i1,i2,i3)=0.
        coeff(m13,i1,i2,i3)=0.
        coeff(m23,i1,i2,i3)=0.
        coeff(m33,i1,i2,i3)=0.

      end do
      end do
      end do

      elseif( nd.eq.3 )then
c       ************************
c       ******* 3D *************      
c       ************************
      m111=1+ec 
      m211=2+ec 
      m311=3+ec 
      m121=4 +ec
      m221=5 +ec
      m321=6 +ec
      m131=7 +ec
      m231=8 +ec
      m331=9 +ec
      m112=10+ec
      m212=11+ec
      m312=12+ec
      m122=13+ec
      m222=14+ec
      m322=15+ec 
      m132=16+ec
      m232=17+ec
      m332=18+ec
      m113=19+ec
      m213=20+ec
      m313=21+ec
      m123=22+ec
      m223=23+ec
      m323=24+ec 
      m133=25+ec
      m233=26+ec
      m333=27+ec
      

c       rectangular
        do i3=n3a,n3b
        do i2=n2a,n2b
        do i1=n1a,n1b

          coeff(m111,i1,i2,i3)=0.
          coeff(m211,i1,i2,i3)=0.
          coeff(m311,i1,i2,i3)=0.
          coeff(m121,i1,i2,i3)=0.
          coeff(m221,i1,i2,i3)=0.
          coeff(m321,i1,i2,i3)=0.
          coeff(m131,i1,i2,i3)=0.
          coeff(m231,i1,i2,i3)=0.
          coeff(m331,i1,i2,i3)=0.
          coeff(m112,i1,i2,i3)=0.
          coeff(m212,i1,i2,i3)=0.
          coeff(m312,i1,i2,i3)=0.
          coeff(m122,i1,i2,i3)=0.
          coeff(m222,i1,i2,i3)=1.
          coeff(m322,i1,i2,i3)=0. 
          coeff(m132,i1,i2,i3)=0.
          coeff(m232,i1,i2,i3)=0. 
          coeff(m332,i1,i2,i3)=0.
          coeff(m113,i1,i2,i3)=0.
          coeff(m213,i1,i2,i3)=0.
          coeff(m313,i1,i2,i3)=0.
          coeff(m123,i1,i2,i3)=0.
          coeff(m223,i1,i2,i3)=0. 
          coeff(m323,i1,i2,i3)=0.
          coeff(m133,i1,i2,i3)=0.
          coeff(m233,i1,i2,i3)=0.
          coeff(m333,i1,i2,i3)=0.


        end do
        end do
        end do

      else
c       ************************
c       ******* 1D *************      
c       ************************

        m12=1+ec ! MCE(-1,0, 0)
        m22=2+ec ! MCE( 0,0, 0)
        m32=3+ec ! MCE(+1,0, 0)

        do i3=n3a,n3b
        do i2=n2a,n2b
        do i1=n1a,n1b

          coeff(m12,i1,i2,i3)=0.
          coeff(m22,i1,i2,i3)=1. 
          coeff(m32,i1,i2,i3)=0.

        end do
        end do
        end do

      end if

      end do  ! end c
      end do  ! end e


      return
      end


