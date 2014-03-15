subroutine test2( )

c  define a macro
#beginMacro loops(arg)
do c=ca,cb
  do i3=n3a,n3b
    do i2=n2a,n2b
      do i1=n1a,n1b
        arg
      end do
    end do
  end do
end do
#endMacro

#beginMacro cases(name)
if( nd.eq.2 )then
  if( order.eq.2 )then
    loops(deriv(i1,i2,i3,c)=s(i1,i2,i3)*name 22R(i1,i2,i3,c) \
                           +SX22R(i1,i2,i3)*UX22R(i1,i2,i3,c))
  else
    loops(deriv(i1,i2,i3,c)=b(i1,i2,i3)*name 42R(i1,i2,i3,c)+SX42R(i1,i2,i3)*UX42R(i1,i2,i3,c))
end if
#endMacro

c NOTES: use indentation from the line below -- split lines if too long.

  cases(LAPLACIAN)

  ! here is some normal code
  if( a.eq.b )then
    do j=1,2
      x(i)=y(j)
    end do
  end if
return 
end
