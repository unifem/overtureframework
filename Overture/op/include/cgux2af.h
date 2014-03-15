c
c CGUX statement functions for second-order difference approximations
c    needs d12(kd) = 1/(2*h(kd))
c          d22(kd) = 1/(h(kd)**2)
c

      ur2(i1,i2,i3,kd)=(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))*d12(1)
      us2(i1,i2,i3,kd)=(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))*d12(2)
      ut2(i1,i2,i3,kd)=(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))*d12(3)
c     ur2a(i1,i2,i3,kd)=(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))*d12(1)
c     us2a(i1,i2,i3,kd)=(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))*d12(2)
c     ut2a(i1,i2,i3,kd)=(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))*d12(3)

      rxr2(i1,i2,i3)=(rx(i1+1,i2,i3)-rx(i1-1,i2,i3))*d12(1)
      ryr2(i1,i2,i3)=(ry(i1+1,i2,i3)-ry(i1-1,i2,i3))*d12(1)
      rzr2(i1,i2,i3)=(rz(i1+1,i2,i3)-rz(i1-1,i2,i3))*d12(1)
      rxs2(i1,i2,i3)=(rx(i1,i2+1,i3)-rx(i1,i2-1,i3))*d12(2)
      rys2(i1,i2,i3)=(ry(i1,i2+1,i3)-ry(i1,i2-1,i3))*d12(2)
      rzs2(i1,i2,i3)=(rz(i1,i2+1,i3)-rz(i1,i2-1,i3))*d12(2)
      rxt2(i1,i2,i3)=(rx(i1,i2,i3+1)-rx(i1,i2,i3-1))*d12(3)
      ryt2(i1,i2,i3)=(ry(i1,i2,i3+1)-ry(i1,i2,i3-1))*d12(3)
      rzt2(i1,i2,i3)=(rz(i1,i2,i3+1)-rz(i1,i2,i3-1))*d12(3)
      sxr2(i1,i2,i3)=(sx(i1+1,i2,i3)-sx(i1-1,i2,i3))*d12(1)
      syr2(i1,i2,i3)=(sy(i1+1,i2,i3)-sy(i1-1,i2,i3))*d12(1)
      szr2(i1,i2,i3)=(sz(i1+1,i2,i3)-sz(i1-1,i2,i3))*d12(1)
      sxs2(i1,i2,i3)=(sx(i1,i2+1,i3)-sx(i1,i2-1,i3))*d12(2)
      sys2(i1,i2,i3)=(sy(i1,i2+1,i3)-sy(i1,i2-1,i3))*d12(2)
      szs2(i1,i2,i3)=(sz(i1,i2+1,i3)-sz(i1,i2-1,i3))*d12(2)
      sxt2(i1,i2,i3)=(sx(i1,i2,i3+1)-sx(i1,i2,i3-1))*d12(3)
      syt2(i1,i2,i3)=(sy(i1,i2,i3+1)-sy(i1,i2,i3-1))*d12(3)
      szt2(i1,i2,i3)=(sz(i1,i2,i3+1)-sz(i1,i2,i3-1))*d12(3)
      txr2(i1,i2,i3)=(tx(i1+1,i2,i3)-tx(i1-1,i2,i3))*d12(1)
      tyr2(i1,i2,i3)=(ty(i1+1,i2,i3)-ty(i1-1,i2,i3))*d12(1)
      tzr2(i1,i2,i3)=(tz(i1+1,i2,i3)-tz(i1-1,i2,i3))*d12(1)
      txs2(i1,i2,i3)=(tx(i1,i2+1,i3)-tx(i1,i2-1,i3))*d12(2)
      tys2(i1,i2,i3)=(ty(i1,i2+1,i3)-ty(i1,i2-1,i3))*d12(2)
      tzs2(i1,i2,i3)=(tz(i1,i2+1,i3)-tz(i1,i2-1,i3))*d12(2)
      txt2(i1,i2,i3)=(tx(i1,i2,i3+1)-tx(i1,i2,i3-1))*d12(3)
      tyt2(i1,i2,i3)=(ty(i1,i2,i3+1)-ty(i1,i2,i3-1))*d12(3)
      tzt2(i1,i2,i3)=(tz(i1,i2,i3+1)-tz(i1,i2,i3-1))*d12(3)

      ux22(i1,i2,i3,kd)= rx(i1,i2,i3)*ur2(i1,i2,i3,kd)
     &                 +sx(i1,i2,i3)*us2(i1,i2,i3,kd)
      uy22(i1,i2,i3,kd)= ry(i1,i2,i3)*ur2(i1,i2,i3,kd)
     &                 +sy(i1,i2,i3)*us2(i1,i2,i3,kd)
      ux23(i1,i2,i3,kd)=rx(i1,i2,i3)*ur2(i1,i2,i3,kd)
     &                 +sx(i1,i2,i3)*us2(i1,i2,i3,kd)
     &                 +tx(i1,i2,i3)*ut2(i1,i2,i3,kd)
      uy23(i1,i2,i3,kd)=ry(i1,i2,i3)*ur2(i1,i2,i3,kd)
     &                 +sy(i1,i2,i3)*us2(i1,i2,i3,kd)
     &                 +ty(i1,i2,i3)*ut2(i1,i2,i3,kd)
      uz23(i1,i2,i3,kd)=rz(i1,i2,i3)*ur2(i1,i2,i3,kd)
     &                 +sz(i1,i2,i3)*us2(i1,i2,i3,kd)
     &                 +tz(i1,i2,i3)*ut2(i1,i2,i3,kd)

      rxx2(i1,i2,i3)=rx(i1,i2,i3)*rxr2(i1,i2,i3)
     &              +sx(i1,i2,i3)*rxs2(i1,i2,i3)
c*wdh 030729      rxy2(i1,i2,i3)=rx(i1,i2,i3)*ryr2(i1,i2,i3)
c*wdh 030729    &              +sx(i1,i2,i3)*rys2(i1,i2,i3)
      rxy2(i1,i2,i3)=ry(i1,i2,i3)*rxr2(i1,i2,i3)
     &              +sy(i1,i2,i3)*rxs2(i1,i2,i3)
      ryy2(i1,i2,i3)=ry(i1,i2,i3)*ryr2(i1,i2,i3)
     &              +sy(i1,i2,i3)*rys2(i1,i2,i3)
      sxx2(i1,i2,i3)=rx(i1,i2,i3)*sxr2(i1,i2,i3)
     &              +sx(i1,i2,i3)*sxs2(i1,i2,i3)
c*wdh 030729       sxy2(i1,i2,i3)=rx(i1,i2,i3)*syr2(i1,i2,i3)
c*wdh 030729      &              +sx(i1,i2,i3)*sys2(i1,i2,i3)
      sxy2(i1,i2,i3)=ry(i1,i2,i3)*sxr2(i1,i2,i3)
     &              +sy(i1,i2,i3)*sxs2(i1,i2,i3)
      syy2(i1,i2,i3)=ry(i1,i2,i3)*syr2(i1,i2,i3)
     &              +sy(i1,i2,i3)*sys2(i1,i2,i3)

      rxx23(i1,i2,i3)=rx(i1,i2,i3)*rxr2(i1,i2,i3)
     &               +sx(i1,i2,i3)*rxs2(i1,i2,i3)
     &               +tx(i1,i2,i3)*rxt2(i1,i2,i3)
      rxy23(i1,i2,i3)=rx(i1,i2,i3)*ryr2(i1,i2,i3)
     &               +sx(i1,i2,i3)*rys2(i1,i2,i3)
     &               +tx(i1,i2,i3)*ryt2(i1,i2,i3)
      rxz23(i1,i2,i3)=rx(i1,i2,i3)*rzr2(i1,i2,i3)
     &               +sx(i1,i2,i3)*rzs2(i1,i2,i3)
     &               +tx(i1,i2,i3)*rzt2(i1,i2,i3)
      ryy23(i1,i2,i3)=ry(i1,i2,i3)*ryr2(i1,i2,i3)
     &               +sy(i1,i2,i3)*rys2(i1,i2,i3)
     &               +ty(i1,i2,i3)*ryt2(i1,i2,i3)
      ryz23(i1,i2,i3)=ry(i1,i2,i3)*rzr2(i1,i2,i3)
     &               +sy(i1,i2,i3)*rzs2(i1,i2,i3)
     &               +ty(i1,i2,i3)*rzt2(i1,i2,i3)
      rzz23(i1,i2,i3)=rz(i1,i2,i3)*rzr2(i1,i2,i3)
     &               +sz(i1,i2,i3)*rzs2(i1,i2,i3)
     &               +tz(i1,i2,i3)*rzt2(i1,i2,i3)
      sxx23(i1,i2,i3)=rx(i1,i2,i3)*sxr2(i1,i2,i3)
     &               +sx(i1,i2,i3)*sxs2(i1,i2,i3)
     &               +tx(i1,i2,i3)*sxt2(i1,i2,i3)
      sxy23(i1,i2,i3)=rx(i1,i2,i3)*syr2(i1,i2,i3)
     &               +sx(i1,i2,i3)*sys2(i1,i2,i3)
     &               +tx(i1,i2,i3)*syt2(i1,i2,i3)
      sxz23(i1,i2,i3)=rx(i1,i2,i3)*szr2(i1,i2,i3)
     &               +sx(i1,i2,i3)*szs2(i1,i2,i3)
     &               +tx(i1,i2,i3)*szt2(i1,i2,i3)
      syy23(i1,i2,i3)=ry(i1,i2,i3)*syr2(i1,i2,i3)
     &               +sy(i1,i2,i3)*sys2(i1,i2,i3)
     &               +ty(i1,i2,i3)*syt2(i1,i2,i3)
      syz23(i1,i2,i3)=ry(i1,i2,i3)*szr2(i1,i2,i3)
     &               +sy(i1,i2,i3)*szs2(i1,i2,i3)
     &               +ty(i1,i2,i3)*szt2(i1,i2,i3)
      szz23(i1,i2,i3)=rz(i1,i2,i3)*szr2(i1,i2,i3)
     &               +sz(i1,i2,i3)*szs2(i1,i2,i3)
     &               +tz(i1,i2,i3)*szt2(i1,i2,i3)
      txx23(i1,i2,i3)=rx(i1,i2,i3)*txr2(i1,i2,i3)
     &               +sx(i1,i2,i3)*txs2(i1,i2,i3)
     &               +tx(i1,i2,i3)*txt2(i1,i2,i3)
      txy23(i1,i2,i3)=rx(i1,i2,i3)*tyr2(i1,i2,i3)
     &               +sx(i1,i2,i3)*tys2(i1,i2,i3)
     &               +tx(i1,i2,i3)*tyt2(i1,i2,i3)
      txz23(i1,i2,i3)=rx(i1,i2,i3)*tzr2(i1,i2,i3)
     &               +sx(i1,i2,i3)*tzs2(i1,i2,i3)
     &               +tx(i1,i2,i3)*tzt2(i1,i2,i3)
      tyy23(i1,i2,i3)=ry(i1,i2,i3)*tyr2(i1,i2,i3)
     &               +sy(i1,i2,i3)*tys2(i1,i2,i3)
     &               +ty(i1,i2,i3)*tyt2(i1,i2,i3)
      tyz23(i1,i2,i3)=ry(i1,i2,i3)*tzr2(i1,i2,i3)
     &               +sy(i1,i2,i3)*tzs2(i1,i2,i3)
     &               +ty(i1,i2,i3)*tzt2(i1,i2,i3)
      tzz23(i1,i2,i3)=rz(i1,i2,i3)*tzr2(i1,i2,i3)
     &               +sz(i1,i2,i3)*tzs2(i1,i2,i3)
     &               +tz(i1,i2,i3)*tzt2(i1,i2,i3)
             
      urr2(i1,i2,i3,kd)=
     & ( -2.*u(i1,i2,i3,kd)
     &  +   (u(i1+1,i2,i3,kd)+u(i1-1,i2,i3,kd))  )*d22(1)
      uss2(i1,i2,i3,kd)=
     & ( -2.*u(i1,i2,i3,kd)
     &  +    (u(i1,i2+1,i3,kd)+u(i1,i2-1,i3,kd)) )*d22(2)
      urs2(i1,i2,i3,kd)=
     &  (ur2(i1,i2+1,i3,kd)-ur2(i1,i2-1,i3,kd))*d12(2)
      utt2(i1,i2,i3,kd)=
     & ( -2.*u(i1,i2,i3,kd)
     &  +   (u(i1,i2,i3+1,kd)+u(i1,i2,i3-1,kd))  )*d22(3)
      urt2(i1,i2,i3,kd)=
     &   (ur2(i1,i2,i3+1,kd)-ur2(i1,i2,i3-1,kd))*d12(3)
      ust2(i1,i2,i3,kd)=
     &  (us2(i1,i2,i3+1,kd)-us2(i1,i2,i3-1,kd))*d12(3)
             
      uxx21(i1,i2,i3,kd)=
     & (rx(i1,i2,i3)**2                )*urr2(i1,i2,i3,kd)
     &     +(rxx2(i1,i2,i3)              )*ur2(i1,i2,i3,kd)


      uxx22(i1,i2,i3,kd)=
     & (rx(i1,i2,i3)**2                )*urr2(i1,i2,i3,kd)
     &+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)                           )
     &                                  *urs2(i1,i2,i3,kd)
     &+(sx(i1,i2,i3)**2                )*uss2(i1,i2,i3,kd)
     &     +(rxx2(i1,i2,i3)              )*ur2(i1,i2,i3,kd)
     &     +(sxx2(i1,i2,i3)              )*us2(i1,i2,i3,kd)
      uyy22(i1,i2,i3,kd)=
     & (                ry(i1,i2,i3)**2)*urr2(i1,i2,i3,kd)
     &+2.*(                           ry(i1,i2,i3)*sy(i1,i2,i3))
     &                                  *urs2(i1,i2,i3,kd)
     &+(                sy(i1,i2,i3)**2)*uss2(i1,i2,i3,kd)
     &     +(              ryy2(i1,i2,i3))*ur2(i1,i2,i3,kd)
     &     +(              syy2(i1,i2,i3))*us2(i1,i2,i3,kd)
      uxy22(i1,i2,i3,kd)=
     &    rx(i1,i2,i3)*ry(i1,i2,i3)*urr2(i1,i2,i3,kd)
     &+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))
     &                             *urs2(i1,i2,i3,kd)
     &+   sx(i1,i2,i3)*sy(i1,i2,i3)*uss2(i1,i2,i3,kd)
     &  +rxy2(i1,i2,i3)              *ur2(i1,i2,i3,kd)
     &  +sxy2(i1,i2,i3)              *us2(i1,i2,i3,kd)
             
      uxx23(i1,i2,i3,kd)=
     & rx(i1,i2,i3)**2 *urr2(i1,i2,i3,kd)
     &+sx(i1,i2,i3)**2 *uss2(i1,i2,i3,kd)
     &+tx(i1,i2,i3)**2 *utt2(i1,i2,i3,kd)
     &+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*urs2(i1,i2,i3,kd)
     &+2.*rx(i1,i2,i3)*tx(i1,i2,i3)*urt2(i1,i2,i3,kd)
     &+2.*sx(i1,i2,i3)*tx(i1,i2,i3)*ust2(i1,i2,i3,kd)
     &+rxx23(i1,i2,i3)*ur2(i1,i2,i3,kd)
     &+sxx23(i1,i2,i3)*us2(i1,i2,i3,kd)
     &+txx23(i1,i2,i3)*ut2(i1,i2,i3,kd)
              
      uyy23(i1,i2,i3,kd)=
     & ry(i1,i2,i3)**2 *urr2(i1,i2,i3,kd)
     &+sy(i1,i2,i3)**2 *uss2(i1,i2,i3,kd)
     &+ty(i1,i2,i3)**2 *utt2(i1,i2,i3,kd)
     &+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*urs2(i1,i2,i3,kd)
     &+2.*ry(i1,i2,i3)*ty(i1,i2,i3)*urt2(i1,i2,i3,kd)
     &+2.*sy(i1,i2,i3)*ty(i1,i2,i3)*ust2(i1,i2,i3,kd)
     &+ryy23(i1,i2,i3)*ur2(i1,i2,i3,kd)
     &+syy23(i1,i2,i3)*us2(i1,i2,i3,kd)
     &+tyy23(i1,i2,i3)*ut2(i1,i2,i3,kd)
                 
      uzz23(i1,i2,i3,kd)=
     & rz(i1,i2,i3)**2 *urr2(i1,i2,i3,kd)
     &+sz(i1,i2,i3)**2 *uss2(i1,i2,i3,kd)
     &+tz(i1,i2,i3)**2 *utt2(i1,i2,i3,kd)
     &+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*urs2(i1,i2,i3,kd)
     &+2.*rz(i1,i2,i3)*tz(i1,i2,i3)*urt2(i1,i2,i3,kd)
     &+2.*sz(i1,i2,i3)*tz(i1,i2,i3)*ust2(i1,i2,i3,kd)
     &+rzz23(i1,i2,i3)*ur2(i1,i2,i3,kd)
     &+szz23(i1,i2,i3)*us2(i1,i2,i3,kd)
     &+tzz23(i1,i2,i3)*ut2(i1,i2,i3,kd)
             
      uxy23(i1,i2,i3,kd)=
     &  rx(i1,i2,i3)*ry(i1,i2,i3)*urr2(i1,i2,i3,kd)
     & +sx(i1,i2,i3)*sy(i1,i2,i3)*uss2(i1,i2,i3,kd)
     & +tx(i1,i2,i3)*ty(i1,i2,i3)*utt2(i1,i2,i3,kd)
     & +(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))
     &                                  *urs2(i1,i2,i3,kd)
     & +(rx(i1,i2,i3)*ty(i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))
     &                                  *urt2(i1,i2,i3,kd)
     & +(sx(i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))
     &                                  *ust2(i1,i2,i3,kd)
     & +rxy23(i1,i2,i3)*ur2(i1,i2,i3,kd)
     & +sxy23(i1,i2,i3)*us2(i1,i2,i3,kd)
     & +txy23(i1,i2,i3)*ut2(i1,i2,i3,kd)
             

      uxz23(i1,i2,i3,kd)=
     &  rx(i1,i2,i3)*rz(i1,i2,i3)*urr2(i1,i2,i3,kd)
     & +sx(i1,i2,i3)*sz(i1,i2,i3)*uss2(i1,i2,i3,kd)
     & +tx(i1,i2,i3)*tz(i1,i2,i3)*utt2(i1,i2,i3,kd)
     & +(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,i2,i3)*sx(i1,i2,i3))
     &                                  *urs2(i1,i2,i3,kd)
     & +(rx(i1,i2,i3)*tz(i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))
     &                                  *urt2(i1,i2,i3,kd)
     & +(sx(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))
     &                                  *ust2(i1,i2,i3,kd)
     & +rxz23(i1,i2,i3)*ur2(i1,i2,i3,kd)
     & +sxz23(i1,i2,i3)*us2(i1,i2,i3,kd)
     & +txz23(i1,i2,i3)*ut2(i1,i2,i3,kd)
               
      uyz23(i1,i2,i3,kd)=
     &  ry(i1,i2,i3)*rz(i1,i2,i3)*urr2(i1,i2,i3,kd)
     & +sy(i1,i2,i3)*sz(i1,i2,i3)*uss2(i1,i2,i3,kd)
     & +ty(i1,i2,i3)*tz(i1,i2,i3)*utt2(i1,i2,i3,kd)
     & +(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,i2,i3)*sy(i1,i2,i3))
     &                                  *urs2(i1,i2,i3,kd)
     & +(ry(i1,i2,i3)*tz(i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))
     &                                  *urt2(i1,i2,i3,kd)
     & +(sy(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))
     &                                  *ust2(i1,i2,i3,kd)
     & +ryz23(i1,i2,i3)*ur2(i1,i2,i3,kd)
     & +syz23(i1,i2,i3)*us2(i1,i2,i3,kd)
     & +tyz23(i1,i2,i3)*ut2(i1,i2,i3,kd)
           
      laplacian22(i1,i2,i3,kd)=
     & (rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*urr2(i1,i2,i3,kd)
     &+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3))
     &                                  *urs2(i1,i2,i3,kd)
     &+(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2)*uss2(i1,i2,i3,kd)
     &     +(rxx2(i1,i2,i3)+ryy2(i1,i2,i3))*ur2(i1,i2,i3,kd)
     &     +(sxx2(i1,i2,i3)+syy2(i1,i2,i3))*us2(i1,i2,i3,kd)
            
      laplacian23(i1,i2,i3,kd)=
     & (rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(i1,i2,i3)**2)
     &                                  *urr2(i1,i2,i3,kd)
     &+(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2+sz(i1,i2,i3)**2)
     &                                  *uss2(i1,i2,i3,kd)
     &+(tx(i1,i2,i3)**2+ty(i1,i2,i3)**2+tz(i1,i2,i3)**2)
     &                                  *utt2(i1,i2,i3,kd)
     &+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)
     &    +rz(i1,i2,i3)*sz(i1,i2,i3))   *urs2(i1,i2,i3,kd)
     &+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(i1,i2,i3)*ty(i1,i2,i3)
     &    +rz(i1,i2,i3)*tz(i1,i2,i3))   *urt2(i1,i2,i3,kd)
     &+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)
     &    +sz(i1,i2,i3)*tz(i1,i2,i3))   *ust2(i1,i2,i3,kd)
     &     +(rxx23(i1,i2,i3)+ryy23(i1,i2,i3)+rzz23(i1,i2,i3))
     &                                  *ur2(i1,i2,i3,kd)
     &     +(sxx23(i1,i2,i3)+syy23(i1,i2,i3)+szz23(i1,i2,i3))
     &                                  *us2(i1,i2,i3,kd)
     &     +(txx23(i1,i2,i3)+tyy23(i1,i2,i3)+tzz23(i1,i2,i3))
     &                                  *ut2(i1,i2,i3,kd)
             
      ux22r(i1,i2,i3,kd)=(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))*h21(1)
      uy22r(i1,i2,i3,kd)=(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))*h21(2)
      uz22r(i1,i2,i3,kd)=(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))*h21(3)

      uxx22r(i1,i2,i3,kd)=(u(i1+1,i2,i3,kd)-2.*u(i1,i2,i3,kd)+
     &   u(i1-1,i2,i3,kd))*h22(1)
      uyy22r(i1,i2,i3,kd)=(u(i1,i2+1,i3,kd)-2.*u(i1,i2,i3,kd)+
     &   u(i1,i2-1,i3,kd))*h22(2)
      uzz22r(i1,i2,i3,kd)=(u(i1,i2,i3+1,kd)-2.*u(i1,i2,i3,kd)+
     &   u(i1,i2,i3-1,kd))*h22(3)


      uxy22r(i1,i2,i3,kd)= (u(i1+1,i2+1,i3,kd)-u(i1-1,i2+1,i3,kd)               
     &	  -u(i1+1,i2-1,i3,kd)+u(i1-1,i2-1,i3,kd))*(h21(1)*h21(2)) 

      uxz22r(i1,i2,i3,kd)=0.
      uyz22r(i1,i2,i3,kd)=0.


c123456789012345678901234567890123456789012345678901234567890123456789012

      ux21(i1,i2,i3,kd)= rx(i1,i2,i3)*ur2(i1,i2,i3,kd)

      ux21r(i1,i2,i3,kd)=(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))*h21(1)

      uxx21r(i1,i2,i3,kd)=(u(i1+1,i2,i3,kd)-2.*u(i1,i2,i3,kd)+
     &   u(i1-1,i2,i3,kd))*h22(1)

      ux23r(i1,i2,i3,kd)=ux22r(i1,i2,i3,kd)
      uy23r(i1,i2,i3,kd)=uy22r(i1,i2,i3,kd)
      uz23r(i1,i2,i3,kd)=uz22r(i1,i2,i3,kd)

      uxx23r(i1,i2,i3,kd)=uxx22r(i1,i2,i3,kd)
      uxy23r(i1,i2,i3,kd)=uxy22r(i1,i2,i3,kd)
      uyy23r(i1,i2,i3,kd)=uyy22r(i1,i2,i3,kd)
      uzz23r(i1,i2,i3,kd)=uzz22r(i1,i2,i3,kd)

      uxz23r(i1,i2,i3,kd)=(u(i1+1,i2,i3+1,kd)-u(i1-1,i2,i3+1,kd)               
     &	  -u(i1+1,i2,i3-1,kd)+u(i1-1,i2,i3-1,kd))*(h21(1)*h21(3)) 

      uyz23r(i1,i2,i3,kd)=(u(i1,i2+1,i3+1,kd)-u(i1,i2+1,i3-1,kd)               
     &	  -u(i1,i2-1,i3+1,kd)+u(i1,i2-1,i3-1,kd))*(h21(2)*h21(3)) 


      laplacian21(i1,i2,i3,kd)=uxx21(i1,i2,i3,kd)

      laplacian21r(i1,i2,i3,kd)=uxx21r(i1,i2,i3,kd)
      laplacian22r(i1,i2,i3,kd)=uxx22r(i1,i2,i3,kd)+uyy22r(i1,i2,i3,kd)
      laplacian23r(i1,i2,i3,kd)=uxx23r(i1,i2,i3,kd)+uyy23r(i1,i2,i3,kd)+
     & uzz23r(i1,i2,i3,kd)


      uz22(i1,i2,i3,kd)=0.
      uxz22(i1,i2,i3,kd)=0.
      uyz22(i1,i2,i3,kd)=0.
      uzz22(i1,i2,i3,kd)=0.

c     uxy21(i1,i2,i3,kd)=0.
c     uxz21(i1,i2,i3,kd)=0.
c     uyz21(i1,i2,i3,kd)=0.
      uzz21(i1,i2,i3,kd)=0.

      uy21(i1,i2,i3,kd)=0.
      uz21(i1,i2,i3,kd)=0.
      uxy21(i1,i2,i3,kd)=0.
      uyy21(i1,i2,i3,kd)=0.
      uxz21(i1,i2,i3,kd)=0.
      uyz21(i1,i2,i3,kd)=0.

      uy21r(i1,i2,i3,kd)=0.
      uz21r(i1,i2,i3,kd)=0.
      uxy21r(i1,i2,i3,kd)=0.
      uxz21r(i1,i2,i3,kd)=0.
      uyy21r(i1,i2,i3,kd)=0.
      uyz21r(i1,i2,i3,kd)=0.
      uzz21r(i1,i2,i3,kd)=0.


