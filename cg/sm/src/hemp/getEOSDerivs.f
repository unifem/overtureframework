      subroutine getEOSDerivs( rho,rho0,p,e,
     *           a,b,c,d,
     *           p_r,p_re,e_r,e_p )
      implicit none
      real rho,rho0,p,e,a,b,c,d
      real p_r,p_re,e_r,e_p
      real tmp1,eta

      eta = rho/rho0
      tmp1 = 1.0/rho0*(a+2.0*b*(eta-1)-3.0*c*(eta-1)**2)

      p_r  = tmp1
      p_re = d/rho0
      e_r  = 1.0/rho*(-e+rho0*(-tmp1)/d)
      e_p = rho0/(d*rho)
      
      return
      end
