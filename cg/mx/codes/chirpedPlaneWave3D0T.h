! File generated by overtureFramework/cg/mx/codes/chirpedPlaneWave.maple
! Here is the 0-th time-derivative of the chirp function in 3D
! chirp = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
! chirp_t = cpwAmp*(1/2*tanh(cpwBeta*(xi+.5*cpwTau))-1/2*tanh(cpwBeta*(xi-.5*cpwTau)))*sin(twoPi*(xi^2*cpwAlpha+cc*xi))
      t1 = .5*cpwTau
      t4 = tanh(cpwBeta*(xi+t1))
      t7 = tanh(cpwBeta*(xi-t1))
      t11 = xi ** 2
      t16 = sin(twoPi*(cc*xi+t11*cpwAlpha))
      chirp = cpwAmp*(t4/2.-t7/2.)*t16

