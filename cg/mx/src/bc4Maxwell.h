

! ************ Answer *******************
      gIII=-tau1*(c22*uss+c2*us)-tau2*(c22*vss+c2*vs)

      tauU=tau1*u(i1,i2,i3,ex)+tau2*u(i1,i2,i3,ey)

      tauUp1=tau1*u(i1+is1,i2+is2,i3+is3,ex)+tau2*u(i1+is1,i2+is2,i3+is3,ey)

      tauUp2=tau1*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)+tau2*u(i1+2*is1,i2+2*is2,i3+2*is3,ey)

      tauUp3=tau1*u(i1+3*is1,i2+3*is2,i3+3*is3,ex)+tau2*u(i1+3*is1,i2+3*is2,i3+3*is3,ey)

      gIV=-6*tauU+4*tauUp1-tauUp2 +gIVf

      ttu1=-1/2.*(12*c11*tauUp1-24*c11*tauU-c11*ctlrr*tauUp2+4*c11*ctlrr*tauUp1-6*c11*ctlrr*tauU-c11*ctlrr*gIV+6*c1*dra*tauUp1-c1*dra*ctlr*tauUp2+2*c1*dra*ctlr*tauUp1+c1*dra*ctlr*gIV-12*gIII*dra**2-12*tau1DotUtt*dra**2)/(6*c11-3*c1*dra+c1*dra*ctlr)
      ttu2=-(24*c11*tauUp1-48*c11*tauU-2*c11*ctlrr*tauUp2+8*c11*ctlrr*tauUp1-12*c11*ctlrr*tauU-2*c11*ctlrr*gIV+12*c1*dra*tauUp1-2*c1*dra*ctlr*tauUp2+4*c1*dra*ctlr*tauUp1+c1*dra*ctlr*gIV-24*gIII*dra**2-24*tau1DotUtt*dra**2-6*gIV*c11+3*gIV*c1*dra)/(6*c11-3*c1*dra+c1*dra*ctlr)

      f1um2=-1/2.*b3u/dra**3-1/12.*b2u/dra**2+1/12.*b1u/dra
      f1um1=b3u/dra**3+4/3.*b2u/dra**2-2/3.*b1u/dra
      f1vm2=-1/2.*b3v/dra**3-1/12.*b2v/dra**2+1/12.*b1v/dra
      f1vm1=b3v/dra**3+4/3.*b2v/dra**2-2/3.*b1v/dra
      f1f  =-1/12.*(b1v*dra**2*u(i1+2*is1,i2+2*is2,i3,ey)-8*b1v*dra**2*u(i1+is1,i2+is2,i3,ey)+b1u*dra**2*u(i1+2*is1,i2+2*is2,i3,ex)-8*b1u*dra**2*u(i1+is1,i2+is2,i3,ex)+b2v*dra*u(i1+2*is1,i2+2*is2,i3,ey)+30*b2v*dra*u(i1,i2,i3,ey)-16*b2v*dra*u(i1+is1,i2+is2,i3,ey)+b2u*dra*u(i1+2*is1,i2+2*is2,i3,ex)+30*b2u*dra*u(i1,i2,i3,ex)-16*b2u*dra*u(i1+is1,i2+is2,i3,ex)+12*b3v*u(i1+is1,i2+is2,i3,ey)-6*b3u*u(i1+2*is1,i2+2*is2,i3,ex)+12*b3u*u(i1+is1,i2+is2,i3,ex)-6*b3v*u(i1+2*is1,i2+2*is2,i3,ey)-12*bf*dra**3)/dra**3

      f2um2=1/12.*a11m2
      f2um1=-2/3.*a11m1
      f2vm2=1/12.*a12m2
      f2vm1=-2/3.*a12m1
      f2f  =2/3.*a11p1*u(i1+is1,i2+is2,i3,ex)+2/3.*a12p1*u(i1+is1,i2+is2,i3,ey)-1/12.*a11p2*u(i1+2*is1,i2+2*is2,i3,ex)-1/12.*a12p2*u(i1+2*is1,i2+2*is2,i3,ey)-Da1DotU*dra

      u(i1-is1,i2-is2,i3,ey) = -1/(tau2**2*f1um2*f2um1-tau2**2*f1um1*f2um2+tau2*tau1*f1vm1*f2um2+tau2*f1um1*tau1*f2vm2-tau2*f1um2*tau1*f2vm1-tau1*f1vm2*tau2*f2um1-tau1**2*f1vm1*f2vm2+tau1**2*f1vm2*f2vm1)*(-tau1**2*f1f*f2vm2-f1um1*ttu1*tau1*f2vm2+tau2*f1um1*ttu1*f2um2-tau2*f1um2*f2um1*ttu1+tau1*f1vm2*f2um1*ttu1-tau2*f1um2*tau1*f2f+tau1*f1vm2*ttu2*f2um2-f1um2*tau1*f2vm2*ttu2+tau1**2*f1vm2*f2f+tau2*tau1*f1f*f2um2)

      u(i1-2*is1,i2-2*is2,i3,ey) = (tau1**2*f2f*f1vm1-tau1**2*f2vm1*f1f+tau1*f2um1*ttu1*f1vm1-tau1*f2vm1*f1um1*ttu1+tau1*tau2*f2um1*f1f-tau1*tau2*f2f*f1um1+ttu2*tau2*f1um2*f2um1-ttu2*tau2*f1um1*f2um2+ttu2*tau1*f1vm1*f2um2-ttu2*f1um2*tau1*f2vm1)/(tau2**2*f1um2*f2um1-tau2**2*f1um1*f2um2+tau2*tau1*f1vm1*f2um2+tau2*f1um1*tau1*f2vm2-tau2*f1um2*tau1*f2vm1-tau1*f1vm2*tau2*f2um1-tau1**2*f1vm1*f2vm2+tau1**2*f1vm2*f2vm1)

      u(i1-is1,i2-is2,i3,ex) = (-tau1*tau2*f1f*f2vm2-tau2**2*f1um2*f2f+f1vm2*ttu2*tau2*f2um2-tau2*f1um2*f2vm2*ttu2+tau1*f1vm2*tau2*f2f+tau2**2*f1f*f2um2+ttu1*tau2*f1vm1*f2um2-ttu1*tau2*f1um2*f2vm1-ttu1*tau1*f1vm1*f2vm2+ttu1*tau1*f1vm2*f2vm1)/(tau2**2*f1um2*f2um1-tau2**2*f1um1*f2um2+tau2*tau1*f1vm1*f2um2+tau2*f1um1*tau1*f2vm2-tau2*f1um2*tau1*f2vm1-tau1*f1vm2*tau2*f2um1-tau1**2*f1vm1*f2vm2+tau1**2*f1vm2*f2vm1)

      u(i1-2*is1,i2-2*is2,i3,ex) = (-f2vm2*ttu2*f1vm1*tau1+f2vm2*ttu2*tau2*f1um1-tau1*tau2*f2f*f1vm1+tau1*ttu2*f1vm2*f2vm1+tau1*tau2*f2vm1*f1f-tau2*f2um1*ttu1*f1vm1-ttu2*f1vm2*tau2*f2um1+tau2*f2vm1*f1um1*ttu1-tau2**2*f2um1*f1f+tau2**2*f2f*f1um1)/(tau2**2*f1um2*f2um1-tau2**2*f1um1*f2um2+tau2*tau1*f1vm1*f2um2+tau2*f1um1*tau1*f2vm2-tau2*f1um2*tau1*f2vm1-tau1*f1vm2*tau2*f2um1-tau1**2*f1vm1*f2vm2+tau1**2*f1vm2*f2vm1)


 ! *********** done *********************
