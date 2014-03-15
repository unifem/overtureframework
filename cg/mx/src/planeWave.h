c **************************************************
c Here are macros that define the:
c      planeWave solution 
c **************************************************

c ======================================================================
c  Slow start function 
c    tba = length of slow start interval (<0 mean no slow start)
c ======================================================================

c cubic ramp
c tba=max(REAL_EPSILON,tb-ta);
c dta=t-ta;
	  
c This (cubic) ramp has 1-derivative zero at t=0 and t=tba
#defineMacro ramp3(t,tba)  (t)*(t)*( -(t)/3.+.5*tba )*6./(tba*tba*tba)
#defineMacro ramp3t(t,tba) (t)*( -(t) + tba )*6./(tba*tba*tba)
#defineMacro ramp3tt(t,tba) ( -2.*(t) + tba )*6./(tba*tba*tba)
#defineMacro ramp3ttt(t,tba) ( -2. )*6./(tba*tba*tba)

c This ramp has 3-derivatives zero at t=0 and t=1
c This is from ramp.maple
c r=-84*t**5+35*t**4-20*t**7+70*t**6
c rt=-420*t**4+140*t**3-140*t**6+420*t**5
c rtt=-1680*t**3+420*t**2-840*t**5+2100*t**4
c rttt=-5040*t**2+840*t-4200*t**4+8400*t**3

#defineMacro ramp(t)    ( -84*(t)**5+35*(t)**4-20*(t)**7+70*(t)**6 )
#defineMacro rampt(t)   ( -420*(t)**4+140*(t)**3-140*(t)**6+420*(t)**5 )
#defineMacro ramptt(t)  ( -1680*(t)**3+420*(t)**2-840*(t)**5+2100*(t)**4 )
#defineMacro rampttt(t) ( -5040*(t)**2+840*(t)-4200*(t)**4+8400*(t)**3 )

c This ramp has 4-derivatives zero at t=0 and t=1
c This is from ramp.maple
c r=126*(t)**5-315*(t)**8+70*(t)**9-420*(t)**6+540*(t)**7
c rt=630*(t)**4-2520*(t)**7+630*(t)**8-2520*(t)**5+3780*(t)**6
c rtt=2520*(t)**3-17640*(t)**6+5040*(t)**7-12600*(t)**4+22680*(t)**5
c rttt=7560*(t)**2-105840*(t)**5+35280*(t)**6-50400*(t)**3+113400*(t)**4

#defineMacro ramp4(t)    ( 126*(t)**5-315*(t)**8+70*(t)**9-420*(t)**6+540*(t)**7 )              
#defineMacro ramp4t(t)   ( 630*(t)**4-2520*(t)**7+630*(t)**8-2520*(t)**5+3780*(t)**6 )         
#defineMacro ramp4tt(t)  ( 2520*(t)**3-17640*(t)**6+5040*(t)**7-12600*(t)**4+22680*(t)**5 )   
#defineMacro ramp4ttt(t) ( 7560*(t)**2-105840*(t)**5+35280*(t)**6-50400*(t)**3+113400*(t)**4 )
#defineMacro ramp4tttt(t) ( 15120*(t)-529200*(t)**4+211680*(t)**5-151200*(t)**2+453600*(t)**3 )

c ============================================================
c  Initialize parameters for the boundary forcing
c   tba: slow start time interval -- no slow start if this is negative
c ===========================================================
#beginMacro initializeBoundaryForcing(t,tba)
c write(*,'("initializeBoundaryForcing tba=",e10.2)') tba
if( t.le.0 .and. tba.gt.0. )then
  ssf = 0.
  ssft = 0. 
  ssftt = 0. 
  ssfttt = 0. 
  ssftttt = 0. 
else if( t.lt.tba )then
  tt=t/tba
  ssf = ramp4(tt)
  ssft = ramp4t(tt)
  ssftt = ramp4tt(tt)
  ssfttt = ramp4ttt(tt)
  ssftttt = ramp4tttt(tt)

! Here we turn off the plane wave after some time:
! else if( t.gt.1.0 )then
!  ssf = 0.
!  ssft = 0. 
!  ssftt = 0. 
!  ssfttt = 0. 
!  ssftttt = 0. 

 else
  ssf = 1.
  ssft = 0. 
  ssftt = 0. 
  ssfttt = 0. 
  ssftttt = 0. 
end if
#endMacro

c **************** Here is the new generic plane wave solution *******************

! component n=ex,ey,ez, hx,hy,hz (assumes ex=0)
#defineMacro planeWave0(x,y,z,t,n) sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc(n)
! one time derivative:
#defineMacro planeWavet0(x,y,z,t,n) (-twoPi*cc)*cos(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc(n)
! two time derivatives:
#defineMacro planeWavett0(x,y,z,t,n) (-(twoPi*cc)**2*sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc(n))
! three time derivatives:
#defineMacro planeWave3ttt0(x,y,z,t,n) ((twoPi*cc)**3*cos(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc(n))

c *************** Here is the 2D planeWave solution ******************************

#defineMacro planeWave2Dex0(x,y,t) sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))*pwc(0)
#defineMacro planeWave2Dey0(x,y,t) sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))*pwc(1)
#defineMacro planeWave2Dhz0(x,y,t) sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))*pwc(5)

c one time derivative:
#defineMacro planeWave2Dext0(x,y,t) (-twoPi*cc)*cos(twoPi*(kx*(x)+ky*(y)-cc*(t)))*pwc(0)
#defineMacro planeWave2Deyt0(x,y,t) (-twoPi*cc)*cos(twoPi*(kx*(x)+ky*(y)-cc*(t)))*pwc(1)
#defineMacro planeWave2Dhzt0(x,y,t) (-twoPi*cc)*cos(twoPi*(kx*(x)+ky*(y)-cc*(t)))*pwc(5)

c two time derivatives:
#defineMacro planeWave2Dextt0(x,y,t) (-(twoPi*cc)**2*sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))*pwc(0))
#defineMacro planeWave2Deytt0(x,y,t) (-(twoPi*cc)**2*sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))*pwc(1))
#defineMacro planeWave2Dhztt0(x,y,t) (-(twoPi*cc)**2*sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))*pwc(5))

c three time derivatives:
#defineMacro planeWave2Dexttt0(x,y,t) ((twoPi*cc)**3*cos(twoPi*(kx*(x)+ky*(y)-cc*(t)))*pwc(0))
#defineMacro planeWave2Deyttt0(x,y,t) ((twoPi*cc)**3*cos(twoPi*(kx*(x)+ky*(y)-cc*(t)))*pwc(1))
#defineMacro planeWave2Dhzttt0(x,y,t) ((twoPi*cc)**3*cos(twoPi*(kx*(x)+ky*(y)-cc*(t)))*pwc(5))

c four time derivatives:
#defineMacro planeWave2Dextttt0(x,y,t) ((twoPi*cc)**4*sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))*pwc(0))
#defineMacro planeWave2Deytttt0(x,y,t) ((twoPi*cc)**4*sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))*pwc(1))
#defineMacro planeWave2Dhztttt0(x,y,t) ((twoPi*cc)**4*sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))*pwc(5))

c Here are the slow start versions
#defineMacro planeWave2Dex(x,y,t) (ssf*planeWave2Dex0(x,y,t))
#defineMacro planeWave2Dey(x,y,t) (ssf*planeWave2Dey0(x,y,t))
#defineMacro planeWave2Dhz(x,y,t) (ssf*planeWave2Dhz0(x,y,t))

c one time derivative:
#defineMacro planeWave2Dext(x,y,t) (ssf*planeWave2Dext0(x,y,t)+ssft*planeWave2Dex0(x,y,t))
#defineMacro planeWave2Deyt(x,y,t) (ssf*planeWave2Deyt0(x,y,t)+ssft*planeWave2Dey0(x,y,t))
#defineMacro planeWave2Dhzt(x,y,t) (ssf*planeWave2Dhzt0(x,y,t)+ssft*planeWave2Dhz0(x,y,t))

c two time derivatives:
#defineMacro planeWave2Dextt(x,y,t) (ssf*planeWave2Dextt0(x,y,t)+2.*ssft*planeWave2Dext0(x,y,t)\
                                 +ssftt*planeWave2Dex0(x,y,t))
#defineMacro planeWave2Deytt(x,y,t) (ssf*planeWave2Deytt0(x,y,t)+2.*ssft*planeWave2Deyt0(x,y,t)\
                                 +ssftt*planeWave2Dey0(x,y,t))
#defineMacro planeWave2Dhztt(x,y,t) (ssf*planeWave2Dhztt0(x,y,t)+2.*ssft*planeWave2Dhzt0(x,y,t)\
                                 +ssftt*planeWave2Dhz0(x,y,t))

c three time derivatives:
#defineMacro planeWave2Dexttt(x,y,t) (ssf*planeWave2Dexttt0(x,y,t)+3.*ssft*planeWave2Dextt0(x,y,t)\
                                 +3.*ssftt*planeWave2Dext0(x,y,t)+ssfttt*planeWave2Dex0(x,y,t))
#defineMacro planeWave2Deyttt(x,y,t) (ssf*planeWave2Deyttt0(x,y,t)+3.*ssft*planeWave2Deytt0(x,y,t)\
                                 +3.*ssftt*planeWave2Deyt0(x,y,t)+ssfttt*planeWave2Dey0(x,y,t))
#defineMacro planeWave2Dhzttt(x,y,t) (ssf*planeWave2Dhzttt0(x,y,t)+3.*ssft*planeWave2Dhztt0(x,y,t)\
                                 +3.*ssftt*planeWave2Dhzt0(x,y,t)+ssfttt*planeWave2Dhz0(x,y,t))

c four time derivatives:
#defineMacro planeWave2Dextttt(x,y,t) (ssf*planeWave2Dextttt0(x,y,t)+4.*ssft*planeWave2Dexttt0(x,y,t)\
                                 +6.*ssftt*planeWave2Dextt0(x,y,t)+4.*ssfttt*planeWave2Dext0(x,y,t)+ssftttt*planeWave2Dex0(x,y,t))
#defineMacro planeWave2Deytttt(x,y,t) (ssf*planeWave2Deytttt0(x,y,t)+4.*ssft*planeWave2Deyttt0(x,y,t)\
                                 +6.*ssftt*planeWave2Deytt0(x,y,t)+4.*ssfttt*planeWave2Deyt0(x,y,t)+ssftttt*planeWave2Dey0(x,y,t))
#defineMacro planeWave2Dhztttt(x,y,t) (ssf*planeWave2Dhztttt0(x,y,t)+4.*ssft*planeWave2Dhzttt0(x,y,t)\
                                 +6.*ssftt*planeWave2Dhztt0(x,y,t)+4.*ssfttt*planeWave2Dhzt0(x,y,t)+ssftttt*planeWave2Dhz0(x,y,t))


c **************** Here is the 3D planeWave solution ***************************************

#defineMacro planeWave3Dex0(x,y,z,t) sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc(0)
#defineMacro planeWave3Dey0(x,y,z,t) sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc(1)
#defineMacro planeWave3Dez0(x,y,z,t) sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc(2)

#defineMacro planeWave3Dhx0(x,y,z,t) sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc(3)
#defineMacro planeWave3Dhy0(x,y,z,t) sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc(4)
#defineMacro planeWave3Dhz0(x,y,z,t) sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc(5)

c one time derivative:
#defineMacro planeWave3Dext0(x,y,z,t) (-twoPi*cc)*cos(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc(0)
#defineMacro planeWave3Deyt0(x,y,z,t) (-twoPi*cc)*cos(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc(1)
#defineMacro planeWave3Dezt0(x,y,z,t) (-twoPi*cc)*cos(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc(2)

#defineMacro planeWave3Dhxt0(x,y,z,t) (-twoPi*cc)*cos(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc(3)
#defineMacro planeWave3Dhyt0(x,y,z,t) (-twoPi*cc)*cos(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc(4)
#defineMacro planeWave3Dhzt0(x,y,z,t) (-twoPi*cc)*cos(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc(5)

c two time derivatives:
#defineMacro planeWave3Dextt0(x,y,z,t) (-(twoPi*cc)**2*sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc(0))
#defineMacro planeWave3Deytt0(x,y,z,t) (-(twoPi*cc)**2*sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc(1))
#defineMacro planeWave3Deztt0(x,y,z,t) (-(twoPi*cc)**2*sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc(2))

#defineMacro planeWave3Dhxtt0(x,y,z,t) (-(twoPi*cc)**2*sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc(3))
#defineMacro planeWave3Dhytt0(x,y,z,t) (-(twoPi*cc)**2*sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc(4))
#defineMacro planeWave3Dhztt0(x,y,z,t) (-(twoPi*cc)**2*sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc(5))

c three time derivatives:
#defineMacro planeWave3Dexttt0(x,y,z,t) ((twoPi*cc)**3*cos(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc(0))
#defineMacro planeWave3Deyttt0(x,y,z,t) ((twoPi*cc)**3*cos(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc(1))
#defineMacro planeWave3Dezttt0(x,y,z,t) ((twoPi*cc)**3*cos(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc(2))

#defineMacro planeWave3Dhxttt0(x,y,z,t) ((twoPi*cc)**3*cos(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc(3))
#defineMacro planeWave3Dhyttt0(x,y,z,t) ((twoPi*cc)**3*cos(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc(4))
#defineMacro planeWave3Dhzttt0(x,y,z,t) ((twoPi*cc)**3*cos(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc(5))

c four time derivatives:
#defineMacro planeWave3Dextttt0(x,y,z,t) ((twoPi*cc)**4*sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc(0))
#defineMacro planeWave3Deytttt0(x,y,z,t) ((twoPi*cc)**4*sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc(1))
#defineMacro planeWave3Deztttt0(x,y,z,t) ((twoPi*cc)**4*sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc(2))

#defineMacro planeWave3Dhxtttt0(x,y,z,t) ((twoPi*cc)**4*sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc(3))
#defineMacro planeWave3Dhytttt0(x,y,z,t) ((twoPi*cc)**4*sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc(4))
#defineMacro planeWave3Dhztttt0(x,y,z,t) ((twoPi*cc)**4*sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc(5))

c Here are the slow start versions
#defineMacro planeWave3Dex(x,y,z,t) (ssf*planeWave3Dex0(x,y,z,t))
#defineMacro planeWave3Dey(x,y,z,t) (ssf*planeWave3Dey0(x,y,z,t))
#defineMacro planeWave3Dez(x,y,z,t) (ssf*planeWave3Dez0(x,y,z,t))

#defineMacro planeWave3Dhx(x,y,z,t) (ssf*planeWave3Dhx0(x,y,z,t))
#defineMacro planeWave3Dhy(x,y,z,t) (ssf*planeWave3Dhy0(x,y,z,t))
#defineMacro planeWave3Dhz(x,y,z,t) (ssf*planeWave3Dhz0(x,y,z,t))

c one time derivative:
#defineMacro planeWave3Dext(x,y,z,t) (ssf*planeWave3Dext0(x,y,z,t)+ssft*planeWave3Dex0(x,y,z,t))
#defineMacro planeWave3Deyt(x,y,z,t) (ssf*planeWave3Deyt0(x,y,z,t)+ssft*planeWave3Dey0(x,y,z,t))
#defineMacro planeWave3Dezt(x,y,z,t) (ssf*planeWave3Dezt0(x,y,z,t)+ssft*planeWave3Dez0(x,y,z,t))

#defineMacro planeWave3Dhxt(x,y,z,t) (ssf*planeWave3Dext0(x,y,z,t)+ssft*planeWave3Dhx0(x,y,z,t))
#defineMacro planeWave3Dhyt(x,y,z,t) (ssf*planeWave3Deyt0(x,y,z,t)+ssft*planeWave3Dhy0(x,y,z,t))
#defineMacro planeWave3Dhzt(x,y,z,t) (ssf*planeWave3Dezt0(x,y,z,t)+ssft*planeWave3Dhz0(x,y,z,t))

c two time derivatives:
#defineMacro planeWave3Dextt(x,y,z,t) (ssf*planeWave3Dextt0(x,y,z,t)+2.*ssft*planeWave3Dext0(x,y,z,t)\
                                 +ssftt*planeWave3Dex0(x,y,z,t))
#defineMacro planeWave3Deytt(x,y,z,t) (ssf*planeWave3Deytt0(x,y,z,t)+2.*ssft*planeWave3Deyt0(x,y,z,t)\
                                 +ssftt*planeWave3Dey0(x,y,z,t))
#defineMacro planeWave3Deztt(x,y,z,t) (ssf*planeWave3Deztt0(x,y,z,t)+2.*ssft*planeWave3Dezt0(x,y,z,t)\
                                 +ssftt*planeWave3Dez0(x,y,z,t))

c three time derivatives:
#defineMacro planeWave3Dexttt(x,y,z,t) (ssf*planeWave3Dexttt0(x,y,z,t)+3.*ssft*planeWave3Dextt0(x,y,z,t)\
                                 +3.*ssftt*planeWave3Dext0(x,y,z,t)+ssfttt*planeWave3Dex0(x,y,z,t))
#defineMacro planeWave3Deyttt(x,y,z,t) (ssf*planeWave3Deyttt0(x,y,z,t)+3.*ssft*planeWave3Deytt0(x,y,z,t)\
                                 +3.*ssftt*planeWave3Deyt0(x,y,z,t)+ssfttt*planeWave3Dey0(x,y,z,t))
#defineMacro planeWave3Dezttt(x,y,z,t) (ssf*planeWave3Dezttt0(x,y,z,t)+3.*ssft*planeWave3Deztt0(x,y,z,t)\
                                 +3.*ssftt*planeWave3Dezt0(x,y,z,t)+ssfttt*planeWave3Dez0(x,y,z,t))

c four time derivatives:
#defineMacro planeWave3Dextttt(x,y,z,t) (ssf*planeWave3Dextttt0(x,y,z,t)+4.*ssft*planeWave3Dexttt0(x,y,z,t)\
                                 +6.*ssftt*planeWave3Dextt0(x,y,z,t)+4.*ssfttt*planeWave3Dext0(x,y,z,t)+ssftttt*planeWave3Dex0(x,y,z,t))
#defineMacro planeWave3Deytttt(x,y,z,t) (ssf*planeWave3Deytttt0(x,y,z,t)+4.*ssft*planeWave3Deyttt0(x,y,z,t)\
                                 +6.*ssftt*planeWave3Deytt0(x,y,z,t)+4.*ssfttt*planeWave3Deyt0(x,y,z,t)+ssftttt*planeWave3Dey0(x,y,z,t))
#defineMacro planeWave3Deztttt(x,y,z,t) (ssf*planeWave3Deztttt0(x,y,z,t)+4.*ssft*planeWave3Dezttt0(x,y,z,t)\
                                 +6.*ssftt*planeWave3Deztt0(x,y,z,t)+4.*ssfttt*planeWave3Dezt0(x,y,z,t)+ssftttt*planeWave3Dez0(x,y,z,t))


c Helper function: Return minus the second time derivative
#beginMacro getMinusPlaneWave3Dtt(i1,i2,i3,t,udd,vdd,wdd)
 x00=xy(i1,i2,i3,0)
 y00=xy(i1,i2,i3,1)
 z00=xy(i1,i2,i3,2)

 if( fieldOption.eq.0 )then
   udd=-planeWave3Dextt(x00,y00,z00,t) 
   vdd=-planeWave3Deytt(x00,y00,z00,t)
   wdd=-planeWave3Deztt(x00,y00,z00,t)
 else
   ! get time derivative (sosup) 
   udd=-planeWave3Dexttt(x00,y00,z00,t) 
   vdd=-planeWave3Deyttt(x00,y00,z00,t)
   wdd=-planeWave3Dezttt(x00,y00,z00,t)
 end if
#endMacro


