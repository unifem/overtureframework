      subroutine getEng( eta,p,e,a,b,c,d )
      implicit none
      real eta,e,p,a,b,c,d

      ! ideal gas gamma=1.4
      !e = p/(0.4e0*eta)

      e = (p-(eta-1.d0)*(a+(eta-1.d0)*(b+(eta-1.d0)*c)))/(d*eta)
      
      return
      end
