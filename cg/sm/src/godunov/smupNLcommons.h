      integer method,iorder,icart,ilimit,itz,ifrc
      integer itype
      real mu,lambda,rho0
      real*8 eptz
      
      common / smupNLdat / mu,lambda,rho0
      common / smupNLflags / method,iorder,icart,ilimit,ifrc
      common / NLModelFlags / itype
      common / smupNLtz / eptz,itz
