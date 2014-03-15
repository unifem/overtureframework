! This file automatically generated from interpOpt.bf with bpp.
! defineInterpOptRes(SP)
       subroutine interpOptResSP ( nd,ndui1a,ndui1b,ndui2a,ndui2b,
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
! #If "SP" == "Full"
! #If "SP" == "TP"
! #If "SP" == "SP"
       if( storageOption.eq.2 )then
c       ****************************************
c       **** sparse         storage option *****
c       ****************************************
       if( nd.eq.2 )then
         if( useVariableWidthInterpolation.ne.0 )then
! beginLoops2d()
             do i=nia,nib
             do c3=c3a,c3b
             do c2=c2a,c2b
             ! check for most common widths first
             if( varWidth(i).eq.3 )then
! interpSparseStorage33(r(i))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q30(c(i,0,0,0))
                 cs0 = q30(c(i,1,0,0))
                 cr1 = q31(c(i,0,0,0))
                 cs1 = q31(c(i,1,0,0))
                 cr2 = q32(c(i,0,0,0))
                 cs2 = q32(c(i,1,0,0))
               r(i) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)+cr1*ui(i1+1,i2  ,c2,
     & c3)+cr2*ui(i1+2,i2  ,c2,c3))+cs1*(cr0*ui(i1  ,i2+1,c2,c3)+cr1*
     & ui(i1+1,i2+1,c2,c3)+cr2*ui(i1+2,i2+1,c2,c3))+cs2*(cr0*ui(i1  ,
     & i2+2,c2,c3)+cr1*ui(i1+1,i2+2,c2,c3)+cr2*ui(i1+2,i2+2,c2,c3))
             else if( varWidth(i).eq.2 )then
! interpSparseStorage22(r(i))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q20(c(i,0,0,0))
                 cs0 = q20(c(i,1,0,0))
                 cr1 = q21(c(i,0,0,0))
                 cs1 = q21(c(i,1,0,0))
               r(i) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)+cr1*ui(i1+1,i2  ,c2,
     & c3))+cs1*(cr0*ui(i1  ,i2+1,c2,c3)+cr1*ui(i1+1,i2+1,c2,c3))
             else if( varWidth(i).eq.1 )then
! interp11(r(i))
               i1=il(i,1)
               i2=il(i,2)
               r(i) = ui(i1  ,i2  ,c2,c3)
             else if( varWidth(i).eq.5 )then
! interpSparseStorage55(r(i))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q50(c(i,0,0,0))
                 cs0 = q50(c(i,1,0,0))
                 cr1 = q51(c(i,0,0,0))
                 cs1 = q51(c(i,1,0,0))
                 cr2 = q52(c(i,0,0,0))
                 cs2 = q52(c(i,1,0,0))
                 cr3 = q53(c(i,0,0,0))
                 cs3 = q53(c(i,1,0,0))
                 cr4 = q54(c(i,0,0,0))
                 cs4 = q54(c(i,1,0,0))
               r(i) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)+cr1*ui(i1+1,i2  ,c2,
     & c3)+cr2*ui(i1+2,i2  ,c2,c3)+cr3*ui(i1+3,i2  ,c2,c3)+cr4*ui(i1+
     & 4,i2  ,c2,c3))+cs1*(cr0*ui(i1  ,i2+1,c2,c3)+cr1*ui(i1+1,i2+1,
     & c2,c3)+cr2*ui(i1+2,i2+1,c2,c3)+cr3*ui(i1+3,i2+1,c2,c3)+cr4*ui(
     & i1+4,i2+1,c2,c3))+cs2*(cr0*ui(i1  ,i2+2,c2,c3)+cr1*ui(i1+1,i2+
     & 2,c2,c3)+cr2*ui(i1+2,i2+2,c2,c3)+cr3*ui(i1+3,i2+2,c2,c3)+cr4*
     & ui(i1+4,i2+2,c2,c3))+cs3*(cr0*ui(i1  ,i2+3,c2,c3)+cr1*ui(i1+1,
     & i2+3,c2,c3)+cr2*ui(i1+2,i2+3,c2,c3)+cr3*ui(i1+3,i2+3,c2,c3)+
     & cr4*ui(i1+4,i2+3,c2,c3))+cs4*(cr0*ui(i1  ,i2+4,c2,c3)+cr1*ui(
     & i1+1,i2+4,c2,c3)+cr2*ui(i1+2,i2+4,c2,c3)+cr3*ui(i1+3,i2+4,c2,
     & c3)+cr4*ui(i1+4,i2+4,c2,c3))
             else if( varWidth(i).eq.4 )then
! interpSparseStorage44(r(i))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q40(c(i,0,0,0))
                 cs0 = q40(c(i,1,0,0))
                 cr1 = q41(c(i,0,0,0))
                 cs1 = q41(c(i,1,0,0))
                 cr2 = q42(c(i,0,0,0))
                 cs2 = q42(c(i,1,0,0))
                 cr3 = q43(c(i,0,0,0))
                 cs3 = q43(c(i,1,0,0))
               r(i) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)+cr1*ui(i1+1,i2  ,c2,
     & c3)+cr2*ui(i1+2,i2  ,c2,c3)+cr3*ui(i1+3,i2  ,c2,c3))+cs1*(cr0*
     & ui(i1  ,i2+1,c2,c3)+cr1*ui(i1+1,i2+1,c2,c3)+cr2*ui(i1+2,i2+1,
     & c2,c3)+cr3*ui(i1+3,i2+1,c2,c3))+cs2*(cr0*ui(i1  ,i2+2,c2,c3)+
     & cr1*ui(i1+1,i2+2,c2,c3)+cr2*ui(i1+2,i2+2,c2,c3)+cr3*ui(i1+3,i2+
     & 2,c2,c3))+cs3*(cr0*ui(i1  ,i2+3,c2,c3)+cr1*ui(i1+1,i2+3,c2,c3)+
     & cr2*ui(i1+2,i2+3,c2,c3)+cr3*ui(i1+3,i2+3,c2,c3))
             else if( varWidth(i).eq.7 )then
! interpSparseStorage77(r(i))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q70(c(i,0,0,0))
                 cs0 = q70(c(i,1,0,0))
                 cr1 = q71(c(i,0,0,0))
                 cs1 = q71(c(i,1,0,0))
                 cr2 = q72(c(i,0,0,0))
                 cs2 = q72(c(i,1,0,0))
                 cr3 = q73(c(i,0,0,0))
                 cs3 = q73(c(i,1,0,0))
                 cr4 = q74(c(i,0,0,0))
                 cs4 = q74(c(i,1,0,0))
                 cr5 = q75(c(i,0,0,0))
                 cs5 = q75(c(i,1,0,0))
                 cr6 = q76(c(i,0,0,0))
                 cs6 = q76(c(i,1,0,0))
               r(i) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)+cr1*ui(i1+1,i2  ,c2,
     & c3)+cr2*ui(i1+2,i2  ,c2,c3)+cr3*ui(i1+3,i2  ,c2,c3)+cr4*ui(i1+
     & 4,i2  ,c2,c3)+cr5*ui(i1+5,i2  ,c2,c3)+cr6*ui(i1+6,i2  ,c2,c3))+
     & cs1*(cr0*ui(i1  ,i2+1,c2,c3)+cr1*ui(i1+1,i2+1,c2,c3)+cr2*ui(i1+
     & 2,i2+1,c2,c3)+cr3*ui(i1+3,i2+1,c2,c3)+cr4*ui(i1+4,i2+1,c2,c3)+
     & cr5*ui(i1+5,i2+1,c2,c3)+cr6*ui(i1+6,i2+1,c2,c3))+cs2*(cr0*ui(
     & i1  ,i2+2,c2,c3)+cr1*ui(i1+1,i2+2,c2,c3)+cr2*ui(i1+2,i2+2,c2,
     & c3)+cr3*ui(i1+3,i2+2,c2,c3)+cr4*ui(i1+4,i2+2,c2,c3)+cr5*ui(i1+
     & 5,i2+2,c2,c3)+cr6*ui(i1+6,i2+2,c2,c3))+cs3*(cr0*ui(i1  ,i2+3,
     & c2,c3)+cr1*ui(i1+1,i2+3,c2,c3)+cr2*ui(i1+2,i2+3,c2,c3)+cr3*ui(
     & i1+3,i2+3,c2,c3)+cr4*ui(i1+4,i2+3,c2,c3)+cr5*ui(i1+5,i2+3,c2,
     & c3)+cr6*ui(i1+6,i2+3,c2,c3))+cs4*(cr0*ui(i1  ,i2+4,c2,c3)+cr1*
     & ui(i1+1,i2+4,c2,c3)+cr2*ui(i1+2,i2+4,c2,c3)+cr3*ui(i1+3,i2+4,
     & c2,c3)+cr4*ui(i1+4,i2+4,c2,c3)+cr5*ui(i1+5,i2+4,c2,c3)+cr6*ui(
     & i1+6,i2+4,c2,c3))+cs5*(cr0*ui(i1  ,i2+5,c2,c3)+cr1*ui(i1+1,i2+
     & 5,c2,c3)+cr2*ui(i1+2,i2+5,c2,c3)+cr3*ui(i1+3,i2+5,c2,c3)+cr4*
     & ui(i1+4,i2+5,c2,c3)+cr5*ui(i1+5,i2+5,c2,c3)+cr6*ui(i1+6,i2+5,
     & c2,c3))+cs6*(cr0*ui(i1  ,i2+6,c2,c3)+cr1*ui(i1+1,i2+6,c2,c3)+
     & cr2*ui(i1+2,i2+6,c2,c3)+cr3*ui(i1+3,i2+6,c2,c3)+cr4*ui(i1+4,i2+
     & 6,c2,c3)+cr5*ui(i1+5,i2+6,c2,c3)+cr6*ui(i1+6,i2+6,c2,c3))
             else if( varWidth(i).eq.6 )then
! interpSparseStorage66(r(i))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q60(c(i,0,0,0))
                 cs0 = q60(c(i,1,0,0))
                 cr1 = q61(c(i,0,0,0))
                 cs1 = q61(c(i,1,0,0))
                 cr2 = q62(c(i,0,0,0))
                 cs2 = q62(c(i,1,0,0))
                 cr3 = q63(c(i,0,0,0))
                 cs3 = q63(c(i,1,0,0))
                 cr4 = q64(c(i,0,0,0))
                 cs4 = q64(c(i,1,0,0))
                 cr5 = q65(c(i,0,0,0))
                 cs5 = q65(c(i,1,0,0))
               r(i) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)+cr1*ui(i1+1,i2  ,c2,
     & c3)+cr2*ui(i1+2,i2  ,c2,c3)+cr3*ui(i1+3,i2  ,c2,c3)+cr4*ui(i1+
     & 4,i2  ,c2,c3)+cr5*ui(i1+5,i2  ,c2,c3))+cs1*(cr0*ui(i1  ,i2+1,
     & c2,c3)+cr1*ui(i1+1,i2+1,c2,c3)+cr2*ui(i1+2,i2+1,c2,c3)+cr3*ui(
     & i1+3,i2+1,c2,c3)+cr4*ui(i1+4,i2+1,c2,c3)+cr5*ui(i1+5,i2+1,c2,
     & c3))+cs2*(cr0*ui(i1  ,i2+2,c2,c3)+cr1*ui(i1+1,i2+2,c2,c3)+cr2*
     & ui(i1+2,i2+2,c2,c3)+cr3*ui(i1+3,i2+2,c2,c3)+cr4*ui(i1+4,i2+2,
     & c2,c3)+cr5*ui(i1+5,i2+2,c2,c3))+cs3*(cr0*ui(i1  ,i2+3,c2,c3)+
     & cr1*ui(i1+1,i2+3,c2,c3)+cr2*ui(i1+2,i2+3,c2,c3)+cr3*ui(i1+3,i2+
     & 3,c2,c3)+cr4*ui(i1+4,i2+3,c2,c3)+cr5*ui(i1+5,i2+3,c2,c3))+cs4*(
     & cr0*ui(i1  ,i2+4,c2,c3)+cr1*ui(i1+1,i2+4,c2,c3)+cr2*ui(i1+2,i2+
     & 4,c2,c3)+cr3*ui(i1+3,i2+4,c2,c3)+cr4*ui(i1+4,i2+4,c2,c3)+cr5*
     & ui(i1+5,i2+4,c2,c3))+cs5*(cr0*ui(i1  ,i2+5,c2,c3)+cr1*ui(i1+1,
     & i2+5,c2,c3)+cr2*ui(i1+2,i2+5,c2,c3)+cr3*ui(i1+3,i2+5,c2,c3)+
     & cr4*ui(i1+4,i2+5,c2,c3)+cr5*ui(i1+5,i2+5,c2,c3))
             else if( varWidth(i).eq.9 )then
! interpSparseStorage99(r(i))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q90(c(i,0,0,0))
                 cs0 = q90(c(i,1,0,0))
                 cr1 = q91(c(i,0,0,0))
                 cs1 = q91(c(i,1,0,0))
                 cr2 = q92(c(i,0,0,0))
                 cs2 = q92(c(i,1,0,0))
                 cr3 = q93(c(i,0,0,0))
                 cs3 = q93(c(i,1,0,0))
                 cr4 = q94(c(i,0,0,0))
                 cs4 = q94(c(i,1,0,0))
                 cr5 = q95(c(i,0,0,0))
                 cs5 = q95(c(i,1,0,0))
                 cr6 = q96(c(i,0,0,0))
                 cs6 = q96(c(i,1,0,0))
                 cr7 = q97(c(i,0,0,0))
                 cs7 = q97(c(i,1,0,0))
                 cr8 = q98(c(i,0,0,0))
                 cs8 = q98(c(i,1,0,0))
               r(i) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)+cr1*ui(i1+1,i2  ,c2,
     & c3)+cr2*ui(i1+2,i2  ,c2,c3)+cr3*ui(i1+3,i2  ,c2,c3)+cr4*ui(i1+
     & 4,i2  ,c2,c3)+cr5*ui(i1+5,i2  ,c2,c3)+cr6*ui(i1+6,i2  ,c2,c3)+
     & cr7*ui(i1+7,i2  ,c2,c3)+cr8*ui(i1+8,i2  ,c2,c3))+cs1*(cr0*ui(
     & i1  ,i2+1,c2,c3)+cr1*ui(i1+1,i2+1,c2,c3)+cr2*ui(i1+2,i2+1,c2,
     & c3)+cr3*ui(i1+3,i2+1,c2,c3)+cr4*ui(i1+4,i2+1,c2,c3)+cr5*ui(i1+
     & 5,i2+1,c2,c3)+cr6*ui(i1+6,i2+1,c2,c3)+cr7*ui(i1+7,i2+1,c2,c3)+
     & cr8*ui(i1+8,i2+1,c2,c3))+cs2*(cr0*ui(i1  ,i2+2,c2,c3)+cr1*ui(
     & i1+1,i2+2,c2,c3)+cr2*ui(i1+2,i2+2,c2,c3)+cr3*ui(i1+3,i2+2,c2,
     & c3)+cr4*ui(i1+4,i2+2,c2,c3)+cr5*ui(i1+5,i2+2,c2,c3)+cr6*ui(i1+
     & 6,i2+2,c2,c3)+cr7*ui(i1+7,i2+2,c2,c3)+cr8*ui(i1+8,i2+2,c2,c3))+
     & cs3*(cr0*ui(i1  ,i2+3,c2,c3)+cr1*ui(i1+1,i2+3,c2,c3)+cr2*ui(i1+
     & 2,i2+3,c2,c3)+cr3*ui(i1+3,i2+3,c2,c3)+cr4*ui(i1+4,i2+3,c2,c3)+
     & cr5*ui(i1+5,i2+3,c2,c3)+cr6*ui(i1+6,i2+3,c2,c3)+cr7*ui(i1+7,i2+
     & 3,c2,c3)+cr8*ui(i1+8,i2+3,c2,c3))+cs4*(cr0*ui(i1  ,i2+4,c2,c3)+
     & cr1*ui(i1+1,i2+4,c2,c3)+cr2*ui(i1+2,i2+4,c2,c3)+cr3*ui(i1+3,i2+
     & 4,c2,c3)+cr4*ui(i1+4,i2+4,c2,c3)+cr5*ui(i1+5,i2+4,c2,c3)+cr6*
     & ui(i1+6,i2+4,c2,c3)+cr7*ui(i1+7,i2+4,c2,c3)+cr8*ui(i1+8,i2+4,
     & c2,c3))+cs5*(cr0*ui(i1  ,i2+5,c2,c3)+cr1*ui(i1+1,i2+5,c2,c3)+
     & cr2*ui(i1+2,i2+5,c2,c3)+cr3*ui(i1+3,i2+5,c2,c3)+cr4*ui(i1+4,i2+
     & 5,c2,c3)+cr5*ui(i1+5,i2+5,c2,c3)+cr6*ui(i1+6,i2+5,c2,c3)+cr7*
     & ui(i1+7,i2+5,c2,c3)+cr8*ui(i1+8,i2+5,c2,c3))+cs6*(cr0*ui(i1  ,
     & i2+6,c2,c3)+cr1*ui(i1+1,i2+6,c2,c3)+cr2*ui(i1+2,i2+6,c2,c3)+
     & cr3*ui(i1+3,i2+6,c2,c3)+cr4*ui(i1+4,i2+6,c2,c3)+cr5*ui(i1+5,i2+
     & 6,c2,c3)+cr6*ui(i1+6,i2+6,c2,c3)+cr7*ui(i1+7,i2+6,c2,c3)+cr8*
     & ui(i1+8,i2+6,c2,c3))+cs7*(cr0*ui(i1  ,i2+7,c2,c3)+cr1*ui(i1+1,
     & i2+7,c2,c3)+cr2*ui(i1+2,i2+7,c2,c3)+cr3*ui(i1+3,i2+7,c2,c3)+
     & cr4*ui(i1+4,i2+7,c2,c3)+cr5*ui(i1+5,i2+7,c2,c3)+cr6*ui(i1+6,i2+
     & 7,c2,c3)+cr7*ui(i1+7,i2+7,c2,c3)+cr8*ui(i1+8,i2+7,c2,c3))+cs8*(
     & cr0*ui(i1  ,i2+8,c2,c3)+cr1*ui(i1+1,i2+8,c2,c3)+cr2*ui(i1+2,i2+
     & 8,c2,c3)+cr3*ui(i1+3,i2+8,c2,c3)+cr4*ui(i1+4,i2+8,c2,c3)+cr5*
     & ui(i1+5,i2+8,c2,c3)+cr6*ui(i1+6,i2+8,c2,c3)+cr7*ui(i1+7,i2+8,
     & c2,c3)+cr8*ui(i1+8,i2+8,c2,c3))
             else if( varWidth(i).eq.8 )then
! interpSparseStorage88(r(i))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q80(c(i,0,0,0))
                 cs0 = q80(c(i,1,0,0))
                 cr1 = q81(c(i,0,0,0))
                 cs1 = q81(c(i,1,0,0))
                 cr2 = q82(c(i,0,0,0))
                 cs2 = q82(c(i,1,0,0))
                 cr3 = q83(c(i,0,0,0))
                 cs3 = q83(c(i,1,0,0))
                 cr4 = q84(c(i,0,0,0))
                 cs4 = q84(c(i,1,0,0))
                 cr5 = q85(c(i,0,0,0))
                 cs5 = q85(c(i,1,0,0))
                 cr6 = q86(c(i,0,0,0))
                 cs6 = q86(c(i,1,0,0))
                 cr7 = q87(c(i,0,0,0))
                 cs7 = q87(c(i,1,0,0))
               r(i) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)+cr1*ui(i1+1,i2  ,c2,
     & c3)+cr2*ui(i1+2,i2  ,c2,c3)+cr3*ui(i1+3,i2  ,c2,c3)+cr4*ui(i1+
     & 4,i2  ,c2,c3)+cr5*ui(i1+5,i2  ,c2,c3)+cr6*ui(i1+6,i2  ,c2,c3)+
     & cr7*ui(i1+7,i2  ,c2,c3))+cs1*(cr0*ui(i1  ,i2+1,c2,c3)+cr1*ui(
     & i1+1,i2+1,c2,c3)+cr2*ui(i1+2,i2+1,c2,c3)+cr3*ui(i1+3,i2+1,c2,
     & c3)+cr4*ui(i1+4,i2+1,c2,c3)+cr5*ui(i1+5,i2+1,c2,c3)+cr6*ui(i1+
     & 6,i2+1,c2,c3)+cr7*ui(i1+7,i2+1,c2,c3))+cs2*(cr0*ui(i1  ,i2+2,
     & c2,c3)+cr1*ui(i1+1,i2+2,c2,c3)+cr2*ui(i1+2,i2+2,c2,c3)+cr3*ui(
     & i1+3,i2+2,c2,c3)+cr4*ui(i1+4,i2+2,c2,c3)+cr5*ui(i1+5,i2+2,c2,
     & c3)+cr6*ui(i1+6,i2+2,c2,c3)+cr7*ui(i1+7,i2+2,c2,c3))+cs3*(cr0*
     & ui(i1  ,i2+3,c2,c3)+cr1*ui(i1+1,i2+3,c2,c3)+cr2*ui(i1+2,i2+3,
     & c2,c3)+cr3*ui(i1+3,i2+3,c2,c3)+cr4*ui(i1+4,i2+3,c2,c3)+cr5*ui(
     & i1+5,i2+3,c2,c3)+cr6*ui(i1+6,i2+3,c2,c3)+cr7*ui(i1+7,i2+3,c2,
     & c3))+cs4*(cr0*ui(i1  ,i2+4,c2,c3)+cr1*ui(i1+1,i2+4,c2,c3)+cr2*
     & ui(i1+2,i2+4,c2,c3)+cr3*ui(i1+3,i2+4,c2,c3)+cr4*ui(i1+4,i2+4,
     & c2,c3)+cr5*ui(i1+5,i2+4,c2,c3)+cr6*ui(i1+6,i2+4,c2,c3)+cr7*ui(
     & i1+7,i2+4,c2,c3))+cs5*(cr0*ui(i1  ,i2+5,c2,c3)+cr1*ui(i1+1,i2+
     & 5,c2,c3)+cr2*ui(i1+2,i2+5,c2,c3)+cr3*ui(i1+3,i2+5,c2,c3)+cr4*
     & ui(i1+4,i2+5,c2,c3)+cr5*ui(i1+5,i2+5,c2,c3)+cr6*ui(i1+6,i2+5,
     & c2,c3)+cr7*ui(i1+7,i2+5,c2,c3))+cs6*(cr0*ui(i1  ,i2+6,c2,c3)+
     & cr1*ui(i1+1,i2+6,c2,c3)+cr2*ui(i1+2,i2+6,c2,c3)+cr3*ui(i1+3,i2+
     & 6,c2,c3)+cr4*ui(i1+4,i2+6,c2,c3)+cr5*ui(i1+5,i2+6,c2,c3)+cr6*
     & ui(i1+6,i2+6,c2,c3)+cr7*ui(i1+7,i2+6,c2,c3))+cs7*(cr0*ui(i1  ,
     & i2+7,c2,c3)+cr1*ui(i1+1,i2+7,c2,c3)+cr2*ui(i1+2,i2+7,c2,c3)+
     & cr3*ui(i1+3,i2+7,c2,c3)+cr4*ui(i1+4,i2+7,c2,c3)+cr5*ui(i1+5,i2+
     & 7,c2,c3)+cr6*ui(i1+6,i2+7,c2,c3)+cr7*ui(i1+7,i2+7,c2,c3))
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
! loops2d($interpSparseStorage33(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),ug(ip(i,1),ip(i,2),c2,c3)= r(i))
           if( c2a.eq.c2b .and. c3a.eq.c3b )then
             do c3=c3a,c3b
             do c2=c2a,c2b
             do i=nia,nib
! interpSparseStorage33(r(i))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q30(c(i,0,0,0))
                 cs0 = q30(c(i,1,0,0))
                 cr1 = q31(c(i,0,0,0))
                 cs1 = q31(c(i,1,0,0))
                 cr2 = q32(c(i,0,0,0))
                 cs2 = q32(c(i,1,0,0))
               r(i) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)+cr1*ui(i1+1,i2  ,c2,
     & c3)+cr2*ui(i1+2,i2  ,c2,c3))+cs1*(cr0*ui(i1  ,i2+1,c2,c3)+cr1*
     & ui(i1+1,i2+1,c2,c3)+cr2*ui(i1+2,i2+1,c2,c3))+cs2*(cr0*ui(i1  ,
     & i2+2,c2,c3)+cr1*ui(i1+1,i2+2,c2,c3)+cr2*ui(i1+2,i2+2,c2,c3))
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
! interpSparseStorage33(r(i))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q30(c(i,0,0,0))
                 cs0 = q30(c(i,1,0,0))
                 cr1 = q31(c(i,0,0,0))
                 cs1 = q31(c(i,1,0,0))
                 cr2 = q32(c(i,0,0,0))
                 cs2 = q32(c(i,1,0,0))
               r(i) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)+cr1*ui(i1+1,i2  ,c2,
     & c3)+cr2*ui(i1+2,i2  ,c2,c3))+cs1*(cr0*ui(i1  ,i2+1,c2,c3)+cr1*
     & ui(i1+1,i2+1,c2,c3)+cr2*ui(i1+2,i2+1,c2,c3))+cs2*(cr0*ui(i1  ,
     & i2+2,c2,c3)+cr1*ui(i1+1,i2+2,c2,c3)+cr2*ui(i1+2,i2+2,c2,c3))
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
! loops2d($interpSparseStorage22(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),ug(ip(i,1),ip(i,2),c2,c3)= r(i))
           if( c2a.eq.c2b .and. c3a.eq.c3b )then
             do c3=c3a,c3b
             do c2=c2a,c2b
             do i=nia,nib
! interpSparseStorage22(r(i))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q20(c(i,0,0,0))
                 cs0 = q20(c(i,1,0,0))
                 cr1 = q21(c(i,0,0,0))
                 cs1 = q21(c(i,1,0,0))
               r(i) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)+cr1*ui(i1+1,i2  ,c2,
     & c3))+cs1*(cr0*ui(i1  ,i2+1,c2,c3)+cr1*ui(i1+1,i2+1,c2,c3))
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
! interpSparseStorage22(r(i))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q20(c(i,0,0,0))
                 cs0 = q20(c(i,1,0,0))
                 cr1 = q21(c(i,0,0,0))
                 cs1 = q21(c(i,1,0,0))
               r(i) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)+cr1*ui(i1+1,i2  ,c2,
     & c3))+cs1*(cr0*ui(i1  ,i2+1,c2,c3)+cr1*ui(i1+1,i2+1,c2,c3))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i)))
               ug(ip(i,1),ip(i,2),c2,c3)=r(i)
             end do
             end do
             end do
           end if
         else if( width(1).eq.4 .and. width(2).eq.4 )then
! loops2d($interpSparseStorage44(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),ug(ip(i,1),ip(i,2),c2,c3)= r(i))
           if( c2a.eq.c2b .and. c3a.eq.c3b )then
             do c3=c3a,c3b
             do c2=c2a,c2b
             do i=nia,nib
! interpSparseStorage44(r(i))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q40(c(i,0,0,0))
                 cs0 = q40(c(i,1,0,0))
                 cr1 = q41(c(i,0,0,0))
                 cs1 = q41(c(i,1,0,0))
                 cr2 = q42(c(i,0,0,0))
                 cs2 = q42(c(i,1,0,0))
                 cr3 = q43(c(i,0,0,0))
                 cs3 = q43(c(i,1,0,0))
               r(i) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)+cr1*ui(i1+1,i2  ,c2,
     & c3)+cr2*ui(i1+2,i2  ,c2,c3)+cr3*ui(i1+3,i2  ,c2,c3))+cs1*(cr0*
     & ui(i1  ,i2+1,c2,c3)+cr1*ui(i1+1,i2+1,c2,c3)+cr2*ui(i1+2,i2+1,
     & c2,c3)+cr3*ui(i1+3,i2+1,c2,c3))+cs2*(cr0*ui(i1  ,i2+2,c2,c3)+
     & cr1*ui(i1+1,i2+2,c2,c3)+cr2*ui(i1+2,i2+2,c2,c3)+cr3*ui(i1+3,i2+
     & 2,c2,c3))+cs3*(cr0*ui(i1  ,i2+3,c2,c3)+cr1*ui(i1+1,i2+3,c2,c3)+
     & cr2*ui(i1+2,i2+3,c2,c3)+cr3*ui(i1+3,i2+3,c2,c3))
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
! interpSparseStorage44(r(i))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q40(c(i,0,0,0))
                 cs0 = q40(c(i,1,0,0))
                 cr1 = q41(c(i,0,0,0))
                 cs1 = q41(c(i,1,0,0))
                 cr2 = q42(c(i,0,0,0))
                 cs2 = q42(c(i,1,0,0))
                 cr3 = q43(c(i,0,0,0))
                 cs3 = q43(c(i,1,0,0))
               r(i) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)+cr1*ui(i1+1,i2  ,c2,
     & c3)+cr2*ui(i1+2,i2  ,c2,c3)+cr3*ui(i1+3,i2  ,c2,c3))+cs1*(cr0*
     & ui(i1  ,i2+1,c2,c3)+cr1*ui(i1+1,i2+1,c2,c3)+cr2*ui(i1+2,i2+1,
     & c2,c3)+cr3*ui(i1+3,i2+1,c2,c3))+cs2*(cr0*ui(i1  ,i2+2,c2,c3)+
     & cr1*ui(i1+1,i2+2,c2,c3)+cr2*ui(i1+2,i2+2,c2,c3)+cr3*ui(i1+3,i2+
     & 2,c2,c3))+cs3*(cr0*ui(i1  ,i2+3,c2,c3)+cr1*ui(i1+1,i2+3,c2,c3)+
     & cr2*ui(i1+2,i2+3,c2,c3)+cr3*ui(i1+3,i2+3,c2,c3))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i)))
               ug(ip(i,1),ip(i,2),c2,c3)=r(i)
             end do
             end do
             end do
           end if
         else if( width(1).eq.5 .and. width(2).eq.5 )then
! loops2d($interpSparseStorage55(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),ug(ip(i,1),ip(i,2),c2,c3)= r(i))
           if( c2a.eq.c2b .and. c3a.eq.c3b )then
             do c3=c3a,c3b
             do c2=c2a,c2b
             do i=nia,nib
! interpSparseStorage55(r(i))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q50(c(i,0,0,0))
                 cs0 = q50(c(i,1,0,0))
                 cr1 = q51(c(i,0,0,0))
                 cs1 = q51(c(i,1,0,0))
                 cr2 = q52(c(i,0,0,0))
                 cs2 = q52(c(i,1,0,0))
                 cr3 = q53(c(i,0,0,0))
                 cs3 = q53(c(i,1,0,0))
                 cr4 = q54(c(i,0,0,0))
                 cs4 = q54(c(i,1,0,0))
               r(i) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)+cr1*ui(i1+1,i2  ,c2,
     & c3)+cr2*ui(i1+2,i2  ,c2,c3)+cr3*ui(i1+3,i2  ,c2,c3)+cr4*ui(i1+
     & 4,i2  ,c2,c3))+cs1*(cr0*ui(i1  ,i2+1,c2,c3)+cr1*ui(i1+1,i2+1,
     & c2,c3)+cr2*ui(i1+2,i2+1,c2,c3)+cr3*ui(i1+3,i2+1,c2,c3)+cr4*ui(
     & i1+4,i2+1,c2,c3))+cs2*(cr0*ui(i1  ,i2+2,c2,c3)+cr1*ui(i1+1,i2+
     & 2,c2,c3)+cr2*ui(i1+2,i2+2,c2,c3)+cr3*ui(i1+3,i2+2,c2,c3)+cr4*
     & ui(i1+4,i2+2,c2,c3))+cs3*(cr0*ui(i1  ,i2+3,c2,c3)+cr1*ui(i1+1,
     & i2+3,c2,c3)+cr2*ui(i1+2,i2+3,c2,c3)+cr3*ui(i1+3,i2+3,c2,c3)+
     & cr4*ui(i1+4,i2+3,c2,c3))+cs4*(cr0*ui(i1  ,i2+4,c2,c3)+cr1*ui(
     & i1+1,i2+4,c2,c3)+cr2*ui(i1+2,i2+4,c2,c3)+cr3*ui(i1+3,i2+4,c2,
     & c3)+cr4*ui(i1+4,i2+4,c2,c3))
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
! interpSparseStorage55(r(i))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q50(c(i,0,0,0))
                 cs0 = q50(c(i,1,0,0))
                 cr1 = q51(c(i,0,0,0))
                 cs1 = q51(c(i,1,0,0))
                 cr2 = q52(c(i,0,0,0))
                 cs2 = q52(c(i,1,0,0))
                 cr3 = q53(c(i,0,0,0))
                 cs3 = q53(c(i,1,0,0))
                 cr4 = q54(c(i,0,0,0))
                 cs4 = q54(c(i,1,0,0))
               r(i) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)+cr1*ui(i1+1,i2  ,c2,
     & c3)+cr2*ui(i1+2,i2  ,c2,c3)+cr3*ui(i1+3,i2  ,c2,c3)+cr4*ui(i1+
     & 4,i2  ,c2,c3))+cs1*(cr0*ui(i1  ,i2+1,c2,c3)+cr1*ui(i1+1,i2+1,
     & c2,c3)+cr2*ui(i1+2,i2+1,c2,c3)+cr3*ui(i1+3,i2+1,c2,c3)+cr4*ui(
     & i1+4,i2+1,c2,c3))+cs2*(cr0*ui(i1  ,i2+2,c2,c3)+cr1*ui(i1+1,i2+
     & 2,c2,c3)+cr2*ui(i1+2,i2+2,c2,c3)+cr3*ui(i1+3,i2+2,c2,c3)+cr4*
     & ui(i1+4,i2+2,c2,c3))+cs3*(cr0*ui(i1  ,i2+3,c2,c3)+cr1*ui(i1+1,
     & i2+3,c2,c3)+cr2*ui(i1+2,i2+3,c2,c3)+cr3*ui(i1+3,i2+3,c2,c3)+
     & cr4*ui(i1+4,i2+3,c2,c3))+cs4*(cr0*ui(i1  ,i2+4,c2,c3)+cr1*ui(
     & i1+1,i2+4,c2,c3)+cr2*ui(i1+2,i2+4,c2,c3)+cr3*ui(i1+3,i2+4,c2,
     & c3)+cr4*ui(i1+4,i2+4,c2,c3))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i)))
               ug(ip(i,1),ip(i,2),c2,c3)=r(i)
             end do
             end do
             end do
           end if
         else if( width(1).eq.6 .and. width(2).eq.6 )then
! loops2d($interpSparseStorage66(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),ug(ip(i,1),ip(i,2),c2,c3)= r(i))
           if( c2a.eq.c2b .and. c3a.eq.c3b )then
             do c3=c3a,c3b
             do c2=c2a,c2b
             do i=nia,nib
! interpSparseStorage66(r(i))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q60(c(i,0,0,0))
                 cs0 = q60(c(i,1,0,0))
                 cr1 = q61(c(i,0,0,0))
                 cs1 = q61(c(i,1,0,0))
                 cr2 = q62(c(i,0,0,0))
                 cs2 = q62(c(i,1,0,0))
                 cr3 = q63(c(i,0,0,0))
                 cs3 = q63(c(i,1,0,0))
                 cr4 = q64(c(i,0,0,0))
                 cs4 = q64(c(i,1,0,0))
                 cr5 = q65(c(i,0,0,0))
                 cs5 = q65(c(i,1,0,0))
               r(i) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)+cr1*ui(i1+1,i2  ,c2,
     & c3)+cr2*ui(i1+2,i2  ,c2,c3)+cr3*ui(i1+3,i2  ,c2,c3)+cr4*ui(i1+
     & 4,i2  ,c2,c3)+cr5*ui(i1+5,i2  ,c2,c3))+cs1*(cr0*ui(i1  ,i2+1,
     & c2,c3)+cr1*ui(i1+1,i2+1,c2,c3)+cr2*ui(i1+2,i2+1,c2,c3)+cr3*ui(
     & i1+3,i2+1,c2,c3)+cr4*ui(i1+4,i2+1,c2,c3)+cr5*ui(i1+5,i2+1,c2,
     & c3))+cs2*(cr0*ui(i1  ,i2+2,c2,c3)+cr1*ui(i1+1,i2+2,c2,c3)+cr2*
     & ui(i1+2,i2+2,c2,c3)+cr3*ui(i1+3,i2+2,c2,c3)+cr4*ui(i1+4,i2+2,
     & c2,c3)+cr5*ui(i1+5,i2+2,c2,c3))+cs3*(cr0*ui(i1  ,i2+3,c2,c3)+
     & cr1*ui(i1+1,i2+3,c2,c3)+cr2*ui(i1+2,i2+3,c2,c3)+cr3*ui(i1+3,i2+
     & 3,c2,c3)+cr4*ui(i1+4,i2+3,c2,c3)+cr5*ui(i1+5,i2+3,c2,c3))+cs4*(
     & cr0*ui(i1  ,i2+4,c2,c3)+cr1*ui(i1+1,i2+4,c2,c3)+cr2*ui(i1+2,i2+
     & 4,c2,c3)+cr3*ui(i1+3,i2+4,c2,c3)+cr4*ui(i1+4,i2+4,c2,c3)+cr5*
     & ui(i1+5,i2+4,c2,c3))+cs5*(cr0*ui(i1  ,i2+5,c2,c3)+cr1*ui(i1+1,
     & i2+5,c2,c3)+cr2*ui(i1+2,i2+5,c2,c3)+cr3*ui(i1+3,i2+5,c2,c3)+
     & cr4*ui(i1+4,i2+5,c2,c3)+cr5*ui(i1+5,i2+5,c2,c3))
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
! interpSparseStorage66(r(i))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q60(c(i,0,0,0))
                 cs0 = q60(c(i,1,0,0))
                 cr1 = q61(c(i,0,0,0))
                 cs1 = q61(c(i,1,0,0))
                 cr2 = q62(c(i,0,0,0))
                 cs2 = q62(c(i,1,0,0))
                 cr3 = q63(c(i,0,0,0))
                 cs3 = q63(c(i,1,0,0))
                 cr4 = q64(c(i,0,0,0))
                 cs4 = q64(c(i,1,0,0))
                 cr5 = q65(c(i,0,0,0))
                 cs5 = q65(c(i,1,0,0))
               r(i) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)+cr1*ui(i1+1,i2  ,c2,
     & c3)+cr2*ui(i1+2,i2  ,c2,c3)+cr3*ui(i1+3,i2  ,c2,c3)+cr4*ui(i1+
     & 4,i2  ,c2,c3)+cr5*ui(i1+5,i2  ,c2,c3))+cs1*(cr0*ui(i1  ,i2+1,
     & c2,c3)+cr1*ui(i1+1,i2+1,c2,c3)+cr2*ui(i1+2,i2+1,c2,c3)+cr3*ui(
     & i1+3,i2+1,c2,c3)+cr4*ui(i1+4,i2+1,c2,c3)+cr5*ui(i1+5,i2+1,c2,
     & c3))+cs2*(cr0*ui(i1  ,i2+2,c2,c3)+cr1*ui(i1+1,i2+2,c2,c3)+cr2*
     & ui(i1+2,i2+2,c2,c3)+cr3*ui(i1+3,i2+2,c2,c3)+cr4*ui(i1+4,i2+2,
     & c2,c3)+cr5*ui(i1+5,i2+2,c2,c3))+cs3*(cr0*ui(i1  ,i2+3,c2,c3)+
     & cr1*ui(i1+1,i2+3,c2,c3)+cr2*ui(i1+2,i2+3,c2,c3)+cr3*ui(i1+3,i2+
     & 3,c2,c3)+cr4*ui(i1+4,i2+3,c2,c3)+cr5*ui(i1+5,i2+3,c2,c3))+cs4*(
     & cr0*ui(i1  ,i2+4,c2,c3)+cr1*ui(i1+1,i2+4,c2,c3)+cr2*ui(i1+2,i2+
     & 4,c2,c3)+cr3*ui(i1+3,i2+4,c2,c3)+cr4*ui(i1+4,i2+4,c2,c3)+cr5*
     & ui(i1+5,i2+4,c2,c3))+cs5*(cr0*ui(i1  ,i2+5,c2,c3)+cr1*ui(i1+1,
     & i2+5,c2,c3)+cr2*ui(i1+2,i2+5,c2,c3)+cr3*ui(i1+3,i2+5,c2,c3)+
     & cr4*ui(i1+4,i2+5,c2,c3)+cr5*ui(i1+5,i2+5,c2,c3))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i)))
               ug(ip(i,1),ip(i,2),c2,c3)=r(i)
             end do
             end do
             end do
           end if
         else if( width(1).eq.7 .and. width(2).eq.7 )then
! loops2d($interpSparseStorage77(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),ug(ip(i,1),ip(i,2),c2,c3)= r(i))
           if( c2a.eq.c2b .and. c3a.eq.c3b )then
             do c3=c3a,c3b
             do c2=c2a,c2b
             do i=nia,nib
! interpSparseStorage77(r(i))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q70(c(i,0,0,0))
                 cs0 = q70(c(i,1,0,0))
                 cr1 = q71(c(i,0,0,0))
                 cs1 = q71(c(i,1,0,0))
                 cr2 = q72(c(i,0,0,0))
                 cs2 = q72(c(i,1,0,0))
                 cr3 = q73(c(i,0,0,0))
                 cs3 = q73(c(i,1,0,0))
                 cr4 = q74(c(i,0,0,0))
                 cs4 = q74(c(i,1,0,0))
                 cr5 = q75(c(i,0,0,0))
                 cs5 = q75(c(i,1,0,0))
                 cr6 = q76(c(i,0,0,0))
                 cs6 = q76(c(i,1,0,0))
               r(i) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)+cr1*ui(i1+1,i2  ,c2,
     & c3)+cr2*ui(i1+2,i2  ,c2,c3)+cr3*ui(i1+3,i2  ,c2,c3)+cr4*ui(i1+
     & 4,i2  ,c2,c3)+cr5*ui(i1+5,i2  ,c2,c3)+cr6*ui(i1+6,i2  ,c2,c3))+
     & cs1*(cr0*ui(i1  ,i2+1,c2,c3)+cr1*ui(i1+1,i2+1,c2,c3)+cr2*ui(i1+
     & 2,i2+1,c2,c3)+cr3*ui(i1+3,i2+1,c2,c3)+cr4*ui(i1+4,i2+1,c2,c3)+
     & cr5*ui(i1+5,i2+1,c2,c3)+cr6*ui(i1+6,i2+1,c2,c3))+cs2*(cr0*ui(
     & i1  ,i2+2,c2,c3)+cr1*ui(i1+1,i2+2,c2,c3)+cr2*ui(i1+2,i2+2,c2,
     & c3)+cr3*ui(i1+3,i2+2,c2,c3)+cr4*ui(i1+4,i2+2,c2,c3)+cr5*ui(i1+
     & 5,i2+2,c2,c3)+cr6*ui(i1+6,i2+2,c2,c3))+cs3*(cr0*ui(i1  ,i2+3,
     & c2,c3)+cr1*ui(i1+1,i2+3,c2,c3)+cr2*ui(i1+2,i2+3,c2,c3)+cr3*ui(
     & i1+3,i2+3,c2,c3)+cr4*ui(i1+4,i2+3,c2,c3)+cr5*ui(i1+5,i2+3,c2,
     & c3)+cr6*ui(i1+6,i2+3,c2,c3))+cs4*(cr0*ui(i1  ,i2+4,c2,c3)+cr1*
     & ui(i1+1,i2+4,c2,c3)+cr2*ui(i1+2,i2+4,c2,c3)+cr3*ui(i1+3,i2+4,
     & c2,c3)+cr4*ui(i1+4,i2+4,c2,c3)+cr5*ui(i1+5,i2+4,c2,c3)+cr6*ui(
     & i1+6,i2+4,c2,c3))+cs5*(cr0*ui(i1  ,i2+5,c2,c3)+cr1*ui(i1+1,i2+
     & 5,c2,c3)+cr2*ui(i1+2,i2+5,c2,c3)+cr3*ui(i1+3,i2+5,c2,c3)+cr4*
     & ui(i1+4,i2+5,c2,c3)+cr5*ui(i1+5,i2+5,c2,c3)+cr6*ui(i1+6,i2+5,
     & c2,c3))+cs6*(cr0*ui(i1  ,i2+6,c2,c3)+cr1*ui(i1+1,i2+6,c2,c3)+
     & cr2*ui(i1+2,i2+6,c2,c3)+cr3*ui(i1+3,i2+6,c2,c3)+cr4*ui(i1+4,i2+
     & 6,c2,c3)+cr5*ui(i1+5,i2+6,c2,c3)+cr6*ui(i1+6,i2+6,c2,c3))
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
! interpSparseStorage77(r(i))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q70(c(i,0,0,0))
                 cs0 = q70(c(i,1,0,0))
                 cr1 = q71(c(i,0,0,0))
                 cs1 = q71(c(i,1,0,0))
                 cr2 = q72(c(i,0,0,0))
                 cs2 = q72(c(i,1,0,0))
                 cr3 = q73(c(i,0,0,0))
                 cs3 = q73(c(i,1,0,0))
                 cr4 = q74(c(i,0,0,0))
                 cs4 = q74(c(i,1,0,0))
                 cr5 = q75(c(i,0,0,0))
                 cs5 = q75(c(i,1,0,0))
                 cr6 = q76(c(i,0,0,0))
                 cs6 = q76(c(i,1,0,0))
               r(i) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)+cr1*ui(i1+1,i2  ,c2,
     & c3)+cr2*ui(i1+2,i2  ,c2,c3)+cr3*ui(i1+3,i2  ,c2,c3)+cr4*ui(i1+
     & 4,i2  ,c2,c3)+cr5*ui(i1+5,i2  ,c2,c3)+cr6*ui(i1+6,i2  ,c2,c3))+
     & cs1*(cr0*ui(i1  ,i2+1,c2,c3)+cr1*ui(i1+1,i2+1,c2,c3)+cr2*ui(i1+
     & 2,i2+1,c2,c3)+cr3*ui(i1+3,i2+1,c2,c3)+cr4*ui(i1+4,i2+1,c2,c3)+
     & cr5*ui(i1+5,i2+1,c2,c3)+cr6*ui(i1+6,i2+1,c2,c3))+cs2*(cr0*ui(
     & i1  ,i2+2,c2,c3)+cr1*ui(i1+1,i2+2,c2,c3)+cr2*ui(i1+2,i2+2,c2,
     & c3)+cr3*ui(i1+3,i2+2,c2,c3)+cr4*ui(i1+4,i2+2,c2,c3)+cr5*ui(i1+
     & 5,i2+2,c2,c3)+cr6*ui(i1+6,i2+2,c2,c3))+cs3*(cr0*ui(i1  ,i2+3,
     & c2,c3)+cr1*ui(i1+1,i2+3,c2,c3)+cr2*ui(i1+2,i2+3,c2,c3)+cr3*ui(
     & i1+3,i2+3,c2,c3)+cr4*ui(i1+4,i2+3,c2,c3)+cr5*ui(i1+5,i2+3,c2,
     & c3)+cr6*ui(i1+6,i2+3,c2,c3))+cs4*(cr0*ui(i1  ,i2+4,c2,c3)+cr1*
     & ui(i1+1,i2+4,c2,c3)+cr2*ui(i1+2,i2+4,c2,c3)+cr3*ui(i1+3,i2+4,
     & c2,c3)+cr4*ui(i1+4,i2+4,c2,c3)+cr5*ui(i1+5,i2+4,c2,c3)+cr6*ui(
     & i1+6,i2+4,c2,c3))+cs5*(cr0*ui(i1  ,i2+5,c2,c3)+cr1*ui(i1+1,i2+
     & 5,c2,c3)+cr2*ui(i1+2,i2+5,c2,c3)+cr3*ui(i1+3,i2+5,c2,c3)+cr4*
     & ui(i1+4,i2+5,c2,c3)+cr5*ui(i1+5,i2+5,c2,c3)+cr6*ui(i1+6,i2+5,
     & c2,c3))+cs6*(cr0*ui(i1  ,i2+6,c2,c3)+cr1*ui(i1+1,i2+6,c2,c3)+
     & cr2*ui(i1+2,i2+6,c2,c3)+cr3*ui(i1+3,i2+6,c2,c3)+cr4*ui(i1+4,i2+
     & 6,c2,c3)+cr5*ui(i1+5,i2+6,c2,c3)+cr6*ui(i1+6,i2+6,c2,c3))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i)))
               ug(ip(i,1),ip(i,2),c2,c3)=r(i)
             end do
             end do
             end do
           end if
         else if( width(1).eq.8 .and. width(2).eq.8 )then
! loops2d($interpSparseStorage88(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),ug(ip(i,1),ip(i,2),c2,c3)= r(i))
           if( c2a.eq.c2b .and. c3a.eq.c3b )then
             do c3=c3a,c3b
             do c2=c2a,c2b
             do i=nia,nib
! interpSparseStorage88(r(i))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q80(c(i,0,0,0))
                 cs0 = q80(c(i,1,0,0))
                 cr1 = q81(c(i,0,0,0))
                 cs1 = q81(c(i,1,0,0))
                 cr2 = q82(c(i,0,0,0))
                 cs2 = q82(c(i,1,0,0))
                 cr3 = q83(c(i,0,0,0))
                 cs3 = q83(c(i,1,0,0))
                 cr4 = q84(c(i,0,0,0))
                 cs4 = q84(c(i,1,0,0))
                 cr5 = q85(c(i,0,0,0))
                 cs5 = q85(c(i,1,0,0))
                 cr6 = q86(c(i,0,0,0))
                 cs6 = q86(c(i,1,0,0))
                 cr7 = q87(c(i,0,0,0))
                 cs7 = q87(c(i,1,0,0))
               r(i) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)+cr1*ui(i1+1,i2  ,c2,
     & c3)+cr2*ui(i1+2,i2  ,c2,c3)+cr3*ui(i1+3,i2  ,c2,c3)+cr4*ui(i1+
     & 4,i2  ,c2,c3)+cr5*ui(i1+5,i2  ,c2,c3)+cr6*ui(i1+6,i2  ,c2,c3)+
     & cr7*ui(i1+7,i2  ,c2,c3))+cs1*(cr0*ui(i1  ,i2+1,c2,c3)+cr1*ui(
     & i1+1,i2+1,c2,c3)+cr2*ui(i1+2,i2+1,c2,c3)+cr3*ui(i1+3,i2+1,c2,
     & c3)+cr4*ui(i1+4,i2+1,c2,c3)+cr5*ui(i1+5,i2+1,c2,c3)+cr6*ui(i1+
     & 6,i2+1,c2,c3)+cr7*ui(i1+7,i2+1,c2,c3))+cs2*(cr0*ui(i1  ,i2+2,
     & c2,c3)+cr1*ui(i1+1,i2+2,c2,c3)+cr2*ui(i1+2,i2+2,c2,c3)+cr3*ui(
     & i1+3,i2+2,c2,c3)+cr4*ui(i1+4,i2+2,c2,c3)+cr5*ui(i1+5,i2+2,c2,
     & c3)+cr6*ui(i1+6,i2+2,c2,c3)+cr7*ui(i1+7,i2+2,c2,c3))+cs3*(cr0*
     & ui(i1  ,i2+3,c2,c3)+cr1*ui(i1+1,i2+3,c2,c3)+cr2*ui(i1+2,i2+3,
     & c2,c3)+cr3*ui(i1+3,i2+3,c2,c3)+cr4*ui(i1+4,i2+3,c2,c3)+cr5*ui(
     & i1+5,i2+3,c2,c3)+cr6*ui(i1+6,i2+3,c2,c3)+cr7*ui(i1+7,i2+3,c2,
     & c3))+cs4*(cr0*ui(i1  ,i2+4,c2,c3)+cr1*ui(i1+1,i2+4,c2,c3)+cr2*
     & ui(i1+2,i2+4,c2,c3)+cr3*ui(i1+3,i2+4,c2,c3)+cr4*ui(i1+4,i2+4,
     & c2,c3)+cr5*ui(i1+5,i2+4,c2,c3)+cr6*ui(i1+6,i2+4,c2,c3)+cr7*ui(
     & i1+7,i2+4,c2,c3))+cs5*(cr0*ui(i1  ,i2+5,c2,c3)+cr1*ui(i1+1,i2+
     & 5,c2,c3)+cr2*ui(i1+2,i2+5,c2,c3)+cr3*ui(i1+3,i2+5,c2,c3)+cr4*
     & ui(i1+4,i2+5,c2,c3)+cr5*ui(i1+5,i2+5,c2,c3)+cr6*ui(i1+6,i2+5,
     & c2,c3)+cr7*ui(i1+7,i2+5,c2,c3))+cs6*(cr0*ui(i1  ,i2+6,c2,c3)+
     & cr1*ui(i1+1,i2+6,c2,c3)+cr2*ui(i1+2,i2+6,c2,c3)+cr3*ui(i1+3,i2+
     & 6,c2,c3)+cr4*ui(i1+4,i2+6,c2,c3)+cr5*ui(i1+5,i2+6,c2,c3)+cr6*
     & ui(i1+6,i2+6,c2,c3)+cr7*ui(i1+7,i2+6,c2,c3))+cs7*(cr0*ui(i1  ,
     & i2+7,c2,c3)+cr1*ui(i1+1,i2+7,c2,c3)+cr2*ui(i1+2,i2+7,c2,c3)+
     & cr3*ui(i1+3,i2+7,c2,c3)+cr4*ui(i1+4,i2+7,c2,c3)+cr5*ui(i1+5,i2+
     & 7,c2,c3)+cr6*ui(i1+6,i2+7,c2,c3)+cr7*ui(i1+7,i2+7,c2,c3))
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
! interpSparseStorage88(r(i))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q80(c(i,0,0,0))
                 cs0 = q80(c(i,1,0,0))
                 cr1 = q81(c(i,0,0,0))
                 cs1 = q81(c(i,1,0,0))
                 cr2 = q82(c(i,0,0,0))
                 cs2 = q82(c(i,1,0,0))
                 cr3 = q83(c(i,0,0,0))
                 cs3 = q83(c(i,1,0,0))
                 cr4 = q84(c(i,0,0,0))
                 cs4 = q84(c(i,1,0,0))
                 cr5 = q85(c(i,0,0,0))
                 cs5 = q85(c(i,1,0,0))
                 cr6 = q86(c(i,0,0,0))
                 cs6 = q86(c(i,1,0,0))
                 cr7 = q87(c(i,0,0,0))
                 cs7 = q87(c(i,1,0,0))
               r(i) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)+cr1*ui(i1+1,i2  ,c2,
     & c3)+cr2*ui(i1+2,i2  ,c2,c3)+cr3*ui(i1+3,i2  ,c2,c3)+cr4*ui(i1+
     & 4,i2  ,c2,c3)+cr5*ui(i1+5,i2  ,c2,c3)+cr6*ui(i1+6,i2  ,c2,c3)+
     & cr7*ui(i1+7,i2  ,c2,c3))+cs1*(cr0*ui(i1  ,i2+1,c2,c3)+cr1*ui(
     & i1+1,i2+1,c2,c3)+cr2*ui(i1+2,i2+1,c2,c3)+cr3*ui(i1+3,i2+1,c2,
     & c3)+cr4*ui(i1+4,i2+1,c2,c3)+cr5*ui(i1+5,i2+1,c2,c3)+cr6*ui(i1+
     & 6,i2+1,c2,c3)+cr7*ui(i1+7,i2+1,c2,c3))+cs2*(cr0*ui(i1  ,i2+2,
     & c2,c3)+cr1*ui(i1+1,i2+2,c2,c3)+cr2*ui(i1+2,i2+2,c2,c3)+cr3*ui(
     & i1+3,i2+2,c2,c3)+cr4*ui(i1+4,i2+2,c2,c3)+cr5*ui(i1+5,i2+2,c2,
     & c3)+cr6*ui(i1+6,i2+2,c2,c3)+cr7*ui(i1+7,i2+2,c2,c3))+cs3*(cr0*
     & ui(i1  ,i2+3,c2,c3)+cr1*ui(i1+1,i2+3,c2,c3)+cr2*ui(i1+2,i2+3,
     & c2,c3)+cr3*ui(i1+3,i2+3,c2,c3)+cr4*ui(i1+4,i2+3,c2,c3)+cr5*ui(
     & i1+5,i2+3,c2,c3)+cr6*ui(i1+6,i2+3,c2,c3)+cr7*ui(i1+7,i2+3,c2,
     & c3))+cs4*(cr0*ui(i1  ,i2+4,c2,c3)+cr1*ui(i1+1,i2+4,c2,c3)+cr2*
     & ui(i1+2,i2+4,c2,c3)+cr3*ui(i1+3,i2+4,c2,c3)+cr4*ui(i1+4,i2+4,
     & c2,c3)+cr5*ui(i1+5,i2+4,c2,c3)+cr6*ui(i1+6,i2+4,c2,c3)+cr7*ui(
     & i1+7,i2+4,c2,c3))+cs5*(cr0*ui(i1  ,i2+5,c2,c3)+cr1*ui(i1+1,i2+
     & 5,c2,c3)+cr2*ui(i1+2,i2+5,c2,c3)+cr3*ui(i1+3,i2+5,c2,c3)+cr4*
     & ui(i1+4,i2+5,c2,c3)+cr5*ui(i1+5,i2+5,c2,c3)+cr6*ui(i1+6,i2+5,
     & c2,c3)+cr7*ui(i1+7,i2+5,c2,c3))+cs6*(cr0*ui(i1  ,i2+6,c2,c3)+
     & cr1*ui(i1+1,i2+6,c2,c3)+cr2*ui(i1+2,i2+6,c2,c3)+cr3*ui(i1+3,i2+
     & 6,c2,c3)+cr4*ui(i1+4,i2+6,c2,c3)+cr5*ui(i1+5,i2+6,c2,c3)+cr6*
     & ui(i1+6,i2+6,c2,c3)+cr7*ui(i1+7,i2+6,c2,c3))+cs7*(cr0*ui(i1  ,
     & i2+7,c2,c3)+cr1*ui(i1+1,i2+7,c2,c3)+cr2*ui(i1+2,i2+7,c2,c3)+
     & cr3*ui(i1+3,i2+7,c2,c3)+cr4*ui(i1+4,i2+7,c2,c3)+cr5*ui(i1+5,i2+
     & 7,c2,c3)+cr6*ui(i1+6,i2+7,c2,c3)+cr7*ui(i1+7,i2+7,c2,c3))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i)))
               ug(ip(i,1),ip(i,2),c2,c3)=r(i)
             end do
             end do
             end do
           end if
         else if( width(1).eq.9 .and. width(2).eq.9 )then
! loops2d($interpSparseStorage99(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),ug(ip(i,1),ip(i,2),c2,c3)= r(i))
           if( c2a.eq.c2b .and. c3a.eq.c3b )then
             do c3=c3a,c3b
             do c2=c2a,c2b
             do i=nia,nib
! interpSparseStorage99(r(i))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q90(c(i,0,0,0))
                 cs0 = q90(c(i,1,0,0))
                 cr1 = q91(c(i,0,0,0))
                 cs1 = q91(c(i,1,0,0))
                 cr2 = q92(c(i,0,0,0))
                 cs2 = q92(c(i,1,0,0))
                 cr3 = q93(c(i,0,0,0))
                 cs3 = q93(c(i,1,0,0))
                 cr4 = q94(c(i,0,0,0))
                 cs4 = q94(c(i,1,0,0))
                 cr5 = q95(c(i,0,0,0))
                 cs5 = q95(c(i,1,0,0))
                 cr6 = q96(c(i,0,0,0))
                 cs6 = q96(c(i,1,0,0))
                 cr7 = q97(c(i,0,0,0))
                 cs7 = q97(c(i,1,0,0))
                 cr8 = q98(c(i,0,0,0))
                 cs8 = q98(c(i,1,0,0))
               r(i) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)+cr1*ui(i1+1,i2  ,c2,
     & c3)+cr2*ui(i1+2,i2  ,c2,c3)+cr3*ui(i1+3,i2  ,c2,c3)+cr4*ui(i1+
     & 4,i2  ,c2,c3)+cr5*ui(i1+5,i2  ,c2,c3)+cr6*ui(i1+6,i2  ,c2,c3)+
     & cr7*ui(i1+7,i2  ,c2,c3)+cr8*ui(i1+8,i2  ,c2,c3))+cs1*(cr0*ui(
     & i1  ,i2+1,c2,c3)+cr1*ui(i1+1,i2+1,c2,c3)+cr2*ui(i1+2,i2+1,c2,
     & c3)+cr3*ui(i1+3,i2+1,c2,c3)+cr4*ui(i1+4,i2+1,c2,c3)+cr5*ui(i1+
     & 5,i2+1,c2,c3)+cr6*ui(i1+6,i2+1,c2,c3)+cr7*ui(i1+7,i2+1,c2,c3)+
     & cr8*ui(i1+8,i2+1,c2,c3))+cs2*(cr0*ui(i1  ,i2+2,c2,c3)+cr1*ui(
     & i1+1,i2+2,c2,c3)+cr2*ui(i1+2,i2+2,c2,c3)+cr3*ui(i1+3,i2+2,c2,
     & c3)+cr4*ui(i1+4,i2+2,c2,c3)+cr5*ui(i1+5,i2+2,c2,c3)+cr6*ui(i1+
     & 6,i2+2,c2,c3)+cr7*ui(i1+7,i2+2,c2,c3)+cr8*ui(i1+8,i2+2,c2,c3))+
     & cs3*(cr0*ui(i1  ,i2+3,c2,c3)+cr1*ui(i1+1,i2+3,c2,c3)+cr2*ui(i1+
     & 2,i2+3,c2,c3)+cr3*ui(i1+3,i2+3,c2,c3)+cr4*ui(i1+4,i2+3,c2,c3)+
     & cr5*ui(i1+5,i2+3,c2,c3)+cr6*ui(i1+6,i2+3,c2,c3)+cr7*ui(i1+7,i2+
     & 3,c2,c3)+cr8*ui(i1+8,i2+3,c2,c3))+cs4*(cr0*ui(i1  ,i2+4,c2,c3)+
     & cr1*ui(i1+1,i2+4,c2,c3)+cr2*ui(i1+2,i2+4,c2,c3)+cr3*ui(i1+3,i2+
     & 4,c2,c3)+cr4*ui(i1+4,i2+4,c2,c3)+cr5*ui(i1+5,i2+4,c2,c3)+cr6*
     & ui(i1+6,i2+4,c2,c3)+cr7*ui(i1+7,i2+4,c2,c3)+cr8*ui(i1+8,i2+4,
     & c2,c3))+cs5*(cr0*ui(i1  ,i2+5,c2,c3)+cr1*ui(i1+1,i2+5,c2,c3)+
     & cr2*ui(i1+2,i2+5,c2,c3)+cr3*ui(i1+3,i2+5,c2,c3)+cr4*ui(i1+4,i2+
     & 5,c2,c3)+cr5*ui(i1+5,i2+5,c2,c3)+cr6*ui(i1+6,i2+5,c2,c3)+cr7*
     & ui(i1+7,i2+5,c2,c3)+cr8*ui(i1+8,i2+5,c2,c3))+cs6*(cr0*ui(i1  ,
     & i2+6,c2,c3)+cr1*ui(i1+1,i2+6,c2,c3)+cr2*ui(i1+2,i2+6,c2,c3)+
     & cr3*ui(i1+3,i2+6,c2,c3)+cr4*ui(i1+4,i2+6,c2,c3)+cr5*ui(i1+5,i2+
     & 6,c2,c3)+cr6*ui(i1+6,i2+6,c2,c3)+cr7*ui(i1+7,i2+6,c2,c3)+cr8*
     & ui(i1+8,i2+6,c2,c3))+cs7*(cr0*ui(i1  ,i2+7,c2,c3)+cr1*ui(i1+1,
     & i2+7,c2,c3)+cr2*ui(i1+2,i2+7,c2,c3)+cr3*ui(i1+3,i2+7,c2,c3)+
     & cr4*ui(i1+4,i2+7,c2,c3)+cr5*ui(i1+5,i2+7,c2,c3)+cr6*ui(i1+6,i2+
     & 7,c2,c3)+cr7*ui(i1+7,i2+7,c2,c3)+cr8*ui(i1+8,i2+7,c2,c3))+cs8*(
     & cr0*ui(i1  ,i2+8,c2,c3)+cr1*ui(i1+1,i2+8,c2,c3)+cr2*ui(i1+2,i2+
     & 8,c2,c3)+cr3*ui(i1+3,i2+8,c2,c3)+cr4*ui(i1+4,i2+8,c2,c3)+cr5*
     & ui(i1+5,i2+8,c2,c3)+cr6*ui(i1+6,i2+8,c2,c3)+cr7*ui(i1+7,i2+8,
     & c2,c3)+cr8*ui(i1+8,i2+8,c2,c3))
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
! interpSparseStorage99(r(i))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q90(c(i,0,0,0))
                 cs0 = q90(c(i,1,0,0))
                 cr1 = q91(c(i,0,0,0))
                 cs1 = q91(c(i,1,0,0))
                 cr2 = q92(c(i,0,0,0))
                 cs2 = q92(c(i,1,0,0))
                 cr3 = q93(c(i,0,0,0))
                 cs3 = q93(c(i,1,0,0))
                 cr4 = q94(c(i,0,0,0))
                 cs4 = q94(c(i,1,0,0))
                 cr5 = q95(c(i,0,0,0))
                 cs5 = q95(c(i,1,0,0))
                 cr6 = q96(c(i,0,0,0))
                 cs6 = q96(c(i,1,0,0))
                 cr7 = q97(c(i,0,0,0))
                 cs7 = q97(c(i,1,0,0))
                 cr8 = q98(c(i,0,0,0))
                 cs8 = q98(c(i,1,0,0))
               r(i) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)+cr1*ui(i1+1,i2  ,c2,
     & c3)+cr2*ui(i1+2,i2  ,c2,c3)+cr3*ui(i1+3,i2  ,c2,c3)+cr4*ui(i1+
     & 4,i2  ,c2,c3)+cr5*ui(i1+5,i2  ,c2,c3)+cr6*ui(i1+6,i2  ,c2,c3)+
     & cr7*ui(i1+7,i2  ,c2,c3)+cr8*ui(i1+8,i2  ,c2,c3))+cs1*(cr0*ui(
     & i1  ,i2+1,c2,c3)+cr1*ui(i1+1,i2+1,c2,c3)+cr2*ui(i1+2,i2+1,c2,
     & c3)+cr3*ui(i1+3,i2+1,c2,c3)+cr4*ui(i1+4,i2+1,c2,c3)+cr5*ui(i1+
     & 5,i2+1,c2,c3)+cr6*ui(i1+6,i2+1,c2,c3)+cr7*ui(i1+7,i2+1,c2,c3)+
     & cr8*ui(i1+8,i2+1,c2,c3))+cs2*(cr0*ui(i1  ,i2+2,c2,c3)+cr1*ui(
     & i1+1,i2+2,c2,c3)+cr2*ui(i1+2,i2+2,c2,c3)+cr3*ui(i1+3,i2+2,c2,
     & c3)+cr4*ui(i1+4,i2+2,c2,c3)+cr5*ui(i1+5,i2+2,c2,c3)+cr6*ui(i1+
     & 6,i2+2,c2,c3)+cr7*ui(i1+7,i2+2,c2,c3)+cr8*ui(i1+8,i2+2,c2,c3))+
     & cs3*(cr0*ui(i1  ,i2+3,c2,c3)+cr1*ui(i1+1,i2+3,c2,c3)+cr2*ui(i1+
     & 2,i2+3,c2,c3)+cr3*ui(i1+3,i2+3,c2,c3)+cr4*ui(i1+4,i2+3,c2,c3)+
     & cr5*ui(i1+5,i2+3,c2,c3)+cr6*ui(i1+6,i2+3,c2,c3)+cr7*ui(i1+7,i2+
     & 3,c2,c3)+cr8*ui(i1+8,i2+3,c2,c3))+cs4*(cr0*ui(i1  ,i2+4,c2,c3)+
     & cr1*ui(i1+1,i2+4,c2,c3)+cr2*ui(i1+2,i2+4,c2,c3)+cr3*ui(i1+3,i2+
     & 4,c2,c3)+cr4*ui(i1+4,i2+4,c2,c3)+cr5*ui(i1+5,i2+4,c2,c3)+cr6*
     & ui(i1+6,i2+4,c2,c3)+cr7*ui(i1+7,i2+4,c2,c3)+cr8*ui(i1+8,i2+4,
     & c2,c3))+cs5*(cr0*ui(i1  ,i2+5,c2,c3)+cr1*ui(i1+1,i2+5,c2,c3)+
     & cr2*ui(i1+2,i2+5,c2,c3)+cr3*ui(i1+3,i2+5,c2,c3)+cr4*ui(i1+4,i2+
     & 5,c2,c3)+cr5*ui(i1+5,i2+5,c2,c3)+cr6*ui(i1+6,i2+5,c2,c3)+cr7*
     & ui(i1+7,i2+5,c2,c3)+cr8*ui(i1+8,i2+5,c2,c3))+cs6*(cr0*ui(i1  ,
     & i2+6,c2,c3)+cr1*ui(i1+1,i2+6,c2,c3)+cr2*ui(i1+2,i2+6,c2,c3)+
     & cr3*ui(i1+3,i2+6,c2,c3)+cr4*ui(i1+4,i2+6,c2,c3)+cr5*ui(i1+5,i2+
     & 6,c2,c3)+cr6*ui(i1+6,i2+6,c2,c3)+cr7*ui(i1+7,i2+6,c2,c3)+cr8*
     & ui(i1+8,i2+6,c2,c3))+cs7*(cr0*ui(i1  ,i2+7,c2,c3)+cr1*ui(i1+1,
     & i2+7,c2,c3)+cr2*ui(i1+2,i2+7,c2,c3)+cr3*ui(i1+3,i2+7,c2,c3)+
     & cr4*ui(i1+4,i2+7,c2,c3)+cr5*ui(i1+5,i2+7,c2,c3)+cr6*ui(i1+6,i2+
     & 7,c2,c3)+cr7*ui(i1+7,i2+7,c2,c3)+cr8*ui(i1+8,i2+7,c2,c3))+cs8*(
     & cr0*ui(i1  ,i2+8,c2,c3)+cr1*ui(i1+1,i2+8,c2,c3)+cr2*ui(i1+2,i2+
     & 8,c2,c3)+cr3*ui(i1+3,i2+8,c2,c3)+cr4*ui(i1+4,i2+8,c2,c3)+cr5*
     & ui(i1+5,i2+8,c2,c3)+cr6*ui(i1+6,i2+8,c2,c3)+cr7*ui(i1+7,i2+8,
     & c2,c3)+cr8*ui(i1+8,i2+8,c2,c3))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i)))
               ug(ip(i,1),ip(i,2),c2,c3)=r(i)
             end do
             end do
             end do
           end if
         else
           write(*,*) 'ERROR width=',width(1),width(2)
           stop 1
         end if
       else
         !     *** 3D ****
         if( useVariableWidthInterpolation.ne.0 )then
! beginLoops3d()
             do i=nia,nib
             do c3=c3a,c3b
             ! check for most common widths first
             if( varWidth(i).eq.3 )then
! interpSparseStorage333(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q30(c(i,0,0,0))
                 cs0 = q30(c(i,1,0,0))
                 ct0 = q30(c(i,2,0,0))
                 cr1 = q31(c(i,0,0,0))
                 cs1 = q31(c(i,1,0,0))
                 ct1 = q31(c(i,2,0,0))
                 cr2 = q32(c(i,0,0,0))
                 cs2 = q32(c(i,1,0,0))
                 ct2 = q32(c(i,2,0,0))
               r(i) = ct0*(cs0*(cr0*ui(i1,i2,i3+0,c3)+cr1*ui(i1+1,i2,
     & i3+0,c3)+cr2*ui(i1+2,i2,i3+0,c3))+cs1*(cr0*ui(i1,i2+1,i3+0,c3)+
     & cr1*ui(i1+1,i2+1,i3+0,c3)+cr2*ui(i1+2,i2+1,i3+0,c3))+cs2*(cr0*
     & ui(i1,i2+2,i3+0,c3)+cr1*ui(i1+1,i2+2,i3+0,c3)+cr2*ui(i1+2,i2+2,
     & i3+0,c3)))+ct1*(cs0*(cr0*ui(i1,i2,i3+1,c3)+cr1*ui(i1+1,i2,i3+1,
     & c3)+cr2*ui(i1+2,i2,i3+1,c3))+cs1*(cr0*ui(i1,i2+1,i3+1,c3)+cr1*
     & ui(i1+1,i2+1,i3+1,c3)+cr2*ui(i1+2,i2+1,i3+1,c3))+cs2*(cr0*ui(
     & i1,i2+2,i3+1,c3)+cr1*ui(i1+1,i2+2,i3+1,c3)+cr2*ui(i1+2,i2+2,i3+
     & 1,c3)))+ct2*(cs0*(cr0*ui(i1,i2,i3+2,c3)+cr1*ui(i1+1,i2,i3+2,c3)
     & +cr2*ui(i1+2,i2,i3+2,c3))+cs1*(cr0*ui(i1,i2+1,i3+2,c3)+cr1*ui(
     & i1+1,i2+1,i3+2,c3)+cr2*ui(i1+2,i2+1,i3+2,c3))+cs2*(cr0*ui(i1,
     & i2+2,i3+2,c3)+cr1*ui(i1+1,i2+2,i3+2,c3)+cr2*ui(i1+2,i2+2,i3+2,
     & c3)))
             else if( varWidth(i).eq.2 )then
! interpSparseStorage222(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q20(c(i,0,0,0))
                 cs0 = q20(c(i,1,0,0))
                 ct0 = q20(c(i,2,0,0))
                 cr1 = q21(c(i,0,0,0))
                 cs1 = q21(c(i,1,0,0))
                 ct1 = q21(c(i,2,0,0))
               r(i) = ct0*(cs0*(cr0*ui(i1,i2,i3+0,c3)+cr1*ui(i1+1,i2,
     & i3+0,c3))+cs1*(cr0*ui(i1,i2+1,i3+0,c3)+cr1*ui(i1+1,i2+1,i3+0,
     & c3)))+ct1*(cs0*(cr0*ui(i1,i2,i3+1,c3)+cr1*ui(i1+1,i2,i3+1,c3))+
     & cs1*(cr0*ui(i1,i2+1,i3+1,c3)+cr1*ui(i1+1,i2+1,i3+1,c3)))
             else if( varWidth(i).eq.1 )then
! interp111(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               r(i) = ui(i1,i2,i3,c3)
             else if( varWidth(i).eq.5 )then
! interpSparseStorage555(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q50(c(i,0,0,0))
                 cs0 = q50(c(i,1,0,0))
                 ct0 = q50(c(i,2,0,0))
                 cr1 = q51(c(i,0,0,0))
                 cs1 = q51(c(i,1,0,0))
                 ct1 = q51(c(i,2,0,0))
                 cr2 = q52(c(i,0,0,0))
                 cs2 = q52(c(i,1,0,0))
                 ct2 = q52(c(i,2,0,0))
                 cr3 = q53(c(i,0,0,0))
                 cs3 = q53(c(i,1,0,0))
                 ct3 = q53(c(i,2,0,0))
                 cr4 = q54(c(i,0,0,0))
                 cs4 = q54(c(i,1,0,0))
                 ct4 = q54(c(i,2,0,0))
               r(i) = ct0*(cs0*(cr0*ui(i1,i2,i3+0,c3)+cr1*ui(i1+1,i2,
     & i3+0,c3)+cr2*ui(i1+2,i2,i3+0,c3)+cr3*ui(i1+3,i2,i3+0,c3)+cr4*
     & ui(i1+4,i2,i3+0,c3))+cs1*(cr0*ui(i1,i2+1,i3+0,c3)+cr1*ui(i1+1,
     & i2+1,i3+0,c3)+cr2*ui(i1+2,i2+1,i3+0,c3)+cr3*ui(i1+3,i2+1,i3+0,
     & c3)+cr4*ui(i1+4,i2+1,i3+0,c3))+cs2*(cr0*ui(i1,i2+2,i3+0,c3)+
     & cr1*ui(i1+1,i2+2,i3+0,c3)+cr2*ui(i1+2,i2+2,i3+0,c3)+cr3*ui(i1+
     & 3,i2+2,i3+0,c3)+cr4*ui(i1+4,i2+2,i3+0,c3))+cs3*(cr0*ui(i1,i2+3,
     & i3+0,c3)+cr1*ui(i1+1,i2+3,i3+0,c3)+cr2*ui(i1+2,i2+3,i3+0,c3)+
     & cr3*ui(i1+3,i2+3,i3+0,c3)+cr4*ui(i1+4,i2+3,i3+0,c3))+cs4*(cr0*
     & ui(i1,i2+4,i3+0,c3)+cr1*ui(i1+1,i2+4,i3+0,c3)+cr2*ui(i1+2,i2+4,
     & i3+0,c3)+cr3*ui(i1+3,i2+4,i3+0,c3)+cr4*ui(i1+4,i2+4,i3+0,c3)))+
     & ct1*(cs0*(cr0*ui(i1,i2,i3+1,c3)+cr1*ui(i1+1,i2,i3+1,c3)+cr2*ui(
     & i1+2,i2,i3+1,c3)+cr3*ui(i1+3,i2,i3+1,c3)+cr4*ui(i1+4,i2,i3+1,
     & c3))+cs1*(cr0*ui(i1,i2+1,i3+1,c3)+cr1*ui(i1+1,i2+1,i3+1,c3)+
     & cr2*ui(i1+2,i2+1,i3+1,c3)+cr3*ui(i1+3,i2+1,i3+1,c3)+cr4*ui(i1+
     & 4,i2+1,i3+1,c3))+cs2*(cr0*ui(i1,i2+2,i3+1,c3)+cr1*ui(i1+1,i2+2,
     & i3+1,c3)+cr2*ui(i1+2,i2+2,i3+1,c3)+cr3*ui(i1+3,i2+2,i3+1,c3)+
     & cr4*ui(i1+4,i2+2,i3+1,c3))+cs3*(cr0*ui(i1,i2+3,i3+1,c3)+cr1*ui(
     & i1+1,i2+3,i3+1,c3)+cr2*ui(i1+2,i2+3,i3+1,c3)+cr3*ui(i1+3,i2+3,
     & i3+1,c3)+cr4*ui(i1+4,i2+3,i3+1,c3))+cs4*(cr0*ui(i1,i2+4,i3+1,
     & c3)+cr1*ui(i1+1,i2+4,i3+1,c3)+cr2*ui(i1+2,i2+4,i3+1,c3)+cr3*ui(
     & i1+3,i2+4,i3+1,c3)+cr4*ui(i1+4,i2+4,i3+1,c3)))+ct2*(cs0*(cr0*
     & ui(i1,i2,i3+2,c3)+cr1*ui(i1+1,i2,i3+2,c3)+cr2*ui(i1+2,i2,i3+2,
     & c3)+cr3*ui(i1+3,i2,i3+2,c3)+cr4*ui(i1+4,i2,i3+2,c3))+cs1*(cr0*
     & ui(i1,i2+1,i3+2,c3)+cr1*ui(i1+1,i2+1,i3+2,c3)+cr2*ui(i1+2,i2+1,
     & i3+2,c3)+cr3*ui(i1+3,i2+1,i3+2,c3)+cr4*ui(i1+4,i2+1,i3+2,c3))+
     & cs2*(cr0*ui(i1,i2+2,i3+2,c3)+cr1*ui(i1+1,i2+2,i3+2,c3)+cr2*ui(
     & i1+2,i2+2,i3+2,c3)+cr3*ui(i1+3,i2+2,i3+2,c3)+cr4*ui(i1+4,i2+2,
     & i3+2,c3))+cs3*(cr0*ui(i1,i2+3,i3+2,c3)+cr1*ui(i1+1,i2+3,i3+2,
     & c3)+cr2*ui(i1+2,i2+3,i3+2,c3)+cr3*ui(i1+3,i2+3,i3+2,c3)+cr4*ui(
     & i1+4,i2+3,i3+2,c3))+cs4*(cr0*ui(i1,i2+4,i3+2,c3)+cr1*ui(i1+1,
     & i2+4,i3+2,c3)+cr2*ui(i1+2,i2+4,i3+2,c3)+cr3*ui(i1+3,i2+4,i3+2,
     & c3)+cr4*ui(i1+4,i2+4,i3+2,c3)))+ct3*(cs0*(cr0*ui(i1,i2,i3+3,c3)
     & +cr1*ui(i1+1,i2,i3+3,c3)+cr2*ui(i1+2,i2,i3+3,c3)+cr3*ui(i1+3,
     & i2,i3+3,c3)+cr4*ui(i1+4,i2,i3+3,c3))+cs1*(cr0*ui(i1,i2+1,i3+3,
     & c3)+cr1*ui(i1+1,i2+1,i3+3,c3)+cr2*ui(i1+2,i2+1,i3+3,c3)+cr3*ui(
     & i1+3,i2+1,i3+3,c3)+cr4*ui(i1+4,i2+1,i3+3,c3))+cs2*(cr0*ui(i1,
     & i2+2,i3+3,c3)+cr1*ui(i1+1,i2+2,i3+3,c3)+cr2*ui(i1+2,i2+2,i3+3,
     & c3)+cr3*ui(i1+3,i2+2,i3+3,c3)+cr4*ui(i1+4,i2+2,i3+3,c3))+cs3*(
     & cr0*ui(i1,i2+3,i3+3,c3)+cr1*ui(i1+1,i2+3,i3+3,c3)+cr2*ui(i1+2,
     & i2+3,i3+3,c3)+cr3*ui(i1+3,i2+3,i3+3,c3)+cr4*ui(i1+4,i2+3,i3+3,
     & c3))+cs4*(cr0*ui(i1,i2+4,i3+3,c3)+cr1*ui(i1+1,i2+4,i3+3,c3)+
     & cr2*ui(i1+2,i2+4,i3+3,c3)+cr3*ui(i1+3,i2+4,i3+3,c3)+cr4*ui(i1+
     & 4,i2+4,i3+3,c3)))+ct4*(cs0*(cr0*ui(i1,i2,i3+4,c3)+cr1*ui(i1+1,
     & i2,i3+4,c3)+cr2*ui(i1+2,i2,i3+4,c3)+cr3*ui(i1+3,i2,i3+4,c3)+
     & cr4*ui(i1+4,i2,i3+4,c3))+cs1*(cr0*ui(i1,i2+1,i3+4,c3)+cr1*ui(
     & i1+1,i2+1,i3+4,c3)+cr2*ui(i1+2,i2+1,i3+4,c3)+cr3*ui(i1+3,i2+1,
     & i3+4,c3)+cr4*ui(i1+4,i2+1,i3+4,c3))+cs2*(cr0*ui(i1,i2+2,i3+4,
     & c3)+cr1*ui(i1+1,i2+2,i3+4,c3)+cr2*ui(i1+2,i2+2,i3+4,c3)+cr3*ui(
     & i1+3,i2+2,i3+4,c3)+cr4*ui(i1+4,i2+2,i3+4,c3))+cs3*(cr0*ui(i1,
     & i2+3,i3+4,c3)+cr1*ui(i1+1,i2+3,i3+4,c3)+cr2*ui(i1+2,i2+3,i3+4,
     & c3)+cr3*ui(i1+3,i2+3,i3+4,c3)+cr4*ui(i1+4,i2+3,i3+4,c3))+cs4*(
     & cr0*ui(i1,i2+4,i3+4,c3)+cr1*ui(i1+1,i2+4,i3+4,c3)+cr2*ui(i1+2,
     & i2+4,i3+4,c3)+cr3*ui(i1+3,i2+4,i3+4,c3)+cr4*ui(i1+4,i2+4,i3+4,
     & c3)))
             else if( varWidth(i).eq.4 )then
! interpSparseStorage444(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q40(c(i,0,0,0))
                 cs0 = q40(c(i,1,0,0))
                 ct0 = q40(c(i,2,0,0))
                 cr1 = q41(c(i,0,0,0))
                 cs1 = q41(c(i,1,0,0))
                 ct1 = q41(c(i,2,0,0))
                 cr2 = q42(c(i,0,0,0))
                 cs2 = q42(c(i,1,0,0))
                 ct2 = q42(c(i,2,0,0))
                 cr3 = q43(c(i,0,0,0))
                 cs3 = q43(c(i,1,0,0))
                 ct3 = q43(c(i,2,0,0))
               r(i) = ct0*(cs0*(cr0*ui(i1,i2,i3+0,c3)+cr1*ui(i1+1,i2,
     & i3+0,c3)+cr2*ui(i1+2,i2,i3+0,c3)+cr3*ui(i1+3,i2,i3+0,c3))+cs1*(
     & cr0*ui(i1,i2+1,i3+0,c3)+cr1*ui(i1+1,i2+1,i3+0,c3)+cr2*ui(i1+2,
     & i2+1,i3+0,c3)+cr3*ui(i1+3,i2+1,i3+0,c3))+cs2*(cr0*ui(i1,i2+2,
     & i3+0,c3)+cr1*ui(i1+1,i2+2,i3+0,c3)+cr2*ui(i1+2,i2+2,i3+0,c3)+
     & cr3*ui(i1+3,i2+2,i3+0,c3))+cs3*(cr0*ui(i1,i2+3,i3+0,c3)+cr1*ui(
     & i1+1,i2+3,i3+0,c3)+cr2*ui(i1+2,i2+3,i3+0,c3)+cr3*ui(i1+3,i2+3,
     & i3+0,c3)))+ct1*(cs0*(cr0*ui(i1,i2,i3+1,c3)+cr1*ui(i1+1,i2,i3+1,
     & c3)+cr2*ui(i1+2,i2,i3+1,c3)+cr3*ui(i1+3,i2,i3+1,c3))+cs1*(cr0*
     & ui(i1,i2+1,i3+1,c3)+cr1*ui(i1+1,i2+1,i3+1,c3)+cr2*ui(i1+2,i2+1,
     & i3+1,c3)+cr3*ui(i1+3,i2+1,i3+1,c3))+cs2*(cr0*ui(i1,i2+2,i3+1,
     & c3)+cr1*ui(i1+1,i2+2,i3+1,c3)+cr2*ui(i1+2,i2+2,i3+1,c3)+cr3*ui(
     & i1+3,i2+2,i3+1,c3))+cs3*(cr0*ui(i1,i2+3,i3+1,c3)+cr1*ui(i1+1,
     & i2+3,i3+1,c3)+cr2*ui(i1+2,i2+3,i3+1,c3)+cr3*ui(i1+3,i2+3,i3+1,
     & c3)))+ct2*(cs0*(cr0*ui(i1,i2,i3+2,c3)+cr1*ui(i1+1,i2,i3+2,c3)+
     & cr2*ui(i1+2,i2,i3+2,c3)+cr3*ui(i1+3,i2,i3+2,c3))+cs1*(cr0*ui(
     & i1,i2+1,i3+2,c3)+cr1*ui(i1+1,i2+1,i3+2,c3)+cr2*ui(i1+2,i2+1,i3+
     & 2,c3)+cr3*ui(i1+3,i2+1,i3+2,c3))+cs2*(cr0*ui(i1,i2+2,i3+2,c3)+
     & cr1*ui(i1+1,i2+2,i3+2,c3)+cr2*ui(i1+2,i2+2,i3+2,c3)+cr3*ui(i1+
     & 3,i2+2,i3+2,c3))+cs3*(cr0*ui(i1,i2+3,i3+2,c3)+cr1*ui(i1+1,i2+3,
     & i3+2,c3)+cr2*ui(i1+2,i2+3,i3+2,c3)+cr3*ui(i1+3,i2+3,i3+2,c3)))+
     & ct3*(cs0*(cr0*ui(i1,i2,i3+3,c3)+cr1*ui(i1+1,i2,i3+3,c3)+cr2*ui(
     & i1+2,i2,i3+3,c3)+cr3*ui(i1+3,i2,i3+3,c3))+cs1*(cr0*ui(i1,i2+1,
     & i3+3,c3)+cr1*ui(i1+1,i2+1,i3+3,c3)+cr2*ui(i1+2,i2+1,i3+3,c3)+
     & cr3*ui(i1+3,i2+1,i3+3,c3))+cs2*(cr0*ui(i1,i2+2,i3+3,c3)+cr1*ui(
     & i1+1,i2+2,i3+3,c3)+cr2*ui(i1+2,i2+2,i3+3,c3)+cr3*ui(i1+3,i2+2,
     & i3+3,c3))+cs3*(cr0*ui(i1,i2+3,i3+3,c3)+cr1*ui(i1+1,i2+3,i3+3,
     & c3)+cr2*ui(i1+2,i2+3,i3+3,c3)+cr3*ui(i1+3,i2+3,i3+3,c3)))
             else if( varWidth(i).eq.7 )then
! interpSparseStorage777(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q70(c(i,0,0,0))
                 cs0 = q70(c(i,1,0,0))
                 ct0 = q70(c(i,2,0,0))
                 cr1 = q71(c(i,0,0,0))
                 cs1 = q71(c(i,1,0,0))
                 ct1 = q71(c(i,2,0,0))
                 cr2 = q72(c(i,0,0,0))
                 cs2 = q72(c(i,1,0,0))
                 ct2 = q72(c(i,2,0,0))
                 cr3 = q73(c(i,0,0,0))
                 cs3 = q73(c(i,1,0,0))
                 ct3 = q73(c(i,2,0,0))
                 cr4 = q74(c(i,0,0,0))
                 cs4 = q74(c(i,1,0,0))
                 ct4 = q74(c(i,2,0,0))
                 cr5 = q75(c(i,0,0,0))
                 cs5 = q75(c(i,1,0,0))
                 ct5 = q75(c(i,2,0,0))
                 cr6 = q76(c(i,0,0,0))
                 cs6 = q76(c(i,1,0,0))
                 ct6 = q76(c(i,2,0,0))
               r(i) = ct0*(cs0*(cr0*ui(i1,i2,i3+0,c3)+cr1*ui(i1+1,i2,
     & i3+0,c3)+cr2*ui(i1+2,i2,i3+0,c3)+cr3*ui(i1+3,i2,i3+0,c3)+cr4*
     & ui(i1+4,i2,i3+0,c3)+cr5*ui(i1+5,i2,i3+0,c3)+cr6*ui(i1+6,i2,i3+
     & 0,c3))+cs1*(cr0*ui(i1,i2+1,i3+0,c3)+cr1*ui(i1+1,i2+1,i3+0,c3)+
     & cr2*ui(i1+2,i2+1,i3+0,c3)+cr3*ui(i1+3,i2+1,i3+0,c3)+cr4*ui(i1+
     & 4,i2+1,i3+0,c3)+cr5*ui(i1+5,i2+1,i3+0,c3)+cr6*ui(i1+6,i2+1,i3+
     & 0,c3))+cs2*(cr0*ui(i1,i2+2,i3+0,c3)+cr1*ui(i1+1,i2+2,i3+0,c3)+
     & cr2*ui(i1+2,i2+2,i3+0,c3)+cr3*ui(i1+3,i2+2,i3+0,c3)+cr4*ui(i1+
     & 4,i2+2,i3+0,c3)+cr5*ui(i1+5,i2+2,i3+0,c3)+cr6*ui(i1+6,i2+2,i3+
     & 0,c3))+cs3*(cr0*ui(i1,i2+3,i3+0,c3)+cr1*ui(i1+1,i2+3,i3+0,c3)+
     & cr2*ui(i1+2,i2+3,i3+0,c3)+cr3*ui(i1+3,i2+3,i3+0,c3)+cr4*ui(i1+
     & 4,i2+3,i3+0,c3)+cr5*ui(i1+5,i2+3,i3+0,c3)+cr6*ui(i1+6,i2+3,i3+
     & 0,c3))+cs4*(cr0*ui(i1,i2+4,i3+0,c3)+cr1*ui(i1+1,i2+4,i3+0,c3)+
     & cr2*ui(i1+2,i2+4,i3+0,c3)+cr3*ui(i1+3,i2+4,i3+0,c3)+cr4*ui(i1+
     & 4,i2+4,i3+0,c3)+cr5*ui(i1+5,i2+4,i3+0,c3)+cr6*ui(i1+6,i2+4,i3+
     & 0,c3))+cs5*(cr0*ui(i1,i2+5,i3+0,c3)+cr1*ui(i1+1,i2+5,i3+0,c3)+
     & cr2*ui(i1+2,i2+5,i3+0,c3)+cr3*ui(i1+3,i2+5,i3+0,c3)+cr4*ui(i1+
     & 4,i2+5,i3+0,c3)+cr5*ui(i1+5,i2+5,i3+0,c3)+cr6*ui(i1+6,i2+5,i3+
     & 0,c3))+cs6*(cr0*ui(i1,i2+6,i3+0,c3)+cr1*ui(i1+1,i2+6,i3+0,c3)+
     & cr2*ui(i1+2,i2+6,i3+0,c3)+cr3*ui(i1+3,i2+6,i3+0,c3)+cr4*ui(i1+
     & 4,i2+6,i3+0,c3)+cr5*ui(i1+5,i2+6,i3+0,c3)+cr6*ui(i1+6,i2+6,i3+
     & 0,c3)))+ct1*(cs0*(cr0*ui(i1,i2,i3+1,c3)+cr1*ui(i1+1,i2,i3+1,c3)
     & +cr2*ui(i1+2,i2,i3+1,c3)+cr3*ui(i1+3,i2,i3+1,c3)+cr4*ui(i1+4,
     & i2,i3+1,c3)+cr5*ui(i1+5,i2,i3+1,c3)+cr6*ui(i1+6,i2,i3+1,c3))+
     & cs1*(cr0*ui(i1,i2+1,i3+1,c3)+cr1*ui(i1+1,i2+1,i3+1,c3)+cr2*ui(
     & i1+2,i2+1,i3+1,c3)+cr3*ui(i1+3,i2+1,i3+1,c3)+cr4*ui(i1+4,i2+1,
     & i3+1,c3)+cr5*ui(i1+5,i2+1,i3+1,c3)+cr6*ui(i1+6,i2+1,i3+1,c3))+
     & cs2*(cr0*ui(i1,i2+2,i3+1,c3)+cr1*ui(i1+1,i2+2,i3+1,c3)+cr2*ui(
     & i1+2,i2+2,i3+1,c3)+cr3*ui(i1+3,i2+2,i3+1,c3)+cr4*ui(i1+4,i2+2,
     & i3+1,c3)+cr5*ui(i1+5,i2+2,i3+1,c3)+cr6*ui(i1+6,i2+2,i3+1,c3))+
     & cs3*(cr0*ui(i1,i2+3,i3+1,c3)+cr1*ui(i1+1,i2+3,i3+1,c3)+cr2*ui(
     & i1+2,i2+3,i3+1,c3)+cr3*ui(i1+3,i2+3,i3+1,c3)+cr4*ui(i1+4,i2+3,
     & i3+1,c3)+cr5*ui(i1+5,i2+3,i3+1,c3)+cr6*ui(i1+6,i2+3,i3+1,c3))+
     & cs4*(cr0*ui(i1,i2+4,i3+1,c3)+cr1*ui(i1+1,i2+4,i3+1,c3)+cr2*ui(
     & i1+2,i2+4,i3+1,c3)+cr3*ui(i1+3,i2+4,i3+1,c3)+cr4*ui(i1+4,i2+4,
     & i3+1,c3)+cr5*ui(i1+5,i2+4,i3+1,c3)+cr6*ui(i1+6,i2+4,i3+1,c3))+
     & cs5*(cr0*ui(i1,i2+5,i3+1,c3)+cr1*ui(i1+1,i2+5,i3+1,c3)+cr2*ui(
     & i1+2,i2+5,i3+1,c3)+cr3*ui(i1+3,i2+5,i3+1,c3)+cr4*ui(i1+4,i2+5,
     & i3+1,c3)+cr5*ui(i1+5,i2+5,i3+1,c3)+cr6*ui(i1+6,i2+5,i3+1,c3))+
     & cs6*(cr0*ui(i1,i2+6,i3+1,c3)+cr1*ui(i1+1,i2+6,i3+1,c3)+cr2*ui(
     & i1+2,i2+6,i3+1,c3)+cr3*ui(i1+3,i2+6,i3+1,c3)+cr4*ui(i1+4,i2+6,
     & i3+1,c3)+cr5*ui(i1+5,i2+6,i3+1,c3)+cr6*ui(i1+6,i2+6,i3+1,c3)))+
     & ct2*(cs0*(cr0*ui(i1,i2,i3+2,c3)+cr1*ui(i1+1,i2,i3+2,c3)+cr2*ui(
     & i1+2,i2,i3+2,c3)+cr3*ui(i1+3,i2,i3+2,c3)+cr4*ui(i1+4,i2,i3+2,
     & c3)+cr5*ui(i1+5,i2,i3+2,c3)+cr6*ui(i1+6,i2,i3+2,c3))+cs1*(cr0*
     & ui(i1,i2+1,i3+2,c3)+cr1*ui(i1+1,i2+1,i3+2,c3)+cr2*ui(i1+2,i2+1,
     & i3+2,c3)+cr3*ui(i1+3,i2+1,i3+2,c3)+cr4*ui(i1+4,i2+1,i3+2,c3)+
     & cr5*ui(i1+5,i2+1,i3+2,c3)+cr6*ui(i1+6,i2+1,i3+2,c3))+cs2*(cr0*
     & ui(i1,i2+2,i3+2,c3)+cr1*ui(i1+1,i2+2,i3+2,c3)+cr2*ui(i1+2,i2+2,
     & i3+2,c3)+cr3*ui(i1+3,i2+2,i3+2,c3)+cr4*ui(i1+4,i2+2,i3+2,c3)+
     & cr5*ui(i1+5,i2+2,i3+2,c3)+cr6*ui(i1+6,i2+2,i3+2,c3))+cs3*(cr0*
     & ui(i1,i2+3,i3+2,c3)+cr1*ui(i1+1,i2+3,i3+2,c3)+cr2*ui(i1+2,i2+3,
     & i3+2,c3)+cr3*ui(i1+3,i2+3,i3+2,c3)+cr4*ui(i1+4,i2+3,i3+2,c3)+
     & cr5*ui(i1+5,i2+3,i3+2,c3)+cr6*ui(i1+6,i2+3,i3+2,c3))+cs4*(cr0*
     & ui(i1,i2+4,i3+2,c3)+cr1*ui(i1+1,i2+4,i3+2,c3)+cr2*ui(i1+2,i2+4,
     & i3+2,c3)+cr3*ui(i1+3,i2+4,i3+2,c3)+cr4*ui(i1+4,i2+4,i3+2,c3)+
     & cr5*ui(i1+5,i2+4,i3+2,c3)+cr6*ui(i1+6,i2+4,i3+2,c3))+cs5*(cr0*
     & ui(i1,i2+5,i3+2,c3)+cr1*ui(i1+1,i2+5,i3+2,c3)+cr2*ui(i1+2,i2+5,
     & i3+2,c3)+cr3*ui(i1+3,i2+5,i3+2,c3)+cr4*ui(i1+4,i2+5,i3+2,c3)+
     & cr5*ui(i1+5,i2+5,i3+2,c3)+cr6*ui(i1+6,i2+5,i3+2,c3))+cs6*(cr0*
     & ui(i1,i2+6,i3+2,c3)+cr1*ui(i1+1,i2+6,i3+2,c3)+cr2*ui(i1+2,i2+6,
     & i3+2,c3)+cr3*ui(i1+3,i2+6,i3+2,c3)+cr4*ui(i1+4,i2+6,i3+2,c3)+
     & cr5*ui(i1+5,i2+6,i3+2,c3)+cr6*ui(i1+6,i2+6,i3+2,c3)))+ct3*(cs0*
     & (cr0*ui(i1,i2,i3+3,c3)+cr1*ui(i1+1,i2,i3+3,c3)+cr2*ui(i1+2,i2,
     & i3+3,c3)+cr3*ui(i1+3,i2,i3+3,c3)+cr4*ui(i1+4,i2,i3+3,c3)+cr5*
     & ui(i1+5,i2,i3+3,c3)+cr6*ui(i1+6,i2,i3+3,c3))+cs1*(cr0*ui(i1,i2+
     & 1,i3+3,c3)+cr1*ui(i1+1,i2+1,i3+3,c3)+cr2*ui(i1+2,i2+1,i3+3,c3)+
     & cr3*ui(i1+3,i2+1,i3+3,c3)+cr4*ui(i1+4,i2+1,i3+3,c3)+cr5*ui(i1+
     & 5,i2+1,i3+3,c3)+cr6*ui(i1+6,i2+1,i3+3,c3))+cs2*(cr0*ui(i1,i2+2,
     & i3+3,c3)+cr1*ui(i1+1,i2+2,i3+3,c3)+cr2*ui(i1+2,i2+2,i3+3,c3)+
     & cr3*ui(i1+3,i2+2,i3+3,c3)+cr4*ui(i1+4,i2+2,i3+3,c3)+cr5*ui(i1+
     & 5,i2+2,i3+3,c3)+cr6*ui(i1+6,i2+2,i3+3,c3))+cs3*(cr0*ui(i1,i2+3,
     & i3+3,c3)+cr1*ui(i1+1,i2+3,i3+3,c3)+cr2*ui(i1+2,i2+3,i3+3,c3)+
     & cr3*ui(i1+3,i2+3,i3+3,c3)+cr4*ui(i1+4,i2+3,i3+3,c3)+cr5*ui(i1+
     & 5,i2+3,i3+3,c3)+cr6*ui(i1+6,i2+3,i3+3,c3))+cs4*(cr0*ui(i1,i2+4,
     & i3+3,c3)+cr1*ui(i1+1,i2+4,i3+3,c3)+cr2*ui(i1+2,i2+4,i3+3,c3)+
     & cr3*ui(i1+3,i2+4,i3+3,c3)+cr4*ui(i1+4,i2+4,i3+3,c3)+cr5*ui(i1+
     & 5,i2+4,i3+3,c3)+cr6*ui(i1+6,i2+4,i3+3,c3))+cs5*(cr0*ui(i1,i2+5,
     & i3+3,c3)+cr1*ui(i1+1,i2+5,i3+3,c3)+cr2*ui(i1+2,i2+5,i3+3,c3)+
     & cr3*ui(i1+3,i2+5,i3+3,c3)+cr4*ui(i1+4,i2+5,i3+3,c3)+cr5*ui(i1+
     & 5,i2+5,i3+3,c3)+cr6*ui(i1+6,i2+5,i3+3,c3))+cs6*(cr0*ui(i1,i2+6,
     & i3+3,c3)+cr1*ui(i1+1,i2+6,i3+3,c3)+cr2*ui(i1+2,i2+6,i3+3,c3)+
     & cr3*ui(i1+3,i2+6,i3+3,c3)+cr4*ui(i1+4,i2+6,i3+3,c3)+cr5*ui(i1+
     & 5,i2+6,i3+3,c3)+cr6*ui(i1+6,i2+6,i3+3,c3)))
               r(i) = r(i)+ct4*(cs0*(cr0*ui(i1,i2,i3+4,c3)+cr1*ui(i1+1,
     & i2,i3+4,c3)+cr2*ui(i1+2,i2,i3+4,c3)+cr3*ui(i1+3,i2,i3+4,c3)+
     & cr4*ui(i1+4,i2,i3+4,c3)+cr5*ui(i1+5,i2,i3+4,c3)+cr6*ui(i1+6,i2,
     & i3+4,c3))+cs1*(cr0*ui(i1,i2+1,i3+4,c3)+cr1*ui(i1+1,i2+1,i3+4,
     & c3)+cr2*ui(i1+2,i2+1,i3+4,c3)+cr3*ui(i1+3,i2+1,i3+4,c3)+cr4*ui(
     & i1+4,i2+1,i3+4,c3)+cr5*ui(i1+5,i2+1,i3+4,c3)+cr6*ui(i1+6,i2+1,
     & i3+4,c3))+cs2*(cr0*ui(i1,i2+2,i3+4,c3)+cr1*ui(i1+1,i2+2,i3+4,
     & c3)+cr2*ui(i1+2,i2+2,i3+4,c3)+cr3*ui(i1+3,i2+2,i3+4,c3)+cr4*ui(
     & i1+4,i2+2,i3+4,c3)+cr5*ui(i1+5,i2+2,i3+4,c3)+cr6*ui(i1+6,i2+2,
     & i3+4,c3))+cs3*(cr0*ui(i1,i2+3,i3+4,c3)+cr1*ui(i1+1,i2+3,i3+4,
     & c3)+cr2*ui(i1+2,i2+3,i3+4,c3)+cr3*ui(i1+3,i2+3,i3+4,c3)+cr4*ui(
     & i1+4,i2+3,i3+4,c3)+cr5*ui(i1+5,i2+3,i3+4,c3)+cr6*ui(i1+6,i2+3,
     & i3+4,c3))+cs4*(cr0*ui(i1,i2+4,i3+4,c3)+cr1*ui(i1+1,i2+4,i3+4,
     & c3)+cr2*ui(i1+2,i2+4,i3+4,c3)+cr3*ui(i1+3,i2+4,i3+4,c3)+cr4*ui(
     & i1+4,i2+4,i3+4,c3)+cr5*ui(i1+5,i2+4,i3+4,c3)+cr6*ui(i1+6,i2+4,
     & i3+4,c3))+cs5*(cr0*ui(i1,i2+5,i3+4,c3)+cr1*ui(i1+1,i2+5,i3+4,
     & c3)+cr2*ui(i1+2,i2+5,i3+4,c3)+cr3*ui(i1+3,i2+5,i3+4,c3)+cr4*ui(
     & i1+4,i2+5,i3+4,c3)+cr5*ui(i1+5,i2+5,i3+4,c3)+cr6*ui(i1+6,i2+5,
     & i3+4,c3))+cs6*(cr0*ui(i1,i2+6,i3+4,c3)+cr1*ui(i1+1,i2+6,i3+4,
     & c3)+cr2*ui(i1+2,i2+6,i3+4,c3)+cr3*ui(i1+3,i2+6,i3+4,c3)+cr4*ui(
     & i1+4,i2+6,i3+4,c3)+cr5*ui(i1+5,i2+6,i3+4,c3)+cr6*ui(i1+6,i2+6,
     & i3+4,c3)))+ct5*(cs0*(cr0*ui(i1,i2,i3+5,c3)+cr1*ui(i1+1,i2,i3+5,
     & c3)+cr2*ui(i1+2,i2,i3+5,c3)+cr3*ui(i1+3,i2,i3+5,c3)+cr4*ui(i1+
     & 4,i2,i3+5,c3)+cr5*ui(i1+5,i2,i3+5,c3)+cr6*ui(i1+6,i2,i3+5,c3))+
     & cs1*(cr0*ui(i1,i2+1,i3+5,c3)+cr1*ui(i1+1,i2+1,i3+5,c3)+cr2*ui(
     & i1+2,i2+1,i3+5,c3)+cr3*ui(i1+3,i2+1,i3+5,c3)+cr4*ui(i1+4,i2+1,
     & i3+5,c3)+cr5*ui(i1+5,i2+1,i3+5,c3)+cr6*ui(i1+6,i2+1,i3+5,c3))+
     & cs2*(cr0*ui(i1,i2+2,i3+5,c3)+cr1*ui(i1+1,i2+2,i3+5,c3)+cr2*ui(
     & i1+2,i2+2,i3+5,c3)+cr3*ui(i1+3,i2+2,i3+5,c3)+cr4*ui(i1+4,i2+2,
     & i3+5,c3)+cr5*ui(i1+5,i2+2,i3+5,c3)+cr6*ui(i1+6,i2+2,i3+5,c3))+
     & cs3*(cr0*ui(i1,i2+3,i3+5,c3)+cr1*ui(i1+1,i2+3,i3+5,c3)+cr2*ui(
     & i1+2,i2+3,i3+5,c3)+cr3*ui(i1+3,i2+3,i3+5,c3)+cr4*ui(i1+4,i2+3,
     & i3+5,c3)+cr5*ui(i1+5,i2+3,i3+5,c3)+cr6*ui(i1+6,i2+3,i3+5,c3))+
     & cs4*(cr0*ui(i1,i2+4,i3+5,c3)+cr1*ui(i1+1,i2+4,i3+5,c3)+cr2*ui(
     & i1+2,i2+4,i3+5,c3)+cr3*ui(i1+3,i2+4,i3+5,c3)+cr4*ui(i1+4,i2+4,
     & i3+5,c3)+cr5*ui(i1+5,i2+4,i3+5,c3)+cr6*ui(i1+6,i2+4,i3+5,c3))+
     & cs5*(cr0*ui(i1,i2+5,i3+5,c3)+cr1*ui(i1+1,i2+5,i3+5,c3)+cr2*ui(
     & i1+2,i2+5,i3+5,c3)+cr3*ui(i1+3,i2+5,i3+5,c3)+cr4*ui(i1+4,i2+5,
     & i3+5,c3)+cr5*ui(i1+5,i2+5,i3+5,c3)+cr6*ui(i1+6,i2+5,i3+5,c3))+
     & cs6*(cr0*ui(i1,i2+6,i3+5,c3)+cr1*ui(i1+1,i2+6,i3+5,c3)+cr2*ui(
     & i1+2,i2+6,i3+5,c3)+cr3*ui(i1+3,i2+6,i3+5,c3)+cr4*ui(i1+4,i2+6,
     & i3+5,c3)+cr5*ui(i1+5,i2+6,i3+5,c3)+cr6*ui(i1+6,i2+6,i3+5,c3)))+
     & ct6*(cs0*(cr0*ui(i1,i2,i3+6,c3)+cr1*ui(i1+1,i2,i3+6,c3)+cr2*ui(
     & i1+2,i2,i3+6,c3)+cr3*ui(i1+3,i2,i3+6,c3)+cr4*ui(i1+4,i2,i3+6,
     & c3)+cr5*ui(i1+5,i2,i3+6,c3)+cr6*ui(i1+6,i2,i3+6,c3))+cs1*(cr0*
     & ui(i1,i2+1,i3+6,c3)+cr1*ui(i1+1,i2+1,i3+6,c3)+cr2*ui(i1+2,i2+1,
     & i3+6,c3)+cr3*ui(i1+3,i2+1,i3+6,c3)+cr4*ui(i1+4,i2+1,i3+6,c3)+
     & cr5*ui(i1+5,i2+1,i3+6,c3)+cr6*ui(i1+6,i2+1,i3+6,c3))+cs2*(cr0*
     & ui(i1,i2+2,i3+6,c3)+cr1*ui(i1+1,i2+2,i3+6,c3)+cr2*ui(i1+2,i2+2,
     & i3+6,c3)+cr3*ui(i1+3,i2+2,i3+6,c3)+cr4*ui(i1+4,i2+2,i3+6,c3)+
     & cr5*ui(i1+5,i2+2,i3+6,c3)+cr6*ui(i1+6,i2+2,i3+6,c3))+cs3*(cr0*
     & ui(i1,i2+3,i3+6,c3)+cr1*ui(i1+1,i2+3,i3+6,c3)+cr2*ui(i1+2,i2+3,
     & i3+6,c3)+cr3*ui(i1+3,i2+3,i3+6,c3)+cr4*ui(i1+4,i2+3,i3+6,c3)+
     & cr5*ui(i1+5,i2+3,i3+6,c3)+cr6*ui(i1+6,i2+3,i3+6,c3))+cs4*(cr0*
     & ui(i1,i2+4,i3+6,c3)+cr1*ui(i1+1,i2+4,i3+6,c3)+cr2*ui(i1+2,i2+4,
     & i3+6,c3)+cr3*ui(i1+3,i2+4,i3+6,c3)+cr4*ui(i1+4,i2+4,i3+6,c3)+
     & cr5*ui(i1+5,i2+4,i3+6,c3)+cr6*ui(i1+6,i2+4,i3+6,c3))+cs5*(cr0*
     & ui(i1,i2+5,i3+6,c3)+cr1*ui(i1+1,i2+5,i3+6,c3)+cr2*ui(i1+2,i2+5,
     & i3+6,c3)+cr3*ui(i1+3,i2+5,i3+6,c3)+cr4*ui(i1+4,i2+5,i3+6,c3)+
     & cr5*ui(i1+5,i2+5,i3+6,c3)+cr6*ui(i1+6,i2+5,i3+6,c3))+cs6*(cr0*
     & ui(i1,i2+6,i3+6,c3)+cr1*ui(i1+1,i2+6,i3+6,c3)+cr2*ui(i1+2,i2+6,
     & i3+6,c3)+cr3*ui(i1+3,i2+6,i3+6,c3)+cr4*ui(i1+4,i2+6,i3+6,c3)+
     & cr5*ui(i1+5,i2+6,i3+6,c3)+cr6*ui(i1+6,i2+6,i3+6,c3)))
             else if( varWidth(i).eq.6 )then
! interpSparseStorage666(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q60(c(i,0,0,0))
                 cs0 = q60(c(i,1,0,0))
                 ct0 = q60(c(i,2,0,0))
                 cr1 = q61(c(i,0,0,0))
                 cs1 = q61(c(i,1,0,0))
                 ct1 = q61(c(i,2,0,0))
                 cr2 = q62(c(i,0,0,0))
                 cs2 = q62(c(i,1,0,0))
                 ct2 = q62(c(i,2,0,0))
                 cr3 = q63(c(i,0,0,0))
                 cs3 = q63(c(i,1,0,0))
                 ct3 = q63(c(i,2,0,0))
                 cr4 = q64(c(i,0,0,0))
                 cs4 = q64(c(i,1,0,0))
                 ct4 = q64(c(i,2,0,0))
                 cr5 = q65(c(i,0,0,0))
                 cs5 = q65(c(i,1,0,0))
                 ct5 = q65(c(i,2,0,0))
               r(i) = ct0*(cs0*(cr0*ui(i1,i2,i3+0,c3)+cr1*ui(i1+1,i2,
     & i3+0,c3)+cr2*ui(i1+2,i2,i3+0,c3)+cr3*ui(i1+3,i2,i3+0,c3)+cr4*
     & ui(i1+4,i2,i3+0,c3)+cr5*ui(i1+5,i2,i3+0,c3))+cs1*(cr0*ui(i1,i2+
     & 1,i3+0,c3)+cr1*ui(i1+1,i2+1,i3+0,c3)+cr2*ui(i1+2,i2+1,i3+0,c3)+
     & cr3*ui(i1+3,i2+1,i3+0,c3)+cr4*ui(i1+4,i2+1,i3+0,c3)+cr5*ui(i1+
     & 5,i2+1,i3+0,c3))+cs2*(cr0*ui(i1,i2+2,i3+0,c3)+cr1*ui(i1+1,i2+2,
     & i3+0,c3)+cr2*ui(i1+2,i2+2,i3+0,c3)+cr3*ui(i1+3,i2+2,i3+0,c3)+
     & cr4*ui(i1+4,i2+2,i3+0,c3)+cr5*ui(i1+5,i2+2,i3+0,c3))+cs3*(cr0*
     & ui(i1,i2+3,i3+0,c3)+cr1*ui(i1+1,i2+3,i3+0,c3)+cr2*ui(i1+2,i2+3,
     & i3+0,c3)+cr3*ui(i1+3,i2+3,i3+0,c3)+cr4*ui(i1+4,i2+3,i3+0,c3)+
     & cr5*ui(i1+5,i2+3,i3+0,c3))+cs4*(cr0*ui(i1,i2+4,i3+0,c3)+cr1*ui(
     & i1+1,i2+4,i3+0,c3)+cr2*ui(i1+2,i2+4,i3+0,c3)+cr3*ui(i1+3,i2+4,
     & i3+0,c3)+cr4*ui(i1+4,i2+4,i3+0,c3)+cr5*ui(i1+5,i2+4,i3+0,c3))+
     & cs5*(cr0*ui(i1,i2+5,i3+0,c3)+cr1*ui(i1+1,i2+5,i3+0,c3)+cr2*ui(
     & i1+2,i2+5,i3+0,c3)+cr3*ui(i1+3,i2+5,i3+0,c3)+cr4*ui(i1+4,i2+5,
     & i3+0,c3)+cr5*ui(i1+5,i2+5,i3+0,c3)))+ct1*(cs0*(cr0*ui(i1,i2,i3+
     & 1,c3)+cr1*ui(i1+1,i2,i3+1,c3)+cr2*ui(i1+2,i2,i3+1,c3)+cr3*ui(
     & i1+3,i2,i3+1,c3)+cr4*ui(i1+4,i2,i3+1,c3)+cr5*ui(i1+5,i2,i3+1,
     & c3))+cs1*(cr0*ui(i1,i2+1,i3+1,c3)+cr1*ui(i1+1,i2+1,i3+1,c3)+
     & cr2*ui(i1+2,i2+1,i3+1,c3)+cr3*ui(i1+3,i2+1,i3+1,c3)+cr4*ui(i1+
     & 4,i2+1,i3+1,c3)+cr5*ui(i1+5,i2+1,i3+1,c3))+cs2*(cr0*ui(i1,i2+2,
     & i3+1,c3)+cr1*ui(i1+1,i2+2,i3+1,c3)+cr2*ui(i1+2,i2+2,i3+1,c3)+
     & cr3*ui(i1+3,i2+2,i3+1,c3)+cr4*ui(i1+4,i2+2,i3+1,c3)+cr5*ui(i1+
     & 5,i2+2,i3+1,c3))+cs3*(cr0*ui(i1,i2+3,i3+1,c3)+cr1*ui(i1+1,i2+3,
     & i3+1,c3)+cr2*ui(i1+2,i2+3,i3+1,c3)+cr3*ui(i1+3,i2+3,i3+1,c3)+
     & cr4*ui(i1+4,i2+3,i3+1,c3)+cr5*ui(i1+5,i2+3,i3+1,c3))+cs4*(cr0*
     & ui(i1,i2+4,i3+1,c3)+cr1*ui(i1+1,i2+4,i3+1,c3)+cr2*ui(i1+2,i2+4,
     & i3+1,c3)+cr3*ui(i1+3,i2+4,i3+1,c3)+cr4*ui(i1+4,i2+4,i3+1,c3)+
     & cr5*ui(i1+5,i2+4,i3+1,c3))+cs5*(cr0*ui(i1,i2+5,i3+1,c3)+cr1*ui(
     & i1+1,i2+5,i3+1,c3)+cr2*ui(i1+2,i2+5,i3+1,c3)+cr3*ui(i1+3,i2+5,
     & i3+1,c3)+cr4*ui(i1+4,i2+5,i3+1,c3)+cr5*ui(i1+5,i2+5,i3+1,c3)))+
     & ct2*(cs0*(cr0*ui(i1,i2,i3+2,c3)+cr1*ui(i1+1,i2,i3+2,c3)+cr2*ui(
     & i1+2,i2,i3+2,c3)+cr3*ui(i1+3,i2,i3+2,c3)+cr4*ui(i1+4,i2,i3+2,
     & c3)+cr5*ui(i1+5,i2,i3+2,c3))+cs1*(cr0*ui(i1,i2+1,i3+2,c3)+cr1*
     & ui(i1+1,i2+1,i3+2,c3)+cr2*ui(i1+2,i2+1,i3+2,c3)+cr3*ui(i1+3,i2+
     & 1,i3+2,c3)+cr4*ui(i1+4,i2+1,i3+2,c3)+cr5*ui(i1+5,i2+1,i3+2,c3))
     & +cs2*(cr0*ui(i1,i2+2,i3+2,c3)+cr1*ui(i1+1,i2+2,i3+2,c3)+cr2*ui(
     & i1+2,i2+2,i3+2,c3)+cr3*ui(i1+3,i2+2,i3+2,c3)+cr4*ui(i1+4,i2+2,
     & i3+2,c3)+cr5*ui(i1+5,i2+2,i3+2,c3))+cs3*(cr0*ui(i1,i2+3,i3+2,
     & c3)+cr1*ui(i1+1,i2+3,i3+2,c3)+cr2*ui(i1+2,i2+3,i3+2,c3)+cr3*ui(
     & i1+3,i2+3,i3+2,c3)+cr4*ui(i1+4,i2+3,i3+2,c3)+cr5*ui(i1+5,i2+3,
     & i3+2,c3))+cs4*(cr0*ui(i1,i2+4,i3+2,c3)+cr1*ui(i1+1,i2+4,i3+2,
     & c3)+cr2*ui(i1+2,i2+4,i3+2,c3)+cr3*ui(i1+3,i2+4,i3+2,c3)+cr4*ui(
     & i1+4,i2+4,i3+2,c3)+cr5*ui(i1+5,i2+4,i3+2,c3))+cs5*(cr0*ui(i1,
     & i2+5,i3+2,c3)+cr1*ui(i1+1,i2+5,i3+2,c3)+cr2*ui(i1+2,i2+5,i3+2,
     & c3)+cr3*ui(i1+3,i2+5,i3+2,c3)+cr4*ui(i1+4,i2+5,i3+2,c3)+cr5*ui(
     & i1+5,i2+5,i3+2,c3)))+ct3*(cs0*(cr0*ui(i1,i2,i3+3,c3)+cr1*ui(i1+
     & 1,i2,i3+3,c3)+cr2*ui(i1+2,i2,i3+3,c3)+cr3*ui(i1+3,i2,i3+3,c3)+
     & cr4*ui(i1+4,i2,i3+3,c3)+cr5*ui(i1+5,i2,i3+3,c3))+cs1*(cr0*ui(
     & i1,i2+1,i3+3,c3)+cr1*ui(i1+1,i2+1,i3+3,c3)+cr2*ui(i1+2,i2+1,i3+
     & 3,c3)+cr3*ui(i1+3,i2+1,i3+3,c3)+cr4*ui(i1+4,i2+1,i3+3,c3)+cr5*
     & ui(i1+5,i2+1,i3+3,c3))+cs2*(cr0*ui(i1,i2+2,i3+3,c3)+cr1*ui(i1+
     & 1,i2+2,i3+3,c3)+cr2*ui(i1+2,i2+2,i3+3,c3)+cr3*ui(i1+3,i2+2,i3+
     & 3,c3)+cr4*ui(i1+4,i2+2,i3+3,c3)+cr5*ui(i1+5,i2+2,i3+3,c3))+cs3*
     & (cr0*ui(i1,i2+3,i3+3,c3)+cr1*ui(i1+1,i2+3,i3+3,c3)+cr2*ui(i1+2,
     & i2+3,i3+3,c3)+cr3*ui(i1+3,i2+3,i3+3,c3)+cr4*ui(i1+4,i2+3,i3+3,
     & c3)+cr5*ui(i1+5,i2+3,i3+3,c3))+cs4*(cr0*ui(i1,i2+4,i3+3,c3)+
     & cr1*ui(i1+1,i2+4,i3+3,c3)+cr2*ui(i1+2,i2+4,i3+3,c3)+cr3*ui(i1+
     & 3,i2+4,i3+3,c3)+cr4*ui(i1+4,i2+4,i3+3,c3)+cr5*ui(i1+5,i2+4,i3+
     & 3,c3))+cs5*(cr0*ui(i1,i2+5,i3+3,c3)+cr1*ui(i1+1,i2+5,i3+3,c3)+
     & cr2*ui(i1+2,i2+5,i3+3,c3)+cr3*ui(i1+3,i2+5,i3+3,c3)+cr4*ui(i1+
     & 4,i2+5,i3+3,c3)+cr5*ui(i1+5,i2+5,i3+3,c3)))+ct4*(cs0*(cr0*ui(
     & i1,i2,i3+4,c3)+cr1*ui(i1+1,i2,i3+4,c3)+cr2*ui(i1+2,i2,i3+4,c3)+
     & cr3*ui(i1+3,i2,i3+4,c3)+cr4*ui(i1+4,i2,i3+4,c3)+cr5*ui(i1+5,i2,
     & i3+4,c3))+cs1*(cr0*ui(i1,i2+1,i3+4,c3)+cr1*ui(i1+1,i2+1,i3+4,
     & c3)+cr2*ui(i1+2,i2+1,i3+4,c3)+cr3*ui(i1+3,i2+1,i3+4,c3)+cr4*ui(
     & i1+4,i2+1,i3+4,c3)+cr5*ui(i1+5,i2+1,i3+4,c3))+cs2*(cr0*ui(i1,
     & i2+2,i3+4,c3)+cr1*ui(i1+1,i2+2,i3+4,c3)+cr2*ui(i1+2,i2+2,i3+4,
     & c3)+cr3*ui(i1+3,i2+2,i3+4,c3)+cr4*ui(i1+4,i2+2,i3+4,c3)+cr5*ui(
     & i1+5,i2+2,i3+4,c3))+cs3*(cr0*ui(i1,i2+3,i3+4,c3)+cr1*ui(i1+1,
     & i2+3,i3+4,c3)+cr2*ui(i1+2,i2+3,i3+4,c3)+cr3*ui(i1+3,i2+3,i3+4,
     & c3)+cr4*ui(i1+4,i2+3,i3+4,c3)+cr5*ui(i1+5,i2+3,i3+4,c3))+cs4*(
     & cr0*ui(i1,i2+4,i3+4,c3)+cr1*ui(i1+1,i2+4,i3+4,c3)+cr2*ui(i1+2,
     & i2+4,i3+4,c3)+cr3*ui(i1+3,i2+4,i3+4,c3)+cr4*ui(i1+4,i2+4,i3+4,
     & c3)+cr5*ui(i1+5,i2+4,i3+4,c3))+cs5*(cr0*ui(i1,i2+5,i3+4,c3)+
     & cr1*ui(i1+1,i2+5,i3+4,c3)+cr2*ui(i1+2,i2+5,i3+4,c3)+cr3*ui(i1+
     & 3,i2+5,i3+4,c3)+cr4*ui(i1+4,i2+5,i3+4,c3)+cr5*ui(i1+5,i2+5,i3+
     & 4,c3)))+ct5*(cs0*(cr0*ui(i1,i2,i3+5,c3)+cr1*ui(i1+1,i2,i3+5,c3)
     & +cr2*ui(i1+2,i2,i3+5,c3)+cr3*ui(i1+3,i2,i3+5,c3)+cr4*ui(i1+4,
     & i2,i3+5,c3)+cr5*ui(i1+5,i2,i3+5,c3))+cs1*(cr0*ui(i1,i2+1,i3+5,
     & c3)+cr1*ui(i1+1,i2+1,i3+5,c3)+cr2*ui(i1+2,i2+1,i3+5,c3)+cr3*ui(
     & i1+3,i2+1,i3+5,c3)+cr4*ui(i1+4,i2+1,i3+5,c3)+cr5*ui(i1+5,i2+1,
     & i3+5,c3))+cs2*(cr0*ui(i1,i2+2,i3+5,c3)+cr1*ui(i1+1,i2+2,i3+5,
     & c3)+cr2*ui(i1+2,i2+2,i3+5,c3)+cr3*ui(i1+3,i2+2,i3+5,c3)+cr4*ui(
     & i1+4,i2+2,i3+5,c3)+cr5*ui(i1+5,i2+2,i3+5,c3))+cs3*(cr0*ui(i1,
     & i2+3,i3+5,c3)+cr1*ui(i1+1,i2+3,i3+5,c3)+cr2*ui(i1+2,i2+3,i3+5,
     & c3)+cr3*ui(i1+3,i2+3,i3+5,c3)+cr4*ui(i1+4,i2+3,i3+5,c3)+cr5*ui(
     & i1+5,i2+3,i3+5,c3))+cs4*(cr0*ui(i1,i2+4,i3+5,c3)+cr1*ui(i1+1,
     & i2+4,i3+5,c3)+cr2*ui(i1+2,i2+4,i3+5,c3)+cr3*ui(i1+3,i2+4,i3+5,
     & c3)+cr4*ui(i1+4,i2+4,i3+5,c3)+cr5*ui(i1+5,i2+4,i3+5,c3))+cs5*(
     & cr0*ui(i1,i2+5,i3+5,c3)+cr1*ui(i1+1,i2+5,i3+5,c3)+cr2*ui(i1+2,
     & i2+5,i3+5,c3)+cr3*ui(i1+3,i2+5,i3+5,c3)+cr4*ui(i1+4,i2+5,i3+5,
     & c3)+cr5*ui(i1+5,i2+5,i3+5,c3)))
             else if( varWidth(i).eq.9 )then
! interpSparseStorage999(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q90(c(i,0,0,0))
                 cs0 = q90(c(i,1,0,0))
                 ct0 = q90(c(i,2,0,0))
                 cr1 = q91(c(i,0,0,0))
                 cs1 = q91(c(i,1,0,0))
                 ct1 = q91(c(i,2,0,0))
                 cr2 = q92(c(i,0,0,0))
                 cs2 = q92(c(i,1,0,0))
                 ct2 = q92(c(i,2,0,0))
                 cr3 = q93(c(i,0,0,0))
                 cs3 = q93(c(i,1,0,0))
                 ct3 = q93(c(i,2,0,0))
                 cr4 = q94(c(i,0,0,0))
                 cs4 = q94(c(i,1,0,0))
                 ct4 = q94(c(i,2,0,0))
                 cr5 = q95(c(i,0,0,0))
                 cs5 = q95(c(i,1,0,0))
                 ct5 = q95(c(i,2,0,0))
                 cr6 = q96(c(i,0,0,0))
                 cs6 = q96(c(i,1,0,0))
                 ct6 = q96(c(i,2,0,0))
                 cr7 = q97(c(i,0,0,0))
                 cs7 = q97(c(i,1,0,0))
                 ct7 = q97(c(i,2,0,0))
                 cr8 = q98(c(i,0,0,0))
                 cs8 = q98(c(i,1,0,0))
                 ct8 = q98(c(i,2,0,0))
               r(i) = ct0*(cs0*(cr0*ui(i1,i2,i3+0,c3)+cr1*ui(i1+1,i2,
     & i3+0,c3)+cr2*ui(i1+2,i2,i3+0,c3)+cr3*ui(i1+3,i2,i3+0,c3)+cr4*
     & ui(i1+4,i2,i3+0,c3)+cr5*ui(i1+5,i2,i3+0,c3)+cr6*ui(i1+6,i2,i3+
     & 0,c3)+cr7*ui(i1+7,i2,i3+0,c3)+cr8*ui(i1+8,i2,i3+0,c3))+cs1*(
     & cr0*ui(i1,i2+1,i3+0,c3)+cr1*ui(i1+1,i2+1,i3+0,c3)+cr2*ui(i1+2,
     & i2+1,i3+0,c3)+cr3*ui(i1+3,i2+1,i3+0,c3)+cr4*ui(i1+4,i2+1,i3+0,
     & c3)+cr5*ui(i1+5,i2+1,i3+0,c3)+cr6*ui(i1+6,i2+1,i3+0,c3)+cr7*ui(
     & i1+7,i2+1,i3+0,c3)+cr8*ui(i1+8,i2+1,i3+0,c3))+cs2*(cr0*ui(i1,
     & i2+2,i3+0,c3)+cr1*ui(i1+1,i2+2,i3+0,c3)+cr2*ui(i1+2,i2+2,i3+0,
     & c3)+cr3*ui(i1+3,i2+2,i3+0,c3)+cr4*ui(i1+4,i2+2,i3+0,c3)+cr5*ui(
     & i1+5,i2+2,i3+0,c3)+cr6*ui(i1+6,i2+2,i3+0,c3)+cr7*ui(i1+7,i2+2,
     & i3+0,c3)+cr8*ui(i1+8,i2+2,i3+0,c3))+cs3*(cr0*ui(i1,i2+3,i3+0,
     & c3)+cr1*ui(i1+1,i2+3,i3+0,c3)+cr2*ui(i1+2,i2+3,i3+0,c3)+cr3*ui(
     & i1+3,i2+3,i3+0,c3)+cr4*ui(i1+4,i2+3,i3+0,c3)+cr5*ui(i1+5,i2+3,
     & i3+0,c3)+cr6*ui(i1+6,i2+3,i3+0,c3)+cr7*ui(i1+7,i2+3,i3+0,c3)+
     & cr8*ui(i1+8,i2+3,i3+0,c3))+cs4*(cr0*ui(i1,i2+4,i3+0,c3)+cr1*ui(
     & i1+1,i2+4,i3+0,c3)+cr2*ui(i1+2,i2+4,i3+0,c3)+cr3*ui(i1+3,i2+4,
     & i3+0,c3)+cr4*ui(i1+4,i2+4,i3+0,c3)+cr5*ui(i1+5,i2+4,i3+0,c3)+
     & cr6*ui(i1+6,i2+4,i3+0,c3)+cr7*ui(i1+7,i2+4,i3+0,c3)+cr8*ui(i1+
     & 8,i2+4,i3+0,c3))+cs5*(cr0*ui(i1,i2+5,i3+0,c3)+cr1*ui(i1+1,i2+5,
     & i3+0,c3)+cr2*ui(i1+2,i2+5,i3+0,c3)+cr3*ui(i1+3,i2+5,i3+0,c3)+
     & cr4*ui(i1+4,i2+5,i3+0,c3)+cr5*ui(i1+5,i2+5,i3+0,c3)+cr6*ui(i1+
     & 6,i2+5,i3+0,c3)+cr7*ui(i1+7,i2+5,i3+0,c3)+cr8*ui(i1+8,i2+5,i3+
     & 0,c3))+cs6*(cr0*ui(i1,i2+6,i3+0,c3)+cr1*ui(i1+1,i2+6,i3+0,c3)+
     & cr2*ui(i1+2,i2+6,i3+0,c3)+cr3*ui(i1+3,i2+6,i3+0,c3)+cr4*ui(i1+
     & 4,i2+6,i3+0,c3)+cr5*ui(i1+5,i2+6,i3+0,c3)+cr6*ui(i1+6,i2+6,i3+
     & 0,c3)+cr7*ui(i1+7,i2+6,i3+0,c3)+cr8*ui(i1+8,i2+6,i3+0,c3))+cs7*
     & (cr0*ui(i1,i2+7,i3+0,c3)+cr1*ui(i1+1,i2+7,i3+0,c3)+cr2*ui(i1+2,
     & i2+7,i3+0,c3)+cr3*ui(i1+3,i2+7,i3+0,c3)+cr4*ui(i1+4,i2+7,i3+0,
     & c3)+cr5*ui(i1+5,i2+7,i3+0,c3)+cr6*ui(i1+6,i2+7,i3+0,c3)+cr7*ui(
     & i1+7,i2+7,i3+0,c3)+cr8*ui(i1+8,i2+7,i3+0,c3))+cs8*(cr0*ui(i1,
     & i2+8,i3+0,c3)+cr1*ui(i1+1,i2+8,i3+0,c3)+cr2*ui(i1+2,i2+8,i3+0,
     & c3)+cr3*ui(i1+3,i2+8,i3+0,c3)+cr4*ui(i1+4,i2+8,i3+0,c3)+cr5*ui(
     & i1+5,i2+8,i3+0,c3)+cr6*ui(i1+6,i2+8,i3+0,c3)+cr7*ui(i1+7,i2+8,
     & i3+0,c3)+cr8*ui(i1+8,i2+8,i3+0,c3)))+ct1*(cs0*(cr0*ui(i1,i2,i3+
     & 1,c3)+cr1*ui(i1+1,i2,i3+1,c3)+cr2*ui(i1+2,i2,i3+1,c3)+cr3*ui(
     & i1+3,i2,i3+1,c3)+cr4*ui(i1+4,i2,i3+1,c3)+cr5*ui(i1+5,i2,i3+1,
     & c3)+cr6*ui(i1+6,i2,i3+1,c3)+cr7*ui(i1+7,i2,i3+1,c3)+cr8*ui(i1+
     & 8,i2,i3+1,c3))+cs1*(cr0*ui(i1,i2+1,i3+1,c3)+cr1*ui(i1+1,i2+1,
     & i3+1,c3)+cr2*ui(i1+2,i2+1,i3+1,c3)+cr3*ui(i1+3,i2+1,i3+1,c3)+
     & cr4*ui(i1+4,i2+1,i3+1,c3)+cr5*ui(i1+5,i2+1,i3+1,c3)+cr6*ui(i1+
     & 6,i2+1,i3+1,c3)+cr7*ui(i1+7,i2+1,i3+1,c3)+cr8*ui(i1+8,i2+1,i3+
     & 1,c3))+cs2*(cr0*ui(i1,i2+2,i3+1,c3)+cr1*ui(i1+1,i2+2,i3+1,c3)+
     & cr2*ui(i1+2,i2+2,i3+1,c3)+cr3*ui(i1+3,i2+2,i3+1,c3)+cr4*ui(i1+
     & 4,i2+2,i3+1,c3)+cr5*ui(i1+5,i2+2,i3+1,c3)+cr6*ui(i1+6,i2+2,i3+
     & 1,c3)+cr7*ui(i1+7,i2+2,i3+1,c3)+cr8*ui(i1+8,i2+2,i3+1,c3))+cs3*
     & (cr0*ui(i1,i2+3,i3+1,c3)+cr1*ui(i1+1,i2+3,i3+1,c3)+cr2*ui(i1+2,
     & i2+3,i3+1,c3)+cr3*ui(i1+3,i2+3,i3+1,c3)+cr4*ui(i1+4,i2+3,i3+1,
     & c3)+cr5*ui(i1+5,i2+3,i3+1,c3)+cr6*ui(i1+6,i2+3,i3+1,c3)+cr7*ui(
     & i1+7,i2+3,i3+1,c3)+cr8*ui(i1+8,i2+3,i3+1,c3))+cs4*(cr0*ui(i1,
     & i2+4,i3+1,c3)+cr1*ui(i1+1,i2+4,i3+1,c3)+cr2*ui(i1+2,i2+4,i3+1,
     & c3)+cr3*ui(i1+3,i2+4,i3+1,c3)+cr4*ui(i1+4,i2+4,i3+1,c3)+cr5*ui(
     & i1+5,i2+4,i3+1,c3)+cr6*ui(i1+6,i2+4,i3+1,c3)+cr7*ui(i1+7,i2+4,
     & i3+1,c3)+cr8*ui(i1+8,i2+4,i3+1,c3))+cs5*(cr0*ui(i1,i2+5,i3+1,
     & c3)+cr1*ui(i1+1,i2+5,i3+1,c3)+cr2*ui(i1+2,i2+5,i3+1,c3)+cr3*ui(
     & i1+3,i2+5,i3+1,c3)+cr4*ui(i1+4,i2+5,i3+1,c3)+cr5*ui(i1+5,i2+5,
     & i3+1,c3)+cr6*ui(i1+6,i2+5,i3+1,c3)+cr7*ui(i1+7,i2+5,i3+1,c3)+
     & cr8*ui(i1+8,i2+5,i3+1,c3))+cs6*(cr0*ui(i1,i2+6,i3+1,c3)+cr1*ui(
     & i1+1,i2+6,i3+1,c3)+cr2*ui(i1+2,i2+6,i3+1,c3)+cr3*ui(i1+3,i2+6,
     & i3+1,c3)+cr4*ui(i1+4,i2+6,i3+1,c3)+cr5*ui(i1+5,i2+6,i3+1,c3)+
     & cr6*ui(i1+6,i2+6,i3+1,c3)+cr7*ui(i1+7,i2+6,i3+1,c3)+cr8*ui(i1+
     & 8,i2+6,i3+1,c3))+cs7*(cr0*ui(i1,i2+7,i3+1,c3)+cr1*ui(i1+1,i2+7,
     & i3+1,c3)+cr2*ui(i1+2,i2+7,i3+1,c3)+cr3*ui(i1+3,i2+7,i3+1,c3)+
     & cr4*ui(i1+4,i2+7,i3+1,c3)+cr5*ui(i1+5,i2+7,i3+1,c3)+cr6*ui(i1+
     & 6,i2+7,i3+1,c3)+cr7*ui(i1+7,i2+7,i3+1,c3)+cr8*ui(i1+8,i2+7,i3+
     & 1,c3))+cs8*(cr0*ui(i1,i2+8,i3+1,c3)+cr1*ui(i1+1,i2+8,i3+1,c3)+
     & cr2*ui(i1+2,i2+8,i3+1,c3)+cr3*ui(i1+3,i2+8,i3+1,c3)+cr4*ui(i1+
     & 4,i2+8,i3+1,c3)+cr5*ui(i1+5,i2+8,i3+1,c3)+cr6*ui(i1+6,i2+8,i3+
     & 1,c3)+cr7*ui(i1+7,i2+8,i3+1,c3)+cr8*ui(i1+8,i2+8,i3+1,c3)))
               r(i) = r(i)+ct2*(cs0*(cr0*ui(i1,i2,i3+2,c3)+cr1*ui(i1+1,
     & i2,i3+2,c3)+cr2*ui(i1+2,i2,i3+2,c3)+cr3*ui(i1+3,i2,i3+2,c3)+
     & cr4*ui(i1+4,i2,i3+2,c3)+cr5*ui(i1+5,i2,i3+2,c3)+cr6*ui(i1+6,i2,
     & i3+2,c3)+cr7*ui(i1+7,i2,i3+2,c3)+cr8*ui(i1+8,i2,i3+2,c3))+cs1*(
     & cr0*ui(i1,i2+1,i3+2,c3)+cr1*ui(i1+1,i2+1,i3+2,c3)+cr2*ui(i1+2,
     & i2+1,i3+2,c3)+cr3*ui(i1+3,i2+1,i3+2,c3)+cr4*ui(i1+4,i2+1,i3+2,
     & c3)+cr5*ui(i1+5,i2+1,i3+2,c3)+cr6*ui(i1+6,i2+1,i3+2,c3)+cr7*ui(
     & i1+7,i2+1,i3+2,c3)+cr8*ui(i1+8,i2+1,i3+2,c3))+cs2*(cr0*ui(i1,
     & i2+2,i3+2,c3)+cr1*ui(i1+1,i2+2,i3+2,c3)+cr2*ui(i1+2,i2+2,i3+2,
     & c3)+cr3*ui(i1+3,i2+2,i3+2,c3)+cr4*ui(i1+4,i2+2,i3+2,c3)+cr5*ui(
     & i1+5,i2+2,i3+2,c3)+cr6*ui(i1+6,i2+2,i3+2,c3)+cr7*ui(i1+7,i2+2,
     & i3+2,c3)+cr8*ui(i1+8,i2+2,i3+2,c3))+cs3*(cr0*ui(i1,i2+3,i3+2,
     & c3)+cr1*ui(i1+1,i2+3,i3+2,c3)+cr2*ui(i1+2,i2+3,i3+2,c3)+cr3*ui(
     & i1+3,i2+3,i3+2,c3)+cr4*ui(i1+4,i2+3,i3+2,c3)+cr5*ui(i1+5,i2+3,
     & i3+2,c3)+cr6*ui(i1+6,i2+3,i3+2,c3)+cr7*ui(i1+7,i2+3,i3+2,c3)+
     & cr8*ui(i1+8,i2+3,i3+2,c3))+cs4*(cr0*ui(i1,i2+4,i3+2,c3)+cr1*ui(
     & i1+1,i2+4,i3+2,c3)+cr2*ui(i1+2,i2+4,i3+2,c3)+cr3*ui(i1+3,i2+4,
     & i3+2,c3)+cr4*ui(i1+4,i2+4,i3+2,c3)+cr5*ui(i1+5,i2+4,i3+2,c3)+
     & cr6*ui(i1+6,i2+4,i3+2,c3)+cr7*ui(i1+7,i2+4,i3+2,c3)+cr8*ui(i1+
     & 8,i2+4,i3+2,c3))+cs5*(cr0*ui(i1,i2+5,i3+2,c3)+cr1*ui(i1+1,i2+5,
     & i3+2,c3)+cr2*ui(i1+2,i2+5,i3+2,c3)+cr3*ui(i1+3,i2+5,i3+2,c3)+
     & cr4*ui(i1+4,i2+5,i3+2,c3)+cr5*ui(i1+5,i2+5,i3+2,c3)+cr6*ui(i1+
     & 6,i2+5,i3+2,c3)+cr7*ui(i1+7,i2+5,i3+2,c3)+cr8*ui(i1+8,i2+5,i3+
     & 2,c3))+cs6*(cr0*ui(i1,i2+6,i3+2,c3)+cr1*ui(i1+1,i2+6,i3+2,c3)+
     & cr2*ui(i1+2,i2+6,i3+2,c3)+cr3*ui(i1+3,i2+6,i3+2,c3)+cr4*ui(i1+
     & 4,i2+6,i3+2,c3)+cr5*ui(i1+5,i2+6,i3+2,c3)+cr6*ui(i1+6,i2+6,i3+
     & 2,c3)+cr7*ui(i1+7,i2+6,i3+2,c3)+cr8*ui(i1+8,i2+6,i3+2,c3))+cs7*
     & (cr0*ui(i1,i2+7,i3+2,c3)+cr1*ui(i1+1,i2+7,i3+2,c3)+cr2*ui(i1+2,
     & i2+7,i3+2,c3)+cr3*ui(i1+3,i2+7,i3+2,c3)+cr4*ui(i1+4,i2+7,i3+2,
     & c3)+cr5*ui(i1+5,i2+7,i3+2,c3)+cr6*ui(i1+6,i2+7,i3+2,c3)+cr7*ui(
     & i1+7,i2+7,i3+2,c3)+cr8*ui(i1+8,i2+7,i3+2,c3))+cs8*(cr0*ui(i1,
     & i2+8,i3+2,c3)+cr1*ui(i1+1,i2+8,i3+2,c3)+cr2*ui(i1+2,i2+8,i3+2,
     & c3)+cr3*ui(i1+3,i2+8,i3+2,c3)+cr4*ui(i1+4,i2+8,i3+2,c3)+cr5*ui(
     & i1+5,i2+8,i3+2,c3)+cr6*ui(i1+6,i2+8,i3+2,c3)+cr7*ui(i1+7,i2+8,
     & i3+2,c3)+cr8*ui(i1+8,i2+8,i3+2,c3)))+ct3*(cs0*(cr0*ui(i1,i2,i3+
     & 3,c3)+cr1*ui(i1+1,i2,i3+3,c3)+cr2*ui(i1+2,i2,i3+3,c3)+cr3*ui(
     & i1+3,i2,i3+3,c3)+cr4*ui(i1+4,i2,i3+3,c3)+cr5*ui(i1+5,i2,i3+3,
     & c3)+cr6*ui(i1+6,i2,i3+3,c3)+cr7*ui(i1+7,i2,i3+3,c3)+cr8*ui(i1+
     & 8,i2,i3+3,c3))+cs1*(cr0*ui(i1,i2+1,i3+3,c3)+cr1*ui(i1+1,i2+1,
     & i3+3,c3)+cr2*ui(i1+2,i2+1,i3+3,c3)+cr3*ui(i1+3,i2+1,i3+3,c3)+
     & cr4*ui(i1+4,i2+1,i3+3,c3)+cr5*ui(i1+5,i2+1,i3+3,c3)+cr6*ui(i1+
     & 6,i2+1,i3+3,c3)+cr7*ui(i1+7,i2+1,i3+3,c3)+cr8*ui(i1+8,i2+1,i3+
     & 3,c3))+cs2*(cr0*ui(i1,i2+2,i3+3,c3)+cr1*ui(i1+1,i2+2,i3+3,c3)+
     & cr2*ui(i1+2,i2+2,i3+3,c3)+cr3*ui(i1+3,i2+2,i3+3,c3)+cr4*ui(i1+
     & 4,i2+2,i3+3,c3)+cr5*ui(i1+5,i2+2,i3+3,c3)+cr6*ui(i1+6,i2+2,i3+
     & 3,c3)+cr7*ui(i1+7,i2+2,i3+3,c3)+cr8*ui(i1+8,i2+2,i3+3,c3))+cs3*
     & (cr0*ui(i1,i2+3,i3+3,c3)+cr1*ui(i1+1,i2+3,i3+3,c3)+cr2*ui(i1+2,
     & i2+3,i3+3,c3)+cr3*ui(i1+3,i2+3,i3+3,c3)+cr4*ui(i1+4,i2+3,i3+3,
     & c3)+cr5*ui(i1+5,i2+3,i3+3,c3)+cr6*ui(i1+6,i2+3,i3+3,c3)+cr7*ui(
     & i1+7,i2+3,i3+3,c3)+cr8*ui(i1+8,i2+3,i3+3,c3))+cs4*(cr0*ui(i1,
     & i2+4,i3+3,c3)+cr1*ui(i1+1,i2+4,i3+3,c3)+cr2*ui(i1+2,i2+4,i3+3,
     & c3)+cr3*ui(i1+3,i2+4,i3+3,c3)+cr4*ui(i1+4,i2+4,i3+3,c3)+cr5*ui(
     & i1+5,i2+4,i3+3,c3)+cr6*ui(i1+6,i2+4,i3+3,c3)+cr7*ui(i1+7,i2+4,
     & i3+3,c3)+cr8*ui(i1+8,i2+4,i3+3,c3))+cs5*(cr0*ui(i1,i2+5,i3+3,
     & c3)+cr1*ui(i1+1,i2+5,i3+3,c3)+cr2*ui(i1+2,i2+5,i3+3,c3)+cr3*ui(
     & i1+3,i2+5,i3+3,c3)+cr4*ui(i1+4,i2+5,i3+3,c3)+cr5*ui(i1+5,i2+5,
     & i3+3,c3)+cr6*ui(i1+6,i2+5,i3+3,c3)+cr7*ui(i1+7,i2+5,i3+3,c3)+
     & cr8*ui(i1+8,i2+5,i3+3,c3))+cs6*(cr0*ui(i1,i2+6,i3+3,c3)+cr1*ui(
     & i1+1,i2+6,i3+3,c3)+cr2*ui(i1+2,i2+6,i3+3,c3)+cr3*ui(i1+3,i2+6,
     & i3+3,c3)+cr4*ui(i1+4,i2+6,i3+3,c3)+cr5*ui(i1+5,i2+6,i3+3,c3)+
     & cr6*ui(i1+6,i2+6,i3+3,c3)+cr7*ui(i1+7,i2+6,i3+3,c3)+cr8*ui(i1+
     & 8,i2+6,i3+3,c3))+cs7*(cr0*ui(i1,i2+7,i3+3,c3)+cr1*ui(i1+1,i2+7,
     & i3+3,c3)+cr2*ui(i1+2,i2+7,i3+3,c3)+cr3*ui(i1+3,i2+7,i3+3,c3)+
     & cr4*ui(i1+4,i2+7,i3+3,c3)+cr5*ui(i1+5,i2+7,i3+3,c3)+cr6*ui(i1+
     & 6,i2+7,i3+3,c3)+cr7*ui(i1+7,i2+7,i3+3,c3)+cr8*ui(i1+8,i2+7,i3+
     & 3,c3))+cs8*(cr0*ui(i1,i2+8,i3+3,c3)+cr1*ui(i1+1,i2+8,i3+3,c3)+
     & cr2*ui(i1+2,i2+8,i3+3,c3)+cr3*ui(i1+3,i2+8,i3+3,c3)+cr4*ui(i1+
     & 4,i2+8,i3+3,c3)+cr5*ui(i1+5,i2+8,i3+3,c3)+cr6*ui(i1+6,i2+8,i3+
     & 3,c3)+cr7*ui(i1+7,i2+8,i3+3,c3)+cr8*ui(i1+8,i2+8,i3+3,c3)))
               r(i) = r(i)+ct4*(cs0*(cr0*ui(i1,i2,i3+4,c3)+cr1*ui(i1+1,
     & i2,i3+4,c3)+cr2*ui(i1+2,i2,i3+4,c3)+cr3*ui(i1+3,i2,i3+4,c3)+
     & cr4*ui(i1+4,i2,i3+4,c3)+cr5*ui(i1+5,i2,i3+4,c3)+cr6*ui(i1+6,i2,
     & i3+4,c3)+cr7*ui(i1+7,i2,i3+4,c3)+cr8*ui(i1+8,i2,i3+4,c3))+cs1*(
     & cr0*ui(i1,i2+1,i3+4,c3)+cr1*ui(i1+1,i2+1,i3+4,c3)+cr2*ui(i1+2,
     & i2+1,i3+4,c3)+cr3*ui(i1+3,i2+1,i3+4,c3)+cr4*ui(i1+4,i2+1,i3+4,
     & c3)+cr5*ui(i1+5,i2+1,i3+4,c3)+cr6*ui(i1+6,i2+1,i3+4,c3)+cr7*ui(
     & i1+7,i2+1,i3+4,c3)+cr8*ui(i1+8,i2+1,i3+4,c3))+cs2*(cr0*ui(i1,
     & i2+2,i3+4,c3)+cr1*ui(i1+1,i2+2,i3+4,c3)+cr2*ui(i1+2,i2+2,i3+4,
     & c3)+cr3*ui(i1+3,i2+2,i3+4,c3)+cr4*ui(i1+4,i2+2,i3+4,c3)+cr5*ui(
     & i1+5,i2+2,i3+4,c3)+cr6*ui(i1+6,i2+2,i3+4,c3)+cr7*ui(i1+7,i2+2,
     & i3+4,c3)+cr8*ui(i1+8,i2+2,i3+4,c3))+cs3*(cr0*ui(i1,i2+3,i3+4,
     & c3)+cr1*ui(i1+1,i2+3,i3+4,c3)+cr2*ui(i1+2,i2+3,i3+4,c3)+cr3*ui(
     & i1+3,i2+3,i3+4,c3)+cr4*ui(i1+4,i2+3,i3+4,c3)+cr5*ui(i1+5,i2+3,
     & i3+4,c3)+cr6*ui(i1+6,i2+3,i3+4,c3)+cr7*ui(i1+7,i2+3,i3+4,c3)+
     & cr8*ui(i1+8,i2+3,i3+4,c3))+cs4*(cr0*ui(i1,i2+4,i3+4,c3)+cr1*ui(
     & i1+1,i2+4,i3+4,c3)+cr2*ui(i1+2,i2+4,i3+4,c3)+cr3*ui(i1+3,i2+4,
     & i3+4,c3)+cr4*ui(i1+4,i2+4,i3+4,c3)+cr5*ui(i1+5,i2+4,i3+4,c3)+
     & cr6*ui(i1+6,i2+4,i3+4,c3)+cr7*ui(i1+7,i2+4,i3+4,c3)+cr8*ui(i1+
     & 8,i2+4,i3+4,c3))+cs5*(cr0*ui(i1,i2+5,i3+4,c3)+cr1*ui(i1+1,i2+5,
     & i3+4,c3)+cr2*ui(i1+2,i2+5,i3+4,c3)+cr3*ui(i1+3,i2+5,i3+4,c3)+
     & cr4*ui(i1+4,i2+5,i3+4,c3)+cr5*ui(i1+5,i2+5,i3+4,c3)+cr6*ui(i1+
     & 6,i2+5,i3+4,c3)+cr7*ui(i1+7,i2+5,i3+4,c3)+cr8*ui(i1+8,i2+5,i3+
     & 4,c3))+cs6*(cr0*ui(i1,i2+6,i3+4,c3)+cr1*ui(i1+1,i2+6,i3+4,c3)+
     & cr2*ui(i1+2,i2+6,i3+4,c3)+cr3*ui(i1+3,i2+6,i3+4,c3)+cr4*ui(i1+
     & 4,i2+6,i3+4,c3)+cr5*ui(i1+5,i2+6,i3+4,c3)+cr6*ui(i1+6,i2+6,i3+
     & 4,c3)+cr7*ui(i1+7,i2+6,i3+4,c3)+cr8*ui(i1+8,i2+6,i3+4,c3))+cs7*
     & (cr0*ui(i1,i2+7,i3+4,c3)+cr1*ui(i1+1,i2+7,i3+4,c3)+cr2*ui(i1+2,
     & i2+7,i3+4,c3)+cr3*ui(i1+3,i2+7,i3+4,c3)+cr4*ui(i1+4,i2+7,i3+4,
     & c3)+cr5*ui(i1+5,i2+7,i3+4,c3)+cr6*ui(i1+6,i2+7,i3+4,c3)+cr7*ui(
     & i1+7,i2+7,i3+4,c3)+cr8*ui(i1+8,i2+7,i3+4,c3))+cs8*(cr0*ui(i1,
     & i2+8,i3+4,c3)+cr1*ui(i1+1,i2+8,i3+4,c3)+cr2*ui(i1+2,i2+8,i3+4,
     & c3)+cr3*ui(i1+3,i2+8,i3+4,c3)+cr4*ui(i1+4,i2+8,i3+4,c3)+cr5*ui(
     & i1+5,i2+8,i3+4,c3)+cr6*ui(i1+6,i2+8,i3+4,c3)+cr7*ui(i1+7,i2+8,
     & i3+4,c3)+cr8*ui(i1+8,i2+8,i3+4,c3)))+ct5*(cs0*(cr0*ui(i1,i2,i3+
     & 5,c3)+cr1*ui(i1+1,i2,i3+5,c3)+cr2*ui(i1+2,i2,i3+5,c3)+cr3*ui(
     & i1+3,i2,i3+5,c3)+cr4*ui(i1+4,i2,i3+5,c3)+cr5*ui(i1+5,i2,i3+5,
     & c3)+cr6*ui(i1+6,i2,i3+5,c3)+cr7*ui(i1+7,i2,i3+5,c3)+cr8*ui(i1+
     & 8,i2,i3+5,c3))+cs1*(cr0*ui(i1,i2+1,i3+5,c3)+cr1*ui(i1+1,i2+1,
     & i3+5,c3)+cr2*ui(i1+2,i2+1,i3+5,c3)+cr3*ui(i1+3,i2+1,i3+5,c3)+
     & cr4*ui(i1+4,i2+1,i3+5,c3)+cr5*ui(i1+5,i2+1,i3+5,c3)+cr6*ui(i1+
     & 6,i2+1,i3+5,c3)+cr7*ui(i1+7,i2+1,i3+5,c3)+cr8*ui(i1+8,i2+1,i3+
     & 5,c3))+cs2*(cr0*ui(i1,i2+2,i3+5,c3)+cr1*ui(i1+1,i2+2,i3+5,c3)+
     & cr2*ui(i1+2,i2+2,i3+5,c3)+cr3*ui(i1+3,i2+2,i3+5,c3)+cr4*ui(i1+
     & 4,i2+2,i3+5,c3)+cr5*ui(i1+5,i2+2,i3+5,c3)+cr6*ui(i1+6,i2+2,i3+
     & 5,c3)+cr7*ui(i1+7,i2+2,i3+5,c3)+cr8*ui(i1+8,i2+2,i3+5,c3))+cs3*
     & (cr0*ui(i1,i2+3,i3+5,c3)+cr1*ui(i1+1,i2+3,i3+5,c3)+cr2*ui(i1+2,
     & i2+3,i3+5,c3)+cr3*ui(i1+3,i2+3,i3+5,c3)+cr4*ui(i1+4,i2+3,i3+5,
     & c3)+cr5*ui(i1+5,i2+3,i3+5,c3)+cr6*ui(i1+6,i2+3,i3+5,c3)+cr7*ui(
     & i1+7,i2+3,i3+5,c3)+cr8*ui(i1+8,i2+3,i3+5,c3))+cs4*(cr0*ui(i1,
     & i2+4,i3+5,c3)+cr1*ui(i1+1,i2+4,i3+5,c3)+cr2*ui(i1+2,i2+4,i3+5,
     & c3)+cr3*ui(i1+3,i2+4,i3+5,c3)+cr4*ui(i1+4,i2+4,i3+5,c3)+cr5*ui(
     & i1+5,i2+4,i3+5,c3)+cr6*ui(i1+6,i2+4,i3+5,c3)+cr7*ui(i1+7,i2+4,
     & i3+5,c3)+cr8*ui(i1+8,i2+4,i3+5,c3))+cs5*(cr0*ui(i1,i2+5,i3+5,
     & c3)+cr1*ui(i1+1,i2+5,i3+5,c3)+cr2*ui(i1+2,i2+5,i3+5,c3)+cr3*ui(
     & i1+3,i2+5,i3+5,c3)+cr4*ui(i1+4,i2+5,i3+5,c3)+cr5*ui(i1+5,i2+5,
     & i3+5,c3)+cr6*ui(i1+6,i2+5,i3+5,c3)+cr7*ui(i1+7,i2+5,i3+5,c3)+
     & cr8*ui(i1+8,i2+5,i3+5,c3))+cs6*(cr0*ui(i1,i2+6,i3+5,c3)+cr1*ui(
     & i1+1,i2+6,i3+5,c3)+cr2*ui(i1+2,i2+6,i3+5,c3)+cr3*ui(i1+3,i2+6,
     & i3+5,c3)+cr4*ui(i1+4,i2+6,i3+5,c3)+cr5*ui(i1+5,i2+6,i3+5,c3)+
     & cr6*ui(i1+6,i2+6,i3+5,c3)+cr7*ui(i1+7,i2+6,i3+5,c3)+cr8*ui(i1+
     & 8,i2+6,i3+5,c3))+cs7*(cr0*ui(i1,i2+7,i3+5,c3)+cr1*ui(i1+1,i2+7,
     & i3+5,c3)+cr2*ui(i1+2,i2+7,i3+5,c3)+cr3*ui(i1+3,i2+7,i3+5,c3)+
     & cr4*ui(i1+4,i2+7,i3+5,c3)+cr5*ui(i1+5,i2+7,i3+5,c3)+cr6*ui(i1+
     & 6,i2+7,i3+5,c3)+cr7*ui(i1+7,i2+7,i3+5,c3)+cr8*ui(i1+8,i2+7,i3+
     & 5,c3))+cs8*(cr0*ui(i1,i2+8,i3+5,c3)+cr1*ui(i1+1,i2+8,i3+5,c3)+
     & cr2*ui(i1+2,i2+8,i3+5,c3)+cr3*ui(i1+3,i2+8,i3+5,c3)+cr4*ui(i1+
     & 4,i2+8,i3+5,c3)+cr5*ui(i1+5,i2+8,i3+5,c3)+cr6*ui(i1+6,i2+8,i3+
     & 5,c3)+cr7*ui(i1+7,i2+8,i3+5,c3)+cr8*ui(i1+8,i2+8,i3+5,c3)))
               r(i) = r(i)+ct6*(cs0*(cr0*ui(i1,i2,i3+6,c3)+cr1*ui(i1+1,
     & i2,i3+6,c3)+cr2*ui(i1+2,i2,i3+6,c3)+cr3*ui(i1+3,i2,i3+6,c3)+
     & cr4*ui(i1+4,i2,i3+6,c3)+cr5*ui(i1+5,i2,i3+6,c3)+cr6*ui(i1+6,i2,
     & i3+6,c3)+cr7*ui(i1+7,i2,i3+6,c3)+cr8*ui(i1+8,i2,i3+6,c3))+cs1*(
     & cr0*ui(i1,i2+1,i3+6,c3)+cr1*ui(i1+1,i2+1,i3+6,c3)+cr2*ui(i1+2,
     & i2+1,i3+6,c3)+cr3*ui(i1+3,i2+1,i3+6,c3)+cr4*ui(i1+4,i2+1,i3+6,
     & c3)+cr5*ui(i1+5,i2+1,i3+6,c3)+cr6*ui(i1+6,i2+1,i3+6,c3)+cr7*ui(
     & i1+7,i2+1,i3+6,c3)+cr8*ui(i1+8,i2+1,i3+6,c3))+cs2*(cr0*ui(i1,
     & i2+2,i3+6,c3)+cr1*ui(i1+1,i2+2,i3+6,c3)+cr2*ui(i1+2,i2+2,i3+6,
     & c3)+cr3*ui(i1+3,i2+2,i3+6,c3)+cr4*ui(i1+4,i2+2,i3+6,c3)+cr5*ui(
     & i1+5,i2+2,i3+6,c3)+cr6*ui(i1+6,i2+2,i3+6,c3)+cr7*ui(i1+7,i2+2,
     & i3+6,c3)+cr8*ui(i1+8,i2+2,i3+6,c3))+cs3*(cr0*ui(i1,i2+3,i3+6,
     & c3)+cr1*ui(i1+1,i2+3,i3+6,c3)+cr2*ui(i1+2,i2+3,i3+6,c3)+cr3*ui(
     & i1+3,i2+3,i3+6,c3)+cr4*ui(i1+4,i2+3,i3+6,c3)+cr5*ui(i1+5,i2+3,
     & i3+6,c3)+cr6*ui(i1+6,i2+3,i3+6,c3)+cr7*ui(i1+7,i2+3,i3+6,c3)+
     & cr8*ui(i1+8,i2+3,i3+6,c3))+cs4*(cr0*ui(i1,i2+4,i3+6,c3)+cr1*ui(
     & i1+1,i2+4,i3+6,c3)+cr2*ui(i1+2,i2+4,i3+6,c3)+cr3*ui(i1+3,i2+4,
     & i3+6,c3)+cr4*ui(i1+4,i2+4,i3+6,c3)+cr5*ui(i1+5,i2+4,i3+6,c3)+
     & cr6*ui(i1+6,i2+4,i3+6,c3)+cr7*ui(i1+7,i2+4,i3+6,c3)+cr8*ui(i1+
     & 8,i2+4,i3+6,c3))+cs5*(cr0*ui(i1,i2+5,i3+6,c3)+cr1*ui(i1+1,i2+5,
     & i3+6,c3)+cr2*ui(i1+2,i2+5,i3+6,c3)+cr3*ui(i1+3,i2+5,i3+6,c3)+
     & cr4*ui(i1+4,i2+5,i3+6,c3)+cr5*ui(i1+5,i2+5,i3+6,c3)+cr6*ui(i1+
     & 6,i2+5,i3+6,c3)+cr7*ui(i1+7,i2+5,i3+6,c3)+cr8*ui(i1+8,i2+5,i3+
     & 6,c3))+cs6*(cr0*ui(i1,i2+6,i3+6,c3)+cr1*ui(i1+1,i2+6,i3+6,c3)+
     & cr2*ui(i1+2,i2+6,i3+6,c3)+cr3*ui(i1+3,i2+6,i3+6,c3)+cr4*ui(i1+
     & 4,i2+6,i3+6,c3)+cr5*ui(i1+5,i2+6,i3+6,c3)+cr6*ui(i1+6,i2+6,i3+
     & 6,c3)+cr7*ui(i1+7,i2+6,i3+6,c3)+cr8*ui(i1+8,i2+6,i3+6,c3))+cs7*
     & (cr0*ui(i1,i2+7,i3+6,c3)+cr1*ui(i1+1,i2+7,i3+6,c3)+cr2*ui(i1+2,
     & i2+7,i3+6,c3)+cr3*ui(i1+3,i2+7,i3+6,c3)+cr4*ui(i1+4,i2+7,i3+6,
     & c3)+cr5*ui(i1+5,i2+7,i3+6,c3)+cr6*ui(i1+6,i2+7,i3+6,c3)+cr7*ui(
     & i1+7,i2+7,i3+6,c3)+cr8*ui(i1+8,i2+7,i3+6,c3))+cs8*(cr0*ui(i1,
     & i2+8,i3+6,c3)+cr1*ui(i1+1,i2+8,i3+6,c3)+cr2*ui(i1+2,i2+8,i3+6,
     & c3)+cr3*ui(i1+3,i2+8,i3+6,c3)+cr4*ui(i1+4,i2+8,i3+6,c3)+cr5*ui(
     & i1+5,i2+8,i3+6,c3)+cr6*ui(i1+6,i2+8,i3+6,c3)+cr7*ui(i1+7,i2+8,
     & i3+6,c3)+cr8*ui(i1+8,i2+8,i3+6,c3)))+ct7*(cs0*(cr0*ui(i1,i2,i3+
     & 7,c3)+cr1*ui(i1+1,i2,i3+7,c3)+cr2*ui(i1+2,i2,i3+7,c3)+cr3*ui(
     & i1+3,i2,i3+7,c3)+cr4*ui(i1+4,i2,i3+7,c3)+cr5*ui(i1+5,i2,i3+7,
     & c3)+cr6*ui(i1+6,i2,i3+7,c3)+cr7*ui(i1+7,i2,i3+7,c3)+cr8*ui(i1+
     & 8,i2,i3+7,c3))+cs1*(cr0*ui(i1,i2+1,i3+7,c3)+cr1*ui(i1+1,i2+1,
     & i3+7,c3)+cr2*ui(i1+2,i2+1,i3+7,c3)+cr3*ui(i1+3,i2+1,i3+7,c3)+
     & cr4*ui(i1+4,i2+1,i3+7,c3)+cr5*ui(i1+5,i2+1,i3+7,c3)+cr6*ui(i1+
     & 6,i2+1,i3+7,c3)+cr7*ui(i1+7,i2+1,i3+7,c3)+cr8*ui(i1+8,i2+1,i3+
     & 7,c3))+cs2*(cr0*ui(i1,i2+2,i3+7,c3)+cr1*ui(i1+1,i2+2,i3+7,c3)+
     & cr2*ui(i1+2,i2+2,i3+7,c3)+cr3*ui(i1+3,i2+2,i3+7,c3)+cr4*ui(i1+
     & 4,i2+2,i3+7,c3)+cr5*ui(i1+5,i2+2,i3+7,c3)+cr6*ui(i1+6,i2+2,i3+
     & 7,c3)+cr7*ui(i1+7,i2+2,i3+7,c3)+cr8*ui(i1+8,i2+2,i3+7,c3))+cs3*
     & (cr0*ui(i1,i2+3,i3+7,c3)+cr1*ui(i1+1,i2+3,i3+7,c3)+cr2*ui(i1+2,
     & i2+3,i3+7,c3)+cr3*ui(i1+3,i2+3,i3+7,c3)+cr4*ui(i1+4,i2+3,i3+7,
     & c3)+cr5*ui(i1+5,i2+3,i3+7,c3)+cr6*ui(i1+6,i2+3,i3+7,c3)+cr7*ui(
     & i1+7,i2+3,i3+7,c3)+cr8*ui(i1+8,i2+3,i3+7,c3))+cs4*(cr0*ui(i1,
     & i2+4,i3+7,c3)+cr1*ui(i1+1,i2+4,i3+7,c3)+cr2*ui(i1+2,i2+4,i3+7,
     & c3)+cr3*ui(i1+3,i2+4,i3+7,c3)+cr4*ui(i1+4,i2+4,i3+7,c3)+cr5*ui(
     & i1+5,i2+4,i3+7,c3)+cr6*ui(i1+6,i2+4,i3+7,c3)+cr7*ui(i1+7,i2+4,
     & i3+7,c3)+cr8*ui(i1+8,i2+4,i3+7,c3))+cs5*(cr0*ui(i1,i2+5,i3+7,
     & c3)+cr1*ui(i1+1,i2+5,i3+7,c3)+cr2*ui(i1+2,i2+5,i3+7,c3)+cr3*ui(
     & i1+3,i2+5,i3+7,c3)+cr4*ui(i1+4,i2+5,i3+7,c3)+cr5*ui(i1+5,i2+5,
     & i3+7,c3)+cr6*ui(i1+6,i2+5,i3+7,c3)+cr7*ui(i1+7,i2+5,i3+7,c3)+
     & cr8*ui(i1+8,i2+5,i3+7,c3))+cs6*(cr0*ui(i1,i2+6,i3+7,c3)+cr1*ui(
     & i1+1,i2+6,i3+7,c3)+cr2*ui(i1+2,i2+6,i3+7,c3)+cr3*ui(i1+3,i2+6,
     & i3+7,c3)+cr4*ui(i1+4,i2+6,i3+7,c3)+cr5*ui(i1+5,i2+6,i3+7,c3)+
     & cr6*ui(i1+6,i2+6,i3+7,c3)+cr7*ui(i1+7,i2+6,i3+7,c3)+cr8*ui(i1+
     & 8,i2+6,i3+7,c3))+cs7*(cr0*ui(i1,i2+7,i3+7,c3)+cr1*ui(i1+1,i2+7,
     & i3+7,c3)+cr2*ui(i1+2,i2+7,i3+7,c3)+cr3*ui(i1+3,i2+7,i3+7,c3)+
     & cr4*ui(i1+4,i2+7,i3+7,c3)+cr5*ui(i1+5,i2+7,i3+7,c3)+cr6*ui(i1+
     & 6,i2+7,i3+7,c3)+cr7*ui(i1+7,i2+7,i3+7,c3)+cr8*ui(i1+8,i2+7,i3+
     & 7,c3))+cs8*(cr0*ui(i1,i2+8,i3+7,c3)+cr1*ui(i1+1,i2+8,i3+7,c3)+
     & cr2*ui(i1+2,i2+8,i3+7,c3)+cr3*ui(i1+3,i2+8,i3+7,c3)+cr4*ui(i1+
     & 4,i2+8,i3+7,c3)+cr5*ui(i1+5,i2+8,i3+7,c3)+cr6*ui(i1+6,i2+8,i3+
     & 7,c3)+cr7*ui(i1+7,i2+8,i3+7,c3)+cr8*ui(i1+8,i2+8,i3+7,c3)))
               r(i) = r(i)+ct8*(cs0*(cr0*ui(i1,i2,i3+8,c3)+cr1*ui(i1+1,
     & i2,i3+8,c3)+cr2*ui(i1+2,i2,i3+8,c3)+cr3*ui(i1+3,i2,i3+8,c3)+
     & cr4*ui(i1+4,i2,i3+8,c3)+cr5*ui(i1+5,i2,i3+8,c3)+cr6*ui(i1+6,i2,
     & i3+8,c3)+cr7*ui(i1+7,i2,i3+8,c3)+cr8*ui(i1+8,i2,i3+8,c3))+cs1*(
     & cr0*ui(i1,i2+1,i3+8,c3)+cr1*ui(i1+1,i2+1,i3+8,c3)+cr2*ui(i1+2,
     & i2+1,i3+8,c3)+cr3*ui(i1+3,i2+1,i3+8,c3)+cr4*ui(i1+4,i2+1,i3+8,
     & c3)+cr5*ui(i1+5,i2+1,i3+8,c3)+cr6*ui(i1+6,i2+1,i3+8,c3)+cr7*ui(
     & i1+7,i2+1,i3+8,c3)+cr8*ui(i1+8,i2+1,i3+8,c3))+cs2*(cr0*ui(i1,
     & i2+2,i3+8,c3)+cr1*ui(i1+1,i2+2,i3+8,c3)+cr2*ui(i1+2,i2+2,i3+8,
     & c3)+cr3*ui(i1+3,i2+2,i3+8,c3)+cr4*ui(i1+4,i2+2,i3+8,c3)+cr5*ui(
     & i1+5,i2+2,i3+8,c3)+cr6*ui(i1+6,i2+2,i3+8,c3)+cr7*ui(i1+7,i2+2,
     & i3+8,c3)+cr8*ui(i1+8,i2+2,i3+8,c3))+cs3*(cr0*ui(i1,i2+3,i3+8,
     & c3)+cr1*ui(i1+1,i2+3,i3+8,c3)+cr2*ui(i1+2,i2+3,i3+8,c3)+cr3*ui(
     & i1+3,i2+3,i3+8,c3)+cr4*ui(i1+4,i2+3,i3+8,c3)+cr5*ui(i1+5,i2+3,
     & i3+8,c3)+cr6*ui(i1+6,i2+3,i3+8,c3)+cr7*ui(i1+7,i2+3,i3+8,c3)+
     & cr8*ui(i1+8,i2+3,i3+8,c3))+cs4*(cr0*ui(i1,i2+4,i3+8,c3)+cr1*ui(
     & i1+1,i2+4,i3+8,c3)+cr2*ui(i1+2,i2+4,i3+8,c3)+cr3*ui(i1+3,i2+4,
     & i3+8,c3)+cr4*ui(i1+4,i2+4,i3+8,c3)+cr5*ui(i1+5,i2+4,i3+8,c3)+
     & cr6*ui(i1+6,i2+4,i3+8,c3)+cr7*ui(i1+7,i2+4,i3+8,c3)+cr8*ui(i1+
     & 8,i2+4,i3+8,c3))+cs5*(cr0*ui(i1,i2+5,i3+8,c3)+cr1*ui(i1+1,i2+5,
     & i3+8,c3)+cr2*ui(i1+2,i2+5,i3+8,c3)+cr3*ui(i1+3,i2+5,i3+8,c3)+
     & cr4*ui(i1+4,i2+5,i3+8,c3)+cr5*ui(i1+5,i2+5,i3+8,c3)+cr6*ui(i1+
     & 6,i2+5,i3+8,c3)+cr7*ui(i1+7,i2+5,i3+8,c3)+cr8*ui(i1+8,i2+5,i3+
     & 8,c3))+cs6*(cr0*ui(i1,i2+6,i3+8,c3)+cr1*ui(i1+1,i2+6,i3+8,c3)+
     & cr2*ui(i1+2,i2+6,i3+8,c3)+cr3*ui(i1+3,i2+6,i3+8,c3)+cr4*ui(i1+
     & 4,i2+6,i3+8,c3)+cr5*ui(i1+5,i2+6,i3+8,c3)+cr6*ui(i1+6,i2+6,i3+
     & 8,c3)+cr7*ui(i1+7,i2+6,i3+8,c3)+cr8*ui(i1+8,i2+6,i3+8,c3))+cs7*
     & (cr0*ui(i1,i2+7,i3+8,c3)+cr1*ui(i1+1,i2+7,i3+8,c3)+cr2*ui(i1+2,
     & i2+7,i3+8,c3)+cr3*ui(i1+3,i2+7,i3+8,c3)+cr4*ui(i1+4,i2+7,i3+8,
     & c3)+cr5*ui(i1+5,i2+7,i3+8,c3)+cr6*ui(i1+6,i2+7,i3+8,c3)+cr7*ui(
     & i1+7,i2+7,i3+8,c3)+cr8*ui(i1+8,i2+7,i3+8,c3))+cs8*(cr0*ui(i1,
     & i2+8,i3+8,c3)+cr1*ui(i1+1,i2+8,i3+8,c3)+cr2*ui(i1+2,i2+8,i3+8,
     & c3)+cr3*ui(i1+3,i2+8,i3+8,c3)+cr4*ui(i1+4,i2+8,i3+8,c3)+cr5*ui(
     & i1+5,i2+8,i3+8,c3)+cr6*ui(i1+6,i2+8,i3+8,c3)+cr7*ui(i1+7,i2+8,
     & i3+8,c3)+cr8*ui(i1+8,i2+8,i3+8,c3)))
             else if( varWidth(i).eq.8 )then
! interpSparseStorage888(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q80(c(i,0,0,0))
                 cs0 = q80(c(i,1,0,0))
                 ct0 = q80(c(i,2,0,0))
                 cr1 = q81(c(i,0,0,0))
                 cs1 = q81(c(i,1,0,0))
                 ct1 = q81(c(i,2,0,0))
                 cr2 = q82(c(i,0,0,0))
                 cs2 = q82(c(i,1,0,0))
                 ct2 = q82(c(i,2,0,0))
                 cr3 = q83(c(i,0,0,0))
                 cs3 = q83(c(i,1,0,0))
                 ct3 = q83(c(i,2,0,0))
                 cr4 = q84(c(i,0,0,0))
                 cs4 = q84(c(i,1,0,0))
                 ct4 = q84(c(i,2,0,0))
                 cr5 = q85(c(i,0,0,0))
                 cs5 = q85(c(i,1,0,0))
                 ct5 = q85(c(i,2,0,0))
                 cr6 = q86(c(i,0,0,0))
                 cs6 = q86(c(i,1,0,0))
                 ct6 = q86(c(i,2,0,0))
                 cr7 = q87(c(i,0,0,0))
                 cs7 = q87(c(i,1,0,0))
                 ct7 = q87(c(i,2,0,0))
               r(i) = ct0*(cs0*(cr0*ui(i1,i2,i3+0,c3)+cr1*ui(i1+1,i2,
     & i3+0,c3)+cr2*ui(i1+2,i2,i3+0,c3)+cr3*ui(i1+3,i2,i3+0,c3)+cr4*
     & ui(i1+4,i2,i3+0,c3)+cr5*ui(i1+5,i2,i3+0,c3)+cr6*ui(i1+6,i2,i3+
     & 0,c3)+cr7*ui(i1+7,i2,i3+0,c3))+cs1*(cr0*ui(i1,i2+1,i3+0,c3)+
     & cr1*ui(i1+1,i2+1,i3+0,c3)+cr2*ui(i1+2,i2+1,i3+0,c3)+cr3*ui(i1+
     & 3,i2+1,i3+0,c3)+cr4*ui(i1+4,i2+1,i3+0,c3)+cr5*ui(i1+5,i2+1,i3+
     & 0,c3)+cr6*ui(i1+6,i2+1,i3+0,c3)+cr7*ui(i1+7,i2+1,i3+0,c3))+cs2*
     & (cr0*ui(i1,i2+2,i3+0,c3)+cr1*ui(i1+1,i2+2,i3+0,c3)+cr2*ui(i1+2,
     & i2+2,i3+0,c3)+cr3*ui(i1+3,i2+2,i3+0,c3)+cr4*ui(i1+4,i2+2,i3+0,
     & c3)+cr5*ui(i1+5,i2+2,i3+0,c3)+cr6*ui(i1+6,i2+2,i3+0,c3)+cr7*ui(
     & i1+7,i2+2,i3+0,c3))+cs3*(cr0*ui(i1,i2+3,i3+0,c3)+cr1*ui(i1+1,
     & i2+3,i3+0,c3)+cr2*ui(i1+2,i2+3,i3+0,c3)+cr3*ui(i1+3,i2+3,i3+0,
     & c3)+cr4*ui(i1+4,i2+3,i3+0,c3)+cr5*ui(i1+5,i2+3,i3+0,c3)+cr6*ui(
     & i1+6,i2+3,i3+0,c3)+cr7*ui(i1+7,i2+3,i3+0,c3))+cs4*(cr0*ui(i1,
     & i2+4,i3+0,c3)+cr1*ui(i1+1,i2+4,i3+0,c3)+cr2*ui(i1+2,i2+4,i3+0,
     & c3)+cr3*ui(i1+3,i2+4,i3+0,c3)+cr4*ui(i1+4,i2+4,i3+0,c3)+cr5*ui(
     & i1+5,i2+4,i3+0,c3)+cr6*ui(i1+6,i2+4,i3+0,c3)+cr7*ui(i1+7,i2+4,
     & i3+0,c3))+cs5*(cr0*ui(i1,i2+5,i3+0,c3)+cr1*ui(i1+1,i2+5,i3+0,
     & c3)+cr2*ui(i1+2,i2+5,i3+0,c3)+cr3*ui(i1+3,i2+5,i3+0,c3)+cr4*ui(
     & i1+4,i2+5,i3+0,c3)+cr5*ui(i1+5,i2+5,i3+0,c3)+cr6*ui(i1+6,i2+5,
     & i3+0,c3)+cr7*ui(i1+7,i2+5,i3+0,c3))+cs6*(cr0*ui(i1,i2+6,i3+0,
     & c3)+cr1*ui(i1+1,i2+6,i3+0,c3)+cr2*ui(i1+2,i2+6,i3+0,c3)+cr3*ui(
     & i1+3,i2+6,i3+0,c3)+cr4*ui(i1+4,i2+6,i3+0,c3)+cr5*ui(i1+5,i2+6,
     & i3+0,c3)+cr6*ui(i1+6,i2+6,i3+0,c3)+cr7*ui(i1+7,i2+6,i3+0,c3))+
     & cs7*(cr0*ui(i1,i2+7,i3+0,c3)+cr1*ui(i1+1,i2+7,i3+0,c3)+cr2*ui(
     & i1+2,i2+7,i3+0,c3)+cr3*ui(i1+3,i2+7,i3+0,c3)+cr4*ui(i1+4,i2+7,
     & i3+0,c3)+cr5*ui(i1+5,i2+7,i3+0,c3)+cr6*ui(i1+6,i2+7,i3+0,c3)+
     & cr7*ui(i1+7,i2+7,i3+0,c3)))+ct1*(cs0*(cr0*ui(i1,i2,i3+1,c3)+
     & cr1*ui(i1+1,i2,i3+1,c3)+cr2*ui(i1+2,i2,i3+1,c3)+cr3*ui(i1+3,i2,
     & i3+1,c3)+cr4*ui(i1+4,i2,i3+1,c3)+cr5*ui(i1+5,i2,i3+1,c3)+cr6*
     & ui(i1+6,i2,i3+1,c3)+cr7*ui(i1+7,i2,i3+1,c3))+cs1*(cr0*ui(i1,i2+
     & 1,i3+1,c3)+cr1*ui(i1+1,i2+1,i3+1,c3)+cr2*ui(i1+2,i2+1,i3+1,c3)+
     & cr3*ui(i1+3,i2+1,i3+1,c3)+cr4*ui(i1+4,i2+1,i3+1,c3)+cr5*ui(i1+
     & 5,i2+1,i3+1,c3)+cr6*ui(i1+6,i2+1,i3+1,c3)+cr7*ui(i1+7,i2+1,i3+
     & 1,c3))+cs2*(cr0*ui(i1,i2+2,i3+1,c3)+cr1*ui(i1+1,i2+2,i3+1,c3)+
     & cr2*ui(i1+2,i2+2,i3+1,c3)+cr3*ui(i1+3,i2+2,i3+1,c3)+cr4*ui(i1+
     & 4,i2+2,i3+1,c3)+cr5*ui(i1+5,i2+2,i3+1,c3)+cr6*ui(i1+6,i2+2,i3+
     & 1,c3)+cr7*ui(i1+7,i2+2,i3+1,c3))+cs3*(cr0*ui(i1,i2+3,i3+1,c3)+
     & cr1*ui(i1+1,i2+3,i3+1,c3)+cr2*ui(i1+2,i2+3,i3+1,c3)+cr3*ui(i1+
     & 3,i2+3,i3+1,c3)+cr4*ui(i1+4,i2+3,i3+1,c3)+cr5*ui(i1+5,i2+3,i3+
     & 1,c3)+cr6*ui(i1+6,i2+3,i3+1,c3)+cr7*ui(i1+7,i2+3,i3+1,c3))+cs4*
     & (cr0*ui(i1,i2+4,i3+1,c3)+cr1*ui(i1+1,i2+4,i3+1,c3)+cr2*ui(i1+2,
     & i2+4,i3+1,c3)+cr3*ui(i1+3,i2+4,i3+1,c3)+cr4*ui(i1+4,i2+4,i3+1,
     & c3)+cr5*ui(i1+5,i2+4,i3+1,c3)+cr6*ui(i1+6,i2+4,i3+1,c3)+cr7*ui(
     & i1+7,i2+4,i3+1,c3))+cs5*(cr0*ui(i1,i2+5,i3+1,c3)+cr1*ui(i1+1,
     & i2+5,i3+1,c3)+cr2*ui(i1+2,i2+5,i3+1,c3)+cr3*ui(i1+3,i2+5,i3+1,
     & c3)+cr4*ui(i1+4,i2+5,i3+1,c3)+cr5*ui(i1+5,i2+5,i3+1,c3)+cr6*ui(
     & i1+6,i2+5,i3+1,c3)+cr7*ui(i1+7,i2+5,i3+1,c3))+cs6*(cr0*ui(i1,
     & i2+6,i3+1,c3)+cr1*ui(i1+1,i2+6,i3+1,c3)+cr2*ui(i1+2,i2+6,i3+1,
     & c3)+cr3*ui(i1+3,i2+6,i3+1,c3)+cr4*ui(i1+4,i2+6,i3+1,c3)+cr5*ui(
     & i1+5,i2+6,i3+1,c3)+cr6*ui(i1+6,i2+6,i3+1,c3)+cr7*ui(i1+7,i2+6,
     & i3+1,c3))+cs7*(cr0*ui(i1,i2+7,i3+1,c3)+cr1*ui(i1+1,i2+7,i3+1,
     & c3)+cr2*ui(i1+2,i2+7,i3+1,c3)+cr3*ui(i1+3,i2+7,i3+1,c3)+cr4*ui(
     & i1+4,i2+7,i3+1,c3)+cr5*ui(i1+5,i2+7,i3+1,c3)+cr6*ui(i1+6,i2+7,
     & i3+1,c3)+cr7*ui(i1+7,i2+7,i3+1,c3)))+ct2*(cs0*(cr0*ui(i1,i2,i3+
     & 2,c3)+cr1*ui(i1+1,i2,i3+2,c3)+cr2*ui(i1+2,i2,i3+2,c3)+cr3*ui(
     & i1+3,i2,i3+2,c3)+cr4*ui(i1+4,i2,i3+2,c3)+cr5*ui(i1+5,i2,i3+2,
     & c3)+cr6*ui(i1+6,i2,i3+2,c3)+cr7*ui(i1+7,i2,i3+2,c3))+cs1*(cr0*
     & ui(i1,i2+1,i3+2,c3)+cr1*ui(i1+1,i2+1,i3+2,c3)+cr2*ui(i1+2,i2+1,
     & i3+2,c3)+cr3*ui(i1+3,i2+1,i3+2,c3)+cr4*ui(i1+4,i2+1,i3+2,c3)+
     & cr5*ui(i1+5,i2+1,i3+2,c3)+cr6*ui(i1+6,i2+1,i3+2,c3)+cr7*ui(i1+
     & 7,i2+1,i3+2,c3))+cs2*(cr0*ui(i1,i2+2,i3+2,c3)+cr1*ui(i1+1,i2+2,
     & i3+2,c3)+cr2*ui(i1+2,i2+2,i3+2,c3)+cr3*ui(i1+3,i2+2,i3+2,c3)+
     & cr4*ui(i1+4,i2+2,i3+2,c3)+cr5*ui(i1+5,i2+2,i3+2,c3)+cr6*ui(i1+
     & 6,i2+2,i3+2,c3)+cr7*ui(i1+7,i2+2,i3+2,c3))+cs3*(cr0*ui(i1,i2+3,
     & i3+2,c3)+cr1*ui(i1+1,i2+3,i3+2,c3)+cr2*ui(i1+2,i2+3,i3+2,c3)+
     & cr3*ui(i1+3,i2+3,i3+2,c3)+cr4*ui(i1+4,i2+3,i3+2,c3)+cr5*ui(i1+
     & 5,i2+3,i3+2,c3)+cr6*ui(i1+6,i2+3,i3+2,c3)+cr7*ui(i1+7,i2+3,i3+
     & 2,c3))+cs4*(cr0*ui(i1,i2+4,i3+2,c3)+cr1*ui(i1+1,i2+4,i3+2,c3)+
     & cr2*ui(i1+2,i2+4,i3+2,c3)+cr3*ui(i1+3,i2+4,i3+2,c3)+cr4*ui(i1+
     & 4,i2+4,i3+2,c3)+cr5*ui(i1+5,i2+4,i3+2,c3)+cr6*ui(i1+6,i2+4,i3+
     & 2,c3)+cr7*ui(i1+7,i2+4,i3+2,c3))+cs5*(cr0*ui(i1,i2+5,i3+2,c3)+
     & cr1*ui(i1+1,i2+5,i3+2,c3)+cr2*ui(i1+2,i2+5,i3+2,c3)+cr3*ui(i1+
     & 3,i2+5,i3+2,c3)+cr4*ui(i1+4,i2+5,i3+2,c3)+cr5*ui(i1+5,i2+5,i3+
     & 2,c3)+cr6*ui(i1+6,i2+5,i3+2,c3)+cr7*ui(i1+7,i2+5,i3+2,c3))+cs6*
     & (cr0*ui(i1,i2+6,i3+2,c3)+cr1*ui(i1+1,i2+6,i3+2,c3)+cr2*ui(i1+2,
     & i2+6,i3+2,c3)+cr3*ui(i1+3,i2+6,i3+2,c3)+cr4*ui(i1+4,i2+6,i3+2,
     & c3)+cr5*ui(i1+5,i2+6,i3+2,c3)+cr6*ui(i1+6,i2+6,i3+2,c3)+cr7*ui(
     & i1+7,i2+6,i3+2,c3))+cs7*(cr0*ui(i1,i2+7,i3+2,c3)+cr1*ui(i1+1,
     & i2+7,i3+2,c3)+cr2*ui(i1+2,i2+7,i3+2,c3)+cr3*ui(i1+3,i2+7,i3+2,
     & c3)+cr4*ui(i1+4,i2+7,i3+2,c3)+cr5*ui(i1+5,i2+7,i3+2,c3)+cr6*ui(
     & i1+6,i2+7,i3+2,c3)+cr7*ui(i1+7,i2+7,i3+2,c3)))
               r(i) = r(i)+ct3*(cs0*(cr0*ui(i1,i2,i3+3,c3)+cr1*ui(i1+1,
     & i2,i3+3,c3)+cr2*ui(i1+2,i2,i3+3,c3)+cr3*ui(i1+3,i2,i3+3,c3)+
     & cr4*ui(i1+4,i2,i3+3,c3)+cr5*ui(i1+5,i2,i3+3,c3)+cr6*ui(i1+6,i2,
     & i3+3,c3)+cr7*ui(i1+7,i2,i3+3,c3))+cs1*(cr0*ui(i1,i2+1,i3+3,c3)+
     & cr1*ui(i1+1,i2+1,i3+3,c3)+cr2*ui(i1+2,i2+1,i3+3,c3)+cr3*ui(i1+
     & 3,i2+1,i3+3,c3)+cr4*ui(i1+4,i2+1,i3+3,c3)+cr5*ui(i1+5,i2+1,i3+
     & 3,c3)+cr6*ui(i1+6,i2+1,i3+3,c3)+cr7*ui(i1+7,i2+1,i3+3,c3))+cs2*
     & (cr0*ui(i1,i2+2,i3+3,c3)+cr1*ui(i1+1,i2+2,i3+3,c3)+cr2*ui(i1+2,
     & i2+2,i3+3,c3)+cr3*ui(i1+3,i2+2,i3+3,c3)+cr4*ui(i1+4,i2+2,i3+3,
     & c3)+cr5*ui(i1+5,i2+2,i3+3,c3)+cr6*ui(i1+6,i2+2,i3+3,c3)+cr7*ui(
     & i1+7,i2+2,i3+3,c3))+cs3*(cr0*ui(i1,i2+3,i3+3,c3)+cr1*ui(i1+1,
     & i2+3,i3+3,c3)+cr2*ui(i1+2,i2+3,i3+3,c3)+cr3*ui(i1+3,i2+3,i3+3,
     & c3)+cr4*ui(i1+4,i2+3,i3+3,c3)+cr5*ui(i1+5,i2+3,i3+3,c3)+cr6*ui(
     & i1+6,i2+3,i3+3,c3)+cr7*ui(i1+7,i2+3,i3+3,c3))+cs4*(cr0*ui(i1,
     & i2+4,i3+3,c3)+cr1*ui(i1+1,i2+4,i3+3,c3)+cr2*ui(i1+2,i2+4,i3+3,
     & c3)+cr3*ui(i1+3,i2+4,i3+3,c3)+cr4*ui(i1+4,i2+4,i3+3,c3)+cr5*ui(
     & i1+5,i2+4,i3+3,c3)+cr6*ui(i1+6,i2+4,i3+3,c3)+cr7*ui(i1+7,i2+4,
     & i3+3,c3))+cs5*(cr0*ui(i1,i2+5,i3+3,c3)+cr1*ui(i1+1,i2+5,i3+3,
     & c3)+cr2*ui(i1+2,i2+5,i3+3,c3)+cr3*ui(i1+3,i2+5,i3+3,c3)+cr4*ui(
     & i1+4,i2+5,i3+3,c3)+cr5*ui(i1+5,i2+5,i3+3,c3)+cr6*ui(i1+6,i2+5,
     & i3+3,c3)+cr7*ui(i1+7,i2+5,i3+3,c3))+cs6*(cr0*ui(i1,i2+6,i3+3,
     & c3)+cr1*ui(i1+1,i2+6,i3+3,c3)+cr2*ui(i1+2,i2+6,i3+3,c3)+cr3*ui(
     & i1+3,i2+6,i3+3,c3)+cr4*ui(i1+4,i2+6,i3+3,c3)+cr5*ui(i1+5,i2+6,
     & i3+3,c3)+cr6*ui(i1+6,i2+6,i3+3,c3)+cr7*ui(i1+7,i2+6,i3+3,c3))+
     & cs7*(cr0*ui(i1,i2+7,i3+3,c3)+cr1*ui(i1+1,i2+7,i3+3,c3)+cr2*ui(
     & i1+2,i2+7,i3+3,c3)+cr3*ui(i1+3,i2+7,i3+3,c3)+cr4*ui(i1+4,i2+7,
     & i3+3,c3)+cr5*ui(i1+5,i2+7,i3+3,c3)+cr6*ui(i1+6,i2+7,i3+3,c3)+
     & cr7*ui(i1+7,i2+7,i3+3,c3)))+ct4*(cs0*(cr0*ui(i1,i2,i3+4,c3)+
     & cr1*ui(i1+1,i2,i3+4,c3)+cr2*ui(i1+2,i2,i3+4,c3)+cr3*ui(i1+3,i2,
     & i3+4,c3)+cr4*ui(i1+4,i2,i3+4,c3)+cr5*ui(i1+5,i2,i3+4,c3)+cr6*
     & ui(i1+6,i2,i3+4,c3)+cr7*ui(i1+7,i2,i3+4,c3))+cs1*(cr0*ui(i1,i2+
     & 1,i3+4,c3)+cr1*ui(i1+1,i2+1,i3+4,c3)+cr2*ui(i1+2,i2+1,i3+4,c3)+
     & cr3*ui(i1+3,i2+1,i3+4,c3)+cr4*ui(i1+4,i2+1,i3+4,c3)+cr5*ui(i1+
     & 5,i2+1,i3+4,c3)+cr6*ui(i1+6,i2+1,i3+4,c3)+cr7*ui(i1+7,i2+1,i3+
     & 4,c3))+cs2*(cr0*ui(i1,i2+2,i3+4,c3)+cr1*ui(i1+1,i2+2,i3+4,c3)+
     & cr2*ui(i1+2,i2+2,i3+4,c3)+cr3*ui(i1+3,i2+2,i3+4,c3)+cr4*ui(i1+
     & 4,i2+2,i3+4,c3)+cr5*ui(i1+5,i2+2,i3+4,c3)+cr6*ui(i1+6,i2+2,i3+
     & 4,c3)+cr7*ui(i1+7,i2+2,i3+4,c3))+cs3*(cr0*ui(i1,i2+3,i3+4,c3)+
     & cr1*ui(i1+1,i2+3,i3+4,c3)+cr2*ui(i1+2,i2+3,i3+4,c3)+cr3*ui(i1+
     & 3,i2+3,i3+4,c3)+cr4*ui(i1+4,i2+3,i3+4,c3)+cr5*ui(i1+5,i2+3,i3+
     & 4,c3)+cr6*ui(i1+6,i2+3,i3+4,c3)+cr7*ui(i1+7,i2+3,i3+4,c3))+cs4*
     & (cr0*ui(i1,i2+4,i3+4,c3)+cr1*ui(i1+1,i2+4,i3+4,c3)+cr2*ui(i1+2,
     & i2+4,i3+4,c3)+cr3*ui(i1+3,i2+4,i3+4,c3)+cr4*ui(i1+4,i2+4,i3+4,
     & c3)+cr5*ui(i1+5,i2+4,i3+4,c3)+cr6*ui(i1+6,i2+4,i3+4,c3)+cr7*ui(
     & i1+7,i2+4,i3+4,c3))+cs5*(cr0*ui(i1,i2+5,i3+4,c3)+cr1*ui(i1+1,
     & i2+5,i3+4,c3)+cr2*ui(i1+2,i2+5,i3+4,c3)+cr3*ui(i1+3,i2+5,i3+4,
     & c3)+cr4*ui(i1+4,i2+5,i3+4,c3)+cr5*ui(i1+5,i2+5,i3+4,c3)+cr6*ui(
     & i1+6,i2+5,i3+4,c3)+cr7*ui(i1+7,i2+5,i3+4,c3))+cs6*(cr0*ui(i1,
     & i2+6,i3+4,c3)+cr1*ui(i1+1,i2+6,i3+4,c3)+cr2*ui(i1+2,i2+6,i3+4,
     & c3)+cr3*ui(i1+3,i2+6,i3+4,c3)+cr4*ui(i1+4,i2+6,i3+4,c3)+cr5*ui(
     & i1+5,i2+6,i3+4,c3)+cr6*ui(i1+6,i2+6,i3+4,c3)+cr7*ui(i1+7,i2+6,
     & i3+4,c3))+cs7*(cr0*ui(i1,i2+7,i3+4,c3)+cr1*ui(i1+1,i2+7,i3+4,
     & c3)+cr2*ui(i1+2,i2+7,i3+4,c3)+cr3*ui(i1+3,i2+7,i3+4,c3)+cr4*ui(
     & i1+4,i2+7,i3+4,c3)+cr5*ui(i1+5,i2+7,i3+4,c3)+cr6*ui(i1+6,i2+7,
     & i3+4,c3)+cr7*ui(i1+7,i2+7,i3+4,c3)))+ct5*(cs0*(cr0*ui(i1,i2,i3+
     & 5,c3)+cr1*ui(i1+1,i2,i3+5,c3)+cr2*ui(i1+2,i2,i3+5,c3)+cr3*ui(
     & i1+3,i2,i3+5,c3)+cr4*ui(i1+4,i2,i3+5,c3)+cr5*ui(i1+5,i2,i3+5,
     & c3)+cr6*ui(i1+6,i2,i3+5,c3)+cr7*ui(i1+7,i2,i3+5,c3))+cs1*(cr0*
     & ui(i1,i2+1,i3+5,c3)+cr1*ui(i1+1,i2+1,i3+5,c3)+cr2*ui(i1+2,i2+1,
     & i3+5,c3)+cr3*ui(i1+3,i2+1,i3+5,c3)+cr4*ui(i1+4,i2+1,i3+5,c3)+
     & cr5*ui(i1+5,i2+1,i3+5,c3)+cr6*ui(i1+6,i2+1,i3+5,c3)+cr7*ui(i1+
     & 7,i2+1,i3+5,c3))+cs2*(cr0*ui(i1,i2+2,i3+5,c3)+cr1*ui(i1+1,i2+2,
     & i3+5,c3)+cr2*ui(i1+2,i2+2,i3+5,c3)+cr3*ui(i1+3,i2+2,i3+5,c3)+
     & cr4*ui(i1+4,i2+2,i3+5,c3)+cr5*ui(i1+5,i2+2,i3+5,c3)+cr6*ui(i1+
     & 6,i2+2,i3+5,c3)+cr7*ui(i1+7,i2+2,i3+5,c3))+cs3*(cr0*ui(i1,i2+3,
     & i3+5,c3)+cr1*ui(i1+1,i2+3,i3+5,c3)+cr2*ui(i1+2,i2+3,i3+5,c3)+
     & cr3*ui(i1+3,i2+3,i3+5,c3)+cr4*ui(i1+4,i2+3,i3+5,c3)+cr5*ui(i1+
     & 5,i2+3,i3+5,c3)+cr6*ui(i1+6,i2+3,i3+5,c3)+cr7*ui(i1+7,i2+3,i3+
     & 5,c3))+cs4*(cr0*ui(i1,i2+4,i3+5,c3)+cr1*ui(i1+1,i2+4,i3+5,c3)+
     & cr2*ui(i1+2,i2+4,i3+5,c3)+cr3*ui(i1+3,i2+4,i3+5,c3)+cr4*ui(i1+
     & 4,i2+4,i3+5,c3)+cr5*ui(i1+5,i2+4,i3+5,c3)+cr6*ui(i1+6,i2+4,i3+
     & 5,c3)+cr7*ui(i1+7,i2+4,i3+5,c3))+cs5*(cr0*ui(i1,i2+5,i3+5,c3)+
     & cr1*ui(i1+1,i2+5,i3+5,c3)+cr2*ui(i1+2,i2+5,i3+5,c3)+cr3*ui(i1+
     & 3,i2+5,i3+5,c3)+cr4*ui(i1+4,i2+5,i3+5,c3)+cr5*ui(i1+5,i2+5,i3+
     & 5,c3)+cr6*ui(i1+6,i2+5,i3+5,c3)+cr7*ui(i1+7,i2+5,i3+5,c3))+cs6*
     & (cr0*ui(i1,i2+6,i3+5,c3)+cr1*ui(i1+1,i2+6,i3+5,c3)+cr2*ui(i1+2,
     & i2+6,i3+5,c3)+cr3*ui(i1+3,i2+6,i3+5,c3)+cr4*ui(i1+4,i2+6,i3+5,
     & c3)+cr5*ui(i1+5,i2+6,i3+5,c3)+cr6*ui(i1+6,i2+6,i3+5,c3)+cr7*ui(
     & i1+7,i2+6,i3+5,c3))+cs7*(cr0*ui(i1,i2+7,i3+5,c3)+cr1*ui(i1+1,
     & i2+7,i3+5,c3)+cr2*ui(i1+2,i2+7,i3+5,c3)+cr3*ui(i1+3,i2+7,i3+5,
     & c3)+cr4*ui(i1+4,i2+7,i3+5,c3)+cr5*ui(i1+5,i2+7,i3+5,c3)+cr6*ui(
     & i1+6,i2+7,i3+5,c3)+cr7*ui(i1+7,i2+7,i3+5,c3)))
               r(i) = r(i)+ct6*(cs0*(cr0*ui(i1,i2,i3+6,c3)+cr1*ui(i1+1,
     & i2,i3+6,c3)+cr2*ui(i1+2,i2,i3+6,c3)+cr3*ui(i1+3,i2,i3+6,c3)+
     & cr4*ui(i1+4,i2,i3+6,c3)+cr5*ui(i1+5,i2,i3+6,c3)+cr6*ui(i1+6,i2,
     & i3+6,c3)+cr7*ui(i1+7,i2,i3+6,c3))+cs1*(cr0*ui(i1,i2+1,i3+6,c3)+
     & cr1*ui(i1+1,i2+1,i3+6,c3)+cr2*ui(i1+2,i2+1,i3+6,c3)+cr3*ui(i1+
     & 3,i2+1,i3+6,c3)+cr4*ui(i1+4,i2+1,i3+6,c3)+cr5*ui(i1+5,i2+1,i3+
     & 6,c3)+cr6*ui(i1+6,i2+1,i3+6,c3)+cr7*ui(i1+7,i2+1,i3+6,c3))+cs2*
     & (cr0*ui(i1,i2+2,i3+6,c3)+cr1*ui(i1+1,i2+2,i3+6,c3)+cr2*ui(i1+2,
     & i2+2,i3+6,c3)+cr3*ui(i1+3,i2+2,i3+6,c3)+cr4*ui(i1+4,i2+2,i3+6,
     & c3)+cr5*ui(i1+5,i2+2,i3+6,c3)+cr6*ui(i1+6,i2+2,i3+6,c3)+cr7*ui(
     & i1+7,i2+2,i3+6,c3))+cs3*(cr0*ui(i1,i2+3,i3+6,c3)+cr1*ui(i1+1,
     & i2+3,i3+6,c3)+cr2*ui(i1+2,i2+3,i3+6,c3)+cr3*ui(i1+3,i2+3,i3+6,
     & c3)+cr4*ui(i1+4,i2+3,i3+6,c3)+cr5*ui(i1+5,i2+3,i3+6,c3)+cr6*ui(
     & i1+6,i2+3,i3+6,c3)+cr7*ui(i1+7,i2+3,i3+6,c3))+cs4*(cr0*ui(i1,
     & i2+4,i3+6,c3)+cr1*ui(i1+1,i2+4,i3+6,c3)+cr2*ui(i1+2,i2+4,i3+6,
     & c3)+cr3*ui(i1+3,i2+4,i3+6,c3)+cr4*ui(i1+4,i2+4,i3+6,c3)+cr5*ui(
     & i1+5,i2+4,i3+6,c3)+cr6*ui(i1+6,i2+4,i3+6,c3)+cr7*ui(i1+7,i2+4,
     & i3+6,c3))+cs5*(cr0*ui(i1,i2+5,i3+6,c3)+cr1*ui(i1+1,i2+5,i3+6,
     & c3)+cr2*ui(i1+2,i2+5,i3+6,c3)+cr3*ui(i1+3,i2+5,i3+6,c3)+cr4*ui(
     & i1+4,i2+5,i3+6,c3)+cr5*ui(i1+5,i2+5,i3+6,c3)+cr6*ui(i1+6,i2+5,
     & i3+6,c3)+cr7*ui(i1+7,i2+5,i3+6,c3))+cs6*(cr0*ui(i1,i2+6,i3+6,
     & c3)+cr1*ui(i1+1,i2+6,i3+6,c3)+cr2*ui(i1+2,i2+6,i3+6,c3)+cr3*ui(
     & i1+3,i2+6,i3+6,c3)+cr4*ui(i1+4,i2+6,i3+6,c3)+cr5*ui(i1+5,i2+6,
     & i3+6,c3)+cr6*ui(i1+6,i2+6,i3+6,c3)+cr7*ui(i1+7,i2+6,i3+6,c3))+
     & cs7*(cr0*ui(i1,i2+7,i3+6,c3)+cr1*ui(i1+1,i2+7,i3+6,c3)+cr2*ui(
     & i1+2,i2+7,i3+6,c3)+cr3*ui(i1+3,i2+7,i3+6,c3)+cr4*ui(i1+4,i2+7,
     & i3+6,c3)+cr5*ui(i1+5,i2+7,i3+6,c3)+cr6*ui(i1+6,i2+7,i3+6,c3)+
     & cr7*ui(i1+7,i2+7,i3+6,c3)))+ct7*(cs0*(cr0*ui(i1,i2,i3+7,c3)+
     & cr1*ui(i1+1,i2,i3+7,c3)+cr2*ui(i1+2,i2,i3+7,c3)+cr3*ui(i1+3,i2,
     & i3+7,c3)+cr4*ui(i1+4,i2,i3+7,c3)+cr5*ui(i1+5,i2,i3+7,c3)+cr6*
     & ui(i1+6,i2,i3+7,c3)+cr7*ui(i1+7,i2,i3+7,c3))+cs1*(cr0*ui(i1,i2+
     & 1,i3+7,c3)+cr1*ui(i1+1,i2+1,i3+7,c3)+cr2*ui(i1+2,i2+1,i3+7,c3)+
     & cr3*ui(i1+3,i2+1,i3+7,c3)+cr4*ui(i1+4,i2+1,i3+7,c3)+cr5*ui(i1+
     & 5,i2+1,i3+7,c3)+cr6*ui(i1+6,i2+1,i3+7,c3)+cr7*ui(i1+7,i2+1,i3+
     & 7,c3))+cs2*(cr0*ui(i1,i2+2,i3+7,c3)+cr1*ui(i1+1,i2+2,i3+7,c3)+
     & cr2*ui(i1+2,i2+2,i3+7,c3)+cr3*ui(i1+3,i2+2,i3+7,c3)+cr4*ui(i1+
     & 4,i2+2,i3+7,c3)+cr5*ui(i1+5,i2+2,i3+7,c3)+cr6*ui(i1+6,i2+2,i3+
     & 7,c3)+cr7*ui(i1+7,i2+2,i3+7,c3))+cs3*(cr0*ui(i1,i2+3,i3+7,c3)+
     & cr1*ui(i1+1,i2+3,i3+7,c3)+cr2*ui(i1+2,i2+3,i3+7,c3)+cr3*ui(i1+
     & 3,i2+3,i3+7,c3)+cr4*ui(i1+4,i2+3,i3+7,c3)+cr5*ui(i1+5,i2+3,i3+
     & 7,c3)+cr6*ui(i1+6,i2+3,i3+7,c3)+cr7*ui(i1+7,i2+3,i3+7,c3))+cs4*
     & (cr0*ui(i1,i2+4,i3+7,c3)+cr1*ui(i1+1,i2+4,i3+7,c3)+cr2*ui(i1+2,
     & i2+4,i3+7,c3)+cr3*ui(i1+3,i2+4,i3+7,c3)+cr4*ui(i1+4,i2+4,i3+7,
     & c3)+cr5*ui(i1+5,i2+4,i3+7,c3)+cr6*ui(i1+6,i2+4,i3+7,c3)+cr7*ui(
     & i1+7,i2+4,i3+7,c3))+cs5*(cr0*ui(i1,i2+5,i3+7,c3)+cr1*ui(i1+1,
     & i2+5,i3+7,c3)+cr2*ui(i1+2,i2+5,i3+7,c3)+cr3*ui(i1+3,i2+5,i3+7,
     & c3)+cr4*ui(i1+4,i2+5,i3+7,c3)+cr5*ui(i1+5,i2+5,i3+7,c3)+cr6*ui(
     & i1+6,i2+5,i3+7,c3)+cr7*ui(i1+7,i2+5,i3+7,c3))+cs6*(cr0*ui(i1,
     & i2+6,i3+7,c3)+cr1*ui(i1+1,i2+6,i3+7,c3)+cr2*ui(i1+2,i2+6,i3+7,
     & c3)+cr3*ui(i1+3,i2+6,i3+7,c3)+cr4*ui(i1+4,i2+6,i3+7,c3)+cr5*ui(
     & i1+5,i2+6,i3+7,c3)+cr6*ui(i1+6,i2+6,i3+7,c3)+cr7*ui(i1+7,i2+6,
     & i3+7,c3))+cs7*(cr0*ui(i1,i2+7,i3+7,c3)+cr1*ui(i1+1,i2+7,i3+7,
     & c3)+cr2*ui(i1+2,i2+7,i3+7,c3)+cr3*ui(i1+3,i2+7,i3+7,c3)+cr4*ui(
     & i1+4,i2+7,i3+7,c3)+cr5*ui(i1+5,i2+7,i3+7,c3)+cr6*ui(i1+6,i2+7,
     & i3+7,c3)+cr7*ui(i1+7,i2+7,i3+7,c3)))
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
! loops3d($interpSparseStorage333(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))
           if( c3a.eq.c3b )then
             do c3=c3a,c3b
             do i=nia,nib
! interpSparseStorage333(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q30(c(i,0,0,0))
                 cs0 = q30(c(i,1,0,0))
                 ct0 = q30(c(i,2,0,0))
                 cr1 = q31(c(i,0,0,0))
                 cs1 = q31(c(i,1,0,0))
                 ct1 = q31(c(i,2,0,0))
                 cr2 = q32(c(i,0,0,0))
                 cs2 = q32(c(i,1,0,0))
                 ct2 = q32(c(i,2,0,0))
               r(i) = ct0*(cs0*(cr0*ui(i1,i2,i3+0,c3)+cr1*ui(i1+1,i2,
     & i3+0,c3)+cr2*ui(i1+2,i2,i3+0,c3))+cs1*(cr0*ui(i1,i2+1,i3+0,c3)+
     & cr1*ui(i1+1,i2+1,i3+0,c3)+cr2*ui(i1+2,i2+1,i3+0,c3))+cs2*(cr0*
     & ui(i1,i2+2,i3+0,c3)+cr1*ui(i1+1,i2+2,i3+0,c3)+cr2*ui(i1+2,i2+2,
     & i3+0,c3)))+ct1*(cs0*(cr0*ui(i1,i2,i3+1,c3)+cr1*ui(i1+1,i2,i3+1,
     & c3)+cr2*ui(i1+2,i2,i3+1,c3))+cs1*(cr0*ui(i1,i2+1,i3+1,c3)+cr1*
     & ui(i1+1,i2+1,i3+1,c3)+cr2*ui(i1+2,i2+1,i3+1,c3))+cs2*(cr0*ui(
     & i1,i2+2,i3+1,c3)+cr1*ui(i1+1,i2+2,i3+1,c3)+cr2*ui(i1+2,i2+2,i3+
     & 1,c3)))+ct2*(cs0*(cr0*ui(i1,i2,i3+2,c3)+cr1*ui(i1+1,i2,i3+2,c3)
     & +cr2*ui(i1+2,i2,i3+2,c3))+cs1*(cr0*ui(i1,i2+1,i3+2,c3)+cr1*ui(
     & i1+1,i2+1,i3+2,c3)+cr2*ui(i1+2,i2+1,i3+2,c3))+cs2*(cr0*ui(i1,
     & i2+2,i3+2,c3)+cr1*ui(i1+1,i2+2,i3+2,c3)+cr2*ui(i1+2,i2+2,i3+2,
     & c3)))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
! interpSparseStorage333(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q30(c(i,0,0,0))
                 cs0 = q30(c(i,1,0,0))
                 ct0 = q30(c(i,2,0,0))
                 cr1 = q31(c(i,0,0,0))
                 cs1 = q31(c(i,1,0,0))
                 ct1 = q31(c(i,2,0,0))
                 cr2 = q32(c(i,0,0,0))
                 cs2 = q32(c(i,1,0,0))
                 ct2 = q32(c(i,2,0,0))
               r(i) = ct0*(cs0*(cr0*ui(i1,i2,i3+0,c3)+cr1*ui(i1+1,i2,
     & i3+0,c3)+cr2*ui(i1+2,i2,i3+0,c3))+cs1*(cr0*ui(i1,i2+1,i3+0,c3)+
     & cr1*ui(i1+1,i2+1,i3+0,c3)+cr2*ui(i1+2,i2+1,i3+0,c3))+cs2*(cr0*
     & ui(i1,i2+2,i3+0,c3)+cr1*ui(i1+1,i2+2,i3+0,c3)+cr2*ui(i1+2,i2+2,
     & i3+0,c3)))+ct1*(cs0*(cr0*ui(i1,i2,i3+1,c3)+cr1*ui(i1+1,i2,i3+1,
     & c3)+cr2*ui(i1+2,i2,i3+1,c3))+cs1*(cr0*ui(i1,i2+1,i3+1,c3)+cr1*
     & ui(i1+1,i2+1,i3+1,c3)+cr2*ui(i1+2,i2+1,i3+1,c3))+cs2*(cr0*ui(
     & i1,i2+2,i3+1,c3)+cr1*ui(i1+1,i2+2,i3+1,c3)+cr2*ui(i1+2,i2+2,i3+
     & 1,c3)))+ct2*(cs0*(cr0*ui(i1,i2,i3+2,c3)+cr1*ui(i1+1,i2,i3+2,c3)
     & +cr2*ui(i1+2,i2,i3+2,c3))+cs1*(cr0*ui(i1,i2+1,i3+2,c3)+cr1*ui(
     & i1+1,i2+1,i3+2,c3)+cr2*ui(i1+2,i2+1,i3+2,c3))+cs2*(cr0*ui(i1,
     & i2+2,i3+2,c3)+cr1*ui(i1+1,i2+2,i3+2,c3)+cr2*ui(i1+2,i2+2,i3+2,
     & c3)))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           end if
         else if( width(1).eq.2 .and. width(2).eq.2 .and. width(3)
     & .eq.2 )then
! loops3d($interpSparseStorage222(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))
           if( c3a.eq.c3b )then
             do c3=c3a,c3b
             do i=nia,nib
! interpSparseStorage222(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q20(c(i,0,0,0))
                 cs0 = q20(c(i,1,0,0))
                 ct0 = q20(c(i,2,0,0))
                 cr1 = q21(c(i,0,0,0))
                 cs1 = q21(c(i,1,0,0))
                 ct1 = q21(c(i,2,0,0))
               r(i) = ct0*(cs0*(cr0*ui(i1,i2,i3+0,c3)+cr1*ui(i1+1,i2,
     & i3+0,c3))+cs1*(cr0*ui(i1,i2+1,i3+0,c3)+cr1*ui(i1+1,i2+1,i3+0,
     & c3)))+ct1*(cs0*(cr0*ui(i1,i2,i3+1,c3)+cr1*ui(i1+1,i2,i3+1,c3))+
     & cs1*(cr0*ui(i1,i2+1,i3+1,c3)+cr1*ui(i1+1,i2+1,i3+1,c3)))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
! interpSparseStorage222(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q20(c(i,0,0,0))
                 cs0 = q20(c(i,1,0,0))
                 ct0 = q20(c(i,2,0,0))
                 cr1 = q21(c(i,0,0,0))
                 cs1 = q21(c(i,1,0,0))
                 ct1 = q21(c(i,2,0,0))
               r(i) = ct0*(cs0*(cr0*ui(i1,i2,i3+0,c3)+cr1*ui(i1+1,i2,
     & i3+0,c3))+cs1*(cr0*ui(i1,i2+1,i3+0,c3)+cr1*ui(i1+1,i2+1,i3+0,
     & c3)))+ct1*(cs0*(cr0*ui(i1,i2,i3+1,c3)+cr1*ui(i1+1,i2,i3+1,c3))+
     & cs1*(cr0*ui(i1,i2+1,i3+1,c3)+cr1*ui(i1+1,i2+1,i3+1,c3)))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           end if
         else if( width(1).eq.4 .and. width(2).eq.4 .and. width(3)
     & .eq.4 )then
! loops3d($interpSparseStorage444(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))
           if( c3a.eq.c3b )then
             do c3=c3a,c3b
             do i=nia,nib
! interpSparseStorage444(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q40(c(i,0,0,0))
                 cs0 = q40(c(i,1,0,0))
                 ct0 = q40(c(i,2,0,0))
                 cr1 = q41(c(i,0,0,0))
                 cs1 = q41(c(i,1,0,0))
                 ct1 = q41(c(i,2,0,0))
                 cr2 = q42(c(i,0,0,0))
                 cs2 = q42(c(i,1,0,0))
                 ct2 = q42(c(i,2,0,0))
                 cr3 = q43(c(i,0,0,0))
                 cs3 = q43(c(i,1,0,0))
                 ct3 = q43(c(i,2,0,0))
               r(i) = ct0*(cs0*(cr0*ui(i1,i2,i3+0,c3)+cr1*ui(i1+1,i2,
     & i3+0,c3)+cr2*ui(i1+2,i2,i3+0,c3)+cr3*ui(i1+3,i2,i3+0,c3))+cs1*(
     & cr0*ui(i1,i2+1,i3+0,c3)+cr1*ui(i1+1,i2+1,i3+0,c3)+cr2*ui(i1+2,
     & i2+1,i3+0,c3)+cr3*ui(i1+3,i2+1,i3+0,c3))+cs2*(cr0*ui(i1,i2+2,
     & i3+0,c3)+cr1*ui(i1+1,i2+2,i3+0,c3)+cr2*ui(i1+2,i2+2,i3+0,c3)+
     & cr3*ui(i1+3,i2+2,i3+0,c3))+cs3*(cr0*ui(i1,i2+3,i3+0,c3)+cr1*ui(
     & i1+1,i2+3,i3+0,c3)+cr2*ui(i1+2,i2+3,i3+0,c3)+cr3*ui(i1+3,i2+3,
     & i3+0,c3)))+ct1*(cs0*(cr0*ui(i1,i2,i3+1,c3)+cr1*ui(i1+1,i2,i3+1,
     & c3)+cr2*ui(i1+2,i2,i3+1,c3)+cr3*ui(i1+3,i2,i3+1,c3))+cs1*(cr0*
     & ui(i1,i2+1,i3+1,c3)+cr1*ui(i1+1,i2+1,i3+1,c3)+cr2*ui(i1+2,i2+1,
     & i3+1,c3)+cr3*ui(i1+3,i2+1,i3+1,c3))+cs2*(cr0*ui(i1,i2+2,i3+1,
     & c3)+cr1*ui(i1+1,i2+2,i3+1,c3)+cr2*ui(i1+2,i2+2,i3+1,c3)+cr3*ui(
     & i1+3,i2+2,i3+1,c3))+cs3*(cr0*ui(i1,i2+3,i3+1,c3)+cr1*ui(i1+1,
     & i2+3,i3+1,c3)+cr2*ui(i1+2,i2+3,i3+1,c3)+cr3*ui(i1+3,i2+3,i3+1,
     & c3)))+ct2*(cs0*(cr0*ui(i1,i2,i3+2,c3)+cr1*ui(i1+1,i2,i3+2,c3)+
     & cr2*ui(i1+2,i2,i3+2,c3)+cr3*ui(i1+3,i2,i3+2,c3))+cs1*(cr0*ui(
     & i1,i2+1,i3+2,c3)+cr1*ui(i1+1,i2+1,i3+2,c3)+cr2*ui(i1+2,i2+1,i3+
     & 2,c3)+cr3*ui(i1+3,i2+1,i3+2,c3))+cs2*(cr0*ui(i1,i2+2,i3+2,c3)+
     & cr1*ui(i1+1,i2+2,i3+2,c3)+cr2*ui(i1+2,i2+2,i3+2,c3)+cr3*ui(i1+
     & 3,i2+2,i3+2,c3))+cs3*(cr0*ui(i1,i2+3,i3+2,c3)+cr1*ui(i1+1,i2+3,
     & i3+2,c3)+cr2*ui(i1+2,i2+3,i3+2,c3)+cr3*ui(i1+3,i2+3,i3+2,c3)))+
     & ct3*(cs0*(cr0*ui(i1,i2,i3+3,c3)+cr1*ui(i1+1,i2,i3+3,c3)+cr2*ui(
     & i1+2,i2,i3+3,c3)+cr3*ui(i1+3,i2,i3+3,c3))+cs1*(cr0*ui(i1,i2+1,
     & i3+3,c3)+cr1*ui(i1+1,i2+1,i3+3,c3)+cr2*ui(i1+2,i2+1,i3+3,c3)+
     & cr3*ui(i1+3,i2+1,i3+3,c3))+cs2*(cr0*ui(i1,i2+2,i3+3,c3)+cr1*ui(
     & i1+1,i2+2,i3+3,c3)+cr2*ui(i1+2,i2+2,i3+3,c3)+cr3*ui(i1+3,i2+2,
     & i3+3,c3))+cs3*(cr0*ui(i1,i2+3,i3+3,c3)+cr1*ui(i1+1,i2+3,i3+3,
     & c3)+cr2*ui(i1+2,i2+3,i3+3,c3)+cr3*ui(i1+3,i2+3,i3+3,c3)))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
! interpSparseStorage444(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q40(c(i,0,0,0))
                 cs0 = q40(c(i,1,0,0))
                 ct0 = q40(c(i,2,0,0))
                 cr1 = q41(c(i,0,0,0))
                 cs1 = q41(c(i,1,0,0))
                 ct1 = q41(c(i,2,0,0))
                 cr2 = q42(c(i,0,0,0))
                 cs2 = q42(c(i,1,0,0))
                 ct2 = q42(c(i,2,0,0))
                 cr3 = q43(c(i,0,0,0))
                 cs3 = q43(c(i,1,0,0))
                 ct3 = q43(c(i,2,0,0))
               r(i) = ct0*(cs0*(cr0*ui(i1,i2,i3+0,c3)+cr1*ui(i1+1,i2,
     & i3+0,c3)+cr2*ui(i1+2,i2,i3+0,c3)+cr3*ui(i1+3,i2,i3+0,c3))+cs1*(
     & cr0*ui(i1,i2+1,i3+0,c3)+cr1*ui(i1+1,i2+1,i3+0,c3)+cr2*ui(i1+2,
     & i2+1,i3+0,c3)+cr3*ui(i1+3,i2+1,i3+0,c3))+cs2*(cr0*ui(i1,i2+2,
     & i3+0,c3)+cr1*ui(i1+1,i2+2,i3+0,c3)+cr2*ui(i1+2,i2+2,i3+0,c3)+
     & cr3*ui(i1+3,i2+2,i3+0,c3))+cs3*(cr0*ui(i1,i2+3,i3+0,c3)+cr1*ui(
     & i1+1,i2+3,i3+0,c3)+cr2*ui(i1+2,i2+3,i3+0,c3)+cr3*ui(i1+3,i2+3,
     & i3+0,c3)))+ct1*(cs0*(cr0*ui(i1,i2,i3+1,c3)+cr1*ui(i1+1,i2,i3+1,
     & c3)+cr2*ui(i1+2,i2,i3+1,c3)+cr3*ui(i1+3,i2,i3+1,c3))+cs1*(cr0*
     & ui(i1,i2+1,i3+1,c3)+cr1*ui(i1+1,i2+1,i3+1,c3)+cr2*ui(i1+2,i2+1,
     & i3+1,c3)+cr3*ui(i1+3,i2+1,i3+1,c3))+cs2*(cr0*ui(i1,i2+2,i3+1,
     & c3)+cr1*ui(i1+1,i2+2,i3+1,c3)+cr2*ui(i1+2,i2+2,i3+1,c3)+cr3*ui(
     & i1+3,i2+2,i3+1,c3))+cs3*(cr0*ui(i1,i2+3,i3+1,c3)+cr1*ui(i1+1,
     & i2+3,i3+1,c3)+cr2*ui(i1+2,i2+3,i3+1,c3)+cr3*ui(i1+3,i2+3,i3+1,
     & c3)))+ct2*(cs0*(cr0*ui(i1,i2,i3+2,c3)+cr1*ui(i1+1,i2,i3+2,c3)+
     & cr2*ui(i1+2,i2,i3+2,c3)+cr3*ui(i1+3,i2,i3+2,c3))+cs1*(cr0*ui(
     & i1,i2+1,i3+2,c3)+cr1*ui(i1+1,i2+1,i3+2,c3)+cr2*ui(i1+2,i2+1,i3+
     & 2,c3)+cr3*ui(i1+3,i2+1,i3+2,c3))+cs2*(cr0*ui(i1,i2+2,i3+2,c3)+
     & cr1*ui(i1+1,i2+2,i3+2,c3)+cr2*ui(i1+2,i2+2,i3+2,c3)+cr3*ui(i1+
     & 3,i2+2,i3+2,c3))+cs3*(cr0*ui(i1,i2+3,i3+2,c3)+cr1*ui(i1+1,i2+3,
     & i3+2,c3)+cr2*ui(i1+2,i2+3,i3+2,c3)+cr3*ui(i1+3,i2+3,i3+2,c3)))+
     & ct3*(cs0*(cr0*ui(i1,i2,i3+3,c3)+cr1*ui(i1+1,i2,i3+3,c3)+cr2*ui(
     & i1+2,i2,i3+3,c3)+cr3*ui(i1+3,i2,i3+3,c3))+cs1*(cr0*ui(i1,i2+1,
     & i3+3,c3)+cr1*ui(i1+1,i2+1,i3+3,c3)+cr2*ui(i1+2,i2+1,i3+3,c3)+
     & cr3*ui(i1+3,i2+1,i3+3,c3))+cs2*(cr0*ui(i1,i2+2,i3+3,c3)+cr1*ui(
     & i1+1,i2+2,i3+3,c3)+cr2*ui(i1+2,i2+2,i3+3,c3)+cr3*ui(i1+3,i2+2,
     & i3+3,c3))+cs3*(cr0*ui(i1,i2+3,i3+3,c3)+cr1*ui(i1+1,i2+3,i3+3,
     & c3)+cr2*ui(i1+2,i2+3,i3+3,c3)+cr3*ui(i1+3,i2+3,i3+3,c3)))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           end if
         else if( width(1).eq.5 .and. width(2).eq.5 .and. width(3)
     & .eq.5 )then
! loops3d($interpSparseStorage555(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))
           if( c3a.eq.c3b )then
             do c3=c3a,c3b
             do i=nia,nib
! interpSparseStorage555(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q50(c(i,0,0,0))
                 cs0 = q50(c(i,1,0,0))
                 ct0 = q50(c(i,2,0,0))
                 cr1 = q51(c(i,0,0,0))
                 cs1 = q51(c(i,1,0,0))
                 ct1 = q51(c(i,2,0,0))
                 cr2 = q52(c(i,0,0,0))
                 cs2 = q52(c(i,1,0,0))
                 ct2 = q52(c(i,2,0,0))
                 cr3 = q53(c(i,0,0,0))
                 cs3 = q53(c(i,1,0,0))
                 ct3 = q53(c(i,2,0,0))
                 cr4 = q54(c(i,0,0,0))
                 cs4 = q54(c(i,1,0,0))
                 ct4 = q54(c(i,2,0,0))
               r(i) = ct0*(cs0*(cr0*ui(i1,i2,i3+0,c3)+cr1*ui(i1+1,i2,
     & i3+0,c3)+cr2*ui(i1+2,i2,i3+0,c3)+cr3*ui(i1+3,i2,i3+0,c3)+cr4*
     & ui(i1+4,i2,i3+0,c3))+cs1*(cr0*ui(i1,i2+1,i3+0,c3)+cr1*ui(i1+1,
     & i2+1,i3+0,c3)+cr2*ui(i1+2,i2+1,i3+0,c3)+cr3*ui(i1+3,i2+1,i3+0,
     & c3)+cr4*ui(i1+4,i2+1,i3+0,c3))+cs2*(cr0*ui(i1,i2+2,i3+0,c3)+
     & cr1*ui(i1+1,i2+2,i3+0,c3)+cr2*ui(i1+2,i2+2,i3+0,c3)+cr3*ui(i1+
     & 3,i2+2,i3+0,c3)+cr4*ui(i1+4,i2+2,i3+0,c3))+cs3*(cr0*ui(i1,i2+3,
     & i3+0,c3)+cr1*ui(i1+1,i2+3,i3+0,c3)+cr2*ui(i1+2,i2+3,i3+0,c3)+
     & cr3*ui(i1+3,i2+3,i3+0,c3)+cr4*ui(i1+4,i2+3,i3+0,c3))+cs4*(cr0*
     & ui(i1,i2+4,i3+0,c3)+cr1*ui(i1+1,i2+4,i3+0,c3)+cr2*ui(i1+2,i2+4,
     & i3+0,c3)+cr3*ui(i1+3,i2+4,i3+0,c3)+cr4*ui(i1+4,i2+4,i3+0,c3)))+
     & ct1*(cs0*(cr0*ui(i1,i2,i3+1,c3)+cr1*ui(i1+1,i2,i3+1,c3)+cr2*ui(
     & i1+2,i2,i3+1,c3)+cr3*ui(i1+3,i2,i3+1,c3)+cr4*ui(i1+4,i2,i3+1,
     & c3))+cs1*(cr0*ui(i1,i2+1,i3+1,c3)+cr1*ui(i1+1,i2+1,i3+1,c3)+
     & cr2*ui(i1+2,i2+1,i3+1,c3)+cr3*ui(i1+3,i2+1,i3+1,c3)+cr4*ui(i1+
     & 4,i2+1,i3+1,c3))+cs2*(cr0*ui(i1,i2+2,i3+1,c3)+cr1*ui(i1+1,i2+2,
     & i3+1,c3)+cr2*ui(i1+2,i2+2,i3+1,c3)+cr3*ui(i1+3,i2+2,i3+1,c3)+
     & cr4*ui(i1+4,i2+2,i3+1,c3))+cs3*(cr0*ui(i1,i2+3,i3+1,c3)+cr1*ui(
     & i1+1,i2+3,i3+1,c3)+cr2*ui(i1+2,i2+3,i3+1,c3)+cr3*ui(i1+3,i2+3,
     & i3+1,c3)+cr4*ui(i1+4,i2+3,i3+1,c3))+cs4*(cr0*ui(i1,i2+4,i3+1,
     & c3)+cr1*ui(i1+1,i2+4,i3+1,c3)+cr2*ui(i1+2,i2+4,i3+1,c3)+cr3*ui(
     & i1+3,i2+4,i3+1,c3)+cr4*ui(i1+4,i2+4,i3+1,c3)))+ct2*(cs0*(cr0*
     & ui(i1,i2,i3+2,c3)+cr1*ui(i1+1,i2,i3+2,c3)+cr2*ui(i1+2,i2,i3+2,
     & c3)+cr3*ui(i1+3,i2,i3+2,c3)+cr4*ui(i1+4,i2,i3+2,c3))+cs1*(cr0*
     & ui(i1,i2+1,i3+2,c3)+cr1*ui(i1+1,i2+1,i3+2,c3)+cr2*ui(i1+2,i2+1,
     & i3+2,c3)+cr3*ui(i1+3,i2+1,i3+2,c3)+cr4*ui(i1+4,i2+1,i3+2,c3))+
     & cs2*(cr0*ui(i1,i2+2,i3+2,c3)+cr1*ui(i1+1,i2+2,i3+2,c3)+cr2*ui(
     & i1+2,i2+2,i3+2,c3)+cr3*ui(i1+3,i2+2,i3+2,c3)+cr4*ui(i1+4,i2+2,
     & i3+2,c3))+cs3*(cr0*ui(i1,i2+3,i3+2,c3)+cr1*ui(i1+1,i2+3,i3+2,
     & c3)+cr2*ui(i1+2,i2+3,i3+2,c3)+cr3*ui(i1+3,i2+3,i3+2,c3)+cr4*ui(
     & i1+4,i2+3,i3+2,c3))+cs4*(cr0*ui(i1,i2+4,i3+2,c3)+cr1*ui(i1+1,
     & i2+4,i3+2,c3)+cr2*ui(i1+2,i2+4,i3+2,c3)+cr3*ui(i1+3,i2+4,i3+2,
     & c3)+cr4*ui(i1+4,i2+4,i3+2,c3)))+ct3*(cs0*(cr0*ui(i1,i2,i3+3,c3)
     & +cr1*ui(i1+1,i2,i3+3,c3)+cr2*ui(i1+2,i2,i3+3,c3)+cr3*ui(i1+3,
     & i2,i3+3,c3)+cr4*ui(i1+4,i2,i3+3,c3))+cs1*(cr0*ui(i1,i2+1,i3+3,
     & c3)+cr1*ui(i1+1,i2+1,i3+3,c3)+cr2*ui(i1+2,i2+1,i3+3,c3)+cr3*ui(
     & i1+3,i2+1,i3+3,c3)+cr4*ui(i1+4,i2+1,i3+3,c3))+cs2*(cr0*ui(i1,
     & i2+2,i3+3,c3)+cr1*ui(i1+1,i2+2,i3+3,c3)+cr2*ui(i1+2,i2+2,i3+3,
     & c3)+cr3*ui(i1+3,i2+2,i3+3,c3)+cr4*ui(i1+4,i2+2,i3+3,c3))+cs3*(
     & cr0*ui(i1,i2+3,i3+3,c3)+cr1*ui(i1+1,i2+3,i3+3,c3)+cr2*ui(i1+2,
     & i2+3,i3+3,c3)+cr3*ui(i1+3,i2+3,i3+3,c3)+cr4*ui(i1+4,i2+3,i3+3,
     & c3))+cs4*(cr0*ui(i1,i2+4,i3+3,c3)+cr1*ui(i1+1,i2+4,i3+3,c3)+
     & cr2*ui(i1+2,i2+4,i3+3,c3)+cr3*ui(i1+3,i2+4,i3+3,c3)+cr4*ui(i1+
     & 4,i2+4,i3+3,c3)))+ct4*(cs0*(cr0*ui(i1,i2,i3+4,c3)+cr1*ui(i1+1,
     & i2,i3+4,c3)+cr2*ui(i1+2,i2,i3+4,c3)+cr3*ui(i1+3,i2,i3+4,c3)+
     & cr4*ui(i1+4,i2,i3+4,c3))+cs1*(cr0*ui(i1,i2+1,i3+4,c3)+cr1*ui(
     & i1+1,i2+1,i3+4,c3)+cr2*ui(i1+2,i2+1,i3+4,c3)+cr3*ui(i1+3,i2+1,
     & i3+4,c3)+cr4*ui(i1+4,i2+1,i3+4,c3))+cs2*(cr0*ui(i1,i2+2,i3+4,
     & c3)+cr1*ui(i1+1,i2+2,i3+4,c3)+cr2*ui(i1+2,i2+2,i3+4,c3)+cr3*ui(
     & i1+3,i2+2,i3+4,c3)+cr4*ui(i1+4,i2+2,i3+4,c3))+cs3*(cr0*ui(i1,
     & i2+3,i3+4,c3)+cr1*ui(i1+1,i2+3,i3+4,c3)+cr2*ui(i1+2,i2+3,i3+4,
     & c3)+cr3*ui(i1+3,i2+3,i3+4,c3)+cr4*ui(i1+4,i2+3,i3+4,c3))+cs4*(
     & cr0*ui(i1,i2+4,i3+4,c3)+cr1*ui(i1+1,i2+4,i3+4,c3)+cr2*ui(i1+2,
     & i2+4,i3+4,c3)+cr3*ui(i1+3,i2+4,i3+4,c3)+cr4*ui(i1+4,i2+4,i3+4,
     & c3)))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
! interpSparseStorage555(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q50(c(i,0,0,0))
                 cs0 = q50(c(i,1,0,0))
                 ct0 = q50(c(i,2,0,0))
                 cr1 = q51(c(i,0,0,0))
                 cs1 = q51(c(i,1,0,0))
                 ct1 = q51(c(i,2,0,0))
                 cr2 = q52(c(i,0,0,0))
                 cs2 = q52(c(i,1,0,0))
                 ct2 = q52(c(i,2,0,0))
                 cr3 = q53(c(i,0,0,0))
                 cs3 = q53(c(i,1,0,0))
                 ct3 = q53(c(i,2,0,0))
                 cr4 = q54(c(i,0,0,0))
                 cs4 = q54(c(i,1,0,0))
                 ct4 = q54(c(i,2,0,0))
               r(i) = ct0*(cs0*(cr0*ui(i1,i2,i3+0,c3)+cr1*ui(i1+1,i2,
     & i3+0,c3)+cr2*ui(i1+2,i2,i3+0,c3)+cr3*ui(i1+3,i2,i3+0,c3)+cr4*
     & ui(i1+4,i2,i3+0,c3))+cs1*(cr0*ui(i1,i2+1,i3+0,c3)+cr1*ui(i1+1,
     & i2+1,i3+0,c3)+cr2*ui(i1+2,i2+1,i3+0,c3)+cr3*ui(i1+3,i2+1,i3+0,
     & c3)+cr4*ui(i1+4,i2+1,i3+0,c3))+cs2*(cr0*ui(i1,i2+2,i3+0,c3)+
     & cr1*ui(i1+1,i2+2,i3+0,c3)+cr2*ui(i1+2,i2+2,i3+0,c3)+cr3*ui(i1+
     & 3,i2+2,i3+0,c3)+cr4*ui(i1+4,i2+2,i3+0,c3))+cs3*(cr0*ui(i1,i2+3,
     & i3+0,c3)+cr1*ui(i1+1,i2+3,i3+0,c3)+cr2*ui(i1+2,i2+3,i3+0,c3)+
     & cr3*ui(i1+3,i2+3,i3+0,c3)+cr4*ui(i1+4,i2+3,i3+0,c3))+cs4*(cr0*
     & ui(i1,i2+4,i3+0,c3)+cr1*ui(i1+1,i2+4,i3+0,c3)+cr2*ui(i1+2,i2+4,
     & i3+0,c3)+cr3*ui(i1+3,i2+4,i3+0,c3)+cr4*ui(i1+4,i2+4,i3+0,c3)))+
     & ct1*(cs0*(cr0*ui(i1,i2,i3+1,c3)+cr1*ui(i1+1,i2,i3+1,c3)+cr2*ui(
     & i1+2,i2,i3+1,c3)+cr3*ui(i1+3,i2,i3+1,c3)+cr4*ui(i1+4,i2,i3+1,
     & c3))+cs1*(cr0*ui(i1,i2+1,i3+1,c3)+cr1*ui(i1+1,i2+1,i3+1,c3)+
     & cr2*ui(i1+2,i2+1,i3+1,c3)+cr3*ui(i1+3,i2+1,i3+1,c3)+cr4*ui(i1+
     & 4,i2+1,i3+1,c3))+cs2*(cr0*ui(i1,i2+2,i3+1,c3)+cr1*ui(i1+1,i2+2,
     & i3+1,c3)+cr2*ui(i1+2,i2+2,i3+1,c3)+cr3*ui(i1+3,i2+2,i3+1,c3)+
     & cr4*ui(i1+4,i2+2,i3+1,c3))+cs3*(cr0*ui(i1,i2+3,i3+1,c3)+cr1*ui(
     & i1+1,i2+3,i3+1,c3)+cr2*ui(i1+2,i2+3,i3+1,c3)+cr3*ui(i1+3,i2+3,
     & i3+1,c3)+cr4*ui(i1+4,i2+3,i3+1,c3))+cs4*(cr0*ui(i1,i2+4,i3+1,
     & c3)+cr1*ui(i1+1,i2+4,i3+1,c3)+cr2*ui(i1+2,i2+4,i3+1,c3)+cr3*ui(
     & i1+3,i2+4,i3+1,c3)+cr4*ui(i1+4,i2+4,i3+1,c3)))+ct2*(cs0*(cr0*
     & ui(i1,i2,i3+2,c3)+cr1*ui(i1+1,i2,i3+2,c3)+cr2*ui(i1+2,i2,i3+2,
     & c3)+cr3*ui(i1+3,i2,i3+2,c3)+cr4*ui(i1+4,i2,i3+2,c3))+cs1*(cr0*
     & ui(i1,i2+1,i3+2,c3)+cr1*ui(i1+1,i2+1,i3+2,c3)+cr2*ui(i1+2,i2+1,
     & i3+2,c3)+cr3*ui(i1+3,i2+1,i3+2,c3)+cr4*ui(i1+4,i2+1,i3+2,c3))+
     & cs2*(cr0*ui(i1,i2+2,i3+2,c3)+cr1*ui(i1+1,i2+2,i3+2,c3)+cr2*ui(
     & i1+2,i2+2,i3+2,c3)+cr3*ui(i1+3,i2+2,i3+2,c3)+cr4*ui(i1+4,i2+2,
     & i3+2,c3))+cs3*(cr0*ui(i1,i2+3,i3+2,c3)+cr1*ui(i1+1,i2+3,i3+2,
     & c3)+cr2*ui(i1+2,i2+3,i3+2,c3)+cr3*ui(i1+3,i2+3,i3+2,c3)+cr4*ui(
     & i1+4,i2+3,i3+2,c3))+cs4*(cr0*ui(i1,i2+4,i3+2,c3)+cr1*ui(i1+1,
     & i2+4,i3+2,c3)+cr2*ui(i1+2,i2+4,i3+2,c3)+cr3*ui(i1+3,i2+4,i3+2,
     & c3)+cr4*ui(i1+4,i2+4,i3+2,c3)))+ct3*(cs0*(cr0*ui(i1,i2,i3+3,c3)
     & +cr1*ui(i1+1,i2,i3+3,c3)+cr2*ui(i1+2,i2,i3+3,c3)+cr3*ui(i1+3,
     & i2,i3+3,c3)+cr4*ui(i1+4,i2,i3+3,c3))+cs1*(cr0*ui(i1,i2+1,i3+3,
     & c3)+cr1*ui(i1+1,i2+1,i3+3,c3)+cr2*ui(i1+2,i2+1,i3+3,c3)+cr3*ui(
     & i1+3,i2+1,i3+3,c3)+cr4*ui(i1+4,i2+1,i3+3,c3))+cs2*(cr0*ui(i1,
     & i2+2,i3+3,c3)+cr1*ui(i1+1,i2+2,i3+3,c3)+cr2*ui(i1+2,i2+2,i3+3,
     & c3)+cr3*ui(i1+3,i2+2,i3+3,c3)+cr4*ui(i1+4,i2+2,i3+3,c3))+cs3*(
     & cr0*ui(i1,i2+3,i3+3,c3)+cr1*ui(i1+1,i2+3,i3+3,c3)+cr2*ui(i1+2,
     & i2+3,i3+3,c3)+cr3*ui(i1+3,i2+3,i3+3,c3)+cr4*ui(i1+4,i2+3,i3+3,
     & c3))+cs4*(cr0*ui(i1,i2+4,i3+3,c3)+cr1*ui(i1+1,i2+4,i3+3,c3)+
     & cr2*ui(i1+2,i2+4,i3+3,c3)+cr3*ui(i1+3,i2+4,i3+3,c3)+cr4*ui(i1+
     & 4,i2+4,i3+3,c3)))+ct4*(cs0*(cr0*ui(i1,i2,i3+4,c3)+cr1*ui(i1+1,
     & i2,i3+4,c3)+cr2*ui(i1+2,i2,i3+4,c3)+cr3*ui(i1+3,i2,i3+4,c3)+
     & cr4*ui(i1+4,i2,i3+4,c3))+cs1*(cr0*ui(i1,i2+1,i3+4,c3)+cr1*ui(
     & i1+1,i2+1,i3+4,c3)+cr2*ui(i1+2,i2+1,i3+4,c3)+cr3*ui(i1+3,i2+1,
     & i3+4,c3)+cr4*ui(i1+4,i2+1,i3+4,c3))+cs2*(cr0*ui(i1,i2+2,i3+4,
     & c3)+cr1*ui(i1+1,i2+2,i3+4,c3)+cr2*ui(i1+2,i2+2,i3+4,c3)+cr3*ui(
     & i1+3,i2+2,i3+4,c3)+cr4*ui(i1+4,i2+2,i3+4,c3))+cs3*(cr0*ui(i1,
     & i2+3,i3+4,c3)+cr1*ui(i1+1,i2+3,i3+4,c3)+cr2*ui(i1+2,i2+3,i3+4,
     & c3)+cr3*ui(i1+3,i2+3,i3+4,c3)+cr4*ui(i1+4,i2+3,i3+4,c3))+cs4*(
     & cr0*ui(i1,i2+4,i3+4,c3)+cr1*ui(i1+1,i2+4,i3+4,c3)+cr2*ui(i1+2,
     & i2+4,i3+4,c3)+cr3*ui(i1+3,i2+4,i3+4,c3)+cr4*ui(i1+4,i2+4,i3+4,
     & c3)))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           end if
         else if( width(1).eq.6 .and. width(2).eq.6 .and. width(3)
     & .eq.6 )then
! loops3d($interpSparseStorage666(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))
           if( c3a.eq.c3b )then
             do c3=c3a,c3b
             do i=nia,nib
! interpSparseStorage666(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q60(c(i,0,0,0))
                 cs0 = q60(c(i,1,0,0))
                 ct0 = q60(c(i,2,0,0))
                 cr1 = q61(c(i,0,0,0))
                 cs1 = q61(c(i,1,0,0))
                 ct1 = q61(c(i,2,0,0))
                 cr2 = q62(c(i,0,0,0))
                 cs2 = q62(c(i,1,0,0))
                 ct2 = q62(c(i,2,0,0))
                 cr3 = q63(c(i,0,0,0))
                 cs3 = q63(c(i,1,0,0))
                 ct3 = q63(c(i,2,0,0))
                 cr4 = q64(c(i,0,0,0))
                 cs4 = q64(c(i,1,0,0))
                 ct4 = q64(c(i,2,0,0))
                 cr5 = q65(c(i,0,0,0))
                 cs5 = q65(c(i,1,0,0))
                 ct5 = q65(c(i,2,0,0))
               r(i) = ct0*(cs0*(cr0*ui(i1,i2,i3+0,c3)+cr1*ui(i1+1,i2,
     & i3+0,c3)+cr2*ui(i1+2,i2,i3+0,c3)+cr3*ui(i1+3,i2,i3+0,c3)+cr4*
     & ui(i1+4,i2,i3+0,c3)+cr5*ui(i1+5,i2,i3+0,c3))+cs1*(cr0*ui(i1,i2+
     & 1,i3+0,c3)+cr1*ui(i1+1,i2+1,i3+0,c3)+cr2*ui(i1+2,i2+1,i3+0,c3)+
     & cr3*ui(i1+3,i2+1,i3+0,c3)+cr4*ui(i1+4,i2+1,i3+0,c3)+cr5*ui(i1+
     & 5,i2+1,i3+0,c3))+cs2*(cr0*ui(i1,i2+2,i3+0,c3)+cr1*ui(i1+1,i2+2,
     & i3+0,c3)+cr2*ui(i1+2,i2+2,i3+0,c3)+cr3*ui(i1+3,i2+2,i3+0,c3)+
     & cr4*ui(i1+4,i2+2,i3+0,c3)+cr5*ui(i1+5,i2+2,i3+0,c3))+cs3*(cr0*
     & ui(i1,i2+3,i3+0,c3)+cr1*ui(i1+1,i2+3,i3+0,c3)+cr2*ui(i1+2,i2+3,
     & i3+0,c3)+cr3*ui(i1+3,i2+3,i3+0,c3)+cr4*ui(i1+4,i2+3,i3+0,c3)+
     & cr5*ui(i1+5,i2+3,i3+0,c3))+cs4*(cr0*ui(i1,i2+4,i3+0,c3)+cr1*ui(
     & i1+1,i2+4,i3+0,c3)+cr2*ui(i1+2,i2+4,i3+0,c3)+cr3*ui(i1+3,i2+4,
     & i3+0,c3)+cr4*ui(i1+4,i2+4,i3+0,c3)+cr5*ui(i1+5,i2+4,i3+0,c3))+
     & cs5*(cr0*ui(i1,i2+5,i3+0,c3)+cr1*ui(i1+1,i2+5,i3+0,c3)+cr2*ui(
     & i1+2,i2+5,i3+0,c3)+cr3*ui(i1+3,i2+5,i3+0,c3)+cr4*ui(i1+4,i2+5,
     & i3+0,c3)+cr5*ui(i1+5,i2+5,i3+0,c3)))+ct1*(cs0*(cr0*ui(i1,i2,i3+
     & 1,c3)+cr1*ui(i1+1,i2,i3+1,c3)+cr2*ui(i1+2,i2,i3+1,c3)+cr3*ui(
     & i1+3,i2,i3+1,c3)+cr4*ui(i1+4,i2,i3+1,c3)+cr5*ui(i1+5,i2,i3+1,
     & c3))+cs1*(cr0*ui(i1,i2+1,i3+1,c3)+cr1*ui(i1+1,i2+1,i3+1,c3)+
     & cr2*ui(i1+2,i2+1,i3+1,c3)+cr3*ui(i1+3,i2+1,i3+1,c3)+cr4*ui(i1+
     & 4,i2+1,i3+1,c3)+cr5*ui(i1+5,i2+1,i3+1,c3))+cs2*(cr0*ui(i1,i2+2,
     & i3+1,c3)+cr1*ui(i1+1,i2+2,i3+1,c3)+cr2*ui(i1+2,i2+2,i3+1,c3)+
     & cr3*ui(i1+3,i2+2,i3+1,c3)+cr4*ui(i1+4,i2+2,i3+1,c3)+cr5*ui(i1+
     & 5,i2+2,i3+1,c3))+cs3*(cr0*ui(i1,i2+3,i3+1,c3)+cr1*ui(i1+1,i2+3,
     & i3+1,c3)+cr2*ui(i1+2,i2+3,i3+1,c3)+cr3*ui(i1+3,i2+3,i3+1,c3)+
     & cr4*ui(i1+4,i2+3,i3+1,c3)+cr5*ui(i1+5,i2+3,i3+1,c3))+cs4*(cr0*
     & ui(i1,i2+4,i3+1,c3)+cr1*ui(i1+1,i2+4,i3+1,c3)+cr2*ui(i1+2,i2+4,
     & i3+1,c3)+cr3*ui(i1+3,i2+4,i3+1,c3)+cr4*ui(i1+4,i2+4,i3+1,c3)+
     & cr5*ui(i1+5,i2+4,i3+1,c3))+cs5*(cr0*ui(i1,i2+5,i3+1,c3)+cr1*ui(
     & i1+1,i2+5,i3+1,c3)+cr2*ui(i1+2,i2+5,i3+1,c3)+cr3*ui(i1+3,i2+5,
     & i3+1,c3)+cr4*ui(i1+4,i2+5,i3+1,c3)+cr5*ui(i1+5,i2+5,i3+1,c3)))+
     & ct2*(cs0*(cr0*ui(i1,i2,i3+2,c3)+cr1*ui(i1+1,i2,i3+2,c3)+cr2*ui(
     & i1+2,i2,i3+2,c3)+cr3*ui(i1+3,i2,i3+2,c3)+cr4*ui(i1+4,i2,i3+2,
     & c3)+cr5*ui(i1+5,i2,i3+2,c3))+cs1*(cr0*ui(i1,i2+1,i3+2,c3)+cr1*
     & ui(i1+1,i2+1,i3+2,c3)+cr2*ui(i1+2,i2+1,i3+2,c3)+cr3*ui(i1+3,i2+
     & 1,i3+2,c3)+cr4*ui(i1+4,i2+1,i3+2,c3)+cr5*ui(i1+5,i2+1,i3+2,c3))
     & +cs2*(cr0*ui(i1,i2+2,i3+2,c3)+cr1*ui(i1+1,i2+2,i3+2,c3)+cr2*ui(
     & i1+2,i2+2,i3+2,c3)+cr3*ui(i1+3,i2+2,i3+2,c3)+cr4*ui(i1+4,i2+2,
     & i3+2,c3)+cr5*ui(i1+5,i2+2,i3+2,c3))+cs3*(cr0*ui(i1,i2+3,i3+2,
     & c3)+cr1*ui(i1+1,i2+3,i3+2,c3)+cr2*ui(i1+2,i2+3,i3+2,c3)+cr3*ui(
     & i1+3,i2+3,i3+2,c3)+cr4*ui(i1+4,i2+3,i3+2,c3)+cr5*ui(i1+5,i2+3,
     & i3+2,c3))+cs4*(cr0*ui(i1,i2+4,i3+2,c3)+cr1*ui(i1+1,i2+4,i3+2,
     & c3)+cr2*ui(i1+2,i2+4,i3+2,c3)+cr3*ui(i1+3,i2+4,i3+2,c3)+cr4*ui(
     & i1+4,i2+4,i3+2,c3)+cr5*ui(i1+5,i2+4,i3+2,c3))+cs5*(cr0*ui(i1,
     & i2+5,i3+2,c3)+cr1*ui(i1+1,i2+5,i3+2,c3)+cr2*ui(i1+2,i2+5,i3+2,
     & c3)+cr3*ui(i1+3,i2+5,i3+2,c3)+cr4*ui(i1+4,i2+5,i3+2,c3)+cr5*ui(
     & i1+5,i2+5,i3+2,c3)))+ct3*(cs0*(cr0*ui(i1,i2,i3+3,c3)+cr1*ui(i1+
     & 1,i2,i3+3,c3)+cr2*ui(i1+2,i2,i3+3,c3)+cr3*ui(i1+3,i2,i3+3,c3)+
     & cr4*ui(i1+4,i2,i3+3,c3)+cr5*ui(i1+5,i2,i3+3,c3))+cs1*(cr0*ui(
     & i1,i2+1,i3+3,c3)+cr1*ui(i1+1,i2+1,i3+3,c3)+cr2*ui(i1+2,i2+1,i3+
     & 3,c3)+cr3*ui(i1+3,i2+1,i3+3,c3)+cr4*ui(i1+4,i2+1,i3+3,c3)+cr5*
     & ui(i1+5,i2+1,i3+3,c3))+cs2*(cr0*ui(i1,i2+2,i3+3,c3)+cr1*ui(i1+
     & 1,i2+2,i3+3,c3)+cr2*ui(i1+2,i2+2,i3+3,c3)+cr3*ui(i1+3,i2+2,i3+
     & 3,c3)+cr4*ui(i1+4,i2+2,i3+3,c3)+cr5*ui(i1+5,i2+2,i3+3,c3))+cs3*
     & (cr0*ui(i1,i2+3,i3+3,c3)+cr1*ui(i1+1,i2+3,i3+3,c3)+cr2*ui(i1+2,
     & i2+3,i3+3,c3)+cr3*ui(i1+3,i2+3,i3+3,c3)+cr4*ui(i1+4,i2+3,i3+3,
     & c3)+cr5*ui(i1+5,i2+3,i3+3,c3))+cs4*(cr0*ui(i1,i2+4,i3+3,c3)+
     & cr1*ui(i1+1,i2+4,i3+3,c3)+cr2*ui(i1+2,i2+4,i3+3,c3)+cr3*ui(i1+
     & 3,i2+4,i3+3,c3)+cr4*ui(i1+4,i2+4,i3+3,c3)+cr5*ui(i1+5,i2+4,i3+
     & 3,c3))+cs5*(cr0*ui(i1,i2+5,i3+3,c3)+cr1*ui(i1+1,i2+5,i3+3,c3)+
     & cr2*ui(i1+2,i2+5,i3+3,c3)+cr3*ui(i1+3,i2+5,i3+3,c3)+cr4*ui(i1+
     & 4,i2+5,i3+3,c3)+cr5*ui(i1+5,i2+5,i3+3,c3)))+ct4*(cs0*(cr0*ui(
     & i1,i2,i3+4,c3)+cr1*ui(i1+1,i2,i3+4,c3)+cr2*ui(i1+2,i2,i3+4,c3)+
     & cr3*ui(i1+3,i2,i3+4,c3)+cr4*ui(i1+4,i2,i3+4,c3)+cr5*ui(i1+5,i2,
     & i3+4,c3))+cs1*(cr0*ui(i1,i2+1,i3+4,c3)+cr1*ui(i1+1,i2+1,i3+4,
     & c3)+cr2*ui(i1+2,i2+1,i3+4,c3)+cr3*ui(i1+3,i2+1,i3+4,c3)+cr4*ui(
     & i1+4,i2+1,i3+4,c3)+cr5*ui(i1+5,i2+1,i3+4,c3))+cs2*(cr0*ui(i1,
     & i2+2,i3+4,c3)+cr1*ui(i1+1,i2+2,i3+4,c3)+cr2*ui(i1+2,i2+2,i3+4,
     & c3)+cr3*ui(i1+3,i2+2,i3+4,c3)+cr4*ui(i1+4,i2+2,i3+4,c3)+cr5*ui(
     & i1+5,i2+2,i3+4,c3))+cs3*(cr0*ui(i1,i2+3,i3+4,c3)+cr1*ui(i1+1,
     & i2+3,i3+4,c3)+cr2*ui(i1+2,i2+3,i3+4,c3)+cr3*ui(i1+3,i2+3,i3+4,
     & c3)+cr4*ui(i1+4,i2+3,i3+4,c3)+cr5*ui(i1+5,i2+3,i3+4,c3))+cs4*(
     & cr0*ui(i1,i2+4,i3+4,c3)+cr1*ui(i1+1,i2+4,i3+4,c3)+cr2*ui(i1+2,
     & i2+4,i3+4,c3)+cr3*ui(i1+3,i2+4,i3+4,c3)+cr4*ui(i1+4,i2+4,i3+4,
     & c3)+cr5*ui(i1+5,i2+4,i3+4,c3))+cs5*(cr0*ui(i1,i2+5,i3+4,c3)+
     & cr1*ui(i1+1,i2+5,i3+4,c3)+cr2*ui(i1+2,i2+5,i3+4,c3)+cr3*ui(i1+
     & 3,i2+5,i3+4,c3)+cr4*ui(i1+4,i2+5,i3+4,c3)+cr5*ui(i1+5,i2+5,i3+
     & 4,c3)))+ct5*(cs0*(cr0*ui(i1,i2,i3+5,c3)+cr1*ui(i1+1,i2,i3+5,c3)
     & +cr2*ui(i1+2,i2,i3+5,c3)+cr3*ui(i1+3,i2,i3+5,c3)+cr4*ui(i1+4,
     & i2,i3+5,c3)+cr5*ui(i1+5,i2,i3+5,c3))+cs1*(cr0*ui(i1,i2+1,i3+5,
     & c3)+cr1*ui(i1+1,i2+1,i3+5,c3)+cr2*ui(i1+2,i2+1,i3+5,c3)+cr3*ui(
     & i1+3,i2+1,i3+5,c3)+cr4*ui(i1+4,i2+1,i3+5,c3)+cr5*ui(i1+5,i2+1,
     & i3+5,c3))+cs2*(cr0*ui(i1,i2+2,i3+5,c3)+cr1*ui(i1+1,i2+2,i3+5,
     & c3)+cr2*ui(i1+2,i2+2,i3+5,c3)+cr3*ui(i1+3,i2+2,i3+5,c3)+cr4*ui(
     & i1+4,i2+2,i3+5,c3)+cr5*ui(i1+5,i2+2,i3+5,c3))+cs3*(cr0*ui(i1,
     & i2+3,i3+5,c3)+cr1*ui(i1+1,i2+3,i3+5,c3)+cr2*ui(i1+2,i2+3,i3+5,
     & c3)+cr3*ui(i1+3,i2+3,i3+5,c3)+cr4*ui(i1+4,i2+3,i3+5,c3)+cr5*ui(
     & i1+5,i2+3,i3+5,c3))+cs4*(cr0*ui(i1,i2+4,i3+5,c3)+cr1*ui(i1+1,
     & i2+4,i3+5,c3)+cr2*ui(i1+2,i2+4,i3+5,c3)+cr3*ui(i1+3,i2+4,i3+5,
     & c3)+cr4*ui(i1+4,i2+4,i3+5,c3)+cr5*ui(i1+5,i2+4,i3+5,c3))+cs5*(
     & cr0*ui(i1,i2+5,i3+5,c3)+cr1*ui(i1+1,i2+5,i3+5,c3)+cr2*ui(i1+2,
     & i2+5,i3+5,c3)+cr3*ui(i1+3,i2+5,i3+5,c3)+cr4*ui(i1+4,i2+5,i3+5,
     & c3)+cr5*ui(i1+5,i2+5,i3+5,c3)))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
! interpSparseStorage666(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q60(c(i,0,0,0))
                 cs0 = q60(c(i,1,0,0))
                 ct0 = q60(c(i,2,0,0))
                 cr1 = q61(c(i,0,0,0))
                 cs1 = q61(c(i,1,0,0))
                 ct1 = q61(c(i,2,0,0))
                 cr2 = q62(c(i,0,0,0))
                 cs2 = q62(c(i,1,0,0))
                 ct2 = q62(c(i,2,0,0))
                 cr3 = q63(c(i,0,0,0))
                 cs3 = q63(c(i,1,0,0))
                 ct3 = q63(c(i,2,0,0))
                 cr4 = q64(c(i,0,0,0))
                 cs4 = q64(c(i,1,0,0))
                 ct4 = q64(c(i,2,0,0))
                 cr5 = q65(c(i,0,0,0))
                 cs5 = q65(c(i,1,0,0))
                 ct5 = q65(c(i,2,0,0))
               r(i) = ct0*(cs0*(cr0*ui(i1,i2,i3+0,c3)+cr1*ui(i1+1,i2,
     & i3+0,c3)+cr2*ui(i1+2,i2,i3+0,c3)+cr3*ui(i1+3,i2,i3+0,c3)+cr4*
     & ui(i1+4,i2,i3+0,c3)+cr5*ui(i1+5,i2,i3+0,c3))+cs1*(cr0*ui(i1,i2+
     & 1,i3+0,c3)+cr1*ui(i1+1,i2+1,i3+0,c3)+cr2*ui(i1+2,i2+1,i3+0,c3)+
     & cr3*ui(i1+3,i2+1,i3+0,c3)+cr4*ui(i1+4,i2+1,i3+0,c3)+cr5*ui(i1+
     & 5,i2+1,i3+0,c3))+cs2*(cr0*ui(i1,i2+2,i3+0,c3)+cr1*ui(i1+1,i2+2,
     & i3+0,c3)+cr2*ui(i1+2,i2+2,i3+0,c3)+cr3*ui(i1+3,i2+2,i3+0,c3)+
     & cr4*ui(i1+4,i2+2,i3+0,c3)+cr5*ui(i1+5,i2+2,i3+0,c3))+cs3*(cr0*
     & ui(i1,i2+3,i3+0,c3)+cr1*ui(i1+1,i2+3,i3+0,c3)+cr2*ui(i1+2,i2+3,
     & i3+0,c3)+cr3*ui(i1+3,i2+3,i3+0,c3)+cr4*ui(i1+4,i2+3,i3+0,c3)+
     & cr5*ui(i1+5,i2+3,i3+0,c3))+cs4*(cr0*ui(i1,i2+4,i3+0,c3)+cr1*ui(
     & i1+1,i2+4,i3+0,c3)+cr2*ui(i1+2,i2+4,i3+0,c3)+cr3*ui(i1+3,i2+4,
     & i3+0,c3)+cr4*ui(i1+4,i2+4,i3+0,c3)+cr5*ui(i1+5,i2+4,i3+0,c3))+
     & cs5*(cr0*ui(i1,i2+5,i3+0,c3)+cr1*ui(i1+1,i2+5,i3+0,c3)+cr2*ui(
     & i1+2,i2+5,i3+0,c3)+cr3*ui(i1+3,i2+5,i3+0,c3)+cr4*ui(i1+4,i2+5,
     & i3+0,c3)+cr5*ui(i1+5,i2+5,i3+0,c3)))+ct1*(cs0*(cr0*ui(i1,i2,i3+
     & 1,c3)+cr1*ui(i1+1,i2,i3+1,c3)+cr2*ui(i1+2,i2,i3+1,c3)+cr3*ui(
     & i1+3,i2,i3+1,c3)+cr4*ui(i1+4,i2,i3+1,c3)+cr5*ui(i1+5,i2,i3+1,
     & c3))+cs1*(cr0*ui(i1,i2+1,i3+1,c3)+cr1*ui(i1+1,i2+1,i3+1,c3)+
     & cr2*ui(i1+2,i2+1,i3+1,c3)+cr3*ui(i1+3,i2+1,i3+1,c3)+cr4*ui(i1+
     & 4,i2+1,i3+1,c3)+cr5*ui(i1+5,i2+1,i3+1,c3))+cs2*(cr0*ui(i1,i2+2,
     & i3+1,c3)+cr1*ui(i1+1,i2+2,i3+1,c3)+cr2*ui(i1+2,i2+2,i3+1,c3)+
     & cr3*ui(i1+3,i2+2,i3+1,c3)+cr4*ui(i1+4,i2+2,i3+1,c3)+cr5*ui(i1+
     & 5,i2+2,i3+1,c3))+cs3*(cr0*ui(i1,i2+3,i3+1,c3)+cr1*ui(i1+1,i2+3,
     & i3+1,c3)+cr2*ui(i1+2,i2+3,i3+1,c3)+cr3*ui(i1+3,i2+3,i3+1,c3)+
     & cr4*ui(i1+4,i2+3,i3+1,c3)+cr5*ui(i1+5,i2+3,i3+1,c3))+cs4*(cr0*
     & ui(i1,i2+4,i3+1,c3)+cr1*ui(i1+1,i2+4,i3+1,c3)+cr2*ui(i1+2,i2+4,
     & i3+1,c3)+cr3*ui(i1+3,i2+4,i3+1,c3)+cr4*ui(i1+4,i2+4,i3+1,c3)+
     & cr5*ui(i1+5,i2+4,i3+1,c3))+cs5*(cr0*ui(i1,i2+5,i3+1,c3)+cr1*ui(
     & i1+1,i2+5,i3+1,c3)+cr2*ui(i1+2,i2+5,i3+1,c3)+cr3*ui(i1+3,i2+5,
     & i3+1,c3)+cr4*ui(i1+4,i2+5,i3+1,c3)+cr5*ui(i1+5,i2+5,i3+1,c3)))+
     & ct2*(cs0*(cr0*ui(i1,i2,i3+2,c3)+cr1*ui(i1+1,i2,i3+2,c3)+cr2*ui(
     & i1+2,i2,i3+2,c3)+cr3*ui(i1+3,i2,i3+2,c3)+cr4*ui(i1+4,i2,i3+2,
     & c3)+cr5*ui(i1+5,i2,i3+2,c3))+cs1*(cr0*ui(i1,i2+1,i3+2,c3)+cr1*
     & ui(i1+1,i2+1,i3+2,c3)+cr2*ui(i1+2,i2+1,i3+2,c3)+cr3*ui(i1+3,i2+
     & 1,i3+2,c3)+cr4*ui(i1+4,i2+1,i3+2,c3)+cr5*ui(i1+5,i2+1,i3+2,c3))
     & +cs2*(cr0*ui(i1,i2+2,i3+2,c3)+cr1*ui(i1+1,i2+2,i3+2,c3)+cr2*ui(
     & i1+2,i2+2,i3+2,c3)+cr3*ui(i1+3,i2+2,i3+2,c3)+cr4*ui(i1+4,i2+2,
     & i3+2,c3)+cr5*ui(i1+5,i2+2,i3+2,c3))+cs3*(cr0*ui(i1,i2+3,i3+2,
     & c3)+cr1*ui(i1+1,i2+3,i3+2,c3)+cr2*ui(i1+2,i2+3,i3+2,c3)+cr3*ui(
     & i1+3,i2+3,i3+2,c3)+cr4*ui(i1+4,i2+3,i3+2,c3)+cr5*ui(i1+5,i2+3,
     & i3+2,c3))+cs4*(cr0*ui(i1,i2+4,i3+2,c3)+cr1*ui(i1+1,i2+4,i3+2,
     & c3)+cr2*ui(i1+2,i2+4,i3+2,c3)+cr3*ui(i1+3,i2+4,i3+2,c3)+cr4*ui(
     & i1+4,i2+4,i3+2,c3)+cr5*ui(i1+5,i2+4,i3+2,c3))+cs5*(cr0*ui(i1,
     & i2+5,i3+2,c3)+cr1*ui(i1+1,i2+5,i3+2,c3)+cr2*ui(i1+2,i2+5,i3+2,
     & c3)+cr3*ui(i1+3,i2+5,i3+2,c3)+cr4*ui(i1+4,i2+5,i3+2,c3)+cr5*ui(
     & i1+5,i2+5,i3+2,c3)))+ct3*(cs0*(cr0*ui(i1,i2,i3+3,c3)+cr1*ui(i1+
     & 1,i2,i3+3,c3)+cr2*ui(i1+2,i2,i3+3,c3)+cr3*ui(i1+3,i2,i3+3,c3)+
     & cr4*ui(i1+4,i2,i3+3,c3)+cr5*ui(i1+5,i2,i3+3,c3))+cs1*(cr0*ui(
     & i1,i2+1,i3+3,c3)+cr1*ui(i1+1,i2+1,i3+3,c3)+cr2*ui(i1+2,i2+1,i3+
     & 3,c3)+cr3*ui(i1+3,i2+1,i3+3,c3)+cr4*ui(i1+4,i2+1,i3+3,c3)+cr5*
     & ui(i1+5,i2+1,i3+3,c3))+cs2*(cr0*ui(i1,i2+2,i3+3,c3)+cr1*ui(i1+
     & 1,i2+2,i3+3,c3)+cr2*ui(i1+2,i2+2,i3+3,c3)+cr3*ui(i1+3,i2+2,i3+
     & 3,c3)+cr4*ui(i1+4,i2+2,i3+3,c3)+cr5*ui(i1+5,i2+2,i3+3,c3))+cs3*
     & (cr0*ui(i1,i2+3,i3+3,c3)+cr1*ui(i1+1,i2+3,i3+3,c3)+cr2*ui(i1+2,
     & i2+3,i3+3,c3)+cr3*ui(i1+3,i2+3,i3+3,c3)+cr4*ui(i1+4,i2+3,i3+3,
     & c3)+cr5*ui(i1+5,i2+3,i3+3,c3))+cs4*(cr0*ui(i1,i2+4,i3+3,c3)+
     & cr1*ui(i1+1,i2+4,i3+3,c3)+cr2*ui(i1+2,i2+4,i3+3,c3)+cr3*ui(i1+
     & 3,i2+4,i3+3,c3)+cr4*ui(i1+4,i2+4,i3+3,c3)+cr5*ui(i1+5,i2+4,i3+
     & 3,c3))+cs5*(cr0*ui(i1,i2+5,i3+3,c3)+cr1*ui(i1+1,i2+5,i3+3,c3)+
     & cr2*ui(i1+2,i2+5,i3+3,c3)+cr3*ui(i1+3,i2+5,i3+3,c3)+cr4*ui(i1+
     & 4,i2+5,i3+3,c3)+cr5*ui(i1+5,i2+5,i3+3,c3)))+ct4*(cs0*(cr0*ui(
     & i1,i2,i3+4,c3)+cr1*ui(i1+1,i2,i3+4,c3)+cr2*ui(i1+2,i2,i3+4,c3)+
     & cr3*ui(i1+3,i2,i3+4,c3)+cr4*ui(i1+4,i2,i3+4,c3)+cr5*ui(i1+5,i2,
     & i3+4,c3))+cs1*(cr0*ui(i1,i2+1,i3+4,c3)+cr1*ui(i1+1,i2+1,i3+4,
     & c3)+cr2*ui(i1+2,i2+1,i3+4,c3)+cr3*ui(i1+3,i2+1,i3+4,c3)+cr4*ui(
     & i1+4,i2+1,i3+4,c3)+cr5*ui(i1+5,i2+1,i3+4,c3))+cs2*(cr0*ui(i1,
     & i2+2,i3+4,c3)+cr1*ui(i1+1,i2+2,i3+4,c3)+cr2*ui(i1+2,i2+2,i3+4,
     & c3)+cr3*ui(i1+3,i2+2,i3+4,c3)+cr4*ui(i1+4,i2+2,i3+4,c3)+cr5*ui(
     & i1+5,i2+2,i3+4,c3))+cs3*(cr0*ui(i1,i2+3,i3+4,c3)+cr1*ui(i1+1,
     & i2+3,i3+4,c3)+cr2*ui(i1+2,i2+3,i3+4,c3)+cr3*ui(i1+3,i2+3,i3+4,
     & c3)+cr4*ui(i1+4,i2+3,i3+4,c3)+cr5*ui(i1+5,i2+3,i3+4,c3))+cs4*(
     & cr0*ui(i1,i2+4,i3+4,c3)+cr1*ui(i1+1,i2+4,i3+4,c3)+cr2*ui(i1+2,
     & i2+4,i3+4,c3)+cr3*ui(i1+3,i2+4,i3+4,c3)+cr4*ui(i1+4,i2+4,i3+4,
     & c3)+cr5*ui(i1+5,i2+4,i3+4,c3))+cs5*(cr0*ui(i1,i2+5,i3+4,c3)+
     & cr1*ui(i1+1,i2+5,i3+4,c3)+cr2*ui(i1+2,i2+5,i3+4,c3)+cr3*ui(i1+
     & 3,i2+5,i3+4,c3)+cr4*ui(i1+4,i2+5,i3+4,c3)+cr5*ui(i1+5,i2+5,i3+
     & 4,c3)))+ct5*(cs0*(cr0*ui(i1,i2,i3+5,c3)+cr1*ui(i1+1,i2,i3+5,c3)
     & +cr2*ui(i1+2,i2,i3+5,c3)+cr3*ui(i1+3,i2,i3+5,c3)+cr4*ui(i1+4,
     & i2,i3+5,c3)+cr5*ui(i1+5,i2,i3+5,c3))+cs1*(cr0*ui(i1,i2+1,i3+5,
     & c3)+cr1*ui(i1+1,i2+1,i3+5,c3)+cr2*ui(i1+2,i2+1,i3+5,c3)+cr3*ui(
     & i1+3,i2+1,i3+5,c3)+cr4*ui(i1+4,i2+1,i3+5,c3)+cr5*ui(i1+5,i2+1,
     & i3+5,c3))+cs2*(cr0*ui(i1,i2+2,i3+5,c3)+cr1*ui(i1+1,i2+2,i3+5,
     & c3)+cr2*ui(i1+2,i2+2,i3+5,c3)+cr3*ui(i1+3,i2+2,i3+5,c3)+cr4*ui(
     & i1+4,i2+2,i3+5,c3)+cr5*ui(i1+5,i2+2,i3+5,c3))+cs3*(cr0*ui(i1,
     & i2+3,i3+5,c3)+cr1*ui(i1+1,i2+3,i3+5,c3)+cr2*ui(i1+2,i2+3,i3+5,
     & c3)+cr3*ui(i1+3,i2+3,i3+5,c3)+cr4*ui(i1+4,i2+3,i3+5,c3)+cr5*ui(
     & i1+5,i2+3,i3+5,c3))+cs4*(cr0*ui(i1,i2+4,i3+5,c3)+cr1*ui(i1+1,
     & i2+4,i3+5,c3)+cr2*ui(i1+2,i2+4,i3+5,c3)+cr3*ui(i1+3,i2+4,i3+5,
     & c3)+cr4*ui(i1+4,i2+4,i3+5,c3)+cr5*ui(i1+5,i2+4,i3+5,c3))+cs5*(
     & cr0*ui(i1,i2+5,i3+5,c3)+cr1*ui(i1+1,i2+5,i3+5,c3)+cr2*ui(i1+2,
     & i2+5,i3+5,c3)+cr3*ui(i1+3,i2+5,i3+5,c3)+cr4*ui(i1+4,i2+5,i3+5,
     & c3)+cr5*ui(i1+5,i2+5,i3+5,c3)))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           end if
         else if( width(1).eq.7 .and. width(2).eq.7 .and. width(3)
     & .eq.7 )then
! loops3d($interpSparseStorage777(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))
           if( c3a.eq.c3b )then
             do c3=c3a,c3b
             do i=nia,nib
! interpSparseStorage777(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q70(c(i,0,0,0))
                 cs0 = q70(c(i,1,0,0))
                 ct0 = q70(c(i,2,0,0))
                 cr1 = q71(c(i,0,0,0))
                 cs1 = q71(c(i,1,0,0))
                 ct1 = q71(c(i,2,0,0))
                 cr2 = q72(c(i,0,0,0))
                 cs2 = q72(c(i,1,0,0))
                 ct2 = q72(c(i,2,0,0))
                 cr3 = q73(c(i,0,0,0))
                 cs3 = q73(c(i,1,0,0))
                 ct3 = q73(c(i,2,0,0))
                 cr4 = q74(c(i,0,0,0))
                 cs4 = q74(c(i,1,0,0))
                 ct4 = q74(c(i,2,0,0))
                 cr5 = q75(c(i,0,0,0))
                 cs5 = q75(c(i,1,0,0))
                 ct5 = q75(c(i,2,0,0))
                 cr6 = q76(c(i,0,0,0))
                 cs6 = q76(c(i,1,0,0))
                 ct6 = q76(c(i,2,0,0))
               r(i) = ct0*(cs0*(cr0*ui(i1,i2,i3+0,c3)+cr1*ui(i1+1,i2,
     & i3+0,c3)+cr2*ui(i1+2,i2,i3+0,c3)+cr3*ui(i1+3,i2,i3+0,c3)+cr4*
     & ui(i1+4,i2,i3+0,c3)+cr5*ui(i1+5,i2,i3+0,c3)+cr6*ui(i1+6,i2,i3+
     & 0,c3))+cs1*(cr0*ui(i1,i2+1,i3+0,c3)+cr1*ui(i1+1,i2+1,i3+0,c3)+
     & cr2*ui(i1+2,i2+1,i3+0,c3)+cr3*ui(i1+3,i2+1,i3+0,c3)+cr4*ui(i1+
     & 4,i2+1,i3+0,c3)+cr5*ui(i1+5,i2+1,i3+0,c3)+cr6*ui(i1+6,i2+1,i3+
     & 0,c3))+cs2*(cr0*ui(i1,i2+2,i3+0,c3)+cr1*ui(i1+1,i2+2,i3+0,c3)+
     & cr2*ui(i1+2,i2+2,i3+0,c3)+cr3*ui(i1+3,i2+2,i3+0,c3)+cr4*ui(i1+
     & 4,i2+2,i3+0,c3)+cr5*ui(i1+5,i2+2,i3+0,c3)+cr6*ui(i1+6,i2+2,i3+
     & 0,c3))+cs3*(cr0*ui(i1,i2+3,i3+0,c3)+cr1*ui(i1+1,i2+3,i3+0,c3)+
     & cr2*ui(i1+2,i2+3,i3+0,c3)+cr3*ui(i1+3,i2+3,i3+0,c3)+cr4*ui(i1+
     & 4,i2+3,i3+0,c3)+cr5*ui(i1+5,i2+3,i3+0,c3)+cr6*ui(i1+6,i2+3,i3+
     & 0,c3))+cs4*(cr0*ui(i1,i2+4,i3+0,c3)+cr1*ui(i1+1,i2+4,i3+0,c3)+
     & cr2*ui(i1+2,i2+4,i3+0,c3)+cr3*ui(i1+3,i2+4,i3+0,c3)+cr4*ui(i1+
     & 4,i2+4,i3+0,c3)+cr5*ui(i1+5,i2+4,i3+0,c3)+cr6*ui(i1+6,i2+4,i3+
     & 0,c3))+cs5*(cr0*ui(i1,i2+5,i3+0,c3)+cr1*ui(i1+1,i2+5,i3+0,c3)+
     & cr2*ui(i1+2,i2+5,i3+0,c3)+cr3*ui(i1+3,i2+5,i3+0,c3)+cr4*ui(i1+
     & 4,i2+5,i3+0,c3)+cr5*ui(i1+5,i2+5,i3+0,c3)+cr6*ui(i1+6,i2+5,i3+
     & 0,c3))+cs6*(cr0*ui(i1,i2+6,i3+0,c3)+cr1*ui(i1+1,i2+6,i3+0,c3)+
     & cr2*ui(i1+2,i2+6,i3+0,c3)+cr3*ui(i1+3,i2+6,i3+0,c3)+cr4*ui(i1+
     & 4,i2+6,i3+0,c3)+cr5*ui(i1+5,i2+6,i3+0,c3)+cr6*ui(i1+6,i2+6,i3+
     & 0,c3)))+ct1*(cs0*(cr0*ui(i1,i2,i3+1,c3)+cr1*ui(i1+1,i2,i3+1,c3)
     & +cr2*ui(i1+2,i2,i3+1,c3)+cr3*ui(i1+3,i2,i3+1,c3)+cr4*ui(i1+4,
     & i2,i3+1,c3)+cr5*ui(i1+5,i2,i3+1,c3)+cr6*ui(i1+6,i2,i3+1,c3))+
     & cs1*(cr0*ui(i1,i2+1,i3+1,c3)+cr1*ui(i1+1,i2+1,i3+1,c3)+cr2*ui(
     & i1+2,i2+1,i3+1,c3)+cr3*ui(i1+3,i2+1,i3+1,c3)+cr4*ui(i1+4,i2+1,
     & i3+1,c3)+cr5*ui(i1+5,i2+1,i3+1,c3)+cr6*ui(i1+6,i2+1,i3+1,c3))+
     & cs2*(cr0*ui(i1,i2+2,i3+1,c3)+cr1*ui(i1+1,i2+2,i3+1,c3)+cr2*ui(
     & i1+2,i2+2,i3+1,c3)+cr3*ui(i1+3,i2+2,i3+1,c3)+cr4*ui(i1+4,i2+2,
     & i3+1,c3)+cr5*ui(i1+5,i2+2,i3+1,c3)+cr6*ui(i1+6,i2+2,i3+1,c3))+
     & cs3*(cr0*ui(i1,i2+3,i3+1,c3)+cr1*ui(i1+1,i2+3,i3+1,c3)+cr2*ui(
     & i1+2,i2+3,i3+1,c3)+cr3*ui(i1+3,i2+3,i3+1,c3)+cr4*ui(i1+4,i2+3,
     & i3+1,c3)+cr5*ui(i1+5,i2+3,i3+1,c3)+cr6*ui(i1+6,i2+3,i3+1,c3))+
     & cs4*(cr0*ui(i1,i2+4,i3+1,c3)+cr1*ui(i1+1,i2+4,i3+1,c3)+cr2*ui(
     & i1+2,i2+4,i3+1,c3)+cr3*ui(i1+3,i2+4,i3+1,c3)+cr4*ui(i1+4,i2+4,
     & i3+1,c3)+cr5*ui(i1+5,i2+4,i3+1,c3)+cr6*ui(i1+6,i2+4,i3+1,c3))+
     & cs5*(cr0*ui(i1,i2+5,i3+1,c3)+cr1*ui(i1+1,i2+5,i3+1,c3)+cr2*ui(
     & i1+2,i2+5,i3+1,c3)+cr3*ui(i1+3,i2+5,i3+1,c3)+cr4*ui(i1+4,i2+5,
     & i3+1,c3)+cr5*ui(i1+5,i2+5,i3+1,c3)+cr6*ui(i1+6,i2+5,i3+1,c3))+
     & cs6*(cr0*ui(i1,i2+6,i3+1,c3)+cr1*ui(i1+1,i2+6,i3+1,c3)+cr2*ui(
     & i1+2,i2+6,i3+1,c3)+cr3*ui(i1+3,i2+6,i3+1,c3)+cr4*ui(i1+4,i2+6,
     & i3+1,c3)+cr5*ui(i1+5,i2+6,i3+1,c3)+cr6*ui(i1+6,i2+6,i3+1,c3)))+
     & ct2*(cs0*(cr0*ui(i1,i2,i3+2,c3)+cr1*ui(i1+1,i2,i3+2,c3)+cr2*ui(
     & i1+2,i2,i3+2,c3)+cr3*ui(i1+3,i2,i3+2,c3)+cr4*ui(i1+4,i2,i3+2,
     & c3)+cr5*ui(i1+5,i2,i3+2,c3)+cr6*ui(i1+6,i2,i3+2,c3))+cs1*(cr0*
     & ui(i1,i2+1,i3+2,c3)+cr1*ui(i1+1,i2+1,i3+2,c3)+cr2*ui(i1+2,i2+1,
     & i3+2,c3)+cr3*ui(i1+3,i2+1,i3+2,c3)+cr4*ui(i1+4,i2+1,i3+2,c3)+
     & cr5*ui(i1+5,i2+1,i3+2,c3)+cr6*ui(i1+6,i2+1,i3+2,c3))+cs2*(cr0*
     & ui(i1,i2+2,i3+2,c3)+cr1*ui(i1+1,i2+2,i3+2,c3)+cr2*ui(i1+2,i2+2,
     & i3+2,c3)+cr3*ui(i1+3,i2+2,i3+2,c3)+cr4*ui(i1+4,i2+2,i3+2,c3)+
     & cr5*ui(i1+5,i2+2,i3+2,c3)+cr6*ui(i1+6,i2+2,i3+2,c3))+cs3*(cr0*
     & ui(i1,i2+3,i3+2,c3)+cr1*ui(i1+1,i2+3,i3+2,c3)+cr2*ui(i1+2,i2+3,
     & i3+2,c3)+cr3*ui(i1+3,i2+3,i3+2,c3)+cr4*ui(i1+4,i2+3,i3+2,c3)+
     & cr5*ui(i1+5,i2+3,i3+2,c3)+cr6*ui(i1+6,i2+3,i3+2,c3))+cs4*(cr0*
     & ui(i1,i2+4,i3+2,c3)+cr1*ui(i1+1,i2+4,i3+2,c3)+cr2*ui(i1+2,i2+4,
     & i3+2,c3)+cr3*ui(i1+3,i2+4,i3+2,c3)+cr4*ui(i1+4,i2+4,i3+2,c3)+
     & cr5*ui(i1+5,i2+4,i3+2,c3)+cr6*ui(i1+6,i2+4,i3+2,c3))+cs5*(cr0*
     & ui(i1,i2+5,i3+2,c3)+cr1*ui(i1+1,i2+5,i3+2,c3)+cr2*ui(i1+2,i2+5,
     & i3+2,c3)+cr3*ui(i1+3,i2+5,i3+2,c3)+cr4*ui(i1+4,i2+5,i3+2,c3)+
     & cr5*ui(i1+5,i2+5,i3+2,c3)+cr6*ui(i1+6,i2+5,i3+2,c3))+cs6*(cr0*
     & ui(i1,i2+6,i3+2,c3)+cr1*ui(i1+1,i2+6,i3+2,c3)+cr2*ui(i1+2,i2+6,
     & i3+2,c3)+cr3*ui(i1+3,i2+6,i3+2,c3)+cr4*ui(i1+4,i2+6,i3+2,c3)+
     & cr5*ui(i1+5,i2+6,i3+2,c3)+cr6*ui(i1+6,i2+6,i3+2,c3)))+ct3*(cs0*
     & (cr0*ui(i1,i2,i3+3,c3)+cr1*ui(i1+1,i2,i3+3,c3)+cr2*ui(i1+2,i2,
     & i3+3,c3)+cr3*ui(i1+3,i2,i3+3,c3)+cr4*ui(i1+4,i2,i3+3,c3)+cr5*
     & ui(i1+5,i2,i3+3,c3)+cr6*ui(i1+6,i2,i3+3,c3))+cs1*(cr0*ui(i1,i2+
     & 1,i3+3,c3)+cr1*ui(i1+1,i2+1,i3+3,c3)+cr2*ui(i1+2,i2+1,i3+3,c3)+
     & cr3*ui(i1+3,i2+1,i3+3,c3)+cr4*ui(i1+4,i2+1,i3+3,c3)+cr5*ui(i1+
     & 5,i2+1,i3+3,c3)+cr6*ui(i1+6,i2+1,i3+3,c3))+cs2*(cr0*ui(i1,i2+2,
     & i3+3,c3)+cr1*ui(i1+1,i2+2,i3+3,c3)+cr2*ui(i1+2,i2+2,i3+3,c3)+
     & cr3*ui(i1+3,i2+2,i3+3,c3)+cr4*ui(i1+4,i2+2,i3+3,c3)+cr5*ui(i1+
     & 5,i2+2,i3+3,c3)+cr6*ui(i1+6,i2+2,i3+3,c3))+cs3*(cr0*ui(i1,i2+3,
     & i3+3,c3)+cr1*ui(i1+1,i2+3,i3+3,c3)+cr2*ui(i1+2,i2+3,i3+3,c3)+
     & cr3*ui(i1+3,i2+3,i3+3,c3)+cr4*ui(i1+4,i2+3,i3+3,c3)+cr5*ui(i1+
     & 5,i2+3,i3+3,c3)+cr6*ui(i1+6,i2+3,i3+3,c3))+cs4*(cr0*ui(i1,i2+4,
     & i3+3,c3)+cr1*ui(i1+1,i2+4,i3+3,c3)+cr2*ui(i1+2,i2+4,i3+3,c3)+
     & cr3*ui(i1+3,i2+4,i3+3,c3)+cr4*ui(i1+4,i2+4,i3+3,c3)+cr5*ui(i1+
     & 5,i2+4,i3+3,c3)+cr6*ui(i1+6,i2+4,i3+3,c3))+cs5*(cr0*ui(i1,i2+5,
     & i3+3,c3)+cr1*ui(i1+1,i2+5,i3+3,c3)+cr2*ui(i1+2,i2+5,i3+3,c3)+
     & cr3*ui(i1+3,i2+5,i3+3,c3)+cr4*ui(i1+4,i2+5,i3+3,c3)+cr5*ui(i1+
     & 5,i2+5,i3+3,c3)+cr6*ui(i1+6,i2+5,i3+3,c3))+cs6*(cr0*ui(i1,i2+6,
     & i3+3,c3)+cr1*ui(i1+1,i2+6,i3+3,c3)+cr2*ui(i1+2,i2+6,i3+3,c3)+
     & cr3*ui(i1+3,i2+6,i3+3,c3)+cr4*ui(i1+4,i2+6,i3+3,c3)+cr5*ui(i1+
     & 5,i2+6,i3+3,c3)+cr6*ui(i1+6,i2+6,i3+3,c3)))
               r(i) = r(i)+ct4*(cs0*(cr0*ui(i1,i2,i3+4,c3)+cr1*ui(i1+1,
     & i2,i3+4,c3)+cr2*ui(i1+2,i2,i3+4,c3)+cr3*ui(i1+3,i2,i3+4,c3)+
     & cr4*ui(i1+4,i2,i3+4,c3)+cr5*ui(i1+5,i2,i3+4,c3)+cr6*ui(i1+6,i2,
     & i3+4,c3))+cs1*(cr0*ui(i1,i2+1,i3+4,c3)+cr1*ui(i1+1,i2+1,i3+4,
     & c3)+cr2*ui(i1+2,i2+1,i3+4,c3)+cr3*ui(i1+3,i2+1,i3+4,c3)+cr4*ui(
     & i1+4,i2+1,i3+4,c3)+cr5*ui(i1+5,i2+1,i3+4,c3)+cr6*ui(i1+6,i2+1,
     & i3+4,c3))+cs2*(cr0*ui(i1,i2+2,i3+4,c3)+cr1*ui(i1+1,i2+2,i3+4,
     & c3)+cr2*ui(i1+2,i2+2,i3+4,c3)+cr3*ui(i1+3,i2+2,i3+4,c3)+cr4*ui(
     & i1+4,i2+2,i3+4,c3)+cr5*ui(i1+5,i2+2,i3+4,c3)+cr6*ui(i1+6,i2+2,
     & i3+4,c3))+cs3*(cr0*ui(i1,i2+3,i3+4,c3)+cr1*ui(i1+1,i2+3,i3+4,
     & c3)+cr2*ui(i1+2,i2+3,i3+4,c3)+cr3*ui(i1+3,i2+3,i3+4,c3)+cr4*ui(
     & i1+4,i2+3,i3+4,c3)+cr5*ui(i1+5,i2+3,i3+4,c3)+cr6*ui(i1+6,i2+3,
     & i3+4,c3))+cs4*(cr0*ui(i1,i2+4,i3+4,c3)+cr1*ui(i1+1,i2+4,i3+4,
     & c3)+cr2*ui(i1+2,i2+4,i3+4,c3)+cr3*ui(i1+3,i2+4,i3+4,c3)+cr4*ui(
     & i1+4,i2+4,i3+4,c3)+cr5*ui(i1+5,i2+4,i3+4,c3)+cr6*ui(i1+6,i2+4,
     & i3+4,c3))+cs5*(cr0*ui(i1,i2+5,i3+4,c3)+cr1*ui(i1+1,i2+5,i3+4,
     & c3)+cr2*ui(i1+2,i2+5,i3+4,c3)+cr3*ui(i1+3,i2+5,i3+4,c3)+cr4*ui(
     & i1+4,i2+5,i3+4,c3)+cr5*ui(i1+5,i2+5,i3+4,c3)+cr6*ui(i1+6,i2+5,
     & i3+4,c3))+cs6*(cr0*ui(i1,i2+6,i3+4,c3)+cr1*ui(i1+1,i2+6,i3+4,
     & c3)+cr2*ui(i1+2,i2+6,i3+4,c3)+cr3*ui(i1+3,i2+6,i3+4,c3)+cr4*ui(
     & i1+4,i2+6,i3+4,c3)+cr5*ui(i1+5,i2+6,i3+4,c3)+cr6*ui(i1+6,i2+6,
     & i3+4,c3)))+ct5*(cs0*(cr0*ui(i1,i2,i3+5,c3)+cr1*ui(i1+1,i2,i3+5,
     & c3)+cr2*ui(i1+2,i2,i3+5,c3)+cr3*ui(i1+3,i2,i3+5,c3)+cr4*ui(i1+
     & 4,i2,i3+5,c3)+cr5*ui(i1+5,i2,i3+5,c3)+cr6*ui(i1+6,i2,i3+5,c3))+
     & cs1*(cr0*ui(i1,i2+1,i3+5,c3)+cr1*ui(i1+1,i2+1,i3+5,c3)+cr2*ui(
     & i1+2,i2+1,i3+5,c3)+cr3*ui(i1+3,i2+1,i3+5,c3)+cr4*ui(i1+4,i2+1,
     & i3+5,c3)+cr5*ui(i1+5,i2+1,i3+5,c3)+cr6*ui(i1+6,i2+1,i3+5,c3))+
     & cs2*(cr0*ui(i1,i2+2,i3+5,c3)+cr1*ui(i1+1,i2+2,i3+5,c3)+cr2*ui(
     & i1+2,i2+2,i3+5,c3)+cr3*ui(i1+3,i2+2,i3+5,c3)+cr4*ui(i1+4,i2+2,
     & i3+5,c3)+cr5*ui(i1+5,i2+2,i3+5,c3)+cr6*ui(i1+6,i2+2,i3+5,c3))+
     & cs3*(cr0*ui(i1,i2+3,i3+5,c3)+cr1*ui(i1+1,i2+3,i3+5,c3)+cr2*ui(
     & i1+2,i2+3,i3+5,c3)+cr3*ui(i1+3,i2+3,i3+5,c3)+cr4*ui(i1+4,i2+3,
     & i3+5,c3)+cr5*ui(i1+5,i2+3,i3+5,c3)+cr6*ui(i1+6,i2+3,i3+5,c3))+
     & cs4*(cr0*ui(i1,i2+4,i3+5,c3)+cr1*ui(i1+1,i2+4,i3+5,c3)+cr2*ui(
     & i1+2,i2+4,i3+5,c3)+cr3*ui(i1+3,i2+4,i3+5,c3)+cr4*ui(i1+4,i2+4,
     & i3+5,c3)+cr5*ui(i1+5,i2+4,i3+5,c3)+cr6*ui(i1+6,i2+4,i3+5,c3))+
     & cs5*(cr0*ui(i1,i2+5,i3+5,c3)+cr1*ui(i1+1,i2+5,i3+5,c3)+cr2*ui(
     & i1+2,i2+5,i3+5,c3)+cr3*ui(i1+3,i2+5,i3+5,c3)+cr4*ui(i1+4,i2+5,
     & i3+5,c3)+cr5*ui(i1+5,i2+5,i3+5,c3)+cr6*ui(i1+6,i2+5,i3+5,c3))+
     & cs6*(cr0*ui(i1,i2+6,i3+5,c3)+cr1*ui(i1+1,i2+6,i3+5,c3)+cr2*ui(
     & i1+2,i2+6,i3+5,c3)+cr3*ui(i1+3,i2+6,i3+5,c3)+cr4*ui(i1+4,i2+6,
     & i3+5,c3)+cr5*ui(i1+5,i2+6,i3+5,c3)+cr6*ui(i1+6,i2+6,i3+5,c3)))+
     & ct6*(cs0*(cr0*ui(i1,i2,i3+6,c3)+cr1*ui(i1+1,i2,i3+6,c3)+cr2*ui(
     & i1+2,i2,i3+6,c3)+cr3*ui(i1+3,i2,i3+6,c3)+cr4*ui(i1+4,i2,i3+6,
     & c3)+cr5*ui(i1+5,i2,i3+6,c3)+cr6*ui(i1+6,i2,i3+6,c3))+cs1*(cr0*
     & ui(i1,i2+1,i3+6,c3)+cr1*ui(i1+1,i2+1,i3+6,c3)+cr2*ui(i1+2,i2+1,
     & i3+6,c3)+cr3*ui(i1+3,i2+1,i3+6,c3)+cr4*ui(i1+4,i2+1,i3+6,c3)+
     & cr5*ui(i1+5,i2+1,i3+6,c3)+cr6*ui(i1+6,i2+1,i3+6,c3))+cs2*(cr0*
     & ui(i1,i2+2,i3+6,c3)+cr1*ui(i1+1,i2+2,i3+6,c3)+cr2*ui(i1+2,i2+2,
     & i3+6,c3)+cr3*ui(i1+3,i2+2,i3+6,c3)+cr4*ui(i1+4,i2+2,i3+6,c3)+
     & cr5*ui(i1+5,i2+2,i3+6,c3)+cr6*ui(i1+6,i2+2,i3+6,c3))+cs3*(cr0*
     & ui(i1,i2+3,i3+6,c3)+cr1*ui(i1+1,i2+3,i3+6,c3)+cr2*ui(i1+2,i2+3,
     & i3+6,c3)+cr3*ui(i1+3,i2+3,i3+6,c3)+cr4*ui(i1+4,i2+3,i3+6,c3)+
     & cr5*ui(i1+5,i2+3,i3+6,c3)+cr6*ui(i1+6,i2+3,i3+6,c3))+cs4*(cr0*
     & ui(i1,i2+4,i3+6,c3)+cr1*ui(i1+1,i2+4,i3+6,c3)+cr2*ui(i1+2,i2+4,
     & i3+6,c3)+cr3*ui(i1+3,i2+4,i3+6,c3)+cr4*ui(i1+4,i2+4,i3+6,c3)+
     & cr5*ui(i1+5,i2+4,i3+6,c3)+cr6*ui(i1+6,i2+4,i3+6,c3))+cs5*(cr0*
     & ui(i1,i2+5,i3+6,c3)+cr1*ui(i1+1,i2+5,i3+6,c3)+cr2*ui(i1+2,i2+5,
     & i3+6,c3)+cr3*ui(i1+3,i2+5,i3+6,c3)+cr4*ui(i1+4,i2+5,i3+6,c3)+
     & cr5*ui(i1+5,i2+5,i3+6,c3)+cr6*ui(i1+6,i2+5,i3+6,c3))+cs6*(cr0*
     & ui(i1,i2+6,i3+6,c3)+cr1*ui(i1+1,i2+6,i3+6,c3)+cr2*ui(i1+2,i2+6,
     & i3+6,c3)+cr3*ui(i1+3,i2+6,i3+6,c3)+cr4*ui(i1+4,i2+6,i3+6,c3)+
     & cr5*ui(i1+5,i2+6,i3+6,c3)+cr6*ui(i1+6,i2+6,i3+6,c3)))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
! interpSparseStorage777(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q70(c(i,0,0,0))
                 cs0 = q70(c(i,1,0,0))
                 ct0 = q70(c(i,2,0,0))
                 cr1 = q71(c(i,0,0,0))
                 cs1 = q71(c(i,1,0,0))
                 ct1 = q71(c(i,2,0,0))
                 cr2 = q72(c(i,0,0,0))
                 cs2 = q72(c(i,1,0,0))
                 ct2 = q72(c(i,2,0,0))
                 cr3 = q73(c(i,0,0,0))
                 cs3 = q73(c(i,1,0,0))
                 ct3 = q73(c(i,2,0,0))
                 cr4 = q74(c(i,0,0,0))
                 cs4 = q74(c(i,1,0,0))
                 ct4 = q74(c(i,2,0,0))
                 cr5 = q75(c(i,0,0,0))
                 cs5 = q75(c(i,1,0,0))
                 ct5 = q75(c(i,2,0,0))
                 cr6 = q76(c(i,0,0,0))
                 cs6 = q76(c(i,1,0,0))
                 ct6 = q76(c(i,2,0,0))
               r(i) = ct0*(cs0*(cr0*ui(i1,i2,i3+0,c3)+cr1*ui(i1+1,i2,
     & i3+0,c3)+cr2*ui(i1+2,i2,i3+0,c3)+cr3*ui(i1+3,i2,i3+0,c3)+cr4*
     & ui(i1+4,i2,i3+0,c3)+cr5*ui(i1+5,i2,i3+0,c3)+cr6*ui(i1+6,i2,i3+
     & 0,c3))+cs1*(cr0*ui(i1,i2+1,i3+0,c3)+cr1*ui(i1+1,i2+1,i3+0,c3)+
     & cr2*ui(i1+2,i2+1,i3+0,c3)+cr3*ui(i1+3,i2+1,i3+0,c3)+cr4*ui(i1+
     & 4,i2+1,i3+0,c3)+cr5*ui(i1+5,i2+1,i3+0,c3)+cr6*ui(i1+6,i2+1,i3+
     & 0,c3))+cs2*(cr0*ui(i1,i2+2,i3+0,c3)+cr1*ui(i1+1,i2+2,i3+0,c3)+
     & cr2*ui(i1+2,i2+2,i3+0,c3)+cr3*ui(i1+3,i2+2,i3+0,c3)+cr4*ui(i1+
     & 4,i2+2,i3+0,c3)+cr5*ui(i1+5,i2+2,i3+0,c3)+cr6*ui(i1+6,i2+2,i3+
     & 0,c3))+cs3*(cr0*ui(i1,i2+3,i3+0,c3)+cr1*ui(i1+1,i2+3,i3+0,c3)+
     & cr2*ui(i1+2,i2+3,i3+0,c3)+cr3*ui(i1+3,i2+3,i3+0,c3)+cr4*ui(i1+
     & 4,i2+3,i3+0,c3)+cr5*ui(i1+5,i2+3,i3+0,c3)+cr6*ui(i1+6,i2+3,i3+
     & 0,c3))+cs4*(cr0*ui(i1,i2+4,i3+0,c3)+cr1*ui(i1+1,i2+4,i3+0,c3)+
     & cr2*ui(i1+2,i2+4,i3+0,c3)+cr3*ui(i1+3,i2+4,i3+0,c3)+cr4*ui(i1+
     & 4,i2+4,i3+0,c3)+cr5*ui(i1+5,i2+4,i3+0,c3)+cr6*ui(i1+6,i2+4,i3+
     & 0,c3))+cs5*(cr0*ui(i1,i2+5,i3+0,c3)+cr1*ui(i1+1,i2+5,i3+0,c3)+
     & cr2*ui(i1+2,i2+5,i3+0,c3)+cr3*ui(i1+3,i2+5,i3+0,c3)+cr4*ui(i1+
     & 4,i2+5,i3+0,c3)+cr5*ui(i1+5,i2+5,i3+0,c3)+cr6*ui(i1+6,i2+5,i3+
     & 0,c3))+cs6*(cr0*ui(i1,i2+6,i3+0,c3)+cr1*ui(i1+1,i2+6,i3+0,c3)+
     & cr2*ui(i1+2,i2+6,i3+0,c3)+cr3*ui(i1+3,i2+6,i3+0,c3)+cr4*ui(i1+
     & 4,i2+6,i3+0,c3)+cr5*ui(i1+5,i2+6,i3+0,c3)+cr6*ui(i1+6,i2+6,i3+
     & 0,c3)))+ct1*(cs0*(cr0*ui(i1,i2,i3+1,c3)+cr1*ui(i1+1,i2,i3+1,c3)
     & +cr2*ui(i1+2,i2,i3+1,c3)+cr3*ui(i1+3,i2,i3+1,c3)+cr4*ui(i1+4,
     & i2,i3+1,c3)+cr5*ui(i1+5,i2,i3+1,c3)+cr6*ui(i1+6,i2,i3+1,c3))+
     & cs1*(cr0*ui(i1,i2+1,i3+1,c3)+cr1*ui(i1+1,i2+1,i3+1,c3)+cr2*ui(
     & i1+2,i2+1,i3+1,c3)+cr3*ui(i1+3,i2+1,i3+1,c3)+cr4*ui(i1+4,i2+1,
     & i3+1,c3)+cr5*ui(i1+5,i2+1,i3+1,c3)+cr6*ui(i1+6,i2+1,i3+1,c3))+
     & cs2*(cr0*ui(i1,i2+2,i3+1,c3)+cr1*ui(i1+1,i2+2,i3+1,c3)+cr2*ui(
     & i1+2,i2+2,i3+1,c3)+cr3*ui(i1+3,i2+2,i3+1,c3)+cr4*ui(i1+4,i2+2,
     & i3+1,c3)+cr5*ui(i1+5,i2+2,i3+1,c3)+cr6*ui(i1+6,i2+2,i3+1,c3))+
     & cs3*(cr0*ui(i1,i2+3,i3+1,c3)+cr1*ui(i1+1,i2+3,i3+1,c3)+cr2*ui(
     & i1+2,i2+3,i3+1,c3)+cr3*ui(i1+3,i2+3,i3+1,c3)+cr4*ui(i1+4,i2+3,
     & i3+1,c3)+cr5*ui(i1+5,i2+3,i3+1,c3)+cr6*ui(i1+6,i2+3,i3+1,c3))+
     & cs4*(cr0*ui(i1,i2+4,i3+1,c3)+cr1*ui(i1+1,i2+4,i3+1,c3)+cr2*ui(
     & i1+2,i2+4,i3+1,c3)+cr3*ui(i1+3,i2+4,i3+1,c3)+cr4*ui(i1+4,i2+4,
     & i3+1,c3)+cr5*ui(i1+5,i2+4,i3+1,c3)+cr6*ui(i1+6,i2+4,i3+1,c3))+
     & cs5*(cr0*ui(i1,i2+5,i3+1,c3)+cr1*ui(i1+1,i2+5,i3+1,c3)+cr2*ui(
     & i1+2,i2+5,i3+1,c3)+cr3*ui(i1+3,i2+5,i3+1,c3)+cr4*ui(i1+4,i2+5,
     & i3+1,c3)+cr5*ui(i1+5,i2+5,i3+1,c3)+cr6*ui(i1+6,i2+5,i3+1,c3))+
     & cs6*(cr0*ui(i1,i2+6,i3+1,c3)+cr1*ui(i1+1,i2+6,i3+1,c3)+cr2*ui(
     & i1+2,i2+6,i3+1,c3)+cr3*ui(i1+3,i2+6,i3+1,c3)+cr4*ui(i1+4,i2+6,
     & i3+1,c3)+cr5*ui(i1+5,i2+6,i3+1,c3)+cr6*ui(i1+6,i2+6,i3+1,c3)))+
     & ct2*(cs0*(cr0*ui(i1,i2,i3+2,c3)+cr1*ui(i1+1,i2,i3+2,c3)+cr2*ui(
     & i1+2,i2,i3+2,c3)+cr3*ui(i1+3,i2,i3+2,c3)+cr4*ui(i1+4,i2,i3+2,
     & c3)+cr5*ui(i1+5,i2,i3+2,c3)+cr6*ui(i1+6,i2,i3+2,c3))+cs1*(cr0*
     & ui(i1,i2+1,i3+2,c3)+cr1*ui(i1+1,i2+1,i3+2,c3)+cr2*ui(i1+2,i2+1,
     & i3+2,c3)+cr3*ui(i1+3,i2+1,i3+2,c3)+cr4*ui(i1+4,i2+1,i3+2,c3)+
     & cr5*ui(i1+5,i2+1,i3+2,c3)+cr6*ui(i1+6,i2+1,i3+2,c3))+cs2*(cr0*
     & ui(i1,i2+2,i3+2,c3)+cr1*ui(i1+1,i2+2,i3+2,c3)+cr2*ui(i1+2,i2+2,
     & i3+2,c3)+cr3*ui(i1+3,i2+2,i3+2,c3)+cr4*ui(i1+4,i2+2,i3+2,c3)+
     & cr5*ui(i1+5,i2+2,i3+2,c3)+cr6*ui(i1+6,i2+2,i3+2,c3))+cs3*(cr0*
     & ui(i1,i2+3,i3+2,c3)+cr1*ui(i1+1,i2+3,i3+2,c3)+cr2*ui(i1+2,i2+3,
     & i3+2,c3)+cr3*ui(i1+3,i2+3,i3+2,c3)+cr4*ui(i1+4,i2+3,i3+2,c3)+
     & cr5*ui(i1+5,i2+3,i3+2,c3)+cr6*ui(i1+6,i2+3,i3+2,c3))+cs4*(cr0*
     & ui(i1,i2+4,i3+2,c3)+cr1*ui(i1+1,i2+4,i3+2,c3)+cr2*ui(i1+2,i2+4,
     & i3+2,c3)+cr3*ui(i1+3,i2+4,i3+2,c3)+cr4*ui(i1+4,i2+4,i3+2,c3)+
     & cr5*ui(i1+5,i2+4,i3+2,c3)+cr6*ui(i1+6,i2+4,i3+2,c3))+cs5*(cr0*
     & ui(i1,i2+5,i3+2,c3)+cr1*ui(i1+1,i2+5,i3+2,c3)+cr2*ui(i1+2,i2+5,
     & i3+2,c3)+cr3*ui(i1+3,i2+5,i3+2,c3)+cr4*ui(i1+4,i2+5,i3+2,c3)+
     & cr5*ui(i1+5,i2+5,i3+2,c3)+cr6*ui(i1+6,i2+5,i3+2,c3))+cs6*(cr0*
     & ui(i1,i2+6,i3+2,c3)+cr1*ui(i1+1,i2+6,i3+2,c3)+cr2*ui(i1+2,i2+6,
     & i3+2,c3)+cr3*ui(i1+3,i2+6,i3+2,c3)+cr4*ui(i1+4,i2+6,i3+2,c3)+
     & cr5*ui(i1+5,i2+6,i3+2,c3)+cr6*ui(i1+6,i2+6,i3+2,c3)))+ct3*(cs0*
     & (cr0*ui(i1,i2,i3+3,c3)+cr1*ui(i1+1,i2,i3+3,c3)+cr2*ui(i1+2,i2,
     & i3+3,c3)+cr3*ui(i1+3,i2,i3+3,c3)+cr4*ui(i1+4,i2,i3+3,c3)+cr5*
     & ui(i1+5,i2,i3+3,c3)+cr6*ui(i1+6,i2,i3+3,c3))+cs1*(cr0*ui(i1,i2+
     & 1,i3+3,c3)+cr1*ui(i1+1,i2+1,i3+3,c3)+cr2*ui(i1+2,i2+1,i3+3,c3)+
     & cr3*ui(i1+3,i2+1,i3+3,c3)+cr4*ui(i1+4,i2+1,i3+3,c3)+cr5*ui(i1+
     & 5,i2+1,i3+3,c3)+cr6*ui(i1+6,i2+1,i3+3,c3))+cs2*(cr0*ui(i1,i2+2,
     & i3+3,c3)+cr1*ui(i1+1,i2+2,i3+3,c3)+cr2*ui(i1+2,i2+2,i3+3,c3)+
     & cr3*ui(i1+3,i2+2,i3+3,c3)+cr4*ui(i1+4,i2+2,i3+3,c3)+cr5*ui(i1+
     & 5,i2+2,i3+3,c3)+cr6*ui(i1+6,i2+2,i3+3,c3))+cs3*(cr0*ui(i1,i2+3,
     & i3+3,c3)+cr1*ui(i1+1,i2+3,i3+3,c3)+cr2*ui(i1+2,i2+3,i3+3,c3)+
     & cr3*ui(i1+3,i2+3,i3+3,c3)+cr4*ui(i1+4,i2+3,i3+3,c3)+cr5*ui(i1+
     & 5,i2+3,i3+3,c3)+cr6*ui(i1+6,i2+3,i3+3,c3))+cs4*(cr0*ui(i1,i2+4,
     & i3+3,c3)+cr1*ui(i1+1,i2+4,i3+3,c3)+cr2*ui(i1+2,i2+4,i3+3,c3)+
     & cr3*ui(i1+3,i2+4,i3+3,c3)+cr4*ui(i1+4,i2+4,i3+3,c3)+cr5*ui(i1+
     & 5,i2+4,i3+3,c3)+cr6*ui(i1+6,i2+4,i3+3,c3))+cs5*(cr0*ui(i1,i2+5,
     & i3+3,c3)+cr1*ui(i1+1,i2+5,i3+3,c3)+cr2*ui(i1+2,i2+5,i3+3,c3)+
     & cr3*ui(i1+3,i2+5,i3+3,c3)+cr4*ui(i1+4,i2+5,i3+3,c3)+cr5*ui(i1+
     & 5,i2+5,i3+3,c3)+cr6*ui(i1+6,i2+5,i3+3,c3))+cs6*(cr0*ui(i1,i2+6,
     & i3+3,c3)+cr1*ui(i1+1,i2+6,i3+3,c3)+cr2*ui(i1+2,i2+6,i3+3,c3)+
     & cr3*ui(i1+3,i2+6,i3+3,c3)+cr4*ui(i1+4,i2+6,i3+3,c3)+cr5*ui(i1+
     & 5,i2+6,i3+3,c3)+cr6*ui(i1+6,i2+6,i3+3,c3)))
               r(i) = r(i)+ct4*(cs0*(cr0*ui(i1,i2,i3+4,c3)+cr1*ui(i1+1,
     & i2,i3+4,c3)+cr2*ui(i1+2,i2,i3+4,c3)+cr3*ui(i1+3,i2,i3+4,c3)+
     & cr4*ui(i1+4,i2,i3+4,c3)+cr5*ui(i1+5,i2,i3+4,c3)+cr6*ui(i1+6,i2,
     & i3+4,c3))+cs1*(cr0*ui(i1,i2+1,i3+4,c3)+cr1*ui(i1+1,i2+1,i3+4,
     & c3)+cr2*ui(i1+2,i2+1,i3+4,c3)+cr3*ui(i1+3,i2+1,i3+4,c3)+cr4*ui(
     & i1+4,i2+1,i3+4,c3)+cr5*ui(i1+5,i2+1,i3+4,c3)+cr6*ui(i1+6,i2+1,
     & i3+4,c3))+cs2*(cr0*ui(i1,i2+2,i3+4,c3)+cr1*ui(i1+1,i2+2,i3+4,
     & c3)+cr2*ui(i1+2,i2+2,i3+4,c3)+cr3*ui(i1+3,i2+2,i3+4,c3)+cr4*ui(
     & i1+4,i2+2,i3+4,c3)+cr5*ui(i1+5,i2+2,i3+4,c3)+cr6*ui(i1+6,i2+2,
     & i3+4,c3))+cs3*(cr0*ui(i1,i2+3,i3+4,c3)+cr1*ui(i1+1,i2+3,i3+4,
     & c3)+cr2*ui(i1+2,i2+3,i3+4,c3)+cr3*ui(i1+3,i2+3,i3+4,c3)+cr4*ui(
     & i1+4,i2+3,i3+4,c3)+cr5*ui(i1+5,i2+3,i3+4,c3)+cr6*ui(i1+6,i2+3,
     & i3+4,c3))+cs4*(cr0*ui(i1,i2+4,i3+4,c3)+cr1*ui(i1+1,i2+4,i3+4,
     & c3)+cr2*ui(i1+2,i2+4,i3+4,c3)+cr3*ui(i1+3,i2+4,i3+4,c3)+cr4*ui(
     & i1+4,i2+4,i3+4,c3)+cr5*ui(i1+5,i2+4,i3+4,c3)+cr6*ui(i1+6,i2+4,
     & i3+4,c3))+cs5*(cr0*ui(i1,i2+5,i3+4,c3)+cr1*ui(i1+1,i2+5,i3+4,
     & c3)+cr2*ui(i1+2,i2+5,i3+4,c3)+cr3*ui(i1+3,i2+5,i3+4,c3)+cr4*ui(
     & i1+4,i2+5,i3+4,c3)+cr5*ui(i1+5,i2+5,i3+4,c3)+cr6*ui(i1+6,i2+5,
     & i3+4,c3))+cs6*(cr0*ui(i1,i2+6,i3+4,c3)+cr1*ui(i1+1,i2+6,i3+4,
     & c3)+cr2*ui(i1+2,i2+6,i3+4,c3)+cr3*ui(i1+3,i2+6,i3+4,c3)+cr4*ui(
     & i1+4,i2+6,i3+4,c3)+cr5*ui(i1+5,i2+6,i3+4,c3)+cr6*ui(i1+6,i2+6,
     & i3+4,c3)))+ct5*(cs0*(cr0*ui(i1,i2,i3+5,c3)+cr1*ui(i1+1,i2,i3+5,
     & c3)+cr2*ui(i1+2,i2,i3+5,c3)+cr3*ui(i1+3,i2,i3+5,c3)+cr4*ui(i1+
     & 4,i2,i3+5,c3)+cr5*ui(i1+5,i2,i3+5,c3)+cr6*ui(i1+6,i2,i3+5,c3))+
     & cs1*(cr0*ui(i1,i2+1,i3+5,c3)+cr1*ui(i1+1,i2+1,i3+5,c3)+cr2*ui(
     & i1+2,i2+1,i3+5,c3)+cr3*ui(i1+3,i2+1,i3+5,c3)+cr4*ui(i1+4,i2+1,
     & i3+5,c3)+cr5*ui(i1+5,i2+1,i3+5,c3)+cr6*ui(i1+6,i2+1,i3+5,c3))+
     & cs2*(cr0*ui(i1,i2+2,i3+5,c3)+cr1*ui(i1+1,i2+2,i3+5,c3)+cr2*ui(
     & i1+2,i2+2,i3+5,c3)+cr3*ui(i1+3,i2+2,i3+5,c3)+cr4*ui(i1+4,i2+2,
     & i3+5,c3)+cr5*ui(i1+5,i2+2,i3+5,c3)+cr6*ui(i1+6,i2+2,i3+5,c3))+
     & cs3*(cr0*ui(i1,i2+3,i3+5,c3)+cr1*ui(i1+1,i2+3,i3+5,c3)+cr2*ui(
     & i1+2,i2+3,i3+5,c3)+cr3*ui(i1+3,i2+3,i3+5,c3)+cr4*ui(i1+4,i2+3,
     & i3+5,c3)+cr5*ui(i1+5,i2+3,i3+5,c3)+cr6*ui(i1+6,i2+3,i3+5,c3))+
     & cs4*(cr0*ui(i1,i2+4,i3+5,c3)+cr1*ui(i1+1,i2+4,i3+5,c3)+cr2*ui(
     & i1+2,i2+4,i3+5,c3)+cr3*ui(i1+3,i2+4,i3+5,c3)+cr4*ui(i1+4,i2+4,
     & i3+5,c3)+cr5*ui(i1+5,i2+4,i3+5,c3)+cr6*ui(i1+6,i2+4,i3+5,c3))+
     & cs5*(cr0*ui(i1,i2+5,i3+5,c3)+cr1*ui(i1+1,i2+5,i3+5,c3)+cr2*ui(
     & i1+2,i2+5,i3+5,c3)+cr3*ui(i1+3,i2+5,i3+5,c3)+cr4*ui(i1+4,i2+5,
     & i3+5,c3)+cr5*ui(i1+5,i2+5,i3+5,c3)+cr6*ui(i1+6,i2+5,i3+5,c3))+
     & cs6*(cr0*ui(i1,i2+6,i3+5,c3)+cr1*ui(i1+1,i2+6,i3+5,c3)+cr2*ui(
     & i1+2,i2+6,i3+5,c3)+cr3*ui(i1+3,i2+6,i3+5,c3)+cr4*ui(i1+4,i2+6,
     & i3+5,c3)+cr5*ui(i1+5,i2+6,i3+5,c3)+cr6*ui(i1+6,i2+6,i3+5,c3)))+
     & ct6*(cs0*(cr0*ui(i1,i2,i3+6,c3)+cr1*ui(i1+1,i2,i3+6,c3)+cr2*ui(
     & i1+2,i2,i3+6,c3)+cr3*ui(i1+3,i2,i3+6,c3)+cr4*ui(i1+4,i2,i3+6,
     & c3)+cr5*ui(i1+5,i2,i3+6,c3)+cr6*ui(i1+6,i2,i3+6,c3))+cs1*(cr0*
     & ui(i1,i2+1,i3+6,c3)+cr1*ui(i1+1,i2+1,i3+6,c3)+cr2*ui(i1+2,i2+1,
     & i3+6,c3)+cr3*ui(i1+3,i2+1,i3+6,c3)+cr4*ui(i1+4,i2+1,i3+6,c3)+
     & cr5*ui(i1+5,i2+1,i3+6,c3)+cr6*ui(i1+6,i2+1,i3+6,c3))+cs2*(cr0*
     & ui(i1,i2+2,i3+6,c3)+cr1*ui(i1+1,i2+2,i3+6,c3)+cr2*ui(i1+2,i2+2,
     & i3+6,c3)+cr3*ui(i1+3,i2+2,i3+6,c3)+cr4*ui(i1+4,i2+2,i3+6,c3)+
     & cr5*ui(i1+5,i2+2,i3+6,c3)+cr6*ui(i1+6,i2+2,i3+6,c3))+cs3*(cr0*
     & ui(i1,i2+3,i3+6,c3)+cr1*ui(i1+1,i2+3,i3+6,c3)+cr2*ui(i1+2,i2+3,
     & i3+6,c3)+cr3*ui(i1+3,i2+3,i3+6,c3)+cr4*ui(i1+4,i2+3,i3+6,c3)+
     & cr5*ui(i1+5,i2+3,i3+6,c3)+cr6*ui(i1+6,i2+3,i3+6,c3))+cs4*(cr0*
     & ui(i1,i2+4,i3+6,c3)+cr1*ui(i1+1,i2+4,i3+6,c3)+cr2*ui(i1+2,i2+4,
     & i3+6,c3)+cr3*ui(i1+3,i2+4,i3+6,c3)+cr4*ui(i1+4,i2+4,i3+6,c3)+
     & cr5*ui(i1+5,i2+4,i3+6,c3)+cr6*ui(i1+6,i2+4,i3+6,c3))+cs5*(cr0*
     & ui(i1,i2+5,i3+6,c3)+cr1*ui(i1+1,i2+5,i3+6,c3)+cr2*ui(i1+2,i2+5,
     & i3+6,c3)+cr3*ui(i1+3,i2+5,i3+6,c3)+cr4*ui(i1+4,i2+5,i3+6,c3)+
     & cr5*ui(i1+5,i2+5,i3+6,c3)+cr6*ui(i1+6,i2+5,i3+6,c3))+cs6*(cr0*
     & ui(i1,i2+6,i3+6,c3)+cr1*ui(i1+1,i2+6,i3+6,c3)+cr2*ui(i1+2,i2+6,
     & i3+6,c3)+cr3*ui(i1+3,i2+6,i3+6,c3)+cr4*ui(i1+4,i2+6,i3+6,c3)+
     & cr5*ui(i1+5,i2+6,i3+6,c3)+cr6*ui(i1+6,i2+6,i3+6,c3)))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           end if
         else if( width(1).eq.8 .and. width(2).eq.8 .and. width(3)
     & .eq.8 )then
! loops3d($interpSparseStorage888(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))
           if( c3a.eq.c3b )then
             do c3=c3a,c3b
             do i=nia,nib
! interpSparseStorage888(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q80(c(i,0,0,0))
                 cs0 = q80(c(i,1,0,0))
                 ct0 = q80(c(i,2,0,0))
                 cr1 = q81(c(i,0,0,0))
                 cs1 = q81(c(i,1,0,0))
                 ct1 = q81(c(i,2,0,0))
                 cr2 = q82(c(i,0,0,0))
                 cs2 = q82(c(i,1,0,0))
                 ct2 = q82(c(i,2,0,0))
                 cr3 = q83(c(i,0,0,0))
                 cs3 = q83(c(i,1,0,0))
                 ct3 = q83(c(i,2,0,0))
                 cr4 = q84(c(i,0,0,0))
                 cs4 = q84(c(i,1,0,0))
                 ct4 = q84(c(i,2,0,0))
                 cr5 = q85(c(i,0,0,0))
                 cs5 = q85(c(i,1,0,0))
                 ct5 = q85(c(i,2,0,0))
                 cr6 = q86(c(i,0,0,0))
                 cs6 = q86(c(i,1,0,0))
                 ct6 = q86(c(i,2,0,0))
                 cr7 = q87(c(i,0,0,0))
                 cs7 = q87(c(i,1,0,0))
                 ct7 = q87(c(i,2,0,0))
               r(i) = ct0*(cs0*(cr0*ui(i1,i2,i3+0,c3)+cr1*ui(i1+1,i2,
     & i3+0,c3)+cr2*ui(i1+2,i2,i3+0,c3)+cr3*ui(i1+3,i2,i3+0,c3)+cr4*
     & ui(i1+4,i2,i3+0,c3)+cr5*ui(i1+5,i2,i3+0,c3)+cr6*ui(i1+6,i2,i3+
     & 0,c3)+cr7*ui(i1+7,i2,i3+0,c3))+cs1*(cr0*ui(i1,i2+1,i3+0,c3)+
     & cr1*ui(i1+1,i2+1,i3+0,c3)+cr2*ui(i1+2,i2+1,i3+0,c3)+cr3*ui(i1+
     & 3,i2+1,i3+0,c3)+cr4*ui(i1+4,i2+1,i3+0,c3)+cr5*ui(i1+5,i2+1,i3+
     & 0,c3)+cr6*ui(i1+6,i2+1,i3+0,c3)+cr7*ui(i1+7,i2+1,i3+0,c3))+cs2*
     & (cr0*ui(i1,i2+2,i3+0,c3)+cr1*ui(i1+1,i2+2,i3+0,c3)+cr2*ui(i1+2,
     & i2+2,i3+0,c3)+cr3*ui(i1+3,i2+2,i3+0,c3)+cr4*ui(i1+4,i2+2,i3+0,
     & c3)+cr5*ui(i1+5,i2+2,i3+0,c3)+cr6*ui(i1+6,i2+2,i3+0,c3)+cr7*ui(
     & i1+7,i2+2,i3+0,c3))+cs3*(cr0*ui(i1,i2+3,i3+0,c3)+cr1*ui(i1+1,
     & i2+3,i3+0,c3)+cr2*ui(i1+2,i2+3,i3+0,c3)+cr3*ui(i1+3,i2+3,i3+0,
     & c3)+cr4*ui(i1+4,i2+3,i3+0,c3)+cr5*ui(i1+5,i2+3,i3+0,c3)+cr6*ui(
     & i1+6,i2+3,i3+0,c3)+cr7*ui(i1+7,i2+3,i3+0,c3))+cs4*(cr0*ui(i1,
     & i2+4,i3+0,c3)+cr1*ui(i1+1,i2+4,i3+0,c3)+cr2*ui(i1+2,i2+4,i3+0,
     & c3)+cr3*ui(i1+3,i2+4,i3+0,c3)+cr4*ui(i1+4,i2+4,i3+0,c3)+cr5*ui(
     & i1+5,i2+4,i3+0,c3)+cr6*ui(i1+6,i2+4,i3+0,c3)+cr7*ui(i1+7,i2+4,
     & i3+0,c3))+cs5*(cr0*ui(i1,i2+5,i3+0,c3)+cr1*ui(i1+1,i2+5,i3+0,
     & c3)+cr2*ui(i1+2,i2+5,i3+0,c3)+cr3*ui(i1+3,i2+5,i3+0,c3)+cr4*ui(
     & i1+4,i2+5,i3+0,c3)+cr5*ui(i1+5,i2+5,i3+0,c3)+cr6*ui(i1+6,i2+5,
     & i3+0,c3)+cr7*ui(i1+7,i2+5,i3+0,c3))+cs6*(cr0*ui(i1,i2+6,i3+0,
     & c3)+cr1*ui(i1+1,i2+6,i3+0,c3)+cr2*ui(i1+2,i2+6,i3+0,c3)+cr3*ui(
     & i1+3,i2+6,i3+0,c3)+cr4*ui(i1+4,i2+6,i3+0,c3)+cr5*ui(i1+5,i2+6,
     & i3+0,c3)+cr6*ui(i1+6,i2+6,i3+0,c3)+cr7*ui(i1+7,i2+6,i3+0,c3))+
     & cs7*(cr0*ui(i1,i2+7,i3+0,c3)+cr1*ui(i1+1,i2+7,i3+0,c3)+cr2*ui(
     & i1+2,i2+7,i3+0,c3)+cr3*ui(i1+3,i2+7,i3+0,c3)+cr4*ui(i1+4,i2+7,
     & i3+0,c3)+cr5*ui(i1+5,i2+7,i3+0,c3)+cr6*ui(i1+6,i2+7,i3+0,c3)+
     & cr7*ui(i1+7,i2+7,i3+0,c3)))+ct1*(cs0*(cr0*ui(i1,i2,i3+1,c3)+
     & cr1*ui(i1+1,i2,i3+1,c3)+cr2*ui(i1+2,i2,i3+1,c3)+cr3*ui(i1+3,i2,
     & i3+1,c3)+cr4*ui(i1+4,i2,i3+1,c3)+cr5*ui(i1+5,i2,i3+1,c3)+cr6*
     & ui(i1+6,i2,i3+1,c3)+cr7*ui(i1+7,i2,i3+1,c3))+cs1*(cr0*ui(i1,i2+
     & 1,i3+1,c3)+cr1*ui(i1+1,i2+1,i3+1,c3)+cr2*ui(i1+2,i2+1,i3+1,c3)+
     & cr3*ui(i1+3,i2+1,i3+1,c3)+cr4*ui(i1+4,i2+1,i3+1,c3)+cr5*ui(i1+
     & 5,i2+1,i3+1,c3)+cr6*ui(i1+6,i2+1,i3+1,c3)+cr7*ui(i1+7,i2+1,i3+
     & 1,c3))+cs2*(cr0*ui(i1,i2+2,i3+1,c3)+cr1*ui(i1+1,i2+2,i3+1,c3)+
     & cr2*ui(i1+2,i2+2,i3+1,c3)+cr3*ui(i1+3,i2+2,i3+1,c3)+cr4*ui(i1+
     & 4,i2+2,i3+1,c3)+cr5*ui(i1+5,i2+2,i3+1,c3)+cr6*ui(i1+6,i2+2,i3+
     & 1,c3)+cr7*ui(i1+7,i2+2,i3+1,c3))+cs3*(cr0*ui(i1,i2+3,i3+1,c3)+
     & cr1*ui(i1+1,i2+3,i3+1,c3)+cr2*ui(i1+2,i2+3,i3+1,c3)+cr3*ui(i1+
     & 3,i2+3,i3+1,c3)+cr4*ui(i1+4,i2+3,i3+1,c3)+cr5*ui(i1+5,i2+3,i3+
     & 1,c3)+cr6*ui(i1+6,i2+3,i3+1,c3)+cr7*ui(i1+7,i2+3,i3+1,c3))+cs4*
     & (cr0*ui(i1,i2+4,i3+1,c3)+cr1*ui(i1+1,i2+4,i3+1,c3)+cr2*ui(i1+2,
     & i2+4,i3+1,c3)+cr3*ui(i1+3,i2+4,i3+1,c3)+cr4*ui(i1+4,i2+4,i3+1,
     & c3)+cr5*ui(i1+5,i2+4,i3+1,c3)+cr6*ui(i1+6,i2+4,i3+1,c3)+cr7*ui(
     & i1+7,i2+4,i3+1,c3))+cs5*(cr0*ui(i1,i2+5,i3+1,c3)+cr1*ui(i1+1,
     & i2+5,i3+1,c3)+cr2*ui(i1+2,i2+5,i3+1,c3)+cr3*ui(i1+3,i2+5,i3+1,
     & c3)+cr4*ui(i1+4,i2+5,i3+1,c3)+cr5*ui(i1+5,i2+5,i3+1,c3)+cr6*ui(
     & i1+6,i2+5,i3+1,c3)+cr7*ui(i1+7,i2+5,i3+1,c3))+cs6*(cr0*ui(i1,
     & i2+6,i3+1,c3)+cr1*ui(i1+1,i2+6,i3+1,c3)+cr2*ui(i1+2,i2+6,i3+1,
     & c3)+cr3*ui(i1+3,i2+6,i3+1,c3)+cr4*ui(i1+4,i2+6,i3+1,c3)+cr5*ui(
     & i1+5,i2+6,i3+1,c3)+cr6*ui(i1+6,i2+6,i3+1,c3)+cr7*ui(i1+7,i2+6,
     & i3+1,c3))+cs7*(cr0*ui(i1,i2+7,i3+1,c3)+cr1*ui(i1+1,i2+7,i3+1,
     & c3)+cr2*ui(i1+2,i2+7,i3+1,c3)+cr3*ui(i1+3,i2+7,i3+1,c3)+cr4*ui(
     & i1+4,i2+7,i3+1,c3)+cr5*ui(i1+5,i2+7,i3+1,c3)+cr6*ui(i1+6,i2+7,
     & i3+1,c3)+cr7*ui(i1+7,i2+7,i3+1,c3)))+ct2*(cs0*(cr0*ui(i1,i2,i3+
     & 2,c3)+cr1*ui(i1+1,i2,i3+2,c3)+cr2*ui(i1+2,i2,i3+2,c3)+cr3*ui(
     & i1+3,i2,i3+2,c3)+cr4*ui(i1+4,i2,i3+2,c3)+cr5*ui(i1+5,i2,i3+2,
     & c3)+cr6*ui(i1+6,i2,i3+2,c3)+cr7*ui(i1+7,i2,i3+2,c3))+cs1*(cr0*
     & ui(i1,i2+1,i3+2,c3)+cr1*ui(i1+1,i2+1,i3+2,c3)+cr2*ui(i1+2,i2+1,
     & i3+2,c3)+cr3*ui(i1+3,i2+1,i3+2,c3)+cr4*ui(i1+4,i2+1,i3+2,c3)+
     & cr5*ui(i1+5,i2+1,i3+2,c3)+cr6*ui(i1+6,i2+1,i3+2,c3)+cr7*ui(i1+
     & 7,i2+1,i3+2,c3))+cs2*(cr0*ui(i1,i2+2,i3+2,c3)+cr1*ui(i1+1,i2+2,
     & i3+2,c3)+cr2*ui(i1+2,i2+2,i3+2,c3)+cr3*ui(i1+3,i2+2,i3+2,c3)+
     & cr4*ui(i1+4,i2+2,i3+2,c3)+cr5*ui(i1+5,i2+2,i3+2,c3)+cr6*ui(i1+
     & 6,i2+2,i3+2,c3)+cr7*ui(i1+7,i2+2,i3+2,c3))+cs3*(cr0*ui(i1,i2+3,
     & i3+2,c3)+cr1*ui(i1+1,i2+3,i3+2,c3)+cr2*ui(i1+2,i2+3,i3+2,c3)+
     & cr3*ui(i1+3,i2+3,i3+2,c3)+cr4*ui(i1+4,i2+3,i3+2,c3)+cr5*ui(i1+
     & 5,i2+3,i3+2,c3)+cr6*ui(i1+6,i2+3,i3+2,c3)+cr7*ui(i1+7,i2+3,i3+
     & 2,c3))+cs4*(cr0*ui(i1,i2+4,i3+2,c3)+cr1*ui(i1+1,i2+4,i3+2,c3)+
     & cr2*ui(i1+2,i2+4,i3+2,c3)+cr3*ui(i1+3,i2+4,i3+2,c3)+cr4*ui(i1+
     & 4,i2+4,i3+2,c3)+cr5*ui(i1+5,i2+4,i3+2,c3)+cr6*ui(i1+6,i2+4,i3+
     & 2,c3)+cr7*ui(i1+7,i2+4,i3+2,c3))+cs5*(cr0*ui(i1,i2+5,i3+2,c3)+
     & cr1*ui(i1+1,i2+5,i3+2,c3)+cr2*ui(i1+2,i2+5,i3+2,c3)+cr3*ui(i1+
     & 3,i2+5,i3+2,c3)+cr4*ui(i1+4,i2+5,i3+2,c3)+cr5*ui(i1+5,i2+5,i3+
     & 2,c3)+cr6*ui(i1+6,i2+5,i3+2,c3)+cr7*ui(i1+7,i2+5,i3+2,c3))+cs6*
     & (cr0*ui(i1,i2+6,i3+2,c3)+cr1*ui(i1+1,i2+6,i3+2,c3)+cr2*ui(i1+2,
     & i2+6,i3+2,c3)+cr3*ui(i1+3,i2+6,i3+2,c3)+cr4*ui(i1+4,i2+6,i3+2,
     & c3)+cr5*ui(i1+5,i2+6,i3+2,c3)+cr6*ui(i1+6,i2+6,i3+2,c3)+cr7*ui(
     & i1+7,i2+6,i3+2,c3))+cs7*(cr0*ui(i1,i2+7,i3+2,c3)+cr1*ui(i1+1,
     & i2+7,i3+2,c3)+cr2*ui(i1+2,i2+7,i3+2,c3)+cr3*ui(i1+3,i2+7,i3+2,
     & c3)+cr4*ui(i1+4,i2+7,i3+2,c3)+cr5*ui(i1+5,i2+7,i3+2,c3)+cr6*ui(
     & i1+6,i2+7,i3+2,c3)+cr7*ui(i1+7,i2+7,i3+2,c3)))
               r(i) = r(i)+ct3*(cs0*(cr0*ui(i1,i2,i3+3,c3)+cr1*ui(i1+1,
     & i2,i3+3,c3)+cr2*ui(i1+2,i2,i3+3,c3)+cr3*ui(i1+3,i2,i3+3,c3)+
     & cr4*ui(i1+4,i2,i3+3,c3)+cr5*ui(i1+5,i2,i3+3,c3)+cr6*ui(i1+6,i2,
     & i3+3,c3)+cr7*ui(i1+7,i2,i3+3,c3))+cs1*(cr0*ui(i1,i2+1,i3+3,c3)+
     & cr1*ui(i1+1,i2+1,i3+3,c3)+cr2*ui(i1+2,i2+1,i3+3,c3)+cr3*ui(i1+
     & 3,i2+1,i3+3,c3)+cr4*ui(i1+4,i2+1,i3+3,c3)+cr5*ui(i1+5,i2+1,i3+
     & 3,c3)+cr6*ui(i1+6,i2+1,i3+3,c3)+cr7*ui(i1+7,i2+1,i3+3,c3))+cs2*
     & (cr0*ui(i1,i2+2,i3+3,c3)+cr1*ui(i1+1,i2+2,i3+3,c3)+cr2*ui(i1+2,
     & i2+2,i3+3,c3)+cr3*ui(i1+3,i2+2,i3+3,c3)+cr4*ui(i1+4,i2+2,i3+3,
     & c3)+cr5*ui(i1+5,i2+2,i3+3,c3)+cr6*ui(i1+6,i2+2,i3+3,c3)+cr7*ui(
     & i1+7,i2+2,i3+3,c3))+cs3*(cr0*ui(i1,i2+3,i3+3,c3)+cr1*ui(i1+1,
     & i2+3,i3+3,c3)+cr2*ui(i1+2,i2+3,i3+3,c3)+cr3*ui(i1+3,i2+3,i3+3,
     & c3)+cr4*ui(i1+4,i2+3,i3+3,c3)+cr5*ui(i1+5,i2+3,i3+3,c3)+cr6*ui(
     & i1+6,i2+3,i3+3,c3)+cr7*ui(i1+7,i2+3,i3+3,c3))+cs4*(cr0*ui(i1,
     & i2+4,i3+3,c3)+cr1*ui(i1+1,i2+4,i3+3,c3)+cr2*ui(i1+2,i2+4,i3+3,
     & c3)+cr3*ui(i1+3,i2+4,i3+3,c3)+cr4*ui(i1+4,i2+4,i3+3,c3)+cr5*ui(
     & i1+5,i2+4,i3+3,c3)+cr6*ui(i1+6,i2+4,i3+3,c3)+cr7*ui(i1+7,i2+4,
     & i3+3,c3))+cs5*(cr0*ui(i1,i2+5,i3+3,c3)+cr1*ui(i1+1,i2+5,i3+3,
     & c3)+cr2*ui(i1+2,i2+5,i3+3,c3)+cr3*ui(i1+3,i2+5,i3+3,c3)+cr4*ui(
     & i1+4,i2+5,i3+3,c3)+cr5*ui(i1+5,i2+5,i3+3,c3)+cr6*ui(i1+6,i2+5,
     & i3+3,c3)+cr7*ui(i1+7,i2+5,i3+3,c3))+cs6*(cr0*ui(i1,i2+6,i3+3,
     & c3)+cr1*ui(i1+1,i2+6,i3+3,c3)+cr2*ui(i1+2,i2+6,i3+3,c3)+cr3*ui(
     & i1+3,i2+6,i3+3,c3)+cr4*ui(i1+4,i2+6,i3+3,c3)+cr5*ui(i1+5,i2+6,
     & i3+3,c3)+cr6*ui(i1+6,i2+6,i3+3,c3)+cr7*ui(i1+7,i2+6,i3+3,c3))+
     & cs7*(cr0*ui(i1,i2+7,i3+3,c3)+cr1*ui(i1+1,i2+7,i3+3,c3)+cr2*ui(
     & i1+2,i2+7,i3+3,c3)+cr3*ui(i1+3,i2+7,i3+3,c3)+cr4*ui(i1+4,i2+7,
     & i3+3,c3)+cr5*ui(i1+5,i2+7,i3+3,c3)+cr6*ui(i1+6,i2+7,i3+3,c3)+
     & cr7*ui(i1+7,i2+7,i3+3,c3)))+ct4*(cs0*(cr0*ui(i1,i2,i3+4,c3)+
     & cr1*ui(i1+1,i2,i3+4,c3)+cr2*ui(i1+2,i2,i3+4,c3)+cr3*ui(i1+3,i2,
     & i3+4,c3)+cr4*ui(i1+4,i2,i3+4,c3)+cr5*ui(i1+5,i2,i3+4,c3)+cr6*
     & ui(i1+6,i2,i3+4,c3)+cr7*ui(i1+7,i2,i3+4,c3))+cs1*(cr0*ui(i1,i2+
     & 1,i3+4,c3)+cr1*ui(i1+1,i2+1,i3+4,c3)+cr2*ui(i1+2,i2+1,i3+4,c3)+
     & cr3*ui(i1+3,i2+1,i3+4,c3)+cr4*ui(i1+4,i2+1,i3+4,c3)+cr5*ui(i1+
     & 5,i2+1,i3+4,c3)+cr6*ui(i1+6,i2+1,i3+4,c3)+cr7*ui(i1+7,i2+1,i3+
     & 4,c3))+cs2*(cr0*ui(i1,i2+2,i3+4,c3)+cr1*ui(i1+1,i2+2,i3+4,c3)+
     & cr2*ui(i1+2,i2+2,i3+4,c3)+cr3*ui(i1+3,i2+2,i3+4,c3)+cr4*ui(i1+
     & 4,i2+2,i3+4,c3)+cr5*ui(i1+5,i2+2,i3+4,c3)+cr6*ui(i1+6,i2+2,i3+
     & 4,c3)+cr7*ui(i1+7,i2+2,i3+4,c3))+cs3*(cr0*ui(i1,i2+3,i3+4,c3)+
     & cr1*ui(i1+1,i2+3,i3+4,c3)+cr2*ui(i1+2,i2+3,i3+4,c3)+cr3*ui(i1+
     & 3,i2+3,i3+4,c3)+cr4*ui(i1+4,i2+3,i3+4,c3)+cr5*ui(i1+5,i2+3,i3+
     & 4,c3)+cr6*ui(i1+6,i2+3,i3+4,c3)+cr7*ui(i1+7,i2+3,i3+4,c3))+cs4*
     & (cr0*ui(i1,i2+4,i3+4,c3)+cr1*ui(i1+1,i2+4,i3+4,c3)+cr2*ui(i1+2,
     & i2+4,i3+4,c3)+cr3*ui(i1+3,i2+4,i3+4,c3)+cr4*ui(i1+4,i2+4,i3+4,
     & c3)+cr5*ui(i1+5,i2+4,i3+4,c3)+cr6*ui(i1+6,i2+4,i3+4,c3)+cr7*ui(
     & i1+7,i2+4,i3+4,c3))+cs5*(cr0*ui(i1,i2+5,i3+4,c3)+cr1*ui(i1+1,
     & i2+5,i3+4,c3)+cr2*ui(i1+2,i2+5,i3+4,c3)+cr3*ui(i1+3,i2+5,i3+4,
     & c3)+cr4*ui(i1+4,i2+5,i3+4,c3)+cr5*ui(i1+5,i2+5,i3+4,c3)+cr6*ui(
     & i1+6,i2+5,i3+4,c3)+cr7*ui(i1+7,i2+5,i3+4,c3))+cs6*(cr0*ui(i1,
     & i2+6,i3+4,c3)+cr1*ui(i1+1,i2+6,i3+4,c3)+cr2*ui(i1+2,i2+6,i3+4,
     & c3)+cr3*ui(i1+3,i2+6,i3+4,c3)+cr4*ui(i1+4,i2+6,i3+4,c3)+cr5*ui(
     & i1+5,i2+6,i3+4,c3)+cr6*ui(i1+6,i2+6,i3+4,c3)+cr7*ui(i1+7,i2+6,
     & i3+4,c3))+cs7*(cr0*ui(i1,i2+7,i3+4,c3)+cr1*ui(i1+1,i2+7,i3+4,
     & c3)+cr2*ui(i1+2,i2+7,i3+4,c3)+cr3*ui(i1+3,i2+7,i3+4,c3)+cr4*ui(
     & i1+4,i2+7,i3+4,c3)+cr5*ui(i1+5,i2+7,i3+4,c3)+cr6*ui(i1+6,i2+7,
     & i3+4,c3)+cr7*ui(i1+7,i2+7,i3+4,c3)))+ct5*(cs0*(cr0*ui(i1,i2,i3+
     & 5,c3)+cr1*ui(i1+1,i2,i3+5,c3)+cr2*ui(i1+2,i2,i3+5,c3)+cr3*ui(
     & i1+3,i2,i3+5,c3)+cr4*ui(i1+4,i2,i3+5,c3)+cr5*ui(i1+5,i2,i3+5,
     & c3)+cr6*ui(i1+6,i2,i3+5,c3)+cr7*ui(i1+7,i2,i3+5,c3))+cs1*(cr0*
     & ui(i1,i2+1,i3+5,c3)+cr1*ui(i1+1,i2+1,i3+5,c3)+cr2*ui(i1+2,i2+1,
     & i3+5,c3)+cr3*ui(i1+3,i2+1,i3+5,c3)+cr4*ui(i1+4,i2+1,i3+5,c3)+
     & cr5*ui(i1+5,i2+1,i3+5,c3)+cr6*ui(i1+6,i2+1,i3+5,c3)+cr7*ui(i1+
     & 7,i2+1,i3+5,c3))+cs2*(cr0*ui(i1,i2+2,i3+5,c3)+cr1*ui(i1+1,i2+2,
     & i3+5,c3)+cr2*ui(i1+2,i2+2,i3+5,c3)+cr3*ui(i1+3,i2+2,i3+5,c3)+
     & cr4*ui(i1+4,i2+2,i3+5,c3)+cr5*ui(i1+5,i2+2,i3+5,c3)+cr6*ui(i1+
     & 6,i2+2,i3+5,c3)+cr7*ui(i1+7,i2+2,i3+5,c3))+cs3*(cr0*ui(i1,i2+3,
     & i3+5,c3)+cr1*ui(i1+1,i2+3,i3+5,c3)+cr2*ui(i1+2,i2+3,i3+5,c3)+
     & cr3*ui(i1+3,i2+3,i3+5,c3)+cr4*ui(i1+4,i2+3,i3+5,c3)+cr5*ui(i1+
     & 5,i2+3,i3+5,c3)+cr6*ui(i1+6,i2+3,i3+5,c3)+cr7*ui(i1+7,i2+3,i3+
     & 5,c3))+cs4*(cr0*ui(i1,i2+4,i3+5,c3)+cr1*ui(i1+1,i2+4,i3+5,c3)+
     & cr2*ui(i1+2,i2+4,i3+5,c3)+cr3*ui(i1+3,i2+4,i3+5,c3)+cr4*ui(i1+
     & 4,i2+4,i3+5,c3)+cr5*ui(i1+5,i2+4,i3+5,c3)+cr6*ui(i1+6,i2+4,i3+
     & 5,c3)+cr7*ui(i1+7,i2+4,i3+5,c3))+cs5*(cr0*ui(i1,i2+5,i3+5,c3)+
     & cr1*ui(i1+1,i2+5,i3+5,c3)+cr2*ui(i1+2,i2+5,i3+5,c3)+cr3*ui(i1+
     & 3,i2+5,i3+5,c3)+cr4*ui(i1+4,i2+5,i3+5,c3)+cr5*ui(i1+5,i2+5,i3+
     & 5,c3)+cr6*ui(i1+6,i2+5,i3+5,c3)+cr7*ui(i1+7,i2+5,i3+5,c3))+cs6*
     & (cr0*ui(i1,i2+6,i3+5,c3)+cr1*ui(i1+1,i2+6,i3+5,c3)+cr2*ui(i1+2,
     & i2+6,i3+5,c3)+cr3*ui(i1+3,i2+6,i3+5,c3)+cr4*ui(i1+4,i2+6,i3+5,
     & c3)+cr5*ui(i1+5,i2+6,i3+5,c3)+cr6*ui(i1+6,i2+6,i3+5,c3)+cr7*ui(
     & i1+7,i2+6,i3+5,c3))+cs7*(cr0*ui(i1,i2+7,i3+5,c3)+cr1*ui(i1+1,
     & i2+7,i3+5,c3)+cr2*ui(i1+2,i2+7,i3+5,c3)+cr3*ui(i1+3,i2+7,i3+5,
     & c3)+cr4*ui(i1+4,i2+7,i3+5,c3)+cr5*ui(i1+5,i2+7,i3+5,c3)+cr6*ui(
     & i1+6,i2+7,i3+5,c3)+cr7*ui(i1+7,i2+7,i3+5,c3)))
               r(i) = r(i)+ct6*(cs0*(cr0*ui(i1,i2,i3+6,c3)+cr1*ui(i1+1,
     & i2,i3+6,c3)+cr2*ui(i1+2,i2,i3+6,c3)+cr3*ui(i1+3,i2,i3+6,c3)+
     & cr4*ui(i1+4,i2,i3+6,c3)+cr5*ui(i1+5,i2,i3+6,c3)+cr6*ui(i1+6,i2,
     & i3+6,c3)+cr7*ui(i1+7,i2,i3+6,c3))+cs1*(cr0*ui(i1,i2+1,i3+6,c3)+
     & cr1*ui(i1+1,i2+1,i3+6,c3)+cr2*ui(i1+2,i2+1,i3+6,c3)+cr3*ui(i1+
     & 3,i2+1,i3+6,c3)+cr4*ui(i1+4,i2+1,i3+6,c3)+cr5*ui(i1+5,i2+1,i3+
     & 6,c3)+cr6*ui(i1+6,i2+1,i3+6,c3)+cr7*ui(i1+7,i2+1,i3+6,c3))+cs2*
     & (cr0*ui(i1,i2+2,i3+6,c3)+cr1*ui(i1+1,i2+2,i3+6,c3)+cr2*ui(i1+2,
     & i2+2,i3+6,c3)+cr3*ui(i1+3,i2+2,i3+6,c3)+cr4*ui(i1+4,i2+2,i3+6,
     & c3)+cr5*ui(i1+5,i2+2,i3+6,c3)+cr6*ui(i1+6,i2+2,i3+6,c3)+cr7*ui(
     & i1+7,i2+2,i3+6,c3))+cs3*(cr0*ui(i1,i2+3,i3+6,c3)+cr1*ui(i1+1,
     & i2+3,i3+6,c3)+cr2*ui(i1+2,i2+3,i3+6,c3)+cr3*ui(i1+3,i2+3,i3+6,
     & c3)+cr4*ui(i1+4,i2+3,i3+6,c3)+cr5*ui(i1+5,i2+3,i3+6,c3)+cr6*ui(
     & i1+6,i2+3,i3+6,c3)+cr7*ui(i1+7,i2+3,i3+6,c3))+cs4*(cr0*ui(i1,
     & i2+4,i3+6,c3)+cr1*ui(i1+1,i2+4,i3+6,c3)+cr2*ui(i1+2,i2+4,i3+6,
     & c3)+cr3*ui(i1+3,i2+4,i3+6,c3)+cr4*ui(i1+4,i2+4,i3+6,c3)+cr5*ui(
     & i1+5,i2+4,i3+6,c3)+cr6*ui(i1+6,i2+4,i3+6,c3)+cr7*ui(i1+7,i2+4,
     & i3+6,c3))+cs5*(cr0*ui(i1,i2+5,i3+6,c3)+cr1*ui(i1+1,i2+5,i3+6,
     & c3)+cr2*ui(i1+2,i2+5,i3+6,c3)+cr3*ui(i1+3,i2+5,i3+6,c3)+cr4*ui(
     & i1+4,i2+5,i3+6,c3)+cr5*ui(i1+5,i2+5,i3+6,c3)+cr6*ui(i1+6,i2+5,
     & i3+6,c3)+cr7*ui(i1+7,i2+5,i3+6,c3))+cs6*(cr0*ui(i1,i2+6,i3+6,
     & c3)+cr1*ui(i1+1,i2+6,i3+6,c3)+cr2*ui(i1+2,i2+6,i3+6,c3)+cr3*ui(
     & i1+3,i2+6,i3+6,c3)+cr4*ui(i1+4,i2+6,i3+6,c3)+cr5*ui(i1+5,i2+6,
     & i3+6,c3)+cr6*ui(i1+6,i2+6,i3+6,c3)+cr7*ui(i1+7,i2+6,i3+6,c3))+
     & cs7*(cr0*ui(i1,i2+7,i3+6,c3)+cr1*ui(i1+1,i2+7,i3+6,c3)+cr2*ui(
     & i1+2,i2+7,i3+6,c3)+cr3*ui(i1+3,i2+7,i3+6,c3)+cr4*ui(i1+4,i2+7,
     & i3+6,c3)+cr5*ui(i1+5,i2+7,i3+6,c3)+cr6*ui(i1+6,i2+7,i3+6,c3)+
     & cr7*ui(i1+7,i2+7,i3+6,c3)))+ct7*(cs0*(cr0*ui(i1,i2,i3+7,c3)+
     & cr1*ui(i1+1,i2,i3+7,c3)+cr2*ui(i1+2,i2,i3+7,c3)+cr3*ui(i1+3,i2,
     & i3+7,c3)+cr4*ui(i1+4,i2,i3+7,c3)+cr5*ui(i1+5,i2,i3+7,c3)+cr6*
     & ui(i1+6,i2,i3+7,c3)+cr7*ui(i1+7,i2,i3+7,c3))+cs1*(cr0*ui(i1,i2+
     & 1,i3+7,c3)+cr1*ui(i1+1,i2+1,i3+7,c3)+cr2*ui(i1+2,i2+1,i3+7,c3)+
     & cr3*ui(i1+3,i2+1,i3+7,c3)+cr4*ui(i1+4,i2+1,i3+7,c3)+cr5*ui(i1+
     & 5,i2+1,i3+7,c3)+cr6*ui(i1+6,i2+1,i3+7,c3)+cr7*ui(i1+7,i2+1,i3+
     & 7,c3))+cs2*(cr0*ui(i1,i2+2,i3+7,c3)+cr1*ui(i1+1,i2+2,i3+7,c3)+
     & cr2*ui(i1+2,i2+2,i3+7,c3)+cr3*ui(i1+3,i2+2,i3+7,c3)+cr4*ui(i1+
     & 4,i2+2,i3+7,c3)+cr5*ui(i1+5,i2+2,i3+7,c3)+cr6*ui(i1+6,i2+2,i3+
     & 7,c3)+cr7*ui(i1+7,i2+2,i3+7,c3))+cs3*(cr0*ui(i1,i2+3,i3+7,c3)+
     & cr1*ui(i1+1,i2+3,i3+7,c3)+cr2*ui(i1+2,i2+3,i3+7,c3)+cr3*ui(i1+
     & 3,i2+3,i3+7,c3)+cr4*ui(i1+4,i2+3,i3+7,c3)+cr5*ui(i1+5,i2+3,i3+
     & 7,c3)+cr6*ui(i1+6,i2+3,i3+7,c3)+cr7*ui(i1+7,i2+3,i3+7,c3))+cs4*
     & (cr0*ui(i1,i2+4,i3+7,c3)+cr1*ui(i1+1,i2+4,i3+7,c3)+cr2*ui(i1+2,
     & i2+4,i3+7,c3)+cr3*ui(i1+3,i2+4,i3+7,c3)+cr4*ui(i1+4,i2+4,i3+7,
     & c3)+cr5*ui(i1+5,i2+4,i3+7,c3)+cr6*ui(i1+6,i2+4,i3+7,c3)+cr7*ui(
     & i1+7,i2+4,i3+7,c3))+cs5*(cr0*ui(i1,i2+5,i3+7,c3)+cr1*ui(i1+1,
     & i2+5,i3+7,c3)+cr2*ui(i1+2,i2+5,i3+7,c3)+cr3*ui(i1+3,i2+5,i3+7,
     & c3)+cr4*ui(i1+4,i2+5,i3+7,c3)+cr5*ui(i1+5,i2+5,i3+7,c3)+cr6*ui(
     & i1+6,i2+5,i3+7,c3)+cr7*ui(i1+7,i2+5,i3+7,c3))+cs6*(cr0*ui(i1,
     & i2+6,i3+7,c3)+cr1*ui(i1+1,i2+6,i3+7,c3)+cr2*ui(i1+2,i2+6,i3+7,
     & c3)+cr3*ui(i1+3,i2+6,i3+7,c3)+cr4*ui(i1+4,i2+6,i3+7,c3)+cr5*ui(
     & i1+5,i2+6,i3+7,c3)+cr6*ui(i1+6,i2+6,i3+7,c3)+cr7*ui(i1+7,i2+6,
     & i3+7,c3))+cs7*(cr0*ui(i1,i2+7,i3+7,c3)+cr1*ui(i1+1,i2+7,i3+7,
     & c3)+cr2*ui(i1+2,i2+7,i3+7,c3)+cr3*ui(i1+3,i2+7,i3+7,c3)+cr4*ui(
     & i1+4,i2+7,i3+7,c3)+cr5*ui(i1+5,i2+7,i3+7,c3)+cr6*ui(i1+6,i2+7,
     & i3+7,c3)+cr7*ui(i1+7,i2+7,i3+7,c3)))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
! interpSparseStorage888(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q80(c(i,0,0,0))
                 cs0 = q80(c(i,1,0,0))
                 ct0 = q80(c(i,2,0,0))
                 cr1 = q81(c(i,0,0,0))
                 cs1 = q81(c(i,1,0,0))
                 ct1 = q81(c(i,2,0,0))
                 cr2 = q82(c(i,0,0,0))
                 cs2 = q82(c(i,1,0,0))
                 ct2 = q82(c(i,2,0,0))
                 cr3 = q83(c(i,0,0,0))
                 cs3 = q83(c(i,1,0,0))
                 ct3 = q83(c(i,2,0,0))
                 cr4 = q84(c(i,0,0,0))
                 cs4 = q84(c(i,1,0,0))
                 ct4 = q84(c(i,2,0,0))
                 cr5 = q85(c(i,0,0,0))
                 cs5 = q85(c(i,1,0,0))
                 ct5 = q85(c(i,2,0,0))
                 cr6 = q86(c(i,0,0,0))
                 cs6 = q86(c(i,1,0,0))
                 ct6 = q86(c(i,2,0,0))
                 cr7 = q87(c(i,0,0,0))
                 cs7 = q87(c(i,1,0,0))
                 ct7 = q87(c(i,2,0,0))
               r(i) = ct0*(cs0*(cr0*ui(i1,i2,i3+0,c3)+cr1*ui(i1+1,i2,
     & i3+0,c3)+cr2*ui(i1+2,i2,i3+0,c3)+cr3*ui(i1+3,i2,i3+0,c3)+cr4*
     & ui(i1+4,i2,i3+0,c3)+cr5*ui(i1+5,i2,i3+0,c3)+cr6*ui(i1+6,i2,i3+
     & 0,c3)+cr7*ui(i1+7,i2,i3+0,c3))+cs1*(cr0*ui(i1,i2+1,i3+0,c3)+
     & cr1*ui(i1+1,i2+1,i3+0,c3)+cr2*ui(i1+2,i2+1,i3+0,c3)+cr3*ui(i1+
     & 3,i2+1,i3+0,c3)+cr4*ui(i1+4,i2+1,i3+0,c3)+cr5*ui(i1+5,i2+1,i3+
     & 0,c3)+cr6*ui(i1+6,i2+1,i3+0,c3)+cr7*ui(i1+7,i2+1,i3+0,c3))+cs2*
     & (cr0*ui(i1,i2+2,i3+0,c3)+cr1*ui(i1+1,i2+2,i3+0,c3)+cr2*ui(i1+2,
     & i2+2,i3+0,c3)+cr3*ui(i1+3,i2+2,i3+0,c3)+cr4*ui(i1+4,i2+2,i3+0,
     & c3)+cr5*ui(i1+5,i2+2,i3+0,c3)+cr6*ui(i1+6,i2+2,i3+0,c3)+cr7*ui(
     & i1+7,i2+2,i3+0,c3))+cs3*(cr0*ui(i1,i2+3,i3+0,c3)+cr1*ui(i1+1,
     & i2+3,i3+0,c3)+cr2*ui(i1+2,i2+3,i3+0,c3)+cr3*ui(i1+3,i2+3,i3+0,
     & c3)+cr4*ui(i1+4,i2+3,i3+0,c3)+cr5*ui(i1+5,i2+3,i3+0,c3)+cr6*ui(
     & i1+6,i2+3,i3+0,c3)+cr7*ui(i1+7,i2+3,i3+0,c3))+cs4*(cr0*ui(i1,
     & i2+4,i3+0,c3)+cr1*ui(i1+1,i2+4,i3+0,c3)+cr2*ui(i1+2,i2+4,i3+0,
     & c3)+cr3*ui(i1+3,i2+4,i3+0,c3)+cr4*ui(i1+4,i2+4,i3+0,c3)+cr5*ui(
     & i1+5,i2+4,i3+0,c3)+cr6*ui(i1+6,i2+4,i3+0,c3)+cr7*ui(i1+7,i2+4,
     & i3+0,c3))+cs5*(cr0*ui(i1,i2+5,i3+0,c3)+cr1*ui(i1+1,i2+5,i3+0,
     & c3)+cr2*ui(i1+2,i2+5,i3+0,c3)+cr3*ui(i1+3,i2+5,i3+0,c3)+cr4*ui(
     & i1+4,i2+5,i3+0,c3)+cr5*ui(i1+5,i2+5,i3+0,c3)+cr6*ui(i1+6,i2+5,
     & i3+0,c3)+cr7*ui(i1+7,i2+5,i3+0,c3))+cs6*(cr0*ui(i1,i2+6,i3+0,
     & c3)+cr1*ui(i1+1,i2+6,i3+0,c3)+cr2*ui(i1+2,i2+6,i3+0,c3)+cr3*ui(
     & i1+3,i2+6,i3+0,c3)+cr4*ui(i1+4,i2+6,i3+0,c3)+cr5*ui(i1+5,i2+6,
     & i3+0,c3)+cr6*ui(i1+6,i2+6,i3+0,c3)+cr7*ui(i1+7,i2+6,i3+0,c3))+
     & cs7*(cr0*ui(i1,i2+7,i3+0,c3)+cr1*ui(i1+1,i2+7,i3+0,c3)+cr2*ui(
     & i1+2,i2+7,i3+0,c3)+cr3*ui(i1+3,i2+7,i3+0,c3)+cr4*ui(i1+4,i2+7,
     & i3+0,c3)+cr5*ui(i1+5,i2+7,i3+0,c3)+cr6*ui(i1+6,i2+7,i3+0,c3)+
     & cr7*ui(i1+7,i2+7,i3+0,c3)))+ct1*(cs0*(cr0*ui(i1,i2,i3+1,c3)+
     & cr1*ui(i1+1,i2,i3+1,c3)+cr2*ui(i1+2,i2,i3+1,c3)+cr3*ui(i1+3,i2,
     & i3+1,c3)+cr4*ui(i1+4,i2,i3+1,c3)+cr5*ui(i1+5,i2,i3+1,c3)+cr6*
     & ui(i1+6,i2,i3+1,c3)+cr7*ui(i1+7,i2,i3+1,c3))+cs1*(cr0*ui(i1,i2+
     & 1,i3+1,c3)+cr1*ui(i1+1,i2+1,i3+1,c3)+cr2*ui(i1+2,i2+1,i3+1,c3)+
     & cr3*ui(i1+3,i2+1,i3+1,c3)+cr4*ui(i1+4,i2+1,i3+1,c3)+cr5*ui(i1+
     & 5,i2+1,i3+1,c3)+cr6*ui(i1+6,i2+1,i3+1,c3)+cr7*ui(i1+7,i2+1,i3+
     & 1,c3))+cs2*(cr0*ui(i1,i2+2,i3+1,c3)+cr1*ui(i1+1,i2+2,i3+1,c3)+
     & cr2*ui(i1+2,i2+2,i3+1,c3)+cr3*ui(i1+3,i2+2,i3+1,c3)+cr4*ui(i1+
     & 4,i2+2,i3+1,c3)+cr5*ui(i1+5,i2+2,i3+1,c3)+cr6*ui(i1+6,i2+2,i3+
     & 1,c3)+cr7*ui(i1+7,i2+2,i3+1,c3))+cs3*(cr0*ui(i1,i2+3,i3+1,c3)+
     & cr1*ui(i1+1,i2+3,i3+1,c3)+cr2*ui(i1+2,i2+3,i3+1,c3)+cr3*ui(i1+
     & 3,i2+3,i3+1,c3)+cr4*ui(i1+4,i2+3,i3+1,c3)+cr5*ui(i1+5,i2+3,i3+
     & 1,c3)+cr6*ui(i1+6,i2+3,i3+1,c3)+cr7*ui(i1+7,i2+3,i3+1,c3))+cs4*
     & (cr0*ui(i1,i2+4,i3+1,c3)+cr1*ui(i1+1,i2+4,i3+1,c3)+cr2*ui(i1+2,
     & i2+4,i3+1,c3)+cr3*ui(i1+3,i2+4,i3+1,c3)+cr4*ui(i1+4,i2+4,i3+1,
     & c3)+cr5*ui(i1+5,i2+4,i3+1,c3)+cr6*ui(i1+6,i2+4,i3+1,c3)+cr7*ui(
     & i1+7,i2+4,i3+1,c3))+cs5*(cr0*ui(i1,i2+5,i3+1,c3)+cr1*ui(i1+1,
     & i2+5,i3+1,c3)+cr2*ui(i1+2,i2+5,i3+1,c3)+cr3*ui(i1+3,i2+5,i3+1,
     & c3)+cr4*ui(i1+4,i2+5,i3+1,c3)+cr5*ui(i1+5,i2+5,i3+1,c3)+cr6*ui(
     & i1+6,i2+5,i3+1,c3)+cr7*ui(i1+7,i2+5,i3+1,c3))+cs6*(cr0*ui(i1,
     & i2+6,i3+1,c3)+cr1*ui(i1+1,i2+6,i3+1,c3)+cr2*ui(i1+2,i2+6,i3+1,
     & c3)+cr3*ui(i1+3,i2+6,i3+1,c3)+cr4*ui(i1+4,i2+6,i3+1,c3)+cr5*ui(
     & i1+5,i2+6,i3+1,c3)+cr6*ui(i1+6,i2+6,i3+1,c3)+cr7*ui(i1+7,i2+6,
     & i3+1,c3))+cs7*(cr0*ui(i1,i2+7,i3+1,c3)+cr1*ui(i1+1,i2+7,i3+1,
     & c3)+cr2*ui(i1+2,i2+7,i3+1,c3)+cr3*ui(i1+3,i2+7,i3+1,c3)+cr4*ui(
     & i1+4,i2+7,i3+1,c3)+cr5*ui(i1+5,i2+7,i3+1,c3)+cr6*ui(i1+6,i2+7,
     & i3+1,c3)+cr7*ui(i1+7,i2+7,i3+1,c3)))+ct2*(cs0*(cr0*ui(i1,i2,i3+
     & 2,c3)+cr1*ui(i1+1,i2,i3+2,c3)+cr2*ui(i1+2,i2,i3+2,c3)+cr3*ui(
     & i1+3,i2,i3+2,c3)+cr4*ui(i1+4,i2,i3+2,c3)+cr5*ui(i1+5,i2,i3+2,
     & c3)+cr6*ui(i1+6,i2,i3+2,c3)+cr7*ui(i1+7,i2,i3+2,c3))+cs1*(cr0*
     & ui(i1,i2+1,i3+2,c3)+cr1*ui(i1+1,i2+1,i3+2,c3)+cr2*ui(i1+2,i2+1,
     & i3+2,c3)+cr3*ui(i1+3,i2+1,i3+2,c3)+cr4*ui(i1+4,i2+1,i3+2,c3)+
     & cr5*ui(i1+5,i2+1,i3+2,c3)+cr6*ui(i1+6,i2+1,i3+2,c3)+cr7*ui(i1+
     & 7,i2+1,i3+2,c3))+cs2*(cr0*ui(i1,i2+2,i3+2,c3)+cr1*ui(i1+1,i2+2,
     & i3+2,c3)+cr2*ui(i1+2,i2+2,i3+2,c3)+cr3*ui(i1+3,i2+2,i3+2,c3)+
     & cr4*ui(i1+4,i2+2,i3+2,c3)+cr5*ui(i1+5,i2+2,i3+2,c3)+cr6*ui(i1+
     & 6,i2+2,i3+2,c3)+cr7*ui(i1+7,i2+2,i3+2,c3))+cs3*(cr0*ui(i1,i2+3,
     & i3+2,c3)+cr1*ui(i1+1,i2+3,i3+2,c3)+cr2*ui(i1+2,i2+3,i3+2,c3)+
     & cr3*ui(i1+3,i2+3,i3+2,c3)+cr4*ui(i1+4,i2+3,i3+2,c3)+cr5*ui(i1+
     & 5,i2+3,i3+2,c3)+cr6*ui(i1+6,i2+3,i3+2,c3)+cr7*ui(i1+7,i2+3,i3+
     & 2,c3))+cs4*(cr0*ui(i1,i2+4,i3+2,c3)+cr1*ui(i1+1,i2+4,i3+2,c3)+
     & cr2*ui(i1+2,i2+4,i3+2,c3)+cr3*ui(i1+3,i2+4,i3+2,c3)+cr4*ui(i1+
     & 4,i2+4,i3+2,c3)+cr5*ui(i1+5,i2+4,i3+2,c3)+cr6*ui(i1+6,i2+4,i3+
     & 2,c3)+cr7*ui(i1+7,i2+4,i3+2,c3))+cs5*(cr0*ui(i1,i2+5,i3+2,c3)+
     & cr1*ui(i1+1,i2+5,i3+2,c3)+cr2*ui(i1+2,i2+5,i3+2,c3)+cr3*ui(i1+
     & 3,i2+5,i3+2,c3)+cr4*ui(i1+4,i2+5,i3+2,c3)+cr5*ui(i1+5,i2+5,i3+
     & 2,c3)+cr6*ui(i1+6,i2+5,i3+2,c3)+cr7*ui(i1+7,i2+5,i3+2,c3))+cs6*
     & (cr0*ui(i1,i2+6,i3+2,c3)+cr1*ui(i1+1,i2+6,i3+2,c3)+cr2*ui(i1+2,
     & i2+6,i3+2,c3)+cr3*ui(i1+3,i2+6,i3+2,c3)+cr4*ui(i1+4,i2+6,i3+2,
     & c3)+cr5*ui(i1+5,i2+6,i3+2,c3)+cr6*ui(i1+6,i2+6,i3+2,c3)+cr7*ui(
     & i1+7,i2+6,i3+2,c3))+cs7*(cr0*ui(i1,i2+7,i3+2,c3)+cr1*ui(i1+1,
     & i2+7,i3+2,c3)+cr2*ui(i1+2,i2+7,i3+2,c3)+cr3*ui(i1+3,i2+7,i3+2,
     & c3)+cr4*ui(i1+4,i2+7,i3+2,c3)+cr5*ui(i1+5,i2+7,i3+2,c3)+cr6*ui(
     & i1+6,i2+7,i3+2,c3)+cr7*ui(i1+7,i2+7,i3+2,c3)))
               r(i) = r(i)+ct3*(cs0*(cr0*ui(i1,i2,i3+3,c3)+cr1*ui(i1+1,
     & i2,i3+3,c3)+cr2*ui(i1+2,i2,i3+3,c3)+cr3*ui(i1+3,i2,i3+3,c3)+
     & cr4*ui(i1+4,i2,i3+3,c3)+cr5*ui(i1+5,i2,i3+3,c3)+cr6*ui(i1+6,i2,
     & i3+3,c3)+cr7*ui(i1+7,i2,i3+3,c3))+cs1*(cr0*ui(i1,i2+1,i3+3,c3)+
     & cr1*ui(i1+1,i2+1,i3+3,c3)+cr2*ui(i1+2,i2+1,i3+3,c3)+cr3*ui(i1+
     & 3,i2+1,i3+3,c3)+cr4*ui(i1+4,i2+1,i3+3,c3)+cr5*ui(i1+5,i2+1,i3+
     & 3,c3)+cr6*ui(i1+6,i2+1,i3+3,c3)+cr7*ui(i1+7,i2+1,i3+3,c3))+cs2*
     & (cr0*ui(i1,i2+2,i3+3,c3)+cr1*ui(i1+1,i2+2,i3+3,c3)+cr2*ui(i1+2,
     & i2+2,i3+3,c3)+cr3*ui(i1+3,i2+2,i3+3,c3)+cr4*ui(i1+4,i2+2,i3+3,
     & c3)+cr5*ui(i1+5,i2+2,i3+3,c3)+cr6*ui(i1+6,i2+2,i3+3,c3)+cr7*ui(
     & i1+7,i2+2,i3+3,c3))+cs3*(cr0*ui(i1,i2+3,i3+3,c3)+cr1*ui(i1+1,
     & i2+3,i3+3,c3)+cr2*ui(i1+2,i2+3,i3+3,c3)+cr3*ui(i1+3,i2+3,i3+3,
     & c3)+cr4*ui(i1+4,i2+3,i3+3,c3)+cr5*ui(i1+5,i2+3,i3+3,c3)+cr6*ui(
     & i1+6,i2+3,i3+3,c3)+cr7*ui(i1+7,i2+3,i3+3,c3))+cs4*(cr0*ui(i1,
     & i2+4,i3+3,c3)+cr1*ui(i1+1,i2+4,i3+3,c3)+cr2*ui(i1+2,i2+4,i3+3,
     & c3)+cr3*ui(i1+3,i2+4,i3+3,c3)+cr4*ui(i1+4,i2+4,i3+3,c3)+cr5*ui(
     & i1+5,i2+4,i3+3,c3)+cr6*ui(i1+6,i2+4,i3+3,c3)+cr7*ui(i1+7,i2+4,
     & i3+3,c3))+cs5*(cr0*ui(i1,i2+5,i3+3,c3)+cr1*ui(i1+1,i2+5,i3+3,
     & c3)+cr2*ui(i1+2,i2+5,i3+3,c3)+cr3*ui(i1+3,i2+5,i3+3,c3)+cr4*ui(
     & i1+4,i2+5,i3+3,c3)+cr5*ui(i1+5,i2+5,i3+3,c3)+cr6*ui(i1+6,i2+5,
     & i3+3,c3)+cr7*ui(i1+7,i2+5,i3+3,c3))+cs6*(cr0*ui(i1,i2+6,i3+3,
     & c3)+cr1*ui(i1+1,i2+6,i3+3,c3)+cr2*ui(i1+2,i2+6,i3+3,c3)+cr3*ui(
     & i1+3,i2+6,i3+3,c3)+cr4*ui(i1+4,i2+6,i3+3,c3)+cr5*ui(i1+5,i2+6,
     & i3+3,c3)+cr6*ui(i1+6,i2+6,i3+3,c3)+cr7*ui(i1+7,i2+6,i3+3,c3))+
     & cs7*(cr0*ui(i1,i2+7,i3+3,c3)+cr1*ui(i1+1,i2+7,i3+3,c3)+cr2*ui(
     & i1+2,i2+7,i3+3,c3)+cr3*ui(i1+3,i2+7,i3+3,c3)+cr4*ui(i1+4,i2+7,
     & i3+3,c3)+cr5*ui(i1+5,i2+7,i3+3,c3)+cr6*ui(i1+6,i2+7,i3+3,c3)+
     & cr7*ui(i1+7,i2+7,i3+3,c3)))+ct4*(cs0*(cr0*ui(i1,i2,i3+4,c3)+
     & cr1*ui(i1+1,i2,i3+4,c3)+cr2*ui(i1+2,i2,i3+4,c3)+cr3*ui(i1+3,i2,
     & i3+4,c3)+cr4*ui(i1+4,i2,i3+4,c3)+cr5*ui(i1+5,i2,i3+4,c3)+cr6*
     & ui(i1+6,i2,i3+4,c3)+cr7*ui(i1+7,i2,i3+4,c3))+cs1*(cr0*ui(i1,i2+
     & 1,i3+4,c3)+cr1*ui(i1+1,i2+1,i3+4,c3)+cr2*ui(i1+2,i2+1,i3+4,c3)+
     & cr3*ui(i1+3,i2+1,i3+4,c3)+cr4*ui(i1+4,i2+1,i3+4,c3)+cr5*ui(i1+
     & 5,i2+1,i3+4,c3)+cr6*ui(i1+6,i2+1,i3+4,c3)+cr7*ui(i1+7,i2+1,i3+
     & 4,c3))+cs2*(cr0*ui(i1,i2+2,i3+4,c3)+cr1*ui(i1+1,i2+2,i3+4,c3)+
     & cr2*ui(i1+2,i2+2,i3+4,c3)+cr3*ui(i1+3,i2+2,i3+4,c3)+cr4*ui(i1+
     & 4,i2+2,i3+4,c3)+cr5*ui(i1+5,i2+2,i3+4,c3)+cr6*ui(i1+6,i2+2,i3+
     & 4,c3)+cr7*ui(i1+7,i2+2,i3+4,c3))+cs3*(cr0*ui(i1,i2+3,i3+4,c3)+
     & cr1*ui(i1+1,i2+3,i3+4,c3)+cr2*ui(i1+2,i2+3,i3+4,c3)+cr3*ui(i1+
     & 3,i2+3,i3+4,c3)+cr4*ui(i1+4,i2+3,i3+4,c3)+cr5*ui(i1+5,i2+3,i3+
     & 4,c3)+cr6*ui(i1+6,i2+3,i3+4,c3)+cr7*ui(i1+7,i2+3,i3+4,c3))+cs4*
     & (cr0*ui(i1,i2+4,i3+4,c3)+cr1*ui(i1+1,i2+4,i3+4,c3)+cr2*ui(i1+2,
     & i2+4,i3+4,c3)+cr3*ui(i1+3,i2+4,i3+4,c3)+cr4*ui(i1+4,i2+4,i3+4,
     & c3)+cr5*ui(i1+5,i2+4,i3+4,c3)+cr6*ui(i1+6,i2+4,i3+4,c3)+cr7*ui(
     & i1+7,i2+4,i3+4,c3))+cs5*(cr0*ui(i1,i2+5,i3+4,c3)+cr1*ui(i1+1,
     & i2+5,i3+4,c3)+cr2*ui(i1+2,i2+5,i3+4,c3)+cr3*ui(i1+3,i2+5,i3+4,
     & c3)+cr4*ui(i1+4,i2+5,i3+4,c3)+cr5*ui(i1+5,i2+5,i3+4,c3)+cr6*ui(
     & i1+6,i2+5,i3+4,c3)+cr7*ui(i1+7,i2+5,i3+4,c3))+cs6*(cr0*ui(i1,
     & i2+6,i3+4,c3)+cr1*ui(i1+1,i2+6,i3+4,c3)+cr2*ui(i1+2,i2+6,i3+4,
     & c3)+cr3*ui(i1+3,i2+6,i3+4,c3)+cr4*ui(i1+4,i2+6,i3+4,c3)+cr5*ui(
     & i1+5,i2+6,i3+4,c3)+cr6*ui(i1+6,i2+6,i3+4,c3)+cr7*ui(i1+7,i2+6,
     & i3+4,c3))+cs7*(cr0*ui(i1,i2+7,i3+4,c3)+cr1*ui(i1+1,i2+7,i3+4,
     & c3)+cr2*ui(i1+2,i2+7,i3+4,c3)+cr3*ui(i1+3,i2+7,i3+4,c3)+cr4*ui(
     & i1+4,i2+7,i3+4,c3)+cr5*ui(i1+5,i2+7,i3+4,c3)+cr6*ui(i1+6,i2+7,
     & i3+4,c3)+cr7*ui(i1+7,i2+7,i3+4,c3)))+ct5*(cs0*(cr0*ui(i1,i2,i3+
     & 5,c3)+cr1*ui(i1+1,i2,i3+5,c3)+cr2*ui(i1+2,i2,i3+5,c3)+cr3*ui(
     & i1+3,i2,i3+5,c3)+cr4*ui(i1+4,i2,i3+5,c3)+cr5*ui(i1+5,i2,i3+5,
     & c3)+cr6*ui(i1+6,i2,i3+5,c3)+cr7*ui(i1+7,i2,i3+5,c3))+cs1*(cr0*
     & ui(i1,i2+1,i3+5,c3)+cr1*ui(i1+1,i2+1,i3+5,c3)+cr2*ui(i1+2,i2+1,
     & i3+5,c3)+cr3*ui(i1+3,i2+1,i3+5,c3)+cr4*ui(i1+4,i2+1,i3+5,c3)+
     & cr5*ui(i1+5,i2+1,i3+5,c3)+cr6*ui(i1+6,i2+1,i3+5,c3)+cr7*ui(i1+
     & 7,i2+1,i3+5,c3))+cs2*(cr0*ui(i1,i2+2,i3+5,c3)+cr1*ui(i1+1,i2+2,
     & i3+5,c3)+cr2*ui(i1+2,i2+2,i3+5,c3)+cr3*ui(i1+3,i2+2,i3+5,c3)+
     & cr4*ui(i1+4,i2+2,i3+5,c3)+cr5*ui(i1+5,i2+2,i3+5,c3)+cr6*ui(i1+
     & 6,i2+2,i3+5,c3)+cr7*ui(i1+7,i2+2,i3+5,c3))+cs3*(cr0*ui(i1,i2+3,
     & i3+5,c3)+cr1*ui(i1+1,i2+3,i3+5,c3)+cr2*ui(i1+2,i2+3,i3+5,c3)+
     & cr3*ui(i1+3,i2+3,i3+5,c3)+cr4*ui(i1+4,i2+3,i3+5,c3)+cr5*ui(i1+
     & 5,i2+3,i3+5,c3)+cr6*ui(i1+6,i2+3,i3+5,c3)+cr7*ui(i1+7,i2+3,i3+
     & 5,c3))+cs4*(cr0*ui(i1,i2+4,i3+5,c3)+cr1*ui(i1+1,i2+4,i3+5,c3)+
     & cr2*ui(i1+2,i2+4,i3+5,c3)+cr3*ui(i1+3,i2+4,i3+5,c3)+cr4*ui(i1+
     & 4,i2+4,i3+5,c3)+cr5*ui(i1+5,i2+4,i3+5,c3)+cr6*ui(i1+6,i2+4,i3+
     & 5,c3)+cr7*ui(i1+7,i2+4,i3+5,c3))+cs5*(cr0*ui(i1,i2+5,i3+5,c3)+
     & cr1*ui(i1+1,i2+5,i3+5,c3)+cr2*ui(i1+2,i2+5,i3+5,c3)+cr3*ui(i1+
     & 3,i2+5,i3+5,c3)+cr4*ui(i1+4,i2+5,i3+5,c3)+cr5*ui(i1+5,i2+5,i3+
     & 5,c3)+cr6*ui(i1+6,i2+5,i3+5,c3)+cr7*ui(i1+7,i2+5,i3+5,c3))+cs6*
     & (cr0*ui(i1,i2+6,i3+5,c3)+cr1*ui(i1+1,i2+6,i3+5,c3)+cr2*ui(i1+2,
     & i2+6,i3+5,c3)+cr3*ui(i1+3,i2+6,i3+5,c3)+cr4*ui(i1+4,i2+6,i3+5,
     & c3)+cr5*ui(i1+5,i2+6,i3+5,c3)+cr6*ui(i1+6,i2+6,i3+5,c3)+cr7*ui(
     & i1+7,i2+6,i3+5,c3))+cs7*(cr0*ui(i1,i2+7,i3+5,c3)+cr1*ui(i1+1,
     & i2+7,i3+5,c3)+cr2*ui(i1+2,i2+7,i3+5,c3)+cr3*ui(i1+3,i2+7,i3+5,
     & c3)+cr4*ui(i1+4,i2+7,i3+5,c3)+cr5*ui(i1+5,i2+7,i3+5,c3)+cr6*ui(
     & i1+6,i2+7,i3+5,c3)+cr7*ui(i1+7,i2+7,i3+5,c3)))
               r(i) = r(i)+ct6*(cs0*(cr0*ui(i1,i2,i3+6,c3)+cr1*ui(i1+1,
     & i2,i3+6,c3)+cr2*ui(i1+2,i2,i3+6,c3)+cr3*ui(i1+3,i2,i3+6,c3)+
     & cr4*ui(i1+4,i2,i3+6,c3)+cr5*ui(i1+5,i2,i3+6,c3)+cr6*ui(i1+6,i2,
     & i3+6,c3)+cr7*ui(i1+7,i2,i3+6,c3))+cs1*(cr0*ui(i1,i2+1,i3+6,c3)+
     & cr1*ui(i1+1,i2+1,i3+6,c3)+cr2*ui(i1+2,i2+1,i3+6,c3)+cr3*ui(i1+
     & 3,i2+1,i3+6,c3)+cr4*ui(i1+4,i2+1,i3+6,c3)+cr5*ui(i1+5,i2+1,i3+
     & 6,c3)+cr6*ui(i1+6,i2+1,i3+6,c3)+cr7*ui(i1+7,i2+1,i3+6,c3))+cs2*
     & (cr0*ui(i1,i2+2,i3+6,c3)+cr1*ui(i1+1,i2+2,i3+6,c3)+cr2*ui(i1+2,
     & i2+2,i3+6,c3)+cr3*ui(i1+3,i2+2,i3+6,c3)+cr4*ui(i1+4,i2+2,i3+6,
     & c3)+cr5*ui(i1+5,i2+2,i3+6,c3)+cr6*ui(i1+6,i2+2,i3+6,c3)+cr7*ui(
     & i1+7,i2+2,i3+6,c3))+cs3*(cr0*ui(i1,i2+3,i3+6,c3)+cr1*ui(i1+1,
     & i2+3,i3+6,c3)+cr2*ui(i1+2,i2+3,i3+6,c3)+cr3*ui(i1+3,i2+3,i3+6,
     & c3)+cr4*ui(i1+4,i2+3,i3+6,c3)+cr5*ui(i1+5,i2+3,i3+6,c3)+cr6*ui(
     & i1+6,i2+3,i3+6,c3)+cr7*ui(i1+7,i2+3,i3+6,c3))+cs4*(cr0*ui(i1,
     & i2+4,i3+6,c3)+cr1*ui(i1+1,i2+4,i3+6,c3)+cr2*ui(i1+2,i2+4,i3+6,
     & c3)+cr3*ui(i1+3,i2+4,i3+6,c3)+cr4*ui(i1+4,i2+4,i3+6,c3)+cr5*ui(
     & i1+5,i2+4,i3+6,c3)+cr6*ui(i1+6,i2+4,i3+6,c3)+cr7*ui(i1+7,i2+4,
     & i3+6,c3))+cs5*(cr0*ui(i1,i2+5,i3+6,c3)+cr1*ui(i1+1,i2+5,i3+6,
     & c3)+cr2*ui(i1+2,i2+5,i3+6,c3)+cr3*ui(i1+3,i2+5,i3+6,c3)+cr4*ui(
     & i1+4,i2+5,i3+6,c3)+cr5*ui(i1+5,i2+5,i3+6,c3)+cr6*ui(i1+6,i2+5,
     & i3+6,c3)+cr7*ui(i1+7,i2+5,i3+6,c3))+cs6*(cr0*ui(i1,i2+6,i3+6,
     & c3)+cr1*ui(i1+1,i2+6,i3+6,c3)+cr2*ui(i1+2,i2+6,i3+6,c3)+cr3*ui(
     & i1+3,i2+6,i3+6,c3)+cr4*ui(i1+4,i2+6,i3+6,c3)+cr5*ui(i1+5,i2+6,
     & i3+6,c3)+cr6*ui(i1+6,i2+6,i3+6,c3)+cr7*ui(i1+7,i2+6,i3+6,c3))+
     & cs7*(cr0*ui(i1,i2+7,i3+6,c3)+cr1*ui(i1+1,i2+7,i3+6,c3)+cr2*ui(
     & i1+2,i2+7,i3+6,c3)+cr3*ui(i1+3,i2+7,i3+6,c3)+cr4*ui(i1+4,i2+7,
     & i3+6,c3)+cr5*ui(i1+5,i2+7,i3+6,c3)+cr6*ui(i1+6,i2+7,i3+6,c3)+
     & cr7*ui(i1+7,i2+7,i3+6,c3)))+ct7*(cs0*(cr0*ui(i1,i2,i3+7,c3)+
     & cr1*ui(i1+1,i2,i3+7,c3)+cr2*ui(i1+2,i2,i3+7,c3)+cr3*ui(i1+3,i2,
     & i3+7,c3)+cr4*ui(i1+4,i2,i3+7,c3)+cr5*ui(i1+5,i2,i3+7,c3)+cr6*
     & ui(i1+6,i2,i3+7,c3)+cr7*ui(i1+7,i2,i3+7,c3))+cs1*(cr0*ui(i1,i2+
     & 1,i3+7,c3)+cr1*ui(i1+1,i2+1,i3+7,c3)+cr2*ui(i1+2,i2+1,i3+7,c3)+
     & cr3*ui(i1+3,i2+1,i3+7,c3)+cr4*ui(i1+4,i2+1,i3+7,c3)+cr5*ui(i1+
     & 5,i2+1,i3+7,c3)+cr6*ui(i1+6,i2+1,i3+7,c3)+cr7*ui(i1+7,i2+1,i3+
     & 7,c3))+cs2*(cr0*ui(i1,i2+2,i3+7,c3)+cr1*ui(i1+1,i2+2,i3+7,c3)+
     & cr2*ui(i1+2,i2+2,i3+7,c3)+cr3*ui(i1+3,i2+2,i3+7,c3)+cr4*ui(i1+
     & 4,i2+2,i3+7,c3)+cr5*ui(i1+5,i2+2,i3+7,c3)+cr6*ui(i1+6,i2+2,i3+
     & 7,c3)+cr7*ui(i1+7,i2+2,i3+7,c3))+cs3*(cr0*ui(i1,i2+3,i3+7,c3)+
     & cr1*ui(i1+1,i2+3,i3+7,c3)+cr2*ui(i1+2,i2+3,i3+7,c3)+cr3*ui(i1+
     & 3,i2+3,i3+7,c3)+cr4*ui(i1+4,i2+3,i3+7,c3)+cr5*ui(i1+5,i2+3,i3+
     & 7,c3)+cr6*ui(i1+6,i2+3,i3+7,c3)+cr7*ui(i1+7,i2+3,i3+7,c3))+cs4*
     & (cr0*ui(i1,i2+4,i3+7,c3)+cr1*ui(i1+1,i2+4,i3+7,c3)+cr2*ui(i1+2,
     & i2+4,i3+7,c3)+cr3*ui(i1+3,i2+4,i3+7,c3)+cr4*ui(i1+4,i2+4,i3+7,
     & c3)+cr5*ui(i1+5,i2+4,i3+7,c3)+cr6*ui(i1+6,i2+4,i3+7,c3)+cr7*ui(
     & i1+7,i2+4,i3+7,c3))+cs5*(cr0*ui(i1,i2+5,i3+7,c3)+cr1*ui(i1+1,
     & i2+5,i3+7,c3)+cr2*ui(i1+2,i2+5,i3+7,c3)+cr3*ui(i1+3,i2+5,i3+7,
     & c3)+cr4*ui(i1+4,i2+5,i3+7,c3)+cr5*ui(i1+5,i2+5,i3+7,c3)+cr6*ui(
     & i1+6,i2+5,i3+7,c3)+cr7*ui(i1+7,i2+5,i3+7,c3))+cs6*(cr0*ui(i1,
     & i2+6,i3+7,c3)+cr1*ui(i1+1,i2+6,i3+7,c3)+cr2*ui(i1+2,i2+6,i3+7,
     & c3)+cr3*ui(i1+3,i2+6,i3+7,c3)+cr4*ui(i1+4,i2+6,i3+7,c3)+cr5*ui(
     & i1+5,i2+6,i3+7,c3)+cr6*ui(i1+6,i2+6,i3+7,c3)+cr7*ui(i1+7,i2+6,
     & i3+7,c3))+cs7*(cr0*ui(i1,i2+7,i3+7,c3)+cr1*ui(i1+1,i2+7,i3+7,
     & c3)+cr2*ui(i1+2,i2+7,i3+7,c3)+cr3*ui(i1+3,i2+7,i3+7,c3)+cr4*ui(
     & i1+4,i2+7,i3+7,c3)+cr5*ui(i1+5,i2+7,i3+7,c3)+cr6*ui(i1+6,i2+7,
     & i3+7,c3)+cr7*ui(i1+7,i2+7,i3+7,c3)))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           end if
         else if( width(1).eq.9 .and. width(2).eq.9 .and. width(3)
     & .eq.9 )then
! loops3d($interpSparseStorage999(r(i)),resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))
           if( c3a.eq.c3b )then
             do c3=c3a,c3b
             do i=nia,nib
! interpSparseStorage999(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q90(c(i,0,0,0))
                 cs0 = q90(c(i,1,0,0))
                 ct0 = q90(c(i,2,0,0))
                 cr1 = q91(c(i,0,0,0))
                 cs1 = q91(c(i,1,0,0))
                 ct1 = q91(c(i,2,0,0))
                 cr2 = q92(c(i,0,0,0))
                 cs2 = q92(c(i,1,0,0))
                 ct2 = q92(c(i,2,0,0))
                 cr3 = q93(c(i,0,0,0))
                 cs3 = q93(c(i,1,0,0))
                 ct3 = q93(c(i,2,0,0))
                 cr4 = q94(c(i,0,0,0))
                 cs4 = q94(c(i,1,0,0))
                 ct4 = q94(c(i,2,0,0))
                 cr5 = q95(c(i,0,0,0))
                 cs5 = q95(c(i,1,0,0))
                 ct5 = q95(c(i,2,0,0))
                 cr6 = q96(c(i,0,0,0))
                 cs6 = q96(c(i,1,0,0))
                 ct6 = q96(c(i,2,0,0))
                 cr7 = q97(c(i,0,0,0))
                 cs7 = q97(c(i,1,0,0))
                 ct7 = q97(c(i,2,0,0))
                 cr8 = q98(c(i,0,0,0))
                 cs8 = q98(c(i,1,0,0))
                 ct8 = q98(c(i,2,0,0))
               r(i) = ct0*(cs0*(cr0*ui(i1,i2,i3+0,c3)+cr1*ui(i1+1,i2,
     & i3+0,c3)+cr2*ui(i1+2,i2,i3+0,c3)+cr3*ui(i1+3,i2,i3+0,c3)+cr4*
     & ui(i1+4,i2,i3+0,c3)+cr5*ui(i1+5,i2,i3+0,c3)+cr6*ui(i1+6,i2,i3+
     & 0,c3)+cr7*ui(i1+7,i2,i3+0,c3)+cr8*ui(i1+8,i2,i3+0,c3))+cs1*(
     & cr0*ui(i1,i2+1,i3+0,c3)+cr1*ui(i1+1,i2+1,i3+0,c3)+cr2*ui(i1+2,
     & i2+1,i3+0,c3)+cr3*ui(i1+3,i2+1,i3+0,c3)+cr4*ui(i1+4,i2+1,i3+0,
     & c3)+cr5*ui(i1+5,i2+1,i3+0,c3)+cr6*ui(i1+6,i2+1,i3+0,c3)+cr7*ui(
     & i1+7,i2+1,i3+0,c3)+cr8*ui(i1+8,i2+1,i3+0,c3))+cs2*(cr0*ui(i1,
     & i2+2,i3+0,c3)+cr1*ui(i1+1,i2+2,i3+0,c3)+cr2*ui(i1+2,i2+2,i3+0,
     & c3)+cr3*ui(i1+3,i2+2,i3+0,c3)+cr4*ui(i1+4,i2+2,i3+0,c3)+cr5*ui(
     & i1+5,i2+2,i3+0,c3)+cr6*ui(i1+6,i2+2,i3+0,c3)+cr7*ui(i1+7,i2+2,
     & i3+0,c3)+cr8*ui(i1+8,i2+2,i3+0,c3))+cs3*(cr0*ui(i1,i2+3,i3+0,
     & c3)+cr1*ui(i1+1,i2+3,i3+0,c3)+cr2*ui(i1+2,i2+3,i3+0,c3)+cr3*ui(
     & i1+3,i2+3,i3+0,c3)+cr4*ui(i1+4,i2+3,i3+0,c3)+cr5*ui(i1+5,i2+3,
     & i3+0,c3)+cr6*ui(i1+6,i2+3,i3+0,c3)+cr7*ui(i1+7,i2+3,i3+0,c3)+
     & cr8*ui(i1+8,i2+3,i3+0,c3))+cs4*(cr0*ui(i1,i2+4,i3+0,c3)+cr1*ui(
     & i1+1,i2+4,i3+0,c3)+cr2*ui(i1+2,i2+4,i3+0,c3)+cr3*ui(i1+3,i2+4,
     & i3+0,c3)+cr4*ui(i1+4,i2+4,i3+0,c3)+cr5*ui(i1+5,i2+4,i3+0,c3)+
     & cr6*ui(i1+6,i2+4,i3+0,c3)+cr7*ui(i1+7,i2+4,i3+0,c3)+cr8*ui(i1+
     & 8,i2+4,i3+0,c3))+cs5*(cr0*ui(i1,i2+5,i3+0,c3)+cr1*ui(i1+1,i2+5,
     & i3+0,c3)+cr2*ui(i1+2,i2+5,i3+0,c3)+cr3*ui(i1+3,i2+5,i3+0,c3)+
     & cr4*ui(i1+4,i2+5,i3+0,c3)+cr5*ui(i1+5,i2+5,i3+0,c3)+cr6*ui(i1+
     & 6,i2+5,i3+0,c3)+cr7*ui(i1+7,i2+5,i3+0,c3)+cr8*ui(i1+8,i2+5,i3+
     & 0,c3))+cs6*(cr0*ui(i1,i2+6,i3+0,c3)+cr1*ui(i1+1,i2+6,i3+0,c3)+
     & cr2*ui(i1+2,i2+6,i3+0,c3)+cr3*ui(i1+3,i2+6,i3+0,c3)+cr4*ui(i1+
     & 4,i2+6,i3+0,c3)+cr5*ui(i1+5,i2+6,i3+0,c3)+cr6*ui(i1+6,i2+6,i3+
     & 0,c3)+cr7*ui(i1+7,i2+6,i3+0,c3)+cr8*ui(i1+8,i2+6,i3+0,c3))+cs7*
     & (cr0*ui(i1,i2+7,i3+0,c3)+cr1*ui(i1+1,i2+7,i3+0,c3)+cr2*ui(i1+2,
     & i2+7,i3+0,c3)+cr3*ui(i1+3,i2+7,i3+0,c3)+cr4*ui(i1+4,i2+7,i3+0,
     & c3)+cr5*ui(i1+5,i2+7,i3+0,c3)+cr6*ui(i1+6,i2+7,i3+0,c3)+cr7*ui(
     & i1+7,i2+7,i3+0,c3)+cr8*ui(i1+8,i2+7,i3+0,c3))+cs8*(cr0*ui(i1,
     & i2+8,i3+0,c3)+cr1*ui(i1+1,i2+8,i3+0,c3)+cr2*ui(i1+2,i2+8,i3+0,
     & c3)+cr3*ui(i1+3,i2+8,i3+0,c3)+cr4*ui(i1+4,i2+8,i3+0,c3)+cr5*ui(
     & i1+5,i2+8,i3+0,c3)+cr6*ui(i1+6,i2+8,i3+0,c3)+cr7*ui(i1+7,i2+8,
     & i3+0,c3)+cr8*ui(i1+8,i2+8,i3+0,c3)))+ct1*(cs0*(cr0*ui(i1,i2,i3+
     & 1,c3)+cr1*ui(i1+1,i2,i3+1,c3)+cr2*ui(i1+2,i2,i3+1,c3)+cr3*ui(
     & i1+3,i2,i3+1,c3)+cr4*ui(i1+4,i2,i3+1,c3)+cr5*ui(i1+5,i2,i3+1,
     & c3)+cr6*ui(i1+6,i2,i3+1,c3)+cr7*ui(i1+7,i2,i3+1,c3)+cr8*ui(i1+
     & 8,i2,i3+1,c3))+cs1*(cr0*ui(i1,i2+1,i3+1,c3)+cr1*ui(i1+1,i2+1,
     & i3+1,c3)+cr2*ui(i1+2,i2+1,i3+1,c3)+cr3*ui(i1+3,i2+1,i3+1,c3)+
     & cr4*ui(i1+4,i2+1,i3+1,c3)+cr5*ui(i1+5,i2+1,i3+1,c3)+cr6*ui(i1+
     & 6,i2+1,i3+1,c3)+cr7*ui(i1+7,i2+1,i3+1,c3)+cr8*ui(i1+8,i2+1,i3+
     & 1,c3))+cs2*(cr0*ui(i1,i2+2,i3+1,c3)+cr1*ui(i1+1,i2+2,i3+1,c3)+
     & cr2*ui(i1+2,i2+2,i3+1,c3)+cr3*ui(i1+3,i2+2,i3+1,c3)+cr4*ui(i1+
     & 4,i2+2,i3+1,c3)+cr5*ui(i1+5,i2+2,i3+1,c3)+cr6*ui(i1+6,i2+2,i3+
     & 1,c3)+cr7*ui(i1+7,i2+2,i3+1,c3)+cr8*ui(i1+8,i2+2,i3+1,c3))+cs3*
     & (cr0*ui(i1,i2+3,i3+1,c3)+cr1*ui(i1+1,i2+3,i3+1,c3)+cr2*ui(i1+2,
     & i2+3,i3+1,c3)+cr3*ui(i1+3,i2+3,i3+1,c3)+cr4*ui(i1+4,i2+3,i3+1,
     & c3)+cr5*ui(i1+5,i2+3,i3+1,c3)+cr6*ui(i1+6,i2+3,i3+1,c3)+cr7*ui(
     & i1+7,i2+3,i3+1,c3)+cr8*ui(i1+8,i2+3,i3+1,c3))+cs4*(cr0*ui(i1,
     & i2+4,i3+1,c3)+cr1*ui(i1+1,i2+4,i3+1,c3)+cr2*ui(i1+2,i2+4,i3+1,
     & c3)+cr3*ui(i1+3,i2+4,i3+1,c3)+cr4*ui(i1+4,i2+4,i3+1,c3)+cr5*ui(
     & i1+5,i2+4,i3+1,c3)+cr6*ui(i1+6,i2+4,i3+1,c3)+cr7*ui(i1+7,i2+4,
     & i3+1,c3)+cr8*ui(i1+8,i2+4,i3+1,c3))+cs5*(cr0*ui(i1,i2+5,i3+1,
     & c3)+cr1*ui(i1+1,i2+5,i3+1,c3)+cr2*ui(i1+2,i2+5,i3+1,c3)+cr3*ui(
     & i1+3,i2+5,i3+1,c3)+cr4*ui(i1+4,i2+5,i3+1,c3)+cr5*ui(i1+5,i2+5,
     & i3+1,c3)+cr6*ui(i1+6,i2+5,i3+1,c3)+cr7*ui(i1+7,i2+5,i3+1,c3)+
     & cr8*ui(i1+8,i2+5,i3+1,c3))+cs6*(cr0*ui(i1,i2+6,i3+1,c3)+cr1*ui(
     & i1+1,i2+6,i3+1,c3)+cr2*ui(i1+2,i2+6,i3+1,c3)+cr3*ui(i1+3,i2+6,
     & i3+1,c3)+cr4*ui(i1+4,i2+6,i3+1,c3)+cr5*ui(i1+5,i2+6,i3+1,c3)+
     & cr6*ui(i1+6,i2+6,i3+1,c3)+cr7*ui(i1+7,i2+6,i3+1,c3)+cr8*ui(i1+
     & 8,i2+6,i3+1,c3))+cs7*(cr0*ui(i1,i2+7,i3+1,c3)+cr1*ui(i1+1,i2+7,
     & i3+1,c3)+cr2*ui(i1+2,i2+7,i3+1,c3)+cr3*ui(i1+3,i2+7,i3+1,c3)+
     & cr4*ui(i1+4,i2+7,i3+1,c3)+cr5*ui(i1+5,i2+7,i3+1,c3)+cr6*ui(i1+
     & 6,i2+7,i3+1,c3)+cr7*ui(i1+7,i2+7,i3+1,c3)+cr8*ui(i1+8,i2+7,i3+
     & 1,c3))+cs8*(cr0*ui(i1,i2+8,i3+1,c3)+cr1*ui(i1+1,i2+8,i3+1,c3)+
     & cr2*ui(i1+2,i2+8,i3+1,c3)+cr3*ui(i1+3,i2+8,i3+1,c3)+cr4*ui(i1+
     & 4,i2+8,i3+1,c3)+cr5*ui(i1+5,i2+8,i3+1,c3)+cr6*ui(i1+6,i2+8,i3+
     & 1,c3)+cr7*ui(i1+7,i2+8,i3+1,c3)+cr8*ui(i1+8,i2+8,i3+1,c3)))
               r(i) = r(i)+ct2*(cs0*(cr0*ui(i1,i2,i3+2,c3)+cr1*ui(i1+1,
     & i2,i3+2,c3)+cr2*ui(i1+2,i2,i3+2,c3)+cr3*ui(i1+3,i2,i3+2,c3)+
     & cr4*ui(i1+4,i2,i3+2,c3)+cr5*ui(i1+5,i2,i3+2,c3)+cr6*ui(i1+6,i2,
     & i3+2,c3)+cr7*ui(i1+7,i2,i3+2,c3)+cr8*ui(i1+8,i2,i3+2,c3))+cs1*(
     & cr0*ui(i1,i2+1,i3+2,c3)+cr1*ui(i1+1,i2+1,i3+2,c3)+cr2*ui(i1+2,
     & i2+1,i3+2,c3)+cr3*ui(i1+3,i2+1,i3+2,c3)+cr4*ui(i1+4,i2+1,i3+2,
     & c3)+cr5*ui(i1+5,i2+1,i3+2,c3)+cr6*ui(i1+6,i2+1,i3+2,c3)+cr7*ui(
     & i1+7,i2+1,i3+2,c3)+cr8*ui(i1+8,i2+1,i3+2,c3))+cs2*(cr0*ui(i1,
     & i2+2,i3+2,c3)+cr1*ui(i1+1,i2+2,i3+2,c3)+cr2*ui(i1+2,i2+2,i3+2,
     & c3)+cr3*ui(i1+3,i2+2,i3+2,c3)+cr4*ui(i1+4,i2+2,i3+2,c3)+cr5*ui(
     & i1+5,i2+2,i3+2,c3)+cr6*ui(i1+6,i2+2,i3+2,c3)+cr7*ui(i1+7,i2+2,
     & i3+2,c3)+cr8*ui(i1+8,i2+2,i3+2,c3))+cs3*(cr0*ui(i1,i2+3,i3+2,
     & c3)+cr1*ui(i1+1,i2+3,i3+2,c3)+cr2*ui(i1+2,i2+3,i3+2,c3)+cr3*ui(
     & i1+3,i2+3,i3+2,c3)+cr4*ui(i1+4,i2+3,i3+2,c3)+cr5*ui(i1+5,i2+3,
     & i3+2,c3)+cr6*ui(i1+6,i2+3,i3+2,c3)+cr7*ui(i1+7,i2+3,i3+2,c3)+
     & cr8*ui(i1+8,i2+3,i3+2,c3))+cs4*(cr0*ui(i1,i2+4,i3+2,c3)+cr1*ui(
     & i1+1,i2+4,i3+2,c3)+cr2*ui(i1+2,i2+4,i3+2,c3)+cr3*ui(i1+3,i2+4,
     & i3+2,c3)+cr4*ui(i1+4,i2+4,i3+2,c3)+cr5*ui(i1+5,i2+4,i3+2,c3)+
     & cr6*ui(i1+6,i2+4,i3+2,c3)+cr7*ui(i1+7,i2+4,i3+2,c3)+cr8*ui(i1+
     & 8,i2+4,i3+2,c3))+cs5*(cr0*ui(i1,i2+5,i3+2,c3)+cr1*ui(i1+1,i2+5,
     & i3+2,c3)+cr2*ui(i1+2,i2+5,i3+2,c3)+cr3*ui(i1+3,i2+5,i3+2,c3)+
     & cr4*ui(i1+4,i2+5,i3+2,c3)+cr5*ui(i1+5,i2+5,i3+2,c3)+cr6*ui(i1+
     & 6,i2+5,i3+2,c3)+cr7*ui(i1+7,i2+5,i3+2,c3)+cr8*ui(i1+8,i2+5,i3+
     & 2,c3))+cs6*(cr0*ui(i1,i2+6,i3+2,c3)+cr1*ui(i1+1,i2+6,i3+2,c3)+
     & cr2*ui(i1+2,i2+6,i3+2,c3)+cr3*ui(i1+3,i2+6,i3+2,c3)+cr4*ui(i1+
     & 4,i2+6,i3+2,c3)+cr5*ui(i1+5,i2+6,i3+2,c3)+cr6*ui(i1+6,i2+6,i3+
     & 2,c3)+cr7*ui(i1+7,i2+6,i3+2,c3)+cr8*ui(i1+8,i2+6,i3+2,c3))+cs7*
     & (cr0*ui(i1,i2+7,i3+2,c3)+cr1*ui(i1+1,i2+7,i3+2,c3)+cr2*ui(i1+2,
     & i2+7,i3+2,c3)+cr3*ui(i1+3,i2+7,i3+2,c3)+cr4*ui(i1+4,i2+7,i3+2,
     & c3)+cr5*ui(i1+5,i2+7,i3+2,c3)+cr6*ui(i1+6,i2+7,i3+2,c3)+cr7*ui(
     & i1+7,i2+7,i3+2,c3)+cr8*ui(i1+8,i2+7,i3+2,c3))+cs8*(cr0*ui(i1,
     & i2+8,i3+2,c3)+cr1*ui(i1+1,i2+8,i3+2,c3)+cr2*ui(i1+2,i2+8,i3+2,
     & c3)+cr3*ui(i1+3,i2+8,i3+2,c3)+cr4*ui(i1+4,i2+8,i3+2,c3)+cr5*ui(
     & i1+5,i2+8,i3+2,c3)+cr6*ui(i1+6,i2+8,i3+2,c3)+cr7*ui(i1+7,i2+8,
     & i3+2,c3)+cr8*ui(i1+8,i2+8,i3+2,c3)))+ct3*(cs0*(cr0*ui(i1,i2,i3+
     & 3,c3)+cr1*ui(i1+1,i2,i3+3,c3)+cr2*ui(i1+2,i2,i3+3,c3)+cr3*ui(
     & i1+3,i2,i3+3,c3)+cr4*ui(i1+4,i2,i3+3,c3)+cr5*ui(i1+5,i2,i3+3,
     & c3)+cr6*ui(i1+6,i2,i3+3,c3)+cr7*ui(i1+7,i2,i3+3,c3)+cr8*ui(i1+
     & 8,i2,i3+3,c3))+cs1*(cr0*ui(i1,i2+1,i3+3,c3)+cr1*ui(i1+1,i2+1,
     & i3+3,c3)+cr2*ui(i1+2,i2+1,i3+3,c3)+cr3*ui(i1+3,i2+1,i3+3,c3)+
     & cr4*ui(i1+4,i2+1,i3+3,c3)+cr5*ui(i1+5,i2+1,i3+3,c3)+cr6*ui(i1+
     & 6,i2+1,i3+3,c3)+cr7*ui(i1+7,i2+1,i3+3,c3)+cr8*ui(i1+8,i2+1,i3+
     & 3,c3))+cs2*(cr0*ui(i1,i2+2,i3+3,c3)+cr1*ui(i1+1,i2+2,i3+3,c3)+
     & cr2*ui(i1+2,i2+2,i3+3,c3)+cr3*ui(i1+3,i2+2,i3+3,c3)+cr4*ui(i1+
     & 4,i2+2,i3+3,c3)+cr5*ui(i1+5,i2+2,i3+3,c3)+cr6*ui(i1+6,i2+2,i3+
     & 3,c3)+cr7*ui(i1+7,i2+2,i3+3,c3)+cr8*ui(i1+8,i2+2,i3+3,c3))+cs3*
     & (cr0*ui(i1,i2+3,i3+3,c3)+cr1*ui(i1+1,i2+3,i3+3,c3)+cr2*ui(i1+2,
     & i2+3,i3+3,c3)+cr3*ui(i1+3,i2+3,i3+3,c3)+cr4*ui(i1+4,i2+3,i3+3,
     & c3)+cr5*ui(i1+5,i2+3,i3+3,c3)+cr6*ui(i1+6,i2+3,i3+3,c3)+cr7*ui(
     & i1+7,i2+3,i3+3,c3)+cr8*ui(i1+8,i2+3,i3+3,c3))+cs4*(cr0*ui(i1,
     & i2+4,i3+3,c3)+cr1*ui(i1+1,i2+4,i3+3,c3)+cr2*ui(i1+2,i2+4,i3+3,
     & c3)+cr3*ui(i1+3,i2+4,i3+3,c3)+cr4*ui(i1+4,i2+4,i3+3,c3)+cr5*ui(
     & i1+5,i2+4,i3+3,c3)+cr6*ui(i1+6,i2+4,i3+3,c3)+cr7*ui(i1+7,i2+4,
     & i3+3,c3)+cr8*ui(i1+8,i2+4,i3+3,c3))+cs5*(cr0*ui(i1,i2+5,i3+3,
     & c3)+cr1*ui(i1+1,i2+5,i3+3,c3)+cr2*ui(i1+2,i2+5,i3+3,c3)+cr3*ui(
     & i1+3,i2+5,i3+3,c3)+cr4*ui(i1+4,i2+5,i3+3,c3)+cr5*ui(i1+5,i2+5,
     & i3+3,c3)+cr6*ui(i1+6,i2+5,i3+3,c3)+cr7*ui(i1+7,i2+5,i3+3,c3)+
     & cr8*ui(i1+8,i2+5,i3+3,c3))+cs6*(cr0*ui(i1,i2+6,i3+3,c3)+cr1*ui(
     & i1+1,i2+6,i3+3,c3)+cr2*ui(i1+2,i2+6,i3+3,c3)+cr3*ui(i1+3,i2+6,
     & i3+3,c3)+cr4*ui(i1+4,i2+6,i3+3,c3)+cr5*ui(i1+5,i2+6,i3+3,c3)+
     & cr6*ui(i1+6,i2+6,i3+3,c3)+cr7*ui(i1+7,i2+6,i3+3,c3)+cr8*ui(i1+
     & 8,i2+6,i3+3,c3))+cs7*(cr0*ui(i1,i2+7,i3+3,c3)+cr1*ui(i1+1,i2+7,
     & i3+3,c3)+cr2*ui(i1+2,i2+7,i3+3,c3)+cr3*ui(i1+3,i2+7,i3+3,c3)+
     & cr4*ui(i1+4,i2+7,i3+3,c3)+cr5*ui(i1+5,i2+7,i3+3,c3)+cr6*ui(i1+
     & 6,i2+7,i3+3,c3)+cr7*ui(i1+7,i2+7,i3+3,c3)+cr8*ui(i1+8,i2+7,i3+
     & 3,c3))+cs8*(cr0*ui(i1,i2+8,i3+3,c3)+cr1*ui(i1+1,i2+8,i3+3,c3)+
     & cr2*ui(i1+2,i2+8,i3+3,c3)+cr3*ui(i1+3,i2+8,i3+3,c3)+cr4*ui(i1+
     & 4,i2+8,i3+3,c3)+cr5*ui(i1+5,i2+8,i3+3,c3)+cr6*ui(i1+6,i2+8,i3+
     & 3,c3)+cr7*ui(i1+7,i2+8,i3+3,c3)+cr8*ui(i1+8,i2+8,i3+3,c3)))
               r(i) = r(i)+ct4*(cs0*(cr0*ui(i1,i2,i3+4,c3)+cr1*ui(i1+1,
     & i2,i3+4,c3)+cr2*ui(i1+2,i2,i3+4,c3)+cr3*ui(i1+3,i2,i3+4,c3)+
     & cr4*ui(i1+4,i2,i3+4,c3)+cr5*ui(i1+5,i2,i3+4,c3)+cr6*ui(i1+6,i2,
     & i3+4,c3)+cr7*ui(i1+7,i2,i3+4,c3)+cr8*ui(i1+8,i2,i3+4,c3))+cs1*(
     & cr0*ui(i1,i2+1,i3+4,c3)+cr1*ui(i1+1,i2+1,i3+4,c3)+cr2*ui(i1+2,
     & i2+1,i3+4,c3)+cr3*ui(i1+3,i2+1,i3+4,c3)+cr4*ui(i1+4,i2+1,i3+4,
     & c3)+cr5*ui(i1+5,i2+1,i3+4,c3)+cr6*ui(i1+6,i2+1,i3+4,c3)+cr7*ui(
     & i1+7,i2+1,i3+4,c3)+cr8*ui(i1+8,i2+1,i3+4,c3))+cs2*(cr0*ui(i1,
     & i2+2,i3+4,c3)+cr1*ui(i1+1,i2+2,i3+4,c3)+cr2*ui(i1+2,i2+2,i3+4,
     & c3)+cr3*ui(i1+3,i2+2,i3+4,c3)+cr4*ui(i1+4,i2+2,i3+4,c3)+cr5*ui(
     & i1+5,i2+2,i3+4,c3)+cr6*ui(i1+6,i2+2,i3+4,c3)+cr7*ui(i1+7,i2+2,
     & i3+4,c3)+cr8*ui(i1+8,i2+2,i3+4,c3))+cs3*(cr0*ui(i1,i2+3,i3+4,
     & c3)+cr1*ui(i1+1,i2+3,i3+4,c3)+cr2*ui(i1+2,i2+3,i3+4,c3)+cr3*ui(
     & i1+3,i2+3,i3+4,c3)+cr4*ui(i1+4,i2+3,i3+4,c3)+cr5*ui(i1+5,i2+3,
     & i3+4,c3)+cr6*ui(i1+6,i2+3,i3+4,c3)+cr7*ui(i1+7,i2+3,i3+4,c3)+
     & cr8*ui(i1+8,i2+3,i3+4,c3))+cs4*(cr0*ui(i1,i2+4,i3+4,c3)+cr1*ui(
     & i1+1,i2+4,i3+4,c3)+cr2*ui(i1+2,i2+4,i3+4,c3)+cr3*ui(i1+3,i2+4,
     & i3+4,c3)+cr4*ui(i1+4,i2+4,i3+4,c3)+cr5*ui(i1+5,i2+4,i3+4,c3)+
     & cr6*ui(i1+6,i2+4,i3+4,c3)+cr7*ui(i1+7,i2+4,i3+4,c3)+cr8*ui(i1+
     & 8,i2+4,i3+4,c3))+cs5*(cr0*ui(i1,i2+5,i3+4,c3)+cr1*ui(i1+1,i2+5,
     & i3+4,c3)+cr2*ui(i1+2,i2+5,i3+4,c3)+cr3*ui(i1+3,i2+5,i3+4,c3)+
     & cr4*ui(i1+4,i2+5,i3+4,c3)+cr5*ui(i1+5,i2+5,i3+4,c3)+cr6*ui(i1+
     & 6,i2+5,i3+4,c3)+cr7*ui(i1+7,i2+5,i3+4,c3)+cr8*ui(i1+8,i2+5,i3+
     & 4,c3))+cs6*(cr0*ui(i1,i2+6,i3+4,c3)+cr1*ui(i1+1,i2+6,i3+4,c3)+
     & cr2*ui(i1+2,i2+6,i3+4,c3)+cr3*ui(i1+3,i2+6,i3+4,c3)+cr4*ui(i1+
     & 4,i2+6,i3+4,c3)+cr5*ui(i1+5,i2+6,i3+4,c3)+cr6*ui(i1+6,i2+6,i3+
     & 4,c3)+cr7*ui(i1+7,i2+6,i3+4,c3)+cr8*ui(i1+8,i2+6,i3+4,c3))+cs7*
     & (cr0*ui(i1,i2+7,i3+4,c3)+cr1*ui(i1+1,i2+7,i3+4,c3)+cr2*ui(i1+2,
     & i2+7,i3+4,c3)+cr3*ui(i1+3,i2+7,i3+4,c3)+cr4*ui(i1+4,i2+7,i3+4,
     & c3)+cr5*ui(i1+5,i2+7,i3+4,c3)+cr6*ui(i1+6,i2+7,i3+4,c3)+cr7*ui(
     & i1+7,i2+7,i3+4,c3)+cr8*ui(i1+8,i2+7,i3+4,c3))+cs8*(cr0*ui(i1,
     & i2+8,i3+4,c3)+cr1*ui(i1+1,i2+8,i3+4,c3)+cr2*ui(i1+2,i2+8,i3+4,
     & c3)+cr3*ui(i1+3,i2+8,i3+4,c3)+cr4*ui(i1+4,i2+8,i3+4,c3)+cr5*ui(
     & i1+5,i2+8,i3+4,c3)+cr6*ui(i1+6,i2+8,i3+4,c3)+cr7*ui(i1+7,i2+8,
     & i3+4,c3)+cr8*ui(i1+8,i2+8,i3+4,c3)))+ct5*(cs0*(cr0*ui(i1,i2,i3+
     & 5,c3)+cr1*ui(i1+1,i2,i3+5,c3)+cr2*ui(i1+2,i2,i3+5,c3)+cr3*ui(
     & i1+3,i2,i3+5,c3)+cr4*ui(i1+4,i2,i3+5,c3)+cr5*ui(i1+5,i2,i3+5,
     & c3)+cr6*ui(i1+6,i2,i3+5,c3)+cr7*ui(i1+7,i2,i3+5,c3)+cr8*ui(i1+
     & 8,i2,i3+5,c3))+cs1*(cr0*ui(i1,i2+1,i3+5,c3)+cr1*ui(i1+1,i2+1,
     & i3+5,c3)+cr2*ui(i1+2,i2+1,i3+5,c3)+cr3*ui(i1+3,i2+1,i3+5,c3)+
     & cr4*ui(i1+4,i2+1,i3+5,c3)+cr5*ui(i1+5,i2+1,i3+5,c3)+cr6*ui(i1+
     & 6,i2+1,i3+5,c3)+cr7*ui(i1+7,i2+1,i3+5,c3)+cr8*ui(i1+8,i2+1,i3+
     & 5,c3))+cs2*(cr0*ui(i1,i2+2,i3+5,c3)+cr1*ui(i1+1,i2+2,i3+5,c3)+
     & cr2*ui(i1+2,i2+2,i3+5,c3)+cr3*ui(i1+3,i2+2,i3+5,c3)+cr4*ui(i1+
     & 4,i2+2,i3+5,c3)+cr5*ui(i1+5,i2+2,i3+5,c3)+cr6*ui(i1+6,i2+2,i3+
     & 5,c3)+cr7*ui(i1+7,i2+2,i3+5,c3)+cr8*ui(i1+8,i2+2,i3+5,c3))+cs3*
     & (cr0*ui(i1,i2+3,i3+5,c3)+cr1*ui(i1+1,i2+3,i3+5,c3)+cr2*ui(i1+2,
     & i2+3,i3+5,c3)+cr3*ui(i1+3,i2+3,i3+5,c3)+cr4*ui(i1+4,i2+3,i3+5,
     & c3)+cr5*ui(i1+5,i2+3,i3+5,c3)+cr6*ui(i1+6,i2+3,i3+5,c3)+cr7*ui(
     & i1+7,i2+3,i3+5,c3)+cr8*ui(i1+8,i2+3,i3+5,c3))+cs4*(cr0*ui(i1,
     & i2+4,i3+5,c3)+cr1*ui(i1+1,i2+4,i3+5,c3)+cr2*ui(i1+2,i2+4,i3+5,
     & c3)+cr3*ui(i1+3,i2+4,i3+5,c3)+cr4*ui(i1+4,i2+4,i3+5,c3)+cr5*ui(
     & i1+5,i2+4,i3+5,c3)+cr6*ui(i1+6,i2+4,i3+5,c3)+cr7*ui(i1+7,i2+4,
     & i3+5,c3)+cr8*ui(i1+8,i2+4,i3+5,c3))+cs5*(cr0*ui(i1,i2+5,i3+5,
     & c3)+cr1*ui(i1+1,i2+5,i3+5,c3)+cr2*ui(i1+2,i2+5,i3+5,c3)+cr3*ui(
     & i1+3,i2+5,i3+5,c3)+cr4*ui(i1+4,i2+5,i3+5,c3)+cr5*ui(i1+5,i2+5,
     & i3+5,c3)+cr6*ui(i1+6,i2+5,i3+5,c3)+cr7*ui(i1+7,i2+5,i3+5,c3)+
     & cr8*ui(i1+8,i2+5,i3+5,c3))+cs6*(cr0*ui(i1,i2+6,i3+5,c3)+cr1*ui(
     & i1+1,i2+6,i3+5,c3)+cr2*ui(i1+2,i2+6,i3+5,c3)+cr3*ui(i1+3,i2+6,
     & i3+5,c3)+cr4*ui(i1+4,i2+6,i3+5,c3)+cr5*ui(i1+5,i2+6,i3+5,c3)+
     & cr6*ui(i1+6,i2+6,i3+5,c3)+cr7*ui(i1+7,i2+6,i3+5,c3)+cr8*ui(i1+
     & 8,i2+6,i3+5,c3))+cs7*(cr0*ui(i1,i2+7,i3+5,c3)+cr1*ui(i1+1,i2+7,
     & i3+5,c3)+cr2*ui(i1+2,i2+7,i3+5,c3)+cr3*ui(i1+3,i2+7,i3+5,c3)+
     & cr4*ui(i1+4,i2+7,i3+5,c3)+cr5*ui(i1+5,i2+7,i3+5,c3)+cr6*ui(i1+
     & 6,i2+7,i3+5,c3)+cr7*ui(i1+7,i2+7,i3+5,c3)+cr8*ui(i1+8,i2+7,i3+
     & 5,c3))+cs8*(cr0*ui(i1,i2+8,i3+5,c3)+cr1*ui(i1+1,i2+8,i3+5,c3)+
     & cr2*ui(i1+2,i2+8,i3+5,c3)+cr3*ui(i1+3,i2+8,i3+5,c3)+cr4*ui(i1+
     & 4,i2+8,i3+5,c3)+cr5*ui(i1+5,i2+8,i3+5,c3)+cr6*ui(i1+6,i2+8,i3+
     & 5,c3)+cr7*ui(i1+7,i2+8,i3+5,c3)+cr8*ui(i1+8,i2+8,i3+5,c3)))
               r(i) = r(i)+ct6*(cs0*(cr0*ui(i1,i2,i3+6,c3)+cr1*ui(i1+1,
     & i2,i3+6,c3)+cr2*ui(i1+2,i2,i3+6,c3)+cr3*ui(i1+3,i2,i3+6,c3)+
     & cr4*ui(i1+4,i2,i3+6,c3)+cr5*ui(i1+5,i2,i3+6,c3)+cr6*ui(i1+6,i2,
     & i3+6,c3)+cr7*ui(i1+7,i2,i3+6,c3)+cr8*ui(i1+8,i2,i3+6,c3))+cs1*(
     & cr0*ui(i1,i2+1,i3+6,c3)+cr1*ui(i1+1,i2+1,i3+6,c3)+cr2*ui(i1+2,
     & i2+1,i3+6,c3)+cr3*ui(i1+3,i2+1,i3+6,c3)+cr4*ui(i1+4,i2+1,i3+6,
     & c3)+cr5*ui(i1+5,i2+1,i3+6,c3)+cr6*ui(i1+6,i2+1,i3+6,c3)+cr7*ui(
     & i1+7,i2+1,i3+6,c3)+cr8*ui(i1+8,i2+1,i3+6,c3))+cs2*(cr0*ui(i1,
     & i2+2,i3+6,c3)+cr1*ui(i1+1,i2+2,i3+6,c3)+cr2*ui(i1+2,i2+2,i3+6,
     & c3)+cr3*ui(i1+3,i2+2,i3+6,c3)+cr4*ui(i1+4,i2+2,i3+6,c3)+cr5*ui(
     & i1+5,i2+2,i3+6,c3)+cr6*ui(i1+6,i2+2,i3+6,c3)+cr7*ui(i1+7,i2+2,
     & i3+6,c3)+cr8*ui(i1+8,i2+2,i3+6,c3))+cs3*(cr0*ui(i1,i2+3,i3+6,
     & c3)+cr1*ui(i1+1,i2+3,i3+6,c3)+cr2*ui(i1+2,i2+3,i3+6,c3)+cr3*ui(
     & i1+3,i2+3,i3+6,c3)+cr4*ui(i1+4,i2+3,i3+6,c3)+cr5*ui(i1+5,i2+3,
     & i3+6,c3)+cr6*ui(i1+6,i2+3,i3+6,c3)+cr7*ui(i1+7,i2+3,i3+6,c3)+
     & cr8*ui(i1+8,i2+3,i3+6,c3))+cs4*(cr0*ui(i1,i2+4,i3+6,c3)+cr1*ui(
     & i1+1,i2+4,i3+6,c3)+cr2*ui(i1+2,i2+4,i3+6,c3)+cr3*ui(i1+3,i2+4,
     & i3+6,c3)+cr4*ui(i1+4,i2+4,i3+6,c3)+cr5*ui(i1+5,i2+4,i3+6,c3)+
     & cr6*ui(i1+6,i2+4,i3+6,c3)+cr7*ui(i1+7,i2+4,i3+6,c3)+cr8*ui(i1+
     & 8,i2+4,i3+6,c3))+cs5*(cr0*ui(i1,i2+5,i3+6,c3)+cr1*ui(i1+1,i2+5,
     & i3+6,c3)+cr2*ui(i1+2,i2+5,i3+6,c3)+cr3*ui(i1+3,i2+5,i3+6,c3)+
     & cr4*ui(i1+4,i2+5,i3+6,c3)+cr5*ui(i1+5,i2+5,i3+6,c3)+cr6*ui(i1+
     & 6,i2+5,i3+6,c3)+cr7*ui(i1+7,i2+5,i3+6,c3)+cr8*ui(i1+8,i2+5,i3+
     & 6,c3))+cs6*(cr0*ui(i1,i2+6,i3+6,c3)+cr1*ui(i1+1,i2+6,i3+6,c3)+
     & cr2*ui(i1+2,i2+6,i3+6,c3)+cr3*ui(i1+3,i2+6,i3+6,c3)+cr4*ui(i1+
     & 4,i2+6,i3+6,c3)+cr5*ui(i1+5,i2+6,i3+6,c3)+cr6*ui(i1+6,i2+6,i3+
     & 6,c3)+cr7*ui(i1+7,i2+6,i3+6,c3)+cr8*ui(i1+8,i2+6,i3+6,c3))+cs7*
     & (cr0*ui(i1,i2+7,i3+6,c3)+cr1*ui(i1+1,i2+7,i3+6,c3)+cr2*ui(i1+2,
     & i2+7,i3+6,c3)+cr3*ui(i1+3,i2+7,i3+6,c3)+cr4*ui(i1+4,i2+7,i3+6,
     & c3)+cr5*ui(i1+5,i2+7,i3+6,c3)+cr6*ui(i1+6,i2+7,i3+6,c3)+cr7*ui(
     & i1+7,i2+7,i3+6,c3)+cr8*ui(i1+8,i2+7,i3+6,c3))+cs8*(cr0*ui(i1,
     & i2+8,i3+6,c3)+cr1*ui(i1+1,i2+8,i3+6,c3)+cr2*ui(i1+2,i2+8,i3+6,
     & c3)+cr3*ui(i1+3,i2+8,i3+6,c3)+cr4*ui(i1+4,i2+8,i3+6,c3)+cr5*ui(
     & i1+5,i2+8,i3+6,c3)+cr6*ui(i1+6,i2+8,i3+6,c3)+cr7*ui(i1+7,i2+8,
     & i3+6,c3)+cr8*ui(i1+8,i2+8,i3+6,c3)))+ct7*(cs0*(cr0*ui(i1,i2,i3+
     & 7,c3)+cr1*ui(i1+1,i2,i3+7,c3)+cr2*ui(i1+2,i2,i3+7,c3)+cr3*ui(
     & i1+3,i2,i3+7,c3)+cr4*ui(i1+4,i2,i3+7,c3)+cr5*ui(i1+5,i2,i3+7,
     & c3)+cr6*ui(i1+6,i2,i3+7,c3)+cr7*ui(i1+7,i2,i3+7,c3)+cr8*ui(i1+
     & 8,i2,i3+7,c3))+cs1*(cr0*ui(i1,i2+1,i3+7,c3)+cr1*ui(i1+1,i2+1,
     & i3+7,c3)+cr2*ui(i1+2,i2+1,i3+7,c3)+cr3*ui(i1+3,i2+1,i3+7,c3)+
     & cr4*ui(i1+4,i2+1,i3+7,c3)+cr5*ui(i1+5,i2+1,i3+7,c3)+cr6*ui(i1+
     & 6,i2+1,i3+7,c3)+cr7*ui(i1+7,i2+1,i3+7,c3)+cr8*ui(i1+8,i2+1,i3+
     & 7,c3))+cs2*(cr0*ui(i1,i2+2,i3+7,c3)+cr1*ui(i1+1,i2+2,i3+7,c3)+
     & cr2*ui(i1+2,i2+2,i3+7,c3)+cr3*ui(i1+3,i2+2,i3+7,c3)+cr4*ui(i1+
     & 4,i2+2,i3+7,c3)+cr5*ui(i1+5,i2+2,i3+7,c3)+cr6*ui(i1+6,i2+2,i3+
     & 7,c3)+cr7*ui(i1+7,i2+2,i3+7,c3)+cr8*ui(i1+8,i2+2,i3+7,c3))+cs3*
     & (cr0*ui(i1,i2+3,i3+7,c3)+cr1*ui(i1+1,i2+3,i3+7,c3)+cr2*ui(i1+2,
     & i2+3,i3+7,c3)+cr3*ui(i1+3,i2+3,i3+7,c3)+cr4*ui(i1+4,i2+3,i3+7,
     & c3)+cr5*ui(i1+5,i2+3,i3+7,c3)+cr6*ui(i1+6,i2+3,i3+7,c3)+cr7*ui(
     & i1+7,i2+3,i3+7,c3)+cr8*ui(i1+8,i2+3,i3+7,c3))+cs4*(cr0*ui(i1,
     & i2+4,i3+7,c3)+cr1*ui(i1+1,i2+4,i3+7,c3)+cr2*ui(i1+2,i2+4,i3+7,
     & c3)+cr3*ui(i1+3,i2+4,i3+7,c3)+cr4*ui(i1+4,i2+4,i3+7,c3)+cr5*ui(
     & i1+5,i2+4,i3+7,c3)+cr6*ui(i1+6,i2+4,i3+7,c3)+cr7*ui(i1+7,i2+4,
     & i3+7,c3)+cr8*ui(i1+8,i2+4,i3+7,c3))+cs5*(cr0*ui(i1,i2+5,i3+7,
     & c3)+cr1*ui(i1+1,i2+5,i3+7,c3)+cr2*ui(i1+2,i2+5,i3+7,c3)+cr3*ui(
     & i1+3,i2+5,i3+7,c3)+cr4*ui(i1+4,i2+5,i3+7,c3)+cr5*ui(i1+5,i2+5,
     & i3+7,c3)+cr6*ui(i1+6,i2+5,i3+7,c3)+cr7*ui(i1+7,i2+5,i3+7,c3)+
     & cr8*ui(i1+8,i2+5,i3+7,c3))+cs6*(cr0*ui(i1,i2+6,i3+7,c3)+cr1*ui(
     & i1+1,i2+6,i3+7,c3)+cr2*ui(i1+2,i2+6,i3+7,c3)+cr3*ui(i1+3,i2+6,
     & i3+7,c3)+cr4*ui(i1+4,i2+6,i3+7,c3)+cr5*ui(i1+5,i2+6,i3+7,c3)+
     & cr6*ui(i1+6,i2+6,i3+7,c3)+cr7*ui(i1+7,i2+6,i3+7,c3)+cr8*ui(i1+
     & 8,i2+6,i3+7,c3))+cs7*(cr0*ui(i1,i2+7,i3+7,c3)+cr1*ui(i1+1,i2+7,
     & i3+7,c3)+cr2*ui(i1+2,i2+7,i3+7,c3)+cr3*ui(i1+3,i2+7,i3+7,c3)+
     & cr4*ui(i1+4,i2+7,i3+7,c3)+cr5*ui(i1+5,i2+7,i3+7,c3)+cr6*ui(i1+
     & 6,i2+7,i3+7,c3)+cr7*ui(i1+7,i2+7,i3+7,c3)+cr8*ui(i1+8,i2+7,i3+
     & 7,c3))+cs8*(cr0*ui(i1,i2+8,i3+7,c3)+cr1*ui(i1+1,i2+8,i3+7,c3)+
     & cr2*ui(i1+2,i2+8,i3+7,c3)+cr3*ui(i1+3,i2+8,i3+7,c3)+cr4*ui(i1+
     & 4,i2+8,i3+7,c3)+cr5*ui(i1+5,i2+8,i3+7,c3)+cr6*ui(i1+6,i2+8,i3+
     & 7,c3)+cr7*ui(i1+7,i2+8,i3+7,c3)+cr8*ui(i1+8,i2+8,i3+7,c3)))
               r(i) = r(i)+ct8*(cs0*(cr0*ui(i1,i2,i3+8,c3)+cr1*ui(i1+1,
     & i2,i3+8,c3)+cr2*ui(i1+2,i2,i3+8,c3)+cr3*ui(i1+3,i2,i3+8,c3)+
     & cr4*ui(i1+4,i2,i3+8,c3)+cr5*ui(i1+5,i2,i3+8,c3)+cr6*ui(i1+6,i2,
     & i3+8,c3)+cr7*ui(i1+7,i2,i3+8,c3)+cr8*ui(i1+8,i2,i3+8,c3))+cs1*(
     & cr0*ui(i1,i2+1,i3+8,c3)+cr1*ui(i1+1,i2+1,i3+8,c3)+cr2*ui(i1+2,
     & i2+1,i3+8,c3)+cr3*ui(i1+3,i2+1,i3+8,c3)+cr4*ui(i1+4,i2+1,i3+8,
     & c3)+cr5*ui(i1+5,i2+1,i3+8,c3)+cr6*ui(i1+6,i2+1,i3+8,c3)+cr7*ui(
     & i1+7,i2+1,i3+8,c3)+cr8*ui(i1+8,i2+1,i3+8,c3))+cs2*(cr0*ui(i1,
     & i2+2,i3+8,c3)+cr1*ui(i1+1,i2+2,i3+8,c3)+cr2*ui(i1+2,i2+2,i3+8,
     & c3)+cr3*ui(i1+3,i2+2,i3+8,c3)+cr4*ui(i1+4,i2+2,i3+8,c3)+cr5*ui(
     & i1+5,i2+2,i3+8,c3)+cr6*ui(i1+6,i2+2,i3+8,c3)+cr7*ui(i1+7,i2+2,
     & i3+8,c3)+cr8*ui(i1+8,i2+2,i3+8,c3))+cs3*(cr0*ui(i1,i2+3,i3+8,
     & c3)+cr1*ui(i1+1,i2+3,i3+8,c3)+cr2*ui(i1+2,i2+3,i3+8,c3)+cr3*ui(
     & i1+3,i2+3,i3+8,c3)+cr4*ui(i1+4,i2+3,i3+8,c3)+cr5*ui(i1+5,i2+3,
     & i3+8,c3)+cr6*ui(i1+6,i2+3,i3+8,c3)+cr7*ui(i1+7,i2+3,i3+8,c3)+
     & cr8*ui(i1+8,i2+3,i3+8,c3))+cs4*(cr0*ui(i1,i2+4,i3+8,c3)+cr1*ui(
     & i1+1,i2+4,i3+8,c3)+cr2*ui(i1+2,i2+4,i3+8,c3)+cr3*ui(i1+3,i2+4,
     & i3+8,c3)+cr4*ui(i1+4,i2+4,i3+8,c3)+cr5*ui(i1+5,i2+4,i3+8,c3)+
     & cr6*ui(i1+6,i2+4,i3+8,c3)+cr7*ui(i1+7,i2+4,i3+8,c3)+cr8*ui(i1+
     & 8,i2+4,i3+8,c3))+cs5*(cr0*ui(i1,i2+5,i3+8,c3)+cr1*ui(i1+1,i2+5,
     & i3+8,c3)+cr2*ui(i1+2,i2+5,i3+8,c3)+cr3*ui(i1+3,i2+5,i3+8,c3)+
     & cr4*ui(i1+4,i2+5,i3+8,c3)+cr5*ui(i1+5,i2+5,i3+8,c3)+cr6*ui(i1+
     & 6,i2+5,i3+8,c3)+cr7*ui(i1+7,i2+5,i3+8,c3)+cr8*ui(i1+8,i2+5,i3+
     & 8,c3))+cs6*(cr0*ui(i1,i2+6,i3+8,c3)+cr1*ui(i1+1,i2+6,i3+8,c3)+
     & cr2*ui(i1+2,i2+6,i3+8,c3)+cr3*ui(i1+3,i2+6,i3+8,c3)+cr4*ui(i1+
     & 4,i2+6,i3+8,c3)+cr5*ui(i1+5,i2+6,i3+8,c3)+cr6*ui(i1+6,i2+6,i3+
     & 8,c3)+cr7*ui(i1+7,i2+6,i3+8,c3)+cr8*ui(i1+8,i2+6,i3+8,c3))+cs7*
     & (cr0*ui(i1,i2+7,i3+8,c3)+cr1*ui(i1+1,i2+7,i3+8,c3)+cr2*ui(i1+2,
     & i2+7,i3+8,c3)+cr3*ui(i1+3,i2+7,i3+8,c3)+cr4*ui(i1+4,i2+7,i3+8,
     & c3)+cr5*ui(i1+5,i2+7,i3+8,c3)+cr6*ui(i1+6,i2+7,i3+8,c3)+cr7*ui(
     & i1+7,i2+7,i3+8,c3)+cr8*ui(i1+8,i2+7,i3+8,c3))+cs8*(cr0*ui(i1,
     & i2+8,i3+8,c3)+cr1*ui(i1+1,i2+8,i3+8,c3)+cr2*ui(i1+2,i2+8,i3+8,
     & c3)+cr3*ui(i1+3,i2+8,i3+8,c3)+cr4*ui(i1+4,i2+8,i3+8,c3)+cr5*ui(
     & i1+5,i2+8,i3+8,c3)+cr6*ui(i1+6,i2+8,i3+8,c3)+cr7*ui(i1+7,i2+8,
     & i3+8,c3)+cr8*ui(i1+8,i2+8,i3+8,c3)))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
! interpSparseStorage999(r(i))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q90(c(i,0,0,0))
                 cs0 = q90(c(i,1,0,0))
                 ct0 = q90(c(i,2,0,0))
                 cr1 = q91(c(i,0,0,0))
                 cs1 = q91(c(i,1,0,0))
                 ct1 = q91(c(i,2,0,0))
                 cr2 = q92(c(i,0,0,0))
                 cs2 = q92(c(i,1,0,0))
                 ct2 = q92(c(i,2,0,0))
                 cr3 = q93(c(i,0,0,0))
                 cs3 = q93(c(i,1,0,0))
                 ct3 = q93(c(i,2,0,0))
                 cr4 = q94(c(i,0,0,0))
                 cs4 = q94(c(i,1,0,0))
                 ct4 = q94(c(i,2,0,0))
                 cr5 = q95(c(i,0,0,0))
                 cs5 = q95(c(i,1,0,0))
                 ct5 = q95(c(i,2,0,0))
                 cr6 = q96(c(i,0,0,0))
                 cs6 = q96(c(i,1,0,0))
                 ct6 = q96(c(i,2,0,0))
                 cr7 = q97(c(i,0,0,0))
                 cs7 = q97(c(i,1,0,0))
                 ct7 = q97(c(i,2,0,0))
                 cr8 = q98(c(i,0,0,0))
                 cs8 = q98(c(i,1,0,0))
                 ct8 = q98(c(i,2,0,0))
               r(i) = ct0*(cs0*(cr0*ui(i1,i2,i3+0,c3)+cr1*ui(i1+1,i2,
     & i3+0,c3)+cr2*ui(i1+2,i2,i3+0,c3)+cr3*ui(i1+3,i2,i3+0,c3)+cr4*
     & ui(i1+4,i2,i3+0,c3)+cr5*ui(i1+5,i2,i3+0,c3)+cr6*ui(i1+6,i2,i3+
     & 0,c3)+cr7*ui(i1+7,i2,i3+0,c3)+cr8*ui(i1+8,i2,i3+0,c3))+cs1*(
     & cr0*ui(i1,i2+1,i3+0,c3)+cr1*ui(i1+1,i2+1,i3+0,c3)+cr2*ui(i1+2,
     & i2+1,i3+0,c3)+cr3*ui(i1+3,i2+1,i3+0,c3)+cr4*ui(i1+4,i2+1,i3+0,
     & c3)+cr5*ui(i1+5,i2+1,i3+0,c3)+cr6*ui(i1+6,i2+1,i3+0,c3)+cr7*ui(
     & i1+7,i2+1,i3+0,c3)+cr8*ui(i1+8,i2+1,i3+0,c3))+cs2*(cr0*ui(i1,
     & i2+2,i3+0,c3)+cr1*ui(i1+1,i2+2,i3+0,c3)+cr2*ui(i1+2,i2+2,i3+0,
     & c3)+cr3*ui(i1+3,i2+2,i3+0,c3)+cr4*ui(i1+4,i2+2,i3+0,c3)+cr5*ui(
     & i1+5,i2+2,i3+0,c3)+cr6*ui(i1+6,i2+2,i3+0,c3)+cr7*ui(i1+7,i2+2,
     & i3+0,c3)+cr8*ui(i1+8,i2+2,i3+0,c3))+cs3*(cr0*ui(i1,i2+3,i3+0,
     & c3)+cr1*ui(i1+1,i2+3,i3+0,c3)+cr2*ui(i1+2,i2+3,i3+0,c3)+cr3*ui(
     & i1+3,i2+3,i3+0,c3)+cr4*ui(i1+4,i2+3,i3+0,c3)+cr5*ui(i1+5,i2+3,
     & i3+0,c3)+cr6*ui(i1+6,i2+3,i3+0,c3)+cr7*ui(i1+7,i2+3,i3+0,c3)+
     & cr8*ui(i1+8,i2+3,i3+0,c3))+cs4*(cr0*ui(i1,i2+4,i3+0,c3)+cr1*ui(
     & i1+1,i2+4,i3+0,c3)+cr2*ui(i1+2,i2+4,i3+0,c3)+cr3*ui(i1+3,i2+4,
     & i3+0,c3)+cr4*ui(i1+4,i2+4,i3+0,c3)+cr5*ui(i1+5,i2+4,i3+0,c3)+
     & cr6*ui(i1+6,i2+4,i3+0,c3)+cr7*ui(i1+7,i2+4,i3+0,c3)+cr8*ui(i1+
     & 8,i2+4,i3+0,c3))+cs5*(cr0*ui(i1,i2+5,i3+0,c3)+cr1*ui(i1+1,i2+5,
     & i3+0,c3)+cr2*ui(i1+2,i2+5,i3+0,c3)+cr3*ui(i1+3,i2+5,i3+0,c3)+
     & cr4*ui(i1+4,i2+5,i3+0,c3)+cr5*ui(i1+5,i2+5,i3+0,c3)+cr6*ui(i1+
     & 6,i2+5,i3+0,c3)+cr7*ui(i1+7,i2+5,i3+0,c3)+cr8*ui(i1+8,i2+5,i3+
     & 0,c3))+cs6*(cr0*ui(i1,i2+6,i3+0,c3)+cr1*ui(i1+1,i2+6,i3+0,c3)+
     & cr2*ui(i1+2,i2+6,i3+0,c3)+cr3*ui(i1+3,i2+6,i3+0,c3)+cr4*ui(i1+
     & 4,i2+6,i3+0,c3)+cr5*ui(i1+5,i2+6,i3+0,c3)+cr6*ui(i1+6,i2+6,i3+
     & 0,c3)+cr7*ui(i1+7,i2+6,i3+0,c3)+cr8*ui(i1+8,i2+6,i3+0,c3))+cs7*
     & (cr0*ui(i1,i2+7,i3+0,c3)+cr1*ui(i1+1,i2+7,i3+0,c3)+cr2*ui(i1+2,
     & i2+7,i3+0,c3)+cr3*ui(i1+3,i2+7,i3+0,c3)+cr4*ui(i1+4,i2+7,i3+0,
     & c3)+cr5*ui(i1+5,i2+7,i3+0,c3)+cr6*ui(i1+6,i2+7,i3+0,c3)+cr7*ui(
     & i1+7,i2+7,i3+0,c3)+cr8*ui(i1+8,i2+7,i3+0,c3))+cs8*(cr0*ui(i1,
     & i2+8,i3+0,c3)+cr1*ui(i1+1,i2+8,i3+0,c3)+cr2*ui(i1+2,i2+8,i3+0,
     & c3)+cr3*ui(i1+3,i2+8,i3+0,c3)+cr4*ui(i1+4,i2+8,i3+0,c3)+cr5*ui(
     & i1+5,i2+8,i3+0,c3)+cr6*ui(i1+6,i2+8,i3+0,c3)+cr7*ui(i1+7,i2+8,
     & i3+0,c3)+cr8*ui(i1+8,i2+8,i3+0,c3)))+ct1*(cs0*(cr0*ui(i1,i2,i3+
     & 1,c3)+cr1*ui(i1+1,i2,i3+1,c3)+cr2*ui(i1+2,i2,i3+1,c3)+cr3*ui(
     & i1+3,i2,i3+1,c3)+cr4*ui(i1+4,i2,i3+1,c3)+cr5*ui(i1+5,i2,i3+1,
     & c3)+cr6*ui(i1+6,i2,i3+1,c3)+cr7*ui(i1+7,i2,i3+1,c3)+cr8*ui(i1+
     & 8,i2,i3+1,c3))+cs1*(cr0*ui(i1,i2+1,i3+1,c3)+cr1*ui(i1+1,i2+1,
     & i3+1,c3)+cr2*ui(i1+2,i2+1,i3+1,c3)+cr3*ui(i1+3,i2+1,i3+1,c3)+
     & cr4*ui(i1+4,i2+1,i3+1,c3)+cr5*ui(i1+5,i2+1,i3+1,c3)+cr6*ui(i1+
     & 6,i2+1,i3+1,c3)+cr7*ui(i1+7,i2+1,i3+1,c3)+cr8*ui(i1+8,i2+1,i3+
     & 1,c3))+cs2*(cr0*ui(i1,i2+2,i3+1,c3)+cr1*ui(i1+1,i2+2,i3+1,c3)+
     & cr2*ui(i1+2,i2+2,i3+1,c3)+cr3*ui(i1+3,i2+2,i3+1,c3)+cr4*ui(i1+
     & 4,i2+2,i3+1,c3)+cr5*ui(i1+5,i2+2,i3+1,c3)+cr6*ui(i1+6,i2+2,i3+
     & 1,c3)+cr7*ui(i1+7,i2+2,i3+1,c3)+cr8*ui(i1+8,i2+2,i3+1,c3))+cs3*
     & (cr0*ui(i1,i2+3,i3+1,c3)+cr1*ui(i1+1,i2+3,i3+1,c3)+cr2*ui(i1+2,
     & i2+3,i3+1,c3)+cr3*ui(i1+3,i2+3,i3+1,c3)+cr4*ui(i1+4,i2+3,i3+1,
     & c3)+cr5*ui(i1+5,i2+3,i3+1,c3)+cr6*ui(i1+6,i2+3,i3+1,c3)+cr7*ui(
     & i1+7,i2+3,i3+1,c3)+cr8*ui(i1+8,i2+3,i3+1,c3))+cs4*(cr0*ui(i1,
     & i2+4,i3+1,c3)+cr1*ui(i1+1,i2+4,i3+1,c3)+cr2*ui(i1+2,i2+4,i3+1,
     & c3)+cr3*ui(i1+3,i2+4,i3+1,c3)+cr4*ui(i1+4,i2+4,i3+1,c3)+cr5*ui(
     & i1+5,i2+4,i3+1,c3)+cr6*ui(i1+6,i2+4,i3+1,c3)+cr7*ui(i1+7,i2+4,
     & i3+1,c3)+cr8*ui(i1+8,i2+4,i3+1,c3))+cs5*(cr0*ui(i1,i2+5,i3+1,
     & c3)+cr1*ui(i1+1,i2+5,i3+1,c3)+cr2*ui(i1+2,i2+5,i3+1,c3)+cr3*ui(
     & i1+3,i2+5,i3+1,c3)+cr4*ui(i1+4,i2+5,i3+1,c3)+cr5*ui(i1+5,i2+5,
     & i3+1,c3)+cr6*ui(i1+6,i2+5,i3+1,c3)+cr7*ui(i1+7,i2+5,i3+1,c3)+
     & cr8*ui(i1+8,i2+5,i3+1,c3))+cs6*(cr0*ui(i1,i2+6,i3+1,c3)+cr1*ui(
     & i1+1,i2+6,i3+1,c3)+cr2*ui(i1+2,i2+6,i3+1,c3)+cr3*ui(i1+3,i2+6,
     & i3+1,c3)+cr4*ui(i1+4,i2+6,i3+1,c3)+cr5*ui(i1+5,i2+6,i3+1,c3)+
     & cr6*ui(i1+6,i2+6,i3+1,c3)+cr7*ui(i1+7,i2+6,i3+1,c3)+cr8*ui(i1+
     & 8,i2+6,i3+1,c3))+cs7*(cr0*ui(i1,i2+7,i3+1,c3)+cr1*ui(i1+1,i2+7,
     & i3+1,c3)+cr2*ui(i1+2,i2+7,i3+1,c3)+cr3*ui(i1+3,i2+7,i3+1,c3)+
     & cr4*ui(i1+4,i2+7,i3+1,c3)+cr5*ui(i1+5,i2+7,i3+1,c3)+cr6*ui(i1+
     & 6,i2+7,i3+1,c3)+cr7*ui(i1+7,i2+7,i3+1,c3)+cr8*ui(i1+8,i2+7,i3+
     & 1,c3))+cs8*(cr0*ui(i1,i2+8,i3+1,c3)+cr1*ui(i1+1,i2+8,i3+1,c3)+
     & cr2*ui(i1+2,i2+8,i3+1,c3)+cr3*ui(i1+3,i2+8,i3+1,c3)+cr4*ui(i1+
     & 4,i2+8,i3+1,c3)+cr5*ui(i1+5,i2+8,i3+1,c3)+cr6*ui(i1+6,i2+8,i3+
     & 1,c3)+cr7*ui(i1+7,i2+8,i3+1,c3)+cr8*ui(i1+8,i2+8,i3+1,c3)))
               r(i) = r(i)+ct2*(cs0*(cr0*ui(i1,i2,i3+2,c3)+cr1*ui(i1+1,
     & i2,i3+2,c3)+cr2*ui(i1+2,i2,i3+2,c3)+cr3*ui(i1+3,i2,i3+2,c3)+
     & cr4*ui(i1+4,i2,i3+2,c3)+cr5*ui(i1+5,i2,i3+2,c3)+cr6*ui(i1+6,i2,
     & i3+2,c3)+cr7*ui(i1+7,i2,i3+2,c3)+cr8*ui(i1+8,i2,i3+2,c3))+cs1*(
     & cr0*ui(i1,i2+1,i3+2,c3)+cr1*ui(i1+1,i2+1,i3+2,c3)+cr2*ui(i1+2,
     & i2+1,i3+2,c3)+cr3*ui(i1+3,i2+1,i3+2,c3)+cr4*ui(i1+4,i2+1,i3+2,
     & c3)+cr5*ui(i1+5,i2+1,i3+2,c3)+cr6*ui(i1+6,i2+1,i3+2,c3)+cr7*ui(
     & i1+7,i2+1,i3+2,c3)+cr8*ui(i1+8,i2+1,i3+2,c3))+cs2*(cr0*ui(i1,
     & i2+2,i3+2,c3)+cr1*ui(i1+1,i2+2,i3+2,c3)+cr2*ui(i1+2,i2+2,i3+2,
     & c3)+cr3*ui(i1+3,i2+2,i3+2,c3)+cr4*ui(i1+4,i2+2,i3+2,c3)+cr5*ui(
     & i1+5,i2+2,i3+2,c3)+cr6*ui(i1+6,i2+2,i3+2,c3)+cr7*ui(i1+7,i2+2,
     & i3+2,c3)+cr8*ui(i1+8,i2+2,i3+2,c3))+cs3*(cr0*ui(i1,i2+3,i3+2,
     & c3)+cr1*ui(i1+1,i2+3,i3+2,c3)+cr2*ui(i1+2,i2+3,i3+2,c3)+cr3*ui(
     & i1+3,i2+3,i3+2,c3)+cr4*ui(i1+4,i2+3,i3+2,c3)+cr5*ui(i1+5,i2+3,
     & i3+2,c3)+cr6*ui(i1+6,i2+3,i3+2,c3)+cr7*ui(i1+7,i2+3,i3+2,c3)+
     & cr8*ui(i1+8,i2+3,i3+2,c3))+cs4*(cr0*ui(i1,i2+4,i3+2,c3)+cr1*ui(
     & i1+1,i2+4,i3+2,c3)+cr2*ui(i1+2,i2+4,i3+2,c3)+cr3*ui(i1+3,i2+4,
     & i3+2,c3)+cr4*ui(i1+4,i2+4,i3+2,c3)+cr5*ui(i1+5,i2+4,i3+2,c3)+
     & cr6*ui(i1+6,i2+4,i3+2,c3)+cr7*ui(i1+7,i2+4,i3+2,c3)+cr8*ui(i1+
     & 8,i2+4,i3+2,c3))+cs5*(cr0*ui(i1,i2+5,i3+2,c3)+cr1*ui(i1+1,i2+5,
     & i3+2,c3)+cr2*ui(i1+2,i2+5,i3+2,c3)+cr3*ui(i1+3,i2+5,i3+2,c3)+
     & cr4*ui(i1+4,i2+5,i3+2,c3)+cr5*ui(i1+5,i2+5,i3+2,c3)+cr6*ui(i1+
     & 6,i2+5,i3+2,c3)+cr7*ui(i1+7,i2+5,i3+2,c3)+cr8*ui(i1+8,i2+5,i3+
     & 2,c3))+cs6*(cr0*ui(i1,i2+6,i3+2,c3)+cr1*ui(i1+1,i2+6,i3+2,c3)+
     & cr2*ui(i1+2,i2+6,i3+2,c3)+cr3*ui(i1+3,i2+6,i3+2,c3)+cr4*ui(i1+
     & 4,i2+6,i3+2,c3)+cr5*ui(i1+5,i2+6,i3+2,c3)+cr6*ui(i1+6,i2+6,i3+
     & 2,c3)+cr7*ui(i1+7,i2+6,i3+2,c3)+cr8*ui(i1+8,i2+6,i3+2,c3))+cs7*
     & (cr0*ui(i1,i2+7,i3+2,c3)+cr1*ui(i1+1,i2+7,i3+2,c3)+cr2*ui(i1+2,
     & i2+7,i3+2,c3)+cr3*ui(i1+3,i2+7,i3+2,c3)+cr4*ui(i1+4,i2+7,i3+2,
     & c3)+cr5*ui(i1+5,i2+7,i3+2,c3)+cr6*ui(i1+6,i2+7,i3+2,c3)+cr7*ui(
     & i1+7,i2+7,i3+2,c3)+cr8*ui(i1+8,i2+7,i3+2,c3))+cs8*(cr0*ui(i1,
     & i2+8,i3+2,c3)+cr1*ui(i1+1,i2+8,i3+2,c3)+cr2*ui(i1+2,i2+8,i3+2,
     & c3)+cr3*ui(i1+3,i2+8,i3+2,c3)+cr4*ui(i1+4,i2+8,i3+2,c3)+cr5*ui(
     & i1+5,i2+8,i3+2,c3)+cr6*ui(i1+6,i2+8,i3+2,c3)+cr7*ui(i1+7,i2+8,
     & i3+2,c3)+cr8*ui(i1+8,i2+8,i3+2,c3)))+ct3*(cs0*(cr0*ui(i1,i2,i3+
     & 3,c3)+cr1*ui(i1+1,i2,i3+3,c3)+cr2*ui(i1+2,i2,i3+3,c3)+cr3*ui(
     & i1+3,i2,i3+3,c3)+cr4*ui(i1+4,i2,i3+3,c3)+cr5*ui(i1+5,i2,i3+3,
     & c3)+cr6*ui(i1+6,i2,i3+3,c3)+cr7*ui(i1+7,i2,i3+3,c3)+cr8*ui(i1+
     & 8,i2,i3+3,c3))+cs1*(cr0*ui(i1,i2+1,i3+3,c3)+cr1*ui(i1+1,i2+1,
     & i3+3,c3)+cr2*ui(i1+2,i2+1,i3+3,c3)+cr3*ui(i1+3,i2+1,i3+3,c3)+
     & cr4*ui(i1+4,i2+1,i3+3,c3)+cr5*ui(i1+5,i2+1,i3+3,c3)+cr6*ui(i1+
     & 6,i2+1,i3+3,c3)+cr7*ui(i1+7,i2+1,i3+3,c3)+cr8*ui(i1+8,i2+1,i3+
     & 3,c3))+cs2*(cr0*ui(i1,i2+2,i3+3,c3)+cr1*ui(i1+1,i2+2,i3+3,c3)+
     & cr2*ui(i1+2,i2+2,i3+3,c3)+cr3*ui(i1+3,i2+2,i3+3,c3)+cr4*ui(i1+
     & 4,i2+2,i3+3,c3)+cr5*ui(i1+5,i2+2,i3+3,c3)+cr6*ui(i1+6,i2+2,i3+
     & 3,c3)+cr7*ui(i1+7,i2+2,i3+3,c3)+cr8*ui(i1+8,i2+2,i3+3,c3))+cs3*
     & (cr0*ui(i1,i2+3,i3+3,c3)+cr1*ui(i1+1,i2+3,i3+3,c3)+cr2*ui(i1+2,
     & i2+3,i3+3,c3)+cr3*ui(i1+3,i2+3,i3+3,c3)+cr4*ui(i1+4,i2+3,i3+3,
     & c3)+cr5*ui(i1+5,i2+3,i3+3,c3)+cr6*ui(i1+6,i2+3,i3+3,c3)+cr7*ui(
     & i1+7,i2+3,i3+3,c3)+cr8*ui(i1+8,i2+3,i3+3,c3))+cs4*(cr0*ui(i1,
     & i2+4,i3+3,c3)+cr1*ui(i1+1,i2+4,i3+3,c3)+cr2*ui(i1+2,i2+4,i3+3,
     & c3)+cr3*ui(i1+3,i2+4,i3+3,c3)+cr4*ui(i1+4,i2+4,i3+3,c3)+cr5*ui(
     & i1+5,i2+4,i3+3,c3)+cr6*ui(i1+6,i2+4,i3+3,c3)+cr7*ui(i1+7,i2+4,
     & i3+3,c3)+cr8*ui(i1+8,i2+4,i3+3,c3))+cs5*(cr0*ui(i1,i2+5,i3+3,
     & c3)+cr1*ui(i1+1,i2+5,i3+3,c3)+cr2*ui(i1+2,i2+5,i3+3,c3)+cr3*ui(
     & i1+3,i2+5,i3+3,c3)+cr4*ui(i1+4,i2+5,i3+3,c3)+cr5*ui(i1+5,i2+5,
     & i3+3,c3)+cr6*ui(i1+6,i2+5,i3+3,c3)+cr7*ui(i1+7,i2+5,i3+3,c3)+
     & cr8*ui(i1+8,i2+5,i3+3,c3))+cs6*(cr0*ui(i1,i2+6,i3+3,c3)+cr1*ui(
     & i1+1,i2+6,i3+3,c3)+cr2*ui(i1+2,i2+6,i3+3,c3)+cr3*ui(i1+3,i2+6,
     & i3+3,c3)+cr4*ui(i1+4,i2+6,i3+3,c3)+cr5*ui(i1+5,i2+6,i3+3,c3)+
     & cr6*ui(i1+6,i2+6,i3+3,c3)+cr7*ui(i1+7,i2+6,i3+3,c3)+cr8*ui(i1+
     & 8,i2+6,i3+3,c3))+cs7*(cr0*ui(i1,i2+7,i3+3,c3)+cr1*ui(i1+1,i2+7,
     & i3+3,c3)+cr2*ui(i1+2,i2+7,i3+3,c3)+cr3*ui(i1+3,i2+7,i3+3,c3)+
     & cr4*ui(i1+4,i2+7,i3+3,c3)+cr5*ui(i1+5,i2+7,i3+3,c3)+cr6*ui(i1+
     & 6,i2+7,i3+3,c3)+cr7*ui(i1+7,i2+7,i3+3,c3)+cr8*ui(i1+8,i2+7,i3+
     & 3,c3))+cs8*(cr0*ui(i1,i2+8,i3+3,c3)+cr1*ui(i1+1,i2+8,i3+3,c3)+
     & cr2*ui(i1+2,i2+8,i3+3,c3)+cr3*ui(i1+3,i2+8,i3+3,c3)+cr4*ui(i1+
     & 4,i2+8,i3+3,c3)+cr5*ui(i1+5,i2+8,i3+3,c3)+cr6*ui(i1+6,i2+8,i3+
     & 3,c3)+cr7*ui(i1+7,i2+8,i3+3,c3)+cr8*ui(i1+8,i2+8,i3+3,c3)))
               r(i) = r(i)+ct4*(cs0*(cr0*ui(i1,i2,i3+4,c3)+cr1*ui(i1+1,
     & i2,i3+4,c3)+cr2*ui(i1+2,i2,i3+4,c3)+cr3*ui(i1+3,i2,i3+4,c3)+
     & cr4*ui(i1+4,i2,i3+4,c3)+cr5*ui(i1+5,i2,i3+4,c3)+cr6*ui(i1+6,i2,
     & i3+4,c3)+cr7*ui(i1+7,i2,i3+4,c3)+cr8*ui(i1+8,i2,i3+4,c3))+cs1*(
     & cr0*ui(i1,i2+1,i3+4,c3)+cr1*ui(i1+1,i2+1,i3+4,c3)+cr2*ui(i1+2,
     & i2+1,i3+4,c3)+cr3*ui(i1+3,i2+1,i3+4,c3)+cr4*ui(i1+4,i2+1,i3+4,
     & c3)+cr5*ui(i1+5,i2+1,i3+4,c3)+cr6*ui(i1+6,i2+1,i3+4,c3)+cr7*ui(
     & i1+7,i2+1,i3+4,c3)+cr8*ui(i1+8,i2+1,i3+4,c3))+cs2*(cr0*ui(i1,
     & i2+2,i3+4,c3)+cr1*ui(i1+1,i2+2,i3+4,c3)+cr2*ui(i1+2,i2+2,i3+4,
     & c3)+cr3*ui(i1+3,i2+2,i3+4,c3)+cr4*ui(i1+4,i2+2,i3+4,c3)+cr5*ui(
     & i1+5,i2+2,i3+4,c3)+cr6*ui(i1+6,i2+2,i3+4,c3)+cr7*ui(i1+7,i2+2,
     & i3+4,c3)+cr8*ui(i1+8,i2+2,i3+4,c3))+cs3*(cr0*ui(i1,i2+3,i3+4,
     & c3)+cr1*ui(i1+1,i2+3,i3+4,c3)+cr2*ui(i1+2,i2+3,i3+4,c3)+cr3*ui(
     & i1+3,i2+3,i3+4,c3)+cr4*ui(i1+4,i2+3,i3+4,c3)+cr5*ui(i1+5,i2+3,
     & i3+4,c3)+cr6*ui(i1+6,i2+3,i3+4,c3)+cr7*ui(i1+7,i2+3,i3+4,c3)+
     & cr8*ui(i1+8,i2+3,i3+4,c3))+cs4*(cr0*ui(i1,i2+4,i3+4,c3)+cr1*ui(
     & i1+1,i2+4,i3+4,c3)+cr2*ui(i1+2,i2+4,i3+4,c3)+cr3*ui(i1+3,i2+4,
     & i3+4,c3)+cr4*ui(i1+4,i2+4,i3+4,c3)+cr5*ui(i1+5,i2+4,i3+4,c3)+
     & cr6*ui(i1+6,i2+4,i3+4,c3)+cr7*ui(i1+7,i2+4,i3+4,c3)+cr8*ui(i1+
     & 8,i2+4,i3+4,c3))+cs5*(cr0*ui(i1,i2+5,i3+4,c3)+cr1*ui(i1+1,i2+5,
     & i3+4,c3)+cr2*ui(i1+2,i2+5,i3+4,c3)+cr3*ui(i1+3,i2+5,i3+4,c3)+
     & cr4*ui(i1+4,i2+5,i3+4,c3)+cr5*ui(i1+5,i2+5,i3+4,c3)+cr6*ui(i1+
     & 6,i2+5,i3+4,c3)+cr7*ui(i1+7,i2+5,i3+4,c3)+cr8*ui(i1+8,i2+5,i3+
     & 4,c3))+cs6*(cr0*ui(i1,i2+6,i3+4,c3)+cr1*ui(i1+1,i2+6,i3+4,c3)+
     & cr2*ui(i1+2,i2+6,i3+4,c3)+cr3*ui(i1+3,i2+6,i3+4,c3)+cr4*ui(i1+
     & 4,i2+6,i3+4,c3)+cr5*ui(i1+5,i2+6,i3+4,c3)+cr6*ui(i1+6,i2+6,i3+
     & 4,c3)+cr7*ui(i1+7,i2+6,i3+4,c3)+cr8*ui(i1+8,i2+6,i3+4,c3))+cs7*
     & (cr0*ui(i1,i2+7,i3+4,c3)+cr1*ui(i1+1,i2+7,i3+4,c3)+cr2*ui(i1+2,
     & i2+7,i3+4,c3)+cr3*ui(i1+3,i2+7,i3+4,c3)+cr4*ui(i1+4,i2+7,i3+4,
     & c3)+cr5*ui(i1+5,i2+7,i3+4,c3)+cr6*ui(i1+6,i2+7,i3+4,c3)+cr7*ui(
     & i1+7,i2+7,i3+4,c3)+cr8*ui(i1+8,i2+7,i3+4,c3))+cs8*(cr0*ui(i1,
     & i2+8,i3+4,c3)+cr1*ui(i1+1,i2+8,i3+4,c3)+cr2*ui(i1+2,i2+8,i3+4,
     & c3)+cr3*ui(i1+3,i2+8,i3+4,c3)+cr4*ui(i1+4,i2+8,i3+4,c3)+cr5*ui(
     & i1+5,i2+8,i3+4,c3)+cr6*ui(i1+6,i2+8,i3+4,c3)+cr7*ui(i1+7,i2+8,
     & i3+4,c3)+cr8*ui(i1+8,i2+8,i3+4,c3)))+ct5*(cs0*(cr0*ui(i1,i2,i3+
     & 5,c3)+cr1*ui(i1+1,i2,i3+5,c3)+cr2*ui(i1+2,i2,i3+5,c3)+cr3*ui(
     & i1+3,i2,i3+5,c3)+cr4*ui(i1+4,i2,i3+5,c3)+cr5*ui(i1+5,i2,i3+5,
     & c3)+cr6*ui(i1+6,i2,i3+5,c3)+cr7*ui(i1+7,i2,i3+5,c3)+cr8*ui(i1+
     & 8,i2,i3+5,c3))+cs1*(cr0*ui(i1,i2+1,i3+5,c3)+cr1*ui(i1+1,i2+1,
     & i3+5,c3)+cr2*ui(i1+2,i2+1,i3+5,c3)+cr3*ui(i1+3,i2+1,i3+5,c3)+
     & cr4*ui(i1+4,i2+1,i3+5,c3)+cr5*ui(i1+5,i2+1,i3+5,c3)+cr6*ui(i1+
     & 6,i2+1,i3+5,c3)+cr7*ui(i1+7,i2+1,i3+5,c3)+cr8*ui(i1+8,i2+1,i3+
     & 5,c3))+cs2*(cr0*ui(i1,i2+2,i3+5,c3)+cr1*ui(i1+1,i2+2,i3+5,c3)+
     & cr2*ui(i1+2,i2+2,i3+5,c3)+cr3*ui(i1+3,i2+2,i3+5,c3)+cr4*ui(i1+
     & 4,i2+2,i3+5,c3)+cr5*ui(i1+5,i2+2,i3+5,c3)+cr6*ui(i1+6,i2+2,i3+
     & 5,c3)+cr7*ui(i1+7,i2+2,i3+5,c3)+cr8*ui(i1+8,i2+2,i3+5,c3))+cs3*
     & (cr0*ui(i1,i2+3,i3+5,c3)+cr1*ui(i1+1,i2+3,i3+5,c3)+cr2*ui(i1+2,
     & i2+3,i3+5,c3)+cr3*ui(i1+3,i2+3,i3+5,c3)+cr4*ui(i1+4,i2+3,i3+5,
     & c3)+cr5*ui(i1+5,i2+3,i3+5,c3)+cr6*ui(i1+6,i2+3,i3+5,c3)+cr7*ui(
     & i1+7,i2+3,i3+5,c3)+cr8*ui(i1+8,i2+3,i3+5,c3))+cs4*(cr0*ui(i1,
     & i2+4,i3+5,c3)+cr1*ui(i1+1,i2+4,i3+5,c3)+cr2*ui(i1+2,i2+4,i3+5,
     & c3)+cr3*ui(i1+3,i2+4,i3+5,c3)+cr4*ui(i1+4,i2+4,i3+5,c3)+cr5*ui(
     & i1+5,i2+4,i3+5,c3)+cr6*ui(i1+6,i2+4,i3+5,c3)+cr7*ui(i1+7,i2+4,
     & i3+5,c3)+cr8*ui(i1+8,i2+4,i3+5,c3))+cs5*(cr0*ui(i1,i2+5,i3+5,
     & c3)+cr1*ui(i1+1,i2+5,i3+5,c3)+cr2*ui(i1+2,i2+5,i3+5,c3)+cr3*ui(
     & i1+3,i2+5,i3+5,c3)+cr4*ui(i1+4,i2+5,i3+5,c3)+cr5*ui(i1+5,i2+5,
     & i3+5,c3)+cr6*ui(i1+6,i2+5,i3+5,c3)+cr7*ui(i1+7,i2+5,i3+5,c3)+
     & cr8*ui(i1+8,i2+5,i3+5,c3))+cs6*(cr0*ui(i1,i2+6,i3+5,c3)+cr1*ui(
     & i1+1,i2+6,i3+5,c3)+cr2*ui(i1+2,i2+6,i3+5,c3)+cr3*ui(i1+3,i2+6,
     & i3+5,c3)+cr4*ui(i1+4,i2+6,i3+5,c3)+cr5*ui(i1+5,i2+6,i3+5,c3)+
     & cr6*ui(i1+6,i2+6,i3+5,c3)+cr7*ui(i1+7,i2+6,i3+5,c3)+cr8*ui(i1+
     & 8,i2+6,i3+5,c3))+cs7*(cr0*ui(i1,i2+7,i3+5,c3)+cr1*ui(i1+1,i2+7,
     & i3+5,c3)+cr2*ui(i1+2,i2+7,i3+5,c3)+cr3*ui(i1+3,i2+7,i3+5,c3)+
     & cr4*ui(i1+4,i2+7,i3+5,c3)+cr5*ui(i1+5,i2+7,i3+5,c3)+cr6*ui(i1+
     & 6,i2+7,i3+5,c3)+cr7*ui(i1+7,i2+7,i3+5,c3)+cr8*ui(i1+8,i2+7,i3+
     & 5,c3))+cs8*(cr0*ui(i1,i2+8,i3+5,c3)+cr1*ui(i1+1,i2+8,i3+5,c3)+
     & cr2*ui(i1+2,i2+8,i3+5,c3)+cr3*ui(i1+3,i2+8,i3+5,c3)+cr4*ui(i1+
     & 4,i2+8,i3+5,c3)+cr5*ui(i1+5,i2+8,i3+5,c3)+cr6*ui(i1+6,i2+8,i3+
     & 5,c3)+cr7*ui(i1+7,i2+8,i3+5,c3)+cr8*ui(i1+8,i2+8,i3+5,c3)))
               r(i) = r(i)+ct6*(cs0*(cr0*ui(i1,i2,i3+6,c3)+cr1*ui(i1+1,
     & i2,i3+6,c3)+cr2*ui(i1+2,i2,i3+6,c3)+cr3*ui(i1+3,i2,i3+6,c3)+
     & cr4*ui(i1+4,i2,i3+6,c3)+cr5*ui(i1+5,i2,i3+6,c3)+cr6*ui(i1+6,i2,
     & i3+6,c3)+cr7*ui(i1+7,i2,i3+6,c3)+cr8*ui(i1+8,i2,i3+6,c3))+cs1*(
     & cr0*ui(i1,i2+1,i3+6,c3)+cr1*ui(i1+1,i2+1,i3+6,c3)+cr2*ui(i1+2,
     & i2+1,i3+6,c3)+cr3*ui(i1+3,i2+1,i3+6,c3)+cr4*ui(i1+4,i2+1,i3+6,
     & c3)+cr5*ui(i1+5,i2+1,i3+6,c3)+cr6*ui(i1+6,i2+1,i3+6,c3)+cr7*ui(
     & i1+7,i2+1,i3+6,c3)+cr8*ui(i1+8,i2+1,i3+6,c3))+cs2*(cr0*ui(i1,
     & i2+2,i3+6,c3)+cr1*ui(i1+1,i2+2,i3+6,c3)+cr2*ui(i1+2,i2+2,i3+6,
     & c3)+cr3*ui(i1+3,i2+2,i3+6,c3)+cr4*ui(i1+4,i2+2,i3+6,c3)+cr5*ui(
     & i1+5,i2+2,i3+6,c3)+cr6*ui(i1+6,i2+2,i3+6,c3)+cr7*ui(i1+7,i2+2,
     & i3+6,c3)+cr8*ui(i1+8,i2+2,i3+6,c3))+cs3*(cr0*ui(i1,i2+3,i3+6,
     & c3)+cr1*ui(i1+1,i2+3,i3+6,c3)+cr2*ui(i1+2,i2+3,i3+6,c3)+cr3*ui(
     & i1+3,i2+3,i3+6,c3)+cr4*ui(i1+4,i2+3,i3+6,c3)+cr5*ui(i1+5,i2+3,
     & i3+6,c3)+cr6*ui(i1+6,i2+3,i3+6,c3)+cr7*ui(i1+7,i2+3,i3+6,c3)+
     & cr8*ui(i1+8,i2+3,i3+6,c3))+cs4*(cr0*ui(i1,i2+4,i3+6,c3)+cr1*ui(
     & i1+1,i2+4,i3+6,c3)+cr2*ui(i1+2,i2+4,i3+6,c3)+cr3*ui(i1+3,i2+4,
     & i3+6,c3)+cr4*ui(i1+4,i2+4,i3+6,c3)+cr5*ui(i1+5,i2+4,i3+6,c3)+
     & cr6*ui(i1+6,i2+4,i3+6,c3)+cr7*ui(i1+7,i2+4,i3+6,c3)+cr8*ui(i1+
     & 8,i2+4,i3+6,c3))+cs5*(cr0*ui(i1,i2+5,i3+6,c3)+cr1*ui(i1+1,i2+5,
     & i3+6,c3)+cr2*ui(i1+2,i2+5,i3+6,c3)+cr3*ui(i1+3,i2+5,i3+6,c3)+
     & cr4*ui(i1+4,i2+5,i3+6,c3)+cr5*ui(i1+5,i2+5,i3+6,c3)+cr6*ui(i1+
     & 6,i2+5,i3+6,c3)+cr7*ui(i1+7,i2+5,i3+6,c3)+cr8*ui(i1+8,i2+5,i3+
     & 6,c3))+cs6*(cr0*ui(i1,i2+6,i3+6,c3)+cr1*ui(i1+1,i2+6,i3+6,c3)+
     & cr2*ui(i1+2,i2+6,i3+6,c3)+cr3*ui(i1+3,i2+6,i3+6,c3)+cr4*ui(i1+
     & 4,i2+6,i3+6,c3)+cr5*ui(i1+5,i2+6,i3+6,c3)+cr6*ui(i1+6,i2+6,i3+
     & 6,c3)+cr7*ui(i1+7,i2+6,i3+6,c3)+cr8*ui(i1+8,i2+6,i3+6,c3))+cs7*
     & (cr0*ui(i1,i2+7,i3+6,c3)+cr1*ui(i1+1,i2+7,i3+6,c3)+cr2*ui(i1+2,
     & i2+7,i3+6,c3)+cr3*ui(i1+3,i2+7,i3+6,c3)+cr4*ui(i1+4,i2+7,i3+6,
     & c3)+cr5*ui(i1+5,i2+7,i3+6,c3)+cr6*ui(i1+6,i2+7,i3+6,c3)+cr7*ui(
     & i1+7,i2+7,i3+6,c3)+cr8*ui(i1+8,i2+7,i3+6,c3))+cs8*(cr0*ui(i1,
     & i2+8,i3+6,c3)+cr1*ui(i1+1,i2+8,i3+6,c3)+cr2*ui(i1+2,i2+8,i3+6,
     & c3)+cr3*ui(i1+3,i2+8,i3+6,c3)+cr4*ui(i1+4,i2+8,i3+6,c3)+cr5*ui(
     & i1+5,i2+8,i3+6,c3)+cr6*ui(i1+6,i2+8,i3+6,c3)+cr7*ui(i1+7,i2+8,
     & i3+6,c3)+cr8*ui(i1+8,i2+8,i3+6,c3)))+ct7*(cs0*(cr0*ui(i1,i2,i3+
     & 7,c3)+cr1*ui(i1+1,i2,i3+7,c3)+cr2*ui(i1+2,i2,i3+7,c3)+cr3*ui(
     & i1+3,i2,i3+7,c3)+cr4*ui(i1+4,i2,i3+7,c3)+cr5*ui(i1+5,i2,i3+7,
     & c3)+cr6*ui(i1+6,i2,i3+7,c3)+cr7*ui(i1+7,i2,i3+7,c3)+cr8*ui(i1+
     & 8,i2,i3+7,c3))+cs1*(cr0*ui(i1,i2+1,i3+7,c3)+cr1*ui(i1+1,i2+1,
     & i3+7,c3)+cr2*ui(i1+2,i2+1,i3+7,c3)+cr3*ui(i1+3,i2+1,i3+7,c3)+
     & cr4*ui(i1+4,i2+1,i3+7,c3)+cr5*ui(i1+5,i2+1,i3+7,c3)+cr6*ui(i1+
     & 6,i2+1,i3+7,c3)+cr7*ui(i1+7,i2+1,i3+7,c3)+cr8*ui(i1+8,i2+1,i3+
     & 7,c3))+cs2*(cr0*ui(i1,i2+2,i3+7,c3)+cr1*ui(i1+1,i2+2,i3+7,c3)+
     & cr2*ui(i1+2,i2+2,i3+7,c3)+cr3*ui(i1+3,i2+2,i3+7,c3)+cr4*ui(i1+
     & 4,i2+2,i3+7,c3)+cr5*ui(i1+5,i2+2,i3+7,c3)+cr6*ui(i1+6,i2+2,i3+
     & 7,c3)+cr7*ui(i1+7,i2+2,i3+7,c3)+cr8*ui(i1+8,i2+2,i3+7,c3))+cs3*
     & (cr0*ui(i1,i2+3,i3+7,c3)+cr1*ui(i1+1,i2+3,i3+7,c3)+cr2*ui(i1+2,
     & i2+3,i3+7,c3)+cr3*ui(i1+3,i2+3,i3+7,c3)+cr4*ui(i1+4,i2+3,i3+7,
     & c3)+cr5*ui(i1+5,i2+3,i3+7,c3)+cr6*ui(i1+6,i2+3,i3+7,c3)+cr7*ui(
     & i1+7,i2+3,i3+7,c3)+cr8*ui(i1+8,i2+3,i3+7,c3))+cs4*(cr0*ui(i1,
     & i2+4,i3+7,c3)+cr1*ui(i1+1,i2+4,i3+7,c3)+cr2*ui(i1+2,i2+4,i3+7,
     & c3)+cr3*ui(i1+3,i2+4,i3+7,c3)+cr4*ui(i1+4,i2+4,i3+7,c3)+cr5*ui(
     & i1+5,i2+4,i3+7,c3)+cr6*ui(i1+6,i2+4,i3+7,c3)+cr7*ui(i1+7,i2+4,
     & i3+7,c3)+cr8*ui(i1+8,i2+4,i3+7,c3))+cs5*(cr0*ui(i1,i2+5,i3+7,
     & c3)+cr1*ui(i1+1,i2+5,i3+7,c3)+cr2*ui(i1+2,i2+5,i3+7,c3)+cr3*ui(
     & i1+3,i2+5,i3+7,c3)+cr4*ui(i1+4,i2+5,i3+7,c3)+cr5*ui(i1+5,i2+5,
     & i3+7,c3)+cr6*ui(i1+6,i2+5,i3+7,c3)+cr7*ui(i1+7,i2+5,i3+7,c3)+
     & cr8*ui(i1+8,i2+5,i3+7,c3))+cs6*(cr0*ui(i1,i2+6,i3+7,c3)+cr1*ui(
     & i1+1,i2+6,i3+7,c3)+cr2*ui(i1+2,i2+6,i3+7,c3)+cr3*ui(i1+3,i2+6,
     & i3+7,c3)+cr4*ui(i1+4,i2+6,i3+7,c3)+cr5*ui(i1+5,i2+6,i3+7,c3)+
     & cr6*ui(i1+6,i2+6,i3+7,c3)+cr7*ui(i1+7,i2+6,i3+7,c3)+cr8*ui(i1+
     & 8,i2+6,i3+7,c3))+cs7*(cr0*ui(i1,i2+7,i3+7,c3)+cr1*ui(i1+1,i2+7,
     & i3+7,c3)+cr2*ui(i1+2,i2+7,i3+7,c3)+cr3*ui(i1+3,i2+7,i3+7,c3)+
     & cr4*ui(i1+4,i2+7,i3+7,c3)+cr5*ui(i1+5,i2+7,i3+7,c3)+cr6*ui(i1+
     & 6,i2+7,i3+7,c3)+cr7*ui(i1+7,i2+7,i3+7,c3)+cr8*ui(i1+8,i2+7,i3+
     & 7,c3))+cs8*(cr0*ui(i1,i2+8,i3+7,c3)+cr1*ui(i1+1,i2+8,i3+7,c3)+
     & cr2*ui(i1+2,i2+8,i3+7,c3)+cr3*ui(i1+3,i2+8,i3+7,c3)+cr4*ui(i1+
     & 4,i2+8,i3+7,c3)+cr5*ui(i1+5,i2+8,i3+7,c3)+cr6*ui(i1+6,i2+8,i3+
     & 7,c3)+cr7*ui(i1+7,i2+8,i3+7,c3)+cr8*ui(i1+8,i2+8,i3+7,c3)))
               r(i) = r(i)+ct8*(cs0*(cr0*ui(i1,i2,i3+8,c3)+cr1*ui(i1+1,
     & i2,i3+8,c3)+cr2*ui(i1+2,i2,i3+8,c3)+cr3*ui(i1+3,i2,i3+8,c3)+
     & cr4*ui(i1+4,i2,i3+8,c3)+cr5*ui(i1+5,i2,i3+8,c3)+cr6*ui(i1+6,i2,
     & i3+8,c3)+cr7*ui(i1+7,i2,i3+8,c3)+cr8*ui(i1+8,i2,i3+8,c3))+cs1*(
     & cr0*ui(i1,i2+1,i3+8,c3)+cr1*ui(i1+1,i2+1,i3+8,c3)+cr2*ui(i1+2,
     & i2+1,i3+8,c3)+cr3*ui(i1+3,i2+1,i3+8,c3)+cr4*ui(i1+4,i2+1,i3+8,
     & c3)+cr5*ui(i1+5,i2+1,i3+8,c3)+cr6*ui(i1+6,i2+1,i3+8,c3)+cr7*ui(
     & i1+7,i2+1,i3+8,c3)+cr8*ui(i1+8,i2+1,i3+8,c3))+cs2*(cr0*ui(i1,
     & i2+2,i3+8,c3)+cr1*ui(i1+1,i2+2,i3+8,c3)+cr2*ui(i1+2,i2+2,i3+8,
     & c3)+cr3*ui(i1+3,i2+2,i3+8,c3)+cr4*ui(i1+4,i2+2,i3+8,c3)+cr5*ui(
     & i1+5,i2+2,i3+8,c3)+cr6*ui(i1+6,i2+2,i3+8,c3)+cr7*ui(i1+7,i2+2,
     & i3+8,c3)+cr8*ui(i1+8,i2+2,i3+8,c3))+cs3*(cr0*ui(i1,i2+3,i3+8,
     & c3)+cr1*ui(i1+1,i2+3,i3+8,c3)+cr2*ui(i1+2,i2+3,i3+8,c3)+cr3*ui(
     & i1+3,i2+3,i3+8,c3)+cr4*ui(i1+4,i2+3,i3+8,c3)+cr5*ui(i1+5,i2+3,
     & i3+8,c3)+cr6*ui(i1+6,i2+3,i3+8,c3)+cr7*ui(i1+7,i2+3,i3+8,c3)+
     & cr8*ui(i1+8,i2+3,i3+8,c3))+cs4*(cr0*ui(i1,i2+4,i3+8,c3)+cr1*ui(
     & i1+1,i2+4,i3+8,c3)+cr2*ui(i1+2,i2+4,i3+8,c3)+cr3*ui(i1+3,i2+4,
     & i3+8,c3)+cr4*ui(i1+4,i2+4,i3+8,c3)+cr5*ui(i1+5,i2+4,i3+8,c3)+
     & cr6*ui(i1+6,i2+4,i3+8,c3)+cr7*ui(i1+7,i2+4,i3+8,c3)+cr8*ui(i1+
     & 8,i2+4,i3+8,c3))+cs5*(cr0*ui(i1,i2+5,i3+8,c3)+cr1*ui(i1+1,i2+5,
     & i3+8,c3)+cr2*ui(i1+2,i2+5,i3+8,c3)+cr3*ui(i1+3,i2+5,i3+8,c3)+
     & cr4*ui(i1+4,i2+5,i3+8,c3)+cr5*ui(i1+5,i2+5,i3+8,c3)+cr6*ui(i1+
     & 6,i2+5,i3+8,c3)+cr7*ui(i1+7,i2+5,i3+8,c3)+cr8*ui(i1+8,i2+5,i3+
     & 8,c3))+cs6*(cr0*ui(i1,i2+6,i3+8,c3)+cr1*ui(i1+1,i2+6,i3+8,c3)+
     & cr2*ui(i1+2,i2+6,i3+8,c3)+cr3*ui(i1+3,i2+6,i3+8,c3)+cr4*ui(i1+
     & 4,i2+6,i3+8,c3)+cr5*ui(i1+5,i2+6,i3+8,c3)+cr6*ui(i1+6,i2+6,i3+
     & 8,c3)+cr7*ui(i1+7,i2+6,i3+8,c3)+cr8*ui(i1+8,i2+6,i3+8,c3))+cs7*
     & (cr0*ui(i1,i2+7,i3+8,c3)+cr1*ui(i1+1,i2+7,i3+8,c3)+cr2*ui(i1+2,
     & i2+7,i3+8,c3)+cr3*ui(i1+3,i2+7,i3+8,c3)+cr4*ui(i1+4,i2+7,i3+8,
     & c3)+cr5*ui(i1+5,i2+7,i3+8,c3)+cr6*ui(i1+6,i2+7,i3+8,c3)+cr7*ui(
     & i1+7,i2+7,i3+8,c3)+cr8*ui(i1+8,i2+7,i3+8,c3))+cs8*(cr0*ui(i1,
     & i2+8,i3+8,c3)+cr1*ui(i1+1,i2+8,i3+8,c3)+cr2*ui(i1+2,i2+8,i3+8,
     & c3)+cr3*ui(i1+3,i2+8,i3+8,c3)+cr4*ui(i1+4,i2+8,i3+8,c3)+cr5*ui(
     & i1+5,i2+8,i3+8,c3)+cr6*ui(i1+6,i2+8,i3+8,c3)+cr7*ui(i1+7,i2+8,
     & i3+8,c3)+cr8*ui(i1+8,i2+8,i3+8,c3)))
               resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(
     & i)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=r(i)
             end do
             end do
           end if
         else
          !   general case in 3D
           write(*,*) 'ERROR width=',width(1),width(2),width(2)
           stop 1
         end if
       end if
       else
         write(*,*) 'interpOpt:ERROR; unknown storage option=',
     & storageOption
       end if ! end storage option
       return
       end
! defineInterpOpt(SP)
       subroutine interpOptSP ( nd,ndui1a,ndui1b,ndui2a,ndui2b,ndui3a,
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
! #If "SP" == "Full"
c **      else if( storageOption.eq.1 )then
! #If "SP" == "TP"
! #If "SP" == "SP"
       if( storageOption.eq.2 )then
c       ****************************************
c       **** sparse         storage option *****
c       ****************************************
       ! write(*,*) 'interpOpt:sparseStorage interp, width=',width(1)
       if( nd.eq.2 )then
         if( useVariableWidthInterpolation.ne.0 )then
! beginLoops2d()
             do i=nia,nib
             do c3=c3a,c3b
             do c2=c2a,c2b
             ! check for most common widths first
             if( varWidth(i).eq.3 )then
! interpSparseStorage33(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q30(c(i,0,0,0))
                 cs0 = q30(c(i,1,0,0))
                 cr1 = q31(c(i,0,0,0))
                 cs1 = q31(c(i,1,0,0))
                 cr2 = q32(c(i,0,0,0))
                 cs2 = q32(c(i,1,0,0))
               ug(ip(i,1),ip(i,2),c2,c3) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)
     & +cr1*ui(i1+1,i2  ,c2,c3)+cr2*ui(i1+2,i2  ,c2,c3))+cs1*(cr0*ui(
     & i1  ,i2+1,c2,c3)+cr1*ui(i1+1,i2+1,c2,c3)+cr2*ui(i1+2,i2+1,c2,
     & c3))+cs2*(cr0*ui(i1  ,i2+2,c2,c3)+cr1*ui(i1+1,i2+2,c2,c3)+cr2*
     & ui(i1+2,i2+2,c2,c3))
             else if( varWidth(i).eq.2 )then
! interpSparseStorage22(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q20(c(i,0,0,0))
                 cs0 = q20(c(i,1,0,0))
                 cr1 = q21(c(i,0,0,0))
                 cs1 = q21(c(i,1,0,0))
               ug(ip(i,1),ip(i,2),c2,c3) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)
     & +cr1*ui(i1+1,i2  ,c2,c3))+cs1*(cr0*ui(i1  ,i2+1,c2,c3)+cr1*ui(
     & i1+1,i2+1,c2,c3))
             else if( varWidth(i).eq.1 )then
! interp11(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
               ug(ip(i,1),ip(i,2),c2,c3) = ui(i1  ,i2  ,c2,c3)
             else if( varWidth(i).eq.5 )then
! interpSparseStorage55(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q50(c(i,0,0,0))
                 cs0 = q50(c(i,1,0,0))
                 cr1 = q51(c(i,0,0,0))
                 cs1 = q51(c(i,1,0,0))
                 cr2 = q52(c(i,0,0,0))
                 cs2 = q52(c(i,1,0,0))
                 cr3 = q53(c(i,0,0,0))
                 cs3 = q53(c(i,1,0,0))
                 cr4 = q54(c(i,0,0,0))
                 cs4 = q54(c(i,1,0,0))
               ug(ip(i,1),ip(i,2),c2,c3) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)
     & +cr1*ui(i1+1,i2  ,c2,c3)+cr2*ui(i1+2,i2  ,c2,c3)+cr3*ui(i1+3,
     & i2  ,c2,c3)+cr4*ui(i1+4,i2  ,c2,c3))+cs1*(cr0*ui(i1  ,i2+1,c2,
     & c3)+cr1*ui(i1+1,i2+1,c2,c3)+cr2*ui(i1+2,i2+1,c2,c3)+cr3*ui(i1+
     & 3,i2+1,c2,c3)+cr4*ui(i1+4,i2+1,c2,c3))+cs2*(cr0*ui(i1  ,i2+2,
     & c2,c3)+cr1*ui(i1+1,i2+2,c2,c3)+cr2*ui(i1+2,i2+2,c2,c3)+cr3*ui(
     & i1+3,i2+2,c2,c3)+cr4*ui(i1+4,i2+2,c2,c3))+cs3*(cr0*ui(i1  ,i2+
     & 3,c2,c3)+cr1*ui(i1+1,i2+3,c2,c3)+cr2*ui(i1+2,i2+3,c2,c3)+cr3*
     & ui(i1+3,i2+3,c2,c3)+cr4*ui(i1+4,i2+3,c2,c3))+cs4*(cr0*ui(i1  ,
     & i2+4,c2,c3)+cr1*ui(i1+1,i2+4,c2,c3)+cr2*ui(i1+2,i2+4,c2,c3)+
     & cr3*ui(i1+3,i2+4,c2,c3)+cr4*ui(i1+4,i2+4,c2,c3))
             else if( varWidth(i).eq.4 )then
! interpSparseStorage44(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q40(c(i,0,0,0))
                 cs0 = q40(c(i,1,0,0))
                 cr1 = q41(c(i,0,0,0))
                 cs1 = q41(c(i,1,0,0))
                 cr2 = q42(c(i,0,0,0))
                 cs2 = q42(c(i,1,0,0))
                 cr3 = q43(c(i,0,0,0))
                 cs3 = q43(c(i,1,0,0))
               ug(ip(i,1),ip(i,2),c2,c3) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)
     & +cr1*ui(i1+1,i2  ,c2,c3)+cr2*ui(i1+2,i2  ,c2,c3)+cr3*ui(i1+3,
     & i2  ,c2,c3))+cs1*(cr0*ui(i1  ,i2+1,c2,c3)+cr1*ui(i1+1,i2+1,c2,
     & c3)+cr2*ui(i1+2,i2+1,c2,c3)+cr3*ui(i1+3,i2+1,c2,c3))+cs2*(cr0*
     & ui(i1  ,i2+2,c2,c3)+cr1*ui(i1+1,i2+2,c2,c3)+cr2*ui(i1+2,i2+2,
     & c2,c3)+cr3*ui(i1+3,i2+2,c2,c3))+cs3*(cr0*ui(i1  ,i2+3,c2,c3)+
     & cr1*ui(i1+1,i2+3,c2,c3)+cr2*ui(i1+2,i2+3,c2,c3)+cr3*ui(i1+3,i2+
     & 3,c2,c3))
             else if( varWidth(i).eq.7 )then
! interpSparseStorage77(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q70(c(i,0,0,0))
                 cs0 = q70(c(i,1,0,0))
                 cr1 = q71(c(i,0,0,0))
                 cs1 = q71(c(i,1,0,0))
                 cr2 = q72(c(i,0,0,0))
                 cs2 = q72(c(i,1,0,0))
                 cr3 = q73(c(i,0,0,0))
                 cs3 = q73(c(i,1,0,0))
                 cr4 = q74(c(i,0,0,0))
                 cs4 = q74(c(i,1,0,0))
                 cr5 = q75(c(i,0,0,0))
                 cs5 = q75(c(i,1,0,0))
                 cr6 = q76(c(i,0,0,0))
                 cs6 = q76(c(i,1,0,0))
               ug(ip(i,1),ip(i,2),c2,c3) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)
     & +cr1*ui(i1+1,i2  ,c2,c3)+cr2*ui(i1+2,i2  ,c2,c3)+cr3*ui(i1+3,
     & i2  ,c2,c3)+cr4*ui(i1+4,i2  ,c2,c3)+cr5*ui(i1+5,i2  ,c2,c3)+
     & cr6*ui(i1+6,i2  ,c2,c3))+cs1*(cr0*ui(i1  ,i2+1,c2,c3)+cr1*ui(
     & i1+1,i2+1,c2,c3)+cr2*ui(i1+2,i2+1,c2,c3)+cr3*ui(i1+3,i2+1,c2,
     & c3)+cr4*ui(i1+4,i2+1,c2,c3)+cr5*ui(i1+5,i2+1,c2,c3)+cr6*ui(i1+
     & 6,i2+1,c2,c3))+cs2*(cr0*ui(i1  ,i2+2,c2,c3)+cr1*ui(i1+1,i2+2,
     & c2,c3)+cr2*ui(i1+2,i2+2,c2,c3)+cr3*ui(i1+3,i2+2,c2,c3)+cr4*ui(
     & i1+4,i2+2,c2,c3)+cr5*ui(i1+5,i2+2,c2,c3)+cr6*ui(i1+6,i2+2,c2,
     & c3))+cs3*(cr0*ui(i1  ,i2+3,c2,c3)+cr1*ui(i1+1,i2+3,c2,c3)+cr2*
     & ui(i1+2,i2+3,c2,c3)+cr3*ui(i1+3,i2+3,c2,c3)+cr4*ui(i1+4,i2+3,
     & c2,c3)+cr5*ui(i1+5,i2+3,c2,c3)+cr6*ui(i1+6,i2+3,c2,c3))+cs4*(
     & cr0*ui(i1  ,i2+4,c2,c3)+cr1*ui(i1+1,i2+4,c2,c3)+cr2*ui(i1+2,i2+
     & 4,c2,c3)+cr3*ui(i1+3,i2+4,c2,c3)+cr4*ui(i1+4,i2+4,c2,c3)+cr5*
     & ui(i1+5,i2+4,c2,c3)+cr6*ui(i1+6,i2+4,c2,c3))+cs5*(cr0*ui(i1  ,
     & i2+5,c2,c3)+cr1*ui(i1+1,i2+5,c2,c3)+cr2*ui(i1+2,i2+5,c2,c3)+
     & cr3*ui(i1+3,i2+5,c2,c3)+cr4*ui(i1+4,i2+5,c2,c3)+cr5*ui(i1+5,i2+
     & 5,c2,c3)+cr6*ui(i1+6,i2+5,c2,c3))+cs6*(cr0*ui(i1  ,i2+6,c2,c3)+
     & cr1*ui(i1+1,i2+6,c2,c3)+cr2*ui(i1+2,i2+6,c2,c3)+cr3*ui(i1+3,i2+
     & 6,c2,c3)+cr4*ui(i1+4,i2+6,c2,c3)+cr5*ui(i1+5,i2+6,c2,c3)+cr6*
     & ui(i1+6,i2+6,c2,c3))
             else if( varWidth(i).eq.6 )then
! interpSparseStorage66(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q60(c(i,0,0,0))
                 cs0 = q60(c(i,1,0,0))
                 cr1 = q61(c(i,0,0,0))
                 cs1 = q61(c(i,1,0,0))
                 cr2 = q62(c(i,0,0,0))
                 cs2 = q62(c(i,1,0,0))
                 cr3 = q63(c(i,0,0,0))
                 cs3 = q63(c(i,1,0,0))
                 cr4 = q64(c(i,0,0,0))
                 cs4 = q64(c(i,1,0,0))
                 cr5 = q65(c(i,0,0,0))
                 cs5 = q65(c(i,1,0,0))
               ug(ip(i,1),ip(i,2),c2,c3) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)
     & +cr1*ui(i1+1,i2  ,c2,c3)+cr2*ui(i1+2,i2  ,c2,c3)+cr3*ui(i1+3,
     & i2  ,c2,c3)+cr4*ui(i1+4,i2  ,c2,c3)+cr5*ui(i1+5,i2  ,c2,c3))+
     & cs1*(cr0*ui(i1  ,i2+1,c2,c3)+cr1*ui(i1+1,i2+1,c2,c3)+cr2*ui(i1+
     & 2,i2+1,c2,c3)+cr3*ui(i1+3,i2+1,c2,c3)+cr4*ui(i1+4,i2+1,c2,c3)+
     & cr5*ui(i1+5,i2+1,c2,c3))+cs2*(cr0*ui(i1  ,i2+2,c2,c3)+cr1*ui(
     & i1+1,i2+2,c2,c3)+cr2*ui(i1+2,i2+2,c2,c3)+cr3*ui(i1+3,i2+2,c2,
     & c3)+cr4*ui(i1+4,i2+2,c2,c3)+cr5*ui(i1+5,i2+2,c2,c3))+cs3*(cr0*
     & ui(i1  ,i2+3,c2,c3)+cr1*ui(i1+1,i2+3,c2,c3)+cr2*ui(i1+2,i2+3,
     & c2,c3)+cr3*ui(i1+3,i2+3,c2,c3)+cr4*ui(i1+4,i2+3,c2,c3)+cr5*ui(
     & i1+5,i2+3,c2,c3))+cs4*(cr0*ui(i1  ,i2+4,c2,c3)+cr1*ui(i1+1,i2+
     & 4,c2,c3)+cr2*ui(i1+2,i2+4,c2,c3)+cr3*ui(i1+3,i2+4,c2,c3)+cr4*
     & ui(i1+4,i2+4,c2,c3)+cr5*ui(i1+5,i2+4,c2,c3))+cs5*(cr0*ui(i1  ,
     & i2+5,c2,c3)+cr1*ui(i1+1,i2+5,c2,c3)+cr2*ui(i1+2,i2+5,c2,c3)+
     & cr3*ui(i1+3,i2+5,c2,c3)+cr4*ui(i1+4,i2+5,c2,c3)+cr5*ui(i1+5,i2+
     & 5,c2,c3))
             else if( varWidth(i).eq.9 )then
! interpSparseStorage99(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q90(c(i,0,0,0))
                 cs0 = q90(c(i,1,0,0))
                 cr1 = q91(c(i,0,0,0))
                 cs1 = q91(c(i,1,0,0))
                 cr2 = q92(c(i,0,0,0))
                 cs2 = q92(c(i,1,0,0))
                 cr3 = q93(c(i,0,0,0))
                 cs3 = q93(c(i,1,0,0))
                 cr4 = q94(c(i,0,0,0))
                 cs4 = q94(c(i,1,0,0))
                 cr5 = q95(c(i,0,0,0))
                 cs5 = q95(c(i,1,0,0))
                 cr6 = q96(c(i,0,0,0))
                 cs6 = q96(c(i,1,0,0))
                 cr7 = q97(c(i,0,0,0))
                 cs7 = q97(c(i,1,0,0))
                 cr8 = q98(c(i,0,0,0))
                 cs8 = q98(c(i,1,0,0))
               ug(ip(i,1),ip(i,2),c2,c3) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)
     & +cr1*ui(i1+1,i2  ,c2,c3)+cr2*ui(i1+2,i2  ,c2,c3)+cr3*ui(i1+3,
     & i2  ,c2,c3)+cr4*ui(i1+4,i2  ,c2,c3)+cr5*ui(i1+5,i2  ,c2,c3)+
     & cr6*ui(i1+6,i2  ,c2,c3)+cr7*ui(i1+7,i2  ,c2,c3)+cr8*ui(i1+8,i2 
     &  ,c2,c3))+cs1*(cr0*ui(i1  ,i2+1,c2,c3)+cr1*ui(i1+1,i2+1,c2,c3)+
     & cr2*ui(i1+2,i2+1,c2,c3)+cr3*ui(i1+3,i2+1,c2,c3)+cr4*ui(i1+4,i2+
     & 1,c2,c3)+cr5*ui(i1+5,i2+1,c2,c3)+cr6*ui(i1+6,i2+1,c2,c3)+cr7*
     & ui(i1+7,i2+1,c2,c3)+cr8*ui(i1+8,i2+1,c2,c3))+cs2*(cr0*ui(i1  ,
     & i2+2,c2,c3)+cr1*ui(i1+1,i2+2,c2,c3)+cr2*ui(i1+2,i2+2,c2,c3)+
     & cr3*ui(i1+3,i2+2,c2,c3)+cr4*ui(i1+4,i2+2,c2,c3)+cr5*ui(i1+5,i2+
     & 2,c2,c3)+cr6*ui(i1+6,i2+2,c2,c3)+cr7*ui(i1+7,i2+2,c2,c3)+cr8*
     & ui(i1+8,i2+2,c2,c3))+cs3*(cr0*ui(i1  ,i2+3,c2,c3)+cr1*ui(i1+1,
     & i2+3,c2,c3)+cr2*ui(i1+2,i2+3,c2,c3)+cr3*ui(i1+3,i2+3,c2,c3)+
     & cr4*ui(i1+4,i2+3,c2,c3)+cr5*ui(i1+5,i2+3,c2,c3)+cr6*ui(i1+6,i2+
     & 3,c2,c3)+cr7*ui(i1+7,i2+3,c2,c3)+cr8*ui(i1+8,i2+3,c2,c3))+cs4*(
     & cr0*ui(i1  ,i2+4,c2,c3)+cr1*ui(i1+1,i2+4,c2,c3)+cr2*ui(i1+2,i2+
     & 4,c2,c3)+cr3*ui(i1+3,i2+4,c2,c3)+cr4*ui(i1+4,i2+4,c2,c3)+cr5*
     & ui(i1+5,i2+4,c2,c3)+cr6*ui(i1+6,i2+4,c2,c3)+cr7*ui(i1+7,i2+4,
     & c2,c3)+cr8*ui(i1+8,i2+4,c2,c3))+cs5*(cr0*ui(i1  ,i2+5,c2,c3)+
     & cr1*ui(i1+1,i2+5,c2,c3)+cr2*ui(i1+2,i2+5,c2,c3)+cr3*ui(i1+3,i2+
     & 5,c2,c3)+cr4*ui(i1+4,i2+5,c2,c3)+cr5*ui(i1+5,i2+5,c2,c3)+cr6*
     & ui(i1+6,i2+5,c2,c3)+cr7*ui(i1+7,i2+5,c2,c3)+cr8*ui(i1+8,i2+5,
     & c2,c3))+cs6*(cr0*ui(i1  ,i2+6,c2,c3)+cr1*ui(i1+1,i2+6,c2,c3)+
     & cr2*ui(i1+2,i2+6,c2,c3)+cr3*ui(i1+3,i2+6,c2,c3)+cr4*ui(i1+4,i2+
     & 6,c2,c3)+cr5*ui(i1+5,i2+6,c2,c3)+cr6*ui(i1+6,i2+6,c2,c3)+cr7*
     & ui(i1+7,i2+6,c2,c3)+cr8*ui(i1+8,i2+6,c2,c3))+cs7*(cr0*ui(i1  ,
     & i2+7,c2,c3)+cr1*ui(i1+1,i2+7,c2,c3)+cr2*ui(i1+2,i2+7,c2,c3)+
     & cr3*ui(i1+3,i2+7,c2,c3)+cr4*ui(i1+4,i2+7,c2,c3)+cr5*ui(i1+5,i2+
     & 7,c2,c3)+cr6*ui(i1+6,i2+7,c2,c3)+cr7*ui(i1+7,i2+7,c2,c3)+cr8*
     & ui(i1+8,i2+7,c2,c3))+cs8*(cr0*ui(i1  ,i2+8,c2,c3)+cr1*ui(i1+1,
     & i2+8,c2,c3)+cr2*ui(i1+2,i2+8,c2,c3)+cr3*ui(i1+3,i2+8,c2,c3)+
     & cr4*ui(i1+4,i2+8,c2,c3)+cr5*ui(i1+5,i2+8,c2,c3)+cr6*ui(i1+6,i2+
     & 8,c2,c3)+cr7*ui(i1+7,i2+8,c2,c3)+cr8*ui(i1+8,i2+8,c2,c3))
             else if( varWidth(i).eq.8 )then
! interpSparseStorage88(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q80(c(i,0,0,0))
                 cs0 = q80(c(i,1,0,0))
                 cr1 = q81(c(i,0,0,0))
                 cs1 = q81(c(i,1,0,0))
                 cr2 = q82(c(i,0,0,0))
                 cs2 = q82(c(i,1,0,0))
                 cr3 = q83(c(i,0,0,0))
                 cs3 = q83(c(i,1,0,0))
                 cr4 = q84(c(i,0,0,0))
                 cs4 = q84(c(i,1,0,0))
                 cr5 = q85(c(i,0,0,0))
                 cs5 = q85(c(i,1,0,0))
                 cr6 = q86(c(i,0,0,0))
                 cs6 = q86(c(i,1,0,0))
                 cr7 = q87(c(i,0,0,0))
                 cs7 = q87(c(i,1,0,0))
               ug(ip(i,1),ip(i,2),c2,c3) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)
     & +cr1*ui(i1+1,i2  ,c2,c3)+cr2*ui(i1+2,i2  ,c2,c3)+cr3*ui(i1+3,
     & i2  ,c2,c3)+cr4*ui(i1+4,i2  ,c2,c3)+cr5*ui(i1+5,i2  ,c2,c3)+
     & cr6*ui(i1+6,i2  ,c2,c3)+cr7*ui(i1+7,i2  ,c2,c3))+cs1*(cr0*ui(
     & i1  ,i2+1,c2,c3)+cr1*ui(i1+1,i2+1,c2,c3)+cr2*ui(i1+2,i2+1,c2,
     & c3)+cr3*ui(i1+3,i2+1,c2,c3)+cr4*ui(i1+4,i2+1,c2,c3)+cr5*ui(i1+
     & 5,i2+1,c2,c3)+cr6*ui(i1+6,i2+1,c2,c3)+cr7*ui(i1+7,i2+1,c2,c3))+
     & cs2*(cr0*ui(i1  ,i2+2,c2,c3)+cr1*ui(i1+1,i2+2,c2,c3)+cr2*ui(i1+
     & 2,i2+2,c2,c3)+cr3*ui(i1+3,i2+2,c2,c3)+cr4*ui(i1+4,i2+2,c2,c3)+
     & cr5*ui(i1+5,i2+2,c2,c3)+cr6*ui(i1+6,i2+2,c2,c3)+cr7*ui(i1+7,i2+
     & 2,c2,c3))+cs3*(cr0*ui(i1  ,i2+3,c2,c3)+cr1*ui(i1+1,i2+3,c2,c3)+
     & cr2*ui(i1+2,i2+3,c2,c3)+cr3*ui(i1+3,i2+3,c2,c3)+cr4*ui(i1+4,i2+
     & 3,c2,c3)+cr5*ui(i1+5,i2+3,c2,c3)+cr6*ui(i1+6,i2+3,c2,c3)+cr7*
     & ui(i1+7,i2+3,c2,c3))+cs4*(cr0*ui(i1  ,i2+4,c2,c3)+cr1*ui(i1+1,
     & i2+4,c2,c3)+cr2*ui(i1+2,i2+4,c2,c3)+cr3*ui(i1+3,i2+4,c2,c3)+
     & cr4*ui(i1+4,i2+4,c2,c3)+cr5*ui(i1+5,i2+4,c2,c3)+cr6*ui(i1+6,i2+
     & 4,c2,c3)+cr7*ui(i1+7,i2+4,c2,c3))+cs5*(cr0*ui(i1  ,i2+5,c2,c3)+
     & cr1*ui(i1+1,i2+5,c2,c3)+cr2*ui(i1+2,i2+5,c2,c3)+cr3*ui(i1+3,i2+
     & 5,c2,c3)+cr4*ui(i1+4,i2+5,c2,c3)+cr5*ui(i1+5,i2+5,c2,c3)+cr6*
     & ui(i1+6,i2+5,c2,c3)+cr7*ui(i1+7,i2+5,c2,c3))+cs6*(cr0*ui(i1  ,
     & i2+6,c2,c3)+cr1*ui(i1+1,i2+6,c2,c3)+cr2*ui(i1+2,i2+6,c2,c3)+
     & cr3*ui(i1+3,i2+6,c2,c3)+cr4*ui(i1+4,i2+6,c2,c3)+cr5*ui(i1+5,i2+
     & 6,c2,c3)+cr6*ui(i1+6,i2+6,c2,c3)+cr7*ui(i1+7,i2+6,c2,c3))+cs7*(
     & cr0*ui(i1  ,i2+7,c2,c3)+cr1*ui(i1+1,i2+7,c2,c3)+cr2*ui(i1+2,i2+
     & 7,c2,c3)+cr3*ui(i1+3,i2+7,c2,c3)+cr4*ui(i1+4,i2+7,c2,c3)+cr5*
     & ui(i1+5,i2+7,c2,c3)+cr6*ui(i1+6,i2+7,c2,c3)+cr7*ui(i1+7,i2+7,
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
! loops2d($interpSparseStorage33(ug(ip(i,1),ip(i,2),c2,c3)),,)
           if( c2a.eq.c2b .and. c3a.eq.c3b )then
             do c3=c3a,c3b
             do c2=c2a,c2b
             do i=nia,nib
! interpSparseStorage33(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q30(c(i,0,0,0))
                 cs0 = q30(c(i,1,0,0))
                 cr1 = q31(c(i,0,0,0))
                 cs1 = q31(c(i,1,0,0))
                 cr2 = q32(c(i,0,0,0))
                 cs2 = q32(c(i,1,0,0))
               ug(ip(i,1),ip(i,2),c2,c3) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)
     & +cr1*ui(i1+1,i2  ,c2,c3)+cr2*ui(i1+2,i2  ,c2,c3))+cs1*(cr0*ui(
     & i1  ,i2+1,c2,c3)+cr1*ui(i1+1,i2+1,c2,c3)+cr2*ui(i1+2,i2+1,c2,
     & c3))+cs2*(cr0*ui(i1  ,i2+2,c2,c3)+cr1*ui(i1+1,i2+2,c2,c3)+cr2*
     & ui(i1+2,i2+2,c2,c3))


             end do
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
             do c2=c2a,c2b
! interpSparseStorage33(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q30(c(i,0,0,0))
                 cs0 = q30(c(i,1,0,0))
                 cr1 = q31(c(i,0,0,0))
                 cs1 = q31(c(i,1,0,0))
                 cr2 = q32(c(i,0,0,0))
                 cs2 = q32(c(i,1,0,0))
               ug(ip(i,1),ip(i,2),c2,c3) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)
     & +cr1*ui(i1+1,i2  ,c2,c3)+cr2*ui(i1+2,i2  ,c2,c3))+cs1*(cr0*ui(
     & i1  ,i2+1,c2,c3)+cr1*ui(i1+1,i2+1,c2,c3)+cr2*ui(i1+2,i2+1,c2,
     & c3))+cs2*(cr0*ui(i1  ,i2+2,c2,c3)+cr1*ui(i1+1,i2+2,c2,c3)+cr2*
     & ui(i1+2,i2+2,c2,c3))


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
! loops2d($interpSparseStorage22(ug(ip(i,1),ip(i,2),c2,c3)),,)
           if( c2a.eq.c2b .and. c3a.eq.c3b )then
             do c3=c3a,c3b
             do c2=c2a,c2b
             do i=nia,nib
! interpSparseStorage22(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q20(c(i,0,0,0))
                 cs0 = q20(c(i,1,0,0))
                 cr1 = q21(c(i,0,0,0))
                 cs1 = q21(c(i,1,0,0))
               ug(ip(i,1),ip(i,2),c2,c3) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)
     & +cr1*ui(i1+1,i2  ,c2,c3))+cs1*(cr0*ui(i1  ,i2+1,c2,c3)+cr1*ui(
     & i1+1,i2+1,c2,c3))


             end do
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
             do c2=c2a,c2b
! interpSparseStorage22(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q20(c(i,0,0,0))
                 cs0 = q20(c(i,1,0,0))
                 cr1 = q21(c(i,0,0,0))
                 cs1 = q21(c(i,1,0,0))
               ug(ip(i,1),ip(i,2),c2,c3) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)
     & +cr1*ui(i1+1,i2  ,c2,c3))+cs1*(cr0*ui(i1  ,i2+1,c2,c3)+cr1*ui(
     & i1+1,i2+1,c2,c3))


             end do
             end do
             end do
           end if
         else if( width(1).eq.4 .and. width(2).eq.4 )then
! loops2d($interpSparseStorage44(ug(ip(i,1),ip(i,2),c2,c3)),,)
           if( c2a.eq.c2b .and. c3a.eq.c3b )then
             do c3=c3a,c3b
             do c2=c2a,c2b
             do i=nia,nib
! interpSparseStorage44(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q40(c(i,0,0,0))
                 cs0 = q40(c(i,1,0,0))
                 cr1 = q41(c(i,0,0,0))
                 cs1 = q41(c(i,1,0,0))
                 cr2 = q42(c(i,0,0,0))
                 cs2 = q42(c(i,1,0,0))
                 cr3 = q43(c(i,0,0,0))
                 cs3 = q43(c(i,1,0,0))
               ug(ip(i,1),ip(i,2),c2,c3) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)
     & +cr1*ui(i1+1,i2  ,c2,c3)+cr2*ui(i1+2,i2  ,c2,c3)+cr3*ui(i1+3,
     & i2  ,c2,c3))+cs1*(cr0*ui(i1  ,i2+1,c2,c3)+cr1*ui(i1+1,i2+1,c2,
     & c3)+cr2*ui(i1+2,i2+1,c2,c3)+cr3*ui(i1+3,i2+1,c2,c3))+cs2*(cr0*
     & ui(i1  ,i2+2,c2,c3)+cr1*ui(i1+1,i2+2,c2,c3)+cr2*ui(i1+2,i2+2,
     & c2,c3)+cr3*ui(i1+3,i2+2,c2,c3))+cs3*(cr0*ui(i1  ,i2+3,c2,c3)+
     & cr1*ui(i1+1,i2+3,c2,c3)+cr2*ui(i1+2,i2+3,c2,c3)+cr3*ui(i1+3,i2+
     & 3,c2,c3))


             end do
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
             do c2=c2a,c2b
! interpSparseStorage44(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q40(c(i,0,0,0))
                 cs0 = q40(c(i,1,0,0))
                 cr1 = q41(c(i,0,0,0))
                 cs1 = q41(c(i,1,0,0))
                 cr2 = q42(c(i,0,0,0))
                 cs2 = q42(c(i,1,0,0))
                 cr3 = q43(c(i,0,0,0))
                 cs3 = q43(c(i,1,0,0))
               ug(ip(i,1),ip(i,2),c2,c3) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)
     & +cr1*ui(i1+1,i2  ,c2,c3)+cr2*ui(i1+2,i2  ,c2,c3)+cr3*ui(i1+3,
     & i2  ,c2,c3))+cs1*(cr0*ui(i1  ,i2+1,c2,c3)+cr1*ui(i1+1,i2+1,c2,
     & c3)+cr2*ui(i1+2,i2+1,c2,c3)+cr3*ui(i1+3,i2+1,c2,c3))+cs2*(cr0*
     & ui(i1  ,i2+2,c2,c3)+cr1*ui(i1+1,i2+2,c2,c3)+cr2*ui(i1+2,i2+2,
     & c2,c3)+cr3*ui(i1+3,i2+2,c2,c3))+cs3*(cr0*ui(i1  ,i2+3,c2,c3)+
     & cr1*ui(i1+1,i2+3,c2,c3)+cr2*ui(i1+2,i2+3,c2,c3)+cr3*ui(i1+3,i2+
     & 3,c2,c3))


             end do
             end do
             end do
           end if
         else if( width(1).eq.5 .and. width(2).eq.5 )then
! loops2d($interpSparseStorage55(ug(ip(i,1),ip(i,2),c2,c3)),,)
           if( c2a.eq.c2b .and. c3a.eq.c3b )then
             do c3=c3a,c3b
             do c2=c2a,c2b
             do i=nia,nib
! interpSparseStorage55(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q50(c(i,0,0,0))
                 cs0 = q50(c(i,1,0,0))
                 cr1 = q51(c(i,0,0,0))
                 cs1 = q51(c(i,1,0,0))
                 cr2 = q52(c(i,0,0,0))
                 cs2 = q52(c(i,1,0,0))
                 cr3 = q53(c(i,0,0,0))
                 cs3 = q53(c(i,1,0,0))
                 cr4 = q54(c(i,0,0,0))
                 cs4 = q54(c(i,1,0,0))
               ug(ip(i,1),ip(i,2),c2,c3) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)
     & +cr1*ui(i1+1,i2  ,c2,c3)+cr2*ui(i1+2,i2  ,c2,c3)+cr3*ui(i1+3,
     & i2  ,c2,c3)+cr4*ui(i1+4,i2  ,c2,c3))+cs1*(cr0*ui(i1  ,i2+1,c2,
     & c3)+cr1*ui(i1+1,i2+1,c2,c3)+cr2*ui(i1+2,i2+1,c2,c3)+cr3*ui(i1+
     & 3,i2+1,c2,c3)+cr4*ui(i1+4,i2+1,c2,c3))+cs2*(cr0*ui(i1  ,i2+2,
     & c2,c3)+cr1*ui(i1+1,i2+2,c2,c3)+cr2*ui(i1+2,i2+2,c2,c3)+cr3*ui(
     & i1+3,i2+2,c2,c3)+cr4*ui(i1+4,i2+2,c2,c3))+cs3*(cr0*ui(i1  ,i2+
     & 3,c2,c3)+cr1*ui(i1+1,i2+3,c2,c3)+cr2*ui(i1+2,i2+3,c2,c3)+cr3*
     & ui(i1+3,i2+3,c2,c3)+cr4*ui(i1+4,i2+3,c2,c3))+cs4*(cr0*ui(i1  ,
     & i2+4,c2,c3)+cr1*ui(i1+1,i2+4,c2,c3)+cr2*ui(i1+2,i2+4,c2,c3)+
     & cr3*ui(i1+3,i2+4,c2,c3)+cr4*ui(i1+4,i2+4,c2,c3))


             end do
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
             do c2=c2a,c2b
! interpSparseStorage55(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q50(c(i,0,0,0))
                 cs0 = q50(c(i,1,0,0))
                 cr1 = q51(c(i,0,0,0))
                 cs1 = q51(c(i,1,0,0))
                 cr2 = q52(c(i,0,0,0))
                 cs2 = q52(c(i,1,0,0))
                 cr3 = q53(c(i,0,0,0))
                 cs3 = q53(c(i,1,0,0))
                 cr4 = q54(c(i,0,0,0))
                 cs4 = q54(c(i,1,0,0))
               ug(ip(i,1),ip(i,2),c2,c3) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)
     & +cr1*ui(i1+1,i2  ,c2,c3)+cr2*ui(i1+2,i2  ,c2,c3)+cr3*ui(i1+3,
     & i2  ,c2,c3)+cr4*ui(i1+4,i2  ,c2,c3))+cs1*(cr0*ui(i1  ,i2+1,c2,
     & c3)+cr1*ui(i1+1,i2+1,c2,c3)+cr2*ui(i1+2,i2+1,c2,c3)+cr3*ui(i1+
     & 3,i2+1,c2,c3)+cr4*ui(i1+4,i2+1,c2,c3))+cs2*(cr0*ui(i1  ,i2+2,
     & c2,c3)+cr1*ui(i1+1,i2+2,c2,c3)+cr2*ui(i1+2,i2+2,c2,c3)+cr3*ui(
     & i1+3,i2+2,c2,c3)+cr4*ui(i1+4,i2+2,c2,c3))+cs3*(cr0*ui(i1  ,i2+
     & 3,c2,c3)+cr1*ui(i1+1,i2+3,c2,c3)+cr2*ui(i1+2,i2+3,c2,c3)+cr3*
     & ui(i1+3,i2+3,c2,c3)+cr4*ui(i1+4,i2+3,c2,c3))+cs4*(cr0*ui(i1  ,
     & i2+4,c2,c3)+cr1*ui(i1+1,i2+4,c2,c3)+cr2*ui(i1+2,i2+4,c2,c3)+
     & cr3*ui(i1+3,i2+4,c2,c3)+cr4*ui(i1+4,i2+4,c2,c3))


             end do
             end do
             end do
           end if
         else if( width(1).eq.6 .and. width(2).eq.6 )then
! loops2d($interpSparseStorage66(ug(ip(i,1),ip(i,2),c2,c3)),,)
           if( c2a.eq.c2b .and. c3a.eq.c3b )then
             do c3=c3a,c3b
             do c2=c2a,c2b
             do i=nia,nib
! interpSparseStorage66(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q60(c(i,0,0,0))
                 cs0 = q60(c(i,1,0,0))
                 cr1 = q61(c(i,0,0,0))
                 cs1 = q61(c(i,1,0,0))
                 cr2 = q62(c(i,0,0,0))
                 cs2 = q62(c(i,1,0,0))
                 cr3 = q63(c(i,0,0,0))
                 cs3 = q63(c(i,1,0,0))
                 cr4 = q64(c(i,0,0,0))
                 cs4 = q64(c(i,1,0,0))
                 cr5 = q65(c(i,0,0,0))
                 cs5 = q65(c(i,1,0,0))
               ug(ip(i,1),ip(i,2),c2,c3) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)
     & +cr1*ui(i1+1,i2  ,c2,c3)+cr2*ui(i1+2,i2  ,c2,c3)+cr3*ui(i1+3,
     & i2  ,c2,c3)+cr4*ui(i1+4,i2  ,c2,c3)+cr5*ui(i1+5,i2  ,c2,c3))+
     & cs1*(cr0*ui(i1  ,i2+1,c2,c3)+cr1*ui(i1+1,i2+1,c2,c3)+cr2*ui(i1+
     & 2,i2+1,c2,c3)+cr3*ui(i1+3,i2+1,c2,c3)+cr4*ui(i1+4,i2+1,c2,c3)+
     & cr5*ui(i1+5,i2+1,c2,c3))+cs2*(cr0*ui(i1  ,i2+2,c2,c3)+cr1*ui(
     & i1+1,i2+2,c2,c3)+cr2*ui(i1+2,i2+2,c2,c3)+cr3*ui(i1+3,i2+2,c2,
     & c3)+cr4*ui(i1+4,i2+2,c2,c3)+cr5*ui(i1+5,i2+2,c2,c3))+cs3*(cr0*
     & ui(i1  ,i2+3,c2,c3)+cr1*ui(i1+1,i2+3,c2,c3)+cr2*ui(i1+2,i2+3,
     & c2,c3)+cr3*ui(i1+3,i2+3,c2,c3)+cr4*ui(i1+4,i2+3,c2,c3)+cr5*ui(
     & i1+5,i2+3,c2,c3))+cs4*(cr0*ui(i1  ,i2+4,c2,c3)+cr1*ui(i1+1,i2+
     & 4,c2,c3)+cr2*ui(i1+2,i2+4,c2,c3)+cr3*ui(i1+3,i2+4,c2,c3)+cr4*
     & ui(i1+4,i2+4,c2,c3)+cr5*ui(i1+5,i2+4,c2,c3))+cs5*(cr0*ui(i1  ,
     & i2+5,c2,c3)+cr1*ui(i1+1,i2+5,c2,c3)+cr2*ui(i1+2,i2+5,c2,c3)+
     & cr3*ui(i1+3,i2+5,c2,c3)+cr4*ui(i1+4,i2+5,c2,c3)+cr5*ui(i1+5,i2+
     & 5,c2,c3))


             end do
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
             do c2=c2a,c2b
! interpSparseStorage66(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q60(c(i,0,0,0))
                 cs0 = q60(c(i,1,0,0))
                 cr1 = q61(c(i,0,0,0))
                 cs1 = q61(c(i,1,0,0))
                 cr2 = q62(c(i,0,0,0))
                 cs2 = q62(c(i,1,0,0))
                 cr3 = q63(c(i,0,0,0))
                 cs3 = q63(c(i,1,0,0))
                 cr4 = q64(c(i,0,0,0))
                 cs4 = q64(c(i,1,0,0))
                 cr5 = q65(c(i,0,0,0))
                 cs5 = q65(c(i,1,0,0))
               ug(ip(i,1),ip(i,2),c2,c3) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)
     & +cr1*ui(i1+1,i2  ,c2,c3)+cr2*ui(i1+2,i2  ,c2,c3)+cr3*ui(i1+3,
     & i2  ,c2,c3)+cr4*ui(i1+4,i2  ,c2,c3)+cr5*ui(i1+5,i2  ,c2,c3))+
     & cs1*(cr0*ui(i1  ,i2+1,c2,c3)+cr1*ui(i1+1,i2+1,c2,c3)+cr2*ui(i1+
     & 2,i2+1,c2,c3)+cr3*ui(i1+3,i2+1,c2,c3)+cr4*ui(i1+4,i2+1,c2,c3)+
     & cr5*ui(i1+5,i2+1,c2,c3))+cs2*(cr0*ui(i1  ,i2+2,c2,c3)+cr1*ui(
     & i1+1,i2+2,c2,c3)+cr2*ui(i1+2,i2+2,c2,c3)+cr3*ui(i1+3,i2+2,c2,
     & c3)+cr4*ui(i1+4,i2+2,c2,c3)+cr5*ui(i1+5,i2+2,c2,c3))+cs3*(cr0*
     & ui(i1  ,i2+3,c2,c3)+cr1*ui(i1+1,i2+3,c2,c3)+cr2*ui(i1+2,i2+3,
     & c2,c3)+cr3*ui(i1+3,i2+3,c2,c3)+cr4*ui(i1+4,i2+3,c2,c3)+cr5*ui(
     & i1+5,i2+3,c2,c3))+cs4*(cr0*ui(i1  ,i2+4,c2,c3)+cr1*ui(i1+1,i2+
     & 4,c2,c3)+cr2*ui(i1+2,i2+4,c2,c3)+cr3*ui(i1+3,i2+4,c2,c3)+cr4*
     & ui(i1+4,i2+4,c2,c3)+cr5*ui(i1+5,i2+4,c2,c3))+cs5*(cr0*ui(i1  ,
     & i2+5,c2,c3)+cr1*ui(i1+1,i2+5,c2,c3)+cr2*ui(i1+2,i2+5,c2,c3)+
     & cr3*ui(i1+3,i2+5,c2,c3)+cr4*ui(i1+4,i2+5,c2,c3)+cr5*ui(i1+5,i2+
     & 5,c2,c3))


             end do
             end do
             end do
           end if
         else if( width(1).eq.7 .and. width(2).eq.7 )then
! loops2d($interpSparseStorage77(ug(ip(i,1),ip(i,2),c2,c3)),,)
           if( c2a.eq.c2b .and. c3a.eq.c3b )then
             do c3=c3a,c3b
             do c2=c2a,c2b
             do i=nia,nib
! interpSparseStorage77(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q70(c(i,0,0,0))
                 cs0 = q70(c(i,1,0,0))
                 cr1 = q71(c(i,0,0,0))
                 cs1 = q71(c(i,1,0,0))
                 cr2 = q72(c(i,0,0,0))
                 cs2 = q72(c(i,1,0,0))
                 cr3 = q73(c(i,0,0,0))
                 cs3 = q73(c(i,1,0,0))
                 cr4 = q74(c(i,0,0,0))
                 cs4 = q74(c(i,1,0,0))
                 cr5 = q75(c(i,0,0,0))
                 cs5 = q75(c(i,1,0,0))
                 cr6 = q76(c(i,0,0,0))
                 cs6 = q76(c(i,1,0,0))
               ug(ip(i,1),ip(i,2),c2,c3) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)
     & +cr1*ui(i1+1,i2  ,c2,c3)+cr2*ui(i1+2,i2  ,c2,c3)+cr3*ui(i1+3,
     & i2  ,c2,c3)+cr4*ui(i1+4,i2  ,c2,c3)+cr5*ui(i1+5,i2  ,c2,c3)+
     & cr6*ui(i1+6,i2  ,c2,c3))+cs1*(cr0*ui(i1  ,i2+1,c2,c3)+cr1*ui(
     & i1+1,i2+1,c2,c3)+cr2*ui(i1+2,i2+1,c2,c3)+cr3*ui(i1+3,i2+1,c2,
     & c3)+cr4*ui(i1+4,i2+1,c2,c3)+cr5*ui(i1+5,i2+1,c2,c3)+cr6*ui(i1+
     & 6,i2+1,c2,c3))+cs2*(cr0*ui(i1  ,i2+2,c2,c3)+cr1*ui(i1+1,i2+2,
     & c2,c3)+cr2*ui(i1+2,i2+2,c2,c3)+cr3*ui(i1+3,i2+2,c2,c3)+cr4*ui(
     & i1+4,i2+2,c2,c3)+cr5*ui(i1+5,i2+2,c2,c3)+cr6*ui(i1+6,i2+2,c2,
     & c3))+cs3*(cr0*ui(i1  ,i2+3,c2,c3)+cr1*ui(i1+1,i2+3,c2,c3)+cr2*
     & ui(i1+2,i2+3,c2,c3)+cr3*ui(i1+3,i2+3,c2,c3)+cr4*ui(i1+4,i2+3,
     & c2,c3)+cr5*ui(i1+5,i2+3,c2,c3)+cr6*ui(i1+6,i2+3,c2,c3))+cs4*(
     & cr0*ui(i1  ,i2+4,c2,c3)+cr1*ui(i1+1,i2+4,c2,c3)+cr2*ui(i1+2,i2+
     & 4,c2,c3)+cr3*ui(i1+3,i2+4,c2,c3)+cr4*ui(i1+4,i2+4,c2,c3)+cr5*
     & ui(i1+5,i2+4,c2,c3)+cr6*ui(i1+6,i2+4,c2,c3))+cs5*(cr0*ui(i1  ,
     & i2+5,c2,c3)+cr1*ui(i1+1,i2+5,c2,c3)+cr2*ui(i1+2,i2+5,c2,c3)+
     & cr3*ui(i1+3,i2+5,c2,c3)+cr4*ui(i1+4,i2+5,c2,c3)+cr5*ui(i1+5,i2+
     & 5,c2,c3)+cr6*ui(i1+6,i2+5,c2,c3))+cs6*(cr0*ui(i1  ,i2+6,c2,c3)+
     & cr1*ui(i1+1,i2+6,c2,c3)+cr2*ui(i1+2,i2+6,c2,c3)+cr3*ui(i1+3,i2+
     & 6,c2,c3)+cr4*ui(i1+4,i2+6,c2,c3)+cr5*ui(i1+5,i2+6,c2,c3)+cr6*
     & ui(i1+6,i2+6,c2,c3))


             end do
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
             do c2=c2a,c2b
! interpSparseStorage77(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q70(c(i,0,0,0))
                 cs0 = q70(c(i,1,0,0))
                 cr1 = q71(c(i,0,0,0))
                 cs1 = q71(c(i,1,0,0))
                 cr2 = q72(c(i,0,0,0))
                 cs2 = q72(c(i,1,0,0))
                 cr3 = q73(c(i,0,0,0))
                 cs3 = q73(c(i,1,0,0))
                 cr4 = q74(c(i,0,0,0))
                 cs4 = q74(c(i,1,0,0))
                 cr5 = q75(c(i,0,0,0))
                 cs5 = q75(c(i,1,0,0))
                 cr6 = q76(c(i,0,0,0))
                 cs6 = q76(c(i,1,0,0))
               ug(ip(i,1),ip(i,2),c2,c3) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)
     & +cr1*ui(i1+1,i2  ,c2,c3)+cr2*ui(i1+2,i2  ,c2,c3)+cr3*ui(i1+3,
     & i2  ,c2,c3)+cr4*ui(i1+4,i2  ,c2,c3)+cr5*ui(i1+5,i2  ,c2,c3)+
     & cr6*ui(i1+6,i2  ,c2,c3))+cs1*(cr0*ui(i1  ,i2+1,c2,c3)+cr1*ui(
     & i1+1,i2+1,c2,c3)+cr2*ui(i1+2,i2+1,c2,c3)+cr3*ui(i1+3,i2+1,c2,
     & c3)+cr4*ui(i1+4,i2+1,c2,c3)+cr5*ui(i1+5,i2+1,c2,c3)+cr6*ui(i1+
     & 6,i2+1,c2,c3))+cs2*(cr0*ui(i1  ,i2+2,c2,c3)+cr1*ui(i1+1,i2+2,
     & c2,c3)+cr2*ui(i1+2,i2+2,c2,c3)+cr3*ui(i1+3,i2+2,c2,c3)+cr4*ui(
     & i1+4,i2+2,c2,c3)+cr5*ui(i1+5,i2+2,c2,c3)+cr6*ui(i1+6,i2+2,c2,
     & c3))+cs3*(cr0*ui(i1  ,i2+3,c2,c3)+cr1*ui(i1+1,i2+3,c2,c3)+cr2*
     & ui(i1+2,i2+3,c2,c3)+cr3*ui(i1+3,i2+3,c2,c3)+cr4*ui(i1+4,i2+3,
     & c2,c3)+cr5*ui(i1+5,i2+3,c2,c3)+cr6*ui(i1+6,i2+3,c2,c3))+cs4*(
     & cr0*ui(i1  ,i2+4,c2,c3)+cr1*ui(i1+1,i2+4,c2,c3)+cr2*ui(i1+2,i2+
     & 4,c2,c3)+cr3*ui(i1+3,i2+4,c2,c3)+cr4*ui(i1+4,i2+4,c2,c3)+cr5*
     & ui(i1+5,i2+4,c2,c3)+cr6*ui(i1+6,i2+4,c2,c3))+cs5*(cr0*ui(i1  ,
     & i2+5,c2,c3)+cr1*ui(i1+1,i2+5,c2,c3)+cr2*ui(i1+2,i2+5,c2,c3)+
     & cr3*ui(i1+3,i2+5,c2,c3)+cr4*ui(i1+4,i2+5,c2,c3)+cr5*ui(i1+5,i2+
     & 5,c2,c3)+cr6*ui(i1+6,i2+5,c2,c3))+cs6*(cr0*ui(i1  ,i2+6,c2,c3)+
     & cr1*ui(i1+1,i2+6,c2,c3)+cr2*ui(i1+2,i2+6,c2,c3)+cr3*ui(i1+3,i2+
     & 6,c2,c3)+cr4*ui(i1+4,i2+6,c2,c3)+cr5*ui(i1+5,i2+6,c2,c3)+cr6*
     & ui(i1+6,i2+6,c2,c3))


             end do
             end do
             end do
           end if
         else if( width(1).eq.8 .and. width(2).eq.8 )then
! loops2d($interpSparseStorage88(ug(ip(i,1),ip(i,2),c2,c3)),,)
           if( c2a.eq.c2b .and. c3a.eq.c3b )then
             do c3=c3a,c3b
             do c2=c2a,c2b
             do i=nia,nib
! interpSparseStorage88(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q80(c(i,0,0,0))
                 cs0 = q80(c(i,1,0,0))
                 cr1 = q81(c(i,0,0,0))
                 cs1 = q81(c(i,1,0,0))
                 cr2 = q82(c(i,0,0,0))
                 cs2 = q82(c(i,1,0,0))
                 cr3 = q83(c(i,0,0,0))
                 cs3 = q83(c(i,1,0,0))
                 cr4 = q84(c(i,0,0,0))
                 cs4 = q84(c(i,1,0,0))
                 cr5 = q85(c(i,0,0,0))
                 cs5 = q85(c(i,1,0,0))
                 cr6 = q86(c(i,0,0,0))
                 cs6 = q86(c(i,1,0,0))
                 cr7 = q87(c(i,0,0,0))
                 cs7 = q87(c(i,1,0,0))
               ug(ip(i,1),ip(i,2),c2,c3) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)
     & +cr1*ui(i1+1,i2  ,c2,c3)+cr2*ui(i1+2,i2  ,c2,c3)+cr3*ui(i1+3,
     & i2  ,c2,c3)+cr4*ui(i1+4,i2  ,c2,c3)+cr5*ui(i1+5,i2  ,c2,c3)+
     & cr6*ui(i1+6,i2  ,c2,c3)+cr7*ui(i1+7,i2  ,c2,c3))+cs1*(cr0*ui(
     & i1  ,i2+1,c2,c3)+cr1*ui(i1+1,i2+1,c2,c3)+cr2*ui(i1+2,i2+1,c2,
     & c3)+cr3*ui(i1+3,i2+1,c2,c3)+cr4*ui(i1+4,i2+1,c2,c3)+cr5*ui(i1+
     & 5,i2+1,c2,c3)+cr6*ui(i1+6,i2+1,c2,c3)+cr7*ui(i1+7,i2+1,c2,c3))+
     & cs2*(cr0*ui(i1  ,i2+2,c2,c3)+cr1*ui(i1+1,i2+2,c2,c3)+cr2*ui(i1+
     & 2,i2+2,c2,c3)+cr3*ui(i1+3,i2+2,c2,c3)+cr4*ui(i1+4,i2+2,c2,c3)+
     & cr5*ui(i1+5,i2+2,c2,c3)+cr6*ui(i1+6,i2+2,c2,c3)+cr7*ui(i1+7,i2+
     & 2,c2,c3))+cs3*(cr0*ui(i1  ,i2+3,c2,c3)+cr1*ui(i1+1,i2+3,c2,c3)+
     & cr2*ui(i1+2,i2+3,c2,c3)+cr3*ui(i1+3,i2+3,c2,c3)+cr4*ui(i1+4,i2+
     & 3,c2,c3)+cr5*ui(i1+5,i2+3,c2,c3)+cr6*ui(i1+6,i2+3,c2,c3)+cr7*
     & ui(i1+7,i2+3,c2,c3))+cs4*(cr0*ui(i1  ,i2+4,c2,c3)+cr1*ui(i1+1,
     & i2+4,c2,c3)+cr2*ui(i1+2,i2+4,c2,c3)+cr3*ui(i1+3,i2+4,c2,c3)+
     & cr4*ui(i1+4,i2+4,c2,c3)+cr5*ui(i1+5,i2+4,c2,c3)+cr6*ui(i1+6,i2+
     & 4,c2,c3)+cr7*ui(i1+7,i2+4,c2,c3))+cs5*(cr0*ui(i1  ,i2+5,c2,c3)+
     & cr1*ui(i1+1,i2+5,c2,c3)+cr2*ui(i1+2,i2+5,c2,c3)+cr3*ui(i1+3,i2+
     & 5,c2,c3)+cr4*ui(i1+4,i2+5,c2,c3)+cr5*ui(i1+5,i2+5,c2,c3)+cr6*
     & ui(i1+6,i2+5,c2,c3)+cr7*ui(i1+7,i2+5,c2,c3))+cs6*(cr0*ui(i1  ,
     & i2+6,c2,c3)+cr1*ui(i1+1,i2+6,c2,c3)+cr2*ui(i1+2,i2+6,c2,c3)+
     & cr3*ui(i1+3,i2+6,c2,c3)+cr4*ui(i1+4,i2+6,c2,c3)+cr5*ui(i1+5,i2+
     & 6,c2,c3)+cr6*ui(i1+6,i2+6,c2,c3)+cr7*ui(i1+7,i2+6,c2,c3))+cs7*(
     & cr0*ui(i1  ,i2+7,c2,c3)+cr1*ui(i1+1,i2+7,c2,c3)+cr2*ui(i1+2,i2+
     & 7,c2,c3)+cr3*ui(i1+3,i2+7,c2,c3)+cr4*ui(i1+4,i2+7,c2,c3)+cr5*
     & ui(i1+5,i2+7,c2,c3)+cr6*ui(i1+6,i2+7,c2,c3)+cr7*ui(i1+7,i2+7,
     & c2,c3))


             end do
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
             do c2=c2a,c2b
! interpSparseStorage88(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q80(c(i,0,0,0))
                 cs0 = q80(c(i,1,0,0))
                 cr1 = q81(c(i,0,0,0))
                 cs1 = q81(c(i,1,0,0))
                 cr2 = q82(c(i,0,0,0))
                 cs2 = q82(c(i,1,0,0))
                 cr3 = q83(c(i,0,0,0))
                 cs3 = q83(c(i,1,0,0))
                 cr4 = q84(c(i,0,0,0))
                 cs4 = q84(c(i,1,0,0))
                 cr5 = q85(c(i,0,0,0))
                 cs5 = q85(c(i,1,0,0))
                 cr6 = q86(c(i,0,0,0))
                 cs6 = q86(c(i,1,0,0))
                 cr7 = q87(c(i,0,0,0))
                 cs7 = q87(c(i,1,0,0))
               ug(ip(i,1),ip(i,2),c2,c3) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)
     & +cr1*ui(i1+1,i2  ,c2,c3)+cr2*ui(i1+2,i2  ,c2,c3)+cr3*ui(i1+3,
     & i2  ,c2,c3)+cr4*ui(i1+4,i2  ,c2,c3)+cr5*ui(i1+5,i2  ,c2,c3)+
     & cr6*ui(i1+6,i2  ,c2,c3)+cr7*ui(i1+7,i2  ,c2,c3))+cs1*(cr0*ui(
     & i1  ,i2+1,c2,c3)+cr1*ui(i1+1,i2+1,c2,c3)+cr2*ui(i1+2,i2+1,c2,
     & c3)+cr3*ui(i1+3,i2+1,c2,c3)+cr4*ui(i1+4,i2+1,c2,c3)+cr5*ui(i1+
     & 5,i2+1,c2,c3)+cr6*ui(i1+6,i2+1,c2,c3)+cr7*ui(i1+7,i2+1,c2,c3))+
     & cs2*(cr0*ui(i1  ,i2+2,c2,c3)+cr1*ui(i1+1,i2+2,c2,c3)+cr2*ui(i1+
     & 2,i2+2,c2,c3)+cr3*ui(i1+3,i2+2,c2,c3)+cr4*ui(i1+4,i2+2,c2,c3)+
     & cr5*ui(i1+5,i2+2,c2,c3)+cr6*ui(i1+6,i2+2,c2,c3)+cr7*ui(i1+7,i2+
     & 2,c2,c3))+cs3*(cr0*ui(i1  ,i2+3,c2,c3)+cr1*ui(i1+1,i2+3,c2,c3)+
     & cr2*ui(i1+2,i2+3,c2,c3)+cr3*ui(i1+3,i2+3,c2,c3)+cr4*ui(i1+4,i2+
     & 3,c2,c3)+cr5*ui(i1+5,i2+3,c2,c3)+cr6*ui(i1+6,i2+3,c2,c3)+cr7*
     & ui(i1+7,i2+3,c2,c3))+cs4*(cr0*ui(i1  ,i2+4,c2,c3)+cr1*ui(i1+1,
     & i2+4,c2,c3)+cr2*ui(i1+2,i2+4,c2,c3)+cr3*ui(i1+3,i2+4,c2,c3)+
     & cr4*ui(i1+4,i2+4,c2,c3)+cr5*ui(i1+5,i2+4,c2,c3)+cr6*ui(i1+6,i2+
     & 4,c2,c3)+cr7*ui(i1+7,i2+4,c2,c3))+cs5*(cr0*ui(i1  ,i2+5,c2,c3)+
     & cr1*ui(i1+1,i2+5,c2,c3)+cr2*ui(i1+2,i2+5,c2,c3)+cr3*ui(i1+3,i2+
     & 5,c2,c3)+cr4*ui(i1+4,i2+5,c2,c3)+cr5*ui(i1+5,i2+5,c2,c3)+cr6*
     & ui(i1+6,i2+5,c2,c3)+cr7*ui(i1+7,i2+5,c2,c3))+cs6*(cr0*ui(i1  ,
     & i2+6,c2,c3)+cr1*ui(i1+1,i2+6,c2,c3)+cr2*ui(i1+2,i2+6,c2,c3)+
     & cr3*ui(i1+3,i2+6,c2,c3)+cr4*ui(i1+4,i2+6,c2,c3)+cr5*ui(i1+5,i2+
     & 6,c2,c3)+cr6*ui(i1+6,i2+6,c2,c3)+cr7*ui(i1+7,i2+6,c2,c3))+cs7*(
     & cr0*ui(i1  ,i2+7,c2,c3)+cr1*ui(i1+1,i2+7,c2,c3)+cr2*ui(i1+2,i2+
     & 7,c2,c3)+cr3*ui(i1+3,i2+7,c2,c3)+cr4*ui(i1+4,i2+7,c2,c3)+cr5*
     & ui(i1+5,i2+7,c2,c3)+cr6*ui(i1+6,i2+7,c2,c3)+cr7*ui(i1+7,i2+7,
     & c2,c3))


             end do
             end do
             end do
           end if
         else if( width(1).eq.9 .and. width(2).eq.9 )then
! loops2d($interpSparseStorage99(ug(ip(i,1),ip(i,2),c2,c3)),,)
           if( c2a.eq.c2b .and. c3a.eq.c3b )then
             do c3=c3a,c3b
             do c2=c2a,c2b
             do i=nia,nib
! interpSparseStorage99(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q90(c(i,0,0,0))
                 cs0 = q90(c(i,1,0,0))
                 cr1 = q91(c(i,0,0,0))
                 cs1 = q91(c(i,1,0,0))
                 cr2 = q92(c(i,0,0,0))
                 cs2 = q92(c(i,1,0,0))
                 cr3 = q93(c(i,0,0,0))
                 cs3 = q93(c(i,1,0,0))
                 cr4 = q94(c(i,0,0,0))
                 cs4 = q94(c(i,1,0,0))
                 cr5 = q95(c(i,0,0,0))
                 cs5 = q95(c(i,1,0,0))
                 cr6 = q96(c(i,0,0,0))
                 cs6 = q96(c(i,1,0,0))
                 cr7 = q97(c(i,0,0,0))
                 cs7 = q97(c(i,1,0,0))
                 cr8 = q98(c(i,0,0,0))
                 cs8 = q98(c(i,1,0,0))
               ug(ip(i,1),ip(i,2),c2,c3) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)
     & +cr1*ui(i1+1,i2  ,c2,c3)+cr2*ui(i1+2,i2  ,c2,c3)+cr3*ui(i1+3,
     & i2  ,c2,c3)+cr4*ui(i1+4,i2  ,c2,c3)+cr5*ui(i1+5,i2  ,c2,c3)+
     & cr6*ui(i1+6,i2  ,c2,c3)+cr7*ui(i1+7,i2  ,c2,c3)+cr8*ui(i1+8,i2 
     &  ,c2,c3))+cs1*(cr0*ui(i1  ,i2+1,c2,c3)+cr1*ui(i1+1,i2+1,c2,c3)+
     & cr2*ui(i1+2,i2+1,c2,c3)+cr3*ui(i1+3,i2+1,c2,c3)+cr4*ui(i1+4,i2+
     & 1,c2,c3)+cr5*ui(i1+5,i2+1,c2,c3)+cr6*ui(i1+6,i2+1,c2,c3)+cr7*
     & ui(i1+7,i2+1,c2,c3)+cr8*ui(i1+8,i2+1,c2,c3))+cs2*(cr0*ui(i1  ,
     & i2+2,c2,c3)+cr1*ui(i1+1,i2+2,c2,c3)+cr2*ui(i1+2,i2+2,c2,c3)+
     & cr3*ui(i1+3,i2+2,c2,c3)+cr4*ui(i1+4,i2+2,c2,c3)+cr5*ui(i1+5,i2+
     & 2,c2,c3)+cr6*ui(i1+6,i2+2,c2,c3)+cr7*ui(i1+7,i2+2,c2,c3)+cr8*
     & ui(i1+8,i2+2,c2,c3))+cs3*(cr0*ui(i1  ,i2+3,c2,c3)+cr1*ui(i1+1,
     & i2+3,c2,c3)+cr2*ui(i1+2,i2+3,c2,c3)+cr3*ui(i1+3,i2+3,c2,c3)+
     & cr4*ui(i1+4,i2+3,c2,c3)+cr5*ui(i1+5,i2+3,c2,c3)+cr6*ui(i1+6,i2+
     & 3,c2,c3)+cr7*ui(i1+7,i2+3,c2,c3)+cr8*ui(i1+8,i2+3,c2,c3))+cs4*(
     & cr0*ui(i1  ,i2+4,c2,c3)+cr1*ui(i1+1,i2+4,c2,c3)+cr2*ui(i1+2,i2+
     & 4,c2,c3)+cr3*ui(i1+3,i2+4,c2,c3)+cr4*ui(i1+4,i2+4,c2,c3)+cr5*
     & ui(i1+5,i2+4,c2,c3)+cr6*ui(i1+6,i2+4,c2,c3)+cr7*ui(i1+7,i2+4,
     & c2,c3)+cr8*ui(i1+8,i2+4,c2,c3))+cs5*(cr0*ui(i1  ,i2+5,c2,c3)+
     & cr1*ui(i1+1,i2+5,c2,c3)+cr2*ui(i1+2,i2+5,c2,c3)+cr3*ui(i1+3,i2+
     & 5,c2,c3)+cr4*ui(i1+4,i2+5,c2,c3)+cr5*ui(i1+5,i2+5,c2,c3)+cr6*
     & ui(i1+6,i2+5,c2,c3)+cr7*ui(i1+7,i2+5,c2,c3)+cr8*ui(i1+8,i2+5,
     & c2,c3))+cs6*(cr0*ui(i1  ,i2+6,c2,c3)+cr1*ui(i1+1,i2+6,c2,c3)+
     & cr2*ui(i1+2,i2+6,c2,c3)+cr3*ui(i1+3,i2+6,c2,c3)+cr4*ui(i1+4,i2+
     & 6,c2,c3)+cr5*ui(i1+5,i2+6,c2,c3)+cr6*ui(i1+6,i2+6,c2,c3)+cr7*
     & ui(i1+7,i2+6,c2,c3)+cr8*ui(i1+8,i2+6,c2,c3))+cs7*(cr0*ui(i1  ,
     & i2+7,c2,c3)+cr1*ui(i1+1,i2+7,c2,c3)+cr2*ui(i1+2,i2+7,c2,c3)+
     & cr3*ui(i1+3,i2+7,c2,c3)+cr4*ui(i1+4,i2+7,c2,c3)+cr5*ui(i1+5,i2+
     & 7,c2,c3)+cr6*ui(i1+6,i2+7,c2,c3)+cr7*ui(i1+7,i2+7,c2,c3)+cr8*
     & ui(i1+8,i2+7,c2,c3))+cs8*(cr0*ui(i1  ,i2+8,c2,c3)+cr1*ui(i1+1,
     & i2+8,c2,c3)+cr2*ui(i1+2,i2+8,c2,c3)+cr3*ui(i1+3,i2+8,c2,c3)+
     & cr4*ui(i1+4,i2+8,c2,c3)+cr5*ui(i1+5,i2+8,c2,c3)+cr6*ui(i1+6,i2+
     & 8,c2,c3)+cr7*ui(i1+7,i2+8,c2,c3)+cr8*ui(i1+8,i2+8,c2,c3))


             end do
             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
             do c2=c2a,c2b
! interpSparseStorage99(ug(ip(i,1),ip(i,2),c2,c3))
               i1=il(i,1)
               i2=il(i,2)
                 cr0 = q90(c(i,0,0,0))
                 cs0 = q90(c(i,1,0,0))
                 cr1 = q91(c(i,0,0,0))
                 cs1 = q91(c(i,1,0,0))
                 cr2 = q92(c(i,0,0,0))
                 cs2 = q92(c(i,1,0,0))
                 cr3 = q93(c(i,0,0,0))
                 cs3 = q93(c(i,1,0,0))
                 cr4 = q94(c(i,0,0,0))
                 cs4 = q94(c(i,1,0,0))
                 cr5 = q95(c(i,0,0,0))
                 cs5 = q95(c(i,1,0,0))
                 cr6 = q96(c(i,0,0,0))
                 cs6 = q96(c(i,1,0,0))
                 cr7 = q97(c(i,0,0,0))
                 cs7 = q97(c(i,1,0,0))
                 cr8 = q98(c(i,0,0,0))
                 cs8 = q98(c(i,1,0,0))
               ug(ip(i,1),ip(i,2),c2,c3) = cs0*(cr0*ui(i1  ,i2  ,c2,c3)
     & +cr1*ui(i1+1,i2  ,c2,c3)+cr2*ui(i1+2,i2  ,c2,c3)+cr3*ui(i1+3,
     & i2  ,c2,c3)+cr4*ui(i1+4,i2  ,c2,c3)+cr5*ui(i1+5,i2  ,c2,c3)+
     & cr6*ui(i1+6,i2  ,c2,c3)+cr7*ui(i1+7,i2  ,c2,c3)+cr8*ui(i1+8,i2 
     &  ,c2,c3))+cs1*(cr0*ui(i1  ,i2+1,c2,c3)+cr1*ui(i1+1,i2+1,c2,c3)+
     & cr2*ui(i1+2,i2+1,c2,c3)+cr3*ui(i1+3,i2+1,c2,c3)+cr4*ui(i1+4,i2+
     & 1,c2,c3)+cr5*ui(i1+5,i2+1,c2,c3)+cr6*ui(i1+6,i2+1,c2,c3)+cr7*
     & ui(i1+7,i2+1,c2,c3)+cr8*ui(i1+8,i2+1,c2,c3))+cs2*(cr0*ui(i1  ,
     & i2+2,c2,c3)+cr1*ui(i1+1,i2+2,c2,c3)+cr2*ui(i1+2,i2+2,c2,c3)+
     & cr3*ui(i1+3,i2+2,c2,c3)+cr4*ui(i1+4,i2+2,c2,c3)+cr5*ui(i1+5,i2+
     & 2,c2,c3)+cr6*ui(i1+6,i2+2,c2,c3)+cr7*ui(i1+7,i2+2,c2,c3)+cr8*
     & ui(i1+8,i2+2,c2,c3))+cs3*(cr0*ui(i1  ,i2+3,c2,c3)+cr1*ui(i1+1,
     & i2+3,c2,c3)+cr2*ui(i1+2,i2+3,c2,c3)+cr3*ui(i1+3,i2+3,c2,c3)+
     & cr4*ui(i1+4,i2+3,c2,c3)+cr5*ui(i1+5,i2+3,c2,c3)+cr6*ui(i1+6,i2+
     & 3,c2,c3)+cr7*ui(i1+7,i2+3,c2,c3)+cr8*ui(i1+8,i2+3,c2,c3))+cs4*(
     & cr0*ui(i1  ,i2+4,c2,c3)+cr1*ui(i1+1,i2+4,c2,c3)+cr2*ui(i1+2,i2+
     & 4,c2,c3)+cr3*ui(i1+3,i2+4,c2,c3)+cr4*ui(i1+4,i2+4,c2,c3)+cr5*
     & ui(i1+5,i2+4,c2,c3)+cr6*ui(i1+6,i2+4,c2,c3)+cr7*ui(i1+7,i2+4,
     & c2,c3)+cr8*ui(i1+8,i2+4,c2,c3))+cs5*(cr0*ui(i1  ,i2+5,c2,c3)+
     & cr1*ui(i1+1,i2+5,c2,c3)+cr2*ui(i1+2,i2+5,c2,c3)+cr3*ui(i1+3,i2+
     & 5,c2,c3)+cr4*ui(i1+4,i2+5,c2,c3)+cr5*ui(i1+5,i2+5,c2,c3)+cr6*
     & ui(i1+6,i2+5,c2,c3)+cr7*ui(i1+7,i2+5,c2,c3)+cr8*ui(i1+8,i2+5,
     & c2,c3))+cs6*(cr0*ui(i1  ,i2+6,c2,c3)+cr1*ui(i1+1,i2+6,c2,c3)+
     & cr2*ui(i1+2,i2+6,c2,c3)+cr3*ui(i1+3,i2+6,c2,c3)+cr4*ui(i1+4,i2+
     & 6,c2,c3)+cr5*ui(i1+5,i2+6,c2,c3)+cr6*ui(i1+6,i2+6,c2,c3)+cr7*
     & ui(i1+7,i2+6,c2,c3)+cr8*ui(i1+8,i2+6,c2,c3))+cs7*(cr0*ui(i1  ,
     & i2+7,c2,c3)+cr1*ui(i1+1,i2+7,c2,c3)+cr2*ui(i1+2,i2+7,c2,c3)+
     & cr3*ui(i1+3,i2+7,c2,c3)+cr4*ui(i1+4,i2+7,c2,c3)+cr5*ui(i1+5,i2+
     & 7,c2,c3)+cr6*ui(i1+6,i2+7,c2,c3)+cr7*ui(i1+7,i2+7,c2,c3)+cr8*
     & ui(i1+8,i2+7,c2,c3))+cs8*(cr0*ui(i1  ,i2+8,c2,c3)+cr1*ui(i1+1,
     & i2+8,c2,c3)+cr2*ui(i1+2,i2+8,c2,c3)+cr3*ui(i1+3,i2+8,c2,c3)+
     & cr4*ui(i1+4,i2+8,c2,c3)+cr5*ui(i1+5,i2+8,c2,c3)+cr6*ui(i1+6,i2+
     & 8,c2,c3)+cr7*ui(i1+7,i2+8,c2,c3)+cr8*ui(i1+8,i2+8,c2,c3))


             end do
             end do
             end do
           end if
         else
c           general case in 2D ********************** fix this ***********************
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
! interpSparseStorage333(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q30(c(i,0,0,0))
                 cs0 = q30(c(i,1,0,0))
                 ct0 = q30(c(i,2,0,0))
                 cr1 = q31(c(i,0,0,0))
                 cs1 = q31(c(i,1,0,0))
                 ct1 = q31(c(i,2,0,0))
                 cr2 = q32(c(i,0,0,0))
                 cs2 = q32(c(i,1,0,0))
                 ct2 = q32(c(i,2,0,0))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ct0*(cs0*(cr0*ui(i1,i2,
     & i3+0,c3)+cr1*ui(i1+1,i2,i3+0,c3)+cr2*ui(i1+2,i2,i3+0,c3))+cs1*(
     & cr0*ui(i1,i2+1,i3+0,c3)+cr1*ui(i1+1,i2+1,i3+0,c3)+cr2*ui(i1+2,
     & i2+1,i3+0,c3))+cs2*(cr0*ui(i1,i2+2,i3+0,c3)+cr1*ui(i1+1,i2+2,
     & i3+0,c3)+cr2*ui(i1+2,i2+2,i3+0,c3)))+ct1*(cs0*(cr0*ui(i1,i2,i3+
     & 1,c3)+cr1*ui(i1+1,i2,i3+1,c3)+cr2*ui(i1+2,i2,i3+1,c3))+cs1*(
     & cr0*ui(i1,i2+1,i3+1,c3)+cr1*ui(i1+1,i2+1,i3+1,c3)+cr2*ui(i1+2,
     & i2+1,i3+1,c3))+cs2*(cr0*ui(i1,i2+2,i3+1,c3)+cr1*ui(i1+1,i2+2,
     & i3+1,c3)+cr2*ui(i1+2,i2+2,i3+1,c3)))+ct2*(cs0*(cr0*ui(i1,i2,i3+
     & 2,c3)+cr1*ui(i1+1,i2,i3+2,c3)+cr2*ui(i1+2,i2,i3+2,c3))+cs1*(
     & cr0*ui(i1,i2+1,i3+2,c3)+cr1*ui(i1+1,i2+1,i3+2,c3)+cr2*ui(i1+2,
     & i2+1,i3+2,c3))+cs2*(cr0*ui(i1,i2+2,i3+2,c3)+cr1*ui(i1+1,i2+2,
     & i3+2,c3)+cr2*ui(i1+2,i2+2,i3+2,c3)))
             else if( varWidth(i).eq.2 )then
! interpSparseStorage222(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q20(c(i,0,0,0))
                 cs0 = q20(c(i,1,0,0))
                 ct0 = q20(c(i,2,0,0))
                 cr1 = q21(c(i,0,0,0))
                 cs1 = q21(c(i,1,0,0))
                 ct1 = q21(c(i,2,0,0))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ct0*(cs0*(cr0*ui(i1,i2,
     & i3+0,c3)+cr1*ui(i1+1,i2,i3+0,c3))+cs1*(cr0*ui(i1,i2+1,i3+0,c3)+
     & cr1*ui(i1+1,i2+1,i3+0,c3)))+ct1*(cs0*(cr0*ui(i1,i2,i3+1,c3)+
     & cr1*ui(i1+1,i2,i3+1,c3))+cs1*(cr0*ui(i1,i2+1,i3+1,c3)+cr1*ui(
     & i1+1,i2+1,i3+1,c3)))
             else if( varWidth(i).eq.1 )then
! interp111(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ui(i1,i2,i3,c3)
             else if( varWidth(i).eq.5 )then
! interpSparseStorage555(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q50(c(i,0,0,0))
                 cs0 = q50(c(i,1,0,0))
                 ct0 = q50(c(i,2,0,0))
                 cr1 = q51(c(i,0,0,0))
                 cs1 = q51(c(i,1,0,0))
                 ct1 = q51(c(i,2,0,0))
                 cr2 = q52(c(i,0,0,0))
                 cs2 = q52(c(i,1,0,0))
                 ct2 = q52(c(i,2,0,0))
                 cr3 = q53(c(i,0,0,0))
                 cs3 = q53(c(i,1,0,0))
                 ct3 = q53(c(i,2,0,0))
                 cr4 = q54(c(i,0,0,0))
                 cs4 = q54(c(i,1,0,0))
                 ct4 = q54(c(i,2,0,0))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ct0*(cs0*(cr0*ui(i1,i2,
     & i3+0,c3)+cr1*ui(i1+1,i2,i3+0,c3)+cr2*ui(i1+2,i2,i3+0,c3)+cr3*
     & ui(i1+3,i2,i3+0,c3)+cr4*ui(i1+4,i2,i3+0,c3))+cs1*(cr0*ui(i1,i2+
     & 1,i3+0,c3)+cr1*ui(i1+1,i2+1,i3+0,c3)+cr2*ui(i1+2,i2+1,i3+0,c3)+
     & cr3*ui(i1+3,i2+1,i3+0,c3)+cr4*ui(i1+4,i2+1,i3+0,c3))+cs2*(cr0*
     & ui(i1,i2+2,i3+0,c3)+cr1*ui(i1+1,i2+2,i3+0,c3)+cr2*ui(i1+2,i2+2,
     & i3+0,c3)+cr3*ui(i1+3,i2+2,i3+0,c3)+cr4*ui(i1+4,i2+2,i3+0,c3))+
     & cs3*(cr0*ui(i1,i2+3,i3+0,c3)+cr1*ui(i1+1,i2+3,i3+0,c3)+cr2*ui(
     & i1+2,i2+3,i3+0,c3)+cr3*ui(i1+3,i2+3,i3+0,c3)+cr4*ui(i1+4,i2+3,
     & i3+0,c3))+cs4*(cr0*ui(i1,i2+4,i3+0,c3)+cr1*ui(i1+1,i2+4,i3+0,
     & c3)+cr2*ui(i1+2,i2+4,i3+0,c3)+cr3*ui(i1+3,i2+4,i3+0,c3)+cr4*ui(
     & i1+4,i2+4,i3+0,c3)))+ct1*(cs0*(cr0*ui(i1,i2,i3+1,c3)+cr1*ui(i1+
     & 1,i2,i3+1,c3)+cr2*ui(i1+2,i2,i3+1,c3)+cr3*ui(i1+3,i2,i3+1,c3)+
     & cr4*ui(i1+4,i2,i3+1,c3))+cs1*(cr0*ui(i1,i2+1,i3+1,c3)+cr1*ui(
     & i1+1,i2+1,i3+1,c3)+cr2*ui(i1+2,i2+1,i3+1,c3)+cr3*ui(i1+3,i2+1,
     & i3+1,c3)+cr4*ui(i1+4,i2+1,i3+1,c3))+cs2*(cr0*ui(i1,i2+2,i3+1,
     & c3)+cr1*ui(i1+1,i2+2,i3+1,c3)+cr2*ui(i1+2,i2+2,i3+1,c3)+cr3*ui(
     & i1+3,i2+2,i3+1,c3)+cr4*ui(i1+4,i2+2,i3+1,c3))+cs3*(cr0*ui(i1,
     & i2+3,i3+1,c3)+cr1*ui(i1+1,i2+3,i3+1,c3)+cr2*ui(i1+2,i2+3,i3+1,
     & c3)+cr3*ui(i1+3,i2+3,i3+1,c3)+cr4*ui(i1+4,i2+3,i3+1,c3))+cs4*(
     & cr0*ui(i1,i2+4,i3+1,c3)+cr1*ui(i1+1,i2+4,i3+1,c3)+cr2*ui(i1+2,
     & i2+4,i3+1,c3)+cr3*ui(i1+3,i2+4,i3+1,c3)+cr4*ui(i1+4,i2+4,i3+1,
     & c3)))+ct2*(cs0*(cr0*ui(i1,i2,i3+2,c3)+cr1*ui(i1+1,i2,i3+2,c3)+
     & cr2*ui(i1+2,i2,i3+2,c3)+cr3*ui(i1+3,i2,i3+2,c3)+cr4*ui(i1+4,i2,
     & i3+2,c3))+cs1*(cr0*ui(i1,i2+1,i3+2,c3)+cr1*ui(i1+1,i2+1,i3+2,
     & c3)+cr2*ui(i1+2,i2+1,i3+2,c3)+cr3*ui(i1+3,i2+1,i3+2,c3)+cr4*ui(
     & i1+4,i2+1,i3+2,c3))+cs2*(cr0*ui(i1,i2+2,i3+2,c3)+cr1*ui(i1+1,
     & i2+2,i3+2,c3)+cr2*ui(i1+2,i2+2,i3+2,c3)+cr3*ui(i1+3,i2+2,i3+2,
     & c3)+cr4*ui(i1+4,i2+2,i3+2,c3))+cs3*(cr0*ui(i1,i2+3,i3+2,c3)+
     & cr1*ui(i1+1,i2+3,i3+2,c3)+cr2*ui(i1+2,i2+3,i3+2,c3)+cr3*ui(i1+
     & 3,i2+3,i3+2,c3)+cr4*ui(i1+4,i2+3,i3+2,c3))+cs4*(cr0*ui(i1,i2+4,
     & i3+2,c3)+cr1*ui(i1+1,i2+4,i3+2,c3)+cr2*ui(i1+2,i2+4,i3+2,c3)+
     & cr3*ui(i1+3,i2+4,i3+2,c3)+cr4*ui(i1+4,i2+4,i3+2,c3)))+ct3*(cs0*
     & (cr0*ui(i1,i2,i3+3,c3)+cr1*ui(i1+1,i2,i3+3,c3)+cr2*ui(i1+2,i2,
     & i3+3,c3)+cr3*ui(i1+3,i2,i3+3,c3)+cr4*ui(i1+4,i2,i3+3,c3))+cs1*(
     & cr0*ui(i1,i2+1,i3+3,c3)+cr1*ui(i1+1,i2+1,i3+3,c3)+cr2*ui(i1+2,
     & i2+1,i3+3,c3)+cr3*ui(i1+3,i2+1,i3+3,c3)+cr4*ui(i1+4,i2+1,i3+3,
     & c3))+cs2*(cr0*ui(i1,i2+2,i3+3,c3)+cr1*ui(i1+1,i2+2,i3+3,c3)+
     & cr2*ui(i1+2,i2+2,i3+3,c3)+cr3*ui(i1+3,i2+2,i3+3,c3)+cr4*ui(i1+
     & 4,i2+2,i3+3,c3))+cs3*(cr0*ui(i1,i2+3,i3+3,c3)+cr1*ui(i1+1,i2+3,
     & i3+3,c3)+cr2*ui(i1+2,i2+3,i3+3,c3)+cr3*ui(i1+3,i2+3,i3+3,c3)+
     & cr4*ui(i1+4,i2+3,i3+3,c3))+cs4*(cr0*ui(i1,i2+4,i3+3,c3)+cr1*ui(
     & i1+1,i2+4,i3+3,c3)+cr2*ui(i1+2,i2+4,i3+3,c3)+cr3*ui(i1+3,i2+4,
     & i3+3,c3)+cr4*ui(i1+4,i2+4,i3+3,c3)))+ct4*(cs0*(cr0*ui(i1,i2,i3+
     & 4,c3)+cr1*ui(i1+1,i2,i3+4,c3)+cr2*ui(i1+2,i2,i3+4,c3)+cr3*ui(
     & i1+3,i2,i3+4,c3)+cr4*ui(i1+4,i2,i3+4,c3))+cs1*(cr0*ui(i1,i2+1,
     & i3+4,c3)+cr1*ui(i1+1,i2+1,i3+4,c3)+cr2*ui(i1+2,i2+1,i3+4,c3)+
     & cr3*ui(i1+3,i2+1,i3+4,c3)+cr4*ui(i1+4,i2+1,i3+4,c3))+cs2*(cr0*
     & ui(i1,i2+2,i3+4,c3)+cr1*ui(i1+1,i2+2,i3+4,c3)+cr2*ui(i1+2,i2+2,
     & i3+4,c3)+cr3*ui(i1+3,i2+2,i3+4,c3)+cr4*ui(i1+4,i2+2,i3+4,c3))+
     & cs3*(cr0*ui(i1,i2+3,i3+4,c3)+cr1*ui(i1+1,i2+3,i3+4,c3)+cr2*ui(
     & i1+2,i2+3,i3+4,c3)+cr3*ui(i1+3,i2+3,i3+4,c3)+cr4*ui(i1+4,i2+3,
     & i3+4,c3))+cs4*(cr0*ui(i1,i2+4,i3+4,c3)+cr1*ui(i1+1,i2+4,i3+4,
     & c3)+cr2*ui(i1+2,i2+4,i3+4,c3)+cr3*ui(i1+3,i2+4,i3+4,c3)+cr4*ui(
     & i1+4,i2+4,i3+4,c3)))
             else if( varWidth(i).eq.4 )then
! interpSparseStorage444(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q40(c(i,0,0,0))
                 cs0 = q40(c(i,1,0,0))
                 ct0 = q40(c(i,2,0,0))
                 cr1 = q41(c(i,0,0,0))
                 cs1 = q41(c(i,1,0,0))
                 ct1 = q41(c(i,2,0,0))
                 cr2 = q42(c(i,0,0,0))
                 cs2 = q42(c(i,1,0,0))
                 ct2 = q42(c(i,2,0,0))
                 cr3 = q43(c(i,0,0,0))
                 cs3 = q43(c(i,1,0,0))
                 ct3 = q43(c(i,2,0,0))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ct0*(cs0*(cr0*ui(i1,i2,
     & i3+0,c3)+cr1*ui(i1+1,i2,i3+0,c3)+cr2*ui(i1+2,i2,i3+0,c3)+cr3*
     & ui(i1+3,i2,i3+0,c3))+cs1*(cr0*ui(i1,i2+1,i3+0,c3)+cr1*ui(i1+1,
     & i2+1,i3+0,c3)+cr2*ui(i1+2,i2+1,i3+0,c3)+cr3*ui(i1+3,i2+1,i3+0,
     & c3))+cs2*(cr0*ui(i1,i2+2,i3+0,c3)+cr1*ui(i1+1,i2+2,i3+0,c3)+
     & cr2*ui(i1+2,i2+2,i3+0,c3)+cr3*ui(i1+3,i2+2,i3+0,c3))+cs3*(cr0*
     & ui(i1,i2+3,i3+0,c3)+cr1*ui(i1+1,i2+3,i3+0,c3)+cr2*ui(i1+2,i2+3,
     & i3+0,c3)+cr3*ui(i1+3,i2+3,i3+0,c3)))+ct1*(cs0*(cr0*ui(i1,i2,i3+
     & 1,c3)+cr1*ui(i1+1,i2,i3+1,c3)+cr2*ui(i1+2,i2,i3+1,c3)+cr3*ui(
     & i1+3,i2,i3+1,c3))+cs1*(cr0*ui(i1,i2+1,i3+1,c3)+cr1*ui(i1+1,i2+
     & 1,i3+1,c3)+cr2*ui(i1+2,i2+1,i3+1,c3)+cr3*ui(i1+3,i2+1,i3+1,c3))
     & +cs2*(cr0*ui(i1,i2+2,i3+1,c3)+cr1*ui(i1+1,i2+2,i3+1,c3)+cr2*ui(
     & i1+2,i2+2,i3+1,c3)+cr3*ui(i1+3,i2+2,i3+1,c3))+cs3*(cr0*ui(i1,
     & i2+3,i3+1,c3)+cr1*ui(i1+1,i2+3,i3+1,c3)+cr2*ui(i1+2,i2+3,i3+1,
     & c3)+cr3*ui(i1+3,i2+3,i3+1,c3)))+ct2*(cs0*(cr0*ui(i1,i2,i3+2,c3)
     & +cr1*ui(i1+1,i2,i3+2,c3)+cr2*ui(i1+2,i2,i3+2,c3)+cr3*ui(i1+3,
     & i2,i3+2,c3))+cs1*(cr0*ui(i1,i2+1,i3+2,c3)+cr1*ui(i1+1,i2+1,i3+
     & 2,c3)+cr2*ui(i1+2,i2+1,i3+2,c3)+cr3*ui(i1+3,i2+1,i3+2,c3))+cs2*
     & (cr0*ui(i1,i2+2,i3+2,c3)+cr1*ui(i1+1,i2+2,i3+2,c3)+cr2*ui(i1+2,
     & i2+2,i3+2,c3)+cr3*ui(i1+3,i2+2,i3+2,c3))+cs3*(cr0*ui(i1,i2+3,
     & i3+2,c3)+cr1*ui(i1+1,i2+3,i3+2,c3)+cr2*ui(i1+2,i2+3,i3+2,c3)+
     & cr3*ui(i1+3,i2+3,i3+2,c3)))+ct3*(cs0*(cr0*ui(i1,i2,i3+3,c3)+
     & cr1*ui(i1+1,i2,i3+3,c3)+cr2*ui(i1+2,i2,i3+3,c3)+cr3*ui(i1+3,i2,
     & i3+3,c3))+cs1*(cr0*ui(i1,i2+1,i3+3,c3)+cr1*ui(i1+1,i2+1,i3+3,
     & c3)+cr2*ui(i1+2,i2+1,i3+3,c3)+cr3*ui(i1+3,i2+1,i3+3,c3))+cs2*(
     & cr0*ui(i1,i2+2,i3+3,c3)+cr1*ui(i1+1,i2+2,i3+3,c3)+cr2*ui(i1+2,
     & i2+2,i3+3,c3)+cr3*ui(i1+3,i2+2,i3+3,c3))+cs3*(cr0*ui(i1,i2+3,
     & i3+3,c3)+cr1*ui(i1+1,i2+3,i3+3,c3)+cr2*ui(i1+2,i2+3,i3+3,c3)+
     & cr3*ui(i1+3,i2+3,i3+3,c3)))
             else if( varWidth(i).eq.7 )then
! interpSparseStorage777(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q70(c(i,0,0,0))
                 cs0 = q70(c(i,1,0,0))
                 ct0 = q70(c(i,2,0,0))
                 cr1 = q71(c(i,0,0,0))
                 cs1 = q71(c(i,1,0,0))
                 ct1 = q71(c(i,2,0,0))
                 cr2 = q72(c(i,0,0,0))
                 cs2 = q72(c(i,1,0,0))
                 ct2 = q72(c(i,2,0,0))
                 cr3 = q73(c(i,0,0,0))
                 cs3 = q73(c(i,1,0,0))
                 ct3 = q73(c(i,2,0,0))
                 cr4 = q74(c(i,0,0,0))
                 cs4 = q74(c(i,1,0,0))
                 ct4 = q74(c(i,2,0,0))
                 cr5 = q75(c(i,0,0,0))
                 cs5 = q75(c(i,1,0,0))
                 ct5 = q75(c(i,2,0,0))
                 cr6 = q76(c(i,0,0,0))
                 cs6 = q76(c(i,1,0,0))
                 ct6 = q76(c(i,2,0,0))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ct0*(cs0*(cr0*ui(i1,i2,
     & i3+0,c3)+cr1*ui(i1+1,i2,i3+0,c3)+cr2*ui(i1+2,i2,i3+0,c3)+cr3*
     & ui(i1+3,i2,i3+0,c3)+cr4*ui(i1+4,i2,i3+0,c3)+cr5*ui(i1+5,i2,i3+
     & 0,c3)+cr6*ui(i1+6,i2,i3+0,c3))+cs1*(cr0*ui(i1,i2+1,i3+0,c3)+
     & cr1*ui(i1+1,i2+1,i3+0,c3)+cr2*ui(i1+2,i2+1,i3+0,c3)+cr3*ui(i1+
     & 3,i2+1,i3+0,c3)+cr4*ui(i1+4,i2+1,i3+0,c3)+cr5*ui(i1+5,i2+1,i3+
     & 0,c3)+cr6*ui(i1+6,i2+1,i3+0,c3))+cs2*(cr0*ui(i1,i2+2,i3+0,c3)+
     & cr1*ui(i1+1,i2+2,i3+0,c3)+cr2*ui(i1+2,i2+2,i3+0,c3)+cr3*ui(i1+
     & 3,i2+2,i3+0,c3)+cr4*ui(i1+4,i2+2,i3+0,c3)+cr5*ui(i1+5,i2+2,i3+
     & 0,c3)+cr6*ui(i1+6,i2+2,i3+0,c3))+cs3*(cr0*ui(i1,i2+3,i3+0,c3)+
     & cr1*ui(i1+1,i2+3,i3+0,c3)+cr2*ui(i1+2,i2+3,i3+0,c3)+cr3*ui(i1+
     & 3,i2+3,i3+0,c3)+cr4*ui(i1+4,i2+3,i3+0,c3)+cr5*ui(i1+5,i2+3,i3+
     & 0,c3)+cr6*ui(i1+6,i2+3,i3+0,c3))+cs4*(cr0*ui(i1,i2+4,i3+0,c3)+
     & cr1*ui(i1+1,i2+4,i3+0,c3)+cr2*ui(i1+2,i2+4,i3+0,c3)+cr3*ui(i1+
     & 3,i2+4,i3+0,c3)+cr4*ui(i1+4,i2+4,i3+0,c3)+cr5*ui(i1+5,i2+4,i3+
     & 0,c3)+cr6*ui(i1+6,i2+4,i3+0,c3))+cs5*(cr0*ui(i1,i2+5,i3+0,c3)+
     & cr1*ui(i1+1,i2+5,i3+0,c3)+cr2*ui(i1+2,i2+5,i3+0,c3)+cr3*ui(i1+
     & 3,i2+5,i3+0,c3)+cr4*ui(i1+4,i2+5,i3+0,c3)+cr5*ui(i1+5,i2+5,i3+
     & 0,c3)+cr6*ui(i1+6,i2+5,i3+0,c3))+cs6*(cr0*ui(i1,i2+6,i3+0,c3)+
     & cr1*ui(i1+1,i2+6,i3+0,c3)+cr2*ui(i1+2,i2+6,i3+0,c3)+cr3*ui(i1+
     & 3,i2+6,i3+0,c3)+cr4*ui(i1+4,i2+6,i3+0,c3)+cr5*ui(i1+5,i2+6,i3+
     & 0,c3)+cr6*ui(i1+6,i2+6,i3+0,c3)))+ct1*(cs0*(cr0*ui(i1,i2,i3+1,
     & c3)+cr1*ui(i1+1,i2,i3+1,c3)+cr2*ui(i1+2,i2,i3+1,c3)+cr3*ui(i1+
     & 3,i2,i3+1,c3)+cr4*ui(i1+4,i2,i3+1,c3)+cr5*ui(i1+5,i2,i3+1,c3)+
     & cr6*ui(i1+6,i2,i3+1,c3))+cs1*(cr0*ui(i1,i2+1,i3+1,c3)+cr1*ui(
     & i1+1,i2+1,i3+1,c3)+cr2*ui(i1+2,i2+1,i3+1,c3)+cr3*ui(i1+3,i2+1,
     & i3+1,c3)+cr4*ui(i1+4,i2+1,i3+1,c3)+cr5*ui(i1+5,i2+1,i3+1,c3)+
     & cr6*ui(i1+6,i2+1,i3+1,c3))+cs2*(cr0*ui(i1,i2+2,i3+1,c3)+cr1*ui(
     & i1+1,i2+2,i3+1,c3)+cr2*ui(i1+2,i2+2,i3+1,c3)+cr3*ui(i1+3,i2+2,
     & i3+1,c3)+cr4*ui(i1+4,i2+2,i3+1,c3)+cr5*ui(i1+5,i2+2,i3+1,c3)+
     & cr6*ui(i1+6,i2+2,i3+1,c3))+cs3*(cr0*ui(i1,i2+3,i3+1,c3)+cr1*ui(
     & i1+1,i2+3,i3+1,c3)+cr2*ui(i1+2,i2+3,i3+1,c3)+cr3*ui(i1+3,i2+3,
     & i3+1,c3)+cr4*ui(i1+4,i2+3,i3+1,c3)+cr5*ui(i1+5,i2+3,i3+1,c3)+
     & cr6*ui(i1+6,i2+3,i3+1,c3))+cs4*(cr0*ui(i1,i2+4,i3+1,c3)+cr1*ui(
     & i1+1,i2+4,i3+1,c3)+cr2*ui(i1+2,i2+4,i3+1,c3)+cr3*ui(i1+3,i2+4,
     & i3+1,c3)+cr4*ui(i1+4,i2+4,i3+1,c3)+cr5*ui(i1+5,i2+4,i3+1,c3)+
     & cr6*ui(i1+6,i2+4,i3+1,c3))+cs5*(cr0*ui(i1,i2+5,i3+1,c3)+cr1*ui(
     & i1+1,i2+5,i3+1,c3)+cr2*ui(i1+2,i2+5,i3+1,c3)+cr3*ui(i1+3,i2+5,
     & i3+1,c3)+cr4*ui(i1+4,i2+5,i3+1,c3)+cr5*ui(i1+5,i2+5,i3+1,c3)+
     & cr6*ui(i1+6,i2+5,i3+1,c3))+cs6*(cr0*ui(i1,i2+6,i3+1,c3)+cr1*ui(
     & i1+1,i2+6,i3+1,c3)+cr2*ui(i1+2,i2+6,i3+1,c3)+cr3*ui(i1+3,i2+6,
     & i3+1,c3)+cr4*ui(i1+4,i2+6,i3+1,c3)+cr5*ui(i1+5,i2+6,i3+1,c3)+
     & cr6*ui(i1+6,i2+6,i3+1,c3)))+ct2*(cs0*(cr0*ui(i1,i2,i3+2,c3)+
     & cr1*ui(i1+1,i2,i3+2,c3)+cr2*ui(i1+2,i2,i3+2,c3)+cr3*ui(i1+3,i2,
     & i3+2,c3)+cr4*ui(i1+4,i2,i3+2,c3)+cr5*ui(i1+5,i2,i3+2,c3)+cr6*
     & ui(i1+6,i2,i3+2,c3))+cs1*(cr0*ui(i1,i2+1,i3+2,c3)+cr1*ui(i1+1,
     & i2+1,i3+2,c3)+cr2*ui(i1+2,i2+1,i3+2,c3)+cr3*ui(i1+3,i2+1,i3+2,
     & c3)+cr4*ui(i1+4,i2+1,i3+2,c3)+cr5*ui(i1+5,i2+1,i3+2,c3)+cr6*ui(
     & i1+6,i2+1,i3+2,c3))+cs2*(cr0*ui(i1,i2+2,i3+2,c3)+cr1*ui(i1+1,
     & i2+2,i3+2,c3)+cr2*ui(i1+2,i2+2,i3+2,c3)+cr3*ui(i1+3,i2+2,i3+2,
     & c3)+cr4*ui(i1+4,i2+2,i3+2,c3)+cr5*ui(i1+5,i2+2,i3+2,c3)+cr6*ui(
     & i1+6,i2+2,i3+2,c3))+cs3*(cr0*ui(i1,i2+3,i3+2,c3)+cr1*ui(i1+1,
     & i2+3,i3+2,c3)+cr2*ui(i1+2,i2+3,i3+2,c3)+cr3*ui(i1+3,i2+3,i3+2,
     & c3)+cr4*ui(i1+4,i2+3,i3+2,c3)+cr5*ui(i1+5,i2+3,i3+2,c3)+cr6*ui(
     & i1+6,i2+3,i3+2,c3))+cs4*(cr0*ui(i1,i2+4,i3+2,c3)+cr1*ui(i1+1,
     & i2+4,i3+2,c3)+cr2*ui(i1+2,i2+4,i3+2,c3)+cr3*ui(i1+3,i2+4,i3+2,
     & c3)+cr4*ui(i1+4,i2+4,i3+2,c3)+cr5*ui(i1+5,i2+4,i3+2,c3)+cr6*ui(
     & i1+6,i2+4,i3+2,c3))+cs5*(cr0*ui(i1,i2+5,i3+2,c3)+cr1*ui(i1+1,
     & i2+5,i3+2,c3)+cr2*ui(i1+2,i2+5,i3+2,c3)+cr3*ui(i1+3,i2+5,i3+2,
     & c3)+cr4*ui(i1+4,i2+5,i3+2,c3)+cr5*ui(i1+5,i2+5,i3+2,c3)+cr6*ui(
     & i1+6,i2+5,i3+2,c3))+cs6*(cr0*ui(i1,i2+6,i3+2,c3)+cr1*ui(i1+1,
     & i2+6,i3+2,c3)+cr2*ui(i1+2,i2+6,i3+2,c3)+cr3*ui(i1+3,i2+6,i3+2,
     & c3)+cr4*ui(i1+4,i2+6,i3+2,c3)+cr5*ui(i1+5,i2+6,i3+2,c3)+cr6*ui(
     & i1+6,i2+6,i3+2,c3)))+ct3*(cs0*(cr0*ui(i1,i2,i3+3,c3)+cr1*ui(i1+
     & 1,i2,i3+3,c3)+cr2*ui(i1+2,i2,i3+3,c3)+cr3*ui(i1+3,i2,i3+3,c3)+
     & cr4*ui(i1+4,i2,i3+3,c3)+cr5*ui(i1+5,i2,i3+3,c3)+cr6*ui(i1+6,i2,
     & i3+3,c3))+cs1*(cr0*ui(i1,i2+1,i3+3,c3)+cr1*ui(i1+1,i2+1,i3+3,
     & c3)+cr2*ui(i1+2,i2+1,i3+3,c3)+cr3*ui(i1+3,i2+1,i3+3,c3)+cr4*ui(
     & i1+4,i2+1,i3+3,c3)+cr5*ui(i1+5,i2+1,i3+3,c3)+cr6*ui(i1+6,i2+1,
     & i3+3,c3))+cs2*(cr0*ui(i1,i2+2,i3+3,c3)+cr1*ui(i1+1,i2+2,i3+3,
     & c3)+cr2*ui(i1+2,i2+2,i3+3,c3)+cr3*ui(i1+3,i2+2,i3+3,c3)+cr4*ui(
     & i1+4,i2+2,i3+3,c3)+cr5*ui(i1+5,i2+2,i3+3,c3)+cr6*ui(i1+6,i2+2,
     & i3+3,c3))+cs3*(cr0*ui(i1,i2+3,i3+3,c3)+cr1*ui(i1+1,i2+3,i3+3,
     & c3)+cr2*ui(i1+2,i2+3,i3+3,c3)+cr3*ui(i1+3,i2+3,i3+3,c3)+cr4*ui(
     & i1+4,i2+3,i3+3,c3)+cr5*ui(i1+5,i2+3,i3+3,c3)+cr6*ui(i1+6,i2+3,
     & i3+3,c3))+cs4*(cr0*ui(i1,i2+4,i3+3,c3)+cr1*ui(i1+1,i2+4,i3+3,
     & c3)+cr2*ui(i1+2,i2+4,i3+3,c3)+cr3*ui(i1+3,i2+4,i3+3,c3)+cr4*ui(
     & i1+4,i2+4,i3+3,c3)+cr5*ui(i1+5,i2+4,i3+3,c3)+cr6*ui(i1+6,i2+4,
     & i3+3,c3))+cs5*(cr0*ui(i1,i2+5,i3+3,c3)+cr1*ui(i1+1,i2+5,i3+3,
     & c3)+cr2*ui(i1+2,i2+5,i3+3,c3)+cr3*ui(i1+3,i2+5,i3+3,c3)+cr4*ui(
     & i1+4,i2+5,i3+3,c3)+cr5*ui(i1+5,i2+5,i3+3,c3)+cr6*ui(i1+6,i2+5,
     & i3+3,c3))+cs6*(cr0*ui(i1,i2+6,i3+3,c3)+cr1*ui(i1+1,i2+6,i3+3,
     & c3)+cr2*ui(i1+2,i2+6,i3+3,c3)+cr3*ui(i1+3,i2+6,i3+3,c3)+cr4*ui(
     & i1+4,i2+6,i3+3,c3)+cr5*ui(i1+5,i2+6,i3+3,c3)+cr6*ui(i1+6,i2+6,
     & i3+3,c3)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+ct4*(cs0*(cr0*ui(i1,i2,i3+4,c3)+cr1*ui(i1+1,i2,i3+4,
     & c3)+cr2*ui(i1+2,i2,i3+4,c3)+cr3*ui(i1+3,i2,i3+4,c3)+cr4*ui(i1+
     & 4,i2,i3+4,c3)+cr5*ui(i1+5,i2,i3+4,c3)+cr6*ui(i1+6,i2,i3+4,c3))+
     & cs1*(cr0*ui(i1,i2+1,i3+4,c3)+cr1*ui(i1+1,i2+1,i3+4,c3)+cr2*ui(
     & i1+2,i2+1,i3+4,c3)+cr3*ui(i1+3,i2+1,i3+4,c3)+cr4*ui(i1+4,i2+1,
     & i3+4,c3)+cr5*ui(i1+5,i2+1,i3+4,c3)+cr6*ui(i1+6,i2+1,i3+4,c3))+
     & cs2*(cr0*ui(i1,i2+2,i3+4,c3)+cr1*ui(i1+1,i2+2,i3+4,c3)+cr2*ui(
     & i1+2,i2+2,i3+4,c3)+cr3*ui(i1+3,i2+2,i3+4,c3)+cr4*ui(i1+4,i2+2,
     & i3+4,c3)+cr5*ui(i1+5,i2+2,i3+4,c3)+cr6*ui(i1+6,i2+2,i3+4,c3))+
     & cs3*(cr0*ui(i1,i2+3,i3+4,c3)+cr1*ui(i1+1,i2+3,i3+4,c3)+cr2*ui(
     & i1+2,i2+3,i3+4,c3)+cr3*ui(i1+3,i2+3,i3+4,c3)+cr4*ui(i1+4,i2+3,
     & i3+4,c3)+cr5*ui(i1+5,i2+3,i3+4,c3)+cr6*ui(i1+6,i2+3,i3+4,c3))+
     & cs4*(cr0*ui(i1,i2+4,i3+4,c3)+cr1*ui(i1+1,i2+4,i3+4,c3)+cr2*ui(
     & i1+2,i2+4,i3+4,c3)+cr3*ui(i1+3,i2+4,i3+4,c3)+cr4*ui(i1+4,i2+4,
     & i3+4,c3)+cr5*ui(i1+5,i2+4,i3+4,c3)+cr6*ui(i1+6,i2+4,i3+4,c3))+
     & cs5*(cr0*ui(i1,i2+5,i3+4,c3)+cr1*ui(i1+1,i2+5,i3+4,c3)+cr2*ui(
     & i1+2,i2+5,i3+4,c3)+cr3*ui(i1+3,i2+5,i3+4,c3)+cr4*ui(i1+4,i2+5,
     & i3+4,c3)+cr5*ui(i1+5,i2+5,i3+4,c3)+cr6*ui(i1+6,i2+5,i3+4,c3))+
     & cs6*(cr0*ui(i1,i2+6,i3+4,c3)+cr1*ui(i1+1,i2+6,i3+4,c3)+cr2*ui(
     & i1+2,i2+6,i3+4,c3)+cr3*ui(i1+3,i2+6,i3+4,c3)+cr4*ui(i1+4,i2+6,
     & i3+4,c3)+cr5*ui(i1+5,i2+6,i3+4,c3)+cr6*ui(i1+6,i2+6,i3+4,c3)))+
     & ct5*(cs0*(cr0*ui(i1,i2,i3+5,c3)+cr1*ui(i1+1,i2,i3+5,c3)+cr2*ui(
     & i1+2,i2,i3+5,c3)+cr3*ui(i1+3,i2,i3+5,c3)+cr4*ui(i1+4,i2,i3+5,
     & c3)+cr5*ui(i1+5,i2,i3+5,c3)+cr6*ui(i1+6,i2,i3+5,c3))+cs1*(cr0*
     & ui(i1,i2+1,i3+5,c3)+cr1*ui(i1+1,i2+1,i3+5,c3)+cr2*ui(i1+2,i2+1,
     & i3+5,c3)+cr3*ui(i1+3,i2+1,i3+5,c3)+cr4*ui(i1+4,i2+1,i3+5,c3)+
     & cr5*ui(i1+5,i2+1,i3+5,c3)+cr6*ui(i1+6,i2+1,i3+5,c3))+cs2*(cr0*
     & ui(i1,i2+2,i3+5,c3)+cr1*ui(i1+1,i2+2,i3+5,c3)+cr2*ui(i1+2,i2+2,
     & i3+5,c3)+cr3*ui(i1+3,i2+2,i3+5,c3)+cr4*ui(i1+4,i2+2,i3+5,c3)+
     & cr5*ui(i1+5,i2+2,i3+5,c3)+cr6*ui(i1+6,i2+2,i3+5,c3))+cs3*(cr0*
     & ui(i1,i2+3,i3+5,c3)+cr1*ui(i1+1,i2+3,i3+5,c3)+cr2*ui(i1+2,i2+3,
     & i3+5,c3)+cr3*ui(i1+3,i2+3,i3+5,c3)+cr4*ui(i1+4,i2+3,i3+5,c3)+
     & cr5*ui(i1+5,i2+3,i3+5,c3)+cr6*ui(i1+6,i2+3,i3+5,c3))+cs4*(cr0*
     & ui(i1,i2+4,i3+5,c3)+cr1*ui(i1+1,i2+4,i3+5,c3)+cr2*ui(i1+2,i2+4,
     & i3+5,c3)+cr3*ui(i1+3,i2+4,i3+5,c3)+cr4*ui(i1+4,i2+4,i3+5,c3)+
     & cr5*ui(i1+5,i2+4,i3+5,c3)+cr6*ui(i1+6,i2+4,i3+5,c3))+cs5*(cr0*
     & ui(i1,i2+5,i3+5,c3)+cr1*ui(i1+1,i2+5,i3+5,c3)+cr2*ui(i1+2,i2+5,
     & i3+5,c3)+cr3*ui(i1+3,i2+5,i3+5,c3)+cr4*ui(i1+4,i2+5,i3+5,c3)+
     & cr5*ui(i1+5,i2+5,i3+5,c3)+cr6*ui(i1+6,i2+5,i3+5,c3))+cs6*(cr0*
     & ui(i1,i2+6,i3+5,c3)+cr1*ui(i1+1,i2+6,i3+5,c3)+cr2*ui(i1+2,i2+6,
     & i3+5,c3)+cr3*ui(i1+3,i2+6,i3+5,c3)+cr4*ui(i1+4,i2+6,i3+5,c3)+
     & cr5*ui(i1+5,i2+6,i3+5,c3)+cr6*ui(i1+6,i2+6,i3+5,c3)))+ct6*(cs0*
     & (cr0*ui(i1,i2,i3+6,c3)+cr1*ui(i1+1,i2,i3+6,c3)+cr2*ui(i1+2,i2,
     & i3+6,c3)+cr3*ui(i1+3,i2,i3+6,c3)+cr4*ui(i1+4,i2,i3+6,c3)+cr5*
     & ui(i1+5,i2,i3+6,c3)+cr6*ui(i1+6,i2,i3+6,c3))+cs1*(cr0*ui(i1,i2+
     & 1,i3+6,c3)+cr1*ui(i1+1,i2+1,i3+6,c3)+cr2*ui(i1+2,i2+1,i3+6,c3)+
     & cr3*ui(i1+3,i2+1,i3+6,c3)+cr4*ui(i1+4,i2+1,i3+6,c3)+cr5*ui(i1+
     & 5,i2+1,i3+6,c3)+cr6*ui(i1+6,i2+1,i3+6,c3))+cs2*(cr0*ui(i1,i2+2,
     & i3+6,c3)+cr1*ui(i1+1,i2+2,i3+6,c3)+cr2*ui(i1+2,i2+2,i3+6,c3)+
     & cr3*ui(i1+3,i2+2,i3+6,c3)+cr4*ui(i1+4,i2+2,i3+6,c3)+cr5*ui(i1+
     & 5,i2+2,i3+6,c3)+cr6*ui(i1+6,i2+2,i3+6,c3))+cs3*(cr0*ui(i1,i2+3,
     & i3+6,c3)+cr1*ui(i1+1,i2+3,i3+6,c3)+cr2*ui(i1+2,i2+3,i3+6,c3)+
     & cr3*ui(i1+3,i2+3,i3+6,c3)+cr4*ui(i1+4,i2+3,i3+6,c3)+cr5*ui(i1+
     & 5,i2+3,i3+6,c3)+cr6*ui(i1+6,i2+3,i3+6,c3))+cs4*(cr0*ui(i1,i2+4,
     & i3+6,c3)+cr1*ui(i1+1,i2+4,i3+6,c3)+cr2*ui(i1+2,i2+4,i3+6,c3)+
     & cr3*ui(i1+3,i2+4,i3+6,c3)+cr4*ui(i1+4,i2+4,i3+6,c3)+cr5*ui(i1+
     & 5,i2+4,i3+6,c3)+cr6*ui(i1+6,i2+4,i3+6,c3))+cs5*(cr0*ui(i1,i2+5,
     & i3+6,c3)+cr1*ui(i1+1,i2+5,i3+6,c3)+cr2*ui(i1+2,i2+5,i3+6,c3)+
     & cr3*ui(i1+3,i2+5,i3+6,c3)+cr4*ui(i1+4,i2+5,i3+6,c3)+cr5*ui(i1+
     & 5,i2+5,i3+6,c3)+cr6*ui(i1+6,i2+5,i3+6,c3))+cs6*(cr0*ui(i1,i2+6,
     & i3+6,c3)+cr1*ui(i1+1,i2+6,i3+6,c3)+cr2*ui(i1+2,i2+6,i3+6,c3)+
     & cr3*ui(i1+3,i2+6,i3+6,c3)+cr4*ui(i1+4,i2+6,i3+6,c3)+cr5*ui(i1+
     & 5,i2+6,i3+6,c3)+cr6*ui(i1+6,i2+6,i3+6,c3)))
             else if( varWidth(i).eq.6 )then
! interpSparseStorage666(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q60(c(i,0,0,0))
                 cs0 = q60(c(i,1,0,0))
                 ct0 = q60(c(i,2,0,0))
                 cr1 = q61(c(i,0,0,0))
                 cs1 = q61(c(i,1,0,0))
                 ct1 = q61(c(i,2,0,0))
                 cr2 = q62(c(i,0,0,0))
                 cs2 = q62(c(i,1,0,0))
                 ct2 = q62(c(i,2,0,0))
                 cr3 = q63(c(i,0,0,0))
                 cs3 = q63(c(i,1,0,0))
                 ct3 = q63(c(i,2,0,0))
                 cr4 = q64(c(i,0,0,0))
                 cs4 = q64(c(i,1,0,0))
                 ct4 = q64(c(i,2,0,0))
                 cr5 = q65(c(i,0,0,0))
                 cs5 = q65(c(i,1,0,0))
                 ct5 = q65(c(i,2,0,0))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ct0*(cs0*(cr0*ui(i1,i2,
     & i3+0,c3)+cr1*ui(i1+1,i2,i3+0,c3)+cr2*ui(i1+2,i2,i3+0,c3)+cr3*
     & ui(i1+3,i2,i3+0,c3)+cr4*ui(i1+4,i2,i3+0,c3)+cr5*ui(i1+5,i2,i3+
     & 0,c3))+cs1*(cr0*ui(i1,i2+1,i3+0,c3)+cr1*ui(i1+1,i2+1,i3+0,c3)+
     & cr2*ui(i1+2,i2+1,i3+0,c3)+cr3*ui(i1+3,i2+1,i3+0,c3)+cr4*ui(i1+
     & 4,i2+1,i3+0,c3)+cr5*ui(i1+5,i2+1,i3+0,c3))+cs2*(cr0*ui(i1,i2+2,
     & i3+0,c3)+cr1*ui(i1+1,i2+2,i3+0,c3)+cr2*ui(i1+2,i2+2,i3+0,c3)+
     & cr3*ui(i1+3,i2+2,i3+0,c3)+cr4*ui(i1+4,i2+2,i3+0,c3)+cr5*ui(i1+
     & 5,i2+2,i3+0,c3))+cs3*(cr0*ui(i1,i2+3,i3+0,c3)+cr1*ui(i1+1,i2+3,
     & i3+0,c3)+cr2*ui(i1+2,i2+3,i3+0,c3)+cr3*ui(i1+3,i2+3,i3+0,c3)+
     & cr4*ui(i1+4,i2+3,i3+0,c3)+cr5*ui(i1+5,i2+3,i3+0,c3))+cs4*(cr0*
     & ui(i1,i2+4,i3+0,c3)+cr1*ui(i1+1,i2+4,i3+0,c3)+cr2*ui(i1+2,i2+4,
     & i3+0,c3)+cr3*ui(i1+3,i2+4,i3+0,c3)+cr4*ui(i1+4,i2+4,i3+0,c3)+
     & cr5*ui(i1+5,i2+4,i3+0,c3))+cs5*(cr0*ui(i1,i2+5,i3+0,c3)+cr1*ui(
     & i1+1,i2+5,i3+0,c3)+cr2*ui(i1+2,i2+5,i3+0,c3)+cr3*ui(i1+3,i2+5,
     & i3+0,c3)+cr4*ui(i1+4,i2+5,i3+0,c3)+cr5*ui(i1+5,i2+5,i3+0,c3)))+
     & ct1*(cs0*(cr0*ui(i1,i2,i3+1,c3)+cr1*ui(i1+1,i2,i3+1,c3)+cr2*ui(
     & i1+2,i2,i3+1,c3)+cr3*ui(i1+3,i2,i3+1,c3)+cr4*ui(i1+4,i2,i3+1,
     & c3)+cr5*ui(i1+5,i2,i3+1,c3))+cs1*(cr0*ui(i1,i2+1,i3+1,c3)+cr1*
     & ui(i1+1,i2+1,i3+1,c3)+cr2*ui(i1+2,i2+1,i3+1,c3)+cr3*ui(i1+3,i2+
     & 1,i3+1,c3)+cr4*ui(i1+4,i2+1,i3+1,c3)+cr5*ui(i1+5,i2+1,i3+1,c3))
     & +cs2*(cr0*ui(i1,i2+2,i3+1,c3)+cr1*ui(i1+1,i2+2,i3+1,c3)+cr2*ui(
     & i1+2,i2+2,i3+1,c3)+cr3*ui(i1+3,i2+2,i3+1,c3)+cr4*ui(i1+4,i2+2,
     & i3+1,c3)+cr5*ui(i1+5,i2+2,i3+1,c3))+cs3*(cr0*ui(i1,i2+3,i3+1,
     & c3)+cr1*ui(i1+1,i2+3,i3+1,c3)+cr2*ui(i1+2,i2+3,i3+1,c3)+cr3*ui(
     & i1+3,i2+3,i3+1,c3)+cr4*ui(i1+4,i2+3,i3+1,c3)+cr5*ui(i1+5,i2+3,
     & i3+1,c3))+cs4*(cr0*ui(i1,i2+4,i3+1,c3)+cr1*ui(i1+1,i2+4,i3+1,
     & c3)+cr2*ui(i1+2,i2+4,i3+1,c3)+cr3*ui(i1+3,i2+4,i3+1,c3)+cr4*ui(
     & i1+4,i2+4,i3+1,c3)+cr5*ui(i1+5,i2+4,i3+1,c3))+cs5*(cr0*ui(i1,
     & i2+5,i3+1,c3)+cr1*ui(i1+1,i2+5,i3+1,c3)+cr2*ui(i1+2,i2+5,i3+1,
     & c3)+cr3*ui(i1+3,i2+5,i3+1,c3)+cr4*ui(i1+4,i2+5,i3+1,c3)+cr5*ui(
     & i1+5,i2+5,i3+1,c3)))+ct2*(cs0*(cr0*ui(i1,i2,i3+2,c3)+cr1*ui(i1+
     & 1,i2,i3+2,c3)+cr2*ui(i1+2,i2,i3+2,c3)+cr3*ui(i1+3,i2,i3+2,c3)+
     & cr4*ui(i1+4,i2,i3+2,c3)+cr5*ui(i1+5,i2,i3+2,c3))+cs1*(cr0*ui(
     & i1,i2+1,i3+2,c3)+cr1*ui(i1+1,i2+1,i3+2,c3)+cr2*ui(i1+2,i2+1,i3+
     & 2,c3)+cr3*ui(i1+3,i2+1,i3+2,c3)+cr4*ui(i1+4,i2+1,i3+2,c3)+cr5*
     & ui(i1+5,i2+1,i3+2,c3))+cs2*(cr0*ui(i1,i2+2,i3+2,c3)+cr1*ui(i1+
     & 1,i2+2,i3+2,c3)+cr2*ui(i1+2,i2+2,i3+2,c3)+cr3*ui(i1+3,i2+2,i3+
     & 2,c3)+cr4*ui(i1+4,i2+2,i3+2,c3)+cr5*ui(i1+5,i2+2,i3+2,c3))+cs3*
     & (cr0*ui(i1,i2+3,i3+2,c3)+cr1*ui(i1+1,i2+3,i3+2,c3)+cr2*ui(i1+2,
     & i2+3,i3+2,c3)+cr3*ui(i1+3,i2+3,i3+2,c3)+cr4*ui(i1+4,i2+3,i3+2,
     & c3)+cr5*ui(i1+5,i2+3,i3+2,c3))+cs4*(cr0*ui(i1,i2+4,i3+2,c3)+
     & cr1*ui(i1+1,i2+4,i3+2,c3)+cr2*ui(i1+2,i2+4,i3+2,c3)+cr3*ui(i1+
     & 3,i2+4,i3+2,c3)+cr4*ui(i1+4,i2+4,i3+2,c3)+cr5*ui(i1+5,i2+4,i3+
     & 2,c3))+cs5*(cr0*ui(i1,i2+5,i3+2,c3)+cr1*ui(i1+1,i2+5,i3+2,c3)+
     & cr2*ui(i1+2,i2+5,i3+2,c3)+cr3*ui(i1+3,i2+5,i3+2,c3)+cr4*ui(i1+
     & 4,i2+5,i3+2,c3)+cr5*ui(i1+5,i2+5,i3+2,c3)))+ct3*(cs0*(cr0*ui(
     & i1,i2,i3+3,c3)+cr1*ui(i1+1,i2,i3+3,c3)+cr2*ui(i1+2,i2,i3+3,c3)+
     & cr3*ui(i1+3,i2,i3+3,c3)+cr4*ui(i1+4,i2,i3+3,c3)+cr5*ui(i1+5,i2,
     & i3+3,c3))+cs1*(cr0*ui(i1,i2+1,i3+3,c3)+cr1*ui(i1+1,i2+1,i3+3,
     & c3)+cr2*ui(i1+2,i2+1,i3+3,c3)+cr3*ui(i1+3,i2+1,i3+3,c3)+cr4*ui(
     & i1+4,i2+1,i3+3,c3)+cr5*ui(i1+5,i2+1,i3+3,c3))+cs2*(cr0*ui(i1,
     & i2+2,i3+3,c3)+cr1*ui(i1+1,i2+2,i3+3,c3)+cr2*ui(i1+2,i2+2,i3+3,
     & c3)+cr3*ui(i1+3,i2+2,i3+3,c3)+cr4*ui(i1+4,i2+2,i3+3,c3)+cr5*ui(
     & i1+5,i2+2,i3+3,c3))+cs3*(cr0*ui(i1,i2+3,i3+3,c3)+cr1*ui(i1+1,
     & i2+3,i3+3,c3)+cr2*ui(i1+2,i2+3,i3+3,c3)+cr3*ui(i1+3,i2+3,i3+3,
     & c3)+cr4*ui(i1+4,i2+3,i3+3,c3)+cr5*ui(i1+5,i2+3,i3+3,c3))+cs4*(
     & cr0*ui(i1,i2+4,i3+3,c3)+cr1*ui(i1+1,i2+4,i3+3,c3)+cr2*ui(i1+2,
     & i2+4,i3+3,c3)+cr3*ui(i1+3,i2+4,i3+3,c3)+cr4*ui(i1+4,i2+4,i3+3,
     & c3)+cr5*ui(i1+5,i2+4,i3+3,c3))+cs5*(cr0*ui(i1,i2+5,i3+3,c3)+
     & cr1*ui(i1+1,i2+5,i3+3,c3)+cr2*ui(i1+2,i2+5,i3+3,c3)+cr3*ui(i1+
     & 3,i2+5,i3+3,c3)+cr4*ui(i1+4,i2+5,i3+3,c3)+cr5*ui(i1+5,i2+5,i3+
     & 3,c3)))+ct4*(cs0*(cr0*ui(i1,i2,i3+4,c3)+cr1*ui(i1+1,i2,i3+4,c3)
     & +cr2*ui(i1+2,i2,i3+4,c3)+cr3*ui(i1+3,i2,i3+4,c3)+cr4*ui(i1+4,
     & i2,i3+4,c3)+cr5*ui(i1+5,i2,i3+4,c3))+cs1*(cr0*ui(i1,i2+1,i3+4,
     & c3)+cr1*ui(i1+1,i2+1,i3+4,c3)+cr2*ui(i1+2,i2+1,i3+4,c3)+cr3*ui(
     & i1+3,i2+1,i3+4,c3)+cr4*ui(i1+4,i2+1,i3+4,c3)+cr5*ui(i1+5,i2+1,
     & i3+4,c3))+cs2*(cr0*ui(i1,i2+2,i3+4,c3)+cr1*ui(i1+1,i2+2,i3+4,
     & c3)+cr2*ui(i1+2,i2+2,i3+4,c3)+cr3*ui(i1+3,i2+2,i3+4,c3)+cr4*ui(
     & i1+4,i2+2,i3+4,c3)+cr5*ui(i1+5,i2+2,i3+4,c3))+cs3*(cr0*ui(i1,
     & i2+3,i3+4,c3)+cr1*ui(i1+1,i2+3,i3+4,c3)+cr2*ui(i1+2,i2+3,i3+4,
     & c3)+cr3*ui(i1+3,i2+3,i3+4,c3)+cr4*ui(i1+4,i2+3,i3+4,c3)+cr5*ui(
     & i1+5,i2+3,i3+4,c3))+cs4*(cr0*ui(i1,i2+4,i3+4,c3)+cr1*ui(i1+1,
     & i2+4,i3+4,c3)+cr2*ui(i1+2,i2+4,i3+4,c3)+cr3*ui(i1+3,i2+4,i3+4,
     & c3)+cr4*ui(i1+4,i2+4,i3+4,c3)+cr5*ui(i1+5,i2+4,i3+4,c3))+cs5*(
     & cr0*ui(i1,i2+5,i3+4,c3)+cr1*ui(i1+1,i2+5,i3+4,c3)+cr2*ui(i1+2,
     & i2+5,i3+4,c3)+cr3*ui(i1+3,i2+5,i3+4,c3)+cr4*ui(i1+4,i2+5,i3+4,
     & c3)+cr5*ui(i1+5,i2+5,i3+4,c3)))+ct5*(cs0*(cr0*ui(i1,i2,i3+5,c3)
     & +cr1*ui(i1+1,i2,i3+5,c3)+cr2*ui(i1+2,i2,i3+5,c3)+cr3*ui(i1+3,
     & i2,i3+5,c3)+cr4*ui(i1+4,i2,i3+5,c3)+cr5*ui(i1+5,i2,i3+5,c3))+
     & cs1*(cr0*ui(i1,i2+1,i3+5,c3)+cr1*ui(i1+1,i2+1,i3+5,c3)+cr2*ui(
     & i1+2,i2+1,i3+5,c3)+cr3*ui(i1+3,i2+1,i3+5,c3)+cr4*ui(i1+4,i2+1,
     & i3+5,c3)+cr5*ui(i1+5,i2+1,i3+5,c3))+cs2*(cr0*ui(i1,i2+2,i3+5,
     & c3)+cr1*ui(i1+1,i2+2,i3+5,c3)+cr2*ui(i1+2,i2+2,i3+5,c3)+cr3*ui(
     & i1+3,i2+2,i3+5,c3)+cr4*ui(i1+4,i2+2,i3+5,c3)+cr5*ui(i1+5,i2+2,
     & i3+5,c3))+cs3*(cr0*ui(i1,i2+3,i3+5,c3)+cr1*ui(i1+1,i2+3,i3+5,
     & c3)+cr2*ui(i1+2,i2+3,i3+5,c3)+cr3*ui(i1+3,i2+3,i3+5,c3)+cr4*ui(
     & i1+4,i2+3,i3+5,c3)+cr5*ui(i1+5,i2+3,i3+5,c3))+cs4*(cr0*ui(i1,
     & i2+4,i3+5,c3)+cr1*ui(i1+1,i2+4,i3+5,c3)+cr2*ui(i1+2,i2+4,i3+5,
     & c3)+cr3*ui(i1+3,i2+4,i3+5,c3)+cr4*ui(i1+4,i2+4,i3+5,c3)+cr5*ui(
     & i1+5,i2+4,i3+5,c3))+cs5*(cr0*ui(i1,i2+5,i3+5,c3)+cr1*ui(i1+1,
     & i2+5,i3+5,c3)+cr2*ui(i1+2,i2+5,i3+5,c3)+cr3*ui(i1+3,i2+5,i3+5,
     & c3)+cr4*ui(i1+4,i2+5,i3+5,c3)+cr5*ui(i1+5,i2+5,i3+5,c3)))
             else if( varWidth(i).eq.9 )then
! interpSparseStorage999(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q90(c(i,0,0,0))
                 cs0 = q90(c(i,1,0,0))
                 ct0 = q90(c(i,2,0,0))
                 cr1 = q91(c(i,0,0,0))
                 cs1 = q91(c(i,1,0,0))
                 ct1 = q91(c(i,2,0,0))
                 cr2 = q92(c(i,0,0,0))
                 cs2 = q92(c(i,1,0,0))
                 ct2 = q92(c(i,2,0,0))
                 cr3 = q93(c(i,0,0,0))
                 cs3 = q93(c(i,1,0,0))
                 ct3 = q93(c(i,2,0,0))
                 cr4 = q94(c(i,0,0,0))
                 cs4 = q94(c(i,1,0,0))
                 ct4 = q94(c(i,2,0,0))
                 cr5 = q95(c(i,0,0,0))
                 cs5 = q95(c(i,1,0,0))
                 ct5 = q95(c(i,2,0,0))
                 cr6 = q96(c(i,0,0,0))
                 cs6 = q96(c(i,1,0,0))
                 ct6 = q96(c(i,2,0,0))
                 cr7 = q97(c(i,0,0,0))
                 cs7 = q97(c(i,1,0,0))
                 ct7 = q97(c(i,2,0,0))
                 cr8 = q98(c(i,0,0,0))
                 cs8 = q98(c(i,1,0,0))
                 ct8 = q98(c(i,2,0,0))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ct0*(cs0*(cr0*ui(i1,i2,
     & i3+0,c3)+cr1*ui(i1+1,i2,i3+0,c3)+cr2*ui(i1+2,i2,i3+0,c3)+cr3*
     & ui(i1+3,i2,i3+0,c3)+cr4*ui(i1+4,i2,i3+0,c3)+cr5*ui(i1+5,i2,i3+
     & 0,c3)+cr6*ui(i1+6,i2,i3+0,c3)+cr7*ui(i1+7,i2,i3+0,c3)+cr8*ui(
     & i1+8,i2,i3+0,c3))+cs1*(cr0*ui(i1,i2+1,i3+0,c3)+cr1*ui(i1+1,i2+
     & 1,i3+0,c3)+cr2*ui(i1+2,i2+1,i3+0,c3)+cr3*ui(i1+3,i2+1,i3+0,c3)+
     & cr4*ui(i1+4,i2+1,i3+0,c3)+cr5*ui(i1+5,i2+1,i3+0,c3)+cr6*ui(i1+
     & 6,i2+1,i3+0,c3)+cr7*ui(i1+7,i2+1,i3+0,c3)+cr8*ui(i1+8,i2+1,i3+
     & 0,c3))+cs2*(cr0*ui(i1,i2+2,i3+0,c3)+cr1*ui(i1+1,i2+2,i3+0,c3)+
     & cr2*ui(i1+2,i2+2,i3+0,c3)+cr3*ui(i1+3,i2+2,i3+0,c3)+cr4*ui(i1+
     & 4,i2+2,i3+0,c3)+cr5*ui(i1+5,i2+2,i3+0,c3)+cr6*ui(i1+6,i2+2,i3+
     & 0,c3)+cr7*ui(i1+7,i2+2,i3+0,c3)+cr8*ui(i1+8,i2+2,i3+0,c3))+cs3*
     & (cr0*ui(i1,i2+3,i3+0,c3)+cr1*ui(i1+1,i2+3,i3+0,c3)+cr2*ui(i1+2,
     & i2+3,i3+0,c3)+cr3*ui(i1+3,i2+3,i3+0,c3)+cr4*ui(i1+4,i2+3,i3+0,
     & c3)+cr5*ui(i1+5,i2+3,i3+0,c3)+cr6*ui(i1+6,i2+3,i3+0,c3)+cr7*ui(
     & i1+7,i2+3,i3+0,c3)+cr8*ui(i1+8,i2+3,i3+0,c3))+cs4*(cr0*ui(i1,
     & i2+4,i3+0,c3)+cr1*ui(i1+1,i2+4,i3+0,c3)+cr2*ui(i1+2,i2+4,i3+0,
     & c3)+cr3*ui(i1+3,i2+4,i3+0,c3)+cr4*ui(i1+4,i2+4,i3+0,c3)+cr5*ui(
     & i1+5,i2+4,i3+0,c3)+cr6*ui(i1+6,i2+4,i3+0,c3)+cr7*ui(i1+7,i2+4,
     & i3+0,c3)+cr8*ui(i1+8,i2+4,i3+0,c3))+cs5*(cr0*ui(i1,i2+5,i3+0,
     & c3)+cr1*ui(i1+1,i2+5,i3+0,c3)+cr2*ui(i1+2,i2+5,i3+0,c3)+cr3*ui(
     & i1+3,i2+5,i3+0,c3)+cr4*ui(i1+4,i2+5,i3+0,c3)+cr5*ui(i1+5,i2+5,
     & i3+0,c3)+cr6*ui(i1+6,i2+5,i3+0,c3)+cr7*ui(i1+7,i2+5,i3+0,c3)+
     & cr8*ui(i1+8,i2+5,i3+0,c3))+cs6*(cr0*ui(i1,i2+6,i3+0,c3)+cr1*ui(
     & i1+1,i2+6,i3+0,c3)+cr2*ui(i1+2,i2+6,i3+0,c3)+cr3*ui(i1+3,i2+6,
     & i3+0,c3)+cr4*ui(i1+4,i2+6,i3+0,c3)+cr5*ui(i1+5,i2+6,i3+0,c3)+
     & cr6*ui(i1+6,i2+6,i3+0,c3)+cr7*ui(i1+7,i2+6,i3+0,c3)+cr8*ui(i1+
     & 8,i2+6,i3+0,c3))+cs7*(cr0*ui(i1,i2+7,i3+0,c3)+cr1*ui(i1+1,i2+7,
     & i3+0,c3)+cr2*ui(i1+2,i2+7,i3+0,c3)+cr3*ui(i1+3,i2+7,i3+0,c3)+
     & cr4*ui(i1+4,i2+7,i3+0,c3)+cr5*ui(i1+5,i2+7,i3+0,c3)+cr6*ui(i1+
     & 6,i2+7,i3+0,c3)+cr7*ui(i1+7,i2+7,i3+0,c3)+cr8*ui(i1+8,i2+7,i3+
     & 0,c3))+cs8*(cr0*ui(i1,i2+8,i3+0,c3)+cr1*ui(i1+1,i2+8,i3+0,c3)+
     & cr2*ui(i1+2,i2+8,i3+0,c3)+cr3*ui(i1+3,i2+8,i3+0,c3)+cr4*ui(i1+
     & 4,i2+8,i3+0,c3)+cr5*ui(i1+5,i2+8,i3+0,c3)+cr6*ui(i1+6,i2+8,i3+
     & 0,c3)+cr7*ui(i1+7,i2+8,i3+0,c3)+cr8*ui(i1+8,i2+8,i3+0,c3)))+
     & ct1*(cs0*(cr0*ui(i1,i2,i3+1,c3)+cr1*ui(i1+1,i2,i3+1,c3)+cr2*ui(
     & i1+2,i2,i3+1,c3)+cr3*ui(i1+3,i2,i3+1,c3)+cr4*ui(i1+4,i2,i3+1,
     & c3)+cr5*ui(i1+5,i2,i3+1,c3)+cr6*ui(i1+6,i2,i3+1,c3)+cr7*ui(i1+
     & 7,i2,i3+1,c3)+cr8*ui(i1+8,i2,i3+1,c3))+cs1*(cr0*ui(i1,i2+1,i3+
     & 1,c3)+cr1*ui(i1+1,i2+1,i3+1,c3)+cr2*ui(i1+2,i2+1,i3+1,c3)+cr3*
     & ui(i1+3,i2+1,i3+1,c3)+cr4*ui(i1+4,i2+1,i3+1,c3)+cr5*ui(i1+5,i2+
     & 1,i3+1,c3)+cr6*ui(i1+6,i2+1,i3+1,c3)+cr7*ui(i1+7,i2+1,i3+1,c3)+
     & cr8*ui(i1+8,i2+1,i3+1,c3))+cs2*(cr0*ui(i1,i2+2,i3+1,c3)+cr1*ui(
     & i1+1,i2+2,i3+1,c3)+cr2*ui(i1+2,i2+2,i3+1,c3)+cr3*ui(i1+3,i2+2,
     & i3+1,c3)+cr4*ui(i1+4,i2+2,i3+1,c3)+cr5*ui(i1+5,i2+2,i3+1,c3)+
     & cr6*ui(i1+6,i2+2,i3+1,c3)+cr7*ui(i1+7,i2+2,i3+1,c3)+cr8*ui(i1+
     & 8,i2+2,i3+1,c3))+cs3*(cr0*ui(i1,i2+3,i3+1,c3)+cr1*ui(i1+1,i2+3,
     & i3+1,c3)+cr2*ui(i1+2,i2+3,i3+1,c3)+cr3*ui(i1+3,i2+3,i3+1,c3)+
     & cr4*ui(i1+4,i2+3,i3+1,c3)+cr5*ui(i1+5,i2+3,i3+1,c3)+cr6*ui(i1+
     & 6,i2+3,i3+1,c3)+cr7*ui(i1+7,i2+3,i3+1,c3)+cr8*ui(i1+8,i2+3,i3+
     & 1,c3))+cs4*(cr0*ui(i1,i2+4,i3+1,c3)+cr1*ui(i1+1,i2+4,i3+1,c3)+
     & cr2*ui(i1+2,i2+4,i3+1,c3)+cr3*ui(i1+3,i2+4,i3+1,c3)+cr4*ui(i1+
     & 4,i2+4,i3+1,c3)+cr5*ui(i1+5,i2+4,i3+1,c3)+cr6*ui(i1+6,i2+4,i3+
     & 1,c3)+cr7*ui(i1+7,i2+4,i3+1,c3)+cr8*ui(i1+8,i2+4,i3+1,c3))+cs5*
     & (cr0*ui(i1,i2+5,i3+1,c3)+cr1*ui(i1+1,i2+5,i3+1,c3)+cr2*ui(i1+2,
     & i2+5,i3+1,c3)+cr3*ui(i1+3,i2+5,i3+1,c3)+cr4*ui(i1+4,i2+5,i3+1,
     & c3)+cr5*ui(i1+5,i2+5,i3+1,c3)+cr6*ui(i1+6,i2+5,i3+1,c3)+cr7*ui(
     & i1+7,i2+5,i3+1,c3)+cr8*ui(i1+8,i2+5,i3+1,c3))+cs6*(cr0*ui(i1,
     & i2+6,i3+1,c3)+cr1*ui(i1+1,i2+6,i3+1,c3)+cr2*ui(i1+2,i2+6,i3+1,
     & c3)+cr3*ui(i1+3,i2+6,i3+1,c3)+cr4*ui(i1+4,i2+6,i3+1,c3)+cr5*ui(
     & i1+5,i2+6,i3+1,c3)+cr6*ui(i1+6,i2+6,i3+1,c3)+cr7*ui(i1+7,i2+6,
     & i3+1,c3)+cr8*ui(i1+8,i2+6,i3+1,c3))+cs7*(cr0*ui(i1,i2+7,i3+1,
     & c3)+cr1*ui(i1+1,i2+7,i3+1,c3)+cr2*ui(i1+2,i2+7,i3+1,c3)+cr3*ui(
     & i1+3,i2+7,i3+1,c3)+cr4*ui(i1+4,i2+7,i3+1,c3)+cr5*ui(i1+5,i2+7,
     & i3+1,c3)+cr6*ui(i1+6,i2+7,i3+1,c3)+cr7*ui(i1+7,i2+7,i3+1,c3)+
     & cr8*ui(i1+8,i2+7,i3+1,c3))+cs8*(cr0*ui(i1,i2+8,i3+1,c3)+cr1*ui(
     & i1+1,i2+8,i3+1,c3)+cr2*ui(i1+2,i2+8,i3+1,c3)+cr3*ui(i1+3,i2+8,
     & i3+1,c3)+cr4*ui(i1+4,i2+8,i3+1,c3)+cr5*ui(i1+5,i2+8,i3+1,c3)+
     & cr6*ui(i1+6,i2+8,i3+1,c3)+cr7*ui(i1+7,i2+8,i3+1,c3)+cr8*ui(i1+
     & 8,i2+8,i3+1,c3)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+ct2*(cs0*(cr0*ui(i1,i2,i3+2,c3)+cr1*ui(i1+1,i2,i3+2,
     & c3)+cr2*ui(i1+2,i2,i3+2,c3)+cr3*ui(i1+3,i2,i3+2,c3)+cr4*ui(i1+
     & 4,i2,i3+2,c3)+cr5*ui(i1+5,i2,i3+2,c3)+cr6*ui(i1+6,i2,i3+2,c3)+
     & cr7*ui(i1+7,i2,i3+2,c3)+cr8*ui(i1+8,i2,i3+2,c3))+cs1*(cr0*ui(
     & i1,i2+1,i3+2,c3)+cr1*ui(i1+1,i2+1,i3+2,c3)+cr2*ui(i1+2,i2+1,i3+
     & 2,c3)+cr3*ui(i1+3,i2+1,i3+2,c3)+cr4*ui(i1+4,i2+1,i3+2,c3)+cr5*
     & ui(i1+5,i2+1,i3+2,c3)+cr6*ui(i1+6,i2+1,i3+2,c3)+cr7*ui(i1+7,i2+
     & 1,i3+2,c3)+cr8*ui(i1+8,i2+1,i3+2,c3))+cs2*(cr0*ui(i1,i2+2,i3+2,
     & c3)+cr1*ui(i1+1,i2+2,i3+2,c3)+cr2*ui(i1+2,i2+2,i3+2,c3)+cr3*ui(
     & i1+3,i2+2,i3+2,c3)+cr4*ui(i1+4,i2+2,i3+2,c3)+cr5*ui(i1+5,i2+2,
     & i3+2,c3)+cr6*ui(i1+6,i2+2,i3+2,c3)+cr7*ui(i1+7,i2+2,i3+2,c3)+
     & cr8*ui(i1+8,i2+2,i3+2,c3))+cs3*(cr0*ui(i1,i2+3,i3+2,c3)+cr1*ui(
     & i1+1,i2+3,i3+2,c3)+cr2*ui(i1+2,i2+3,i3+2,c3)+cr3*ui(i1+3,i2+3,
     & i3+2,c3)+cr4*ui(i1+4,i2+3,i3+2,c3)+cr5*ui(i1+5,i2+3,i3+2,c3)+
     & cr6*ui(i1+6,i2+3,i3+2,c3)+cr7*ui(i1+7,i2+3,i3+2,c3)+cr8*ui(i1+
     & 8,i2+3,i3+2,c3))+cs4*(cr0*ui(i1,i2+4,i3+2,c3)+cr1*ui(i1+1,i2+4,
     & i3+2,c3)+cr2*ui(i1+2,i2+4,i3+2,c3)+cr3*ui(i1+3,i2+4,i3+2,c3)+
     & cr4*ui(i1+4,i2+4,i3+2,c3)+cr5*ui(i1+5,i2+4,i3+2,c3)+cr6*ui(i1+
     & 6,i2+4,i3+2,c3)+cr7*ui(i1+7,i2+4,i3+2,c3)+cr8*ui(i1+8,i2+4,i3+
     & 2,c3))+cs5*(cr0*ui(i1,i2+5,i3+2,c3)+cr1*ui(i1+1,i2+5,i3+2,c3)+
     & cr2*ui(i1+2,i2+5,i3+2,c3)+cr3*ui(i1+3,i2+5,i3+2,c3)+cr4*ui(i1+
     & 4,i2+5,i3+2,c3)+cr5*ui(i1+5,i2+5,i3+2,c3)+cr6*ui(i1+6,i2+5,i3+
     & 2,c3)+cr7*ui(i1+7,i2+5,i3+2,c3)+cr8*ui(i1+8,i2+5,i3+2,c3))+cs6*
     & (cr0*ui(i1,i2+6,i3+2,c3)+cr1*ui(i1+1,i2+6,i3+2,c3)+cr2*ui(i1+2,
     & i2+6,i3+2,c3)+cr3*ui(i1+3,i2+6,i3+2,c3)+cr4*ui(i1+4,i2+6,i3+2,
     & c3)+cr5*ui(i1+5,i2+6,i3+2,c3)+cr6*ui(i1+6,i2+6,i3+2,c3)+cr7*ui(
     & i1+7,i2+6,i3+2,c3)+cr8*ui(i1+8,i2+6,i3+2,c3))+cs7*(cr0*ui(i1,
     & i2+7,i3+2,c3)+cr1*ui(i1+1,i2+7,i3+2,c3)+cr2*ui(i1+2,i2+7,i3+2,
     & c3)+cr3*ui(i1+3,i2+7,i3+2,c3)+cr4*ui(i1+4,i2+7,i3+2,c3)+cr5*ui(
     & i1+5,i2+7,i3+2,c3)+cr6*ui(i1+6,i2+7,i3+2,c3)+cr7*ui(i1+7,i2+7,
     & i3+2,c3)+cr8*ui(i1+8,i2+7,i3+2,c3))+cs8*(cr0*ui(i1,i2+8,i3+2,
     & c3)+cr1*ui(i1+1,i2+8,i3+2,c3)+cr2*ui(i1+2,i2+8,i3+2,c3)+cr3*ui(
     & i1+3,i2+8,i3+2,c3)+cr4*ui(i1+4,i2+8,i3+2,c3)+cr5*ui(i1+5,i2+8,
     & i3+2,c3)+cr6*ui(i1+6,i2+8,i3+2,c3)+cr7*ui(i1+7,i2+8,i3+2,c3)+
     & cr8*ui(i1+8,i2+8,i3+2,c3)))+ct3*(cs0*(cr0*ui(i1,i2,i3+3,c3)+
     & cr1*ui(i1+1,i2,i3+3,c3)+cr2*ui(i1+2,i2,i3+3,c3)+cr3*ui(i1+3,i2,
     & i3+3,c3)+cr4*ui(i1+4,i2,i3+3,c3)+cr5*ui(i1+5,i2,i3+3,c3)+cr6*
     & ui(i1+6,i2,i3+3,c3)+cr7*ui(i1+7,i2,i3+3,c3)+cr8*ui(i1+8,i2,i3+
     & 3,c3))+cs1*(cr0*ui(i1,i2+1,i3+3,c3)+cr1*ui(i1+1,i2+1,i3+3,c3)+
     & cr2*ui(i1+2,i2+1,i3+3,c3)+cr3*ui(i1+3,i2+1,i3+3,c3)+cr4*ui(i1+
     & 4,i2+1,i3+3,c3)+cr5*ui(i1+5,i2+1,i3+3,c3)+cr6*ui(i1+6,i2+1,i3+
     & 3,c3)+cr7*ui(i1+7,i2+1,i3+3,c3)+cr8*ui(i1+8,i2+1,i3+3,c3))+cs2*
     & (cr0*ui(i1,i2+2,i3+3,c3)+cr1*ui(i1+1,i2+2,i3+3,c3)+cr2*ui(i1+2,
     & i2+2,i3+3,c3)+cr3*ui(i1+3,i2+2,i3+3,c3)+cr4*ui(i1+4,i2+2,i3+3,
     & c3)+cr5*ui(i1+5,i2+2,i3+3,c3)+cr6*ui(i1+6,i2+2,i3+3,c3)+cr7*ui(
     & i1+7,i2+2,i3+3,c3)+cr8*ui(i1+8,i2+2,i3+3,c3))+cs3*(cr0*ui(i1,
     & i2+3,i3+3,c3)+cr1*ui(i1+1,i2+3,i3+3,c3)+cr2*ui(i1+2,i2+3,i3+3,
     & c3)+cr3*ui(i1+3,i2+3,i3+3,c3)+cr4*ui(i1+4,i2+3,i3+3,c3)+cr5*ui(
     & i1+5,i2+3,i3+3,c3)+cr6*ui(i1+6,i2+3,i3+3,c3)+cr7*ui(i1+7,i2+3,
     & i3+3,c3)+cr8*ui(i1+8,i2+3,i3+3,c3))+cs4*(cr0*ui(i1,i2+4,i3+3,
     & c3)+cr1*ui(i1+1,i2+4,i3+3,c3)+cr2*ui(i1+2,i2+4,i3+3,c3)+cr3*ui(
     & i1+3,i2+4,i3+3,c3)+cr4*ui(i1+4,i2+4,i3+3,c3)+cr5*ui(i1+5,i2+4,
     & i3+3,c3)+cr6*ui(i1+6,i2+4,i3+3,c3)+cr7*ui(i1+7,i2+4,i3+3,c3)+
     & cr8*ui(i1+8,i2+4,i3+3,c3))+cs5*(cr0*ui(i1,i2+5,i3+3,c3)+cr1*ui(
     & i1+1,i2+5,i3+3,c3)+cr2*ui(i1+2,i2+5,i3+3,c3)+cr3*ui(i1+3,i2+5,
     & i3+3,c3)+cr4*ui(i1+4,i2+5,i3+3,c3)+cr5*ui(i1+5,i2+5,i3+3,c3)+
     & cr6*ui(i1+6,i2+5,i3+3,c3)+cr7*ui(i1+7,i2+5,i3+3,c3)+cr8*ui(i1+
     & 8,i2+5,i3+3,c3))+cs6*(cr0*ui(i1,i2+6,i3+3,c3)+cr1*ui(i1+1,i2+6,
     & i3+3,c3)+cr2*ui(i1+2,i2+6,i3+3,c3)+cr3*ui(i1+3,i2+6,i3+3,c3)+
     & cr4*ui(i1+4,i2+6,i3+3,c3)+cr5*ui(i1+5,i2+6,i3+3,c3)+cr6*ui(i1+
     & 6,i2+6,i3+3,c3)+cr7*ui(i1+7,i2+6,i3+3,c3)+cr8*ui(i1+8,i2+6,i3+
     & 3,c3))+cs7*(cr0*ui(i1,i2+7,i3+3,c3)+cr1*ui(i1+1,i2+7,i3+3,c3)+
     & cr2*ui(i1+2,i2+7,i3+3,c3)+cr3*ui(i1+3,i2+7,i3+3,c3)+cr4*ui(i1+
     & 4,i2+7,i3+3,c3)+cr5*ui(i1+5,i2+7,i3+3,c3)+cr6*ui(i1+6,i2+7,i3+
     & 3,c3)+cr7*ui(i1+7,i2+7,i3+3,c3)+cr8*ui(i1+8,i2+7,i3+3,c3))+cs8*
     & (cr0*ui(i1,i2+8,i3+3,c3)+cr1*ui(i1+1,i2+8,i3+3,c3)+cr2*ui(i1+2,
     & i2+8,i3+3,c3)+cr3*ui(i1+3,i2+8,i3+3,c3)+cr4*ui(i1+4,i2+8,i3+3,
     & c3)+cr5*ui(i1+5,i2+8,i3+3,c3)+cr6*ui(i1+6,i2+8,i3+3,c3)+cr7*ui(
     & i1+7,i2+8,i3+3,c3)+cr8*ui(i1+8,i2+8,i3+3,c3)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+ct4*(cs0*(cr0*ui(i1,i2,i3+4,c3)+cr1*ui(i1+1,i2,i3+4,
     & c3)+cr2*ui(i1+2,i2,i3+4,c3)+cr3*ui(i1+3,i2,i3+4,c3)+cr4*ui(i1+
     & 4,i2,i3+4,c3)+cr5*ui(i1+5,i2,i3+4,c3)+cr6*ui(i1+6,i2,i3+4,c3)+
     & cr7*ui(i1+7,i2,i3+4,c3)+cr8*ui(i1+8,i2,i3+4,c3))+cs1*(cr0*ui(
     & i1,i2+1,i3+4,c3)+cr1*ui(i1+1,i2+1,i3+4,c3)+cr2*ui(i1+2,i2+1,i3+
     & 4,c3)+cr3*ui(i1+3,i2+1,i3+4,c3)+cr4*ui(i1+4,i2+1,i3+4,c3)+cr5*
     & ui(i1+5,i2+1,i3+4,c3)+cr6*ui(i1+6,i2+1,i3+4,c3)+cr7*ui(i1+7,i2+
     & 1,i3+4,c3)+cr8*ui(i1+8,i2+1,i3+4,c3))+cs2*(cr0*ui(i1,i2+2,i3+4,
     & c3)+cr1*ui(i1+1,i2+2,i3+4,c3)+cr2*ui(i1+2,i2+2,i3+4,c3)+cr3*ui(
     & i1+3,i2+2,i3+4,c3)+cr4*ui(i1+4,i2+2,i3+4,c3)+cr5*ui(i1+5,i2+2,
     & i3+4,c3)+cr6*ui(i1+6,i2+2,i3+4,c3)+cr7*ui(i1+7,i2+2,i3+4,c3)+
     & cr8*ui(i1+8,i2+2,i3+4,c3))+cs3*(cr0*ui(i1,i2+3,i3+4,c3)+cr1*ui(
     & i1+1,i2+3,i3+4,c3)+cr2*ui(i1+2,i2+3,i3+4,c3)+cr3*ui(i1+3,i2+3,
     & i3+4,c3)+cr4*ui(i1+4,i2+3,i3+4,c3)+cr5*ui(i1+5,i2+3,i3+4,c3)+
     & cr6*ui(i1+6,i2+3,i3+4,c3)+cr7*ui(i1+7,i2+3,i3+4,c3)+cr8*ui(i1+
     & 8,i2+3,i3+4,c3))+cs4*(cr0*ui(i1,i2+4,i3+4,c3)+cr1*ui(i1+1,i2+4,
     & i3+4,c3)+cr2*ui(i1+2,i2+4,i3+4,c3)+cr3*ui(i1+3,i2+4,i3+4,c3)+
     & cr4*ui(i1+4,i2+4,i3+4,c3)+cr5*ui(i1+5,i2+4,i3+4,c3)+cr6*ui(i1+
     & 6,i2+4,i3+4,c3)+cr7*ui(i1+7,i2+4,i3+4,c3)+cr8*ui(i1+8,i2+4,i3+
     & 4,c3))+cs5*(cr0*ui(i1,i2+5,i3+4,c3)+cr1*ui(i1+1,i2+5,i3+4,c3)+
     & cr2*ui(i1+2,i2+5,i3+4,c3)+cr3*ui(i1+3,i2+5,i3+4,c3)+cr4*ui(i1+
     & 4,i2+5,i3+4,c3)+cr5*ui(i1+5,i2+5,i3+4,c3)+cr6*ui(i1+6,i2+5,i3+
     & 4,c3)+cr7*ui(i1+7,i2+5,i3+4,c3)+cr8*ui(i1+8,i2+5,i3+4,c3))+cs6*
     & (cr0*ui(i1,i2+6,i3+4,c3)+cr1*ui(i1+1,i2+6,i3+4,c3)+cr2*ui(i1+2,
     & i2+6,i3+4,c3)+cr3*ui(i1+3,i2+6,i3+4,c3)+cr4*ui(i1+4,i2+6,i3+4,
     & c3)+cr5*ui(i1+5,i2+6,i3+4,c3)+cr6*ui(i1+6,i2+6,i3+4,c3)+cr7*ui(
     & i1+7,i2+6,i3+4,c3)+cr8*ui(i1+8,i2+6,i3+4,c3))+cs7*(cr0*ui(i1,
     & i2+7,i3+4,c3)+cr1*ui(i1+1,i2+7,i3+4,c3)+cr2*ui(i1+2,i2+7,i3+4,
     & c3)+cr3*ui(i1+3,i2+7,i3+4,c3)+cr4*ui(i1+4,i2+7,i3+4,c3)+cr5*ui(
     & i1+5,i2+7,i3+4,c3)+cr6*ui(i1+6,i2+7,i3+4,c3)+cr7*ui(i1+7,i2+7,
     & i3+4,c3)+cr8*ui(i1+8,i2+7,i3+4,c3))+cs8*(cr0*ui(i1,i2+8,i3+4,
     & c3)+cr1*ui(i1+1,i2+8,i3+4,c3)+cr2*ui(i1+2,i2+8,i3+4,c3)+cr3*ui(
     & i1+3,i2+8,i3+4,c3)+cr4*ui(i1+4,i2+8,i3+4,c3)+cr5*ui(i1+5,i2+8,
     & i3+4,c3)+cr6*ui(i1+6,i2+8,i3+4,c3)+cr7*ui(i1+7,i2+8,i3+4,c3)+
     & cr8*ui(i1+8,i2+8,i3+4,c3)))+ct5*(cs0*(cr0*ui(i1,i2,i3+5,c3)+
     & cr1*ui(i1+1,i2,i3+5,c3)+cr2*ui(i1+2,i2,i3+5,c3)+cr3*ui(i1+3,i2,
     & i3+5,c3)+cr4*ui(i1+4,i2,i3+5,c3)+cr5*ui(i1+5,i2,i3+5,c3)+cr6*
     & ui(i1+6,i2,i3+5,c3)+cr7*ui(i1+7,i2,i3+5,c3)+cr8*ui(i1+8,i2,i3+
     & 5,c3))+cs1*(cr0*ui(i1,i2+1,i3+5,c3)+cr1*ui(i1+1,i2+1,i3+5,c3)+
     & cr2*ui(i1+2,i2+1,i3+5,c3)+cr3*ui(i1+3,i2+1,i3+5,c3)+cr4*ui(i1+
     & 4,i2+1,i3+5,c3)+cr5*ui(i1+5,i2+1,i3+5,c3)+cr6*ui(i1+6,i2+1,i3+
     & 5,c3)+cr7*ui(i1+7,i2+1,i3+5,c3)+cr8*ui(i1+8,i2+1,i3+5,c3))+cs2*
     & (cr0*ui(i1,i2+2,i3+5,c3)+cr1*ui(i1+1,i2+2,i3+5,c3)+cr2*ui(i1+2,
     & i2+2,i3+5,c3)+cr3*ui(i1+3,i2+2,i3+5,c3)+cr4*ui(i1+4,i2+2,i3+5,
     & c3)+cr5*ui(i1+5,i2+2,i3+5,c3)+cr6*ui(i1+6,i2+2,i3+5,c3)+cr7*ui(
     & i1+7,i2+2,i3+5,c3)+cr8*ui(i1+8,i2+2,i3+5,c3))+cs3*(cr0*ui(i1,
     & i2+3,i3+5,c3)+cr1*ui(i1+1,i2+3,i3+5,c3)+cr2*ui(i1+2,i2+3,i3+5,
     & c3)+cr3*ui(i1+3,i2+3,i3+5,c3)+cr4*ui(i1+4,i2+3,i3+5,c3)+cr5*ui(
     & i1+5,i2+3,i3+5,c3)+cr6*ui(i1+6,i2+3,i3+5,c3)+cr7*ui(i1+7,i2+3,
     & i3+5,c3)+cr8*ui(i1+8,i2+3,i3+5,c3))+cs4*(cr0*ui(i1,i2+4,i3+5,
     & c3)+cr1*ui(i1+1,i2+4,i3+5,c3)+cr2*ui(i1+2,i2+4,i3+5,c3)+cr3*ui(
     & i1+3,i2+4,i3+5,c3)+cr4*ui(i1+4,i2+4,i3+5,c3)+cr5*ui(i1+5,i2+4,
     & i3+5,c3)+cr6*ui(i1+6,i2+4,i3+5,c3)+cr7*ui(i1+7,i2+4,i3+5,c3)+
     & cr8*ui(i1+8,i2+4,i3+5,c3))+cs5*(cr0*ui(i1,i2+5,i3+5,c3)+cr1*ui(
     & i1+1,i2+5,i3+5,c3)+cr2*ui(i1+2,i2+5,i3+5,c3)+cr3*ui(i1+3,i2+5,
     & i3+5,c3)+cr4*ui(i1+4,i2+5,i3+5,c3)+cr5*ui(i1+5,i2+5,i3+5,c3)+
     & cr6*ui(i1+6,i2+5,i3+5,c3)+cr7*ui(i1+7,i2+5,i3+5,c3)+cr8*ui(i1+
     & 8,i2+5,i3+5,c3))+cs6*(cr0*ui(i1,i2+6,i3+5,c3)+cr1*ui(i1+1,i2+6,
     & i3+5,c3)+cr2*ui(i1+2,i2+6,i3+5,c3)+cr3*ui(i1+3,i2+6,i3+5,c3)+
     & cr4*ui(i1+4,i2+6,i3+5,c3)+cr5*ui(i1+5,i2+6,i3+5,c3)+cr6*ui(i1+
     & 6,i2+6,i3+5,c3)+cr7*ui(i1+7,i2+6,i3+5,c3)+cr8*ui(i1+8,i2+6,i3+
     & 5,c3))+cs7*(cr0*ui(i1,i2+7,i3+5,c3)+cr1*ui(i1+1,i2+7,i3+5,c3)+
     & cr2*ui(i1+2,i2+7,i3+5,c3)+cr3*ui(i1+3,i2+7,i3+5,c3)+cr4*ui(i1+
     & 4,i2+7,i3+5,c3)+cr5*ui(i1+5,i2+7,i3+5,c3)+cr6*ui(i1+6,i2+7,i3+
     & 5,c3)+cr7*ui(i1+7,i2+7,i3+5,c3)+cr8*ui(i1+8,i2+7,i3+5,c3))+cs8*
     & (cr0*ui(i1,i2+8,i3+5,c3)+cr1*ui(i1+1,i2+8,i3+5,c3)+cr2*ui(i1+2,
     & i2+8,i3+5,c3)+cr3*ui(i1+3,i2+8,i3+5,c3)+cr4*ui(i1+4,i2+8,i3+5,
     & c3)+cr5*ui(i1+5,i2+8,i3+5,c3)+cr6*ui(i1+6,i2+8,i3+5,c3)+cr7*ui(
     & i1+7,i2+8,i3+5,c3)+cr8*ui(i1+8,i2+8,i3+5,c3)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+ct6*(cs0*(cr0*ui(i1,i2,i3+6,c3)+cr1*ui(i1+1,i2,i3+6,
     & c3)+cr2*ui(i1+2,i2,i3+6,c3)+cr3*ui(i1+3,i2,i3+6,c3)+cr4*ui(i1+
     & 4,i2,i3+6,c3)+cr5*ui(i1+5,i2,i3+6,c3)+cr6*ui(i1+6,i2,i3+6,c3)+
     & cr7*ui(i1+7,i2,i3+6,c3)+cr8*ui(i1+8,i2,i3+6,c3))+cs1*(cr0*ui(
     & i1,i2+1,i3+6,c3)+cr1*ui(i1+1,i2+1,i3+6,c3)+cr2*ui(i1+2,i2+1,i3+
     & 6,c3)+cr3*ui(i1+3,i2+1,i3+6,c3)+cr4*ui(i1+4,i2+1,i3+6,c3)+cr5*
     & ui(i1+5,i2+1,i3+6,c3)+cr6*ui(i1+6,i2+1,i3+6,c3)+cr7*ui(i1+7,i2+
     & 1,i3+6,c3)+cr8*ui(i1+8,i2+1,i3+6,c3))+cs2*(cr0*ui(i1,i2+2,i3+6,
     & c3)+cr1*ui(i1+1,i2+2,i3+6,c3)+cr2*ui(i1+2,i2+2,i3+6,c3)+cr3*ui(
     & i1+3,i2+2,i3+6,c3)+cr4*ui(i1+4,i2+2,i3+6,c3)+cr5*ui(i1+5,i2+2,
     & i3+6,c3)+cr6*ui(i1+6,i2+2,i3+6,c3)+cr7*ui(i1+7,i2+2,i3+6,c3)+
     & cr8*ui(i1+8,i2+2,i3+6,c3))+cs3*(cr0*ui(i1,i2+3,i3+6,c3)+cr1*ui(
     & i1+1,i2+3,i3+6,c3)+cr2*ui(i1+2,i2+3,i3+6,c3)+cr3*ui(i1+3,i2+3,
     & i3+6,c3)+cr4*ui(i1+4,i2+3,i3+6,c3)+cr5*ui(i1+5,i2+3,i3+6,c3)+
     & cr6*ui(i1+6,i2+3,i3+6,c3)+cr7*ui(i1+7,i2+3,i3+6,c3)+cr8*ui(i1+
     & 8,i2+3,i3+6,c3))+cs4*(cr0*ui(i1,i2+4,i3+6,c3)+cr1*ui(i1+1,i2+4,
     & i3+6,c3)+cr2*ui(i1+2,i2+4,i3+6,c3)+cr3*ui(i1+3,i2+4,i3+6,c3)+
     & cr4*ui(i1+4,i2+4,i3+6,c3)+cr5*ui(i1+5,i2+4,i3+6,c3)+cr6*ui(i1+
     & 6,i2+4,i3+6,c3)+cr7*ui(i1+7,i2+4,i3+6,c3)+cr8*ui(i1+8,i2+4,i3+
     & 6,c3))+cs5*(cr0*ui(i1,i2+5,i3+6,c3)+cr1*ui(i1+1,i2+5,i3+6,c3)+
     & cr2*ui(i1+2,i2+5,i3+6,c3)+cr3*ui(i1+3,i2+5,i3+6,c3)+cr4*ui(i1+
     & 4,i2+5,i3+6,c3)+cr5*ui(i1+5,i2+5,i3+6,c3)+cr6*ui(i1+6,i2+5,i3+
     & 6,c3)+cr7*ui(i1+7,i2+5,i3+6,c3)+cr8*ui(i1+8,i2+5,i3+6,c3))+cs6*
     & (cr0*ui(i1,i2+6,i3+6,c3)+cr1*ui(i1+1,i2+6,i3+6,c3)+cr2*ui(i1+2,
     & i2+6,i3+6,c3)+cr3*ui(i1+3,i2+6,i3+6,c3)+cr4*ui(i1+4,i2+6,i3+6,
     & c3)+cr5*ui(i1+5,i2+6,i3+6,c3)+cr6*ui(i1+6,i2+6,i3+6,c3)+cr7*ui(
     & i1+7,i2+6,i3+6,c3)+cr8*ui(i1+8,i2+6,i3+6,c3))+cs7*(cr0*ui(i1,
     & i2+7,i3+6,c3)+cr1*ui(i1+1,i2+7,i3+6,c3)+cr2*ui(i1+2,i2+7,i3+6,
     & c3)+cr3*ui(i1+3,i2+7,i3+6,c3)+cr4*ui(i1+4,i2+7,i3+6,c3)+cr5*ui(
     & i1+5,i2+7,i3+6,c3)+cr6*ui(i1+6,i2+7,i3+6,c3)+cr7*ui(i1+7,i2+7,
     & i3+6,c3)+cr8*ui(i1+8,i2+7,i3+6,c3))+cs8*(cr0*ui(i1,i2+8,i3+6,
     & c3)+cr1*ui(i1+1,i2+8,i3+6,c3)+cr2*ui(i1+2,i2+8,i3+6,c3)+cr3*ui(
     & i1+3,i2+8,i3+6,c3)+cr4*ui(i1+4,i2+8,i3+6,c3)+cr5*ui(i1+5,i2+8,
     & i3+6,c3)+cr6*ui(i1+6,i2+8,i3+6,c3)+cr7*ui(i1+7,i2+8,i3+6,c3)+
     & cr8*ui(i1+8,i2+8,i3+6,c3)))+ct7*(cs0*(cr0*ui(i1,i2,i3+7,c3)+
     & cr1*ui(i1+1,i2,i3+7,c3)+cr2*ui(i1+2,i2,i3+7,c3)+cr3*ui(i1+3,i2,
     & i3+7,c3)+cr4*ui(i1+4,i2,i3+7,c3)+cr5*ui(i1+5,i2,i3+7,c3)+cr6*
     & ui(i1+6,i2,i3+7,c3)+cr7*ui(i1+7,i2,i3+7,c3)+cr8*ui(i1+8,i2,i3+
     & 7,c3))+cs1*(cr0*ui(i1,i2+1,i3+7,c3)+cr1*ui(i1+1,i2+1,i3+7,c3)+
     & cr2*ui(i1+2,i2+1,i3+7,c3)+cr3*ui(i1+3,i2+1,i3+7,c3)+cr4*ui(i1+
     & 4,i2+1,i3+7,c3)+cr5*ui(i1+5,i2+1,i3+7,c3)+cr6*ui(i1+6,i2+1,i3+
     & 7,c3)+cr7*ui(i1+7,i2+1,i3+7,c3)+cr8*ui(i1+8,i2+1,i3+7,c3))+cs2*
     & (cr0*ui(i1,i2+2,i3+7,c3)+cr1*ui(i1+1,i2+2,i3+7,c3)+cr2*ui(i1+2,
     & i2+2,i3+7,c3)+cr3*ui(i1+3,i2+2,i3+7,c3)+cr4*ui(i1+4,i2+2,i3+7,
     & c3)+cr5*ui(i1+5,i2+2,i3+7,c3)+cr6*ui(i1+6,i2+2,i3+7,c3)+cr7*ui(
     & i1+7,i2+2,i3+7,c3)+cr8*ui(i1+8,i2+2,i3+7,c3))+cs3*(cr0*ui(i1,
     & i2+3,i3+7,c3)+cr1*ui(i1+1,i2+3,i3+7,c3)+cr2*ui(i1+2,i2+3,i3+7,
     & c3)+cr3*ui(i1+3,i2+3,i3+7,c3)+cr4*ui(i1+4,i2+3,i3+7,c3)+cr5*ui(
     & i1+5,i2+3,i3+7,c3)+cr6*ui(i1+6,i2+3,i3+7,c3)+cr7*ui(i1+7,i2+3,
     & i3+7,c3)+cr8*ui(i1+8,i2+3,i3+7,c3))+cs4*(cr0*ui(i1,i2+4,i3+7,
     & c3)+cr1*ui(i1+1,i2+4,i3+7,c3)+cr2*ui(i1+2,i2+4,i3+7,c3)+cr3*ui(
     & i1+3,i2+4,i3+7,c3)+cr4*ui(i1+4,i2+4,i3+7,c3)+cr5*ui(i1+5,i2+4,
     & i3+7,c3)+cr6*ui(i1+6,i2+4,i3+7,c3)+cr7*ui(i1+7,i2+4,i3+7,c3)+
     & cr8*ui(i1+8,i2+4,i3+7,c3))+cs5*(cr0*ui(i1,i2+5,i3+7,c3)+cr1*ui(
     & i1+1,i2+5,i3+7,c3)+cr2*ui(i1+2,i2+5,i3+7,c3)+cr3*ui(i1+3,i2+5,
     & i3+7,c3)+cr4*ui(i1+4,i2+5,i3+7,c3)+cr5*ui(i1+5,i2+5,i3+7,c3)+
     & cr6*ui(i1+6,i2+5,i3+7,c3)+cr7*ui(i1+7,i2+5,i3+7,c3)+cr8*ui(i1+
     & 8,i2+5,i3+7,c3))+cs6*(cr0*ui(i1,i2+6,i3+7,c3)+cr1*ui(i1+1,i2+6,
     & i3+7,c3)+cr2*ui(i1+2,i2+6,i3+7,c3)+cr3*ui(i1+3,i2+6,i3+7,c3)+
     & cr4*ui(i1+4,i2+6,i3+7,c3)+cr5*ui(i1+5,i2+6,i3+7,c3)+cr6*ui(i1+
     & 6,i2+6,i3+7,c3)+cr7*ui(i1+7,i2+6,i3+7,c3)+cr8*ui(i1+8,i2+6,i3+
     & 7,c3))+cs7*(cr0*ui(i1,i2+7,i3+7,c3)+cr1*ui(i1+1,i2+7,i3+7,c3)+
     & cr2*ui(i1+2,i2+7,i3+7,c3)+cr3*ui(i1+3,i2+7,i3+7,c3)+cr4*ui(i1+
     & 4,i2+7,i3+7,c3)+cr5*ui(i1+5,i2+7,i3+7,c3)+cr6*ui(i1+6,i2+7,i3+
     & 7,c3)+cr7*ui(i1+7,i2+7,i3+7,c3)+cr8*ui(i1+8,i2+7,i3+7,c3))+cs8*
     & (cr0*ui(i1,i2+8,i3+7,c3)+cr1*ui(i1+1,i2+8,i3+7,c3)+cr2*ui(i1+2,
     & i2+8,i3+7,c3)+cr3*ui(i1+3,i2+8,i3+7,c3)+cr4*ui(i1+4,i2+8,i3+7,
     & c3)+cr5*ui(i1+5,i2+8,i3+7,c3)+cr6*ui(i1+6,i2+8,i3+7,c3)+cr7*ui(
     & i1+7,i2+8,i3+7,c3)+cr8*ui(i1+8,i2+8,i3+7,c3)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+ct8*(cs0*(cr0*ui(i1,i2,i3+8,c3)+cr1*ui(i1+1,i2,i3+8,
     & c3)+cr2*ui(i1+2,i2,i3+8,c3)+cr3*ui(i1+3,i2,i3+8,c3)+cr4*ui(i1+
     & 4,i2,i3+8,c3)+cr5*ui(i1+5,i2,i3+8,c3)+cr6*ui(i1+6,i2,i3+8,c3)+
     & cr7*ui(i1+7,i2,i3+8,c3)+cr8*ui(i1+8,i2,i3+8,c3))+cs1*(cr0*ui(
     & i1,i2+1,i3+8,c3)+cr1*ui(i1+1,i2+1,i3+8,c3)+cr2*ui(i1+2,i2+1,i3+
     & 8,c3)+cr3*ui(i1+3,i2+1,i3+8,c3)+cr4*ui(i1+4,i2+1,i3+8,c3)+cr5*
     & ui(i1+5,i2+1,i3+8,c3)+cr6*ui(i1+6,i2+1,i3+8,c3)+cr7*ui(i1+7,i2+
     & 1,i3+8,c3)+cr8*ui(i1+8,i2+1,i3+8,c3))+cs2*(cr0*ui(i1,i2+2,i3+8,
     & c3)+cr1*ui(i1+1,i2+2,i3+8,c3)+cr2*ui(i1+2,i2+2,i3+8,c3)+cr3*ui(
     & i1+3,i2+2,i3+8,c3)+cr4*ui(i1+4,i2+2,i3+8,c3)+cr5*ui(i1+5,i2+2,
     & i3+8,c3)+cr6*ui(i1+6,i2+2,i3+8,c3)+cr7*ui(i1+7,i2+2,i3+8,c3)+
     & cr8*ui(i1+8,i2+2,i3+8,c3))+cs3*(cr0*ui(i1,i2+3,i3+8,c3)+cr1*ui(
     & i1+1,i2+3,i3+8,c3)+cr2*ui(i1+2,i2+3,i3+8,c3)+cr3*ui(i1+3,i2+3,
     & i3+8,c3)+cr4*ui(i1+4,i2+3,i3+8,c3)+cr5*ui(i1+5,i2+3,i3+8,c3)+
     & cr6*ui(i1+6,i2+3,i3+8,c3)+cr7*ui(i1+7,i2+3,i3+8,c3)+cr8*ui(i1+
     & 8,i2+3,i3+8,c3))+cs4*(cr0*ui(i1,i2+4,i3+8,c3)+cr1*ui(i1+1,i2+4,
     & i3+8,c3)+cr2*ui(i1+2,i2+4,i3+8,c3)+cr3*ui(i1+3,i2+4,i3+8,c3)+
     & cr4*ui(i1+4,i2+4,i3+8,c3)+cr5*ui(i1+5,i2+4,i3+8,c3)+cr6*ui(i1+
     & 6,i2+4,i3+8,c3)+cr7*ui(i1+7,i2+4,i3+8,c3)+cr8*ui(i1+8,i2+4,i3+
     & 8,c3))+cs5*(cr0*ui(i1,i2+5,i3+8,c3)+cr1*ui(i1+1,i2+5,i3+8,c3)+
     & cr2*ui(i1+2,i2+5,i3+8,c3)+cr3*ui(i1+3,i2+5,i3+8,c3)+cr4*ui(i1+
     & 4,i2+5,i3+8,c3)+cr5*ui(i1+5,i2+5,i3+8,c3)+cr6*ui(i1+6,i2+5,i3+
     & 8,c3)+cr7*ui(i1+7,i2+5,i3+8,c3)+cr8*ui(i1+8,i2+5,i3+8,c3))+cs6*
     & (cr0*ui(i1,i2+6,i3+8,c3)+cr1*ui(i1+1,i2+6,i3+8,c3)+cr2*ui(i1+2,
     & i2+6,i3+8,c3)+cr3*ui(i1+3,i2+6,i3+8,c3)+cr4*ui(i1+4,i2+6,i3+8,
     & c3)+cr5*ui(i1+5,i2+6,i3+8,c3)+cr6*ui(i1+6,i2+6,i3+8,c3)+cr7*ui(
     & i1+7,i2+6,i3+8,c3)+cr8*ui(i1+8,i2+6,i3+8,c3))+cs7*(cr0*ui(i1,
     & i2+7,i3+8,c3)+cr1*ui(i1+1,i2+7,i3+8,c3)+cr2*ui(i1+2,i2+7,i3+8,
     & c3)+cr3*ui(i1+3,i2+7,i3+8,c3)+cr4*ui(i1+4,i2+7,i3+8,c3)+cr5*ui(
     & i1+5,i2+7,i3+8,c3)+cr6*ui(i1+6,i2+7,i3+8,c3)+cr7*ui(i1+7,i2+7,
     & i3+8,c3)+cr8*ui(i1+8,i2+7,i3+8,c3))+cs8*(cr0*ui(i1,i2+8,i3+8,
     & c3)+cr1*ui(i1+1,i2+8,i3+8,c3)+cr2*ui(i1+2,i2+8,i3+8,c3)+cr3*ui(
     & i1+3,i2+8,i3+8,c3)+cr4*ui(i1+4,i2+8,i3+8,c3)+cr5*ui(i1+5,i2+8,
     & i3+8,c3)+cr6*ui(i1+6,i2+8,i3+8,c3)+cr7*ui(i1+7,i2+8,i3+8,c3)+
     & cr8*ui(i1+8,i2+8,i3+8,c3)))
             else if( varWidth(i).eq.8 )then
! interpSparseStorage888(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q80(c(i,0,0,0))
                 cs0 = q80(c(i,1,0,0))
                 ct0 = q80(c(i,2,0,0))
                 cr1 = q81(c(i,0,0,0))
                 cs1 = q81(c(i,1,0,0))
                 ct1 = q81(c(i,2,0,0))
                 cr2 = q82(c(i,0,0,0))
                 cs2 = q82(c(i,1,0,0))
                 ct2 = q82(c(i,2,0,0))
                 cr3 = q83(c(i,0,0,0))
                 cs3 = q83(c(i,1,0,0))
                 ct3 = q83(c(i,2,0,0))
                 cr4 = q84(c(i,0,0,0))
                 cs4 = q84(c(i,1,0,0))
                 ct4 = q84(c(i,2,0,0))
                 cr5 = q85(c(i,0,0,0))
                 cs5 = q85(c(i,1,0,0))
                 ct5 = q85(c(i,2,0,0))
                 cr6 = q86(c(i,0,0,0))
                 cs6 = q86(c(i,1,0,0))
                 ct6 = q86(c(i,2,0,0))
                 cr7 = q87(c(i,0,0,0))
                 cs7 = q87(c(i,1,0,0))
                 ct7 = q87(c(i,2,0,0))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ct0*(cs0*(cr0*ui(i1,i2,
     & i3+0,c3)+cr1*ui(i1+1,i2,i3+0,c3)+cr2*ui(i1+2,i2,i3+0,c3)+cr3*
     & ui(i1+3,i2,i3+0,c3)+cr4*ui(i1+4,i2,i3+0,c3)+cr5*ui(i1+5,i2,i3+
     & 0,c3)+cr6*ui(i1+6,i2,i3+0,c3)+cr7*ui(i1+7,i2,i3+0,c3))+cs1*(
     & cr0*ui(i1,i2+1,i3+0,c3)+cr1*ui(i1+1,i2+1,i3+0,c3)+cr2*ui(i1+2,
     & i2+1,i3+0,c3)+cr3*ui(i1+3,i2+1,i3+0,c3)+cr4*ui(i1+4,i2+1,i3+0,
     & c3)+cr5*ui(i1+5,i2+1,i3+0,c3)+cr6*ui(i1+6,i2+1,i3+0,c3)+cr7*ui(
     & i1+7,i2+1,i3+0,c3))+cs2*(cr0*ui(i1,i2+2,i3+0,c3)+cr1*ui(i1+1,
     & i2+2,i3+0,c3)+cr2*ui(i1+2,i2+2,i3+0,c3)+cr3*ui(i1+3,i2+2,i3+0,
     & c3)+cr4*ui(i1+4,i2+2,i3+0,c3)+cr5*ui(i1+5,i2+2,i3+0,c3)+cr6*ui(
     & i1+6,i2+2,i3+0,c3)+cr7*ui(i1+7,i2+2,i3+0,c3))+cs3*(cr0*ui(i1,
     & i2+3,i3+0,c3)+cr1*ui(i1+1,i2+3,i3+0,c3)+cr2*ui(i1+2,i2+3,i3+0,
     & c3)+cr3*ui(i1+3,i2+3,i3+0,c3)+cr4*ui(i1+4,i2+3,i3+0,c3)+cr5*ui(
     & i1+5,i2+3,i3+0,c3)+cr6*ui(i1+6,i2+3,i3+0,c3)+cr7*ui(i1+7,i2+3,
     & i3+0,c3))+cs4*(cr0*ui(i1,i2+4,i3+0,c3)+cr1*ui(i1+1,i2+4,i3+0,
     & c3)+cr2*ui(i1+2,i2+4,i3+0,c3)+cr3*ui(i1+3,i2+4,i3+0,c3)+cr4*ui(
     & i1+4,i2+4,i3+0,c3)+cr5*ui(i1+5,i2+4,i3+0,c3)+cr6*ui(i1+6,i2+4,
     & i3+0,c3)+cr7*ui(i1+7,i2+4,i3+0,c3))+cs5*(cr0*ui(i1,i2+5,i3+0,
     & c3)+cr1*ui(i1+1,i2+5,i3+0,c3)+cr2*ui(i1+2,i2+5,i3+0,c3)+cr3*ui(
     & i1+3,i2+5,i3+0,c3)+cr4*ui(i1+4,i2+5,i3+0,c3)+cr5*ui(i1+5,i2+5,
     & i3+0,c3)+cr6*ui(i1+6,i2+5,i3+0,c3)+cr7*ui(i1+7,i2+5,i3+0,c3))+
     & cs6*(cr0*ui(i1,i2+6,i3+0,c3)+cr1*ui(i1+1,i2+6,i3+0,c3)+cr2*ui(
     & i1+2,i2+6,i3+0,c3)+cr3*ui(i1+3,i2+6,i3+0,c3)+cr4*ui(i1+4,i2+6,
     & i3+0,c3)+cr5*ui(i1+5,i2+6,i3+0,c3)+cr6*ui(i1+6,i2+6,i3+0,c3)+
     & cr7*ui(i1+7,i2+6,i3+0,c3))+cs7*(cr0*ui(i1,i2+7,i3+0,c3)+cr1*ui(
     & i1+1,i2+7,i3+0,c3)+cr2*ui(i1+2,i2+7,i3+0,c3)+cr3*ui(i1+3,i2+7,
     & i3+0,c3)+cr4*ui(i1+4,i2+7,i3+0,c3)+cr5*ui(i1+5,i2+7,i3+0,c3)+
     & cr6*ui(i1+6,i2+7,i3+0,c3)+cr7*ui(i1+7,i2+7,i3+0,c3)))+ct1*(cs0*
     & (cr0*ui(i1,i2,i3+1,c3)+cr1*ui(i1+1,i2,i3+1,c3)+cr2*ui(i1+2,i2,
     & i3+1,c3)+cr3*ui(i1+3,i2,i3+1,c3)+cr4*ui(i1+4,i2,i3+1,c3)+cr5*
     & ui(i1+5,i2,i3+1,c3)+cr6*ui(i1+6,i2,i3+1,c3)+cr7*ui(i1+7,i2,i3+
     & 1,c3))+cs1*(cr0*ui(i1,i2+1,i3+1,c3)+cr1*ui(i1+1,i2+1,i3+1,c3)+
     & cr2*ui(i1+2,i2+1,i3+1,c3)+cr3*ui(i1+3,i2+1,i3+1,c3)+cr4*ui(i1+
     & 4,i2+1,i3+1,c3)+cr5*ui(i1+5,i2+1,i3+1,c3)+cr6*ui(i1+6,i2+1,i3+
     & 1,c3)+cr7*ui(i1+7,i2+1,i3+1,c3))+cs2*(cr0*ui(i1,i2+2,i3+1,c3)+
     & cr1*ui(i1+1,i2+2,i3+1,c3)+cr2*ui(i1+2,i2+2,i3+1,c3)+cr3*ui(i1+
     & 3,i2+2,i3+1,c3)+cr4*ui(i1+4,i2+2,i3+1,c3)+cr5*ui(i1+5,i2+2,i3+
     & 1,c3)+cr6*ui(i1+6,i2+2,i3+1,c3)+cr7*ui(i1+7,i2+2,i3+1,c3))+cs3*
     & (cr0*ui(i1,i2+3,i3+1,c3)+cr1*ui(i1+1,i2+3,i3+1,c3)+cr2*ui(i1+2,
     & i2+3,i3+1,c3)+cr3*ui(i1+3,i2+3,i3+1,c3)+cr4*ui(i1+4,i2+3,i3+1,
     & c3)+cr5*ui(i1+5,i2+3,i3+1,c3)+cr6*ui(i1+6,i2+3,i3+1,c3)+cr7*ui(
     & i1+7,i2+3,i3+1,c3))+cs4*(cr0*ui(i1,i2+4,i3+1,c3)+cr1*ui(i1+1,
     & i2+4,i3+1,c3)+cr2*ui(i1+2,i2+4,i3+1,c3)+cr3*ui(i1+3,i2+4,i3+1,
     & c3)+cr4*ui(i1+4,i2+4,i3+1,c3)+cr5*ui(i1+5,i2+4,i3+1,c3)+cr6*ui(
     & i1+6,i2+4,i3+1,c3)+cr7*ui(i1+7,i2+4,i3+1,c3))+cs5*(cr0*ui(i1,
     & i2+5,i3+1,c3)+cr1*ui(i1+1,i2+5,i3+1,c3)+cr2*ui(i1+2,i2+5,i3+1,
     & c3)+cr3*ui(i1+3,i2+5,i3+1,c3)+cr4*ui(i1+4,i2+5,i3+1,c3)+cr5*ui(
     & i1+5,i2+5,i3+1,c3)+cr6*ui(i1+6,i2+5,i3+1,c3)+cr7*ui(i1+7,i2+5,
     & i3+1,c3))+cs6*(cr0*ui(i1,i2+6,i3+1,c3)+cr1*ui(i1+1,i2+6,i3+1,
     & c3)+cr2*ui(i1+2,i2+6,i3+1,c3)+cr3*ui(i1+3,i2+6,i3+1,c3)+cr4*ui(
     & i1+4,i2+6,i3+1,c3)+cr5*ui(i1+5,i2+6,i3+1,c3)+cr6*ui(i1+6,i2+6,
     & i3+1,c3)+cr7*ui(i1+7,i2+6,i3+1,c3))+cs7*(cr0*ui(i1,i2+7,i3+1,
     & c3)+cr1*ui(i1+1,i2+7,i3+1,c3)+cr2*ui(i1+2,i2+7,i3+1,c3)+cr3*ui(
     & i1+3,i2+7,i3+1,c3)+cr4*ui(i1+4,i2+7,i3+1,c3)+cr5*ui(i1+5,i2+7,
     & i3+1,c3)+cr6*ui(i1+6,i2+7,i3+1,c3)+cr7*ui(i1+7,i2+7,i3+1,c3)))+
     & ct2*(cs0*(cr0*ui(i1,i2,i3+2,c3)+cr1*ui(i1+1,i2,i3+2,c3)+cr2*ui(
     & i1+2,i2,i3+2,c3)+cr3*ui(i1+3,i2,i3+2,c3)+cr4*ui(i1+4,i2,i3+2,
     & c3)+cr5*ui(i1+5,i2,i3+2,c3)+cr6*ui(i1+6,i2,i3+2,c3)+cr7*ui(i1+
     & 7,i2,i3+2,c3))+cs1*(cr0*ui(i1,i2+1,i3+2,c3)+cr1*ui(i1+1,i2+1,
     & i3+2,c3)+cr2*ui(i1+2,i2+1,i3+2,c3)+cr3*ui(i1+3,i2+1,i3+2,c3)+
     & cr4*ui(i1+4,i2+1,i3+2,c3)+cr5*ui(i1+5,i2+1,i3+2,c3)+cr6*ui(i1+
     & 6,i2+1,i3+2,c3)+cr7*ui(i1+7,i2+1,i3+2,c3))+cs2*(cr0*ui(i1,i2+2,
     & i3+2,c3)+cr1*ui(i1+1,i2+2,i3+2,c3)+cr2*ui(i1+2,i2+2,i3+2,c3)+
     & cr3*ui(i1+3,i2+2,i3+2,c3)+cr4*ui(i1+4,i2+2,i3+2,c3)+cr5*ui(i1+
     & 5,i2+2,i3+2,c3)+cr6*ui(i1+6,i2+2,i3+2,c3)+cr7*ui(i1+7,i2+2,i3+
     & 2,c3))+cs3*(cr0*ui(i1,i2+3,i3+2,c3)+cr1*ui(i1+1,i2+3,i3+2,c3)+
     & cr2*ui(i1+2,i2+3,i3+2,c3)+cr3*ui(i1+3,i2+3,i3+2,c3)+cr4*ui(i1+
     & 4,i2+3,i3+2,c3)+cr5*ui(i1+5,i2+3,i3+2,c3)+cr6*ui(i1+6,i2+3,i3+
     & 2,c3)+cr7*ui(i1+7,i2+3,i3+2,c3))+cs4*(cr0*ui(i1,i2+4,i3+2,c3)+
     & cr1*ui(i1+1,i2+4,i3+2,c3)+cr2*ui(i1+2,i2+4,i3+2,c3)+cr3*ui(i1+
     & 3,i2+4,i3+2,c3)+cr4*ui(i1+4,i2+4,i3+2,c3)+cr5*ui(i1+5,i2+4,i3+
     & 2,c3)+cr6*ui(i1+6,i2+4,i3+2,c3)+cr7*ui(i1+7,i2+4,i3+2,c3))+cs5*
     & (cr0*ui(i1,i2+5,i3+2,c3)+cr1*ui(i1+1,i2+5,i3+2,c3)+cr2*ui(i1+2,
     & i2+5,i3+2,c3)+cr3*ui(i1+3,i2+5,i3+2,c3)+cr4*ui(i1+4,i2+5,i3+2,
     & c3)+cr5*ui(i1+5,i2+5,i3+2,c3)+cr6*ui(i1+6,i2+5,i3+2,c3)+cr7*ui(
     & i1+7,i2+5,i3+2,c3))+cs6*(cr0*ui(i1,i2+6,i3+2,c3)+cr1*ui(i1+1,
     & i2+6,i3+2,c3)+cr2*ui(i1+2,i2+6,i3+2,c3)+cr3*ui(i1+3,i2+6,i3+2,
     & c3)+cr4*ui(i1+4,i2+6,i3+2,c3)+cr5*ui(i1+5,i2+6,i3+2,c3)+cr6*ui(
     & i1+6,i2+6,i3+2,c3)+cr7*ui(i1+7,i2+6,i3+2,c3))+cs7*(cr0*ui(i1,
     & i2+7,i3+2,c3)+cr1*ui(i1+1,i2+7,i3+2,c3)+cr2*ui(i1+2,i2+7,i3+2,
     & c3)+cr3*ui(i1+3,i2+7,i3+2,c3)+cr4*ui(i1+4,i2+7,i3+2,c3)+cr5*ui(
     & i1+5,i2+7,i3+2,c3)+cr6*ui(i1+6,i2+7,i3+2,c3)+cr7*ui(i1+7,i2+7,
     & i3+2,c3)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+ct3*(cs0*(cr0*ui(i1,i2,i3+3,c3)+cr1*ui(i1+1,i2,i3+3,
     & c3)+cr2*ui(i1+2,i2,i3+3,c3)+cr3*ui(i1+3,i2,i3+3,c3)+cr4*ui(i1+
     & 4,i2,i3+3,c3)+cr5*ui(i1+5,i2,i3+3,c3)+cr6*ui(i1+6,i2,i3+3,c3)+
     & cr7*ui(i1+7,i2,i3+3,c3))+cs1*(cr0*ui(i1,i2+1,i3+3,c3)+cr1*ui(
     & i1+1,i2+1,i3+3,c3)+cr2*ui(i1+2,i2+1,i3+3,c3)+cr3*ui(i1+3,i2+1,
     & i3+3,c3)+cr4*ui(i1+4,i2+1,i3+3,c3)+cr5*ui(i1+5,i2+1,i3+3,c3)+
     & cr6*ui(i1+6,i2+1,i3+3,c3)+cr7*ui(i1+7,i2+1,i3+3,c3))+cs2*(cr0*
     & ui(i1,i2+2,i3+3,c3)+cr1*ui(i1+1,i2+2,i3+3,c3)+cr2*ui(i1+2,i2+2,
     & i3+3,c3)+cr3*ui(i1+3,i2+2,i3+3,c3)+cr4*ui(i1+4,i2+2,i3+3,c3)+
     & cr5*ui(i1+5,i2+2,i3+3,c3)+cr6*ui(i1+6,i2+2,i3+3,c3)+cr7*ui(i1+
     & 7,i2+2,i3+3,c3))+cs3*(cr0*ui(i1,i2+3,i3+3,c3)+cr1*ui(i1+1,i2+3,
     & i3+3,c3)+cr2*ui(i1+2,i2+3,i3+3,c3)+cr3*ui(i1+3,i2+3,i3+3,c3)+
     & cr4*ui(i1+4,i2+3,i3+3,c3)+cr5*ui(i1+5,i2+3,i3+3,c3)+cr6*ui(i1+
     & 6,i2+3,i3+3,c3)+cr7*ui(i1+7,i2+3,i3+3,c3))+cs4*(cr0*ui(i1,i2+4,
     & i3+3,c3)+cr1*ui(i1+1,i2+4,i3+3,c3)+cr2*ui(i1+2,i2+4,i3+3,c3)+
     & cr3*ui(i1+3,i2+4,i3+3,c3)+cr4*ui(i1+4,i2+4,i3+3,c3)+cr5*ui(i1+
     & 5,i2+4,i3+3,c3)+cr6*ui(i1+6,i2+4,i3+3,c3)+cr7*ui(i1+7,i2+4,i3+
     & 3,c3))+cs5*(cr0*ui(i1,i2+5,i3+3,c3)+cr1*ui(i1+1,i2+5,i3+3,c3)+
     & cr2*ui(i1+2,i2+5,i3+3,c3)+cr3*ui(i1+3,i2+5,i3+3,c3)+cr4*ui(i1+
     & 4,i2+5,i3+3,c3)+cr5*ui(i1+5,i2+5,i3+3,c3)+cr6*ui(i1+6,i2+5,i3+
     & 3,c3)+cr7*ui(i1+7,i2+5,i3+3,c3))+cs6*(cr0*ui(i1,i2+6,i3+3,c3)+
     & cr1*ui(i1+1,i2+6,i3+3,c3)+cr2*ui(i1+2,i2+6,i3+3,c3)+cr3*ui(i1+
     & 3,i2+6,i3+3,c3)+cr4*ui(i1+4,i2+6,i3+3,c3)+cr5*ui(i1+5,i2+6,i3+
     & 3,c3)+cr6*ui(i1+6,i2+6,i3+3,c3)+cr7*ui(i1+7,i2+6,i3+3,c3))+cs7*
     & (cr0*ui(i1,i2+7,i3+3,c3)+cr1*ui(i1+1,i2+7,i3+3,c3)+cr2*ui(i1+2,
     & i2+7,i3+3,c3)+cr3*ui(i1+3,i2+7,i3+3,c3)+cr4*ui(i1+4,i2+7,i3+3,
     & c3)+cr5*ui(i1+5,i2+7,i3+3,c3)+cr6*ui(i1+6,i2+7,i3+3,c3)+cr7*ui(
     & i1+7,i2+7,i3+3,c3)))+ct4*(cs0*(cr0*ui(i1,i2,i3+4,c3)+cr1*ui(i1+
     & 1,i2,i3+4,c3)+cr2*ui(i1+2,i2,i3+4,c3)+cr3*ui(i1+3,i2,i3+4,c3)+
     & cr4*ui(i1+4,i2,i3+4,c3)+cr5*ui(i1+5,i2,i3+4,c3)+cr6*ui(i1+6,i2,
     & i3+4,c3)+cr7*ui(i1+7,i2,i3+4,c3))+cs1*(cr0*ui(i1,i2+1,i3+4,c3)+
     & cr1*ui(i1+1,i2+1,i3+4,c3)+cr2*ui(i1+2,i2+1,i3+4,c3)+cr3*ui(i1+
     & 3,i2+1,i3+4,c3)+cr4*ui(i1+4,i2+1,i3+4,c3)+cr5*ui(i1+5,i2+1,i3+
     & 4,c3)+cr6*ui(i1+6,i2+1,i3+4,c3)+cr7*ui(i1+7,i2+1,i3+4,c3))+cs2*
     & (cr0*ui(i1,i2+2,i3+4,c3)+cr1*ui(i1+1,i2+2,i3+4,c3)+cr2*ui(i1+2,
     & i2+2,i3+4,c3)+cr3*ui(i1+3,i2+2,i3+4,c3)+cr4*ui(i1+4,i2+2,i3+4,
     & c3)+cr5*ui(i1+5,i2+2,i3+4,c3)+cr6*ui(i1+6,i2+2,i3+4,c3)+cr7*ui(
     & i1+7,i2+2,i3+4,c3))+cs3*(cr0*ui(i1,i2+3,i3+4,c3)+cr1*ui(i1+1,
     & i2+3,i3+4,c3)+cr2*ui(i1+2,i2+3,i3+4,c3)+cr3*ui(i1+3,i2+3,i3+4,
     & c3)+cr4*ui(i1+4,i2+3,i3+4,c3)+cr5*ui(i1+5,i2+3,i3+4,c3)+cr6*ui(
     & i1+6,i2+3,i3+4,c3)+cr7*ui(i1+7,i2+3,i3+4,c3))+cs4*(cr0*ui(i1,
     & i2+4,i3+4,c3)+cr1*ui(i1+1,i2+4,i3+4,c3)+cr2*ui(i1+2,i2+4,i3+4,
     & c3)+cr3*ui(i1+3,i2+4,i3+4,c3)+cr4*ui(i1+4,i2+4,i3+4,c3)+cr5*ui(
     & i1+5,i2+4,i3+4,c3)+cr6*ui(i1+6,i2+4,i3+4,c3)+cr7*ui(i1+7,i2+4,
     & i3+4,c3))+cs5*(cr0*ui(i1,i2+5,i3+4,c3)+cr1*ui(i1+1,i2+5,i3+4,
     & c3)+cr2*ui(i1+2,i2+5,i3+4,c3)+cr3*ui(i1+3,i2+5,i3+4,c3)+cr4*ui(
     & i1+4,i2+5,i3+4,c3)+cr5*ui(i1+5,i2+5,i3+4,c3)+cr6*ui(i1+6,i2+5,
     & i3+4,c3)+cr7*ui(i1+7,i2+5,i3+4,c3))+cs6*(cr0*ui(i1,i2+6,i3+4,
     & c3)+cr1*ui(i1+1,i2+6,i3+4,c3)+cr2*ui(i1+2,i2+6,i3+4,c3)+cr3*ui(
     & i1+3,i2+6,i3+4,c3)+cr4*ui(i1+4,i2+6,i3+4,c3)+cr5*ui(i1+5,i2+6,
     & i3+4,c3)+cr6*ui(i1+6,i2+6,i3+4,c3)+cr7*ui(i1+7,i2+6,i3+4,c3))+
     & cs7*(cr0*ui(i1,i2+7,i3+4,c3)+cr1*ui(i1+1,i2+7,i3+4,c3)+cr2*ui(
     & i1+2,i2+7,i3+4,c3)+cr3*ui(i1+3,i2+7,i3+4,c3)+cr4*ui(i1+4,i2+7,
     & i3+4,c3)+cr5*ui(i1+5,i2+7,i3+4,c3)+cr6*ui(i1+6,i2+7,i3+4,c3)+
     & cr7*ui(i1+7,i2+7,i3+4,c3)))+ct5*(cs0*(cr0*ui(i1,i2,i3+5,c3)+
     & cr1*ui(i1+1,i2,i3+5,c3)+cr2*ui(i1+2,i2,i3+5,c3)+cr3*ui(i1+3,i2,
     & i3+5,c3)+cr4*ui(i1+4,i2,i3+5,c3)+cr5*ui(i1+5,i2,i3+5,c3)+cr6*
     & ui(i1+6,i2,i3+5,c3)+cr7*ui(i1+7,i2,i3+5,c3))+cs1*(cr0*ui(i1,i2+
     & 1,i3+5,c3)+cr1*ui(i1+1,i2+1,i3+5,c3)+cr2*ui(i1+2,i2+1,i3+5,c3)+
     & cr3*ui(i1+3,i2+1,i3+5,c3)+cr4*ui(i1+4,i2+1,i3+5,c3)+cr5*ui(i1+
     & 5,i2+1,i3+5,c3)+cr6*ui(i1+6,i2+1,i3+5,c3)+cr7*ui(i1+7,i2+1,i3+
     & 5,c3))+cs2*(cr0*ui(i1,i2+2,i3+5,c3)+cr1*ui(i1+1,i2+2,i3+5,c3)+
     & cr2*ui(i1+2,i2+2,i3+5,c3)+cr3*ui(i1+3,i2+2,i3+5,c3)+cr4*ui(i1+
     & 4,i2+2,i3+5,c3)+cr5*ui(i1+5,i2+2,i3+5,c3)+cr6*ui(i1+6,i2+2,i3+
     & 5,c3)+cr7*ui(i1+7,i2+2,i3+5,c3))+cs3*(cr0*ui(i1,i2+3,i3+5,c3)+
     & cr1*ui(i1+1,i2+3,i3+5,c3)+cr2*ui(i1+2,i2+3,i3+5,c3)+cr3*ui(i1+
     & 3,i2+3,i3+5,c3)+cr4*ui(i1+4,i2+3,i3+5,c3)+cr5*ui(i1+5,i2+3,i3+
     & 5,c3)+cr6*ui(i1+6,i2+3,i3+5,c3)+cr7*ui(i1+7,i2+3,i3+5,c3))+cs4*
     & (cr0*ui(i1,i2+4,i3+5,c3)+cr1*ui(i1+1,i2+4,i3+5,c3)+cr2*ui(i1+2,
     & i2+4,i3+5,c3)+cr3*ui(i1+3,i2+4,i3+5,c3)+cr4*ui(i1+4,i2+4,i3+5,
     & c3)+cr5*ui(i1+5,i2+4,i3+5,c3)+cr6*ui(i1+6,i2+4,i3+5,c3)+cr7*ui(
     & i1+7,i2+4,i3+5,c3))+cs5*(cr0*ui(i1,i2+5,i3+5,c3)+cr1*ui(i1+1,
     & i2+5,i3+5,c3)+cr2*ui(i1+2,i2+5,i3+5,c3)+cr3*ui(i1+3,i2+5,i3+5,
     & c3)+cr4*ui(i1+4,i2+5,i3+5,c3)+cr5*ui(i1+5,i2+5,i3+5,c3)+cr6*ui(
     & i1+6,i2+5,i3+5,c3)+cr7*ui(i1+7,i2+5,i3+5,c3))+cs6*(cr0*ui(i1,
     & i2+6,i3+5,c3)+cr1*ui(i1+1,i2+6,i3+5,c3)+cr2*ui(i1+2,i2+6,i3+5,
     & c3)+cr3*ui(i1+3,i2+6,i3+5,c3)+cr4*ui(i1+4,i2+6,i3+5,c3)+cr5*ui(
     & i1+5,i2+6,i3+5,c3)+cr6*ui(i1+6,i2+6,i3+5,c3)+cr7*ui(i1+7,i2+6,
     & i3+5,c3))+cs7*(cr0*ui(i1,i2+7,i3+5,c3)+cr1*ui(i1+1,i2+7,i3+5,
     & c3)+cr2*ui(i1+2,i2+7,i3+5,c3)+cr3*ui(i1+3,i2+7,i3+5,c3)+cr4*ui(
     & i1+4,i2+7,i3+5,c3)+cr5*ui(i1+5,i2+7,i3+5,c3)+cr6*ui(i1+6,i2+7,
     & i3+5,c3)+cr7*ui(i1+7,i2+7,i3+5,c3)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+ct6*(cs0*(cr0*ui(i1,i2,i3+6,c3)+cr1*ui(i1+1,i2,i3+6,
     & c3)+cr2*ui(i1+2,i2,i3+6,c3)+cr3*ui(i1+3,i2,i3+6,c3)+cr4*ui(i1+
     & 4,i2,i3+6,c3)+cr5*ui(i1+5,i2,i3+6,c3)+cr6*ui(i1+6,i2,i3+6,c3)+
     & cr7*ui(i1+7,i2,i3+6,c3))+cs1*(cr0*ui(i1,i2+1,i3+6,c3)+cr1*ui(
     & i1+1,i2+1,i3+6,c3)+cr2*ui(i1+2,i2+1,i3+6,c3)+cr3*ui(i1+3,i2+1,
     & i3+6,c3)+cr4*ui(i1+4,i2+1,i3+6,c3)+cr5*ui(i1+5,i2+1,i3+6,c3)+
     & cr6*ui(i1+6,i2+1,i3+6,c3)+cr7*ui(i1+7,i2+1,i3+6,c3))+cs2*(cr0*
     & ui(i1,i2+2,i3+6,c3)+cr1*ui(i1+1,i2+2,i3+6,c3)+cr2*ui(i1+2,i2+2,
     & i3+6,c3)+cr3*ui(i1+3,i2+2,i3+6,c3)+cr4*ui(i1+4,i2+2,i3+6,c3)+
     & cr5*ui(i1+5,i2+2,i3+6,c3)+cr6*ui(i1+6,i2+2,i3+6,c3)+cr7*ui(i1+
     & 7,i2+2,i3+6,c3))+cs3*(cr0*ui(i1,i2+3,i3+6,c3)+cr1*ui(i1+1,i2+3,
     & i3+6,c3)+cr2*ui(i1+2,i2+3,i3+6,c3)+cr3*ui(i1+3,i2+3,i3+6,c3)+
     & cr4*ui(i1+4,i2+3,i3+6,c3)+cr5*ui(i1+5,i2+3,i3+6,c3)+cr6*ui(i1+
     & 6,i2+3,i3+6,c3)+cr7*ui(i1+7,i2+3,i3+6,c3))+cs4*(cr0*ui(i1,i2+4,
     & i3+6,c3)+cr1*ui(i1+1,i2+4,i3+6,c3)+cr2*ui(i1+2,i2+4,i3+6,c3)+
     & cr3*ui(i1+3,i2+4,i3+6,c3)+cr4*ui(i1+4,i2+4,i3+6,c3)+cr5*ui(i1+
     & 5,i2+4,i3+6,c3)+cr6*ui(i1+6,i2+4,i3+6,c3)+cr7*ui(i1+7,i2+4,i3+
     & 6,c3))+cs5*(cr0*ui(i1,i2+5,i3+6,c3)+cr1*ui(i1+1,i2+5,i3+6,c3)+
     & cr2*ui(i1+2,i2+5,i3+6,c3)+cr3*ui(i1+3,i2+5,i3+6,c3)+cr4*ui(i1+
     & 4,i2+5,i3+6,c3)+cr5*ui(i1+5,i2+5,i3+6,c3)+cr6*ui(i1+6,i2+5,i3+
     & 6,c3)+cr7*ui(i1+7,i2+5,i3+6,c3))+cs6*(cr0*ui(i1,i2+6,i3+6,c3)+
     & cr1*ui(i1+1,i2+6,i3+6,c3)+cr2*ui(i1+2,i2+6,i3+6,c3)+cr3*ui(i1+
     & 3,i2+6,i3+6,c3)+cr4*ui(i1+4,i2+6,i3+6,c3)+cr5*ui(i1+5,i2+6,i3+
     & 6,c3)+cr6*ui(i1+6,i2+6,i3+6,c3)+cr7*ui(i1+7,i2+6,i3+6,c3))+cs7*
     & (cr0*ui(i1,i2+7,i3+6,c3)+cr1*ui(i1+1,i2+7,i3+6,c3)+cr2*ui(i1+2,
     & i2+7,i3+6,c3)+cr3*ui(i1+3,i2+7,i3+6,c3)+cr4*ui(i1+4,i2+7,i3+6,
     & c3)+cr5*ui(i1+5,i2+7,i3+6,c3)+cr6*ui(i1+6,i2+7,i3+6,c3)+cr7*ui(
     & i1+7,i2+7,i3+6,c3)))+ct7*(cs0*(cr0*ui(i1,i2,i3+7,c3)+cr1*ui(i1+
     & 1,i2,i3+7,c3)+cr2*ui(i1+2,i2,i3+7,c3)+cr3*ui(i1+3,i2,i3+7,c3)+
     & cr4*ui(i1+4,i2,i3+7,c3)+cr5*ui(i1+5,i2,i3+7,c3)+cr6*ui(i1+6,i2,
     & i3+7,c3)+cr7*ui(i1+7,i2,i3+7,c3))+cs1*(cr0*ui(i1,i2+1,i3+7,c3)+
     & cr1*ui(i1+1,i2+1,i3+7,c3)+cr2*ui(i1+2,i2+1,i3+7,c3)+cr3*ui(i1+
     & 3,i2+1,i3+7,c3)+cr4*ui(i1+4,i2+1,i3+7,c3)+cr5*ui(i1+5,i2+1,i3+
     & 7,c3)+cr6*ui(i1+6,i2+1,i3+7,c3)+cr7*ui(i1+7,i2+1,i3+7,c3))+cs2*
     & (cr0*ui(i1,i2+2,i3+7,c3)+cr1*ui(i1+1,i2+2,i3+7,c3)+cr2*ui(i1+2,
     & i2+2,i3+7,c3)+cr3*ui(i1+3,i2+2,i3+7,c3)+cr4*ui(i1+4,i2+2,i3+7,
     & c3)+cr5*ui(i1+5,i2+2,i3+7,c3)+cr6*ui(i1+6,i2+2,i3+7,c3)+cr7*ui(
     & i1+7,i2+2,i3+7,c3))+cs3*(cr0*ui(i1,i2+3,i3+7,c3)+cr1*ui(i1+1,
     & i2+3,i3+7,c3)+cr2*ui(i1+2,i2+3,i3+7,c3)+cr3*ui(i1+3,i2+3,i3+7,
     & c3)+cr4*ui(i1+4,i2+3,i3+7,c3)+cr5*ui(i1+5,i2+3,i3+7,c3)+cr6*ui(
     & i1+6,i2+3,i3+7,c3)+cr7*ui(i1+7,i2+3,i3+7,c3))+cs4*(cr0*ui(i1,
     & i2+4,i3+7,c3)+cr1*ui(i1+1,i2+4,i3+7,c3)+cr2*ui(i1+2,i2+4,i3+7,
     & c3)+cr3*ui(i1+3,i2+4,i3+7,c3)+cr4*ui(i1+4,i2+4,i3+7,c3)+cr5*ui(
     & i1+5,i2+4,i3+7,c3)+cr6*ui(i1+6,i2+4,i3+7,c3)+cr7*ui(i1+7,i2+4,
     & i3+7,c3))+cs5*(cr0*ui(i1,i2+5,i3+7,c3)+cr1*ui(i1+1,i2+5,i3+7,
     & c3)+cr2*ui(i1+2,i2+5,i3+7,c3)+cr3*ui(i1+3,i2+5,i3+7,c3)+cr4*ui(
     & i1+4,i2+5,i3+7,c3)+cr5*ui(i1+5,i2+5,i3+7,c3)+cr6*ui(i1+6,i2+5,
     & i3+7,c3)+cr7*ui(i1+7,i2+5,i3+7,c3))+cs6*(cr0*ui(i1,i2+6,i3+7,
     & c3)+cr1*ui(i1+1,i2+6,i3+7,c3)+cr2*ui(i1+2,i2+6,i3+7,c3)+cr3*ui(
     & i1+3,i2+6,i3+7,c3)+cr4*ui(i1+4,i2+6,i3+7,c3)+cr5*ui(i1+5,i2+6,
     & i3+7,c3)+cr6*ui(i1+6,i2+6,i3+7,c3)+cr7*ui(i1+7,i2+6,i3+7,c3))+
     & cs7*(cr0*ui(i1,i2+7,i3+7,c3)+cr1*ui(i1+1,i2+7,i3+7,c3)+cr2*ui(
     & i1+2,i2+7,i3+7,c3)+cr3*ui(i1+3,i2+7,i3+7,c3)+cr4*ui(i1+4,i2+7,
     & i3+7,c3)+cr5*ui(i1+5,i2+7,i3+7,c3)+cr6*ui(i1+6,i2+7,i3+7,c3)+
     & cr7*ui(i1+7,i2+7,i3+7,c3)))
             else
               write(*,*) 'ERROR varWidth=',varWidth(i)
               stop 151
             end if
! endLoops3d()
             end do
             end do
         else if( width(1).eq.3 .and. width(2).eq.3 .and. width(3)
     & .eq.3 )then
! loops3d($interpSparseStorage333(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)
           if( c3a.eq.c3b )then
             do c3=c3a,c3b
             do i=nia,nib
! interpSparseStorage333(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q30(c(i,0,0,0))
                 cs0 = q30(c(i,1,0,0))
                 ct0 = q30(c(i,2,0,0))
                 cr1 = q31(c(i,0,0,0))
                 cs1 = q31(c(i,1,0,0))
                 ct1 = q31(c(i,2,0,0))
                 cr2 = q32(c(i,0,0,0))
                 cs2 = q32(c(i,1,0,0))
                 ct2 = q32(c(i,2,0,0))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ct0*(cs0*(cr0*ui(i1,i2,
     & i3+0,c3)+cr1*ui(i1+1,i2,i3+0,c3)+cr2*ui(i1+2,i2,i3+0,c3))+cs1*(
     & cr0*ui(i1,i2+1,i3+0,c3)+cr1*ui(i1+1,i2+1,i3+0,c3)+cr2*ui(i1+2,
     & i2+1,i3+0,c3))+cs2*(cr0*ui(i1,i2+2,i3+0,c3)+cr1*ui(i1+1,i2+2,
     & i3+0,c3)+cr2*ui(i1+2,i2+2,i3+0,c3)))+ct1*(cs0*(cr0*ui(i1,i2,i3+
     & 1,c3)+cr1*ui(i1+1,i2,i3+1,c3)+cr2*ui(i1+2,i2,i3+1,c3))+cs1*(
     & cr0*ui(i1,i2+1,i3+1,c3)+cr1*ui(i1+1,i2+1,i3+1,c3)+cr2*ui(i1+2,
     & i2+1,i3+1,c3))+cs2*(cr0*ui(i1,i2+2,i3+1,c3)+cr1*ui(i1+1,i2+2,
     & i3+1,c3)+cr2*ui(i1+2,i2+2,i3+1,c3)))+ct2*(cs0*(cr0*ui(i1,i2,i3+
     & 2,c3)+cr1*ui(i1+1,i2,i3+2,c3)+cr2*ui(i1+2,i2,i3+2,c3))+cs1*(
     & cr0*ui(i1,i2+1,i3+2,c3)+cr1*ui(i1+1,i2+1,i3+2,c3)+cr2*ui(i1+2,
     & i2+1,i3+2,c3))+cs2*(cr0*ui(i1,i2+2,i3+2,c3)+cr1*ui(i1+1,i2+2,
     & i3+2,c3)+cr2*ui(i1+2,i2+2,i3+2,c3)))


             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
! interpSparseStorage333(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q30(c(i,0,0,0))
                 cs0 = q30(c(i,1,0,0))
                 ct0 = q30(c(i,2,0,0))
                 cr1 = q31(c(i,0,0,0))
                 cs1 = q31(c(i,1,0,0))
                 ct1 = q31(c(i,2,0,0))
                 cr2 = q32(c(i,0,0,0))
                 cs2 = q32(c(i,1,0,0))
                 ct2 = q32(c(i,2,0,0))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ct0*(cs0*(cr0*ui(i1,i2,
     & i3+0,c3)+cr1*ui(i1+1,i2,i3+0,c3)+cr2*ui(i1+2,i2,i3+0,c3))+cs1*(
     & cr0*ui(i1,i2+1,i3+0,c3)+cr1*ui(i1+1,i2+1,i3+0,c3)+cr2*ui(i1+2,
     & i2+1,i3+0,c3))+cs2*(cr0*ui(i1,i2+2,i3+0,c3)+cr1*ui(i1+1,i2+2,
     & i3+0,c3)+cr2*ui(i1+2,i2+2,i3+0,c3)))+ct1*(cs0*(cr0*ui(i1,i2,i3+
     & 1,c3)+cr1*ui(i1+1,i2,i3+1,c3)+cr2*ui(i1+2,i2,i3+1,c3))+cs1*(
     & cr0*ui(i1,i2+1,i3+1,c3)+cr1*ui(i1+1,i2+1,i3+1,c3)+cr2*ui(i1+2,
     & i2+1,i3+1,c3))+cs2*(cr0*ui(i1,i2+2,i3+1,c3)+cr1*ui(i1+1,i2+2,
     & i3+1,c3)+cr2*ui(i1+2,i2+2,i3+1,c3)))+ct2*(cs0*(cr0*ui(i1,i2,i3+
     & 2,c3)+cr1*ui(i1+1,i2,i3+2,c3)+cr2*ui(i1+2,i2,i3+2,c3))+cs1*(
     & cr0*ui(i1,i2+1,i3+2,c3)+cr1*ui(i1+1,i2+1,i3+2,c3)+cr2*ui(i1+2,
     & i2+1,i3+2,c3))+cs2*(cr0*ui(i1,i2+2,i3+2,c3)+cr1*ui(i1+1,i2+2,
     & i3+2,c3)+cr2*ui(i1+2,i2+2,i3+2,c3)))


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
! loops3d($interpSparseStorage222(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)
           if( c3a.eq.c3b )then
             do c3=c3a,c3b
             do i=nia,nib
! interpSparseStorage222(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q20(c(i,0,0,0))
                 cs0 = q20(c(i,1,0,0))
                 ct0 = q20(c(i,2,0,0))
                 cr1 = q21(c(i,0,0,0))
                 cs1 = q21(c(i,1,0,0))
                 ct1 = q21(c(i,2,0,0))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ct0*(cs0*(cr0*ui(i1,i2,
     & i3+0,c3)+cr1*ui(i1+1,i2,i3+0,c3))+cs1*(cr0*ui(i1,i2+1,i3+0,c3)+
     & cr1*ui(i1+1,i2+1,i3+0,c3)))+ct1*(cs0*(cr0*ui(i1,i2,i3+1,c3)+
     & cr1*ui(i1+1,i2,i3+1,c3))+cs1*(cr0*ui(i1,i2+1,i3+1,c3)+cr1*ui(
     & i1+1,i2+1,i3+1,c3)))


             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
! interpSparseStorage222(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q20(c(i,0,0,0))
                 cs0 = q20(c(i,1,0,0))
                 ct0 = q20(c(i,2,0,0))
                 cr1 = q21(c(i,0,0,0))
                 cs1 = q21(c(i,1,0,0))
                 ct1 = q21(c(i,2,0,0))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ct0*(cs0*(cr0*ui(i1,i2,
     & i3+0,c3)+cr1*ui(i1+1,i2,i3+0,c3))+cs1*(cr0*ui(i1,i2+1,i3+0,c3)+
     & cr1*ui(i1+1,i2+1,i3+0,c3)))+ct1*(cs0*(cr0*ui(i1,i2,i3+1,c3)+
     & cr1*ui(i1+1,i2,i3+1,c3))+cs1*(cr0*ui(i1,i2+1,i3+1,c3)+cr1*ui(
     & i1+1,i2+1,i3+1,c3)))


             end do
             end do
           end if
         else if( width(1).eq.4 .and. width(2).eq.4 .and. width(3)
     & .eq.4 )then
! loops3d($interpSparseStorage444(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)
           if( c3a.eq.c3b )then
             do c3=c3a,c3b
             do i=nia,nib
! interpSparseStorage444(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q40(c(i,0,0,0))
                 cs0 = q40(c(i,1,0,0))
                 ct0 = q40(c(i,2,0,0))
                 cr1 = q41(c(i,0,0,0))
                 cs1 = q41(c(i,1,0,0))
                 ct1 = q41(c(i,2,0,0))
                 cr2 = q42(c(i,0,0,0))
                 cs2 = q42(c(i,1,0,0))
                 ct2 = q42(c(i,2,0,0))
                 cr3 = q43(c(i,0,0,0))
                 cs3 = q43(c(i,1,0,0))
                 ct3 = q43(c(i,2,0,0))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ct0*(cs0*(cr0*ui(i1,i2,
     & i3+0,c3)+cr1*ui(i1+1,i2,i3+0,c3)+cr2*ui(i1+2,i2,i3+0,c3)+cr3*
     & ui(i1+3,i2,i3+0,c3))+cs1*(cr0*ui(i1,i2+1,i3+0,c3)+cr1*ui(i1+1,
     & i2+1,i3+0,c3)+cr2*ui(i1+2,i2+1,i3+0,c3)+cr3*ui(i1+3,i2+1,i3+0,
     & c3))+cs2*(cr0*ui(i1,i2+2,i3+0,c3)+cr1*ui(i1+1,i2+2,i3+0,c3)+
     & cr2*ui(i1+2,i2+2,i3+0,c3)+cr3*ui(i1+3,i2+2,i3+0,c3))+cs3*(cr0*
     & ui(i1,i2+3,i3+0,c3)+cr1*ui(i1+1,i2+3,i3+0,c3)+cr2*ui(i1+2,i2+3,
     & i3+0,c3)+cr3*ui(i1+3,i2+3,i3+0,c3)))+ct1*(cs0*(cr0*ui(i1,i2,i3+
     & 1,c3)+cr1*ui(i1+1,i2,i3+1,c3)+cr2*ui(i1+2,i2,i3+1,c3)+cr3*ui(
     & i1+3,i2,i3+1,c3))+cs1*(cr0*ui(i1,i2+1,i3+1,c3)+cr1*ui(i1+1,i2+
     & 1,i3+1,c3)+cr2*ui(i1+2,i2+1,i3+1,c3)+cr3*ui(i1+3,i2+1,i3+1,c3))
     & +cs2*(cr0*ui(i1,i2+2,i3+1,c3)+cr1*ui(i1+1,i2+2,i3+1,c3)+cr2*ui(
     & i1+2,i2+2,i3+1,c3)+cr3*ui(i1+3,i2+2,i3+1,c3))+cs3*(cr0*ui(i1,
     & i2+3,i3+1,c3)+cr1*ui(i1+1,i2+3,i3+1,c3)+cr2*ui(i1+2,i2+3,i3+1,
     & c3)+cr3*ui(i1+3,i2+3,i3+1,c3)))+ct2*(cs0*(cr0*ui(i1,i2,i3+2,c3)
     & +cr1*ui(i1+1,i2,i3+2,c3)+cr2*ui(i1+2,i2,i3+2,c3)+cr3*ui(i1+3,
     & i2,i3+2,c3))+cs1*(cr0*ui(i1,i2+1,i3+2,c3)+cr1*ui(i1+1,i2+1,i3+
     & 2,c3)+cr2*ui(i1+2,i2+1,i3+2,c3)+cr3*ui(i1+3,i2+1,i3+2,c3))+cs2*
     & (cr0*ui(i1,i2+2,i3+2,c3)+cr1*ui(i1+1,i2+2,i3+2,c3)+cr2*ui(i1+2,
     & i2+2,i3+2,c3)+cr3*ui(i1+3,i2+2,i3+2,c3))+cs3*(cr0*ui(i1,i2+3,
     & i3+2,c3)+cr1*ui(i1+1,i2+3,i3+2,c3)+cr2*ui(i1+2,i2+3,i3+2,c3)+
     & cr3*ui(i1+3,i2+3,i3+2,c3)))+ct3*(cs0*(cr0*ui(i1,i2,i3+3,c3)+
     & cr1*ui(i1+1,i2,i3+3,c3)+cr2*ui(i1+2,i2,i3+3,c3)+cr3*ui(i1+3,i2,
     & i3+3,c3))+cs1*(cr0*ui(i1,i2+1,i3+3,c3)+cr1*ui(i1+1,i2+1,i3+3,
     & c3)+cr2*ui(i1+2,i2+1,i3+3,c3)+cr3*ui(i1+3,i2+1,i3+3,c3))+cs2*(
     & cr0*ui(i1,i2+2,i3+3,c3)+cr1*ui(i1+1,i2+2,i3+3,c3)+cr2*ui(i1+2,
     & i2+2,i3+3,c3)+cr3*ui(i1+3,i2+2,i3+3,c3))+cs3*(cr0*ui(i1,i2+3,
     & i3+3,c3)+cr1*ui(i1+1,i2+3,i3+3,c3)+cr2*ui(i1+2,i2+3,i3+3,c3)+
     & cr3*ui(i1+3,i2+3,i3+3,c3)))


             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
! interpSparseStorage444(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q40(c(i,0,0,0))
                 cs0 = q40(c(i,1,0,0))
                 ct0 = q40(c(i,2,0,0))
                 cr1 = q41(c(i,0,0,0))
                 cs1 = q41(c(i,1,0,0))
                 ct1 = q41(c(i,2,0,0))
                 cr2 = q42(c(i,0,0,0))
                 cs2 = q42(c(i,1,0,0))
                 ct2 = q42(c(i,2,0,0))
                 cr3 = q43(c(i,0,0,0))
                 cs3 = q43(c(i,1,0,0))
                 ct3 = q43(c(i,2,0,0))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ct0*(cs0*(cr0*ui(i1,i2,
     & i3+0,c3)+cr1*ui(i1+1,i2,i3+0,c3)+cr2*ui(i1+2,i2,i3+0,c3)+cr3*
     & ui(i1+3,i2,i3+0,c3))+cs1*(cr0*ui(i1,i2+1,i3+0,c3)+cr1*ui(i1+1,
     & i2+1,i3+0,c3)+cr2*ui(i1+2,i2+1,i3+0,c3)+cr3*ui(i1+3,i2+1,i3+0,
     & c3))+cs2*(cr0*ui(i1,i2+2,i3+0,c3)+cr1*ui(i1+1,i2+2,i3+0,c3)+
     & cr2*ui(i1+2,i2+2,i3+0,c3)+cr3*ui(i1+3,i2+2,i3+0,c3))+cs3*(cr0*
     & ui(i1,i2+3,i3+0,c3)+cr1*ui(i1+1,i2+3,i3+0,c3)+cr2*ui(i1+2,i2+3,
     & i3+0,c3)+cr3*ui(i1+3,i2+3,i3+0,c3)))+ct1*(cs0*(cr0*ui(i1,i2,i3+
     & 1,c3)+cr1*ui(i1+1,i2,i3+1,c3)+cr2*ui(i1+2,i2,i3+1,c3)+cr3*ui(
     & i1+3,i2,i3+1,c3))+cs1*(cr0*ui(i1,i2+1,i3+1,c3)+cr1*ui(i1+1,i2+
     & 1,i3+1,c3)+cr2*ui(i1+2,i2+1,i3+1,c3)+cr3*ui(i1+3,i2+1,i3+1,c3))
     & +cs2*(cr0*ui(i1,i2+2,i3+1,c3)+cr1*ui(i1+1,i2+2,i3+1,c3)+cr2*ui(
     & i1+2,i2+2,i3+1,c3)+cr3*ui(i1+3,i2+2,i3+1,c3))+cs3*(cr0*ui(i1,
     & i2+3,i3+1,c3)+cr1*ui(i1+1,i2+3,i3+1,c3)+cr2*ui(i1+2,i2+3,i3+1,
     & c3)+cr3*ui(i1+3,i2+3,i3+1,c3)))+ct2*(cs0*(cr0*ui(i1,i2,i3+2,c3)
     & +cr1*ui(i1+1,i2,i3+2,c3)+cr2*ui(i1+2,i2,i3+2,c3)+cr3*ui(i1+3,
     & i2,i3+2,c3))+cs1*(cr0*ui(i1,i2+1,i3+2,c3)+cr1*ui(i1+1,i2+1,i3+
     & 2,c3)+cr2*ui(i1+2,i2+1,i3+2,c3)+cr3*ui(i1+3,i2+1,i3+2,c3))+cs2*
     & (cr0*ui(i1,i2+2,i3+2,c3)+cr1*ui(i1+1,i2+2,i3+2,c3)+cr2*ui(i1+2,
     & i2+2,i3+2,c3)+cr3*ui(i1+3,i2+2,i3+2,c3))+cs3*(cr0*ui(i1,i2+3,
     & i3+2,c3)+cr1*ui(i1+1,i2+3,i3+2,c3)+cr2*ui(i1+2,i2+3,i3+2,c3)+
     & cr3*ui(i1+3,i2+3,i3+2,c3)))+ct3*(cs0*(cr0*ui(i1,i2,i3+3,c3)+
     & cr1*ui(i1+1,i2,i3+3,c3)+cr2*ui(i1+2,i2,i3+3,c3)+cr3*ui(i1+3,i2,
     & i3+3,c3))+cs1*(cr0*ui(i1,i2+1,i3+3,c3)+cr1*ui(i1+1,i2+1,i3+3,
     & c3)+cr2*ui(i1+2,i2+1,i3+3,c3)+cr3*ui(i1+3,i2+1,i3+3,c3))+cs2*(
     & cr0*ui(i1,i2+2,i3+3,c3)+cr1*ui(i1+1,i2+2,i3+3,c3)+cr2*ui(i1+2,
     & i2+2,i3+3,c3)+cr3*ui(i1+3,i2+2,i3+3,c3))+cs3*(cr0*ui(i1,i2+3,
     & i3+3,c3)+cr1*ui(i1+1,i2+3,i3+3,c3)+cr2*ui(i1+2,i2+3,i3+3,c3)+
     & cr3*ui(i1+3,i2+3,i3+3,c3)))


             end do
             end do
           end if
         else if( width(1).eq.5 .and. width(2).eq.5 .and. width(3)
     & .eq.5 )then
           ! write(*,*) 'interpOpt explicit interp width=5'
! loops3d($interpSparseStorage555(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)
           if( c3a.eq.c3b )then
             do c3=c3a,c3b
             do i=nia,nib
! interpSparseStorage555(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q50(c(i,0,0,0))
                 cs0 = q50(c(i,1,0,0))
                 ct0 = q50(c(i,2,0,0))
                 cr1 = q51(c(i,0,0,0))
                 cs1 = q51(c(i,1,0,0))
                 ct1 = q51(c(i,2,0,0))
                 cr2 = q52(c(i,0,0,0))
                 cs2 = q52(c(i,1,0,0))
                 ct2 = q52(c(i,2,0,0))
                 cr3 = q53(c(i,0,0,0))
                 cs3 = q53(c(i,1,0,0))
                 ct3 = q53(c(i,2,0,0))
                 cr4 = q54(c(i,0,0,0))
                 cs4 = q54(c(i,1,0,0))
                 ct4 = q54(c(i,2,0,0))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ct0*(cs0*(cr0*ui(i1,i2,
     & i3+0,c3)+cr1*ui(i1+1,i2,i3+0,c3)+cr2*ui(i1+2,i2,i3+0,c3)+cr3*
     & ui(i1+3,i2,i3+0,c3)+cr4*ui(i1+4,i2,i3+0,c3))+cs1*(cr0*ui(i1,i2+
     & 1,i3+0,c3)+cr1*ui(i1+1,i2+1,i3+0,c3)+cr2*ui(i1+2,i2+1,i3+0,c3)+
     & cr3*ui(i1+3,i2+1,i3+0,c3)+cr4*ui(i1+4,i2+1,i3+0,c3))+cs2*(cr0*
     & ui(i1,i2+2,i3+0,c3)+cr1*ui(i1+1,i2+2,i3+0,c3)+cr2*ui(i1+2,i2+2,
     & i3+0,c3)+cr3*ui(i1+3,i2+2,i3+0,c3)+cr4*ui(i1+4,i2+2,i3+0,c3))+
     & cs3*(cr0*ui(i1,i2+3,i3+0,c3)+cr1*ui(i1+1,i2+3,i3+0,c3)+cr2*ui(
     & i1+2,i2+3,i3+0,c3)+cr3*ui(i1+3,i2+3,i3+0,c3)+cr4*ui(i1+4,i2+3,
     & i3+0,c3))+cs4*(cr0*ui(i1,i2+4,i3+0,c3)+cr1*ui(i1+1,i2+4,i3+0,
     & c3)+cr2*ui(i1+2,i2+4,i3+0,c3)+cr3*ui(i1+3,i2+4,i3+0,c3)+cr4*ui(
     & i1+4,i2+4,i3+0,c3)))+ct1*(cs0*(cr0*ui(i1,i2,i3+1,c3)+cr1*ui(i1+
     & 1,i2,i3+1,c3)+cr2*ui(i1+2,i2,i3+1,c3)+cr3*ui(i1+3,i2,i3+1,c3)+
     & cr4*ui(i1+4,i2,i3+1,c3))+cs1*(cr0*ui(i1,i2+1,i3+1,c3)+cr1*ui(
     & i1+1,i2+1,i3+1,c3)+cr2*ui(i1+2,i2+1,i3+1,c3)+cr3*ui(i1+3,i2+1,
     & i3+1,c3)+cr4*ui(i1+4,i2+1,i3+1,c3))+cs2*(cr0*ui(i1,i2+2,i3+1,
     & c3)+cr1*ui(i1+1,i2+2,i3+1,c3)+cr2*ui(i1+2,i2+2,i3+1,c3)+cr3*ui(
     & i1+3,i2+2,i3+1,c3)+cr4*ui(i1+4,i2+2,i3+1,c3))+cs3*(cr0*ui(i1,
     & i2+3,i3+1,c3)+cr1*ui(i1+1,i2+3,i3+1,c3)+cr2*ui(i1+2,i2+3,i3+1,
     & c3)+cr3*ui(i1+3,i2+3,i3+1,c3)+cr4*ui(i1+4,i2+3,i3+1,c3))+cs4*(
     & cr0*ui(i1,i2+4,i3+1,c3)+cr1*ui(i1+1,i2+4,i3+1,c3)+cr2*ui(i1+2,
     & i2+4,i3+1,c3)+cr3*ui(i1+3,i2+4,i3+1,c3)+cr4*ui(i1+4,i2+4,i3+1,
     & c3)))+ct2*(cs0*(cr0*ui(i1,i2,i3+2,c3)+cr1*ui(i1+1,i2,i3+2,c3)+
     & cr2*ui(i1+2,i2,i3+2,c3)+cr3*ui(i1+3,i2,i3+2,c3)+cr4*ui(i1+4,i2,
     & i3+2,c3))+cs1*(cr0*ui(i1,i2+1,i3+2,c3)+cr1*ui(i1+1,i2+1,i3+2,
     & c3)+cr2*ui(i1+2,i2+1,i3+2,c3)+cr3*ui(i1+3,i2+1,i3+2,c3)+cr4*ui(
     & i1+4,i2+1,i3+2,c3))+cs2*(cr0*ui(i1,i2+2,i3+2,c3)+cr1*ui(i1+1,
     & i2+2,i3+2,c3)+cr2*ui(i1+2,i2+2,i3+2,c3)+cr3*ui(i1+3,i2+2,i3+2,
     & c3)+cr4*ui(i1+4,i2+2,i3+2,c3))+cs3*(cr0*ui(i1,i2+3,i3+2,c3)+
     & cr1*ui(i1+1,i2+3,i3+2,c3)+cr2*ui(i1+2,i2+3,i3+2,c3)+cr3*ui(i1+
     & 3,i2+3,i3+2,c3)+cr4*ui(i1+4,i2+3,i3+2,c3))+cs4*(cr0*ui(i1,i2+4,
     & i3+2,c3)+cr1*ui(i1+1,i2+4,i3+2,c3)+cr2*ui(i1+2,i2+4,i3+2,c3)+
     & cr3*ui(i1+3,i2+4,i3+2,c3)+cr4*ui(i1+4,i2+4,i3+2,c3)))+ct3*(cs0*
     & (cr0*ui(i1,i2,i3+3,c3)+cr1*ui(i1+1,i2,i3+3,c3)+cr2*ui(i1+2,i2,
     & i3+3,c3)+cr3*ui(i1+3,i2,i3+3,c3)+cr4*ui(i1+4,i2,i3+3,c3))+cs1*(
     & cr0*ui(i1,i2+1,i3+3,c3)+cr1*ui(i1+1,i2+1,i3+3,c3)+cr2*ui(i1+2,
     & i2+1,i3+3,c3)+cr3*ui(i1+3,i2+1,i3+3,c3)+cr4*ui(i1+4,i2+1,i3+3,
     & c3))+cs2*(cr0*ui(i1,i2+2,i3+3,c3)+cr1*ui(i1+1,i2+2,i3+3,c3)+
     & cr2*ui(i1+2,i2+2,i3+3,c3)+cr3*ui(i1+3,i2+2,i3+3,c3)+cr4*ui(i1+
     & 4,i2+2,i3+3,c3))+cs3*(cr0*ui(i1,i2+3,i3+3,c3)+cr1*ui(i1+1,i2+3,
     & i3+3,c3)+cr2*ui(i1+2,i2+3,i3+3,c3)+cr3*ui(i1+3,i2+3,i3+3,c3)+
     & cr4*ui(i1+4,i2+3,i3+3,c3))+cs4*(cr0*ui(i1,i2+4,i3+3,c3)+cr1*ui(
     & i1+1,i2+4,i3+3,c3)+cr2*ui(i1+2,i2+4,i3+3,c3)+cr3*ui(i1+3,i2+4,
     & i3+3,c3)+cr4*ui(i1+4,i2+4,i3+3,c3)))+ct4*(cs0*(cr0*ui(i1,i2,i3+
     & 4,c3)+cr1*ui(i1+1,i2,i3+4,c3)+cr2*ui(i1+2,i2,i3+4,c3)+cr3*ui(
     & i1+3,i2,i3+4,c3)+cr4*ui(i1+4,i2,i3+4,c3))+cs1*(cr0*ui(i1,i2+1,
     & i3+4,c3)+cr1*ui(i1+1,i2+1,i3+4,c3)+cr2*ui(i1+2,i2+1,i3+4,c3)+
     & cr3*ui(i1+3,i2+1,i3+4,c3)+cr4*ui(i1+4,i2+1,i3+4,c3))+cs2*(cr0*
     & ui(i1,i2+2,i3+4,c3)+cr1*ui(i1+1,i2+2,i3+4,c3)+cr2*ui(i1+2,i2+2,
     & i3+4,c3)+cr3*ui(i1+3,i2+2,i3+4,c3)+cr4*ui(i1+4,i2+2,i3+4,c3))+
     & cs3*(cr0*ui(i1,i2+3,i3+4,c3)+cr1*ui(i1+1,i2+3,i3+4,c3)+cr2*ui(
     & i1+2,i2+3,i3+4,c3)+cr3*ui(i1+3,i2+3,i3+4,c3)+cr4*ui(i1+4,i2+3,
     & i3+4,c3))+cs4*(cr0*ui(i1,i2+4,i3+4,c3)+cr1*ui(i1+1,i2+4,i3+4,
     & c3)+cr2*ui(i1+2,i2+4,i3+4,c3)+cr3*ui(i1+3,i2+4,i3+4,c3)+cr4*ui(
     & i1+4,i2+4,i3+4,c3)))


             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
! interpSparseStorage555(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q50(c(i,0,0,0))
                 cs0 = q50(c(i,1,0,0))
                 ct0 = q50(c(i,2,0,0))
                 cr1 = q51(c(i,0,0,0))
                 cs1 = q51(c(i,1,0,0))
                 ct1 = q51(c(i,2,0,0))
                 cr2 = q52(c(i,0,0,0))
                 cs2 = q52(c(i,1,0,0))
                 ct2 = q52(c(i,2,0,0))
                 cr3 = q53(c(i,0,0,0))
                 cs3 = q53(c(i,1,0,0))
                 ct3 = q53(c(i,2,0,0))
                 cr4 = q54(c(i,0,0,0))
                 cs4 = q54(c(i,1,0,0))
                 ct4 = q54(c(i,2,0,0))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ct0*(cs0*(cr0*ui(i1,i2,
     & i3+0,c3)+cr1*ui(i1+1,i2,i3+0,c3)+cr2*ui(i1+2,i2,i3+0,c3)+cr3*
     & ui(i1+3,i2,i3+0,c3)+cr4*ui(i1+4,i2,i3+0,c3))+cs1*(cr0*ui(i1,i2+
     & 1,i3+0,c3)+cr1*ui(i1+1,i2+1,i3+0,c3)+cr2*ui(i1+2,i2+1,i3+0,c3)+
     & cr3*ui(i1+3,i2+1,i3+0,c3)+cr4*ui(i1+4,i2+1,i3+0,c3))+cs2*(cr0*
     & ui(i1,i2+2,i3+0,c3)+cr1*ui(i1+1,i2+2,i3+0,c3)+cr2*ui(i1+2,i2+2,
     & i3+0,c3)+cr3*ui(i1+3,i2+2,i3+0,c3)+cr4*ui(i1+4,i2+2,i3+0,c3))+
     & cs3*(cr0*ui(i1,i2+3,i3+0,c3)+cr1*ui(i1+1,i2+3,i3+0,c3)+cr2*ui(
     & i1+2,i2+3,i3+0,c3)+cr3*ui(i1+3,i2+3,i3+0,c3)+cr4*ui(i1+4,i2+3,
     & i3+0,c3))+cs4*(cr0*ui(i1,i2+4,i3+0,c3)+cr1*ui(i1+1,i2+4,i3+0,
     & c3)+cr2*ui(i1+2,i2+4,i3+0,c3)+cr3*ui(i1+3,i2+4,i3+0,c3)+cr4*ui(
     & i1+4,i2+4,i3+0,c3)))+ct1*(cs0*(cr0*ui(i1,i2,i3+1,c3)+cr1*ui(i1+
     & 1,i2,i3+1,c3)+cr2*ui(i1+2,i2,i3+1,c3)+cr3*ui(i1+3,i2,i3+1,c3)+
     & cr4*ui(i1+4,i2,i3+1,c3))+cs1*(cr0*ui(i1,i2+1,i3+1,c3)+cr1*ui(
     & i1+1,i2+1,i3+1,c3)+cr2*ui(i1+2,i2+1,i3+1,c3)+cr3*ui(i1+3,i2+1,
     & i3+1,c3)+cr4*ui(i1+4,i2+1,i3+1,c3))+cs2*(cr0*ui(i1,i2+2,i3+1,
     & c3)+cr1*ui(i1+1,i2+2,i3+1,c3)+cr2*ui(i1+2,i2+2,i3+1,c3)+cr3*ui(
     & i1+3,i2+2,i3+1,c3)+cr4*ui(i1+4,i2+2,i3+1,c3))+cs3*(cr0*ui(i1,
     & i2+3,i3+1,c3)+cr1*ui(i1+1,i2+3,i3+1,c3)+cr2*ui(i1+2,i2+3,i3+1,
     & c3)+cr3*ui(i1+3,i2+3,i3+1,c3)+cr4*ui(i1+4,i2+3,i3+1,c3))+cs4*(
     & cr0*ui(i1,i2+4,i3+1,c3)+cr1*ui(i1+1,i2+4,i3+1,c3)+cr2*ui(i1+2,
     & i2+4,i3+1,c3)+cr3*ui(i1+3,i2+4,i3+1,c3)+cr4*ui(i1+4,i2+4,i3+1,
     & c3)))+ct2*(cs0*(cr0*ui(i1,i2,i3+2,c3)+cr1*ui(i1+1,i2,i3+2,c3)+
     & cr2*ui(i1+2,i2,i3+2,c3)+cr3*ui(i1+3,i2,i3+2,c3)+cr4*ui(i1+4,i2,
     & i3+2,c3))+cs1*(cr0*ui(i1,i2+1,i3+2,c3)+cr1*ui(i1+1,i2+1,i3+2,
     & c3)+cr2*ui(i1+2,i2+1,i3+2,c3)+cr3*ui(i1+3,i2+1,i3+2,c3)+cr4*ui(
     & i1+4,i2+1,i3+2,c3))+cs2*(cr0*ui(i1,i2+2,i3+2,c3)+cr1*ui(i1+1,
     & i2+2,i3+2,c3)+cr2*ui(i1+2,i2+2,i3+2,c3)+cr3*ui(i1+3,i2+2,i3+2,
     & c3)+cr4*ui(i1+4,i2+2,i3+2,c3))+cs3*(cr0*ui(i1,i2+3,i3+2,c3)+
     & cr1*ui(i1+1,i2+3,i3+2,c3)+cr2*ui(i1+2,i2+3,i3+2,c3)+cr3*ui(i1+
     & 3,i2+3,i3+2,c3)+cr4*ui(i1+4,i2+3,i3+2,c3))+cs4*(cr0*ui(i1,i2+4,
     & i3+2,c3)+cr1*ui(i1+1,i2+4,i3+2,c3)+cr2*ui(i1+2,i2+4,i3+2,c3)+
     & cr3*ui(i1+3,i2+4,i3+2,c3)+cr4*ui(i1+4,i2+4,i3+2,c3)))+ct3*(cs0*
     & (cr0*ui(i1,i2,i3+3,c3)+cr1*ui(i1+1,i2,i3+3,c3)+cr2*ui(i1+2,i2,
     & i3+3,c3)+cr3*ui(i1+3,i2,i3+3,c3)+cr4*ui(i1+4,i2,i3+3,c3))+cs1*(
     & cr0*ui(i1,i2+1,i3+3,c3)+cr1*ui(i1+1,i2+1,i3+3,c3)+cr2*ui(i1+2,
     & i2+1,i3+3,c3)+cr3*ui(i1+3,i2+1,i3+3,c3)+cr4*ui(i1+4,i2+1,i3+3,
     & c3))+cs2*(cr0*ui(i1,i2+2,i3+3,c3)+cr1*ui(i1+1,i2+2,i3+3,c3)+
     & cr2*ui(i1+2,i2+2,i3+3,c3)+cr3*ui(i1+3,i2+2,i3+3,c3)+cr4*ui(i1+
     & 4,i2+2,i3+3,c3))+cs3*(cr0*ui(i1,i2+3,i3+3,c3)+cr1*ui(i1+1,i2+3,
     & i3+3,c3)+cr2*ui(i1+2,i2+3,i3+3,c3)+cr3*ui(i1+3,i2+3,i3+3,c3)+
     & cr4*ui(i1+4,i2+3,i3+3,c3))+cs4*(cr0*ui(i1,i2+4,i3+3,c3)+cr1*ui(
     & i1+1,i2+4,i3+3,c3)+cr2*ui(i1+2,i2+4,i3+3,c3)+cr3*ui(i1+3,i2+4,
     & i3+3,c3)+cr4*ui(i1+4,i2+4,i3+3,c3)))+ct4*(cs0*(cr0*ui(i1,i2,i3+
     & 4,c3)+cr1*ui(i1+1,i2,i3+4,c3)+cr2*ui(i1+2,i2,i3+4,c3)+cr3*ui(
     & i1+3,i2,i3+4,c3)+cr4*ui(i1+4,i2,i3+4,c3))+cs1*(cr0*ui(i1,i2+1,
     & i3+4,c3)+cr1*ui(i1+1,i2+1,i3+4,c3)+cr2*ui(i1+2,i2+1,i3+4,c3)+
     & cr3*ui(i1+3,i2+1,i3+4,c3)+cr4*ui(i1+4,i2+1,i3+4,c3))+cs2*(cr0*
     & ui(i1,i2+2,i3+4,c3)+cr1*ui(i1+1,i2+2,i3+4,c3)+cr2*ui(i1+2,i2+2,
     & i3+4,c3)+cr3*ui(i1+3,i2+2,i3+4,c3)+cr4*ui(i1+4,i2+2,i3+4,c3))+
     & cs3*(cr0*ui(i1,i2+3,i3+4,c3)+cr1*ui(i1+1,i2+3,i3+4,c3)+cr2*ui(
     & i1+2,i2+3,i3+4,c3)+cr3*ui(i1+3,i2+3,i3+4,c3)+cr4*ui(i1+4,i2+3,
     & i3+4,c3))+cs4*(cr0*ui(i1,i2+4,i3+4,c3)+cr1*ui(i1+1,i2+4,i3+4,
     & c3)+cr2*ui(i1+2,i2+4,i3+4,c3)+cr3*ui(i1+3,i2+4,i3+4,c3)+cr4*ui(
     & i1+4,i2+4,i3+4,c3)))


             end do
             end do
           end if
         else if( width(1).eq.6 .and. width(2).eq.6 .and. width(3)
     & .eq.6 )then
! loops3d($interpSparseStorage666(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)
           if( c3a.eq.c3b )then
             do c3=c3a,c3b
             do i=nia,nib
! interpSparseStorage666(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q60(c(i,0,0,0))
                 cs0 = q60(c(i,1,0,0))
                 ct0 = q60(c(i,2,0,0))
                 cr1 = q61(c(i,0,0,0))
                 cs1 = q61(c(i,1,0,0))
                 ct1 = q61(c(i,2,0,0))
                 cr2 = q62(c(i,0,0,0))
                 cs2 = q62(c(i,1,0,0))
                 ct2 = q62(c(i,2,0,0))
                 cr3 = q63(c(i,0,0,0))
                 cs3 = q63(c(i,1,0,0))
                 ct3 = q63(c(i,2,0,0))
                 cr4 = q64(c(i,0,0,0))
                 cs4 = q64(c(i,1,0,0))
                 ct4 = q64(c(i,2,0,0))
                 cr5 = q65(c(i,0,0,0))
                 cs5 = q65(c(i,1,0,0))
                 ct5 = q65(c(i,2,0,0))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ct0*(cs0*(cr0*ui(i1,i2,
     & i3+0,c3)+cr1*ui(i1+1,i2,i3+0,c3)+cr2*ui(i1+2,i2,i3+0,c3)+cr3*
     & ui(i1+3,i2,i3+0,c3)+cr4*ui(i1+4,i2,i3+0,c3)+cr5*ui(i1+5,i2,i3+
     & 0,c3))+cs1*(cr0*ui(i1,i2+1,i3+0,c3)+cr1*ui(i1+1,i2+1,i3+0,c3)+
     & cr2*ui(i1+2,i2+1,i3+0,c3)+cr3*ui(i1+3,i2+1,i3+0,c3)+cr4*ui(i1+
     & 4,i2+1,i3+0,c3)+cr5*ui(i1+5,i2+1,i3+0,c3))+cs2*(cr0*ui(i1,i2+2,
     & i3+0,c3)+cr1*ui(i1+1,i2+2,i3+0,c3)+cr2*ui(i1+2,i2+2,i3+0,c3)+
     & cr3*ui(i1+3,i2+2,i3+0,c3)+cr4*ui(i1+4,i2+2,i3+0,c3)+cr5*ui(i1+
     & 5,i2+2,i3+0,c3))+cs3*(cr0*ui(i1,i2+3,i3+0,c3)+cr1*ui(i1+1,i2+3,
     & i3+0,c3)+cr2*ui(i1+2,i2+3,i3+0,c3)+cr3*ui(i1+3,i2+3,i3+0,c3)+
     & cr4*ui(i1+4,i2+3,i3+0,c3)+cr5*ui(i1+5,i2+3,i3+0,c3))+cs4*(cr0*
     & ui(i1,i2+4,i3+0,c3)+cr1*ui(i1+1,i2+4,i3+0,c3)+cr2*ui(i1+2,i2+4,
     & i3+0,c3)+cr3*ui(i1+3,i2+4,i3+0,c3)+cr4*ui(i1+4,i2+4,i3+0,c3)+
     & cr5*ui(i1+5,i2+4,i3+0,c3))+cs5*(cr0*ui(i1,i2+5,i3+0,c3)+cr1*ui(
     & i1+1,i2+5,i3+0,c3)+cr2*ui(i1+2,i2+5,i3+0,c3)+cr3*ui(i1+3,i2+5,
     & i3+0,c3)+cr4*ui(i1+4,i2+5,i3+0,c3)+cr5*ui(i1+5,i2+5,i3+0,c3)))+
     & ct1*(cs0*(cr0*ui(i1,i2,i3+1,c3)+cr1*ui(i1+1,i2,i3+1,c3)+cr2*ui(
     & i1+2,i2,i3+1,c3)+cr3*ui(i1+3,i2,i3+1,c3)+cr4*ui(i1+4,i2,i3+1,
     & c3)+cr5*ui(i1+5,i2,i3+1,c3))+cs1*(cr0*ui(i1,i2+1,i3+1,c3)+cr1*
     & ui(i1+1,i2+1,i3+1,c3)+cr2*ui(i1+2,i2+1,i3+1,c3)+cr3*ui(i1+3,i2+
     & 1,i3+1,c3)+cr4*ui(i1+4,i2+1,i3+1,c3)+cr5*ui(i1+5,i2+1,i3+1,c3))
     & +cs2*(cr0*ui(i1,i2+2,i3+1,c3)+cr1*ui(i1+1,i2+2,i3+1,c3)+cr2*ui(
     & i1+2,i2+2,i3+1,c3)+cr3*ui(i1+3,i2+2,i3+1,c3)+cr4*ui(i1+4,i2+2,
     & i3+1,c3)+cr5*ui(i1+5,i2+2,i3+1,c3))+cs3*(cr0*ui(i1,i2+3,i3+1,
     & c3)+cr1*ui(i1+1,i2+3,i3+1,c3)+cr2*ui(i1+2,i2+3,i3+1,c3)+cr3*ui(
     & i1+3,i2+3,i3+1,c3)+cr4*ui(i1+4,i2+3,i3+1,c3)+cr5*ui(i1+5,i2+3,
     & i3+1,c3))+cs4*(cr0*ui(i1,i2+4,i3+1,c3)+cr1*ui(i1+1,i2+4,i3+1,
     & c3)+cr2*ui(i1+2,i2+4,i3+1,c3)+cr3*ui(i1+3,i2+4,i3+1,c3)+cr4*ui(
     & i1+4,i2+4,i3+1,c3)+cr5*ui(i1+5,i2+4,i3+1,c3))+cs5*(cr0*ui(i1,
     & i2+5,i3+1,c3)+cr1*ui(i1+1,i2+5,i3+1,c3)+cr2*ui(i1+2,i2+5,i3+1,
     & c3)+cr3*ui(i1+3,i2+5,i3+1,c3)+cr4*ui(i1+4,i2+5,i3+1,c3)+cr5*ui(
     & i1+5,i2+5,i3+1,c3)))+ct2*(cs0*(cr0*ui(i1,i2,i3+2,c3)+cr1*ui(i1+
     & 1,i2,i3+2,c3)+cr2*ui(i1+2,i2,i3+2,c3)+cr3*ui(i1+3,i2,i3+2,c3)+
     & cr4*ui(i1+4,i2,i3+2,c3)+cr5*ui(i1+5,i2,i3+2,c3))+cs1*(cr0*ui(
     & i1,i2+1,i3+2,c3)+cr1*ui(i1+1,i2+1,i3+2,c3)+cr2*ui(i1+2,i2+1,i3+
     & 2,c3)+cr3*ui(i1+3,i2+1,i3+2,c3)+cr4*ui(i1+4,i2+1,i3+2,c3)+cr5*
     & ui(i1+5,i2+1,i3+2,c3))+cs2*(cr0*ui(i1,i2+2,i3+2,c3)+cr1*ui(i1+
     & 1,i2+2,i3+2,c3)+cr2*ui(i1+2,i2+2,i3+2,c3)+cr3*ui(i1+3,i2+2,i3+
     & 2,c3)+cr4*ui(i1+4,i2+2,i3+2,c3)+cr5*ui(i1+5,i2+2,i3+2,c3))+cs3*
     & (cr0*ui(i1,i2+3,i3+2,c3)+cr1*ui(i1+1,i2+3,i3+2,c3)+cr2*ui(i1+2,
     & i2+3,i3+2,c3)+cr3*ui(i1+3,i2+3,i3+2,c3)+cr4*ui(i1+4,i2+3,i3+2,
     & c3)+cr5*ui(i1+5,i2+3,i3+2,c3))+cs4*(cr0*ui(i1,i2+4,i3+2,c3)+
     & cr1*ui(i1+1,i2+4,i3+2,c3)+cr2*ui(i1+2,i2+4,i3+2,c3)+cr3*ui(i1+
     & 3,i2+4,i3+2,c3)+cr4*ui(i1+4,i2+4,i3+2,c3)+cr5*ui(i1+5,i2+4,i3+
     & 2,c3))+cs5*(cr0*ui(i1,i2+5,i3+2,c3)+cr1*ui(i1+1,i2+5,i3+2,c3)+
     & cr2*ui(i1+2,i2+5,i3+2,c3)+cr3*ui(i1+3,i2+5,i3+2,c3)+cr4*ui(i1+
     & 4,i2+5,i3+2,c3)+cr5*ui(i1+5,i2+5,i3+2,c3)))+ct3*(cs0*(cr0*ui(
     & i1,i2,i3+3,c3)+cr1*ui(i1+1,i2,i3+3,c3)+cr2*ui(i1+2,i2,i3+3,c3)+
     & cr3*ui(i1+3,i2,i3+3,c3)+cr4*ui(i1+4,i2,i3+3,c3)+cr5*ui(i1+5,i2,
     & i3+3,c3))+cs1*(cr0*ui(i1,i2+1,i3+3,c3)+cr1*ui(i1+1,i2+1,i3+3,
     & c3)+cr2*ui(i1+2,i2+1,i3+3,c3)+cr3*ui(i1+3,i2+1,i3+3,c3)+cr4*ui(
     & i1+4,i2+1,i3+3,c3)+cr5*ui(i1+5,i2+1,i3+3,c3))+cs2*(cr0*ui(i1,
     & i2+2,i3+3,c3)+cr1*ui(i1+1,i2+2,i3+3,c3)+cr2*ui(i1+2,i2+2,i3+3,
     & c3)+cr3*ui(i1+3,i2+2,i3+3,c3)+cr4*ui(i1+4,i2+2,i3+3,c3)+cr5*ui(
     & i1+5,i2+2,i3+3,c3))+cs3*(cr0*ui(i1,i2+3,i3+3,c3)+cr1*ui(i1+1,
     & i2+3,i3+3,c3)+cr2*ui(i1+2,i2+3,i3+3,c3)+cr3*ui(i1+3,i2+3,i3+3,
     & c3)+cr4*ui(i1+4,i2+3,i3+3,c3)+cr5*ui(i1+5,i2+3,i3+3,c3))+cs4*(
     & cr0*ui(i1,i2+4,i3+3,c3)+cr1*ui(i1+1,i2+4,i3+3,c3)+cr2*ui(i1+2,
     & i2+4,i3+3,c3)+cr3*ui(i1+3,i2+4,i3+3,c3)+cr4*ui(i1+4,i2+4,i3+3,
     & c3)+cr5*ui(i1+5,i2+4,i3+3,c3))+cs5*(cr0*ui(i1,i2+5,i3+3,c3)+
     & cr1*ui(i1+1,i2+5,i3+3,c3)+cr2*ui(i1+2,i2+5,i3+3,c3)+cr3*ui(i1+
     & 3,i2+5,i3+3,c3)+cr4*ui(i1+4,i2+5,i3+3,c3)+cr5*ui(i1+5,i2+5,i3+
     & 3,c3)))+ct4*(cs0*(cr0*ui(i1,i2,i3+4,c3)+cr1*ui(i1+1,i2,i3+4,c3)
     & +cr2*ui(i1+2,i2,i3+4,c3)+cr3*ui(i1+3,i2,i3+4,c3)+cr4*ui(i1+4,
     & i2,i3+4,c3)+cr5*ui(i1+5,i2,i3+4,c3))+cs1*(cr0*ui(i1,i2+1,i3+4,
     & c3)+cr1*ui(i1+1,i2+1,i3+4,c3)+cr2*ui(i1+2,i2+1,i3+4,c3)+cr3*ui(
     & i1+3,i2+1,i3+4,c3)+cr4*ui(i1+4,i2+1,i3+4,c3)+cr5*ui(i1+5,i2+1,
     & i3+4,c3))+cs2*(cr0*ui(i1,i2+2,i3+4,c3)+cr1*ui(i1+1,i2+2,i3+4,
     & c3)+cr2*ui(i1+2,i2+2,i3+4,c3)+cr3*ui(i1+3,i2+2,i3+4,c3)+cr4*ui(
     & i1+4,i2+2,i3+4,c3)+cr5*ui(i1+5,i2+2,i3+4,c3))+cs3*(cr0*ui(i1,
     & i2+3,i3+4,c3)+cr1*ui(i1+1,i2+3,i3+4,c3)+cr2*ui(i1+2,i2+3,i3+4,
     & c3)+cr3*ui(i1+3,i2+3,i3+4,c3)+cr4*ui(i1+4,i2+3,i3+4,c3)+cr5*ui(
     & i1+5,i2+3,i3+4,c3))+cs4*(cr0*ui(i1,i2+4,i3+4,c3)+cr1*ui(i1+1,
     & i2+4,i3+4,c3)+cr2*ui(i1+2,i2+4,i3+4,c3)+cr3*ui(i1+3,i2+4,i3+4,
     & c3)+cr4*ui(i1+4,i2+4,i3+4,c3)+cr5*ui(i1+5,i2+4,i3+4,c3))+cs5*(
     & cr0*ui(i1,i2+5,i3+4,c3)+cr1*ui(i1+1,i2+5,i3+4,c3)+cr2*ui(i1+2,
     & i2+5,i3+4,c3)+cr3*ui(i1+3,i2+5,i3+4,c3)+cr4*ui(i1+4,i2+5,i3+4,
     & c3)+cr5*ui(i1+5,i2+5,i3+4,c3)))+ct5*(cs0*(cr0*ui(i1,i2,i3+5,c3)
     & +cr1*ui(i1+1,i2,i3+5,c3)+cr2*ui(i1+2,i2,i3+5,c3)+cr3*ui(i1+3,
     & i2,i3+5,c3)+cr4*ui(i1+4,i2,i3+5,c3)+cr5*ui(i1+5,i2,i3+5,c3))+
     & cs1*(cr0*ui(i1,i2+1,i3+5,c3)+cr1*ui(i1+1,i2+1,i3+5,c3)+cr2*ui(
     & i1+2,i2+1,i3+5,c3)+cr3*ui(i1+3,i2+1,i3+5,c3)+cr4*ui(i1+4,i2+1,
     & i3+5,c3)+cr5*ui(i1+5,i2+1,i3+5,c3))+cs2*(cr0*ui(i1,i2+2,i3+5,
     & c3)+cr1*ui(i1+1,i2+2,i3+5,c3)+cr2*ui(i1+2,i2+2,i3+5,c3)+cr3*ui(
     & i1+3,i2+2,i3+5,c3)+cr4*ui(i1+4,i2+2,i3+5,c3)+cr5*ui(i1+5,i2+2,
     & i3+5,c3))+cs3*(cr0*ui(i1,i2+3,i3+5,c3)+cr1*ui(i1+1,i2+3,i3+5,
     & c3)+cr2*ui(i1+2,i2+3,i3+5,c3)+cr3*ui(i1+3,i2+3,i3+5,c3)+cr4*ui(
     & i1+4,i2+3,i3+5,c3)+cr5*ui(i1+5,i2+3,i3+5,c3))+cs4*(cr0*ui(i1,
     & i2+4,i3+5,c3)+cr1*ui(i1+1,i2+4,i3+5,c3)+cr2*ui(i1+2,i2+4,i3+5,
     & c3)+cr3*ui(i1+3,i2+4,i3+5,c3)+cr4*ui(i1+4,i2+4,i3+5,c3)+cr5*ui(
     & i1+5,i2+4,i3+5,c3))+cs5*(cr0*ui(i1,i2+5,i3+5,c3)+cr1*ui(i1+1,
     & i2+5,i3+5,c3)+cr2*ui(i1+2,i2+5,i3+5,c3)+cr3*ui(i1+3,i2+5,i3+5,
     & c3)+cr4*ui(i1+4,i2+5,i3+5,c3)+cr5*ui(i1+5,i2+5,i3+5,c3)))


             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
! interpSparseStorage666(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q60(c(i,0,0,0))
                 cs0 = q60(c(i,1,0,0))
                 ct0 = q60(c(i,2,0,0))
                 cr1 = q61(c(i,0,0,0))
                 cs1 = q61(c(i,1,0,0))
                 ct1 = q61(c(i,2,0,0))
                 cr2 = q62(c(i,0,0,0))
                 cs2 = q62(c(i,1,0,0))
                 ct2 = q62(c(i,2,0,0))
                 cr3 = q63(c(i,0,0,0))
                 cs3 = q63(c(i,1,0,0))
                 ct3 = q63(c(i,2,0,0))
                 cr4 = q64(c(i,0,0,0))
                 cs4 = q64(c(i,1,0,0))
                 ct4 = q64(c(i,2,0,0))
                 cr5 = q65(c(i,0,0,0))
                 cs5 = q65(c(i,1,0,0))
                 ct5 = q65(c(i,2,0,0))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ct0*(cs0*(cr0*ui(i1,i2,
     & i3+0,c3)+cr1*ui(i1+1,i2,i3+0,c3)+cr2*ui(i1+2,i2,i3+0,c3)+cr3*
     & ui(i1+3,i2,i3+0,c3)+cr4*ui(i1+4,i2,i3+0,c3)+cr5*ui(i1+5,i2,i3+
     & 0,c3))+cs1*(cr0*ui(i1,i2+1,i3+0,c3)+cr1*ui(i1+1,i2+1,i3+0,c3)+
     & cr2*ui(i1+2,i2+1,i3+0,c3)+cr3*ui(i1+3,i2+1,i3+0,c3)+cr4*ui(i1+
     & 4,i2+1,i3+0,c3)+cr5*ui(i1+5,i2+1,i3+0,c3))+cs2*(cr0*ui(i1,i2+2,
     & i3+0,c3)+cr1*ui(i1+1,i2+2,i3+0,c3)+cr2*ui(i1+2,i2+2,i3+0,c3)+
     & cr3*ui(i1+3,i2+2,i3+0,c3)+cr4*ui(i1+4,i2+2,i3+0,c3)+cr5*ui(i1+
     & 5,i2+2,i3+0,c3))+cs3*(cr0*ui(i1,i2+3,i3+0,c3)+cr1*ui(i1+1,i2+3,
     & i3+0,c3)+cr2*ui(i1+2,i2+3,i3+0,c3)+cr3*ui(i1+3,i2+3,i3+0,c3)+
     & cr4*ui(i1+4,i2+3,i3+0,c3)+cr5*ui(i1+5,i2+3,i3+0,c3))+cs4*(cr0*
     & ui(i1,i2+4,i3+0,c3)+cr1*ui(i1+1,i2+4,i3+0,c3)+cr2*ui(i1+2,i2+4,
     & i3+0,c3)+cr3*ui(i1+3,i2+4,i3+0,c3)+cr4*ui(i1+4,i2+4,i3+0,c3)+
     & cr5*ui(i1+5,i2+4,i3+0,c3))+cs5*(cr0*ui(i1,i2+5,i3+0,c3)+cr1*ui(
     & i1+1,i2+5,i3+0,c3)+cr2*ui(i1+2,i2+5,i3+0,c3)+cr3*ui(i1+3,i2+5,
     & i3+0,c3)+cr4*ui(i1+4,i2+5,i3+0,c3)+cr5*ui(i1+5,i2+5,i3+0,c3)))+
     & ct1*(cs0*(cr0*ui(i1,i2,i3+1,c3)+cr1*ui(i1+1,i2,i3+1,c3)+cr2*ui(
     & i1+2,i2,i3+1,c3)+cr3*ui(i1+3,i2,i3+1,c3)+cr4*ui(i1+4,i2,i3+1,
     & c3)+cr5*ui(i1+5,i2,i3+1,c3))+cs1*(cr0*ui(i1,i2+1,i3+1,c3)+cr1*
     & ui(i1+1,i2+1,i3+1,c3)+cr2*ui(i1+2,i2+1,i3+1,c3)+cr3*ui(i1+3,i2+
     & 1,i3+1,c3)+cr4*ui(i1+4,i2+1,i3+1,c3)+cr5*ui(i1+5,i2+1,i3+1,c3))
     & +cs2*(cr0*ui(i1,i2+2,i3+1,c3)+cr1*ui(i1+1,i2+2,i3+1,c3)+cr2*ui(
     & i1+2,i2+2,i3+1,c3)+cr3*ui(i1+3,i2+2,i3+1,c3)+cr4*ui(i1+4,i2+2,
     & i3+1,c3)+cr5*ui(i1+5,i2+2,i3+1,c3))+cs3*(cr0*ui(i1,i2+3,i3+1,
     & c3)+cr1*ui(i1+1,i2+3,i3+1,c3)+cr2*ui(i1+2,i2+3,i3+1,c3)+cr3*ui(
     & i1+3,i2+3,i3+1,c3)+cr4*ui(i1+4,i2+3,i3+1,c3)+cr5*ui(i1+5,i2+3,
     & i3+1,c3))+cs4*(cr0*ui(i1,i2+4,i3+1,c3)+cr1*ui(i1+1,i2+4,i3+1,
     & c3)+cr2*ui(i1+2,i2+4,i3+1,c3)+cr3*ui(i1+3,i2+4,i3+1,c3)+cr4*ui(
     & i1+4,i2+4,i3+1,c3)+cr5*ui(i1+5,i2+4,i3+1,c3))+cs5*(cr0*ui(i1,
     & i2+5,i3+1,c3)+cr1*ui(i1+1,i2+5,i3+1,c3)+cr2*ui(i1+2,i2+5,i3+1,
     & c3)+cr3*ui(i1+3,i2+5,i3+1,c3)+cr4*ui(i1+4,i2+5,i3+1,c3)+cr5*ui(
     & i1+5,i2+5,i3+1,c3)))+ct2*(cs0*(cr0*ui(i1,i2,i3+2,c3)+cr1*ui(i1+
     & 1,i2,i3+2,c3)+cr2*ui(i1+2,i2,i3+2,c3)+cr3*ui(i1+3,i2,i3+2,c3)+
     & cr4*ui(i1+4,i2,i3+2,c3)+cr5*ui(i1+5,i2,i3+2,c3))+cs1*(cr0*ui(
     & i1,i2+1,i3+2,c3)+cr1*ui(i1+1,i2+1,i3+2,c3)+cr2*ui(i1+2,i2+1,i3+
     & 2,c3)+cr3*ui(i1+3,i2+1,i3+2,c3)+cr4*ui(i1+4,i2+1,i3+2,c3)+cr5*
     & ui(i1+5,i2+1,i3+2,c3))+cs2*(cr0*ui(i1,i2+2,i3+2,c3)+cr1*ui(i1+
     & 1,i2+2,i3+2,c3)+cr2*ui(i1+2,i2+2,i3+2,c3)+cr3*ui(i1+3,i2+2,i3+
     & 2,c3)+cr4*ui(i1+4,i2+2,i3+2,c3)+cr5*ui(i1+5,i2+2,i3+2,c3))+cs3*
     & (cr0*ui(i1,i2+3,i3+2,c3)+cr1*ui(i1+1,i2+3,i3+2,c3)+cr2*ui(i1+2,
     & i2+3,i3+2,c3)+cr3*ui(i1+3,i2+3,i3+2,c3)+cr4*ui(i1+4,i2+3,i3+2,
     & c3)+cr5*ui(i1+5,i2+3,i3+2,c3))+cs4*(cr0*ui(i1,i2+4,i3+2,c3)+
     & cr1*ui(i1+1,i2+4,i3+2,c3)+cr2*ui(i1+2,i2+4,i3+2,c3)+cr3*ui(i1+
     & 3,i2+4,i3+2,c3)+cr4*ui(i1+4,i2+4,i3+2,c3)+cr5*ui(i1+5,i2+4,i3+
     & 2,c3))+cs5*(cr0*ui(i1,i2+5,i3+2,c3)+cr1*ui(i1+1,i2+5,i3+2,c3)+
     & cr2*ui(i1+2,i2+5,i3+2,c3)+cr3*ui(i1+3,i2+5,i3+2,c3)+cr4*ui(i1+
     & 4,i2+5,i3+2,c3)+cr5*ui(i1+5,i2+5,i3+2,c3)))+ct3*(cs0*(cr0*ui(
     & i1,i2,i3+3,c3)+cr1*ui(i1+1,i2,i3+3,c3)+cr2*ui(i1+2,i2,i3+3,c3)+
     & cr3*ui(i1+3,i2,i3+3,c3)+cr4*ui(i1+4,i2,i3+3,c3)+cr5*ui(i1+5,i2,
     & i3+3,c3))+cs1*(cr0*ui(i1,i2+1,i3+3,c3)+cr1*ui(i1+1,i2+1,i3+3,
     & c3)+cr2*ui(i1+2,i2+1,i3+3,c3)+cr3*ui(i1+3,i2+1,i3+3,c3)+cr4*ui(
     & i1+4,i2+1,i3+3,c3)+cr5*ui(i1+5,i2+1,i3+3,c3))+cs2*(cr0*ui(i1,
     & i2+2,i3+3,c3)+cr1*ui(i1+1,i2+2,i3+3,c3)+cr2*ui(i1+2,i2+2,i3+3,
     & c3)+cr3*ui(i1+3,i2+2,i3+3,c3)+cr4*ui(i1+4,i2+2,i3+3,c3)+cr5*ui(
     & i1+5,i2+2,i3+3,c3))+cs3*(cr0*ui(i1,i2+3,i3+3,c3)+cr1*ui(i1+1,
     & i2+3,i3+3,c3)+cr2*ui(i1+2,i2+3,i3+3,c3)+cr3*ui(i1+3,i2+3,i3+3,
     & c3)+cr4*ui(i1+4,i2+3,i3+3,c3)+cr5*ui(i1+5,i2+3,i3+3,c3))+cs4*(
     & cr0*ui(i1,i2+4,i3+3,c3)+cr1*ui(i1+1,i2+4,i3+3,c3)+cr2*ui(i1+2,
     & i2+4,i3+3,c3)+cr3*ui(i1+3,i2+4,i3+3,c3)+cr4*ui(i1+4,i2+4,i3+3,
     & c3)+cr5*ui(i1+5,i2+4,i3+3,c3))+cs5*(cr0*ui(i1,i2+5,i3+3,c3)+
     & cr1*ui(i1+1,i2+5,i3+3,c3)+cr2*ui(i1+2,i2+5,i3+3,c3)+cr3*ui(i1+
     & 3,i2+5,i3+3,c3)+cr4*ui(i1+4,i2+5,i3+3,c3)+cr5*ui(i1+5,i2+5,i3+
     & 3,c3)))+ct4*(cs0*(cr0*ui(i1,i2,i3+4,c3)+cr1*ui(i1+1,i2,i3+4,c3)
     & +cr2*ui(i1+2,i2,i3+4,c3)+cr3*ui(i1+3,i2,i3+4,c3)+cr4*ui(i1+4,
     & i2,i3+4,c3)+cr5*ui(i1+5,i2,i3+4,c3))+cs1*(cr0*ui(i1,i2+1,i3+4,
     & c3)+cr1*ui(i1+1,i2+1,i3+4,c3)+cr2*ui(i1+2,i2+1,i3+4,c3)+cr3*ui(
     & i1+3,i2+1,i3+4,c3)+cr4*ui(i1+4,i2+1,i3+4,c3)+cr5*ui(i1+5,i2+1,
     & i3+4,c3))+cs2*(cr0*ui(i1,i2+2,i3+4,c3)+cr1*ui(i1+1,i2+2,i3+4,
     & c3)+cr2*ui(i1+2,i2+2,i3+4,c3)+cr3*ui(i1+3,i2+2,i3+4,c3)+cr4*ui(
     & i1+4,i2+2,i3+4,c3)+cr5*ui(i1+5,i2+2,i3+4,c3))+cs3*(cr0*ui(i1,
     & i2+3,i3+4,c3)+cr1*ui(i1+1,i2+3,i3+4,c3)+cr2*ui(i1+2,i2+3,i3+4,
     & c3)+cr3*ui(i1+3,i2+3,i3+4,c3)+cr4*ui(i1+4,i2+3,i3+4,c3)+cr5*ui(
     & i1+5,i2+3,i3+4,c3))+cs4*(cr0*ui(i1,i2+4,i3+4,c3)+cr1*ui(i1+1,
     & i2+4,i3+4,c3)+cr2*ui(i1+2,i2+4,i3+4,c3)+cr3*ui(i1+3,i2+4,i3+4,
     & c3)+cr4*ui(i1+4,i2+4,i3+4,c3)+cr5*ui(i1+5,i2+4,i3+4,c3))+cs5*(
     & cr0*ui(i1,i2+5,i3+4,c3)+cr1*ui(i1+1,i2+5,i3+4,c3)+cr2*ui(i1+2,
     & i2+5,i3+4,c3)+cr3*ui(i1+3,i2+5,i3+4,c3)+cr4*ui(i1+4,i2+5,i3+4,
     & c3)+cr5*ui(i1+5,i2+5,i3+4,c3)))+ct5*(cs0*(cr0*ui(i1,i2,i3+5,c3)
     & +cr1*ui(i1+1,i2,i3+5,c3)+cr2*ui(i1+2,i2,i3+5,c3)+cr3*ui(i1+3,
     & i2,i3+5,c3)+cr4*ui(i1+4,i2,i3+5,c3)+cr5*ui(i1+5,i2,i3+5,c3))+
     & cs1*(cr0*ui(i1,i2+1,i3+5,c3)+cr1*ui(i1+1,i2+1,i3+5,c3)+cr2*ui(
     & i1+2,i2+1,i3+5,c3)+cr3*ui(i1+3,i2+1,i3+5,c3)+cr4*ui(i1+4,i2+1,
     & i3+5,c3)+cr5*ui(i1+5,i2+1,i3+5,c3))+cs2*(cr0*ui(i1,i2+2,i3+5,
     & c3)+cr1*ui(i1+1,i2+2,i3+5,c3)+cr2*ui(i1+2,i2+2,i3+5,c3)+cr3*ui(
     & i1+3,i2+2,i3+5,c3)+cr4*ui(i1+4,i2+2,i3+5,c3)+cr5*ui(i1+5,i2+2,
     & i3+5,c3))+cs3*(cr0*ui(i1,i2+3,i3+5,c3)+cr1*ui(i1+1,i2+3,i3+5,
     & c3)+cr2*ui(i1+2,i2+3,i3+5,c3)+cr3*ui(i1+3,i2+3,i3+5,c3)+cr4*ui(
     & i1+4,i2+3,i3+5,c3)+cr5*ui(i1+5,i2+3,i3+5,c3))+cs4*(cr0*ui(i1,
     & i2+4,i3+5,c3)+cr1*ui(i1+1,i2+4,i3+5,c3)+cr2*ui(i1+2,i2+4,i3+5,
     & c3)+cr3*ui(i1+3,i2+4,i3+5,c3)+cr4*ui(i1+4,i2+4,i3+5,c3)+cr5*ui(
     & i1+5,i2+4,i3+5,c3))+cs5*(cr0*ui(i1,i2+5,i3+5,c3)+cr1*ui(i1+1,
     & i2+5,i3+5,c3)+cr2*ui(i1+2,i2+5,i3+5,c3)+cr3*ui(i1+3,i2+5,i3+5,
     & c3)+cr4*ui(i1+4,i2+5,i3+5,c3)+cr5*ui(i1+5,i2+5,i3+5,c3)))


             end do
             end do
           end if
         else if( width(1).eq.7 .and. width(2).eq.7 .and. width(3)
     & .eq.7 )then
! loops3d($interpSparseStorage777(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)
           if( c3a.eq.c3b )then
             do c3=c3a,c3b
             do i=nia,nib
! interpSparseStorage777(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q70(c(i,0,0,0))
                 cs0 = q70(c(i,1,0,0))
                 ct0 = q70(c(i,2,0,0))
                 cr1 = q71(c(i,0,0,0))
                 cs1 = q71(c(i,1,0,0))
                 ct1 = q71(c(i,2,0,0))
                 cr2 = q72(c(i,0,0,0))
                 cs2 = q72(c(i,1,0,0))
                 ct2 = q72(c(i,2,0,0))
                 cr3 = q73(c(i,0,0,0))
                 cs3 = q73(c(i,1,0,0))
                 ct3 = q73(c(i,2,0,0))
                 cr4 = q74(c(i,0,0,0))
                 cs4 = q74(c(i,1,0,0))
                 ct4 = q74(c(i,2,0,0))
                 cr5 = q75(c(i,0,0,0))
                 cs5 = q75(c(i,1,0,0))
                 ct5 = q75(c(i,2,0,0))
                 cr6 = q76(c(i,0,0,0))
                 cs6 = q76(c(i,1,0,0))
                 ct6 = q76(c(i,2,0,0))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ct0*(cs0*(cr0*ui(i1,i2,
     & i3+0,c3)+cr1*ui(i1+1,i2,i3+0,c3)+cr2*ui(i1+2,i2,i3+0,c3)+cr3*
     & ui(i1+3,i2,i3+0,c3)+cr4*ui(i1+4,i2,i3+0,c3)+cr5*ui(i1+5,i2,i3+
     & 0,c3)+cr6*ui(i1+6,i2,i3+0,c3))+cs1*(cr0*ui(i1,i2+1,i3+0,c3)+
     & cr1*ui(i1+1,i2+1,i3+0,c3)+cr2*ui(i1+2,i2+1,i3+0,c3)+cr3*ui(i1+
     & 3,i2+1,i3+0,c3)+cr4*ui(i1+4,i2+1,i3+0,c3)+cr5*ui(i1+5,i2+1,i3+
     & 0,c3)+cr6*ui(i1+6,i2+1,i3+0,c3))+cs2*(cr0*ui(i1,i2+2,i3+0,c3)+
     & cr1*ui(i1+1,i2+2,i3+0,c3)+cr2*ui(i1+2,i2+2,i3+0,c3)+cr3*ui(i1+
     & 3,i2+2,i3+0,c3)+cr4*ui(i1+4,i2+2,i3+0,c3)+cr5*ui(i1+5,i2+2,i3+
     & 0,c3)+cr6*ui(i1+6,i2+2,i3+0,c3))+cs3*(cr0*ui(i1,i2+3,i3+0,c3)+
     & cr1*ui(i1+1,i2+3,i3+0,c3)+cr2*ui(i1+2,i2+3,i3+0,c3)+cr3*ui(i1+
     & 3,i2+3,i3+0,c3)+cr4*ui(i1+4,i2+3,i3+0,c3)+cr5*ui(i1+5,i2+3,i3+
     & 0,c3)+cr6*ui(i1+6,i2+3,i3+0,c3))+cs4*(cr0*ui(i1,i2+4,i3+0,c3)+
     & cr1*ui(i1+1,i2+4,i3+0,c3)+cr2*ui(i1+2,i2+4,i3+0,c3)+cr3*ui(i1+
     & 3,i2+4,i3+0,c3)+cr4*ui(i1+4,i2+4,i3+0,c3)+cr5*ui(i1+5,i2+4,i3+
     & 0,c3)+cr6*ui(i1+6,i2+4,i3+0,c3))+cs5*(cr0*ui(i1,i2+5,i3+0,c3)+
     & cr1*ui(i1+1,i2+5,i3+0,c3)+cr2*ui(i1+2,i2+5,i3+0,c3)+cr3*ui(i1+
     & 3,i2+5,i3+0,c3)+cr4*ui(i1+4,i2+5,i3+0,c3)+cr5*ui(i1+5,i2+5,i3+
     & 0,c3)+cr6*ui(i1+6,i2+5,i3+0,c3))+cs6*(cr0*ui(i1,i2+6,i3+0,c3)+
     & cr1*ui(i1+1,i2+6,i3+0,c3)+cr2*ui(i1+2,i2+6,i3+0,c3)+cr3*ui(i1+
     & 3,i2+6,i3+0,c3)+cr4*ui(i1+4,i2+6,i3+0,c3)+cr5*ui(i1+5,i2+6,i3+
     & 0,c3)+cr6*ui(i1+6,i2+6,i3+0,c3)))+ct1*(cs0*(cr0*ui(i1,i2,i3+1,
     & c3)+cr1*ui(i1+1,i2,i3+1,c3)+cr2*ui(i1+2,i2,i3+1,c3)+cr3*ui(i1+
     & 3,i2,i3+1,c3)+cr4*ui(i1+4,i2,i3+1,c3)+cr5*ui(i1+5,i2,i3+1,c3)+
     & cr6*ui(i1+6,i2,i3+1,c3))+cs1*(cr0*ui(i1,i2+1,i3+1,c3)+cr1*ui(
     & i1+1,i2+1,i3+1,c3)+cr2*ui(i1+2,i2+1,i3+1,c3)+cr3*ui(i1+3,i2+1,
     & i3+1,c3)+cr4*ui(i1+4,i2+1,i3+1,c3)+cr5*ui(i1+5,i2+1,i3+1,c3)+
     & cr6*ui(i1+6,i2+1,i3+1,c3))+cs2*(cr0*ui(i1,i2+2,i3+1,c3)+cr1*ui(
     & i1+1,i2+2,i3+1,c3)+cr2*ui(i1+2,i2+2,i3+1,c3)+cr3*ui(i1+3,i2+2,
     & i3+1,c3)+cr4*ui(i1+4,i2+2,i3+1,c3)+cr5*ui(i1+5,i2+2,i3+1,c3)+
     & cr6*ui(i1+6,i2+2,i3+1,c3))+cs3*(cr0*ui(i1,i2+3,i3+1,c3)+cr1*ui(
     & i1+1,i2+3,i3+1,c3)+cr2*ui(i1+2,i2+3,i3+1,c3)+cr3*ui(i1+3,i2+3,
     & i3+1,c3)+cr4*ui(i1+4,i2+3,i3+1,c3)+cr5*ui(i1+5,i2+3,i3+1,c3)+
     & cr6*ui(i1+6,i2+3,i3+1,c3))+cs4*(cr0*ui(i1,i2+4,i3+1,c3)+cr1*ui(
     & i1+1,i2+4,i3+1,c3)+cr2*ui(i1+2,i2+4,i3+1,c3)+cr3*ui(i1+3,i2+4,
     & i3+1,c3)+cr4*ui(i1+4,i2+4,i3+1,c3)+cr5*ui(i1+5,i2+4,i3+1,c3)+
     & cr6*ui(i1+6,i2+4,i3+1,c3))+cs5*(cr0*ui(i1,i2+5,i3+1,c3)+cr1*ui(
     & i1+1,i2+5,i3+1,c3)+cr2*ui(i1+2,i2+5,i3+1,c3)+cr3*ui(i1+3,i2+5,
     & i3+1,c3)+cr4*ui(i1+4,i2+5,i3+1,c3)+cr5*ui(i1+5,i2+5,i3+1,c3)+
     & cr6*ui(i1+6,i2+5,i3+1,c3))+cs6*(cr0*ui(i1,i2+6,i3+1,c3)+cr1*ui(
     & i1+1,i2+6,i3+1,c3)+cr2*ui(i1+2,i2+6,i3+1,c3)+cr3*ui(i1+3,i2+6,
     & i3+1,c3)+cr4*ui(i1+4,i2+6,i3+1,c3)+cr5*ui(i1+5,i2+6,i3+1,c3)+
     & cr6*ui(i1+6,i2+6,i3+1,c3)))+ct2*(cs0*(cr0*ui(i1,i2,i3+2,c3)+
     & cr1*ui(i1+1,i2,i3+2,c3)+cr2*ui(i1+2,i2,i3+2,c3)+cr3*ui(i1+3,i2,
     & i3+2,c3)+cr4*ui(i1+4,i2,i3+2,c3)+cr5*ui(i1+5,i2,i3+2,c3)+cr6*
     & ui(i1+6,i2,i3+2,c3))+cs1*(cr0*ui(i1,i2+1,i3+2,c3)+cr1*ui(i1+1,
     & i2+1,i3+2,c3)+cr2*ui(i1+2,i2+1,i3+2,c3)+cr3*ui(i1+3,i2+1,i3+2,
     & c3)+cr4*ui(i1+4,i2+1,i3+2,c3)+cr5*ui(i1+5,i2+1,i3+2,c3)+cr6*ui(
     & i1+6,i2+1,i3+2,c3))+cs2*(cr0*ui(i1,i2+2,i3+2,c3)+cr1*ui(i1+1,
     & i2+2,i3+2,c3)+cr2*ui(i1+2,i2+2,i3+2,c3)+cr3*ui(i1+3,i2+2,i3+2,
     & c3)+cr4*ui(i1+4,i2+2,i3+2,c3)+cr5*ui(i1+5,i2+2,i3+2,c3)+cr6*ui(
     & i1+6,i2+2,i3+2,c3))+cs3*(cr0*ui(i1,i2+3,i3+2,c3)+cr1*ui(i1+1,
     & i2+3,i3+2,c3)+cr2*ui(i1+2,i2+3,i3+2,c3)+cr3*ui(i1+3,i2+3,i3+2,
     & c3)+cr4*ui(i1+4,i2+3,i3+2,c3)+cr5*ui(i1+5,i2+3,i3+2,c3)+cr6*ui(
     & i1+6,i2+3,i3+2,c3))+cs4*(cr0*ui(i1,i2+4,i3+2,c3)+cr1*ui(i1+1,
     & i2+4,i3+2,c3)+cr2*ui(i1+2,i2+4,i3+2,c3)+cr3*ui(i1+3,i2+4,i3+2,
     & c3)+cr4*ui(i1+4,i2+4,i3+2,c3)+cr5*ui(i1+5,i2+4,i3+2,c3)+cr6*ui(
     & i1+6,i2+4,i3+2,c3))+cs5*(cr0*ui(i1,i2+5,i3+2,c3)+cr1*ui(i1+1,
     & i2+5,i3+2,c3)+cr2*ui(i1+2,i2+5,i3+2,c3)+cr3*ui(i1+3,i2+5,i3+2,
     & c3)+cr4*ui(i1+4,i2+5,i3+2,c3)+cr5*ui(i1+5,i2+5,i3+2,c3)+cr6*ui(
     & i1+6,i2+5,i3+2,c3))+cs6*(cr0*ui(i1,i2+6,i3+2,c3)+cr1*ui(i1+1,
     & i2+6,i3+2,c3)+cr2*ui(i1+2,i2+6,i3+2,c3)+cr3*ui(i1+3,i2+6,i3+2,
     & c3)+cr4*ui(i1+4,i2+6,i3+2,c3)+cr5*ui(i1+5,i2+6,i3+2,c3)+cr6*ui(
     & i1+6,i2+6,i3+2,c3)))+ct3*(cs0*(cr0*ui(i1,i2,i3+3,c3)+cr1*ui(i1+
     & 1,i2,i3+3,c3)+cr2*ui(i1+2,i2,i3+3,c3)+cr3*ui(i1+3,i2,i3+3,c3)+
     & cr4*ui(i1+4,i2,i3+3,c3)+cr5*ui(i1+5,i2,i3+3,c3)+cr6*ui(i1+6,i2,
     & i3+3,c3))+cs1*(cr0*ui(i1,i2+1,i3+3,c3)+cr1*ui(i1+1,i2+1,i3+3,
     & c3)+cr2*ui(i1+2,i2+1,i3+3,c3)+cr3*ui(i1+3,i2+1,i3+3,c3)+cr4*ui(
     & i1+4,i2+1,i3+3,c3)+cr5*ui(i1+5,i2+1,i3+3,c3)+cr6*ui(i1+6,i2+1,
     & i3+3,c3))+cs2*(cr0*ui(i1,i2+2,i3+3,c3)+cr1*ui(i1+1,i2+2,i3+3,
     & c3)+cr2*ui(i1+2,i2+2,i3+3,c3)+cr3*ui(i1+3,i2+2,i3+3,c3)+cr4*ui(
     & i1+4,i2+2,i3+3,c3)+cr5*ui(i1+5,i2+2,i3+3,c3)+cr6*ui(i1+6,i2+2,
     & i3+3,c3))+cs3*(cr0*ui(i1,i2+3,i3+3,c3)+cr1*ui(i1+1,i2+3,i3+3,
     & c3)+cr2*ui(i1+2,i2+3,i3+3,c3)+cr3*ui(i1+3,i2+3,i3+3,c3)+cr4*ui(
     & i1+4,i2+3,i3+3,c3)+cr5*ui(i1+5,i2+3,i3+3,c3)+cr6*ui(i1+6,i2+3,
     & i3+3,c3))+cs4*(cr0*ui(i1,i2+4,i3+3,c3)+cr1*ui(i1+1,i2+4,i3+3,
     & c3)+cr2*ui(i1+2,i2+4,i3+3,c3)+cr3*ui(i1+3,i2+4,i3+3,c3)+cr4*ui(
     & i1+4,i2+4,i3+3,c3)+cr5*ui(i1+5,i2+4,i3+3,c3)+cr6*ui(i1+6,i2+4,
     & i3+3,c3))+cs5*(cr0*ui(i1,i2+5,i3+3,c3)+cr1*ui(i1+1,i2+5,i3+3,
     & c3)+cr2*ui(i1+2,i2+5,i3+3,c3)+cr3*ui(i1+3,i2+5,i3+3,c3)+cr4*ui(
     & i1+4,i2+5,i3+3,c3)+cr5*ui(i1+5,i2+5,i3+3,c3)+cr6*ui(i1+6,i2+5,
     & i3+3,c3))+cs6*(cr0*ui(i1,i2+6,i3+3,c3)+cr1*ui(i1+1,i2+6,i3+3,
     & c3)+cr2*ui(i1+2,i2+6,i3+3,c3)+cr3*ui(i1+3,i2+6,i3+3,c3)+cr4*ui(
     & i1+4,i2+6,i3+3,c3)+cr5*ui(i1+5,i2+6,i3+3,c3)+cr6*ui(i1+6,i2+6,
     & i3+3,c3)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+ct4*(cs0*(cr0*ui(i1,i2,i3+4,c3)+cr1*ui(i1+1,i2,i3+4,
     & c3)+cr2*ui(i1+2,i2,i3+4,c3)+cr3*ui(i1+3,i2,i3+4,c3)+cr4*ui(i1+
     & 4,i2,i3+4,c3)+cr5*ui(i1+5,i2,i3+4,c3)+cr6*ui(i1+6,i2,i3+4,c3))+
     & cs1*(cr0*ui(i1,i2+1,i3+4,c3)+cr1*ui(i1+1,i2+1,i3+4,c3)+cr2*ui(
     & i1+2,i2+1,i3+4,c3)+cr3*ui(i1+3,i2+1,i3+4,c3)+cr4*ui(i1+4,i2+1,
     & i3+4,c3)+cr5*ui(i1+5,i2+1,i3+4,c3)+cr6*ui(i1+6,i2+1,i3+4,c3))+
     & cs2*(cr0*ui(i1,i2+2,i3+4,c3)+cr1*ui(i1+1,i2+2,i3+4,c3)+cr2*ui(
     & i1+2,i2+2,i3+4,c3)+cr3*ui(i1+3,i2+2,i3+4,c3)+cr4*ui(i1+4,i2+2,
     & i3+4,c3)+cr5*ui(i1+5,i2+2,i3+4,c3)+cr6*ui(i1+6,i2+2,i3+4,c3))+
     & cs3*(cr0*ui(i1,i2+3,i3+4,c3)+cr1*ui(i1+1,i2+3,i3+4,c3)+cr2*ui(
     & i1+2,i2+3,i3+4,c3)+cr3*ui(i1+3,i2+3,i3+4,c3)+cr4*ui(i1+4,i2+3,
     & i3+4,c3)+cr5*ui(i1+5,i2+3,i3+4,c3)+cr6*ui(i1+6,i2+3,i3+4,c3))+
     & cs4*(cr0*ui(i1,i2+4,i3+4,c3)+cr1*ui(i1+1,i2+4,i3+4,c3)+cr2*ui(
     & i1+2,i2+4,i3+4,c3)+cr3*ui(i1+3,i2+4,i3+4,c3)+cr4*ui(i1+4,i2+4,
     & i3+4,c3)+cr5*ui(i1+5,i2+4,i3+4,c3)+cr6*ui(i1+6,i2+4,i3+4,c3))+
     & cs5*(cr0*ui(i1,i2+5,i3+4,c3)+cr1*ui(i1+1,i2+5,i3+4,c3)+cr2*ui(
     & i1+2,i2+5,i3+4,c3)+cr3*ui(i1+3,i2+5,i3+4,c3)+cr4*ui(i1+4,i2+5,
     & i3+4,c3)+cr5*ui(i1+5,i2+5,i3+4,c3)+cr6*ui(i1+6,i2+5,i3+4,c3))+
     & cs6*(cr0*ui(i1,i2+6,i3+4,c3)+cr1*ui(i1+1,i2+6,i3+4,c3)+cr2*ui(
     & i1+2,i2+6,i3+4,c3)+cr3*ui(i1+3,i2+6,i3+4,c3)+cr4*ui(i1+4,i2+6,
     & i3+4,c3)+cr5*ui(i1+5,i2+6,i3+4,c3)+cr6*ui(i1+6,i2+6,i3+4,c3)))+
     & ct5*(cs0*(cr0*ui(i1,i2,i3+5,c3)+cr1*ui(i1+1,i2,i3+5,c3)+cr2*ui(
     & i1+2,i2,i3+5,c3)+cr3*ui(i1+3,i2,i3+5,c3)+cr4*ui(i1+4,i2,i3+5,
     & c3)+cr5*ui(i1+5,i2,i3+5,c3)+cr6*ui(i1+6,i2,i3+5,c3))+cs1*(cr0*
     & ui(i1,i2+1,i3+5,c3)+cr1*ui(i1+1,i2+1,i3+5,c3)+cr2*ui(i1+2,i2+1,
     & i3+5,c3)+cr3*ui(i1+3,i2+1,i3+5,c3)+cr4*ui(i1+4,i2+1,i3+5,c3)+
     & cr5*ui(i1+5,i2+1,i3+5,c3)+cr6*ui(i1+6,i2+1,i3+5,c3))+cs2*(cr0*
     & ui(i1,i2+2,i3+5,c3)+cr1*ui(i1+1,i2+2,i3+5,c3)+cr2*ui(i1+2,i2+2,
     & i3+5,c3)+cr3*ui(i1+3,i2+2,i3+5,c3)+cr4*ui(i1+4,i2+2,i3+5,c3)+
     & cr5*ui(i1+5,i2+2,i3+5,c3)+cr6*ui(i1+6,i2+2,i3+5,c3))+cs3*(cr0*
     & ui(i1,i2+3,i3+5,c3)+cr1*ui(i1+1,i2+3,i3+5,c3)+cr2*ui(i1+2,i2+3,
     & i3+5,c3)+cr3*ui(i1+3,i2+3,i3+5,c3)+cr4*ui(i1+4,i2+3,i3+5,c3)+
     & cr5*ui(i1+5,i2+3,i3+5,c3)+cr6*ui(i1+6,i2+3,i3+5,c3))+cs4*(cr0*
     & ui(i1,i2+4,i3+5,c3)+cr1*ui(i1+1,i2+4,i3+5,c3)+cr2*ui(i1+2,i2+4,
     & i3+5,c3)+cr3*ui(i1+3,i2+4,i3+5,c3)+cr4*ui(i1+4,i2+4,i3+5,c3)+
     & cr5*ui(i1+5,i2+4,i3+5,c3)+cr6*ui(i1+6,i2+4,i3+5,c3))+cs5*(cr0*
     & ui(i1,i2+5,i3+5,c3)+cr1*ui(i1+1,i2+5,i3+5,c3)+cr2*ui(i1+2,i2+5,
     & i3+5,c3)+cr3*ui(i1+3,i2+5,i3+5,c3)+cr4*ui(i1+4,i2+5,i3+5,c3)+
     & cr5*ui(i1+5,i2+5,i3+5,c3)+cr6*ui(i1+6,i2+5,i3+5,c3))+cs6*(cr0*
     & ui(i1,i2+6,i3+5,c3)+cr1*ui(i1+1,i2+6,i3+5,c3)+cr2*ui(i1+2,i2+6,
     & i3+5,c3)+cr3*ui(i1+3,i2+6,i3+5,c3)+cr4*ui(i1+4,i2+6,i3+5,c3)+
     & cr5*ui(i1+5,i2+6,i3+5,c3)+cr6*ui(i1+6,i2+6,i3+5,c3)))+ct6*(cs0*
     & (cr0*ui(i1,i2,i3+6,c3)+cr1*ui(i1+1,i2,i3+6,c3)+cr2*ui(i1+2,i2,
     & i3+6,c3)+cr3*ui(i1+3,i2,i3+6,c3)+cr4*ui(i1+4,i2,i3+6,c3)+cr5*
     & ui(i1+5,i2,i3+6,c3)+cr6*ui(i1+6,i2,i3+6,c3))+cs1*(cr0*ui(i1,i2+
     & 1,i3+6,c3)+cr1*ui(i1+1,i2+1,i3+6,c3)+cr2*ui(i1+2,i2+1,i3+6,c3)+
     & cr3*ui(i1+3,i2+1,i3+6,c3)+cr4*ui(i1+4,i2+1,i3+6,c3)+cr5*ui(i1+
     & 5,i2+1,i3+6,c3)+cr6*ui(i1+6,i2+1,i3+6,c3))+cs2*(cr0*ui(i1,i2+2,
     & i3+6,c3)+cr1*ui(i1+1,i2+2,i3+6,c3)+cr2*ui(i1+2,i2+2,i3+6,c3)+
     & cr3*ui(i1+3,i2+2,i3+6,c3)+cr4*ui(i1+4,i2+2,i3+6,c3)+cr5*ui(i1+
     & 5,i2+2,i3+6,c3)+cr6*ui(i1+6,i2+2,i3+6,c3))+cs3*(cr0*ui(i1,i2+3,
     & i3+6,c3)+cr1*ui(i1+1,i2+3,i3+6,c3)+cr2*ui(i1+2,i2+3,i3+6,c3)+
     & cr3*ui(i1+3,i2+3,i3+6,c3)+cr4*ui(i1+4,i2+3,i3+6,c3)+cr5*ui(i1+
     & 5,i2+3,i3+6,c3)+cr6*ui(i1+6,i2+3,i3+6,c3))+cs4*(cr0*ui(i1,i2+4,
     & i3+6,c3)+cr1*ui(i1+1,i2+4,i3+6,c3)+cr2*ui(i1+2,i2+4,i3+6,c3)+
     & cr3*ui(i1+3,i2+4,i3+6,c3)+cr4*ui(i1+4,i2+4,i3+6,c3)+cr5*ui(i1+
     & 5,i2+4,i3+6,c3)+cr6*ui(i1+6,i2+4,i3+6,c3))+cs5*(cr0*ui(i1,i2+5,
     & i3+6,c3)+cr1*ui(i1+1,i2+5,i3+6,c3)+cr2*ui(i1+2,i2+5,i3+6,c3)+
     & cr3*ui(i1+3,i2+5,i3+6,c3)+cr4*ui(i1+4,i2+5,i3+6,c3)+cr5*ui(i1+
     & 5,i2+5,i3+6,c3)+cr6*ui(i1+6,i2+5,i3+6,c3))+cs6*(cr0*ui(i1,i2+6,
     & i3+6,c3)+cr1*ui(i1+1,i2+6,i3+6,c3)+cr2*ui(i1+2,i2+6,i3+6,c3)+
     & cr3*ui(i1+3,i2+6,i3+6,c3)+cr4*ui(i1+4,i2+6,i3+6,c3)+cr5*ui(i1+
     & 5,i2+6,i3+6,c3)+cr6*ui(i1+6,i2+6,i3+6,c3)))


             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
! interpSparseStorage777(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q70(c(i,0,0,0))
                 cs0 = q70(c(i,1,0,0))
                 ct0 = q70(c(i,2,0,0))
                 cr1 = q71(c(i,0,0,0))
                 cs1 = q71(c(i,1,0,0))
                 ct1 = q71(c(i,2,0,0))
                 cr2 = q72(c(i,0,0,0))
                 cs2 = q72(c(i,1,0,0))
                 ct2 = q72(c(i,2,0,0))
                 cr3 = q73(c(i,0,0,0))
                 cs3 = q73(c(i,1,0,0))
                 ct3 = q73(c(i,2,0,0))
                 cr4 = q74(c(i,0,0,0))
                 cs4 = q74(c(i,1,0,0))
                 ct4 = q74(c(i,2,0,0))
                 cr5 = q75(c(i,0,0,0))
                 cs5 = q75(c(i,1,0,0))
                 ct5 = q75(c(i,2,0,0))
                 cr6 = q76(c(i,0,0,0))
                 cs6 = q76(c(i,1,0,0))
                 ct6 = q76(c(i,2,0,0))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ct0*(cs0*(cr0*ui(i1,i2,
     & i3+0,c3)+cr1*ui(i1+1,i2,i3+0,c3)+cr2*ui(i1+2,i2,i3+0,c3)+cr3*
     & ui(i1+3,i2,i3+0,c3)+cr4*ui(i1+4,i2,i3+0,c3)+cr5*ui(i1+5,i2,i3+
     & 0,c3)+cr6*ui(i1+6,i2,i3+0,c3))+cs1*(cr0*ui(i1,i2+1,i3+0,c3)+
     & cr1*ui(i1+1,i2+1,i3+0,c3)+cr2*ui(i1+2,i2+1,i3+0,c3)+cr3*ui(i1+
     & 3,i2+1,i3+0,c3)+cr4*ui(i1+4,i2+1,i3+0,c3)+cr5*ui(i1+5,i2+1,i3+
     & 0,c3)+cr6*ui(i1+6,i2+1,i3+0,c3))+cs2*(cr0*ui(i1,i2+2,i3+0,c3)+
     & cr1*ui(i1+1,i2+2,i3+0,c3)+cr2*ui(i1+2,i2+2,i3+0,c3)+cr3*ui(i1+
     & 3,i2+2,i3+0,c3)+cr4*ui(i1+4,i2+2,i3+0,c3)+cr5*ui(i1+5,i2+2,i3+
     & 0,c3)+cr6*ui(i1+6,i2+2,i3+0,c3))+cs3*(cr0*ui(i1,i2+3,i3+0,c3)+
     & cr1*ui(i1+1,i2+3,i3+0,c3)+cr2*ui(i1+2,i2+3,i3+0,c3)+cr3*ui(i1+
     & 3,i2+3,i3+0,c3)+cr4*ui(i1+4,i2+3,i3+0,c3)+cr5*ui(i1+5,i2+3,i3+
     & 0,c3)+cr6*ui(i1+6,i2+3,i3+0,c3))+cs4*(cr0*ui(i1,i2+4,i3+0,c3)+
     & cr1*ui(i1+1,i2+4,i3+0,c3)+cr2*ui(i1+2,i2+4,i3+0,c3)+cr3*ui(i1+
     & 3,i2+4,i3+0,c3)+cr4*ui(i1+4,i2+4,i3+0,c3)+cr5*ui(i1+5,i2+4,i3+
     & 0,c3)+cr6*ui(i1+6,i2+4,i3+0,c3))+cs5*(cr0*ui(i1,i2+5,i3+0,c3)+
     & cr1*ui(i1+1,i2+5,i3+0,c3)+cr2*ui(i1+2,i2+5,i3+0,c3)+cr3*ui(i1+
     & 3,i2+5,i3+0,c3)+cr4*ui(i1+4,i2+5,i3+0,c3)+cr5*ui(i1+5,i2+5,i3+
     & 0,c3)+cr6*ui(i1+6,i2+5,i3+0,c3))+cs6*(cr0*ui(i1,i2+6,i3+0,c3)+
     & cr1*ui(i1+1,i2+6,i3+0,c3)+cr2*ui(i1+2,i2+6,i3+0,c3)+cr3*ui(i1+
     & 3,i2+6,i3+0,c3)+cr4*ui(i1+4,i2+6,i3+0,c3)+cr5*ui(i1+5,i2+6,i3+
     & 0,c3)+cr6*ui(i1+6,i2+6,i3+0,c3)))+ct1*(cs0*(cr0*ui(i1,i2,i3+1,
     & c3)+cr1*ui(i1+1,i2,i3+1,c3)+cr2*ui(i1+2,i2,i3+1,c3)+cr3*ui(i1+
     & 3,i2,i3+1,c3)+cr4*ui(i1+4,i2,i3+1,c3)+cr5*ui(i1+5,i2,i3+1,c3)+
     & cr6*ui(i1+6,i2,i3+1,c3))+cs1*(cr0*ui(i1,i2+1,i3+1,c3)+cr1*ui(
     & i1+1,i2+1,i3+1,c3)+cr2*ui(i1+2,i2+1,i3+1,c3)+cr3*ui(i1+3,i2+1,
     & i3+1,c3)+cr4*ui(i1+4,i2+1,i3+1,c3)+cr5*ui(i1+5,i2+1,i3+1,c3)+
     & cr6*ui(i1+6,i2+1,i3+1,c3))+cs2*(cr0*ui(i1,i2+2,i3+1,c3)+cr1*ui(
     & i1+1,i2+2,i3+1,c3)+cr2*ui(i1+2,i2+2,i3+1,c3)+cr3*ui(i1+3,i2+2,
     & i3+1,c3)+cr4*ui(i1+4,i2+2,i3+1,c3)+cr5*ui(i1+5,i2+2,i3+1,c3)+
     & cr6*ui(i1+6,i2+2,i3+1,c3))+cs3*(cr0*ui(i1,i2+3,i3+1,c3)+cr1*ui(
     & i1+1,i2+3,i3+1,c3)+cr2*ui(i1+2,i2+3,i3+1,c3)+cr3*ui(i1+3,i2+3,
     & i3+1,c3)+cr4*ui(i1+4,i2+3,i3+1,c3)+cr5*ui(i1+5,i2+3,i3+1,c3)+
     & cr6*ui(i1+6,i2+3,i3+1,c3))+cs4*(cr0*ui(i1,i2+4,i3+1,c3)+cr1*ui(
     & i1+1,i2+4,i3+1,c3)+cr2*ui(i1+2,i2+4,i3+1,c3)+cr3*ui(i1+3,i2+4,
     & i3+1,c3)+cr4*ui(i1+4,i2+4,i3+1,c3)+cr5*ui(i1+5,i2+4,i3+1,c3)+
     & cr6*ui(i1+6,i2+4,i3+1,c3))+cs5*(cr0*ui(i1,i2+5,i3+1,c3)+cr1*ui(
     & i1+1,i2+5,i3+1,c3)+cr2*ui(i1+2,i2+5,i3+1,c3)+cr3*ui(i1+3,i2+5,
     & i3+1,c3)+cr4*ui(i1+4,i2+5,i3+1,c3)+cr5*ui(i1+5,i2+5,i3+1,c3)+
     & cr6*ui(i1+6,i2+5,i3+1,c3))+cs6*(cr0*ui(i1,i2+6,i3+1,c3)+cr1*ui(
     & i1+1,i2+6,i3+1,c3)+cr2*ui(i1+2,i2+6,i3+1,c3)+cr3*ui(i1+3,i2+6,
     & i3+1,c3)+cr4*ui(i1+4,i2+6,i3+1,c3)+cr5*ui(i1+5,i2+6,i3+1,c3)+
     & cr6*ui(i1+6,i2+6,i3+1,c3)))+ct2*(cs0*(cr0*ui(i1,i2,i3+2,c3)+
     & cr1*ui(i1+1,i2,i3+2,c3)+cr2*ui(i1+2,i2,i3+2,c3)+cr3*ui(i1+3,i2,
     & i3+2,c3)+cr4*ui(i1+4,i2,i3+2,c3)+cr5*ui(i1+5,i2,i3+2,c3)+cr6*
     & ui(i1+6,i2,i3+2,c3))+cs1*(cr0*ui(i1,i2+1,i3+2,c3)+cr1*ui(i1+1,
     & i2+1,i3+2,c3)+cr2*ui(i1+2,i2+1,i3+2,c3)+cr3*ui(i1+3,i2+1,i3+2,
     & c3)+cr4*ui(i1+4,i2+1,i3+2,c3)+cr5*ui(i1+5,i2+1,i3+2,c3)+cr6*ui(
     & i1+6,i2+1,i3+2,c3))+cs2*(cr0*ui(i1,i2+2,i3+2,c3)+cr1*ui(i1+1,
     & i2+2,i3+2,c3)+cr2*ui(i1+2,i2+2,i3+2,c3)+cr3*ui(i1+3,i2+2,i3+2,
     & c3)+cr4*ui(i1+4,i2+2,i3+2,c3)+cr5*ui(i1+5,i2+2,i3+2,c3)+cr6*ui(
     & i1+6,i2+2,i3+2,c3))+cs3*(cr0*ui(i1,i2+3,i3+2,c3)+cr1*ui(i1+1,
     & i2+3,i3+2,c3)+cr2*ui(i1+2,i2+3,i3+2,c3)+cr3*ui(i1+3,i2+3,i3+2,
     & c3)+cr4*ui(i1+4,i2+3,i3+2,c3)+cr5*ui(i1+5,i2+3,i3+2,c3)+cr6*ui(
     & i1+6,i2+3,i3+2,c3))+cs4*(cr0*ui(i1,i2+4,i3+2,c3)+cr1*ui(i1+1,
     & i2+4,i3+2,c3)+cr2*ui(i1+2,i2+4,i3+2,c3)+cr3*ui(i1+3,i2+4,i3+2,
     & c3)+cr4*ui(i1+4,i2+4,i3+2,c3)+cr5*ui(i1+5,i2+4,i3+2,c3)+cr6*ui(
     & i1+6,i2+4,i3+2,c3))+cs5*(cr0*ui(i1,i2+5,i3+2,c3)+cr1*ui(i1+1,
     & i2+5,i3+2,c3)+cr2*ui(i1+2,i2+5,i3+2,c3)+cr3*ui(i1+3,i2+5,i3+2,
     & c3)+cr4*ui(i1+4,i2+5,i3+2,c3)+cr5*ui(i1+5,i2+5,i3+2,c3)+cr6*ui(
     & i1+6,i2+5,i3+2,c3))+cs6*(cr0*ui(i1,i2+6,i3+2,c3)+cr1*ui(i1+1,
     & i2+6,i3+2,c3)+cr2*ui(i1+2,i2+6,i3+2,c3)+cr3*ui(i1+3,i2+6,i3+2,
     & c3)+cr4*ui(i1+4,i2+6,i3+2,c3)+cr5*ui(i1+5,i2+6,i3+2,c3)+cr6*ui(
     & i1+6,i2+6,i3+2,c3)))+ct3*(cs0*(cr0*ui(i1,i2,i3+3,c3)+cr1*ui(i1+
     & 1,i2,i3+3,c3)+cr2*ui(i1+2,i2,i3+3,c3)+cr3*ui(i1+3,i2,i3+3,c3)+
     & cr4*ui(i1+4,i2,i3+3,c3)+cr5*ui(i1+5,i2,i3+3,c3)+cr6*ui(i1+6,i2,
     & i3+3,c3))+cs1*(cr0*ui(i1,i2+1,i3+3,c3)+cr1*ui(i1+1,i2+1,i3+3,
     & c3)+cr2*ui(i1+2,i2+1,i3+3,c3)+cr3*ui(i1+3,i2+1,i3+3,c3)+cr4*ui(
     & i1+4,i2+1,i3+3,c3)+cr5*ui(i1+5,i2+1,i3+3,c3)+cr6*ui(i1+6,i2+1,
     & i3+3,c3))+cs2*(cr0*ui(i1,i2+2,i3+3,c3)+cr1*ui(i1+1,i2+2,i3+3,
     & c3)+cr2*ui(i1+2,i2+2,i3+3,c3)+cr3*ui(i1+3,i2+2,i3+3,c3)+cr4*ui(
     & i1+4,i2+2,i3+3,c3)+cr5*ui(i1+5,i2+2,i3+3,c3)+cr6*ui(i1+6,i2+2,
     & i3+3,c3))+cs3*(cr0*ui(i1,i2+3,i3+3,c3)+cr1*ui(i1+1,i2+3,i3+3,
     & c3)+cr2*ui(i1+2,i2+3,i3+3,c3)+cr3*ui(i1+3,i2+3,i3+3,c3)+cr4*ui(
     & i1+4,i2+3,i3+3,c3)+cr5*ui(i1+5,i2+3,i3+3,c3)+cr6*ui(i1+6,i2+3,
     & i3+3,c3))+cs4*(cr0*ui(i1,i2+4,i3+3,c3)+cr1*ui(i1+1,i2+4,i3+3,
     & c3)+cr2*ui(i1+2,i2+4,i3+3,c3)+cr3*ui(i1+3,i2+4,i3+3,c3)+cr4*ui(
     & i1+4,i2+4,i3+3,c3)+cr5*ui(i1+5,i2+4,i3+3,c3)+cr6*ui(i1+6,i2+4,
     & i3+3,c3))+cs5*(cr0*ui(i1,i2+5,i3+3,c3)+cr1*ui(i1+1,i2+5,i3+3,
     & c3)+cr2*ui(i1+2,i2+5,i3+3,c3)+cr3*ui(i1+3,i2+5,i3+3,c3)+cr4*ui(
     & i1+4,i2+5,i3+3,c3)+cr5*ui(i1+5,i2+5,i3+3,c3)+cr6*ui(i1+6,i2+5,
     & i3+3,c3))+cs6*(cr0*ui(i1,i2+6,i3+3,c3)+cr1*ui(i1+1,i2+6,i3+3,
     & c3)+cr2*ui(i1+2,i2+6,i3+3,c3)+cr3*ui(i1+3,i2+6,i3+3,c3)+cr4*ui(
     & i1+4,i2+6,i3+3,c3)+cr5*ui(i1+5,i2+6,i3+3,c3)+cr6*ui(i1+6,i2+6,
     & i3+3,c3)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+ct4*(cs0*(cr0*ui(i1,i2,i3+4,c3)+cr1*ui(i1+1,i2,i3+4,
     & c3)+cr2*ui(i1+2,i2,i3+4,c3)+cr3*ui(i1+3,i2,i3+4,c3)+cr4*ui(i1+
     & 4,i2,i3+4,c3)+cr5*ui(i1+5,i2,i3+4,c3)+cr6*ui(i1+6,i2,i3+4,c3))+
     & cs1*(cr0*ui(i1,i2+1,i3+4,c3)+cr1*ui(i1+1,i2+1,i3+4,c3)+cr2*ui(
     & i1+2,i2+1,i3+4,c3)+cr3*ui(i1+3,i2+1,i3+4,c3)+cr4*ui(i1+4,i2+1,
     & i3+4,c3)+cr5*ui(i1+5,i2+1,i3+4,c3)+cr6*ui(i1+6,i2+1,i3+4,c3))+
     & cs2*(cr0*ui(i1,i2+2,i3+4,c3)+cr1*ui(i1+1,i2+2,i3+4,c3)+cr2*ui(
     & i1+2,i2+2,i3+4,c3)+cr3*ui(i1+3,i2+2,i3+4,c3)+cr4*ui(i1+4,i2+2,
     & i3+4,c3)+cr5*ui(i1+5,i2+2,i3+4,c3)+cr6*ui(i1+6,i2+2,i3+4,c3))+
     & cs3*(cr0*ui(i1,i2+3,i3+4,c3)+cr1*ui(i1+1,i2+3,i3+4,c3)+cr2*ui(
     & i1+2,i2+3,i3+4,c3)+cr3*ui(i1+3,i2+3,i3+4,c3)+cr4*ui(i1+4,i2+3,
     & i3+4,c3)+cr5*ui(i1+5,i2+3,i3+4,c3)+cr6*ui(i1+6,i2+3,i3+4,c3))+
     & cs4*(cr0*ui(i1,i2+4,i3+4,c3)+cr1*ui(i1+1,i2+4,i3+4,c3)+cr2*ui(
     & i1+2,i2+4,i3+4,c3)+cr3*ui(i1+3,i2+4,i3+4,c3)+cr4*ui(i1+4,i2+4,
     & i3+4,c3)+cr5*ui(i1+5,i2+4,i3+4,c3)+cr6*ui(i1+6,i2+4,i3+4,c3))+
     & cs5*(cr0*ui(i1,i2+5,i3+4,c3)+cr1*ui(i1+1,i2+5,i3+4,c3)+cr2*ui(
     & i1+2,i2+5,i3+4,c3)+cr3*ui(i1+3,i2+5,i3+4,c3)+cr4*ui(i1+4,i2+5,
     & i3+4,c3)+cr5*ui(i1+5,i2+5,i3+4,c3)+cr6*ui(i1+6,i2+5,i3+4,c3))+
     & cs6*(cr0*ui(i1,i2+6,i3+4,c3)+cr1*ui(i1+1,i2+6,i3+4,c3)+cr2*ui(
     & i1+2,i2+6,i3+4,c3)+cr3*ui(i1+3,i2+6,i3+4,c3)+cr4*ui(i1+4,i2+6,
     & i3+4,c3)+cr5*ui(i1+5,i2+6,i3+4,c3)+cr6*ui(i1+6,i2+6,i3+4,c3)))+
     & ct5*(cs0*(cr0*ui(i1,i2,i3+5,c3)+cr1*ui(i1+1,i2,i3+5,c3)+cr2*ui(
     & i1+2,i2,i3+5,c3)+cr3*ui(i1+3,i2,i3+5,c3)+cr4*ui(i1+4,i2,i3+5,
     & c3)+cr5*ui(i1+5,i2,i3+5,c3)+cr6*ui(i1+6,i2,i3+5,c3))+cs1*(cr0*
     & ui(i1,i2+1,i3+5,c3)+cr1*ui(i1+1,i2+1,i3+5,c3)+cr2*ui(i1+2,i2+1,
     & i3+5,c3)+cr3*ui(i1+3,i2+1,i3+5,c3)+cr4*ui(i1+4,i2+1,i3+5,c3)+
     & cr5*ui(i1+5,i2+1,i3+5,c3)+cr6*ui(i1+6,i2+1,i3+5,c3))+cs2*(cr0*
     & ui(i1,i2+2,i3+5,c3)+cr1*ui(i1+1,i2+2,i3+5,c3)+cr2*ui(i1+2,i2+2,
     & i3+5,c3)+cr3*ui(i1+3,i2+2,i3+5,c3)+cr4*ui(i1+4,i2+2,i3+5,c3)+
     & cr5*ui(i1+5,i2+2,i3+5,c3)+cr6*ui(i1+6,i2+2,i3+5,c3))+cs3*(cr0*
     & ui(i1,i2+3,i3+5,c3)+cr1*ui(i1+1,i2+3,i3+5,c3)+cr2*ui(i1+2,i2+3,
     & i3+5,c3)+cr3*ui(i1+3,i2+3,i3+5,c3)+cr4*ui(i1+4,i2+3,i3+5,c3)+
     & cr5*ui(i1+5,i2+3,i3+5,c3)+cr6*ui(i1+6,i2+3,i3+5,c3))+cs4*(cr0*
     & ui(i1,i2+4,i3+5,c3)+cr1*ui(i1+1,i2+4,i3+5,c3)+cr2*ui(i1+2,i2+4,
     & i3+5,c3)+cr3*ui(i1+3,i2+4,i3+5,c3)+cr4*ui(i1+4,i2+4,i3+5,c3)+
     & cr5*ui(i1+5,i2+4,i3+5,c3)+cr6*ui(i1+6,i2+4,i3+5,c3))+cs5*(cr0*
     & ui(i1,i2+5,i3+5,c3)+cr1*ui(i1+1,i2+5,i3+5,c3)+cr2*ui(i1+2,i2+5,
     & i3+5,c3)+cr3*ui(i1+3,i2+5,i3+5,c3)+cr4*ui(i1+4,i2+5,i3+5,c3)+
     & cr5*ui(i1+5,i2+5,i3+5,c3)+cr6*ui(i1+6,i2+5,i3+5,c3))+cs6*(cr0*
     & ui(i1,i2+6,i3+5,c3)+cr1*ui(i1+1,i2+6,i3+5,c3)+cr2*ui(i1+2,i2+6,
     & i3+5,c3)+cr3*ui(i1+3,i2+6,i3+5,c3)+cr4*ui(i1+4,i2+6,i3+5,c3)+
     & cr5*ui(i1+5,i2+6,i3+5,c3)+cr6*ui(i1+6,i2+6,i3+5,c3)))+ct6*(cs0*
     & (cr0*ui(i1,i2,i3+6,c3)+cr1*ui(i1+1,i2,i3+6,c3)+cr2*ui(i1+2,i2,
     & i3+6,c3)+cr3*ui(i1+3,i2,i3+6,c3)+cr4*ui(i1+4,i2,i3+6,c3)+cr5*
     & ui(i1+5,i2,i3+6,c3)+cr6*ui(i1+6,i2,i3+6,c3))+cs1*(cr0*ui(i1,i2+
     & 1,i3+6,c3)+cr1*ui(i1+1,i2+1,i3+6,c3)+cr2*ui(i1+2,i2+1,i3+6,c3)+
     & cr3*ui(i1+3,i2+1,i3+6,c3)+cr4*ui(i1+4,i2+1,i3+6,c3)+cr5*ui(i1+
     & 5,i2+1,i3+6,c3)+cr6*ui(i1+6,i2+1,i3+6,c3))+cs2*(cr0*ui(i1,i2+2,
     & i3+6,c3)+cr1*ui(i1+1,i2+2,i3+6,c3)+cr2*ui(i1+2,i2+2,i3+6,c3)+
     & cr3*ui(i1+3,i2+2,i3+6,c3)+cr4*ui(i1+4,i2+2,i3+6,c3)+cr5*ui(i1+
     & 5,i2+2,i3+6,c3)+cr6*ui(i1+6,i2+2,i3+6,c3))+cs3*(cr0*ui(i1,i2+3,
     & i3+6,c3)+cr1*ui(i1+1,i2+3,i3+6,c3)+cr2*ui(i1+2,i2+3,i3+6,c3)+
     & cr3*ui(i1+3,i2+3,i3+6,c3)+cr4*ui(i1+4,i2+3,i3+6,c3)+cr5*ui(i1+
     & 5,i2+3,i3+6,c3)+cr6*ui(i1+6,i2+3,i3+6,c3))+cs4*(cr0*ui(i1,i2+4,
     & i3+6,c3)+cr1*ui(i1+1,i2+4,i3+6,c3)+cr2*ui(i1+2,i2+4,i3+6,c3)+
     & cr3*ui(i1+3,i2+4,i3+6,c3)+cr4*ui(i1+4,i2+4,i3+6,c3)+cr5*ui(i1+
     & 5,i2+4,i3+6,c3)+cr6*ui(i1+6,i2+4,i3+6,c3))+cs5*(cr0*ui(i1,i2+5,
     & i3+6,c3)+cr1*ui(i1+1,i2+5,i3+6,c3)+cr2*ui(i1+2,i2+5,i3+6,c3)+
     & cr3*ui(i1+3,i2+5,i3+6,c3)+cr4*ui(i1+4,i2+5,i3+6,c3)+cr5*ui(i1+
     & 5,i2+5,i3+6,c3)+cr6*ui(i1+6,i2+5,i3+6,c3))+cs6*(cr0*ui(i1,i2+6,
     & i3+6,c3)+cr1*ui(i1+1,i2+6,i3+6,c3)+cr2*ui(i1+2,i2+6,i3+6,c3)+
     & cr3*ui(i1+3,i2+6,i3+6,c3)+cr4*ui(i1+4,i2+6,i3+6,c3)+cr5*ui(i1+
     & 5,i2+6,i3+6,c3)+cr6*ui(i1+6,i2+6,i3+6,c3)))


             end do
             end do
           end if
         else if( width(1).eq.8 .and. width(2).eq.8 .and. width(3)
     & .eq.8 )then
! loops3d($interpSparseStorage888(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)
           if( c3a.eq.c3b )then
             do c3=c3a,c3b
             do i=nia,nib
! interpSparseStorage888(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q80(c(i,0,0,0))
                 cs0 = q80(c(i,1,0,0))
                 ct0 = q80(c(i,2,0,0))
                 cr1 = q81(c(i,0,0,0))
                 cs1 = q81(c(i,1,0,0))
                 ct1 = q81(c(i,2,0,0))
                 cr2 = q82(c(i,0,0,0))
                 cs2 = q82(c(i,1,0,0))
                 ct2 = q82(c(i,2,0,0))
                 cr3 = q83(c(i,0,0,0))
                 cs3 = q83(c(i,1,0,0))
                 ct3 = q83(c(i,2,0,0))
                 cr4 = q84(c(i,0,0,0))
                 cs4 = q84(c(i,1,0,0))
                 ct4 = q84(c(i,2,0,0))
                 cr5 = q85(c(i,0,0,0))
                 cs5 = q85(c(i,1,0,0))
                 ct5 = q85(c(i,2,0,0))
                 cr6 = q86(c(i,0,0,0))
                 cs6 = q86(c(i,1,0,0))
                 ct6 = q86(c(i,2,0,0))
                 cr7 = q87(c(i,0,0,0))
                 cs7 = q87(c(i,1,0,0))
                 ct7 = q87(c(i,2,0,0))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ct0*(cs0*(cr0*ui(i1,i2,
     & i3+0,c3)+cr1*ui(i1+1,i2,i3+0,c3)+cr2*ui(i1+2,i2,i3+0,c3)+cr3*
     & ui(i1+3,i2,i3+0,c3)+cr4*ui(i1+4,i2,i3+0,c3)+cr5*ui(i1+5,i2,i3+
     & 0,c3)+cr6*ui(i1+6,i2,i3+0,c3)+cr7*ui(i1+7,i2,i3+0,c3))+cs1*(
     & cr0*ui(i1,i2+1,i3+0,c3)+cr1*ui(i1+1,i2+1,i3+0,c3)+cr2*ui(i1+2,
     & i2+1,i3+0,c3)+cr3*ui(i1+3,i2+1,i3+0,c3)+cr4*ui(i1+4,i2+1,i3+0,
     & c3)+cr5*ui(i1+5,i2+1,i3+0,c3)+cr6*ui(i1+6,i2+1,i3+0,c3)+cr7*ui(
     & i1+7,i2+1,i3+0,c3))+cs2*(cr0*ui(i1,i2+2,i3+0,c3)+cr1*ui(i1+1,
     & i2+2,i3+0,c3)+cr2*ui(i1+2,i2+2,i3+0,c3)+cr3*ui(i1+3,i2+2,i3+0,
     & c3)+cr4*ui(i1+4,i2+2,i3+0,c3)+cr5*ui(i1+5,i2+2,i3+0,c3)+cr6*ui(
     & i1+6,i2+2,i3+0,c3)+cr7*ui(i1+7,i2+2,i3+0,c3))+cs3*(cr0*ui(i1,
     & i2+3,i3+0,c3)+cr1*ui(i1+1,i2+3,i3+0,c3)+cr2*ui(i1+2,i2+3,i3+0,
     & c3)+cr3*ui(i1+3,i2+3,i3+0,c3)+cr4*ui(i1+4,i2+3,i3+0,c3)+cr5*ui(
     & i1+5,i2+3,i3+0,c3)+cr6*ui(i1+6,i2+3,i3+0,c3)+cr7*ui(i1+7,i2+3,
     & i3+0,c3))+cs4*(cr0*ui(i1,i2+4,i3+0,c3)+cr1*ui(i1+1,i2+4,i3+0,
     & c3)+cr2*ui(i1+2,i2+4,i3+0,c3)+cr3*ui(i1+3,i2+4,i3+0,c3)+cr4*ui(
     & i1+4,i2+4,i3+0,c3)+cr5*ui(i1+5,i2+4,i3+0,c3)+cr6*ui(i1+6,i2+4,
     & i3+0,c3)+cr7*ui(i1+7,i2+4,i3+0,c3))+cs5*(cr0*ui(i1,i2+5,i3+0,
     & c3)+cr1*ui(i1+1,i2+5,i3+0,c3)+cr2*ui(i1+2,i2+5,i3+0,c3)+cr3*ui(
     & i1+3,i2+5,i3+0,c3)+cr4*ui(i1+4,i2+5,i3+0,c3)+cr5*ui(i1+5,i2+5,
     & i3+0,c3)+cr6*ui(i1+6,i2+5,i3+0,c3)+cr7*ui(i1+7,i2+5,i3+0,c3))+
     & cs6*(cr0*ui(i1,i2+6,i3+0,c3)+cr1*ui(i1+1,i2+6,i3+0,c3)+cr2*ui(
     & i1+2,i2+6,i3+0,c3)+cr3*ui(i1+3,i2+6,i3+0,c3)+cr4*ui(i1+4,i2+6,
     & i3+0,c3)+cr5*ui(i1+5,i2+6,i3+0,c3)+cr6*ui(i1+6,i2+6,i3+0,c3)+
     & cr7*ui(i1+7,i2+6,i3+0,c3))+cs7*(cr0*ui(i1,i2+7,i3+0,c3)+cr1*ui(
     & i1+1,i2+7,i3+0,c3)+cr2*ui(i1+2,i2+7,i3+0,c3)+cr3*ui(i1+3,i2+7,
     & i3+0,c3)+cr4*ui(i1+4,i2+7,i3+0,c3)+cr5*ui(i1+5,i2+7,i3+0,c3)+
     & cr6*ui(i1+6,i2+7,i3+0,c3)+cr7*ui(i1+7,i2+7,i3+0,c3)))+ct1*(cs0*
     & (cr0*ui(i1,i2,i3+1,c3)+cr1*ui(i1+1,i2,i3+1,c3)+cr2*ui(i1+2,i2,
     & i3+1,c3)+cr3*ui(i1+3,i2,i3+1,c3)+cr4*ui(i1+4,i2,i3+1,c3)+cr5*
     & ui(i1+5,i2,i3+1,c3)+cr6*ui(i1+6,i2,i3+1,c3)+cr7*ui(i1+7,i2,i3+
     & 1,c3))+cs1*(cr0*ui(i1,i2+1,i3+1,c3)+cr1*ui(i1+1,i2+1,i3+1,c3)+
     & cr2*ui(i1+2,i2+1,i3+1,c3)+cr3*ui(i1+3,i2+1,i3+1,c3)+cr4*ui(i1+
     & 4,i2+1,i3+1,c3)+cr5*ui(i1+5,i2+1,i3+1,c3)+cr6*ui(i1+6,i2+1,i3+
     & 1,c3)+cr7*ui(i1+7,i2+1,i3+1,c3))+cs2*(cr0*ui(i1,i2+2,i3+1,c3)+
     & cr1*ui(i1+1,i2+2,i3+1,c3)+cr2*ui(i1+2,i2+2,i3+1,c3)+cr3*ui(i1+
     & 3,i2+2,i3+1,c3)+cr4*ui(i1+4,i2+2,i3+1,c3)+cr5*ui(i1+5,i2+2,i3+
     & 1,c3)+cr6*ui(i1+6,i2+2,i3+1,c3)+cr7*ui(i1+7,i2+2,i3+1,c3))+cs3*
     & (cr0*ui(i1,i2+3,i3+1,c3)+cr1*ui(i1+1,i2+3,i3+1,c3)+cr2*ui(i1+2,
     & i2+3,i3+1,c3)+cr3*ui(i1+3,i2+3,i3+1,c3)+cr4*ui(i1+4,i2+3,i3+1,
     & c3)+cr5*ui(i1+5,i2+3,i3+1,c3)+cr6*ui(i1+6,i2+3,i3+1,c3)+cr7*ui(
     & i1+7,i2+3,i3+1,c3))+cs4*(cr0*ui(i1,i2+4,i3+1,c3)+cr1*ui(i1+1,
     & i2+4,i3+1,c3)+cr2*ui(i1+2,i2+4,i3+1,c3)+cr3*ui(i1+3,i2+4,i3+1,
     & c3)+cr4*ui(i1+4,i2+4,i3+1,c3)+cr5*ui(i1+5,i2+4,i3+1,c3)+cr6*ui(
     & i1+6,i2+4,i3+1,c3)+cr7*ui(i1+7,i2+4,i3+1,c3))+cs5*(cr0*ui(i1,
     & i2+5,i3+1,c3)+cr1*ui(i1+1,i2+5,i3+1,c3)+cr2*ui(i1+2,i2+5,i3+1,
     & c3)+cr3*ui(i1+3,i2+5,i3+1,c3)+cr4*ui(i1+4,i2+5,i3+1,c3)+cr5*ui(
     & i1+5,i2+5,i3+1,c3)+cr6*ui(i1+6,i2+5,i3+1,c3)+cr7*ui(i1+7,i2+5,
     & i3+1,c3))+cs6*(cr0*ui(i1,i2+6,i3+1,c3)+cr1*ui(i1+1,i2+6,i3+1,
     & c3)+cr2*ui(i1+2,i2+6,i3+1,c3)+cr3*ui(i1+3,i2+6,i3+1,c3)+cr4*ui(
     & i1+4,i2+6,i3+1,c3)+cr5*ui(i1+5,i2+6,i3+1,c3)+cr6*ui(i1+6,i2+6,
     & i3+1,c3)+cr7*ui(i1+7,i2+6,i3+1,c3))+cs7*(cr0*ui(i1,i2+7,i3+1,
     & c3)+cr1*ui(i1+1,i2+7,i3+1,c3)+cr2*ui(i1+2,i2+7,i3+1,c3)+cr3*ui(
     & i1+3,i2+7,i3+1,c3)+cr4*ui(i1+4,i2+7,i3+1,c3)+cr5*ui(i1+5,i2+7,
     & i3+1,c3)+cr6*ui(i1+6,i2+7,i3+1,c3)+cr7*ui(i1+7,i2+7,i3+1,c3)))+
     & ct2*(cs0*(cr0*ui(i1,i2,i3+2,c3)+cr1*ui(i1+1,i2,i3+2,c3)+cr2*ui(
     & i1+2,i2,i3+2,c3)+cr3*ui(i1+3,i2,i3+2,c3)+cr4*ui(i1+4,i2,i3+2,
     & c3)+cr5*ui(i1+5,i2,i3+2,c3)+cr6*ui(i1+6,i2,i3+2,c3)+cr7*ui(i1+
     & 7,i2,i3+2,c3))+cs1*(cr0*ui(i1,i2+1,i3+2,c3)+cr1*ui(i1+1,i2+1,
     & i3+2,c3)+cr2*ui(i1+2,i2+1,i3+2,c3)+cr3*ui(i1+3,i2+1,i3+2,c3)+
     & cr4*ui(i1+4,i2+1,i3+2,c3)+cr5*ui(i1+5,i2+1,i3+2,c3)+cr6*ui(i1+
     & 6,i2+1,i3+2,c3)+cr7*ui(i1+7,i2+1,i3+2,c3))+cs2*(cr0*ui(i1,i2+2,
     & i3+2,c3)+cr1*ui(i1+1,i2+2,i3+2,c3)+cr2*ui(i1+2,i2+2,i3+2,c3)+
     & cr3*ui(i1+3,i2+2,i3+2,c3)+cr4*ui(i1+4,i2+2,i3+2,c3)+cr5*ui(i1+
     & 5,i2+2,i3+2,c3)+cr6*ui(i1+6,i2+2,i3+2,c3)+cr7*ui(i1+7,i2+2,i3+
     & 2,c3))+cs3*(cr0*ui(i1,i2+3,i3+2,c3)+cr1*ui(i1+1,i2+3,i3+2,c3)+
     & cr2*ui(i1+2,i2+3,i3+2,c3)+cr3*ui(i1+3,i2+3,i3+2,c3)+cr4*ui(i1+
     & 4,i2+3,i3+2,c3)+cr5*ui(i1+5,i2+3,i3+2,c3)+cr6*ui(i1+6,i2+3,i3+
     & 2,c3)+cr7*ui(i1+7,i2+3,i3+2,c3))+cs4*(cr0*ui(i1,i2+4,i3+2,c3)+
     & cr1*ui(i1+1,i2+4,i3+2,c3)+cr2*ui(i1+2,i2+4,i3+2,c3)+cr3*ui(i1+
     & 3,i2+4,i3+2,c3)+cr4*ui(i1+4,i2+4,i3+2,c3)+cr5*ui(i1+5,i2+4,i3+
     & 2,c3)+cr6*ui(i1+6,i2+4,i3+2,c3)+cr7*ui(i1+7,i2+4,i3+2,c3))+cs5*
     & (cr0*ui(i1,i2+5,i3+2,c3)+cr1*ui(i1+1,i2+5,i3+2,c3)+cr2*ui(i1+2,
     & i2+5,i3+2,c3)+cr3*ui(i1+3,i2+5,i3+2,c3)+cr4*ui(i1+4,i2+5,i3+2,
     & c3)+cr5*ui(i1+5,i2+5,i3+2,c3)+cr6*ui(i1+6,i2+5,i3+2,c3)+cr7*ui(
     & i1+7,i2+5,i3+2,c3))+cs6*(cr0*ui(i1,i2+6,i3+2,c3)+cr1*ui(i1+1,
     & i2+6,i3+2,c3)+cr2*ui(i1+2,i2+6,i3+2,c3)+cr3*ui(i1+3,i2+6,i3+2,
     & c3)+cr4*ui(i1+4,i2+6,i3+2,c3)+cr5*ui(i1+5,i2+6,i3+2,c3)+cr6*ui(
     & i1+6,i2+6,i3+2,c3)+cr7*ui(i1+7,i2+6,i3+2,c3))+cs7*(cr0*ui(i1,
     & i2+7,i3+2,c3)+cr1*ui(i1+1,i2+7,i3+2,c3)+cr2*ui(i1+2,i2+7,i3+2,
     & c3)+cr3*ui(i1+3,i2+7,i3+2,c3)+cr4*ui(i1+4,i2+7,i3+2,c3)+cr5*ui(
     & i1+5,i2+7,i3+2,c3)+cr6*ui(i1+6,i2+7,i3+2,c3)+cr7*ui(i1+7,i2+7,
     & i3+2,c3)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+ct3*(cs0*(cr0*ui(i1,i2,i3+3,c3)+cr1*ui(i1+1,i2,i3+3,
     & c3)+cr2*ui(i1+2,i2,i3+3,c3)+cr3*ui(i1+3,i2,i3+3,c3)+cr4*ui(i1+
     & 4,i2,i3+3,c3)+cr5*ui(i1+5,i2,i3+3,c3)+cr6*ui(i1+6,i2,i3+3,c3)+
     & cr7*ui(i1+7,i2,i3+3,c3))+cs1*(cr0*ui(i1,i2+1,i3+3,c3)+cr1*ui(
     & i1+1,i2+1,i3+3,c3)+cr2*ui(i1+2,i2+1,i3+3,c3)+cr3*ui(i1+3,i2+1,
     & i3+3,c3)+cr4*ui(i1+4,i2+1,i3+3,c3)+cr5*ui(i1+5,i2+1,i3+3,c3)+
     & cr6*ui(i1+6,i2+1,i3+3,c3)+cr7*ui(i1+7,i2+1,i3+3,c3))+cs2*(cr0*
     & ui(i1,i2+2,i3+3,c3)+cr1*ui(i1+1,i2+2,i3+3,c3)+cr2*ui(i1+2,i2+2,
     & i3+3,c3)+cr3*ui(i1+3,i2+2,i3+3,c3)+cr4*ui(i1+4,i2+2,i3+3,c3)+
     & cr5*ui(i1+5,i2+2,i3+3,c3)+cr6*ui(i1+6,i2+2,i3+3,c3)+cr7*ui(i1+
     & 7,i2+2,i3+3,c3))+cs3*(cr0*ui(i1,i2+3,i3+3,c3)+cr1*ui(i1+1,i2+3,
     & i3+3,c3)+cr2*ui(i1+2,i2+3,i3+3,c3)+cr3*ui(i1+3,i2+3,i3+3,c3)+
     & cr4*ui(i1+4,i2+3,i3+3,c3)+cr5*ui(i1+5,i2+3,i3+3,c3)+cr6*ui(i1+
     & 6,i2+3,i3+3,c3)+cr7*ui(i1+7,i2+3,i3+3,c3))+cs4*(cr0*ui(i1,i2+4,
     & i3+3,c3)+cr1*ui(i1+1,i2+4,i3+3,c3)+cr2*ui(i1+2,i2+4,i3+3,c3)+
     & cr3*ui(i1+3,i2+4,i3+3,c3)+cr4*ui(i1+4,i2+4,i3+3,c3)+cr5*ui(i1+
     & 5,i2+4,i3+3,c3)+cr6*ui(i1+6,i2+4,i3+3,c3)+cr7*ui(i1+7,i2+4,i3+
     & 3,c3))+cs5*(cr0*ui(i1,i2+5,i3+3,c3)+cr1*ui(i1+1,i2+5,i3+3,c3)+
     & cr2*ui(i1+2,i2+5,i3+3,c3)+cr3*ui(i1+3,i2+5,i3+3,c3)+cr4*ui(i1+
     & 4,i2+5,i3+3,c3)+cr5*ui(i1+5,i2+5,i3+3,c3)+cr6*ui(i1+6,i2+5,i3+
     & 3,c3)+cr7*ui(i1+7,i2+5,i3+3,c3))+cs6*(cr0*ui(i1,i2+6,i3+3,c3)+
     & cr1*ui(i1+1,i2+6,i3+3,c3)+cr2*ui(i1+2,i2+6,i3+3,c3)+cr3*ui(i1+
     & 3,i2+6,i3+3,c3)+cr4*ui(i1+4,i2+6,i3+3,c3)+cr5*ui(i1+5,i2+6,i3+
     & 3,c3)+cr6*ui(i1+6,i2+6,i3+3,c3)+cr7*ui(i1+7,i2+6,i3+3,c3))+cs7*
     & (cr0*ui(i1,i2+7,i3+3,c3)+cr1*ui(i1+1,i2+7,i3+3,c3)+cr2*ui(i1+2,
     & i2+7,i3+3,c3)+cr3*ui(i1+3,i2+7,i3+3,c3)+cr4*ui(i1+4,i2+7,i3+3,
     & c3)+cr5*ui(i1+5,i2+7,i3+3,c3)+cr6*ui(i1+6,i2+7,i3+3,c3)+cr7*ui(
     & i1+7,i2+7,i3+3,c3)))+ct4*(cs0*(cr0*ui(i1,i2,i3+4,c3)+cr1*ui(i1+
     & 1,i2,i3+4,c3)+cr2*ui(i1+2,i2,i3+4,c3)+cr3*ui(i1+3,i2,i3+4,c3)+
     & cr4*ui(i1+4,i2,i3+4,c3)+cr5*ui(i1+5,i2,i3+4,c3)+cr6*ui(i1+6,i2,
     & i3+4,c3)+cr7*ui(i1+7,i2,i3+4,c3))+cs1*(cr0*ui(i1,i2+1,i3+4,c3)+
     & cr1*ui(i1+1,i2+1,i3+4,c3)+cr2*ui(i1+2,i2+1,i3+4,c3)+cr3*ui(i1+
     & 3,i2+1,i3+4,c3)+cr4*ui(i1+4,i2+1,i3+4,c3)+cr5*ui(i1+5,i2+1,i3+
     & 4,c3)+cr6*ui(i1+6,i2+1,i3+4,c3)+cr7*ui(i1+7,i2+1,i3+4,c3))+cs2*
     & (cr0*ui(i1,i2+2,i3+4,c3)+cr1*ui(i1+1,i2+2,i3+4,c3)+cr2*ui(i1+2,
     & i2+2,i3+4,c3)+cr3*ui(i1+3,i2+2,i3+4,c3)+cr4*ui(i1+4,i2+2,i3+4,
     & c3)+cr5*ui(i1+5,i2+2,i3+4,c3)+cr6*ui(i1+6,i2+2,i3+4,c3)+cr7*ui(
     & i1+7,i2+2,i3+4,c3))+cs3*(cr0*ui(i1,i2+3,i3+4,c3)+cr1*ui(i1+1,
     & i2+3,i3+4,c3)+cr2*ui(i1+2,i2+3,i3+4,c3)+cr3*ui(i1+3,i2+3,i3+4,
     & c3)+cr4*ui(i1+4,i2+3,i3+4,c3)+cr5*ui(i1+5,i2+3,i3+4,c3)+cr6*ui(
     & i1+6,i2+3,i3+4,c3)+cr7*ui(i1+7,i2+3,i3+4,c3))+cs4*(cr0*ui(i1,
     & i2+4,i3+4,c3)+cr1*ui(i1+1,i2+4,i3+4,c3)+cr2*ui(i1+2,i2+4,i3+4,
     & c3)+cr3*ui(i1+3,i2+4,i3+4,c3)+cr4*ui(i1+4,i2+4,i3+4,c3)+cr5*ui(
     & i1+5,i2+4,i3+4,c3)+cr6*ui(i1+6,i2+4,i3+4,c3)+cr7*ui(i1+7,i2+4,
     & i3+4,c3))+cs5*(cr0*ui(i1,i2+5,i3+4,c3)+cr1*ui(i1+1,i2+5,i3+4,
     & c3)+cr2*ui(i1+2,i2+5,i3+4,c3)+cr3*ui(i1+3,i2+5,i3+4,c3)+cr4*ui(
     & i1+4,i2+5,i3+4,c3)+cr5*ui(i1+5,i2+5,i3+4,c3)+cr6*ui(i1+6,i2+5,
     & i3+4,c3)+cr7*ui(i1+7,i2+5,i3+4,c3))+cs6*(cr0*ui(i1,i2+6,i3+4,
     & c3)+cr1*ui(i1+1,i2+6,i3+4,c3)+cr2*ui(i1+2,i2+6,i3+4,c3)+cr3*ui(
     & i1+3,i2+6,i3+4,c3)+cr4*ui(i1+4,i2+6,i3+4,c3)+cr5*ui(i1+5,i2+6,
     & i3+4,c3)+cr6*ui(i1+6,i2+6,i3+4,c3)+cr7*ui(i1+7,i2+6,i3+4,c3))+
     & cs7*(cr0*ui(i1,i2+7,i3+4,c3)+cr1*ui(i1+1,i2+7,i3+4,c3)+cr2*ui(
     & i1+2,i2+7,i3+4,c3)+cr3*ui(i1+3,i2+7,i3+4,c3)+cr4*ui(i1+4,i2+7,
     & i3+4,c3)+cr5*ui(i1+5,i2+7,i3+4,c3)+cr6*ui(i1+6,i2+7,i3+4,c3)+
     & cr7*ui(i1+7,i2+7,i3+4,c3)))+ct5*(cs0*(cr0*ui(i1,i2,i3+5,c3)+
     & cr1*ui(i1+1,i2,i3+5,c3)+cr2*ui(i1+2,i2,i3+5,c3)+cr3*ui(i1+3,i2,
     & i3+5,c3)+cr4*ui(i1+4,i2,i3+5,c3)+cr5*ui(i1+5,i2,i3+5,c3)+cr6*
     & ui(i1+6,i2,i3+5,c3)+cr7*ui(i1+7,i2,i3+5,c3))+cs1*(cr0*ui(i1,i2+
     & 1,i3+5,c3)+cr1*ui(i1+1,i2+1,i3+5,c3)+cr2*ui(i1+2,i2+1,i3+5,c3)+
     & cr3*ui(i1+3,i2+1,i3+5,c3)+cr4*ui(i1+4,i2+1,i3+5,c3)+cr5*ui(i1+
     & 5,i2+1,i3+5,c3)+cr6*ui(i1+6,i2+1,i3+5,c3)+cr7*ui(i1+7,i2+1,i3+
     & 5,c3))+cs2*(cr0*ui(i1,i2+2,i3+5,c3)+cr1*ui(i1+1,i2+2,i3+5,c3)+
     & cr2*ui(i1+2,i2+2,i3+5,c3)+cr3*ui(i1+3,i2+2,i3+5,c3)+cr4*ui(i1+
     & 4,i2+2,i3+5,c3)+cr5*ui(i1+5,i2+2,i3+5,c3)+cr6*ui(i1+6,i2+2,i3+
     & 5,c3)+cr7*ui(i1+7,i2+2,i3+5,c3))+cs3*(cr0*ui(i1,i2+3,i3+5,c3)+
     & cr1*ui(i1+1,i2+3,i3+5,c3)+cr2*ui(i1+2,i2+3,i3+5,c3)+cr3*ui(i1+
     & 3,i2+3,i3+5,c3)+cr4*ui(i1+4,i2+3,i3+5,c3)+cr5*ui(i1+5,i2+3,i3+
     & 5,c3)+cr6*ui(i1+6,i2+3,i3+5,c3)+cr7*ui(i1+7,i2+3,i3+5,c3))+cs4*
     & (cr0*ui(i1,i2+4,i3+5,c3)+cr1*ui(i1+1,i2+4,i3+5,c3)+cr2*ui(i1+2,
     & i2+4,i3+5,c3)+cr3*ui(i1+3,i2+4,i3+5,c3)+cr4*ui(i1+4,i2+4,i3+5,
     & c3)+cr5*ui(i1+5,i2+4,i3+5,c3)+cr6*ui(i1+6,i2+4,i3+5,c3)+cr7*ui(
     & i1+7,i2+4,i3+5,c3))+cs5*(cr0*ui(i1,i2+5,i3+5,c3)+cr1*ui(i1+1,
     & i2+5,i3+5,c3)+cr2*ui(i1+2,i2+5,i3+5,c3)+cr3*ui(i1+3,i2+5,i3+5,
     & c3)+cr4*ui(i1+4,i2+5,i3+5,c3)+cr5*ui(i1+5,i2+5,i3+5,c3)+cr6*ui(
     & i1+6,i2+5,i3+5,c3)+cr7*ui(i1+7,i2+5,i3+5,c3))+cs6*(cr0*ui(i1,
     & i2+6,i3+5,c3)+cr1*ui(i1+1,i2+6,i3+5,c3)+cr2*ui(i1+2,i2+6,i3+5,
     & c3)+cr3*ui(i1+3,i2+6,i3+5,c3)+cr4*ui(i1+4,i2+6,i3+5,c3)+cr5*ui(
     & i1+5,i2+6,i3+5,c3)+cr6*ui(i1+6,i2+6,i3+5,c3)+cr7*ui(i1+7,i2+6,
     & i3+5,c3))+cs7*(cr0*ui(i1,i2+7,i3+5,c3)+cr1*ui(i1+1,i2+7,i3+5,
     & c3)+cr2*ui(i1+2,i2+7,i3+5,c3)+cr3*ui(i1+3,i2+7,i3+5,c3)+cr4*ui(
     & i1+4,i2+7,i3+5,c3)+cr5*ui(i1+5,i2+7,i3+5,c3)+cr6*ui(i1+6,i2+7,
     & i3+5,c3)+cr7*ui(i1+7,i2+7,i3+5,c3)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+ct6*(cs0*(cr0*ui(i1,i2,i3+6,c3)+cr1*ui(i1+1,i2,i3+6,
     & c3)+cr2*ui(i1+2,i2,i3+6,c3)+cr3*ui(i1+3,i2,i3+6,c3)+cr4*ui(i1+
     & 4,i2,i3+6,c3)+cr5*ui(i1+5,i2,i3+6,c3)+cr6*ui(i1+6,i2,i3+6,c3)+
     & cr7*ui(i1+7,i2,i3+6,c3))+cs1*(cr0*ui(i1,i2+1,i3+6,c3)+cr1*ui(
     & i1+1,i2+1,i3+6,c3)+cr2*ui(i1+2,i2+1,i3+6,c3)+cr3*ui(i1+3,i2+1,
     & i3+6,c3)+cr4*ui(i1+4,i2+1,i3+6,c3)+cr5*ui(i1+5,i2+1,i3+6,c3)+
     & cr6*ui(i1+6,i2+1,i3+6,c3)+cr7*ui(i1+7,i2+1,i3+6,c3))+cs2*(cr0*
     & ui(i1,i2+2,i3+6,c3)+cr1*ui(i1+1,i2+2,i3+6,c3)+cr2*ui(i1+2,i2+2,
     & i3+6,c3)+cr3*ui(i1+3,i2+2,i3+6,c3)+cr4*ui(i1+4,i2+2,i3+6,c3)+
     & cr5*ui(i1+5,i2+2,i3+6,c3)+cr6*ui(i1+6,i2+2,i3+6,c3)+cr7*ui(i1+
     & 7,i2+2,i3+6,c3))+cs3*(cr0*ui(i1,i2+3,i3+6,c3)+cr1*ui(i1+1,i2+3,
     & i3+6,c3)+cr2*ui(i1+2,i2+3,i3+6,c3)+cr3*ui(i1+3,i2+3,i3+6,c3)+
     & cr4*ui(i1+4,i2+3,i3+6,c3)+cr5*ui(i1+5,i2+3,i3+6,c3)+cr6*ui(i1+
     & 6,i2+3,i3+6,c3)+cr7*ui(i1+7,i2+3,i3+6,c3))+cs4*(cr0*ui(i1,i2+4,
     & i3+6,c3)+cr1*ui(i1+1,i2+4,i3+6,c3)+cr2*ui(i1+2,i2+4,i3+6,c3)+
     & cr3*ui(i1+3,i2+4,i3+6,c3)+cr4*ui(i1+4,i2+4,i3+6,c3)+cr5*ui(i1+
     & 5,i2+4,i3+6,c3)+cr6*ui(i1+6,i2+4,i3+6,c3)+cr7*ui(i1+7,i2+4,i3+
     & 6,c3))+cs5*(cr0*ui(i1,i2+5,i3+6,c3)+cr1*ui(i1+1,i2+5,i3+6,c3)+
     & cr2*ui(i1+2,i2+5,i3+6,c3)+cr3*ui(i1+3,i2+5,i3+6,c3)+cr4*ui(i1+
     & 4,i2+5,i3+6,c3)+cr5*ui(i1+5,i2+5,i3+6,c3)+cr6*ui(i1+6,i2+5,i3+
     & 6,c3)+cr7*ui(i1+7,i2+5,i3+6,c3))+cs6*(cr0*ui(i1,i2+6,i3+6,c3)+
     & cr1*ui(i1+1,i2+6,i3+6,c3)+cr2*ui(i1+2,i2+6,i3+6,c3)+cr3*ui(i1+
     & 3,i2+6,i3+6,c3)+cr4*ui(i1+4,i2+6,i3+6,c3)+cr5*ui(i1+5,i2+6,i3+
     & 6,c3)+cr6*ui(i1+6,i2+6,i3+6,c3)+cr7*ui(i1+7,i2+6,i3+6,c3))+cs7*
     & (cr0*ui(i1,i2+7,i3+6,c3)+cr1*ui(i1+1,i2+7,i3+6,c3)+cr2*ui(i1+2,
     & i2+7,i3+6,c3)+cr3*ui(i1+3,i2+7,i3+6,c3)+cr4*ui(i1+4,i2+7,i3+6,
     & c3)+cr5*ui(i1+5,i2+7,i3+6,c3)+cr6*ui(i1+6,i2+7,i3+6,c3)+cr7*ui(
     & i1+7,i2+7,i3+6,c3)))+ct7*(cs0*(cr0*ui(i1,i2,i3+7,c3)+cr1*ui(i1+
     & 1,i2,i3+7,c3)+cr2*ui(i1+2,i2,i3+7,c3)+cr3*ui(i1+3,i2,i3+7,c3)+
     & cr4*ui(i1+4,i2,i3+7,c3)+cr5*ui(i1+5,i2,i3+7,c3)+cr6*ui(i1+6,i2,
     & i3+7,c3)+cr7*ui(i1+7,i2,i3+7,c3))+cs1*(cr0*ui(i1,i2+1,i3+7,c3)+
     & cr1*ui(i1+1,i2+1,i3+7,c3)+cr2*ui(i1+2,i2+1,i3+7,c3)+cr3*ui(i1+
     & 3,i2+1,i3+7,c3)+cr4*ui(i1+4,i2+1,i3+7,c3)+cr5*ui(i1+5,i2+1,i3+
     & 7,c3)+cr6*ui(i1+6,i2+1,i3+7,c3)+cr7*ui(i1+7,i2+1,i3+7,c3))+cs2*
     & (cr0*ui(i1,i2+2,i3+7,c3)+cr1*ui(i1+1,i2+2,i3+7,c3)+cr2*ui(i1+2,
     & i2+2,i3+7,c3)+cr3*ui(i1+3,i2+2,i3+7,c3)+cr4*ui(i1+4,i2+2,i3+7,
     & c3)+cr5*ui(i1+5,i2+2,i3+7,c3)+cr6*ui(i1+6,i2+2,i3+7,c3)+cr7*ui(
     & i1+7,i2+2,i3+7,c3))+cs3*(cr0*ui(i1,i2+3,i3+7,c3)+cr1*ui(i1+1,
     & i2+3,i3+7,c3)+cr2*ui(i1+2,i2+3,i3+7,c3)+cr3*ui(i1+3,i2+3,i3+7,
     & c3)+cr4*ui(i1+4,i2+3,i3+7,c3)+cr5*ui(i1+5,i2+3,i3+7,c3)+cr6*ui(
     & i1+6,i2+3,i3+7,c3)+cr7*ui(i1+7,i2+3,i3+7,c3))+cs4*(cr0*ui(i1,
     & i2+4,i3+7,c3)+cr1*ui(i1+1,i2+4,i3+7,c3)+cr2*ui(i1+2,i2+4,i3+7,
     & c3)+cr3*ui(i1+3,i2+4,i3+7,c3)+cr4*ui(i1+4,i2+4,i3+7,c3)+cr5*ui(
     & i1+5,i2+4,i3+7,c3)+cr6*ui(i1+6,i2+4,i3+7,c3)+cr7*ui(i1+7,i2+4,
     & i3+7,c3))+cs5*(cr0*ui(i1,i2+5,i3+7,c3)+cr1*ui(i1+1,i2+5,i3+7,
     & c3)+cr2*ui(i1+2,i2+5,i3+7,c3)+cr3*ui(i1+3,i2+5,i3+7,c3)+cr4*ui(
     & i1+4,i2+5,i3+7,c3)+cr5*ui(i1+5,i2+5,i3+7,c3)+cr6*ui(i1+6,i2+5,
     & i3+7,c3)+cr7*ui(i1+7,i2+5,i3+7,c3))+cs6*(cr0*ui(i1,i2+6,i3+7,
     & c3)+cr1*ui(i1+1,i2+6,i3+7,c3)+cr2*ui(i1+2,i2+6,i3+7,c3)+cr3*ui(
     & i1+3,i2+6,i3+7,c3)+cr4*ui(i1+4,i2+6,i3+7,c3)+cr5*ui(i1+5,i2+6,
     & i3+7,c3)+cr6*ui(i1+6,i2+6,i3+7,c3)+cr7*ui(i1+7,i2+6,i3+7,c3))+
     & cs7*(cr0*ui(i1,i2+7,i3+7,c3)+cr1*ui(i1+1,i2+7,i3+7,c3)+cr2*ui(
     & i1+2,i2+7,i3+7,c3)+cr3*ui(i1+3,i2+7,i3+7,c3)+cr4*ui(i1+4,i2+7,
     & i3+7,c3)+cr5*ui(i1+5,i2+7,i3+7,c3)+cr6*ui(i1+6,i2+7,i3+7,c3)+
     & cr7*ui(i1+7,i2+7,i3+7,c3)))


             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
! interpSparseStorage888(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q80(c(i,0,0,0))
                 cs0 = q80(c(i,1,0,0))
                 ct0 = q80(c(i,2,0,0))
                 cr1 = q81(c(i,0,0,0))
                 cs1 = q81(c(i,1,0,0))
                 ct1 = q81(c(i,2,0,0))
                 cr2 = q82(c(i,0,0,0))
                 cs2 = q82(c(i,1,0,0))
                 ct2 = q82(c(i,2,0,0))
                 cr3 = q83(c(i,0,0,0))
                 cs3 = q83(c(i,1,0,0))
                 ct3 = q83(c(i,2,0,0))
                 cr4 = q84(c(i,0,0,0))
                 cs4 = q84(c(i,1,0,0))
                 ct4 = q84(c(i,2,0,0))
                 cr5 = q85(c(i,0,0,0))
                 cs5 = q85(c(i,1,0,0))
                 ct5 = q85(c(i,2,0,0))
                 cr6 = q86(c(i,0,0,0))
                 cs6 = q86(c(i,1,0,0))
                 ct6 = q86(c(i,2,0,0))
                 cr7 = q87(c(i,0,0,0))
                 cs7 = q87(c(i,1,0,0))
                 ct7 = q87(c(i,2,0,0))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ct0*(cs0*(cr0*ui(i1,i2,
     & i3+0,c3)+cr1*ui(i1+1,i2,i3+0,c3)+cr2*ui(i1+2,i2,i3+0,c3)+cr3*
     & ui(i1+3,i2,i3+0,c3)+cr4*ui(i1+4,i2,i3+0,c3)+cr5*ui(i1+5,i2,i3+
     & 0,c3)+cr6*ui(i1+6,i2,i3+0,c3)+cr7*ui(i1+7,i2,i3+0,c3))+cs1*(
     & cr0*ui(i1,i2+1,i3+0,c3)+cr1*ui(i1+1,i2+1,i3+0,c3)+cr2*ui(i1+2,
     & i2+1,i3+0,c3)+cr3*ui(i1+3,i2+1,i3+0,c3)+cr4*ui(i1+4,i2+1,i3+0,
     & c3)+cr5*ui(i1+5,i2+1,i3+0,c3)+cr6*ui(i1+6,i2+1,i3+0,c3)+cr7*ui(
     & i1+7,i2+1,i3+0,c3))+cs2*(cr0*ui(i1,i2+2,i3+0,c3)+cr1*ui(i1+1,
     & i2+2,i3+0,c3)+cr2*ui(i1+2,i2+2,i3+0,c3)+cr3*ui(i1+3,i2+2,i3+0,
     & c3)+cr4*ui(i1+4,i2+2,i3+0,c3)+cr5*ui(i1+5,i2+2,i3+0,c3)+cr6*ui(
     & i1+6,i2+2,i3+0,c3)+cr7*ui(i1+7,i2+2,i3+0,c3))+cs3*(cr0*ui(i1,
     & i2+3,i3+0,c3)+cr1*ui(i1+1,i2+3,i3+0,c3)+cr2*ui(i1+2,i2+3,i3+0,
     & c3)+cr3*ui(i1+3,i2+3,i3+0,c3)+cr4*ui(i1+4,i2+3,i3+0,c3)+cr5*ui(
     & i1+5,i2+3,i3+0,c3)+cr6*ui(i1+6,i2+3,i3+0,c3)+cr7*ui(i1+7,i2+3,
     & i3+0,c3))+cs4*(cr0*ui(i1,i2+4,i3+0,c3)+cr1*ui(i1+1,i2+4,i3+0,
     & c3)+cr2*ui(i1+2,i2+4,i3+0,c3)+cr3*ui(i1+3,i2+4,i3+0,c3)+cr4*ui(
     & i1+4,i2+4,i3+0,c3)+cr5*ui(i1+5,i2+4,i3+0,c3)+cr6*ui(i1+6,i2+4,
     & i3+0,c3)+cr7*ui(i1+7,i2+4,i3+0,c3))+cs5*(cr0*ui(i1,i2+5,i3+0,
     & c3)+cr1*ui(i1+1,i2+5,i3+0,c3)+cr2*ui(i1+2,i2+5,i3+0,c3)+cr3*ui(
     & i1+3,i2+5,i3+0,c3)+cr4*ui(i1+4,i2+5,i3+0,c3)+cr5*ui(i1+5,i2+5,
     & i3+0,c3)+cr6*ui(i1+6,i2+5,i3+0,c3)+cr7*ui(i1+7,i2+5,i3+0,c3))+
     & cs6*(cr0*ui(i1,i2+6,i3+0,c3)+cr1*ui(i1+1,i2+6,i3+0,c3)+cr2*ui(
     & i1+2,i2+6,i3+0,c3)+cr3*ui(i1+3,i2+6,i3+0,c3)+cr4*ui(i1+4,i2+6,
     & i3+0,c3)+cr5*ui(i1+5,i2+6,i3+0,c3)+cr6*ui(i1+6,i2+6,i3+0,c3)+
     & cr7*ui(i1+7,i2+6,i3+0,c3))+cs7*(cr0*ui(i1,i2+7,i3+0,c3)+cr1*ui(
     & i1+1,i2+7,i3+0,c3)+cr2*ui(i1+2,i2+7,i3+0,c3)+cr3*ui(i1+3,i2+7,
     & i3+0,c3)+cr4*ui(i1+4,i2+7,i3+0,c3)+cr5*ui(i1+5,i2+7,i3+0,c3)+
     & cr6*ui(i1+6,i2+7,i3+0,c3)+cr7*ui(i1+7,i2+7,i3+0,c3)))+ct1*(cs0*
     & (cr0*ui(i1,i2,i3+1,c3)+cr1*ui(i1+1,i2,i3+1,c3)+cr2*ui(i1+2,i2,
     & i3+1,c3)+cr3*ui(i1+3,i2,i3+1,c3)+cr4*ui(i1+4,i2,i3+1,c3)+cr5*
     & ui(i1+5,i2,i3+1,c3)+cr6*ui(i1+6,i2,i3+1,c3)+cr7*ui(i1+7,i2,i3+
     & 1,c3))+cs1*(cr0*ui(i1,i2+1,i3+1,c3)+cr1*ui(i1+1,i2+1,i3+1,c3)+
     & cr2*ui(i1+2,i2+1,i3+1,c3)+cr3*ui(i1+3,i2+1,i3+1,c3)+cr4*ui(i1+
     & 4,i2+1,i3+1,c3)+cr5*ui(i1+5,i2+1,i3+1,c3)+cr6*ui(i1+6,i2+1,i3+
     & 1,c3)+cr7*ui(i1+7,i2+1,i3+1,c3))+cs2*(cr0*ui(i1,i2+2,i3+1,c3)+
     & cr1*ui(i1+1,i2+2,i3+1,c3)+cr2*ui(i1+2,i2+2,i3+1,c3)+cr3*ui(i1+
     & 3,i2+2,i3+1,c3)+cr4*ui(i1+4,i2+2,i3+1,c3)+cr5*ui(i1+5,i2+2,i3+
     & 1,c3)+cr6*ui(i1+6,i2+2,i3+1,c3)+cr7*ui(i1+7,i2+2,i3+1,c3))+cs3*
     & (cr0*ui(i1,i2+3,i3+1,c3)+cr1*ui(i1+1,i2+3,i3+1,c3)+cr2*ui(i1+2,
     & i2+3,i3+1,c3)+cr3*ui(i1+3,i2+3,i3+1,c3)+cr4*ui(i1+4,i2+3,i3+1,
     & c3)+cr5*ui(i1+5,i2+3,i3+1,c3)+cr6*ui(i1+6,i2+3,i3+1,c3)+cr7*ui(
     & i1+7,i2+3,i3+1,c3))+cs4*(cr0*ui(i1,i2+4,i3+1,c3)+cr1*ui(i1+1,
     & i2+4,i3+1,c3)+cr2*ui(i1+2,i2+4,i3+1,c3)+cr3*ui(i1+3,i2+4,i3+1,
     & c3)+cr4*ui(i1+4,i2+4,i3+1,c3)+cr5*ui(i1+5,i2+4,i3+1,c3)+cr6*ui(
     & i1+6,i2+4,i3+1,c3)+cr7*ui(i1+7,i2+4,i3+1,c3))+cs5*(cr0*ui(i1,
     & i2+5,i3+1,c3)+cr1*ui(i1+1,i2+5,i3+1,c3)+cr2*ui(i1+2,i2+5,i3+1,
     & c3)+cr3*ui(i1+3,i2+5,i3+1,c3)+cr4*ui(i1+4,i2+5,i3+1,c3)+cr5*ui(
     & i1+5,i2+5,i3+1,c3)+cr6*ui(i1+6,i2+5,i3+1,c3)+cr7*ui(i1+7,i2+5,
     & i3+1,c3))+cs6*(cr0*ui(i1,i2+6,i3+1,c3)+cr1*ui(i1+1,i2+6,i3+1,
     & c3)+cr2*ui(i1+2,i2+6,i3+1,c3)+cr3*ui(i1+3,i2+6,i3+1,c3)+cr4*ui(
     & i1+4,i2+6,i3+1,c3)+cr5*ui(i1+5,i2+6,i3+1,c3)+cr6*ui(i1+6,i2+6,
     & i3+1,c3)+cr7*ui(i1+7,i2+6,i3+1,c3))+cs7*(cr0*ui(i1,i2+7,i3+1,
     & c3)+cr1*ui(i1+1,i2+7,i3+1,c3)+cr2*ui(i1+2,i2+7,i3+1,c3)+cr3*ui(
     & i1+3,i2+7,i3+1,c3)+cr4*ui(i1+4,i2+7,i3+1,c3)+cr5*ui(i1+5,i2+7,
     & i3+1,c3)+cr6*ui(i1+6,i2+7,i3+1,c3)+cr7*ui(i1+7,i2+7,i3+1,c3)))+
     & ct2*(cs0*(cr0*ui(i1,i2,i3+2,c3)+cr1*ui(i1+1,i2,i3+2,c3)+cr2*ui(
     & i1+2,i2,i3+2,c3)+cr3*ui(i1+3,i2,i3+2,c3)+cr4*ui(i1+4,i2,i3+2,
     & c3)+cr5*ui(i1+5,i2,i3+2,c3)+cr6*ui(i1+6,i2,i3+2,c3)+cr7*ui(i1+
     & 7,i2,i3+2,c3))+cs1*(cr0*ui(i1,i2+1,i3+2,c3)+cr1*ui(i1+1,i2+1,
     & i3+2,c3)+cr2*ui(i1+2,i2+1,i3+2,c3)+cr3*ui(i1+3,i2+1,i3+2,c3)+
     & cr4*ui(i1+4,i2+1,i3+2,c3)+cr5*ui(i1+5,i2+1,i3+2,c3)+cr6*ui(i1+
     & 6,i2+1,i3+2,c3)+cr7*ui(i1+7,i2+1,i3+2,c3))+cs2*(cr0*ui(i1,i2+2,
     & i3+2,c3)+cr1*ui(i1+1,i2+2,i3+2,c3)+cr2*ui(i1+2,i2+2,i3+2,c3)+
     & cr3*ui(i1+3,i2+2,i3+2,c3)+cr4*ui(i1+4,i2+2,i3+2,c3)+cr5*ui(i1+
     & 5,i2+2,i3+2,c3)+cr6*ui(i1+6,i2+2,i3+2,c3)+cr7*ui(i1+7,i2+2,i3+
     & 2,c3))+cs3*(cr0*ui(i1,i2+3,i3+2,c3)+cr1*ui(i1+1,i2+3,i3+2,c3)+
     & cr2*ui(i1+2,i2+3,i3+2,c3)+cr3*ui(i1+3,i2+3,i3+2,c3)+cr4*ui(i1+
     & 4,i2+3,i3+2,c3)+cr5*ui(i1+5,i2+3,i3+2,c3)+cr6*ui(i1+6,i2+3,i3+
     & 2,c3)+cr7*ui(i1+7,i2+3,i3+2,c3))+cs4*(cr0*ui(i1,i2+4,i3+2,c3)+
     & cr1*ui(i1+1,i2+4,i3+2,c3)+cr2*ui(i1+2,i2+4,i3+2,c3)+cr3*ui(i1+
     & 3,i2+4,i3+2,c3)+cr4*ui(i1+4,i2+4,i3+2,c3)+cr5*ui(i1+5,i2+4,i3+
     & 2,c3)+cr6*ui(i1+6,i2+4,i3+2,c3)+cr7*ui(i1+7,i2+4,i3+2,c3))+cs5*
     & (cr0*ui(i1,i2+5,i3+2,c3)+cr1*ui(i1+1,i2+5,i3+2,c3)+cr2*ui(i1+2,
     & i2+5,i3+2,c3)+cr3*ui(i1+3,i2+5,i3+2,c3)+cr4*ui(i1+4,i2+5,i3+2,
     & c3)+cr5*ui(i1+5,i2+5,i3+2,c3)+cr6*ui(i1+6,i2+5,i3+2,c3)+cr7*ui(
     & i1+7,i2+5,i3+2,c3))+cs6*(cr0*ui(i1,i2+6,i3+2,c3)+cr1*ui(i1+1,
     & i2+6,i3+2,c3)+cr2*ui(i1+2,i2+6,i3+2,c3)+cr3*ui(i1+3,i2+6,i3+2,
     & c3)+cr4*ui(i1+4,i2+6,i3+2,c3)+cr5*ui(i1+5,i2+6,i3+2,c3)+cr6*ui(
     & i1+6,i2+6,i3+2,c3)+cr7*ui(i1+7,i2+6,i3+2,c3))+cs7*(cr0*ui(i1,
     & i2+7,i3+2,c3)+cr1*ui(i1+1,i2+7,i3+2,c3)+cr2*ui(i1+2,i2+7,i3+2,
     & c3)+cr3*ui(i1+3,i2+7,i3+2,c3)+cr4*ui(i1+4,i2+7,i3+2,c3)+cr5*ui(
     & i1+5,i2+7,i3+2,c3)+cr6*ui(i1+6,i2+7,i3+2,c3)+cr7*ui(i1+7,i2+7,
     & i3+2,c3)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+ct3*(cs0*(cr0*ui(i1,i2,i3+3,c3)+cr1*ui(i1+1,i2,i3+3,
     & c3)+cr2*ui(i1+2,i2,i3+3,c3)+cr3*ui(i1+3,i2,i3+3,c3)+cr4*ui(i1+
     & 4,i2,i3+3,c3)+cr5*ui(i1+5,i2,i3+3,c3)+cr6*ui(i1+6,i2,i3+3,c3)+
     & cr7*ui(i1+7,i2,i3+3,c3))+cs1*(cr0*ui(i1,i2+1,i3+3,c3)+cr1*ui(
     & i1+1,i2+1,i3+3,c3)+cr2*ui(i1+2,i2+1,i3+3,c3)+cr3*ui(i1+3,i2+1,
     & i3+3,c3)+cr4*ui(i1+4,i2+1,i3+3,c3)+cr5*ui(i1+5,i2+1,i3+3,c3)+
     & cr6*ui(i1+6,i2+1,i3+3,c3)+cr7*ui(i1+7,i2+1,i3+3,c3))+cs2*(cr0*
     & ui(i1,i2+2,i3+3,c3)+cr1*ui(i1+1,i2+2,i3+3,c3)+cr2*ui(i1+2,i2+2,
     & i3+3,c3)+cr3*ui(i1+3,i2+2,i3+3,c3)+cr4*ui(i1+4,i2+2,i3+3,c3)+
     & cr5*ui(i1+5,i2+2,i3+3,c3)+cr6*ui(i1+6,i2+2,i3+3,c3)+cr7*ui(i1+
     & 7,i2+2,i3+3,c3))+cs3*(cr0*ui(i1,i2+3,i3+3,c3)+cr1*ui(i1+1,i2+3,
     & i3+3,c3)+cr2*ui(i1+2,i2+3,i3+3,c3)+cr3*ui(i1+3,i2+3,i3+3,c3)+
     & cr4*ui(i1+4,i2+3,i3+3,c3)+cr5*ui(i1+5,i2+3,i3+3,c3)+cr6*ui(i1+
     & 6,i2+3,i3+3,c3)+cr7*ui(i1+7,i2+3,i3+3,c3))+cs4*(cr0*ui(i1,i2+4,
     & i3+3,c3)+cr1*ui(i1+1,i2+4,i3+3,c3)+cr2*ui(i1+2,i2+4,i3+3,c3)+
     & cr3*ui(i1+3,i2+4,i3+3,c3)+cr4*ui(i1+4,i2+4,i3+3,c3)+cr5*ui(i1+
     & 5,i2+4,i3+3,c3)+cr6*ui(i1+6,i2+4,i3+3,c3)+cr7*ui(i1+7,i2+4,i3+
     & 3,c3))+cs5*(cr0*ui(i1,i2+5,i3+3,c3)+cr1*ui(i1+1,i2+5,i3+3,c3)+
     & cr2*ui(i1+2,i2+5,i3+3,c3)+cr3*ui(i1+3,i2+5,i3+3,c3)+cr4*ui(i1+
     & 4,i2+5,i3+3,c3)+cr5*ui(i1+5,i2+5,i3+3,c3)+cr6*ui(i1+6,i2+5,i3+
     & 3,c3)+cr7*ui(i1+7,i2+5,i3+3,c3))+cs6*(cr0*ui(i1,i2+6,i3+3,c3)+
     & cr1*ui(i1+1,i2+6,i3+3,c3)+cr2*ui(i1+2,i2+6,i3+3,c3)+cr3*ui(i1+
     & 3,i2+6,i3+3,c3)+cr4*ui(i1+4,i2+6,i3+3,c3)+cr5*ui(i1+5,i2+6,i3+
     & 3,c3)+cr6*ui(i1+6,i2+6,i3+3,c3)+cr7*ui(i1+7,i2+6,i3+3,c3))+cs7*
     & (cr0*ui(i1,i2+7,i3+3,c3)+cr1*ui(i1+1,i2+7,i3+3,c3)+cr2*ui(i1+2,
     & i2+7,i3+3,c3)+cr3*ui(i1+3,i2+7,i3+3,c3)+cr4*ui(i1+4,i2+7,i3+3,
     & c3)+cr5*ui(i1+5,i2+7,i3+3,c3)+cr6*ui(i1+6,i2+7,i3+3,c3)+cr7*ui(
     & i1+7,i2+7,i3+3,c3)))+ct4*(cs0*(cr0*ui(i1,i2,i3+4,c3)+cr1*ui(i1+
     & 1,i2,i3+4,c3)+cr2*ui(i1+2,i2,i3+4,c3)+cr3*ui(i1+3,i2,i3+4,c3)+
     & cr4*ui(i1+4,i2,i3+4,c3)+cr5*ui(i1+5,i2,i3+4,c3)+cr6*ui(i1+6,i2,
     & i3+4,c3)+cr7*ui(i1+7,i2,i3+4,c3))+cs1*(cr0*ui(i1,i2+1,i3+4,c3)+
     & cr1*ui(i1+1,i2+1,i3+4,c3)+cr2*ui(i1+2,i2+1,i3+4,c3)+cr3*ui(i1+
     & 3,i2+1,i3+4,c3)+cr4*ui(i1+4,i2+1,i3+4,c3)+cr5*ui(i1+5,i2+1,i3+
     & 4,c3)+cr6*ui(i1+6,i2+1,i3+4,c3)+cr7*ui(i1+7,i2+1,i3+4,c3))+cs2*
     & (cr0*ui(i1,i2+2,i3+4,c3)+cr1*ui(i1+1,i2+2,i3+4,c3)+cr2*ui(i1+2,
     & i2+2,i3+4,c3)+cr3*ui(i1+3,i2+2,i3+4,c3)+cr4*ui(i1+4,i2+2,i3+4,
     & c3)+cr5*ui(i1+5,i2+2,i3+4,c3)+cr6*ui(i1+6,i2+2,i3+4,c3)+cr7*ui(
     & i1+7,i2+2,i3+4,c3))+cs3*(cr0*ui(i1,i2+3,i3+4,c3)+cr1*ui(i1+1,
     & i2+3,i3+4,c3)+cr2*ui(i1+2,i2+3,i3+4,c3)+cr3*ui(i1+3,i2+3,i3+4,
     & c3)+cr4*ui(i1+4,i2+3,i3+4,c3)+cr5*ui(i1+5,i2+3,i3+4,c3)+cr6*ui(
     & i1+6,i2+3,i3+4,c3)+cr7*ui(i1+7,i2+3,i3+4,c3))+cs4*(cr0*ui(i1,
     & i2+4,i3+4,c3)+cr1*ui(i1+1,i2+4,i3+4,c3)+cr2*ui(i1+2,i2+4,i3+4,
     & c3)+cr3*ui(i1+3,i2+4,i3+4,c3)+cr4*ui(i1+4,i2+4,i3+4,c3)+cr5*ui(
     & i1+5,i2+4,i3+4,c3)+cr6*ui(i1+6,i2+4,i3+4,c3)+cr7*ui(i1+7,i2+4,
     & i3+4,c3))+cs5*(cr0*ui(i1,i2+5,i3+4,c3)+cr1*ui(i1+1,i2+5,i3+4,
     & c3)+cr2*ui(i1+2,i2+5,i3+4,c3)+cr3*ui(i1+3,i2+5,i3+4,c3)+cr4*ui(
     & i1+4,i2+5,i3+4,c3)+cr5*ui(i1+5,i2+5,i3+4,c3)+cr6*ui(i1+6,i2+5,
     & i3+4,c3)+cr7*ui(i1+7,i2+5,i3+4,c3))+cs6*(cr0*ui(i1,i2+6,i3+4,
     & c3)+cr1*ui(i1+1,i2+6,i3+4,c3)+cr2*ui(i1+2,i2+6,i3+4,c3)+cr3*ui(
     & i1+3,i2+6,i3+4,c3)+cr4*ui(i1+4,i2+6,i3+4,c3)+cr5*ui(i1+5,i2+6,
     & i3+4,c3)+cr6*ui(i1+6,i2+6,i3+4,c3)+cr7*ui(i1+7,i2+6,i3+4,c3))+
     & cs7*(cr0*ui(i1,i2+7,i3+4,c3)+cr1*ui(i1+1,i2+7,i3+4,c3)+cr2*ui(
     & i1+2,i2+7,i3+4,c3)+cr3*ui(i1+3,i2+7,i3+4,c3)+cr4*ui(i1+4,i2+7,
     & i3+4,c3)+cr5*ui(i1+5,i2+7,i3+4,c3)+cr6*ui(i1+6,i2+7,i3+4,c3)+
     & cr7*ui(i1+7,i2+7,i3+4,c3)))+ct5*(cs0*(cr0*ui(i1,i2,i3+5,c3)+
     & cr1*ui(i1+1,i2,i3+5,c3)+cr2*ui(i1+2,i2,i3+5,c3)+cr3*ui(i1+3,i2,
     & i3+5,c3)+cr4*ui(i1+4,i2,i3+5,c3)+cr5*ui(i1+5,i2,i3+5,c3)+cr6*
     & ui(i1+6,i2,i3+5,c3)+cr7*ui(i1+7,i2,i3+5,c3))+cs1*(cr0*ui(i1,i2+
     & 1,i3+5,c3)+cr1*ui(i1+1,i2+1,i3+5,c3)+cr2*ui(i1+2,i2+1,i3+5,c3)+
     & cr3*ui(i1+3,i2+1,i3+5,c3)+cr4*ui(i1+4,i2+1,i3+5,c3)+cr5*ui(i1+
     & 5,i2+1,i3+5,c3)+cr6*ui(i1+6,i2+1,i3+5,c3)+cr7*ui(i1+7,i2+1,i3+
     & 5,c3))+cs2*(cr0*ui(i1,i2+2,i3+5,c3)+cr1*ui(i1+1,i2+2,i3+5,c3)+
     & cr2*ui(i1+2,i2+2,i3+5,c3)+cr3*ui(i1+3,i2+2,i3+5,c3)+cr4*ui(i1+
     & 4,i2+2,i3+5,c3)+cr5*ui(i1+5,i2+2,i3+5,c3)+cr6*ui(i1+6,i2+2,i3+
     & 5,c3)+cr7*ui(i1+7,i2+2,i3+5,c3))+cs3*(cr0*ui(i1,i2+3,i3+5,c3)+
     & cr1*ui(i1+1,i2+3,i3+5,c3)+cr2*ui(i1+2,i2+3,i3+5,c3)+cr3*ui(i1+
     & 3,i2+3,i3+5,c3)+cr4*ui(i1+4,i2+3,i3+5,c3)+cr5*ui(i1+5,i2+3,i3+
     & 5,c3)+cr6*ui(i1+6,i2+3,i3+5,c3)+cr7*ui(i1+7,i2+3,i3+5,c3))+cs4*
     & (cr0*ui(i1,i2+4,i3+5,c3)+cr1*ui(i1+1,i2+4,i3+5,c3)+cr2*ui(i1+2,
     & i2+4,i3+5,c3)+cr3*ui(i1+3,i2+4,i3+5,c3)+cr4*ui(i1+4,i2+4,i3+5,
     & c3)+cr5*ui(i1+5,i2+4,i3+5,c3)+cr6*ui(i1+6,i2+4,i3+5,c3)+cr7*ui(
     & i1+7,i2+4,i3+5,c3))+cs5*(cr0*ui(i1,i2+5,i3+5,c3)+cr1*ui(i1+1,
     & i2+5,i3+5,c3)+cr2*ui(i1+2,i2+5,i3+5,c3)+cr3*ui(i1+3,i2+5,i3+5,
     & c3)+cr4*ui(i1+4,i2+5,i3+5,c3)+cr5*ui(i1+5,i2+5,i3+5,c3)+cr6*ui(
     & i1+6,i2+5,i3+5,c3)+cr7*ui(i1+7,i2+5,i3+5,c3))+cs6*(cr0*ui(i1,
     & i2+6,i3+5,c3)+cr1*ui(i1+1,i2+6,i3+5,c3)+cr2*ui(i1+2,i2+6,i3+5,
     & c3)+cr3*ui(i1+3,i2+6,i3+5,c3)+cr4*ui(i1+4,i2+6,i3+5,c3)+cr5*ui(
     & i1+5,i2+6,i3+5,c3)+cr6*ui(i1+6,i2+6,i3+5,c3)+cr7*ui(i1+7,i2+6,
     & i3+5,c3))+cs7*(cr0*ui(i1,i2+7,i3+5,c3)+cr1*ui(i1+1,i2+7,i3+5,
     & c3)+cr2*ui(i1+2,i2+7,i3+5,c3)+cr3*ui(i1+3,i2+7,i3+5,c3)+cr4*ui(
     & i1+4,i2+7,i3+5,c3)+cr5*ui(i1+5,i2+7,i3+5,c3)+cr6*ui(i1+6,i2+7,
     & i3+5,c3)+cr7*ui(i1+7,i2+7,i3+5,c3)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+ct6*(cs0*(cr0*ui(i1,i2,i3+6,c3)+cr1*ui(i1+1,i2,i3+6,
     & c3)+cr2*ui(i1+2,i2,i3+6,c3)+cr3*ui(i1+3,i2,i3+6,c3)+cr4*ui(i1+
     & 4,i2,i3+6,c3)+cr5*ui(i1+5,i2,i3+6,c3)+cr6*ui(i1+6,i2,i3+6,c3)+
     & cr7*ui(i1+7,i2,i3+6,c3))+cs1*(cr0*ui(i1,i2+1,i3+6,c3)+cr1*ui(
     & i1+1,i2+1,i3+6,c3)+cr2*ui(i1+2,i2+1,i3+6,c3)+cr3*ui(i1+3,i2+1,
     & i3+6,c3)+cr4*ui(i1+4,i2+1,i3+6,c3)+cr5*ui(i1+5,i2+1,i3+6,c3)+
     & cr6*ui(i1+6,i2+1,i3+6,c3)+cr7*ui(i1+7,i2+1,i3+6,c3))+cs2*(cr0*
     & ui(i1,i2+2,i3+6,c3)+cr1*ui(i1+1,i2+2,i3+6,c3)+cr2*ui(i1+2,i2+2,
     & i3+6,c3)+cr3*ui(i1+3,i2+2,i3+6,c3)+cr4*ui(i1+4,i2+2,i3+6,c3)+
     & cr5*ui(i1+5,i2+2,i3+6,c3)+cr6*ui(i1+6,i2+2,i3+6,c3)+cr7*ui(i1+
     & 7,i2+2,i3+6,c3))+cs3*(cr0*ui(i1,i2+3,i3+6,c3)+cr1*ui(i1+1,i2+3,
     & i3+6,c3)+cr2*ui(i1+2,i2+3,i3+6,c3)+cr3*ui(i1+3,i2+3,i3+6,c3)+
     & cr4*ui(i1+4,i2+3,i3+6,c3)+cr5*ui(i1+5,i2+3,i3+6,c3)+cr6*ui(i1+
     & 6,i2+3,i3+6,c3)+cr7*ui(i1+7,i2+3,i3+6,c3))+cs4*(cr0*ui(i1,i2+4,
     & i3+6,c3)+cr1*ui(i1+1,i2+4,i3+6,c3)+cr2*ui(i1+2,i2+4,i3+6,c3)+
     & cr3*ui(i1+3,i2+4,i3+6,c3)+cr4*ui(i1+4,i2+4,i3+6,c3)+cr5*ui(i1+
     & 5,i2+4,i3+6,c3)+cr6*ui(i1+6,i2+4,i3+6,c3)+cr7*ui(i1+7,i2+4,i3+
     & 6,c3))+cs5*(cr0*ui(i1,i2+5,i3+6,c3)+cr1*ui(i1+1,i2+5,i3+6,c3)+
     & cr2*ui(i1+2,i2+5,i3+6,c3)+cr3*ui(i1+3,i2+5,i3+6,c3)+cr4*ui(i1+
     & 4,i2+5,i3+6,c3)+cr5*ui(i1+5,i2+5,i3+6,c3)+cr6*ui(i1+6,i2+5,i3+
     & 6,c3)+cr7*ui(i1+7,i2+5,i3+6,c3))+cs6*(cr0*ui(i1,i2+6,i3+6,c3)+
     & cr1*ui(i1+1,i2+6,i3+6,c3)+cr2*ui(i1+2,i2+6,i3+6,c3)+cr3*ui(i1+
     & 3,i2+6,i3+6,c3)+cr4*ui(i1+4,i2+6,i3+6,c3)+cr5*ui(i1+5,i2+6,i3+
     & 6,c3)+cr6*ui(i1+6,i2+6,i3+6,c3)+cr7*ui(i1+7,i2+6,i3+6,c3))+cs7*
     & (cr0*ui(i1,i2+7,i3+6,c3)+cr1*ui(i1+1,i2+7,i3+6,c3)+cr2*ui(i1+2,
     & i2+7,i3+6,c3)+cr3*ui(i1+3,i2+7,i3+6,c3)+cr4*ui(i1+4,i2+7,i3+6,
     & c3)+cr5*ui(i1+5,i2+7,i3+6,c3)+cr6*ui(i1+6,i2+7,i3+6,c3)+cr7*ui(
     & i1+7,i2+7,i3+6,c3)))+ct7*(cs0*(cr0*ui(i1,i2,i3+7,c3)+cr1*ui(i1+
     & 1,i2,i3+7,c3)+cr2*ui(i1+2,i2,i3+7,c3)+cr3*ui(i1+3,i2,i3+7,c3)+
     & cr4*ui(i1+4,i2,i3+7,c3)+cr5*ui(i1+5,i2,i3+7,c3)+cr6*ui(i1+6,i2,
     & i3+7,c3)+cr7*ui(i1+7,i2,i3+7,c3))+cs1*(cr0*ui(i1,i2+1,i3+7,c3)+
     & cr1*ui(i1+1,i2+1,i3+7,c3)+cr2*ui(i1+2,i2+1,i3+7,c3)+cr3*ui(i1+
     & 3,i2+1,i3+7,c3)+cr4*ui(i1+4,i2+1,i3+7,c3)+cr5*ui(i1+5,i2+1,i3+
     & 7,c3)+cr6*ui(i1+6,i2+1,i3+7,c3)+cr7*ui(i1+7,i2+1,i3+7,c3))+cs2*
     & (cr0*ui(i1,i2+2,i3+7,c3)+cr1*ui(i1+1,i2+2,i3+7,c3)+cr2*ui(i1+2,
     & i2+2,i3+7,c3)+cr3*ui(i1+3,i2+2,i3+7,c3)+cr4*ui(i1+4,i2+2,i3+7,
     & c3)+cr5*ui(i1+5,i2+2,i3+7,c3)+cr6*ui(i1+6,i2+2,i3+7,c3)+cr7*ui(
     & i1+7,i2+2,i3+7,c3))+cs3*(cr0*ui(i1,i2+3,i3+7,c3)+cr1*ui(i1+1,
     & i2+3,i3+7,c3)+cr2*ui(i1+2,i2+3,i3+7,c3)+cr3*ui(i1+3,i2+3,i3+7,
     & c3)+cr4*ui(i1+4,i2+3,i3+7,c3)+cr5*ui(i1+5,i2+3,i3+7,c3)+cr6*ui(
     & i1+6,i2+3,i3+7,c3)+cr7*ui(i1+7,i2+3,i3+7,c3))+cs4*(cr0*ui(i1,
     & i2+4,i3+7,c3)+cr1*ui(i1+1,i2+4,i3+7,c3)+cr2*ui(i1+2,i2+4,i3+7,
     & c3)+cr3*ui(i1+3,i2+4,i3+7,c3)+cr4*ui(i1+4,i2+4,i3+7,c3)+cr5*ui(
     & i1+5,i2+4,i3+7,c3)+cr6*ui(i1+6,i2+4,i3+7,c3)+cr7*ui(i1+7,i2+4,
     & i3+7,c3))+cs5*(cr0*ui(i1,i2+5,i3+7,c3)+cr1*ui(i1+1,i2+5,i3+7,
     & c3)+cr2*ui(i1+2,i2+5,i3+7,c3)+cr3*ui(i1+3,i2+5,i3+7,c3)+cr4*ui(
     & i1+4,i2+5,i3+7,c3)+cr5*ui(i1+5,i2+5,i3+7,c3)+cr6*ui(i1+6,i2+5,
     & i3+7,c3)+cr7*ui(i1+7,i2+5,i3+7,c3))+cs6*(cr0*ui(i1,i2+6,i3+7,
     & c3)+cr1*ui(i1+1,i2+6,i3+7,c3)+cr2*ui(i1+2,i2+6,i3+7,c3)+cr3*ui(
     & i1+3,i2+6,i3+7,c3)+cr4*ui(i1+4,i2+6,i3+7,c3)+cr5*ui(i1+5,i2+6,
     & i3+7,c3)+cr6*ui(i1+6,i2+6,i3+7,c3)+cr7*ui(i1+7,i2+6,i3+7,c3))+
     & cs7*(cr0*ui(i1,i2+7,i3+7,c3)+cr1*ui(i1+1,i2+7,i3+7,c3)+cr2*ui(
     & i1+2,i2+7,i3+7,c3)+cr3*ui(i1+3,i2+7,i3+7,c3)+cr4*ui(i1+4,i2+7,
     & i3+7,c3)+cr5*ui(i1+5,i2+7,i3+7,c3)+cr6*ui(i1+6,i2+7,i3+7,c3)+
     & cr7*ui(i1+7,i2+7,i3+7,c3)))


             end do
             end do
           end if
         else if( width(1).eq.9 .and. width(2).eq.9 .and. width(3)
     & .eq.9 )then
! loops3d($interpSparseStorage999(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)
           if( c3a.eq.c3b )then
             do c3=c3a,c3b
             do i=nia,nib
! interpSparseStorage999(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q90(c(i,0,0,0))
                 cs0 = q90(c(i,1,0,0))
                 ct0 = q90(c(i,2,0,0))
                 cr1 = q91(c(i,0,0,0))
                 cs1 = q91(c(i,1,0,0))
                 ct1 = q91(c(i,2,0,0))
                 cr2 = q92(c(i,0,0,0))
                 cs2 = q92(c(i,1,0,0))
                 ct2 = q92(c(i,2,0,0))
                 cr3 = q93(c(i,0,0,0))
                 cs3 = q93(c(i,1,0,0))
                 ct3 = q93(c(i,2,0,0))
                 cr4 = q94(c(i,0,0,0))
                 cs4 = q94(c(i,1,0,0))
                 ct4 = q94(c(i,2,0,0))
                 cr5 = q95(c(i,0,0,0))
                 cs5 = q95(c(i,1,0,0))
                 ct5 = q95(c(i,2,0,0))
                 cr6 = q96(c(i,0,0,0))
                 cs6 = q96(c(i,1,0,0))
                 ct6 = q96(c(i,2,0,0))
                 cr7 = q97(c(i,0,0,0))
                 cs7 = q97(c(i,1,0,0))
                 ct7 = q97(c(i,2,0,0))
                 cr8 = q98(c(i,0,0,0))
                 cs8 = q98(c(i,1,0,0))
                 ct8 = q98(c(i,2,0,0))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ct0*(cs0*(cr0*ui(i1,i2,
     & i3+0,c3)+cr1*ui(i1+1,i2,i3+0,c3)+cr2*ui(i1+2,i2,i3+0,c3)+cr3*
     & ui(i1+3,i2,i3+0,c3)+cr4*ui(i1+4,i2,i3+0,c3)+cr5*ui(i1+5,i2,i3+
     & 0,c3)+cr6*ui(i1+6,i2,i3+0,c3)+cr7*ui(i1+7,i2,i3+0,c3)+cr8*ui(
     & i1+8,i2,i3+0,c3))+cs1*(cr0*ui(i1,i2+1,i3+0,c3)+cr1*ui(i1+1,i2+
     & 1,i3+0,c3)+cr2*ui(i1+2,i2+1,i3+0,c3)+cr3*ui(i1+3,i2+1,i3+0,c3)+
     & cr4*ui(i1+4,i2+1,i3+0,c3)+cr5*ui(i1+5,i2+1,i3+0,c3)+cr6*ui(i1+
     & 6,i2+1,i3+0,c3)+cr7*ui(i1+7,i2+1,i3+0,c3)+cr8*ui(i1+8,i2+1,i3+
     & 0,c3))+cs2*(cr0*ui(i1,i2+2,i3+0,c3)+cr1*ui(i1+1,i2+2,i3+0,c3)+
     & cr2*ui(i1+2,i2+2,i3+0,c3)+cr3*ui(i1+3,i2+2,i3+0,c3)+cr4*ui(i1+
     & 4,i2+2,i3+0,c3)+cr5*ui(i1+5,i2+2,i3+0,c3)+cr6*ui(i1+6,i2+2,i3+
     & 0,c3)+cr7*ui(i1+7,i2+2,i3+0,c3)+cr8*ui(i1+8,i2+2,i3+0,c3))+cs3*
     & (cr0*ui(i1,i2+3,i3+0,c3)+cr1*ui(i1+1,i2+3,i3+0,c3)+cr2*ui(i1+2,
     & i2+3,i3+0,c3)+cr3*ui(i1+3,i2+3,i3+0,c3)+cr4*ui(i1+4,i2+3,i3+0,
     & c3)+cr5*ui(i1+5,i2+3,i3+0,c3)+cr6*ui(i1+6,i2+3,i3+0,c3)+cr7*ui(
     & i1+7,i2+3,i3+0,c3)+cr8*ui(i1+8,i2+3,i3+0,c3))+cs4*(cr0*ui(i1,
     & i2+4,i3+0,c3)+cr1*ui(i1+1,i2+4,i3+0,c3)+cr2*ui(i1+2,i2+4,i3+0,
     & c3)+cr3*ui(i1+3,i2+4,i3+0,c3)+cr4*ui(i1+4,i2+4,i3+0,c3)+cr5*ui(
     & i1+5,i2+4,i3+0,c3)+cr6*ui(i1+6,i2+4,i3+0,c3)+cr7*ui(i1+7,i2+4,
     & i3+0,c3)+cr8*ui(i1+8,i2+4,i3+0,c3))+cs5*(cr0*ui(i1,i2+5,i3+0,
     & c3)+cr1*ui(i1+1,i2+5,i3+0,c3)+cr2*ui(i1+2,i2+5,i3+0,c3)+cr3*ui(
     & i1+3,i2+5,i3+0,c3)+cr4*ui(i1+4,i2+5,i3+0,c3)+cr5*ui(i1+5,i2+5,
     & i3+0,c3)+cr6*ui(i1+6,i2+5,i3+0,c3)+cr7*ui(i1+7,i2+5,i3+0,c3)+
     & cr8*ui(i1+8,i2+5,i3+0,c3))+cs6*(cr0*ui(i1,i2+6,i3+0,c3)+cr1*ui(
     & i1+1,i2+6,i3+0,c3)+cr2*ui(i1+2,i2+6,i3+0,c3)+cr3*ui(i1+3,i2+6,
     & i3+0,c3)+cr4*ui(i1+4,i2+6,i3+0,c3)+cr5*ui(i1+5,i2+6,i3+0,c3)+
     & cr6*ui(i1+6,i2+6,i3+0,c3)+cr7*ui(i1+7,i2+6,i3+0,c3)+cr8*ui(i1+
     & 8,i2+6,i3+0,c3))+cs7*(cr0*ui(i1,i2+7,i3+0,c3)+cr1*ui(i1+1,i2+7,
     & i3+0,c3)+cr2*ui(i1+2,i2+7,i3+0,c3)+cr3*ui(i1+3,i2+7,i3+0,c3)+
     & cr4*ui(i1+4,i2+7,i3+0,c3)+cr5*ui(i1+5,i2+7,i3+0,c3)+cr6*ui(i1+
     & 6,i2+7,i3+0,c3)+cr7*ui(i1+7,i2+7,i3+0,c3)+cr8*ui(i1+8,i2+7,i3+
     & 0,c3))+cs8*(cr0*ui(i1,i2+8,i3+0,c3)+cr1*ui(i1+1,i2+8,i3+0,c3)+
     & cr2*ui(i1+2,i2+8,i3+0,c3)+cr3*ui(i1+3,i2+8,i3+0,c3)+cr4*ui(i1+
     & 4,i2+8,i3+0,c3)+cr5*ui(i1+5,i2+8,i3+0,c3)+cr6*ui(i1+6,i2+8,i3+
     & 0,c3)+cr7*ui(i1+7,i2+8,i3+0,c3)+cr8*ui(i1+8,i2+8,i3+0,c3)))+
     & ct1*(cs0*(cr0*ui(i1,i2,i3+1,c3)+cr1*ui(i1+1,i2,i3+1,c3)+cr2*ui(
     & i1+2,i2,i3+1,c3)+cr3*ui(i1+3,i2,i3+1,c3)+cr4*ui(i1+4,i2,i3+1,
     & c3)+cr5*ui(i1+5,i2,i3+1,c3)+cr6*ui(i1+6,i2,i3+1,c3)+cr7*ui(i1+
     & 7,i2,i3+1,c3)+cr8*ui(i1+8,i2,i3+1,c3))+cs1*(cr0*ui(i1,i2+1,i3+
     & 1,c3)+cr1*ui(i1+1,i2+1,i3+1,c3)+cr2*ui(i1+2,i2+1,i3+1,c3)+cr3*
     & ui(i1+3,i2+1,i3+1,c3)+cr4*ui(i1+4,i2+1,i3+1,c3)+cr5*ui(i1+5,i2+
     & 1,i3+1,c3)+cr6*ui(i1+6,i2+1,i3+1,c3)+cr7*ui(i1+7,i2+1,i3+1,c3)+
     & cr8*ui(i1+8,i2+1,i3+1,c3))+cs2*(cr0*ui(i1,i2+2,i3+1,c3)+cr1*ui(
     & i1+1,i2+2,i3+1,c3)+cr2*ui(i1+2,i2+2,i3+1,c3)+cr3*ui(i1+3,i2+2,
     & i3+1,c3)+cr4*ui(i1+4,i2+2,i3+1,c3)+cr5*ui(i1+5,i2+2,i3+1,c3)+
     & cr6*ui(i1+6,i2+2,i3+1,c3)+cr7*ui(i1+7,i2+2,i3+1,c3)+cr8*ui(i1+
     & 8,i2+2,i3+1,c3))+cs3*(cr0*ui(i1,i2+3,i3+1,c3)+cr1*ui(i1+1,i2+3,
     & i3+1,c3)+cr2*ui(i1+2,i2+3,i3+1,c3)+cr3*ui(i1+3,i2+3,i3+1,c3)+
     & cr4*ui(i1+4,i2+3,i3+1,c3)+cr5*ui(i1+5,i2+3,i3+1,c3)+cr6*ui(i1+
     & 6,i2+3,i3+1,c3)+cr7*ui(i1+7,i2+3,i3+1,c3)+cr8*ui(i1+8,i2+3,i3+
     & 1,c3))+cs4*(cr0*ui(i1,i2+4,i3+1,c3)+cr1*ui(i1+1,i2+4,i3+1,c3)+
     & cr2*ui(i1+2,i2+4,i3+1,c3)+cr3*ui(i1+3,i2+4,i3+1,c3)+cr4*ui(i1+
     & 4,i2+4,i3+1,c3)+cr5*ui(i1+5,i2+4,i3+1,c3)+cr6*ui(i1+6,i2+4,i3+
     & 1,c3)+cr7*ui(i1+7,i2+4,i3+1,c3)+cr8*ui(i1+8,i2+4,i3+1,c3))+cs5*
     & (cr0*ui(i1,i2+5,i3+1,c3)+cr1*ui(i1+1,i2+5,i3+1,c3)+cr2*ui(i1+2,
     & i2+5,i3+1,c3)+cr3*ui(i1+3,i2+5,i3+1,c3)+cr4*ui(i1+4,i2+5,i3+1,
     & c3)+cr5*ui(i1+5,i2+5,i3+1,c3)+cr6*ui(i1+6,i2+5,i3+1,c3)+cr7*ui(
     & i1+7,i2+5,i3+1,c3)+cr8*ui(i1+8,i2+5,i3+1,c3))+cs6*(cr0*ui(i1,
     & i2+6,i3+1,c3)+cr1*ui(i1+1,i2+6,i3+1,c3)+cr2*ui(i1+2,i2+6,i3+1,
     & c3)+cr3*ui(i1+3,i2+6,i3+1,c3)+cr4*ui(i1+4,i2+6,i3+1,c3)+cr5*ui(
     & i1+5,i2+6,i3+1,c3)+cr6*ui(i1+6,i2+6,i3+1,c3)+cr7*ui(i1+7,i2+6,
     & i3+1,c3)+cr8*ui(i1+8,i2+6,i3+1,c3))+cs7*(cr0*ui(i1,i2+7,i3+1,
     & c3)+cr1*ui(i1+1,i2+7,i3+1,c3)+cr2*ui(i1+2,i2+7,i3+1,c3)+cr3*ui(
     & i1+3,i2+7,i3+1,c3)+cr4*ui(i1+4,i2+7,i3+1,c3)+cr5*ui(i1+5,i2+7,
     & i3+1,c3)+cr6*ui(i1+6,i2+7,i3+1,c3)+cr7*ui(i1+7,i2+7,i3+1,c3)+
     & cr8*ui(i1+8,i2+7,i3+1,c3))+cs8*(cr0*ui(i1,i2+8,i3+1,c3)+cr1*ui(
     & i1+1,i2+8,i3+1,c3)+cr2*ui(i1+2,i2+8,i3+1,c3)+cr3*ui(i1+3,i2+8,
     & i3+1,c3)+cr4*ui(i1+4,i2+8,i3+1,c3)+cr5*ui(i1+5,i2+8,i3+1,c3)+
     & cr6*ui(i1+6,i2+8,i3+1,c3)+cr7*ui(i1+7,i2+8,i3+1,c3)+cr8*ui(i1+
     & 8,i2+8,i3+1,c3)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+ct2*(cs0*(cr0*ui(i1,i2,i3+2,c3)+cr1*ui(i1+1,i2,i3+2,
     & c3)+cr2*ui(i1+2,i2,i3+2,c3)+cr3*ui(i1+3,i2,i3+2,c3)+cr4*ui(i1+
     & 4,i2,i3+2,c3)+cr5*ui(i1+5,i2,i3+2,c3)+cr6*ui(i1+6,i2,i3+2,c3)+
     & cr7*ui(i1+7,i2,i3+2,c3)+cr8*ui(i1+8,i2,i3+2,c3))+cs1*(cr0*ui(
     & i1,i2+1,i3+2,c3)+cr1*ui(i1+1,i2+1,i3+2,c3)+cr2*ui(i1+2,i2+1,i3+
     & 2,c3)+cr3*ui(i1+3,i2+1,i3+2,c3)+cr4*ui(i1+4,i2+1,i3+2,c3)+cr5*
     & ui(i1+5,i2+1,i3+2,c3)+cr6*ui(i1+6,i2+1,i3+2,c3)+cr7*ui(i1+7,i2+
     & 1,i3+2,c3)+cr8*ui(i1+8,i2+1,i3+2,c3))+cs2*(cr0*ui(i1,i2+2,i3+2,
     & c3)+cr1*ui(i1+1,i2+2,i3+2,c3)+cr2*ui(i1+2,i2+2,i3+2,c3)+cr3*ui(
     & i1+3,i2+2,i3+2,c3)+cr4*ui(i1+4,i2+2,i3+2,c3)+cr5*ui(i1+5,i2+2,
     & i3+2,c3)+cr6*ui(i1+6,i2+2,i3+2,c3)+cr7*ui(i1+7,i2+2,i3+2,c3)+
     & cr8*ui(i1+8,i2+2,i3+2,c3))+cs3*(cr0*ui(i1,i2+3,i3+2,c3)+cr1*ui(
     & i1+1,i2+3,i3+2,c3)+cr2*ui(i1+2,i2+3,i3+2,c3)+cr3*ui(i1+3,i2+3,
     & i3+2,c3)+cr4*ui(i1+4,i2+3,i3+2,c3)+cr5*ui(i1+5,i2+3,i3+2,c3)+
     & cr6*ui(i1+6,i2+3,i3+2,c3)+cr7*ui(i1+7,i2+3,i3+2,c3)+cr8*ui(i1+
     & 8,i2+3,i3+2,c3))+cs4*(cr0*ui(i1,i2+4,i3+2,c3)+cr1*ui(i1+1,i2+4,
     & i3+2,c3)+cr2*ui(i1+2,i2+4,i3+2,c3)+cr3*ui(i1+3,i2+4,i3+2,c3)+
     & cr4*ui(i1+4,i2+4,i3+2,c3)+cr5*ui(i1+5,i2+4,i3+2,c3)+cr6*ui(i1+
     & 6,i2+4,i3+2,c3)+cr7*ui(i1+7,i2+4,i3+2,c3)+cr8*ui(i1+8,i2+4,i3+
     & 2,c3))+cs5*(cr0*ui(i1,i2+5,i3+2,c3)+cr1*ui(i1+1,i2+5,i3+2,c3)+
     & cr2*ui(i1+2,i2+5,i3+2,c3)+cr3*ui(i1+3,i2+5,i3+2,c3)+cr4*ui(i1+
     & 4,i2+5,i3+2,c3)+cr5*ui(i1+5,i2+5,i3+2,c3)+cr6*ui(i1+6,i2+5,i3+
     & 2,c3)+cr7*ui(i1+7,i2+5,i3+2,c3)+cr8*ui(i1+8,i2+5,i3+2,c3))+cs6*
     & (cr0*ui(i1,i2+6,i3+2,c3)+cr1*ui(i1+1,i2+6,i3+2,c3)+cr2*ui(i1+2,
     & i2+6,i3+2,c3)+cr3*ui(i1+3,i2+6,i3+2,c3)+cr4*ui(i1+4,i2+6,i3+2,
     & c3)+cr5*ui(i1+5,i2+6,i3+2,c3)+cr6*ui(i1+6,i2+6,i3+2,c3)+cr7*ui(
     & i1+7,i2+6,i3+2,c3)+cr8*ui(i1+8,i2+6,i3+2,c3))+cs7*(cr0*ui(i1,
     & i2+7,i3+2,c3)+cr1*ui(i1+1,i2+7,i3+2,c3)+cr2*ui(i1+2,i2+7,i3+2,
     & c3)+cr3*ui(i1+3,i2+7,i3+2,c3)+cr4*ui(i1+4,i2+7,i3+2,c3)+cr5*ui(
     & i1+5,i2+7,i3+2,c3)+cr6*ui(i1+6,i2+7,i3+2,c3)+cr7*ui(i1+7,i2+7,
     & i3+2,c3)+cr8*ui(i1+8,i2+7,i3+2,c3))+cs8*(cr0*ui(i1,i2+8,i3+2,
     & c3)+cr1*ui(i1+1,i2+8,i3+2,c3)+cr2*ui(i1+2,i2+8,i3+2,c3)+cr3*ui(
     & i1+3,i2+8,i3+2,c3)+cr4*ui(i1+4,i2+8,i3+2,c3)+cr5*ui(i1+5,i2+8,
     & i3+2,c3)+cr6*ui(i1+6,i2+8,i3+2,c3)+cr7*ui(i1+7,i2+8,i3+2,c3)+
     & cr8*ui(i1+8,i2+8,i3+2,c3)))+ct3*(cs0*(cr0*ui(i1,i2,i3+3,c3)+
     & cr1*ui(i1+1,i2,i3+3,c3)+cr2*ui(i1+2,i2,i3+3,c3)+cr3*ui(i1+3,i2,
     & i3+3,c3)+cr4*ui(i1+4,i2,i3+3,c3)+cr5*ui(i1+5,i2,i3+3,c3)+cr6*
     & ui(i1+6,i2,i3+3,c3)+cr7*ui(i1+7,i2,i3+3,c3)+cr8*ui(i1+8,i2,i3+
     & 3,c3))+cs1*(cr0*ui(i1,i2+1,i3+3,c3)+cr1*ui(i1+1,i2+1,i3+3,c3)+
     & cr2*ui(i1+2,i2+1,i3+3,c3)+cr3*ui(i1+3,i2+1,i3+3,c3)+cr4*ui(i1+
     & 4,i2+1,i3+3,c3)+cr5*ui(i1+5,i2+1,i3+3,c3)+cr6*ui(i1+6,i2+1,i3+
     & 3,c3)+cr7*ui(i1+7,i2+1,i3+3,c3)+cr8*ui(i1+8,i2+1,i3+3,c3))+cs2*
     & (cr0*ui(i1,i2+2,i3+3,c3)+cr1*ui(i1+1,i2+2,i3+3,c3)+cr2*ui(i1+2,
     & i2+2,i3+3,c3)+cr3*ui(i1+3,i2+2,i3+3,c3)+cr4*ui(i1+4,i2+2,i3+3,
     & c3)+cr5*ui(i1+5,i2+2,i3+3,c3)+cr6*ui(i1+6,i2+2,i3+3,c3)+cr7*ui(
     & i1+7,i2+2,i3+3,c3)+cr8*ui(i1+8,i2+2,i3+3,c3))+cs3*(cr0*ui(i1,
     & i2+3,i3+3,c3)+cr1*ui(i1+1,i2+3,i3+3,c3)+cr2*ui(i1+2,i2+3,i3+3,
     & c3)+cr3*ui(i1+3,i2+3,i3+3,c3)+cr4*ui(i1+4,i2+3,i3+3,c3)+cr5*ui(
     & i1+5,i2+3,i3+3,c3)+cr6*ui(i1+6,i2+3,i3+3,c3)+cr7*ui(i1+7,i2+3,
     & i3+3,c3)+cr8*ui(i1+8,i2+3,i3+3,c3))+cs4*(cr0*ui(i1,i2+4,i3+3,
     & c3)+cr1*ui(i1+1,i2+4,i3+3,c3)+cr2*ui(i1+2,i2+4,i3+3,c3)+cr3*ui(
     & i1+3,i2+4,i3+3,c3)+cr4*ui(i1+4,i2+4,i3+3,c3)+cr5*ui(i1+5,i2+4,
     & i3+3,c3)+cr6*ui(i1+6,i2+4,i3+3,c3)+cr7*ui(i1+7,i2+4,i3+3,c3)+
     & cr8*ui(i1+8,i2+4,i3+3,c3))+cs5*(cr0*ui(i1,i2+5,i3+3,c3)+cr1*ui(
     & i1+1,i2+5,i3+3,c3)+cr2*ui(i1+2,i2+5,i3+3,c3)+cr3*ui(i1+3,i2+5,
     & i3+3,c3)+cr4*ui(i1+4,i2+5,i3+3,c3)+cr5*ui(i1+5,i2+5,i3+3,c3)+
     & cr6*ui(i1+6,i2+5,i3+3,c3)+cr7*ui(i1+7,i2+5,i3+3,c3)+cr8*ui(i1+
     & 8,i2+5,i3+3,c3))+cs6*(cr0*ui(i1,i2+6,i3+3,c3)+cr1*ui(i1+1,i2+6,
     & i3+3,c3)+cr2*ui(i1+2,i2+6,i3+3,c3)+cr3*ui(i1+3,i2+6,i3+3,c3)+
     & cr4*ui(i1+4,i2+6,i3+3,c3)+cr5*ui(i1+5,i2+6,i3+3,c3)+cr6*ui(i1+
     & 6,i2+6,i3+3,c3)+cr7*ui(i1+7,i2+6,i3+3,c3)+cr8*ui(i1+8,i2+6,i3+
     & 3,c3))+cs7*(cr0*ui(i1,i2+7,i3+3,c3)+cr1*ui(i1+1,i2+7,i3+3,c3)+
     & cr2*ui(i1+2,i2+7,i3+3,c3)+cr3*ui(i1+3,i2+7,i3+3,c3)+cr4*ui(i1+
     & 4,i2+7,i3+3,c3)+cr5*ui(i1+5,i2+7,i3+3,c3)+cr6*ui(i1+6,i2+7,i3+
     & 3,c3)+cr7*ui(i1+7,i2+7,i3+3,c3)+cr8*ui(i1+8,i2+7,i3+3,c3))+cs8*
     & (cr0*ui(i1,i2+8,i3+3,c3)+cr1*ui(i1+1,i2+8,i3+3,c3)+cr2*ui(i1+2,
     & i2+8,i3+3,c3)+cr3*ui(i1+3,i2+8,i3+3,c3)+cr4*ui(i1+4,i2+8,i3+3,
     & c3)+cr5*ui(i1+5,i2+8,i3+3,c3)+cr6*ui(i1+6,i2+8,i3+3,c3)+cr7*ui(
     & i1+7,i2+8,i3+3,c3)+cr8*ui(i1+8,i2+8,i3+3,c3)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+ct4*(cs0*(cr0*ui(i1,i2,i3+4,c3)+cr1*ui(i1+1,i2,i3+4,
     & c3)+cr2*ui(i1+2,i2,i3+4,c3)+cr3*ui(i1+3,i2,i3+4,c3)+cr4*ui(i1+
     & 4,i2,i3+4,c3)+cr5*ui(i1+5,i2,i3+4,c3)+cr6*ui(i1+6,i2,i3+4,c3)+
     & cr7*ui(i1+7,i2,i3+4,c3)+cr8*ui(i1+8,i2,i3+4,c3))+cs1*(cr0*ui(
     & i1,i2+1,i3+4,c3)+cr1*ui(i1+1,i2+1,i3+4,c3)+cr2*ui(i1+2,i2+1,i3+
     & 4,c3)+cr3*ui(i1+3,i2+1,i3+4,c3)+cr4*ui(i1+4,i2+1,i3+4,c3)+cr5*
     & ui(i1+5,i2+1,i3+4,c3)+cr6*ui(i1+6,i2+1,i3+4,c3)+cr7*ui(i1+7,i2+
     & 1,i3+4,c3)+cr8*ui(i1+8,i2+1,i3+4,c3))+cs2*(cr0*ui(i1,i2+2,i3+4,
     & c3)+cr1*ui(i1+1,i2+2,i3+4,c3)+cr2*ui(i1+2,i2+2,i3+4,c3)+cr3*ui(
     & i1+3,i2+2,i3+4,c3)+cr4*ui(i1+4,i2+2,i3+4,c3)+cr5*ui(i1+5,i2+2,
     & i3+4,c3)+cr6*ui(i1+6,i2+2,i3+4,c3)+cr7*ui(i1+7,i2+2,i3+4,c3)+
     & cr8*ui(i1+8,i2+2,i3+4,c3))+cs3*(cr0*ui(i1,i2+3,i3+4,c3)+cr1*ui(
     & i1+1,i2+3,i3+4,c3)+cr2*ui(i1+2,i2+3,i3+4,c3)+cr3*ui(i1+3,i2+3,
     & i3+4,c3)+cr4*ui(i1+4,i2+3,i3+4,c3)+cr5*ui(i1+5,i2+3,i3+4,c3)+
     & cr6*ui(i1+6,i2+3,i3+4,c3)+cr7*ui(i1+7,i2+3,i3+4,c3)+cr8*ui(i1+
     & 8,i2+3,i3+4,c3))+cs4*(cr0*ui(i1,i2+4,i3+4,c3)+cr1*ui(i1+1,i2+4,
     & i3+4,c3)+cr2*ui(i1+2,i2+4,i3+4,c3)+cr3*ui(i1+3,i2+4,i3+4,c3)+
     & cr4*ui(i1+4,i2+4,i3+4,c3)+cr5*ui(i1+5,i2+4,i3+4,c3)+cr6*ui(i1+
     & 6,i2+4,i3+4,c3)+cr7*ui(i1+7,i2+4,i3+4,c3)+cr8*ui(i1+8,i2+4,i3+
     & 4,c3))+cs5*(cr0*ui(i1,i2+5,i3+4,c3)+cr1*ui(i1+1,i2+5,i3+4,c3)+
     & cr2*ui(i1+2,i2+5,i3+4,c3)+cr3*ui(i1+3,i2+5,i3+4,c3)+cr4*ui(i1+
     & 4,i2+5,i3+4,c3)+cr5*ui(i1+5,i2+5,i3+4,c3)+cr6*ui(i1+6,i2+5,i3+
     & 4,c3)+cr7*ui(i1+7,i2+5,i3+4,c3)+cr8*ui(i1+8,i2+5,i3+4,c3))+cs6*
     & (cr0*ui(i1,i2+6,i3+4,c3)+cr1*ui(i1+1,i2+6,i3+4,c3)+cr2*ui(i1+2,
     & i2+6,i3+4,c3)+cr3*ui(i1+3,i2+6,i3+4,c3)+cr4*ui(i1+4,i2+6,i3+4,
     & c3)+cr5*ui(i1+5,i2+6,i3+4,c3)+cr6*ui(i1+6,i2+6,i3+4,c3)+cr7*ui(
     & i1+7,i2+6,i3+4,c3)+cr8*ui(i1+8,i2+6,i3+4,c3))+cs7*(cr0*ui(i1,
     & i2+7,i3+4,c3)+cr1*ui(i1+1,i2+7,i3+4,c3)+cr2*ui(i1+2,i2+7,i3+4,
     & c3)+cr3*ui(i1+3,i2+7,i3+4,c3)+cr4*ui(i1+4,i2+7,i3+4,c3)+cr5*ui(
     & i1+5,i2+7,i3+4,c3)+cr6*ui(i1+6,i2+7,i3+4,c3)+cr7*ui(i1+7,i2+7,
     & i3+4,c3)+cr8*ui(i1+8,i2+7,i3+4,c3))+cs8*(cr0*ui(i1,i2+8,i3+4,
     & c3)+cr1*ui(i1+1,i2+8,i3+4,c3)+cr2*ui(i1+2,i2+8,i3+4,c3)+cr3*ui(
     & i1+3,i2+8,i3+4,c3)+cr4*ui(i1+4,i2+8,i3+4,c3)+cr5*ui(i1+5,i2+8,
     & i3+4,c3)+cr6*ui(i1+6,i2+8,i3+4,c3)+cr7*ui(i1+7,i2+8,i3+4,c3)+
     & cr8*ui(i1+8,i2+8,i3+4,c3)))+ct5*(cs0*(cr0*ui(i1,i2,i3+5,c3)+
     & cr1*ui(i1+1,i2,i3+5,c3)+cr2*ui(i1+2,i2,i3+5,c3)+cr3*ui(i1+3,i2,
     & i3+5,c3)+cr4*ui(i1+4,i2,i3+5,c3)+cr5*ui(i1+5,i2,i3+5,c3)+cr6*
     & ui(i1+6,i2,i3+5,c3)+cr7*ui(i1+7,i2,i3+5,c3)+cr8*ui(i1+8,i2,i3+
     & 5,c3))+cs1*(cr0*ui(i1,i2+1,i3+5,c3)+cr1*ui(i1+1,i2+1,i3+5,c3)+
     & cr2*ui(i1+2,i2+1,i3+5,c3)+cr3*ui(i1+3,i2+1,i3+5,c3)+cr4*ui(i1+
     & 4,i2+1,i3+5,c3)+cr5*ui(i1+5,i2+1,i3+5,c3)+cr6*ui(i1+6,i2+1,i3+
     & 5,c3)+cr7*ui(i1+7,i2+1,i3+5,c3)+cr8*ui(i1+8,i2+1,i3+5,c3))+cs2*
     & (cr0*ui(i1,i2+2,i3+5,c3)+cr1*ui(i1+1,i2+2,i3+5,c3)+cr2*ui(i1+2,
     & i2+2,i3+5,c3)+cr3*ui(i1+3,i2+2,i3+5,c3)+cr4*ui(i1+4,i2+2,i3+5,
     & c3)+cr5*ui(i1+5,i2+2,i3+5,c3)+cr6*ui(i1+6,i2+2,i3+5,c3)+cr7*ui(
     & i1+7,i2+2,i3+5,c3)+cr8*ui(i1+8,i2+2,i3+5,c3))+cs3*(cr0*ui(i1,
     & i2+3,i3+5,c3)+cr1*ui(i1+1,i2+3,i3+5,c3)+cr2*ui(i1+2,i2+3,i3+5,
     & c3)+cr3*ui(i1+3,i2+3,i3+5,c3)+cr4*ui(i1+4,i2+3,i3+5,c3)+cr5*ui(
     & i1+5,i2+3,i3+5,c3)+cr6*ui(i1+6,i2+3,i3+5,c3)+cr7*ui(i1+7,i2+3,
     & i3+5,c3)+cr8*ui(i1+8,i2+3,i3+5,c3))+cs4*(cr0*ui(i1,i2+4,i3+5,
     & c3)+cr1*ui(i1+1,i2+4,i3+5,c3)+cr2*ui(i1+2,i2+4,i3+5,c3)+cr3*ui(
     & i1+3,i2+4,i3+5,c3)+cr4*ui(i1+4,i2+4,i3+5,c3)+cr5*ui(i1+5,i2+4,
     & i3+5,c3)+cr6*ui(i1+6,i2+4,i3+5,c3)+cr7*ui(i1+7,i2+4,i3+5,c3)+
     & cr8*ui(i1+8,i2+4,i3+5,c3))+cs5*(cr0*ui(i1,i2+5,i3+5,c3)+cr1*ui(
     & i1+1,i2+5,i3+5,c3)+cr2*ui(i1+2,i2+5,i3+5,c3)+cr3*ui(i1+3,i2+5,
     & i3+5,c3)+cr4*ui(i1+4,i2+5,i3+5,c3)+cr5*ui(i1+5,i2+5,i3+5,c3)+
     & cr6*ui(i1+6,i2+5,i3+5,c3)+cr7*ui(i1+7,i2+5,i3+5,c3)+cr8*ui(i1+
     & 8,i2+5,i3+5,c3))+cs6*(cr0*ui(i1,i2+6,i3+5,c3)+cr1*ui(i1+1,i2+6,
     & i3+5,c3)+cr2*ui(i1+2,i2+6,i3+5,c3)+cr3*ui(i1+3,i2+6,i3+5,c3)+
     & cr4*ui(i1+4,i2+6,i3+5,c3)+cr5*ui(i1+5,i2+6,i3+5,c3)+cr6*ui(i1+
     & 6,i2+6,i3+5,c3)+cr7*ui(i1+7,i2+6,i3+5,c3)+cr8*ui(i1+8,i2+6,i3+
     & 5,c3))+cs7*(cr0*ui(i1,i2+7,i3+5,c3)+cr1*ui(i1+1,i2+7,i3+5,c3)+
     & cr2*ui(i1+2,i2+7,i3+5,c3)+cr3*ui(i1+3,i2+7,i3+5,c3)+cr4*ui(i1+
     & 4,i2+7,i3+5,c3)+cr5*ui(i1+5,i2+7,i3+5,c3)+cr6*ui(i1+6,i2+7,i3+
     & 5,c3)+cr7*ui(i1+7,i2+7,i3+5,c3)+cr8*ui(i1+8,i2+7,i3+5,c3))+cs8*
     & (cr0*ui(i1,i2+8,i3+5,c3)+cr1*ui(i1+1,i2+8,i3+5,c3)+cr2*ui(i1+2,
     & i2+8,i3+5,c3)+cr3*ui(i1+3,i2+8,i3+5,c3)+cr4*ui(i1+4,i2+8,i3+5,
     & c3)+cr5*ui(i1+5,i2+8,i3+5,c3)+cr6*ui(i1+6,i2+8,i3+5,c3)+cr7*ui(
     & i1+7,i2+8,i3+5,c3)+cr8*ui(i1+8,i2+8,i3+5,c3)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+ct6*(cs0*(cr0*ui(i1,i2,i3+6,c3)+cr1*ui(i1+1,i2,i3+6,
     & c3)+cr2*ui(i1+2,i2,i3+6,c3)+cr3*ui(i1+3,i2,i3+6,c3)+cr4*ui(i1+
     & 4,i2,i3+6,c3)+cr5*ui(i1+5,i2,i3+6,c3)+cr6*ui(i1+6,i2,i3+6,c3)+
     & cr7*ui(i1+7,i2,i3+6,c3)+cr8*ui(i1+8,i2,i3+6,c3))+cs1*(cr0*ui(
     & i1,i2+1,i3+6,c3)+cr1*ui(i1+1,i2+1,i3+6,c3)+cr2*ui(i1+2,i2+1,i3+
     & 6,c3)+cr3*ui(i1+3,i2+1,i3+6,c3)+cr4*ui(i1+4,i2+1,i3+6,c3)+cr5*
     & ui(i1+5,i2+1,i3+6,c3)+cr6*ui(i1+6,i2+1,i3+6,c3)+cr7*ui(i1+7,i2+
     & 1,i3+6,c3)+cr8*ui(i1+8,i2+1,i3+6,c3))+cs2*(cr0*ui(i1,i2+2,i3+6,
     & c3)+cr1*ui(i1+1,i2+2,i3+6,c3)+cr2*ui(i1+2,i2+2,i3+6,c3)+cr3*ui(
     & i1+3,i2+2,i3+6,c3)+cr4*ui(i1+4,i2+2,i3+6,c3)+cr5*ui(i1+5,i2+2,
     & i3+6,c3)+cr6*ui(i1+6,i2+2,i3+6,c3)+cr7*ui(i1+7,i2+2,i3+6,c3)+
     & cr8*ui(i1+8,i2+2,i3+6,c3))+cs3*(cr0*ui(i1,i2+3,i3+6,c3)+cr1*ui(
     & i1+1,i2+3,i3+6,c3)+cr2*ui(i1+2,i2+3,i3+6,c3)+cr3*ui(i1+3,i2+3,
     & i3+6,c3)+cr4*ui(i1+4,i2+3,i3+6,c3)+cr5*ui(i1+5,i2+3,i3+6,c3)+
     & cr6*ui(i1+6,i2+3,i3+6,c3)+cr7*ui(i1+7,i2+3,i3+6,c3)+cr8*ui(i1+
     & 8,i2+3,i3+6,c3))+cs4*(cr0*ui(i1,i2+4,i3+6,c3)+cr1*ui(i1+1,i2+4,
     & i3+6,c3)+cr2*ui(i1+2,i2+4,i3+6,c3)+cr3*ui(i1+3,i2+4,i3+6,c3)+
     & cr4*ui(i1+4,i2+4,i3+6,c3)+cr5*ui(i1+5,i2+4,i3+6,c3)+cr6*ui(i1+
     & 6,i2+4,i3+6,c3)+cr7*ui(i1+7,i2+4,i3+6,c3)+cr8*ui(i1+8,i2+4,i3+
     & 6,c3))+cs5*(cr0*ui(i1,i2+5,i3+6,c3)+cr1*ui(i1+1,i2+5,i3+6,c3)+
     & cr2*ui(i1+2,i2+5,i3+6,c3)+cr3*ui(i1+3,i2+5,i3+6,c3)+cr4*ui(i1+
     & 4,i2+5,i3+6,c3)+cr5*ui(i1+5,i2+5,i3+6,c3)+cr6*ui(i1+6,i2+5,i3+
     & 6,c3)+cr7*ui(i1+7,i2+5,i3+6,c3)+cr8*ui(i1+8,i2+5,i3+6,c3))+cs6*
     & (cr0*ui(i1,i2+6,i3+6,c3)+cr1*ui(i1+1,i2+6,i3+6,c3)+cr2*ui(i1+2,
     & i2+6,i3+6,c3)+cr3*ui(i1+3,i2+6,i3+6,c3)+cr4*ui(i1+4,i2+6,i3+6,
     & c3)+cr5*ui(i1+5,i2+6,i3+6,c3)+cr6*ui(i1+6,i2+6,i3+6,c3)+cr7*ui(
     & i1+7,i2+6,i3+6,c3)+cr8*ui(i1+8,i2+6,i3+6,c3))+cs7*(cr0*ui(i1,
     & i2+7,i3+6,c3)+cr1*ui(i1+1,i2+7,i3+6,c3)+cr2*ui(i1+2,i2+7,i3+6,
     & c3)+cr3*ui(i1+3,i2+7,i3+6,c3)+cr4*ui(i1+4,i2+7,i3+6,c3)+cr5*ui(
     & i1+5,i2+7,i3+6,c3)+cr6*ui(i1+6,i2+7,i3+6,c3)+cr7*ui(i1+7,i2+7,
     & i3+6,c3)+cr8*ui(i1+8,i2+7,i3+6,c3))+cs8*(cr0*ui(i1,i2+8,i3+6,
     & c3)+cr1*ui(i1+1,i2+8,i3+6,c3)+cr2*ui(i1+2,i2+8,i3+6,c3)+cr3*ui(
     & i1+3,i2+8,i3+6,c3)+cr4*ui(i1+4,i2+8,i3+6,c3)+cr5*ui(i1+5,i2+8,
     & i3+6,c3)+cr6*ui(i1+6,i2+8,i3+6,c3)+cr7*ui(i1+7,i2+8,i3+6,c3)+
     & cr8*ui(i1+8,i2+8,i3+6,c3)))+ct7*(cs0*(cr0*ui(i1,i2,i3+7,c3)+
     & cr1*ui(i1+1,i2,i3+7,c3)+cr2*ui(i1+2,i2,i3+7,c3)+cr3*ui(i1+3,i2,
     & i3+7,c3)+cr4*ui(i1+4,i2,i3+7,c3)+cr5*ui(i1+5,i2,i3+7,c3)+cr6*
     & ui(i1+6,i2,i3+7,c3)+cr7*ui(i1+7,i2,i3+7,c3)+cr8*ui(i1+8,i2,i3+
     & 7,c3))+cs1*(cr0*ui(i1,i2+1,i3+7,c3)+cr1*ui(i1+1,i2+1,i3+7,c3)+
     & cr2*ui(i1+2,i2+1,i3+7,c3)+cr3*ui(i1+3,i2+1,i3+7,c3)+cr4*ui(i1+
     & 4,i2+1,i3+7,c3)+cr5*ui(i1+5,i2+1,i3+7,c3)+cr6*ui(i1+6,i2+1,i3+
     & 7,c3)+cr7*ui(i1+7,i2+1,i3+7,c3)+cr8*ui(i1+8,i2+1,i3+7,c3))+cs2*
     & (cr0*ui(i1,i2+2,i3+7,c3)+cr1*ui(i1+1,i2+2,i3+7,c3)+cr2*ui(i1+2,
     & i2+2,i3+7,c3)+cr3*ui(i1+3,i2+2,i3+7,c3)+cr4*ui(i1+4,i2+2,i3+7,
     & c3)+cr5*ui(i1+5,i2+2,i3+7,c3)+cr6*ui(i1+6,i2+2,i3+7,c3)+cr7*ui(
     & i1+7,i2+2,i3+7,c3)+cr8*ui(i1+8,i2+2,i3+7,c3))+cs3*(cr0*ui(i1,
     & i2+3,i3+7,c3)+cr1*ui(i1+1,i2+3,i3+7,c3)+cr2*ui(i1+2,i2+3,i3+7,
     & c3)+cr3*ui(i1+3,i2+3,i3+7,c3)+cr4*ui(i1+4,i2+3,i3+7,c3)+cr5*ui(
     & i1+5,i2+3,i3+7,c3)+cr6*ui(i1+6,i2+3,i3+7,c3)+cr7*ui(i1+7,i2+3,
     & i3+7,c3)+cr8*ui(i1+8,i2+3,i3+7,c3))+cs4*(cr0*ui(i1,i2+4,i3+7,
     & c3)+cr1*ui(i1+1,i2+4,i3+7,c3)+cr2*ui(i1+2,i2+4,i3+7,c3)+cr3*ui(
     & i1+3,i2+4,i3+7,c3)+cr4*ui(i1+4,i2+4,i3+7,c3)+cr5*ui(i1+5,i2+4,
     & i3+7,c3)+cr6*ui(i1+6,i2+4,i3+7,c3)+cr7*ui(i1+7,i2+4,i3+7,c3)+
     & cr8*ui(i1+8,i2+4,i3+7,c3))+cs5*(cr0*ui(i1,i2+5,i3+7,c3)+cr1*ui(
     & i1+1,i2+5,i3+7,c3)+cr2*ui(i1+2,i2+5,i3+7,c3)+cr3*ui(i1+3,i2+5,
     & i3+7,c3)+cr4*ui(i1+4,i2+5,i3+7,c3)+cr5*ui(i1+5,i2+5,i3+7,c3)+
     & cr6*ui(i1+6,i2+5,i3+7,c3)+cr7*ui(i1+7,i2+5,i3+7,c3)+cr8*ui(i1+
     & 8,i2+5,i3+7,c3))+cs6*(cr0*ui(i1,i2+6,i3+7,c3)+cr1*ui(i1+1,i2+6,
     & i3+7,c3)+cr2*ui(i1+2,i2+6,i3+7,c3)+cr3*ui(i1+3,i2+6,i3+7,c3)+
     & cr4*ui(i1+4,i2+6,i3+7,c3)+cr5*ui(i1+5,i2+6,i3+7,c3)+cr6*ui(i1+
     & 6,i2+6,i3+7,c3)+cr7*ui(i1+7,i2+6,i3+7,c3)+cr8*ui(i1+8,i2+6,i3+
     & 7,c3))+cs7*(cr0*ui(i1,i2+7,i3+7,c3)+cr1*ui(i1+1,i2+7,i3+7,c3)+
     & cr2*ui(i1+2,i2+7,i3+7,c3)+cr3*ui(i1+3,i2+7,i3+7,c3)+cr4*ui(i1+
     & 4,i2+7,i3+7,c3)+cr5*ui(i1+5,i2+7,i3+7,c3)+cr6*ui(i1+6,i2+7,i3+
     & 7,c3)+cr7*ui(i1+7,i2+7,i3+7,c3)+cr8*ui(i1+8,i2+7,i3+7,c3))+cs8*
     & (cr0*ui(i1,i2+8,i3+7,c3)+cr1*ui(i1+1,i2+8,i3+7,c3)+cr2*ui(i1+2,
     & i2+8,i3+7,c3)+cr3*ui(i1+3,i2+8,i3+7,c3)+cr4*ui(i1+4,i2+8,i3+7,
     & c3)+cr5*ui(i1+5,i2+8,i3+7,c3)+cr6*ui(i1+6,i2+8,i3+7,c3)+cr7*ui(
     & i1+7,i2+8,i3+7,c3)+cr8*ui(i1+8,i2+8,i3+7,c3)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+ct8*(cs0*(cr0*ui(i1,i2,i3+8,c3)+cr1*ui(i1+1,i2,i3+8,
     & c3)+cr2*ui(i1+2,i2,i3+8,c3)+cr3*ui(i1+3,i2,i3+8,c3)+cr4*ui(i1+
     & 4,i2,i3+8,c3)+cr5*ui(i1+5,i2,i3+8,c3)+cr6*ui(i1+6,i2,i3+8,c3)+
     & cr7*ui(i1+7,i2,i3+8,c3)+cr8*ui(i1+8,i2,i3+8,c3))+cs1*(cr0*ui(
     & i1,i2+1,i3+8,c3)+cr1*ui(i1+1,i2+1,i3+8,c3)+cr2*ui(i1+2,i2+1,i3+
     & 8,c3)+cr3*ui(i1+3,i2+1,i3+8,c3)+cr4*ui(i1+4,i2+1,i3+8,c3)+cr5*
     & ui(i1+5,i2+1,i3+8,c3)+cr6*ui(i1+6,i2+1,i3+8,c3)+cr7*ui(i1+7,i2+
     & 1,i3+8,c3)+cr8*ui(i1+8,i2+1,i3+8,c3))+cs2*(cr0*ui(i1,i2+2,i3+8,
     & c3)+cr1*ui(i1+1,i2+2,i3+8,c3)+cr2*ui(i1+2,i2+2,i3+8,c3)+cr3*ui(
     & i1+3,i2+2,i3+8,c3)+cr4*ui(i1+4,i2+2,i3+8,c3)+cr5*ui(i1+5,i2+2,
     & i3+8,c3)+cr6*ui(i1+6,i2+2,i3+8,c3)+cr7*ui(i1+7,i2+2,i3+8,c3)+
     & cr8*ui(i1+8,i2+2,i3+8,c3))+cs3*(cr0*ui(i1,i2+3,i3+8,c3)+cr1*ui(
     & i1+1,i2+3,i3+8,c3)+cr2*ui(i1+2,i2+3,i3+8,c3)+cr3*ui(i1+3,i2+3,
     & i3+8,c3)+cr4*ui(i1+4,i2+3,i3+8,c3)+cr5*ui(i1+5,i2+3,i3+8,c3)+
     & cr6*ui(i1+6,i2+3,i3+8,c3)+cr7*ui(i1+7,i2+3,i3+8,c3)+cr8*ui(i1+
     & 8,i2+3,i3+8,c3))+cs4*(cr0*ui(i1,i2+4,i3+8,c3)+cr1*ui(i1+1,i2+4,
     & i3+8,c3)+cr2*ui(i1+2,i2+4,i3+8,c3)+cr3*ui(i1+3,i2+4,i3+8,c3)+
     & cr4*ui(i1+4,i2+4,i3+8,c3)+cr5*ui(i1+5,i2+4,i3+8,c3)+cr6*ui(i1+
     & 6,i2+4,i3+8,c3)+cr7*ui(i1+7,i2+4,i3+8,c3)+cr8*ui(i1+8,i2+4,i3+
     & 8,c3))+cs5*(cr0*ui(i1,i2+5,i3+8,c3)+cr1*ui(i1+1,i2+5,i3+8,c3)+
     & cr2*ui(i1+2,i2+5,i3+8,c3)+cr3*ui(i1+3,i2+5,i3+8,c3)+cr4*ui(i1+
     & 4,i2+5,i3+8,c3)+cr5*ui(i1+5,i2+5,i3+8,c3)+cr6*ui(i1+6,i2+5,i3+
     & 8,c3)+cr7*ui(i1+7,i2+5,i3+8,c3)+cr8*ui(i1+8,i2+5,i3+8,c3))+cs6*
     & (cr0*ui(i1,i2+6,i3+8,c3)+cr1*ui(i1+1,i2+6,i3+8,c3)+cr2*ui(i1+2,
     & i2+6,i3+8,c3)+cr3*ui(i1+3,i2+6,i3+8,c3)+cr4*ui(i1+4,i2+6,i3+8,
     & c3)+cr5*ui(i1+5,i2+6,i3+8,c3)+cr6*ui(i1+6,i2+6,i3+8,c3)+cr7*ui(
     & i1+7,i2+6,i3+8,c3)+cr8*ui(i1+8,i2+6,i3+8,c3))+cs7*(cr0*ui(i1,
     & i2+7,i3+8,c3)+cr1*ui(i1+1,i2+7,i3+8,c3)+cr2*ui(i1+2,i2+7,i3+8,
     & c3)+cr3*ui(i1+3,i2+7,i3+8,c3)+cr4*ui(i1+4,i2+7,i3+8,c3)+cr5*ui(
     & i1+5,i2+7,i3+8,c3)+cr6*ui(i1+6,i2+7,i3+8,c3)+cr7*ui(i1+7,i2+7,
     & i3+8,c3)+cr8*ui(i1+8,i2+7,i3+8,c3))+cs8*(cr0*ui(i1,i2+8,i3+8,
     & c3)+cr1*ui(i1+1,i2+8,i3+8,c3)+cr2*ui(i1+2,i2+8,i3+8,c3)+cr3*ui(
     & i1+3,i2+8,i3+8,c3)+cr4*ui(i1+4,i2+8,i3+8,c3)+cr5*ui(i1+5,i2+8,
     & i3+8,c3)+cr6*ui(i1+6,i2+8,i3+8,c3)+cr7*ui(i1+7,i2+8,i3+8,c3)+
     & cr8*ui(i1+8,i2+8,i3+8,c3)))


             end do
             end do
           else
             ! put "c" loop as inner loop, this seems to be faster
             do i=nia,nib
             do c3=c3a,c3b
! interpSparseStorage999(ug(ip(i,1),ip(i,2),ip(i,3),c3))
               i1=il(i,1)
               i2=il(i,2)
               i3=il(i,3)
                 cr0 = q90(c(i,0,0,0))
                 cs0 = q90(c(i,1,0,0))
                 ct0 = q90(c(i,2,0,0))
                 cr1 = q91(c(i,0,0,0))
                 cs1 = q91(c(i,1,0,0))
                 ct1 = q91(c(i,2,0,0))
                 cr2 = q92(c(i,0,0,0))
                 cs2 = q92(c(i,1,0,0))
                 ct2 = q92(c(i,2,0,0))
                 cr3 = q93(c(i,0,0,0))
                 cs3 = q93(c(i,1,0,0))
                 ct3 = q93(c(i,2,0,0))
                 cr4 = q94(c(i,0,0,0))
                 cs4 = q94(c(i,1,0,0))
                 ct4 = q94(c(i,2,0,0))
                 cr5 = q95(c(i,0,0,0))
                 cs5 = q95(c(i,1,0,0))
                 ct5 = q95(c(i,2,0,0))
                 cr6 = q96(c(i,0,0,0))
                 cs6 = q96(c(i,1,0,0))
                 ct6 = q96(c(i,2,0,0))
                 cr7 = q97(c(i,0,0,0))
                 cs7 = q97(c(i,1,0,0))
                 ct7 = q97(c(i,2,0,0))
                 cr8 = q98(c(i,0,0,0))
                 cs8 = q98(c(i,1,0,0))
                 ct8 = q98(c(i,2,0,0))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ct0*(cs0*(cr0*ui(i1,i2,
     & i3+0,c3)+cr1*ui(i1+1,i2,i3+0,c3)+cr2*ui(i1+2,i2,i3+0,c3)+cr3*
     & ui(i1+3,i2,i3+0,c3)+cr4*ui(i1+4,i2,i3+0,c3)+cr5*ui(i1+5,i2,i3+
     & 0,c3)+cr6*ui(i1+6,i2,i3+0,c3)+cr7*ui(i1+7,i2,i3+0,c3)+cr8*ui(
     & i1+8,i2,i3+0,c3))+cs1*(cr0*ui(i1,i2+1,i3+0,c3)+cr1*ui(i1+1,i2+
     & 1,i3+0,c3)+cr2*ui(i1+2,i2+1,i3+0,c3)+cr3*ui(i1+3,i2+1,i3+0,c3)+
     & cr4*ui(i1+4,i2+1,i3+0,c3)+cr5*ui(i1+5,i2+1,i3+0,c3)+cr6*ui(i1+
     & 6,i2+1,i3+0,c3)+cr7*ui(i1+7,i2+1,i3+0,c3)+cr8*ui(i1+8,i2+1,i3+
     & 0,c3))+cs2*(cr0*ui(i1,i2+2,i3+0,c3)+cr1*ui(i1+1,i2+2,i3+0,c3)+
     & cr2*ui(i1+2,i2+2,i3+0,c3)+cr3*ui(i1+3,i2+2,i3+0,c3)+cr4*ui(i1+
     & 4,i2+2,i3+0,c3)+cr5*ui(i1+5,i2+2,i3+0,c3)+cr6*ui(i1+6,i2+2,i3+
     & 0,c3)+cr7*ui(i1+7,i2+2,i3+0,c3)+cr8*ui(i1+8,i2+2,i3+0,c3))+cs3*
     & (cr0*ui(i1,i2+3,i3+0,c3)+cr1*ui(i1+1,i2+3,i3+0,c3)+cr2*ui(i1+2,
     & i2+3,i3+0,c3)+cr3*ui(i1+3,i2+3,i3+0,c3)+cr4*ui(i1+4,i2+3,i3+0,
     & c3)+cr5*ui(i1+5,i2+3,i3+0,c3)+cr6*ui(i1+6,i2+3,i3+0,c3)+cr7*ui(
     & i1+7,i2+3,i3+0,c3)+cr8*ui(i1+8,i2+3,i3+0,c3))+cs4*(cr0*ui(i1,
     & i2+4,i3+0,c3)+cr1*ui(i1+1,i2+4,i3+0,c3)+cr2*ui(i1+2,i2+4,i3+0,
     & c3)+cr3*ui(i1+3,i2+4,i3+0,c3)+cr4*ui(i1+4,i2+4,i3+0,c3)+cr5*ui(
     & i1+5,i2+4,i3+0,c3)+cr6*ui(i1+6,i2+4,i3+0,c3)+cr7*ui(i1+7,i2+4,
     & i3+0,c3)+cr8*ui(i1+8,i2+4,i3+0,c3))+cs5*(cr0*ui(i1,i2+5,i3+0,
     & c3)+cr1*ui(i1+1,i2+5,i3+0,c3)+cr2*ui(i1+2,i2+5,i3+0,c3)+cr3*ui(
     & i1+3,i2+5,i3+0,c3)+cr4*ui(i1+4,i2+5,i3+0,c3)+cr5*ui(i1+5,i2+5,
     & i3+0,c3)+cr6*ui(i1+6,i2+5,i3+0,c3)+cr7*ui(i1+7,i2+5,i3+0,c3)+
     & cr8*ui(i1+8,i2+5,i3+0,c3))+cs6*(cr0*ui(i1,i2+6,i3+0,c3)+cr1*ui(
     & i1+1,i2+6,i3+0,c3)+cr2*ui(i1+2,i2+6,i3+0,c3)+cr3*ui(i1+3,i2+6,
     & i3+0,c3)+cr4*ui(i1+4,i2+6,i3+0,c3)+cr5*ui(i1+5,i2+6,i3+0,c3)+
     & cr6*ui(i1+6,i2+6,i3+0,c3)+cr7*ui(i1+7,i2+6,i3+0,c3)+cr8*ui(i1+
     & 8,i2+6,i3+0,c3))+cs7*(cr0*ui(i1,i2+7,i3+0,c3)+cr1*ui(i1+1,i2+7,
     & i3+0,c3)+cr2*ui(i1+2,i2+7,i3+0,c3)+cr3*ui(i1+3,i2+7,i3+0,c3)+
     & cr4*ui(i1+4,i2+7,i3+0,c3)+cr5*ui(i1+5,i2+7,i3+0,c3)+cr6*ui(i1+
     & 6,i2+7,i3+0,c3)+cr7*ui(i1+7,i2+7,i3+0,c3)+cr8*ui(i1+8,i2+7,i3+
     & 0,c3))+cs8*(cr0*ui(i1,i2+8,i3+0,c3)+cr1*ui(i1+1,i2+8,i3+0,c3)+
     & cr2*ui(i1+2,i2+8,i3+0,c3)+cr3*ui(i1+3,i2+8,i3+0,c3)+cr4*ui(i1+
     & 4,i2+8,i3+0,c3)+cr5*ui(i1+5,i2+8,i3+0,c3)+cr6*ui(i1+6,i2+8,i3+
     & 0,c3)+cr7*ui(i1+7,i2+8,i3+0,c3)+cr8*ui(i1+8,i2+8,i3+0,c3)))+
     & ct1*(cs0*(cr0*ui(i1,i2,i3+1,c3)+cr1*ui(i1+1,i2,i3+1,c3)+cr2*ui(
     & i1+2,i2,i3+1,c3)+cr3*ui(i1+3,i2,i3+1,c3)+cr4*ui(i1+4,i2,i3+1,
     & c3)+cr5*ui(i1+5,i2,i3+1,c3)+cr6*ui(i1+6,i2,i3+1,c3)+cr7*ui(i1+
     & 7,i2,i3+1,c3)+cr8*ui(i1+8,i2,i3+1,c3))+cs1*(cr0*ui(i1,i2+1,i3+
     & 1,c3)+cr1*ui(i1+1,i2+1,i3+1,c3)+cr2*ui(i1+2,i2+1,i3+1,c3)+cr3*
     & ui(i1+3,i2+1,i3+1,c3)+cr4*ui(i1+4,i2+1,i3+1,c3)+cr5*ui(i1+5,i2+
     & 1,i3+1,c3)+cr6*ui(i1+6,i2+1,i3+1,c3)+cr7*ui(i1+7,i2+1,i3+1,c3)+
     & cr8*ui(i1+8,i2+1,i3+1,c3))+cs2*(cr0*ui(i1,i2+2,i3+1,c3)+cr1*ui(
     & i1+1,i2+2,i3+1,c3)+cr2*ui(i1+2,i2+2,i3+1,c3)+cr3*ui(i1+3,i2+2,
     & i3+1,c3)+cr4*ui(i1+4,i2+2,i3+1,c3)+cr5*ui(i1+5,i2+2,i3+1,c3)+
     & cr6*ui(i1+6,i2+2,i3+1,c3)+cr7*ui(i1+7,i2+2,i3+1,c3)+cr8*ui(i1+
     & 8,i2+2,i3+1,c3))+cs3*(cr0*ui(i1,i2+3,i3+1,c3)+cr1*ui(i1+1,i2+3,
     & i3+1,c3)+cr2*ui(i1+2,i2+3,i3+1,c3)+cr3*ui(i1+3,i2+3,i3+1,c3)+
     & cr4*ui(i1+4,i2+3,i3+1,c3)+cr5*ui(i1+5,i2+3,i3+1,c3)+cr6*ui(i1+
     & 6,i2+3,i3+1,c3)+cr7*ui(i1+7,i2+3,i3+1,c3)+cr8*ui(i1+8,i2+3,i3+
     & 1,c3))+cs4*(cr0*ui(i1,i2+4,i3+1,c3)+cr1*ui(i1+1,i2+4,i3+1,c3)+
     & cr2*ui(i1+2,i2+4,i3+1,c3)+cr3*ui(i1+3,i2+4,i3+1,c3)+cr4*ui(i1+
     & 4,i2+4,i3+1,c3)+cr5*ui(i1+5,i2+4,i3+1,c3)+cr6*ui(i1+6,i2+4,i3+
     & 1,c3)+cr7*ui(i1+7,i2+4,i3+1,c3)+cr8*ui(i1+8,i2+4,i3+1,c3))+cs5*
     & (cr0*ui(i1,i2+5,i3+1,c3)+cr1*ui(i1+1,i2+5,i3+1,c3)+cr2*ui(i1+2,
     & i2+5,i3+1,c3)+cr3*ui(i1+3,i2+5,i3+1,c3)+cr4*ui(i1+4,i2+5,i3+1,
     & c3)+cr5*ui(i1+5,i2+5,i3+1,c3)+cr6*ui(i1+6,i2+5,i3+1,c3)+cr7*ui(
     & i1+7,i2+5,i3+1,c3)+cr8*ui(i1+8,i2+5,i3+1,c3))+cs6*(cr0*ui(i1,
     & i2+6,i3+1,c3)+cr1*ui(i1+1,i2+6,i3+1,c3)+cr2*ui(i1+2,i2+6,i3+1,
     & c3)+cr3*ui(i1+3,i2+6,i3+1,c3)+cr4*ui(i1+4,i2+6,i3+1,c3)+cr5*ui(
     & i1+5,i2+6,i3+1,c3)+cr6*ui(i1+6,i2+6,i3+1,c3)+cr7*ui(i1+7,i2+6,
     & i3+1,c3)+cr8*ui(i1+8,i2+6,i3+1,c3))+cs7*(cr0*ui(i1,i2+7,i3+1,
     & c3)+cr1*ui(i1+1,i2+7,i3+1,c3)+cr2*ui(i1+2,i2+7,i3+1,c3)+cr3*ui(
     & i1+3,i2+7,i3+1,c3)+cr4*ui(i1+4,i2+7,i3+1,c3)+cr5*ui(i1+5,i2+7,
     & i3+1,c3)+cr6*ui(i1+6,i2+7,i3+1,c3)+cr7*ui(i1+7,i2+7,i3+1,c3)+
     & cr8*ui(i1+8,i2+7,i3+1,c3))+cs8*(cr0*ui(i1,i2+8,i3+1,c3)+cr1*ui(
     & i1+1,i2+8,i3+1,c3)+cr2*ui(i1+2,i2+8,i3+1,c3)+cr3*ui(i1+3,i2+8,
     & i3+1,c3)+cr4*ui(i1+4,i2+8,i3+1,c3)+cr5*ui(i1+5,i2+8,i3+1,c3)+
     & cr6*ui(i1+6,i2+8,i3+1,c3)+cr7*ui(i1+7,i2+8,i3+1,c3)+cr8*ui(i1+
     & 8,i2+8,i3+1,c3)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+ct2*(cs0*(cr0*ui(i1,i2,i3+2,c3)+cr1*ui(i1+1,i2,i3+2,
     & c3)+cr2*ui(i1+2,i2,i3+2,c3)+cr3*ui(i1+3,i2,i3+2,c3)+cr4*ui(i1+
     & 4,i2,i3+2,c3)+cr5*ui(i1+5,i2,i3+2,c3)+cr6*ui(i1+6,i2,i3+2,c3)+
     & cr7*ui(i1+7,i2,i3+2,c3)+cr8*ui(i1+8,i2,i3+2,c3))+cs1*(cr0*ui(
     & i1,i2+1,i3+2,c3)+cr1*ui(i1+1,i2+1,i3+2,c3)+cr2*ui(i1+2,i2+1,i3+
     & 2,c3)+cr3*ui(i1+3,i2+1,i3+2,c3)+cr4*ui(i1+4,i2+1,i3+2,c3)+cr5*
     & ui(i1+5,i2+1,i3+2,c3)+cr6*ui(i1+6,i2+1,i3+2,c3)+cr7*ui(i1+7,i2+
     & 1,i3+2,c3)+cr8*ui(i1+8,i2+1,i3+2,c3))+cs2*(cr0*ui(i1,i2+2,i3+2,
     & c3)+cr1*ui(i1+1,i2+2,i3+2,c3)+cr2*ui(i1+2,i2+2,i3+2,c3)+cr3*ui(
     & i1+3,i2+2,i3+2,c3)+cr4*ui(i1+4,i2+2,i3+2,c3)+cr5*ui(i1+5,i2+2,
     & i3+2,c3)+cr6*ui(i1+6,i2+2,i3+2,c3)+cr7*ui(i1+7,i2+2,i3+2,c3)+
     & cr8*ui(i1+8,i2+2,i3+2,c3))+cs3*(cr0*ui(i1,i2+3,i3+2,c3)+cr1*ui(
     & i1+1,i2+3,i3+2,c3)+cr2*ui(i1+2,i2+3,i3+2,c3)+cr3*ui(i1+3,i2+3,
     & i3+2,c3)+cr4*ui(i1+4,i2+3,i3+2,c3)+cr5*ui(i1+5,i2+3,i3+2,c3)+
     & cr6*ui(i1+6,i2+3,i3+2,c3)+cr7*ui(i1+7,i2+3,i3+2,c3)+cr8*ui(i1+
     & 8,i2+3,i3+2,c3))+cs4*(cr0*ui(i1,i2+4,i3+2,c3)+cr1*ui(i1+1,i2+4,
     & i3+2,c3)+cr2*ui(i1+2,i2+4,i3+2,c3)+cr3*ui(i1+3,i2+4,i3+2,c3)+
     & cr4*ui(i1+4,i2+4,i3+2,c3)+cr5*ui(i1+5,i2+4,i3+2,c3)+cr6*ui(i1+
     & 6,i2+4,i3+2,c3)+cr7*ui(i1+7,i2+4,i3+2,c3)+cr8*ui(i1+8,i2+4,i3+
     & 2,c3))+cs5*(cr0*ui(i1,i2+5,i3+2,c3)+cr1*ui(i1+1,i2+5,i3+2,c3)+
     & cr2*ui(i1+2,i2+5,i3+2,c3)+cr3*ui(i1+3,i2+5,i3+2,c3)+cr4*ui(i1+
     & 4,i2+5,i3+2,c3)+cr5*ui(i1+5,i2+5,i3+2,c3)+cr6*ui(i1+6,i2+5,i3+
     & 2,c3)+cr7*ui(i1+7,i2+5,i3+2,c3)+cr8*ui(i1+8,i2+5,i3+2,c3))+cs6*
     & (cr0*ui(i1,i2+6,i3+2,c3)+cr1*ui(i1+1,i2+6,i3+2,c3)+cr2*ui(i1+2,
     & i2+6,i3+2,c3)+cr3*ui(i1+3,i2+6,i3+2,c3)+cr4*ui(i1+4,i2+6,i3+2,
     & c3)+cr5*ui(i1+5,i2+6,i3+2,c3)+cr6*ui(i1+6,i2+6,i3+2,c3)+cr7*ui(
     & i1+7,i2+6,i3+2,c3)+cr8*ui(i1+8,i2+6,i3+2,c3))+cs7*(cr0*ui(i1,
     & i2+7,i3+2,c3)+cr1*ui(i1+1,i2+7,i3+2,c3)+cr2*ui(i1+2,i2+7,i3+2,
     & c3)+cr3*ui(i1+3,i2+7,i3+2,c3)+cr4*ui(i1+4,i2+7,i3+2,c3)+cr5*ui(
     & i1+5,i2+7,i3+2,c3)+cr6*ui(i1+6,i2+7,i3+2,c3)+cr7*ui(i1+7,i2+7,
     & i3+2,c3)+cr8*ui(i1+8,i2+7,i3+2,c3))+cs8*(cr0*ui(i1,i2+8,i3+2,
     & c3)+cr1*ui(i1+1,i2+8,i3+2,c3)+cr2*ui(i1+2,i2+8,i3+2,c3)+cr3*ui(
     & i1+3,i2+8,i3+2,c3)+cr4*ui(i1+4,i2+8,i3+2,c3)+cr5*ui(i1+5,i2+8,
     & i3+2,c3)+cr6*ui(i1+6,i2+8,i3+2,c3)+cr7*ui(i1+7,i2+8,i3+2,c3)+
     & cr8*ui(i1+8,i2+8,i3+2,c3)))+ct3*(cs0*(cr0*ui(i1,i2,i3+3,c3)+
     & cr1*ui(i1+1,i2,i3+3,c3)+cr2*ui(i1+2,i2,i3+3,c3)+cr3*ui(i1+3,i2,
     & i3+3,c3)+cr4*ui(i1+4,i2,i3+3,c3)+cr5*ui(i1+5,i2,i3+3,c3)+cr6*
     & ui(i1+6,i2,i3+3,c3)+cr7*ui(i1+7,i2,i3+3,c3)+cr8*ui(i1+8,i2,i3+
     & 3,c3))+cs1*(cr0*ui(i1,i2+1,i3+3,c3)+cr1*ui(i1+1,i2+1,i3+3,c3)+
     & cr2*ui(i1+2,i2+1,i3+3,c3)+cr3*ui(i1+3,i2+1,i3+3,c3)+cr4*ui(i1+
     & 4,i2+1,i3+3,c3)+cr5*ui(i1+5,i2+1,i3+3,c3)+cr6*ui(i1+6,i2+1,i3+
     & 3,c3)+cr7*ui(i1+7,i2+1,i3+3,c3)+cr8*ui(i1+8,i2+1,i3+3,c3))+cs2*
     & (cr0*ui(i1,i2+2,i3+3,c3)+cr1*ui(i1+1,i2+2,i3+3,c3)+cr2*ui(i1+2,
     & i2+2,i3+3,c3)+cr3*ui(i1+3,i2+2,i3+3,c3)+cr4*ui(i1+4,i2+2,i3+3,
     & c3)+cr5*ui(i1+5,i2+2,i3+3,c3)+cr6*ui(i1+6,i2+2,i3+3,c3)+cr7*ui(
     & i1+7,i2+2,i3+3,c3)+cr8*ui(i1+8,i2+2,i3+3,c3))+cs3*(cr0*ui(i1,
     & i2+3,i3+3,c3)+cr1*ui(i1+1,i2+3,i3+3,c3)+cr2*ui(i1+2,i2+3,i3+3,
     & c3)+cr3*ui(i1+3,i2+3,i3+3,c3)+cr4*ui(i1+4,i2+3,i3+3,c3)+cr5*ui(
     & i1+5,i2+3,i3+3,c3)+cr6*ui(i1+6,i2+3,i3+3,c3)+cr7*ui(i1+7,i2+3,
     & i3+3,c3)+cr8*ui(i1+8,i2+3,i3+3,c3))+cs4*(cr0*ui(i1,i2+4,i3+3,
     & c3)+cr1*ui(i1+1,i2+4,i3+3,c3)+cr2*ui(i1+2,i2+4,i3+3,c3)+cr3*ui(
     & i1+3,i2+4,i3+3,c3)+cr4*ui(i1+4,i2+4,i3+3,c3)+cr5*ui(i1+5,i2+4,
     & i3+3,c3)+cr6*ui(i1+6,i2+4,i3+3,c3)+cr7*ui(i1+7,i2+4,i3+3,c3)+
     & cr8*ui(i1+8,i2+4,i3+3,c3))+cs5*(cr0*ui(i1,i2+5,i3+3,c3)+cr1*ui(
     & i1+1,i2+5,i3+3,c3)+cr2*ui(i1+2,i2+5,i3+3,c3)+cr3*ui(i1+3,i2+5,
     & i3+3,c3)+cr4*ui(i1+4,i2+5,i3+3,c3)+cr5*ui(i1+5,i2+5,i3+3,c3)+
     & cr6*ui(i1+6,i2+5,i3+3,c3)+cr7*ui(i1+7,i2+5,i3+3,c3)+cr8*ui(i1+
     & 8,i2+5,i3+3,c3))+cs6*(cr0*ui(i1,i2+6,i3+3,c3)+cr1*ui(i1+1,i2+6,
     & i3+3,c3)+cr2*ui(i1+2,i2+6,i3+3,c3)+cr3*ui(i1+3,i2+6,i3+3,c3)+
     & cr4*ui(i1+4,i2+6,i3+3,c3)+cr5*ui(i1+5,i2+6,i3+3,c3)+cr6*ui(i1+
     & 6,i2+6,i3+3,c3)+cr7*ui(i1+7,i2+6,i3+3,c3)+cr8*ui(i1+8,i2+6,i3+
     & 3,c3))+cs7*(cr0*ui(i1,i2+7,i3+3,c3)+cr1*ui(i1+1,i2+7,i3+3,c3)+
     & cr2*ui(i1+2,i2+7,i3+3,c3)+cr3*ui(i1+3,i2+7,i3+3,c3)+cr4*ui(i1+
     & 4,i2+7,i3+3,c3)+cr5*ui(i1+5,i2+7,i3+3,c3)+cr6*ui(i1+6,i2+7,i3+
     & 3,c3)+cr7*ui(i1+7,i2+7,i3+3,c3)+cr8*ui(i1+8,i2+7,i3+3,c3))+cs8*
     & (cr0*ui(i1,i2+8,i3+3,c3)+cr1*ui(i1+1,i2+8,i3+3,c3)+cr2*ui(i1+2,
     & i2+8,i3+3,c3)+cr3*ui(i1+3,i2+8,i3+3,c3)+cr4*ui(i1+4,i2+8,i3+3,
     & c3)+cr5*ui(i1+5,i2+8,i3+3,c3)+cr6*ui(i1+6,i2+8,i3+3,c3)+cr7*ui(
     & i1+7,i2+8,i3+3,c3)+cr8*ui(i1+8,i2+8,i3+3,c3)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+ct4*(cs0*(cr0*ui(i1,i2,i3+4,c3)+cr1*ui(i1+1,i2,i3+4,
     & c3)+cr2*ui(i1+2,i2,i3+4,c3)+cr3*ui(i1+3,i2,i3+4,c3)+cr4*ui(i1+
     & 4,i2,i3+4,c3)+cr5*ui(i1+5,i2,i3+4,c3)+cr6*ui(i1+6,i2,i3+4,c3)+
     & cr7*ui(i1+7,i2,i3+4,c3)+cr8*ui(i1+8,i2,i3+4,c3))+cs1*(cr0*ui(
     & i1,i2+1,i3+4,c3)+cr1*ui(i1+1,i2+1,i3+4,c3)+cr2*ui(i1+2,i2+1,i3+
     & 4,c3)+cr3*ui(i1+3,i2+1,i3+4,c3)+cr4*ui(i1+4,i2+1,i3+4,c3)+cr5*
     & ui(i1+5,i2+1,i3+4,c3)+cr6*ui(i1+6,i2+1,i3+4,c3)+cr7*ui(i1+7,i2+
     & 1,i3+4,c3)+cr8*ui(i1+8,i2+1,i3+4,c3))+cs2*(cr0*ui(i1,i2+2,i3+4,
     & c3)+cr1*ui(i1+1,i2+2,i3+4,c3)+cr2*ui(i1+2,i2+2,i3+4,c3)+cr3*ui(
     & i1+3,i2+2,i3+4,c3)+cr4*ui(i1+4,i2+2,i3+4,c3)+cr5*ui(i1+5,i2+2,
     & i3+4,c3)+cr6*ui(i1+6,i2+2,i3+4,c3)+cr7*ui(i1+7,i2+2,i3+4,c3)+
     & cr8*ui(i1+8,i2+2,i3+4,c3))+cs3*(cr0*ui(i1,i2+3,i3+4,c3)+cr1*ui(
     & i1+1,i2+3,i3+4,c3)+cr2*ui(i1+2,i2+3,i3+4,c3)+cr3*ui(i1+3,i2+3,
     & i3+4,c3)+cr4*ui(i1+4,i2+3,i3+4,c3)+cr5*ui(i1+5,i2+3,i3+4,c3)+
     & cr6*ui(i1+6,i2+3,i3+4,c3)+cr7*ui(i1+7,i2+3,i3+4,c3)+cr8*ui(i1+
     & 8,i2+3,i3+4,c3))+cs4*(cr0*ui(i1,i2+4,i3+4,c3)+cr1*ui(i1+1,i2+4,
     & i3+4,c3)+cr2*ui(i1+2,i2+4,i3+4,c3)+cr3*ui(i1+3,i2+4,i3+4,c3)+
     & cr4*ui(i1+4,i2+4,i3+4,c3)+cr5*ui(i1+5,i2+4,i3+4,c3)+cr6*ui(i1+
     & 6,i2+4,i3+4,c3)+cr7*ui(i1+7,i2+4,i3+4,c3)+cr8*ui(i1+8,i2+4,i3+
     & 4,c3))+cs5*(cr0*ui(i1,i2+5,i3+4,c3)+cr1*ui(i1+1,i2+5,i3+4,c3)+
     & cr2*ui(i1+2,i2+5,i3+4,c3)+cr3*ui(i1+3,i2+5,i3+4,c3)+cr4*ui(i1+
     & 4,i2+5,i3+4,c3)+cr5*ui(i1+5,i2+5,i3+4,c3)+cr6*ui(i1+6,i2+5,i3+
     & 4,c3)+cr7*ui(i1+7,i2+5,i3+4,c3)+cr8*ui(i1+8,i2+5,i3+4,c3))+cs6*
     & (cr0*ui(i1,i2+6,i3+4,c3)+cr1*ui(i1+1,i2+6,i3+4,c3)+cr2*ui(i1+2,
     & i2+6,i3+4,c3)+cr3*ui(i1+3,i2+6,i3+4,c3)+cr4*ui(i1+4,i2+6,i3+4,
     & c3)+cr5*ui(i1+5,i2+6,i3+4,c3)+cr6*ui(i1+6,i2+6,i3+4,c3)+cr7*ui(
     & i1+7,i2+6,i3+4,c3)+cr8*ui(i1+8,i2+6,i3+4,c3))+cs7*(cr0*ui(i1,
     & i2+7,i3+4,c3)+cr1*ui(i1+1,i2+7,i3+4,c3)+cr2*ui(i1+2,i2+7,i3+4,
     & c3)+cr3*ui(i1+3,i2+7,i3+4,c3)+cr4*ui(i1+4,i2+7,i3+4,c3)+cr5*ui(
     & i1+5,i2+7,i3+4,c3)+cr6*ui(i1+6,i2+7,i3+4,c3)+cr7*ui(i1+7,i2+7,
     & i3+4,c3)+cr8*ui(i1+8,i2+7,i3+4,c3))+cs8*(cr0*ui(i1,i2+8,i3+4,
     & c3)+cr1*ui(i1+1,i2+8,i3+4,c3)+cr2*ui(i1+2,i2+8,i3+4,c3)+cr3*ui(
     & i1+3,i2+8,i3+4,c3)+cr4*ui(i1+4,i2+8,i3+4,c3)+cr5*ui(i1+5,i2+8,
     & i3+4,c3)+cr6*ui(i1+6,i2+8,i3+4,c3)+cr7*ui(i1+7,i2+8,i3+4,c3)+
     & cr8*ui(i1+8,i2+8,i3+4,c3)))+ct5*(cs0*(cr0*ui(i1,i2,i3+5,c3)+
     & cr1*ui(i1+1,i2,i3+5,c3)+cr2*ui(i1+2,i2,i3+5,c3)+cr3*ui(i1+3,i2,
     & i3+5,c3)+cr4*ui(i1+4,i2,i3+5,c3)+cr5*ui(i1+5,i2,i3+5,c3)+cr6*
     & ui(i1+6,i2,i3+5,c3)+cr7*ui(i1+7,i2,i3+5,c3)+cr8*ui(i1+8,i2,i3+
     & 5,c3))+cs1*(cr0*ui(i1,i2+1,i3+5,c3)+cr1*ui(i1+1,i2+1,i3+5,c3)+
     & cr2*ui(i1+2,i2+1,i3+5,c3)+cr3*ui(i1+3,i2+1,i3+5,c3)+cr4*ui(i1+
     & 4,i2+1,i3+5,c3)+cr5*ui(i1+5,i2+1,i3+5,c3)+cr6*ui(i1+6,i2+1,i3+
     & 5,c3)+cr7*ui(i1+7,i2+1,i3+5,c3)+cr8*ui(i1+8,i2+1,i3+5,c3))+cs2*
     & (cr0*ui(i1,i2+2,i3+5,c3)+cr1*ui(i1+1,i2+2,i3+5,c3)+cr2*ui(i1+2,
     & i2+2,i3+5,c3)+cr3*ui(i1+3,i2+2,i3+5,c3)+cr4*ui(i1+4,i2+2,i3+5,
     & c3)+cr5*ui(i1+5,i2+2,i3+5,c3)+cr6*ui(i1+6,i2+2,i3+5,c3)+cr7*ui(
     & i1+7,i2+2,i3+5,c3)+cr8*ui(i1+8,i2+2,i3+5,c3))+cs3*(cr0*ui(i1,
     & i2+3,i3+5,c3)+cr1*ui(i1+1,i2+3,i3+5,c3)+cr2*ui(i1+2,i2+3,i3+5,
     & c3)+cr3*ui(i1+3,i2+3,i3+5,c3)+cr4*ui(i1+4,i2+3,i3+5,c3)+cr5*ui(
     & i1+5,i2+3,i3+5,c3)+cr6*ui(i1+6,i2+3,i3+5,c3)+cr7*ui(i1+7,i2+3,
     & i3+5,c3)+cr8*ui(i1+8,i2+3,i3+5,c3))+cs4*(cr0*ui(i1,i2+4,i3+5,
     & c3)+cr1*ui(i1+1,i2+4,i3+5,c3)+cr2*ui(i1+2,i2+4,i3+5,c3)+cr3*ui(
     & i1+3,i2+4,i3+5,c3)+cr4*ui(i1+4,i2+4,i3+5,c3)+cr5*ui(i1+5,i2+4,
     & i3+5,c3)+cr6*ui(i1+6,i2+4,i3+5,c3)+cr7*ui(i1+7,i2+4,i3+5,c3)+
     & cr8*ui(i1+8,i2+4,i3+5,c3))+cs5*(cr0*ui(i1,i2+5,i3+5,c3)+cr1*ui(
     & i1+1,i2+5,i3+5,c3)+cr2*ui(i1+2,i2+5,i3+5,c3)+cr3*ui(i1+3,i2+5,
     & i3+5,c3)+cr4*ui(i1+4,i2+5,i3+5,c3)+cr5*ui(i1+5,i2+5,i3+5,c3)+
     & cr6*ui(i1+6,i2+5,i3+5,c3)+cr7*ui(i1+7,i2+5,i3+5,c3)+cr8*ui(i1+
     & 8,i2+5,i3+5,c3))+cs6*(cr0*ui(i1,i2+6,i3+5,c3)+cr1*ui(i1+1,i2+6,
     & i3+5,c3)+cr2*ui(i1+2,i2+6,i3+5,c3)+cr3*ui(i1+3,i2+6,i3+5,c3)+
     & cr4*ui(i1+4,i2+6,i3+5,c3)+cr5*ui(i1+5,i2+6,i3+5,c3)+cr6*ui(i1+
     & 6,i2+6,i3+5,c3)+cr7*ui(i1+7,i2+6,i3+5,c3)+cr8*ui(i1+8,i2+6,i3+
     & 5,c3))+cs7*(cr0*ui(i1,i2+7,i3+5,c3)+cr1*ui(i1+1,i2+7,i3+5,c3)+
     & cr2*ui(i1+2,i2+7,i3+5,c3)+cr3*ui(i1+3,i2+7,i3+5,c3)+cr4*ui(i1+
     & 4,i2+7,i3+5,c3)+cr5*ui(i1+5,i2+7,i3+5,c3)+cr6*ui(i1+6,i2+7,i3+
     & 5,c3)+cr7*ui(i1+7,i2+7,i3+5,c3)+cr8*ui(i1+8,i2+7,i3+5,c3))+cs8*
     & (cr0*ui(i1,i2+8,i3+5,c3)+cr1*ui(i1+1,i2+8,i3+5,c3)+cr2*ui(i1+2,
     & i2+8,i3+5,c3)+cr3*ui(i1+3,i2+8,i3+5,c3)+cr4*ui(i1+4,i2+8,i3+5,
     & c3)+cr5*ui(i1+5,i2+8,i3+5,c3)+cr6*ui(i1+6,i2+8,i3+5,c3)+cr7*ui(
     & i1+7,i2+8,i3+5,c3)+cr8*ui(i1+8,i2+8,i3+5,c3)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+ct6*(cs0*(cr0*ui(i1,i2,i3+6,c3)+cr1*ui(i1+1,i2,i3+6,
     & c3)+cr2*ui(i1+2,i2,i3+6,c3)+cr3*ui(i1+3,i2,i3+6,c3)+cr4*ui(i1+
     & 4,i2,i3+6,c3)+cr5*ui(i1+5,i2,i3+6,c3)+cr6*ui(i1+6,i2,i3+6,c3)+
     & cr7*ui(i1+7,i2,i3+6,c3)+cr8*ui(i1+8,i2,i3+6,c3))+cs1*(cr0*ui(
     & i1,i2+1,i3+6,c3)+cr1*ui(i1+1,i2+1,i3+6,c3)+cr2*ui(i1+2,i2+1,i3+
     & 6,c3)+cr3*ui(i1+3,i2+1,i3+6,c3)+cr4*ui(i1+4,i2+1,i3+6,c3)+cr5*
     & ui(i1+5,i2+1,i3+6,c3)+cr6*ui(i1+6,i2+1,i3+6,c3)+cr7*ui(i1+7,i2+
     & 1,i3+6,c3)+cr8*ui(i1+8,i2+1,i3+6,c3))+cs2*(cr0*ui(i1,i2+2,i3+6,
     & c3)+cr1*ui(i1+1,i2+2,i3+6,c3)+cr2*ui(i1+2,i2+2,i3+6,c3)+cr3*ui(
     & i1+3,i2+2,i3+6,c3)+cr4*ui(i1+4,i2+2,i3+6,c3)+cr5*ui(i1+5,i2+2,
     & i3+6,c3)+cr6*ui(i1+6,i2+2,i3+6,c3)+cr7*ui(i1+7,i2+2,i3+6,c3)+
     & cr8*ui(i1+8,i2+2,i3+6,c3))+cs3*(cr0*ui(i1,i2+3,i3+6,c3)+cr1*ui(
     & i1+1,i2+3,i3+6,c3)+cr2*ui(i1+2,i2+3,i3+6,c3)+cr3*ui(i1+3,i2+3,
     & i3+6,c3)+cr4*ui(i1+4,i2+3,i3+6,c3)+cr5*ui(i1+5,i2+3,i3+6,c3)+
     & cr6*ui(i1+6,i2+3,i3+6,c3)+cr7*ui(i1+7,i2+3,i3+6,c3)+cr8*ui(i1+
     & 8,i2+3,i3+6,c3))+cs4*(cr0*ui(i1,i2+4,i3+6,c3)+cr1*ui(i1+1,i2+4,
     & i3+6,c3)+cr2*ui(i1+2,i2+4,i3+6,c3)+cr3*ui(i1+3,i2+4,i3+6,c3)+
     & cr4*ui(i1+4,i2+4,i3+6,c3)+cr5*ui(i1+5,i2+4,i3+6,c3)+cr6*ui(i1+
     & 6,i2+4,i3+6,c3)+cr7*ui(i1+7,i2+4,i3+6,c3)+cr8*ui(i1+8,i2+4,i3+
     & 6,c3))+cs5*(cr0*ui(i1,i2+5,i3+6,c3)+cr1*ui(i1+1,i2+5,i3+6,c3)+
     & cr2*ui(i1+2,i2+5,i3+6,c3)+cr3*ui(i1+3,i2+5,i3+6,c3)+cr4*ui(i1+
     & 4,i2+5,i3+6,c3)+cr5*ui(i1+5,i2+5,i3+6,c3)+cr6*ui(i1+6,i2+5,i3+
     & 6,c3)+cr7*ui(i1+7,i2+5,i3+6,c3)+cr8*ui(i1+8,i2+5,i3+6,c3))+cs6*
     & (cr0*ui(i1,i2+6,i3+6,c3)+cr1*ui(i1+1,i2+6,i3+6,c3)+cr2*ui(i1+2,
     & i2+6,i3+6,c3)+cr3*ui(i1+3,i2+6,i3+6,c3)+cr4*ui(i1+4,i2+6,i3+6,
     & c3)+cr5*ui(i1+5,i2+6,i3+6,c3)+cr6*ui(i1+6,i2+6,i3+6,c3)+cr7*ui(
     & i1+7,i2+6,i3+6,c3)+cr8*ui(i1+8,i2+6,i3+6,c3))+cs7*(cr0*ui(i1,
     & i2+7,i3+6,c3)+cr1*ui(i1+1,i2+7,i3+6,c3)+cr2*ui(i1+2,i2+7,i3+6,
     & c3)+cr3*ui(i1+3,i2+7,i3+6,c3)+cr4*ui(i1+4,i2+7,i3+6,c3)+cr5*ui(
     & i1+5,i2+7,i3+6,c3)+cr6*ui(i1+6,i2+7,i3+6,c3)+cr7*ui(i1+7,i2+7,
     & i3+6,c3)+cr8*ui(i1+8,i2+7,i3+6,c3))+cs8*(cr0*ui(i1,i2+8,i3+6,
     & c3)+cr1*ui(i1+1,i2+8,i3+6,c3)+cr2*ui(i1+2,i2+8,i3+6,c3)+cr3*ui(
     & i1+3,i2+8,i3+6,c3)+cr4*ui(i1+4,i2+8,i3+6,c3)+cr5*ui(i1+5,i2+8,
     & i3+6,c3)+cr6*ui(i1+6,i2+8,i3+6,c3)+cr7*ui(i1+7,i2+8,i3+6,c3)+
     & cr8*ui(i1+8,i2+8,i3+6,c3)))+ct7*(cs0*(cr0*ui(i1,i2,i3+7,c3)+
     & cr1*ui(i1+1,i2,i3+7,c3)+cr2*ui(i1+2,i2,i3+7,c3)+cr3*ui(i1+3,i2,
     & i3+7,c3)+cr4*ui(i1+4,i2,i3+7,c3)+cr5*ui(i1+5,i2,i3+7,c3)+cr6*
     & ui(i1+6,i2,i3+7,c3)+cr7*ui(i1+7,i2,i3+7,c3)+cr8*ui(i1+8,i2,i3+
     & 7,c3))+cs1*(cr0*ui(i1,i2+1,i3+7,c3)+cr1*ui(i1+1,i2+1,i3+7,c3)+
     & cr2*ui(i1+2,i2+1,i3+7,c3)+cr3*ui(i1+3,i2+1,i3+7,c3)+cr4*ui(i1+
     & 4,i2+1,i3+7,c3)+cr5*ui(i1+5,i2+1,i3+7,c3)+cr6*ui(i1+6,i2+1,i3+
     & 7,c3)+cr7*ui(i1+7,i2+1,i3+7,c3)+cr8*ui(i1+8,i2+1,i3+7,c3))+cs2*
     & (cr0*ui(i1,i2+2,i3+7,c3)+cr1*ui(i1+1,i2+2,i3+7,c3)+cr2*ui(i1+2,
     & i2+2,i3+7,c3)+cr3*ui(i1+3,i2+2,i3+7,c3)+cr4*ui(i1+4,i2+2,i3+7,
     & c3)+cr5*ui(i1+5,i2+2,i3+7,c3)+cr6*ui(i1+6,i2+2,i3+7,c3)+cr7*ui(
     & i1+7,i2+2,i3+7,c3)+cr8*ui(i1+8,i2+2,i3+7,c3))+cs3*(cr0*ui(i1,
     & i2+3,i3+7,c3)+cr1*ui(i1+1,i2+3,i3+7,c3)+cr2*ui(i1+2,i2+3,i3+7,
     & c3)+cr3*ui(i1+3,i2+3,i3+7,c3)+cr4*ui(i1+4,i2+3,i3+7,c3)+cr5*ui(
     & i1+5,i2+3,i3+7,c3)+cr6*ui(i1+6,i2+3,i3+7,c3)+cr7*ui(i1+7,i2+3,
     & i3+7,c3)+cr8*ui(i1+8,i2+3,i3+7,c3))+cs4*(cr0*ui(i1,i2+4,i3+7,
     & c3)+cr1*ui(i1+1,i2+4,i3+7,c3)+cr2*ui(i1+2,i2+4,i3+7,c3)+cr3*ui(
     & i1+3,i2+4,i3+7,c3)+cr4*ui(i1+4,i2+4,i3+7,c3)+cr5*ui(i1+5,i2+4,
     & i3+7,c3)+cr6*ui(i1+6,i2+4,i3+7,c3)+cr7*ui(i1+7,i2+4,i3+7,c3)+
     & cr8*ui(i1+8,i2+4,i3+7,c3))+cs5*(cr0*ui(i1,i2+5,i3+7,c3)+cr1*ui(
     & i1+1,i2+5,i3+7,c3)+cr2*ui(i1+2,i2+5,i3+7,c3)+cr3*ui(i1+3,i2+5,
     & i3+7,c3)+cr4*ui(i1+4,i2+5,i3+7,c3)+cr5*ui(i1+5,i2+5,i3+7,c3)+
     & cr6*ui(i1+6,i2+5,i3+7,c3)+cr7*ui(i1+7,i2+5,i3+7,c3)+cr8*ui(i1+
     & 8,i2+5,i3+7,c3))+cs6*(cr0*ui(i1,i2+6,i3+7,c3)+cr1*ui(i1+1,i2+6,
     & i3+7,c3)+cr2*ui(i1+2,i2+6,i3+7,c3)+cr3*ui(i1+3,i2+6,i3+7,c3)+
     & cr4*ui(i1+4,i2+6,i3+7,c3)+cr5*ui(i1+5,i2+6,i3+7,c3)+cr6*ui(i1+
     & 6,i2+6,i3+7,c3)+cr7*ui(i1+7,i2+6,i3+7,c3)+cr8*ui(i1+8,i2+6,i3+
     & 7,c3))+cs7*(cr0*ui(i1,i2+7,i3+7,c3)+cr1*ui(i1+1,i2+7,i3+7,c3)+
     & cr2*ui(i1+2,i2+7,i3+7,c3)+cr3*ui(i1+3,i2+7,i3+7,c3)+cr4*ui(i1+
     & 4,i2+7,i3+7,c3)+cr5*ui(i1+5,i2+7,i3+7,c3)+cr6*ui(i1+6,i2+7,i3+
     & 7,c3)+cr7*ui(i1+7,i2+7,i3+7,c3)+cr8*ui(i1+8,i2+7,i3+7,c3))+cs8*
     & (cr0*ui(i1,i2+8,i3+7,c3)+cr1*ui(i1+1,i2+8,i3+7,c3)+cr2*ui(i1+2,
     & i2+8,i3+7,c3)+cr3*ui(i1+3,i2+8,i3+7,c3)+cr4*ui(i1+4,i2+8,i3+7,
     & c3)+cr5*ui(i1+5,i2+8,i3+7,c3)+cr6*ui(i1+6,i2+8,i3+7,c3)+cr7*ui(
     & i1+7,i2+8,i3+7,c3)+cr8*ui(i1+8,i2+8,i3+7,c3)))
               ug(ip(i,1),ip(i,2),ip(i,3),c3) = ug(ip(i,1),ip(i,2),ip(
     & i,3),c3)+ct8*(cs0*(cr0*ui(i1,i2,i3+8,c3)+cr1*ui(i1+1,i2,i3+8,
     & c3)+cr2*ui(i1+2,i2,i3+8,c3)+cr3*ui(i1+3,i2,i3+8,c3)+cr4*ui(i1+
     & 4,i2,i3+8,c3)+cr5*ui(i1+5,i2,i3+8,c3)+cr6*ui(i1+6,i2,i3+8,c3)+
     & cr7*ui(i1+7,i2,i3+8,c3)+cr8*ui(i1+8,i2,i3+8,c3))+cs1*(cr0*ui(
     & i1,i2+1,i3+8,c3)+cr1*ui(i1+1,i2+1,i3+8,c3)+cr2*ui(i1+2,i2+1,i3+
     & 8,c3)+cr3*ui(i1+3,i2+1,i3+8,c3)+cr4*ui(i1+4,i2+1,i3+8,c3)+cr5*
     & ui(i1+5,i2+1,i3+8,c3)+cr6*ui(i1+6,i2+1,i3+8,c3)+cr7*ui(i1+7,i2+
     & 1,i3+8,c3)+cr8*ui(i1+8,i2+1,i3+8,c3))+cs2*(cr0*ui(i1,i2+2,i3+8,
     & c3)+cr1*ui(i1+1,i2+2,i3+8,c3)+cr2*ui(i1+2,i2+2,i3+8,c3)+cr3*ui(
     & i1+3,i2+2,i3+8,c3)+cr4*ui(i1+4,i2+2,i3+8,c3)+cr5*ui(i1+5,i2+2,
     & i3+8,c3)+cr6*ui(i1+6,i2+2,i3+8,c3)+cr7*ui(i1+7,i2+2,i3+8,c3)+
     & cr8*ui(i1+8,i2+2,i3+8,c3))+cs3*(cr0*ui(i1,i2+3,i3+8,c3)+cr1*ui(
     & i1+1,i2+3,i3+8,c3)+cr2*ui(i1+2,i2+3,i3+8,c3)+cr3*ui(i1+3,i2+3,
     & i3+8,c3)+cr4*ui(i1+4,i2+3,i3+8,c3)+cr5*ui(i1+5,i2+3,i3+8,c3)+
     & cr6*ui(i1+6,i2+3,i3+8,c3)+cr7*ui(i1+7,i2+3,i3+8,c3)+cr8*ui(i1+
     & 8,i2+3,i3+8,c3))+cs4*(cr0*ui(i1,i2+4,i3+8,c3)+cr1*ui(i1+1,i2+4,
     & i3+8,c3)+cr2*ui(i1+2,i2+4,i3+8,c3)+cr3*ui(i1+3,i2+4,i3+8,c3)+
     & cr4*ui(i1+4,i2+4,i3+8,c3)+cr5*ui(i1+5,i2+4,i3+8,c3)+cr6*ui(i1+
     & 6,i2+4,i3+8,c3)+cr7*ui(i1+7,i2+4,i3+8,c3)+cr8*ui(i1+8,i2+4,i3+
     & 8,c3))+cs5*(cr0*ui(i1,i2+5,i3+8,c3)+cr1*ui(i1+1,i2+5,i3+8,c3)+
     & cr2*ui(i1+2,i2+5,i3+8,c3)+cr3*ui(i1+3,i2+5,i3+8,c3)+cr4*ui(i1+
     & 4,i2+5,i3+8,c3)+cr5*ui(i1+5,i2+5,i3+8,c3)+cr6*ui(i1+6,i2+5,i3+
     & 8,c3)+cr7*ui(i1+7,i2+5,i3+8,c3)+cr8*ui(i1+8,i2+5,i3+8,c3))+cs6*
     & (cr0*ui(i1,i2+6,i3+8,c3)+cr1*ui(i1+1,i2+6,i3+8,c3)+cr2*ui(i1+2,
     & i2+6,i3+8,c3)+cr3*ui(i1+3,i2+6,i3+8,c3)+cr4*ui(i1+4,i2+6,i3+8,
     & c3)+cr5*ui(i1+5,i2+6,i3+8,c3)+cr6*ui(i1+6,i2+6,i3+8,c3)+cr7*ui(
     & i1+7,i2+6,i3+8,c3)+cr8*ui(i1+8,i2+6,i3+8,c3))+cs7*(cr0*ui(i1,
     & i2+7,i3+8,c3)+cr1*ui(i1+1,i2+7,i3+8,c3)+cr2*ui(i1+2,i2+7,i3+8,
     & c3)+cr3*ui(i1+3,i2+7,i3+8,c3)+cr4*ui(i1+4,i2+7,i3+8,c3)+cr5*ui(
     & i1+5,i2+7,i3+8,c3)+cr6*ui(i1+6,i2+7,i3+8,c3)+cr7*ui(i1+7,i2+7,
     & i3+8,c3)+cr8*ui(i1+8,i2+7,i3+8,c3))+cs8*(cr0*ui(i1,i2+8,i3+8,
     & c3)+cr1*ui(i1+1,i2+8,i3+8,c3)+cr2*ui(i1+2,i2+8,i3+8,c3)+cr3*ui(
     & i1+3,i2+8,i3+8,c3)+cr4*ui(i1+4,i2+8,i3+8,c3)+cr5*ui(i1+5,i2+8,
     & i3+8,c3)+cr6*ui(i1+6,i2+8,i3+8,c3)+cr7*ui(i1+7,i2+8,i3+8,c3)+
     & cr8*ui(i1+8,i2+8,i3+8,c3)))


             end do
             end do
           end if
         else
           ! general case in 3D ********************** fix this ***********************
          stop 7
          end if ! width
        end if ! nd
       else
         write(*,*) 'interpOpt:ERROR; unknown storage option=',
     & storageOption
         stop 3
       end if ! end storage option
       return
       end
