      subroutine timef( n,x,y,z )
      real x(n), y(n), z(n)
      
      do i=1,n
        z(i)=x(i)+y(i)
      end do
      return
      end      
