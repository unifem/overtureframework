! This file automatically generated from bppTest2.bf with bpp.
subroutine test2( )

c  define a macro


c NOTES: use indentation from the line below -- split lines if too long.

! cases(LAPLACIAN)
  if( nd.eq.2 )then
    if( order.eq.2 )then
! loops(deriv(i1,i2,i3,c)=s(i1,i2,i3)*LAPLACIAN 22R(i1,i2,i3,c) +SX22R(i1,i2,i3)*UX22R(i1,i2,i3,c))
      do c=ca,cb
        do i3=n3a,n3b
          do i2=n2a,n2b
            do i1=n1a,n1b
              deriv(i1,i2,i3,c)=s(i1,i2,i3)*LAPLACIAN22R(i1,i2,i3,c)+
     & SX22R(i1,i2,i3)*UX22R(i1,i2,i3,c)
            end do
          end do
        end do
      end do
    else
! loops(deriv(i1,i2,i3,c)=b(i1,i2,i3)*LAPLACIAN 42R(i1,i2,i3,c)+SX42R(i1,i2,i3)*UX42R(i1,i2,i3,c))
      do c=ca,cb
        do i3=n3a,n3b
          do i2=n2a,n2b
            do i1=n1a,n1b
              deriv(i1,i2,i3,c)=b(i1,i2,i3)*LAPLACIAN42R(i1,i2,i3,c)+
     & SX42R(i1,i2,i3)*UX42R(i1,i2,i3,c)
            end do
          end do
        end do
      end do
  end if

  ! here is some normal code
  if( a.eq.b )then
    do j=1,2
      x(i)=y(j)
    end do
  end if
return
end
