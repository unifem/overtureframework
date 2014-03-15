! This file automatically generated from interpOpt.bf with bpp.
! defineInterpOptRes(Full)
       subroutine interpOptResFull ( nd,ndui1a,ndui1b,ndui2a,ndui2b,
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
! #If "Full" == "Full"
       if( storageOption.eq.0 )then
c       ******************************
c       **** full storage option *****
c       ******************************
       if( nd.eq.2 )then
         if( useVariableWidthInterpolation.ne.0 )then
! beginLoops2d()
             do i=nia,nib
             do c3=c3a,c3b
             do c2=c2a,c2b
             ! check for most common widths first
             if( varWidth(i).eq.3 )then
! interp33(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,c3)+c(i,0,1,0)*ui(i1  
     & ,i2+1,c2,c3)+c(i,1,1,0)*ui(i1+1,i2+1,c2,c3)+c(i,2,1,0)*ui(i1+2,
     & i2+1,c2,c3)+c(i,0,2,0)*ui(i1  ,i2+2,c2,c3)+c(i,1,2,0)*ui(i1+1,
     & i2+2,c2,c3)+c(i,2,2,0)*ui(i1+2,i2+2,c2,c3)
             else if( varWidth(i).eq.2 )then
! interp22(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2  ,c2,c3)+c(i,0,1,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,1,0)*ui(i1+
     & 1,i2+1,c2,c3)
             else if( varWidth(i).eq.1 )then
! interp11(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = ui(i1  ,i2  ,c2,c3)
             else if( varWidth(i).eq.5 )then
! interp55(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,c3)+c(i,3,0,0)*ui(i1+
     & 3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,c3)+c(i,0,1,0)*ui(i1  
     & ,i2+1,c2,c3)+c(i,1,1,0)*ui(i1+1,i2+1,c2,c3)+c(i,2,1,0)*ui(i1+2,
     & i2+1,c2,c3)+c(i,3,1,0)*ui(i1+3,i2+1,c2,c3)+c(i,4,1,0)*ui(i1+4,
     & i2+1,c2,c3)+c(i,0,2,0)*ui(i1  ,i2+2,c2,c3)+c(i,1,2,0)*ui(i1+1,
     & i2+2,c2,c3)+c(i,2,2,0)*ui(i1+2,i2+2,c2,c3)+c(i,3,2,0)*ui(i1+3,
     & i2+2,c2,c3)+c(i,4,2,0)*ui(i1+4,i2+2,c2,c3)+c(i,0,3,0)*ui(i1  ,
     & i2+3,c2,c3)+c(i,1,3,0)*ui(i1+1,i2+3,c2,c3)+c(i,2,3,0)*ui(i1+2,
     & i2+3,c2,c3)+c(i,3,3,0)*ui(i1+3,i2+3,c2,c3)+c(i,4,3,0)*ui(i1+4,
     & i2+3,c2,c3)+c(i,0,4,0)*ui(i1  ,i2+4,c2,c3)+c(i,1,4,0)*ui(i1+1,
     & i2+4,c2,c3)+c(i,2,4,0)*ui(i1+2,i2+4,c2,c3)+c(i,3,4,0)*ui(i1+3,
     & i2+4,c2,c3)+c(i,4,4,0)*ui(i1+4,i2+4,c2,c3)
             else if( varWidth(i).eq.4 )then
! interp44(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,c3)+c(i,3,0,0)*ui(i1+
     & 3,i2  ,c2,c3)+c(i,0,1,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,1,0)*ui(i1+
     & 1,i2+1,c2,c3)+c(i,2,1,0)*ui(i1+2,i2+1,c2,c3)+c(i,3,1,0)*ui(i1+
     & 3,i2+1,c2,c3)+c(i,0,2,0)*ui(i1  ,i2+2,c2,c3)+c(i,1,2,0)*ui(i1+
     & 1,i2+2,c2,c3)+c(i,2,2,0)*ui(i1+2,i2+2,c2,c3)+c(i,3,2,0)*ui(i1+
     & 3,i2+2,c2,c3)+c(i,0,3,0)*ui(i1  ,i2+3,c2,c3)+c(i,1,3,0)*ui(i1+
     & 1,i2+3,c2,c3)+c(i,2,3,0)*ui(i1+2,i2+3,c2,c3)+c(i,3,3,0)*ui(i1+
     & 3,i2+3,c2,c3)
             else if( varWidth(i).eq.7 )then
! interp77(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,c3)+c(i,3,0,0)*ui(i1+
     & 3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,c3)+c(i,5,0,0)*ui(i1+
     & 5,i2  ,c2,c3)+c(i,6,0,0)*ui(i1+6,i2  ,c2,c3)+c(i,0,1,0)*ui(i1  
     & ,i2+1,c2,c3)+c(i,1,1,0)*ui(i1+1,i2+1,c2,c3)+c(i,2,1,0)*ui(i1+2,
     & i2+1,c2,c3)+c(i,3,1,0)*ui(i1+3,i2+1,c2,c3)+c(i,4,1,0)*ui(i1+4,
     & i2+1,c2,c3)+c(i,5,1,0)*ui(i1+5,i2+1,c2,c3)+c(i,6,1,0)*ui(i1+6,
     & i2+1,c2,c3)+c(i,0,2,0)*ui(i1  ,i2+2,c2,c3)+c(i,1,2,0)*ui(i1+1,
     & i2+2,c2,c3)+c(i,2,2,0)*ui(i1+2,i2+2,c2,c3)+c(i,3,2,0)*ui(i1+3,
     & i2+2,c2,c3)+c(i,4,2,0)*ui(i1+4,i2+2,c2,c3)+c(i,5,2,0)*ui(i1+5,
     & i2+2,c2,c3)+c(i,6,2,0)*ui(i1+6,i2+2,c2,c3)+c(i,0,3,0)*ui(i1  ,
     & i2+3,c2,c3)+c(i,1,3,0)*ui(i1+1,i2+3,c2,c3)+c(i,2,3,0)*ui(i1+2,
     & i2+3,c2,c3)+c(i,3,3,0)*ui(i1+3,i2+3,c2,c3)+c(i,4,3,0)*ui(i1+4,
     & i2+3,c2,c3)+c(i,5,3,0)*ui(i1+5,i2+3,c2,c3)+c(i,6,3,0)*ui(i1+6,
     & i2+3,c2,c3)+c(i,0,4,0)*ui(i1  ,i2+4,c2,c3)+c(i,1,4,0)*ui(i1+1,
     & i2+4,c2,c3)+c(i,2,4,0)*ui(i1+2,i2+4,c2,c3)+c(i,3,4,0)*ui(i1+3,
     & i2+4,c2,c3)+c(i,4,4,0)*ui(i1+4,i2+4,c2,c3)+c(i,5,4,0)*ui(i1+5,
     & i2+4,c2,c3)+c(i,6,4,0)*ui(i1+6,i2+4,c2,c3)+c(i,0,5,0)*ui(i1  ,
     & i2+5,c2,c3)+c(i,1,5,0)*ui(i1+1,i2+5,c2,c3)+c(i,2,5,0)*ui(i1+2,
     & i2+5,c2,c3)+c(i,3,5,0)*ui(i1+3,i2+5,c2,c3)+c(i,4,5,0)*ui(i1+4,
     & i2+5,c2,c3)+c(i,5,5,0)*ui(i1+5,i2+5,c2,c3)+c(i,6,5,0)*ui(i1+6,
     & i2+5,c2,c3)+c(i,0,6,0)*ui(i1  ,i2+6,c2,c3)+c(i,1,6,0)*ui(i1+1,
     & i2+6,c2,c3)+c(i,2,6,0)*ui(i1+2,i2+6,c2,c3)+c(i,3,6,0)*ui(i1+3,
     & i2+6,c2,c3)+c(i,4,6,0)*ui(i1+4,i2+6,c2,c3)+c(i,5,6,0)*ui(i1+5,
     & i2+6,c2,c3)+c(i,6,6,0)*ui(i1+6,i2+6,c2,c3)
             else if( varWidth(i).eq.6 )then
! interp66(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,c3)+c(i,3,0,0)*ui(i1+
     & 3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,c3)+c(i,5,0,0)*ui(i1+
     & 5,i2  ,c2,c3)+c(i,0,1,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,1,0)*ui(i1+
     & 1,i2+1,c2,c3)+c(i,2,1,0)*ui(i1+2,i2+1,c2,c3)+c(i,3,1,0)*ui(i1+
     & 3,i2+1,c2,c3)+c(i,4,1,0)*ui(i1+4,i2+1,c2,c3)+c(i,5,1,0)*ui(i1+
     & 5,i2+1,c2,c3)+c(i,0,2,0)*ui(i1  ,i2+2,c2,c3)+c(i,1,2,0)*ui(i1+
     & 1,i2+2,c2,c3)+c(i,2,2,0)*ui(i1+2,i2+2,c2,c3)+c(i,3,2,0)*ui(i1+
     & 3,i2+2,c2,c3)+c(i,4,2,0)*ui(i1+4,i2+2,c2,c3)+c(i,5,2,0)*ui(i1+
     & 5,i2+2,c2,c3)+c(i,0,3,0)*ui(i1  ,i2+3,c2,c3)+c(i,1,3,0)*ui(i1+
     & 1,i2+3,c2,c3)+c(i,2,3,0)*ui(i1+2,i2+3,c2,c3)+c(i,3,3,0)*ui(i1+
     & 3,i2+3,c2,c3)+c(i,4,3,0)*ui(i1+4,i2+3,c2,c3)+c(i,5,3,0)*ui(i1+
     & 5,i2+3,c2,c3)+c(i,0,4,0)*ui(i1  ,i2+4,c2,c3)+c(i,1,4,0)*ui(i1+
     & 1,i2+4,c2,c3)+c(i,2,4,0)*ui(i1+2,i2+4,c2,c3)+c(i,3,4,0)*ui(i1+
     & 3,i2+4,c2,c3)+c(i,4,4,0)*ui(i1+4,i2+4,c2,c3)+c(i,5,4,0)*ui(i1+
     & 5,i2+4,c2,c3)+c(i,0,5,0)*ui(i1  ,i2+5,c2,c3)+c(i,1,5,0)*ui(i1+
     & 1,i2+5,c2,c3)+c(i,2,5,0)*ui(i1+2,i2+5,c2,c3)+c(i,3,5,0)*ui(i1+
     & 3,i2+5,c2,c3)+c(i,4,5,0)*ui(i1+4,i2+5,c2,c3)+c(i,5,5,0)*ui(i1+
     & 5,i2+5,c2,c3)
             else if( varWidth(i).eq.9 )then
! interp99(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,c3)+c(i,3,0,0)*ui(i1+
     & 3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,c3)+c(i,5,0,0)*ui(i1+
     & 5,i2  ,c2,c3)+c(i,6,0,0)*ui(i1+6,i2  ,c2,c3)+c(i,7,0,0)*ui(i1+
     & 7,i2  ,c2,c3)+c(i,8,0,0)*ui(i1+8,i2  ,c2,c3)+c(i,0,1,0)*ui(i1  
     & ,i2+1,c2,c3)+c(i,1,1,0)*ui(i1+1,i2+1,c2,c3)+c(i,2,1,0)*ui(i1+2,
     & i2+1,c2,c3)+c(i,3,1,0)*ui(i1+3,i2+1,c2,c3)+c(i,4,1,0)*ui(i1+4,
     & i2+1,c2,c3)+c(i,5,1,0)*ui(i1+5,i2+1,c2,c3)+c(i,6,1,0)*ui(i1+6,
     & i2+1,c2,c3)+c(i,7,1,0)*ui(i1+7,i2+1,c2,c3)+c(i,8,1,0)*ui(i1+8,
     & i2+1,c2,c3)+c(i,0,2,0)*ui(i1  ,i2+2,c2,c3)+c(i,1,2,0)*ui(i1+1,
     & i2+2,c2,c3)+c(i,2,2,0)*ui(i1+2,i2+2,c2,c3)+c(i,3,2,0)*ui(i1+3,
     & i2+2,c2,c3)+c(i,4,2,0)*ui(i1+4,i2+2,c2,c3)+c(i,5,2,0)*ui(i1+5,
     & i2+2,c2,c3)+c(i,6,2,0)*ui(i1+6,i2+2,c2,c3)+c(i,7,2,0)*ui(i1+7,
     & i2+2,c2,c3)+c(i,8,2,0)*ui(i1+8,i2+2,c2,c3)+c(i,0,3,0)*ui(i1  ,
     & i2+3,c2,c3)+c(i,1,3,0)*ui(i1+1,i2+3,c2,c3)+c(i,2,3,0)*ui(i1+2,
     & i2+3,c2,c3)+c(i,3,3,0)*ui(i1+3,i2+3,c2,c3)+c(i,4,3,0)*ui(i1+4,
     & i2+3,c2,c3)+c(i,5,3,0)*ui(i1+5,i2+3,c2,c3)+c(i,6,3,0)*ui(i1+6,
     & i2+3,c2,c3)+c(i,7,3,0)*ui(i1+7,i2+3,c2,c3)+c(i,8,3,0)*ui(i1+8,
     & i2+3,c2,c3)+c(i,0,4,0)*ui(i1  ,i2+4,c2,c3)+c(i,1,4,0)*ui(i1+1,
     & i2+4,c2,c3)+c(i,2,4,0)*ui(i1+2,i2+4,c2,c3)+c(i,3,4,0)*ui(i1+3,
     & i2+4,c2,c3)+c(i,4,4,0)*ui(i1+4,i2+4,c2,c3)+c(i,5,4,0)*ui(i1+5,
     & i2+4,c2,c3)+c(i,6,4,0)*ui(i1+6,i2+4,c2,c3)+c(i,7,4,0)*ui(i1+7,
     & i2+4,c2,c3)+c(i,8,4,0)*ui(i1+8,i2+4,c2,c3)+c(i,0,5,0)*ui(i1  ,
     & i2+5,c2,c3)+c(i,1,5,0)*ui(i1+1,i2+5,c2,c3)+c(i,2,5,0)*ui(i1+2,
     & i2+5,c2,c3)+c(i,3,5,0)*ui(i1+3,i2+5,c2,c3)+c(i,4,5,0)*ui(i1+4,
     & i2+5,c2,c3)+c(i,5,5,0)*ui(i1+5,i2+5,c2,c3)+c(i,6,5,0)*ui(i1+6,
     & i2+5,c2,c3)+c(i,7,5,0)*ui(i1+7,i2+5,c2,c3)+c(i,8,5,0)*ui(i1+8,
     & i2+5,c2,c3)+c(i,0,6,0)*ui(i1  ,i2+6,c2,c3)+c(i,1,6,0)*ui(i1+1,
     & i2+6,c2,c3)+c(i,2,6,0)*ui(i1+2,i2+6,c2,c3)+c(i,3,6,0)*ui(i1+3,
     & i2+6,c2,c3)+c(i,4,6,0)*ui(i1+4,i2+6,c2,c3)+c(i,5,6,0)*ui(i1+5,
     & i2+6,c2,c3)+c(i,6,6,0)*ui(i1+6,i2+6,c2,c3)+c(i,7,6,0)*ui(i1+7,
     & i2+6,c2,c3)+c(i,8,6,0)*ui(i1+8,i2+6,c2,c3)+c(i,0,7,0)*ui(i1  ,
     & i2+7,c2,c3)+c(i,1,7,0)*ui(i1+1,i2+7,c2,c3)+c(i,2,7,0)*ui(i1+2,
     & i2+7,c2,c3)+c(i,3,7,0)*ui(i1+3,i2+7,c2,c3)+c(i,4,7,0)*ui(i1+4,
     & i2+7,c2,c3)+c(i,5,7,0)*ui(i1+5,i2+7,c2,c3)+c(i,6,7,0)*ui(i1+6,
     & i2+7,c2,c3)+c(i,7,7,0)*ui(i1+7,i2+7,c2,c3)+c(i,8,7,0)*ui(i1+8,
     & i2+7,c2,c3)+c(i,0,8,0)*ui(i1  ,i2+8,c2,c3)+c(i,1,8,0)*ui(i1+1,
     & i2+8,c2,c3)+c(i,2,8,0)*ui(i1+2,i2+8,c2,c3)+c(i,3,8,0)*ui(i1+3,
     & i2+8,c2,c3)+c(i,4,8,0)*ui(i1+4,i2+8,c2,c3)+c(i,5,8,0)*ui(i1+5,
     & i2+8,c2,c3)+c(i,6,8,0)*ui(i1+6,i2+8,c2,c3)+c(i,7,8,0)*ui(i1+7,
     & i2+8,c2,c3)+c(i,8,8,0)*ui(i1+8,i2+8,c2,c3)
             else if( varWidth(i).eq.8 )then
! interp88(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,c3)+c(i,3,0,0)*ui(i1+
     & 3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,c3)+c(i,5,0,0)*ui(i1+
     & 5,i2  ,c2,c3)+c(i,6,0,0)*ui(i1+6,i2  ,c2,c3)+c(i,7,0,0)*ui(i1+
     & 7,i2  ,c2,c3)+c(i,0,1,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,1,0)*ui(i1+
     & 1,i2+1,c2,c3)+c(i,2,1,0)*ui(i1+2,i2+1,c2,c3)+c(i,3,1,0)*ui(i1+
     & 3,i2+1,c2,c3)+c(i,4,1,0)*ui(i1+4,i2+1,c2,c3)+c(i,5,1,0)*ui(i1+
     & 5,i2+1,c2,c3)+c(i,6,1,0)*ui(i1+6,i2+1,c2,c3)+c(i,7,1,0)*ui(i1+
     & 7,i2+1,c2,c3)+c(i,0,2,0)*ui(i1  ,i2+2,c2,c3)+c(i,1,2,0)*ui(i1+
     & 1,i2+2,c2,c3)+c(i,2,2,0)*ui(i1+2,i2+2,c2,c3)+c(i,3,2,0)*ui(i1+
     & 3,i2+2,c2,c3)+c(i,4,2,0)*ui(i1+4,i2+2,c2,c3)+c(i,5,2,0)*ui(i1+
     & 5,i2+2,c2,c3)+c(i,6,2,0)*ui(i1+6,i2+2,c2,c3)+c(i,7,2,0)*ui(i1+
     & 7,i2+2,c2,c3)+c(i,0,3,0)*ui(i1  ,i2+3,c2,c3)+c(i,1,3,0)*ui(i1+
     & 1,i2+3,c2,c3)+c(i,2,3,0)*ui(i1+2,i2+3,c2,c3)+c(i,3,3,0)*ui(i1+
     & 3,i2+3,c2,c3)+c(i,4,3,0)*ui(i1+4,i2+3,c2,c3)+c(i,5,3,0)*ui(i1+
     & 5,i2+3,c2,c3)+c(i,6,3,0)*ui(i1+6,i2+3,c2,c3)+c(i,7,3,0)*ui(i1+
     & 7,i2+3,c2,c3)+c(i,0,4,0)*ui(i1  ,i2+4,c2,c3)+c(i,1,4,0)*ui(i1+
     & 1,i2+4,c2,c3)+c(i,2,4,0)*ui(i1+2,i2+4,c2,c3)+c(i,3,4,0)*ui(i1+
     & 3,i2+4,c2,c3)+c(i,4,4,0)*ui(i1+4,i2+4,c2,c3)+c(i,5,4,0)*ui(i1+
     & 5,i2+4,c2,c3)+c(i,6,4,0)*ui(i1+6,i2+4,c2,c3)+c(i,7,4,0)*ui(i1+
     & 7,i2+4,c2,c3)+c(i,0,5,0)*ui(i1  ,i2+5,c2,c3)+c(i,1,5,0)*ui(i1+
     & 1,i2+5,c2,c3)+c(i,2,5,0)*ui(i1+2,i2+5,c2,c3)+c(i,3,5,0)*ui(i1+
     & 3,i2+5,c2,c3)+c(i,4,5,0)*ui(i1+4,i2+5,c2,c3)+c(i,5,5,0)*ui(i1+
     & 5,i2+5,c2,c3)+c(i,6,5,0)*ui(i1+6,i2+5,c2,c3)+c(i,7,5,0)*ui(i1+
     & 7,i2+5,c2,c3)+c(i,0,6,0)*ui(i1  ,i2+6,c2,c3)+c(i,1,6,0)*ui(i1+
     & 1,i2+6,c2,c3)+c(i,2,6,0)*ui(i1+2,i2+6,c2,c3)+c(i,3,6,0)*ui(i1+
     & 3,i2+6,c2,c3)+c(i,4,6,0)*ui(i1+4,i2+6,c2,c3)+c(i,5,6,0)*ui(i1+
     & 5,i2+6,c2,c3)+c(i,6,6,0)*ui(i1+6,i2+6,c2,c3)+c(i,7,6,0)*ui(i1+
     & 7,i2+6,c2,c3)+c(i,0,7,0)*ui(i1  ,i2+7,c2,c3)+c(i,1,7,0)*ui(i1+
     & 1,i2+7,c2,c3)+c(i,2,7,0)*ui(i1+2,i2+7,c2,c3)+c(i,3,7,0)*ui(i1+
     & 3,i2+7,c2,c3)+c(i,4,7,0)*ui(i1+4,i2+7,c2,c3)+c(i,5,7,0)*ui(i1+
     & 5,i2+7,c2,c3)+c(i,6,7,0)*ui(i1+6,i2+7,c2,c3)+c(i,7,7,0)*ui(i1+
     & 7,i2+7,c2,c3)
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
! loops2d($interp33(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),ug(ip(i,1),ip(i,2),c2,c3)= r(i))
           if( c2a.eq.c2b .and. c3a.eq.c3b )then
             do c3=c3a,c3b
             do c2=c2a,c2b
             do i=nia,nib
! interp33(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,c3)+c(i,0,1,0)*ui(i1  
     & ,i2+1,c2,c3)+c(i,1,1,0)*ui(i1+1,i2+1,c2,c3)+c(i,2,1,0)*ui(i1+2,
     & i2+1,c2,c3)+c(i,0,2,0)*ui(i1  ,i2+2,c2,c3)+c(i,1,2,0)*ui(i1+1,
     & i2+2,c2,c3)+c(i,2,2,0)*ui(i1+2,i2+2,c2,c3)
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
! interp33(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,c3)+c(i,0,1,0)*ui(i1  
     & ,i2+1,c2,c3)+c(i,1,1,0)*ui(i1+1,i2+1,c2,c3)+c(i,2,1,0)*ui(i1+2,
     & i2+1,c2,c3)+c(i,0,2,0)*ui(i1  ,i2+2,c2,c3)+c(i,1,2,0)*ui(i1+1,
     & i2+2,c2,c3)+c(i,2,2,0)*ui(i1+2,i2+2,c2,c3)
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
! loops2d($interp22(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),ug(ip(i,1),ip(i,2),c2,c3)= r(i))
           if( c2a.eq.c2b .and. c3a.eq.c3b )then
             do c3=c3a,c3b
             do c2=c2a,c2b
             do i=nia,nib
! interp22(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2  ,c2,c3)+c(i,0,1,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,1,0)*ui(i1+
     & 1,i2+1,c2,c3)
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
! interp22(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2  ,c2,c3)+c(i,0,1,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,1,0)*ui(i1+
     & 1,i2+1,c2,c3)
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i)))
               ug(ip(i,1),ip(i,2),c2,c3)=r(i)
             end do
             end do
             end do
           end if
         else if( width(1).eq.4 .and. width(2).eq.4 )then
! loops2d($interp44(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),ug(ip(i,1),ip(i,2),c2,c3)= r(i))
           if( c2a.eq.c2b .and. c3a.eq.c3b )then
             do c3=c3a,c3b
             do c2=c2a,c2b
             do i=nia,nib
! interp44(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,c3)+c(i,3,0,0)*ui(i1+
     & 3,i2  ,c2,c3)+c(i,0,1,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,1,0)*ui(i1+
     & 1,i2+1,c2,c3)+c(i,2,1,0)*ui(i1+2,i2+1,c2,c3)+c(i,3,1,0)*ui(i1+
     & 3,i2+1,c2,c3)+c(i,0,2,0)*ui(i1  ,i2+2,c2,c3)+c(i,1,2,0)*ui(i1+
     & 1,i2+2,c2,c3)+c(i,2,2,0)*ui(i1+2,i2+2,c2,c3)+c(i,3,2,0)*ui(i1+
     & 3,i2+2,c2,c3)+c(i,0,3,0)*ui(i1  ,i2+3,c2,c3)+c(i,1,3,0)*ui(i1+
     & 1,i2+3,c2,c3)+c(i,2,3,0)*ui(i1+2,i2+3,c2,c3)+c(i,3,3,0)*ui(i1+
     & 3,i2+3,c2,c3)
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
! interp44(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,c3)+c(i,3,0,0)*ui(i1+
     & 3,i2  ,c2,c3)+c(i,0,1,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,1,0)*ui(i1+
     & 1,i2+1,c2,c3)+c(i,2,1,0)*ui(i1+2,i2+1,c2,c3)+c(i,3,1,0)*ui(i1+
     & 3,i2+1,c2,c3)+c(i,0,2,0)*ui(i1  ,i2+2,c2,c3)+c(i,1,2,0)*ui(i1+
     & 1,i2+2,c2,c3)+c(i,2,2,0)*ui(i1+2,i2+2,c2,c3)+c(i,3,2,0)*ui(i1+
     & 3,i2+2,c2,c3)+c(i,0,3,0)*ui(i1  ,i2+3,c2,c3)+c(i,1,3,0)*ui(i1+
     & 1,i2+3,c2,c3)+c(i,2,3,0)*ui(i1+2,i2+3,c2,c3)+c(i,3,3,0)*ui(i1+
     & 3,i2+3,c2,c3)
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i)))
               ug(ip(i,1),ip(i,2),c2,c3)=r(i)
             end do
             end do
             end do
           end if
         else if( width(1).eq.5 .and. width(2).eq.5 )then
! loops2d($interp55(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),ug(ip(i,1),ip(i,2),c2,c3)= r(i))
           if( c2a.eq.c2b .and. c3a.eq.c3b )then
             do c3=c3a,c3b
             do c2=c2a,c2b
             do i=nia,nib
! interp55(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,c3)+c(i,3,0,0)*ui(i1+
     & 3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,c3)+c(i,0,1,0)*ui(i1  
     & ,i2+1,c2,c3)+c(i,1,1,0)*ui(i1+1,i2+1,c2,c3)+c(i,2,1,0)*ui(i1+2,
     & i2+1,c2,c3)+c(i,3,1,0)*ui(i1+3,i2+1,c2,c3)+c(i,4,1,0)*ui(i1+4,
     & i2+1,c2,c3)+c(i,0,2,0)*ui(i1  ,i2+2,c2,c3)+c(i,1,2,0)*ui(i1+1,
     & i2+2,c2,c3)+c(i,2,2,0)*ui(i1+2,i2+2,c2,c3)+c(i,3,2,0)*ui(i1+3,
     & i2+2,c2,c3)+c(i,4,2,0)*ui(i1+4,i2+2,c2,c3)+c(i,0,3,0)*ui(i1  ,
     & i2+3,c2,c3)+c(i,1,3,0)*ui(i1+1,i2+3,c2,c3)+c(i,2,3,0)*ui(i1+2,
     & i2+3,c2,c3)+c(i,3,3,0)*ui(i1+3,i2+3,c2,c3)+c(i,4,3,0)*ui(i1+4,
     & i2+3,c2,c3)+c(i,0,4,0)*ui(i1  ,i2+4,c2,c3)+c(i,1,4,0)*ui(i1+1,
     & i2+4,c2,c3)+c(i,2,4,0)*ui(i1+2,i2+4,c2,c3)+c(i,3,4,0)*ui(i1+3,
     & i2+4,c2,c3)+c(i,4,4,0)*ui(i1+4,i2+4,c2,c3)
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
! interp55(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,c3)+c(i,3,0,0)*ui(i1+
     & 3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,c3)+c(i,0,1,0)*ui(i1  
     & ,i2+1,c2,c3)+c(i,1,1,0)*ui(i1+1,i2+1,c2,c3)+c(i,2,1,0)*ui(i1+2,
     & i2+1,c2,c3)+c(i,3,1,0)*ui(i1+3,i2+1,c2,c3)+c(i,4,1,0)*ui(i1+4,
     & i2+1,c2,c3)+c(i,0,2,0)*ui(i1  ,i2+2,c2,c3)+c(i,1,2,0)*ui(i1+1,
     & i2+2,c2,c3)+c(i,2,2,0)*ui(i1+2,i2+2,c2,c3)+c(i,3,2,0)*ui(i1+3,
     & i2+2,c2,c3)+c(i,4,2,0)*ui(i1+4,i2+2,c2,c3)+c(i,0,3,0)*ui(i1  ,
     & i2+3,c2,c3)+c(i,1,3,0)*ui(i1+1,i2+3,c2,c3)+c(i,2,3,0)*ui(i1+2,
     & i2+3,c2,c3)+c(i,3,3,0)*ui(i1+3,i2+3,c2,c3)+c(i,4,3,0)*ui(i1+4,
     & i2+3,c2,c3)+c(i,0,4,0)*ui(i1  ,i2+4,c2,c3)+c(i,1,4,0)*ui(i1+1,
     & i2+4,c2,c3)+c(i,2,4,0)*ui(i1+2,i2+4,c2,c3)+c(i,3,4,0)*ui(i1+3,
     & i2+4,c2,c3)+c(i,4,4,0)*ui(i1+4,i2+4,c2,c3)
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i)))
               ug(ip(i,1),ip(i,2),c2,c3)=r(i)
             end do
             end do
             end do
           end if
         else if( width(1).eq.6 .and. width(2).eq.6 )then
! loops2d($interp66(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),ug(ip(i,1),ip(i,2),c2,c3)= r(i))
           if( c2a.eq.c2b .and. c3a.eq.c3b )then
             do c3=c3a,c3b
             do c2=c2a,c2b
             do i=nia,nib
! interp66(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,c3)+c(i,3,0,0)*ui(i1+
     & 3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,c3)+c(i,5,0,0)*ui(i1+
     & 5,i2  ,c2,c3)+c(i,0,1,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,1,0)*ui(i1+
     & 1,i2+1,c2,c3)+c(i,2,1,0)*ui(i1+2,i2+1,c2,c3)+c(i,3,1,0)*ui(i1+
     & 3,i2+1,c2,c3)+c(i,4,1,0)*ui(i1+4,i2+1,c2,c3)+c(i,5,1,0)*ui(i1+
     & 5,i2+1,c2,c3)+c(i,0,2,0)*ui(i1  ,i2+2,c2,c3)+c(i,1,2,0)*ui(i1+
     & 1,i2+2,c2,c3)+c(i,2,2,0)*ui(i1+2,i2+2,c2,c3)+c(i,3,2,0)*ui(i1+
     & 3,i2+2,c2,c3)+c(i,4,2,0)*ui(i1+4,i2+2,c2,c3)+c(i,5,2,0)*ui(i1+
     & 5,i2+2,c2,c3)+c(i,0,3,0)*ui(i1  ,i2+3,c2,c3)+c(i,1,3,0)*ui(i1+
     & 1,i2+3,c2,c3)+c(i,2,3,0)*ui(i1+2,i2+3,c2,c3)+c(i,3,3,0)*ui(i1+
     & 3,i2+3,c2,c3)+c(i,4,3,0)*ui(i1+4,i2+3,c2,c3)+c(i,5,3,0)*ui(i1+
     & 5,i2+3,c2,c3)+c(i,0,4,0)*ui(i1  ,i2+4,c2,c3)+c(i,1,4,0)*ui(i1+
     & 1,i2+4,c2,c3)+c(i,2,4,0)*ui(i1+2,i2+4,c2,c3)+c(i,3,4,0)*ui(i1+
     & 3,i2+4,c2,c3)+c(i,4,4,0)*ui(i1+4,i2+4,c2,c3)+c(i,5,4,0)*ui(i1+
     & 5,i2+4,c2,c3)+c(i,0,5,0)*ui(i1  ,i2+5,c2,c3)+c(i,1,5,0)*ui(i1+
     & 1,i2+5,c2,c3)+c(i,2,5,0)*ui(i1+2,i2+5,c2,c3)+c(i,3,5,0)*ui(i1+
     & 3,i2+5,c2,c3)+c(i,4,5,0)*ui(i1+4,i2+5,c2,c3)+c(i,5,5,0)*ui(i1+
     & 5,i2+5,c2,c3)
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
! interp66(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,c3)+c(i,3,0,0)*ui(i1+
     & 3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,c3)+c(i,5,0,0)*ui(i1+
     & 5,i2  ,c2,c3)+c(i,0,1,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,1,0)*ui(i1+
     & 1,i2+1,c2,c3)+c(i,2,1,0)*ui(i1+2,i2+1,c2,c3)+c(i,3,1,0)*ui(i1+
     & 3,i2+1,c2,c3)+c(i,4,1,0)*ui(i1+4,i2+1,c2,c3)+c(i,5,1,0)*ui(i1+
     & 5,i2+1,c2,c3)+c(i,0,2,0)*ui(i1  ,i2+2,c2,c3)+c(i,1,2,0)*ui(i1+
     & 1,i2+2,c2,c3)+c(i,2,2,0)*ui(i1+2,i2+2,c2,c3)+c(i,3,2,0)*ui(i1+
     & 3,i2+2,c2,c3)+c(i,4,2,0)*ui(i1+4,i2+2,c2,c3)+c(i,5,2,0)*ui(i1+
     & 5,i2+2,c2,c3)+c(i,0,3,0)*ui(i1  ,i2+3,c2,c3)+c(i,1,3,0)*ui(i1+
     & 1,i2+3,c2,c3)+c(i,2,3,0)*ui(i1+2,i2+3,c2,c3)+c(i,3,3,0)*ui(i1+
     & 3,i2+3,c2,c3)+c(i,4,3,0)*ui(i1+4,i2+3,c2,c3)+c(i,5,3,0)*ui(i1+
     & 5,i2+3,c2,c3)+c(i,0,4,0)*ui(i1  ,i2+4,c2,c3)+c(i,1,4,0)*ui(i1+
     & 1,i2+4,c2,c3)+c(i,2,4,0)*ui(i1+2,i2+4,c2,c3)+c(i,3,4,0)*ui(i1+
     & 3,i2+4,c2,c3)+c(i,4,4,0)*ui(i1+4,i2+4,c2,c3)+c(i,5,4,0)*ui(i1+
     & 5,i2+4,c2,c3)+c(i,0,5,0)*ui(i1  ,i2+5,c2,c3)+c(i,1,5,0)*ui(i1+
     & 1,i2+5,c2,c3)+c(i,2,5,0)*ui(i1+2,i2+5,c2,c3)+c(i,3,5,0)*ui(i1+
     & 3,i2+5,c2,c3)+c(i,4,5,0)*ui(i1+4,i2+5,c2,c3)+c(i,5,5,0)*ui(i1+
     & 5,i2+5,c2,c3)
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i)))
               ug(ip(i,1),ip(i,2),c2,c3)=r(i)
             end do
             end do
             end do
           end if
         else if( width(1).eq.7 .and. width(2).eq.7 )then
! loops2d($interp77(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),ug(ip(i,1),ip(i,2),c2,c3)= r(i))
           if( c2a.eq.c2b .and. c3a.eq.c3b )then
             do c3=c3a,c3b
             do c2=c2a,c2b
             do i=nia,nib
! interp77(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,c3)+c(i,3,0,0)*ui(i1+
     & 3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,c3)+c(i,5,0,0)*ui(i1+
     & 5,i2  ,c2,c3)+c(i,6,0,0)*ui(i1+6,i2  ,c2,c3)+c(i,0,1,0)*ui(i1  
     & ,i2+1,c2,c3)+c(i,1,1,0)*ui(i1+1,i2+1,c2,c3)+c(i,2,1,0)*ui(i1+2,
     & i2+1,c2,c3)+c(i,3,1,0)*ui(i1+3,i2+1,c2,c3)+c(i,4,1,0)*ui(i1+4,
     & i2+1,c2,c3)+c(i,5,1,0)*ui(i1+5,i2+1,c2,c3)+c(i,6,1,0)*ui(i1+6,
     & i2+1,c2,c3)+c(i,0,2,0)*ui(i1  ,i2+2,c2,c3)+c(i,1,2,0)*ui(i1+1,
     & i2+2,c2,c3)+c(i,2,2,0)*ui(i1+2,i2+2,c2,c3)+c(i,3,2,0)*ui(i1+3,
     & i2+2,c2,c3)+c(i,4,2,0)*ui(i1+4,i2+2,c2,c3)+c(i,5,2,0)*ui(i1+5,
     & i2+2,c2,c3)+c(i,6,2,0)*ui(i1+6,i2+2,c2,c3)+c(i,0,3,0)*ui(i1  ,
     & i2+3,c2,c3)+c(i,1,3,0)*ui(i1+1,i2+3,c2,c3)+c(i,2,3,0)*ui(i1+2,
     & i2+3,c2,c3)+c(i,3,3,0)*ui(i1+3,i2+3,c2,c3)+c(i,4,3,0)*ui(i1+4,
     & i2+3,c2,c3)+c(i,5,3,0)*ui(i1+5,i2+3,c2,c3)+c(i,6,3,0)*ui(i1+6,
     & i2+3,c2,c3)+c(i,0,4,0)*ui(i1  ,i2+4,c2,c3)+c(i,1,4,0)*ui(i1+1,
     & i2+4,c2,c3)+c(i,2,4,0)*ui(i1+2,i2+4,c2,c3)+c(i,3,4,0)*ui(i1+3,
     & i2+4,c2,c3)+c(i,4,4,0)*ui(i1+4,i2+4,c2,c3)+c(i,5,4,0)*ui(i1+5,
     & i2+4,c2,c3)+c(i,6,4,0)*ui(i1+6,i2+4,c2,c3)+c(i,0,5,0)*ui(i1  ,
     & i2+5,c2,c3)+c(i,1,5,0)*ui(i1+1,i2+5,c2,c3)+c(i,2,5,0)*ui(i1+2,
     & i2+5,c2,c3)+c(i,3,5,0)*ui(i1+3,i2+5,c2,c3)+c(i,4,5,0)*ui(i1+4,
     & i2+5,c2,c3)+c(i,5,5,0)*ui(i1+5,i2+5,c2,c3)+c(i,6,5,0)*ui(i1+6,
     & i2+5,c2,c3)+c(i,0,6,0)*ui(i1  ,i2+6,c2,c3)+c(i,1,6,0)*ui(i1+1,
     & i2+6,c2,c3)+c(i,2,6,0)*ui(i1+2,i2+6,c2,c3)+c(i,3,6,0)*ui(i1+3,
     & i2+6,c2,c3)+c(i,4,6,0)*ui(i1+4,i2+6,c2,c3)+c(i,5,6,0)*ui(i1+5,
     & i2+6,c2,c3)+c(i,6,6,0)*ui(i1+6,i2+6,c2,c3)
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
! interp77(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,c3)+c(i,3,0,0)*ui(i1+
     & 3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,c3)+c(i,5,0,0)*ui(i1+
     & 5,i2  ,c2,c3)+c(i,6,0,0)*ui(i1+6,i2  ,c2,c3)+c(i,0,1,0)*ui(i1  
     & ,i2+1,c2,c3)+c(i,1,1,0)*ui(i1+1,i2+1,c2,c3)+c(i,2,1,0)*ui(i1+2,
     & i2+1,c2,c3)+c(i,3,1,0)*ui(i1+3,i2+1,c2,c3)+c(i,4,1,0)*ui(i1+4,
     & i2+1,c2,c3)+c(i,5,1,0)*ui(i1+5,i2+1,c2,c3)+c(i,6,1,0)*ui(i1+6,
     & i2+1,c2,c3)+c(i,0,2,0)*ui(i1  ,i2+2,c2,c3)+c(i,1,2,0)*ui(i1+1,
     & i2+2,c2,c3)+c(i,2,2,0)*ui(i1+2,i2+2,c2,c3)+c(i,3,2,0)*ui(i1+3,
     & i2+2,c2,c3)+c(i,4,2,0)*ui(i1+4,i2+2,c2,c3)+c(i,5,2,0)*ui(i1+5,
     & i2+2,c2,c3)+c(i,6,2,0)*ui(i1+6,i2+2,c2,c3)+c(i,0,3,0)*ui(i1  ,
     & i2+3,c2,c3)+c(i,1,3,0)*ui(i1+1,i2+3,c2,c3)+c(i,2,3,0)*ui(i1+2,
     & i2+3,c2,c3)+c(i,3,3,0)*ui(i1+3,i2+3,c2,c3)+c(i,4,3,0)*ui(i1+4,
     & i2+3,c2,c3)+c(i,5,3,0)*ui(i1+5,i2+3,c2,c3)+c(i,6,3,0)*ui(i1+6,
     & i2+3,c2,c3)+c(i,0,4,0)*ui(i1  ,i2+4,c2,c3)+c(i,1,4,0)*ui(i1+1,
     & i2+4,c2,c3)+c(i,2,4,0)*ui(i1+2,i2+4,c2,c3)+c(i,3,4,0)*ui(i1+3,
     & i2+4,c2,c3)+c(i,4,4,0)*ui(i1+4,i2+4,c2,c3)+c(i,5,4,0)*ui(i1+5,
     & i2+4,c2,c3)+c(i,6,4,0)*ui(i1+6,i2+4,c2,c3)+c(i,0,5,0)*ui(i1  ,
     & i2+5,c2,c3)+c(i,1,5,0)*ui(i1+1,i2+5,c2,c3)+c(i,2,5,0)*ui(i1+2,
     & i2+5,c2,c3)+c(i,3,5,0)*ui(i1+3,i2+5,c2,c3)+c(i,4,5,0)*ui(i1+4,
     & i2+5,c2,c3)+c(i,5,5,0)*ui(i1+5,i2+5,c2,c3)+c(i,6,5,0)*ui(i1+6,
     & i2+5,c2,c3)+c(i,0,6,0)*ui(i1  ,i2+6,c2,c3)+c(i,1,6,0)*ui(i1+1,
     & i2+6,c2,c3)+c(i,2,6,0)*ui(i1+2,i2+6,c2,c3)+c(i,3,6,0)*ui(i1+3,
     & i2+6,c2,c3)+c(i,4,6,0)*ui(i1+4,i2+6,c2,c3)+c(i,5,6,0)*ui(i1+5,
     & i2+6,c2,c3)+c(i,6,6,0)*ui(i1+6,i2+6,c2,c3)
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i)))
               ug(ip(i,1),ip(i,2),c2,c3)=r(i)
             end do
             end do
             end do
           end if
         else if( width(1).eq.8 .and. width(2).eq.8 )then
! loops2d($interp88(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),ug(ip(i,1),ip(i,2),c2,c3)= r(i))
           if( c2a.eq.c2b .and. c3a.eq.c3b )then
             do c3=c3a,c3b
             do c2=c2a,c2b
             do i=nia,nib
! interp88(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,c3)+c(i,3,0,0)*ui(i1+
     & 3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,c3)+c(i,5,0,0)*ui(i1+
     & 5,i2  ,c2,c3)+c(i,6,0,0)*ui(i1+6,i2  ,c2,c3)+c(i,7,0,0)*ui(i1+
     & 7,i2  ,c2,c3)+c(i,0,1,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,1,0)*ui(i1+
     & 1,i2+1,c2,c3)+c(i,2,1,0)*ui(i1+2,i2+1,c2,c3)+c(i,3,1,0)*ui(i1+
     & 3,i2+1,c2,c3)+c(i,4,1,0)*ui(i1+4,i2+1,c2,c3)+c(i,5,1,0)*ui(i1+
     & 5,i2+1,c2,c3)+c(i,6,1,0)*ui(i1+6,i2+1,c2,c3)+c(i,7,1,0)*ui(i1+
     & 7,i2+1,c2,c3)+c(i,0,2,0)*ui(i1  ,i2+2,c2,c3)+c(i,1,2,0)*ui(i1+
     & 1,i2+2,c2,c3)+c(i,2,2,0)*ui(i1+2,i2+2,c2,c3)+c(i,3,2,0)*ui(i1+
     & 3,i2+2,c2,c3)+c(i,4,2,0)*ui(i1+4,i2+2,c2,c3)+c(i,5,2,0)*ui(i1+
     & 5,i2+2,c2,c3)+c(i,6,2,0)*ui(i1+6,i2+2,c2,c3)+c(i,7,2,0)*ui(i1+
     & 7,i2+2,c2,c3)+c(i,0,3,0)*ui(i1  ,i2+3,c2,c3)+c(i,1,3,0)*ui(i1+
     & 1,i2+3,c2,c3)+c(i,2,3,0)*ui(i1+2,i2+3,c2,c3)+c(i,3,3,0)*ui(i1+
     & 3,i2+3,c2,c3)+c(i,4,3,0)*ui(i1+4,i2+3,c2,c3)+c(i,5,3,0)*ui(i1+
     & 5,i2+3,c2,c3)+c(i,6,3,0)*ui(i1+6,i2+3,c2,c3)+c(i,7,3,0)*ui(i1+
     & 7,i2+3,c2,c3)+c(i,0,4,0)*ui(i1  ,i2+4,c2,c3)+c(i,1,4,0)*ui(i1+
     & 1,i2+4,c2,c3)+c(i,2,4,0)*ui(i1+2,i2+4,c2,c3)+c(i,3,4,0)*ui(i1+
     & 3,i2+4,c2,c3)+c(i,4,4,0)*ui(i1+4,i2+4,c2,c3)+c(i,5,4,0)*ui(i1+
     & 5,i2+4,c2,c3)+c(i,6,4,0)*ui(i1+6,i2+4,c2,c3)+c(i,7,4,0)*ui(i1+
     & 7,i2+4,c2,c3)+c(i,0,5,0)*ui(i1  ,i2+5,c2,c3)+c(i,1,5,0)*ui(i1+
     & 1,i2+5,c2,c3)+c(i,2,5,0)*ui(i1+2,i2+5,c2,c3)+c(i,3,5,0)*ui(i1+
     & 3,i2+5,c2,c3)+c(i,4,5,0)*ui(i1+4,i2+5,c2,c3)+c(i,5,5,0)*ui(i1+
     & 5,i2+5,c2,c3)+c(i,6,5,0)*ui(i1+6,i2+5,c2,c3)+c(i,7,5,0)*ui(i1+
     & 7,i2+5,c2,c3)+c(i,0,6,0)*ui(i1  ,i2+6,c2,c3)+c(i,1,6,0)*ui(i1+
     & 1,i2+6,c2,c3)+c(i,2,6,0)*ui(i1+2,i2+6,c2,c3)+c(i,3,6,0)*ui(i1+
     & 3,i2+6,c2,c3)+c(i,4,6,0)*ui(i1+4,i2+6,c2,c3)+c(i,5,6,0)*ui(i1+
     & 5,i2+6,c2,c3)+c(i,6,6,0)*ui(i1+6,i2+6,c2,c3)+c(i,7,6,0)*ui(i1+
     & 7,i2+6,c2,c3)+c(i,0,7,0)*ui(i1  ,i2+7,c2,c3)+c(i,1,7,0)*ui(i1+
     & 1,i2+7,c2,c3)+c(i,2,7,0)*ui(i1+2,i2+7,c2,c3)+c(i,3,7,0)*ui(i1+
     & 3,i2+7,c2,c3)+c(i,4,7,0)*ui(i1+4,i2+7,c2,c3)+c(i,5,7,0)*ui(i1+
     & 5,i2+7,c2,c3)+c(i,6,7,0)*ui(i1+6,i2+7,c2,c3)+c(i,7,7,0)*ui(i1+
     & 7,i2+7,c2,c3)
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
! interp88(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,c3)+c(i,3,0,0)*ui(i1+
     & 3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,c3)+c(i,5,0,0)*ui(i1+
     & 5,i2  ,c2,c3)+c(i,6,0,0)*ui(i1+6,i2  ,c2,c3)+c(i,7,0,0)*ui(i1+
     & 7,i2  ,c2,c3)+c(i,0,1,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,1,0)*ui(i1+
     & 1,i2+1,c2,c3)+c(i,2,1,0)*ui(i1+2,i2+1,c2,c3)+c(i,3,1,0)*ui(i1+
     & 3,i2+1,c2,c3)+c(i,4,1,0)*ui(i1+4,i2+1,c2,c3)+c(i,5,1,0)*ui(i1+
     & 5,i2+1,c2,c3)+c(i,6,1,0)*ui(i1+6,i2+1,c2,c3)+c(i,7,1,0)*ui(i1+
     & 7,i2+1,c2,c3)+c(i,0,2,0)*ui(i1  ,i2+2,c2,c3)+c(i,1,2,0)*ui(i1+
     & 1,i2+2,c2,c3)+c(i,2,2,0)*ui(i1+2,i2+2,c2,c3)+c(i,3,2,0)*ui(i1+
     & 3,i2+2,c2,c3)+c(i,4,2,0)*ui(i1+4,i2+2,c2,c3)+c(i,5,2,0)*ui(i1+
     & 5,i2+2,c2,c3)+c(i,6,2,0)*ui(i1+6,i2+2,c2,c3)+c(i,7,2,0)*ui(i1+
     & 7,i2+2,c2,c3)+c(i,0,3,0)*ui(i1  ,i2+3,c2,c3)+c(i,1,3,0)*ui(i1+
     & 1,i2+3,c2,c3)+c(i,2,3,0)*ui(i1+2,i2+3,c2,c3)+c(i,3,3,0)*ui(i1+
     & 3,i2+3,c2,c3)+c(i,4,3,0)*ui(i1+4,i2+3,c2,c3)+c(i,5,3,0)*ui(i1+
     & 5,i2+3,c2,c3)+c(i,6,3,0)*ui(i1+6,i2+3,c2,c3)+c(i,7,3,0)*ui(i1+
     & 7,i2+3,c2,c3)+c(i,0,4,0)*ui(i1  ,i2+4,c2,c3)+c(i,1,4,0)*ui(i1+
     & 1,i2+4,c2,c3)+c(i,2,4,0)*ui(i1+2,i2+4,c2,c3)+c(i,3,4,0)*ui(i1+
     & 3,i2+4,c2,c3)+c(i,4,4,0)*ui(i1+4,i2+4,c2,c3)+c(i,5,4,0)*ui(i1+
     & 5,i2+4,c2,c3)+c(i,6,4,0)*ui(i1+6,i2+4,c2,c3)+c(i,7,4,0)*ui(i1+
     & 7,i2+4,c2,c3)+c(i,0,5,0)*ui(i1  ,i2+5,c2,c3)+c(i,1,5,0)*ui(i1+
     & 1,i2+5,c2,c3)+c(i,2,5,0)*ui(i1+2,i2+5,c2,c3)+c(i,3,5,0)*ui(i1+
     & 3,i2+5,c2,c3)+c(i,4,5,0)*ui(i1+4,i2+5,c2,c3)+c(i,5,5,0)*ui(i1+
     & 5,i2+5,c2,c3)+c(i,6,5,0)*ui(i1+6,i2+5,c2,c3)+c(i,7,5,0)*ui(i1+
     & 7,i2+5,c2,c3)+c(i,0,6,0)*ui(i1  ,i2+6,c2,c3)+c(i,1,6,0)*ui(i1+
     & 1,i2+6,c2,c3)+c(i,2,6,0)*ui(i1+2,i2+6,c2,c3)+c(i,3,6,0)*ui(i1+
     & 3,i2+6,c2,c3)+c(i,4,6,0)*ui(i1+4,i2+6,c2,c3)+c(i,5,6,0)*ui(i1+
     & 5,i2+6,c2,c3)+c(i,6,6,0)*ui(i1+6,i2+6,c2,c3)+c(i,7,6,0)*ui(i1+
     & 7,i2+6,c2,c3)+c(i,0,7,0)*ui(i1  ,i2+7,c2,c3)+c(i,1,7,0)*ui(i1+
     & 1,i2+7,c2,c3)+c(i,2,7,0)*ui(i1+2,i2+7,c2,c3)+c(i,3,7,0)*ui(i1+
     & 3,i2+7,c2,c3)+c(i,4,7,0)*ui(i1+4,i2+7,c2,c3)+c(i,5,7,0)*ui(i1+
     & 5,i2+7,c2,c3)+c(i,6,7,0)*ui(i1+6,i2+7,c2,c3)+c(i,7,7,0)*ui(i1+
     & 7,i2+7,c2,c3)
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i)))
               ug(ip(i,1),ip(i,2),c2,c3)=r(i)
             end do
             end do
             end do
           end if
         else if( width(1).eq.9 .and. width(2).eq.9 )then
! loops2d($interp99(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),ug(ip(i,1),ip(i,2),c2,c3)= r(i))
           if( c2a.eq.c2b .and. c3a.eq.c3b )then
             do c3=c3a,c3b
             do c2=c2a,c2b
             do i=nia,nib
! interp99(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,c3)+c(i,3,0,0)*ui(i1+
     & 3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,c3)+c(i,5,0,0)*ui(i1+
     & 5,i2  ,c2,c3)+c(i,6,0,0)*ui(i1+6,i2  ,c2,c3)+c(i,7,0,0)*ui(i1+
     & 7,i2  ,c2,c3)+c(i,8,0,0)*ui(i1+8,i2  ,c2,c3)+c(i,0,1,0)*ui(i1  
     & ,i2+1,c2,c3)+c(i,1,1,0)*ui(i1+1,i2+1,c2,c3)+c(i,2,1,0)*ui(i1+2,
     & i2+1,c2,c3)+c(i,3,1,0)*ui(i1+3,i2+1,c2,c3)+c(i,4,1,0)*ui(i1+4,
     & i2+1,c2,c3)+c(i,5,1,0)*ui(i1+5,i2+1,c2,c3)+c(i,6,1,0)*ui(i1+6,
     & i2+1,c2,c3)+c(i,7,1,0)*ui(i1+7,i2+1,c2,c3)+c(i,8,1,0)*ui(i1+8,
     & i2+1,c2,c3)+c(i,0,2,0)*ui(i1  ,i2+2,c2,c3)+c(i,1,2,0)*ui(i1+1,
     & i2+2,c2,c3)+c(i,2,2,0)*ui(i1+2,i2+2,c2,c3)+c(i,3,2,0)*ui(i1+3,
     & i2+2,c2,c3)+c(i,4,2,0)*ui(i1+4,i2+2,c2,c3)+c(i,5,2,0)*ui(i1+5,
     & i2+2,c2,c3)+c(i,6,2,0)*ui(i1+6,i2+2,c2,c3)+c(i,7,2,0)*ui(i1+7,
     & i2+2,c2,c3)+c(i,8,2,0)*ui(i1+8,i2+2,c2,c3)+c(i,0,3,0)*ui(i1  ,
     & i2+3,c2,c3)+c(i,1,3,0)*ui(i1+1,i2+3,c2,c3)+c(i,2,3,0)*ui(i1+2,
     & i2+3,c2,c3)+c(i,3,3,0)*ui(i1+3,i2+3,c2,c3)+c(i,4,3,0)*ui(i1+4,
     & i2+3,c2,c3)+c(i,5,3,0)*ui(i1+5,i2+3,c2,c3)+c(i,6,3,0)*ui(i1+6,
     & i2+3,c2,c3)+c(i,7,3,0)*ui(i1+7,i2+3,c2,c3)+c(i,8,3,0)*ui(i1+8,
     & i2+3,c2,c3)+c(i,0,4,0)*ui(i1  ,i2+4,c2,c3)+c(i,1,4,0)*ui(i1+1,
     & i2+4,c2,c3)+c(i,2,4,0)*ui(i1+2,i2+4,c2,c3)+c(i,3,4,0)*ui(i1+3,
     & i2+4,c2,c3)+c(i,4,4,0)*ui(i1+4,i2+4,c2,c3)+c(i,5,4,0)*ui(i1+5,
     & i2+4,c2,c3)+c(i,6,4,0)*ui(i1+6,i2+4,c2,c3)+c(i,7,4,0)*ui(i1+7,
     & i2+4,c2,c3)+c(i,8,4,0)*ui(i1+8,i2+4,c2,c3)+c(i,0,5,0)*ui(i1  ,
     & i2+5,c2,c3)+c(i,1,5,0)*ui(i1+1,i2+5,c2,c3)+c(i,2,5,0)*ui(i1+2,
     & i2+5,c2,c3)+c(i,3,5,0)*ui(i1+3,i2+5,c2,c3)+c(i,4,5,0)*ui(i1+4,
     & i2+5,c2,c3)+c(i,5,5,0)*ui(i1+5,i2+5,c2,c3)+c(i,6,5,0)*ui(i1+6,
     & i2+5,c2,c3)+c(i,7,5,0)*ui(i1+7,i2+5,c2,c3)+c(i,8,5,0)*ui(i1+8,
     & i2+5,c2,c3)+c(i,0,6,0)*ui(i1  ,i2+6,c2,c3)+c(i,1,6,0)*ui(i1+1,
     & i2+6,c2,c3)+c(i,2,6,0)*ui(i1+2,i2+6,c2,c3)+c(i,3,6,0)*ui(i1+3,
     & i2+6,c2,c3)+c(i,4,6,0)*ui(i1+4,i2+6,c2,c3)+c(i,5,6,0)*ui(i1+5,
     & i2+6,c2,c3)+c(i,6,6,0)*ui(i1+6,i2+6,c2,c3)+c(i,7,6,0)*ui(i1+7,
     & i2+6,c2,c3)+c(i,8,6,0)*ui(i1+8,i2+6,c2,c3)+c(i,0,7,0)*ui(i1  ,
     & i2+7,c2,c3)+c(i,1,7,0)*ui(i1+1,i2+7,c2,c3)+c(i,2,7,0)*ui(i1+2,
     & i2+7,c2,c3)+c(i,3,7,0)*ui(i1+3,i2+7,c2,c3)+c(i,4,7,0)*ui(i1+4,
     & i2+7,c2,c3)+c(i,5,7,0)*ui(i1+5,i2+7,c2,c3)+c(i,6,7,0)*ui(i1+6,
     & i2+7,c2,c3)+c(i,7,7,0)*ui(i1+7,i2+7,c2,c3)+c(i,8,7,0)*ui(i1+8,
     & i2+7,c2,c3)+c(i,0,8,0)*ui(i1  ,i2+8,c2,c3)+c(i,1,8,0)*ui(i1+1,
     & i2+8,c2,c3)+c(i,2,8,0)*ui(i1+2,i2+8,c2,c3)+c(i,3,8,0)*ui(i1+3,
     & i2+8,c2,c3)+c(i,4,8,0)*ui(i1+4,i2+8,c2,c3)+c(i,5,8,0)*ui(i1+5,
     & i2+8,c2,c3)+c(i,6,8,0)*ui(i1+6,i2+8,c2,c3)+c(i,7,8,0)*ui(i1+7,
     & i2+8,c2,c3)+c(i,8,8,0)*ui(i1+8,i2+8,c2,c3)
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
! interp99(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = c(i,0,0,0)*ui(i1  ,i2  ,c2,c3)+c(i,1,0,0)*ui(i1+
     & 1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,c3)+c(i,3,0,0)*ui(i1+
     & 3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,c3)+c(i,5,0,0)*ui(i1+
     & 5,i2  ,c2,c3)+c(i,6,0,0)*ui(i1+6,i2  ,c2,c3)+c(i,7,0,0)*ui(i1+
     & 7,i2  ,c2,c3)+c(i,8,0,0)*ui(i1+8,i2  ,c2,c3)+c(i,0,1,0)*ui(i1  
     & ,i2+1,c2,c3)+c(i,1,1,0)*ui(i1+1,i2+1,c2,c3)+c(i,2,1,0)*ui(i1+2,
     & i2+1,c2,c3)+c(i,3,1,0)*ui(i1+3,i2+1,c2,c3)+c(i,4,1,0)*ui(i1+4,
     & i2+1,c2,c3)+c(i,5,1,0)*ui(i1+5,i2+1,c2,c3)+c(i,6,1,0)*ui(i1+6,
     & i2+1,c2,c3)+c(i,7,1,0)*ui(i1+7,i2+1,c2,c3)+c(i,8,1,0)*ui(i1+8,
     & i2+1,c2,c3)+c(i,0,2,0)*ui(i1  ,i2+2,c2,c3)+c(i,1,2,0)*ui(i1+1,
     & i2+2,c2,c3)+c(i,2,2,0)*ui(i1+2,i2+2,c2,c3)+c(i,3,2,0)*ui(i1+3,
     & i2+2,c2,c3)+c(i,4,2,0)*ui(i1+4,i2+2,c2,c3)+c(i,5,2,0)*ui(i1+5,
     & i2+2,c2,c3)+c(i,6,2,0)*ui(i1+6,i2+2,c2,c3)+c(i,7,2,0)*ui(i1+7,
     & i2+2,c2,c3)+c(i,8,2,0)*ui(i1+8,i2+2,c2,c3)+c(i,0,3,0)*ui(i1  ,
     & i2+3,c2,c3)+c(i,1,3,0)*ui(i1+1,i2+3,c2,c3)+c(i,2,3,0)*ui(i1+2,
     & i2+3,c2,c3)+c(i,3,3,0)*ui(i1+3,i2+3,c2,c3)+c(i,4,3,0)*ui(i1+4,
     & i2+3,c2,c3)+c(i,5,3,0)*ui(i1+5,i2+3,c2,c3)+c(i,6,3,0)*ui(i1+6,
     & i2+3,c2,c3)+c(i,7,3,0)*ui(i1+7,i2+3,c2,c3)+c(i,8,3,0)*ui(i1+8,
     & i2+3,c2,c3)+c(i,0,4,0)*ui(i1  ,i2+4,c2,c3)+c(i,1,4,0)*ui(i1+1,
     & i2+4,c2,c3)+c(i,2,4,0)*ui(i1+2,i2+4,c2,c3)+c(i,3,4,0)*ui(i1+3,
     & i2+4,c2,c3)+c(i,4,4,0)*ui(i1+4,i2+4,c2,c3)+c(i,5,4,0)*ui(i1+5,
     & i2+4,c2,c3)+c(i,6,4,0)*ui(i1+6,i2+4,c2,c3)+c(i,7,4,0)*ui(i1+7,
     & i2+4,c2,c3)+c(i,8,4,0)*ui(i1+8,i2+4,c2,c3)+c(i,0,5,0)*ui(i1  ,
     & i2+5,c2,c3)+c(i,1,5,0)*ui(i1+1,i2+5,c2,c3)+c(i,2,5,0)*ui(i1+2,
     & i2+5,c2,c3)+c(i,3,5,0)*ui(i1+3,i2+5,c2,c3)+c(i,4,5,0)*ui(i1+4,
     & i2+5,c2,c3)+c(i,5,5,0)*ui(i1+5,i2+5,c2,c3)+c(i,6,5,0)*ui(i1+6,
     & i2+5,c2,c3)+c(i,7,5,0)*ui(i1+7,i2+5,c2,c3)+c(i,8,5,0)*ui(i1+8,
     & i2+5,c2,c3)+c(i,0,6,0)*ui(i1  ,i2+6,c2,c3)+c(i,1,6,0)*ui(i1+1,
     & i2+6,c2,c3)+c(i,2,6,0)*ui(i1+2,i2+6,c2,c3)+c(i,3,6,0)*ui(i1+3,
     & i2+6,c2,c3)+c(i,4,6,0)*ui(i1+4,i2+6,c2,c3)+c(i,5,6,0)*ui(i1+5,
     & i2+6,c2,c3)+c(i,6,6,0)*ui(i1+6,i2+6,c2,c3)+c(i,7,6,0)*ui(i1+7,
     & i2+6,c2,c3)+c(i,8,6,0)*ui(i1+8,i2+6,c2,c3)+c(i,0,7,0)*ui(i1  ,
     & i2+7,c2,c3)+c(i,1,7,0)*ui(i1+1,i2+7,c2,c3)+c(i,2,7,0)*ui(i1+2,
     & i2+7,c2,c3)+c(i,3,7,0)*ui(i1+3,i2+7,c2,c3)+c(i,4,7,0)*ui(i1+4,
     & i2+7,c2,c3)+c(i,5,7,0)*ui(i1+5,i2+7,c2,c3)+c(i,6,7,0)*ui(i1+6,
     & i2+7,c2,c3)+c(i,7,7,0)*ui(i1+7,i2+7,c2,c3)+c(i,8,7,0)*ui(i1+8,
     & i2+7,c2,c3)+c(i,0,8,0)*ui(i1  ,i2+8,c2,c3)+c(i,1,8,0)*ui(i1+1,
     & i2+8,c2,c3)+c(i,2,8,0)*ui(i1+2,i2+8,c2,c3)+c(i,3,8,0)*ui(i1+3,
     & i2+8,c2,c3)+c(i,4,8,0)*ui(i1+4,i2+8,c2,c3)+c(i,5,8,0)*ui(i1+5,
     & i2+8,c2,c3)+c(i,6,8,0)*ui(i1+6,i2+8,c2,c3)+c(i,7,8,0)*ui(i1+7,
     & i2+8,c2,c3)+c(i,8,8,0)*ui(i1+8,i2+8,c2,c3)
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i)))
               ug(ip(i,1),ip(i,2),c2,c3)=r(i)
             end do
             end do
             end do
           end if
         else
c           general case in 2D
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
c     *** 3D ****
         if( useVariableWidthInterpolation.ne.0 )then
! beginLoops3d()
             do i=nia,nib
             do c3=c3a,c3b
             ! check for most common widths first
             if( varWidth(i).eq.3 )then
! interp333(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,0,0)*ui(i1,i2,i3,c3)+c(i,1,0,0)*ui(i1+1,i2,
     & i3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3,c3)+c(i,0,1,0)*ui(i1,i2+1,i3,
     & c3)+c(i,1,1,0)*ui(i1+1,i2+1,i3,c3)+c(i,2,1,0)*ui(i1+2,i2+1,i3,
     & c3)+c(i,0,2,0)*ui(i1,i2+2,i3,c3)+c(i,1,2,0)*ui(i1+1,i2+2,i3,c3)
     & +c(i,2,2,0)*ui(i1+2,i2+2,i3,c3)+c(i,0,0,1)*ui(i1,i2,i3+1,c3)+c(
     & i,1,0,1)*ui(i1+1,i2,i3+1,c3)+c(i,2,0,1)*ui(i1+2,i2,i3+1,c3)+c(
     & i,0,1,1)*ui(i1,i2+1,i3+1,c3)+c(i,1,1,1)*ui(i1+1,i2+1,i3+1,c3)+
     & c(i,2,1,1)*ui(i1+2,i2+1,i3+1,c3)+c(i,0,2,1)*ui(i1,i2+2,i3+1,c3)
     & +c(i,1,2,1)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,2,1)*ui(i1+2,i2+2,i3+1,
     & c3)+c(i,0,0,2)*ui(i1,i2,i3+2,c3)+c(i,1,0,2)*ui(i1+1,i2,i3+2,c3)
     & +c(i,2,0,2)*ui(i1+2,i2,i3+2,c3)+c(i,0,1,2)*ui(i1,i2+1,i3+2,c3)+
     & c(i,1,1,2)*ui(i1+1,i2+1,i3+2,c3)+c(i,2,1,2)*ui(i1+2,i2+1,i3+2,
     & c3)+c(i,0,2,2)*ui(i1,i2+2,i3+2,c3)+c(i,1,2,2)*ui(i1+1,i2+2,i3+
     & 2,c3)+c(i,2,2,2)*ui(i1+2,i2+2,i3+2,c3)
             else if( varWidth(i).eq.2 )then
! interp222(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,0,0)*ui(i1,i2,i3,c3)+c(i,1,0,0)*ui(i1+1,i2,
     & i3,c3)+c(i,0,1,0)*ui(i1,i2+1,i3,c3)+c(i,1,1,0)*ui(i1+1,i2+1,i3,
     & c3)+c(i,0,0,1)*ui(i1,i2,i3+1,c3)+c(i,1,0,1)*ui(i1+1,i2,i3+1,c3)
     & +c(i,0,1,1)*ui(i1,i2+1,i3+1,c3)+c(i,1,1,1)*ui(i1+1,i2+1,i3+1,
     & c3)
             else if( varWidth(i).eq.1 )then
! interp111(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = ui(i1,i2,i3,c3)
             else if( varWidth(i).eq.5 )then
! interp555(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,0,0)*ui(i1,i2,i3,c3)+c(i,1,0,0)*ui(i1+1,i2,
     & i3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3,c3)+c(i,3,0,0)*ui(i1+3,i2,i3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2,i3,c3)+c(i,0,1,0)*ui(i1,i2+1,i3,c3)+
     & c(i,1,1,0)*ui(i1+1,i2+1,i3,c3)+c(i,2,1,0)*ui(i1+2,i2+1,i3,c3)+
     & c(i,3,1,0)*ui(i1+3,i2+1,i3,c3)+c(i,4,1,0)*ui(i1+4,i2+1,i3,c3)+
     & c(i,0,2,0)*ui(i1,i2+2,i3,c3)+c(i,1,2,0)*ui(i1+1,i2+2,i3,c3)+c(
     & i,2,2,0)*ui(i1+2,i2+2,i3,c3)+c(i,3,2,0)*ui(i1+3,i2+2,i3,c3)+c(
     & i,4,2,0)*ui(i1+4,i2+2,i3,c3)+c(i,0,3,0)*ui(i1,i2+3,i3,c3)+c(i,
     & 1,3,0)*ui(i1+1,i2+3,i3,c3)+c(i,2,3,0)*ui(i1+2,i2+3,i3,c3)+c(i,
     & 3,3,0)*ui(i1+3,i2+3,i3,c3)+c(i,4,3,0)*ui(i1+4,i2+3,i3,c3)+c(i,
     & 0,4,0)*ui(i1,i2+4,i3,c3)+c(i,1,4,0)*ui(i1+1,i2+4,i3,c3)+c(i,2,
     & 4,0)*ui(i1+2,i2+4,i3,c3)+c(i,3,4,0)*ui(i1+3,i2+4,i3,c3)+c(i,4,
     & 4,0)*ui(i1+4,i2+4,i3,c3)+c(i,0,0,1)*ui(i1,i2,i3+1,c3)+c(i,1,0,
     & 1)*ui(i1+1,i2,i3+1,c3)+c(i,2,0,1)*ui(i1+2,i2,i3+1,c3)+c(i,3,0,
     & 1)*ui(i1+3,i2,i3+1,c3)+c(i,4,0,1)*ui(i1+4,i2,i3+1,c3)+c(i,0,1,
     & 1)*ui(i1,i2+1,i3+1,c3)+c(i,1,1,1)*ui(i1+1,i2+1,i3+1,c3)+c(i,2,
     & 1,1)*ui(i1+2,i2+1,i3+1,c3)+c(i,3,1,1)*ui(i1+3,i2+1,i3+1,c3)+c(
     & i,4,1,1)*ui(i1+4,i2+1,i3+1,c3)+c(i,0,2,1)*ui(i1,i2+2,i3+1,c3)+
     & c(i,1,2,1)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,2,1)*ui(i1+2,i2+2,i3+1,
     & c3)+c(i,3,2,1)*ui(i1+3,i2+2,i3+1,c3)+c(i,4,2,1)*ui(i1+4,i2+2,
     & i3+1,c3)+c(i,0,3,1)*ui(i1,i2+3,i3+1,c3)+c(i,1,3,1)*ui(i1+1,i2+
     & 3,i3+1,c3)+c(i,2,3,1)*ui(i1+2,i2+3,i3+1,c3)+c(i,3,3,1)*ui(i1+3,
     & i2+3,i3+1,c3)+c(i,4,3,1)*ui(i1+4,i2+3,i3+1,c3)+c(i,0,4,1)*ui(
     & i1,i2+4,i3+1,c3)+c(i,1,4,1)*ui(i1+1,i2+4,i3+1,c3)+c(i,2,4,1)*
     & ui(i1+2,i2+4,i3+1,c3)+c(i,3,4,1)*ui(i1+3,i2+4,i3+1,c3)+c(i,4,4,
     & 1)*ui(i1+4,i2+4,i3+1,c3)+c(i,0,0,2)*ui(i1,i2,i3+2,c3)+c(i,1,0,
     & 2)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,2)*ui(i1+2,i2,i3+2,c3)+c(i,3,0,
     & 2)*ui(i1+3,i2,i3+2,c3)+c(i,4,0,2)*ui(i1+4,i2,i3+2,c3)+c(i,0,1,
     & 2)*ui(i1,i2+1,i3+2,c3)+c(i,1,1,2)*ui(i1+1,i2+1,i3+2,c3)+c(i,2,
     & 1,2)*ui(i1+2,i2+1,i3+2,c3)+c(i,3,1,2)*ui(i1+3,i2+1,i3+2,c3)+c(
     & i,4,1,2)*ui(i1+4,i2+1,i3+2,c3)+c(i,0,2,2)*ui(i1,i2+2,i3+2,c3)+
     & c(i,1,2,2)*ui(i1+1,i2+2,i3+2,c3)+c(i,2,2,2)*ui(i1+2,i2+2,i3+2,
     & c3)+c(i,3,2,2)*ui(i1+3,i2+2,i3+2,c3)+c(i,4,2,2)*ui(i1+4,i2+2,
     & i3+2,c3)+c(i,0,3,2)*ui(i1,i2+3,i3+2,c3)+c(i,1,3,2)*ui(i1+1,i2+
     & 3,i3+2,c3)+c(i,2,3,2)*ui(i1+2,i2+3,i3+2,c3)+c(i,3,3,2)*ui(i1+3,
     & i2+3,i3+2,c3)+c(i,4,3,2)*ui(i1+4,i2+3,i3+2,c3)+c(i,0,4,2)*ui(
     & i1,i2+4,i3+2,c3)+c(i,1,4,2)*ui(i1+1,i2+4,i3+2,c3)+c(i,2,4,2)*
     & ui(i1+2,i2+4,i3+2,c3)+c(i,3,4,2)*ui(i1+3,i2+4,i3+2,c3)+c(i,4,4,
     & 2)*ui(i1+4,i2+4,i3+2,c3)+c(i,0,0,3)*ui(i1,i2,i3+3,c3)+c(i,1,0,
     & 3)*ui(i1+1,i2,i3+3,c3)+c(i,2,0,3)*ui(i1+2,i2,i3+3,c3)+c(i,3,0,
     & 3)*ui(i1+3,i2,i3+3,c3)+c(i,4,0,3)*ui(i1+4,i2,i3+3,c3)+c(i,0,1,
     & 3)*ui(i1,i2+1,i3+3,c3)+c(i,1,1,3)*ui(i1+1,i2+1,i3+3,c3)+c(i,2,
     & 1,3)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,1,3)*ui(i1+3,i2+1,i3+3,c3)+c(
     & i,4,1,3)*ui(i1+4,i2+1,i3+3,c3)+c(i,0,2,3)*ui(i1,i2+2,i3+3,c3)+
     & c(i,1,2,3)*ui(i1+1,i2+2,i3+3,c3)+c(i,2,2,3)*ui(i1+2,i2+2,i3+3,
     & c3)+c(i,3,2,3)*ui(i1+3,i2+2,i3+3,c3)+c(i,4,2,3)*ui(i1+4,i2+2,
     & i3+3,c3)+c(i,0,3,3)*ui(i1,i2+3,i3+3,c3)+c(i,1,3,3)*ui(i1+1,i2+
     & 3,i3+3,c3)+c(i,2,3,3)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,3,3)*ui(i1+3,
     & i2+3,i3+3,c3)+c(i,4,3,3)*ui(i1+4,i2+3,i3+3,c3)+c(i,0,4,3)*ui(
     & i1,i2+4,i3+3,c3)+c(i,1,4,3)*ui(i1+1,i2+4,i3+3,c3)+c(i,2,4,3)*
     & ui(i1+2,i2+4,i3+3,c3)+c(i,3,4,3)*ui(i1+3,i2+4,i3+3,c3)+c(i,4,4,
     & 3)*ui(i1+4,i2+4,i3+3,c3)+c(i,0,0,4)*ui(i1,i2,i3+4,c3)+c(i,1,0,
     & 4)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,4)*ui(i1+2,i2,i3+4,c3)+c(i,3,0,
     & 4)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,4)*ui(i1+4,i2,i3+4,c3)+c(i,0,1,
     & 4)*ui(i1,i2+1,i3+4,c3)+c(i,1,1,4)*ui(i1+1,i2+1,i3+4,c3)+c(i,2,
     & 1,4)*ui(i1+2,i2+1,i3+4,c3)+c(i,3,1,4)*ui(i1+3,i2+1,i3+4,c3)+c(
     & i,4,1,4)*ui(i1+4,i2+1,i3+4,c3)+c(i,0,2,4)*ui(i1,i2+2,i3+4,c3)+
     & c(i,1,2,4)*ui(i1+1,i2+2,i3+4,c3)+c(i,2,2,4)*ui(i1+2,i2+2,i3+4,
     & c3)+c(i,3,2,4)*ui(i1+3,i2+2,i3+4,c3)+c(i,4,2,4)*ui(i1+4,i2+2,
     & i3+4,c3)+c(i,0,3,4)*ui(i1,i2+3,i3+4,c3)+c(i,1,3,4)*ui(i1+1,i2+
     & 3,i3+4,c3)+c(i,2,3,4)*ui(i1+2,i2+3,i3+4,c3)+c(i,3,3,4)*ui(i1+3,
     & i2+3,i3+4,c3)+c(i,4,3,4)*ui(i1+4,i2+3,i3+4,c3)+c(i,0,4,4)*ui(
     & i1,i2+4,i3+4,c3)+c(i,1,4,4)*ui(i1+1,i2+4,i3+4,c3)+c(i,2,4,4)*
     & ui(i1+2,i2+4,i3+4,c3)+c(i,3,4,4)*ui(i1+3,i2+4,i3+4,c3)+c(i,4,4,
     & 4)*ui(i1+4,i2+4,i3+4,c3)
             else if( varWidth(i).eq.4 )then
! interp444(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,0,0)*ui(i1,i2,i3,c3)+c(i,1,0,0)*ui(i1+1,i2,
     & i3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3,c3)+c(i,3,0,0)*ui(i1+3,i2,i3,
     & c3)+c(i,0,1,0)*ui(i1,i2+1,i3,c3)+c(i,1,1,0)*ui(i1+1,i2+1,i3,c3)
     & +c(i,2,1,0)*ui(i1+2,i2+1,i3,c3)+c(i,3,1,0)*ui(i1+3,i2+1,i3,c3)+
     & c(i,0,2,0)*ui(i1,i2+2,i3,c3)+c(i,1,2,0)*ui(i1+1,i2+2,i3,c3)+c(
     & i,2,2,0)*ui(i1+2,i2+2,i3,c3)+c(i,3,2,0)*ui(i1+3,i2+2,i3,c3)+c(
     & i,0,3,0)*ui(i1,i2+3,i3,c3)+c(i,1,3,0)*ui(i1+1,i2+3,i3,c3)+c(i,
     & 2,3,0)*ui(i1+2,i2+3,i3,c3)+c(i,3,3,0)*ui(i1+3,i2+3,i3,c3)+c(i,
     & 0,0,1)*ui(i1,i2,i3+1,c3)+c(i,1,0,1)*ui(i1+1,i2,i3+1,c3)+c(i,2,
     & 0,1)*ui(i1+2,i2,i3+1,c3)+c(i,3,0,1)*ui(i1+3,i2,i3+1,c3)+c(i,0,
     & 1,1)*ui(i1,i2+1,i3+1,c3)+c(i,1,1,1)*ui(i1+1,i2+1,i3+1,c3)+c(i,
     & 2,1,1)*ui(i1+2,i2+1,i3+1,c3)+c(i,3,1,1)*ui(i1+3,i2+1,i3+1,c3)+
     & c(i,0,2,1)*ui(i1,i2+2,i3+1,c3)+c(i,1,2,1)*ui(i1+1,i2+2,i3+1,c3)
     & +c(i,2,2,1)*ui(i1+2,i2+2,i3+1,c3)+c(i,3,2,1)*ui(i1+3,i2+2,i3+1,
     & c3)+c(i,0,3,1)*ui(i1,i2+3,i3+1,c3)+c(i,1,3,1)*ui(i1+1,i2+3,i3+
     & 1,c3)+c(i,2,3,1)*ui(i1+2,i2+3,i3+1,c3)+c(i,3,3,1)*ui(i1+3,i2+3,
     & i3+1,c3)+c(i,0,0,2)*ui(i1,i2,i3+2,c3)+c(i,1,0,2)*ui(i1+1,i2,i3+
     & 2,c3)+c(i,2,0,2)*ui(i1+2,i2,i3+2,c3)+c(i,3,0,2)*ui(i1+3,i2,i3+
     & 2,c3)+c(i,0,1,2)*ui(i1,i2+1,i3+2,c3)+c(i,1,1,2)*ui(i1+1,i2+1,
     & i3+2,c3)+c(i,2,1,2)*ui(i1+2,i2+1,i3+2,c3)+c(i,3,1,2)*ui(i1+3,
     & i2+1,i3+2,c3)+c(i,0,2,2)*ui(i1,i2+2,i3+2,c3)+c(i,1,2,2)*ui(i1+
     & 1,i2+2,i3+2,c3)+c(i,2,2,2)*ui(i1+2,i2+2,i3+2,c3)+c(i,3,2,2)*ui(
     & i1+3,i2+2,i3+2,c3)+c(i,0,3,2)*ui(i1,i2+3,i3+2,c3)+c(i,1,3,2)*
     & ui(i1+1,i2+3,i3+2,c3)+c(i,2,3,2)*ui(i1+2,i2+3,i3+2,c3)+c(i,3,3,
     & 2)*ui(i1+3,i2+3,i3+2,c3)+c(i,0,0,3)*ui(i1,i2,i3+3,c3)+c(i,1,0,
     & 3)*ui(i1+1,i2,i3+3,c3)+c(i,2,0,3)*ui(i1+2,i2,i3+3,c3)+c(i,3,0,
     & 3)*ui(i1+3,i2,i3+3,c3)+c(i,0,1,3)*ui(i1,i2+1,i3+3,c3)+c(i,1,1,
     & 3)*ui(i1+1,i2+1,i3+3,c3)+c(i,2,1,3)*ui(i1+2,i2+1,i3+3,c3)+c(i,
     & 3,1,3)*ui(i1+3,i2+1,i3+3,c3)+c(i,0,2,3)*ui(i1,i2+2,i3+3,c3)+c(
     & i,1,2,3)*ui(i1+1,i2+2,i3+3,c3)+c(i,2,2,3)*ui(i1+2,i2+2,i3+3,c3)
     & +c(i,3,2,3)*ui(i1+3,i2+2,i3+3,c3)+c(i,0,3,3)*ui(i1,i2+3,i3+3,
     & c3)+c(i,1,3,3)*ui(i1+1,i2+3,i3+3,c3)+c(i,2,3,3)*ui(i1+2,i2+3,
     & i3+3,c3)+c(i,3,3,3)*ui(i1+3,i2+3,i3+3,c3)
             else if( varWidth(i).eq.7 )then
! interp777(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,0,0)*ui(i1,i2,i3,c3)+c(i,1,0,0)*ui(i1+1,i2,
     & i3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3,c3)+c(i,3,0,0)*ui(i1+3,i2,i3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2,i3,c3)+c(i,5,0,0)*ui(i1+5,i2,i3,c3)+
     & c(i,6,0,0)*ui(i1+6,i2,i3,c3)+c(i,0,1,0)*ui(i1,i2+1,i3,c3)+c(i,
     & 1,1,0)*ui(i1+1,i2+1,i3,c3)+c(i,2,1,0)*ui(i1+2,i2+1,i3,c3)+c(i,
     & 3,1,0)*ui(i1+3,i2+1,i3,c3)+c(i,4,1,0)*ui(i1+4,i2+1,i3,c3)+c(i,
     & 5,1,0)*ui(i1+5,i2+1,i3,c3)+c(i,6,1,0)*ui(i1+6,i2+1,i3,c3)+c(i,
     & 0,2,0)*ui(i1,i2+2,i3,c3)+c(i,1,2,0)*ui(i1+1,i2+2,i3,c3)+c(i,2,
     & 2,0)*ui(i1+2,i2+2,i3,c3)+c(i,3,2,0)*ui(i1+3,i2+2,i3,c3)+c(i,4,
     & 2,0)*ui(i1+4,i2+2,i3,c3)+c(i,5,2,0)*ui(i1+5,i2+2,i3,c3)+c(i,6,
     & 2,0)*ui(i1+6,i2+2,i3,c3)+c(i,0,3,0)*ui(i1,i2+3,i3,c3)+c(i,1,3,
     & 0)*ui(i1+1,i2+3,i3,c3)+c(i,2,3,0)*ui(i1+2,i2+3,i3,c3)+c(i,3,3,
     & 0)*ui(i1+3,i2+3,i3,c3)+c(i,4,3,0)*ui(i1+4,i2+3,i3,c3)+c(i,5,3,
     & 0)*ui(i1+5,i2+3,i3,c3)+c(i,6,3,0)*ui(i1+6,i2+3,i3,c3)+c(i,0,4,
     & 0)*ui(i1,i2+4,i3,c3)+c(i,1,4,0)*ui(i1+1,i2+4,i3,c3)+c(i,2,4,0)*
     & ui(i1+2,i2+4,i3,c3)+c(i,3,4,0)*ui(i1+3,i2+4,i3,c3)+c(i,4,4,0)*
     & ui(i1+4,i2+4,i3,c3)+c(i,5,4,0)*ui(i1+5,i2+4,i3,c3)+c(i,6,4,0)*
     & ui(i1+6,i2+4,i3,c3)+c(i,0,5,0)*ui(i1,i2+5,i3,c3)+c(i,1,5,0)*ui(
     & i1+1,i2+5,i3,c3)+c(i,2,5,0)*ui(i1+2,i2+5,i3,c3)+c(i,3,5,0)*ui(
     & i1+3,i2+5,i3,c3)+c(i,4,5,0)*ui(i1+4,i2+5,i3,c3)+c(i,5,5,0)*ui(
     & i1+5,i2+5,i3,c3)+c(i,6,5,0)*ui(i1+6,i2+5,i3,c3)+c(i,0,6,0)*ui(
     & i1,i2+6,i3,c3)+c(i,1,6,0)*ui(i1+1,i2+6,i3,c3)+c(i,2,6,0)*ui(i1+
     & 2,i2+6,i3,c3)+c(i,3,6,0)*ui(i1+3,i2+6,i3,c3)+c(i,4,6,0)*ui(i1+
     & 4,i2+6,i3,c3)+c(i,5,6,0)*ui(i1+5,i2+6,i3,c3)+c(i,6,6,0)*ui(i1+
     & 6,i2+6,i3,c3)+c(i,0,0,1)*ui(i1,i2,i3+1,c3)+c(i,1,0,1)*ui(i1+1,
     & i2,i3+1,c3)+c(i,2,0,1)*ui(i1+2,i2,i3+1,c3)+c(i,3,0,1)*ui(i1+3,
     & i2,i3+1,c3)+c(i,4,0,1)*ui(i1+4,i2,i3+1,c3)+c(i,5,0,1)*ui(i1+5,
     & i2,i3+1,c3)+c(i,6,0,1)*ui(i1+6,i2,i3+1,c3)+c(i,0,1,1)*ui(i1,i2+
     & 1,i3+1,c3)+c(i,1,1,1)*ui(i1+1,i2+1,i3+1,c3)+c(i,2,1,1)*ui(i1+2,
     & i2+1,i3+1,c3)+c(i,3,1,1)*ui(i1+3,i2+1,i3+1,c3)+c(i,4,1,1)*ui(
     & i1+4,i2+1,i3+1,c3)+c(i,5,1,1)*ui(i1+5,i2+1,i3+1,c3)+c(i,6,1,1)*
     & ui(i1+6,i2+1,i3+1,c3)+c(i,0,2,1)*ui(i1,i2+2,i3+1,c3)+c(i,1,2,1)
     & *ui(i1+1,i2+2,i3+1,c3)+c(i,2,2,1)*ui(i1+2,i2+2,i3+1,c3)+c(i,3,
     & 2,1)*ui(i1+3,i2+2,i3+1,c3)+c(i,4,2,1)*ui(i1+4,i2+2,i3+1,c3)+c(
     & i,5,2,1)*ui(i1+5,i2+2,i3+1,c3)+c(i,6,2,1)*ui(i1+6,i2+2,i3+1,c3)
     & +c(i,0,3,1)*ui(i1,i2+3,i3+1,c3)+c(i,1,3,1)*ui(i1+1,i2+3,i3+1,
     & c3)+c(i,2,3,1)*ui(i1+2,i2+3,i3+1,c3)+c(i,3,3,1)*ui(i1+3,i2+3,
     & i3+1,c3)+c(i,4,3,1)*ui(i1+4,i2+3,i3+1,c3)+c(i,5,3,1)*ui(i1+5,
     & i2+3,i3+1,c3)+c(i,6,3,1)*ui(i1+6,i2+3,i3+1,c3)+c(i,0,4,1)*ui(
     & i1,i2+4,i3+1,c3)+c(i,1,4,1)*ui(i1+1,i2+4,i3+1,c3)+c(i,2,4,1)*
     & ui(i1+2,i2+4,i3+1,c3)+c(i,3,4,1)*ui(i1+3,i2+4,i3+1,c3)+c(i,4,4,
     & 1)*ui(i1+4,i2+4,i3+1,c3)+c(i,5,4,1)*ui(i1+5,i2+4,i3+1,c3)+c(i,
     & 6,4,1)*ui(i1+6,i2+4,i3+1,c3)+c(i,0,5,1)*ui(i1,i2+5,i3+1,c3)+c(
     & i,1,5,1)*ui(i1+1,i2+5,i3+1,c3)+c(i,2,5,1)*ui(i1+2,i2+5,i3+1,c3)
     & +c(i,3,5,1)*ui(i1+3,i2+5,i3+1,c3)+c(i,4,5,1)*ui(i1+4,i2+5,i3+1,
     & c3)+c(i,5,5,1)*ui(i1+5,i2+5,i3+1,c3)+c(i,6,5,1)*ui(i1+6,i2+5,
     & i3+1,c3)+c(i,0,6,1)*ui(i1,i2+6,i3+1,c3)+c(i,1,6,1)*ui(i1+1,i2+
     & 6,i3+1,c3)+c(i,2,6,1)*ui(i1+2,i2+6,i3+1,c3)+c(i,3,6,1)*ui(i1+3,
     & i2+6,i3+1,c3)+c(i,4,6,1)*ui(i1+4,i2+6,i3+1,c3)+c(i,5,6,1)*ui(
     & i1+5,i2+6,i3+1,c3)+c(i,6,6,1)*ui(i1+6,i2+6,i3+1,c3)+c(i,0,0,2)*
     & ui(i1,i2,i3+2,c3)+c(i,1,0,2)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,2)*ui(
     & i1+2,i2,i3+2,c3)+c(i,3,0,2)*ui(i1+3,i2,i3+2,c3)+c(i,4,0,2)*ui(
     & i1+4,i2,i3+2,c3)+c(i,5,0,2)*ui(i1+5,i2,i3+2,c3)+c(i,6,0,2)*ui(
     & i1+6,i2,i3+2,c3)+c(i,0,1,2)*ui(i1,i2+1,i3+2,c3)+c(i,1,1,2)*ui(
     & i1+1,i2+1,i3+2,c3)+c(i,2,1,2)*ui(i1+2,i2+1,i3+2,c3)+c(i,3,1,2)*
     & ui(i1+3,i2+1,i3+2,c3)+c(i,4,1,2)*ui(i1+4,i2+1,i3+2,c3)+c(i,5,1,
     & 2)*ui(i1+5,i2+1,i3+2,c3)+c(i,6,1,2)*ui(i1+6,i2+1,i3+2,c3)+c(i,
     & 0,2,2)*ui(i1,i2+2,i3+2,c3)+c(i,1,2,2)*ui(i1+1,i2+2,i3+2,c3)+c(
     & i,2,2,2)*ui(i1+2,i2+2,i3+2,c3)+c(i,3,2,2)*ui(i1+3,i2+2,i3+2,c3)
     & +c(i,4,2,2)*ui(i1+4,i2+2,i3+2,c3)+c(i,5,2,2)*ui(i1+5,i2+2,i3+2,
     & c3)+c(i,6,2,2)*ui(i1+6,i2+2,i3+2,c3)+c(i,0,3,2)*ui(i1,i2+3,i3+
     & 2,c3)+c(i,1,3,2)*ui(i1+1,i2+3,i3+2,c3)+c(i,2,3,2)*ui(i1+2,i2+3,
     & i3+2,c3)+c(i,3,3,2)*ui(i1+3,i2+3,i3+2,c3)+c(i,4,3,2)*ui(i1+4,
     & i2+3,i3+2,c3)+c(i,5,3,2)*ui(i1+5,i2+3,i3+2,c3)+c(i,6,3,2)*ui(
     & i1+6,i2+3,i3+2,c3)+c(i,0,4,2)*ui(i1,i2+4,i3+2,c3)+c(i,1,4,2)*
     & ui(i1+1,i2+4,i3+2,c3)+c(i,2,4,2)*ui(i1+2,i2+4,i3+2,c3)+c(i,3,4,
     & 2)*ui(i1+3,i2+4,i3+2,c3)+c(i,4,4,2)*ui(i1+4,i2+4,i3+2,c3)+c(i,
     & 5,4,2)*ui(i1+5,i2+4,i3+2,c3)+c(i,6,4,2)*ui(i1+6,i2+4,i3+2,c3)+
     & c(i,0,5,2)*ui(i1,i2+5,i3+2,c3)+c(i,1,5,2)*ui(i1+1,i2+5,i3+2,c3)
     & +c(i,2,5,2)*ui(i1+2,i2+5,i3+2,c3)+c(i,3,5,2)*ui(i1+3,i2+5,i3+2,
     & c3)+c(i,4,5,2)*ui(i1+4,i2+5,i3+2,c3)+c(i,5,5,2)*ui(i1+5,i2+5,
     & i3+2,c3)+c(i,6,5,2)*ui(i1+6,i2+5,i3+2,c3)+c(i,0,6,2)*ui(i1,i2+
     & 6,i3+2,c3)+c(i,1,6,2)*ui(i1+1,i2+6,i3+2,c3)+c(i,2,6,2)*ui(i1+2,
     & i2+6,i3+2,c3)+c(i,3,6,2)*ui(i1+3,i2+6,i3+2,c3)+c(i,4,6,2)*ui(
     & i1+4,i2+6,i3+2,c3)+c(i,5,6,2)*ui(i1+5,i2+6,i3+2,c3)+c(i,6,6,2)*
     & ui(i1+6,i2+6,i3+2,c3)+c(i,0,0,3)*ui(i1,i2,i3+3,c3)+c(i,1,0,3)*
     & ui(i1+1,i2,i3+3,c3)+c(i,2,0,3)*ui(i1+2,i2,i3+3,c3)+c(i,3,0,3)*
     & ui(i1+3,i2,i3+3,c3)+c(i,4,0,3)*ui(i1+4,i2,i3+3,c3)+c(i,5,0,3)*
     & ui(i1+5,i2,i3+3,c3)+c(i,6,0,3)*ui(i1+6,i2,i3+3,c3)+c(i,0,1,3)*
     & ui(i1,i2+1,i3+3,c3)+c(i,1,1,3)*ui(i1+1,i2+1,i3+3,c3)+c(i,2,1,3)
     & *ui(i1+2,i2+1,i3+3,c3)+c(i,3,1,3)*ui(i1+3,i2+1,i3+3,c3)+c(i,4,
     & 1,3)*ui(i1+4,i2+1,i3+3,c3)+c(i,5,1,3)*ui(i1+5,i2+1,i3+3,c3)+c(
     & i,6,1,3)*ui(i1+6,i2+1,i3+3,c3)+c(i,0,2,3)*ui(i1,i2+2,i3+3,c3)+
     & c(i,1,2,3)*ui(i1+1,i2+2,i3+3,c3)+c(i,2,2,3)*ui(i1+2,i2+2,i3+3,
     & c3)+c(i,3,2,3)*ui(i1+3,i2+2,i3+3,c3)+c(i,4,2,3)*ui(i1+4,i2+2,
     & i3+3,c3)+c(i,5,2,3)*ui(i1+5,i2+2,i3+3,c3)+c(i,6,2,3)*ui(i1+6,
     & i2+2,i3+3,c3)+c(i,0,3,3)*ui(i1,i2+3,i3+3,c3)+c(i,1,3,3)*ui(i1+
     & 1,i2+3,i3+3,c3)+c(i,2,3,3)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,3,3)*ui(
     & i1+3,i2+3,i3+3,c3)+c(i,4,3,3)*ui(i1+4,i2+3,i3+3,c3)+c(i,5,3,3)*
     & ui(i1+5,i2+3,i3+3,c3)+c(i,6,3,3)*ui(i1+6,i2+3,i3+3,c3)+c(i,0,4,
     & 3)*ui(i1,i2+4,i3+3,c3)+c(i,1,4,3)*ui(i1+1,i2+4,i3+3,c3)+c(i,2,
     & 4,3)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,4,3)*ui(i1+3,i2+4,i3+3,c3)+c(
     & i,4,4,3)*ui(i1+4,i2+4,i3+3,c3)+c(i,5,4,3)*ui(i1+5,i2+4,i3+3,c3)
     & +c(i,6,4,3)*ui(i1+6,i2+4,i3+3,c3)+c(i,0,5,3)*ui(i1,i2+5,i3+3,
     & c3)+c(i,1,5,3)*ui(i1+1,i2+5,i3+3,c3)
               r(i) = r(i)+c(i,2,5,3)*ui(i1+2,i2+5,i3+3,c3)+c(i,3,5,3)*
     & ui(i1+3,i2+5,i3+3,c3)+c(i,4,5,3)*ui(i1+4,i2+5,i3+3,c3)+c(i,5,5,
     & 3)*ui(i1+5,i2+5,i3+3,c3)+c(i,6,5,3)*ui(i1+6,i2+5,i3+3,c3)+c(i,
     & 0,6,3)*ui(i1,i2+6,i3+3,c3)+c(i,1,6,3)*ui(i1+1,i2+6,i3+3,c3)+c(
     & i,2,6,3)*ui(i1+2,i2+6,i3+3,c3)+c(i,3,6,3)*ui(i1+3,i2+6,i3+3,c3)
     & +c(i,4,6,3)*ui(i1+4,i2+6,i3+3,c3)+c(i,5,6,3)*ui(i1+5,i2+6,i3+3,
     & c3)+c(i,6,6,3)*ui(i1+6,i2+6,i3+3,c3)+c(i,0,0,4)*ui(i1,i2,i3+4,
     & c3)+c(i,1,0,4)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,4)*ui(i1+2,i2,i3+4,
     & c3)+c(i,3,0,4)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,4)*ui(i1+4,i2,i3+4,
     & c3)+c(i,5,0,4)*ui(i1+5,i2,i3+4,c3)+c(i,6,0,4)*ui(i1+6,i2,i3+4,
     & c3)+c(i,0,1,4)*ui(i1,i2+1,i3+4,c3)+c(i,1,1,4)*ui(i1+1,i2+1,i3+
     & 4,c3)+c(i,2,1,4)*ui(i1+2,i2+1,i3+4,c3)+c(i,3,1,4)*ui(i1+3,i2+1,
     & i3+4,c3)+c(i,4,1,4)*ui(i1+4,i2+1,i3+4,c3)+c(i,5,1,4)*ui(i1+5,
     & i2+1,i3+4,c3)+c(i,6,1,4)*ui(i1+6,i2+1,i3+4,c3)+c(i,0,2,4)*ui(
     & i1,i2+2,i3+4,c3)+c(i,1,2,4)*ui(i1+1,i2+2,i3+4,c3)+c(i,2,2,4)*
     & ui(i1+2,i2+2,i3+4,c3)+c(i,3,2,4)*ui(i1+3,i2+2,i3+4,c3)+c(i,4,2,
     & 4)*ui(i1+4,i2+2,i3+4,c3)+c(i,5,2,4)*ui(i1+5,i2+2,i3+4,c3)+c(i,
     & 6,2,4)*ui(i1+6,i2+2,i3+4,c3)+c(i,0,3,4)*ui(i1,i2+3,i3+4,c3)+c(
     & i,1,3,4)*ui(i1+1,i2+3,i3+4,c3)+c(i,2,3,4)*ui(i1+2,i2+3,i3+4,c3)
     & +c(i,3,3,4)*ui(i1+3,i2+3,i3+4,c3)+c(i,4,3,4)*ui(i1+4,i2+3,i3+4,
     & c3)+c(i,5,3,4)*ui(i1+5,i2+3,i3+4,c3)+c(i,6,3,4)*ui(i1+6,i2+3,
     & i3+4,c3)+c(i,0,4,4)*ui(i1,i2+4,i3+4,c3)+c(i,1,4,4)*ui(i1+1,i2+
     & 4,i3+4,c3)+c(i,2,4,4)*ui(i1+2,i2+4,i3+4,c3)+c(i,3,4,4)*ui(i1+3,
     & i2+4,i3+4,c3)+c(i,4,4,4)*ui(i1+4,i2+4,i3+4,c3)+c(i,5,4,4)*ui(
     & i1+5,i2+4,i3+4,c3)+c(i,6,4,4)*ui(i1+6,i2+4,i3+4,c3)+c(i,0,5,4)*
     & ui(i1,i2+5,i3+4,c3)+c(i,1,5,4)*ui(i1+1,i2+5,i3+4,c3)+c(i,2,5,4)
     & *ui(i1+2,i2+5,i3+4,c3)+c(i,3,5,4)*ui(i1+3,i2+5,i3+4,c3)+c(i,4,
     & 5,4)*ui(i1+4,i2+5,i3+4,c3)+c(i,5,5,4)*ui(i1+5,i2+5,i3+4,c3)+c(
     & i,6,5,4)*ui(i1+6,i2+5,i3+4,c3)+c(i,0,6,4)*ui(i1,i2+6,i3+4,c3)+
     & c(i,1,6,4)*ui(i1+1,i2+6,i3+4,c3)+c(i,2,6,4)*ui(i1+2,i2+6,i3+4,
     & c3)+c(i,3,6,4)*ui(i1+3,i2+6,i3+4,c3)+c(i,4,6,4)*ui(i1+4,i2+6,
     & i3+4,c3)+c(i,5,6,4)*ui(i1+5,i2+6,i3+4,c3)+c(i,6,6,4)*ui(i1+6,
     & i2+6,i3+4,c3)+c(i,0,0,5)*ui(i1,i2,i3+5,c3)+c(i,1,0,5)*ui(i1+1,
     & i2,i3+5,c3)+c(i,2,0,5)*ui(i1+2,i2,i3+5,c3)+c(i,3,0,5)*ui(i1+3,
     & i2,i3+5,c3)+c(i,4,0,5)*ui(i1+4,i2,i3+5,c3)+c(i,5,0,5)*ui(i1+5,
     & i2,i3+5,c3)+c(i,6,0,5)*ui(i1+6,i2,i3+5,c3)+c(i,0,1,5)*ui(i1,i2+
     & 1,i3+5,c3)+c(i,1,1,5)*ui(i1+1,i2+1,i3+5,c3)+c(i,2,1,5)*ui(i1+2,
     & i2+1,i3+5,c3)+c(i,3,1,5)*ui(i1+3,i2+1,i3+5,c3)+c(i,4,1,5)*ui(
     & i1+4,i2+1,i3+5,c3)+c(i,5,1,5)*ui(i1+5,i2+1,i3+5,c3)+c(i,6,1,5)*
     & ui(i1+6,i2+1,i3+5,c3)+c(i,0,2,5)*ui(i1,i2+2,i3+5,c3)+c(i,1,2,5)
     & *ui(i1+1,i2+2,i3+5,c3)+c(i,2,2,5)*ui(i1+2,i2+2,i3+5,c3)+c(i,3,
     & 2,5)*ui(i1+3,i2+2,i3+5,c3)+c(i,4,2,5)*ui(i1+4,i2+2,i3+5,c3)+c(
     & i,5,2,5)*ui(i1+5,i2+2,i3+5,c3)+c(i,6,2,5)*ui(i1+6,i2+2,i3+5,c3)
     & +c(i,0,3,5)*ui(i1,i2+3,i3+5,c3)+c(i,1,3,5)*ui(i1+1,i2+3,i3+5,
     & c3)+c(i,2,3,5)*ui(i1+2,i2+3,i3+5,c3)+c(i,3,3,5)*ui(i1+3,i2+3,
     & i3+5,c3)+c(i,4,3,5)*ui(i1+4,i2+3,i3+5,c3)+c(i,5,3,5)*ui(i1+5,
     & i2+3,i3+5,c3)+c(i,6,3,5)*ui(i1+6,i2+3,i3+5,c3)+c(i,0,4,5)*ui(
     & i1,i2+4,i3+5,c3)+c(i,1,4,5)*ui(i1+1,i2+4,i3+5,c3)+c(i,2,4,5)*
     & ui(i1+2,i2+4,i3+5,c3)+c(i,3,4,5)*ui(i1+3,i2+4,i3+5,c3)+c(i,4,4,
     & 5)*ui(i1+4,i2+4,i3+5,c3)+c(i,5,4,5)*ui(i1+5,i2+4,i3+5,c3)+c(i,
     & 6,4,5)*ui(i1+6,i2+4,i3+5,c3)+c(i,0,5,5)*ui(i1,i2+5,i3+5,c3)+c(
     & i,1,5,5)*ui(i1+1,i2+5,i3+5,c3)+c(i,2,5,5)*ui(i1+2,i2+5,i3+5,c3)
     & +c(i,3,5,5)*ui(i1+3,i2+5,i3+5,c3)+c(i,4,5,5)*ui(i1+4,i2+5,i3+5,
     & c3)+c(i,5,5,5)*ui(i1+5,i2+5,i3+5,c3)+c(i,6,5,5)*ui(i1+6,i2+5,
     & i3+5,c3)+c(i,0,6,5)*ui(i1,i2+6,i3+5,c3)+c(i,1,6,5)*ui(i1+1,i2+
     & 6,i3+5,c3)+c(i,2,6,5)*ui(i1+2,i2+6,i3+5,c3)+c(i,3,6,5)*ui(i1+3,
     & i2+6,i3+5,c3)+c(i,4,6,5)*ui(i1+4,i2+6,i3+5,c3)+c(i,5,6,5)*ui(
     & i1+5,i2+6,i3+5,c3)+c(i,6,6,5)*ui(i1+6,i2+6,i3+5,c3)+c(i,0,0,6)*
     & ui(i1,i2,i3+6,c3)+c(i,1,0,6)*ui(i1+1,i2,i3+6,c3)+c(i,2,0,6)*ui(
     & i1+2,i2,i3+6,c3)+c(i,3,0,6)*ui(i1+3,i2,i3+6,c3)+c(i,4,0,6)*ui(
     & i1+4,i2,i3+6,c3)+c(i,5,0,6)*ui(i1+5,i2,i3+6,c3)+c(i,6,0,6)*ui(
     & i1+6,i2,i3+6,c3)+c(i,0,1,6)*ui(i1,i2+1,i3+6,c3)+c(i,1,1,6)*ui(
     & i1+1,i2+1,i3+6,c3)+c(i,2,1,6)*ui(i1+2,i2+1,i3+6,c3)+c(i,3,1,6)*
     & ui(i1+3,i2+1,i3+6,c3)+c(i,4,1,6)*ui(i1+4,i2+1,i3+6,c3)+c(i,5,1,
     & 6)*ui(i1+5,i2+1,i3+6,c3)+c(i,6,1,6)*ui(i1+6,i2+1,i3+6,c3)+c(i,
     & 0,2,6)*ui(i1,i2+2,i3+6,c3)+c(i,1,2,6)*ui(i1+1,i2+2,i3+6,c3)+c(
     & i,2,2,6)*ui(i1+2,i2+2,i3+6,c3)+c(i,3,2,6)*ui(i1+3,i2+2,i3+6,c3)
     & +c(i,4,2,6)*ui(i1+4,i2+2,i3+6,c3)+c(i,5,2,6)*ui(i1+5,i2+2,i3+6,
     & c3)+c(i,6,2,6)*ui(i1+6,i2+2,i3+6,c3)+c(i,0,3,6)*ui(i1,i2+3,i3+
     & 6,c3)+c(i,1,3,6)*ui(i1+1,i2+3,i3+6,c3)+c(i,2,3,6)*ui(i1+2,i2+3,
     & i3+6,c3)+c(i,3,3,6)*ui(i1+3,i2+3,i3+6,c3)+c(i,4,3,6)*ui(i1+4,
     & i2+3,i3+6,c3)+c(i,5,3,6)*ui(i1+5,i2+3,i3+6,c3)+c(i,6,3,6)*ui(
     & i1+6,i2+3,i3+6,c3)+c(i,0,4,6)*ui(i1,i2+4,i3+6,c3)+c(i,1,4,6)*
     & ui(i1+1,i2+4,i3+6,c3)+c(i,2,4,6)*ui(i1+2,i2+4,i3+6,c3)+c(i,3,4,
     & 6)*ui(i1+3,i2+4,i3+6,c3)+c(i,4,4,6)*ui(i1+4,i2+4,i3+6,c3)+c(i,
     & 5,4,6)*ui(i1+5,i2+4,i3+6,c3)+c(i,6,4,6)*ui(i1+6,i2+4,i3+6,c3)+
     & c(i,0,5,6)*ui(i1,i2+5,i3+6,c3)+c(i,1,5,6)*ui(i1+1,i2+5,i3+6,c3)
     & +c(i,2,5,6)*ui(i1+2,i2+5,i3+6,c3)+c(i,3,5,6)*ui(i1+3,i2+5,i3+6,
     & c3)+c(i,4,5,6)*ui(i1+4,i2+5,i3+6,c3)+c(i,5,5,6)*ui(i1+5,i2+5,
     & i3+6,c3)+c(i,6,5,6)*ui(i1+6,i2+5,i3+6,c3)+c(i,0,6,6)*ui(i1,i2+
     & 6,i3+6,c3)+c(i,1,6,6)*ui(i1+1,i2+6,i3+6,c3)+c(i,2,6,6)*ui(i1+2,
     & i2+6,i3+6,c3)+c(i,3,6,6)*ui(i1+3,i2+6,i3+6,c3)+c(i,4,6,6)*ui(
     & i1+4,i2+6,i3+6,c3)+c(i,5,6,6)*ui(i1+5,i2+6,i3+6,c3)+c(i,6,6,6)*
     & ui(i1+6,i2+6,i3+6,c3)
             else if( varWidth(i).eq.6 )then
! interp666(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,0,0)*ui(i1,i2,i3,c3)+c(i,1,0,0)*ui(i1+1,i2,
     & i3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3,c3)+c(i,3,0,0)*ui(i1+3,i2,i3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2,i3,c3)+c(i,5,0,0)*ui(i1+5,i2,i3,c3)+
     & c(i,0,1,0)*ui(i1,i2+1,i3,c3)+c(i,1,1,0)*ui(i1+1,i2+1,i3,c3)+c(
     & i,2,1,0)*ui(i1+2,i2+1,i3,c3)+c(i,3,1,0)*ui(i1+3,i2+1,i3,c3)+c(
     & i,4,1,0)*ui(i1+4,i2+1,i3,c3)+c(i,5,1,0)*ui(i1+5,i2+1,i3,c3)+c(
     & i,0,2,0)*ui(i1,i2+2,i3,c3)+c(i,1,2,0)*ui(i1+1,i2+2,i3,c3)+c(i,
     & 2,2,0)*ui(i1+2,i2+2,i3,c3)+c(i,3,2,0)*ui(i1+3,i2+2,i3,c3)+c(i,
     & 4,2,0)*ui(i1+4,i2+2,i3,c3)+c(i,5,2,0)*ui(i1+5,i2+2,i3,c3)+c(i,
     & 0,3,0)*ui(i1,i2+3,i3,c3)+c(i,1,3,0)*ui(i1+1,i2+3,i3,c3)+c(i,2,
     & 3,0)*ui(i1+2,i2+3,i3,c3)+c(i,3,3,0)*ui(i1+3,i2+3,i3,c3)+c(i,4,
     & 3,0)*ui(i1+4,i2+3,i3,c3)+c(i,5,3,0)*ui(i1+5,i2+3,i3,c3)+c(i,0,
     & 4,0)*ui(i1,i2+4,i3,c3)+c(i,1,4,0)*ui(i1+1,i2+4,i3,c3)+c(i,2,4,
     & 0)*ui(i1+2,i2+4,i3,c3)+c(i,3,4,0)*ui(i1+3,i2+4,i3,c3)+c(i,4,4,
     & 0)*ui(i1+4,i2+4,i3,c3)+c(i,5,4,0)*ui(i1+5,i2+4,i3,c3)+c(i,0,5,
     & 0)*ui(i1,i2+5,i3,c3)+c(i,1,5,0)*ui(i1+1,i2+5,i3,c3)+c(i,2,5,0)*
     & ui(i1+2,i2+5,i3,c3)+c(i,3,5,0)*ui(i1+3,i2+5,i3,c3)+c(i,4,5,0)*
     & ui(i1+4,i2+5,i3,c3)+c(i,5,5,0)*ui(i1+5,i2+5,i3,c3)+c(i,0,0,1)*
     & ui(i1,i2,i3+1,c3)+c(i,1,0,1)*ui(i1+1,i2,i3+1,c3)+c(i,2,0,1)*ui(
     & i1+2,i2,i3+1,c3)+c(i,3,0,1)*ui(i1+3,i2,i3+1,c3)+c(i,4,0,1)*ui(
     & i1+4,i2,i3+1,c3)+c(i,5,0,1)*ui(i1+5,i2,i3+1,c3)+c(i,0,1,1)*ui(
     & i1,i2+1,i3+1,c3)+c(i,1,1,1)*ui(i1+1,i2+1,i3+1,c3)+c(i,2,1,1)*
     & ui(i1+2,i2+1,i3+1,c3)+c(i,3,1,1)*ui(i1+3,i2+1,i3+1,c3)+c(i,4,1,
     & 1)*ui(i1+4,i2+1,i3+1,c3)+c(i,5,1,1)*ui(i1+5,i2+1,i3+1,c3)+c(i,
     & 0,2,1)*ui(i1,i2+2,i3+1,c3)+c(i,1,2,1)*ui(i1+1,i2+2,i3+1,c3)+c(
     & i,2,2,1)*ui(i1+2,i2+2,i3+1,c3)+c(i,3,2,1)*ui(i1+3,i2+2,i3+1,c3)
     & +c(i,4,2,1)*ui(i1+4,i2+2,i3+1,c3)+c(i,5,2,1)*ui(i1+5,i2+2,i3+1,
     & c3)+c(i,0,3,1)*ui(i1,i2+3,i3+1,c3)+c(i,1,3,1)*ui(i1+1,i2+3,i3+
     & 1,c3)+c(i,2,3,1)*ui(i1+2,i2+3,i3+1,c3)+c(i,3,3,1)*ui(i1+3,i2+3,
     & i3+1,c3)+c(i,4,3,1)*ui(i1+4,i2+3,i3+1,c3)+c(i,5,3,1)*ui(i1+5,
     & i2+3,i3+1,c3)+c(i,0,4,1)*ui(i1,i2+4,i3+1,c3)+c(i,1,4,1)*ui(i1+
     & 1,i2+4,i3+1,c3)+c(i,2,4,1)*ui(i1+2,i2+4,i3+1,c3)+c(i,3,4,1)*ui(
     & i1+3,i2+4,i3+1,c3)+c(i,4,4,1)*ui(i1+4,i2+4,i3+1,c3)+c(i,5,4,1)*
     & ui(i1+5,i2+4,i3+1,c3)+c(i,0,5,1)*ui(i1,i2+5,i3+1,c3)+c(i,1,5,1)
     & *ui(i1+1,i2+5,i3+1,c3)+c(i,2,5,1)*ui(i1+2,i2+5,i3+1,c3)+c(i,3,
     & 5,1)*ui(i1+3,i2+5,i3+1,c3)+c(i,4,5,1)*ui(i1+4,i2+5,i3+1,c3)+c(
     & i,5,5,1)*ui(i1+5,i2+5,i3+1,c3)+c(i,0,0,2)*ui(i1,i2,i3+2,c3)+c(
     & i,1,0,2)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,2)*ui(i1+2,i2,i3+2,c3)+c(
     & i,3,0,2)*ui(i1+3,i2,i3+2,c3)+c(i,4,0,2)*ui(i1+4,i2,i3+2,c3)+c(
     & i,5,0,2)*ui(i1+5,i2,i3+2,c3)+c(i,0,1,2)*ui(i1,i2+1,i3+2,c3)+c(
     & i,1,1,2)*ui(i1+1,i2+1,i3+2,c3)+c(i,2,1,2)*ui(i1+2,i2+1,i3+2,c3)
     & +c(i,3,1,2)*ui(i1+3,i2+1,i3+2,c3)+c(i,4,1,2)*ui(i1+4,i2+1,i3+2,
     & c3)+c(i,5,1,2)*ui(i1+5,i2+1,i3+2,c3)+c(i,0,2,2)*ui(i1,i2+2,i3+
     & 2,c3)+c(i,1,2,2)*ui(i1+1,i2+2,i3+2,c3)+c(i,2,2,2)*ui(i1+2,i2+2,
     & i3+2,c3)+c(i,3,2,2)*ui(i1+3,i2+2,i3+2,c3)+c(i,4,2,2)*ui(i1+4,
     & i2+2,i3+2,c3)+c(i,5,2,2)*ui(i1+5,i2+2,i3+2,c3)+c(i,0,3,2)*ui(
     & i1,i2+3,i3+2,c3)+c(i,1,3,2)*ui(i1+1,i2+3,i3+2,c3)+c(i,2,3,2)*
     & ui(i1+2,i2+3,i3+2,c3)+c(i,3,3,2)*ui(i1+3,i2+3,i3+2,c3)+c(i,4,3,
     & 2)*ui(i1+4,i2+3,i3+2,c3)+c(i,5,3,2)*ui(i1+5,i2+3,i3+2,c3)+c(i,
     & 0,4,2)*ui(i1,i2+4,i3+2,c3)+c(i,1,4,2)*ui(i1+1,i2+4,i3+2,c3)+c(
     & i,2,4,2)*ui(i1+2,i2+4,i3+2,c3)+c(i,3,4,2)*ui(i1+3,i2+4,i3+2,c3)
     & +c(i,4,4,2)*ui(i1+4,i2+4,i3+2,c3)+c(i,5,4,2)*ui(i1+5,i2+4,i3+2,
     & c3)+c(i,0,5,2)*ui(i1,i2+5,i3+2,c3)+c(i,1,5,2)*ui(i1+1,i2+5,i3+
     & 2,c3)+c(i,2,5,2)*ui(i1+2,i2+5,i3+2,c3)+c(i,3,5,2)*ui(i1+3,i2+5,
     & i3+2,c3)+c(i,4,5,2)*ui(i1+4,i2+5,i3+2,c3)+c(i,5,5,2)*ui(i1+5,
     & i2+5,i3+2,c3)+c(i,0,0,3)*ui(i1,i2,i3+3,c3)+c(i,1,0,3)*ui(i1+1,
     & i2,i3+3,c3)+c(i,2,0,3)*ui(i1+2,i2,i3+3,c3)+c(i,3,0,3)*ui(i1+3,
     & i2,i3+3,c3)+c(i,4,0,3)*ui(i1+4,i2,i3+3,c3)+c(i,5,0,3)*ui(i1+5,
     & i2,i3+3,c3)+c(i,0,1,3)*ui(i1,i2+1,i3+3,c3)+c(i,1,1,3)*ui(i1+1,
     & i2+1,i3+3,c3)+c(i,2,1,3)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,1,3)*ui(
     & i1+3,i2+1,i3+3,c3)+c(i,4,1,3)*ui(i1+4,i2+1,i3+3,c3)+c(i,5,1,3)*
     & ui(i1+5,i2+1,i3+3,c3)+c(i,0,2,3)*ui(i1,i2+2,i3+3,c3)+c(i,1,2,3)
     & *ui(i1+1,i2+2,i3+3,c3)+c(i,2,2,3)*ui(i1+2,i2+2,i3+3,c3)+c(i,3,
     & 2,3)*ui(i1+3,i2+2,i3+3,c3)+c(i,4,2,3)*ui(i1+4,i2+2,i3+3,c3)+c(
     & i,5,2,3)*ui(i1+5,i2+2,i3+3,c3)+c(i,0,3,3)*ui(i1,i2+3,i3+3,c3)+
     & c(i,1,3,3)*ui(i1+1,i2+3,i3+3,c3)+c(i,2,3,3)*ui(i1+2,i2+3,i3+3,
     & c3)+c(i,3,3,3)*ui(i1+3,i2+3,i3+3,c3)+c(i,4,3,3)*ui(i1+4,i2+3,
     & i3+3,c3)+c(i,5,3,3)*ui(i1+5,i2+3,i3+3,c3)+c(i,0,4,3)*ui(i1,i2+
     & 4,i3+3,c3)+c(i,1,4,3)*ui(i1+1,i2+4,i3+3,c3)+c(i,2,4,3)*ui(i1+2,
     & i2+4,i3+3,c3)+c(i,3,4,3)*ui(i1+3,i2+4,i3+3,c3)+c(i,4,4,3)*ui(
     & i1+4,i2+4,i3+3,c3)+c(i,5,4,3)*ui(i1+5,i2+4,i3+3,c3)+c(i,0,5,3)*
     & ui(i1,i2+5,i3+3,c3)+c(i,1,5,3)*ui(i1+1,i2+5,i3+3,c3)+c(i,2,5,3)
     & *ui(i1+2,i2+5,i3+3,c3)+c(i,3,5,3)*ui(i1+3,i2+5,i3+3,c3)+c(i,4,
     & 5,3)*ui(i1+4,i2+5,i3+3,c3)+c(i,5,5,3)*ui(i1+5,i2+5,i3+3,c3)+c(
     & i,0,0,4)*ui(i1,i2,i3+4,c3)+c(i,1,0,4)*ui(i1+1,i2,i3+4,c3)+c(i,
     & 2,0,4)*ui(i1+2,i2,i3+4,c3)+c(i,3,0,4)*ui(i1+3,i2,i3+4,c3)+c(i,
     & 4,0,4)*ui(i1+4,i2,i3+4,c3)+c(i,5,0,4)*ui(i1+5,i2,i3+4,c3)+c(i,
     & 0,1,4)*ui(i1,i2+1,i3+4,c3)+c(i,1,1,4)*ui(i1+1,i2+1,i3+4,c3)+c(
     & i,2,1,4)*ui(i1+2,i2+1,i3+4,c3)+c(i,3,1,4)*ui(i1+3,i2+1,i3+4,c3)
     & +c(i,4,1,4)*ui(i1+4,i2+1,i3+4,c3)+c(i,5,1,4)*ui(i1+5,i2+1,i3+4,
     & c3)+c(i,0,2,4)*ui(i1,i2+2,i3+4,c3)+c(i,1,2,4)*ui(i1+1,i2+2,i3+
     & 4,c3)+c(i,2,2,4)*ui(i1+2,i2+2,i3+4,c3)+c(i,3,2,4)*ui(i1+3,i2+2,
     & i3+4,c3)+c(i,4,2,4)*ui(i1+4,i2+2,i3+4,c3)+c(i,5,2,4)*ui(i1+5,
     & i2+2,i3+4,c3)+c(i,0,3,4)*ui(i1,i2+3,i3+4,c3)+c(i,1,3,4)*ui(i1+
     & 1,i2+3,i3+4,c3)+c(i,2,3,4)*ui(i1+2,i2+3,i3+4,c3)+c(i,3,3,4)*ui(
     & i1+3,i2+3,i3+4,c3)+c(i,4,3,4)*ui(i1+4,i2+3,i3+4,c3)+c(i,5,3,4)*
     & ui(i1+5,i2+3,i3+4,c3)+c(i,0,4,4)*ui(i1,i2+4,i3+4,c3)+c(i,1,4,4)
     & *ui(i1+1,i2+4,i3+4,c3)+c(i,2,4,4)*ui(i1+2,i2+4,i3+4,c3)+c(i,3,
     & 4,4)*ui(i1+3,i2+4,i3+4,c3)+c(i,4,4,4)*ui(i1+4,i2+4,i3+4,c3)+c(
     & i,5,4,4)*ui(i1+5,i2+4,i3+4,c3)+c(i,0,5,4)*ui(i1,i2+5,i3+4,c3)+
     & c(i,1,5,4)*ui(i1+1,i2+5,i3+4,c3)+c(i,2,5,4)*ui(i1+2,i2+5,i3+4,
     & c3)+c(i,3,5,4)*ui(i1+3,i2+5,i3+4,c3)+c(i,4,5,4)*ui(i1+4,i2+5,
     & i3+4,c3)+c(i,5,5,4)*ui(i1+5,i2+5,i3+4,c3)+c(i,0,0,5)*ui(i1,i2,
     & i3+5,c3)+c(i,1,0,5)*ui(i1+1,i2,i3+5,c3)+c(i,2,0,5)*ui(i1+2,i2,
     & i3+5,c3)+c(i,3,0,5)*ui(i1+3,i2,i3+5,c3)
               r(i) = r(i)+c(i,4,0,5)*ui(i1+4,i2,i3+5,c3)+c(i,5,0,5)*
     & ui(i1+5,i2,i3+5,c3)+c(i,0,1,5)*ui(i1,i2+1,i3+5,c3)+c(i,1,1,5)*
     & ui(i1+1,i2+1,i3+5,c3)+c(i,2,1,5)*ui(i1+2,i2+1,i3+5,c3)+c(i,3,1,
     & 5)*ui(i1+3,i2+1,i3+5,c3)+c(i,4,1,5)*ui(i1+4,i2+1,i3+5,c3)+c(i,
     & 5,1,5)*ui(i1+5,i2+1,i3+5,c3)+c(i,0,2,5)*ui(i1,i2+2,i3+5,c3)+c(
     & i,1,2,5)*ui(i1+1,i2+2,i3+5,c3)+c(i,2,2,5)*ui(i1+2,i2+2,i3+5,c3)
     & +c(i,3,2,5)*ui(i1+3,i2+2,i3+5,c3)+c(i,4,2,5)*ui(i1+4,i2+2,i3+5,
     & c3)+c(i,5,2,5)*ui(i1+5,i2+2,i3+5,c3)+c(i,0,3,5)*ui(i1,i2+3,i3+
     & 5,c3)+c(i,1,3,5)*ui(i1+1,i2+3,i3+5,c3)+c(i,2,3,5)*ui(i1+2,i2+3,
     & i3+5,c3)+c(i,3,3,5)*ui(i1+3,i2+3,i3+5,c3)+c(i,4,3,5)*ui(i1+4,
     & i2+3,i3+5,c3)+c(i,5,3,5)*ui(i1+5,i2+3,i3+5,c3)+c(i,0,4,5)*ui(
     & i1,i2+4,i3+5,c3)+c(i,1,4,5)*ui(i1+1,i2+4,i3+5,c3)+c(i,2,4,5)*
     & ui(i1+2,i2+4,i3+5,c3)+c(i,3,4,5)*ui(i1+3,i2+4,i3+5,c3)+c(i,4,4,
     & 5)*ui(i1+4,i2+4,i3+5,c3)+c(i,5,4,5)*ui(i1+5,i2+4,i3+5,c3)+c(i,
     & 0,5,5)*ui(i1,i2+5,i3+5,c3)+c(i,1,5,5)*ui(i1+1,i2+5,i3+5,c3)+c(
     & i,2,5,5)*ui(i1+2,i2+5,i3+5,c3)+c(i,3,5,5)*ui(i1+3,i2+5,i3+5,c3)
     & +c(i,4,5,5)*ui(i1+4,i2+5,i3+5,c3)+c(i,5,5,5)*ui(i1+5,i2+5,i3+5,
     & c3)
             else if( varWidth(i).eq.9 )then
! interp999(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,0,0)*ui(i1,i2,i3,c3)+c(i,1,0,0)*ui(i1+1,i2,
     & i3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3,c3)+c(i,3,0,0)*ui(i1+3,i2,i3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2,i3,c3)+c(i,5,0,0)*ui(i1+5,i2,i3,c3)+
     & c(i,6,0,0)*ui(i1+6,i2,i3,c3)+c(i,7,0,0)*ui(i1+7,i2,i3,c3)+c(i,
     & 8,0,0)*ui(i1+8,i2,i3,c3)+c(i,0,1,0)*ui(i1,i2+1,i3,c3)+c(i,1,1,
     & 0)*ui(i1+1,i2+1,i3,c3)+c(i,2,1,0)*ui(i1+2,i2+1,i3,c3)+c(i,3,1,
     & 0)*ui(i1+3,i2+1,i3,c3)+c(i,4,1,0)*ui(i1+4,i2+1,i3,c3)+c(i,5,1,
     & 0)*ui(i1+5,i2+1,i3,c3)+c(i,6,1,0)*ui(i1+6,i2+1,i3,c3)+c(i,7,1,
     & 0)*ui(i1+7,i2+1,i3,c3)+c(i,8,1,0)*ui(i1+8,i2+1,i3,c3)+c(i,0,2,
     & 0)*ui(i1,i2+2,i3,c3)+c(i,1,2,0)*ui(i1+1,i2+2,i3,c3)+c(i,2,2,0)*
     & ui(i1+2,i2+2,i3,c3)+c(i,3,2,0)*ui(i1+3,i2+2,i3,c3)+c(i,4,2,0)*
     & ui(i1+4,i2+2,i3,c3)+c(i,5,2,0)*ui(i1+5,i2+2,i3,c3)+c(i,6,2,0)*
     & ui(i1+6,i2+2,i3,c3)+c(i,7,2,0)*ui(i1+7,i2+2,i3,c3)+c(i,8,2,0)*
     & ui(i1+8,i2+2,i3,c3)+c(i,0,3,0)*ui(i1,i2+3,i3,c3)+c(i,1,3,0)*ui(
     & i1+1,i2+3,i3,c3)+c(i,2,3,0)*ui(i1+2,i2+3,i3,c3)+c(i,3,3,0)*ui(
     & i1+3,i2+3,i3,c3)+c(i,4,3,0)*ui(i1+4,i2+3,i3,c3)+c(i,5,3,0)*ui(
     & i1+5,i2+3,i3,c3)+c(i,6,3,0)*ui(i1+6,i2+3,i3,c3)+c(i,7,3,0)*ui(
     & i1+7,i2+3,i3,c3)+c(i,8,3,0)*ui(i1+8,i2+3,i3,c3)+c(i,0,4,0)*ui(
     & i1,i2+4,i3,c3)+c(i,1,4,0)*ui(i1+1,i2+4,i3,c3)+c(i,2,4,0)*ui(i1+
     & 2,i2+4,i3,c3)+c(i,3,4,0)*ui(i1+3,i2+4,i3,c3)+c(i,4,4,0)*ui(i1+
     & 4,i2+4,i3,c3)+c(i,5,4,0)*ui(i1+5,i2+4,i3,c3)+c(i,6,4,0)*ui(i1+
     & 6,i2+4,i3,c3)+c(i,7,4,0)*ui(i1+7,i2+4,i3,c3)+c(i,8,4,0)*ui(i1+
     & 8,i2+4,i3,c3)+c(i,0,5,0)*ui(i1,i2+5,i3,c3)+c(i,1,5,0)*ui(i1+1,
     & i2+5,i3,c3)+c(i,2,5,0)*ui(i1+2,i2+5,i3,c3)+c(i,3,5,0)*ui(i1+3,
     & i2+5,i3,c3)+c(i,4,5,0)*ui(i1+4,i2+5,i3,c3)+c(i,5,5,0)*ui(i1+5,
     & i2+5,i3,c3)+c(i,6,5,0)*ui(i1+6,i2+5,i3,c3)+c(i,7,5,0)*ui(i1+7,
     & i2+5,i3,c3)+c(i,8,5,0)*ui(i1+8,i2+5,i3,c3)+c(i,0,6,0)*ui(i1,i2+
     & 6,i3,c3)+c(i,1,6,0)*ui(i1+1,i2+6,i3,c3)+c(i,2,6,0)*ui(i1+2,i2+
     & 6,i3,c3)+c(i,3,6,0)*ui(i1+3,i2+6,i3,c3)+c(i,4,6,0)*ui(i1+4,i2+
     & 6,i3,c3)+c(i,5,6,0)*ui(i1+5,i2+6,i3,c3)+c(i,6,6,0)*ui(i1+6,i2+
     & 6,i3,c3)+c(i,7,6,0)*ui(i1+7,i2+6,i3,c3)+c(i,8,6,0)*ui(i1+8,i2+
     & 6,i3,c3)+c(i,0,7,0)*ui(i1,i2+7,i3,c3)+c(i,1,7,0)*ui(i1+1,i2+7,
     & i3,c3)+c(i,2,7,0)*ui(i1+2,i2+7,i3,c3)+c(i,3,7,0)*ui(i1+3,i2+7,
     & i3,c3)+c(i,4,7,0)*ui(i1+4,i2+7,i3,c3)+c(i,5,7,0)*ui(i1+5,i2+7,
     & i3,c3)+c(i,6,7,0)*ui(i1+6,i2+7,i3,c3)+c(i,7,7,0)*ui(i1+7,i2+7,
     & i3,c3)+c(i,8,7,0)*ui(i1+8,i2+7,i3,c3)+c(i,0,8,0)*ui(i1,i2+8,i3,
     & c3)+c(i,1,8,0)*ui(i1+1,i2+8,i3,c3)+c(i,2,8,0)*ui(i1+2,i2+8,i3,
     & c3)+c(i,3,8,0)*ui(i1+3,i2+8,i3,c3)+c(i,4,8,0)*ui(i1+4,i2+8,i3,
     & c3)+c(i,5,8,0)*ui(i1+5,i2+8,i3,c3)+c(i,6,8,0)*ui(i1+6,i2+8,i3,
     & c3)+c(i,7,8,0)*ui(i1+7,i2+8,i3,c3)+c(i,8,8,0)*ui(i1+8,i2+8,i3,
     & c3)+c(i,0,0,1)*ui(i1,i2,i3+1,c3)+c(i,1,0,1)*ui(i1+1,i2,i3+1,c3)
     & +c(i,2,0,1)*ui(i1+2,i2,i3+1,c3)+c(i,3,0,1)*ui(i1+3,i2,i3+1,c3)+
     & c(i,4,0,1)*ui(i1+4,i2,i3+1,c3)+c(i,5,0,1)*ui(i1+5,i2,i3+1,c3)+
     & c(i,6,0,1)*ui(i1+6,i2,i3+1,c3)+c(i,7,0,1)*ui(i1+7,i2,i3+1,c3)+
     & c(i,8,0,1)*ui(i1+8,i2,i3+1,c3)+c(i,0,1,1)*ui(i1,i2+1,i3+1,c3)+
     & c(i,1,1,1)*ui(i1+1,i2+1,i3+1,c3)+c(i,2,1,1)*ui(i1+2,i2+1,i3+1,
     & c3)+c(i,3,1,1)*ui(i1+3,i2+1,i3+1,c3)+c(i,4,1,1)*ui(i1+4,i2+1,
     & i3+1,c3)+c(i,5,1,1)*ui(i1+5,i2+1,i3+1,c3)+c(i,6,1,1)*ui(i1+6,
     & i2+1,i3+1,c3)+c(i,7,1,1)*ui(i1+7,i2+1,i3+1,c3)+c(i,8,1,1)*ui(
     & i1+8,i2+1,i3+1,c3)+c(i,0,2,1)*ui(i1,i2+2,i3+1,c3)+c(i,1,2,1)*
     & ui(i1+1,i2+2,i3+1,c3)+c(i,2,2,1)*ui(i1+2,i2+2,i3+1,c3)+c(i,3,2,
     & 1)*ui(i1+3,i2+2,i3+1,c3)+c(i,4,2,1)*ui(i1+4,i2+2,i3+1,c3)+c(i,
     & 5,2,1)*ui(i1+5,i2+2,i3+1,c3)+c(i,6,2,1)*ui(i1+6,i2+2,i3+1,c3)+
     & c(i,7,2,1)*ui(i1+7,i2+2,i3+1,c3)+c(i,8,2,1)*ui(i1+8,i2+2,i3+1,
     & c3)+c(i,0,3,1)*ui(i1,i2+3,i3+1,c3)+c(i,1,3,1)*ui(i1+1,i2+3,i3+
     & 1,c3)+c(i,2,3,1)*ui(i1+2,i2+3,i3+1,c3)+c(i,3,3,1)*ui(i1+3,i2+3,
     & i3+1,c3)+c(i,4,3,1)*ui(i1+4,i2+3,i3+1,c3)+c(i,5,3,1)*ui(i1+5,
     & i2+3,i3+1,c3)+c(i,6,3,1)*ui(i1+6,i2+3,i3+1,c3)+c(i,7,3,1)*ui(
     & i1+7,i2+3,i3+1,c3)+c(i,8,3,1)*ui(i1+8,i2+3,i3+1,c3)+c(i,0,4,1)*
     & ui(i1,i2+4,i3+1,c3)+c(i,1,4,1)*ui(i1+1,i2+4,i3+1,c3)+c(i,2,4,1)
     & *ui(i1+2,i2+4,i3+1,c3)+c(i,3,4,1)*ui(i1+3,i2+4,i3+1,c3)+c(i,4,
     & 4,1)*ui(i1+4,i2+4,i3+1,c3)+c(i,5,4,1)*ui(i1+5,i2+4,i3+1,c3)+c(
     & i,6,4,1)*ui(i1+6,i2+4,i3+1,c3)+c(i,7,4,1)*ui(i1+7,i2+4,i3+1,c3)
     & +c(i,8,4,1)*ui(i1+8,i2+4,i3+1,c3)+c(i,0,5,1)*ui(i1,i2+5,i3+1,
     & c3)+c(i,1,5,1)*ui(i1+1,i2+5,i3+1,c3)+c(i,2,5,1)*ui(i1+2,i2+5,
     & i3+1,c3)+c(i,3,5,1)*ui(i1+3,i2+5,i3+1,c3)+c(i,4,5,1)*ui(i1+4,
     & i2+5,i3+1,c3)+c(i,5,5,1)*ui(i1+5,i2+5,i3+1,c3)+c(i,6,5,1)*ui(
     & i1+6,i2+5,i3+1,c3)+c(i,7,5,1)*ui(i1+7,i2+5,i3+1,c3)+c(i,8,5,1)*
     & ui(i1+8,i2+5,i3+1,c3)+c(i,0,6,1)*ui(i1,i2+6,i3+1,c3)+c(i,1,6,1)
     & *ui(i1+1,i2+6,i3+1,c3)+c(i,2,6,1)*ui(i1+2,i2+6,i3+1,c3)+c(i,3,
     & 6,1)*ui(i1+3,i2+6,i3+1,c3)+c(i,4,6,1)*ui(i1+4,i2+6,i3+1,c3)+c(
     & i,5,6,1)*ui(i1+5,i2+6,i3+1,c3)+c(i,6,6,1)*ui(i1+6,i2+6,i3+1,c3)
     & +c(i,7,6,1)*ui(i1+7,i2+6,i3+1,c3)+c(i,8,6,1)*ui(i1+8,i2+6,i3+1,
     & c3)+c(i,0,7,1)*ui(i1,i2+7,i3+1,c3)+c(i,1,7,1)*ui(i1+1,i2+7,i3+
     & 1,c3)+c(i,2,7,1)*ui(i1+2,i2+7,i3+1,c3)+c(i,3,7,1)*ui(i1+3,i2+7,
     & i3+1,c3)+c(i,4,7,1)*ui(i1+4,i2+7,i3+1,c3)+c(i,5,7,1)*ui(i1+5,
     & i2+7,i3+1,c3)+c(i,6,7,1)*ui(i1+6,i2+7,i3+1,c3)+c(i,7,7,1)*ui(
     & i1+7,i2+7,i3+1,c3)+c(i,8,7,1)*ui(i1+8,i2+7,i3+1,c3)+c(i,0,8,1)*
     & ui(i1,i2+8,i3+1,c3)+c(i,1,8,1)*ui(i1+1,i2+8,i3+1,c3)+c(i,2,8,1)
     & *ui(i1+2,i2+8,i3+1,c3)+c(i,3,8,1)*ui(i1+3,i2+8,i3+1,c3)+c(i,4,
     & 8,1)*ui(i1+4,i2+8,i3+1,c3)+c(i,5,8,1)*ui(i1+5,i2+8,i3+1,c3)+c(
     & i,6,8,1)*ui(i1+6,i2+8,i3+1,c3)+c(i,7,8,1)*ui(i1+7,i2+8,i3+1,c3)
     & +c(i,8,8,1)*ui(i1+8,i2+8,i3+1,c3)+c(i,0,0,2)*ui(i1,i2,i3+2,c3)+
     & c(i,1,0,2)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,2)*ui(i1+2,i2,i3+2,c3)+
     & c(i,3,0,2)*ui(i1+3,i2,i3+2,c3)+c(i,4,0,2)*ui(i1+4,i2,i3+2,c3)+
     & c(i,5,0,2)*ui(i1+5,i2,i3+2,c3)+c(i,6,0,2)*ui(i1+6,i2,i3+2,c3)+
     & c(i,7,0,2)*ui(i1+7,i2,i3+2,c3)+c(i,8,0,2)*ui(i1+8,i2,i3+2,c3)+
     & c(i,0,1,2)*ui(i1,i2+1,i3+2,c3)+c(i,1,1,2)*ui(i1+1,i2+1,i3+2,c3)
     & +c(i,2,1,2)*ui(i1+2,i2+1,i3+2,c3)+c(i,3,1,2)*ui(i1+3,i2+1,i3+2,
     & c3)+c(i,4,1,2)*ui(i1+4,i2+1,i3+2,c3)+c(i,5,1,2)*ui(i1+5,i2+1,
     & i3+2,c3)+c(i,6,1,2)*ui(i1+6,i2+1,i3+2,c3)+c(i,7,1,2)*ui(i1+7,
     & i2+1,i3+2,c3)+c(i,8,1,2)*ui(i1+8,i2+1,i3+2,c3)+c(i,0,2,2)*ui(
     & i1,i2+2,i3+2,c3)+c(i,1,2,2)*ui(i1+1,i2+2,i3+2,c3)+c(i,2,2,2)*
     & ui(i1+2,i2+2,i3+2,c3)+c(i,3,2,2)*ui(i1+3,i2+2,i3+2,c3)
               r(i) = r(i)+c(i,4,2,2)*ui(i1+4,i2+2,i3+2,c3)+c(i,5,2,2)*
     & ui(i1+5,i2+2,i3+2,c3)+c(i,6,2,2)*ui(i1+6,i2+2,i3+2,c3)+c(i,7,2,
     & 2)*ui(i1+7,i2+2,i3+2,c3)+c(i,8,2,2)*ui(i1+8,i2+2,i3+2,c3)+c(i,
     & 0,3,2)*ui(i1,i2+3,i3+2,c3)+c(i,1,3,2)*ui(i1+1,i2+3,i3+2,c3)+c(
     & i,2,3,2)*ui(i1+2,i2+3,i3+2,c3)+c(i,3,3,2)*ui(i1+3,i2+3,i3+2,c3)
     & +c(i,4,3,2)*ui(i1+4,i2+3,i3+2,c3)+c(i,5,3,2)*ui(i1+5,i2+3,i3+2,
     & c3)+c(i,6,3,2)*ui(i1+6,i2+3,i3+2,c3)+c(i,7,3,2)*ui(i1+7,i2+3,
     & i3+2,c3)+c(i,8,3,2)*ui(i1+8,i2+3,i3+2,c3)+c(i,0,4,2)*ui(i1,i2+
     & 4,i3+2,c3)+c(i,1,4,2)*ui(i1+1,i2+4,i3+2,c3)+c(i,2,4,2)*ui(i1+2,
     & i2+4,i3+2,c3)+c(i,3,4,2)*ui(i1+3,i2+4,i3+2,c3)+c(i,4,4,2)*ui(
     & i1+4,i2+4,i3+2,c3)+c(i,5,4,2)*ui(i1+5,i2+4,i3+2,c3)+c(i,6,4,2)*
     & ui(i1+6,i2+4,i3+2,c3)+c(i,7,4,2)*ui(i1+7,i2+4,i3+2,c3)+c(i,8,4,
     & 2)*ui(i1+8,i2+4,i3+2,c3)+c(i,0,5,2)*ui(i1,i2+5,i3+2,c3)+c(i,1,
     & 5,2)*ui(i1+1,i2+5,i3+2,c3)+c(i,2,5,2)*ui(i1+2,i2+5,i3+2,c3)+c(
     & i,3,5,2)*ui(i1+3,i2+5,i3+2,c3)+c(i,4,5,2)*ui(i1+4,i2+5,i3+2,c3)
     & +c(i,5,5,2)*ui(i1+5,i2+5,i3+2,c3)+c(i,6,5,2)*ui(i1+6,i2+5,i3+2,
     & c3)+c(i,7,5,2)*ui(i1+7,i2+5,i3+2,c3)+c(i,8,5,2)*ui(i1+8,i2+5,
     & i3+2,c3)+c(i,0,6,2)*ui(i1,i2+6,i3+2,c3)+c(i,1,6,2)*ui(i1+1,i2+
     & 6,i3+2,c3)+c(i,2,6,2)*ui(i1+2,i2+6,i3+2,c3)+c(i,3,6,2)*ui(i1+3,
     & i2+6,i3+2,c3)+c(i,4,6,2)*ui(i1+4,i2+6,i3+2,c3)+c(i,5,6,2)*ui(
     & i1+5,i2+6,i3+2,c3)+c(i,6,6,2)*ui(i1+6,i2+6,i3+2,c3)+c(i,7,6,2)*
     & ui(i1+7,i2+6,i3+2,c3)+c(i,8,6,2)*ui(i1+8,i2+6,i3+2,c3)+c(i,0,7,
     & 2)*ui(i1,i2+7,i3+2,c3)+c(i,1,7,2)*ui(i1+1,i2+7,i3+2,c3)+c(i,2,
     & 7,2)*ui(i1+2,i2+7,i3+2,c3)+c(i,3,7,2)*ui(i1+3,i2+7,i3+2,c3)+c(
     & i,4,7,2)*ui(i1+4,i2+7,i3+2,c3)+c(i,5,7,2)*ui(i1+5,i2+7,i3+2,c3)
     & +c(i,6,7,2)*ui(i1+6,i2+7,i3+2,c3)+c(i,7,7,2)*ui(i1+7,i2+7,i3+2,
     & c3)+c(i,8,7,2)*ui(i1+8,i2+7,i3+2,c3)+c(i,0,8,2)*ui(i1,i2+8,i3+
     & 2,c3)+c(i,1,8,2)*ui(i1+1,i2+8,i3+2,c3)+c(i,2,8,2)*ui(i1+2,i2+8,
     & i3+2,c3)+c(i,3,8,2)*ui(i1+3,i2+8,i3+2,c3)+c(i,4,8,2)*ui(i1+4,
     & i2+8,i3+2,c3)+c(i,5,8,2)*ui(i1+5,i2+8,i3+2,c3)+c(i,6,8,2)*ui(
     & i1+6,i2+8,i3+2,c3)+c(i,7,8,2)*ui(i1+7,i2+8,i3+2,c3)+c(i,8,8,2)*
     & ui(i1+8,i2+8,i3+2,c3)+c(i,0,0,3)*ui(i1,i2,i3+3,c3)+c(i,1,0,3)*
     & ui(i1+1,i2,i3+3,c3)+c(i,2,0,3)*ui(i1+2,i2,i3+3,c3)+c(i,3,0,3)*
     & ui(i1+3,i2,i3+3,c3)+c(i,4,0,3)*ui(i1+4,i2,i3+3,c3)+c(i,5,0,3)*
     & ui(i1+5,i2,i3+3,c3)+c(i,6,0,3)*ui(i1+6,i2,i3+3,c3)+c(i,7,0,3)*
     & ui(i1+7,i2,i3+3,c3)+c(i,8,0,3)*ui(i1+8,i2,i3+3,c3)+c(i,0,1,3)*
     & ui(i1,i2+1,i3+3,c3)+c(i,1,1,3)*ui(i1+1,i2+1,i3+3,c3)+c(i,2,1,3)
     & *ui(i1+2,i2+1,i3+3,c3)+c(i,3,1,3)*ui(i1+3,i2+1,i3+3,c3)+c(i,4,
     & 1,3)*ui(i1+4,i2+1,i3+3,c3)+c(i,5,1,3)*ui(i1+5,i2+1,i3+3,c3)+c(
     & i,6,1,3)*ui(i1+6,i2+1,i3+3,c3)+c(i,7,1,3)*ui(i1+7,i2+1,i3+3,c3)
     & +c(i,8,1,3)*ui(i1+8,i2+1,i3+3,c3)+c(i,0,2,3)*ui(i1,i2+2,i3+3,
     & c3)+c(i,1,2,3)*ui(i1+1,i2+2,i3+3,c3)+c(i,2,2,3)*ui(i1+2,i2+2,
     & i3+3,c3)+c(i,3,2,3)*ui(i1+3,i2+2,i3+3,c3)+c(i,4,2,3)*ui(i1+4,
     & i2+2,i3+3,c3)+c(i,5,2,3)*ui(i1+5,i2+2,i3+3,c3)+c(i,6,2,3)*ui(
     & i1+6,i2+2,i3+3,c3)+c(i,7,2,3)*ui(i1+7,i2+2,i3+3,c3)+c(i,8,2,3)*
     & ui(i1+8,i2+2,i3+3,c3)+c(i,0,3,3)*ui(i1,i2+3,i3+3,c3)+c(i,1,3,3)
     & *ui(i1+1,i2+3,i3+3,c3)+c(i,2,3,3)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,
     & 3,3)*ui(i1+3,i2+3,i3+3,c3)+c(i,4,3,3)*ui(i1+4,i2+3,i3+3,c3)+c(
     & i,5,3,3)*ui(i1+5,i2+3,i3+3,c3)+c(i,6,3,3)*ui(i1+6,i2+3,i3+3,c3)
     & +c(i,7,3,3)*ui(i1+7,i2+3,i3+3,c3)+c(i,8,3,3)*ui(i1+8,i2+3,i3+3,
     & c3)+c(i,0,4,3)*ui(i1,i2+4,i3+3,c3)+c(i,1,4,3)*ui(i1+1,i2+4,i3+
     & 3,c3)+c(i,2,4,3)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,4,3)*ui(i1+3,i2+4,
     & i3+3,c3)+c(i,4,4,3)*ui(i1+4,i2+4,i3+3,c3)+c(i,5,4,3)*ui(i1+5,
     & i2+4,i3+3,c3)+c(i,6,4,3)*ui(i1+6,i2+4,i3+3,c3)+c(i,7,4,3)*ui(
     & i1+7,i2+4,i3+3,c3)+c(i,8,4,3)*ui(i1+8,i2+4,i3+3,c3)+c(i,0,5,3)*
     & ui(i1,i2+5,i3+3,c3)+c(i,1,5,3)*ui(i1+1,i2+5,i3+3,c3)+c(i,2,5,3)
     & *ui(i1+2,i2+5,i3+3,c3)+c(i,3,5,3)*ui(i1+3,i2+5,i3+3,c3)+c(i,4,
     & 5,3)*ui(i1+4,i2+5,i3+3,c3)+c(i,5,5,3)*ui(i1+5,i2+5,i3+3,c3)+c(
     & i,6,5,3)*ui(i1+6,i2+5,i3+3,c3)+c(i,7,5,3)*ui(i1+7,i2+5,i3+3,c3)
     & +c(i,8,5,3)*ui(i1+8,i2+5,i3+3,c3)+c(i,0,6,3)*ui(i1,i2+6,i3+3,
     & c3)+c(i,1,6,3)*ui(i1+1,i2+6,i3+3,c3)+c(i,2,6,3)*ui(i1+2,i2+6,
     & i3+3,c3)+c(i,3,6,3)*ui(i1+3,i2+6,i3+3,c3)+c(i,4,6,3)*ui(i1+4,
     & i2+6,i3+3,c3)+c(i,5,6,3)*ui(i1+5,i2+6,i3+3,c3)+c(i,6,6,3)*ui(
     & i1+6,i2+6,i3+3,c3)+c(i,7,6,3)*ui(i1+7,i2+6,i3+3,c3)+c(i,8,6,3)*
     & ui(i1+8,i2+6,i3+3,c3)+c(i,0,7,3)*ui(i1,i2+7,i3+3,c3)+c(i,1,7,3)
     & *ui(i1+1,i2+7,i3+3,c3)+c(i,2,7,3)*ui(i1+2,i2+7,i3+3,c3)+c(i,3,
     & 7,3)*ui(i1+3,i2+7,i3+3,c3)+c(i,4,7,3)*ui(i1+4,i2+7,i3+3,c3)+c(
     & i,5,7,3)*ui(i1+5,i2+7,i3+3,c3)+c(i,6,7,3)*ui(i1+6,i2+7,i3+3,c3)
     & +c(i,7,7,3)*ui(i1+7,i2+7,i3+3,c3)+c(i,8,7,3)*ui(i1+8,i2+7,i3+3,
     & c3)+c(i,0,8,3)*ui(i1,i2+8,i3+3,c3)+c(i,1,8,3)*ui(i1+1,i2+8,i3+
     & 3,c3)+c(i,2,8,3)*ui(i1+2,i2+8,i3+3,c3)+c(i,3,8,3)*ui(i1+3,i2+8,
     & i3+3,c3)+c(i,4,8,3)*ui(i1+4,i2+8,i3+3,c3)+c(i,5,8,3)*ui(i1+5,
     & i2+8,i3+3,c3)+c(i,6,8,3)*ui(i1+6,i2+8,i3+3,c3)+c(i,7,8,3)*ui(
     & i1+7,i2+8,i3+3,c3)+c(i,8,8,3)*ui(i1+8,i2+8,i3+3,c3)+c(i,0,0,4)*
     & ui(i1,i2,i3+4,c3)+c(i,1,0,4)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,4)*ui(
     & i1+2,i2,i3+4,c3)+c(i,3,0,4)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,4)*ui(
     & i1+4,i2,i3+4,c3)+c(i,5,0,4)*ui(i1+5,i2,i3+4,c3)+c(i,6,0,4)*ui(
     & i1+6,i2,i3+4,c3)+c(i,7,0,4)*ui(i1+7,i2,i3+4,c3)+c(i,8,0,4)*ui(
     & i1+8,i2,i3+4,c3)+c(i,0,1,4)*ui(i1,i2+1,i3+4,c3)+c(i,1,1,4)*ui(
     & i1+1,i2+1,i3+4,c3)+c(i,2,1,4)*ui(i1+2,i2+1,i3+4,c3)+c(i,3,1,4)*
     & ui(i1+3,i2+1,i3+4,c3)+c(i,4,1,4)*ui(i1+4,i2+1,i3+4,c3)+c(i,5,1,
     & 4)*ui(i1+5,i2+1,i3+4,c3)+c(i,6,1,4)*ui(i1+6,i2+1,i3+4,c3)+c(i,
     & 7,1,4)*ui(i1+7,i2+1,i3+4,c3)+c(i,8,1,4)*ui(i1+8,i2+1,i3+4,c3)+
     & c(i,0,2,4)*ui(i1,i2+2,i3+4,c3)+c(i,1,2,4)*ui(i1+1,i2+2,i3+4,c3)
     & +c(i,2,2,4)*ui(i1+2,i2+2,i3+4,c3)+c(i,3,2,4)*ui(i1+3,i2+2,i3+4,
     & c3)+c(i,4,2,4)*ui(i1+4,i2+2,i3+4,c3)+c(i,5,2,4)*ui(i1+5,i2+2,
     & i3+4,c3)+c(i,6,2,4)*ui(i1+6,i2+2,i3+4,c3)+c(i,7,2,4)*ui(i1+7,
     & i2+2,i3+4,c3)+c(i,8,2,4)*ui(i1+8,i2+2,i3+4,c3)+c(i,0,3,4)*ui(
     & i1,i2+3,i3+4,c3)+c(i,1,3,4)*ui(i1+1,i2+3,i3+4,c3)+c(i,2,3,4)*
     & ui(i1+2,i2+3,i3+4,c3)+c(i,3,3,4)*ui(i1+3,i2+3,i3+4,c3)+c(i,4,3,
     & 4)*ui(i1+4,i2+3,i3+4,c3)+c(i,5,3,4)*ui(i1+5,i2+3,i3+4,c3)+c(i,
     & 6,3,4)*ui(i1+6,i2+3,i3+4,c3)+c(i,7,3,4)*ui(i1+7,i2+3,i3+4,c3)+
     & c(i,8,3,4)*ui(i1+8,i2+3,i3+4,c3)+c(i,0,4,4)*ui(i1,i2+4,i3+4,c3)
     & +c(i,1,4,4)*ui(i1+1,i2+4,i3+4,c3)+c(i,2,4,4)*ui(i1+2,i2+4,i3+4,
     & c3)+c(i,3,4,4)*ui(i1+3,i2+4,i3+4,c3)+c(i,4,4,4)*ui(i1+4,i2+4,
     & i3+4,c3)+c(i,5,4,4)*ui(i1+5,i2+4,i3+4,c3)+c(i,6,4,4)*ui(i1+6,
     & i2+4,i3+4,c3)+c(i,7,4,4)*ui(i1+7,i2+4,i3+4,c3)
               r(i) = r(i)+c(i,8,4,4)*ui(i1+8,i2+4,i3+4,c3)+c(i,0,5,4)*
     & ui(i1,i2+5,i3+4,c3)+c(i,1,5,4)*ui(i1+1,i2+5,i3+4,c3)+c(i,2,5,4)
     & *ui(i1+2,i2+5,i3+4,c3)+c(i,3,5,4)*ui(i1+3,i2+5,i3+4,c3)+c(i,4,
     & 5,4)*ui(i1+4,i2+5,i3+4,c3)+c(i,5,5,4)*ui(i1+5,i2+5,i3+4,c3)+c(
     & i,6,5,4)*ui(i1+6,i2+5,i3+4,c3)+c(i,7,5,4)*ui(i1+7,i2+5,i3+4,c3)
     & +c(i,8,5,4)*ui(i1+8,i2+5,i3+4,c3)+c(i,0,6,4)*ui(i1,i2+6,i3+4,
     & c3)+c(i,1,6,4)*ui(i1+1,i2+6,i3+4,c3)+c(i,2,6,4)*ui(i1+2,i2+6,
     & i3+4,c3)+c(i,3,6,4)*ui(i1+3,i2+6,i3+4,c3)+c(i,4,6,4)*ui(i1+4,
     & i2+6,i3+4,c3)+c(i,5,6,4)*ui(i1+5,i2+6,i3+4,c3)+c(i,6,6,4)*ui(
     & i1+6,i2+6,i3+4,c3)+c(i,7,6,4)*ui(i1+7,i2+6,i3+4,c3)+c(i,8,6,4)*
     & ui(i1+8,i2+6,i3+4,c3)+c(i,0,7,4)*ui(i1,i2+7,i3+4,c3)+c(i,1,7,4)
     & *ui(i1+1,i2+7,i3+4,c3)+c(i,2,7,4)*ui(i1+2,i2+7,i3+4,c3)+c(i,3,
     & 7,4)*ui(i1+3,i2+7,i3+4,c3)+c(i,4,7,4)*ui(i1+4,i2+7,i3+4,c3)+c(
     & i,5,7,4)*ui(i1+5,i2+7,i3+4,c3)+c(i,6,7,4)*ui(i1+6,i2+7,i3+4,c3)
     & +c(i,7,7,4)*ui(i1+7,i2+7,i3+4,c3)+c(i,8,7,4)*ui(i1+8,i2+7,i3+4,
     & c3)+c(i,0,8,4)*ui(i1,i2+8,i3+4,c3)+c(i,1,8,4)*ui(i1+1,i2+8,i3+
     & 4,c3)+c(i,2,8,4)*ui(i1+2,i2+8,i3+4,c3)+c(i,3,8,4)*ui(i1+3,i2+8,
     & i3+4,c3)+c(i,4,8,4)*ui(i1+4,i2+8,i3+4,c3)+c(i,5,8,4)*ui(i1+5,
     & i2+8,i3+4,c3)+c(i,6,8,4)*ui(i1+6,i2+8,i3+4,c3)+c(i,7,8,4)*ui(
     & i1+7,i2+8,i3+4,c3)+c(i,8,8,4)*ui(i1+8,i2+8,i3+4,c3)+c(i,0,0,5)*
     & ui(i1,i2,i3+5,c3)+c(i,1,0,5)*ui(i1+1,i2,i3+5,c3)+c(i,2,0,5)*ui(
     & i1+2,i2,i3+5,c3)+c(i,3,0,5)*ui(i1+3,i2,i3+5,c3)+c(i,4,0,5)*ui(
     & i1+4,i2,i3+5,c3)+c(i,5,0,5)*ui(i1+5,i2,i3+5,c3)+c(i,6,0,5)*ui(
     & i1+6,i2,i3+5,c3)+c(i,7,0,5)*ui(i1+7,i2,i3+5,c3)+c(i,8,0,5)*ui(
     & i1+8,i2,i3+5,c3)+c(i,0,1,5)*ui(i1,i2+1,i3+5,c3)+c(i,1,1,5)*ui(
     & i1+1,i2+1,i3+5,c3)+c(i,2,1,5)*ui(i1+2,i2+1,i3+5,c3)+c(i,3,1,5)*
     & ui(i1+3,i2+1,i3+5,c3)+c(i,4,1,5)*ui(i1+4,i2+1,i3+5,c3)+c(i,5,1,
     & 5)*ui(i1+5,i2+1,i3+5,c3)+c(i,6,1,5)*ui(i1+6,i2+1,i3+5,c3)+c(i,
     & 7,1,5)*ui(i1+7,i2+1,i3+5,c3)+c(i,8,1,5)*ui(i1+8,i2+1,i3+5,c3)+
     & c(i,0,2,5)*ui(i1,i2+2,i3+5,c3)+c(i,1,2,5)*ui(i1+1,i2+2,i3+5,c3)
     & +c(i,2,2,5)*ui(i1+2,i2+2,i3+5,c3)+c(i,3,2,5)*ui(i1+3,i2+2,i3+5,
     & c3)+c(i,4,2,5)*ui(i1+4,i2+2,i3+5,c3)+c(i,5,2,5)*ui(i1+5,i2+2,
     & i3+5,c3)+c(i,6,2,5)*ui(i1+6,i2+2,i3+5,c3)+c(i,7,2,5)*ui(i1+7,
     & i2+2,i3+5,c3)+c(i,8,2,5)*ui(i1+8,i2+2,i3+5,c3)+c(i,0,3,5)*ui(
     & i1,i2+3,i3+5,c3)+c(i,1,3,5)*ui(i1+1,i2+3,i3+5,c3)+c(i,2,3,5)*
     & ui(i1+2,i2+3,i3+5,c3)+c(i,3,3,5)*ui(i1+3,i2+3,i3+5,c3)+c(i,4,3,
     & 5)*ui(i1+4,i2+3,i3+5,c3)+c(i,5,3,5)*ui(i1+5,i2+3,i3+5,c3)+c(i,
     & 6,3,5)*ui(i1+6,i2+3,i3+5,c3)+c(i,7,3,5)*ui(i1+7,i2+3,i3+5,c3)+
     & c(i,8,3,5)*ui(i1+8,i2+3,i3+5,c3)+c(i,0,4,5)*ui(i1,i2+4,i3+5,c3)
     & +c(i,1,4,5)*ui(i1+1,i2+4,i3+5,c3)+c(i,2,4,5)*ui(i1+2,i2+4,i3+5,
     & c3)+c(i,3,4,5)*ui(i1+3,i2+4,i3+5,c3)+c(i,4,4,5)*ui(i1+4,i2+4,
     & i3+5,c3)+c(i,5,4,5)*ui(i1+5,i2+4,i3+5,c3)+c(i,6,4,5)*ui(i1+6,
     & i2+4,i3+5,c3)+c(i,7,4,5)*ui(i1+7,i2+4,i3+5,c3)+c(i,8,4,5)*ui(
     & i1+8,i2+4,i3+5,c3)+c(i,0,5,5)*ui(i1,i2+5,i3+5,c3)+c(i,1,5,5)*
     & ui(i1+1,i2+5,i3+5,c3)+c(i,2,5,5)*ui(i1+2,i2+5,i3+5,c3)+c(i,3,5,
     & 5)*ui(i1+3,i2+5,i3+5,c3)+c(i,4,5,5)*ui(i1+4,i2+5,i3+5,c3)+c(i,
     & 5,5,5)*ui(i1+5,i2+5,i3+5,c3)+c(i,6,5,5)*ui(i1+6,i2+5,i3+5,c3)+
     & c(i,7,5,5)*ui(i1+7,i2+5,i3+5,c3)+c(i,8,5,5)*ui(i1+8,i2+5,i3+5,
     & c3)+c(i,0,6,5)*ui(i1,i2+6,i3+5,c3)+c(i,1,6,5)*ui(i1+1,i2+6,i3+
     & 5,c3)+c(i,2,6,5)*ui(i1+2,i2+6,i3+5,c3)+c(i,3,6,5)*ui(i1+3,i2+6,
     & i3+5,c3)+c(i,4,6,5)*ui(i1+4,i2+6,i3+5,c3)+c(i,5,6,5)*ui(i1+5,
     & i2+6,i3+5,c3)+c(i,6,6,5)*ui(i1+6,i2+6,i3+5,c3)+c(i,7,6,5)*ui(
     & i1+7,i2+6,i3+5,c3)+c(i,8,6,5)*ui(i1+8,i2+6,i3+5,c3)+c(i,0,7,5)*
     & ui(i1,i2+7,i3+5,c3)+c(i,1,7,5)*ui(i1+1,i2+7,i3+5,c3)+c(i,2,7,5)
     & *ui(i1+2,i2+7,i3+5,c3)+c(i,3,7,5)*ui(i1+3,i2+7,i3+5,c3)+c(i,4,
     & 7,5)*ui(i1+4,i2+7,i3+5,c3)+c(i,5,7,5)*ui(i1+5,i2+7,i3+5,c3)+c(
     & i,6,7,5)*ui(i1+6,i2+7,i3+5,c3)+c(i,7,7,5)*ui(i1+7,i2+7,i3+5,c3)
     & +c(i,8,7,5)*ui(i1+8,i2+7,i3+5,c3)+c(i,0,8,5)*ui(i1,i2+8,i3+5,
     & c3)+c(i,1,8,5)*ui(i1+1,i2+8,i3+5,c3)+c(i,2,8,5)*ui(i1+2,i2+8,
     & i3+5,c3)+c(i,3,8,5)*ui(i1+3,i2+8,i3+5,c3)+c(i,4,8,5)*ui(i1+4,
     & i2+8,i3+5,c3)+c(i,5,8,5)*ui(i1+5,i2+8,i3+5,c3)+c(i,6,8,5)*ui(
     & i1+6,i2+8,i3+5,c3)+c(i,7,8,5)*ui(i1+7,i2+8,i3+5,c3)+c(i,8,8,5)*
     & ui(i1+8,i2+8,i3+5,c3)+c(i,0,0,6)*ui(i1,i2,i3+6,c3)+c(i,1,0,6)*
     & ui(i1+1,i2,i3+6,c3)+c(i,2,0,6)*ui(i1+2,i2,i3+6,c3)+c(i,3,0,6)*
     & ui(i1+3,i2,i3+6,c3)+c(i,4,0,6)*ui(i1+4,i2,i3+6,c3)+c(i,5,0,6)*
     & ui(i1+5,i2,i3+6,c3)+c(i,6,0,6)*ui(i1+6,i2,i3+6,c3)+c(i,7,0,6)*
     & ui(i1+7,i2,i3+6,c3)+c(i,8,0,6)*ui(i1+8,i2,i3+6,c3)+c(i,0,1,6)*
     & ui(i1,i2+1,i3+6,c3)+c(i,1,1,6)*ui(i1+1,i2+1,i3+6,c3)+c(i,2,1,6)
     & *ui(i1+2,i2+1,i3+6,c3)+c(i,3,1,6)*ui(i1+3,i2+1,i3+6,c3)+c(i,4,
     & 1,6)*ui(i1+4,i2+1,i3+6,c3)+c(i,5,1,6)*ui(i1+5,i2+1,i3+6,c3)+c(
     & i,6,1,6)*ui(i1+6,i2+1,i3+6,c3)+c(i,7,1,6)*ui(i1+7,i2+1,i3+6,c3)
     & +c(i,8,1,6)*ui(i1+8,i2+1,i3+6,c3)+c(i,0,2,6)*ui(i1,i2+2,i3+6,
     & c3)+c(i,1,2,6)*ui(i1+1,i2+2,i3+6,c3)+c(i,2,2,6)*ui(i1+2,i2+2,
     & i3+6,c3)+c(i,3,2,6)*ui(i1+3,i2+2,i3+6,c3)+c(i,4,2,6)*ui(i1+4,
     & i2+2,i3+6,c3)+c(i,5,2,6)*ui(i1+5,i2+2,i3+6,c3)+c(i,6,2,6)*ui(
     & i1+6,i2+2,i3+6,c3)+c(i,7,2,6)*ui(i1+7,i2+2,i3+6,c3)+c(i,8,2,6)*
     & ui(i1+8,i2+2,i3+6,c3)+c(i,0,3,6)*ui(i1,i2+3,i3+6,c3)+c(i,1,3,6)
     & *ui(i1+1,i2+3,i3+6,c3)+c(i,2,3,6)*ui(i1+2,i2+3,i3+6,c3)+c(i,3,
     & 3,6)*ui(i1+3,i2+3,i3+6,c3)+c(i,4,3,6)*ui(i1+4,i2+3,i3+6,c3)+c(
     & i,5,3,6)*ui(i1+5,i2+3,i3+6,c3)+c(i,6,3,6)*ui(i1+6,i2+3,i3+6,c3)
     & +c(i,7,3,6)*ui(i1+7,i2+3,i3+6,c3)+c(i,8,3,6)*ui(i1+8,i2+3,i3+6,
     & c3)+c(i,0,4,6)*ui(i1,i2+4,i3+6,c3)+c(i,1,4,6)*ui(i1+1,i2+4,i3+
     & 6,c3)+c(i,2,4,6)*ui(i1+2,i2+4,i3+6,c3)+c(i,3,4,6)*ui(i1+3,i2+4,
     & i3+6,c3)+c(i,4,4,6)*ui(i1+4,i2+4,i3+6,c3)+c(i,5,4,6)*ui(i1+5,
     & i2+4,i3+6,c3)+c(i,6,4,6)*ui(i1+6,i2+4,i3+6,c3)+c(i,7,4,6)*ui(
     & i1+7,i2+4,i3+6,c3)+c(i,8,4,6)*ui(i1+8,i2+4,i3+6,c3)+c(i,0,5,6)*
     & ui(i1,i2+5,i3+6,c3)+c(i,1,5,6)*ui(i1+1,i2+5,i3+6,c3)+c(i,2,5,6)
     & *ui(i1+2,i2+5,i3+6,c3)+c(i,3,5,6)*ui(i1+3,i2+5,i3+6,c3)+c(i,4,
     & 5,6)*ui(i1+4,i2+5,i3+6,c3)+c(i,5,5,6)*ui(i1+5,i2+5,i3+6,c3)+c(
     & i,6,5,6)*ui(i1+6,i2+5,i3+6,c3)+c(i,7,5,6)*ui(i1+7,i2+5,i3+6,c3)
     & +c(i,8,5,6)*ui(i1+8,i2+5,i3+6,c3)+c(i,0,6,6)*ui(i1,i2+6,i3+6,
     & c3)+c(i,1,6,6)*ui(i1+1,i2+6,i3+6,c3)+c(i,2,6,6)*ui(i1+2,i2+6,
     & i3+6,c3)+c(i,3,6,6)*ui(i1+3,i2+6,i3+6,c3)+c(i,4,6,6)*ui(i1+4,
     & i2+6,i3+6,c3)+c(i,5,6,6)*ui(i1+5,i2+6,i3+6,c3)+c(i,6,6,6)*ui(
     & i1+6,i2+6,i3+6,c3)+c(i,7,6,6)*ui(i1+7,i2+6,i3+6,c3)+c(i,8,6,6)*
     & ui(i1+8,i2+6,i3+6,c3)+c(i,0,7,6)*ui(i1,i2+7,i3+6,c3)+c(i,1,7,6)
     & *ui(i1+1,i2+7,i3+6,c3)+c(i,2,7,6)*ui(i1+2,i2+7,i3+6,c3)
               r(i) = r(i)+c(i,3,7,6)*ui(i1+3,i2+7,i3+6,c3)+c(i,4,7,6)*
     & ui(i1+4,i2+7,i3+6,c3)+c(i,5,7,6)*ui(i1+5,i2+7,i3+6,c3)+c(i,6,7,
     & 6)*ui(i1+6,i2+7,i3+6,c3)+c(i,7,7,6)*ui(i1+7,i2+7,i3+6,c3)+c(i,
     & 8,7,6)*ui(i1+8,i2+7,i3+6,c3)+c(i,0,8,6)*ui(i1,i2+8,i3+6,c3)+c(
     & i,1,8,6)*ui(i1+1,i2+8,i3+6,c3)+c(i,2,8,6)*ui(i1+2,i2+8,i3+6,c3)
     & +c(i,3,8,6)*ui(i1+3,i2+8,i3+6,c3)+c(i,4,8,6)*ui(i1+4,i2+8,i3+6,
     & c3)+c(i,5,8,6)*ui(i1+5,i2+8,i3+6,c3)+c(i,6,8,6)*ui(i1+6,i2+8,
     & i3+6,c3)+c(i,7,8,6)*ui(i1+7,i2+8,i3+6,c3)+c(i,8,8,6)*ui(i1+8,
     & i2+8,i3+6,c3)+c(i,0,0,7)*ui(i1,i2,i3+7,c3)+c(i,1,0,7)*ui(i1+1,
     & i2,i3+7,c3)+c(i,2,0,7)*ui(i1+2,i2,i3+7,c3)+c(i,3,0,7)*ui(i1+3,
     & i2,i3+7,c3)+c(i,4,0,7)*ui(i1+4,i2,i3+7,c3)+c(i,5,0,7)*ui(i1+5,
     & i2,i3+7,c3)+c(i,6,0,7)*ui(i1+6,i2,i3+7,c3)+c(i,7,0,7)*ui(i1+7,
     & i2,i3+7,c3)+c(i,8,0,7)*ui(i1+8,i2,i3+7,c3)+c(i,0,1,7)*ui(i1,i2+
     & 1,i3+7,c3)+c(i,1,1,7)*ui(i1+1,i2+1,i3+7,c3)+c(i,2,1,7)*ui(i1+2,
     & i2+1,i3+7,c3)+c(i,3,1,7)*ui(i1+3,i2+1,i3+7,c3)+c(i,4,1,7)*ui(
     & i1+4,i2+1,i3+7,c3)+c(i,5,1,7)*ui(i1+5,i2+1,i3+7,c3)+c(i,6,1,7)*
     & ui(i1+6,i2+1,i3+7,c3)+c(i,7,1,7)*ui(i1+7,i2+1,i3+7,c3)+c(i,8,1,
     & 7)*ui(i1+8,i2+1,i3+7,c3)+c(i,0,2,7)*ui(i1,i2+2,i3+7,c3)+c(i,1,
     & 2,7)*ui(i1+1,i2+2,i3+7,c3)+c(i,2,2,7)*ui(i1+2,i2+2,i3+7,c3)+c(
     & i,3,2,7)*ui(i1+3,i2+2,i3+7,c3)+c(i,4,2,7)*ui(i1+4,i2+2,i3+7,c3)
     & +c(i,5,2,7)*ui(i1+5,i2+2,i3+7,c3)+c(i,6,2,7)*ui(i1+6,i2+2,i3+7,
     & c3)+c(i,7,2,7)*ui(i1+7,i2+2,i3+7,c3)+c(i,8,2,7)*ui(i1+8,i2+2,
     & i3+7,c3)+c(i,0,3,7)*ui(i1,i2+3,i3+7,c3)+c(i,1,3,7)*ui(i1+1,i2+
     & 3,i3+7,c3)+c(i,2,3,7)*ui(i1+2,i2+3,i3+7,c3)+c(i,3,3,7)*ui(i1+3,
     & i2+3,i3+7,c3)+c(i,4,3,7)*ui(i1+4,i2+3,i3+7,c3)+c(i,5,3,7)*ui(
     & i1+5,i2+3,i3+7,c3)+c(i,6,3,7)*ui(i1+6,i2+3,i3+7,c3)+c(i,7,3,7)*
     & ui(i1+7,i2+3,i3+7,c3)+c(i,8,3,7)*ui(i1+8,i2+3,i3+7,c3)+c(i,0,4,
     & 7)*ui(i1,i2+4,i3+7,c3)+c(i,1,4,7)*ui(i1+1,i2+4,i3+7,c3)+c(i,2,
     & 4,7)*ui(i1+2,i2+4,i3+7,c3)+c(i,3,4,7)*ui(i1+3,i2+4,i3+7,c3)+c(
     & i,4,4,7)*ui(i1+4,i2+4,i3+7,c3)+c(i,5,4,7)*ui(i1+5,i2+4,i3+7,c3)
     & +c(i,6,4,7)*ui(i1+6,i2+4,i3+7,c3)+c(i,7,4,7)*ui(i1+7,i2+4,i3+7,
     & c3)+c(i,8,4,7)*ui(i1+8,i2+4,i3+7,c3)+c(i,0,5,7)*ui(i1,i2+5,i3+
     & 7,c3)+c(i,1,5,7)*ui(i1+1,i2+5,i3+7,c3)+c(i,2,5,7)*ui(i1+2,i2+5,
     & i3+7,c3)+c(i,3,5,7)*ui(i1+3,i2+5,i3+7,c3)+c(i,4,5,7)*ui(i1+4,
     & i2+5,i3+7,c3)+c(i,5,5,7)*ui(i1+5,i2+5,i3+7,c3)+c(i,6,5,7)*ui(
     & i1+6,i2+5,i3+7,c3)+c(i,7,5,7)*ui(i1+7,i2+5,i3+7,c3)+c(i,8,5,7)*
     & ui(i1+8,i2+5,i3+7,c3)+c(i,0,6,7)*ui(i1,i2+6,i3+7,c3)+c(i,1,6,7)
     & *ui(i1+1,i2+6,i3+7,c3)+c(i,2,6,7)*ui(i1+2,i2+6,i3+7,c3)+c(i,3,
     & 6,7)*ui(i1+3,i2+6,i3+7,c3)+c(i,4,6,7)*ui(i1+4,i2+6,i3+7,c3)+c(
     & i,5,6,7)*ui(i1+5,i2+6,i3+7,c3)+c(i,6,6,7)*ui(i1+6,i2+6,i3+7,c3)
     & +c(i,7,6,7)*ui(i1+7,i2+6,i3+7,c3)+c(i,8,6,7)*ui(i1+8,i2+6,i3+7,
     & c3)+c(i,0,7,7)*ui(i1,i2+7,i3+7,c3)+c(i,1,7,7)*ui(i1+1,i2+7,i3+
     & 7,c3)+c(i,2,7,7)*ui(i1+2,i2+7,i3+7,c3)+c(i,3,7,7)*ui(i1+3,i2+7,
     & i3+7,c3)+c(i,4,7,7)*ui(i1+4,i2+7,i3+7,c3)+c(i,5,7,7)*ui(i1+5,
     & i2+7,i3+7,c3)+c(i,6,7,7)*ui(i1+6,i2+7,i3+7,c3)+c(i,7,7,7)*ui(
     & i1+7,i2+7,i3+7,c3)+c(i,8,7,7)*ui(i1+8,i2+7,i3+7,c3)+c(i,0,8,7)*
     & ui(i1,i2+8,i3+7,c3)+c(i,1,8,7)*ui(i1+1,i2+8,i3+7,c3)+c(i,2,8,7)
     & *ui(i1+2,i2+8,i3+7,c3)+c(i,3,8,7)*ui(i1+3,i2+8,i3+7,c3)+c(i,4,
     & 8,7)*ui(i1+4,i2+8,i3+7,c3)+c(i,5,8,7)*ui(i1+5,i2+8,i3+7,c3)+c(
     & i,6,8,7)*ui(i1+6,i2+8,i3+7,c3)+c(i,7,8,7)*ui(i1+7,i2+8,i3+7,c3)
     & +c(i,8,8,7)*ui(i1+8,i2+8,i3+7,c3)+c(i,0,0,8)*ui(i1,i2,i3+8,c3)+
     & c(i,1,0,8)*ui(i1+1,i2,i3+8,c3)+c(i,2,0,8)*ui(i1+2,i2,i3+8,c3)+
     & c(i,3,0,8)*ui(i1+3,i2,i3+8,c3)+c(i,4,0,8)*ui(i1+4,i2,i3+8,c3)+
     & c(i,5,0,8)*ui(i1+5,i2,i3+8,c3)+c(i,6,0,8)*ui(i1+6,i2,i3+8,c3)+
     & c(i,7,0,8)*ui(i1+7,i2,i3+8,c3)+c(i,8,0,8)*ui(i1+8,i2,i3+8,c3)+
     & c(i,0,1,8)*ui(i1,i2+1,i3+8,c3)+c(i,1,1,8)*ui(i1+1,i2+1,i3+8,c3)
     & +c(i,2,1,8)*ui(i1+2,i2+1,i3+8,c3)+c(i,3,1,8)*ui(i1+3,i2+1,i3+8,
     & c3)+c(i,4,1,8)*ui(i1+4,i2+1,i3+8,c3)+c(i,5,1,8)*ui(i1+5,i2+1,
     & i3+8,c3)+c(i,6,1,8)*ui(i1+6,i2+1,i3+8,c3)+c(i,7,1,8)*ui(i1+7,
     & i2+1,i3+8,c3)+c(i,8,1,8)*ui(i1+8,i2+1,i3+8,c3)+c(i,0,2,8)*ui(
     & i1,i2+2,i3+8,c3)+c(i,1,2,8)*ui(i1+1,i2+2,i3+8,c3)+c(i,2,2,8)*
     & ui(i1+2,i2+2,i3+8,c3)+c(i,3,2,8)*ui(i1+3,i2+2,i3+8,c3)+c(i,4,2,
     & 8)*ui(i1+4,i2+2,i3+8,c3)+c(i,5,2,8)*ui(i1+5,i2+2,i3+8,c3)+c(i,
     & 6,2,8)*ui(i1+6,i2+2,i3+8,c3)+c(i,7,2,8)*ui(i1+7,i2+2,i3+8,c3)+
     & c(i,8,2,8)*ui(i1+8,i2+2,i3+8,c3)+c(i,0,3,8)*ui(i1,i2+3,i3+8,c3)
     & +c(i,1,3,8)*ui(i1+1,i2+3,i3+8,c3)+c(i,2,3,8)*ui(i1+2,i2+3,i3+8,
     & c3)+c(i,3,3,8)*ui(i1+3,i2+3,i3+8,c3)+c(i,4,3,8)*ui(i1+4,i2+3,
     & i3+8,c3)+c(i,5,3,8)*ui(i1+5,i2+3,i3+8,c3)+c(i,6,3,8)*ui(i1+6,
     & i2+3,i3+8,c3)+c(i,7,3,8)*ui(i1+7,i2+3,i3+8,c3)+c(i,8,3,8)*ui(
     & i1+8,i2+3,i3+8,c3)+c(i,0,4,8)*ui(i1,i2+4,i3+8,c3)+c(i,1,4,8)*
     & ui(i1+1,i2+4,i3+8,c3)+c(i,2,4,8)*ui(i1+2,i2+4,i3+8,c3)+c(i,3,4,
     & 8)*ui(i1+3,i2+4,i3+8,c3)+c(i,4,4,8)*ui(i1+4,i2+4,i3+8,c3)+c(i,
     & 5,4,8)*ui(i1+5,i2+4,i3+8,c3)+c(i,6,4,8)*ui(i1+6,i2+4,i3+8,c3)+
     & c(i,7,4,8)*ui(i1+7,i2+4,i3+8,c3)+c(i,8,4,8)*ui(i1+8,i2+4,i3+8,
     & c3)+c(i,0,5,8)*ui(i1,i2+5,i3+8,c3)+c(i,1,5,8)*ui(i1+1,i2+5,i3+
     & 8,c3)+c(i,2,5,8)*ui(i1+2,i2+5,i3+8,c3)+c(i,3,5,8)*ui(i1+3,i2+5,
     & i3+8,c3)+c(i,4,5,8)*ui(i1+4,i2+5,i3+8,c3)+c(i,5,5,8)*ui(i1+5,
     & i2+5,i3+8,c3)+c(i,6,5,8)*ui(i1+6,i2+5,i3+8,c3)+c(i,7,5,8)*ui(
     & i1+7,i2+5,i3+8,c3)+c(i,8,5,8)*ui(i1+8,i2+5,i3+8,c3)+c(i,0,6,8)*
     & ui(i1,i2+6,i3+8,c3)+c(i,1,6,8)*ui(i1+1,i2+6,i3+8,c3)+c(i,2,6,8)
     & *ui(i1+2,i2+6,i3+8,c3)+c(i,3,6,8)*ui(i1+3,i2+6,i3+8,c3)+c(i,4,
     & 6,8)*ui(i1+4,i2+6,i3+8,c3)+c(i,5,6,8)*ui(i1+5,i2+6,i3+8,c3)+c(
     & i,6,6,8)*ui(i1+6,i2+6,i3+8,c3)+c(i,7,6,8)*ui(i1+7,i2+6,i3+8,c3)
     & +c(i,8,6,8)*ui(i1+8,i2+6,i3+8,c3)+c(i,0,7,8)*ui(i1,i2+7,i3+8,
     & c3)+c(i,1,7,8)*ui(i1+1,i2+7,i3+8,c3)+c(i,2,7,8)*ui(i1+2,i2+7,
     & i3+8,c3)+c(i,3,7,8)*ui(i1+3,i2+7,i3+8,c3)+c(i,4,7,8)*ui(i1+4,
     & i2+7,i3+8,c3)+c(i,5,7,8)*ui(i1+5,i2+7,i3+8,c3)+c(i,6,7,8)*ui(
     & i1+6,i2+7,i3+8,c3)+c(i,7,7,8)*ui(i1+7,i2+7,i3+8,c3)+c(i,8,7,8)*
     & ui(i1+8,i2+7,i3+8,c3)+c(i,0,8,8)*ui(i1,i2+8,i3+8,c3)+c(i,1,8,8)
     & *ui(i1+1,i2+8,i3+8,c3)+c(i,2,8,8)*ui(i1+2,i2+8,i3+8,c3)+c(i,3,
     & 8,8)*ui(i1+3,i2+8,i3+8,c3)+c(i,4,8,8)*ui(i1+4,i2+8,i3+8,c3)+c(
     & i,5,8,8)*ui(i1+5,i2+8,i3+8,c3)+c(i,6,8,8)*ui(i1+6,i2+8,i3+8,c3)
     & +c(i,7,8,8)*ui(i1+7,i2+8,i3+8,c3)+c(i,8,8,8)*ui(i1+8,i2+8,i3+8,
     & c3)
             else if( varWidth(i).eq.8 )then
! interp888(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,0,0)*ui(i1,i2,i3,c3)+c(i,1,0,0)*ui(i1+1,i2,
     & i3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3,c3)+c(i,3,0,0)*ui(i1+3,i2,i3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2,i3,c3)+c(i,5,0,0)*ui(i1+5,i2,i3,c3)+
     & c(i,6,0,0)*ui(i1+6,i2,i3,c3)+c(i,7,0,0)*ui(i1+7,i2,i3,c3)+c(i,
     & 0,1,0)*ui(i1,i2+1,i3,c3)+c(i,1,1,0)*ui(i1+1,i2+1,i3,c3)+c(i,2,
     & 1,0)*ui(i1+2,i2+1,i3,c3)+c(i,3,1,0)*ui(i1+3,i2+1,i3,c3)+c(i,4,
     & 1,0)*ui(i1+4,i2+1,i3,c3)+c(i,5,1,0)*ui(i1+5,i2+1,i3,c3)+c(i,6,
     & 1,0)*ui(i1+6,i2+1,i3,c3)+c(i,7,1,0)*ui(i1+7,i2+1,i3,c3)+c(i,0,
     & 2,0)*ui(i1,i2+2,i3,c3)+c(i,1,2,0)*ui(i1+1,i2+2,i3,c3)+c(i,2,2,
     & 0)*ui(i1+2,i2+2,i3,c3)+c(i,3,2,0)*ui(i1+3,i2+2,i3,c3)+c(i,4,2,
     & 0)*ui(i1+4,i2+2,i3,c3)+c(i,5,2,0)*ui(i1+5,i2+2,i3,c3)+c(i,6,2,
     & 0)*ui(i1+6,i2+2,i3,c3)+c(i,7,2,0)*ui(i1+7,i2+2,i3,c3)+c(i,0,3,
     & 0)*ui(i1,i2+3,i3,c3)+c(i,1,3,0)*ui(i1+1,i2+3,i3,c3)+c(i,2,3,0)*
     & ui(i1+2,i2+3,i3,c3)+c(i,3,3,0)*ui(i1+3,i2+3,i3,c3)+c(i,4,3,0)*
     & ui(i1+4,i2+3,i3,c3)+c(i,5,3,0)*ui(i1+5,i2+3,i3,c3)+c(i,6,3,0)*
     & ui(i1+6,i2+3,i3,c3)+c(i,7,3,0)*ui(i1+7,i2+3,i3,c3)+c(i,0,4,0)*
     & ui(i1,i2+4,i3,c3)+c(i,1,4,0)*ui(i1+1,i2+4,i3,c3)+c(i,2,4,0)*ui(
     & i1+2,i2+4,i3,c3)+c(i,3,4,0)*ui(i1+3,i2+4,i3,c3)+c(i,4,4,0)*ui(
     & i1+4,i2+4,i3,c3)+c(i,5,4,0)*ui(i1+5,i2+4,i3,c3)+c(i,6,4,0)*ui(
     & i1+6,i2+4,i3,c3)+c(i,7,4,0)*ui(i1+7,i2+4,i3,c3)+c(i,0,5,0)*ui(
     & i1,i2+5,i3,c3)+c(i,1,5,0)*ui(i1+1,i2+5,i3,c3)+c(i,2,5,0)*ui(i1+
     & 2,i2+5,i3,c3)+c(i,3,5,0)*ui(i1+3,i2+5,i3,c3)+c(i,4,5,0)*ui(i1+
     & 4,i2+5,i3,c3)+c(i,5,5,0)*ui(i1+5,i2+5,i3,c3)+c(i,6,5,0)*ui(i1+
     & 6,i2+5,i3,c3)+c(i,7,5,0)*ui(i1+7,i2+5,i3,c3)+c(i,0,6,0)*ui(i1,
     & i2+6,i3,c3)+c(i,1,6,0)*ui(i1+1,i2+6,i3,c3)+c(i,2,6,0)*ui(i1+2,
     & i2+6,i3,c3)+c(i,3,6,0)*ui(i1+3,i2+6,i3,c3)+c(i,4,6,0)*ui(i1+4,
     & i2+6,i3,c3)+c(i,5,6,0)*ui(i1+5,i2+6,i3,c3)+c(i,6,6,0)*ui(i1+6,
     & i2+6,i3,c3)+c(i,7,6,0)*ui(i1+7,i2+6,i3,c3)+c(i,0,7,0)*ui(i1,i2+
     & 7,i3,c3)+c(i,1,7,0)*ui(i1+1,i2+7,i3,c3)+c(i,2,7,0)*ui(i1+2,i2+
     & 7,i3,c3)+c(i,3,7,0)*ui(i1+3,i2+7,i3,c3)+c(i,4,7,0)*ui(i1+4,i2+
     & 7,i3,c3)+c(i,5,7,0)*ui(i1+5,i2+7,i3,c3)+c(i,6,7,0)*ui(i1+6,i2+
     & 7,i3,c3)+c(i,7,7,0)*ui(i1+7,i2+7,i3,c3)+c(i,0,0,1)*ui(i1,i2,i3+
     & 1,c3)+c(i,1,0,1)*ui(i1+1,i2,i3+1,c3)+c(i,2,0,1)*ui(i1+2,i2,i3+
     & 1,c3)+c(i,3,0,1)*ui(i1+3,i2,i3+1,c3)+c(i,4,0,1)*ui(i1+4,i2,i3+
     & 1,c3)+c(i,5,0,1)*ui(i1+5,i2,i3+1,c3)+c(i,6,0,1)*ui(i1+6,i2,i3+
     & 1,c3)+c(i,7,0,1)*ui(i1+7,i2,i3+1,c3)+c(i,0,1,1)*ui(i1,i2+1,i3+
     & 1,c3)+c(i,1,1,1)*ui(i1+1,i2+1,i3+1,c3)+c(i,2,1,1)*ui(i1+2,i2+1,
     & i3+1,c3)+c(i,3,1,1)*ui(i1+3,i2+1,i3+1,c3)+c(i,4,1,1)*ui(i1+4,
     & i2+1,i3+1,c3)+c(i,5,1,1)*ui(i1+5,i2+1,i3+1,c3)+c(i,6,1,1)*ui(
     & i1+6,i2+1,i3+1,c3)+c(i,7,1,1)*ui(i1+7,i2+1,i3+1,c3)+c(i,0,2,1)*
     & ui(i1,i2+2,i3+1,c3)+c(i,1,2,1)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,2,1)
     & *ui(i1+2,i2+2,i3+1,c3)+c(i,3,2,1)*ui(i1+3,i2+2,i3+1,c3)+c(i,4,
     & 2,1)*ui(i1+4,i2+2,i3+1,c3)+c(i,5,2,1)*ui(i1+5,i2+2,i3+1,c3)+c(
     & i,6,2,1)*ui(i1+6,i2+2,i3+1,c3)+c(i,7,2,1)*ui(i1+7,i2+2,i3+1,c3)
     & +c(i,0,3,1)*ui(i1,i2+3,i3+1,c3)+c(i,1,3,1)*ui(i1+1,i2+3,i3+1,
     & c3)+c(i,2,3,1)*ui(i1+2,i2+3,i3+1,c3)+c(i,3,3,1)*ui(i1+3,i2+3,
     & i3+1,c3)+c(i,4,3,1)*ui(i1+4,i2+3,i3+1,c3)+c(i,5,3,1)*ui(i1+5,
     & i2+3,i3+1,c3)+c(i,6,3,1)*ui(i1+6,i2+3,i3+1,c3)+c(i,7,3,1)*ui(
     & i1+7,i2+3,i3+1,c3)+c(i,0,4,1)*ui(i1,i2+4,i3+1,c3)+c(i,1,4,1)*
     & ui(i1+1,i2+4,i3+1,c3)+c(i,2,4,1)*ui(i1+2,i2+4,i3+1,c3)+c(i,3,4,
     & 1)*ui(i1+3,i2+4,i3+1,c3)+c(i,4,4,1)*ui(i1+4,i2+4,i3+1,c3)+c(i,
     & 5,4,1)*ui(i1+5,i2+4,i3+1,c3)+c(i,6,4,1)*ui(i1+6,i2+4,i3+1,c3)+
     & c(i,7,4,1)*ui(i1+7,i2+4,i3+1,c3)+c(i,0,5,1)*ui(i1,i2+5,i3+1,c3)
     & +c(i,1,5,1)*ui(i1+1,i2+5,i3+1,c3)+c(i,2,5,1)*ui(i1+2,i2+5,i3+1,
     & c3)+c(i,3,5,1)*ui(i1+3,i2+5,i3+1,c3)+c(i,4,5,1)*ui(i1+4,i2+5,
     & i3+1,c3)+c(i,5,5,1)*ui(i1+5,i2+5,i3+1,c3)+c(i,6,5,1)*ui(i1+6,
     & i2+5,i3+1,c3)+c(i,7,5,1)*ui(i1+7,i2+5,i3+1,c3)+c(i,0,6,1)*ui(
     & i1,i2+6,i3+1,c3)+c(i,1,6,1)*ui(i1+1,i2+6,i3+1,c3)+c(i,2,6,1)*
     & ui(i1+2,i2+6,i3+1,c3)+c(i,3,6,1)*ui(i1+3,i2+6,i3+1,c3)+c(i,4,6,
     & 1)*ui(i1+4,i2+6,i3+1,c3)+c(i,5,6,1)*ui(i1+5,i2+6,i3+1,c3)+c(i,
     & 6,6,1)*ui(i1+6,i2+6,i3+1,c3)+c(i,7,6,1)*ui(i1+7,i2+6,i3+1,c3)+
     & c(i,0,7,1)*ui(i1,i2+7,i3+1,c3)+c(i,1,7,1)*ui(i1+1,i2+7,i3+1,c3)
     & +c(i,2,7,1)*ui(i1+2,i2+7,i3+1,c3)+c(i,3,7,1)*ui(i1+3,i2+7,i3+1,
     & c3)+c(i,4,7,1)*ui(i1+4,i2+7,i3+1,c3)+c(i,5,7,1)*ui(i1+5,i2+7,
     & i3+1,c3)+c(i,6,7,1)*ui(i1+6,i2+7,i3+1,c3)+c(i,7,7,1)*ui(i1+7,
     & i2+7,i3+1,c3)+c(i,0,0,2)*ui(i1,i2,i3+2,c3)+c(i,1,0,2)*ui(i1+1,
     & i2,i3+2,c3)+c(i,2,0,2)*ui(i1+2,i2,i3+2,c3)+c(i,3,0,2)*ui(i1+3,
     & i2,i3+2,c3)+c(i,4,0,2)*ui(i1+4,i2,i3+2,c3)+c(i,5,0,2)*ui(i1+5,
     & i2,i3+2,c3)+c(i,6,0,2)*ui(i1+6,i2,i3+2,c3)+c(i,7,0,2)*ui(i1+7,
     & i2,i3+2,c3)+c(i,0,1,2)*ui(i1,i2+1,i3+2,c3)+c(i,1,1,2)*ui(i1+1,
     & i2+1,i3+2,c3)+c(i,2,1,2)*ui(i1+2,i2+1,i3+2,c3)+c(i,3,1,2)*ui(
     & i1+3,i2+1,i3+2,c3)+c(i,4,1,2)*ui(i1+4,i2+1,i3+2,c3)+c(i,5,1,2)*
     & ui(i1+5,i2+1,i3+2,c3)+c(i,6,1,2)*ui(i1+6,i2+1,i3+2,c3)+c(i,7,1,
     & 2)*ui(i1+7,i2+1,i3+2,c3)+c(i,0,2,2)*ui(i1,i2+2,i3+2,c3)+c(i,1,
     & 2,2)*ui(i1+1,i2+2,i3+2,c3)+c(i,2,2,2)*ui(i1+2,i2+2,i3+2,c3)+c(
     & i,3,2,2)*ui(i1+3,i2+2,i3+2,c3)+c(i,4,2,2)*ui(i1+4,i2+2,i3+2,c3)
     & +c(i,5,2,2)*ui(i1+5,i2+2,i3+2,c3)+c(i,6,2,2)*ui(i1+6,i2+2,i3+2,
     & c3)+c(i,7,2,2)*ui(i1+7,i2+2,i3+2,c3)+c(i,0,3,2)*ui(i1,i2+3,i3+
     & 2,c3)+c(i,1,3,2)*ui(i1+1,i2+3,i3+2,c3)+c(i,2,3,2)*ui(i1+2,i2+3,
     & i3+2,c3)+c(i,3,3,2)*ui(i1+3,i2+3,i3+2,c3)+c(i,4,3,2)*ui(i1+4,
     & i2+3,i3+2,c3)+c(i,5,3,2)*ui(i1+5,i2+3,i3+2,c3)+c(i,6,3,2)*ui(
     & i1+6,i2+3,i3+2,c3)+c(i,7,3,2)*ui(i1+7,i2+3,i3+2,c3)+c(i,0,4,2)*
     & ui(i1,i2+4,i3+2,c3)+c(i,1,4,2)*ui(i1+1,i2+4,i3+2,c3)+c(i,2,4,2)
     & *ui(i1+2,i2+4,i3+2,c3)+c(i,3,4,2)*ui(i1+3,i2+4,i3+2,c3)+c(i,4,
     & 4,2)*ui(i1+4,i2+4,i3+2,c3)+c(i,5,4,2)*ui(i1+5,i2+4,i3+2,c3)+c(
     & i,6,4,2)*ui(i1+6,i2+4,i3+2,c3)+c(i,7,4,2)*ui(i1+7,i2+4,i3+2,c3)
     & +c(i,0,5,2)*ui(i1,i2+5,i3+2,c3)+c(i,1,5,2)*ui(i1+1,i2+5,i3+2,
     & c3)+c(i,2,5,2)*ui(i1+2,i2+5,i3+2,c3)+c(i,3,5,2)*ui(i1+3,i2+5,
     & i3+2,c3)+c(i,4,5,2)*ui(i1+4,i2+5,i3+2,c3)+c(i,5,5,2)*ui(i1+5,
     & i2+5,i3+2,c3)+c(i,6,5,2)*ui(i1+6,i2+5,i3+2,c3)+c(i,7,5,2)*ui(
     & i1+7,i2+5,i3+2,c3)+c(i,0,6,2)*ui(i1,i2+6,i3+2,c3)+c(i,1,6,2)*
     & ui(i1+1,i2+6,i3+2,c3)+c(i,2,6,2)*ui(i1+2,i2+6,i3+2,c3)+c(i,3,6,
     & 2)*ui(i1+3,i2+6,i3+2,c3)+c(i,4,6,2)*ui(i1+4,i2+6,i3+2,c3)+c(i,
     & 5,6,2)*ui(i1+5,i2+6,i3+2,c3)+c(i,6,6,2)*ui(i1+6,i2+6,i3+2,c3)+
     & c(i,7,6,2)*ui(i1+7,i2+6,i3+2,c3)
               r(i) = r(i)+c(i,0,7,2)*ui(i1,i2+7,i3+2,c3)+c(i,1,7,2)*
     & ui(i1+1,i2+7,i3+2,c3)+c(i,2,7,2)*ui(i1+2,i2+7,i3+2,c3)+c(i,3,7,
     & 2)*ui(i1+3,i2+7,i3+2,c3)+c(i,4,7,2)*ui(i1+4,i2+7,i3+2,c3)+c(i,
     & 5,7,2)*ui(i1+5,i2+7,i3+2,c3)+c(i,6,7,2)*ui(i1+6,i2+7,i3+2,c3)+
     & c(i,7,7,2)*ui(i1+7,i2+7,i3+2,c3)+c(i,0,0,3)*ui(i1,i2,i3+3,c3)+
     & c(i,1,0,3)*ui(i1+1,i2,i3+3,c3)+c(i,2,0,3)*ui(i1+2,i2,i3+3,c3)+
     & c(i,3,0,3)*ui(i1+3,i2,i3+3,c3)+c(i,4,0,3)*ui(i1+4,i2,i3+3,c3)+
     & c(i,5,0,3)*ui(i1+5,i2,i3+3,c3)+c(i,6,0,3)*ui(i1+6,i2,i3+3,c3)+
     & c(i,7,0,3)*ui(i1+7,i2,i3+3,c3)+c(i,0,1,3)*ui(i1,i2+1,i3+3,c3)+
     & c(i,1,1,3)*ui(i1+1,i2+1,i3+3,c3)+c(i,2,1,3)*ui(i1+2,i2+1,i3+3,
     & c3)+c(i,3,1,3)*ui(i1+3,i2+1,i3+3,c3)+c(i,4,1,3)*ui(i1+4,i2+1,
     & i3+3,c3)+c(i,5,1,3)*ui(i1+5,i2+1,i3+3,c3)+c(i,6,1,3)*ui(i1+6,
     & i2+1,i3+3,c3)+c(i,7,1,3)*ui(i1+7,i2+1,i3+3,c3)+c(i,0,2,3)*ui(
     & i1,i2+2,i3+3,c3)+c(i,1,2,3)*ui(i1+1,i2+2,i3+3,c3)+c(i,2,2,3)*
     & ui(i1+2,i2+2,i3+3,c3)+c(i,3,2,3)*ui(i1+3,i2+2,i3+3,c3)+c(i,4,2,
     & 3)*ui(i1+4,i2+2,i3+3,c3)+c(i,5,2,3)*ui(i1+5,i2+2,i3+3,c3)+c(i,
     & 6,2,3)*ui(i1+6,i2+2,i3+3,c3)+c(i,7,2,3)*ui(i1+7,i2+2,i3+3,c3)+
     & c(i,0,3,3)*ui(i1,i2+3,i3+3,c3)+c(i,1,3,3)*ui(i1+1,i2+3,i3+3,c3)
     & +c(i,2,3,3)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,3,3)*ui(i1+3,i2+3,i3+3,
     & c3)+c(i,4,3,3)*ui(i1+4,i2+3,i3+3,c3)+c(i,5,3,3)*ui(i1+5,i2+3,
     & i3+3,c3)+c(i,6,3,3)*ui(i1+6,i2+3,i3+3,c3)+c(i,7,3,3)*ui(i1+7,
     & i2+3,i3+3,c3)+c(i,0,4,3)*ui(i1,i2+4,i3+3,c3)+c(i,1,4,3)*ui(i1+
     & 1,i2+4,i3+3,c3)+c(i,2,4,3)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,4,3)*ui(
     & i1+3,i2+4,i3+3,c3)+c(i,4,4,3)*ui(i1+4,i2+4,i3+3,c3)+c(i,5,4,3)*
     & ui(i1+5,i2+4,i3+3,c3)+c(i,6,4,3)*ui(i1+6,i2+4,i3+3,c3)+c(i,7,4,
     & 3)*ui(i1+7,i2+4,i3+3,c3)+c(i,0,5,3)*ui(i1,i2+5,i3+3,c3)+c(i,1,
     & 5,3)*ui(i1+1,i2+5,i3+3,c3)+c(i,2,5,3)*ui(i1+2,i2+5,i3+3,c3)+c(
     & i,3,5,3)*ui(i1+3,i2+5,i3+3,c3)+c(i,4,5,3)*ui(i1+4,i2+5,i3+3,c3)
     & +c(i,5,5,3)*ui(i1+5,i2+5,i3+3,c3)+c(i,6,5,3)*ui(i1+6,i2+5,i3+3,
     & c3)+c(i,7,5,3)*ui(i1+7,i2+5,i3+3,c3)+c(i,0,6,3)*ui(i1,i2+6,i3+
     & 3,c3)+c(i,1,6,3)*ui(i1+1,i2+6,i3+3,c3)+c(i,2,6,3)*ui(i1+2,i2+6,
     & i3+3,c3)+c(i,3,6,3)*ui(i1+3,i2+6,i3+3,c3)+c(i,4,6,3)*ui(i1+4,
     & i2+6,i3+3,c3)+c(i,5,6,3)*ui(i1+5,i2+6,i3+3,c3)+c(i,6,6,3)*ui(
     & i1+6,i2+6,i3+3,c3)+c(i,7,6,3)*ui(i1+7,i2+6,i3+3,c3)+c(i,0,7,3)*
     & ui(i1,i2+7,i3+3,c3)+c(i,1,7,3)*ui(i1+1,i2+7,i3+3,c3)+c(i,2,7,3)
     & *ui(i1+2,i2+7,i3+3,c3)+c(i,3,7,3)*ui(i1+3,i2+7,i3+3,c3)+c(i,4,
     & 7,3)*ui(i1+4,i2+7,i3+3,c3)+c(i,5,7,3)*ui(i1+5,i2+7,i3+3,c3)+c(
     & i,6,7,3)*ui(i1+6,i2+7,i3+3,c3)+c(i,7,7,3)*ui(i1+7,i2+7,i3+3,c3)
     & +c(i,0,0,4)*ui(i1,i2,i3+4,c3)+c(i,1,0,4)*ui(i1+1,i2,i3+4,c3)+c(
     & i,2,0,4)*ui(i1+2,i2,i3+4,c3)+c(i,3,0,4)*ui(i1+3,i2,i3+4,c3)+c(
     & i,4,0,4)*ui(i1+4,i2,i3+4,c3)+c(i,5,0,4)*ui(i1+5,i2,i3+4,c3)+c(
     & i,6,0,4)*ui(i1+6,i2,i3+4,c3)+c(i,7,0,4)*ui(i1+7,i2,i3+4,c3)+c(
     & i,0,1,4)*ui(i1,i2+1,i3+4,c3)+c(i,1,1,4)*ui(i1+1,i2+1,i3+4,c3)+
     & c(i,2,1,4)*ui(i1+2,i2+1,i3+4,c3)+c(i,3,1,4)*ui(i1+3,i2+1,i3+4,
     & c3)+c(i,4,1,4)*ui(i1+4,i2+1,i3+4,c3)+c(i,5,1,4)*ui(i1+5,i2+1,
     & i3+4,c3)+c(i,6,1,4)*ui(i1+6,i2+1,i3+4,c3)+c(i,7,1,4)*ui(i1+7,
     & i2+1,i3+4,c3)+c(i,0,2,4)*ui(i1,i2+2,i3+4,c3)+c(i,1,2,4)*ui(i1+
     & 1,i2+2,i3+4,c3)+c(i,2,2,4)*ui(i1+2,i2+2,i3+4,c3)+c(i,3,2,4)*ui(
     & i1+3,i2+2,i3+4,c3)+c(i,4,2,4)*ui(i1+4,i2+2,i3+4,c3)+c(i,5,2,4)*
     & ui(i1+5,i2+2,i3+4,c3)+c(i,6,2,4)*ui(i1+6,i2+2,i3+4,c3)+c(i,7,2,
     & 4)*ui(i1+7,i2+2,i3+4,c3)+c(i,0,3,4)*ui(i1,i2+3,i3+4,c3)+c(i,1,
     & 3,4)*ui(i1+1,i2+3,i3+4,c3)+c(i,2,3,4)*ui(i1+2,i2+3,i3+4,c3)+c(
     & i,3,3,4)*ui(i1+3,i2+3,i3+4,c3)+c(i,4,3,4)*ui(i1+4,i2+3,i3+4,c3)
     & +c(i,5,3,4)*ui(i1+5,i2+3,i3+4,c3)+c(i,6,3,4)*ui(i1+6,i2+3,i3+4,
     & c3)+c(i,7,3,4)*ui(i1+7,i2+3,i3+4,c3)+c(i,0,4,4)*ui(i1,i2+4,i3+
     & 4,c3)+c(i,1,4,4)*ui(i1+1,i2+4,i3+4,c3)+c(i,2,4,4)*ui(i1+2,i2+4,
     & i3+4,c3)+c(i,3,4,4)*ui(i1+3,i2+4,i3+4,c3)+c(i,4,4,4)*ui(i1+4,
     & i2+4,i3+4,c3)+c(i,5,4,4)*ui(i1+5,i2+4,i3+4,c3)+c(i,6,4,4)*ui(
     & i1+6,i2+4,i3+4,c3)+c(i,7,4,4)*ui(i1+7,i2+4,i3+4,c3)+c(i,0,5,4)*
     & ui(i1,i2+5,i3+4,c3)+c(i,1,5,4)*ui(i1+1,i2+5,i3+4,c3)+c(i,2,5,4)
     & *ui(i1+2,i2+5,i3+4,c3)+c(i,3,5,4)*ui(i1+3,i2+5,i3+4,c3)+c(i,4,
     & 5,4)*ui(i1+4,i2+5,i3+4,c3)+c(i,5,5,4)*ui(i1+5,i2+5,i3+4,c3)+c(
     & i,6,5,4)*ui(i1+6,i2+5,i3+4,c3)+c(i,7,5,4)*ui(i1+7,i2+5,i3+4,c3)
     & +c(i,0,6,4)*ui(i1,i2+6,i3+4,c3)+c(i,1,6,4)*ui(i1+1,i2+6,i3+4,
     & c3)+c(i,2,6,4)*ui(i1+2,i2+6,i3+4,c3)+c(i,3,6,4)*ui(i1+3,i2+6,
     & i3+4,c3)+c(i,4,6,4)*ui(i1+4,i2+6,i3+4,c3)+c(i,5,6,4)*ui(i1+5,
     & i2+6,i3+4,c3)+c(i,6,6,4)*ui(i1+6,i2+6,i3+4,c3)+c(i,7,6,4)*ui(
     & i1+7,i2+6,i3+4,c3)+c(i,0,7,4)*ui(i1,i2+7,i3+4,c3)+c(i,1,7,4)*
     & ui(i1+1,i2+7,i3+4,c3)+c(i,2,7,4)*ui(i1+2,i2+7,i3+4,c3)+c(i,3,7,
     & 4)*ui(i1+3,i2+7,i3+4,c3)+c(i,4,7,4)*ui(i1+4,i2+7,i3+4,c3)+c(i,
     & 5,7,4)*ui(i1+5,i2+7,i3+4,c3)+c(i,6,7,4)*ui(i1+6,i2+7,i3+4,c3)+
     & c(i,7,7,4)*ui(i1+7,i2+7,i3+4,c3)+c(i,0,0,5)*ui(i1,i2,i3+5,c3)+
     & c(i,1,0,5)*ui(i1+1,i2,i3+5,c3)+c(i,2,0,5)*ui(i1+2,i2,i3+5,c3)+
     & c(i,3,0,5)*ui(i1+3,i2,i3+5,c3)+c(i,4,0,5)*ui(i1+4,i2,i3+5,c3)+
     & c(i,5,0,5)*ui(i1+5,i2,i3+5,c3)+c(i,6,0,5)*ui(i1+6,i2,i3+5,c3)+
     & c(i,7,0,5)*ui(i1+7,i2,i3+5,c3)+c(i,0,1,5)*ui(i1,i2+1,i3+5,c3)+
     & c(i,1,1,5)*ui(i1+1,i2+1,i3+5,c3)+c(i,2,1,5)*ui(i1+2,i2+1,i3+5,
     & c3)+c(i,3,1,5)*ui(i1+3,i2+1,i3+5,c3)+c(i,4,1,5)*ui(i1+4,i2+1,
     & i3+5,c3)+c(i,5,1,5)*ui(i1+5,i2+1,i3+5,c3)+c(i,6,1,5)*ui(i1+6,
     & i2+1,i3+5,c3)+c(i,7,1,5)*ui(i1+7,i2+1,i3+5,c3)+c(i,0,2,5)*ui(
     & i1,i2+2,i3+5,c3)+c(i,1,2,5)*ui(i1+1,i2+2,i3+5,c3)+c(i,2,2,5)*
     & ui(i1+2,i2+2,i3+5,c3)+c(i,3,2,5)*ui(i1+3,i2+2,i3+5,c3)+c(i,4,2,
     & 5)*ui(i1+4,i2+2,i3+5,c3)+c(i,5,2,5)*ui(i1+5,i2+2,i3+5,c3)+c(i,
     & 6,2,5)*ui(i1+6,i2+2,i3+5,c3)+c(i,7,2,5)*ui(i1+7,i2+2,i3+5,c3)+
     & c(i,0,3,5)*ui(i1,i2+3,i3+5,c3)+c(i,1,3,5)*ui(i1+1,i2+3,i3+5,c3)
     & +c(i,2,3,5)*ui(i1+2,i2+3,i3+5,c3)+c(i,3,3,5)*ui(i1+3,i2+3,i3+5,
     & c3)+c(i,4,3,5)*ui(i1+4,i2+3,i3+5,c3)+c(i,5,3,5)*ui(i1+5,i2+3,
     & i3+5,c3)+c(i,6,3,5)*ui(i1+6,i2+3,i3+5,c3)+c(i,7,3,5)*ui(i1+7,
     & i2+3,i3+5,c3)+c(i,0,4,5)*ui(i1,i2+4,i3+5,c3)+c(i,1,4,5)*ui(i1+
     & 1,i2+4,i3+5,c3)+c(i,2,4,5)*ui(i1+2,i2+4,i3+5,c3)+c(i,3,4,5)*ui(
     & i1+3,i2+4,i3+5,c3)+c(i,4,4,5)*ui(i1+4,i2+4,i3+5,c3)+c(i,5,4,5)*
     & ui(i1+5,i2+4,i3+5,c3)+c(i,6,4,5)*ui(i1+6,i2+4,i3+5,c3)+c(i,7,4,
     & 5)*ui(i1+7,i2+4,i3+5,c3)+c(i,0,5,5)*ui(i1,i2+5,i3+5,c3)+c(i,1,
     & 5,5)*ui(i1+1,i2+5,i3+5,c3)+c(i,2,5,5)*ui(i1+2,i2+5,i3+5,c3)+c(
     & i,3,5,5)*ui(i1+3,i2+5,i3+5,c3)+c(i,4,5,5)*ui(i1+4,i2+5,i3+5,c3)
     & +c(i,5,5,5)*ui(i1+5,i2+5,i3+5,c3)+c(i,6,5,5)*ui(i1+6,i2+5,i3+5,
     & c3)+c(i,7,5,5)*ui(i1+7,i2+5,i3+5,c3)
               r(i) = r(i)+c(i,0,6,5)*ui(i1,i2+6,i3+5,c3)+c(i,1,6,5)*
     & ui(i1+1,i2+6,i3+5,c3)+c(i,2,6,5)*ui(i1+2,i2+6,i3+5,c3)+c(i,3,6,
     & 5)*ui(i1+3,i2+6,i3+5,c3)+c(i,4,6,5)*ui(i1+4,i2+6,i3+5,c3)+c(i,
     & 5,6,5)*ui(i1+5,i2+6,i3+5,c3)+c(i,6,6,5)*ui(i1+6,i2+6,i3+5,c3)+
     & c(i,7,6,5)*ui(i1+7,i2+6,i3+5,c3)+c(i,0,7,5)*ui(i1,i2+7,i3+5,c3)
     & +c(i,1,7,5)*ui(i1+1,i2+7,i3+5,c3)+c(i,2,7,5)*ui(i1+2,i2+7,i3+5,
     & c3)+c(i,3,7,5)*ui(i1+3,i2+7,i3+5,c3)+c(i,4,7,5)*ui(i1+4,i2+7,
     & i3+5,c3)+c(i,5,7,5)*ui(i1+5,i2+7,i3+5,c3)+c(i,6,7,5)*ui(i1+6,
     & i2+7,i3+5,c3)+c(i,7,7,5)*ui(i1+7,i2+7,i3+5,c3)+c(i,0,0,6)*ui(
     & i1,i2,i3+6,c3)+c(i,1,0,6)*ui(i1+1,i2,i3+6,c3)+c(i,2,0,6)*ui(i1+
     & 2,i2,i3+6,c3)+c(i,3,0,6)*ui(i1+3,i2,i3+6,c3)+c(i,4,0,6)*ui(i1+
     & 4,i2,i3+6,c3)+c(i,5,0,6)*ui(i1+5,i2,i3+6,c3)+c(i,6,0,6)*ui(i1+
     & 6,i2,i3+6,c3)+c(i,7,0,6)*ui(i1+7,i2,i3+6,c3)+c(i,0,1,6)*ui(i1,
     & i2+1,i3+6,c3)+c(i,1,1,6)*ui(i1+1,i2+1,i3+6,c3)+c(i,2,1,6)*ui(
     & i1+2,i2+1,i3+6,c3)+c(i,3,1,6)*ui(i1+3,i2+1,i3+6,c3)+c(i,4,1,6)*
     & ui(i1+4,i2+1,i3+6,c3)+c(i,5,1,6)*ui(i1+5,i2+1,i3+6,c3)+c(i,6,1,
     & 6)*ui(i1+6,i2+1,i3+6,c3)+c(i,7,1,6)*ui(i1+7,i2+1,i3+6,c3)+c(i,
     & 0,2,6)*ui(i1,i2+2,i3+6,c3)+c(i,1,2,6)*ui(i1+1,i2+2,i3+6,c3)+c(
     & i,2,2,6)*ui(i1+2,i2+2,i3+6,c3)+c(i,3,2,6)*ui(i1+3,i2+2,i3+6,c3)
     & +c(i,4,2,6)*ui(i1+4,i2+2,i3+6,c3)+c(i,5,2,6)*ui(i1+5,i2+2,i3+6,
     & c3)+c(i,6,2,6)*ui(i1+6,i2+2,i3+6,c3)+c(i,7,2,6)*ui(i1+7,i2+2,
     & i3+6,c3)+c(i,0,3,6)*ui(i1,i2+3,i3+6,c3)+c(i,1,3,6)*ui(i1+1,i2+
     & 3,i3+6,c3)+c(i,2,3,6)*ui(i1+2,i2+3,i3+6,c3)+c(i,3,3,6)*ui(i1+3,
     & i2+3,i3+6,c3)+c(i,4,3,6)*ui(i1+4,i2+3,i3+6,c3)+c(i,5,3,6)*ui(
     & i1+5,i2+3,i3+6,c3)+c(i,6,3,6)*ui(i1+6,i2+3,i3+6,c3)+c(i,7,3,6)*
     & ui(i1+7,i2+3,i3+6,c3)+c(i,0,4,6)*ui(i1,i2+4,i3+6,c3)+c(i,1,4,6)
     & *ui(i1+1,i2+4,i3+6,c3)+c(i,2,4,6)*ui(i1+2,i2+4,i3+6,c3)+c(i,3,
     & 4,6)*ui(i1+3,i2+4,i3+6,c3)+c(i,4,4,6)*ui(i1+4,i2+4,i3+6,c3)+c(
     & i,5,4,6)*ui(i1+5,i2+4,i3+6,c3)+c(i,6,4,6)*ui(i1+6,i2+4,i3+6,c3)
     & +c(i,7,4,6)*ui(i1+7,i2+4,i3+6,c3)+c(i,0,5,6)*ui(i1,i2+5,i3+6,
     & c3)+c(i,1,5,6)*ui(i1+1,i2+5,i3+6,c3)+c(i,2,5,6)*ui(i1+2,i2+5,
     & i3+6,c3)+c(i,3,5,6)*ui(i1+3,i2+5,i3+6,c3)+c(i,4,5,6)*ui(i1+4,
     & i2+5,i3+6,c3)+c(i,5,5,6)*ui(i1+5,i2+5,i3+6,c3)+c(i,6,5,6)*ui(
     & i1+6,i2+5,i3+6,c3)+c(i,7,5,6)*ui(i1+7,i2+5,i3+6,c3)+c(i,0,6,6)*
     & ui(i1,i2+6,i3+6,c3)+c(i,1,6,6)*ui(i1+1,i2+6,i3+6,c3)+c(i,2,6,6)
     & *ui(i1+2,i2+6,i3+6,c3)+c(i,3,6,6)*ui(i1+3,i2+6,i3+6,c3)+c(i,4,
     & 6,6)*ui(i1+4,i2+6,i3+6,c3)+c(i,5,6,6)*ui(i1+5,i2+6,i3+6,c3)+c(
     & i,6,6,6)*ui(i1+6,i2+6,i3+6,c3)+c(i,7,6,6)*ui(i1+7,i2+6,i3+6,c3)
     & +c(i,0,7,6)*ui(i1,i2+7,i3+6,c3)+c(i,1,7,6)*ui(i1+1,i2+7,i3+6,
     & c3)+c(i,2,7,6)*ui(i1+2,i2+7,i3+6,c3)+c(i,3,7,6)*ui(i1+3,i2+7,
     & i3+6,c3)+c(i,4,7,6)*ui(i1+4,i2+7,i3+6,c3)+c(i,5,7,6)*ui(i1+5,
     & i2+7,i3+6,c3)+c(i,6,7,6)*ui(i1+6,i2+7,i3+6,c3)+c(i,7,7,6)*ui(
     & i1+7,i2+7,i3+6,c3)+c(i,0,0,7)*ui(i1,i2,i3+7,c3)+c(i,1,0,7)*ui(
     & i1+1,i2,i3+7,c3)+c(i,2,0,7)*ui(i1+2,i2,i3+7,c3)+c(i,3,0,7)*ui(
     & i1+3,i2,i3+7,c3)+c(i,4,0,7)*ui(i1+4,i2,i3+7,c3)+c(i,5,0,7)*ui(
     & i1+5,i2,i3+7,c3)+c(i,6,0,7)*ui(i1+6,i2,i3+7,c3)+c(i,7,0,7)*ui(
     & i1+7,i2,i3+7,c3)+c(i,0,1,7)*ui(i1,i2+1,i3+7,c3)+c(i,1,1,7)*ui(
     & i1+1,i2+1,i3+7,c3)+c(i,2,1,7)*ui(i1+2,i2+1,i3+7,c3)+c(i,3,1,7)*
     & ui(i1+3,i2+1,i3+7,c3)+c(i,4,1,7)*ui(i1+4,i2+1,i3+7,c3)+c(i,5,1,
     & 7)*ui(i1+5,i2+1,i3+7,c3)+c(i,6,1,7)*ui(i1+6,i2+1,i3+7,c3)+c(i,
     & 7,1,7)*ui(i1+7,i2+1,i3+7,c3)+c(i,0,2,7)*ui(i1,i2+2,i3+7,c3)+c(
     & i,1,2,7)*ui(i1+1,i2+2,i3+7,c3)+c(i,2,2,7)*ui(i1+2,i2+2,i3+7,c3)
     & +c(i,3,2,7)*ui(i1+3,i2+2,i3+7,c3)+c(i,4,2,7)*ui(i1+4,i2+2,i3+7,
     & c3)+c(i,5,2,7)*ui(i1+5,i2+2,i3+7,c3)+c(i,6,2,7)*ui(i1+6,i2+2,
     & i3+7,c3)+c(i,7,2,7)*ui(i1+7,i2+2,i3+7,c3)+c(i,0,3,7)*ui(i1,i2+
     & 3,i3+7,c3)+c(i,1,3,7)*ui(i1+1,i2+3,i3+7,c3)+c(i,2,3,7)*ui(i1+2,
     & i2+3,i3+7,c3)+c(i,3,3,7)*ui(i1+3,i2+3,i3+7,c3)+c(i,4,3,7)*ui(
     & i1+4,i2+3,i3+7,c3)+c(i,5,3,7)*ui(i1+5,i2+3,i3+7,c3)+c(i,6,3,7)*
     & ui(i1+6,i2+3,i3+7,c3)+c(i,7,3,7)*ui(i1+7,i2+3,i3+7,c3)+c(i,0,4,
     & 7)*ui(i1,i2+4,i3+7,c3)+c(i,1,4,7)*ui(i1+1,i2+4,i3+7,c3)+c(i,2,
     & 4,7)*ui(i1+2,i2+4,i3+7,c3)+c(i,3,4,7)*ui(i1+3,i2+4,i3+7,c3)+c(
     & i,4,4,7)*ui(i1+4,i2+4,i3+7,c3)+c(i,5,4,7)*ui(i1+5,i2+4,i3+7,c3)
     & +c(i,6,4,7)*ui(i1+6,i2+4,i3+7,c3)+c(i,7,4,7)*ui(i1+7,i2+4,i3+7,
     & c3)+c(i,0,5,7)*ui(i1,i2+5,i3+7,c3)+c(i,1,5,7)*ui(i1+1,i2+5,i3+
     & 7,c3)+c(i,2,5,7)*ui(i1+2,i2+5,i3+7,c3)+c(i,3,5,7)*ui(i1+3,i2+5,
     & i3+7,c3)+c(i,4,5,7)*ui(i1+4,i2+5,i3+7,c3)+c(i,5,5,7)*ui(i1+5,
     & i2+5,i3+7,c3)+c(i,6,5,7)*ui(i1+6,i2+5,i3+7,c3)+c(i,7,5,7)*ui(
     & i1+7,i2+5,i3+7,c3)+c(i,0,6,7)*ui(i1,i2+6,i3+7,c3)+c(i,1,6,7)*
     & ui(i1+1,i2+6,i3+7,c3)+c(i,2,6,7)*ui(i1+2,i2+6,i3+7,c3)+c(i,3,6,
     & 7)*ui(i1+3,i2+6,i3+7,c3)+c(i,4,6,7)*ui(i1+4,i2+6,i3+7,c3)+c(i,
     & 5,6,7)*ui(i1+5,i2+6,i3+7,c3)+c(i,6,6,7)*ui(i1+6,i2+6,i3+7,c3)+
     & c(i,7,6,7)*ui(i1+7,i2+6,i3+7,c3)+c(i,0,7,7)*ui(i1,i2+7,i3+7,c3)
     & +c(i,1,7,7)*ui(i1+1,i2+7,i3+7,c3)+c(i,2,7,7)*ui(i1+2,i2+7,i3+7,
     & c3)+c(i,3,7,7)*ui(i1+3,i2+7,i3+7,c3)+c(i,4,7,7)*ui(i1+4,i2+7,
     & i3+7,c3)+c(i,5,7,7)*ui(i1+5,i2+7,i3+7,c3)+c(i,6,7,7)*ui(i1+6,
     & i2+7,i3+7,c3)+c(i,7,7,7)*ui(i1+7,i2+7,i3+7,c3)
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
! loops3d($interp333(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))
           if( c3a.eq.c3b )then
             do c3=c3a,c3b
             do i=nia,nib
! interp333(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,0,0)*ui(i1,i2,i3,c3)+c(i,1,0,0)*ui(i1+1,i2,
     & i3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3,c3)+c(i,0,1,0)*ui(i1,i2+1,i3,
     & c3)+c(i,1,1,0)*ui(i1+1,i2+1,i3,c3)+c(i,2,1,0)*ui(i1+2,i2+1,i3,
     & c3)+c(i,0,2,0)*ui(i1,i2+2,i3,c3)+c(i,1,2,0)*ui(i1+1,i2+2,i3,c3)
     & +c(i,2,2,0)*ui(i1+2,i2+2,i3,c3)+c(i,0,0,1)*ui(i1,i2,i3+1,c3)+c(
     & i,1,0,1)*ui(i1+1,i2,i3+1,c3)+c(i,2,0,1)*ui(i1+2,i2,i3+1,c3)+c(
     & i,0,1,1)*ui(i1,i2+1,i3+1,c3)+c(i,1,1,1)*ui(i1+1,i2+1,i3+1,c3)+
     & c(i,2,1,1)*ui(i1+2,i2+1,i3+1,c3)+c(i,0,2,1)*ui(i1,i2+2,i3+1,c3)
     & +c(i,1,2,1)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,2,1)*ui(i1+2,i2+2,i3+1,
     & c3)+c(i,0,0,2)*ui(i1,i2,i3+2,c3)+c(i,1,0,2)*ui(i1+1,i2,i3+2,c3)
     & +c(i,2,0,2)*ui(i1+2,i2,i3+2,c3)+c(i,0,1,2)*ui(i1,i2+1,i3+2,c3)+
     & c(i,1,1,2)*ui(i1+1,i2+1,i3+2,c3)+c(i,2,1,2)*ui(i1+2,i2+1,i3+2,
     & c3)+c(i,0,2,2)*ui(i1,i2+2,i3+2,c3)+c(i,1,2,2)*ui(i1+1,i2+2,i3+
     & 2,c3)+c(i,2,2,2)*ui(i1+2,i2+2,i3+2,c3)
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
! interp333(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,0,0)*ui(i1,i2,i3,c3)+c(i,1,0,0)*ui(i1+1,i2,
     & i3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3,c3)+c(i,0,1,0)*ui(i1,i2+1,i3,
     & c3)+c(i,1,1,0)*ui(i1+1,i2+1,i3,c3)+c(i,2,1,0)*ui(i1+2,i2+1,i3,
     & c3)+c(i,0,2,0)*ui(i1,i2+2,i3,c3)+c(i,1,2,0)*ui(i1+1,i2+2,i3,c3)
     & +c(i,2,2,0)*ui(i1+2,i2+2,i3,c3)+c(i,0,0,1)*ui(i1,i2,i3+1,c3)+c(
     & i,1,0,1)*ui(i1+1,i2,i3+1,c3)+c(i,2,0,1)*ui(i1+2,i2,i3+1,c3)+c(
     & i,0,1,1)*ui(i1,i2+1,i3+1,c3)+c(i,1,1,1)*ui(i1+1,i2+1,i3+1,c3)+
     & c(i,2,1,1)*ui(i1+2,i2+1,i3+1,c3)+c(i,0,2,1)*ui(i1,i2+2,i3+1,c3)
     & +c(i,1,2,1)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,2,1)*ui(i1+2,i2+2,i3+1,
     & c3)+c(i,0,0,2)*ui(i1,i2,i3+2,c3)+c(i,1,0,2)*ui(i1+1,i2,i3+2,c3)
     & +c(i,2,0,2)*ui(i1+2,i2,i3+2,c3)+c(i,0,1,2)*ui(i1,i2+1,i3+2,c3)+
     & c(i,1,1,2)*ui(i1+1,i2+1,i3+2,c3)+c(i,2,1,2)*ui(i1+2,i2+1,i3+2,
     & c3)+c(i,0,2,2)*ui(i1,i2+2,i3+2,c3)+c(i,1,2,2)*ui(i1+1,i2+2,i3+
     & 2,c3)+c(i,2,2,2)*ui(i1+2,i2+2,i3+2,c3)
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           end if
         else if( width(1).eq.1 .and. width(2).eq.1 .and. width(3)
     & .eq.1 )then
! loops3d($interp111(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))
           if( c3a.eq.c3b )then
             do c3=c3a,c3b
             do i=nia,nib
! interp111(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = ui(i1,i2,i3,c3)
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
! interp111(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = ui(i1,i2,i3,c3)
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           end if
         else if( width(1).eq.2 .and. width(2).eq.2 .and. width(3)
     & .eq.2 )then
! loops3d($interp222(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))
           if( c3a.eq.c3b )then
             do c3=c3a,c3b
             do i=nia,nib
! interp222(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,0,0)*ui(i1,i2,i3,c3)+c(i,1,0,0)*ui(i1+1,i2,
     & i3,c3)+c(i,0,1,0)*ui(i1,i2+1,i3,c3)+c(i,1,1,0)*ui(i1+1,i2+1,i3,
     & c3)+c(i,0,0,1)*ui(i1,i2,i3+1,c3)+c(i,1,0,1)*ui(i1+1,i2,i3+1,c3)
     & +c(i,0,1,1)*ui(i1,i2+1,i3+1,c3)+c(i,1,1,1)*ui(i1+1,i2+1,i3+1,
     & c3)
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
! interp222(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,0,0)*ui(i1,i2,i3,c3)+c(i,1,0,0)*ui(i1+1,i2,
     & i3,c3)+c(i,0,1,0)*ui(i1,i2+1,i3,c3)+c(i,1,1,0)*ui(i1+1,i2+1,i3,
     & c3)+c(i,0,0,1)*ui(i1,i2,i3+1,c3)+c(i,1,0,1)*ui(i1+1,i2,i3+1,c3)
     & +c(i,0,1,1)*ui(i1,i2+1,i3+1,c3)+c(i,1,1,1)*ui(i1+1,i2+1,i3+1,
     & c3)
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           end if
         else if( width(1).eq.4 .and. width(2).eq.4 .and. width(3)
     & .eq.4 )then
! loops3d($interp444(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))
           if( c3a.eq.c3b )then
             do c3=c3a,c3b
             do i=nia,nib
! interp444(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,0,0)*ui(i1,i2,i3,c3)+c(i,1,0,0)*ui(i1+1,i2,
     & i3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3,c3)+c(i,3,0,0)*ui(i1+3,i2,i3,
     & c3)+c(i,0,1,0)*ui(i1,i2+1,i3,c3)+c(i,1,1,0)*ui(i1+1,i2+1,i3,c3)
     & +c(i,2,1,0)*ui(i1+2,i2+1,i3,c3)+c(i,3,1,0)*ui(i1+3,i2+1,i3,c3)+
     & c(i,0,2,0)*ui(i1,i2+2,i3,c3)+c(i,1,2,0)*ui(i1+1,i2+2,i3,c3)+c(
     & i,2,2,0)*ui(i1+2,i2+2,i3,c3)+c(i,3,2,0)*ui(i1+3,i2+2,i3,c3)+c(
     & i,0,3,0)*ui(i1,i2+3,i3,c3)+c(i,1,3,0)*ui(i1+1,i2+3,i3,c3)+c(i,
     & 2,3,0)*ui(i1+2,i2+3,i3,c3)+c(i,3,3,0)*ui(i1+3,i2+3,i3,c3)+c(i,
     & 0,0,1)*ui(i1,i2,i3+1,c3)+c(i,1,0,1)*ui(i1+1,i2,i3+1,c3)+c(i,2,
     & 0,1)*ui(i1+2,i2,i3+1,c3)+c(i,3,0,1)*ui(i1+3,i2,i3+1,c3)+c(i,0,
     & 1,1)*ui(i1,i2+1,i3+1,c3)+c(i,1,1,1)*ui(i1+1,i2+1,i3+1,c3)+c(i,
     & 2,1,1)*ui(i1+2,i2+1,i3+1,c3)+c(i,3,1,1)*ui(i1+3,i2+1,i3+1,c3)+
     & c(i,0,2,1)*ui(i1,i2+2,i3+1,c3)+c(i,1,2,1)*ui(i1+1,i2+2,i3+1,c3)
     & +c(i,2,2,1)*ui(i1+2,i2+2,i3+1,c3)+c(i,3,2,1)*ui(i1+3,i2+2,i3+1,
     & c3)+c(i,0,3,1)*ui(i1,i2+3,i3+1,c3)+c(i,1,3,1)*ui(i1+1,i2+3,i3+
     & 1,c3)+c(i,2,3,1)*ui(i1+2,i2+3,i3+1,c3)+c(i,3,3,1)*ui(i1+3,i2+3,
     & i3+1,c3)+c(i,0,0,2)*ui(i1,i2,i3+2,c3)+c(i,1,0,2)*ui(i1+1,i2,i3+
     & 2,c3)+c(i,2,0,2)*ui(i1+2,i2,i3+2,c3)+c(i,3,0,2)*ui(i1+3,i2,i3+
     & 2,c3)+c(i,0,1,2)*ui(i1,i2+1,i3+2,c3)+c(i,1,1,2)*ui(i1+1,i2+1,
     & i3+2,c3)+c(i,2,1,2)*ui(i1+2,i2+1,i3+2,c3)+c(i,3,1,2)*ui(i1+3,
     & i2+1,i3+2,c3)+c(i,0,2,2)*ui(i1,i2+2,i3+2,c3)+c(i,1,2,2)*ui(i1+
     & 1,i2+2,i3+2,c3)+c(i,2,2,2)*ui(i1+2,i2+2,i3+2,c3)+c(i,3,2,2)*ui(
     & i1+3,i2+2,i3+2,c3)+c(i,0,3,2)*ui(i1,i2+3,i3+2,c3)+c(i,1,3,2)*
     & ui(i1+1,i2+3,i3+2,c3)+c(i,2,3,2)*ui(i1+2,i2+3,i3+2,c3)+c(i,3,3,
     & 2)*ui(i1+3,i2+3,i3+2,c3)+c(i,0,0,3)*ui(i1,i2,i3+3,c3)+c(i,1,0,
     & 3)*ui(i1+1,i2,i3+3,c3)+c(i,2,0,3)*ui(i1+2,i2,i3+3,c3)+c(i,3,0,
     & 3)*ui(i1+3,i2,i3+3,c3)+c(i,0,1,3)*ui(i1,i2+1,i3+3,c3)+c(i,1,1,
     & 3)*ui(i1+1,i2+1,i3+3,c3)+c(i,2,1,3)*ui(i1+2,i2+1,i3+3,c3)+c(i,
     & 3,1,3)*ui(i1+3,i2+1,i3+3,c3)+c(i,0,2,3)*ui(i1,i2+2,i3+3,c3)+c(
     & i,1,2,3)*ui(i1+1,i2+2,i3+3,c3)+c(i,2,2,3)*ui(i1+2,i2+2,i3+3,c3)
     & +c(i,3,2,3)*ui(i1+3,i2+2,i3+3,c3)+c(i,0,3,3)*ui(i1,i2+3,i3+3,
     & c3)+c(i,1,3,3)*ui(i1+1,i2+3,i3+3,c3)+c(i,2,3,3)*ui(i1+2,i2+3,
     & i3+3,c3)+c(i,3,3,3)*ui(i1+3,i2+3,i3+3,c3)
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
! interp444(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,0,0)*ui(i1,i2,i3,c3)+c(i,1,0,0)*ui(i1+1,i2,
     & i3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3,c3)+c(i,3,0,0)*ui(i1+3,i2,i3,
     & c3)+c(i,0,1,0)*ui(i1,i2+1,i3,c3)+c(i,1,1,0)*ui(i1+1,i2+1,i3,c3)
     & +c(i,2,1,0)*ui(i1+2,i2+1,i3,c3)+c(i,3,1,0)*ui(i1+3,i2+1,i3,c3)+
     & c(i,0,2,0)*ui(i1,i2+2,i3,c3)+c(i,1,2,0)*ui(i1+1,i2+2,i3,c3)+c(
     & i,2,2,0)*ui(i1+2,i2+2,i3,c3)+c(i,3,2,0)*ui(i1+3,i2+2,i3,c3)+c(
     & i,0,3,0)*ui(i1,i2+3,i3,c3)+c(i,1,3,0)*ui(i1+1,i2+3,i3,c3)+c(i,
     & 2,3,0)*ui(i1+2,i2+3,i3,c3)+c(i,3,3,0)*ui(i1+3,i2+3,i3,c3)+c(i,
     & 0,0,1)*ui(i1,i2,i3+1,c3)+c(i,1,0,1)*ui(i1+1,i2,i3+1,c3)+c(i,2,
     & 0,1)*ui(i1+2,i2,i3+1,c3)+c(i,3,0,1)*ui(i1+3,i2,i3+1,c3)+c(i,0,
     & 1,1)*ui(i1,i2+1,i3+1,c3)+c(i,1,1,1)*ui(i1+1,i2+1,i3+1,c3)+c(i,
     & 2,1,1)*ui(i1+2,i2+1,i3+1,c3)+c(i,3,1,1)*ui(i1+3,i2+1,i3+1,c3)+
     & c(i,0,2,1)*ui(i1,i2+2,i3+1,c3)+c(i,1,2,1)*ui(i1+1,i2+2,i3+1,c3)
     & +c(i,2,2,1)*ui(i1+2,i2+2,i3+1,c3)+c(i,3,2,1)*ui(i1+3,i2+2,i3+1,
     & c3)+c(i,0,3,1)*ui(i1,i2+3,i3+1,c3)+c(i,1,3,1)*ui(i1+1,i2+3,i3+
     & 1,c3)+c(i,2,3,1)*ui(i1+2,i2+3,i3+1,c3)+c(i,3,3,1)*ui(i1+3,i2+3,
     & i3+1,c3)+c(i,0,0,2)*ui(i1,i2,i3+2,c3)+c(i,1,0,2)*ui(i1+1,i2,i3+
     & 2,c3)+c(i,2,0,2)*ui(i1+2,i2,i3+2,c3)+c(i,3,0,2)*ui(i1+3,i2,i3+
     & 2,c3)+c(i,0,1,2)*ui(i1,i2+1,i3+2,c3)+c(i,1,1,2)*ui(i1+1,i2+1,
     & i3+2,c3)+c(i,2,1,2)*ui(i1+2,i2+1,i3+2,c3)+c(i,3,1,2)*ui(i1+3,
     & i2+1,i3+2,c3)+c(i,0,2,2)*ui(i1,i2+2,i3+2,c3)+c(i,1,2,2)*ui(i1+
     & 1,i2+2,i3+2,c3)+c(i,2,2,2)*ui(i1+2,i2+2,i3+2,c3)+c(i,3,2,2)*ui(
     & i1+3,i2+2,i3+2,c3)+c(i,0,3,2)*ui(i1,i2+3,i3+2,c3)+c(i,1,3,2)*
     & ui(i1+1,i2+3,i3+2,c3)+c(i,2,3,2)*ui(i1+2,i2+3,i3+2,c3)+c(i,3,3,
     & 2)*ui(i1+3,i2+3,i3+2,c3)+c(i,0,0,3)*ui(i1,i2,i3+3,c3)+c(i,1,0,
     & 3)*ui(i1+1,i2,i3+3,c3)+c(i,2,0,3)*ui(i1+2,i2,i3+3,c3)+c(i,3,0,
     & 3)*ui(i1+3,i2,i3+3,c3)+c(i,0,1,3)*ui(i1,i2+1,i3+3,c3)+c(i,1,1,
     & 3)*ui(i1+1,i2+1,i3+3,c3)+c(i,2,1,3)*ui(i1+2,i2+1,i3+3,c3)+c(i,
     & 3,1,3)*ui(i1+3,i2+1,i3+3,c3)+c(i,0,2,3)*ui(i1,i2+2,i3+3,c3)+c(
     & i,1,2,3)*ui(i1+1,i2+2,i3+3,c3)+c(i,2,2,3)*ui(i1+2,i2+2,i3+3,c3)
     & +c(i,3,2,3)*ui(i1+3,i2+2,i3+3,c3)+c(i,0,3,3)*ui(i1,i2+3,i3+3,
     & c3)+c(i,1,3,3)*ui(i1+1,i2+3,i3+3,c3)+c(i,2,3,3)*ui(i1+2,i2+3,
     & i3+3,c3)+c(i,3,3,3)*ui(i1+3,i2+3,i3+3,c3)
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           end if
         else if( width(1).eq.5 .and. width(2).eq.5 .and. width(3)
     & .eq.5 )then
! loops3d($interp555(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))
           if( c3a.eq.c3b )then
             do c3=c3a,c3b
             do i=nia,nib
! interp555(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,0,0)*ui(i1,i2,i3,c3)+c(i,1,0,0)*ui(i1+1,i2,
     & i3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3,c3)+c(i,3,0,0)*ui(i1+3,i2,i3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2,i3,c3)+c(i,0,1,0)*ui(i1,i2+1,i3,c3)+
     & c(i,1,1,0)*ui(i1+1,i2+1,i3,c3)+c(i,2,1,0)*ui(i1+2,i2+1,i3,c3)+
     & c(i,3,1,0)*ui(i1+3,i2+1,i3,c3)+c(i,4,1,0)*ui(i1+4,i2+1,i3,c3)+
     & c(i,0,2,0)*ui(i1,i2+2,i3,c3)+c(i,1,2,0)*ui(i1+1,i2+2,i3,c3)+c(
     & i,2,2,0)*ui(i1+2,i2+2,i3,c3)+c(i,3,2,0)*ui(i1+3,i2+2,i3,c3)+c(
     & i,4,2,0)*ui(i1+4,i2+2,i3,c3)+c(i,0,3,0)*ui(i1,i2+3,i3,c3)+c(i,
     & 1,3,0)*ui(i1+1,i2+3,i3,c3)+c(i,2,3,0)*ui(i1+2,i2+3,i3,c3)+c(i,
     & 3,3,0)*ui(i1+3,i2+3,i3,c3)+c(i,4,3,0)*ui(i1+4,i2+3,i3,c3)+c(i,
     & 0,4,0)*ui(i1,i2+4,i3,c3)+c(i,1,4,0)*ui(i1+1,i2+4,i3,c3)+c(i,2,
     & 4,0)*ui(i1+2,i2+4,i3,c3)+c(i,3,4,0)*ui(i1+3,i2+4,i3,c3)+c(i,4,
     & 4,0)*ui(i1+4,i2+4,i3,c3)+c(i,0,0,1)*ui(i1,i2,i3+1,c3)+c(i,1,0,
     & 1)*ui(i1+1,i2,i3+1,c3)+c(i,2,0,1)*ui(i1+2,i2,i3+1,c3)+c(i,3,0,
     & 1)*ui(i1+3,i2,i3+1,c3)+c(i,4,0,1)*ui(i1+4,i2,i3+1,c3)+c(i,0,1,
     & 1)*ui(i1,i2+1,i3+1,c3)+c(i,1,1,1)*ui(i1+1,i2+1,i3+1,c3)+c(i,2,
     & 1,1)*ui(i1+2,i2+1,i3+1,c3)+c(i,3,1,1)*ui(i1+3,i2+1,i3+1,c3)+c(
     & i,4,1,1)*ui(i1+4,i2+1,i3+1,c3)+c(i,0,2,1)*ui(i1,i2+2,i3+1,c3)+
     & c(i,1,2,1)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,2,1)*ui(i1+2,i2+2,i3+1,
     & c3)+c(i,3,2,1)*ui(i1+3,i2+2,i3+1,c3)+c(i,4,2,1)*ui(i1+4,i2+2,
     & i3+1,c3)+c(i,0,3,1)*ui(i1,i2+3,i3+1,c3)+c(i,1,3,1)*ui(i1+1,i2+
     & 3,i3+1,c3)+c(i,2,3,1)*ui(i1+2,i2+3,i3+1,c3)+c(i,3,3,1)*ui(i1+3,
     & i2+3,i3+1,c3)+c(i,4,3,1)*ui(i1+4,i2+3,i3+1,c3)+c(i,0,4,1)*ui(
     & i1,i2+4,i3+1,c3)+c(i,1,4,1)*ui(i1+1,i2+4,i3+1,c3)+c(i,2,4,1)*
     & ui(i1+2,i2+4,i3+1,c3)+c(i,3,4,1)*ui(i1+3,i2+4,i3+1,c3)+c(i,4,4,
     & 1)*ui(i1+4,i2+4,i3+1,c3)+c(i,0,0,2)*ui(i1,i2,i3+2,c3)+c(i,1,0,
     & 2)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,2)*ui(i1+2,i2,i3+2,c3)+c(i,3,0,
     & 2)*ui(i1+3,i2,i3+2,c3)+c(i,4,0,2)*ui(i1+4,i2,i3+2,c3)+c(i,0,1,
     & 2)*ui(i1,i2+1,i3+2,c3)+c(i,1,1,2)*ui(i1+1,i2+1,i3+2,c3)+c(i,2,
     & 1,2)*ui(i1+2,i2+1,i3+2,c3)+c(i,3,1,2)*ui(i1+3,i2+1,i3+2,c3)+c(
     & i,4,1,2)*ui(i1+4,i2+1,i3+2,c3)+c(i,0,2,2)*ui(i1,i2+2,i3+2,c3)+
     & c(i,1,2,2)*ui(i1+1,i2+2,i3+2,c3)+c(i,2,2,2)*ui(i1+2,i2+2,i3+2,
     & c3)+c(i,3,2,2)*ui(i1+3,i2+2,i3+2,c3)+c(i,4,2,2)*ui(i1+4,i2+2,
     & i3+2,c3)+c(i,0,3,2)*ui(i1,i2+3,i3+2,c3)+c(i,1,3,2)*ui(i1+1,i2+
     & 3,i3+2,c3)+c(i,2,3,2)*ui(i1+2,i2+3,i3+2,c3)+c(i,3,3,2)*ui(i1+3,
     & i2+3,i3+2,c3)+c(i,4,3,2)*ui(i1+4,i2+3,i3+2,c3)+c(i,0,4,2)*ui(
     & i1,i2+4,i3+2,c3)+c(i,1,4,2)*ui(i1+1,i2+4,i3+2,c3)+c(i,2,4,2)*
     & ui(i1+2,i2+4,i3+2,c3)+c(i,3,4,2)*ui(i1+3,i2+4,i3+2,c3)+c(i,4,4,
     & 2)*ui(i1+4,i2+4,i3+2,c3)+c(i,0,0,3)*ui(i1,i2,i3+3,c3)+c(i,1,0,
     & 3)*ui(i1+1,i2,i3+3,c3)+c(i,2,0,3)*ui(i1+2,i2,i3+3,c3)+c(i,3,0,
     & 3)*ui(i1+3,i2,i3+3,c3)+c(i,4,0,3)*ui(i1+4,i2,i3+3,c3)+c(i,0,1,
     & 3)*ui(i1,i2+1,i3+3,c3)+c(i,1,1,3)*ui(i1+1,i2+1,i3+3,c3)+c(i,2,
     & 1,3)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,1,3)*ui(i1+3,i2+1,i3+3,c3)+c(
     & i,4,1,3)*ui(i1+4,i2+1,i3+3,c3)+c(i,0,2,3)*ui(i1,i2+2,i3+3,c3)+
     & c(i,1,2,3)*ui(i1+1,i2+2,i3+3,c3)+c(i,2,2,3)*ui(i1+2,i2+2,i3+3,
     & c3)+c(i,3,2,3)*ui(i1+3,i2+2,i3+3,c3)+c(i,4,2,3)*ui(i1+4,i2+2,
     & i3+3,c3)+c(i,0,3,3)*ui(i1,i2+3,i3+3,c3)+c(i,1,3,3)*ui(i1+1,i2+
     & 3,i3+3,c3)+c(i,2,3,3)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,3,3)*ui(i1+3,
     & i2+3,i3+3,c3)+c(i,4,3,3)*ui(i1+4,i2+3,i3+3,c3)+c(i,0,4,3)*ui(
     & i1,i2+4,i3+3,c3)+c(i,1,4,3)*ui(i1+1,i2+4,i3+3,c3)+c(i,2,4,3)*
     & ui(i1+2,i2+4,i3+3,c3)+c(i,3,4,3)*ui(i1+3,i2+4,i3+3,c3)+c(i,4,4,
     & 3)*ui(i1+4,i2+4,i3+3,c3)+c(i,0,0,4)*ui(i1,i2,i3+4,c3)+c(i,1,0,
     & 4)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,4)*ui(i1+2,i2,i3+4,c3)+c(i,3,0,
     & 4)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,4)*ui(i1+4,i2,i3+4,c3)+c(i,0,1,
     & 4)*ui(i1,i2+1,i3+4,c3)+c(i,1,1,4)*ui(i1+1,i2+1,i3+4,c3)+c(i,2,
     & 1,4)*ui(i1+2,i2+1,i3+4,c3)+c(i,3,1,4)*ui(i1+3,i2+1,i3+4,c3)+c(
     & i,4,1,4)*ui(i1+4,i2+1,i3+4,c3)+c(i,0,2,4)*ui(i1,i2+2,i3+4,c3)+
     & c(i,1,2,4)*ui(i1+1,i2+2,i3+4,c3)+c(i,2,2,4)*ui(i1+2,i2+2,i3+4,
     & c3)+c(i,3,2,4)*ui(i1+3,i2+2,i3+4,c3)+c(i,4,2,4)*ui(i1+4,i2+2,
     & i3+4,c3)+c(i,0,3,4)*ui(i1,i2+3,i3+4,c3)+c(i,1,3,4)*ui(i1+1,i2+
     & 3,i3+4,c3)+c(i,2,3,4)*ui(i1+2,i2+3,i3+4,c3)+c(i,3,3,4)*ui(i1+3,
     & i2+3,i3+4,c3)+c(i,4,3,4)*ui(i1+4,i2+3,i3+4,c3)+c(i,0,4,4)*ui(
     & i1,i2+4,i3+4,c3)+c(i,1,4,4)*ui(i1+1,i2+4,i3+4,c3)+c(i,2,4,4)*
     & ui(i1+2,i2+4,i3+4,c3)+c(i,3,4,4)*ui(i1+3,i2+4,i3+4,c3)+c(i,4,4,
     & 4)*ui(i1+4,i2+4,i3+4,c3)
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
! interp555(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,0,0)*ui(i1,i2,i3,c3)+c(i,1,0,0)*ui(i1+1,i2,
     & i3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3,c3)+c(i,3,0,0)*ui(i1+3,i2,i3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2,i3,c3)+c(i,0,1,0)*ui(i1,i2+1,i3,c3)+
     & c(i,1,1,0)*ui(i1+1,i2+1,i3,c3)+c(i,2,1,0)*ui(i1+2,i2+1,i3,c3)+
     & c(i,3,1,0)*ui(i1+3,i2+1,i3,c3)+c(i,4,1,0)*ui(i1+4,i2+1,i3,c3)+
     & c(i,0,2,0)*ui(i1,i2+2,i3,c3)+c(i,1,2,0)*ui(i1+1,i2+2,i3,c3)+c(
     & i,2,2,0)*ui(i1+2,i2+2,i3,c3)+c(i,3,2,0)*ui(i1+3,i2+2,i3,c3)+c(
     & i,4,2,0)*ui(i1+4,i2+2,i3,c3)+c(i,0,3,0)*ui(i1,i2+3,i3,c3)+c(i,
     & 1,3,0)*ui(i1+1,i2+3,i3,c3)+c(i,2,3,0)*ui(i1+2,i2+3,i3,c3)+c(i,
     & 3,3,0)*ui(i1+3,i2+3,i3,c3)+c(i,4,3,0)*ui(i1+4,i2+3,i3,c3)+c(i,
     & 0,4,0)*ui(i1,i2+4,i3,c3)+c(i,1,4,0)*ui(i1+1,i2+4,i3,c3)+c(i,2,
     & 4,0)*ui(i1+2,i2+4,i3,c3)+c(i,3,4,0)*ui(i1+3,i2+4,i3,c3)+c(i,4,
     & 4,0)*ui(i1+4,i2+4,i3,c3)+c(i,0,0,1)*ui(i1,i2,i3+1,c3)+c(i,1,0,
     & 1)*ui(i1+1,i2,i3+1,c3)+c(i,2,0,1)*ui(i1+2,i2,i3+1,c3)+c(i,3,0,
     & 1)*ui(i1+3,i2,i3+1,c3)+c(i,4,0,1)*ui(i1+4,i2,i3+1,c3)+c(i,0,1,
     & 1)*ui(i1,i2+1,i3+1,c3)+c(i,1,1,1)*ui(i1+1,i2+1,i3+1,c3)+c(i,2,
     & 1,1)*ui(i1+2,i2+1,i3+1,c3)+c(i,3,1,1)*ui(i1+3,i2+1,i3+1,c3)+c(
     & i,4,1,1)*ui(i1+4,i2+1,i3+1,c3)+c(i,0,2,1)*ui(i1,i2+2,i3+1,c3)+
     & c(i,1,2,1)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,2,1)*ui(i1+2,i2+2,i3+1,
     & c3)+c(i,3,2,1)*ui(i1+3,i2+2,i3+1,c3)+c(i,4,2,1)*ui(i1+4,i2+2,
     & i3+1,c3)+c(i,0,3,1)*ui(i1,i2+3,i3+1,c3)+c(i,1,3,1)*ui(i1+1,i2+
     & 3,i3+1,c3)+c(i,2,3,1)*ui(i1+2,i2+3,i3+1,c3)+c(i,3,3,1)*ui(i1+3,
     & i2+3,i3+1,c3)+c(i,4,3,1)*ui(i1+4,i2+3,i3+1,c3)+c(i,0,4,1)*ui(
     & i1,i2+4,i3+1,c3)+c(i,1,4,1)*ui(i1+1,i2+4,i3+1,c3)+c(i,2,4,1)*
     & ui(i1+2,i2+4,i3+1,c3)+c(i,3,4,1)*ui(i1+3,i2+4,i3+1,c3)+c(i,4,4,
     & 1)*ui(i1+4,i2+4,i3+1,c3)+c(i,0,0,2)*ui(i1,i2,i3+2,c3)+c(i,1,0,
     & 2)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,2)*ui(i1+2,i2,i3+2,c3)+c(i,3,0,
     & 2)*ui(i1+3,i2,i3+2,c3)+c(i,4,0,2)*ui(i1+4,i2,i3+2,c3)+c(i,0,1,
     & 2)*ui(i1,i2+1,i3+2,c3)+c(i,1,1,2)*ui(i1+1,i2+1,i3+2,c3)+c(i,2,
     & 1,2)*ui(i1+2,i2+1,i3+2,c3)+c(i,3,1,2)*ui(i1+3,i2+1,i3+2,c3)+c(
     & i,4,1,2)*ui(i1+4,i2+1,i3+2,c3)+c(i,0,2,2)*ui(i1,i2+2,i3+2,c3)+
     & c(i,1,2,2)*ui(i1+1,i2+2,i3+2,c3)+c(i,2,2,2)*ui(i1+2,i2+2,i3+2,
     & c3)+c(i,3,2,2)*ui(i1+3,i2+2,i3+2,c3)+c(i,4,2,2)*ui(i1+4,i2+2,
     & i3+2,c3)+c(i,0,3,2)*ui(i1,i2+3,i3+2,c3)+c(i,1,3,2)*ui(i1+1,i2+
     & 3,i3+2,c3)+c(i,2,3,2)*ui(i1+2,i2+3,i3+2,c3)+c(i,3,3,2)*ui(i1+3,
     & i2+3,i3+2,c3)+c(i,4,3,2)*ui(i1+4,i2+3,i3+2,c3)+c(i,0,4,2)*ui(
     & i1,i2+4,i3+2,c3)+c(i,1,4,2)*ui(i1+1,i2+4,i3+2,c3)+c(i,2,4,2)*
     & ui(i1+2,i2+4,i3+2,c3)+c(i,3,4,2)*ui(i1+3,i2+4,i3+2,c3)+c(i,4,4,
     & 2)*ui(i1+4,i2+4,i3+2,c3)+c(i,0,0,3)*ui(i1,i2,i3+3,c3)+c(i,1,0,
     & 3)*ui(i1+1,i2,i3+3,c3)+c(i,2,0,3)*ui(i1+2,i2,i3+3,c3)+c(i,3,0,
     & 3)*ui(i1+3,i2,i3+3,c3)+c(i,4,0,3)*ui(i1+4,i2,i3+3,c3)+c(i,0,1,
     & 3)*ui(i1,i2+1,i3+3,c3)+c(i,1,1,3)*ui(i1+1,i2+1,i3+3,c3)+c(i,2,
     & 1,3)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,1,3)*ui(i1+3,i2+1,i3+3,c3)+c(
     & i,4,1,3)*ui(i1+4,i2+1,i3+3,c3)+c(i,0,2,3)*ui(i1,i2+2,i3+3,c3)+
     & c(i,1,2,3)*ui(i1+1,i2+2,i3+3,c3)+c(i,2,2,3)*ui(i1+2,i2+2,i3+3,
     & c3)+c(i,3,2,3)*ui(i1+3,i2+2,i3+3,c3)+c(i,4,2,3)*ui(i1+4,i2+2,
     & i3+3,c3)+c(i,0,3,3)*ui(i1,i2+3,i3+3,c3)+c(i,1,3,3)*ui(i1+1,i2+
     & 3,i3+3,c3)+c(i,2,3,3)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,3,3)*ui(i1+3,
     & i2+3,i3+3,c3)+c(i,4,3,3)*ui(i1+4,i2+3,i3+3,c3)+c(i,0,4,3)*ui(
     & i1,i2+4,i3+3,c3)+c(i,1,4,3)*ui(i1+1,i2+4,i3+3,c3)+c(i,2,4,3)*
     & ui(i1+2,i2+4,i3+3,c3)+c(i,3,4,3)*ui(i1+3,i2+4,i3+3,c3)+c(i,4,4,
     & 3)*ui(i1+4,i2+4,i3+3,c3)+c(i,0,0,4)*ui(i1,i2,i3+4,c3)+c(i,1,0,
     & 4)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,4)*ui(i1+2,i2,i3+4,c3)+c(i,3,0,
     & 4)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,4)*ui(i1+4,i2,i3+4,c3)+c(i,0,1,
     & 4)*ui(i1,i2+1,i3+4,c3)+c(i,1,1,4)*ui(i1+1,i2+1,i3+4,c3)+c(i,2,
     & 1,4)*ui(i1+2,i2+1,i3+4,c3)+c(i,3,1,4)*ui(i1+3,i2+1,i3+4,c3)+c(
     & i,4,1,4)*ui(i1+4,i2+1,i3+4,c3)+c(i,0,2,4)*ui(i1,i2+2,i3+4,c3)+
     & c(i,1,2,4)*ui(i1+1,i2+2,i3+4,c3)+c(i,2,2,4)*ui(i1+2,i2+2,i3+4,
     & c3)+c(i,3,2,4)*ui(i1+3,i2+2,i3+4,c3)+c(i,4,2,4)*ui(i1+4,i2+2,
     & i3+4,c3)+c(i,0,3,4)*ui(i1,i2+3,i3+4,c3)+c(i,1,3,4)*ui(i1+1,i2+
     & 3,i3+4,c3)+c(i,2,3,4)*ui(i1+2,i2+3,i3+4,c3)+c(i,3,3,4)*ui(i1+3,
     & i2+3,i3+4,c3)+c(i,4,3,4)*ui(i1+4,i2+3,i3+4,c3)+c(i,0,4,4)*ui(
     & i1,i2+4,i3+4,c3)+c(i,1,4,4)*ui(i1+1,i2+4,i3+4,c3)+c(i,2,4,4)*
     & ui(i1+2,i2+4,i3+4,c3)+c(i,3,4,4)*ui(i1+3,i2+4,i3+4,c3)+c(i,4,4,
     & 4)*ui(i1+4,i2+4,i3+4,c3)
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           end if
         else if( width(1).eq.6 .and. width(2).eq.6 .and. width(3)
     & .eq.6 )then
! loops3d($interp666(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))
           if( c3a.eq.c3b )then
             do c3=c3a,c3b
             do i=nia,nib
! interp666(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,0,0)*ui(i1,i2,i3,c3)+c(i,1,0,0)*ui(i1+1,i2,
     & i3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3,c3)+c(i,3,0,0)*ui(i1+3,i2,i3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2,i3,c3)+c(i,5,0,0)*ui(i1+5,i2,i3,c3)+
     & c(i,0,1,0)*ui(i1,i2+1,i3,c3)+c(i,1,1,0)*ui(i1+1,i2+1,i3,c3)+c(
     & i,2,1,0)*ui(i1+2,i2+1,i3,c3)+c(i,3,1,0)*ui(i1+3,i2+1,i3,c3)+c(
     & i,4,1,0)*ui(i1+4,i2+1,i3,c3)+c(i,5,1,0)*ui(i1+5,i2+1,i3,c3)+c(
     & i,0,2,0)*ui(i1,i2+2,i3,c3)+c(i,1,2,0)*ui(i1+1,i2+2,i3,c3)+c(i,
     & 2,2,0)*ui(i1+2,i2+2,i3,c3)+c(i,3,2,0)*ui(i1+3,i2+2,i3,c3)+c(i,
     & 4,2,0)*ui(i1+4,i2+2,i3,c3)+c(i,5,2,0)*ui(i1+5,i2+2,i3,c3)+c(i,
     & 0,3,0)*ui(i1,i2+3,i3,c3)+c(i,1,3,0)*ui(i1+1,i2+3,i3,c3)+c(i,2,
     & 3,0)*ui(i1+2,i2+3,i3,c3)+c(i,3,3,0)*ui(i1+3,i2+3,i3,c3)+c(i,4,
     & 3,0)*ui(i1+4,i2+3,i3,c3)+c(i,5,3,0)*ui(i1+5,i2+3,i3,c3)+c(i,0,
     & 4,0)*ui(i1,i2+4,i3,c3)+c(i,1,4,0)*ui(i1+1,i2+4,i3,c3)+c(i,2,4,
     & 0)*ui(i1+2,i2+4,i3,c3)+c(i,3,4,0)*ui(i1+3,i2+4,i3,c3)+c(i,4,4,
     & 0)*ui(i1+4,i2+4,i3,c3)+c(i,5,4,0)*ui(i1+5,i2+4,i3,c3)+c(i,0,5,
     & 0)*ui(i1,i2+5,i3,c3)+c(i,1,5,0)*ui(i1+1,i2+5,i3,c3)+c(i,2,5,0)*
     & ui(i1+2,i2+5,i3,c3)+c(i,3,5,0)*ui(i1+3,i2+5,i3,c3)+c(i,4,5,0)*
     & ui(i1+4,i2+5,i3,c3)+c(i,5,5,0)*ui(i1+5,i2+5,i3,c3)+c(i,0,0,1)*
     & ui(i1,i2,i3+1,c3)+c(i,1,0,1)*ui(i1+1,i2,i3+1,c3)+c(i,2,0,1)*ui(
     & i1+2,i2,i3+1,c3)+c(i,3,0,1)*ui(i1+3,i2,i3+1,c3)+c(i,4,0,1)*ui(
     & i1+4,i2,i3+1,c3)+c(i,5,0,1)*ui(i1+5,i2,i3+1,c3)+c(i,0,1,1)*ui(
     & i1,i2+1,i3+1,c3)+c(i,1,1,1)*ui(i1+1,i2+1,i3+1,c3)+c(i,2,1,1)*
     & ui(i1+2,i2+1,i3+1,c3)+c(i,3,1,1)*ui(i1+3,i2+1,i3+1,c3)+c(i,4,1,
     & 1)*ui(i1+4,i2+1,i3+1,c3)+c(i,5,1,1)*ui(i1+5,i2+1,i3+1,c3)+c(i,
     & 0,2,1)*ui(i1,i2+2,i3+1,c3)+c(i,1,2,1)*ui(i1+1,i2+2,i3+1,c3)+c(
     & i,2,2,1)*ui(i1+2,i2+2,i3+1,c3)+c(i,3,2,1)*ui(i1+3,i2+2,i3+1,c3)
     & +c(i,4,2,1)*ui(i1+4,i2+2,i3+1,c3)+c(i,5,2,1)*ui(i1+5,i2+2,i3+1,
     & c3)+c(i,0,3,1)*ui(i1,i2+3,i3+1,c3)+c(i,1,3,1)*ui(i1+1,i2+3,i3+
     & 1,c3)+c(i,2,3,1)*ui(i1+2,i2+3,i3+1,c3)+c(i,3,3,1)*ui(i1+3,i2+3,
     & i3+1,c3)+c(i,4,3,1)*ui(i1+4,i2+3,i3+1,c3)+c(i,5,3,1)*ui(i1+5,
     & i2+3,i3+1,c3)+c(i,0,4,1)*ui(i1,i2+4,i3+1,c3)+c(i,1,4,1)*ui(i1+
     & 1,i2+4,i3+1,c3)+c(i,2,4,1)*ui(i1+2,i2+4,i3+1,c3)+c(i,3,4,1)*ui(
     & i1+3,i2+4,i3+1,c3)+c(i,4,4,1)*ui(i1+4,i2+4,i3+1,c3)+c(i,5,4,1)*
     & ui(i1+5,i2+4,i3+1,c3)+c(i,0,5,1)*ui(i1,i2+5,i3+1,c3)+c(i,1,5,1)
     & *ui(i1+1,i2+5,i3+1,c3)+c(i,2,5,1)*ui(i1+2,i2+5,i3+1,c3)+c(i,3,
     & 5,1)*ui(i1+3,i2+5,i3+1,c3)+c(i,4,5,1)*ui(i1+4,i2+5,i3+1,c3)+c(
     & i,5,5,1)*ui(i1+5,i2+5,i3+1,c3)+c(i,0,0,2)*ui(i1,i2,i3+2,c3)+c(
     & i,1,0,2)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,2)*ui(i1+2,i2,i3+2,c3)+c(
     & i,3,0,2)*ui(i1+3,i2,i3+2,c3)+c(i,4,0,2)*ui(i1+4,i2,i3+2,c3)+c(
     & i,5,0,2)*ui(i1+5,i2,i3+2,c3)+c(i,0,1,2)*ui(i1,i2+1,i3+2,c3)+c(
     & i,1,1,2)*ui(i1+1,i2+1,i3+2,c3)+c(i,2,1,2)*ui(i1+2,i2+1,i3+2,c3)
     & +c(i,3,1,2)*ui(i1+3,i2+1,i3+2,c3)+c(i,4,1,2)*ui(i1+4,i2+1,i3+2,
     & c3)+c(i,5,1,2)*ui(i1+5,i2+1,i3+2,c3)+c(i,0,2,2)*ui(i1,i2+2,i3+
     & 2,c3)+c(i,1,2,2)*ui(i1+1,i2+2,i3+2,c3)+c(i,2,2,2)*ui(i1+2,i2+2,
     & i3+2,c3)+c(i,3,2,2)*ui(i1+3,i2+2,i3+2,c3)+c(i,4,2,2)*ui(i1+4,
     & i2+2,i3+2,c3)+c(i,5,2,2)*ui(i1+5,i2+2,i3+2,c3)+c(i,0,3,2)*ui(
     & i1,i2+3,i3+2,c3)+c(i,1,3,2)*ui(i1+1,i2+3,i3+2,c3)+c(i,2,3,2)*
     & ui(i1+2,i2+3,i3+2,c3)+c(i,3,3,2)*ui(i1+3,i2+3,i3+2,c3)+c(i,4,3,
     & 2)*ui(i1+4,i2+3,i3+2,c3)+c(i,5,3,2)*ui(i1+5,i2+3,i3+2,c3)+c(i,
     & 0,4,2)*ui(i1,i2+4,i3+2,c3)+c(i,1,4,2)*ui(i1+1,i2+4,i3+2,c3)+c(
     & i,2,4,2)*ui(i1+2,i2+4,i3+2,c3)+c(i,3,4,2)*ui(i1+3,i2+4,i3+2,c3)
     & +c(i,4,4,2)*ui(i1+4,i2+4,i3+2,c3)+c(i,5,4,2)*ui(i1+5,i2+4,i3+2,
     & c3)+c(i,0,5,2)*ui(i1,i2+5,i3+2,c3)+c(i,1,5,2)*ui(i1+1,i2+5,i3+
     & 2,c3)+c(i,2,5,2)*ui(i1+2,i2+5,i3+2,c3)+c(i,3,5,2)*ui(i1+3,i2+5,
     & i3+2,c3)+c(i,4,5,2)*ui(i1+4,i2+5,i3+2,c3)+c(i,5,5,2)*ui(i1+5,
     & i2+5,i3+2,c3)+c(i,0,0,3)*ui(i1,i2,i3+3,c3)+c(i,1,0,3)*ui(i1+1,
     & i2,i3+3,c3)+c(i,2,0,3)*ui(i1+2,i2,i3+3,c3)+c(i,3,0,3)*ui(i1+3,
     & i2,i3+3,c3)+c(i,4,0,3)*ui(i1+4,i2,i3+3,c3)+c(i,5,0,3)*ui(i1+5,
     & i2,i3+3,c3)+c(i,0,1,3)*ui(i1,i2+1,i3+3,c3)+c(i,1,1,3)*ui(i1+1,
     & i2+1,i3+3,c3)+c(i,2,1,3)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,1,3)*ui(
     & i1+3,i2+1,i3+3,c3)+c(i,4,1,3)*ui(i1+4,i2+1,i3+3,c3)+c(i,5,1,3)*
     & ui(i1+5,i2+1,i3+3,c3)+c(i,0,2,3)*ui(i1,i2+2,i3+3,c3)+c(i,1,2,3)
     & *ui(i1+1,i2+2,i3+3,c3)+c(i,2,2,3)*ui(i1+2,i2+2,i3+3,c3)+c(i,3,
     & 2,3)*ui(i1+3,i2+2,i3+3,c3)+c(i,4,2,3)*ui(i1+4,i2+2,i3+3,c3)+c(
     & i,5,2,3)*ui(i1+5,i2+2,i3+3,c3)+c(i,0,3,3)*ui(i1,i2+3,i3+3,c3)+
     & c(i,1,3,3)*ui(i1+1,i2+3,i3+3,c3)+c(i,2,3,3)*ui(i1+2,i2+3,i3+3,
     & c3)+c(i,3,3,3)*ui(i1+3,i2+3,i3+3,c3)+c(i,4,3,3)*ui(i1+4,i2+3,
     & i3+3,c3)+c(i,5,3,3)*ui(i1+5,i2+3,i3+3,c3)+c(i,0,4,3)*ui(i1,i2+
     & 4,i3+3,c3)+c(i,1,4,3)*ui(i1+1,i2+4,i3+3,c3)+c(i,2,4,3)*ui(i1+2,
     & i2+4,i3+3,c3)+c(i,3,4,3)*ui(i1+3,i2+4,i3+3,c3)+c(i,4,4,3)*ui(
     & i1+4,i2+4,i3+3,c3)+c(i,5,4,3)*ui(i1+5,i2+4,i3+3,c3)+c(i,0,5,3)*
     & ui(i1,i2+5,i3+3,c3)+c(i,1,5,3)*ui(i1+1,i2+5,i3+3,c3)+c(i,2,5,3)
     & *ui(i1+2,i2+5,i3+3,c3)+c(i,3,5,3)*ui(i1+3,i2+5,i3+3,c3)+c(i,4,
     & 5,3)*ui(i1+4,i2+5,i3+3,c3)+c(i,5,5,3)*ui(i1+5,i2+5,i3+3,c3)+c(
     & i,0,0,4)*ui(i1,i2,i3+4,c3)+c(i,1,0,4)*ui(i1+1,i2,i3+4,c3)+c(i,
     & 2,0,4)*ui(i1+2,i2,i3+4,c3)+c(i,3,0,4)*ui(i1+3,i2,i3+4,c3)+c(i,
     & 4,0,4)*ui(i1+4,i2,i3+4,c3)+c(i,5,0,4)*ui(i1+5,i2,i3+4,c3)+c(i,
     & 0,1,4)*ui(i1,i2+1,i3+4,c3)+c(i,1,1,4)*ui(i1+1,i2+1,i3+4,c3)+c(
     & i,2,1,4)*ui(i1+2,i2+1,i3+4,c3)+c(i,3,1,4)*ui(i1+3,i2+1,i3+4,c3)
     & +c(i,4,1,4)*ui(i1+4,i2+1,i3+4,c3)+c(i,5,1,4)*ui(i1+5,i2+1,i3+4,
     & c3)+c(i,0,2,4)*ui(i1,i2+2,i3+4,c3)+c(i,1,2,4)*ui(i1+1,i2+2,i3+
     & 4,c3)+c(i,2,2,4)*ui(i1+2,i2+2,i3+4,c3)+c(i,3,2,4)*ui(i1+3,i2+2,
     & i3+4,c3)+c(i,4,2,4)*ui(i1+4,i2+2,i3+4,c3)+c(i,5,2,4)*ui(i1+5,
     & i2+2,i3+4,c3)+c(i,0,3,4)*ui(i1,i2+3,i3+4,c3)+c(i,1,3,4)*ui(i1+
     & 1,i2+3,i3+4,c3)+c(i,2,3,4)*ui(i1+2,i2+3,i3+4,c3)+c(i,3,3,4)*ui(
     & i1+3,i2+3,i3+4,c3)+c(i,4,3,4)*ui(i1+4,i2+3,i3+4,c3)+c(i,5,3,4)*
     & ui(i1+5,i2+3,i3+4,c3)+c(i,0,4,4)*ui(i1,i2+4,i3+4,c3)+c(i,1,4,4)
     & *ui(i1+1,i2+4,i3+4,c3)+c(i,2,4,4)*ui(i1+2,i2+4,i3+4,c3)+c(i,3,
     & 4,4)*ui(i1+3,i2+4,i3+4,c3)+c(i,4,4,4)*ui(i1+4,i2+4,i3+4,c3)+c(
     & i,5,4,4)*ui(i1+5,i2+4,i3+4,c3)+c(i,0,5,4)*ui(i1,i2+5,i3+4,c3)+
     & c(i,1,5,4)*ui(i1+1,i2+5,i3+4,c3)+c(i,2,5,4)*ui(i1+2,i2+5,i3+4,
     & c3)+c(i,3,5,4)*ui(i1+3,i2+5,i3+4,c3)+c(i,4,5,4)*ui(i1+4,i2+5,
     & i3+4,c3)+c(i,5,5,4)*ui(i1+5,i2+5,i3+4,c3)+c(i,0,0,5)*ui(i1,i2,
     & i3+5,c3)+c(i,1,0,5)*ui(i1+1,i2,i3+5,c3)+c(i,2,0,5)*ui(i1+2,i2,
     & i3+5,c3)+c(i,3,0,5)*ui(i1+3,i2,i3+5,c3)
               r(i) = r(i)+c(i,4,0,5)*ui(i1+4,i2,i3+5,c3)+c(i,5,0,5)*
     & ui(i1+5,i2,i3+5,c3)+c(i,0,1,5)*ui(i1,i2+1,i3+5,c3)+c(i,1,1,5)*
     & ui(i1+1,i2+1,i3+5,c3)+c(i,2,1,5)*ui(i1+2,i2+1,i3+5,c3)+c(i,3,1,
     & 5)*ui(i1+3,i2+1,i3+5,c3)+c(i,4,1,5)*ui(i1+4,i2+1,i3+5,c3)+c(i,
     & 5,1,5)*ui(i1+5,i2+1,i3+5,c3)+c(i,0,2,5)*ui(i1,i2+2,i3+5,c3)+c(
     & i,1,2,5)*ui(i1+1,i2+2,i3+5,c3)+c(i,2,2,5)*ui(i1+2,i2+2,i3+5,c3)
     & +c(i,3,2,5)*ui(i1+3,i2+2,i3+5,c3)+c(i,4,2,5)*ui(i1+4,i2+2,i3+5,
     & c3)+c(i,5,2,5)*ui(i1+5,i2+2,i3+5,c3)+c(i,0,3,5)*ui(i1,i2+3,i3+
     & 5,c3)+c(i,1,3,5)*ui(i1+1,i2+3,i3+5,c3)+c(i,2,3,5)*ui(i1+2,i2+3,
     & i3+5,c3)+c(i,3,3,5)*ui(i1+3,i2+3,i3+5,c3)+c(i,4,3,5)*ui(i1+4,
     & i2+3,i3+5,c3)+c(i,5,3,5)*ui(i1+5,i2+3,i3+5,c3)+c(i,0,4,5)*ui(
     & i1,i2+4,i3+5,c3)+c(i,1,4,5)*ui(i1+1,i2+4,i3+5,c3)+c(i,2,4,5)*
     & ui(i1+2,i2+4,i3+5,c3)+c(i,3,4,5)*ui(i1+3,i2+4,i3+5,c3)+c(i,4,4,
     & 5)*ui(i1+4,i2+4,i3+5,c3)+c(i,5,4,5)*ui(i1+5,i2+4,i3+5,c3)+c(i,
     & 0,5,5)*ui(i1,i2+5,i3+5,c3)+c(i,1,5,5)*ui(i1+1,i2+5,i3+5,c3)+c(
     & i,2,5,5)*ui(i1+2,i2+5,i3+5,c3)+c(i,3,5,5)*ui(i1+3,i2+5,i3+5,c3)
     & +c(i,4,5,5)*ui(i1+4,i2+5,i3+5,c3)+c(i,5,5,5)*ui(i1+5,i2+5,i3+5,
     & c3)
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
! interp666(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,0,0)*ui(i1,i2,i3,c3)+c(i,1,0,0)*ui(i1+1,i2,
     & i3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3,c3)+c(i,3,0,0)*ui(i1+3,i2,i3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2,i3,c3)+c(i,5,0,0)*ui(i1+5,i2,i3,c3)+
     & c(i,0,1,0)*ui(i1,i2+1,i3,c3)+c(i,1,1,0)*ui(i1+1,i2+1,i3,c3)+c(
     & i,2,1,0)*ui(i1+2,i2+1,i3,c3)+c(i,3,1,0)*ui(i1+3,i2+1,i3,c3)+c(
     & i,4,1,0)*ui(i1+4,i2+1,i3,c3)+c(i,5,1,0)*ui(i1+5,i2+1,i3,c3)+c(
     & i,0,2,0)*ui(i1,i2+2,i3,c3)+c(i,1,2,0)*ui(i1+1,i2+2,i3,c3)+c(i,
     & 2,2,0)*ui(i1+2,i2+2,i3,c3)+c(i,3,2,0)*ui(i1+3,i2+2,i3,c3)+c(i,
     & 4,2,0)*ui(i1+4,i2+2,i3,c3)+c(i,5,2,0)*ui(i1+5,i2+2,i3,c3)+c(i,
     & 0,3,0)*ui(i1,i2+3,i3,c3)+c(i,1,3,0)*ui(i1+1,i2+3,i3,c3)+c(i,2,
     & 3,0)*ui(i1+2,i2+3,i3,c3)+c(i,3,3,0)*ui(i1+3,i2+3,i3,c3)+c(i,4,
     & 3,0)*ui(i1+4,i2+3,i3,c3)+c(i,5,3,0)*ui(i1+5,i2+3,i3,c3)+c(i,0,
     & 4,0)*ui(i1,i2+4,i3,c3)+c(i,1,4,0)*ui(i1+1,i2+4,i3,c3)+c(i,2,4,
     & 0)*ui(i1+2,i2+4,i3,c3)+c(i,3,4,0)*ui(i1+3,i2+4,i3,c3)+c(i,4,4,
     & 0)*ui(i1+4,i2+4,i3,c3)+c(i,5,4,0)*ui(i1+5,i2+4,i3,c3)+c(i,0,5,
     & 0)*ui(i1,i2+5,i3,c3)+c(i,1,5,0)*ui(i1+1,i2+5,i3,c3)+c(i,2,5,0)*
     & ui(i1+2,i2+5,i3,c3)+c(i,3,5,0)*ui(i1+3,i2+5,i3,c3)+c(i,4,5,0)*
     & ui(i1+4,i2+5,i3,c3)+c(i,5,5,0)*ui(i1+5,i2+5,i3,c3)+c(i,0,0,1)*
     & ui(i1,i2,i3+1,c3)+c(i,1,0,1)*ui(i1+1,i2,i3+1,c3)+c(i,2,0,1)*ui(
     & i1+2,i2,i3+1,c3)+c(i,3,0,1)*ui(i1+3,i2,i3+1,c3)+c(i,4,0,1)*ui(
     & i1+4,i2,i3+1,c3)+c(i,5,0,1)*ui(i1+5,i2,i3+1,c3)+c(i,0,1,1)*ui(
     & i1,i2+1,i3+1,c3)+c(i,1,1,1)*ui(i1+1,i2+1,i3+1,c3)+c(i,2,1,1)*
     & ui(i1+2,i2+1,i3+1,c3)+c(i,3,1,1)*ui(i1+3,i2+1,i3+1,c3)+c(i,4,1,
     & 1)*ui(i1+4,i2+1,i3+1,c3)+c(i,5,1,1)*ui(i1+5,i2+1,i3+1,c3)+c(i,
     & 0,2,1)*ui(i1,i2+2,i3+1,c3)+c(i,1,2,1)*ui(i1+1,i2+2,i3+1,c3)+c(
     & i,2,2,1)*ui(i1+2,i2+2,i3+1,c3)+c(i,3,2,1)*ui(i1+3,i2+2,i3+1,c3)
     & +c(i,4,2,1)*ui(i1+4,i2+2,i3+1,c3)+c(i,5,2,1)*ui(i1+5,i2+2,i3+1,
     & c3)+c(i,0,3,1)*ui(i1,i2+3,i3+1,c3)+c(i,1,3,1)*ui(i1+1,i2+3,i3+
     & 1,c3)+c(i,2,3,1)*ui(i1+2,i2+3,i3+1,c3)+c(i,3,3,1)*ui(i1+3,i2+3,
     & i3+1,c3)+c(i,4,3,1)*ui(i1+4,i2+3,i3+1,c3)+c(i,5,3,1)*ui(i1+5,
     & i2+3,i3+1,c3)+c(i,0,4,1)*ui(i1,i2+4,i3+1,c3)+c(i,1,4,1)*ui(i1+
     & 1,i2+4,i3+1,c3)+c(i,2,4,1)*ui(i1+2,i2+4,i3+1,c3)+c(i,3,4,1)*ui(
     & i1+3,i2+4,i3+1,c3)+c(i,4,4,1)*ui(i1+4,i2+4,i3+1,c3)+c(i,5,4,1)*
     & ui(i1+5,i2+4,i3+1,c3)+c(i,0,5,1)*ui(i1,i2+5,i3+1,c3)+c(i,1,5,1)
     & *ui(i1+1,i2+5,i3+1,c3)+c(i,2,5,1)*ui(i1+2,i2+5,i3+1,c3)+c(i,3,
     & 5,1)*ui(i1+3,i2+5,i3+1,c3)+c(i,4,5,1)*ui(i1+4,i2+5,i3+1,c3)+c(
     & i,5,5,1)*ui(i1+5,i2+5,i3+1,c3)+c(i,0,0,2)*ui(i1,i2,i3+2,c3)+c(
     & i,1,0,2)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,2)*ui(i1+2,i2,i3+2,c3)+c(
     & i,3,0,2)*ui(i1+3,i2,i3+2,c3)+c(i,4,0,2)*ui(i1+4,i2,i3+2,c3)+c(
     & i,5,0,2)*ui(i1+5,i2,i3+2,c3)+c(i,0,1,2)*ui(i1,i2+1,i3+2,c3)+c(
     & i,1,1,2)*ui(i1+1,i2+1,i3+2,c3)+c(i,2,1,2)*ui(i1+2,i2+1,i3+2,c3)
     & +c(i,3,1,2)*ui(i1+3,i2+1,i3+2,c3)+c(i,4,1,2)*ui(i1+4,i2+1,i3+2,
     & c3)+c(i,5,1,2)*ui(i1+5,i2+1,i3+2,c3)+c(i,0,2,2)*ui(i1,i2+2,i3+
     & 2,c3)+c(i,1,2,2)*ui(i1+1,i2+2,i3+2,c3)+c(i,2,2,2)*ui(i1+2,i2+2,
     & i3+2,c3)+c(i,3,2,2)*ui(i1+3,i2+2,i3+2,c3)+c(i,4,2,2)*ui(i1+4,
     & i2+2,i3+2,c3)+c(i,5,2,2)*ui(i1+5,i2+2,i3+2,c3)+c(i,0,3,2)*ui(
     & i1,i2+3,i3+2,c3)+c(i,1,3,2)*ui(i1+1,i2+3,i3+2,c3)+c(i,2,3,2)*
     & ui(i1+2,i2+3,i3+2,c3)+c(i,3,3,2)*ui(i1+3,i2+3,i3+2,c3)+c(i,4,3,
     & 2)*ui(i1+4,i2+3,i3+2,c3)+c(i,5,3,2)*ui(i1+5,i2+3,i3+2,c3)+c(i,
     & 0,4,2)*ui(i1,i2+4,i3+2,c3)+c(i,1,4,2)*ui(i1+1,i2+4,i3+2,c3)+c(
     & i,2,4,2)*ui(i1+2,i2+4,i3+2,c3)+c(i,3,4,2)*ui(i1+3,i2+4,i3+2,c3)
     & +c(i,4,4,2)*ui(i1+4,i2+4,i3+2,c3)+c(i,5,4,2)*ui(i1+5,i2+4,i3+2,
     & c3)+c(i,0,5,2)*ui(i1,i2+5,i3+2,c3)+c(i,1,5,2)*ui(i1+1,i2+5,i3+
     & 2,c3)+c(i,2,5,2)*ui(i1+2,i2+5,i3+2,c3)+c(i,3,5,2)*ui(i1+3,i2+5,
     & i3+2,c3)+c(i,4,5,2)*ui(i1+4,i2+5,i3+2,c3)+c(i,5,5,2)*ui(i1+5,
     & i2+5,i3+2,c3)+c(i,0,0,3)*ui(i1,i2,i3+3,c3)+c(i,1,0,3)*ui(i1+1,
     & i2,i3+3,c3)+c(i,2,0,3)*ui(i1+2,i2,i3+3,c3)+c(i,3,0,3)*ui(i1+3,
     & i2,i3+3,c3)+c(i,4,0,3)*ui(i1+4,i2,i3+3,c3)+c(i,5,0,3)*ui(i1+5,
     & i2,i3+3,c3)+c(i,0,1,3)*ui(i1,i2+1,i3+3,c3)+c(i,1,1,3)*ui(i1+1,
     & i2+1,i3+3,c3)+c(i,2,1,3)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,1,3)*ui(
     & i1+3,i2+1,i3+3,c3)+c(i,4,1,3)*ui(i1+4,i2+1,i3+3,c3)+c(i,5,1,3)*
     & ui(i1+5,i2+1,i3+3,c3)+c(i,0,2,3)*ui(i1,i2+2,i3+3,c3)+c(i,1,2,3)
     & *ui(i1+1,i2+2,i3+3,c3)+c(i,2,2,3)*ui(i1+2,i2+2,i3+3,c3)+c(i,3,
     & 2,3)*ui(i1+3,i2+2,i3+3,c3)+c(i,4,2,3)*ui(i1+4,i2+2,i3+3,c3)+c(
     & i,5,2,3)*ui(i1+5,i2+2,i3+3,c3)+c(i,0,3,3)*ui(i1,i2+3,i3+3,c3)+
     & c(i,1,3,3)*ui(i1+1,i2+3,i3+3,c3)+c(i,2,3,3)*ui(i1+2,i2+3,i3+3,
     & c3)+c(i,3,3,3)*ui(i1+3,i2+3,i3+3,c3)+c(i,4,3,3)*ui(i1+4,i2+3,
     & i3+3,c3)+c(i,5,3,3)*ui(i1+5,i2+3,i3+3,c3)+c(i,0,4,3)*ui(i1,i2+
     & 4,i3+3,c3)+c(i,1,4,3)*ui(i1+1,i2+4,i3+3,c3)+c(i,2,4,3)*ui(i1+2,
     & i2+4,i3+3,c3)+c(i,3,4,3)*ui(i1+3,i2+4,i3+3,c3)+c(i,4,4,3)*ui(
     & i1+4,i2+4,i3+3,c3)+c(i,5,4,3)*ui(i1+5,i2+4,i3+3,c3)+c(i,0,5,3)*
     & ui(i1,i2+5,i3+3,c3)+c(i,1,5,3)*ui(i1+1,i2+5,i3+3,c3)+c(i,2,5,3)
     & *ui(i1+2,i2+5,i3+3,c3)+c(i,3,5,3)*ui(i1+3,i2+5,i3+3,c3)+c(i,4,
     & 5,3)*ui(i1+4,i2+5,i3+3,c3)+c(i,5,5,3)*ui(i1+5,i2+5,i3+3,c3)+c(
     & i,0,0,4)*ui(i1,i2,i3+4,c3)+c(i,1,0,4)*ui(i1+1,i2,i3+4,c3)+c(i,
     & 2,0,4)*ui(i1+2,i2,i3+4,c3)+c(i,3,0,4)*ui(i1+3,i2,i3+4,c3)+c(i,
     & 4,0,4)*ui(i1+4,i2,i3+4,c3)+c(i,5,0,4)*ui(i1+5,i2,i3+4,c3)+c(i,
     & 0,1,4)*ui(i1,i2+1,i3+4,c3)+c(i,1,1,4)*ui(i1+1,i2+1,i3+4,c3)+c(
     & i,2,1,4)*ui(i1+2,i2+1,i3+4,c3)+c(i,3,1,4)*ui(i1+3,i2+1,i3+4,c3)
     & +c(i,4,1,4)*ui(i1+4,i2+1,i3+4,c3)+c(i,5,1,4)*ui(i1+5,i2+1,i3+4,
     & c3)+c(i,0,2,4)*ui(i1,i2+2,i3+4,c3)+c(i,1,2,4)*ui(i1+1,i2+2,i3+
     & 4,c3)+c(i,2,2,4)*ui(i1+2,i2+2,i3+4,c3)+c(i,3,2,4)*ui(i1+3,i2+2,
     & i3+4,c3)+c(i,4,2,4)*ui(i1+4,i2+2,i3+4,c3)+c(i,5,2,4)*ui(i1+5,
     & i2+2,i3+4,c3)+c(i,0,3,4)*ui(i1,i2+3,i3+4,c3)+c(i,1,3,4)*ui(i1+
     & 1,i2+3,i3+4,c3)+c(i,2,3,4)*ui(i1+2,i2+3,i3+4,c3)+c(i,3,3,4)*ui(
     & i1+3,i2+3,i3+4,c3)+c(i,4,3,4)*ui(i1+4,i2+3,i3+4,c3)+c(i,5,3,4)*
     & ui(i1+5,i2+3,i3+4,c3)+c(i,0,4,4)*ui(i1,i2+4,i3+4,c3)+c(i,1,4,4)
     & *ui(i1+1,i2+4,i3+4,c3)+c(i,2,4,4)*ui(i1+2,i2+4,i3+4,c3)+c(i,3,
     & 4,4)*ui(i1+3,i2+4,i3+4,c3)+c(i,4,4,4)*ui(i1+4,i2+4,i3+4,c3)+c(
     & i,5,4,4)*ui(i1+5,i2+4,i3+4,c3)+c(i,0,5,4)*ui(i1,i2+5,i3+4,c3)+
     & c(i,1,5,4)*ui(i1+1,i2+5,i3+4,c3)+c(i,2,5,4)*ui(i1+2,i2+5,i3+4,
     & c3)+c(i,3,5,4)*ui(i1+3,i2+5,i3+4,c3)+c(i,4,5,4)*ui(i1+4,i2+5,
     & i3+4,c3)+c(i,5,5,4)*ui(i1+5,i2+5,i3+4,c3)+c(i,0,0,5)*ui(i1,i2,
     & i3+5,c3)+c(i,1,0,5)*ui(i1+1,i2,i3+5,c3)+c(i,2,0,5)*ui(i1+2,i2,
     & i3+5,c3)+c(i,3,0,5)*ui(i1+3,i2,i3+5,c3)
               r(i) = r(i)+c(i,4,0,5)*ui(i1+4,i2,i3+5,c3)+c(i,5,0,5)*
     & ui(i1+5,i2,i3+5,c3)+c(i,0,1,5)*ui(i1,i2+1,i3+5,c3)+c(i,1,1,5)*
     & ui(i1+1,i2+1,i3+5,c3)+c(i,2,1,5)*ui(i1+2,i2+1,i3+5,c3)+c(i,3,1,
     & 5)*ui(i1+3,i2+1,i3+5,c3)+c(i,4,1,5)*ui(i1+4,i2+1,i3+5,c3)+c(i,
     & 5,1,5)*ui(i1+5,i2+1,i3+5,c3)+c(i,0,2,5)*ui(i1,i2+2,i3+5,c3)+c(
     & i,1,2,5)*ui(i1+1,i2+2,i3+5,c3)+c(i,2,2,5)*ui(i1+2,i2+2,i3+5,c3)
     & +c(i,3,2,5)*ui(i1+3,i2+2,i3+5,c3)+c(i,4,2,5)*ui(i1+4,i2+2,i3+5,
     & c3)+c(i,5,2,5)*ui(i1+5,i2+2,i3+5,c3)+c(i,0,3,5)*ui(i1,i2+3,i3+
     & 5,c3)+c(i,1,3,5)*ui(i1+1,i2+3,i3+5,c3)+c(i,2,3,5)*ui(i1+2,i2+3,
     & i3+5,c3)+c(i,3,3,5)*ui(i1+3,i2+3,i3+5,c3)+c(i,4,3,5)*ui(i1+4,
     & i2+3,i3+5,c3)+c(i,5,3,5)*ui(i1+5,i2+3,i3+5,c3)+c(i,0,4,5)*ui(
     & i1,i2+4,i3+5,c3)+c(i,1,4,5)*ui(i1+1,i2+4,i3+5,c3)+c(i,2,4,5)*
     & ui(i1+2,i2+4,i3+5,c3)+c(i,3,4,5)*ui(i1+3,i2+4,i3+5,c3)+c(i,4,4,
     & 5)*ui(i1+4,i2+4,i3+5,c3)+c(i,5,4,5)*ui(i1+5,i2+4,i3+5,c3)+c(i,
     & 0,5,5)*ui(i1,i2+5,i3+5,c3)+c(i,1,5,5)*ui(i1+1,i2+5,i3+5,c3)+c(
     & i,2,5,5)*ui(i1+2,i2+5,i3+5,c3)+c(i,3,5,5)*ui(i1+3,i2+5,i3+5,c3)
     & +c(i,4,5,5)*ui(i1+4,i2+5,i3+5,c3)+c(i,5,5,5)*ui(i1+5,i2+5,i3+5,
     & c3)
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           end if
         else if( width(1).eq.7 .and. width(2).eq.7 .and. width(3)
     & .eq.7 )then
! loops3d($interp777(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))
           if( c3a.eq.c3b )then
             do c3=c3a,c3b
             do i=nia,nib
! interp777(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,0,0)*ui(i1,i2,i3,c3)+c(i,1,0,0)*ui(i1+1,i2,
     & i3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3,c3)+c(i,3,0,0)*ui(i1+3,i2,i3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2,i3,c3)+c(i,5,0,0)*ui(i1+5,i2,i3,c3)+
     & c(i,6,0,0)*ui(i1+6,i2,i3,c3)+c(i,0,1,0)*ui(i1,i2+1,i3,c3)+c(i,
     & 1,1,0)*ui(i1+1,i2+1,i3,c3)+c(i,2,1,0)*ui(i1+2,i2+1,i3,c3)+c(i,
     & 3,1,0)*ui(i1+3,i2+1,i3,c3)+c(i,4,1,0)*ui(i1+4,i2+1,i3,c3)+c(i,
     & 5,1,0)*ui(i1+5,i2+1,i3,c3)+c(i,6,1,0)*ui(i1+6,i2+1,i3,c3)+c(i,
     & 0,2,0)*ui(i1,i2+2,i3,c3)+c(i,1,2,0)*ui(i1+1,i2+2,i3,c3)+c(i,2,
     & 2,0)*ui(i1+2,i2+2,i3,c3)+c(i,3,2,0)*ui(i1+3,i2+2,i3,c3)+c(i,4,
     & 2,0)*ui(i1+4,i2+2,i3,c3)+c(i,5,2,0)*ui(i1+5,i2+2,i3,c3)+c(i,6,
     & 2,0)*ui(i1+6,i2+2,i3,c3)+c(i,0,3,0)*ui(i1,i2+3,i3,c3)+c(i,1,3,
     & 0)*ui(i1+1,i2+3,i3,c3)+c(i,2,3,0)*ui(i1+2,i2+3,i3,c3)+c(i,3,3,
     & 0)*ui(i1+3,i2+3,i3,c3)+c(i,4,3,0)*ui(i1+4,i2+3,i3,c3)+c(i,5,3,
     & 0)*ui(i1+5,i2+3,i3,c3)+c(i,6,3,0)*ui(i1+6,i2+3,i3,c3)+c(i,0,4,
     & 0)*ui(i1,i2+4,i3,c3)+c(i,1,4,0)*ui(i1+1,i2+4,i3,c3)+c(i,2,4,0)*
     & ui(i1+2,i2+4,i3,c3)+c(i,3,4,0)*ui(i1+3,i2+4,i3,c3)+c(i,4,4,0)*
     & ui(i1+4,i2+4,i3,c3)+c(i,5,4,0)*ui(i1+5,i2+4,i3,c3)+c(i,6,4,0)*
     & ui(i1+6,i2+4,i3,c3)+c(i,0,5,0)*ui(i1,i2+5,i3,c3)+c(i,1,5,0)*ui(
     & i1+1,i2+5,i3,c3)+c(i,2,5,0)*ui(i1+2,i2+5,i3,c3)+c(i,3,5,0)*ui(
     & i1+3,i2+5,i3,c3)+c(i,4,5,0)*ui(i1+4,i2+5,i3,c3)+c(i,5,5,0)*ui(
     & i1+5,i2+5,i3,c3)+c(i,6,5,0)*ui(i1+6,i2+5,i3,c3)+c(i,0,6,0)*ui(
     & i1,i2+6,i3,c3)+c(i,1,6,0)*ui(i1+1,i2+6,i3,c3)+c(i,2,6,0)*ui(i1+
     & 2,i2+6,i3,c3)+c(i,3,6,0)*ui(i1+3,i2+6,i3,c3)+c(i,4,6,0)*ui(i1+
     & 4,i2+6,i3,c3)+c(i,5,6,0)*ui(i1+5,i2+6,i3,c3)+c(i,6,6,0)*ui(i1+
     & 6,i2+6,i3,c3)+c(i,0,0,1)*ui(i1,i2,i3+1,c3)+c(i,1,0,1)*ui(i1+1,
     & i2,i3+1,c3)+c(i,2,0,1)*ui(i1+2,i2,i3+1,c3)+c(i,3,0,1)*ui(i1+3,
     & i2,i3+1,c3)+c(i,4,0,1)*ui(i1+4,i2,i3+1,c3)+c(i,5,0,1)*ui(i1+5,
     & i2,i3+1,c3)+c(i,6,0,1)*ui(i1+6,i2,i3+1,c3)+c(i,0,1,1)*ui(i1,i2+
     & 1,i3+1,c3)+c(i,1,1,1)*ui(i1+1,i2+1,i3+1,c3)+c(i,2,1,1)*ui(i1+2,
     & i2+1,i3+1,c3)+c(i,3,1,1)*ui(i1+3,i2+1,i3+1,c3)+c(i,4,1,1)*ui(
     & i1+4,i2+1,i3+1,c3)+c(i,5,1,1)*ui(i1+5,i2+1,i3+1,c3)+c(i,6,1,1)*
     & ui(i1+6,i2+1,i3+1,c3)+c(i,0,2,1)*ui(i1,i2+2,i3+1,c3)+c(i,1,2,1)
     & *ui(i1+1,i2+2,i3+1,c3)+c(i,2,2,1)*ui(i1+2,i2+2,i3+1,c3)+c(i,3,
     & 2,1)*ui(i1+3,i2+2,i3+1,c3)+c(i,4,2,1)*ui(i1+4,i2+2,i3+1,c3)+c(
     & i,5,2,1)*ui(i1+5,i2+2,i3+1,c3)+c(i,6,2,1)*ui(i1+6,i2+2,i3+1,c3)
     & +c(i,0,3,1)*ui(i1,i2+3,i3+1,c3)+c(i,1,3,1)*ui(i1+1,i2+3,i3+1,
     & c3)+c(i,2,3,1)*ui(i1+2,i2+3,i3+1,c3)+c(i,3,3,1)*ui(i1+3,i2+3,
     & i3+1,c3)+c(i,4,3,1)*ui(i1+4,i2+3,i3+1,c3)+c(i,5,3,1)*ui(i1+5,
     & i2+3,i3+1,c3)+c(i,6,3,1)*ui(i1+6,i2+3,i3+1,c3)+c(i,0,4,1)*ui(
     & i1,i2+4,i3+1,c3)+c(i,1,4,1)*ui(i1+1,i2+4,i3+1,c3)+c(i,2,4,1)*
     & ui(i1+2,i2+4,i3+1,c3)+c(i,3,4,1)*ui(i1+3,i2+4,i3+1,c3)+c(i,4,4,
     & 1)*ui(i1+4,i2+4,i3+1,c3)+c(i,5,4,1)*ui(i1+5,i2+4,i3+1,c3)+c(i,
     & 6,4,1)*ui(i1+6,i2+4,i3+1,c3)+c(i,0,5,1)*ui(i1,i2+5,i3+1,c3)+c(
     & i,1,5,1)*ui(i1+1,i2+5,i3+1,c3)+c(i,2,5,1)*ui(i1+2,i2+5,i3+1,c3)
     & +c(i,3,5,1)*ui(i1+3,i2+5,i3+1,c3)+c(i,4,5,1)*ui(i1+4,i2+5,i3+1,
     & c3)+c(i,5,5,1)*ui(i1+5,i2+5,i3+1,c3)+c(i,6,5,1)*ui(i1+6,i2+5,
     & i3+1,c3)+c(i,0,6,1)*ui(i1,i2+6,i3+1,c3)+c(i,1,6,1)*ui(i1+1,i2+
     & 6,i3+1,c3)+c(i,2,6,1)*ui(i1+2,i2+6,i3+1,c3)+c(i,3,6,1)*ui(i1+3,
     & i2+6,i3+1,c3)+c(i,4,6,1)*ui(i1+4,i2+6,i3+1,c3)+c(i,5,6,1)*ui(
     & i1+5,i2+6,i3+1,c3)+c(i,6,6,1)*ui(i1+6,i2+6,i3+1,c3)+c(i,0,0,2)*
     & ui(i1,i2,i3+2,c3)+c(i,1,0,2)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,2)*ui(
     & i1+2,i2,i3+2,c3)+c(i,3,0,2)*ui(i1+3,i2,i3+2,c3)+c(i,4,0,2)*ui(
     & i1+4,i2,i3+2,c3)+c(i,5,0,2)*ui(i1+5,i2,i3+2,c3)+c(i,6,0,2)*ui(
     & i1+6,i2,i3+2,c3)+c(i,0,1,2)*ui(i1,i2+1,i3+2,c3)+c(i,1,1,2)*ui(
     & i1+1,i2+1,i3+2,c3)+c(i,2,1,2)*ui(i1+2,i2+1,i3+2,c3)+c(i,3,1,2)*
     & ui(i1+3,i2+1,i3+2,c3)+c(i,4,1,2)*ui(i1+4,i2+1,i3+2,c3)+c(i,5,1,
     & 2)*ui(i1+5,i2+1,i3+2,c3)+c(i,6,1,2)*ui(i1+6,i2+1,i3+2,c3)+c(i,
     & 0,2,2)*ui(i1,i2+2,i3+2,c3)+c(i,1,2,2)*ui(i1+1,i2+2,i3+2,c3)+c(
     & i,2,2,2)*ui(i1+2,i2+2,i3+2,c3)+c(i,3,2,2)*ui(i1+3,i2+2,i3+2,c3)
     & +c(i,4,2,2)*ui(i1+4,i2+2,i3+2,c3)+c(i,5,2,2)*ui(i1+5,i2+2,i3+2,
     & c3)+c(i,6,2,2)*ui(i1+6,i2+2,i3+2,c3)+c(i,0,3,2)*ui(i1,i2+3,i3+
     & 2,c3)+c(i,1,3,2)*ui(i1+1,i2+3,i3+2,c3)+c(i,2,3,2)*ui(i1+2,i2+3,
     & i3+2,c3)+c(i,3,3,2)*ui(i1+3,i2+3,i3+2,c3)+c(i,4,3,2)*ui(i1+4,
     & i2+3,i3+2,c3)+c(i,5,3,2)*ui(i1+5,i2+3,i3+2,c3)+c(i,6,3,2)*ui(
     & i1+6,i2+3,i3+2,c3)+c(i,0,4,2)*ui(i1,i2+4,i3+2,c3)+c(i,1,4,2)*
     & ui(i1+1,i2+4,i3+2,c3)+c(i,2,4,2)*ui(i1+2,i2+4,i3+2,c3)+c(i,3,4,
     & 2)*ui(i1+3,i2+4,i3+2,c3)+c(i,4,4,2)*ui(i1+4,i2+4,i3+2,c3)+c(i,
     & 5,4,2)*ui(i1+5,i2+4,i3+2,c3)+c(i,6,4,2)*ui(i1+6,i2+4,i3+2,c3)+
     & c(i,0,5,2)*ui(i1,i2+5,i3+2,c3)+c(i,1,5,2)*ui(i1+1,i2+5,i3+2,c3)
     & +c(i,2,5,2)*ui(i1+2,i2+5,i3+2,c3)+c(i,3,5,2)*ui(i1+3,i2+5,i3+2,
     & c3)+c(i,4,5,2)*ui(i1+4,i2+5,i3+2,c3)+c(i,5,5,2)*ui(i1+5,i2+5,
     & i3+2,c3)+c(i,6,5,2)*ui(i1+6,i2+5,i3+2,c3)+c(i,0,6,2)*ui(i1,i2+
     & 6,i3+2,c3)+c(i,1,6,2)*ui(i1+1,i2+6,i3+2,c3)+c(i,2,6,2)*ui(i1+2,
     & i2+6,i3+2,c3)+c(i,3,6,2)*ui(i1+3,i2+6,i3+2,c3)+c(i,4,6,2)*ui(
     & i1+4,i2+6,i3+2,c3)+c(i,5,6,2)*ui(i1+5,i2+6,i3+2,c3)+c(i,6,6,2)*
     & ui(i1+6,i2+6,i3+2,c3)+c(i,0,0,3)*ui(i1,i2,i3+3,c3)+c(i,1,0,3)*
     & ui(i1+1,i2,i3+3,c3)+c(i,2,0,3)*ui(i1+2,i2,i3+3,c3)+c(i,3,0,3)*
     & ui(i1+3,i2,i3+3,c3)+c(i,4,0,3)*ui(i1+4,i2,i3+3,c3)+c(i,5,0,3)*
     & ui(i1+5,i2,i3+3,c3)+c(i,6,0,3)*ui(i1+6,i2,i3+3,c3)+c(i,0,1,3)*
     & ui(i1,i2+1,i3+3,c3)+c(i,1,1,3)*ui(i1+1,i2+1,i3+3,c3)+c(i,2,1,3)
     & *ui(i1+2,i2+1,i3+3,c3)+c(i,3,1,3)*ui(i1+3,i2+1,i3+3,c3)+c(i,4,
     & 1,3)*ui(i1+4,i2+1,i3+3,c3)+c(i,5,1,3)*ui(i1+5,i2+1,i3+3,c3)+c(
     & i,6,1,3)*ui(i1+6,i2+1,i3+3,c3)+c(i,0,2,3)*ui(i1,i2+2,i3+3,c3)+
     & c(i,1,2,3)*ui(i1+1,i2+2,i3+3,c3)+c(i,2,2,3)*ui(i1+2,i2+2,i3+3,
     & c3)+c(i,3,2,3)*ui(i1+3,i2+2,i3+3,c3)+c(i,4,2,3)*ui(i1+4,i2+2,
     & i3+3,c3)+c(i,5,2,3)*ui(i1+5,i2+2,i3+3,c3)+c(i,6,2,3)*ui(i1+6,
     & i2+2,i3+3,c3)+c(i,0,3,3)*ui(i1,i2+3,i3+3,c3)+c(i,1,3,3)*ui(i1+
     & 1,i2+3,i3+3,c3)+c(i,2,3,3)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,3,3)*ui(
     & i1+3,i2+3,i3+3,c3)+c(i,4,3,3)*ui(i1+4,i2+3,i3+3,c3)+c(i,5,3,3)*
     & ui(i1+5,i2+3,i3+3,c3)+c(i,6,3,3)*ui(i1+6,i2+3,i3+3,c3)+c(i,0,4,
     & 3)*ui(i1,i2+4,i3+3,c3)+c(i,1,4,3)*ui(i1+1,i2+4,i3+3,c3)+c(i,2,
     & 4,3)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,4,3)*ui(i1+3,i2+4,i3+3,c3)+c(
     & i,4,4,3)*ui(i1+4,i2+4,i3+3,c3)+c(i,5,4,3)*ui(i1+5,i2+4,i3+3,c3)
     & +c(i,6,4,3)*ui(i1+6,i2+4,i3+3,c3)+c(i,0,5,3)*ui(i1,i2+5,i3+3,
     & c3)+c(i,1,5,3)*ui(i1+1,i2+5,i3+3,c3)
               r(i) = r(i)+c(i,2,5,3)*ui(i1+2,i2+5,i3+3,c3)+c(i,3,5,3)*
     & ui(i1+3,i2+5,i3+3,c3)+c(i,4,5,3)*ui(i1+4,i2+5,i3+3,c3)+c(i,5,5,
     & 3)*ui(i1+5,i2+5,i3+3,c3)+c(i,6,5,3)*ui(i1+6,i2+5,i3+3,c3)+c(i,
     & 0,6,3)*ui(i1,i2+6,i3+3,c3)+c(i,1,6,3)*ui(i1+1,i2+6,i3+3,c3)+c(
     & i,2,6,3)*ui(i1+2,i2+6,i3+3,c3)+c(i,3,6,3)*ui(i1+3,i2+6,i3+3,c3)
     & +c(i,4,6,3)*ui(i1+4,i2+6,i3+3,c3)+c(i,5,6,3)*ui(i1+5,i2+6,i3+3,
     & c3)+c(i,6,6,3)*ui(i1+6,i2+6,i3+3,c3)+c(i,0,0,4)*ui(i1,i2,i3+4,
     & c3)+c(i,1,0,4)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,4)*ui(i1+2,i2,i3+4,
     & c3)+c(i,3,0,4)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,4)*ui(i1+4,i2,i3+4,
     & c3)+c(i,5,0,4)*ui(i1+5,i2,i3+4,c3)+c(i,6,0,4)*ui(i1+6,i2,i3+4,
     & c3)+c(i,0,1,4)*ui(i1,i2+1,i3+4,c3)+c(i,1,1,4)*ui(i1+1,i2+1,i3+
     & 4,c3)+c(i,2,1,4)*ui(i1+2,i2+1,i3+4,c3)+c(i,3,1,4)*ui(i1+3,i2+1,
     & i3+4,c3)+c(i,4,1,4)*ui(i1+4,i2+1,i3+4,c3)+c(i,5,1,4)*ui(i1+5,
     & i2+1,i3+4,c3)+c(i,6,1,4)*ui(i1+6,i2+1,i3+4,c3)+c(i,0,2,4)*ui(
     & i1,i2+2,i3+4,c3)+c(i,1,2,4)*ui(i1+1,i2+2,i3+4,c3)+c(i,2,2,4)*
     & ui(i1+2,i2+2,i3+4,c3)+c(i,3,2,4)*ui(i1+3,i2+2,i3+4,c3)+c(i,4,2,
     & 4)*ui(i1+4,i2+2,i3+4,c3)+c(i,5,2,4)*ui(i1+5,i2+2,i3+4,c3)+c(i,
     & 6,2,4)*ui(i1+6,i2+2,i3+4,c3)+c(i,0,3,4)*ui(i1,i2+3,i3+4,c3)+c(
     & i,1,3,4)*ui(i1+1,i2+3,i3+4,c3)+c(i,2,3,4)*ui(i1+2,i2+3,i3+4,c3)
     & +c(i,3,3,4)*ui(i1+3,i2+3,i3+4,c3)+c(i,4,3,4)*ui(i1+4,i2+3,i3+4,
     & c3)+c(i,5,3,4)*ui(i1+5,i2+3,i3+4,c3)+c(i,6,3,4)*ui(i1+6,i2+3,
     & i3+4,c3)+c(i,0,4,4)*ui(i1,i2+4,i3+4,c3)+c(i,1,4,4)*ui(i1+1,i2+
     & 4,i3+4,c3)+c(i,2,4,4)*ui(i1+2,i2+4,i3+4,c3)+c(i,3,4,4)*ui(i1+3,
     & i2+4,i3+4,c3)+c(i,4,4,4)*ui(i1+4,i2+4,i3+4,c3)+c(i,5,4,4)*ui(
     & i1+5,i2+4,i3+4,c3)+c(i,6,4,4)*ui(i1+6,i2+4,i3+4,c3)+c(i,0,5,4)*
     & ui(i1,i2+5,i3+4,c3)+c(i,1,5,4)*ui(i1+1,i2+5,i3+4,c3)+c(i,2,5,4)
     & *ui(i1+2,i2+5,i3+4,c3)+c(i,3,5,4)*ui(i1+3,i2+5,i3+4,c3)+c(i,4,
     & 5,4)*ui(i1+4,i2+5,i3+4,c3)+c(i,5,5,4)*ui(i1+5,i2+5,i3+4,c3)+c(
     & i,6,5,4)*ui(i1+6,i2+5,i3+4,c3)+c(i,0,6,4)*ui(i1,i2+6,i3+4,c3)+
     & c(i,1,6,4)*ui(i1+1,i2+6,i3+4,c3)+c(i,2,6,4)*ui(i1+2,i2+6,i3+4,
     & c3)+c(i,3,6,4)*ui(i1+3,i2+6,i3+4,c3)+c(i,4,6,4)*ui(i1+4,i2+6,
     & i3+4,c3)+c(i,5,6,4)*ui(i1+5,i2+6,i3+4,c3)+c(i,6,6,4)*ui(i1+6,
     & i2+6,i3+4,c3)+c(i,0,0,5)*ui(i1,i2,i3+5,c3)+c(i,1,0,5)*ui(i1+1,
     & i2,i3+5,c3)+c(i,2,0,5)*ui(i1+2,i2,i3+5,c3)+c(i,3,0,5)*ui(i1+3,
     & i2,i3+5,c3)+c(i,4,0,5)*ui(i1+4,i2,i3+5,c3)+c(i,5,0,5)*ui(i1+5,
     & i2,i3+5,c3)+c(i,6,0,5)*ui(i1+6,i2,i3+5,c3)+c(i,0,1,5)*ui(i1,i2+
     & 1,i3+5,c3)+c(i,1,1,5)*ui(i1+1,i2+1,i3+5,c3)+c(i,2,1,5)*ui(i1+2,
     & i2+1,i3+5,c3)+c(i,3,1,5)*ui(i1+3,i2+1,i3+5,c3)+c(i,4,1,5)*ui(
     & i1+4,i2+1,i3+5,c3)+c(i,5,1,5)*ui(i1+5,i2+1,i3+5,c3)+c(i,6,1,5)*
     & ui(i1+6,i2+1,i3+5,c3)+c(i,0,2,5)*ui(i1,i2+2,i3+5,c3)+c(i,1,2,5)
     & *ui(i1+1,i2+2,i3+5,c3)+c(i,2,2,5)*ui(i1+2,i2+2,i3+5,c3)+c(i,3,
     & 2,5)*ui(i1+3,i2+2,i3+5,c3)+c(i,4,2,5)*ui(i1+4,i2+2,i3+5,c3)+c(
     & i,5,2,5)*ui(i1+5,i2+2,i3+5,c3)+c(i,6,2,5)*ui(i1+6,i2+2,i3+5,c3)
     & +c(i,0,3,5)*ui(i1,i2+3,i3+5,c3)+c(i,1,3,5)*ui(i1+1,i2+3,i3+5,
     & c3)+c(i,2,3,5)*ui(i1+2,i2+3,i3+5,c3)+c(i,3,3,5)*ui(i1+3,i2+3,
     & i3+5,c3)+c(i,4,3,5)*ui(i1+4,i2+3,i3+5,c3)+c(i,5,3,5)*ui(i1+5,
     & i2+3,i3+5,c3)+c(i,6,3,5)*ui(i1+6,i2+3,i3+5,c3)+c(i,0,4,5)*ui(
     & i1,i2+4,i3+5,c3)+c(i,1,4,5)*ui(i1+1,i2+4,i3+5,c3)+c(i,2,4,5)*
     & ui(i1+2,i2+4,i3+5,c3)+c(i,3,4,5)*ui(i1+3,i2+4,i3+5,c3)+c(i,4,4,
     & 5)*ui(i1+4,i2+4,i3+5,c3)+c(i,5,4,5)*ui(i1+5,i2+4,i3+5,c3)+c(i,
     & 6,4,5)*ui(i1+6,i2+4,i3+5,c3)+c(i,0,5,5)*ui(i1,i2+5,i3+5,c3)+c(
     & i,1,5,5)*ui(i1+1,i2+5,i3+5,c3)+c(i,2,5,5)*ui(i1+2,i2+5,i3+5,c3)
     & +c(i,3,5,5)*ui(i1+3,i2+5,i3+5,c3)+c(i,4,5,5)*ui(i1+4,i2+5,i3+5,
     & c3)+c(i,5,5,5)*ui(i1+5,i2+5,i3+5,c3)+c(i,6,5,5)*ui(i1+6,i2+5,
     & i3+5,c3)+c(i,0,6,5)*ui(i1,i2+6,i3+5,c3)+c(i,1,6,5)*ui(i1+1,i2+
     & 6,i3+5,c3)+c(i,2,6,5)*ui(i1+2,i2+6,i3+5,c3)+c(i,3,6,5)*ui(i1+3,
     & i2+6,i3+5,c3)+c(i,4,6,5)*ui(i1+4,i2+6,i3+5,c3)+c(i,5,6,5)*ui(
     & i1+5,i2+6,i3+5,c3)+c(i,6,6,5)*ui(i1+6,i2+6,i3+5,c3)+c(i,0,0,6)*
     & ui(i1,i2,i3+6,c3)+c(i,1,0,6)*ui(i1+1,i2,i3+6,c3)+c(i,2,0,6)*ui(
     & i1+2,i2,i3+6,c3)+c(i,3,0,6)*ui(i1+3,i2,i3+6,c3)+c(i,4,0,6)*ui(
     & i1+4,i2,i3+6,c3)+c(i,5,0,6)*ui(i1+5,i2,i3+6,c3)+c(i,6,0,6)*ui(
     & i1+6,i2,i3+6,c3)+c(i,0,1,6)*ui(i1,i2+1,i3+6,c3)+c(i,1,1,6)*ui(
     & i1+1,i2+1,i3+6,c3)+c(i,2,1,6)*ui(i1+2,i2+1,i3+6,c3)+c(i,3,1,6)*
     & ui(i1+3,i2+1,i3+6,c3)+c(i,4,1,6)*ui(i1+4,i2+1,i3+6,c3)+c(i,5,1,
     & 6)*ui(i1+5,i2+1,i3+6,c3)+c(i,6,1,6)*ui(i1+6,i2+1,i3+6,c3)+c(i,
     & 0,2,6)*ui(i1,i2+2,i3+6,c3)+c(i,1,2,6)*ui(i1+1,i2+2,i3+6,c3)+c(
     & i,2,2,6)*ui(i1+2,i2+2,i3+6,c3)+c(i,3,2,6)*ui(i1+3,i2+2,i3+6,c3)
     & +c(i,4,2,6)*ui(i1+4,i2+2,i3+6,c3)+c(i,5,2,6)*ui(i1+5,i2+2,i3+6,
     & c3)+c(i,6,2,6)*ui(i1+6,i2+2,i3+6,c3)+c(i,0,3,6)*ui(i1,i2+3,i3+
     & 6,c3)+c(i,1,3,6)*ui(i1+1,i2+3,i3+6,c3)+c(i,2,3,6)*ui(i1+2,i2+3,
     & i3+6,c3)+c(i,3,3,6)*ui(i1+3,i2+3,i3+6,c3)+c(i,4,3,6)*ui(i1+4,
     & i2+3,i3+6,c3)+c(i,5,3,6)*ui(i1+5,i2+3,i3+6,c3)+c(i,6,3,6)*ui(
     & i1+6,i2+3,i3+6,c3)+c(i,0,4,6)*ui(i1,i2+4,i3+6,c3)+c(i,1,4,6)*
     & ui(i1+1,i2+4,i3+6,c3)+c(i,2,4,6)*ui(i1+2,i2+4,i3+6,c3)+c(i,3,4,
     & 6)*ui(i1+3,i2+4,i3+6,c3)+c(i,4,4,6)*ui(i1+4,i2+4,i3+6,c3)+c(i,
     & 5,4,6)*ui(i1+5,i2+4,i3+6,c3)+c(i,6,4,6)*ui(i1+6,i2+4,i3+6,c3)+
     & c(i,0,5,6)*ui(i1,i2+5,i3+6,c3)+c(i,1,5,6)*ui(i1+1,i2+5,i3+6,c3)
     & +c(i,2,5,6)*ui(i1+2,i2+5,i3+6,c3)+c(i,3,5,6)*ui(i1+3,i2+5,i3+6,
     & c3)+c(i,4,5,6)*ui(i1+4,i2+5,i3+6,c3)+c(i,5,5,6)*ui(i1+5,i2+5,
     & i3+6,c3)+c(i,6,5,6)*ui(i1+6,i2+5,i3+6,c3)+c(i,0,6,6)*ui(i1,i2+
     & 6,i3+6,c3)+c(i,1,6,6)*ui(i1+1,i2+6,i3+6,c3)+c(i,2,6,6)*ui(i1+2,
     & i2+6,i3+6,c3)+c(i,3,6,6)*ui(i1+3,i2+6,i3+6,c3)+c(i,4,6,6)*ui(
     & i1+4,i2+6,i3+6,c3)+c(i,5,6,6)*ui(i1+5,i2+6,i3+6,c3)+c(i,6,6,6)*
     & ui(i1+6,i2+6,i3+6,c3)
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
! interp777(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,0,0)*ui(i1,i2,i3,c3)+c(i,1,0,0)*ui(i1+1,i2,
     & i3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3,c3)+c(i,3,0,0)*ui(i1+3,i2,i3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2,i3,c3)+c(i,5,0,0)*ui(i1+5,i2,i3,c3)+
     & c(i,6,0,0)*ui(i1+6,i2,i3,c3)+c(i,0,1,0)*ui(i1,i2+1,i3,c3)+c(i,
     & 1,1,0)*ui(i1+1,i2+1,i3,c3)+c(i,2,1,0)*ui(i1+2,i2+1,i3,c3)+c(i,
     & 3,1,0)*ui(i1+3,i2+1,i3,c3)+c(i,4,1,0)*ui(i1+4,i2+1,i3,c3)+c(i,
     & 5,1,0)*ui(i1+5,i2+1,i3,c3)+c(i,6,1,0)*ui(i1+6,i2+1,i3,c3)+c(i,
     & 0,2,0)*ui(i1,i2+2,i3,c3)+c(i,1,2,0)*ui(i1+1,i2+2,i3,c3)+c(i,2,
     & 2,0)*ui(i1+2,i2+2,i3,c3)+c(i,3,2,0)*ui(i1+3,i2+2,i3,c3)+c(i,4,
     & 2,0)*ui(i1+4,i2+2,i3,c3)+c(i,5,2,0)*ui(i1+5,i2+2,i3,c3)+c(i,6,
     & 2,0)*ui(i1+6,i2+2,i3,c3)+c(i,0,3,0)*ui(i1,i2+3,i3,c3)+c(i,1,3,
     & 0)*ui(i1+1,i2+3,i3,c3)+c(i,2,3,0)*ui(i1+2,i2+3,i3,c3)+c(i,3,3,
     & 0)*ui(i1+3,i2+3,i3,c3)+c(i,4,3,0)*ui(i1+4,i2+3,i3,c3)+c(i,5,3,
     & 0)*ui(i1+5,i2+3,i3,c3)+c(i,6,3,0)*ui(i1+6,i2+3,i3,c3)+c(i,0,4,
     & 0)*ui(i1,i2+4,i3,c3)+c(i,1,4,0)*ui(i1+1,i2+4,i3,c3)+c(i,2,4,0)*
     & ui(i1+2,i2+4,i3,c3)+c(i,3,4,0)*ui(i1+3,i2+4,i3,c3)+c(i,4,4,0)*
     & ui(i1+4,i2+4,i3,c3)+c(i,5,4,0)*ui(i1+5,i2+4,i3,c3)+c(i,6,4,0)*
     & ui(i1+6,i2+4,i3,c3)+c(i,0,5,0)*ui(i1,i2+5,i3,c3)+c(i,1,5,0)*ui(
     & i1+1,i2+5,i3,c3)+c(i,2,5,0)*ui(i1+2,i2+5,i3,c3)+c(i,3,5,0)*ui(
     & i1+3,i2+5,i3,c3)+c(i,4,5,0)*ui(i1+4,i2+5,i3,c3)+c(i,5,5,0)*ui(
     & i1+5,i2+5,i3,c3)+c(i,6,5,0)*ui(i1+6,i2+5,i3,c3)+c(i,0,6,0)*ui(
     & i1,i2+6,i3,c3)+c(i,1,6,0)*ui(i1+1,i2+6,i3,c3)+c(i,2,6,0)*ui(i1+
     & 2,i2+6,i3,c3)+c(i,3,6,0)*ui(i1+3,i2+6,i3,c3)+c(i,4,6,0)*ui(i1+
     & 4,i2+6,i3,c3)+c(i,5,6,0)*ui(i1+5,i2+6,i3,c3)+c(i,6,6,0)*ui(i1+
     & 6,i2+6,i3,c3)+c(i,0,0,1)*ui(i1,i2,i3+1,c3)+c(i,1,0,1)*ui(i1+1,
     & i2,i3+1,c3)+c(i,2,0,1)*ui(i1+2,i2,i3+1,c3)+c(i,3,0,1)*ui(i1+3,
     & i2,i3+1,c3)+c(i,4,0,1)*ui(i1+4,i2,i3+1,c3)+c(i,5,0,1)*ui(i1+5,
     & i2,i3+1,c3)+c(i,6,0,1)*ui(i1+6,i2,i3+1,c3)+c(i,0,1,1)*ui(i1,i2+
     & 1,i3+1,c3)+c(i,1,1,1)*ui(i1+1,i2+1,i3+1,c3)+c(i,2,1,1)*ui(i1+2,
     & i2+1,i3+1,c3)+c(i,3,1,1)*ui(i1+3,i2+1,i3+1,c3)+c(i,4,1,1)*ui(
     & i1+4,i2+1,i3+1,c3)+c(i,5,1,1)*ui(i1+5,i2+1,i3+1,c3)+c(i,6,1,1)*
     & ui(i1+6,i2+1,i3+1,c3)+c(i,0,2,1)*ui(i1,i2+2,i3+1,c3)+c(i,1,2,1)
     & *ui(i1+1,i2+2,i3+1,c3)+c(i,2,2,1)*ui(i1+2,i2+2,i3+1,c3)+c(i,3,
     & 2,1)*ui(i1+3,i2+2,i3+1,c3)+c(i,4,2,1)*ui(i1+4,i2+2,i3+1,c3)+c(
     & i,5,2,1)*ui(i1+5,i2+2,i3+1,c3)+c(i,6,2,1)*ui(i1+6,i2+2,i3+1,c3)
     & +c(i,0,3,1)*ui(i1,i2+3,i3+1,c3)+c(i,1,3,1)*ui(i1+1,i2+3,i3+1,
     & c3)+c(i,2,3,1)*ui(i1+2,i2+3,i3+1,c3)+c(i,3,3,1)*ui(i1+3,i2+3,
     & i3+1,c3)+c(i,4,3,1)*ui(i1+4,i2+3,i3+1,c3)+c(i,5,3,1)*ui(i1+5,
     & i2+3,i3+1,c3)+c(i,6,3,1)*ui(i1+6,i2+3,i3+1,c3)+c(i,0,4,1)*ui(
     & i1,i2+4,i3+1,c3)+c(i,1,4,1)*ui(i1+1,i2+4,i3+1,c3)+c(i,2,4,1)*
     & ui(i1+2,i2+4,i3+1,c3)+c(i,3,4,1)*ui(i1+3,i2+4,i3+1,c3)+c(i,4,4,
     & 1)*ui(i1+4,i2+4,i3+1,c3)+c(i,5,4,1)*ui(i1+5,i2+4,i3+1,c3)+c(i,
     & 6,4,1)*ui(i1+6,i2+4,i3+1,c3)+c(i,0,5,1)*ui(i1,i2+5,i3+1,c3)+c(
     & i,1,5,1)*ui(i1+1,i2+5,i3+1,c3)+c(i,2,5,1)*ui(i1+2,i2+5,i3+1,c3)
     & +c(i,3,5,1)*ui(i1+3,i2+5,i3+1,c3)+c(i,4,5,1)*ui(i1+4,i2+5,i3+1,
     & c3)+c(i,5,5,1)*ui(i1+5,i2+5,i3+1,c3)+c(i,6,5,1)*ui(i1+6,i2+5,
     & i3+1,c3)+c(i,0,6,1)*ui(i1,i2+6,i3+1,c3)+c(i,1,6,1)*ui(i1+1,i2+
     & 6,i3+1,c3)+c(i,2,6,1)*ui(i1+2,i2+6,i3+1,c3)+c(i,3,6,1)*ui(i1+3,
     & i2+6,i3+1,c3)+c(i,4,6,1)*ui(i1+4,i2+6,i3+1,c3)+c(i,5,6,1)*ui(
     & i1+5,i2+6,i3+1,c3)+c(i,6,6,1)*ui(i1+6,i2+6,i3+1,c3)+c(i,0,0,2)*
     & ui(i1,i2,i3+2,c3)+c(i,1,0,2)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,2)*ui(
     & i1+2,i2,i3+2,c3)+c(i,3,0,2)*ui(i1+3,i2,i3+2,c3)+c(i,4,0,2)*ui(
     & i1+4,i2,i3+2,c3)+c(i,5,0,2)*ui(i1+5,i2,i3+2,c3)+c(i,6,0,2)*ui(
     & i1+6,i2,i3+2,c3)+c(i,0,1,2)*ui(i1,i2+1,i3+2,c3)+c(i,1,1,2)*ui(
     & i1+1,i2+1,i3+2,c3)+c(i,2,1,2)*ui(i1+2,i2+1,i3+2,c3)+c(i,3,1,2)*
     & ui(i1+3,i2+1,i3+2,c3)+c(i,4,1,2)*ui(i1+4,i2+1,i3+2,c3)+c(i,5,1,
     & 2)*ui(i1+5,i2+1,i3+2,c3)+c(i,6,1,2)*ui(i1+6,i2+1,i3+2,c3)+c(i,
     & 0,2,2)*ui(i1,i2+2,i3+2,c3)+c(i,1,2,2)*ui(i1+1,i2+2,i3+2,c3)+c(
     & i,2,2,2)*ui(i1+2,i2+2,i3+2,c3)+c(i,3,2,2)*ui(i1+3,i2+2,i3+2,c3)
     & +c(i,4,2,2)*ui(i1+4,i2+2,i3+2,c3)+c(i,5,2,2)*ui(i1+5,i2+2,i3+2,
     & c3)+c(i,6,2,2)*ui(i1+6,i2+2,i3+2,c3)+c(i,0,3,2)*ui(i1,i2+3,i3+
     & 2,c3)+c(i,1,3,2)*ui(i1+1,i2+3,i3+2,c3)+c(i,2,3,2)*ui(i1+2,i2+3,
     & i3+2,c3)+c(i,3,3,2)*ui(i1+3,i2+3,i3+2,c3)+c(i,4,3,2)*ui(i1+4,
     & i2+3,i3+2,c3)+c(i,5,3,2)*ui(i1+5,i2+3,i3+2,c3)+c(i,6,3,2)*ui(
     & i1+6,i2+3,i3+2,c3)+c(i,0,4,2)*ui(i1,i2+4,i3+2,c3)+c(i,1,4,2)*
     & ui(i1+1,i2+4,i3+2,c3)+c(i,2,4,2)*ui(i1+2,i2+4,i3+2,c3)+c(i,3,4,
     & 2)*ui(i1+3,i2+4,i3+2,c3)+c(i,4,4,2)*ui(i1+4,i2+4,i3+2,c3)+c(i,
     & 5,4,2)*ui(i1+5,i2+4,i3+2,c3)+c(i,6,4,2)*ui(i1+6,i2+4,i3+2,c3)+
     & c(i,0,5,2)*ui(i1,i2+5,i3+2,c3)+c(i,1,5,2)*ui(i1+1,i2+5,i3+2,c3)
     & +c(i,2,5,2)*ui(i1+2,i2+5,i3+2,c3)+c(i,3,5,2)*ui(i1+3,i2+5,i3+2,
     & c3)+c(i,4,5,2)*ui(i1+4,i2+5,i3+2,c3)+c(i,5,5,2)*ui(i1+5,i2+5,
     & i3+2,c3)+c(i,6,5,2)*ui(i1+6,i2+5,i3+2,c3)+c(i,0,6,2)*ui(i1,i2+
     & 6,i3+2,c3)+c(i,1,6,2)*ui(i1+1,i2+6,i3+2,c3)+c(i,2,6,2)*ui(i1+2,
     & i2+6,i3+2,c3)+c(i,3,6,2)*ui(i1+3,i2+6,i3+2,c3)+c(i,4,6,2)*ui(
     & i1+4,i2+6,i3+2,c3)+c(i,5,6,2)*ui(i1+5,i2+6,i3+2,c3)+c(i,6,6,2)*
     & ui(i1+6,i2+6,i3+2,c3)+c(i,0,0,3)*ui(i1,i2,i3+3,c3)+c(i,1,0,3)*
     & ui(i1+1,i2,i3+3,c3)+c(i,2,0,3)*ui(i1+2,i2,i3+3,c3)+c(i,3,0,3)*
     & ui(i1+3,i2,i3+3,c3)+c(i,4,0,3)*ui(i1+4,i2,i3+3,c3)+c(i,5,0,3)*
     & ui(i1+5,i2,i3+3,c3)+c(i,6,0,3)*ui(i1+6,i2,i3+3,c3)+c(i,0,1,3)*
     & ui(i1,i2+1,i3+3,c3)+c(i,1,1,3)*ui(i1+1,i2+1,i3+3,c3)+c(i,2,1,3)
     & *ui(i1+2,i2+1,i3+3,c3)+c(i,3,1,3)*ui(i1+3,i2+1,i3+3,c3)+c(i,4,
     & 1,3)*ui(i1+4,i2+1,i3+3,c3)+c(i,5,1,3)*ui(i1+5,i2+1,i3+3,c3)+c(
     & i,6,1,3)*ui(i1+6,i2+1,i3+3,c3)+c(i,0,2,3)*ui(i1,i2+2,i3+3,c3)+
     & c(i,1,2,3)*ui(i1+1,i2+2,i3+3,c3)+c(i,2,2,3)*ui(i1+2,i2+2,i3+3,
     & c3)+c(i,3,2,3)*ui(i1+3,i2+2,i3+3,c3)+c(i,4,2,3)*ui(i1+4,i2+2,
     & i3+3,c3)+c(i,5,2,3)*ui(i1+5,i2+2,i3+3,c3)+c(i,6,2,3)*ui(i1+6,
     & i2+2,i3+3,c3)+c(i,0,3,3)*ui(i1,i2+3,i3+3,c3)+c(i,1,3,3)*ui(i1+
     & 1,i2+3,i3+3,c3)+c(i,2,3,3)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,3,3)*ui(
     & i1+3,i2+3,i3+3,c3)+c(i,4,3,3)*ui(i1+4,i2+3,i3+3,c3)+c(i,5,3,3)*
     & ui(i1+5,i2+3,i3+3,c3)+c(i,6,3,3)*ui(i1+6,i2+3,i3+3,c3)+c(i,0,4,
     & 3)*ui(i1,i2+4,i3+3,c3)+c(i,1,4,3)*ui(i1+1,i2+4,i3+3,c3)+c(i,2,
     & 4,3)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,4,3)*ui(i1+3,i2+4,i3+3,c3)+c(
     & i,4,4,3)*ui(i1+4,i2+4,i3+3,c3)+c(i,5,4,3)*ui(i1+5,i2+4,i3+3,c3)
     & +c(i,6,4,3)*ui(i1+6,i2+4,i3+3,c3)+c(i,0,5,3)*ui(i1,i2+5,i3+3,
     & c3)+c(i,1,5,3)*ui(i1+1,i2+5,i3+3,c3)
               r(i) = r(i)+c(i,2,5,3)*ui(i1+2,i2+5,i3+3,c3)+c(i,3,5,3)*
     & ui(i1+3,i2+5,i3+3,c3)+c(i,4,5,3)*ui(i1+4,i2+5,i3+3,c3)+c(i,5,5,
     & 3)*ui(i1+5,i2+5,i3+3,c3)+c(i,6,5,3)*ui(i1+6,i2+5,i3+3,c3)+c(i,
     & 0,6,3)*ui(i1,i2+6,i3+3,c3)+c(i,1,6,3)*ui(i1+1,i2+6,i3+3,c3)+c(
     & i,2,6,3)*ui(i1+2,i2+6,i3+3,c3)+c(i,3,6,3)*ui(i1+3,i2+6,i3+3,c3)
     & +c(i,4,6,3)*ui(i1+4,i2+6,i3+3,c3)+c(i,5,6,3)*ui(i1+5,i2+6,i3+3,
     & c3)+c(i,6,6,3)*ui(i1+6,i2+6,i3+3,c3)+c(i,0,0,4)*ui(i1,i2,i3+4,
     & c3)+c(i,1,0,4)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,4)*ui(i1+2,i2,i3+4,
     & c3)+c(i,3,0,4)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,4)*ui(i1+4,i2,i3+4,
     & c3)+c(i,5,0,4)*ui(i1+5,i2,i3+4,c3)+c(i,6,0,4)*ui(i1+6,i2,i3+4,
     & c3)+c(i,0,1,4)*ui(i1,i2+1,i3+4,c3)+c(i,1,1,4)*ui(i1+1,i2+1,i3+
     & 4,c3)+c(i,2,1,4)*ui(i1+2,i2+1,i3+4,c3)+c(i,3,1,4)*ui(i1+3,i2+1,
     & i3+4,c3)+c(i,4,1,4)*ui(i1+4,i2+1,i3+4,c3)+c(i,5,1,4)*ui(i1+5,
     & i2+1,i3+4,c3)+c(i,6,1,4)*ui(i1+6,i2+1,i3+4,c3)+c(i,0,2,4)*ui(
     & i1,i2+2,i3+4,c3)+c(i,1,2,4)*ui(i1+1,i2+2,i3+4,c3)+c(i,2,2,4)*
     & ui(i1+2,i2+2,i3+4,c3)+c(i,3,2,4)*ui(i1+3,i2+2,i3+4,c3)+c(i,4,2,
     & 4)*ui(i1+4,i2+2,i3+4,c3)+c(i,5,2,4)*ui(i1+5,i2+2,i3+4,c3)+c(i,
     & 6,2,4)*ui(i1+6,i2+2,i3+4,c3)+c(i,0,3,4)*ui(i1,i2+3,i3+4,c3)+c(
     & i,1,3,4)*ui(i1+1,i2+3,i3+4,c3)+c(i,2,3,4)*ui(i1+2,i2+3,i3+4,c3)
     & +c(i,3,3,4)*ui(i1+3,i2+3,i3+4,c3)+c(i,4,3,4)*ui(i1+4,i2+3,i3+4,
     & c3)+c(i,5,3,4)*ui(i1+5,i2+3,i3+4,c3)+c(i,6,3,4)*ui(i1+6,i2+3,
     & i3+4,c3)+c(i,0,4,4)*ui(i1,i2+4,i3+4,c3)+c(i,1,4,4)*ui(i1+1,i2+
     & 4,i3+4,c3)+c(i,2,4,4)*ui(i1+2,i2+4,i3+4,c3)+c(i,3,4,4)*ui(i1+3,
     & i2+4,i3+4,c3)+c(i,4,4,4)*ui(i1+4,i2+4,i3+4,c3)+c(i,5,4,4)*ui(
     & i1+5,i2+4,i3+4,c3)+c(i,6,4,4)*ui(i1+6,i2+4,i3+4,c3)+c(i,0,5,4)*
     & ui(i1,i2+5,i3+4,c3)+c(i,1,5,4)*ui(i1+1,i2+5,i3+4,c3)+c(i,2,5,4)
     & *ui(i1+2,i2+5,i3+4,c3)+c(i,3,5,4)*ui(i1+3,i2+5,i3+4,c3)+c(i,4,
     & 5,4)*ui(i1+4,i2+5,i3+4,c3)+c(i,5,5,4)*ui(i1+5,i2+5,i3+4,c3)+c(
     & i,6,5,4)*ui(i1+6,i2+5,i3+4,c3)+c(i,0,6,4)*ui(i1,i2+6,i3+4,c3)+
     & c(i,1,6,4)*ui(i1+1,i2+6,i3+4,c3)+c(i,2,6,4)*ui(i1+2,i2+6,i3+4,
     & c3)+c(i,3,6,4)*ui(i1+3,i2+6,i3+4,c3)+c(i,4,6,4)*ui(i1+4,i2+6,
     & i3+4,c3)+c(i,5,6,4)*ui(i1+5,i2+6,i3+4,c3)+c(i,6,6,4)*ui(i1+6,
     & i2+6,i3+4,c3)+c(i,0,0,5)*ui(i1,i2,i3+5,c3)+c(i,1,0,5)*ui(i1+1,
     & i2,i3+5,c3)+c(i,2,0,5)*ui(i1+2,i2,i3+5,c3)+c(i,3,0,5)*ui(i1+3,
     & i2,i3+5,c3)+c(i,4,0,5)*ui(i1+4,i2,i3+5,c3)+c(i,5,0,5)*ui(i1+5,
     & i2,i3+5,c3)+c(i,6,0,5)*ui(i1+6,i2,i3+5,c3)+c(i,0,1,5)*ui(i1,i2+
     & 1,i3+5,c3)+c(i,1,1,5)*ui(i1+1,i2+1,i3+5,c3)+c(i,2,1,5)*ui(i1+2,
     & i2+1,i3+5,c3)+c(i,3,1,5)*ui(i1+3,i2+1,i3+5,c3)+c(i,4,1,5)*ui(
     & i1+4,i2+1,i3+5,c3)+c(i,5,1,5)*ui(i1+5,i2+1,i3+5,c3)+c(i,6,1,5)*
     & ui(i1+6,i2+1,i3+5,c3)+c(i,0,2,5)*ui(i1,i2+2,i3+5,c3)+c(i,1,2,5)
     & *ui(i1+1,i2+2,i3+5,c3)+c(i,2,2,5)*ui(i1+2,i2+2,i3+5,c3)+c(i,3,
     & 2,5)*ui(i1+3,i2+2,i3+5,c3)+c(i,4,2,5)*ui(i1+4,i2+2,i3+5,c3)+c(
     & i,5,2,5)*ui(i1+5,i2+2,i3+5,c3)+c(i,6,2,5)*ui(i1+6,i2+2,i3+5,c3)
     & +c(i,0,3,5)*ui(i1,i2+3,i3+5,c3)+c(i,1,3,5)*ui(i1+1,i2+3,i3+5,
     & c3)+c(i,2,3,5)*ui(i1+2,i2+3,i3+5,c3)+c(i,3,3,5)*ui(i1+3,i2+3,
     & i3+5,c3)+c(i,4,3,5)*ui(i1+4,i2+3,i3+5,c3)+c(i,5,3,5)*ui(i1+5,
     & i2+3,i3+5,c3)+c(i,6,3,5)*ui(i1+6,i2+3,i3+5,c3)+c(i,0,4,5)*ui(
     & i1,i2+4,i3+5,c3)+c(i,1,4,5)*ui(i1+1,i2+4,i3+5,c3)+c(i,2,4,5)*
     & ui(i1+2,i2+4,i3+5,c3)+c(i,3,4,5)*ui(i1+3,i2+4,i3+5,c3)+c(i,4,4,
     & 5)*ui(i1+4,i2+4,i3+5,c3)+c(i,5,4,5)*ui(i1+5,i2+4,i3+5,c3)+c(i,
     & 6,4,5)*ui(i1+6,i2+4,i3+5,c3)+c(i,0,5,5)*ui(i1,i2+5,i3+5,c3)+c(
     & i,1,5,5)*ui(i1+1,i2+5,i3+5,c3)+c(i,2,5,5)*ui(i1+2,i2+5,i3+5,c3)
     & +c(i,3,5,5)*ui(i1+3,i2+5,i3+5,c3)+c(i,4,5,5)*ui(i1+4,i2+5,i3+5,
     & c3)+c(i,5,5,5)*ui(i1+5,i2+5,i3+5,c3)+c(i,6,5,5)*ui(i1+6,i2+5,
     & i3+5,c3)+c(i,0,6,5)*ui(i1,i2+6,i3+5,c3)+c(i,1,6,5)*ui(i1+1,i2+
     & 6,i3+5,c3)+c(i,2,6,5)*ui(i1+2,i2+6,i3+5,c3)+c(i,3,6,5)*ui(i1+3,
     & i2+6,i3+5,c3)+c(i,4,6,5)*ui(i1+4,i2+6,i3+5,c3)+c(i,5,6,5)*ui(
     & i1+5,i2+6,i3+5,c3)+c(i,6,6,5)*ui(i1+6,i2+6,i3+5,c3)+c(i,0,0,6)*
     & ui(i1,i2,i3+6,c3)+c(i,1,0,6)*ui(i1+1,i2,i3+6,c3)+c(i,2,0,6)*ui(
     & i1+2,i2,i3+6,c3)+c(i,3,0,6)*ui(i1+3,i2,i3+6,c3)+c(i,4,0,6)*ui(
     & i1+4,i2,i3+6,c3)+c(i,5,0,6)*ui(i1+5,i2,i3+6,c3)+c(i,6,0,6)*ui(
     & i1+6,i2,i3+6,c3)+c(i,0,1,6)*ui(i1,i2+1,i3+6,c3)+c(i,1,1,6)*ui(
     & i1+1,i2+1,i3+6,c3)+c(i,2,1,6)*ui(i1+2,i2+1,i3+6,c3)+c(i,3,1,6)*
     & ui(i1+3,i2+1,i3+6,c3)+c(i,4,1,6)*ui(i1+4,i2+1,i3+6,c3)+c(i,5,1,
     & 6)*ui(i1+5,i2+1,i3+6,c3)+c(i,6,1,6)*ui(i1+6,i2+1,i3+6,c3)+c(i,
     & 0,2,6)*ui(i1,i2+2,i3+6,c3)+c(i,1,2,6)*ui(i1+1,i2+2,i3+6,c3)+c(
     & i,2,2,6)*ui(i1+2,i2+2,i3+6,c3)+c(i,3,2,6)*ui(i1+3,i2+2,i3+6,c3)
     & +c(i,4,2,6)*ui(i1+4,i2+2,i3+6,c3)+c(i,5,2,6)*ui(i1+5,i2+2,i3+6,
     & c3)+c(i,6,2,6)*ui(i1+6,i2+2,i3+6,c3)+c(i,0,3,6)*ui(i1,i2+3,i3+
     & 6,c3)+c(i,1,3,6)*ui(i1+1,i2+3,i3+6,c3)+c(i,2,3,6)*ui(i1+2,i2+3,
     & i3+6,c3)+c(i,3,3,6)*ui(i1+3,i2+3,i3+6,c3)+c(i,4,3,6)*ui(i1+4,
     & i2+3,i3+6,c3)+c(i,5,3,6)*ui(i1+5,i2+3,i3+6,c3)+c(i,6,3,6)*ui(
     & i1+6,i2+3,i3+6,c3)+c(i,0,4,6)*ui(i1,i2+4,i3+6,c3)+c(i,1,4,6)*
     & ui(i1+1,i2+4,i3+6,c3)+c(i,2,4,6)*ui(i1+2,i2+4,i3+6,c3)+c(i,3,4,
     & 6)*ui(i1+3,i2+4,i3+6,c3)+c(i,4,4,6)*ui(i1+4,i2+4,i3+6,c3)+c(i,
     & 5,4,6)*ui(i1+5,i2+4,i3+6,c3)+c(i,6,4,6)*ui(i1+6,i2+4,i3+6,c3)+
     & c(i,0,5,6)*ui(i1,i2+5,i3+6,c3)+c(i,1,5,6)*ui(i1+1,i2+5,i3+6,c3)
     & +c(i,2,5,6)*ui(i1+2,i2+5,i3+6,c3)+c(i,3,5,6)*ui(i1+3,i2+5,i3+6,
     & c3)+c(i,4,5,6)*ui(i1+4,i2+5,i3+6,c3)+c(i,5,5,6)*ui(i1+5,i2+5,
     & i3+6,c3)+c(i,6,5,6)*ui(i1+6,i2+5,i3+6,c3)+c(i,0,6,6)*ui(i1,i2+
     & 6,i3+6,c3)+c(i,1,6,6)*ui(i1+1,i2+6,i3+6,c3)+c(i,2,6,6)*ui(i1+2,
     & i2+6,i3+6,c3)+c(i,3,6,6)*ui(i1+3,i2+6,i3+6,c3)+c(i,4,6,6)*ui(
     & i1+4,i2+6,i3+6,c3)+c(i,5,6,6)*ui(i1+5,i2+6,i3+6,c3)+c(i,6,6,6)*
     & ui(i1+6,i2+6,i3+6,c3)
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           end if
         else if( width(1).eq.8 .and. width(2).eq.8 .and. width(3)
     & .eq.8 )then
! loops3d($interp888(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))
           if( c3a.eq.c3b )then
             do c3=c3a,c3b
             do i=nia,nib
! interp888(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,0,0)*ui(i1,i2,i3,c3)+c(i,1,0,0)*ui(i1+1,i2,
     & i3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3,c3)+c(i,3,0,0)*ui(i1+3,i2,i3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2,i3,c3)+c(i,5,0,0)*ui(i1+5,i2,i3,c3)+
     & c(i,6,0,0)*ui(i1+6,i2,i3,c3)+c(i,7,0,0)*ui(i1+7,i2,i3,c3)+c(i,
     & 0,1,0)*ui(i1,i2+1,i3,c3)+c(i,1,1,0)*ui(i1+1,i2+1,i3,c3)+c(i,2,
     & 1,0)*ui(i1+2,i2+1,i3,c3)+c(i,3,1,0)*ui(i1+3,i2+1,i3,c3)+c(i,4,
     & 1,0)*ui(i1+4,i2+1,i3,c3)+c(i,5,1,0)*ui(i1+5,i2+1,i3,c3)+c(i,6,
     & 1,0)*ui(i1+6,i2+1,i3,c3)+c(i,7,1,0)*ui(i1+7,i2+1,i3,c3)+c(i,0,
     & 2,0)*ui(i1,i2+2,i3,c3)+c(i,1,2,0)*ui(i1+1,i2+2,i3,c3)+c(i,2,2,
     & 0)*ui(i1+2,i2+2,i3,c3)+c(i,3,2,0)*ui(i1+3,i2+2,i3,c3)+c(i,4,2,
     & 0)*ui(i1+4,i2+2,i3,c3)+c(i,5,2,0)*ui(i1+5,i2+2,i3,c3)+c(i,6,2,
     & 0)*ui(i1+6,i2+2,i3,c3)+c(i,7,2,0)*ui(i1+7,i2+2,i3,c3)+c(i,0,3,
     & 0)*ui(i1,i2+3,i3,c3)+c(i,1,3,0)*ui(i1+1,i2+3,i3,c3)+c(i,2,3,0)*
     & ui(i1+2,i2+3,i3,c3)+c(i,3,3,0)*ui(i1+3,i2+3,i3,c3)+c(i,4,3,0)*
     & ui(i1+4,i2+3,i3,c3)+c(i,5,3,0)*ui(i1+5,i2+3,i3,c3)+c(i,6,3,0)*
     & ui(i1+6,i2+3,i3,c3)+c(i,7,3,0)*ui(i1+7,i2+3,i3,c3)+c(i,0,4,0)*
     & ui(i1,i2+4,i3,c3)+c(i,1,4,0)*ui(i1+1,i2+4,i3,c3)+c(i,2,4,0)*ui(
     & i1+2,i2+4,i3,c3)+c(i,3,4,0)*ui(i1+3,i2+4,i3,c3)+c(i,4,4,0)*ui(
     & i1+4,i2+4,i3,c3)+c(i,5,4,0)*ui(i1+5,i2+4,i3,c3)+c(i,6,4,0)*ui(
     & i1+6,i2+4,i3,c3)+c(i,7,4,0)*ui(i1+7,i2+4,i3,c3)+c(i,0,5,0)*ui(
     & i1,i2+5,i3,c3)+c(i,1,5,0)*ui(i1+1,i2+5,i3,c3)+c(i,2,5,0)*ui(i1+
     & 2,i2+5,i3,c3)+c(i,3,5,0)*ui(i1+3,i2+5,i3,c3)+c(i,4,5,0)*ui(i1+
     & 4,i2+5,i3,c3)+c(i,5,5,0)*ui(i1+5,i2+5,i3,c3)+c(i,6,5,0)*ui(i1+
     & 6,i2+5,i3,c3)+c(i,7,5,0)*ui(i1+7,i2+5,i3,c3)+c(i,0,6,0)*ui(i1,
     & i2+6,i3,c3)+c(i,1,6,0)*ui(i1+1,i2+6,i3,c3)+c(i,2,6,0)*ui(i1+2,
     & i2+6,i3,c3)+c(i,3,6,0)*ui(i1+3,i2+6,i3,c3)+c(i,4,6,0)*ui(i1+4,
     & i2+6,i3,c3)+c(i,5,6,0)*ui(i1+5,i2+6,i3,c3)+c(i,6,6,0)*ui(i1+6,
     & i2+6,i3,c3)+c(i,7,6,0)*ui(i1+7,i2+6,i3,c3)+c(i,0,7,0)*ui(i1,i2+
     & 7,i3,c3)+c(i,1,7,0)*ui(i1+1,i2+7,i3,c3)+c(i,2,7,0)*ui(i1+2,i2+
     & 7,i3,c3)+c(i,3,7,0)*ui(i1+3,i2+7,i3,c3)+c(i,4,7,0)*ui(i1+4,i2+
     & 7,i3,c3)+c(i,5,7,0)*ui(i1+5,i2+7,i3,c3)+c(i,6,7,0)*ui(i1+6,i2+
     & 7,i3,c3)+c(i,7,7,0)*ui(i1+7,i2+7,i3,c3)+c(i,0,0,1)*ui(i1,i2,i3+
     & 1,c3)+c(i,1,0,1)*ui(i1+1,i2,i3+1,c3)+c(i,2,0,1)*ui(i1+2,i2,i3+
     & 1,c3)+c(i,3,0,1)*ui(i1+3,i2,i3+1,c3)+c(i,4,0,1)*ui(i1+4,i2,i3+
     & 1,c3)+c(i,5,0,1)*ui(i1+5,i2,i3+1,c3)+c(i,6,0,1)*ui(i1+6,i2,i3+
     & 1,c3)+c(i,7,0,1)*ui(i1+7,i2,i3+1,c3)+c(i,0,1,1)*ui(i1,i2+1,i3+
     & 1,c3)+c(i,1,1,1)*ui(i1+1,i2+1,i3+1,c3)+c(i,2,1,1)*ui(i1+2,i2+1,
     & i3+1,c3)+c(i,3,1,1)*ui(i1+3,i2+1,i3+1,c3)+c(i,4,1,1)*ui(i1+4,
     & i2+1,i3+1,c3)+c(i,5,1,1)*ui(i1+5,i2+1,i3+1,c3)+c(i,6,1,1)*ui(
     & i1+6,i2+1,i3+1,c3)+c(i,7,1,1)*ui(i1+7,i2+1,i3+1,c3)+c(i,0,2,1)*
     & ui(i1,i2+2,i3+1,c3)+c(i,1,2,1)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,2,1)
     & *ui(i1+2,i2+2,i3+1,c3)+c(i,3,2,1)*ui(i1+3,i2+2,i3+1,c3)+c(i,4,
     & 2,1)*ui(i1+4,i2+2,i3+1,c3)+c(i,5,2,1)*ui(i1+5,i2+2,i3+1,c3)+c(
     & i,6,2,1)*ui(i1+6,i2+2,i3+1,c3)+c(i,7,2,1)*ui(i1+7,i2+2,i3+1,c3)
     & +c(i,0,3,1)*ui(i1,i2+3,i3+1,c3)+c(i,1,3,1)*ui(i1+1,i2+3,i3+1,
     & c3)+c(i,2,3,1)*ui(i1+2,i2+3,i3+1,c3)+c(i,3,3,1)*ui(i1+3,i2+3,
     & i3+1,c3)+c(i,4,3,1)*ui(i1+4,i2+3,i3+1,c3)+c(i,5,3,1)*ui(i1+5,
     & i2+3,i3+1,c3)+c(i,6,3,1)*ui(i1+6,i2+3,i3+1,c3)+c(i,7,3,1)*ui(
     & i1+7,i2+3,i3+1,c3)+c(i,0,4,1)*ui(i1,i2+4,i3+1,c3)+c(i,1,4,1)*
     & ui(i1+1,i2+4,i3+1,c3)+c(i,2,4,1)*ui(i1+2,i2+4,i3+1,c3)+c(i,3,4,
     & 1)*ui(i1+3,i2+4,i3+1,c3)+c(i,4,4,1)*ui(i1+4,i2+4,i3+1,c3)+c(i,
     & 5,4,1)*ui(i1+5,i2+4,i3+1,c3)+c(i,6,4,1)*ui(i1+6,i2+4,i3+1,c3)+
     & c(i,7,4,1)*ui(i1+7,i2+4,i3+1,c3)+c(i,0,5,1)*ui(i1,i2+5,i3+1,c3)
     & +c(i,1,5,1)*ui(i1+1,i2+5,i3+1,c3)+c(i,2,5,1)*ui(i1+2,i2+5,i3+1,
     & c3)+c(i,3,5,1)*ui(i1+3,i2+5,i3+1,c3)+c(i,4,5,1)*ui(i1+4,i2+5,
     & i3+1,c3)+c(i,5,5,1)*ui(i1+5,i2+5,i3+1,c3)+c(i,6,5,1)*ui(i1+6,
     & i2+5,i3+1,c3)+c(i,7,5,1)*ui(i1+7,i2+5,i3+1,c3)+c(i,0,6,1)*ui(
     & i1,i2+6,i3+1,c3)+c(i,1,6,1)*ui(i1+1,i2+6,i3+1,c3)+c(i,2,6,1)*
     & ui(i1+2,i2+6,i3+1,c3)+c(i,3,6,1)*ui(i1+3,i2+6,i3+1,c3)+c(i,4,6,
     & 1)*ui(i1+4,i2+6,i3+1,c3)+c(i,5,6,1)*ui(i1+5,i2+6,i3+1,c3)+c(i,
     & 6,6,1)*ui(i1+6,i2+6,i3+1,c3)+c(i,7,6,1)*ui(i1+7,i2+6,i3+1,c3)+
     & c(i,0,7,1)*ui(i1,i2+7,i3+1,c3)+c(i,1,7,1)*ui(i1+1,i2+7,i3+1,c3)
     & +c(i,2,7,1)*ui(i1+2,i2+7,i3+1,c3)+c(i,3,7,1)*ui(i1+3,i2+7,i3+1,
     & c3)+c(i,4,7,1)*ui(i1+4,i2+7,i3+1,c3)+c(i,5,7,1)*ui(i1+5,i2+7,
     & i3+1,c3)+c(i,6,7,1)*ui(i1+6,i2+7,i3+1,c3)+c(i,7,7,1)*ui(i1+7,
     & i2+7,i3+1,c3)+c(i,0,0,2)*ui(i1,i2,i3+2,c3)+c(i,1,0,2)*ui(i1+1,
     & i2,i3+2,c3)+c(i,2,0,2)*ui(i1+2,i2,i3+2,c3)+c(i,3,0,2)*ui(i1+3,
     & i2,i3+2,c3)+c(i,4,0,2)*ui(i1+4,i2,i3+2,c3)+c(i,5,0,2)*ui(i1+5,
     & i2,i3+2,c3)+c(i,6,0,2)*ui(i1+6,i2,i3+2,c3)+c(i,7,0,2)*ui(i1+7,
     & i2,i3+2,c3)+c(i,0,1,2)*ui(i1,i2+1,i3+2,c3)+c(i,1,1,2)*ui(i1+1,
     & i2+1,i3+2,c3)+c(i,2,1,2)*ui(i1+2,i2+1,i3+2,c3)+c(i,3,1,2)*ui(
     & i1+3,i2+1,i3+2,c3)+c(i,4,1,2)*ui(i1+4,i2+1,i3+2,c3)+c(i,5,1,2)*
     & ui(i1+5,i2+1,i3+2,c3)+c(i,6,1,2)*ui(i1+6,i2+1,i3+2,c3)+c(i,7,1,
     & 2)*ui(i1+7,i2+1,i3+2,c3)+c(i,0,2,2)*ui(i1,i2+2,i3+2,c3)+c(i,1,
     & 2,2)*ui(i1+1,i2+2,i3+2,c3)+c(i,2,2,2)*ui(i1+2,i2+2,i3+2,c3)+c(
     & i,3,2,2)*ui(i1+3,i2+2,i3+2,c3)+c(i,4,2,2)*ui(i1+4,i2+2,i3+2,c3)
     & +c(i,5,2,2)*ui(i1+5,i2+2,i3+2,c3)+c(i,6,2,2)*ui(i1+6,i2+2,i3+2,
     & c3)+c(i,7,2,2)*ui(i1+7,i2+2,i3+2,c3)+c(i,0,3,2)*ui(i1,i2+3,i3+
     & 2,c3)+c(i,1,3,2)*ui(i1+1,i2+3,i3+2,c3)+c(i,2,3,2)*ui(i1+2,i2+3,
     & i3+2,c3)+c(i,3,3,2)*ui(i1+3,i2+3,i3+2,c3)+c(i,4,3,2)*ui(i1+4,
     & i2+3,i3+2,c3)+c(i,5,3,2)*ui(i1+5,i2+3,i3+2,c3)+c(i,6,3,2)*ui(
     & i1+6,i2+3,i3+2,c3)+c(i,7,3,2)*ui(i1+7,i2+3,i3+2,c3)+c(i,0,4,2)*
     & ui(i1,i2+4,i3+2,c3)+c(i,1,4,2)*ui(i1+1,i2+4,i3+2,c3)+c(i,2,4,2)
     & *ui(i1+2,i2+4,i3+2,c3)+c(i,3,4,2)*ui(i1+3,i2+4,i3+2,c3)+c(i,4,
     & 4,2)*ui(i1+4,i2+4,i3+2,c3)+c(i,5,4,2)*ui(i1+5,i2+4,i3+2,c3)+c(
     & i,6,4,2)*ui(i1+6,i2+4,i3+2,c3)+c(i,7,4,2)*ui(i1+7,i2+4,i3+2,c3)
     & +c(i,0,5,2)*ui(i1,i2+5,i3+2,c3)+c(i,1,5,2)*ui(i1+1,i2+5,i3+2,
     & c3)+c(i,2,5,2)*ui(i1+2,i2+5,i3+2,c3)+c(i,3,5,2)*ui(i1+3,i2+5,
     & i3+2,c3)+c(i,4,5,2)*ui(i1+4,i2+5,i3+2,c3)+c(i,5,5,2)*ui(i1+5,
     & i2+5,i3+2,c3)+c(i,6,5,2)*ui(i1+6,i2+5,i3+2,c3)+c(i,7,5,2)*ui(
     & i1+7,i2+5,i3+2,c3)+c(i,0,6,2)*ui(i1,i2+6,i3+2,c3)+c(i,1,6,2)*
     & ui(i1+1,i2+6,i3+2,c3)+c(i,2,6,2)*ui(i1+2,i2+6,i3+2,c3)+c(i,3,6,
     & 2)*ui(i1+3,i2+6,i3+2,c3)+c(i,4,6,2)*ui(i1+4,i2+6,i3+2,c3)+c(i,
     & 5,6,2)*ui(i1+5,i2+6,i3+2,c3)+c(i,6,6,2)*ui(i1+6,i2+6,i3+2,c3)+
     & c(i,7,6,2)*ui(i1+7,i2+6,i3+2,c3)
               r(i) = r(i)+c(i,0,7,2)*ui(i1,i2+7,i3+2,c3)+c(i,1,7,2)*
     & ui(i1+1,i2+7,i3+2,c3)+c(i,2,7,2)*ui(i1+2,i2+7,i3+2,c3)+c(i,3,7,
     & 2)*ui(i1+3,i2+7,i3+2,c3)+c(i,4,7,2)*ui(i1+4,i2+7,i3+2,c3)+c(i,
     & 5,7,2)*ui(i1+5,i2+7,i3+2,c3)+c(i,6,7,2)*ui(i1+6,i2+7,i3+2,c3)+
     & c(i,7,7,2)*ui(i1+7,i2+7,i3+2,c3)+c(i,0,0,3)*ui(i1,i2,i3+3,c3)+
     & c(i,1,0,3)*ui(i1+1,i2,i3+3,c3)+c(i,2,0,3)*ui(i1+2,i2,i3+3,c3)+
     & c(i,3,0,3)*ui(i1+3,i2,i3+3,c3)+c(i,4,0,3)*ui(i1+4,i2,i3+3,c3)+
     & c(i,5,0,3)*ui(i1+5,i2,i3+3,c3)+c(i,6,0,3)*ui(i1+6,i2,i3+3,c3)+
     & c(i,7,0,3)*ui(i1+7,i2,i3+3,c3)+c(i,0,1,3)*ui(i1,i2+1,i3+3,c3)+
     & c(i,1,1,3)*ui(i1+1,i2+1,i3+3,c3)+c(i,2,1,3)*ui(i1+2,i2+1,i3+3,
     & c3)+c(i,3,1,3)*ui(i1+3,i2+1,i3+3,c3)+c(i,4,1,3)*ui(i1+4,i2+1,
     & i3+3,c3)+c(i,5,1,3)*ui(i1+5,i2+1,i3+3,c3)+c(i,6,1,3)*ui(i1+6,
     & i2+1,i3+3,c3)+c(i,7,1,3)*ui(i1+7,i2+1,i3+3,c3)+c(i,0,2,3)*ui(
     & i1,i2+2,i3+3,c3)+c(i,1,2,3)*ui(i1+1,i2+2,i3+3,c3)+c(i,2,2,3)*
     & ui(i1+2,i2+2,i3+3,c3)+c(i,3,2,3)*ui(i1+3,i2+2,i3+3,c3)+c(i,4,2,
     & 3)*ui(i1+4,i2+2,i3+3,c3)+c(i,5,2,3)*ui(i1+5,i2+2,i3+3,c3)+c(i,
     & 6,2,3)*ui(i1+6,i2+2,i3+3,c3)+c(i,7,2,3)*ui(i1+7,i2+2,i3+3,c3)+
     & c(i,0,3,3)*ui(i1,i2+3,i3+3,c3)+c(i,1,3,3)*ui(i1+1,i2+3,i3+3,c3)
     & +c(i,2,3,3)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,3,3)*ui(i1+3,i2+3,i3+3,
     & c3)+c(i,4,3,3)*ui(i1+4,i2+3,i3+3,c3)+c(i,5,3,3)*ui(i1+5,i2+3,
     & i3+3,c3)+c(i,6,3,3)*ui(i1+6,i2+3,i3+3,c3)+c(i,7,3,3)*ui(i1+7,
     & i2+3,i3+3,c3)+c(i,0,4,3)*ui(i1,i2+4,i3+3,c3)+c(i,1,4,3)*ui(i1+
     & 1,i2+4,i3+3,c3)+c(i,2,4,3)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,4,3)*ui(
     & i1+3,i2+4,i3+3,c3)+c(i,4,4,3)*ui(i1+4,i2+4,i3+3,c3)+c(i,5,4,3)*
     & ui(i1+5,i2+4,i3+3,c3)+c(i,6,4,3)*ui(i1+6,i2+4,i3+3,c3)+c(i,7,4,
     & 3)*ui(i1+7,i2+4,i3+3,c3)+c(i,0,5,3)*ui(i1,i2+5,i3+3,c3)+c(i,1,
     & 5,3)*ui(i1+1,i2+5,i3+3,c3)+c(i,2,5,3)*ui(i1+2,i2+5,i3+3,c3)+c(
     & i,3,5,3)*ui(i1+3,i2+5,i3+3,c3)+c(i,4,5,3)*ui(i1+4,i2+5,i3+3,c3)
     & +c(i,5,5,3)*ui(i1+5,i2+5,i3+3,c3)+c(i,6,5,3)*ui(i1+6,i2+5,i3+3,
     & c3)+c(i,7,5,3)*ui(i1+7,i2+5,i3+3,c3)+c(i,0,6,3)*ui(i1,i2+6,i3+
     & 3,c3)+c(i,1,6,3)*ui(i1+1,i2+6,i3+3,c3)+c(i,2,6,3)*ui(i1+2,i2+6,
     & i3+3,c3)+c(i,3,6,3)*ui(i1+3,i2+6,i3+3,c3)+c(i,4,6,3)*ui(i1+4,
     & i2+6,i3+3,c3)+c(i,5,6,3)*ui(i1+5,i2+6,i3+3,c3)+c(i,6,6,3)*ui(
     & i1+6,i2+6,i3+3,c3)+c(i,7,6,3)*ui(i1+7,i2+6,i3+3,c3)+c(i,0,7,3)*
     & ui(i1,i2+7,i3+3,c3)+c(i,1,7,3)*ui(i1+1,i2+7,i3+3,c3)+c(i,2,7,3)
     & *ui(i1+2,i2+7,i3+3,c3)+c(i,3,7,3)*ui(i1+3,i2+7,i3+3,c3)+c(i,4,
     & 7,3)*ui(i1+4,i2+7,i3+3,c3)+c(i,5,7,3)*ui(i1+5,i2+7,i3+3,c3)+c(
     & i,6,7,3)*ui(i1+6,i2+7,i3+3,c3)+c(i,7,7,3)*ui(i1+7,i2+7,i3+3,c3)
     & +c(i,0,0,4)*ui(i1,i2,i3+4,c3)+c(i,1,0,4)*ui(i1+1,i2,i3+4,c3)+c(
     & i,2,0,4)*ui(i1+2,i2,i3+4,c3)+c(i,3,0,4)*ui(i1+3,i2,i3+4,c3)+c(
     & i,4,0,4)*ui(i1+4,i2,i3+4,c3)+c(i,5,0,4)*ui(i1+5,i2,i3+4,c3)+c(
     & i,6,0,4)*ui(i1+6,i2,i3+4,c3)+c(i,7,0,4)*ui(i1+7,i2,i3+4,c3)+c(
     & i,0,1,4)*ui(i1,i2+1,i3+4,c3)+c(i,1,1,4)*ui(i1+1,i2+1,i3+4,c3)+
     & c(i,2,1,4)*ui(i1+2,i2+1,i3+4,c3)+c(i,3,1,4)*ui(i1+3,i2+1,i3+4,
     & c3)+c(i,4,1,4)*ui(i1+4,i2+1,i3+4,c3)+c(i,5,1,4)*ui(i1+5,i2+1,
     & i3+4,c3)+c(i,6,1,4)*ui(i1+6,i2+1,i3+4,c3)+c(i,7,1,4)*ui(i1+7,
     & i2+1,i3+4,c3)+c(i,0,2,4)*ui(i1,i2+2,i3+4,c3)+c(i,1,2,4)*ui(i1+
     & 1,i2+2,i3+4,c3)+c(i,2,2,4)*ui(i1+2,i2+2,i3+4,c3)+c(i,3,2,4)*ui(
     & i1+3,i2+2,i3+4,c3)+c(i,4,2,4)*ui(i1+4,i2+2,i3+4,c3)+c(i,5,2,4)*
     & ui(i1+5,i2+2,i3+4,c3)+c(i,6,2,4)*ui(i1+6,i2+2,i3+4,c3)+c(i,7,2,
     & 4)*ui(i1+7,i2+2,i3+4,c3)+c(i,0,3,4)*ui(i1,i2+3,i3+4,c3)+c(i,1,
     & 3,4)*ui(i1+1,i2+3,i3+4,c3)+c(i,2,3,4)*ui(i1+2,i2+3,i3+4,c3)+c(
     & i,3,3,4)*ui(i1+3,i2+3,i3+4,c3)+c(i,4,3,4)*ui(i1+4,i2+3,i3+4,c3)
     & +c(i,5,3,4)*ui(i1+5,i2+3,i3+4,c3)+c(i,6,3,4)*ui(i1+6,i2+3,i3+4,
     & c3)+c(i,7,3,4)*ui(i1+7,i2+3,i3+4,c3)+c(i,0,4,4)*ui(i1,i2+4,i3+
     & 4,c3)+c(i,1,4,4)*ui(i1+1,i2+4,i3+4,c3)+c(i,2,4,4)*ui(i1+2,i2+4,
     & i3+4,c3)+c(i,3,4,4)*ui(i1+3,i2+4,i3+4,c3)+c(i,4,4,4)*ui(i1+4,
     & i2+4,i3+4,c3)+c(i,5,4,4)*ui(i1+5,i2+4,i3+4,c3)+c(i,6,4,4)*ui(
     & i1+6,i2+4,i3+4,c3)+c(i,7,4,4)*ui(i1+7,i2+4,i3+4,c3)+c(i,0,5,4)*
     & ui(i1,i2+5,i3+4,c3)+c(i,1,5,4)*ui(i1+1,i2+5,i3+4,c3)+c(i,2,5,4)
     & *ui(i1+2,i2+5,i3+4,c3)+c(i,3,5,4)*ui(i1+3,i2+5,i3+4,c3)+c(i,4,
     & 5,4)*ui(i1+4,i2+5,i3+4,c3)+c(i,5,5,4)*ui(i1+5,i2+5,i3+4,c3)+c(
     & i,6,5,4)*ui(i1+6,i2+5,i3+4,c3)+c(i,7,5,4)*ui(i1+7,i2+5,i3+4,c3)
     & +c(i,0,6,4)*ui(i1,i2+6,i3+4,c3)+c(i,1,6,4)*ui(i1+1,i2+6,i3+4,
     & c3)+c(i,2,6,4)*ui(i1+2,i2+6,i3+4,c3)+c(i,3,6,4)*ui(i1+3,i2+6,
     & i3+4,c3)+c(i,4,6,4)*ui(i1+4,i2+6,i3+4,c3)+c(i,5,6,4)*ui(i1+5,
     & i2+6,i3+4,c3)+c(i,6,6,4)*ui(i1+6,i2+6,i3+4,c3)+c(i,7,6,4)*ui(
     & i1+7,i2+6,i3+4,c3)+c(i,0,7,4)*ui(i1,i2+7,i3+4,c3)+c(i,1,7,4)*
     & ui(i1+1,i2+7,i3+4,c3)+c(i,2,7,4)*ui(i1+2,i2+7,i3+4,c3)+c(i,3,7,
     & 4)*ui(i1+3,i2+7,i3+4,c3)+c(i,4,7,4)*ui(i1+4,i2+7,i3+4,c3)+c(i,
     & 5,7,4)*ui(i1+5,i2+7,i3+4,c3)+c(i,6,7,4)*ui(i1+6,i2+7,i3+4,c3)+
     & c(i,7,7,4)*ui(i1+7,i2+7,i3+4,c3)+c(i,0,0,5)*ui(i1,i2,i3+5,c3)+
     & c(i,1,0,5)*ui(i1+1,i2,i3+5,c3)+c(i,2,0,5)*ui(i1+2,i2,i3+5,c3)+
     & c(i,3,0,5)*ui(i1+3,i2,i3+5,c3)+c(i,4,0,5)*ui(i1+4,i2,i3+5,c3)+
     & c(i,5,0,5)*ui(i1+5,i2,i3+5,c3)+c(i,6,0,5)*ui(i1+6,i2,i3+5,c3)+
     & c(i,7,0,5)*ui(i1+7,i2,i3+5,c3)+c(i,0,1,5)*ui(i1,i2+1,i3+5,c3)+
     & c(i,1,1,5)*ui(i1+1,i2+1,i3+5,c3)+c(i,2,1,5)*ui(i1+2,i2+1,i3+5,
     & c3)+c(i,3,1,5)*ui(i1+3,i2+1,i3+5,c3)+c(i,4,1,5)*ui(i1+4,i2+1,
     & i3+5,c3)+c(i,5,1,5)*ui(i1+5,i2+1,i3+5,c3)+c(i,6,1,5)*ui(i1+6,
     & i2+1,i3+5,c3)+c(i,7,1,5)*ui(i1+7,i2+1,i3+5,c3)+c(i,0,2,5)*ui(
     & i1,i2+2,i3+5,c3)+c(i,1,2,5)*ui(i1+1,i2+2,i3+5,c3)+c(i,2,2,5)*
     & ui(i1+2,i2+2,i3+5,c3)+c(i,3,2,5)*ui(i1+3,i2+2,i3+5,c3)+c(i,4,2,
     & 5)*ui(i1+4,i2+2,i3+5,c3)+c(i,5,2,5)*ui(i1+5,i2+2,i3+5,c3)+c(i,
     & 6,2,5)*ui(i1+6,i2+2,i3+5,c3)+c(i,7,2,5)*ui(i1+7,i2+2,i3+5,c3)+
     & c(i,0,3,5)*ui(i1,i2+3,i3+5,c3)+c(i,1,3,5)*ui(i1+1,i2+3,i3+5,c3)
     & +c(i,2,3,5)*ui(i1+2,i2+3,i3+5,c3)+c(i,3,3,5)*ui(i1+3,i2+3,i3+5,
     & c3)+c(i,4,3,5)*ui(i1+4,i2+3,i3+5,c3)+c(i,5,3,5)*ui(i1+5,i2+3,
     & i3+5,c3)+c(i,6,3,5)*ui(i1+6,i2+3,i3+5,c3)+c(i,7,3,5)*ui(i1+7,
     & i2+3,i3+5,c3)+c(i,0,4,5)*ui(i1,i2+4,i3+5,c3)+c(i,1,4,5)*ui(i1+
     & 1,i2+4,i3+5,c3)+c(i,2,4,5)*ui(i1+2,i2+4,i3+5,c3)+c(i,3,4,5)*ui(
     & i1+3,i2+4,i3+5,c3)+c(i,4,4,5)*ui(i1+4,i2+4,i3+5,c3)+c(i,5,4,5)*
     & ui(i1+5,i2+4,i3+5,c3)+c(i,6,4,5)*ui(i1+6,i2+4,i3+5,c3)+c(i,7,4,
     & 5)*ui(i1+7,i2+4,i3+5,c3)+c(i,0,5,5)*ui(i1,i2+5,i3+5,c3)+c(i,1,
     & 5,5)*ui(i1+1,i2+5,i3+5,c3)+c(i,2,5,5)*ui(i1+2,i2+5,i3+5,c3)+c(
     & i,3,5,5)*ui(i1+3,i2+5,i3+5,c3)+c(i,4,5,5)*ui(i1+4,i2+5,i3+5,c3)
     & +c(i,5,5,5)*ui(i1+5,i2+5,i3+5,c3)+c(i,6,5,5)*ui(i1+6,i2+5,i3+5,
     & c3)+c(i,7,5,5)*ui(i1+7,i2+5,i3+5,c3)
               r(i) = r(i)+c(i,0,6,5)*ui(i1,i2+6,i3+5,c3)+c(i,1,6,5)*
     & ui(i1+1,i2+6,i3+5,c3)+c(i,2,6,5)*ui(i1+2,i2+6,i3+5,c3)+c(i,3,6,
     & 5)*ui(i1+3,i2+6,i3+5,c3)+c(i,4,6,5)*ui(i1+4,i2+6,i3+5,c3)+c(i,
     & 5,6,5)*ui(i1+5,i2+6,i3+5,c3)+c(i,6,6,5)*ui(i1+6,i2+6,i3+5,c3)+
     & c(i,7,6,5)*ui(i1+7,i2+6,i3+5,c3)+c(i,0,7,5)*ui(i1,i2+7,i3+5,c3)
     & +c(i,1,7,5)*ui(i1+1,i2+7,i3+5,c3)+c(i,2,7,5)*ui(i1+2,i2+7,i3+5,
     & c3)+c(i,3,7,5)*ui(i1+3,i2+7,i3+5,c3)+c(i,4,7,5)*ui(i1+4,i2+7,
     & i3+5,c3)+c(i,5,7,5)*ui(i1+5,i2+7,i3+5,c3)+c(i,6,7,5)*ui(i1+6,
     & i2+7,i3+5,c3)+c(i,7,7,5)*ui(i1+7,i2+7,i3+5,c3)+c(i,0,0,6)*ui(
     & i1,i2,i3+6,c3)+c(i,1,0,6)*ui(i1+1,i2,i3+6,c3)+c(i,2,0,6)*ui(i1+
     & 2,i2,i3+6,c3)+c(i,3,0,6)*ui(i1+3,i2,i3+6,c3)+c(i,4,0,6)*ui(i1+
     & 4,i2,i3+6,c3)+c(i,5,0,6)*ui(i1+5,i2,i3+6,c3)+c(i,6,0,6)*ui(i1+
     & 6,i2,i3+6,c3)+c(i,7,0,6)*ui(i1+7,i2,i3+6,c3)+c(i,0,1,6)*ui(i1,
     & i2+1,i3+6,c3)+c(i,1,1,6)*ui(i1+1,i2+1,i3+6,c3)+c(i,2,1,6)*ui(
     & i1+2,i2+1,i3+6,c3)+c(i,3,1,6)*ui(i1+3,i2+1,i3+6,c3)+c(i,4,1,6)*
     & ui(i1+4,i2+1,i3+6,c3)+c(i,5,1,6)*ui(i1+5,i2+1,i3+6,c3)+c(i,6,1,
     & 6)*ui(i1+6,i2+1,i3+6,c3)+c(i,7,1,6)*ui(i1+7,i2+1,i3+6,c3)+c(i,
     & 0,2,6)*ui(i1,i2+2,i3+6,c3)+c(i,1,2,6)*ui(i1+1,i2+2,i3+6,c3)+c(
     & i,2,2,6)*ui(i1+2,i2+2,i3+6,c3)+c(i,3,2,6)*ui(i1+3,i2+2,i3+6,c3)
     & +c(i,4,2,6)*ui(i1+4,i2+2,i3+6,c3)+c(i,5,2,6)*ui(i1+5,i2+2,i3+6,
     & c3)+c(i,6,2,6)*ui(i1+6,i2+2,i3+6,c3)+c(i,7,2,6)*ui(i1+7,i2+2,
     & i3+6,c3)+c(i,0,3,6)*ui(i1,i2+3,i3+6,c3)+c(i,1,3,6)*ui(i1+1,i2+
     & 3,i3+6,c3)+c(i,2,3,6)*ui(i1+2,i2+3,i3+6,c3)+c(i,3,3,6)*ui(i1+3,
     & i2+3,i3+6,c3)+c(i,4,3,6)*ui(i1+4,i2+3,i3+6,c3)+c(i,5,3,6)*ui(
     & i1+5,i2+3,i3+6,c3)+c(i,6,3,6)*ui(i1+6,i2+3,i3+6,c3)+c(i,7,3,6)*
     & ui(i1+7,i2+3,i3+6,c3)+c(i,0,4,6)*ui(i1,i2+4,i3+6,c3)+c(i,1,4,6)
     & *ui(i1+1,i2+4,i3+6,c3)+c(i,2,4,6)*ui(i1+2,i2+4,i3+6,c3)+c(i,3,
     & 4,6)*ui(i1+3,i2+4,i3+6,c3)+c(i,4,4,6)*ui(i1+4,i2+4,i3+6,c3)+c(
     & i,5,4,6)*ui(i1+5,i2+4,i3+6,c3)+c(i,6,4,6)*ui(i1+6,i2+4,i3+6,c3)
     & +c(i,7,4,6)*ui(i1+7,i2+4,i3+6,c3)+c(i,0,5,6)*ui(i1,i2+5,i3+6,
     & c3)+c(i,1,5,6)*ui(i1+1,i2+5,i3+6,c3)+c(i,2,5,6)*ui(i1+2,i2+5,
     & i3+6,c3)+c(i,3,5,6)*ui(i1+3,i2+5,i3+6,c3)+c(i,4,5,6)*ui(i1+4,
     & i2+5,i3+6,c3)+c(i,5,5,6)*ui(i1+5,i2+5,i3+6,c3)+c(i,6,5,6)*ui(
     & i1+6,i2+5,i3+6,c3)+c(i,7,5,6)*ui(i1+7,i2+5,i3+6,c3)+c(i,0,6,6)*
     & ui(i1,i2+6,i3+6,c3)+c(i,1,6,6)*ui(i1+1,i2+6,i3+6,c3)+c(i,2,6,6)
     & *ui(i1+2,i2+6,i3+6,c3)+c(i,3,6,6)*ui(i1+3,i2+6,i3+6,c3)+c(i,4,
     & 6,6)*ui(i1+4,i2+6,i3+6,c3)+c(i,5,6,6)*ui(i1+5,i2+6,i3+6,c3)+c(
     & i,6,6,6)*ui(i1+6,i2+6,i3+6,c3)+c(i,7,6,6)*ui(i1+7,i2+6,i3+6,c3)
     & +c(i,0,7,6)*ui(i1,i2+7,i3+6,c3)+c(i,1,7,6)*ui(i1+1,i2+7,i3+6,
     & c3)+c(i,2,7,6)*ui(i1+2,i2+7,i3+6,c3)+c(i,3,7,6)*ui(i1+3,i2+7,
     & i3+6,c3)+c(i,4,7,6)*ui(i1+4,i2+7,i3+6,c3)+c(i,5,7,6)*ui(i1+5,
     & i2+7,i3+6,c3)+c(i,6,7,6)*ui(i1+6,i2+7,i3+6,c3)+c(i,7,7,6)*ui(
     & i1+7,i2+7,i3+6,c3)+c(i,0,0,7)*ui(i1,i2,i3+7,c3)+c(i,1,0,7)*ui(
     & i1+1,i2,i3+7,c3)+c(i,2,0,7)*ui(i1+2,i2,i3+7,c3)+c(i,3,0,7)*ui(
     & i1+3,i2,i3+7,c3)+c(i,4,0,7)*ui(i1+4,i2,i3+7,c3)+c(i,5,0,7)*ui(
     & i1+5,i2,i3+7,c3)+c(i,6,0,7)*ui(i1+6,i2,i3+7,c3)+c(i,7,0,7)*ui(
     & i1+7,i2,i3+7,c3)+c(i,0,1,7)*ui(i1,i2+1,i3+7,c3)+c(i,1,1,7)*ui(
     & i1+1,i2+1,i3+7,c3)+c(i,2,1,7)*ui(i1+2,i2+1,i3+7,c3)+c(i,3,1,7)*
     & ui(i1+3,i2+1,i3+7,c3)+c(i,4,1,7)*ui(i1+4,i2+1,i3+7,c3)+c(i,5,1,
     & 7)*ui(i1+5,i2+1,i3+7,c3)+c(i,6,1,7)*ui(i1+6,i2+1,i3+7,c3)+c(i,
     & 7,1,7)*ui(i1+7,i2+1,i3+7,c3)+c(i,0,2,7)*ui(i1,i2+2,i3+7,c3)+c(
     & i,1,2,7)*ui(i1+1,i2+2,i3+7,c3)+c(i,2,2,7)*ui(i1+2,i2+2,i3+7,c3)
     & +c(i,3,2,7)*ui(i1+3,i2+2,i3+7,c3)+c(i,4,2,7)*ui(i1+4,i2+2,i3+7,
     & c3)+c(i,5,2,7)*ui(i1+5,i2+2,i3+7,c3)+c(i,6,2,7)*ui(i1+6,i2+2,
     & i3+7,c3)+c(i,7,2,7)*ui(i1+7,i2+2,i3+7,c3)+c(i,0,3,7)*ui(i1,i2+
     & 3,i3+7,c3)+c(i,1,3,7)*ui(i1+1,i2+3,i3+7,c3)+c(i,2,3,7)*ui(i1+2,
     & i2+3,i3+7,c3)+c(i,3,3,7)*ui(i1+3,i2+3,i3+7,c3)+c(i,4,3,7)*ui(
     & i1+4,i2+3,i3+7,c3)+c(i,5,3,7)*ui(i1+5,i2+3,i3+7,c3)+c(i,6,3,7)*
     & ui(i1+6,i2+3,i3+7,c3)+c(i,7,3,7)*ui(i1+7,i2+3,i3+7,c3)+c(i,0,4,
     & 7)*ui(i1,i2+4,i3+7,c3)+c(i,1,4,7)*ui(i1+1,i2+4,i3+7,c3)+c(i,2,
     & 4,7)*ui(i1+2,i2+4,i3+7,c3)+c(i,3,4,7)*ui(i1+3,i2+4,i3+7,c3)+c(
     & i,4,4,7)*ui(i1+4,i2+4,i3+7,c3)+c(i,5,4,7)*ui(i1+5,i2+4,i3+7,c3)
     & +c(i,6,4,7)*ui(i1+6,i2+4,i3+7,c3)+c(i,7,4,7)*ui(i1+7,i2+4,i3+7,
     & c3)+c(i,0,5,7)*ui(i1,i2+5,i3+7,c3)+c(i,1,5,7)*ui(i1+1,i2+5,i3+
     & 7,c3)+c(i,2,5,7)*ui(i1+2,i2+5,i3+7,c3)+c(i,3,5,7)*ui(i1+3,i2+5,
     & i3+7,c3)+c(i,4,5,7)*ui(i1+4,i2+5,i3+7,c3)+c(i,5,5,7)*ui(i1+5,
     & i2+5,i3+7,c3)+c(i,6,5,7)*ui(i1+6,i2+5,i3+7,c3)+c(i,7,5,7)*ui(
     & i1+7,i2+5,i3+7,c3)+c(i,0,6,7)*ui(i1,i2+6,i3+7,c3)+c(i,1,6,7)*
     & ui(i1+1,i2+6,i3+7,c3)+c(i,2,6,7)*ui(i1+2,i2+6,i3+7,c3)+c(i,3,6,
     & 7)*ui(i1+3,i2+6,i3+7,c3)+c(i,4,6,7)*ui(i1+4,i2+6,i3+7,c3)+c(i,
     & 5,6,7)*ui(i1+5,i2+6,i3+7,c3)+c(i,6,6,7)*ui(i1+6,i2+6,i3+7,c3)+
     & c(i,7,6,7)*ui(i1+7,i2+6,i3+7,c3)+c(i,0,7,7)*ui(i1,i2+7,i3+7,c3)
     & +c(i,1,7,7)*ui(i1+1,i2+7,i3+7,c3)+c(i,2,7,7)*ui(i1+2,i2+7,i3+7,
     & c3)+c(i,3,7,7)*ui(i1+3,i2+7,i3+7,c3)+c(i,4,7,7)*ui(i1+4,i2+7,
     & i3+7,c3)+c(i,5,7,7)*ui(i1+5,i2+7,i3+7,c3)+c(i,6,7,7)*ui(i1+6,
     & i2+7,i3+7,c3)+c(i,7,7,7)*ui(i1+7,i2+7,i3+7,c3)
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
! interp888(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,0,0)*ui(i1,i2,i3,c3)+c(i,1,0,0)*ui(i1+1,i2,
     & i3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3,c3)+c(i,3,0,0)*ui(i1+3,i2,i3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2,i3,c3)+c(i,5,0,0)*ui(i1+5,i2,i3,c3)+
     & c(i,6,0,0)*ui(i1+6,i2,i3,c3)+c(i,7,0,0)*ui(i1+7,i2,i3,c3)+c(i,
     & 0,1,0)*ui(i1,i2+1,i3,c3)+c(i,1,1,0)*ui(i1+1,i2+1,i3,c3)+c(i,2,
     & 1,0)*ui(i1+2,i2+1,i3,c3)+c(i,3,1,0)*ui(i1+3,i2+1,i3,c3)+c(i,4,
     & 1,0)*ui(i1+4,i2+1,i3,c3)+c(i,5,1,0)*ui(i1+5,i2+1,i3,c3)+c(i,6,
     & 1,0)*ui(i1+6,i2+1,i3,c3)+c(i,7,1,0)*ui(i1+7,i2+1,i3,c3)+c(i,0,
     & 2,0)*ui(i1,i2+2,i3,c3)+c(i,1,2,0)*ui(i1+1,i2+2,i3,c3)+c(i,2,2,
     & 0)*ui(i1+2,i2+2,i3,c3)+c(i,3,2,0)*ui(i1+3,i2+2,i3,c3)+c(i,4,2,
     & 0)*ui(i1+4,i2+2,i3,c3)+c(i,5,2,0)*ui(i1+5,i2+2,i3,c3)+c(i,6,2,
     & 0)*ui(i1+6,i2+2,i3,c3)+c(i,7,2,0)*ui(i1+7,i2+2,i3,c3)+c(i,0,3,
     & 0)*ui(i1,i2+3,i3,c3)+c(i,1,3,0)*ui(i1+1,i2+3,i3,c3)+c(i,2,3,0)*
     & ui(i1+2,i2+3,i3,c3)+c(i,3,3,0)*ui(i1+3,i2+3,i3,c3)+c(i,4,3,0)*
     & ui(i1+4,i2+3,i3,c3)+c(i,5,3,0)*ui(i1+5,i2+3,i3,c3)+c(i,6,3,0)*
     & ui(i1+6,i2+3,i3,c3)+c(i,7,3,0)*ui(i1+7,i2+3,i3,c3)+c(i,0,4,0)*
     & ui(i1,i2+4,i3,c3)+c(i,1,4,0)*ui(i1+1,i2+4,i3,c3)+c(i,2,4,0)*ui(
     & i1+2,i2+4,i3,c3)+c(i,3,4,0)*ui(i1+3,i2+4,i3,c3)+c(i,4,4,0)*ui(
     & i1+4,i2+4,i3,c3)+c(i,5,4,0)*ui(i1+5,i2+4,i3,c3)+c(i,6,4,0)*ui(
     & i1+6,i2+4,i3,c3)+c(i,7,4,0)*ui(i1+7,i2+4,i3,c3)+c(i,0,5,0)*ui(
     & i1,i2+5,i3,c3)+c(i,1,5,0)*ui(i1+1,i2+5,i3,c3)+c(i,2,5,0)*ui(i1+
     & 2,i2+5,i3,c3)+c(i,3,5,0)*ui(i1+3,i2+5,i3,c3)+c(i,4,5,0)*ui(i1+
     & 4,i2+5,i3,c3)+c(i,5,5,0)*ui(i1+5,i2+5,i3,c3)+c(i,6,5,0)*ui(i1+
     & 6,i2+5,i3,c3)+c(i,7,5,0)*ui(i1+7,i2+5,i3,c3)+c(i,0,6,0)*ui(i1,
     & i2+6,i3,c3)+c(i,1,6,0)*ui(i1+1,i2+6,i3,c3)+c(i,2,6,0)*ui(i1+2,
     & i2+6,i3,c3)+c(i,3,6,0)*ui(i1+3,i2+6,i3,c3)+c(i,4,6,0)*ui(i1+4,
     & i2+6,i3,c3)+c(i,5,6,0)*ui(i1+5,i2+6,i3,c3)+c(i,6,6,0)*ui(i1+6,
     & i2+6,i3,c3)+c(i,7,6,0)*ui(i1+7,i2+6,i3,c3)+c(i,0,7,0)*ui(i1,i2+
     & 7,i3,c3)+c(i,1,7,0)*ui(i1+1,i2+7,i3,c3)+c(i,2,7,0)*ui(i1+2,i2+
     & 7,i3,c3)+c(i,3,7,0)*ui(i1+3,i2+7,i3,c3)+c(i,4,7,0)*ui(i1+4,i2+
     & 7,i3,c3)+c(i,5,7,0)*ui(i1+5,i2+7,i3,c3)+c(i,6,7,0)*ui(i1+6,i2+
     & 7,i3,c3)+c(i,7,7,0)*ui(i1+7,i2+7,i3,c3)+c(i,0,0,1)*ui(i1,i2,i3+
     & 1,c3)+c(i,1,0,1)*ui(i1+1,i2,i3+1,c3)+c(i,2,0,1)*ui(i1+2,i2,i3+
     & 1,c3)+c(i,3,0,1)*ui(i1+3,i2,i3+1,c3)+c(i,4,0,1)*ui(i1+4,i2,i3+
     & 1,c3)+c(i,5,0,1)*ui(i1+5,i2,i3+1,c3)+c(i,6,0,1)*ui(i1+6,i2,i3+
     & 1,c3)+c(i,7,0,1)*ui(i1+7,i2,i3+1,c3)+c(i,0,1,1)*ui(i1,i2+1,i3+
     & 1,c3)+c(i,1,1,1)*ui(i1+1,i2+1,i3+1,c3)+c(i,2,1,1)*ui(i1+2,i2+1,
     & i3+1,c3)+c(i,3,1,1)*ui(i1+3,i2+1,i3+1,c3)+c(i,4,1,1)*ui(i1+4,
     & i2+1,i3+1,c3)+c(i,5,1,1)*ui(i1+5,i2+1,i3+1,c3)+c(i,6,1,1)*ui(
     & i1+6,i2+1,i3+1,c3)+c(i,7,1,1)*ui(i1+7,i2+1,i3+1,c3)+c(i,0,2,1)*
     & ui(i1,i2+2,i3+1,c3)+c(i,1,2,1)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,2,1)
     & *ui(i1+2,i2+2,i3+1,c3)+c(i,3,2,1)*ui(i1+3,i2+2,i3+1,c3)+c(i,4,
     & 2,1)*ui(i1+4,i2+2,i3+1,c3)+c(i,5,2,1)*ui(i1+5,i2+2,i3+1,c3)+c(
     & i,6,2,1)*ui(i1+6,i2+2,i3+1,c3)+c(i,7,2,1)*ui(i1+7,i2+2,i3+1,c3)
     & +c(i,0,3,1)*ui(i1,i2+3,i3+1,c3)+c(i,1,3,1)*ui(i1+1,i2+3,i3+1,
     & c3)+c(i,2,3,1)*ui(i1+2,i2+3,i3+1,c3)+c(i,3,3,1)*ui(i1+3,i2+3,
     & i3+1,c3)+c(i,4,3,1)*ui(i1+4,i2+3,i3+1,c3)+c(i,5,3,1)*ui(i1+5,
     & i2+3,i3+1,c3)+c(i,6,3,1)*ui(i1+6,i2+3,i3+1,c3)+c(i,7,3,1)*ui(
     & i1+7,i2+3,i3+1,c3)+c(i,0,4,1)*ui(i1,i2+4,i3+1,c3)+c(i,1,4,1)*
     & ui(i1+1,i2+4,i3+1,c3)+c(i,2,4,1)*ui(i1+2,i2+4,i3+1,c3)+c(i,3,4,
     & 1)*ui(i1+3,i2+4,i3+1,c3)+c(i,4,4,1)*ui(i1+4,i2+4,i3+1,c3)+c(i,
     & 5,4,1)*ui(i1+5,i2+4,i3+1,c3)+c(i,6,4,1)*ui(i1+6,i2+4,i3+1,c3)+
     & c(i,7,4,1)*ui(i1+7,i2+4,i3+1,c3)+c(i,0,5,1)*ui(i1,i2+5,i3+1,c3)
     & +c(i,1,5,1)*ui(i1+1,i2+5,i3+1,c3)+c(i,2,5,1)*ui(i1+2,i2+5,i3+1,
     & c3)+c(i,3,5,1)*ui(i1+3,i2+5,i3+1,c3)+c(i,4,5,1)*ui(i1+4,i2+5,
     & i3+1,c3)+c(i,5,5,1)*ui(i1+5,i2+5,i3+1,c3)+c(i,6,5,1)*ui(i1+6,
     & i2+5,i3+1,c3)+c(i,7,5,1)*ui(i1+7,i2+5,i3+1,c3)+c(i,0,6,1)*ui(
     & i1,i2+6,i3+1,c3)+c(i,1,6,1)*ui(i1+1,i2+6,i3+1,c3)+c(i,2,6,1)*
     & ui(i1+2,i2+6,i3+1,c3)+c(i,3,6,1)*ui(i1+3,i2+6,i3+1,c3)+c(i,4,6,
     & 1)*ui(i1+4,i2+6,i3+1,c3)+c(i,5,6,1)*ui(i1+5,i2+6,i3+1,c3)+c(i,
     & 6,6,1)*ui(i1+6,i2+6,i3+1,c3)+c(i,7,6,1)*ui(i1+7,i2+6,i3+1,c3)+
     & c(i,0,7,1)*ui(i1,i2+7,i3+1,c3)+c(i,1,7,1)*ui(i1+1,i2+7,i3+1,c3)
     & +c(i,2,7,1)*ui(i1+2,i2+7,i3+1,c3)+c(i,3,7,1)*ui(i1+3,i2+7,i3+1,
     & c3)+c(i,4,7,1)*ui(i1+4,i2+7,i3+1,c3)+c(i,5,7,1)*ui(i1+5,i2+7,
     & i3+1,c3)+c(i,6,7,1)*ui(i1+6,i2+7,i3+1,c3)+c(i,7,7,1)*ui(i1+7,
     & i2+7,i3+1,c3)+c(i,0,0,2)*ui(i1,i2,i3+2,c3)+c(i,1,0,2)*ui(i1+1,
     & i2,i3+2,c3)+c(i,2,0,2)*ui(i1+2,i2,i3+2,c3)+c(i,3,0,2)*ui(i1+3,
     & i2,i3+2,c3)+c(i,4,0,2)*ui(i1+4,i2,i3+2,c3)+c(i,5,0,2)*ui(i1+5,
     & i2,i3+2,c3)+c(i,6,0,2)*ui(i1+6,i2,i3+2,c3)+c(i,7,0,2)*ui(i1+7,
     & i2,i3+2,c3)+c(i,0,1,2)*ui(i1,i2+1,i3+2,c3)+c(i,1,1,2)*ui(i1+1,
     & i2+1,i3+2,c3)+c(i,2,1,2)*ui(i1+2,i2+1,i3+2,c3)+c(i,3,1,2)*ui(
     & i1+3,i2+1,i3+2,c3)+c(i,4,1,2)*ui(i1+4,i2+1,i3+2,c3)+c(i,5,1,2)*
     & ui(i1+5,i2+1,i3+2,c3)+c(i,6,1,2)*ui(i1+6,i2+1,i3+2,c3)+c(i,7,1,
     & 2)*ui(i1+7,i2+1,i3+2,c3)+c(i,0,2,2)*ui(i1,i2+2,i3+2,c3)+c(i,1,
     & 2,2)*ui(i1+1,i2+2,i3+2,c3)+c(i,2,2,2)*ui(i1+2,i2+2,i3+2,c3)+c(
     & i,3,2,2)*ui(i1+3,i2+2,i3+2,c3)+c(i,4,2,2)*ui(i1+4,i2+2,i3+2,c3)
     & +c(i,5,2,2)*ui(i1+5,i2+2,i3+2,c3)+c(i,6,2,2)*ui(i1+6,i2+2,i3+2,
     & c3)+c(i,7,2,2)*ui(i1+7,i2+2,i3+2,c3)+c(i,0,3,2)*ui(i1,i2+3,i3+
     & 2,c3)+c(i,1,3,2)*ui(i1+1,i2+3,i3+2,c3)+c(i,2,3,2)*ui(i1+2,i2+3,
     & i3+2,c3)+c(i,3,3,2)*ui(i1+3,i2+3,i3+2,c3)+c(i,4,3,2)*ui(i1+4,
     & i2+3,i3+2,c3)+c(i,5,3,2)*ui(i1+5,i2+3,i3+2,c3)+c(i,6,3,2)*ui(
     & i1+6,i2+3,i3+2,c3)+c(i,7,3,2)*ui(i1+7,i2+3,i3+2,c3)+c(i,0,4,2)*
     & ui(i1,i2+4,i3+2,c3)+c(i,1,4,2)*ui(i1+1,i2+4,i3+2,c3)+c(i,2,4,2)
     & *ui(i1+2,i2+4,i3+2,c3)+c(i,3,4,2)*ui(i1+3,i2+4,i3+2,c3)+c(i,4,
     & 4,2)*ui(i1+4,i2+4,i3+2,c3)+c(i,5,4,2)*ui(i1+5,i2+4,i3+2,c3)+c(
     & i,6,4,2)*ui(i1+6,i2+4,i3+2,c3)+c(i,7,4,2)*ui(i1+7,i2+4,i3+2,c3)
     & +c(i,0,5,2)*ui(i1,i2+5,i3+2,c3)+c(i,1,5,2)*ui(i1+1,i2+5,i3+2,
     & c3)+c(i,2,5,2)*ui(i1+2,i2+5,i3+2,c3)+c(i,3,5,2)*ui(i1+3,i2+5,
     & i3+2,c3)+c(i,4,5,2)*ui(i1+4,i2+5,i3+2,c3)+c(i,5,5,2)*ui(i1+5,
     & i2+5,i3+2,c3)+c(i,6,5,2)*ui(i1+6,i2+5,i3+2,c3)+c(i,7,5,2)*ui(
     & i1+7,i2+5,i3+2,c3)+c(i,0,6,2)*ui(i1,i2+6,i3+2,c3)+c(i,1,6,2)*
     & ui(i1+1,i2+6,i3+2,c3)+c(i,2,6,2)*ui(i1+2,i2+6,i3+2,c3)+c(i,3,6,
     & 2)*ui(i1+3,i2+6,i3+2,c3)+c(i,4,6,2)*ui(i1+4,i2+6,i3+2,c3)+c(i,
     & 5,6,2)*ui(i1+5,i2+6,i3+2,c3)+c(i,6,6,2)*ui(i1+6,i2+6,i3+2,c3)+
     & c(i,7,6,2)*ui(i1+7,i2+6,i3+2,c3)
               r(i) = r(i)+c(i,0,7,2)*ui(i1,i2+7,i3+2,c3)+c(i,1,7,2)*
     & ui(i1+1,i2+7,i3+2,c3)+c(i,2,7,2)*ui(i1+2,i2+7,i3+2,c3)+c(i,3,7,
     & 2)*ui(i1+3,i2+7,i3+2,c3)+c(i,4,7,2)*ui(i1+4,i2+7,i3+2,c3)+c(i,
     & 5,7,2)*ui(i1+5,i2+7,i3+2,c3)+c(i,6,7,2)*ui(i1+6,i2+7,i3+2,c3)+
     & c(i,7,7,2)*ui(i1+7,i2+7,i3+2,c3)+c(i,0,0,3)*ui(i1,i2,i3+3,c3)+
     & c(i,1,0,3)*ui(i1+1,i2,i3+3,c3)+c(i,2,0,3)*ui(i1+2,i2,i3+3,c3)+
     & c(i,3,0,3)*ui(i1+3,i2,i3+3,c3)+c(i,4,0,3)*ui(i1+4,i2,i3+3,c3)+
     & c(i,5,0,3)*ui(i1+5,i2,i3+3,c3)+c(i,6,0,3)*ui(i1+6,i2,i3+3,c3)+
     & c(i,7,0,3)*ui(i1+7,i2,i3+3,c3)+c(i,0,1,3)*ui(i1,i2+1,i3+3,c3)+
     & c(i,1,1,3)*ui(i1+1,i2+1,i3+3,c3)+c(i,2,1,3)*ui(i1+2,i2+1,i3+3,
     & c3)+c(i,3,1,3)*ui(i1+3,i2+1,i3+3,c3)+c(i,4,1,3)*ui(i1+4,i2+1,
     & i3+3,c3)+c(i,5,1,3)*ui(i1+5,i2+1,i3+3,c3)+c(i,6,1,3)*ui(i1+6,
     & i2+1,i3+3,c3)+c(i,7,1,3)*ui(i1+7,i2+1,i3+3,c3)+c(i,0,2,3)*ui(
     & i1,i2+2,i3+3,c3)+c(i,1,2,3)*ui(i1+1,i2+2,i3+3,c3)+c(i,2,2,3)*
     & ui(i1+2,i2+2,i3+3,c3)+c(i,3,2,3)*ui(i1+3,i2+2,i3+3,c3)+c(i,4,2,
     & 3)*ui(i1+4,i2+2,i3+3,c3)+c(i,5,2,3)*ui(i1+5,i2+2,i3+3,c3)+c(i,
     & 6,2,3)*ui(i1+6,i2+2,i3+3,c3)+c(i,7,2,3)*ui(i1+7,i2+2,i3+3,c3)+
     & c(i,0,3,3)*ui(i1,i2+3,i3+3,c3)+c(i,1,3,3)*ui(i1+1,i2+3,i3+3,c3)
     & +c(i,2,3,3)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,3,3)*ui(i1+3,i2+3,i3+3,
     & c3)+c(i,4,3,3)*ui(i1+4,i2+3,i3+3,c3)+c(i,5,3,3)*ui(i1+5,i2+3,
     & i3+3,c3)+c(i,6,3,3)*ui(i1+6,i2+3,i3+3,c3)+c(i,7,3,3)*ui(i1+7,
     & i2+3,i3+3,c3)+c(i,0,4,3)*ui(i1,i2+4,i3+3,c3)+c(i,1,4,3)*ui(i1+
     & 1,i2+4,i3+3,c3)+c(i,2,4,3)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,4,3)*ui(
     & i1+3,i2+4,i3+3,c3)+c(i,4,4,3)*ui(i1+4,i2+4,i3+3,c3)+c(i,5,4,3)*
     & ui(i1+5,i2+4,i3+3,c3)+c(i,6,4,3)*ui(i1+6,i2+4,i3+3,c3)+c(i,7,4,
     & 3)*ui(i1+7,i2+4,i3+3,c3)+c(i,0,5,3)*ui(i1,i2+5,i3+3,c3)+c(i,1,
     & 5,3)*ui(i1+1,i2+5,i3+3,c3)+c(i,2,5,3)*ui(i1+2,i2+5,i3+3,c3)+c(
     & i,3,5,3)*ui(i1+3,i2+5,i3+3,c3)+c(i,4,5,3)*ui(i1+4,i2+5,i3+3,c3)
     & +c(i,5,5,3)*ui(i1+5,i2+5,i3+3,c3)+c(i,6,5,3)*ui(i1+6,i2+5,i3+3,
     & c3)+c(i,7,5,3)*ui(i1+7,i2+5,i3+3,c3)+c(i,0,6,3)*ui(i1,i2+6,i3+
     & 3,c3)+c(i,1,6,3)*ui(i1+1,i2+6,i3+3,c3)+c(i,2,6,3)*ui(i1+2,i2+6,
     & i3+3,c3)+c(i,3,6,3)*ui(i1+3,i2+6,i3+3,c3)+c(i,4,6,3)*ui(i1+4,
     & i2+6,i3+3,c3)+c(i,5,6,3)*ui(i1+5,i2+6,i3+3,c3)+c(i,6,6,3)*ui(
     & i1+6,i2+6,i3+3,c3)+c(i,7,6,3)*ui(i1+7,i2+6,i3+3,c3)+c(i,0,7,3)*
     & ui(i1,i2+7,i3+3,c3)+c(i,1,7,3)*ui(i1+1,i2+7,i3+3,c3)+c(i,2,7,3)
     & *ui(i1+2,i2+7,i3+3,c3)+c(i,3,7,3)*ui(i1+3,i2+7,i3+3,c3)+c(i,4,
     & 7,3)*ui(i1+4,i2+7,i3+3,c3)+c(i,5,7,3)*ui(i1+5,i2+7,i3+3,c3)+c(
     & i,6,7,3)*ui(i1+6,i2+7,i3+3,c3)+c(i,7,7,3)*ui(i1+7,i2+7,i3+3,c3)
     & +c(i,0,0,4)*ui(i1,i2,i3+4,c3)+c(i,1,0,4)*ui(i1+1,i2,i3+4,c3)+c(
     & i,2,0,4)*ui(i1+2,i2,i3+4,c3)+c(i,3,0,4)*ui(i1+3,i2,i3+4,c3)+c(
     & i,4,0,4)*ui(i1+4,i2,i3+4,c3)+c(i,5,0,4)*ui(i1+5,i2,i3+4,c3)+c(
     & i,6,0,4)*ui(i1+6,i2,i3+4,c3)+c(i,7,0,4)*ui(i1+7,i2,i3+4,c3)+c(
     & i,0,1,4)*ui(i1,i2+1,i3+4,c3)+c(i,1,1,4)*ui(i1+1,i2+1,i3+4,c3)+
     & c(i,2,1,4)*ui(i1+2,i2+1,i3+4,c3)+c(i,3,1,4)*ui(i1+3,i2+1,i3+4,
     & c3)+c(i,4,1,4)*ui(i1+4,i2+1,i3+4,c3)+c(i,5,1,4)*ui(i1+5,i2+1,
     & i3+4,c3)+c(i,6,1,4)*ui(i1+6,i2+1,i3+4,c3)+c(i,7,1,4)*ui(i1+7,
     & i2+1,i3+4,c3)+c(i,0,2,4)*ui(i1,i2+2,i3+4,c3)+c(i,1,2,4)*ui(i1+
     & 1,i2+2,i3+4,c3)+c(i,2,2,4)*ui(i1+2,i2+2,i3+4,c3)+c(i,3,2,4)*ui(
     & i1+3,i2+2,i3+4,c3)+c(i,4,2,4)*ui(i1+4,i2+2,i3+4,c3)+c(i,5,2,4)*
     & ui(i1+5,i2+2,i3+4,c3)+c(i,6,2,4)*ui(i1+6,i2+2,i3+4,c3)+c(i,7,2,
     & 4)*ui(i1+7,i2+2,i3+4,c3)+c(i,0,3,4)*ui(i1,i2+3,i3+4,c3)+c(i,1,
     & 3,4)*ui(i1+1,i2+3,i3+4,c3)+c(i,2,3,4)*ui(i1+2,i2+3,i3+4,c3)+c(
     & i,3,3,4)*ui(i1+3,i2+3,i3+4,c3)+c(i,4,3,4)*ui(i1+4,i2+3,i3+4,c3)
     & +c(i,5,3,4)*ui(i1+5,i2+3,i3+4,c3)+c(i,6,3,4)*ui(i1+6,i2+3,i3+4,
     & c3)+c(i,7,3,4)*ui(i1+7,i2+3,i3+4,c3)+c(i,0,4,4)*ui(i1,i2+4,i3+
     & 4,c3)+c(i,1,4,4)*ui(i1+1,i2+4,i3+4,c3)+c(i,2,4,4)*ui(i1+2,i2+4,
     & i3+4,c3)+c(i,3,4,4)*ui(i1+3,i2+4,i3+4,c3)+c(i,4,4,4)*ui(i1+4,
     & i2+4,i3+4,c3)+c(i,5,4,4)*ui(i1+5,i2+4,i3+4,c3)+c(i,6,4,4)*ui(
     & i1+6,i2+4,i3+4,c3)+c(i,7,4,4)*ui(i1+7,i2+4,i3+4,c3)+c(i,0,5,4)*
     & ui(i1,i2+5,i3+4,c3)+c(i,1,5,4)*ui(i1+1,i2+5,i3+4,c3)+c(i,2,5,4)
     & *ui(i1+2,i2+5,i3+4,c3)+c(i,3,5,4)*ui(i1+3,i2+5,i3+4,c3)+c(i,4,
     & 5,4)*ui(i1+4,i2+5,i3+4,c3)+c(i,5,5,4)*ui(i1+5,i2+5,i3+4,c3)+c(
     & i,6,5,4)*ui(i1+6,i2+5,i3+4,c3)+c(i,7,5,4)*ui(i1+7,i2+5,i3+4,c3)
     & +c(i,0,6,4)*ui(i1,i2+6,i3+4,c3)+c(i,1,6,4)*ui(i1+1,i2+6,i3+4,
     & c3)+c(i,2,6,4)*ui(i1+2,i2+6,i3+4,c3)+c(i,3,6,4)*ui(i1+3,i2+6,
     & i3+4,c3)+c(i,4,6,4)*ui(i1+4,i2+6,i3+4,c3)+c(i,5,6,4)*ui(i1+5,
     & i2+6,i3+4,c3)+c(i,6,6,4)*ui(i1+6,i2+6,i3+4,c3)+c(i,7,6,4)*ui(
     & i1+7,i2+6,i3+4,c3)+c(i,0,7,4)*ui(i1,i2+7,i3+4,c3)+c(i,1,7,4)*
     & ui(i1+1,i2+7,i3+4,c3)+c(i,2,7,4)*ui(i1+2,i2+7,i3+4,c3)+c(i,3,7,
     & 4)*ui(i1+3,i2+7,i3+4,c3)+c(i,4,7,4)*ui(i1+4,i2+7,i3+4,c3)+c(i,
     & 5,7,4)*ui(i1+5,i2+7,i3+4,c3)+c(i,6,7,4)*ui(i1+6,i2+7,i3+4,c3)+
     & c(i,7,7,4)*ui(i1+7,i2+7,i3+4,c3)+c(i,0,0,5)*ui(i1,i2,i3+5,c3)+
     & c(i,1,0,5)*ui(i1+1,i2,i3+5,c3)+c(i,2,0,5)*ui(i1+2,i2,i3+5,c3)+
     & c(i,3,0,5)*ui(i1+3,i2,i3+5,c3)+c(i,4,0,5)*ui(i1+4,i2,i3+5,c3)+
     & c(i,5,0,5)*ui(i1+5,i2,i3+5,c3)+c(i,6,0,5)*ui(i1+6,i2,i3+5,c3)+
     & c(i,7,0,5)*ui(i1+7,i2,i3+5,c3)+c(i,0,1,5)*ui(i1,i2+1,i3+5,c3)+
     & c(i,1,1,5)*ui(i1+1,i2+1,i3+5,c3)+c(i,2,1,5)*ui(i1+2,i2+1,i3+5,
     & c3)+c(i,3,1,5)*ui(i1+3,i2+1,i3+5,c3)+c(i,4,1,5)*ui(i1+4,i2+1,
     & i3+5,c3)+c(i,5,1,5)*ui(i1+5,i2+1,i3+5,c3)+c(i,6,1,5)*ui(i1+6,
     & i2+1,i3+5,c3)+c(i,7,1,5)*ui(i1+7,i2+1,i3+5,c3)+c(i,0,2,5)*ui(
     & i1,i2+2,i3+5,c3)+c(i,1,2,5)*ui(i1+1,i2+2,i3+5,c3)+c(i,2,2,5)*
     & ui(i1+2,i2+2,i3+5,c3)+c(i,3,2,5)*ui(i1+3,i2+2,i3+5,c3)+c(i,4,2,
     & 5)*ui(i1+4,i2+2,i3+5,c3)+c(i,5,2,5)*ui(i1+5,i2+2,i3+5,c3)+c(i,
     & 6,2,5)*ui(i1+6,i2+2,i3+5,c3)+c(i,7,2,5)*ui(i1+7,i2+2,i3+5,c3)+
     & c(i,0,3,5)*ui(i1,i2+3,i3+5,c3)+c(i,1,3,5)*ui(i1+1,i2+3,i3+5,c3)
     & +c(i,2,3,5)*ui(i1+2,i2+3,i3+5,c3)+c(i,3,3,5)*ui(i1+3,i2+3,i3+5,
     & c3)+c(i,4,3,5)*ui(i1+4,i2+3,i3+5,c3)+c(i,5,3,5)*ui(i1+5,i2+3,
     & i3+5,c3)+c(i,6,3,5)*ui(i1+6,i2+3,i3+5,c3)+c(i,7,3,5)*ui(i1+7,
     & i2+3,i3+5,c3)+c(i,0,4,5)*ui(i1,i2+4,i3+5,c3)+c(i,1,4,5)*ui(i1+
     & 1,i2+4,i3+5,c3)+c(i,2,4,5)*ui(i1+2,i2+4,i3+5,c3)+c(i,3,4,5)*ui(
     & i1+3,i2+4,i3+5,c3)+c(i,4,4,5)*ui(i1+4,i2+4,i3+5,c3)+c(i,5,4,5)*
     & ui(i1+5,i2+4,i3+5,c3)+c(i,6,4,5)*ui(i1+6,i2+4,i3+5,c3)+c(i,7,4,
     & 5)*ui(i1+7,i2+4,i3+5,c3)+c(i,0,5,5)*ui(i1,i2+5,i3+5,c3)+c(i,1,
     & 5,5)*ui(i1+1,i2+5,i3+5,c3)+c(i,2,5,5)*ui(i1+2,i2+5,i3+5,c3)+c(
     & i,3,5,5)*ui(i1+3,i2+5,i3+5,c3)+c(i,4,5,5)*ui(i1+4,i2+5,i3+5,c3)
     & +c(i,5,5,5)*ui(i1+5,i2+5,i3+5,c3)+c(i,6,5,5)*ui(i1+6,i2+5,i3+5,
     & c3)+c(i,7,5,5)*ui(i1+7,i2+5,i3+5,c3)
               r(i) = r(i)+c(i,0,6,5)*ui(i1,i2+6,i3+5,c3)+c(i,1,6,5)*
     & ui(i1+1,i2+6,i3+5,c3)+c(i,2,6,5)*ui(i1+2,i2+6,i3+5,c3)+c(i,3,6,
     & 5)*ui(i1+3,i2+6,i3+5,c3)+c(i,4,6,5)*ui(i1+4,i2+6,i3+5,c3)+c(i,
     & 5,6,5)*ui(i1+5,i2+6,i3+5,c3)+c(i,6,6,5)*ui(i1+6,i2+6,i3+5,c3)+
     & c(i,7,6,5)*ui(i1+7,i2+6,i3+5,c3)+c(i,0,7,5)*ui(i1,i2+7,i3+5,c3)
     & +c(i,1,7,5)*ui(i1+1,i2+7,i3+5,c3)+c(i,2,7,5)*ui(i1+2,i2+7,i3+5,
     & c3)+c(i,3,7,5)*ui(i1+3,i2+7,i3+5,c3)+c(i,4,7,5)*ui(i1+4,i2+7,
     & i3+5,c3)+c(i,5,7,5)*ui(i1+5,i2+7,i3+5,c3)+c(i,6,7,5)*ui(i1+6,
     & i2+7,i3+5,c3)+c(i,7,7,5)*ui(i1+7,i2+7,i3+5,c3)+c(i,0,0,6)*ui(
     & i1,i2,i3+6,c3)+c(i,1,0,6)*ui(i1+1,i2,i3+6,c3)+c(i,2,0,6)*ui(i1+
     & 2,i2,i3+6,c3)+c(i,3,0,6)*ui(i1+3,i2,i3+6,c3)+c(i,4,0,6)*ui(i1+
     & 4,i2,i3+6,c3)+c(i,5,0,6)*ui(i1+5,i2,i3+6,c3)+c(i,6,0,6)*ui(i1+
     & 6,i2,i3+6,c3)+c(i,7,0,6)*ui(i1+7,i2,i3+6,c3)+c(i,0,1,6)*ui(i1,
     & i2+1,i3+6,c3)+c(i,1,1,6)*ui(i1+1,i2+1,i3+6,c3)+c(i,2,1,6)*ui(
     & i1+2,i2+1,i3+6,c3)+c(i,3,1,6)*ui(i1+3,i2+1,i3+6,c3)+c(i,4,1,6)*
     & ui(i1+4,i2+1,i3+6,c3)+c(i,5,1,6)*ui(i1+5,i2+1,i3+6,c3)+c(i,6,1,
     & 6)*ui(i1+6,i2+1,i3+6,c3)+c(i,7,1,6)*ui(i1+7,i2+1,i3+6,c3)+c(i,
     & 0,2,6)*ui(i1,i2+2,i3+6,c3)+c(i,1,2,6)*ui(i1+1,i2+2,i3+6,c3)+c(
     & i,2,2,6)*ui(i1+2,i2+2,i3+6,c3)+c(i,3,2,6)*ui(i1+3,i2+2,i3+6,c3)
     & +c(i,4,2,6)*ui(i1+4,i2+2,i3+6,c3)+c(i,5,2,6)*ui(i1+5,i2+2,i3+6,
     & c3)+c(i,6,2,6)*ui(i1+6,i2+2,i3+6,c3)+c(i,7,2,6)*ui(i1+7,i2+2,
     & i3+6,c3)+c(i,0,3,6)*ui(i1,i2+3,i3+6,c3)+c(i,1,3,6)*ui(i1+1,i2+
     & 3,i3+6,c3)+c(i,2,3,6)*ui(i1+2,i2+3,i3+6,c3)+c(i,3,3,6)*ui(i1+3,
     & i2+3,i3+6,c3)+c(i,4,3,6)*ui(i1+4,i2+3,i3+6,c3)+c(i,5,3,6)*ui(
     & i1+5,i2+3,i3+6,c3)+c(i,6,3,6)*ui(i1+6,i2+3,i3+6,c3)+c(i,7,3,6)*
     & ui(i1+7,i2+3,i3+6,c3)+c(i,0,4,6)*ui(i1,i2+4,i3+6,c3)+c(i,1,4,6)
     & *ui(i1+1,i2+4,i3+6,c3)+c(i,2,4,6)*ui(i1+2,i2+4,i3+6,c3)+c(i,3,
     & 4,6)*ui(i1+3,i2+4,i3+6,c3)+c(i,4,4,6)*ui(i1+4,i2+4,i3+6,c3)+c(
     & i,5,4,6)*ui(i1+5,i2+4,i3+6,c3)+c(i,6,4,6)*ui(i1+6,i2+4,i3+6,c3)
     & +c(i,7,4,6)*ui(i1+7,i2+4,i3+6,c3)+c(i,0,5,6)*ui(i1,i2+5,i3+6,
     & c3)+c(i,1,5,6)*ui(i1+1,i2+5,i3+6,c3)+c(i,2,5,6)*ui(i1+2,i2+5,
     & i3+6,c3)+c(i,3,5,6)*ui(i1+3,i2+5,i3+6,c3)+c(i,4,5,6)*ui(i1+4,
     & i2+5,i3+6,c3)+c(i,5,5,6)*ui(i1+5,i2+5,i3+6,c3)+c(i,6,5,6)*ui(
     & i1+6,i2+5,i3+6,c3)+c(i,7,5,6)*ui(i1+7,i2+5,i3+6,c3)+c(i,0,6,6)*
     & ui(i1,i2+6,i3+6,c3)+c(i,1,6,6)*ui(i1+1,i2+6,i3+6,c3)+c(i,2,6,6)
     & *ui(i1+2,i2+6,i3+6,c3)+c(i,3,6,6)*ui(i1+3,i2+6,i3+6,c3)+c(i,4,
     & 6,6)*ui(i1+4,i2+6,i3+6,c3)+c(i,5,6,6)*ui(i1+5,i2+6,i3+6,c3)+c(
     & i,6,6,6)*ui(i1+6,i2+6,i3+6,c3)+c(i,7,6,6)*ui(i1+7,i2+6,i3+6,c3)
     & +c(i,0,7,6)*ui(i1,i2+7,i3+6,c3)+c(i,1,7,6)*ui(i1+1,i2+7,i3+6,
     & c3)+c(i,2,7,6)*ui(i1+2,i2+7,i3+6,c3)+c(i,3,7,6)*ui(i1+3,i2+7,
     & i3+6,c3)+c(i,4,7,6)*ui(i1+4,i2+7,i3+6,c3)+c(i,5,7,6)*ui(i1+5,
     & i2+7,i3+6,c3)+c(i,6,7,6)*ui(i1+6,i2+7,i3+6,c3)+c(i,7,7,6)*ui(
     & i1+7,i2+7,i3+6,c3)+c(i,0,0,7)*ui(i1,i2,i3+7,c3)+c(i,1,0,7)*ui(
     & i1+1,i2,i3+7,c3)+c(i,2,0,7)*ui(i1+2,i2,i3+7,c3)+c(i,3,0,7)*ui(
     & i1+3,i2,i3+7,c3)+c(i,4,0,7)*ui(i1+4,i2,i3+7,c3)+c(i,5,0,7)*ui(
     & i1+5,i2,i3+7,c3)+c(i,6,0,7)*ui(i1+6,i2,i3+7,c3)+c(i,7,0,7)*ui(
     & i1+7,i2,i3+7,c3)+c(i,0,1,7)*ui(i1,i2+1,i3+7,c3)+c(i,1,1,7)*ui(
     & i1+1,i2+1,i3+7,c3)+c(i,2,1,7)*ui(i1+2,i2+1,i3+7,c3)+c(i,3,1,7)*
     & ui(i1+3,i2+1,i3+7,c3)+c(i,4,1,7)*ui(i1+4,i2+1,i3+7,c3)+c(i,5,1,
     & 7)*ui(i1+5,i2+1,i3+7,c3)+c(i,6,1,7)*ui(i1+6,i2+1,i3+7,c3)+c(i,
     & 7,1,7)*ui(i1+7,i2+1,i3+7,c3)+c(i,0,2,7)*ui(i1,i2+2,i3+7,c3)+c(
     & i,1,2,7)*ui(i1+1,i2+2,i3+7,c3)+c(i,2,2,7)*ui(i1+2,i2+2,i3+7,c3)
     & +c(i,3,2,7)*ui(i1+3,i2+2,i3+7,c3)+c(i,4,2,7)*ui(i1+4,i2+2,i3+7,
     & c3)+c(i,5,2,7)*ui(i1+5,i2+2,i3+7,c3)+c(i,6,2,7)*ui(i1+6,i2+2,
     & i3+7,c3)+c(i,7,2,7)*ui(i1+7,i2+2,i3+7,c3)+c(i,0,3,7)*ui(i1,i2+
     & 3,i3+7,c3)+c(i,1,3,7)*ui(i1+1,i2+3,i3+7,c3)+c(i,2,3,7)*ui(i1+2,
     & i2+3,i3+7,c3)+c(i,3,3,7)*ui(i1+3,i2+3,i3+7,c3)+c(i,4,3,7)*ui(
     & i1+4,i2+3,i3+7,c3)+c(i,5,3,7)*ui(i1+5,i2+3,i3+7,c3)+c(i,6,3,7)*
     & ui(i1+6,i2+3,i3+7,c3)+c(i,7,3,7)*ui(i1+7,i2+3,i3+7,c3)+c(i,0,4,
     & 7)*ui(i1,i2+4,i3+7,c3)+c(i,1,4,7)*ui(i1+1,i2+4,i3+7,c3)+c(i,2,
     & 4,7)*ui(i1+2,i2+4,i3+7,c3)+c(i,3,4,7)*ui(i1+3,i2+4,i3+7,c3)+c(
     & i,4,4,7)*ui(i1+4,i2+4,i3+7,c3)+c(i,5,4,7)*ui(i1+5,i2+4,i3+7,c3)
     & +c(i,6,4,7)*ui(i1+6,i2+4,i3+7,c3)+c(i,7,4,7)*ui(i1+7,i2+4,i3+7,
     & c3)+c(i,0,5,7)*ui(i1,i2+5,i3+7,c3)+c(i,1,5,7)*ui(i1+1,i2+5,i3+
     & 7,c3)+c(i,2,5,7)*ui(i1+2,i2+5,i3+7,c3)+c(i,3,5,7)*ui(i1+3,i2+5,
     & i3+7,c3)+c(i,4,5,7)*ui(i1+4,i2+5,i3+7,c3)+c(i,5,5,7)*ui(i1+5,
     & i2+5,i3+7,c3)+c(i,6,5,7)*ui(i1+6,i2+5,i3+7,c3)+c(i,7,5,7)*ui(
     & i1+7,i2+5,i3+7,c3)+c(i,0,6,7)*ui(i1,i2+6,i3+7,c3)+c(i,1,6,7)*
     & ui(i1+1,i2+6,i3+7,c3)+c(i,2,6,7)*ui(i1+2,i2+6,i3+7,c3)+c(i,3,6,
     & 7)*ui(i1+3,i2+6,i3+7,c3)+c(i,4,6,7)*ui(i1+4,i2+6,i3+7,c3)+c(i,
     & 5,6,7)*ui(i1+5,i2+6,i3+7,c3)+c(i,6,6,7)*ui(i1+6,i2+6,i3+7,c3)+
     & c(i,7,6,7)*ui(i1+7,i2+6,i3+7,c3)+c(i,0,7,7)*ui(i1,i2+7,i3+7,c3)
     & +c(i,1,7,7)*ui(i1+1,i2+7,i3+7,c3)+c(i,2,7,7)*ui(i1+2,i2+7,i3+7,
     & c3)+c(i,3,7,7)*ui(i1+3,i2+7,i3+7,c3)+c(i,4,7,7)*ui(i1+4,i2+7,
     & i3+7,c3)+c(i,5,7,7)*ui(i1+5,i2+7,i3+7,c3)+c(i,6,7,7)*ui(i1+6,
     & i2+7,i3+7,c3)+c(i,7,7,7)*ui(i1+7,i2+7,i3+7,c3)
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           end if
         else if( width(1).eq.9 .and. width(2).eq.9 .and. width(3)
     & .eq.9 )then
! loops3d($interp999(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))
           if( c3a.eq.c3b )then
             do c3=c3a,c3b
             do i=nia,nib
! interp999(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,0,0)*ui(i1,i2,i3,c3)+c(i,1,0,0)*ui(i1+1,i2,
     & i3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3,c3)+c(i,3,0,0)*ui(i1+3,i2,i3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2,i3,c3)+c(i,5,0,0)*ui(i1+5,i2,i3,c3)+
     & c(i,6,0,0)*ui(i1+6,i2,i3,c3)+c(i,7,0,0)*ui(i1+7,i2,i3,c3)+c(i,
     & 8,0,0)*ui(i1+8,i2,i3,c3)+c(i,0,1,0)*ui(i1,i2+1,i3,c3)+c(i,1,1,
     & 0)*ui(i1+1,i2+1,i3,c3)+c(i,2,1,0)*ui(i1+2,i2+1,i3,c3)+c(i,3,1,
     & 0)*ui(i1+3,i2+1,i3,c3)+c(i,4,1,0)*ui(i1+4,i2+1,i3,c3)+c(i,5,1,
     & 0)*ui(i1+5,i2+1,i3,c3)+c(i,6,1,0)*ui(i1+6,i2+1,i3,c3)+c(i,7,1,
     & 0)*ui(i1+7,i2+1,i3,c3)+c(i,8,1,0)*ui(i1+8,i2+1,i3,c3)+c(i,0,2,
     & 0)*ui(i1,i2+2,i3,c3)+c(i,1,2,0)*ui(i1+1,i2+2,i3,c3)+c(i,2,2,0)*
     & ui(i1+2,i2+2,i3,c3)+c(i,3,2,0)*ui(i1+3,i2+2,i3,c3)+c(i,4,2,0)*
     & ui(i1+4,i2+2,i3,c3)+c(i,5,2,0)*ui(i1+5,i2+2,i3,c3)+c(i,6,2,0)*
     & ui(i1+6,i2+2,i3,c3)+c(i,7,2,0)*ui(i1+7,i2+2,i3,c3)+c(i,8,2,0)*
     & ui(i1+8,i2+2,i3,c3)+c(i,0,3,0)*ui(i1,i2+3,i3,c3)+c(i,1,3,0)*ui(
     & i1+1,i2+3,i3,c3)+c(i,2,3,0)*ui(i1+2,i2+3,i3,c3)+c(i,3,3,0)*ui(
     & i1+3,i2+3,i3,c3)+c(i,4,3,0)*ui(i1+4,i2+3,i3,c3)+c(i,5,3,0)*ui(
     & i1+5,i2+3,i3,c3)+c(i,6,3,0)*ui(i1+6,i2+3,i3,c3)+c(i,7,3,0)*ui(
     & i1+7,i2+3,i3,c3)+c(i,8,3,0)*ui(i1+8,i2+3,i3,c3)+c(i,0,4,0)*ui(
     & i1,i2+4,i3,c3)+c(i,1,4,0)*ui(i1+1,i2+4,i3,c3)+c(i,2,4,0)*ui(i1+
     & 2,i2+4,i3,c3)+c(i,3,4,0)*ui(i1+3,i2+4,i3,c3)+c(i,4,4,0)*ui(i1+
     & 4,i2+4,i3,c3)+c(i,5,4,0)*ui(i1+5,i2+4,i3,c3)+c(i,6,4,0)*ui(i1+
     & 6,i2+4,i3,c3)+c(i,7,4,0)*ui(i1+7,i2+4,i3,c3)+c(i,8,4,0)*ui(i1+
     & 8,i2+4,i3,c3)+c(i,0,5,0)*ui(i1,i2+5,i3,c3)+c(i,1,5,0)*ui(i1+1,
     & i2+5,i3,c3)+c(i,2,5,0)*ui(i1+2,i2+5,i3,c3)+c(i,3,5,0)*ui(i1+3,
     & i2+5,i3,c3)+c(i,4,5,0)*ui(i1+4,i2+5,i3,c3)+c(i,5,5,0)*ui(i1+5,
     & i2+5,i3,c3)+c(i,6,5,0)*ui(i1+6,i2+5,i3,c3)+c(i,7,5,0)*ui(i1+7,
     & i2+5,i3,c3)+c(i,8,5,0)*ui(i1+8,i2+5,i3,c3)+c(i,0,6,0)*ui(i1,i2+
     & 6,i3,c3)+c(i,1,6,0)*ui(i1+1,i2+6,i3,c3)+c(i,2,6,0)*ui(i1+2,i2+
     & 6,i3,c3)+c(i,3,6,0)*ui(i1+3,i2+6,i3,c3)+c(i,4,6,0)*ui(i1+4,i2+
     & 6,i3,c3)+c(i,5,6,0)*ui(i1+5,i2+6,i3,c3)+c(i,6,6,0)*ui(i1+6,i2+
     & 6,i3,c3)+c(i,7,6,0)*ui(i1+7,i2+6,i3,c3)+c(i,8,6,0)*ui(i1+8,i2+
     & 6,i3,c3)+c(i,0,7,0)*ui(i1,i2+7,i3,c3)+c(i,1,7,0)*ui(i1+1,i2+7,
     & i3,c3)+c(i,2,7,0)*ui(i1+2,i2+7,i3,c3)+c(i,3,7,0)*ui(i1+3,i2+7,
     & i3,c3)+c(i,4,7,0)*ui(i1+4,i2+7,i3,c3)+c(i,5,7,0)*ui(i1+5,i2+7,
     & i3,c3)+c(i,6,7,0)*ui(i1+6,i2+7,i3,c3)+c(i,7,7,0)*ui(i1+7,i2+7,
     & i3,c3)+c(i,8,7,0)*ui(i1+8,i2+7,i3,c3)+c(i,0,8,0)*ui(i1,i2+8,i3,
     & c3)+c(i,1,8,0)*ui(i1+1,i2+8,i3,c3)+c(i,2,8,0)*ui(i1+2,i2+8,i3,
     & c3)+c(i,3,8,0)*ui(i1+3,i2+8,i3,c3)+c(i,4,8,0)*ui(i1+4,i2+8,i3,
     & c3)+c(i,5,8,0)*ui(i1+5,i2+8,i3,c3)+c(i,6,8,0)*ui(i1+6,i2+8,i3,
     & c3)+c(i,7,8,0)*ui(i1+7,i2+8,i3,c3)+c(i,8,8,0)*ui(i1+8,i2+8,i3,
     & c3)+c(i,0,0,1)*ui(i1,i2,i3+1,c3)+c(i,1,0,1)*ui(i1+1,i2,i3+1,c3)
     & +c(i,2,0,1)*ui(i1+2,i2,i3+1,c3)+c(i,3,0,1)*ui(i1+3,i2,i3+1,c3)+
     & c(i,4,0,1)*ui(i1+4,i2,i3+1,c3)+c(i,5,0,1)*ui(i1+5,i2,i3+1,c3)+
     & c(i,6,0,1)*ui(i1+6,i2,i3+1,c3)+c(i,7,0,1)*ui(i1+7,i2,i3+1,c3)+
     & c(i,8,0,1)*ui(i1+8,i2,i3+1,c3)+c(i,0,1,1)*ui(i1,i2+1,i3+1,c3)+
     & c(i,1,1,1)*ui(i1+1,i2+1,i3+1,c3)+c(i,2,1,1)*ui(i1+2,i2+1,i3+1,
     & c3)+c(i,3,1,1)*ui(i1+3,i2+1,i3+1,c3)+c(i,4,1,1)*ui(i1+4,i2+1,
     & i3+1,c3)+c(i,5,1,1)*ui(i1+5,i2+1,i3+1,c3)+c(i,6,1,1)*ui(i1+6,
     & i2+1,i3+1,c3)+c(i,7,1,1)*ui(i1+7,i2+1,i3+1,c3)+c(i,8,1,1)*ui(
     & i1+8,i2+1,i3+1,c3)+c(i,0,2,1)*ui(i1,i2+2,i3+1,c3)+c(i,1,2,1)*
     & ui(i1+1,i2+2,i3+1,c3)+c(i,2,2,1)*ui(i1+2,i2+2,i3+1,c3)+c(i,3,2,
     & 1)*ui(i1+3,i2+2,i3+1,c3)+c(i,4,2,1)*ui(i1+4,i2+2,i3+1,c3)+c(i,
     & 5,2,1)*ui(i1+5,i2+2,i3+1,c3)+c(i,6,2,1)*ui(i1+6,i2+2,i3+1,c3)+
     & c(i,7,2,1)*ui(i1+7,i2+2,i3+1,c3)+c(i,8,2,1)*ui(i1+8,i2+2,i3+1,
     & c3)+c(i,0,3,1)*ui(i1,i2+3,i3+1,c3)+c(i,1,3,1)*ui(i1+1,i2+3,i3+
     & 1,c3)+c(i,2,3,1)*ui(i1+2,i2+3,i3+1,c3)+c(i,3,3,1)*ui(i1+3,i2+3,
     & i3+1,c3)+c(i,4,3,1)*ui(i1+4,i2+3,i3+1,c3)+c(i,5,3,1)*ui(i1+5,
     & i2+3,i3+1,c3)+c(i,6,3,1)*ui(i1+6,i2+3,i3+1,c3)+c(i,7,3,1)*ui(
     & i1+7,i2+3,i3+1,c3)+c(i,8,3,1)*ui(i1+8,i2+3,i3+1,c3)+c(i,0,4,1)*
     & ui(i1,i2+4,i3+1,c3)+c(i,1,4,1)*ui(i1+1,i2+4,i3+1,c3)+c(i,2,4,1)
     & *ui(i1+2,i2+4,i3+1,c3)+c(i,3,4,1)*ui(i1+3,i2+4,i3+1,c3)+c(i,4,
     & 4,1)*ui(i1+4,i2+4,i3+1,c3)+c(i,5,4,1)*ui(i1+5,i2+4,i3+1,c3)+c(
     & i,6,4,1)*ui(i1+6,i2+4,i3+1,c3)+c(i,7,4,1)*ui(i1+7,i2+4,i3+1,c3)
     & +c(i,8,4,1)*ui(i1+8,i2+4,i3+1,c3)+c(i,0,5,1)*ui(i1,i2+5,i3+1,
     & c3)+c(i,1,5,1)*ui(i1+1,i2+5,i3+1,c3)+c(i,2,5,1)*ui(i1+2,i2+5,
     & i3+1,c3)+c(i,3,5,1)*ui(i1+3,i2+5,i3+1,c3)+c(i,4,5,1)*ui(i1+4,
     & i2+5,i3+1,c3)+c(i,5,5,1)*ui(i1+5,i2+5,i3+1,c3)+c(i,6,5,1)*ui(
     & i1+6,i2+5,i3+1,c3)+c(i,7,5,1)*ui(i1+7,i2+5,i3+1,c3)+c(i,8,5,1)*
     & ui(i1+8,i2+5,i3+1,c3)+c(i,0,6,1)*ui(i1,i2+6,i3+1,c3)+c(i,1,6,1)
     & *ui(i1+1,i2+6,i3+1,c3)+c(i,2,6,1)*ui(i1+2,i2+6,i3+1,c3)+c(i,3,
     & 6,1)*ui(i1+3,i2+6,i3+1,c3)+c(i,4,6,1)*ui(i1+4,i2+6,i3+1,c3)+c(
     & i,5,6,1)*ui(i1+5,i2+6,i3+1,c3)+c(i,6,6,1)*ui(i1+6,i2+6,i3+1,c3)
     & +c(i,7,6,1)*ui(i1+7,i2+6,i3+1,c3)+c(i,8,6,1)*ui(i1+8,i2+6,i3+1,
     & c3)+c(i,0,7,1)*ui(i1,i2+7,i3+1,c3)+c(i,1,7,1)*ui(i1+1,i2+7,i3+
     & 1,c3)+c(i,2,7,1)*ui(i1+2,i2+7,i3+1,c3)+c(i,3,7,1)*ui(i1+3,i2+7,
     & i3+1,c3)+c(i,4,7,1)*ui(i1+4,i2+7,i3+1,c3)+c(i,5,7,1)*ui(i1+5,
     & i2+7,i3+1,c3)+c(i,6,7,1)*ui(i1+6,i2+7,i3+1,c3)+c(i,7,7,1)*ui(
     & i1+7,i2+7,i3+1,c3)+c(i,8,7,1)*ui(i1+8,i2+7,i3+1,c3)+c(i,0,8,1)*
     & ui(i1,i2+8,i3+1,c3)+c(i,1,8,1)*ui(i1+1,i2+8,i3+1,c3)+c(i,2,8,1)
     & *ui(i1+2,i2+8,i3+1,c3)+c(i,3,8,1)*ui(i1+3,i2+8,i3+1,c3)+c(i,4,
     & 8,1)*ui(i1+4,i2+8,i3+1,c3)+c(i,5,8,1)*ui(i1+5,i2+8,i3+1,c3)+c(
     & i,6,8,1)*ui(i1+6,i2+8,i3+1,c3)+c(i,7,8,1)*ui(i1+7,i2+8,i3+1,c3)
     & +c(i,8,8,1)*ui(i1+8,i2+8,i3+1,c3)+c(i,0,0,2)*ui(i1,i2,i3+2,c3)+
     & c(i,1,0,2)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,2)*ui(i1+2,i2,i3+2,c3)+
     & c(i,3,0,2)*ui(i1+3,i2,i3+2,c3)+c(i,4,0,2)*ui(i1+4,i2,i3+2,c3)+
     & c(i,5,0,2)*ui(i1+5,i2,i3+2,c3)+c(i,6,0,2)*ui(i1+6,i2,i3+2,c3)+
     & c(i,7,0,2)*ui(i1+7,i2,i3+2,c3)+c(i,8,0,2)*ui(i1+8,i2,i3+2,c3)+
     & c(i,0,1,2)*ui(i1,i2+1,i3+2,c3)+c(i,1,1,2)*ui(i1+1,i2+1,i3+2,c3)
     & +c(i,2,1,2)*ui(i1+2,i2+1,i3+2,c3)+c(i,3,1,2)*ui(i1+3,i2+1,i3+2,
     & c3)+c(i,4,1,2)*ui(i1+4,i2+1,i3+2,c3)+c(i,5,1,2)*ui(i1+5,i2+1,
     & i3+2,c3)+c(i,6,1,2)*ui(i1+6,i2+1,i3+2,c3)+c(i,7,1,2)*ui(i1+7,
     & i2+1,i3+2,c3)+c(i,8,1,2)*ui(i1+8,i2+1,i3+2,c3)+c(i,0,2,2)*ui(
     & i1,i2+2,i3+2,c3)+c(i,1,2,2)*ui(i1+1,i2+2,i3+2,c3)+c(i,2,2,2)*
     & ui(i1+2,i2+2,i3+2,c3)+c(i,3,2,2)*ui(i1+3,i2+2,i3+2,c3)
               r(i) = r(i)+c(i,4,2,2)*ui(i1+4,i2+2,i3+2,c3)+c(i,5,2,2)*
     & ui(i1+5,i2+2,i3+2,c3)+c(i,6,2,2)*ui(i1+6,i2+2,i3+2,c3)+c(i,7,2,
     & 2)*ui(i1+7,i2+2,i3+2,c3)+c(i,8,2,2)*ui(i1+8,i2+2,i3+2,c3)+c(i,
     & 0,3,2)*ui(i1,i2+3,i3+2,c3)+c(i,1,3,2)*ui(i1+1,i2+3,i3+2,c3)+c(
     & i,2,3,2)*ui(i1+2,i2+3,i3+2,c3)+c(i,3,3,2)*ui(i1+3,i2+3,i3+2,c3)
     & +c(i,4,3,2)*ui(i1+4,i2+3,i3+2,c3)+c(i,5,3,2)*ui(i1+5,i2+3,i3+2,
     & c3)+c(i,6,3,2)*ui(i1+6,i2+3,i3+2,c3)+c(i,7,3,2)*ui(i1+7,i2+3,
     & i3+2,c3)+c(i,8,3,2)*ui(i1+8,i2+3,i3+2,c3)+c(i,0,4,2)*ui(i1,i2+
     & 4,i3+2,c3)+c(i,1,4,2)*ui(i1+1,i2+4,i3+2,c3)+c(i,2,4,2)*ui(i1+2,
     & i2+4,i3+2,c3)+c(i,3,4,2)*ui(i1+3,i2+4,i3+2,c3)+c(i,4,4,2)*ui(
     & i1+4,i2+4,i3+2,c3)+c(i,5,4,2)*ui(i1+5,i2+4,i3+2,c3)+c(i,6,4,2)*
     & ui(i1+6,i2+4,i3+2,c3)+c(i,7,4,2)*ui(i1+7,i2+4,i3+2,c3)+c(i,8,4,
     & 2)*ui(i1+8,i2+4,i3+2,c3)+c(i,0,5,2)*ui(i1,i2+5,i3+2,c3)+c(i,1,
     & 5,2)*ui(i1+1,i2+5,i3+2,c3)+c(i,2,5,2)*ui(i1+2,i2+5,i3+2,c3)+c(
     & i,3,5,2)*ui(i1+3,i2+5,i3+2,c3)+c(i,4,5,2)*ui(i1+4,i2+5,i3+2,c3)
     & +c(i,5,5,2)*ui(i1+5,i2+5,i3+2,c3)+c(i,6,5,2)*ui(i1+6,i2+5,i3+2,
     & c3)+c(i,7,5,2)*ui(i1+7,i2+5,i3+2,c3)+c(i,8,5,2)*ui(i1+8,i2+5,
     & i3+2,c3)+c(i,0,6,2)*ui(i1,i2+6,i3+2,c3)+c(i,1,6,2)*ui(i1+1,i2+
     & 6,i3+2,c3)+c(i,2,6,2)*ui(i1+2,i2+6,i3+2,c3)+c(i,3,6,2)*ui(i1+3,
     & i2+6,i3+2,c3)+c(i,4,6,2)*ui(i1+4,i2+6,i3+2,c3)+c(i,5,6,2)*ui(
     & i1+5,i2+6,i3+2,c3)+c(i,6,6,2)*ui(i1+6,i2+6,i3+2,c3)+c(i,7,6,2)*
     & ui(i1+7,i2+6,i3+2,c3)+c(i,8,6,2)*ui(i1+8,i2+6,i3+2,c3)+c(i,0,7,
     & 2)*ui(i1,i2+7,i3+2,c3)+c(i,1,7,2)*ui(i1+1,i2+7,i3+2,c3)+c(i,2,
     & 7,2)*ui(i1+2,i2+7,i3+2,c3)+c(i,3,7,2)*ui(i1+3,i2+7,i3+2,c3)+c(
     & i,4,7,2)*ui(i1+4,i2+7,i3+2,c3)+c(i,5,7,2)*ui(i1+5,i2+7,i3+2,c3)
     & +c(i,6,7,2)*ui(i1+6,i2+7,i3+2,c3)+c(i,7,7,2)*ui(i1+7,i2+7,i3+2,
     & c3)+c(i,8,7,2)*ui(i1+8,i2+7,i3+2,c3)+c(i,0,8,2)*ui(i1,i2+8,i3+
     & 2,c3)+c(i,1,8,2)*ui(i1+1,i2+8,i3+2,c3)+c(i,2,8,2)*ui(i1+2,i2+8,
     & i3+2,c3)+c(i,3,8,2)*ui(i1+3,i2+8,i3+2,c3)+c(i,4,8,2)*ui(i1+4,
     & i2+8,i3+2,c3)+c(i,5,8,2)*ui(i1+5,i2+8,i3+2,c3)+c(i,6,8,2)*ui(
     & i1+6,i2+8,i3+2,c3)+c(i,7,8,2)*ui(i1+7,i2+8,i3+2,c3)+c(i,8,8,2)*
     & ui(i1+8,i2+8,i3+2,c3)+c(i,0,0,3)*ui(i1,i2,i3+3,c3)+c(i,1,0,3)*
     & ui(i1+1,i2,i3+3,c3)+c(i,2,0,3)*ui(i1+2,i2,i3+3,c3)+c(i,3,0,3)*
     & ui(i1+3,i2,i3+3,c3)+c(i,4,0,3)*ui(i1+4,i2,i3+3,c3)+c(i,5,0,3)*
     & ui(i1+5,i2,i3+3,c3)+c(i,6,0,3)*ui(i1+6,i2,i3+3,c3)+c(i,7,0,3)*
     & ui(i1+7,i2,i3+3,c3)+c(i,8,0,3)*ui(i1+8,i2,i3+3,c3)+c(i,0,1,3)*
     & ui(i1,i2+1,i3+3,c3)+c(i,1,1,3)*ui(i1+1,i2+1,i3+3,c3)+c(i,2,1,3)
     & *ui(i1+2,i2+1,i3+3,c3)+c(i,3,1,3)*ui(i1+3,i2+1,i3+3,c3)+c(i,4,
     & 1,3)*ui(i1+4,i2+1,i3+3,c3)+c(i,5,1,3)*ui(i1+5,i2+1,i3+3,c3)+c(
     & i,6,1,3)*ui(i1+6,i2+1,i3+3,c3)+c(i,7,1,3)*ui(i1+7,i2+1,i3+3,c3)
     & +c(i,8,1,3)*ui(i1+8,i2+1,i3+3,c3)+c(i,0,2,3)*ui(i1,i2+2,i3+3,
     & c3)+c(i,1,2,3)*ui(i1+1,i2+2,i3+3,c3)+c(i,2,2,3)*ui(i1+2,i2+2,
     & i3+3,c3)+c(i,3,2,3)*ui(i1+3,i2+2,i3+3,c3)+c(i,4,2,3)*ui(i1+4,
     & i2+2,i3+3,c3)+c(i,5,2,3)*ui(i1+5,i2+2,i3+3,c3)+c(i,6,2,3)*ui(
     & i1+6,i2+2,i3+3,c3)+c(i,7,2,3)*ui(i1+7,i2+2,i3+3,c3)+c(i,8,2,3)*
     & ui(i1+8,i2+2,i3+3,c3)+c(i,0,3,3)*ui(i1,i2+3,i3+3,c3)+c(i,1,3,3)
     & *ui(i1+1,i2+3,i3+3,c3)+c(i,2,3,3)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,
     & 3,3)*ui(i1+3,i2+3,i3+3,c3)+c(i,4,3,3)*ui(i1+4,i2+3,i3+3,c3)+c(
     & i,5,3,3)*ui(i1+5,i2+3,i3+3,c3)+c(i,6,3,3)*ui(i1+6,i2+3,i3+3,c3)
     & +c(i,7,3,3)*ui(i1+7,i2+3,i3+3,c3)+c(i,8,3,3)*ui(i1+8,i2+3,i3+3,
     & c3)+c(i,0,4,3)*ui(i1,i2+4,i3+3,c3)+c(i,1,4,3)*ui(i1+1,i2+4,i3+
     & 3,c3)+c(i,2,4,3)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,4,3)*ui(i1+3,i2+4,
     & i3+3,c3)+c(i,4,4,3)*ui(i1+4,i2+4,i3+3,c3)+c(i,5,4,3)*ui(i1+5,
     & i2+4,i3+3,c3)+c(i,6,4,3)*ui(i1+6,i2+4,i3+3,c3)+c(i,7,4,3)*ui(
     & i1+7,i2+4,i3+3,c3)+c(i,8,4,3)*ui(i1+8,i2+4,i3+3,c3)+c(i,0,5,3)*
     & ui(i1,i2+5,i3+3,c3)+c(i,1,5,3)*ui(i1+1,i2+5,i3+3,c3)+c(i,2,5,3)
     & *ui(i1+2,i2+5,i3+3,c3)+c(i,3,5,3)*ui(i1+3,i2+5,i3+3,c3)+c(i,4,
     & 5,3)*ui(i1+4,i2+5,i3+3,c3)+c(i,5,5,3)*ui(i1+5,i2+5,i3+3,c3)+c(
     & i,6,5,3)*ui(i1+6,i2+5,i3+3,c3)+c(i,7,5,3)*ui(i1+7,i2+5,i3+3,c3)
     & +c(i,8,5,3)*ui(i1+8,i2+5,i3+3,c3)+c(i,0,6,3)*ui(i1,i2+6,i3+3,
     & c3)+c(i,1,6,3)*ui(i1+1,i2+6,i3+3,c3)+c(i,2,6,3)*ui(i1+2,i2+6,
     & i3+3,c3)+c(i,3,6,3)*ui(i1+3,i2+6,i3+3,c3)+c(i,4,6,3)*ui(i1+4,
     & i2+6,i3+3,c3)+c(i,5,6,3)*ui(i1+5,i2+6,i3+3,c3)+c(i,6,6,3)*ui(
     & i1+6,i2+6,i3+3,c3)+c(i,7,6,3)*ui(i1+7,i2+6,i3+3,c3)+c(i,8,6,3)*
     & ui(i1+8,i2+6,i3+3,c3)+c(i,0,7,3)*ui(i1,i2+7,i3+3,c3)+c(i,1,7,3)
     & *ui(i1+1,i2+7,i3+3,c3)+c(i,2,7,3)*ui(i1+2,i2+7,i3+3,c3)+c(i,3,
     & 7,3)*ui(i1+3,i2+7,i3+3,c3)+c(i,4,7,3)*ui(i1+4,i2+7,i3+3,c3)+c(
     & i,5,7,3)*ui(i1+5,i2+7,i3+3,c3)+c(i,6,7,3)*ui(i1+6,i2+7,i3+3,c3)
     & +c(i,7,7,3)*ui(i1+7,i2+7,i3+3,c3)+c(i,8,7,3)*ui(i1+8,i2+7,i3+3,
     & c3)+c(i,0,8,3)*ui(i1,i2+8,i3+3,c3)+c(i,1,8,3)*ui(i1+1,i2+8,i3+
     & 3,c3)+c(i,2,8,3)*ui(i1+2,i2+8,i3+3,c3)+c(i,3,8,3)*ui(i1+3,i2+8,
     & i3+3,c3)+c(i,4,8,3)*ui(i1+4,i2+8,i3+3,c3)+c(i,5,8,3)*ui(i1+5,
     & i2+8,i3+3,c3)+c(i,6,8,3)*ui(i1+6,i2+8,i3+3,c3)+c(i,7,8,3)*ui(
     & i1+7,i2+8,i3+3,c3)+c(i,8,8,3)*ui(i1+8,i2+8,i3+3,c3)+c(i,0,0,4)*
     & ui(i1,i2,i3+4,c3)+c(i,1,0,4)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,4)*ui(
     & i1+2,i2,i3+4,c3)+c(i,3,0,4)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,4)*ui(
     & i1+4,i2,i3+4,c3)+c(i,5,0,4)*ui(i1+5,i2,i3+4,c3)+c(i,6,0,4)*ui(
     & i1+6,i2,i3+4,c3)+c(i,7,0,4)*ui(i1+7,i2,i3+4,c3)+c(i,8,0,4)*ui(
     & i1+8,i2,i3+4,c3)+c(i,0,1,4)*ui(i1,i2+1,i3+4,c3)+c(i,1,1,4)*ui(
     & i1+1,i2+1,i3+4,c3)+c(i,2,1,4)*ui(i1+2,i2+1,i3+4,c3)+c(i,3,1,4)*
     & ui(i1+3,i2+1,i3+4,c3)+c(i,4,1,4)*ui(i1+4,i2+1,i3+4,c3)+c(i,5,1,
     & 4)*ui(i1+5,i2+1,i3+4,c3)+c(i,6,1,4)*ui(i1+6,i2+1,i3+4,c3)+c(i,
     & 7,1,4)*ui(i1+7,i2+1,i3+4,c3)+c(i,8,1,4)*ui(i1+8,i2+1,i3+4,c3)+
     & c(i,0,2,4)*ui(i1,i2+2,i3+4,c3)+c(i,1,2,4)*ui(i1+1,i2+2,i3+4,c3)
     & +c(i,2,2,4)*ui(i1+2,i2+2,i3+4,c3)+c(i,3,2,4)*ui(i1+3,i2+2,i3+4,
     & c3)+c(i,4,2,4)*ui(i1+4,i2+2,i3+4,c3)+c(i,5,2,4)*ui(i1+5,i2+2,
     & i3+4,c3)+c(i,6,2,4)*ui(i1+6,i2+2,i3+4,c3)+c(i,7,2,4)*ui(i1+7,
     & i2+2,i3+4,c3)+c(i,8,2,4)*ui(i1+8,i2+2,i3+4,c3)+c(i,0,3,4)*ui(
     & i1,i2+3,i3+4,c3)+c(i,1,3,4)*ui(i1+1,i2+3,i3+4,c3)+c(i,2,3,4)*
     & ui(i1+2,i2+3,i3+4,c3)+c(i,3,3,4)*ui(i1+3,i2+3,i3+4,c3)+c(i,4,3,
     & 4)*ui(i1+4,i2+3,i3+4,c3)+c(i,5,3,4)*ui(i1+5,i2+3,i3+4,c3)+c(i,
     & 6,3,4)*ui(i1+6,i2+3,i3+4,c3)+c(i,7,3,4)*ui(i1+7,i2+3,i3+4,c3)+
     & c(i,8,3,4)*ui(i1+8,i2+3,i3+4,c3)+c(i,0,4,4)*ui(i1,i2+4,i3+4,c3)
     & +c(i,1,4,4)*ui(i1+1,i2+4,i3+4,c3)+c(i,2,4,4)*ui(i1+2,i2+4,i3+4,
     & c3)+c(i,3,4,4)*ui(i1+3,i2+4,i3+4,c3)+c(i,4,4,4)*ui(i1+4,i2+4,
     & i3+4,c3)+c(i,5,4,4)*ui(i1+5,i2+4,i3+4,c3)+c(i,6,4,4)*ui(i1+6,
     & i2+4,i3+4,c3)+c(i,7,4,4)*ui(i1+7,i2+4,i3+4,c3)
               r(i) = r(i)+c(i,8,4,4)*ui(i1+8,i2+4,i3+4,c3)+c(i,0,5,4)*
     & ui(i1,i2+5,i3+4,c3)+c(i,1,5,4)*ui(i1+1,i2+5,i3+4,c3)+c(i,2,5,4)
     & *ui(i1+2,i2+5,i3+4,c3)+c(i,3,5,4)*ui(i1+3,i2+5,i3+4,c3)+c(i,4,
     & 5,4)*ui(i1+4,i2+5,i3+4,c3)+c(i,5,5,4)*ui(i1+5,i2+5,i3+4,c3)+c(
     & i,6,5,4)*ui(i1+6,i2+5,i3+4,c3)+c(i,7,5,4)*ui(i1+7,i2+5,i3+4,c3)
     & +c(i,8,5,4)*ui(i1+8,i2+5,i3+4,c3)+c(i,0,6,4)*ui(i1,i2+6,i3+4,
     & c3)+c(i,1,6,4)*ui(i1+1,i2+6,i3+4,c3)+c(i,2,6,4)*ui(i1+2,i2+6,
     & i3+4,c3)+c(i,3,6,4)*ui(i1+3,i2+6,i3+4,c3)+c(i,4,6,4)*ui(i1+4,
     & i2+6,i3+4,c3)+c(i,5,6,4)*ui(i1+5,i2+6,i3+4,c3)+c(i,6,6,4)*ui(
     & i1+6,i2+6,i3+4,c3)+c(i,7,6,4)*ui(i1+7,i2+6,i3+4,c3)+c(i,8,6,4)*
     & ui(i1+8,i2+6,i3+4,c3)+c(i,0,7,4)*ui(i1,i2+7,i3+4,c3)+c(i,1,7,4)
     & *ui(i1+1,i2+7,i3+4,c3)+c(i,2,7,4)*ui(i1+2,i2+7,i3+4,c3)+c(i,3,
     & 7,4)*ui(i1+3,i2+7,i3+4,c3)+c(i,4,7,4)*ui(i1+4,i2+7,i3+4,c3)+c(
     & i,5,7,4)*ui(i1+5,i2+7,i3+4,c3)+c(i,6,7,4)*ui(i1+6,i2+7,i3+4,c3)
     & +c(i,7,7,4)*ui(i1+7,i2+7,i3+4,c3)+c(i,8,7,4)*ui(i1+8,i2+7,i3+4,
     & c3)+c(i,0,8,4)*ui(i1,i2+8,i3+4,c3)+c(i,1,8,4)*ui(i1+1,i2+8,i3+
     & 4,c3)+c(i,2,8,4)*ui(i1+2,i2+8,i3+4,c3)+c(i,3,8,4)*ui(i1+3,i2+8,
     & i3+4,c3)+c(i,4,8,4)*ui(i1+4,i2+8,i3+4,c3)+c(i,5,8,4)*ui(i1+5,
     & i2+8,i3+4,c3)+c(i,6,8,4)*ui(i1+6,i2+8,i3+4,c3)+c(i,7,8,4)*ui(
     & i1+7,i2+8,i3+4,c3)+c(i,8,8,4)*ui(i1+8,i2+8,i3+4,c3)+c(i,0,0,5)*
     & ui(i1,i2,i3+5,c3)+c(i,1,0,5)*ui(i1+1,i2,i3+5,c3)+c(i,2,0,5)*ui(
     & i1+2,i2,i3+5,c3)+c(i,3,0,5)*ui(i1+3,i2,i3+5,c3)+c(i,4,0,5)*ui(
     & i1+4,i2,i3+5,c3)+c(i,5,0,5)*ui(i1+5,i2,i3+5,c3)+c(i,6,0,5)*ui(
     & i1+6,i2,i3+5,c3)+c(i,7,0,5)*ui(i1+7,i2,i3+5,c3)+c(i,8,0,5)*ui(
     & i1+8,i2,i3+5,c3)+c(i,0,1,5)*ui(i1,i2+1,i3+5,c3)+c(i,1,1,5)*ui(
     & i1+1,i2+1,i3+5,c3)+c(i,2,1,5)*ui(i1+2,i2+1,i3+5,c3)+c(i,3,1,5)*
     & ui(i1+3,i2+1,i3+5,c3)+c(i,4,1,5)*ui(i1+4,i2+1,i3+5,c3)+c(i,5,1,
     & 5)*ui(i1+5,i2+1,i3+5,c3)+c(i,6,1,5)*ui(i1+6,i2+1,i3+5,c3)+c(i,
     & 7,1,5)*ui(i1+7,i2+1,i3+5,c3)+c(i,8,1,5)*ui(i1+8,i2+1,i3+5,c3)+
     & c(i,0,2,5)*ui(i1,i2+2,i3+5,c3)+c(i,1,2,5)*ui(i1+1,i2+2,i3+5,c3)
     & +c(i,2,2,5)*ui(i1+2,i2+2,i3+5,c3)+c(i,3,2,5)*ui(i1+3,i2+2,i3+5,
     & c3)+c(i,4,2,5)*ui(i1+4,i2+2,i3+5,c3)+c(i,5,2,5)*ui(i1+5,i2+2,
     & i3+5,c3)+c(i,6,2,5)*ui(i1+6,i2+2,i3+5,c3)+c(i,7,2,5)*ui(i1+7,
     & i2+2,i3+5,c3)+c(i,8,2,5)*ui(i1+8,i2+2,i3+5,c3)+c(i,0,3,5)*ui(
     & i1,i2+3,i3+5,c3)+c(i,1,3,5)*ui(i1+1,i2+3,i3+5,c3)+c(i,2,3,5)*
     & ui(i1+2,i2+3,i3+5,c3)+c(i,3,3,5)*ui(i1+3,i2+3,i3+5,c3)+c(i,4,3,
     & 5)*ui(i1+4,i2+3,i3+5,c3)+c(i,5,3,5)*ui(i1+5,i2+3,i3+5,c3)+c(i,
     & 6,3,5)*ui(i1+6,i2+3,i3+5,c3)+c(i,7,3,5)*ui(i1+7,i2+3,i3+5,c3)+
     & c(i,8,3,5)*ui(i1+8,i2+3,i3+5,c3)+c(i,0,4,5)*ui(i1,i2+4,i3+5,c3)
     & +c(i,1,4,5)*ui(i1+1,i2+4,i3+5,c3)+c(i,2,4,5)*ui(i1+2,i2+4,i3+5,
     & c3)+c(i,3,4,5)*ui(i1+3,i2+4,i3+5,c3)+c(i,4,4,5)*ui(i1+4,i2+4,
     & i3+5,c3)+c(i,5,4,5)*ui(i1+5,i2+4,i3+5,c3)+c(i,6,4,5)*ui(i1+6,
     & i2+4,i3+5,c3)+c(i,7,4,5)*ui(i1+7,i2+4,i3+5,c3)+c(i,8,4,5)*ui(
     & i1+8,i2+4,i3+5,c3)+c(i,0,5,5)*ui(i1,i2+5,i3+5,c3)+c(i,1,5,5)*
     & ui(i1+1,i2+5,i3+5,c3)+c(i,2,5,5)*ui(i1+2,i2+5,i3+5,c3)+c(i,3,5,
     & 5)*ui(i1+3,i2+5,i3+5,c3)+c(i,4,5,5)*ui(i1+4,i2+5,i3+5,c3)+c(i,
     & 5,5,5)*ui(i1+5,i2+5,i3+5,c3)+c(i,6,5,5)*ui(i1+6,i2+5,i3+5,c3)+
     & c(i,7,5,5)*ui(i1+7,i2+5,i3+5,c3)+c(i,8,5,5)*ui(i1+8,i2+5,i3+5,
     & c3)+c(i,0,6,5)*ui(i1,i2+6,i3+5,c3)+c(i,1,6,5)*ui(i1+1,i2+6,i3+
     & 5,c3)+c(i,2,6,5)*ui(i1+2,i2+6,i3+5,c3)+c(i,3,6,5)*ui(i1+3,i2+6,
     & i3+5,c3)+c(i,4,6,5)*ui(i1+4,i2+6,i3+5,c3)+c(i,5,6,5)*ui(i1+5,
     & i2+6,i3+5,c3)+c(i,6,6,5)*ui(i1+6,i2+6,i3+5,c3)+c(i,7,6,5)*ui(
     & i1+7,i2+6,i3+5,c3)+c(i,8,6,5)*ui(i1+8,i2+6,i3+5,c3)+c(i,0,7,5)*
     & ui(i1,i2+7,i3+5,c3)+c(i,1,7,5)*ui(i1+1,i2+7,i3+5,c3)+c(i,2,7,5)
     & *ui(i1+2,i2+7,i3+5,c3)+c(i,3,7,5)*ui(i1+3,i2+7,i3+5,c3)+c(i,4,
     & 7,5)*ui(i1+4,i2+7,i3+5,c3)+c(i,5,7,5)*ui(i1+5,i2+7,i3+5,c3)+c(
     & i,6,7,5)*ui(i1+6,i2+7,i3+5,c3)+c(i,7,7,5)*ui(i1+7,i2+7,i3+5,c3)
     & +c(i,8,7,5)*ui(i1+8,i2+7,i3+5,c3)+c(i,0,8,5)*ui(i1,i2+8,i3+5,
     & c3)+c(i,1,8,5)*ui(i1+1,i2+8,i3+5,c3)+c(i,2,8,5)*ui(i1+2,i2+8,
     & i3+5,c3)+c(i,3,8,5)*ui(i1+3,i2+8,i3+5,c3)+c(i,4,8,5)*ui(i1+4,
     & i2+8,i3+5,c3)+c(i,5,8,5)*ui(i1+5,i2+8,i3+5,c3)+c(i,6,8,5)*ui(
     & i1+6,i2+8,i3+5,c3)+c(i,7,8,5)*ui(i1+7,i2+8,i3+5,c3)+c(i,8,8,5)*
     & ui(i1+8,i2+8,i3+5,c3)+c(i,0,0,6)*ui(i1,i2,i3+6,c3)+c(i,1,0,6)*
     & ui(i1+1,i2,i3+6,c3)+c(i,2,0,6)*ui(i1+2,i2,i3+6,c3)+c(i,3,0,6)*
     & ui(i1+3,i2,i3+6,c3)+c(i,4,0,6)*ui(i1+4,i2,i3+6,c3)+c(i,5,0,6)*
     & ui(i1+5,i2,i3+6,c3)+c(i,6,0,6)*ui(i1+6,i2,i3+6,c3)+c(i,7,0,6)*
     & ui(i1+7,i2,i3+6,c3)+c(i,8,0,6)*ui(i1+8,i2,i3+6,c3)+c(i,0,1,6)*
     & ui(i1,i2+1,i3+6,c3)+c(i,1,1,6)*ui(i1+1,i2+1,i3+6,c3)+c(i,2,1,6)
     & *ui(i1+2,i2+1,i3+6,c3)+c(i,3,1,6)*ui(i1+3,i2+1,i3+6,c3)+c(i,4,
     & 1,6)*ui(i1+4,i2+1,i3+6,c3)+c(i,5,1,6)*ui(i1+5,i2+1,i3+6,c3)+c(
     & i,6,1,6)*ui(i1+6,i2+1,i3+6,c3)+c(i,7,1,6)*ui(i1+7,i2+1,i3+6,c3)
     & +c(i,8,1,6)*ui(i1+8,i2+1,i3+6,c3)+c(i,0,2,6)*ui(i1,i2+2,i3+6,
     & c3)+c(i,1,2,6)*ui(i1+1,i2+2,i3+6,c3)+c(i,2,2,6)*ui(i1+2,i2+2,
     & i3+6,c3)+c(i,3,2,6)*ui(i1+3,i2+2,i3+6,c3)+c(i,4,2,6)*ui(i1+4,
     & i2+2,i3+6,c3)+c(i,5,2,6)*ui(i1+5,i2+2,i3+6,c3)+c(i,6,2,6)*ui(
     & i1+6,i2+2,i3+6,c3)+c(i,7,2,6)*ui(i1+7,i2+2,i3+6,c3)+c(i,8,2,6)*
     & ui(i1+8,i2+2,i3+6,c3)+c(i,0,3,6)*ui(i1,i2+3,i3+6,c3)+c(i,1,3,6)
     & *ui(i1+1,i2+3,i3+6,c3)+c(i,2,3,6)*ui(i1+2,i2+3,i3+6,c3)+c(i,3,
     & 3,6)*ui(i1+3,i2+3,i3+6,c3)+c(i,4,3,6)*ui(i1+4,i2+3,i3+6,c3)+c(
     & i,5,3,6)*ui(i1+5,i2+3,i3+6,c3)+c(i,6,3,6)*ui(i1+6,i2+3,i3+6,c3)
     & +c(i,7,3,6)*ui(i1+7,i2+3,i3+6,c3)+c(i,8,3,6)*ui(i1+8,i2+3,i3+6,
     & c3)+c(i,0,4,6)*ui(i1,i2+4,i3+6,c3)+c(i,1,4,6)*ui(i1+1,i2+4,i3+
     & 6,c3)+c(i,2,4,6)*ui(i1+2,i2+4,i3+6,c3)+c(i,3,4,6)*ui(i1+3,i2+4,
     & i3+6,c3)+c(i,4,4,6)*ui(i1+4,i2+4,i3+6,c3)+c(i,5,4,6)*ui(i1+5,
     & i2+4,i3+6,c3)+c(i,6,4,6)*ui(i1+6,i2+4,i3+6,c3)+c(i,7,4,6)*ui(
     & i1+7,i2+4,i3+6,c3)+c(i,8,4,6)*ui(i1+8,i2+4,i3+6,c3)+c(i,0,5,6)*
     & ui(i1,i2+5,i3+6,c3)+c(i,1,5,6)*ui(i1+1,i2+5,i3+6,c3)+c(i,2,5,6)
     & *ui(i1+2,i2+5,i3+6,c3)+c(i,3,5,6)*ui(i1+3,i2+5,i3+6,c3)+c(i,4,
     & 5,6)*ui(i1+4,i2+5,i3+6,c3)+c(i,5,5,6)*ui(i1+5,i2+5,i3+6,c3)+c(
     & i,6,5,6)*ui(i1+6,i2+5,i3+6,c3)+c(i,7,5,6)*ui(i1+7,i2+5,i3+6,c3)
     & +c(i,8,5,6)*ui(i1+8,i2+5,i3+6,c3)+c(i,0,6,6)*ui(i1,i2+6,i3+6,
     & c3)+c(i,1,6,6)*ui(i1+1,i2+6,i3+6,c3)+c(i,2,6,6)*ui(i1+2,i2+6,
     & i3+6,c3)+c(i,3,6,6)*ui(i1+3,i2+6,i3+6,c3)+c(i,4,6,6)*ui(i1+4,
     & i2+6,i3+6,c3)+c(i,5,6,6)*ui(i1+5,i2+6,i3+6,c3)+c(i,6,6,6)*ui(
     & i1+6,i2+6,i3+6,c3)+c(i,7,6,6)*ui(i1+7,i2+6,i3+6,c3)+c(i,8,6,6)*
     & ui(i1+8,i2+6,i3+6,c3)+c(i,0,7,6)*ui(i1,i2+7,i3+6,c3)+c(i,1,7,6)
     & *ui(i1+1,i2+7,i3+6,c3)+c(i,2,7,6)*ui(i1+2,i2+7,i3+6,c3)
               r(i) = r(i)+c(i,3,7,6)*ui(i1+3,i2+7,i3+6,c3)+c(i,4,7,6)*
     & ui(i1+4,i2+7,i3+6,c3)+c(i,5,7,6)*ui(i1+5,i2+7,i3+6,c3)+c(i,6,7,
     & 6)*ui(i1+6,i2+7,i3+6,c3)+c(i,7,7,6)*ui(i1+7,i2+7,i3+6,c3)+c(i,
     & 8,7,6)*ui(i1+8,i2+7,i3+6,c3)+c(i,0,8,6)*ui(i1,i2+8,i3+6,c3)+c(
     & i,1,8,6)*ui(i1+1,i2+8,i3+6,c3)+c(i,2,8,6)*ui(i1+2,i2+8,i3+6,c3)
     & +c(i,3,8,6)*ui(i1+3,i2+8,i3+6,c3)+c(i,4,8,6)*ui(i1+4,i2+8,i3+6,
     & c3)+c(i,5,8,6)*ui(i1+5,i2+8,i3+6,c3)+c(i,6,8,6)*ui(i1+6,i2+8,
     & i3+6,c3)+c(i,7,8,6)*ui(i1+7,i2+8,i3+6,c3)+c(i,8,8,6)*ui(i1+8,
     & i2+8,i3+6,c3)+c(i,0,0,7)*ui(i1,i2,i3+7,c3)+c(i,1,0,7)*ui(i1+1,
     & i2,i3+7,c3)+c(i,2,0,7)*ui(i1+2,i2,i3+7,c3)+c(i,3,0,7)*ui(i1+3,
     & i2,i3+7,c3)+c(i,4,0,7)*ui(i1+4,i2,i3+7,c3)+c(i,5,0,7)*ui(i1+5,
     & i2,i3+7,c3)+c(i,6,0,7)*ui(i1+6,i2,i3+7,c3)+c(i,7,0,7)*ui(i1+7,
     & i2,i3+7,c3)+c(i,8,0,7)*ui(i1+8,i2,i3+7,c3)+c(i,0,1,7)*ui(i1,i2+
     & 1,i3+7,c3)+c(i,1,1,7)*ui(i1+1,i2+1,i3+7,c3)+c(i,2,1,7)*ui(i1+2,
     & i2+1,i3+7,c3)+c(i,3,1,7)*ui(i1+3,i2+1,i3+7,c3)+c(i,4,1,7)*ui(
     & i1+4,i2+1,i3+7,c3)+c(i,5,1,7)*ui(i1+5,i2+1,i3+7,c3)+c(i,6,1,7)*
     & ui(i1+6,i2+1,i3+7,c3)+c(i,7,1,7)*ui(i1+7,i2+1,i3+7,c3)+c(i,8,1,
     & 7)*ui(i1+8,i2+1,i3+7,c3)+c(i,0,2,7)*ui(i1,i2+2,i3+7,c3)+c(i,1,
     & 2,7)*ui(i1+1,i2+2,i3+7,c3)+c(i,2,2,7)*ui(i1+2,i2+2,i3+7,c3)+c(
     & i,3,2,7)*ui(i1+3,i2+2,i3+7,c3)+c(i,4,2,7)*ui(i1+4,i2+2,i3+7,c3)
     & +c(i,5,2,7)*ui(i1+5,i2+2,i3+7,c3)+c(i,6,2,7)*ui(i1+6,i2+2,i3+7,
     & c3)+c(i,7,2,7)*ui(i1+7,i2+2,i3+7,c3)+c(i,8,2,7)*ui(i1+8,i2+2,
     & i3+7,c3)+c(i,0,3,7)*ui(i1,i2+3,i3+7,c3)+c(i,1,3,7)*ui(i1+1,i2+
     & 3,i3+7,c3)+c(i,2,3,7)*ui(i1+2,i2+3,i3+7,c3)+c(i,3,3,7)*ui(i1+3,
     & i2+3,i3+7,c3)+c(i,4,3,7)*ui(i1+4,i2+3,i3+7,c3)+c(i,5,3,7)*ui(
     & i1+5,i2+3,i3+7,c3)+c(i,6,3,7)*ui(i1+6,i2+3,i3+7,c3)+c(i,7,3,7)*
     & ui(i1+7,i2+3,i3+7,c3)+c(i,8,3,7)*ui(i1+8,i2+3,i3+7,c3)+c(i,0,4,
     & 7)*ui(i1,i2+4,i3+7,c3)+c(i,1,4,7)*ui(i1+1,i2+4,i3+7,c3)+c(i,2,
     & 4,7)*ui(i1+2,i2+4,i3+7,c3)+c(i,3,4,7)*ui(i1+3,i2+4,i3+7,c3)+c(
     & i,4,4,7)*ui(i1+4,i2+4,i3+7,c3)+c(i,5,4,7)*ui(i1+5,i2+4,i3+7,c3)
     & +c(i,6,4,7)*ui(i1+6,i2+4,i3+7,c3)+c(i,7,4,7)*ui(i1+7,i2+4,i3+7,
     & c3)+c(i,8,4,7)*ui(i1+8,i2+4,i3+7,c3)+c(i,0,5,7)*ui(i1,i2+5,i3+
     & 7,c3)+c(i,1,5,7)*ui(i1+1,i2+5,i3+7,c3)+c(i,2,5,7)*ui(i1+2,i2+5,
     & i3+7,c3)+c(i,3,5,7)*ui(i1+3,i2+5,i3+7,c3)+c(i,4,5,7)*ui(i1+4,
     & i2+5,i3+7,c3)+c(i,5,5,7)*ui(i1+5,i2+5,i3+7,c3)+c(i,6,5,7)*ui(
     & i1+6,i2+5,i3+7,c3)+c(i,7,5,7)*ui(i1+7,i2+5,i3+7,c3)+c(i,8,5,7)*
     & ui(i1+8,i2+5,i3+7,c3)+c(i,0,6,7)*ui(i1,i2+6,i3+7,c3)+c(i,1,6,7)
     & *ui(i1+1,i2+6,i3+7,c3)+c(i,2,6,7)*ui(i1+2,i2+6,i3+7,c3)+c(i,3,
     & 6,7)*ui(i1+3,i2+6,i3+7,c3)+c(i,4,6,7)*ui(i1+4,i2+6,i3+7,c3)+c(
     & i,5,6,7)*ui(i1+5,i2+6,i3+7,c3)+c(i,6,6,7)*ui(i1+6,i2+6,i3+7,c3)
     & +c(i,7,6,7)*ui(i1+7,i2+6,i3+7,c3)+c(i,8,6,7)*ui(i1+8,i2+6,i3+7,
     & c3)+c(i,0,7,7)*ui(i1,i2+7,i3+7,c3)+c(i,1,7,7)*ui(i1+1,i2+7,i3+
     & 7,c3)+c(i,2,7,7)*ui(i1+2,i2+7,i3+7,c3)+c(i,3,7,7)*ui(i1+3,i2+7,
     & i3+7,c3)+c(i,4,7,7)*ui(i1+4,i2+7,i3+7,c3)+c(i,5,7,7)*ui(i1+5,
     & i2+7,i3+7,c3)+c(i,6,7,7)*ui(i1+6,i2+7,i3+7,c3)+c(i,7,7,7)*ui(
     & i1+7,i2+7,i3+7,c3)+c(i,8,7,7)*ui(i1+8,i2+7,i3+7,c3)+c(i,0,8,7)*
     & ui(i1,i2+8,i3+7,c3)+c(i,1,8,7)*ui(i1+1,i2+8,i3+7,c3)+c(i,2,8,7)
     & *ui(i1+2,i2+8,i3+7,c3)+c(i,3,8,7)*ui(i1+3,i2+8,i3+7,c3)+c(i,4,
     & 8,7)*ui(i1+4,i2+8,i3+7,c3)+c(i,5,8,7)*ui(i1+5,i2+8,i3+7,c3)+c(
     & i,6,8,7)*ui(i1+6,i2+8,i3+7,c3)+c(i,7,8,7)*ui(i1+7,i2+8,i3+7,c3)
     & +c(i,8,8,7)*ui(i1+8,i2+8,i3+7,c3)+c(i,0,0,8)*ui(i1,i2,i3+8,c3)+
     & c(i,1,0,8)*ui(i1+1,i2,i3+8,c3)+c(i,2,0,8)*ui(i1+2,i2,i3+8,c3)+
     & c(i,3,0,8)*ui(i1+3,i2,i3+8,c3)+c(i,4,0,8)*ui(i1+4,i2,i3+8,c3)+
     & c(i,5,0,8)*ui(i1+5,i2,i3+8,c3)+c(i,6,0,8)*ui(i1+6,i2,i3+8,c3)+
     & c(i,7,0,8)*ui(i1+7,i2,i3+8,c3)+c(i,8,0,8)*ui(i1+8,i2,i3+8,c3)+
     & c(i,0,1,8)*ui(i1,i2+1,i3+8,c3)+c(i,1,1,8)*ui(i1+1,i2+1,i3+8,c3)
     & +c(i,2,1,8)*ui(i1+2,i2+1,i3+8,c3)+c(i,3,1,8)*ui(i1+3,i2+1,i3+8,
     & c3)+c(i,4,1,8)*ui(i1+4,i2+1,i3+8,c3)+c(i,5,1,8)*ui(i1+5,i2+1,
     & i3+8,c3)+c(i,6,1,8)*ui(i1+6,i2+1,i3+8,c3)+c(i,7,1,8)*ui(i1+7,
     & i2+1,i3+8,c3)+c(i,8,1,8)*ui(i1+8,i2+1,i3+8,c3)+c(i,0,2,8)*ui(
     & i1,i2+2,i3+8,c3)+c(i,1,2,8)*ui(i1+1,i2+2,i3+8,c3)+c(i,2,2,8)*
     & ui(i1+2,i2+2,i3+8,c3)+c(i,3,2,8)*ui(i1+3,i2+2,i3+8,c3)+c(i,4,2,
     & 8)*ui(i1+4,i2+2,i3+8,c3)+c(i,5,2,8)*ui(i1+5,i2+2,i3+8,c3)+c(i,
     & 6,2,8)*ui(i1+6,i2+2,i3+8,c3)+c(i,7,2,8)*ui(i1+7,i2+2,i3+8,c3)+
     & c(i,8,2,8)*ui(i1+8,i2+2,i3+8,c3)+c(i,0,3,8)*ui(i1,i2+3,i3+8,c3)
     & +c(i,1,3,8)*ui(i1+1,i2+3,i3+8,c3)+c(i,2,3,8)*ui(i1+2,i2+3,i3+8,
     & c3)+c(i,3,3,8)*ui(i1+3,i2+3,i3+8,c3)+c(i,4,3,8)*ui(i1+4,i2+3,
     & i3+8,c3)+c(i,5,3,8)*ui(i1+5,i2+3,i3+8,c3)+c(i,6,3,8)*ui(i1+6,
     & i2+3,i3+8,c3)+c(i,7,3,8)*ui(i1+7,i2+3,i3+8,c3)+c(i,8,3,8)*ui(
     & i1+8,i2+3,i3+8,c3)+c(i,0,4,8)*ui(i1,i2+4,i3+8,c3)+c(i,1,4,8)*
     & ui(i1+1,i2+4,i3+8,c3)+c(i,2,4,8)*ui(i1+2,i2+4,i3+8,c3)+c(i,3,4,
     & 8)*ui(i1+3,i2+4,i3+8,c3)+c(i,4,4,8)*ui(i1+4,i2+4,i3+8,c3)+c(i,
     & 5,4,8)*ui(i1+5,i2+4,i3+8,c3)+c(i,6,4,8)*ui(i1+6,i2+4,i3+8,c3)+
     & c(i,7,4,8)*ui(i1+7,i2+4,i3+8,c3)+c(i,8,4,8)*ui(i1+8,i2+4,i3+8,
     & c3)+c(i,0,5,8)*ui(i1,i2+5,i3+8,c3)+c(i,1,5,8)*ui(i1+1,i2+5,i3+
     & 8,c3)+c(i,2,5,8)*ui(i1+2,i2+5,i3+8,c3)+c(i,3,5,8)*ui(i1+3,i2+5,
     & i3+8,c3)+c(i,4,5,8)*ui(i1+4,i2+5,i3+8,c3)+c(i,5,5,8)*ui(i1+5,
     & i2+5,i3+8,c3)+c(i,6,5,8)*ui(i1+6,i2+5,i3+8,c3)+c(i,7,5,8)*ui(
     & i1+7,i2+5,i3+8,c3)+c(i,8,5,8)*ui(i1+8,i2+5,i3+8,c3)+c(i,0,6,8)*
     & ui(i1,i2+6,i3+8,c3)+c(i,1,6,8)*ui(i1+1,i2+6,i3+8,c3)+c(i,2,6,8)
     & *ui(i1+2,i2+6,i3+8,c3)+c(i,3,6,8)*ui(i1+3,i2+6,i3+8,c3)+c(i,4,
     & 6,8)*ui(i1+4,i2+6,i3+8,c3)+c(i,5,6,8)*ui(i1+5,i2+6,i3+8,c3)+c(
     & i,6,6,8)*ui(i1+6,i2+6,i3+8,c3)+c(i,7,6,8)*ui(i1+7,i2+6,i3+8,c3)
     & +c(i,8,6,8)*ui(i1+8,i2+6,i3+8,c3)+c(i,0,7,8)*ui(i1,i2+7,i3+8,
     & c3)+c(i,1,7,8)*ui(i1+1,i2+7,i3+8,c3)+c(i,2,7,8)*ui(i1+2,i2+7,
     & i3+8,c3)+c(i,3,7,8)*ui(i1+3,i2+7,i3+8,c3)+c(i,4,7,8)*ui(i1+4,
     & i2+7,i3+8,c3)+c(i,5,7,8)*ui(i1+5,i2+7,i3+8,c3)+c(i,6,7,8)*ui(
     & i1+6,i2+7,i3+8,c3)+c(i,7,7,8)*ui(i1+7,i2+7,i3+8,c3)+c(i,8,7,8)*
     & ui(i1+8,i2+7,i3+8,c3)+c(i,0,8,8)*ui(i1,i2+8,i3+8,c3)+c(i,1,8,8)
     & *ui(i1+1,i2+8,i3+8,c3)+c(i,2,8,8)*ui(i1+2,i2+8,i3+8,c3)+c(i,3,
     & 8,8)*ui(i1+3,i2+8,i3+8,c3)+c(i,4,8,8)*ui(i1+4,i2+8,i3+8,c3)+c(
     & i,5,8,8)*ui(i1+5,i2+8,i3+8,c3)+c(i,6,8,8)*ui(i1+6,i2+8,i3+8,c3)
     & +c(i,7,8,8)*ui(i1+7,i2+8,i3+8,c3)+c(i,8,8,8)*ui(i1+8,i2+8,i3+8,
     & c3)
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
! interp999(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = c(i,0,0,0)*ui(i1,i2,i3,c3)+c(i,1,0,0)*ui(i1+1,i2,
     & i3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3,c3)+c(i,3,0,0)*ui(i1+3,i2,i3,
     & c3)+c(i,4,0,0)*ui(i1+4,i2,i3,c3)+c(i,5,0,0)*ui(i1+5,i2,i3,c3)+
     & c(i,6,0,0)*ui(i1+6,i2,i3,c3)+c(i,7,0,0)*ui(i1+7,i2,i3,c3)+c(i,
     & 8,0,0)*ui(i1+8,i2,i3,c3)+c(i,0,1,0)*ui(i1,i2+1,i3,c3)+c(i,1,1,
     & 0)*ui(i1+1,i2+1,i3,c3)+c(i,2,1,0)*ui(i1+2,i2+1,i3,c3)+c(i,3,1,
     & 0)*ui(i1+3,i2+1,i3,c3)+c(i,4,1,0)*ui(i1+4,i2+1,i3,c3)+c(i,5,1,
     & 0)*ui(i1+5,i2+1,i3,c3)+c(i,6,1,0)*ui(i1+6,i2+1,i3,c3)+c(i,7,1,
     & 0)*ui(i1+7,i2+1,i3,c3)+c(i,8,1,0)*ui(i1+8,i2+1,i3,c3)+c(i,0,2,
     & 0)*ui(i1,i2+2,i3,c3)+c(i,1,2,0)*ui(i1+1,i2+2,i3,c3)+c(i,2,2,0)*
     & ui(i1+2,i2+2,i3,c3)+c(i,3,2,0)*ui(i1+3,i2+2,i3,c3)+c(i,4,2,0)*
     & ui(i1+4,i2+2,i3,c3)+c(i,5,2,0)*ui(i1+5,i2+2,i3,c3)+c(i,6,2,0)*
     & ui(i1+6,i2+2,i3,c3)+c(i,7,2,0)*ui(i1+7,i2+2,i3,c3)+c(i,8,2,0)*
     & ui(i1+8,i2+2,i3,c3)+c(i,0,3,0)*ui(i1,i2+3,i3,c3)+c(i,1,3,0)*ui(
     & i1+1,i2+3,i3,c3)+c(i,2,3,0)*ui(i1+2,i2+3,i3,c3)+c(i,3,3,0)*ui(
     & i1+3,i2+3,i3,c3)+c(i,4,3,0)*ui(i1+4,i2+3,i3,c3)+c(i,5,3,0)*ui(
     & i1+5,i2+3,i3,c3)+c(i,6,3,0)*ui(i1+6,i2+3,i3,c3)+c(i,7,3,0)*ui(
     & i1+7,i2+3,i3,c3)+c(i,8,3,0)*ui(i1+8,i2+3,i3,c3)+c(i,0,4,0)*ui(
     & i1,i2+4,i3,c3)+c(i,1,4,0)*ui(i1+1,i2+4,i3,c3)+c(i,2,4,0)*ui(i1+
     & 2,i2+4,i3,c3)+c(i,3,4,0)*ui(i1+3,i2+4,i3,c3)+c(i,4,4,0)*ui(i1+
     & 4,i2+4,i3,c3)+c(i,5,4,0)*ui(i1+5,i2+4,i3,c3)+c(i,6,4,0)*ui(i1+
     & 6,i2+4,i3,c3)+c(i,7,4,0)*ui(i1+7,i2+4,i3,c3)+c(i,8,4,0)*ui(i1+
     & 8,i2+4,i3,c3)+c(i,0,5,0)*ui(i1,i2+5,i3,c3)+c(i,1,5,0)*ui(i1+1,
     & i2+5,i3,c3)+c(i,2,5,0)*ui(i1+2,i2+5,i3,c3)+c(i,3,5,0)*ui(i1+3,
     & i2+5,i3,c3)+c(i,4,5,0)*ui(i1+4,i2+5,i3,c3)+c(i,5,5,0)*ui(i1+5,
     & i2+5,i3,c3)+c(i,6,5,0)*ui(i1+6,i2+5,i3,c3)+c(i,7,5,0)*ui(i1+7,
     & i2+5,i3,c3)+c(i,8,5,0)*ui(i1+8,i2+5,i3,c3)+c(i,0,6,0)*ui(i1,i2+
     & 6,i3,c3)+c(i,1,6,0)*ui(i1+1,i2+6,i3,c3)+c(i,2,6,0)*ui(i1+2,i2+
     & 6,i3,c3)+c(i,3,6,0)*ui(i1+3,i2+6,i3,c3)+c(i,4,6,0)*ui(i1+4,i2+
     & 6,i3,c3)+c(i,5,6,0)*ui(i1+5,i2+6,i3,c3)+c(i,6,6,0)*ui(i1+6,i2+
     & 6,i3,c3)+c(i,7,6,0)*ui(i1+7,i2+6,i3,c3)+c(i,8,6,0)*ui(i1+8,i2+
     & 6,i3,c3)+c(i,0,7,0)*ui(i1,i2+7,i3,c3)+c(i,1,7,0)*ui(i1+1,i2+7,
     & i3,c3)+c(i,2,7,0)*ui(i1+2,i2+7,i3,c3)+c(i,3,7,0)*ui(i1+3,i2+7,
     & i3,c3)+c(i,4,7,0)*ui(i1+4,i2+7,i3,c3)+c(i,5,7,0)*ui(i1+5,i2+7,
     & i3,c3)+c(i,6,7,0)*ui(i1+6,i2+7,i3,c3)+c(i,7,7,0)*ui(i1+7,i2+7,
     & i3,c3)+c(i,8,7,0)*ui(i1+8,i2+7,i3,c3)+c(i,0,8,0)*ui(i1,i2+8,i3,
     & c3)+c(i,1,8,0)*ui(i1+1,i2+8,i3,c3)+c(i,2,8,0)*ui(i1+2,i2+8,i3,
     & c3)+c(i,3,8,0)*ui(i1+3,i2+8,i3,c3)+c(i,4,8,0)*ui(i1+4,i2+8,i3,
     & c3)+c(i,5,8,0)*ui(i1+5,i2+8,i3,c3)+c(i,6,8,0)*ui(i1+6,i2+8,i3,
     & c3)+c(i,7,8,0)*ui(i1+7,i2+8,i3,c3)+c(i,8,8,0)*ui(i1+8,i2+8,i3,
     & c3)+c(i,0,0,1)*ui(i1,i2,i3+1,c3)+c(i,1,0,1)*ui(i1+1,i2,i3+1,c3)
     & +c(i,2,0,1)*ui(i1+2,i2,i3+1,c3)+c(i,3,0,1)*ui(i1+3,i2,i3+1,c3)+
     & c(i,4,0,1)*ui(i1+4,i2,i3+1,c3)+c(i,5,0,1)*ui(i1+5,i2,i3+1,c3)+
     & c(i,6,0,1)*ui(i1+6,i2,i3+1,c3)+c(i,7,0,1)*ui(i1+7,i2,i3+1,c3)+
     & c(i,8,0,1)*ui(i1+8,i2,i3+1,c3)+c(i,0,1,1)*ui(i1,i2+1,i3+1,c3)+
     & c(i,1,1,1)*ui(i1+1,i2+1,i3+1,c3)+c(i,2,1,1)*ui(i1+2,i2+1,i3+1,
     & c3)+c(i,3,1,1)*ui(i1+3,i2+1,i3+1,c3)+c(i,4,1,1)*ui(i1+4,i2+1,
     & i3+1,c3)+c(i,5,1,1)*ui(i1+5,i2+1,i3+1,c3)+c(i,6,1,1)*ui(i1+6,
     & i2+1,i3+1,c3)+c(i,7,1,1)*ui(i1+7,i2+1,i3+1,c3)+c(i,8,1,1)*ui(
     & i1+8,i2+1,i3+1,c3)+c(i,0,2,1)*ui(i1,i2+2,i3+1,c3)+c(i,1,2,1)*
     & ui(i1+1,i2+2,i3+1,c3)+c(i,2,2,1)*ui(i1+2,i2+2,i3+1,c3)+c(i,3,2,
     & 1)*ui(i1+3,i2+2,i3+1,c3)+c(i,4,2,1)*ui(i1+4,i2+2,i3+1,c3)+c(i,
     & 5,2,1)*ui(i1+5,i2+2,i3+1,c3)+c(i,6,2,1)*ui(i1+6,i2+2,i3+1,c3)+
     & c(i,7,2,1)*ui(i1+7,i2+2,i3+1,c3)+c(i,8,2,1)*ui(i1+8,i2+2,i3+1,
     & c3)+c(i,0,3,1)*ui(i1,i2+3,i3+1,c3)+c(i,1,3,1)*ui(i1+1,i2+3,i3+
     & 1,c3)+c(i,2,3,1)*ui(i1+2,i2+3,i3+1,c3)+c(i,3,3,1)*ui(i1+3,i2+3,
     & i3+1,c3)+c(i,4,3,1)*ui(i1+4,i2+3,i3+1,c3)+c(i,5,3,1)*ui(i1+5,
     & i2+3,i3+1,c3)+c(i,6,3,1)*ui(i1+6,i2+3,i3+1,c3)+c(i,7,3,1)*ui(
     & i1+7,i2+3,i3+1,c3)+c(i,8,3,1)*ui(i1+8,i2+3,i3+1,c3)+c(i,0,4,1)*
     & ui(i1,i2+4,i3+1,c3)+c(i,1,4,1)*ui(i1+1,i2+4,i3+1,c3)+c(i,2,4,1)
     & *ui(i1+2,i2+4,i3+1,c3)+c(i,3,4,1)*ui(i1+3,i2+4,i3+1,c3)+c(i,4,
     & 4,1)*ui(i1+4,i2+4,i3+1,c3)+c(i,5,4,1)*ui(i1+5,i2+4,i3+1,c3)+c(
     & i,6,4,1)*ui(i1+6,i2+4,i3+1,c3)+c(i,7,4,1)*ui(i1+7,i2+4,i3+1,c3)
     & +c(i,8,4,1)*ui(i1+8,i2+4,i3+1,c3)+c(i,0,5,1)*ui(i1,i2+5,i3+1,
     & c3)+c(i,1,5,1)*ui(i1+1,i2+5,i3+1,c3)+c(i,2,5,1)*ui(i1+2,i2+5,
     & i3+1,c3)+c(i,3,5,1)*ui(i1+3,i2+5,i3+1,c3)+c(i,4,5,1)*ui(i1+4,
     & i2+5,i3+1,c3)+c(i,5,5,1)*ui(i1+5,i2+5,i3+1,c3)+c(i,6,5,1)*ui(
     & i1+6,i2+5,i3+1,c3)+c(i,7,5,1)*ui(i1+7,i2+5,i3+1,c3)+c(i,8,5,1)*
     & ui(i1+8,i2+5,i3+1,c3)+c(i,0,6,1)*ui(i1,i2+6,i3+1,c3)+c(i,1,6,1)
     & *ui(i1+1,i2+6,i3+1,c3)+c(i,2,6,1)*ui(i1+2,i2+6,i3+1,c3)+c(i,3,
     & 6,1)*ui(i1+3,i2+6,i3+1,c3)+c(i,4,6,1)*ui(i1+4,i2+6,i3+1,c3)+c(
     & i,5,6,1)*ui(i1+5,i2+6,i3+1,c3)+c(i,6,6,1)*ui(i1+6,i2+6,i3+1,c3)
     & +c(i,7,6,1)*ui(i1+7,i2+6,i3+1,c3)+c(i,8,6,1)*ui(i1+8,i2+6,i3+1,
     & c3)+c(i,0,7,1)*ui(i1,i2+7,i3+1,c3)+c(i,1,7,1)*ui(i1+1,i2+7,i3+
     & 1,c3)+c(i,2,7,1)*ui(i1+2,i2+7,i3+1,c3)+c(i,3,7,1)*ui(i1+3,i2+7,
     & i3+1,c3)+c(i,4,7,1)*ui(i1+4,i2+7,i3+1,c3)+c(i,5,7,1)*ui(i1+5,
     & i2+7,i3+1,c3)+c(i,6,7,1)*ui(i1+6,i2+7,i3+1,c3)+c(i,7,7,1)*ui(
     & i1+7,i2+7,i3+1,c3)+c(i,8,7,1)*ui(i1+8,i2+7,i3+1,c3)+c(i,0,8,1)*
     & ui(i1,i2+8,i3+1,c3)+c(i,1,8,1)*ui(i1+1,i2+8,i3+1,c3)+c(i,2,8,1)
     & *ui(i1+2,i2+8,i3+1,c3)+c(i,3,8,1)*ui(i1+3,i2+8,i3+1,c3)+c(i,4,
     & 8,1)*ui(i1+4,i2+8,i3+1,c3)+c(i,5,8,1)*ui(i1+5,i2+8,i3+1,c3)+c(
     & i,6,8,1)*ui(i1+6,i2+8,i3+1,c3)+c(i,7,8,1)*ui(i1+7,i2+8,i3+1,c3)
     & +c(i,8,8,1)*ui(i1+8,i2+8,i3+1,c3)+c(i,0,0,2)*ui(i1,i2,i3+2,c3)+
     & c(i,1,0,2)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,2)*ui(i1+2,i2,i3+2,c3)+
     & c(i,3,0,2)*ui(i1+3,i2,i3+2,c3)+c(i,4,0,2)*ui(i1+4,i2,i3+2,c3)+
     & c(i,5,0,2)*ui(i1+5,i2,i3+2,c3)+c(i,6,0,2)*ui(i1+6,i2,i3+2,c3)+
     & c(i,7,0,2)*ui(i1+7,i2,i3+2,c3)+c(i,8,0,2)*ui(i1+8,i2,i3+2,c3)+
     & c(i,0,1,2)*ui(i1,i2+1,i3+2,c3)+c(i,1,1,2)*ui(i1+1,i2+1,i3+2,c3)
     & +c(i,2,1,2)*ui(i1+2,i2+1,i3+2,c3)+c(i,3,1,2)*ui(i1+3,i2+1,i3+2,
     & c3)+c(i,4,1,2)*ui(i1+4,i2+1,i3+2,c3)+c(i,5,1,2)*ui(i1+5,i2+1,
     & i3+2,c3)+c(i,6,1,2)*ui(i1+6,i2+1,i3+2,c3)+c(i,7,1,2)*ui(i1+7,
     & i2+1,i3+2,c3)+c(i,8,1,2)*ui(i1+8,i2+1,i3+2,c3)+c(i,0,2,2)*ui(
     & i1,i2+2,i3+2,c3)+c(i,1,2,2)*ui(i1+1,i2+2,i3+2,c3)+c(i,2,2,2)*
     & ui(i1+2,i2+2,i3+2,c3)+c(i,3,2,2)*ui(i1+3,i2+2,i3+2,c3)
               r(i) = r(i)+c(i,4,2,2)*ui(i1+4,i2+2,i3+2,c3)+c(i,5,2,2)*
     & ui(i1+5,i2+2,i3+2,c3)+c(i,6,2,2)*ui(i1+6,i2+2,i3+2,c3)+c(i,7,2,
     & 2)*ui(i1+7,i2+2,i3+2,c3)+c(i,8,2,2)*ui(i1+8,i2+2,i3+2,c3)+c(i,
     & 0,3,2)*ui(i1,i2+3,i3+2,c3)+c(i,1,3,2)*ui(i1+1,i2+3,i3+2,c3)+c(
     & i,2,3,2)*ui(i1+2,i2+3,i3+2,c3)+c(i,3,3,2)*ui(i1+3,i2+3,i3+2,c3)
     & +c(i,4,3,2)*ui(i1+4,i2+3,i3+2,c3)+c(i,5,3,2)*ui(i1+5,i2+3,i3+2,
     & c3)+c(i,6,3,2)*ui(i1+6,i2+3,i3+2,c3)+c(i,7,3,2)*ui(i1+7,i2+3,
     & i3+2,c3)+c(i,8,3,2)*ui(i1+8,i2+3,i3+2,c3)+c(i,0,4,2)*ui(i1,i2+
     & 4,i3+2,c3)+c(i,1,4,2)*ui(i1+1,i2+4,i3+2,c3)+c(i,2,4,2)*ui(i1+2,
     & i2+4,i3+2,c3)+c(i,3,4,2)*ui(i1+3,i2+4,i3+2,c3)+c(i,4,4,2)*ui(
     & i1+4,i2+4,i3+2,c3)+c(i,5,4,2)*ui(i1+5,i2+4,i3+2,c3)+c(i,6,4,2)*
     & ui(i1+6,i2+4,i3+2,c3)+c(i,7,4,2)*ui(i1+7,i2+4,i3+2,c3)+c(i,8,4,
     & 2)*ui(i1+8,i2+4,i3+2,c3)+c(i,0,5,2)*ui(i1,i2+5,i3+2,c3)+c(i,1,
     & 5,2)*ui(i1+1,i2+5,i3+2,c3)+c(i,2,5,2)*ui(i1+2,i2+5,i3+2,c3)+c(
     & i,3,5,2)*ui(i1+3,i2+5,i3+2,c3)+c(i,4,5,2)*ui(i1+4,i2+5,i3+2,c3)
     & +c(i,5,5,2)*ui(i1+5,i2+5,i3+2,c3)+c(i,6,5,2)*ui(i1+6,i2+5,i3+2,
     & c3)+c(i,7,5,2)*ui(i1+7,i2+5,i3+2,c3)+c(i,8,5,2)*ui(i1+8,i2+5,
     & i3+2,c3)+c(i,0,6,2)*ui(i1,i2+6,i3+2,c3)+c(i,1,6,2)*ui(i1+1,i2+
     & 6,i3+2,c3)+c(i,2,6,2)*ui(i1+2,i2+6,i3+2,c3)+c(i,3,6,2)*ui(i1+3,
     & i2+6,i3+2,c3)+c(i,4,6,2)*ui(i1+4,i2+6,i3+2,c3)+c(i,5,6,2)*ui(
     & i1+5,i2+6,i3+2,c3)+c(i,6,6,2)*ui(i1+6,i2+6,i3+2,c3)+c(i,7,6,2)*
     & ui(i1+7,i2+6,i3+2,c3)+c(i,8,6,2)*ui(i1+8,i2+6,i3+2,c3)+c(i,0,7,
     & 2)*ui(i1,i2+7,i3+2,c3)+c(i,1,7,2)*ui(i1+1,i2+7,i3+2,c3)+c(i,2,
     & 7,2)*ui(i1+2,i2+7,i3+2,c3)+c(i,3,7,2)*ui(i1+3,i2+7,i3+2,c3)+c(
     & i,4,7,2)*ui(i1+4,i2+7,i3+2,c3)+c(i,5,7,2)*ui(i1+5,i2+7,i3+2,c3)
     & +c(i,6,7,2)*ui(i1+6,i2+7,i3+2,c3)+c(i,7,7,2)*ui(i1+7,i2+7,i3+2,
     & c3)+c(i,8,7,2)*ui(i1+8,i2+7,i3+2,c3)+c(i,0,8,2)*ui(i1,i2+8,i3+
     & 2,c3)+c(i,1,8,2)*ui(i1+1,i2+8,i3+2,c3)+c(i,2,8,2)*ui(i1+2,i2+8,
     & i3+2,c3)+c(i,3,8,2)*ui(i1+3,i2+8,i3+2,c3)+c(i,4,8,2)*ui(i1+4,
     & i2+8,i3+2,c3)+c(i,5,8,2)*ui(i1+5,i2+8,i3+2,c3)+c(i,6,8,2)*ui(
     & i1+6,i2+8,i3+2,c3)+c(i,7,8,2)*ui(i1+7,i2+8,i3+2,c3)+c(i,8,8,2)*
     & ui(i1+8,i2+8,i3+2,c3)+c(i,0,0,3)*ui(i1,i2,i3+3,c3)+c(i,1,0,3)*
     & ui(i1+1,i2,i3+3,c3)+c(i,2,0,3)*ui(i1+2,i2,i3+3,c3)+c(i,3,0,3)*
     & ui(i1+3,i2,i3+3,c3)+c(i,4,0,3)*ui(i1+4,i2,i3+3,c3)+c(i,5,0,3)*
     & ui(i1+5,i2,i3+3,c3)+c(i,6,0,3)*ui(i1+6,i2,i3+3,c3)+c(i,7,0,3)*
     & ui(i1+7,i2,i3+3,c3)+c(i,8,0,3)*ui(i1+8,i2,i3+3,c3)+c(i,0,1,3)*
     & ui(i1,i2+1,i3+3,c3)+c(i,1,1,3)*ui(i1+1,i2+1,i3+3,c3)+c(i,2,1,3)
     & *ui(i1+2,i2+1,i3+3,c3)+c(i,3,1,3)*ui(i1+3,i2+1,i3+3,c3)+c(i,4,
     & 1,3)*ui(i1+4,i2+1,i3+3,c3)+c(i,5,1,3)*ui(i1+5,i2+1,i3+3,c3)+c(
     & i,6,1,3)*ui(i1+6,i2+1,i3+3,c3)+c(i,7,1,3)*ui(i1+7,i2+1,i3+3,c3)
     & +c(i,8,1,3)*ui(i1+8,i2+1,i3+3,c3)+c(i,0,2,3)*ui(i1,i2+2,i3+3,
     & c3)+c(i,1,2,3)*ui(i1+1,i2+2,i3+3,c3)+c(i,2,2,3)*ui(i1+2,i2+2,
     & i3+3,c3)+c(i,3,2,3)*ui(i1+3,i2+2,i3+3,c3)+c(i,4,2,3)*ui(i1+4,
     & i2+2,i3+3,c3)+c(i,5,2,3)*ui(i1+5,i2+2,i3+3,c3)+c(i,6,2,3)*ui(
     & i1+6,i2+2,i3+3,c3)+c(i,7,2,3)*ui(i1+7,i2+2,i3+3,c3)+c(i,8,2,3)*
     & ui(i1+8,i2+2,i3+3,c3)+c(i,0,3,3)*ui(i1,i2+3,i3+3,c3)+c(i,1,3,3)
     & *ui(i1+1,i2+3,i3+3,c3)+c(i,2,3,3)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,
     & 3,3)*ui(i1+3,i2+3,i3+3,c3)+c(i,4,3,3)*ui(i1+4,i2+3,i3+3,c3)+c(
     & i,5,3,3)*ui(i1+5,i2+3,i3+3,c3)+c(i,6,3,3)*ui(i1+6,i2+3,i3+3,c3)
     & +c(i,7,3,3)*ui(i1+7,i2+3,i3+3,c3)+c(i,8,3,3)*ui(i1+8,i2+3,i3+3,
     & c3)+c(i,0,4,3)*ui(i1,i2+4,i3+3,c3)+c(i,1,4,3)*ui(i1+1,i2+4,i3+
     & 3,c3)+c(i,2,4,3)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,4,3)*ui(i1+3,i2+4,
     & i3+3,c3)+c(i,4,4,3)*ui(i1+4,i2+4,i3+3,c3)+c(i,5,4,3)*ui(i1+5,
     & i2+4,i3+3,c3)+c(i,6,4,3)*ui(i1+6,i2+4,i3+3,c3)+c(i,7,4,3)*ui(
     & i1+7,i2+4,i3+3,c3)+c(i,8,4,3)*ui(i1+8,i2+4,i3+3,c3)+c(i,0,5,3)*
     & ui(i1,i2+5,i3+3,c3)+c(i,1,5,3)*ui(i1+1,i2+5,i3+3,c3)+c(i,2,5,3)
     & *ui(i1+2,i2+5,i3+3,c3)+c(i,3,5,3)*ui(i1+3,i2+5,i3+3,c3)+c(i,4,
     & 5,3)*ui(i1+4,i2+5,i3+3,c3)+c(i,5,5,3)*ui(i1+5,i2+5,i3+3,c3)+c(
     & i,6,5,3)*ui(i1+6,i2+5,i3+3,c3)+c(i,7,5,3)*ui(i1+7,i2+5,i3+3,c3)
     & +c(i,8,5,3)*ui(i1+8,i2+5,i3+3,c3)+c(i,0,6,3)*ui(i1,i2+6,i3+3,
     & c3)+c(i,1,6,3)*ui(i1+1,i2+6,i3+3,c3)+c(i,2,6,3)*ui(i1+2,i2+6,
     & i3+3,c3)+c(i,3,6,3)*ui(i1+3,i2+6,i3+3,c3)+c(i,4,6,3)*ui(i1+4,
     & i2+6,i3+3,c3)+c(i,5,6,3)*ui(i1+5,i2+6,i3+3,c3)+c(i,6,6,3)*ui(
     & i1+6,i2+6,i3+3,c3)+c(i,7,6,3)*ui(i1+7,i2+6,i3+3,c3)+c(i,8,6,3)*
     & ui(i1+8,i2+6,i3+3,c3)+c(i,0,7,3)*ui(i1,i2+7,i3+3,c3)+c(i,1,7,3)
     & *ui(i1+1,i2+7,i3+3,c3)+c(i,2,7,3)*ui(i1+2,i2+7,i3+3,c3)+c(i,3,
     & 7,3)*ui(i1+3,i2+7,i3+3,c3)+c(i,4,7,3)*ui(i1+4,i2+7,i3+3,c3)+c(
     & i,5,7,3)*ui(i1+5,i2+7,i3+3,c3)+c(i,6,7,3)*ui(i1+6,i2+7,i3+3,c3)
     & +c(i,7,7,3)*ui(i1+7,i2+7,i3+3,c3)+c(i,8,7,3)*ui(i1+8,i2+7,i3+3,
     & c3)+c(i,0,8,3)*ui(i1,i2+8,i3+3,c3)+c(i,1,8,3)*ui(i1+1,i2+8,i3+
     & 3,c3)+c(i,2,8,3)*ui(i1+2,i2+8,i3+3,c3)+c(i,3,8,3)*ui(i1+3,i2+8,
     & i3+3,c3)+c(i,4,8,3)*ui(i1+4,i2+8,i3+3,c3)+c(i,5,8,3)*ui(i1+5,
     & i2+8,i3+3,c3)+c(i,6,8,3)*ui(i1+6,i2+8,i3+3,c3)+c(i,7,8,3)*ui(
     & i1+7,i2+8,i3+3,c3)+c(i,8,8,3)*ui(i1+8,i2+8,i3+3,c3)+c(i,0,0,4)*
     & ui(i1,i2,i3+4,c3)+c(i,1,0,4)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,4)*ui(
     & i1+2,i2,i3+4,c3)+c(i,3,0,4)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,4)*ui(
     & i1+4,i2,i3+4,c3)+c(i,5,0,4)*ui(i1+5,i2,i3+4,c3)+c(i,6,0,4)*ui(
     & i1+6,i2,i3+4,c3)+c(i,7,0,4)*ui(i1+7,i2,i3+4,c3)+c(i,8,0,4)*ui(
     & i1+8,i2,i3+4,c3)+c(i,0,1,4)*ui(i1,i2+1,i3+4,c3)+c(i,1,1,4)*ui(
     & i1+1,i2+1,i3+4,c3)+c(i,2,1,4)*ui(i1+2,i2+1,i3+4,c3)+c(i,3,1,4)*
     & ui(i1+3,i2+1,i3+4,c3)+c(i,4,1,4)*ui(i1+4,i2+1,i3+4,c3)+c(i,5,1,
     & 4)*ui(i1+5,i2+1,i3+4,c3)+c(i,6,1,4)*ui(i1+6,i2+1,i3+4,c3)+c(i,
     & 7,1,4)*ui(i1+7,i2+1,i3+4,c3)+c(i,8,1,4)*ui(i1+8,i2+1,i3+4,c3)+
     & c(i,0,2,4)*ui(i1,i2+2,i3+4,c3)+c(i,1,2,4)*ui(i1+1,i2+2,i3+4,c3)
     & +c(i,2,2,4)*ui(i1+2,i2+2,i3+4,c3)+c(i,3,2,4)*ui(i1+3,i2+2,i3+4,
     & c3)+c(i,4,2,4)*ui(i1+4,i2+2,i3+4,c3)+c(i,5,2,4)*ui(i1+5,i2+2,
     & i3+4,c3)+c(i,6,2,4)*ui(i1+6,i2+2,i3+4,c3)+c(i,7,2,4)*ui(i1+7,
     & i2+2,i3+4,c3)+c(i,8,2,4)*ui(i1+8,i2+2,i3+4,c3)+c(i,0,3,4)*ui(
     & i1,i2+3,i3+4,c3)+c(i,1,3,4)*ui(i1+1,i2+3,i3+4,c3)+c(i,2,3,4)*
     & ui(i1+2,i2+3,i3+4,c3)+c(i,3,3,4)*ui(i1+3,i2+3,i3+4,c3)+c(i,4,3,
     & 4)*ui(i1+4,i2+3,i3+4,c3)+c(i,5,3,4)*ui(i1+5,i2+3,i3+4,c3)+c(i,
     & 6,3,4)*ui(i1+6,i2+3,i3+4,c3)+c(i,7,3,4)*ui(i1+7,i2+3,i3+4,c3)+
     & c(i,8,3,4)*ui(i1+8,i2+3,i3+4,c3)+c(i,0,4,4)*ui(i1,i2+4,i3+4,c3)
     & +c(i,1,4,4)*ui(i1+1,i2+4,i3+4,c3)+c(i,2,4,4)*ui(i1+2,i2+4,i3+4,
     & c3)+c(i,3,4,4)*ui(i1+3,i2+4,i3+4,c3)+c(i,4,4,4)*ui(i1+4,i2+4,
     & i3+4,c3)+c(i,5,4,4)*ui(i1+5,i2+4,i3+4,c3)+c(i,6,4,4)*ui(i1+6,
     & i2+4,i3+4,c3)+c(i,7,4,4)*ui(i1+7,i2+4,i3+4,c3)
               r(i) = r(i)+c(i,8,4,4)*ui(i1+8,i2+4,i3+4,c3)+c(i,0,5,4)*
     & ui(i1,i2+5,i3+4,c3)+c(i,1,5,4)*ui(i1+1,i2+5,i3+4,c3)+c(i,2,5,4)
     & *ui(i1+2,i2+5,i3+4,c3)+c(i,3,5,4)*ui(i1+3,i2+5,i3+4,c3)+c(i,4,
     & 5,4)*ui(i1+4,i2+5,i3+4,c3)+c(i,5,5,4)*ui(i1+5,i2+5,i3+4,c3)+c(
     & i,6,5,4)*ui(i1+6,i2+5,i3+4,c3)+c(i,7,5,4)*ui(i1+7,i2+5,i3+4,c3)
     & +c(i,8,5,4)*ui(i1+8,i2+5,i3+4,c3)+c(i,0,6,4)*ui(i1,i2+6,i3+4,
     & c3)+c(i,1,6,4)*ui(i1+1,i2+6,i3+4,c3)+c(i,2,6,4)*ui(i1+2,i2+6,
     & i3+4,c3)+c(i,3,6,4)*ui(i1+3,i2+6,i3+4,c3)+c(i,4,6,4)*ui(i1+4,
     & i2+6,i3+4,c3)+c(i,5,6,4)*ui(i1+5,i2+6,i3+4,c3)+c(i,6,6,4)*ui(
     & i1+6,i2+6,i3+4,c3)+c(i,7,6,4)*ui(i1+7,i2+6,i3+4,c3)+c(i,8,6,4)*
     & ui(i1+8,i2+6,i3+4,c3)+c(i,0,7,4)*ui(i1,i2+7,i3+4,c3)+c(i,1,7,4)
     & *ui(i1+1,i2+7,i3+4,c3)+c(i,2,7,4)*ui(i1+2,i2+7,i3+4,c3)+c(i,3,
     & 7,4)*ui(i1+3,i2+7,i3+4,c3)+c(i,4,7,4)*ui(i1+4,i2+7,i3+4,c3)+c(
     & i,5,7,4)*ui(i1+5,i2+7,i3+4,c3)+c(i,6,7,4)*ui(i1+6,i2+7,i3+4,c3)
     & +c(i,7,7,4)*ui(i1+7,i2+7,i3+4,c3)+c(i,8,7,4)*ui(i1+8,i2+7,i3+4,
     & c3)+c(i,0,8,4)*ui(i1,i2+8,i3+4,c3)+c(i,1,8,4)*ui(i1+1,i2+8,i3+
     & 4,c3)+c(i,2,8,4)*ui(i1+2,i2+8,i3+4,c3)+c(i,3,8,4)*ui(i1+3,i2+8,
     & i3+4,c3)+c(i,4,8,4)*ui(i1+4,i2+8,i3+4,c3)+c(i,5,8,4)*ui(i1+5,
     & i2+8,i3+4,c3)+c(i,6,8,4)*ui(i1+6,i2+8,i3+4,c3)+c(i,7,8,4)*ui(
     & i1+7,i2+8,i3+4,c3)+c(i,8,8,4)*ui(i1+8,i2+8,i3+4,c3)+c(i,0,0,5)*
     & ui(i1,i2,i3+5,c3)+c(i,1,0,5)*ui(i1+1,i2,i3+5,c3)+c(i,2,0,5)*ui(
     & i1+2,i2,i3+5,c3)+c(i,3,0,5)*ui(i1+3,i2,i3+5,c3)+c(i,4,0,5)*ui(
     & i1+4,i2,i3+5,c3)+c(i,5,0,5)*ui(i1+5,i2,i3+5,c3)+c(i,6,0,5)*ui(
     & i1+6,i2,i3+5,c3)+c(i,7,0,5)*ui(i1+7,i2,i3+5,c3)+c(i,8,0,5)*ui(
     & i1+8,i2,i3+5,c3)+c(i,0,1,5)*ui(i1,i2+1,i3+5,c3)+c(i,1,1,5)*ui(
     & i1+1,i2+1,i3+5,c3)+c(i,2,1,5)*ui(i1+2,i2+1,i3+5,c3)+c(i,3,1,5)*
     & ui(i1+3,i2+1,i3+5,c3)+c(i,4,1,5)*ui(i1+4,i2+1,i3+5,c3)+c(i,5,1,
     & 5)*ui(i1+5,i2+1,i3+5,c3)+c(i,6,1,5)*ui(i1+6,i2+1,i3+5,c3)+c(i,
     & 7,1,5)*ui(i1+7,i2+1,i3+5,c3)+c(i,8,1,5)*ui(i1+8,i2+1,i3+5,c3)+
     & c(i,0,2,5)*ui(i1,i2+2,i3+5,c3)+c(i,1,2,5)*ui(i1+1,i2+2,i3+5,c3)
     & +c(i,2,2,5)*ui(i1+2,i2+2,i3+5,c3)+c(i,3,2,5)*ui(i1+3,i2+2,i3+5,
     & c3)+c(i,4,2,5)*ui(i1+4,i2+2,i3+5,c3)+c(i,5,2,5)*ui(i1+5,i2+2,
     & i3+5,c3)+c(i,6,2,5)*ui(i1+6,i2+2,i3+5,c3)+c(i,7,2,5)*ui(i1+7,
     & i2+2,i3+5,c3)+c(i,8,2,5)*ui(i1+8,i2+2,i3+5,c3)+c(i,0,3,5)*ui(
     & i1,i2+3,i3+5,c3)+c(i,1,3,5)*ui(i1+1,i2+3,i3+5,c3)+c(i,2,3,5)*
     & ui(i1+2,i2+3,i3+5,c3)+c(i,3,3,5)*ui(i1+3,i2+3,i3+5,c3)+c(i,4,3,
     & 5)*ui(i1+4,i2+3,i3+5,c3)+c(i,5,3,5)*ui(i1+5,i2+3,i3+5,c3)+c(i,
     & 6,3,5)*ui(i1+6,i2+3,i3+5,c3)+c(i,7,3,5)*ui(i1+7,i2+3,i3+5,c3)+
     & c(i,8,3,5)*ui(i1+8,i2+3,i3+5,c3)+c(i,0,4,5)*ui(i1,i2+4,i3+5,c3)
     & +c(i,1,4,5)*ui(i1+1,i2+4,i3+5,c3)+c(i,2,4,5)*ui(i1+2,i2+4,i3+5,
     & c3)+c(i,3,4,5)*ui(i1+3,i2+4,i3+5,c3)+c(i,4,4,5)*ui(i1+4,i2+4,
     & i3+5,c3)+c(i,5,4,5)*ui(i1+5,i2+4,i3+5,c3)+c(i,6,4,5)*ui(i1+6,
     & i2+4,i3+5,c3)+c(i,7,4,5)*ui(i1+7,i2+4,i3+5,c3)+c(i,8,4,5)*ui(
     & i1+8,i2+4,i3+5,c3)+c(i,0,5,5)*ui(i1,i2+5,i3+5,c3)+c(i,1,5,5)*
     & ui(i1+1,i2+5,i3+5,c3)+c(i,2,5,5)*ui(i1+2,i2+5,i3+5,c3)+c(i,3,5,
     & 5)*ui(i1+3,i2+5,i3+5,c3)+c(i,4,5,5)*ui(i1+4,i2+5,i3+5,c3)+c(i,
     & 5,5,5)*ui(i1+5,i2+5,i3+5,c3)+c(i,6,5,5)*ui(i1+6,i2+5,i3+5,c3)+
     & c(i,7,5,5)*ui(i1+7,i2+5,i3+5,c3)+c(i,8,5,5)*ui(i1+8,i2+5,i3+5,
     & c3)+c(i,0,6,5)*ui(i1,i2+6,i3+5,c3)+c(i,1,6,5)*ui(i1+1,i2+6,i3+
     & 5,c3)+c(i,2,6,5)*ui(i1+2,i2+6,i3+5,c3)+c(i,3,6,5)*ui(i1+3,i2+6,
     & i3+5,c3)+c(i,4,6,5)*ui(i1+4,i2+6,i3+5,c3)+c(i,5,6,5)*ui(i1+5,
     & i2+6,i3+5,c3)+c(i,6,6,5)*ui(i1+6,i2+6,i3+5,c3)+c(i,7,6,5)*ui(
     & i1+7,i2+6,i3+5,c3)+c(i,8,6,5)*ui(i1+8,i2+6,i3+5,c3)+c(i,0,7,5)*
     & ui(i1,i2+7,i3+5,c3)+c(i,1,7,5)*ui(i1+1,i2+7,i3+5,c3)+c(i,2,7,5)
     & *ui(i1+2,i2+7,i3+5,c3)+c(i,3,7,5)*ui(i1+3,i2+7,i3+5,c3)+c(i,4,
     & 7,5)*ui(i1+4,i2+7,i3+5,c3)+c(i,5,7,5)*ui(i1+5,i2+7,i3+5,c3)+c(
     & i,6,7,5)*ui(i1+6,i2+7,i3+5,c3)+c(i,7,7,5)*ui(i1+7,i2+7,i3+5,c3)
     & +c(i,8,7,5)*ui(i1+8,i2+7,i3+5,c3)+c(i,0,8,5)*ui(i1,i2+8,i3+5,
     & c3)+c(i,1,8,5)*ui(i1+1,i2+8,i3+5,c3)+c(i,2,8,5)*ui(i1+2,i2+8,
     & i3+5,c3)+c(i,3,8,5)*ui(i1+3,i2+8,i3+5,c3)+c(i,4,8,5)*ui(i1+4,
     & i2+8,i3+5,c3)+c(i,5,8,5)*ui(i1+5,i2+8,i3+5,c3)+c(i,6,8,5)*ui(
     & i1+6,i2+8,i3+5,c3)+c(i,7,8,5)*ui(i1+7,i2+8,i3+5,c3)+c(i,8,8,5)*
     & ui(i1+8,i2+8,i3+5,c3)+c(i,0,0,6)*ui(i1,i2,i3+6,c3)+c(i,1,0,6)*
     & ui(i1+1,i2,i3+6,c3)+c(i,2,0,6)*ui(i1+2,i2,i3+6,c3)+c(i,3,0,6)*
     & ui(i1+3,i2,i3+6,c3)+c(i,4,0,6)*ui(i1+4,i2,i3+6,c3)+c(i,5,0,6)*
     & ui(i1+5,i2,i3+6,c3)+c(i,6,0,6)*ui(i1+6,i2,i3+6,c3)+c(i,7,0,6)*
     & ui(i1+7,i2,i3+6,c3)+c(i,8,0,6)*ui(i1+8,i2,i3+6,c3)+c(i,0,1,6)*
     & ui(i1,i2+1,i3+6,c3)+c(i,1,1,6)*ui(i1+1,i2+1,i3+6,c3)+c(i,2,1,6)
     & *ui(i1+2,i2+1,i3+6,c3)+c(i,3,1,6)*ui(i1+3,i2+1,i3+6,c3)+c(i,4,
     & 1,6)*ui(i1+4,i2+1,i3+6,c3)+c(i,5,1,6)*ui(i1+5,i2+1,i3+6,c3)+c(
     & i,6,1,6)*ui(i1+6,i2+1,i3+6,c3)+c(i,7,1,6)*ui(i1+7,i2+1,i3+6,c3)
     & +c(i,8,1,6)*ui(i1+8,i2+1,i3+6,c3)+c(i,0,2,6)*ui(i1,i2+2,i3+6,
     & c3)+c(i,1,2,6)*ui(i1+1,i2+2,i3+6,c3)+c(i,2,2,6)*ui(i1+2,i2+2,
     & i3+6,c3)+c(i,3,2,6)*ui(i1+3,i2+2,i3+6,c3)+c(i,4,2,6)*ui(i1+4,
     & i2+2,i3+6,c3)+c(i,5,2,6)*ui(i1+5,i2+2,i3+6,c3)+c(i,6,2,6)*ui(
     & i1+6,i2+2,i3+6,c3)+c(i,7,2,6)*ui(i1+7,i2+2,i3+6,c3)+c(i,8,2,6)*
     & ui(i1+8,i2+2,i3+6,c3)+c(i,0,3,6)*ui(i1,i2+3,i3+6,c3)+c(i,1,3,6)
     & *ui(i1+1,i2+3,i3+6,c3)+c(i,2,3,6)*ui(i1+2,i2+3,i3+6,c3)+c(i,3,
     & 3,6)*ui(i1+3,i2+3,i3+6,c3)+c(i,4,3,6)*ui(i1+4,i2+3,i3+6,c3)+c(
     & i,5,3,6)*ui(i1+5,i2+3,i3+6,c3)+c(i,6,3,6)*ui(i1+6,i2+3,i3+6,c3)
     & +c(i,7,3,6)*ui(i1+7,i2+3,i3+6,c3)+c(i,8,3,6)*ui(i1+8,i2+3,i3+6,
     & c3)+c(i,0,4,6)*ui(i1,i2+4,i3+6,c3)+c(i,1,4,6)*ui(i1+1,i2+4,i3+
     & 6,c3)+c(i,2,4,6)*ui(i1+2,i2+4,i3+6,c3)+c(i,3,4,6)*ui(i1+3,i2+4,
     & i3+6,c3)+c(i,4,4,6)*ui(i1+4,i2+4,i3+6,c3)+c(i,5,4,6)*ui(i1+5,
     & i2+4,i3+6,c3)+c(i,6,4,6)*ui(i1+6,i2+4,i3+6,c3)+c(i,7,4,6)*ui(
     & i1+7,i2+4,i3+6,c3)+c(i,8,4,6)*ui(i1+8,i2+4,i3+6,c3)+c(i,0,5,6)*
     & ui(i1,i2+5,i3+6,c3)+c(i,1,5,6)*ui(i1+1,i2+5,i3+6,c3)+c(i,2,5,6)
     & *ui(i1+2,i2+5,i3+6,c3)+c(i,3,5,6)*ui(i1+3,i2+5,i3+6,c3)+c(i,4,
     & 5,6)*ui(i1+4,i2+5,i3+6,c3)+c(i,5,5,6)*ui(i1+5,i2+5,i3+6,c3)+c(
     & i,6,5,6)*ui(i1+6,i2+5,i3+6,c3)+c(i,7,5,6)*ui(i1+7,i2+5,i3+6,c3)
     & +c(i,8,5,6)*ui(i1+8,i2+5,i3+6,c3)+c(i,0,6,6)*ui(i1,i2+6,i3+6,
     & c3)+c(i,1,6,6)*ui(i1+1,i2+6,i3+6,c3)+c(i,2,6,6)*ui(i1+2,i2+6,
     & i3+6,c3)+c(i,3,6,6)*ui(i1+3,i2+6,i3+6,c3)+c(i,4,6,6)*ui(i1+4,
     & i2+6,i3+6,c3)+c(i,5,6,6)*ui(i1+5,i2+6,i3+6,c3)+c(i,6,6,6)*ui(
     & i1+6,i2+6,i3+6,c3)+c(i,7,6,6)*ui(i1+7,i2+6,i3+6,c3)+c(i,8,6,6)*
     & ui(i1+8,i2+6,i3+6,c3)+c(i,0,7,6)*ui(i1,i2+7,i3+6,c3)+c(i,1,7,6)
     & *ui(i1+1,i2+7,i3+6,c3)+c(i,2,7,6)*ui(i1+2,i2+7,i3+6,c3)
               r(i) = r(i)+c(i,3,7,6)*ui(i1+3,i2+7,i3+6,c3)+c(i,4,7,6)*
     & ui(i1+4,i2+7,i3+6,c3)+c(i,5,7,6)*ui(i1+5,i2+7,i3+6,c3)+c(i,6,7,
     & 6)*ui(i1+6,i2+7,i3+6,c3)+c(i,7,7,6)*ui(i1+7,i2+7,i3+6,c3)+c(i,
     & 8,7,6)*ui(i1+8,i2+7,i3+6,c3)+c(i,0,8,6)*ui(i1,i2+8,i3+6,c3)+c(
     & i,1,8,6)*ui(i1+1,i2+8,i3+6,c3)+c(i,2,8,6)*ui(i1+2,i2+8,i3+6,c3)
     & +c(i,3,8,6)*ui(i1+3,i2+8,i3+6,c3)+c(i,4,8,6)*ui(i1+4,i2+8,i3+6,
     & c3)+c(i,5,8,6)*ui(i1+5,i2+8,i3+6,c3)+c(i,6,8,6)*ui(i1+6,i2+8,
     & i3+6,c3)+c(i,7,8,6)*ui(i1+7,i2+8,i3+6,c3)+c(i,8,8,6)*ui(i1+8,
     & i2+8,i3+6,c3)+c(i,0,0,7)*ui(i1,i2,i3+7,c3)+c(i,1,0,7)*ui(i1+1,
     & i2,i3+7,c3)+c(i,2,0,7)*ui(i1+2,i2,i3+7,c3)+c(i,3,0,7)*ui(i1+3,
     & i2,i3+7,c3)+c(i,4,0,7)*ui(i1+4,i2,i3+7,c3)+c(i,5,0,7)*ui(i1+5,
     & i2,i3+7,c3)+c(i,6,0,7)*ui(i1+6,i2,i3+7,c3)+c(i,7,0,7)*ui(i1+7,
     & i2,i3+7,c3)+c(i,8,0,7)*ui(i1+8,i2,i3+7,c3)+c(i,0,1,7)*ui(i1,i2+
     & 1,i3+7,c3)+c(i,1,1,7)*ui(i1+1,i2+1,i3+7,c3)+c(i,2,1,7)*ui(i1+2,
     & i2+1,i3+7,c3)+c(i,3,1,7)*ui(i1+3,i2+1,i3+7,c3)+c(i,4,1,7)*ui(
     & i1+4,i2+1,i3+7,c3)+c(i,5,1,7)*ui(i1+5,i2+1,i3+7,c3)+c(i,6,1,7)*
     & ui(i1+6,i2+1,i3+7,c3)+c(i,7,1,7)*ui(i1+7,i2+1,i3+7,c3)+c(i,8,1,
     & 7)*ui(i1+8,i2+1,i3+7,c3)+c(i,0,2,7)*ui(i1,i2+2,i3+7,c3)+c(i,1,
     & 2,7)*ui(i1+1,i2+2,i3+7,c3)+c(i,2,2,7)*ui(i1+2,i2+2,i3+7,c3)+c(
     & i,3,2,7)*ui(i1+3,i2+2,i3+7,c3)+c(i,4,2,7)*ui(i1+4,i2+2,i3+7,c3)
     & +c(i,5,2,7)*ui(i1+5,i2+2,i3+7,c3)+c(i,6,2,7)*ui(i1+6,i2+2,i3+7,
     & c3)+c(i,7,2,7)*ui(i1+7,i2+2,i3+7,c3)+c(i,8,2,7)*ui(i1+8,i2+2,
     & i3+7,c3)+c(i,0,3,7)*ui(i1,i2+3,i3+7,c3)+c(i,1,3,7)*ui(i1+1,i2+
     & 3,i3+7,c3)+c(i,2,3,7)*ui(i1+2,i2+3,i3+7,c3)+c(i,3,3,7)*ui(i1+3,
     & i2+3,i3+7,c3)+c(i,4,3,7)*ui(i1+4,i2+3,i3+7,c3)+c(i,5,3,7)*ui(
     & i1+5,i2+3,i3+7,c3)+c(i,6,3,7)*ui(i1+6,i2+3,i3+7,c3)+c(i,7,3,7)*
     & ui(i1+7,i2+3,i3+7,c3)+c(i,8,3,7)*ui(i1+8,i2+3,i3+7,c3)+c(i,0,4,
     & 7)*ui(i1,i2+4,i3+7,c3)+c(i,1,4,7)*ui(i1+1,i2+4,i3+7,c3)+c(i,2,
     & 4,7)*ui(i1+2,i2+4,i3+7,c3)+c(i,3,4,7)*ui(i1+3,i2+4,i3+7,c3)+c(
     & i,4,4,7)*ui(i1+4,i2+4,i3+7,c3)+c(i,5,4,7)*ui(i1+5,i2+4,i3+7,c3)
     & +c(i,6,4,7)*ui(i1+6,i2+4,i3+7,c3)+c(i,7,4,7)*ui(i1+7,i2+4,i3+7,
     & c3)+c(i,8,4,7)*ui(i1+8,i2+4,i3+7,c3)+c(i,0,5,7)*ui(i1,i2+5,i3+
     & 7,c3)+c(i,1,5,7)*ui(i1+1,i2+5,i3+7,c3)+c(i,2,5,7)*ui(i1+2,i2+5,
     & i3+7,c3)+c(i,3,5,7)*ui(i1+3,i2+5,i3+7,c3)+c(i,4,5,7)*ui(i1+4,
     & i2+5,i3+7,c3)+c(i,5,5,7)*ui(i1+5,i2+5,i3+7,c3)+c(i,6,5,7)*ui(
     & i1+6,i2+5,i3+7,c3)+c(i,7,5,7)*ui(i1+7,i2+5,i3+7,c3)+c(i,8,5,7)*
     & ui(i1+8,i2+5,i3+7,c3)+c(i,0,6,7)*ui(i1,i2+6,i3+7,c3)+c(i,1,6,7)
     & *ui(i1+1,i2+6,i3+7,c3)+c(i,2,6,7)*ui(i1+2,i2+6,i3+7,c3)+c(i,3,
     & 6,7)*ui(i1+3,i2+6,i3+7,c3)+c(i,4,6,7)*ui(i1+4,i2+6,i3+7,c3)+c(
     & i,5,6,7)*ui(i1+5,i2+6,i3+7,c3)+c(i,6,6,7)*ui(i1+6,i2+6,i3+7,c3)
     & +c(i,7,6,7)*ui(i1+7,i2+6,i3+7,c3)+c(i,8,6,7)*ui(i1+8,i2+6,i3+7,
     & c3)+c(i,0,7,7)*ui(i1,i2+7,i3+7,c3)+c(i,1,7,7)*ui(i1+1,i2+7,i3+
     & 7,c3)+c(i,2,7,7)*ui(i1+2,i2+7,i3+7,c3)+c(i,3,7,7)*ui(i1+3,i2+7,
     & i3+7,c3)+c(i,4,7,7)*ui(i1+4,i2+7,i3+7,c3)+c(i,5,7,7)*ui(i1+5,
     & i2+7,i3+7,c3)+c(i,6,7,7)*ui(i1+6,i2+7,i3+7,c3)+c(i,7,7,7)*ui(
     & i1+7,i2+7,i3+7,c3)+c(i,8,7,7)*ui(i1+8,i2+7,i3+7,c3)+c(i,0,8,7)*
     & ui(i1,i2+8,i3+7,c3)+c(i,1,8,7)*ui(i1+1,i2+8,i3+7,c3)+c(i,2,8,7)
     & *ui(i1+2,i2+8,i3+7,c3)+c(i,3,8,7)*ui(i1+3,i2+8,i3+7,c3)+c(i,4,
     & 8,7)*ui(i1+4,i2+8,i3+7,c3)+c(i,5,8,7)*ui(i1+5,i2+8,i3+7,c3)+c(
     & i,6,8,7)*ui(i1+6,i2+8,i3+7,c3)+c(i,7,8,7)*ui(i1+7,i2+8,i3+7,c3)
     & +c(i,8,8,7)*ui(i1+8,i2+8,i3+7,c3)+c(i,0,0,8)*ui(i1,i2,i3+8,c3)+
     & c(i,1,0,8)*ui(i1+1,i2,i3+8,c3)+c(i,2,0,8)*ui(i1+2,i2,i3+8,c3)+
     & c(i,3,0,8)*ui(i1+3,i2,i3+8,c3)+c(i,4,0,8)*ui(i1+4,i2,i3+8,c3)+
     & c(i,5,0,8)*ui(i1+5,i2,i3+8,c3)+c(i,6,0,8)*ui(i1+6,i2,i3+8,c3)+
     & c(i,7,0,8)*ui(i1+7,i2,i3+8,c3)+c(i,8,0,8)*ui(i1+8,i2,i3+8,c3)+
     & c(i,0,1,8)*ui(i1,i2+1,i3+8,c3)+c(i,1,1,8)*ui(i1+1,i2+1,i3+8,c3)
     & +c(i,2,1,8)*ui(i1+2,i2+1,i3+8,c3)+c(i,3,1,8)*ui(i1+3,i2+1,i3+8,
     & c3)+c(i,4,1,8)*ui(i1+4,i2+1,i3+8,c3)+c(i,5,1,8)*ui(i1+5,i2+1,
     & i3+8,c3)+c(i,6,1,8)*ui(i1+6,i2+1,i3+8,c3)+c(i,7,1,8)*ui(i1+7,
     & i2+1,i3+8,c3)+c(i,8,1,8)*ui(i1+8,i2+1,i3+8,c3)+c(i,0,2,8)*ui(
     & i1,i2+2,i3+8,c3)+c(i,1,2,8)*ui(i1+1,i2+2,i3+8,c3)+c(i,2,2,8)*
     & ui(i1+2,i2+2,i3+8,c3)+c(i,3,2,8)*ui(i1+3,i2+2,i3+8,c3)+c(i,4,2,
     & 8)*ui(i1+4,i2+2,i3+8,c3)+c(i,5,2,8)*ui(i1+5,i2+2,i3+8,c3)+c(i,
     & 6,2,8)*ui(i1+6,i2+2,i3+8,c3)+c(i,7,2,8)*ui(i1+7,i2+2,i3+8,c3)+
     & c(i,8,2,8)*ui(i1+8,i2+2,i3+8,c3)+c(i,0,3,8)*ui(i1,i2+3,i3+8,c3)
     & +c(i,1,3,8)*ui(i1+1,i2+3,i3+8,c3)+c(i,2,3,8)*ui(i1+2,i2+3,i3+8,
     & c3)+c(i,3,3,8)*ui(i1+3,i2+3,i3+8,c3)+c(i,4,3,8)*ui(i1+4,i2+3,
     & i3+8,c3)+c(i,5,3,8)*ui(i1+5,i2+3,i3+8,c3)+c(i,6,3,8)*ui(i1+6,
     & i2+3,i3+8,c3)+c(i,7,3,8)*ui(i1+7,i2+3,i3+8,c3)+c(i,8,3,8)*ui(
     & i1+8,i2+3,i3+8,c3)+c(i,0,4,8)*ui(i1,i2+4,i3+8,c3)+c(i,1,4,8)*
     & ui(i1+1,i2+4,i3+8,c3)+c(i,2,4,8)*ui(i1+2,i2+4,i3+8,c3)+c(i,3,4,
     & 8)*ui(i1+3,i2+4,i3+8,c3)+c(i,4,4,8)*ui(i1+4,i2+4,i3+8,c3)+c(i,
     & 5,4,8)*ui(i1+5,i2+4,i3+8,c3)+c(i,6,4,8)*ui(i1+6,i2+4,i3+8,c3)+
     & c(i,7,4,8)*ui(i1+7,i2+4,i3+8,c3)+c(i,8,4,8)*ui(i1+8,i2+4,i3+8,
     & c3)+c(i,0,5,8)*ui(i1,i2+5,i3+8,c3)+c(i,1,5,8)*ui(i1+1,i2+5,i3+
     & 8,c3)+c(i,2,5,8)*ui(i1+2,i2+5,i3+8,c3)+c(i,3,5,8)*ui(i1+3,i2+5,
     & i3+8,c3)+c(i,4,5,8)*ui(i1+4,i2+5,i3+8,c3)+c(i,5,5,8)*ui(i1+5,
     & i2+5,i3+8,c3)+c(i,6,5,8)*ui(i1+6,i2+5,i3+8,c3)+c(i,7,5,8)*ui(
     & i1+7,i2+5,i3+8,c3)+c(i,8,5,8)*ui(i1+8,i2+5,i3+8,c3)+c(i,0,6,8)*
     & ui(i1,i2+6,i3+8,c3)+c(i,1,6,8)*ui(i1+1,i2+6,i3+8,c3)+c(i,2,6,8)
     & *ui(i1+2,i2+6,i3+8,c3)+c(i,3,6,8)*ui(i1+3,i2+6,i3+8,c3)+c(i,4,
     & 6,8)*ui(i1+4,i2+6,i3+8,c3)+c(i,5,6,8)*ui(i1+5,i2+6,i3+8,c3)+c(
     & i,6,6,8)*ui(i1+6,i2+6,i3+8,c3)+c(i,7,6,8)*ui(i1+7,i2+6,i3+8,c3)
     & +c(i,8,6,8)*ui(i1+8,i2+6,i3+8,c3)+c(i,0,7,8)*ui(i1,i2+7,i3+8,
     & c3)+c(i,1,7,8)*ui(i1+1,i2+7,i3+8,c3)+c(i,2,7,8)*ui(i1+2,i2+7,
     & i3+8,c3)+c(i,3,7,8)*ui(i1+3,i2+7,i3+8,c3)+c(i,4,7,8)*ui(i1+4,
     & i2+7,i3+8,c3)+c(i,5,7,8)*ui(i1+5,i2+7,i3+8,c3)+c(i,6,7,8)*ui(
     & i1+6,i2+7,i3+8,c3)+c(i,7,7,8)*ui(i1+7,i2+7,i3+8,c3)+c(i,8,7,8)*
     & ui(i1+8,i2+7,i3+8,c3)+c(i,0,8,8)*ui(i1,i2+8,i3+8,c3)+c(i,1,8,8)
     & *ui(i1+1,i2+8,i3+8,c3)+c(i,2,8,8)*ui(i1+2,i2+8,i3+8,c3)+c(i,3,
     & 8,8)*ui(i1+3,i2+8,i3+8,c3)+c(i,4,8,8)*ui(i1+4,i2+8,i3+8,c3)+c(
     & i,5,8,8)*ui(i1+5,i2+8,i3+8,c3)+c(i,6,8,8)*ui(i1+6,i2+8,i3+8,c3)
     & +c(i,7,8,8)*ui(i1+7,i2+8,i3+8,c3)+c(i,8,8,8)*ui(i1+8,i2+8,i3+8,
     & c3)
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           end if
         else
c     general case in 3D
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
! #If "Full" == "TP"
! #If "Full" == "SP"
       else
         write(*,*) 'interpOpt:ERROR; unknown storage option=',
     & storageOption
       end if ! end storage option
       return
       end
! defineInterpOpt(Full)
       subroutine interpOptFull ( nd,ndui1a,ndui1b,ndui2a,ndui2b,
     & ndui3a,ndui3b,ndui4a,ndui4b,ndug1a,ndug1b,ndug2a,ndug2b,ndug3a,
     & ndug3b,ndug4a,ndug4b,ndil,ndip,ndc1,ndc2,ndc3,ipar,ui,ug,c,il,
     & ip,varWidth,width )
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
! #If "Full" == "Full"
        if( storageOption.eq.0 )then
c       ******************************
c       **** full storage option *****
c       ******************************
        if( nd.eq.2 )then
          if( useVariableWidthInterpolation.ne.0 )then
! beginLoops2d()
             do i=nia,nib
             do c3=c3a,c3b
             do c2=c2a,c2b
             ! check for most common widths first
             if( varWidth(i).eq.3 )then
! interp33(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
               ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,0,0)*ui(i1  ,i2  ,c2,
     & c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,
     & c3)+c(i,0,1,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,1,0)*ui(i1+1,i2+1,c2,
     & c3)+c(i,2,1,0)*ui(i1+2,i2+1,c2,c3)+c(i,0,2,0)*ui(i1  ,i2+2,c2,
     & c3)+c(i,1,2,0)*ui(i1+1,i2+2,c2,c3)+c(i,2,2,0)*ui(i1+2,i2+2,c2,
     & c3)
             else if( varWidth(i).eq.2 )then
! interp22(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
               ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,0,0)*ui(i1  ,i2  ,c2,
     & c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,0,1,0)*ui(i1  ,i2+1,c2,
     & c3)+c(i,1,1,0)*ui(i1+1,i2+1,c2,c3)
             else if( varWidth(i).eq.1 )then
! interp11(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
               ug(ip(i,1),ip(i,2),c2,c3) = ui(i1  ,i2  ,c2,c3)
             else if( varWidth(i).eq.5 )then
! interp55(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
               ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,0,0)*ui(i1  ,i2  ,c2,
     & c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,
     & c3)+c(i,3,0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,
     & c3)+c(i,0,1,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,1,0)*ui(i1+1,i2+1,c2,
     & c3)+c(i,2,1,0)*ui(i1+2,i2+1,c2,c3)+c(i,3,1,0)*ui(i1+3,i2+1,c2,
     & c3)+c(i,4,1,0)*ui(i1+4,i2+1,c2,c3)+c(i,0,2,0)*ui(i1  ,i2+2,c2,
     & c3)+c(i,1,2,0)*ui(i1+1,i2+2,c2,c3)+c(i,2,2,0)*ui(i1+2,i2+2,c2,
     & c3)+c(i,3,2,0)*ui(i1+3,i2+2,c2,c3)+c(i,4,2,0)*ui(i1+4,i2+2,c2,
     & c3)+c(i,0,3,0)*ui(i1  ,i2+3,c2,c3)+c(i,1,3,0)*ui(i1+1,i2+3,c2,
     & c3)+c(i,2,3,0)*ui(i1+2,i2+3,c2,c3)+c(i,3,3,0)*ui(i1+3,i2+3,c2,
     & c3)+c(i,4,3,0)*ui(i1+4,i2+3,c2,c3)+c(i,0,4,0)*ui(i1  ,i2+4,c2,
     & c3)+c(i,1,4,0)*ui(i1+1,i2+4,c2,c3)+c(i,2,4,0)*ui(i1+2,i2+4,c2,
     & c3)+c(i,3,4,0)*ui(i1+3,i2+4,c2,c3)+c(i,4,4,0)*ui(i1+4,i2+4,c2,
     & c3)
             else if( varWidth(i).eq.4 )then
! interp44(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
               ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,0,0)*ui(i1  ,i2  ,c2,
     & c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,
     & c3)+c(i,3,0,0)*ui(i1+3,i2  ,c2,c3)+c(i,0,1,0)*ui(i1  ,i2+1,c2,
     & c3)+c(i,1,1,0)*ui(i1+1,i2+1,c2,c3)+c(i,2,1,0)*ui(i1+2,i2+1,c2,
     & c3)+c(i,3,1,0)*ui(i1+3,i2+1,c2,c3)+c(i,0,2,0)*ui(i1  ,i2+2,c2,
     & c3)+c(i,1,2,0)*ui(i1+1,i2+2,c2,c3)+c(i,2,2,0)*ui(i1+2,i2+2,c2,
     & c3)+c(i,3,2,0)*ui(i1+3,i2+2,c2,c3)+c(i,0,3,0)*ui(i1  ,i2+3,c2,
     & c3)+c(i,1,3,0)*ui(i1+1,i2+3,c2,c3)+c(i,2,3,0)*ui(i1+2,i2+3,c2,
     & c3)+c(i,3,3,0)*ui(i1+3,i2+3,c2,c3)
             else if( varWidth(i).eq.7 )then
! interp77(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
               ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,0,0)*ui(i1  ,i2  ,c2,
     & c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,
     & c3)+c(i,3,0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,
     & c3)+c(i,5,0,0)*ui(i1+5,i2  ,c2,c3)+c(i,6,0,0)*ui(i1+6,i2  ,c2,
     & c3)+c(i,0,1,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,1,0)*ui(i1+1,i2+1,c2,
     & c3)+c(i,2,1,0)*ui(i1+2,i2+1,c2,c3)+c(i,3,1,0)*ui(i1+3,i2+1,c2,
     & c3)+c(i,4,1,0)*ui(i1+4,i2+1,c2,c3)+c(i,5,1,0)*ui(i1+5,i2+1,c2,
     & c3)+c(i,6,1,0)*ui(i1+6,i2+1,c2,c3)+c(i,0,2,0)*ui(i1  ,i2+2,c2,
     & c3)+c(i,1,2,0)*ui(i1+1,i2+2,c2,c3)+c(i,2,2,0)*ui(i1+2,i2+2,c2,
     & c3)+c(i,3,2,0)*ui(i1+3,i2+2,c2,c3)+c(i,4,2,0)*ui(i1+4,i2+2,c2,
     & c3)+c(i,5,2,0)*ui(i1+5,i2+2,c2,c3)+c(i,6,2,0)*ui(i1+6,i2+2,c2,
     & c3)+c(i,0,3,0)*ui(i1  ,i2+3,c2,c3)+c(i,1,3,0)*ui(i1+1,i2+3,c2,
     & c3)+c(i,2,3,0)*ui(i1+2,i2+3,c2,c3)+c(i,3,3,0)*ui(i1+3,i2+3,c2,
     & c3)+c(i,4,3,0)*ui(i1+4,i2+3,c2,c3)+c(i,5,3,0)*ui(i1+5,i2+3,c2,
     & c3)+c(i,6,3,0)*ui(i1+6,i2+3,c2,c3)+c(i,0,4,0)*ui(i1  ,i2+4,c2,
     & c3)+c(i,1,4,0)*ui(i1+1,i2+4,c2,c3)+c(i,2,4,0)*ui(i1+2,i2+4,c2,
     & c3)+c(i,3,4,0)*ui(i1+3,i2+4,c2,c3)+c(i,4,4,0)*ui(i1+4,i2+4,c2,
     & c3)+c(i,5,4,0)*ui(i1+5,i2+4,c2,c3)+c(i,6,4,0)*ui(i1+6,i2+4,c2,
     & c3)+c(i,0,5,0)*ui(i1  ,i2+5,c2,c3)+c(i,1,5,0)*ui(i1+1,i2+5,c2,
     & c3)+c(i,2,5,0)*ui(i1+2,i2+5,c2,c3)+c(i,3,5,0)*ui(i1+3,i2+5,c2,
     & c3)+c(i,4,5,0)*ui(i1+4,i2+5,c2,c3)+c(i,5,5,0)*ui(i1+5,i2+5,c2,
     & c3)+c(i,6,5,0)*ui(i1+6,i2+5,c2,c3)+c(i,0,6,0)*ui(i1  ,i2+6,c2,
     & c3)+c(i,1,6,0)*ui(i1+1,i2+6,c2,c3)+c(i,2,6,0)*ui(i1+2,i2+6,c2,
     & c3)+c(i,3,6,0)*ui(i1+3,i2+6,c2,c3)+c(i,4,6,0)*ui(i1+4,i2+6,c2,
     & c3)+c(i,5,6,0)*ui(i1+5,i2+6,c2,c3)+c(i,6,6,0)*ui(i1+6,i2+6,c2,
     & c3)
             else if( varWidth(i).eq.6 )then
! interp66(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
               ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,0,0)*ui(i1  ,i2  ,c2,
     & c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,
     & c3)+c(i,3,0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,
     & c3)+c(i,5,0,0)*ui(i1+5,i2  ,c2,c3)+c(i,0,1,0)*ui(i1  ,i2+1,c2,
     & c3)+c(i,1,1,0)*ui(i1+1,i2+1,c2,c3)+c(i,2,1,0)*ui(i1+2,i2+1,c2,
     & c3)+c(i,3,1,0)*ui(i1+3,i2+1,c2,c3)+c(i,4,1,0)*ui(i1+4,i2+1,c2,
     & c3)+c(i,5,1,0)*ui(i1+5,i2+1,c2,c3)+c(i,0,2,0)*ui(i1  ,i2+2,c2,
     & c3)+c(i,1,2,0)*ui(i1+1,i2+2,c2,c3)+c(i,2,2,0)*ui(i1+2,i2+2,c2,
     & c3)+c(i,3,2,0)*ui(i1+3,i2+2,c2,c3)+c(i,4,2,0)*ui(i1+4,i2+2,c2,
     & c3)+c(i,5,2,0)*ui(i1+5,i2+2,c2,c3)+c(i,0,3,0)*ui(i1  ,i2+3,c2,
     & c3)+c(i,1,3,0)*ui(i1+1,i2+3,c2,c3)+c(i,2,3,0)*ui(i1+2,i2+3,c2,
     & c3)+c(i,3,3,0)*ui(i1+3,i2+3,c2,c3)+c(i,4,3,0)*ui(i1+4,i2+3,c2,
     & c3)+c(i,5,3,0)*ui(i1+5,i2+3,c2,c3)+c(i,0,4,0)*ui(i1  ,i2+4,c2,
     & c3)+c(i,1,4,0)*ui(i1+1,i2+4,c2,c3)+c(i,2,4,0)*ui(i1+2,i2+4,c2,
     & c3)+c(i,3,4,0)*ui(i1+3,i2+4,c2,c3)+c(i,4,4,0)*ui(i1+4,i2+4,c2,
     & c3)+c(i,5,4,0)*ui(i1+5,i2+4,c2,c3)+c(i,0,5,0)*ui(i1  ,i2+5,c2,
     & c3)+c(i,1,5,0)*ui(i1+1,i2+5,c2,c3)+c(i,2,5,0)*ui(i1+2,i2+5,c2,
     & c3)+c(i,3,5,0)*ui(i1+3,i2+5,c2,c3)+c(i,4,5,0)*ui(i1+4,i2+5,c2,
     & c3)+c(i,5,5,0)*ui(i1+5,i2+5,c2,c3)
             else if( varWidth(i).eq.9 )then
! interp99(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
               ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,0,0)*ui(i1  ,i2  ,c2,
     & c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,
     & c3)+c(i,3,0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,
     & c3)+c(i,5,0,0)*ui(i1+5,i2  ,c2,c3)+c(i,6,0,0)*ui(i1+6,i2  ,c2,
     & c3)+c(i,7,0,0)*ui(i1+7,i2  ,c2,c3)+c(i,8,0,0)*ui(i1+8,i2  ,c2,
     & c3)+c(i,0,1,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,1,0)*ui(i1+1,i2+1,c2,
     & c3)+c(i,2,1,0)*ui(i1+2,i2+1,c2,c3)+c(i,3,1,0)*ui(i1+3,i2+1,c2,
     & c3)+c(i,4,1,0)*ui(i1+4,i2+1,c2,c3)+c(i,5,1,0)*ui(i1+5,i2+1,c2,
     & c3)+c(i,6,1,0)*ui(i1+6,i2+1,c2,c3)+c(i,7,1,0)*ui(i1+7,i2+1,c2,
     & c3)+c(i,8,1,0)*ui(i1+8,i2+1,c2,c3)+c(i,0,2,0)*ui(i1  ,i2+2,c2,
     & c3)+c(i,1,2,0)*ui(i1+1,i2+2,c2,c3)+c(i,2,2,0)*ui(i1+2,i2+2,c2,
     & c3)+c(i,3,2,0)*ui(i1+3,i2+2,c2,c3)+c(i,4,2,0)*ui(i1+4,i2+2,c2,
     & c3)+c(i,5,2,0)*ui(i1+5,i2+2,c2,c3)+c(i,6,2,0)*ui(i1+6,i2+2,c2,
     & c3)+c(i,7,2,0)*ui(i1+7,i2+2,c2,c3)+c(i,8,2,0)*ui(i1+8,i2+2,c2,
     & c3)+c(i,0,3,0)*ui(i1  ,i2+3,c2,c3)+c(i,1,3,0)*ui(i1+1,i2+3,c2,
     & c3)+c(i,2,3,0)*ui(i1+2,i2+3,c2,c3)+c(i,3,3,0)*ui(i1+3,i2+3,c2,
     & c3)+c(i,4,3,0)*ui(i1+4,i2+3,c2,c3)+c(i,5,3,0)*ui(i1+5,i2+3,c2,
     & c3)+c(i,6,3,0)*ui(i1+6,i2+3,c2,c3)+c(i,7,3,0)*ui(i1+7,i2+3,c2,
     & c3)+c(i,8,3,0)*ui(i1+8,i2+3,c2,c3)+c(i,0,4,0)*ui(i1  ,i2+4,c2,
     & c3)+c(i,1,4,0)*ui(i1+1,i2+4,c2,c3)+c(i,2,4,0)*ui(i1+2,i2+4,c2,
     & c3)+c(i,3,4,0)*ui(i1+3,i2+4,c2,c3)+c(i,4,4,0)*ui(i1+4,i2+4,c2,
     & c3)+c(i,5,4,0)*ui(i1+5,i2+4,c2,c3)+c(i,6,4,0)*ui(i1+6,i2+4,c2,
     & c3)+c(i,7,4,0)*ui(i1+7,i2+4,c2,c3)+c(i,8,4,0)*ui(i1+8,i2+4,c2,
     & c3)+c(i,0,5,0)*ui(i1  ,i2+5,c2,c3)+c(i,1,5,0)*ui(i1+1,i2+5,c2,
     & c3)+c(i,2,5,0)*ui(i1+2,i2+5,c2,c3)+c(i,3,5,0)*ui(i1+3,i2+5,c2,
     & c3)+c(i,4,5,0)*ui(i1+4,i2+5,c2,c3)+c(i,5,5,0)*ui(i1+5,i2+5,c2,
     & c3)+c(i,6,5,0)*ui(i1+6,i2+5,c2,c3)+c(i,7,5,0)*ui(i1+7,i2+5,c2,
     & c3)+c(i,8,5,0)*ui(i1+8,i2+5,c2,c3)+c(i,0,6,0)*ui(i1  ,i2+6,c2,
     & c3)+c(i,1,6,0)*ui(i1+1,i2+6,c2,c3)+c(i,2,6,0)*ui(i1+2,i2+6,c2,
     & c3)+c(i,3,6,0)*ui(i1+3,i2+6,c2,c3)+c(i,4,6,0)*ui(i1+4,i2+6,c2,
     & c3)+c(i,5,6,0)*ui(i1+5,i2+6,c2,c3)+c(i,6,6,0)*ui(i1+6,i2+6,c2,
     & c3)+c(i,7,6,0)*ui(i1+7,i2+6,c2,c3)+c(i,8,6,0)*ui(i1+8,i2+6,c2,
     & c3)+c(i,0,7,0)*ui(i1  ,i2+7,c2,c3)+c(i,1,7,0)*ui(i1+1,i2+7,c2,
     & c3)+c(i,2,7,0)*ui(i1+2,i2+7,c2,c3)+c(i,3,7,0)*ui(i1+3,i2+7,c2,
     & c3)+c(i,4,7,0)*ui(i1+4,i2+7,c2,c3)+c(i,5,7,0)*ui(i1+5,i2+7,c2,
     & c3)+c(i,6,7,0)*ui(i1+6,i2+7,c2,c3)+c(i,7,7,0)*ui(i1+7,i2+7,c2,
     & c3)+c(i,8,7,0)*ui(i1+8,i2+7,c2,c3)+c(i,0,8,0)*ui(i1  ,i2+8,c2,
     & c3)+c(i,1,8,0)*ui(i1+1,i2+8,c2,c3)+c(i,2,8,0)*ui(i1+2,i2+8,c2,
     & c3)+c(i,3,8,0)*ui(i1+3,i2+8,c2,c3)+c(i,4,8,0)*ui(i1+4,i2+8,c2,
     & c3)+c(i,5,8,0)*ui(i1+5,i2+8,c2,c3)+c(i,6,8,0)*ui(i1+6,i2+8,c2,
     & c3)+c(i,7,8,0)*ui(i1+7,i2+8,c2,c3)+c(i,8,8,0)*ui(i1+8,i2+8,c2,
     & c3)
             else if( varWidth(i).eq.8 )then
! interp88(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
               ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,0,0)*ui(i1  ,i2  ,c2,
     & c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,
     & c3)+c(i,3,0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,
     & c3)+c(i,5,0,0)*ui(i1+5,i2  ,c2,c3)+c(i,6,0,0)*ui(i1+6,i2  ,c2,
     & c3)+c(i,7,0,0)*ui(i1+7,i2  ,c2,c3)+c(i,0,1,0)*ui(i1  ,i2+1,c2,
     & c3)+c(i,1,1,0)*ui(i1+1,i2+1,c2,c3)+c(i,2,1,0)*ui(i1+2,i2+1,c2,
     & c3)+c(i,3,1,0)*ui(i1+3,i2+1,c2,c3)+c(i,4,1,0)*ui(i1+4,i2+1,c2,
     & c3)+c(i,5,1,0)*ui(i1+5,i2+1,c2,c3)+c(i,6,1,0)*ui(i1+6,i2+1,c2,
     & c3)+c(i,7,1,0)*ui(i1+7,i2+1,c2,c3)+c(i,0,2,0)*ui(i1  ,i2+2,c2,
     & c3)+c(i,1,2,0)*ui(i1+1,i2+2,c2,c3)+c(i,2,2,0)*ui(i1+2,i2+2,c2,
     & c3)+c(i,3,2,0)*ui(i1+3,i2+2,c2,c3)+c(i,4,2,0)*ui(i1+4,i2+2,c2,
     & c3)+c(i,5,2,0)*ui(i1+5,i2+2,c2,c3)+c(i,6,2,0)*ui(i1+6,i2+2,c2,
     & c3)+c(i,7,2,0)*ui(i1+7,i2+2,c2,c3)+c(i,0,3,0)*ui(i1  ,i2+3,c2,
     & c3)+c(i,1,3,0)*ui(i1+1,i2+3,c2,c3)+c(i,2,3,0)*ui(i1+2,i2+3,c2,
     & c3)+c(i,3,3,0)*ui(i1+3,i2+3,c2,c3)+c(i,4,3,0)*ui(i1+4,i2+3,c2,
     & c3)+c(i,5,3,0)*ui(i1+5,i2+3,c2,c3)+c(i,6,3,0)*ui(i1+6,i2+3,c2,
     & c3)+c(i,7,3,0)*ui(i1+7,i2+3,c2,c3)+c(i,0,4,0)*ui(i1  ,i2+4,c2,
     & c3)+c(i,1,4,0)*ui(i1+1,i2+4,c2,c3)+c(i,2,4,0)*ui(i1+2,i2+4,c2,
     & c3)+c(i,3,4,0)*ui(i1+3,i2+4,c2,c3)+c(i,4,4,0)*ui(i1+4,i2+4,c2,
     & c3)+c(i,5,4,0)*ui(i1+5,i2+4,c2,c3)+c(i,6,4,0)*ui(i1+6,i2+4,c2,
     & c3)+c(i,7,4,0)*ui(i1+7,i2+4,c2,c3)+c(i,0,5,0)*ui(i1  ,i2+5,c2,
     & c3)+c(i,1,5,0)*ui(i1+1,i2+5,c2,c3)+c(i,2,5,0)*ui(i1+2,i2+5,c2,
     & c3)+c(i,3,5,0)*ui(i1+3,i2+5,c2,c3)+c(i,4,5,0)*ui(i1+4,i2+5,c2,
     & c3)+c(i,5,5,0)*ui(i1+5,i2+5,c2,c3)+c(i,6,5,0)*ui(i1+6,i2+5,c2,
     & c3)+c(i,7,5,0)*ui(i1+7,i2+5,c2,c3)+c(i,0,6,0)*ui(i1  ,i2+6,c2,
     & c3)+c(i,1,6,0)*ui(i1+1,i2+6,c2,c3)+c(i,2,6,0)*ui(i1+2,i2+6,c2,
     & c3)+c(i,3,6,0)*ui(i1+3,i2+6,c2,c3)+c(i,4,6,0)*ui(i1+4,i2+6,c2,
     & c3)+c(i,5,6,0)*ui(i1+5,i2+6,c2,c3)+c(i,6,6,0)*ui(i1+6,i2+6,c2,
     & c3)+c(i,7,6,0)*ui(i1+7,i2+6,c2,c3)+c(i,0,7,0)*ui(i1  ,i2+7,c2,
     & c3)+c(i,1,7,0)*ui(i1+1,i2+7,c2,c3)+c(i,2,7,0)*ui(i1+2,i2+7,c2,
     & c3)+c(i,3,7,0)*ui(i1+3,i2+7,c2,c3)+c(i,4,7,0)*ui(i1+4,i2+7,c2,
     & c3)+c(i,5,7,0)*ui(i1+5,i2+7,c2,c3)+c(i,6,7,0)*ui(i1+6,i2+7,c2,
     & c3)+c(i,7,7,0)*ui(i1+7,i2+7,c2,c3)
             else
               write(*,*) 'ERROR varWidth=',varWidth(i)
               stop 151
             end if
! endLoops2d()
             end do
             end do
             end do
         else if( width(1).eq.3 .and. width(2).eq.3 ) then ! most common case
! loops2d($interp33(ug(ip(i,1),ip(i,2),c2,c3)),,)
            if( c2a.eq.c2b .and. c3a.eq.c3b )then
              do c3=c3a,c3b
              do c2=c2a,c2b
              do i=nia,nib
! interp33(ug(ip(i,1),ip(i,2),c2,c3))
                i1=il(i,1)
                i2=il(i,2)
                ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,0,0)*ui(i1  ,i2  ,c2,
     & c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,
     & c3)+c(i,0,1,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,1,0)*ui(i1+1,i2+1,c2,
     & c3)+c(i,2,1,0)*ui(i1+2,i2+1,c2,c3)+c(i,0,2,0)*ui(i1  ,i2+2,c2,
     & c3)+c(i,1,2,0)*ui(i1+1,i2+2,c2,c3)+c(i,2,2,0)*ui(i1+2,i2+2,c2,
     & c3)


              end do
              end do
              end do
            else
              ! put "c" loop as inner loop, this seems to be faster
              do i=nia,nib
              do c3=c3a,c3b
              do c2=c2a,c2b
! interp33(ug(ip(i,1),ip(i,2),c2,c3))
                i1=il(i,1)
                i2=il(i,2)
                ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,0,0)*ui(i1  ,i2  ,c2,
     & c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,
     & c3)+c(i,0,1,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,1,0)*ui(i1+1,i2+1,c2,
     & c3)+c(i,2,1,0)*ui(i1+2,i2+1,c2,c3)+c(i,0,2,0)*ui(i1  ,i2+2,c2,
     & c3)+c(i,1,2,0)*ui(i1+1,i2+2,c2,c3)+c(i,2,2,0)*ui(i1+2,i2+2,c2,
     & c3)


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
! loops2d($interp22(ug(ip(i,1),ip(i,2),c2,c3)),,)
            if( c2a.eq.c2b .and. c3a.eq.c3b )then
              do c3=c3a,c3b
              do c2=c2a,c2b
              do i=nia,nib
! interp22(ug(ip(i,1),ip(i,2),c2,c3))
                i1=il(i,1)
                i2=il(i,2)
                ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,0,0)*ui(i1  ,i2  ,c2,
     & c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,0,1,0)*ui(i1  ,i2+1,c2,
     & c3)+c(i,1,1,0)*ui(i1+1,i2+1,c2,c3)


              end do
              end do
              end do
            else
              ! put "c" loop as inner loop, this seems to be faster
              do i=nia,nib
              do c3=c3a,c3b
              do c2=c2a,c2b
! interp22(ug(ip(i,1),ip(i,2),c2,c3))
                i1=il(i,1)
                i2=il(i,2)
                ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,0,0)*ui(i1  ,i2  ,c2,
     & c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,0,1,0)*ui(i1  ,i2+1,c2,
     & c3)+c(i,1,1,0)*ui(i1+1,i2+1,c2,c3)


              end do
              end do
              end do
            end if
          else if( width(1).eq.4 .and. width(2).eq.4 )then
! loops2d($interp44(ug(ip(i,1),ip(i,2),c2,c3)),,)
            if( c2a.eq.c2b .and. c3a.eq.c3b )then
              do c3=c3a,c3b
              do c2=c2a,c2b
              do i=nia,nib
! interp44(ug(ip(i,1),ip(i,2),c2,c3))
                i1=il(i,1)
                i2=il(i,2)
                ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,0,0)*ui(i1  ,i2  ,c2,
     & c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,
     & c3)+c(i,3,0,0)*ui(i1+3,i2  ,c2,c3)+c(i,0,1,0)*ui(i1  ,i2+1,c2,
     & c3)+c(i,1,1,0)*ui(i1+1,i2+1,c2,c3)+c(i,2,1,0)*ui(i1+2,i2+1,c2,
     & c3)+c(i,3,1,0)*ui(i1+3,i2+1,c2,c3)+c(i,0,2,0)*ui(i1  ,i2+2,c2,
     & c3)+c(i,1,2,0)*ui(i1+1,i2+2,c2,c3)+c(i,2,2,0)*ui(i1+2,i2+2,c2,
     & c3)+c(i,3,2,0)*ui(i1+3,i2+2,c2,c3)+c(i,0,3,0)*ui(i1  ,i2+3,c2,
     & c3)+c(i,1,3,0)*ui(i1+1,i2+3,c2,c3)+c(i,2,3,0)*ui(i1+2,i2+3,c2,
     & c3)+c(i,3,3,0)*ui(i1+3,i2+3,c2,c3)


              end do
              end do
              end do
            else
              ! put "c" loop as inner loop, this seems to be faster
              do i=nia,nib
              do c3=c3a,c3b
              do c2=c2a,c2b
! interp44(ug(ip(i,1),ip(i,2),c2,c3))
                i1=il(i,1)
                i2=il(i,2)
                ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,0,0)*ui(i1  ,i2  ,c2,
     & c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,
     & c3)+c(i,3,0,0)*ui(i1+3,i2  ,c2,c3)+c(i,0,1,0)*ui(i1  ,i2+1,c2,
     & c3)+c(i,1,1,0)*ui(i1+1,i2+1,c2,c3)+c(i,2,1,0)*ui(i1+2,i2+1,c2,
     & c3)+c(i,3,1,0)*ui(i1+3,i2+1,c2,c3)+c(i,0,2,0)*ui(i1  ,i2+2,c2,
     & c3)+c(i,1,2,0)*ui(i1+1,i2+2,c2,c3)+c(i,2,2,0)*ui(i1+2,i2+2,c2,
     & c3)+c(i,3,2,0)*ui(i1+3,i2+2,c2,c3)+c(i,0,3,0)*ui(i1  ,i2+3,c2,
     & c3)+c(i,1,3,0)*ui(i1+1,i2+3,c2,c3)+c(i,2,3,0)*ui(i1+2,i2+3,c2,
     & c3)+c(i,3,3,0)*ui(i1+3,i2+3,c2,c3)


              end do
              end do
              end do
            end if
          else if( width(1).eq.5 .and. width(2).eq.5 )then
! loops2d($interp55(ug(ip(i,1),ip(i,2),c2,c3)),,)
            if( c2a.eq.c2b .and. c3a.eq.c3b )then
              do c3=c3a,c3b
              do c2=c2a,c2b
              do i=nia,nib
! interp55(ug(ip(i,1),ip(i,2),c2,c3))
                i1=il(i,1)
                i2=il(i,2)
                ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,0,0)*ui(i1  ,i2  ,c2,
     & c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,
     & c3)+c(i,3,0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,
     & c3)+c(i,0,1,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,1,0)*ui(i1+1,i2+1,c2,
     & c3)+c(i,2,1,0)*ui(i1+2,i2+1,c2,c3)+c(i,3,1,0)*ui(i1+3,i2+1,c2,
     & c3)+c(i,4,1,0)*ui(i1+4,i2+1,c2,c3)+c(i,0,2,0)*ui(i1  ,i2+2,c2,
     & c3)+c(i,1,2,0)*ui(i1+1,i2+2,c2,c3)+c(i,2,2,0)*ui(i1+2,i2+2,c2,
     & c3)+c(i,3,2,0)*ui(i1+3,i2+2,c2,c3)+c(i,4,2,0)*ui(i1+4,i2+2,c2,
     & c3)+c(i,0,3,0)*ui(i1  ,i2+3,c2,c3)+c(i,1,3,0)*ui(i1+1,i2+3,c2,
     & c3)+c(i,2,3,0)*ui(i1+2,i2+3,c2,c3)+c(i,3,3,0)*ui(i1+3,i2+3,c2,
     & c3)+c(i,4,3,0)*ui(i1+4,i2+3,c2,c3)+c(i,0,4,0)*ui(i1  ,i2+4,c2,
     & c3)+c(i,1,4,0)*ui(i1+1,i2+4,c2,c3)+c(i,2,4,0)*ui(i1+2,i2+4,c2,
     & c3)+c(i,3,4,0)*ui(i1+3,i2+4,c2,c3)+c(i,4,4,0)*ui(i1+4,i2+4,c2,
     & c3)


              end do
              end do
              end do
            else
              ! put "c" loop as inner loop, this seems to be faster
              do i=nia,nib
              do c3=c3a,c3b
              do c2=c2a,c2b
! interp55(ug(ip(i,1),ip(i,2),c2,c3))
                i1=il(i,1)
                i2=il(i,2)
                ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,0,0)*ui(i1  ,i2  ,c2,
     & c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,
     & c3)+c(i,3,0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,
     & c3)+c(i,0,1,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,1,0)*ui(i1+1,i2+1,c2,
     & c3)+c(i,2,1,0)*ui(i1+2,i2+1,c2,c3)+c(i,3,1,0)*ui(i1+3,i2+1,c2,
     & c3)+c(i,4,1,0)*ui(i1+4,i2+1,c2,c3)+c(i,0,2,0)*ui(i1  ,i2+2,c2,
     & c3)+c(i,1,2,0)*ui(i1+1,i2+2,c2,c3)+c(i,2,2,0)*ui(i1+2,i2+2,c2,
     & c3)+c(i,3,2,0)*ui(i1+3,i2+2,c2,c3)+c(i,4,2,0)*ui(i1+4,i2+2,c2,
     & c3)+c(i,0,3,0)*ui(i1  ,i2+3,c2,c3)+c(i,1,3,0)*ui(i1+1,i2+3,c2,
     & c3)+c(i,2,3,0)*ui(i1+2,i2+3,c2,c3)+c(i,3,3,0)*ui(i1+3,i2+3,c2,
     & c3)+c(i,4,3,0)*ui(i1+4,i2+3,c2,c3)+c(i,0,4,0)*ui(i1  ,i2+4,c2,
     & c3)+c(i,1,4,0)*ui(i1+1,i2+4,c2,c3)+c(i,2,4,0)*ui(i1+2,i2+4,c2,
     & c3)+c(i,3,4,0)*ui(i1+3,i2+4,c2,c3)+c(i,4,4,0)*ui(i1+4,i2+4,c2,
     & c3)


              end do
              end do
              end do
            end if
          else if( width(1).eq.6 .and. width(2).eq.6 )then
! loops2d($interp66(ug(ip(i,1),ip(i,2),c2,c3)),,)
            if( c2a.eq.c2b .and. c3a.eq.c3b )then
              do c3=c3a,c3b
              do c2=c2a,c2b
              do i=nia,nib
! interp66(ug(ip(i,1),ip(i,2),c2,c3))
                i1=il(i,1)
                i2=il(i,2)
                ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,0,0)*ui(i1  ,i2  ,c2,
     & c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,
     & c3)+c(i,3,0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,
     & c3)+c(i,5,0,0)*ui(i1+5,i2  ,c2,c3)+c(i,0,1,0)*ui(i1  ,i2+1,c2,
     & c3)+c(i,1,1,0)*ui(i1+1,i2+1,c2,c3)+c(i,2,1,0)*ui(i1+2,i2+1,c2,
     & c3)+c(i,3,1,0)*ui(i1+3,i2+1,c2,c3)+c(i,4,1,0)*ui(i1+4,i2+1,c2,
     & c3)+c(i,5,1,0)*ui(i1+5,i2+1,c2,c3)+c(i,0,2,0)*ui(i1  ,i2+2,c2,
     & c3)+c(i,1,2,0)*ui(i1+1,i2+2,c2,c3)+c(i,2,2,0)*ui(i1+2,i2+2,c2,
     & c3)+c(i,3,2,0)*ui(i1+3,i2+2,c2,c3)+c(i,4,2,0)*ui(i1+4,i2+2,c2,
     & c3)+c(i,5,2,0)*ui(i1+5,i2+2,c2,c3)+c(i,0,3,0)*ui(i1  ,i2+3,c2,
     & c3)+c(i,1,3,0)*ui(i1+1,i2+3,c2,c3)+c(i,2,3,0)*ui(i1+2,i2+3,c2,
     & c3)+c(i,3,3,0)*ui(i1+3,i2+3,c2,c3)+c(i,4,3,0)*ui(i1+4,i2+3,c2,
     & c3)+c(i,5,3,0)*ui(i1+5,i2+3,c2,c3)+c(i,0,4,0)*ui(i1  ,i2+4,c2,
     & c3)+c(i,1,4,0)*ui(i1+1,i2+4,c2,c3)+c(i,2,4,0)*ui(i1+2,i2+4,c2,
     & c3)+c(i,3,4,0)*ui(i1+3,i2+4,c2,c3)+c(i,4,4,0)*ui(i1+4,i2+4,c2,
     & c3)+c(i,5,4,0)*ui(i1+5,i2+4,c2,c3)+c(i,0,5,0)*ui(i1  ,i2+5,c2,
     & c3)+c(i,1,5,0)*ui(i1+1,i2+5,c2,c3)+c(i,2,5,0)*ui(i1+2,i2+5,c2,
     & c3)+c(i,3,5,0)*ui(i1+3,i2+5,c2,c3)+c(i,4,5,0)*ui(i1+4,i2+5,c2,
     & c3)+c(i,5,5,0)*ui(i1+5,i2+5,c2,c3)


              end do
              end do
              end do
            else
              ! put "c" loop as inner loop, this seems to be faster
              do i=nia,nib
              do c3=c3a,c3b
              do c2=c2a,c2b
! interp66(ug(ip(i,1),ip(i,2),c2,c3))
                i1=il(i,1)
                i2=il(i,2)
                ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,0,0)*ui(i1  ,i2  ,c2,
     & c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,
     & c3)+c(i,3,0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,
     & c3)+c(i,5,0,0)*ui(i1+5,i2  ,c2,c3)+c(i,0,1,0)*ui(i1  ,i2+1,c2,
     & c3)+c(i,1,1,0)*ui(i1+1,i2+1,c2,c3)+c(i,2,1,0)*ui(i1+2,i2+1,c2,
     & c3)+c(i,3,1,0)*ui(i1+3,i2+1,c2,c3)+c(i,4,1,0)*ui(i1+4,i2+1,c2,
     & c3)+c(i,5,1,0)*ui(i1+5,i2+1,c2,c3)+c(i,0,2,0)*ui(i1  ,i2+2,c2,
     & c3)+c(i,1,2,0)*ui(i1+1,i2+2,c2,c3)+c(i,2,2,0)*ui(i1+2,i2+2,c2,
     & c3)+c(i,3,2,0)*ui(i1+3,i2+2,c2,c3)+c(i,4,2,0)*ui(i1+4,i2+2,c2,
     & c3)+c(i,5,2,0)*ui(i1+5,i2+2,c2,c3)+c(i,0,3,0)*ui(i1  ,i2+3,c2,
     & c3)+c(i,1,3,0)*ui(i1+1,i2+3,c2,c3)+c(i,2,3,0)*ui(i1+2,i2+3,c2,
     & c3)+c(i,3,3,0)*ui(i1+3,i2+3,c2,c3)+c(i,4,3,0)*ui(i1+4,i2+3,c2,
     & c3)+c(i,5,3,0)*ui(i1+5,i2+3,c2,c3)+c(i,0,4,0)*ui(i1  ,i2+4,c2,
     & c3)+c(i,1,4,0)*ui(i1+1,i2+4,c2,c3)+c(i,2,4,0)*ui(i1+2,i2+4,c2,
     & c3)+c(i,3,4,0)*ui(i1+3,i2+4,c2,c3)+c(i,4,4,0)*ui(i1+4,i2+4,c2,
     & c3)+c(i,5,4,0)*ui(i1+5,i2+4,c2,c3)+c(i,0,5,0)*ui(i1  ,i2+5,c2,
     & c3)+c(i,1,5,0)*ui(i1+1,i2+5,c2,c3)+c(i,2,5,0)*ui(i1+2,i2+5,c2,
     & c3)+c(i,3,5,0)*ui(i1+3,i2+5,c2,c3)+c(i,4,5,0)*ui(i1+4,i2+5,c2,
     & c3)+c(i,5,5,0)*ui(i1+5,i2+5,c2,c3)


              end do
              end do
              end do
            end if
          else if( width(1).eq.7 .and. width(2).eq.7 )then
! loops2d($interp77(ug(ip(i,1),ip(i,2),c2,c3)),,)
            if( c2a.eq.c2b .and. c3a.eq.c3b )then
              do c3=c3a,c3b
              do c2=c2a,c2b
              do i=nia,nib
! interp77(ug(ip(i,1),ip(i,2),c2,c3))
                i1=il(i,1)
                i2=il(i,2)
                ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,0,0)*ui(i1  ,i2  ,c2,
     & c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,
     & c3)+c(i,3,0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,
     & c3)+c(i,5,0,0)*ui(i1+5,i2  ,c2,c3)+c(i,6,0,0)*ui(i1+6,i2  ,c2,
     & c3)+c(i,0,1,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,1,0)*ui(i1+1,i2+1,c2,
     & c3)+c(i,2,1,0)*ui(i1+2,i2+1,c2,c3)+c(i,3,1,0)*ui(i1+3,i2+1,c2,
     & c3)+c(i,4,1,0)*ui(i1+4,i2+1,c2,c3)+c(i,5,1,0)*ui(i1+5,i2+1,c2,
     & c3)+c(i,6,1,0)*ui(i1+6,i2+1,c2,c3)+c(i,0,2,0)*ui(i1  ,i2+2,c2,
     & c3)+c(i,1,2,0)*ui(i1+1,i2+2,c2,c3)+c(i,2,2,0)*ui(i1+2,i2+2,c2,
     & c3)+c(i,3,2,0)*ui(i1+3,i2+2,c2,c3)+c(i,4,2,0)*ui(i1+4,i2+2,c2,
     & c3)+c(i,5,2,0)*ui(i1+5,i2+2,c2,c3)+c(i,6,2,0)*ui(i1+6,i2+2,c2,
     & c3)+c(i,0,3,0)*ui(i1  ,i2+3,c2,c3)+c(i,1,3,0)*ui(i1+1,i2+3,c2,
     & c3)+c(i,2,3,0)*ui(i1+2,i2+3,c2,c3)+c(i,3,3,0)*ui(i1+3,i2+3,c2,
     & c3)+c(i,4,3,0)*ui(i1+4,i2+3,c2,c3)+c(i,5,3,0)*ui(i1+5,i2+3,c2,
     & c3)+c(i,6,3,0)*ui(i1+6,i2+3,c2,c3)+c(i,0,4,0)*ui(i1  ,i2+4,c2,
     & c3)+c(i,1,4,0)*ui(i1+1,i2+4,c2,c3)+c(i,2,4,0)*ui(i1+2,i2+4,c2,
     & c3)+c(i,3,4,0)*ui(i1+3,i2+4,c2,c3)+c(i,4,4,0)*ui(i1+4,i2+4,c2,
     & c3)+c(i,5,4,0)*ui(i1+5,i2+4,c2,c3)+c(i,6,4,0)*ui(i1+6,i2+4,c2,
     & c3)+c(i,0,5,0)*ui(i1  ,i2+5,c2,c3)+c(i,1,5,0)*ui(i1+1,i2+5,c2,
     & c3)+c(i,2,5,0)*ui(i1+2,i2+5,c2,c3)+c(i,3,5,0)*ui(i1+3,i2+5,c2,
     & c3)+c(i,4,5,0)*ui(i1+4,i2+5,c2,c3)+c(i,5,5,0)*ui(i1+5,i2+5,c2,
     & c3)+c(i,6,5,0)*ui(i1+6,i2+5,c2,c3)+c(i,0,6,0)*ui(i1  ,i2+6,c2,
     & c3)+c(i,1,6,0)*ui(i1+1,i2+6,c2,c3)+c(i,2,6,0)*ui(i1+2,i2+6,c2,
     & c3)+c(i,3,6,0)*ui(i1+3,i2+6,c2,c3)+c(i,4,6,0)*ui(i1+4,i2+6,c2,
     & c3)+c(i,5,6,0)*ui(i1+5,i2+6,c2,c3)+c(i,6,6,0)*ui(i1+6,i2+6,c2,
     & c3)


              end do
              end do
              end do
            else
              ! put "c" loop as inner loop, this seems to be faster
              do i=nia,nib
              do c3=c3a,c3b
              do c2=c2a,c2b
! interp77(ug(ip(i,1),ip(i,2),c2,c3))
                i1=il(i,1)
                i2=il(i,2)
                ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,0,0)*ui(i1  ,i2  ,c2,
     & c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,
     & c3)+c(i,3,0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,
     & c3)+c(i,5,0,0)*ui(i1+5,i2  ,c2,c3)+c(i,6,0,0)*ui(i1+6,i2  ,c2,
     & c3)+c(i,0,1,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,1,0)*ui(i1+1,i2+1,c2,
     & c3)+c(i,2,1,0)*ui(i1+2,i2+1,c2,c3)+c(i,3,1,0)*ui(i1+3,i2+1,c2,
     & c3)+c(i,4,1,0)*ui(i1+4,i2+1,c2,c3)+c(i,5,1,0)*ui(i1+5,i2+1,c2,
     & c3)+c(i,6,1,0)*ui(i1+6,i2+1,c2,c3)+c(i,0,2,0)*ui(i1  ,i2+2,c2,
     & c3)+c(i,1,2,0)*ui(i1+1,i2+2,c2,c3)+c(i,2,2,0)*ui(i1+2,i2+2,c2,
     & c3)+c(i,3,2,0)*ui(i1+3,i2+2,c2,c3)+c(i,4,2,0)*ui(i1+4,i2+2,c2,
     & c3)+c(i,5,2,0)*ui(i1+5,i2+2,c2,c3)+c(i,6,2,0)*ui(i1+6,i2+2,c2,
     & c3)+c(i,0,3,0)*ui(i1  ,i2+3,c2,c3)+c(i,1,3,0)*ui(i1+1,i2+3,c2,
     & c3)+c(i,2,3,0)*ui(i1+2,i2+3,c2,c3)+c(i,3,3,0)*ui(i1+3,i2+3,c2,
     & c3)+c(i,4,3,0)*ui(i1+4,i2+3,c2,c3)+c(i,5,3,0)*ui(i1+5,i2+3,c2,
     & c3)+c(i,6,3,0)*ui(i1+6,i2+3,c2,c3)+c(i,0,4,0)*ui(i1  ,i2+4,c2,
     & c3)+c(i,1,4,0)*ui(i1+1,i2+4,c2,c3)+c(i,2,4,0)*ui(i1+2,i2+4,c2,
     & c3)+c(i,3,4,0)*ui(i1+3,i2+4,c2,c3)+c(i,4,4,0)*ui(i1+4,i2+4,c2,
     & c3)+c(i,5,4,0)*ui(i1+5,i2+4,c2,c3)+c(i,6,4,0)*ui(i1+6,i2+4,c2,
     & c3)+c(i,0,5,0)*ui(i1  ,i2+5,c2,c3)+c(i,1,5,0)*ui(i1+1,i2+5,c2,
     & c3)+c(i,2,5,0)*ui(i1+2,i2+5,c2,c3)+c(i,3,5,0)*ui(i1+3,i2+5,c2,
     & c3)+c(i,4,5,0)*ui(i1+4,i2+5,c2,c3)+c(i,5,5,0)*ui(i1+5,i2+5,c2,
     & c3)+c(i,6,5,0)*ui(i1+6,i2+5,c2,c3)+c(i,0,6,0)*ui(i1  ,i2+6,c2,
     & c3)+c(i,1,6,0)*ui(i1+1,i2+6,c2,c3)+c(i,2,6,0)*ui(i1+2,i2+6,c2,
     & c3)+c(i,3,6,0)*ui(i1+3,i2+6,c2,c3)+c(i,4,6,0)*ui(i1+4,i2+6,c2,
     & c3)+c(i,5,6,0)*ui(i1+5,i2+6,c2,c3)+c(i,6,6,0)*ui(i1+6,i2+6,c2,
     & c3)


              end do
              end do
              end do
            end if
          else if( width(1).eq.8 .and. width(2).eq.8 )then
! loops2d($interp88(ug(ip(i,1),ip(i,2),c2,c3)),,)
            if( c2a.eq.c2b .and. c3a.eq.c3b )then
              do c3=c3a,c3b
              do c2=c2a,c2b
              do i=nia,nib
! interp88(ug(ip(i,1),ip(i,2),c2,c3))
                i1=il(i,1)
                i2=il(i,2)
                ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,0,0)*ui(i1  ,i2  ,c2,
     & c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,
     & c3)+c(i,3,0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,
     & c3)+c(i,5,0,0)*ui(i1+5,i2  ,c2,c3)+c(i,6,0,0)*ui(i1+6,i2  ,c2,
     & c3)+c(i,7,0,0)*ui(i1+7,i2  ,c2,c3)+c(i,0,1,0)*ui(i1  ,i2+1,c2,
     & c3)+c(i,1,1,0)*ui(i1+1,i2+1,c2,c3)+c(i,2,1,0)*ui(i1+2,i2+1,c2,
     & c3)+c(i,3,1,0)*ui(i1+3,i2+1,c2,c3)+c(i,4,1,0)*ui(i1+4,i2+1,c2,
     & c3)+c(i,5,1,0)*ui(i1+5,i2+1,c2,c3)+c(i,6,1,0)*ui(i1+6,i2+1,c2,
     & c3)+c(i,7,1,0)*ui(i1+7,i2+1,c2,c3)+c(i,0,2,0)*ui(i1  ,i2+2,c2,
     & c3)+c(i,1,2,0)*ui(i1+1,i2+2,c2,c3)+c(i,2,2,0)*ui(i1+2,i2+2,c2,
     & c3)+c(i,3,2,0)*ui(i1+3,i2+2,c2,c3)+c(i,4,2,0)*ui(i1+4,i2+2,c2,
     & c3)+c(i,5,2,0)*ui(i1+5,i2+2,c2,c3)+c(i,6,2,0)*ui(i1+6,i2+2,c2,
     & c3)+c(i,7,2,0)*ui(i1+7,i2+2,c2,c3)+c(i,0,3,0)*ui(i1  ,i2+3,c2,
     & c3)+c(i,1,3,0)*ui(i1+1,i2+3,c2,c3)+c(i,2,3,0)*ui(i1+2,i2+3,c2,
     & c3)+c(i,3,3,0)*ui(i1+3,i2+3,c2,c3)+c(i,4,3,0)*ui(i1+4,i2+3,c2,
     & c3)+c(i,5,3,0)*ui(i1+5,i2+3,c2,c3)+c(i,6,3,0)*ui(i1+6,i2+3,c2,
     & c3)+c(i,7,3,0)*ui(i1+7,i2+3,c2,c3)+c(i,0,4,0)*ui(i1  ,i2+4,c2,
     & c3)+c(i,1,4,0)*ui(i1+1,i2+4,c2,c3)+c(i,2,4,0)*ui(i1+2,i2+4,c2,
     & c3)+c(i,3,4,0)*ui(i1+3,i2+4,c2,c3)+c(i,4,4,0)*ui(i1+4,i2+4,c2,
     & c3)+c(i,5,4,0)*ui(i1+5,i2+4,c2,c3)+c(i,6,4,0)*ui(i1+6,i2+4,c2,
     & c3)+c(i,7,4,0)*ui(i1+7,i2+4,c2,c3)+c(i,0,5,0)*ui(i1  ,i2+5,c2,
     & c3)+c(i,1,5,0)*ui(i1+1,i2+5,c2,c3)+c(i,2,5,0)*ui(i1+2,i2+5,c2,
     & c3)+c(i,3,5,0)*ui(i1+3,i2+5,c2,c3)+c(i,4,5,0)*ui(i1+4,i2+5,c2,
     & c3)+c(i,5,5,0)*ui(i1+5,i2+5,c2,c3)+c(i,6,5,0)*ui(i1+6,i2+5,c2,
     & c3)+c(i,7,5,0)*ui(i1+7,i2+5,c2,c3)+c(i,0,6,0)*ui(i1  ,i2+6,c2,
     & c3)+c(i,1,6,0)*ui(i1+1,i2+6,c2,c3)+c(i,2,6,0)*ui(i1+2,i2+6,c2,
     & c3)+c(i,3,6,0)*ui(i1+3,i2+6,c2,c3)+c(i,4,6,0)*ui(i1+4,i2+6,c2,
     & c3)+c(i,5,6,0)*ui(i1+5,i2+6,c2,c3)+c(i,6,6,0)*ui(i1+6,i2+6,c2,
     & c3)+c(i,7,6,0)*ui(i1+7,i2+6,c2,c3)+c(i,0,7,0)*ui(i1  ,i2+7,c2,
     & c3)+c(i,1,7,0)*ui(i1+1,i2+7,c2,c3)+c(i,2,7,0)*ui(i1+2,i2+7,c2,
     & c3)+c(i,3,7,0)*ui(i1+3,i2+7,c2,c3)+c(i,4,7,0)*ui(i1+4,i2+7,c2,
     & c3)+c(i,5,7,0)*ui(i1+5,i2+7,c2,c3)+c(i,6,7,0)*ui(i1+6,i2+7,c2,
     & c3)+c(i,7,7,0)*ui(i1+7,i2+7,c2,c3)


              end do
              end do
              end do
            else
              ! put "c" loop as inner loop, this seems to be faster
              do i=nia,nib
              do c3=c3a,c3b
              do c2=c2a,c2b
! interp88(ug(ip(i,1),ip(i,2),c2,c3))
                i1=il(i,1)
                i2=il(i,2)
                ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,0,0)*ui(i1  ,i2  ,c2,
     & c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,
     & c3)+c(i,3,0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,
     & c3)+c(i,5,0,0)*ui(i1+5,i2  ,c2,c3)+c(i,6,0,0)*ui(i1+6,i2  ,c2,
     & c3)+c(i,7,0,0)*ui(i1+7,i2  ,c2,c3)+c(i,0,1,0)*ui(i1  ,i2+1,c2,
     & c3)+c(i,1,1,0)*ui(i1+1,i2+1,c2,c3)+c(i,2,1,0)*ui(i1+2,i2+1,c2,
     & c3)+c(i,3,1,0)*ui(i1+3,i2+1,c2,c3)+c(i,4,1,0)*ui(i1+4,i2+1,c2,
     & c3)+c(i,5,1,0)*ui(i1+5,i2+1,c2,c3)+c(i,6,1,0)*ui(i1+6,i2+1,c2,
     & c3)+c(i,7,1,0)*ui(i1+7,i2+1,c2,c3)+c(i,0,2,0)*ui(i1  ,i2+2,c2,
     & c3)+c(i,1,2,0)*ui(i1+1,i2+2,c2,c3)+c(i,2,2,0)*ui(i1+2,i2+2,c2,
     & c3)+c(i,3,2,0)*ui(i1+3,i2+2,c2,c3)+c(i,4,2,0)*ui(i1+4,i2+2,c2,
     & c3)+c(i,5,2,0)*ui(i1+5,i2+2,c2,c3)+c(i,6,2,0)*ui(i1+6,i2+2,c2,
     & c3)+c(i,7,2,0)*ui(i1+7,i2+2,c2,c3)+c(i,0,3,0)*ui(i1  ,i2+3,c2,
     & c3)+c(i,1,3,0)*ui(i1+1,i2+3,c2,c3)+c(i,2,3,0)*ui(i1+2,i2+3,c2,
     & c3)+c(i,3,3,0)*ui(i1+3,i2+3,c2,c3)+c(i,4,3,0)*ui(i1+4,i2+3,c2,
     & c3)+c(i,5,3,0)*ui(i1+5,i2+3,c2,c3)+c(i,6,3,0)*ui(i1+6,i2+3,c2,
     & c3)+c(i,7,3,0)*ui(i1+7,i2+3,c2,c3)+c(i,0,4,0)*ui(i1  ,i2+4,c2,
     & c3)+c(i,1,4,0)*ui(i1+1,i2+4,c2,c3)+c(i,2,4,0)*ui(i1+2,i2+4,c2,
     & c3)+c(i,3,4,0)*ui(i1+3,i2+4,c2,c3)+c(i,4,4,0)*ui(i1+4,i2+4,c2,
     & c3)+c(i,5,4,0)*ui(i1+5,i2+4,c2,c3)+c(i,6,4,0)*ui(i1+6,i2+4,c2,
     & c3)+c(i,7,4,0)*ui(i1+7,i2+4,c2,c3)+c(i,0,5,0)*ui(i1  ,i2+5,c2,
     & c3)+c(i,1,5,0)*ui(i1+1,i2+5,c2,c3)+c(i,2,5,0)*ui(i1+2,i2+5,c2,
     & c3)+c(i,3,5,0)*ui(i1+3,i2+5,c2,c3)+c(i,4,5,0)*ui(i1+4,i2+5,c2,
     & c3)+c(i,5,5,0)*ui(i1+5,i2+5,c2,c3)+c(i,6,5,0)*ui(i1+6,i2+5,c2,
     & c3)+c(i,7,5,0)*ui(i1+7,i2+5,c2,c3)+c(i,0,6,0)*ui(i1  ,i2+6,c2,
     & c3)+c(i,1,6,0)*ui(i1+1,i2+6,c2,c3)+c(i,2,6,0)*ui(i1+2,i2+6,c2,
     & c3)+c(i,3,6,0)*ui(i1+3,i2+6,c2,c3)+c(i,4,6,0)*ui(i1+4,i2+6,c2,
     & c3)+c(i,5,6,0)*ui(i1+5,i2+6,c2,c3)+c(i,6,6,0)*ui(i1+6,i2+6,c2,
     & c3)+c(i,7,6,0)*ui(i1+7,i2+6,c2,c3)+c(i,0,7,0)*ui(i1  ,i2+7,c2,
     & c3)+c(i,1,7,0)*ui(i1+1,i2+7,c2,c3)+c(i,2,7,0)*ui(i1+2,i2+7,c2,
     & c3)+c(i,3,7,0)*ui(i1+3,i2+7,c2,c3)+c(i,4,7,0)*ui(i1+4,i2+7,c2,
     & c3)+c(i,5,7,0)*ui(i1+5,i2+7,c2,c3)+c(i,6,7,0)*ui(i1+6,i2+7,c2,
     & c3)+c(i,7,7,0)*ui(i1+7,i2+7,c2,c3)


              end do
              end do
              end do
            end if
          else if( width(1).eq.9 .and. width(2).eq.9 )then
! loops2d($interp99(ug(ip(i,1),ip(i,2),c2,c3)),,)
            if( c2a.eq.c2b .and. c3a.eq.c3b )then
              do c3=c3a,c3b
              do c2=c2a,c2b
              do i=nia,nib
! interp99(ug(ip(i,1),ip(i,2),c2,c3))
                i1=il(i,1)
                i2=il(i,2)
                ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,0,0)*ui(i1  ,i2  ,c2,
     & c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,
     & c3)+c(i,3,0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,
     & c3)+c(i,5,0,0)*ui(i1+5,i2  ,c2,c3)+c(i,6,0,0)*ui(i1+6,i2  ,c2,
     & c3)+c(i,7,0,0)*ui(i1+7,i2  ,c2,c3)+c(i,8,0,0)*ui(i1+8,i2  ,c2,
     & c3)+c(i,0,1,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,1,0)*ui(i1+1,i2+1,c2,
     & c3)+c(i,2,1,0)*ui(i1+2,i2+1,c2,c3)+c(i,3,1,0)*ui(i1+3,i2+1,c2,
     & c3)+c(i,4,1,0)*ui(i1+4,i2+1,c2,c3)+c(i,5,1,0)*ui(i1+5,i2+1,c2,
     & c3)+c(i,6,1,0)*ui(i1+6,i2+1,c2,c3)+c(i,7,1,0)*ui(i1+7,i2+1,c2,
     & c3)+c(i,8,1,0)*ui(i1+8,i2+1,c2,c3)+c(i,0,2,0)*ui(i1  ,i2+2,c2,
     & c3)+c(i,1,2,0)*ui(i1+1,i2+2,c2,c3)+c(i,2,2,0)*ui(i1+2,i2+2,c2,
     & c3)+c(i,3,2,0)*ui(i1+3,i2+2,c2,c3)+c(i,4,2,0)*ui(i1+4,i2+2,c2,
     & c3)+c(i,5,2,0)*ui(i1+5,i2+2,c2,c3)+c(i,6,2,0)*ui(i1+6,i2+2,c2,
     & c3)+c(i,7,2,0)*ui(i1+7,i2+2,c2,c3)+c(i,8,2,0)*ui(i1+8,i2+2,c2,
     & c3)+c(i,0,3,0)*ui(i1  ,i2+3,c2,c3)+c(i,1,3,0)*ui(i1+1,i2+3,c2,
     & c3)+c(i,2,3,0)*ui(i1+2,i2+3,c2,c3)+c(i,3,3,0)*ui(i1+3,i2+3,c2,
     & c3)+c(i,4,3,0)*ui(i1+4,i2+3,c2,c3)+c(i,5,3,0)*ui(i1+5,i2+3,c2,
     & c3)+c(i,6,3,0)*ui(i1+6,i2+3,c2,c3)+c(i,7,3,0)*ui(i1+7,i2+3,c2,
     & c3)+c(i,8,3,0)*ui(i1+8,i2+3,c2,c3)+c(i,0,4,0)*ui(i1  ,i2+4,c2,
     & c3)+c(i,1,4,0)*ui(i1+1,i2+4,c2,c3)+c(i,2,4,0)*ui(i1+2,i2+4,c2,
     & c3)+c(i,3,4,0)*ui(i1+3,i2+4,c2,c3)+c(i,4,4,0)*ui(i1+4,i2+4,c2,
     & c3)+c(i,5,4,0)*ui(i1+5,i2+4,c2,c3)+c(i,6,4,0)*ui(i1+6,i2+4,c2,
     & c3)+c(i,7,4,0)*ui(i1+7,i2+4,c2,c3)+c(i,8,4,0)*ui(i1+8,i2+4,c2,
     & c3)+c(i,0,5,0)*ui(i1  ,i2+5,c2,c3)+c(i,1,5,0)*ui(i1+1,i2+5,c2,
     & c3)+c(i,2,5,0)*ui(i1+2,i2+5,c2,c3)+c(i,3,5,0)*ui(i1+3,i2+5,c2,
     & c3)+c(i,4,5,0)*ui(i1+4,i2+5,c2,c3)+c(i,5,5,0)*ui(i1+5,i2+5,c2,
     & c3)+c(i,6,5,0)*ui(i1+6,i2+5,c2,c3)+c(i,7,5,0)*ui(i1+7,i2+5,c2,
     & c3)+c(i,8,5,0)*ui(i1+8,i2+5,c2,c3)+c(i,0,6,0)*ui(i1  ,i2+6,c2,
     & c3)+c(i,1,6,0)*ui(i1+1,i2+6,c2,c3)+c(i,2,6,0)*ui(i1+2,i2+6,c2,
     & c3)+c(i,3,6,0)*ui(i1+3,i2+6,c2,c3)+c(i,4,6,0)*ui(i1+4,i2+6,c2,
     & c3)+c(i,5,6,0)*ui(i1+5,i2+6,c2,c3)+c(i,6,6,0)*ui(i1+6,i2+6,c2,
     & c3)+c(i,7,6,0)*ui(i1+7,i2+6,c2,c3)+c(i,8,6,0)*ui(i1+8,i2+6,c2,
     & c3)+c(i,0,7,0)*ui(i1  ,i2+7,c2,c3)+c(i,1,7,0)*ui(i1+1,i2+7,c2,
     & c3)+c(i,2,7,0)*ui(i1+2,i2+7,c2,c3)+c(i,3,7,0)*ui(i1+3,i2+7,c2,
     & c3)+c(i,4,7,0)*ui(i1+4,i2+7,c2,c3)+c(i,5,7,0)*ui(i1+5,i2+7,c2,
     & c3)+c(i,6,7,0)*ui(i1+6,i2+7,c2,c3)+c(i,7,7,0)*ui(i1+7,i2+7,c2,
     & c3)+c(i,8,7,0)*ui(i1+8,i2+7,c2,c3)+c(i,0,8,0)*ui(i1  ,i2+8,c2,
     & c3)+c(i,1,8,0)*ui(i1+1,i2+8,c2,c3)+c(i,2,8,0)*ui(i1+2,i2+8,c2,
     & c3)+c(i,3,8,0)*ui(i1+3,i2+8,c2,c3)+c(i,4,8,0)*ui(i1+4,i2+8,c2,
     & c3)+c(i,5,8,0)*ui(i1+5,i2+8,c2,c3)+c(i,6,8,0)*ui(i1+6,i2+8,c2,
     & c3)+c(i,7,8,0)*ui(i1+7,i2+8,c2,c3)+c(i,8,8,0)*ui(i1+8,i2+8,c2,
     & c3)


              end do
              end do
              end do
            else
              ! put "c" loop as inner loop, this seems to be faster
              do i=nia,nib
              do c3=c3a,c3b
              do c2=c2a,c2b
! interp99(ug(ip(i,1),ip(i,2),c2,c3))
                i1=il(i,1)
                i2=il(i,2)
                ug(ip(i,1),ip(i,2),c2,c3) = c(i,0,0,0)*ui(i1  ,i2  ,c2,
     & c3)+c(i,1,0,0)*ui(i1+1,i2  ,c2,c3)+c(i,2,0,0)*ui(i1+2,i2  ,c2,
     & c3)+c(i,3,0,0)*ui(i1+3,i2  ,c2,c3)+c(i,4,0,0)*ui(i1+4,i2  ,c2,
     & c3)+c(i,5,0,0)*ui(i1+5,i2  ,c2,c3)+c(i,6,0,0)*ui(i1+6,i2  ,c2,
     & c3)+c(i,7,0,0)*ui(i1+7,i2  ,c2,c3)+c(i,8,0,0)*ui(i1+8,i2  ,c2,
     & c3)+c(i,0,1,0)*ui(i1  ,i2+1,c2,c3)+c(i,1,1,0)*ui(i1+1,i2+1,c2,
     & c3)+c(i,2,1,0)*ui(i1+2,i2+1,c2,c3)+c(i,3,1,0)*ui(i1+3,i2+1,c2,
     & c3)+c(i,4,1,0)*ui(i1+4,i2+1,c2,c3)+c(i,5,1,0)*ui(i1+5,i2+1,c2,
     & c3)+c(i,6,1,0)*ui(i1+6,i2+1,c2,c3)+c(i,7,1,0)*ui(i1+7,i2+1,c2,
     & c3)+c(i,8,1,0)*ui(i1+8,i2+1,c2,c3)+c(i,0,2,0)*ui(i1  ,i2+2,c2,
     & c3)+c(i,1,2,0)*ui(i1+1,i2+2,c2,c3)+c(i,2,2,0)*ui(i1+2,i2+2,c2,
     & c3)+c(i,3,2,0)*ui(i1+3,i2+2,c2,c3)+c(i,4,2,0)*ui(i1+4,i2+2,c2,
     & c3)+c(i,5,2,0)*ui(i1+5,i2+2,c2,c3)+c(i,6,2,0)*ui(i1+6,i2+2,c2,
     & c3)+c(i,7,2,0)*ui(i1+7,i2+2,c2,c3)+c(i,8,2,0)*ui(i1+8,i2+2,c2,
     & c3)+c(i,0,3,0)*ui(i1  ,i2+3,c2,c3)+c(i,1,3,0)*ui(i1+1,i2+3,c2,
     & c3)+c(i,2,3,0)*ui(i1+2,i2+3,c2,c3)+c(i,3,3,0)*ui(i1+3,i2+3,c2,
     & c3)+c(i,4,3,0)*ui(i1+4,i2+3,c2,c3)+c(i,5,3,0)*ui(i1+5,i2+3,c2,
     & c3)+c(i,6,3,0)*ui(i1+6,i2+3,c2,c3)+c(i,7,3,0)*ui(i1+7,i2+3,c2,
     & c3)+c(i,8,3,0)*ui(i1+8,i2+3,c2,c3)+c(i,0,4,0)*ui(i1  ,i2+4,c2,
     & c3)+c(i,1,4,0)*ui(i1+1,i2+4,c2,c3)+c(i,2,4,0)*ui(i1+2,i2+4,c2,
     & c3)+c(i,3,4,0)*ui(i1+3,i2+4,c2,c3)+c(i,4,4,0)*ui(i1+4,i2+4,c2,
     & c3)+c(i,5,4,0)*ui(i1+5,i2+4,c2,c3)+c(i,6,4,0)*ui(i1+6,i2+4,c2,
     & c3)+c(i,7,4,0)*ui(i1+7,i2+4,c2,c3)+c(i,8,4,0)*ui(i1+8,i2+4,c2,
     & c3)+c(i,0,5,0)*ui(i1  ,i2+5,c2,c3)+c(i,1,5,0)*ui(i1+1,i2+5,c2,
     & c3)+c(i,2,5,0)*ui(i1+2,i2+5,c2,c3)+c(i,3,5,0)*ui(i1+3,i2+5,c2,
     & c3)+c(i,4,5,0)*ui(i1+4,i2+5,c2,c3)+c(i,5,5,0)*ui(i1+5,i2+5,c2,
     & c3)+c(i,6,5,0)*ui(i1+6,i2+5,c2,c3)+c(i,7,5,0)*ui(i1+7,i2+5,c2,
     & c3)+c(i,8,5,0)*ui(i1+8,i2+5,c2,c3)+c(i,0,6,0)*ui(i1  ,i2+6,c2,
     & c3)+c(i,1,6,0)*ui(i1+1,i2+6,c2,c3)+c(i,2,6,0)*ui(i1+2,i2+6,c2,
     & c3)+c(i,3,6,0)*ui(i1+3,i2+6,c2,c3)+c(i,4,6,0)*ui(i1+4,i2+6,c2,
     & c3)+c(i,5,6,0)*ui(i1+5,i2+6,c2,c3)+c(i,6,6,0)*ui(i1+6,i2+6,c2,
     & c3)+c(i,7,6,0)*ui(i1+7,i2+6,c2,c3)+c(i,8,6,0)*ui(i1+8,i2+6,c2,
     & c3)+c(i,0,7,0)*ui(i1  ,i2+7,c2,c3)+c(i,1,7,0)*ui(i1+1,i2+7,c2,
     & c3)+c(i,2,7,0)*ui(i1+2,i2+7,c2,c3)+c(i,3,7,0)*ui(i1+3,i2+7,c2,
     & c3)+c(i,4,7,0)*ui(i1+4,i2+7,c2,c3)+c(i,5,7,0)*ui(i1+5,i2+7,c2,
     & c3)+c(i,6,7,0)*ui(i1+6,i2+7,c2,c3)+c(i,7,7,0)*ui(i1+7,i2+7,c2,
     & c3)+c(i,8,7,0)*ui(i1+8,i2+7,c2,c3)+c(i,0,8,0)*ui(i1  ,i2+8,c2,
     & c3)+c(i,1,8,0)*ui(i1+1,i2+8,c2,c3)+c(i,2,8,0)*ui(i1+2,i2+8,c2,
     & c3)+c(i,3,8,0)*ui(i1+3,i2+8,c2,c3)+c(i,4,8,0)*ui(i1+4,i2+8,c2,
     & c3)+c(i,5,8,0)*ui(i1+5,i2+8,c2,c3)+c(i,6,8,0)*ui(i1+6,i2+8,c2,
     & c3)+c(i,7,8,0)*ui(i1+7,i2+8,c2,c3)+c(i,8,8,0)*ui(i1+8,i2+8,c2,
     & c3)


              end do
              end do
              end do
            end if
          else
c           general case in 2D
c write(*,*)'interpOpt:WARNING:Gen case width=',width(1),width(2)
            do c3=c3a,c3b
              do c2=c2a,c2b
                do i=nia,nib
                  ug(ip(i,1),ip(i,2),c2,c3)=0.
                end do
                do w2=0,width(2)-1
                  do w1=0,width(1)-1
                    do i=nia,nib
                      ug(ip(i,1),ip(i,2),c2,c3)=ug(ip(i,1),ip(i,2),c2,
     & c3)+c(i,w1,w2,0)*ui(il(i,1)+w1,il(i,2)+w2,c2,c3)
                    end do
                  end do
                end do
              end do
            end do
          end if
        else
c     *** 3D ****
         if( useVariableWidthInterpolation.ne.0 )then
! beginLoops3d()
             do i=nia,nib
             do c3=c3a,c3b
             ! check for most common widths first
             if( varWidth(i).eq.3 )then
! interp333(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,0,0)*ui(i1,i2,i3,
     & c3)+c(i,1,0,0)*ui(i1+1,i2,i3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3,c3)+
     & c(i,0,1,0)*ui(i1,i2+1,i3,c3)+c(i,1,1,0)*ui(i1+1,i2+1,i3,c3)+c(
     & i,2,1,0)*ui(i1+2,i2+1,i3,c3)+c(i,0,2,0)*ui(i1,i2+2,i3,c3)+c(i,
     & 1,2,0)*ui(i1+1,i2+2,i3,c3)+c(i,2,2,0)*ui(i1+2,i2+2,i3,c3)+c(i,
     & 0,0,1)*ui(i1,i2,i3+1,c3)+c(i,1,0,1)*ui(i1+1,i2,i3+1,c3)+c(i,2,
     & 0,1)*ui(i1+2,i2,i3+1,c3)+c(i,0,1,1)*ui(i1,i2+1,i3+1,c3)+c(i,1,
     & 1,1)*ui(i1+1,i2+1,i3+1,c3)+c(i,2,1,1)*ui(i1+2,i2+1,i3+1,c3)+c(
     & i,0,2,1)*ui(i1,i2+2,i3+1,c3)+c(i,1,2,1)*ui(i1+1,i2+2,i3+1,c3)+
     & c(i,2,2,1)*ui(i1+2,i2+2,i3+1,c3)+c(i,0,0,2)*ui(i1,i2,i3+2,c3)+
     & c(i,1,0,2)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,2)*ui(i1+2,i2,i3+2,c3)+
     & c(i,0,1,2)*ui(i1,i2+1,i3+2,c3)+c(i,1,1,2)*ui(i1+1,i2+1,i3+2,c3)
     & +c(i,2,1,2)*ui(i1+2,i2+1,i3+2,c3)+c(i,0,2,2)*ui(i1,i2+2,i3+2,
     & c3)+c(i,1,2,2)*ui(i1+1,i2+2,i3+2,c3)+c(i,2,2,2)*ui(i1+2,i2+2,
     & i3+2,c3)
             else if( varWidth(i).eq.2 )then
! interp222(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,0,0)*ui(i1,i2,i3,
     & c3)+c(i,1,0,0)*ui(i1+1,i2,i3,c3)+c(i,0,1,0)*ui(i1,i2+1,i3,c3)+
     & c(i,1,1,0)*ui(i1+1,i2+1,i3,c3)+c(i,0,0,1)*ui(i1,i2,i3+1,c3)+c(
     & i,1,0,1)*ui(i1+1,i2,i3+1,c3)+c(i,0,1,1)*ui(i1,i2+1,i3+1,c3)+c(
     & i,1,1,1)*ui(i1+1,i2+1,i3+1,c3)
             else if( varWidth(i).eq.1 )then
! interp111(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ui(i1,i2,i3,c3)
             else if( varWidth(i).eq.5 )then
! interp555(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,0,0)*ui(i1,i2,i3,
     & c3)+c(i,1,0,0)*ui(i1+1,i2,i3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3,c3)+
     & c(i,3,0,0)*ui(i1+3,i2,i3,c3)+c(i,4,0,0)*ui(i1+4,i2,i3,c3)+c(i,
     & 0,1,0)*ui(i1,i2+1,i3,c3)+c(i,1,1,0)*ui(i1+1,i2+1,i3,c3)+c(i,2,
     & 1,0)*ui(i1+2,i2+1,i3,c3)+c(i,3,1,0)*ui(i1+3,i2+1,i3,c3)+c(i,4,
     & 1,0)*ui(i1+4,i2+1,i3,c3)+c(i,0,2,0)*ui(i1,i2+2,i3,c3)+c(i,1,2,
     & 0)*ui(i1+1,i2+2,i3,c3)+c(i,2,2,0)*ui(i1+2,i2+2,i3,c3)+c(i,3,2,
     & 0)*ui(i1+3,i2+2,i3,c3)+c(i,4,2,0)*ui(i1+4,i2+2,i3,c3)+c(i,0,3,
     & 0)*ui(i1,i2+3,i3,c3)+c(i,1,3,0)*ui(i1+1,i2+3,i3,c3)+c(i,2,3,0)*
     & ui(i1+2,i2+3,i3,c3)+c(i,3,3,0)*ui(i1+3,i2+3,i3,c3)+c(i,4,3,0)*
     & ui(i1+4,i2+3,i3,c3)+c(i,0,4,0)*ui(i1,i2+4,i3,c3)+c(i,1,4,0)*ui(
     & i1+1,i2+4,i3,c3)+c(i,2,4,0)*ui(i1+2,i2+4,i3,c3)+c(i,3,4,0)*ui(
     & i1+3,i2+4,i3,c3)+c(i,4,4,0)*ui(i1+4,i2+4,i3,c3)+c(i,0,0,1)*ui(
     & i1,i2,i3+1,c3)+c(i,1,0,1)*ui(i1+1,i2,i3+1,c3)+c(i,2,0,1)*ui(i1+
     & 2,i2,i3+1,c3)+c(i,3,0,1)*ui(i1+3,i2,i3+1,c3)+c(i,4,0,1)*ui(i1+
     & 4,i2,i3+1,c3)+c(i,0,1,1)*ui(i1,i2+1,i3+1,c3)+c(i,1,1,1)*ui(i1+
     & 1,i2+1,i3+1,c3)+c(i,2,1,1)*ui(i1+2,i2+1,i3+1,c3)+c(i,3,1,1)*ui(
     & i1+3,i2+1,i3+1,c3)+c(i,4,1,1)*ui(i1+4,i2+1,i3+1,c3)+c(i,0,2,1)*
     & ui(i1,i2+2,i3+1,c3)+c(i,1,2,1)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,2,1)
     & *ui(i1+2,i2+2,i3+1,c3)+c(i,3,2,1)*ui(i1+3,i2+2,i3+1,c3)+c(i,4,
     & 2,1)*ui(i1+4,i2+2,i3+1,c3)+c(i,0,3,1)*ui(i1,i2+3,i3+1,c3)+c(i,
     & 1,3,1)*ui(i1+1,i2+3,i3+1,c3)+c(i,2,3,1)*ui(i1+2,i2+3,i3+1,c3)+
     & c(i,3,3,1)*ui(i1+3,i2+3,i3+1,c3)+c(i,4,3,1)*ui(i1+4,i2+3,i3+1,
     & c3)+c(i,0,4,1)*ui(i1,i2+4,i3+1,c3)+c(i,1,4,1)*ui(i1+1,i2+4,i3+
     & 1,c3)+c(i,2,4,1)*ui(i1+2,i2+4,i3+1,c3)+c(i,3,4,1)*ui(i1+3,i2+4,
     & i3+1,c3)+c(i,4,4,1)*ui(i1+4,i2+4,i3+1,c3)+c(i,0,0,2)*ui(i1,i2,
     & i3+2,c3)+c(i,1,0,2)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,2)*ui(i1+2,i2,
     & i3+2,c3)+c(i,3,0,2)*ui(i1+3,i2,i3+2,c3)+c(i,4,0,2)*ui(i1+4,i2,
     & i3+2,c3)+c(i,0,1,2)*ui(i1,i2+1,i3+2,c3)+c(i,1,1,2)*ui(i1+1,i2+
     & 1,i3+2,c3)+c(i,2,1,2)*ui(i1+2,i2+1,i3+2,c3)+c(i,3,1,2)*ui(i1+3,
     & i2+1,i3+2,c3)+c(i,4,1,2)*ui(i1+4,i2+1,i3+2,c3)+c(i,0,2,2)*ui(
     & i1,i2+2,i3+2,c3)+c(i,1,2,2)*ui(i1+1,i2+2,i3+2,c3)+c(i,2,2,2)*
     & ui(i1+2,i2+2,i3+2,c3)+c(i,3,2,2)*ui(i1+3,i2+2,i3+2,c3)+c(i,4,2,
     & 2)*ui(i1+4,i2+2,i3+2,c3)+c(i,0,3,2)*ui(i1,i2+3,i3+2,c3)+c(i,1,
     & 3,2)*ui(i1+1,i2+3,i3+2,c3)+c(i,2,3,2)*ui(i1+2,i2+3,i3+2,c3)+c(
     & i,3,3,2)*ui(i1+3,i2+3,i3+2,c3)+c(i,4,3,2)*ui(i1+4,i2+3,i3+2,c3)
     & +c(i,0,4,2)*ui(i1,i2+4,i3+2,c3)+c(i,1,4,2)*ui(i1+1,i2+4,i3+2,
     & c3)+c(i,2,4,2)*ui(i1+2,i2+4,i3+2,c3)+c(i,3,4,2)*ui(i1+3,i2+4,
     & i3+2,c3)+c(i,4,4,2)*ui(i1+4,i2+4,i3+2,c3)+c(i,0,0,3)*ui(i1,i2,
     & i3+3,c3)+c(i,1,0,3)*ui(i1+1,i2,i3+3,c3)+c(i,2,0,3)*ui(i1+2,i2,
     & i3+3,c3)+c(i,3,0,3)*ui(i1+3,i2,i3+3,c3)+c(i,4,0,3)*ui(i1+4,i2,
     & i3+3,c3)+c(i,0,1,3)*ui(i1,i2+1,i3+3,c3)+c(i,1,1,3)*ui(i1+1,i2+
     & 1,i3+3,c3)+c(i,2,1,3)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,1,3)*ui(i1+3,
     & i2+1,i3+3,c3)+c(i,4,1,3)*ui(i1+4,i2+1,i3+3,c3)+c(i,0,2,3)*ui(
     & i1,i2+2,i3+3,c3)+c(i,1,2,3)*ui(i1+1,i2+2,i3+3,c3)+c(i,2,2,3)*
     & ui(i1+2,i2+2,i3+3,c3)+c(i,3,2,3)*ui(i1+3,i2+2,i3+3,c3)+c(i,4,2,
     & 3)*ui(i1+4,i2+2,i3+3,c3)+c(i,0,3,3)*ui(i1,i2+3,i3+3,c3)+c(i,1,
     & 3,3)*ui(i1+1,i2+3,i3+3,c3)+c(i,2,3,3)*ui(i1+2,i2+3,i3+3,c3)+c(
     & i,3,3,3)*ui(i1+3,i2+3,i3+3,c3)+c(i,4,3,3)*ui(i1+4,i2+3,i3+3,c3)
     & +c(i,0,4,3)*ui(i1,i2+4,i3+3,c3)+c(i,1,4,3)*ui(i1+1,i2+4,i3+3,
     & c3)+c(i,2,4,3)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,4,3)*ui(i1+3,i2+4,
     & i3+3,c3)+c(i,4,4,3)*ui(i1+4,i2+4,i3+3,c3)+c(i,0,0,4)*ui(i1,i2,
     & i3+4,c3)+c(i,1,0,4)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,4)*ui(i1+2,i2,
     & i3+4,c3)+c(i,3,0,4)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,4)*ui(i1+4,i2,
     & i3+4,c3)+c(i,0,1,4)*ui(i1,i2+1,i3+4,c3)+c(i,1,1,4)*ui(i1+1,i2+
     & 1,i3+4,c3)+c(i,2,1,4)*ui(i1+2,i2+1,i3+4,c3)+c(i,3,1,4)*ui(i1+3,
     & i2+1,i3+4,c3)+c(i,4,1,4)*ui(i1+4,i2+1,i3+4,c3)+c(i,0,2,4)*ui(
     & i1,i2+2,i3+4,c3)+c(i,1,2,4)*ui(i1+1,i2+2,i3+4,c3)+c(i,2,2,4)*
     & ui(i1+2,i2+2,i3+4,c3)+c(i,3,2,4)*ui(i1+3,i2+2,i3+4,c3)+c(i,4,2,
     & 4)*ui(i1+4,i2+2,i3+4,c3)+c(i,0,3,4)*ui(i1,i2+3,i3+4,c3)+c(i,1,
     & 3,4)*ui(i1+1,i2+3,i3+4,c3)+c(i,2,3,4)*ui(i1+2,i2+3,i3+4,c3)+c(
     & i,3,3,4)*ui(i1+3,i2+3,i3+4,c3)+c(i,4,3,4)*ui(i1+4,i2+3,i3+4,c3)
     & +c(i,0,4,4)*ui(i1,i2+4,i3+4,c3)+c(i,1,4,4)*ui(i1+1,i2+4,i3+4,
     & c3)+c(i,2,4,4)*ui(i1+2,i2+4,i3+4,c3)+c(i,3,4,4)*ui(i1+3,i2+4,
     & i3+4,c3)+c(i,4,4,4)*ui(i1+4,i2+4,i3+4,c3)
             else if( varWidth(i).eq.4 )then
! interp444(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,0,0)*ui(i1,i2,i3,
     & c3)+c(i,1,0,0)*ui(i1+1,i2,i3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3,c3)+
     & c(i,3,0,0)*ui(i1+3,i2,i3,c3)+c(i,0,1,0)*ui(i1,i2+1,i3,c3)+c(i,
     & 1,1,0)*ui(i1+1,i2+1,i3,c3)+c(i,2,1,0)*ui(i1+2,i2+1,i3,c3)+c(i,
     & 3,1,0)*ui(i1+3,i2+1,i3,c3)+c(i,0,2,0)*ui(i1,i2+2,i3,c3)+c(i,1,
     & 2,0)*ui(i1+1,i2+2,i3,c3)+c(i,2,2,0)*ui(i1+2,i2+2,i3,c3)+c(i,3,
     & 2,0)*ui(i1+3,i2+2,i3,c3)+c(i,0,3,0)*ui(i1,i2+3,i3,c3)+c(i,1,3,
     & 0)*ui(i1+1,i2+3,i3,c3)+c(i,2,3,0)*ui(i1+2,i2+3,i3,c3)+c(i,3,3,
     & 0)*ui(i1+3,i2+3,i3,c3)+c(i,0,0,1)*ui(i1,i2,i3+1,c3)+c(i,1,0,1)*
     & ui(i1+1,i2,i3+1,c3)+c(i,2,0,1)*ui(i1+2,i2,i3+1,c3)+c(i,3,0,1)*
     & ui(i1+3,i2,i3+1,c3)+c(i,0,1,1)*ui(i1,i2+1,i3+1,c3)+c(i,1,1,1)*
     & ui(i1+1,i2+1,i3+1,c3)+c(i,2,1,1)*ui(i1+2,i2+1,i3+1,c3)+c(i,3,1,
     & 1)*ui(i1+3,i2+1,i3+1,c3)+c(i,0,2,1)*ui(i1,i2+2,i3+1,c3)+c(i,1,
     & 2,1)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,2,1)*ui(i1+2,i2+2,i3+1,c3)+c(
     & i,3,2,1)*ui(i1+3,i2+2,i3+1,c3)+c(i,0,3,1)*ui(i1,i2+3,i3+1,c3)+
     & c(i,1,3,1)*ui(i1+1,i2+3,i3+1,c3)+c(i,2,3,1)*ui(i1+2,i2+3,i3+1,
     & c3)+c(i,3,3,1)*ui(i1+3,i2+3,i3+1,c3)+c(i,0,0,2)*ui(i1,i2,i3+2,
     & c3)+c(i,1,0,2)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,2)*ui(i1+2,i2,i3+2,
     & c3)+c(i,3,0,2)*ui(i1+3,i2,i3+2,c3)+c(i,0,1,2)*ui(i1,i2+1,i3+2,
     & c3)+c(i,1,1,2)*ui(i1+1,i2+1,i3+2,c3)+c(i,2,1,2)*ui(i1+2,i2+1,
     & i3+2,c3)+c(i,3,1,2)*ui(i1+3,i2+1,i3+2,c3)+c(i,0,2,2)*ui(i1,i2+
     & 2,i3+2,c3)+c(i,1,2,2)*ui(i1+1,i2+2,i3+2,c3)+c(i,2,2,2)*ui(i1+2,
     & i2+2,i3+2,c3)+c(i,3,2,2)*ui(i1+3,i2+2,i3+2,c3)+c(i,0,3,2)*ui(
     & i1,i2+3,i3+2,c3)+c(i,1,3,2)*ui(i1+1,i2+3,i3+2,c3)+c(i,2,3,2)*
     & ui(i1+2,i2+3,i3+2,c3)+c(i,3,3,2)*ui(i1+3,i2+3,i3+2,c3)+c(i,0,0,
     & 3)*ui(i1,i2,i3+3,c3)+c(i,1,0,3)*ui(i1+1,i2,i3+3,c3)+c(i,2,0,3)*
     & ui(i1+2,i2,i3+3,c3)+c(i,3,0,3)*ui(i1+3,i2,i3+3,c3)+c(i,0,1,3)*
     & ui(i1,i2+1,i3+3,c3)+c(i,1,1,3)*ui(i1+1,i2+1,i3+3,c3)+c(i,2,1,3)
     & *ui(i1+2,i2+1,i3+3,c3)+c(i,3,1,3)*ui(i1+3,i2+1,i3+3,c3)+c(i,0,
     & 2,3)*ui(i1,i2+2,i3+3,c3)+c(i,1,2,3)*ui(i1+1,i2+2,i3+3,c3)+c(i,
     & 2,2,3)*ui(i1+2,i2+2,i3+3,c3)+c(i,3,2,3)*ui(i1+3,i2+2,i3+3,c3)+
     & c(i,0,3,3)*ui(i1,i2+3,i3+3,c3)+c(i,1,3,3)*ui(i1+1,i2+3,i3+3,c3)
     & +c(i,2,3,3)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,3,3)*ui(i1+3,i2+3,i3+3,
     & c3)
             else if( varWidth(i).eq.7 )then
! interp777(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,0,0)*ui(i1,i2,i3,
     & c3)+c(i,1,0,0)*ui(i1+1,i2,i3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3,c3)+
     & c(i,3,0,0)*ui(i1+3,i2,i3,c3)+c(i,4,0,0)*ui(i1+4,i2,i3,c3)+c(i,
     & 5,0,0)*ui(i1+5,i2,i3,c3)+c(i,6,0,0)*ui(i1+6,i2,i3,c3)+c(i,0,1,
     & 0)*ui(i1,i2+1,i3,c3)+c(i,1,1,0)*ui(i1+1,i2+1,i3,c3)+c(i,2,1,0)*
     & ui(i1+2,i2+1,i3,c3)+c(i,3,1,0)*ui(i1+3,i2+1,i3,c3)+c(i,4,1,0)*
     & ui(i1+4,i2+1,i3,c3)+c(i,5,1,0)*ui(i1+5,i2+1,i3,c3)+c(i,6,1,0)*
     & ui(i1+6,i2+1,i3,c3)+c(i,0,2,0)*ui(i1,i2+2,i3,c3)+c(i,1,2,0)*ui(
     & i1+1,i2+2,i3,c3)+c(i,2,2,0)*ui(i1+2,i2+2,i3,c3)+c(i,3,2,0)*ui(
     & i1+3,i2+2,i3,c3)+c(i,4,2,0)*ui(i1+4,i2+2,i3,c3)+c(i,5,2,0)*ui(
     & i1+5,i2+2,i3,c3)+c(i,6,2,0)*ui(i1+6,i2+2,i3,c3)+c(i,0,3,0)*ui(
     & i1,i2+3,i3,c3)+c(i,1,3,0)*ui(i1+1,i2+3,i3,c3)+c(i,2,3,0)*ui(i1+
     & 2,i2+3,i3,c3)+c(i,3,3,0)*ui(i1+3,i2+3,i3,c3)+c(i,4,3,0)*ui(i1+
     & 4,i2+3,i3,c3)+c(i,5,3,0)*ui(i1+5,i2+3,i3,c3)+c(i,6,3,0)*ui(i1+
     & 6,i2+3,i3,c3)+c(i,0,4,0)*ui(i1,i2+4,i3,c3)+c(i,1,4,0)*ui(i1+1,
     & i2+4,i3,c3)+c(i,2,4,0)*ui(i1+2,i2+4,i3,c3)+c(i,3,4,0)*ui(i1+3,
     & i2+4,i3,c3)+c(i,4,4,0)*ui(i1+4,i2+4,i3,c3)+c(i,5,4,0)*ui(i1+5,
     & i2+4,i3,c3)+c(i,6,4,0)*ui(i1+6,i2+4,i3,c3)+c(i,0,5,0)*ui(i1,i2+
     & 5,i3,c3)+c(i,1,5,0)*ui(i1+1,i2+5,i3,c3)+c(i,2,5,0)*ui(i1+2,i2+
     & 5,i3,c3)+c(i,3,5,0)*ui(i1+3,i2+5,i3,c3)+c(i,4,5,0)*ui(i1+4,i2+
     & 5,i3,c3)+c(i,5,5,0)*ui(i1+5,i2+5,i3,c3)+c(i,6,5,0)*ui(i1+6,i2+
     & 5,i3,c3)+c(i,0,6,0)*ui(i1,i2+6,i3,c3)+c(i,1,6,0)*ui(i1+1,i2+6,
     & i3,c3)+c(i,2,6,0)*ui(i1+2,i2+6,i3,c3)+c(i,3,6,0)*ui(i1+3,i2+6,
     & i3,c3)+c(i,4,6,0)*ui(i1+4,i2+6,i3,c3)+c(i,5,6,0)*ui(i1+5,i2+6,
     & i3,c3)+c(i,6,6,0)*ui(i1+6,i2+6,i3,c3)+c(i,0,0,1)*ui(i1,i2,i3+1,
     & c3)+c(i,1,0,1)*ui(i1+1,i2,i3+1,c3)+c(i,2,0,1)*ui(i1+2,i2,i3+1,
     & c3)+c(i,3,0,1)*ui(i1+3,i2,i3+1,c3)+c(i,4,0,1)*ui(i1+4,i2,i3+1,
     & c3)+c(i,5,0,1)*ui(i1+5,i2,i3+1,c3)+c(i,6,0,1)*ui(i1+6,i2,i3+1,
     & c3)+c(i,0,1,1)*ui(i1,i2+1,i3+1,c3)+c(i,1,1,1)*ui(i1+1,i2+1,i3+
     & 1,c3)+c(i,2,1,1)*ui(i1+2,i2+1,i3+1,c3)+c(i,3,1,1)*ui(i1+3,i2+1,
     & i3+1,c3)+c(i,4,1,1)*ui(i1+4,i2+1,i3+1,c3)+c(i,5,1,1)*ui(i1+5,
     & i2+1,i3+1,c3)+c(i,6,1,1)*ui(i1+6,i2+1,i3+1,c3)+c(i,0,2,1)*ui(
     & i1,i2+2,i3+1,c3)+c(i,1,2,1)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,2,1)*
     & ui(i1+2,i2+2,i3+1,c3)+c(i,3,2,1)*ui(i1+3,i2+2,i3+1,c3)+c(i,4,2,
     & 1)*ui(i1+4,i2+2,i3+1,c3)+c(i,5,2,1)*ui(i1+5,i2+2,i3+1,c3)+c(i,
     & 6,2,1)*ui(i1+6,i2+2,i3+1,c3)+c(i,0,3,1)*ui(i1,i2+3,i3+1,c3)+c(
     & i,1,3,1)*ui(i1+1,i2+3,i3+1,c3)+c(i,2,3,1)*ui(i1+2,i2+3,i3+1,c3)
     & +c(i,3,3,1)*ui(i1+3,i2+3,i3+1,c3)+c(i,4,3,1)*ui(i1+4,i2+3,i3+1,
     & c3)+c(i,5,3,1)*ui(i1+5,i2+3,i3+1,c3)+c(i,6,3,1)*ui(i1+6,i2+3,
     & i3+1,c3)+c(i,0,4,1)*ui(i1,i2+4,i3+1,c3)+c(i,1,4,1)*ui(i1+1,i2+
     & 4,i3+1,c3)+c(i,2,4,1)*ui(i1+2,i2+4,i3+1,c3)+c(i,3,4,1)*ui(i1+3,
     & i2+4,i3+1,c3)+c(i,4,4,1)*ui(i1+4,i2+4,i3+1,c3)+c(i,5,4,1)*ui(
     & i1+5,i2+4,i3+1,c3)+c(i,6,4,1)*ui(i1+6,i2+4,i3+1,c3)+c(i,0,5,1)*
     & ui(i1,i2+5,i3+1,c3)+c(i,1,5,1)*ui(i1+1,i2+5,i3+1,c3)+c(i,2,5,1)
     & *ui(i1+2,i2+5,i3+1,c3)+c(i,3,5,1)*ui(i1+3,i2+5,i3+1,c3)+c(i,4,
     & 5,1)*ui(i1+4,i2+5,i3+1,c3)+c(i,5,5,1)*ui(i1+5,i2+5,i3+1,c3)+c(
     & i,6,5,1)*ui(i1+6,i2+5,i3+1,c3)+c(i,0,6,1)*ui(i1,i2+6,i3+1,c3)+
     & c(i,1,6,1)*ui(i1+1,i2+6,i3+1,c3)+c(i,2,6,1)*ui(i1+2,i2+6,i3+1,
     & c3)+c(i,3,6,1)*ui(i1+3,i2+6,i3+1,c3)+c(i,4,6,1)*ui(i1+4,i2+6,
     & i3+1,c3)+c(i,5,6,1)*ui(i1+5,i2+6,i3+1,c3)+c(i,6,6,1)*ui(i1+6,
     & i2+6,i3+1,c3)+c(i,0,0,2)*ui(i1,i2,i3+2,c3)+c(i,1,0,2)*ui(i1+1,
     & i2,i3+2,c3)+c(i,2,0,2)*ui(i1+2,i2,i3+2,c3)+c(i,3,0,2)*ui(i1+3,
     & i2,i3+2,c3)+c(i,4,0,2)*ui(i1+4,i2,i3+2,c3)+c(i,5,0,2)*ui(i1+5,
     & i2,i3+2,c3)+c(i,6,0,2)*ui(i1+6,i2,i3+2,c3)+c(i,0,1,2)*ui(i1,i2+
     & 1,i3+2,c3)+c(i,1,1,2)*ui(i1+1,i2+1,i3+2,c3)+c(i,2,1,2)*ui(i1+2,
     & i2+1,i3+2,c3)+c(i,3,1,2)*ui(i1+3,i2+1,i3+2,c3)+c(i,4,1,2)*ui(
     & i1+4,i2+1,i3+2,c3)+c(i,5,1,2)*ui(i1+5,i2+1,i3+2,c3)+c(i,6,1,2)*
     & ui(i1+6,i2+1,i3+2,c3)+c(i,0,2,2)*ui(i1,i2+2,i3+2,c3)+c(i,1,2,2)
     & *ui(i1+1,i2+2,i3+2,c3)+c(i,2,2,2)*ui(i1+2,i2+2,i3+2,c3)+c(i,3,
     & 2,2)*ui(i1+3,i2+2,i3+2,c3)+c(i,4,2,2)*ui(i1+4,i2+2,i3+2,c3)+c(
     & i,5,2,2)*ui(i1+5,i2+2,i3+2,c3)+c(i,6,2,2)*ui(i1+6,i2+2,i3+2,c3)
     & +c(i,0,3,2)*ui(i1,i2+3,i3+2,c3)+c(i,1,3,2)*ui(i1+1,i2+3,i3+2,
     & c3)+c(i,2,3,2)*ui(i1+2,i2+3,i3+2,c3)+c(i,3,3,2)*ui(i1+3,i2+3,
     & i3+2,c3)+c(i,4,3,2)*ui(i1+4,i2+3,i3+2,c3)+c(i,5,3,2)*ui(i1+5,
     & i2+3,i3+2,c3)+c(i,6,3,2)*ui(i1+6,i2+3,i3+2,c3)+c(i,0,4,2)*ui(
     & i1,i2+4,i3+2,c3)+c(i,1,4,2)*ui(i1+1,i2+4,i3+2,c3)+c(i,2,4,2)*
     & ui(i1+2,i2+4,i3+2,c3)+c(i,3,4,2)*ui(i1+3,i2+4,i3+2,c3)+c(i,4,4,
     & 2)*ui(i1+4,i2+4,i3+2,c3)+c(i,5,4,2)*ui(i1+5,i2+4,i3+2,c3)+c(i,
     & 6,4,2)*ui(i1+6,i2+4,i3+2,c3)+c(i,0,5,2)*ui(i1,i2+5,i3+2,c3)+c(
     & i,1,5,2)*ui(i1+1,i2+5,i3+2,c3)+c(i,2,5,2)*ui(i1+2,i2+5,i3+2,c3)
     & +c(i,3,5,2)*ui(i1+3,i2+5,i3+2,c3)+c(i,4,5,2)*ui(i1+4,i2+5,i3+2,
     & c3)+c(i,5,5,2)*ui(i1+5,i2+5,i3+2,c3)+c(i,6,5,2)*ui(i1+6,i2+5,
     & i3+2,c3)+c(i,0,6,2)*ui(i1,i2+6,i3+2,c3)+c(i,1,6,2)*ui(i1+1,i2+
     & 6,i3+2,c3)+c(i,2,6,2)*ui(i1+2,i2+6,i3+2,c3)+c(i,3,6,2)*ui(i1+3,
     & i2+6,i3+2,c3)+c(i,4,6,2)*ui(i1+4,i2+6,i3+2,c3)+c(i,5,6,2)*ui(
     & i1+5,i2+6,i3+2,c3)+c(i,6,6,2)*ui(i1+6,i2+6,i3+2,c3)+c(i,0,0,3)*
     & ui(i1,i2,i3+3,c3)+c(i,1,0,3)*ui(i1+1,i2,i3+3,c3)+c(i,2,0,3)*ui(
     & i1+2,i2,i3+3,c3)+c(i,3,0,3)*ui(i1+3,i2,i3+3,c3)+c(i,4,0,3)*ui(
     & i1+4,i2,i3+3,c3)+c(i,5,0,3)*ui(i1+5,i2,i3+3,c3)+c(i,6,0,3)*ui(
     & i1+6,i2,i3+3,c3)+c(i,0,1,3)*ui(i1,i2+1,i3+3,c3)+c(i,1,1,3)*ui(
     & i1+1,i2+1,i3+3,c3)+c(i,2,1,3)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,1,3)*
     & ui(i1+3,i2+1,i3+3,c3)+c(i,4,1,3)*ui(i1+4,i2+1,i3+3,c3)+c(i,5,1,
     & 3)*ui(i1+5,i2+1,i3+3,c3)+c(i,6,1,3)*ui(i1+6,i2+1,i3+3,c3)+c(i,
     & 0,2,3)*ui(i1,i2+2,i3+3,c3)+c(i,1,2,3)*ui(i1+1,i2+2,i3+3,c3)+c(
     & i,2,2,3)*ui(i1+2,i2+2,i3+3,c3)+c(i,3,2,3)*ui(i1+3,i2+2,i3+3,c3)
     & +c(i,4,2,3)*ui(i1+4,i2+2,i3+3,c3)+c(i,5,2,3)*ui(i1+5,i2+2,i3+3,
     & c3)+c(i,6,2,3)*ui(i1+6,i2+2,i3+3,c3)+c(i,0,3,3)*ui(i1,i2+3,i3+
     & 3,c3)+c(i,1,3,3)*ui(i1+1,i2+3,i3+3,c3)+c(i,2,3,3)*ui(i1+2,i2+3,
     & i3+3,c3)+c(i,3,3,3)*ui(i1+3,i2+3,i3+3,c3)+c(i,4,3,3)*ui(i1+4,
     & i2+3,i3+3,c3)+c(i,5,3,3)*ui(i1+5,i2+3,i3+3,c3)+c(i,6,3,3)*ui(
     & i1+6,i2+3,i3+3,c3)+c(i,0,4,3)*ui(i1,i2+4,i3+3,c3)+c(i,1,4,3)*
     & ui(i1+1,i2+4,i3+3,c3)+c(i,2,4,3)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,4,
     & 3)*ui(i1+3,i2+4,i3+3,c3)+c(i,4,4,3)*ui(i1+4,i2+4,i3+3,c3)+c(i,
     & 5,4,3)*ui(i1+5,i2+4,i3+3,c3)+c(i,6,4,3)*ui(i1+6,i2+4,i3+3,c3)+
     & c(i,0,5,3)*ui(i1,i2+5,i3+3,c3)+c(i,1,5,3)*ui(i1+1,i2+5,i3+3,c3)
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,2,5,3)*ui(i1+2,i2+5,i3+3,c3)+c(i,3,5,3)*ui(i1+3,
     & i2+5,i3+3,c3)+c(i,4,5,3)*ui(i1+4,i2+5,i3+3,c3)+c(i,5,5,3)*ui(
     & i1+5,i2+5,i3+3,c3)+c(i,6,5,3)*ui(i1+6,i2+5,i3+3,c3)+c(i,0,6,3)*
     & ui(i1,i2+6,i3+3,c3)+c(i,1,6,3)*ui(i1+1,i2+6,i3+3,c3)+c(i,2,6,3)
     & *ui(i1+2,i2+6,i3+3,c3)+c(i,3,6,3)*ui(i1+3,i2+6,i3+3,c3)+c(i,4,
     & 6,3)*ui(i1+4,i2+6,i3+3,c3)+c(i,5,6,3)*ui(i1+5,i2+6,i3+3,c3)+c(
     & i,6,6,3)*ui(i1+6,i2+6,i3+3,c3)+c(i,0,0,4)*ui(i1,i2,i3+4,c3)+c(
     & i,1,0,4)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,4)*ui(i1+2,i2,i3+4,c3)+c(
     & i,3,0,4)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,4)*ui(i1+4,i2,i3+4,c3)+c(
     & i,5,0,4)*ui(i1+5,i2,i3+4,c3)+c(i,6,0,4)*ui(i1+6,i2,i3+4,c3)+c(
     & i,0,1,4)*ui(i1,i2+1,i3+4,c3)+c(i,1,1,4)*ui(i1+1,i2+1,i3+4,c3)+
     & c(i,2,1,4)*ui(i1+2,i2+1,i3+4,c3)+c(i,3,1,4)*ui(i1+3,i2+1,i3+4,
     & c3)+c(i,4,1,4)*ui(i1+4,i2+1,i3+4,c3)+c(i,5,1,4)*ui(i1+5,i2+1,
     & i3+4,c3)+c(i,6,1,4)*ui(i1+6,i2+1,i3+4,c3)+c(i,0,2,4)*ui(i1,i2+
     & 2,i3+4,c3)+c(i,1,2,4)*ui(i1+1,i2+2,i3+4,c3)+c(i,2,2,4)*ui(i1+2,
     & i2+2,i3+4,c3)+c(i,3,2,4)*ui(i1+3,i2+2,i3+4,c3)+c(i,4,2,4)*ui(
     & i1+4,i2+2,i3+4,c3)+c(i,5,2,4)*ui(i1+5,i2+2,i3+4,c3)+c(i,6,2,4)*
     & ui(i1+6,i2+2,i3+4,c3)+c(i,0,3,4)*ui(i1,i2+3,i3+4,c3)+c(i,1,3,4)
     & *ui(i1+1,i2+3,i3+4,c3)+c(i,2,3,4)*ui(i1+2,i2+3,i3+4,c3)+c(i,3,
     & 3,4)*ui(i1+3,i2+3,i3+4,c3)+c(i,4,3,4)*ui(i1+4,i2+3,i3+4,c3)+c(
     & i,5,3,4)*ui(i1+5,i2+3,i3+4,c3)+c(i,6,3,4)*ui(i1+6,i2+3,i3+4,c3)
     & +c(i,0,4,4)*ui(i1,i2+4,i3+4,c3)+c(i,1,4,4)*ui(i1+1,i2+4,i3+4,
     & c3)+c(i,2,4,4)*ui(i1+2,i2+4,i3+4,c3)+c(i,3,4,4)*ui(i1+3,i2+4,
     & i3+4,c3)+c(i,4,4,4)*ui(i1+4,i2+4,i3+4,c3)+c(i,5,4,4)*ui(i1+5,
     & i2+4,i3+4,c3)+c(i,6,4,4)*ui(i1+6,i2+4,i3+4,c3)+c(i,0,5,4)*ui(
     & i1,i2+5,i3+4,c3)+c(i,1,5,4)*ui(i1+1,i2+5,i3+4,c3)+c(i,2,5,4)*
     & ui(i1+2,i2+5,i3+4,c3)+c(i,3,5,4)*ui(i1+3,i2+5,i3+4,c3)+c(i,4,5,
     & 4)*ui(i1+4,i2+5,i3+4,c3)+c(i,5,5,4)*ui(i1+5,i2+5,i3+4,c3)+c(i,
     & 6,5,4)*ui(i1+6,i2+5,i3+4,c3)+c(i,0,6,4)*ui(i1,i2+6,i3+4,c3)+c(
     & i,1,6,4)*ui(i1+1,i2+6,i3+4,c3)+c(i,2,6,4)*ui(i1+2,i2+6,i3+4,c3)
     & +c(i,3,6,4)*ui(i1+3,i2+6,i3+4,c3)+c(i,4,6,4)*ui(i1+4,i2+6,i3+4,
     & c3)+c(i,5,6,4)*ui(i1+5,i2+6,i3+4,c3)+c(i,6,6,4)*ui(i1+6,i2+6,
     & i3+4,c3)+c(i,0,0,5)*ui(i1,i2,i3+5,c3)+c(i,1,0,5)*ui(i1+1,i2,i3+
     & 5,c3)+c(i,2,0,5)*ui(i1+2,i2,i3+5,c3)+c(i,3,0,5)*ui(i1+3,i2,i3+
     & 5,c3)+c(i,4,0,5)*ui(i1+4,i2,i3+5,c3)+c(i,5,0,5)*ui(i1+5,i2,i3+
     & 5,c3)+c(i,6,0,5)*ui(i1+6,i2,i3+5,c3)+c(i,0,1,5)*ui(i1,i2+1,i3+
     & 5,c3)+c(i,1,1,5)*ui(i1+1,i2+1,i3+5,c3)+c(i,2,1,5)*ui(i1+2,i2+1,
     & i3+5,c3)+c(i,3,1,5)*ui(i1+3,i2+1,i3+5,c3)+c(i,4,1,5)*ui(i1+4,
     & i2+1,i3+5,c3)+c(i,5,1,5)*ui(i1+5,i2+1,i3+5,c3)+c(i,6,1,5)*ui(
     & i1+6,i2+1,i3+5,c3)+c(i,0,2,5)*ui(i1,i2+2,i3+5,c3)+c(i,1,2,5)*
     & ui(i1+1,i2+2,i3+5,c3)+c(i,2,2,5)*ui(i1+2,i2+2,i3+5,c3)+c(i,3,2,
     & 5)*ui(i1+3,i2+2,i3+5,c3)+c(i,4,2,5)*ui(i1+4,i2+2,i3+5,c3)+c(i,
     & 5,2,5)*ui(i1+5,i2+2,i3+5,c3)+c(i,6,2,5)*ui(i1+6,i2+2,i3+5,c3)+
     & c(i,0,3,5)*ui(i1,i2+3,i3+5,c3)+c(i,1,3,5)*ui(i1+1,i2+3,i3+5,c3)
     & +c(i,2,3,5)*ui(i1+2,i2+3,i3+5,c3)+c(i,3,3,5)*ui(i1+3,i2+3,i3+5,
     & c3)+c(i,4,3,5)*ui(i1+4,i2+3,i3+5,c3)+c(i,5,3,5)*ui(i1+5,i2+3,
     & i3+5,c3)+c(i,6,3,5)*ui(i1+6,i2+3,i3+5,c3)+c(i,0,4,5)*ui(i1,i2+
     & 4,i3+5,c3)+c(i,1,4,5)*ui(i1+1,i2+4,i3+5,c3)+c(i,2,4,5)*ui(i1+2,
     & i2+4,i3+5,c3)+c(i,3,4,5)*ui(i1+3,i2+4,i3+5,c3)+c(i,4,4,5)*ui(
     & i1+4,i2+4,i3+5,c3)+c(i,5,4,5)*ui(i1+5,i2+4,i3+5,c3)+c(i,6,4,5)*
     & ui(i1+6,i2+4,i3+5,c3)+c(i,0,5,5)*ui(i1,i2+5,i3+5,c3)+c(i,1,5,5)
     & *ui(i1+1,i2+5,i3+5,c3)+c(i,2,5,5)*ui(i1+2,i2+5,i3+5,c3)+c(i,3,
     & 5,5)*ui(i1+3,i2+5,i3+5,c3)+c(i,4,5,5)*ui(i1+4,i2+5,i3+5,c3)+c(
     & i,5,5,5)*ui(i1+5,i2+5,i3+5,c3)+c(i,6,5,5)*ui(i1+6,i2+5,i3+5,c3)
     & +c(i,0,6,5)*ui(i1,i2+6,i3+5,c3)+c(i,1,6,5)*ui(i1+1,i2+6,i3+5,
     & c3)+c(i,2,6,5)*ui(i1+2,i2+6,i3+5,c3)+c(i,3,6,5)*ui(i1+3,i2+6,
     & i3+5,c3)+c(i,4,6,5)*ui(i1+4,i2+6,i3+5,c3)+c(i,5,6,5)*ui(i1+5,
     & i2+6,i3+5,c3)+c(i,6,6,5)*ui(i1+6,i2+6,i3+5,c3)+c(i,0,0,6)*ui(
     & i1,i2,i3+6,c3)+c(i,1,0,6)*ui(i1+1,i2,i3+6,c3)+c(i,2,0,6)*ui(i1+
     & 2,i2,i3+6,c3)+c(i,3,0,6)*ui(i1+3,i2,i3+6,c3)+c(i,4,0,6)*ui(i1+
     & 4,i2,i3+6,c3)+c(i,5,0,6)*ui(i1+5,i2,i3+6,c3)+c(i,6,0,6)*ui(i1+
     & 6,i2,i3+6,c3)+c(i,0,1,6)*ui(i1,i2+1,i3+6,c3)+c(i,1,1,6)*ui(i1+
     & 1,i2+1,i3+6,c3)+c(i,2,1,6)*ui(i1+2,i2+1,i3+6,c3)+c(i,3,1,6)*ui(
     & i1+3,i2+1,i3+6,c3)+c(i,4,1,6)*ui(i1+4,i2+1,i3+6,c3)+c(i,5,1,6)*
     & ui(i1+5,i2+1,i3+6,c3)+c(i,6,1,6)*ui(i1+6,i2+1,i3+6,c3)+c(i,0,2,
     & 6)*ui(i1,i2+2,i3+6,c3)+c(i,1,2,6)*ui(i1+1,i2+2,i3+6,c3)+c(i,2,
     & 2,6)*ui(i1+2,i2+2,i3+6,c3)+c(i,3,2,6)*ui(i1+3,i2+2,i3+6,c3)+c(
     & i,4,2,6)*ui(i1+4,i2+2,i3+6,c3)+c(i,5,2,6)*ui(i1+5,i2+2,i3+6,c3)
     & +c(i,6,2,6)*ui(i1+6,i2+2,i3+6,c3)+c(i,0,3,6)*ui(i1,i2+3,i3+6,
     & c3)+c(i,1,3,6)*ui(i1+1,i2+3,i3+6,c3)+c(i,2,3,6)*ui(i1+2,i2+3,
     & i3+6,c3)+c(i,3,3,6)*ui(i1+3,i2+3,i3+6,c3)+c(i,4,3,6)*ui(i1+4,
     & i2+3,i3+6,c3)+c(i,5,3,6)*ui(i1+5,i2+3,i3+6,c3)+c(i,6,3,6)*ui(
     & i1+6,i2+3,i3+6,c3)+c(i,0,4,6)*ui(i1,i2+4,i3+6,c3)+c(i,1,4,6)*
     & ui(i1+1,i2+4,i3+6,c3)+c(i,2,4,6)*ui(i1+2,i2+4,i3+6,c3)+c(i,3,4,
     & 6)*ui(i1+3,i2+4,i3+6,c3)+c(i,4,4,6)*ui(i1+4,i2+4,i3+6,c3)+c(i,
     & 5,4,6)*ui(i1+5,i2+4,i3+6,c3)+c(i,6,4,6)*ui(i1+6,i2+4,i3+6,c3)+
     & c(i,0,5,6)*ui(i1,i2+5,i3+6,c3)+c(i,1,5,6)*ui(i1+1,i2+5,i3+6,c3)
     & +c(i,2,5,6)*ui(i1+2,i2+5,i3+6,c3)+c(i,3,5,6)*ui(i1+3,i2+5,i3+6,
     & c3)+c(i,4,5,6)*ui(i1+4,i2+5,i3+6,c3)+c(i,5,5,6)*ui(i1+5,i2+5,
     & i3+6,c3)+c(i,6,5,6)*ui(i1+6,i2+5,i3+6,c3)+c(i,0,6,6)*ui(i1,i2+
     & 6,i3+6,c3)+c(i,1,6,6)*ui(i1+1,i2+6,i3+6,c3)+c(i,2,6,6)*ui(i1+2,
     & i2+6,i3+6,c3)+c(i,3,6,6)*ui(i1+3,i2+6,i3+6,c3)+c(i,4,6,6)*ui(
     & i1+4,i2+6,i3+6,c3)+c(i,5,6,6)*ui(i1+5,i2+6,i3+6,c3)+c(i,6,6,6)*
     & ui(i1+6,i2+6,i3+6,c3)
             else if( varWidth(i).eq.6 )then
! interp666(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,0,0)*ui(i1,i2,i3,
     & c3)+c(i,1,0,0)*ui(i1+1,i2,i3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3,c3)+
     & c(i,3,0,0)*ui(i1+3,i2,i3,c3)+c(i,4,0,0)*ui(i1+4,i2,i3,c3)+c(i,
     & 5,0,0)*ui(i1+5,i2,i3,c3)+c(i,0,1,0)*ui(i1,i2+1,i3,c3)+c(i,1,1,
     & 0)*ui(i1+1,i2+1,i3,c3)+c(i,2,1,0)*ui(i1+2,i2+1,i3,c3)+c(i,3,1,
     & 0)*ui(i1+3,i2+1,i3,c3)+c(i,4,1,0)*ui(i1+4,i2+1,i3,c3)+c(i,5,1,
     & 0)*ui(i1+5,i2+1,i3,c3)+c(i,0,2,0)*ui(i1,i2+2,i3,c3)+c(i,1,2,0)*
     & ui(i1+1,i2+2,i3,c3)+c(i,2,2,0)*ui(i1+2,i2+2,i3,c3)+c(i,3,2,0)*
     & ui(i1+3,i2+2,i3,c3)+c(i,4,2,0)*ui(i1+4,i2+2,i3,c3)+c(i,5,2,0)*
     & ui(i1+5,i2+2,i3,c3)+c(i,0,3,0)*ui(i1,i2+3,i3,c3)+c(i,1,3,0)*ui(
     & i1+1,i2+3,i3,c3)+c(i,2,3,0)*ui(i1+2,i2+3,i3,c3)+c(i,3,3,0)*ui(
     & i1+3,i2+3,i3,c3)+c(i,4,3,0)*ui(i1+4,i2+3,i3,c3)+c(i,5,3,0)*ui(
     & i1+5,i2+3,i3,c3)+c(i,0,4,0)*ui(i1,i2+4,i3,c3)+c(i,1,4,0)*ui(i1+
     & 1,i2+4,i3,c3)+c(i,2,4,0)*ui(i1+2,i2+4,i3,c3)+c(i,3,4,0)*ui(i1+
     & 3,i2+4,i3,c3)+c(i,4,4,0)*ui(i1+4,i2+4,i3,c3)+c(i,5,4,0)*ui(i1+
     & 5,i2+4,i3,c3)+c(i,0,5,0)*ui(i1,i2+5,i3,c3)+c(i,1,5,0)*ui(i1+1,
     & i2+5,i3,c3)+c(i,2,5,0)*ui(i1+2,i2+5,i3,c3)+c(i,3,5,0)*ui(i1+3,
     & i2+5,i3,c3)+c(i,4,5,0)*ui(i1+4,i2+5,i3,c3)+c(i,5,5,0)*ui(i1+5,
     & i2+5,i3,c3)+c(i,0,0,1)*ui(i1,i2,i3+1,c3)+c(i,1,0,1)*ui(i1+1,i2,
     & i3+1,c3)+c(i,2,0,1)*ui(i1+2,i2,i3+1,c3)+c(i,3,0,1)*ui(i1+3,i2,
     & i3+1,c3)+c(i,4,0,1)*ui(i1+4,i2,i3+1,c3)+c(i,5,0,1)*ui(i1+5,i2,
     & i3+1,c3)+c(i,0,1,1)*ui(i1,i2+1,i3+1,c3)+c(i,1,1,1)*ui(i1+1,i2+
     & 1,i3+1,c3)+c(i,2,1,1)*ui(i1+2,i2+1,i3+1,c3)+c(i,3,1,1)*ui(i1+3,
     & i2+1,i3+1,c3)+c(i,4,1,1)*ui(i1+4,i2+1,i3+1,c3)+c(i,5,1,1)*ui(
     & i1+5,i2+1,i3+1,c3)+c(i,0,2,1)*ui(i1,i2+2,i3+1,c3)+c(i,1,2,1)*
     & ui(i1+1,i2+2,i3+1,c3)+c(i,2,2,1)*ui(i1+2,i2+2,i3+1,c3)+c(i,3,2,
     & 1)*ui(i1+3,i2+2,i3+1,c3)+c(i,4,2,1)*ui(i1+4,i2+2,i3+1,c3)+c(i,
     & 5,2,1)*ui(i1+5,i2+2,i3+1,c3)+c(i,0,3,1)*ui(i1,i2+3,i3+1,c3)+c(
     & i,1,3,1)*ui(i1+1,i2+3,i3+1,c3)+c(i,2,3,1)*ui(i1+2,i2+3,i3+1,c3)
     & +c(i,3,3,1)*ui(i1+3,i2+3,i3+1,c3)+c(i,4,3,1)*ui(i1+4,i2+3,i3+1,
     & c3)+c(i,5,3,1)*ui(i1+5,i2+3,i3+1,c3)+c(i,0,4,1)*ui(i1,i2+4,i3+
     & 1,c3)+c(i,1,4,1)*ui(i1+1,i2+4,i3+1,c3)+c(i,2,4,1)*ui(i1+2,i2+4,
     & i3+1,c3)+c(i,3,4,1)*ui(i1+3,i2+4,i3+1,c3)+c(i,4,4,1)*ui(i1+4,
     & i2+4,i3+1,c3)+c(i,5,4,1)*ui(i1+5,i2+4,i3+1,c3)+c(i,0,5,1)*ui(
     & i1,i2+5,i3+1,c3)+c(i,1,5,1)*ui(i1+1,i2+5,i3+1,c3)+c(i,2,5,1)*
     & ui(i1+2,i2+5,i3+1,c3)+c(i,3,5,1)*ui(i1+3,i2+5,i3+1,c3)+c(i,4,5,
     & 1)*ui(i1+4,i2+5,i3+1,c3)+c(i,5,5,1)*ui(i1+5,i2+5,i3+1,c3)+c(i,
     & 0,0,2)*ui(i1,i2,i3+2,c3)+c(i,1,0,2)*ui(i1+1,i2,i3+2,c3)+c(i,2,
     & 0,2)*ui(i1+2,i2,i3+2,c3)+c(i,3,0,2)*ui(i1+3,i2,i3+2,c3)+c(i,4,
     & 0,2)*ui(i1+4,i2,i3+2,c3)+c(i,5,0,2)*ui(i1+5,i2,i3+2,c3)+c(i,0,
     & 1,2)*ui(i1,i2+1,i3+2,c3)+c(i,1,1,2)*ui(i1+1,i2+1,i3+2,c3)+c(i,
     & 2,1,2)*ui(i1+2,i2+1,i3+2,c3)+c(i,3,1,2)*ui(i1+3,i2+1,i3+2,c3)+
     & c(i,4,1,2)*ui(i1+4,i2+1,i3+2,c3)+c(i,5,1,2)*ui(i1+5,i2+1,i3+2,
     & c3)+c(i,0,2,2)*ui(i1,i2+2,i3+2,c3)+c(i,1,2,2)*ui(i1+1,i2+2,i3+
     & 2,c3)+c(i,2,2,2)*ui(i1+2,i2+2,i3+2,c3)+c(i,3,2,2)*ui(i1+3,i2+2,
     & i3+2,c3)+c(i,4,2,2)*ui(i1+4,i2+2,i3+2,c3)+c(i,5,2,2)*ui(i1+5,
     & i2+2,i3+2,c3)+c(i,0,3,2)*ui(i1,i2+3,i3+2,c3)+c(i,1,3,2)*ui(i1+
     & 1,i2+3,i3+2,c3)+c(i,2,3,2)*ui(i1+2,i2+3,i3+2,c3)+c(i,3,3,2)*ui(
     & i1+3,i2+3,i3+2,c3)+c(i,4,3,2)*ui(i1+4,i2+3,i3+2,c3)+c(i,5,3,2)*
     & ui(i1+5,i2+3,i3+2,c3)+c(i,0,4,2)*ui(i1,i2+4,i3+2,c3)+c(i,1,4,2)
     & *ui(i1+1,i2+4,i3+2,c3)+c(i,2,4,2)*ui(i1+2,i2+4,i3+2,c3)+c(i,3,
     & 4,2)*ui(i1+3,i2+4,i3+2,c3)+c(i,4,4,2)*ui(i1+4,i2+4,i3+2,c3)+c(
     & i,5,4,2)*ui(i1+5,i2+4,i3+2,c3)+c(i,0,5,2)*ui(i1,i2+5,i3+2,c3)+
     & c(i,1,5,2)*ui(i1+1,i2+5,i3+2,c3)+c(i,2,5,2)*ui(i1+2,i2+5,i3+2,
     & c3)+c(i,3,5,2)*ui(i1+3,i2+5,i3+2,c3)+c(i,4,5,2)*ui(i1+4,i2+5,
     & i3+2,c3)+c(i,5,5,2)*ui(i1+5,i2+5,i3+2,c3)+c(i,0,0,3)*ui(i1,i2,
     & i3+3,c3)+c(i,1,0,3)*ui(i1+1,i2,i3+3,c3)+c(i,2,0,3)*ui(i1+2,i2,
     & i3+3,c3)+c(i,3,0,3)*ui(i1+3,i2,i3+3,c3)+c(i,4,0,3)*ui(i1+4,i2,
     & i3+3,c3)+c(i,5,0,3)*ui(i1+5,i2,i3+3,c3)+c(i,0,1,3)*ui(i1,i2+1,
     & i3+3,c3)+c(i,1,1,3)*ui(i1+1,i2+1,i3+3,c3)+c(i,2,1,3)*ui(i1+2,
     & i2+1,i3+3,c3)+c(i,3,1,3)*ui(i1+3,i2+1,i3+3,c3)+c(i,4,1,3)*ui(
     & i1+4,i2+1,i3+3,c3)+c(i,5,1,3)*ui(i1+5,i2+1,i3+3,c3)+c(i,0,2,3)*
     & ui(i1,i2+2,i3+3,c3)+c(i,1,2,3)*ui(i1+1,i2+2,i3+3,c3)+c(i,2,2,3)
     & *ui(i1+2,i2+2,i3+3,c3)+c(i,3,2,3)*ui(i1+3,i2+2,i3+3,c3)+c(i,4,
     & 2,3)*ui(i1+4,i2+2,i3+3,c3)+c(i,5,2,3)*ui(i1+5,i2+2,i3+3,c3)+c(
     & i,0,3,3)*ui(i1,i2+3,i3+3,c3)+c(i,1,3,3)*ui(i1+1,i2+3,i3+3,c3)+
     & c(i,2,3,3)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,3,3)*ui(i1+3,i2+3,i3+3,
     & c3)+c(i,4,3,3)*ui(i1+4,i2+3,i3+3,c3)+c(i,5,3,3)*ui(i1+5,i2+3,
     & i3+3,c3)+c(i,0,4,3)*ui(i1,i2+4,i3+3,c3)+c(i,1,4,3)*ui(i1+1,i2+
     & 4,i3+3,c3)+c(i,2,4,3)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,4,3)*ui(i1+3,
     & i2+4,i3+3,c3)+c(i,4,4,3)*ui(i1+4,i2+4,i3+3,c3)+c(i,5,4,3)*ui(
     & i1+5,i2+4,i3+3,c3)+c(i,0,5,3)*ui(i1,i2+5,i3+3,c3)+c(i,1,5,3)*
     & ui(i1+1,i2+5,i3+3,c3)+c(i,2,5,3)*ui(i1+2,i2+5,i3+3,c3)+c(i,3,5,
     & 3)*ui(i1+3,i2+5,i3+3,c3)+c(i,4,5,3)*ui(i1+4,i2+5,i3+3,c3)+c(i,
     & 5,5,3)*ui(i1+5,i2+5,i3+3,c3)+c(i,0,0,4)*ui(i1,i2,i3+4,c3)+c(i,
     & 1,0,4)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,4)*ui(i1+2,i2,i3+4,c3)+c(i,
     & 3,0,4)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,4)*ui(i1+4,i2,i3+4,c3)+c(i,
     & 5,0,4)*ui(i1+5,i2,i3+4,c3)+c(i,0,1,4)*ui(i1,i2+1,i3+4,c3)+c(i,
     & 1,1,4)*ui(i1+1,i2+1,i3+4,c3)+c(i,2,1,4)*ui(i1+2,i2+1,i3+4,c3)+
     & c(i,3,1,4)*ui(i1+3,i2+1,i3+4,c3)+c(i,4,1,4)*ui(i1+4,i2+1,i3+4,
     & c3)+c(i,5,1,4)*ui(i1+5,i2+1,i3+4,c3)+c(i,0,2,4)*ui(i1,i2+2,i3+
     & 4,c3)+c(i,1,2,4)*ui(i1+1,i2+2,i3+4,c3)+c(i,2,2,4)*ui(i1+2,i2+2,
     & i3+4,c3)+c(i,3,2,4)*ui(i1+3,i2+2,i3+4,c3)+c(i,4,2,4)*ui(i1+4,
     & i2+2,i3+4,c3)+c(i,5,2,4)*ui(i1+5,i2+2,i3+4,c3)+c(i,0,3,4)*ui(
     & i1,i2+3,i3+4,c3)+c(i,1,3,4)*ui(i1+1,i2+3,i3+4,c3)+c(i,2,3,4)*
     & ui(i1+2,i2+3,i3+4,c3)+c(i,3,3,4)*ui(i1+3,i2+3,i3+4,c3)+c(i,4,3,
     & 4)*ui(i1+4,i2+3,i3+4,c3)+c(i,5,3,4)*ui(i1+5,i2+3,i3+4,c3)+c(i,
     & 0,4,4)*ui(i1,i2+4,i3+4,c3)+c(i,1,4,4)*ui(i1+1,i2+4,i3+4,c3)+c(
     & i,2,4,4)*ui(i1+2,i2+4,i3+4,c3)+c(i,3,4,4)*ui(i1+3,i2+4,i3+4,c3)
     & +c(i,4,4,4)*ui(i1+4,i2+4,i3+4,c3)+c(i,5,4,4)*ui(i1+5,i2+4,i3+4,
     & c3)+c(i,0,5,4)*ui(i1,i2+5,i3+4,c3)+c(i,1,5,4)*ui(i1+1,i2+5,i3+
     & 4,c3)+c(i,2,5,4)*ui(i1+2,i2+5,i3+4,c3)+c(i,3,5,4)*ui(i1+3,i2+5,
     & i3+4,c3)+c(i,4,5,4)*ui(i1+4,i2+5,i3+4,c3)+c(i,5,5,4)*ui(i1+5,
     & i2+5,i3+4,c3)+c(i,0,0,5)*ui(i1,i2,i3+5,c3)+c(i,1,0,5)*ui(i1+1,
     & i2,i3+5,c3)+c(i,2,0,5)*ui(i1+2,i2,i3+5,c3)+c(i,3,0,5)*ui(i1+3,
     & i2,i3+5,c3)
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,4,0,5)*ui(i1+4,i2,i3+5,c3)+c(i,5,0,5)*ui(i1+5,i2,
     & i3+5,c3)+c(i,0,1,5)*ui(i1,i2+1,i3+5,c3)+c(i,1,1,5)*ui(i1+1,i2+
     & 1,i3+5,c3)+c(i,2,1,5)*ui(i1+2,i2+1,i3+5,c3)+c(i,3,1,5)*ui(i1+3,
     & i2+1,i3+5,c3)+c(i,4,1,5)*ui(i1+4,i2+1,i3+5,c3)+c(i,5,1,5)*ui(
     & i1+5,i2+1,i3+5,c3)+c(i,0,2,5)*ui(i1,i2+2,i3+5,c3)+c(i,1,2,5)*
     & ui(i1+1,i2+2,i3+5,c3)+c(i,2,2,5)*ui(i1+2,i2+2,i3+5,c3)+c(i,3,2,
     & 5)*ui(i1+3,i2+2,i3+5,c3)+c(i,4,2,5)*ui(i1+4,i2+2,i3+5,c3)+c(i,
     & 5,2,5)*ui(i1+5,i2+2,i3+5,c3)+c(i,0,3,5)*ui(i1,i2+3,i3+5,c3)+c(
     & i,1,3,5)*ui(i1+1,i2+3,i3+5,c3)+c(i,2,3,5)*ui(i1+2,i2+3,i3+5,c3)
     & +c(i,3,3,5)*ui(i1+3,i2+3,i3+5,c3)+c(i,4,3,5)*ui(i1+4,i2+3,i3+5,
     & c3)+c(i,5,3,5)*ui(i1+5,i2+3,i3+5,c3)+c(i,0,4,5)*ui(i1,i2+4,i3+
     & 5,c3)+c(i,1,4,5)*ui(i1+1,i2+4,i3+5,c3)+c(i,2,4,5)*ui(i1+2,i2+4,
     & i3+5,c3)+c(i,3,4,5)*ui(i1+3,i2+4,i3+5,c3)+c(i,4,4,5)*ui(i1+4,
     & i2+4,i3+5,c3)+c(i,5,4,5)*ui(i1+5,i2+4,i3+5,c3)+c(i,0,5,5)*ui(
     & i1,i2+5,i3+5,c3)+c(i,1,5,5)*ui(i1+1,i2+5,i3+5,c3)+c(i,2,5,5)*
     & ui(i1+2,i2+5,i3+5,c3)+c(i,3,5,5)*ui(i1+3,i2+5,i3+5,c3)+c(i,4,5,
     & 5)*ui(i1+4,i2+5,i3+5,c3)+c(i,5,5,5)*ui(i1+5,i2+5,i3+5,c3)
             else if( varWidth(i).eq.9 )then
! interp999(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,0,0)*ui(i1,i2,i3,
     & c3)+c(i,1,0,0)*ui(i1+1,i2,i3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3,c3)+
     & c(i,3,0,0)*ui(i1+3,i2,i3,c3)+c(i,4,0,0)*ui(i1+4,i2,i3,c3)+c(i,
     & 5,0,0)*ui(i1+5,i2,i3,c3)+c(i,6,0,0)*ui(i1+6,i2,i3,c3)+c(i,7,0,
     & 0)*ui(i1+7,i2,i3,c3)+c(i,8,0,0)*ui(i1+8,i2,i3,c3)+c(i,0,1,0)*
     & ui(i1,i2+1,i3,c3)+c(i,1,1,0)*ui(i1+1,i2+1,i3,c3)+c(i,2,1,0)*ui(
     & i1+2,i2+1,i3,c3)+c(i,3,1,0)*ui(i1+3,i2+1,i3,c3)+c(i,4,1,0)*ui(
     & i1+4,i2+1,i3,c3)+c(i,5,1,0)*ui(i1+5,i2+1,i3,c3)+c(i,6,1,0)*ui(
     & i1+6,i2+1,i3,c3)+c(i,7,1,0)*ui(i1+7,i2+1,i3,c3)+c(i,8,1,0)*ui(
     & i1+8,i2+1,i3,c3)+c(i,0,2,0)*ui(i1,i2+2,i3,c3)+c(i,1,2,0)*ui(i1+
     & 1,i2+2,i3,c3)+c(i,2,2,0)*ui(i1+2,i2+2,i3,c3)+c(i,3,2,0)*ui(i1+
     & 3,i2+2,i3,c3)+c(i,4,2,0)*ui(i1+4,i2+2,i3,c3)+c(i,5,2,0)*ui(i1+
     & 5,i2+2,i3,c3)+c(i,6,2,0)*ui(i1+6,i2+2,i3,c3)+c(i,7,2,0)*ui(i1+
     & 7,i2+2,i3,c3)+c(i,8,2,0)*ui(i1+8,i2+2,i3,c3)+c(i,0,3,0)*ui(i1,
     & i2+3,i3,c3)+c(i,1,3,0)*ui(i1+1,i2+3,i3,c3)+c(i,2,3,0)*ui(i1+2,
     & i2+3,i3,c3)+c(i,3,3,0)*ui(i1+3,i2+3,i3,c3)+c(i,4,3,0)*ui(i1+4,
     & i2+3,i3,c3)+c(i,5,3,0)*ui(i1+5,i2+3,i3,c3)+c(i,6,3,0)*ui(i1+6,
     & i2+3,i3,c3)+c(i,7,3,0)*ui(i1+7,i2+3,i3,c3)+c(i,8,3,0)*ui(i1+8,
     & i2+3,i3,c3)+c(i,0,4,0)*ui(i1,i2+4,i3,c3)+c(i,1,4,0)*ui(i1+1,i2+
     & 4,i3,c3)+c(i,2,4,0)*ui(i1+2,i2+4,i3,c3)+c(i,3,4,0)*ui(i1+3,i2+
     & 4,i3,c3)+c(i,4,4,0)*ui(i1+4,i2+4,i3,c3)+c(i,5,4,0)*ui(i1+5,i2+
     & 4,i3,c3)+c(i,6,4,0)*ui(i1+6,i2+4,i3,c3)+c(i,7,4,0)*ui(i1+7,i2+
     & 4,i3,c3)+c(i,8,4,0)*ui(i1+8,i2+4,i3,c3)+c(i,0,5,0)*ui(i1,i2+5,
     & i3,c3)+c(i,1,5,0)*ui(i1+1,i2+5,i3,c3)+c(i,2,5,0)*ui(i1+2,i2+5,
     & i3,c3)+c(i,3,5,0)*ui(i1+3,i2+5,i3,c3)+c(i,4,5,0)*ui(i1+4,i2+5,
     & i3,c3)+c(i,5,5,0)*ui(i1+5,i2+5,i3,c3)+c(i,6,5,0)*ui(i1+6,i2+5,
     & i3,c3)+c(i,7,5,0)*ui(i1+7,i2+5,i3,c3)+c(i,8,5,0)*ui(i1+8,i2+5,
     & i3,c3)+c(i,0,6,0)*ui(i1,i2+6,i3,c3)+c(i,1,6,0)*ui(i1+1,i2+6,i3,
     & c3)+c(i,2,6,0)*ui(i1+2,i2+6,i3,c3)+c(i,3,6,0)*ui(i1+3,i2+6,i3,
     & c3)+c(i,4,6,0)*ui(i1+4,i2+6,i3,c3)+c(i,5,6,0)*ui(i1+5,i2+6,i3,
     & c3)+c(i,6,6,0)*ui(i1+6,i2+6,i3,c3)+c(i,7,6,0)*ui(i1+7,i2+6,i3,
     & c3)+c(i,8,6,0)*ui(i1+8,i2+6,i3,c3)+c(i,0,7,0)*ui(i1,i2+7,i3,c3)
     & +c(i,1,7,0)*ui(i1+1,i2+7,i3,c3)+c(i,2,7,0)*ui(i1+2,i2+7,i3,c3)+
     & c(i,3,7,0)*ui(i1+3,i2+7,i3,c3)+c(i,4,7,0)*ui(i1+4,i2+7,i3,c3)+
     & c(i,5,7,0)*ui(i1+5,i2+7,i3,c3)+c(i,6,7,0)*ui(i1+6,i2+7,i3,c3)+
     & c(i,7,7,0)*ui(i1+7,i2+7,i3,c3)+c(i,8,7,0)*ui(i1+8,i2+7,i3,c3)+
     & c(i,0,8,0)*ui(i1,i2+8,i3,c3)+c(i,1,8,0)*ui(i1+1,i2+8,i3,c3)+c(
     & i,2,8,0)*ui(i1+2,i2+8,i3,c3)+c(i,3,8,0)*ui(i1+3,i2+8,i3,c3)+c(
     & i,4,8,0)*ui(i1+4,i2+8,i3,c3)+c(i,5,8,0)*ui(i1+5,i2+8,i3,c3)+c(
     & i,6,8,0)*ui(i1+6,i2+8,i3,c3)+c(i,7,8,0)*ui(i1+7,i2+8,i3,c3)+c(
     & i,8,8,0)*ui(i1+8,i2+8,i3,c3)+c(i,0,0,1)*ui(i1,i2,i3+1,c3)+c(i,
     & 1,0,1)*ui(i1+1,i2,i3+1,c3)+c(i,2,0,1)*ui(i1+2,i2,i3+1,c3)+c(i,
     & 3,0,1)*ui(i1+3,i2,i3+1,c3)+c(i,4,0,1)*ui(i1+4,i2,i3+1,c3)+c(i,
     & 5,0,1)*ui(i1+5,i2,i3+1,c3)+c(i,6,0,1)*ui(i1+6,i2,i3+1,c3)+c(i,
     & 7,0,1)*ui(i1+7,i2,i3+1,c3)+c(i,8,0,1)*ui(i1+8,i2,i3+1,c3)+c(i,
     & 0,1,1)*ui(i1,i2+1,i3+1,c3)+c(i,1,1,1)*ui(i1+1,i2+1,i3+1,c3)+c(
     & i,2,1,1)*ui(i1+2,i2+1,i3+1,c3)+c(i,3,1,1)*ui(i1+3,i2+1,i3+1,c3)
     & +c(i,4,1,1)*ui(i1+4,i2+1,i3+1,c3)+c(i,5,1,1)*ui(i1+5,i2+1,i3+1,
     & c3)+c(i,6,1,1)*ui(i1+6,i2+1,i3+1,c3)+c(i,7,1,1)*ui(i1+7,i2+1,
     & i3+1,c3)+c(i,8,1,1)*ui(i1+8,i2+1,i3+1,c3)+c(i,0,2,1)*ui(i1,i2+
     & 2,i3+1,c3)+c(i,1,2,1)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,2,1)*ui(i1+2,
     & i2+2,i3+1,c3)+c(i,3,2,1)*ui(i1+3,i2+2,i3+1,c3)+c(i,4,2,1)*ui(
     & i1+4,i2+2,i3+1,c3)+c(i,5,2,1)*ui(i1+5,i2+2,i3+1,c3)+c(i,6,2,1)*
     & ui(i1+6,i2+2,i3+1,c3)+c(i,7,2,1)*ui(i1+7,i2+2,i3+1,c3)+c(i,8,2,
     & 1)*ui(i1+8,i2+2,i3+1,c3)+c(i,0,3,1)*ui(i1,i2+3,i3+1,c3)+c(i,1,
     & 3,1)*ui(i1+1,i2+3,i3+1,c3)+c(i,2,3,1)*ui(i1+2,i2+3,i3+1,c3)+c(
     & i,3,3,1)*ui(i1+3,i2+3,i3+1,c3)+c(i,4,3,1)*ui(i1+4,i2+3,i3+1,c3)
     & +c(i,5,3,1)*ui(i1+5,i2+3,i3+1,c3)+c(i,6,3,1)*ui(i1+6,i2+3,i3+1,
     & c3)+c(i,7,3,1)*ui(i1+7,i2+3,i3+1,c3)+c(i,8,3,1)*ui(i1+8,i2+3,
     & i3+1,c3)+c(i,0,4,1)*ui(i1,i2+4,i3+1,c3)+c(i,1,4,1)*ui(i1+1,i2+
     & 4,i3+1,c3)+c(i,2,4,1)*ui(i1+2,i2+4,i3+1,c3)+c(i,3,4,1)*ui(i1+3,
     & i2+4,i3+1,c3)+c(i,4,4,1)*ui(i1+4,i2+4,i3+1,c3)+c(i,5,4,1)*ui(
     & i1+5,i2+4,i3+1,c3)+c(i,6,4,1)*ui(i1+6,i2+4,i3+1,c3)+c(i,7,4,1)*
     & ui(i1+7,i2+4,i3+1,c3)+c(i,8,4,1)*ui(i1+8,i2+4,i3+1,c3)+c(i,0,5,
     & 1)*ui(i1,i2+5,i3+1,c3)+c(i,1,5,1)*ui(i1+1,i2+5,i3+1,c3)+c(i,2,
     & 5,1)*ui(i1+2,i2+5,i3+1,c3)+c(i,3,5,1)*ui(i1+3,i2+5,i3+1,c3)+c(
     & i,4,5,1)*ui(i1+4,i2+5,i3+1,c3)+c(i,5,5,1)*ui(i1+5,i2+5,i3+1,c3)
     & +c(i,6,5,1)*ui(i1+6,i2+5,i3+1,c3)+c(i,7,5,1)*ui(i1+7,i2+5,i3+1,
     & c3)+c(i,8,5,1)*ui(i1+8,i2+5,i3+1,c3)+c(i,0,6,1)*ui(i1,i2+6,i3+
     & 1,c3)+c(i,1,6,1)*ui(i1+1,i2+6,i3+1,c3)+c(i,2,6,1)*ui(i1+2,i2+6,
     & i3+1,c3)+c(i,3,6,1)*ui(i1+3,i2+6,i3+1,c3)+c(i,4,6,1)*ui(i1+4,
     & i2+6,i3+1,c3)+c(i,5,6,1)*ui(i1+5,i2+6,i3+1,c3)+c(i,6,6,1)*ui(
     & i1+6,i2+6,i3+1,c3)+c(i,7,6,1)*ui(i1+7,i2+6,i3+1,c3)+c(i,8,6,1)*
     & ui(i1+8,i2+6,i3+1,c3)+c(i,0,7,1)*ui(i1,i2+7,i3+1,c3)+c(i,1,7,1)
     & *ui(i1+1,i2+7,i3+1,c3)+c(i,2,7,1)*ui(i1+2,i2+7,i3+1,c3)+c(i,3,
     & 7,1)*ui(i1+3,i2+7,i3+1,c3)+c(i,4,7,1)*ui(i1+4,i2+7,i3+1,c3)+c(
     & i,5,7,1)*ui(i1+5,i2+7,i3+1,c3)+c(i,6,7,1)*ui(i1+6,i2+7,i3+1,c3)
     & +c(i,7,7,1)*ui(i1+7,i2+7,i3+1,c3)+c(i,8,7,1)*ui(i1+8,i2+7,i3+1,
     & c3)+c(i,0,8,1)*ui(i1,i2+8,i3+1,c3)+c(i,1,8,1)*ui(i1+1,i2+8,i3+
     & 1,c3)+c(i,2,8,1)*ui(i1+2,i2+8,i3+1,c3)+c(i,3,8,1)*ui(i1+3,i2+8,
     & i3+1,c3)+c(i,4,8,1)*ui(i1+4,i2+8,i3+1,c3)+c(i,5,8,1)*ui(i1+5,
     & i2+8,i3+1,c3)+c(i,6,8,1)*ui(i1+6,i2+8,i3+1,c3)+c(i,7,8,1)*ui(
     & i1+7,i2+8,i3+1,c3)+c(i,8,8,1)*ui(i1+8,i2+8,i3+1,c3)+c(i,0,0,2)*
     & ui(i1,i2,i3+2,c3)+c(i,1,0,2)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,2)*ui(
     & i1+2,i2,i3+2,c3)+c(i,3,0,2)*ui(i1+3,i2,i3+2,c3)+c(i,4,0,2)*ui(
     & i1+4,i2,i3+2,c3)+c(i,5,0,2)*ui(i1+5,i2,i3+2,c3)+c(i,6,0,2)*ui(
     & i1+6,i2,i3+2,c3)+c(i,7,0,2)*ui(i1+7,i2,i3+2,c3)+c(i,8,0,2)*ui(
     & i1+8,i2,i3+2,c3)+c(i,0,1,2)*ui(i1,i2+1,i3+2,c3)+c(i,1,1,2)*ui(
     & i1+1,i2+1,i3+2,c3)+c(i,2,1,2)*ui(i1+2,i2+1,i3+2,c3)+c(i,3,1,2)*
     & ui(i1+3,i2+1,i3+2,c3)+c(i,4,1,2)*ui(i1+4,i2+1,i3+2,c3)+c(i,5,1,
     & 2)*ui(i1+5,i2+1,i3+2,c3)+c(i,6,1,2)*ui(i1+6,i2+1,i3+2,c3)+c(i,
     & 7,1,2)*ui(i1+7,i2+1,i3+2,c3)+c(i,8,1,2)*ui(i1+8,i2+1,i3+2,c3)+
     & c(i,0,2,2)*ui(i1,i2+2,i3+2,c3)+c(i,1,2,2)*ui(i1+1,i2+2,i3+2,c3)
     & +c(i,2,2,2)*ui(i1+2,i2+2,i3+2,c3)+c(i,3,2,2)*ui(i1+3,i2+2,i3+2,
     & c3)
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,4,2,2)*ui(i1+4,i2+2,i3+2,c3)+c(i,5,2,2)*ui(i1+5,
     & i2+2,i3+2,c3)+c(i,6,2,2)*ui(i1+6,i2+2,i3+2,c3)+c(i,7,2,2)*ui(
     & i1+7,i2+2,i3+2,c3)+c(i,8,2,2)*ui(i1+8,i2+2,i3+2,c3)+c(i,0,3,2)*
     & ui(i1,i2+3,i3+2,c3)+c(i,1,3,2)*ui(i1+1,i2+3,i3+2,c3)+c(i,2,3,2)
     & *ui(i1+2,i2+3,i3+2,c3)+c(i,3,3,2)*ui(i1+3,i2+3,i3+2,c3)+c(i,4,
     & 3,2)*ui(i1+4,i2+3,i3+2,c3)+c(i,5,3,2)*ui(i1+5,i2+3,i3+2,c3)+c(
     & i,6,3,2)*ui(i1+6,i2+3,i3+2,c3)+c(i,7,3,2)*ui(i1+7,i2+3,i3+2,c3)
     & +c(i,8,3,2)*ui(i1+8,i2+3,i3+2,c3)+c(i,0,4,2)*ui(i1,i2+4,i3+2,
     & c3)+c(i,1,4,2)*ui(i1+1,i2+4,i3+2,c3)+c(i,2,4,2)*ui(i1+2,i2+4,
     & i3+2,c3)+c(i,3,4,2)*ui(i1+3,i2+4,i3+2,c3)+c(i,4,4,2)*ui(i1+4,
     & i2+4,i3+2,c3)+c(i,5,4,2)*ui(i1+5,i2+4,i3+2,c3)+c(i,6,4,2)*ui(
     & i1+6,i2+4,i3+2,c3)+c(i,7,4,2)*ui(i1+7,i2+4,i3+2,c3)+c(i,8,4,2)*
     & ui(i1+8,i2+4,i3+2,c3)+c(i,0,5,2)*ui(i1,i2+5,i3+2,c3)+c(i,1,5,2)
     & *ui(i1+1,i2+5,i3+2,c3)+c(i,2,5,2)*ui(i1+2,i2+5,i3+2,c3)+c(i,3,
     & 5,2)*ui(i1+3,i2+5,i3+2,c3)+c(i,4,5,2)*ui(i1+4,i2+5,i3+2,c3)+c(
     & i,5,5,2)*ui(i1+5,i2+5,i3+2,c3)+c(i,6,5,2)*ui(i1+6,i2+5,i3+2,c3)
     & +c(i,7,5,2)*ui(i1+7,i2+5,i3+2,c3)+c(i,8,5,2)*ui(i1+8,i2+5,i3+2,
     & c3)+c(i,0,6,2)*ui(i1,i2+6,i3+2,c3)+c(i,1,6,2)*ui(i1+1,i2+6,i3+
     & 2,c3)+c(i,2,6,2)*ui(i1+2,i2+6,i3+2,c3)+c(i,3,6,2)*ui(i1+3,i2+6,
     & i3+2,c3)+c(i,4,6,2)*ui(i1+4,i2+6,i3+2,c3)+c(i,5,6,2)*ui(i1+5,
     & i2+6,i3+2,c3)+c(i,6,6,2)*ui(i1+6,i2+6,i3+2,c3)+c(i,7,6,2)*ui(
     & i1+7,i2+6,i3+2,c3)+c(i,8,6,2)*ui(i1+8,i2+6,i3+2,c3)+c(i,0,7,2)*
     & ui(i1,i2+7,i3+2,c3)+c(i,1,7,2)*ui(i1+1,i2+7,i3+2,c3)+c(i,2,7,2)
     & *ui(i1+2,i2+7,i3+2,c3)+c(i,3,7,2)*ui(i1+3,i2+7,i3+2,c3)+c(i,4,
     & 7,2)*ui(i1+4,i2+7,i3+2,c3)+c(i,5,7,2)*ui(i1+5,i2+7,i3+2,c3)+c(
     & i,6,7,2)*ui(i1+6,i2+7,i3+2,c3)+c(i,7,7,2)*ui(i1+7,i2+7,i3+2,c3)
     & +c(i,8,7,2)*ui(i1+8,i2+7,i3+2,c3)+c(i,0,8,2)*ui(i1,i2+8,i3+2,
     & c3)+c(i,1,8,2)*ui(i1+1,i2+8,i3+2,c3)+c(i,2,8,2)*ui(i1+2,i2+8,
     & i3+2,c3)+c(i,3,8,2)*ui(i1+3,i2+8,i3+2,c3)+c(i,4,8,2)*ui(i1+4,
     & i2+8,i3+2,c3)+c(i,5,8,2)*ui(i1+5,i2+8,i3+2,c3)+c(i,6,8,2)*ui(
     & i1+6,i2+8,i3+2,c3)+c(i,7,8,2)*ui(i1+7,i2+8,i3+2,c3)+c(i,8,8,2)*
     & ui(i1+8,i2+8,i3+2,c3)+c(i,0,0,3)*ui(i1,i2,i3+3,c3)+c(i,1,0,3)*
     & ui(i1+1,i2,i3+3,c3)+c(i,2,0,3)*ui(i1+2,i2,i3+3,c3)+c(i,3,0,3)*
     & ui(i1+3,i2,i3+3,c3)+c(i,4,0,3)*ui(i1+4,i2,i3+3,c3)+c(i,5,0,3)*
     & ui(i1+5,i2,i3+3,c3)+c(i,6,0,3)*ui(i1+6,i2,i3+3,c3)+c(i,7,0,3)*
     & ui(i1+7,i2,i3+3,c3)+c(i,8,0,3)*ui(i1+8,i2,i3+3,c3)+c(i,0,1,3)*
     & ui(i1,i2+1,i3+3,c3)+c(i,1,1,3)*ui(i1+1,i2+1,i3+3,c3)+c(i,2,1,3)
     & *ui(i1+2,i2+1,i3+3,c3)+c(i,3,1,3)*ui(i1+3,i2+1,i3+3,c3)+c(i,4,
     & 1,3)*ui(i1+4,i2+1,i3+3,c3)+c(i,5,1,3)*ui(i1+5,i2+1,i3+3,c3)+c(
     & i,6,1,3)*ui(i1+6,i2+1,i3+3,c3)+c(i,7,1,3)*ui(i1+7,i2+1,i3+3,c3)
     & +c(i,8,1,3)*ui(i1+8,i2+1,i3+3,c3)+c(i,0,2,3)*ui(i1,i2+2,i3+3,
     & c3)+c(i,1,2,3)*ui(i1+1,i2+2,i3+3,c3)+c(i,2,2,3)*ui(i1+2,i2+2,
     & i3+3,c3)+c(i,3,2,3)*ui(i1+3,i2+2,i3+3,c3)+c(i,4,2,3)*ui(i1+4,
     & i2+2,i3+3,c3)+c(i,5,2,3)*ui(i1+5,i2+2,i3+3,c3)+c(i,6,2,3)*ui(
     & i1+6,i2+2,i3+3,c3)+c(i,7,2,3)*ui(i1+7,i2+2,i3+3,c3)+c(i,8,2,3)*
     & ui(i1+8,i2+2,i3+3,c3)+c(i,0,3,3)*ui(i1,i2+3,i3+3,c3)+c(i,1,3,3)
     & *ui(i1+1,i2+3,i3+3,c3)+c(i,2,3,3)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,
     & 3,3)*ui(i1+3,i2+3,i3+3,c3)+c(i,4,3,3)*ui(i1+4,i2+3,i3+3,c3)+c(
     & i,5,3,3)*ui(i1+5,i2+3,i3+3,c3)+c(i,6,3,3)*ui(i1+6,i2+3,i3+3,c3)
     & +c(i,7,3,3)*ui(i1+7,i2+3,i3+3,c3)+c(i,8,3,3)*ui(i1+8,i2+3,i3+3,
     & c3)+c(i,0,4,3)*ui(i1,i2+4,i3+3,c3)+c(i,1,4,3)*ui(i1+1,i2+4,i3+
     & 3,c3)+c(i,2,4,3)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,4,3)*ui(i1+3,i2+4,
     & i3+3,c3)+c(i,4,4,3)*ui(i1+4,i2+4,i3+3,c3)+c(i,5,4,3)*ui(i1+5,
     & i2+4,i3+3,c3)+c(i,6,4,3)*ui(i1+6,i2+4,i3+3,c3)+c(i,7,4,3)*ui(
     & i1+7,i2+4,i3+3,c3)+c(i,8,4,3)*ui(i1+8,i2+4,i3+3,c3)+c(i,0,5,3)*
     & ui(i1,i2+5,i3+3,c3)+c(i,1,5,3)*ui(i1+1,i2+5,i3+3,c3)+c(i,2,5,3)
     & *ui(i1+2,i2+5,i3+3,c3)+c(i,3,5,3)*ui(i1+3,i2+5,i3+3,c3)+c(i,4,
     & 5,3)*ui(i1+4,i2+5,i3+3,c3)+c(i,5,5,3)*ui(i1+5,i2+5,i3+3,c3)+c(
     & i,6,5,3)*ui(i1+6,i2+5,i3+3,c3)+c(i,7,5,3)*ui(i1+7,i2+5,i3+3,c3)
     & +c(i,8,5,3)*ui(i1+8,i2+5,i3+3,c3)+c(i,0,6,3)*ui(i1,i2+6,i3+3,
     & c3)+c(i,1,6,3)*ui(i1+1,i2+6,i3+3,c3)+c(i,2,6,3)*ui(i1+2,i2+6,
     & i3+3,c3)+c(i,3,6,3)*ui(i1+3,i2+6,i3+3,c3)+c(i,4,6,3)*ui(i1+4,
     & i2+6,i3+3,c3)+c(i,5,6,3)*ui(i1+5,i2+6,i3+3,c3)+c(i,6,6,3)*ui(
     & i1+6,i2+6,i3+3,c3)+c(i,7,6,3)*ui(i1+7,i2+6,i3+3,c3)+c(i,8,6,3)*
     & ui(i1+8,i2+6,i3+3,c3)+c(i,0,7,3)*ui(i1,i2+7,i3+3,c3)+c(i,1,7,3)
     & *ui(i1+1,i2+7,i3+3,c3)+c(i,2,7,3)*ui(i1+2,i2+7,i3+3,c3)+c(i,3,
     & 7,3)*ui(i1+3,i2+7,i3+3,c3)+c(i,4,7,3)*ui(i1+4,i2+7,i3+3,c3)+c(
     & i,5,7,3)*ui(i1+5,i2+7,i3+3,c3)+c(i,6,7,3)*ui(i1+6,i2+7,i3+3,c3)
     & +c(i,7,7,3)*ui(i1+7,i2+7,i3+3,c3)+c(i,8,7,3)*ui(i1+8,i2+7,i3+3,
     & c3)+c(i,0,8,3)*ui(i1,i2+8,i3+3,c3)+c(i,1,8,3)*ui(i1+1,i2+8,i3+
     & 3,c3)+c(i,2,8,3)*ui(i1+2,i2+8,i3+3,c3)+c(i,3,8,3)*ui(i1+3,i2+8,
     & i3+3,c3)+c(i,4,8,3)*ui(i1+4,i2+8,i3+3,c3)+c(i,5,8,3)*ui(i1+5,
     & i2+8,i3+3,c3)+c(i,6,8,3)*ui(i1+6,i2+8,i3+3,c3)+c(i,7,8,3)*ui(
     & i1+7,i2+8,i3+3,c3)+c(i,8,8,3)*ui(i1+8,i2+8,i3+3,c3)+c(i,0,0,4)*
     & ui(i1,i2,i3+4,c3)+c(i,1,0,4)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,4)*ui(
     & i1+2,i2,i3+4,c3)+c(i,3,0,4)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,4)*ui(
     & i1+4,i2,i3+4,c3)+c(i,5,0,4)*ui(i1+5,i2,i3+4,c3)+c(i,6,0,4)*ui(
     & i1+6,i2,i3+4,c3)+c(i,7,0,4)*ui(i1+7,i2,i3+4,c3)+c(i,8,0,4)*ui(
     & i1+8,i2,i3+4,c3)+c(i,0,1,4)*ui(i1,i2+1,i3+4,c3)+c(i,1,1,4)*ui(
     & i1+1,i2+1,i3+4,c3)+c(i,2,1,4)*ui(i1+2,i2+1,i3+4,c3)+c(i,3,1,4)*
     & ui(i1+3,i2+1,i3+4,c3)+c(i,4,1,4)*ui(i1+4,i2+1,i3+4,c3)+c(i,5,1,
     & 4)*ui(i1+5,i2+1,i3+4,c3)+c(i,6,1,4)*ui(i1+6,i2+1,i3+4,c3)+c(i,
     & 7,1,4)*ui(i1+7,i2+1,i3+4,c3)+c(i,8,1,4)*ui(i1+8,i2+1,i3+4,c3)+
     & c(i,0,2,4)*ui(i1,i2+2,i3+4,c3)+c(i,1,2,4)*ui(i1+1,i2+2,i3+4,c3)
     & +c(i,2,2,4)*ui(i1+2,i2+2,i3+4,c3)+c(i,3,2,4)*ui(i1+3,i2+2,i3+4,
     & c3)+c(i,4,2,4)*ui(i1+4,i2+2,i3+4,c3)+c(i,5,2,4)*ui(i1+5,i2+2,
     & i3+4,c3)+c(i,6,2,4)*ui(i1+6,i2+2,i3+4,c3)+c(i,7,2,4)*ui(i1+7,
     & i2+2,i3+4,c3)+c(i,8,2,4)*ui(i1+8,i2+2,i3+4,c3)+c(i,0,3,4)*ui(
     & i1,i2+3,i3+4,c3)+c(i,1,3,4)*ui(i1+1,i2+3,i3+4,c3)+c(i,2,3,4)*
     & ui(i1+2,i2+3,i3+4,c3)+c(i,3,3,4)*ui(i1+3,i2+3,i3+4,c3)+c(i,4,3,
     & 4)*ui(i1+4,i2+3,i3+4,c3)+c(i,5,3,4)*ui(i1+5,i2+3,i3+4,c3)+c(i,
     & 6,3,4)*ui(i1+6,i2+3,i3+4,c3)+c(i,7,3,4)*ui(i1+7,i2+3,i3+4,c3)+
     & c(i,8,3,4)*ui(i1+8,i2+3,i3+4,c3)+c(i,0,4,4)*ui(i1,i2+4,i3+4,c3)
     & +c(i,1,4,4)*ui(i1+1,i2+4,i3+4,c3)+c(i,2,4,4)*ui(i1+2,i2+4,i3+4,
     & c3)+c(i,3,4,4)*ui(i1+3,i2+4,i3+4,c3)+c(i,4,4,4)*ui(i1+4,i2+4,
     & i3+4,c3)+c(i,5,4,4)*ui(i1+5,i2+4,i3+4,c3)+c(i,6,4,4)*ui(i1+6,
     & i2+4,i3+4,c3)+c(i,7,4,4)*ui(i1+7,i2+4,i3+4,c3)
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,8,4,4)*ui(i1+8,i2+4,i3+4,c3)+c(i,0,5,4)*ui(i1,i2+
     & 5,i3+4,c3)+c(i,1,5,4)*ui(i1+1,i2+5,i3+4,c3)+c(i,2,5,4)*ui(i1+2,
     & i2+5,i3+4,c3)+c(i,3,5,4)*ui(i1+3,i2+5,i3+4,c3)+c(i,4,5,4)*ui(
     & i1+4,i2+5,i3+4,c3)+c(i,5,5,4)*ui(i1+5,i2+5,i3+4,c3)+c(i,6,5,4)*
     & ui(i1+6,i2+5,i3+4,c3)+c(i,7,5,4)*ui(i1+7,i2+5,i3+4,c3)+c(i,8,5,
     & 4)*ui(i1+8,i2+5,i3+4,c3)+c(i,0,6,4)*ui(i1,i2+6,i3+4,c3)+c(i,1,
     & 6,4)*ui(i1+1,i2+6,i3+4,c3)+c(i,2,6,4)*ui(i1+2,i2+6,i3+4,c3)+c(
     & i,3,6,4)*ui(i1+3,i2+6,i3+4,c3)+c(i,4,6,4)*ui(i1+4,i2+6,i3+4,c3)
     & +c(i,5,6,4)*ui(i1+5,i2+6,i3+4,c3)+c(i,6,6,4)*ui(i1+6,i2+6,i3+4,
     & c3)+c(i,7,6,4)*ui(i1+7,i2+6,i3+4,c3)+c(i,8,6,4)*ui(i1+8,i2+6,
     & i3+4,c3)+c(i,0,7,4)*ui(i1,i2+7,i3+4,c3)+c(i,1,7,4)*ui(i1+1,i2+
     & 7,i3+4,c3)+c(i,2,7,4)*ui(i1+2,i2+7,i3+4,c3)+c(i,3,7,4)*ui(i1+3,
     & i2+7,i3+4,c3)+c(i,4,7,4)*ui(i1+4,i2+7,i3+4,c3)+c(i,5,7,4)*ui(
     & i1+5,i2+7,i3+4,c3)+c(i,6,7,4)*ui(i1+6,i2+7,i3+4,c3)+c(i,7,7,4)*
     & ui(i1+7,i2+7,i3+4,c3)+c(i,8,7,4)*ui(i1+8,i2+7,i3+4,c3)+c(i,0,8,
     & 4)*ui(i1,i2+8,i3+4,c3)+c(i,1,8,4)*ui(i1+1,i2+8,i3+4,c3)+c(i,2,
     & 8,4)*ui(i1+2,i2+8,i3+4,c3)+c(i,3,8,4)*ui(i1+3,i2+8,i3+4,c3)+c(
     & i,4,8,4)*ui(i1+4,i2+8,i3+4,c3)+c(i,5,8,4)*ui(i1+5,i2+8,i3+4,c3)
     & +c(i,6,8,4)*ui(i1+6,i2+8,i3+4,c3)+c(i,7,8,4)*ui(i1+7,i2+8,i3+4,
     & c3)+c(i,8,8,4)*ui(i1+8,i2+8,i3+4,c3)+c(i,0,0,5)*ui(i1,i2,i3+5,
     & c3)+c(i,1,0,5)*ui(i1+1,i2,i3+5,c3)+c(i,2,0,5)*ui(i1+2,i2,i3+5,
     & c3)+c(i,3,0,5)*ui(i1+3,i2,i3+5,c3)+c(i,4,0,5)*ui(i1+4,i2,i3+5,
     & c3)+c(i,5,0,5)*ui(i1+5,i2,i3+5,c3)+c(i,6,0,5)*ui(i1+6,i2,i3+5,
     & c3)+c(i,7,0,5)*ui(i1+7,i2,i3+5,c3)+c(i,8,0,5)*ui(i1+8,i2,i3+5,
     & c3)+c(i,0,1,5)*ui(i1,i2+1,i3+5,c3)+c(i,1,1,5)*ui(i1+1,i2+1,i3+
     & 5,c3)+c(i,2,1,5)*ui(i1+2,i2+1,i3+5,c3)+c(i,3,1,5)*ui(i1+3,i2+1,
     & i3+5,c3)+c(i,4,1,5)*ui(i1+4,i2+1,i3+5,c3)+c(i,5,1,5)*ui(i1+5,
     & i2+1,i3+5,c3)+c(i,6,1,5)*ui(i1+6,i2+1,i3+5,c3)+c(i,7,1,5)*ui(
     & i1+7,i2+1,i3+5,c3)+c(i,8,1,5)*ui(i1+8,i2+1,i3+5,c3)+c(i,0,2,5)*
     & ui(i1,i2+2,i3+5,c3)+c(i,1,2,5)*ui(i1+1,i2+2,i3+5,c3)+c(i,2,2,5)
     & *ui(i1+2,i2+2,i3+5,c3)+c(i,3,2,5)*ui(i1+3,i2+2,i3+5,c3)+c(i,4,
     & 2,5)*ui(i1+4,i2+2,i3+5,c3)+c(i,5,2,5)*ui(i1+5,i2+2,i3+5,c3)+c(
     & i,6,2,5)*ui(i1+6,i2+2,i3+5,c3)+c(i,7,2,5)*ui(i1+7,i2+2,i3+5,c3)
     & +c(i,8,2,5)*ui(i1+8,i2+2,i3+5,c3)+c(i,0,3,5)*ui(i1,i2+3,i3+5,
     & c3)+c(i,1,3,5)*ui(i1+1,i2+3,i3+5,c3)+c(i,2,3,5)*ui(i1+2,i2+3,
     & i3+5,c3)+c(i,3,3,5)*ui(i1+3,i2+3,i3+5,c3)+c(i,4,3,5)*ui(i1+4,
     & i2+3,i3+5,c3)+c(i,5,3,5)*ui(i1+5,i2+3,i3+5,c3)+c(i,6,3,5)*ui(
     & i1+6,i2+3,i3+5,c3)+c(i,7,3,5)*ui(i1+7,i2+3,i3+5,c3)+c(i,8,3,5)*
     & ui(i1+8,i2+3,i3+5,c3)+c(i,0,4,5)*ui(i1,i2+4,i3+5,c3)+c(i,1,4,5)
     & *ui(i1+1,i2+4,i3+5,c3)+c(i,2,4,5)*ui(i1+2,i2+4,i3+5,c3)+c(i,3,
     & 4,5)*ui(i1+3,i2+4,i3+5,c3)+c(i,4,4,5)*ui(i1+4,i2+4,i3+5,c3)+c(
     & i,5,4,5)*ui(i1+5,i2+4,i3+5,c3)+c(i,6,4,5)*ui(i1+6,i2+4,i3+5,c3)
     & +c(i,7,4,5)*ui(i1+7,i2+4,i3+5,c3)+c(i,8,4,5)*ui(i1+8,i2+4,i3+5,
     & c3)+c(i,0,5,5)*ui(i1,i2+5,i3+5,c3)+c(i,1,5,5)*ui(i1+1,i2+5,i3+
     & 5,c3)+c(i,2,5,5)*ui(i1+2,i2+5,i3+5,c3)+c(i,3,5,5)*ui(i1+3,i2+5,
     & i3+5,c3)+c(i,4,5,5)*ui(i1+4,i2+5,i3+5,c3)+c(i,5,5,5)*ui(i1+5,
     & i2+5,i3+5,c3)+c(i,6,5,5)*ui(i1+6,i2+5,i3+5,c3)+c(i,7,5,5)*ui(
     & i1+7,i2+5,i3+5,c3)+c(i,8,5,5)*ui(i1+8,i2+5,i3+5,c3)+c(i,0,6,5)*
     & ui(i1,i2+6,i3+5,c3)+c(i,1,6,5)*ui(i1+1,i2+6,i3+5,c3)+c(i,2,6,5)
     & *ui(i1+2,i2+6,i3+5,c3)+c(i,3,6,5)*ui(i1+3,i2+6,i3+5,c3)+c(i,4,
     & 6,5)*ui(i1+4,i2+6,i3+5,c3)+c(i,5,6,5)*ui(i1+5,i2+6,i3+5,c3)+c(
     & i,6,6,5)*ui(i1+6,i2+6,i3+5,c3)+c(i,7,6,5)*ui(i1+7,i2+6,i3+5,c3)
     & +c(i,8,6,5)*ui(i1+8,i2+6,i3+5,c3)+c(i,0,7,5)*ui(i1,i2+7,i3+5,
     & c3)+c(i,1,7,5)*ui(i1+1,i2+7,i3+5,c3)+c(i,2,7,5)*ui(i1+2,i2+7,
     & i3+5,c3)+c(i,3,7,5)*ui(i1+3,i2+7,i3+5,c3)+c(i,4,7,5)*ui(i1+4,
     & i2+7,i3+5,c3)+c(i,5,7,5)*ui(i1+5,i2+7,i3+5,c3)+c(i,6,7,5)*ui(
     & i1+6,i2+7,i3+5,c3)+c(i,7,7,5)*ui(i1+7,i2+7,i3+5,c3)+c(i,8,7,5)*
     & ui(i1+8,i2+7,i3+5,c3)+c(i,0,8,5)*ui(i1,i2+8,i3+5,c3)+c(i,1,8,5)
     & *ui(i1+1,i2+8,i3+5,c3)+c(i,2,8,5)*ui(i1+2,i2+8,i3+5,c3)+c(i,3,
     & 8,5)*ui(i1+3,i2+8,i3+5,c3)+c(i,4,8,5)*ui(i1+4,i2+8,i3+5,c3)+c(
     & i,5,8,5)*ui(i1+5,i2+8,i3+5,c3)+c(i,6,8,5)*ui(i1+6,i2+8,i3+5,c3)
     & +c(i,7,8,5)*ui(i1+7,i2+8,i3+5,c3)+c(i,8,8,5)*ui(i1+8,i2+8,i3+5,
     & c3)+c(i,0,0,6)*ui(i1,i2,i3+6,c3)+c(i,1,0,6)*ui(i1+1,i2,i3+6,c3)
     & +c(i,2,0,6)*ui(i1+2,i2,i3+6,c3)+c(i,3,0,6)*ui(i1+3,i2,i3+6,c3)+
     & c(i,4,0,6)*ui(i1+4,i2,i3+6,c3)+c(i,5,0,6)*ui(i1+5,i2,i3+6,c3)+
     & c(i,6,0,6)*ui(i1+6,i2,i3+6,c3)+c(i,7,0,6)*ui(i1+7,i2,i3+6,c3)+
     & c(i,8,0,6)*ui(i1+8,i2,i3+6,c3)+c(i,0,1,6)*ui(i1,i2+1,i3+6,c3)+
     & c(i,1,1,6)*ui(i1+1,i2+1,i3+6,c3)+c(i,2,1,6)*ui(i1+2,i2+1,i3+6,
     & c3)+c(i,3,1,6)*ui(i1+3,i2+1,i3+6,c3)+c(i,4,1,6)*ui(i1+4,i2+1,
     & i3+6,c3)+c(i,5,1,6)*ui(i1+5,i2+1,i3+6,c3)+c(i,6,1,6)*ui(i1+6,
     & i2+1,i3+6,c3)+c(i,7,1,6)*ui(i1+7,i2+1,i3+6,c3)+c(i,8,1,6)*ui(
     & i1+8,i2+1,i3+6,c3)+c(i,0,2,6)*ui(i1,i2+2,i3+6,c3)+c(i,1,2,6)*
     & ui(i1+1,i2+2,i3+6,c3)+c(i,2,2,6)*ui(i1+2,i2+2,i3+6,c3)+c(i,3,2,
     & 6)*ui(i1+3,i2+2,i3+6,c3)+c(i,4,2,6)*ui(i1+4,i2+2,i3+6,c3)+c(i,
     & 5,2,6)*ui(i1+5,i2+2,i3+6,c3)+c(i,6,2,6)*ui(i1+6,i2+2,i3+6,c3)+
     & c(i,7,2,6)*ui(i1+7,i2+2,i3+6,c3)+c(i,8,2,6)*ui(i1+8,i2+2,i3+6,
     & c3)+c(i,0,3,6)*ui(i1,i2+3,i3+6,c3)+c(i,1,3,6)*ui(i1+1,i2+3,i3+
     & 6,c3)+c(i,2,3,6)*ui(i1+2,i2+3,i3+6,c3)+c(i,3,3,6)*ui(i1+3,i2+3,
     & i3+6,c3)+c(i,4,3,6)*ui(i1+4,i2+3,i3+6,c3)+c(i,5,3,6)*ui(i1+5,
     & i2+3,i3+6,c3)+c(i,6,3,6)*ui(i1+6,i2+3,i3+6,c3)+c(i,7,3,6)*ui(
     & i1+7,i2+3,i3+6,c3)+c(i,8,3,6)*ui(i1+8,i2+3,i3+6,c3)+c(i,0,4,6)*
     & ui(i1,i2+4,i3+6,c3)+c(i,1,4,6)*ui(i1+1,i2+4,i3+6,c3)+c(i,2,4,6)
     & *ui(i1+2,i2+4,i3+6,c3)+c(i,3,4,6)*ui(i1+3,i2+4,i3+6,c3)+c(i,4,
     & 4,6)*ui(i1+4,i2+4,i3+6,c3)+c(i,5,4,6)*ui(i1+5,i2+4,i3+6,c3)+c(
     & i,6,4,6)*ui(i1+6,i2+4,i3+6,c3)+c(i,7,4,6)*ui(i1+7,i2+4,i3+6,c3)
     & +c(i,8,4,6)*ui(i1+8,i2+4,i3+6,c3)+c(i,0,5,6)*ui(i1,i2+5,i3+6,
     & c3)+c(i,1,5,6)*ui(i1+1,i2+5,i3+6,c3)+c(i,2,5,6)*ui(i1+2,i2+5,
     & i3+6,c3)+c(i,3,5,6)*ui(i1+3,i2+5,i3+6,c3)+c(i,4,5,6)*ui(i1+4,
     & i2+5,i3+6,c3)+c(i,5,5,6)*ui(i1+5,i2+5,i3+6,c3)+c(i,6,5,6)*ui(
     & i1+6,i2+5,i3+6,c3)+c(i,7,5,6)*ui(i1+7,i2+5,i3+6,c3)+c(i,8,5,6)*
     & ui(i1+8,i2+5,i3+6,c3)+c(i,0,6,6)*ui(i1,i2+6,i3+6,c3)+c(i,1,6,6)
     & *ui(i1+1,i2+6,i3+6,c3)+c(i,2,6,6)*ui(i1+2,i2+6,i3+6,c3)+c(i,3,
     & 6,6)*ui(i1+3,i2+6,i3+6,c3)+c(i,4,6,6)*ui(i1+4,i2+6,i3+6,c3)+c(
     & i,5,6,6)*ui(i1+5,i2+6,i3+6,c3)+c(i,6,6,6)*ui(i1+6,i2+6,i3+6,c3)
     & +c(i,7,6,6)*ui(i1+7,i2+6,i3+6,c3)+c(i,8,6,6)*ui(i1+8,i2+6,i3+6,
     & c3)+c(i,0,7,6)*ui(i1,i2+7,i3+6,c3)+c(i,1,7,6)*ui(i1+1,i2+7,i3+
     & 6,c3)+c(i,2,7,6)*ui(i1+2,i2+7,i3+6,c3)
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,3,7,6)*ui(i1+3,i2+7,i3+6,c3)+c(i,4,7,6)*ui(i1+4,
     & i2+7,i3+6,c3)+c(i,5,7,6)*ui(i1+5,i2+7,i3+6,c3)+c(i,6,7,6)*ui(
     & i1+6,i2+7,i3+6,c3)+c(i,7,7,6)*ui(i1+7,i2+7,i3+6,c3)+c(i,8,7,6)*
     & ui(i1+8,i2+7,i3+6,c3)+c(i,0,8,6)*ui(i1,i2+8,i3+6,c3)+c(i,1,8,6)
     & *ui(i1+1,i2+8,i3+6,c3)+c(i,2,8,6)*ui(i1+2,i2+8,i3+6,c3)+c(i,3,
     & 8,6)*ui(i1+3,i2+8,i3+6,c3)+c(i,4,8,6)*ui(i1+4,i2+8,i3+6,c3)+c(
     & i,5,8,6)*ui(i1+5,i2+8,i3+6,c3)+c(i,6,8,6)*ui(i1+6,i2+8,i3+6,c3)
     & +c(i,7,8,6)*ui(i1+7,i2+8,i3+6,c3)+c(i,8,8,6)*ui(i1+8,i2+8,i3+6,
     & c3)+c(i,0,0,7)*ui(i1,i2,i3+7,c3)+c(i,1,0,7)*ui(i1+1,i2,i3+7,c3)
     & +c(i,2,0,7)*ui(i1+2,i2,i3+7,c3)+c(i,3,0,7)*ui(i1+3,i2,i3+7,c3)+
     & c(i,4,0,7)*ui(i1+4,i2,i3+7,c3)+c(i,5,0,7)*ui(i1+5,i2,i3+7,c3)+
     & c(i,6,0,7)*ui(i1+6,i2,i3+7,c3)+c(i,7,0,7)*ui(i1+7,i2,i3+7,c3)+
     & c(i,8,0,7)*ui(i1+8,i2,i3+7,c3)+c(i,0,1,7)*ui(i1,i2+1,i3+7,c3)+
     & c(i,1,1,7)*ui(i1+1,i2+1,i3+7,c3)+c(i,2,1,7)*ui(i1+2,i2+1,i3+7,
     & c3)+c(i,3,1,7)*ui(i1+3,i2+1,i3+7,c3)+c(i,4,1,7)*ui(i1+4,i2+1,
     & i3+7,c3)+c(i,5,1,7)*ui(i1+5,i2+1,i3+7,c3)+c(i,6,1,7)*ui(i1+6,
     & i2+1,i3+7,c3)+c(i,7,1,7)*ui(i1+7,i2+1,i3+7,c3)+c(i,8,1,7)*ui(
     & i1+8,i2+1,i3+7,c3)+c(i,0,2,7)*ui(i1,i2+2,i3+7,c3)+c(i,1,2,7)*
     & ui(i1+1,i2+2,i3+7,c3)+c(i,2,2,7)*ui(i1+2,i2+2,i3+7,c3)+c(i,3,2,
     & 7)*ui(i1+3,i2+2,i3+7,c3)+c(i,4,2,7)*ui(i1+4,i2+2,i3+7,c3)+c(i,
     & 5,2,7)*ui(i1+5,i2+2,i3+7,c3)+c(i,6,2,7)*ui(i1+6,i2+2,i3+7,c3)+
     & c(i,7,2,7)*ui(i1+7,i2+2,i3+7,c3)+c(i,8,2,7)*ui(i1+8,i2+2,i3+7,
     & c3)+c(i,0,3,7)*ui(i1,i2+3,i3+7,c3)+c(i,1,3,7)*ui(i1+1,i2+3,i3+
     & 7,c3)+c(i,2,3,7)*ui(i1+2,i2+3,i3+7,c3)+c(i,3,3,7)*ui(i1+3,i2+3,
     & i3+7,c3)+c(i,4,3,7)*ui(i1+4,i2+3,i3+7,c3)+c(i,5,3,7)*ui(i1+5,
     & i2+3,i3+7,c3)+c(i,6,3,7)*ui(i1+6,i2+3,i3+7,c3)+c(i,7,3,7)*ui(
     & i1+7,i2+3,i3+7,c3)+c(i,8,3,7)*ui(i1+8,i2+3,i3+7,c3)+c(i,0,4,7)*
     & ui(i1,i2+4,i3+7,c3)+c(i,1,4,7)*ui(i1+1,i2+4,i3+7,c3)+c(i,2,4,7)
     & *ui(i1+2,i2+4,i3+7,c3)+c(i,3,4,7)*ui(i1+3,i2+4,i3+7,c3)+c(i,4,
     & 4,7)*ui(i1+4,i2+4,i3+7,c3)+c(i,5,4,7)*ui(i1+5,i2+4,i3+7,c3)+c(
     & i,6,4,7)*ui(i1+6,i2+4,i3+7,c3)+c(i,7,4,7)*ui(i1+7,i2+4,i3+7,c3)
     & +c(i,8,4,7)*ui(i1+8,i2+4,i3+7,c3)+c(i,0,5,7)*ui(i1,i2+5,i3+7,
     & c3)+c(i,1,5,7)*ui(i1+1,i2+5,i3+7,c3)+c(i,2,5,7)*ui(i1+2,i2+5,
     & i3+7,c3)+c(i,3,5,7)*ui(i1+3,i2+5,i3+7,c3)+c(i,4,5,7)*ui(i1+4,
     & i2+5,i3+7,c3)+c(i,5,5,7)*ui(i1+5,i2+5,i3+7,c3)+c(i,6,5,7)*ui(
     & i1+6,i2+5,i3+7,c3)+c(i,7,5,7)*ui(i1+7,i2+5,i3+7,c3)+c(i,8,5,7)*
     & ui(i1+8,i2+5,i3+7,c3)+c(i,0,6,7)*ui(i1,i2+6,i3+7,c3)+c(i,1,6,7)
     & *ui(i1+1,i2+6,i3+7,c3)+c(i,2,6,7)*ui(i1+2,i2+6,i3+7,c3)+c(i,3,
     & 6,7)*ui(i1+3,i2+6,i3+7,c3)+c(i,4,6,7)*ui(i1+4,i2+6,i3+7,c3)+c(
     & i,5,6,7)*ui(i1+5,i2+6,i3+7,c3)+c(i,6,6,7)*ui(i1+6,i2+6,i3+7,c3)
     & +c(i,7,6,7)*ui(i1+7,i2+6,i3+7,c3)+c(i,8,6,7)*ui(i1+8,i2+6,i3+7,
     & c3)+c(i,0,7,7)*ui(i1,i2+7,i3+7,c3)+c(i,1,7,7)*ui(i1+1,i2+7,i3+
     & 7,c3)+c(i,2,7,7)*ui(i1+2,i2+7,i3+7,c3)+c(i,3,7,7)*ui(i1+3,i2+7,
     & i3+7,c3)+c(i,4,7,7)*ui(i1+4,i2+7,i3+7,c3)+c(i,5,7,7)*ui(i1+5,
     & i2+7,i3+7,c3)+c(i,6,7,7)*ui(i1+6,i2+7,i3+7,c3)+c(i,7,7,7)*ui(
     & i1+7,i2+7,i3+7,c3)+c(i,8,7,7)*ui(i1+8,i2+7,i3+7,c3)+c(i,0,8,7)*
     & ui(i1,i2+8,i3+7,c3)+c(i,1,8,7)*ui(i1+1,i2+8,i3+7,c3)+c(i,2,8,7)
     & *ui(i1+2,i2+8,i3+7,c3)+c(i,3,8,7)*ui(i1+3,i2+8,i3+7,c3)+c(i,4,
     & 8,7)*ui(i1+4,i2+8,i3+7,c3)+c(i,5,8,7)*ui(i1+5,i2+8,i3+7,c3)+c(
     & i,6,8,7)*ui(i1+6,i2+8,i3+7,c3)+c(i,7,8,7)*ui(i1+7,i2+8,i3+7,c3)
     & +c(i,8,8,7)*ui(i1+8,i2+8,i3+7,c3)+c(i,0,0,8)*ui(i1,i2,i3+8,c3)+
     & c(i,1,0,8)*ui(i1+1,i2,i3+8,c3)+c(i,2,0,8)*ui(i1+2,i2,i3+8,c3)+
     & c(i,3,0,8)*ui(i1+3,i2,i3+8,c3)+c(i,4,0,8)*ui(i1+4,i2,i3+8,c3)+
     & c(i,5,0,8)*ui(i1+5,i2,i3+8,c3)+c(i,6,0,8)*ui(i1+6,i2,i3+8,c3)+
     & c(i,7,0,8)*ui(i1+7,i2,i3+8,c3)+c(i,8,0,8)*ui(i1+8,i2,i3+8,c3)+
     & c(i,0,1,8)*ui(i1,i2+1,i3+8,c3)+c(i,1,1,8)*ui(i1+1,i2+1,i3+8,c3)
     & +c(i,2,1,8)*ui(i1+2,i2+1,i3+8,c3)+c(i,3,1,8)*ui(i1+3,i2+1,i3+8,
     & c3)+c(i,4,1,8)*ui(i1+4,i2+1,i3+8,c3)+c(i,5,1,8)*ui(i1+5,i2+1,
     & i3+8,c3)+c(i,6,1,8)*ui(i1+6,i2+1,i3+8,c3)+c(i,7,1,8)*ui(i1+7,
     & i2+1,i3+8,c3)+c(i,8,1,8)*ui(i1+8,i2+1,i3+8,c3)+c(i,0,2,8)*ui(
     & i1,i2+2,i3+8,c3)+c(i,1,2,8)*ui(i1+1,i2+2,i3+8,c3)+c(i,2,2,8)*
     & ui(i1+2,i2+2,i3+8,c3)+c(i,3,2,8)*ui(i1+3,i2+2,i3+8,c3)+c(i,4,2,
     & 8)*ui(i1+4,i2+2,i3+8,c3)+c(i,5,2,8)*ui(i1+5,i2+2,i3+8,c3)+c(i,
     & 6,2,8)*ui(i1+6,i2+2,i3+8,c3)+c(i,7,2,8)*ui(i1+7,i2+2,i3+8,c3)+
     & c(i,8,2,8)*ui(i1+8,i2+2,i3+8,c3)+c(i,0,3,8)*ui(i1,i2+3,i3+8,c3)
     & +c(i,1,3,8)*ui(i1+1,i2+3,i3+8,c3)+c(i,2,3,8)*ui(i1+2,i2+3,i3+8,
     & c3)+c(i,3,3,8)*ui(i1+3,i2+3,i3+8,c3)+c(i,4,3,8)*ui(i1+4,i2+3,
     & i3+8,c3)+c(i,5,3,8)*ui(i1+5,i2+3,i3+8,c3)+c(i,6,3,8)*ui(i1+6,
     & i2+3,i3+8,c3)+c(i,7,3,8)*ui(i1+7,i2+3,i3+8,c3)+c(i,8,3,8)*ui(
     & i1+8,i2+3,i3+8,c3)+c(i,0,4,8)*ui(i1,i2+4,i3+8,c3)+c(i,1,4,8)*
     & ui(i1+1,i2+4,i3+8,c3)+c(i,2,4,8)*ui(i1+2,i2+4,i3+8,c3)+c(i,3,4,
     & 8)*ui(i1+3,i2+4,i3+8,c3)+c(i,4,4,8)*ui(i1+4,i2+4,i3+8,c3)+c(i,
     & 5,4,8)*ui(i1+5,i2+4,i3+8,c3)+c(i,6,4,8)*ui(i1+6,i2+4,i3+8,c3)+
     & c(i,7,4,8)*ui(i1+7,i2+4,i3+8,c3)+c(i,8,4,8)*ui(i1+8,i2+4,i3+8,
     & c3)+c(i,0,5,8)*ui(i1,i2+5,i3+8,c3)+c(i,1,5,8)*ui(i1+1,i2+5,i3+
     & 8,c3)+c(i,2,5,8)*ui(i1+2,i2+5,i3+8,c3)+c(i,3,5,8)*ui(i1+3,i2+5,
     & i3+8,c3)+c(i,4,5,8)*ui(i1+4,i2+5,i3+8,c3)+c(i,5,5,8)*ui(i1+5,
     & i2+5,i3+8,c3)+c(i,6,5,8)*ui(i1+6,i2+5,i3+8,c3)+c(i,7,5,8)*ui(
     & i1+7,i2+5,i3+8,c3)+c(i,8,5,8)*ui(i1+8,i2+5,i3+8,c3)+c(i,0,6,8)*
     & ui(i1,i2+6,i3+8,c3)+c(i,1,6,8)*ui(i1+1,i2+6,i3+8,c3)+c(i,2,6,8)
     & *ui(i1+2,i2+6,i3+8,c3)+c(i,3,6,8)*ui(i1+3,i2+6,i3+8,c3)+c(i,4,
     & 6,8)*ui(i1+4,i2+6,i3+8,c3)+c(i,5,6,8)*ui(i1+5,i2+6,i3+8,c3)+c(
     & i,6,6,8)*ui(i1+6,i2+6,i3+8,c3)+c(i,7,6,8)*ui(i1+7,i2+6,i3+8,c3)
     & +c(i,8,6,8)*ui(i1+8,i2+6,i3+8,c3)+c(i,0,7,8)*ui(i1,i2+7,i3+8,
     & c3)+c(i,1,7,8)*ui(i1+1,i2+7,i3+8,c3)+c(i,2,7,8)*ui(i1+2,i2+7,
     & i3+8,c3)+c(i,3,7,8)*ui(i1+3,i2+7,i3+8,c3)+c(i,4,7,8)*ui(i1+4,
     & i2+7,i3+8,c3)+c(i,5,7,8)*ui(i1+5,i2+7,i3+8,c3)+c(i,6,7,8)*ui(
     & i1+6,i2+7,i3+8,c3)+c(i,7,7,8)*ui(i1+7,i2+7,i3+8,c3)+c(i,8,7,8)*
     & ui(i1+8,i2+7,i3+8,c3)+c(i,0,8,8)*ui(i1,i2+8,i3+8,c3)+c(i,1,8,8)
     & *ui(i1+1,i2+8,i3+8,c3)+c(i,2,8,8)*ui(i1+2,i2+8,i3+8,c3)+c(i,3,
     & 8,8)*ui(i1+3,i2+8,i3+8,c3)+c(i,4,8,8)*ui(i1+4,i2+8,i3+8,c3)+c(
     & i,5,8,8)*ui(i1+5,i2+8,i3+8,c3)+c(i,6,8,8)*ui(i1+6,i2+8,i3+8,c3)
     & +c(i,7,8,8)*ui(i1+7,i2+8,i3+8,c3)+c(i,8,8,8)*ui(i1+8,i2+8,i3+8,
     & c3)
             else if( varWidth(i).eq.8 )then
! interp888(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,0,0)*ui(i1,i2,i3,
     & c3)+c(i,1,0,0)*ui(i1+1,i2,i3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3,c3)+
     & c(i,3,0,0)*ui(i1+3,i2,i3,c3)+c(i,4,0,0)*ui(i1+4,i2,i3,c3)+c(i,
     & 5,0,0)*ui(i1+5,i2,i3,c3)+c(i,6,0,0)*ui(i1+6,i2,i3,c3)+c(i,7,0,
     & 0)*ui(i1+7,i2,i3,c3)+c(i,0,1,0)*ui(i1,i2+1,i3,c3)+c(i,1,1,0)*
     & ui(i1+1,i2+1,i3,c3)+c(i,2,1,0)*ui(i1+2,i2+1,i3,c3)+c(i,3,1,0)*
     & ui(i1+3,i2+1,i3,c3)+c(i,4,1,0)*ui(i1+4,i2+1,i3,c3)+c(i,5,1,0)*
     & ui(i1+5,i2+1,i3,c3)+c(i,6,1,0)*ui(i1+6,i2+1,i3,c3)+c(i,7,1,0)*
     & ui(i1+7,i2+1,i3,c3)+c(i,0,2,0)*ui(i1,i2+2,i3,c3)+c(i,1,2,0)*ui(
     & i1+1,i2+2,i3,c3)+c(i,2,2,0)*ui(i1+2,i2+2,i3,c3)+c(i,3,2,0)*ui(
     & i1+3,i2+2,i3,c3)+c(i,4,2,0)*ui(i1+4,i2+2,i3,c3)+c(i,5,2,0)*ui(
     & i1+5,i2+2,i3,c3)+c(i,6,2,0)*ui(i1+6,i2+2,i3,c3)+c(i,7,2,0)*ui(
     & i1+7,i2+2,i3,c3)+c(i,0,3,0)*ui(i1,i2+3,i3,c3)+c(i,1,3,0)*ui(i1+
     & 1,i2+3,i3,c3)+c(i,2,3,0)*ui(i1+2,i2+3,i3,c3)+c(i,3,3,0)*ui(i1+
     & 3,i2+3,i3,c3)+c(i,4,3,0)*ui(i1+4,i2+3,i3,c3)+c(i,5,3,0)*ui(i1+
     & 5,i2+3,i3,c3)+c(i,6,3,0)*ui(i1+6,i2+3,i3,c3)+c(i,7,3,0)*ui(i1+
     & 7,i2+3,i3,c3)+c(i,0,4,0)*ui(i1,i2+4,i3,c3)+c(i,1,4,0)*ui(i1+1,
     & i2+4,i3,c3)+c(i,2,4,0)*ui(i1+2,i2+4,i3,c3)+c(i,3,4,0)*ui(i1+3,
     & i2+4,i3,c3)+c(i,4,4,0)*ui(i1+4,i2+4,i3,c3)+c(i,5,4,0)*ui(i1+5,
     & i2+4,i3,c3)+c(i,6,4,0)*ui(i1+6,i2+4,i3,c3)+c(i,7,4,0)*ui(i1+7,
     & i2+4,i3,c3)+c(i,0,5,0)*ui(i1,i2+5,i3,c3)+c(i,1,5,0)*ui(i1+1,i2+
     & 5,i3,c3)+c(i,2,5,0)*ui(i1+2,i2+5,i3,c3)+c(i,3,5,0)*ui(i1+3,i2+
     & 5,i3,c3)+c(i,4,5,0)*ui(i1+4,i2+5,i3,c3)+c(i,5,5,0)*ui(i1+5,i2+
     & 5,i3,c3)+c(i,6,5,0)*ui(i1+6,i2+5,i3,c3)+c(i,7,5,0)*ui(i1+7,i2+
     & 5,i3,c3)+c(i,0,6,0)*ui(i1,i2+6,i3,c3)+c(i,1,6,0)*ui(i1+1,i2+6,
     & i3,c3)+c(i,2,6,0)*ui(i1+2,i2+6,i3,c3)+c(i,3,6,0)*ui(i1+3,i2+6,
     & i3,c3)+c(i,4,6,0)*ui(i1+4,i2+6,i3,c3)+c(i,5,6,0)*ui(i1+5,i2+6,
     & i3,c3)+c(i,6,6,0)*ui(i1+6,i2+6,i3,c3)+c(i,7,6,0)*ui(i1+7,i2+6,
     & i3,c3)+c(i,0,7,0)*ui(i1,i2+7,i3,c3)+c(i,1,7,0)*ui(i1+1,i2+7,i3,
     & c3)+c(i,2,7,0)*ui(i1+2,i2+7,i3,c3)+c(i,3,7,0)*ui(i1+3,i2+7,i3,
     & c3)+c(i,4,7,0)*ui(i1+4,i2+7,i3,c3)+c(i,5,7,0)*ui(i1+5,i2+7,i3,
     & c3)+c(i,6,7,0)*ui(i1+6,i2+7,i3,c3)+c(i,7,7,0)*ui(i1+7,i2+7,i3,
     & c3)+c(i,0,0,1)*ui(i1,i2,i3+1,c3)+c(i,1,0,1)*ui(i1+1,i2,i3+1,c3)
     & +c(i,2,0,1)*ui(i1+2,i2,i3+1,c3)+c(i,3,0,1)*ui(i1+3,i2,i3+1,c3)+
     & c(i,4,0,1)*ui(i1+4,i2,i3+1,c3)+c(i,5,0,1)*ui(i1+5,i2,i3+1,c3)+
     & c(i,6,0,1)*ui(i1+6,i2,i3+1,c3)+c(i,7,0,1)*ui(i1+7,i2,i3+1,c3)+
     & c(i,0,1,1)*ui(i1,i2+1,i3+1,c3)+c(i,1,1,1)*ui(i1+1,i2+1,i3+1,c3)
     & +c(i,2,1,1)*ui(i1+2,i2+1,i3+1,c3)+c(i,3,1,1)*ui(i1+3,i2+1,i3+1,
     & c3)+c(i,4,1,1)*ui(i1+4,i2+1,i3+1,c3)+c(i,5,1,1)*ui(i1+5,i2+1,
     & i3+1,c3)+c(i,6,1,1)*ui(i1+6,i2+1,i3+1,c3)+c(i,7,1,1)*ui(i1+7,
     & i2+1,i3+1,c3)+c(i,0,2,1)*ui(i1,i2+2,i3+1,c3)+c(i,1,2,1)*ui(i1+
     & 1,i2+2,i3+1,c3)+c(i,2,2,1)*ui(i1+2,i2+2,i3+1,c3)+c(i,3,2,1)*ui(
     & i1+3,i2+2,i3+1,c3)+c(i,4,2,1)*ui(i1+4,i2+2,i3+1,c3)+c(i,5,2,1)*
     & ui(i1+5,i2+2,i3+1,c3)+c(i,6,2,1)*ui(i1+6,i2+2,i3+1,c3)+c(i,7,2,
     & 1)*ui(i1+7,i2+2,i3+1,c3)+c(i,0,3,1)*ui(i1,i2+3,i3+1,c3)+c(i,1,
     & 3,1)*ui(i1+1,i2+3,i3+1,c3)+c(i,2,3,1)*ui(i1+2,i2+3,i3+1,c3)+c(
     & i,3,3,1)*ui(i1+3,i2+3,i3+1,c3)+c(i,4,3,1)*ui(i1+4,i2+3,i3+1,c3)
     & +c(i,5,3,1)*ui(i1+5,i2+3,i3+1,c3)+c(i,6,3,1)*ui(i1+6,i2+3,i3+1,
     & c3)+c(i,7,3,1)*ui(i1+7,i2+3,i3+1,c3)+c(i,0,4,1)*ui(i1,i2+4,i3+
     & 1,c3)+c(i,1,4,1)*ui(i1+1,i2+4,i3+1,c3)+c(i,2,4,1)*ui(i1+2,i2+4,
     & i3+1,c3)+c(i,3,4,1)*ui(i1+3,i2+4,i3+1,c3)+c(i,4,4,1)*ui(i1+4,
     & i2+4,i3+1,c3)+c(i,5,4,1)*ui(i1+5,i2+4,i3+1,c3)+c(i,6,4,1)*ui(
     & i1+6,i2+4,i3+1,c3)+c(i,7,4,1)*ui(i1+7,i2+4,i3+1,c3)+c(i,0,5,1)*
     & ui(i1,i2+5,i3+1,c3)+c(i,1,5,1)*ui(i1+1,i2+5,i3+1,c3)+c(i,2,5,1)
     & *ui(i1+2,i2+5,i3+1,c3)+c(i,3,5,1)*ui(i1+3,i2+5,i3+1,c3)+c(i,4,
     & 5,1)*ui(i1+4,i2+5,i3+1,c3)+c(i,5,5,1)*ui(i1+5,i2+5,i3+1,c3)+c(
     & i,6,5,1)*ui(i1+6,i2+5,i3+1,c3)+c(i,7,5,1)*ui(i1+7,i2+5,i3+1,c3)
     & +c(i,0,6,1)*ui(i1,i2+6,i3+1,c3)+c(i,1,6,1)*ui(i1+1,i2+6,i3+1,
     & c3)+c(i,2,6,1)*ui(i1+2,i2+6,i3+1,c3)+c(i,3,6,1)*ui(i1+3,i2+6,
     & i3+1,c3)+c(i,4,6,1)*ui(i1+4,i2+6,i3+1,c3)+c(i,5,6,1)*ui(i1+5,
     & i2+6,i3+1,c3)+c(i,6,6,1)*ui(i1+6,i2+6,i3+1,c3)+c(i,7,6,1)*ui(
     & i1+7,i2+6,i3+1,c3)+c(i,0,7,1)*ui(i1,i2+7,i3+1,c3)+c(i,1,7,1)*
     & ui(i1+1,i2+7,i3+1,c3)+c(i,2,7,1)*ui(i1+2,i2+7,i3+1,c3)+c(i,3,7,
     & 1)*ui(i1+3,i2+7,i3+1,c3)+c(i,4,7,1)*ui(i1+4,i2+7,i3+1,c3)+c(i,
     & 5,7,1)*ui(i1+5,i2+7,i3+1,c3)+c(i,6,7,1)*ui(i1+6,i2+7,i3+1,c3)+
     & c(i,7,7,1)*ui(i1+7,i2+7,i3+1,c3)+c(i,0,0,2)*ui(i1,i2,i3+2,c3)+
     & c(i,1,0,2)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,2)*ui(i1+2,i2,i3+2,c3)+
     & c(i,3,0,2)*ui(i1+3,i2,i3+2,c3)+c(i,4,0,2)*ui(i1+4,i2,i3+2,c3)+
     & c(i,5,0,2)*ui(i1+5,i2,i3+2,c3)+c(i,6,0,2)*ui(i1+6,i2,i3+2,c3)+
     & c(i,7,0,2)*ui(i1+7,i2,i3+2,c3)+c(i,0,1,2)*ui(i1,i2+1,i3+2,c3)+
     & c(i,1,1,2)*ui(i1+1,i2+1,i3+2,c3)+c(i,2,1,2)*ui(i1+2,i2+1,i3+2,
     & c3)+c(i,3,1,2)*ui(i1+3,i2+1,i3+2,c3)+c(i,4,1,2)*ui(i1+4,i2+1,
     & i3+2,c3)+c(i,5,1,2)*ui(i1+5,i2+1,i3+2,c3)+c(i,6,1,2)*ui(i1+6,
     & i2+1,i3+2,c3)+c(i,7,1,2)*ui(i1+7,i2+1,i3+2,c3)+c(i,0,2,2)*ui(
     & i1,i2+2,i3+2,c3)+c(i,1,2,2)*ui(i1+1,i2+2,i3+2,c3)+c(i,2,2,2)*
     & ui(i1+2,i2+2,i3+2,c3)+c(i,3,2,2)*ui(i1+3,i2+2,i3+2,c3)+c(i,4,2,
     & 2)*ui(i1+4,i2+2,i3+2,c3)+c(i,5,2,2)*ui(i1+5,i2+2,i3+2,c3)+c(i,
     & 6,2,2)*ui(i1+6,i2+2,i3+2,c3)+c(i,7,2,2)*ui(i1+7,i2+2,i3+2,c3)+
     & c(i,0,3,2)*ui(i1,i2+3,i3+2,c3)+c(i,1,3,2)*ui(i1+1,i2+3,i3+2,c3)
     & +c(i,2,3,2)*ui(i1+2,i2+3,i3+2,c3)+c(i,3,3,2)*ui(i1+3,i2+3,i3+2,
     & c3)+c(i,4,3,2)*ui(i1+4,i2+3,i3+2,c3)+c(i,5,3,2)*ui(i1+5,i2+3,
     & i3+2,c3)+c(i,6,3,2)*ui(i1+6,i2+3,i3+2,c3)+c(i,7,3,2)*ui(i1+7,
     & i2+3,i3+2,c3)+c(i,0,4,2)*ui(i1,i2+4,i3+2,c3)+c(i,1,4,2)*ui(i1+
     & 1,i2+4,i3+2,c3)+c(i,2,4,2)*ui(i1+2,i2+4,i3+2,c3)+c(i,3,4,2)*ui(
     & i1+3,i2+4,i3+2,c3)+c(i,4,4,2)*ui(i1+4,i2+4,i3+2,c3)+c(i,5,4,2)*
     & ui(i1+5,i2+4,i3+2,c3)+c(i,6,4,2)*ui(i1+6,i2+4,i3+2,c3)+c(i,7,4,
     & 2)*ui(i1+7,i2+4,i3+2,c3)+c(i,0,5,2)*ui(i1,i2+5,i3+2,c3)+c(i,1,
     & 5,2)*ui(i1+1,i2+5,i3+2,c3)+c(i,2,5,2)*ui(i1+2,i2+5,i3+2,c3)+c(
     & i,3,5,2)*ui(i1+3,i2+5,i3+2,c3)+c(i,4,5,2)*ui(i1+4,i2+5,i3+2,c3)
     & +c(i,5,5,2)*ui(i1+5,i2+5,i3+2,c3)+c(i,6,5,2)*ui(i1+6,i2+5,i3+2,
     & c3)+c(i,7,5,2)*ui(i1+7,i2+5,i3+2,c3)+c(i,0,6,2)*ui(i1,i2+6,i3+
     & 2,c3)+c(i,1,6,2)*ui(i1+1,i2+6,i3+2,c3)+c(i,2,6,2)*ui(i1+2,i2+6,
     & i3+2,c3)+c(i,3,6,2)*ui(i1+3,i2+6,i3+2,c3)+c(i,4,6,2)*ui(i1+4,
     & i2+6,i3+2,c3)+c(i,5,6,2)*ui(i1+5,i2+6,i3+2,c3)+c(i,6,6,2)*ui(
     & i1+6,i2+6,i3+2,c3)+c(i,7,6,2)*ui(i1+7,i2+6,i3+2,c3)
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,0,7,2)*ui(i1,i2+7,i3+2,c3)+c(i,1,7,2)*ui(i1+1,i2+
     & 7,i3+2,c3)+c(i,2,7,2)*ui(i1+2,i2+7,i3+2,c3)+c(i,3,7,2)*ui(i1+3,
     & i2+7,i3+2,c3)+c(i,4,7,2)*ui(i1+4,i2+7,i3+2,c3)+c(i,5,7,2)*ui(
     & i1+5,i2+7,i3+2,c3)+c(i,6,7,2)*ui(i1+6,i2+7,i3+2,c3)+c(i,7,7,2)*
     & ui(i1+7,i2+7,i3+2,c3)+c(i,0,0,3)*ui(i1,i2,i3+3,c3)+c(i,1,0,3)*
     & ui(i1+1,i2,i3+3,c3)+c(i,2,0,3)*ui(i1+2,i2,i3+3,c3)+c(i,3,0,3)*
     & ui(i1+3,i2,i3+3,c3)+c(i,4,0,3)*ui(i1+4,i2,i3+3,c3)+c(i,5,0,3)*
     & ui(i1+5,i2,i3+3,c3)+c(i,6,0,3)*ui(i1+6,i2,i3+3,c3)+c(i,7,0,3)*
     & ui(i1+7,i2,i3+3,c3)+c(i,0,1,3)*ui(i1,i2+1,i3+3,c3)+c(i,1,1,3)*
     & ui(i1+1,i2+1,i3+3,c3)+c(i,2,1,3)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,1,
     & 3)*ui(i1+3,i2+1,i3+3,c3)+c(i,4,1,3)*ui(i1+4,i2+1,i3+3,c3)+c(i,
     & 5,1,3)*ui(i1+5,i2+1,i3+3,c3)+c(i,6,1,3)*ui(i1+6,i2+1,i3+3,c3)+
     & c(i,7,1,3)*ui(i1+7,i2+1,i3+3,c3)+c(i,0,2,3)*ui(i1,i2+2,i3+3,c3)
     & +c(i,1,2,3)*ui(i1+1,i2+2,i3+3,c3)+c(i,2,2,3)*ui(i1+2,i2+2,i3+3,
     & c3)+c(i,3,2,3)*ui(i1+3,i2+2,i3+3,c3)+c(i,4,2,3)*ui(i1+4,i2+2,
     & i3+3,c3)+c(i,5,2,3)*ui(i1+5,i2+2,i3+3,c3)+c(i,6,2,3)*ui(i1+6,
     & i2+2,i3+3,c3)+c(i,7,2,3)*ui(i1+7,i2+2,i3+3,c3)+c(i,0,3,3)*ui(
     & i1,i2+3,i3+3,c3)+c(i,1,3,3)*ui(i1+1,i2+3,i3+3,c3)+c(i,2,3,3)*
     & ui(i1+2,i2+3,i3+3,c3)+c(i,3,3,3)*ui(i1+3,i2+3,i3+3,c3)+c(i,4,3,
     & 3)*ui(i1+4,i2+3,i3+3,c3)+c(i,5,3,3)*ui(i1+5,i2+3,i3+3,c3)+c(i,
     & 6,3,3)*ui(i1+6,i2+3,i3+3,c3)+c(i,7,3,3)*ui(i1+7,i2+3,i3+3,c3)+
     & c(i,0,4,3)*ui(i1,i2+4,i3+3,c3)+c(i,1,4,3)*ui(i1+1,i2+4,i3+3,c3)
     & +c(i,2,4,3)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,4,3)*ui(i1+3,i2+4,i3+3,
     & c3)+c(i,4,4,3)*ui(i1+4,i2+4,i3+3,c3)+c(i,5,4,3)*ui(i1+5,i2+4,
     & i3+3,c3)+c(i,6,4,3)*ui(i1+6,i2+4,i3+3,c3)+c(i,7,4,3)*ui(i1+7,
     & i2+4,i3+3,c3)+c(i,0,5,3)*ui(i1,i2+5,i3+3,c3)+c(i,1,5,3)*ui(i1+
     & 1,i2+5,i3+3,c3)+c(i,2,5,3)*ui(i1+2,i2+5,i3+3,c3)+c(i,3,5,3)*ui(
     & i1+3,i2+5,i3+3,c3)+c(i,4,5,3)*ui(i1+4,i2+5,i3+3,c3)+c(i,5,5,3)*
     & ui(i1+5,i2+5,i3+3,c3)+c(i,6,5,3)*ui(i1+6,i2+5,i3+3,c3)+c(i,7,5,
     & 3)*ui(i1+7,i2+5,i3+3,c3)+c(i,0,6,3)*ui(i1,i2+6,i3+3,c3)+c(i,1,
     & 6,3)*ui(i1+1,i2+6,i3+3,c3)+c(i,2,6,3)*ui(i1+2,i2+6,i3+3,c3)+c(
     & i,3,6,3)*ui(i1+3,i2+6,i3+3,c3)+c(i,4,6,3)*ui(i1+4,i2+6,i3+3,c3)
     & +c(i,5,6,3)*ui(i1+5,i2+6,i3+3,c3)+c(i,6,6,3)*ui(i1+6,i2+6,i3+3,
     & c3)+c(i,7,6,3)*ui(i1+7,i2+6,i3+3,c3)+c(i,0,7,3)*ui(i1,i2+7,i3+
     & 3,c3)+c(i,1,7,3)*ui(i1+1,i2+7,i3+3,c3)+c(i,2,7,3)*ui(i1+2,i2+7,
     & i3+3,c3)+c(i,3,7,3)*ui(i1+3,i2+7,i3+3,c3)+c(i,4,7,3)*ui(i1+4,
     & i2+7,i3+3,c3)+c(i,5,7,3)*ui(i1+5,i2+7,i3+3,c3)+c(i,6,7,3)*ui(
     & i1+6,i2+7,i3+3,c3)+c(i,7,7,3)*ui(i1+7,i2+7,i3+3,c3)+c(i,0,0,4)*
     & ui(i1,i2,i3+4,c3)+c(i,1,0,4)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,4)*ui(
     & i1+2,i2,i3+4,c3)+c(i,3,0,4)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,4)*ui(
     & i1+4,i2,i3+4,c3)+c(i,5,0,4)*ui(i1+5,i2,i3+4,c3)+c(i,6,0,4)*ui(
     & i1+6,i2,i3+4,c3)+c(i,7,0,4)*ui(i1+7,i2,i3+4,c3)+c(i,0,1,4)*ui(
     & i1,i2+1,i3+4,c3)+c(i,1,1,4)*ui(i1+1,i2+1,i3+4,c3)+c(i,2,1,4)*
     & ui(i1+2,i2+1,i3+4,c3)+c(i,3,1,4)*ui(i1+3,i2+1,i3+4,c3)+c(i,4,1,
     & 4)*ui(i1+4,i2+1,i3+4,c3)+c(i,5,1,4)*ui(i1+5,i2+1,i3+4,c3)+c(i,
     & 6,1,4)*ui(i1+6,i2+1,i3+4,c3)+c(i,7,1,4)*ui(i1+7,i2+1,i3+4,c3)+
     & c(i,0,2,4)*ui(i1,i2+2,i3+4,c3)+c(i,1,2,4)*ui(i1+1,i2+2,i3+4,c3)
     & +c(i,2,2,4)*ui(i1+2,i2+2,i3+4,c3)+c(i,3,2,4)*ui(i1+3,i2+2,i3+4,
     & c3)+c(i,4,2,4)*ui(i1+4,i2+2,i3+4,c3)+c(i,5,2,4)*ui(i1+5,i2+2,
     & i3+4,c3)+c(i,6,2,4)*ui(i1+6,i2+2,i3+4,c3)+c(i,7,2,4)*ui(i1+7,
     & i2+2,i3+4,c3)+c(i,0,3,4)*ui(i1,i2+3,i3+4,c3)+c(i,1,3,4)*ui(i1+
     & 1,i2+3,i3+4,c3)+c(i,2,3,4)*ui(i1+2,i2+3,i3+4,c3)+c(i,3,3,4)*ui(
     & i1+3,i2+3,i3+4,c3)+c(i,4,3,4)*ui(i1+4,i2+3,i3+4,c3)+c(i,5,3,4)*
     & ui(i1+5,i2+3,i3+4,c3)+c(i,6,3,4)*ui(i1+6,i2+3,i3+4,c3)+c(i,7,3,
     & 4)*ui(i1+7,i2+3,i3+4,c3)+c(i,0,4,4)*ui(i1,i2+4,i3+4,c3)+c(i,1,
     & 4,4)*ui(i1+1,i2+4,i3+4,c3)+c(i,2,4,4)*ui(i1+2,i2+4,i3+4,c3)+c(
     & i,3,4,4)*ui(i1+3,i2+4,i3+4,c3)+c(i,4,4,4)*ui(i1+4,i2+4,i3+4,c3)
     & +c(i,5,4,4)*ui(i1+5,i2+4,i3+4,c3)+c(i,6,4,4)*ui(i1+6,i2+4,i3+4,
     & c3)+c(i,7,4,4)*ui(i1+7,i2+4,i3+4,c3)+c(i,0,5,4)*ui(i1,i2+5,i3+
     & 4,c3)+c(i,1,5,4)*ui(i1+1,i2+5,i3+4,c3)+c(i,2,5,4)*ui(i1+2,i2+5,
     & i3+4,c3)+c(i,3,5,4)*ui(i1+3,i2+5,i3+4,c3)+c(i,4,5,4)*ui(i1+4,
     & i2+5,i3+4,c3)+c(i,5,5,4)*ui(i1+5,i2+5,i3+4,c3)+c(i,6,5,4)*ui(
     & i1+6,i2+5,i3+4,c3)+c(i,7,5,4)*ui(i1+7,i2+5,i3+4,c3)+c(i,0,6,4)*
     & ui(i1,i2+6,i3+4,c3)+c(i,1,6,4)*ui(i1+1,i2+6,i3+4,c3)+c(i,2,6,4)
     & *ui(i1+2,i2+6,i3+4,c3)+c(i,3,6,4)*ui(i1+3,i2+6,i3+4,c3)+c(i,4,
     & 6,4)*ui(i1+4,i2+6,i3+4,c3)+c(i,5,6,4)*ui(i1+5,i2+6,i3+4,c3)+c(
     & i,6,6,4)*ui(i1+6,i2+6,i3+4,c3)+c(i,7,6,4)*ui(i1+7,i2+6,i3+4,c3)
     & +c(i,0,7,4)*ui(i1,i2+7,i3+4,c3)+c(i,1,7,4)*ui(i1+1,i2+7,i3+4,
     & c3)+c(i,2,7,4)*ui(i1+2,i2+7,i3+4,c3)+c(i,3,7,4)*ui(i1+3,i2+7,
     & i3+4,c3)+c(i,4,7,4)*ui(i1+4,i2+7,i3+4,c3)+c(i,5,7,4)*ui(i1+5,
     & i2+7,i3+4,c3)+c(i,6,7,4)*ui(i1+6,i2+7,i3+4,c3)+c(i,7,7,4)*ui(
     & i1+7,i2+7,i3+4,c3)+c(i,0,0,5)*ui(i1,i2,i3+5,c3)+c(i,1,0,5)*ui(
     & i1+1,i2,i3+5,c3)+c(i,2,0,5)*ui(i1+2,i2,i3+5,c3)+c(i,3,0,5)*ui(
     & i1+3,i2,i3+5,c3)+c(i,4,0,5)*ui(i1+4,i2,i3+5,c3)+c(i,5,0,5)*ui(
     & i1+5,i2,i3+5,c3)+c(i,6,0,5)*ui(i1+6,i2,i3+5,c3)+c(i,7,0,5)*ui(
     & i1+7,i2,i3+5,c3)+c(i,0,1,5)*ui(i1,i2+1,i3+5,c3)+c(i,1,1,5)*ui(
     & i1+1,i2+1,i3+5,c3)+c(i,2,1,5)*ui(i1+2,i2+1,i3+5,c3)+c(i,3,1,5)*
     & ui(i1+3,i2+1,i3+5,c3)+c(i,4,1,5)*ui(i1+4,i2+1,i3+5,c3)+c(i,5,1,
     & 5)*ui(i1+5,i2+1,i3+5,c3)+c(i,6,1,5)*ui(i1+6,i2+1,i3+5,c3)+c(i,
     & 7,1,5)*ui(i1+7,i2+1,i3+5,c3)+c(i,0,2,5)*ui(i1,i2+2,i3+5,c3)+c(
     & i,1,2,5)*ui(i1+1,i2+2,i3+5,c3)+c(i,2,2,5)*ui(i1+2,i2+2,i3+5,c3)
     & +c(i,3,2,5)*ui(i1+3,i2+2,i3+5,c3)+c(i,4,2,5)*ui(i1+4,i2+2,i3+5,
     & c3)+c(i,5,2,5)*ui(i1+5,i2+2,i3+5,c3)+c(i,6,2,5)*ui(i1+6,i2+2,
     & i3+5,c3)+c(i,7,2,5)*ui(i1+7,i2+2,i3+5,c3)+c(i,0,3,5)*ui(i1,i2+
     & 3,i3+5,c3)+c(i,1,3,5)*ui(i1+1,i2+3,i3+5,c3)+c(i,2,3,5)*ui(i1+2,
     & i2+3,i3+5,c3)+c(i,3,3,5)*ui(i1+3,i2+3,i3+5,c3)+c(i,4,3,5)*ui(
     & i1+4,i2+3,i3+5,c3)+c(i,5,3,5)*ui(i1+5,i2+3,i3+5,c3)+c(i,6,3,5)*
     & ui(i1+6,i2+3,i3+5,c3)+c(i,7,3,5)*ui(i1+7,i2+3,i3+5,c3)+c(i,0,4,
     & 5)*ui(i1,i2+4,i3+5,c3)+c(i,1,4,5)*ui(i1+1,i2+4,i3+5,c3)+c(i,2,
     & 4,5)*ui(i1+2,i2+4,i3+5,c3)+c(i,3,4,5)*ui(i1+3,i2+4,i3+5,c3)+c(
     & i,4,4,5)*ui(i1+4,i2+4,i3+5,c3)+c(i,5,4,5)*ui(i1+5,i2+4,i3+5,c3)
     & +c(i,6,4,5)*ui(i1+6,i2+4,i3+5,c3)+c(i,7,4,5)*ui(i1+7,i2+4,i3+5,
     & c3)+c(i,0,5,5)*ui(i1,i2+5,i3+5,c3)+c(i,1,5,5)*ui(i1+1,i2+5,i3+
     & 5,c3)+c(i,2,5,5)*ui(i1+2,i2+5,i3+5,c3)+c(i,3,5,5)*ui(i1+3,i2+5,
     & i3+5,c3)+c(i,4,5,5)*ui(i1+4,i2+5,i3+5,c3)+c(i,5,5,5)*ui(i1+5,
     & i2+5,i3+5,c3)+c(i,6,5,5)*ui(i1+6,i2+5,i3+5,c3)+c(i,7,5,5)*ui(
     & i1+7,i2+5,i3+5,c3)
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,0,6,5)*ui(i1,i2+6,i3+5,c3)+c(i,1,6,5)*ui(i1+1,i2+
     & 6,i3+5,c3)+c(i,2,6,5)*ui(i1+2,i2+6,i3+5,c3)+c(i,3,6,5)*ui(i1+3,
     & i2+6,i3+5,c3)+c(i,4,6,5)*ui(i1+4,i2+6,i3+5,c3)+c(i,5,6,5)*ui(
     & i1+5,i2+6,i3+5,c3)+c(i,6,6,5)*ui(i1+6,i2+6,i3+5,c3)+c(i,7,6,5)*
     & ui(i1+7,i2+6,i3+5,c3)+c(i,0,7,5)*ui(i1,i2+7,i3+5,c3)+c(i,1,7,5)
     & *ui(i1+1,i2+7,i3+5,c3)+c(i,2,7,5)*ui(i1+2,i2+7,i3+5,c3)+c(i,3,
     & 7,5)*ui(i1+3,i2+7,i3+5,c3)+c(i,4,7,5)*ui(i1+4,i2+7,i3+5,c3)+c(
     & i,5,7,5)*ui(i1+5,i2+7,i3+5,c3)+c(i,6,7,5)*ui(i1+6,i2+7,i3+5,c3)
     & +c(i,7,7,5)*ui(i1+7,i2+7,i3+5,c3)+c(i,0,0,6)*ui(i1,i2,i3+6,c3)+
     & c(i,1,0,6)*ui(i1+1,i2,i3+6,c3)+c(i,2,0,6)*ui(i1+2,i2,i3+6,c3)+
     & c(i,3,0,6)*ui(i1+3,i2,i3+6,c3)+c(i,4,0,6)*ui(i1+4,i2,i3+6,c3)+
     & c(i,5,0,6)*ui(i1+5,i2,i3+6,c3)+c(i,6,0,6)*ui(i1+6,i2,i3+6,c3)+
     & c(i,7,0,6)*ui(i1+7,i2,i3+6,c3)+c(i,0,1,6)*ui(i1,i2+1,i3+6,c3)+
     & c(i,1,1,6)*ui(i1+1,i2+1,i3+6,c3)+c(i,2,1,6)*ui(i1+2,i2+1,i3+6,
     & c3)+c(i,3,1,6)*ui(i1+3,i2+1,i3+6,c3)+c(i,4,1,6)*ui(i1+4,i2+1,
     & i3+6,c3)+c(i,5,1,6)*ui(i1+5,i2+1,i3+6,c3)+c(i,6,1,6)*ui(i1+6,
     & i2+1,i3+6,c3)+c(i,7,1,6)*ui(i1+7,i2+1,i3+6,c3)+c(i,0,2,6)*ui(
     & i1,i2+2,i3+6,c3)+c(i,1,2,6)*ui(i1+1,i2+2,i3+6,c3)+c(i,2,2,6)*
     & ui(i1+2,i2+2,i3+6,c3)+c(i,3,2,6)*ui(i1+3,i2+2,i3+6,c3)+c(i,4,2,
     & 6)*ui(i1+4,i2+2,i3+6,c3)+c(i,5,2,6)*ui(i1+5,i2+2,i3+6,c3)+c(i,
     & 6,2,6)*ui(i1+6,i2+2,i3+6,c3)+c(i,7,2,6)*ui(i1+7,i2+2,i3+6,c3)+
     & c(i,0,3,6)*ui(i1,i2+3,i3+6,c3)+c(i,1,3,6)*ui(i1+1,i2+3,i3+6,c3)
     & +c(i,2,3,6)*ui(i1+2,i2+3,i3+6,c3)+c(i,3,3,6)*ui(i1+3,i2+3,i3+6,
     & c3)+c(i,4,3,6)*ui(i1+4,i2+3,i3+6,c3)+c(i,5,3,6)*ui(i1+5,i2+3,
     & i3+6,c3)+c(i,6,3,6)*ui(i1+6,i2+3,i3+6,c3)+c(i,7,3,6)*ui(i1+7,
     & i2+3,i3+6,c3)+c(i,0,4,6)*ui(i1,i2+4,i3+6,c3)+c(i,1,4,6)*ui(i1+
     & 1,i2+4,i3+6,c3)+c(i,2,4,6)*ui(i1+2,i2+4,i3+6,c3)+c(i,3,4,6)*ui(
     & i1+3,i2+4,i3+6,c3)+c(i,4,4,6)*ui(i1+4,i2+4,i3+6,c3)+c(i,5,4,6)*
     & ui(i1+5,i2+4,i3+6,c3)+c(i,6,4,6)*ui(i1+6,i2+4,i3+6,c3)+c(i,7,4,
     & 6)*ui(i1+7,i2+4,i3+6,c3)+c(i,0,5,6)*ui(i1,i2+5,i3+6,c3)+c(i,1,
     & 5,6)*ui(i1+1,i2+5,i3+6,c3)+c(i,2,5,6)*ui(i1+2,i2+5,i3+6,c3)+c(
     & i,3,5,6)*ui(i1+3,i2+5,i3+6,c3)+c(i,4,5,6)*ui(i1+4,i2+5,i3+6,c3)
     & +c(i,5,5,6)*ui(i1+5,i2+5,i3+6,c3)+c(i,6,5,6)*ui(i1+6,i2+5,i3+6,
     & c3)+c(i,7,5,6)*ui(i1+7,i2+5,i3+6,c3)+c(i,0,6,6)*ui(i1,i2+6,i3+
     & 6,c3)+c(i,1,6,6)*ui(i1+1,i2+6,i3+6,c3)+c(i,2,6,6)*ui(i1+2,i2+6,
     & i3+6,c3)+c(i,3,6,6)*ui(i1+3,i2+6,i3+6,c3)+c(i,4,6,6)*ui(i1+4,
     & i2+6,i3+6,c3)+c(i,5,6,6)*ui(i1+5,i2+6,i3+6,c3)+c(i,6,6,6)*ui(
     & i1+6,i2+6,i3+6,c3)+c(i,7,6,6)*ui(i1+7,i2+6,i3+6,c3)+c(i,0,7,6)*
     & ui(i1,i2+7,i3+6,c3)+c(i,1,7,6)*ui(i1+1,i2+7,i3+6,c3)+c(i,2,7,6)
     & *ui(i1+2,i2+7,i3+6,c3)+c(i,3,7,6)*ui(i1+3,i2+7,i3+6,c3)+c(i,4,
     & 7,6)*ui(i1+4,i2+7,i3+6,c3)+c(i,5,7,6)*ui(i1+5,i2+7,i3+6,c3)+c(
     & i,6,7,6)*ui(i1+6,i2+7,i3+6,c3)+c(i,7,7,6)*ui(i1+7,i2+7,i3+6,c3)
     & +c(i,0,0,7)*ui(i1,i2,i3+7,c3)+c(i,1,0,7)*ui(i1+1,i2,i3+7,c3)+c(
     & i,2,0,7)*ui(i1+2,i2,i3+7,c3)+c(i,3,0,7)*ui(i1+3,i2,i3+7,c3)+c(
     & i,4,0,7)*ui(i1+4,i2,i3+7,c3)+c(i,5,0,7)*ui(i1+5,i2,i3+7,c3)+c(
     & i,6,0,7)*ui(i1+6,i2,i3+7,c3)+c(i,7,0,7)*ui(i1+7,i2,i3+7,c3)+c(
     & i,0,1,7)*ui(i1,i2+1,i3+7,c3)+c(i,1,1,7)*ui(i1+1,i2+1,i3+7,c3)+
     & c(i,2,1,7)*ui(i1+2,i2+1,i3+7,c3)+c(i,3,1,7)*ui(i1+3,i2+1,i3+7,
     & c3)+c(i,4,1,7)*ui(i1+4,i2+1,i3+7,c3)+c(i,5,1,7)*ui(i1+5,i2+1,
     & i3+7,c3)+c(i,6,1,7)*ui(i1+6,i2+1,i3+7,c3)+c(i,7,1,7)*ui(i1+7,
     & i2+1,i3+7,c3)+c(i,0,2,7)*ui(i1,i2+2,i3+7,c3)+c(i,1,2,7)*ui(i1+
     & 1,i2+2,i3+7,c3)+c(i,2,2,7)*ui(i1+2,i2+2,i3+7,c3)+c(i,3,2,7)*ui(
     & i1+3,i2+2,i3+7,c3)+c(i,4,2,7)*ui(i1+4,i2+2,i3+7,c3)+c(i,5,2,7)*
     & ui(i1+5,i2+2,i3+7,c3)+c(i,6,2,7)*ui(i1+6,i2+2,i3+7,c3)+c(i,7,2,
     & 7)*ui(i1+7,i2+2,i3+7,c3)+c(i,0,3,7)*ui(i1,i2+3,i3+7,c3)+c(i,1,
     & 3,7)*ui(i1+1,i2+3,i3+7,c3)+c(i,2,3,7)*ui(i1+2,i2+3,i3+7,c3)+c(
     & i,3,3,7)*ui(i1+3,i2+3,i3+7,c3)+c(i,4,3,7)*ui(i1+4,i2+3,i3+7,c3)
     & +c(i,5,3,7)*ui(i1+5,i2+3,i3+7,c3)+c(i,6,3,7)*ui(i1+6,i2+3,i3+7,
     & c3)+c(i,7,3,7)*ui(i1+7,i2+3,i3+7,c3)+c(i,0,4,7)*ui(i1,i2+4,i3+
     & 7,c3)+c(i,1,4,7)*ui(i1+1,i2+4,i3+7,c3)+c(i,2,4,7)*ui(i1+2,i2+4,
     & i3+7,c3)+c(i,3,4,7)*ui(i1+3,i2+4,i3+7,c3)+c(i,4,4,7)*ui(i1+4,
     & i2+4,i3+7,c3)+c(i,5,4,7)*ui(i1+5,i2+4,i3+7,c3)+c(i,6,4,7)*ui(
     & i1+6,i2+4,i3+7,c3)+c(i,7,4,7)*ui(i1+7,i2+4,i3+7,c3)+c(i,0,5,7)*
     & ui(i1,i2+5,i3+7,c3)+c(i,1,5,7)*ui(i1+1,i2+5,i3+7,c3)+c(i,2,5,7)
     & *ui(i1+2,i2+5,i3+7,c3)+c(i,3,5,7)*ui(i1+3,i2+5,i3+7,c3)+c(i,4,
     & 5,7)*ui(i1+4,i2+5,i3+7,c3)+c(i,5,5,7)*ui(i1+5,i2+5,i3+7,c3)+c(
     & i,6,5,7)*ui(i1+6,i2+5,i3+7,c3)+c(i,7,5,7)*ui(i1+7,i2+5,i3+7,c3)
     & +c(i,0,6,7)*ui(i1,i2+6,i3+7,c3)+c(i,1,6,7)*ui(i1+1,i2+6,i3+7,
     & c3)+c(i,2,6,7)*ui(i1+2,i2+6,i3+7,c3)+c(i,3,6,7)*ui(i1+3,i2+6,
     & i3+7,c3)+c(i,4,6,7)*ui(i1+4,i2+6,i3+7,c3)+c(i,5,6,7)*ui(i1+5,
     & i2+6,i3+7,c3)+c(i,6,6,7)*ui(i1+6,i2+6,i3+7,c3)+c(i,7,6,7)*ui(
     & i1+7,i2+6,i3+7,c3)+c(i,0,7,7)*ui(i1,i2+7,i3+7,c3)+c(i,1,7,7)*
     & ui(i1+1,i2+7,i3+7,c3)+c(i,2,7,7)*ui(i1+2,i2+7,i3+7,c3)+c(i,3,7,
     & 7)*ui(i1+3,i2+7,i3+7,c3)+c(i,4,7,7)*ui(i1+4,i2+7,i3+7,c3)+c(i,
     & 5,7,7)*ui(i1+5,i2+7,i3+7,c3)+c(i,6,7,7)*ui(i1+6,i2+7,i3+7,c3)+
     & c(i,7,7,7)*ui(i1+7,i2+7,i3+7,c3)
             else
               write(*,*) 'ERROR varWidth=',varWidth(i)
               stop 151
             end if
! endLoops3d()
             end do
             end do
         else if( width(1).eq.3 .and. width(2).eq.3 .and. width(3)
     & .eq.3 )then
! loops3d($interp333(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)
            if( c3a.eq.c3b )then
              do c3=c3a,c3b
              do i=nia,nib
! interp333(ug(ip(i,1),ip(i,2),ip(i,3),c3))
                i1=il(i,1)
                i2=il(i,2)
                i3=il(i,3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,0,0)*ui(i1,i2,
     & i3,c3)+c(i,1,0,0)*ui(i1+1,i2,i3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3,
     & c3)+c(i,0,1,0)*ui(i1,i2+1,i3,c3)+c(i,1,1,0)*ui(i1+1,i2+1,i3,c3)
     & +c(i,2,1,0)*ui(i1+2,i2+1,i3,c3)+c(i,0,2,0)*ui(i1,i2+2,i3,c3)+c(
     & i,1,2,0)*ui(i1+1,i2+2,i3,c3)+c(i,2,2,0)*ui(i1+2,i2+2,i3,c3)+c(
     & i,0,0,1)*ui(i1,i2,i3+1,c3)+c(i,1,0,1)*ui(i1+1,i2,i3+1,c3)+c(i,
     & 2,0,1)*ui(i1+2,i2,i3+1,c3)+c(i,0,1,1)*ui(i1,i2+1,i3+1,c3)+c(i,
     & 1,1,1)*ui(i1+1,i2+1,i3+1,c3)+c(i,2,1,1)*ui(i1+2,i2+1,i3+1,c3)+
     & c(i,0,2,1)*ui(i1,i2+2,i3+1,c3)+c(i,1,2,1)*ui(i1+1,i2+2,i3+1,c3)
     & +c(i,2,2,1)*ui(i1+2,i2+2,i3+1,c3)+c(i,0,0,2)*ui(i1,i2,i3+2,c3)+
     & c(i,1,0,2)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,2)*ui(i1+2,i2,i3+2,c3)+
     & c(i,0,1,2)*ui(i1,i2+1,i3+2,c3)+c(i,1,1,2)*ui(i1+1,i2+1,i3+2,c3)
     & +c(i,2,1,2)*ui(i1+2,i2+1,i3+2,c3)+c(i,0,2,2)*ui(i1,i2+2,i3+2,
     & c3)+c(i,1,2,2)*ui(i1+1,i2+2,i3+2,c3)+c(i,2,2,2)*ui(i1+2,i2+2,
     & i3+2,c3)


              end do
              end do
            else
              ! put "c" loop as inner loop, this seems to be faster
              do i=nia,nib
              do c3=c3a,c3b
! interp333(ug(ip(i,1),ip(i,2),ip(i,3),c3))
                i1=il(i,1)
                i2=il(i,2)
                i3=il(i,3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,0,0)*ui(i1,i2,
     & i3,c3)+c(i,1,0,0)*ui(i1+1,i2,i3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3,
     & c3)+c(i,0,1,0)*ui(i1,i2+1,i3,c3)+c(i,1,1,0)*ui(i1+1,i2+1,i3,c3)
     & +c(i,2,1,0)*ui(i1+2,i2+1,i3,c3)+c(i,0,2,0)*ui(i1,i2+2,i3,c3)+c(
     & i,1,2,0)*ui(i1+1,i2+2,i3,c3)+c(i,2,2,0)*ui(i1+2,i2+2,i3,c3)+c(
     & i,0,0,1)*ui(i1,i2,i3+1,c3)+c(i,1,0,1)*ui(i1+1,i2,i3+1,c3)+c(i,
     & 2,0,1)*ui(i1+2,i2,i3+1,c3)+c(i,0,1,1)*ui(i1,i2+1,i3+1,c3)+c(i,
     & 1,1,1)*ui(i1+1,i2+1,i3+1,c3)+c(i,2,1,1)*ui(i1+2,i2+1,i3+1,c3)+
     & c(i,0,2,1)*ui(i1,i2+2,i3+1,c3)+c(i,1,2,1)*ui(i1+1,i2+2,i3+1,c3)
     & +c(i,2,2,1)*ui(i1+2,i2+2,i3+1,c3)+c(i,0,0,2)*ui(i1,i2,i3+2,c3)+
     & c(i,1,0,2)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,2)*ui(i1+2,i2,i3+2,c3)+
     & c(i,0,1,2)*ui(i1,i2+1,i3+2,c3)+c(i,1,1,2)*ui(i1+1,i2+1,i3+2,c3)
     & +c(i,2,1,2)*ui(i1+2,i2+1,i3+2,c3)+c(i,0,2,2)*ui(i1,i2+2,i3+2,
     & c3)+c(i,1,2,2)*ui(i1+1,i2+2,i3+2,c3)+c(i,2,2,2)*ui(i1+2,i2+2,
     & i3+2,c3)


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
! loops3d($interp222(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)
            if( c3a.eq.c3b )then
              do c3=c3a,c3b
              do i=nia,nib
! interp222(ug(ip(i,1),ip(i,2),ip(i,3),c3))
                i1=il(i,1)
                i2=il(i,2)
                i3=il(i,3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,0,0)*ui(i1,i2,
     & i3,c3)+c(i,1,0,0)*ui(i1+1,i2,i3,c3)+c(i,0,1,0)*ui(i1,i2+1,i3,
     & c3)+c(i,1,1,0)*ui(i1+1,i2+1,i3,c3)+c(i,0,0,1)*ui(i1,i2,i3+1,c3)
     & +c(i,1,0,1)*ui(i1+1,i2,i3+1,c3)+c(i,0,1,1)*ui(i1,i2+1,i3+1,c3)+
     & c(i,1,1,1)*ui(i1+1,i2+1,i3+1,c3)


              end do
              end do
            else
              ! put "c" loop as inner loop, this seems to be faster
              do i=nia,nib
              do c3=c3a,c3b
! interp222(ug(ip(i,1),ip(i,2),ip(i,3),c3))
                i1=il(i,1)
                i2=il(i,2)
                i3=il(i,3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,0,0)*ui(i1,i2,
     & i3,c3)+c(i,1,0,0)*ui(i1+1,i2,i3,c3)+c(i,0,1,0)*ui(i1,i2+1,i3,
     & c3)+c(i,1,1,0)*ui(i1+1,i2+1,i3,c3)+c(i,0,0,1)*ui(i1,i2,i3+1,c3)
     & +c(i,1,0,1)*ui(i1+1,i2,i3+1,c3)+c(i,0,1,1)*ui(i1,i2+1,i3+1,c3)+
     & c(i,1,1,1)*ui(i1+1,i2+1,i3+1,c3)


              end do
              end do
            end if
          else if( width(1).eq.4 .and. width(2).eq.4 .and. width(3)
     & .eq.4 )then
! loops3d($interp444(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)
            if( c3a.eq.c3b )then
              do c3=c3a,c3b
              do i=nia,nib
! interp444(ug(ip(i,1),ip(i,2),ip(i,3),c3))
                i1=il(i,1)
                i2=il(i,2)
                i3=il(i,3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,0,0)*ui(i1,i2,
     & i3,c3)+c(i,1,0,0)*ui(i1+1,i2,i3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3,
     & c3)+c(i,3,0,0)*ui(i1+3,i2,i3,c3)+c(i,0,1,0)*ui(i1,i2+1,i3,c3)+
     & c(i,1,1,0)*ui(i1+1,i2+1,i3,c3)+c(i,2,1,0)*ui(i1+2,i2+1,i3,c3)+
     & c(i,3,1,0)*ui(i1+3,i2+1,i3,c3)+c(i,0,2,0)*ui(i1,i2+2,i3,c3)+c(
     & i,1,2,0)*ui(i1+1,i2+2,i3,c3)+c(i,2,2,0)*ui(i1+2,i2+2,i3,c3)+c(
     & i,3,2,0)*ui(i1+3,i2+2,i3,c3)+c(i,0,3,0)*ui(i1,i2+3,i3,c3)+c(i,
     & 1,3,0)*ui(i1+1,i2+3,i3,c3)+c(i,2,3,0)*ui(i1+2,i2+3,i3,c3)+c(i,
     & 3,3,0)*ui(i1+3,i2+3,i3,c3)+c(i,0,0,1)*ui(i1,i2,i3+1,c3)+c(i,1,
     & 0,1)*ui(i1+1,i2,i3+1,c3)+c(i,2,0,1)*ui(i1+2,i2,i3+1,c3)+c(i,3,
     & 0,1)*ui(i1+3,i2,i3+1,c3)+c(i,0,1,1)*ui(i1,i2+1,i3+1,c3)+c(i,1,
     & 1,1)*ui(i1+1,i2+1,i3+1,c3)+c(i,2,1,1)*ui(i1+2,i2+1,i3+1,c3)+c(
     & i,3,1,1)*ui(i1+3,i2+1,i3+1,c3)+c(i,0,2,1)*ui(i1,i2+2,i3+1,c3)+
     & c(i,1,2,1)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,2,1)*ui(i1+2,i2+2,i3+1,
     & c3)+c(i,3,2,1)*ui(i1+3,i2+2,i3+1,c3)+c(i,0,3,1)*ui(i1,i2+3,i3+
     & 1,c3)+c(i,1,3,1)*ui(i1+1,i2+3,i3+1,c3)+c(i,2,3,1)*ui(i1+2,i2+3,
     & i3+1,c3)+c(i,3,3,1)*ui(i1+3,i2+3,i3+1,c3)+c(i,0,0,2)*ui(i1,i2,
     & i3+2,c3)+c(i,1,0,2)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,2)*ui(i1+2,i2,
     & i3+2,c3)+c(i,3,0,2)*ui(i1+3,i2,i3+2,c3)+c(i,0,1,2)*ui(i1,i2+1,
     & i3+2,c3)+c(i,1,1,2)*ui(i1+1,i2+1,i3+2,c3)+c(i,2,1,2)*ui(i1+2,
     & i2+1,i3+2,c3)+c(i,3,1,2)*ui(i1+3,i2+1,i3+2,c3)+c(i,0,2,2)*ui(
     & i1,i2+2,i3+2,c3)+c(i,1,2,2)*ui(i1+1,i2+2,i3+2,c3)+c(i,2,2,2)*
     & ui(i1+2,i2+2,i3+2,c3)+c(i,3,2,2)*ui(i1+3,i2+2,i3+2,c3)+c(i,0,3,
     & 2)*ui(i1,i2+3,i3+2,c3)+c(i,1,3,2)*ui(i1+1,i2+3,i3+2,c3)+c(i,2,
     & 3,2)*ui(i1+2,i2+3,i3+2,c3)+c(i,3,3,2)*ui(i1+3,i2+3,i3+2,c3)+c(
     & i,0,0,3)*ui(i1,i2,i3+3,c3)+c(i,1,0,3)*ui(i1+1,i2,i3+3,c3)+c(i,
     & 2,0,3)*ui(i1+2,i2,i3+3,c3)+c(i,3,0,3)*ui(i1+3,i2,i3+3,c3)+c(i,
     & 0,1,3)*ui(i1,i2+1,i3+3,c3)+c(i,1,1,3)*ui(i1+1,i2+1,i3+3,c3)+c(
     & i,2,1,3)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,1,3)*ui(i1+3,i2+1,i3+3,c3)
     & +c(i,0,2,3)*ui(i1,i2+2,i3+3,c3)+c(i,1,2,3)*ui(i1+1,i2+2,i3+3,
     & c3)+c(i,2,2,3)*ui(i1+2,i2+2,i3+3,c3)+c(i,3,2,3)*ui(i1+3,i2+2,
     & i3+3,c3)+c(i,0,3,3)*ui(i1,i2+3,i3+3,c3)+c(i,1,3,3)*ui(i1+1,i2+
     & 3,i3+3,c3)+c(i,2,3,3)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,3,3)*ui(i1+3,
     & i2+3,i3+3,c3)


              end do
              end do
            else
              ! put "c" loop as inner loop, this seems to be faster
              do i=nia,nib
              do c3=c3a,c3b
! interp444(ug(ip(i,1),ip(i,2),ip(i,3),c3))
                i1=il(i,1)
                i2=il(i,2)
                i3=il(i,3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,0,0)*ui(i1,i2,
     & i3,c3)+c(i,1,0,0)*ui(i1+1,i2,i3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3,
     & c3)+c(i,3,0,0)*ui(i1+3,i2,i3,c3)+c(i,0,1,0)*ui(i1,i2+1,i3,c3)+
     & c(i,1,1,0)*ui(i1+1,i2+1,i3,c3)+c(i,2,1,0)*ui(i1+2,i2+1,i3,c3)+
     & c(i,3,1,0)*ui(i1+3,i2+1,i3,c3)+c(i,0,2,0)*ui(i1,i2+2,i3,c3)+c(
     & i,1,2,0)*ui(i1+1,i2+2,i3,c3)+c(i,2,2,0)*ui(i1+2,i2+2,i3,c3)+c(
     & i,3,2,0)*ui(i1+3,i2+2,i3,c3)+c(i,0,3,0)*ui(i1,i2+3,i3,c3)+c(i,
     & 1,3,0)*ui(i1+1,i2+3,i3,c3)+c(i,2,3,0)*ui(i1+2,i2+3,i3,c3)+c(i,
     & 3,3,0)*ui(i1+3,i2+3,i3,c3)+c(i,0,0,1)*ui(i1,i2,i3+1,c3)+c(i,1,
     & 0,1)*ui(i1+1,i2,i3+1,c3)+c(i,2,0,1)*ui(i1+2,i2,i3+1,c3)+c(i,3,
     & 0,1)*ui(i1+3,i2,i3+1,c3)+c(i,0,1,1)*ui(i1,i2+1,i3+1,c3)+c(i,1,
     & 1,1)*ui(i1+1,i2+1,i3+1,c3)+c(i,2,1,1)*ui(i1+2,i2+1,i3+1,c3)+c(
     & i,3,1,1)*ui(i1+3,i2+1,i3+1,c3)+c(i,0,2,1)*ui(i1,i2+2,i3+1,c3)+
     & c(i,1,2,1)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,2,1)*ui(i1+2,i2+2,i3+1,
     & c3)+c(i,3,2,1)*ui(i1+3,i2+2,i3+1,c3)+c(i,0,3,1)*ui(i1,i2+3,i3+
     & 1,c3)+c(i,1,3,1)*ui(i1+1,i2+3,i3+1,c3)+c(i,2,3,1)*ui(i1+2,i2+3,
     & i3+1,c3)+c(i,3,3,1)*ui(i1+3,i2+3,i3+1,c3)+c(i,0,0,2)*ui(i1,i2,
     & i3+2,c3)+c(i,1,0,2)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,2)*ui(i1+2,i2,
     & i3+2,c3)+c(i,3,0,2)*ui(i1+3,i2,i3+2,c3)+c(i,0,1,2)*ui(i1,i2+1,
     & i3+2,c3)+c(i,1,1,2)*ui(i1+1,i2+1,i3+2,c3)+c(i,2,1,2)*ui(i1+2,
     & i2+1,i3+2,c3)+c(i,3,1,2)*ui(i1+3,i2+1,i3+2,c3)+c(i,0,2,2)*ui(
     & i1,i2+2,i3+2,c3)+c(i,1,2,2)*ui(i1+1,i2+2,i3+2,c3)+c(i,2,2,2)*
     & ui(i1+2,i2+2,i3+2,c3)+c(i,3,2,2)*ui(i1+3,i2+2,i3+2,c3)+c(i,0,3,
     & 2)*ui(i1,i2+3,i3+2,c3)+c(i,1,3,2)*ui(i1+1,i2+3,i3+2,c3)+c(i,2,
     & 3,2)*ui(i1+2,i2+3,i3+2,c3)+c(i,3,3,2)*ui(i1+3,i2+3,i3+2,c3)+c(
     & i,0,0,3)*ui(i1,i2,i3+3,c3)+c(i,1,0,3)*ui(i1+1,i2,i3+3,c3)+c(i,
     & 2,0,3)*ui(i1+2,i2,i3+3,c3)+c(i,3,0,3)*ui(i1+3,i2,i3+3,c3)+c(i,
     & 0,1,3)*ui(i1,i2+1,i3+3,c3)+c(i,1,1,3)*ui(i1+1,i2+1,i3+3,c3)+c(
     & i,2,1,3)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,1,3)*ui(i1+3,i2+1,i3+3,c3)
     & +c(i,0,2,3)*ui(i1,i2+2,i3+3,c3)+c(i,1,2,3)*ui(i1+1,i2+2,i3+3,
     & c3)+c(i,2,2,3)*ui(i1+2,i2+2,i3+3,c3)+c(i,3,2,3)*ui(i1+3,i2+2,
     & i3+3,c3)+c(i,0,3,3)*ui(i1,i2+3,i3+3,c3)+c(i,1,3,3)*ui(i1+1,i2+
     & 3,i3+3,c3)+c(i,2,3,3)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,3,3)*ui(i1+3,
     & i2+3,i3+3,c3)


              end do
              end do
            end if
          else if( width(1).eq.5 .and. width(2).eq.5 .and. width(3)
     & .eq.5 )then
            ! write(*,*) 'interpOpt explicit interp width=5'
! loops3d($interp555(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)
            if( c3a.eq.c3b )then
              do c3=c3a,c3b
              do i=nia,nib
! interp555(ug(ip(i,1),ip(i,2),ip(i,3),c3))
                i1=il(i,1)
                i2=il(i,2)
                i3=il(i,3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,0,0)*ui(i1,i2,
     & i3,c3)+c(i,1,0,0)*ui(i1+1,i2,i3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3,
     & c3)+c(i,3,0,0)*ui(i1+3,i2,i3,c3)+c(i,4,0,0)*ui(i1+4,i2,i3,c3)+
     & c(i,0,1,0)*ui(i1,i2+1,i3,c3)+c(i,1,1,0)*ui(i1+1,i2+1,i3,c3)+c(
     & i,2,1,0)*ui(i1+2,i2+1,i3,c3)+c(i,3,1,0)*ui(i1+3,i2+1,i3,c3)+c(
     & i,4,1,0)*ui(i1+4,i2+1,i3,c3)+c(i,0,2,0)*ui(i1,i2+2,i3,c3)+c(i,
     & 1,2,0)*ui(i1+1,i2+2,i3,c3)+c(i,2,2,0)*ui(i1+2,i2+2,i3,c3)+c(i,
     & 3,2,0)*ui(i1+3,i2+2,i3,c3)+c(i,4,2,0)*ui(i1+4,i2+2,i3,c3)+c(i,
     & 0,3,0)*ui(i1,i2+3,i3,c3)+c(i,1,3,0)*ui(i1+1,i2+3,i3,c3)+c(i,2,
     & 3,0)*ui(i1+2,i2+3,i3,c3)+c(i,3,3,0)*ui(i1+3,i2+3,i3,c3)+c(i,4,
     & 3,0)*ui(i1+4,i2+3,i3,c3)+c(i,0,4,0)*ui(i1,i2+4,i3,c3)+c(i,1,4,
     & 0)*ui(i1+1,i2+4,i3,c3)+c(i,2,4,0)*ui(i1+2,i2+4,i3,c3)+c(i,3,4,
     & 0)*ui(i1+3,i2+4,i3,c3)+c(i,4,4,0)*ui(i1+4,i2+4,i3,c3)+c(i,0,0,
     & 1)*ui(i1,i2,i3+1,c3)+c(i,1,0,1)*ui(i1+1,i2,i3+1,c3)+c(i,2,0,1)*
     & ui(i1+2,i2,i3+1,c3)+c(i,3,0,1)*ui(i1+3,i2,i3+1,c3)+c(i,4,0,1)*
     & ui(i1+4,i2,i3+1,c3)+c(i,0,1,1)*ui(i1,i2+1,i3+1,c3)+c(i,1,1,1)*
     & ui(i1+1,i2+1,i3+1,c3)+c(i,2,1,1)*ui(i1+2,i2+1,i3+1,c3)+c(i,3,1,
     & 1)*ui(i1+3,i2+1,i3+1,c3)+c(i,4,1,1)*ui(i1+4,i2+1,i3+1,c3)+c(i,
     & 0,2,1)*ui(i1,i2+2,i3+1,c3)+c(i,1,2,1)*ui(i1+1,i2+2,i3+1,c3)+c(
     & i,2,2,1)*ui(i1+2,i2+2,i3+1,c3)+c(i,3,2,1)*ui(i1+3,i2+2,i3+1,c3)
     & +c(i,4,2,1)*ui(i1+4,i2+2,i3+1,c3)+c(i,0,3,1)*ui(i1,i2+3,i3+1,
     & c3)+c(i,1,3,1)*ui(i1+1,i2+3,i3+1,c3)+c(i,2,3,1)*ui(i1+2,i2+3,
     & i3+1,c3)+c(i,3,3,1)*ui(i1+3,i2+3,i3+1,c3)+c(i,4,3,1)*ui(i1+4,
     & i2+3,i3+1,c3)+c(i,0,4,1)*ui(i1,i2+4,i3+1,c3)+c(i,1,4,1)*ui(i1+
     & 1,i2+4,i3+1,c3)+c(i,2,4,1)*ui(i1+2,i2+4,i3+1,c3)+c(i,3,4,1)*ui(
     & i1+3,i2+4,i3+1,c3)+c(i,4,4,1)*ui(i1+4,i2+4,i3+1,c3)+c(i,0,0,2)*
     & ui(i1,i2,i3+2,c3)+c(i,1,0,2)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,2)*ui(
     & i1+2,i2,i3+2,c3)+c(i,3,0,2)*ui(i1+3,i2,i3+2,c3)+c(i,4,0,2)*ui(
     & i1+4,i2,i3+2,c3)+c(i,0,1,2)*ui(i1,i2+1,i3+2,c3)+c(i,1,1,2)*ui(
     & i1+1,i2+1,i3+2,c3)+c(i,2,1,2)*ui(i1+2,i2+1,i3+2,c3)+c(i,3,1,2)*
     & ui(i1+3,i2+1,i3+2,c3)+c(i,4,1,2)*ui(i1+4,i2+1,i3+2,c3)+c(i,0,2,
     & 2)*ui(i1,i2+2,i3+2,c3)+c(i,1,2,2)*ui(i1+1,i2+2,i3+2,c3)+c(i,2,
     & 2,2)*ui(i1+2,i2+2,i3+2,c3)+c(i,3,2,2)*ui(i1+3,i2+2,i3+2,c3)+c(
     & i,4,2,2)*ui(i1+4,i2+2,i3+2,c3)+c(i,0,3,2)*ui(i1,i2+3,i3+2,c3)+
     & c(i,1,3,2)*ui(i1+1,i2+3,i3+2,c3)+c(i,2,3,2)*ui(i1+2,i2+3,i3+2,
     & c3)+c(i,3,3,2)*ui(i1+3,i2+3,i3+2,c3)+c(i,4,3,2)*ui(i1+4,i2+3,
     & i3+2,c3)+c(i,0,4,2)*ui(i1,i2+4,i3+2,c3)+c(i,1,4,2)*ui(i1+1,i2+
     & 4,i3+2,c3)+c(i,2,4,2)*ui(i1+2,i2+4,i3+2,c3)+c(i,3,4,2)*ui(i1+3,
     & i2+4,i3+2,c3)+c(i,4,4,2)*ui(i1+4,i2+4,i3+2,c3)+c(i,0,0,3)*ui(
     & i1,i2,i3+3,c3)+c(i,1,0,3)*ui(i1+1,i2,i3+3,c3)+c(i,2,0,3)*ui(i1+
     & 2,i2,i3+3,c3)+c(i,3,0,3)*ui(i1+3,i2,i3+3,c3)+c(i,4,0,3)*ui(i1+
     & 4,i2,i3+3,c3)+c(i,0,1,3)*ui(i1,i2+1,i3+3,c3)+c(i,1,1,3)*ui(i1+
     & 1,i2+1,i3+3,c3)+c(i,2,1,3)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,1,3)*ui(
     & i1+3,i2+1,i3+3,c3)+c(i,4,1,3)*ui(i1+4,i2+1,i3+3,c3)+c(i,0,2,3)*
     & ui(i1,i2+2,i3+3,c3)+c(i,1,2,3)*ui(i1+1,i2+2,i3+3,c3)+c(i,2,2,3)
     & *ui(i1+2,i2+2,i3+3,c3)+c(i,3,2,3)*ui(i1+3,i2+2,i3+3,c3)+c(i,4,
     & 2,3)*ui(i1+4,i2+2,i3+3,c3)+c(i,0,3,3)*ui(i1,i2+3,i3+3,c3)+c(i,
     & 1,3,3)*ui(i1+1,i2+3,i3+3,c3)+c(i,2,3,3)*ui(i1+2,i2+3,i3+3,c3)+
     & c(i,3,3,3)*ui(i1+3,i2+3,i3+3,c3)+c(i,4,3,3)*ui(i1+4,i2+3,i3+3,
     & c3)+c(i,0,4,3)*ui(i1,i2+4,i3+3,c3)+c(i,1,4,3)*ui(i1+1,i2+4,i3+
     & 3,c3)+c(i,2,4,3)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,4,3)*ui(i1+3,i2+4,
     & i3+3,c3)+c(i,4,4,3)*ui(i1+4,i2+4,i3+3,c3)+c(i,0,0,4)*ui(i1,i2,
     & i3+4,c3)+c(i,1,0,4)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,4)*ui(i1+2,i2,
     & i3+4,c3)+c(i,3,0,4)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,4)*ui(i1+4,i2,
     & i3+4,c3)+c(i,0,1,4)*ui(i1,i2+1,i3+4,c3)+c(i,1,1,4)*ui(i1+1,i2+
     & 1,i3+4,c3)+c(i,2,1,4)*ui(i1+2,i2+1,i3+4,c3)+c(i,3,1,4)*ui(i1+3,
     & i2+1,i3+4,c3)+c(i,4,1,4)*ui(i1+4,i2+1,i3+4,c3)+c(i,0,2,4)*ui(
     & i1,i2+2,i3+4,c3)+c(i,1,2,4)*ui(i1+1,i2+2,i3+4,c3)+c(i,2,2,4)*
     & ui(i1+2,i2+2,i3+4,c3)+c(i,3,2,4)*ui(i1+3,i2+2,i3+4,c3)+c(i,4,2,
     & 4)*ui(i1+4,i2+2,i3+4,c3)+c(i,0,3,4)*ui(i1,i2+3,i3+4,c3)+c(i,1,
     & 3,4)*ui(i1+1,i2+3,i3+4,c3)+c(i,2,3,4)*ui(i1+2,i2+3,i3+4,c3)+c(
     & i,3,3,4)*ui(i1+3,i2+3,i3+4,c3)+c(i,4,3,4)*ui(i1+4,i2+3,i3+4,c3)
     & +c(i,0,4,4)*ui(i1,i2+4,i3+4,c3)+c(i,1,4,4)*ui(i1+1,i2+4,i3+4,
     & c3)+c(i,2,4,4)*ui(i1+2,i2+4,i3+4,c3)+c(i,3,4,4)*ui(i1+3,i2+4,
     & i3+4,c3)+c(i,4,4,4)*ui(i1+4,i2+4,i3+4,c3)


              end do
              end do
            else
              ! put "c" loop as inner loop, this seems to be faster
              do i=nia,nib
              do c3=c3a,c3b
! interp555(ug(ip(i,1),ip(i,2),ip(i,3),c3))
                i1=il(i,1)
                i2=il(i,2)
                i3=il(i,3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,0,0)*ui(i1,i2,
     & i3,c3)+c(i,1,0,0)*ui(i1+1,i2,i3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3,
     & c3)+c(i,3,0,0)*ui(i1+3,i2,i3,c3)+c(i,4,0,0)*ui(i1+4,i2,i3,c3)+
     & c(i,0,1,0)*ui(i1,i2+1,i3,c3)+c(i,1,1,0)*ui(i1+1,i2+1,i3,c3)+c(
     & i,2,1,0)*ui(i1+2,i2+1,i3,c3)+c(i,3,1,0)*ui(i1+3,i2+1,i3,c3)+c(
     & i,4,1,0)*ui(i1+4,i2+1,i3,c3)+c(i,0,2,0)*ui(i1,i2+2,i3,c3)+c(i,
     & 1,2,0)*ui(i1+1,i2+2,i3,c3)+c(i,2,2,0)*ui(i1+2,i2+2,i3,c3)+c(i,
     & 3,2,0)*ui(i1+3,i2+2,i3,c3)+c(i,4,2,0)*ui(i1+4,i2+2,i3,c3)+c(i,
     & 0,3,0)*ui(i1,i2+3,i3,c3)+c(i,1,3,0)*ui(i1+1,i2+3,i3,c3)+c(i,2,
     & 3,0)*ui(i1+2,i2+3,i3,c3)+c(i,3,3,0)*ui(i1+3,i2+3,i3,c3)+c(i,4,
     & 3,0)*ui(i1+4,i2+3,i3,c3)+c(i,0,4,0)*ui(i1,i2+4,i3,c3)+c(i,1,4,
     & 0)*ui(i1+1,i2+4,i3,c3)+c(i,2,4,0)*ui(i1+2,i2+4,i3,c3)+c(i,3,4,
     & 0)*ui(i1+3,i2+4,i3,c3)+c(i,4,4,0)*ui(i1+4,i2+4,i3,c3)+c(i,0,0,
     & 1)*ui(i1,i2,i3+1,c3)+c(i,1,0,1)*ui(i1+1,i2,i3+1,c3)+c(i,2,0,1)*
     & ui(i1+2,i2,i3+1,c3)+c(i,3,0,1)*ui(i1+3,i2,i3+1,c3)+c(i,4,0,1)*
     & ui(i1+4,i2,i3+1,c3)+c(i,0,1,1)*ui(i1,i2+1,i3+1,c3)+c(i,1,1,1)*
     & ui(i1+1,i2+1,i3+1,c3)+c(i,2,1,1)*ui(i1+2,i2+1,i3+1,c3)+c(i,3,1,
     & 1)*ui(i1+3,i2+1,i3+1,c3)+c(i,4,1,1)*ui(i1+4,i2+1,i3+1,c3)+c(i,
     & 0,2,1)*ui(i1,i2+2,i3+1,c3)+c(i,1,2,1)*ui(i1+1,i2+2,i3+1,c3)+c(
     & i,2,2,1)*ui(i1+2,i2+2,i3+1,c3)+c(i,3,2,1)*ui(i1+3,i2+2,i3+1,c3)
     & +c(i,4,2,1)*ui(i1+4,i2+2,i3+1,c3)+c(i,0,3,1)*ui(i1,i2+3,i3+1,
     & c3)+c(i,1,3,1)*ui(i1+1,i2+3,i3+1,c3)+c(i,2,3,1)*ui(i1+2,i2+3,
     & i3+1,c3)+c(i,3,3,1)*ui(i1+3,i2+3,i3+1,c3)+c(i,4,3,1)*ui(i1+4,
     & i2+3,i3+1,c3)+c(i,0,4,1)*ui(i1,i2+4,i3+1,c3)+c(i,1,4,1)*ui(i1+
     & 1,i2+4,i3+1,c3)+c(i,2,4,1)*ui(i1+2,i2+4,i3+1,c3)+c(i,3,4,1)*ui(
     & i1+3,i2+4,i3+1,c3)+c(i,4,4,1)*ui(i1+4,i2+4,i3+1,c3)+c(i,0,0,2)*
     & ui(i1,i2,i3+2,c3)+c(i,1,0,2)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,2)*ui(
     & i1+2,i2,i3+2,c3)+c(i,3,0,2)*ui(i1+3,i2,i3+2,c3)+c(i,4,0,2)*ui(
     & i1+4,i2,i3+2,c3)+c(i,0,1,2)*ui(i1,i2+1,i3+2,c3)+c(i,1,1,2)*ui(
     & i1+1,i2+1,i3+2,c3)+c(i,2,1,2)*ui(i1+2,i2+1,i3+2,c3)+c(i,3,1,2)*
     & ui(i1+3,i2+1,i3+2,c3)+c(i,4,1,2)*ui(i1+4,i2+1,i3+2,c3)+c(i,0,2,
     & 2)*ui(i1,i2+2,i3+2,c3)+c(i,1,2,2)*ui(i1+1,i2+2,i3+2,c3)+c(i,2,
     & 2,2)*ui(i1+2,i2+2,i3+2,c3)+c(i,3,2,2)*ui(i1+3,i2+2,i3+2,c3)+c(
     & i,4,2,2)*ui(i1+4,i2+2,i3+2,c3)+c(i,0,3,2)*ui(i1,i2+3,i3+2,c3)+
     & c(i,1,3,2)*ui(i1+1,i2+3,i3+2,c3)+c(i,2,3,2)*ui(i1+2,i2+3,i3+2,
     & c3)+c(i,3,3,2)*ui(i1+3,i2+3,i3+2,c3)+c(i,4,3,2)*ui(i1+4,i2+3,
     & i3+2,c3)+c(i,0,4,2)*ui(i1,i2+4,i3+2,c3)+c(i,1,4,2)*ui(i1+1,i2+
     & 4,i3+2,c3)+c(i,2,4,2)*ui(i1+2,i2+4,i3+2,c3)+c(i,3,4,2)*ui(i1+3,
     & i2+4,i3+2,c3)+c(i,4,4,2)*ui(i1+4,i2+4,i3+2,c3)+c(i,0,0,3)*ui(
     & i1,i2,i3+3,c3)+c(i,1,0,3)*ui(i1+1,i2,i3+3,c3)+c(i,2,0,3)*ui(i1+
     & 2,i2,i3+3,c3)+c(i,3,0,3)*ui(i1+3,i2,i3+3,c3)+c(i,4,0,3)*ui(i1+
     & 4,i2,i3+3,c3)+c(i,0,1,3)*ui(i1,i2+1,i3+3,c3)+c(i,1,1,3)*ui(i1+
     & 1,i2+1,i3+3,c3)+c(i,2,1,3)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,1,3)*ui(
     & i1+3,i2+1,i3+3,c3)+c(i,4,1,3)*ui(i1+4,i2+1,i3+3,c3)+c(i,0,2,3)*
     & ui(i1,i2+2,i3+3,c3)+c(i,1,2,3)*ui(i1+1,i2+2,i3+3,c3)+c(i,2,2,3)
     & *ui(i1+2,i2+2,i3+3,c3)+c(i,3,2,3)*ui(i1+3,i2+2,i3+3,c3)+c(i,4,
     & 2,3)*ui(i1+4,i2+2,i3+3,c3)+c(i,0,3,3)*ui(i1,i2+3,i3+3,c3)+c(i,
     & 1,3,3)*ui(i1+1,i2+3,i3+3,c3)+c(i,2,3,3)*ui(i1+2,i2+3,i3+3,c3)+
     & c(i,3,3,3)*ui(i1+3,i2+3,i3+3,c3)+c(i,4,3,3)*ui(i1+4,i2+3,i3+3,
     & c3)+c(i,0,4,3)*ui(i1,i2+4,i3+3,c3)+c(i,1,4,3)*ui(i1+1,i2+4,i3+
     & 3,c3)+c(i,2,4,3)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,4,3)*ui(i1+3,i2+4,
     & i3+3,c3)+c(i,4,4,3)*ui(i1+4,i2+4,i3+3,c3)+c(i,0,0,4)*ui(i1,i2,
     & i3+4,c3)+c(i,1,0,4)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,4)*ui(i1+2,i2,
     & i3+4,c3)+c(i,3,0,4)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,4)*ui(i1+4,i2,
     & i3+4,c3)+c(i,0,1,4)*ui(i1,i2+1,i3+4,c3)+c(i,1,1,4)*ui(i1+1,i2+
     & 1,i3+4,c3)+c(i,2,1,4)*ui(i1+2,i2+1,i3+4,c3)+c(i,3,1,4)*ui(i1+3,
     & i2+1,i3+4,c3)+c(i,4,1,4)*ui(i1+4,i2+1,i3+4,c3)+c(i,0,2,4)*ui(
     & i1,i2+2,i3+4,c3)+c(i,1,2,4)*ui(i1+1,i2+2,i3+4,c3)+c(i,2,2,4)*
     & ui(i1+2,i2+2,i3+4,c3)+c(i,3,2,4)*ui(i1+3,i2+2,i3+4,c3)+c(i,4,2,
     & 4)*ui(i1+4,i2+2,i3+4,c3)+c(i,0,3,4)*ui(i1,i2+3,i3+4,c3)+c(i,1,
     & 3,4)*ui(i1+1,i2+3,i3+4,c3)+c(i,2,3,4)*ui(i1+2,i2+3,i3+4,c3)+c(
     & i,3,3,4)*ui(i1+3,i2+3,i3+4,c3)+c(i,4,3,4)*ui(i1+4,i2+3,i3+4,c3)
     & +c(i,0,4,4)*ui(i1,i2+4,i3+4,c3)+c(i,1,4,4)*ui(i1+1,i2+4,i3+4,
     & c3)+c(i,2,4,4)*ui(i1+2,i2+4,i3+4,c3)+c(i,3,4,4)*ui(i1+3,i2+4,
     & i3+4,c3)+c(i,4,4,4)*ui(i1+4,i2+4,i3+4,c3)


              end do
              end do
            end if
          else if( width(1).eq.6 .and. width(2).eq.6 .and. width(3)
     & .eq.6 )then
! loops3d($interp666(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)
            if( c3a.eq.c3b )then
              do c3=c3a,c3b
              do i=nia,nib
! interp666(ug(ip(i,1),ip(i,2),ip(i,3),c3))
                i1=il(i,1)
                i2=il(i,2)
                i3=il(i,3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,0,0)*ui(i1,i2,
     & i3,c3)+c(i,1,0,0)*ui(i1+1,i2,i3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3,
     & c3)+c(i,3,0,0)*ui(i1+3,i2,i3,c3)+c(i,4,0,0)*ui(i1+4,i2,i3,c3)+
     & c(i,5,0,0)*ui(i1+5,i2,i3,c3)+c(i,0,1,0)*ui(i1,i2+1,i3,c3)+c(i,
     & 1,1,0)*ui(i1+1,i2+1,i3,c3)+c(i,2,1,0)*ui(i1+2,i2+1,i3,c3)+c(i,
     & 3,1,0)*ui(i1+3,i2+1,i3,c3)+c(i,4,1,0)*ui(i1+4,i2+1,i3,c3)+c(i,
     & 5,1,0)*ui(i1+5,i2+1,i3,c3)+c(i,0,2,0)*ui(i1,i2+2,i3,c3)+c(i,1,
     & 2,0)*ui(i1+1,i2+2,i3,c3)+c(i,2,2,0)*ui(i1+2,i2+2,i3,c3)+c(i,3,
     & 2,0)*ui(i1+3,i2+2,i3,c3)+c(i,4,2,0)*ui(i1+4,i2+2,i3,c3)+c(i,5,
     & 2,0)*ui(i1+5,i2+2,i3,c3)+c(i,0,3,0)*ui(i1,i2+3,i3,c3)+c(i,1,3,
     & 0)*ui(i1+1,i2+3,i3,c3)+c(i,2,3,0)*ui(i1+2,i2+3,i3,c3)+c(i,3,3,
     & 0)*ui(i1+3,i2+3,i3,c3)+c(i,4,3,0)*ui(i1+4,i2+3,i3,c3)+c(i,5,3,
     & 0)*ui(i1+5,i2+3,i3,c3)+c(i,0,4,0)*ui(i1,i2+4,i3,c3)+c(i,1,4,0)*
     & ui(i1+1,i2+4,i3,c3)+c(i,2,4,0)*ui(i1+2,i2+4,i3,c3)+c(i,3,4,0)*
     & ui(i1+3,i2+4,i3,c3)+c(i,4,4,0)*ui(i1+4,i2+4,i3,c3)+c(i,5,4,0)*
     & ui(i1+5,i2+4,i3,c3)+c(i,0,5,0)*ui(i1,i2+5,i3,c3)+c(i,1,5,0)*ui(
     & i1+1,i2+5,i3,c3)+c(i,2,5,0)*ui(i1+2,i2+5,i3,c3)+c(i,3,5,0)*ui(
     & i1+3,i2+5,i3,c3)+c(i,4,5,0)*ui(i1+4,i2+5,i3,c3)+c(i,5,5,0)*ui(
     & i1+5,i2+5,i3,c3)+c(i,0,0,1)*ui(i1,i2,i3+1,c3)+c(i,1,0,1)*ui(i1+
     & 1,i2,i3+1,c3)+c(i,2,0,1)*ui(i1+2,i2,i3+1,c3)+c(i,3,0,1)*ui(i1+
     & 3,i2,i3+1,c3)+c(i,4,0,1)*ui(i1+4,i2,i3+1,c3)+c(i,5,0,1)*ui(i1+
     & 5,i2,i3+1,c3)+c(i,0,1,1)*ui(i1,i2+1,i3+1,c3)+c(i,1,1,1)*ui(i1+
     & 1,i2+1,i3+1,c3)+c(i,2,1,1)*ui(i1+2,i2+1,i3+1,c3)+c(i,3,1,1)*ui(
     & i1+3,i2+1,i3+1,c3)+c(i,4,1,1)*ui(i1+4,i2+1,i3+1,c3)+c(i,5,1,1)*
     & ui(i1+5,i2+1,i3+1,c3)+c(i,0,2,1)*ui(i1,i2+2,i3+1,c3)+c(i,1,2,1)
     & *ui(i1+1,i2+2,i3+1,c3)+c(i,2,2,1)*ui(i1+2,i2+2,i3+1,c3)+c(i,3,
     & 2,1)*ui(i1+3,i2+2,i3+1,c3)+c(i,4,2,1)*ui(i1+4,i2+2,i3+1,c3)+c(
     & i,5,2,1)*ui(i1+5,i2+2,i3+1,c3)+c(i,0,3,1)*ui(i1,i2+3,i3+1,c3)+
     & c(i,1,3,1)*ui(i1+1,i2+3,i3+1,c3)+c(i,2,3,1)*ui(i1+2,i2+3,i3+1,
     & c3)+c(i,3,3,1)*ui(i1+3,i2+3,i3+1,c3)+c(i,4,3,1)*ui(i1+4,i2+3,
     & i3+1,c3)+c(i,5,3,1)*ui(i1+5,i2+3,i3+1,c3)+c(i,0,4,1)*ui(i1,i2+
     & 4,i3+1,c3)+c(i,1,4,1)*ui(i1+1,i2+4,i3+1,c3)+c(i,2,4,1)*ui(i1+2,
     & i2+4,i3+1,c3)+c(i,3,4,1)*ui(i1+3,i2+4,i3+1,c3)+c(i,4,4,1)*ui(
     & i1+4,i2+4,i3+1,c3)+c(i,5,4,1)*ui(i1+5,i2+4,i3+1,c3)+c(i,0,5,1)*
     & ui(i1,i2+5,i3+1,c3)+c(i,1,5,1)*ui(i1+1,i2+5,i3+1,c3)+c(i,2,5,1)
     & *ui(i1+2,i2+5,i3+1,c3)+c(i,3,5,1)*ui(i1+3,i2+5,i3+1,c3)+c(i,4,
     & 5,1)*ui(i1+4,i2+5,i3+1,c3)+c(i,5,5,1)*ui(i1+5,i2+5,i3+1,c3)+c(
     & i,0,0,2)*ui(i1,i2,i3+2,c3)+c(i,1,0,2)*ui(i1+1,i2,i3+2,c3)+c(i,
     & 2,0,2)*ui(i1+2,i2,i3+2,c3)+c(i,3,0,2)*ui(i1+3,i2,i3+2,c3)+c(i,
     & 4,0,2)*ui(i1+4,i2,i3+2,c3)+c(i,5,0,2)*ui(i1+5,i2,i3+2,c3)+c(i,
     & 0,1,2)*ui(i1,i2+1,i3+2,c3)+c(i,1,1,2)*ui(i1+1,i2+1,i3+2,c3)+c(
     & i,2,1,2)*ui(i1+2,i2+1,i3+2,c3)+c(i,3,1,2)*ui(i1+3,i2+1,i3+2,c3)
     & +c(i,4,1,2)*ui(i1+4,i2+1,i3+2,c3)+c(i,5,1,2)*ui(i1+5,i2+1,i3+2,
     & c3)+c(i,0,2,2)*ui(i1,i2+2,i3+2,c3)+c(i,1,2,2)*ui(i1+1,i2+2,i3+
     & 2,c3)+c(i,2,2,2)*ui(i1+2,i2+2,i3+2,c3)+c(i,3,2,2)*ui(i1+3,i2+2,
     & i3+2,c3)+c(i,4,2,2)*ui(i1+4,i2+2,i3+2,c3)+c(i,5,2,2)*ui(i1+5,
     & i2+2,i3+2,c3)+c(i,0,3,2)*ui(i1,i2+3,i3+2,c3)+c(i,1,3,2)*ui(i1+
     & 1,i2+3,i3+2,c3)+c(i,2,3,2)*ui(i1+2,i2+3,i3+2,c3)+c(i,3,3,2)*ui(
     & i1+3,i2+3,i3+2,c3)+c(i,4,3,2)*ui(i1+4,i2+3,i3+2,c3)+c(i,5,3,2)*
     & ui(i1+5,i2+3,i3+2,c3)+c(i,0,4,2)*ui(i1,i2+4,i3+2,c3)+c(i,1,4,2)
     & *ui(i1+1,i2+4,i3+2,c3)+c(i,2,4,2)*ui(i1+2,i2+4,i3+2,c3)+c(i,3,
     & 4,2)*ui(i1+3,i2+4,i3+2,c3)+c(i,4,4,2)*ui(i1+4,i2+4,i3+2,c3)+c(
     & i,5,4,2)*ui(i1+5,i2+4,i3+2,c3)+c(i,0,5,2)*ui(i1,i2+5,i3+2,c3)+
     & c(i,1,5,2)*ui(i1+1,i2+5,i3+2,c3)+c(i,2,5,2)*ui(i1+2,i2+5,i3+2,
     & c3)+c(i,3,5,2)*ui(i1+3,i2+5,i3+2,c3)+c(i,4,5,2)*ui(i1+4,i2+5,
     & i3+2,c3)+c(i,5,5,2)*ui(i1+5,i2+5,i3+2,c3)+c(i,0,0,3)*ui(i1,i2,
     & i3+3,c3)+c(i,1,0,3)*ui(i1+1,i2,i3+3,c3)+c(i,2,0,3)*ui(i1+2,i2,
     & i3+3,c3)+c(i,3,0,3)*ui(i1+3,i2,i3+3,c3)+c(i,4,0,3)*ui(i1+4,i2,
     & i3+3,c3)+c(i,5,0,3)*ui(i1+5,i2,i3+3,c3)+c(i,0,1,3)*ui(i1,i2+1,
     & i3+3,c3)+c(i,1,1,3)*ui(i1+1,i2+1,i3+3,c3)+c(i,2,1,3)*ui(i1+2,
     & i2+1,i3+3,c3)+c(i,3,1,3)*ui(i1+3,i2+1,i3+3,c3)+c(i,4,1,3)*ui(
     & i1+4,i2+1,i3+3,c3)+c(i,5,1,3)*ui(i1+5,i2+1,i3+3,c3)+c(i,0,2,3)*
     & ui(i1,i2+2,i3+3,c3)+c(i,1,2,3)*ui(i1+1,i2+2,i3+3,c3)+c(i,2,2,3)
     & *ui(i1+2,i2+2,i3+3,c3)+c(i,3,2,3)*ui(i1+3,i2+2,i3+3,c3)+c(i,4,
     & 2,3)*ui(i1+4,i2+2,i3+3,c3)+c(i,5,2,3)*ui(i1+5,i2+2,i3+3,c3)+c(
     & i,0,3,3)*ui(i1,i2+3,i3+3,c3)+c(i,1,3,3)*ui(i1+1,i2+3,i3+3,c3)+
     & c(i,2,3,3)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,3,3)*ui(i1+3,i2+3,i3+3,
     & c3)+c(i,4,3,3)*ui(i1+4,i2+3,i3+3,c3)+c(i,5,3,3)*ui(i1+5,i2+3,
     & i3+3,c3)+c(i,0,4,3)*ui(i1,i2+4,i3+3,c3)+c(i,1,4,3)*ui(i1+1,i2+
     & 4,i3+3,c3)+c(i,2,4,3)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,4,3)*ui(i1+3,
     & i2+4,i3+3,c3)+c(i,4,4,3)*ui(i1+4,i2+4,i3+3,c3)+c(i,5,4,3)*ui(
     & i1+5,i2+4,i3+3,c3)+c(i,0,5,3)*ui(i1,i2+5,i3+3,c3)+c(i,1,5,3)*
     & ui(i1+1,i2+5,i3+3,c3)+c(i,2,5,3)*ui(i1+2,i2+5,i3+3,c3)+c(i,3,5,
     & 3)*ui(i1+3,i2+5,i3+3,c3)+c(i,4,5,3)*ui(i1+4,i2+5,i3+3,c3)+c(i,
     & 5,5,3)*ui(i1+5,i2+5,i3+3,c3)+c(i,0,0,4)*ui(i1,i2,i3+4,c3)+c(i,
     & 1,0,4)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,4)*ui(i1+2,i2,i3+4,c3)+c(i,
     & 3,0,4)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,4)*ui(i1+4,i2,i3+4,c3)+c(i,
     & 5,0,4)*ui(i1+5,i2,i3+4,c3)+c(i,0,1,4)*ui(i1,i2+1,i3+4,c3)+c(i,
     & 1,1,4)*ui(i1+1,i2+1,i3+4,c3)+c(i,2,1,4)*ui(i1+2,i2+1,i3+4,c3)+
     & c(i,3,1,4)*ui(i1+3,i2+1,i3+4,c3)+c(i,4,1,4)*ui(i1+4,i2+1,i3+4,
     & c3)+c(i,5,1,4)*ui(i1+5,i2+1,i3+4,c3)+c(i,0,2,4)*ui(i1,i2+2,i3+
     & 4,c3)+c(i,1,2,4)*ui(i1+1,i2+2,i3+4,c3)+c(i,2,2,4)*ui(i1+2,i2+2,
     & i3+4,c3)+c(i,3,2,4)*ui(i1+3,i2+2,i3+4,c3)+c(i,4,2,4)*ui(i1+4,
     & i2+2,i3+4,c3)+c(i,5,2,4)*ui(i1+5,i2+2,i3+4,c3)+c(i,0,3,4)*ui(
     & i1,i2+3,i3+4,c3)+c(i,1,3,4)*ui(i1+1,i2+3,i3+4,c3)+c(i,2,3,4)*
     & ui(i1+2,i2+3,i3+4,c3)+c(i,3,3,4)*ui(i1+3,i2+3,i3+4,c3)+c(i,4,3,
     & 4)*ui(i1+4,i2+3,i3+4,c3)+c(i,5,3,4)*ui(i1+5,i2+3,i3+4,c3)+c(i,
     & 0,4,4)*ui(i1,i2+4,i3+4,c3)+c(i,1,4,4)*ui(i1+1,i2+4,i3+4,c3)+c(
     & i,2,4,4)*ui(i1+2,i2+4,i3+4,c3)+c(i,3,4,4)*ui(i1+3,i2+4,i3+4,c3)
     & +c(i,4,4,4)*ui(i1+4,i2+4,i3+4,c3)+c(i,5,4,4)*ui(i1+5,i2+4,i3+4,
     & c3)+c(i,0,5,4)*ui(i1,i2+5,i3+4,c3)+c(i,1,5,4)*ui(i1+1,i2+5,i3+
     & 4,c3)+c(i,2,5,4)*ui(i1+2,i2+5,i3+4,c3)+c(i,3,5,4)*ui(i1+3,i2+5,
     & i3+4,c3)+c(i,4,5,4)*ui(i1+4,i2+5,i3+4,c3)+c(i,5,5,4)*ui(i1+5,
     & i2+5,i3+4,c3)+c(i,0,0,5)*ui(i1,i2,i3+5,c3)+c(i,1,0,5)*ui(i1+1,
     & i2,i3+5,c3)+c(i,2,0,5)*ui(i1+2,i2,i3+5,c3)+c(i,3,0,5)*ui(i1+3,
     & i2,i3+5,c3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,4,0,5)*ui(i1+4,i2,i3+5,c3)+c(i,5,0,5)*ui(i1+5,i2,
     & i3+5,c3)+c(i,0,1,5)*ui(i1,i2+1,i3+5,c3)+c(i,1,1,5)*ui(i1+1,i2+
     & 1,i3+5,c3)+c(i,2,1,5)*ui(i1+2,i2+1,i3+5,c3)+c(i,3,1,5)*ui(i1+3,
     & i2+1,i3+5,c3)+c(i,4,1,5)*ui(i1+4,i2+1,i3+5,c3)+c(i,5,1,5)*ui(
     & i1+5,i2+1,i3+5,c3)+c(i,0,2,5)*ui(i1,i2+2,i3+5,c3)+c(i,1,2,5)*
     & ui(i1+1,i2+2,i3+5,c3)+c(i,2,2,5)*ui(i1+2,i2+2,i3+5,c3)+c(i,3,2,
     & 5)*ui(i1+3,i2+2,i3+5,c3)+c(i,4,2,5)*ui(i1+4,i2+2,i3+5,c3)+c(i,
     & 5,2,5)*ui(i1+5,i2+2,i3+5,c3)+c(i,0,3,5)*ui(i1,i2+3,i3+5,c3)+c(
     & i,1,3,5)*ui(i1+1,i2+3,i3+5,c3)+c(i,2,3,5)*ui(i1+2,i2+3,i3+5,c3)
     & +c(i,3,3,5)*ui(i1+3,i2+3,i3+5,c3)+c(i,4,3,5)*ui(i1+4,i2+3,i3+5,
     & c3)+c(i,5,3,5)*ui(i1+5,i2+3,i3+5,c3)+c(i,0,4,5)*ui(i1,i2+4,i3+
     & 5,c3)+c(i,1,4,5)*ui(i1+1,i2+4,i3+5,c3)+c(i,2,4,5)*ui(i1+2,i2+4,
     & i3+5,c3)+c(i,3,4,5)*ui(i1+3,i2+4,i3+5,c3)+c(i,4,4,5)*ui(i1+4,
     & i2+4,i3+5,c3)+c(i,5,4,5)*ui(i1+5,i2+4,i3+5,c3)+c(i,0,5,5)*ui(
     & i1,i2+5,i3+5,c3)+c(i,1,5,5)*ui(i1+1,i2+5,i3+5,c3)+c(i,2,5,5)*
     & ui(i1+2,i2+5,i3+5,c3)+c(i,3,5,5)*ui(i1+3,i2+5,i3+5,c3)+c(i,4,5,
     & 5)*ui(i1+4,i2+5,i3+5,c3)+c(i,5,5,5)*ui(i1+5,i2+5,i3+5,c3)


              end do
              end do
            else
              ! put "c" loop as inner loop, this seems to be faster
              do i=nia,nib
              do c3=c3a,c3b
! interp666(ug(ip(i,1),ip(i,2),ip(i,3),c3))
                i1=il(i,1)
                i2=il(i,2)
                i3=il(i,3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,0,0)*ui(i1,i2,
     & i3,c3)+c(i,1,0,0)*ui(i1+1,i2,i3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3,
     & c3)+c(i,3,0,0)*ui(i1+3,i2,i3,c3)+c(i,4,0,0)*ui(i1+4,i2,i3,c3)+
     & c(i,5,0,0)*ui(i1+5,i2,i3,c3)+c(i,0,1,0)*ui(i1,i2+1,i3,c3)+c(i,
     & 1,1,0)*ui(i1+1,i2+1,i3,c3)+c(i,2,1,0)*ui(i1+2,i2+1,i3,c3)+c(i,
     & 3,1,0)*ui(i1+3,i2+1,i3,c3)+c(i,4,1,0)*ui(i1+4,i2+1,i3,c3)+c(i,
     & 5,1,0)*ui(i1+5,i2+1,i3,c3)+c(i,0,2,0)*ui(i1,i2+2,i3,c3)+c(i,1,
     & 2,0)*ui(i1+1,i2+2,i3,c3)+c(i,2,2,0)*ui(i1+2,i2+2,i3,c3)+c(i,3,
     & 2,0)*ui(i1+3,i2+2,i3,c3)+c(i,4,2,0)*ui(i1+4,i2+2,i3,c3)+c(i,5,
     & 2,0)*ui(i1+5,i2+2,i3,c3)+c(i,0,3,0)*ui(i1,i2+3,i3,c3)+c(i,1,3,
     & 0)*ui(i1+1,i2+3,i3,c3)+c(i,2,3,0)*ui(i1+2,i2+3,i3,c3)+c(i,3,3,
     & 0)*ui(i1+3,i2+3,i3,c3)+c(i,4,3,0)*ui(i1+4,i2+3,i3,c3)+c(i,5,3,
     & 0)*ui(i1+5,i2+3,i3,c3)+c(i,0,4,0)*ui(i1,i2+4,i3,c3)+c(i,1,4,0)*
     & ui(i1+1,i2+4,i3,c3)+c(i,2,4,0)*ui(i1+2,i2+4,i3,c3)+c(i,3,4,0)*
     & ui(i1+3,i2+4,i3,c3)+c(i,4,4,0)*ui(i1+4,i2+4,i3,c3)+c(i,5,4,0)*
     & ui(i1+5,i2+4,i3,c3)+c(i,0,5,0)*ui(i1,i2+5,i3,c3)+c(i,1,5,0)*ui(
     & i1+1,i2+5,i3,c3)+c(i,2,5,0)*ui(i1+2,i2+5,i3,c3)+c(i,3,5,0)*ui(
     & i1+3,i2+5,i3,c3)+c(i,4,5,0)*ui(i1+4,i2+5,i3,c3)+c(i,5,5,0)*ui(
     & i1+5,i2+5,i3,c3)+c(i,0,0,1)*ui(i1,i2,i3+1,c3)+c(i,1,0,1)*ui(i1+
     & 1,i2,i3+1,c3)+c(i,2,0,1)*ui(i1+2,i2,i3+1,c3)+c(i,3,0,1)*ui(i1+
     & 3,i2,i3+1,c3)+c(i,4,0,1)*ui(i1+4,i2,i3+1,c3)+c(i,5,0,1)*ui(i1+
     & 5,i2,i3+1,c3)+c(i,0,1,1)*ui(i1,i2+1,i3+1,c3)+c(i,1,1,1)*ui(i1+
     & 1,i2+1,i3+1,c3)+c(i,2,1,1)*ui(i1+2,i2+1,i3+1,c3)+c(i,3,1,1)*ui(
     & i1+3,i2+1,i3+1,c3)+c(i,4,1,1)*ui(i1+4,i2+1,i3+1,c3)+c(i,5,1,1)*
     & ui(i1+5,i2+1,i3+1,c3)+c(i,0,2,1)*ui(i1,i2+2,i3+1,c3)+c(i,1,2,1)
     & *ui(i1+1,i2+2,i3+1,c3)+c(i,2,2,1)*ui(i1+2,i2+2,i3+1,c3)+c(i,3,
     & 2,1)*ui(i1+3,i2+2,i3+1,c3)+c(i,4,2,1)*ui(i1+4,i2+2,i3+1,c3)+c(
     & i,5,2,1)*ui(i1+5,i2+2,i3+1,c3)+c(i,0,3,1)*ui(i1,i2+3,i3+1,c3)+
     & c(i,1,3,1)*ui(i1+1,i2+3,i3+1,c3)+c(i,2,3,1)*ui(i1+2,i2+3,i3+1,
     & c3)+c(i,3,3,1)*ui(i1+3,i2+3,i3+1,c3)+c(i,4,3,1)*ui(i1+4,i2+3,
     & i3+1,c3)+c(i,5,3,1)*ui(i1+5,i2+3,i3+1,c3)+c(i,0,4,1)*ui(i1,i2+
     & 4,i3+1,c3)+c(i,1,4,1)*ui(i1+1,i2+4,i3+1,c3)+c(i,2,4,1)*ui(i1+2,
     & i2+4,i3+1,c3)+c(i,3,4,1)*ui(i1+3,i2+4,i3+1,c3)+c(i,4,4,1)*ui(
     & i1+4,i2+4,i3+1,c3)+c(i,5,4,1)*ui(i1+5,i2+4,i3+1,c3)+c(i,0,5,1)*
     & ui(i1,i2+5,i3+1,c3)+c(i,1,5,1)*ui(i1+1,i2+5,i3+1,c3)+c(i,2,5,1)
     & *ui(i1+2,i2+5,i3+1,c3)+c(i,3,5,1)*ui(i1+3,i2+5,i3+1,c3)+c(i,4,
     & 5,1)*ui(i1+4,i2+5,i3+1,c3)+c(i,5,5,1)*ui(i1+5,i2+5,i3+1,c3)+c(
     & i,0,0,2)*ui(i1,i2,i3+2,c3)+c(i,1,0,2)*ui(i1+1,i2,i3+2,c3)+c(i,
     & 2,0,2)*ui(i1+2,i2,i3+2,c3)+c(i,3,0,2)*ui(i1+3,i2,i3+2,c3)+c(i,
     & 4,0,2)*ui(i1+4,i2,i3+2,c3)+c(i,5,0,2)*ui(i1+5,i2,i3+2,c3)+c(i,
     & 0,1,2)*ui(i1,i2+1,i3+2,c3)+c(i,1,1,2)*ui(i1+1,i2+1,i3+2,c3)+c(
     & i,2,1,2)*ui(i1+2,i2+1,i3+2,c3)+c(i,3,1,2)*ui(i1+3,i2+1,i3+2,c3)
     & +c(i,4,1,2)*ui(i1+4,i2+1,i3+2,c3)+c(i,5,1,2)*ui(i1+5,i2+1,i3+2,
     & c3)+c(i,0,2,2)*ui(i1,i2+2,i3+2,c3)+c(i,1,2,2)*ui(i1+1,i2+2,i3+
     & 2,c3)+c(i,2,2,2)*ui(i1+2,i2+2,i3+2,c3)+c(i,3,2,2)*ui(i1+3,i2+2,
     & i3+2,c3)+c(i,4,2,2)*ui(i1+4,i2+2,i3+2,c3)+c(i,5,2,2)*ui(i1+5,
     & i2+2,i3+2,c3)+c(i,0,3,2)*ui(i1,i2+3,i3+2,c3)+c(i,1,3,2)*ui(i1+
     & 1,i2+3,i3+2,c3)+c(i,2,3,2)*ui(i1+2,i2+3,i3+2,c3)+c(i,3,3,2)*ui(
     & i1+3,i2+3,i3+2,c3)+c(i,4,3,2)*ui(i1+4,i2+3,i3+2,c3)+c(i,5,3,2)*
     & ui(i1+5,i2+3,i3+2,c3)+c(i,0,4,2)*ui(i1,i2+4,i3+2,c3)+c(i,1,4,2)
     & *ui(i1+1,i2+4,i3+2,c3)+c(i,2,4,2)*ui(i1+2,i2+4,i3+2,c3)+c(i,3,
     & 4,2)*ui(i1+3,i2+4,i3+2,c3)+c(i,4,4,2)*ui(i1+4,i2+4,i3+2,c3)+c(
     & i,5,4,2)*ui(i1+5,i2+4,i3+2,c3)+c(i,0,5,2)*ui(i1,i2+5,i3+2,c3)+
     & c(i,1,5,2)*ui(i1+1,i2+5,i3+2,c3)+c(i,2,5,2)*ui(i1+2,i2+5,i3+2,
     & c3)+c(i,3,5,2)*ui(i1+3,i2+5,i3+2,c3)+c(i,4,5,2)*ui(i1+4,i2+5,
     & i3+2,c3)+c(i,5,5,2)*ui(i1+5,i2+5,i3+2,c3)+c(i,0,0,3)*ui(i1,i2,
     & i3+3,c3)+c(i,1,0,3)*ui(i1+1,i2,i3+3,c3)+c(i,2,0,3)*ui(i1+2,i2,
     & i3+3,c3)+c(i,3,0,3)*ui(i1+3,i2,i3+3,c3)+c(i,4,0,3)*ui(i1+4,i2,
     & i3+3,c3)+c(i,5,0,3)*ui(i1+5,i2,i3+3,c3)+c(i,0,1,3)*ui(i1,i2+1,
     & i3+3,c3)+c(i,1,1,3)*ui(i1+1,i2+1,i3+3,c3)+c(i,2,1,3)*ui(i1+2,
     & i2+1,i3+3,c3)+c(i,3,1,3)*ui(i1+3,i2+1,i3+3,c3)+c(i,4,1,3)*ui(
     & i1+4,i2+1,i3+3,c3)+c(i,5,1,3)*ui(i1+5,i2+1,i3+3,c3)+c(i,0,2,3)*
     & ui(i1,i2+2,i3+3,c3)+c(i,1,2,3)*ui(i1+1,i2+2,i3+3,c3)+c(i,2,2,3)
     & *ui(i1+2,i2+2,i3+3,c3)+c(i,3,2,3)*ui(i1+3,i2+2,i3+3,c3)+c(i,4,
     & 2,3)*ui(i1+4,i2+2,i3+3,c3)+c(i,5,2,3)*ui(i1+5,i2+2,i3+3,c3)+c(
     & i,0,3,3)*ui(i1,i2+3,i3+3,c3)+c(i,1,3,3)*ui(i1+1,i2+3,i3+3,c3)+
     & c(i,2,3,3)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,3,3)*ui(i1+3,i2+3,i3+3,
     & c3)+c(i,4,3,3)*ui(i1+4,i2+3,i3+3,c3)+c(i,5,3,3)*ui(i1+5,i2+3,
     & i3+3,c3)+c(i,0,4,3)*ui(i1,i2+4,i3+3,c3)+c(i,1,4,3)*ui(i1+1,i2+
     & 4,i3+3,c3)+c(i,2,4,3)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,4,3)*ui(i1+3,
     & i2+4,i3+3,c3)+c(i,4,4,3)*ui(i1+4,i2+4,i3+3,c3)+c(i,5,4,3)*ui(
     & i1+5,i2+4,i3+3,c3)+c(i,0,5,3)*ui(i1,i2+5,i3+3,c3)+c(i,1,5,3)*
     & ui(i1+1,i2+5,i3+3,c3)+c(i,2,5,3)*ui(i1+2,i2+5,i3+3,c3)+c(i,3,5,
     & 3)*ui(i1+3,i2+5,i3+3,c3)+c(i,4,5,3)*ui(i1+4,i2+5,i3+3,c3)+c(i,
     & 5,5,3)*ui(i1+5,i2+5,i3+3,c3)+c(i,0,0,4)*ui(i1,i2,i3+4,c3)+c(i,
     & 1,0,4)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,4)*ui(i1+2,i2,i3+4,c3)+c(i,
     & 3,0,4)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,4)*ui(i1+4,i2,i3+4,c3)+c(i,
     & 5,0,4)*ui(i1+5,i2,i3+4,c3)+c(i,0,1,4)*ui(i1,i2+1,i3+4,c3)+c(i,
     & 1,1,4)*ui(i1+1,i2+1,i3+4,c3)+c(i,2,1,4)*ui(i1+2,i2+1,i3+4,c3)+
     & c(i,3,1,4)*ui(i1+3,i2+1,i3+4,c3)+c(i,4,1,4)*ui(i1+4,i2+1,i3+4,
     & c3)+c(i,5,1,4)*ui(i1+5,i2+1,i3+4,c3)+c(i,0,2,4)*ui(i1,i2+2,i3+
     & 4,c3)+c(i,1,2,4)*ui(i1+1,i2+2,i3+4,c3)+c(i,2,2,4)*ui(i1+2,i2+2,
     & i3+4,c3)+c(i,3,2,4)*ui(i1+3,i2+2,i3+4,c3)+c(i,4,2,4)*ui(i1+4,
     & i2+2,i3+4,c3)+c(i,5,2,4)*ui(i1+5,i2+2,i3+4,c3)+c(i,0,3,4)*ui(
     & i1,i2+3,i3+4,c3)+c(i,1,3,4)*ui(i1+1,i2+3,i3+4,c3)+c(i,2,3,4)*
     & ui(i1+2,i2+3,i3+4,c3)+c(i,3,3,4)*ui(i1+3,i2+3,i3+4,c3)+c(i,4,3,
     & 4)*ui(i1+4,i2+3,i3+4,c3)+c(i,5,3,4)*ui(i1+5,i2+3,i3+4,c3)+c(i,
     & 0,4,4)*ui(i1,i2+4,i3+4,c3)+c(i,1,4,4)*ui(i1+1,i2+4,i3+4,c3)+c(
     & i,2,4,4)*ui(i1+2,i2+4,i3+4,c3)+c(i,3,4,4)*ui(i1+3,i2+4,i3+4,c3)
     & +c(i,4,4,4)*ui(i1+4,i2+4,i3+4,c3)+c(i,5,4,4)*ui(i1+5,i2+4,i3+4,
     & c3)+c(i,0,5,4)*ui(i1,i2+5,i3+4,c3)+c(i,1,5,4)*ui(i1+1,i2+5,i3+
     & 4,c3)+c(i,2,5,4)*ui(i1+2,i2+5,i3+4,c3)+c(i,3,5,4)*ui(i1+3,i2+5,
     & i3+4,c3)+c(i,4,5,4)*ui(i1+4,i2+5,i3+4,c3)+c(i,5,5,4)*ui(i1+5,
     & i2+5,i3+4,c3)+c(i,0,0,5)*ui(i1,i2,i3+5,c3)+c(i,1,0,5)*ui(i1+1,
     & i2,i3+5,c3)+c(i,2,0,5)*ui(i1+2,i2,i3+5,c3)+c(i,3,0,5)*ui(i1+3,
     & i2,i3+5,c3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,4,0,5)*ui(i1+4,i2,i3+5,c3)+c(i,5,0,5)*ui(i1+5,i2,
     & i3+5,c3)+c(i,0,1,5)*ui(i1,i2+1,i3+5,c3)+c(i,1,1,5)*ui(i1+1,i2+
     & 1,i3+5,c3)+c(i,2,1,5)*ui(i1+2,i2+1,i3+5,c3)+c(i,3,1,5)*ui(i1+3,
     & i2+1,i3+5,c3)+c(i,4,1,5)*ui(i1+4,i2+1,i3+5,c3)+c(i,5,1,5)*ui(
     & i1+5,i2+1,i3+5,c3)+c(i,0,2,5)*ui(i1,i2+2,i3+5,c3)+c(i,1,2,5)*
     & ui(i1+1,i2+2,i3+5,c3)+c(i,2,2,5)*ui(i1+2,i2+2,i3+5,c3)+c(i,3,2,
     & 5)*ui(i1+3,i2+2,i3+5,c3)+c(i,4,2,5)*ui(i1+4,i2+2,i3+5,c3)+c(i,
     & 5,2,5)*ui(i1+5,i2+2,i3+5,c3)+c(i,0,3,5)*ui(i1,i2+3,i3+5,c3)+c(
     & i,1,3,5)*ui(i1+1,i2+3,i3+5,c3)+c(i,2,3,5)*ui(i1+2,i2+3,i3+5,c3)
     & +c(i,3,3,5)*ui(i1+3,i2+3,i3+5,c3)+c(i,4,3,5)*ui(i1+4,i2+3,i3+5,
     & c3)+c(i,5,3,5)*ui(i1+5,i2+3,i3+5,c3)+c(i,0,4,5)*ui(i1,i2+4,i3+
     & 5,c3)+c(i,1,4,5)*ui(i1+1,i2+4,i3+5,c3)+c(i,2,4,5)*ui(i1+2,i2+4,
     & i3+5,c3)+c(i,3,4,5)*ui(i1+3,i2+4,i3+5,c3)+c(i,4,4,5)*ui(i1+4,
     & i2+4,i3+5,c3)+c(i,5,4,5)*ui(i1+5,i2+4,i3+5,c3)+c(i,0,5,5)*ui(
     & i1,i2+5,i3+5,c3)+c(i,1,5,5)*ui(i1+1,i2+5,i3+5,c3)+c(i,2,5,5)*
     & ui(i1+2,i2+5,i3+5,c3)+c(i,3,5,5)*ui(i1+3,i2+5,i3+5,c3)+c(i,4,5,
     & 5)*ui(i1+4,i2+5,i3+5,c3)+c(i,5,5,5)*ui(i1+5,i2+5,i3+5,c3)


              end do
              end do
            end if
          else if( width(1).eq.7 .and. width(2).eq.7 .and. width(3)
     & .eq.7 )then
! loops3d($interp777(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)
            if( c3a.eq.c3b )then
              do c3=c3a,c3b
              do i=nia,nib
! interp777(ug(ip(i,1),ip(i,2),ip(i,3),c3))
                i1=il(i,1)
                i2=il(i,2)
                i3=il(i,3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,0,0)*ui(i1,i2,
     & i3,c3)+c(i,1,0,0)*ui(i1+1,i2,i3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3,
     & c3)+c(i,3,0,0)*ui(i1+3,i2,i3,c3)+c(i,4,0,0)*ui(i1+4,i2,i3,c3)+
     & c(i,5,0,0)*ui(i1+5,i2,i3,c3)+c(i,6,0,0)*ui(i1+6,i2,i3,c3)+c(i,
     & 0,1,0)*ui(i1,i2+1,i3,c3)+c(i,1,1,0)*ui(i1+1,i2+1,i3,c3)+c(i,2,
     & 1,0)*ui(i1+2,i2+1,i3,c3)+c(i,3,1,0)*ui(i1+3,i2+1,i3,c3)+c(i,4,
     & 1,0)*ui(i1+4,i2+1,i3,c3)+c(i,5,1,0)*ui(i1+5,i2+1,i3,c3)+c(i,6,
     & 1,0)*ui(i1+6,i2+1,i3,c3)+c(i,0,2,0)*ui(i1,i2+2,i3,c3)+c(i,1,2,
     & 0)*ui(i1+1,i2+2,i3,c3)+c(i,2,2,0)*ui(i1+2,i2+2,i3,c3)+c(i,3,2,
     & 0)*ui(i1+3,i2+2,i3,c3)+c(i,4,2,0)*ui(i1+4,i2+2,i3,c3)+c(i,5,2,
     & 0)*ui(i1+5,i2+2,i3,c3)+c(i,6,2,0)*ui(i1+6,i2+2,i3,c3)+c(i,0,3,
     & 0)*ui(i1,i2+3,i3,c3)+c(i,1,3,0)*ui(i1+1,i2+3,i3,c3)+c(i,2,3,0)*
     & ui(i1+2,i2+3,i3,c3)+c(i,3,3,0)*ui(i1+3,i2+3,i3,c3)+c(i,4,3,0)*
     & ui(i1+4,i2+3,i3,c3)+c(i,5,3,0)*ui(i1+5,i2+3,i3,c3)+c(i,6,3,0)*
     & ui(i1+6,i2+3,i3,c3)+c(i,0,4,0)*ui(i1,i2+4,i3,c3)+c(i,1,4,0)*ui(
     & i1+1,i2+4,i3,c3)+c(i,2,4,0)*ui(i1+2,i2+4,i3,c3)+c(i,3,4,0)*ui(
     & i1+3,i2+4,i3,c3)+c(i,4,4,0)*ui(i1+4,i2+4,i3,c3)+c(i,5,4,0)*ui(
     & i1+5,i2+4,i3,c3)+c(i,6,4,0)*ui(i1+6,i2+4,i3,c3)+c(i,0,5,0)*ui(
     & i1,i2+5,i3,c3)+c(i,1,5,0)*ui(i1+1,i2+5,i3,c3)+c(i,2,5,0)*ui(i1+
     & 2,i2+5,i3,c3)+c(i,3,5,0)*ui(i1+3,i2+5,i3,c3)+c(i,4,5,0)*ui(i1+
     & 4,i2+5,i3,c3)+c(i,5,5,0)*ui(i1+5,i2+5,i3,c3)+c(i,6,5,0)*ui(i1+
     & 6,i2+5,i3,c3)+c(i,0,6,0)*ui(i1,i2+6,i3,c3)+c(i,1,6,0)*ui(i1+1,
     & i2+6,i3,c3)+c(i,2,6,0)*ui(i1+2,i2+6,i3,c3)+c(i,3,6,0)*ui(i1+3,
     & i2+6,i3,c3)+c(i,4,6,0)*ui(i1+4,i2+6,i3,c3)+c(i,5,6,0)*ui(i1+5,
     & i2+6,i3,c3)+c(i,6,6,0)*ui(i1+6,i2+6,i3,c3)+c(i,0,0,1)*ui(i1,i2,
     & i3+1,c3)+c(i,1,0,1)*ui(i1+1,i2,i3+1,c3)+c(i,2,0,1)*ui(i1+2,i2,
     & i3+1,c3)+c(i,3,0,1)*ui(i1+3,i2,i3+1,c3)+c(i,4,0,1)*ui(i1+4,i2,
     & i3+1,c3)+c(i,5,0,1)*ui(i1+5,i2,i3+1,c3)+c(i,6,0,1)*ui(i1+6,i2,
     & i3+1,c3)+c(i,0,1,1)*ui(i1,i2+1,i3+1,c3)+c(i,1,1,1)*ui(i1+1,i2+
     & 1,i3+1,c3)+c(i,2,1,1)*ui(i1+2,i2+1,i3+1,c3)+c(i,3,1,1)*ui(i1+3,
     & i2+1,i3+1,c3)+c(i,4,1,1)*ui(i1+4,i2+1,i3+1,c3)+c(i,5,1,1)*ui(
     & i1+5,i2+1,i3+1,c3)+c(i,6,1,1)*ui(i1+6,i2+1,i3+1,c3)+c(i,0,2,1)*
     & ui(i1,i2+2,i3+1,c3)+c(i,1,2,1)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,2,1)
     & *ui(i1+2,i2+2,i3+1,c3)+c(i,3,2,1)*ui(i1+3,i2+2,i3+1,c3)+c(i,4,
     & 2,1)*ui(i1+4,i2+2,i3+1,c3)+c(i,5,2,1)*ui(i1+5,i2+2,i3+1,c3)+c(
     & i,6,2,1)*ui(i1+6,i2+2,i3+1,c3)+c(i,0,3,1)*ui(i1,i2+3,i3+1,c3)+
     & c(i,1,3,1)*ui(i1+1,i2+3,i3+1,c3)+c(i,2,3,1)*ui(i1+2,i2+3,i3+1,
     & c3)+c(i,3,3,1)*ui(i1+3,i2+3,i3+1,c3)+c(i,4,3,1)*ui(i1+4,i2+3,
     & i3+1,c3)+c(i,5,3,1)*ui(i1+5,i2+3,i3+1,c3)+c(i,6,3,1)*ui(i1+6,
     & i2+3,i3+1,c3)+c(i,0,4,1)*ui(i1,i2+4,i3+1,c3)+c(i,1,4,1)*ui(i1+
     & 1,i2+4,i3+1,c3)+c(i,2,4,1)*ui(i1+2,i2+4,i3+1,c3)+c(i,3,4,1)*ui(
     & i1+3,i2+4,i3+1,c3)+c(i,4,4,1)*ui(i1+4,i2+4,i3+1,c3)+c(i,5,4,1)*
     & ui(i1+5,i2+4,i3+1,c3)+c(i,6,4,1)*ui(i1+6,i2+4,i3+1,c3)+c(i,0,5,
     & 1)*ui(i1,i2+5,i3+1,c3)+c(i,1,5,1)*ui(i1+1,i2+5,i3+1,c3)+c(i,2,
     & 5,1)*ui(i1+2,i2+5,i3+1,c3)+c(i,3,5,1)*ui(i1+3,i2+5,i3+1,c3)+c(
     & i,4,5,1)*ui(i1+4,i2+5,i3+1,c3)+c(i,5,5,1)*ui(i1+5,i2+5,i3+1,c3)
     & +c(i,6,5,1)*ui(i1+6,i2+5,i3+1,c3)+c(i,0,6,1)*ui(i1,i2+6,i3+1,
     & c3)+c(i,1,6,1)*ui(i1+1,i2+6,i3+1,c3)+c(i,2,6,1)*ui(i1+2,i2+6,
     & i3+1,c3)+c(i,3,6,1)*ui(i1+3,i2+6,i3+1,c3)+c(i,4,6,1)*ui(i1+4,
     & i2+6,i3+1,c3)+c(i,5,6,1)*ui(i1+5,i2+6,i3+1,c3)+c(i,6,6,1)*ui(
     & i1+6,i2+6,i3+1,c3)+c(i,0,0,2)*ui(i1,i2,i3+2,c3)+c(i,1,0,2)*ui(
     & i1+1,i2,i3+2,c3)+c(i,2,0,2)*ui(i1+2,i2,i3+2,c3)+c(i,3,0,2)*ui(
     & i1+3,i2,i3+2,c3)+c(i,4,0,2)*ui(i1+4,i2,i3+2,c3)+c(i,5,0,2)*ui(
     & i1+5,i2,i3+2,c3)+c(i,6,0,2)*ui(i1+6,i2,i3+2,c3)+c(i,0,1,2)*ui(
     & i1,i2+1,i3+2,c3)+c(i,1,1,2)*ui(i1+1,i2+1,i3+2,c3)+c(i,2,1,2)*
     & ui(i1+2,i2+1,i3+2,c3)+c(i,3,1,2)*ui(i1+3,i2+1,i3+2,c3)+c(i,4,1,
     & 2)*ui(i1+4,i2+1,i3+2,c3)+c(i,5,1,2)*ui(i1+5,i2+1,i3+2,c3)+c(i,
     & 6,1,2)*ui(i1+6,i2+1,i3+2,c3)+c(i,0,2,2)*ui(i1,i2+2,i3+2,c3)+c(
     & i,1,2,2)*ui(i1+1,i2+2,i3+2,c3)+c(i,2,2,2)*ui(i1+2,i2+2,i3+2,c3)
     & +c(i,3,2,2)*ui(i1+3,i2+2,i3+2,c3)+c(i,4,2,2)*ui(i1+4,i2+2,i3+2,
     & c3)+c(i,5,2,2)*ui(i1+5,i2+2,i3+2,c3)+c(i,6,2,2)*ui(i1+6,i2+2,
     & i3+2,c3)+c(i,0,3,2)*ui(i1,i2+3,i3+2,c3)+c(i,1,3,2)*ui(i1+1,i2+
     & 3,i3+2,c3)+c(i,2,3,2)*ui(i1+2,i2+3,i3+2,c3)+c(i,3,3,2)*ui(i1+3,
     & i2+3,i3+2,c3)+c(i,4,3,2)*ui(i1+4,i2+3,i3+2,c3)+c(i,5,3,2)*ui(
     & i1+5,i2+3,i3+2,c3)+c(i,6,3,2)*ui(i1+6,i2+3,i3+2,c3)+c(i,0,4,2)*
     & ui(i1,i2+4,i3+2,c3)+c(i,1,4,2)*ui(i1+1,i2+4,i3+2,c3)+c(i,2,4,2)
     & *ui(i1+2,i2+4,i3+2,c3)+c(i,3,4,2)*ui(i1+3,i2+4,i3+2,c3)+c(i,4,
     & 4,2)*ui(i1+4,i2+4,i3+2,c3)+c(i,5,4,2)*ui(i1+5,i2+4,i3+2,c3)+c(
     & i,6,4,2)*ui(i1+6,i2+4,i3+2,c3)+c(i,0,5,2)*ui(i1,i2+5,i3+2,c3)+
     & c(i,1,5,2)*ui(i1+1,i2+5,i3+2,c3)+c(i,2,5,2)*ui(i1+2,i2+5,i3+2,
     & c3)+c(i,3,5,2)*ui(i1+3,i2+5,i3+2,c3)+c(i,4,5,2)*ui(i1+4,i2+5,
     & i3+2,c3)+c(i,5,5,2)*ui(i1+5,i2+5,i3+2,c3)+c(i,6,5,2)*ui(i1+6,
     & i2+5,i3+2,c3)+c(i,0,6,2)*ui(i1,i2+6,i3+2,c3)+c(i,1,6,2)*ui(i1+
     & 1,i2+6,i3+2,c3)+c(i,2,6,2)*ui(i1+2,i2+6,i3+2,c3)+c(i,3,6,2)*ui(
     & i1+3,i2+6,i3+2,c3)+c(i,4,6,2)*ui(i1+4,i2+6,i3+2,c3)+c(i,5,6,2)*
     & ui(i1+5,i2+6,i3+2,c3)+c(i,6,6,2)*ui(i1+6,i2+6,i3+2,c3)+c(i,0,0,
     & 3)*ui(i1,i2,i3+3,c3)+c(i,1,0,3)*ui(i1+1,i2,i3+3,c3)+c(i,2,0,3)*
     & ui(i1+2,i2,i3+3,c3)+c(i,3,0,3)*ui(i1+3,i2,i3+3,c3)+c(i,4,0,3)*
     & ui(i1+4,i2,i3+3,c3)+c(i,5,0,3)*ui(i1+5,i2,i3+3,c3)+c(i,6,0,3)*
     & ui(i1+6,i2,i3+3,c3)+c(i,0,1,3)*ui(i1,i2+1,i3+3,c3)+c(i,1,1,3)*
     & ui(i1+1,i2+1,i3+3,c3)+c(i,2,1,3)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,1,
     & 3)*ui(i1+3,i2+1,i3+3,c3)+c(i,4,1,3)*ui(i1+4,i2+1,i3+3,c3)+c(i,
     & 5,1,3)*ui(i1+5,i2+1,i3+3,c3)+c(i,6,1,3)*ui(i1+6,i2+1,i3+3,c3)+
     & c(i,0,2,3)*ui(i1,i2+2,i3+3,c3)+c(i,1,2,3)*ui(i1+1,i2+2,i3+3,c3)
     & +c(i,2,2,3)*ui(i1+2,i2+2,i3+3,c3)+c(i,3,2,3)*ui(i1+3,i2+2,i3+3,
     & c3)+c(i,4,2,3)*ui(i1+4,i2+2,i3+3,c3)+c(i,5,2,3)*ui(i1+5,i2+2,
     & i3+3,c3)+c(i,6,2,3)*ui(i1+6,i2+2,i3+3,c3)+c(i,0,3,3)*ui(i1,i2+
     & 3,i3+3,c3)+c(i,1,3,3)*ui(i1+1,i2+3,i3+3,c3)+c(i,2,3,3)*ui(i1+2,
     & i2+3,i3+3,c3)+c(i,3,3,3)*ui(i1+3,i2+3,i3+3,c3)+c(i,4,3,3)*ui(
     & i1+4,i2+3,i3+3,c3)+c(i,5,3,3)*ui(i1+5,i2+3,i3+3,c3)+c(i,6,3,3)*
     & ui(i1+6,i2+3,i3+3,c3)+c(i,0,4,3)*ui(i1,i2+4,i3+3,c3)+c(i,1,4,3)
     & *ui(i1+1,i2+4,i3+3,c3)+c(i,2,4,3)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,
     & 4,3)*ui(i1+3,i2+4,i3+3,c3)+c(i,4,4,3)*ui(i1+4,i2+4,i3+3,c3)+c(
     & i,5,4,3)*ui(i1+5,i2+4,i3+3,c3)+c(i,6,4,3)*ui(i1+6,i2+4,i3+3,c3)
     & +c(i,0,5,3)*ui(i1,i2+5,i3+3,c3)+c(i,1,5,3)*ui(i1+1,i2+5,i3+3,
     & c3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,2,5,3)*ui(i1+2,i2+5,i3+3,c3)+c(i,3,5,3)*ui(i1+3,
     & i2+5,i3+3,c3)+c(i,4,5,3)*ui(i1+4,i2+5,i3+3,c3)+c(i,5,5,3)*ui(
     & i1+5,i2+5,i3+3,c3)+c(i,6,5,3)*ui(i1+6,i2+5,i3+3,c3)+c(i,0,6,3)*
     & ui(i1,i2+6,i3+3,c3)+c(i,1,6,3)*ui(i1+1,i2+6,i3+3,c3)+c(i,2,6,3)
     & *ui(i1+2,i2+6,i3+3,c3)+c(i,3,6,3)*ui(i1+3,i2+6,i3+3,c3)+c(i,4,
     & 6,3)*ui(i1+4,i2+6,i3+3,c3)+c(i,5,6,3)*ui(i1+5,i2+6,i3+3,c3)+c(
     & i,6,6,3)*ui(i1+6,i2+6,i3+3,c3)+c(i,0,0,4)*ui(i1,i2,i3+4,c3)+c(
     & i,1,0,4)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,4)*ui(i1+2,i2,i3+4,c3)+c(
     & i,3,0,4)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,4)*ui(i1+4,i2,i3+4,c3)+c(
     & i,5,0,4)*ui(i1+5,i2,i3+4,c3)+c(i,6,0,4)*ui(i1+6,i2,i3+4,c3)+c(
     & i,0,1,4)*ui(i1,i2+1,i3+4,c3)+c(i,1,1,4)*ui(i1+1,i2+1,i3+4,c3)+
     & c(i,2,1,4)*ui(i1+2,i2+1,i3+4,c3)+c(i,3,1,4)*ui(i1+3,i2+1,i3+4,
     & c3)+c(i,4,1,4)*ui(i1+4,i2+1,i3+4,c3)+c(i,5,1,4)*ui(i1+5,i2+1,
     & i3+4,c3)+c(i,6,1,4)*ui(i1+6,i2+1,i3+4,c3)+c(i,0,2,4)*ui(i1,i2+
     & 2,i3+4,c3)+c(i,1,2,4)*ui(i1+1,i2+2,i3+4,c3)+c(i,2,2,4)*ui(i1+2,
     & i2+2,i3+4,c3)+c(i,3,2,4)*ui(i1+3,i2+2,i3+4,c3)+c(i,4,2,4)*ui(
     & i1+4,i2+2,i3+4,c3)+c(i,5,2,4)*ui(i1+5,i2+2,i3+4,c3)+c(i,6,2,4)*
     & ui(i1+6,i2+2,i3+4,c3)+c(i,0,3,4)*ui(i1,i2+3,i3+4,c3)+c(i,1,3,4)
     & *ui(i1+1,i2+3,i3+4,c3)+c(i,2,3,4)*ui(i1+2,i2+3,i3+4,c3)+c(i,3,
     & 3,4)*ui(i1+3,i2+3,i3+4,c3)+c(i,4,3,4)*ui(i1+4,i2+3,i3+4,c3)+c(
     & i,5,3,4)*ui(i1+5,i2+3,i3+4,c3)+c(i,6,3,4)*ui(i1+6,i2+3,i3+4,c3)
     & +c(i,0,4,4)*ui(i1,i2+4,i3+4,c3)+c(i,1,4,4)*ui(i1+1,i2+4,i3+4,
     & c3)+c(i,2,4,4)*ui(i1+2,i2+4,i3+4,c3)+c(i,3,4,4)*ui(i1+3,i2+4,
     & i3+4,c3)+c(i,4,4,4)*ui(i1+4,i2+4,i3+4,c3)+c(i,5,4,4)*ui(i1+5,
     & i2+4,i3+4,c3)+c(i,6,4,4)*ui(i1+6,i2+4,i3+4,c3)+c(i,0,5,4)*ui(
     & i1,i2+5,i3+4,c3)+c(i,1,5,4)*ui(i1+1,i2+5,i3+4,c3)+c(i,2,5,4)*
     & ui(i1+2,i2+5,i3+4,c3)+c(i,3,5,4)*ui(i1+3,i2+5,i3+4,c3)+c(i,4,5,
     & 4)*ui(i1+4,i2+5,i3+4,c3)+c(i,5,5,4)*ui(i1+5,i2+5,i3+4,c3)+c(i,
     & 6,5,4)*ui(i1+6,i2+5,i3+4,c3)+c(i,0,6,4)*ui(i1,i2+6,i3+4,c3)+c(
     & i,1,6,4)*ui(i1+1,i2+6,i3+4,c3)+c(i,2,6,4)*ui(i1+2,i2+6,i3+4,c3)
     & +c(i,3,6,4)*ui(i1+3,i2+6,i3+4,c3)+c(i,4,6,4)*ui(i1+4,i2+6,i3+4,
     & c3)+c(i,5,6,4)*ui(i1+5,i2+6,i3+4,c3)+c(i,6,6,4)*ui(i1+6,i2+6,
     & i3+4,c3)+c(i,0,0,5)*ui(i1,i2,i3+5,c3)+c(i,1,0,5)*ui(i1+1,i2,i3+
     & 5,c3)+c(i,2,0,5)*ui(i1+2,i2,i3+5,c3)+c(i,3,0,5)*ui(i1+3,i2,i3+
     & 5,c3)+c(i,4,0,5)*ui(i1+4,i2,i3+5,c3)+c(i,5,0,5)*ui(i1+5,i2,i3+
     & 5,c3)+c(i,6,0,5)*ui(i1+6,i2,i3+5,c3)+c(i,0,1,5)*ui(i1,i2+1,i3+
     & 5,c3)+c(i,1,1,5)*ui(i1+1,i2+1,i3+5,c3)+c(i,2,1,5)*ui(i1+2,i2+1,
     & i3+5,c3)+c(i,3,1,5)*ui(i1+3,i2+1,i3+5,c3)+c(i,4,1,5)*ui(i1+4,
     & i2+1,i3+5,c3)+c(i,5,1,5)*ui(i1+5,i2+1,i3+5,c3)+c(i,6,1,5)*ui(
     & i1+6,i2+1,i3+5,c3)+c(i,0,2,5)*ui(i1,i2+2,i3+5,c3)+c(i,1,2,5)*
     & ui(i1+1,i2+2,i3+5,c3)+c(i,2,2,5)*ui(i1+2,i2+2,i3+5,c3)+c(i,3,2,
     & 5)*ui(i1+3,i2+2,i3+5,c3)+c(i,4,2,5)*ui(i1+4,i2+2,i3+5,c3)+c(i,
     & 5,2,5)*ui(i1+5,i2+2,i3+5,c3)+c(i,6,2,5)*ui(i1+6,i2+2,i3+5,c3)+
     & c(i,0,3,5)*ui(i1,i2+3,i3+5,c3)+c(i,1,3,5)*ui(i1+1,i2+3,i3+5,c3)
     & +c(i,2,3,5)*ui(i1+2,i2+3,i3+5,c3)+c(i,3,3,5)*ui(i1+3,i2+3,i3+5,
     & c3)+c(i,4,3,5)*ui(i1+4,i2+3,i3+5,c3)+c(i,5,3,5)*ui(i1+5,i2+3,
     & i3+5,c3)+c(i,6,3,5)*ui(i1+6,i2+3,i3+5,c3)+c(i,0,4,5)*ui(i1,i2+
     & 4,i3+5,c3)+c(i,1,4,5)*ui(i1+1,i2+4,i3+5,c3)+c(i,2,4,5)*ui(i1+2,
     & i2+4,i3+5,c3)+c(i,3,4,5)*ui(i1+3,i2+4,i3+5,c3)+c(i,4,4,5)*ui(
     & i1+4,i2+4,i3+5,c3)+c(i,5,4,5)*ui(i1+5,i2+4,i3+5,c3)+c(i,6,4,5)*
     & ui(i1+6,i2+4,i3+5,c3)+c(i,0,5,5)*ui(i1,i2+5,i3+5,c3)+c(i,1,5,5)
     & *ui(i1+1,i2+5,i3+5,c3)+c(i,2,5,5)*ui(i1+2,i2+5,i3+5,c3)+c(i,3,
     & 5,5)*ui(i1+3,i2+5,i3+5,c3)+c(i,4,5,5)*ui(i1+4,i2+5,i3+5,c3)+c(
     & i,5,5,5)*ui(i1+5,i2+5,i3+5,c3)+c(i,6,5,5)*ui(i1+6,i2+5,i3+5,c3)
     & +c(i,0,6,5)*ui(i1,i2+6,i3+5,c3)+c(i,1,6,5)*ui(i1+1,i2+6,i3+5,
     & c3)+c(i,2,6,5)*ui(i1+2,i2+6,i3+5,c3)+c(i,3,6,5)*ui(i1+3,i2+6,
     & i3+5,c3)+c(i,4,6,5)*ui(i1+4,i2+6,i3+5,c3)+c(i,5,6,5)*ui(i1+5,
     & i2+6,i3+5,c3)+c(i,6,6,5)*ui(i1+6,i2+6,i3+5,c3)+c(i,0,0,6)*ui(
     & i1,i2,i3+6,c3)+c(i,1,0,6)*ui(i1+1,i2,i3+6,c3)+c(i,2,0,6)*ui(i1+
     & 2,i2,i3+6,c3)+c(i,3,0,6)*ui(i1+3,i2,i3+6,c3)+c(i,4,0,6)*ui(i1+
     & 4,i2,i3+6,c3)+c(i,5,0,6)*ui(i1+5,i2,i3+6,c3)+c(i,6,0,6)*ui(i1+
     & 6,i2,i3+6,c3)+c(i,0,1,6)*ui(i1,i2+1,i3+6,c3)+c(i,1,1,6)*ui(i1+
     & 1,i2+1,i3+6,c3)+c(i,2,1,6)*ui(i1+2,i2+1,i3+6,c3)+c(i,3,1,6)*ui(
     & i1+3,i2+1,i3+6,c3)+c(i,4,1,6)*ui(i1+4,i2+1,i3+6,c3)+c(i,5,1,6)*
     & ui(i1+5,i2+1,i3+6,c3)+c(i,6,1,6)*ui(i1+6,i2+1,i3+6,c3)+c(i,0,2,
     & 6)*ui(i1,i2+2,i3+6,c3)+c(i,1,2,6)*ui(i1+1,i2+2,i3+6,c3)+c(i,2,
     & 2,6)*ui(i1+2,i2+2,i3+6,c3)+c(i,3,2,6)*ui(i1+3,i2+2,i3+6,c3)+c(
     & i,4,2,6)*ui(i1+4,i2+2,i3+6,c3)+c(i,5,2,6)*ui(i1+5,i2+2,i3+6,c3)
     & +c(i,6,2,6)*ui(i1+6,i2+2,i3+6,c3)+c(i,0,3,6)*ui(i1,i2+3,i3+6,
     & c3)+c(i,1,3,6)*ui(i1+1,i2+3,i3+6,c3)+c(i,2,3,6)*ui(i1+2,i2+3,
     & i3+6,c3)+c(i,3,3,6)*ui(i1+3,i2+3,i3+6,c3)+c(i,4,3,6)*ui(i1+4,
     & i2+3,i3+6,c3)+c(i,5,3,6)*ui(i1+5,i2+3,i3+6,c3)+c(i,6,3,6)*ui(
     & i1+6,i2+3,i3+6,c3)+c(i,0,4,6)*ui(i1,i2+4,i3+6,c3)+c(i,1,4,6)*
     & ui(i1+1,i2+4,i3+6,c3)+c(i,2,4,6)*ui(i1+2,i2+4,i3+6,c3)+c(i,3,4,
     & 6)*ui(i1+3,i2+4,i3+6,c3)+c(i,4,4,6)*ui(i1+4,i2+4,i3+6,c3)+c(i,
     & 5,4,6)*ui(i1+5,i2+4,i3+6,c3)+c(i,6,4,6)*ui(i1+6,i2+4,i3+6,c3)+
     & c(i,0,5,6)*ui(i1,i2+5,i3+6,c3)+c(i,1,5,6)*ui(i1+1,i2+5,i3+6,c3)
     & +c(i,2,5,6)*ui(i1+2,i2+5,i3+6,c3)+c(i,3,5,6)*ui(i1+3,i2+5,i3+6,
     & c3)+c(i,4,5,6)*ui(i1+4,i2+5,i3+6,c3)+c(i,5,5,6)*ui(i1+5,i2+5,
     & i3+6,c3)+c(i,6,5,6)*ui(i1+6,i2+5,i3+6,c3)+c(i,0,6,6)*ui(i1,i2+
     & 6,i3+6,c3)+c(i,1,6,6)*ui(i1+1,i2+6,i3+6,c3)+c(i,2,6,6)*ui(i1+2,
     & i2+6,i3+6,c3)+c(i,3,6,6)*ui(i1+3,i2+6,i3+6,c3)+c(i,4,6,6)*ui(
     & i1+4,i2+6,i3+6,c3)+c(i,5,6,6)*ui(i1+5,i2+6,i3+6,c3)+c(i,6,6,6)*
     & ui(i1+6,i2+6,i3+6,c3)


              end do
              end do
            else
              ! put "c" loop as inner loop, this seems to be faster
              do i=nia,nib
              do c3=c3a,c3b
! interp777(ug(ip(i,1),ip(i,2),ip(i,3),c3))
                i1=il(i,1)
                i2=il(i,2)
                i3=il(i,3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,0,0)*ui(i1,i2,
     & i3,c3)+c(i,1,0,0)*ui(i1+1,i2,i3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3,
     & c3)+c(i,3,0,0)*ui(i1+3,i2,i3,c3)+c(i,4,0,0)*ui(i1+4,i2,i3,c3)+
     & c(i,5,0,0)*ui(i1+5,i2,i3,c3)+c(i,6,0,0)*ui(i1+6,i2,i3,c3)+c(i,
     & 0,1,0)*ui(i1,i2+1,i3,c3)+c(i,1,1,0)*ui(i1+1,i2+1,i3,c3)+c(i,2,
     & 1,0)*ui(i1+2,i2+1,i3,c3)+c(i,3,1,0)*ui(i1+3,i2+1,i3,c3)+c(i,4,
     & 1,0)*ui(i1+4,i2+1,i3,c3)+c(i,5,1,0)*ui(i1+5,i2+1,i3,c3)+c(i,6,
     & 1,0)*ui(i1+6,i2+1,i3,c3)+c(i,0,2,0)*ui(i1,i2+2,i3,c3)+c(i,1,2,
     & 0)*ui(i1+1,i2+2,i3,c3)+c(i,2,2,0)*ui(i1+2,i2+2,i3,c3)+c(i,3,2,
     & 0)*ui(i1+3,i2+2,i3,c3)+c(i,4,2,0)*ui(i1+4,i2+2,i3,c3)+c(i,5,2,
     & 0)*ui(i1+5,i2+2,i3,c3)+c(i,6,2,0)*ui(i1+6,i2+2,i3,c3)+c(i,0,3,
     & 0)*ui(i1,i2+3,i3,c3)+c(i,1,3,0)*ui(i1+1,i2+3,i3,c3)+c(i,2,3,0)*
     & ui(i1+2,i2+3,i3,c3)+c(i,3,3,0)*ui(i1+3,i2+3,i3,c3)+c(i,4,3,0)*
     & ui(i1+4,i2+3,i3,c3)+c(i,5,3,0)*ui(i1+5,i2+3,i3,c3)+c(i,6,3,0)*
     & ui(i1+6,i2+3,i3,c3)+c(i,0,4,0)*ui(i1,i2+4,i3,c3)+c(i,1,4,0)*ui(
     & i1+1,i2+4,i3,c3)+c(i,2,4,0)*ui(i1+2,i2+4,i3,c3)+c(i,3,4,0)*ui(
     & i1+3,i2+4,i3,c3)+c(i,4,4,0)*ui(i1+4,i2+4,i3,c3)+c(i,5,4,0)*ui(
     & i1+5,i2+4,i3,c3)+c(i,6,4,0)*ui(i1+6,i2+4,i3,c3)+c(i,0,5,0)*ui(
     & i1,i2+5,i3,c3)+c(i,1,5,0)*ui(i1+1,i2+5,i3,c3)+c(i,2,5,0)*ui(i1+
     & 2,i2+5,i3,c3)+c(i,3,5,0)*ui(i1+3,i2+5,i3,c3)+c(i,4,5,0)*ui(i1+
     & 4,i2+5,i3,c3)+c(i,5,5,0)*ui(i1+5,i2+5,i3,c3)+c(i,6,5,0)*ui(i1+
     & 6,i2+5,i3,c3)+c(i,0,6,0)*ui(i1,i2+6,i3,c3)+c(i,1,6,0)*ui(i1+1,
     & i2+6,i3,c3)+c(i,2,6,0)*ui(i1+2,i2+6,i3,c3)+c(i,3,6,0)*ui(i1+3,
     & i2+6,i3,c3)+c(i,4,6,0)*ui(i1+4,i2+6,i3,c3)+c(i,5,6,0)*ui(i1+5,
     & i2+6,i3,c3)+c(i,6,6,0)*ui(i1+6,i2+6,i3,c3)+c(i,0,0,1)*ui(i1,i2,
     & i3+1,c3)+c(i,1,0,1)*ui(i1+1,i2,i3+1,c3)+c(i,2,0,1)*ui(i1+2,i2,
     & i3+1,c3)+c(i,3,0,1)*ui(i1+3,i2,i3+1,c3)+c(i,4,0,1)*ui(i1+4,i2,
     & i3+1,c3)+c(i,5,0,1)*ui(i1+5,i2,i3+1,c3)+c(i,6,0,1)*ui(i1+6,i2,
     & i3+1,c3)+c(i,0,1,1)*ui(i1,i2+1,i3+1,c3)+c(i,1,1,1)*ui(i1+1,i2+
     & 1,i3+1,c3)+c(i,2,1,1)*ui(i1+2,i2+1,i3+1,c3)+c(i,3,1,1)*ui(i1+3,
     & i2+1,i3+1,c3)+c(i,4,1,1)*ui(i1+4,i2+1,i3+1,c3)+c(i,5,1,1)*ui(
     & i1+5,i2+1,i3+1,c3)+c(i,6,1,1)*ui(i1+6,i2+1,i3+1,c3)+c(i,0,2,1)*
     & ui(i1,i2+2,i3+1,c3)+c(i,1,2,1)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,2,1)
     & *ui(i1+2,i2+2,i3+1,c3)+c(i,3,2,1)*ui(i1+3,i2+2,i3+1,c3)+c(i,4,
     & 2,1)*ui(i1+4,i2+2,i3+1,c3)+c(i,5,2,1)*ui(i1+5,i2+2,i3+1,c3)+c(
     & i,6,2,1)*ui(i1+6,i2+2,i3+1,c3)+c(i,0,3,1)*ui(i1,i2+3,i3+1,c3)+
     & c(i,1,3,1)*ui(i1+1,i2+3,i3+1,c3)+c(i,2,3,1)*ui(i1+2,i2+3,i3+1,
     & c3)+c(i,3,3,1)*ui(i1+3,i2+3,i3+1,c3)+c(i,4,3,1)*ui(i1+4,i2+3,
     & i3+1,c3)+c(i,5,3,1)*ui(i1+5,i2+3,i3+1,c3)+c(i,6,3,1)*ui(i1+6,
     & i2+3,i3+1,c3)+c(i,0,4,1)*ui(i1,i2+4,i3+1,c3)+c(i,1,4,1)*ui(i1+
     & 1,i2+4,i3+1,c3)+c(i,2,4,1)*ui(i1+2,i2+4,i3+1,c3)+c(i,3,4,1)*ui(
     & i1+3,i2+4,i3+1,c3)+c(i,4,4,1)*ui(i1+4,i2+4,i3+1,c3)+c(i,5,4,1)*
     & ui(i1+5,i2+4,i3+1,c3)+c(i,6,4,1)*ui(i1+6,i2+4,i3+1,c3)+c(i,0,5,
     & 1)*ui(i1,i2+5,i3+1,c3)+c(i,1,5,1)*ui(i1+1,i2+5,i3+1,c3)+c(i,2,
     & 5,1)*ui(i1+2,i2+5,i3+1,c3)+c(i,3,5,1)*ui(i1+3,i2+5,i3+1,c3)+c(
     & i,4,5,1)*ui(i1+4,i2+5,i3+1,c3)+c(i,5,5,1)*ui(i1+5,i2+5,i3+1,c3)
     & +c(i,6,5,1)*ui(i1+6,i2+5,i3+1,c3)+c(i,0,6,1)*ui(i1,i2+6,i3+1,
     & c3)+c(i,1,6,1)*ui(i1+1,i2+6,i3+1,c3)+c(i,2,6,1)*ui(i1+2,i2+6,
     & i3+1,c3)+c(i,3,6,1)*ui(i1+3,i2+6,i3+1,c3)+c(i,4,6,1)*ui(i1+4,
     & i2+6,i3+1,c3)+c(i,5,6,1)*ui(i1+5,i2+6,i3+1,c3)+c(i,6,6,1)*ui(
     & i1+6,i2+6,i3+1,c3)+c(i,0,0,2)*ui(i1,i2,i3+2,c3)+c(i,1,0,2)*ui(
     & i1+1,i2,i3+2,c3)+c(i,2,0,2)*ui(i1+2,i2,i3+2,c3)+c(i,3,0,2)*ui(
     & i1+3,i2,i3+2,c3)+c(i,4,0,2)*ui(i1+4,i2,i3+2,c3)+c(i,5,0,2)*ui(
     & i1+5,i2,i3+2,c3)+c(i,6,0,2)*ui(i1+6,i2,i3+2,c3)+c(i,0,1,2)*ui(
     & i1,i2+1,i3+2,c3)+c(i,1,1,2)*ui(i1+1,i2+1,i3+2,c3)+c(i,2,1,2)*
     & ui(i1+2,i2+1,i3+2,c3)+c(i,3,1,2)*ui(i1+3,i2+1,i3+2,c3)+c(i,4,1,
     & 2)*ui(i1+4,i2+1,i3+2,c3)+c(i,5,1,2)*ui(i1+5,i2+1,i3+2,c3)+c(i,
     & 6,1,2)*ui(i1+6,i2+1,i3+2,c3)+c(i,0,2,2)*ui(i1,i2+2,i3+2,c3)+c(
     & i,1,2,2)*ui(i1+1,i2+2,i3+2,c3)+c(i,2,2,2)*ui(i1+2,i2+2,i3+2,c3)
     & +c(i,3,2,2)*ui(i1+3,i2+2,i3+2,c3)+c(i,4,2,2)*ui(i1+4,i2+2,i3+2,
     & c3)+c(i,5,2,2)*ui(i1+5,i2+2,i3+2,c3)+c(i,6,2,2)*ui(i1+6,i2+2,
     & i3+2,c3)+c(i,0,3,2)*ui(i1,i2+3,i3+2,c3)+c(i,1,3,2)*ui(i1+1,i2+
     & 3,i3+2,c3)+c(i,2,3,2)*ui(i1+2,i2+3,i3+2,c3)+c(i,3,3,2)*ui(i1+3,
     & i2+3,i3+2,c3)+c(i,4,3,2)*ui(i1+4,i2+3,i3+2,c3)+c(i,5,3,2)*ui(
     & i1+5,i2+3,i3+2,c3)+c(i,6,3,2)*ui(i1+6,i2+3,i3+2,c3)+c(i,0,4,2)*
     & ui(i1,i2+4,i3+2,c3)+c(i,1,4,2)*ui(i1+1,i2+4,i3+2,c3)+c(i,2,4,2)
     & *ui(i1+2,i2+4,i3+2,c3)+c(i,3,4,2)*ui(i1+3,i2+4,i3+2,c3)+c(i,4,
     & 4,2)*ui(i1+4,i2+4,i3+2,c3)+c(i,5,4,2)*ui(i1+5,i2+4,i3+2,c3)+c(
     & i,6,4,2)*ui(i1+6,i2+4,i3+2,c3)+c(i,0,5,2)*ui(i1,i2+5,i3+2,c3)+
     & c(i,1,5,2)*ui(i1+1,i2+5,i3+2,c3)+c(i,2,5,2)*ui(i1+2,i2+5,i3+2,
     & c3)+c(i,3,5,2)*ui(i1+3,i2+5,i3+2,c3)+c(i,4,5,2)*ui(i1+4,i2+5,
     & i3+2,c3)+c(i,5,5,2)*ui(i1+5,i2+5,i3+2,c3)+c(i,6,5,2)*ui(i1+6,
     & i2+5,i3+2,c3)+c(i,0,6,2)*ui(i1,i2+6,i3+2,c3)+c(i,1,6,2)*ui(i1+
     & 1,i2+6,i3+2,c3)+c(i,2,6,2)*ui(i1+2,i2+6,i3+2,c3)+c(i,3,6,2)*ui(
     & i1+3,i2+6,i3+2,c3)+c(i,4,6,2)*ui(i1+4,i2+6,i3+2,c3)+c(i,5,6,2)*
     & ui(i1+5,i2+6,i3+2,c3)+c(i,6,6,2)*ui(i1+6,i2+6,i3+2,c3)+c(i,0,0,
     & 3)*ui(i1,i2,i3+3,c3)+c(i,1,0,3)*ui(i1+1,i2,i3+3,c3)+c(i,2,0,3)*
     & ui(i1+2,i2,i3+3,c3)+c(i,3,0,3)*ui(i1+3,i2,i3+3,c3)+c(i,4,0,3)*
     & ui(i1+4,i2,i3+3,c3)+c(i,5,0,3)*ui(i1+5,i2,i3+3,c3)+c(i,6,0,3)*
     & ui(i1+6,i2,i3+3,c3)+c(i,0,1,3)*ui(i1,i2+1,i3+3,c3)+c(i,1,1,3)*
     & ui(i1+1,i2+1,i3+3,c3)+c(i,2,1,3)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,1,
     & 3)*ui(i1+3,i2+1,i3+3,c3)+c(i,4,1,3)*ui(i1+4,i2+1,i3+3,c3)+c(i,
     & 5,1,3)*ui(i1+5,i2+1,i3+3,c3)+c(i,6,1,3)*ui(i1+6,i2+1,i3+3,c3)+
     & c(i,0,2,3)*ui(i1,i2+2,i3+3,c3)+c(i,1,2,3)*ui(i1+1,i2+2,i3+3,c3)
     & +c(i,2,2,3)*ui(i1+2,i2+2,i3+3,c3)+c(i,3,2,3)*ui(i1+3,i2+2,i3+3,
     & c3)+c(i,4,2,3)*ui(i1+4,i2+2,i3+3,c3)+c(i,5,2,3)*ui(i1+5,i2+2,
     & i3+3,c3)+c(i,6,2,3)*ui(i1+6,i2+2,i3+3,c3)+c(i,0,3,3)*ui(i1,i2+
     & 3,i3+3,c3)+c(i,1,3,3)*ui(i1+1,i2+3,i3+3,c3)+c(i,2,3,3)*ui(i1+2,
     & i2+3,i3+3,c3)+c(i,3,3,3)*ui(i1+3,i2+3,i3+3,c3)+c(i,4,3,3)*ui(
     & i1+4,i2+3,i3+3,c3)+c(i,5,3,3)*ui(i1+5,i2+3,i3+3,c3)+c(i,6,3,3)*
     & ui(i1+6,i2+3,i3+3,c3)+c(i,0,4,3)*ui(i1,i2+4,i3+3,c3)+c(i,1,4,3)
     & *ui(i1+1,i2+4,i3+3,c3)+c(i,2,4,3)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,
     & 4,3)*ui(i1+3,i2+4,i3+3,c3)+c(i,4,4,3)*ui(i1+4,i2+4,i3+3,c3)+c(
     & i,5,4,3)*ui(i1+5,i2+4,i3+3,c3)+c(i,6,4,3)*ui(i1+6,i2+4,i3+3,c3)
     & +c(i,0,5,3)*ui(i1,i2+5,i3+3,c3)+c(i,1,5,3)*ui(i1+1,i2+5,i3+3,
     & c3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,2,5,3)*ui(i1+2,i2+5,i3+3,c3)+c(i,3,5,3)*ui(i1+3,
     & i2+5,i3+3,c3)+c(i,4,5,3)*ui(i1+4,i2+5,i3+3,c3)+c(i,5,5,3)*ui(
     & i1+5,i2+5,i3+3,c3)+c(i,6,5,3)*ui(i1+6,i2+5,i3+3,c3)+c(i,0,6,3)*
     & ui(i1,i2+6,i3+3,c3)+c(i,1,6,3)*ui(i1+1,i2+6,i3+3,c3)+c(i,2,6,3)
     & *ui(i1+2,i2+6,i3+3,c3)+c(i,3,6,3)*ui(i1+3,i2+6,i3+3,c3)+c(i,4,
     & 6,3)*ui(i1+4,i2+6,i3+3,c3)+c(i,5,6,3)*ui(i1+5,i2+6,i3+3,c3)+c(
     & i,6,6,3)*ui(i1+6,i2+6,i3+3,c3)+c(i,0,0,4)*ui(i1,i2,i3+4,c3)+c(
     & i,1,0,4)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,4)*ui(i1+2,i2,i3+4,c3)+c(
     & i,3,0,4)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,4)*ui(i1+4,i2,i3+4,c3)+c(
     & i,5,0,4)*ui(i1+5,i2,i3+4,c3)+c(i,6,0,4)*ui(i1+6,i2,i3+4,c3)+c(
     & i,0,1,4)*ui(i1,i2+1,i3+4,c3)+c(i,1,1,4)*ui(i1+1,i2+1,i3+4,c3)+
     & c(i,2,1,4)*ui(i1+2,i2+1,i3+4,c3)+c(i,3,1,4)*ui(i1+3,i2+1,i3+4,
     & c3)+c(i,4,1,4)*ui(i1+4,i2+1,i3+4,c3)+c(i,5,1,4)*ui(i1+5,i2+1,
     & i3+4,c3)+c(i,6,1,4)*ui(i1+6,i2+1,i3+4,c3)+c(i,0,2,4)*ui(i1,i2+
     & 2,i3+4,c3)+c(i,1,2,4)*ui(i1+1,i2+2,i3+4,c3)+c(i,2,2,4)*ui(i1+2,
     & i2+2,i3+4,c3)+c(i,3,2,4)*ui(i1+3,i2+2,i3+4,c3)+c(i,4,2,4)*ui(
     & i1+4,i2+2,i3+4,c3)+c(i,5,2,4)*ui(i1+5,i2+2,i3+4,c3)+c(i,6,2,4)*
     & ui(i1+6,i2+2,i3+4,c3)+c(i,0,3,4)*ui(i1,i2+3,i3+4,c3)+c(i,1,3,4)
     & *ui(i1+1,i2+3,i3+4,c3)+c(i,2,3,4)*ui(i1+2,i2+3,i3+4,c3)+c(i,3,
     & 3,4)*ui(i1+3,i2+3,i3+4,c3)+c(i,4,3,4)*ui(i1+4,i2+3,i3+4,c3)+c(
     & i,5,3,4)*ui(i1+5,i2+3,i3+4,c3)+c(i,6,3,4)*ui(i1+6,i2+3,i3+4,c3)
     & +c(i,0,4,4)*ui(i1,i2+4,i3+4,c3)+c(i,1,4,4)*ui(i1+1,i2+4,i3+4,
     & c3)+c(i,2,4,4)*ui(i1+2,i2+4,i3+4,c3)+c(i,3,4,4)*ui(i1+3,i2+4,
     & i3+4,c3)+c(i,4,4,4)*ui(i1+4,i2+4,i3+4,c3)+c(i,5,4,4)*ui(i1+5,
     & i2+4,i3+4,c3)+c(i,6,4,4)*ui(i1+6,i2+4,i3+4,c3)+c(i,0,5,4)*ui(
     & i1,i2+5,i3+4,c3)+c(i,1,5,4)*ui(i1+1,i2+5,i3+4,c3)+c(i,2,5,4)*
     & ui(i1+2,i2+5,i3+4,c3)+c(i,3,5,4)*ui(i1+3,i2+5,i3+4,c3)+c(i,4,5,
     & 4)*ui(i1+4,i2+5,i3+4,c3)+c(i,5,5,4)*ui(i1+5,i2+5,i3+4,c3)+c(i,
     & 6,5,4)*ui(i1+6,i2+5,i3+4,c3)+c(i,0,6,4)*ui(i1,i2+6,i3+4,c3)+c(
     & i,1,6,4)*ui(i1+1,i2+6,i3+4,c3)+c(i,2,6,4)*ui(i1+2,i2+6,i3+4,c3)
     & +c(i,3,6,4)*ui(i1+3,i2+6,i3+4,c3)+c(i,4,6,4)*ui(i1+4,i2+6,i3+4,
     & c3)+c(i,5,6,4)*ui(i1+5,i2+6,i3+4,c3)+c(i,6,6,4)*ui(i1+6,i2+6,
     & i3+4,c3)+c(i,0,0,5)*ui(i1,i2,i3+5,c3)+c(i,1,0,5)*ui(i1+1,i2,i3+
     & 5,c3)+c(i,2,0,5)*ui(i1+2,i2,i3+5,c3)+c(i,3,0,5)*ui(i1+3,i2,i3+
     & 5,c3)+c(i,4,0,5)*ui(i1+4,i2,i3+5,c3)+c(i,5,0,5)*ui(i1+5,i2,i3+
     & 5,c3)+c(i,6,0,5)*ui(i1+6,i2,i3+5,c3)+c(i,0,1,5)*ui(i1,i2+1,i3+
     & 5,c3)+c(i,1,1,5)*ui(i1+1,i2+1,i3+5,c3)+c(i,2,1,5)*ui(i1+2,i2+1,
     & i3+5,c3)+c(i,3,1,5)*ui(i1+3,i2+1,i3+5,c3)+c(i,4,1,5)*ui(i1+4,
     & i2+1,i3+5,c3)+c(i,5,1,5)*ui(i1+5,i2+1,i3+5,c3)+c(i,6,1,5)*ui(
     & i1+6,i2+1,i3+5,c3)+c(i,0,2,5)*ui(i1,i2+2,i3+5,c3)+c(i,1,2,5)*
     & ui(i1+1,i2+2,i3+5,c3)+c(i,2,2,5)*ui(i1+2,i2+2,i3+5,c3)+c(i,3,2,
     & 5)*ui(i1+3,i2+2,i3+5,c3)+c(i,4,2,5)*ui(i1+4,i2+2,i3+5,c3)+c(i,
     & 5,2,5)*ui(i1+5,i2+2,i3+5,c3)+c(i,6,2,5)*ui(i1+6,i2+2,i3+5,c3)+
     & c(i,0,3,5)*ui(i1,i2+3,i3+5,c3)+c(i,1,3,5)*ui(i1+1,i2+3,i3+5,c3)
     & +c(i,2,3,5)*ui(i1+2,i2+3,i3+5,c3)+c(i,3,3,5)*ui(i1+3,i2+3,i3+5,
     & c3)+c(i,4,3,5)*ui(i1+4,i2+3,i3+5,c3)+c(i,5,3,5)*ui(i1+5,i2+3,
     & i3+5,c3)+c(i,6,3,5)*ui(i1+6,i2+3,i3+5,c3)+c(i,0,4,5)*ui(i1,i2+
     & 4,i3+5,c3)+c(i,1,4,5)*ui(i1+1,i2+4,i3+5,c3)+c(i,2,4,5)*ui(i1+2,
     & i2+4,i3+5,c3)+c(i,3,4,5)*ui(i1+3,i2+4,i3+5,c3)+c(i,4,4,5)*ui(
     & i1+4,i2+4,i3+5,c3)+c(i,5,4,5)*ui(i1+5,i2+4,i3+5,c3)+c(i,6,4,5)*
     & ui(i1+6,i2+4,i3+5,c3)+c(i,0,5,5)*ui(i1,i2+5,i3+5,c3)+c(i,1,5,5)
     & *ui(i1+1,i2+5,i3+5,c3)+c(i,2,5,5)*ui(i1+2,i2+5,i3+5,c3)+c(i,3,
     & 5,5)*ui(i1+3,i2+5,i3+5,c3)+c(i,4,5,5)*ui(i1+4,i2+5,i3+5,c3)+c(
     & i,5,5,5)*ui(i1+5,i2+5,i3+5,c3)+c(i,6,5,5)*ui(i1+6,i2+5,i3+5,c3)
     & +c(i,0,6,5)*ui(i1,i2+6,i3+5,c3)+c(i,1,6,5)*ui(i1+1,i2+6,i3+5,
     & c3)+c(i,2,6,5)*ui(i1+2,i2+6,i3+5,c3)+c(i,3,6,5)*ui(i1+3,i2+6,
     & i3+5,c3)+c(i,4,6,5)*ui(i1+4,i2+6,i3+5,c3)+c(i,5,6,5)*ui(i1+5,
     & i2+6,i3+5,c3)+c(i,6,6,5)*ui(i1+6,i2+6,i3+5,c3)+c(i,0,0,6)*ui(
     & i1,i2,i3+6,c3)+c(i,1,0,6)*ui(i1+1,i2,i3+6,c3)+c(i,2,0,6)*ui(i1+
     & 2,i2,i3+6,c3)+c(i,3,0,6)*ui(i1+3,i2,i3+6,c3)+c(i,4,0,6)*ui(i1+
     & 4,i2,i3+6,c3)+c(i,5,0,6)*ui(i1+5,i2,i3+6,c3)+c(i,6,0,6)*ui(i1+
     & 6,i2,i3+6,c3)+c(i,0,1,6)*ui(i1,i2+1,i3+6,c3)+c(i,1,1,6)*ui(i1+
     & 1,i2+1,i3+6,c3)+c(i,2,1,6)*ui(i1+2,i2+1,i3+6,c3)+c(i,3,1,6)*ui(
     & i1+3,i2+1,i3+6,c3)+c(i,4,1,6)*ui(i1+4,i2+1,i3+6,c3)+c(i,5,1,6)*
     & ui(i1+5,i2+1,i3+6,c3)+c(i,6,1,6)*ui(i1+6,i2+1,i3+6,c3)+c(i,0,2,
     & 6)*ui(i1,i2+2,i3+6,c3)+c(i,1,2,6)*ui(i1+1,i2+2,i3+6,c3)+c(i,2,
     & 2,6)*ui(i1+2,i2+2,i3+6,c3)+c(i,3,2,6)*ui(i1+3,i2+2,i3+6,c3)+c(
     & i,4,2,6)*ui(i1+4,i2+2,i3+6,c3)+c(i,5,2,6)*ui(i1+5,i2+2,i3+6,c3)
     & +c(i,6,2,6)*ui(i1+6,i2+2,i3+6,c3)+c(i,0,3,6)*ui(i1,i2+3,i3+6,
     & c3)+c(i,1,3,6)*ui(i1+1,i2+3,i3+6,c3)+c(i,2,3,6)*ui(i1+2,i2+3,
     & i3+6,c3)+c(i,3,3,6)*ui(i1+3,i2+3,i3+6,c3)+c(i,4,3,6)*ui(i1+4,
     & i2+3,i3+6,c3)+c(i,5,3,6)*ui(i1+5,i2+3,i3+6,c3)+c(i,6,3,6)*ui(
     & i1+6,i2+3,i3+6,c3)+c(i,0,4,6)*ui(i1,i2+4,i3+6,c3)+c(i,1,4,6)*
     & ui(i1+1,i2+4,i3+6,c3)+c(i,2,4,6)*ui(i1+2,i2+4,i3+6,c3)+c(i,3,4,
     & 6)*ui(i1+3,i2+4,i3+6,c3)+c(i,4,4,6)*ui(i1+4,i2+4,i3+6,c3)+c(i,
     & 5,4,6)*ui(i1+5,i2+4,i3+6,c3)+c(i,6,4,6)*ui(i1+6,i2+4,i3+6,c3)+
     & c(i,0,5,6)*ui(i1,i2+5,i3+6,c3)+c(i,1,5,6)*ui(i1+1,i2+5,i3+6,c3)
     & +c(i,2,5,6)*ui(i1+2,i2+5,i3+6,c3)+c(i,3,5,6)*ui(i1+3,i2+5,i3+6,
     & c3)+c(i,4,5,6)*ui(i1+4,i2+5,i3+6,c3)+c(i,5,5,6)*ui(i1+5,i2+5,
     & i3+6,c3)+c(i,6,5,6)*ui(i1+6,i2+5,i3+6,c3)+c(i,0,6,6)*ui(i1,i2+
     & 6,i3+6,c3)+c(i,1,6,6)*ui(i1+1,i2+6,i3+6,c3)+c(i,2,6,6)*ui(i1+2,
     & i2+6,i3+6,c3)+c(i,3,6,6)*ui(i1+3,i2+6,i3+6,c3)+c(i,4,6,6)*ui(
     & i1+4,i2+6,i3+6,c3)+c(i,5,6,6)*ui(i1+5,i2+6,i3+6,c3)+c(i,6,6,6)*
     & ui(i1+6,i2+6,i3+6,c3)


              end do
              end do
            end if
          else if( width(1).eq.8 .and. width(2).eq.8 .and. width(3)
     & .eq.8 )then
! loops3d($interp888(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)
            if( c3a.eq.c3b )then
              do c3=c3a,c3b
              do i=nia,nib
! interp888(ug(ip(i,1),ip(i,2),ip(i,3),c3))
                i1=il(i,1)
                i2=il(i,2)
                i3=il(i,3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,0,0)*ui(i1,i2,
     & i3,c3)+c(i,1,0,0)*ui(i1+1,i2,i3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3,
     & c3)+c(i,3,0,0)*ui(i1+3,i2,i3,c3)+c(i,4,0,0)*ui(i1+4,i2,i3,c3)+
     & c(i,5,0,0)*ui(i1+5,i2,i3,c3)+c(i,6,0,0)*ui(i1+6,i2,i3,c3)+c(i,
     & 7,0,0)*ui(i1+7,i2,i3,c3)+c(i,0,1,0)*ui(i1,i2+1,i3,c3)+c(i,1,1,
     & 0)*ui(i1+1,i2+1,i3,c3)+c(i,2,1,0)*ui(i1+2,i2+1,i3,c3)+c(i,3,1,
     & 0)*ui(i1+3,i2+1,i3,c3)+c(i,4,1,0)*ui(i1+4,i2+1,i3,c3)+c(i,5,1,
     & 0)*ui(i1+5,i2+1,i3,c3)+c(i,6,1,0)*ui(i1+6,i2+1,i3,c3)+c(i,7,1,
     & 0)*ui(i1+7,i2+1,i3,c3)+c(i,0,2,0)*ui(i1,i2+2,i3,c3)+c(i,1,2,0)*
     & ui(i1+1,i2+2,i3,c3)+c(i,2,2,0)*ui(i1+2,i2+2,i3,c3)+c(i,3,2,0)*
     & ui(i1+3,i2+2,i3,c3)+c(i,4,2,0)*ui(i1+4,i2+2,i3,c3)+c(i,5,2,0)*
     & ui(i1+5,i2+2,i3,c3)+c(i,6,2,0)*ui(i1+6,i2+2,i3,c3)+c(i,7,2,0)*
     & ui(i1+7,i2+2,i3,c3)+c(i,0,3,0)*ui(i1,i2+3,i3,c3)+c(i,1,3,0)*ui(
     & i1+1,i2+3,i3,c3)+c(i,2,3,0)*ui(i1+2,i2+3,i3,c3)+c(i,3,3,0)*ui(
     & i1+3,i2+3,i3,c3)+c(i,4,3,0)*ui(i1+4,i2+3,i3,c3)+c(i,5,3,0)*ui(
     & i1+5,i2+3,i3,c3)+c(i,6,3,0)*ui(i1+6,i2+3,i3,c3)+c(i,7,3,0)*ui(
     & i1+7,i2+3,i3,c3)+c(i,0,4,0)*ui(i1,i2+4,i3,c3)+c(i,1,4,0)*ui(i1+
     & 1,i2+4,i3,c3)+c(i,2,4,0)*ui(i1+2,i2+4,i3,c3)+c(i,3,4,0)*ui(i1+
     & 3,i2+4,i3,c3)+c(i,4,4,0)*ui(i1+4,i2+4,i3,c3)+c(i,5,4,0)*ui(i1+
     & 5,i2+4,i3,c3)+c(i,6,4,0)*ui(i1+6,i2+4,i3,c3)+c(i,7,4,0)*ui(i1+
     & 7,i2+4,i3,c3)+c(i,0,5,0)*ui(i1,i2+5,i3,c3)+c(i,1,5,0)*ui(i1+1,
     & i2+5,i3,c3)+c(i,2,5,0)*ui(i1+2,i2+5,i3,c3)+c(i,3,5,0)*ui(i1+3,
     & i2+5,i3,c3)+c(i,4,5,0)*ui(i1+4,i2+5,i3,c3)+c(i,5,5,0)*ui(i1+5,
     & i2+5,i3,c3)+c(i,6,5,0)*ui(i1+6,i2+5,i3,c3)+c(i,7,5,0)*ui(i1+7,
     & i2+5,i3,c3)+c(i,0,6,0)*ui(i1,i2+6,i3,c3)+c(i,1,6,0)*ui(i1+1,i2+
     & 6,i3,c3)+c(i,2,6,0)*ui(i1+2,i2+6,i3,c3)+c(i,3,6,0)*ui(i1+3,i2+
     & 6,i3,c3)+c(i,4,6,0)*ui(i1+4,i2+6,i3,c3)+c(i,5,6,0)*ui(i1+5,i2+
     & 6,i3,c3)+c(i,6,6,0)*ui(i1+6,i2+6,i3,c3)+c(i,7,6,0)*ui(i1+7,i2+
     & 6,i3,c3)+c(i,0,7,0)*ui(i1,i2+7,i3,c3)+c(i,1,7,0)*ui(i1+1,i2+7,
     & i3,c3)+c(i,2,7,0)*ui(i1+2,i2+7,i3,c3)+c(i,3,7,0)*ui(i1+3,i2+7,
     & i3,c3)+c(i,4,7,0)*ui(i1+4,i2+7,i3,c3)+c(i,5,7,0)*ui(i1+5,i2+7,
     & i3,c3)+c(i,6,7,0)*ui(i1+6,i2+7,i3,c3)+c(i,7,7,0)*ui(i1+7,i2+7,
     & i3,c3)+c(i,0,0,1)*ui(i1,i2,i3+1,c3)+c(i,1,0,1)*ui(i1+1,i2,i3+1,
     & c3)+c(i,2,0,1)*ui(i1+2,i2,i3+1,c3)+c(i,3,0,1)*ui(i1+3,i2,i3+1,
     & c3)+c(i,4,0,1)*ui(i1+4,i2,i3+1,c3)+c(i,5,0,1)*ui(i1+5,i2,i3+1,
     & c3)+c(i,6,0,1)*ui(i1+6,i2,i3+1,c3)+c(i,7,0,1)*ui(i1+7,i2,i3+1,
     & c3)+c(i,0,1,1)*ui(i1,i2+1,i3+1,c3)+c(i,1,1,1)*ui(i1+1,i2+1,i3+
     & 1,c3)+c(i,2,1,1)*ui(i1+2,i2+1,i3+1,c3)+c(i,3,1,1)*ui(i1+3,i2+1,
     & i3+1,c3)+c(i,4,1,1)*ui(i1+4,i2+1,i3+1,c3)+c(i,5,1,1)*ui(i1+5,
     & i2+1,i3+1,c3)+c(i,6,1,1)*ui(i1+6,i2+1,i3+1,c3)+c(i,7,1,1)*ui(
     & i1+7,i2+1,i3+1,c3)+c(i,0,2,1)*ui(i1,i2+2,i3+1,c3)+c(i,1,2,1)*
     & ui(i1+1,i2+2,i3+1,c3)+c(i,2,2,1)*ui(i1+2,i2+2,i3+1,c3)+c(i,3,2,
     & 1)*ui(i1+3,i2+2,i3+1,c3)+c(i,4,2,1)*ui(i1+4,i2+2,i3+1,c3)+c(i,
     & 5,2,1)*ui(i1+5,i2+2,i3+1,c3)+c(i,6,2,1)*ui(i1+6,i2+2,i3+1,c3)+
     & c(i,7,2,1)*ui(i1+7,i2+2,i3+1,c3)+c(i,0,3,1)*ui(i1,i2+3,i3+1,c3)
     & +c(i,1,3,1)*ui(i1+1,i2+3,i3+1,c3)+c(i,2,3,1)*ui(i1+2,i2+3,i3+1,
     & c3)+c(i,3,3,1)*ui(i1+3,i2+3,i3+1,c3)+c(i,4,3,1)*ui(i1+4,i2+3,
     & i3+1,c3)+c(i,5,3,1)*ui(i1+5,i2+3,i3+1,c3)+c(i,6,3,1)*ui(i1+6,
     & i2+3,i3+1,c3)+c(i,7,3,1)*ui(i1+7,i2+3,i3+1,c3)+c(i,0,4,1)*ui(
     & i1,i2+4,i3+1,c3)+c(i,1,4,1)*ui(i1+1,i2+4,i3+1,c3)+c(i,2,4,1)*
     & ui(i1+2,i2+4,i3+1,c3)+c(i,3,4,1)*ui(i1+3,i2+4,i3+1,c3)+c(i,4,4,
     & 1)*ui(i1+4,i2+4,i3+1,c3)+c(i,5,4,1)*ui(i1+5,i2+4,i3+1,c3)+c(i,
     & 6,4,1)*ui(i1+6,i2+4,i3+1,c3)+c(i,7,4,1)*ui(i1+7,i2+4,i3+1,c3)+
     & c(i,0,5,1)*ui(i1,i2+5,i3+1,c3)+c(i,1,5,1)*ui(i1+1,i2+5,i3+1,c3)
     & +c(i,2,5,1)*ui(i1+2,i2+5,i3+1,c3)+c(i,3,5,1)*ui(i1+3,i2+5,i3+1,
     & c3)+c(i,4,5,1)*ui(i1+4,i2+5,i3+1,c3)+c(i,5,5,1)*ui(i1+5,i2+5,
     & i3+1,c3)+c(i,6,5,1)*ui(i1+6,i2+5,i3+1,c3)+c(i,7,5,1)*ui(i1+7,
     & i2+5,i3+1,c3)+c(i,0,6,1)*ui(i1,i2+6,i3+1,c3)+c(i,1,6,1)*ui(i1+
     & 1,i2+6,i3+1,c3)+c(i,2,6,1)*ui(i1+2,i2+6,i3+1,c3)+c(i,3,6,1)*ui(
     & i1+3,i2+6,i3+1,c3)+c(i,4,6,1)*ui(i1+4,i2+6,i3+1,c3)+c(i,5,6,1)*
     & ui(i1+5,i2+6,i3+1,c3)+c(i,6,6,1)*ui(i1+6,i2+6,i3+1,c3)+c(i,7,6,
     & 1)*ui(i1+7,i2+6,i3+1,c3)+c(i,0,7,1)*ui(i1,i2+7,i3+1,c3)+c(i,1,
     & 7,1)*ui(i1+1,i2+7,i3+1,c3)+c(i,2,7,1)*ui(i1+2,i2+7,i3+1,c3)+c(
     & i,3,7,1)*ui(i1+3,i2+7,i3+1,c3)+c(i,4,7,1)*ui(i1+4,i2+7,i3+1,c3)
     & +c(i,5,7,1)*ui(i1+5,i2+7,i3+1,c3)+c(i,6,7,1)*ui(i1+6,i2+7,i3+1,
     & c3)+c(i,7,7,1)*ui(i1+7,i2+7,i3+1,c3)+c(i,0,0,2)*ui(i1,i2,i3+2,
     & c3)+c(i,1,0,2)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,2)*ui(i1+2,i2,i3+2,
     & c3)+c(i,3,0,2)*ui(i1+3,i2,i3+2,c3)+c(i,4,0,2)*ui(i1+4,i2,i3+2,
     & c3)+c(i,5,0,2)*ui(i1+5,i2,i3+2,c3)+c(i,6,0,2)*ui(i1+6,i2,i3+2,
     & c3)+c(i,7,0,2)*ui(i1+7,i2,i3+2,c3)+c(i,0,1,2)*ui(i1,i2+1,i3+2,
     & c3)+c(i,1,1,2)*ui(i1+1,i2+1,i3+2,c3)+c(i,2,1,2)*ui(i1+2,i2+1,
     & i3+2,c3)+c(i,3,1,2)*ui(i1+3,i2+1,i3+2,c3)+c(i,4,1,2)*ui(i1+4,
     & i2+1,i3+2,c3)+c(i,5,1,2)*ui(i1+5,i2+1,i3+2,c3)+c(i,6,1,2)*ui(
     & i1+6,i2+1,i3+2,c3)+c(i,7,1,2)*ui(i1+7,i2+1,i3+2,c3)+c(i,0,2,2)*
     & ui(i1,i2+2,i3+2,c3)+c(i,1,2,2)*ui(i1+1,i2+2,i3+2,c3)+c(i,2,2,2)
     & *ui(i1+2,i2+2,i3+2,c3)+c(i,3,2,2)*ui(i1+3,i2+2,i3+2,c3)+c(i,4,
     & 2,2)*ui(i1+4,i2+2,i3+2,c3)+c(i,5,2,2)*ui(i1+5,i2+2,i3+2,c3)+c(
     & i,6,2,2)*ui(i1+6,i2+2,i3+2,c3)+c(i,7,2,2)*ui(i1+7,i2+2,i3+2,c3)
     & +c(i,0,3,2)*ui(i1,i2+3,i3+2,c3)+c(i,1,3,2)*ui(i1+1,i2+3,i3+2,
     & c3)+c(i,2,3,2)*ui(i1+2,i2+3,i3+2,c3)+c(i,3,3,2)*ui(i1+3,i2+3,
     & i3+2,c3)+c(i,4,3,2)*ui(i1+4,i2+3,i3+2,c3)+c(i,5,3,2)*ui(i1+5,
     & i2+3,i3+2,c3)+c(i,6,3,2)*ui(i1+6,i2+3,i3+2,c3)+c(i,7,3,2)*ui(
     & i1+7,i2+3,i3+2,c3)+c(i,0,4,2)*ui(i1,i2+4,i3+2,c3)+c(i,1,4,2)*
     & ui(i1+1,i2+4,i3+2,c3)+c(i,2,4,2)*ui(i1+2,i2+4,i3+2,c3)+c(i,3,4,
     & 2)*ui(i1+3,i2+4,i3+2,c3)+c(i,4,4,2)*ui(i1+4,i2+4,i3+2,c3)+c(i,
     & 5,4,2)*ui(i1+5,i2+4,i3+2,c3)+c(i,6,4,2)*ui(i1+6,i2+4,i3+2,c3)+
     & c(i,7,4,2)*ui(i1+7,i2+4,i3+2,c3)+c(i,0,5,2)*ui(i1,i2+5,i3+2,c3)
     & +c(i,1,5,2)*ui(i1+1,i2+5,i3+2,c3)+c(i,2,5,2)*ui(i1+2,i2+5,i3+2,
     & c3)+c(i,3,5,2)*ui(i1+3,i2+5,i3+2,c3)+c(i,4,5,2)*ui(i1+4,i2+5,
     & i3+2,c3)+c(i,5,5,2)*ui(i1+5,i2+5,i3+2,c3)+c(i,6,5,2)*ui(i1+6,
     & i2+5,i3+2,c3)+c(i,7,5,2)*ui(i1+7,i2+5,i3+2,c3)+c(i,0,6,2)*ui(
     & i1,i2+6,i3+2,c3)+c(i,1,6,2)*ui(i1+1,i2+6,i3+2,c3)+c(i,2,6,2)*
     & ui(i1+2,i2+6,i3+2,c3)+c(i,3,6,2)*ui(i1+3,i2+6,i3+2,c3)+c(i,4,6,
     & 2)*ui(i1+4,i2+6,i3+2,c3)+c(i,5,6,2)*ui(i1+5,i2+6,i3+2,c3)+c(i,
     & 6,6,2)*ui(i1+6,i2+6,i3+2,c3)+c(i,7,6,2)*ui(i1+7,i2+6,i3+2,c3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,0,7,2)*ui(i1,i2+7,i3+2,c3)+c(i,1,7,2)*ui(i1+1,i2+
     & 7,i3+2,c3)+c(i,2,7,2)*ui(i1+2,i2+7,i3+2,c3)+c(i,3,7,2)*ui(i1+3,
     & i2+7,i3+2,c3)+c(i,4,7,2)*ui(i1+4,i2+7,i3+2,c3)+c(i,5,7,2)*ui(
     & i1+5,i2+7,i3+2,c3)+c(i,6,7,2)*ui(i1+6,i2+7,i3+2,c3)+c(i,7,7,2)*
     & ui(i1+7,i2+7,i3+2,c3)+c(i,0,0,3)*ui(i1,i2,i3+3,c3)+c(i,1,0,3)*
     & ui(i1+1,i2,i3+3,c3)+c(i,2,0,3)*ui(i1+2,i2,i3+3,c3)+c(i,3,0,3)*
     & ui(i1+3,i2,i3+3,c3)+c(i,4,0,3)*ui(i1+4,i2,i3+3,c3)+c(i,5,0,3)*
     & ui(i1+5,i2,i3+3,c3)+c(i,6,0,3)*ui(i1+6,i2,i3+3,c3)+c(i,7,0,3)*
     & ui(i1+7,i2,i3+3,c3)+c(i,0,1,3)*ui(i1,i2+1,i3+3,c3)+c(i,1,1,3)*
     & ui(i1+1,i2+1,i3+3,c3)+c(i,2,1,3)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,1,
     & 3)*ui(i1+3,i2+1,i3+3,c3)+c(i,4,1,3)*ui(i1+4,i2+1,i3+3,c3)+c(i,
     & 5,1,3)*ui(i1+5,i2+1,i3+3,c3)+c(i,6,1,3)*ui(i1+6,i2+1,i3+3,c3)+
     & c(i,7,1,3)*ui(i1+7,i2+1,i3+3,c3)+c(i,0,2,3)*ui(i1,i2+2,i3+3,c3)
     & +c(i,1,2,3)*ui(i1+1,i2+2,i3+3,c3)+c(i,2,2,3)*ui(i1+2,i2+2,i3+3,
     & c3)+c(i,3,2,3)*ui(i1+3,i2+2,i3+3,c3)+c(i,4,2,3)*ui(i1+4,i2+2,
     & i3+3,c3)+c(i,5,2,3)*ui(i1+5,i2+2,i3+3,c3)+c(i,6,2,3)*ui(i1+6,
     & i2+2,i3+3,c3)+c(i,7,2,3)*ui(i1+7,i2+2,i3+3,c3)+c(i,0,3,3)*ui(
     & i1,i2+3,i3+3,c3)+c(i,1,3,3)*ui(i1+1,i2+3,i3+3,c3)+c(i,2,3,3)*
     & ui(i1+2,i2+3,i3+3,c3)+c(i,3,3,3)*ui(i1+3,i2+3,i3+3,c3)+c(i,4,3,
     & 3)*ui(i1+4,i2+3,i3+3,c3)+c(i,5,3,3)*ui(i1+5,i2+3,i3+3,c3)+c(i,
     & 6,3,3)*ui(i1+6,i2+3,i3+3,c3)+c(i,7,3,3)*ui(i1+7,i2+3,i3+3,c3)+
     & c(i,0,4,3)*ui(i1,i2+4,i3+3,c3)+c(i,1,4,3)*ui(i1+1,i2+4,i3+3,c3)
     & +c(i,2,4,3)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,4,3)*ui(i1+3,i2+4,i3+3,
     & c3)+c(i,4,4,3)*ui(i1+4,i2+4,i3+3,c3)+c(i,5,4,3)*ui(i1+5,i2+4,
     & i3+3,c3)+c(i,6,4,3)*ui(i1+6,i2+4,i3+3,c3)+c(i,7,4,3)*ui(i1+7,
     & i2+4,i3+3,c3)+c(i,0,5,3)*ui(i1,i2+5,i3+3,c3)+c(i,1,5,3)*ui(i1+
     & 1,i2+5,i3+3,c3)+c(i,2,5,3)*ui(i1+2,i2+5,i3+3,c3)+c(i,3,5,3)*ui(
     & i1+3,i2+5,i3+3,c3)+c(i,4,5,3)*ui(i1+4,i2+5,i3+3,c3)+c(i,5,5,3)*
     & ui(i1+5,i2+5,i3+3,c3)+c(i,6,5,3)*ui(i1+6,i2+5,i3+3,c3)+c(i,7,5,
     & 3)*ui(i1+7,i2+5,i3+3,c3)+c(i,0,6,3)*ui(i1,i2+6,i3+3,c3)+c(i,1,
     & 6,3)*ui(i1+1,i2+6,i3+3,c3)+c(i,2,6,3)*ui(i1+2,i2+6,i3+3,c3)+c(
     & i,3,6,3)*ui(i1+3,i2+6,i3+3,c3)+c(i,4,6,3)*ui(i1+4,i2+6,i3+3,c3)
     & +c(i,5,6,3)*ui(i1+5,i2+6,i3+3,c3)+c(i,6,6,3)*ui(i1+6,i2+6,i3+3,
     & c3)+c(i,7,6,3)*ui(i1+7,i2+6,i3+3,c3)+c(i,0,7,3)*ui(i1,i2+7,i3+
     & 3,c3)+c(i,1,7,3)*ui(i1+1,i2+7,i3+3,c3)+c(i,2,7,3)*ui(i1+2,i2+7,
     & i3+3,c3)+c(i,3,7,3)*ui(i1+3,i2+7,i3+3,c3)+c(i,4,7,3)*ui(i1+4,
     & i2+7,i3+3,c3)+c(i,5,7,3)*ui(i1+5,i2+7,i3+3,c3)+c(i,6,7,3)*ui(
     & i1+6,i2+7,i3+3,c3)+c(i,7,7,3)*ui(i1+7,i2+7,i3+3,c3)+c(i,0,0,4)*
     & ui(i1,i2,i3+4,c3)+c(i,1,0,4)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,4)*ui(
     & i1+2,i2,i3+4,c3)+c(i,3,0,4)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,4)*ui(
     & i1+4,i2,i3+4,c3)+c(i,5,0,4)*ui(i1+5,i2,i3+4,c3)+c(i,6,0,4)*ui(
     & i1+6,i2,i3+4,c3)+c(i,7,0,4)*ui(i1+7,i2,i3+4,c3)+c(i,0,1,4)*ui(
     & i1,i2+1,i3+4,c3)+c(i,1,1,4)*ui(i1+1,i2+1,i3+4,c3)+c(i,2,1,4)*
     & ui(i1+2,i2+1,i3+4,c3)+c(i,3,1,4)*ui(i1+3,i2+1,i3+4,c3)+c(i,4,1,
     & 4)*ui(i1+4,i2+1,i3+4,c3)+c(i,5,1,4)*ui(i1+5,i2+1,i3+4,c3)+c(i,
     & 6,1,4)*ui(i1+6,i2+1,i3+4,c3)+c(i,7,1,4)*ui(i1+7,i2+1,i3+4,c3)+
     & c(i,0,2,4)*ui(i1,i2+2,i3+4,c3)+c(i,1,2,4)*ui(i1+1,i2+2,i3+4,c3)
     & +c(i,2,2,4)*ui(i1+2,i2+2,i3+4,c3)+c(i,3,2,4)*ui(i1+3,i2+2,i3+4,
     & c3)+c(i,4,2,4)*ui(i1+4,i2+2,i3+4,c3)+c(i,5,2,4)*ui(i1+5,i2+2,
     & i3+4,c3)+c(i,6,2,4)*ui(i1+6,i2+2,i3+4,c3)+c(i,7,2,4)*ui(i1+7,
     & i2+2,i3+4,c3)+c(i,0,3,4)*ui(i1,i2+3,i3+4,c3)+c(i,1,3,4)*ui(i1+
     & 1,i2+3,i3+4,c3)+c(i,2,3,4)*ui(i1+2,i2+3,i3+4,c3)+c(i,3,3,4)*ui(
     & i1+3,i2+3,i3+4,c3)+c(i,4,3,4)*ui(i1+4,i2+3,i3+4,c3)+c(i,5,3,4)*
     & ui(i1+5,i2+3,i3+4,c3)+c(i,6,3,4)*ui(i1+6,i2+3,i3+4,c3)+c(i,7,3,
     & 4)*ui(i1+7,i2+3,i3+4,c3)+c(i,0,4,4)*ui(i1,i2+4,i3+4,c3)+c(i,1,
     & 4,4)*ui(i1+1,i2+4,i3+4,c3)+c(i,2,4,4)*ui(i1+2,i2+4,i3+4,c3)+c(
     & i,3,4,4)*ui(i1+3,i2+4,i3+4,c3)+c(i,4,4,4)*ui(i1+4,i2+4,i3+4,c3)
     & +c(i,5,4,4)*ui(i1+5,i2+4,i3+4,c3)+c(i,6,4,4)*ui(i1+6,i2+4,i3+4,
     & c3)+c(i,7,4,4)*ui(i1+7,i2+4,i3+4,c3)+c(i,0,5,4)*ui(i1,i2+5,i3+
     & 4,c3)+c(i,1,5,4)*ui(i1+1,i2+5,i3+4,c3)+c(i,2,5,4)*ui(i1+2,i2+5,
     & i3+4,c3)+c(i,3,5,4)*ui(i1+3,i2+5,i3+4,c3)+c(i,4,5,4)*ui(i1+4,
     & i2+5,i3+4,c3)+c(i,5,5,4)*ui(i1+5,i2+5,i3+4,c3)+c(i,6,5,4)*ui(
     & i1+6,i2+5,i3+4,c3)+c(i,7,5,4)*ui(i1+7,i2+5,i3+4,c3)+c(i,0,6,4)*
     & ui(i1,i2+6,i3+4,c3)+c(i,1,6,4)*ui(i1+1,i2+6,i3+4,c3)+c(i,2,6,4)
     & *ui(i1+2,i2+6,i3+4,c3)+c(i,3,6,4)*ui(i1+3,i2+6,i3+4,c3)+c(i,4,
     & 6,4)*ui(i1+4,i2+6,i3+4,c3)+c(i,5,6,4)*ui(i1+5,i2+6,i3+4,c3)+c(
     & i,6,6,4)*ui(i1+6,i2+6,i3+4,c3)+c(i,7,6,4)*ui(i1+7,i2+6,i3+4,c3)
     & +c(i,0,7,4)*ui(i1,i2+7,i3+4,c3)+c(i,1,7,4)*ui(i1+1,i2+7,i3+4,
     & c3)+c(i,2,7,4)*ui(i1+2,i2+7,i3+4,c3)+c(i,3,7,4)*ui(i1+3,i2+7,
     & i3+4,c3)+c(i,4,7,4)*ui(i1+4,i2+7,i3+4,c3)+c(i,5,7,4)*ui(i1+5,
     & i2+7,i3+4,c3)+c(i,6,7,4)*ui(i1+6,i2+7,i3+4,c3)+c(i,7,7,4)*ui(
     & i1+7,i2+7,i3+4,c3)+c(i,0,0,5)*ui(i1,i2,i3+5,c3)+c(i,1,0,5)*ui(
     & i1+1,i2,i3+5,c3)+c(i,2,0,5)*ui(i1+2,i2,i3+5,c3)+c(i,3,0,5)*ui(
     & i1+3,i2,i3+5,c3)+c(i,4,0,5)*ui(i1+4,i2,i3+5,c3)+c(i,5,0,5)*ui(
     & i1+5,i2,i3+5,c3)+c(i,6,0,5)*ui(i1+6,i2,i3+5,c3)+c(i,7,0,5)*ui(
     & i1+7,i2,i3+5,c3)+c(i,0,1,5)*ui(i1,i2+1,i3+5,c3)+c(i,1,1,5)*ui(
     & i1+1,i2+1,i3+5,c3)+c(i,2,1,5)*ui(i1+2,i2+1,i3+5,c3)+c(i,3,1,5)*
     & ui(i1+3,i2+1,i3+5,c3)+c(i,4,1,5)*ui(i1+4,i2+1,i3+5,c3)+c(i,5,1,
     & 5)*ui(i1+5,i2+1,i3+5,c3)+c(i,6,1,5)*ui(i1+6,i2+1,i3+5,c3)+c(i,
     & 7,1,5)*ui(i1+7,i2+1,i3+5,c3)+c(i,0,2,5)*ui(i1,i2+2,i3+5,c3)+c(
     & i,1,2,5)*ui(i1+1,i2+2,i3+5,c3)+c(i,2,2,5)*ui(i1+2,i2+2,i3+5,c3)
     & +c(i,3,2,5)*ui(i1+3,i2+2,i3+5,c3)+c(i,4,2,5)*ui(i1+4,i2+2,i3+5,
     & c3)+c(i,5,2,5)*ui(i1+5,i2+2,i3+5,c3)+c(i,6,2,5)*ui(i1+6,i2+2,
     & i3+5,c3)+c(i,7,2,5)*ui(i1+7,i2+2,i3+5,c3)+c(i,0,3,5)*ui(i1,i2+
     & 3,i3+5,c3)+c(i,1,3,5)*ui(i1+1,i2+3,i3+5,c3)+c(i,2,3,5)*ui(i1+2,
     & i2+3,i3+5,c3)+c(i,3,3,5)*ui(i1+3,i2+3,i3+5,c3)+c(i,4,3,5)*ui(
     & i1+4,i2+3,i3+5,c3)+c(i,5,3,5)*ui(i1+5,i2+3,i3+5,c3)+c(i,6,3,5)*
     & ui(i1+6,i2+3,i3+5,c3)+c(i,7,3,5)*ui(i1+7,i2+3,i3+5,c3)+c(i,0,4,
     & 5)*ui(i1,i2+4,i3+5,c3)+c(i,1,4,5)*ui(i1+1,i2+4,i3+5,c3)+c(i,2,
     & 4,5)*ui(i1+2,i2+4,i3+5,c3)+c(i,3,4,5)*ui(i1+3,i2+4,i3+5,c3)+c(
     & i,4,4,5)*ui(i1+4,i2+4,i3+5,c3)+c(i,5,4,5)*ui(i1+5,i2+4,i3+5,c3)
     & +c(i,6,4,5)*ui(i1+6,i2+4,i3+5,c3)+c(i,7,4,5)*ui(i1+7,i2+4,i3+5,
     & c3)+c(i,0,5,5)*ui(i1,i2+5,i3+5,c3)+c(i,1,5,5)*ui(i1+1,i2+5,i3+
     & 5,c3)+c(i,2,5,5)*ui(i1+2,i2+5,i3+5,c3)+c(i,3,5,5)*ui(i1+3,i2+5,
     & i3+5,c3)+c(i,4,5,5)*ui(i1+4,i2+5,i3+5,c3)+c(i,5,5,5)*ui(i1+5,
     & i2+5,i3+5,c3)+c(i,6,5,5)*ui(i1+6,i2+5,i3+5,c3)+c(i,7,5,5)*ui(
     & i1+7,i2+5,i3+5,c3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,0,6,5)*ui(i1,i2+6,i3+5,c3)+c(i,1,6,5)*ui(i1+1,i2+
     & 6,i3+5,c3)+c(i,2,6,5)*ui(i1+2,i2+6,i3+5,c3)+c(i,3,6,5)*ui(i1+3,
     & i2+6,i3+5,c3)+c(i,4,6,5)*ui(i1+4,i2+6,i3+5,c3)+c(i,5,6,5)*ui(
     & i1+5,i2+6,i3+5,c3)+c(i,6,6,5)*ui(i1+6,i2+6,i3+5,c3)+c(i,7,6,5)*
     & ui(i1+7,i2+6,i3+5,c3)+c(i,0,7,5)*ui(i1,i2+7,i3+5,c3)+c(i,1,7,5)
     & *ui(i1+1,i2+7,i3+5,c3)+c(i,2,7,5)*ui(i1+2,i2+7,i3+5,c3)+c(i,3,
     & 7,5)*ui(i1+3,i2+7,i3+5,c3)+c(i,4,7,5)*ui(i1+4,i2+7,i3+5,c3)+c(
     & i,5,7,5)*ui(i1+5,i2+7,i3+5,c3)+c(i,6,7,5)*ui(i1+6,i2+7,i3+5,c3)
     & +c(i,7,7,5)*ui(i1+7,i2+7,i3+5,c3)+c(i,0,0,6)*ui(i1,i2,i3+6,c3)+
     & c(i,1,0,6)*ui(i1+1,i2,i3+6,c3)+c(i,2,0,6)*ui(i1+2,i2,i3+6,c3)+
     & c(i,3,0,6)*ui(i1+3,i2,i3+6,c3)+c(i,4,0,6)*ui(i1+4,i2,i3+6,c3)+
     & c(i,5,0,6)*ui(i1+5,i2,i3+6,c3)+c(i,6,0,6)*ui(i1+6,i2,i3+6,c3)+
     & c(i,7,0,6)*ui(i1+7,i2,i3+6,c3)+c(i,0,1,6)*ui(i1,i2+1,i3+6,c3)+
     & c(i,1,1,6)*ui(i1+1,i2+1,i3+6,c3)+c(i,2,1,6)*ui(i1+2,i2+1,i3+6,
     & c3)+c(i,3,1,6)*ui(i1+3,i2+1,i3+6,c3)+c(i,4,1,6)*ui(i1+4,i2+1,
     & i3+6,c3)+c(i,5,1,6)*ui(i1+5,i2+1,i3+6,c3)+c(i,6,1,6)*ui(i1+6,
     & i2+1,i3+6,c3)+c(i,7,1,6)*ui(i1+7,i2+1,i3+6,c3)+c(i,0,2,6)*ui(
     & i1,i2+2,i3+6,c3)+c(i,1,2,6)*ui(i1+1,i2+2,i3+6,c3)+c(i,2,2,6)*
     & ui(i1+2,i2+2,i3+6,c3)+c(i,3,2,6)*ui(i1+3,i2+2,i3+6,c3)+c(i,4,2,
     & 6)*ui(i1+4,i2+2,i3+6,c3)+c(i,5,2,6)*ui(i1+5,i2+2,i3+6,c3)+c(i,
     & 6,2,6)*ui(i1+6,i2+2,i3+6,c3)+c(i,7,2,6)*ui(i1+7,i2+2,i3+6,c3)+
     & c(i,0,3,6)*ui(i1,i2+3,i3+6,c3)+c(i,1,3,6)*ui(i1+1,i2+3,i3+6,c3)
     & +c(i,2,3,6)*ui(i1+2,i2+3,i3+6,c3)+c(i,3,3,6)*ui(i1+3,i2+3,i3+6,
     & c3)+c(i,4,3,6)*ui(i1+4,i2+3,i3+6,c3)+c(i,5,3,6)*ui(i1+5,i2+3,
     & i3+6,c3)+c(i,6,3,6)*ui(i1+6,i2+3,i3+6,c3)+c(i,7,3,6)*ui(i1+7,
     & i2+3,i3+6,c3)+c(i,0,4,6)*ui(i1,i2+4,i3+6,c3)+c(i,1,4,6)*ui(i1+
     & 1,i2+4,i3+6,c3)+c(i,2,4,6)*ui(i1+2,i2+4,i3+6,c3)+c(i,3,4,6)*ui(
     & i1+3,i2+4,i3+6,c3)+c(i,4,4,6)*ui(i1+4,i2+4,i3+6,c3)+c(i,5,4,6)*
     & ui(i1+5,i2+4,i3+6,c3)+c(i,6,4,6)*ui(i1+6,i2+4,i3+6,c3)+c(i,7,4,
     & 6)*ui(i1+7,i2+4,i3+6,c3)+c(i,0,5,6)*ui(i1,i2+5,i3+6,c3)+c(i,1,
     & 5,6)*ui(i1+1,i2+5,i3+6,c3)+c(i,2,5,6)*ui(i1+2,i2+5,i3+6,c3)+c(
     & i,3,5,6)*ui(i1+3,i2+5,i3+6,c3)+c(i,4,5,6)*ui(i1+4,i2+5,i3+6,c3)
     & +c(i,5,5,6)*ui(i1+5,i2+5,i3+6,c3)+c(i,6,5,6)*ui(i1+6,i2+5,i3+6,
     & c3)+c(i,7,5,6)*ui(i1+7,i2+5,i3+6,c3)+c(i,0,6,6)*ui(i1,i2+6,i3+
     & 6,c3)+c(i,1,6,6)*ui(i1+1,i2+6,i3+6,c3)+c(i,2,6,6)*ui(i1+2,i2+6,
     & i3+6,c3)+c(i,3,6,6)*ui(i1+3,i2+6,i3+6,c3)+c(i,4,6,6)*ui(i1+4,
     & i2+6,i3+6,c3)+c(i,5,6,6)*ui(i1+5,i2+6,i3+6,c3)+c(i,6,6,6)*ui(
     & i1+6,i2+6,i3+6,c3)+c(i,7,6,6)*ui(i1+7,i2+6,i3+6,c3)+c(i,0,7,6)*
     & ui(i1,i2+7,i3+6,c3)+c(i,1,7,6)*ui(i1+1,i2+7,i3+6,c3)+c(i,2,7,6)
     & *ui(i1+2,i2+7,i3+6,c3)+c(i,3,7,6)*ui(i1+3,i2+7,i3+6,c3)+c(i,4,
     & 7,6)*ui(i1+4,i2+7,i3+6,c3)+c(i,5,7,6)*ui(i1+5,i2+7,i3+6,c3)+c(
     & i,6,7,6)*ui(i1+6,i2+7,i3+6,c3)+c(i,7,7,6)*ui(i1+7,i2+7,i3+6,c3)
     & +c(i,0,0,7)*ui(i1,i2,i3+7,c3)+c(i,1,0,7)*ui(i1+1,i2,i3+7,c3)+c(
     & i,2,0,7)*ui(i1+2,i2,i3+7,c3)+c(i,3,0,7)*ui(i1+3,i2,i3+7,c3)+c(
     & i,4,0,7)*ui(i1+4,i2,i3+7,c3)+c(i,5,0,7)*ui(i1+5,i2,i3+7,c3)+c(
     & i,6,0,7)*ui(i1+6,i2,i3+7,c3)+c(i,7,0,7)*ui(i1+7,i2,i3+7,c3)+c(
     & i,0,1,7)*ui(i1,i2+1,i3+7,c3)+c(i,1,1,7)*ui(i1+1,i2+1,i3+7,c3)+
     & c(i,2,1,7)*ui(i1+2,i2+1,i3+7,c3)+c(i,3,1,7)*ui(i1+3,i2+1,i3+7,
     & c3)+c(i,4,1,7)*ui(i1+4,i2+1,i3+7,c3)+c(i,5,1,7)*ui(i1+5,i2+1,
     & i3+7,c3)+c(i,6,1,7)*ui(i1+6,i2+1,i3+7,c3)+c(i,7,1,7)*ui(i1+7,
     & i2+1,i3+7,c3)+c(i,0,2,7)*ui(i1,i2+2,i3+7,c3)+c(i,1,2,7)*ui(i1+
     & 1,i2+2,i3+7,c3)+c(i,2,2,7)*ui(i1+2,i2+2,i3+7,c3)+c(i,3,2,7)*ui(
     & i1+3,i2+2,i3+7,c3)+c(i,4,2,7)*ui(i1+4,i2+2,i3+7,c3)+c(i,5,2,7)*
     & ui(i1+5,i2+2,i3+7,c3)+c(i,6,2,7)*ui(i1+6,i2+2,i3+7,c3)+c(i,7,2,
     & 7)*ui(i1+7,i2+2,i3+7,c3)+c(i,0,3,7)*ui(i1,i2+3,i3+7,c3)+c(i,1,
     & 3,7)*ui(i1+1,i2+3,i3+7,c3)+c(i,2,3,7)*ui(i1+2,i2+3,i3+7,c3)+c(
     & i,3,3,7)*ui(i1+3,i2+3,i3+7,c3)+c(i,4,3,7)*ui(i1+4,i2+3,i3+7,c3)
     & +c(i,5,3,7)*ui(i1+5,i2+3,i3+7,c3)+c(i,6,3,7)*ui(i1+6,i2+3,i3+7,
     & c3)+c(i,7,3,7)*ui(i1+7,i2+3,i3+7,c3)+c(i,0,4,7)*ui(i1,i2+4,i3+
     & 7,c3)+c(i,1,4,7)*ui(i1+1,i2+4,i3+7,c3)+c(i,2,4,7)*ui(i1+2,i2+4,
     & i3+7,c3)+c(i,3,4,7)*ui(i1+3,i2+4,i3+7,c3)+c(i,4,4,7)*ui(i1+4,
     & i2+4,i3+7,c3)+c(i,5,4,7)*ui(i1+5,i2+4,i3+7,c3)+c(i,6,4,7)*ui(
     & i1+6,i2+4,i3+7,c3)+c(i,7,4,7)*ui(i1+7,i2+4,i3+7,c3)+c(i,0,5,7)*
     & ui(i1,i2+5,i3+7,c3)+c(i,1,5,7)*ui(i1+1,i2+5,i3+7,c3)+c(i,2,5,7)
     & *ui(i1+2,i2+5,i3+7,c3)+c(i,3,5,7)*ui(i1+3,i2+5,i3+7,c3)+c(i,4,
     & 5,7)*ui(i1+4,i2+5,i3+7,c3)+c(i,5,5,7)*ui(i1+5,i2+5,i3+7,c3)+c(
     & i,6,5,7)*ui(i1+6,i2+5,i3+7,c3)+c(i,7,5,7)*ui(i1+7,i2+5,i3+7,c3)
     & +c(i,0,6,7)*ui(i1,i2+6,i3+7,c3)+c(i,1,6,7)*ui(i1+1,i2+6,i3+7,
     & c3)+c(i,2,6,7)*ui(i1+2,i2+6,i3+7,c3)+c(i,3,6,7)*ui(i1+3,i2+6,
     & i3+7,c3)+c(i,4,6,7)*ui(i1+4,i2+6,i3+7,c3)+c(i,5,6,7)*ui(i1+5,
     & i2+6,i3+7,c3)+c(i,6,6,7)*ui(i1+6,i2+6,i3+7,c3)+c(i,7,6,7)*ui(
     & i1+7,i2+6,i3+7,c3)+c(i,0,7,7)*ui(i1,i2+7,i3+7,c3)+c(i,1,7,7)*
     & ui(i1+1,i2+7,i3+7,c3)+c(i,2,7,7)*ui(i1+2,i2+7,i3+7,c3)+c(i,3,7,
     & 7)*ui(i1+3,i2+7,i3+7,c3)+c(i,4,7,7)*ui(i1+4,i2+7,i3+7,c3)+c(i,
     & 5,7,7)*ui(i1+5,i2+7,i3+7,c3)+c(i,6,7,7)*ui(i1+6,i2+7,i3+7,c3)+
     & c(i,7,7,7)*ui(i1+7,i2+7,i3+7,c3)


              end do
              end do
            else
              ! put "c" loop as inner loop, this seems to be faster
              do i=nia,nib
              do c3=c3a,c3b
! interp888(ug(ip(i,1),ip(i,2),ip(i,3),c3))
                i1=il(i,1)
                i2=il(i,2)
                i3=il(i,3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,0,0)*ui(i1,i2,
     & i3,c3)+c(i,1,0,0)*ui(i1+1,i2,i3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3,
     & c3)+c(i,3,0,0)*ui(i1+3,i2,i3,c3)+c(i,4,0,0)*ui(i1+4,i2,i3,c3)+
     & c(i,5,0,0)*ui(i1+5,i2,i3,c3)+c(i,6,0,0)*ui(i1+6,i2,i3,c3)+c(i,
     & 7,0,0)*ui(i1+7,i2,i3,c3)+c(i,0,1,0)*ui(i1,i2+1,i3,c3)+c(i,1,1,
     & 0)*ui(i1+1,i2+1,i3,c3)+c(i,2,1,0)*ui(i1+2,i2+1,i3,c3)+c(i,3,1,
     & 0)*ui(i1+3,i2+1,i3,c3)+c(i,4,1,0)*ui(i1+4,i2+1,i3,c3)+c(i,5,1,
     & 0)*ui(i1+5,i2+1,i3,c3)+c(i,6,1,0)*ui(i1+6,i2+1,i3,c3)+c(i,7,1,
     & 0)*ui(i1+7,i2+1,i3,c3)+c(i,0,2,0)*ui(i1,i2+2,i3,c3)+c(i,1,2,0)*
     & ui(i1+1,i2+2,i3,c3)+c(i,2,2,0)*ui(i1+2,i2+2,i3,c3)+c(i,3,2,0)*
     & ui(i1+3,i2+2,i3,c3)+c(i,4,2,0)*ui(i1+4,i2+2,i3,c3)+c(i,5,2,0)*
     & ui(i1+5,i2+2,i3,c3)+c(i,6,2,0)*ui(i1+6,i2+2,i3,c3)+c(i,7,2,0)*
     & ui(i1+7,i2+2,i3,c3)+c(i,0,3,0)*ui(i1,i2+3,i3,c3)+c(i,1,3,0)*ui(
     & i1+1,i2+3,i3,c3)+c(i,2,3,0)*ui(i1+2,i2+3,i3,c3)+c(i,3,3,0)*ui(
     & i1+3,i2+3,i3,c3)+c(i,4,3,0)*ui(i1+4,i2+3,i3,c3)+c(i,5,3,0)*ui(
     & i1+5,i2+3,i3,c3)+c(i,6,3,0)*ui(i1+6,i2+3,i3,c3)+c(i,7,3,0)*ui(
     & i1+7,i2+3,i3,c3)+c(i,0,4,0)*ui(i1,i2+4,i3,c3)+c(i,1,4,0)*ui(i1+
     & 1,i2+4,i3,c3)+c(i,2,4,0)*ui(i1+2,i2+4,i3,c3)+c(i,3,4,0)*ui(i1+
     & 3,i2+4,i3,c3)+c(i,4,4,0)*ui(i1+4,i2+4,i3,c3)+c(i,5,4,0)*ui(i1+
     & 5,i2+4,i3,c3)+c(i,6,4,0)*ui(i1+6,i2+4,i3,c3)+c(i,7,4,0)*ui(i1+
     & 7,i2+4,i3,c3)+c(i,0,5,0)*ui(i1,i2+5,i3,c3)+c(i,1,5,0)*ui(i1+1,
     & i2+5,i3,c3)+c(i,2,5,0)*ui(i1+2,i2+5,i3,c3)+c(i,3,5,0)*ui(i1+3,
     & i2+5,i3,c3)+c(i,4,5,0)*ui(i1+4,i2+5,i3,c3)+c(i,5,5,0)*ui(i1+5,
     & i2+5,i3,c3)+c(i,6,5,0)*ui(i1+6,i2+5,i3,c3)+c(i,7,5,0)*ui(i1+7,
     & i2+5,i3,c3)+c(i,0,6,0)*ui(i1,i2+6,i3,c3)+c(i,1,6,0)*ui(i1+1,i2+
     & 6,i3,c3)+c(i,2,6,0)*ui(i1+2,i2+6,i3,c3)+c(i,3,6,0)*ui(i1+3,i2+
     & 6,i3,c3)+c(i,4,6,0)*ui(i1+4,i2+6,i3,c3)+c(i,5,6,0)*ui(i1+5,i2+
     & 6,i3,c3)+c(i,6,6,0)*ui(i1+6,i2+6,i3,c3)+c(i,7,6,0)*ui(i1+7,i2+
     & 6,i3,c3)+c(i,0,7,0)*ui(i1,i2+7,i3,c3)+c(i,1,7,0)*ui(i1+1,i2+7,
     & i3,c3)+c(i,2,7,0)*ui(i1+2,i2+7,i3,c3)+c(i,3,7,0)*ui(i1+3,i2+7,
     & i3,c3)+c(i,4,7,0)*ui(i1+4,i2+7,i3,c3)+c(i,5,7,0)*ui(i1+5,i2+7,
     & i3,c3)+c(i,6,7,0)*ui(i1+6,i2+7,i3,c3)+c(i,7,7,0)*ui(i1+7,i2+7,
     & i3,c3)+c(i,0,0,1)*ui(i1,i2,i3+1,c3)+c(i,1,0,1)*ui(i1+1,i2,i3+1,
     & c3)+c(i,2,0,1)*ui(i1+2,i2,i3+1,c3)+c(i,3,0,1)*ui(i1+3,i2,i3+1,
     & c3)+c(i,4,0,1)*ui(i1+4,i2,i3+1,c3)+c(i,5,0,1)*ui(i1+5,i2,i3+1,
     & c3)+c(i,6,0,1)*ui(i1+6,i2,i3+1,c3)+c(i,7,0,1)*ui(i1+7,i2,i3+1,
     & c3)+c(i,0,1,1)*ui(i1,i2+1,i3+1,c3)+c(i,1,1,1)*ui(i1+1,i2+1,i3+
     & 1,c3)+c(i,2,1,1)*ui(i1+2,i2+1,i3+1,c3)+c(i,3,1,1)*ui(i1+3,i2+1,
     & i3+1,c3)+c(i,4,1,1)*ui(i1+4,i2+1,i3+1,c3)+c(i,5,1,1)*ui(i1+5,
     & i2+1,i3+1,c3)+c(i,6,1,1)*ui(i1+6,i2+1,i3+1,c3)+c(i,7,1,1)*ui(
     & i1+7,i2+1,i3+1,c3)+c(i,0,2,1)*ui(i1,i2+2,i3+1,c3)+c(i,1,2,1)*
     & ui(i1+1,i2+2,i3+1,c3)+c(i,2,2,1)*ui(i1+2,i2+2,i3+1,c3)+c(i,3,2,
     & 1)*ui(i1+3,i2+2,i3+1,c3)+c(i,4,2,1)*ui(i1+4,i2+2,i3+1,c3)+c(i,
     & 5,2,1)*ui(i1+5,i2+2,i3+1,c3)+c(i,6,2,1)*ui(i1+6,i2+2,i3+1,c3)+
     & c(i,7,2,1)*ui(i1+7,i2+2,i3+1,c3)+c(i,0,3,1)*ui(i1,i2+3,i3+1,c3)
     & +c(i,1,3,1)*ui(i1+1,i2+3,i3+1,c3)+c(i,2,3,1)*ui(i1+2,i2+3,i3+1,
     & c3)+c(i,3,3,1)*ui(i1+3,i2+3,i3+1,c3)+c(i,4,3,1)*ui(i1+4,i2+3,
     & i3+1,c3)+c(i,5,3,1)*ui(i1+5,i2+3,i3+1,c3)+c(i,6,3,1)*ui(i1+6,
     & i2+3,i3+1,c3)+c(i,7,3,1)*ui(i1+7,i2+3,i3+1,c3)+c(i,0,4,1)*ui(
     & i1,i2+4,i3+1,c3)+c(i,1,4,1)*ui(i1+1,i2+4,i3+1,c3)+c(i,2,4,1)*
     & ui(i1+2,i2+4,i3+1,c3)+c(i,3,4,1)*ui(i1+3,i2+4,i3+1,c3)+c(i,4,4,
     & 1)*ui(i1+4,i2+4,i3+1,c3)+c(i,5,4,1)*ui(i1+5,i2+4,i3+1,c3)+c(i,
     & 6,4,1)*ui(i1+6,i2+4,i3+1,c3)+c(i,7,4,1)*ui(i1+7,i2+4,i3+1,c3)+
     & c(i,0,5,1)*ui(i1,i2+5,i3+1,c3)+c(i,1,5,1)*ui(i1+1,i2+5,i3+1,c3)
     & +c(i,2,5,1)*ui(i1+2,i2+5,i3+1,c3)+c(i,3,5,1)*ui(i1+3,i2+5,i3+1,
     & c3)+c(i,4,5,1)*ui(i1+4,i2+5,i3+1,c3)+c(i,5,5,1)*ui(i1+5,i2+5,
     & i3+1,c3)+c(i,6,5,1)*ui(i1+6,i2+5,i3+1,c3)+c(i,7,5,1)*ui(i1+7,
     & i2+5,i3+1,c3)+c(i,0,6,1)*ui(i1,i2+6,i3+1,c3)+c(i,1,6,1)*ui(i1+
     & 1,i2+6,i3+1,c3)+c(i,2,6,1)*ui(i1+2,i2+6,i3+1,c3)+c(i,3,6,1)*ui(
     & i1+3,i2+6,i3+1,c3)+c(i,4,6,1)*ui(i1+4,i2+6,i3+1,c3)+c(i,5,6,1)*
     & ui(i1+5,i2+6,i3+1,c3)+c(i,6,6,1)*ui(i1+6,i2+6,i3+1,c3)+c(i,7,6,
     & 1)*ui(i1+7,i2+6,i3+1,c3)+c(i,0,7,1)*ui(i1,i2+7,i3+1,c3)+c(i,1,
     & 7,1)*ui(i1+1,i2+7,i3+1,c3)+c(i,2,7,1)*ui(i1+2,i2+7,i3+1,c3)+c(
     & i,3,7,1)*ui(i1+3,i2+7,i3+1,c3)+c(i,4,7,1)*ui(i1+4,i2+7,i3+1,c3)
     & +c(i,5,7,1)*ui(i1+5,i2+7,i3+1,c3)+c(i,6,7,1)*ui(i1+6,i2+7,i3+1,
     & c3)+c(i,7,7,1)*ui(i1+7,i2+7,i3+1,c3)+c(i,0,0,2)*ui(i1,i2,i3+2,
     & c3)+c(i,1,0,2)*ui(i1+1,i2,i3+2,c3)+c(i,2,0,2)*ui(i1+2,i2,i3+2,
     & c3)+c(i,3,0,2)*ui(i1+3,i2,i3+2,c3)+c(i,4,0,2)*ui(i1+4,i2,i3+2,
     & c3)+c(i,5,0,2)*ui(i1+5,i2,i3+2,c3)+c(i,6,0,2)*ui(i1+6,i2,i3+2,
     & c3)+c(i,7,0,2)*ui(i1+7,i2,i3+2,c3)+c(i,0,1,2)*ui(i1,i2+1,i3+2,
     & c3)+c(i,1,1,2)*ui(i1+1,i2+1,i3+2,c3)+c(i,2,1,2)*ui(i1+2,i2+1,
     & i3+2,c3)+c(i,3,1,2)*ui(i1+3,i2+1,i3+2,c3)+c(i,4,1,2)*ui(i1+4,
     & i2+1,i3+2,c3)+c(i,5,1,2)*ui(i1+5,i2+1,i3+2,c3)+c(i,6,1,2)*ui(
     & i1+6,i2+1,i3+2,c3)+c(i,7,1,2)*ui(i1+7,i2+1,i3+2,c3)+c(i,0,2,2)*
     & ui(i1,i2+2,i3+2,c3)+c(i,1,2,2)*ui(i1+1,i2+2,i3+2,c3)+c(i,2,2,2)
     & *ui(i1+2,i2+2,i3+2,c3)+c(i,3,2,2)*ui(i1+3,i2+2,i3+2,c3)+c(i,4,
     & 2,2)*ui(i1+4,i2+2,i3+2,c3)+c(i,5,2,2)*ui(i1+5,i2+2,i3+2,c3)+c(
     & i,6,2,2)*ui(i1+6,i2+2,i3+2,c3)+c(i,7,2,2)*ui(i1+7,i2+2,i3+2,c3)
     & +c(i,0,3,2)*ui(i1,i2+3,i3+2,c3)+c(i,1,3,2)*ui(i1+1,i2+3,i3+2,
     & c3)+c(i,2,3,2)*ui(i1+2,i2+3,i3+2,c3)+c(i,3,3,2)*ui(i1+3,i2+3,
     & i3+2,c3)+c(i,4,3,2)*ui(i1+4,i2+3,i3+2,c3)+c(i,5,3,2)*ui(i1+5,
     & i2+3,i3+2,c3)+c(i,6,3,2)*ui(i1+6,i2+3,i3+2,c3)+c(i,7,3,2)*ui(
     & i1+7,i2+3,i3+2,c3)+c(i,0,4,2)*ui(i1,i2+4,i3+2,c3)+c(i,1,4,2)*
     & ui(i1+1,i2+4,i3+2,c3)+c(i,2,4,2)*ui(i1+2,i2+4,i3+2,c3)+c(i,3,4,
     & 2)*ui(i1+3,i2+4,i3+2,c3)+c(i,4,4,2)*ui(i1+4,i2+4,i3+2,c3)+c(i,
     & 5,4,2)*ui(i1+5,i2+4,i3+2,c3)+c(i,6,4,2)*ui(i1+6,i2+4,i3+2,c3)+
     & c(i,7,4,2)*ui(i1+7,i2+4,i3+2,c3)+c(i,0,5,2)*ui(i1,i2+5,i3+2,c3)
     & +c(i,1,5,2)*ui(i1+1,i2+5,i3+2,c3)+c(i,2,5,2)*ui(i1+2,i2+5,i3+2,
     & c3)+c(i,3,5,2)*ui(i1+3,i2+5,i3+2,c3)+c(i,4,5,2)*ui(i1+4,i2+5,
     & i3+2,c3)+c(i,5,5,2)*ui(i1+5,i2+5,i3+2,c3)+c(i,6,5,2)*ui(i1+6,
     & i2+5,i3+2,c3)+c(i,7,5,2)*ui(i1+7,i2+5,i3+2,c3)+c(i,0,6,2)*ui(
     & i1,i2+6,i3+2,c3)+c(i,1,6,2)*ui(i1+1,i2+6,i3+2,c3)+c(i,2,6,2)*
     & ui(i1+2,i2+6,i3+2,c3)+c(i,3,6,2)*ui(i1+3,i2+6,i3+2,c3)+c(i,4,6,
     & 2)*ui(i1+4,i2+6,i3+2,c3)+c(i,5,6,2)*ui(i1+5,i2+6,i3+2,c3)+c(i,
     & 6,6,2)*ui(i1+6,i2+6,i3+2,c3)+c(i,7,6,2)*ui(i1+7,i2+6,i3+2,c3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,0,7,2)*ui(i1,i2+7,i3+2,c3)+c(i,1,7,2)*ui(i1+1,i2+
     & 7,i3+2,c3)+c(i,2,7,2)*ui(i1+2,i2+7,i3+2,c3)+c(i,3,7,2)*ui(i1+3,
     & i2+7,i3+2,c3)+c(i,4,7,2)*ui(i1+4,i2+7,i3+2,c3)+c(i,5,7,2)*ui(
     & i1+5,i2+7,i3+2,c3)+c(i,6,7,2)*ui(i1+6,i2+7,i3+2,c3)+c(i,7,7,2)*
     & ui(i1+7,i2+7,i3+2,c3)+c(i,0,0,3)*ui(i1,i2,i3+3,c3)+c(i,1,0,3)*
     & ui(i1+1,i2,i3+3,c3)+c(i,2,0,3)*ui(i1+2,i2,i3+3,c3)+c(i,3,0,3)*
     & ui(i1+3,i2,i3+3,c3)+c(i,4,0,3)*ui(i1+4,i2,i3+3,c3)+c(i,5,0,3)*
     & ui(i1+5,i2,i3+3,c3)+c(i,6,0,3)*ui(i1+6,i2,i3+3,c3)+c(i,7,0,3)*
     & ui(i1+7,i2,i3+3,c3)+c(i,0,1,3)*ui(i1,i2+1,i3+3,c3)+c(i,1,1,3)*
     & ui(i1+1,i2+1,i3+3,c3)+c(i,2,1,3)*ui(i1+2,i2+1,i3+3,c3)+c(i,3,1,
     & 3)*ui(i1+3,i2+1,i3+3,c3)+c(i,4,1,3)*ui(i1+4,i2+1,i3+3,c3)+c(i,
     & 5,1,3)*ui(i1+5,i2+1,i3+3,c3)+c(i,6,1,3)*ui(i1+6,i2+1,i3+3,c3)+
     & c(i,7,1,3)*ui(i1+7,i2+1,i3+3,c3)+c(i,0,2,3)*ui(i1,i2+2,i3+3,c3)
     & +c(i,1,2,3)*ui(i1+1,i2+2,i3+3,c3)+c(i,2,2,3)*ui(i1+2,i2+2,i3+3,
     & c3)+c(i,3,2,3)*ui(i1+3,i2+2,i3+3,c3)+c(i,4,2,3)*ui(i1+4,i2+2,
     & i3+3,c3)+c(i,5,2,3)*ui(i1+5,i2+2,i3+3,c3)+c(i,6,2,3)*ui(i1+6,
     & i2+2,i3+3,c3)+c(i,7,2,3)*ui(i1+7,i2+2,i3+3,c3)+c(i,0,3,3)*ui(
     & i1,i2+3,i3+3,c3)+c(i,1,3,3)*ui(i1+1,i2+3,i3+3,c3)+c(i,2,3,3)*
     & ui(i1+2,i2+3,i3+3,c3)+c(i,3,3,3)*ui(i1+3,i2+3,i3+3,c3)+c(i,4,3,
     & 3)*ui(i1+4,i2+3,i3+3,c3)+c(i,5,3,3)*ui(i1+5,i2+3,i3+3,c3)+c(i,
     & 6,3,3)*ui(i1+6,i2+3,i3+3,c3)+c(i,7,3,3)*ui(i1+7,i2+3,i3+3,c3)+
     & c(i,0,4,3)*ui(i1,i2+4,i3+3,c3)+c(i,1,4,3)*ui(i1+1,i2+4,i3+3,c3)
     & +c(i,2,4,3)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,4,3)*ui(i1+3,i2+4,i3+3,
     & c3)+c(i,4,4,3)*ui(i1+4,i2+4,i3+3,c3)+c(i,5,4,3)*ui(i1+5,i2+4,
     & i3+3,c3)+c(i,6,4,3)*ui(i1+6,i2+4,i3+3,c3)+c(i,7,4,3)*ui(i1+7,
     & i2+4,i3+3,c3)+c(i,0,5,3)*ui(i1,i2+5,i3+3,c3)+c(i,1,5,3)*ui(i1+
     & 1,i2+5,i3+3,c3)+c(i,2,5,3)*ui(i1+2,i2+5,i3+3,c3)+c(i,3,5,3)*ui(
     & i1+3,i2+5,i3+3,c3)+c(i,4,5,3)*ui(i1+4,i2+5,i3+3,c3)+c(i,5,5,3)*
     & ui(i1+5,i2+5,i3+3,c3)+c(i,6,5,3)*ui(i1+6,i2+5,i3+3,c3)+c(i,7,5,
     & 3)*ui(i1+7,i2+5,i3+3,c3)+c(i,0,6,3)*ui(i1,i2+6,i3+3,c3)+c(i,1,
     & 6,3)*ui(i1+1,i2+6,i3+3,c3)+c(i,2,6,3)*ui(i1+2,i2+6,i3+3,c3)+c(
     & i,3,6,3)*ui(i1+3,i2+6,i3+3,c3)+c(i,4,6,3)*ui(i1+4,i2+6,i3+3,c3)
     & +c(i,5,6,3)*ui(i1+5,i2+6,i3+3,c3)+c(i,6,6,3)*ui(i1+6,i2+6,i3+3,
     & c3)+c(i,7,6,3)*ui(i1+7,i2+6,i3+3,c3)+c(i,0,7,3)*ui(i1,i2+7,i3+
     & 3,c3)+c(i,1,7,3)*ui(i1+1,i2+7,i3+3,c3)+c(i,2,7,3)*ui(i1+2,i2+7,
     & i3+3,c3)+c(i,3,7,3)*ui(i1+3,i2+7,i3+3,c3)+c(i,4,7,3)*ui(i1+4,
     & i2+7,i3+3,c3)+c(i,5,7,3)*ui(i1+5,i2+7,i3+3,c3)+c(i,6,7,3)*ui(
     & i1+6,i2+7,i3+3,c3)+c(i,7,7,3)*ui(i1+7,i2+7,i3+3,c3)+c(i,0,0,4)*
     & ui(i1,i2,i3+4,c3)+c(i,1,0,4)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,4)*ui(
     & i1+2,i2,i3+4,c3)+c(i,3,0,4)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,4)*ui(
     & i1+4,i2,i3+4,c3)+c(i,5,0,4)*ui(i1+5,i2,i3+4,c3)+c(i,6,0,4)*ui(
     & i1+6,i2,i3+4,c3)+c(i,7,0,4)*ui(i1+7,i2,i3+4,c3)+c(i,0,1,4)*ui(
     & i1,i2+1,i3+4,c3)+c(i,1,1,4)*ui(i1+1,i2+1,i3+4,c3)+c(i,2,1,4)*
     & ui(i1+2,i2+1,i3+4,c3)+c(i,3,1,4)*ui(i1+3,i2+1,i3+4,c3)+c(i,4,1,
     & 4)*ui(i1+4,i2+1,i3+4,c3)+c(i,5,1,4)*ui(i1+5,i2+1,i3+4,c3)+c(i,
     & 6,1,4)*ui(i1+6,i2+1,i3+4,c3)+c(i,7,1,4)*ui(i1+7,i2+1,i3+4,c3)+
     & c(i,0,2,4)*ui(i1,i2+2,i3+4,c3)+c(i,1,2,4)*ui(i1+1,i2+2,i3+4,c3)
     & +c(i,2,2,4)*ui(i1+2,i2+2,i3+4,c3)+c(i,3,2,4)*ui(i1+3,i2+2,i3+4,
     & c3)+c(i,4,2,4)*ui(i1+4,i2+2,i3+4,c3)+c(i,5,2,4)*ui(i1+5,i2+2,
     & i3+4,c3)+c(i,6,2,4)*ui(i1+6,i2+2,i3+4,c3)+c(i,7,2,4)*ui(i1+7,
     & i2+2,i3+4,c3)+c(i,0,3,4)*ui(i1,i2+3,i3+4,c3)+c(i,1,3,4)*ui(i1+
     & 1,i2+3,i3+4,c3)+c(i,2,3,4)*ui(i1+2,i2+3,i3+4,c3)+c(i,3,3,4)*ui(
     & i1+3,i2+3,i3+4,c3)+c(i,4,3,4)*ui(i1+4,i2+3,i3+4,c3)+c(i,5,3,4)*
     & ui(i1+5,i2+3,i3+4,c3)+c(i,6,3,4)*ui(i1+6,i2+3,i3+4,c3)+c(i,7,3,
     & 4)*ui(i1+7,i2+3,i3+4,c3)+c(i,0,4,4)*ui(i1,i2+4,i3+4,c3)+c(i,1,
     & 4,4)*ui(i1+1,i2+4,i3+4,c3)+c(i,2,4,4)*ui(i1+2,i2+4,i3+4,c3)+c(
     & i,3,4,4)*ui(i1+3,i2+4,i3+4,c3)+c(i,4,4,4)*ui(i1+4,i2+4,i3+4,c3)
     & +c(i,5,4,4)*ui(i1+5,i2+4,i3+4,c3)+c(i,6,4,4)*ui(i1+6,i2+4,i3+4,
     & c3)+c(i,7,4,4)*ui(i1+7,i2+4,i3+4,c3)+c(i,0,5,4)*ui(i1,i2+5,i3+
     & 4,c3)+c(i,1,5,4)*ui(i1+1,i2+5,i3+4,c3)+c(i,2,5,4)*ui(i1+2,i2+5,
     & i3+4,c3)+c(i,3,5,4)*ui(i1+3,i2+5,i3+4,c3)+c(i,4,5,4)*ui(i1+4,
     & i2+5,i3+4,c3)+c(i,5,5,4)*ui(i1+5,i2+5,i3+4,c3)+c(i,6,5,4)*ui(
     & i1+6,i2+5,i3+4,c3)+c(i,7,5,4)*ui(i1+7,i2+5,i3+4,c3)+c(i,0,6,4)*
     & ui(i1,i2+6,i3+4,c3)+c(i,1,6,4)*ui(i1+1,i2+6,i3+4,c3)+c(i,2,6,4)
     & *ui(i1+2,i2+6,i3+4,c3)+c(i,3,6,4)*ui(i1+3,i2+6,i3+4,c3)+c(i,4,
     & 6,4)*ui(i1+4,i2+6,i3+4,c3)+c(i,5,6,4)*ui(i1+5,i2+6,i3+4,c3)+c(
     & i,6,6,4)*ui(i1+6,i2+6,i3+4,c3)+c(i,7,6,4)*ui(i1+7,i2+6,i3+4,c3)
     & +c(i,0,7,4)*ui(i1,i2+7,i3+4,c3)+c(i,1,7,4)*ui(i1+1,i2+7,i3+4,
     & c3)+c(i,2,7,4)*ui(i1+2,i2+7,i3+4,c3)+c(i,3,7,4)*ui(i1+3,i2+7,
     & i3+4,c3)+c(i,4,7,4)*ui(i1+4,i2+7,i3+4,c3)+c(i,5,7,4)*ui(i1+5,
     & i2+7,i3+4,c3)+c(i,6,7,4)*ui(i1+6,i2+7,i3+4,c3)+c(i,7,7,4)*ui(
     & i1+7,i2+7,i3+4,c3)+c(i,0,0,5)*ui(i1,i2,i3+5,c3)+c(i,1,0,5)*ui(
     & i1+1,i2,i3+5,c3)+c(i,2,0,5)*ui(i1+2,i2,i3+5,c3)+c(i,3,0,5)*ui(
     & i1+3,i2,i3+5,c3)+c(i,4,0,5)*ui(i1+4,i2,i3+5,c3)+c(i,5,0,5)*ui(
     & i1+5,i2,i3+5,c3)+c(i,6,0,5)*ui(i1+6,i2,i3+5,c3)+c(i,7,0,5)*ui(
     & i1+7,i2,i3+5,c3)+c(i,0,1,5)*ui(i1,i2+1,i3+5,c3)+c(i,1,1,5)*ui(
     & i1+1,i2+1,i3+5,c3)+c(i,2,1,5)*ui(i1+2,i2+1,i3+5,c3)+c(i,3,1,5)*
     & ui(i1+3,i2+1,i3+5,c3)+c(i,4,1,5)*ui(i1+4,i2+1,i3+5,c3)+c(i,5,1,
     & 5)*ui(i1+5,i2+1,i3+5,c3)+c(i,6,1,5)*ui(i1+6,i2+1,i3+5,c3)+c(i,
     & 7,1,5)*ui(i1+7,i2+1,i3+5,c3)+c(i,0,2,5)*ui(i1,i2+2,i3+5,c3)+c(
     & i,1,2,5)*ui(i1+1,i2+2,i3+5,c3)+c(i,2,2,5)*ui(i1+2,i2+2,i3+5,c3)
     & +c(i,3,2,5)*ui(i1+3,i2+2,i3+5,c3)+c(i,4,2,5)*ui(i1+4,i2+2,i3+5,
     & c3)+c(i,5,2,5)*ui(i1+5,i2+2,i3+5,c3)+c(i,6,2,5)*ui(i1+6,i2+2,
     & i3+5,c3)+c(i,7,2,5)*ui(i1+7,i2+2,i3+5,c3)+c(i,0,3,5)*ui(i1,i2+
     & 3,i3+5,c3)+c(i,1,3,5)*ui(i1+1,i2+3,i3+5,c3)+c(i,2,3,5)*ui(i1+2,
     & i2+3,i3+5,c3)+c(i,3,3,5)*ui(i1+3,i2+3,i3+5,c3)+c(i,4,3,5)*ui(
     & i1+4,i2+3,i3+5,c3)+c(i,5,3,5)*ui(i1+5,i2+3,i3+5,c3)+c(i,6,3,5)*
     & ui(i1+6,i2+3,i3+5,c3)+c(i,7,3,5)*ui(i1+7,i2+3,i3+5,c3)+c(i,0,4,
     & 5)*ui(i1,i2+4,i3+5,c3)+c(i,1,4,5)*ui(i1+1,i2+4,i3+5,c3)+c(i,2,
     & 4,5)*ui(i1+2,i2+4,i3+5,c3)+c(i,3,4,5)*ui(i1+3,i2+4,i3+5,c3)+c(
     & i,4,4,5)*ui(i1+4,i2+4,i3+5,c3)+c(i,5,4,5)*ui(i1+5,i2+4,i3+5,c3)
     & +c(i,6,4,5)*ui(i1+6,i2+4,i3+5,c3)+c(i,7,4,5)*ui(i1+7,i2+4,i3+5,
     & c3)+c(i,0,5,5)*ui(i1,i2+5,i3+5,c3)+c(i,1,5,5)*ui(i1+1,i2+5,i3+
     & 5,c3)+c(i,2,5,5)*ui(i1+2,i2+5,i3+5,c3)+c(i,3,5,5)*ui(i1+3,i2+5,
     & i3+5,c3)+c(i,4,5,5)*ui(i1+4,i2+5,i3+5,c3)+c(i,5,5,5)*ui(i1+5,
     & i2+5,i3+5,c3)+c(i,6,5,5)*ui(i1+6,i2+5,i3+5,c3)+c(i,7,5,5)*ui(
     & i1+7,i2+5,i3+5,c3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,0,6,5)*ui(i1,i2+6,i3+5,c3)+c(i,1,6,5)*ui(i1+1,i2+
     & 6,i3+5,c3)+c(i,2,6,5)*ui(i1+2,i2+6,i3+5,c3)+c(i,3,6,5)*ui(i1+3,
     & i2+6,i3+5,c3)+c(i,4,6,5)*ui(i1+4,i2+6,i3+5,c3)+c(i,5,6,5)*ui(
     & i1+5,i2+6,i3+5,c3)+c(i,6,6,5)*ui(i1+6,i2+6,i3+5,c3)+c(i,7,6,5)*
     & ui(i1+7,i2+6,i3+5,c3)+c(i,0,7,5)*ui(i1,i2+7,i3+5,c3)+c(i,1,7,5)
     & *ui(i1+1,i2+7,i3+5,c3)+c(i,2,7,5)*ui(i1+2,i2+7,i3+5,c3)+c(i,3,
     & 7,5)*ui(i1+3,i2+7,i3+5,c3)+c(i,4,7,5)*ui(i1+4,i2+7,i3+5,c3)+c(
     & i,5,7,5)*ui(i1+5,i2+7,i3+5,c3)+c(i,6,7,5)*ui(i1+6,i2+7,i3+5,c3)
     & +c(i,7,7,5)*ui(i1+7,i2+7,i3+5,c3)+c(i,0,0,6)*ui(i1,i2,i3+6,c3)+
     & c(i,1,0,6)*ui(i1+1,i2,i3+6,c3)+c(i,2,0,6)*ui(i1+2,i2,i3+6,c3)+
     & c(i,3,0,6)*ui(i1+3,i2,i3+6,c3)+c(i,4,0,6)*ui(i1+4,i2,i3+6,c3)+
     & c(i,5,0,6)*ui(i1+5,i2,i3+6,c3)+c(i,6,0,6)*ui(i1+6,i2,i3+6,c3)+
     & c(i,7,0,6)*ui(i1+7,i2,i3+6,c3)+c(i,0,1,6)*ui(i1,i2+1,i3+6,c3)+
     & c(i,1,1,6)*ui(i1+1,i2+1,i3+6,c3)+c(i,2,1,6)*ui(i1+2,i2+1,i3+6,
     & c3)+c(i,3,1,6)*ui(i1+3,i2+1,i3+6,c3)+c(i,4,1,6)*ui(i1+4,i2+1,
     & i3+6,c3)+c(i,5,1,6)*ui(i1+5,i2+1,i3+6,c3)+c(i,6,1,6)*ui(i1+6,
     & i2+1,i3+6,c3)+c(i,7,1,6)*ui(i1+7,i2+1,i3+6,c3)+c(i,0,2,6)*ui(
     & i1,i2+2,i3+6,c3)+c(i,1,2,6)*ui(i1+1,i2+2,i3+6,c3)+c(i,2,2,6)*
     & ui(i1+2,i2+2,i3+6,c3)+c(i,3,2,6)*ui(i1+3,i2+2,i3+6,c3)+c(i,4,2,
     & 6)*ui(i1+4,i2+2,i3+6,c3)+c(i,5,2,6)*ui(i1+5,i2+2,i3+6,c3)+c(i,
     & 6,2,6)*ui(i1+6,i2+2,i3+6,c3)+c(i,7,2,6)*ui(i1+7,i2+2,i3+6,c3)+
     & c(i,0,3,6)*ui(i1,i2+3,i3+6,c3)+c(i,1,3,6)*ui(i1+1,i2+3,i3+6,c3)
     & +c(i,2,3,6)*ui(i1+2,i2+3,i3+6,c3)+c(i,3,3,6)*ui(i1+3,i2+3,i3+6,
     & c3)+c(i,4,3,6)*ui(i1+4,i2+3,i3+6,c3)+c(i,5,3,6)*ui(i1+5,i2+3,
     & i3+6,c3)+c(i,6,3,6)*ui(i1+6,i2+3,i3+6,c3)+c(i,7,3,6)*ui(i1+7,
     & i2+3,i3+6,c3)+c(i,0,4,6)*ui(i1,i2+4,i3+6,c3)+c(i,1,4,6)*ui(i1+
     & 1,i2+4,i3+6,c3)+c(i,2,4,6)*ui(i1+2,i2+4,i3+6,c3)+c(i,3,4,6)*ui(
     & i1+3,i2+4,i3+6,c3)+c(i,4,4,6)*ui(i1+4,i2+4,i3+6,c3)+c(i,5,4,6)*
     & ui(i1+5,i2+4,i3+6,c3)+c(i,6,4,6)*ui(i1+6,i2+4,i3+6,c3)+c(i,7,4,
     & 6)*ui(i1+7,i2+4,i3+6,c3)+c(i,0,5,6)*ui(i1,i2+5,i3+6,c3)+c(i,1,
     & 5,6)*ui(i1+1,i2+5,i3+6,c3)+c(i,2,5,6)*ui(i1+2,i2+5,i3+6,c3)+c(
     & i,3,5,6)*ui(i1+3,i2+5,i3+6,c3)+c(i,4,5,6)*ui(i1+4,i2+5,i3+6,c3)
     & +c(i,5,5,6)*ui(i1+5,i2+5,i3+6,c3)+c(i,6,5,6)*ui(i1+6,i2+5,i3+6,
     & c3)+c(i,7,5,6)*ui(i1+7,i2+5,i3+6,c3)+c(i,0,6,6)*ui(i1,i2+6,i3+
     & 6,c3)+c(i,1,6,6)*ui(i1+1,i2+6,i3+6,c3)+c(i,2,6,6)*ui(i1+2,i2+6,
     & i3+6,c3)+c(i,3,6,6)*ui(i1+3,i2+6,i3+6,c3)+c(i,4,6,6)*ui(i1+4,
     & i2+6,i3+6,c3)+c(i,5,6,6)*ui(i1+5,i2+6,i3+6,c3)+c(i,6,6,6)*ui(
     & i1+6,i2+6,i3+6,c3)+c(i,7,6,6)*ui(i1+7,i2+6,i3+6,c3)+c(i,0,7,6)*
     & ui(i1,i2+7,i3+6,c3)+c(i,1,7,6)*ui(i1+1,i2+7,i3+6,c3)+c(i,2,7,6)
     & *ui(i1+2,i2+7,i3+6,c3)+c(i,3,7,6)*ui(i1+3,i2+7,i3+6,c3)+c(i,4,
     & 7,6)*ui(i1+4,i2+7,i3+6,c3)+c(i,5,7,6)*ui(i1+5,i2+7,i3+6,c3)+c(
     & i,6,7,6)*ui(i1+6,i2+7,i3+6,c3)+c(i,7,7,6)*ui(i1+7,i2+7,i3+6,c3)
     & +c(i,0,0,7)*ui(i1,i2,i3+7,c3)+c(i,1,0,7)*ui(i1+1,i2,i3+7,c3)+c(
     & i,2,0,7)*ui(i1+2,i2,i3+7,c3)+c(i,3,0,7)*ui(i1+3,i2,i3+7,c3)+c(
     & i,4,0,7)*ui(i1+4,i2,i3+7,c3)+c(i,5,0,7)*ui(i1+5,i2,i3+7,c3)+c(
     & i,6,0,7)*ui(i1+6,i2,i3+7,c3)+c(i,7,0,7)*ui(i1+7,i2,i3+7,c3)+c(
     & i,0,1,7)*ui(i1,i2+1,i3+7,c3)+c(i,1,1,7)*ui(i1+1,i2+1,i3+7,c3)+
     & c(i,2,1,7)*ui(i1+2,i2+1,i3+7,c3)+c(i,3,1,7)*ui(i1+3,i2+1,i3+7,
     & c3)+c(i,4,1,7)*ui(i1+4,i2+1,i3+7,c3)+c(i,5,1,7)*ui(i1+5,i2+1,
     & i3+7,c3)+c(i,6,1,7)*ui(i1+6,i2+1,i3+7,c3)+c(i,7,1,7)*ui(i1+7,
     & i2+1,i3+7,c3)+c(i,0,2,7)*ui(i1,i2+2,i3+7,c3)+c(i,1,2,7)*ui(i1+
     & 1,i2+2,i3+7,c3)+c(i,2,2,7)*ui(i1+2,i2+2,i3+7,c3)+c(i,3,2,7)*ui(
     & i1+3,i2+2,i3+7,c3)+c(i,4,2,7)*ui(i1+4,i2+2,i3+7,c3)+c(i,5,2,7)*
     & ui(i1+5,i2+2,i3+7,c3)+c(i,6,2,7)*ui(i1+6,i2+2,i3+7,c3)+c(i,7,2,
     & 7)*ui(i1+7,i2+2,i3+7,c3)+c(i,0,3,7)*ui(i1,i2+3,i3+7,c3)+c(i,1,
     & 3,7)*ui(i1+1,i2+3,i3+7,c3)+c(i,2,3,7)*ui(i1+2,i2+3,i3+7,c3)+c(
     & i,3,3,7)*ui(i1+3,i2+3,i3+7,c3)+c(i,4,3,7)*ui(i1+4,i2+3,i3+7,c3)
     & +c(i,5,3,7)*ui(i1+5,i2+3,i3+7,c3)+c(i,6,3,7)*ui(i1+6,i2+3,i3+7,
     & c3)+c(i,7,3,7)*ui(i1+7,i2+3,i3+7,c3)+c(i,0,4,7)*ui(i1,i2+4,i3+
     & 7,c3)+c(i,1,4,7)*ui(i1+1,i2+4,i3+7,c3)+c(i,2,4,7)*ui(i1+2,i2+4,
     & i3+7,c3)+c(i,3,4,7)*ui(i1+3,i2+4,i3+7,c3)+c(i,4,4,7)*ui(i1+4,
     & i2+4,i3+7,c3)+c(i,5,4,7)*ui(i1+5,i2+4,i3+7,c3)+c(i,6,4,7)*ui(
     & i1+6,i2+4,i3+7,c3)+c(i,7,4,7)*ui(i1+7,i2+4,i3+7,c3)+c(i,0,5,7)*
     & ui(i1,i2+5,i3+7,c3)+c(i,1,5,7)*ui(i1+1,i2+5,i3+7,c3)+c(i,2,5,7)
     & *ui(i1+2,i2+5,i3+7,c3)+c(i,3,5,7)*ui(i1+3,i2+5,i3+7,c3)+c(i,4,
     & 5,7)*ui(i1+4,i2+5,i3+7,c3)+c(i,5,5,7)*ui(i1+5,i2+5,i3+7,c3)+c(
     & i,6,5,7)*ui(i1+6,i2+5,i3+7,c3)+c(i,7,5,7)*ui(i1+7,i2+5,i3+7,c3)
     & +c(i,0,6,7)*ui(i1,i2+6,i3+7,c3)+c(i,1,6,7)*ui(i1+1,i2+6,i3+7,
     & c3)+c(i,2,6,7)*ui(i1+2,i2+6,i3+7,c3)+c(i,3,6,7)*ui(i1+3,i2+6,
     & i3+7,c3)+c(i,4,6,7)*ui(i1+4,i2+6,i3+7,c3)+c(i,5,6,7)*ui(i1+5,
     & i2+6,i3+7,c3)+c(i,6,6,7)*ui(i1+6,i2+6,i3+7,c3)+c(i,7,6,7)*ui(
     & i1+7,i2+6,i3+7,c3)+c(i,0,7,7)*ui(i1,i2+7,i3+7,c3)+c(i,1,7,7)*
     & ui(i1+1,i2+7,i3+7,c3)+c(i,2,7,7)*ui(i1+2,i2+7,i3+7,c3)+c(i,3,7,
     & 7)*ui(i1+3,i2+7,i3+7,c3)+c(i,4,7,7)*ui(i1+4,i2+7,i3+7,c3)+c(i,
     & 5,7,7)*ui(i1+5,i2+7,i3+7,c3)+c(i,6,7,7)*ui(i1+6,i2+7,i3+7,c3)+
     & c(i,7,7,7)*ui(i1+7,i2+7,i3+7,c3)


              end do
              end do
            end if
          else if( width(1).eq.9 .and. width(2).eq.9 .and. width(3)
     & .eq.9 )then
! loops3d($interp999(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)
            if( c3a.eq.c3b )then
              do c3=c3a,c3b
              do i=nia,nib
! interp999(ug(ip(i,1),ip(i,2),ip(i,3),c3))
                i1=il(i,1)
                i2=il(i,2)
                i3=il(i,3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,0,0)*ui(i1,i2,
     & i3,c3)+c(i,1,0,0)*ui(i1+1,i2,i3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3,
     & c3)+c(i,3,0,0)*ui(i1+3,i2,i3,c3)+c(i,4,0,0)*ui(i1+4,i2,i3,c3)+
     & c(i,5,0,0)*ui(i1+5,i2,i3,c3)+c(i,6,0,0)*ui(i1+6,i2,i3,c3)+c(i,
     & 7,0,0)*ui(i1+7,i2,i3,c3)+c(i,8,0,0)*ui(i1+8,i2,i3,c3)+c(i,0,1,
     & 0)*ui(i1,i2+1,i3,c3)+c(i,1,1,0)*ui(i1+1,i2+1,i3,c3)+c(i,2,1,0)*
     & ui(i1+2,i2+1,i3,c3)+c(i,3,1,0)*ui(i1+3,i2+1,i3,c3)+c(i,4,1,0)*
     & ui(i1+4,i2+1,i3,c3)+c(i,5,1,0)*ui(i1+5,i2+1,i3,c3)+c(i,6,1,0)*
     & ui(i1+6,i2+1,i3,c3)+c(i,7,1,0)*ui(i1+7,i2+1,i3,c3)+c(i,8,1,0)*
     & ui(i1+8,i2+1,i3,c3)+c(i,0,2,0)*ui(i1,i2+2,i3,c3)+c(i,1,2,0)*ui(
     & i1+1,i2+2,i3,c3)+c(i,2,2,0)*ui(i1+2,i2+2,i3,c3)+c(i,3,2,0)*ui(
     & i1+3,i2+2,i3,c3)+c(i,4,2,0)*ui(i1+4,i2+2,i3,c3)+c(i,5,2,0)*ui(
     & i1+5,i2+2,i3,c3)+c(i,6,2,0)*ui(i1+6,i2+2,i3,c3)+c(i,7,2,0)*ui(
     & i1+7,i2+2,i3,c3)+c(i,8,2,0)*ui(i1+8,i2+2,i3,c3)+c(i,0,3,0)*ui(
     & i1,i2+3,i3,c3)+c(i,1,3,0)*ui(i1+1,i2+3,i3,c3)+c(i,2,3,0)*ui(i1+
     & 2,i2+3,i3,c3)+c(i,3,3,0)*ui(i1+3,i2+3,i3,c3)+c(i,4,3,0)*ui(i1+
     & 4,i2+3,i3,c3)+c(i,5,3,0)*ui(i1+5,i2+3,i3,c3)+c(i,6,3,0)*ui(i1+
     & 6,i2+3,i3,c3)+c(i,7,3,0)*ui(i1+7,i2+3,i3,c3)+c(i,8,3,0)*ui(i1+
     & 8,i2+3,i3,c3)+c(i,0,4,0)*ui(i1,i2+4,i3,c3)+c(i,1,4,0)*ui(i1+1,
     & i2+4,i3,c3)+c(i,2,4,0)*ui(i1+2,i2+4,i3,c3)+c(i,3,4,0)*ui(i1+3,
     & i2+4,i3,c3)+c(i,4,4,0)*ui(i1+4,i2+4,i3,c3)+c(i,5,4,0)*ui(i1+5,
     & i2+4,i3,c3)+c(i,6,4,0)*ui(i1+6,i2+4,i3,c3)+c(i,7,4,0)*ui(i1+7,
     & i2+4,i3,c3)+c(i,8,4,0)*ui(i1+8,i2+4,i3,c3)+c(i,0,5,0)*ui(i1,i2+
     & 5,i3,c3)+c(i,1,5,0)*ui(i1+1,i2+5,i3,c3)+c(i,2,5,0)*ui(i1+2,i2+
     & 5,i3,c3)+c(i,3,5,0)*ui(i1+3,i2+5,i3,c3)+c(i,4,5,0)*ui(i1+4,i2+
     & 5,i3,c3)+c(i,5,5,0)*ui(i1+5,i2+5,i3,c3)+c(i,6,5,0)*ui(i1+6,i2+
     & 5,i3,c3)+c(i,7,5,0)*ui(i1+7,i2+5,i3,c3)+c(i,8,5,0)*ui(i1+8,i2+
     & 5,i3,c3)+c(i,0,6,0)*ui(i1,i2+6,i3,c3)+c(i,1,6,0)*ui(i1+1,i2+6,
     & i3,c3)+c(i,2,6,0)*ui(i1+2,i2+6,i3,c3)+c(i,3,6,0)*ui(i1+3,i2+6,
     & i3,c3)+c(i,4,6,0)*ui(i1+4,i2+6,i3,c3)+c(i,5,6,0)*ui(i1+5,i2+6,
     & i3,c3)+c(i,6,6,0)*ui(i1+6,i2+6,i3,c3)+c(i,7,6,0)*ui(i1+7,i2+6,
     & i3,c3)+c(i,8,6,0)*ui(i1+8,i2+6,i3,c3)+c(i,0,7,0)*ui(i1,i2+7,i3,
     & c3)+c(i,1,7,0)*ui(i1+1,i2+7,i3,c3)+c(i,2,7,0)*ui(i1+2,i2+7,i3,
     & c3)+c(i,3,7,0)*ui(i1+3,i2+7,i3,c3)+c(i,4,7,0)*ui(i1+4,i2+7,i3,
     & c3)+c(i,5,7,0)*ui(i1+5,i2+7,i3,c3)+c(i,6,7,0)*ui(i1+6,i2+7,i3,
     & c3)+c(i,7,7,0)*ui(i1+7,i2+7,i3,c3)+c(i,8,7,0)*ui(i1+8,i2+7,i3,
     & c3)+c(i,0,8,0)*ui(i1,i2+8,i3,c3)+c(i,1,8,0)*ui(i1+1,i2+8,i3,c3)
     & +c(i,2,8,0)*ui(i1+2,i2+8,i3,c3)+c(i,3,8,0)*ui(i1+3,i2+8,i3,c3)+
     & c(i,4,8,0)*ui(i1+4,i2+8,i3,c3)+c(i,5,8,0)*ui(i1+5,i2+8,i3,c3)+
     & c(i,6,8,0)*ui(i1+6,i2+8,i3,c3)+c(i,7,8,0)*ui(i1+7,i2+8,i3,c3)+
     & c(i,8,8,0)*ui(i1+8,i2+8,i3,c3)+c(i,0,0,1)*ui(i1,i2,i3+1,c3)+c(
     & i,1,0,1)*ui(i1+1,i2,i3+1,c3)+c(i,2,0,1)*ui(i1+2,i2,i3+1,c3)+c(
     & i,3,0,1)*ui(i1+3,i2,i3+1,c3)+c(i,4,0,1)*ui(i1+4,i2,i3+1,c3)+c(
     & i,5,0,1)*ui(i1+5,i2,i3+1,c3)+c(i,6,0,1)*ui(i1+6,i2,i3+1,c3)+c(
     & i,7,0,1)*ui(i1+7,i2,i3+1,c3)+c(i,8,0,1)*ui(i1+8,i2,i3+1,c3)+c(
     & i,0,1,1)*ui(i1,i2+1,i3+1,c3)+c(i,1,1,1)*ui(i1+1,i2+1,i3+1,c3)+
     & c(i,2,1,1)*ui(i1+2,i2+1,i3+1,c3)+c(i,3,1,1)*ui(i1+3,i2+1,i3+1,
     & c3)+c(i,4,1,1)*ui(i1+4,i2+1,i3+1,c3)+c(i,5,1,1)*ui(i1+5,i2+1,
     & i3+1,c3)+c(i,6,1,1)*ui(i1+6,i2+1,i3+1,c3)+c(i,7,1,1)*ui(i1+7,
     & i2+1,i3+1,c3)+c(i,8,1,1)*ui(i1+8,i2+1,i3+1,c3)+c(i,0,2,1)*ui(
     & i1,i2+2,i3+1,c3)+c(i,1,2,1)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,2,1)*
     & ui(i1+2,i2+2,i3+1,c3)+c(i,3,2,1)*ui(i1+3,i2+2,i3+1,c3)+c(i,4,2,
     & 1)*ui(i1+4,i2+2,i3+1,c3)+c(i,5,2,1)*ui(i1+5,i2+2,i3+1,c3)+c(i,
     & 6,2,1)*ui(i1+6,i2+2,i3+1,c3)+c(i,7,2,1)*ui(i1+7,i2+2,i3+1,c3)+
     & c(i,8,2,1)*ui(i1+8,i2+2,i3+1,c3)+c(i,0,3,1)*ui(i1,i2+3,i3+1,c3)
     & +c(i,1,3,1)*ui(i1+1,i2+3,i3+1,c3)+c(i,2,3,1)*ui(i1+2,i2+3,i3+1,
     & c3)+c(i,3,3,1)*ui(i1+3,i2+3,i3+1,c3)+c(i,4,3,1)*ui(i1+4,i2+3,
     & i3+1,c3)+c(i,5,3,1)*ui(i1+5,i2+3,i3+1,c3)+c(i,6,3,1)*ui(i1+6,
     & i2+3,i3+1,c3)+c(i,7,3,1)*ui(i1+7,i2+3,i3+1,c3)+c(i,8,3,1)*ui(
     & i1+8,i2+3,i3+1,c3)+c(i,0,4,1)*ui(i1,i2+4,i3+1,c3)+c(i,1,4,1)*
     & ui(i1+1,i2+4,i3+1,c3)+c(i,2,4,1)*ui(i1+2,i2+4,i3+1,c3)+c(i,3,4,
     & 1)*ui(i1+3,i2+4,i3+1,c3)+c(i,4,4,1)*ui(i1+4,i2+4,i3+1,c3)+c(i,
     & 5,4,1)*ui(i1+5,i2+4,i3+1,c3)+c(i,6,4,1)*ui(i1+6,i2+4,i3+1,c3)+
     & c(i,7,4,1)*ui(i1+7,i2+4,i3+1,c3)+c(i,8,4,1)*ui(i1+8,i2+4,i3+1,
     & c3)+c(i,0,5,1)*ui(i1,i2+5,i3+1,c3)+c(i,1,5,1)*ui(i1+1,i2+5,i3+
     & 1,c3)+c(i,2,5,1)*ui(i1+2,i2+5,i3+1,c3)+c(i,3,5,1)*ui(i1+3,i2+5,
     & i3+1,c3)+c(i,4,5,1)*ui(i1+4,i2+5,i3+1,c3)+c(i,5,5,1)*ui(i1+5,
     & i2+5,i3+1,c3)+c(i,6,5,1)*ui(i1+6,i2+5,i3+1,c3)+c(i,7,5,1)*ui(
     & i1+7,i2+5,i3+1,c3)+c(i,8,5,1)*ui(i1+8,i2+5,i3+1,c3)+c(i,0,6,1)*
     & ui(i1,i2+6,i3+1,c3)+c(i,1,6,1)*ui(i1+1,i2+6,i3+1,c3)+c(i,2,6,1)
     & *ui(i1+2,i2+6,i3+1,c3)+c(i,3,6,1)*ui(i1+3,i2+6,i3+1,c3)+c(i,4,
     & 6,1)*ui(i1+4,i2+6,i3+1,c3)+c(i,5,6,1)*ui(i1+5,i2+6,i3+1,c3)+c(
     & i,6,6,1)*ui(i1+6,i2+6,i3+1,c3)+c(i,7,6,1)*ui(i1+7,i2+6,i3+1,c3)
     & +c(i,8,6,1)*ui(i1+8,i2+6,i3+1,c3)+c(i,0,7,1)*ui(i1,i2+7,i3+1,
     & c3)+c(i,1,7,1)*ui(i1+1,i2+7,i3+1,c3)+c(i,2,7,1)*ui(i1+2,i2+7,
     & i3+1,c3)+c(i,3,7,1)*ui(i1+3,i2+7,i3+1,c3)+c(i,4,7,1)*ui(i1+4,
     & i2+7,i3+1,c3)+c(i,5,7,1)*ui(i1+5,i2+7,i3+1,c3)+c(i,6,7,1)*ui(
     & i1+6,i2+7,i3+1,c3)+c(i,7,7,1)*ui(i1+7,i2+7,i3+1,c3)+c(i,8,7,1)*
     & ui(i1+8,i2+7,i3+1,c3)+c(i,0,8,1)*ui(i1,i2+8,i3+1,c3)+c(i,1,8,1)
     & *ui(i1+1,i2+8,i3+1,c3)+c(i,2,8,1)*ui(i1+2,i2+8,i3+1,c3)+c(i,3,
     & 8,1)*ui(i1+3,i2+8,i3+1,c3)+c(i,4,8,1)*ui(i1+4,i2+8,i3+1,c3)+c(
     & i,5,8,1)*ui(i1+5,i2+8,i3+1,c3)+c(i,6,8,1)*ui(i1+6,i2+8,i3+1,c3)
     & +c(i,7,8,1)*ui(i1+7,i2+8,i3+1,c3)+c(i,8,8,1)*ui(i1+8,i2+8,i3+1,
     & c3)+c(i,0,0,2)*ui(i1,i2,i3+2,c3)+c(i,1,0,2)*ui(i1+1,i2,i3+2,c3)
     & +c(i,2,0,2)*ui(i1+2,i2,i3+2,c3)+c(i,3,0,2)*ui(i1+3,i2,i3+2,c3)+
     & c(i,4,0,2)*ui(i1+4,i2,i3+2,c3)+c(i,5,0,2)*ui(i1+5,i2,i3+2,c3)+
     & c(i,6,0,2)*ui(i1+6,i2,i3+2,c3)+c(i,7,0,2)*ui(i1+7,i2,i3+2,c3)+
     & c(i,8,0,2)*ui(i1+8,i2,i3+2,c3)+c(i,0,1,2)*ui(i1,i2+1,i3+2,c3)+
     & c(i,1,1,2)*ui(i1+1,i2+1,i3+2,c3)+c(i,2,1,2)*ui(i1+2,i2+1,i3+2,
     & c3)+c(i,3,1,2)*ui(i1+3,i2+1,i3+2,c3)+c(i,4,1,2)*ui(i1+4,i2+1,
     & i3+2,c3)+c(i,5,1,2)*ui(i1+5,i2+1,i3+2,c3)+c(i,6,1,2)*ui(i1+6,
     & i2+1,i3+2,c3)+c(i,7,1,2)*ui(i1+7,i2+1,i3+2,c3)+c(i,8,1,2)*ui(
     & i1+8,i2+1,i3+2,c3)+c(i,0,2,2)*ui(i1,i2+2,i3+2,c3)+c(i,1,2,2)*
     & ui(i1+1,i2+2,i3+2,c3)+c(i,2,2,2)*ui(i1+2,i2+2,i3+2,c3)+c(i,3,2,
     & 2)*ui(i1+3,i2+2,i3+2,c3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,4,2,2)*ui(i1+4,i2+2,i3+2,c3)+c(i,5,2,2)*ui(i1+5,
     & i2+2,i3+2,c3)+c(i,6,2,2)*ui(i1+6,i2+2,i3+2,c3)+c(i,7,2,2)*ui(
     & i1+7,i2+2,i3+2,c3)+c(i,8,2,2)*ui(i1+8,i2+2,i3+2,c3)+c(i,0,3,2)*
     & ui(i1,i2+3,i3+2,c3)+c(i,1,3,2)*ui(i1+1,i2+3,i3+2,c3)+c(i,2,3,2)
     & *ui(i1+2,i2+3,i3+2,c3)+c(i,3,3,2)*ui(i1+3,i2+3,i3+2,c3)+c(i,4,
     & 3,2)*ui(i1+4,i2+3,i3+2,c3)+c(i,5,3,2)*ui(i1+5,i2+3,i3+2,c3)+c(
     & i,6,3,2)*ui(i1+6,i2+3,i3+2,c3)+c(i,7,3,2)*ui(i1+7,i2+3,i3+2,c3)
     & +c(i,8,3,2)*ui(i1+8,i2+3,i3+2,c3)+c(i,0,4,2)*ui(i1,i2+4,i3+2,
     & c3)+c(i,1,4,2)*ui(i1+1,i2+4,i3+2,c3)+c(i,2,4,2)*ui(i1+2,i2+4,
     & i3+2,c3)+c(i,3,4,2)*ui(i1+3,i2+4,i3+2,c3)+c(i,4,4,2)*ui(i1+4,
     & i2+4,i3+2,c3)+c(i,5,4,2)*ui(i1+5,i2+4,i3+2,c3)+c(i,6,4,2)*ui(
     & i1+6,i2+4,i3+2,c3)+c(i,7,4,2)*ui(i1+7,i2+4,i3+2,c3)+c(i,8,4,2)*
     & ui(i1+8,i2+4,i3+2,c3)+c(i,0,5,2)*ui(i1,i2+5,i3+2,c3)+c(i,1,5,2)
     & *ui(i1+1,i2+5,i3+2,c3)+c(i,2,5,2)*ui(i1+2,i2+5,i3+2,c3)+c(i,3,
     & 5,2)*ui(i1+3,i2+5,i3+2,c3)+c(i,4,5,2)*ui(i1+4,i2+5,i3+2,c3)+c(
     & i,5,5,2)*ui(i1+5,i2+5,i3+2,c3)+c(i,6,5,2)*ui(i1+6,i2+5,i3+2,c3)
     & +c(i,7,5,2)*ui(i1+7,i2+5,i3+2,c3)+c(i,8,5,2)*ui(i1+8,i2+5,i3+2,
     & c3)+c(i,0,6,2)*ui(i1,i2+6,i3+2,c3)+c(i,1,6,2)*ui(i1+1,i2+6,i3+
     & 2,c3)+c(i,2,6,2)*ui(i1+2,i2+6,i3+2,c3)+c(i,3,6,2)*ui(i1+3,i2+6,
     & i3+2,c3)+c(i,4,6,2)*ui(i1+4,i2+6,i3+2,c3)+c(i,5,6,2)*ui(i1+5,
     & i2+6,i3+2,c3)+c(i,6,6,2)*ui(i1+6,i2+6,i3+2,c3)+c(i,7,6,2)*ui(
     & i1+7,i2+6,i3+2,c3)+c(i,8,6,2)*ui(i1+8,i2+6,i3+2,c3)+c(i,0,7,2)*
     & ui(i1,i2+7,i3+2,c3)+c(i,1,7,2)*ui(i1+1,i2+7,i3+2,c3)+c(i,2,7,2)
     & *ui(i1+2,i2+7,i3+2,c3)+c(i,3,7,2)*ui(i1+3,i2+7,i3+2,c3)+c(i,4,
     & 7,2)*ui(i1+4,i2+7,i3+2,c3)+c(i,5,7,2)*ui(i1+5,i2+7,i3+2,c3)+c(
     & i,6,7,2)*ui(i1+6,i2+7,i3+2,c3)+c(i,7,7,2)*ui(i1+7,i2+7,i3+2,c3)
     & +c(i,8,7,2)*ui(i1+8,i2+7,i3+2,c3)+c(i,0,8,2)*ui(i1,i2+8,i3+2,
     & c3)+c(i,1,8,2)*ui(i1+1,i2+8,i3+2,c3)+c(i,2,8,2)*ui(i1+2,i2+8,
     & i3+2,c3)+c(i,3,8,2)*ui(i1+3,i2+8,i3+2,c3)+c(i,4,8,2)*ui(i1+4,
     & i2+8,i3+2,c3)+c(i,5,8,2)*ui(i1+5,i2+8,i3+2,c3)+c(i,6,8,2)*ui(
     & i1+6,i2+8,i3+2,c3)+c(i,7,8,2)*ui(i1+7,i2+8,i3+2,c3)+c(i,8,8,2)*
     & ui(i1+8,i2+8,i3+2,c3)+c(i,0,0,3)*ui(i1,i2,i3+3,c3)+c(i,1,0,3)*
     & ui(i1+1,i2,i3+3,c3)+c(i,2,0,3)*ui(i1+2,i2,i3+3,c3)+c(i,3,0,3)*
     & ui(i1+3,i2,i3+3,c3)+c(i,4,0,3)*ui(i1+4,i2,i3+3,c3)+c(i,5,0,3)*
     & ui(i1+5,i2,i3+3,c3)+c(i,6,0,3)*ui(i1+6,i2,i3+3,c3)+c(i,7,0,3)*
     & ui(i1+7,i2,i3+3,c3)+c(i,8,0,3)*ui(i1+8,i2,i3+3,c3)+c(i,0,1,3)*
     & ui(i1,i2+1,i3+3,c3)+c(i,1,1,3)*ui(i1+1,i2+1,i3+3,c3)+c(i,2,1,3)
     & *ui(i1+2,i2+1,i3+3,c3)+c(i,3,1,3)*ui(i1+3,i2+1,i3+3,c3)+c(i,4,
     & 1,3)*ui(i1+4,i2+1,i3+3,c3)+c(i,5,1,3)*ui(i1+5,i2+1,i3+3,c3)+c(
     & i,6,1,3)*ui(i1+6,i2+1,i3+3,c3)+c(i,7,1,3)*ui(i1+7,i2+1,i3+3,c3)
     & +c(i,8,1,3)*ui(i1+8,i2+1,i3+3,c3)+c(i,0,2,3)*ui(i1,i2+2,i3+3,
     & c3)+c(i,1,2,3)*ui(i1+1,i2+2,i3+3,c3)+c(i,2,2,3)*ui(i1+2,i2+2,
     & i3+3,c3)+c(i,3,2,3)*ui(i1+3,i2+2,i3+3,c3)+c(i,4,2,3)*ui(i1+4,
     & i2+2,i3+3,c3)+c(i,5,2,3)*ui(i1+5,i2+2,i3+3,c3)+c(i,6,2,3)*ui(
     & i1+6,i2+2,i3+3,c3)+c(i,7,2,3)*ui(i1+7,i2+2,i3+3,c3)+c(i,8,2,3)*
     & ui(i1+8,i2+2,i3+3,c3)+c(i,0,3,3)*ui(i1,i2+3,i3+3,c3)+c(i,1,3,3)
     & *ui(i1+1,i2+3,i3+3,c3)+c(i,2,3,3)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,
     & 3,3)*ui(i1+3,i2+3,i3+3,c3)+c(i,4,3,3)*ui(i1+4,i2+3,i3+3,c3)+c(
     & i,5,3,3)*ui(i1+5,i2+3,i3+3,c3)+c(i,6,3,3)*ui(i1+6,i2+3,i3+3,c3)
     & +c(i,7,3,3)*ui(i1+7,i2+3,i3+3,c3)+c(i,8,3,3)*ui(i1+8,i2+3,i3+3,
     & c3)+c(i,0,4,3)*ui(i1,i2+4,i3+3,c3)+c(i,1,4,3)*ui(i1+1,i2+4,i3+
     & 3,c3)+c(i,2,4,3)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,4,3)*ui(i1+3,i2+4,
     & i3+3,c3)+c(i,4,4,3)*ui(i1+4,i2+4,i3+3,c3)+c(i,5,4,3)*ui(i1+5,
     & i2+4,i3+3,c3)+c(i,6,4,3)*ui(i1+6,i2+4,i3+3,c3)+c(i,7,4,3)*ui(
     & i1+7,i2+4,i3+3,c3)+c(i,8,4,3)*ui(i1+8,i2+4,i3+3,c3)+c(i,0,5,3)*
     & ui(i1,i2+5,i3+3,c3)+c(i,1,5,3)*ui(i1+1,i2+5,i3+3,c3)+c(i,2,5,3)
     & *ui(i1+2,i2+5,i3+3,c3)+c(i,3,5,3)*ui(i1+3,i2+5,i3+3,c3)+c(i,4,
     & 5,3)*ui(i1+4,i2+5,i3+3,c3)+c(i,5,5,3)*ui(i1+5,i2+5,i3+3,c3)+c(
     & i,6,5,3)*ui(i1+6,i2+5,i3+3,c3)+c(i,7,5,3)*ui(i1+7,i2+5,i3+3,c3)
     & +c(i,8,5,3)*ui(i1+8,i2+5,i3+3,c3)+c(i,0,6,3)*ui(i1,i2+6,i3+3,
     & c3)+c(i,1,6,3)*ui(i1+1,i2+6,i3+3,c3)+c(i,2,6,3)*ui(i1+2,i2+6,
     & i3+3,c3)+c(i,3,6,3)*ui(i1+3,i2+6,i3+3,c3)+c(i,4,6,3)*ui(i1+4,
     & i2+6,i3+3,c3)+c(i,5,6,3)*ui(i1+5,i2+6,i3+3,c3)+c(i,6,6,3)*ui(
     & i1+6,i2+6,i3+3,c3)+c(i,7,6,3)*ui(i1+7,i2+6,i3+3,c3)+c(i,8,6,3)*
     & ui(i1+8,i2+6,i3+3,c3)+c(i,0,7,3)*ui(i1,i2+7,i3+3,c3)+c(i,1,7,3)
     & *ui(i1+1,i2+7,i3+3,c3)+c(i,2,7,3)*ui(i1+2,i2+7,i3+3,c3)+c(i,3,
     & 7,3)*ui(i1+3,i2+7,i3+3,c3)+c(i,4,7,3)*ui(i1+4,i2+7,i3+3,c3)+c(
     & i,5,7,3)*ui(i1+5,i2+7,i3+3,c3)+c(i,6,7,3)*ui(i1+6,i2+7,i3+3,c3)
     & +c(i,7,7,3)*ui(i1+7,i2+7,i3+3,c3)+c(i,8,7,3)*ui(i1+8,i2+7,i3+3,
     & c3)+c(i,0,8,3)*ui(i1,i2+8,i3+3,c3)+c(i,1,8,3)*ui(i1+1,i2+8,i3+
     & 3,c3)+c(i,2,8,3)*ui(i1+2,i2+8,i3+3,c3)+c(i,3,8,3)*ui(i1+3,i2+8,
     & i3+3,c3)+c(i,4,8,3)*ui(i1+4,i2+8,i3+3,c3)+c(i,5,8,3)*ui(i1+5,
     & i2+8,i3+3,c3)+c(i,6,8,3)*ui(i1+6,i2+8,i3+3,c3)+c(i,7,8,3)*ui(
     & i1+7,i2+8,i3+3,c3)+c(i,8,8,3)*ui(i1+8,i2+8,i3+3,c3)+c(i,0,0,4)*
     & ui(i1,i2,i3+4,c3)+c(i,1,0,4)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,4)*ui(
     & i1+2,i2,i3+4,c3)+c(i,3,0,4)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,4)*ui(
     & i1+4,i2,i3+4,c3)+c(i,5,0,4)*ui(i1+5,i2,i3+4,c3)+c(i,6,0,4)*ui(
     & i1+6,i2,i3+4,c3)+c(i,7,0,4)*ui(i1+7,i2,i3+4,c3)+c(i,8,0,4)*ui(
     & i1+8,i2,i3+4,c3)+c(i,0,1,4)*ui(i1,i2+1,i3+4,c3)+c(i,1,1,4)*ui(
     & i1+1,i2+1,i3+4,c3)+c(i,2,1,4)*ui(i1+2,i2+1,i3+4,c3)+c(i,3,1,4)*
     & ui(i1+3,i2+1,i3+4,c3)+c(i,4,1,4)*ui(i1+4,i2+1,i3+4,c3)+c(i,5,1,
     & 4)*ui(i1+5,i2+1,i3+4,c3)+c(i,6,1,4)*ui(i1+6,i2+1,i3+4,c3)+c(i,
     & 7,1,4)*ui(i1+7,i2+1,i3+4,c3)+c(i,8,1,4)*ui(i1+8,i2+1,i3+4,c3)+
     & c(i,0,2,4)*ui(i1,i2+2,i3+4,c3)+c(i,1,2,4)*ui(i1+1,i2+2,i3+4,c3)
     & +c(i,2,2,4)*ui(i1+2,i2+2,i3+4,c3)+c(i,3,2,4)*ui(i1+3,i2+2,i3+4,
     & c3)+c(i,4,2,4)*ui(i1+4,i2+2,i3+4,c3)+c(i,5,2,4)*ui(i1+5,i2+2,
     & i3+4,c3)+c(i,6,2,4)*ui(i1+6,i2+2,i3+4,c3)+c(i,7,2,4)*ui(i1+7,
     & i2+2,i3+4,c3)+c(i,8,2,4)*ui(i1+8,i2+2,i3+4,c3)+c(i,0,3,4)*ui(
     & i1,i2+3,i3+4,c3)+c(i,1,3,4)*ui(i1+1,i2+3,i3+4,c3)+c(i,2,3,4)*
     & ui(i1+2,i2+3,i3+4,c3)+c(i,3,3,4)*ui(i1+3,i2+3,i3+4,c3)+c(i,4,3,
     & 4)*ui(i1+4,i2+3,i3+4,c3)+c(i,5,3,4)*ui(i1+5,i2+3,i3+4,c3)+c(i,
     & 6,3,4)*ui(i1+6,i2+3,i3+4,c3)+c(i,7,3,4)*ui(i1+7,i2+3,i3+4,c3)+
     & c(i,8,3,4)*ui(i1+8,i2+3,i3+4,c3)+c(i,0,4,4)*ui(i1,i2+4,i3+4,c3)
     & +c(i,1,4,4)*ui(i1+1,i2+4,i3+4,c3)+c(i,2,4,4)*ui(i1+2,i2+4,i3+4,
     & c3)+c(i,3,4,4)*ui(i1+3,i2+4,i3+4,c3)+c(i,4,4,4)*ui(i1+4,i2+4,
     & i3+4,c3)+c(i,5,4,4)*ui(i1+5,i2+4,i3+4,c3)+c(i,6,4,4)*ui(i1+6,
     & i2+4,i3+4,c3)+c(i,7,4,4)*ui(i1+7,i2+4,i3+4,c3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,8,4,4)*ui(i1+8,i2+4,i3+4,c3)+c(i,0,5,4)*ui(i1,i2+
     & 5,i3+4,c3)+c(i,1,5,4)*ui(i1+1,i2+5,i3+4,c3)+c(i,2,5,4)*ui(i1+2,
     & i2+5,i3+4,c3)+c(i,3,5,4)*ui(i1+3,i2+5,i3+4,c3)+c(i,4,5,4)*ui(
     & i1+4,i2+5,i3+4,c3)+c(i,5,5,4)*ui(i1+5,i2+5,i3+4,c3)+c(i,6,5,4)*
     & ui(i1+6,i2+5,i3+4,c3)+c(i,7,5,4)*ui(i1+7,i2+5,i3+4,c3)+c(i,8,5,
     & 4)*ui(i1+8,i2+5,i3+4,c3)+c(i,0,6,4)*ui(i1,i2+6,i3+4,c3)+c(i,1,
     & 6,4)*ui(i1+1,i2+6,i3+4,c3)+c(i,2,6,4)*ui(i1+2,i2+6,i3+4,c3)+c(
     & i,3,6,4)*ui(i1+3,i2+6,i3+4,c3)+c(i,4,6,4)*ui(i1+4,i2+6,i3+4,c3)
     & +c(i,5,6,4)*ui(i1+5,i2+6,i3+4,c3)+c(i,6,6,4)*ui(i1+6,i2+6,i3+4,
     & c3)+c(i,7,6,4)*ui(i1+7,i2+6,i3+4,c3)+c(i,8,6,4)*ui(i1+8,i2+6,
     & i3+4,c3)+c(i,0,7,4)*ui(i1,i2+7,i3+4,c3)+c(i,1,7,4)*ui(i1+1,i2+
     & 7,i3+4,c3)+c(i,2,7,4)*ui(i1+2,i2+7,i3+4,c3)+c(i,3,7,4)*ui(i1+3,
     & i2+7,i3+4,c3)+c(i,4,7,4)*ui(i1+4,i2+7,i3+4,c3)+c(i,5,7,4)*ui(
     & i1+5,i2+7,i3+4,c3)+c(i,6,7,4)*ui(i1+6,i2+7,i3+4,c3)+c(i,7,7,4)*
     & ui(i1+7,i2+7,i3+4,c3)+c(i,8,7,4)*ui(i1+8,i2+7,i3+4,c3)+c(i,0,8,
     & 4)*ui(i1,i2+8,i3+4,c3)+c(i,1,8,4)*ui(i1+1,i2+8,i3+4,c3)+c(i,2,
     & 8,4)*ui(i1+2,i2+8,i3+4,c3)+c(i,3,8,4)*ui(i1+3,i2+8,i3+4,c3)+c(
     & i,4,8,4)*ui(i1+4,i2+8,i3+4,c3)+c(i,5,8,4)*ui(i1+5,i2+8,i3+4,c3)
     & +c(i,6,8,4)*ui(i1+6,i2+8,i3+4,c3)+c(i,7,8,4)*ui(i1+7,i2+8,i3+4,
     & c3)+c(i,8,8,4)*ui(i1+8,i2+8,i3+4,c3)+c(i,0,0,5)*ui(i1,i2,i3+5,
     & c3)+c(i,1,0,5)*ui(i1+1,i2,i3+5,c3)+c(i,2,0,5)*ui(i1+2,i2,i3+5,
     & c3)+c(i,3,0,5)*ui(i1+3,i2,i3+5,c3)+c(i,4,0,5)*ui(i1+4,i2,i3+5,
     & c3)+c(i,5,0,5)*ui(i1+5,i2,i3+5,c3)+c(i,6,0,5)*ui(i1+6,i2,i3+5,
     & c3)+c(i,7,0,5)*ui(i1+7,i2,i3+5,c3)+c(i,8,0,5)*ui(i1+8,i2,i3+5,
     & c3)+c(i,0,1,5)*ui(i1,i2+1,i3+5,c3)+c(i,1,1,5)*ui(i1+1,i2+1,i3+
     & 5,c3)+c(i,2,1,5)*ui(i1+2,i2+1,i3+5,c3)+c(i,3,1,5)*ui(i1+3,i2+1,
     & i3+5,c3)+c(i,4,1,5)*ui(i1+4,i2+1,i3+5,c3)+c(i,5,1,5)*ui(i1+5,
     & i2+1,i3+5,c3)+c(i,6,1,5)*ui(i1+6,i2+1,i3+5,c3)+c(i,7,1,5)*ui(
     & i1+7,i2+1,i3+5,c3)+c(i,8,1,5)*ui(i1+8,i2+1,i3+5,c3)+c(i,0,2,5)*
     & ui(i1,i2+2,i3+5,c3)+c(i,1,2,5)*ui(i1+1,i2+2,i3+5,c3)+c(i,2,2,5)
     & *ui(i1+2,i2+2,i3+5,c3)+c(i,3,2,5)*ui(i1+3,i2+2,i3+5,c3)+c(i,4,
     & 2,5)*ui(i1+4,i2+2,i3+5,c3)+c(i,5,2,5)*ui(i1+5,i2+2,i3+5,c3)+c(
     & i,6,2,5)*ui(i1+6,i2+2,i3+5,c3)+c(i,7,2,5)*ui(i1+7,i2+2,i3+5,c3)
     & +c(i,8,2,5)*ui(i1+8,i2+2,i3+5,c3)+c(i,0,3,5)*ui(i1,i2+3,i3+5,
     & c3)+c(i,1,3,5)*ui(i1+1,i2+3,i3+5,c3)+c(i,2,3,5)*ui(i1+2,i2+3,
     & i3+5,c3)+c(i,3,3,5)*ui(i1+3,i2+3,i3+5,c3)+c(i,4,3,5)*ui(i1+4,
     & i2+3,i3+5,c3)+c(i,5,3,5)*ui(i1+5,i2+3,i3+5,c3)+c(i,6,3,5)*ui(
     & i1+6,i2+3,i3+5,c3)+c(i,7,3,5)*ui(i1+7,i2+3,i3+5,c3)+c(i,8,3,5)*
     & ui(i1+8,i2+3,i3+5,c3)+c(i,0,4,5)*ui(i1,i2+4,i3+5,c3)+c(i,1,4,5)
     & *ui(i1+1,i2+4,i3+5,c3)+c(i,2,4,5)*ui(i1+2,i2+4,i3+5,c3)+c(i,3,
     & 4,5)*ui(i1+3,i2+4,i3+5,c3)+c(i,4,4,5)*ui(i1+4,i2+4,i3+5,c3)+c(
     & i,5,4,5)*ui(i1+5,i2+4,i3+5,c3)+c(i,6,4,5)*ui(i1+6,i2+4,i3+5,c3)
     & +c(i,7,4,5)*ui(i1+7,i2+4,i3+5,c3)+c(i,8,4,5)*ui(i1+8,i2+4,i3+5,
     & c3)+c(i,0,5,5)*ui(i1,i2+5,i3+5,c3)+c(i,1,5,5)*ui(i1+1,i2+5,i3+
     & 5,c3)+c(i,2,5,5)*ui(i1+2,i2+5,i3+5,c3)+c(i,3,5,5)*ui(i1+3,i2+5,
     & i3+5,c3)+c(i,4,5,5)*ui(i1+4,i2+5,i3+5,c3)+c(i,5,5,5)*ui(i1+5,
     & i2+5,i3+5,c3)+c(i,6,5,5)*ui(i1+6,i2+5,i3+5,c3)+c(i,7,5,5)*ui(
     & i1+7,i2+5,i3+5,c3)+c(i,8,5,5)*ui(i1+8,i2+5,i3+5,c3)+c(i,0,6,5)*
     & ui(i1,i2+6,i3+5,c3)+c(i,1,6,5)*ui(i1+1,i2+6,i3+5,c3)+c(i,2,6,5)
     & *ui(i1+2,i2+6,i3+5,c3)+c(i,3,6,5)*ui(i1+3,i2+6,i3+5,c3)+c(i,4,
     & 6,5)*ui(i1+4,i2+6,i3+5,c3)+c(i,5,6,5)*ui(i1+5,i2+6,i3+5,c3)+c(
     & i,6,6,5)*ui(i1+6,i2+6,i3+5,c3)+c(i,7,6,5)*ui(i1+7,i2+6,i3+5,c3)
     & +c(i,8,6,5)*ui(i1+8,i2+6,i3+5,c3)+c(i,0,7,5)*ui(i1,i2+7,i3+5,
     & c3)+c(i,1,7,5)*ui(i1+1,i2+7,i3+5,c3)+c(i,2,7,5)*ui(i1+2,i2+7,
     & i3+5,c3)+c(i,3,7,5)*ui(i1+3,i2+7,i3+5,c3)+c(i,4,7,5)*ui(i1+4,
     & i2+7,i3+5,c3)+c(i,5,7,5)*ui(i1+5,i2+7,i3+5,c3)+c(i,6,7,5)*ui(
     & i1+6,i2+7,i3+5,c3)+c(i,7,7,5)*ui(i1+7,i2+7,i3+5,c3)+c(i,8,7,5)*
     & ui(i1+8,i2+7,i3+5,c3)+c(i,0,8,5)*ui(i1,i2+8,i3+5,c3)+c(i,1,8,5)
     & *ui(i1+1,i2+8,i3+5,c3)+c(i,2,8,5)*ui(i1+2,i2+8,i3+5,c3)+c(i,3,
     & 8,5)*ui(i1+3,i2+8,i3+5,c3)+c(i,4,8,5)*ui(i1+4,i2+8,i3+5,c3)+c(
     & i,5,8,5)*ui(i1+5,i2+8,i3+5,c3)+c(i,6,8,5)*ui(i1+6,i2+8,i3+5,c3)
     & +c(i,7,8,5)*ui(i1+7,i2+8,i3+5,c3)+c(i,8,8,5)*ui(i1+8,i2+8,i3+5,
     & c3)+c(i,0,0,6)*ui(i1,i2,i3+6,c3)+c(i,1,0,6)*ui(i1+1,i2,i3+6,c3)
     & +c(i,2,0,6)*ui(i1+2,i2,i3+6,c3)+c(i,3,0,6)*ui(i1+3,i2,i3+6,c3)+
     & c(i,4,0,6)*ui(i1+4,i2,i3+6,c3)+c(i,5,0,6)*ui(i1+5,i2,i3+6,c3)+
     & c(i,6,0,6)*ui(i1+6,i2,i3+6,c3)+c(i,7,0,6)*ui(i1+7,i2,i3+6,c3)+
     & c(i,8,0,6)*ui(i1+8,i2,i3+6,c3)+c(i,0,1,6)*ui(i1,i2+1,i3+6,c3)+
     & c(i,1,1,6)*ui(i1+1,i2+1,i3+6,c3)+c(i,2,1,6)*ui(i1+2,i2+1,i3+6,
     & c3)+c(i,3,1,6)*ui(i1+3,i2+1,i3+6,c3)+c(i,4,1,6)*ui(i1+4,i2+1,
     & i3+6,c3)+c(i,5,1,6)*ui(i1+5,i2+1,i3+6,c3)+c(i,6,1,6)*ui(i1+6,
     & i2+1,i3+6,c3)+c(i,7,1,6)*ui(i1+7,i2+1,i3+6,c3)+c(i,8,1,6)*ui(
     & i1+8,i2+1,i3+6,c3)+c(i,0,2,6)*ui(i1,i2+2,i3+6,c3)+c(i,1,2,6)*
     & ui(i1+1,i2+2,i3+6,c3)+c(i,2,2,6)*ui(i1+2,i2+2,i3+6,c3)+c(i,3,2,
     & 6)*ui(i1+3,i2+2,i3+6,c3)+c(i,4,2,6)*ui(i1+4,i2+2,i3+6,c3)+c(i,
     & 5,2,6)*ui(i1+5,i2+2,i3+6,c3)+c(i,6,2,6)*ui(i1+6,i2+2,i3+6,c3)+
     & c(i,7,2,6)*ui(i1+7,i2+2,i3+6,c3)+c(i,8,2,6)*ui(i1+8,i2+2,i3+6,
     & c3)+c(i,0,3,6)*ui(i1,i2+3,i3+6,c3)+c(i,1,3,6)*ui(i1+1,i2+3,i3+
     & 6,c3)+c(i,2,3,6)*ui(i1+2,i2+3,i3+6,c3)+c(i,3,3,6)*ui(i1+3,i2+3,
     & i3+6,c3)+c(i,4,3,6)*ui(i1+4,i2+3,i3+6,c3)+c(i,5,3,6)*ui(i1+5,
     & i2+3,i3+6,c3)+c(i,6,3,6)*ui(i1+6,i2+3,i3+6,c3)+c(i,7,3,6)*ui(
     & i1+7,i2+3,i3+6,c3)+c(i,8,3,6)*ui(i1+8,i2+3,i3+6,c3)+c(i,0,4,6)*
     & ui(i1,i2+4,i3+6,c3)+c(i,1,4,6)*ui(i1+1,i2+4,i3+6,c3)+c(i,2,4,6)
     & *ui(i1+2,i2+4,i3+6,c3)+c(i,3,4,6)*ui(i1+3,i2+4,i3+6,c3)+c(i,4,
     & 4,6)*ui(i1+4,i2+4,i3+6,c3)+c(i,5,4,6)*ui(i1+5,i2+4,i3+6,c3)+c(
     & i,6,4,6)*ui(i1+6,i2+4,i3+6,c3)+c(i,7,4,6)*ui(i1+7,i2+4,i3+6,c3)
     & +c(i,8,4,6)*ui(i1+8,i2+4,i3+6,c3)+c(i,0,5,6)*ui(i1,i2+5,i3+6,
     & c3)+c(i,1,5,6)*ui(i1+1,i2+5,i3+6,c3)+c(i,2,5,6)*ui(i1+2,i2+5,
     & i3+6,c3)+c(i,3,5,6)*ui(i1+3,i2+5,i3+6,c3)+c(i,4,5,6)*ui(i1+4,
     & i2+5,i3+6,c3)+c(i,5,5,6)*ui(i1+5,i2+5,i3+6,c3)+c(i,6,5,6)*ui(
     & i1+6,i2+5,i3+6,c3)+c(i,7,5,6)*ui(i1+7,i2+5,i3+6,c3)+c(i,8,5,6)*
     & ui(i1+8,i2+5,i3+6,c3)+c(i,0,6,6)*ui(i1,i2+6,i3+6,c3)+c(i,1,6,6)
     & *ui(i1+1,i2+6,i3+6,c3)+c(i,2,6,6)*ui(i1+2,i2+6,i3+6,c3)+c(i,3,
     & 6,6)*ui(i1+3,i2+6,i3+6,c3)+c(i,4,6,6)*ui(i1+4,i2+6,i3+6,c3)+c(
     & i,5,6,6)*ui(i1+5,i2+6,i3+6,c3)+c(i,6,6,6)*ui(i1+6,i2+6,i3+6,c3)
     & +c(i,7,6,6)*ui(i1+7,i2+6,i3+6,c3)+c(i,8,6,6)*ui(i1+8,i2+6,i3+6,
     & c3)+c(i,0,7,6)*ui(i1,i2+7,i3+6,c3)+c(i,1,7,6)*ui(i1+1,i2+7,i3+
     & 6,c3)+c(i,2,7,6)*ui(i1+2,i2+7,i3+6,c3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,3,7,6)*ui(i1+3,i2+7,i3+6,c3)+c(i,4,7,6)*ui(i1+4,
     & i2+7,i3+6,c3)+c(i,5,7,6)*ui(i1+5,i2+7,i3+6,c3)+c(i,6,7,6)*ui(
     & i1+6,i2+7,i3+6,c3)+c(i,7,7,6)*ui(i1+7,i2+7,i3+6,c3)+c(i,8,7,6)*
     & ui(i1+8,i2+7,i3+6,c3)+c(i,0,8,6)*ui(i1,i2+8,i3+6,c3)+c(i,1,8,6)
     & *ui(i1+1,i2+8,i3+6,c3)+c(i,2,8,6)*ui(i1+2,i2+8,i3+6,c3)+c(i,3,
     & 8,6)*ui(i1+3,i2+8,i3+6,c3)+c(i,4,8,6)*ui(i1+4,i2+8,i3+6,c3)+c(
     & i,5,8,6)*ui(i1+5,i2+8,i3+6,c3)+c(i,6,8,6)*ui(i1+6,i2+8,i3+6,c3)
     & +c(i,7,8,6)*ui(i1+7,i2+8,i3+6,c3)+c(i,8,8,6)*ui(i1+8,i2+8,i3+6,
     & c3)+c(i,0,0,7)*ui(i1,i2,i3+7,c3)+c(i,1,0,7)*ui(i1+1,i2,i3+7,c3)
     & +c(i,2,0,7)*ui(i1+2,i2,i3+7,c3)+c(i,3,0,7)*ui(i1+3,i2,i3+7,c3)+
     & c(i,4,0,7)*ui(i1+4,i2,i3+7,c3)+c(i,5,0,7)*ui(i1+5,i2,i3+7,c3)+
     & c(i,6,0,7)*ui(i1+6,i2,i3+7,c3)+c(i,7,0,7)*ui(i1+7,i2,i3+7,c3)+
     & c(i,8,0,7)*ui(i1+8,i2,i3+7,c3)+c(i,0,1,7)*ui(i1,i2+1,i3+7,c3)+
     & c(i,1,1,7)*ui(i1+1,i2+1,i3+7,c3)+c(i,2,1,7)*ui(i1+2,i2+1,i3+7,
     & c3)+c(i,3,1,7)*ui(i1+3,i2+1,i3+7,c3)+c(i,4,1,7)*ui(i1+4,i2+1,
     & i3+7,c3)+c(i,5,1,7)*ui(i1+5,i2+1,i3+7,c3)+c(i,6,1,7)*ui(i1+6,
     & i2+1,i3+7,c3)+c(i,7,1,7)*ui(i1+7,i2+1,i3+7,c3)+c(i,8,1,7)*ui(
     & i1+8,i2+1,i3+7,c3)+c(i,0,2,7)*ui(i1,i2+2,i3+7,c3)+c(i,1,2,7)*
     & ui(i1+1,i2+2,i3+7,c3)+c(i,2,2,7)*ui(i1+2,i2+2,i3+7,c3)+c(i,3,2,
     & 7)*ui(i1+3,i2+2,i3+7,c3)+c(i,4,2,7)*ui(i1+4,i2+2,i3+7,c3)+c(i,
     & 5,2,7)*ui(i1+5,i2+2,i3+7,c3)+c(i,6,2,7)*ui(i1+6,i2+2,i3+7,c3)+
     & c(i,7,2,7)*ui(i1+7,i2+2,i3+7,c3)+c(i,8,2,7)*ui(i1+8,i2+2,i3+7,
     & c3)+c(i,0,3,7)*ui(i1,i2+3,i3+7,c3)+c(i,1,3,7)*ui(i1+1,i2+3,i3+
     & 7,c3)+c(i,2,3,7)*ui(i1+2,i2+3,i3+7,c3)+c(i,3,3,7)*ui(i1+3,i2+3,
     & i3+7,c3)+c(i,4,3,7)*ui(i1+4,i2+3,i3+7,c3)+c(i,5,3,7)*ui(i1+5,
     & i2+3,i3+7,c3)+c(i,6,3,7)*ui(i1+6,i2+3,i3+7,c3)+c(i,7,3,7)*ui(
     & i1+7,i2+3,i3+7,c3)+c(i,8,3,7)*ui(i1+8,i2+3,i3+7,c3)+c(i,0,4,7)*
     & ui(i1,i2+4,i3+7,c3)+c(i,1,4,7)*ui(i1+1,i2+4,i3+7,c3)+c(i,2,4,7)
     & *ui(i1+2,i2+4,i3+7,c3)+c(i,3,4,7)*ui(i1+3,i2+4,i3+7,c3)+c(i,4,
     & 4,7)*ui(i1+4,i2+4,i3+7,c3)+c(i,5,4,7)*ui(i1+5,i2+4,i3+7,c3)+c(
     & i,6,4,7)*ui(i1+6,i2+4,i3+7,c3)+c(i,7,4,7)*ui(i1+7,i2+4,i3+7,c3)
     & +c(i,8,4,7)*ui(i1+8,i2+4,i3+7,c3)+c(i,0,5,7)*ui(i1,i2+5,i3+7,
     & c3)+c(i,1,5,7)*ui(i1+1,i2+5,i3+7,c3)+c(i,2,5,7)*ui(i1+2,i2+5,
     & i3+7,c3)+c(i,3,5,7)*ui(i1+3,i2+5,i3+7,c3)+c(i,4,5,7)*ui(i1+4,
     & i2+5,i3+7,c3)+c(i,5,5,7)*ui(i1+5,i2+5,i3+7,c3)+c(i,6,5,7)*ui(
     & i1+6,i2+5,i3+7,c3)+c(i,7,5,7)*ui(i1+7,i2+5,i3+7,c3)+c(i,8,5,7)*
     & ui(i1+8,i2+5,i3+7,c3)+c(i,0,6,7)*ui(i1,i2+6,i3+7,c3)+c(i,1,6,7)
     & *ui(i1+1,i2+6,i3+7,c3)+c(i,2,6,7)*ui(i1+2,i2+6,i3+7,c3)+c(i,3,
     & 6,7)*ui(i1+3,i2+6,i3+7,c3)+c(i,4,6,7)*ui(i1+4,i2+6,i3+7,c3)+c(
     & i,5,6,7)*ui(i1+5,i2+6,i3+7,c3)+c(i,6,6,7)*ui(i1+6,i2+6,i3+7,c3)
     & +c(i,7,6,7)*ui(i1+7,i2+6,i3+7,c3)+c(i,8,6,7)*ui(i1+8,i2+6,i3+7,
     & c3)+c(i,0,7,7)*ui(i1,i2+7,i3+7,c3)+c(i,1,7,7)*ui(i1+1,i2+7,i3+
     & 7,c3)+c(i,2,7,7)*ui(i1+2,i2+7,i3+7,c3)+c(i,3,7,7)*ui(i1+3,i2+7,
     & i3+7,c3)+c(i,4,7,7)*ui(i1+4,i2+7,i3+7,c3)+c(i,5,7,7)*ui(i1+5,
     & i2+7,i3+7,c3)+c(i,6,7,7)*ui(i1+6,i2+7,i3+7,c3)+c(i,7,7,7)*ui(
     & i1+7,i2+7,i3+7,c3)+c(i,8,7,7)*ui(i1+8,i2+7,i3+7,c3)+c(i,0,8,7)*
     & ui(i1,i2+8,i3+7,c3)+c(i,1,8,7)*ui(i1+1,i2+8,i3+7,c3)+c(i,2,8,7)
     & *ui(i1+2,i2+8,i3+7,c3)+c(i,3,8,7)*ui(i1+3,i2+8,i3+7,c3)+c(i,4,
     & 8,7)*ui(i1+4,i2+8,i3+7,c3)+c(i,5,8,7)*ui(i1+5,i2+8,i3+7,c3)+c(
     & i,6,8,7)*ui(i1+6,i2+8,i3+7,c3)+c(i,7,8,7)*ui(i1+7,i2+8,i3+7,c3)
     & +c(i,8,8,7)*ui(i1+8,i2+8,i3+7,c3)+c(i,0,0,8)*ui(i1,i2,i3+8,c3)+
     & c(i,1,0,8)*ui(i1+1,i2,i3+8,c3)+c(i,2,0,8)*ui(i1+2,i2,i3+8,c3)+
     & c(i,3,0,8)*ui(i1+3,i2,i3+8,c3)+c(i,4,0,8)*ui(i1+4,i2,i3+8,c3)+
     & c(i,5,0,8)*ui(i1+5,i2,i3+8,c3)+c(i,6,0,8)*ui(i1+6,i2,i3+8,c3)+
     & c(i,7,0,8)*ui(i1+7,i2,i3+8,c3)+c(i,8,0,8)*ui(i1+8,i2,i3+8,c3)+
     & c(i,0,1,8)*ui(i1,i2+1,i3+8,c3)+c(i,1,1,8)*ui(i1+1,i2+1,i3+8,c3)
     & +c(i,2,1,8)*ui(i1+2,i2+1,i3+8,c3)+c(i,3,1,8)*ui(i1+3,i2+1,i3+8,
     & c3)+c(i,4,1,8)*ui(i1+4,i2+1,i3+8,c3)+c(i,5,1,8)*ui(i1+5,i2+1,
     & i3+8,c3)+c(i,6,1,8)*ui(i1+6,i2+1,i3+8,c3)+c(i,7,1,8)*ui(i1+7,
     & i2+1,i3+8,c3)+c(i,8,1,8)*ui(i1+8,i2+1,i3+8,c3)+c(i,0,2,8)*ui(
     & i1,i2+2,i3+8,c3)+c(i,1,2,8)*ui(i1+1,i2+2,i3+8,c3)+c(i,2,2,8)*
     & ui(i1+2,i2+2,i3+8,c3)+c(i,3,2,8)*ui(i1+3,i2+2,i3+8,c3)+c(i,4,2,
     & 8)*ui(i1+4,i2+2,i3+8,c3)+c(i,5,2,8)*ui(i1+5,i2+2,i3+8,c3)+c(i,
     & 6,2,8)*ui(i1+6,i2+2,i3+8,c3)+c(i,7,2,8)*ui(i1+7,i2+2,i3+8,c3)+
     & c(i,8,2,8)*ui(i1+8,i2+2,i3+8,c3)+c(i,0,3,8)*ui(i1,i2+3,i3+8,c3)
     & +c(i,1,3,8)*ui(i1+1,i2+3,i3+8,c3)+c(i,2,3,8)*ui(i1+2,i2+3,i3+8,
     & c3)+c(i,3,3,8)*ui(i1+3,i2+3,i3+8,c3)+c(i,4,3,8)*ui(i1+4,i2+3,
     & i3+8,c3)+c(i,5,3,8)*ui(i1+5,i2+3,i3+8,c3)+c(i,6,3,8)*ui(i1+6,
     & i2+3,i3+8,c3)+c(i,7,3,8)*ui(i1+7,i2+3,i3+8,c3)+c(i,8,3,8)*ui(
     & i1+8,i2+3,i3+8,c3)+c(i,0,4,8)*ui(i1,i2+4,i3+8,c3)+c(i,1,4,8)*
     & ui(i1+1,i2+4,i3+8,c3)+c(i,2,4,8)*ui(i1+2,i2+4,i3+8,c3)+c(i,3,4,
     & 8)*ui(i1+3,i2+4,i3+8,c3)+c(i,4,4,8)*ui(i1+4,i2+4,i3+8,c3)+c(i,
     & 5,4,8)*ui(i1+5,i2+4,i3+8,c3)+c(i,6,4,8)*ui(i1+6,i2+4,i3+8,c3)+
     & c(i,7,4,8)*ui(i1+7,i2+4,i3+8,c3)+c(i,8,4,8)*ui(i1+8,i2+4,i3+8,
     & c3)+c(i,0,5,8)*ui(i1,i2+5,i3+8,c3)+c(i,1,5,8)*ui(i1+1,i2+5,i3+
     & 8,c3)+c(i,2,5,8)*ui(i1+2,i2+5,i3+8,c3)+c(i,3,5,8)*ui(i1+3,i2+5,
     & i3+8,c3)+c(i,4,5,8)*ui(i1+4,i2+5,i3+8,c3)+c(i,5,5,8)*ui(i1+5,
     & i2+5,i3+8,c3)+c(i,6,5,8)*ui(i1+6,i2+5,i3+8,c3)+c(i,7,5,8)*ui(
     & i1+7,i2+5,i3+8,c3)+c(i,8,5,8)*ui(i1+8,i2+5,i3+8,c3)+c(i,0,6,8)*
     & ui(i1,i2+6,i3+8,c3)+c(i,1,6,8)*ui(i1+1,i2+6,i3+8,c3)+c(i,2,6,8)
     & *ui(i1+2,i2+6,i3+8,c3)+c(i,3,6,8)*ui(i1+3,i2+6,i3+8,c3)+c(i,4,
     & 6,8)*ui(i1+4,i2+6,i3+8,c3)+c(i,5,6,8)*ui(i1+5,i2+6,i3+8,c3)+c(
     & i,6,6,8)*ui(i1+6,i2+6,i3+8,c3)+c(i,7,6,8)*ui(i1+7,i2+6,i3+8,c3)
     & +c(i,8,6,8)*ui(i1+8,i2+6,i3+8,c3)+c(i,0,7,8)*ui(i1,i2+7,i3+8,
     & c3)+c(i,1,7,8)*ui(i1+1,i2+7,i3+8,c3)+c(i,2,7,8)*ui(i1+2,i2+7,
     & i3+8,c3)+c(i,3,7,8)*ui(i1+3,i2+7,i3+8,c3)+c(i,4,7,8)*ui(i1+4,
     & i2+7,i3+8,c3)+c(i,5,7,8)*ui(i1+5,i2+7,i3+8,c3)+c(i,6,7,8)*ui(
     & i1+6,i2+7,i3+8,c3)+c(i,7,7,8)*ui(i1+7,i2+7,i3+8,c3)+c(i,8,7,8)*
     & ui(i1+8,i2+7,i3+8,c3)+c(i,0,8,8)*ui(i1,i2+8,i3+8,c3)+c(i,1,8,8)
     & *ui(i1+1,i2+8,i3+8,c3)+c(i,2,8,8)*ui(i1+2,i2+8,i3+8,c3)+c(i,3,
     & 8,8)*ui(i1+3,i2+8,i3+8,c3)+c(i,4,8,8)*ui(i1+4,i2+8,i3+8,c3)+c(
     & i,5,8,8)*ui(i1+5,i2+8,i3+8,c3)+c(i,6,8,8)*ui(i1+6,i2+8,i3+8,c3)
     & +c(i,7,8,8)*ui(i1+7,i2+8,i3+8,c3)+c(i,8,8,8)*ui(i1+8,i2+8,i3+8,
     & c3)


              end do
              end do
            else
              ! put "c" loop as inner loop, this seems to be faster
              do i=nia,nib
              do c3=c3a,c3b
! interp999(ug(ip(i,1),ip(i,2),ip(i,3),c3))
                i1=il(i,1)
                i2=il(i,2)
                i3=il(i,3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = c(i,0,0,0)*ui(i1,i2,
     & i3,c3)+c(i,1,0,0)*ui(i1+1,i2,i3,c3)+c(i,2,0,0)*ui(i1+2,i2,i3,
     & c3)+c(i,3,0,0)*ui(i1+3,i2,i3,c3)+c(i,4,0,0)*ui(i1+4,i2,i3,c3)+
     & c(i,5,0,0)*ui(i1+5,i2,i3,c3)+c(i,6,0,0)*ui(i1+6,i2,i3,c3)+c(i,
     & 7,0,0)*ui(i1+7,i2,i3,c3)+c(i,8,0,0)*ui(i1+8,i2,i3,c3)+c(i,0,1,
     & 0)*ui(i1,i2+1,i3,c3)+c(i,1,1,0)*ui(i1+1,i2+1,i3,c3)+c(i,2,1,0)*
     & ui(i1+2,i2+1,i3,c3)+c(i,3,1,0)*ui(i1+3,i2+1,i3,c3)+c(i,4,1,0)*
     & ui(i1+4,i2+1,i3,c3)+c(i,5,1,0)*ui(i1+5,i2+1,i3,c3)+c(i,6,1,0)*
     & ui(i1+6,i2+1,i3,c3)+c(i,7,1,0)*ui(i1+7,i2+1,i3,c3)+c(i,8,1,0)*
     & ui(i1+8,i2+1,i3,c3)+c(i,0,2,0)*ui(i1,i2+2,i3,c3)+c(i,1,2,0)*ui(
     & i1+1,i2+2,i3,c3)+c(i,2,2,0)*ui(i1+2,i2+2,i3,c3)+c(i,3,2,0)*ui(
     & i1+3,i2+2,i3,c3)+c(i,4,2,0)*ui(i1+4,i2+2,i3,c3)+c(i,5,2,0)*ui(
     & i1+5,i2+2,i3,c3)+c(i,6,2,0)*ui(i1+6,i2+2,i3,c3)+c(i,7,2,0)*ui(
     & i1+7,i2+2,i3,c3)+c(i,8,2,0)*ui(i1+8,i2+2,i3,c3)+c(i,0,3,0)*ui(
     & i1,i2+3,i3,c3)+c(i,1,3,0)*ui(i1+1,i2+3,i3,c3)+c(i,2,3,0)*ui(i1+
     & 2,i2+3,i3,c3)+c(i,3,3,0)*ui(i1+3,i2+3,i3,c3)+c(i,4,3,0)*ui(i1+
     & 4,i2+3,i3,c3)+c(i,5,3,0)*ui(i1+5,i2+3,i3,c3)+c(i,6,3,0)*ui(i1+
     & 6,i2+3,i3,c3)+c(i,7,3,0)*ui(i1+7,i2+3,i3,c3)+c(i,8,3,0)*ui(i1+
     & 8,i2+3,i3,c3)+c(i,0,4,0)*ui(i1,i2+4,i3,c3)+c(i,1,4,0)*ui(i1+1,
     & i2+4,i3,c3)+c(i,2,4,0)*ui(i1+2,i2+4,i3,c3)+c(i,3,4,0)*ui(i1+3,
     & i2+4,i3,c3)+c(i,4,4,0)*ui(i1+4,i2+4,i3,c3)+c(i,5,4,0)*ui(i1+5,
     & i2+4,i3,c3)+c(i,6,4,0)*ui(i1+6,i2+4,i3,c3)+c(i,7,4,0)*ui(i1+7,
     & i2+4,i3,c3)+c(i,8,4,0)*ui(i1+8,i2+4,i3,c3)+c(i,0,5,0)*ui(i1,i2+
     & 5,i3,c3)+c(i,1,5,0)*ui(i1+1,i2+5,i3,c3)+c(i,2,5,0)*ui(i1+2,i2+
     & 5,i3,c3)+c(i,3,5,0)*ui(i1+3,i2+5,i3,c3)+c(i,4,5,0)*ui(i1+4,i2+
     & 5,i3,c3)+c(i,5,5,0)*ui(i1+5,i2+5,i3,c3)+c(i,6,5,0)*ui(i1+6,i2+
     & 5,i3,c3)+c(i,7,5,0)*ui(i1+7,i2+5,i3,c3)+c(i,8,5,0)*ui(i1+8,i2+
     & 5,i3,c3)+c(i,0,6,0)*ui(i1,i2+6,i3,c3)+c(i,1,6,0)*ui(i1+1,i2+6,
     & i3,c3)+c(i,2,6,0)*ui(i1+2,i2+6,i3,c3)+c(i,3,6,0)*ui(i1+3,i2+6,
     & i3,c3)+c(i,4,6,0)*ui(i1+4,i2+6,i3,c3)+c(i,5,6,0)*ui(i1+5,i2+6,
     & i3,c3)+c(i,6,6,0)*ui(i1+6,i2+6,i3,c3)+c(i,7,6,0)*ui(i1+7,i2+6,
     & i3,c3)+c(i,8,6,0)*ui(i1+8,i2+6,i3,c3)+c(i,0,7,0)*ui(i1,i2+7,i3,
     & c3)+c(i,1,7,0)*ui(i1+1,i2+7,i3,c3)+c(i,2,7,0)*ui(i1+2,i2+7,i3,
     & c3)+c(i,3,7,0)*ui(i1+3,i2+7,i3,c3)+c(i,4,7,0)*ui(i1+4,i2+7,i3,
     & c3)+c(i,5,7,0)*ui(i1+5,i2+7,i3,c3)+c(i,6,7,0)*ui(i1+6,i2+7,i3,
     & c3)+c(i,7,7,0)*ui(i1+7,i2+7,i3,c3)+c(i,8,7,0)*ui(i1+8,i2+7,i3,
     & c3)+c(i,0,8,0)*ui(i1,i2+8,i3,c3)+c(i,1,8,0)*ui(i1+1,i2+8,i3,c3)
     & +c(i,2,8,0)*ui(i1+2,i2+8,i3,c3)+c(i,3,8,0)*ui(i1+3,i2+8,i3,c3)+
     & c(i,4,8,0)*ui(i1+4,i2+8,i3,c3)+c(i,5,8,0)*ui(i1+5,i2+8,i3,c3)+
     & c(i,6,8,0)*ui(i1+6,i2+8,i3,c3)+c(i,7,8,0)*ui(i1+7,i2+8,i3,c3)+
     & c(i,8,8,0)*ui(i1+8,i2+8,i3,c3)+c(i,0,0,1)*ui(i1,i2,i3+1,c3)+c(
     & i,1,0,1)*ui(i1+1,i2,i3+1,c3)+c(i,2,0,1)*ui(i1+2,i2,i3+1,c3)+c(
     & i,3,0,1)*ui(i1+3,i2,i3+1,c3)+c(i,4,0,1)*ui(i1+4,i2,i3+1,c3)+c(
     & i,5,0,1)*ui(i1+5,i2,i3+1,c3)+c(i,6,0,1)*ui(i1+6,i2,i3+1,c3)+c(
     & i,7,0,1)*ui(i1+7,i2,i3+1,c3)+c(i,8,0,1)*ui(i1+8,i2,i3+1,c3)+c(
     & i,0,1,1)*ui(i1,i2+1,i3+1,c3)+c(i,1,1,1)*ui(i1+1,i2+1,i3+1,c3)+
     & c(i,2,1,1)*ui(i1+2,i2+1,i3+1,c3)+c(i,3,1,1)*ui(i1+3,i2+1,i3+1,
     & c3)+c(i,4,1,1)*ui(i1+4,i2+1,i3+1,c3)+c(i,5,1,1)*ui(i1+5,i2+1,
     & i3+1,c3)+c(i,6,1,1)*ui(i1+6,i2+1,i3+1,c3)+c(i,7,1,1)*ui(i1+7,
     & i2+1,i3+1,c3)+c(i,8,1,1)*ui(i1+8,i2+1,i3+1,c3)+c(i,0,2,1)*ui(
     & i1,i2+2,i3+1,c3)+c(i,1,2,1)*ui(i1+1,i2+2,i3+1,c3)+c(i,2,2,1)*
     & ui(i1+2,i2+2,i3+1,c3)+c(i,3,2,1)*ui(i1+3,i2+2,i3+1,c3)+c(i,4,2,
     & 1)*ui(i1+4,i2+2,i3+1,c3)+c(i,5,2,1)*ui(i1+5,i2+2,i3+1,c3)+c(i,
     & 6,2,1)*ui(i1+6,i2+2,i3+1,c3)+c(i,7,2,1)*ui(i1+7,i2+2,i3+1,c3)+
     & c(i,8,2,1)*ui(i1+8,i2+2,i3+1,c3)+c(i,0,3,1)*ui(i1,i2+3,i3+1,c3)
     & +c(i,1,3,1)*ui(i1+1,i2+3,i3+1,c3)+c(i,2,3,1)*ui(i1+2,i2+3,i3+1,
     & c3)+c(i,3,3,1)*ui(i1+3,i2+3,i3+1,c3)+c(i,4,3,1)*ui(i1+4,i2+3,
     & i3+1,c3)+c(i,5,3,1)*ui(i1+5,i2+3,i3+1,c3)+c(i,6,3,1)*ui(i1+6,
     & i2+3,i3+1,c3)+c(i,7,3,1)*ui(i1+7,i2+3,i3+1,c3)+c(i,8,3,1)*ui(
     & i1+8,i2+3,i3+1,c3)+c(i,0,4,1)*ui(i1,i2+4,i3+1,c3)+c(i,1,4,1)*
     & ui(i1+1,i2+4,i3+1,c3)+c(i,2,4,1)*ui(i1+2,i2+4,i3+1,c3)+c(i,3,4,
     & 1)*ui(i1+3,i2+4,i3+1,c3)+c(i,4,4,1)*ui(i1+4,i2+4,i3+1,c3)+c(i,
     & 5,4,1)*ui(i1+5,i2+4,i3+1,c3)+c(i,6,4,1)*ui(i1+6,i2+4,i3+1,c3)+
     & c(i,7,4,1)*ui(i1+7,i2+4,i3+1,c3)+c(i,8,4,1)*ui(i1+8,i2+4,i3+1,
     & c3)+c(i,0,5,1)*ui(i1,i2+5,i3+1,c3)+c(i,1,5,1)*ui(i1+1,i2+5,i3+
     & 1,c3)+c(i,2,5,1)*ui(i1+2,i2+5,i3+1,c3)+c(i,3,5,1)*ui(i1+3,i2+5,
     & i3+1,c3)+c(i,4,5,1)*ui(i1+4,i2+5,i3+1,c3)+c(i,5,5,1)*ui(i1+5,
     & i2+5,i3+1,c3)+c(i,6,5,1)*ui(i1+6,i2+5,i3+1,c3)+c(i,7,5,1)*ui(
     & i1+7,i2+5,i3+1,c3)+c(i,8,5,1)*ui(i1+8,i2+5,i3+1,c3)+c(i,0,6,1)*
     & ui(i1,i2+6,i3+1,c3)+c(i,1,6,1)*ui(i1+1,i2+6,i3+1,c3)+c(i,2,6,1)
     & *ui(i1+2,i2+6,i3+1,c3)+c(i,3,6,1)*ui(i1+3,i2+6,i3+1,c3)+c(i,4,
     & 6,1)*ui(i1+4,i2+6,i3+1,c3)+c(i,5,6,1)*ui(i1+5,i2+6,i3+1,c3)+c(
     & i,6,6,1)*ui(i1+6,i2+6,i3+1,c3)+c(i,7,6,1)*ui(i1+7,i2+6,i3+1,c3)
     & +c(i,8,6,1)*ui(i1+8,i2+6,i3+1,c3)+c(i,0,7,1)*ui(i1,i2+7,i3+1,
     & c3)+c(i,1,7,1)*ui(i1+1,i2+7,i3+1,c3)+c(i,2,7,1)*ui(i1+2,i2+7,
     & i3+1,c3)+c(i,3,7,1)*ui(i1+3,i2+7,i3+1,c3)+c(i,4,7,1)*ui(i1+4,
     & i2+7,i3+1,c3)+c(i,5,7,1)*ui(i1+5,i2+7,i3+1,c3)+c(i,6,7,1)*ui(
     & i1+6,i2+7,i3+1,c3)+c(i,7,7,1)*ui(i1+7,i2+7,i3+1,c3)+c(i,8,7,1)*
     & ui(i1+8,i2+7,i3+1,c3)+c(i,0,8,1)*ui(i1,i2+8,i3+1,c3)+c(i,1,8,1)
     & *ui(i1+1,i2+8,i3+1,c3)+c(i,2,8,1)*ui(i1+2,i2+8,i3+1,c3)+c(i,3,
     & 8,1)*ui(i1+3,i2+8,i3+1,c3)+c(i,4,8,1)*ui(i1+4,i2+8,i3+1,c3)+c(
     & i,5,8,1)*ui(i1+5,i2+8,i3+1,c3)+c(i,6,8,1)*ui(i1+6,i2+8,i3+1,c3)
     & +c(i,7,8,1)*ui(i1+7,i2+8,i3+1,c3)+c(i,8,8,1)*ui(i1+8,i2+8,i3+1,
     & c3)+c(i,0,0,2)*ui(i1,i2,i3+2,c3)+c(i,1,0,2)*ui(i1+1,i2,i3+2,c3)
     & +c(i,2,0,2)*ui(i1+2,i2,i3+2,c3)+c(i,3,0,2)*ui(i1+3,i2,i3+2,c3)+
     & c(i,4,0,2)*ui(i1+4,i2,i3+2,c3)+c(i,5,0,2)*ui(i1+5,i2,i3+2,c3)+
     & c(i,6,0,2)*ui(i1+6,i2,i3+2,c3)+c(i,7,0,2)*ui(i1+7,i2,i3+2,c3)+
     & c(i,8,0,2)*ui(i1+8,i2,i3+2,c3)+c(i,0,1,2)*ui(i1,i2+1,i3+2,c3)+
     & c(i,1,1,2)*ui(i1+1,i2+1,i3+2,c3)+c(i,2,1,2)*ui(i1+2,i2+1,i3+2,
     & c3)+c(i,3,1,2)*ui(i1+3,i2+1,i3+2,c3)+c(i,4,1,2)*ui(i1+4,i2+1,
     & i3+2,c3)+c(i,5,1,2)*ui(i1+5,i2+1,i3+2,c3)+c(i,6,1,2)*ui(i1+6,
     & i2+1,i3+2,c3)+c(i,7,1,2)*ui(i1+7,i2+1,i3+2,c3)+c(i,8,1,2)*ui(
     & i1+8,i2+1,i3+2,c3)+c(i,0,2,2)*ui(i1,i2+2,i3+2,c3)+c(i,1,2,2)*
     & ui(i1+1,i2+2,i3+2,c3)+c(i,2,2,2)*ui(i1+2,i2+2,i3+2,c3)+c(i,3,2,
     & 2)*ui(i1+3,i2+2,i3+2,c3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,4,2,2)*ui(i1+4,i2+2,i3+2,c3)+c(i,5,2,2)*ui(i1+5,
     & i2+2,i3+2,c3)+c(i,6,2,2)*ui(i1+6,i2+2,i3+2,c3)+c(i,7,2,2)*ui(
     & i1+7,i2+2,i3+2,c3)+c(i,8,2,2)*ui(i1+8,i2+2,i3+2,c3)+c(i,0,3,2)*
     & ui(i1,i2+3,i3+2,c3)+c(i,1,3,2)*ui(i1+1,i2+3,i3+2,c3)+c(i,2,3,2)
     & *ui(i1+2,i2+3,i3+2,c3)+c(i,3,3,2)*ui(i1+3,i2+3,i3+2,c3)+c(i,4,
     & 3,2)*ui(i1+4,i2+3,i3+2,c3)+c(i,5,3,2)*ui(i1+5,i2+3,i3+2,c3)+c(
     & i,6,3,2)*ui(i1+6,i2+3,i3+2,c3)+c(i,7,3,2)*ui(i1+7,i2+3,i3+2,c3)
     & +c(i,8,3,2)*ui(i1+8,i2+3,i3+2,c3)+c(i,0,4,2)*ui(i1,i2+4,i3+2,
     & c3)+c(i,1,4,2)*ui(i1+1,i2+4,i3+2,c3)+c(i,2,4,2)*ui(i1+2,i2+4,
     & i3+2,c3)+c(i,3,4,2)*ui(i1+3,i2+4,i3+2,c3)+c(i,4,4,2)*ui(i1+4,
     & i2+4,i3+2,c3)+c(i,5,4,2)*ui(i1+5,i2+4,i3+2,c3)+c(i,6,4,2)*ui(
     & i1+6,i2+4,i3+2,c3)+c(i,7,4,2)*ui(i1+7,i2+4,i3+2,c3)+c(i,8,4,2)*
     & ui(i1+8,i2+4,i3+2,c3)+c(i,0,5,2)*ui(i1,i2+5,i3+2,c3)+c(i,1,5,2)
     & *ui(i1+1,i2+5,i3+2,c3)+c(i,2,5,2)*ui(i1+2,i2+5,i3+2,c3)+c(i,3,
     & 5,2)*ui(i1+3,i2+5,i3+2,c3)+c(i,4,5,2)*ui(i1+4,i2+5,i3+2,c3)+c(
     & i,5,5,2)*ui(i1+5,i2+5,i3+2,c3)+c(i,6,5,2)*ui(i1+6,i2+5,i3+2,c3)
     & +c(i,7,5,2)*ui(i1+7,i2+5,i3+2,c3)+c(i,8,5,2)*ui(i1+8,i2+5,i3+2,
     & c3)+c(i,0,6,2)*ui(i1,i2+6,i3+2,c3)+c(i,1,6,2)*ui(i1+1,i2+6,i3+
     & 2,c3)+c(i,2,6,2)*ui(i1+2,i2+6,i3+2,c3)+c(i,3,6,2)*ui(i1+3,i2+6,
     & i3+2,c3)+c(i,4,6,2)*ui(i1+4,i2+6,i3+2,c3)+c(i,5,6,2)*ui(i1+5,
     & i2+6,i3+2,c3)+c(i,6,6,2)*ui(i1+6,i2+6,i3+2,c3)+c(i,7,6,2)*ui(
     & i1+7,i2+6,i3+2,c3)+c(i,8,6,2)*ui(i1+8,i2+6,i3+2,c3)+c(i,0,7,2)*
     & ui(i1,i2+7,i3+2,c3)+c(i,1,7,2)*ui(i1+1,i2+7,i3+2,c3)+c(i,2,7,2)
     & *ui(i1+2,i2+7,i3+2,c3)+c(i,3,7,2)*ui(i1+3,i2+7,i3+2,c3)+c(i,4,
     & 7,2)*ui(i1+4,i2+7,i3+2,c3)+c(i,5,7,2)*ui(i1+5,i2+7,i3+2,c3)+c(
     & i,6,7,2)*ui(i1+6,i2+7,i3+2,c3)+c(i,7,7,2)*ui(i1+7,i2+7,i3+2,c3)
     & +c(i,8,7,2)*ui(i1+8,i2+7,i3+2,c3)+c(i,0,8,2)*ui(i1,i2+8,i3+2,
     & c3)+c(i,1,8,2)*ui(i1+1,i2+8,i3+2,c3)+c(i,2,8,2)*ui(i1+2,i2+8,
     & i3+2,c3)+c(i,3,8,2)*ui(i1+3,i2+8,i3+2,c3)+c(i,4,8,2)*ui(i1+4,
     & i2+8,i3+2,c3)+c(i,5,8,2)*ui(i1+5,i2+8,i3+2,c3)+c(i,6,8,2)*ui(
     & i1+6,i2+8,i3+2,c3)+c(i,7,8,2)*ui(i1+7,i2+8,i3+2,c3)+c(i,8,8,2)*
     & ui(i1+8,i2+8,i3+2,c3)+c(i,0,0,3)*ui(i1,i2,i3+3,c3)+c(i,1,0,3)*
     & ui(i1+1,i2,i3+3,c3)+c(i,2,0,3)*ui(i1+2,i2,i3+3,c3)+c(i,3,0,3)*
     & ui(i1+3,i2,i3+3,c3)+c(i,4,0,3)*ui(i1+4,i2,i3+3,c3)+c(i,5,0,3)*
     & ui(i1+5,i2,i3+3,c3)+c(i,6,0,3)*ui(i1+6,i2,i3+3,c3)+c(i,7,0,3)*
     & ui(i1+7,i2,i3+3,c3)+c(i,8,0,3)*ui(i1+8,i2,i3+3,c3)+c(i,0,1,3)*
     & ui(i1,i2+1,i3+3,c3)+c(i,1,1,3)*ui(i1+1,i2+1,i3+3,c3)+c(i,2,1,3)
     & *ui(i1+2,i2+1,i3+3,c3)+c(i,3,1,3)*ui(i1+3,i2+1,i3+3,c3)+c(i,4,
     & 1,3)*ui(i1+4,i2+1,i3+3,c3)+c(i,5,1,3)*ui(i1+5,i2+1,i3+3,c3)+c(
     & i,6,1,3)*ui(i1+6,i2+1,i3+3,c3)+c(i,7,1,3)*ui(i1+7,i2+1,i3+3,c3)
     & +c(i,8,1,3)*ui(i1+8,i2+1,i3+3,c3)+c(i,0,2,3)*ui(i1,i2+2,i3+3,
     & c3)+c(i,1,2,3)*ui(i1+1,i2+2,i3+3,c3)+c(i,2,2,3)*ui(i1+2,i2+2,
     & i3+3,c3)+c(i,3,2,3)*ui(i1+3,i2+2,i3+3,c3)+c(i,4,2,3)*ui(i1+4,
     & i2+2,i3+3,c3)+c(i,5,2,3)*ui(i1+5,i2+2,i3+3,c3)+c(i,6,2,3)*ui(
     & i1+6,i2+2,i3+3,c3)+c(i,7,2,3)*ui(i1+7,i2+2,i3+3,c3)+c(i,8,2,3)*
     & ui(i1+8,i2+2,i3+3,c3)+c(i,0,3,3)*ui(i1,i2+3,i3+3,c3)+c(i,1,3,3)
     & *ui(i1+1,i2+3,i3+3,c3)+c(i,2,3,3)*ui(i1+2,i2+3,i3+3,c3)+c(i,3,
     & 3,3)*ui(i1+3,i2+3,i3+3,c3)+c(i,4,3,3)*ui(i1+4,i2+3,i3+3,c3)+c(
     & i,5,3,3)*ui(i1+5,i2+3,i3+3,c3)+c(i,6,3,3)*ui(i1+6,i2+3,i3+3,c3)
     & +c(i,7,3,3)*ui(i1+7,i2+3,i3+3,c3)+c(i,8,3,3)*ui(i1+8,i2+3,i3+3,
     & c3)+c(i,0,4,3)*ui(i1,i2+4,i3+3,c3)+c(i,1,4,3)*ui(i1+1,i2+4,i3+
     & 3,c3)+c(i,2,4,3)*ui(i1+2,i2+4,i3+3,c3)+c(i,3,4,3)*ui(i1+3,i2+4,
     & i3+3,c3)+c(i,4,4,3)*ui(i1+4,i2+4,i3+3,c3)+c(i,5,4,3)*ui(i1+5,
     & i2+4,i3+3,c3)+c(i,6,4,3)*ui(i1+6,i2+4,i3+3,c3)+c(i,7,4,3)*ui(
     & i1+7,i2+4,i3+3,c3)+c(i,8,4,3)*ui(i1+8,i2+4,i3+3,c3)+c(i,0,5,3)*
     & ui(i1,i2+5,i3+3,c3)+c(i,1,5,3)*ui(i1+1,i2+5,i3+3,c3)+c(i,2,5,3)
     & *ui(i1+2,i2+5,i3+3,c3)+c(i,3,5,3)*ui(i1+3,i2+5,i3+3,c3)+c(i,4,
     & 5,3)*ui(i1+4,i2+5,i3+3,c3)+c(i,5,5,3)*ui(i1+5,i2+5,i3+3,c3)+c(
     & i,6,5,3)*ui(i1+6,i2+5,i3+3,c3)+c(i,7,5,3)*ui(i1+7,i2+5,i3+3,c3)
     & +c(i,8,5,3)*ui(i1+8,i2+5,i3+3,c3)+c(i,0,6,3)*ui(i1,i2+6,i3+3,
     & c3)+c(i,1,6,3)*ui(i1+1,i2+6,i3+3,c3)+c(i,2,6,3)*ui(i1+2,i2+6,
     & i3+3,c3)+c(i,3,6,3)*ui(i1+3,i2+6,i3+3,c3)+c(i,4,6,3)*ui(i1+4,
     & i2+6,i3+3,c3)+c(i,5,6,3)*ui(i1+5,i2+6,i3+3,c3)+c(i,6,6,3)*ui(
     & i1+6,i2+6,i3+3,c3)+c(i,7,6,3)*ui(i1+7,i2+6,i3+3,c3)+c(i,8,6,3)*
     & ui(i1+8,i2+6,i3+3,c3)+c(i,0,7,3)*ui(i1,i2+7,i3+3,c3)+c(i,1,7,3)
     & *ui(i1+1,i2+7,i3+3,c3)+c(i,2,7,3)*ui(i1+2,i2+7,i3+3,c3)+c(i,3,
     & 7,3)*ui(i1+3,i2+7,i3+3,c3)+c(i,4,7,3)*ui(i1+4,i2+7,i3+3,c3)+c(
     & i,5,7,3)*ui(i1+5,i2+7,i3+3,c3)+c(i,6,7,3)*ui(i1+6,i2+7,i3+3,c3)
     & +c(i,7,7,3)*ui(i1+7,i2+7,i3+3,c3)+c(i,8,7,3)*ui(i1+8,i2+7,i3+3,
     & c3)+c(i,0,8,3)*ui(i1,i2+8,i3+3,c3)+c(i,1,8,3)*ui(i1+1,i2+8,i3+
     & 3,c3)+c(i,2,8,3)*ui(i1+2,i2+8,i3+3,c3)+c(i,3,8,3)*ui(i1+3,i2+8,
     & i3+3,c3)+c(i,4,8,3)*ui(i1+4,i2+8,i3+3,c3)+c(i,5,8,3)*ui(i1+5,
     & i2+8,i3+3,c3)+c(i,6,8,3)*ui(i1+6,i2+8,i3+3,c3)+c(i,7,8,3)*ui(
     & i1+7,i2+8,i3+3,c3)+c(i,8,8,3)*ui(i1+8,i2+8,i3+3,c3)+c(i,0,0,4)*
     & ui(i1,i2,i3+4,c3)+c(i,1,0,4)*ui(i1+1,i2,i3+4,c3)+c(i,2,0,4)*ui(
     & i1+2,i2,i3+4,c3)+c(i,3,0,4)*ui(i1+3,i2,i3+4,c3)+c(i,4,0,4)*ui(
     & i1+4,i2,i3+4,c3)+c(i,5,0,4)*ui(i1+5,i2,i3+4,c3)+c(i,6,0,4)*ui(
     & i1+6,i2,i3+4,c3)+c(i,7,0,4)*ui(i1+7,i2,i3+4,c3)+c(i,8,0,4)*ui(
     & i1+8,i2,i3+4,c3)+c(i,0,1,4)*ui(i1,i2+1,i3+4,c3)+c(i,1,1,4)*ui(
     & i1+1,i2+1,i3+4,c3)+c(i,2,1,4)*ui(i1+2,i2+1,i3+4,c3)+c(i,3,1,4)*
     & ui(i1+3,i2+1,i3+4,c3)+c(i,4,1,4)*ui(i1+4,i2+1,i3+4,c3)+c(i,5,1,
     & 4)*ui(i1+5,i2+1,i3+4,c3)+c(i,6,1,4)*ui(i1+6,i2+1,i3+4,c3)+c(i,
     & 7,1,4)*ui(i1+7,i2+1,i3+4,c3)+c(i,8,1,4)*ui(i1+8,i2+1,i3+4,c3)+
     & c(i,0,2,4)*ui(i1,i2+2,i3+4,c3)+c(i,1,2,4)*ui(i1+1,i2+2,i3+4,c3)
     & +c(i,2,2,4)*ui(i1+2,i2+2,i3+4,c3)+c(i,3,2,4)*ui(i1+3,i2+2,i3+4,
     & c3)+c(i,4,2,4)*ui(i1+4,i2+2,i3+4,c3)+c(i,5,2,4)*ui(i1+5,i2+2,
     & i3+4,c3)+c(i,6,2,4)*ui(i1+6,i2+2,i3+4,c3)+c(i,7,2,4)*ui(i1+7,
     & i2+2,i3+4,c3)+c(i,8,2,4)*ui(i1+8,i2+2,i3+4,c3)+c(i,0,3,4)*ui(
     & i1,i2+3,i3+4,c3)+c(i,1,3,4)*ui(i1+1,i2+3,i3+4,c3)+c(i,2,3,4)*
     & ui(i1+2,i2+3,i3+4,c3)+c(i,3,3,4)*ui(i1+3,i2+3,i3+4,c3)+c(i,4,3,
     & 4)*ui(i1+4,i2+3,i3+4,c3)+c(i,5,3,4)*ui(i1+5,i2+3,i3+4,c3)+c(i,
     & 6,3,4)*ui(i1+6,i2+3,i3+4,c3)+c(i,7,3,4)*ui(i1+7,i2+3,i3+4,c3)+
     & c(i,8,3,4)*ui(i1+8,i2+3,i3+4,c3)+c(i,0,4,4)*ui(i1,i2+4,i3+4,c3)
     & +c(i,1,4,4)*ui(i1+1,i2+4,i3+4,c3)+c(i,2,4,4)*ui(i1+2,i2+4,i3+4,
     & c3)+c(i,3,4,4)*ui(i1+3,i2+4,i3+4,c3)+c(i,4,4,4)*ui(i1+4,i2+4,
     & i3+4,c3)+c(i,5,4,4)*ui(i1+5,i2+4,i3+4,c3)+c(i,6,4,4)*ui(i1+6,
     & i2+4,i3+4,c3)+c(i,7,4,4)*ui(i1+7,i2+4,i3+4,c3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,8,4,4)*ui(i1+8,i2+4,i3+4,c3)+c(i,0,5,4)*ui(i1,i2+
     & 5,i3+4,c3)+c(i,1,5,4)*ui(i1+1,i2+5,i3+4,c3)+c(i,2,5,4)*ui(i1+2,
     & i2+5,i3+4,c3)+c(i,3,5,4)*ui(i1+3,i2+5,i3+4,c3)+c(i,4,5,4)*ui(
     & i1+4,i2+5,i3+4,c3)+c(i,5,5,4)*ui(i1+5,i2+5,i3+4,c3)+c(i,6,5,4)*
     & ui(i1+6,i2+5,i3+4,c3)+c(i,7,5,4)*ui(i1+7,i2+5,i3+4,c3)+c(i,8,5,
     & 4)*ui(i1+8,i2+5,i3+4,c3)+c(i,0,6,4)*ui(i1,i2+6,i3+4,c3)+c(i,1,
     & 6,4)*ui(i1+1,i2+6,i3+4,c3)+c(i,2,6,4)*ui(i1+2,i2+6,i3+4,c3)+c(
     & i,3,6,4)*ui(i1+3,i2+6,i3+4,c3)+c(i,4,6,4)*ui(i1+4,i2+6,i3+4,c3)
     & +c(i,5,6,4)*ui(i1+5,i2+6,i3+4,c3)+c(i,6,6,4)*ui(i1+6,i2+6,i3+4,
     & c3)+c(i,7,6,4)*ui(i1+7,i2+6,i3+4,c3)+c(i,8,6,4)*ui(i1+8,i2+6,
     & i3+4,c3)+c(i,0,7,4)*ui(i1,i2+7,i3+4,c3)+c(i,1,7,4)*ui(i1+1,i2+
     & 7,i3+4,c3)+c(i,2,7,4)*ui(i1+2,i2+7,i3+4,c3)+c(i,3,7,4)*ui(i1+3,
     & i2+7,i3+4,c3)+c(i,4,7,4)*ui(i1+4,i2+7,i3+4,c3)+c(i,5,7,4)*ui(
     & i1+5,i2+7,i3+4,c3)+c(i,6,7,4)*ui(i1+6,i2+7,i3+4,c3)+c(i,7,7,4)*
     & ui(i1+7,i2+7,i3+4,c3)+c(i,8,7,4)*ui(i1+8,i2+7,i3+4,c3)+c(i,0,8,
     & 4)*ui(i1,i2+8,i3+4,c3)+c(i,1,8,4)*ui(i1+1,i2+8,i3+4,c3)+c(i,2,
     & 8,4)*ui(i1+2,i2+8,i3+4,c3)+c(i,3,8,4)*ui(i1+3,i2+8,i3+4,c3)+c(
     & i,4,8,4)*ui(i1+4,i2+8,i3+4,c3)+c(i,5,8,4)*ui(i1+5,i2+8,i3+4,c3)
     & +c(i,6,8,4)*ui(i1+6,i2+8,i3+4,c3)+c(i,7,8,4)*ui(i1+7,i2+8,i3+4,
     & c3)+c(i,8,8,4)*ui(i1+8,i2+8,i3+4,c3)+c(i,0,0,5)*ui(i1,i2,i3+5,
     & c3)+c(i,1,0,5)*ui(i1+1,i2,i3+5,c3)+c(i,2,0,5)*ui(i1+2,i2,i3+5,
     & c3)+c(i,3,0,5)*ui(i1+3,i2,i3+5,c3)+c(i,4,0,5)*ui(i1+4,i2,i3+5,
     & c3)+c(i,5,0,5)*ui(i1+5,i2,i3+5,c3)+c(i,6,0,5)*ui(i1+6,i2,i3+5,
     & c3)+c(i,7,0,5)*ui(i1+7,i2,i3+5,c3)+c(i,8,0,5)*ui(i1+8,i2,i3+5,
     & c3)+c(i,0,1,5)*ui(i1,i2+1,i3+5,c3)+c(i,1,1,5)*ui(i1+1,i2+1,i3+
     & 5,c3)+c(i,2,1,5)*ui(i1+2,i2+1,i3+5,c3)+c(i,3,1,5)*ui(i1+3,i2+1,
     & i3+5,c3)+c(i,4,1,5)*ui(i1+4,i2+1,i3+5,c3)+c(i,5,1,5)*ui(i1+5,
     & i2+1,i3+5,c3)+c(i,6,1,5)*ui(i1+6,i2+1,i3+5,c3)+c(i,7,1,5)*ui(
     & i1+7,i2+1,i3+5,c3)+c(i,8,1,5)*ui(i1+8,i2+1,i3+5,c3)+c(i,0,2,5)*
     & ui(i1,i2+2,i3+5,c3)+c(i,1,2,5)*ui(i1+1,i2+2,i3+5,c3)+c(i,2,2,5)
     & *ui(i1+2,i2+2,i3+5,c3)+c(i,3,2,5)*ui(i1+3,i2+2,i3+5,c3)+c(i,4,
     & 2,5)*ui(i1+4,i2+2,i3+5,c3)+c(i,5,2,5)*ui(i1+5,i2+2,i3+5,c3)+c(
     & i,6,2,5)*ui(i1+6,i2+2,i3+5,c3)+c(i,7,2,5)*ui(i1+7,i2+2,i3+5,c3)
     & +c(i,8,2,5)*ui(i1+8,i2+2,i3+5,c3)+c(i,0,3,5)*ui(i1,i2+3,i3+5,
     & c3)+c(i,1,3,5)*ui(i1+1,i2+3,i3+5,c3)+c(i,2,3,5)*ui(i1+2,i2+3,
     & i3+5,c3)+c(i,3,3,5)*ui(i1+3,i2+3,i3+5,c3)+c(i,4,3,5)*ui(i1+4,
     & i2+3,i3+5,c3)+c(i,5,3,5)*ui(i1+5,i2+3,i3+5,c3)+c(i,6,3,5)*ui(
     & i1+6,i2+3,i3+5,c3)+c(i,7,3,5)*ui(i1+7,i2+3,i3+5,c3)+c(i,8,3,5)*
     & ui(i1+8,i2+3,i3+5,c3)+c(i,0,4,5)*ui(i1,i2+4,i3+5,c3)+c(i,1,4,5)
     & *ui(i1+1,i2+4,i3+5,c3)+c(i,2,4,5)*ui(i1+2,i2+4,i3+5,c3)+c(i,3,
     & 4,5)*ui(i1+3,i2+4,i3+5,c3)+c(i,4,4,5)*ui(i1+4,i2+4,i3+5,c3)+c(
     & i,5,4,5)*ui(i1+5,i2+4,i3+5,c3)+c(i,6,4,5)*ui(i1+6,i2+4,i3+5,c3)
     & +c(i,7,4,5)*ui(i1+7,i2+4,i3+5,c3)+c(i,8,4,5)*ui(i1+8,i2+4,i3+5,
     & c3)+c(i,0,5,5)*ui(i1,i2+5,i3+5,c3)+c(i,1,5,5)*ui(i1+1,i2+5,i3+
     & 5,c3)+c(i,2,5,5)*ui(i1+2,i2+5,i3+5,c3)+c(i,3,5,5)*ui(i1+3,i2+5,
     & i3+5,c3)+c(i,4,5,5)*ui(i1+4,i2+5,i3+5,c3)+c(i,5,5,5)*ui(i1+5,
     & i2+5,i3+5,c3)+c(i,6,5,5)*ui(i1+6,i2+5,i3+5,c3)+c(i,7,5,5)*ui(
     & i1+7,i2+5,i3+5,c3)+c(i,8,5,5)*ui(i1+8,i2+5,i3+5,c3)+c(i,0,6,5)*
     & ui(i1,i2+6,i3+5,c3)+c(i,1,6,5)*ui(i1+1,i2+6,i3+5,c3)+c(i,2,6,5)
     & *ui(i1+2,i2+6,i3+5,c3)+c(i,3,6,5)*ui(i1+3,i2+6,i3+5,c3)+c(i,4,
     & 6,5)*ui(i1+4,i2+6,i3+5,c3)+c(i,5,6,5)*ui(i1+5,i2+6,i3+5,c3)+c(
     & i,6,6,5)*ui(i1+6,i2+6,i3+5,c3)+c(i,7,6,5)*ui(i1+7,i2+6,i3+5,c3)
     & +c(i,8,6,5)*ui(i1+8,i2+6,i3+5,c3)+c(i,0,7,5)*ui(i1,i2+7,i3+5,
     & c3)+c(i,1,7,5)*ui(i1+1,i2+7,i3+5,c3)+c(i,2,7,5)*ui(i1+2,i2+7,
     & i3+5,c3)+c(i,3,7,5)*ui(i1+3,i2+7,i3+5,c3)+c(i,4,7,5)*ui(i1+4,
     & i2+7,i3+5,c3)+c(i,5,7,5)*ui(i1+5,i2+7,i3+5,c3)+c(i,6,7,5)*ui(
     & i1+6,i2+7,i3+5,c3)+c(i,7,7,5)*ui(i1+7,i2+7,i3+5,c3)+c(i,8,7,5)*
     & ui(i1+8,i2+7,i3+5,c3)+c(i,0,8,5)*ui(i1,i2+8,i3+5,c3)+c(i,1,8,5)
     & *ui(i1+1,i2+8,i3+5,c3)+c(i,2,8,5)*ui(i1+2,i2+8,i3+5,c3)+c(i,3,
     & 8,5)*ui(i1+3,i2+8,i3+5,c3)+c(i,4,8,5)*ui(i1+4,i2+8,i3+5,c3)+c(
     & i,5,8,5)*ui(i1+5,i2+8,i3+5,c3)+c(i,6,8,5)*ui(i1+6,i2+8,i3+5,c3)
     & +c(i,7,8,5)*ui(i1+7,i2+8,i3+5,c3)+c(i,8,8,5)*ui(i1+8,i2+8,i3+5,
     & c3)+c(i,0,0,6)*ui(i1,i2,i3+6,c3)+c(i,1,0,6)*ui(i1+1,i2,i3+6,c3)
     & +c(i,2,0,6)*ui(i1+2,i2,i3+6,c3)+c(i,3,0,6)*ui(i1+3,i2,i3+6,c3)+
     & c(i,4,0,6)*ui(i1+4,i2,i3+6,c3)+c(i,5,0,6)*ui(i1+5,i2,i3+6,c3)+
     & c(i,6,0,6)*ui(i1+6,i2,i3+6,c3)+c(i,7,0,6)*ui(i1+7,i2,i3+6,c3)+
     & c(i,8,0,6)*ui(i1+8,i2,i3+6,c3)+c(i,0,1,6)*ui(i1,i2+1,i3+6,c3)+
     & c(i,1,1,6)*ui(i1+1,i2+1,i3+6,c3)+c(i,2,1,6)*ui(i1+2,i2+1,i3+6,
     & c3)+c(i,3,1,6)*ui(i1+3,i2+1,i3+6,c3)+c(i,4,1,6)*ui(i1+4,i2+1,
     & i3+6,c3)+c(i,5,1,6)*ui(i1+5,i2+1,i3+6,c3)+c(i,6,1,6)*ui(i1+6,
     & i2+1,i3+6,c3)+c(i,7,1,6)*ui(i1+7,i2+1,i3+6,c3)+c(i,8,1,6)*ui(
     & i1+8,i2+1,i3+6,c3)+c(i,0,2,6)*ui(i1,i2+2,i3+6,c3)+c(i,1,2,6)*
     & ui(i1+1,i2+2,i3+6,c3)+c(i,2,2,6)*ui(i1+2,i2+2,i3+6,c3)+c(i,3,2,
     & 6)*ui(i1+3,i2+2,i3+6,c3)+c(i,4,2,6)*ui(i1+4,i2+2,i3+6,c3)+c(i,
     & 5,2,6)*ui(i1+5,i2+2,i3+6,c3)+c(i,6,2,6)*ui(i1+6,i2+2,i3+6,c3)+
     & c(i,7,2,6)*ui(i1+7,i2+2,i3+6,c3)+c(i,8,2,6)*ui(i1+8,i2+2,i3+6,
     & c3)+c(i,0,3,6)*ui(i1,i2+3,i3+6,c3)+c(i,1,3,6)*ui(i1+1,i2+3,i3+
     & 6,c3)+c(i,2,3,6)*ui(i1+2,i2+3,i3+6,c3)+c(i,3,3,6)*ui(i1+3,i2+3,
     & i3+6,c3)+c(i,4,3,6)*ui(i1+4,i2+3,i3+6,c3)+c(i,5,3,6)*ui(i1+5,
     & i2+3,i3+6,c3)+c(i,6,3,6)*ui(i1+6,i2+3,i3+6,c3)+c(i,7,3,6)*ui(
     & i1+7,i2+3,i3+6,c3)+c(i,8,3,6)*ui(i1+8,i2+3,i3+6,c3)+c(i,0,4,6)*
     & ui(i1,i2+4,i3+6,c3)+c(i,1,4,6)*ui(i1+1,i2+4,i3+6,c3)+c(i,2,4,6)
     & *ui(i1+2,i2+4,i3+6,c3)+c(i,3,4,6)*ui(i1+3,i2+4,i3+6,c3)+c(i,4,
     & 4,6)*ui(i1+4,i2+4,i3+6,c3)+c(i,5,4,6)*ui(i1+5,i2+4,i3+6,c3)+c(
     & i,6,4,6)*ui(i1+6,i2+4,i3+6,c3)+c(i,7,4,6)*ui(i1+7,i2+4,i3+6,c3)
     & +c(i,8,4,6)*ui(i1+8,i2+4,i3+6,c3)+c(i,0,5,6)*ui(i1,i2+5,i3+6,
     & c3)+c(i,1,5,6)*ui(i1+1,i2+5,i3+6,c3)+c(i,2,5,6)*ui(i1+2,i2+5,
     & i3+6,c3)+c(i,3,5,6)*ui(i1+3,i2+5,i3+6,c3)+c(i,4,5,6)*ui(i1+4,
     & i2+5,i3+6,c3)+c(i,5,5,6)*ui(i1+5,i2+5,i3+6,c3)+c(i,6,5,6)*ui(
     & i1+6,i2+5,i3+6,c3)+c(i,7,5,6)*ui(i1+7,i2+5,i3+6,c3)+c(i,8,5,6)*
     & ui(i1+8,i2+5,i3+6,c3)+c(i,0,6,6)*ui(i1,i2+6,i3+6,c3)+c(i,1,6,6)
     & *ui(i1+1,i2+6,i3+6,c3)+c(i,2,6,6)*ui(i1+2,i2+6,i3+6,c3)+c(i,3,
     & 6,6)*ui(i1+3,i2+6,i3+6,c3)+c(i,4,6,6)*ui(i1+4,i2+6,i3+6,c3)+c(
     & i,5,6,6)*ui(i1+5,i2+6,i3+6,c3)+c(i,6,6,6)*ui(i1+6,i2+6,i3+6,c3)
     & +c(i,7,6,6)*ui(i1+7,i2+6,i3+6,c3)+c(i,8,6,6)*ui(i1+8,i2+6,i3+6,
     & c3)+c(i,0,7,6)*ui(i1,i2+7,i3+6,c3)+c(i,1,7,6)*ui(i1+1,i2+7,i3+
     & 6,c3)+c(i,2,7,6)*ui(i1+2,i2+7,i3+6,c3)
                ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+c(i,3,7,6)*ui(i1+3,i2+7,i3+6,c3)+c(i,4,7,6)*ui(i1+4,
     & i2+7,i3+6,c3)+c(i,5,7,6)*ui(i1+5,i2+7,i3+6,c3)+c(i,6,7,6)*ui(
     & i1+6,i2+7,i3+6,c3)+c(i,7,7,6)*ui(i1+7,i2+7,i3+6,c3)+c(i,8,7,6)*
     & ui(i1+8,i2+7,i3+6,c3)+c(i,0,8,6)*ui(i1,i2+8,i3+6,c3)+c(i,1,8,6)
     & *ui(i1+1,i2+8,i3+6,c3)+c(i,2,8,6)*ui(i1+2,i2+8,i3+6,c3)+c(i,3,
     & 8,6)*ui(i1+3,i2+8,i3+6,c3)+c(i,4,8,6)*ui(i1+4,i2+8,i3+6,c3)+c(
     & i,5,8,6)*ui(i1+5,i2+8,i3+6,c3)+c(i,6,8,6)*ui(i1+6,i2+8,i3+6,c3)
     & +c(i,7,8,6)*ui(i1+7,i2+8,i3+6,c3)+c(i,8,8,6)*ui(i1+8,i2+8,i3+6,
     & c3)+c(i,0,0,7)*ui(i1,i2,i3+7,c3)+c(i,1,0,7)*ui(i1+1,i2,i3+7,c3)
     & +c(i,2,0,7)*ui(i1+2,i2,i3+7,c3)+c(i,3,0,7)*ui(i1+3,i2,i3+7,c3)+
     & c(i,4,0,7)*ui(i1+4,i2,i3+7,c3)+c(i,5,0,7)*ui(i1+5,i2,i3+7,c3)+
     & c(i,6,0,7)*ui(i1+6,i2,i3+7,c3)+c(i,7,0,7)*ui(i1+7,i2,i3+7,c3)+
     & c(i,8,0,7)*ui(i1+8,i2,i3+7,c3)+c(i,0,1,7)*ui(i1,i2+1,i3+7,c3)+
     & c(i,1,1,7)*ui(i1+1,i2+1,i3+7,c3)+c(i,2,1,7)*ui(i1+2,i2+1,i3+7,
     & c3)+c(i,3,1,7)*ui(i1+3,i2+1,i3+7,c3)+c(i,4,1,7)*ui(i1+4,i2+1,
     & i3+7,c3)+c(i,5,1,7)*ui(i1+5,i2+1,i3+7,c3)+c(i,6,1,7)*ui(i1+6,
     & i2+1,i3+7,c3)+c(i,7,1,7)*ui(i1+7,i2+1,i3+7,c3)+c(i,8,1,7)*ui(
     & i1+8,i2+1,i3+7,c3)+c(i,0,2,7)*ui(i1,i2+2,i3+7,c3)+c(i,1,2,7)*
     & ui(i1+1,i2+2,i3+7,c3)+c(i,2,2,7)*ui(i1+2,i2+2,i3+7,c3)+c(i,3,2,
     & 7)*ui(i1+3,i2+2,i3+7,c3)+c(i,4,2,7)*ui(i1+4,i2+2,i3+7,c3)+c(i,
     & 5,2,7)*ui(i1+5,i2+2,i3+7,c3)+c(i,6,2,7)*ui(i1+6,i2+2,i3+7,c3)+
     & c(i,7,2,7)*ui(i1+7,i2+2,i3+7,c3)+c(i,8,2,7)*ui(i1+8,i2+2,i3+7,
     & c3)+c(i,0,3,7)*ui(i1,i2+3,i3+7,c3)+c(i,1,3,7)*ui(i1+1,i2+3,i3+
     & 7,c3)+c(i,2,3,7)*ui(i1+2,i2+3,i3+7,c3)+c(i,3,3,7)*ui(i1+3,i2+3,
     & i3+7,c3)+c(i,4,3,7)*ui(i1+4,i2+3,i3+7,c3)+c(i,5,3,7)*ui(i1+5,
     & i2+3,i3+7,c3)+c(i,6,3,7)*ui(i1+6,i2+3,i3+7,c3)+c(i,7,3,7)*ui(
     & i1+7,i2+3,i3+7,c3)+c(i,8,3,7)*ui(i1+8,i2+3,i3+7,c3)+c(i,0,4,7)*
     & ui(i1,i2+4,i3+7,c3)+c(i,1,4,7)*ui(i1+1,i2+4,i3+7,c3)+c(i,2,4,7)
     & *ui(i1+2,i2+4,i3+7,c3)+c(i,3,4,7)*ui(i1+3,i2+4,i3+7,c3)+c(i,4,
     & 4,7)*ui(i1+4,i2+4,i3+7,c3)+c(i,5,4,7)*ui(i1+5,i2+4,i3+7,c3)+c(
     & i,6,4,7)*ui(i1+6,i2+4,i3+7,c3)+c(i,7,4,7)*ui(i1+7,i2+4,i3+7,c3)
     & +c(i,8,4,7)*ui(i1+8,i2+4,i3+7,c3)+c(i,0,5,7)*ui(i1,i2+5,i3+7,
     & c3)+c(i,1,5,7)*ui(i1+1,i2+5,i3+7,c3)+c(i,2,5,7)*ui(i1+2,i2+5,
     & i3+7,c3)+c(i,3,5,7)*ui(i1+3,i2+5,i3+7,c3)+c(i,4,5,7)*ui(i1+4,
     & i2+5,i3+7,c3)+c(i,5,5,7)*ui(i1+5,i2+5,i3+7,c3)+c(i,6,5,7)*ui(
     & i1+6,i2+5,i3+7,c3)+c(i,7,5,7)*ui(i1+7,i2+5,i3+7,c3)+c(i,8,5,7)*
     & ui(i1+8,i2+5,i3+7,c3)+c(i,0,6,7)*ui(i1,i2+6,i3+7,c3)+c(i,1,6,7)
     & *ui(i1+1,i2+6,i3+7,c3)+c(i,2,6,7)*ui(i1+2,i2+6,i3+7,c3)+c(i,3,
     & 6,7)*ui(i1+3,i2+6,i3+7,c3)+c(i,4,6,7)*ui(i1+4,i2+6,i3+7,c3)+c(
     & i,5,6,7)*ui(i1+5,i2+6,i3+7,c3)+c(i,6,6,7)*ui(i1+6,i2+6,i3+7,c3)
     & +c(i,7,6,7)*ui(i1+7,i2+6,i3+7,c3)+c(i,8,6,7)*ui(i1+8,i2+6,i3+7,
     & c3)+c(i,0,7,7)*ui(i1,i2+7,i3+7,c3)+c(i,1,7,7)*ui(i1+1,i2+7,i3+
     & 7,c3)+c(i,2,7,7)*ui(i1+2,i2+7,i3+7,c3)+c(i,3,7,7)*ui(i1+3,i2+7,
     & i3+7,c3)+c(i,4,7,7)*ui(i1+4,i2+7,i3+7,c3)+c(i,5,7,7)*ui(i1+5,
     & i2+7,i3+7,c3)+c(i,6,7,7)*ui(i1+6,i2+7,i3+7,c3)+c(i,7,7,7)*ui(
     & i1+7,i2+7,i3+7,c3)+c(i,8,7,7)*ui(i1+8,i2+7,i3+7,c3)+c(i,0,8,7)*
     & ui(i1,i2+8,i3+7,c3)+c(i,1,8,7)*ui(i1+1,i2+8,i3+7,c3)+c(i,2,8,7)
     & *ui(i1+2,i2+8,i3+7,c3)+c(i,3,8,7)*ui(i1+3,i2+8,i3+7,c3)+c(i,4,
     & 8,7)*ui(i1+4,i2+8,i3+7,c3)+c(i,5,8,7)*ui(i1+5,i2+8,i3+7,c3)+c(
     & i,6,8,7)*ui(i1+6,i2+8,i3+7,c3)+c(i,7,8,7)*ui(i1+7,i2+8,i3+7,c3)
     & +c(i,8,8,7)*ui(i1+8,i2+8,i3+7,c3)+c(i,0,0,8)*ui(i1,i2,i3+8,c3)+
     & c(i,1,0,8)*ui(i1+1,i2,i3+8,c3)+c(i,2,0,8)*ui(i1+2,i2,i3+8,c3)+
     & c(i,3,0,8)*ui(i1+3,i2,i3+8,c3)+c(i,4,0,8)*ui(i1+4,i2,i3+8,c3)+
     & c(i,5,0,8)*ui(i1+5,i2,i3+8,c3)+c(i,6,0,8)*ui(i1+6,i2,i3+8,c3)+
     & c(i,7,0,8)*ui(i1+7,i2,i3+8,c3)+c(i,8,0,8)*ui(i1+8,i2,i3+8,c3)+
     & c(i,0,1,8)*ui(i1,i2+1,i3+8,c3)+c(i,1,1,8)*ui(i1+1,i2+1,i3+8,c3)
     & +c(i,2,1,8)*ui(i1+2,i2+1,i3+8,c3)+c(i,3,1,8)*ui(i1+3,i2+1,i3+8,
     & c3)+c(i,4,1,8)*ui(i1+4,i2+1,i3+8,c3)+c(i,5,1,8)*ui(i1+5,i2+1,
     & i3+8,c3)+c(i,6,1,8)*ui(i1+6,i2+1,i3+8,c3)+c(i,7,1,8)*ui(i1+7,
     & i2+1,i3+8,c3)+c(i,8,1,8)*ui(i1+8,i2+1,i3+8,c3)+c(i,0,2,8)*ui(
     & i1,i2+2,i3+8,c3)+c(i,1,2,8)*ui(i1+1,i2+2,i3+8,c3)+c(i,2,2,8)*
     & ui(i1+2,i2+2,i3+8,c3)+c(i,3,2,8)*ui(i1+3,i2+2,i3+8,c3)+c(i,4,2,
     & 8)*ui(i1+4,i2+2,i3+8,c3)+c(i,5,2,8)*ui(i1+5,i2+2,i3+8,c3)+c(i,
     & 6,2,8)*ui(i1+6,i2+2,i3+8,c3)+c(i,7,2,8)*ui(i1+7,i2+2,i3+8,c3)+
     & c(i,8,2,8)*ui(i1+8,i2+2,i3+8,c3)+c(i,0,3,8)*ui(i1,i2+3,i3+8,c3)
     & +c(i,1,3,8)*ui(i1+1,i2+3,i3+8,c3)+c(i,2,3,8)*ui(i1+2,i2+3,i3+8,
     & c3)+c(i,3,3,8)*ui(i1+3,i2+3,i3+8,c3)+c(i,4,3,8)*ui(i1+4,i2+3,
     & i3+8,c3)+c(i,5,3,8)*ui(i1+5,i2+3,i3+8,c3)+c(i,6,3,8)*ui(i1+6,
     & i2+3,i3+8,c3)+c(i,7,3,8)*ui(i1+7,i2+3,i3+8,c3)+c(i,8,3,8)*ui(
     & i1+8,i2+3,i3+8,c3)+c(i,0,4,8)*ui(i1,i2+4,i3+8,c3)+c(i,1,4,8)*
     & ui(i1+1,i2+4,i3+8,c3)+c(i,2,4,8)*ui(i1+2,i2+4,i3+8,c3)+c(i,3,4,
     & 8)*ui(i1+3,i2+4,i3+8,c3)+c(i,4,4,8)*ui(i1+4,i2+4,i3+8,c3)+c(i,
     & 5,4,8)*ui(i1+5,i2+4,i3+8,c3)+c(i,6,4,8)*ui(i1+6,i2+4,i3+8,c3)+
     & c(i,7,4,8)*ui(i1+7,i2+4,i3+8,c3)+c(i,8,4,8)*ui(i1+8,i2+4,i3+8,
     & c3)+c(i,0,5,8)*ui(i1,i2+5,i3+8,c3)+c(i,1,5,8)*ui(i1+1,i2+5,i3+
     & 8,c3)+c(i,2,5,8)*ui(i1+2,i2+5,i3+8,c3)+c(i,3,5,8)*ui(i1+3,i2+5,
     & i3+8,c3)+c(i,4,5,8)*ui(i1+4,i2+5,i3+8,c3)+c(i,5,5,8)*ui(i1+5,
     & i2+5,i3+8,c3)+c(i,6,5,8)*ui(i1+6,i2+5,i3+8,c3)+c(i,7,5,8)*ui(
     & i1+7,i2+5,i3+8,c3)+c(i,8,5,8)*ui(i1+8,i2+5,i3+8,c3)+c(i,0,6,8)*
     & ui(i1,i2+6,i3+8,c3)+c(i,1,6,8)*ui(i1+1,i2+6,i3+8,c3)+c(i,2,6,8)
     & *ui(i1+2,i2+6,i3+8,c3)+c(i,3,6,8)*ui(i1+3,i2+6,i3+8,c3)+c(i,4,
     & 6,8)*ui(i1+4,i2+6,i3+8,c3)+c(i,5,6,8)*ui(i1+5,i2+6,i3+8,c3)+c(
     & i,6,6,8)*ui(i1+6,i2+6,i3+8,c3)+c(i,7,6,8)*ui(i1+7,i2+6,i3+8,c3)
     & +c(i,8,6,8)*ui(i1+8,i2+6,i3+8,c3)+c(i,0,7,8)*ui(i1,i2+7,i3+8,
     & c3)+c(i,1,7,8)*ui(i1+1,i2+7,i3+8,c3)+c(i,2,7,8)*ui(i1+2,i2+7,
     & i3+8,c3)+c(i,3,7,8)*ui(i1+3,i2+7,i3+8,c3)+c(i,4,7,8)*ui(i1+4,
     & i2+7,i3+8,c3)+c(i,5,7,8)*ui(i1+5,i2+7,i3+8,c3)+c(i,6,7,8)*ui(
     & i1+6,i2+7,i3+8,c3)+c(i,7,7,8)*ui(i1+7,i2+7,i3+8,c3)+c(i,8,7,8)*
     & ui(i1+8,i2+7,i3+8,c3)+c(i,0,8,8)*ui(i1,i2+8,i3+8,c3)+c(i,1,8,8)
     & *ui(i1+1,i2+8,i3+8,c3)+c(i,2,8,8)*ui(i1+2,i2+8,i3+8,c3)+c(i,3,
     & 8,8)*ui(i1+3,i2+8,i3+8,c3)+c(i,4,8,8)*ui(i1+4,i2+8,i3+8,c3)+c(
     & i,5,8,8)*ui(i1+5,i2+8,i3+8,c3)+c(i,6,8,8)*ui(i1+6,i2+8,i3+8,c3)
     & +c(i,7,8,8)*ui(i1+7,i2+8,i3+8,c3)+c(i,8,8,8)*ui(i1+8,i2+8,i3+8,
     & c3)


              end do
              end do
            end if
          else
c     general case in 3D
            do c3=c3a,c3b
              do i=nia,nib
                ug(ip(i,1),ip(i,2),ip(i,3),c3)=0.
              end do
              do w3=0,width(3)-1
                do w2=0,width(2)-1
                  do w1=0,width(1)-1
                    do i=nia,nib
                      ug(ip(i,1),ip(i,2),ip(i,3),c3)=ug(ip(i,1),ip(i,2)
     & ,ip(i,3),c3)+c(i,w1,w2,w3)*ui(il(i,1)+w1,il(i,2)+w2,il(i,3)+w3,
     & c3)
                    end do
                  end do
                end do
              end do
            end do
          end if
        end if
c **      else if( storageOption.eq.1 )then
! #If "Full" == "TP"
! #If "Full" == "SP"
       else
         write(*,*) 'interpOpt:ERROR; unknown storage option=',
     & storageOption
         stop 3
       end if ! end storage option
       return
       end
