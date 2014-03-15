! This file automatically generated from interpOpt.bf with bpp.
! defineInterpOptRes(TP)
       subroutine interpOptResTP ( nd,ndui1a,ndui1b,ndui2a,ndui2b,
     & ndui3a,ndui3b,ndui4a,ndui4b,ndug1a,ndug1b,ndug2a,ndug2b,ndug3a,
     & ndug3b,ndug4a,ndug4b,ndil,ndip,ndc1,ndc2,ndc3,ipar,ui,ug,c,r,
     & il,ip,varWidth, width, resMax )
c=================================================================================
c  Optimised interpolation with residual computation.
c   This version is for the iterative implicit method
c  since it also computes a residual.
c=================================================================================
       implicit none
       integer nd,nia,nib,c2a,c2b,c3a,c3b,ndil,ndip,ndc1,ndc2,ndc3
       integer ndui1a,ndui1b,ndui2a,ndui2b,ndui3a,ndui3b,ndui4a,ndui4b,
     & ndug1a,ndug1b,ndug2a,ndug2b,ndug3a,ndug3b,ndug4a,ndug4b
       real ui(ndui1a:ndui1b,ndui2a:ndui2b,ndui3a:ndui3b,ndui4a:ndui4b)
       real ug(ndug1a:ndug1b,ndug2a:ndug2b,ndug3a:ndug3b,ndug4a:ndug4b)
       real r(0:*),resMax
       real c(0:ndc1-1,0:ndc2-1,0:ndc3-1,0:*)
       integer width(3), il(0:ndil-1,*), ip(0:ndip-1,*), varWidth(0:*)
       integer ipar(0:*),storageOption,useVariableWidthInterpolation
       integer i,c2,c3,w1,w2,w3,i1,i2,i3,m2,m3
       real x
       real cr0,cr1,cr2,cr3,cr4,cr5,cr6,cr7,cr8,cr9,cr10
       real cs0,cs1,cs2,cs3,cs4,cs5,cs6,cs7,cs8,cs9,cs10
       real ct0,ct1,ct2,ct3,ct4,ct5,ct6,ct7,ct8,ct9,ct10
c real tpi2,tpi3,tpi4,tpi5,tpi6,tpi7,tpi8,tpi9
c  real spi2,spi3,spi4,spi5,spi6,spi7,spi8,spi9
c ---- start statement functions
!       #Include "lagrangePolynomials.h"
      real q10,q20,q21,q30,q31,q32,q40,q41,q42,q43,q50,q51,q52,q53,q54,
     & q60,q61,q62,q63,q64,q65,q70,q71,q72,q73,q74,q75,q76,q80,q81,
     & q82,q83,q84,q85,q86,q87,q90,q91,q92,q93,q94,q95,q96,q97,q98
      q10(x)=1
      q20(x)=-x+1
      q21(x)=x
      q30(x)=(x-1)*(x-2)/2.
      q31(x)=-x*(x-2)
      q32(x)=x*(x-1)/2.
      q40(x)=-(x-1)*(x-2)*(x-3)/6.
      q41(x)=x*(x-2)*(x-3)/2.
      q42(x)=-x*(x-1)*(x-3)/2.
      q43(x)=x*(x-1)*(x-2)/6.
      q50(x)=(x-1)*(x-2)*(x-3)*(x-4)/24.
      q51(x)=-x*(x-2)*(x-3)*(x-4)/6.
      q52(x)=x*(x-1)*(x-3)*(x-4)/4.
      q53(x)=-x*(x-1)*(x-2)*(x-4)/6.
      q54(x)=x*(x-1)*(x-2)*(x-3)/24.
      q60(x)=-(x-1)*(x-2)*(x-3)*(x-4)*(x-5)/120.
      q61(x)=x*(x-2)*(x-3)*(x-4)*(x-5)/24.
      q62(x)=-x*(x-1)*(x-3)*(x-4)*(x-5)/12.
      q63(x)=x*(x-1)*(x-2)*(x-4)*(x-5)/12.
      q64(x)=-x*(x-1)*(x-2)*(x-3)*(x-5)/24.
      q65(x)=x*(x-1)*(x-2)*(x-3)*(x-4)/120.
      q70(x)=(x-1)*(x-2)*(x-3)*(x-4)*(x-5)*(x-6)/720.
      q71(x)=-x*(x-2)*(x-3)*(x-4)*(x-5)*(x-6)/120.
      q72(x)=x*(x-1)*(x-3)*(x-4)*(x-5)*(x-6)/48.
      q73(x)=-x*(x-1)*(x-2)*(x-4)*(x-5)*(x-6)/36.
      q74(x)=x*(x-1)*(x-2)*(x-3)*(x-5)*(x-6)/48.
      q75(x)=-x*(x-1)*(x-2)*(x-3)*(x-4)*(x-6)/120.
      q76(x)=x*(x-1)*(x-2)*(x-3)*(x-4)*(x-5)/720.
      q80(x)=-(x-1)*(x-2)*(x-3)*(x-4)*(x-5)*(x-6)*(x-7)/5040.
      q81(x)=x*(x-2)*(x-3)*(x-4)*(x-5)*(x-6)*(x-7)/720.
      q82(x)=-x*(x-1)*(x-3)*(x-4)*(x-5)*(x-6)*(x-7)/240.
      q83(x)=x*(x-1)*(x-2)*(x-4)*(x-5)*(x-6)*(x-7)/144.
      q84(x)=-x*(x-1)*(x-2)*(x-3)*(x-5)*(x-6)*(x-7)/144.
      q85(x)=x*(x-1)*(x-2)*(x-3)*(x-4)*(x-6)*(x-7)/240.
      q86(x)=-x*(x-1)*(x-2)*(x-3)*(x-4)*(x-5)*(x-7)/720.
      q87(x)=x*(x-1)*(x-2)*(x-3)*(x-4)*(x-5)*(x-6)/5040.
      q90(x)=(x-1)*(x-2)*(x-3)*(x-4)*(x-5)*(x-6)*(x-7)*(x-8)/40320.
      q91(x)=-x*(x-2)*(x-3)*(x-4)*(x-5)*(x-6)*(x-7)*(x-8)/5040.
      q92(x)=x*(x-1)*(x-3)*(x-4)*(x-5)*(x-6)*(x-7)*(x-8)/1440.
      q93(x)=-x*(x-1)*(x-2)*(x-4)*(x-5)*(x-6)*(x-7)*(x-8)/720.
      q94(x)=x*(x-1)*(x-2)*(x-3)*(x-5)*(x-6)*(x-7)*(x-8)/576.
      q95(x)=-x*(x-1)*(x-2)*(x-3)*(x-4)*(x-6)*(x-7)*(x-8)/720.
      q96(x)=x*(x-1)*(x-2)*(x-3)*(x-4)*(x-5)*(x-7)*(x-8)/1440.
      q97(x)=-x*(x-1)*(x-2)*(x-3)*(x-4)*(x-5)*(x-6)*(x-8)/5040.
      q98(x)=x*(x-1)*(x-2)*(x-3)*(x-4)*(x-5)*(x-6)*(x-7)/40320.
c ---- end statement functions
c write(*,*) 'interpOptRes: width=',width(1),width(2)
        nia=ipar(0)
        nib=ipar(1)
        c2a=ipar(2)
        c2b=ipar(3)
        c3a=ipar(4)
        c3b=ipar(5)
        storageOption=ipar(6)
        useVariableWidthInterpolation=ipar(7)
       ! write(*,'(" **interpOptRes: useVariableWidthInterpolation=",i2)') useVariableWidthInterpolation
! #If "TP" == "Full"
! #If "TP" == "TP"
       if( storageOption.eq.1 )then
c       ****************************************
c       **** tensor-product storage option *****
c       ****************************************
       if( nd.eq.2 )then
         if( useVariableWidthInterpolation.ne.0 )then
! beginLoops2d()
             do i=nia,nib
             do c3=c3a,c3b
             do c2=c2a,c2b
             ! check for most common widths first
             if( varWidth(i).eq.3 )then
! interpTensorProduct33(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,1,0)*(c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,
     & 0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,c3))+c(i,1,
     & 1,0)*(c(i,0,0,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,
     & c2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,c2,c3))+c(i,2,1,0)*(c(i,0,0,0)*
     & ui(i1  ,i2+2,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,c2,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+2,c2,c3))
             else if( varWidth(i).eq.2 )then
! interpTensorProduct22(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,1,0)*(c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,
     & 0,0)*ui(i1+1,i2  ,c2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1  ,i2+1,
     & c2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,c2,c3))
             else if( varWidth(i).eq.1 )then
! interp11(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = ui(i1  ,i2  ,c2,c3)
             else if( varWidth(i).eq.5 )then
! interpTensorProduct55(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,1,0)*(c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,
     & 0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,c3))+c(i,1,
     & 1,0)*(c(i,0,0,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,
     & c2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,
     & c2,c3)+c(i,4,0,0)*ui(i1+4,i2+1,c2,c3))+c(i,2,1,0)*(c(i,0,0,0)*
     & ui(i1  ,i2+2,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,c2,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+2,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+2,c2,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+2,c2,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1  ,i2+3,c2,c3)
     & +c(i,1,0,0)*ui(i1+1,i2+3,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+3,c2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+3,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+3,c2,c3))+
     & c(i,4,1,0)*(c(i,0,0,0)*ui(i1  ,i2+4,c2,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+4,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,c2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,c2,c3))
             else if( varWidth(i).eq.4 )then
! interpTensorProduct44(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,1,0)*(c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,
     & 0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2  ,c2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1  ,i2+1,
     & c2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,
     & c2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,c2,c3))+c(i,2,1,0)*(c(i,0,0,0)*
     & ui(i1  ,i2+2,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,c2,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+2,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+2,c2,c3))+c(i,3,1,0)*
     & (c(i,0,0,0)*ui(i1  ,i2+3,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+3,c2,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+3,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+3,c2,c3))
             else if( varWidth(i).eq.7 )then
! interpTensorProduct77(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,1,0)*(c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,
     & 0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,c3)+c(i,5,
     & 0,0)*ui(i1+5,i2  ,c2,c3)+c(i,6,0,0)*ui(i1+6,i2  ,c2,c3))+c(i,1,
     & 1,0)*(c(i,0,0,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,
     & c2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,
     & c2,c3)+c(i,4,0,0)*ui(i1+4,i2+1,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+1,
     & c2,c3)+c(i,6,0,0)*ui(i1+6,i2+1,c2,c3))+c(i,2,1,0)*(c(i,0,0,0)*
     & ui(i1  ,i2+2,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,c2,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+2,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+2,c2,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+2,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+2,c2,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+2,c2,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1  ,i2+3,c2,c3)
     & +c(i,1,0,0)*ui(i1+1,i2+3,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+3,c2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+3,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+3,c2,c3)+
     & c(i,5,0,0)*ui(i1+5,i2+3,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+3,c2,c3))+
     & c(i,4,1,0)*(c(i,0,0,0)*ui(i1  ,i2+4,c2,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+4,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,c2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,c2,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+4,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+4,c2,c3))+c(i,5,1,0)*(c(i,0,
     & 0,0)*ui(i1  ,i2+5,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+5,c2,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+5,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+5,c2,c3)+c(i,4,
     & 0,0)*ui(i1+4,i2+5,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+5,c2,c3)+c(i,6,
     & 0,0)*ui(i1+6,i2+5,c2,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1  ,i2+6,
     & c2,c3)+c(i,1,0,0)*ui(i1+1,i2+6,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+6,
     & c2,c3)+c(i,3,0,0)*ui(i1+3,i2+6,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+6,
     & c2,c3)+c(i,5,0,0)*ui(i1+5,i2+6,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+6,
     & c2,c3))
             else if( varWidth(i).eq.6 )then
! interpTensorProduct66(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,1,0)*(c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,
     & 0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,c3)+c(i,5,
     & 0,0)*ui(i1+5,i2  ,c2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1  ,i2+1,
     & c2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,
     & c2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+1,
     & c2,c3)+c(i,5,0,0)*ui(i1+5,i2+1,c2,c3))+c(i,2,1,0)*(c(i,0,0,0)*
     & ui(i1  ,i2+2,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,c2,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+2,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+2,c2,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+2,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+2,c2,c3))+c(i,3,1,0)*
     & (c(i,0,0,0)*ui(i1  ,i2+3,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+3,c2,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+3,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+3,c2,c3)+
     & c(i,4,0,0)*ui(i1+4,i2+3,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+3,c2,c3))+
     & c(i,4,1,0)*(c(i,0,0,0)*ui(i1  ,i2+4,c2,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+4,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,c2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,c2,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+4,c2,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1  ,i2+5,c2,c3)+c(i,1,
     & 0,0)*ui(i1+1,i2+5,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+5,c2,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+5,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+5,c2,c3)+c(i,5,
     & 0,0)*ui(i1+5,i2+5,c2,c3))
             else if( varWidth(i).eq.9 )then
! interpTensorProduct99(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,1,0)*(c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,
     & 0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,c3)+c(i,5,
     & 0,0)*ui(i1+5,i2  ,c2,c3)+c(i,6,0,0)*ui(i1+6,i2  ,c2,c3)+c(i,7,
     & 0,0)*ui(i1+7,i2  ,c2,c3)+c(i,8,0,0)*ui(i1+8,i2  ,c2,c3))+c(i,1,
     & 1,0)*(c(i,0,0,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,
     & c2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,
     & c2,c3)+c(i,4,0,0)*ui(i1+4,i2+1,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+1,
     & c2,c3)+c(i,6,0,0)*ui(i1+6,i2+1,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+1,
     & c2,c3)+c(i,8,0,0)*ui(i1+8,i2+1,c2,c3))+c(i,2,1,0)*(c(i,0,0,0)*
     & ui(i1  ,i2+2,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,c2,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+2,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+2,c2,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+2,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+2,c2,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+2,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+2,c2,c3)+c(i,8,0,0)*
     & ui(i1+8,i2+2,c2,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1  ,i2+3,c2,c3)
     & +c(i,1,0,0)*ui(i1+1,i2+3,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+3,c2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+3,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+3,c2,c3)+
     & c(i,5,0,0)*ui(i1+5,i2+3,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+3,c2,c3)+
     & c(i,7,0,0)*ui(i1+7,i2+3,c2,c3)+c(i,8,0,0)*ui(i1+8,i2+3,c2,c3))+
     & c(i,4,1,0)*(c(i,0,0,0)*ui(i1  ,i2+4,c2,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+4,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,c2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,c2,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+4,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+4,c2,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+4,c2,c3)+c(i,8,0,0)*ui(i1+8,i2+4,c2,c3))+c(i,5,1,0)*(c(i,0,
     & 0,0)*ui(i1  ,i2+5,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+5,c2,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+5,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+5,c2,c3)+c(i,4,
     & 0,0)*ui(i1+4,i2+5,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+5,c2,c3)+c(i,6,
     & 0,0)*ui(i1+6,i2+5,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+5,c2,c3)+c(i,8,
     & 0,0)*ui(i1+8,i2+5,c2,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1  ,i2+6,
     & c2,c3)+c(i,1,0,0)*ui(i1+1,i2+6,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+6,
     & c2,c3)+c(i,3,0,0)*ui(i1+3,i2+6,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+6,
     & c2,c3)+c(i,5,0,0)*ui(i1+5,i2+6,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+6,
     & c2,c3)+c(i,7,0,0)*ui(i1+7,i2+6,c2,c3)+c(i,8,0,0)*ui(i1+8,i2+6,
     & c2,c3))+c(i,7,1,0)*(c(i,0,0,0)*ui(i1  ,i2+7,c2,c3)+c(i,1,0,0)*
     & ui(i1+1,i2+7,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+7,c2,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+7,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+7,c2,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+7,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+7,c2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+7,c2,c3)+c(i,8,0,0)*ui(i1+8,i2+7,c2,c3))+c(i,8,1,0)*
     & (c(i,0,0,0)*ui(i1  ,i2+8,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+8,c2,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+8,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+8,c2,c3)+
     & c(i,4,0,0)*ui(i1+4,i2+8,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+8,c2,c3)+
     & c(i,6,0,0)*ui(i1+6,i2+8,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+8,c2,c3)+
     & c(i,8,0,0)*ui(i1+8,i2+8,c2,c3))
             else if( varWidth(i).eq.8 )then
! interpTensorProduct88(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,1,0)*(c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,
     & 0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,c3)+c(i,5,
     & 0,0)*ui(i1+5,i2  ,c2,c3)+c(i,6,0,0)*ui(i1+6,i2  ,c2,c3)+c(i,7,
     & 0,0)*ui(i1+7,i2  ,c2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1  ,i2+1,
     & c2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,
     & c2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+1,
     & c2,c3)+c(i,5,0,0)*ui(i1+5,i2+1,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+1,
     & c2,c3)+c(i,7,0,0)*ui(i1+7,i2+1,c2,c3))+c(i,2,1,0)*(c(i,0,0,0)*
     & ui(i1  ,i2+2,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,c2,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+2,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+2,c2,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+2,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+2,c2,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+2,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+2,c2,c3))+c(i,3,1,0)*
     & (c(i,0,0,0)*ui(i1  ,i2+3,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+3,c2,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+3,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+3,c2,c3)+
     & c(i,4,0,0)*ui(i1+4,i2+3,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+3,c2,c3)+
     & c(i,6,0,0)*ui(i1+6,i2+3,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+3,c2,c3))+
     & c(i,4,1,0)*(c(i,0,0,0)*ui(i1  ,i2+4,c2,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+4,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,c2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,c2,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+4,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+4,c2,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+4,c2,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1  ,i2+5,c2,c3)+c(i,1,
     & 0,0)*ui(i1+1,i2+5,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+5,c2,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+5,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+5,c2,c3)+c(i,5,
     & 0,0)*ui(i1+5,i2+5,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+5,c2,c3)+c(i,7,
     & 0,0)*ui(i1+7,i2+5,c2,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1  ,i2+6,
     & c2,c3)+c(i,1,0,0)*ui(i1+1,i2+6,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+6,
     & c2,c3)+c(i,3,0,0)*ui(i1+3,i2+6,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+6,
     & c2,c3)+c(i,5,0,0)*ui(i1+5,i2+6,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+6,
     & c2,c3)+c(i,7,0,0)*ui(i1+7,i2+6,c2,c3))+c(i,7,1,0)*(c(i,0,0,0)*
     & ui(i1  ,i2+7,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+7,c2,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+7,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+7,c2,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+7,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+7,c2,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+7,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+7,c2,c3))
             else
               write(*,*) 'ERROR varWidth=',varWidth(i)
               stop 151
             end if
             resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i)))
             ug(ip(i,1),ip(i,2),c2,c3)= r(i)
! endLoops2d()
             end do
             end do
             end do
         else if( width(1).eq.3 .and. width(2).eq.3 ) then
! loops2d($interpTensorProduct33(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),ug(ip(i,1),ip(i,2),c2,c3)= r(i))
           if( c2a.eq.c2b .and. c3a.eq.c3b )then
             do c3=c3a,c3b
             do c2=c2a,c2b
             do i=nia,nib
! interpTensorProduct33(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,1,0)*(c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,
     & 0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,c3))+c(i,1,
     & 1,0)*(c(i,0,0,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,
     & c2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,c2,c3))+c(i,2,1,0)*(c(i,0,0,0)*
     & ui(i1  ,i2+2,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,c2,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+2,c2,c3))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i)))
               ug(ip(i,1),ip(i,2),c2,c3)=r(i)
             end do
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
             do c2=c2a,c2b
! interpTensorProduct33(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,1,0)*(c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,
     & 0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,c3))+c(i,1,
     & 1,0)*(c(i,0,0,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,
     & c2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,c2,c3))+c(i,2,1,0)*(c(i,0,0,0)*
     & ui(i1  ,i2+2,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,c2,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+2,c2,c3))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i)))
               ug(ip(i,1),ip(i,2),c2,c3)=r(i)
             end do
             end do
             end do
           end if
         else if( width(1).eq.1 .and. width(1).eq.1)then
! loops2d($interp11(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),ug(ip(i,1),ip(i,2),c2,c3)= r(i))
           if( c2a.eq.c2b .and. c3a.eq.c3b )then
             do c3=c3a,c3b
             do c2=c2a,c2b
             do i=nia,nib
! interp11(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = ui(i1  ,i2  ,c2,c3)
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i)))
               ug(ip(i,1),ip(i,2),c2,c3)=r(i)
             end do
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
             do c2=c2a,c2b
! interp11(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = ui(i1  ,i2  ,c2,c3)
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i)))
               ug(ip(i,1),ip(i,2),c2,c3)=r(i)
             end do
             end do
             end do
           end if
         else if( width(1).eq.2 .and. width(2).eq.2 )then
! loops2d($interpTensorProduct22(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),ug(ip(i,1),ip(i,2),c2,c3)= r(i))
           if( c2a.eq.c2b .and. c3a.eq.c3b )then
             do c3=c3a,c3b
             do c2=c2a,c2b
             do i=nia,nib
! interpTensorProduct22(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,1,0)*(c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,
     & 0,0)*ui(i1+1,i2  ,c2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1  ,i2+1,
     & c2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,c2,c3))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i)))
               ug(ip(i,1),ip(i,2),c2,c3)=r(i)
             end do
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
             do c2=c2a,c2b
! interpTensorProduct22(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,1,0)*(c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,
     & 0,0)*ui(i1+1,i2  ,c2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1  ,i2+1,
     & c2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,c2,c3))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i)))
               ug(ip(i,1),ip(i,2),c2,c3)=r(i)
             end do
             end do
             end do
           end if
         else if( width(1).eq.4 .and. width(2).eq.4 )then
! loops2d($interpTensorProduct44(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),ug(ip(i,1),ip(i,2),c2,c3)= r(i))
           if( c2a.eq.c2b .and. c3a.eq.c3b )then
             do c3=c3a,c3b
             do c2=c2a,c2b
             do i=nia,nib
! interpTensorProduct44(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,1,0)*(c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,
     & 0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2  ,c2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1  ,i2+1,
     & c2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,
     & c2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,c2,c3))+c(i,2,1,0)*(c(i,0,0,0)*
     & ui(i1  ,i2+2,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,c2,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+2,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+2,c2,c3))+c(i,3,1,0)*
     & (c(i,0,0,0)*ui(i1  ,i2+3,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+3,c2,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+3,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+3,c2,c3))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i)))
               ug(ip(i,1),ip(i,2),c2,c3)=r(i)
             end do
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
             do c2=c2a,c2b
! interpTensorProduct44(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,1,0)*(c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,
     & 0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2  ,c2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1  ,i2+1,
     & c2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,
     & c2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,c2,c3))+c(i,2,1,0)*(c(i,0,0,0)*
     & ui(i1  ,i2+2,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,c2,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+2,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+2,c2,c3))+c(i,3,1,0)*
     & (c(i,0,0,0)*ui(i1  ,i2+3,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+3,c2,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+3,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+3,c2,c3))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i)))
               ug(ip(i,1),ip(i,2),c2,c3)=r(i)
             end do
             end do
             end do
           end if
         else if( width(1).eq.5 .and. width(2).eq.5 )then
! loops2d($interpTensorProduct55(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),ug(ip(i,1),ip(i,2),c2,c3)= r(i))
           if( c2a.eq.c2b .and. c3a.eq.c3b )then
             do c3=c3a,c3b
             do c2=c2a,c2b
             do i=nia,nib
! interpTensorProduct55(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,1,0)*(c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,
     & 0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,c3))+c(i,1,
     & 1,0)*(c(i,0,0,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,
     & c2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,
     & c2,c3)+c(i,4,0,0)*ui(i1+4,i2+1,c2,c3))+c(i,2,1,0)*(c(i,0,0,0)*
     & ui(i1  ,i2+2,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,c2,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+2,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+2,c2,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+2,c2,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1  ,i2+3,c2,c3)
     & +c(i,1,0,0)*ui(i1+1,i2+3,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+3,c2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+3,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+3,c2,c3))+
     & c(i,4,1,0)*(c(i,0,0,0)*ui(i1  ,i2+4,c2,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+4,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,c2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,c2,c3))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i)))
               ug(ip(i,1),ip(i,2),c2,c3)=r(i)
             end do
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
             do c2=c2a,c2b
! interpTensorProduct55(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,1,0)*(c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,
     & 0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,c3))+c(i,1,
     & 1,0)*(c(i,0,0,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,
     & c2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,
     & c2,c3)+c(i,4,0,0)*ui(i1+4,i2+1,c2,c3))+c(i,2,1,0)*(c(i,0,0,0)*
     & ui(i1  ,i2+2,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,c2,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+2,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+2,c2,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+2,c2,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1  ,i2+3,c2,c3)
     & +c(i,1,0,0)*ui(i1+1,i2+3,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+3,c2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+3,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+3,c2,c3))+
     & c(i,4,1,0)*(c(i,0,0,0)*ui(i1  ,i2+4,c2,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+4,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,c2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,c2,c3))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i)))
               ug(ip(i,1),ip(i,2),c2,c3)=r(i)
             end do
             end do
             end do
           end if
         else if( width(1).eq.6 .and. width(2).eq.6 )then
! loops2d($interpTensorProduct66(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),ug(ip(i,1),ip(i,2),c2,c3)= r(i))
           if( c2a.eq.c2b .and. c3a.eq.c3b )then
             do c3=c3a,c3b
             do c2=c2a,c2b
             do i=nia,nib
! interpTensorProduct66(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,1,0)*(c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,
     & 0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,c3)+c(i,5,
     & 0,0)*ui(i1+5,i2  ,c2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1  ,i2+1,
     & c2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,
     & c2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+1,
     & c2,c3)+c(i,5,0,0)*ui(i1+5,i2+1,c2,c3))+c(i,2,1,0)*(c(i,0,0,0)*
     & ui(i1  ,i2+2,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,c2,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+2,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+2,c2,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+2,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+2,c2,c3))+c(i,3,1,0)*
     & (c(i,0,0,0)*ui(i1  ,i2+3,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+3,c2,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+3,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+3,c2,c3)+
     & c(i,4,0,0)*ui(i1+4,i2+3,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+3,c2,c3))+
     & c(i,4,1,0)*(c(i,0,0,0)*ui(i1  ,i2+4,c2,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+4,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,c2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,c2,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+4,c2,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1  ,i2+5,c2,c3)+c(i,1,
     & 0,0)*ui(i1+1,i2+5,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+5,c2,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+5,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+5,c2,c3)+c(i,5,
     & 0,0)*ui(i1+5,i2+5,c2,c3))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i)))
               ug(ip(i,1),ip(i,2),c2,c3)=r(i)
             end do
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
             do c2=c2a,c2b
! interpTensorProduct66(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,1,0)*(c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,
     & 0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,c3)+c(i,5,
     & 0,0)*ui(i1+5,i2  ,c2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1  ,i2+1,
     & c2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,
     & c2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+1,
     & c2,c3)+c(i,5,0,0)*ui(i1+5,i2+1,c2,c3))+c(i,2,1,0)*(c(i,0,0,0)*
     & ui(i1  ,i2+2,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,c2,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+2,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+2,c2,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+2,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+2,c2,c3))+c(i,3,1,0)*
     & (c(i,0,0,0)*ui(i1  ,i2+3,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+3,c2,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+3,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+3,c2,c3)+
     & c(i,4,0,0)*ui(i1+4,i2+3,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+3,c2,c3))+
     & c(i,4,1,0)*(c(i,0,0,0)*ui(i1  ,i2+4,c2,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+4,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,c2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,c2,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+4,c2,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1  ,i2+5,c2,c3)+c(i,1,
     & 0,0)*ui(i1+1,i2+5,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+5,c2,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+5,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+5,c2,c3)+c(i,5,
     & 0,0)*ui(i1+5,i2+5,c2,c3))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i)))
               ug(ip(i,1),ip(i,2),c2,c3)=r(i)
             end do
             end do
             end do
           end if
         else if( width(1).eq.7 .and. width(2).eq.7 )then
! loops2d($interpTensorProduct77(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),ug(ip(i,1),ip(i,2),c2,c3)= r(i))
           if( c2a.eq.c2b .and. c3a.eq.c3b )then
             do c3=c3a,c3b
             do c2=c2a,c2b
             do i=nia,nib
! interpTensorProduct77(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,1,0)*(c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,
     & 0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,c3)+c(i,5,
     & 0,0)*ui(i1+5,i2  ,c2,c3)+c(i,6,0,0)*ui(i1+6,i2  ,c2,c3))+c(i,1,
     & 1,0)*(c(i,0,0,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,
     & c2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,
     & c2,c3)+c(i,4,0,0)*ui(i1+4,i2+1,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+1,
     & c2,c3)+c(i,6,0,0)*ui(i1+6,i2+1,c2,c3))+c(i,2,1,0)*(c(i,0,0,0)*
     & ui(i1  ,i2+2,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,c2,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+2,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+2,c2,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+2,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+2,c2,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+2,c2,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1  ,i2+3,c2,c3)
     & +c(i,1,0,0)*ui(i1+1,i2+3,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+3,c2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+3,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+3,c2,c3)+
     & c(i,5,0,0)*ui(i1+5,i2+3,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+3,c2,c3))+
     & c(i,4,1,0)*(c(i,0,0,0)*ui(i1  ,i2+4,c2,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+4,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,c2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,c2,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+4,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+4,c2,c3))+c(i,5,1,0)*(c(i,0,
     & 0,0)*ui(i1  ,i2+5,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+5,c2,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+5,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+5,c2,c3)+c(i,4,
     & 0,0)*ui(i1+4,i2+5,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+5,c2,c3)+c(i,6,
     & 0,0)*ui(i1+6,i2+5,c2,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1  ,i2+6,
     & c2,c3)+c(i,1,0,0)*ui(i1+1,i2+6,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+6,
     & c2,c3)+c(i,3,0,0)*ui(i1+3,i2+6,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+6,
     & c2,c3)+c(i,5,0,0)*ui(i1+5,i2+6,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+6,
     & c2,c3))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i)))
               ug(ip(i,1),ip(i,2),c2,c3)=r(i)
             end do
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
             do c2=c2a,c2b
! interpTensorProduct77(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,1,0)*(c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,
     & 0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,c3)+c(i,5,
     & 0,0)*ui(i1+5,i2  ,c2,c3)+c(i,6,0,0)*ui(i1+6,i2  ,c2,c3))+c(i,1,
     & 1,0)*(c(i,0,0,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,
     & c2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,
     & c2,c3)+c(i,4,0,0)*ui(i1+4,i2+1,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+1,
     & c2,c3)+c(i,6,0,0)*ui(i1+6,i2+1,c2,c3))+c(i,2,1,0)*(c(i,0,0,0)*
     & ui(i1  ,i2+2,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,c2,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+2,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+2,c2,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+2,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+2,c2,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+2,c2,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1  ,i2+3,c2,c3)
     & +c(i,1,0,0)*ui(i1+1,i2+3,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+3,c2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+3,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+3,c2,c3)+
     & c(i,5,0,0)*ui(i1+5,i2+3,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+3,c2,c3))+
     & c(i,4,1,0)*(c(i,0,0,0)*ui(i1  ,i2+4,c2,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+4,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,c2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,c2,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+4,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+4,c2,c3))+c(i,5,1,0)*(c(i,0,
     & 0,0)*ui(i1  ,i2+5,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+5,c2,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+5,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+5,c2,c3)+c(i,4,
     & 0,0)*ui(i1+4,i2+5,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+5,c2,c3)+c(i,6,
     & 0,0)*ui(i1+6,i2+5,c2,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1  ,i2+6,
     & c2,c3)+c(i,1,0,0)*ui(i1+1,i2+6,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+6,
     & c2,c3)+c(i,3,0,0)*ui(i1+3,i2+6,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+6,
     & c2,c3)+c(i,5,0,0)*ui(i1+5,i2+6,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+6,
     & c2,c3))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i)))
               ug(ip(i,1),ip(i,2),c2,c3)=r(i)
             end do
             end do
             end do
           end if
         else if( width(1).eq.8 .and. width(2).eq.8 )then
! loops2d($interpTensorProduct88(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),ug(ip(i,1),ip(i,2),c2,c3)= r(i))
           if( c2a.eq.c2b .and. c3a.eq.c3b )then
             do c3=c3a,c3b
             do c2=c2a,c2b
             do i=nia,nib
! interpTensorProduct88(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,1,0)*(c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,
     & 0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,c3)+c(i,5,
     & 0,0)*ui(i1+5,i2  ,c2,c3)+c(i,6,0,0)*ui(i1+6,i2  ,c2,c3)+c(i,7,
     & 0,0)*ui(i1+7,i2  ,c2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1  ,i2+1,
     & c2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,
     & c2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+1,
     & c2,c3)+c(i,5,0,0)*ui(i1+5,i2+1,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+1,
     & c2,c3)+c(i,7,0,0)*ui(i1+7,i2+1,c2,c3))+c(i,2,1,0)*(c(i,0,0,0)*
     & ui(i1  ,i2+2,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,c2,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+2,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+2,c2,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+2,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+2,c2,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+2,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+2,c2,c3))+c(i,3,1,0)*
     & (c(i,0,0,0)*ui(i1  ,i2+3,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+3,c2,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+3,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+3,c2,c3)+
     & c(i,4,0,0)*ui(i1+4,i2+3,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+3,c2,c3)+
     & c(i,6,0,0)*ui(i1+6,i2+3,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+3,c2,c3))+
     & c(i,4,1,0)*(c(i,0,0,0)*ui(i1  ,i2+4,c2,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+4,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,c2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,c2,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+4,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+4,c2,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+4,c2,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1  ,i2+5,c2,c3)+c(i,1,
     & 0,0)*ui(i1+1,i2+5,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+5,c2,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+5,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+5,c2,c3)+c(i,5,
     & 0,0)*ui(i1+5,i2+5,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+5,c2,c3)+c(i,7,
     & 0,0)*ui(i1+7,i2+5,c2,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1  ,i2+6,
     & c2,c3)+c(i,1,0,0)*ui(i1+1,i2+6,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+6,
     & c2,c3)+c(i,3,0,0)*ui(i1+3,i2+6,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+6,
     & c2,c3)+c(i,5,0,0)*ui(i1+5,i2+6,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+6,
     & c2,c3)+c(i,7,0,0)*ui(i1+7,i2+6,c2,c3))+c(i,7,1,0)*(c(i,0,0,0)*
     & ui(i1  ,i2+7,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+7,c2,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+7,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+7,c2,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+7,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+7,c2,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+7,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+7,c2,c3))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i)))
               ug(ip(i,1),ip(i,2),c2,c3)=r(i)
             end do
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
             do c2=c2a,c2b
! interpTensorProduct88(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,1,0)*(c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,
     & 0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,c3)+c(i,5,
     & 0,0)*ui(i1+5,i2  ,c2,c3)+c(i,6,0,0)*ui(i1+6,i2  ,c2,c3)+c(i,7,
     & 0,0)*ui(i1+7,i2  ,c2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1  ,i2+1,
     & c2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,
     & c2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+1,
     & c2,c3)+c(i,5,0,0)*ui(i1+5,i2+1,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+1,
     & c2,c3)+c(i,7,0,0)*ui(i1+7,i2+1,c2,c3))+c(i,2,1,0)*(c(i,0,0,0)*
     & ui(i1  ,i2+2,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,c2,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+2,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+2,c2,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+2,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+2,c2,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+2,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+2,c2,c3))+c(i,3,1,0)*
     & (c(i,0,0,0)*ui(i1  ,i2+3,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+3,c2,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+3,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+3,c2,c3)+
     & c(i,4,0,0)*ui(i1+4,i2+3,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+3,c2,c3)+
     & c(i,6,0,0)*ui(i1+6,i2+3,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+3,c2,c3))+
     & c(i,4,1,0)*(c(i,0,0,0)*ui(i1  ,i2+4,c2,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+4,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,c2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,c2,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+4,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+4,c2,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+4,c2,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1  ,i2+5,c2,c3)+c(i,1,
     & 0,0)*ui(i1+1,i2+5,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+5,c2,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+5,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+5,c2,c3)+c(i,5,
     & 0,0)*ui(i1+5,i2+5,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+5,c2,c3)+c(i,7,
     & 0,0)*ui(i1+7,i2+5,c2,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1  ,i2+6,
     & c2,c3)+c(i,1,0,0)*ui(i1+1,i2+6,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+6,
     & c2,c3)+c(i,3,0,0)*ui(i1+3,i2+6,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+6,
     & c2,c3)+c(i,5,0,0)*ui(i1+5,i2+6,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+6,
     & c2,c3)+c(i,7,0,0)*ui(i1+7,i2+6,c2,c3))+c(i,7,1,0)*(c(i,0,0,0)*
     & ui(i1  ,i2+7,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+7,c2,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+7,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+7,c2,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+7,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+7,c2,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+7,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+7,c2,c3))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i)))
               ug(ip(i,1),ip(i,2),c2,c3)=r(i)
             end do
             end do
             end do
           end if
         else if( width(1).eq.9 .and. width(2).eq.9 )then
! loops2d($interpTensorProduct99(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),ug(ip(i,1),ip(i,2),c2,c3)= r(i))
           if( c2a.eq.c2b .and. c3a.eq.c3b )then
             do c3=c3a,c3b
             do c2=c2a,c2b
             do i=nia,nib
! interpTensorProduct99(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,1,0)*(c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,
     & 0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,c3)+c(i,5,
     & 0,0)*ui(i1+5,i2  ,c2,c3)+c(i,6,0,0)*ui(i1+6,i2  ,c2,c3)+c(i,7,
     & 0,0)*ui(i1+7,i2  ,c2,c3)+c(i,8,0,0)*ui(i1+8,i2  ,c2,c3))+c(i,1,
     & 1,0)*(c(i,0,0,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,
     & c2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,
     & c2,c3)+c(i,4,0,0)*ui(i1+4,i2+1,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+1,
     & c2,c3)+c(i,6,0,0)*ui(i1+6,i2+1,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+1,
     & c2,c3)+c(i,8,0,0)*ui(i1+8,i2+1,c2,c3))+c(i,2,1,0)*(c(i,0,0,0)*
     & ui(i1  ,i2+2,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,c2,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+2,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+2,c2,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+2,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+2,c2,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+2,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+2,c2,c3)+c(i,8,0,0)*
     & ui(i1+8,i2+2,c2,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1  ,i2+3,c2,c3)
     & +c(i,1,0,0)*ui(i1+1,i2+3,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+3,c2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+3,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+3,c2,c3)+
     & c(i,5,0,0)*ui(i1+5,i2+3,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+3,c2,c3)+
     & c(i,7,0,0)*ui(i1+7,i2+3,c2,c3)+c(i,8,0,0)*ui(i1+8,i2+3,c2,c3))+
     & c(i,4,1,0)*(c(i,0,0,0)*ui(i1  ,i2+4,c2,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+4,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,c2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,c2,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+4,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+4,c2,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+4,c2,c3)+c(i,8,0,0)*ui(i1+8,i2+4,c2,c3))+c(i,5,1,0)*(c(i,0,
     & 0,0)*ui(i1  ,i2+5,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+5,c2,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+5,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+5,c2,c3)+c(i,4,
     & 0,0)*ui(i1+4,i2+5,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+5,c2,c3)+c(i,6,
     & 0,0)*ui(i1+6,i2+5,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+5,c2,c3)+c(i,8,
     & 0,0)*ui(i1+8,i2+5,c2,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1  ,i2+6,
     & c2,c3)+c(i,1,0,0)*ui(i1+1,i2+6,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+6,
     & c2,c3)+c(i,3,0,0)*ui(i1+3,i2+6,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+6,
     & c2,c3)+c(i,5,0,0)*ui(i1+5,i2+6,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+6,
     & c2,c3)+c(i,7,0,0)*ui(i1+7,i2+6,c2,c3)+c(i,8,0,0)*ui(i1+8,i2+6,
     & c2,c3))+c(i,7,1,0)*(c(i,0,0,0)*ui(i1  ,i2+7,c2,c3)+c(i,1,0,0)*
     & ui(i1+1,i2+7,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+7,c2,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+7,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+7,c2,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+7,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+7,c2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+7,c2,c3)+c(i,8,0,0)*ui(i1+8,i2+7,c2,c3))+c(i,8,1,0)*
     & (c(i,0,0,0)*ui(i1  ,i2+8,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+8,c2,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+8,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+8,c2,c3)+
     & c(i,4,0,0)*ui(i1+4,i2+8,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+8,c2,c3)+
     & c(i,6,0,0)*ui(i1+6,i2+8,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+8,c2,c3)+
     & c(i,8,0,0)*ui(i1+8,i2+8,c2,c3))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i)))
               ug(ip(i,1),ip(i,2),c2,c3)=r(i)
             end do
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
             do c2=c2a,c2b
! interpTensorProduct99(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,1,0)*(c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,
     & 0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,c3)+c(i,5,
     & 0,0)*ui(i1+5,i2  ,c2,c3)+c(i,6,0,0)*ui(i1+6,i2  ,c2,c3)+c(i,7,
     & 0,0)*ui(i1+7,i2  ,c2,c3)+c(i,8,0,0)*ui(i1+8,i2  ,c2,c3))+c(i,1,
     & 1,0)*(c(i,0,0,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,
     & c2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,
     & c2,c3)+c(i,4,0,0)*ui(i1+4,i2+1,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+1,
     & c2,c3)+c(i,6,0,0)*ui(i1+6,i2+1,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+1,
     & c2,c3)+c(i,8,0,0)*ui(i1+8,i2+1,c2,c3))+c(i,2,1,0)*(c(i,0,0,0)*
     & ui(i1  ,i2+2,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,c2,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+2,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+2,c2,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+2,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+2,c2,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+2,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+2,c2,c3)+c(i,8,0,0)*
     & ui(i1+8,i2+2,c2,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1  ,i2+3,c2,c3)
     & +c(i,1,0,0)*ui(i1+1,i2+3,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+3,c2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+3,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+3,c2,c3)+
     & c(i,5,0,0)*ui(i1+5,i2+3,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+3,c2,c3)+
     & c(i,7,0,0)*ui(i1+7,i2+3,c2,c3)+c(i,8,0,0)*ui(i1+8,i2+3,c2,c3))+
     & c(i,4,1,0)*(c(i,0,0,0)*ui(i1  ,i2+4,c2,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+4,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,c2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,c2,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+4,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+4,c2,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+4,c2,c3)+c(i,8,0,0)*ui(i1+8,i2+4,c2,c3))+c(i,5,1,0)*(c(i,0,
     & 0,0)*ui(i1  ,i2+5,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+5,c2,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+5,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+5,c2,c3)+c(i,4,
     & 0,0)*ui(i1+4,i2+5,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+5,c2,c3)+c(i,6,
     & 0,0)*ui(i1+6,i2+5,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+5,c2,c3)+c(i,8,
     & 0,0)*ui(i1+8,i2+5,c2,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1  ,i2+6,
     & c2,c3)+c(i,1,0,0)*ui(i1+1,i2+6,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+6,
     & c2,c3)+c(i,3,0,0)*ui(i1+3,i2+6,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+6,
     & c2,c3)+c(i,5,0,0)*ui(i1+5,i2+6,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+6,
     & c2,c3)+c(i,7,0,0)*ui(i1+7,i2+6,c2,c3)+c(i,8,0,0)*ui(i1+8,i2+6,
     & c2,c3))+c(i,7,1,0)*(c(i,0,0,0)*ui(i1  ,i2+7,c2,c3)+c(i,1,0,0)*
     & ui(i1+1,i2+7,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+7,c2,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+7,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+7,c2,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+7,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+7,c2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+7,c2,c3)+c(i,8,0,0)*ui(i1+8,i2+7,c2,c3))+c(i,8,1,0)*
     & (c(i,0,0,0)*ui(i1  ,i2+8,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+8,c2,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+8,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+8,c2,c3)+
     & c(i,4,0,0)*ui(i1+4,i2+8,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+8,c2,c3)+
     & c(i,6,0,0)*ui(i1+6,i2+8,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+8,c2,c3)+
     & c(i,8,0,0)*ui(i1+8,i2+8,c2,c3))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i)))
               ug(ip(i,1),ip(i,2),c2,c3)=r(i)
             end do
             end do
             end do
           end if
         else
         !     general case in 2D
           do c3=c3a,c3b
             do c2=c2a,c2b
               do i=nia,nib
                 r(i)=0.
               end do
               do w2=0,width(2)-1
                 do w1=0,width(1)-1
                   do i=nia,nib
                     r(i)=r(i)+c(i,w1,w2,0)*ui(il(i,1)+w1,il(i,2)+w2,
     & c2,c3)
                   end do
                 end do
               end do
               do i=nia,nib
                 resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i)))
                 ug(ip(i,1),ip(i,2),c2,c3)= r(i)
               end do
             end do
           end do
         end if
       else
          !   *** 3D ****
         if( useVariableWidthInterpolation.ne.0 )then
! beginLoops3d()
             do i=nia,nib
             do c3=c3a,c3b
             ! check for most common widths first
             if( varWidth(i).eq.3 )then
! interpTensorProduct333(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+0,
     & c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+0,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+1,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+0,c3))+c(i,2,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+0,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+0,c3)))+c(i,1,2,0)*(c(i,0,1,0)*(
     & c(i,0,0,0)*ui(i1,i2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+1,c3)+c(
     & i,2,0,0)*ui(i1+2,i2,i3+1,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 1,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+1,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+1,i3+1,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+1,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+1,c3)))
     & +c(i,2,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+2,c3)+c(i,1,0,
     & 0)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+2,c3))+c(i,1,1,
     & 0)*(c(i,0,0,0)*ui(i1,i2+1,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+
     & 2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+2,c3))+c(i,2,1,0)*(c(i,0,0,0)*
     & ui(i1,i2+2,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+2,c3)+c(i,2,0,0)
     & *ui(i1+2,i2+2,i3+2,c3)))
             else if( varWidth(i).eq.2 )then
! interpTensorProduct222(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(
     & i1,i2+1,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+0,c3)))+c(i,1,2,0)*
     & (c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,
     & i2,i3+1,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+1,c3)+c(i,1,
     & 0,0)*ui(i1+1,i2+1,i3+1,c3)))
             else if( varWidth(i).eq.1 )then
! interp111(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = ui(i1,i2,i3,c3)
             else if( varWidth(i).eq.5 )then
! interpTensorProduct555(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+0,
     & c3)+c(i,3,0,0)*ui(i1+3,i2,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+0,
     & c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+0,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+1,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+0,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+1,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+0,c3))+c(i,2,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+0,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+0,c3))+c(i,3,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+3,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+0,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+3,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+0,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+0,c3))+c(i,4,1,0)*(c(i,0,0,0)*
     & ui(i1,i2+4,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+0,c3)+c(i,2,0,0)
     & *ui(i1+2,i2+4,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+0,c3)+c(i,4,
     & 0,0)*ui(i1+4,i2+4,i3+0,c3)))+c(i,1,2,0)*(c(i,0,1,0)*(c(i,0,0,0)
     & *ui(i1,i2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+1,c3)+c(i,2,0,0)*
     & ui(i1+2,i2,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+1,c3)+c(i,4,0,0)*
     & ui(i1+4,i2,i3+1,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+1,c3)
     & +c(i,1,0,0)*ui(i1+1,i2+1,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+1,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+1,
     & i3+1,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+1,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+2,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+1,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+2,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+1,c3))+c(
     & i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+
     & 3,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+1,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+1,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+1,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+1,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+1,c3)))+c(i,2,2,0)*(c(i,0,1,0)*(
     & c(i,0,0,0)*ui(i1,i2,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+2,c3)+c(
     & i,2,0,0)*ui(i1+2,i2,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+2,c3)+c(
     & i,4,0,0)*ui(i1+4,i2,i3+2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 1,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+2,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+1,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+2,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+1,i3+2,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+2,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+2,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+2,
     & i3+2,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+2,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+3,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+2,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+3,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+2,c3))+c(
     & i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+
     & 4,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+2,c3)))+c(i,3,2,0)*(c(
     & i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2,
     & i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2,
     & i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+3,c3))+c(i,1,1,0)*(c(i,0,0,0)
     & *ui(i1,i2+1,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+3,c3)+c(i,2,0,
     & 0)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+3,c3)+c(i,
     & 4,0,0)*ui(i1+4,i2+1,i3+3,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 2,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+3,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+2,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+3,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+2,i3+3,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+3,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+3,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+3,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+3,
     & i3+3,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+3,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+4,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+4,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+3,c3)))+
     & c(i,4,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+4,c3)+c(i,1,0,0)
     & *ui(i1+1,i2,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+4,c3)+c(i,3,0,0)*
     & ui(i1+3,i2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+4,c3))+c(i,1,1,0)*
     & (c(i,0,0,0)*ui(i1,i2+1,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+4,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+1,
     & i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+4,c3))+c(i,2,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+2,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+4,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+2,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+4,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+2,i3+4,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+3,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+4,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+3,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+4,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+3,i3+4,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+4,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+4,
     & i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+4,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+4,i3+4,c3)))
             else if( varWidth(i).eq.4 )then
! interpTensorProduct444(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+0,
     & c3)+c(i,3,0,0)*ui(i1+3,i2,i3+0,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(
     & i1,i2+1,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+0,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+1,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+0,c3))+c(i,2,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+0,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+0,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+0,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+3,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+0,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+3,i3+0,c3)))+c(i,1,2,0)*(c(i,0,1,0)*(c(i,
     & 0,0,0)*ui(i1,i2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+1,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+1,c3))+c(i,1,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+1,
     & i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+1,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+1,i3+1,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+1,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+1,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+2,i3+1,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+3,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+1,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+3,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+1,c3)))+c(i,2,2,
     & 0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+2,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+2,c3)+c(i,3,0,0)*ui(i1+
     & 3,i2,i3+2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+2,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+1,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+1,i3+2,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+2,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+2,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+2,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+2,c3))+c(i,3,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+3,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+2,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+3,
     & i3+2,c3)))+c(i,3,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+3,c3)
     & +c(i,1,0,0)*ui(i1+1,i2,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+3,c3)+
     & c(i,3,0,0)*ui(i1+3,i2,i3+3,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+1,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+3,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+1,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+3,c3))+c(i,2,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+2,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+3,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+2,
     & i3+3,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+3,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+3,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+3,i3+3,c3)))
             else if( varWidth(i).eq.7 )then
! interpTensorProduct777(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+0,
     & c3)+c(i,3,0,0)*ui(i1+3,i2,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+0,
     & c3)+c(i,5,0,0)*ui(i1+5,i2,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+0,
     & c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+0,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+1,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+0,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+1,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+0,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+1,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+0,c3))+c(i,
     & 2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+0,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+0,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+2,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+0,c3))+c(i,3,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+3,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+0,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+3,
     & i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+0,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+3,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+0,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+0,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+0,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+4,
     & i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+0,c3))+c(i,5,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+5,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+0,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+5,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+0,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+5,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+0,c3)
     & +c(i,6,0,0)*ui(i1+6,i2+5,i3+0,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(
     & i1,i2+6,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+0,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+6,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+0,c3)+c(i,4,0,
     & 0)*ui(i1+4,i2+6,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+0,c3)+c(i,
     & 6,0,0)*ui(i1+6,i2+6,i3+0,c3)))+c(i,1,2,0)*(c(i,0,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+1,c3)+c(i,2,0,0)*
     & ui(i1+2,i2,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+1,c3)+c(i,4,0,0)*
     & ui(i1+4,i2,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+1,c3)+c(i,6,0,0)*
     & ui(i1+6,i2,i3+1,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+1,c3)
     & +c(i,1,0,0)*ui(i1+1,i2+1,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+1,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+1,
     & i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+1,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+1,i3+1,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+1,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+1,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+2,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+1,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+2,
     & i3+1,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+1,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+3,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+1,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+3,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+1,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+3,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+1,c3)
     & )+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+1,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2+4,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+1,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+4,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+1,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+4,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+1,c3))+c(i,5,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+5,
     & i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+1,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+5,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+1,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+5,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+1,c3))+c(i,6,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+6,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+1,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+6,
     & i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+1,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+6,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+1,c3)))+c(i,2,2,0)*(c(
     & i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2,
     & i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2,
     & i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+2,c3))+c(i,1,1,0)*(c(i,0,0,0)
     & *ui(i1,i2+1,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+2,c3)+c(i,2,0,
     & 0)*ui(i1+2,i2+1,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+2,c3)+c(i,
     & 4,0,0)*ui(i1+4,i2+1,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+2,c3)+
     & c(i,6,0,0)*ui(i1+6,i2+1,i3+2,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+2,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+2,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+2,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+2,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+2,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+2,c3)+c(i,6,0,
     & 0)*ui(i1+6,i2+2,i3+2,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+
     & 2,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+3,
     & i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+2,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+3,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+2,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+3,i3+2,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+2,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+4,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+2,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,
     & i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+2,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+4,i3+2,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+2,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+5,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+5,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+2,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+5,
     & i3+2,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+2,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+6,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+2,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+6,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+2,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+6,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+2,c3)
     & ))
               r(i) = r(i)+c(i,3,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,
     & i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2,
     & i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2,
     & i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2,
     & i3+3,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+3,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+1,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+1,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+3,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+1,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+3,c3)
     & )+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+3,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2+2,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+3,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+2,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+3,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+2,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+3,c3))+c(i,3,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+3,
     & i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+3,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+3,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+3,c3))+c(i,4,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+4,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+3,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+4,
     & i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+3,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+4,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+3,c3))+c(i,5,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+5,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+5,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+5,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+3,c3))+c(i,6,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+6,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+3,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+6,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+3,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+6,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+3,c3)
     & +c(i,6,0,0)*ui(i1+6,i2+6,i3+3,c3)))+c(i,4,2,0)*(c(i,0,1,0)*(c(
     & i,0,0,0)*ui(i1,i2,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+4,c3)+c(i,
     & 2,0,0)*ui(i1+2,i2,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+4,c3)+c(i,
     & 4,0,0)*ui(i1+4,i2,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+4,c3)+c(i,
     & 6,0,0)*ui(i1+6,i2,i3+4,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,
     & i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+4,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+1,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+4,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+1,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+4,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+1,i3+4,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+4,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+2,
     & i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+2,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+4,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+2,i3+4,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+4,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+3,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+4,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+3,
     & i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+4,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+3,i3+4,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+4,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+4,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+4,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+4,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+4,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+4,
     & i3+4,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+4,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+5,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+4,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+5,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+4,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+5,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+4,c3)
     & )+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+4,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2+6,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+4,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+6,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+4,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+6,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+4,c3)))+c(i,5,
     & 2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+5,c3)+c(i,1,0,0)*ui(
     & i1+1,i2,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+5,c3)+c(i,3,0,0)*ui(
     & i1+3,i2,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+5,c3)+c(i,5,0,0)*ui(
     & i1+5,i2,i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+5,c3))+c(i,1,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+1,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+1,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+1,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+5,c3))+c(i,2,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+2,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+5,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+2,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+5,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+2,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+5,c3)
     & +c(i,6,0,0)*ui(i1+6,i2+2,i3+5,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(
     & i1,i2+3,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+5,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+3,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+5,c3)+c(i,4,0,
     & 0)*ui(i1+4,i2+3,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+5,c3)+c(i,
     & 6,0,0)*ui(i1+6,i2+3,i3+5,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 4,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+5,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+4,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+5,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+4,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+5,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+4,i3+5,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+5,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+5,
     & i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+5,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+5,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+5,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+5,i3+5,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+5,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+6,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+5,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+6,
     & i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+5,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+6,i3+5,c3)))
               r(i) = r(i)+c(i,6,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,
     & i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2,
     & i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2,
     & i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2,
     & i3+6,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+6,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+1,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+6,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+1,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+6,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+1,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+6,c3)
     & )+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+6,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2+2,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+6,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+2,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+6,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+2,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+6,c3))+c(i,3,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+3,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+3,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+6,c3))+c(i,4,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+4,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+6,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+4,
     & i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+6,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+4,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+6,c3))+c(i,5,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+5,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+6,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+5,i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+6,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2+5,
     & i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+6,c3))+c(i,6,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+6,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+6,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+6,i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+6,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+6,i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+6,c3)
     & +c(i,6,0,0)*ui(i1+6,i2+6,i3+6,c3)))
             else if( varWidth(i).eq.6 )then
! interpTensorProduct666(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+0,
     & c3)+c(i,3,0,0)*ui(i1+3,i2,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+0,
     & c3)+c(i,5,0,0)*ui(i1+5,i2,i3+0,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(
     & i1,i2+1,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+0,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+1,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+0,c3)+c(i,4,0,
     & 0)*ui(i1+4,i2+1,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+0,c3))+c(i,
     & 2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+0,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+0,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+2,i3+0,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+0,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+3,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+0,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+3,
     & i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+0,c3))+c(i,4,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+4,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+0,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+4,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+0,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+4,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+0,c3)
     & )+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+0,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2+5,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+0,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+5,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+0,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+5,i3+0,c3)))+c(i,1,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(
     & i1,i2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+1,c3)+c(i,2,0,0)*ui(i1+
     & 2,i2,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+1,c3)+c(i,4,0,0)*ui(i1+
     & 4,i2,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+1,c3))+c(i,1,1,0)*(c(i,
     & 0,0,0)*ui(i1,i2+1,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+1,c3)+c(
     & i,2,0,0)*ui(i1+2,i2+1,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+1,c3)
     & +c(i,4,0,0)*ui(i1+4,i2+1,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+1,
     & c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+1,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+2,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+1,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+2,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+1,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+2,i3+1,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+
     & 1,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+3,
     & i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+1,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+3,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+1,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+1,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+1,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+4,
     & i3+1,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+1,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+5,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+1,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+5,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+1,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+5,i3+1,c3)))+c(i,2,2,0)*(c(i,0,1,0)*(c(i,0,
     & 0,0)*ui(i1,i2,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,
     & 0)*ui(i1+2,i2,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+2,c3)+c(i,4,0,
     & 0)*ui(i1+4,i2,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+2,c3))+c(i,1,1,
     & 0)*(c(i,0,0,0)*ui(i1,i2+1,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+
     & 2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,
     & i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+2,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+1,i3+2,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+2,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+2,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+2,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+2,c3))+c(i,3,1,0)*(c(i,0,0,0)*
     & ui(i1,i2+3,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+2,c3)+c(i,2,0,0)
     & *ui(i1+2,i2+3,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+2,c3)+c(i,4,
     & 0,0)*ui(i1+4,i2+3,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+2,c3))+c(
     & i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+
     & 4,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+4,i3+2,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+2,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+5,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+2,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+5,
     & i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+2,c3)))+c(i,3,2,0)*(c(i,0,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+3,
     & c3)+c(i,2,0,0)*ui(i1+2,i2,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+3,
     & c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+3,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+1,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+1,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+3,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+1,i3+3,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+
     & 3,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+2,
     & i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+3,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+2,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+3,c3))+c(i,3,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+3,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+3,
     & i3+3,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+3,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+4,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+4,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+3,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+4,i3+3,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+5,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+3,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+5,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+3,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+5,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+3,c3)))
               r(i) = r(i)+c(i,4,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,
     & i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2,
     & i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2,
     & i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+4,c3))+c(i,1,1,0)*(c(i,0,0,0)
     & *ui(i1,i2+1,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+4,c3)+c(i,2,0,
     & 0)*ui(i1+2,i2+1,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+4,c3)+c(i,
     & 4,0,0)*ui(i1+4,i2+1,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+4,c3))+
     & c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+4,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+2,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+4,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+4,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+2,i3+4,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+4,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+3,
     & i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+4,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+3,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+4,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+4,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+4,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+4,
     & i3+4,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+4,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+5,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+4,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+5,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+4,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+5,i3+4,c3)))+c(i,5,2,0)*(c(i,0,1,0)*(c(i,0,
     & 0,0)*ui(i1,i2,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+5,c3)+c(i,2,0,
     & 0)*ui(i1+2,i2,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+5,c3)+c(i,4,0,
     & 0)*ui(i1+4,i2,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+5,c3))+c(i,1,1,
     & 0)*(c(i,0,0,0)*ui(i1,i2+1,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+
     & 5,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+1,
     & i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+5,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+1,i3+5,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+5,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+2,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+5,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+2,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+5,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+5,c3))+c(i,3,1,0)*(c(i,0,0,0)*
     & ui(i1,i2+3,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+5,c3)+c(i,2,0,0)
     & *ui(i1+2,i2+3,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+5,c3)+c(i,4,
     & 0,0)*ui(i1+4,i2+3,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+5,c3))+c(
     & i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+
     & 4,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+5,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+5,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+4,i3+5,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+5,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+5,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+5,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+5,
     & i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+5,c3)))
             else if( varWidth(i).eq.9 )then
! interpTensorProduct999(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+0,
     & c3)+c(i,3,0,0)*ui(i1+3,i2,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+0,
     & c3)+c(i,5,0,0)*ui(i1+5,i2,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+0,
     & c3)+c(i,7,0,0)*ui(i1+7,i2,i3+0,c3)+c(i,8,0,0)*ui(i1+8,i2,i3+0,
     & c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+0,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+1,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+0,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+1,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+0,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+1,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+0,c3)+c(i,
     & 7,0,0)*ui(i1+7,i2+1,i3+0,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+0,c3))+
     & c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+0,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+2,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+0,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+2,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+0,c3)+c(i,7,0,
     & 0)*ui(i1+7,i2+2,i3+0,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+0,c3))+c(i,
     & 3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+3,
     & i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+0,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+0,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+3,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+0,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+3,i3+0,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+0,c3))+c(i,4,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+4,
     & i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+0,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+0,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+4,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+0,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+4,i3+0,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+0,c3))+c(i,5,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+5,
     & i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+0,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+5,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+0,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+5,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+0,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+5,i3+0,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+0,c3))+c(i,6,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+6,
     & i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+0,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+6,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+0,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+6,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+0,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+6,i3+0,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+0,c3))+c(i,7,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+7,
     & i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+0,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+7,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+0,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+7,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+0,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+7,i3+0,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+0,c3))+c(i,8,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+8,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+8,
     & i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+8,i3+0,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+8,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+0,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+8,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+0,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+8,i3+0,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+0,c3)))+c(i,1,
     & 2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+1,c3)+c(i,1,0,0)*ui(
     & i1+1,i2,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+1,c3)+c(i,3,0,0)*ui(
     & i1+3,i2,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+1,c3)+c(i,5,0,0)*ui(
     & i1+5,i2,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+1,c3)+c(i,7,0,0)*ui(
     & i1+7,i2,i3+1,c3)+c(i,8,0,0)*ui(i1+8,i2,i3+1,c3))+c(i,1,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+1,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+1,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+1,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+1,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+1,
     & i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+1,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+1,i3+1,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+1,c3))+c(i,2,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+1,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+2,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+1,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+2,
     & i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+1,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+2,i3+1,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+1,c3))+c(i,3,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+3,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+1,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+3,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+1,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+3,
     & i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+1,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+3,i3+1,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+1,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+1,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+1,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+4,
     & i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+1,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+4,i3+1,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+1,c3))+c(i,5,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+5,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+1,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+5,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+1,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+5,
     & i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+1,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+5,i3+1,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+1,c3))+c(i,6,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+6,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+1,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+6,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+1,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+6,
     & i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+1,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+6,i3+1,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+1,c3))+c(i,7,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+7,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+1,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+7,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+1,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+1,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+7,i3+1,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+1,c3))+c(i,8,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+8,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+8,i3+1,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+8,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+8,i3+1,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+8,
     & i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+1,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+8,i3+1,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+1,c3)))
               r(i) = r(i)+c(i,2,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,
     & i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2,
     & i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2,
     & i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2,
     & i3+2,c3)+c(i,7,0,0)*ui(i1+7,i2,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2,
     & i3+2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+2,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+1,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+2,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+1,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+2,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+1,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+2,c3)
     & +c(i,7,0,0)*ui(i1+7,i2+1,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+2,
     & c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+2,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+2,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+2,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+2,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+2,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+2,c3)+c(i,
     & 7,0,0)*ui(i1+7,i2+2,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+2,c3))+
     & c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+2,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+3,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+2,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+3,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+2,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+3,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+2,c3)+c(i,7,0,
     & 0)*ui(i1+7,i2+3,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+2,c3))+c(i,
     & 4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+4,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+4,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+4,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+2,c3))+c(i,5,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+5,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+5,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+5,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+5,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+2,c3))+c(i,6,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+6,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+6,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+6,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+6,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+2,c3))+c(i,7,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+7,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+7,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+7,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+7,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+2,c3))+c(i,8,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+8,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+8,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+8,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+8,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+8,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+8,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+2,c3)))+c(i,3,
     & 2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+3,c3)+c(i,1,0,0)*ui(
     & i1+1,i2,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+3,c3)+c(i,3,0,0)*ui(
     & i1+3,i2,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+3,c3)+c(i,5,0,0)*ui(
     & i1+5,i2,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+3,c3)+c(i,7,0,0)*ui(
     & i1+7,i2,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2,i3+3,c3))+c(i,1,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+1,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+1,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+1,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+3,c3))+c(i,2,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+2,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+2,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+2,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+2,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+3,c3))+c(i,3,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+3,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+3,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+3,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+3,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+4,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+4,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+3,c3))+c(i,5,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+5,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+5,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+5,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+5,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+3,c3))+c(i,6,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+6,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+6,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+6,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+6,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+3,c3))+c(i,7,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+7,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+7,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+7,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+3,c3))+c(i,8,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+8,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+8,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+8,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+8,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+8,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+8,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+3,c3)))
               r(i) = r(i)+c(i,4,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,
     & i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2,
     & i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2,
     & i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2,
     & i3+4,c3)+c(i,7,0,0)*ui(i1+7,i2,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2,
     & i3+4,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+4,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+1,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+4,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+1,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+4,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+1,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+4,c3)
     & +c(i,7,0,0)*ui(i1+7,i2+1,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+4,
     & c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+4,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+2,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+4,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+4,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+2,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+4,c3)+c(i,
     & 7,0,0)*ui(i1+7,i2+2,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+4,c3))+
     & c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+4,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+3,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+4,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+3,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+4,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+3,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+4,c3)+c(i,7,0,
     & 0)*ui(i1+7,i2+3,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+4,c3))+c(i,
     & 4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+4,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+4,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+4,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+4,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+4,c3))+c(i,5,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+5,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+5,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+5,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+4,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+5,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+4,c3))+c(i,6,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+6,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+6,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+6,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+4,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+6,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+4,c3))+c(i,7,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+7,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+7,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+7,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+4,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+7,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+4,c3))+c(i,8,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+8,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+8,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+8,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+8,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+8,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+4,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+8,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+4,c3)))+c(i,5,
     & 2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+5,c3)+c(i,1,0,0)*ui(
     & i1+1,i2,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+5,c3)+c(i,3,0,0)*ui(
     & i1+3,i2,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+5,c3)+c(i,5,0,0)*ui(
     & i1+5,i2,i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+5,c3)+c(i,7,0,0)*ui(
     & i1+7,i2,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2,i3+5,c3))+c(i,1,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+1,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+1,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+1,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+1,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+5,c3))+c(i,2,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+2,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+2,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+2,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+2,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+5,c3))+c(i,3,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+3,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+3,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+3,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+3,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+5,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+4,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+4,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+5,c3))+c(i,5,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+5,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+5,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+5,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+5,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+5,c3))+c(i,6,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+6,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+6,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+6,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+6,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+5,c3))+c(i,7,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+7,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+7,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+7,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+5,c3))+c(i,8,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+8,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+8,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+8,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+8,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+8,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+8,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+5,c3)))
               r(i) = r(i)+c(i,6,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,
     & i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2,
     & i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2,
     & i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2,
     & i3+6,c3)+c(i,7,0,0)*ui(i1+7,i2,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2,
     & i3+6,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+6,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+1,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+6,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+1,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+6,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+1,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+6,c3)
     & +c(i,7,0,0)*ui(i1+7,i2+1,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+6,
     & c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+6,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+2,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+6,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+2,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+6,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+2,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+6,c3)+c(i,
     & 7,0,0)*ui(i1+7,i2+2,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+6,c3))+
     & c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+6,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+3,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+6,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+3,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+6,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+3,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+6,c3)+c(i,7,0,
     & 0)*ui(i1+7,i2+3,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+6,c3))+c(i,
     & 4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+4,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+4,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+6,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+4,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+6,c3))+c(i,5,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+5,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+5,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+5,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+6,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+5,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+6,c3))+c(i,6,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+6,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+6,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+6,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+6,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+6,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+6,c3))+c(i,7,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+7,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+7,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+7,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+6,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+7,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+6,c3))+c(i,8,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+8,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+8,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+8,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+8,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+8,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+6,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+8,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+6,c3)))+c(i,7,
     & 2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+7,c3)+c(i,1,0,0)*ui(
     & i1+1,i2,i3+7,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+7,c3)+c(i,3,0,0)*ui(
     & i1+3,i2,i3+7,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+7,c3)+c(i,5,0,0)*ui(
     & i1+5,i2,i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+7,c3)+c(i,7,0,0)*ui(
     & i1+7,i2,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2,i3+7,c3))+c(i,1,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+1,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+1,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+1,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+1,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+7,c3))+c(i,2,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+2,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+2,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+2,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+2,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+7,c3))+c(i,3,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+3,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+3,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+3,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+3,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+7,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+4,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+4,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+7,c3))+c(i,5,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+5,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+5,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+5,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+5,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+7,c3))+c(i,6,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+6,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+6,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+6,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+6,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+7,c3))+c(i,7,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+7,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+7,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+7,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+7,c3))+c(i,8,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+8,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+8,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+8,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+8,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+8,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+8,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+7,c3)))
               r(i) = r(i)+c(i,8,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,
     & i3+8,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2,
     & i3+8,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2,
     & i3+8,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2,
     & i3+8,c3)+c(i,7,0,0)*ui(i1+7,i2,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2,
     & i3+8,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+8,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+1,i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+8,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+1,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+8,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+1,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+8,c3)
     & +c(i,7,0,0)*ui(i1+7,i2+1,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+8,
     & c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+8,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+2,i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+8,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+2,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+8,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+2,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+8,c3)+c(i,
     & 7,0,0)*ui(i1+7,i2+2,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+8,c3))+
     & c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+8,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+3,i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+8,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+3,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+8,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+3,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+8,c3)+c(i,7,0,
     & 0)*ui(i1+7,i2+3,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+8,c3))+c(i,
     & 4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+8,c3)+c(i,1,0,0)*ui(i1+1,i2+4,
     & i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+8,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+8,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+4,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+8,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+4,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+8,c3))+c(i,5,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+8,c3)+c(i,1,0,0)*ui(i1+1,i2+5,
     & i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+8,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+5,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+8,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+5,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+8,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+5,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+8,c3))+c(i,6,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+8,c3)+c(i,1,0,0)*ui(i1+1,i2+6,
     & i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+8,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+6,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+8,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+6,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+8,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+6,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+8,c3))+c(i,7,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+8,c3)+c(i,1,0,0)*ui(i1+1,i2+7,
     & i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+8,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+7,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+8,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+7,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+8,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+7,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+8,c3))+c(i,8,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+8,i3+8,c3)+c(i,1,0,0)*ui(i1+1,i2+8,
     & i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+8,i3+8,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+8,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+8,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+8,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+8,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+8,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+8,c3)))
             else if( varWidth(i).eq.8 )then
! interpTensorProduct888(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+0,
     & c3)+c(i,3,0,0)*ui(i1+3,i2,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+0,
     & c3)+c(i,5,0,0)*ui(i1+5,i2,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+0,
     & c3)+c(i,7,0,0)*ui(i1+7,i2,i3+0,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(
     & i1,i2+1,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+0,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+1,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+0,c3)+c(i,4,0,
     & 0)*ui(i1+4,i2+1,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+0,c3)+c(i,
     & 6,0,0)*ui(i1+6,i2+1,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+1,i3+0,c3))+
     & c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+0,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+2,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+0,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+2,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+0,c3)+c(i,7,0,
     & 0)*ui(i1+7,i2+2,i3+0,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+
     & 0,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+3,
     & i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+0,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+3,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+0,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+3,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+3,i3+0,c3))+c(i,4,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+4,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+0,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+4,
     & i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+0,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+4,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+0,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+4,i3+0,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+0,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+5,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+0,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+5,
     & i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+0,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+5,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+5,i3+0,c3))+c(i,6,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+6,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+0,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+6,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+0,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+6,
     & i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+0,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+6,i3+0,c3))+c(i,7,1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+0,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+7,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+0,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+7,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+0,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+7,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+7,
     & i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+7,i3+0,c3)))+c(i,1,2,0)*(c(i,0,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+1,
     & c3)+c(i,2,0,0)*ui(i1+2,i2,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+1,
     & c3)+c(i,4,0,0)*ui(i1+4,i2,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+1,
     & c3)+c(i,6,0,0)*ui(i1+6,i2,i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2,i3+1,
     & c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+1,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+1,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+1,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+1,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+1,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+1,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+1,c3)+c(i,
     & 7,0,0)*ui(i1+7,i2+1,i3+1,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+2,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+1,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+2,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+1,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+2,i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+2,i3+1,c3))+c(i,3,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+3,
     & i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+1,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+1,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+3,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+1,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+3,i3+1,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+1,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+4,
     & i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+1,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+4,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+1,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+4,i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+4,i3+1,c3))+c(i,5,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+5,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+1,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+5,
     & i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+1,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+5,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+1,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+5,i3+1,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+1,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+6,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+1,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+6,
     & i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+1,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+6,i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+6,i3+1,c3))+c(i,7,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+7,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+1,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+7,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+1,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+1,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+7,i3+1,c3)))
               r(i) = r(i)+c(i,2,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,
     & i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2,
     & i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2,
     & i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2,
     & i3+2,c3)+c(i,7,0,0)*ui(i1+7,i2,i3+2,c3))+c(i,1,1,0)*(c(i,0,0,0)
     & *ui(i1,i2+1,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+2,c3)+c(i,2,0,
     & 0)*ui(i1+2,i2+1,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+2,c3)+c(i,
     & 4,0,0)*ui(i1+4,i2+1,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+2,c3)+
     & c(i,6,0,0)*ui(i1+6,i2+1,i3+2,c3)+c(i,7,0,0)*ui(i1+7,i2+1,i3+2,
     & c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+2,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+2,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+2,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+2,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+2,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+2,c3)+c(i,
     & 7,0,0)*ui(i1+7,i2+2,i3+2,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 3,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+2,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+3,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+2,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+3,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+2,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+3,i3+2,c3)+c(i,7,0,0)*ui(i1+7,i2+3,i3+2,c3))+c(i,4,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+4,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+4,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+4,i3+2,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+2,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+5,
     & i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+2,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+5,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+2,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+5,i3+2,c3)+c(i,7,0,0)*ui(i1+7,i2+5,i3+2,c3))+c(i,6,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+6,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+2,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+6,
     & i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+2,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+6,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+2,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+6,i3+2,c3))+c(i,7,1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+2,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+7,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+2,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+7,
     & i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+7,i3+2,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+7,i3+2,c3)+c(i,7,0,0)*ui(i1+7,i2+7,i3+2,c3)))+c(i,3,2,0)*(c(
     & i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2,
     & i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2,
     & i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+3,c3)+c(i,7,0,0)*ui(i1+7,i2,
     & i3+3,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+3,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+1,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+1,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+3,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+1,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+3,c3)
     & +c(i,7,0,0)*ui(i1+7,i2+1,i3+3,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(
     & i1,i2+2,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+3,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+2,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+3,c3)+c(i,4,0,
     & 0)*ui(i1+4,i2+2,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+3,c3)+c(i,
     & 6,0,0)*ui(i1+6,i2+2,i3+3,c3)+c(i,7,0,0)*ui(i1+7,i2+2,i3+3,c3))+
     & c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+3,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+3,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+3,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+3,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+3,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+3,c3)+c(i,7,0,
     & 0)*ui(i1+7,i2+3,i3+3,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+
     & 3,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+4,
     & i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+3,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+4,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+3,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+4,i3+3,c3)+c(i,7,0,0)*ui(i1+7,i2+4,i3+3,c3))+c(i,5,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+5,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+3,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+5,
     & i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+3,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+5,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+3,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+5,i3+3,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+3,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+6,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+3,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+6,
     & i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+3,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+6,i3+3,c3)+c(i,7,0,0)*ui(i1+7,i2+6,i3+3,c3))+c(i,7,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+7,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+7,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+7,i3+3,c3)))
               r(i) = r(i)+c(i,4,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,
     & i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2,
     & i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2,
     & i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2,
     & i3+4,c3)+c(i,7,0,0)*ui(i1+7,i2,i3+4,c3))+c(i,1,1,0)*(c(i,0,0,0)
     & *ui(i1,i2+1,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+4,c3)+c(i,2,0,
     & 0)*ui(i1+2,i2+1,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+4,c3)+c(i,
     & 4,0,0)*ui(i1+4,i2+1,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+4,c3)+
     & c(i,6,0,0)*ui(i1+6,i2+1,i3+4,c3)+c(i,7,0,0)*ui(i1+7,i2+1,i3+4,
     & c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+4,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+2,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+4,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+4,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+2,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+4,c3)+c(i,
     & 7,0,0)*ui(i1+7,i2+2,i3+4,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 3,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+4,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+3,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+4,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+3,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+4,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+3,i3+4,c3)+c(i,7,0,0)*ui(i1+7,i2+3,i3+4,c3))+c(i,4,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+4,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+4,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+4,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+4,i3+4,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+4,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+5,
     & i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+4,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+5,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+4,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+5,i3+4,c3)+c(i,7,0,0)*ui(i1+7,i2+5,i3+4,c3))+c(i,6,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+6,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+4,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+6,
     & i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+4,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+6,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+4,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+6,i3+4,c3))+c(i,7,1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+4,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+7,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+4,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+7,
     & i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+7,i3+4,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+7,i3+4,c3)+c(i,7,0,0)*ui(i1+7,i2+7,i3+4,c3)))+c(i,5,2,0)*(c(
     & i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2,
     & i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2,
     & i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+5,c3)+c(i,7,0,0)*ui(i1+7,i2,
     & i3+5,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+5,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+1,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+5,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+1,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+5,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+1,i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+5,c3)
     & +c(i,7,0,0)*ui(i1+7,i2+1,i3+5,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(
     & i1,i2+2,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+5,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+2,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+5,c3)+c(i,4,0,
     & 0)*ui(i1+4,i2+2,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+5,c3)+c(i,
     & 6,0,0)*ui(i1+6,i2+2,i3+5,c3)+c(i,7,0,0)*ui(i1+7,i2+2,i3+5,c3))+
     & c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+5,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+3,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+5,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+3,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+5,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+3,i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+5,c3)+c(i,7,0,
     & 0)*ui(i1+7,i2+3,i3+5,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+
     & 5,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+4,
     & i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+5,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+4,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+5,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+4,i3+5,c3)+c(i,7,0,0)*ui(i1+7,i2+4,i3+5,c3))+c(i,5,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+5,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+5,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+5,
     & i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+5,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+5,i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+5,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+5,i3+5,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+5,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+6,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+5,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+6,
     & i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+5,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+6,i3+5,c3)+c(i,7,0,0)*ui(i1+7,i2+6,i3+5,c3))+c(i,7,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+7,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+7,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+7,i3+5,c3)))
               r(i) = r(i)+c(i,6,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,
     & i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2,
     & i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2,
     & i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2,
     & i3+6,c3)+c(i,7,0,0)*ui(i1+7,i2,i3+6,c3))+c(i,1,1,0)*(c(i,0,0,0)
     & *ui(i1,i2+1,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+6,c3)+c(i,2,0,
     & 0)*ui(i1+2,i2+1,i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+6,c3)+c(i,
     & 4,0,0)*ui(i1+4,i2+1,i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+6,c3)+
     & c(i,6,0,0)*ui(i1+6,i2+1,i3+6,c3)+c(i,7,0,0)*ui(i1+7,i2+1,i3+6,
     & c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+6,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+2,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+6,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+2,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+6,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+2,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+6,c3)+c(i,
     & 7,0,0)*ui(i1+7,i2+2,i3+6,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 3,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+6,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+3,i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+6,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+3,i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+6,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+3,i3+6,c3)+c(i,7,0,0)*ui(i1+7,i2+3,i3+6,c3))+c(i,4,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+4,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+4,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+6,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+4,i3+6,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+6,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+5,
     & i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+6,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+5,i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+6,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+5,i3+6,c3)+c(i,7,0,0)*ui(i1+7,i2+5,i3+6,c3))+c(i,6,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+6,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+6,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+6,
     & i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+6,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+6,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+6,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+6,i3+6,c3))+c(i,7,1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+6,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+7,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+6,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+7,
     & i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2+7,i3+6,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+7,i3+6,c3)+c(i,7,0,0)*ui(i1+7,i2+7,i3+6,c3)))+c(i,7,2,0)*(c(
     & i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2,
     & i3+7,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2,
     & i3+7,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+7,c3)+c(i,7,0,0)*ui(i1+7,i2,
     & i3+7,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+7,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+1,i3+7,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+7,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+1,i3+7,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+7,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+1,i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+7,c3)
     & +c(i,7,0,0)*ui(i1+7,i2+1,i3+7,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(
     & i1,i2+2,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+7,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+2,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+7,c3)+c(i,4,0,
     & 0)*ui(i1+4,i2+2,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+7,c3)+c(i,
     & 6,0,0)*ui(i1+6,i2+2,i3+7,c3)+c(i,7,0,0)*ui(i1+7,i2+2,i3+7,c3))+
     & c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+7,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+3,i3+7,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+7,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+3,i3+7,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+7,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+3,i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+7,c3)+c(i,7,0,
     & 0)*ui(i1+7,i2+3,i3+7,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+
     & 7,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+7,c3)+c(i,2,0,0)*ui(i1+2,i2+4,
     & i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+7,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+4,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+7,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+4,i3+7,c3)+c(i,7,0,0)*ui(i1+7,i2+4,i3+7,c3))+c(i,5,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+5,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+7,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+5,
     & i3+7,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+7,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+5,i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+7,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+5,i3+7,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+7,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+6,i3+7,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+7,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+7,c3)+c(i,4,0,0)*ui(i1+4,i2+6,
     & i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+7,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+6,i3+7,c3)+c(i,7,0,0)*ui(i1+7,i2+6,i3+7,c3))+c(i,7,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+7,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+7,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+7,i3+7,c3)))
             else
               write(*,*) 'ERROR varWidth=',varWidth(i)
               stop 151
             end if
             resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))
     & )
             ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i)
! endLoops3d()
             end do
             end do
         else if( width(1).eq.3 .and. width(2).eq.3 .and. width(3)
     & .eq.3 )then
! loops3d($interpTensorProduct333(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))
           if( c3a.eq.c3b )then
             do c3=c3a,c3b
             do i=nia,nib
! interpTensorProduct333(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+0,
     & c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+0,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+1,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+0,c3))+c(i,2,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+0,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+0,c3)))+c(i,1,2,0)*(c(i,0,1,0)*(
     & c(i,0,0,0)*ui(i1,i2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+1,c3)+c(
     & i,2,0,0)*ui(i1+2,i2,i3+1,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 1,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+1,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+1,i3+1,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+1,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+1,c3)))
     & +c(i,2,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+2,c3)+c(i,1,0,
     & 0)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+2,c3))+c(i,1,1,
     & 0)*(c(i,0,0,0)*ui(i1,i2+1,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+
     & 2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+2,c3))+c(i,2,1,0)*(c(i,0,0,0)*
     & ui(i1,i2+2,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+2,c3)+c(i,2,0,0)
     & *ui(i1+2,i2+2,i3+2,c3)))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
! interpTensorProduct333(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+0,
     & c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+0,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+1,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+0,c3))+c(i,2,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+0,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+0,c3)))+c(i,1,2,0)*(c(i,0,1,0)*(
     & c(i,0,0,0)*ui(i1,i2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+1,c3)+c(
     & i,2,0,0)*ui(i1+2,i2,i3+1,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 1,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+1,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+1,i3+1,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+1,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+1,c3)))
     & +c(i,2,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+2,c3)+c(i,1,0,
     & 0)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+2,c3))+c(i,1,1,
     & 0)*(c(i,0,0,0)*ui(i1,i2+1,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+
     & 2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+2,c3))+c(i,2,1,0)*(c(i,0,0,0)*
     & ui(i1,i2+2,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+2,c3)+c(i,2,0,0)
     & *ui(i1+2,i2+2,i3+2,c3)))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           end if
         else if( width(1).eq.2 .and. width(2).eq.2 .and. width(3)
     & .eq.2 )then
! loops3d($interpTensorProduct222(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))
           if( c3a.eq.c3b )then
             do c3=c3a,c3b
             do i=nia,nib
! interpTensorProduct222(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(
     & i1,i2+1,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+0,c3)))+c(i,1,2,0)*
     & (c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,
     & i2,i3+1,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+1,c3)+c(i,1,
     & 0,0)*ui(i1+1,i2+1,i3+1,c3)))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
! interpTensorProduct222(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(
     & i1,i2+1,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+0,c3)))+c(i,1,2,0)*
     & (c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,
     & i2,i3+1,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+1,c3)+c(i,1,
     & 0,0)*ui(i1+1,i2+1,i3+1,c3)))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           end if
         else if( width(1).eq.4 .and. width(2).eq.4 .and. width(3)
     & .eq.4 )then
! loops3d($interpTensorProduct444(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))
           if( c3a.eq.c3b )then
             do c3=c3a,c3b
             do i=nia,nib
! interpTensorProduct444(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+0,
     & c3)+c(i,3,0,0)*ui(i1+3,i2,i3+0,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(
     & i1,i2+1,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+0,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+1,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+0,c3))+c(i,2,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+0,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+0,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+0,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+3,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+0,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+3,i3+0,c3)))+c(i,1,2,0)*(c(i,0,1,0)*(c(i,
     & 0,0,0)*ui(i1,i2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+1,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+1,c3))+c(i,1,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+1,
     & i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+1,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+1,i3+1,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+1,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+1,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+2,i3+1,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+3,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+1,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+3,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+1,c3)))+c(i,2,2,
     & 0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+2,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+2,c3)+c(i,3,0,0)*ui(i1+
     & 3,i2,i3+2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+2,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+1,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+1,i3+2,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+2,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+2,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+2,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+2,c3))+c(i,3,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+3,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+2,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+3,
     & i3+2,c3)))+c(i,3,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+3,c3)
     & +c(i,1,0,0)*ui(i1+1,i2,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+3,c3)+
     & c(i,3,0,0)*ui(i1+3,i2,i3+3,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+1,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+3,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+1,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+3,c3))+c(i,2,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+2,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+3,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+2,
     & i3+3,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+3,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+3,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+3,i3+3,c3)))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
! interpTensorProduct444(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+0,
     & c3)+c(i,3,0,0)*ui(i1+3,i2,i3+0,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(
     & i1,i2+1,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+0,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+1,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+0,c3))+c(i,2,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+0,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+0,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+0,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+3,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+0,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+3,i3+0,c3)))+c(i,1,2,0)*(c(i,0,1,0)*(c(i,
     & 0,0,0)*ui(i1,i2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+1,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+1,c3))+c(i,1,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+1,
     & i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+1,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+1,i3+1,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+1,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+1,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+2,i3+1,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+3,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+1,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+3,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+1,c3)))+c(i,2,2,
     & 0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+2,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+2,c3)+c(i,3,0,0)*ui(i1+
     & 3,i2,i3+2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+2,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+1,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+1,i3+2,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+2,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+2,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+2,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+2,c3))+c(i,3,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+3,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+2,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+3,
     & i3+2,c3)))+c(i,3,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+3,c3)
     & +c(i,1,0,0)*ui(i1+1,i2,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+3,c3)+
     & c(i,3,0,0)*ui(i1+3,i2,i3+3,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+1,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+3,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+1,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+3,c3))+c(i,2,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+2,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+3,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+2,
     & i3+3,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+3,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+3,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+3,i3+3,c3)))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           end if
         else if( width(1).eq.5 .and. width(2).eq.5 .and. width(3)
     & .eq.5 )then
! loops3d($interpTensorProduct555(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))
           if( c3a.eq.c3b )then
             do c3=c3a,c3b
             do i=nia,nib
! interpTensorProduct555(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+0,
     & c3)+c(i,3,0,0)*ui(i1+3,i2,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+0,
     & c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+0,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+1,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+0,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+1,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+0,c3))+c(i,2,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+0,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+0,c3))+c(i,3,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+3,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+0,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+3,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+0,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+0,c3))+c(i,4,1,0)*(c(i,0,0,0)*
     & ui(i1,i2+4,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+0,c3)+c(i,2,0,0)
     & *ui(i1+2,i2+4,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+0,c3)+c(i,4,
     & 0,0)*ui(i1+4,i2+4,i3+0,c3)))+c(i,1,2,0)*(c(i,0,1,0)*(c(i,0,0,0)
     & *ui(i1,i2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+1,c3)+c(i,2,0,0)*
     & ui(i1+2,i2,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+1,c3)+c(i,4,0,0)*
     & ui(i1+4,i2,i3+1,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+1,c3)
     & +c(i,1,0,0)*ui(i1+1,i2+1,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+1,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+1,
     & i3+1,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+1,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+2,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+1,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+2,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+1,c3))+c(
     & i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+
     & 3,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+1,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+1,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+1,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+1,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+1,c3)))+c(i,2,2,0)*(c(i,0,1,0)*(
     & c(i,0,0,0)*ui(i1,i2,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+2,c3)+c(
     & i,2,0,0)*ui(i1+2,i2,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+2,c3)+c(
     & i,4,0,0)*ui(i1+4,i2,i3+2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 1,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+2,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+1,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+2,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+1,i3+2,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+2,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+2,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+2,
     & i3+2,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+2,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+3,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+2,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+3,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+2,c3))+c(
     & i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+
     & 4,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+2,c3)))+c(i,3,2,0)*(c(
     & i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2,
     & i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2,
     & i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+3,c3))+c(i,1,1,0)*(c(i,0,0,0)
     & *ui(i1,i2+1,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+3,c3)+c(i,2,0,
     & 0)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+3,c3)+c(i,
     & 4,0,0)*ui(i1+4,i2+1,i3+3,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 2,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+3,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+2,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+3,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+2,i3+3,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+3,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+3,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+3,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+3,
     & i3+3,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+3,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+4,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+4,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+3,c3)))+
     & c(i,4,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+4,c3)+c(i,1,0,0)
     & *ui(i1+1,i2,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+4,c3)+c(i,3,0,0)*
     & ui(i1+3,i2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+4,c3))+c(i,1,1,0)*
     & (c(i,0,0,0)*ui(i1,i2+1,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+4,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+1,
     & i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+4,c3))+c(i,2,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+2,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+4,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+2,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+4,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+2,i3+4,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+3,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+4,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+3,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+4,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+3,i3+4,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+4,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+4,
     & i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+4,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+4,i3+4,c3)))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
! interpTensorProduct555(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+0,
     & c3)+c(i,3,0,0)*ui(i1+3,i2,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+0,
     & c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+0,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+1,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+0,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+1,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+0,c3))+c(i,2,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+0,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+0,c3))+c(i,3,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+3,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+0,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+3,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+0,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+0,c3))+c(i,4,1,0)*(c(i,0,0,0)*
     & ui(i1,i2+4,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+0,c3)+c(i,2,0,0)
     & *ui(i1+2,i2+4,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+0,c3)+c(i,4,
     & 0,0)*ui(i1+4,i2+4,i3+0,c3)))+c(i,1,2,0)*(c(i,0,1,0)*(c(i,0,0,0)
     & *ui(i1,i2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+1,c3)+c(i,2,0,0)*
     & ui(i1+2,i2,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+1,c3)+c(i,4,0,0)*
     & ui(i1+4,i2,i3+1,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+1,c3)
     & +c(i,1,0,0)*ui(i1+1,i2+1,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+1,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+1,
     & i3+1,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+1,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+2,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+1,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+2,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+1,c3))+c(
     & i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+
     & 3,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+1,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+1,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+1,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+1,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+1,c3)))+c(i,2,2,0)*(c(i,0,1,0)*(
     & c(i,0,0,0)*ui(i1,i2,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+2,c3)+c(
     & i,2,0,0)*ui(i1+2,i2,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+2,c3)+c(
     & i,4,0,0)*ui(i1+4,i2,i3+2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 1,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+2,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+1,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+2,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+1,i3+2,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+2,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+2,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+2,
     & i3+2,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+2,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+3,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+2,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+3,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+2,c3))+c(
     & i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+
     & 4,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+2,c3)))+c(i,3,2,0)*(c(
     & i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2,
     & i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2,
     & i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+3,c3))+c(i,1,1,0)*(c(i,0,0,0)
     & *ui(i1,i2+1,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+3,c3)+c(i,2,0,
     & 0)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+3,c3)+c(i,
     & 4,0,0)*ui(i1+4,i2+1,i3+3,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 2,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+3,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+2,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+3,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+2,i3+3,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+3,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+3,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+3,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+3,
     & i3+3,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+3,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+4,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+4,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+3,c3)))+
     & c(i,4,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+4,c3)+c(i,1,0,0)
     & *ui(i1+1,i2,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+4,c3)+c(i,3,0,0)*
     & ui(i1+3,i2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+4,c3))+c(i,1,1,0)*
     & (c(i,0,0,0)*ui(i1,i2+1,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+4,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+1,
     & i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+4,c3))+c(i,2,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+2,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+4,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+2,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+4,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+2,i3+4,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+3,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+4,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+3,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+4,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+3,i3+4,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+4,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+4,
     & i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+4,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+4,i3+4,c3)))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           end if
         else if( width(1).eq.6 .and. width(2).eq.6 .and. width(3)
     & .eq.6 )then
! loops3d($interpTensorProduct666(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))
           if( c3a.eq.c3b )then
             do c3=c3a,c3b
             do i=nia,nib
! interpTensorProduct666(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+0,
     & c3)+c(i,3,0,0)*ui(i1+3,i2,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+0,
     & c3)+c(i,5,0,0)*ui(i1+5,i2,i3+0,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(
     & i1,i2+1,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+0,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+1,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+0,c3)+c(i,4,0,
     & 0)*ui(i1+4,i2+1,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+0,c3))+c(i,
     & 2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+0,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+0,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+2,i3+0,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+0,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+3,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+0,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+3,
     & i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+0,c3))+c(i,4,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+4,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+0,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+4,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+0,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+4,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+0,c3)
     & )+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+0,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2+5,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+0,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+5,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+0,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+5,i3+0,c3)))+c(i,1,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(
     & i1,i2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+1,c3)+c(i,2,0,0)*ui(i1+
     & 2,i2,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+1,c3)+c(i,4,0,0)*ui(i1+
     & 4,i2,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+1,c3))+c(i,1,1,0)*(c(i,
     & 0,0,0)*ui(i1,i2+1,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+1,c3)+c(
     & i,2,0,0)*ui(i1+2,i2+1,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+1,c3)
     & +c(i,4,0,0)*ui(i1+4,i2+1,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+1,
     & c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+1,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+2,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+1,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+2,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+1,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+2,i3+1,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+
     & 1,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+3,
     & i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+1,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+3,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+1,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+1,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+1,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+4,
     & i3+1,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+1,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+5,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+1,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+5,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+1,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+5,i3+1,c3)))+c(i,2,2,0)*(c(i,0,1,0)*(c(i,0,
     & 0,0)*ui(i1,i2,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,
     & 0)*ui(i1+2,i2,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+2,c3)+c(i,4,0,
     & 0)*ui(i1+4,i2,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+2,c3))+c(i,1,1,
     & 0)*(c(i,0,0,0)*ui(i1,i2+1,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+
     & 2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,
     & i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+2,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+1,i3+2,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+2,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+2,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+2,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+2,c3))+c(i,3,1,0)*(c(i,0,0,0)*
     & ui(i1,i2+3,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+2,c3)+c(i,2,0,0)
     & *ui(i1+2,i2+3,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+2,c3)+c(i,4,
     & 0,0)*ui(i1+4,i2+3,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+2,c3))+c(
     & i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+
     & 4,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+4,i3+2,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+2,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+5,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+2,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+5,
     & i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+2,c3)))+c(i,3,2,0)*(c(i,0,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+3,
     & c3)+c(i,2,0,0)*ui(i1+2,i2,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+3,
     & c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+3,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+1,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+1,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+3,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+1,i3+3,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+
     & 3,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+2,
     & i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+3,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+2,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+3,c3))+c(i,3,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+3,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+3,
     & i3+3,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+3,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+4,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+4,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+3,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+4,i3+3,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+5,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+3,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+5,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+3,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+5,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+3,c3)))
               r(i) = r(i)+c(i,4,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,
     & i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2,
     & i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2,
     & i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+4,c3))+c(i,1,1,0)*(c(i,0,0,0)
     & *ui(i1,i2+1,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+4,c3)+c(i,2,0,
     & 0)*ui(i1+2,i2+1,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+4,c3)+c(i,
     & 4,0,0)*ui(i1+4,i2+1,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+4,c3))+
     & c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+4,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+2,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+4,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+4,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+2,i3+4,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+4,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+3,
     & i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+4,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+3,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+4,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+4,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+4,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+4,
     & i3+4,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+4,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+5,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+4,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+5,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+4,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+5,i3+4,c3)))+c(i,5,2,0)*(c(i,0,1,0)*(c(i,0,
     & 0,0)*ui(i1,i2,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+5,c3)+c(i,2,0,
     & 0)*ui(i1+2,i2,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+5,c3)+c(i,4,0,
     & 0)*ui(i1+4,i2,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+5,c3))+c(i,1,1,
     & 0)*(c(i,0,0,0)*ui(i1,i2+1,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+
     & 5,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+1,
     & i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+5,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+1,i3+5,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+5,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+2,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+5,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+2,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+5,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+5,c3))+c(i,3,1,0)*(c(i,0,0,0)*
     & ui(i1,i2+3,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+5,c3)+c(i,2,0,0)
     & *ui(i1+2,i2+3,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+5,c3)+c(i,4,
     & 0,0)*ui(i1+4,i2+3,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+5,c3))+c(
     & i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+
     & 4,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+5,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+5,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+4,i3+5,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+5,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+5,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+5,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+5,
     & i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+5,c3)))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
! interpTensorProduct666(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+0,
     & c3)+c(i,3,0,0)*ui(i1+3,i2,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+0,
     & c3)+c(i,5,0,0)*ui(i1+5,i2,i3+0,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(
     & i1,i2+1,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+0,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+1,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+0,c3)+c(i,4,0,
     & 0)*ui(i1+4,i2+1,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+0,c3))+c(i,
     & 2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+0,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+0,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+2,i3+0,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+0,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+3,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+0,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+3,
     & i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+0,c3))+c(i,4,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+4,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+0,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+4,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+0,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+4,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+0,c3)
     & )+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+0,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2+5,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+0,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+5,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+0,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+5,i3+0,c3)))+c(i,1,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(
     & i1,i2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+1,c3)+c(i,2,0,0)*ui(i1+
     & 2,i2,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+1,c3)+c(i,4,0,0)*ui(i1+
     & 4,i2,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+1,c3))+c(i,1,1,0)*(c(i,
     & 0,0,0)*ui(i1,i2+1,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+1,c3)+c(
     & i,2,0,0)*ui(i1+2,i2+1,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+1,c3)
     & +c(i,4,0,0)*ui(i1+4,i2+1,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+1,
     & c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+1,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+2,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+1,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+2,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+1,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+2,i3+1,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+
     & 1,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+3,
     & i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+1,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+3,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+1,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+1,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+1,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+4,
     & i3+1,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+1,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+5,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+1,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+5,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+1,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+5,i3+1,c3)))+c(i,2,2,0)*(c(i,0,1,0)*(c(i,0,
     & 0,0)*ui(i1,i2,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,
     & 0)*ui(i1+2,i2,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+2,c3)+c(i,4,0,
     & 0)*ui(i1+4,i2,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+2,c3))+c(i,1,1,
     & 0)*(c(i,0,0,0)*ui(i1,i2+1,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+
     & 2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,
     & i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+2,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+1,i3+2,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+2,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+2,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+2,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+2,c3))+c(i,3,1,0)*(c(i,0,0,0)*
     & ui(i1,i2+3,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+2,c3)+c(i,2,0,0)
     & *ui(i1+2,i2+3,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+2,c3)+c(i,4,
     & 0,0)*ui(i1+4,i2+3,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+2,c3))+c(
     & i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+
     & 4,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+4,i3+2,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+2,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+5,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+2,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+5,
     & i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+2,c3)))+c(i,3,2,0)*(c(i,0,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+3,
     & c3)+c(i,2,0,0)*ui(i1+2,i2,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+3,
     & c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+3,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+1,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+1,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+3,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+1,i3+3,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+
     & 3,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+2,
     & i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+3,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+2,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+3,c3))+c(i,3,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+3,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+3,
     & i3+3,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+3,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+4,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+4,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+3,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+4,i3+3,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+5,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+3,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+5,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+3,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+5,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+3,c3)))
               r(i) = r(i)+c(i,4,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,
     & i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2,
     & i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2,
     & i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+4,c3))+c(i,1,1,0)*(c(i,0,0,0)
     & *ui(i1,i2+1,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+4,c3)+c(i,2,0,
     & 0)*ui(i1+2,i2+1,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+4,c3)+c(i,
     & 4,0,0)*ui(i1+4,i2+1,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+4,c3))+
     & c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+4,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+2,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+4,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+4,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+2,i3+4,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+4,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+3,
     & i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+4,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+3,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+4,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+4,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+4,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+4,
     & i3+4,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+4,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+5,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+4,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+5,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+4,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+5,i3+4,c3)))+c(i,5,2,0)*(c(i,0,1,0)*(c(i,0,
     & 0,0)*ui(i1,i2,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+5,c3)+c(i,2,0,
     & 0)*ui(i1+2,i2,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+5,c3)+c(i,4,0,
     & 0)*ui(i1+4,i2,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+5,c3))+c(i,1,1,
     & 0)*(c(i,0,0,0)*ui(i1,i2+1,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+
     & 5,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+1,
     & i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+5,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+1,i3+5,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+5,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+2,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+5,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+2,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+5,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+5,c3))+c(i,3,1,0)*(c(i,0,0,0)*
     & ui(i1,i2+3,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+5,c3)+c(i,2,0,0)
     & *ui(i1+2,i2+3,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+5,c3)+c(i,4,
     & 0,0)*ui(i1+4,i2+3,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+5,c3))+c(
     & i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+
     & 4,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+5,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+5,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+4,i3+5,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+5,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+5,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+5,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+5,
     & i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+5,c3)))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           end if
         else if( width(1).eq.7 .and. width(2).eq.7 .and. width(3)
     & .eq.7 )then
! loops3d($interpTensorProduct777(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))
           if( c3a.eq.c3b )then
             do c3=c3a,c3b
             do i=nia,nib
! interpTensorProduct777(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+0,
     & c3)+c(i,3,0,0)*ui(i1+3,i2,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+0,
     & c3)+c(i,5,0,0)*ui(i1+5,i2,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+0,
     & c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+0,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+1,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+0,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+1,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+0,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+1,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+0,c3))+c(i,
     & 2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+0,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+0,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+2,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+0,c3))+c(i,3,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+3,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+0,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+3,
     & i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+0,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+3,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+0,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+0,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+0,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+4,
     & i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+0,c3))+c(i,5,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+5,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+0,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+5,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+0,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+5,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+0,c3)
     & +c(i,6,0,0)*ui(i1+6,i2+5,i3+0,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(
     & i1,i2+6,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+0,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+6,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+0,c3)+c(i,4,0,
     & 0)*ui(i1+4,i2+6,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+0,c3)+c(i,
     & 6,0,0)*ui(i1+6,i2+6,i3+0,c3)))+c(i,1,2,0)*(c(i,0,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+1,c3)+c(i,2,0,0)*
     & ui(i1+2,i2,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+1,c3)+c(i,4,0,0)*
     & ui(i1+4,i2,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+1,c3)+c(i,6,0,0)*
     & ui(i1+6,i2,i3+1,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+1,c3)
     & +c(i,1,0,0)*ui(i1+1,i2+1,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+1,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+1,
     & i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+1,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+1,i3+1,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+1,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+1,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+2,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+1,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+2,
     & i3+1,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+1,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+3,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+1,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+3,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+1,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+3,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+1,c3)
     & )+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+1,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2+4,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+1,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+4,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+1,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+4,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+1,c3))+c(i,5,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+5,
     & i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+1,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+5,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+1,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+5,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+1,c3))+c(i,6,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+6,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+1,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+6,
     & i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+1,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+6,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+1,c3)))+c(i,2,2,0)*(c(
     & i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2,
     & i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2,
     & i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+2,c3))+c(i,1,1,0)*(c(i,0,0,0)
     & *ui(i1,i2+1,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+2,c3)+c(i,2,0,
     & 0)*ui(i1+2,i2+1,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+2,c3)+c(i,
     & 4,0,0)*ui(i1+4,i2+1,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+2,c3)+
     & c(i,6,0,0)*ui(i1+6,i2+1,i3+2,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+2,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+2,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+2,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+2,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+2,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+2,c3)+c(i,6,0,
     & 0)*ui(i1+6,i2+2,i3+2,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+
     & 2,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+3,
     & i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+2,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+3,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+2,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+3,i3+2,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+2,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+4,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+2,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,
     & i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+2,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+4,i3+2,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+2,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+5,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+5,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+2,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+5,
     & i3+2,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+2,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+6,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+2,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+6,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+2,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+6,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+2,c3)
     & ))
               r(i) = r(i)+c(i,3,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,
     & i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2,
     & i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2,
     & i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2,
     & i3+3,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+3,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+1,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+1,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+3,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+1,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+3,c3)
     & )+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+3,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2+2,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+3,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+2,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+3,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+2,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+3,c3))+c(i,3,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+3,
     & i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+3,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+3,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+3,c3))+c(i,4,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+4,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+3,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+4,
     & i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+3,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+4,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+3,c3))+c(i,5,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+5,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+5,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+5,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+3,c3))+c(i,6,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+6,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+3,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+6,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+3,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+6,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+3,c3)
     & +c(i,6,0,0)*ui(i1+6,i2+6,i3+3,c3)))+c(i,4,2,0)*(c(i,0,1,0)*(c(
     & i,0,0,0)*ui(i1,i2,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+4,c3)+c(i,
     & 2,0,0)*ui(i1+2,i2,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+4,c3)+c(i,
     & 4,0,0)*ui(i1+4,i2,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+4,c3)+c(i,
     & 6,0,0)*ui(i1+6,i2,i3+4,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,
     & i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+4,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+1,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+4,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+1,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+4,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+1,i3+4,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+4,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+2,
     & i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+2,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+4,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+2,i3+4,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+4,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+3,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+4,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+3,
     & i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+4,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+3,i3+4,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+4,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+4,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+4,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+4,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+4,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+4,
     & i3+4,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+4,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+5,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+4,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+5,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+4,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+5,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+4,c3)
     & )+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+4,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2+6,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+4,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+6,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+4,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+6,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+4,c3)))+c(i,5,
     & 2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+5,c3)+c(i,1,0,0)*ui(
     & i1+1,i2,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+5,c3)+c(i,3,0,0)*ui(
     & i1+3,i2,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+5,c3)+c(i,5,0,0)*ui(
     & i1+5,i2,i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+5,c3))+c(i,1,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+1,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+1,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+1,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+5,c3))+c(i,2,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+2,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+5,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+2,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+5,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+2,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+5,c3)
     & +c(i,6,0,0)*ui(i1+6,i2+2,i3+5,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(
     & i1,i2+3,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+5,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+3,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+5,c3)+c(i,4,0,
     & 0)*ui(i1+4,i2+3,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+5,c3)+c(i,
     & 6,0,0)*ui(i1+6,i2+3,i3+5,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 4,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+5,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+4,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+5,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+4,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+5,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+4,i3+5,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+5,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+5,
     & i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+5,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+5,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+5,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+5,i3+5,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+5,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+6,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+5,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+6,
     & i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+5,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+6,i3+5,c3)))
               r(i) = r(i)+c(i,6,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,
     & i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2,
     & i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2,
     & i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2,
     & i3+6,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+6,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+1,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+6,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+1,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+6,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+1,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+6,c3)
     & )+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+6,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2+2,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+6,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+2,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+6,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+2,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+6,c3))+c(i,3,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+3,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+3,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+6,c3))+c(i,4,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+4,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+6,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+4,
     & i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+6,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+4,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+6,c3))+c(i,5,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+5,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+6,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+5,i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+6,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2+5,
     & i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+6,c3))+c(i,6,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+6,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+6,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+6,i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+6,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+6,i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+6,c3)
     & +c(i,6,0,0)*ui(i1+6,i2+6,i3+6,c3)))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
! interpTensorProduct777(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+0,
     & c3)+c(i,3,0,0)*ui(i1+3,i2,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+0,
     & c3)+c(i,5,0,0)*ui(i1+5,i2,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+0,
     & c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+0,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+1,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+0,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+1,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+0,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+1,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+0,c3))+c(i,
     & 2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+0,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+0,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+2,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+0,c3))+c(i,3,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+3,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+0,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+3,
     & i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+0,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+3,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+0,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+0,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+0,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+4,
     & i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+0,c3))+c(i,5,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+5,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+0,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+5,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+0,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+5,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+0,c3)
     & +c(i,6,0,0)*ui(i1+6,i2+5,i3+0,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(
     & i1,i2+6,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+0,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+6,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+0,c3)+c(i,4,0,
     & 0)*ui(i1+4,i2+6,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+0,c3)+c(i,
     & 6,0,0)*ui(i1+6,i2+6,i3+0,c3)))+c(i,1,2,0)*(c(i,0,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+1,c3)+c(i,2,0,0)*
     & ui(i1+2,i2,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+1,c3)+c(i,4,0,0)*
     & ui(i1+4,i2,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+1,c3)+c(i,6,0,0)*
     & ui(i1+6,i2,i3+1,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+1,c3)
     & +c(i,1,0,0)*ui(i1+1,i2+1,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+1,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+1,
     & i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+1,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+1,i3+1,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+1,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+1,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+2,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+1,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+2,
     & i3+1,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+1,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+3,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+1,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+3,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+1,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+3,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+1,c3)
     & )+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+1,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2+4,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+1,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+4,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+1,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+4,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+1,c3))+c(i,5,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+5,
     & i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+1,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+5,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+1,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+5,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+1,c3))+c(i,6,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+6,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+1,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+6,
     & i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+1,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+6,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+1,c3)))+c(i,2,2,0)*(c(
     & i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2,
     & i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2,
     & i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+2,c3))+c(i,1,1,0)*(c(i,0,0,0)
     & *ui(i1,i2+1,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+2,c3)+c(i,2,0,
     & 0)*ui(i1+2,i2+1,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+2,c3)+c(i,
     & 4,0,0)*ui(i1+4,i2+1,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+2,c3)+
     & c(i,6,0,0)*ui(i1+6,i2+1,i3+2,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+2,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+2,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+2,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+2,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+2,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+2,c3)+c(i,6,0,
     & 0)*ui(i1+6,i2+2,i3+2,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+
     & 2,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+3,
     & i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+2,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+3,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+2,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+3,i3+2,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+2,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+4,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+2,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,
     & i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+2,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+4,i3+2,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+2,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+5,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+5,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+2,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+5,
     & i3+2,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+2,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+6,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+2,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+6,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+2,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+6,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+2,c3)
     & ))
               r(i) = r(i)+c(i,3,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,
     & i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2,
     & i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2,
     & i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2,
     & i3+3,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+3,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+1,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+1,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+3,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+1,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+3,c3)
     & )+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+3,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2+2,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+3,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+2,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+3,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+2,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+3,c3))+c(i,3,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+3,
     & i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+3,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+3,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+3,c3))+c(i,4,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+4,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+3,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+4,
     & i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+3,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+4,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+3,c3))+c(i,5,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+5,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+5,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+5,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+3,c3))+c(i,6,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+6,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+3,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+6,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+3,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+6,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+3,c3)
     & +c(i,6,0,0)*ui(i1+6,i2+6,i3+3,c3)))+c(i,4,2,0)*(c(i,0,1,0)*(c(
     & i,0,0,0)*ui(i1,i2,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+4,c3)+c(i,
     & 2,0,0)*ui(i1+2,i2,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+4,c3)+c(i,
     & 4,0,0)*ui(i1+4,i2,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+4,c3)+c(i,
     & 6,0,0)*ui(i1+6,i2,i3+4,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,
     & i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+4,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+1,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+4,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+1,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+4,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+1,i3+4,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+4,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+2,
     & i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+2,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+4,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+2,i3+4,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+4,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+3,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+4,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+3,
     & i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+4,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+3,i3+4,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+4,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+4,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+4,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+4,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+4,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+4,
     & i3+4,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+4,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+5,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+4,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+5,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+4,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+5,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+4,c3)
     & )+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+4,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2+6,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+4,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+6,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+4,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+6,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+4,c3)))+c(i,5,
     & 2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+5,c3)+c(i,1,0,0)*ui(
     & i1+1,i2,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+5,c3)+c(i,3,0,0)*ui(
     & i1+3,i2,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+5,c3)+c(i,5,0,0)*ui(
     & i1+5,i2,i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+5,c3))+c(i,1,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+1,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+1,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+1,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+5,c3))+c(i,2,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+2,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+5,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+2,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+5,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+2,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+5,c3)
     & +c(i,6,0,0)*ui(i1+6,i2+2,i3+5,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(
     & i1,i2+3,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+5,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+3,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+5,c3)+c(i,4,0,
     & 0)*ui(i1+4,i2+3,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+5,c3)+c(i,
     & 6,0,0)*ui(i1+6,i2+3,i3+5,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 4,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+5,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+4,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+5,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+4,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+5,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+4,i3+5,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+5,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+5,
     & i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+5,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+5,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+5,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+5,i3+5,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+5,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+6,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+5,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+6,
     & i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+5,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+6,i3+5,c3)))
               r(i) = r(i)+c(i,6,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,
     & i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2,
     & i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2,
     & i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2,
     & i3+6,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+6,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+1,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+6,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+1,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+6,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+1,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+6,c3)
     & )+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+6,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2+2,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+6,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+2,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+6,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+2,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+6,c3))+c(i,3,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+3,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+3,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+6,c3))+c(i,4,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+4,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+6,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+4,
     & i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+6,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+4,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+6,c3))+c(i,5,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+5,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+6,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+5,i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+6,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2+5,
     & i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+6,c3))+c(i,6,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+6,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+6,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+6,i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+6,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+6,i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+6,c3)
     & +c(i,6,0,0)*ui(i1+6,i2+6,i3+6,c3)))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           end if
         else if( width(1).eq.8 .and. width(2).eq.8 .and. width(3)
     & .eq.8 )then
! loops3d($interpTensorProduct888(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))
           if( c3a.eq.c3b )then
             do c3=c3a,c3b
             do i=nia,nib
! interpTensorProduct888(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+0,
     & c3)+c(i,3,0,0)*ui(i1+3,i2,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+0,
     & c3)+c(i,5,0,0)*ui(i1+5,i2,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+0,
     & c3)+c(i,7,0,0)*ui(i1+7,i2,i3+0,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(
     & i1,i2+1,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+0,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+1,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+0,c3)+c(i,4,0,
     & 0)*ui(i1+4,i2+1,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+0,c3)+c(i,
     & 6,0,0)*ui(i1+6,i2+1,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+1,i3+0,c3))+
     & c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+0,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+2,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+0,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+2,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+0,c3)+c(i,7,0,
     & 0)*ui(i1+7,i2+2,i3+0,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+
     & 0,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+3,
     & i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+0,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+3,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+0,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+3,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+3,i3+0,c3))+c(i,4,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+4,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+0,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+4,
     & i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+0,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+4,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+0,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+4,i3+0,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+0,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+5,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+0,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+5,
     & i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+0,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+5,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+5,i3+0,c3))+c(i,6,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+6,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+0,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+6,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+0,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+6,
     & i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+0,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+6,i3+0,c3))+c(i,7,1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+0,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+7,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+0,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+7,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+0,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+7,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+7,
     & i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+7,i3+0,c3)))+c(i,1,2,0)*(c(i,0,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+1,
     & c3)+c(i,2,0,0)*ui(i1+2,i2,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+1,
     & c3)+c(i,4,0,0)*ui(i1+4,i2,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+1,
     & c3)+c(i,6,0,0)*ui(i1+6,i2,i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2,i3+1,
     & c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+1,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+1,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+1,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+1,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+1,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+1,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+1,c3)+c(i,
     & 7,0,0)*ui(i1+7,i2+1,i3+1,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+2,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+1,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+2,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+1,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+2,i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+2,i3+1,c3))+c(i,3,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+3,
     & i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+1,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+1,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+3,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+1,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+3,i3+1,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+1,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+4,
     & i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+1,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+4,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+1,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+4,i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+4,i3+1,c3))+c(i,5,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+5,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+1,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+5,
     & i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+1,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+5,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+1,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+5,i3+1,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+1,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+6,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+1,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+6,
     & i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+1,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+6,i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+6,i3+1,c3))+c(i,7,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+7,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+1,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+7,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+1,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+1,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+7,i3+1,c3)))
               r(i) = r(i)+c(i,2,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,
     & i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2,
     & i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2,
     & i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2,
     & i3+2,c3)+c(i,7,0,0)*ui(i1+7,i2,i3+2,c3))+c(i,1,1,0)*(c(i,0,0,0)
     & *ui(i1,i2+1,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+2,c3)+c(i,2,0,
     & 0)*ui(i1+2,i2+1,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+2,c3)+c(i,
     & 4,0,0)*ui(i1+4,i2+1,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+2,c3)+
     & c(i,6,0,0)*ui(i1+6,i2+1,i3+2,c3)+c(i,7,0,0)*ui(i1+7,i2+1,i3+2,
     & c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+2,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+2,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+2,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+2,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+2,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+2,c3)+c(i,
     & 7,0,0)*ui(i1+7,i2+2,i3+2,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 3,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+2,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+3,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+2,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+3,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+2,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+3,i3+2,c3)+c(i,7,0,0)*ui(i1+7,i2+3,i3+2,c3))+c(i,4,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+4,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+4,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+4,i3+2,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+2,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+5,
     & i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+2,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+5,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+2,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+5,i3+2,c3)+c(i,7,0,0)*ui(i1+7,i2+5,i3+2,c3))+c(i,6,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+6,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+2,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+6,
     & i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+2,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+6,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+2,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+6,i3+2,c3))+c(i,7,1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+2,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+7,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+2,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+7,
     & i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+7,i3+2,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+7,i3+2,c3)+c(i,7,0,0)*ui(i1+7,i2+7,i3+2,c3)))+c(i,3,2,0)*(c(
     & i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2,
     & i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2,
     & i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+3,c3)+c(i,7,0,0)*ui(i1+7,i2,
     & i3+3,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+3,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+1,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+1,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+3,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+1,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+3,c3)
     & +c(i,7,0,0)*ui(i1+7,i2+1,i3+3,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(
     & i1,i2+2,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+3,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+2,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+3,c3)+c(i,4,0,
     & 0)*ui(i1+4,i2+2,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+3,c3)+c(i,
     & 6,0,0)*ui(i1+6,i2+2,i3+3,c3)+c(i,7,0,0)*ui(i1+7,i2+2,i3+3,c3))+
     & c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+3,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+3,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+3,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+3,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+3,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+3,c3)+c(i,7,0,
     & 0)*ui(i1+7,i2+3,i3+3,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+
     & 3,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+4,
     & i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+3,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+4,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+3,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+4,i3+3,c3)+c(i,7,0,0)*ui(i1+7,i2+4,i3+3,c3))+c(i,5,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+5,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+3,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+5,
     & i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+3,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+5,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+3,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+5,i3+3,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+3,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+6,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+3,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+6,
     & i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+3,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+6,i3+3,c3)+c(i,7,0,0)*ui(i1+7,i2+6,i3+3,c3))+c(i,7,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+7,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+7,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+7,i3+3,c3)))
               r(i) = r(i)+c(i,4,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,
     & i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2,
     & i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2,
     & i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2,
     & i3+4,c3)+c(i,7,0,0)*ui(i1+7,i2,i3+4,c3))+c(i,1,1,0)*(c(i,0,0,0)
     & *ui(i1,i2+1,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+4,c3)+c(i,2,0,
     & 0)*ui(i1+2,i2+1,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+4,c3)+c(i,
     & 4,0,0)*ui(i1+4,i2+1,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+4,c3)+
     & c(i,6,0,0)*ui(i1+6,i2+1,i3+4,c3)+c(i,7,0,0)*ui(i1+7,i2+1,i3+4,
     & c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+4,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+2,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+4,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+4,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+2,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+4,c3)+c(i,
     & 7,0,0)*ui(i1+7,i2+2,i3+4,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 3,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+4,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+3,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+4,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+3,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+4,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+3,i3+4,c3)+c(i,7,0,0)*ui(i1+7,i2+3,i3+4,c3))+c(i,4,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+4,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+4,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+4,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+4,i3+4,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+4,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+5,
     & i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+4,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+5,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+4,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+5,i3+4,c3)+c(i,7,0,0)*ui(i1+7,i2+5,i3+4,c3))+c(i,6,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+6,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+4,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+6,
     & i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+4,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+6,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+4,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+6,i3+4,c3))+c(i,7,1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+4,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+7,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+4,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+7,
     & i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+7,i3+4,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+7,i3+4,c3)+c(i,7,0,0)*ui(i1+7,i2+7,i3+4,c3)))+c(i,5,2,0)*(c(
     & i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2,
     & i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2,
     & i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+5,c3)+c(i,7,0,0)*ui(i1+7,i2,
     & i3+5,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+5,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+1,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+5,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+1,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+5,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+1,i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+5,c3)
     & +c(i,7,0,0)*ui(i1+7,i2+1,i3+5,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(
     & i1,i2+2,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+5,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+2,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+5,c3)+c(i,4,0,
     & 0)*ui(i1+4,i2+2,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+5,c3)+c(i,
     & 6,0,0)*ui(i1+6,i2+2,i3+5,c3)+c(i,7,0,0)*ui(i1+7,i2+2,i3+5,c3))+
     & c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+5,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+3,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+5,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+3,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+5,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+3,i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+5,c3)+c(i,7,0,
     & 0)*ui(i1+7,i2+3,i3+5,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+
     & 5,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+4,
     & i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+5,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+4,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+5,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+4,i3+5,c3)+c(i,7,0,0)*ui(i1+7,i2+4,i3+5,c3))+c(i,5,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+5,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+5,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+5,
     & i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+5,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+5,i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+5,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+5,i3+5,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+5,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+6,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+5,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+6,
     & i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+5,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+6,i3+5,c3)+c(i,7,0,0)*ui(i1+7,i2+6,i3+5,c3))+c(i,7,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+7,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+7,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+7,i3+5,c3)))
               r(i) = r(i)+c(i,6,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,
     & i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2,
     & i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2,
     & i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2,
     & i3+6,c3)+c(i,7,0,0)*ui(i1+7,i2,i3+6,c3))+c(i,1,1,0)*(c(i,0,0,0)
     & *ui(i1,i2+1,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+6,c3)+c(i,2,0,
     & 0)*ui(i1+2,i2+1,i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+6,c3)+c(i,
     & 4,0,0)*ui(i1+4,i2+1,i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+6,c3)+
     & c(i,6,0,0)*ui(i1+6,i2+1,i3+6,c3)+c(i,7,0,0)*ui(i1+7,i2+1,i3+6,
     & c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+6,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+2,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+6,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+2,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+6,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+2,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+6,c3)+c(i,
     & 7,0,0)*ui(i1+7,i2+2,i3+6,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 3,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+6,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+3,i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+6,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+3,i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+6,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+3,i3+6,c3)+c(i,7,0,0)*ui(i1+7,i2+3,i3+6,c3))+c(i,4,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+4,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+4,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+6,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+4,i3+6,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+6,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+5,
     & i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+6,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+5,i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+6,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+5,i3+6,c3)+c(i,7,0,0)*ui(i1+7,i2+5,i3+6,c3))+c(i,6,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+6,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+6,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+6,
     & i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+6,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+6,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+6,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+6,i3+6,c3))+c(i,7,1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+6,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+7,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+6,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+7,
     & i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2+7,i3+6,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+7,i3+6,c3)+c(i,7,0,0)*ui(i1+7,i2+7,i3+6,c3)))+c(i,7,2,0)*(c(
     & i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2,
     & i3+7,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2,
     & i3+7,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+7,c3)+c(i,7,0,0)*ui(i1+7,i2,
     & i3+7,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+7,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+1,i3+7,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+7,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+1,i3+7,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+7,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+1,i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+7,c3)
     & +c(i,7,0,0)*ui(i1+7,i2+1,i3+7,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(
     & i1,i2+2,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+7,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+2,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+7,c3)+c(i,4,0,
     & 0)*ui(i1+4,i2+2,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+7,c3)+c(i,
     & 6,0,0)*ui(i1+6,i2+2,i3+7,c3)+c(i,7,0,0)*ui(i1+7,i2+2,i3+7,c3))+
     & c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+7,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+3,i3+7,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+7,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+3,i3+7,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+7,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+3,i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+7,c3)+c(i,7,0,
     & 0)*ui(i1+7,i2+3,i3+7,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+
     & 7,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+7,c3)+c(i,2,0,0)*ui(i1+2,i2+4,
     & i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+7,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+4,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+7,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+4,i3+7,c3)+c(i,7,0,0)*ui(i1+7,i2+4,i3+7,c3))+c(i,5,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+5,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+7,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+5,
     & i3+7,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+7,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+5,i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+7,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+5,i3+7,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+7,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+6,i3+7,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+7,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+7,c3)+c(i,4,0,0)*ui(i1+4,i2+6,
     & i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+7,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+6,i3+7,c3)+c(i,7,0,0)*ui(i1+7,i2+6,i3+7,c3))+c(i,7,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+7,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+7,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+7,i3+7,c3)))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
! interpTensorProduct888(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+0,
     & c3)+c(i,3,0,0)*ui(i1+3,i2,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+0,
     & c3)+c(i,5,0,0)*ui(i1+5,i2,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+0,
     & c3)+c(i,7,0,0)*ui(i1+7,i2,i3+0,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(
     & i1,i2+1,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+0,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+1,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+0,c3)+c(i,4,0,
     & 0)*ui(i1+4,i2+1,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+0,c3)+c(i,
     & 6,0,0)*ui(i1+6,i2+1,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+1,i3+0,c3))+
     & c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+0,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+2,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+0,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+2,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+0,c3)+c(i,7,0,
     & 0)*ui(i1+7,i2+2,i3+0,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+
     & 0,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+3,
     & i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+0,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+3,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+0,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+3,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+3,i3+0,c3))+c(i,4,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+4,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+0,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+4,
     & i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+0,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+4,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+0,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+4,i3+0,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+0,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+5,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+0,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+5,
     & i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+0,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+5,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+5,i3+0,c3))+c(i,6,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+6,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+0,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+6,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+0,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+6,
     & i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+0,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+6,i3+0,c3))+c(i,7,1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+0,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+7,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+0,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+7,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+0,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+7,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+7,
     & i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+7,i3+0,c3)))+c(i,1,2,0)*(c(i,0,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+1,
     & c3)+c(i,2,0,0)*ui(i1+2,i2,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+1,
     & c3)+c(i,4,0,0)*ui(i1+4,i2,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+1,
     & c3)+c(i,6,0,0)*ui(i1+6,i2,i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2,i3+1,
     & c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+1,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+1,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+1,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+1,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+1,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+1,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+1,c3)+c(i,
     & 7,0,0)*ui(i1+7,i2+1,i3+1,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+2,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+1,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+2,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+1,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+2,i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+2,i3+1,c3))+c(i,3,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+3,
     & i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+1,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+1,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+3,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+1,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+3,i3+1,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+1,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+4,
     & i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+1,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+4,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+1,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+4,i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+4,i3+1,c3))+c(i,5,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+5,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+1,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+5,
     & i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+1,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+5,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+1,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+5,i3+1,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+1,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+6,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+1,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+6,
     & i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+1,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+6,i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+6,i3+1,c3))+c(i,7,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+7,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+1,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+7,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+1,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+1,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+7,i3+1,c3)))
               r(i) = r(i)+c(i,2,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,
     & i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2,
     & i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2,
     & i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2,
     & i3+2,c3)+c(i,7,0,0)*ui(i1+7,i2,i3+2,c3))+c(i,1,1,0)*(c(i,0,0,0)
     & *ui(i1,i2+1,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+2,c3)+c(i,2,0,
     & 0)*ui(i1+2,i2+1,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+2,c3)+c(i,
     & 4,0,0)*ui(i1+4,i2+1,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+2,c3)+
     & c(i,6,0,0)*ui(i1+6,i2+1,i3+2,c3)+c(i,7,0,0)*ui(i1+7,i2+1,i3+2,
     & c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+2,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+2,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+2,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+2,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+2,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+2,c3)+c(i,
     & 7,0,0)*ui(i1+7,i2+2,i3+2,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 3,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+2,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+3,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+2,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+3,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+2,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+3,i3+2,c3)+c(i,7,0,0)*ui(i1+7,i2+3,i3+2,c3))+c(i,4,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+4,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+4,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+4,i3+2,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+2,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+5,
     & i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+2,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+5,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+2,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+5,i3+2,c3)+c(i,7,0,0)*ui(i1+7,i2+5,i3+2,c3))+c(i,6,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+6,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+2,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+6,
     & i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+2,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+6,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+2,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+6,i3+2,c3))+c(i,7,1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+2,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+7,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+2,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+7,
     & i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+7,i3+2,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+7,i3+2,c3)+c(i,7,0,0)*ui(i1+7,i2+7,i3+2,c3)))+c(i,3,2,0)*(c(
     & i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2,
     & i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2,
     & i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+3,c3)+c(i,7,0,0)*ui(i1+7,i2,
     & i3+3,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+3,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+1,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+1,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+3,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+1,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+3,c3)
     & +c(i,7,0,0)*ui(i1+7,i2+1,i3+3,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(
     & i1,i2+2,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+3,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+2,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+3,c3)+c(i,4,0,
     & 0)*ui(i1+4,i2+2,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+3,c3)+c(i,
     & 6,0,0)*ui(i1+6,i2+2,i3+3,c3)+c(i,7,0,0)*ui(i1+7,i2+2,i3+3,c3))+
     & c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+3,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+3,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+3,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+3,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+3,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+3,c3)+c(i,7,0,
     & 0)*ui(i1+7,i2+3,i3+3,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+
     & 3,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+4,
     & i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+3,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+4,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+3,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+4,i3+3,c3)+c(i,7,0,0)*ui(i1+7,i2+4,i3+3,c3))+c(i,5,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+5,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+3,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+5,
     & i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+3,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+5,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+3,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+5,i3+3,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+3,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+6,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+3,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+6,
     & i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+3,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+6,i3+3,c3)+c(i,7,0,0)*ui(i1+7,i2+6,i3+3,c3))+c(i,7,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+7,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+7,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+7,i3+3,c3)))
               r(i) = r(i)+c(i,4,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,
     & i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2,
     & i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2,
     & i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2,
     & i3+4,c3)+c(i,7,0,0)*ui(i1+7,i2,i3+4,c3))+c(i,1,1,0)*(c(i,0,0,0)
     & *ui(i1,i2+1,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+4,c3)+c(i,2,0,
     & 0)*ui(i1+2,i2+1,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+4,c3)+c(i,
     & 4,0,0)*ui(i1+4,i2+1,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+4,c3)+
     & c(i,6,0,0)*ui(i1+6,i2+1,i3+4,c3)+c(i,7,0,0)*ui(i1+7,i2+1,i3+4,
     & c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+4,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+2,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+4,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+4,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+2,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+4,c3)+c(i,
     & 7,0,0)*ui(i1+7,i2+2,i3+4,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 3,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+4,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+3,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+4,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+3,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+4,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+3,i3+4,c3)+c(i,7,0,0)*ui(i1+7,i2+3,i3+4,c3))+c(i,4,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+4,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+4,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+4,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+4,i3+4,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+4,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+5,
     & i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+4,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+5,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+4,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+5,i3+4,c3)+c(i,7,0,0)*ui(i1+7,i2+5,i3+4,c3))+c(i,6,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+6,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+4,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+6,
     & i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+4,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+6,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+4,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+6,i3+4,c3))+c(i,7,1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+4,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+7,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+4,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+7,
     & i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+7,i3+4,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+7,i3+4,c3)+c(i,7,0,0)*ui(i1+7,i2+7,i3+4,c3)))+c(i,5,2,0)*(c(
     & i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2,
     & i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2,
     & i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+5,c3)+c(i,7,0,0)*ui(i1+7,i2,
     & i3+5,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+5,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+1,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+5,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+1,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+5,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+1,i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+5,c3)
     & +c(i,7,0,0)*ui(i1+7,i2+1,i3+5,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(
     & i1,i2+2,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+5,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+2,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+5,c3)+c(i,4,0,
     & 0)*ui(i1+4,i2+2,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+5,c3)+c(i,
     & 6,0,0)*ui(i1+6,i2+2,i3+5,c3)+c(i,7,0,0)*ui(i1+7,i2+2,i3+5,c3))+
     & c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+5,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+3,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+5,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+3,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+5,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+3,i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+5,c3)+c(i,7,0,
     & 0)*ui(i1+7,i2+3,i3+5,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+
     & 5,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+4,
     & i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+5,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+4,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+5,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+4,i3+5,c3)+c(i,7,0,0)*ui(i1+7,i2+4,i3+5,c3))+c(i,5,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+5,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+5,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+5,
     & i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+5,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+5,i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+5,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+5,i3+5,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+5,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+6,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+5,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+6,
     & i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+5,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+6,i3+5,c3)+c(i,7,0,0)*ui(i1+7,i2+6,i3+5,c3))+c(i,7,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+7,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+7,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+7,i3+5,c3)))
               r(i) = r(i)+c(i,6,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,
     & i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2,
     & i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2,
     & i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2,
     & i3+6,c3)+c(i,7,0,0)*ui(i1+7,i2,i3+6,c3))+c(i,1,1,0)*(c(i,0,0,0)
     & *ui(i1,i2+1,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+6,c3)+c(i,2,0,
     & 0)*ui(i1+2,i2+1,i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+6,c3)+c(i,
     & 4,0,0)*ui(i1+4,i2+1,i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+6,c3)+
     & c(i,6,0,0)*ui(i1+6,i2+1,i3+6,c3)+c(i,7,0,0)*ui(i1+7,i2+1,i3+6,
     & c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+6,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+2,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+6,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+2,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+6,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+2,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+6,c3)+c(i,
     & 7,0,0)*ui(i1+7,i2+2,i3+6,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 3,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+6,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+3,i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+6,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+3,i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+6,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+3,i3+6,c3)+c(i,7,0,0)*ui(i1+7,i2+3,i3+6,c3))+c(i,4,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+4,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+4,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+6,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+4,i3+6,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+6,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+5,
     & i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+6,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+5,i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+6,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+5,i3+6,c3)+c(i,7,0,0)*ui(i1+7,i2+5,i3+6,c3))+c(i,6,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+6,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+6,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+6,
     & i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+6,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+6,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+6,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+6,i3+6,c3))+c(i,7,1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+6,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+7,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+6,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+7,
     & i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2+7,i3+6,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+7,i3+6,c3)+c(i,7,0,0)*ui(i1+7,i2+7,i3+6,c3)))+c(i,7,2,0)*(c(
     & i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2,
     & i3+7,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2,
     & i3+7,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+7,c3)+c(i,7,0,0)*ui(i1+7,i2,
     & i3+7,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+7,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+1,i3+7,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+7,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+1,i3+7,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+7,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+1,i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+7,c3)
     & +c(i,7,0,0)*ui(i1+7,i2+1,i3+7,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(
     & i1,i2+2,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+7,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+2,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+7,c3)+c(i,4,0,
     & 0)*ui(i1+4,i2+2,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+7,c3)+c(i,
     & 6,0,0)*ui(i1+6,i2+2,i3+7,c3)+c(i,7,0,0)*ui(i1+7,i2+2,i3+7,c3))+
     & c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+7,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+3,i3+7,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+7,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+3,i3+7,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+7,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+3,i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+7,c3)+c(i,7,0,
     & 0)*ui(i1+7,i2+3,i3+7,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+
     & 7,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+7,c3)+c(i,2,0,0)*ui(i1+2,i2+4,
     & i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+7,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+4,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+7,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+4,i3+7,c3)+c(i,7,0,0)*ui(i1+7,i2+4,i3+7,c3))+c(i,5,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+5,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+7,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+5,
     & i3+7,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+7,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+5,i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+7,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+5,i3+7,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+7,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+6,i3+7,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+7,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+7,c3)+c(i,4,0,0)*ui(i1+4,i2+6,
     & i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+7,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+6,i3+7,c3)+c(i,7,0,0)*ui(i1+7,i2+6,i3+7,c3))+c(i,7,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+7,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+7,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+7,i3+7,c3)))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           end if
         else if( width(1).eq.9 .and. width(2).eq.9 .and. width(3)
     & .eq.9 )then
! loops3d($interpTensorProduct999(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))
           if( c3a.eq.c3b )then
             do c3=c3a,c3b
             do i=nia,nib
! interpTensorProduct999(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+0,
     & c3)+c(i,3,0,0)*ui(i1+3,i2,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+0,
     & c3)+c(i,5,0,0)*ui(i1+5,i2,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+0,
     & c3)+c(i,7,0,0)*ui(i1+7,i2,i3+0,c3)+c(i,8,0,0)*ui(i1+8,i2,i3+0,
     & c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+0,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+1,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+0,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+1,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+0,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+1,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+0,c3)+c(i,
     & 7,0,0)*ui(i1+7,i2+1,i3+0,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+0,c3))+
     & c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+0,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+2,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+0,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+2,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+0,c3)+c(i,7,0,
     & 0)*ui(i1+7,i2+2,i3+0,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+0,c3))+c(i,
     & 3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+3,
     & i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+0,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+0,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+3,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+0,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+3,i3+0,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+0,c3))+c(i,4,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+4,
     & i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+0,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+0,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+4,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+0,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+4,i3+0,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+0,c3))+c(i,5,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+5,
     & i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+0,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+5,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+0,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+5,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+0,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+5,i3+0,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+0,c3))+c(i,6,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+6,
     & i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+0,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+6,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+0,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+6,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+0,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+6,i3+0,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+0,c3))+c(i,7,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+7,
     & i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+0,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+7,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+0,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+7,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+0,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+7,i3+0,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+0,c3))+c(i,8,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+8,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+8,
     & i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+8,i3+0,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+8,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+0,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+8,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+0,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+8,i3+0,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+0,c3)))+c(i,1,
     & 2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+1,c3)+c(i,1,0,0)*ui(
     & i1+1,i2,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+1,c3)+c(i,3,0,0)*ui(
     & i1+3,i2,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+1,c3)+c(i,5,0,0)*ui(
     & i1+5,i2,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+1,c3)+c(i,7,0,0)*ui(
     & i1+7,i2,i3+1,c3)+c(i,8,0,0)*ui(i1+8,i2,i3+1,c3))+c(i,1,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+1,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+1,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+1,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+1,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+1,
     & i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+1,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+1,i3+1,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+1,c3))+c(i,2,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+1,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+2,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+1,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+2,
     & i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+1,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+2,i3+1,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+1,c3))+c(i,3,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+3,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+1,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+3,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+1,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+3,
     & i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+1,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+3,i3+1,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+1,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+1,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+1,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+4,
     & i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+1,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+4,i3+1,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+1,c3))+c(i,5,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+5,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+1,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+5,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+1,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+5,
     & i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+1,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+5,i3+1,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+1,c3))+c(i,6,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+6,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+1,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+6,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+1,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+6,
     & i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+1,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+6,i3+1,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+1,c3))+c(i,7,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+7,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+1,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+7,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+1,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+1,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+7,i3+1,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+1,c3))+c(i,8,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+8,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+8,i3+1,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+8,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+8,i3+1,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+8,
     & i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+1,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+8,i3+1,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+1,c3)))
               r(i) = r(i)+c(i,2,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,
     & i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2,
     & i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2,
     & i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2,
     & i3+2,c3)+c(i,7,0,0)*ui(i1+7,i2,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2,
     & i3+2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+2,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+1,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+2,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+1,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+2,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+1,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+2,c3)
     & +c(i,7,0,0)*ui(i1+7,i2+1,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+2,
     & c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+2,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+2,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+2,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+2,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+2,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+2,c3)+c(i,
     & 7,0,0)*ui(i1+7,i2+2,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+2,c3))+
     & c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+2,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+3,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+2,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+3,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+2,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+3,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+2,c3)+c(i,7,0,
     & 0)*ui(i1+7,i2+3,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+2,c3))+c(i,
     & 4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+4,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+4,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+4,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+2,c3))+c(i,5,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+5,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+5,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+5,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+5,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+2,c3))+c(i,6,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+6,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+6,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+6,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+6,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+2,c3))+c(i,7,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+7,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+7,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+7,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+7,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+2,c3))+c(i,8,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+8,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+8,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+8,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+8,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+8,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+8,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+2,c3)))+c(i,3,
     & 2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+3,c3)+c(i,1,0,0)*ui(
     & i1+1,i2,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+3,c3)+c(i,3,0,0)*ui(
     & i1+3,i2,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+3,c3)+c(i,5,0,0)*ui(
     & i1+5,i2,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+3,c3)+c(i,7,0,0)*ui(
     & i1+7,i2,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2,i3+3,c3))+c(i,1,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+1,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+1,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+1,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+3,c3))+c(i,2,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+2,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+2,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+2,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+2,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+3,c3))+c(i,3,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+3,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+3,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+3,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+3,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+4,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+4,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+3,c3))+c(i,5,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+5,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+5,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+5,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+5,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+3,c3))+c(i,6,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+6,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+6,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+6,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+6,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+3,c3))+c(i,7,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+7,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+7,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+7,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+3,c3))+c(i,8,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+8,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+8,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+8,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+8,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+8,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+8,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+3,c3)))
               r(i) = r(i)+c(i,4,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,
     & i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2,
     & i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2,
     & i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2,
     & i3+4,c3)+c(i,7,0,0)*ui(i1+7,i2,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2,
     & i3+4,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+4,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+1,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+4,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+1,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+4,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+1,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+4,c3)
     & +c(i,7,0,0)*ui(i1+7,i2+1,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+4,
     & c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+4,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+2,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+4,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+4,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+2,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+4,c3)+c(i,
     & 7,0,0)*ui(i1+7,i2+2,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+4,c3))+
     & c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+4,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+3,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+4,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+3,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+4,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+3,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+4,c3)+c(i,7,0,
     & 0)*ui(i1+7,i2+3,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+4,c3))+c(i,
     & 4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+4,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+4,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+4,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+4,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+4,c3))+c(i,5,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+5,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+5,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+5,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+4,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+5,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+4,c3))+c(i,6,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+6,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+6,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+6,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+4,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+6,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+4,c3))+c(i,7,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+7,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+7,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+7,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+4,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+7,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+4,c3))+c(i,8,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+8,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+8,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+8,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+8,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+8,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+4,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+8,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+4,c3)))+c(i,5,
     & 2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+5,c3)+c(i,1,0,0)*ui(
     & i1+1,i2,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+5,c3)+c(i,3,0,0)*ui(
     & i1+3,i2,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+5,c3)+c(i,5,0,0)*ui(
     & i1+5,i2,i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+5,c3)+c(i,7,0,0)*ui(
     & i1+7,i2,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2,i3+5,c3))+c(i,1,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+1,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+1,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+1,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+1,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+5,c3))+c(i,2,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+2,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+2,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+2,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+2,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+5,c3))+c(i,3,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+3,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+3,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+3,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+3,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+5,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+4,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+4,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+5,c3))+c(i,5,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+5,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+5,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+5,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+5,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+5,c3))+c(i,6,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+6,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+6,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+6,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+6,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+5,c3))+c(i,7,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+7,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+7,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+7,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+5,c3))+c(i,8,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+8,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+8,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+8,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+8,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+8,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+8,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+5,c3)))
               r(i) = r(i)+c(i,6,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,
     & i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2,
     & i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2,
     & i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2,
     & i3+6,c3)+c(i,7,0,0)*ui(i1+7,i2,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2,
     & i3+6,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+6,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+1,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+6,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+1,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+6,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+1,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+6,c3)
     & +c(i,7,0,0)*ui(i1+7,i2+1,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+6,
     & c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+6,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+2,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+6,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+2,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+6,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+2,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+6,c3)+c(i,
     & 7,0,0)*ui(i1+7,i2+2,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+6,c3))+
     & c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+6,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+3,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+6,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+3,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+6,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+3,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+6,c3)+c(i,7,0,
     & 0)*ui(i1+7,i2+3,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+6,c3))+c(i,
     & 4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+4,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+4,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+6,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+4,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+6,c3))+c(i,5,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+5,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+5,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+5,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+6,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+5,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+6,c3))+c(i,6,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+6,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+6,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+6,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+6,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+6,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+6,c3))+c(i,7,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+7,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+7,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+7,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+6,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+7,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+6,c3))+c(i,8,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+8,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+8,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+8,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+8,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+8,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+6,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+8,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+6,c3)))+c(i,7,
     & 2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+7,c3)+c(i,1,0,0)*ui(
     & i1+1,i2,i3+7,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+7,c3)+c(i,3,0,0)*ui(
     & i1+3,i2,i3+7,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+7,c3)+c(i,5,0,0)*ui(
     & i1+5,i2,i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+7,c3)+c(i,7,0,0)*ui(
     & i1+7,i2,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2,i3+7,c3))+c(i,1,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+1,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+1,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+1,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+1,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+7,c3))+c(i,2,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+2,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+2,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+2,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+2,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+7,c3))+c(i,3,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+3,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+3,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+3,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+3,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+7,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+4,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+4,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+7,c3))+c(i,5,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+5,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+5,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+5,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+5,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+7,c3))+c(i,6,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+6,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+6,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+6,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+6,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+7,c3))+c(i,7,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+7,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+7,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+7,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+7,c3))+c(i,8,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+8,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+8,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+8,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+8,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+8,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+8,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+7,c3)))
               r(i) = r(i)+c(i,8,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,
     & i3+8,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2,
     & i3+8,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2,
     & i3+8,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2,
     & i3+8,c3)+c(i,7,0,0)*ui(i1+7,i2,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2,
     & i3+8,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+8,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+1,i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+8,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+1,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+8,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+1,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+8,c3)
     & +c(i,7,0,0)*ui(i1+7,i2+1,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+8,
     & c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+8,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+2,i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+8,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+2,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+8,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+2,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+8,c3)+c(i,
     & 7,0,0)*ui(i1+7,i2+2,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+8,c3))+
     & c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+8,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+3,i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+8,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+3,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+8,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+3,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+8,c3)+c(i,7,0,
     & 0)*ui(i1+7,i2+3,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+8,c3))+c(i,
     & 4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+8,c3)+c(i,1,0,0)*ui(i1+1,i2+4,
     & i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+8,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+8,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+4,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+8,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+4,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+8,c3))+c(i,5,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+8,c3)+c(i,1,0,0)*ui(i1+1,i2+5,
     & i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+8,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+5,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+8,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+5,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+8,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+5,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+8,c3))+c(i,6,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+8,c3)+c(i,1,0,0)*ui(i1+1,i2+6,
     & i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+8,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+6,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+8,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+6,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+8,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+6,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+8,c3))+c(i,7,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+8,c3)+c(i,1,0,0)*ui(i1+1,i2+7,
     & i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+8,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+7,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+8,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+7,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+8,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+7,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+8,c3))+c(i,8,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+8,i3+8,c3)+c(i,1,0,0)*ui(i1+1,i2+8,
     & i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+8,i3+8,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+8,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+8,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+8,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+8,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+8,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+8,c3)))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
! interpTensorProduct999(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+0,
     & c3)+c(i,3,0,0)*ui(i1+3,i2,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+0,
     & c3)+c(i,5,0,0)*ui(i1+5,i2,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+0,
     & c3)+c(i,7,0,0)*ui(i1+7,i2,i3+0,c3)+c(i,8,0,0)*ui(i1+8,i2,i3+0,
     & c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+0,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+1,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+0,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+1,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+0,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+1,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+0,c3)+c(i,
     & 7,0,0)*ui(i1+7,i2+1,i3+0,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+0,c3))+
     & c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+0,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+2,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+0,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+2,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+0,c3)+c(i,7,0,
     & 0)*ui(i1+7,i2+2,i3+0,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+0,c3))+c(i,
     & 3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+3,
     & i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+0,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+0,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+3,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+0,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+3,i3+0,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+0,c3))+c(i,4,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+4,
     & i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+0,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+0,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+4,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+0,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+4,i3+0,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+0,c3))+c(i,5,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+5,
     & i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+0,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+5,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+0,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+5,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+0,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+5,i3+0,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+0,c3))+c(i,6,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+6,
     & i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+0,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+6,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+0,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+6,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+0,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+6,i3+0,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+0,c3))+c(i,7,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+7,
     & i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+0,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+7,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+0,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+7,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+0,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+7,i3+0,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+0,c3))+c(i,8,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+8,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+8,
     & i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+8,i3+0,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+8,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+0,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+8,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+0,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+8,i3+0,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+0,c3)))+c(i,1,
     & 2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+1,c3)+c(i,1,0,0)*ui(
     & i1+1,i2,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+1,c3)+c(i,3,0,0)*ui(
     & i1+3,i2,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+1,c3)+c(i,5,0,0)*ui(
     & i1+5,i2,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+1,c3)+c(i,7,0,0)*ui(
     & i1+7,i2,i3+1,c3)+c(i,8,0,0)*ui(i1+8,i2,i3+1,c3))+c(i,1,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+1,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+1,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+1,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+1,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+1,
     & i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+1,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+1,i3+1,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+1,c3))+c(i,2,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+1,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+2,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+1,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+2,
     & i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+1,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+2,i3+1,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+1,c3))+c(i,3,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+3,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+1,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+3,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+1,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+3,
     & i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+1,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+3,i3+1,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+1,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+1,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+1,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+4,
     & i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+1,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+4,i3+1,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+1,c3))+c(i,5,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+5,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+1,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+5,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+1,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+5,
     & i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+1,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+5,i3+1,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+1,c3))+c(i,6,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+6,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+1,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+6,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+1,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+6,
     & i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+1,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+6,i3+1,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+1,c3))+c(i,7,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+7,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+1,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+7,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+1,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+1,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+7,i3+1,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+1,c3))+c(i,8,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+8,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+8,i3+1,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+8,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+8,i3+1,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+8,
     & i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+1,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+8,i3+1,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+1,c3)))
               r(i) = r(i)+c(i,2,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,
     & i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2,
     & i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2,
     & i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2,
     & i3+2,c3)+c(i,7,0,0)*ui(i1+7,i2,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2,
     & i3+2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+2,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+1,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+2,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+1,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+2,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+1,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+2,c3)
     & +c(i,7,0,0)*ui(i1+7,i2+1,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+2,
     & c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+2,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+2,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+2,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+2,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+2,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+2,c3)+c(i,
     & 7,0,0)*ui(i1+7,i2+2,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+2,c3))+
     & c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+2,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+3,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+2,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+3,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+2,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+3,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+2,c3)+c(i,7,0,
     & 0)*ui(i1+7,i2+3,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+2,c3))+c(i,
     & 4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+4,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+4,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+4,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+2,c3))+c(i,5,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+5,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+5,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+5,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+5,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+2,c3))+c(i,6,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+6,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+6,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+6,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+6,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+2,c3))+c(i,7,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+7,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+7,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+7,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+7,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+2,c3))+c(i,8,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+8,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+8,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+8,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+8,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+8,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+8,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+2,c3)))+c(i,3,
     & 2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+3,c3)+c(i,1,0,0)*ui(
     & i1+1,i2,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+3,c3)+c(i,3,0,0)*ui(
     & i1+3,i2,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+3,c3)+c(i,5,0,0)*ui(
     & i1+5,i2,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+3,c3)+c(i,7,0,0)*ui(
     & i1+7,i2,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2,i3+3,c3))+c(i,1,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+1,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+1,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+1,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+3,c3))+c(i,2,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+2,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+2,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+2,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+2,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+3,c3))+c(i,3,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+3,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+3,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+3,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+3,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+4,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+4,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+3,c3))+c(i,5,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+5,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+5,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+5,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+5,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+3,c3))+c(i,6,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+6,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+6,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+6,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+6,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+3,c3))+c(i,7,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+7,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+7,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+7,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+3,c3))+c(i,8,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+8,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+8,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+8,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+8,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+8,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+8,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+3,c3)))
               r(i) = r(i)+c(i,4,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,
     & i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2,
     & i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2,
     & i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2,
     & i3+4,c3)+c(i,7,0,0)*ui(i1+7,i2,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2,
     & i3+4,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+4,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+1,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+4,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+1,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+4,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+1,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+4,c3)
     & +c(i,7,0,0)*ui(i1+7,i2+1,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+4,
     & c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+4,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+2,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+4,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+4,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+2,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+4,c3)+c(i,
     & 7,0,0)*ui(i1+7,i2+2,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+4,c3))+
     & c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+4,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+3,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+4,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+3,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+4,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+3,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+4,c3)+c(i,7,0,
     & 0)*ui(i1+7,i2+3,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+4,c3))+c(i,
     & 4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+4,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+4,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+4,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+4,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+4,c3))+c(i,5,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+5,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+5,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+5,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+4,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+5,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+4,c3))+c(i,6,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+6,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+6,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+6,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+4,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+6,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+4,c3))+c(i,7,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+7,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+7,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+7,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+4,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+7,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+4,c3))+c(i,8,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+8,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+8,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+8,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+8,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+8,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+4,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+8,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+4,c3)))+c(i,5,
     & 2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+5,c3)+c(i,1,0,0)*ui(
     & i1+1,i2,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+5,c3)+c(i,3,0,0)*ui(
     & i1+3,i2,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+5,c3)+c(i,5,0,0)*ui(
     & i1+5,i2,i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+5,c3)+c(i,7,0,0)*ui(
     & i1+7,i2,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2,i3+5,c3))+c(i,1,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+1,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+1,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+1,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+1,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+5,c3))+c(i,2,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+2,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+2,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+2,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+2,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+5,c3))+c(i,3,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+3,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+3,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+3,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+3,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+5,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+4,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+4,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+5,c3))+c(i,5,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+5,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+5,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+5,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+5,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+5,c3))+c(i,6,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+6,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+6,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+6,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+6,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+5,c3))+c(i,7,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+7,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+7,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+7,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+5,c3))+c(i,8,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+8,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+8,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+8,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+8,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+8,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+8,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+5,c3)))
               r(i) = r(i)+c(i,6,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,
     & i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2,
     & i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2,
     & i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2,
     & i3+6,c3)+c(i,7,0,0)*ui(i1+7,i2,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2,
     & i3+6,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+6,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+1,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+6,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+1,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+6,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+1,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+6,c3)
     & +c(i,7,0,0)*ui(i1+7,i2+1,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+6,
     & c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+6,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+2,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+6,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+2,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+6,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+2,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+6,c3)+c(i,
     & 7,0,0)*ui(i1+7,i2+2,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+6,c3))+
     & c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+6,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+3,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+6,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+3,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+6,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+3,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+6,c3)+c(i,7,0,
     & 0)*ui(i1+7,i2+3,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+6,c3))+c(i,
     & 4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+4,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+4,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+6,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+4,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+6,c3))+c(i,5,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+5,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+5,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+5,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+6,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+5,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+6,c3))+c(i,6,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+6,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+6,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+6,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+6,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+6,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+6,c3))+c(i,7,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+7,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+7,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+7,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+6,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+7,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+6,c3))+c(i,8,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+8,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+8,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+8,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+8,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+8,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+6,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+8,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+6,c3)))+c(i,7,
     & 2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+7,c3)+c(i,1,0,0)*ui(
     & i1+1,i2,i3+7,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+7,c3)+c(i,3,0,0)*ui(
     & i1+3,i2,i3+7,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+7,c3)+c(i,5,0,0)*ui(
     & i1+5,i2,i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+7,c3)+c(i,7,0,0)*ui(
     & i1+7,i2,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2,i3+7,c3))+c(i,1,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+1,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+1,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+1,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+1,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+7,c3))+c(i,2,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+2,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+2,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+2,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+2,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+7,c3))+c(i,3,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+3,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+3,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+3,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+3,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+7,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+4,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+4,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+7,c3))+c(i,5,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+5,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+5,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+5,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+5,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+7,c3))+c(i,6,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+6,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+6,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+6,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+6,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+7,c3))+c(i,7,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+7,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+7,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+7,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+7,c3))+c(i,8,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+8,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+8,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+8,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+8,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+8,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+8,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+7,c3)))
               r(i) = r(i)+c(i,8,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,
     & i3+8,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2,
     & i3+8,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2,
     & i3+8,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2,
     & i3+8,c3)+c(i,7,0,0)*ui(i1+7,i2,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2,
     & i3+8,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+8,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+1,i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+8,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+1,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+8,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+1,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+8,c3)
     & +c(i,7,0,0)*ui(i1+7,i2+1,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+8,
     & c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+8,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+2,i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+8,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+2,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+8,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+2,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+8,c3)+c(i,
     & 7,0,0)*ui(i1+7,i2+2,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+8,c3))+
     & c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+8,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+3,i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+8,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+3,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+8,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+3,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+8,c3)+c(i,7,0,
     & 0)*ui(i1+7,i2+3,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+8,c3))+c(i,
     & 4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+8,c3)+c(i,1,0,0)*ui(i1+1,i2+4,
     & i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+8,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+8,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+4,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+8,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+4,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+8,c3))+c(i,5,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+8,c3)+c(i,1,0,0)*ui(i1+1,i2+5,
     & i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+8,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+5,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+8,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+5,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+8,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+5,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+8,c3))+c(i,6,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+8,c3)+c(i,1,0,0)*ui(i1+1,i2+6,
     & i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+8,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+6,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+8,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+6,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+8,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+6,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+8,c3))+c(i,7,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+8,c3)+c(i,1,0,0)*ui(i1+1,i2+7,
     & i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+8,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+7,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+8,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+7,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+8,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+7,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+8,c3))+c(i,8,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+8,i3+8,c3)+c(i,1,0,0)*ui(i1+1,i2+8,
     & i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+8,i3+8,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+8,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+8,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+8,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+8,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+8,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+8,c3)))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           end if
         else
           !   general case in 3D
           do c3=c3a,c3b
             do i=nia,nib
               r(i)=0.
             end do
             do w3=0,width(3)-1
               do w2=0,width(2)-1
                 do w1=0,width(1)-1
                   do i=nia,nib
                     r(i)=r(i)+c(i,w1,w2,w3)*ui(il(i,1)+w1,il(i,2)+w2,
     & il(i,3)+w3,c3)
                   end do
                 end do
               end do
             end do
             do i=nia,nib
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i)
             end do
           end do
         end if
       end if
! #If "TP" == "SP"
       else
         write(*,*) 'interpOpt:ERROR; unknown storage option=',
     & storageOption
       end if ! end storage option
       return
       end
! defineInterpOpt(TP)
       subroutine interpOptTP ( nd,ndui1a,ndui1b,ndui2a,ndui2b,ndui3a,
     & ndui3b,ndui4a,ndui4b,ndug1a,ndug1b,ndug2a,ndug2b,ndug3a,ndug3b,
     & ndug4a,ndug4b,ndil,ndip,ndc1,ndc2,ndc3,ipar,ui,ug,c,il,ip,
     & varWidth,width )
c=================================================================================
c  Optimised interpolation
c=================================================================================
       implicit none
       integer nd,nia,nib,c2a,c2b,c3a,c3b,ndil,ndip,ndc1,ndc2,ndc3
       integer ndui1a,ndui1b,ndui2a,ndui2b,ndui3a,ndui3b,ndui4a,ndui4b,
     & ndug1a,ndug1b,ndug2a,ndug2b,ndug3a,ndug3b,ndug4a,ndug4b
       real ui(ndui1a:ndui1b,ndui2a:ndui2b,ndui3a:ndui3b,ndui4a:ndui4b)
       real ug(ndug1a:ndug1b,ndug2a:ndug2b,ndug3a:ndug3b,ndug4a:ndug4b)
       real c(0:ndc1-1,0:ndc2-1,0:ndc3-1,0:*)
       integer width(3), il(0:ndil-1,*), ip(0:ndip-1,*), varWidth(0:*)
       integer ipar(0:*)
       integer storageOption,useVariableWidthInterpolation
       integer i,c2,c3,w1,w2,w3,i1,i2,i3,m2,m3
       real x
       real cr0,cr1,cr2,cr3,cr4,cr5,cr6,cr7,cr8,cr9,cr10
       real cs0,cs1,cs2,cs3,cs4,cs5,cs6,cs7,cs8,cs9,cs10
       real ct0,ct1,ct2,ct3,ct4,ct5,ct6,ct7,ct8,ct9,ct10
c real tpi2,tpi3,tpi4,tpi5,tpi6,tpi7,tpi8,tpi9
c real spi2,spi3,spi4,spi5,spi6,spi7,spi8,spi9
c ---- start statement functions
!       #Include "lagrangePolynomials.h"
      real q10,q20,q21,q30,q31,q32,q40,q41,q42,q43,q50,q51,q52,q53,q54,
     & q60,q61,q62,q63,q64,q65,q70,q71,q72,q73,q74,q75,q76,q80,q81,
     & q82,q83,q84,q85,q86,q87,q90,q91,q92,q93,q94,q95,q96,q97,q98
      q10(x)=1
      q20(x)=-x+1
      q21(x)=x
      q30(x)=(x-1)*(x-2)/2.
      q31(x)=-x*(x-2)
      q32(x)=x*(x-1)/2.
      q40(x)=-(x-1)*(x-2)*(x-3)/6.
      q41(x)=x*(x-2)*(x-3)/2.
      q42(x)=-x*(x-1)*(x-3)/2.
      q43(x)=x*(x-1)*(x-2)/6.
      q50(x)=(x-1)*(x-2)*(x-3)*(x-4)/24.
      q51(x)=-x*(x-2)*(x-3)*(x-4)/6.
      q52(x)=x*(x-1)*(x-3)*(x-4)/4.
      q53(x)=-x*(x-1)*(x-2)*(x-4)/6.
      q54(x)=x*(x-1)*(x-2)*(x-3)/24.
      q60(x)=-(x-1)*(x-2)*(x-3)*(x-4)*(x-5)/120.
      q61(x)=x*(x-2)*(x-3)*(x-4)*(x-5)/24.
      q62(x)=-x*(x-1)*(x-3)*(x-4)*(x-5)/12.
      q63(x)=x*(x-1)*(x-2)*(x-4)*(x-5)/12.
      q64(x)=-x*(x-1)*(x-2)*(x-3)*(x-5)/24.
      q65(x)=x*(x-1)*(x-2)*(x-3)*(x-4)/120.
      q70(x)=(x-1)*(x-2)*(x-3)*(x-4)*(x-5)*(x-6)/720.
      q71(x)=-x*(x-2)*(x-3)*(x-4)*(x-5)*(x-6)/120.
      q72(x)=x*(x-1)*(x-3)*(x-4)*(x-5)*(x-6)/48.
      q73(x)=-x*(x-1)*(x-2)*(x-4)*(x-5)*(x-6)/36.
      q74(x)=x*(x-1)*(x-2)*(x-3)*(x-5)*(x-6)/48.
      q75(x)=-x*(x-1)*(x-2)*(x-3)*(x-4)*(x-6)/120.
      q76(x)=x*(x-1)*(x-2)*(x-3)*(x-4)*(x-5)/720.
      q80(x)=-(x-1)*(x-2)*(x-3)*(x-4)*(x-5)*(x-6)*(x-7)/5040.
      q81(x)=x*(x-2)*(x-3)*(x-4)*(x-5)*(x-6)*(x-7)/720.
      q82(x)=-x*(x-1)*(x-3)*(x-4)*(x-5)*(x-6)*(x-7)/240.
      q83(x)=x*(x-1)*(x-2)*(x-4)*(x-5)*(x-6)*(x-7)/144.
      q84(x)=-x*(x-1)*(x-2)*(x-3)*(x-5)*(x-6)*(x-7)/144.
      q85(x)=x*(x-1)*(x-2)*(x-3)*(x-4)*(x-6)*(x-7)/240.
      q86(x)=-x*(x-1)*(x-2)*(x-3)*(x-4)*(x-5)*(x-7)/720.
      q87(x)=x*(x-1)*(x-2)*(x-3)*(x-4)*(x-5)*(x-6)/5040.
      q90(x)=(x-1)*(x-2)*(x-3)*(x-4)*(x-5)*(x-6)*(x-7)*(x-8)/40320.
      q91(x)=-x*(x-2)*(x-3)*(x-4)*(x-5)*(x-6)*(x-7)*(x-8)/5040.
      q92(x)=x*(x-1)*(x-3)*(x-4)*(x-5)*(x-6)*(x-7)*(x-8)/1440.
      q93(x)=-x*(x-1)*(x-2)*(x-4)*(x-5)*(x-6)*(x-7)*(x-8)/720.
      q94(x)=x*(x-1)*(x-2)*(x-3)*(x-5)*(x-6)*(x-7)*(x-8)/576.
      q95(x)=-x*(x-1)*(x-2)*(x-3)*(x-4)*(x-6)*(x-7)*(x-8)/720.
      q96(x)=x*(x-1)*(x-2)*(x-3)*(x-4)*(x-5)*(x-7)*(x-8)/1440.
      q97(x)=-x*(x-1)*(x-2)*(x-3)*(x-4)*(x-5)*(x-6)*(x-8)/5040.
      q98(x)=x*(x-1)*(x-2)*(x-3)*(x-4)*(x-5)*(x-6)*(x-7)/40320.
c      interpStatementFunctions
c ---- end statement functions
c write(*,*) 'interpOpt: width=',width(1),width(2)
        nia=ipar(0)
        nib=ipar(1)
        c2a=ipar(2)
        c2b=ipar(3)
        c3a=ipar(4)
        c3b=ipar(5)
        storageOption=ipar(6)
        useVariableWidthInterpolation=ipar(7)
! #If "TP" == "Full"
c **      else if( storageOption.eq.1 )then
! #If "TP" == "TP"
        if( storageOption.eq.1 )then
c       ****************************************
c       **** tensor-product storage option *****
c       ****************************************
       ! write(*,*) 'interpOpt:tensorProduct interp, width=',width(1)
       if( nd.eq.2 )then
         if( useVariableWidthInterpolation.ne.0 )then
! beginLoops2d()
             do i=nia,nib
             do c3=c3a,c3b
             do c2=c2a,c2b
             ! check for most common widths first
             if( varWidth(i).eq.3 )then
! interpTensorProduct33(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
               ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,1,0)*(c(i,0,0,0)*ui(
     & i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(
     & i1+2,i2  ,c2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1  ,i2+1,c2,c3)+c(
     & i,1,0,0)*ui(i1+1,i2+1,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,c2,c3))+c(
     & i,2,1,0)*(c(i,0,0,0)*ui(i1  ,i2+2,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+
     & 2,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,c2,c3))
             else if( varWidth(i).eq.2 )then
! interpTensorProduct22(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
               ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,1,0)*(c(i,0,0,0)*ui(
     & i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3))+c(i,1,1,0)*(c(
     & i,0,0,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,c2,c3))
             else if( varWidth(i).eq.1 )then
! interp11(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
               ug(ip(i,1),ip(i,2),c2,c3) = ui(i1  ,i2  ,c2,c3)
             else if( varWidth(i).eq.5 )then
! interpTensorProduct55(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
               ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,1,0)*(c(i,0,0,0)*ui(
     & i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(
     & i1+2,i2  ,c2,c3)+c(i,3,0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(
     & i1+4,i2  ,c2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1  ,i2+1,c2,c3)+c(
     & i,1,0,0)*ui(i1+1,i2+1,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,c2,c3)+c(
     & i,3,0,0)*ui(i1+3,i2+1,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+1,c2,c3))+c(
     & i,2,1,0)*(c(i,0,0,0)*ui(i1  ,i2+2,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+
     & 2,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+
     & 2,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+2,c2,c3))+c(i,3,1,0)*(c(i,0,0,0)
     & *ui(i1  ,i2+3,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+3,c2,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+3,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+3,c2,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+3,c2,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1  ,i2+4,c2,c3)
     & +c(i,1,0,0)*ui(i1+1,i2+4,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,c2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+4,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,c2,c3))
             else if( varWidth(i).eq.4 )then
! interpTensorProduct44(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
               ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,1,0)*(c(i,0,0,0)*ui(
     & i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(
     & i1+2,i2  ,c2,c3)+c(i,3,0,0)*ui(i1+3,i2  ,c2,c3))+c(i,1,1,0)*(c(
     & i,0,0,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,c2,c3)+c(
     & i,2,0,0)*ui(i1+2,i2+1,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,c2,c3))+c(
     & i,2,1,0)*(c(i,0,0,0)*ui(i1  ,i2+2,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+
     & 2,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+
     & 2,c2,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1  ,i2+3,c2,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+3,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+3,c2,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+3,c2,c3))
             else if( varWidth(i).eq.7 )then
! interpTensorProduct77(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
               ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,1,0)*(c(i,0,0,0)*ui(
     & i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(
     & i1+2,i2  ,c2,c3)+c(i,3,0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(
     & i1+4,i2  ,c2,c3)+c(i,5,0,0)*ui(i1+5,i2  ,c2,c3)+c(i,6,0,0)*ui(
     & i1+6,i2  ,c2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1  ,i2+1,c2,c3)+c(
     & i,1,0,0)*ui(i1+1,i2+1,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,c2,c3)+c(
     & i,3,0,0)*ui(i1+3,i2+1,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+1,c2,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+1,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+1,c2,c3))+c(
     & i,2,1,0)*(c(i,0,0,0)*ui(i1  ,i2+2,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+
     & 2,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+
     & 2,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+2,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+
     & 2,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+2,c2,c3))+c(i,3,1,0)*(c(i,0,0,0)
     & *ui(i1  ,i2+3,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+3,c2,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+3,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+3,c2,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+3,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+3,c2,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+3,c2,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1  ,i2+4,c2,c3)
     & +c(i,1,0,0)*ui(i1+1,i2+4,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,c2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+4,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,c2,c3)+
     & c(i,5,0,0)*ui(i1+5,i2+4,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+4,c2,c3))+
     & c(i,5,1,0)*(c(i,0,0,0)*ui(i1  ,i2+5,c2,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+5,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+5,c2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+5,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+5,c2,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+5,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+5,c2,c3))+c(i,6,1,0)*(c(i,0,
     & 0,0)*ui(i1  ,i2+6,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+6,c2,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+6,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+6,c2,c3)+c(i,4,
     & 0,0)*ui(i1+4,i2+6,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+6,c2,c3)+c(i,6,
     & 0,0)*ui(i1+6,i2+6,c2,c3))
             else if( varWidth(i).eq.6 )then
! interpTensorProduct66(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
               ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,1,0)*(c(i,0,0,0)*ui(
     & i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(
     & i1+2,i2  ,c2,c3)+c(i,3,0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(
     & i1+4,i2  ,c2,c3)+c(i,5,0,0)*ui(i1+5,i2  ,c2,c3))+c(i,1,1,0)*(c(
     & i,0,0,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,c2,c3)+c(
     & i,2,0,0)*ui(i1+2,i2+1,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,c2,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+1,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+1,c2,c3))+c(
     & i,2,1,0)*(c(i,0,0,0)*ui(i1  ,i2+2,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+
     & 2,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+
     & 2,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+2,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+
     & 2,c2,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1  ,i2+3,c2,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+3,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+3,c2,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+3,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+3,c2,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+3,c2,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1  ,i2+4,c2,c3)
     & +c(i,1,0,0)*ui(i1+1,i2+4,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,c2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+4,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,c2,c3)+
     & c(i,5,0,0)*ui(i1+5,i2+4,c2,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1  ,
     & i2+5,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+5,c2,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+5,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+5,c2,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+5,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+5,c2,c3))
             else if( varWidth(i).eq.9 )then
! interpTensorProduct99(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
               ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,1,0)*(c(i,0,0,0)*ui(
     & i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(
     & i1+2,i2  ,c2,c3)+c(i,3,0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(
     & i1+4,i2  ,c2,c3)+c(i,5,0,0)*ui(i1+5,i2  ,c2,c3)+c(i,6,0,0)*ui(
     & i1+6,i2  ,c2,c3)+c(i,7,0,0)*ui(i1+7,i2  ,c2,c3)+c(i,8,0,0)*ui(
     & i1+8,i2  ,c2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1  ,i2+1,c2,c3)+c(
     & i,1,0,0)*ui(i1+1,i2+1,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,c2,c3)+c(
     & i,3,0,0)*ui(i1+3,i2+1,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+1,c2,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+1,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+1,c2,c3)+c(
     & i,7,0,0)*ui(i1+7,i2+1,c2,c3)+c(i,8,0,0)*ui(i1+8,i2+1,c2,c3))+c(
     & i,2,1,0)*(c(i,0,0,0)*ui(i1  ,i2+2,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+
     & 2,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+
     & 2,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+2,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+
     & 2,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+2,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+
     & 2,c2,c3)+c(i,8,0,0)*ui(i1+8,i2+2,c2,c3))+c(i,3,1,0)*(c(i,0,0,0)
     & *ui(i1  ,i2+3,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+3,c2,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+3,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+3,c2,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+3,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+3,c2,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+3,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+3,c2,c3)+c(i,8,0,0)*
     & ui(i1+8,i2+3,c2,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1  ,i2+4,c2,c3)
     & +c(i,1,0,0)*ui(i1+1,i2+4,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,c2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+4,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,c2,c3)+
     & c(i,5,0,0)*ui(i1+5,i2+4,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+4,c2,c3)+
     & c(i,7,0,0)*ui(i1+7,i2+4,c2,c3)+c(i,8,0,0)*ui(i1+8,i2+4,c2,c3))+
     & c(i,5,1,0)*(c(i,0,0,0)*ui(i1  ,i2+5,c2,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+5,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+5,c2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+5,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+5,c2,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+5,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+5,c2,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+5,c2,c3)+c(i,8,0,0)*ui(i1+8,i2+5,c2,c3))+c(i,6,1,0)*(c(i,0,
     & 0,0)*ui(i1  ,i2+6,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+6,c2,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+6,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+6,c2,c3)+c(i,4,
     & 0,0)*ui(i1+4,i2+6,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+6,c2,c3)+c(i,6,
     & 0,0)*ui(i1+6,i2+6,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+6,c2,c3)+c(i,8,
     & 0,0)*ui(i1+8,i2+6,c2,c3))+c(i,7,1,0)*(c(i,0,0,0)*ui(i1  ,i2+7,
     & c2,c3)+c(i,1,0,0)*ui(i1+1,i2+7,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+7,
     & c2,c3)+c(i,3,0,0)*ui(i1+3,i2+7,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+7,
     & c2,c3)+c(i,5,0,0)*ui(i1+5,i2+7,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+7,
     & c2,c3)+c(i,7,0,0)*ui(i1+7,i2+7,c2,c3)+c(i,8,0,0)*ui(i1+8,i2+7,
     & c2,c3))+c(i,8,1,0)*(c(i,0,0,0)*ui(i1  ,i2+8,c2,c3)+c(i,1,0,0)*
     & ui(i1+1,i2+8,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+8,c2,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+8,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+8,c2,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+8,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+8,c2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+8,c2,c3)+c(i,8,0,0)*ui(i1+8,i2+8,c2,c3))
             else if( varWidth(i).eq.8 )then
! interpTensorProduct88(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
               ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,1,0)*(c(i,0,0,0)*ui(
     & i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(
     & i1+2,i2  ,c2,c3)+c(i,3,0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(
     & i1+4,i2  ,c2,c3)+c(i,5,0,0)*ui(i1+5,i2  ,c2,c3)+c(i,6,0,0)*ui(
     & i1+6,i2  ,c2,c3)+c(i,7,0,0)*ui(i1+7,i2  ,c2,c3))+c(i,1,1,0)*(c(
     & i,0,0,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,c2,c3)+c(
     & i,2,0,0)*ui(i1+2,i2+1,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,c2,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+1,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+1,c2,c3)+c(
     & i,6,0,0)*ui(i1+6,i2+1,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+1,c2,c3))+c(
     & i,2,1,0)*(c(i,0,0,0)*ui(i1  ,i2+2,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+
     & 2,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+
     & 2,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+2,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+
     & 2,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+2,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+
     & 2,c2,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1  ,i2+3,c2,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+3,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+3,c2,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+3,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+3,c2,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+3,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+3,c2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+3,c2,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1  ,i2+4,c2,c3)
     & +c(i,1,0,0)*ui(i1+1,i2+4,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,c2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+4,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,c2,c3)+
     & c(i,5,0,0)*ui(i1+5,i2+4,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+4,c2,c3)+
     & c(i,7,0,0)*ui(i1+7,i2+4,c2,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1  ,
     & i2+5,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+5,c2,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+5,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+5,c2,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+5,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+5,c2,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+5,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+5,c2,c3))+c(i,6,1,0)*(c(i,0,
     & 0,0)*ui(i1  ,i2+6,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+6,c2,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+6,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+6,c2,c3)+c(i,4,
     & 0,0)*ui(i1+4,i2+6,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+6,c2,c3)+c(i,6,
     & 0,0)*ui(i1+6,i2+6,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+6,c2,c3))+c(i,7,
     & 1,0)*(c(i,0,0,0)*ui(i1  ,i2+7,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+7,
     & c2,c3)+c(i,2,0,0)*ui(i1+2,i2+7,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+7,
     & c2,c3)+c(i,4,0,0)*ui(i1+4,i2+7,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & c2,c3)+c(i,6,0,0)*ui(i1+6,i2+7,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+7,
     & c2,c3))
             else
               write(*,*) 'ERROR varWidth=',varWidth(i)
               stop 151
             end if
! endLoops2d()
             end do
             end do
             end do
         else if( width(1).eq.3 .and. width(2).eq.3 ) then ! most common case
! loops2d($interpTensorProduct33(ug(ip(i,1),ip(i,2),c2,c3)),,)
           if( c2a.eq.c2b .and. c3a.eq.c3b )then
             do c3=c3a,c3b
             do c2=c2a,c2b
             do i=nia,nib
! interpTensorProduct33(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
               ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,1,0)*(c(i,0,0,0)*ui(
     & i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(
     & i1+2,i2  ,c2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1  ,i2+1,c2,c3)+c(
     & i,1,0,0)*ui(i1+1,i2+1,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,c2,c3))+c(
     & i,2,1,0)*(c(i,0,0,0)*ui(i1  ,i2+2,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+
     & 2,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,c2,c3))


             end do
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
             do c2=c2a,c2b
! interpTensorProduct33(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
               ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,1,0)*(c(i,0,0,0)*ui(
     & i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(
     & i1+2,i2  ,c2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1  ,i2+1,c2,c3)+c(
     & i,1,0,0)*ui(i1+1,i2+1,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,c2,c3))+c(
     & i,2,1,0)*(c(i,0,0,0)*ui(i1  ,i2+2,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+
     & 2,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,c2,c3))


             end do
             end do
             end do
           end if
         else if( width(1).eq.1 .and. width(1).eq.1)then
! loops2d($interp11(ug(ip(i,1),ip(i,2),c2,c3)),,)
           if( c2a.eq.c2b .and. c3a.eq.c3b )then
             do c3=c3a,c3b
             do c2=c2a,c2b
             do i=nia,nib
! interp11(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
               ug(ip(i,1),ip(i,2),c2,c3) = ui(i1  ,i2  ,c2,c3)


             end do
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
             do c2=c2a,c2b
! interp11(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
               ug(ip(i,1),ip(i,2),c2,c3) = ui(i1  ,i2  ,c2,c3)


             end do
             end do
             end do
           end if
         else if( width(1).eq.2 .and. width(2).eq.2 )then
! loops2d($interpTensorProduct22(ug(ip(i,1),ip(i,2),c2,c3)),,)
           if( c2a.eq.c2b .and. c3a.eq.c3b )then
             do c3=c3a,c3b
             do c2=c2a,c2b
             do i=nia,nib
! interpTensorProduct22(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
               ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,1,0)*(c(i,0,0,0)*ui(
     & i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3))+c(i,1,1,0)*(c(
     & i,0,0,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,c2,c3))


             end do
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
             do c2=c2a,c2b
! interpTensorProduct22(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
               ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,1,0)*(c(i,0,0,0)*ui(
     & i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3))+c(i,1,1,0)*(c(
     & i,0,0,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,c2,c3))


             end do
             end do
             end do
           end if
         else if( width(1).eq.4 .and. width(2).eq.4 )then
! loops2d($interpTensorProduct44(ug(ip(i,1),ip(i,2),c2,c3)),,)
           if( c2a.eq.c2b .and. c3a.eq.c3b )then
             do c3=c3a,c3b
             do c2=c2a,c2b
             do i=nia,nib
! interpTensorProduct44(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
               ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,1,0)*(c(i,0,0,0)*ui(
     & i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(
     & i1+2,i2  ,c2,c3)+c(i,3,0,0)*ui(i1+3,i2  ,c2,c3))+c(i,1,1,0)*(c(
     & i,0,0,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,c2,c3)+c(
     & i,2,0,0)*ui(i1+2,i2+1,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,c2,c3))+c(
     & i,2,1,0)*(c(i,0,0,0)*ui(i1  ,i2+2,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+
     & 2,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+
     & 2,c2,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1  ,i2+3,c2,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+3,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+3,c2,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+3,c2,c3))


             end do
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
             do c2=c2a,c2b
! interpTensorProduct44(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
               ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,1,0)*(c(i,0,0,0)*ui(
     & i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(
     & i1+2,i2  ,c2,c3)+c(i,3,0,0)*ui(i1+3,i2  ,c2,c3))+c(i,1,1,0)*(c(
     & i,0,0,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,c2,c3)+c(
     & i,2,0,0)*ui(i1+2,i2+1,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,c2,c3))+c(
     & i,2,1,0)*(c(i,0,0,0)*ui(i1  ,i2+2,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+
     & 2,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+
     & 2,c2,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1  ,i2+3,c2,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+3,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+3,c2,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+3,c2,c3))


             end do
             end do
             end do
           end if
         else if( width(1).eq.5 .and. width(2).eq.5 )then
! loops2d($interpTensorProduct55(ug(ip(i,1),ip(i,2),c2,c3)),,)
           if( c2a.eq.c2b .and. c3a.eq.c3b )then
             do c3=c3a,c3b
             do c2=c2a,c2b
             do i=nia,nib
! interpTensorProduct55(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
               ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,1,0)*(c(i,0,0,0)*ui(
     & i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(
     & i1+2,i2  ,c2,c3)+c(i,3,0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(
     & i1+4,i2  ,c2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1  ,i2+1,c2,c3)+c(
     & i,1,0,0)*ui(i1+1,i2+1,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,c2,c3)+c(
     & i,3,0,0)*ui(i1+3,i2+1,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+1,c2,c3))+c(
     & i,2,1,0)*(c(i,0,0,0)*ui(i1  ,i2+2,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+
     & 2,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+
     & 2,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+2,c2,c3))+c(i,3,1,0)*(c(i,0,0,0)
     & *ui(i1  ,i2+3,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+3,c2,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+3,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+3,c2,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+3,c2,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1  ,i2+4,c2,c3)
     & +c(i,1,0,0)*ui(i1+1,i2+4,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,c2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+4,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,c2,c3))


             end do
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
             do c2=c2a,c2b
! interpTensorProduct55(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
               ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,1,0)*(c(i,0,0,0)*ui(
     & i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(
     & i1+2,i2  ,c2,c3)+c(i,3,0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(
     & i1+4,i2  ,c2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1  ,i2+1,c2,c3)+c(
     & i,1,0,0)*ui(i1+1,i2+1,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,c2,c3)+c(
     & i,3,0,0)*ui(i1+3,i2+1,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+1,c2,c3))+c(
     & i,2,1,0)*(c(i,0,0,0)*ui(i1  ,i2+2,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+
     & 2,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+
     & 2,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+2,c2,c3))+c(i,3,1,0)*(c(i,0,0,0)
     & *ui(i1  ,i2+3,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+3,c2,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+3,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+3,c2,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+3,c2,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1  ,i2+4,c2,c3)
     & +c(i,1,0,0)*ui(i1+1,i2+4,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,c2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+4,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,c2,c3))


             end do
             end do
             end do
           end if
         else if( width(1).eq.6 .and. width(2).eq.6 )then
! loops2d($interpTensorProduct66(ug(ip(i,1),ip(i,2),c2,c3)),,)
           if( c2a.eq.c2b .and. c3a.eq.c3b )then
             do c3=c3a,c3b
             do c2=c2a,c2b
             do i=nia,nib
! interpTensorProduct66(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
               ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,1,0)*(c(i,0,0,0)*ui(
     & i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(
     & i1+2,i2  ,c2,c3)+c(i,3,0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(
     & i1+4,i2  ,c2,c3)+c(i,5,0,0)*ui(i1+5,i2  ,c2,c3))+c(i,1,1,0)*(c(
     & i,0,0,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,c2,c3)+c(
     & i,2,0,0)*ui(i1+2,i2+1,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,c2,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+1,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+1,c2,c3))+c(
     & i,2,1,0)*(c(i,0,0,0)*ui(i1  ,i2+2,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+
     & 2,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+
     & 2,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+2,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+
     & 2,c2,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1  ,i2+3,c2,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+3,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+3,c2,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+3,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+3,c2,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+3,c2,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1  ,i2+4,c2,c3)
     & +c(i,1,0,0)*ui(i1+1,i2+4,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,c2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+4,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,c2,c3)+
     & c(i,5,0,0)*ui(i1+5,i2+4,c2,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1  ,
     & i2+5,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+5,c2,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+5,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+5,c2,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+5,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+5,c2,c3))


             end do
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
             do c2=c2a,c2b
! interpTensorProduct66(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
               ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,1,0)*(c(i,0,0,0)*ui(
     & i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(
     & i1+2,i2  ,c2,c3)+c(i,3,0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(
     & i1+4,i2  ,c2,c3)+c(i,5,0,0)*ui(i1+5,i2  ,c2,c3))+c(i,1,1,0)*(c(
     & i,0,0,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,c2,c3)+c(
     & i,2,0,0)*ui(i1+2,i2+1,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,c2,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+1,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+1,c2,c3))+c(
     & i,2,1,0)*(c(i,0,0,0)*ui(i1  ,i2+2,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+
     & 2,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+
     & 2,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+2,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+
     & 2,c2,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1  ,i2+3,c2,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+3,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+3,c2,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+3,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+3,c2,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+3,c2,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1  ,i2+4,c2,c3)
     & +c(i,1,0,0)*ui(i1+1,i2+4,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,c2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+4,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,c2,c3)+
     & c(i,5,0,0)*ui(i1+5,i2+4,c2,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1  ,
     & i2+5,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+5,c2,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+5,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+5,c2,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+5,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+5,c2,c3))


             end do
             end do
             end do
           end if
         else if( width(1).eq.7 .and. width(2).eq.7 )then
! loops2d($interpTensorProduct77(ug(ip(i,1),ip(i,2),c2,c3)),,)
           if( c2a.eq.c2b .and. c3a.eq.c3b )then
             do c3=c3a,c3b
             do c2=c2a,c2b
             do i=nia,nib
! interpTensorProduct77(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
               ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,1,0)*(c(i,0,0,0)*ui(
     & i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(
     & i1+2,i2  ,c2,c3)+c(i,3,0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(
     & i1+4,i2  ,c2,c3)+c(i,5,0,0)*ui(i1+5,i2  ,c2,c3)+c(i,6,0,0)*ui(
     & i1+6,i2  ,c2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1  ,i2+1,c2,c3)+c(
     & i,1,0,0)*ui(i1+1,i2+1,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,c2,c3)+c(
     & i,3,0,0)*ui(i1+3,i2+1,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+1,c2,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+1,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+1,c2,c3))+c(
     & i,2,1,0)*(c(i,0,0,0)*ui(i1  ,i2+2,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+
     & 2,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+
     & 2,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+2,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+
     & 2,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+2,c2,c3))+c(i,3,1,0)*(c(i,0,0,0)
     & *ui(i1  ,i2+3,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+3,c2,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+3,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+3,c2,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+3,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+3,c2,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+3,c2,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1  ,i2+4,c2,c3)
     & +c(i,1,0,0)*ui(i1+1,i2+4,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,c2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+4,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,c2,c3)+
     & c(i,5,0,0)*ui(i1+5,i2+4,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+4,c2,c3))+
     & c(i,5,1,0)*(c(i,0,0,0)*ui(i1  ,i2+5,c2,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+5,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+5,c2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+5,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+5,c2,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+5,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+5,c2,c3))+c(i,6,1,0)*(c(i,0,
     & 0,0)*ui(i1  ,i2+6,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+6,c2,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+6,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+6,c2,c3)+c(i,4,
     & 0,0)*ui(i1+4,i2+6,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+6,c2,c3)+c(i,6,
     & 0,0)*ui(i1+6,i2+6,c2,c3))


             end do
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
             do c2=c2a,c2b
! interpTensorProduct77(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
               ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,1,0)*(c(i,0,0,0)*ui(
     & i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(
     & i1+2,i2  ,c2,c3)+c(i,3,0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(
     & i1+4,i2  ,c2,c3)+c(i,5,0,0)*ui(i1+5,i2  ,c2,c3)+c(i,6,0,0)*ui(
     & i1+6,i2  ,c2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1  ,i2+1,c2,c3)+c(
     & i,1,0,0)*ui(i1+1,i2+1,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,c2,c3)+c(
     & i,3,0,0)*ui(i1+3,i2+1,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+1,c2,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+1,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+1,c2,c3))+c(
     & i,2,1,0)*(c(i,0,0,0)*ui(i1  ,i2+2,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+
     & 2,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+
     & 2,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+2,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+
     & 2,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+2,c2,c3))+c(i,3,1,0)*(c(i,0,0,0)
     & *ui(i1  ,i2+3,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+3,c2,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+3,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+3,c2,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+3,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+3,c2,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+3,c2,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1  ,i2+4,c2,c3)
     & +c(i,1,0,0)*ui(i1+1,i2+4,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,c2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+4,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,c2,c3)+
     & c(i,5,0,0)*ui(i1+5,i2+4,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+4,c2,c3))+
     & c(i,5,1,0)*(c(i,0,0,0)*ui(i1  ,i2+5,c2,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+5,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+5,c2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+5,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+5,c2,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+5,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+5,c2,c3))+c(i,6,1,0)*(c(i,0,
     & 0,0)*ui(i1  ,i2+6,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+6,c2,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+6,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+6,c2,c3)+c(i,4,
     & 0,0)*ui(i1+4,i2+6,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+6,c2,c3)+c(i,6,
     & 0,0)*ui(i1+6,i2+6,c2,c3))


             end do
             end do
             end do
           end if
         else if( width(1).eq.8 .and. width(2).eq.8 )then
! loops2d($interpTensorProduct88(ug(ip(i,1),ip(i,2),c2,c3)),,)
           if( c2a.eq.c2b .and. c3a.eq.c3b )then
             do c3=c3a,c3b
             do c2=c2a,c2b
             do i=nia,nib
! interpTensorProduct88(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
               ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,1,0)*(c(i,0,0,0)*ui(
     & i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(
     & i1+2,i2  ,c2,c3)+c(i,3,0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(
     & i1+4,i2  ,c2,c3)+c(i,5,0,0)*ui(i1+5,i2  ,c2,c3)+c(i,6,0,0)*ui(
     & i1+6,i2  ,c2,c3)+c(i,7,0,0)*ui(i1+7,i2  ,c2,c3))+c(i,1,1,0)*(c(
     & i,0,0,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,c2,c3)+c(
     & i,2,0,0)*ui(i1+2,i2+1,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,c2,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+1,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+1,c2,c3)+c(
     & i,6,0,0)*ui(i1+6,i2+1,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+1,c2,c3))+c(
     & i,2,1,0)*(c(i,0,0,0)*ui(i1  ,i2+2,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+
     & 2,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+
     & 2,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+2,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+
     & 2,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+2,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+
     & 2,c2,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1  ,i2+3,c2,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+3,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+3,c2,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+3,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+3,c2,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+3,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+3,c2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+3,c2,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1  ,i2+4,c2,c3)
     & +c(i,1,0,0)*ui(i1+1,i2+4,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,c2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+4,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,c2,c3)+
     & c(i,5,0,0)*ui(i1+5,i2+4,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+4,c2,c3)+
     & c(i,7,0,0)*ui(i1+7,i2+4,c2,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1  ,
     & i2+5,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+5,c2,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+5,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+5,c2,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+5,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+5,c2,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+5,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+5,c2,c3))+c(i,6,1,0)*(c(i,0,
     & 0,0)*ui(i1  ,i2+6,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+6,c2,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+6,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+6,c2,c3)+c(i,4,
     & 0,0)*ui(i1+4,i2+6,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+6,c2,c3)+c(i,6,
     & 0,0)*ui(i1+6,i2+6,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+6,c2,c3))+c(i,7,
     & 1,0)*(c(i,0,0,0)*ui(i1  ,i2+7,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+7,
     & c2,c3)+c(i,2,0,0)*ui(i1+2,i2+7,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+7,
     & c2,c3)+c(i,4,0,0)*ui(i1+4,i2+7,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & c2,c3)+c(i,6,0,0)*ui(i1+6,i2+7,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+7,
     & c2,c3))


             end do
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
             do c2=c2a,c2b
! interpTensorProduct88(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
               ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,1,0)*(c(i,0,0,0)*ui(
     & i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(
     & i1+2,i2  ,c2,c3)+c(i,3,0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(
     & i1+4,i2  ,c2,c3)+c(i,5,0,0)*ui(i1+5,i2  ,c2,c3)+c(i,6,0,0)*ui(
     & i1+6,i2  ,c2,c3)+c(i,7,0,0)*ui(i1+7,i2  ,c2,c3))+c(i,1,1,0)*(c(
     & i,0,0,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,c2,c3)+c(
     & i,2,0,0)*ui(i1+2,i2+1,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,c2,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+1,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+1,c2,c3)+c(
     & i,6,0,0)*ui(i1+6,i2+1,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+1,c2,c3))+c(
     & i,2,1,0)*(c(i,0,0,0)*ui(i1  ,i2+2,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+
     & 2,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+
     & 2,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+2,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+
     & 2,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+2,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+
     & 2,c2,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1  ,i2+3,c2,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+3,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+3,c2,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+3,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+3,c2,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+3,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+3,c2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+3,c2,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1  ,i2+4,c2,c3)
     & +c(i,1,0,0)*ui(i1+1,i2+4,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,c2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+4,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,c2,c3)+
     & c(i,5,0,0)*ui(i1+5,i2+4,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+4,c2,c3)+
     & c(i,7,0,0)*ui(i1+7,i2+4,c2,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1  ,
     & i2+5,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+5,c2,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+5,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+5,c2,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+5,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+5,c2,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+5,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+5,c2,c3))+c(i,6,1,0)*(c(i,0,
     & 0,0)*ui(i1  ,i2+6,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+6,c2,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+6,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+6,c2,c3)+c(i,4,
     & 0,0)*ui(i1+4,i2+6,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+6,c2,c3)+c(i,6,
     & 0,0)*ui(i1+6,i2+6,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+6,c2,c3))+c(i,7,
     & 1,0)*(c(i,0,0,0)*ui(i1  ,i2+7,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+7,
     & c2,c3)+c(i,2,0,0)*ui(i1+2,i2+7,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+7,
     & c2,c3)+c(i,4,0,0)*ui(i1+4,i2+7,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & c2,c3)+c(i,6,0,0)*ui(i1+6,i2+7,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+7,
     & c2,c3))


             end do
             end do
             end do
           end if
         else if( width(1).eq.9 .and. width(2).eq.9 )then
! loops2d($interpTensorProduct99(ug(ip(i,1),ip(i,2),c2,c3)),,)
           if( c2a.eq.c2b .and. c3a.eq.c3b )then
             do c3=c3a,c3b
             do c2=c2a,c2b
             do i=nia,nib
! interpTensorProduct99(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
               ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,1,0)*(c(i,0,0,0)*ui(
     & i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(
     & i1+2,i2  ,c2,c3)+c(i,3,0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(
     & i1+4,i2  ,c2,c3)+c(i,5,0,0)*ui(i1+5,i2  ,c2,c3)+c(i,6,0,0)*ui(
     & i1+6,i2  ,c2,c3)+c(i,7,0,0)*ui(i1+7,i2  ,c2,c3)+c(i,8,0,0)*ui(
     & i1+8,i2  ,c2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1  ,i2+1,c2,c3)+c(
     & i,1,0,0)*ui(i1+1,i2+1,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,c2,c3)+c(
     & i,3,0,0)*ui(i1+3,i2+1,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+1,c2,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+1,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+1,c2,c3)+c(
     & i,7,0,0)*ui(i1+7,i2+1,c2,c3)+c(i,8,0,0)*ui(i1+8,i2+1,c2,c3))+c(
     & i,2,1,0)*(c(i,0,0,0)*ui(i1  ,i2+2,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+
     & 2,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+
     & 2,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+2,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+
     & 2,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+2,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+
     & 2,c2,c3)+c(i,8,0,0)*ui(i1+8,i2+2,c2,c3))+c(i,3,1,0)*(c(i,0,0,0)
     & *ui(i1  ,i2+3,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+3,c2,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+3,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+3,c2,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+3,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+3,c2,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+3,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+3,c2,c3)+c(i,8,0,0)*
     & ui(i1+8,i2+3,c2,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1  ,i2+4,c2,c3)
     & +c(i,1,0,0)*ui(i1+1,i2+4,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,c2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+4,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,c2,c3)+
     & c(i,5,0,0)*ui(i1+5,i2+4,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+4,c2,c3)+
     & c(i,7,0,0)*ui(i1+7,i2+4,c2,c3)+c(i,8,0,0)*ui(i1+8,i2+4,c2,c3))+
     & c(i,5,1,0)*(c(i,0,0,0)*ui(i1  ,i2+5,c2,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+5,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+5,c2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+5,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+5,c2,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+5,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+5,c2,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+5,c2,c3)+c(i,8,0,0)*ui(i1+8,i2+5,c2,c3))+c(i,6,1,0)*(c(i,0,
     & 0,0)*ui(i1  ,i2+6,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+6,c2,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+6,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+6,c2,c3)+c(i,4,
     & 0,0)*ui(i1+4,i2+6,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+6,c2,c3)+c(i,6,
     & 0,0)*ui(i1+6,i2+6,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+6,c2,c3)+c(i,8,
     & 0,0)*ui(i1+8,i2+6,c2,c3))+c(i,7,1,0)*(c(i,0,0,0)*ui(i1  ,i2+7,
     & c2,c3)+c(i,1,0,0)*ui(i1+1,i2+7,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+7,
     & c2,c3)+c(i,3,0,0)*ui(i1+3,i2+7,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+7,
     & c2,c3)+c(i,5,0,0)*ui(i1+5,i2+7,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+7,
     & c2,c3)+c(i,7,0,0)*ui(i1+7,i2+7,c2,c3)+c(i,8,0,0)*ui(i1+8,i2+7,
     & c2,c3))+c(i,8,1,0)*(c(i,0,0,0)*ui(i1  ,i2+8,c2,c3)+c(i,1,0,0)*
     & ui(i1+1,i2+8,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+8,c2,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+8,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+8,c2,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+8,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+8,c2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+8,c2,c3)+c(i,8,0,0)*ui(i1+8,i2+8,c2,c3))


             end do
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
             do c2=c2a,c2b
! interpTensorProduct99(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
               ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,1,0)*(c(i,0,0,0)*ui(
     & i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(
     & i1+2,i2  ,c2,c3)+c(i,3,0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(
     & i1+4,i2  ,c2,c3)+c(i,5,0,0)*ui(i1+5,i2  ,c2,c3)+c(i,6,0,0)*ui(
     & i1+6,i2  ,c2,c3)+c(i,7,0,0)*ui(i1+7,i2  ,c2,c3)+c(i,8,0,0)*ui(
     & i1+8,i2  ,c2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1  ,i2+1,c2,c3)+c(
     & i,1,0,0)*ui(i1+1,i2+1,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,c2,c3)+c(
     & i,3,0,0)*ui(i1+3,i2+1,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+1,c2,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+1,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+1,c2,c3)+c(
     & i,7,0,0)*ui(i1+7,i2+1,c2,c3)+c(i,8,0,0)*ui(i1+8,i2+1,c2,c3))+c(
     & i,2,1,0)*(c(i,0,0,0)*ui(i1  ,i2+2,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+
     & 2,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+
     & 2,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+2,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+
     & 2,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+2,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+
     & 2,c2,c3)+c(i,8,0,0)*ui(i1+8,i2+2,c2,c3))+c(i,3,1,0)*(c(i,0,0,0)
     & *ui(i1  ,i2+3,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+3,c2,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+3,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+3,c2,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+3,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+3,c2,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+3,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+3,c2,c3)+c(i,8,0,0)*
     & ui(i1+8,i2+3,c2,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1  ,i2+4,c2,c3)
     & +c(i,1,0,0)*ui(i1+1,i2+4,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,c2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+4,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,c2,c3)+
     & c(i,5,0,0)*ui(i1+5,i2+4,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+4,c2,c3)+
     & c(i,7,0,0)*ui(i1+7,i2+4,c2,c3)+c(i,8,0,0)*ui(i1+8,i2+4,c2,c3))+
     & c(i,5,1,0)*(c(i,0,0,0)*ui(i1  ,i2+5,c2,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+5,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+5,c2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+5,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+5,c2,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+5,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+5,c2,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+5,c2,c3)+c(i,8,0,0)*ui(i1+8,i2+5,c2,c3))+c(i,6,1,0)*(c(i,0,
     & 0,0)*ui(i1  ,i2+6,c2,c3)+c(i,1,0,0)*ui(i1+1,i2+6,c2,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+6,c2,c3)+c(i,3,0,0)*ui(i1+3,i2+6,c2,c3)+c(i,4,
     & 0,0)*ui(i1+4,i2+6,c2,c3)+c(i,5,0,0)*ui(i1+5,i2+6,c2,c3)+c(i,6,
     & 0,0)*ui(i1+6,i2+6,c2,c3)+c(i,7,0,0)*ui(i1+7,i2+6,c2,c3)+c(i,8,
     & 0,0)*ui(i1+8,i2+6,c2,c3))+c(i,7,1,0)*(c(i,0,0,0)*ui(i1  ,i2+7,
     & c2,c3)+c(i,1,0,0)*ui(i1+1,i2+7,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+7,
     & c2,c3)+c(i,3,0,0)*ui(i1+3,i2+7,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+7,
     & c2,c3)+c(i,5,0,0)*ui(i1+5,i2+7,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+7,
     & c2,c3)+c(i,7,0,0)*ui(i1+7,i2+7,c2,c3)+c(i,8,0,0)*ui(i1+8,i2+7,
     & c2,c3))+c(i,8,1,0)*(c(i,0,0,0)*ui(i1  ,i2+8,c2,c3)+c(i,1,0,0)*
     & ui(i1+1,i2+8,c2,c3)+c(i,2,0,0)*ui(i1+2,i2+8,c2,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+8,c2,c3)+c(i,4,0,0)*ui(i1+4,i2+8,c2,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+8,c2,c3)+c(i,6,0,0)*ui(i1+6,i2+8,c2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+8,c2,c3)+c(i,8,0,0)*ui(i1+8,i2+8,c2,c3))


             end do
             end do
             end do
           end if
         else
c           general case in 2D ****fix this*****
c write(*,*)'interpOpt:WARNING:Gen case width=',width(1),width(2)
                  stop 2
         end if
       else
c     *** 3D ****
         if( useVariableWidthInterpolation.ne.0 )then
! beginLoops3d()
             do i=nia,nib
             do c3=c3a,c3b
             ! check for most common widths first
             if( varWidth(i).eq.3 )then
! interpTensorProduct333(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,2,0)*(c(i,0,1,0)*
     & (c(i,0,0,0)*ui(i1,i2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3)+c(
     & i,2,0,0)*ui(i1+2,i2,i3+0,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 1,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+0,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+1,i3+0,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+0,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+0,c3)))
     & +c(i,1,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+1,c3)+c(i,1,0,
     & 0)*ui(i1+1,i2,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+1,c3))+c(i,1,1,
     & 0)*(c(i,0,0,0)*ui(i1,i2+1,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+
     & 1,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+1,c3))+c(i,2,1,0)*(c(i,0,0,0)*
     & ui(i1,i2+2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,0,0)
     & *ui(i1+2,i2+2,i3+1,c3)))+c(i,2,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(
     & i1,i2,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,0)*ui(i1+
     & 2,i2,i3+2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+2,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+1,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+2,c3))+
     & c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+2,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+2,c3)))
             else if( varWidth(i).eq.2 )then
! interpTensorProduct222(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,2,0)*(c(i,0,1,0)*
     & (c(i,0,0,0)*ui(i1,i2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3))+
     & c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+0,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+1,i3+0,c3)))+c(i,1,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+
     & 1,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+1,c3))+c(i,1,1,0)*(c(i,0,0,0)*
     & ui(i1,i2+1,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+1,c3)))
             else if( varWidth(i).eq.1 )then
! interp111(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ui(i1,i2,i3,c3)
             else if( varWidth(i).eq.5 )then
! interpTensorProduct555(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,2,0)*(c(i,0,1,0)*
     & (c(i,0,0,0)*ui(i1,i2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3)+c(
     & i,2,0,0)*ui(i1+2,i2,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+0,c3)+c(
     & i,4,0,0)*ui(i1+4,i2,i3+0,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 1,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+0,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+1,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+0,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+1,i3+0,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+0,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+0,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+2,
     & i3+0,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+0,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+3,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+0,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+3,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+0,c3))+c(
     & i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+
     & 4,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+0,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+0,c3)))+c(i,1,2,0)*(c(
     & i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2,
     & i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2,
     & i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+1,c3))+c(i,1,1,0)*(c(i,0,0,0)
     & *ui(i1,i2+1,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+1,c3)+c(i,2,0,
     & 0)*ui(i1+2,i2+1,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+1,c3)+c(i,
     & 4,0,0)*ui(i1+4,i2+1,i3+1,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+2,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+1,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+2,i3+1,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+1,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+3,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+1,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+3,
     & i3+1,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+1,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+4,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+1,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+4,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+1,c3)))+
     & c(i,2,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+2,c3)+c(i,1,0,0)
     & *ui(i1+1,i2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+2,c3)+c(i,3,0,0)*
     & ui(i1+3,i2,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+2,c3))+c(i,1,1,0)*
     & (c(i,0,0,0)*ui(i1,i2+1,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+2,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,
     & i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+2,c3))+c(i,2,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+2,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+2,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+2,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+2,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+2,i3+2,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+3,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+2,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+3,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+2,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+3,i3+2,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+2,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,
     & i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+2,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+4,i3+2,c3)))+c(i,3,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+
     & 3,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+
     & 3,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+
     & 3,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+3,c3)+c(i,1,0,0)*
     & ui(i1+1,i2+1,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,0,
     & 0)*ui(i1+3,i2+1,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+3,c3))+c(i,
     & 2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+3,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+3,c3))+c(i,3,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+3,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+3,c3))+c(i,4,1,0)*(c(i,0,0,0)*
     & ui(i1,i2+4,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+3,c3)+c(i,2,0,0)
     & *ui(i1+2,i2+4,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+3,c3)+c(i,4,
     & 0,0)*ui(i1+4,i2+4,i3+3,c3)))+c(i,4,2,0)*(c(i,0,1,0)*(c(i,0,0,0)
     & *ui(i1,i2,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,0)*
     & ui(i1+2,i2,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,0)*
     & ui(i1+4,i2,i3+4,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+4,c3)
     & +c(i,1,0,0)*ui(i1+1,i2+1,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+4,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+1,
     & i3+4,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+4,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+2,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+4,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+4,c3))+c(
     & i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+
     & 3,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+4,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+4,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+4,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+4,c3)))
             else if( varWidth(i).eq.4 )then
! interpTensorProduct444(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,2,0)*(c(i,0,1,0)*
     & (c(i,0,0,0)*ui(i1,i2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3)+c(
     & i,2,0,0)*ui(i1+2,i2,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+0,c3))+c(
     & i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+
     & 1,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+0,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+1,i3+0,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+0,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+0,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+2,i3+0,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+3,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+0,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+3,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+0,c3)))+c(i,1,2,
     & 0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+1,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+1,c3)+c(i,3,0,0)*ui(i1+
     & 3,i2,i3+1,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+1,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+1,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+1,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+1,i3+1,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+2,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+1,c3))+c(i,3,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+3,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+1,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+3,
     & i3+1,c3)))+c(i,2,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+2,c3)
     & +c(i,1,0,0)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2,i3+2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+1,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+2,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+1,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+2,c3))+c(i,2,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+2,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+2,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+2,
     & i3+2,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+2,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+3,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+2,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+3,i3+2,c3)))+c(i,3,2,0)*(c(i,0,1,0)*(c(i,0,0,0)
     & *ui(i1,i2,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+3,c3)+c(i,2,0,0)*
     & ui(i1+2,i2,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+3,c3))+c(i,1,1,0)*
     & (c(i,0,0,0)*ui(i1,i2+1,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+3,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+1,
     & i3+3,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+3,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+2,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+3,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+2,i3+3,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,
     & i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+3,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+3,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+3,c3)))
             else if( varWidth(i).eq.7 )then
! interpTensorProduct777(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,2,0)*(c(i,0,1,0)*
     & (c(i,0,0,0)*ui(i1,i2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3)+c(
     & i,2,0,0)*ui(i1+2,i2,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+0,c3)+c(
     & i,4,0,0)*ui(i1+4,i2,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+0,c3)+c(
     & i,6,0,0)*ui(i1+6,i2,i3+0,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 1,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+0,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+1,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+0,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+1,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+0,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+1,i3+0,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+2,
     & i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+0,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+2,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+0,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+2,i3+0,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+0,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+3,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+0,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+3,
     & i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+0,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+3,i3+0,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+0,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+4,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+0,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+4,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+0,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+4,
     & i3+0,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+0,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+5,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+0,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+5,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+0,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+5,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+0,c3)
     & )+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+0,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2+6,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+0,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+6,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+0,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+6,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+0,c3)))+c(i,1,
     & 2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+1,c3)+c(i,1,0,0)*ui(
     & i1+1,i2,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+1,c3)+c(i,3,0,0)*ui(
     & i1+3,i2,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+1,c3)+c(i,5,0,0)*ui(
     & i1+5,i2,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+1,c3))+c(i,1,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+1,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+1,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+1,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+1,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+1,
     & i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+1,c3))+c(i,2,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+2,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+1,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+2,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+1,c3)
     & +c(i,6,0,0)*ui(i1+6,i2+2,i3+1,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(
     & i1,i2+3,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+1,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+3,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+1,c3)+c(i,4,0,
     & 0)*ui(i1+4,i2+3,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+1,c3)+c(i,
     & 6,0,0)*ui(i1+6,i2+3,i3+1,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 4,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+1,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+4,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+1,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+4,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+1,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+4,i3+1,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+1,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+5,
     & i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+1,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+5,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+1,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+5,i3+1,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+1,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+6,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+1,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+6,
     & i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+1,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+6,i3+1,c3)))+c(i,2,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+
     & 2,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+
     & 2,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+
     & 2,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+
     & 2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+2,c3)+c(i,1,0,0)*
     & ui(i1+1,i2+1,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+2,c3)+c(i,3,0,
     & 0)*ui(i1+3,i2+1,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+2,c3)+c(i,
     & 5,0,0)*ui(i1+5,i2+1,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+2,c3))+
     & c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+2,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+2,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+2,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+2,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+2,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+2,c3))+c(i,3,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+3,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+3,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+2,c3))+c(i,4,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+4,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+2,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+4,
     & i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+2,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+4,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+2,c3))+c(i,5,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+5,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+2,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+5,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+2,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+5,
     & i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+2,c3))+c(i,6,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+6,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+2,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+6,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+2,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+6,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+2,c3)
     & +c(i,6,0,0)*ui(i1+6,i2+6,i3+2,c3)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,3,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+3,c3)+
     & c(i,1,0,0)*ui(i1+1,i2,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+3,c3)+
     & c(i,3,0,0)*ui(i1+3,i2,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+3,c3)+
     & c(i,5,0,0)*ui(i1+5,i2,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+3,c3))+
     & c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+3,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+1,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+1,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+3,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+1,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+3,c3))+c(i,2,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+3,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+3,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+2,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+3,c3))+c(i,3,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+3,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+3,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+3,
     & i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+3,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+3,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+3,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+4,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+3,c3))+c(i,5,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+5,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+3,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+5,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+3,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+5,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+3,c3)
     & +c(i,6,0,0)*ui(i1+6,i2+5,i3+3,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(
     & i1,i2+6,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+3,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+6,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+3,c3)+c(i,4,0,
     & 0)*ui(i1+4,i2+6,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+3,c3)+c(i,
     & 6,0,0)*ui(i1+6,i2+6,i3+3,c3)))+c(i,4,2,0)*(c(i,0,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,0)*
     & ui(i1+2,i2,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,0)*
     & ui(i1+4,i2,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+4,c3)+c(i,6,0,0)*
     & ui(i1+6,i2,i3+4,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+4,c3)
     & +c(i,1,0,0)*ui(i1+1,i2+1,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+4,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+1,
     & i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+4,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+1,i3+4,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+4,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+2,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+4,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+4,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+2,
     & i3+4,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+4,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+3,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+4,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+3,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+4,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+3,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+4,c3)
     & )+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+4,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2+4,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+4,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+4,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+4,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+4,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+4,c3))+c(i,5,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+5,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+5,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+5,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+4,c3))+c(i,6,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+6,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+4,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+6,
     & i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+4,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+6,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+4,c3)))+c(i,5,2,0)*(c(
     & i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2,
     & i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2,
     & i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+5,c3))+c(i,1,1,0)*(c(i,0,0,0)
     & *ui(i1,i2+1,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+5,c3)+c(i,2,0,
     & 0)*ui(i1+2,i2+1,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+5,c3)+c(i,
     & 4,0,0)*ui(i1+4,i2+1,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+5,c3)+
     & c(i,6,0,0)*ui(i1+6,i2+1,i3+5,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+2,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+5,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+2,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+5,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+2,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+5,c3)+c(i,6,0,
     & 0)*ui(i1+6,i2+2,i3+5,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+
     & 5,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+3,
     & i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+5,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+3,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+5,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+3,i3+5,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+5,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+4,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+5,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+4,
     & i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+5,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+4,i3+5,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+5,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+5,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+5,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+5,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+5,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+5,
     & i3+5,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+5,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+6,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+5,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+6,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+5,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+6,i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+5,c3)
     & ))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,6,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+6,c3)+
     & c(i,1,0,0)*ui(i1+1,i2,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+6,c3)+
     & c(i,3,0,0)*ui(i1+3,i2,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+6,c3)+
     & c(i,5,0,0)*ui(i1+5,i2,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+6,c3))+
     & c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+6,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+1,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+6,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+1,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+6,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+1,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+6,c3))+c(i,2,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+2,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+6,c3))+c(i,3,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+3,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+6,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+3,
     & i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+6,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+3,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+6,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+6,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+6,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2+4,
     & i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+6,c3))+c(i,5,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+5,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+6,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+5,i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+6,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+5,i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+6,c3)
     & +c(i,6,0,0)*ui(i1+6,i2+5,i3+6,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(
     & i1,i2+6,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+6,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+6,i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+6,c3)+c(i,4,0,
     & 0)*ui(i1+4,i2+6,i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+6,c3)+c(i,
     & 6,0,0)*ui(i1+6,i2+6,i3+6,c3)))
             else if( varWidth(i).eq.6 )then
! interpTensorProduct666(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,2,0)*(c(i,0,1,0)*
     & (c(i,0,0,0)*ui(i1,i2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3)+c(
     & i,2,0,0)*ui(i1+2,i2,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+0,c3)+c(
     & i,4,0,0)*ui(i1+4,i2,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+0,c3))+c(
     & i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+
     & 1,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+0,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+1,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+0,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+1,i3+0,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+0,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+0,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+2,
     & i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+0,c3))+c(i,3,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+3,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+0,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+3,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+0,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+3,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+0,c3)
     & )+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+0,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2+4,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+0,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+4,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+0,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+4,i3+0,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+5,
     & i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+0,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+5,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+0,c3)))+c(i,1,2,0)*(c(
     & i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2,
     & i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2,
     & i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2,
     & i3+1,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+1,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+1,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+1,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+1,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+1,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+1,i3+1,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+2,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+1,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+2,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+1,c3))+c(i,3,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+3,
     & i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+1,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+1,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+3,i3+1,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+1,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+4,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+1,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+4,
     & i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+1,c3))+c(i,5,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+5,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+1,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+5,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+1,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+5,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+1,c3)
     & ))+c(i,2,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+2,c3)+c(i,1,
     & 0,0)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+2,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+2,c3)+c(i,5,
     & 0,0)*ui(i1+5,i2,i3+2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+
     & 2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,
     & i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+2,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+1,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+2,c3))+c(i,2,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+2,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+2,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+2,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+2,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+2,
     & i3+2,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+2,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+3,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+2,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+3,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+2,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+3,i3+2,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+4,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+2,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+4,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+2,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+4,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+2,c3))+c(i,5,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+5,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+5,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+5,i3+2,c3)))+c(i,3,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,
     & i2,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+3,c3)+c(i,2,0,0)*ui(i1+2,
     & i2,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+3,c3)+c(i,4,0,0)*ui(i1+4,
     & i2,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+3,c3))+c(i,1,1,0)*(c(i,0,
     & 0,0)*ui(i1,i2+1,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+3,c3)+c(i,
     & 2,0,0)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+3,c3)+
     & c(i,4,0,0)*ui(i1+4,i2+1,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+3,
     & c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+3,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+2,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+3,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+2,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+3,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+2,i3+3,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+
     & 3,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+3,
     & i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+3,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+3,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+3,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+4,
     & i3+3,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+3,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+5,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+3,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+5,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+3,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+5,i3+3,c3)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,4,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+4,c3)+
     & c(i,1,0,0)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+4,c3)+
     & c(i,3,0,0)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+4,c3)+
     & c(i,5,0,0)*ui(i1+5,i2,i3+4,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+1,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+4,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+1,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+4,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+1,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+4,c3))+c(i,2,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+2,i3+4,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+4,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+3,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+4,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+3,
     & i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+4,c3))+c(i,4,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+4,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+4,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+4,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+4,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+4,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+4,c3)
     & )+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+4,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2+5,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+4,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+5,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+4,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+5,i3+4,c3)))+c(i,5,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(
     & i1,i2,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+5,c3)+c(i,2,0,0)*ui(i1+
     & 2,i2,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+5,c3)+c(i,4,0,0)*ui(i1+
     & 4,i2,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+5,c3))+c(i,1,1,0)*(c(i,
     & 0,0,0)*ui(i1,i2+1,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+5,c3)+c(
     & i,2,0,0)*ui(i1+2,i2+1,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+5,c3)
     & +c(i,4,0,0)*ui(i1+4,i2+1,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+5,
     & c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+5,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+2,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+5,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+2,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+5,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+2,i3+5,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+
     & 5,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+3,
     & i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+5,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+3,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+5,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+4,
     & i3+5,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+5,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+5,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+5,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+5,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+5,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+5,i3+5,c3)))
             else if( varWidth(i).eq.9 )then
! interpTensorProduct999(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,2,0)*(c(i,0,1,0)*
     & (c(i,0,0,0)*ui(i1,i2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3)+c(
     & i,2,0,0)*ui(i1+2,i2,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+0,c3)+c(
     & i,4,0,0)*ui(i1+4,i2,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+0,c3)+c(
     & i,6,0,0)*ui(i1+6,i2,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2,i3+0,c3)+c(
     & i,8,0,0)*ui(i1+8,i2,i3+0,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 1,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+0,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+1,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+0,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+1,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+0,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+1,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+1,i3+0,c3)+c(i,8,0,
     & 0)*ui(i1+8,i2+1,i3+0,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+
     & 0,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+2,
     & i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+0,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+2,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+0,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+2,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+2,i3+0,c3)+c(i,8,0,0)*
     & ui(i1+8,i2+2,i3+0,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+3,
     & i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+0,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+3,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+0,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+3,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+3,i3+0,c3)+c(i,8,0,0)*
     & ui(i1+8,i2+3,i3+0,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+4,
     & i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+0,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+4,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+0,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+4,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+4,i3+0,c3)+c(i,8,0,0)*
     & ui(i1+8,i2+4,i3+0,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+5,
     & i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+0,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+5,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+0,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+5,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+5,i3+0,c3)+c(i,8,0,0)*
     & ui(i1+8,i2+5,i3+0,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+6,
     & i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+0,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+6,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+0,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+6,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+6,i3+0,c3)+c(i,8,0,0)*
     & ui(i1+8,i2+6,i3+0,c3))+c(i,7,1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+7,
     & i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+0,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+7,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+7,i3+0,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+7,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+7,i3+0,c3)+c(i,8,0,0)*
     & ui(i1+8,i2+7,i3+0,c3))+c(i,8,1,0)*(c(i,0,0,0)*ui(i1,i2+8,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+8,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+8,
     & i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+8,i3+0,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+8,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+8,i3+0,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+8,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+8,i3+0,c3)+c(i,8,0,0)*
     & ui(i1+8,i2+8,i3+0,c3)))+c(i,1,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(
     & i1,i2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+1,c3)+c(i,2,0,0)*ui(i1+
     & 2,i2,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+1,c3)+c(i,4,0,0)*ui(i1+
     & 4,i2,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+1,c3)+c(i,6,0,0)*ui(i1+
     & 6,i2,i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2,i3+1,c3)+c(i,8,0,0)*ui(i1+
     & 8,i2,i3+1,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+1,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+1,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+1,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+1,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+1,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+1,
     & i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+1,i3+1,c3)+c(i,8,0,0)*ui(i1+8,
     & i2+1,i3+1,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+1,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+1,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+2,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+1,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+2,
     & i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+2,i3+1,c3)+c(i,8,0,0)*ui(i1+8,
     & i2+2,i3+1,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+1,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+3,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+1,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+3,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+1,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+3,
     & i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+3,i3+1,c3)+c(i,8,0,0)*ui(i1+8,
     & i2+3,i3+1,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+1,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+4,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+1,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+4,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+1,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+4,
     & i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+4,i3+1,c3)+c(i,8,0,0)*ui(i1+8,
     & i2+4,i3+1,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+1,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+5,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+1,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+5,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+1,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+5,
     & i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+5,i3+1,c3)+c(i,8,0,0)*ui(i1+8,
     & i2+5,i3+1,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+1,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+6,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+1,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+6,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+1,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+6,
     & i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+6,i3+1,c3)+c(i,8,0,0)*ui(i1+8,
     & i2+6,i3+1,c3))+c(i,7,1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+1,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+7,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+1,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+7,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+1,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+7,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+7,
     & i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+7,i3+1,c3)+c(i,8,0,0)*ui(i1+8,
     & i2+7,i3+1,c3))+c(i,8,1,0)*(c(i,0,0,0)*ui(i1,i2+8,i3+1,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+8,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+8,i3+1,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+8,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+1,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+8,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+8,
     & i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+8,i3+1,c3)+c(i,8,0,0)*ui(i1+8,
     & i2+8,i3+1,c3)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,2,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+2,c3)+
     & c(i,1,0,0)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+2,c3)+
     & c(i,5,0,0)*ui(i1+5,i2,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+2,c3)+
     & c(i,7,0,0)*ui(i1+7,i2,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2,i3+2,c3))+
     & c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+2,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+1,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+2,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+1,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+2,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+1,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+2,c3)+c(i,7,0,
     & 0)*ui(i1+7,i2+1,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+2,c3))+c(i,
     & 2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+2,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+2,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+2,c3))+c(i,3,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+3,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+3,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+3,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+2,c3))+c(i,4,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+4,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+4,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+4,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+2,c3))+c(i,5,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+5,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+5,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+5,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+5,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+2,c3))+c(i,6,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+6,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+6,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+6,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+6,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+2,c3))+c(i,7,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+7,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+7,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+7,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+7,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+2,c3))+c(i,8,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+8,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+8,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+8,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+8,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+8,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+8,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+2,c3)))+c(i,3,
     & 2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+3,c3)+c(i,1,0,0)*ui(
     & i1+1,i2,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+3,c3)+c(i,3,0,0)*ui(
     & i1+3,i2,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+3,c3)+c(i,5,0,0)*ui(
     & i1+5,i2,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+3,c3)+c(i,7,0,0)*ui(
     & i1+7,i2,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2,i3+3,c3))+c(i,1,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+1,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+1,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+1,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+3,c3))+c(i,2,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+2,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+2,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+2,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+2,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+3,c3))+c(i,3,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+3,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+3,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+3,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+3,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+4,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+4,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+3,c3))+c(i,5,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+5,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+5,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+5,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+5,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+3,c3))+c(i,6,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+6,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+6,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+6,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+6,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+3,c3))+c(i,7,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+7,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+7,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+7,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+3,c3))+c(i,8,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+8,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+8,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+8,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+8,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+8,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+8,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+3,c3)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,4,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+4,c3)+
     & c(i,1,0,0)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+4,c3)+
     & c(i,3,0,0)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+4,c3)+
     & c(i,5,0,0)*ui(i1+5,i2,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+4,c3)+
     & c(i,7,0,0)*ui(i1+7,i2,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2,i3+4,c3))+
     & c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+4,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+1,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+4,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+1,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+4,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+1,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+4,c3)+c(i,7,0,
     & 0)*ui(i1+7,i2+1,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+4,c3))+c(i,
     & 2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+2,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+4,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+2,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+4,c3))+c(i,3,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+3,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+3,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+4,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+3,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+4,c3))+c(i,4,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+4,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+4,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+4,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+4,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+4,c3))+c(i,5,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+5,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+5,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+5,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+4,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+5,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+4,c3))+c(i,6,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+6,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+6,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+6,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+4,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+6,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+4,c3))+c(i,7,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+7,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+7,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+7,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+4,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+7,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+4,c3))+c(i,8,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+8,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+8,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+8,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+8,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+8,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+4,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+8,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+4,c3)))+c(i,5,
     & 2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+5,c3)+c(i,1,0,0)*ui(
     & i1+1,i2,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+5,c3)+c(i,3,0,0)*ui(
     & i1+3,i2,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+5,c3)+c(i,5,0,0)*ui(
     & i1+5,i2,i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+5,c3)+c(i,7,0,0)*ui(
     & i1+7,i2,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2,i3+5,c3))+c(i,1,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+1,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+1,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+1,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+1,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+5,c3))+c(i,2,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+2,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+2,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+2,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+2,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+5,c3))+c(i,3,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+3,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+3,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+3,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+3,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+5,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+4,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+4,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+5,c3))+c(i,5,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+5,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+5,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+5,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+5,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+5,c3))+c(i,6,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+6,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+6,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+6,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+6,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+5,c3))+c(i,7,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+7,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+7,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+7,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+5,c3))+c(i,8,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+8,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+8,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+8,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+8,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+8,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+8,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+5,c3)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,6,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+6,c3)+
     & c(i,1,0,0)*ui(i1+1,i2,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+6,c3)+
     & c(i,3,0,0)*ui(i1+3,i2,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+6,c3)+
     & c(i,5,0,0)*ui(i1+5,i2,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+6,c3)+
     & c(i,7,0,0)*ui(i1+7,i2,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2,i3+6,c3))+
     & c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+6,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+1,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+6,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+1,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+6,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+1,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+6,c3)+c(i,7,0,
     & 0)*ui(i1+7,i2+1,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+6,c3))+c(i,
     & 2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+2,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+6,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+2,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+6,c3))+c(i,3,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+3,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+3,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+6,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+3,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+6,c3))+c(i,4,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+4,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+4,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+6,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+4,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+6,c3))+c(i,5,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+5,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+5,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+5,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+6,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+5,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+6,c3))+c(i,6,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+6,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+6,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+6,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+6,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+6,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+6,c3))+c(i,7,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+7,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+7,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+7,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+6,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+7,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+6,c3))+c(i,8,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+8,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+8,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+8,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+8,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+8,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+6,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+8,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+6,c3)))+c(i,7,
     & 2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+7,c3)+c(i,1,0,0)*ui(
     & i1+1,i2,i3+7,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+7,c3)+c(i,3,0,0)*ui(
     & i1+3,i2,i3+7,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+7,c3)+c(i,5,0,0)*ui(
     & i1+5,i2,i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+7,c3)+c(i,7,0,0)*ui(
     & i1+7,i2,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2,i3+7,c3))+c(i,1,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+1,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+1,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+1,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+1,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+7,c3))+c(i,2,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+2,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+2,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+2,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+2,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+7,c3))+c(i,3,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+3,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+3,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+3,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+3,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+7,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+4,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+4,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+7,c3))+c(i,5,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+5,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+5,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+5,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+5,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+7,c3))+c(i,6,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+6,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+6,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+6,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+6,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+7,c3))+c(i,7,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+7,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+7,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+7,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+7,c3))+c(i,8,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+8,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+8,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+8,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+8,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+8,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+8,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+7,c3)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,8,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+8,c3)+
     & c(i,1,0,0)*ui(i1+1,i2,i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+8,c3)+
     & c(i,3,0,0)*ui(i1+3,i2,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+8,c3)+
     & c(i,5,0,0)*ui(i1+5,i2,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+8,c3)+
     & c(i,7,0,0)*ui(i1+7,i2,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2,i3+8,c3))+
     & c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+8,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+1,i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+8,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+1,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+8,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+1,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+8,c3)+c(i,7,0,
     & 0)*ui(i1+7,i2+1,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+8,c3))+c(i,
     & 2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+8,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+8,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+8,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+2,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+8,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+2,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+8,c3))+c(i,3,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+8,c3)+c(i,1,0,0)*ui(i1+1,i2+3,
     & i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+8,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+8,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+3,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+8,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+3,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+8,c3))+c(i,4,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+8,c3)+c(i,1,0,0)*ui(i1+1,i2+4,
     & i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+8,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+8,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+4,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+8,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+4,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+8,c3))+c(i,5,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+8,c3)+c(i,1,0,0)*ui(i1+1,i2+5,
     & i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+8,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+5,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+8,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+5,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+8,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+5,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+8,c3))+c(i,6,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+8,c3)+c(i,1,0,0)*ui(i1+1,i2+6,
     & i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+8,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+6,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+8,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+6,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+8,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+6,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+8,c3))+c(i,7,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+8,c3)+c(i,1,0,0)*ui(i1+1,i2+7,
     & i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+8,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+7,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+8,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+7,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+8,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+7,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+8,c3))+c(i,8,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+8,i3+8,c3)+c(i,1,0,0)*ui(i1+1,i2+8,
     & i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+8,i3+8,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+8,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+8,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+8,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+8,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+8,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+8,c3)))
             else if( varWidth(i).eq.8 )then
! interpTensorProduct888(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,2,0)*(c(i,0,1,0)*
     & (c(i,0,0,0)*ui(i1,i2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3)+c(
     & i,2,0,0)*ui(i1+2,i2,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+0,c3)+c(
     & i,4,0,0)*ui(i1+4,i2,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+0,c3)+c(
     & i,6,0,0)*ui(i1+6,i2,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2,i3+0,c3))+c(
     & i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+
     & 1,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+0,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+1,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+0,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+1,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+0,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+1,i3+0,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+2,
     & i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+0,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+2,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+0,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+2,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+2,i3+0,c3))+c(i,3,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+3,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+0,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+3,
     & i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+0,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+3,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+0,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+3,i3+0,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+0,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+4,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+0,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+4,
     & i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+0,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+4,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+4,i3+0,c3))+c(i,5,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+5,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+0,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+5,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+0,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+5,
     & i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+0,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+5,i3+0,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+0,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+6,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+0,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+6,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+0,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+6,
     & i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+6,i3+0,c3))+c(i,7,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+7,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+0,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+7,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+0,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+7,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+7,i3+0,c3)
     & +c(i,6,0,0)*ui(i1+6,i2+7,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+7,i3+0,
     & c3)))+c(i,1,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+1,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+1,c3)+c(i,
     & 3,0,0)*ui(i1+3,i2,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+1,c3)+c(i,
     & 5,0,0)*ui(i1+5,i2,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+1,c3)+c(i,
     & 7,0,0)*ui(i1+7,i2,i3+1,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,
     & i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+1,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+1,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+1,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+1,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+1,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+1,i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+1,i3+1,c3))+c(i,2,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+1,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+1,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+2,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+1,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+2,i3+1,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+1,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+3,
     & i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+1,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+3,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+1,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+3,i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+3,i3+1,c3))+c(i,4,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+4,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+1,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+4,
     & i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+1,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+4,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+1,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+4,i3+1,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+1,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+5,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+1,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+5,
     & i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+1,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+5,i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+5,i3+1,c3))+c(i,6,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+6,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+1,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+6,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+1,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+6,
     & i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+1,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+6,i3+1,c3))+c(i,7,1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+1,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+7,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+1,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+7,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+1,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+7,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+7,
     & i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+7,i3+1,c3)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,2,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+2,c3)+
     & c(i,1,0,0)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+2,c3)+
     & c(i,5,0,0)*ui(i1+5,i2,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+2,c3)+
     & c(i,7,0,0)*ui(i1+7,i2,i3+2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+1,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+2,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+1,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+2,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+1,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+2,c3)+c(i,6,0,
     & 0)*ui(i1+6,i2+1,i3+2,c3)+c(i,7,0,0)*ui(i1+7,i2+1,i3+2,c3))+c(i,
     & 2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+2,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+2,i3+2,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+2,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+3,
     & i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+2,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+3,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+2,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+3,i3+2,c3)+c(i,7,0,0)*ui(i1+7,i2+3,i3+2,c3))+c(i,4,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+4,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+2,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+4,
     & i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+2,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+4,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+2,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+4,i3+2,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+2,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+5,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+2,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+5,
     & i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+2,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+5,i3+2,c3)+c(i,7,0,0)*ui(i1+7,i2+5,i3+2,c3))+c(i,6,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+6,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+2,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+6,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+2,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+6,
     & i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+2,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+6,i3+2,c3))+c(i,7,1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+2,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+7,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+7,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+2,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+7,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+7,
     & i3+2,c3)+c(i,7,0,0)*ui(i1+7,i2+7,i3+2,c3)))+c(i,3,2,0)*(c(i,0,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+3,
     & c3)+c(i,2,0,0)*ui(i1+2,i2,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+3,
     & c3)+c(i,6,0,0)*ui(i1+6,i2,i3+3,c3)+c(i,7,0,0)*ui(i1+7,i2,i3+3,
     & c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+3,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+1,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+1,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+3,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+1,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+3,c3)+c(i,
     & 7,0,0)*ui(i1+7,i2+1,i3+3,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 2,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+3,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+2,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+3,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+2,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+3,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+2,i3+3,c3)+c(i,7,0,0)*ui(i1+7,i2+2,i3+3,c3))+c(i,3,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+3,
     & i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+3,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+3,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+3,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+3,i3+3,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+3,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+4,
     & i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+3,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+4,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+3,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+4,i3+3,c3)+c(i,7,0,0)*ui(i1+7,i2+4,i3+3,c3))+c(i,5,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+5,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+3,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+5,
     & i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+3,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+5,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+3,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+5,i3+3,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+3,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+6,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+3,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+6,
     & i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+3,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+6,i3+3,c3)+c(i,7,0,0)*ui(i1+7,i2+6,i3+3,c3))+c(i,7,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+7,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+7,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+7,i3+3,c3)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,4,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+4,c3)+
     & c(i,1,0,0)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+4,c3)+
     & c(i,3,0,0)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+4,c3)+
     & c(i,5,0,0)*ui(i1+5,i2,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+4,c3)+
     & c(i,7,0,0)*ui(i1+7,i2,i3+4,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+1,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+4,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+1,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+4,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+1,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+4,c3)+c(i,6,0,
     & 0)*ui(i1+6,i2+1,i3+4,c3)+c(i,7,0,0)*ui(i1+7,i2+1,i3+4,c3))+c(i,
     & 2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+2,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+4,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+2,i3+4,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+4,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+3,
     & i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+4,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+3,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+4,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+3,i3+4,c3)+c(i,7,0,0)*ui(i1+7,i2+3,i3+4,c3))+c(i,4,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+4,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+4,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+4,
     & i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+4,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+4,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+4,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+4,i3+4,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+4,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+5,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+4,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+5,
     & i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+4,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+5,i3+4,c3)+c(i,7,0,0)*ui(i1+7,i2+5,i3+4,c3))+c(i,6,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+6,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+4,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+6,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+4,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+6,
     & i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+4,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+6,i3+4,c3))+c(i,7,1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+4,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+7,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+4,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+7,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+4,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+7,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+7,
     & i3+4,c3)+c(i,7,0,0)*ui(i1+7,i2+7,i3+4,c3)))+c(i,5,2,0)*(c(i,0,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+5,
     & c3)+c(i,2,0,0)*ui(i1+2,i2,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+5,
     & c3)+c(i,6,0,0)*ui(i1+6,i2,i3+5,c3)+c(i,7,0,0)*ui(i1+7,i2,i3+5,
     & c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+5,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+1,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+5,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+1,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+5,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+1,i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+5,c3)+c(i,
     & 7,0,0)*ui(i1+7,i2+1,i3+5,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 2,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+5,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+2,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+5,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+2,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+5,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+2,i3+5,c3)+c(i,7,0,0)*ui(i1+7,i2+2,i3+5,c3))+c(i,3,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+3,
     & i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+5,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+5,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+3,i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+5,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+3,i3+5,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+5,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+4,
     & i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+5,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+4,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+5,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+4,i3+5,c3)+c(i,7,0,0)*ui(i1+7,i2+4,i3+5,c3))+c(i,5,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+5,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+5,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+5,
     & i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+5,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+5,i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+5,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+5,i3+5,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+5,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+6,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+5,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+6,
     & i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+5,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+6,i3+5,c3)+c(i,7,0,0)*ui(i1+7,i2+6,i3+5,c3))+c(i,7,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+7,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+7,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+7,i3+5,c3)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,6,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+6,c3)+
     & c(i,1,0,0)*ui(i1+1,i2,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+6,c3)+
     & c(i,3,0,0)*ui(i1+3,i2,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+6,c3)+
     & c(i,5,0,0)*ui(i1+5,i2,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+6,c3)+
     & c(i,7,0,0)*ui(i1+7,i2,i3+6,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+1,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+6,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+1,i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+6,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+1,i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+6,c3)+c(i,6,0,
     & 0)*ui(i1+6,i2+1,i3+6,c3)+c(i,7,0,0)*ui(i1+7,i2+1,i3+6,c3))+c(i,
     & 2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+2,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+6,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+2,i3+6,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+6,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+3,
     & i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+6,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+3,i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+6,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+3,i3+6,c3)+c(i,7,0,0)*ui(i1+7,i2+3,i3+6,c3))+c(i,4,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+4,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+6,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+4,
     & i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+6,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+4,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+6,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+4,i3+6,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+6,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+5,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+6,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+5,
     & i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+6,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+5,i3+6,c3)+c(i,7,0,0)*ui(i1+7,i2+5,i3+6,c3))+c(i,6,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+6,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+6,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+6,i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+6,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2+6,
     & i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+6,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+6,i3+6,c3))+c(i,7,1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+6,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+7,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+6,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+7,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+6,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+7,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+7,
     & i3+6,c3)+c(i,7,0,0)*ui(i1+7,i2+7,i3+6,c3)))+c(i,7,2,0)*(c(i,0,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+7,
     & c3)+c(i,2,0,0)*ui(i1+2,i2,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+7,
     & c3)+c(i,6,0,0)*ui(i1+6,i2,i3+7,c3)+c(i,7,0,0)*ui(i1+7,i2,i3+7,
     & c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+7,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+1,i3+7,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+7,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+1,i3+7,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+7,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+1,i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+7,c3)+c(i,
     & 7,0,0)*ui(i1+7,i2+1,i3+7,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 2,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+7,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+2,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+7,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+2,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+7,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+2,i3+7,c3)+c(i,7,0,0)*ui(i1+7,i2+2,i3+7,c3))+c(i,3,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+3,
     & i3+7,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+7,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+7,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+7,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+3,i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+7,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+3,i3+7,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+7,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+7,c3)+c(i,2,0,0)*ui(i1+2,i2+4,
     & i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+7,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+4,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+7,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+4,i3+7,c3)+c(i,7,0,0)*ui(i1+7,i2+4,i3+7,c3))+c(i,5,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+5,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+7,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+5,
     & i3+7,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+7,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+5,i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+7,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+5,i3+7,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+7,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+6,i3+7,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+7,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+7,c3)+c(i,4,0,0)*ui(i1+4,i2+6,
     & i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+7,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+6,i3+7,c3)+c(i,7,0,0)*ui(i1+7,i2+6,i3+7,c3))+c(i,7,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+7,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+7,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+7,i3+7,c3)))
             else
               write(*,*) 'ERROR varWidth=',varWidth(i)
               stop 151
             end if
! endLoops3d()
             end do
             end do
         else if( width(1).eq.3 .and. width(2).eq.3 .and. width(3)
     & .eq.3 )then
! loops3d($interpTensorProduct333(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)
            if( c3a.eq.c3b )then
              do c3=c3a,c3b
              do i=nia,nib
! interpTensorProduct333(ug(ip(i,1),ip(i,2),ip(i,3),c3))
                i1=il(i,1)
                i2=il(i,2)
                i3=il(i,3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,2,0)*(c(i,0,1,0)
     & *(c(i,0,0,0)*ui(i1,i2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3)+
     & c(i,2,0,0)*ui(i1+2,i2,i3+0,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+1,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+0,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+1,i3+0,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+0,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+0,
     & c3)))+c(i,1,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+1,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+1,c3))+c(i,
     & 1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+1,
     & i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+1,c3))+c(i,2,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+2,i3+1,c3)))+c(i,2,2,0)*(c(i,0,1,0)*(c(i,0,0,0)
     & *ui(i1,i2,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,0)*
     & ui(i1+2,i2,i3+2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+2,c3)
     & +c(i,1,0,0)*ui(i1+1,i2+1,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+2,
     & c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+2,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+2,c3)))


              end do
              end do
            else
              ! put "c" loop as inner loop, this seems to be faster
              do i=nia,nib
              do c3=c3a,c3b
! interpTensorProduct333(ug(ip(i,1),ip(i,2),ip(i,3),c3))
                i1=il(i,1)
                i2=il(i,2)
                i3=il(i,3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,2,0)*(c(i,0,1,0)
     & *(c(i,0,0,0)*ui(i1,i2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3)+
     & c(i,2,0,0)*ui(i1+2,i2,i3+0,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+1,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+0,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+1,i3+0,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+0,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+0,
     & c3)))+c(i,1,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+1,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+1,c3))+c(i,
     & 1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+1,
     & i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+1,c3))+c(i,2,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+2,i3+1,c3)))+c(i,2,2,0)*(c(i,0,1,0)*(c(i,0,0,0)
     & *ui(i1,i2,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,0)*
     & ui(i1+2,i2,i3+2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+2,c3)
     & +c(i,1,0,0)*ui(i1+1,i2+1,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+2,
     & c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+2,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+2,c3)))


              end do
              end do
            end if
         else if( width(1).eq.1 .and. width(2).eq.1 .and. width(3)
     & .eq.1 )then
! loops3d($interp111(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)
           if( c3a.eq.c3b )then
             do c3=c3a,c3b
             do i=nia,nib
! interp111(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ui(i1,i2,i3,c3)


             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
! interp111(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ui(i1,i2,i3,c3)


             end do
             end do
           end if
          else if( width(1).eq.2 .and. width(2).eq.2 .and. width(3)
     & .eq.2 )then
! loops3d($interpTensorProduct222(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)
            if( c3a.eq.c3b )then
              do c3=c3a,c3b
              do i=nia,nib
! interpTensorProduct222(ug(ip(i,1),ip(i,2),ip(i,3),c3))
                i1=il(i,1)
                i2=il(i,2)
                i3=il(i,3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,2,0)*(c(i,0,1,0)
     & *(c(i,0,0,0)*ui(i1,i2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3))+
     & c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+0,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+1,i3+0,c3)))+c(i,1,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+
     & 1,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+1,c3))+c(i,1,1,0)*(c(i,0,0,0)*
     & ui(i1,i2+1,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+1,c3)))


              end do
              end do
            else
              ! put "c" loop as inner loop, this seems to be faster
              do i=nia,nib
              do c3=c3a,c3b
! interpTensorProduct222(ug(ip(i,1),ip(i,2),ip(i,3),c3))
                i1=il(i,1)
                i2=il(i,2)
                i3=il(i,3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,2,0)*(c(i,0,1,0)
     & *(c(i,0,0,0)*ui(i1,i2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3))+
     & c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+0,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+1,i3+0,c3)))+c(i,1,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+
     & 1,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+1,c3))+c(i,1,1,0)*(c(i,0,0,0)*
     & ui(i1,i2+1,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+1,c3)))


              end do
              end do
            end if
          else if( width(1).eq.4 .and. width(2).eq.4 .and. width(3)
     & .eq.4 )then
! loops3d($interpTensorProduct444(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)
            if( c3a.eq.c3b )then
              do c3=c3a,c3b
              do i=nia,nib
! interpTensorProduct444(ug(ip(i,1),ip(i,2),ip(i,3),c3))
                i1=il(i,1)
                i2=il(i,2)
                i3=il(i,3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,2,0)*(c(i,0,1,0)
     & *(c(i,0,0,0)*ui(i1,i2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3)+
     & c(i,2,0,0)*ui(i1+2,i2,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+0,c3))+
     & c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+0,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+1,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+0,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+1,i3+0,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+0,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+0,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+0,c3))+c(i,3,1,0)*(c(i,0,0,0)*
     & ui(i1,i2+3,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+0,c3)+c(i,2,0,0)
     & *ui(i1+2,i2+3,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+0,c3)))+c(i,
     & 1,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+1,c3)+c(i,1,0,0)*ui(
     & i1+1,i2,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+1,c3)+c(i,3,0,0)*ui(
     & i1+3,i2,i3+1,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+1,c3)+c(
     & i,1,0,0)*ui(i1+1,i2+1,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+1,c3)
     & +c(i,3,0,0)*ui(i1+3,i2+1,i3+1,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(
     & i1,i2+2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+2,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+1,c3))+c(i,3,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+3,
     & i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+1,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+1,c3)))+c(i,2,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+
     & 2,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+
     & 2,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+2,c3))+c(i,1,1,0)*(c(i,0,0,0)*
     & ui(i1,i2+1,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+2,c3)+c(i,2,0,0)
     & *ui(i1+2,i2+1,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+2,c3))+c(i,2,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+2,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+2,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+3,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+3,i3+2,c3)))+c(i,3,2,0)*(c(i,0,1,0)*(c(i,
     & 0,0,0)*ui(i1,i2,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+3,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+3,c3))+c(i,1,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+1,
     & i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+1,i3+3,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+3,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+2,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+3,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+2,i3+3,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+3,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+3,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+3,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+3,c3)))


              end do
              end do
            else
              ! put "c" loop as inner loop, this seems to be faster
              do i=nia,nib
              do c3=c3a,c3b
! interpTensorProduct444(ug(ip(i,1),ip(i,2),ip(i,3),c3))
                i1=il(i,1)
                i2=il(i,2)
                i3=il(i,3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,2,0)*(c(i,0,1,0)
     & *(c(i,0,0,0)*ui(i1,i2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3)+
     & c(i,2,0,0)*ui(i1+2,i2,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+0,c3))+
     & c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+0,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+1,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+0,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+1,i3+0,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+0,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+0,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+0,c3))+c(i,3,1,0)*(c(i,0,0,0)*
     & ui(i1,i2+3,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+0,c3)+c(i,2,0,0)
     & *ui(i1+2,i2+3,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+0,c3)))+c(i,
     & 1,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+1,c3)+c(i,1,0,0)*ui(
     & i1+1,i2,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+1,c3)+c(i,3,0,0)*ui(
     & i1+3,i2,i3+1,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+1,c3)+c(
     & i,1,0,0)*ui(i1+1,i2+1,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+1,c3)
     & +c(i,3,0,0)*ui(i1+3,i2+1,i3+1,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(
     & i1,i2+2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+2,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+1,c3))+c(i,3,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+3,
     & i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+1,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+1,c3)))+c(i,2,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+
     & 2,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+
     & 2,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+2,c3))+c(i,1,1,0)*(c(i,0,0,0)*
     & ui(i1,i2+1,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+2,c3)+c(i,2,0,0)
     & *ui(i1+2,i2+1,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+2,c3))+c(i,2,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+2,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+2,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+3,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+3,i3+2,c3)))+c(i,3,2,0)*(c(i,0,1,0)*(c(i,
     & 0,0,0)*ui(i1,i2,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+3,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+3,c3))+c(i,1,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+1,
     & i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+1,i3+3,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+3,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+2,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+3,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+2,i3+3,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+3,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+3,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+3,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+3,c3)))


              end do
              end do
            end if
          else if( width(1).eq.5 .and. width(2).eq.5 .and. width(3)
     & .eq.5 )then
            ! write(*,*) 'interpOpt explicit interp width=5'
! loops3d($interpTensorProduct555(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)
            if( c3a.eq.c3b )then
              do c3=c3a,c3b
              do i=nia,nib
! interpTensorProduct555(ug(ip(i,1),ip(i,2),ip(i,3),c3))
                i1=il(i,1)
                i2=il(i,2)
                i3=il(i,3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,2,0)*(c(i,0,1,0)
     & *(c(i,0,0,0)*ui(i1,i2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3)+
     & c(i,2,0,0)*ui(i1+2,i2,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+0,c3)+
     & c(i,4,0,0)*ui(i1+4,i2,i3+0,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+1,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+0,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+1,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+0,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+1,i3+0,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+2,
     & i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+0,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+2,i3+0,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+0,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+3,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+0,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+3,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+0,
     & c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+0,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+4,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+0,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+4,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+0,c3)))+c(i,1,
     & 2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+1,c3)+c(i,1,0,0)*ui(
     & i1+1,i2,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+1,c3)+c(i,3,0,0)*ui(
     & i1+3,i2,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+1,c3))+c(i,1,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+1,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+1,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+1,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+1,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+1,c3))+c(i,2,1,0)*(c(i,0,0,0)*
     & ui(i1,i2+2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,0,0)
     & *ui(i1+2,i2+2,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+1,c3)+c(i,4,
     & 0,0)*ui(i1+4,i2+2,i3+1,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,
     & i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+1,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+3,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+1,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+3,i3+1,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+1,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+4,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+1,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+4,
     & i3+1,c3)))+c(i,2,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+2,c3)
     & +c(i,1,0,0)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+2,c3))+
     & c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+2,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+1,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+2,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+1,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+2,c3))+c(i,2,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+2,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+2,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+2,
     & i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+2,c3))+c(i,3,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+3,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+2,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+3,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+2,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+3,i3+2,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+4,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+2,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+4,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+2,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+4,i3+2,c3)))+c(i,3,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(
     & i1,i2,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+3,c3)+c(i,2,0,0)*ui(i1+
     & 2,i2,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+3,c3)+c(i,4,0,0)*ui(i1+
     & 4,i2,i3+3,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+3,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+1,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+3,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+1,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+3,
     & c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+3,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+2,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+3,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+2,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+3,c3))+c(i,3,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+3,
     & i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+3,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+3,c3)))+c(i,4,2,0)*(c(i,0,1,0)*(
     & c(i,0,0,0)*ui(i1,i2,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+4,c3)+c(
     & i,2,0,0)*ui(i1+2,i2,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+4,c3)+c(
     & i,4,0,0)*ui(i1+4,i2,i3+4,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 1,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+4,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+1,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+4,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+1,i3+4,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+4,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+2,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+4,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+2,
     & i3+4,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+4,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+3,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+4,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+3,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+4,c3))+c(
     & i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+
     & 4,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+4,c3)))


              end do
              end do
            else
              ! put "c" loop as inner loop, this seems to be faster
              do i=nia,nib
              do c3=c3a,c3b
! interpTensorProduct555(ug(ip(i,1),ip(i,2),ip(i,3),c3))
                i1=il(i,1)
                i2=il(i,2)
                i3=il(i,3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,2,0)*(c(i,0,1,0)
     & *(c(i,0,0,0)*ui(i1,i2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3)+
     & c(i,2,0,0)*ui(i1+2,i2,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+0,c3)+
     & c(i,4,0,0)*ui(i1+4,i2,i3+0,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+1,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+0,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+1,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+0,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+1,i3+0,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+2,
     & i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+0,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+2,i3+0,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+0,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+3,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+0,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+3,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+0,
     & c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+0,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+4,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+0,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+4,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+0,c3)))+c(i,1,
     & 2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+1,c3)+c(i,1,0,0)*ui(
     & i1+1,i2,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+1,c3)+c(i,3,0,0)*ui(
     & i1+3,i2,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+1,c3))+c(i,1,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+1,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+1,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+1,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+1,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+1,c3))+c(i,2,1,0)*(c(i,0,0,0)*
     & ui(i1,i2+2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,0,0)
     & *ui(i1+2,i2+2,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+1,c3)+c(i,4,
     & 0,0)*ui(i1+4,i2+2,i3+1,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,
     & i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+1,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+3,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+1,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+3,i3+1,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+1,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+4,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+1,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+4,
     & i3+1,c3)))+c(i,2,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+2,c3)
     & +c(i,1,0,0)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+2,c3))+
     & c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+2,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+1,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+2,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+1,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+2,c3))+c(i,2,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+2,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+2,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+2,
     & i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+2,c3))+c(i,3,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+3,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+2,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+3,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+2,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+3,i3+2,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+4,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+2,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+4,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+2,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+4,i3+2,c3)))+c(i,3,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(
     & i1,i2,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+3,c3)+c(i,2,0,0)*ui(i1+
     & 2,i2,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+3,c3)+c(i,4,0,0)*ui(i1+
     & 4,i2,i3+3,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+3,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+1,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+3,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+1,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+3,
     & c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+3,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+2,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+3,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+2,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+3,c3))+c(i,3,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+3,
     & i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+3,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+3,c3)))+c(i,4,2,0)*(c(i,0,1,0)*(
     & c(i,0,0,0)*ui(i1,i2,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+4,c3)+c(
     & i,2,0,0)*ui(i1+2,i2,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+4,c3)+c(
     & i,4,0,0)*ui(i1+4,i2,i3+4,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 1,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+4,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+1,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+4,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+1,i3+4,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+4,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+2,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+4,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+2,
     & i3+4,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+4,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+3,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+4,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+3,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+4,c3))+c(
     & i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+
     & 4,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+4,c3)))


              end do
              end do
            end if
          else if( width(1).eq.6 .and. width(2).eq.6 .and. width(3)
     & .eq.6 )then
! loops3d($interpTensorProduct666(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)
            if( c3a.eq.c3b )then
              do c3=c3a,c3b
              do i=nia,nib
! interpTensorProduct666(ug(ip(i,1),ip(i,2),ip(i,3),c3))
                i1=il(i,1)
                i2=il(i,2)
                i3=il(i,3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,2,0)*(c(i,0,1,0)
     & *(c(i,0,0,0)*ui(i1,i2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3)+
     & c(i,2,0,0)*ui(i1+2,i2,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+0,c3)+
     & c(i,4,0,0)*ui(i1+4,i2,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+0,c3))+
     & c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+0,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+1,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+0,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+1,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+0,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+1,i3+0,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+2,
     & i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+0,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+2,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+0,c3))+c(i,3,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+3,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+0,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+3,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+0,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+3,
     & i3+0,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+0,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+4,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+0,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+4,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+0,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+4,i3+0,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+5,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+0,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+5,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+0,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+5,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+0,c3)))+c(i,1,
     & 2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+1,c3)+c(i,1,0,0)*ui(
     & i1+1,i2,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+1,c3)+c(i,3,0,0)*ui(
     & i1+3,i2,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+1,c3)+c(i,5,0,0)*ui(
     & i1+5,i2,i3+1,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+1,c3)+c(
     & i,1,0,0)*ui(i1+1,i2+1,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+1,c3)
     & +c(i,3,0,0)*ui(i1+3,i2+1,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+1,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+1,c3))+c(i,2,1,0)*(c(i,0,0,0)*
     & ui(i1,i2+2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,0,0)
     & *ui(i1+2,i2+2,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+1,c3)+c(i,4,
     & 0,0)*ui(i1+4,i2+2,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+1,c3))+c(
     & i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+
     & 3,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+1,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+1,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+3,i3+1,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+1,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+4,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+1,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+4,
     & i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+1,c3))+c(i,5,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+5,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+1,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+5,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+1,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+5,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+1,c3)
     & ))+c(i,2,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+2,c3)+c(i,1,
     & 0,0)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+2,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+2,c3)+c(i,5,
     & 0,0)*ui(i1+5,i2,i3+2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+
     & 2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,
     & i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+2,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+1,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+2,c3))+c(i,2,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+2,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+2,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+2,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+2,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+2,
     & i3+2,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+2,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+3,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+2,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+3,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+2,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+3,i3+2,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+4,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+2,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+4,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+2,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+4,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+2,c3))+c(i,5,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+5,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+5,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+5,i3+2,c3)))+c(i,3,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,
     & i2,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+3,c3)+c(i,2,0,0)*ui(i1+2,
     & i2,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+3,c3)+c(i,4,0,0)*ui(i1+4,
     & i2,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+3,c3))+c(i,1,1,0)*(c(i,0,
     & 0,0)*ui(i1,i2+1,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+3,c3)+c(i,
     & 2,0,0)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+3,c3)+
     & c(i,4,0,0)*ui(i1+4,i2+1,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+3,
     & c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+3,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+2,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+3,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+2,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+3,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+2,i3+3,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+
     & 3,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+3,
     & i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+3,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+3,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+3,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+4,
     & i3+3,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+3,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+5,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+3,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+5,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+3,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+5,i3+3,c3)))
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,4,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+4,c3)+
     & c(i,1,0,0)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+4,c3)+
     & c(i,3,0,0)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+4,c3)+
     & c(i,5,0,0)*ui(i1+5,i2,i3+4,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+1,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+4,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+1,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+4,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+1,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+4,c3))+c(i,2,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+2,i3+4,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+4,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+3,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+4,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+3,
     & i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+4,c3))+c(i,4,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+4,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+4,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+4,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+4,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+4,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+4,c3)
     & )+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+4,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2+5,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+4,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+5,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+4,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+5,i3+4,c3)))+c(i,5,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(
     & i1,i2,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+5,c3)+c(i,2,0,0)*ui(i1+
     & 2,i2,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+5,c3)+c(i,4,0,0)*ui(i1+
     & 4,i2,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+5,c3))+c(i,1,1,0)*(c(i,
     & 0,0,0)*ui(i1,i2+1,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+5,c3)+c(
     & i,2,0,0)*ui(i1+2,i2+1,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+5,c3)
     & +c(i,4,0,0)*ui(i1+4,i2+1,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+5,
     & c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+5,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+2,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+5,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+2,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+5,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+2,i3+5,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+
     & 5,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+3,
     & i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+5,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+3,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+5,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+4,
     & i3+5,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+5,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+5,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+5,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+5,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+5,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+5,i3+5,c3)))


              end do
              end do
            else
              ! put "c" loop as inner loop, this seems to be faster
              do i=nia,nib
              do c3=c3a,c3b
! interpTensorProduct666(ug(ip(i,1),ip(i,2),ip(i,3),c3))
                i1=il(i,1)
                i2=il(i,2)
                i3=il(i,3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,2,0)*(c(i,0,1,0)
     & *(c(i,0,0,0)*ui(i1,i2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3)+
     & c(i,2,0,0)*ui(i1+2,i2,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+0,c3)+
     & c(i,4,0,0)*ui(i1+4,i2,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+0,c3))+
     & c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+0,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+1,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+0,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+1,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+0,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+1,i3+0,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+2,
     & i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+0,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+2,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+0,c3))+c(i,3,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+3,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+0,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+3,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+0,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+3,
     & i3+0,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+0,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+4,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+0,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+4,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+0,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+4,i3+0,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+5,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+0,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+5,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+0,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+5,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+0,c3)))+c(i,1,
     & 2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+1,c3)+c(i,1,0,0)*ui(
     & i1+1,i2,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+1,c3)+c(i,3,0,0)*ui(
     & i1+3,i2,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+1,c3)+c(i,5,0,0)*ui(
     & i1+5,i2,i3+1,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+1,c3)+c(
     & i,1,0,0)*ui(i1+1,i2+1,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+1,c3)
     & +c(i,3,0,0)*ui(i1+3,i2+1,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+1,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+1,c3))+c(i,2,1,0)*(c(i,0,0,0)*
     & ui(i1,i2+2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,0,0)
     & *ui(i1+2,i2+2,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+1,c3)+c(i,4,
     & 0,0)*ui(i1+4,i2+2,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+1,c3))+c(
     & i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+
     & 3,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+1,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+1,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+3,i3+1,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+1,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+4,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+1,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+4,
     & i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+1,c3))+c(i,5,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+5,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+1,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+5,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+1,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+5,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+1,c3)
     & ))+c(i,2,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+2,c3)+c(i,1,
     & 0,0)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+2,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+2,c3)+c(i,5,
     & 0,0)*ui(i1+5,i2,i3+2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+
     & 2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,
     & i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+2,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+1,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+2,c3))+c(i,2,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+2,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+2,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+2,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+2,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+2,
     & i3+2,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+2,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+3,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+2,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+3,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+2,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+3,i3+2,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+4,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+2,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+4,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+2,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+4,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+2,c3))+c(i,5,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+5,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+5,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+5,i3+2,c3)))+c(i,3,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,
     & i2,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+3,c3)+c(i,2,0,0)*ui(i1+2,
     & i2,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+3,c3)+c(i,4,0,0)*ui(i1+4,
     & i2,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+3,c3))+c(i,1,1,0)*(c(i,0,
     & 0,0)*ui(i1,i2+1,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+3,c3)+c(i,
     & 2,0,0)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+3,c3)+
     & c(i,4,0,0)*ui(i1+4,i2+1,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+3,
     & c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+3,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+2,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+3,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+2,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+3,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+2,i3+3,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+
     & 3,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+3,
     & i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+3,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+3,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+3,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+4,
     & i3+3,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+3,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+5,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+3,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+5,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+3,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+5,i3+3,c3)))
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,4,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+4,c3)+
     & c(i,1,0,0)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+4,c3)+
     & c(i,3,0,0)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+4,c3)+
     & c(i,5,0,0)*ui(i1+5,i2,i3+4,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+1,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+4,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+1,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+4,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+1,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+4,c3))+c(i,2,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+2,i3+4,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+4,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+3,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+4,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+3,
     & i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+4,c3))+c(i,4,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+4,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+4,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+4,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+4,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+4,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+4,c3)
     & )+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+4,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2+5,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+4,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+5,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+4,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+5,i3+4,c3)))+c(i,5,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(
     & i1,i2,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+5,c3)+c(i,2,0,0)*ui(i1+
     & 2,i2,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+5,c3)+c(i,4,0,0)*ui(i1+
     & 4,i2,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+5,c3))+c(i,1,1,0)*(c(i,
     & 0,0,0)*ui(i1,i2+1,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+5,c3)+c(
     & i,2,0,0)*ui(i1+2,i2+1,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+5,c3)
     & +c(i,4,0,0)*ui(i1+4,i2+1,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+5,
     & c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+5,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+2,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+5,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+2,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+5,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+2,i3+5,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+
     & 5,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+3,
     & i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+5,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+3,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+5,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+4,
     & i3+5,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+5,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+5,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+5,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+5,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+5,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+5,i3+5,c3)))


              end do
              end do
            end if
          else if( width(1).eq.7 .and. width(2).eq.7 .and. width(3)
     & .eq.7 )then
! loops3d($interpTensorProduct777(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)
            if( c3a.eq.c3b )then
              do c3=c3a,c3b
              do i=nia,nib
! interpTensorProduct777(ug(ip(i,1),ip(i,2),ip(i,3),c3))
                i1=il(i,1)
                i2=il(i,2)
                i3=il(i,3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,2,0)*(c(i,0,1,0)
     & *(c(i,0,0,0)*ui(i1,i2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3)+
     & c(i,2,0,0)*ui(i1+2,i2,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+0,c3)+
     & c(i,4,0,0)*ui(i1+4,i2,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+0,c3)+
     & c(i,6,0,0)*ui(i1+6,i2,i3+0,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+1,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+0,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+1,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+0,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+1,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+0,c3)+c(i,6,0,
     & 0)*ui(i1+6,i2+1,i3+0,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+
     & 0,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+2,
     & i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+0,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+2,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+0,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+2,i3+0,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+0,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+3,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+0,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+3,
     & i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+0,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+3,i3+0,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+0,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+4,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+0,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+4,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+0,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+4,
     & i3+0,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+0,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+5,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+0,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+5,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+0,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+5,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+0,c3)
     & )+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+0,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2+6,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+0,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+6,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+0,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+6,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+0,c3)))+c(i,1,
     & 2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+1,c3)+c(i,1,0,0)*ui(
     & i1+1,i2,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+1,c3)+c(i,3,0,0)*ui(
     & i1+3,i2,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+1,c3)+c(i,5,0,0)*ui(
     & i1+5,i2,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+1,c3))+c(i,1,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+1,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+1,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+1,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+1,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+1,
     & i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+1,c3))+c(i,2,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+2,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+1,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+2,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+1,c3)
     & +c(i,6,0,0)*ui(i1+6,i2+2,i3+1,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(
     & i1,i2+3,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+1,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+3,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+1,c3)+c(i,4,0,
     & 0)*ui(i1+4,i2+3,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+1,c3)+c(i,
     & 6,0,0)*ui(i1+6,i2+3,i3+1,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 4,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+1,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+4,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+1,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+4,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+1,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+4,i3+1,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+1,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+5,
     & i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+1,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+5,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+1,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+5,i3+1,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+1,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+6,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+1,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+6,
     & i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+1,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+6,i3+1,c3)))+c(i,2,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+
     & 2,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+
     & 2,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+
     & 2,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+
     & 2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+2,c3)+c(i,1,0,0)*
     & ui(i1+1,i2+1,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+2,c3)+c(i,3,0,
     & 0)*ui(i1+3,i2+1,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+2,c3)+c(i,
     & 5,0,0)*ui(i1+5,i2+1,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+2,c3))+
     & c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+2,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+2,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+2,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+2,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+2,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+2,c3))+c(i,3,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+3,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+3,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+2,c3))+c(i,4,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+4,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+2,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+4,
     & i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+2,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+4,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+2,c3))+c(i,5,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+5,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+2,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+5,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+2,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+5,
     & i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+2,c3))+c(i,6,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+6,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+2,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+6,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+2,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+6,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+2,c3)
     & +c(i,6,0,0)*ui(i1+6,i2+6,i3+2,c3)))
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,3,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+3,c3)+
     & c(i,1,0,0)*ui(i1+1,i2,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+3,c3)+
     & c(i,3,0,0)*ui(i1+3,i2,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+3,c3)+
     & c(i,5,0,0)*ui(i1+5,i2,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+3,c3))+
     & c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+3,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+1,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+1,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+3,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+1,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+3,c3))+c(i,2,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+3,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+3,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+2,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+3,c3))+c(i,3,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+3,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+3,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+3,
     & i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+3,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+3,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+3,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+4,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+3,c3))+c(i,5,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+5,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+3,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+5,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+3,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+5,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+3,c3)
     & +c(i,6,0,0)*ui(i1+6,i2+5,i3+3,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(
     & i1,i2+6,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+3,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+6,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+3,c3)+c(i,4,0,
     & 0)*ui(i1+4,i2+6,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+3,c3)+c(i,
     & 6,0,0)*ui(i1+6,i2+6,i3+3,c3)))+c(i,4,2,0)*(c(i,0,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,0)*
     & ui(i1+2,i2,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,0)*
     & ui(i1+4,i2,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+4,c3)+c(i,6,0,0)*
     & ui(i1+6,i2,i3+4,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+4,c3)
     & +c(i,1,0,0)*ui(i1+1,i2+1,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+4,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+1,
     & i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+4,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+1,i3+4,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+4,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+2,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+4,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+4,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+2,
     & i3+4,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+4,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+3,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+4,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+3,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+4,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+3,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+4,c3)
     & )+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+4,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2+4,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+4,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+4,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+4,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+4,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+4,c3))+c(i,5,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+5,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+5,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+5,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+4,c3))+c(i,6,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+6,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+4,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+6,
     & i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+4,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+6,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+4,c3)))+c(i,5,2,0)*(c(
     & i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2,
     & i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2,
     & i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+5,c3))+c(i,1,1,0)*(c(i,0,0,0)
     & *ui(i1,i2+1,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+5,c3)+c(i,2,0,
     & 0)*ui(i1+2,i2+1,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+5,c3)+c(i,
     & 4,0,0)*ui(i1+4,i2+1,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+5,c3)+
     & c(i,6,0,0)*ui(i1+6,i2+1,i3+5,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+2,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+5,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+2,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+5,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+2,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+5,c3)+c(i,6,0,
     & 0)*ui(i1+6,i2+2,i3+5,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+
     & 5,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+3,
     & i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+5,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+3,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+5,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+3,i3+5,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+5,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+4,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+5,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+4,
     & i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+5,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+4,i3+5,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+5,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+5,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+5,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+5,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+5,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+5,
     & i3+5,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+5,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+6,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+5,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+6,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+5,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+6,i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+5,c3)
     & ))
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,6,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+6,c3)+
     & c(i,1,0,0)*ui(i1+1,i2,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+6,c3)+
     & c(i,3,0,0)*ui(i1+3,i2,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+6,c3)+
     & c(i,5,0,0)*ui(i1+5,i2,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+6,c3))+
     & c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+6,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+1,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+6,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+1,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+6,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+1,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+6,c3))+c(i,2,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+2,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+6,c3))+c(i,3,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+3,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+6,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+3,
     & i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+6,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+3,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+6,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+6,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+6,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2+4,
     & i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+6,c3))+c(i,5,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+5,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+6,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+5,i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+6,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+5,i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+6,c3)
     & +c(i,6,0,0)*ui(i1+6,i2+5,i3+6,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(
     & i1,i2+6,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+6,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+6,i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+6,c3)+c(i,4,0,
     & 0)*ui(i1+4,i2+6,i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+6,c3)+c(i,
     & 6,0,0)*ui(i1+6,i2+6,i3+6,c3)))


              end do
              end do
            else
              ! put "c" loop as inner loop, this seems to be faster
              do i=nia,nib
              do c3=c3a,c3b
! interpTensorProduct777(ug(ip(i,1),ip(i,2),ip(i,3),c3))
                i1=il(i,1)
                i2=il(i,2)
                i3=il(i,3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,2,0)*(c(i,0,1,0)
     & *(c(i,0,0,0)*ui(i1,i2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3)+
     & c(i,2,0,0)*ui(i1+2,i2,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+0,c3)+
     & c(i,4,0,0)*ui(i1+4,i2,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+0,c3)+
     & c(i,6,0,0)*ui(i1+6,i2,i3+0,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+1,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+0,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+1,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+0,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+1,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+0,c3)+c(i,6,0,
     & 0)*ui(i1+6,i2+1,i3+0,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+
     & 0,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+2,
     & i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+0,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+2,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+0,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+2,i3+0,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+0,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+3,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+0,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+3,
     & i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+0,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+3,i3+0,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+0,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+4,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+0,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+4,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+0,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+4,
     & i3+0,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+0,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+5,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+0,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+5,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+0,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+5,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+0,c3)
     & )+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+0,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2+6,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+0,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+6,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+0,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+6,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+0,c3)))+c(i,1,
     & 2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+1,c3)+c(i,1,0,0)*ui(
     & i1+1,i2,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+1,c3)+c(i,3,0,0)*ui(
     & i1+3,i2,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+1,c3)+c(i,5,0,0)*ui(
     & i1+5,i2,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+1,c3))+c(i,1,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+1,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+1,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+1,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+1,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+1,
     & i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+1,c3))+c(i,2,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+2,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+1,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+2,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+1,c3)
     & +c(i,6,0,0)*ui(i1+6,i2+2,i3+1,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(
     & i1,i2+3,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+1,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+3,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+1,c3)+c(i,4,0,
     & 0)*ui(i1+4,i2+3,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+1,c3)+c(i,
     & 6,0,0)*ui(i1+6,i2+3,i3+1,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 4,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+1,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+4,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+1,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+4,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+1,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+4,i3+1,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+1,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+5,
     & i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+1,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+5,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+1,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+5,i3+1,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+1,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+6,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+1,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+6,
     & i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+1,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+6,i3+1,c3)))+c(i,2,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+
     & 2,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+
     & 2,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+
     & 2,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+
     & 2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+2,c3)+c(i,1,0,0)*
     & ui(i1+1,i2+1,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+2,c3)+c(i,3,0,
     & 0)*ui(i1+3,i2+1,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+2,c3)+c(i,
     & 5,0,0)*ui(i1+5,i2+1,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+2,c3))+
     & c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+2,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+2,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+2,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+2,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+2,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+2,c3))+c(i,3,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+3,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+3,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+2,c3))+c(i,4,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+4,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+2,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+4,
     & i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+2,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+4,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+2,c3))+c(i,5,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+5,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+2,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+5,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+2,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+5,
     & i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+2,c3))+c(i,6,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+6,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+2,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+6,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+2,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+6,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+2,c3)
     & +c(i,6,0,0)*ui(i1+6,i2+6,i3+2,c3)))
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,3,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+3,c3)+
     & c(i,1,0,0)*ui(i1+1,i2,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+3,c3)+
     & c(i,3,0,0)*ui(i1+3,i2,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+3,c3)+
     & c(i,5,0,0)*ui(i1+5,i2,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+3,c3))+
     & c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+3,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+1,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+1,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+3,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+1,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+3,c3))+c(i,2,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+3,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+3,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+2,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+3,c3))+c(i,3,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+3,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+3,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+3,
     & i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+3,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+3,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+3,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+4,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+3,c3))+c(i,5,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+5,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+3,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+5,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+3,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+5,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+3,c3)
     & +c(i,6,0,0)*ui(i1+6,i2+5,i3+3,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(
     & i1,i2+6,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+3,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+6,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+3,c3)+c(i,4,0,
     & 0)*ui(i1+4,i2+6,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+3,c3)+c(i,
     & 6,0,0)*ui(i1+6,i2+6,i3+3,c3)))+c(i,4,2,0)*(c(i,0,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,0)*
     & ui(i1+2,i2,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,0)*
     & ui(i1+4,i2,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+4,c3)+c(i,6,0,0)*
     & ui(i1+6,i2,i3+4,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+4,c3)
     & +c(i,1,0,0)*ui(i1+1,i2+1,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+4,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+1,
     & i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+4,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+1,i3+4,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+4,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+2,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+4,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+4,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+2,
     & i3+4,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+4,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+3,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+4,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+3,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+4,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+3,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+4,c3)
     & )+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+4,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2+4,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+4,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+4,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+4,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+4,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+4,c3))+c(i,5,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+5,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+5,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+5,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+4,c3))+c(i,6,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+6,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+4,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+6,
     & i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+4,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+6,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+4,c3)))+c(i,5,2,0)*(c(
     & i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2,
     & i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2,
     & i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+5,c3))+c(i,1,1,0)*(c(i,0,0,0)
     & *ui(i1,i2+1,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+5,c3)+c(i,2,0,
     & 0)*ui(i1+2,i2+1,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+5,c3)+c(i,
     & 4,0,0)*ui(i1+4,i2+1,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+5,c3)+
     & c(i,6,0,0)*ui(i1+6,i2+1,i3+5,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+2,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+5,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+2,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+5,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+2,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+5,c3)+c(i,6,0,
     & 0)*ui(i1+6,i2+2,i3+5,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+
     & 5,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+3,
     & i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+5,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+3,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+5,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+3,i3+5,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+5,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+4,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+5,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+4,
     & i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+5,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+4,i3+5,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+5,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+5,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+5,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+5,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+5,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+5,
     & i3+5,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+5,c3)+c(i,1,0,0)
     & *ui(i1+1,i2+6,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+5,c3)+c(i,3,
     & 0,0)*ui(i1+3,i2+6,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+5,c3)+c(
     & i,5,0,0)*ui(i1+5,i2+6,i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+5,c3)
     & ))
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,6,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+6,c3)+
     & c(i,1,0,0)*ui(i1+1,i2,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+6,c3)+
     & c(i,3,0,0)*ui(i1+3,i2,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+6,c3)+
     & c(i,5,0,0)*ui(i1+5,i2,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+6,c3))+
     & c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+6,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+1,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+6,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+1,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+6,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+1,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+6,c3))+c(i,2,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+2,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+6,c3))+c(i,3,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+3,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+6,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+3,
     & i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+6,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+3,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+6,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+6,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+6,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2+4,
     & i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+6,c3))+c(i,5,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+5,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+6,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+5,i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+6,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+5,i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+6,c3)
     & +c(i,6,0,0)*ui(i1+6,i2+5,i3+6,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(
     & i1,i2+6,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+6,c3)+c(i,2,0,0)*
     & ui(i1+2,i2+6,i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+6,c3)+c(i,4,0,
     & 0)*ui(i1+4,i2+6,i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+6,c3)+c(i,
     & 6,0,0)*ui(i1+6,i2+6,i3+6,c3)))


              end do
              end do
            end if
          else if( width(1).eq.8 .and. width(2).eq.8 .and. width(3)
     & .eq.8 )then
! loops3d($interpTensorProduct888(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)
            if( c3a.eq.c3b )then
              do c3=c3a,c3b
              do i=nia,nib
! interpTensorProduct888(ug(ip(i,1),ip(i,2),ip(i,3),c3))
                i1=il(i,1)
                i2=il(i,2)
                i3=il(i,3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,2,0)*(c(i,0,1,0)
     & *(c(i,0,0,0)*ui(i1,i2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3)+
     & c(i,2,0,0)*ui(i1+2,i2,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+0,c3)+
     & c(i,4,0,0)*ui(i1+4,i2,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+0,c3)+
     & c(i,6,0,0)*ui(i1+6,i2,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2,i3+0,c3))+
     & c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+0,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+1,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+0,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+1,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+0,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+1,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+0,c3)+c(i,7,0,
     & 0)*ui(i1+7,i2+1,i3+0,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+
     & 0,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+2,
     & i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+0,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+2,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+0,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+2,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+2,i3+0,c3))+c(i,3,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+3,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+0,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+3,
     & i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+0,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+3,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+0,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+3,i3+0,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+0,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+4,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+0,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+4,
     & i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+0,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+4,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+4,i3+0,c3))+c(i,5,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+5,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+0,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+5,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+0,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+5,
     & i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+0,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+5,i3+0,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+0,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+6,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+0,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+6,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+0,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+6,
     & i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+6,i3+0,c3))+c(i,7,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+7,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+0,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+7,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+0,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+7,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+7,i3+0,c3)
     & +c(i,6,0,0)*ui(i1+6,i2+7,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+7,i3+0,
     & c3)))+c(i,1,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+1,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+1,c3)+c(i,
     & 3,0,0)*ui(i1+3,i2,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+1,c3)+c(i,
     & 5,0,0)*ui(i1+5,i2,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+1,c3)+c(i,
     & 7,0,0)*ui(i1+7,i2,i3+1,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,
     & i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+1,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+1,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+1,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+1,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+1,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+1,i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+1,i3+1,c3))+c(i,2,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+1,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+1,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+2,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+1,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+2,i3+1,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+1,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+3,
     & i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+1,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+3,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+1,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+3,i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+3,i3+1,c3))+c(i,4,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+4,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+1,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+4,
     & i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+1,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+4,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+1,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+4,i3+1,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+1,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+5,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+1,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+5,
     & i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+1,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+5,i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+5,i3+1,c3))+c(i,6,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+6,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+1,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+6,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+1,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+6,
     & i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+1,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+6,i3+1,c3))+c(i,7,1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+1,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+7,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+1,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+7,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+1,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+7,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+7,
     & i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+7,i3+1,c3)))
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,2,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+2,c3)+
     & c(i,1,0,0)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+2,c3)+
     & c(i,5,0,0)*ui(i1+5,i2,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+2,c3)+
     & c(i,7,0,0)*ui(i1+7,i2,i3+2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+1,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+2,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+1,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+2,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+1,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+2,c3)+c(i,6,0,
     & 0)*ui(i1+6,i2+1,i3+2,c3)+c(i,7,0,0)*ui(i1+7,i2+1,i3+2,c3))+c(i,
     & 2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+2,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+2,i3+2,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+2,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+3,
     & i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+2,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+3,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+2,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+3,i3+2,c3)+c(i,7,0,0)*ui(i1+7,i2+3,i3+2,c3))+c(i,4,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+4,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+2,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+4,
     & i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+2,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+4,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+2,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+4,i3+2,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+2,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+5,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+2,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+5,
     & i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+2,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+5,i3+2,c3)+c(i,7,0,0)*ui(i1+7,i2+5,i3+2,c3))+c(i,6,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+6,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+2,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+6,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+2,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+6,
     & i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+2,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+6,i3+2,c3))+c(i,7,1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+2,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+7,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+7,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+2,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+7,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+7,
     & i3+2,c3)+c(i,7,0,0)*ui(i1+7,i2+7,i3+2,c3)))+c(i,3,2,0)*(c(i,0,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+3,
     & c3)+c(i,2,0,0)*ui(i1+2,i2,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+3,
     & c3)+c(i,6,0,0)*ui(i1+6,i2,i3+3,c3)+c(i,7,0,0)*ui(i1+7,i2,i3+3,
     & c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+3,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+1,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+1,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+3,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+1,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+3,c3)+c(i,
     & 7,0,0)*ui(i1+7,i2+1,i3+3,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 2,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+3,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+2,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+3,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+2,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+3,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+2,i3+3,c3)+c(i,7,0,0)*ui(i1+7,i2+2,i3+3,c3))+c(i,3,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+3,
     & i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+3,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+3,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+3,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+3,i3+3,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+3,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+4,
     & i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+3,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+4,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+3,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+4,i3+3,c3)+c(i,7,0,0)*ui(i1+7,i2+4,i3+3,c3))+c(i,5,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+5,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+3,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+5,
     & i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+3,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+5,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+3,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+5,i3+3,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+3,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+6,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+3,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+6,
     & i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+3,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+6,i3+3,c3)+c(i,7,0,0)*ui(i1+7,i2+6,i3+3,c3))+c(i,7,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+7,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+7,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+7,i3+3,c3)))
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,4,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+4,c3)+
     & c(i,1,0,0)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+4,c3)+
     & c(i,3,0,0)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+4,c3)+
     & c(i,5,0,0)*ui(i1+5,i2,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+4,c3)+
     & c(i,7,0,0)*ui(i1+7,i2,i3+4,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+1,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+4,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+1,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+4,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+1,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+4,c3)+c(i,6,0,
     & 0)*ui(i1+6,i2+1,i3+4,c3)+c(i,7,0,0)*ui(i1+7,i2+1,i3+4,c3))+c(i,
     & 2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+2,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+4,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+2,i3+4,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+4,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+3,
     & i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+4,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+3,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+4,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+3,i3+4,c3)+c(i,7,0,0)*ui(i1+7,i2+3,i3+4,c3))+c(i,4,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+4,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+4,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+4,
     & i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+4,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+4,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+4,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+4,i3+4,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+4,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+5,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+4,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+5,
     & i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+4,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+5,i3+4,c3)+c(i,7,0,0)*ui(i1+7,i2+5,i3+4,c3))+c(i,6,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+6,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+4,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+6,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+4,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+6,
     & i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+4,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+6,i3+4,c3))+c(i,7,1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+4,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+7,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+4,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+7,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+4,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+7,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+7,
     & i3+4,c3)+c(i,7,0,0)*ui(i1+7,i2+7,i3+4,c3)))+c(i,5,2,0)*(c(i,0,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+5,
     & c3)+c(i,2,0,0)*ui(i1+2,i2,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+5,
     & c3)+c(i,6,0,0)*ui(i1+6,i2,i3+5,c3)+c(i,7,0,0)*ui(i1+7,i2,i3+5,
     & c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+5,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+1,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+5,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+1,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+5,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+1,i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+5,c3)+c(i,
     & 7,0,0)*ui(i1+7,i2+1,i3+5,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 2,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+5,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+2,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+5,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+2,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+5,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+2,i3+5,c3)+c(i,7,0,0)*ui(i1+7,i2+2,i3+5,c3))+c(i,3,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+3,
     & i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+5,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+5,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+3,i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+5,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+3,i3+5,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+5,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+4,
     & i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+5,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+4,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+5,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+4,i3+5,c3)+c(i,7,0,0)*ui(i1+7,i2+4,i3+5,c3))+c(i,5,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+5,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+5,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+5,
     & i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+5,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+5,i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+5,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+5,i3+5,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+5,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+6,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+5,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+6,
     & i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+5,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+6,i3+5,c3)+c(i,7,0,0)*ui(i1+7,i2+6,i3+5,c3))+c(i,7,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+7,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+7,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+7,i3+5,c3)))
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,6,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+6,c3)+
     & c(i,1,0,0)*ui(i1+1,i2,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+6,c3)+
     & c(i,3,0,0)*ui(i1+3,i2,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+6,c3)+
     & c(i,5,0,0)*ui(i1+5,i2,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+6,c3)+
     & c(i,7,0,0)*ui(i1+7,i2,i3+6,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+1,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+6,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+1,i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+6,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+1,i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+6,c3)+c(i,6,0,
     & 0)*ui(i1+6,i2+1,i3+6,c3)+c(i,7,0,0)*ui(i1+7,i2+1,i3+6,c3))+c(i,
     & 2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+2,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+6,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+2,i3+6,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+6,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+3,
     & i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+6,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+3,i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+6,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+3,i3+6,c3)+c(i,7,0,0)*ui(i1+7,i2+3,i3+6,c3))+c(i,4,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+4,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+6,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+4,
     & i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+6,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+4,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+6,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+4,i3+6,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+6,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+5,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+6,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+5,
     & i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+6,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+5,i3+6,c3)+c(i,7,0,0)*ui(i1+7,i2+5,i3+6,c3))+c(i,6,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+6,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+6,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+6,i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+6,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2+6,
     & i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+6,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+6,i3+6,c3))+c(i,7,1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+6,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+7,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+6,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+7,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+6,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+7,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+7,
     & i3+6,c3)+c(i,7,0,0)*ui(i1+7,i2+7,i3+6,c3)))+c(i,7,2,0)*(c(i,0,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+7,
     & c3)+c(i,2,0,0)*ui(i1+2,i2,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+7,
     & c3)+c(i,6,0,0)*ui(i1+6,i2,i3+7,c3)+c(i,7,0,0)*ui(i1+7,i2,i3+7,
     & c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+7,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+1,i3+7,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+7,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+1,i3+7,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+7,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+1,i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+7,c3)+c(i,
     & 7,0,0)*ui(i1+7,i2+1,i3+7,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 2,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+7,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+2,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+7,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+2,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+7,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+2,i3+7,c3)+c(i,7,0,0)*ui(i1+7,i2+2,i3+7,c3))+c(i,3,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+3,
     & i3+7,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+7,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+7,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+7,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+3,i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+7,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+3,i3+7,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+7,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+7,c3)+c(i,2,0,0)*ui(i1+2,i2+4,
     & i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+7,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+4,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+7,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+4,i3+7,c3)+c(i,7,0,0)*ui(i1+7,i2+4,i3+7,c3))+c(i,5,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+5,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+7,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+5,
     & i3+7,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+7,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+5,i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+7,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+5,i3+7,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+7,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+6,i3+7,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+7,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+7,c3)+c(i,4,0,0)*ui(i1+4,i2+6,
     & i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+7,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+6,i3+7,c3)+c(i,7,0,0)*ui(i1+7,i2+6,i3+7,c3))+c(i,7,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+7,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+7,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+7,i3+7,c3)))


              end do
              end do
            else
              ! put "c" loop as inner loop, this seems to be faster
              do i=nia,nib
              do c3=c3a,c3b
! interpTensorProduct888(ug(ip(i,1),ip(i,2),ip(i,3),c3))
                i1=il(i,1)
                i2=il(i,2)
                i3=il(i,3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,2,0)*(c(i,0,1,0)
     & *(c(i,0,0,0)*ui(i1,i2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3)+
     & c(i,2,0,0)*ui(i1+2,i2,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+0,c3)+
     & c(i,4,0,0)*ui(i1+4,i2,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+0,c3)+
     & c(i,6,0,0)*ui(i1+6,i2,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2,i3+0,c3))+
     & c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+0,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+1,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+0,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+1,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+0,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+1,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+0,c3)+c(i,7,0,
     & 0)*ui(i1+7,i2+1,i3+0,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+
     & 0,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+2,
     & i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+0,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+2,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+0,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+2,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+2,i3+0,c3))+c(i,3,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+3,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+0,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+3,
     & i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+0,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+3,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+0,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+3,i3+0,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+0,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+4,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+0,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+4,
     & i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+0,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+4,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+4,i3+0,c3))+c(i,5,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+5,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+0,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+5,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+0,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+5,
     & i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+0,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+5,i3+0,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+0,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+6,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+0,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+6,i3+0,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+0,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+0,c3)+c(i,6,0,0)*ui(i1+6,i2+6,
     & i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+6,i3+0,c3))+c(i,7,1,0)*(c(i,0,0,
     & 0)*ui(i1,i2+7,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+0,c3)+c(i,2,
     & 0,0)*ui(i1+2,i2+7,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+0,c3)+c(
     & i,4,0,0)*ui(i1+4,i2+7,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+7,i3+0,c3)
     & +c(i,6,0,0)*ui(i1+6,i2+7,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+7,i3+0,
     & c3)))+c(i,1,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+1,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+1,c3)+c(i,
     & 3,0,0)*ui(i1+3,i2,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+1,c3)+c(i,
     & 5,0,0)*ui(i1+5,i2,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+1,c3)+c(i,
     & 7,0,0)*ui(i1+7,i2,i3+1,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,
     & i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+1,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+1,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+1,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+1,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+1,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+1,i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+1,i3+1,c3))+c(i,2,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+1,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+1,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+2,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+1,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+2,i3+1,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+1,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+3,
     & i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+1,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+3,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+1,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+3,i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+3,i3+1,c3))+c(i,4,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+4,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+1,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+4,
     & i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+1,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+4,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+1,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+4,i3+1,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+1,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+5,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+1,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+5,
     & i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+1,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+5,i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+5,i3+1,c3))+c(i,6,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+6,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+1,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+6,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+1,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2+6,
     & i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+1,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+6,i3+1,c3))+c(i,7,1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+1,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+7,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+1,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+7,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+1,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+7,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+7,
     & i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+7,i3+1,c3)))
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,2,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+2,c3)+
     & c(i,1,0,0)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+2,c3)+
     & c(i,5,0,0)*ui(i1+5,i2,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+2,c3)+
     & c(i,7,0,0)*ui(i1+7,i2,i3+2,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+1,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+2,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+1,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+2,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+1,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+2,c3)+c(i,6,0,
     & 0)*ui(i1+6,i2+1,i3+2,c3)+c(i,7,0,0)*ui(i1+7,i2+1,i3+2,c3))+c(i,
     & 2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+2,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+2,i3+2,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+2,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+3,
     & i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+2,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+3,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+2,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+3,i3+2,c3)+c(i,7,0,0)*ui(i1+7,i2+3,i3+2,c3))+c(i,4,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+4,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+2,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+4,
     & i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+2,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+4,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+2,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+4,i3+2,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+2,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+5,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+2,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+5,
     & i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+2,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+5,i3+2,c3)+c(i,7,0,0)*ui(i1+7,i2+5,i3+2,c3))+c(i,6,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+6,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+2,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+6,i3+2,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+2,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+2,c3)+c(i,5,0,0)*ui(i1+5,i2+6,
     & i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+2,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+6,i3+2,c3))+c(i,7,1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+2,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+7,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+7,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+2,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+7,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+7,
     & i3+2,c3)+c(i,7,0,0)*ui(i1+7,i2+7,i3+2,c3)))+c(i,3,2,0)*(c(i,0,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+3,
     & c3)+c(i,2,0,0)*ui(i1+2,i2,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+3,
     & c3)+c(i,6,0,0)*ui(i1+6,i2,i3+3,c3)+c(i,7,0,0)*ui(i1+7,i2,i3+3,
     & c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+3,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+1,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+1,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+3,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+1,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+3,c3)+c(i,
     & 7,0,0)*ui(i1+7,i2+1,i3+3,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 2,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+3,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+2,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+3,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+2,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+3,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+2,i3+3,c3)+c(i,7,0,0)*ui(i1+7,i2+2,i3+3,c3))+c(i,3,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+3,
     & i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+3,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+3,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+3,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+3,i3+3,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+3,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+4,
     & i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+3,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+4,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+3,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+4,i3+3,c3)+c(i,7,0,0)*ui(i1+7,i2+4,i3+3,c3))+c(i,5,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+5,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+3,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+5,
     & i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+3,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+5,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+3,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+5,i3+3,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+3,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+6,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+3,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2+6,
     & i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+3,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+6,i3+3,c3)+c(i,7,0,0)*ui(i1+7,i2+6,i3+3,c3))+c(i,7,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+7,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+7,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+7,i3+3,c3)))
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,4,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+4,c3)+
     & c(i,1,0,0)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+4,c3)+
     & c(i,3,0,0)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+4,c3)+
     & c(i,5,0,0)*ui(i1+5,i2,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+4,c3)+
     & c(i,7,0,0)*ui(i1+7,i2,i3+4,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+1,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+4,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+1,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+4,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+1,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+4,c3)+c(i,6,0,
     & 0)*ui(i1+6,i2+1,i3+4,c3)+c(i,7,0,0)*ui(i1+7,i2+1,i3+4,c3))+c(i,
     & 2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+2,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+4,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+2,i3+4,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+4,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+3,
     & i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+4,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+3,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+4,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+3,i3+4,c3)+c(i,7,0,0)*ui(i1+7,i2+3,i3+4,c3))+c(i,4,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+4,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+4,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+4,
     & i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+4,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+4,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+4,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+4,i3+4,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+4,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+5,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+4,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+5,
     & i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+4,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+5,i3+4,c3)+c(i,7,0,0)*ui(i1+7,i2+5,i3+4,c3))+c(i,6,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+6,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+4,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+6,i3+4,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+4,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+4,c3)+c(i,5,0,0)*ui(i1+5,i2+6,
     & i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+4,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+6,i3+4,c3))+c(i,7,1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+4,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+7,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+4,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+7,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+4,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+7,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+7,
     & i3+4,c3)+c(i,7,0,0)*ui(i1+7,i2+7,i3+4,c3)))+c(i,5,2,0)*(c(i,0,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+5,
     & c3)+c(i,2,0,0)*ui(i1+2,i2,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+5,
     & c3)+c(i,6,0,0)*ui(i1+6,i2,i3+5,c3)+c(i,7,0,0)*ui(i1+7,i2,i3+5,
     & c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+5,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+1,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+5,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+1,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+5,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+1,i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+5,c3)+c(i,
     & 7,0,0)*ui(i1+7,i2+1,i3+5,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 2,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+5,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+2,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+5,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+2,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+5,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+2,i3+5,c3)+c(i,7,0,0)*ui(i1+7,i2+2,i3+5,c3))+c(i,3,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+3,
     & i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+5,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+5,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+3,i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+5,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+3,i3+5,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+5,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+4,
     & i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+5,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+4,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+5,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+4,i3+5,c3)+c(i,7,0,0)*ui(i1+7,i2+4,i3+5,c3))+c(i,5,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+5,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+5,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+5,
     & i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+5,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+5,i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+5,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+5,i3+5,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+5,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+6,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+5,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2+6,
     & i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+5,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+6,i3+5,c3)+c(i,7,0,0)*ui(i1+7,i2+6,i3+5,c3))+c(i,7,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+7,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+7,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+7,i3+5,c3)))
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,6,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+6,c3)+
     & c(i,1,0,0)*ui(i1+1,i2,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+6,c3)+
     & c(i,3,0,0)*ui(i1+3,i2,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+6,c3)+
     & c(i,5,0,0)*ui(i1+5,i2,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+6,c3)+
     & c(i,7,0,0)*ui(i1+7,i2,i3+6,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+1,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+6,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+1,i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+6,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+1,i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+6,c3)+c(i,6,0,
     & 0)*ui(i1+6,i2+1,i3+6,c3)+c(i,7,0,0)*ui(i1+7,i2+1,i3+6,c3))+c(i,
     & 2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+2,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+6,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+2,i3+6,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+6,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+3,
     & i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+6,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+3,i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+6,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+3,i3+6,c3)+c(i,7,0,0)*ui(i1+7,i2+3,i3+6,c3))+c(i,4,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+4,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+6,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+4,
     & i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+6,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+4,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+6,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+4,i3+6,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+6,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+5,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+6,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+5,
     & i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+6,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+5,i3+6,c3)+c(i,7,0,0)*ui(i1+7,i2+5,i3+6,c3))+c(i,6,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+6,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+6,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+6,i3+6,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+6,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+6,c3)+c(i,5,0,0)*ui(i1+5,i2+6,
     & i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+6,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+6,i3+6,c3))+c(i,7,1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+6,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+7,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+6,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+7,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+6,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+7,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+7,
     & i3+6,c3)+c(i,7,0,0)*ui(i1+7,i2+7,i3+6,c3)))+c(i,7,2,0)*(c(i,0,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+7,
     & c3)+c(i,2,0,0)*ui(i1+2,i2,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+7,
     & c3)+c(i,6,0,0)*ui(i1+6,i2,i3+7,c3)+c(i,7,0,0)*ui(i1+7,i2,i3+7,
     & c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+7,c3)+c(i,1,0,0)*ui(
     & i1+1,i2+1,i3+7,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+7,c3)+c(i,3,0,0)*
     & ui(i1+3,i2+1,i3+7,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+7,c3)+c(i,5,0,
     & 0)*ui(i1+5,i2+1,i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+7,c3)+c(i,
     & 7,0,0)*ui(i1+7,i2+1,i3+7,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 2,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+7,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+2,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+7,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+2,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+7,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+2,i3+7,c3)+c(i,7,0,0)*ui(i1+7,i2+2,i3+7,c3))+c(i,3,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+3,
     & i3+7,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+7,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+7,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+7,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+3,i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+7,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+3,i3+7,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+7,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+7,c3)+c(i,2,0,0)*ui(i1+2,i2+4,
     & i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+7,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+4,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+7,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+4,i3+7,c3)+c(i,7,0,0)*ui(i1+7,i2+4,i3+7,c3))+c(i,5,1,0)
     & *(c(i,0,0,0)*ui(i1,i2+5,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+7,
     & c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+5,
     & i3+7,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+7,c3)+c(i,5,0,0)*ui(i1+5,
     & i2+5,i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+7,c3)+c(i,7,0,0)*ui(
     & i1+7,i2+5,i3+7,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+7,c3)+
     & c(i,1,0,0)*ui(i1+1,i2+6,i3+7,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+7,
     & c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+7,c3)+c(i,4,0,0)*ui(i1+4,i2+6,
     & i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+7,c3)+c(i,6,0,0)*ui(i1+6,
     & i2+6,i3+7,c3)+c(i,7,0,0)*ui(i1+7,i2+6,i3+7,c3))+c(i,7,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+7,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+7,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+7,i3+7,c3)))


              end do
              end do
            end if
          else if( width(1).eq.9 .and. width(2).eq.9 .and. width(3)
     & .eq.9 )then
! loops3d($interpTensorProduct999(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)
            if( c3a.eq.c3b )then
              do c3=c3a,c3b
              do i=nia,nib
! interpTensorProduct999(ug(ip(i,1),ip(i,2),ip(i,3),c3))
                i1=il(i,1)
                i2=il(i,2)
                i3=il(i,3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,2,0)*(c(i,0,1,0)
     & *(c(i,0,0,0)*ui(i1,i2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3)+
     & c(i,2,0,0)*ui(i1+2,i2,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+0,c3)+
     & c(i,4,0,0)*ui(i1+4,i2,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+0,c3)+
     & c(i,6,0,0)*ui(i1+6,i2,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2,i3+0,c3)+
     & c(i,8,0,0)*ui(i1+8,i2,i3+0,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+1,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+0,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+1,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+0,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+1,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+0,c3)+c(i,6,0,
     & 0)*ui(i1+6,i2+1,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+1,i3+0,c3)+c(i,
     & 8,0,0)*ui(i1+8,i2+1,i3+0,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+2,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+0,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+2,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+0,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+2,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+2,i3+0,c3)+c(i,8,0,
     & 0)*ui(i1+8,i2+2,i3+0,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+
     & 0,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+3,
     & i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+0,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+3,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+0,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+3,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+3,i3+0,c3)+c(i,8,0,0)*
     & ui(i1+8,i2+3,i3+0,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+4,
     & i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+0,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+4,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+0,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+4,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+4,i3+0,c3)+c(i,8,0,0)*
     & ui(i1+8,i2+4,i3+0,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+5,
     & i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+0,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+5,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+0,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+5,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+5,i3+0,c3)+c(i,8,0,0)*
     & ui(i1+8,i2+5,i3+0,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+6,
     & i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+0,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+6,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+0,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+6,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+6,i3+0,c3)+c(i,8,0,0)*
     & ui(i1+8,i2+6,i3+0,c3))+c(i,7,1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+7,
     & i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+0,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+7,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+7,i3+0,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+7,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+7,i3+0,c3)+c(i,8,0,0)*
     & ui(i1+8,i2+7,i3+0,c3))+c(i,8,1,0)*(c(i,0,0,0)*ui(i1,i2+8,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+8,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+8,
     & i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+8,i3+0,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+8,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+8,i3+0,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+8,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+8,i3+0,c3)+c(i,8,0,0)*
     & ui(i1+8,i2+8,i3+0,c3)))+c(i,1,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(
     & i1,i2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+1,c3)+c(i,2,0,0)*ui(i1+
     & 2,i2,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+1,c3)+c(i,4,0,0)*ui(i1+
     & 4,i2,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+1,c3)+c(i,6,0,0)*ui(i1+
     & 6,i2,i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2,i3+1,c3)+c(i,8,0,0)*ui(i1+
     & 8,i2,i3+1,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+1,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+1,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+1,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+1,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+1,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+1,
     & i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+1,i3+1,c3)+c(i,8,0,0)*ui(i1+8,
     & i2+1,i3+1,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+1,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+1,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+2,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+1,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+2,
     & i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+2,i3+1,c3)+c(i,8,0,0)*ui(i1+8,
     & i2+2,i3+1,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+1,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+3,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+1,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+3,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+1,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+3,
     & i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+3,i3+1,c3)+c(i,8,0,0)*ui(i1+8,
     & i2+3,i3+1,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+1,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+4,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+1,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+4,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+1,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+4,
     & i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+4,i3+1,c3)+c(i,8,0,0)*ui(i1+8,
     & i2+4,i3+1,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+1,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+5,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+1,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+5,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+1,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+5,
     & i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+5,i3+1,c3)+c(i,8,0,0)*ui(i1+8,
     & i2+5,i3+1,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+1,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+6,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+1,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+6,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+1,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+6,
     & i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+6,i3+1,c3)+c(i,8,0,0)*ui(i1+8,
     & i2+6,i3+1,c3))+c(i,7,1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+1,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+7,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+1,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+7,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+1,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+7,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+7,
     & i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+7,i3+1,c3)+c(i,8,0,0)*ui(i1+8,
     & i2+7,i3+1,c3))+c(i,8,1,0)*(c(i,0,0,0)*ui(i1,i2+8,i3+1,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+8,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+8,i3+1,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+8,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+1,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+8,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+8,
     & i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+8,i3+1,c3)+c(i,8,0,0)*ui(i1+8,
     & i2+8,i3+1,c3)))
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,2,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+2,c3)+
     & c(i,1,0,0)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+2,c3)+
     & c(i,5,0,0)*ui(i1+5,i2,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+2,c3)+
     & c(i,7,0,0)*ui(i1+7,i2,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2,i3+2,c3))+
     & c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+2,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+1,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+2,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+1,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+2,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+1,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+2,c3)+c(i,7,0,
     & 0)*ui(i1+7,i2+1,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+2,c3))+c(i,
     & 2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+2,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+2,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+2,c3))+c(i,3,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+3,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+3,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+3,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+2,c3))+c(i,4,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+4,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+4,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+4,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+2,c3))+c(i,5,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+5,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+5,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+5,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+5,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+2,c3))+c(i,6,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+6,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+6,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+6,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+6,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+2,c3))+c(i,7,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+7,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+7,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+7,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+7,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+2,c3))+c(i,8,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+8,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+8,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+8,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+8,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+8,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+8,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+2,c3)))+c(i,3,
     & 2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+3,c3)+c(i,1,0,0)*ui(
     & i1+1,i2,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+3,c3)+c(i,3,0,0)*ui(
     & i1+3,i2,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+3,c3)+c(i,5,0,0)*ui(
     & i1+5,i2,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+3,c3)+c(i,7,0,0)*ui(
     & i1+7,i2,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2,i3+3,c3))+c(i,1,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+1,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+1,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+1,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+3,c3))+c(i,2,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+2,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+2,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+2,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+2,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+3,c3))+c(i,3,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+3,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+3,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+3,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+3,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+4,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+4,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+3,c3))+c(i,5,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+5,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+5,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+5,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+5,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+3,c3))+c(i,6,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+6,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+6,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+6,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+6,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+3,c3))+c(i,7,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+7,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+7,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+7,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+3,c3))+c(i,8,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+8,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+8,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+8,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+8,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+8,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+8,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+3,c3)))
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,4,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+4,c3)+
     & c(i,1,0,0)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+4,c3)+
     & c(i,3,0,0)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+4,c3)+
     & c(i,5,0,0)*ui(i1+5,i2,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+4,c3)+
     & c(i,7,0,0)*ui(i1+7,i2,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2,i3+4,c3))+
     & c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+4,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+1,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+4,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+1,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+4,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+1,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+4,c3)+c(i,7,0,
     & 0)*ui(i1+7,i2+1,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+4,c3))+c(i,
     & 2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+2,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+4,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+2,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+4,c3))+c(i,3,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+3,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+3,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+4,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+3,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+4,c3))+c(i,4,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+4,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+4,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+4,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+4,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+4,c3))+c(i,5,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+5,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+5,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+5,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+4,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+5,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+4,c3))+c(i,6,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+6,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+6,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+6,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+4,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+6,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+4,c3))+c(i,7,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+7,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+7,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+7,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+4,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+7,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+4,c3))+c(i,8,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+8,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+8,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+8,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+8,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+8,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+4,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+8,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+4,c3)))+c(i,5,
     & 2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+5,c3)+c(i,1,0,0)*ui(
     & i1+1,i2,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+5,c3)+c(i,3,0,0)*ui(
     & i1+3,i2,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+5,c3)+c(i,5,0,0)*ui(
     & i1+5,i2,i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+5,c3)+c(i,7,0,0)*ui(
     & i1+7,i2,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2,i3+5,c3))+c(i,1,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+1,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+1,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+1,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+1,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+5,c3))+c(i,2,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+2,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+2,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+2,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+2,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+5,c3))+c(i,3,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+3,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+3,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+3,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+3,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+5,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+4,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+4,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+5,c3))+c(i,5,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+5,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+5,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+5,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+5,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+5,c3))+c(i,6,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+6,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+6,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+6,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+6,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+5,c3))+c(i,7,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+7,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+7,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+7,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+5,c3))+c(i,8,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+8,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+8,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+8,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+8,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+8,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+8,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+5,c3)))
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,6,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+6,c3)+
     & c(i,1,0,0)*ui(i1+1,i2,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+6,c3)+
     & c(i,3,0,0)*ui(i1+3,i2,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+6,c3)+
     & c(i,5,0,0)*ui(i1+5,i2,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+6,c3)+
     & c(i,7,0,0)*ui(i1+7,i2,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2,i3+6,c3))+
     & c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+6,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+1,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+6,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+1,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+6,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+1,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+6,c3)+c(i,7,0,
     & 0)*ui(i1+7,i2+1,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+6,c3))+c(i,
     & 2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+2,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+6,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+2,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+6,c3))+c(i,3,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+3,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+3,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+6,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+3,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+6,c3))+c(i,4,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+4,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+4,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+6,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+4,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+6,c3))+c(i,5,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+5,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+5,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+5,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+6,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+5,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+6,c3))+c(i,6,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+6,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+6,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+6,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+6,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+6,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+6,c3))+c(i,7,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+7,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+7,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+7,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+6,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+7,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+6,c3))+c(i,8,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+8,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+8,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+8,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+8,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+8,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+6,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+8,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+6,c3)))+c(i,7,
     & 2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+7,c3)+c(i,1,0,0)*ui(
     & i1+1,i2,i3+7,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+7,c3)+c(i,3,0,0)*ui(
     & i1+3,i2,i3+7,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+7,c3)+c(i,5,0,0)*ui(
     & i1+5,i2,i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+7,c3)+c(i,7,0,0)*ui(
     & i1+7,i2,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2,i3+7,c3))+c(i,1,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+1,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+1,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+1,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+1,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+7,c3))+c(i,2,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+2,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+2,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+2,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+2,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+7,c3))+c(i,3,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+3,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+3,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+3,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+3,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+7,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+4,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+4,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+7,c3))+c(i,5,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+5,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+5,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+5,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+5,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+7,c3))+c(i,6,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+6,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+6,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+6,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+6,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+7,c3))+c(i,7,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+7,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+7,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+7,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+7,c3))+c(i,8,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+8,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+8,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+8,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+8,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+8,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+8,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+7,c3)))
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,8,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+8,c3)+
     & c(i,1,0,0)*ui(i1+1,i2,i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+8,c3)+
     & c(i,3,0,0)*ui(i1+3,i2,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+8,c3)+
     & c(i,5,0,0)*ui(i1+5,i2,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+8,c3)+
     & c(i,7,0,0)*ui(i1+7,i2,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2,i3+8,c3))+
     & c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+8,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+1,i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+8,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+1,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+8,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+1,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+8,c3)+c(i,7,0,
     & 0)*ui(i1+7,i2+1,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+8,c3))+c(i,
     & 2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+8,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+8,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+8,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+2,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+8,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+2,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+8,c3))+c(i,3,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+8,c3)+c(i,1,0,0)*ui(i1+1,i2+3,
     & i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+8,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+8,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+3,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+8,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+3,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+8,c3))+c(i,4,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+8,c3)+c(i,1,0,0)*ui(i1+1,i2+4,
     & i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+8,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+8,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+4,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+8,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+4,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+8,c3))+c(i,5,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+8,c3)+c(i,1,0,0)*ui(i1+1,i2+5,
     & i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+8,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+5,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+8,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+5,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+8,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+5,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+8,c3))+c(i,6,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+8,c3)+c(i,1,0,0)*ui(i1+1,i2+6,
     & i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+8,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+6,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+8,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+6,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+8,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+6,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+8,c3))+c(i,7,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+8,c3)+c(i,1,0,0)*ui(i1+1,i2+7,
     & i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+8,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+7,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+8,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+7,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+8,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+7,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+8,c3))+c(i,8,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+8,i3+8,c3)+c(i,1,0,0)*ui(i1+1,i2+8,
     & i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+8,i3+8,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+8,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+8,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+8,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+8,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+8,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+8,c3)))


              end do
              end do
            else
              ! put "c" loop as inner loop, this seems to be faster
              do i=nia,nib
              do c3=c3a,c3b
! interpTensorProduct999(ug(ip(i,1),ip(i,2),ip(i,3),c3))
                i1=il(i,1)
                i2=il(i,2)
                i3=il(i,3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,2,0)*(c(i,0,1,0)
     & *(c(i,0,0,0)*ui(i1,i2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+0,c3)+
     & c(i,2,0,0)*ui(i1+2,i2,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+0,c3)+
     & c(i,4,0,0)*ui(i1+4,i2,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+0,c3)+
     & c(i,6,0,0)*ui(i1+6,i2,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2,i3+0,c3)+
     & c(i,8,0,0)*ui(i1+8,i2,i3+0,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,
     & i2+1,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+0,c3)+c(i,2,0,0)*ui(
     & i1+2,i2+1,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+0,c3)+c(i,4,0,0)*
     & ui(i1+4,i2+1,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+0,c3)+c(i,6,0,
     & 0)*ui(i1+6,i2+1,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+1,i3+0,c3)+c(i,
     & 8,0,0)*ui(i1+8,i2+1,i3+0,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+
     & 2,i3+0,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+0,c3)+c(i,2,0,0)*ui(i1+2,
     & i2+2,i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+0,c3)+c(i,4,0,0)*ui(
     & i1+4,i2+2,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+0,c3)+c(i,6,0,0)*
     & ui(i1+6,i2+2,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+2,i3+0,c3)+c(i,8,0,
     & 0)*ui(i1+8,i2+2,i3+0,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+
     & 0,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+3,
     & i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+0,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+3,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+0,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+3,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+3,i3+0,c3)+c(i,8,0,0)*
     & ui(i1+8,i2+3,i3+0,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+4,
     & i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+0,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+4,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+0,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+4,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+4,i3+0,c3)+c(i,8,0,0)*
     & ui(i1+8,i2+4,i3+0,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+5,
     & i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+0,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+5,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+0,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+5,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+5,i3+0,c3)+c(i,8,0,0)*
     & ui(i1+8,i2+5,i3+0,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+6,
     & i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+0,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+6,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+0,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+6,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+6,i3+0,c3)+c(i,8,0,0)*
     & ui(i1+8,i2+6,i3+0,c3))+c(i,7,1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+7,
     & i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+0,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+7,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+7,i3+0,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+7,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+7,i3+0,c3)+c(i,8,0,0)*
     & ui(i1+8,i2+7,i3+0,c3))+c(i,8,1,0)*(c(i,0,0,0)*ui(i1,i2+8,i3+0,
     & c3)+c(i,1,0,0)*ui(i1+1,i2+8,i3+0,c3)+c(i,2,0,0)*ui(i1+2,i2+8,
     & i3+0,c3)+c(i,3,0,0)*ui(i1+3,i2+8,i3+0,c3)+c(i,4,0,0)*ui(i1+4,
     & i2+8,i3+0,c3)+c(i,5,0,0)*ui(i1+5,i2+8,i3+0,c3)+c(i,6,0,0)*ui(
     & i1+6,i2+8,i3+0,c3)+c(i,7,0,0)*ui(i1+7,i2+8,i3+0,c3)+c(i,8,0,0)*
     & ui(i1+8,i2+8,i3+0,c3)))+c(i,1,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(
     & i1,i2,i3+1,c3)+c(i,1,0,0)*ui(i1+1,i2,i3+1,c3)+c(i,2,0,0)*ui(i1+
     & 2,i2,i3+1,c3)+c(i,3,0,0)*ui(i1+3,i2,i3+1,c3)+c(i,4,0,0)*ui(i1+
     & 4,i2,i3+1,c3)+c(i,5,0,0)*ui(i1+5,i2,i3+1,c3)+c(i,6,0,0)*ui(i1+
     & 6,i2,i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2,i3+1,c3)+c(i,8,0,0)*ui(i1+
     & 8,i2,i3+1,c3))+c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+1,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+1,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+1,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+1,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+1,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+1,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+1,
     & i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+1,i3+1,c3)+c(i,8,0,0)*ui(i1+8,
     & i2+1,i3+1,c3))+c(i,2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+1,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+1,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+2,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+1,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+2,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+2,
     & i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+2,i3+1,c3)+c(i,8,0,0)*ui(i1+8,
     & i2+2,i3+1,c3))+c(i,3,1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+1,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+3,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+1,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+3,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+1,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+3,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+3,
     & i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+3,i3+1,c3)+c(i,8,0,0)*ui(i1+8,
     & i2+3,i3+1,c3))+c(i,4,1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+1,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+4,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+1,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+4,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+1,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+4,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+4,
     & i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+4,i3+1,c3)+c(i,8,0,0)*ui(i1+8,
     & i2+4,i3+1,c3))+c(i,5,1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+1,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+5,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+1,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+5,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+1,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+5,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+5,
     & i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+5,i3+1,c3)+c(i,8,0,0)*ui(i1+8,
     & i2+5,i3+1,c3))+c(i,6,1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+1,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+6,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+1,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+6,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+1,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+6,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+6,
     & i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+6,i3+1,c3)+c(i,8,0,0)*ui(i1+8,
     & i2+6,i3+1,c3))+c(i,7,1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+1,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+7,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+1,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+7,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+1,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+7,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+7,
     & i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+7,i3+1,c3)+c(i,8,0,0)*ui(i1+8,
     & i2+7,i3+1,c3))+c(i,8,1,0)*(c(i,0,0,0)*ui(i1,i2+8,i3+1,c3)+c(i,
     & 1,0,0)*ui(i1+1,i2+8,i3+1,c3)+c(i,2,0,0)*ui(i1+2,i2+8,i3+1,c3)+
     & c(i,3,0,0)*ui(i1+3,i2+8,i3+1,c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+1,
     & c3)+c(i,5,0,0)*ui(i1+5,i2+8,i3+1,c3)+c(i,6,0,0)*ui(i1+6,i2+8,
     & i3+1,c3)+c(i,7,0,0)*ui(i1+7,i2+8,i3+1,c3)+c(i,8,0,0)*ui(i1+8,
     & i2+8,i3+1,c3)))
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,2,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+2,c3)+
     & c(i,1,0,0)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+2,c3)+
     & c(i,3,0,0)*ui(i1+3,i2,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+2,c3)+
     & c(i,5,0,0)*ui(i1+5,i2,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+2,c3)+
     & c(i,7,0,0)*ui(i1+7,i2,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2,i3+2,c3))+
     & c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+2,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+1,i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+2,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+1,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+2,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+1,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+2,c3)+c(i,7,0,
     & 0)*ui(i1+7,i2+1,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+2,c3))+c(i,
     & 2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+2,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+2,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+2,c3))+c(i,3,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+3,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+3,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+3,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+2,c3))+c(i,4,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+4,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+4,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+4,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+2,c3))+c(i,5,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+5,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+5,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+5,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+5,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+2,c3))+c(i,6,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+6,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+6,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+6,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+6,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+2,c3))+c(i,7,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+7,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+7,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+7,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+7,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+2,c3))+c(i,8,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+8,i3+2,c3)+c(i,1,0,0)*ui(i1+1,i2+8,
     & i3+2,c3)+c(i,2,0,0)*ui(i1+2,i2+8,i3+2,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+8,i3+2,c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+2,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+8,i3+2,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+2,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+8,i3+2,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+2,c3)))+c(i,3,
     & 2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+3,c3)+c(i,1,0,0)*ui(
     & i1+1,i2,i3+3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+3,c3)+c(i,3,0,0)*ui(
     & i1+3,i2,i3+3,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+3,c3)+c(i,5,0,0)*ui(
     & i1+5,i2,i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+3,c3)+c(i,7,0,0)*ui(
     & i1+7,i2,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2,i3+3,c3))+c(i,1,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+1,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+1,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+1,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+3,c3))+c(i,2,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+2,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+2,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+2,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+2,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+3,c3))+c(i,3,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+3,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+3,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+3,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+3,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+4,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+4,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+3,c3))+c(i,5,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+5,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+5,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+5,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+5,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+3,c3))+c(i,6,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+6,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+6,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+6,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+6,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+3,c3))+c(i,7,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+7,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+7,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+7,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+3,c3))+c(i,8,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+8,i3+3,c3)+c(i,1,0,0)*ui(i1+1,i2+8,i3+3,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+8,i3+3,c3)+c(i,3,0,0)*ui(i1+3,i2+8,i3+3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+3,c3)+c(i,5,0,0)*ui(i1+5,i2+8,
     & i3+3,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+3,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+8,i3+3,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+3,c3)))
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,4,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+4,c3)+
     & c(i,1,0,0)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+4,c3)+
     & c(i,3,0,0)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+4,c3)+
     & c(i,5,0,0)*ui(i1+5,i2,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+4,c3)+
     & c(i,7,0,0)*ui(i1+7,i2,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2,i3+4,c3))+
     & c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+4,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+1,i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+4,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+1,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+4,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+1,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+4,c3)+c(i,7,0,
     & 0)*ui(i1+7,i2+1,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+4,c3))+c(i,
     & 2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+2,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+4,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+2,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+4,c3))+c(i,3,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+3,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+3,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+4,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+3,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+4,c3))+c(i,4,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+4,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+4,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+4,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+4,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+4,c3))+c(i,5,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+5,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+5,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+5,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+4,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+5,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+4,c3))+c(i,6,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+6,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+6,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+6,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+4,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+6,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+4,c3))+c(i,7,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+7,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+7,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+7,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+4,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+7,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+4,c3))+c(i,8,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+8,i3+4,c3)+c(i,1,0,0)*ui(i1+1,i2+8,
     & i3+4,c3)+c(i,2,0,0)*ui(i1+2,i2+8,i3+4,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+8,i3+4,c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+4,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+8,i3+4,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+4,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+8,i3+4,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+4,c3)))+c(i,5,
     & 2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+5,c3)+c(i,1,0,0)*ui(
     & i1+1,i2,i3+5,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+5,c3)+c(i,3,0,0)*ui(
     & i1+3,i2,i3+5,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+5,c3)+c(i,5,0,0)*ui(
     & i1+5,i2,i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+5,c3)+c(i,7,0,0)*ui(
     & i1+7,i2,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2,i3+5,c3))+c(i,1,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+1,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+1,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+1,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+1,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+5,c3))+c(i,2,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+2,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+2,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+2,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+2,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+5,c3))+c(i,3,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+3,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+3,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+3,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+3,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+5,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+4,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+4,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+5,c3))+c(i,5,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+5,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+5,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+5,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+5,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+5,c3))+c(i,6,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+6,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+6,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+6,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+6,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+5,c3))+c(i,7,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+7,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+7,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+7,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+5,c3))+c(i,8,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+8,i3+5,c3)+c(i,1,0,0)*ui(i1+1,i2+8,i3+5,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+8,i3+5,c3)+c(i,3,0,0)*ui(i1+3,i2+8,i3+5,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+5,c3)+c(i,5,0,0)*ui(i1+5,i2+8,
     & i3+5,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+5,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+8,i3+5,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+5,c3)))
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,6,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+6,c3)+
     & c(i,1,0,0)*ui(i1+1,i2,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+6,c3)+
     & c(i,3,0,0)*ui(i1+3,i2,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+6,c3)+
     & c(i,5,0,0)*ui(i1+5,i2,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+6,c3)+
     & c(i,7,0,0)*ui(i1+7,i2,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2,i3+6,c3))+
     & c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+6,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+1,i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+6,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+1,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+6,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+1,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+6,c3)+c(i,7,0,
     & 0)*ui(i1+7,i2+1,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+6,c3))+c(i,
     & 2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+2,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+6,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+2,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+6,c3))+c(i,3,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+3,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+3,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+6,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+3,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+6,c3))+c(i,4,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+4,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+4,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+6,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+4,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+6,c3))+c(i,5,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+5,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+5,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+5,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+6,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+5,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+6,c3))+c(i,6,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+6,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+6,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+6,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+6,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+6,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+6,c3))+c(i,7,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+7,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+7,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+7,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+6,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+7,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+6,c3))+c(i,8,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+8,i3+6,c3)+c(i,1,0,0)*ui(i1+1,i2+8,
     & i3+6,c3)+c(i,2,0,0)*ui(i1+2,i2+8,i3+6,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+8,i3+6,c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+6,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+8,i3+6,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+6,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+8,i3+6,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+6,c3)))+c(i,7,
     & 2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+7,c3)+c(i,1,0,0)*ui(
     & i1+1,i2,i3+7,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+7,c3)+c(i,3,0,0)*ui(
     & i1+3,i2,i3+7,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+7,c3)+c(i,5,0,0)*ui(
     & i1+5,i2,i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+7,c3)+c(i,7,0,0)*ui(
     & i1+7,i2,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2,i3+7,c3))+c(i,1,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+1,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+1,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+1,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+1,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+1,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+1,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+7,c3))+c(i,2,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+2,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+2,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+2,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+2,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+2,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+2,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+7,c3))+c(i,3,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+3,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+3,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+3,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+3,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+3,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+3,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+7,c3))+c(i,4,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+4,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+4,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+4,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+4,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+4,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+4,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+7,c3))+c(i,5,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+5,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+5,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+5,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+5,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+5,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+5,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+7,c3))+c(i,6,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+6,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+6,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+6,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+6,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+6,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+6,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+7,c3))+c(i,7,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+7,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+7,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+7,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+7,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+7,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+7,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+7,c3))+c(i,8,1,0)*(c(
     & i,0,0,0)*ui(i1,i2+8,i3+7,c3)+c(i,1,0,0)*ui(i1+1,i2+8,i3+7,c3)+
     & c(i,2,0,0)*ui(i1+2,i2+8,i3+7,c3)+c(i,3,0,0)*ui(i1+3,i2+8,i3+7,
     & c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+7,c3)+c(i,5,0,0)*ui(i1+5,i2+8,
     & i3+7,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+7,c3)+c(i,7,0,0)*ui(i1+7,
     & i2+8,i3+7,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+7,c3)))
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,8,2,0)*(c(i,0,1,0)*(c(i,0,0,0)*ui(i1,i2,i3+8,c3)+
     & c(i,1,0,0)*ui(i1+1,i2,i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2,i3+8,c3)+
     & c(i,3,0,0)*ui(i1+3,i2,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2,i3+8,c3)+
     & c(i,5,0,0)*ui(i1+5,i2,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2,i3+8,c3)+
     & c(i,7,0,0)*ui(i1+7,i2,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2,i3+8,c3))+
     & c(i,1,1,0)*(c(i,0,0,0)*ui(i1,i2+1,i3+8,c3)+c(i,1,0,0)*ui(i1+1,
     & i2+1,i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+1,i3+8,c3)+c(i,3,0,0)*ui(
     & i1+3,i2+1,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+1,i3+8,c3)+c(i,5,0,0)*
     & ui(i1+5,i2+1,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+1,i3+8,c3)+c(i,7,0,
     & 0)*ui(i1+7,i2+1,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+1,i3+8,c3))+c(i,
     & 2,1,0)*(c(i,0,0,0)*ui(i1,i2+2,i3+8,c3)+c(i,1,0,0)*ui(i1+1,i2+2,
     & i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+2,i3+8,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+2,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+2,i3+8,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+2,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+2,i3+8,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+2,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+2,i3+8,c3))+c(i,3,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+3,i3+8,c3)+c(i,1,0,0)*ui(i1+1,i2+3,
     & i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+3,i3+8,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+3,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+3,i3+8,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+3,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+3,i3+8,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+3,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+3,i3+8,c3))+c(i,4,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+4,i3+8,c3)+c(i,1,0,0)*ui(i1+1,i2+4,
     & i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+4,i3+8,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+4,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+4,i3+8,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+4,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+4,i3+8,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+4,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+4,i3+8,c3))+c(i,5,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+5,i3+8,c3)+c(i,1,0,0)*ui(i1+1,i2+5,
     & i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+5,i3+8,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+5,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+5,i3+8,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+5,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+5,i3+8,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+5,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+5,i3+8,c3))+c(i,6,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+6,i3+8,c3)+c(i,1,0,0)*ui(i1+1,i2+6,
     & i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+6,i3+8,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+6,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+6,i3+8,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+6,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+6,i3+8,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+6,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+6,i3+8,c3))+c(i,7,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+7,i3+8,c3)+c(i,1,0,0)*ui(i1+1,i2+7,
     & i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+7,i3+8,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+7,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+7,i3+8,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+7,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+7,i3+8,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+7,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+7,i3+8,c3))+c(i,8,
     & 1,0)*(c(i,0,0,0)*ui(i1,i2+8,i3+8,c3)+c(i,1,0,0)*ui(i1+1,i2+8,
     & i3+8,c3)+c(i,2,0,0)*ui(i1+2,i2+8,i3+8,c3)+c(i,3,0,0)*ui(i1+3,
     & i2+8,i3+8,c3)+c(i,4,0,0)*ui(i1+4,i2+8,i3+8,c3)+c(i,5,0,0)*ui(
     & i1+5,i2+8,i3+8,c3)+c(i,6,0,0)*ui(i1+6,i2+8,i3+8,c3)+c(i,7,0,0)*
     & ui(i1+7,i2+8,i3+8,c3)+c(i,8,0,0)*ui(i1+8,i2+8,i3+8,c3)))


              end do
              end do
            end if
          else
            ! general case width's in 3D **** fix this *********
            stop 5
          end if ! end width
        end if ! end nd
c**      else if( storageOption.eq.2 )then
! #If "TP" == "SP"
       else
         write(*,*) 'interpOpt:ERROR; unknown storage option=',
     & storageOption
         stop 3
       end if ! end storage option
       return
       end
