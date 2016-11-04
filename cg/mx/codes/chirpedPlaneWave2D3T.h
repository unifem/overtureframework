! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 3-th time-derivative of the chirp function in 2D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(-cpwBeta^3*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)^2+2*cpwBeta^3*tanh(cpwBeta*(xi+.5*cpwTau))^2*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^3*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2)^2-2*cpwBeta^3*tanh(cpwBeta*(xi-.5*cpwTau))^2*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))+3*cpwAmp*(-cpwBeta^2*tanh(cpwBeta*(xi+.5*cpwTau))*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)+cpwBeta^2*tanh(cpwBeta*(xi-.5*cpwTau))*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*(2*xi*cpwAlpha+cc)*cos(twoPi*(xi^2*cpwAlpha+cc*xi))+6*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi*cpwAlpha*cos(twoPi*(xi^2*cpwAlpha+cc*xi))-3*cpwAmp*(1/2*cpwBeta*(1-tanh(cpwBeta*(xi+.5*cpwTau))^2)-1/2*cpwBeta*(1-tanh(cpwBeta*(xi-.5*cpwTau))^2))*twoPi^2*(2*xi*cpwAlpha+cc)^2*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-6*cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^2*cpwAlpha*(2*xi*cpwAlpha+cc)*sin(twoPi*(xi^2*cpwAlpha+cc*xi))-cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*twoPi^3*(2*xi*cpwAlpha+cc)^3*cos(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = cpwBeta ** 2
      t2 = t1*cpwBeta
      t3 = .5*cpwTau
      t6 = tanh(cpwBeta*(xi+t3))
      t7 = t6 ** 2
      t8 = 1.-t7
      t9 = t8 ** 2
      t16 = tanh(cpwBeta*(xi-t3))
      t17 = t16 ** 2
      t18 = 1.-t17
      t19 = t18 ** 2
      t26 = xi ** 2
      t30 = twoPi*(cc*xi+t26*cpwAlpha)
      t31 = sin(t30)
      t41 = 2.*xi*cpwAlpha+cc
      t43 = cos(t30)
      t51 = cpwAmp*(-cpwBeta*t18/2.+cpwBeta*t8/2.)
      t56 = twoPi ** 2
      t57 = t41 ** 2
      t64 = cpwAmp*(t6/2.-t16/2.)
      chirp = cpwAmp*(-2.*t2*t17*t18+2.*t2*t7*t8+t2*t19-t2*t9)*t31+0.3E1*cpwAmp*(t1*t16*t18-t1*t6*t8)*twoPi*t41*t43+6.*t51*twoPi*cpwAlpha*t43-0.3E1*t51*t56*t57*t31-6.*t64*t56*cpwAlpha*t41*t31-t64*t56*twoPi*t57*t41*t43
