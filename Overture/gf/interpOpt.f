! This file automatically generated from interpOpt.bf with bpp.
c Define optimized overlapping grid interpolation for the Interpolant class










c **** formulae below generated from interp.maple -> file=higherOrderInterp.h ***********

! #Include "higherOrderInterp.h"


c =====2D : order 1 ====
c kkc 050121
c hey, at least I can do this one!

c =====2D : order 2 ====




c =====3D : order 1 ====
c kkc 050121
c hey, at least I can do this one!


c =====3D : order 2 ====




c =====2D : order 3 ====




c =====3D : order 3 ====




c =====2D : order 4 ====




c =====3D : order 4 ====




c =====2D : order 5 ====




c =====3D : order 5 ====




c =====2D : order 6 ====




c =====3D : order 6 ====




c =====2D : order 7 ====




c =====3D : order 7 ====




c =====2D : order 8 ====




c =====3D : order 8 ====




c =====2D : order 9 ====




c =====3D : order 9 ====








      subroutine interpOptRes( nd,
     & ndui1a,ndui1b,ndui2a,ndui2b,ndui3a,ndui3b,ndui4a,ndui4b,
     & ndug1a,ndug1b,ndug2a,ndug2b,ndug3a,ndug3b,ndug4a,ndug4b,
     & ndil,ndip,ndc1,ndc2,ndc3,
     &   ipar,
     &   ui,ug,c,r,il,ip,varWidth,width, resMax )
c=================================================================================
c  Optimised interpolation with residual computation.
c   This version is for the iterative implicit method
c  since it also computes a residual.
c=================================================================================

      implicit none

      integer nd,nia,nib,c2a,c2b,c3a,c3b,ndil,ndip,ndc1,ndc2,ndc3
      integer ndui1a,ndui1b,ndui2a,ndui2b,ndui3a,ndui3b,ndui4a,ndui4b,
     &        ndug1a,ndug1b,ndug2a,ndug2b,ndug3a,ndug3b,ndug4a,ndug4b

      real ui(ndui1a:ndui1b,ndui2a:ndui2b,ndui3a:ndui3b,ndui4a:ndui4b)
      real ug(ndug1a:ndug1b,ndug2a:ndug2b,ndug3a:ndug3b,ndug4a:ndug4b)
      real r(0:*),resMax
      real c(0:ndc1-1,0:ndc2-1,0:ndc3-1,0:*)
      integer width(3), il(0:ndil-1,*), ip(0:ndip-1,*), varWidth(0:*)
      integer ipar(0:*),storageOption

      storageOption=ipar(6)

      if( storageOption.eq.0 )then

c       ******************************
c       **** full storage option *****
c       ******************************

        call interpOptResFull(nd,
     & ndui1a,ndui1b,ndui2a,ndui2b,ndui3a,ndui3b,ndui4a,ndui4b,
     & ndug1a,ndug1b,ndug2a,ndug2b,ndug3a,ndug3b,ndug4a,ndug4b,
     & ndil,ndip,ndc1,ndc2,ndc3,
     &   ipar,
     &   ui,ug,c,r,il,ip,varWidth,width, resMax )

      else if( storageOption.eq.1 )then

c       ****************************************
c       **** tensor-product storage option *****
c       ****************************************

        call interpOptResTP(nd,
     & ndui1a,ndui1b,ndui2a,ndui2b,ndui3a,ndui3b,ndui4a,ndui4b,
     & ndug1a,ndug1b,ndug2a,ndug2b,ndug3a,ndug3b,ndug4a,ndug4b,
     & ndil,ndip,ndc1,ndc2,ndc3,
     &   ipar,
     &   ui,ug,c,r,il,ip,varWidth,width, resMax )

      else if( storageOption.eq.2 )then

c       ****************************************
c       **** sparse         storage option *****
c       ****************************************

        call interpOptResSP(nd,
     & ndui1a,ndui1b,ndui2a,ndui2b,ndui3a,ndui3b,ndui4a,ndui4b,
     & ndug1a,ndug1b,ndug2a,ndug2b,ndug3a,ndug3b,ndug4a,ndug4b,
     & ndil,ndip,ndc1,ndc2,ndc3,
     &   ipar,
     &   ui,ug,c,r,il,ip,varWidth,width, resMax )

      else
        write(*,*) 'interpOptRes:ERROR; unknown storage option=',
     & storageOption
      end if ! end storage option


      return
      end


      subroutine interpOpt( nd,
     & ndui1a,ndui1b,ndui2a,ndui2b,ndui3a,ndui3b,ndui4a,ndui4b,
     & ndug1a,ndug1b,ndug2a,ndug2b,ndug3a,ndug3b,ndug4a,ndug4b,
     & ndil,ndip,ndc1,ndc2,ndc3,
     &   ipar,
     &   ui,ug,c,il,ip,varWidth, width )
c=================================================================================
c  Optimised interpolation
c=================================================================================

      implicit none

      integer nd,nia,nib,c2a,c2b,c3a,c3b,ndil,ndip,ndc1,ndc2,ndc3
      integer ndui1a,ndui1b,ndui2a,ndui2b,ndui3a,ndui3b,ndui4a,ndui4b,
     &        ndug1a,ndug1b,ndug2a,ndug2b,ndug3a,ndug3b,ndug4a,ndug4b

      real ui(ndui1a:ndui1b,ndui2a:ndui2b,ndui3a:ndui3b,ndui4a:ndui4b)
      real ug(ndug1a:ndug1b,ndug2a:ndug2b,ndug3a:ndug3b,ndug4a:ndug4b)
      real c(0:ndc1-1,0:ndc2-1,0:ndc3-1,0:*)
      integer width(3), il(0:ndil-1,*), ip(0:ndip-1,*), varWidth(0:*)
      integer ipar(0:*)
      integer storageOption

      storageOption=ipar(6)

      if( storageOption.eq.0 )then

c       ******************************
c       **** full storage option *****
c       ******************************

        call interpOptFull( nd,
     & ndui1a,ndui1b,ndui2a,ndui2b,ndui3a,ndui3b,ndui4a,ndui4b,
     & ndug1a,ndug1b,ndug2a,ndug2b,ndug3a,ndug3b,ndug4a,ndug4b,
     & ndil,ndip,ndc1,ndc2,ndc3,ipar,ui,ug,c,il,ip,varWidth,width )

      else if( storageOption.eq.1 )then

c       ****************************************
c       **** tensor-product storage option *****
c       ****************************************

        call interpOptTP( nd,
     & ndui1a,ndui1b,ndui2a,ndui2b,ndui3a,ndui3b,ndui4a,ndui4b,
     & ndug1a,ndug1b,ndug2a,ndug2b,ndug3a,ndug3b,ndug4a,ndug4b,
     & ndil,ndip,ndc1,ndc2,ndc3,ipar,ui,ug,c,il,ip,varWidth,width )

      else if( storageOption.eq.2 )then

c       ****************************************
c       **** sparse         storage option *****
c       ****************************************

        call interpOptSP( nd,
     & ndui1a,ndui1b,ndui2a,ndui2b,ndui3a,ndui3b,ndui4a,ndui4b,
     & ndug1a,ndug1b,ndug2a,ndug2b,ndug3a,ndug3b,ndug4a,ndug4b,
     & ndil,ndip,ndc1,ndc2,ndc3,ipar,ui,ug,c,il,ip,varWidth,width )

      else
        write(*,*) 'interpOpt:ERROR; unknown storage option=',
     & storageOption
        stop 3
      end if ! end storage option
      return
      end




c  We need to save these in separate files for the dec compiler

! buildFile(Full)
! buildFile(TP)
! buildFile(SP)


c use the mask 


       subroutine fixupOpt( nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,
     &    n1a,n1b,n2a,n2b,n3a,n3b, u,val, mask, bc, nMin,nMax, nGhost )
c===================================================================================
c Fixup unused points : optimized version
c
c    Set values at:
c      1) Any point where the mask==0
c      2) All ghost points on ghost lines greater than "nGhost" on boundaries where bc>0
c===================================================================================
      implicit none
      integer nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,
     &        n1a,n1b,n2a,n2b,n3a,n3b, nMin,nMax, nGhost

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real val(nd4a:*)
      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer bc(0:1,0:*)

c...........local variables
      integer i1,i2,i3,n,side,axis
      integer m1a,m1b,m2a,m2b,m3a,m3b


! beginLoops(nd1a,nd1b,nd2a,nd2b,nd3a,nd3b)
      do i3=nd3a,nd3b
      do i2=nd2a,nd2b
      do i1=nd1a,nd1b
        if( mask(i1,i2,i3).eq.0 )then
          do n=nMin,nMax
            u(i1,i2,i3,n)=val(n)
          end do
        end if
! endLoops()
      end do
      end do
      end do

      ! we set all values outside "nGhost" ghost lines
      do axis=0,nd-1
      do side=0,1
        if( bc(side,axis).gt.0 )then

          m1a=nd1a
          m1b=nd1b
          m2a=nd2a
          m2b=nd2b
          m3a=nd3a
          m3b=nd3b

          if( side.eq.0 )then
            if( axis.eq.0 )then
              m1b=n1a-nGhost-1
            else if( axis.eq.1 )then
              m2b=n2a-nGhost-1
            else
              m3b=n3a-nGhost-1
            end if
          else
            if( axis.eq.0 )then
              m1a=n1b+nGhost+1
            else if( axis.eq.1 )then
              m2a=n2b+nGhost+1
            else
              m3a=n3b+nGhost+1
            end if
          end if

! beginLoops(m1a,m1b,m2a,m2b,m3a,m3b)
          do i3=m3a,m3b
          do i2=m2a,m2b
          do i1=m1a,m1b
            do n=nMin,nMax
              u(i1,i2,i3,n)=val(n)
            end do
! endLoops()
          end do
          end do
          end do

        end if
      end do
      end do

      return
      end

