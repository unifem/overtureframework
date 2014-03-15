c Define optimized overlapping grid interpolation for the Interpolant class

#beginMacro loops2d(e1,e2,e3)
if( c2a.eq.c2b .and. c3a.eq.c3b )then
  do c3=c3a,c3b
  do c2=c2a,c2b
  do i=nia,nib
    e1
    e2
    e3
  end do
  end do
  end do
else
  ! put "c" loop as inner loop, this seems to be faster
  do i=nia,nib
  do c3=c3a,c3b
  do c2=c2a,c2b
    e1
    e2
    e3
  end do
  end do
  end do
end if
#endMacro

#beginMacro loops3d(e1,e2,e3)
if( c3a.eq.c3b )then
  do c3=c3a,c3b
  do i=nia,nib
    e1
    e2
    e3
  end do
  end do
else
  ! put "c" loop as inner loop, this seems to be faster
  do i=nia,nib
  do c3=c3a,c3b
    e1
    e2
    e3
  end do
  end do
end if
#endMacro


#beginMacro beginLoops2d()
  do i=nia,nib
  do c3=c3a,c3b
  do c2=c2a,c2b
#endMacro

#beginMacro endLoops2d()
  end do
  end do
  end do
#endMacro

#beginMacro beginLoops3d()
  do i=nia,nib
  do c3=c3a,c3b
#endMacro

#beginMacro endLoops3d()
  end do
  end do
#endMacro


#beginMacro interp11(lhs)
i1=il(i,1)
i2=il(i,2)
lhs = ui(i1  ,i2  ,c2,c3)
#endMacro
#beginMacro interp111(lhs)
i1=il(i,1)
i2=il(i,2)
i3=il(i,3)
lhs = ui(i1,i2,i3,c3)
#endMacro

c **** formulae below generated from interp.maple -> file=higherOrderInterp.h ***********

#Include "higherOrderInterp.h"

#beginMacro defineInterpOptRes(STORAGE)

subroutine interpOptRes ## STORAGE ( nd,\
 ndui1a,ndui1b,ndui2a,ndui2b,ndui3a,ndui3b,ndui4a,ndui4b,\
 ndug1a,ndug1b,ndug2a,ndug2b,ndug3a,ndug3b,ndug4a,ndug4b,\
 ndil,ndip,ndc1,ndc2,ndc3,\
   ipar,\
   ui,ug,c,r,il,ip,varWidth, width, resMax )
c=================================================================================
c  Optimised interpolation with residual computation.
c   This version is for the iterative implicit method
c  since it also computes a residual.
c=================================================================================

implicit none

integer nd,nia,nib,c2a,c2b,c3a,c3b,ndil,ndip,ndc1,ndc2,ndc3
integer ndui1a,ndui1b,ndui2a,ndui2b,ndui3a,ndui3b,ndui4a,ndui4b,\
        ndug1a,ndug1b,ndug2a,ndug2b,ndug3a,ndug3b,ndug4a,ndug4b

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
      #Include "lagrangePolynomials.h"
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

#If #STORAGE == "Full"
if( storageOption.eq.0 )then

c       ******************************
c       **** full storage option *****
c       ******************************

if( nd.eq.2 )then

  if( useVariableWidthInterpolation.ne.0 )then

    beginLoops2d()
      ! check for most common widths first
      if( varWidth(i).eq.3 )then
        interp33(r(i))
      else if( varWidth(i).eq.2 )then
        interp22(r(i))
      else if( varWidth(i).eq.1 )then
        interp11(r(i))
      else if( varWidth(i).eq.5 )then
        interp55(r(i))
      else if( varWidth(i).eq.4 )then
        interp44(r(i))
      else if( varWidth(i).eq.7 )then
        interp77(r(i))
      else if( varWidth(i).eq.6 )then
        interp66(r(i))
      else if( varWidth(i).eq.9 )then
        interp99(r(i))
      else if( varWidth(i).eq.8 )then
        interp88(r(i))
      else
        write(*,*) 'ERROR varWidth=',varWidth(i) 
        stop 151
      end if

      resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i)))
      ug(ip(i,1),ip(i,2),c2,c3)= r(i)
    endLoops2d()

  else if( width(1).eq.3 .and. width(2).eq.3 ) then

    loops2d($interp33(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),\
          ug(ip(i,1),ip(i,2),c2,c3)= r(i))

  else if( width(1).eq.1 .and. width(1).eq.1)then

    loops2d($interp11(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),\
          ug(ip(i,1),ip(i,2),c2,c3)= r(i))

  else if( width(1).eq.2 .and. width(2).eq.2 )then

    loops2d($interp22(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),\
          ug(ip(i,1),ip(i,2),c2,c3)= r(i))

  else if( width(1).eq.4 .and. width(2).eq.4 )then

    loops2d($interp44(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),\
          ug(ip(i,1),ip(i,2),c2,c3)= r(i))

  else if( width(1).eq.5 .and. width(2).eq.5 )then

    loops2d($interp55(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),\
          ug(ip(i,1),ip(i,2),c2,c3)= r(i))

  else if( width(1).eq.6 .and. width(2).eq.6 )then

    loops2d($interp66(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),\
          ug(ip(i,1),ip(i,2),c2,c3)= r(i))

  else if( width(1).eq.7 .and. width(2).eq.7 )then

    loops2d($interp77(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),\
          ug(ip(i,1),ip(i,2),c2,c3)= r(i))

  else if( width(1).eq.8 .and. width(2).eq.8 )then

    loops2d($interp88(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),\
          ug(ip(i,1),ip(i,2),c2,c3)= r(i))

  else if( width(1).eq.9 .and. width(2).eq.9 )then

    loops2d($interp99(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),\
          ug(ip(i,1),ip(i,2),c2,c3)= r(i))
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
              r(i)=r(i)+c(i,w1,w2,0)*ui(il(i,1)+w1,il(i,2)+w2,c2,c3) 
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

    beginLoops3d()
      ! check for most common widths first
      if( varWidth(i).eq.3 )then
        interp333(r(i))
      else if( varWidth(i).eq.2 )then
        interp222(r(i))
      else if( varWidth(i).eq.1 )then
        interp111(r(i))
      else if( varWidth(i).eq.5 )then
        interp555(r(i))
      else if( varWidth(i).eq.4 )then
        interp444(r(i))
      else if( varWidth(i).eq.7 )then
        interp777(r(i))
      else if( varWidth(i).eq.6 )then
        interp666(r(i))
      else if( varWidth(i).eq.9 )then
        interp999(r(i))
      else if( varWidth(i).eq.8 )then
        interp888(r(i))
      else
        write(*,*) 'ERROR varWidth=',varWidth(i) 
        stop 151
      end if

      resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i)))
      ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i)
    endLoops3d()

  else if( width(1).eq.3 .and. width(2).eq.3 .and. width(3).eq.3 )then

    loops3d($interp333(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),\
          ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))


  else if( width(1).eq.1 .and. width(2).eq.1 .and. width(3).eq.1 )then
    loops3d($interp111(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),\
          ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))
      
  else if( width(1).eq.2 .and. width(2).eq.2 .and. width(3).eq.2 )then

    loops3d($interp222(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),\
          ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))

  else if( width(1).eq.4 .and. width(2).eq.4 .and. width(3).eq.4 )then

    loops3d($interp444(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),\
          ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))

  else if( width(1).eq.5 .and. width(2).eq.5 .and. width(3).eq.5 )then

    loops3d($interp555(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),\
          ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))

  else if( width(1).eq.6 .and. width(2).eq.6 .and. width(3).eq.6 )then

    loops3d($interp666(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),\
          ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))

  else if( width(1).eq.7 .and. width(2).eq.7 .and. width(3).eq.7 )then

    loops3d($interp777(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),\
          ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))

  else if( width(1).eq.8 .and. width(2).eq.8 .and. width(3).eq.8 )then

    loops3d($interp888(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),\
          ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))

  else if( width(1).eq.9 .and. width(2).eq.9 .and. width(3).eq.9 )then

    loops3d($interp999(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),\
          ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))

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
              r(i)=r(i)+c(i,w1,w2,w3)*ui(il(i,1)+w1,il(i,2)+w2,il(i,3)+w3,c3) 
            end do
          end do
        end do
      end do

      do i=nia,nib
        resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i)))
        ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i)                   
      end do

    end do
  end if

end if

#End
#If #STORAGE == "TP"

if( storageOption.eq.1 )then

c       ****************************************
c       **** tensor-product storage option *****
c       ****************************************

if( nd.eq.2 )then

  if( useVariableWidthInterpolation.ne.0 )then

    beginLoops2d()
      ! check for most common widths first
      if( varWidth(i).eq.3 )then
        interpTensorProduct33(r(i))
      else if( varWidth(i).eq.2 )then
        interpTensorProduct22(r(i))
      else if( varWidth(i).eq.1 )then
        interp11(r(i))
      else if( varWidth(i).eq.5 )then
        interpTensorProduct55(r(i))
      else if( varWidth(i).eq.4 )then
        interpTensorProduct44(r(i))
      else if( varWidth(i).eq.7 )then
        interpTensorProduct77(r(i))
      else if( varWidth(i).eq.6 )then
        interpTensorProduct66(r(i))
      else if( varWidth(i).eq.9 )then
        interpTensorProduct99(r(i))
      else if( varWidth(i).eq.8 )then
        interpTensorProduct88(r(i))
      else
        write(*,*) 'ERROR varWidth=',varWidth(i) 
        stop 151
      end if

      resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i)))
      ug(ip(i,1),ip(i,2),c2,c3)= r(i)
    endLoops2d()


  else if( width(1).eq.3 .and. width(2).eq.3 ) then

    loops2d($interpTensorProduct33(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),\
          ug(ip(i,1),ip(i,2),c2,c3)= r(i))

  else if( width(1).eq.1 .and. width(1).eq.1)then

    loops2d($interp11(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),\
          ug(ip(i,1),ip(i,2),c2,c3)= r(i))

  else if( width(1).eq.2 .and. width(2).eq.2 )then

    loops2d($interpTensorProduct22(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),\
          ug(ip(i,1),ip(i,2),c2,c3)= r(i))

  else if( width(1).eq.4 .and. width(2).eq.4 )then

    loops2d($interpTensorProduct44(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),\
          ug(ip(i,1),ip(i,2),c2,c3)= r(i))

  else if( width(1).eq.5 .and. width(2).eq.5 )then

    loops2d($interpTensorProduct55(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),\
          ug(ip(i,1),ip(i,2),c2,c3)= r(i))

  else if( width(1).eq.6 .and. width(2).eq.6 )then

    loops2d($interpTensorProduct66(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),\
          ug(ip(i,1),ip(i,2),c2,c3)= r(i))

  else if( width(1).eq.7 .and. width(2).eq.7 )then

    loops2d($interpTensorProduct77(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),\
          ug(ip(i,1),ip(i,2),c2,c3)= r(i))

  else if( width(1).eq.8 .and. width(2).eq.8 )then

    loops2d($interpTensorProduct88(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),\
          ug(ip(i,1),ip(i,2),c2,c3)= r(i))

  else if( width(1).eq.9 .and. width(2).eq.9 )then

    loops2d($interpTensorProduct99(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),\
          ug(ip(i,1),ip(i,2),c2,c3)= r(i))
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
              r(i)=r(i)+c(i,w1,w2,0)*ui(il(i,1)+w1,il(i,2)+w2,c2,c3) 
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

    beginLoops3d()
      ! check for most common widths first
      if( varWidth(i).eq.3 )then
        interpTensorProduct333(r(i))
      else if( varWidth(i).eq.2 )then
        interpTensorProduct222(r(i))
      else if( varWidth(i).eq.1 )then
        interp111(r(i))
      else if( varWidth(i).eq.5 )then
        interpTensorProduct555(r(i))
      else if( varWidth(i).eq.4 )then
        interpTensorProduct444(r(i))
      else if( varWidth(i).eq.7 )then
        interpTensorProduct777(r(i))
      else if( varWidth(i).eq.6 )then
        interpTensorProduct666(r(i))
      else if( varWidth(i).eq.9 )then
        interpTensorProduct999(r(i))
      else if( varWidth(i).eq.8 )then
        interpTensorProduct888(r(i))
      else
        write(*,*) 'ERROR varWidth=',varWidth(i) 
        stop 151
      end if

      resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i)))
      ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i)
    endLoops3d()

  else if( width(1).eq.3 .and. width(2).eq.3 .and. width(3).eq.3 )then

    loops3d($interpTensorProduct333(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),\
          ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))


  else if( width(1).eq.2 .and. width(2).eq.2 .and. width(3).eq.2 )then

    loops3d($interpTensorProduct222(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),\
          ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))

  else if( width(1).eq.4 .and. width(2).eq.4 .and. width(3).eq.4 )then

    loops3d($interpTensorProduct444(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),\
          ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))

  else if( width(1).eq.5 .and. width(2).eq.5 .and. width(3).eq.5 )then

    loops3d($interpTensorProduct555(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),\
          ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))

  else if( width(1).eq.6 .and. width(2).eq.6 .and. width(3).eq.6 )then

    loops3d($interpTensorProduct666(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),\
          ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))

  else if( width(1).eq.7 .and. width(2).eq.7 .and. width(3).eq.7 )then

    loops3d($interpTensorProduct777(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),\
          ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))

  else if( width(1).eq.8 .and. width(2).eq.8 .and. width(3).eq.8 )then

    loops3d($interpTensorProduct888(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),\
          ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))

  else if( width(1).eq.9 .and. width(2).eq.9 .and. width(3).eq.9 )then

    loops3d($interpTensorProduct999(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),\
          ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))

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
              r(i)=r(i)+c(i,w1,w2,w3)*ui(il(i,1)+w1,il(i,2)+w2,il(i,3)+w3,c3) 
            end do
          end do
        end do
      end do

      do i=nia,nib
        resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i)))
        ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i)                   
      end do

    end do
  end if

end if


#End
#If #STORAGE == "SP"
if( storageOption.eq.2 )then

c       ****************************************
c       **** sparse         storage option *****
c       ****************************************

if( nd.eq.2 )then

  if( useVariableWidthInterpolation.ne.0 )then

    beginLoops2d()
      ! check for most common widths first
      if( varWidth(i).eq.3 )then
        interpSparseStorage33(r(i))
      else if( varWidth(i).eq.2 )then
        interpSparseStorage22(r(i))
      else if( varWidth(i).eq.1 )then
        interp11(r(i))
      else if( varWidth(i).eq.5 )then
        interpSparseStorage55(r(i))
      else if( varWidth(i).eq.4 )then
        interpSparseStorage44(r(i))
      else if( varWidth(i).eq.7 )then
        interpSparseStorage77(r(i))
      else if( varWidth(i).eq.6 )then
        interpSparseStorage66(r(i))
      else if( varWidth(i).eq.9 )then
        interpSparseStorage99(r(i))
      else if( varWidth(i).eq.8 )then
        interpSparseStorage88(r(i))
      else
        write(*,*) 'ERROR varWidth=',varWidth(i) 
        stop 151
      end if

      resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i)))
      ug(ip(i,1),ip(i,2),c2,c3)= r(i)
    endLoops2d()

  else if( width(1).eq.3 .and. width(2).eq.3 ) then

    loops2d($interpSparseStorage33(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),\
          ug(ip(i,1),ip(i,2),c2,c3)= r(i))

  else if( width(1).eq.1 .and. width(1).eq.1)then

    loops2d($interp11(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),\
          ug(ip(i,1),ip(i,2),c2,c3)= r(i))

  else if( width(1).eq.2 .and. width(2).eq.2 )then

    loops2d($interpSparseStorage22(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),\
          ug(ip(i,1),ip(i,2),c2,c3)= r(i))

  else if( width(1).eq.4 .and. width(2).eq.4 )then

    loops2d($interpSparseStorage44(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),\
          ug(ip(i,1),ip(i,2),c2,c3)= r(i))

  else if( width(1).eq.5 .and. width(2).eq.5 )then

    loops2d($interpSparseStorage55(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),\
          ug(ip(i,1),ip(i,2),c2,c3)= r(i))

  else if( width(1).eq.6 .and. width(2).eq.6 )then

    loops2d($interpSparseStorage66(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),\
          ug(ip(i,1),ip(i,2),c2,c3)= r(i))

  else if( width(1).eq.7 .and. width(2).eq.7 )then

    loops2d($interpSparseStorage77(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),\
          ug(ip(i,1),ip(i,2),c2,c3)= r(i))

  else if( width(1).eq.8 .and. width(2).eq.8 )then

    loops2d($interpSparseStorage88(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),\
          ug(ip(i,1),ip(i,2),c2,c3)= r(i))

  else if( width(1).eq.9 .and. width(2).eq.9 )then

    loops2d($interpSparseStorage99(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),c2,c3)-r(i))),\
          ug(ip(i,1),ip(i,2),c2,c3)= r(i))
  else
    write(*,*) 'ERROR width=',width(1),width(2)
    stop 1
  end if
else
  !     *** 3D ****

  if( useVariableWidthInterpolation.ne.0 )then

    beginLoops3d()
      ! check for most common widths first
      if( varWidth(i).eq.3 )then
        interpSparseStorage333(r(i))
      else if( varWidth(i).eq.2 )then
        interpSparseStorage222(r(i))
      else if( varWidth(i).eq.1 )then
        interp111(r(i))
      else if( varWidth(i).eq.5 )then
        interpSparseStorage555(r(i))
      else if( varWidth(i).eq.4 )then
        interpSparseStorage444(r(i))
      else if( varWidth(i).eq.7 )then
        interpSparseStorage777(r(i))
      else if( varWidth(i).eq.6 )then
        interpSparseStorage666(r(i))
      else if( varWidth(i).eq.9 )then
        interpSparseStorage999(r(i))
      else if( varWidth(i).eq.8 )then
        interpSparseStorage888(r(i))
      else
        write(*,*) 'ERROR varWidth=',varWidth(i) 
        stop 151
      end if

      resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i)))
      ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i)
    endLoops3d()

  else if( width(1).eq.3 .and. width(2).eq.3 .and. width(3).eq.3 )then

    loops3d($interpSparseStorage333(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),\
          ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))


  else if( width(1).eq.2 .and. width(2).eq.2 .and. width(3).eq.2 )then

    loops3d($interpSparseStorage222(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),\
          ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))

  else if( width(1).eq.4 .and. width(2).eq.4 .and. width(3).eq.4 )then

    loops3d($interpSparseStorage444(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),\
          ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))

  else if( width(1).eq.5 .and. width(2).eq.5 .and. width(3).eq.5 )then

    loops3d($interpSparseStorage555(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),\
          ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))

  else if( width(1).eq.6 .and. width(2).eq.6 .and. width(3).eq.6 )then

    loops3d($interpSparseStorage666(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),\
          ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))

  else if( width(1).eq.7 .and. width(2).eq.7 .and. width(3).eq.7 )then

    loops3d($interpSparseStorage777(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),\
          ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))

  else if( width(1).eq.8 .and. width(2).eq.8 .and. width(3).eq.8 )then

    loops3d($interpSparseStorage888(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),\
          ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))

  else if( width(1).eq.9 .and. width(2).eq.9 .and. width(3).eq.9 )then

    loops3d($interpSparseStorage999(r(i)),\
          resMax=max(resMax,abs(ug(ip(i,1),ip(i,2),ip(i,3),c3)-r(i))),\
          ug(ip(i,1),ip(i,2),ip(i,3),c3)= r(i))

  else

   !   general case in 3D
    write(*,*) 'ERROR width=',width(1),width(2),width(2)
    stop 1

  end if

end if

#End

else
  write(*,*) 'interpOpt:ERROR; unknown storage option=',storageOption
end if ! end storage option


return
end
#endMacro

#beginMacro defineInterpOpt(STORAGE)

subroutine interpOpt ## STORAGE ( nd,\
 ndui1a,ndui1b,ndui2a,ndui2b,ndui3a,ndui3b,ndui4a,ndui4b,\
 ndug1a,ndug1b,ndug2a,ndug2b,ndug3a,ndug3b,ndug4a,ndug4b,\
 ndil,ndip,ndc1,ndc2,ndc3,\
   ipar,\
   ui,ug,c,il,ip,varWidth,width )
c=================================================================================
c  Optimised interpolation
c=================================================================================

implicit none

integer nd,nia,nib,c2a,c2b,c3a,c3b,ndil,ndip,ndc1,ndc2,ndc3
integer ndui1a,ndui1b,ndui2a,ndui2b,ndui3a,ndui3b,ndui4a,ndui4b,\
        ndug1a,ndug1b,ndug2a,ndug2b,ndug3a,ndug3b,ndug4a,ndug4b

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
      #Include "lagrangePolynomials.h"
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

#If #STORAGE == "Full"
 if( storageOption.eq.0 )then
c       ******************************
c       **** full storage option *****
c       ******************************

 if( nd.eq.2 )then
   if( useVariableWidthInterpolation.ne.0 )then

    beginLoops2d()
      ! check for most common widths first
      if( varWidth(i).eq.3 )then
        interp33(ug(ip(i,1),ip(i,2),c2,c3))
      else if( varWidth(i).eq.2 )then
        interp22(ug(ip(i,1),ip(i,2),c2,c3))
      else if( varWidth(i).eq.1 )then
        interp11(ug(ip(i,1),ip(i,2),c2,c3))
      else if( varWidth(i).eq.5 )then
        interp55(ug(ip(i,1),ip(i,2),c2,c3))
      else if( varWidth(i).eq.4 )then
        interp44(ug(ip(i,1),ip(i,2),c2,c3))
      else if( varWidth(i).eq.7 )then
        interp77(ug(ip(i,1),ip(i,2),c2,c3))
      else if( varWidth(i).eq.6 )then
        interp66(ug(ip(i,1),ip(i,2),c2,c3))
      else if( varWidth(i).eq.9 )then
        interp99(ug(ip(i,1),ip(i,2),c2,c3))
      else if( varWidth(i).eq.8 )then
        interp88(ug(ip(i,1),ip(i,2),c2,c3))
      else
        write(*,*) 'ERROR varWidth=',varWidth(i) 
        stop 151
      end if

    endLoops2d()

  else if( width(1).eq.3 .and. width(2).eq.3 ) then ! most common case

     loops2d($interp33(ug(ip(i,1),ip(i,2),c2,c3)),,)

   else if( width(1).eq.1 .and. width(1).eq.1)then

    loops2d($interp11(ug(ip(i,1),ip(i,2),c2,c3)),,)

   else if( width(1).eq.2 .and. width(2).eq.2 )then

     loops2d($interp22(ug(ip(i,1),ip(i,2),c2,c3)),,)

   else if( width(1).eq.4 .and. width(2).eq.4 )then

     loops2d($interp44(ug(ip(i,1),ip(i,2),c2,c3)),,)

   else if( width(1).eq.5 .and. width(2).eq.5 )then

     loops2d($interp55(ug(ip(i,1),ip(i,2),c2,c3)),,)

   else if( width(1).eq.6 .and. width(2).eq.6 )then

     loops2d($interp66(ug(ip(i,1),ip(i,2),c2,c3)),,)

   else if( width(1).eq.7 .and. width(2).eq.7 )then

     loops2d($interp77(ug(ip(i,1),ip(i,2),c2,c3)),,)

   else if( width(1).eq.8 .and. width(2).eq.8 )then

     loops2d($interp88(ug(ip(i,1),ip(i,2),c2,c3)),,)

   else if( width(1).eq.9 .and. width(2).eq.9 )then

     loops2d($interp99(ug(ip(i,1),ip(i,2),c2,c3)),,)

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
               ug(ip(i,1),ip(i,2),c2,c3)=ug(ip(i,1),ip(i,2),c2,c3)+c(i,w1,w2,0)*ui(il(i,1)+w1,il(i,2)+w2,c2,c3) 
             end do
           end do
         end do
       end do
     end do

   end if
 else
c     *** 3D ****
 
  if( useVariableWidthInterpolation.ne.0 )then

    beginLoops3d()
      ! check for most common widths first
      if( varWidth(i).eq.3 )then
        interp333(ug(ip(i,1),ip(i,2),ip(i,3),c3))
      else if( varWidth(i).eq.2 )then
        interp222(ug(ip(i,1),ip(i,2),ip(i,3),c3))
      else if( varWidth(i).eq.1 )then
        interp111(ug(ip(i,1),ip(i,2),ip(i,3),c3))
      else if( varWidth(i).eq.5 )then
        interp555(ug(ip(i,1),ip(i,2),ip(i,3),c3))
      else if( varWidth(i).eq.4 )then
        interp444(ug(ip(i,1),ip(i,2),ip(i,3),c3))
      else if( varWidth(i).eq.7 )then
        interp777(ug(ip(i,1),ip(i,2),ip(i,3),c3))
      else if( varWidth(i).eq.6 )then
        interp666(ug(ip(i,1),ip(i,2),ip(i,3),c3))
      else if( varWidth(i).eq.9 )then
        interp999(ug(ip(i,1),ip(i,2),ip(i,3),c3))
      else if( varWidth(i).eq.8 )then
        interp888(ug(ip(i,1),ip(i,2),ip(i,3),c3))
      else
        write(*,*) 'ERROR varWidth=',varWidth(i) 
        stop 151
      end if

    endLoops3d()

  else if( width(1).eq.3 .and. width(2).eq.3 .and. width(3).eq.3 )then

     loops3d($interp333(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)

  else if( width(1).eq.1 .and. width(2).eq.1 .and. width(3).eq.1 )then
    loops3d($interp111(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)

   else if( width(1).eq.2 .and. width(2).eq.2 .and. width(3).eq.2 )then

     loops3d($interp222(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)

   else if( width(1).eq.4 .and. width(2).eq.4 .and. width(3).eq.4 )then

     loops3d($interp444(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)

   else if( width(1).eq.5 .and. width(2).eq.5 .and. width(3).eq.5 )then

     ! write(*,*) 'interpOpt explicit interp width=5'
     loops3d($interp555(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)

   else if( width(1).eq.6 .and. width(2).eq.6 .and. width(3).eq.6 )then

     loops3d($interp666(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)

   else if( width(1).eq.7 .and. width(2).eq.7 .and. width(3).eq.7 )then

     loops3d($interp777(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)

   else if( width(1).eq.8 .and. width(2).eq.8 .and. width(3).eq.8 )then

     loops3d($interp888(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)

   else if( width(1).eq.9 .and. width(2).eq.9 .and. width(3).eq.9 )then

     loops3d($interp999(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)

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
               ug(ip(i,1),ip(i,2),ip(i,3),c3)=\
                 ug(ip(i,1),ip(i,2),ip(i,3),c3)+c(i,w1,w2,w3)*ui(il(i,1)+w1,il(i,2)+w2,il(i,3)+w3,c3) 
             end do
           end do
         end do
       end do
     end do
   end if

 end if
#End

c **      else if( storageOption.eq.1 )then
#If #STORAGE == "TP"
 if( storageOption.eq.1 )then

c       ****************************************
c       **** tensor-product storage option *****
c       ****************************************

! write(*,*) 'interpOpt:tensorProduct interp, width=',width(1)

if( nd.eq.2 )then

  if( useVariableWidthInterpolation.ne.0 )then

    beginLoops2d()
      ! check for most common widths first
      if( varWidth(i).eq.3 )then
        interpTensorProduct33(ug(ip(i,1),ip(i,2),c2,c3))
      else if( varWidth(i).eq.2 )then
        interpTensorProduct22(ug(ip(i,1),ip(i,2),c2,c3))
      else if( varWidth(i).eq.1 )then
        interp11(ug(ip(i,1),ip(i,2),c2,c3))
      else if( varWidth(i).eq.5 )then
        interpTensorProduct55(ug(ip(i,1),ip(i,2),c2,c3))
      else if( varWidth(i).eq.4 )then
        interpTensorProduct44(ug(ip(i,1),ip(i,2),c2,c3))
      else if( varWidth(i).eq.7 )then
        interpTensorProduct77(ug(ip(i,1),ip(i,2),c2,c3))
      else if( varWidth(i).eq.6 )then
        interpTensorProduct66(ug(ip(i,1),ip(i,2),c2,c3))
      else if( varWidth(i).eq.9 )then
        interpTensorProduct99(ug(ip(i,1),ip(i,2),c2,c3))
      else if( varWidth(i).eq.8 )then
        interpTensorProduct88(ug(ip(i,1),ip(i,2),c2,c3))
      else
        write(*,*) 'ERROR varWidth=',varWidth(i) 
        stop 151
      end if

    endLoops2d()


  else if( width(1).eq.3 .and. width(2).eq.3 ) then ! most common case

    loops2d($interpTensorProduct33(ug(ip(i,1),ip(i,2),c2,c3)),,)

  else if( width(1).eq.1 .and. width(1).eq.1)then

    loops2d($interp11(ug(ip(i,1),ip(i,2),c2,c3)),,)

  else if( width(1).eq.2 .and. width(2).eq.2 )then

    loops2d($interpTensorProduct22(ug(ip(i,1),ip(i,2),c2,c3)),,)

  else if( width(1).eq.4 .and. width(2).eq.4 )then

    loops2d($interpTensorProduct44(ug(ip(i,1),ip(i,2),c2,c3)),,)

  else if( width(1).eq.5 .and. width(2).eq.5 )then

    loops2d($interpTensorProduct55(ug(ip(i,1),ip(i,2),c2,c3)),,)

  else if( width(1).eq.6 .and. width(2).eq.6 )then

    loops2d($interpTensorProduct66(ug(ip(i,1),ip(i,2),c2,c3)),,)

  else if( width(1).eq.7 .and. width(2).eq.7 )then

    loops2d($interpTensorProduct77(ug(ip(i,1),ip(i,2),c2,c3)),,)

  else if( width(1).eq.8 .and. width(2).eq.8 )then

    loops2d($interpTensorProduct88(ug(ip(i,1),ip(i,2),c2,c3)),,)

  else if( width(1).eq.9 .and. width(2).eq.9 )then

    loops2d($interpTensorProduct99(ug(ip(i,1),ip(i,2),c2,c3)),,)

  else
c           general case in 2D ****fix this*****
c write(*,*)'interpOpt:WARNING:Gen case width=',width(1),width(2)
           stop 2
  end if
else
c     *** 3D ****
 
  if( useVariableWidthInterpolation.ne.0 )then

    beginLoops3d()
      ! check for most common widths first
      if( varWidth(i).eq.3 )then
        interpTensorProduct333(ug(ip(i,1),ip(i,2),ip(i,3),c3))
      else if( varWidth(i).eq.2 )then
        interpTensorProduct222(ug(ip(i,1),ip(i,2),ip(i,3),c3))
      else if( varWidth(i).eq.1 )then
        interp111(ug(ip(i,1),ip(i,2),ip(i,3),c3))
      else if( varWidth(i).eq.5 )then
        interpTensorProduct555(ug(ip(i,1),ip(i,2),ip(i,3),c3))
      else if( varWidth(i).eq.4 )then
        interpTensorProduct444(ug(ip(i,1),ip(i,2),ip(i,3),c3))
      else if( varWidth(i).eq.7 )then
        interpTensorProduct777(ug(ip(i,1),ip(i,2),ip(i,3),c3))
      else if( varWidth(i).eq.6 )then
        interpTensorProduct666(ug(ip(i,1),ip(i,2),ip(i,3),c3))
      else if( varWidth(i).eq.9 )then
        interpTensorProduct999(ug(ip(i,1),ip(i,2),ip(i,3),c3))
      else if( varWidth(i).eq.8 )then
        interpTensorProduct888(ug(ip(i,1),ip(i,2),ip(i,3),c3))
      else
        write(*,*) 'ERROR varWidth=',varWidth(i) 
        stop 151
      end if

    endLoops3d()

  else if( width(1).eq.3 .and. width(2).eq.3 .and. width(3).eq.3 )then

     loops3d($interpTensorProduct333(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)

  else if( width(1).eq.1 .and. width(2).eq.1 .and. width(3).eq.1 )then

    loops3d($interp111(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)

   else if( width(1).eq.2 .and. width(2).eq.2 .and. width(3).eq.2 )then

     loops3d($interpTensorProduct222(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)

   else if( width(1).eq.4 .and. width(2).eq.4 .and. width(3).eq.4 )then

     loops3d($interpTensorProduct444(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)

   else if( width(1).eq.5 .and. width(2).eq.5 .and. width(3).eq.5 )then

     ! write(*,*) 'interpOpt explicit interp width=5'
     loops3d($interpTensorProduct555(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)

   else if( width(1).eq.6 .and. width(2).eq.6 .and. width(3).eq.6 )then

     loops3d($interpTensorProduct666(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)

   else if( width(1).eq.7 .and. width(2).eq.7 .and. width(3).eq.7 )then

     loops3d($interpTensorProduct777(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)

   else if( width(1).eq.8 .and. width(2).eq.8 .and. width(3).eq.8 )then

     loops3d($interpTensorProduct888(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)

   else if( width(1).eq.9 .and. width(2).eq.9 .and. width(3).eq.9 )then

     loops3d($interpTensorProduct999(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)

   else

     ! general case width's in 3D **** fix this *********
     stop 5

   end if ! end width

 end if ! end nd

c**      else if( storageOption.eq.2 )then

#End

#If #STORAGE == "SP"
if( storageOption.eq.2 )then

c       ****************************************
c       **** sparse         storage option *****
c       ****************************************

! write(*,*) 'interpOpt:sparseStorage interp, width=',width(1)

if( nd.eq.2 )then

  if( useVariableWidthInterpolation.ne.0 )then

    beginLoops2d()
      ! check for most common widths first
      if( varWidth(i).eq.3 )then
        interpSparseStorage33(ug(ip(i,1),ip(i,2),c2,c3))
      else if( varWidth(i).eq.2 )then
        interpSparseStorage22(ug(ip(i,1),ip(i,2),c2,c3))
      else if( varWidth(i).eq.1 )then
        interp11(ug(ip(i,1),ip(i,2),c2,c3))
      else if( varWidth(i).eq.5 )then
        interpSparseStorage55(ug(ip(i,1),ip(i,2),c2,c3))
      else if( varWidth(i).eq.4 )then
        interpSparseStorage44(ug(ip(i,1),ip(i,2),c2,c3))
      else if( varWidth(i).eq.7 )then
        interpSparseStorage77(ug(ip(i,1),ip(i,2),c2,c3))
      else if( varWidth(i).eq.6 )then
        interpSparseStorage66(ug(ip(i,1),ip(i,2),c2,c3))
      else if( varWidth(i).eq.9 )then
        interpSparseStorage99(ug(ip(i,1),ip(i,2),c2,c3))
      else if( varWidth(i).eq.8 )then
        interpSparseStorage88(ug(ip(i,1),ip(i,2),c2,c3))
      else
        write(*,*) 'ERROR varWidth=',varWidth(i) 
        stop 151
      end if

    endLoops2d()

  else if( width(1).eq.3 .and. width(2).eq.3 ) then ! most common case

    loops2d($interpSparseStorage33(ug(ip(i,1),ip(i,2),c2,c3)),,)

  else if( width(1).eq.1 .and. width(1).eq.1)then

    loops2d($interp11(ug(ip(i,1),ip(i,2),c2,c3)),,)

  else if( width(1).eq.2 .and. width(2).eq.2 )then

    loops2d($interpSparseStorage22(ug(ip(i,1),ip(i,2),c2,c3)),,)

  else if( width(1).eq.4 .and. width(2).eq.4 )then

    loops2d($interpSparseStorage44(ug(ip(i,1),ip(i,2),c2,c3)),,)

  else if( width(1).eq.5 .and. width(2).eq.5 )then

    loops2d($interpSparseStorage55(ug(ip(i,1),ip(i,2),c2,c3)),,)

  else if( width(1).eq.6 .and. width(2).eq.6 )then

    loops2d($interpSparseStorage66(ug(ip(i,1),ip(i,2),c2,c3)),,)

  else if( width(1).eq.7 .and. width(2).eq.7 )then

    loops2d($interpSparseStorage77(ug(ip(i,1),ip(i,2),c2,c3)),,)

  else if( width(1).eq.8 .and. width(2).eq.8 )then

    loops2d($interpSparseStorage88(ug(ip(i,1),ip(i,2),c2,c3)),,)

  else if( width(1).eq.9 .and. width(2).eq.9 )then

    loops2d($interpSparseStorage99(ug(ip(i,1),ip(i,2),c2,c3)),,)

  else
c           general case in 2D ********************** fix this ***********************
c write(*,*)'interpOpt:WARNING:Gen case width=',width(1),width(2)
           stop 2
 
  end if
else
c     *** 3D ****
 
  if( useVariableWidthInterpolation.ne.0 )then

    beginLoops3d()
      ! check for most common widths first
      if( varWidth(i).eq.3 )then
        interpSparseStorage333(ug(ip(i,1),ip(i,2),ip(i,3),c3))
      else if( varWidth(i).eq.2 )then
        interpSparseStorage222(ug(ip(i,1),ip(i,2),ip(i,3),c3))
      else if( varWidth(i).eq.1 )then
        interp111(ug(ip(i,1),ip(i,2),ip(i,3),c3))
      else if( varWidth(i).eq.5 )then
        interpSparseStorage555(ug(ip(i,1),ip(i,2),ip(i,3),c3))
      else if( varWidth(i).eq.4 )then
        interpSparseStorage444(ug(ip(i,1),ip(i,2),ip(i,3),c3))
      else if( varWidth(i).eq.7 )then
        interpSparseStorage777(ug(ip(i,1),ip(i,2),ip(i,3),c3))
      else if( varWidth(i).eq.6 )then
        interpSparseStorage666(ug(ip(i,1),ip(i,2),ip(i,3),c3))
      else if( varWidth(i).eq.9 )then
        interpSparseStorage999(ug(ip(i,1),ip(i,2),ip(i,3),c3))
      else if( varWidth(i).eq.8 )then
        interpSparseStorage888(ug(ip(i,1),ip(i,2),ip(i,3),c3))
      else
        write(*,*) 'ERROR varWidth=',varWidth(i) 
        stop 151
      end if

    endLoops3d()

  else if( width(1).eq.3 .and. width(2).eq.3 .and. width(3).eq.3 )then

    loops3d($interpSparseStorage333(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)

  else if( width(1).eq.1 .and. width(2).eq.1 .and. width(3).eq.1 )then

    loops3d($interp111(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)

  else if( width(1).eq.2 .and. width(2).eq.2 .and. width(3).eq.2 )then

    loops3d($interpSparseStorage222(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)

  else if( width(1).eq.4 .and. width(2).eq.4 .and. width(3).eq.4 )then

    loops3d($interpSparseStorage444(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)

  else if( width(1).eq.5 .and. width(2).eq.5 .and. width(3).eq.5 )then

    ! write(*,*) 'interpOpt explicit interp width=5'
    loops3d($interpSparseStorage555(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)

  else if( width(1).eq.6 .and. width(2).eq.6 .and. width(3).eq.6 )then

    loops3d($interpSparseStorage666(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)

  else if( width(1).eq.7 .and. width(2).eq.7 .and. width(3).eq.7 )then

    loops3d($interpSparseStorage777(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)

  else if( width(1).eq.8 .and. width(2).eq.8 .and. width(3).eq.8 )then

    loops3d($interpSparseStorage888(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)

  else if( width(1).eq.9 .and. width(2).eq.9 .and. width(3).eq.9 )then

    loops3d($interpSparseStorage999(ug(ip(i,1),ip(i,2),ip(i,3),c3)),,)

  else
 
    ! general case in 3D ********************** fix this ***********************

   stop 7
   end if ! width

 end if ! nd

#End

else
  write(*,*) 'interpOpt:ERROR; unknown storage option=',storageOption
  stop 3
end if ! end storage option
return
end

#endMacro



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
        write(*,*) 'interpOptRes:ERROR; unknown storage option=',storageOption
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
        write(*,*) 'interpOpt:ERROR; unknown storage option=',storageOption
        stop 3
      end if ! end storage option
      return
      end


#beginMacro buildFile(x)
#beginFile interpOpt ## x ## .f
 defineInterpOptRes(x)
 defineInterpOpt(x)
#endFile
#endMacro


c  We need to save these in separate files for the dec compiler

      buildFile(Full)
      buildFile(TP)
      buildFile(SP)
         

c use the mask 
#beginMacro beginLoops(nn1a,nn1b,nn2a,nn2b,nn3a,nn3b)
do i3=nn3a,nn3b
do i2=nn2a,nn2b
do i1=nn1a,nn1b
#endMacro

#beginMacro endLoops()
end do
end do
end do
#endMacro

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


      beginLoops(nd1a,nd1b,nd2a,nd2b,nd3a,nd3b)
        if( mask(i1,i2,i3).eq.0 )then
          do n=nMin,nMax
            u(i1,i2,i3,n)=val(n)
          end do
        end if
      endLoops()

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

          beginLoops(m1a,m1b,m2a,m2b,m3a,m3b)
            do n=nMin,nMax
              u(i1,i2,i3,n)=val(n)
            end do
          endLoops()

        end if
      end do
      end do

      return 
      end

