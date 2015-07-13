! ************ Results from mx/codes/bc4.maple *******************
      gIII=-tau1*(c2*us+c22*uss)-tau2*(c2*vs+c22*vss)

      tauU=tau1*u(i1,i2,i3,ex)+tau2*u(i1,i2,i3,ey)

      tauUp1=tau1*u(i1+is1,i2+is2,i3+is3,ex)+tau2*u(i1+is1,i2+is2,i3+is3,ey)

      tauUp2=tau1*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)+tau2*u(i1+2*is1,i2+2*is2,i3+2*is3,ey)

      tauUp3=tau1*u(i1+3*is1,i2+3*is2,i3+3*is3,ex)+tau2*u(i1+3*is1,i2+3*is2,i3+3*is3,ey)

      gIV=-10*tauU+10*tauUp1-5*tauUp2+tauUp3 +gIVf

      ttu1=-1/(3*c1*ctlr*dra-6*c1*dra-c11*ctlrr+12*c11)*(c1*ctlr*dra*gIV+2*c1*ctlr*dra*tauUp1-c1*ctlr*dra*tauUp2+6*c1*dra*tauUp1-c11*ctlrr*gIV-6*c11*ctlrr*tauU+4*c11*ctlrr*tauUp1-c11*ctlrr*tauUp2-12*dra**2*gIII-12*dra**2*tau1DotUtt-24*c11*tauU+12*c11*tauUp1)
      ttu2=-(2*c1*ctlr*dra*gIV+10*c1*ctlr*dra*tauUp1-5*c1*ctlr*dra*tauUp2+6*c1*dra*gIV+30*c1*dra*tauUp1-4*c11*ctlrr*gIV-30*c11*ctlrr*tauU+20*c11*ctlrr*tauUp1-5*c11*ctlrr*tauUp2-60*dra**2*gIII-60*dra**2*tau1DotUtt-12*c11*gIV-120*c11*tauU+60*c11*tauUp1)/(3*c1*ctlr*dra-6*c1*dra-c11*ctlrr+12*c11)

      f1um2=-1/2.*b3u/dra**3-1/12.*b2u/dra**2+1/12.*b1u/dra
      f1um1=b3u/dra**3+4/3.*b2u/dra**2-2/3.*b1u/dra
      f1vm2=-1/2.*b3v/dra**3-1/12.*b2v/dra**2+1/12.*b1v/dra
      f1vm1=b3v/dra**3+4/3.*b2v/dra**2-2/3.*b1v/dra
      f1f  =-1/12.*(b1u*dra**2*u(i1+2*is1,i2+2*is2,i3,ex)-8*b1u*dra**2*u(i1+is1,i2+is2,i3,ex)+b1v*dra**2*u(i1+2*is1,i2+2*is2,i3,ey)-8*b1v*dra**2*u(i1+is1,i2+is2,i3,ey)-12*bf*dra**3+b2u*dra*u(i1+2*is1,i2+2*is2,i3,ex)-16*b2u*dra*u(i1+is1,i2+is2,i3,ex)+b2v*dra*u(i1+2*is1,i2+2*is2,i3,ey)-16*b2v*dra*u(i1+is1,i2+is2,i3,ey)+30*b2u*dra*u(i1,i2,i3,ex)+30*b2v*dra*u(i1,i2,i3,ey)-6*b3u*u(i1+2*is1,i2+2*is2,i3,ex)+12*b3u*u(i1+is1,i2+is2,i3,ex)-6*b3v*u(i1+2*is1,i2+2*is2,i3,ey)+12*b3v*u(i1+is1,i2+is2,i3,ey))/dra**3

      f2um2=1/12.*a11m2
      f2um1=-2/3.*a11m1
      f2vm2=1/12.*a12m2
      f2vm1=-2/3.*a12m1
      f2f  =2/3.*a11p1*u(i1+is1,i2+is2,i3,ex)+2/3.*a12p1*u(i1+is1,i2+is2,i3,ey)-1/12.*a11p2*u(i1+2*is1,i2+2*is2,i3,ex)-1/12.*a12p2*u(i1+2*is1,i2+2*is2,i3,ey)-Da1DotU*dra

      u(i1-2*is1,i2-2*is2,i3,ex) = (f1f*f2um1*tau2**2-f1f*f2vm1*tau1*tau2-f1um1*f2f*tau2**2-f1um1*f2vm1*tau2*ttu1-f1um1*f2vm2*tau2*ttu2+f1vm1*f2f*tau1*tau2+f1vm1*f2um1*tau2*ttu1+f1vm1*f2vm2*tau1*ttu2+f1vm2*f2um1*tau2*ttu2-f1vm2*f2vm1*tau1*ttu2)/(f1um1*f2um2*tau2**2-f1um1*f2vm2*tau1*tau2-f1um2*f2um1*tau2**2+f1um2*f2vm1*tau1*tau2-f1vm1*f2um2*tau1*tau2+f1vm1*f2vm2*tau1**2+f1vm2*f2um1*tau1*tau2-f1vm2*f2vm1*tau1**2)

      u(i1-is1,i2-is2,i3,ex) = -(f1f*f2um2*tau2**2-f1f*f2vm2*tau1*tau2-f1um2*f2f*tau2**2-f1um2*f2vm1*tau2*ttu1-f1um2*f2vm2*tau2*ttu2+f1vm1*f2um2*tau2*ttu1-f1vm1*f2vm2*tau1*ttu1+f1vm2*f2f*tau1*tau2+f1vm2*f2um2*tau2*ttu2+f1vm2*f2vm1*tau1*ttu1)/(f1um1*f2um2*tau2**2-f1um1*f2vm2*tau1*tau2-f1um2*f2um1*tau2**2+f1um2*f2vm1*tau1*tau2-f1vm1*f2um2*tau1*tau2+f1vm1*f2vm2*tau1**2+f1vm2*f2um1*tau1*tau2-f1vm2*f2vm1*tau1**2)

      u(i1-2*is1,i2-2*is2,i3,ey) = -(f1f*f2um1*tau1*tau2-f1f*f2vm1*tau1**2-f1um1*f2f*tau1*tau2-f1um1*f2um2*tau2*ttu2-f1um1*f2vm1*tau1*ttu1+f1um2*f2um1*tau2*ttu2-f1um2*f2vm1*tau1*ttu2+f1vm1*f2f*tau1**2+f1vm1*f2um1*tau1*ttu1+f1vm1*f2um2*tau1*ttu2)/(f1um1*f2um2*tau2**2-f1um1*f2vm2*tau1*tau2-f1um2*f2um1*tau2**2+f1um2*f2vm1*tau1*tau2-f1vm1*f2um2*tau1*tau2+f1vm1*f2vm2*tau1**2+f1vm2*f2um1*tau1*tau2-f1vm2*f2vm1*tau1**2)

      u(i1-is1,i2-is2,i3,ey) = (f1f*f2um2*tau1*tau2-f1f*f2vm2*tau1**2+f1um1*f2um2*tau2*ttu1-f1um1*f2vm2*tau1*ttu1-f1um2*f2f*tau1*tau2-f1um2*f2um1*tau2*ttu1-f1um2*f2vm2*tau1*ttu2+f1vm2*f2f*tau1**2+f1vm2*f2um1*tau1*ttu1+f1vm2*f2um2*tau1*ttu2)/(f1um1*f2um2*tau2**2-f1um1*f2vm2*tau1*tau2-f1um2*f2um1*tau2**2+f1um2*f2vm1*tau1*tau2-f1vm1*f2um2*tau1*tau2+f1vm1*f2vm2*tau1**2+f1vm2*f2um1*tau1*tau2-f1vm2*f2vm1*tau1**2)


 ! *********** done *********************
