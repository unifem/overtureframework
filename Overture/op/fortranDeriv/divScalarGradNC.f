! This file automatically generated from dsg.bf with bpp.
! nonConservative(divScalarGradNC)
       subroutine divScalarGradNC( nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b, ndd1a,ndd1b,
     & ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b, n1a,n1b,n2a,n2b,n3a,n3b, 
     & ca,cb, h21,d22,d12,h22,d14,d24,h41,h42,rsxy,u,s,deriv,
     & derivOption,gridType,order,averagingType,dir1,dir2 )
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
       integer nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,ndu1a,ndu1b,ndu2a,
     & ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,
     & ndd3b,ndd4a,ndd4b,n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,derivOption, 
     & gridType, order, averagingType, dir1, dir2
       integer laplace,divScalarGrad,derivativeScalarDerivative,
     & divTensorGrad
       parameter(laplace=0,divScalarGrad=1,
     & derivativeScalarDerivative=2,divTensorGrad=3)
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
       sx22(i1,i2,i3)= rx(i1,i2,i3)*sr2(i1,i2,i3)+sx(i1,i2,i3)*ss2(i1,
     & i2,i3)
       sy22(i1,i2,i3)= ry(i1,i2,i3)*sr2(i1,i2,i3)+sy(i1,i2,i3)*ss2(i1,
     & i2,i3)
       sx23(i1,i2,i3)=rx(i1,i2,i3)*sr2(i1,i2,i3)+sx(i1,i2,i3)*ss2(i1,
     & i2,i3)+tx(i1,i2,i3)*st2(i1,i2,i3)
       sy23(i1,i2,i3)=ry(i1,i2,i3)*sr2(i1,i2,i3)+sy(i1,i2,i3)*ss2(i1,
     & i2,i3)+ty(i1,i2,i3)*st2(i1,i2,i3)
       sz23(i1,i2,i3)=rz(i1,i2,i3)*sr2(i1,i2,i3)+sz(i1,i2,i3)*ss2(i1,
     & i2,i3)+tz(i1,i2,i3)*st2(i1,i2,i3)
       sx22r(i1,i2,i3)=(s(i1+1,i2,i3,0)-s(i1-1,i2,i3,0))*h21(1)
       sy22r(i1,i2,i3)=(s(i1,i2+1,i3,0)-s(i1,i2-1,i3,0))*h21(2)
       sz22r(i1,i2,i3)=(s(i1,i2,i3+1,0)-s(i1,i2,i3-1,0))*h21(3)
       sr(i1,i2,i3)=(8.*(s(i1+1,i2,i3,0)-s(i1-1,i2,i3,0))-(s(i1+2,i2,
     & i3,0)-s(i1-2,i2,i3,0)))*d14(1)
       ss(i1,i2,i3)=(8.*(s(i1,i2+1,i3,0)-s(i1,i2-1,i3,0))-(s(i1,i2+2,
     & i3,0)-s(i1,i2-2,i3,0)))*d14(2)
       st(i1,i2,i3)=(8.*(s(i1,i2,i3+1,0)-s(i1,i2,i3-1,0))-(s(i1,i2,i3+
     & 2,0)-s(i1,i2,i3-2,0)))*d14(3)
       sx41(i1,i2,i3)= rx(i1,i2,i3)*sr(i1,i2,i3)
       sx42(i1,i2,i3)= rx(i1,i2,i3)*sr(i1,i2,i3)+sx(i1,i2,i3)*ss(i1,i2,
     & i3)
       sy42(i1,i2,i3)= ry(i1,i2,i3)*sr(i1,i2,i3) +sy(i1,i2,i3)*ss(i1,
     & i2,i3)
       sx43(i1,i2,i3)=rx(i1,i2,i3)*sr(i1,i2,i3)+sx(i1,i2,i3)*ss(i1,i2,
     & i3)+tx(i1,i2,i3)*st(i1,i2,i3)
        sy43(i1,i2,i3)=ry(i1,i2,i3)*sr(i1,i2,i3)+sy(i1,i2,i3)*ss(i1,i2,
     & i3)+ty(i1,i2,i3)*st(i1,i2,i3)
       sz43(i1,i2,i3)=rz(i1,i2,i3)*sr(i1,i2,i3)+sz(i1,i2,i3)*ss(i1,i2,
     & i3)+tz(i1,i2,i3)*st(i1,i2,i3)
       sx42r(i1,i2,i3)=(8.*(s(i1+1,i2,i3,0)-s(i1-1,i2,i3,0))-(s(i1+2,
     & i2,i3,0)-s(i1-2,i2,i3,0)))*h41(1)
       sy42r(i1,i2,i3)=(8.*(s(i1,i2+1,i3,0)-s(i1,i2-1,i3,0))-(s(i1,i2+
     & 2,i3,0)-s(i1,i2-2,i3,0)))*h41(2)
       sz42r(i1,i2,i3)=(8.*(s(i1,i2,i3+1,0)-s(i1,i2,i3-1,0))-(s(i1,i2,
     & i3+2,0)-s(i1,i2,i3-2,0)))*h41(3)
       sy21(i1,i2,i3)=0.
       sz21(i1,i2,i3)=0.
       sz42(i1,i2,i3)=0.
       sz22(i1,i2,i3)=0.
       sz41(i1,i2,i3)=0.
       sy41(i1,i2,i3)=0.
c......end statement function
       kd3=nd
! #If "divScalarGradNC" == "divScalarGradNC"
c       ****** divScalarGrad ******
       if( nd .eq. 2 )then
c         ******* 2D *************      
         if( gridType .eq. 0 )then
c           rectangular
           if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*LAPLACIAN22R(i1,i2,i3,c)+SX22R(i1,i2,i3)*UX22R(i1,i2,i3,c)+SY22R(i1,i2,i3)*UY22R(i1,i2,i3,c))
             do c=ca,cb
               do i3=n3a,n3b
                 do i2=n2a,n2b
                   do i1=n1a,n1b
                     deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*LAPLACIAN22R(i1,
     & i2,i3,c)+SX22R(i1,i2,i3)*UX22R(i1,i2,i3,c)+SY22R(i1,i2,i3)*
     & UY22R(i1,i2,i3,c)
                   end do
                 end do
               end do
             end do
           else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*LAPLACIAN42R(i1,i2,i3,c) +SX42R(i1,i2,i3)*UX42R(i1,i2,i3,c)+SY42R(i1,i2,i3)*UY42R(i1,i2,i3,c))
             do c=ca,cb
               do i3=n3a,n3b
                 do i2=n2a,n2b
                   do i1=n1a,n1b
                     deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*LAPLACIAN42R(i1,
     & i2,i3,c)+SX42R(i1,i2,i3)*UX42R(i1,i2,i3,c)+SY42R(i1,i2,i3)*
     & UY42R(i1,i2,i3,c)
                   end do
                 end do
               end do
             end do
           end if
         else
c           ***** not rectangular *****
           if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*LAPLACIAN22(i1,i2,i3,c)+SX22(i1,i2,i3)*UX22(i1,i2,i3,c)+SY22(i1,i2,i3)*UY22(i1,i2,i3,c))
             do c=ca,cb
               do i3=n3a,n3b
                 do i2=n2a,n2b
                   do i1=n1a,n1b
                     deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*LAPLACIAN22(i1,i2,
     & i3,c)+SX22(i1,i2,i3)*UX22(i1,i2,i3,c)+SY22(i1,i2,i3)*UY22(i1,
     & i2,i3,c)
                   end do
                 end do
               end do
             end do
           else
! loopsDSG(deriv(i1,i2,i3,c)= s(i1,i2,i3,0)*LAPLACIAN42(i1,i2,i3,c)+SX42(i1,i2,i3)*UX42(i1,i2,i3,c)+SY42(i1,i2,i3)*UY42(i1,i2,i3,c))
             do c=ca,cb
               do i3=n3a,n3b
                 do i2=n2a,n2b
                   do i1=n1a,n1b
                     deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*LAPLACIAN42(i1,i2,
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
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*LAPLACIAN23R(i1,i2,i3,c)+SX22R(i1,i2,i3)*UX23R(i1,i2,i3,c)+SY22R(i1,i2,i3)*UY23R(i1,i2,i3,c)+SZ22R(i1,i2,i3)*UZ23R(i1,i2,i3,c))
             do c=ca,cb
               do i3=n3a,n3b
                 do i2=n2a,n2b
                   do i1=n1a,n1b
                     deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*LAPLACIAN23R(i1,
     & i2,i3,c)+SX22R(i1,i2,i3)*UX23R(i1,i2,i3,c)+SY22R(i1,i2,i3)*
     & UY23R(i1,i2,i3,c)+SZ22R(i1,i2,i3)*UZ23R(i1,i2,i3,c)
                   end do
                 end do
               end do
             end do
           else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*LAPLACIAN43R(i1,i2,i3,c)+SX42R(i1,i2,i3)*UX42R(i1,i2,i3,c)+SY42R(i1,i2,i3)*UY42R(i1,i2,i3,c)+SZ42R(i1,i2,i3)*UZ42R(i1,i2,i3,c))
             do c=ca,cb
               do i3=n3a,n3b
                 do i2=n2a,n2b
                   do i1=n1a,n1b
                     deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*LAPLACIAN43R(i1,
     & i2,i3,c)+SX42R(i1,i2,i3)*UX42R(i1,i2,i3,c)+SY42R(i1,i2,i3)*
     & UY42R(i1,i2,i3,c)+SZ42R(i1,i2,i3)*UZ42R(i1,i2,i3,c)
                   end do
                 end do
               end do
             end do
           end if
         else
c           ***** not rectangular *****
           if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*LAPLACIAN23(i1,i2,i3,c)+SX23(i1,i2,i3)*UX23(i1,i2,i3,c)+SY23(i1,i2,i3)*UY23(i1,i2,i3,c)+SZ23(i1,i2,i3)*UZ23(i1,i2,i3,c))
             do c=ca,cb
               do i3=n3a,n3b
                 do i2=n2a,n2b
                   do i1=n1a,n1b
                     deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*LAPLACIAN23(i1,i2,
     & i3,c)+SX23(i1,i2,i3)*UX23(i1,i2,i3,c)+SY23(i1,i2,i3)*UY23(i1,
     & i2,i3,c)+SZ23(i1,i2,i3)*UZ23(i1,i2,i3,c)
                   end do
                 end do
               end do
             end do
           else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*LAPLACIAN43(i1,i2,i3,c)+SX43(i1,i2,i3)*UX43(i1,i2,i3,c)+SY43(i1,i2,i3)*UY43(i1,i2,i3,c)+SZ43(i1,i2,i3)*UZ43(i1,i2,i3,c))
             do c=ca,cb
               do i3=n3a,n3b
                 do i2=n2a,n2b
                   do i1=n1a,n1b
                     deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*LAPLACIAN43(i1,i2,
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
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*LAPLACIAN21R(i1,i2,i3,c)+SX22R(i1,i2,i3)*UX22R(i1,i2,i3,c))
             do c=ca,cb
               do i3=n3a,n3b
                 do i2=n2a,n2b
                   do i1=n1a,n1b
                     deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*LAPLACIAN21R(i1,
     & i2,i3,c)+SX22R(i1,i2,i3)*UX22R(i1,i2,i3,c)
                   end do
                 end do
               end do
             end do
           else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*LAPLACIAN41R(i1,i2,i3,c)+SX42R(i1,i2,i3)*UX42R(i1,i2,i3,c))
             do c=ca,cb
               do i3=n3a,n3b
                 do i2=n2a,n2b
                   do i1=n1a,n1b
                     deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*LAPLACIAN41R(i1,
     & i2,i3,c)+SX42R(i1,i2,i3)*UX42R(i1,i2,i3,c)
                   end do
                 end do
               end do
             end do
           end if
         else
c            ***** not rectangular *****
           if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*LAPLACIAN21(i1,i2,i3,c)+SX21(i1,i2,i3)*UX21(i1,i2,i3,c))
             do c=ca,cb
               do i3=n3a,n3b
                 do i2=n2a,n2b
                   do i1=n1a,n1b
                     deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*LAPLACIAN21(i1,i2,
     & i3,c)+SX21(i1,i2,i3)*UX21(i1,i2,i3,c)
                   end do
                 end do
               end do
             end do
           else
! loopsDSG(deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*LAPLACIAN41(i1,i2,i3,c)+SX41(i1,i2,i3)*UX41(i1,i2,i3,c))
             do c=ca,cb
               do i3=n3a,n3b
                 do i2=n2a,n2b
                   do i1=n1a,n1b
                     deriv(i1,i2,i3,c)=s(i1,i2,i3,0)*LAPLACIAN41(i1,i2,
     & i3,c)+SX41(i1,i2,i3)*UX41(i1,i2,i3,c)
                   end do
                 end do
               end do
             end do
           end if
         endif
       end if
       if( nd.eq.0 )then
c       *** add these lines to avoid warnings about unused statement functions
         include "cgux2afNoWarnings.h"
         include "cgux4afNoWarnings.h"
       end if
       return
       end
! nonConservativeNew(divScalarGradNC)
       subroutine divScalarGradNCNew( nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b, ndd1a,ndd1b,
     & ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b, n1a,n1b,n2a,n2b,n3a,n3b, 
     & ca,cb, dr,dx,rsxy,u,sc,deriv,derivOption,gridType,order,
     & averagingType,dir1,dir2 )
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
       integer nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,ndu1a,ndu1b,ndu2a,
     & ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,
     & ndd3b,ndd4a,ndd4b,n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,derivOption, 
     & gridType, order, averagingType, dir1, dir2
       integer laplace,divScalarGrad,derivativeScalarDerivative,
     & divTensorGrad
       parameter(laplace=0,divScalarGrad=1,
     & derivativeScalarDerivative=2,divTensorGrad=3)
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
! defineDifferenceOrder2Components1(u,RX)
! #If "RX" == "RX"
        d12(kd) = 1./(2.*dr(kd))
        d22(kd) = 1./(dr(kd)**2)
        ur2(i1,i2,i3,kd)=(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))*d12(0)
        us2(i1,i2,i3,kd)=(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))*d12(1)
        ut2(i1,i2,i3,kd)=(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))*d12(2)
        urr2(i1,i2,i3,kd)=(-2.*u(i1,i2,i3,kd)+(u(i1+1,i2,i3,kd)+u(i1-1,
     & i2,i3,kd)) )*d22(0)
        uss2(i1,i2,i3,kd)=(-2.*u(i1,i2,i3,kd)+(u(i1,i2+1,i3,kd)+u(i1,
     & i2-1,i3,kd)) )*d22(1)
        urs2(i1,i2,i3,kd)=(ur2(i1,i2+1,i3,kd)-ur2(i1,i2-1,i3,kd))*d12(
     & 1)
        utt2(i1,i2,i3,kd)=(-2.*u(i1,i2,i3,kd)+(u(i1,i2,i3+1,kd)+u(i1,
     & i2,i3-1,kd)) )*d22(2)
        urt2(i1,i2,i3,kd)=(ur2(i1,i2,i3+1,kd)-ur2(i1,i2,i3-1,kd))*d12(
     & 2)
        ust2(i1,i2,i3,kd)=(us2(i1,i2,i3+1,kd)-us2(i1,i2,i3-1,kd))*d12(
     & 2)
        urrr2(i1,i2,i3,kd)=(-2.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))+(u(
     & i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd)) )*d22(0)*d12(0)
        usss2(i1,i2,i3,kd)=(-2.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))+(u(
     & i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd)) )*d22(1)*d12(1)
        uttt2(i1,i2,i3,kd)=(-2.*(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))+(u(
     & i1,i2,i3+2,kd)-u(i1,i2,i3-2,kd)) )*d22(2)*d12(2)
! #If "RX" == "RX"
        rxr2(i1,i2,i3)=(rx(i1+1,i2,i3)-rx(i1-1,i2,i3))*d12(0)
        rxs2(i1,i2,i3)=(rx(i1,i2+1,i3)-rx(i1,i2-1,i3))*d12(1)
        rxt2(i1,i2,i3)=(rx(i1,i2,i3+1)-rx(i1,i2,i3-1))*d12(2)
        rxrr2(i1,i2,i3)=(-2.*rx(i1,i2,i3)+(rx(i1+1,i2,i3)+rx(i1-1,i2,
     & i3)) )*d22(0)
        rxss2(i1,i2,i3)=(-2.*rx(i1,i2,i3)+(rx(i1,i2+1,i3)+rx(i1,i2-1,
     & i3)) )*d22(1)
        rxrs2(i1,i2,i3)=(rxr2(i1,i2+1,i3)-rxr2(i1,i2-1,i3))*d12(1)
        ryr2(i1,i2,i3)=(ry(i1+1,i2,i3)-ry(i1-1,i2,i3))*d12(0)
        rys2(i1,i2,i3)=(ry(i1,i2+1,i3)-ry(i1,i2-1,i3))*d12(1)
        ryt2(i1,i2,i3)=(ry(i1,i2,i3+1)-ry(i1,i2,i3-1))*d12(2)
        ryrr2(i1,i2,i3)=(-2.*ry(i1,i2,i3)+(ry(i1+1,i2,i3)+ry(i1-1,i2,
     & i3)) )*d22(0)
        ryss2(i1,i2,i3)=(-2.*ry(i1,i2,i3)+(ry(i1,i2+1,i3)+ry(i1,i2-1,
     & i3)) )*d22(1)
        ryrs2(i1,i2,i3)=(ryr2(i1,i2+1,i3)-ryr2(i1,i2-1,i3))*d12(1)
        rzr2(i1,i2,i3)=(rz(i1+1,i2,i3)-rz(i1-1,i2,i3))*d12(0)
        rzs2(i1,i2,i3)=(rz(i1,i2+1,i3)-rz(i1,i2-1,i3))*d12(1)
        rzt2(i1,i2,i3)=(rz(i1,i2,i3+1)-rz(i1,i2,i3-1))*d12(2)
        rzrr2(i1,i2,i3)=(-2.*rz(i1,i2,i3)+(rz(i1+1,i2,i3)+rz(i1-1,i2,
     & i3)) )*d22(0)
        rzss2(i1,i2,i3)=(-2.*rz(i1,i2,i3)+(rz(i1,i2+1,i3)+rz(i1,i2-1,
     & i3)) )*d22(1)
        rzrs2(i1,i2,i3)=(rzr2(i1,i2+1,i3)-rzr2(i1,i2-1,i3))*d12(1)
        sxr2(i1,i2,i3)=(sx(i1+1,i2,i3)-sx(i1-1,i2,i3))*d12(0)
        sxs2(i1,i2,i3)=(sx(i1,i2+1,i3)-sx(i1,i2-1,i3))*d12(1)
        sxt2(i1,i2,i3)=(sx(i1,i2,i3+1)-sx(i1,i2,i3-1))*d12(2)
        sxrr2(i1,i2,i3)=(-2.*sx(i1,i2,i3)+(sx(i1+1,i2,i3)+sx(i1-1,i2,
     & i3)) )*d22(0)
        sxss2(i1,i2,i3)=(-2.*sx(i1,i2,i3)+(sx(i1,i2+1,i3)+sx(i1,i2-1,
     & i3)) )*d22(1)
        sxrs2(i1,i2,i3)=(sxr2(i1,i2+1,i3)-sxr2(i1,i2-1,i3))*d12(1)
        syr2(i1,i2,i3)=(sy(i1+1,i2,i3)-sy(i1-1,i2,i3))*d12(0)
        sys2(i1,i2,i3)=(sy(i1,i2+1,i3)-sy(i1,i2-1,i3))*d12(1)
        syt2(i1,i2,i3)=(sy(i1,i2,i3+1)-sy(i1,i2,i3-1))*d12(2)
        syrr2(i1,i2,i3)=(-2.*sy(i1,i2,i3)+(sy(i1+1,i2,i3)+sy(i1-1,i2,
     & i3)) )*d22(0)
        syss2(i1,i2,i3)=(-2.*sy(i1,i2,i3)+(sy(i1,i2+1,i3)+sy(i1,i2-1,
     & i3)) )*d22(1)
        syrs2(i1,i2,i3)=(syr2(i1,i2+1,i3)-syr2(i1,i2-1,i3))*d12(1)
        szr2(i1,i2,i3)=(sz(i1+1,i2,i3)-sz(i1-1,i2,i3))*d12(0)
        szs2(i1,i2,i3)=(sz(i1,i2+1,i3)-sz(i1,i2-1,i3))*d12(1)
        szt2(i1,i2,i3)=(sz(i1,i2,i3+1)-sz(i1,i2,i3-1))*d12(2)
        szrr2(i1,i2,i3)=(-2.*sz(i1,i2,i3)+(sz(i1+1,i2,i3)+sz(i1-1,i2,
     & i3)) )*d22(0)
        szss2(i1,i2,i3)=(-2.*sz(i1,i2,i3)+(sz(i1,i2+1,i3)+sz(i1,i2-1,
     & i3)) )*d22(1)
        szrs2(i1,i2,i3)=(szr2(i1,i2+1,i3)-szr2(i1,i2-1,i3))*d12(1)
        txr2(i1,i2,i3)=(tx(i1+1,i2,i3)-tx(i1-1,i2,i3))*d12(0)
        txs2(i1,i2,i3)=(tx(i1,i2+1,i3)-tx(i1,i2-1,i3))*d12(1)
        txt2(i1,i2,i3)=(tx(i1,i2,i3+1)-tx(i1,i2,i3-1))*d12(2)
        txrr2(i1,i2,i3)=(-2.*tx(i1,i2,i3)+(tx(i1+1,i2,i3)+tx(i1-1,i2,
     & i3)) )*d22(0)
        txss2(i1,i2,i3)=(-2.*tx(i1,i2,i3)+(tx(i1,i2+1,i3)+tx(i1,i2-1,
     & i3)) )*d22(1)
        txrs2(i1,i2,i3)=(txr2(i1,i2+1,i3)-txr2(i1,i2-1,i3))*d12(1)
        tyr2(i1,i2,i3)=(ty(i1+1,i2,i3)-ty(i1-1,i2,i3))*d12(0)
        tys2(i1,i2,i3)=(ty(i1,i2+1,i3)-ty(i1,i2-1,i3))*d12(1)
        tyt2(i1,i2,i3)=(ty(i1,i2,i3+1)-ty(i1,i2,i3-1))*d12(2)
        tyrr2(i1,i2,i3)=(-2.*ty(i1,i2,i3)+(ty(i1+1,i2,i3)+ty(i1-1,i2,
     & i3)) )*d22(0)
        tyss2(i1,i2,i3)=(-2.*ty(i1,i2,i3)+(ty(i1,i2+1,i3)+ty(i1,i2-1,
     & i3)) )*d22(1)
        tyrs2(i1,i2,i3)=(tyr2(i1,i2+1,i3)-tyr2(i1,i2-1,i3))*d12(1)
        tzr2(i1,i2,i3)=(tz(i1+1,i2,i3)-tz(i1-1,i2,i3))*d12(0)
        tzs2(i1,i2,i3)=(tz(i1,i2+1,i3)-tz(i1,i2-1,i3))*d12(1)
        tzt2(i1,i2,i3)=(tz(i1,i2,i3+1)-tz(i1,i2,i3-1))*d12(2)
        tzrr2(i1,i2,i3)=(-2.*tz(i1,i2,i3)+(tz(i1+1,i2,i3)+tz(i1-1,i2,
     & i3)) )*d22(0)
        tzss2(i1,i2,i3)=(-2.*tz(i1,i2,i3)+(tz(i1,i2+1,i3)+tz(i1,i2-1,
     & i3)) )*d22(1)
        tzrs2(i1,i2,i3)=(tzr2(i1,i2+1,i3)-tzr2(i1,i2-1,i3))*d12(1)
        ux21(i1,i2,i3,kd)= rx(i1,i2,i3)*ur2(i1,i2,i3,kd)
        uy21(i1,i2,i3,kd)=0
        uz21(i1,i2,i3,kd)=0
        ux22(i1,i2,i3,kd)= rx(i1,i2,i3)*ur2(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & us2(i1,i2,i3,kd)
        uy22(i1,i2,i3,kd)= ry(i1,i2,i3)*ur2(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & us2(i1,i2,i3,kd)
        uz22(i1,i2,i3,kd)=0
        ux23(i1,i2,i3,kd)=rx(i1,i2,i3)*ur2(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & us2(i1,i2,i3,kd)+tx(i1,i2,i3)*ut2(i1,i2,i3,kd)
        uy23(i1,i2,i3,kd)=ry(i1,i2,i3)*ur2(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & us2(i1,i2,i3,kd)+ty(i1,i2,i3)*ut2(i1,i2,i3,kd)
        uz23(i1,i2,i3,kd)=rz(i1,i2,i3)*ur2(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & us2(i1,i2,i3,kd)+tz(i1,i2,i3)*ut2(i1,i2,i3,kd)
! #If "RX" == "RX"
        rxx21(i1,i2,i3)= rx(i1,i2,i3)*rxr2(i1,i2,i3)
        rxx22(i1,i2,i3)= rx(i1,i2,i3)*rxr2(i1,i2,i3)+sx(i1,i2,i3)*rxs2(
     & i1,i2,i3)
        rxy22(i1,i2,i3)= ry(i1,i2,i3)*rxr2(i1,i2,i3)+sy(i1,i2,i3)*rxs2(
     & i1,i2,i3)
        rxx23(i1,i2,i3)=rx(i1,i2,i3)*rxr2(i1,i2,i3)+sx(i1,i2,i3)*rxs2(
     & i1,i2,i3)+tx(i1,i2,i3)*rxt2(i1,i2,i3)
        rxy23(i1,i2,i3)=ry(i1,i2,i3)*rxr2(i1,i2,i3)+sy(i1,i2,i3)*rxs2(
     & i1,i2,i3)+ty(i1,i2,i3)*rxt2(i1,i2,i3)
        rxz23(i1,i2,i3)=rz(i1,i2,i3)*rxr2(i1,i2,i3)+sz(i1,i2,i3)*rxs2(
     & i1,i2,i3)+tz(i1,i2,i3)*rxt2(i1,i2,i3)
        ryx22(i1,i2,i3)= rx(i1,i2,i3)*ryr2(i1,i2,i3)+sx(i1,i2,i3)*rys2(
     & i1,i2,i3)
        ryy22(i1,i2,i3)= ry(i1,i2,i3)*ryr2(i1,i2,i3)+sy(i1,i2,i3)*rys2(
     & i1,i2,i3)
        ryx23(i1,i2,i3)=rx(i1,i2,i3)*ryr2(i1,i2,i3)+sx(i1,i2,i3)*rys2(
     & i1,i2,i3)+tx(i1,i2,i3)*ryt2(i1,i2,i3)
        ryy23(i1,i2,i3)=ry(i1,i2,i3)*ryr2(i1,i2,i3)+sy(i1,i2,i3)*rys2(
     & i1,i2,i3)+ty(i1,i2,i3)*ryt2(i1,i2,i3)
        ryz23(i1,i2,i3)=rz(i1,i2,i3)*ryr2(i1,i2,i3)+sz(i1,i2,i3)*rys2(
     & i1,i2,i3)+tz(i1,i2,i3)*ryt2(i1,i2,i3)
        rzx22(i1,i2,i3)= rx(i1,i2,i3)*rzr2(i1,i2,i3)+sx(i1,i2,i3)*rzs2(
     & i1,i2,i3)
        rzy22(i1,i2,i3)= ry(i1,i2,i3)*rzr2(i1,i2,i3)+sy(i1,i2,i3)*rzs2(
     & i1,i2,i3)
        rzx23(i1,i2,i3)=rx(i1,i2,i3)*rzr2(i1,i2,i3)+sx(i1,i2,i3)*rzs2(
     & i1,i2,i3)+tx(i1,i2,i3)*rzt2(i1,i2,i3)
        rzy23(i1,i2,i3)=ry(i1,i2,i3)*rzr2(i1,i2,i3)+sy(i1,i2,i3)*rzs2(
     & i1,i2,i3)+ty(i1,i2,i3)*rzt2(i1,i2,i3)
        rzz23(i1,i2,i3)=rz(i1,i2,i3)*rzr2(i1,i2,i3)+sz(i1,i2,i3)*rzs2(
     & i1,i2,i3)+tz(i1,i2,i3)*rzt2(i1,i2,i3)
        sxx22(i1,i2,i3)= rx(i1,i2,i3)*sxr2(i1,i2,i3)+sx(i1,i2,i3)*sxs2(
     & i1,i2,i3)
        sxy22(i1,i2,i3)= ry(i1,i2,i3)*sxr2(i1,i2,i3)+sy(i1,i2,i3)*sxs2(
     & i1,i2,i3)
        sxx23(i1,i2,i3)=rx(i1,i2,i3)*sxr2(i1,i2,i3)+sx(i1,i2,i3)*sxs2(
     & i1,i2,i3)+tx(i1,i2,i3)*sxt2(i1,i2,i3)
        sxy23(i1,i2,i3)=ry(i1,i2,i3)*sxr2(i1,i2,i3)+sy(i1,i2,i3)*sxs2(
     & i1,i2,i3)+ty(i1,i2,i3)*sxt2(i1,i2,i3)
        sxz23(i1,i2,i3)=rz(i1,i2,i3)*sxr2(i1,i2,i3)+sz(i1,i2,i3)*sxs2(
     & i1,i2,i3)+tz(i1,i2,i3)*sxt2(i1,i2,i3)
        syx22(i1,i2,i3)= rx(i1,i2,i3)*syr2(i1,i2,i3)+sx(i1,i2,i3)*sys2(
     & i1,i2,i3)
        syy22(i1,i2,i3)= ry(i1,i2,i3)*syr2(i1,i2,i3)+sy(i1,i2,i3)*sys2(
     & i1,i2,i3)
        syx23(i1,i2,i3)=rx(i1,i2,i3)*syr2(i1,i2,i3)+sx(i1,i2,i3)*sys2(
     & i1,i2,i3)+tx(i1,i2,i3)*syt2(i1,i2,i3)
        syy23(i1,i2,i3)=ry(i1,i2,i3)*syr2(i1,i2,i3)+sy(i1,i2,i3)*sys2(
     & i1,i2,i3)+ty(i1,i2,i3)*syt2(i1,i2,i3)
        syz23(i1,i2,i3)=rz(i1,i2,i3)*syr2(i1,i2,i3)+sz(i1,i2,i3)*sys2(
     & i1,i2,i3)+tz(i1,i2,i3)*syt2(i1,i2,i3)
        szx22(i1,i2,i3)= rx(i1,i2,i3)*szr2(i1,i2,i3)+sx(i1,i2,i3)*szs2(
     & i1,i2,i3)
        szy22(i1,i2,i3)= ry(i1,i2,i3)*szr2(i1,i2,i3)+sy(i1,i2,i3)*szs2(
     & i1,i2,i3)
        szx23(i1,i2,i3)=rx(i1,i2,i3)*szr2(i1,i2,i3)+sx(i1,i2,i3)*szs2(
     & i1,i2,i3)+tx(i1,i2,i3)*szt2(i1,i2,i3)
        szy23(i1,i2,i3)=ry(i1,i2,i3)*szr2(i1,i2,i3)+sy(i1,i2,i3)*szs2(
     & i1,i2,i3)+ty(i1,i2,i3)*szt2(i1,i2,i3)
        szz23(i1,i2,i3)=rz(i1,i2,i3)*szr2(i1,i2,i3)+sz(i1,i2,i3)*szs2(
     & i1,i2,i3)+tz(i1,i2,i3)*szt2(i1,i2,i3)
        txx22(i1,i2,i3)= rx(i1,i2,i3)*txr2(i1,i2,i3)+sx(i1,i2,i3)*txs2(
     & i1,i2,i3)
        txy22(i1,i2,i3)= ry(i1,i2,i3)*txr2(i1,i2,i3)+sy(i1,i2,i3)*txs2(
     & i1,i2,i3)
        txx23(i1,i2,i3)=rx(i1,i2,i3)*txr2(i1,i2,i3)+sx(i1,i2,i3)*txs2(
     & i1,i2,i3)+tx(i1,i2,i3)*txt2(i1,i2,i3)
        txy23(i1,i2,i3)=ry(i1,i2,i3)*txr2(i1,i2,i3)+sy(i1,i2,i3)*txs2(
     & i1,i2,i3)+ty(i1,i2,i3)*txt2(i1,i2,i3)
        txz23(i1,i2,i3)=rz(i1,i2,i3)*txr2(i1,i2,i3)+sz(i1,i2,i3)*txs2(
     & i1,i2,i3)+tz(i1,i2,i3)*txt2(i1,i2,i3)
        tyx22(i1,i2,i3)= rx(i1,i2,i3)*tyr2(i1,i2,i3)+sx(i1,i2,i3)*tys2(
     & i1,i2,i3)
        tyy22(i1,i2,i3)= ry(i1,i2,i3)*tyr2(i1,i2,i3)+sy(i1,i2,i3)*tys2(
     & i1,i2,i3)
        tyx23(i1,i2,i3)=rx(i1,i2,i3)*tyr2(i1,i2,i3)+sx(i1,i2,i3)*tys2(
     & i1,i2,i3)+tx(i1,i2,i3)*tyt2(i1,i2,i3)
        tyy23(i1,i2,i3)=ry(i1,i2,i3)*tyr2(i1,i2,i3)+sy(i1,i2,i3)*tys2(
     & i1,i2,i3)+ty(i1,i2,i3)*tyt2(i1,i2,i3)
        tyz23(i1,i2,i3)=rz(i1,i2,i3)*tyr2(i1,i2,i3)+sz(i1,i2,i3)*tys2(
     & i1,i2,i3)+tz(i1,i2,i3)*tyt2(i1,i2,i3)
        tzx22(i1,i2,i3)= rx(i1,i2,i3)*tzr2(i1,i2,i3)+sx(i1,i2,i3)*tzs2(
     & i1,i2,i3)
        tzy22(i1,i2,i3)= ry(i1,i2,i3)*tzr2(i1,i2,i3)+sy(i1,i2,i3)*tzs2(
     & i1,i2,i3)
        tzx23(i1,i2,i3)=rx(i1,i2,i3)*tzr2(i1,i2,i3)+sx(i1,i2,i3)*tzs2(
     & i1,i2,i3)+tx(i1,i2,i3)*tzt2(i1,i2,i3)
        tzy23(i1,i2,i3)=ry(i1,i2,i3)*tzr2(i1,i2,i3)+sy(i1,i2,i3)*tzs2(
     & i1,i2,i3)+ty(i1,i2,i3)*tzt2(i1,i2,i3)
        tzz23(i1,i2,i3)=rz(i1,i2,i3)*tzr2(i1,i2,i3)+sz(i1,i2,i3)*tzs2(
     & i1,i2,i3)+tz(i1,i2,i3)*tzt2(i1,i2,i3)
        uxx21(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*urr2(i1,i2,i3,kd)+(rxx22(
     & i1,i2,i3))*ur2(i1,i2,i3,kd)
        uyy21(i1,i2,i3,kd)=0
        uxy21(i1,i2,i3,kd)=0
        uxz21(i1,i2,i3,kd)=0
        uyz21(i1,i2,i3,kd)=0
        uzz21(i1,i2,i3,kd)=0
        ulaplacian21(i1,i2,i3,kd)=uxx21(i1,i2,i3,kd)
        uxx22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*urr2(i1,i2,i3,kd)+2.*(rx(
     & i1,i2,i3)*sx(i1,i2,i3))*urs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2)*
     & uss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3))*ur2(i1,i2,i3,kd)+(sxx22(i1,
     & i2,i3))*us2(i1,i2,i3,kd)
        uyy22(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*urr2(i1,i2,i3,kd)+2.*(ry(
     & i1,i2,i3)*sy(i1,i2,i3))*urs2(i1,i2,i3,kd)+(sy(i1,i2,i3)**2)*
     & uss2(i1,i2,i3,kd)+(ryy22(i1,i2,i3))*ur2(i1,i2,i3,kd)+(syy22(i1,
     & i2,i3))*us2(i1,i2,i3,kd)
        uxy22(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*urr2(i1,i2,i3,kd)+
     & (rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*urs2(i1,
     & i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*uss2(i1,i2,i3,kd)+rxy22(i1,
     & i2,i3)*ur2(i1,i2,i3,kd)+sxy22(i1,i2,i3)*us2(i1,i2,i3,kd)
        uxz22(i1,i2,i3,kd)=0
        uyz22(i1,i2,i3,kd)=0
        uzz22(i1,i2,i3,kd)=0
        ulaplacian22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & urr2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*
     & sy(i1,i2,i3))*urs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)**
     & 2)*uss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3)+ryy22(i1,i2,i3))*ur2(i1,
     & i2,i3,kd)+(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*us2(i1,i2,i3,kd)
        uxx23(i1,i2,i3,kd)=rx(i1,i2,i3)**2*urr2(i1,i2,i3,kd)+sx(i1,i2,
     & i3)**2*uss2(i1,i2,i3,kd)+tx(i1,i2,i3)**2*utt2(i1,i2,i3,kd)+2.*
     & rx(i1,i2,i3)*sx(i1,i2,i3)*urs2(i1,i2,i3,kd)+2.*rx(i1,i2,i3)*tx(
     & i1,i2,i3)*urt2(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,i2,i3)*ust2(
     & i1,i2,i3,kd)+rxx23(i1,i2,i3)*ur2(i1,i2,i3,kd)+sxx23(i1,i2,i3)*
     & us2(i1,i2,i3,kd)+txx23(i1,i2,i3)*ut2(i1,i2,i3,kd)
        uyy23(i1,i2,i3,kd)=ry(i1,i2,i3)**2*urr2(i1,i2,i3,kd)+sy(i1,i2,
     & i3)**2*uss2(i1,i2,i3,kd)+ty(i1,i2,i3)**2*utt2(i1,i2,i3,kd)+2.*
     & ry(i1,i2,i3)*sy(i1,i2,i3)*urs2(i1,i2,i3,kd)+2.*ry(i1,i2,i3)*ty(
     & i1,i2,i3)*urt2(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,i2,i3)*ust2(
     & i1,i2,i3,kd)+ryy23(i1,i2,i3)*ur2(i1,i2,i3,kd)+syy23(i1,i2,i3)*
     & us2(i1,i2,i3,kd)+tyy23(i1,i2,i3)*ut2(i1,i2,i3,kd)
        uzz23(i1,i2,i3,kd)=rz(i1,i2,i3)**2*urr2(i1,i2,i3,kd)+sz(i1,i2,
     & i3)**2*uss2(i1,i2,i3,kd)+tz(i1,i2,i3)**2*utt2(i1,i2,i3,kd)+2.*
     & rz(i1,i2,i3)*sz(i1,i2,i3)*urs2(i1,i2,i3,kd)+2.*rz(i1,i2,i3)*tz(
     & i1,i2,i3)*urt2(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,i2,i3)*ust2(
     & i1,i2,i3,kd)+rzz23(i1,i2,i3)*ur2(i1,i2,i3,kd)+szz23(i1,i2,i3)*
     & us2(i1,i2,i3,kd)+tzz23(i1,i2,i3)*ut2(i1,i2,i3,kd)
        uxy23(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*urr2(i1,i2,i3,kd)+
     & sx(i1,i2,i3)*sy(i1,i2,i3)*uss2(i1,i2,i3,kd)+tx(i1,i2,i3)*ty(i1,
     & i2,i3)*utt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,
     & i3)*sx(i1,i2,i3))*urs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(i1,i2,i3)+
     & ry(i1,i2,i3)*tx(i1,i2,i3))*urt2(i1,i2,i3,kd)+(sx(i1,i2,i3)*ty(
     & i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*ust2(i1,i2,i3,kd)+rxy23(
     & i1,i2,i3)*ur2(i1,i2,i3,kd)+sxy23(i1,i2,i3)*us2(i1,i2,i3,kd)+
     & txy23(i1,i2,i3)*ut2(i1,i2,i3,kd)
        uxz23(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*urr2(i1,i2,i3,kd)+
     & sx(i1,i2,i3)*sz(i1,i2,i3)*uss2(i1,i2,i3,kd)+tx(i1,i2,i3)*tz(i1,
     & i2,i3)*utt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,i2,
     & i3)*sx(i1,i2,i3))*urs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(i1,i2,i3)+
     & rz(i1,i2,i3)*tx(i1,i2,i3))*urt2(i1,i2,i3,kd)+(sx(i1,i2,i3)*tz(
     & i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*ust2(i1,i2,i3,kd)+rxz23(
     & i1,i2,i3)*ur2(i1,i2,i3,kd)+sxz23(i1,i2,i3)*us2(i1,i2,i3,kd)+
     & txz23(i1,i2,i3)*ut2(i1,i2,i3,kd)
        uyz23(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*urr2(i1,i2,i3,kd)+
     & sy(i1,i2,i3)*sz(i1,i2,i3)*uss2(i1,i2,i3,kd)+ty(i1,i2,i3)*tz(i1,
     & i2,i3)*utt2(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,i2,
     & i3)*sy(i1,i2,i3))*urs2(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(i1,i2,i3)+
     & rz(i1,i2,i3)*ty(i1,i2,i3))*urt2(i1,i2,i3,kd)+(sy(i1,i2,i3)*tz(
     & i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*ust2(i1,i2,i3,kd)+ryz23(
     & i1,i2,i3)*ur2(i1,i2,i3,kd)+syz23(i1,i2,i3)*us2(i1,i2,i3,kd)+
     & tyz23(i1,i2,i3)*ut2(i1,i2,i3,kd)
        ulaplacian23(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*urr2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)**
     & 2+sz(i1,i2,i3)**2)*uss2(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(i1,i2,
     & i3)**2+tz(i1,i2,i3)**2)*utt2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(
     & i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,i3))
     & *urs2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(i1,i2,i3)*
     & ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*urt2(i1,i2,i3,kd)+2.*(
     & sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)+sz(i1,i2,
     & i3)*tz(i1,i2,i3))*ust2(i1,i2,i3,kd)+(rxx23(i1,i2,i3)+ryy23(i1,
     & i2,i3)+rzz23(i1,i2,i3))*ur2(i1,i2,i3,kd)+(sxx23(i1,i2,i3)+
     & syy23(i1,i2,i3)+szz23(i1,i2,i3))*us2(i1,i2,i3,kd)+(txx23(i1,i2,
     & i3)+tyy23(i1,i2,i3)+tzz23(i1,i2,i3))*ut2(i1,i2,i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
! #If "RX" == "RX"
        h12(kd) = 1./(2.*dx(kd))
        h22(kd) = 1./(dx(kd)**2)
        ux23r(i1,i2,i3,kd)=(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))*h12(0)
        uy23r(i1,i2,i3,kd)=(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))*h12(1)
        uz23r(i1,i2,i3,kd)=(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))*h12(2)
        uxx23r(i1,i2,i3,kd)=(-2.*u(i1,i2,i3,kd)+(u(i1+1,i2,i3,kd)+u(i1-
     & 1,i2,i3,kd)) )*h22(0)
        uyy23r(i1,i2,i3,kd)=(-2.*u(i1,i2,i3,kd)+(u(i1,i2+1,i3,kd)+u(i1,
     & i2-1,i3,kd)) )*h22(1)
        uxy23r(i1,i2,i3,kd)=(ux23r(i1,i2+1,i3,kd)-ux23r(i1,i2-1,i3,kd))
     & *h12(1)
        uzz23r(i1,i2,i3,kd)=(-2.*u(i1,i2,i3,kd)+(u(i1,i2,i3+1,kd)+u(i1,
     & i2,i3-1,kd)) )*h22(2)
        uxz23r(i1,i2,i3,kd)=(ux23r(i1,i2,i3+1,kd)-ux23r(i1,i2,i3-1,kd))
     & *h12(2)
        uyz23r(i1,i2,i3,kd)=(uy23r(i1,i2,i3+1,kd)-uy23r(i1,i2,i3-1,kd))
     & *h12(2)
        ux21r(i1,i2,i3,kd)= ux23r(i1,i2,i3,kd)
        uy21r(i1,i2,i3,kd)= uy23r(i1,i2,i3,kd)
        uz21r(i1,i2,i3,kd)= uz23r(i1,i2,i3,kd)
        uxx21r(i1,i2,i3,kd)= uxx23r(i1,i2,i3,kd)
        uyy21r(i1,i2,i3,kd)= uyy23r(i1,i2,i3,kd)
        uzz21r(i1,i2,i3,kd)= uzz23r(i1,i2,i3,kd)
        uxy21r(i1,i2,i3,kd)= uxy23r(i1,i2,i3,kd)
        uxz21r(i1,i2,i3,kd)= uxz23r(i1,i2,i3,kd)
        uyz21r(i1,i2,i3,kd)= uyz23r(i1,i2,i3,kd)
        ulaplacian21r(i1,i2,i3,kd)=uxx23r(i1,i2,i3,kd)
        ux22r(i1,i2,i3,kd)= ux23r(i1,i2,i3,kd)
        uy22r(i1,i2,i3,kd)= uy23r(i1,i2,i3,kd)
        uz22r(i1,i2,i3,kd)= uz23r(i1,i2,i3,kd)
        uxx22r(i1,i2,i3,kd)= uxx23r(i1,i2,i3,kd)
        uyy22r(i1,i2,i3,kd)= uyy23r(i1,i2,i3,kd)
        uzz22r(i1,i2,i3,kd)= uzz23r(i1,i2,i3,kd)
        uxy22r(i1,i2,i3,kd)= uxy23r(i1,i2,i3,kd)
        uxz22r(i1,i2,i3,kd)= uxz23r(i1,i2,i3,kd)
        uyz22r(i1,i2,i3,kd)= uyz23r(i1,i2,i3,kd)
        ulaplacian22r(i1,i2,i3,kd)=uxx23r(i1,i2,i3,kd)+uyy23r(i1,i2,i3,
     & kd)
        ulaplacian23r(i1,i2,i3,kd)=uxx23r(i1,i2,i3,kd)+uyy23r(i1,i2,i3,
     & kd)+uzz23r(i1,i2,i3,kd)
        uxxx22r(i1,i2,i3,kd)=(-2.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))+(
     & u(i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
        uyyy22r(i1,i2,i3,kd)=(-2.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))+(
     & u(i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
        uxxy22r(i1,i2,i3,kd)=( uxx22r(i1,i2+1,i3,kd)-uxx22r(i1,i2-1,i3,
     & kd))/(2.*dx(1))
        uxyy22r(i1,i2,i3,kd)=( uyy22r(i1+1,i2,i3,kd)-uyy22r(i1-1,i2,i3,
     & kd))/(2.*dx(0))
        uxxxx22r(i1,i2,i3,kd)=(6.*u(i1,i2,i3,kd)-4.*(u(i1+1,i2,i3,kd)+
     & u(i1-1,i2,i3,kd))+(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)) )/(dx(0)*
     & *4)
        uyyyy22r(i1,i2,i3,kd)=(6.*u(i1,i2,i3,kd)-4.*(u(i1,i2+1,i3,kd)+
     & u(i1,i2-1,i3,kd))+(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )/(dx(1)*
     & *4)
        uxxyy22r(i1,i2,i3,kd)=( 4.*u(i1,i2,i3,kd)     -2.*(u(i1+1,i2,
     & i3,kd)+u(i1-1,i2,i3,kd)+u(i1,i2+1,i3,kd)+u(i1,i2-1,i3,kd))   + 
     &   (u(i1+1,i2+1,i3,kd)+u(i1-1,i2+1,i3,kd)+u(i1+1,i2-1,i3,kd)+u(
     & i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
        ! 2D laplacian squared = u.xxxx + 2 u.xxyy + u.yyyy
        uLapSq22r(i1,i2,i3,kd)= ( 6.*u(i1,i2,i3,kd)   - 4.*(u(i1+1,i2,
     & i3,kd)+u(i1-1,i2,i3,kd))    +(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)
     & ) )/(dx(0)**4) +( 6.*u(i1,i2,i3,kd)    -4.*(u(i1,i2+1,i3,kd)+u(
     & i1,i2-1,i3,kd))    +(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )/(dx(
     & 1)**4)  +( 8.*u(i1,i2,i3,kd)     -4.*(u(i1+1,i2,i3,kd)+u(i1-1,
     & i2,i3,kd)+u(i1,i2+1,i3,kd)+u(i1,i2-1,i3,kd))   +2.*(u(i1+1,i2+
     & 1,i3,kd)+u(i1-1,i2+1,i3,kd)+u(i1+1,i2-1,i3,kd)+u(i1-1,i2-1,i3,
     & kd)) )/(dx(0)**2*dx(1)**2)
        uxxx23r(i1,i2,i3,kd)=(-2.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))+(
     & u(i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
        uyyy23r(i1,i2,i3,kd)=(-2.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))+(
     & u(i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
        uzzz23r(i1,i2,i3,kd)=(-2.*(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))+(
     & u(i1,i2,i3+2,kd)-u(i1,i2,i3-2,kd)) )*h22(1)*h12(2)
        uxxy23r(i1,i2,i3,kd)=( uxx22r(i1,i2+1,i3,kd)-uxx22r(i1,i2-1,i3,
     & kd))/(2.*dx(1))
        uxyy23r(i1,i2,i3,kd)=( uyy22r(i1+1,i2,i3,kd)-uyy22r(i1-1,i2,i3,
     & kd))/(2.*dx(0))
        uxxz23r(i1,i2,i3,kd)=( uxx22r(i1,i2,i3+1,kd)-uxx22r(i1,i2,i3-1,
     & kd))/(2.*dx(2))
        uyyz23r(i1,i2,i3,kd)=( uyy22r(i1,i2,i3+1,kd)-uyy22r(i1,i2,i3-1,
     & kd))/(2.*dx(2))
        uxzz23r(i1,i2,i3,kd)=( uzz22r(i1+1,i2,i3,kd)-uzz22r(i1-1,i2,i3,
     & kd))/(2.*dx(0))
        uyzz23r(i1,i2,i3,kd)=( uzz22r(i1,i2+1,i3,kd)-uzz22r(i1,i2-1,i3,
     & kd))/(2.*dx(1))
        uxxxx23r(i1,i2,i3,kd)=(6.*u(i1,i2,i3,kd)-4.*(u(i1+1,i2,i3,kd)+
     & u(i1-1,i2,i3,kd))+(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)) )/(dx(0)*
     & *4)
        uyyyy23r(i1,i2,i3,kd)=(6.*u(i1,i2,i3,kd)-4.*(u(i1,i2+1,i3,kd)+
     & u(i1,i2-1,i3,kd))+(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )/(dx(1)*
     & *4)
        uzzzz23r(i1,i2,i3,kd)=(6.*u(i1,i2,i3,kd)-4.*(u(i1,i2,i3+1,kd)+
     & u(i1,i2,i3-1,kd))+(u(i1,i2,i3+2,kd)+u(i1,i2,i3-2,kd)) )/(dx(2)*
     & *4)
        uxxyy23r(i1,i2,i3,kd)=( 4.*u(i1,i2,i3,kd)     -2.*(u(i1+1,i2,
     & i3,kd)+u(i1-1,i2,i3,kd)+u(i1,i2+1,i3,kd)+u(i1,i2-1,i3,kd))   + 
     &   (u(i1+1,i2+1,i3,kd)+u(i1-1,i2+1,i3,kd)+u(i1+1,i2-1,i3,kd)+u(
     & i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
        uxxzz23r(i1,i2,i3,kd)=( 4.*u(i1,i2,i3,kd)     -2.*(u(i1+1,i2,
     & i3,kd)+u(i1-1,i2,i3,kd)+u(i1,i2,i3+1,kd)+u(i1,i2,i3-1,kd))   + 
     &   (u(i1+1,i2,i3+1,kd)+u(i1-1,i2,i3+1,kd)+u(i1+1,i2,i3-1,kd)+u(
     & i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)
        uyyzz23r(i1,i2,i3,kd)=( 4.*u(i1,i2,i3,kd)     -2.*(u(i1,i2+1,
     & i3,kd)  +u(i1,i2-1,i3,kd)+  u(i1,i2  ,i3+1,kd)+u(i1,i2  ,i3-1,
     & kd))   +   (u(i1,i2+1,i3+1,kd)+u(i1,i2-1,i3+1,kd)+u(i1,i2+1,i3-
     & 1,kd)+u(i1,i2-1,i3-1,kd)) )/(dx(1)**2*dx(2)**2)
        ! 3D laplacian squared = u.xxxx + u.yyyy + u.zzzz + 2 (u.xxyy + u.xxzz + u.yyzz )
        uLapSq23r(i1,i2,i3,kd)= ( 6.*u(i1,i2,i3,kd)   - 4.*(u(i1+1,i2,
     & i3,kd)+u(i1-1,i2,i3,kd))    +(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)
     & ) )/(dx(0)**4) +( 6.*u(i1,i2,i3,kd)    -4.*(u(i1,i2+1,i3,kd)+u(
     & i1,i2-1,i3,kd))    +(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )/(dx(
     & 1)**4)  +( 6.*u(i1,i2,i3,kd)    -4.*(u(i1,i2,i3+1,kd)+u(i1,i2,
     & i3-1,kd))    +(u(i1,i2,i3+2,kd)+u(i1,i2,i3-2,kd)) )/(dx(2)**4) 
     &  +( 8.*u(i1,i2,i3,kd)     -4.*(u(i1+1,i2,i3,kd)  +u(i1-1,i2,i3,
     & kd)  +u(i1  ,i2+1,i3,kd)+u(i1  ,i2-1,i3,kd))   +2.*(u(i1+1,i2+
     & 1,i3,kd)+u(i1-1,i2+1,i3,kd)+u(i1+1,i2-1,i3,kd)+u(i1-1,i2-1,i3,
     & kd)) )/(dx(0)**2*dx(1)**2)+( 8.*u(i1,i2,i3,kd)     -4.*(u(i1+1,
     & i2,i3,kd)  +u(i1-1,i2,i3,kd)  +u(i1  ,i2,i3+1,kd)+u(i1  ,i2,i3-
     & 1,kd))   +2.*(u(i1+1,i2,i3+1,kd)+u(i1-1,i2,i3+1,kd)+u(i1+1,i2,
     & i3-1,kd)+u(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)+( 8.*u(i1,
     & i2,i3,kd)     -4.*(u(i1,i2+1,i3,kd)  +u(i1,i2-1,i3,kd)  +u(i1,
     & i2  ,i3+1,kd)+u(i1,i2  ,i3-1,kd))   +2.*(u(i1,i2+1,i3+1,kd)+u(
     & i1,i2-1,i3+1,kd)+u(i1,i2+1,i3-1,kd)+u(i1,i2-1,i3-1,kd)) )/(dx(
     & 1)**2*dx(2)**2)
! defineDifferenceOrder4Components1(u,RX)
! #If "RX" == "RX"
        d14(kd) = 1./(12.*dr(kd))
        d24(kd) = 1./(12.*dr(kd)**2)
        ur4(i1,i2,i3,kd)=(8.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))-(u(i1+
     & 2,i2,i3,kd)-u(i1-2,i2,i3,kd)))*d14(0)
        us4(i1,i2,i3,kd)=(8.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))-(u(i1,
     & i2+2,i3,kd)-u(i1,i2-2,i3,kd)))*d14(1)
        ut4(i1,i2,i3,kd)=(8.*(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))-(u(i1,
     & i2,i3+2,kd)-u(i1,i2,i3-2,kd)))*d14(2)
        urr4(i1,i2,i3,kd)=(-30.*u(i1,i2,i3,kd)+16.*(u(i1+1,i2,i3,kd)+u(
     & i1-1,i2,i3,kd))-(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)) )*d24(0)
        uss4(i1,i2,i3,kd)=(-30.*u(i1,i2,i3,kd)+16.*(u(i1,i2+1,i3,kd)+u(
     & i1,i2-1,i3,kd))-(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )*d24(1)
        utt4(i1,i2,i3,kd)=(-30.*u(i1,i2,i3,kd)+16.*(u(i1,i2,i3+1,kd)+u(
     & i1,i2,i3-1,kd))-(u(i1,i2,i3+2,kd)+u(i1,i2,i3-2,kd)) )*d24(2)
        urs4(i1,i2,i3,kd)=(8.*(ur4(i1,i2+1,i3,kd)-ur4(i1,i2-1,i3,kd))-(
     & ur4(i1,i2+2,i3,kd)-ur4(i1,i2-2,i3,kd)))*d14(1)
        urt4(i1,i2,i3,kd)=(8.*(ur4(i1,i2,i3+1,kd)-ur4(i1,i2,i3-1,kd))-(
     & ur4(i1,i2,i3+2,kd)-ur4(i1,i2,i3-2,kd)))*d14(2)
        ust4(i1,i2,i3,kd)=(8.*(us4(i1,i2,i3+1,kd)-us4(i1,i2,i3-1,kd))-(
     & us4(i1,i2,i3+2,kd)-us4(i1,i2,i3-2,kd)))*d14(2)
! #If "RX" == "RX"
        rxr4(i1,i2,i3)=(8.*(rx(i1+1,i2,i3)-rx(i1-1,i2,i3))-(rx(i1+2,i2,
     & i3)-rx(i1-2,i2,i3)))*d14(0)
        rxs4(i1,i2,i3)=(8.*(rx(i1,i2+1,i3)-rx(i1,i2-1,i3))-(rx(i1,i2+2,
     & i3)-rx(i1,i2-2,i3)))*d14(1)
        rxt4(i1,i2,i3)=(8.*(rx(i1,i2,i3+1)-rx(i1,i2,i3-1))-(rx(i1,i2,
     & i3+2)-rx(i1,i2,i3-2)))*d14(2)
        ryr4(i1,i2,i3)=(8.*(ry(i1+1,i2,i3)-ry(i1-1,i2,i3))-(ry(i1+2,i2,
     & i3)-ry(i1-2,i2,i3)))*d14(0)
        rys4(i1,i2,i3)=(8.*(ry(i1,i2+1,i3)-ry(i1,i2-1,i3))-(ry(i1,i2+2,
     & i3)-ry(i1,i2-2,i3)))*d14(1)
        ryt4(i1,i2,i3)=(8.*(ry(i1,i2,i3+1)-ry(i1,i2,i3-1))-(ry(i1,i2,
     & i3+2)-ry(i1,i2,i3-2)))*d14(2)
        rzr4(i1,i2,i3)=(8.*(rz(i1+1,i2,i3)-rz(i1-1,i2,i3))-(rz(i1+2,i2,
     & i3)-rz(i1-2,i2,i3)))*d14(0)
        rzs4(i1,i2,i3)=(8.*(rz(i1,i2+1,i3)-rz(i1,i2-1,i3))-(rz(i1,i2+2,
     & i3)-rz(i1,i2-2,i3)))*d14(1)
        rzt4(i1,i2,i3)=(8.*(rz(i1,i2,i3+1)-rz(i1,i2,i3-1))-(rz(i1,i2,
     & i3+2)-rz(i1,i2,i3-2)))*d14(2)
        sxr4(i1,i2,i3)=(8.*(sx(i1+1,i2,i3)-sx(i1-1,i2,i3))-(sx(i1+2,i2,
     & i3)-sx(i1-2,i2,i3)))*d14(0)
        sxs4(i1,i2,i3)=(8.*(sx(i1,i2+1,i3)-sx(i1,i2-1,i3))-(sx(i1,i2+2,
     & i3)-sx(i1,i2-2,i3)))*d14(1)
        sxt4(i1,i2,i3)=(8.*(sx(i1,i2,i3+1)-sx(i1,i2,i3-1))-(sx(i1,i2,
     & i3+2)-sx(i1,i2,i3-2)))*d14(2)
        syr4(i1,i2,i3)=(8.*(sy(i1+1,i2,i3)-sy(i1-1,i2,i3))-(sy(i1+2,i2,
     & i3)-sy(i1-2,i2,i3)))*d14(0)
        sys4(i1,i2,i3)=(8.*(sy(i1,i2+1,i3)-sy(i1,i2-1,i3))-(sy(i1,i2+2,
     & i3)-sy(i1,i2-2,i3)))*d14(1)
        syt4(i1,i2,i3)=(8.*(sy(i1,i2,i3+1)-sy(i1,i2,i3-1))-(sy(i1,i2,
     & i3+2)-sy(i1,i2,i3-2)))*d14(2)
        szr4(i1,i2,i3)=(8.*(sz(i1+1,i2,i3)-sz(i1-1,i2,i3))-(sz(i1+2,i2,
     & i3)-sz(i1-2,i2,i3)))*d14(0)
        szs4(i1,i2,i3)=(8.*(sz(i1,i2+1,i3)-sz(i1,i2-1,i3))-(sz(i1,i2+2,
     & i3)-sz(i1,i2-2,i3)))*d14(1)
        szt4(i1,i2,i3)=(8.*(sz(i1,i2,i3+1)-sz(i1,i2,i3-1))-(sz(i1,i2,
     & i3+2)-sz(i1,i2,i3-2)))*d14(2)
        txr4(i1,i2,i3)=(8.*(tx(i1+1,i2,i3)-tx(i1-1,i2,i3))-(tx(i1+2,i2,
     & i3)-tx(i1-2,i2,i3)))*d14(0)
        txs4(i1,i2,i3)=(8.*(tx(i1,i2+1,i3)-tx(i1,i2-1,i3))-(tx(i1,i2+2,
     & i3)-tx(i1,i2-2,i3)))*d14(1)
        txt4(i1,i2,i3)=(8.*(tx(i1,i2,i3+1)-tx(i1,i2,i3-1))-(tx(i1,i2,
     & i3+2)-tx(i1,i2,i3-2)))*d14(2)
        tyr4(i1,i2,i3)=(8.*(ty(i1+1,i2,i3)-ty(i1-1,i2,i3))-(ty(i1+2,i2,
     & i3)-ty(i1-2,i2,i3)))*d14(0)
        tys4(i1,i2,i3)=(8.*(ty(i1,i2+1,i3)-ty(i1,i2-1,i3))-(ty(i1,i2+2,
     & i3)-ty(i1,i2-2,i3)))*d14(1)
        tyt4(i1,i2,i3)=(8.*(ty(i1,i2,i3+1)-ty(i1,i2,i3-1))-(ty(i1,i2,
     & i3+2)-ty(i1,i2,i3-2)))*d14(2)
        tzr4(i1,i2,i3)=(8.*(tz(i1+1,i2,i3)-tz(i1-1,i2,i3))-(tz(i1+2,i2,
     & i3)-tz(i1-2,i2,i3)))*d14(0)
        tzs4(i1,i2,i3)=(8.*(tz(i1,i2+1,i3)-tz(i1,i2-1,i3))-(tz(i1,i2+2,
     & i3)-tz(i1,i2-2,i3)))*d14(1)
        tzt4(i1,i2,i3)=(8.*(tz(i1,i2,i3+1)-tz(i1,i2,i3-1))-(tz(i1,i2,
     & i3+2)-tz(i1,i2,i3-2)))*d14(2)
        ux41(i1,i2,i3,kd)= rx(i1,i2,i3)*ur4(i1,i2,i3,kd)
        uy41(i1,i2,i3,kd)=0
        uz41(i1,i2,i3,kd)=0
        ux42(i1,i2,i3,kd)= rx(i1,i2,i3)*ur4(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & us4(i1,i2,i3,kd)
        uy42(i1,i2,i3,kd)= ry(i1,i2,i3)*ur4(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & us4(i1,i2,i3,kd)
        uz42(i1,i2,i3,kd)=0
        ux43(i1,i2,i3,kd)=rx(i1,i2,i3)*ur4(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & us4(i1,i2,i3,kd)+tx(i1,i2,i3)*ut4(i1,i2,i3,kd)
        uy43(i1,i2,i3,kd)=ry(i1,i2,i3)*ur4(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & us4(i1,i2,i3,kd)+ty(i1,i2,i3)*ut4(i1,i2,i3,kd)
        uz43(i1,i2,i3,kd)=rz(i1,i2,i3)*ur4(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & us4(i1,i2,i3,kd)+tz(i1,i2,i3)*ut4(i1,i2,i3,kd)
! #If "RX" == "RX"
        rxx41(i1,i2,i3)= rx(i1,i2,i3)*rxr4(i1,i2,i3)
        rxx42(i1,i2,i3)= rx(i1,i2,i3)*rxr4(i1,i2,i3)+sx(i1,i2,i3)*rxs4(
     & i1,i2,i3)
        rxy42(i1,i2,i3)= ry(i1,i2,i3)*rxr4(i1,i2,i3)+sy(i1,i2,i3)*rxs4(
     & i1,i2,i3)
        rxx43(i1,i2,i3)=rx(i1,i2,i3)*rxr4(i1,i2,i3)+sx(i1,i2,i3)*rxs4(
     & i1,i2,i3)+tx(i1,i2,i3)*rxt4(i1,i2,i3)
        rxy43(i1,i2,i3)=ry(i1,i2,i3)*rxr4(i1,i2,i3)+sy(i1,i2,i3)*rxs4(
     & i1,i2,i3)+ty(i1,i2,i3)*rxt4(i1,i2,i3)
        rxz43(i1,i2,i3)=rz(i1,i2,i3)*rxr4(i1,i2,i3)+sz(i1,i2,i3)*rxs4(
     & i1,i2,i3)+tz(i1,i2,i3)*rxt4(i1,i2,i3)
        ryx42(i1,i2,i3)= rx(i1,i2,i3)*ryr4(i1,i2,i3)+sx(i1,i2,i3)*rys4(
     & i1,i2,i3)
        ryy42(i1,i2,i3)= ry(i1,i2,i3)*ryr4(i1,i2,i3)+sy(i1,i2,i3)*rys4(
     & i1,i2,i3)
        ryx43(i1,i2,i3)=rx(i1,i2,i3)*ryr4(i1,i2,i3)+sx(i1,i2,i3)*rys4(
     & i1,i2,i3)+tx(i1,i2,i3)*ryt4(i1,i2,i3)
        ryy43(i1,i2,i3)=ry(i1,i2,i3)*ryr4(i1,i2,i3)+sy(i1,i2,i3)*rys4(
     & i1,i2,i3)+ty(i1,i2,i3)*ryt4(i1,i2,i3)
        ryz43(i1,i2,i3)=rz(i1,i2,i3)*ryr4(i1,i2,i3)+sz(i1,i2,i3)*rys4(
     & i1,i2,i3)+tz(i1,i2,i3)*ryt4(i1,i2,i3)
        rzx42(i1,i2,i3)= rx(i1,i2,i3)*rzr4(i1,i2,i3)+sx(i1,i2,i3)*rzs4(
     & i1,i2,i3)
        rzy42(i1,i2,i3)= ry(i1,i2,i3)*rzr4(i1,i2,i3)+sy(i1,i2,i3)*rzs4(
     & i1,i2,i3)
        rzx43(i1,i2,i3)=rx(i1,i2,i3)*rzr4(i1,i2,i3)+sx(i1,i2,i3)*rzs4(
     & i1,i2,i3)+tx(i1,i2,i3)*rzt4(i1,i2,i3)
        rzy43(i1,i2,i3)=ry(i1,i2,i3)*rzr4(i1,i2,i3)+sy(i1,i2,i3)*rzs4(
     & i1,i2,i3)+ty(i1,i2,i3)*rzt4(i1,i2,i3)
        rzz43(i1,i2,i3)=rz(i1,i2,i3)*rzr4(i1,i2,i3)+sz(i1,i2,i3)*rzs4(
     & i1,i2,i3)+tz(i1,i2,i3)*rzt4(i1,i2,i3)
        sxx42(i1,i2,i3)= rx(i1,i2,i3)*sxr4(i1,i2,i3)+sx(i1,i2,i3)*sxs4(
     & i1,i2,i3)
        sxy42(i1,i2,i3)= ry(i1,i2,i3)*sxr4(i1,i2,i3)+sy(i1,i2,i3)*sxs4(
     & i1,i2,i3)
        sxx43(i1,i2,i3)=rx(i1,i2,i3)*sxr4(i1,i2,i3)+sx(i1,i2,i3)*sxs4(
     & i1,i2,i3)+tx(i1,i2,i3)*sxt4(i1,i2,i3)
        sxy43(i1,i2,i3)=ry(i1,i2,i3)*sxr4(i1,i2,i3)+sy(i1,i2,i3)*sxs4(
     & i1,i2,i3)+ty(i1,i2,i3)*sxt4(i1,i2,i3)
        sxz43(i1,i2,i3)=rz(i1,i2,i3)*sxr4(i1,i2,i3)+sz(i1,i2,i3)*sxs4(
     & i1,i2,i3)+tz(i1,i2,i3)*sxt4(i1,i2,i3)
        syx42(i1,i2,i3)= rx(i1,i2,i3)*syr4(i1,i2,i3)+sx(i1,i2,i3)*sys4(
     & i1,i2,i3)
        syy42(i1,i2,i3)= ry(i1,i2,i3)*syr4(i1,i2,i3)+sy(i1,i2,i3)*sys4(
     & i1,i2,i3)
        syx43(i1,i2,i3)=rx(i1,i2,i3)*syr4(i1,i2,i3)+sx(i1,i2,i3)*sys4(
     & i1,i2,i3)+tx(i1,i2,i3)*syt4(i1,i2,i3)
        syy43(i1,i2,i3)=ry(i1,i2,i3)*syr4(i1,i2,i3)+sy(i1,i2,i3)*sys4(
     & i1,i2,i3)+ty(i1,i2,i3)*syt4(i1,i2,i3)
        syz43(i1,i2,i3)=rz(i1,i2,i3)*syr4(i1,i2,i3)+sz(i1,i2,i3)*sys4(
     & i1,i2,i3)+tz(i1,i2,i3)*syt4(i1,i2,i3)
        szx42(i1,i2,i3)= rx(i1,i2,i3)*szr4(i1,i2,i3)+sx(i1,i2,i3)*szs4(
     & i1,i2,i3)
        szy42(i1,i2,i3)= ry(i1,i2,i3)*szr4(i1,i2,i3)+sy(i1,i2,i3)*szs4(
     & i1,i2,i3)
        szx43(i1,i2,i3)=rx(i1,i2,i3)*szr4(i1,i2,i3)+sx(i1,i2,i3)*szs4(
     & i1,i2,i3)+tx(i1,i2,i3)*szt4(i1,i2,i3)
        szy43(i1,i2,i3)=ry(i1,i2,i3)*szr4(i1,i2,i3)+sy(i1,i2,i3)*szs4(
     & i1,i2,i3)+ty(i1,i2,i3)*szt4(i1,i2,i3)
        szz43(i1,i2,i3)=rz(i1,i2,i3)*szr4(i1,i2,i3)+sz(i1,i2,i3)*szs4(
     & i1,i2,i3)+tz(i1,i2,i3)*szt4(i1,i2,i3)
        txx42(i1,i2,i3)= rx(i1,i2,i3)*txr4(i1,i2,i3)+sx(i1,i2,i3)*txs4(
     & i1,i2,i3)
        txy42(i1,i2,i3)= ry(i1,i2,i3)*txr4(i1,i2,i3)+sy(i1,i2,i3)*txs4(
     & i1,i2,i3)
        txx43(i1,i2,i3)=rx(i1,i2,i3)*txr4(i1,i2,i3)+sx(i1,i2,i3)*txs4(
     & i1,i2,i3)+tx(i1,i2,i3)*txt4(i1,i2,i3)
        txy43(i1,i2,i3)=ry(i1,i2,i3)*txr4(i1,i2,i3)+sy(i1,i2,i3)*txs4(
     & i1,i2,i3)+ty(i1,i2,i3)*txt4(i1,i2,i3)
        txz43(i1,i2,i3)=rz(i1,i2,i3)*txr4(i1,i2,i3)+sz(i1,i2,i3)*txs4(
     & i1,i2,i3)+tz(i1,i2,i3)*txt4(i1,i2,i3)
        tyx42(i1,i2,i3)= rx(i1,i2,i3)*tyr4(i1,i2,i3)+sx(i1,i2,i3)*tys4(
     & i1,i2,i3)
        tyy42(i1,i2,i3)= ry(i1,i2,i3)*tyr4(i1,i2,i3)+sy(i1,i2,i3)*tys4(
     & i1,i2,i3)
        tyx43(i1,i2,i3)=rx(i1,i2,i3)*tyr4(i1,i2,i3)+sx(i1,i2,i3)*tys4(
     & i1,i2,i3)+tx(i1,i2,i3)*tyt4(i1,i2,i3)
        tyy43(i1,i2,i3)=ry(i1,i2,i3)*tyr4(i1,i2,i3)+sy(i1,i2,i3)*tys4(
     & i1,i2,i3)+ty(i1,i2,i3)*tyt4(i1,i2,i3)
        tyz43(i1,i2,i3)=rz(i1,i2,i3)*tyr4(i1,i2,i3)+sz(i1,i2,i3)*tys4(
     & i1,i2,i3)+tz(i1,i2,i3)*tyt4(i1,i2,i3)
        tzx42(i1,i2,i3)= rx(i1,i2,i3)*tzr4(i1,i2,i3)+sx(i1,i2,i3)*tzs4(
     & i1,i2,i3)
        tzy42(i1,i2,i3)= ry(i1,i2,i3)*tzr4(i1,i2,i3)+sy(i1,i2,i3)*tzs4(
     & i1,i2,i3)
        tzx43(i1,i2,i3)=rx(i1,i2,i3)*tzr4(i1,i2,i3)+sx(i1,i2,i3)*tzs4(
     & i1,i2,i3)+tx(i1,i2,i3)*tzt4(i1,i2,i3)
        tzy43(i1,i2,i3)=ry(i1,i2,i3)*tzr4(i1,i2,i3)+sy(i1,i2,i3)*tzs4(
     & i1,i2,i3)+ty(i1,i2,i3)*tzt4(i1,i2,i3)
        tzz43(i1,i2,i3)=rz(i1,i2,i3)*tzr4(i1,i2,i3)+sz(i1,i2,i3)*tzs4(
     & i1,i2,i3)+tz(i1,i2,i3)*tzt4(i1,i2,i3)
        uxx41(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*urr4(i1,i2,i3,kd)+(rxx42(
     & i1,i2,i3))*ur4(i1,i2,i3,kd)
        uyy41(i1,i2,i3,kd)=0
        uxy41(i1,i2,i3,kd)=0
        uxz41(i1,i2,i3,kd)=0
        uyz41(i1,i2,i3,kd)=0
        uzz41(i1,i2,i3,kd)=0
        ulaplacian41(i1,i2,i3,kd)=uxx41(i1,i2,i3,kd)
        uxx42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*urr4(i1,i2,i3,kd)+2.*(rx(
     & i1,i2,i3)*sx(i1,i2,i3))*urs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2)*
     & uss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3))*ur4(i1,i2,i3,kd)+(sxx42(i1,
     & i2,i3))*us4(i1,i2,i3,kd)
        uyy42(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*urr4(i1,i2,i3,kd)+2.*(ry(
     & i1,i2,i3)*sy(i1,i2,i3))*urs4(i1,i2,i3,kd)+(sy(i1,i2,i3)**2)*
     & uss4(i1,i2,i3,kd)+(ryy42(i1,i2,i3))*ur4(i1,i2,i3,kd)+(syy42(i1,
     & i2,i3))*us4(i1,i2,i3,kd)
        uxy42(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*urr4(i1,i2,i3,kd)+
     & (rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*urs4(i1,
     & i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*uss4(i1,i2,i3,kd)+rxy42(i1,
     & i2,i3)*ur4(i1,i2,i3,kd)+sxy42(i1,i2,i3)*us4(i1,i2,i3,kd)
        uxz42(i1,i2,i3,kd)=0
        uyz42(i1,i2,i3,kd)=0
        uzz42(i1,i2,i3,kd)=0
        ulaplacian42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & urr4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*
     & sy(i1,i2,i3))*urs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)**
     & 2)*uss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3)+ryy42(i1,i2,i3))*ur4(i1,
     & i2,i3,kd)+(sxx42(i1,i2,i3)+syy42(i1,i2,i3))*us4(i1,i2,i3,kd)
        uxx43(i1,i2,i3,kd)=rx(i1,i2,i3)**2*urr4(i1,i2,i3,kd)+sx(i1,i2,
     & i3)**2*uss4(i1,i2,i3,kd)+tx(i1,i2,i3)**2*utt4(i1,i2,i3,kd)+2.*
     & rx(i1,i2,i3)*sx(i1,i2,i3)*urs4(i1,i2,i3,kd)+2.*rx(i1,i2,i3)*tx(
     & i1,i2,i3)*urt4(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,i2,i3)*ust4(
     & i1,i2,i3,kd)+rxx43(i1,i2,i3)*ur4(i1,i2,i3,kd)+sxx43(i1,i2,i3)*
     & us4(i1,i2,i3,kd)+txx43(i1,i2,i3)*ut4(i1,i2,i3,kd)
        uyy43(i1,i2,i3,kd)=ry(i1,i2,i3)**2*urr4(i1,i2,i3,kd)+sy(i1,i2,
     & i3)**2*uss4(i1,i2,i3,kd)+ty(i1,i2,i3)**2*utt4(i1,i2,i3,kd)+2.*
     & ry(i1,i2,i3)*sy(i1,i2,i3)*urs4(i1,i2,i3,kd)+2.*ry(i1,i2,i3)*ty(
     & i1,i2,i3)*urt4(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,i2,i3)*ust4(
     & i1,i2,i3,kd)+ryy43(i1,i2,i3)*ur4(i1,i2,i3,kd)+syy43(i1,i2,i3)*
     & us4(i1,i2,i3,kd)+tyy43(i1,i2,i3)*ut4(i1,i2,i3,kd)
        uzz43(i1,i2,i3,kd)=rz(i1,i2,i3)**2*urr4(i1,i2,i3,kd)+sz(i1,i2,
     & i3)**2*uss4(i1,i2,i3,kd)+tz(i1,i2,i3)**2*utt4(i1,i2,i3,kd)+2.*
     & rz(i1,i2,i3)*sz(i1,i2,i3)*urs4(i1,i2,i3,kd)+2.*rz(i1,i2,i3)*tz(
     & i1,i2,i3)*urt4(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,i2,i3)*ust4(
     & i1,i2,i3,kd)+rzz43(i1,i2,i3)*ur4(i1,i2,i3,kd)+szz43(i1,i2,i3)*
     & us4(i1,i2,i3,kd)+tzz43(i1,i2,i3)*ut4(i1,i2,i3,kd)
        uxy43(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*urr4(i1,i2,i3,kd)+
     & sx(i1,i2,i3)*sy(i1,i2,i3)*uss4(i1,i2,i3,kd)+tx(i1,i2,i3)*ty(i1,
     & i2,i3)*utt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,
     & i3)*sx(i1,i2,i3))*urs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(i1,i2,i3)+
     & ry(i1,i2,i3)*tx(i1,i2,i3))*urt4(i1,i2,i3,kd)+(sx(i1,i2,i3)*ty(
     & i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*ust4(i1,i2,i3,kd)+rxy43(
     & i1,i2,i3)*ur4(i1,i2,i3,kd)+sxy43(i1,i2,i3)*us4(i1,i2,i3,kd)+
     & txy43(i1,i2,i3)*ut4(i1,i2,i3,kd)
        uxz43(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*urr4(i1,i2,i3,kd)+
     & sx(i1,i2,i3)*sz(i1,i2,i3)*uss4(i1,i2,i3,kd)+tx(i1,i2,i3)*tz(i1,
     & i2,i3)*utt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,i2,
     & i3)*sx(i1,i2,i3))*urs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(i1,i2,i3)+
     & rz(i1,i2,i3)*tx(i1,i2,i3))*urt4(i1,i2,i3,kd)+(sx(i1,i2,i3)*tz(
     & i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*ust4(i1,i2,i3,kd)+rxz43(
     & i1,i2,i3)*ur4(i1,i2,i3,kd)+sxz43(i1,i2,i3)*us4(i1,i2,i3,kd)+
     & txz43(i1,i2,i3)*ut4(i1,i2,i3,kd)
        uyz43(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*urr4(i1,i2,i3,kd)+
     & sy(i1,i2,i3)*sz(i1,i2,i3)*uss4(i1,i2,i3,kd)+ty(i1,i2,i3)*tz(i1,
     & i2,i3)*utt4(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,i2,
     & i3)*sy(i1,i2,i3))*urs4(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(i1,i2,i3)+
     & rz(i1,i2,i3)*ty(i1,i2,i3))*urt4(i1,i2,i3,kd)+(sy(i1,i2,i3)*tz(
     & i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*ust4(i1,i2,i3,kd)+ryz43(
     & i1,i2,i3)*ur4(i1,i2,i3,kd)+syz43(i1,i2,i3)*us4(i1,i2,i3,kd)+
     & tyz43(i1,i2,i3)*ut4(i1,i2,i3,kd)
        ulaplacian43(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*urr4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)**
     & 2+sz(i1,i2,i3)**2)*uss4(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(i1,i2,
     & i3)**2+tz(i1,i2,i3)**2)*utt4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(
     & i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,i3))
     & *urs4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(i1,i2,i3)*
     & ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*urt4(i1,i2,i3,kd)+2.*(
     & sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)+sz(i1,i2,
     & i3)*tz(i1,i2,i3))*ust4(i1,i2,i3,kd)+(rxx43(i1,i2,i3)+ryy43(i1,
     & i2,i3)+rzz43(i1,i2,i3))*ur4(i1,i2,i3,kd)+(sxx43(i1,i2,i3)+
     & syy43(i1,i2,i3)+szz43(i1,i2,i3))*us4(i1,i2,i3,kd)+(txx43(i1,i2,
     & i3)+tyy43(i1,i2,i3)+tzz43(i1,i2,i3))*ut4(i1,i2,i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
! #If "RX" == "RX"
        h41(kd) = 1./(12.*dx(kd))
        h42(kd) = 1./(12.*dx(kd)**2)
        ux43r(i1,i2,i3,kd)=(8.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))-(u(
     & i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd)))*h41(0)
        uy43r(i1,i2,i3,kd)=(8.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))-(u(
     & i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd)))*h41(1)
        uz43r(i1,i2,i3,kd)=(8.*(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))-(u(
     & i1,i2,i3+2,kd)-u(i1,i2,i3-2,kd)))*h41(2)
        uxx43r(i1,i2,i3,kd)=( -30.*u(i1,i2,i3,kd)+16.*(u(i1+1,i2,i3,kd)
     & +u(i1-1,i2,i3,kd))-(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)) )*h42(0)
        uyy43r(i1,i2,i3,kd)=( -30.*u(i1,i2,i3,kd)+16.*(u(i1,i2+1,i3,kd)
     & +u(i1,i2-1,i3,kd))-(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )*h42(1)
        uzz43r(i1,i2,i3,kd)=( -30.*u(i1,i2,i3,kd)+16.*(u(i1,i2,i3+1,kd)
     & +u(i1,i2,i3-1,kd))-(u(i1,i2,i3+2,kd)+u(i1,i2,i3-2,kd)) )*h42(2)
        uxy43r(i1,i2,i3,kd)=( (u(i1+2,i2+2,i3,kd)-u(i1-2,i2+2,i3,kd)- 
     & u(i1+2,i2-2,i3,kd)+u(i1-2,i2-2,i3,kd)) +8.*(u(i1-1,i2+2,i3,kd)-
     & u(i1-1,i2-2,i3,kd)-u(i1+1,i2+2,i3,kd)+u(i1+1,i2-2,i3,kd) +u(i1+
     & 2,i2-1,i3,kd)-u(i1-2,i2-1,i3,kd)-u(i1+2,i2+1,i3,kd)+u(i1-2,i2+
     & 1,i3,kd))+64.*(u(i1+1,i2+1,i3,kd)-u(i1-1,i2+1,i3,kd)- u(i1+1,
     & i2-1,i3,kd)+u(i1-1,i2-1,i3,kd)))*(h41(0)*h41(1))
        uxz43r(i1,i2,i3,kd)=( (u(i1+2,i2,i3+2,kd)-u(i1-2,i2,i3+2,kd)-u(
     & i1+2,i2,i3-2,kd)+u(i1-2,i2,i3-2,kd)) +8.*(u(i1-1,i2,i3+2,kd)-u(
     & i1-1,i2,i3-2,kd)-u(i1+1,i2,i3+2,kd)+u(i1+1,i2,i3-2,kd) +u(i1+2,
     & i2,i3-1,kd)-u(i1-2,i2,i3-1,kd)- u(i1+2,i2,i3+1,kd)+u(i1-2,i2,
     & i3+1,kd)) +64.*(u(i1+1,i2,i3+1,kd)-u(i1-1,i2,i3+1,kd)-u(i1+1,
     & i2,i3-1,kd)+u(i1-1,i2,i3-1,kd)) )*(h41(0)*h41(2))
        uyz43r(i1,i2,i3,kd)=( (u(i1,i2+2,i3+2,kd)-u(i1,i2-2,i3+2,kd)-u(
     & i1,i2+2,i3-2,kd)+u(i1,i2-2,i3-2,kd)) +8.*(u(i1,i2-1,i3+2,kd)-u(
     & i1,i2-1,i3-2,kd)-u(i1,i2+1,i3+2,kd)+u(i1,i2+1,i3-2,kd) +u(i1,
     & i2+2,i3-1,kd)-u(i1,i2-2,i3-1,kd)-u(i1,i2+2,i3+1,kd)+u(i1,i2-2,
     & i3+1,kd)) +64.*(u(i1,i2+1,i3+1,kd)-u(i1,i2-1,i3+1,kd)-u(i1,i2+
     & 1,i3-1,kd)+u(i1,i2-1,i3-1,kd)) )*(h41(1)*h41(2))
        ux41r(i1,i2,i3,kd)= ux43r(i1,i2,i3,kd)
        uy41r(i1,i2,i3,kd)= uy43r(i1,i2,i3,kd)
        uz41r(i1,i2,i3,kd)= uz43r(i1,i2,i3,kd)
        uxx41r(i1,i2,i3,kd)= uxx43r(i1,i2,i3,kd)
        uyy41r(i1,i2,i3,kd)= uyy43r(i1,i2,i3,kd)
        uzz41r(i1,i2,i3,kd)= uzz43r(i1,i2,i3,kd)
        uxy41r(i1,i2,i3,kd)= uxy43r(i1,i2,i3,kd)
        uxz41r(i1,i2,i3,kd)= uxz43r(i1,i2,i3,kd)
        uyz41r(i1,i2,i3,kd)= uyz43r(i1,i2,i3,kd)
        ulaplacian41r(i1,i2,i3,kd)=uxx43r(i1,i2,i3,kd)
        ux42r(i1,i2,i3,kd)= ux43r(i1,i2,i3,kd)
        uy42r(i1,i2,i3,kd)= uy43r(i1,i2,i3,kd)
        uz42r(i1,i2,i3,kd)= uz43r(i1,i2,i3,kd)
        uxx42r(i1,i2,i3,kd)= uxx43r(i1,i2,i3,kd)
        uyy42r(i1,i2,i3,kd)= uyy43r(i1,i2,i3,kd)
        uzz42r(i1,i2,i3,kd)= uzz43r(i1,i2,i3,kd)
        uxy42r(i1,i2,i3,kd)= uxy43r(i1,i2,i3,kd)
        uxz42r(i1,i2,i3,kd)= uxz43r(i1,i2,i3,kd)
        uyz42r(i1,i2,i3,kd)= uyz43r(i1,i2,i3,kd)
        ulaplacian42r(i1,i2,i3,kd)=uxx43r(i1,i2,i3,kd)+uyy43r(i1,i2,i3,
     & kd)
        ulaplacian43r(i1,i2,i3,kd)=uxx43r(i1,i2,i3,kd)+uyy43r(i1,i2,i3,
     & kd)+uzz43r(i1,i2,i3,kd)
! defineDifferenceOrder6Components1(u,RX)
! #If "RX" == "RX"
        d16(kd) = 1./(60.*dr(kd))
        d26(kd) = 1./(180.*dr(kd)**2)
        ur6(i1,i2,i3,kd)=(45.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))-9.*(
     & u(i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd))+(u(i1+3,i2,i3,kd)-u(i1-3,i2,
     & i3,kd)))*d16(0)
        us6(i1,i2,i3,kd)=(45.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))-9.*(
     & u(i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd))+(u(i1,i2+3,i3,kd)-u(i1,i2-3,
     & i3,kd)))*d16(1)
        ut6(i1,i2,i3,kd)=(45.*(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))-9.*(
     & u(i1,i2,i3+2,kd)-u(i1,i2,i3-2,kd))+(u(i1,i2,i3+3,kd)-u(i1,i2,
     & i3-3,kd)))*d16(2)
        urr6(i1,i2,i3,kd)=(-490.*u(i1,i2,i3,kd)+270.*(u(i1+1,i2,i3,kd)+
     & u(i1-1,i2,i3,kd))-27.*(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd))+2.*(
     & u(i1+3,i2,i3,kd)+u(i1-3,i2,i3,kd)) )*d26(0)
        uss6(i1,i2,i3,kd)=(-490.*u(i1,i2,i3,kd)+270.*(u(i1,i2+1,i3,kd)+
     & u(i1,i2-1,i3,kd))-27.*(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd))+2.*(
     & u(i1,i2+3,i3,kd)+u(i1,i2-3,i3,kd)) )*d26(1)
        utt6(i1,i2,i3,kd)=(-490.*u(i1,i2,i3,kd)+270.*(u(i1,i2,i3+1,kd)+
     & u(i1,i2,i3-1,kd))-27.*(u(i1,i2,i3+2,kd)+u(i1,i2,i3-2,kd))+2.*(
     & u(i1,i2,i3+3,kd)+u(i1,i2,i3-3,kd)) )*d26(2)
        urs6(i1,i2,i3,kd)=(45.*(ur6(i1,i2+1,i3,kd)-ur6(i1,i2-1,i3,kd))-
     & 9.*(ur6(i1,i2+2,i3,kd)-ur6(i1,i2-2,i3,kd))+(ur6(i1,i2+3,i3,kd)-
     & ur6(i1,i2-3,i3,kd)))*d16(1)
        urt6(i1,i2,i3,kd)=(45.*(ur6(i1,i2,i3+1,kd)-ur6(i1,i2,i3-1,kd))-
     & 9.*(ur6(i1,i2,i3+2,kd)-ur6(i1,i2,i3-2,kd))+(ur6(i1,i2,i3+3,kd)-
     & ur6(i1,i2,i3-3,kd)))*d16(2)
        ust6(i1,i2,i3,kd)=(45.*(us6(i1,i2,i3+1,kd)-us6(i1,i2,i3-1,kd))-
     & 9.*(us6(i1,i2,i3+2,kd)-us6(i1,i2,i3-2,kd))+(us6(i1,i2,i3+3,kd)-
     & us6(i1,i2,i3-3,kd)))*d16(2)
! #If "RX" == "RX"
        rxr6(i1,i2,i3)=(45.*(rx(i1+1,i2,i3)-rx(i1-1,i2,i3))-9.*(rx(i1+
     & 2,i2,i3)-rx(i1-2,i2,i3))+(rx(i1+3,i2,i3)-rx(i1-3,i2,i3)))*d16(
     & 0)
        rxs6(i1,i2,i3)=(45.*(rx(i1,i2+1,i3)-rx(i1,i2-1,i3))-9.*(rx(i1,
     & i2+2,i3)-rx(i1,i2-2,i3))+(rx(i1,i2+3,i3)-rx(i1,i2-3,i3)))*d16(
     & 1)
        rxt6(i1,i2,i3)=(45.*(rx(i1,i2,i3+1)-rx(i1,i2,i3-1))-9.*(rx(i1,
     & i2,i3+2)-rx(i1,i2,i3-2))+(rx(i1,i2,i3+3)-rx(i1,i2,i3-3)))*d16(
     & 2)
        ryr6(i1,i2,i3)=(45.*(ry(i1+1,i2,i3)-ry(i1-1,i2,i3))-9.*(ry(i1+
     & 2,i2,i3)-ry(i1-2,i2,i3))+(ry(i1+3,i2,i3)-ry(i1-3,i2,i3)))*d16(
     & 0)
        rys6(i1,i2,i3)=(45.*(ry(i1,i2+1,i3)-ry(i1,i2-1,i3))-9.*(ry(i1,
     & i2+2,i3)-ry(i1,i2-2,i3))+(ry(i1,i2+3,i3)-ry(i1,i2-3,i3)))*d16(
     & 1)
        ryt6(i1,i2,i3)=(45.*(ry(i1,i2,i3+1)-ry(i1,i2,i3-1))-9.*(ry(i1,
     & i2,i3+2)-ry(i1,i2,i3-2))+(ry(i1,i2,i3+3)-ry(i1,i2,i3-3)))*d16(
     & 2)
        rzr6(i1,i2,i3)=(45.*(rz(i1+1,i2,i3)-rz(i1-1,i2,i3))-9.*(rz(i1+
     & 2,i2,i3)-rz(i1-2,i2,i3))+(rz(i1+3,i2,i3)-rz(i1-3,i2,i3)))*d16(
     & 0)
        rzs6(i1,i2,i3)=(45.*(rz(i1,i2+1,i3)-rz(i1,i2-1,i3))-9.*(rz(i1,
     & i2+2,i3)-rz(i1,i2-2,i3))+(rz(i1,i2+3,i3)-rz(i1,i2-3,i3)))*d16(
     & 1)
        rzt6(i1,i2,i3)=(45.*(rz(i1,i2,i3+1)-rz(i1,i2,i3-1))-9.*(rz(i1,
     & i2,i3+2)-rz(i1,i2,i3-2))+(rz(i1,i2,i3+3)-rz(i1,i2,i3-3)))*d16(
     & 2)
        sxr6(i1,i2,i3)=(45.*(sx(i1+1,i2,i3)-sx(i1-1,i2,i3))-9.*(sx(i1+
     & 2,i2,i3)-sx(i1-2,i2,i3))+(sx(i1+3,i2,i3)-sx(i1-3,i2,i3)))*d16(
     & 0)
        sxs6(i1,i2,i3)=(45.*(sx(i1,i2+1,i3)-sx(i1,i2-1,i3))-9.*(sx(i1,
     & i2+2,i3)-sx(i1,i2-2,i3))+(sx(i1,i2+3,i3)-sx(i1,i2-3,i3)))*d16(
     & 1)
        sxt6(i1,i2,i3)=(45.*(sx(i1,i2,i3+1)-sx(i1,i2,i3-1))-9.*(sx(i1,
     & i2,i3+2)-sx(i1,i2,i3-2))+(sx(i1,i2,i3+3)-sx(i1,i2,i3-3)))*d16(
     & 2)
        syr6(i1,i2,i3)=(45.*(sy(i1+1,i2,i3)-sy(i1-1,i2,i3))-9.*(sy(i1+
     & 2,i2,i3)-sy(i1-2,i2,i3))+(sy(i1+3,i2,i3)-sy(i1-3,i2,i3)))*d16(
     & 0)
        sys6(i1,i2,i3)=(45.*(sy(i1,i2+1,i3)-sy(i1,i2-1,i3))-9.*(sy(i1,
     & i2+2,i3)-sy(i1,i2-2,i3))+(sy(i1,i2+3,i3)-sy(i1,i2-3,i3)))*d16(
     & 1)
        syt6(i1,i2,i3)=(45.*(sy(i1,i2,i3+1)-sy(i1,i2,i3-1))-9.*(sy(i1,
     & i2,i3+2)-sy(i1,i2,i3-2))+(sy(i1,i2,i3+3)-sy(i1,i2,i3-3)))*d16(
     & 2)
        szr6(i1,i2,i3)=(45.*(sz(i1+1,i2,i3)-sz(i1-1,i2,i3))-9.*(sz(i1+
     & 2,i2,i3)-sz(i1-2,i2,i3))+(sz(i1+3,i2,i3)-sz(i1-3,i2,i3)))*d16(
     & 0)
        szs6(i1,i2,i3)=(45.*(sz(i1,i2+1,i3)-sz(i1,i2-1,i3))-9.*(sz(i1,
     & i2+2,i3)-sz(i1,i2-2,i3))+(sz(i1,i2+3,i3)-sz(i1,i2-3,i3)))*d16(
     & 1)
        szt6(i1,i2,i3)=(45.*(sz(i1,i2,i3+1)-sz(i1,i2,i3-1))-9.*(sz(i1,
     & i2,i3+2)-sz(i1,i2,i3-2))+(sz(i1,i2,i3+3)-sz(i1,i2,i3-3)))*d16(
     & 2)
        txr6(i1,i2,i3)=(45.*(tx(i1+1,i2,i3)-tx(i1-1,i2,i3))-9.*(tx(i1+
     & 2,i2,i3)-tx(i1-2,i2,i3))+(tx(i1+3,i2,i3)-tx(i1-3,i2,i3)))*d16(
     & 0)
        txs6(i1,i2,i3)=(45.*(tx(i1,i2+1,i3)-tx(i1,i2-1,i3))-9.*(tx(i1,
     & i2+2,i3)-tx(i1,i2-2,i3))+(tx(i1,i2+3,i3)-tx(i1,i2-3,i3)))*d16(
     & 1)
        txt6(i1,i2,i3)=(45.*(tx(i1,i2,i3+1)-tx(i1,i2,i3-1))-9.*(tx(i1,
     & i2,i3+2)-tx(i1,i2,i3-2))+(tx(i1,i2,i3+3)-tx(i1,i2,i3-3)))*d16(
     & 2)
        tyr6(i1,i2,i3)=(45.*(ty(i1+1,i2,i3)-ty(i1-1,i2,i3))-9.*(ty(i1+
     & 2,i2,i3)-ty(i1-2,i2,i3))+(ty(i1+3,i2,i3)-ty(i1-3,i2,i3)))*d16(
     & 0)
        tys6(i1,i2,i3)=(45.*(ty(i1,i2+1,i3)-ty(i1,i2-1,i3))-9.*(ty(i1,
     & i2+2,i3)-ty(i1,i2-2,i3))+(ty(i1,i2+3,i3)-ty(i1,i2-3,i3)))*d16(
     & 1)
        tyt6(i1,i2,i3)=(45.*(ty(i1,i2,i3+1)-ty(i1,i2,i3-1))-9.*(ty(i1,
     & i2,i3+2)-ty(i1,i2,i3-2))+(ty(i1,i2,i3+3)-ty(i1,i2,i3-3)))*d16(
     & 2)
        tzr6(i1,i2,i3)=(45.*(tz(i1+1,i2,i3)-tz(i1-1,i2,i3))-9.*(tz(i1+
     & 2,i2,i3)-tz(i1-2,i2,i3))+(tz(i1+3,i2,i3)-tz(i1-3,i2,i3)))*d16(
     & 0)
        tzs6(i1,i2,i3)=(45.*(tz(i1,i2+1,i3)-tz(i1,i2-1,i3))-9.*(tz(i1,
     & i2+2,i3)-tz(i1,i2-2,i3))+(tz(i1,i2+3,i3)-tz(i1,i2-3,i3)))*d16(
     & 1)
        tzt6(i1,i2,i3)=(45.*(tz(i1,i2,i3+1)-tz(i1,i2,i3-1))-9.*(tz(i1,
     & i2,i3+2)-tz(i1,i2,i3-2))+(tz(i1,i2,i3+3)-tz(i1,i2,i3-3)))*d16(
     & 2)
        ux61(i1,i2,i3,kd)= rx(i1,i2,i3)*ur6(i1,i2,i3,kd)
        uy61(i1,i2,i3,kd)=0
        uz61(i1,i2,i3,kd)=0
        ux62(i1,i2,i3,kd)= rx(i1,i2,i3)*ur6(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & us6(i1,i2,i3,kd)
        uy62(i1,i2,i3,kd)= ry(i1,i2,i3)*ur6(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & us6(i1,i2,i3,kd)
        uz62(i1,i2,i3,kd)=0
        ux63(i1,i2,i3,kd)=rx(i1,i2,i3)*ur6(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & us6(i1,i2,i3,kd)+tx(i1,i2,i3)*ut6(i1,i2,i3,kd)
        uy63(i1,i2,i3,kd)=ry(i1,i2,i3)*ur6(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & us6(i1,i2,i3,kd)+ty(i1,i2,i3)*ut6(i1,i2,i3,kd)
        uz63(i1,i2,i3,kd)=rz(i1,i2,i3)*ur6(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & us6(i1,i2,i3,kd)+tz(i1,i2,i3)*ut6(i1,i2,i3,kd)
! #If "RX" == "RX"
        rxx61(i1,i2,i3)= rx(i1,i2,i3)*rxr6(i1,i2,i3)
        rxx62(i1,i2,i3)= rx(i1,i2,i3)*rxr6(i1,i2,i3)+sx(i1,i2,i3)*rxs6(
     & i1,i2,i3)
        rxy62(i1,i2,i3)= ry(i1,i2,i3)*rxr6(i1,i2,i3)+sy(i1,i2,i3)*rxs6(
     & i1,i2,i3)
        rxx63(i1,i2,i3)=rx(i1,i2,i3)*rxr6(i1,i2,i3)+sx(i1,i2,i3)*rxs6(
     & i1,i2,i3)+tx(i1,i2,i3)*rxt6(i1,i2,i3)
        rxy63(i1,i2,i3)=ry(i1,i2,i3)*rxr6(i1,i2,i3)+sy(i1,i2,i3)*rxs6(
     & i1,i2,i3)+ty(i1,i2,i3)*rxt6(i1,i2,i3)
        rxz63(i1,i2,i3)=rz(i1,i2,i3)*rxr6(i1,i2,i3)+sz(i1,i2,i3)*rxs6(
     & i1,i2,i3)+tz(i1,i2,i3)*rxt6(i1,i2,i3)
        ryx62(i1,i2,i3)= rx(i1,i2,i3)*ryr6(i1,i2,i3)+sx(i1,i2,i3)*rys6(
     & i1,i2,i3)
        ryy62(i1,i2,i3)= ry(i1,i2,i3)*ryr6(i1,i2,i3)+sy(i1,i2,i3)*rys6(
     & i1,i2,i3)
        ryx63(i1,i2,i3)=rx(i1,i2,i3)*ryr6(i1,i2,i3)+sx(i1,i2,i3)*rys6(
     & i1,i2,i3)+tx(i1,i2,i3)*ryt6(i1,i2,i3)
        ryy63(i1,i2,i3)=ry(i1,i2,i3)*ryr6(i1,i2,i3)+sy(i1,i2,i3)*rys6(
     & i1,i2,i3)+ty(i1,i2,i3)*ryt6(i1,i2,i3)
        ryz63(i1,i2,i3)=rz(i1,i2,i3)*ryr6(i1,i2,i3)+sz(i1,i2,i3)*rys6(
     & i1,i2,i3)+tz(i1,i2,i3)*ryt6(i1,i2,i3)
        rzx62(i1,i2,i3)= rx(i1,i2,i3)*rzr6(i1,i2,i3)+sx(i1,i2,i3)*rzs6(
     & i1,i2,i3)
        rzy62(i1,i2,i3)= ry(i1,i2,i3)*rzr6(i1,i2,i3)+sy(i1,i2,i3)*rzs6(
     & i1,i2,i3)
        rzx63(i1,i2,i3)=rx(i1,i2,i3)*rzr6(i1,i2,i3)+sx(i1,i2,i3)*rzs6(
     & i1,i2,i3)+tx(i1,i2,i3)*rzt6(i1,i2,i3)
        rzy63(i1,i2,i3)=ry(i1,i2,i3)*rzr6(i1,i2,i3)+sy(i1,i2,i3)*rzs6(
     & i1,i2,i3)+ty(i1,i2,i3)*rzt6(i1,i2,i3)
        rzz63(i1,i2,i3)=rz(i1,i2,i3)*rzr6(i1,i2,i3)+sz(i1,i2,i3)*rzs6(
     & i1,i2,i3)+tz(i1,i2,i3)*rzt6(i1,i2,i3)
        sxx62(i1,i2,i3)= rx(i1,i2,i3)*sxr6(i1,i2,i3)+sx(i1,i2,i3)*sxs6(
     & i1,i2,i3)
        sxy62(i1,i2,i3)= ry(i1,i2,i3)*sxr6(i1,i2,i3)+sy(i1,i2,i3)*sxs6(
     & i1,i2,i3)
        sxx63(i1,i2,i3)=rx(i1,i2,i3)*sxr6(i1,i2,i3)+sx(i1,i2,i3)*sxs6(
     & i1,i2,i3)+tx(i1,i2,i3)*sxt6(i1,i2,i3)
        sxy63(i1,i2,i3)=ry(i1,i2,i3)*sxr6(i1,i2,i3)+sy(i1,i2,i3)*sxs6(
     & i1,i2,i3)+ty(i1,i2,i3)*sxt6(i1,i2,i3)
        sxz63(i1,i2,i3)=rz(i1,i2,i3)*sxr6(i1,i2,i3)+sz(i1,i2,i3)*sxs6(
     & i1,i2,i3)+tz(i1,i2,i3)*sxt6(i1,i2,i3)
        syx62(i1,i2,i3)= rx(i1,i2,i3)*syr6(i1,i2,i3)+sx(i1,i2,i3)*sys6(
     & i1,i2,i3)
        syy62(i1,i2,i3)= ry(i1,i2,i3)*syr6(i1,i2,i3)+sy(i1,i2,i3)*sys6(
     & i1,i2,i3)
        syx63(i1,i2,i3)=rx(i1,i2,i3)*syr6(i1,i2,i3)+sx(i1,i2,i3)*sys6(
     & i1,i2,i3)+tx(i1,i2,i3)*syt6(i1,i2,i3)
        syy63(i1,i2,i3)=ry(i1,i2,i3)*syr6(i1,i2,i3)+sy(i1,i2,i3)*sys6(
     & i1,i2,i3)+ty(i1,i2,i3)*syt6(i1,i2,i3)
        syz63(i1,i2,i3)=rz(i1,i2,i3)*syr6(i1,i2,i3)+sz(i1,i2,i3)*sys6(
     & i1,i2,i3)+tz(i1,i2,i3)*syt6(i1,i2,i3)
        szx62(i1,i2,i3)= rx(i1,i2,i3)*szr6(i1,i2,i3)+sx(i1,i2,i3)*szs6(
     & i1,i2,i3)
        szy62(i1,i2,i3)= ry(i1,i2,i3)*szr6(i1,i2,i3)+sy(i1,i2,i3)*szs6(
     & i1,i2,i3)
        szx63(i1,i2,i3)=rx(i1,i2,i3)*szr6(i1,i2,i3)+sx(i1,i2,i3)*szs6(
     & i1,i2,i3)+tx(i1,i2,i3)*szt6(i1,i2,i3)
        szy63(i1,i2,i3)=ry(i1,i2,i3)*szr6(i1,i2,i3)+sy(i1,i2,i3)*szs6(
     & i1,i2,i3)+ty(i1,i2,i3)*szt6(i1,i2,i3)
        szz63(i1,i2,i3)=rz(i1,i2,i3)*szr6(i1,i2,i3)+sz(i1,i2,i3)*szs6(
     & i1,i2,i3)+tz(i1,i2,i3)*szt6(i1,i2,i3)
        txx62(i1,i2,i3)= rx(i1,i2,i3)*txr6(i1,i2,i3)+sx(i1,i2,i3)*txs6(
     & i1,i2,i3)
        txy62(i1,i2,i3)= ry(i1,i2,i3)*txr6(i1,i2,i3)+sy(i1,i2,i3)*txs6(
     & i1,i2,i3)
        txx63(i1,i2,i3)=rx(i1,i2,i3)*txr6(i1,i2,i3)+sx(i1,i2,i3)*txs6(
     & i1,i2,i3)+tx(i1,i2,i3)*txt6(i1,i2,i3)
        txy63(i1,i2,i3)=ry(i1,i2,i3)*txr6(i1,i2,i3)+sy(i1,i2,i3)*txs6(
     & i1,i2,i3)+ty(i1,i2,i3)*txt6(i1,i2,i3)
        txz63(i1,i2,i3)=rz(i1,i2,i3)*txr6(i1,i2,i3)+sz(i1,i2,i3)*txs6(
     & i1,i2,i3)+tz(i1,i2,i3)*txt6(i1,i2,i3)
        tyx62(i1,i2,i3)= rx(i1,i2,i3)*tyr6(i1,i2,i3)+sx(i1,i2,i3)*tys6(
     & i1,i2,i3)
        tyy62(i1,i2,i3)= ry(i1,i2,i3)*tyr6(i1,i2,i3)+sy(i1,i2,i3)*tys6(
     & i1,i2,i3)
        tyx63(i1,i2,i3)=rx(i1,i2,i3)*tyr6(i1,i2,i3)+sx(i1,i2,i3)*tys6(
     & i1,i2,i3)+tx(i1,i2,i3)*tyt6(i1,i2,i3)
        tyy63(i1,i2,i3)=ry(i1,i2,i3)*tyr6(i1,i2,i3)+sy(i1,i2,i3)*tys6(
     & i1,i2,i3)+ty(i1,i2,i3)*tyt6(i1,i2,i3)
        tyz63(i1,i2,i3)=rz(i1,i2,i3)*tyr6(i1,i2,i3)+sz(i1,i2,i3)*tys6(
     & i1,i2,i3)+tz(i1,i2,i3)*tyt6(i1,i2,i3)
        tzx62(i1,i2,i3)= rx(i1,i2,i3)*tzr6(i1,i2,i3)+sx(i1,i2,i3)*tzs6(
     & i1,i2,i3)
        tzy62(i1,i2,i3)= ry(i1,i2,i3)*tzr6(i1,i2,i3)+sy(i1,i2,i3)*tzs6(
     & i1,i2,i3)
        tzx63(i1,i2,i3)=rx(i1,i2,i3)*tzr6(i1,i2,i3)+sx(i1,i2,i3)*tzs6(
     & i1,i2,i3)+tx(i1,i2,i3)*tzt6(i1,i2,i3)
        tzy63(i1,i2,i3)=ry(i1,i2,i3)*tzr6(i1,i2,i3)+sy(i1,i2,i3)*tzs6(
     & i1,i2,i3)+ty(i1,i2,i3)*tzt6(i1,i2,i3)
        tzz63(i1,i2,i3)=rz(i1,i2,i3)*tzr6(i1,i2,i3)+sz(i1,i2,i3)*tzs6(
     & i1,i2,i3)+tz(i1,i2,i3)*tzt6(i1,i2,i3)
        uxx61(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*urr6(i1,i2,i3,kd)+(rxx62(
     & i1,i2,i3))*ur6(i1,i2,i3,kd)
        uyy61(i1,i2,i3,kd)=0
        uxy61(i1,i2,i3,kd)=0
        uxz61(i1,i2,i3,kd)=0
        uyz61(i1,i2,i3,kd)=0
        uzz61(i1,i2,i3,kd)=0
        ulaplacian61(i1,i2,i3,kd)=uxx61(i1,i2,i3,kd)
        uxx62(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*urr6(i1,i2,i3,kd)+2.*(rx(
     & i1,i2,i3)*sx(i1,i2,i3))*urs6(i1,i2,i3,kd)+(sx(i1,i2,i3)**2)*
     & uss6(i1,i2,i3,kd)+(rxx62(i1,i2,i3))*ur6(i1,i2,i3,kd)+(sxx62(i1,
     & i2,i3))*us6(i1,i2,i3,kd)
        uyy62(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*urr6(i1,i2,i3,kd)+2.*(ry(
     & i1,i2,i3)*sy(i1,i2,i3))*urs6(i1,i2,i3,kd)+(sy(i1,i2,i3)**2)*
     & uss6(i1,i2,i3,kd)+(ryy62(i1,i2,i3))*ur6(i1,i2,i3,kd)+(syy62(i1,
     & i2,i3))*us6(i1,i2,i3,kd)
        uxy62(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*urr6(i1,i2,i3,kd)+
     & (rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*urs6(i1,
     & i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*uss6(i1,i2,i3,kd)+rxy62(i1,
     & i2,i3)*ur6(i1,i2,i3,kd)+sxy62(i1,i2,i3)*us6(i1,i2,i3,kd)
        uxz62(i1,i2,i3,kd)=0
        uyz62(i1,i2,i3,kd)=0
        uzz62(i1,i2,i3,kd)=0
        ulaplacian62(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & urr6(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*
     & sy(i1,i2,i3))*urs6(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)**
     & 2)*uss6(i1,i2,i3,kd)+(rxx62(i1,i2,i3)+ryy62(i1,i2,i3))*ur6(i1,
     & i2,i3,kd)+(sxx62(i1,i2,i3)+syy62(i1,i2,i3))*us6(i1,i2,i3,kd)
        uxx63(i1,i2,i3,kd)=rx(i1,i2,i3)**2*urr6(i1,i2,i3,kd)+sx(i1,i2,
     & i3)**2*uss6(i1,i2,i3,kd)+tx(i1,i2,i3)**2*utt6(i1,i2,i3,kd)+2.*
     & rx(i1,i2,i3)*sx(i1,i2,i3)*urs6(i1,i2,i3,kd)+2.*rx(i1,i2,i3)*tx(
     & i1,i2,i3)*urt6(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,i2,i3)*ust6(
     & i1,i2,i3,kd)+rxx63(i1,i2,i3)*ur6(i1,i2,i3,kd)+sxx63(i1,i2,i3)*
     & us6(i1,i2,i3,kd)+txx63(i1,i2,i3)*ut6(i1,i2,i3,kd)
        uyy63(i1,i2,i3,kd)=ry(i1,i2,i3)**2*urr6(i1,i2,i3,kd)+sy(i1,i2,
     & i3)**2*uss6(i1,i2,i3,kd)+ty(i1,i2,i3)**2*utt6(i1,i2,i3,kd)+2.*
     & ry(i1,i2,i3)*sy(i1,i2,i3)*urs6(i1,i2,i3,kd)+2.*ry(i1,i2,i3)*ty(
     & i1,i2,i3)*urt6(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,i2,i3)*ust6(
     & i1,i2,i3,kd)+ryy63(i1,i2,i3)*ur6(i1,i2,i3,kd)+syy63(i1,i2,i3)*
     & us6(i1,i2,i3,kd)+tyy63(i1,i2,i3)*ut6(i1,i2,i3,kd)
        uzz63(i1,i2,i3,kd)=rz(i1,i2,i3)**2*urr6(i1,i2,i3,kd)+sz(i1,i2,
     & i3)**2*uss6(i1,i2,i3,kd)+tz(i1,i2,i3)**2*utt6(i1,i2,i3,kd)+2.*
     & rz(i1,i2,i3)*sz(i1,i2,i3)*urs6(i1,i2,i3,kd)+2.*rz(i1,i2,i3)*tz(
     & i1,i2,i3)*urt6(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,i2,i3)*ust6(
     & i1,i2,i3,kd)+rzz63(i1,i2,i3)*ur6(i1,i2,i3,kd)+szz63(i1,i2,i3)*
     & us6(i1,i2,i3,kd)+tzz63(i1,i2,i3)*ut6(i1,i2,i3,kd)
        uxy63(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*urr6(i1,i2,i3,kd)+
     & sx(i1,i2,i3)*sy(i1,i2,i3)*uss6(i1,i2,i3,kd)+tx(i1,i2,i3)*ty(i1,
     & i2,i3)*utt6(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,
     & i3)*sx(i1,i2,i3))*urs6(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(i1,i2,i3)+
     & ry(i1,i2,i3)*tx(i1,i2,i3))*urt6(i1,i2,i3,kd)+(sx(i1,i2,i3)*ty(
     & i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*ust6(i1,i2,i3,kd)+rxy63(
     & i1,i2,i3)*ur6(i1,i2,i3,kd)+sxy63(i1,i2,i3)*us6(i1,i2,i3,kd)+
     & txy63(i1,i2,i3)*ut6(i1,i2,i3,kd)
        uxz63(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*urr6(i1,i2,i3,kd)+
     & sx(i1,i2,i3)*sz(i1,i2,i3)*uss6(i1,i2,i3,kd)+tx(i1,i2,i3)*tz(i1,
     & i2,i3)*utt6(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,i2,
     & i3)*sx(i1,i2,i3))*urs6(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(i1,i2,i3)+
     & rz(i1,i2,i3)*tx(i1,i2,i3))*urt6(i1,i2,i3,kd)+(sx(i1,i2,i3)*tz(
     & i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*ust6(i1,i2,i3,kd)+rxz63(
     & i1,i2,i3)*ur6(i1,i2,i3,kd)+sxz63(i1,i2,i3)*us6(i1,i2,i3,kd)+
     & txz63(i1,i2,i3)*ut6(i1,i2,i3,kd)
        uyz63(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*urr6(i1,i2,i3,kd)+
     & sy(i1,i2,i3)*sz(i1,i2,i3)*uss6(i1,i2,i3,kd)+ty(i1,i2,i3)*tz(i1,
     & i2,i3)*utt6(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,i2,
     & i3)*sy(i1,i2,i3))*urs6(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(i1,i2,i3)+
     & rz(i1,i2,i3)*ty(i1,i2,i3))*urt6(i1,i2,i3,kd)+(sy(i1,i2,i3)*tz(
     & i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*ust6(i1,i2,i3,kd)+ryz63(
     & i1,i2,i3)*ur6(i1,i2,i3,kd)+syz63(i1,i2,i3)*us6(i1,i2,i3,kd)+
     & tyz63(i1,i2,i3)*ut6(i1,i2,i3,kd)
        ulaplacian63(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*urr6(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)**
     & 2+sz(i1,i2,i3)**2)*uss6(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(i1,i2,
     & i3)**2+tz(i1,i2,i3)**2)*utt6(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(
     & i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,i3))
     & *urs6(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(i1,i2,i3)*
     & ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*urt6(i1,i2,i3,kd)+2.*(
     & sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)+sz(i1,i2,
     & i3)*tz(i1,i2,i3))*ust6(i1,i2,i3,kd)+(rxx63(i1,i2,i3)+ryy63(i1,
     & i2,i3)+rzz63(i1,i2,i3))*ur6(i1,i2,i3,kd)+(sxx63(i1,i2,i3)+
     & syy63(i1,i2,i3)+szz63(i1,i2,i3))*us6(i1,i2,i3,kd)+(txx63(i1,i2,
     & i3)+tyy63(i1,i2,i3)+tzz63(i1,i2,i3))*ut6(i1,i2,i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
! #If "RX" == "RX"
        h16(kd) = 1./(60.*dx(kd))
        h26(kd) = 1./(180.*dx(kd)**2)
        ux63r(i1,i2,i3,kd)=(45.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))-9.*
     & (u(i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd))+(u(i1+3,i2,i3,kd)-u(i1-3,
     & i2,i3,kd)))*h16(0)
        uy63r(i1,i2,i3,kd)=(45.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))-9.*
     & (u(i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd))+(u(i1,i2+3,i3,kd)-u(i1,i2-
     & 3,i3,kd)))*h16(1)
        uz63r(i1,i2,i3,kd)=(45.*(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))-9.*
     & (u(i1,i2,i3+2,kd)-u(i1,i2,i3-2,kd))+(u(i1,i2,i3+3,kd)-u(i1,i2,
     & i3-3,kd)))*h16(2)
        uxx63r(i1,i2,i3,kd)=(-490.*u(i1,i2,i3,kd)+270.*(u(i1+1,i2,i3,
     & kd)+u(i1-1,i2,i3,kd))-27.*(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd))+
     & 2.*(u(i1+3,i2,i3,kd)+u(i1-3,i2,i3,kd)) )*h26(0)
        uyy63r(i1,i2,i3,kd)=(-490.*u(i1,i2,i3,kd)+270.*(u(i1,i2+1,i3,
     & kd)+u(i1,i2-1,i3,kd))-27.*(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd))+
     & 2.*(u(i1,i2+3,i3,kd)+u(i1,i2-3,i3,kd)) )*h26(1)
        uzz63r(i1,i2,i3,kd)=(-490.*u(i1,i2,i3,kd)+270.*(u(i1,i2,i3+1,
     & kd)+u(i1,i2,i3-1,kd))-27.*(u(i1,i2,i3+2,kd)+u(i1,i2,i3-2,kd))+
     & 2.*(u(i1,i2,i3+3,kd)+u(i1,i2,i3-3,kd)) )*h26(2)
        uxy63r(i1,i2,i3,kd)=(45.*(ux63r(i1,i2+1,i3,kd)-ux63r(i1,i2-1,
     & i3,kd))-9.*(ux63r(i1,i2+2,i3,kd)-ux63r(i1,i2-2,i3,kd))+(ux63r(
     & i1,i2+3,i3,kd)-ux63r(i1,i2-3,i3,kd)))*h16(1)
        uxz63r(i1,i2,i3,kd)=(45.*(ux63r(i1,i2,i3+1,kd)-ux63r(i1,i2,i3-
     & 1,kd))-9.*(ux63r(i1,i2,i3+2,kd)-ux63r(i1,i2,i3-2,kd))+(ux63r(
     & i1,i2,i3+3,kd)-ux63r(i1,i2,i3-3,kd)))*h16(2)
        uyz63r(i1,i2,i3,kd)=(45.*(uy63r(i1,i2,i3+1,kd)-uy63r(i1,i2,i3-
     & 1,kd))-9.*(uy63r(i1,i2,i3+2,kd)-uy63r(i1,i2,i3-2,kd))+(uy63r(
     & i1,i2,i3+3,kd)-uy63r(i1,i2,i3-3,kd)))*h16(2)
        ux61r(i1,i2,i3,kd)= ux63r(i1,i2,i3,kd)
        uy61r(i1,i2,i3,kd)= uy63r(i1,i2,i3,kd)
        uz61r(i1,i2,i3,kd)= uz63r(i1,i2,i3,kd)
        uxx61r(i1,i2,i3,kd)= uxx63r(i1,i2,i3,kd)
        uyy61r(i1,i2,i3,kd)= uyy63r(i1,i2,i3,kd)
        uzz61r(i1,i2,i3,kd)= uzz63r(i1,i2,i3,kd)
        uxy61r(i1,i2,i3,kd)= uxy63r(i1,i2,i3,kd)
        uxz61r(i1,i2,i3,kd)= uxz63r(i1,i2,i3,kd)
        uyz61r(i1,i2,i3,kd)= uyz63r(i1,i2,i3,kd)
        ulaplacian61r(i1,i2,i3,kd)=uxx63r(i1,i2,i3,kd)
        ux62r(i1,i2,i3,kd)= ux63r(i1,i2,i3,kd)
        uy62r(i1,i2,i3,kd)= uy63r(i1,i2,i3,kd)
        uz62r(i1,i2,i3,kd)= uz63r(i1,i2,i3,kd)
        uxx62r(i1,i2,i3,kd)= uxx63r(i1,i2,i3,kd)
        uyy62r(i1,i2,i3,kd)= uyy63r(i1,i2,i3,kd)
        uzz62r(i1,i2,i3,kd)= uzz63r(i1,i2,i3,kd)
        uxy62r(i1,i2,i3,kd)= uxy63r(i1,i2,i3,kd)
        uxz62r(i1,i2,i3,kd)= uxz63r(i1,i2,i3,kd)
        uyz62r(i1,i2,i3,kd)= uyz63r(i1,i2,i3,kd)
        ulaplacian62r(i1,i2,i3,kd)=uxx63r(i1,i2,i3,kd)+uyy63r(i1,i2,i3,
     & kd)
        ulaplacian63r(i1,i2,i3,kd)=uxx63r(i1,i2,i3,kd)+uyy63r(i1,i2,i3,
     & kd)+uzz63r(i1,i2,i3,kd)
! defineDifferenceOrder8Components1(u,RX)
! #If "RX" == "RX"
        d18(kd) = 1./(840.*dr(kd))
        d28(kd) = 1./(5040.*dr(kd)**2)
        ur8(i1,i2,i3,kd)=(672.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))-
     & 168.*(u(i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd))+32.*(u(i1+3,i2,i3,kd)-
     & u(i1-3,i2,i3,kd))-3.*(u(i1+4,i2,i3,kd)-u(i1-4,i2,i3,kd)))*d18(
     & 0)
        us8(i1,i2,i3,kd)=(672.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))-
     & 168.*(u(i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd))+32.*(u(i1,i2+3,i3,kd)-
     & u(i1,i2-3,i3,kd))-3.*(u(i1,i2+4,i3,kd)-u(i1,i2-4,i3,kd)))*d18(
     & 1)
        ut8(i1,i2,i3,kd)=(672.*(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))-
     & 168.*(u(i1,i2,i3+2,kd)-u(i1,i2,i3-2,kd))+32.*(u(i1,i2,i3+3,kd)-
     & u(i1,i2,i3-3,kd))-3.*(u(i1,i2,i3+4,kd)-u(i1,i2,i3-4,kd)))*d18(
     & 2)
        urr8(i1,i2,i3,kd)=(-14350.*u(i1,i2,i3,kd)+8064.*(u(i1+1,i2,i3,
     & kd)+u(i1-1,i2,i3,kd))-1008.*(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd))
     & +128.*(u(i1+3,i2,i3,kd)+u(i1-3,i2,i3,kd))-9.*(u(i1+4,i2,i3,kd)+
     & u(i1-4,i2,i3,kd)) )*d28(0)
        uss8(i1,i2,i3,kd)=(-14350.*u(i1,i2,i3,kd)+8064.*(u(i1,i2+1,i3,
     & kd)+u(i1,i2-1,i3,kd))-1008.*(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd))
     & +128.*(u(i1,i2+3,i3,kd)+u(i1,i2-3,i3,kd))-9.*(u(i1,i2+4,i3,kd)+
     & u(i1,i2-4,i3,kd)) )*d28(1)
        utt8(i1,i2,i3,kd)=(-14350.*u(i1,i2,i3,kd)+8064.*(u(i1,i2,i3+1,
     & kd)+u(i1,i2,i3-1,kd))-1008.*(u(i1,i2,i3+2,kd)+u(i1,i2,i3-2,kd))
     & +128.*(u(i1,i2,i3+3,kd)+u(i1,i2,i3-3,kd))-9.*(u(i1,i2,i3+4,kd)+
     & u(i1,i2,i3-4,kd)) )*d28(2)
        urs8(i1,i2,i3,kd)=(672.*(ur8(i1,i2+1,i3,kd)-ur8(i1,i2-1,i3,kd))
     & -168.*(ur8(i1,i2+2,i3,kd)-ur8(i1,i2-2,i3,kd))+32.*(ur8(i1,i2+3,
     & i3,kd)-ur8(i1,i2-3,i3,kd))-3.*(ur8(i1,i2+4,i3,kd)-ur8(i1,i2-4,
     & i3,kd)))*d18(1)
        urt8(i1,i2,i3,kd)=(672.*(ur8(i1,i2,i3+1,kd)-ur8(i1,i2,i3-1,kd))
     & -168.*(ur8(i1,i2,i3+2,kd)-ur8(i1,i2,i3-2,kd))+32.*(ur8(i1,i2,
     & i3+3,kd)-ur8(i1,i2,i3-3,kd))-3.*(ur8(i1,i2,i3+4,kd)-ur8(i1,i2,
     & i3-4,kd)))*d18(2)
        ust8(i1,i2,i3,kd)=(672.*(us8(i1,i2,i3+1,kd)-us8(i1,i2,i3-1,kd))
     & -168.*(us8(i1,i2,i3+2,kd)-us8(i1,i2,i3-2,kd))+32.*(us8(i1,i2,
     & i3+3,kd)-us8(i1,i2,i3-3,kd))-3.*(us8(i1,i2,i3+4,kd)-us8(i1,i2,
     & i3-4,kd)))*d18(2)
! #If "RX" == "RX"
        rxr8(i1,i2,i3)=(672.*(rx(i1+1,i2,i3)-rx(i1-1,i2,i3))-168.*(rx(
     & i1+2,i2,i3)-rx(i1-2,i2,i3))+32.*(rx(i1+3,i2,i3)-rx(i1-3,i2,i3))
     & -3.*(rx(i1+4,i2,i3)-rx(i1-4,i2,i3)))*d18(0)
        rxs8(i1,i2,i3)=(672.*(rx(i1,i2+1,i3)-rx(i1,i2-1,i3))-168.*(rx(
     & i1,i2+2,i3)-rx(i1,i2-2,i3))+32.*(rx(i1,i2+3,i3)-rx(i1,i2-3,i3))
     & -3.*(rx(i1,i2+4,i3)-rx(i1,i2-4,i3)))*d18(1)
        rxt8(i1,i2,i3)=(672.*(rx(i1,i2,i3+1)-rx(i1,i2,i3-1))-168.*(rx(
     & i1,i2,i3+2)-rx(i1,i2,i3-2))+32.*(rx(i1,i2,i3+3)-rx(i1,i2,i3-3))
     & -3.*(rx(i1,i2,i3+4)-rx(i1,i2,i3-4)))*d18(2)
        ryr8(i1,i2,i3)=(672.*(ry(i1+1,i2,i3)-ry(i1-1,i2,i3))-168.*(ry(
     & i1+2,i2,i3)-ry(i1-2,i2,i3))+32.*(ry(i1+3,i2,i3)-ry(i1-3,i2,i3))
     & -3.*(ry(i1+4,i2,i3)-ry(i1-4,i2,i3)))*d18(0)
        rys8(i1,i2,i3)=(672.*(ry(i1,i2+1,i3)-ry(i1,i2-1,i3))-168.*(ry(
     & i1,i2+2,i3)-ry(i1,i2-2,i3))+32.*(ry(i1,i2+3,i3)-ry(i1,i2-3,i3))
     & -3.*(ry(i1,i2+4,i3)-ry(i1,i2-4,i3)))*d18(1)
        ryt8(i1,i2,i3)=(672.*(ry(i1,i2,i3+1)-ry(i1,i2,i3-1))-168.*(ry(
     & i1,i2,i3+2)-ry(i1,i2,i3-2))+32.*(ry(i1,i2,i3+3)-ry(i1,i2,i3-3))
     & -3.*(ry(i1,i2,i3+4)-ry(i1,i2,i3-4)))*d18(2)
        rzr8(i1,i2,i3)=(672.*(rz(i1+1,i2,i3)-rz(i1-1,i2,i3))-168.*(rz(
     & i1+2,i2,i3)-rz(i1-2,i2,i3))+32.*(rz(i1+3,i2,i3)-rz(i1-3,i2,i3))
     & -3.*(rz(i1+4,i2,i3)-rz(i1-4,i2,i3)))*d18(0)
        rzs8(i1,i2,i3)=(672.*(rz(i1,i2+1,i3)-rz(i1,i2-1,i3))-168.*(rz(
     & i1,i2+2,i3)-rz(i1,i2-2,i3))+32.*(rz(i1,i2+3,i3)-rz(i1,i2-3,i3))
     & -3.*(rz(i1,i2+4,i3)-rz(i1,i2-4,i3)))*d18(1)
        rzt8(i1,i2,i3)=(672.*(rz(i1,i2,i3+1)-rz(i1,i2,i3-1))-168.*(rz(
     & i1,i2,i3+2)-rz(i1,i2,i3-2))+32.*(rz(i1,i2,i3+3)-rz(i1,i2,i3-3))
     & -3.*(rz(i1,i2,i3+4)-rz(i1,i2,i3-4)))*d18(2)
        sxr8(i1,i2,i3)=(672.*(sx(i1+1,i2,i3)-sx(i1-1,i2,i3))-168.*(sx(
     & i1+2,i2,i3)-sx(i1-2,i2,i3))+32.*(sx(i1+3,i2,i3)-sx(i1-3,i2,i3))
     & -3.*(sx(i1+4,i2,i3)-sx(i1-4,i2,i3)))*d18(0)
        sxs8(i1,i2,i3)=(672.*(sx(i1,i2+1,i3)-sx(i1,i2-1,i3))-168.*(sx(
     & i1,i2+2,i3)-sx(i1,i2-2,i3))+32.*(sx(i1,i2+3,i3)-sx(i1,i2-3,i3))
     & -3.*(sx(i1,i2+4,i3)-sx(i1,i2-4,i3)))*d18(1)
        sxt8(i1,i2,i3)=(672.*(sx(i1,i2,i3+1)-sx(i1,i2,i3-1))-168.*(sx(
     & i1,i2,i3+2)-sx(i1,i2,i3-2))+32.*(sx(i1,i2,i3+3)-sx(i1,i2,i3-3))
     & -3.*(sx(i1,i2,i3+4)-sx(i1,i2,i3-4)))*d18(2)
        syr8(i1,i2,i3)=(672.*(sy(i1+1,i2,i3)-sy(i1-1,i2,i3))-168.*(sy(
     & i1+2,i2,i3)-sy(i1-2,i2,i3))+32.*(sy(i1+3,i2,i3)-sy(i1-3,i2,i3))
     & -3.*(sy(i1+4,i2,i3)-sy(i1-4,i2,i3)))*d18(0)
        sys8(i1,i2,i3)=(672.*(sy(i1,i2+1,i3)-sy(i1,i2-1,i3))-168.*(sy(
     & i1,i2+2,i3)-sy(i1,i2-2,i3))+32.*(sy(i1,i2+3,i3)-sy(i1,i2-3,i3))
     & -3.*(sy(i1,i2+4,i3)-sy(i1,i2-4,i3)))*d18(1)
        syt8(i1,i2,i3)=(672.*(sy(i1,i2,i3+1)-sy(i1,i2,i3-1))-168.*(sy(
     & i1,i2,i3+2)-sy(i1,i2,i3-2))+32.*(sy(i1,i2,i3+3)-sy(i1,i2,i3-3))
     & -3.*(sy(i1,i2,i3+4)-sy(i1,i2,i3-4)))*d18(2)
        szr8(i1,i2,i3)=(672.*(sz(i1+1,i2,i3)-sz(i1-1,i2,i3))-168.*(sz(
     & i1+2,i2,i3)-sz(i1-2,i2,i3))+32.*(sz(i1+3,i2,i3)-sz(i1-3,i2,i3))
     & -3.*(sz(i1+4,i2,i3)-sz(i1-4,i2,i3)))*d18(0)
        szs8(i1,i2,i3)=(672.*(sz(i1,i2+1,i3)-sz(i1,i2-1,i3))-168.*(sz(
     & i1,i2+2,i3)-sz(i1,i2-2,i3))+32.*(sz(i1,i2+3,i3)-sz(i1,i2-3,i3))
     & -3.*(sz(i1,i2+4,i3)-sz(i1,i2-4,i3)))*d18(1)
        szt8(i1,i2,i3)=(672.*(sz(i1,i2,i3+1)-sz(i1,i2,i3-1))-168.*(sz(
     & i1,i2,i3+2)-sz(i1,i2,i3-2))+32.*(sz(i1,i2,i3+3)-sz(i1,i2,i3-3))
     & -3.*(sz(i1,i2,i3+4)-sz(i1,i2,i3-4)))*d18(2)
        txr8(i1,i2,i3)=(672.*(tx(i1+1,i2,i3)-tx(i1-1,i2,i3))-168.*(tx(
     & i1+2,i2,i3)-tx(i1-2,i2,i3))+32.*(tx(i1+3,i2,i3)-tx(i1-3,i2,i3))
     & -3.*(tx(i1+4,i2,i3)-tx(i1-4,i2,i3)))*d18(0)
        txs8(i1,i2,i3)=(672.*(tx(i1,i2+1,i3)-tx(i1,i2-1,i3))-168.*(tx(
     & i1,i2+2,i3)-tx(i1,i2-2,i3))+32.*(tx(i1,i2+3,i3)-tx(i1,i2-3,i3))
     & -3.*(tx(i1,i2+4,i3)-tx(i1,i2-4,i3)))*d18(1)
        txt8(i1,i2,i3)=(672.*(tx(i1,i2,i3+1)-tx(i1,i2,i3-1))-168.*(tx(
     & i1,i2,i3+2)-tx(i1,i2,i3-2))+32.*(tx(i1,i2,i3+3)-tx(i1,i2,i3-3))
     & -3.*(tx(i1,i2,i3+4)-tx(i1,i2,i3-4)))*d18(2)
        tyr8(i1,i2,i3)=(672.*(ty(i1+1,i2,i3)-ty(i1-1,i2,i3))-168.*(ty(
     & i1+2,i2,i3)-ty(i1-2,i2,i3))+32.*(ty(i1+3,i2,i3)-ty(i1-3,i2,i3))
     & -3.*(ty(i1+4,i2,i3)-ty(i1-4,i2,i3)))*d18(0)
        tys8(i1,i2,i3)=(672.*(ty(i1,i2+1,i3)-ty(i1,i2-1,i3))-168.*(ty(
     & i1,i2+2,i3)-ty(i1,i2-2,i3))+32.*(ty(i1,i2+3,i3)-ty(i1,i2-3,i3))
     & -3.*(ty(i1,i2+4,i3)-ty(i1,i2-4,i3)))*d18(1)
        tyt8(i1,i2,i3)=(672.*(ty(i1,i2,i3+1)-ty(i1,i2,i3-1))-168.*(ty(
     & i1,i2,i3+2)-ty(i1,i2,i3-2))+32.*(ty(i1,i2,i3+3)-ty(i1,i2,i3-3))
     & -3.*(ty(i1,i2,i3+4)-ty(i1,i2,i3-4)))*d18(2)
        tzr8(i1,i2,i3)=(672.*(tz(i1+1,i2,i3)-tz(i1-1,i2,i3))-168.*(tz(
     & i1+2,i2,i3)-tz(i1-2,i2,i3))+32.*(tz(i1+3,i2,i3)-tz(i1-3,i2,i3))
     & -3.*(tz(i1+4,i2,i3)-tz(i1-4,i2,i3)))*d18(0)
        tzs8(i1,i2,i3)=(672.*(tz(i1,i2+1,i3)-tz(i1,i2-1,i3))-168.*(tz(
     & i1,i2+2,i3)-tz(i1,i2-2,i3))+32.*(tz(i1,i2+3,i3)-tz(i1,i2-3,i3))
     & -3.*(tz(i1,i2+4,i3)-tz(i1,i2-4,i3)))*d18(1)
        tzt8(i1,i2,i3)=(672.*(tz(i1,i2,i3+1)-tz(i1,i2,i3-1))-168.*(tz(
     & i1,i2,i3+2)-tz(i1,i2,i3-2))+32.*(tz(i1,i2,i3+3)-tz(i1,i2,i3-3))
     & -3.*(tz(i1,i2,i3+4)-tz(i1,i2,i3-4)))*d18(2)
        ux81(i1,i2,i3,kd)= rx(i1,i2,i3)*ur8(i1,i2,i3,kd)
        uy81(i1,i2,i3,kd)=0
        uz81(i1,i2,i3,kd)=0
        ux82(i1,i2,i3,kd)= rx(i1,i2,i3)*ur8(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & us8(i1,i2,i3,kd)
        uy82(i1,i2,i3,kd)= ry(i1,i2,i3)*ur8(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & us8(i1,i2,i3,kd)
        uz82(i1,i2,i3,kd)=0
        ux83(i1,i2,i3,kd)=rx(i1,i2,i3)*ur8(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & us8(i1,i2,i3,kd)+tx(i1,i2,i3)*ut8(i1,i2,i3,kd)
        uy83(i1,i2,i3,kd)=ry(i1,i2,i3)*ur8(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & us8(i1,i2,i3,kd)+ty(i1,i2,i3)*ut8(i1,i2,i3,kd)
        uz83(i1,i2,i3,kd)=rz(i1,i2,i3)*ur8(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & us8(i1,i2,i3,kd)+tz(i1,i2,i3)*ut8(i1,i2,i3,kd)
! #If "RX" == "RX"
        rxx81(i1,i2,i3)= rx(i1,i2,i3)*rxr8(i1,i2,i3)
        rxx82(i1,i2,i3)= rx(i1,i2,i3)*rxr8(i1,i2,i3)+sx(i1,i2,i3)*rxs8(
     & i1,i2,i3)
        rxy82(i1,i2,i3)= ry(i1,i2,i3)*rxr8(i1,i2,i3)+sy(i1,i2,i3)*rxs8(
     & i1,i2,i3)
        rxx83(i1,i2,i3)=rx(i1,i2,i3)*rxr8(i1,i2,i3)+sx(i1,i2,i3)*rxs8(
     & i1,i2,i3)+tx(i1,i2,i3)*rxt8(i1,i2,i3)
        rxy83(i1,i2,i3)=ry(i1,i2,i3)*rxr8(i1,i2,i3)+sy(i1,i2,i3)*rxs8(
     & i1,i2,i3)+ty(i1,i2,i3)*rxt8(i1,i2,i3)
        rxz83(i1,i2,i3)=rz(i1,i2,i3)*rxr8(i1,i2,i3)+sz(i1,i2,i3)*rxs8(
     & i1,i2,i3)+tz(i1,i2,i3)*rxt8(i1,i2,i3)
        ryx82(i1,i2,i3)= rx(i1,i2,i3)*ryr8(i1,i2,i3)+sx(i1,i2,i3)*rys8(
     & i1,i2,i3)
        ryy82(i1,i2,i3)= ry(i1,i2,i3)*ryr8(i1,i2,i3)+sy(i1,i2,i3)*rys8(
     & i1,i2,i3)
        ryx83(i1,i2,i3)=rx(i1,i2,i3)*ryr8(i1,i2,i3)+sx(i1,i2,i3)*rys8(
     & i1,i2,i3)+tx(i1,i2,i3)*ryt8(i1,i2,i3)
        ryy83(i1,i2,i3)=ry(i1,i2,i3)*ryr8(i1,i2,i3)+sy(i1,i2,i3)*rys8(
     & i1,i2,i3)+ty(i1,i2,i3)*ryt8(i1,i2,i3)
        ryz83(i1,i2,i3)=rz(i1,i2,i3)*ryr8(i1,i2,i3)+sz(i1,i2,i3)*rys8(
     & i1,i2,i3)+tz(i1,i2,i3)*ryt8(i1,i2,i3)
        rzx82(i1,i2,i3)= rx(i1,i2,i3)*rzr8(i1,i2,i3)+sx(i1,i2,i3)*rzs8(
     & i1,i2,i3)
        rzy82(i1,i2,i3)= ry(i1,i2,i3)*rzr8(i1,i2,i3)+sy(i1,i2,i3)*rzs8(
     & i1,i2,i3)
        rzx83(i1,i2,i3)=rx(i1,i2,i3)*rzr8(i1,i2,i3)+sx(i1,i2,i3)*rzs8(
     & i1,i2,i3)+tx(i1,i2,i3)*rzt8(i1,i2,i3)
        rzy83(i1,i2,i3)=ry(i1,i2,i3)*rzr8(i1,i2,i3)+sy(i1,i2,i3)*rzs8(
     & i1,i2,i3)+ty(i1,i2,i3)*rzt8(i1,i2,i3)
        rzz83(i1,i2,i3)=rz(i1,i2,i3)*rzr8(i1,i2,i3)+sz(i1,i2,i3)*rzs8(
     & i1,i2,i3)+tz(i1,i2,i3)*rzt8(i1,i2,i3)
        sxx82(i1,i2,i3)= rx(i1,i2,i3)*sxr8(i1,i2,i3)+sx(i1,i2,i3)*sxs8(
     & i1,i2,i3)
        sxy82(i1,i2,i3)= ry(i1,i2,i3)*sxr8(i1,i2,i3)+sy(i1,i2,i3)*sxs8(
     & i1,i2,i3)
        sxx83(i1,i2,i3)=rx(i1,i2,i3)*sxr8(i1,i2,i3)+sx(i1,i2,i3)*sxs8(
     & i1,i2,i3)+tx(i1,i2,i3)*sxt8(i1,i2,i3)
        sxy83(i1,i2,i3)=ry(i1,i2,i3)*sxr8(i1,i2,i3)+sy(i1,i2,i3)*sxs8(
     & i1,i2,i3)+ty(i1,i2,i3)*sxt8(i1,i2,i3)
        sxz83(i1,i2,i3)=rz(i1,i2,i3)*sxr8(i1,i2,i3)+sz(i1,i2,i3)*sxs8(
     & i1,i2,i3)+tz(i1,i2,i3)*sxt8(i1,i2,i3)
        syx82(i1,i2,i3)= rx(i1,i2,i3)*syr8(i1,i2,i3)+sx(i1,i2,i3)*sys8(
     & i1,i2,i3)
        syy82(i1,i2,i3)= ry(i1,i2,i3)*syr8(i1,i2,i3)+sy(i1,i2,i3)*sys8(
     & i1,i2,i3)
        syx83(i1,i2,i3)=rx(i1,i2,i3)*syr8(i1,i2,i3)+sx(i1,i2,i3)*sys8(
     & i1,i2,i3)+tx(i1,i2,i3)*syt8(i1,i2,i3)
        syy83(i1,i2,i3)=ry(i1,i2,i3)*syr8(i1,i2,i3)+sy(i1,i2,i3)*sys8(
     & i1,i2,i3)+ty(i1,i2,i3)*syt8(i1,i2,i3)
        syz83(i1,i2,i3)=rz(i1,i2,i3)*syr8(i1,i2,i3)+sz(i1,i2,i3)*sys8(
     & i1,i2,i3)+tz(i1,i2,i3)*syt8(i1,i2,i3)
        szx82(i1,i2,i3)= rx(i1,i2,i3)*szr8(i1,i2,i3)+sx(i1,i2,i3)*szs8(
     & i1,i2,i3)
        szy82(i1,i2,i3)= ry(i1,i2,i3)*szr8(i1,i2,i3)+sy(i1,i2,i3)*szs8(
     & i1,i2,i3)
        szx83(i1,i2,i3)=rx(i1,i2,i3)*szr8(i1,i2,i3)+sx(i1,i2,i3)*szs8(
     & i1,i2,i3)+tx(i1,i2,i3)*szt8(i1,i2,i3)
        szy83(i1,i2,i3)=ry(i1,i2,i3)*szr8(i1,i2,i3)+sy(i1,i2,i3)*szs8(
     & i1,i2,i3)+ty(i1,i2,i3)*szt8(i1,i2,i3)
        szz83(i1,i2,i3)=rz(i1,i2,i3)*szr8(i1,i2,i3)+sz(i1,i2,i3)*szs8(
     & i1,i2,i3)+tz(i1,i2,i3)*szt8(i1,i2,i3)
        txx82(i1,i2,i3)= rx(i1,i2,i3)*txr8(i1,i2,i3)+sx(i1,i2,i3)*txs8(
     & i1,i2,i3)
        txy82(i1,i2,i3)= ry(i1,i2,i3)*txr8(i1,i2,i3)+sy(i1,i2,i3)*txs8(
     & i1,i2,i3)
        txx83(i1,i2,i3)=rx(i1,i2,i3)*txr8(i1,i2,i3)+sx(i1,i2,i3)*txs8(
     & i1,i2,i3)+tx(i1,i2,i3)*txt8(i1,i2,i3)
        txy83(i1,i2,i3)=ry(i1,i2,i3)*txr8(i1,i2,i3)+sy(i1,i2,i3)*txs8(
     & i1,i2,i3)+ty(i1,i2,i3)*txt8(i1,i2,i3)
        txz83(i1,i2,i3)=rz(i1,i2,i3)*txr8(i1,i2,i3)+sz(i1,i2,i3)*txs8(
     & i1,i2,i3)+tz(i1,i2,i3)*txt8(i1,i2,i3)
        tyx82(i1,i2,i3)= rx(i1,i2,i3)*tyr8(i1,i2,i3)+sx(i1,i2,i3)*tys8(
     & i1,i2,i3)
        tyy82(i1,i2,i3)= ry(i1,i2,i3)*tyr8(i1,i2,i3)+sy(i1,i2,i3)*tys8(
     & i1,i2,i3)
        tyx83(i1,i2,i3)=rx(i1,i2,i3)*tyr8(i1,i2,i3)+sx(i1,i2,i3)*tys8(
     & i1,i2,i3)+tx(i1,i2,i3)*tyt8(i1,i2,i3)
        tyy83(i1,i2,i3)=ry(i1,i2,i3)*tyr8(i1,i2,i3)+sy(i1,i2,i3)*tys8(
     & i1,i2,i3)+ty(i1,i2,i3)*tyt8(i1,i2,i3)
        tyz83(i1,i2,i3)=rz(i1,i2,i3)*tyr8(i1,i2,i3)+sz(i1,i2,i3)*tys8(
     & i1,i2,i3)+tz(i1,i2,i3)*tyt8(i1,i2,i3)
        tzx82(i1,i2,i3)= rx(i1,i2,i3)*tzr8(i1,i2,i3)+sx(i1,i2,i3)*tzs8(
     & i1,i2,i3)
        tzy82(i1,i2,i3)= ry(i1,i2,i3)*tzr8(i1,i2,i3)+sy(i1,i2,i3)*tzs8(
     & i1,i2,i3)
        tzx83(i1,i2,i3)=rx(i1,i2,i3)*tzr8(i1,i2,i3)+sx(i1,i2,i3)*tzs8(
     & i1,i2,i3)+tx(i1,i2,i3)*tzt8(i1,i2,i3)
        tzy83(i1,i2,i3)=ry(i1,i2,i3)*tzr8(i1,i2,i3)+sy(i1,i2,i3)*tzs8(
     & i1,i2,i3)+ty(i1,i2,i3)*tzt8(i1,i2,i3)
        tzz83(i1,i2,i3)=rz(i1,i2,i3)*tzr8(i1,i2,i3)+sz(i1,i2,i3)*tzs8(
     & i1,i2,i3)+tz(i1,i2,i3)*tzt8(i1,i2,i3)
        uxx81(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*urr8(i1,i2,i3,kd)+(rxx82(
     & i1,i2,i3))*ur8(i1,i2,i3,kd)
        uyy81(i1,i2,i3,kd)=0
        uxy81(i1,i2,i3,kd)=0
        uxz81(i1,i2,i3,kd)=0
        uyz81(i1,i2,i3,kd)=0
        uzz81(i1,i2,i3,kd)=0
        ulaplacian81(i1,i2,i3,kd)=uxx81(i1,i2,i3,kd)
        uxx82(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*urr8(i1,i2,i3,kd)+2.*(rx(
     & i1,i2,i3)*sx(i1,i2,i3))*urs8(i1,i2,i3,kd)+(sx(i1,i2,i3)**2)*
     & uss8(i1,i2,i3,kd)+(rxx82(i1,i2,i3))*ur8(i1,i2,i3,kd)+(sxx82(i1,
     & i2,i3))*us8(i1,i2,i3,kd)
        uyy82(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*urr8(i1,i2,i3,kd)+2.*(ry(
     & i1,i2,i3)*sy(i1,i2,i3))*urs8(i1,i2,i3,kd)+(sy(i1,i2,i3)**2)*
     & uss8(i1,i2,i3,kd)+(ryy82(i1,i2,i3))*ur8(i1,i2,i3,kd)+(syy82(i1,
     & i2,i3))*us8(i1,i2,i3,kd)
        uxy82(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*urr8(i1,i2,i3,kd)+
     & (rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*urs8(i1,
     & i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*uss8(i1,i2,i3,kd)+rxy82(i1,
     & i2,i3)*ur8(i1,i2,i3,kd)+sxy82(i1,i2,i3)*us8(i1,i2,i3,kd)
        uxz82(i1,i2,i3,kd)=0
        uyz82(i1,i2,i3,kd)=0
        uzz82(i1,i2,i3,kd)=0
        ulaplacian82(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & urr8(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*
     & sy(i1,i2,i3))*urs8(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)**
     & 2)*uss8(i1,i2,i3,kd)+(rxx82(i1,i2,i3)+ryy82(i1,i2,i3))*ur8(i1,
     & i2,i3,kd)+(sxx82(i1,i2,i3)+syy82(i1,i2,i3))*us8(i1,i2,i3,kd)
        uxx83(i1,i2,i3,kd)=rx(i1,i2,i3)**2*urr8(i1,i2,i3,kd)+sx(i1,i2,
     & i3)**2*uss8(i1,i2,i3,kd)+tx(i1,i2,i3)**2*utt8(i1,i2,i3,kd)+2.*
     & rx(i1,i2,i3)*sx(i1,i2,i3)*urs8(i1,i2,i3,kd)+2.*rx(i1,i2,i3)*tx(
     & i1,i2,i3)*urt8(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,i2,i3)*ust8(
     & i1,i2,i3,kd)+rxx83(i1,i2,i3)*ur8(i1,i2,i3,kd)+sxx83(i1,i2,i3)*
     & us8(i1,i2,i3,kd)+txx83(i1,i2,i3)*ut8(i1,i2,i3,kd)
        uyy83(i1,i2,i3,kd)=ry(i1,i2,i3)**2*urr8(i1,i2,i3,kd)+sy(i1,i2,
     & i3)**2*uss8(i1,i2,i3,kd)+ty(i1,i2,i3)**2*utt8(i1,i2,i3,kd)+2.*
     & ry(i1,i2,i3)*sy(i1,i2,i3)*urs8(i1,i2,i3,kd)+2.*ry(i1,i2,i3)*ty(
     & i1,i2,i3)*urt8(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,i2,i3)*ust8(
     & i1,i2,i3,kd)+ryy83(i1,i2,i3)*ur8(i1,i2,i3,kd)+syy83(i1,i2,i3)*
     & us8(i1,i2,i3,kd)+tyy83(i1,i2,i3)*ut8(i1,i2,i3,kd)
        uzz83(i1,i2,i3,kd)=rz(i1,i2,i3)**2*urr8(i1,i2,i3,kd)+sz(i1,i2,
     & i3)**2*uss8(i1,i2,i3,kd)+tz(i1,i2,i3)**2*utt8(i1,i2,i3,kd)+2.*
     & rz(i1,i2,i3)*sz(i1,i2,i3)*urs8(i1,i2,i3,kd)+2.*rz(i1,i2,i3)*tz(
     & i1,i2,i3)*urt8(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,i2,i3)*ust8(
     & i1,i2,i3,kd)+rzz83(i1,i2,i3)*ur8(i1,i2,i3,kd)+szz83(i1,i2,i3)*
     & us8(i1,i2,i3,kd)+tzz83(i1,i2,i3)*ut8(i1,i2,i3,kd)
        uxy83(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*urr8(i1,i2,i3,kd)+
     & sx(i1,i2,i3)*sy(i1,i2,i3)*uss8(i1,i2,i3,kd)+tx(i1,i2,i3)*ty(i1,
     & i2,i3)*utt8(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,
     & i3)*sx(i1,i2,i3))*urs8(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(i1,i2,i3)+
     & ry(i1,i2,i3)*tx(i1,i2,i3))*urt8(i1,i2,i3,kd)+(sx(i1,i2,i3)*ty(
     & i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*ust8(i1,i2,i3,kd)+rxy83(
     & i1,i2,i3)*ur8(i1,i2,i3,kd)+sxy83(i1,i2,i3)*us8(i1,i2,i3,kd)+
     & txy83(i1,i2,i3)*ut8(i1,i2,i3,kd)
        uxz83(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*urr8(i1,i2,i3,kd)+
     & sx(i1,i2,i3)*sz(i1,i2,i3)*uss8(i1,i2,i3,kd)+tx(i1,i2,i3)*tz(i1,
     & i2,i3)*utt8(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,i2,
     & i3)*sx(i1,i2,i3))*urs8(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(i1,i2,i3)+
     & rz(i1,i2,i3)*tx(i1,i2,i3))*urt8(i1,i2,i3,kd)+(sx(i1,i2,i3)*tz(
     & i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*ust8(i1,i2,i3,kd)+rxz83(
     & i1,i2,i3)*ur8(i1,i2,i3,kd)+sxz83(i1,i2,i3)*us8(i1,i2,i3,kd)+
     & txz83(i1,i2,i3)*ut8(i1,i2,i3,kd)
        uyz83(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*urr8(i1,i2,i3,kd)+
     & sy(i1,i2,i3)*sz(i1,i2,i3)*uss8(i1,i2,i3,kd)+ty(i1,i2,i3)*tz(i1,
     & i2,i3)*utt8(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,i2,
     & i3)*sy(i1,i2,i3))*urs8(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(i1,i2,i3)+
     & rz(i1,i2,i3)*ty(i1,i2,i3))*urt8(i1,i2,i3,kd)+(sy(i1,i2,i3)*tz(
     & i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*ust8(i1,i2,i3,kd)+ryz83(
     & i1,i2,i3)*ur8(i1,i2,i3,kd)+syz83(i1,i2,i3)*us8(i1,i2,i3,kd)+
     & tyz83(i1,i2,i3)*ut8(i1,i2,i3,kd)
        ulaplacian83(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*urr8(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)**
     & 2+sz(i1,i2,i3)**2)*uss8(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(i1,i2,
     & i3)**2+tz(i1,i2,i3)**2)*utt8(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(
     & i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,i3))
     & *urs8(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(i1,i2,i3)*
     & ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*urt8(i1,i2,i3,kd)+2.*(
     & sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)+sz(i1,i2,
     & i3)*tz(i1,i2,i3))*ust8(i1,i2,i3,kd)+(rxx83(i1,i2,i3)+ryy83(i1,
     & i2,i3)+rzz83(i1,i2,i3))*ur8(i1,i2,i3,kd)+(sxx83(i1,i2,i3)+
     & syy83(i1,i2,i3)+szz83(i1,i2,i3))*us8(i1,i2,i3,kd)+(txx83(i1,i2,
     & i3)+tyy83(i1,i2,i3)+tzz83(i1,i2,i3))*ut8(i1,i2,i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
! #If "RX" == "RX"
        h18(kd) = 1./(840.*dx(kd))
        h28(kd) = 1./(5040.*dx(kd)**2)
        ux83r(i1,i2,i3,kd)=(672.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))-
     & 168.*(u(i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd))+32.*(u(i1+3,i2,i3,kd)-
     & u(i1-3,i2,i3,kd))-3.*(u(i1+4,i2,i3,kd)-u(i1-4,i2,i3,kd)))*h18(
     & 0)
        uy83r(i1,i2,i3,kd)=(672.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))-
     & 168.*(u(i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd))+32.*(u(i1,i2+3,i3,kd)-
     & u(i1,i2-3,i3,kd))-3.*(u(i1,i2+4,i3,kd)-u(i1,i2-4,i3,kd)))*h18(
     & 1)
        uz83r(i1,i2,i3,kd)=(672.*(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))-
     & 168.*(u(i1,i2,i3+2,kd)-u(i1,i2,i3-2,kd))+32.*(u(i1,i2,i3+3,kd)-
     & u(i1,i2,i3-3,kd))-3.*(u(i1,i2,i3+4,kd)-u(i1,i2,i3-4,kd)))*h18(
     & 2)
        uxx83r(i1,i2,i3,kd)=(-14350.*u(i1,i2,i3,kd)+8064.*(u(i1+1,i2,
     & i3,kd)+u(i1-1,i2,i3,kd))-1008.*(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,
     & kd))+128.*(u(i1+3,i2,i3,kd)+u(i1-3,i2,i3,kd))-9.*(u(i1+4,i2,i3,
     & kd)+u(i1-4,i2,i3,kd)) )*h28(0)
        uyy83r(i1,i2,i3,kd)=(-14350.*u(i1,i2,i3,kd)+8064.*(u(i1,i2+1,
     & i3,kd)+u(i1,i2-1,i3,kd))-1008.*(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,
     & kd))+128.*(u(i1,i2+3,i3,kd)+u(i1,i2-3,i3,kd))-9.*(u(i1,i2+4,i3,
     & kd)+u(i1,i2-4,i3,kd)) )*h28(1)
        uzz83r(i1,i2,i3,kd)=(-14350.*u(i1,i2,i3,kd)+8064.*(u(i1,i2,i3+
     & 1,kd)+u(i1,i2,i3-1,kd))-1008.*(u(i1,i2,i3+2,kd)+u(i1,i2,i3-2,
     & kd))+128.*(u(i1,i2,i3+3,kd)+u(i1,i2,i3-3,kd))-9.*(u(i1,i2,i3+4,
     & kd)+u(i1,i2,i3-4,kd)) )*h28(2)
        uxy83r(i1,i2,i3,kd)=(672.*(ux83r(i1,i2+1,i3,kd)-ux83r(i1,i2-1,
     & i3,kd))-168.*(ux83r(i1,i2+2,i3,kd)-ux83r(i1,i2-2,i3,kd))+32.*(
     & ux83r(i1,i2+3,i3,kd)-ux83r(i1,i2-3,i3,kd))-3.*(ux83r(i1,i2+4,
     & i3,kd)-ux83r(i1,i2-4,i3,kd)))*h18(1)
        uxz83r(i1,i2,i3,kd)=(672.*(ux83r(i1,i2,i3+1,kd)-ux83r(i1,i2,i3-
     & 1,kd))-168.*(ux83r(i1,i2,i3+2,kd)-ux83r(i1,i2,i3-2,kd))+32.*(
     & ux83r(i1,i2,i3+3,kd)-ux83r(i1,i2,i3-3,kd))-3.*(ux83r(i1,i2,i3+
     & 4,kd)-ux83r(i1,i2,i3-4,kd)))*h18(2)
        uyz83r(i1,i2,i3,kd)=(672.*(uy83r(i1,i2,i3+1,kd)-uy83r(i1,i2,i3-
     & 1,kd))-168.*(uy83r(i1,i2,i3+2,kd)-uy83r(i1,i2,i3-2,kd))+32.*(
     & uy83r(i1,i2,i3+3,kd)-uy83r(i1,i2,i3-3,kd))-3.*(uy83r(i1,i2,i3+
     & 4,kd)-uy83r(i1,i2,i3-4,kd)))*h18(2)
        ux81r(i1,i2,i3,kd)= ux83r(i1,i2,i3,kd)
        uy81r(i1,i2,i3,kd)= uy83r(i1,i2,i3,kd)
        uz81r(i1,i2,i3,kd)= uz83r(i1,i2,i3,kd)
        uxx81r(i1,i2,i3,kd)= uxx83r(i1,i2,i3,kd)
        uyy81r(i1,i2,i3,kd)= uyy83r(i1,i2,i3,kd)
        uzz81r(i1,i2,i3,kd)= uzz83r(i1,i2,i3,kd)
        uxy81r(i1,i2,i3,kd)= uxy83r(i1,i2,i3,kd)
        uxz81r(i1,i2,i3,kd)= uxz83r(i1,i2,i3,kd)
        uyz81r(i1,i2,i3,kd)= uyz83r(i1,i2,i3,kd)
        ulaplacian81r(i1,i2,i3,kd)=uxx83r(i1,i2,i3,kd)
        ux82r(i1,i2,i3,kd)= ux83r(i1,i2,i3,kd)
        uy82r(i1,i2,i3,kd)= uy83r(i1,i2,i3,kd)
        uz82r(i1,i2,i3,kd)= uz83r(i1,i2,i3,kd)
        uxx82r(i1,i2,i3,kd)= uxx83r(i1,i2,i3,kd)
        uyy82r(i1,i2,i3,kd)= uyy83r(i1,i2,i3,kd)
        uzz82r(i1,i2,i3,kd)= uzz83r(i1,i2,i3,kd)
        uxy82r(i1,i2,i3,kd)= uxy83r(i1,i2,i3,kd)
        uxz82r(i1,i2,i3,kd)= uxz83r(i1,i2,i3,kd)
        uyz82r(i1,i2,i3,kd)= uyz83r(i1,i2,i3,kd)
        ulaplacian82r(i1,i2,i3,kd)=uxx83r(i1,i2,i3,kd)+uyy83r(i1,i2,i3,
     & kd)
        ulaplacian83r(i1,i2,i3,kd)=uxx83r(i1,i2,i3,kd)+uyy83r(i1,i2,i3,
     & kd)+uzz83r(i1,i2,i3,kd)
c we also need derivatives of the scalar "sc"
! defineDifferenceOrder2Components1(sc,)
! #If "" == "RX"
        scr2(i1,i2,i3,kd)=(sc(i1+1,i2,i3,kd)-sc(i1-1,i2,i3,kd))*d12(0)
        scs2(i1,i2,i3,kd)=(sc(i1,i2+1,i3,kd)-sc(i1,i2-1,i3,kd))*d12(1)
        sct2(i1,i2,i3,kd)=(sc(i1,i2,i3+1,kd)-sc(i1,i2,i3-1,kd))*d12(2)
        scrr2(i1,i2,i3,kd)=(-2.*sc(i1,i2,i3,kd)+(sc(i1+1,i2,i3,kd)+sc(
     & i1-1,i2,i3,kd)) )*d22(0)
        scss2(i1,i2,i3,kd)=(-2.*sc(i1,i2,i3,kd)+(sc(i1,i2+1,i3,kd)+sc(
     & i1,i2-1,i3,kd)) )*d22(1)
        scrs2(i1,i2,i3,kd)=(scr2(i1,i2+1,i3,kd)-scr2(i1,i2-1,i3,kd))*
     & d12(1)
        sctt2(i1,i2,i3,kd)=(-2.*sc(i1,i2,i3,kd)+(sc(i1,i2,i3+1,kd)+sc(
     & i1,i2,i3-1,kd)) )*d22(2)
        scrt2(i1,i2,i3,kd)=(scr2(i1,i2,i3+1,kd)-scr2(i1,i2,i3-1,kd))*
     & d12(2)
        scst2(i1,i2,i3,kd)=(scs2(i1,i2,i3+1,kd)-scs2(i1,i2,i3-1,kd))*
     & d12(2)
        scrrr2(i1,i2,i3,kd)=(-2.*(sc(i1+1,i2,i3,kd)-sc(i1-1,i2,i3,kd))+
     & (sc(i1+2,i2,i3,kd)-sc(i1-2,i2,i3,kd)) )*d22(0)*d12(0)
        scsss2(i1,i2,i3,kd)=(-2.*(sc(i1,i2+1,i3,kd)-sc(i1,i2-1,i3,kd))+
     & (sc(i1,i2+2,i3,kd)-sc(i1,i2-2,i3,kd)) )*d22(1)*d12(1)
        scttt2(i1,i2,i3,kd)=(-2.*(sc(i1,i2,i3+1,kd)-sc(i1,i2,i3-1,kd))+
     & (sc(i1,i2,i3+2,kd)-sc(i1,i2,i3-2,kd)) )*d22(2)*d12(2)
! #If "" == "RX"
        scx21(i1,i2,i3,kd)= rx(i1,i2,i3)*scr2(i1,i2,i3,kd)
        scy21(i1,i2,i3,kd)=0
        scz21(i1,i2,i3,kd)=0
        scx22(i1,i2,i3,kd)= rx(i1,i2,i3)*scr2(i1,i2,i3,kd)+sx(i1,i2,i3)
     & *scs2(i1,i2,i3,kd)
        scy22(i1,i2,i3,kd)= ry(i1,i2,i3)*scr2(i1,i2,i3,kd)+sy(i1,i2,i3)
     & *scs2(i1,i2,i3,kd)
        scz22(i1,i2,i3,kd)=0
        scx23(i1,i2,i3,kd)=rx(i1,i2,i3)*scr2(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & scs2(i1,i2,i3,kd)+tx(i1,i2,i3)*sct2(i1,i2,i3,kd)
        scy23(i1,i2,i3,kd)=ry(i1,i2,i3)*scr2(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & scs2(i1,i2,i3,kd)+ty(i1,i2,i3)*sct2(i1,i2,i3,kd)
        scz23(i1,i2,i3,kd)=rz(i1,i2,i3)*scr2(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & scs2(i1,i2,i3,kd)+tz(i1,i2,i3)*sct2(i1,i2,i3,kd)
! #If "" == "RX"
        scxx21(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*scrr2(i1,i2,i3,kd)+(
     & rxx22(i1,i2,i3))*scr2(i1,i2,i3,kd)
        scyy21(i1,i2,i3,kd)=0
        scxy21(i1,i2,i3,kd)=0
        scxz21(i1,i2,i3,kd)=0
        scyz21(i1,i2,i3,kd)=0
        sczz21(i1,i2,i3,kd)=0
        sclaplacian21(i1,i2,i3,kd)=scxx21(i1,i2,i3,kd)
        scxx22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*scrr2(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*scrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2)
     & *scss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3))*scr2(i1,i2,i3,kd)+(sxx22(
     & i1,i2,i3))*scs2(i1,i2,i3,kd)
        scyy22(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*scrr2(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*scrs2(i1,i2,i3,kd)+(sy(i1,i2,i3)**2)
     & *scss2(i1,i2,i3,kd)+(ryy22(i1,i2,i3))*scr2(i1,i2,i3,kd)+(syy22(
     & i1,i2,i3))*scs2(i1,i2,i3,kd)
        scxy22(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*scrr2(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & scrs2(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*scss2(i1,i2,i3,kd)
     & +rxy22(i1,i2,i3)*scr2(i1,i2,i3,kd)+sxy22(i1,i2,i3)*scs2(i1,i2,
     & i3,kd)
        scxz22(i1,i2,i3,kd)=0
        scyz22(i1,i2,i3,kd)=0
        sczz22(i1,i2,i3,kd)=0
        sclaplacian22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & scrr2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*
     & sy(i1,i2,i3))*scrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)*
     & *2)*scss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3)+ryy22(i1,i2,i3))*scr2(
     & i1,i2,i3,kd)+(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*scs2(i1,i2,i3,
     & kd)
        scxx23(i1,i2,i3,kd)=rx(i1,i2,i3)**2*scrr2(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*scss2(i1,i2,i3,kd)+tx(i1,i2,i3)**2*sctt2(i1,i2,i3,kd)
     & +2.*rx(i1,i2,i3)*sx(i1,i2,i3)*scrs2(i1,i2,i3,kd)+2.*rx(i1,i2,
     & i3)*tx(i1,i2,i3)*scrt2(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,i2,
     & i3)*scst2(i1,i2,i3,kd)+rxx23(i1,i2,i3)*scr2(i1,i2,i3,kd)+sxx23(
     & i1,i2,i3)*scs2(i1,i2,i3,kd)+txx23(i1,i2,i3)*sct2(i1,i2,i3,kd)
        scyy23(i1,i2,i3,kd)=ry(i1,i2,i3)**2*scrr2(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*scss2(i1,i2,i3,kd)+ty(i1,i2,i3)**2*sctt2(i1,i2,i3,kd)
     & +2.*ry(i1,i2,i3)*sy(i1,i2,i3)*scrs2(i1,i2,i3,kd)+2.*ry(i1,i2,
     & i3)*ty(i1,i2,i3)*scrt2(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,i2,
     & i3)*scst2(i1,i2,i3,kd)+ryy23(i1,i2,i3)*scr2(i1,i2,i3,kd)+syy23(
     & i1,i2,i3)*scs2(i1,i2,i3,kd)+tyy23(i1,i2,i3)*sct2(i1,i2,i3,kd)
        sczz23(i1,i2,i3,kd)=rz(i1,i2,i3)**2*scrr2(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*scss2(i1,i2,i3,kd)+tz(i1,i2,i3)**2*sctt2(i1,i2,i3,kd)
     & +2.*rz(i1,i2,i3)*sz(i1,i2,i3)*scrs2(i1,i2,i3,kd)+2.*rz(i1,i2,
     & i3)*tz(i1,i2,i3)*scrt2(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,i2,
     & i3)*scst2(i1,i2,i3,kd)+rzz23(i1,i2,i3)*scr2(i1,i2,i3,kd)+szz23(
     & i1,i2,i3)*scs2(i1,i2,i3,kd)+tzz23(i1,i2,i3)*sct2(i1,i2,i3,kd)
        scxy23(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*scrr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*scss2(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & ty(i1,i2,i3)*sctt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(
     & i1,i2,i3)*sx(i1,i2,i3))*scrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(i1,
     & i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*scrt2(i1,i2,i3,kd)+(sx(i1,i2,
     & i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*scst2(i1,i2,i3,kd)+
     & rxy23(i1,i2,i3)*scr2(i1,i2,i3,kd)+sxy23(i1,i2,i3)*scs2(i1,i2,
     & i3,kd)+txy23(i1,i2,i3)*sct2(i1,i2,i3,kd)
        scxz23(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*scrr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*scss2(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & tz(i1,i2,i3)*sctt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sx(i1,i2,i3))*scrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(i1,
     & i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*scrt2(i1,i2,i3,kd)+(sx(i1,i2,
     & i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*scst2(i1,i2,i3,kd)+
     & rxz23(i1,i2,i3)*scr2(i1,i2,i3,kd)+sxz23(i1,i2,i3)*scs2(i1,i2,
     & i3,kd)+txz23(i1,i2,i3)*sct2(i1,i2,i3,kd)
        scyz23(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*scrr2(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*scss2(i1,i2,i3,kd)+ty(i1,i2,i3)*
     & tz(i1,i2,i3)*sctt2(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sy(i1,i2,i3))*scrs2(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(i1,
     & i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*scrt2(i1,i2,i3,kd)+(sy(i1,i2,
     & i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*scst2(i1,i2,i3,kd)+
     & ryz23(i1,i2,i3)*scr2(i1,i2,i3,kd)+syz23(i1,i2,i3)*scs2(i1,i2,
     & i3,kd)+tyz23(i1,i2,i3)*sct2(i1,i2,i3,kd)
        sclaplacian23(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*scrr2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)*
     & *2+sz(i1,i2,i3)**2)*scss2(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(i1,
     & i2,i3)**2+tz(i1,i2,i3)**2)*sctt2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*
     & sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,
     & i3))*scrs2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(i1,
     & i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*scrt2(i1,i2,i3,
     & kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)+
     & sz(i1,i2,i3)*tz(i1,i2,i3))*scst2(i1,i2,i3,kd)+(rxx23(i1,i2,i3)+
     & ryy23(i1,i2,i3)+rzz23(i1,i2,i3))*scr2(i1,i2,i3,kd)+(sxx23(i1,
     & i2,i3)+syy23(i1,i2,i3)+szz23(i1,i2,i3))*scs2(i1,i2,i3,kd)+(
     & txx23(i1,i2,i3)+tyy23(i1,i2,i3)+tzz23(i1,i2,i3))*sct2(i1,i2,i3,
     & kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
! #If "" == "RX"
        scx23r(i1,i2,i3,kd)=(sc(i1+1,i2,i3,kd)-sc(i1-1,i2,i3,kd))*h12(
     & 0)
        scy23r(i1,i2,i3,kd)=(sc(i1,i2+1,i3,kd)-sc(i1,i2-1,i3,kd))*h12(
     & 1)
        scz23r(i1,i2,i3,kd)=(sc(i1,i2,i3+1,kd)-sc(i1,i2,i3-1,kd))*h12(
     & 2)
        scxx23r(i1,i2,i3,kd)=(-2.*sc(i1,i2,i3,kd)+(sc(i1+1,i2,i3,kd)+
     & sc(i1-1,i2,i3,kd)) )*h22(0)
        scyy23r(i1,i2,i3,kd)=(-2.*sc(i1,i2,i3,kd)+(sc(i1,i2+1,i3,kd)+
     & sc(i1,i2-1,i3,kd)) )*h22(1)
        scxy23r(i1,i2,i3,kd)=(scx23r(i1,i2+1,i3,kd)-scx23r(i1,i2-1,i3,
     & kd))*h12(1)
        sczz23r(i1,i2,i3,kd)=(-2.*sc(i1,i2,i3,kd)+(sc(i1,i2,i3+1,kd)+
     & sc(i1,i2,i3-1,kd)) )*h22(2)
        scxz23r(i1,i2,i3,kd)=(scx23r(i1,i2,i3+1,kd)-scx23r(i1,i2,i3-1,
     & kd))*h12(2)
        scyz23r(i1,i2,i3,kd)=(scy23r(i1,i2,i3+1,kd)-scy23r(i1,i2,i3-1,
     & kd))*h12(2)
        scx21r(i1,i2,i3,kd)= scx23r(i1,i2,i3,kd)
        scy21r(i1,i2,i3,kd)= scy23r(i1,i2,i3,kd)
        scz21r(i1,i2,i3,kd)= scz23r(i1,i2,i3,kd)
        scxx21r(i1,i2,i3,kd)= scxx23r(i1,i2,i3,kd)
        scyy21r(i1,i2,i3,kd)= scyy23r(i1,i2,i3,kd)
        sczz21r(i1,i2,i3,kd)= sczz23r(i1,i2,i3,kd)
        scxy21r(i1,i2,i3,kd)= scxy23r(i1,i2,i3,kd)
        scxz21r(i1,i2,i3,kd)= scxz23r(i1,i2,i3,kd)
        scyz21r(i1,i2,i3,kd)= scyz23r(i1,i2,i3,kd)
        sclaplacian21r(i1,i2,i3,kd)=scxx23r(i1,i2,i3,kd)
        scx22r(i1,i2,i3,kd)= scx23r(i1,i2,i3,kd)
        scy22r(i1,i2,i3,kd)= scy23r(i1,i2,i3,kd)
        scz22r(i1,i2,i3,kd)= scz23r(i1,i2,i3,kd)
        scxx22r(i1,i2,i3,kd)= scxx23r(i1,i2,i3,kd)
        scyy22r(i1,i2,i3,kd)= scyy23r(i1,i2,i3,kd)
        sczz22r(i1,i2,i3,kd)= sczz23r(i1,i2,i3,kd)
        scxy22r(i1,i2,i3,kd)= scxy23r(i1,i2,i3,kd)
        scxz22r(i1,i2,i3,kd)= scxz23r(i1,i2,i3,kd)
        scyz22r(i1,i2,i3,kd)= scyz23r(i1,i2,i3,kd)
        sclaplacian22r(i1,i2,i3,kd)=scxx23r(i1,i2,i3,kd)+scyy23r(i1,i2,
     & i3,kd)
        sclaplacian23r(i1,i2,i3,kd)=scxx23r(i1,i2,i3,kd)+scyy23r(i1,i2,
     & i3,kd)+sczz23r(i1,i2,i3,kd)
        scxxx22r(i1,i2,i3,kd)=(-2.*(sc(i1+1,i2,i3,kd)-sc(i1-1,i2,i3,kd)
     & )+(sc(i1+2,i2,i3,kd)-sc(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
        scyyy22r(i1,i2,i3,kd)=(-2.*(sc(i1,i2+1,i3,kd)-sc(i1,i2-1,i3,kd)
     & )+(sc(i1,i2+2,i3,kd)-sc(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
        scxxy22r(i1,i2,i3,kd)=( scxx22r(i1,i2+1,i3,kd)-scxx22r(i1,i2-1,
     & i3,kd))/(2.*dx(1))
        scxyy22r(i1,i2,i3,kd)=( scyy22r(i1+1,i2,i3,kd)-scyy22r(i1-1,i2,
     & i3,kd))/(2.*dx(0))
        scxxxx22r(i1,i2,i3,kd)=(6.*sc(i1,i2,i3,kd)-4.*(sc(i1+1,i2,i3,
     & kd)+sc(i1-1,i2,i3,kd))+(sc(i1+2,i2,i3,kd)+sc(i1-2,i2,i3,kd)) )
     & /(dx(0)**4)
        scyyyy22r(i1,i2,i3,kd)=(6.*sc(i1,i2,i3,kd)-4.*(sc(i1,i2+1,i3,
     & kd)+sc(i1,i2-1,i3,kd))+(sc(i1,i2+2,i3,kd)+sc(i1,i2-2,i3,kd)) )
     & /(dx(1)**4)
        scxxyy22r(i1,i2,i3,kd)=( 4.*sc(i1,i2,i3,kd)     -2.*(sc(i1+1,
     & i2,i3,kd)+sc(i1-1,i2,i3,kd)+sc(i1,i2+1,i3,kd)+sc(i1,i2-1,i3,kd)
     & )   +   (sc(i1+1,i2+1,i3,kd)+sc(i1-1,i2+1,i3,kd)+sc(i1+1,i2-1,
     & i3,kd)+sc(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
        ! 2D laplacian squared = sc.xxxx + 2 sc.xxyy + sc.yyyy
        scLapSq22r(i1,i2,i3,kd)= ( 6.*sc(i1,i2,i3,kd)   - 4.*(sc(i1+1,
     & i2,i3,kd)+sc(i1-1,i2,i3,kd))    +(sc(i1+2,i2,i3,kd)+sc(i1-2,i2,
     & i3,kd)) )/(dx(0)**4) +( 6.*sc(i1,i2,i3,kd)    -4.*(sc(i1,i2+1,
     & i3,kd)+sc(i1,i2-1,i3,kd))    +(sc(i1,i2+2,i3,kd)+sc(i1,i2-2,i3,
     & kd)) )/(dx(1)**4)  +( 8.*sc(i1,i2,i3,kd)     -4.*(sc(i1+1,i2,
     & i3,kd)+sc(i1-1,i2,i3,kd)+sc(i1,i2+1,i3,kd)+sc(i1,i2-1,i3,kd))  
     &  +2.*(sc(i1+1,i2+1,i3,kd)+sc(i1-1,i2+1,i3,kd)+sc(i1+1,i2-1,i3,
     & kd)+sc(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
        scxxx23r(i1,i2,i3,kd)=(-2.*(sc(i1+1,i2,i3,kd)-sc(i1-1,i2,i3,kd)
     & )+(sc(i1+2,i2,i3,kd)-sc(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
        scyyy23r(i1,i2,i3,kd)=(-2.*(sc(i1,i2+1,i3,kd)-sc(i1,i2-1,i3,kd)
     & )+(sc(i1,i2+2,i3,kd)-sc(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
        sczzz23r(i1,i2,i3,kd)=(-2.*(sc(i1,i2,i3+1,kd)-sc(i1,i2,i3-1,kd)
     & )+(sc(i1,i2,i3+2,kd)-sc(i1,i2,i3-2,kd)) )*h22(1)*h12(2)
        scxxy23r(i1,i2,i3,kd)=( scxx22r(i1,i2+1,i3,kd)-scxx22r(i1,i2-1,
     & i3,kd))/(2.*dx(1))
        scxyy23r(i1,i2,i3,kd)=( scyy22r(i1+1,i2,i3,kd)-scyy22r(i1-1,i2,
     & i3,kd))/(2.*dx(0))
        scxxz23r(i1,i2,i3,kd)=( scxx22r(i1,i2,i3+1,kd)-scxx22r(i1,i2,
     & i3-1,kd))/(2.*dx(2))
        scyyz23r(i1,i2,i3,kd)=( scyy22r(i1,i2,i3+1,kd)-scyy22r(i1,i2,
     & i3-1,kd))/(2.*dx(2))
        scxzz23r(i1,i2,i3,kd)=( sczz22r(i1+1,i2,i3,kd)-sczz22r(i1-1,i2,
     & i3,kd))/(2.*dx(0))
        scyzz23r(i1,i2,i3,kd)=( sczz22r(i1,i2+1,i3,kd)-sczz22r(i1,i2-1,
     & i3,kd))/(2.*dx(1))
        scxxxx23r(i1,i2,i3,kd)=(6.*sc(i1,i2,i3,kd)-4.*(sc(i1+1,i2,i3,
     & kd)+sc(i1-1,i2,i3,kd))+(sc(i1+2,i2,i3,kd)+sc(i1-2,i2,i3,kd)) )
     & /(dx(0)**4)
        scyyyy23r(i1,i2,i3,kd)=(6.*sc(i1,i2,i3,kd)-4.*(sc(i1,i2+1,i3,
     & kd)+sc(i1,i2-1,i3,kd))+(sc(i1,i2+2,i3,kd)+sc(i1,i2-2,i3,kd)) )
     & /(dx(1)**4)
        sczzzz23r(i1,i2,i3,kd)=(6.*sc(i1,i2,i3,kd)-4.*(sc(i1,i2,i3+1,
     & kd)+sc(i1,i2,i3-1,kd))+(sc(i1,i2,i3+2,kd)+sc(i1,i2,i3-2,kd)) )
     & /(dx(2)**4)
        scxxyy23r(i1,i2,i3,kd)=( 4.*sc(i1,i2,i3,kd)     -2.*(sc(i1+1,
     & i2,i3,kd)+sc(i1-1,i2,i3,kd)+sc(i1,i2+1,i3,kd)+sc(i1,i2-1,i3,kd)
     & )   +   (sc(i1+1,i2+1,i3,kd)+sc(i1-1,i2+1,i3,kd)+sc(i1+1,i2-1,
     & i3,kd)+sc(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
        scxxzz23r(i1,i2,i3,kd)=( 4.*sc(i1,i2,i3,kd)     -2.*(sc(i1+1,
     & i2,i3,kd)+sc(i1-1,i2,i3,kd)+sc(i1,i2,i3+1,kd)+sc(i1,i2,i3-1,kd)
     & )   +   (sc(i1+1,i2,i3+1,kd)+sc(i1-1,i2,i3+1,kd)+sc(i1+1,i2,i3-
     & 1,kd)+sc(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)
        scyyzz23r(i1,i2,i3,kd)=( 4.*sc(i1,i2,i3,kd)     -2.*(sc(i1,i2+
     & 1,i3,kd)  +sc(i1,i2-1,i3,kd)+  sc(i1,i2  ,i3+1,kd)+sc(i1,i2  ,
     & i3-1,kd))   +   (sc(i1,i2+1,i3+1,kd)+sc(i1,i2-1,i3+1,kd)+sc(i1,
     & i2+1,i3-1,kd)+sc(i1,i2-1,i3-1,kd)) )/(dx(1)**2*dx(2)**2)
        ! 3D laplacian squared = sc.xxxx + sc.yyyy + sc.zzzz + 2 (sc.xxyy + sc.xxzz + sc.yyzz )
        scLapSq23r(i1,i2,i3,kd)= ( 6.*sc(i1,i2,i3,kd)   - 4.*(sc(i1+1,
     & i2,i3,kd)+sc(i1-1,i2,i3,kd))    +(sc(i1+2,i2,i3,kd)+sc(i1-2,i2,
     & i3,kd)) )/(dx(0)**4) +( 6.*sc(i1,i2,i3,kd)    -4.*(sc(i1,i2+1,
     & i3,kd)+sc(i1,i2-1,i3,kd))    +(sc(i1,i2+2,i3,kd)+sc(i1,i2-2,i3,
     & kd)) )/(dx(1)**4)  +( 6.*sc(i1,i2,i3,kd)    -4.*(sc(i1,i2,i3+1,
     & kd)+sc(i1,i2,i3-1,kd))    +(sc(i1,i2,i3+2,kd)+sc(i1,i2,i3-2,kd)
     & ) )/(dx(2)**4)  +( 8.*sc(i1,i2,i3,kd)     -4.*(sc(i1+1,i2,i3,
     & kd)  +sc(i1-1,i2,i3,kd)  +sc(i1  ,i2+1,i3,kd)+sc(i1  ,i2-1,i3,
     & kd))   +2.*(sc(i1+1,i2+1,i3,kd)+sc(i1-1,i2+1,i3,kd)+sc(i1+1,i2-
     & 1,i3,kd)+sc(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)+( 8.*sc(i1,
     & i2,i3,kd)     -4.*(sc(i1+1,i2,i3,kd)  +sc(i1-1,i2,i3,kd)  +sc(
     & i1  ,i2,i3+1,kd)+sc(i1  ,i2,i3-1,kd))   +2.*(sc(i1+1,i2,i3+1,
     & kd)+sc(i1-1,i2,i3+1,kd)+sc(i1+1,i2,i3-1,kd)+sc(i1-1,i2,i3-1,kd)
     & ) )/(dx(0)**2*dx(2)**2)+( 8.*sc(i1,i2,i3,kd)     -4.*(sc(i1,i2+
     & 1,i3,kd)  +sc(i1,i2-1,i3,kd)  +sc(i1,i2  ,i3+1,kd)+sc(i1,i2  ,
     & i3-1,kd))   +2.*(sc(i1,i2+1,i3+1,kd)+sc(i1,i2-1,i3+1,kd)+sc(i1,
     & i2+1,i3-1,kd)+sc(i1,i2-1,i3-1,kd)) )/(dx(1)**2*dx(2)**2)
! defineDifferenceOrder4Components1(sc,)
! #If "" == "RX"
        scr4(i1,i2,i3,kd)=(8.*(sc(i1+1,i2,i3,kd)-sc(i1-1,i2,i3,kd))-(
     & sc(i1+2,i2,i3,kd)-sc(i1-2,i2,i3,kd)))*d14(0)
        scs4(i1,i2,i3,kd)=(8.*(sc(i1,i2+1,i3,kd)-sc(i1,i2-1,i3,kd))-(
     & sc(i1,i2+2,i3,kd)-sc(i1,i2-2,i3,kd)))*d14(1)
        sct4(i1,i2,i3,kd)=(8.*(sc(i1,i2,i3+1,kd)-sc(i1,i2,i3-1,kd))-(
     & sc(i1,i2,i3+2,kd)-sc(i1,i2,i3-2,kd)))*d14(2)
        scrr4(i1,i2,i3,kd)=(-30.*sc(i1,i2,i3,kd)+16.*(sc(i1+1,i2,i3,kd)
     & +sc(i1-1,i2,i3,kd))-(sc(i1+2,i2,i3,kd)+sc(i1-2,i2,i3,kd)) )*
     & d24(0)
        scss4(i1,i2,i3,kd)=(-30.*sc(i1,i2,i3,kd)+16.*(sc(i1,i2+1,i3,kd)
     & +sc(i1,i2-1,i3,kd))-(sc(i1,i2+2,i3,kd)+sc(i1,i2-2,i3,kd)) )*
     & d24(1)
        sctt4(i1,i2,i3,kd)=(-30.*sc(i1,i2,i3,kd)+16.*(sc(i1,i2,i3+1,kd)
     & +sc(i1,i2,i3-1,kd))-(sc(i1,i2,i3+2,kd)+sc(i1,i2,i3-2,kd)) )*
     & d24(2)
        scrs4(i1,i2,i3,kd)=(8.*(scr4(i1,i2+1,i3,kd)-scr4(i1,i2-1,i3,kd)
     & )-(scr4(i1,i2+2,i3,kd)-scr4(i1,i2-2,i3,kd)))*d14(1)
        scrt4(i1,i2,i3,kd)=(8.*(scr4(i1,i2,i3+1,kd)-scr4(i1,i2,i3-1,kd)
     & )-(scr4(i1,i2,i3+2,kd)-scr4(i1,i2,i3-2,kd)))*d14(2)
        scst4(i1,i2,i3,kd)=(8.*(scs4(i1,i2,i3+1,kd)-scs4(i1,i2,i3-1,kd)
     & )-(scs4(i1,i2,i3+2,kd)-scs4(i1,i2,i3-2,kd)))*d14(2)
! #If "" == "RX"
        scx41(i1,i2,i3,kd)= rx(i1,i2,i3)*scr4(i1,i2,i3,kd)
        scy41(i1,i2,i3,kd)=0
        scz41(i1,i2,i3,kd)=0
        scx42(i1,i2,i3,kd)= rx(i1,i2,i3)*scr4(i1,i2,i3,kd)+sx(i1,i2,i3)
     & *scs4(i1,i2,i3,kd)
        scy42(i1,i2,i3,kd)= ry(i1,i2,i3)*scr4(i1,i2,i3,kd)+sy(i1,i2,i3)
     & *scs4(i1,i2,i3,kd)
        scz42(i1,i2,i3,kd)=0
        scx43(i1,i2,i3,kd)=rx(i1,i2,i3)*scr4(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & scs4(i1,i2,i3,kd)+tx(i1,i2,i3)*sct4(i1,i2,i3,kd)
        scy43(i1,i2,i3,kd)=ry(i1,i2,i3)*scr4(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & scs4(i1,i2,i3,kd)+ty(i1,i2,i3)*sct4(i1,i2,i3,kd)
        scz43(i1,i2,i3,kd)=rz(i1,i2,i3)*scr4(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & scs4(i1,i2,i3,kd)+tz(i1,i2,i3)*sct4(i1,i2,i3,kd)
! #If "" == "RX"
        scxx41(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*scrr4(i1,i2,i3,kd)+(
     & rxx42(i1,i2,i3))*scr4(i1,i2,i3,kd)
        scyy41(i1,i2,i3,kd)=0
        scxy41(i1,i2,i3,kd)=0
        scxz41(i1,i2,i3,kd)=0
        scyz41(i1,i2,i3,kd)=0
        sczz41(i1,i2,i3,kd)=0
        sclaplacian41(i1,i2,i3,kd)=scxx41(i1,i2,i3,kd)
        scxx42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*scrr4(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*scrs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2)
     & *scss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3))*scr4(i1,i2,i3,kd)+(sxx42(
     & i1,i2,i3))*scs4(i1,i2,i3,kd)
        scyy42(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*scrr4(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*scrs4(i1,i2,i3,kd)+(sy(i1,i2,i3)**2)
     & *scss4(i1,i2,i3,kd)+(ryy42(i1,i2,i3))*scr4(i1,i2,i3,kd)+(syy42(
     & i1,i2,i3))*scs4(i1,i2,i3,kd)
        scxy42(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*scrr4(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & scrs4(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*scss4(i1,i2,i3,kd)
     & +rxy42(i1,i2,i3)*scr4(i1,i2,i3,kd)+sxy42(i1,i2,i3)*scs4(i1,i2,
     & i3,kd)
        scxz42(i1,i2,i3,kd)=0
        scyz42(i1,i2,i3,kd)=0
        sczz42(i1,i2,i3,kd)=0
        sclaplacian42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & scrr4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*
     & sy(i1,i2,i3))*scrs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)*
     & *2)*scss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3)+ryy42(i1,i2,i3))*scr4(
     & i1,i2,i3,kd)+(sxx42(i1,i2,i3)+syy42(i1,i2,i3))*scs4(i1,i2,i3,
     & kd)
        scxx43(i1,i2,i3,kd)=rx(i1,i2,i3)**2*scrr4(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*scss4(i1,i2,i3,kd)+tx(i1,i2,i3)**2*sctt4(i1,i2,i3,kd)
     & +2.*rx(i1,i2,i3)*sx(i1,i2,i3)*scrs4(i1,i2,i3,kd)+2.*rx(i1,i2,
     & i3)*tx(i1,i2,i3)*scrt4(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,i2,
     & i3)*scst4(i1,i2,i3,kd)+rxx43(i1,i2,i3)*scr4(i1,i2,i3,kd)+sxx43(
     & i1,i2,i3)*scs4(i1,i2,i3,kd)+txx43(i1,i2,i3)*sct4(i1,i2,i3,kd)
        scyy43(i1,i2,i3,kd)=ry(i1,i2,i3)**2*scrr4(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*scss4(i1,i2,i3,kd)+ty(i1,i2,i3)**2*sctt4(i1,i2,i3,kd)
     & +2.*ry(i1,i2,i3)*sy(i1,i2,i3)*scrs4(i1,i2,i3,kd)+2.*ry(i1,i2,
     & i3)*ty(i1,i2,i3)*scrt4(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,i2,
     & i3)*scst4(i1,i2,i3,kd)+ryy43(i1,i2,i3)*scr4(i1,i2,i3,kd)+syy43(
     & i1,i2,i3)*scs4(i1,i2,i3,kd)+tyy43(i1,i2,i3)*sct4(i1,i2,i3,kd)
        sczz43(i1,i2,i3,kd)=rz(i1,i2,i3)**2*scrr4(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*scss4(i1,i2,i3,kd)+tz(i1,i2,i3)**2*sctt4(i1,i2,i3,kd)
     & +2.*rz(i1,i2,i3)*sz(i1,i2,i3)*scrs4(i1,i2,i3,kd)+2.*rz(i1,i2,
     & i3)*tz(i1,i2,i3)*scrt4(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,i2,
     & i3)*scst4(i1,i2,i3,kd)+rzz43(i1,i2,i3)*scr4(i1,i2,i3,kd)+szz43(
     & i1,i2,i3)*scs4(i1,i2,i3,kd)+tzz43(i1,i2,i3)*sct4(i1,i2,i3,kd)
        scxy43(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*scrr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*scss4(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & ty(i1,i2,i3)*sctt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(
     & i1,i2,i3)*sx(i1,i2,i3))*scrs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(i1,
     & i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*scrt4(i1,i2,i3,kd)+(sx(i1,i2,
     & i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*scst4(i1,i2,i3,kd)+
     & rxy43(i1,i2,i3)*scr4(i1,i2,i3,kd)+sxy43(i1,i2,i3)*scs4(i1,i2,
     & i3,kd)+txy43(i1,i2,i3)*sct4(i1,i2,i3,kd)
        scxz43(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*scrr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*scss4(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & tz(i1,i2,i3)*sctt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sx(i1,i2,i3))*scrs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(i1,
     & i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*scrt4(i1,i2,i3,kd)+(sx(i1,i2,
     & i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*scst4(i1,i2,i3,kd)+
     & rxz43(i1,i2,i3)*scr4(i1,i2,i3,kd)+sxz43(i1,i2,i3)*scs4(i1,i2,
     & i3,kd)+txz43(i1,i2,i3)*sct4(i1,i2,i3,kd)
        scyz43(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*scrr4(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*scss4(i1,i2,i3,kd)+ty(i1,i2,i3)*
     & tz(i1,i2,i3)*sctt4(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sy(i1,i2,i3))*scrs4(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(i1,
     & i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*scrt4(i1,i2,i3,kd)+(sy(i1,i2,
     & i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*scst4(i1,i2,i3,kd)+
     & ryz43(i1,i2,i3)*scr4(i1,i2,i3,kd)+syz43(i1,i2,i3)*scs4(i1,i2,
     & i3,kd)+tyz43(i1,i2,i3)*sct4(i1,i2,i3,kd)
        sclaplacian43(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*scrr4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)*
     & *2+sz(i1,i2,i3)**2)*scss4(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(i1,
     & i2,i3)**2+tz(i1,i2,i3)**2)*sctt4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*
     & sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,
     & i3))*scrs4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(i1,
     & i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*scrt4(i1,i2,i3,
     & kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)+
     & sz(i1,i2,i3)*tz(i1,i2,i3))*scst4(i1,i2,i3,kd)+(rxx43(i1,i2,i3)+
     & ryy43(i1,i2,i3)+rzz43(i1,i2,i3))*scr4(i1,i2,i3,kd)+(sxx43(i1,
     & i2,i3)+syy43(i1,i2,i3)+szz43(i1,i2,i3))*scs4(i1,i2,i3,kd)+(
     & txx43(i1,i2,i3)+tyy43(i1,i2,i3)+tzz43(i1,i2,i3))*sct4(i1,i2,i3,
     & kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
! #If "" == "RX"
        scx43r(i1,i2,i3,kd)=(8.*(sc(i1+1,i2,i3,kd)-sc(i1-1,i2,i3,kd))-(
     & sc(i1+2,i2,i3,kd)-sc(i1-2,i2,i3,kd)))*h41(0)
        scy43r(i1,i2,i3,kd)=(8.*(sc(i1,i2+1,i3,kd)-sc(i1,i2-1,i3,kd))-(
     & sc(i1,i2+2,i3,kd)-sc(i1,i2-2,i3,kd)))*h41(1)
        scz43r(i1,i2,i3,kd)=(8.*(sc(i1,i2,i3+1,kd)-sc(i1,i2,i3-1,kd))-(
     & sc(i1,i2,i3+2,kd)-sc(i1,i2,i3-2,kd)))*h41(2)
        scxx43r(i1,i2,i3,kd)=( -30.*sc(i1,i2,i3,kd)+16.*(sc(i1+1,i2,i3,
     & kd)+sc(i1-1,i2,i3,kd))-(sc(i1+2,i2,i3,kd)+sc(i1-2,i2,i3,kd)) )*
     & h42(0)
        scyy43r(i1,i2,i3,kd)=( -30.*sc(i1,i2,i3,kd)+16.*(sc(i1,i2+1,i3,
     & kd)+sc(i1,i2-1,i3,kd))-(sc(i1,i2+2,i3,kd)+sc(i1,i2-2,i3,kd)) )*
     & h42(1)
        sczz43r(i1,i2,i3,kd)=( -30.*sc(i1,i2,i3,kd)+16.*(sc(i1,i2,i3+1,
     & kd)+sc(i1,i2,i3-1,kd))-(sc(i1,i2,i3+2,kd)+sc(i1,i2,i3-2,kd)) )*
     & h42(2)
        scxy43r(i1,i2,i3,kd)=( (sc(i1+2,i2+2,i3,kd)-sc(i1-2,i2+2,i3,kd)
     & - sc(i1+2,i2-2,i3,kd)+sc(i1-2,i2-2,i3,kd)) +8.*(sc(i1-1,i2+2,
     & i3,kd)-sc(i1-1,i2-2,i3,kd)-sc(i1+1,i2+2,i3,kd)+sc(i1+1,i2-2,i3,
     & kd) +sc(i1+2,i2-1,i3,kd)-sc(i1-2,i2-1,i3,kd)-sc(i1+2,i2+1,i3,
     & kd)+sc(i1-2,i2+1,i3,kd))+64.*(sc(i1+1,i2+1,i3,kd)-sc(i1-1,i2+1,
     & i3,kd)- sc(i1+1,i2-1,i3,kd)+sc(i1-1,i2-1,i3,kd)))*(h41(0)*h41(
     & 1))
        scxz43r(i1,i2,i3,kd)=( (sc(i1+2,i2,i3+2,kd)-sc(i1-2,i2,i3+2,kd)
     & -sc(i1+2,i2,i3-2,kd)+sc(i1-2,i2,i3-2,kd)) +8.*(sc(i1-1,i2,i3+2,
     & kd)-sc(i1-1,i2,i3-2,kd)-sc(i1+1,i2,i3+2,kd)+sc(i1+1,i2,i3-2,kd)
     &  +sc(i1+2,i2,i3-1,kd)-sc(i1-2,i2,i3-1,kd)- sc(i1+2,i2,i3+1,kd)+
     & sc(i1-2,i2,i3+1,kd)) +64.*(sc(i1+1,i2,i3+1,kd)-sc(i1-1,i2,i3+1,
     & kd)-sc(i1+1,i2,i3-1,kd)+sc(i1-1,i2,i3-1,kd)) )*(h41(0)*h41(2))
        scyz43r(i1,i2,i3,kd)=( (sc(i1,i2+2,i3+2,kd)-sc(i1,i2-2,i3+2,kd)
     & -sc(i1,i2+2,i3-2,kd)+sc(i1,i2-2,i3-2,kd)) +8.*(sc(i1,i2-1,i3+2,
     & kd)-sc(i1,i2-1,i3-2,kd)-sc(i1,i2+1,i3+2,kd)+sc(i1,i2+1,i3-2,kd)
     &  +sc(i1,i2+2,i3-1,kd)-sc(i1,i2-2,i3-1,kd)-sc(i1,i2+2,i3+1,kd)+
     & sc(i1,i2-2,i3+1,kd)) +64.*(sc(i1,i2+1,i3+1,kd)-sc(i1,i2-1,i3+1,
     & kd)-sc(i1,i2+1,i3-1,kd)+sc(i1,i2-1,i3-1,kd)) )*(h41(1)*h41(2))
        scx41r(i1,i2,i3,kd)= scx43r(i1,i2,i3,kd)
        scy41r(i1,i2,i3,kd)= scy43r(i1,i2,i3,kd)
        scz41r(i1,i2,i3,kd)= scz43r(i1,i2,i3,kd)
        scxx41r(i1,i2,i3,kd)= scxx43r(i1,i2,i3,kd)
        scyy41r(i1,i2,i3,kd)= scyy43r(i1,i2,i3,kd)
        sczz41r(i1,i2,i3,kd)= sczz43r(i1,i2,i3,kd)
        scxy41r(i1,i2,i3,kd)= scxy43r(i1,i2,i3,kd)
        scxz41r(i1,i2,i3,kd)= scxz43r(i1,i2,i3,kd)
        scyz41r(i1,i2,i3,kd)= scyz43r(i1,i2,i3,kd)
        sclaplacian41r(i1,i2,i3,kd)=scxx43r(i1,i2,i3,kd)
        scx42r(i1,i2,i3,kd)= scx43r(i1,i2,i3,kd)
        scy42r(i1,i2,i3,kd)= scy43r(i1,i2,i3,kd)
        scz42r(i1,i2,i3,kd)= scz43r(i1,i2,i3,kd)
        scxx42r(i1,i2,i3,kd)= scxx43r(i1,i2,i3,kd)
        scyy42r(i1,i2,i3,kd)= scyy43r(i1,i2,i3,kd)
        sczz42r(i1,i2,i3,kd)= sczz43r(i1,i2,i3,kd)
        scxy42r(i1,i2,i3,kd)= scxy43r(i1,i2,i3,kd)
        scxz42r(i1,i2,i3,kd)= scxz43r(i1,i2,i3,kd)
        scyz42r(i1,i2,i3,kd)= scyz43r(i1,i2,i3,kd)
        sclaplacian42r(i1,i2,i3,kd)=scxx43r(i1,i2,i3,kd)+scyy43r(i1,i2,
     & i3,kd)
        sclaplacian43r(i1,i2,i3,kd)=scxx43r(i1,i2,i3,kd)+scyy43r(i1,i2,
     & i3,kd)+sczz43r(i1,i2,i3,kd)
! defineDifferenceOrder6Components1(sc,)
! #If "" == "RX"
        scr6(i1,i2,i3,kd)=(45.*(sc(i1+1,i2,i3,kd)-sc(i1-1,i2,i3,kd))-
     & 9.*(sc(i1+2,i2,i3,kd)-sc(i1-2,i2,i3,kd))+(sc(i1+3,i2,i3,kd)-sc(
     & i1-3,i2,i3,kd)))*d16(0)
        scs6(i1,i2,i3,kd)=(45.*(sc(i1,i2+1,i3,kd)-sc(i1,i2-1,i3,kd))-
     & 9.*(sc(i1,i2+2,i3,kd)-sc(i1,i2-2,i3,kd))+(sc(i1,i2+3,i3,kd)-sc(
     & i1,i2-3,i3,kd)))*d16(1)
        sct6(i1,i2,i3,kd)=(45.*(sc(i1,i2,i3+1,kd)-sc(i1,i2,i3-1,kd))-
     & 9.*(sc(i1,i2,i3+2,kd)-sc(i1,i2,i3-2,kd))+(sc(i1,i2,i3+3,kd)-sc(
     & i1,i2,i3-3,kd)))*d16(2)
        scrr6(i1,i2,i3,kd)=(-490.*sc(i1,i2,i3,kd)+270.*(sc(i1+1,i2,i3,
     & kd)+sc(i1-1,i2,i3,kd))-27.*(sc(i1+2,i2,i3,kd)+sc(i1-2,i2,i3,kd)
     & )+2.*(sc(i1+3,i2,i3,kd)+sc(i1-3,i2,i3,kd)) )*d26(0)
        scss6(i1,i2,i3,kd)=(-490.*sc(i1,i2,i3,kd)+270.*(sc(i1,i2+1,i3,
     & kd)+sc(i1,i2-1,i3,kd))-27.*(sc(i1,i2+2,i3,kd)+sc(i1,i2-2,i3,kd)
     & )+2.*(sc(i1,i2+3,i3,kd)+sc(i1,i2-3,i3,kd)) )*d26(1)
        sctt6(i1,i2,i3,kd)=(-490.*sc(i1,i2,i3,kd)+270.*(sc(i1,i2,i3+1,
     & kd)+sc(i1,i2,i3-1,kd))-27.*(sc(i1,i2,i3+2,kd)+sc(i1,i2,i3-2,kd)
     & )+2.*(sc(i1,i2,i3+3,kd)+sc(i1,i2,i3-3,kd)) )*d26(2)
        scrs6(i1,i2,i3,kd)=(45.*(scr6(i1,i2+1,i3,kd)-scr6(i1,i2-1,i3,
     & kd))-9.*(scr6(i1,i2+2,i3,kd)-scr6(i1,i2-2,i3,kd))+(scr6(i1,i2+
     & 3,i3,kd)-scr6(i1,i2-3,i3,kd)))*d16(1)
        scrt6(i1,i2,i3,kd)=(45.*(scr6(i1,i2,i3+1,kd)-scr6(i1,i2,i3-1,
     & kd))-9.*(scr6(i1,i2,i3+2,kd)-scr6(i1,i2,i3-2,kd))+(scr6(i1,i2,
     & i3+3,kd)-scr6(i1,i2,i3-3,kd)))*d16(2)
        scst6(i1,i2,i3,kd)=(45.*(scs6(i1,i2,i3+1,kd)-scs6(i1,i2,i3-1,
     & kd))-9.*(scs6(i1,i2,i3+2,kd)-scs6(i1,i2,i3-2,kd))+(scs6(i1,i2,
     & i3+3,kd)-scs6(i1,i2,i3-3,kd)))*d16(2)
! #If "" == "RX"
        scx61(i1,i2,i3,kd)= rx(i1,i2,i3)*scr6(i1,i2,i3,kd)
        scy61(i1,i2,i3,kd)=0
        scz61(i1,i2,i3,kd)=0
        scx62(i1,i2,i3,kd)= rx(i1,i2,i3)*scr6(i1,i2,i3,kd)+sx(i1,i2,i3)
     & *scs6(i1,i2,i3,kd)
        scy62(i1,i2,i3,kd)= ry(i1,i2,i3)*scr6(i1,i2,i3,kd)+sy(i1,i2,i3)
     & *scs6(i1,i2,i3,kd)
        scz62(i1,i2,i3,kd)=0
        scx63(i1,i2,i3,kd)=rx(i1,i2,i3)*scr6(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & scs6(i1,i2,i3,kd)+tx(i1,i2,i3)*sct6(i1,i2,i3,kd)
        scy63(i1,i2,i3,kd)=ry(i1,i2,i3)*scr6(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & scs6(i1,i2,i3,kd)+ty(i1,i2,i3)*sct6(i1,i2,i3,kd)
        scz63(i1,i2,i3,kd)=rz(i1,i2,i3)*scr6(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & scs6(i1,i2,i3,kd)+tz(i1,i2,i3)*sct6(i1,i2,i3,kd)
! #If "" == "RX"
        scxx61(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*scrr6(i1,i2,i3,kd)+(
     & rxx62(i1,i2,i3))*scr6(i1,i2,i3,kd)
        scyy61(i1,i2,i3,kd)=0
        scxy61(i1,i2,i3,kd)=0
        scxz61(i1,i2,i3,kd)=0
        scyz61(i1,i2,i3,kd)=0
        sczz61(i1,i2,i3,kd)=0
        sclaplacian61(i1,i2,i3,kd)=scxx61(i1,i2,i3,kd)
        scxx62(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*scrr6(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*scrs6(i1,i2,i3,kd)+(sx(i1,i2,i3)**2)
     & *scss6(i1,i2,i3,kd)+(rxx62(i1,i2,i3))*scr6(i1,i2,i3,kd)+(sxx62(
     & i1,i2,i3))*scs6(i1,i2,i3,kd)
        scyy62(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*scrr6(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*scrs6(i1,i2,i3,kd)+(sy(i1,i2,i3)**2)
     & *scss6(i1,i2,i3,kd)+(ryy62(i1,i2,i3))*scr6(i1,i2,i3,kd)+(syy62(
     & i1,i2,i3))*scs6(i1,i2,i3,kd)
        scxy62(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*scrr6(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & scrs6(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*scss6(i1,i2,i3,kd)
     & +rxy62(i1,i2,i3)*scr6(i1,i2,i3,kd)+sxy62(i1,i2,i3)*scs6(i1,i2,
     & i3,kd)
        scxz62(i1,i2,i3,kd)=0
        scyz62(i1,i2,i3,kd)=0
        sczz62(i1,i2,i3,kd)=0
        sclaplacian62(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & scrr6(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*
     & sy(i1,i2,i3))*scrs6(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)*
     & *2)*scss6(i1,i2,i3,kd)+(rxx62(i1,i2,i3)+ryy62(i1,i2,i3))*scr6(
     & i1,i2,i3,kd)+(sxx62(i1,i2,i3)+syy62(i1,i2,i3))*scs6(i1,i2,i3,
     & kd)
        scxx63(i1,i2,i3,kd)=rx(i1,i2,i3)**2*scrr6(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*scss6(i1,i2,i3,kd)+tx(i1,i2,i3)**2*sctt6(i1,i2,i3,kd)
     & +2.*rx(i1,i2,i3)*sx(i1,i2,i3)*scrs6(i1,i2,i3,kd)+2.*rx(i1,i2,
     & i3)*tx(i1,i2,i3)*scrt6(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,i2,
     & i3)*scst6(i1,i2,i3,kd)+rxx63(i1,i2,i3)*scr6(i1,i2,i3,kd)+sxx63(
     & i1,i2,i3)*scs6(i1,i2,i3,kd)+txx63(i1,i2,i3)*sct6(i1,i2,i3,kd)
        scyy63(i1,i2,i3,kd)=ry(i1,i2,i3)**2*scrr6(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*scss6(i1,i2,i3,kd)+ty(i1,i2,i3)**2*sctt6(i1,i2,i3,kd)
     & +2.*ry(i1,i2,i3)*sy(i1,i2,i3)*scrs6(i1,i2,i3,kd)+2.*ry(i1,i2,
     & i3)*ty(i1,i2,i3)*scrt6(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,i2,
     & i3)*scst6(i1,i2,i3,kd)+ryy63(i1,i2,i3)*scr6(i1,i2,i3,kd)+syy63(
     & i1,i2,i3)*scs6(i1,i2,i3,kd)+tyy63(i1,i2,i3)*sct6(i1,i2,i3,kd)
        sczz63(i1,i2,i3,kd)=rz(i1,i2,i3)**2*scrr6(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*scss6(i1,i2,i3,kd)+tz(i1,i2,i3)**2*sctt6(i1,i2,i3,kd)
     & +2.*rz(i1,i2,i3)*sz(i1,i2,i3)*scrs6(i1,i2,i3,kd)+2.*rz(i1,i2,
     & i3)*tz(i1,i2,i3)*scrt6(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,i2,
     & i3)*scst6(i1,i2,i3,kd)+rzz63(i1,i2,i3)*scr6(i1,i2,i3,kd)+szz63(
     & i1,i2,i3)*scs6(i1,i2,i3,kd)+tzz63(i1,i2,i3)*sct6(i1,i2,i3,kd)
        scxy63(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*scrr6(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*scss6(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & ty(i1,i2,i3)*sctt6(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(
     & i1,i2,i3)*sx(i1,i2,i3))*scrs6(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(i1,
     & i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*scrt6(i1,i2,i3,kd)+(sx(i1,i2,
     & i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*scst6(i1,i2,i3,kd)+
     & rxy63(i1,i2,i3)*scr6(i1,i2,i3,kd)+sxy63(i1,i2,i3)*scs6(i1,i2,
     & i3,kd)+txy63(i1,i2,i3)*sct6(i1,i2,i3,kd)
        scxz63(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*scrr6(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*scss6(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & tz(i1,i2,i3)*sctt6(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sx(i1,i2,i3))*scrs6(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(i1,
     & i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*scrt6(i1,i2,i3,kd)+(sx(i1,i2,
     & i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*scst6(i1,i2,i3,kd)+
     & rxz63(i1,i2,i3)*scr6(i1,i2,i3,kd)+sxz63(i1,i2,i3)*scs6(i1,i2,
     & i3,kd)+txz63(i1,i2,i3)*sct6(i1,i2,i3,kd)
        scyz63(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*scrr6(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*scss6(i1,i2,i3,kd)+ty(i1,i2,i3)*
     & tz(i1,i2,i3)*sctt6(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sy(i1,i2,i3))*scrs6(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(i1,
     & i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*scrt6(i1,i2,i3,kd)+(sy(i1,i2,
     & i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*scst6(i1,i2,i3,kd)+
     & ryz63(i1,i2,i3)*scr6(i1,i2,i3,kd)+syz63(i1,i2,i3)*scs6(i1,i2,
     & i3,kd)+tyz63(i1,i2,i3)*sct6(i1,i2,i3,kd)
        sclaplacian63(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*scrr6(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)*
     & *2+sz(i1,i2,i3)**2)*scss6(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(i1,
     & i2,i3)**2+tz(i1,i2,i3)**2)*sctt6(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*
     & sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,
     & i3))*scrs6(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(i1,
     & i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*scrt6(i1,i2,i3,
     & kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)+
     & sz(i1,i2,i3)*tz(i1,i2,i3))*scst6(i1,i2,i3,kd)+(rxx63(i1,i2,i3)+
     & ryy63(i1,i2,i3)+rzz63(i1,i2,i3))*scr6(i1,i2,i3,kd)+(sxx63(i1,
     & i2,i3)+syy63(i1,i2,i3)+szz63(i1,i2,i3))*scs6(i1,i2,i3,kd)+(
     & txx63(i1,i2,i3)+tyy63(i1,i2,i3)+tzz63(i1,i2,i3))*sct6(i1,i2,i3,
     & kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
! #If "" == "RX"
        scx63r(i1,i2,i3,kd)=(45.*(sc(i1+1,i2,i3,kd)-sc(i1-1,i2,i3,kd))-
     & 9.*(sc(i1+2,i2,i3,kd)-sc(i1-2,i2,i3,kd))+(sc(i1+3,i2,i3,kd)-sc(
     & i1-3,i2,i3,kd)))*h16(0)
        scy63r(i1,i2,i3,kd)=(45.*(sc(i1,i2+1,i3,kd)-sc(i1,i2-1,i3,kd))-
     & 9.*(sc(i1,i2+2,i3,kd)-sc(i1,i2-2,i3,kd))+(sc(i1,i2+3,i3,kd)-sc(
     & i1,i2-3,i3,kd)))*h16(1)
        scz63r(i1,i2,i3,kd)=(45.*(sc(i1,i2,i3+1,kd)-sc(i1,i2,i3-1,kd))-
     & 9.*(sc(i1,i2,i3+2,kd)-sc(i1,i2,i3-2,kd))+(sc(i1,i2,i3+3,kd)-sc(
     & i1,i2,i3-3,kd)))*h16(2)
        scxx63r(i1,i2,i3,kd)=(-490.*sc(i1,i2,i3,kd)+270.*(sc(i1+1,i2,
     & i3,kd)+sc(i1-1,i2,i3,kd))-27.*(sc(i1+2,i2,i3,kd)+sc(i1-2,i2,i3,
     & kd))+2.*(sc(i1+3,i2,i3,kd)+sc(i1-3,i2,i3,kd)) )*h26(0)
        scyy63r(i1,i2,i3,kd)=(-490.*sc(i1,i2,i3,kd)+270.*(sc(i1,i2+1,
     & i3,kd)+sc(i1,i2-1,i3,kd))-27.*(sc(i1,i2+2,i3,kd)+sc(i1,i2-2,i3,
     & kd))+2.*(sc(i1,i2+3,i3,kd)+sc(i1,i2-3,i3,kd)) )*h26(1)
        sczz63r(i1,i2,i3,kd)=(-490.*sc(i1,i2,i3,kd)+270.*(sc(i1,i2,i3+
     & 1,kd)+sc(i1,i2,i3-1,kd))-27.*(sc(i1,i2,i3+2,kd)+sc(i1,i2,i3-2,
     & kd))+2.*(sc(i1,i2,i3+3,kd)+sc(i1,i2,i3-3,kd)) )*h26(2)
        scxy63r(i1,i2,i3,kd)=(45.*(scx63r(i1,i2+1,i3,kd)-scx63r(i1,i2-
     & 1,i3,kd))-9.*(scx63r(i1,i2+2,i3,kd)-scx63r(i1,i2-2,i3,kd))+(
     & scx63r(i1,i2+3,i3,kd)-scx63r(i1,i2-3,i3,kd)))*h16(1)
        scxz63r(i1,i2,i3,kd)=(45.*(scx63r(i1,i2,i3+1,kd)-scx63r(i1,i2,
     & i3-1,kd))-9.*(scx63r(i1,i2,i3+2,kd)-scx63r(i1,i2,i3-2,kd))+(
     & scx63r(i1,i2,i3+3,kd)-scx63r(i1,i2,i3-3,kd)))*h16(2)
        scyz63r(i1,i2,i3,kd)=(45.*(scy63r(i1,i2,i3+1,kd)-scy63r(i1,i2,
     & i3-1,kd))-9.*(scy63r(i1,i2,i3+2,kd)-scy63r(i1,i2,i3-2,kd))+(
     & scy63r(i1,i2,i3+3,kd)-scy63r(i1,i2,i3-3,kd)))*h16(2)
        scx61r(i1,i2,i3,kd)= scx63r(i1,i2,i3,kd)
        scy61r(i1,i2,i3,kd)= scy63r(i1,i2,i3,kd)
        scz61r(i1,i2,i3,kd)= scz63r(i1,i2,i3,kd)
        scxx61r(i1,i2,i3,kd)= scxx63r(i1,i2,i3,kd)
        scyy61r(i1,i2,i3,kd)= scyy63r(i1,i2,i3,kd)
        sczz61r(i1,i2,i3,kd)= sczz63r(i1,i2,i3,kd)
        scxy61r(i1,i2,i3,kd)= scxy63r(i1,i2,i3,kd)
        scxz61r(i1,i2,i3,kd)= scxz63r(i1,i2,i3,kd)
        scyz61r(i1,i2,i3,kd)= scyz63r(i1,i2,i3,kd)
        sclaplacian61r(i1,i2,i3,kd)=scxx63r(i1,i2,i3,kd)
        scx62r(i1,i2,i3,kd)= scx63r(i1,i2,i3,kd)
        scy62r(i1,i2,i3,kd)= scy63r(i1,i2,i3,kd)
        scz62r(i1,i2,i3,kd)= scz63r(i1,i2,i3,kd)
        scxx62r(i1,i2,i3,kd)= scxx63r(i1,i2,i3,kd)
        scyy62r(i1,i2,i3,kd)= scyy63r(i1,i2,i3,kd)
        sczz62r(i1,i2,i3,kd)= sczz63r(i1,i2,i3,kd)
        scxy62r(i1,i2,i3,kd)= scxy63r(i1,i2,i3,kd)
        scxz62r(i1,i2,i3,kd)= scxz63r(i1,i2,i3,kd)
        scyz62r(i1,i2,i3,kd)= scyz63r(i1,i2,i3,kd)
        sclaplacian62r(i1,i2,i3,kd)=scxx63r(i1,i2,i3,kd)+scyy63r(i1,i2,
     & i3,kd)
        sclaplacian63r(i1,i2,i3,kd)=scxx63r(i1,i2,i3,kd)+scyy63r(i1,i2,
     & i3,kd)+sczz63r(i1,i2,i3,kd)
! defineDifferenceOrder8Components1(sc,)
! #If "" == "RX"
        scr8(i1,i2,i3,kd)=(672.*(sc(i1+1,i2,i3,kd)-sc(i1-1,i2,i3,kd))-
     & 168.*(sc(i1+2,i2,i3,kd)-sc(i1-2,i2,i3,kd))+32.*(sc(i1+3,i2,i3,
     & kd)-sc(i1-3,i2,i3,kd))-3.*(sc(i1+4,i2,i3,kd)-sc(i1-4,i2,i3,kd))
     & )*d18(0)
        scs8(i1,i2,i3,kd)=(672.*(sc(i1,i2+1,i3,kd)-sc(i1,i2-1,i3,kd))-
     & 168.*(sc(i1,i2+2,i3,kd)-sc(i1,i2-2,i3,kd))+32.*(sc(i1,i2+3,i3,
     & kd)-sc(i1,i2-3,i3,kd))-3.*(sc(i1,i2+4,i3,kd)-sc(i1,i2-4,i3,kd))
     & )*d18(1)
        sct8(i1,i2,i3,kd)=(672.*(sc(i1,i2,i3+1,kd)-sc(i1,i2,i3-1,kd))-
     & 168.*(sc(i1,i2,i3+2,kd)-sc(i1,i2,i3-2,kd))+32.*(sc(i1,i2,i3+3,
     & kd)-sc(i1,i2,i3-3,kd))-3.*(sc(i1,i2,i3+4,kd)-sc(i1,i2,i3-4,kd))
     & )*d18(2)
        scrr8(i1,i2,i3,kd)=(-14350.*sc(i1,i2,i3,kd)+8064.*(sc(i1+1,i2,
     & i3,kd)+sc(i1-1,i2,i3,kd))-1008.*(sc(i1+2,i2,i3,kd)+sc(i1-2,i2,
     & i3,kd))+128.*(sc(i1+3,i2,i3,kd)+sc(i1-3,i2,i3,kd))-9.*(sc(i1+4,
     & i2,i3,kd)+sc(i1-4,i2,i3,kd)) )*d28(0)
        scss8(i1,i2,i3,kd)=(-14350.*sc(i1,i2,i3,kd)+8064.*(sc(i1,i2+1,
     & i3,kd)+sc(i1,i2-1,i3,kd))-1008.*(sc(i1,i2+2,i3,kd)+sc(i1,i2-2,
     & i3,kd))+128.*(sc(i1,i2+3,i3,kd)+sc(i1,i2-3,i3,kd))-9.*(sc(i1,
     & i2+4,i3,kd)+sc(i1,i2-4,i3,kd)) )*d28(1)
        sctt8(i1,i2,i3,kd)=(-14350.*sc(i1,i2,i3,kd)+8064.*(sc(i1,i2,i3+
     & 1,kd)+sc(i1,i2,i3-1,kd))-1008.*(sc(i1,i2,i3+2,kd)+sc(i1,i2,i3-
     & 2,kd))+128.*(sc(i1,i2,i3+3,kd)+sc(i1,i2,i3-3,kd))-9.*(sc(i1,i2,
     & i3+4,kd)+sc(i1,i2,i3-4,kd)) )*d28(2)
        scrs8(i1,i2,i3,kd)=(672.*(scr8(i1,i2+1,i3,kd)-scr8(i1,i2-1,i3,
     & kd))-168.*(scr8(i1,i2+2,i3,kd)-scr8(i1,i2-2,i3,kd))+32.*(scr8(
     & i1,i2+3,i3,kd)-scr8(i1,i2-3,i3,kd))-3.*(scr8(i1,i2+4,i3,kd)-
     & scr8(i1,i2-4,i3,kd)))*d18(1)
        scrt8(i1,i2,i3,kd)=(672.*(scr8(i1,i2,i3+1,kd)-scr8(i1,i2,i3-1,
     & kd))-168.*(scr8(i1,i2,i3+2,kd)-scr8(i1,i2,i3-2,kd))+32.*(scr8(
     & i1,i2,i3+3,kd)-scr8(i1,i2,i3-3,kd))-3.*(scr8(i1,i2,i3+4,kd)-
     & scr8(i1,i2,i3-4,kd)))*d18(2)
        scst8(i1,i2,i3,kd)=(672.*(scs8(i1,i2,i3+1,kd)-scs8(i1,i2,i3-1,
     & kd))-168.*(scs8(i1,i2,i3+2,kd)-scs8(i1,i2,i3-2,kd))+32.*(scs8(
     & i1,i2,i3+3,kd)-scs8(i1,i2,i3-3,kd))-3.*(scs8(i1,i2,i3+4,kd)-
     & scs8(i1,i2,i3-4,kd)))*d18(2)
! #If "" == "RX"
        scx81(i1,i2,i3,kd)= rx(i1,i2,i3)*scr8(i1,i2,i3,kd)
        scy81(i1,i2,i3,kd)=0
        scz81(i1,i2,i3,kd)=0
        scx82(i1,i2,i3,kd)= rx(i1,i2,i3)*scr8(i1,i2,i3,kd)+sx(i1,i2,i3)
     & *scs8(i1,i2,i3,kd)
        scy82(i1,i2,i3,kd)= ry(i1,i2,i3)*scr8(i1,i2,i3,kd)+sy(i1,i2,i3)
     & *scs8(i1,i2,i3,kd)
        scz82(i1,i2,i3,kd)=0
        scx83(i1,i2,i3,kd)=rx(i1,i2,i3)*scr8(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & scs8(i1,i2,i3,kd)+tx(i1,i2,i3)*sct8(i1,i2,i3,kd)
        scy83(i1,i2,i3,kd)=ry(i1,i2,i3)*scr8(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & scs8(i1,i2,i3,kd)+ty(i1,i2,i3)*sct8(i1,i2,i3,kd)
        scz83(i1,i2,i3,kd)=rz(i1,i2,i3)*scr8(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & scs8(i1,i2,i3,kd)+tz(i1,i2,i3)*sct8(i1,i2,i3,kd)
! #If "" == "RX"
        scxx81(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*scrr8(i1,i2,i3,kd)+(
     & rxx82(i1,i2,i3))*scr8(i1,i2,i3,kd)
        scyy81(i1,i2,i3,kd)=0
        scxy81(i1,i2,i3,kd)=0
        scxz81(i1,i2,i3,kd)=0
        scyz81(i1,i2,i3,kd)=0
        sczz81(i1,i2,i3,kd)=0
        sclaplacian81(i1,i2,i3,kd)=scxx81(i1,i2,i3,kd)
        scxx82(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*scrr8(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*scrs8(i1,i2,i3,kd)+(sx(i1,i2,i3)**2)
     & *scss8(i1,i2,i3,kd)+(rxx82(i1,i2,i3))*scr8(i1,i2,i3,kd)+(sxx82(
     & i1,i2,i3))*scs8(i1,i2,i3,kd)
        scyy82(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*scrr8(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*scrs8(i1,i2,i3,kd)+(sy(i1,i2,i3)**2)
     & *scss8(i1,i2,i3,kd)+(ryy82(i1,i2,i3))*scr8(i1,i2,i3,kd)+(syy82(
     & i1,i2,i3))*scs8(i1,i2,i3,kd)
        scxy82(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*scrr8(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & scrs8(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*scss8(i1,i2,i3,kd)
     & +rxy82(i1,i2,i3)*scr8(i1,i2,i3,kd)+sxy82(i1,i2,i3)*scs8(i1,i2,
     & i3,kd)
        scxz82(i1,i2,i3,kd)=0
        scyz82(i1,i2,i3,kd)=0
        sczz82(i1,i2,i3,kd)=0
        sclaplacian82(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & scrr8(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*
     & sy(i1,i2,i3))*scrs8(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)*
     & *2)*scss8(i1,i2,i3,kd)+(rxx82(i1,i2,i3)+ryy82(i1,i2,i3))*scr8(
     & i1,i2,i3,kd)+(sxx82(i1,i2,i3)+syy82(i1,i2,i3))*scs8(i1,i2,i3,
     & kd)
        scxx83(i1,i2,i3,kd)=rx(i1,i2,i3)**2*scrr8(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*scss8(i1,i2,i3,kd)+tx(i1,i2,i3)**2*sctt8(i1,i2,i3,kd)
     & +2.*rx(i1,i2,i3)*sx(i1,i2,i3)*scrs8(i1,i2,i3,kd)+2.*rx(i1,i2,
     & i3)*tx(i1,i2,i3)*scrt8(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,i2,
     & i3)*scst8(i1,i2,i3,kd)+rxx83(i1,i2,i3)*scr8(i1,i2,i3,kd)+sxx83(
     & i1,i2,i3)*scs8(i1,i2,i3,kd)+txx83(i1,i2,i3)*sct8(i1,i2,i3,kd)
        scyy83(i1,i2,i3,kd)=ry(i1,i2,i3)**2*scrr8(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*scss8(i1,i2,i3,kd)+ty(i1,i2,i3)**2*sctt8(i1,i2,i3,kd)
     & +2.*ry(i1,i2,i3)*sy(i1,i2,i3)*scrs8(i1,i2,i3,kd)+2.*ry(i1,i2,
     & i3)*ty(i1,i2,i3)*scrt8(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,i2,
     & i3)*scst8(i1,i2,i3,kd)+ryy83(i1,i2,i3)*scr8(i1,i2,i3,kd)+syy83(
     & i1,i2,i3)*scs8(i1,i2,i3,kd)+tyy83(i1,i2,i3)*sct8(i1,i2,i3,kd)
        sczz83(i1,i2,i3,kd)=rz(i1,i2,i3)**2*scrr8(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*scss8(i1,i2,i3,kd)+tz(i1,i2,i3)**2*sctt8(i1,i2,i3,kd)
     & +2.*rz(i1,i2,i3)*sz(i1,i2,i3)*scrs8(i1,i2,i3,kd)+2.*rz(i1,i2,
     & i3)*tz(i1,i2,i3)*scrt8(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,i2,
     & i3)*scst8(i1,i2,i3,kd)+rzz83(i1,i2,i3)*scr8(i1,i2,i3,kd)+szz83(
     & i1,i2,i3)*scs8(i1,i2,i3,kd)+tzz83(i1,i2,i3)*sct8(i1,i2,i3,kd)
        scxy83(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*scrr8(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*scss8(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & ty(i1,i2,i3)*sctt8(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(
     & i1,i2,i3)*sx(i1,i2,i3))*scrs8(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(i1,
     & i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*scrt8(i1,i2,i3,kd)+(sx(i1,i2,
     & i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*scst8(i1,i2,i3,kd)+
     & rxy83(i1,i2,i3)*scr8(i1,i2,i3,kd)+sxy83(i1,i2,i3)*scs8(i1,i2,
     & i3,kd)+txy83(i1,i2,i3)*sct8(i1,i2,i3,kd)
        scxz83(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*scrr8(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*scss8(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & tz(i1,i2,i3)*sctt8(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sx(i1,i2,i3))*scrs8(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(i1,
     & i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*scrt8(i1,i2,i3,kd)+(sx(i1,i2,
     & i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*scst8(i1,i2,i3,kd)+
     & rxz83(i1,i2,i3)*scr8(i1,i2,i3,kd)+sxz83(i1,i2,i3)*scs8(i1,i2,
     & i3,kd)+txz83(i1,i2,i3)*sct8(i1,i2,i3,kd)
        scyz83(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*scrr8(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*scss8(i1,i2,i3,kd)+ty(i1,i2,i3)*
     & tz(i1,i2,i3)*sctt8(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sy(i1,i2,i3))*scrs8(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(i1,
     & i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*scrt8(i1,i2,i3,kd)+(sy(i1,i2,
     & i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*scst8(i1,i2,i3,kd)+
     & ryz83(i1,i2,i3)*scr8(i1,i2,i3,kd)+syz83(i1,i2,i3)*scs8(i1,i2,
     & i3,kd)+tyz83(i1,i2,i3)*sct8(i1,i2,i3,kd)
        sclaplacian83(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*scrr8(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)*
     & *2+sz(i1,i2,i3)**2)*scss8(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(i1,
     & i2,i3)**2+tz(i1,i2,i3)**2)*sctt8(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*
     & sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,
     & i3))*scrs8(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(i1,
     & i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*scrt8(i1,i2,i3,
     & kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)+
     & sz(i1,i2,i3)*tz(i1,i2,i3))*scst8(i1,i2,i3,kd)+(rxx83(i1,i2,i3)+
     & ryy83(i1,i2,i3)+rzz83(i1,i2,i3))*scr8(i1,i2,i3,kd)+(sxx83(i1,
     & i2,i3)+syy83(i1,i2,i3)+szz83(i1,i2,i3))*scs8(i1,i2,i3,kd)+(
     & txx83(i1,i2,i3)+tyy83(i1,i2,i3)+tzz83(i1,i2,i3))*sct8(i1,i2,i3,
     & kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
! #If "" == "RX"
        scx83r(i1,i2,i3,kd)=(672.*(sc(i1+1,i2,i3,kd)-sc(i1-1,i2,i3,kd))
     & -168.*(sc(i1+2,i2,i3,kd)-sc(i1-2,i2,i3,kd))+32.*(sc(i1+3,i2,i3,
     & kd)-sc(i1-3,i2,i3,kd))-3.*(sc(i1+4,i2,i3,kd)-sc(i1-4,i2,i3,kd))
     & )*h18(0)
        scy83r(i1,i2,i3,kd)=(672.*(sc(i1,i2+1,i3,kd)-sc(i1,i2-1,i3,kd))
     & -168.*(sc(i1,i2+2,i3,kd)-sc(i1,i2-2,i3,kd))+32.*(sc(i1,i2+3,i3,
     & kd)-sc(i1,i2-3,i3,kd))-3.*(sc(i1,i2+4,i3,kd)-sc(i1,i2-4,i3,kd))
     & )*h18(1)
        scz83r(i1,i2,i3,kd)=(672.*(sc(i1,i2,i3+1,kd)-sc(i1,i2,i3-1,kd))
     & -168.*(sc(i1,i2,i3+2,kd)-sc(i1,i2,i3-2,kd))+32.*(sc(i1,i2,i3+3,
     & kd)-sc(i1,i2,i3-3,kd))-3.*(sc(i1,i2,i3+4,kd)-sc(i1,i2,i3-4,kd))
     & )*h18(2)
        scxx83r(i1,i2,i3,kd)=(-14350.*sc(i1,i2,i3,kd)+8064.*(sc(i1+1,
     & i2,i3,kd)+sc(i1-1,i2,i3,kd))-1008.*(sc(i1+2,i2,i3,kd)+sc(i1-2,
     & i2,i3,kd))+128.*(sc(i1+3,i2,i3,kd)+sc(i1-3,i2,i3,kd))-9.*(sc(
     & i1+4,i2,i3,kd)+sc(i1-4,i2,i3,kd)) )*h28(0)
        scyy83r(i1,i2,i3,kd)=(-14350.*sc(i1,i2,i3,kd)+8064.*(sc(i1,i2+
     & 1,i3,kd)+sc(i1,i2-1,i3,kd))-1008.*(sc(i1,i2+2,i3,kd)+sc(i1,i2-
     & 2,i3,kd))+128.*(sc(i1,i2+3,i3,kd)+sc(i1,i2-3,i3,kd))-9.*(sc(i1,
     & i2+4,i3,kd)+sc(i1,i2-4,i3,kd)) )*h28(1)
        sczz83r(i1,i2,i3,kd)=(-14350.*sc(i1,i2,i3,kd)+8064.*(sc(i1,i2,
     & i3+1,kd)+sc(i1,i2,i3-1,kd))-1008.*(sc(i1,i2,i3+2,kd)+sc(i1,i2,
     & i3-2,kd))+128.*(sc(i1,i2,i3+3,kd)+sc(i1,i2,i3-3,kd))-9.*(sc(i1,
     & i2,i3+4,kd)+sc(i1,i2,i3-4,kd)) )*h28(2)
        scxy83r(i1,i2,i3,kd)=(672.*(scx83r(i1,i2+1,i3,kd)-scx83r(i1,i2-
     & 1,i3,kd))-168.*(scx83r(i1,i2+2,i3,kd)-scx83r(i1,i2-2,i3,kd))+
     & 32.*(scx83r(i1,i2+3,i3,kd)-scx83r(i1,i2-3,i3,kd))-3.*(scx83r(
     & i1,i2+4,i3,kd)-scx83r(i1,i2-4,i3,kd)))*h18(1)
        scxz83r(i1,i2,i3,kd)=(672.*(scx83r(i1,i2,i3+1,kd)-scx83r(i1,i2,
     & i3-1,kd))-168.*(scx83r(i1,i2,i3+2,kd)-scx83r(i1,i2,i3-2,kd))+
     & 32.*(scx83r(i1,i2,i3+3,kd)-scx83r(i1,i2,i3-3,kd))-3.*(scx83r(
     & i1,i2,i3+4,kd)-scx83r(i1,i2,i3-4,kd)))*h18(2)
        scyz83r(i1,i2,i3,kd)=(672.*(scy83r(i1,i2,i3+1,kd)-scy83r(i1,i2,
     & i3-1,kd))-168.*(scy83r(i1,i2,i3+2,kd)-scy83r(i1,i2,i3-2,kd))+
     & 32.*(scy83r(i1,i2,i3+3,kd)-scy83r(i1,i2,i3-3,kd))-3.*(scy83r(
     & i1,i2,i3+4,kd)-scy83r(i1,i2,i3-4,kd)))*h18(2)
        scx81r(i1,i2,i3,kd)= scx83r(i1,i2,i3,kd)
        scy81r(i1,i2,i3,kd)= scy83r(i1,i2,i3,kd)
        scz81r(i1,i2,i3,kd)= scz83r(i1,i2,i3,kd)
        scxx81r(i1,i2,i3,kd)= scxx83r(i1,i2,i3,kd)
        scyy81r(i1,i2,i3,kd)= scyy83r(i1,i2,i3,kd)
        sczz81r(i1,i2,i3,kd)= sczz83r(i1,i2,i3,kd)
        scxy81r(i1,i2,i3,kd)= scxy83r(i1,i2,i3,kd)
        scxz81r(i1,i2,i3,kd)= scxz83r(i1,i2,i3,kd)
        scyz81r(i1,i2,i3,kd)= scyz83r(i1,i2,i3,kd)
        sclaplacian81r(i1,i2,i3,kd)=scxx83r(i1,i2,i3,kd)
        scx82r(i1,i2,i3,kd)= scx83r(i1,i2,i3,kd)
        scy82r(i1,i2,i3,kd)= scy83r(i1,i2,i3,kd)
        scz82r(i1,i2,i3,kd)= scz83r(i1,i2,i3,kd)
        scxx82r(i1,i2,i3,kd)= scxx83r(i1,i2,i3,kd)
        scyy82r(i1,i2,i3,kd)= scyy83r(i1,i2,i3,kd)
        sczz82r(i1,i2,i3,kd)= sczz83r(i1,i2,i3,kd)
        scxy82r(i1,i2,i3,kd)= scxy83r(i1,i2,i3,kd)
        scxz82r(i1,i2,i3,kd)= scxz83r(i1,i2,i3,kd)
        scyz82r(i1,i2,i3,kd)= scyz83r(i1,i2,i3,kd)
        sclaplacian82r(i1,i2,i3,kd)=scxx83r(i1,i2,i3,kd)+scyy83r(i1,i2,
     & i3,kd)
        sclaplacian83r(i1,i2,i3,kd)=scxx83r(i1,i2,i3,kd)+scyy83r(i1,i2,
     & i3,kd)+sczz83r(i1,i2,i3,kd)
c......end statement functions
       kd3=nd
! #If "divScalarGradNC" == "divScalarGradNC"
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
! loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN22R(i1,i2,i3,c)+SCX22R(i1,i2,i3,0)*UX22R(i1,i2,i3,c)+SCY22R(i1,i2,i3,0)*UY22R(i1,i2,i3,c))
             do c=ca,cb
               do i3=n3a,n3b
                 do i2=n2a,n2b
                   do i1=n1a,n1b
                     deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN22R(i1,
     & i2,i3,c)+SCX22R(i1,i2,i3,0)*UX22R(i1,i2,i3,c)+SCY22R(i1,i2,i3,
     & 0)*UY22R(i1,i2,i3,c)
                   end do
                 end do
               end do
             end do
           else if( order.eq.4 )then
! loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN42R(i1,i2,i3,c) +SCX42R(i1,i2,i3,0)*UX42R(i1,i2,i3,c)+SCY42R(i1,i2,i3,0)*UY42R(i1,i2,i3,c))
             do c=ca,cb
               do i3=n3a,n3b
                 do i2=n2a,n2b
                   do i1=n1a,n1b
                     deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN42R(i1,
     & i2,i3,c)+SCX42R(i1,i2,i3,0)*UX42R(i1,i2,i3,c)+SCY42R(i1,i2,i3,
     & 0)*UY42R(i1,i2,i3,c)
                   end do
                 end do
               end do
             end do
           else if( order.eq.6 )then
! loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN62R(i1,i2,i3,c) +SCX62R(i1,i2,i3,0)*UX62R(i1,i2,i3,c)+SCY62R(i1,i2,i3,0)*UY62R(i1,i2,i3,c))
             do c=ca,cb
               do i3=n3a,n3b
                 do i2=n2a,n2b
                   do i1=n1a,n1b
                     deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN62R(i1,
     & i2,i3,c)+SCX62R(i1,i2,i3,0)*UX62R(i1,i2,i3,c)+SCY62R(i1,i2,i3,
     & 0)*UY62R(i1,i2,i3,c)
                   end do
                 end do
               end do
             end do
           else if( order.eq.8 )then
! loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN82R(i1,i2,i3,c) +SCX82R(i1,i2,i3,0)*UX82R(i1,i2,i3,c)+SCY82R(i1,i2,i3,0)*UY82R(i1,i2,i3,c))
             do c=ca,cb
               do i3=n3a,n3b
                 do i2=n2a,n2b
                   do i1=n1a,n1b
                     deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN82R(i1,
     & i2,i3,c)+SCX82R(i1,i2,i3,0)*UX82R(i1,i2,i3,c)+SCY82R(i1,i2,i3,
     & 0)*UY82R(i1,i2,i3,c)
                   end do
                 end do
               end do
             end do
           else
             write(*,*) 'ERROR:divScalarGradNC:order=',order
             stop 43
           end if
         else
c           ***** not rectangular *****
           if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN22(i1,i2,i3,c)+SCX22(i1,i2,i3,0)*UX22(i1,i2,i3,c)+SCY22(i1,i2,i3,0)*UY22(i1,i2,i3,c))
             do c=ca,cb
               do i3=n3a,n3b
                 do i2=n2a,n2b
                   do i1=n1a,n1b
                     deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN22(i1,
     & i2,i3,c)+SCX22(i1,i2,i3,0)*UX22(i1,i2,i3,c)+SCY22(i1,i2,i3,0)*
     & UY22(i1,i2,i3,c)
                   end do
                 end do
               end do
             end do
           else if( order.eq.4 )then
! loopsDSG(deriv(i1,i2,i3,c)= sc(i1,i2,i3,0)*ULAPLACIAN42(i1,i2,i3,c)+SCX42(i1,i2,i3,0)*UX42(i1,i2,i3,c)+SCY42(i1,i2,i3,0)*UY42(i1,i2,i3,c))
             do c=ca,cb
               do i3=n3a,n3b
                 do i2=n2a,n2b
                   do i1=n1a,n1b
                     deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN42(i1,
     & i2,i3,c)+SCX42(i1,i2,i3,0)*UX42(i1,i2,i3,c)+SCY42(i1,i2,i3,0)*
     & UY42(i1,i2,i3,c)
                   end do
                 end do
               end do
             end do
           else if( order.eq.6 )then
! loopsDSG(deriv(i1,i2,i3,c)= sc(i1,i2,i3,0)*ULAPLACIAN62(i1,i2,i3,c)+SCX62(i1,i2,i3,0)*UX62(i1,i2,i3,c)+SCY62(i1,i2,i3,0)*UY62(i1,i2,i3,c))
             do c=ca,cb
               do i3=n3a,n3b
                 do i2=n2a,n2b
                   do i1=n1a,n1b
                     deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN62(i1,
     & i2,i3,c)+SCX62(i1,i2,i3,0)*UX62(i1,i2,i3,c)+SCY62(i1,i2,i3,0)*
     & UY62(i1,i2,i3,c)
                   end do
                 end do
               end do
             end do
           else if( order.eq.8 )then
! loopsDSG(deriv(i1,i2,i3,c)= sc(i1,i2,i3,0)*ULAPLACIAN82(i1,i2,i3,c)+SCX82(i1,i2,i3,0)*UX82(i1,i2,i3,c)+SCY82(i1,i2,i3,0)*UY82(i1,i2,i3,c))
             do c=ca,cb
               do i3=n3a,n3b
                 do i2=n2a,n2b
                   do i1=n1a,n1b
                     deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN82(i1,
     & i2,i3,c)+SCX82(i1,i2,i3,0)*UX82(i1,i2,i3,c)+SCY82(i1,i2,i3,0)*
     & UY82(i1,i2,i3,c)
                   end do
                 end do
               end do
             end do
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
! loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN23R(i1,i2,i3,c)+SCX22R(i1,i2,i3,0)*UX23R(i1,i2,i3,c)+SCY22R(i1,i2,i3,0)*UY23R(i1,i2,i3,c)+SCZ22R(i1,i2,i3,0)*UZ23R(i1,i2,i3,c))
             do c=ca,cb
               do i3=n3a,n3b
                 do i2=n2a,n2b
                   do i1=n1a,n1b
                     deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN23R(i1,
     & i2,i3,c)+SCX22R(i1,i2,i3,0)*UX23R(i1,i2,i3,c)+SCY22R(i1,i2,i3,
     & 0)*UY23R(i1,i2,i3,c)+SCZ22R(i1,i2,i3,0)*UZ23R(i1,i2,i3,c)
                   end do
                 end do
               end do
             end do
           else if( order.eq.4 )then
! loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN43R(i1,i2,i3,c)+SCX42R(i1,i2,i3,0)*UX42R(i1,i2,i3,c)+SCY42R(i1,i2,i3,0)*UY42R(i1,i2,i3,c)+SCZ42R(i1,i2,i3,0)*UZ42R(i1,i2,i3,c))
             do c=ca,cb
               do i3=n3a,n3b
                 do i2=n2a,n2b
                   do i1=n1a,n1b
                     deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN43R(i1,
     & i2,i3,c)+SCX42R(i1,i2,i3,0)*UX42R(i1,i2,i3,c)+SCY42R(i1,i2,i3,
     & 0)*UY42R(i1,i2,i3,c)+SCZ42R(i1,i2,i3,0)*UZ42R(i1,i2,i3,c)
                   end do
                 end do
               end do
             end do
           else if( order.eq.6 )then
! loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN63R(i1,i2,i3,c)+SCX62R(i1,i2,i3,0)*UX62R(i1,i2,i3,c)+SCY62R(i1,i2,i3,0)*UY62R(i1,i2,i3,c)+SCZ62R(i1,i2,i3,0)*UZ62R(i1,i2,i3,c))
             do c=ca,cb
               do i3=n3a,n3b
                 do i2=n2a,n2b
                   do i1=n1a,n1b
                     deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN63R(i1,
     & i2,i3,c)+SCX62R(i1,i2,i3,0)*UX62R(i1,i2,i3,c)+SCY62R(i1,i2,i3,
     & 0)*UY62R(i1,i2,i3,c)+SCZ62R(i1,i2,i3,0)*UZ62R(i1,i2,i3,c)
                   end do
                 end do
               end do
             end do
           else if( order.eq.8 )then
! loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN83R(i1,i2,i3,c)+SCX82R(i1,i2,i3,0)*UX82R(i1,i2,i3,c)+SCY82R(i1,i2,i3,0)*UY82R(i1,i2,i3,c)+SCZ82R(i1,i2,i3,0)*UZ82R(i1,i2,i3,c))
             do c=ca,cb
               do i3=n3a,n3b
                 do i2=n2a,n2b
                   do i1=n1a,n1b
                     deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN83R(i1,
     & i2,i3,c)+SCX82R(i1,i2,i3,0)*UX82R(i1,i2,i3,c)+SCY82R(i1,i2,i3,
     & 0)*UY82R(i1,i2,i3,c)+SCZ82R(i1,i2,i3,0)*UZ82R(i1,i2,i3,c)
                   end do
                 end do
               end do
             end do
           end if
         else
c           ***** not rectangular *****
           if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN23(i1,i2,i3,c)+SCX23(i1,i2,i3,0)*UX23(i1,i2,i3,c)+SCY23(i1,i2,i3,0)*UY23(i1,i2,i3,c)+SCZ23(i1,i2,i3,0)*UZ23(i1,i2,i3,c))
             do c=ca,cb
               do i3=n3a,n3b
                 do i2=n2a,n2b
                   do i1=n1a,n1b
                     deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN23(i1,
     & i2,i3,c)+SCX23(i1,i2,i3,0)*UX23(i1,i2,i3,c)+SCY23(i1,i2,i3,0)*
     & UY23(i1,i2,i3,c)+SCZ23(i1,i2,i3,0)*UZ23(i1,i2,i3,c)
                   end do
                 end do
               end do
             end do
           else if( order.eq.4 )then
! loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN43(i1,i2,i3,c)+SCX43(i1,i2,i3,0)*UX43(i1,i2,i3,c)+SCY43(i1,i2,i3,0)*UY43(i1,i2,i3,c)+SCZ43(i1,i2,i3,0)*UZ43(i1,i2,i3,c))
             do c=ca,cb
               do i3=n3a,n3b
                 do i2=n2a,n2b
                   do i1=n1a,n1b
                     deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN43(i1,
     & i2,i3,c)+SCX43(i1,i2,i3,0)*UX43(i1,i2,i3,c)+SCY43(i1,i2,i3,0)*
     & UY43(i1,i2,i3,c)+SCZ43(i1,i2,i3,0)*UZ43(i1,i2,i3,c)
                   end do
                 end do
               end do
             end do
           else if( order.eq.6 )then
! loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN63(i1,i2,i3,c)+SCX63(i1,i2,i3,0)*UX63(i1,i2,i3,c)+SCY63(i1,i2,i3,0)*UY63(i1,i2,i3,c)+SCZ63(i1,i2,i3,0)*UZ63(i1,i2,i3,c))
             do c=ca,cb
               do i3=n3a,n3b
                 do i2=n2a,n2b
                   do i1=n1a,n1b
                     deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN63(i1,
     & i2,i3,c)+SCX63(i1,i2,i3,0)*UX63(i1,i2,i3,c)+SCY63(i1,i2,i3,0)*
     & UY63(i1,i2,i3,c)+SCZ63(i1,i2,i3,0)*UZ63(i1,i2,i3,c)
                   end do
                 end do
               end do
             end do
           else if( order.eq.8 )then
! loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN83(i1,i2,i3,c)+SCX83(i1,i2,i3,0)*UX83(i1,i2,i3,c)+SCY83(i1,i2,i3,0)*UY83(i1,i2,i3,c)+SCZ83(i1,i2,i3,0)*UZ83(i1,i2,i3,c))
             do c=ca,cb
               do i3=n3a,n3b
                 do i2=n2a,n2b
                   do i1=n1a,n1b
                     deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN83(i1,
     & i2,i3,c)+SCX83(i1,i2,i3,0)*UX83(i1,i2,i3,c)+SCY83(i1,i2,i3,0)*
     & UY83(i1,i2,i3,c)+SCZ83(i1,i2,i3,0)*UZ83(i1,i2,i3,c)
                   end do
                 end do
               end do
             end do
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
! loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN21R(i1,i2,i3,c)+SCX22R(i1,i2,i3,0)*UX22R(i1,i2,i3,c))
             do c=ca,cb
               do i3=n3a,n3b
                 do i2=n2a,n2b
                   do i1=n1a,n1b
                     deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN21R(i1,
     & i2,i3,c)+SCX22R(i1,i2,i3,0)*UX22R(i1,i2,i3,c)
                   end do
                 end do
               end do
             end do
           else if( order.eq.4 )then
! loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN41R(i1,i2,i3,c)+SCX42R(i1,i2,i3,0)*UX42R(i1,i2,i3,c))
             do c=ca,cb
               do i3=n3a,n3b
                 do i2=n2a,n2b
                   do i1=n1a,n1b
                     deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN41R(i1,
     & i2,i3,c)+SCX42R(i1,i2,i3,0)*UX42R(i1,i2,i3,c)
                   end do
                 end do
               end do
             end do
           else if( order.eq.6 )then
! loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN61R(i1,i2,i3,c)+SCX62R(i1,i2,i3,0)*UX62R(i1,i2,i3,c))
             do c=ca,cb
               do i3=n3a,n3b
                 do i2=n2a,n2b
                   do i1=n1a,n1b
                     deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN61R(i1,
     & i2,i3,c)+SCX62R(i1,i2,i3,0)*UX62R(i1,i2,i3,c)
                   end do
                 end do
               end do
             end do
           else if( order.eq.8 )then
! loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN81R(i1,i2,i3,c)+SCX82R(i1,i2,i3,0)*UX82R(i1,i2,i3,c))
             do c=ca,cb
               do i3=n3a,n3b
                 do i2=n2a,n2b
                   do i1=n1a,n1b
                     deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN81R(i1,
     & i2,i3,c)+SCX82R(i1,i2,i3,0)*UX82R(i1,i2,i3,c)
                   end do
                 end do
               end do
             end do
           else
             write(*,*) 'ERROR:divScalarGradNC:order=',order
             stop 43
           end if
         else
c            ***** not rectangular *****
           if( order.eq.2 )then
! loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN21(i1,i2,i3,c)+SCX21(i1,i2,i3,0)*UX21(i1,i2,i3,c))
             do c=ca,cb
               do i3=n3a,n3b
                 do i2=n2a,n2b
                   do i1=n1a,n1b
                     deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN21(i1,
     & i2,i3,c)+SCX21(i1,i2,i3,0)*UX21(i1,i2,i3,c)
                   end do
                 end do
               end do
             end do
           else if( order.eq.4 )then
! loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN41(i1,i2,i3,c)+SCX41(i1,i2,i3,0)*UX41(i1,i2,i3,c))
             do c=ca,cb
               do i3=n3a,n3b
                 do i2=n2a,n2b
                   do i1=n1a,n1b
                     deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN41(i1,
     & i2,i3,c)+SCX41(i1,i2,i3,0)*UX41(i1,i2,i3,c)
                   end do
                 end do
               end do
             end do
           else if( order.eq.6 )then
! loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN61(i1,i2,i3,c)+SCX61(i1,i2,i3,0)*UX61(i1,i2,i3,c))
             do c=ca,cb
               do i3=n3a,n3b
                 do i2=n2a,n2b
                   do i1=n1a,n1b
                     deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN61(i1,
     & i2,i3,c)+SCX61(i1,i2,i3,0)*UX61(i1,i2,i3,c)
                   end do
                 end do
               end do
             end do
           else if( order.eq.8 )then
! loopsDSG(deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN81(i1,i2,i3,c)+SCX81(i1,i2,i3,0)*UX81(i1,i2,i3,c))
             do c=ca,cb
               do i3=n3a,n3b
                 do i2=n2a,n2b
                   do i1=n1a,n1b
                     deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*ULAPLACIAN81(i1,
     & i2,i3,c)+SCX81(i1,i2,i3,0)*UX81(i1,i2,i3,c)
                   end do
                 end do
               end do
             end do
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
! divTensorGrad2d(22R)
             do c=ca,cb
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
              uxc = UX22R(i1,i2,i3,c)
              uyc = UY22R(i1,i2,i3,c)
              uxxc= UXX22R(i1,i2,i3,c)
              uxyc= UXY22R(i1,i2,i3,c)
              uyyc= UYY22R(i1,i2,i3,c)
              deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*uxxc +SCX22R(i1,i2,i3,0)
     & *uxc+ sc(i1,i2,i3,1)*uxyc +SCY22R(i1,i2,i3,1)*uxc+ sc(i1,i2,i3,
     & 2)*uxyc +SCX22R(i1,i2,i3,2)*uyc+ sc(i1,i2,i3,3)*uyyc +SCY22R(
     & i1,i2,i3,3)*uyc
             end do
             end do
             end do
             end do
           else if( order.eq.4 )then
! divTensorGrad2d(42R)
             do c=ca,cb
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
              uxc = UX42R(i1,i2,i3,c)
              uyc = UY42R(i1,i2,i3,c)
              uxxc= UXX42R(i1,i2,i3,c)
              uxyc= UXY42R(i1,i2,i3,c)
              uyyc= UYY42R(i1,i2,i3,c)
              deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*uxxc +SCX42R(i1,i2,i3,0)
     & *uxc+ sc(i1,i2,i3,1)*uxyc +SCY42R(i1,i2,i3,1)*uxc+ sc(i1,i2,i3,
     & 2)*uxyc +SCX42R(i1,i2,i3,2)*uyc+ sc(i1,i2,i3,3)*uyyc +SCY42R(
     & i1,i2,i3,3)*uyc
             end do
             end do
             end do
             end do
           else if( order.eq.6 )then
! divTensorGrad2d(62R)
             do c=ca,cb
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
              uxc = UX62R(i1,i2,i3,c)
              uyc = UY62R(i1,i2,i3,c)
              uxxc= UXX62R(i1,i2,i3,c)
              uxyc= UXY62R(i1,i2,i3,c)
              uyyc= UYY62R(i1,i2,i3,c)
              deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*uxxc +SCX62R(i1,i2,i3,0)
     & *uxc+ sc(i1,i2,i3,1)*uxyc +SCY62R(i1,i2,i3,1)*uxc+ sc(i1,i2,i3,
     & 2)*uxyc +SCX62R(i1,i2,i3,2)*uyc+ sc(i1,i2,i3,3)*uyyc +SCY62R(
     & i1,i2,i3,3)*uyc
             end do
             end do
             end do
             end do
           else if( order.eq.8 )then
! divTensorGrad2d(82R)
             do c=ca,cb
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
              uxc = UX82R(i1,i2,i3,c)
              uyc = UY82R(i1,i2,i3,c)
              uxxc= UXX82R(i1,i2,i3,c)
              uxyc= UXY82R(i1,i2,i3,c)
              uyyc= UYY82R(i1,i2,i3,c)
              deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*uxxc +SCX82R(i1,i2,i3,0)
     & *uxc+ sc(i1,i2,i3,1)*uxyc +SCY82R(i1,i2,i3,1)*uxc+ sc(i1,i2,i3,
     & 2)*uxyc +SCX82R(i1,i2,i3,2)*uyc+ sc(i1,i2,i3,3)*uyyc +SCY82R(
     & i1,i2,i3,3)*uyc
             end do
             end do
             end do
             end do
           else
             write(*,*) 'ERROR:divTensorGradNC:order=',order
             stop 43
           end if
         else
c           ***** not rectangular *****
           if( order.eq.2 )then
! divTensorGrad2d(22)
             do c=ca,cb
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
              uxc = UX22(i1,i2,i3,c)
              uyc = UY22(i1,i2,i3,c)
              uxxc= UXX22(i1,i2,i3,c)
              uxyc= UXY22(i1,i2,i3,c)
              uyyc= UYY22(i1,i2,i3,c)
              deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*uxxc +SCX22(i1,i2,i3,0)*
     & uxc+ sc(i1,i2,i3,1)*uxyc +SCY22(i1,i2,i3,1)*uxc+ sc(i1,i2,i3,2)
     & *uxyc +SCX22(i1,i2,i3,2)*uyc+ sc(i1,i2,i3,3)*uyyc +SCY22(i1,i2,
     & i3,3)*uyc
             end do
             end do
             end do
             end do
           else if( order.eq.4 )then
! divTensorGrad2d(42)
             do c=ca,cb
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
              uxc = UX42(i1,i2,i3,c)
              uyc = UY42(i1,i2,i3,c)
              uxxc= UXX42(i1,i2,i3,c)
              uxyc= UXY42(i1,i2,i3,c)
              uyyc= UYY42(i1,i2,i3,c)
              deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*uxxc +SCX42(i1,i2,i3,0)*
     & uxc+ sc(i1,i2,i3,1)*uxyc +SCY42(i1,i2,i3,1)*uxc+ sc(i1,i2,i3,2)
     & *uxyc +SCX42(i1,i2,i3,2)*uyc+ sc(i1,i2,i3,3)*uyyc +SCY42(i1,i2,
     & i3,3)*uyc
             end do
             end do
             end do
             end do
           else if( order.eq.6 )then
! divTensorGrad2d(62)
             do c=ca,cb
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
              uxc = UX62(i1,i2,i3,c)
              uyc = UY62(i1,i2,i3,c)
              uxxc= UXX62(i1,i2,i3,c)
              uxyc= UXY62(i1,i2,i3,c)
              uyyc= UYY62(i1,i2,i3,c)
              deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*uxxc +SCX62(i1,i2,i3,0)*
     & uxc+ sc(i1,i2,i3,1)*uxyc +SCY62(i1,i2,i3,1)*uxc+ sc(i1,i2,i3,2)
     & *uxyc +SCX62(i1,i2,i3,2)*uyc+ sc(i1,i2,i3,3)*uyyc +SCY62(i1,i2,
     & i3,3)*uyc
             end do
             end do
             end do
             end do
           else if( order.eq.8 )then
! divTensorGrad2d(82)
             do c=ca,cb
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
              uxc = UX82(i1,i2,i3,c)
              uyc = UY82(i1,i2,i3,c)
              uxxc= UXX82(i1,i2,i3,c)
              uxyc= UXY82(i1,i2,i3,c)
              uyyc= UYY82(i1,i2,i3,c)
              deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*uxxc +SCX82(i1,i2,i3,0)*
     & uxc+ sc(i1,i2,i3,1)*uxyc +SCY82(i1,i2,i3,1)*uxc+ sc(i1,i2,i3,2)
     & *uxyc +SCX82(i1,i2,i3,2)*uyc+ sc(i1,i2,i3,3)*uyyc +SCY82(i1,i2,
     & i3,3)*uyc
             end do
             end do
             end do
             end do
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
! divTensorGrad3d(23R)
             do c=ca,cb
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
              uxc = UX23R(i1,i2,i3,c)
              uyc = UY23R(i1,i2,i3,c)
              uzc = UZ23R(i1,i2,i3,c)
              uxxc= UXX23R(i1,i2,i3,c)
              uxyc= UXY23R(i1,i2,i3,c)
              uxzc= UXZ23R(i1,i2,i3,c)
              uyxc=uxyc
              uyyc= UYY23R(i1,i2,i3,c)
              uyzc= UYZ23R(i1,i2,i3,c)
              uzxc=uxzc
              uzyc=uyzc
              uzzc= UZZ23R(i1,i2,i3,c)
              deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*uxxc +SCX23R(i1,i2,i3,0)
     & *uxc+ sc(i1,i2,i3,1)*uyxc +SCY23R(i1,i2,i3,1)*uxc+ sc(i1,i2,i3,
     & 2)*uzxc +SCZ23R(i1,i2,i3,2)*uxc+ sc(i1,i2,i3,3)*uxyc +SCX23R(
     & i1,i2,i3,3)*uyc+ sc(i1,i2,i3,4)*uyyc +SCY23R(i1,i2,i3,4)*uyc+ 
     & sc(i1,i2,i3,5)*uzyc +SCZ23R(i1,i2,i3,5)*uyc+ sc(i1,i2,i3,6)*
     & uxzc +SCX23R(i1,i2,i3,6)*uzc+ sc(i1,i2,i3,7)*uyzc +SCY23R(i1,
     & i2,i3,7)*uzc+ sc(i1,i2,i3,8)*uzzc +SCZ23R(i1,i2,i3,8)*uzc
             end do
             end do
             end do
             end do
           else if( order.eq.4 )then
! divTensorGrad3d(43R)
             do c=ca,cb
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
              uxc = UX43R(i1,i2,i3,c)
              uyc = UY43R(i1,i2,i3,c)
              uzc = UZ43R(i1,i2,i3,c)
              uxxc= UXX43R(i1,i2,i3,c)
              uxyc= UXY43R(i1,i2,i3,c)
              uxzc= UXZ43R(i1,i2,i3,c)
              uyxc=uxyc
              uyyc= UYY43R(i1,i2,i3,c)
              uyzc= UYZ43R(i1,i2,i3,c)
              uzxc=uxzc
              uzyc=uyzc
              uzzc= UZZ43R(i1,i2,i3,c)
              deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*uxxc +SCX43R(i1,i2,i3,0)
     & *uxc+ sc(i1,i2,i3,1)*uyxc +SCY43R(i1,i2,i3,1)*uxc+ sc(i1,i2,i3,
     & 2)*uzxc +SCZ43R(i1,i2,i3,2)*uxc+ sc(i1,i2,i3,3)*uxyc +SCX43R(
     & i1,i2,i3,3)*uyc+ sc(i1,i2,i3,4)*uyyc +SCY43R(i1,i2,i3,4)*uyc+ 
     & sc(i1,i2,i3,5)*uzyc +SCZ43R(i1,i2,i3,5)*uyc+ sc(i1,i2,i3,6)*
     & uxzc +SCX43R(i1,i2,i3,6)*uzc+ sc(i1,i2,i3,7)*uyzc +SCY43R(i1,
     & i2,i3,7)*uzc+ sc(i1,i2,i3,8)*uzzc +SCZ43R(i1,i2,i3,8)*uzc
             end do
             end do
             end do
             end do
           else if( order.eq.6 )then
! divTensorGrad3d(63R)
             do c=ca,cb
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
              uxc = UX63R(i1,i2,i3,c)
              uyc = UY63R(i1,i2,i3,c)
              uzc = UZ63R(i1,i2,i3,c)
              uxxc= UXX63R(i1,i2,i3,c)
              uxyc= UXY63R(i1,i2,i3,c)
              uxzc= UXZ63R(i1,i2,i3,c)
              uyxc=uxyc
              uyyc= UYY63R(i1,i2,i3,c)
              uyzc= UYZ63R(i1,i2,i3,c)
              uzxc=uxzc
              uzyc=uyzc
              uzzc= UZZ63R(i1,i2,i3,c)
              deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*uxxc +SCX63R(i1,i2,i3,0)
     & *uxc+ sc(i1,i2,i3,1)*uyxc +SCY63R(i1,i2,i3,1)*uxc+ sc(i1,i2,i3,
     & 2)*uzxc +SCZ63R(i1,i2,i3,2)*uxc+ sc(i1,i2,i3,3)*uxyc +SCX63R(
     & i1,i2,i3,3)*uyc+ sc(i1,i2,i3,4)*uyyc +SCY63R(i1,i2,i3,4)*uyc+ 
     & sc(i1,i2,i3,5)*uzyc +SCZ63R(i1,i2,i3,5)*uyc+ sc(i1,i2,i3,6)*
     & uxzc +SCX63R(i1,i2,i3,6)*uzc+ sc(i1,i2,i3,7)*uyzc +SCY63R(i1,
     & i2,i3,7)*uzc+ sc(i1,i2,i3,8)*uzzc +SCZ63R(i1,i2,i3,8)*uzc
             end do
             end do
             end do
             end do
           else if( order.eq.8 )then
! divTensorGrad3d(83R)
             do c=ca,cb
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
              uxc = UX83R(i1,i2,i3,c)
              uyc = UY83R(i1,i2,i3,c)
              uzc = UZ83R(i1,i2,i3,c)
              uxxc= UXX83R(i1,i2,i3,c)
              uxyc= UXY83R(i1,i2,i3,c)
              uxzc= UXZ83R(i1,i2,i3,c)
              uyxc=uxyc
              uyyc= UYY83R(i1,i2,i3,c)
              uyzc= UYZ83R(i1,i2,i3,c)
              uzxc=uxzc
              uzyc=uyzc
              uzzc= UZZ83R(i1,i2,i3,c)
              deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*uxxc +SCX83R(i1,i2,i3,0)
     & *uxc+ sc(i1,i2,i3,1)*uyxc +SCY83R(i1,i2,i3,1)*uxc+ sc(i1,i2,i3,
     & 2)*uzxc +SCZ83R(i1,i2,i3,2)*uxc+ sc(i1,i2,i3,3)*uxyc +SCX83R(
     & i1,i2,i3,3)*uyc+ sc(i1,i2,i3,4)*uyyc +SCY83R(i1,i2,i3,4)*uyc+ 
     & sc(i1,i2,i3,5)*uzyc +SCZ83R(i1,i2,i3,5)*uyc+ sc(i1,i2,i3,6)*
     & uxzc +SCX83R(i1,i2,i3,6)*uzc+ sc(i1,i2,i3,7)*uyzc +SCY83R(i1,
     & i2,i3,7)*uzc+ sc(i1,i2,i3,8)*uzzc +SCZ83R(i1,i2,i3,8)*uzc
             end do
             end do
             end do
             end do
           else
             write(*,*) 'ERROR:divTensorGradNC:order=',order
             stop 43
           end if
         else
c           ***** not rectangular *****
           if( order.eq.2 )then
! divTensorGrad3d(23)
             do c=ca,cb
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
              uxc = UX23(i1,i2,i3,c)
              uyc = UY23(i1,i2,i3,c)
              uzc = UZ23(i1,i2,i3,c)
              uxxc= UXX23(i1,i2,i3,c)
              uxyc= UXY23(i1,i2,i3,c)
              uxzc= UXZ23(i1,i2,i3,c)
              uyxc=uxyc
              uyyc= UYY23(i1,i2,i3,c)
              uyzc= UYZ23(i1,i2,i3,c)
              uzxc=uxzc
              uzyc=uyzc
              uzzc= UZZ23(i1,i2,i3,c)
              deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*uxxc +SCX23(i1,i2,i3,0)*
     & uxc+ sc(i1,i2,i3,1)*uyxc +SCY23(i1,i2,i3,1)*uxc+ sc(i1,i2,i3,2)
     & *uzxc +SCZ23(i1,i2,i3,2)*uxc+ sc(i1,i2,i3,3)*uxyc +SCX23(i1,i2,
     & i3,3)*uyc+ sc(i1,i2,i3,4)*uyyc +SCY23(i1,i2,i3,4)*uyc+ sc(i1,
     & i2,i3,5)*uzyc +SCZ23(i1,i2,i3,5)*uyc+ sc(i1,i2,i3,6)*uxzc +
     & SCX23(i1,i2,i3,6)*uzc+ sc(i1,i2,i3,7)*uyzc +SCY23(i1,i2,i3,7)*
     & uzc+ sc(i1,i2,i3,8)*uzzc +SCZ23(i1,i2,i3,8)*uzc
             end do
             end do
             end do
             end do
           else if( order.eq.4 )then
! divTensorGrad3d(43)
             do c=ca,cb
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
              uxc = UX43(i1,i2,i3,c)
              uyc = UY43(i1,i2,i3,c)
              uzc = UZ43(i1,i2,i3,c)
              uxxc= UXX43(i1,i2,i3,c)
              uxyc= UXY43(i1,i2,i3,c)
              uxzc= UXZ43(i1,i2,i3,c)
              uyxc=uxyc
              uyyc= UYY43(i1,i2,i3,c)
              uyzc= UYZ43(i1,i2,i3,c)
              uzxc=uxzc
              uzyc=uyzc
              uzzc= UZZ43(i1,i2,i3,c)
              deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*uxxc +SCX43(i1,i2,i3,0)*
     & uxc+ sc(i1,i2,i3,1)*uyxc +SCY43(i1,i2,i3,1)*uxc+ sc(i1,i2,i3,2)
     & *uzxc +SCZ43(i1,i2,i3,2)*uxc+ sc(i1,i2,i3,3)*uxyc +SCX43(i1,i2,
     & i3,3)*uyc+ sc(i1,i2,i3,4)*uyyc +SCY43(i1,i2,i3,4)*uyc+ sc(i1,
     & i2,i3,5)*uzyc +SCZ43(i1,i2,i3,5)*uyc+ sc(i1,i2,i3,6)*uxzc +
     & SCX43(i1,i2,i3,6)*uzc+ sc(i1,i2,i3,7)*uyzc +SCY43(i1,i2,i3,7)*
     & uzc+ sc(i1,i2,i3,8)*uzzc +SCZ43(i1,i2,i3,8)*uzc
             end do
             end do
             end do
             end do
           else if( order.eq.6 )then
! divTensorGrad3d(63)
             do c=ca,cb
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
              uxc = UX63(i1,i2,i3,c)
              uyc = UY63(i1,i2,i3,c)
              uzc = UZ63(i1,i2,i3,c)
              uxxc= UXX63(i1,i2,i3,c)
              uxyc= UXY63(i1,i2,i3,c)
              uxzc= UXZ63(i1,i2,i3,c)
              uyxc=uxyc
              uyyc= UYY63(i1,i2,i3,c)
              uyzc= UYZ63(i1,i2,i3,c)
              uzxc=uxzc
              uzyc=uyzc
              uzzc= UZZ63(i1,i2,i3,c)
              deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*uxxc +SCX63(i1,i2,i3,0)*
     & uxc+ sc(i1,i2,i3,1)*uyxc +SCY63(i1,i2,i3,1)*uxc+ sc(i1,i2,i3,2)
     & *uzxc +SCZ63(i1,i2,i3,2)*uxc+ sc(i1,i2,i3,3)*uxyc +SCX63(i1,i2,
     & i3,3)*uyc+ sc(i1,i2,i3,4)*uyyc +SCY63(i1,i2,i3,4)*uyc+ sc(i1,
     & i2,i3,5)*uzyc +SCZ63(i1,i2,i3,5)*uyc+ sc(i1,i2,i3,6)*uxzc +
     & SCX63(i1,i2,i3,6)*uzc+ sc(i1,i2,i3,7)*uyzc +SCY63(i1,i2,i3,7)*
     & uzc+ sc(i1,i2,i3,8)*uzzc +SCZ63(i1,i2,i3,8)*uzc
             end do
             end do
             end do
             end do
           else if( order.eq.8 )then
! divTensorGrad3d(83)
             do c=ca,cb
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
              uxc = UX83(i1,i2,i3,c)
              uyc = UY83(i1,i2,i3,c)
              uzc = UZ83(i1,i2,i3,c)
              uxxc= UXX83(i1,i2,i3,c)
              uxyc= UXY83(i1,i2,i3,c)
              uxzc= UXZ83(i1,i2,i3,c)
              uyxc=uxyc
              uyyc= UYY83(i1,i2,i3,c)
              uyzc= UYZ83(i1,i2,i3,c)
              uzxc=uxzc
              uzyc=uyzc
              uzzc= UZZ83(i1,i2,i3,c)
              deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*uxxc +SCX83(i1,i2,i3,0)*
     & uxc+ sc(i1,i2,i3,1)*uyxc +SCY83(i1,i2,i3,1)*uxc+ sc(i1,i2,i3,2)
     & *uzxc +SCZ83(i1,i2,i3,2)*uxc+ sc(i1,i2,i3,3)*uxyc +SCX83(i1,i2,
     & i3,3)*uyc+ sc(i1,i2,i3,4)*uyyc +SCY83(i1,i2,i3,4)*uyc+ sc(i1,
     & i2,i3,5)*uzyc +SCZ83(i1,i2,i3,5)*uyc+ sc(i1,i2,i3,6)*uxzc +
     & SCX83(i1,i2,i3,6)*uzc+ sc(i1,i2,i3,7)*uyzc +SCY83(i1,i2,i3,7)*
     & uzc+ sc(i1,i2,i3,8)*uzzc +SCZ83(i1,i2,i3,8)*uzc
             end do
             end do
             end do
             end do
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
! divTensorGrad1d(21R)
             do c=ca,cb
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
              uxc = UX21R(i1,i2,i3,c)
              uxxc= UXX21R(i1,i2,i3,c)
              deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*uxxc +SCX21R(i1,i2,i3,0)
     & *uxc
             end do
             end do
             end do
             end do
           else if( order.eq.4 )then
! divTensorGrad1d(41R)
             do c=ca,cb
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
              uxc = UX41R(i1,i2,i3,c)
              uxxc= UXX41R(i1,i2,i3,c)
              deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*uxxc +SCX41R(i1,i2,i3,0)
     & *uxc
             end do
             end do
             end do
             end do
           else if( order.eq.6 )then
! divTensorGrad1d(61R)
             do c=ca,cb
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
              uxc = UX61R(i1,i2,i3,c)
              uxxc= UXX61R(i1,i2,i3,c)
              deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*uxxc +SCX61R(i1,i2,i3,0)
     & *uxc
             end do
             end do
             end do
             end do
           else if( order.eq.8 )then
! divTensorGrad1d(81R)
             do c=ca,cb
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
              uxc = UX81R(i1,i2,i3,c)
              uxxc= UXX81R(i1,i2,i3,c)
              deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*uxxc +SCX81R(i1,i2,i3,0)
     & *uxc
             end do
             end do
             end do
             end do
           else
             write(*,*) 'ERROR:divTensorGradNC:order=',order
             stop 41
           end if
         else
c           ***** not rectangular *****
           if( order.eq.2 )then
! divTensorGrad1d(21)
             do c=ca,cb
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
              uxc = UX21(i1,i2,i3,c)
              uxxc= UXX21(i1,i2,i3,c)
              deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*uxxc +SCX21(i1,i2,i3,0)*
     & uxc
             end do
             end do
             end do
             end do
           else if( order.eq.4 )then
! divTensorGrad1d(41)
             do c=ca,cb
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
              uxc = UX41(i1,i2,i3,c)
              uxxc= UXX41(i1,i2,i3,c)
              deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*uxxc +SCX41(i1,i2,i3,0)*
     & uxc
             end do
             end do
             end do
             end do
           else if( order.eq.6 )then
! divTensorGrad1d(61)
             do c=ca,cb
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
              uxc = UX61(i1,i2,i3,c)
              uxxc= UXX61(i1,i2,i3,c)
              deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*uxxc +SCX61(i1,i2,i3,0)*
     & uxc
             end do
             end do
             end do
             end do
           else if( order.eq.8 )then
! divTensorGrad1d(81)
             do c=ca,cb
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
              uxc = UX81(i1,i2,i3,c)
              uxxc= UXX81(i1,i2,i3,c)
              deriv(i1,i2,i3,c)=sc(i1,i2,i3,0)*uxxc +SCX81(i1,i2,i3,0)*
     & uxc
             end do
             end do
             end do
             end do
           else
             write(*,*) 'ERROR:divTensorGradNC:order=',order
             stop 41
           end if
         endif
        end if ! end nd.eq.1
       else
         write(*,'(" Unexpected value for derivOption=",i6)') 
     & derivOption
         stop 4523
       end if
       if( nd.eq.0 )then
c       *** add these lines to avoid warnings about unused statement functions
       end if
       return
       end
