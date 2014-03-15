      rsxyr2(i1,i2,i3,m,n)=(rsxy(i1+1,i2,i3,m,n)-rsxy(i1-1,i2,i3,m,n))*d12(0)
      rsxys2(i1,i2,i3,m,n)=(rsxy(i1,i2+1,i3,m,n)-rsxy(i1,i2-1,i3,m,n))*d12(1)
      rsxyt2(i1,i2,i3,m,n)=(rsxy(i1,i2,i3+1,m,n)-rsxy(i1,i2,i3-1,m,n))*d12(2)
     
      rsxyrr2(i1,i2,i3,m,n)=(rsxy(i1+1,i2,i3,m,n)-2.*rsxy(i1,i2,i3,m,n)+rsxy(i1-1,i2,i3,m,n))*d22(0)
      rsxyss2(i1,i2,i3,m,n)=(rsxy(i1,i2+1,i3,m,n)-2.*rsxy(i1,i2,i3,m,n)+rsxy(i1,i2-1,i3,m,n))*d22(1)
      rsxytt2(i1,i2,i3,m,n)=(rsxy(i1,i2,i3+1,m,n)-2.*rsxy(i1,i2,i3,m,n)+rsxy(i1,i2,i3-1,m,n))*d22(2)
     
      rsxyx22(i1,i2,i3,m,n)= rx(i1,i2,i3)*rsxyr2(i1,i2,i3,m,n)+sx(i1,i2,i3)*rsxys2(i1,i2,i3,m,n)
      rsxyy22(i1,i2,i3,m,n)= ry(i1,i2,i3)*rsxyr2(i1,i2,i3,m,n)+sy(i1,i2,i3)*rsxys2(i1,i2,i3,m,n)
     
     
      ! check these again:
      !  -- 2nd -order ---
     
      rsxyrs2(i1,i2,i3,m,n)=(rsxyr2(i1,i2+1,i3,m,n)-rsxyr2(i1,i2-1,i3,m,n))*d12(1)
      rsxyrt2(i1,i2,i3,m,n)=(rsxyr2(i1,i2,i3+1,m,n)-rsxyr2(i1,i2,i3-1,m,n))*d12(2)
      rsxyst2(i1,i2,i3,m,n)=(rsxys2(i1,i2,i3+1,m,n)-rsxys2(i1,i2,i3-1,m,n))*d12(2)
     
      rsxyxr22(i1,i2,i3,m,n)= rsxyr2(i1,i2,i3,0,0)*rsxyr2(i1,i2,i3,m,n) + rx(i1,i2,i3)*rsxyrr2(i1,i2,i3,m,n)\
                             +rsxyr2(i1,i2,i3,1,0)*rsxys2(i1,i2,i3,m,n) + sx(i1,i2,i3)*rsxyrs2(i1,i2,i3,m,n)
      rsxyxs22(i1,i2,i3,m,n)= rsxys2(i1,i2,i3,0,0)*rsxyr2(i1,i2,i3,m,n) + rx(i1,i2,i3)*rsxyrs2(i1,i2,i3,m,n)\
                             +rsxys2(i1,i2,i3,1,0)*rsxys2(i1,i2,i3,m,n) + sx(i1,i2,i3)*rsxyss2(i1,i2,i3,m,n)
     
      rsxyyr22(i1,i2,i3,m,n)= rsxyr2(i1,i2,i3,0,1)*rsxyr2(i1,i2,i3,m,n) + ry(i1,i2,i3)*rsxyrr2(i1,i2,i3,m,n)\
                             +rsxyr2(i1,i2,i3,1,1)*rsxys2(i1,i2,i3,m,n) + sy(i1,i2,i3)*rsxyrs2(i1,i2,i3,m,n)
      rsxyys22(i1,i2,i3,m,n)= rsxys2(i1,i2,i3,0,1)*rsxyr2(i1,i2,i3,m,n) + ry(i1,i2,i3)*rsxyrs2(i1,i2,i3,m,n)\
                             +rsxys2(i1,i2,i3,1,1)*rsxys2(i1,i2,i3,m,n) + sy(i1,i2,i3)*rsxyss2(i1,i2,i3,m,n)
     
      ! 3d versions -- check these again
      rsxyx23(i1,i2,i3,m,n)= rx(i1,i2,i3)*rsxyr2(i1,i2,i3,m,n)+sx(i1,i2,i3)*rsxys2(i1,i2,i3,m,n)\
                            +tx(i1,i2,i3)*rsxyt2(i1,i2,i3,m,n)
      rsxyy23(i1,i2,i3,m,n)= ry(i1,i2,i3)*rsxyr2(i1,i2,i3,m,n)+sy(i1,i2,i3)*rsxys2(i1,i2,i3,m,n)\
                            +ty(i1,i2,i3)*rsxyt2(i1,i2,i3,m,n)
      rsxyz23(i1,i2,i3,m,n)= rz(i1,i2,i3)*rsxyr2(i1,i2,i3,m,n)+sz(i1,i2,i3)*rsxys2(i1,i2,i3,m,n)\
                            +tz(i1,i2,i3)*rsxyt2(i1,i2,i3,m,n)
     
      rsxyxr23(i1,i2,i3,m,n)= rsxyr2(i1,i2,i3,0,0)*rsxyr2(i1,i2,i3,m,n) + rx(i1,i2,i3)*rsxyrr2(i1,i2,i3,m,n)\
                             +rsxyr2(i1,i2,i3,1,0)*rsxys2(i1,i2,i3,m,n) + sx(i1,i2,i3)*rsxyrs2(i1,i2,i3,m,n)\
                             +rsxyr2(i1,i2,i3,2,0)*rsxyt2(i1,i2,i3,m,n) + tx(i1,i2,i3)*rsxyrt2(i1,i2,i3,m,n)
     
      rsxyxs23(i1,i2,i3,m,n)= rsxys2(i1,i2,i3,0,0)*rsxyr2(i1,i2,i3,m,n) + rx(i1,i2,i3)*rsxyrs2(i1,i2,i3,m,n)\
                             +rsxys2(i1,i2,i3,1,0)*rsxys2(i1,i2,i3,m,n) + sx(i1,i2,i3)*rsxyss2(i1,i2,i3,m,n)\
                             +rsxys2(i1,i2,i3,2,0)*rsxyt2(i1,i2,i3,m,n) + tx(i1,i2,i3)*rsxyst2(i1,i2,i3,m,n)
     
      rsxyxt23(i1,i2,i3,m,n)= rsxyt2(i1,i2,i3,0,0)*rsxyr2(i1,i2,i3,m,n) + rx(i1,i2,i3)*rsxyrt2(i1,i2,i3,m,n)\
                             +rsxyt2(i1,i2,i3,1,0)*rsxys2(i1,i2,i3,m,n) + sx(i1,i2,i3)*rsxyst2(i1,i2,i3,m,n)\
                             +rsxyt2(i1,i2,i3,2,0)*rsxyt2(i1,i2,i3,m,n) + tx(i1,i2,i3)*rsxytt2(i1,i2,i3,m,n)
     
      rsxyyr23(i1,i2,i3,m,n)= rsxyr2(i1,i2,i3,0,1)*rsxyr2(i1,i2,i3,m,n) + ry(i1,i2,i3)*rsxyrr2(i1,i2,i3,m,n)\
                             +rsxyr2(i1,i2,i3,1,1)*rsxys2(i1,i2,i3,m,n) + sy(i1,i2,i3)*rsxyrs2(i1,i2,i3,m,n)\
                             +rsxyr2(i1,i2,i3,2,1)*rsxyt2(i1,i2,i3,m,n) + ty(i1,i2,i3)*rsxyrt2(i1,i2,i3,m,n)
     
      rsxyys23(i1,i2,i3,m,n)= rsxys2(i1,i2,i3,0,1)*rsxyr2(i1,i2,i3,m,n) + ry(i1,i2,i3)*rsxyrs2(i1,i2,i3,m,n)\
                             +rsxys2(i1,i2,i3,1,1)*rsxys2(i1,i2,i3,m,n) + sy(i1,i2,i3)*rsxyss2(i1,i2,i3,m,n)\
                             +rsxys2(i1,i2,i3,2,1)*rsxyt2(i1,i2,i3,m,n) + ty(i1,i2,i3)*rsxyst2(i1,i2,i3,m,n)
     
      rsxyyt23(i1,i2,i3,m,n)= rsxyt2(i1,i2,i3,0,1)*rsxyr2(i1,i2,i3,m,n) + ry(i1,i2,i3)*rsxyrt2(i1,i2,i3,m,n)\
                             +rsxyt2(i1,i2,i3,1,1)*rsxys2(i1,i2,i3,m,n) + sy(i1,i2,i3)*rsxyst2(i1,i2,i3,m,n)\
                             +rsxyt2(i1,i2,i3,2,1)*rsxyt2(i1,i2,i3,m,n) + ty(i1,i2,i3)*rsxytt2(i1,i2,i3,m,n)
     
      rsxyzr23(i1,i2,i3,m,n)= rsxyr2(i1,i2,i3,0,2)*rsxyr2(i1,i2,i3,m,n) + rz(i1,i2,i3)*rsxyrr2(i1,i2,i3,m,n)\
                             +rsxyr2(i1,i2,i3,1,2)*rsxys2(i1,i2,i3,m,n) + sz(i1,i2,i3)*rsxyrs2(i1,i2,i3,m,n)\
                             +rsxyr2(i1,i2,i3,2,2)*rsxyt2(i1,i2,i3,m,n) + tz(i1,i2,i3)*rsxyrt2(i1,i2,i3,m,n)
     
      rsxyzs23(i1,i2,i3,m,n)= rsxys2(i1,i2,i3,0,2)*rsxyr2(i1,i2,i3,m,n) + rz(i1,i2,i3)*rsxyrs2(i1,i2,i3,m,n)\
                             +rsxys2(i1,i2,i3,1,2)*rsxys2(i1,i2,i3,m,n) + sz(i1,i2,i3)*rsxyss2(i1,i2,i3,m,n)\
                             +rsxys2(i1,i2,i3,2,2)*rsxyt2(i1,i2,i3,m,n) + tz(i1,i2,i3)*rsxyst2(i1,i2,i3,m,n)
     
      rsxyzt23(i1,i2,i3,m,n)= rsxyt2(i1,i2,i3,0,2)*rsxyr2(i1,i2,i3,m,n) + rz(i1,i2,i3)*rsxyrt2(i1,i2,i3,m,n)\
                             +rsxyt2(i1,i2,i3,1,2)*rsxys2(i1,i2,i3,m,n) + sz(i1,i2,i3)*rsxyst2(i1,i2,i3,m,n)\
                             +rsxyt2(i1,i2,i3,2,2)*rsxyt2(i1,i2,i3,m,n) + tz(i1,i2,i3)*rsxytt2(i1,i2,i3,m,n)
     
      ! ---- 4th order ---
     
      rsxyr4(i1,i2,i3,m,n)=(8.*(rsxy(i1+1,i2,i3,m,n)-rsxy(i1-1,i2,i3,m,n))\
                              -(rsxy(i1+2,i2,i3,m,n)-rsxy(i1-2,i2,i3,m,n)))*d14(0)
      rsxys4(i1,i2,i3,m,n)=(8.*(rsxy(i1,i2+1,i3,m,n)-rsxy(i1,i2-1,i3,m,n))\
                              -(rsxy(i1,i2+2,i3,m,n)-rsxy(i1,i2-2,i3,m,n)))*d14(1)
      rsxyt4(i1,i2,i3,m,n)=(8.*(rsxy(i1,i2,i3+1,m,n)-rsxy(i1,i2,i3-1,m,n))\
                              -(rsxy(i1,i2,i3+2,m,n)-rsxy(i1,i2,i3-2,m,n)))*d14(2)
     
      rsxyrr4(i1,i2,i3,m,n)=(-30.*rsxy(i1,i2,i3,m,n)+16.*(rsxy(i1+1,i2,i3,m,n)+rsxy(i1-1,i2,i3,m,n))\
                                -(rsxy(i1+2,i2,i3,m,n)+rsxy(i1-2,i2,i3,m,n)) )*d24(0)
     
      rsxyss4(i1,i2,i3,m,n)=(-30.*rsxy(i1,i2,i3,m,n)+16.*(rsxy(i1,i2+1,i3,m,n)+rsxy(i1,i2-1,i3,m,n))\
                                -(rsxy(i1,i2+2,i3,m,n)+rsxy(i1,i2-2,i3,m,n)) )*d24(1)
     
      rsxytt4(i1,i2,i3,m,n)=(-30.*rsxy(i1,i2,i3,m,n)+16.*(rsxy(i1,i2,i3+1,m,n)+rsxy(i1,i2,i3-1,m,n))\
                                -(rsxy(i1,i2,i3+2,m,n)+rsxy(i1,i2,i3-2,m,n)) )*d24(2)
     
      rsxyrs4(i1,i2,i3,m,n)=(8.*(rsxyr4(i1,i2+1,i3,m,n)-rsxyr4(i1,i2-1,i3,m,n))\
                               -(rsxyr4(i1,i2+2,i3,m,n)-rsxyr4(i1,i2-2,i3,m,n)))*d14(1)
     
      rsxyrt4(i1,i2,i3,m,n)=(8.*(rsxyr4(i1,i2,i3+1,m,n)-rsxyr4(i1,i2,i3-1,m,n))\
                               -(rsxyr4(i1,i2,i3+2,m,n)-rsxyr4(i1,i2,i3-2,m,n)))*d14(2)
     
      rsxyst4(i1,i2,i3,m,n)=(8.*(rsxys4(i1,i2,i3+1,m,n)-rsxys4(i1,i2,i3-1,m,n))\
                               -(rsxys4(i1,i2,i3+2,m,n)-rsxys4(i1,i2,i3-2,m,n)))*d14(2)
     
      rsxyx42(i1,i2,i3,m,n)= rx(i1,i2,i3)*rsxyr4(i1,i2,i3,m,n)+sx(i1,i2,i3)*rsxys4(i1,i2,i3,m,n)
      rsxyy42(i1,i2,i3,m,n)= ry(i1,i2,i3)*rsxyr4(i1,i2,i3,m,n)+sy(i1,i2,i3)*rsxys4(i1,i2,i3,m,n)
     
      rsxyxr42(i1,i2,i3,m,n)= rsxyr4(i1,i2,i3,0,0)*rsxyr4(i1,i2,i3,m,n) + rx(i1,i2,i3)*rsxyrr4(i1,i2,i3,m,n)\
                             +rsxyr4(i1,i2,i3,1,0)*rsxys4(i1,i2,i3,m,n) + sx(i1,i2,i3)*rsxyrs4(i1,i2,i3,m,n)
      rsxyxs42(i1,i2,i3,m,n)= rsxys4(i1,i2,i3,0,0)*rsxyr4(i1,i2,i3,m,n) + rx(i1,i2,i3)*rsxyrs4(i1,i2,i3,m,n)\
                             +rsxys4(i1,i2,i3,1,0)*rsxys4(i1,i2,i3,m,n) + sx(i1,i2,i3)*rsxyss4(i1,i2,i3,m,n)
     
      rsxyyr42(i1,i2,i3,m,n)= rsxyr4(i1,i2,i3,0,1)*rsxyr4(i1,i2,i3,m,n) + ry(i1,i2,i3)*rsxyrr4(i1,i2,i3,m,n)\
                             +rsxyr4(i1,i2,i3,1,1)*rsxys4(i1,i2,i3,m,n) + sy(i1,i2,i3)*rsxyrs4(i1,i2,i3,m,n)
      rsxyys42(i1,i2,i3,m,n)= rsxys4(i1,i2,i3,0,1)*rsxyr4(i1,i2,i3,m,n) + ry(i1,i2,i3)*rsxyrs4(i1,i2,i3,m,n)\
                             +rsxys4(i1,i2,i3,1,1)*rsxys4(i1,i2,i3,m,n) + sy(i1,i2,i3)*rsxyss4(i1,i2,i3,m,n)

      rsxyx43(i1,i2,i3,m,n)= rx(i1,i2,i3)*rsxyr4(i1,i2,i3,m,n)+sx(i1,i2,i3)*rsxys4(i1,i2,i3,m,n)\
                            +tx(i1,i2,i3)*rsxyt4(i1,i2,i3,m,n)
      rsxyy43(i1,i2,i3,m,n)= ry(i1,i2,i3)*rsxyr4(i1,i2,i3,m,n)+sy(i1,i2,i3)*rsxys4(i1,i2,i3,m,n)\
                            +ty(i1,i2,i3)*rsxyt4(i1,i2,i3,m,n)
      rsxyz43(i1,i2,i3,m,n)= rz(i1,i2,i3)*rsxyr4(i1,i2,i3,m,n)+sz(i1,i2,i3)*rsxys4(i1,i2,i3,m,n)\
                            +tz(i1,i2,i3)*rsxyt4(i1,i2,i3,m,n)
     
      rsxyxr43(i1,i2,i3,m,n)= rsxyr4(i1,i2,i3,0,0)*rsxyr4(i1,i2,i3,m,n) + rx(i1,i2,i3)*rsxyrr4(i1,i2,i3,m,n)\
                             +rsxyr4(i1,i2,i3,1,0)*rsxys4(i1,i2,i3,m,n) + sx(i1,i2,i3)*rsxyrs4(i1,i2,i3,m,n)\
                             +rsxyr4(i1,i2,i3,2,0)*rsxyt4(i1,i2,i3,m,n) + tx(i1,i2,i3)*rsxyrt4(i1,i2,i3,m,n)
     
      rsxyxs43(i1,i2,i3,m,n)= rsxys4(i1,i2,i3,0,0)*rsxyr4(i1,i2,i3,m,n) + rx(i1,i2,i3)*rsxyrs4(i1,i2,i3,m,n)\
                             +rsxys4(i1,i2,i3,1,0)*rsxys4(i1,i2,i3,m,n) + sx(i1,i2,i3)*rsxyss4(i1,i2,i3,m,n)\
                             +rsxys4(i1,i2,i3,2,0)*rsxyt4(i1,i2,i3,m,n) + tx(i1,i2,i3)*rsxyst4(i1,i2,i3,m,n)
     
      rsxyxt43(i1,i2,i3,m,n)= rsxyt4(i1,i2,i3,0,0)*rsxyr4(i1,i2,i3,m,n) + rx(i1,i2,i3)*rsxyrt4(i1,i2,i3,m,n)\
                             +rsxyt4(i1,i2,i3,1,0)*rsxys4(i1,i2,i3,m,n) + sx(i1,i2,i3)*rsxyst4(i1,i2,i3,m,n)\
                             +rsxyt4(i1,i2,i3,2,0)*rsxyt4(i1,i2,i3,m,n) + tx(i1,i2,i3)*rsxytt4(i1,i2,i3,m,n)
     
      rsxyyr43(i1,i2,i3,m,n)= rsxyr4(i1,i2,i3,0,1)*rsxyr4(i1,i2,i3,m,n) + ry(i1,i2,i3)*rsxyrr4(i1,i2,i3,m,n)\
                             +rsxyr4(i1,i2,i3,1,1)*rsxys4(i1,i2,i3,m,n) + sy(i1,i2,i3)*rsxyrs4(i1,i2,i3,m,n)\
                             +rsxyr4(i1,i2,i3,2,1)*rsxyt4(i1,i2,i3,m,n) + ty(i1,i2,i3)*rsxyrt4(i1,i2,i3,m,n)
     
      rsxyys43(i1,i2,i3,m,n)= rsxys4(i1,i2,i3,0,1)*rsxyr4(i1,i2,i3,m,n) + ry(i1,i2,i3)*rsxyrs4(i1,i2,i3,m,n)\
                             +rsxys4(i1,i2,i3,1,1)*rsxys4(i1,i2,i3,m,n) + sy(i1,i2,i3)*rsxyss4(i1,i2,i3,m,n)\
                             +rsxys4(i1,i2,i3,2,1)*rsxyt4(i1,i2,i3,m,n) + ty(i1,i2,i3)*rsxyst4(i1,i2,i3,m,n)
     
      rsxyyt43(i1,i2,i3,m,n)= rsxyt4(i1,i2,i3,0,1)*rsxyr4(i1,i2,i3,m,n) + ry(i1,i2,i3)*rsxyrt4(i1,i2,i3,m,n)\
                             +rsxyt4(i1,i2,i3,1,1)*rsxys4(i1,i2,i3,m,n) + sy(i1,i2,i3)*rsxyst4(i1,i2,i3,m,n)\
                             +rsxyt4(i1,i2,i3,2,1)*rsxyt4(i1,i2,i3,m,n) + ty(i1,i2,i3)*rsxytt4(i1,i2,i3,m,n)
     
      rsxyzr43(i1,i2,i3,m,n)= rsxyr4(i1,i2,i3,0,2)*rsxyr4(i1,i2,i3,m,n) + rz(i1,i2,i3)*rsxyrr4(i1,i2,i3,m,n)\
                             +rsxyr4(i1,i2,i3,1,2)*rsxys4(i1,i2,i3,m,n) + sz(i1,i2,i3)*rsxyrs4(i1,i2,i3,m,n)\
                             +rsxyr4(i1,i2,i3,2,2)*rsxyt4(i1,i2,i3,m,n) + tz(i1,i2,i3)*rsxyrt4(i1,i2,i3,m,n)
     
      rsxyzs43(i1,i2,i3,m,n)= rsxys4(i1,i2,i3,0,2)*rsxyr4(i1,i2,i3,m,n) + rz(i1,i2,i3)*rsxyrs4(i1,i2,i3,m,n)\
                             +rsxys4(i1,i2,i3,1,2)*rsxys4(i1,i2,i3,m,n) + sz(i1,i2,i3)*rsxyss4(i1,i2,i3,m,n)\
                             +rsxys4(i1,i2,i3,2,2)*rsxyt4(i1,i2,i3,m,n) + tz(i1,i2,i3)*rsxyst4(i1,i2,i3,m,n)
     
      rsxyzt43(i1,i2,i3,m,n)= rsxyt4(i1,i2,i3,0,2)*rsxyr4(i1,i2,i3,m,n) + rz(i1,i2,i3)*rsxyrt4(i1,i2,i3,m,n)\
                             +rsxyt4(i1,i2,i3,1,2)*rsxys4(i1,i2,i3,m,n) + sz(i1,i2,i3)*rsxyst4(i1,i2,i3,m,n)\
                             +rsxyt4(i1,i2,i3,2,2)*rsxyt4(i1,i2,i3,m,n) + tz(i1,i2,i3)*rsxytt4(i1,i2,i3,m,n)
     
