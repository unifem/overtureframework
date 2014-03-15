

! ************ Hz Answer *******************
      cw2=c1+c11r
      cw1=c1r
      bfw2=c22r*wss+c2r*ws-fw2

      u(i1-is1,i2-is2,i3,hz) = 1/2.*(-18*c11*u(i1+is1,i2+is2,i3,hz)+36*c11*fw1*dra-12*cw2*dra*u(i1+is1,i2+is2,i3,hz)+15*cw2*dra*u(i1,i2,i3,hz)+cw2*dra*u(i1+2*is1,i2+2*is2,i3,hz)+6*cw2*dra**2*fw1-6*cw1*dra**3*fw1-6*bfw2*dra**3)/(-9*c11+2*cw2*dra)

      u(i1-2*is1,i2-2*is2,i3,hz) = (-64*cw2*dra*u(i1+is1,i2+is2,i3,hz)+36*c11*fw1*dra+60*cw2*dra*u(i1,i2,i3,hz)+6*cw2*dra*u(i1+2*is1,i2+2*is2,i3,hz)+48*cw2*dra**2*fw1-24*cw1*dra**3*fw1-24*bfw2*dra**3-9*c11*u(i1+2*is1,i2+2*is2,i3,hz))/(-9*c11+2*cw2*dra)


 ! *********** Hz done *********************
