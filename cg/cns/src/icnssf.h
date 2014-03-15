c     icf computes the coefficient location given an index offset m1,m2,m3, equation e, and component c
c     icf corresponds to the M123CE macro found in many C++ files dealing with coefficient gridFunctions
c      integer icf
      icf (m1,m2,m3,e,c) = 
     &               (off(1)+m1)+hwidth+width*((off(2)+m2)+hwidth+width3
     &                                          *((off(3)+m3)+hwidth)) +
     &               isten_size*((occ+c)+ncmp*(oce+e))

c     rxr,rxs,rxt are approximate second derivatives of the mapping in the parameter space (r,s,t)
c       so rxr(i1,i2,i3,c,d) is the r-derivative of the "c"-derivative (x,y,z) of coordinate "d" (r,s,t)
c      double precision rxr 
      rxr(i1,i2,i3,d,c)=(rx(i1+1,i2,i3,d,c)-rx(i1-1,i2,i3,d,c))*dri2(1) ! (d_c)_r
c      double precision rxs
      rxs(i1,i2,i3,d,c)=(rx(i1,i2+1,i3,d,c)-rx(i1,i2-1,i3,d,c))*dri2(2) ! (d_c)_s
c      double precision rxt
      rxt(i1,i2,i3,d,c)=(rx(i1,i2,i3-1,d,c)-rx(i1,i2,i3+1,d,c))*dri2(3) ! (d_c)_t

c     rxx is the approximate second derivative of the mapping in physical space
c     XXX only 2D implemented
c      double precision rxx ! e (x,y) derivative of the "c"-derivative (x,y,z) of coordinate "d"
                           ! basically you get d_{ce}
      rxx(i1,i2,i3,d,c,e) = rx(i1,i2,i3,1,e)*rxr(i1,i2,i3,d,c)+ ! r_e * ( d_c )_r +
     &                      rx(i1,i2,i3,2,e)*rxs(i1,i2,i3,d,c)  ! s_e * ( d_c )_s
c      double precision rxy
      rxy(i1,i2,i3,d,c) = rx(i1,i2,i3,1,2)*rxr(i1,i2,i3,d,c)+ ! r_y * ( d_c )_r +
     &                    rx(i1,i2,i3,2,2)*rxs(i1,i2,i3,d,c)  ! s_y * ( d_c )_s

c     rxxx : third c (x,y) derivative of computational coordinate "d"
      rxxx(i1,i2,i3,d,c) =
     &    dri2(1)*dri2(2)*(rx(i1,i2,i3,1,c)*rx(i1,i2,i3,2,c)+
     &     rx(i1,i2,i3,1,c)*rx(i1,i2,i3,2,c)) * rx(i1-1,i2-1,i3,d,c) + !-1,-1
     &    (rx(i1,i2,i3,2,c)*rx(i1,i2,i3,2,c)*dri(2)*dri(2) -
     &     rxx(i1,i2,i3,2,c,c)*dri2(2) )      * rx(i1,i2-1,i3,d,c)   - ! 0,-1
     &    dri2(1)*dri2(2)*(rx(i1,i2,i3,1,c)*rx(i1,i2,i3,2,c)+
     &     rx(i1,i2,i3,1,c)*rx(i1,i2,i3,2,c)) * rx(i1+1,i2-1,i3,d,c) + !+1,-1
     &    (rx(i1,i2,i3,1,c)*rx(i1,i2,i3,1,c)*dri(1)*dri(1) -
     &     rxx(i1,i2,i3,1,c,c)*dri2(1) )      * rx(i1-1,i2,i3,d,c)  -  !-1, 0
     &    2d0 * ( rx(i1,i2,i3,1,c)*rx(i1,i2,i3,1,c)*dri(1)*dri(1) +
     &     rx(i1,i2,i3,2,c)*rx(i1,i2,i3,2,c)*dri(2)*dri(2) )
     &                                          * rx(i1,i2,i3,d,c) +     ! 0, 0
     &    (rx(i1,i2,i3,1,c)*rx(i1,i2,i3,1,c)*dri(1)*dri(1) +
     &      rxx(i1,i2,i3,1,c,c)*dri2(1) )     * rx(i1+1,i2,i3,d,c) -   !+1, 0
     &    dri2(1)*dri2(2)*(rx(i1,i2,i3,1,c)*rx(i1,i2,i3,2,c)+
     &     rx(i1,i2,i3,1,c)*rx(i1,i2,i3,2,c)) * rx(i1-1,i2+1,i3,d,c) + !-1,+1
     &    (rx(i1,i2,i3,2,c)*rx(i1,i2,i3,2,c)*dri(2)*dri(2) +
     &     rxx(i1,i2,i3,2,c,c)*dri2(2) )      * rx(i1,i2+1,i3,d,c) +   ! 0,+1
     &    dri2(1)*dri2(2)*(rx(i1,i2,i3,1,c)*rx(i1,i2,i3,2,c)+
     &     rx(i1,i2,i3,1,c)*rx(i1,i2,i3,2,c)) * rx(i1+1,i2+1,i3,d,c) !+1,+1

c     rxxxx : fourth c (x,y) derivative of computation coordinate "d"
      rxxxx(i1,i2,i3,d,c) = (rxxx(i1+1,i2,i3,d,c)-rxxx(i1-1,i2,i3,d,c))*
     &     dri(1)*rx(i1,i2,i3,1,c) +
     &                      (rxxx(i1,i2+1,i3,d,c)-rxxx(i1,i2+1,i3,d,c))*
     &     dri(2)*rx(i1,i2,i3,2,c)

c     ux approximates a first derivative of component c
c      double precision ux
      ux(i1,i2,i3,c,m1) = 
     &     rx(i1,i2,i3,1,m1)*(u(i1+1,i2,i3,c)-u(i1-1,i2,i3,c))*dri2(1) + ! r_m * u_r +
     &     rx(i1,i2,i3,2,m1)*(u(i1,i2+1,i3,c)-u(i1,i2-1,i3,c))*dri2(2)   ! s_m * u_s

c     ux4m D_o - a*h^2D_-D_+^2
      ux4m(i1,i2,i3,c,m1) =
     &     rx(i1,i2,i3,1,m1)*( -alpha*u(i1+2,i2,i3,c)+
     &               (.5d0+3d0*alpha)*u(i1+1,i2,i3,c) - 
     &                      3d0*alpha*u(i1,i2,i3,c) + 
     &                   (alpha-.5d0)*u(i1-1,i2,i3,c))*dri(1) +
     &     rx(i1,i2,i3,2,m1)*( -alpha*u(i1,i2+2,i3,c)+
     &               (.5d0+3d0*alpha)*u(i1,i2+1,i3,c) - 
     &                      3d0*alpha*u(i1,i2,i3,c) + 
     &                   (alpha-.5d0)*u(i1,i2-1,i3,c))*dri(2)

      logux4m(i1,i2,i3,c,m1) =
     &     rx(i1,i2,i3,1,m1)*( -alpha*log(abs(u(i1+2,i2,i3,c)))+
     &               (.5d0+3d0*alpha)*log(abs(u(i1+1,i2,i3,c))) - 
     &                      3d0*alpha*log(abs(u(i1,i2,i3,c))) + 
     &                   (alpha-.5d0)*log(abs(u(i1-1,i2,i3,c))))*dri(1)+
     &     rx(i1,i2,i3,2,m1)*( -alpha*log(abs(u(i1,i2+2,i3,c)))+
     &               (.5d0+3d0*alpha)*log(abs(u(i1,i2+1,i3,c))) - 
     &                      3d0*alpha*log(abs(u(i1,i2,i3,c))) + 
     &                   (alpha-.5d0)*log(abs(u(i1,i2-1,i3,c))))*dri(2)

      logux(i1,i2,i3,c,m1) =
     &     rx(i1,i2,i3,1,m1)*( 
     &               (.5d0)*log(abs(u(i1+1,i2,i3,c))) -
     &               (.5d0)*log(abs(u(i1-1,i2,i3,c))))*dri(1)+
     &     rx(i1,i2,i3,2,m1)*( 
     &               (.5d0)*log(abs(u(i1,i2+1,i3,c))) - 
     &               (.5d0)*log(abs(u(i1,i2-1,i3,c))))*dri(2)

c     ux4p D_o - a*h^2D_+D_-^2
      ux4p(i1,i2,i3,c,m1) =
     &     rx(i1,i2,i3,1,m1)*( +alpha*u(i1-2,i2,i3,c) -
     &               (.5d0+3d0*alpha)*u(i1-1,i2,i3,c) + 
     &                      3d0*alpha*u(i1,i2,i3,c) -
     &                   (alpha-.5d0)*u(i1+1,i2,i3,c))*dri(1) +
     &     rx(i1,i2,i3,2,m1)*( +alpha*u(i1,i2-2,i3,c)-
     &               (.5d0+3d0*alpha)*u(i1,i2-1,i3,c) + 
     &                      3d0*alpha*u(i1,i2,i3,c) -
     &                   (alpha-.5d0)*u(i1,i2+1,i3,c))*dri(2)

      logux4p(i1,i2,i3,c,m1) =
     &     rx(i1,i2,i3,1,m1)*( +alpha*log(abs(u(i1-2,i2,i3,c))) -
     &               (.5d0+3d0*alpha)*log(abs(u(i1-1,i2,i3,c))) + 
     &                      3d0*alpha*log(abs(u(i1,i2,i3,c))) -
     &                   (alpha-.5d0)*log(abs(u(i1+1,i2,i3,c))))*dri(1)+
     &     rx(i1,i2,i3,2,m1)*( +alpha*log(abs(u(i1,i2-2,i3,c)))-
     &               (.5d0+3d0*alpha)*log(abs(u(i1,i2-1,i3,c))) + 
     &                      3d0*alpha*log(abs(u(i1,i2,i3,c))) -
     &                   (alpha-.5d0)*log(abs(u(i1,i2+1,i3,c))))*dri(2)

c     uxx approximates a second (possibly mixed) derivative of component c
c      double precision uxx
      uxx(i1,i2,i3,c,m1,m2) =
     &    dri2(1)*dri2(2)*(rx(i1,i2,i3,1,m1)*rx(i1,i2,i3,2,m2)+
     &     rx(i1,i2,i3,1,m2)*rx(i1,i2,i3,2,m1)) * u(i1-1,i2-1,i3,c) + !-1,-1
     &    (rx(i1,i2,i3,2,m1)*rx(i1,i2,i3,2,m2)*dri(2)*dri(2) -
     &     rxx(i1,i2,i3,2,m1,m2)*dri2(2) )      * u(i1,i2-1,i3,c)   - ! 0,-1
     &    dri2(1)*dri2(2)*(rx(i1,i2,i3,1,m1)*rx(i1,i2,i3,2,m2)+
     &     rx(i1,i2,i3,1,m2)*rx(i1,i2,i3,2,m1)) * u(i1+1,i2-1,i3,c) + !+1,-1
     &    (rx(i1,i2,i3,1,m1)*rx(i1,i2,i3,1,m2)*dri(1)*dri(1) -
     &     rxx(i1,i2,i3,1,m1,m2)*dri2(1) )      * u(i1-1,i2,i3,c)  -  !-1, 0
     &    2d0 * ( rx(i1,i2,i3,1,m1)*rx(i1,i2,i3,1,m2)*dri(1)*dri(1) +
     &     rx(i1,i2,i3,2,m1)*rx(i1,i2,i3,2,m2)*dri(2)*dri(2) )
     &                                          * u(i1,i2,i3,c) +     ! 0, 0
     &    (rx(i1,i2,i3,1,m1)*rx(i1,i2,i3,1,m2)*dri(1)*dri(1) +
     &      rxx(i1,i2,i3,1,m1,m2)*dri2(1) )     * u(i1+1,i2,i3,c) -   !+1, 0
     &    dri2(1)*dri2(2)*(rx(i1,i2,i3,1,m1)*rx(i1,i2,i3,2,m2)+
     &     rx(i1,i2,i3,1,m2)*rx(i1,i2,i3,2,m1)) * u(i1-1,i2+1,i3,c) + !-1,+1
     &    (rx(i1,i2,i3,2,m1)*rx(i1,i2,i3,2,m2)*dri(2)*dri(2) +
     &     rxx(i1,i2,i3,2,m1,m2)*dri2(2) )      * u(i1,i2+1,i3,c) +   ! 0,+1
     &    dri2(1)*dri2(2)*(rx(i1,i2,i3,1,m1)*rx(i1,i2,i3,2,m2)+
     &     rx(i1,i2,i3,1,m2)*rx(i1,i2,i3,2,m1)) * u(i1+1,i2+1,i3,c) !+1,+1

c    radx4p
      radx4p(i1,i2,i3) = rx(i1,i2,i3,1,2)*dri(1)*(
     &         alpha*xyz(i1-2,i2,i3,2)- 
     &     3d0*alpha*xyz(i1-1,i2,i3,2)+
     &     3d0*alpha*xyz(i1,i2,i3,2)-
     &         alpha*xyz(i1+1,i2,i3,2)) + 
     &                  rx(i1,i2,i3,2,2)*dri(2)*(
     &         alpha*xyz(i1,i2-2,i3,2)- 
     &     3d0*alpha*xyz(i1,i2-1,i3,2)+
     &     3d0*alpha*xyz(i1,i2,i3,2)-
     &         alpha*xyz(i1,i2+1,i3,2))

c     END OF STATEMENT FUNCTIONS
