c
c statement functions for fourth order difference approximations
c

      ur(i1,i2,i3,kd)=(8.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))
     &                   -(u(i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd)))*d14(1)
      us(i1,i2,i3,kd)=(8.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))
     &                   -(u(i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd)))*d14(2)
      ut(i1,i2,i3,kd)=(8.*(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))
     &                   -(u(i1,i2,i3+2,kd)-u(i1,i2,i3-2,kd)))*d14(3)
c      ura(i1,i2,i3,kd)=(8.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))
c     &                    -(u(i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd)))*d14(1)
c      usa(i1,i2,i3,kd)=(8.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))
c     &                    -(u(i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd)))*d14(2)
c      uta(i1,i2,i3,kd)=(8.*(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))
c     &                    -(u(i1,i2,i3+2,kd)-u(i1,i2,i3-2,kd)))*d14(3)

      rsxyr(i1,i2,i3,kd,kdd)=
     &  (8.*(rsxy(i1+1,i2,i3,kd,kdd)-rsxy(i1-1,i2,i3,kd,kdd))
     &     -(rsxy(i1+2,i2,i3,kd,kdd)-rsxy(i1-2,i2,i3,kd,kdd)))*d14(1)
      rsxys(i1,i2,i3,kd,kdd)=
     &  (8.*(rsxy(i1,i2+1,i3,kd,kdd)-rsxy(i1,i2-1,i3,kd,kdd))
     &     -(rsxy(i1,i2+2,i3,kd,kdd)-rsxy(i1,i2-2,i3,kd,kdd)))*d14(2)
      rsxyt(i1,i2,i3,kd,kdd)=
     &  (8.*(rsxy(i1,i2,i3+1,kd,kdd)-rsxy(i1,i2,i3-1,kd,kdd))
     &     -(rsxy(i1,i2,i3+2,kd,kdd)-rsxy(i1,i2,i3-2,kd,kdd)))*d14(3)
      rxr(i1,i2,i3)=(8.*(rx(i1+1,i2,i3)-rx(i1-1,i2,i3))
     &                 -(rx(i1+2,i2,i3)-rx(i1-2,i2,i3)))*d14(1)
      ryr(i1,i2,i3)=(8.*(ry(i1+1,i2,i3)-ry(i1-1,i2,i3))
     &                 -(ry(i1+2,i2,i3)-ry(i1-2,i2,i3)))*d14(1)
      rzr(i1,i2,i3)=(8.*(rz(i1+1,i2,i3)-rz(i1-1,i2,i3))
     &                 -(rz(i1+2,i2,i3)-rz(i1-2,i2,i3)))*d14(1)
      rxs(i1,i2,i3)=(8.*(rx(i1,i2+1,i3)-rx(i1,i2-1,i3))
     &                 -(rx(i1,i2+2,i3)-rx(i1,i2-2,i3)))*d14(2)
      rys(i1,i2,i3)=(8.*(ry(i1,i2+1,i3)-ry(i1,i2-1,i3))
     &                 -(ry(i1,i2+2,i3)-ry(i1,i2-2,i3)))*d14(2)
      rzs(i1,i2,i3)=(8.*(rz(i1,i2+1,i3)-rz(i1,i2-1,i3))
     &                 -(rz(i1,i2+2,i3)-rz(i1,i2-2,i3)))*d14(2)
      rxt(i1,i2,i3)=(8.*(rx(i1,i2,i3+1)-rx(i1,i2,i3-1))
     &                 -(rx(i1,i2,i3+2)-rx(i1,i2,i3-2)))*d14(3)
      ryt(i1,i2,i3)=(8.*(ry(i1,i2,i3+1)-ry(i1,i2,i3-1))
     &                 -(ry(i1,i2,i3+2)-ry(i1,i2,i3-2)))*d14(3)
      rzt(i1,i2,i3)=(8.*(rz(i1,i2,i3+1)-rz(i1,i2,i3-1))
     &                 -(rz(i1,i2,i3+2)-rz(i1,i2,i3-2)))*d14(3)
      sxr(i1,i2,i3)=(8.*(sx(i1+1,i2,i3)-sx(i1-1,i2,i3))
     &                 -(sx(i1+2,i2,i3)-sx(i1-2,i2,i3)))*d14(1)
      syr(i1,i2,i3)=(8.*(sy(i1+1,i2,i3)-sy(i1-1,i2,i3))
     &                 -(sy(i1+2,i2,i3)-sy(i1-2,i2,i3)))*d14(1)
      szr(i1,i2,i3)=(8.*(sz(i1+1,i2,i3)-sz(i1-1,i2,i3))
     &                 -(sz(i1+2,i2,i3)-sz(i1-2,i2,i3)))*d14(1)
      sxs(i1,i2,i3)=(8.*(sx(i1,i2+1,i3)-sx(i1,i2-1,i3))
     &                 -(sx(i1,i2+2,i3)-sx(i1,i2-2,i3)))*d14(2)
      sys(i1,i2,i3)=(8.*(sy(i1,i2+1,i3)-sy(i1,i2-1,i3))
     &                 -(sy(i1,i2+2,i3)-sy(i1,i2-2,i3)))*d14(2)
      szs(i1,i2,i3)=(8.*(sz(i1,i2+1,i3)-sz(i1,i2-1,i3))
     &                 -(sz(i1,i2+2,i3)-sz(i1,i2-2,i3)))*d14(2)
      sxt(i1,i2,i3)=(8.*(sx(i1,i2,i3+1)-sx(i1,i2,i3-1))
     &                 -(sx(i1,i2,i3+2)-sx(i1,i2,i3-2)))*d14(3)
      syt(i1,i2,i3)=(8.*(sy(i1,i2,i3+1)-sy(i1,i2,i3-1))
     &                 -(sy(i1,i2,i3+2)-sy(i1,i2,i3-2)))*d14(3)
      szt(i1,i2,i3)=(8.*(sz(i1,i2,i3+1)-sz(i1,i2,i3-1))
     &                 -(sz(i1,i2,i3+2)-sz(i1,i2,i3-2)))*d14(3)
      txr(i1,i2,i3)=(8.*(tx(i1+1,i2,i3)-tx(i1-1,i2,i3))
     &                 -(tx(i1+2,i2,i3)-tx(i1-2,i2,i3)))*d14(1)
      tyr(i1,i2,i3)=(8.*(ty(i1+1,i2,i3)-ty(i1-1,i2,i3))
     &                 -(ty(i1+2,i2,i3)-ty(i1-2,i2,i3)))*d14(1)
      tzr(i1,i2,i3)=(8.*(tz(i1+1,i2,i3)-tz(i1-1,i2,i3))
     &                 -(tz(i1+2,i2,i3)-tz(i1-2,i2,i3)))*d14(1)
      txs(i1,i2,i3)=(8.*(tx(i1,i2+1,i3)-tx(i1,i2-1,i3))
     &                 -(tx(i1,i2+2,i3)-tx(i1,i2-2,i3)))*d14(2)
      tys(i1,i2,i3)=(8.*(ty(i1,i2+1,i3)-ty(i1,i2-1,i3))
     &                 -(ty(i1,i2+2,i3)-ty(i1,i2-2,i3)))*d14(2)
      tzs(i1,i2,i3)=(8.*(tz(i1,i2+1,i3)-tz(i1,i2-1,i3))
     &                 -(tz(i1,i2+2,i3)-tz(i1,i2-2,i3)))*d14(2)
      txt(i1,i2,i3)=(8.*(tx(i1,i2,i3+1)-tx(i1,i2,i3-1))
     &                 -(tx(i1,i2,i3+2)-tx(i1,i2,i3-2)))*d14(3)
      tyt(i1,i2,i3)=(8.*(ty(i1,i2,i3+1)-ty(i1,i2,i3-1))
     &                 -(ty(i1,i2,i3+2)-ty(i1,i2,i3-2)))*d14(3)
      tzt(i1,i2,i3)=(8.*(tz(i1,i2,i3+1)-tz(i1,i2,i3-1))
     &                 -(tz(i1,i2,i3+2)-tz(i1,i2,i3-2)))*d14(3)

      ux42(i1,i2,i3,kd)= rx(i1,i2,i3)*ur(i1,i2,i3,kd)
     &                +sx(i1,i2,i3)*us(i1,i2,i3,kd)
      uy42(i1,i2,i3,kd)= ry(i1,i2,i3)*ur(i1,i2,i3,kd)
     &                +sy(i1,i2,i3)*us(i1,i2,i3,kd)
      ux43(i1,i2,i3,kd)=rx(i1,i2,i3)*ur(i1,i2,i3,kd)
     &                +sx(i1,i2,i3)*us(i1,i2,i3,kd)
     &                +tx(i1,i2,i3)*ut(i1,i2,i3,kd)
      uy43(i1,i2,i3,kd)=ry(i1,i2,i3)*ur(i1,i2,i3,kd)
     &                +sy(i1,i2,i3)*us(i1,i2,i3,kd)
     &                +ty(i1,i2,i3)*ut(i1,i2,i3,kd)
      uz43(i1,i2,i3,kd)=rz(i1,i2,i3)*ur(i1,i2,i3,kd)
     &                +sz(i1,i2,i3)*us(i1,i2,i3,kd)
     &                +tz(i1,i2,i3)*ut(i1,i2,i3,kd)


      rxx(i1,i2,i3)=rx(i1,i2,i3)*rxr(i1,i2,i3)
     &             +sx(i1,i2,i3)*rxs(i1,i2,i3)
      rxy(i1,i2,i3)=rx(i1,i2,i3)*ryr(i1,i2,i3)
     &             +sx(i1,i2,i3)*rys(i1,i2,i3)
      ryy(i1,i2,i3)=ry(i1,i2,i3)*ryr(i1,i2,i3)
     &             +sy(i1,i2,i3)*rys(i1,i2,i3)
      sxx(i1,i2,i3)=rx(i1,i2,i3)*sxr(i1,i2,i3)
     &             +sx(i1,i2,i3)*sxs(i1,i2,i3)
      sxy(i1,i2,i3)=rx(i1,i2,i3)*syr(i1,i2,i3)
     &             +sx(i1,i2,i3)*sys(i1,i2,i3)
      syy(i1,i2,i3)=ry(i1,i2,i3)*syr(i1,i2,i3)
     &             +sy(i1,i2,i3)*sys(i1,i2,i3)
      rxx3(i1,i2,i3)=rx(i1,i2,i3)*rxr(i1,i2,i3)
     &              +sx(i1,i2,i3)*rxs(i1,i2,i3)
     &              +tx(i1,i2,i3)*rxt(i1,i2,i3)
      rxy3(i1,i2,i3)=rx(i1,i2,i3)*ryr(i1,i2,i3)
     &              +sx(i1,i2,i3)*rys(i1,i2,i3)
     &              +tx(i1,i2,i3)*ryt(i1,i2,i3)
      rxz3(i1,i2,i3)=rx(i1,i2,i3)*rzr(i1,i2,i3)
     &              +sx(i1,i2,i3)*rzs(i1,i2,i3)
     &              +tx(i1,i2,i3)*rzt(i1,i2,i3)
      ryy3(i1,i2,i3)=ry(i1,i2,i3)*ryr(i1,i2,i3)
     &              +sy(i1,i2,i3)*rys(i1,i2,i3)
     &              +ty(i1,i2,i3)*ryt(i1,i2,i3)
      ryz3(i1,i2,i3)=ry(i1,i2,i3)*rzr(i1,i2,i3)
     &              +sy(i1,i2,i3)*rzs(i1,i2,i3)
     &              +ty(i1,i2,i3)*rzt(i1,i2,i3)
      rzz3(i1,i2,i3)=rz(i1,i2,i3)*rzr(i1,i2,i3)
     &              +sz(i1,i2,i3)*rzs(i1,i2,i3)
     &              +tz(i1,i2,i3)*rzt(i1,i2,i3)
      sxx3(i1,i2,i3)=rx(i1,i2,i3)*sxr(i1,i2,i3)
     &              +sx(i1,i2,i3)*sxs(i1,i2,i3)
     &              +tx(i1,i2,i3)*sxt(i1,i2,i3)
      sxy3(i1,i2,i3)=rx(i1,i2,i3)*syr(i1,i2,i3)
     &              +sx(i1,i2,i3)*sys(i1,i2,i3)
     &              +tx(i1,i2,i3)*syt(i1,i2,i3)
      sxz3(i1,i2,i3)=rx(i1,i2,i3)*szr(i1,i2,i3)
     &              +sx(i1,i2,i3)*szs(i1,i2,i3)
     &              +tx(i1,i2,i3)*szt(i1,i2,i3)
      syy3(i1,i2,i3)=ry(i1,i2,i3)*syr(i1,i2,i3)
     &              +sy(i1,i2,i3)*sys(i1,i2,i3)
     &              +ty(i1,i2,i3)*syt(i1,i2,i3)
      syz3(i1,i2,i3)=ry(i1,i2,i3)*szr(i1,i2,i3)
     &              +sy(i1,i2,i3)*szs(i1,i2,i3)
     &              +ty(i1,i2,i3)*szt(i1,i2,i3)
      szz3(i1,i2,i3)=rz(i1,i2,i3)*szr(i1,i2,i3)
     &              +sz(i1,i2,i3)*szs(i1,i2,i3)
     &              +tz(i1,i2,i3)*szt(i1,i2,i3)
      txx3(i1,i2,i3)=rx(i1,i2,i3)*txr(i1,i2,i3)
     &              +sx(i1,i2,i3)*txs(i1,i2,i3)
     &              +tx(i1,i2,i3)*txt(i1,i2,i3)
      txy3(i1,i2,i3)=rx(i1,i2,i3)*tyr(i1,i2,i3)
     &              +sx(i1,i2,i3)*tys(i1,i2,i3)
     &              +tx(i1,i2,i3)*tyt(i1,i2,i3)
      txz3(i1,i2,i3)=rx(i1,i2,i3)*tzr(i1,i2,i3)
     &              +sx(i1,i2,i3)*tzs(i1,i2,i3)
     &              +tx(i1,i2,i3)*tzt(i1,i2,i3)
      tyy3(i1,i2,i3)=ry(i1,i2,i3)*tyr(i1,i2,i3)
     &              +sy(i1,i2,i3)*tys(i1,i2,i3)
     &              +ty(i1,i2,i3)*tyt(i1,i2,i3)
      tyz3(i1,i2,i3)=ry(i1,i2,i3)*tzr(i1,i2,i3)
     &              +sy(i1,i2,i3)*tzs(i1,i2,i3)
     &              +ty(i1,i2,i3)*tzt(i1,i2,i3)
      tzz3(i1,i2,i3)=rz(i1,i2,i3)*tzr(i1,i2,i3)
     &              +sz(i1,i2,i3)*tzs(i1,i2,i3)
     &              +tz(i1,i2,i3)*tzt(i1,i2,i3)
      urr(i1,i2,i3,kd)=
     & ( -30.*u(i1,i2,i3,kd)
     &  +16.*(u(i1+1,i2,i3,kd)+u(i1-1,i2,i3,kd))
     &      -(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)) )*d24(1)
      uss(i1,i2,i3,kd)=
     &+( -30.*u(i1,i2,i3,kd)
     &  +16.*(u(i1,i2+1,i3,kd)+u(i1,i2-1,i3,kd))
     &      -(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )*d24(2)
      urs(i1,i2,i3,kd)=(8.*(ur(i1,i2+1,i3,kd)-ur(i1,i2-1,i3,kd))
     &                  -(ur(i1,i2+2,i3,kd)-ur(i1,i2-2,i3,kd)))*d14(2)

      utt(i1,i2,i3,kd)=
     & ( -30.*u(i1,i2,i3,kd)
     &  +16.*(u(i1,i2,i3+1,kd)+u(i1,i2,i3-1,kd))
     &      -(u(i1,i2,i3+2,kd)+u(i1,i2,i3-2,kd)) )*d24(3)
      urt(i1,i2,i3,kd)=(8.*(ur(i1,i2,i3+1,kd)-ur(i1,i2,i3-1,kd))
     &                  -(ur(i1,i2,i3+2,kd)-ur(i1,i2,i3-2,kd)))*d14(3)
      ust(i1,i2,i3,kd)=(8.*(us(i1,i2,i3+1,kd)-us(i1,i2,i3-1,kd))
     &                  -(us(i1,i2,i3+2,kd)-us(i1,i2,i3-2,kd)))*d14(3)
      laplacian42(i1,i2,i3,kd)=
     & (rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*urr(i1,i2,i3,kd)
     &+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3))
     &  *urs(i1,i2,i3,kd)
     &+(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2)*uss(i1,i2,i3,kd)
     &     +(rxx(i1,i2,i3)+ryy(i1,i2,i3))*ur(i1,i2,i3,kd)
     &     +(sxx(i1,i2,i3)+syy(i1,i2,i3))*us(i1,i2,i3,kd)
      laplacian43(i1,i2,i3,kd)=
     & (rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(i1,i2,i3)**2)
     & *urr(i1,i2,i3,kd) 
     & +(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2+sz(i1,i2,i3)**2)
     &   *uss(i1,i2,i3,kd)
     &+(tx(i1,i2,i3)**2+ty(i1,i2,i3)**2+tz(i1,i2,i3)**2)
     &   *utt(i1,i2,i3,kd)
     &+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)
     &    +rz(i1,i2,i3)*sz(i1,i2,i3))
     &    *urs(i1,i2,i3,kd)
     &+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(i1,i2,i3)*ty(i1,i2,i3)
     &    +rz(i1,i2,i3)*tz(i1,i2,i3)) 
     &   *urt(i1,i2,i3,kd)
     &+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)
     &    +sz(i1,i2,i3)*tz(i1,i2,i3))
     &     *ust(i1,i2,i3,kd)
     &     +(rxx3(i1,i2,i3)+ryy3(i1,i2,i3)+rzz3(i1,i2,i3))
     & *ur(i1,i2,i3,kd)+(sxx3(i1,i2,i3)+syy3(i1,i2,i3)+szz3(i1,i2,i3))
     & *us(i1,i2,i3,kd)+(txx3(i1,i2,i3)+tyy3(i1,i2,i3)+tzz3(i1,i2,i3))
     &  *ut(i1,i2,i3,kd)

      uxx42(i1,i2,i3,kd)=
     & (rx(i1,i2,i3)**2                )*urr(i1,i2,i3,kd)
     &+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)                           )
     &                                  *urs(i1,i2,i3,kd)
     &+(sx(i1,i2,i3)**2                )*uss(i1,i2,i3,kd)
     &     +(rxx(i1,i2,i3)              )*ur(i1,i2,i3,kd)
     &     +(sxx(i1,i2,i3)              )*us(i1,i2,i3,kd)
      uyy42(i1,i2,i3,kd)=
     & (                ry(i1,i2,i3)**2)*urr(i1,i2,i3,kd)
     &+2.*(                           ry(i1,i2,i3)*sy(i1,i2,i3))
     &                                  *urs(i1,i2,i3,kd)
     &+(                sy(i1,i2,i3)**2)*uss(i1,i2,i3,kd)
     &     +(              ryy(i1,i2,i3))*ur(i1,i2,i3,kd)
     &     +(              syy(i1,i2,i3))*us(i1,i2,i3,kd)
      uxy42(i1,i2,i3,kd)=
     &    rx(i1,i2,i3)*ry(i1,i2,i3)*urr(i1,i2,i3,kd)
     &+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))
     &                             *urs(i1,i2,i3,kd)
     &+   sx(i1,i2,i3)*sy(i1,i2,i3)*uss(i1,i2,i3,kd)
     &  +rxy(i1,i2,i3)              *ur(i1,i2,i3,kd)
     &  +sxy(i1,i2,i3)              *us(i1,i2,i3,kd)
      uxx43(i1,i2,i3,kd)=
     &              rx(i1,i2,i3)**2*urr(i1,i2,i3,kd)
     &             +sx(i1,i2,i3)**2*uss(i1,i2,i3,kd)
     &             +tx(i1,i2,i3)**2*utt(i1,i2,i3,kd)
     &+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*urs(i1,i2,i3,kd)
     &+2.*rx(i1,i2,i3)*tx(i1,i2,i3)*urt(i1,i2,i3,kd)
     &+2.*sx(i1,i2,i3)*tx(i1,i2,i3)*ust(i1,i2,i3,kd)
     &               +rxx3(i1,i2,i3)*ur(i1,i2,i3,kd)
     &               +sxx3(i1,i2,i3)*us(i1,i2,i3,kd)
     &               +txx3(i1,i2,i3)*ut(i1,i2,i3,kd)
      uyy43(i1,i2,i3,kd)=
     &              ry(i1,i2,i3)**2*urr(i1,i2,i3,kd)
     &             +sy(i1,i2,i3)**2*uss(i1,i2,i3,kd)
     &             +ty(i1,i2,i3)**2*utt(i1,i2,i3,kd)
     &+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*urs(i1,i2,i3,kd)
     &+2.*ry(i1,i2,i3)*ty(i1,i2,i3)*urt(i1,i2,i3,kd)
     &+2.*sy(i1,i2,i3)*ty(i1,i2,i3)*ust(i1,i2,i3,kd)
     &               +ryy3(i1,i2,i3)*ur(i1,i2,i3,kd)
     &               +syy3(i1,i2,i3)*us(i1,i2,i3,kd)
     &               +tyy3(i1,i2,i3)*ut(i1,i2,i3,kd)
      uzz43(i1,i2,i3,kd)=
     &              rz(i1,i2,i3)**2*urr(i1,i2,i3,kd)
     &             +sz(i1,i2,i3)**2*uss(i1,i2,i3,kd)
     &             +tz(i1,i2,i3)**2*utt(i1,i2,i3,kd)
     &+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*urs(i1,i2,i3,kd)
     &+2.*rz(i1,i2,i3)*tz(i1,i2,i3)*urt(i1,i2,i3,kd)
     &+2.*sz(i1,i2,i3)*tz(i1,i2,i3)*ust(i1,i2,i3,kd)
     &               +rzz3(i1,i2,i3)*ur(i1,i2,i3,kd)
     &               +szz3(i1,i2,i3)*us(i1,i2,i3,kd)
     &               +tzz3(i1,i2,i3)*ut(i1,i2,i3,kd)
      uxy43(i1,i2,i3,kd)=
     &   rx(i1,i2,i3)*ry(i1,i2,i3)*urr(i1,i2,i3,kd)
     &  +sx(i1,i2,i3)*sy(i1,i2,i3)*uss(i1,i2,i3,kd)
     &  +tx(i1,i2,i3)*ty(i1,i2,i3)*utt(i1,i2,i3,kd)
     &+(rx(i1,i2,i3)*sy(i1,i2,i3)
     & +ry(i1,i2,i3)*sx(i1,i2,i3))*urs(i1,i2,i3,kd)
     &+(rx(i1,i2,i3)*ty(i1,i2,i3)
     & +ry(i1,i2,i3)*tx(i1,i2,i3))*urt(i1,i2,i3,kd)
     &+(sx(i1,i2,i3)*ty(i1,i2,i3)
     & +sy(i1,i2,i3)*tx(i1,i2,i3))*ust(i1,i2,i3,kd)
     &              +rxy3(i1,i2,i3)*ur(i1,i2,i3,kd)
     &              +sxy3(i1,i2,i3)*us(i1,i2,i3,kd)
     &              +txy3(i1,i2,i3)*ut(i1,i2,i3,kd)
      uxz43(i1,i2,i3,kd)=
     &   rx(i1,i2,i3)*rz(i1,i2,i3)*urr(i1,i2,i3,kd)
     &  +sx(i1,i2,i3)*sz(i1,i2,i3)*uss(i1,i2,i3,kd)
     &  +tx(i1,i2,i3)*tz(i1,i2,i3)*utt(i1,i2,i3,kd)
     &+(rx(i1,i2,i3)*sz(i1,i2,i3)
     & +rz(i1,i2,i3)*sx(i1,i2,i3))*urs(i1,i2,i3,kd)
     &+(rx(i1,i2,i3)*tz(i1,i2,i3)
     & +rz(i1,i2,i3)*tx(i1,i2,i3))*urt(i1,i2,i3,kd)
     &+(sx(i1,i2,i3)*tz(i1,i2,i3)
     & +sz(i1,i2,i3)*tx(i1,i2,i3))*ust(i1,i2,i3,kd)
     &              +rxz3(i1,i2,i3)*ur(i1,i2,i3,kd)
     &              +sxz3(i1,i2,i3)*us(i1,i2,i3,kd)
     &              +txz3(i1,i2,i3)*ut(i1,i2,i3,kd)
      uyz43(i1,i2,i3,kd)=
     &   ry(i1,i2,i3)*rz(i1,i2,i3)*urr(i1,i2,i3,kd)
     &  +sy(i1,i2,i3)*sz(i1,i2,i3)*uss(i1,i2,i3,kd)
     &  +ty(i1,i2,i3)*tz(i1,i2,i3)*utt(i1,i2,i3,kd)
     &+(ry(i1,i2,i3)*sz(i1,i2,i3)
     & +rz(i1,i2,i3)*sy(i1,i2,i3))*urs(i1,i2,i3,kd)
     &+(ry(i1,i2,i3)*tz(i1,i2,i3)
     & +rz(i1,i2,i3)*ty(i1,i2,i3))*urt(i1,i2,i3,kd)
     &+(sy(i1,i2,i3)*tz(i1,i2,i3)
     & +sz(i1,i2,i3)*ty(i1,i2,i3))*ust(i1,i2,i3,kd)
     &              +ryz3(i1,i2,i3)*ur(i1,i2,i3,kd)
     &              +syz3(i1,i2,i3)*us(i1,i2,i3,kd)
     &              +tyz3(i1,i2,i3)*ut(i1,i2,i3,kd)
       

      ux41(i1,i2,i3,kd)= rx(i1,i2,i3)*ur(i1,i2,i3,kd)
      uxx41(i1,i2,i3,kd)=
     & (rx(i1,i2,i3)**2                )*urr(i1,i2,i3,kd)
     &     +(rxx(i1,i2,i3)              )*ur(i1,i2,i3,kd)


c============================================================================================
c Define derivatives for a rectangular grid
c
c These definitions assume that the follwoing values are defined:
c    h41(axis) = 1./(12.*deltaX(axis))    : 41=4th order, first derivative
c    h42(axis) = 1./(12.*deltaX(axis)^2)
c============================================================================================
c ** 42 means 4th order, 2D:

      ux42r(i1,i2,i3,kd)=(8.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))  
     &               -(u(i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd)))*h41(1)
      uy42r(i1,i2,i3,kd)=(8.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))  
     &            -(u(i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd)))*h41(2) 
      uz42r(i1,i2,i3,kd)=(8.*(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd)) 
     &              -(u(i1,i2,i3+2,kd)-u(i1,i2,i3-2,kd)))*h41(3) 

      uxx42r(i1,i2,i3,kd)=( -30.*u(i1,i2,i3,kd)     
     &  +16.*(u(i1+1,i2,i3,kd)+u(i1-1,i2,i3,kd))    
     &      -(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)) )*h42(1)  

      uyy42r(i1,i2,i3,kd)=    
     & ( -30.*u(i1,i2,i3,kd)    
     &  +16.*(u(i1,i2+1,i3,kd)+u(i1,i2-1,i3,kd))    
     &      -(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )*h42(2) 

      uzz42r(i1,i2,i3,kd)=    
     & ( -30.*u(i1,i2,i3,kd)     
     &  +16.*(u(i1,i2,i3+1,kd)+u(i1,i2,i3-1,kd))     
     &      -(u(i1,i2,i3+2,kd)+u(i1,i2,i3-2,kd)) )*h42(3) 

      uxy42r(i1,i2,i3,kd)=
     &    ( (u(i1+2,i2+2,i3,kd)-u(i1-2,i2+2,i3,kd)-
     &       u(i1+2,i2-2,i3,kd)+u(i1-2,i2-2,i3,kd)) 
     &  +8.*(u(i1-1,i2+2,i3,kd)-u(i1-1,i2-2,i3,kd)-
     &       u(i1+1,i2+2,i3,kd)+u(i1+1,i2-2,i3,kd)  
     &      +u(i1+2,i2-1,i3,kd)-u(i1-2,i2-1,i3,kd)-
     &       u(i1+2,i2+1,i3,kd)+u(i1-2,i2+1,i3,kd)) 
     & +64.*(u(i1+1,i2+1,i3,kd)-u(i1-1,i2+1,i3,kd)-
     &       u(i1+1,i2-1,i3,kd)+u(i1-1,i2-1,i3,kd)) 
     &                      )*(h41(1)*h41(2)) 

      uxz42r(i1,i2,i3,kd)= 
     &    ( (u(i1+2,i2,i3+2,kd)-u(i1-2,i2,i3+2,kd)-
     &       u(i1+2,i2,i3-2,kd)+u(i1-2,i2,i3-2,kd)) 
     &  +8.*(u(i1-1,i2,i3+2,kd)-u(i1-1,i2,i3-2,kd)-
     &       u(i1+1,i2,i3+2,kd)+u(i1+1,i2,i3-2,kd)  
     &      +u(i1+2,i2,i3-1,kd)-u(i1-2,i2,i3-1,kd)-
     &       u(i1+2,i2,i3+1,kd)+u(i1-2,i2,i3+1,kd)) 
     & +64.*(u(i1+1,i2,i3+1,kd)-u(i1-1,i2,i3+1,kd)-
     &       u(i1+1,i2,i3-1,kd)+u(i1-1,i2,i3-1,kd)) 
     &           )*(h41(1)*h41(3))
                            
      uyz42r(i1,i2,i3,kd)=
     &    ( (u(i1,i2+2,i3+2,kd)-u(i1,i2-2,i3+2,kd)-
     &       u(i1,i2+2,i3-2,kd)+u(i1,i2-2,i3-2,kd)) 
     &  +8.*(u(i1,i2-1,i3+2,kd)-u(i1,i2-1,i3-2,kd)-
     &       u(i1,i2+1,i3+2,kd)+u(i1,i2+1,i3-2,kd)  
     &      +u(i1,i2+2,i3-1,kd)-u(i1,i2-2,i3-1,kd)-
     &       u(i1,i2+2,i3+1,kd)+u(i1,i2-2,i3+1,kd)) 
     & +64.*(u(i1,i2+1,i3+1,kd)-u(i1,i2-1,i3+1,kd)-
     &       u(i1,i2+1,i3-1,kd)+u(i1,i2-1,i3-1,kd)) 
     &                )*(h41(2)*h41(3)) 


      laplacian42r(i1,i2,i3,kd)=uxx42r(i1,i2,i3,kd)+
     &   uyy42r(i1,i2,i3,kd)


c here are the versions for 3d, most are the same as 2d

      ux43r(i1,i2,i3,kd)= ux42r(i1,i2,i3,kd) 
      uy43r(i1,i2,i3,kd)= uy42r(i1,i2,i3,kd) 
      uz43r(i1,i2,i3,kd)= uz42r(i1,i2,i3,kd) 
			   
      uxx43r(i1,i2,i3,kd)= uxx42r(i1,i2,i3,kd)
      uyy43r(i1,i2,i3,kd)= uyy42r(i1,i2,i3,kd)
      uzz43r(i1,i2,i3,kd)= uzz42r(i1,i2,i3,kd)
			   
      uxy43r(i1,i2,i3,kd)= uxy42r(i1,i2,i3,kd)
      uxz43r(i1,i2,i3,kd)= uxz42r(i1,i2,i3,kd)
      uyz43r(i1,i2,i3,kd)= uyz42r(i1,i2,i3,kd)
                           
      laplacian43r(i1,i2,i3,kd)=uxx42r(i1,i2,i3,kd)+uyy42r(i1,i2,i3,kd)+
     &   uzz42r(i1,i2,i3,kd) 


c here are the versions for 1d, most are the same as 2d

      ux41r(i1,i2,i3,kd)= ux42r(i1,i2,i3,kd) 
      uy41r(i1,i2,i3,kd)= uy42r(i1,i2,i3,kd) 
      uz41r(i1,i2,i3,kd)= uz42r(i1,i2,i3,kd) 
			   
      uxx41r(i1,i2,i3,kd)= uxx42r(i1,i2,i3,kd)
      uyy41r(i1,i2,i3,kd)= uyy42r(i1,i2,i3,kd)
      uzz41r(i1,i2,i3,kd)= uzz42r(i1,i2,i3,kd)
			   
      uxy41r(i1,i2,i3,kd)= uxy42r(i1,i2,i3,kd)
      uxz41r(i1,i2,i3,kd)= uxz42r(i1,i2,i3,kd)
      uyz41r(i1,i2,i3,kd)= uyz42r(i1,i2,i3,kd)
                           
      laplacian41(i1,i2,i3,kd)=uxx42(i1,i2,i3,kd)
      laplacian41r(i1,i2,i3,kd)=uxx42r(i1,i2,i3,kd)


      uz42(i1,i2,i3,kd)=0.
      uxz42(i1,i2,i3,kd)=0.
      uyz42(i1,i2,i3,kd)=0.
      uzz42(i1,i2,i3,kd)=0.

      uy41(i1,i2,i3,kd)=0.
      uz41(i1,i2,i3,kd)=0.
      uxz41(i1,i2,i3,kd)=0.
      uyz41(i1,i2,i3,kd)=0.
      uzz41(i1,i2,i3,kd)=0.

      uxy41(i1,i2,i3,kd)=0.
      uyy41(i1,i2,i3,kd)=0.
