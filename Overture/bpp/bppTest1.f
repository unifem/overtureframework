! This file automatically generated from bppTest1.bf with bpp.
  implicit none
  emplicit none

subroutine test1( )





! cases(a(i)=1.)
  if( nd.eq.2 )then
! loops(a(i)=1.)
     do i=1,10
       a(i)=1.
     end do
  end if


! cases(b(i)=1.)
  if( nd.eq.2 )then
! loops(b(i)=1.)
     do j=6,7
       b(i)=1.
     end do
  end if

return
end
