      subroutine getPress( eta,e,p,a,b,c,d )
      implicit none
      real eta,e,p,a,b,c,d

      ! ideal gas gamma=1.4
      !p = eta*e*0.4e0

      p = d*eta*e+(eta-1.d0)*(a+(eta-1.d0)*(b+(eta-1.d0)*c))
      
      return
      end
