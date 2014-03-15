      program tf
      integer i,j,k
      integer*4 i4
      real x,y,z
      real*4 r1mach
      real*8 d1mach

      x=1.
      write(*,*) 'x/3.=',x/3.
      x=1./3.
      write(*,*) 'x=1./3.=',x

      write(*,*) 'sizeof(x)=', sizeof(x), ' sizeof(i)=',sizeof(i)

      write(*,*) 'r1mach(3)=', r1mach(3), ' d1mach(3)=',d1mach(3)
      write(*,*) 'r1mach(4)=', r1mach(4), ' d1mach(4)=',d1mach(4)
    
      stop
      end
