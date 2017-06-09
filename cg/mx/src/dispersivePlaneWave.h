! -*- mode: f90; -*-

! **************************************************
! Here are macros that define the:
!      dispersive plane wave solution 
! **************************************************



! *************** Here is the 2D dispersive plane wave solution ******************************

! #defineMacro planeWave2Dex0(x,y,t) sint*dpwc(0)
! #defineMacro planeWave2Dey0(x,y,t) sint*dpwc(1)
! #defineMacro planeWave2Dhz0(x,y,t) sint*dpwc() + cost*dpwc()
! 
! ! one time derivative:
! #defineMacro planeWave2Dext0(x,y,t) (-twoPi*cc)*cos(twoPi*(kx*(x)+ky*(y)-cc*(t)))*pwc(0)
! #defineMacro planeWave2Deyt0(x,y,t) (-twoPi*cc)*cos(twoPi*(kx*(x)+ky*(y)-cc*(t)))*pwc(1)
! #defineMacro planeWave2Dhzt0(x,y,t) (-twoPi*cc)*cos(twoPi*(kx*(x)+ky*(y)-cc*(t)))*pwc(5)
! 
! ! two time derivatives:
! #defineMacro planeWave2Dextt0(x,y,t) (-(twoPi*cc)**2*sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))*pwc(0))
! #defineMacro planeWave2Deytt0(x,y,t) (-(twoPi*cc)**2*sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))*pwc(1))
! #defineMacro planeWave2Dhztt0(x,y,t) (-(twoPi*cc)**2*sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))*pwc(5))
! 
! 
! ! Here are the slow start versions
! #defineMacro planeWave2Dex(x,y,t) (ssf*planeWave2Dex0(x,y,t))
! #defineMacro planeWave2Dey(x,y,t) (ssf*planeWave2Dey0(x,y,t))
! #defineMacro planeWave2Dhz(x,y,t) (ssf*planeWave2Dhz0(x,y,t))
! 
! ! one time derivative:
! #defineMacro planeWave2Dext(x,y,t) (ssf*planeWave2Dext0(x,y,t)+ssft*planeWave2Dex0(x,y,t))
! #defineMacro planeWave2Deyt(x,y,t) (ssf*planeWave2Deyt0(x,y,t)+ssft*planeWave2Dey0(x,y,t))
! #defineMacro planeWave2Dhzt(x,y,t) (ssf*planeWave2Dhzt0(x,y,t)+ssft*planeWave2Dhz0(x,y,t))

! --------------------------------------------------------------------
! Macro: Initialize values needed to eval the dispersive plane wave 
! --------------------------------------------------------------------
#beginMacro initializeDispersivePlaneWave()
  ! --- pre-calculations for the dispersive plane wave ---
  kk = twoPi*sqrt( kx*kx+ky*ky+kz*kz)
  ck2 = (c*kk)**2

  ! si=-si
  ! s^2 E = -(ck)^2 E - (s^2/eps) P --> gives P = -eps*( 1 + (ck)^2/s^2 ) E 
  sNormSq=sr**2+si**2
  sNorm4=sNormSq*sNormSq
  pc = -eps*( 2.*sr*si*ck2/sNorm4 )    ! check sign 
  ps = -eps*( 1. + ck2*(sr*sr-si*si)/sNorm4 )

  ! (1/s) * (kx*Ey - ky*Ex )/mu
  ! *check me*      
  hfactor = twoPi*( kx*pwc(1) - ky*pwc(0) )/mu  
  hs =  hfactor*si/sNormSq
  hc = -hfactor*sr/sNormSq  ! check sign 
#endMacro

! --------------------------------------------------------------------
! Macro: Evaluate the dispersive plane wave in 2D
! 
!  x,y,t (input) : point to evaluate at 
!  numberOfTimeDerivatives : evaluate this time derivative
!  ubc(.)  (output) : ubc(ex), etc. 
! --------------------------------------------------------------------
#beginMacro getDispersivePlaneWave2D(x,y,t,numberOfTimeDerivatives,ubv)

  expt = exp(sr*t) 
  xi = twoPi*(kx*(x)+ky*(y)) - si*(t)
  sinxi = sin(xi)*expt 
  cosxi = cos(xi)*expt 

  if( numberOfTimeDerivatives==0 )then
    if( polarizationOption.eq.0 )then
      ubv(ex) = sinxi*pwc(0)
      ubv(ey) = sinxi*pwc(1)
      ubv(hz) = hc*cosxi+hs*sinxi
    else
      ! polarization vector: (ex=pxc, ey=pyc) 
      ubv(ex) = (pc*cosxi+ps*sinxi)*pwc(0)
      ubv(ey) = (pc*cosxi+ps*sinxi)*pwc(1)

     ! *check me* -- just repeat hz for now 
      ubv(hz) = (hc*cosxi+hs*sinxi)*expt
    end if

  else if( numberOfTimeDerivatives==1 )then
    !write(*,'(" GDPW ntd=1 : fix me")')
    !stop 2738

    sinxip = -si*cosxi + sr*sinxi ! d(sinxi)/dt 
    cosxip =  si*sinxi + sr*cosxi ! d(cosxi)/dt 
    if( polarizationOption.eq.0 )then
      ubv(ex) = sinxip*pwc(0)
      ubv(ey) = sinxip*pwc(1)
      ubv(hz) = hc*cosxip+hs*sinxip
    else
      ! polarization vector: (ex=pxc, ey=pyc) 
      ubv(ex) = (pc*cosxip+ps*sinxip)*pwc(0)
      ubv(ey) = (pc*cosxip+ps*sinxip)*pwc(1)
      ! *check me* -- just repeat hz for now 
      ubv(hz) = hc*cosxip+hs*sinxip
    end if

  else if( numberOfTimeDerivatives==2 )then
    ! write(*,'(" GDPW ntd=2 : fix me")')
    ! stop 2738

    sinxid = -si*cosxi + sr*sinxi ! d(sinxi)/dt
    cosxid =  si*sinxi + sr*cosxi ! d(cosxi)/dt

    sinxip = -si*cosxid + sr*sinxid ! d^2(sinxi)/dt^2
    cosxip =  si*sinxid + sr*cosxid ! d^2(cosxi)/dt^2 

    if( polarizationOption.eq.0 )then
      ubv(ex) = sinxip*pwc(0)
      ubv(ey) = sinxip*pwc(1)
      ubv(hz) = hc*cosxip+hs*sinxip
    else
      ! polarization vector: (ex=pxc, ey=pyc) 
      ubv(ex) = (pc*cosxip+ps*sinxip)*pwc(0)
      ubv(ey) = (pc*cosxip+ps*sinxip)*pwc(1)
      ! *check me* -- just repeat hz for now 
      ubv(hz) = hc*cosxip+hs*sinxip
    end if

  else if( numberOfTimeDerivatives==3 )then
    write(*,'(" GDPW ntd=3 : fix me")')
    stop 2738
  else if( numberOfTimeDerivatives==4 )then
    write(*,'(" GDPW ntd=4 : fix me")')
    stop 2738
  else
    stop 2738
  end if
#endMacro


! --------------------------------------------------------------------
! Evaluate the dispersive plane wave in 3D
! 
!  x,y,z,t (input) : point to evaluate at 
!  numberOfTimeDerivatives : evaluate this time derivative
!  ubc(.)  (output) : ubc(ex), etc. 
! --------------------------------------------------------------------
#beginMacro getDispersivePlaneWave3D(x,y,z,t,numberOfTimeDerivatives,ubv)

  write(*,'(" GDPW3D : fix me")')
  stop 2739

  if( numberOfTimeDerivatives==0 )then
    ! ubv(ex) = planeWave3Dex(x,y,z,t)
    ! ubv(ey) = planeWave3Dey(x,y,z,t)
    ! ubv(ez) = planeWave3Dez(x,y,z,t)
  else if( numberOfTimeDerivatives==1 )then
    ! ubv(ex) = planeWave3Dext(x,y,z,t)
    ! ubv(ey) = planeWave3Deyt(x,y,z,t)
    ! ubv(ez) = planeWave3Dezt(x,y,z,t)
  else if( numberOfTimeDerivatives==2 )then
    ! ubv(ex) = planeWave3Dextt(x,y,z,t)
    ! ubv(ey) = planeWave3Deytt(x,y,z,t)
    ! ubv(ez) = planeWave3Deztt(x,y,z,t)
  else if( numberOfTimeDerivatives==3 )then
    ! ubv(ex) = planeWave3Dexttt(x,y,z,t)
    ! ubv(ey) = planeWave3Deyttt(x,y,z,t)
    ! ubv(ez) = planeWave3Dezttt(x,y,z,t)
  else if( numberOfTimeDerivatives==4 )then
    ! ubv(ex) = planeWave3Dextttt(x,y,z,t)
    ! ubv(ey) = planeWave3Deytttt(x,y,z,t)
    ! ubv(ez) = planeWave3Deztttt(x,y,z,t)
  else
    stop 2739
  end if

#endMacro



