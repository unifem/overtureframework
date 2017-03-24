! This file automatically generated from updateOpt.bf with bpp.





      subroutine updateOpt(nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b, 
     & mask,  u1,u2, ut1,ut2,ut3,ut4, ipar, rpar, ierr )
!======================================================================
!   Update solutions for predictor correct methods etc.
!
!  option = ipar(0)
!  maskOption = ipar(1)
!
!  n1a    = ipar(2)   
!  n1b    = ipar(3)
!  n2a    = ipar(4)
!  n2b    = ipar(5)
!  n3a    = ipar(6)
!  n3b    = ipar(7)
!  n4a    = ipar(8)
!  n4b    = ipar(9)
!
!  ct1 = rpar(0)
!  ct2 = rpar(1)
!  ct3 = rpar(2)
!  ct4 = rpar(3)
!
!  cu1 = rpar(10)
!
!
!  option=2:
!     u2(n1a:n1b,n2a:n2b,n3a:n3b,n4a:n4b) = u1 + ct1*ut1 + ct2*ut2
!
!  option=3:
!     u2(n1a:n1b,n2a:n2b,n3a:n3b,n4a:n4b) = u1 + ct1*ut1 + ct2*ut2 + ct3*ut3
!
!  option=4:
!     u2(n1a:n1b,n2a:n2b,n3a:n3b,n4a:n4b) = u1 + ct1*ut1 + ct2*ut2 + ct3*ut3+ ct4*ut4
!
!  option=5:
!     u2(n1a:n1b,n2a:n2b,n3a:n3b,n4a:n4b) = cu1*u1 + ct1*ut1 + ct2*ut2 + ct3*ut3+ ct4*ut4
!
!   maskOption=0 : assign points where  mask>0 otherwise set u2=u1
!             =1 : assign all points
!
!
!======================================================================
      implicit none
      integer n1a,n1b,n2a,n2b,n3a,n3b,n4a,n4b,nd1a,nd1b,nd2a,nd2b,nd3a,
     & nd3b,nd4a,nd4b

      real u1(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real u2(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real ut1(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real ut2(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real ut3(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real ut4(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer ierr

      integer ipar(0:*)
      real rpar(0:*)

      integer option,maskOption
      real ct1,ct2,ct3,ct4,cu1

!     ---- local variables -----
      integer i1,i2,i3,i4

!     --- end statement functions

      ierr=0


      option    = ipar(0)
      maskOption= ipar(1)
      n1a       = ipar(2)
      n1b       = ipar(3)
      n2a       = ipar(4)
      n2b       = ipar(5)
      n3a       = ipar(6)
      n3b       = ipar(7)
      n4a       = ipar(8)
      n4b       = ipar(9)

      ct1    = rpar(0)
      ct2    = rpar(1)
      ct3    = rpar(2)
      ct4    = rpar(3)
      cu1    = rpar(4)

      if( option.eq.1 .and. maskOption.eq.0 )then

        do i4=n4a,n4b
        do i3=n3a,n3b
        do i2=n2a,n2b
        do i1=n1a,n1b
          if( mask(i1,i2,i3).gt.0 )then
          u2(i1,i2,i3,i4)=u1(i1,i2,i3,i4)+ct1*ut1(i1,i2,i3,i4)
          else
            u2(i1,i2,i3,i4)=u1(i1,i2,i3,i4)
          end if
        end do
        end do
        end do
        end do

      else if( option.eq.2 .and. maskOption.eq.0 )then

        do i4=n4a,n4b
        do i3=n3a,n3b
        do i2=n2a,n2b
        do i1=n1a,n1b
          if( mask(i1,i2,i3).gt.0 )then
          u2(i1,i2,i3,i4)=u1(i1,i2,i3,i4)+ct1*ut1(i1,i2,i3,i4)+ct2*ut2(
     & i1,i2,i3,i4)
          else
            u2(i1,i2,i3,i4)=u1(i1,i2,i3,i4)
          end if
        end do
        end do
        end do
        end do

      else if( option.eq.3 .and. maskOption.eq.0 )then

        do i4=n4a,n4b
        do i3=n3a,n3b
        do i2=n2a,n2b
        do i1=n1a,n1b
          if( mask(i1,i2,i3).gt.0 )then
          u2(i1,i2,i3,i4)=u1(i1,i2,i3,i4)+ct1*ut1(i1,i2,i3,i4)+ct2*ut2(
     & i1,i2,i3,i4)+ct3*ut3(i1,i2,i3,i4)
          else
            u2(i1,i2,i3,i4)=u1(i1,i2,i3,i4)
          end if
        end do
        end do
        end do
        end do

      else if( option.eq.4 .and. maskOption.eq.0 )then

        do i4=n4a,n4b
        do i3=n3a,n3b
        do i2=n2a,n2b
        do i1=n1a,n1b
          if( mask(i1,i2,i3).gt.0 )then
          u2(i1,i2,i3,i4)=u1(i1,i2,i3,i4)+ct1*ut1(i1,i2,i3,i4)+ct2*ut2(
     & i1,i2,i3,i4)+ct3*ut3(i1,i2,i3,i4)+ct4*ut4(i1,i2,i3,i4)
          else
            u2(i1,i2,i3,i4)=u1(i1,i2,i3,i4)
          end if
        end do
        end do
        end do
        end do

      else if( option.eq.5 .and. maskOption.eq.0 )then

        ! *wdh* Jan 28, 2017 -- for IMEX-BDF2
        do i4=n4a,n4b
        do i3=n3a,n3b
        do i2=n2a,n2b
        do i1=n1a,n1b
          if( mask(i1,i2,i3).gt.0 )then
          u2(i1,i2,i3,i4)=cu1*u1(i1,i2,i3,i4)+ct1*ut1(i1,i2,i3,i4)+ct2*
     & ut2(i1,i2,i3,i4)+ct3*ut3(i1,i2,i3,i4)+ct4*ut4(i1,i2,i3,i4)
          else
            u2(i1,i2,i3,i4)=u1(i1,i2,i3,i4)
          end if
        end do
        end do
        end do
        end do

      else if( option.eq.1 .and. maskOption.eq.1 )then

        do i4=n4a,n4b
        do i3=n3a,n3b
        do i2=n2a,n2b
        do i1=n1a,n1b
          u2(i1,i2,i3,i4)=u1(i1,i2,i3,i4)+ct1*ut1(i1,i2,i3,i4)
        end do
        end do
        end do
        end do

      else if( option.eq.2 .and. maskOption.eq.1 )then

        do i4=n4a,n4b
        do i3=n3a,n3b
        do i2=n2a,n2b
        do i1=n1a,n1b
          u2(i1,i2,i3,i4)=u1(i1,i2,i3,i4)+ct1*ut1(i1,i2,i3,i4)+ct2*ut2(
     & i1,i2,i3,i4)
        end do
        end do
        end do
        end do

      else
        write(*,'("updateOpt: ERROR: unknown values for option=",i3," 
     & maskOption=",i3)') option,maskOption
        stop 6183
      end if

      return
      end



      subroutine updateOptNew(nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b, 
     & mask,  uNew, u1,u2,u3,u4,u5,u6,u7,u8,u9,u10, ipar, rpar, ierr )
!======================================================================
!   Update solutions for predictor correct methods etc.
!
!  option = ipar(0)
!  maskOption = ipar(1)
!
!  n1a    = ipar(2)   
!  n1b    = ipar(3)
!  n2a    = ipar(4)
!  n2b    = ipar(5)
!  n3a    = ipar(6)
!  n3b    = ipar(7)
!  n4a    = ipar(8)
!  n4b    = ipar(9)
!
!  c1 = rpar(0)
!  c2 = rpar(1)
!  c3 = rpar(2)
!  c4 = rpar(3)
!  etc.
!
!  option=3:
!     uNew(n1a:n1b,n2a:n2b,n3a:n3b,n4a:n4b) = c1*u1 + c2*u2 + c3*u3
!
!  option=4:
!     uNew(n1a:n1b,n2a:n2b,n3a:n3b,n4a:n4b) = c1*u1 + c2*u2 + c3*u3 + c4*u4
!
!  option=M: 
!     u2(n1a:n1b,n2a:n2b,n3a:n3b,n4a:n4b) = SUM_k=1^M c_k * u_k 
!
!
!======================================================================
      implicit none
      integer n1a,n1b,n2a,n2b,n3a,n3b,n4a,n4b,nd1a,nd1b,nd2a,nd2b,nd3a,
     & nd3b,nd4a,nd4b

      real uNew(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)

      real u1(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real u2(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real u3(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real u4(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real u5(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real u6(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real u7(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real u8(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real u9(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real u10(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer ierr

      integer ipar(0:*)
      real rpar(0:*)

      integer option,maskOption
      real c1,c2,c3,c4,c5,c6,c7,c8,c9,c10

!     ---- local variables -----
      integer i1,i2,i3,i4

!     --- end statement functions

      ierr=0


      option    = ipar(0)
      maskOption= ipar(1)
      n1a       = ipar(2)
      n1b       = ipar(3)
      n2a       = ipar(4)
      n2b       = ipar(5)
      n3a       = ipar(6)
      n3b       = ipar(7)
      n4a       = ipar(8)
      n4b       = ipar(9)

      c1    = rpar(0)
      c2    = rpar(1)
      c3    = rpar(2)
      c4    = rpar(3)
      c5    = rpar(4)
      c6    = rpar(5)
      c7    = rpar(6)
      c8    = rpar(7)
      c9    = rpar(8)
      c10   = rpar(9)

      if( option.eq.1 )then

        do i4=n4a,n4b
        do i3=n3a,n3b
        do i2=n2a,n2b
        do i1=n1a,n1b
          if( mask(i1,i2,i3).gt.0 )then
          u2(i1,i2,i3,i4)=c1*u1(i1,i2,i3,i4)
          else
            u2(i1,i2,i3,i4)=u1(i1,i2,i3,i4)
          end if
        end do
        end do
        end do
        end do

      else if( option.eq.2 )then

        do i4=n4a,n4b
        do i3=n3a,n3b
        do i2=n2a,n2b
        do i1=n1a,n1b
          if( mask(i1,i2,i3).gt.0 )then
          uNew(i1,i2,i3,i4)=c1*u1(i1,i2,i3,i4)+c2*u2(i1,i2,i3,i4)
          else
            u2(i1,i2,i3,i4)=u1(i1,i2,i3,i4)
          end if
        end do
        end do
        end do
        end do

       else if( option.eq.3 )then

        do i4=n4a,n4b
        do i3=n3a,n3b
        do i2=n2a,n2b
        do i1=n1a,n1b
          if( mask(i1,i2,i3).gt.0 )then
          uNew(i1,i2,i3,i4)=c1*u1(i1,i2,i3,i4)+c2*u2(i1,i2,i3,i4)+c3*
     & u3(i1,i2,i3,i4)
          else
            u2(i1,i2,i3,i4)=u1(i1,i2,i3,i4)
          end if
        end do
        end do
        end do
        end do

      else if( option.eq.4 )then

        do i4=n4a,n4b
        do i3=n3a,n3b
        do i2=n2a,n2b
        do i1=n1a,n1b
          if( mask(i1,i2,i3).gt.0 )then
          uNew(i1,i2,i3,i4)=c1*u1(i1,i2,i3,i4)+c2*u2(i1,i2,i3,i4)+c3*
     & u3(i1,i2,i3,i4)+c4*u4(i1,i2,i3,i4)
          else
            u2(i1,i2,i3,i4)=u1(i1,i2,i3,i4)
          end if
        end do
        end do
        end do
        end do

      else if( option.eq.5 )then

        do i4=n4a,n4b
        do i3=n3a,n3b
        do i2=n2a,n2b
        do i1=n1a,n1b
          if( mask(i1,i2,i3).gt.0 )then
          uNew(i1,i2,i3,i4)=c1*u1(i1,i2,i3,i4)+c2*u2(i1,i2,i3,i4)+c3*
     & u3(i1,i2,i3,i4)+c4*u4(i1,i2,i3,i4)+c5*u5(i1,i2,i3,i4)
          else
            u2(i1,i2,i3,i4)=u1(i1,i2,i3,i4)
          end if
        end do
        end do
        end do
        end do

      else if( option.eq.6 )then

        do i4=n4a,n4b
        do i3=n3a,n3b
        do i2=n2a,n2b
        do i1=n1a,n1b
          if( mask(i1,i2,i3).gt.0 )then
          uNew(i1,i2,i3,i4)=c1*u1(i1,i2,i3,i4)+c2*u2(i1,i2,i3,i4)+c3*
     & u3(i1,i2,i3,i4)+c4*u4(i1,i2,i3,i4)+c5*u5(i1,i2,i3,i4)+c6*u6(i1,
     & i2,i3,i4)
          else
            u2(i1,i2,i3,i4)=u1(i1,i2,i3,i4)
          end if
        end do
        end do
        end do
        end do

      else if( option.eq.7 )then

        do i4=n4a,n4b
        do i3=n3a,n3b
        do i2=n2a,n2b
        do i1=n1a,n1b
          if( mask(i1,i2,i3).gt.0 )then
          uNew(i1,i2,i3,i4)=c1*u1(i1,i2,i3,i4)+c2*u2(i1,i2,i3,i4)+c3*
     & u3(i1,i2,i3,i4)+c4*u4(i1,i2,i3,i4)+c5*u5(i1,i2,i3,i4)+c6*u6(i1,
     & i2,i3,i4)+c7*u7(i1,i2,i3,i4)
          else
            u2(i1,i2,i3,i4)=u1(i1,i2,i3,i4)
          end if
        end do
        end do
        end do
        end do

      else if( option.eq.8 )then

        do i4=n4a,n4b
        do i3=n3a,n3b
        do i2=n2a,n2b
        do i1=n1a,n1b
          if( mask(i1,i2,i3).gt.0 )then
          uNew(i1,i2,i3,i4)=c1*u1(i1,i2,i3,i4)+c2*u2(i1,i2,i3,i4)+c3*
     & u3(i1,i2,i3,i4)+c4*u4(i1,i2,i3,i4)+c5*u5(i1,i2,i3,i4)+c6*u6(i1,
     & i2,i3,i4)+c7*u7(i1,i2,i3,i4)+c8*u8(i1,i2,i3,i4)
          else
            u2(i1,i2,i3,i4)=u1(i1,i2,i3,i4)
          end if
        end do
        end do
        end do
        end do

      else if( option.eq.9 )then

        do i4=n4a,n4b
        do i3=n3a,n3b
        do i2=n2a,n2b
        do i1=n1a,n1b
          if( mask(i1,i2,i3).gt.0 )then
          uNew(i1,i2,i3,i4)=c1*u1(i1,i2,i3,i4)+c2*u2(i1,i2,i3,i4)+c3*
     & u3(i1,i2,i3,i4)+c4*u4(i1,i2,i3,i4)+c5*u5(i1,i2,i3,i4)+c6*u6(i1,
     & i2,i3,i4)+c7*u7(i1,i2,i3,i4)+c8*u8(i1,i2,i3,i4)+c9*u9(i1,i2,i3,
     & i4)
          else
            u2(i1,i2,i3,i4)=u1(i1,i2,i3,i4)
          end if
        end do
        end do
        end do
        end do

      else if( option.eq.10 )then

        do i4=n4a,n4b
        do i3=n3a,n3b
        do i2=n2a,n2b
        do i1=n1a,n1b
          if( mask(i1,i2,i3).gt.0 )then
          uNew(i1,i2,i3,i4)=c1*u1(i1,i2,i3,i4)+c2*u2(i1,i2,i3,i4)+c3*
     & u3(i1,i2,i3,i4)+c4*u4(i1,i2,i3,i4)+c5*u5(i1,i2,i3,i4)+c6*u6(i1,
     & i2,i3,i4)+c7*u7(i1,i2,i3,i4)+c8*u8(i1,i2,i3,i4)+c9*u9(i1,i2,i3,
     & i4)+c10*u10(i1,i2,i3,i4)
          else
            u2(i1,i2,i3,i4)=u1(i1,i2,i3,i4)
          end if
        end do
        end do
        end do
        end do


      else
        write(*,'("updateOpt: ERROR: unknown values for option=",i3," 
     & maskOption=",i3)') option,maskOption
        stop 6183
      end if

      return
      end
