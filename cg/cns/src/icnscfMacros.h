#beginMacro UXC(X,FF,CC)
c     coefficients for a  difference approximation to FF * u[CC]_X
c     r-part
      coeff(icf(+1,0,0,eqn,CC),i1,i2,i3) = coeff(icf(+1,0,0,eqn,CC),i1,i2,i3) + (FF)*rx(i1+off(1),i2+off(2),i3+off(3),1,X)*dri2(1)
      coeff(icf(-1,0,0,eqn,CC),i1,i2,i3) = coeff(icf(-1,0,0,eqn,CC),i1,i2,i3) - (FF)*rx(i1+off(1),i2+off(2),i3+off(3),1,X)*dri2(1)
c     s-part
      coeff(icf(0,+1,0,eqn,CC),i1,i2,i3) = coeff(icf(0,+1,0,eqn,CC),i1,i2,i3) + (FF)*rx(i1+off(1),i2+off(2),i3+off(3),2,X)*dri2(2)
      coeff(icf(0,-1,0,eqn,CC),i1,i2,i3) = coeff(icf(0,-1,0,eqn,CC),i1,i2,i3) - (FF)*rx(i1+off(1),i2+off(2),i3+off(3),2,X)*dri2(2)

#endMacro

#beginMacro UXYC(X,Y,FF,CC)
c     9 point curvilinear grid second derivative stencil:  FF * u[CC]_{XY}
            coeff(icf(-1,-1,0,eqn,CC),i1,i2,i3) =  coeff(icf(-1,-1,0,eqn,CC),i1,i2,i3) + ( FF)*(rx(i1+off(1),i2+off(2),i3+off(3),1,X)*rx(i1+off(1),i2+off(2),i3+off(3),2,Y) +  rx(i1+off(1),i2+off(2),i3+off(3),1,Y)*rx(i1+off(1),i2+off(2),i3+off(3),2,X))*dri2(1)*dri2(2)
            coeff(icf( 0,-1,0,eqn,CC),i1,i2,i3) =  coeff(icf( 0,-1,0,eqn,CC),i1,i2,i3) + ( FF)*(rx(i1+off(1),i2+off(2),i3+off(3),2,X)*rx(i1+off(1),i2+off(2),i3+off(3),2,Y)*dri(2)*dri(2) - rxx(i1,i2,i3,2,X,Y)*dri2(2))

            coeff(icf(+1,-1,0,eqn,CC),i1,i2,i3) =  coeff(icf(+1,-1,0,eqn,CC),i1,i2,i3) + (-FF)*(rx(i1+off(1),i2+off(2),i3+off(3),1,X)*rx(i1+off(1),i2+off(2),i3+off(3),2,Y) +  rx(i1+off(1),i2+off(2),i3+off(3),1,Y)*rx(i1+off(1),i2+off(2),i3+off(3),2,X))*dri2(1)*dri2(2)

            coeff(icf(-1, 0,0,eqn,CC),i1,i2,i3) =  coeff(icf(-1, 0,0,eqn,CC),i1,i2,i3) + ( FF)*(rx(i1+off(1),i2+off(2),i3+off(3),1,X)*rx(i1+off(1),i2+off(2),i3+off(3),1,Y)*dri(1)*dri(1) - rxx(i1,i2,i3,1,X,Y)*dri2(1))

            coeff(icf( 0, 0,0,eqn,CC),i1,i2,i3) =  coeff(icf( 0, 0,0,eqn,CC),i1,i2,i3) + (-2d0*FF)*(rx(i1+off(1),i2+off(2),i3+off(3),1,X)*rx(i1+off(1),i2+off(2),i3+off(3),1,Y)*dri(1)*dri(1) + rx(i1+off(1),i2+off(2),i3+off(3),2,X)*rx(i1+off(1),i2+off(2),i3+off(3),2,Y)*dri(2)*dri(2))

            coeff(icf(+1, 0,0,eqn,CC),i1,i2,i3) =  coeff(icf(+1, 0,0,eqn,CC),i1,i2,i3) + ( FF)*(rx(i1+off(1),i2+off(2),i3+off(3),1,X)*rx(i1+off(1),i2+off(2),i3+off(3),1,Y)*dri(1)*dri(1) + rxx(i1,i2,i3,1,X,Y)*dri2(1))

            coeff(icf(-1,+1,0,eqn,CC),i1,i2,i3) =  coeff(icf(-1,+1,0,eqn,CC),i1,i2,i3) + (-FF)*(rx(i1+off(1),i2+off(2),i3+off(3),1,X)*rx(i1+off(1),i2+off(2),i3+off(3),2,Y) + rx(i1+off(1),i2+off(2),i3+off(3),1,Y)*rx(i1+off(1),i2+off(2),i3+off(3),2,X))*dri2(1)*dri2(2)

            coeff(icf( 0,+1,0,eqn,CC),i1,i2,i3) =  coeff(icf( 0,+1,0,eqn,CC),i1,i2,i3) + ( FF)*(rx(i1+off(1),i2+off(2),i3+off(3),2,X)*rx(i1+off(1),i2+off(2),i3+off(3),2,Y)*dri(2)*dri(2) + rxx(i1,i2,i3,2,X,Y)*dri2(2))

            coeff(icf(+1,+1,0,eqn,CC),i1,i2,i3) =  coeff(icf(+1,+1,0,eqn,CC),i1,i2,i3) + ( FF)*(rx(i1+off(1),i2+off(2),i3+off(3),1,X)*rx(i1+off(1),i2+off(2),i3+off(3),2,Y) + rx(i1+off(1),i2+off(2),i3+off(3),1,Y)*rx(i1+off(1),i2+off(2),i3+off(3),2,X))*dri2(1)*dri2(2)

#endMacro

#beginMacro UUXC(X,FF,UC,CC)
c     coefficients for a central difference approximation to FF * u[UC] u[CC]_X

c                 u[UC]^{n+1} * u[CC]^n_X
                  coeff(icf(0,0,0,eqn,UC),i1,i2,i3) = coeff(icf(0,0,0,eqn,UC),i1,i2,i3) + (FF) * ux(i1,i2,i3,CC,X)

c                 u[UC]^n * (r_X u[CC]_r^{n+1} + s_X u[CC]_s^{n+1} )
c                 r - part
                  coeff(icf(+1,0,0,eqn,CC),i1,i2,i3) =  coeff(icf(+1,0,0,eqn,CC),i1,i2,i3) + (FF) * rx(i1+off(1),i2+off(2),i3+off(3),1,X)*u(i1+off(1),i2+off(2),i3+off(3),UC)*dri2(1)
                  coeff(icf(-1,0,0,eqn,CC),i1,i2,i3) =  coeff(icf(-1,0,0,eqn,CC),i1,i2,i3) - (FF) * rx(i1+off(1),i2+off(2),i3+off(3),1,X)*u(i1+off(1),i2+off(2),i3+off(3),UC)*dri2(1)

c                 s - part
                  coeff(icf(0,+1,0,eqn,CC),i1,i2,i3) =  coeff(icf(0,+1,0,eqn,CC),i1,i2,i3) + (FF) * rx(i1+off(1),i2+off(2),i3+off(3),2,X)*u(i1+off(1),i2+off(2),i3+off(3),UC)*dri2(2)
                  coeff(icf(0,-1,0,eqn,CC),i1,i2,i3) =  coeff(icf(0,-1,0,eqn,CC),i1,i2,i3) - (FF) * rx(i1+off(1),i2+off(2),i3+off(3),2,X)*u(i1+off(1),i2+off(2),i3+off(3),UC)*dri2(2)

#endMacro

#beginMacro UX4PC(X,FF,CC)
c     coefficients for a 3rd order  difference approximation to FF * u[CC]_X
c                  (r_X u[CC]_r^{n+1} + s_X u[CC]_s^{n+1} )
c                 r - part
      coeff(icf(+1,0,0,eqn,CC),i1,i2,i3) =  coeff(icf(+1,0,0,eqn,CC),i1,i2,i3) - (FF) * (alpha-.5d0)    *rx(i1+off(1),i2+off(2),i3+off(3),1,X)*dri(1)
      coeff(icf( 0,0,0,eqn,CC),i1,i2,i3) =  coeff(icf( 0,0,0,eqn,CC),i1,i2,i3) + (FF) * (3d0*alpha)     *rx(i1+off(1),i2+off(2),i3+off(3),1,X)*dri(1)
      coeff(icf(-1,0,0,eqn,CC),i1,i2,i3) =  coeff(icf(-1,0,0,eqn,CC),i1,i2,i3) - (FF) * (.5d0+3d0*alpha)*rx(i1+off(1),i2+off(2),i3+off(3),1,X)*dri(1)
      coeff(icf(-2,0,0,eqn,CC),i1,i2,i3) =  coeff(icf(-2,0,0,eqn,CC),i1,i2,i3) + (FF) * (alpha)         *rx(i1+off(1),i2+off(2),i3+off(3),1,X)*dri(1)

c                 s - part
      coeff(icf(0,+1,0,eqn,CC),i1,i2,i3) =  coeff(icf(0,+1,0,eqn,CC),i1,i2,i3) - (FF) * (alpha-.5d0)    *rx(i1+off(1),i2+off(2),i3+off(3),2,X)*dri(2)
      coeff(icf( 0,0,0,eqn,CC),i1,i2,i3) =  coeff(icf(0,0,0,eqn,CC),i1,i2,i3)  + (FF) * (3d0*alpha)     *rx(i1+off(1),i2+off(2),i3+off(3),2,X)*dri(2)
      coeff(icf(0,-1,0,eqn,CC),i1,i2,i3) =  coeff(icf(0,-1,0,eqn,CC),i1,i2,i3) - (FF) * (.5d0+3d0*alpha)*rx(i1+off(1),i2+off(2),i3+off(3),2,X)*dri(2)
      coeff(icf(0,-2,0,eqn,CC),i1,i2,i3) =  coeff(icf(0,-2,0,eqn,CC),i1,i2,i3) + (FF) * (alpha)         *rx(i1+off(1),i2+off(2),i3+off(3),2,X)*dri(2)
#endMacro

#beginMacro UX4MC(X,FF,CC)
c     coefficients for a 3rd order  difference approximation to FF * u[CC]_X
c                  (r_X u[CC]_r^{n+1} + s_X u[CC]_s^{n+1} )
c                 r - part
      coeff(icf(-1,0,0,eqn,CC),i1,i2,i3) =  coeff(icf(-1,0,0,eqn,CC),i1,i2,i3) + (FF) * (alpha-.5d0)    *rx(i1+off(1),i2+off(2),i3+off(3),1,X)*dri(1)
      coeff(icf( 0,0,0,eqn,CC),i1,i2,i3) =  coeff(icf( 0,0,0,eqn,CC),i1,i2,i3) - (FF) * (3d0*alpha)     *rx(i1+off(1),i2+off(2),i3+off(3),1,X)*dri(1)
      coeff(icf(+1,0,0,eqn,CC),i1,i2,i3) =  coeff(icf(+1,0,0,eqn,CC),i1,i2,i3) + (FF) * (.5d0+3d0*alpha)*rx(i1+off(1),i2+off(2),i3+off(3),1,X)*dri(1)
      coeff(icf(+2,0,0,eqn,CC),i1,i2,i3) =  coeff(icf(+2,0,0,eqn,CC),i1,i2,i3) - (FF) * (alpha)         *rx(i1+off(1),i2+off(2),i3+off(3),1,X)*dri(1)

c                 s - part
      coeff(icf(0 ,-1,0,eqn,CC),i1,i2,i3) =  coeff(icf(0,-1,0,eqn,CC),i1,i2,i3) + (FF) * (alpha-.5d0)    *rx(i1+off(1),i2+off(2),i3+off(3),2,X)*dri(2)
      coeff(icf(0 , 0,0,eqn,CC),i1,i2,i3) =  coeff(icf(0, 0,0,eqn,CC),i1,i2,i3) - (FF) * (3d0*alpha)     *rx(i1+off(1),i2+off(2),i3+off(3),2,X)*dri(2)
      coeff(icf(0 ,+1,0,eqn,CC),i1,i2,i3) =  coeff(icf(0,+1,0,eqn,CC),i1,i2,i3) + (FF) * (.5d0+3d0*alpha)*rx(i1+off(1),i2+off(2),i3+off(3),2,X)*dri(2)
      coeff(icf(0 ,+2,0,eqn,CC),i1,i2,i3) =  coeff(icf(0,+2,0,eqn,CC),i1,i2,i3) - (FF) * (alpha)         *rx(i1+off(1),i2+off(2),i3+off(3),2,X)*dri(2)
#endMacro

#beginMacro UUX4PC(X,FF,UC,CC)
c     coefficients for a 3rd order  difference approximation to FF * u[UC] u[CC]_X

c                 u[UC]^{n+1} * u[CC]^n_X
      coeff(icf(0,0,0,eqn,UC),i1,i2,i3) = coeff(icf(0,0,0,eqn,UC),i1,i2,i3) + (FF) * ux4p(i1,i2,i3,CC,X)

c                  (r_X u[CC]_r^{n+1} + s_X u[CC]_s^{n+1} )
      UX4PC(X,(FF)*u(i1+off(1),i2+off(2),i3+off(3),UC),CC)
c                 r - part
c      coeff(icf(+1,0,0,eqn,CC),i1,i2,i3) =  coeff(icf(+1,0,0,eqn,CC),i1,i2,i3) - (FF) * (alpha-.5d0)    *rx(i1+off(1),i2+off(2),i3+off(3),1,X)*u(i1+off(1),i2+off(2),i3+off(3),UC)*dri(1)
c      coeff(icf( 0,0,0,eqn,CC),i1,i2,i3) =  coeff(icf( 0,0,0,eqn,CC),i1,i2,i3) + (FF) * (3d0*alpha)     *rx(i1+off(1),i2+off(2),i3+off(3),1,X)*u(i1+off(1),i2+off(2),i3+off(3),UC)*dri(1)
c      coeff(icf(-1,0,0,eqn,CC),i1,i2,i3) =  coeff(icf(-1,0,0,eqn,CC),i1,i2,i3) - (FF) * (.5d0+3d0*alpha)*rx(i1+off(1),i2+off(2),i3+off(3),1,X)*u(i1+off(1),i2+off(2),i3+off(3),UC)*dri(1)
c      coeff(icf(-2,0,0,eqn,CC),i1,i2,i3) =  coeff(icf(-2,0,0,eqn,CC),i1,i2,i3) + (FF) * (alpha)         *rx(i1+off(1),i2+off(2),i3+off(3),1,X)*u(i1+off(1),i2+off(2),i3+off(3),UC)*dri(1)

c                 s - part
c      coeff(icf(0,+1,0,eqn,CC),i1,i2,i3) =  coeff(icf(0,+1,0,eqn,CC),i1,i2,i3) - (FF) * (alpha-.5d0)    *rx(i1+off(1),i2+off(2),i3+off(3),2,X)*u(i1+off(1),i2+off(2),i3+off(3),UC)*dri(2)
c      coeff(icf( 0,0,0,eqn,CC),i1,i2,i3) =  coeff(icf(0,0,0,eqn,CC),i1,i2,i3)  + (FF) * (3d0*alpha)     *rx(i1+off(1),i2+off(2),i3+off(3),2,X)*u(i1+off(1),i2+off(2),i3+off(3),UC)*dri(2)
c      coeff(icf(0,-1,0,eqn,CC),i1,i2,i3) =  coeff(icf(0,-1,0,eqn,CC),i1,i2,i3) - (FF) * (.5d0+3d0*alpha)*rx(i1+off(1),i2+off(2),i3+off(3),2,X)*u(i1+off(1),i2+off(2),i3+off(3),UC)*dri(2)
c      coeff(icf(0,-2,0,eqn,CC),i1,i2,i3) =  coeff(icf(0,-2,0,eqn,CC),i1,i2,i3) + (FF) * (alpha)         *rx(i1+off(1),i2+off(2),i3+off(3),2,X)*u(i1+off(1),i2+off(2),i3+off(3),UC)*dri(2)

#endMacro

#beginMacro UUX4MC(X,FF,UC,CC)
c     coefficients for a 3rd order  difference approximation to FF * u[UC] u[CC]_X

c                 u[UC]^{n+1} * u[CC]^n_X
      coeff(icf(0,0,0,eqn,UC),i1,i2,i3) = coeff(icf(0,0,0,eqn,UC),i1,i2,i3) + (FF) * ux4m(i1,i2,i3,CC,X)

c                 u[UC]^n * (r_X u[CC]_r^{n+1} + s_X u[CC]_s^{n+1} )
      UX4MC(X,(FF)*u(i1+off(1),i2+off(2),i3+off(3),UC),CC)
c                 r - part
c      coeff(icf(-1,0,0,eqn,CC),i1,i2,i3) =  coeff(icf(-1,0,0,eqn,CC),i1,i2,i3) + (FF) * (alpha-.5d0)    *rx(i1+off(1),i2+off(2),i3+off(3),1,X)*u(i1+off(1),i2+off(2),i3+off(3),UC)*dri(1)
c      coeff(icf( 0,0,0,eqn,CC),i1,i2,i3) =  coeff(icf( 0,0,0,eqn,CC),i1,i2,i3) - (FF) * (3d0*alpha)     *rx(i1+off(1),i2+off(2),i3+off(3),1,X)*u(i1+off(1),i2+off(2),i3+off(3),UC)*dri(1)
c      coeff(icf(+1,0,0,eqn,CC),i1,i2,i3) =  coeff(icf(+1,0,0,eqn,CC),i1,i2,i3) + (FF) * (.5d0+3d0*alpha)*rx(i1+off(1),i2+off(2),i3+off(3),1,X)*u(i1+off(1),i2+off(2),i3+off(3),UC)*dri(1)
c      coeff(icf(+2,0,0,eqn,CC),i1,i2,i3) =  coeff(icf(+2,0,0,eqn,CC),i1,i2,i3) - (FF) * (alpha)         *rx(i1+off(1),i2+off(2),i3+off(3),1,X)*u(i1+off(1),i2+off(2),i3+off(3),UC)*dri(1)

c                 s - part
c      coeff(icf(0,-1,0,eqn,CC),i1,i2,i3) =  coeff(icf(0,-1,0,eqn,CC),i1,i2,i3) + (FF) * (alpha-.5d0)    *rx(i1+off(1),i2+off(2),i3+off(3),2,X)*u(i1+off(1),i2+off(2),i3+off(3),UC)*dri(2)
c      coeff(icf( 0,0,0,eqn,CC),i1,i2,i3) =  coeff(icf(0,0,0,eqn,CC),i1,i2,i3)  - (FF) * (3d0*alpha)     *rx(i1+off(1),i2+off(2),i3+off(3),2,X)*u(i1+off(1),i2+off(2),i3+off(3),UC)*dri(2)
c      coeff(icf(0,+1,0,eqn,CC),i1,i2,i3) =  coeff(icf(0,+1,0,eqn,CC),i1,i2,i3) + (FF) * (.5d0+3d0*alpha)*rx(i1+off(1),i2+off(2),i3+off(3),2,X)*u(i1+off(1),i2+off(2),i3+off(3),UC)*dri(2)
c      coeff(icf(0,+2,0,eqn,CC),i1,i2,i3) =  coeff(icf(0,+2,0,eqn,CC),i1,i2,i3) - (FF) * (alpha)         *rx(i1+off(1),i2+off(2),i3+off(3),2,X)*u(i1+off(1),i2+off(2),i3+off(3),UC)*dri(2)

#endMacro

#beginMacro ULOGUX4MC(X,FF,UC,CC)
c     coefficients for a 3rd order  difference approximation to FF *  u[UC]*(log(u[CC]))_X

c                 u[UC]^{n+1} * u[CC]^n_X
      coeff(icf(0,0,0,eqn,UC),i1,i2,i3) = coeff(icf(0,0,0,eqn,UC),i1,i2,i3) + (FF) * logux4m(i1+off(1),i2+off(2),i3+off(3),CC,X)
      
c                 r - part
      coeff(icf(-1,0,0,eqn,CC),i1,i2,i3) =  coeff(icf(-1,0,0,eqn,CC),i1,i2,i3) + (FF) * (alpha-.5d0)    *rx(i1+off(1),i2+off(2),i3+off(3),1,X)*u(i1+off(1),i2+off(2),i3+off(3),UC)*dri(1)/u(i1-1+off(1),i2+off(2),i3+off(3),CC)
      coeff(icf( 0,0,0,eqn,CC),i1,i2,i3) =  coeff(icf( 0,0,0,eqn,CC),i1,i2,i3) - (FF) * (3d0*alpha)     *rx(i1+off(1),i2+off(2),i3+off(3),1,X)*u(i1+off(1),i2+off(2),i3+off(3),UC)*dri(1)/u(i1  +off(1),  i2+off(2),i3+off(3),CC)
      coeff(icf(+1,0,0,eqn,CC),i1,i2,i3) =  coeff(icf(+1,0,0,eqn,CC),i1,i2,i3) + (FF) * (.5d0+3d0*alpha)*rx(i1+off(1),i2+off(2),i3+off(3),1,X)*u(i1+off(1),i2+off(2),i3+off(3),UC)*dri(1)/u(i1+1+off(1),i2+off(2),i3+off(3),CC)
      if ( usestrik ) then
      coeff(icf(+2,0,0,eqn,CC),i1,i2,i3) =  coeff(icf(+2,0,0,eqn,CC),i1,i2,i3) - (FF) * (alpha)         *rx(i1+off(1),i2+off(2),i3+off(3),1,X)*u(i1+off(1),i2+off(2),i3+off(3),UC)*dri(1)/u(i1+2+off(1),i2+off(2),i3+off(3),CC)
      endif
c                 s - part
      coeff(icf(0,-1,0,eqn,CC),i1,i2,i3) =  coeff(icf(0,-1,0,eqn,CC),i1,i2,i3) + (FF) * (alpha-.5d0)    *rx(i1+off(1),i2+off(2),i3+off(3),2,X)*u(i1+off(1),i2+off(2),i3+off(3),UC)*dri(2)/u(i1+off(1),i2-1+off(2),i3+off(3),CC)
      coeff(icf( 0,0,0,eqn,CC),i1,i2,i3) =  coeff(icf(0,0,0,eqn,CC),i1,i2,i3)  - (FF) * (3d0*alpha)     *rx(i1+off(1),i2+off(2),i3+off(3),2,X)*u(i1+off(1),i2+off(2),i3+off(3),UC)*dri(2)/u(i1+off(1),i2  +off(2),i3+off(3),CC)
      coeff(icf(0,+1,0,eqn,CC),i1,i2,i3) =  coeff(icf(0,+1,0,eqn,CC),i1,i2,i3) + (FF) * (.5d0+3d0*alpha)*rx(i1+off(1),i2+off(2),i3+off(3),2,X)*u(i1+off(1),i2+off(2),i3+off(3),UC)*dri(2)/u(i1+off(1),i2+1+off(2),i3+off(3),CC)
      if ( usestrik ) then
      coeff(icf(0,+2,0,eqn,CC),i1,i2,i3) =  coeff(icf(0,+2,0,eqn,CC),i1,i2,i3) - (FF) * (alpha)         *rx(i1+off(1),i2+off(2),i3+off(3),2,X)*u(i1+off(1),i2+off(2),i3+off(3),UC)*dri(2)/u(i1+off(1),i2+2+off(2),i3+off(3),CC)
      endif

#endMacro

#beginMacro ULOGUX4PC(X,FF,UC,CC)
c     coefficients for a 3rd order  difference approximation to FF * u[UC] u[CC]_X

c                 u[UC]^{n+1} * u[CC]^n_X
      coeff(icf(0,0,0,eqn,UC),i1,i2,i3) = coeff(icf(0,0,0,eqn,UC),i1,i2,i3) + (FF) * logux4p(i1,i2,i3,CC,X)

c                  (r_X u[CC]_r^{n+1} + s_X u[CC]_s^{n+1} )
c                 r - part
      coeff(icf(+1,0,0,eqn,CC),i1,i2,i3) =  coeff(icf(+1,0,0,eqn,CC),i1,i2,i3) - (FF) * (alpha-.5d0)    *rx(i1+off(1),i2+off(2),i3+off(3),1,X)*u(i1+off(1),i2+off(2),i3+off(3),UC)*dri(1)/u(i1+1+off(1),i2+off(2),i3+off(3),CC)
      coeff(icf( 0,0,0,eqn,CC),i1,i2,i3) =  coeff(icf( 0,0,0,eqn,CC),i1,i2,i3) + (FF) * (3d0*alpha)     *rx(i1+off(1),i2+off(2),i3+off(3),1,X)*u(i1+off(1),i2+off(2),i3+off(3),UC)*dri(1)/u(i1+off(1),i2+off(2),i3+off(3),CC)
      coeff(icf(-1,0,0,eqn,CC),i1,i2,i3) =  coeff(icf(-1,0,0,eqn,CC),i1,i2,i3) - (FF) * (.5d0+3d0*alpha)*rx(i1+off(1),i2+off(2),i3+off(3),1,X)*u(i1+off(1),i2+off(2),i3+off(3),UC)*dri(1)/u(i1-1+off(1),i2+off(2),i3+off(3),CC)
      if ( usestrik ) then
      coeff(icf(-2,0,0,eqn,CC),i1,i2,i3) =  coeff(icf(-2,0,0,eqn,CC),i1,i2,i3) + (FF) * (alpha)         *rx(i1+off(1),i2+off(2),i3+off(3),1,X)*u(i1+off(1),i2+off(2),i3+off(3),UC)*dri(1)/u(i1-2+off(1),i2+off(2),i3+off(3),CC)
      endif
c                 s - part
      coeff(icf(0,+1,0,eqn,CC),i1,i2,i3) =  coeff(icf(0,+1,0,eqn,CC),i1,i2,i3) - (FF) * (alpha-.5d0)    *rx(i1+off(1),i2+off(2),i3+off(3),2,X)*u(i1+off(1),i2+off(2),i3+off(3),UC)*dri(2)/u(i1+off(1),i2+1+off(2),i3+off(3),CC)
      coeff(icf( 0,0,0,eqn,CC),i1,i2,i3) =  coeff(icf(0,0,0,eqn,CC),i1,i2,i3)  + (FF) * (3d0*alpha)     *rx(i1+off(1),i2+off(2),i3+off(3),2,X)*u(i1+off(1),i2+off(2),i3+off(3),UC)*dri(2)/u(i1+off(1),i2+off(2),i3+off(3),CC)
      coeff(icf(0,-1,0,eqn,CC),i1,i2,i3) =  coeff(icf(0,-1,0,eqn,CC),i1,i2,i3) - (FF) * (.5d0+3d0*alpha)*rx(i1+off(1),i2+off(2),i3+off(3),2,X)*u(i1+off(1),i2+off(2),i3+off(3),UC)*dri(2)/u(i1+off(1),i2-1+off(2),i3+off(3),CC)
      if ( usestrik ) then
      coeff(icf(0,-2,0,eqn,CC),i1,i2,i3) =  coeff(icf(0,-2,0,eqn,CC),i1,i2,i3) + (FF) * (alpha)         *rx(i1+off(1),i2+off(2),i3+off(3),2,X)*u(i1+off(1),i2+off(2),i3+off(3),UC)*dri(2)/u(i1+off(1),i2-2+off(2),i3+off(3),CC)
      endif
#endMacro

#beginMacro UX2OR(X,FF,CC)
c     linearize and add terms like FF * u[CC]_X^2/u[rc] to the matrix
c     FF * u[CC]_{X}^2/u[rc] --> FF * ( 2 u[CC]_X^n u[CC]_X^{n+1} - uc[rc]^{n+1} u[CC]_X^n/uc[rc]^n)/uc[rc]^n

c     FF * ( 2 u[CC]_X^n u[CC]_X^{n+1} )/u[rc]^n
c     r-part
      coeff(icf(+1,0,0,eqn,CC),i1,i2,i3) = coeff(icf(+1,0,0,eqn,CC),i1,i2,i3) + (FF)*2d0*ux(i1,i2,i3,CC,X)*rx(i1+off(1),i2+off(2),i3+off(3),1,X)*dri2(1)/u(i1+off(1),i2+off(2),i3+off(3),rc)
      coeff(icf(-1,0,0,eqn,CC),i1,i2,i3) = coeff(icf(-1,0,0,eqn,CC),i1,i2,i3) - (FF)*2d0*ux(i1,i2,i3,CC,X)*rx(i1+off(1),i2+off(2),i3+off(3),1,X)*dri2(1)/u(i1+off(1),i2+off(2),i3+off(3),rc)
c     s-part
      coeff(icf(0,+1,0,eqn,CC),i1,i2,i3) = coeff(icf(0,+1,0,eqn,CC),i1,i2,i3) + (FF)*2d0*ux(i1,i2,i3,CC,X)*rx(i1+off(1),i2+off(2),i3+off(3),2,X)*dri2(2)/u(i1+off(1),i2+off(2),i3+off(3),rc)
      coeff(icf(0,-1,0,eqn,CC),i1,i2,i3) = coeff(icf(0,-1,0,eqn,CC),i1,i2,i3) - (FF)*2d0*ux(i1,i2,i3,CC,X)*rx(i1+off(1),i2+off(2),i3+off(3),2,X)*dri2(2)/u(i1+off(1),i2+off(2),i3+off(3),rc)

c     - FF uc[rc]^{n+1} (u[CC]_X^n)^2/uc[rc]^n/uc[rc]^n
      coeff(icf(0,0,0,eqn,rc),i1,i2,i3) = coeff(icf(0,0,0,eqn,rc),i1,i2,i3) - (FF) * ux(i1,i2,i3,CC,X)**2/(u(i1+off(1),i2+off(2),i3+off(3),rc)**2)

#endMacro

c *wdh* 081214 -- xlf does not like +- --> change O1 to (O1) etc. below
#beginMacro UVCF(O1,O2,O3,FF,U,V,CFORRHS)
c      linearize terms like FF * u(U)* u(V) at the grid point offset by O1, O2, O3
      #If #CFORRHS == "RHS"
      rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (FF)*u(i1+(O1)+off(1),i2+(O2)+off(2),i3+(O3)+off(3),U)*u(i1+(O1)+off(1),i2+(O2)+off(2),i3+(O3)+off(3),V)
      #Else
      coeff(icf((O1),(O2),(O3),eqn,U),i1,i2,i3) = coeff(icf((O1),(O2),(O3),eqn,U),i1,i2,i3) + (FF)*u(i1+(O1)+off(1),i2+(O2)+off(2),i3+(O3)+off(3),V)
      coeff(icf((O1),(O2),(O3),eqn,V),i1,i2,i3) = coeff(icf((O1),(O2),(O3),eqn,V),i1,i2,i3) + (FF)*u(i1+(O1)+off(1),i2+(O2)+off(2),i3+(O3)+off(3),U)
      #End

#endMacro

#beginMacro UVCF_OFFSET(OU1,OU2,OU3,OV1,OV2,OV3,FF,U,V,CFORRHS)
c      linearize terms like FF * u(U)* u(V) at the grid point offset by O1, O2, O3
      #If #CFORRHS == "RHS"
      rhs(i1,i2,i3,eqn) = rhs(i1,i2,i3,eqn) + (FF)*u(i1+(OU1)+off(1),i2+(OU2)+off(2),i3+(OU3)+off(3),U)*u(i1+(OV1)+off(1),i2+(OV2)+off(2),i3+(OV3)+off(3),V)
      #Else
      coeff(icf((OU1),(OU2),(OU3),eqn,U),i1,i2,i3) = coeff(icf((OU1),(OU2),(OU3),eqn,U),i1,i2,i3) + (FF)*u(i1+(OV1)+off(1),i2+(OV2)+off(2),i3+(OV3)+off(3),V)
      coeff(icf((OV1),(OV2),(OV3),eqn,V),i1,i2,i3) = coeff(icf((OV1),(OV2),(OV3),eqn,V),i1,i2,i3) + (FF)*u(i1+(OU1)+off(1),i2+(OU2)+off(2),i3+(OU3)+off(3),U)
      #End

#endMacro

#beginMacro AXIAVG(FF,U,V,CFORRHS)

      if ( .true. ) then
      drar = (FF)*dri2(1)*jaci*jac(i1+off(1)-1,i2+off(2),i3+off(3))*(xyz(i1+off(1),i2+off(2),i3+off(3),2)-xyz(i1+off(1)-1,i2+off(2),i3+off(3),2))*rx(i1+off(1)-1,i2+off(2),i3+off(3),1,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
      UVCF(-1,0,0,drar,U,V,CFORRHS)
      
      drar = (FF)*dri2(1)*jaci*jac(i1+off(1)+1,i2+off(2),i3+off(3))*(xyz(i1+off(1)+1,i2+off(2),i3+off(3),2)-xyz(i1+off(1),i2+off(2),i3+off(3),2))*rx(i1+off(1)+1,i2+off(2),i3+off(3),1,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
      UVCF(+1,0,0,drar,U,V,CFORRHS)
      
      drar = (FF)*dri2(2)*jaci*jac(i1+off(1),i2+off(2)-1,i3+off(3))*(xyz(i1+off(1),i2+off(2),i3+off(3),2)-xyz(i1+off(1),i2+off(2)-1,i3+off(3),2))*rx(i1+off(1),i2+off(2)-1,i3+off(3),2,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
      UVCF(0,-1,0,drar,U,V,CFORRHS)
      
      drar = (FF)*dri2(2)*jaci*jac(i1+off(1),i2+off(2)+1,i3+off(3))*(xyz(i1+off(1),i2+off(2)+1,i3+off(3),2)-xyz(i1+off(1),i2+off(2),i3+off(3),2))*rx(i1+off(1),i2+off(2)+1,i3+off(3),2,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
      UVCF(0,+1,0,drar,U,V,CFORRHS)
      else if ( .false. ) then
         drar = (FF)*.25d0
        UVCF(-1,0,0,drar/xyz(i1-1,i2,i3,2),U,V,CFORRHS)
        UVCF(+1,0,0,drar/xyz(i1+1,i2,i3,2),U,V,CFORRHS)
        UVCF(0,-1,0,drar/xyz(i1,i2-1,i3,2),U,V,CFORRHS)
        UVCF(0,+1,0,drar/xyz(i1,i2+1,i3,2),U,V,CFORRHS)
      else

         UVCF(0,0,0,((FF)/(xyz(i1,i2,i3,2) )),U,V,CFORRHS)

      endif

#endMacro

#beginMacro AXIAVG_1(FF,U,V,CFORRHS)

      drar = (FF)*dri2(1)*jaci*jac(i1+off(1)-1,i2+off(2),i3+off(3))*(xyz(i1+off(1),i2+off(2),i3+off(3),2)-xyz(i1+off(1)-1,i2+off(2),i3+off(3),2))*rx(i1+off(1)-1,i2+off(2),i3+off(3),1,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
      UVCF_OFFSET(0,0,0,-1,0,0,drar,U,V,CFORRHS)
      
      drar = (FF)*dri2(1)*jaci*jac(i1+off(1)+1,i2+off(2),i3+off(3))*(xyz(i1+off(1)+1,i2+off(2),i3+off(3),2)-xyz(i1+off(1),i2+off(2),i3+off(3),2))*rx(i1+off(1)+1,i2+off(2),i3+off(3),1,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
      UVCF_OFFSET(0,0,0,+1,0,0,drar,U,V,CFORRHS)
      
      drar = (FF)*dri2(2)*jaci*jac(i1+off(1),i2+off(2)-1,i3+off(3))*(xyz(i1+off(1),i2+off(2),i3+off(3),2)-xyz(i1+off(1),i2+off(2)-1,i3+off(3),2))*rx(i1+off(1),i2+off(2)-1,i3+off(3),2,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
      UVCF_OFFSET(0,0,0,0,-1,0,drar,U,V,CFORRHS)
      
      drar = (FF)*dri2(2)*jaci*jac(i1+off(1),i2+off(2)+1,i3+off(3))*(xyz(i1+off(1),i2+off(2)+1,i3+off(3),2)-xyz(i1+off(1),i2+off(2),i3+off(3),2))*rx(i1+off(1),i2+off(2)+1,i3+off(3),2,d)/xyz(i1+off(1),i2+off(2),i3+off(3),2)
      UVCF_OFFSET(0,0,0,0,+1,0,drar,U,V,CFORRHS)


#endMacro



