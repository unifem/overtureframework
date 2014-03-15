#beginMacro beginLoops()
do i4=n4a,n4b
do i3=n3a,n3b
do i2=n2a,n2b
do i1=n1a,n1b
#endMacro

#beginMacro endLoops()
end do
end do
end do
end do
#endMacro

#beginMacro beginLoopsMask()
do i4=n4a,n4b
do i3=n3a,n3b
do i2=n2a,n2b
do i1=n1a,n1b
  if( mask(i1,i2,i3).gt.0 )then
#endMacro

#beginMacro endLoopsMask()
  else
    u2(i1,i2,i3,i4)=u1(i1,i2,i3,i4)
  end if
end do
end do
end do
end do
#endMacro


      subroutine updateOpt(nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b, \
                           mask,  u1,u2, ut1,ut2,ut3,ut4, ipar, rpar, ierr )
c======================================================================
c   Update solutions for predictor correct methods etc.
c
c  option = ipar(0)
c  maskOption = ipar(1)
c
c  n1a    = ipar(2)   
c  n1b    = ipar(3)
c  n2a    = ipar(4)
c  n2b    = ipar(5)
c  n3a    = ipar(6)
c  n3b    = ipar(7)
c  n4a    = ipar(8)
c  n4b    = ipar(9)
c
c  ct1 = rpar(0)
c  ct2 = rpar(1)
c
c  option=2:
c     u2(n1a:n1b,n2a:n2b,n3a:n3b,n4a:n4b) = u1 + ct1*ut1 + ct2*ut2
c
c  option=3:
c     u2(n1a:n1b,n2a:n2b,n3a:n3b,n4a:n4b) = u1 + ct1*ut1 + ct2*ut2 + ct3*ut3
c
c  option=4:
c     u2(n1a:n1b,n2a:n2b,n3a:n3b,n4a:n4b) = u1 + ct1*ut1 + ct2*ut2 + ct3*ut3+ ct4*ut4
c
c   maskOption=0 : assign points where  mask>0 otherwise set u2=u1
c             =1 : assign all points
c
c
c======================================================================
      implicit none
      integer n1a,n1b,n2a,n2b,n3a,n3b,n4a,n4b,\
       nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b

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
      real ct1,ct2,ct3,ct4

c     ---- local variables -----
      integer i1,i2,i3,i4

c     --- end statement functions

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

      if( option.eq.1 .and. maskOption.eq.0 )then

        beginLoopsMask()
          u2(i1,i2,i3,i4)=u1(i1,i2,i3,i4)+ct1*ut1(i1,i2,i3,i4)
        endLoopsMask()

      else if( option.eq.2 .and. maskOption.eq.0 )then

        beginLoopsMask()
          u2(i1,i2,i3,i4)=u1(i1,i2,i3,i4)+ct1*ut1(i1,i2,i3,i4)+ct2*ut2(i1,i2,i3,i4)
        endLoopsMask()

      else if( option.eq.3 .and. maskOption.eq.0 )then

        beginLoopsMask()
          u2(i1,i2,i3,i4)=u1(i1,i2,i3,i4)+ct1*ut1(i1,i2,i3,i4)+ct2*ut2(i1,i2,i3,i4)+ct3*ut3(i1,i2,i3,i4)
        endLoopsMask()

      else if( option.eq.4 .and. maskOption.eq.0 )then

        beginLoopsMask()
          u2(i1,i2,i3,i4)=u1(i1,i2,i3,i4)+ct1*ut1(i1,i2,i3,i4)+ct2*ut2(i1,i2,i3,i4)+ct3*ut3(i1,i2,i3,i4)+ct4*ut4(i1,i2,i3,i4)
        endLoopsMask()

      else if( option.eq.1 .and. maskOption.eq.1 )then

        beginLoops()
          u2(i1,i2,i3,i4)=u1(i1,i2,i3,i4)+ct1*ut1(i1,i2,i3,i4)
        endLoops()

      else if( option.eq.2 .and. maskOption.eq.1 )then

        beginLoops()
          u2(i1,i2,i3,i4)=u1(i1,i2,i3,i4)+ct1*ut1(i1,i2,i3,i4)+ct2*ut2(i1,i2,i3,i4)
        endLoops()

      else
        write(*,'("updateOpt: ERROR: unknown values for option=",i3," maskOption=",i3)') option,maskOption
        stop 6183
      end if

      return
      end
