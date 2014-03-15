  implicit none
  emplicit none

subroutine test1( )

#beginMacro cases(name)
if( nd.eq.2 )then
end if
#endMacro


#beginMacro loops(arg)
  do i=1,10
    arg
  end do
#endMacro

#beginMacro cases(name)
if( nd.eq.2 )then
 loops(name)
end if
#endMacro

  cases(a(i)=1.)

#beginMacro loops(arg)
  do j=6,7
    arg
  end do
#endMacro

  cases(b(i)=1.)

return 
end
