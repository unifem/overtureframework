      real*4 function r1mach(i)
      integer i
      call r1machc(i,r1mach)
      return 
      end

      real*8 function d1mach(i)
      integer i
      call d1machc(i,d1mach)
      return 
      end

      integer*4 function i1mach(i)
      integer i
      call i1machc(i,i1mach)
      return 
      end

