! *** Fourth order boundary conditions for Maxwell ****


! These next include files will define the macros that will define the difference approximations
! The actual macro is called below
#Include "defineDiffOrder2f.h"
#Include "defineDiffOrder4f.h"

!**************************************************************************

! Include macros that are common to different orders of accuracy

#Include "bcOptMaxwellMacros.h"

!**************************************************************************

! Here are macros that define the planeWave solution
#Include "planeWave.h"

!===================================================================================
!  Put the inner loop for the 4th-order BC here so we can repeat it for testing 
!==================================================================================
! #beginMacro bcCurv2dOrder4InnerLoop()
! #endMacro

! This macro is for the BC on Hz in 2D
#defineMacro FW12D(i1,i2,i3,ws,ut0,vt0) \
        ( -(rsxy(i1,i2,i3,axis,0)*rsxy(i1,i2,i3,axisp1,0)+rsxy(i1,i2,i3,axis,1)*rsxy(i1,i2,i3,axisp1,1))*ws \
        - rsxy(i1,i2,i3,axis,0)*vt0 + rsxy(i1,i2,i3,axis,1)*ut0 \
       )/(rsxy(i1,i2,i3,axis,0)**2+rsxy(i1,i2,i3,axis,1)**2)

! -------------------------------------------------------------------------------------------------------
! Macro: fifth-order extrapolation:
! -------------------------------------------------------------------------------------------------------
#defineMacro extrap5(ec,j1,j2,j3,is1,is2,is3)\
      ( 5.*u(j1      ,j2      ,j3      ,ec)-10.*u(j1+  is1,j2+  is2,j3+  is3,ec)+10.*u(j1+2*is1,j2+2*is2,j3+2*is3,ec)\
       -5.*u(j1+3*is1,j2+3*is2,j3+3*is3,ec)+    u(j1+4*is1,j2+4*is2,j3+4*is3,ec) ) 
                                        

! ===================================================================================
!  BCs for curvilinear grids in 2D
!
!  FORCING: none, twilightZone
! ===================================================================================
#beginMacro bcCurvilinear2dOrder4(FORCING)

 ! Since is1 is +1 or -1 we need to flip the sign of dr in the derivative approximations
 dra = dr(axis)*(1-2*side)
 dsa = dr(axisp1)*(1-2*side)
 drb = dr(axis  )
 dsb = dr(axisp1)

 if( debug .gt.0 )then
  write(*,'(" ******* Start: grid=",i2," side,axis=",2i2)') grid,side,axis
 end if
 beginLoops()
 if( mask(i1,i2,i3).gt.0 )then

  jacm1=1./RXDET2D(i1-is1,i2-is2,i3)
  a11m1 =rsxy(i1-is1,i2-is2,i3,axis  ,0)*jacm1
  a12m1 =rsxy(i1-is1,i2-is2,i3,axis  ,1)*jacm1

  jac=1./RXDET2D(i1,i2,i3)
  a11 =rsxy(i1,i2,i3,axis  ,0)*jac
  a12 =rsxy(i1,i2,i3,axis  ,1)*jac

  a21 =rsxy(i1,i2,i3,axisp1,0)*jac
  a22 =rsxy(i1,i2,i3,axisp1,1)*jac


  jacp1=1./RXDET2D(i1+is1,i2+is2,i3)
  a11p1=rsxy(i1+is1,i2+is2,i3,axis,0)*jacp1
  a12p1=rsxy(i1+is1,i2+is2,i3,axis,1)*jacp1

  jacm2=1./RXDET2D(i1-2*is1,i2-2*is2,i3)
  a11m2=rsxy(i1-2*is1,i2-2*is2,i3,axis,0)*jacm2
  a12m2=rsxy(i1-2*is1,i2-2*is2,i3,axis,1)*jacm2

  jacp2=1./RXDET2D(i1+2*is1,i2+2*is2,i3)
  a11p2=rsxy(i1+2*is1,i2+2*is2,i3,axis,0)*jacp2
  a12p2=rsxy(i1+2*is1,i2+2*is2,i3,axis,1)*jacp2


 a11r = DR4($A11)
 a12r = DR4($A12)
 a21r = DR4($A21)
 a22r = DR4($A22)

 a11s = DS4($A11)
 a12s = DS4($A12)
 a21s = DS4($A21)
 a22s = DS4($A22)

 a11rs = Drs4($A11)
 a12rs = Drs4($A12)
 a21rs = Drs4($A21)
 a22rs = Drs4($A22)

 a11rr = DRR4($A11)
 a12rr = DRR4($A12)
 a21rr = DRR4($A21)
 a22rr = DRR4($A22)

 a11ss = DSS4($A11)
 a12ss = DSS4($A12)
 a21ss = DSS4($A21)
 a22ss = DSS4($A22)

 if( .true. )then
   a11sss = DSSS2($A11)
   a12sss = DSSS2($A12)
   a21sss = DSSS2($A21)
   a22sss = DSSS2($A22)
 else
   ! not enough ghost points for the periodic or interp case for: (since we solve at i1=0)
   a11sss = DSSS4($A11)
   a12sss = DSSS4($A12)
   a21sss = DSSS4($A21)
   a22sss = DSSS4($A22)
 end if

 if( axis.eq.0 )then
   a11rss = Drss4($A11)
   a12rss = Drss4($A12)
   a21rss = Drss4($A21)
   a22rss = Drss4($A22)
 else
   a11rss = Drrs4($A11)
   a12rss = Drrs4($A12)
   a21rss = Drrs4($A21)
   a22rss = Drrs4($A22)
 end if

   c11 = C11(i1,i2,i3)
   c22 = C22(i1,i2,i3)

   c1 = C1Order4(i1,i2,i3)
   c2 = C2Order4(i1,i2,i3)

   ! *** we require only one s derivative of c11,c22,c1,c2: ****

   ! 2nd order:
   ! c11s = (C11(i1+js1,i2+js2,i3)-C11(i1-js1,i2-js2,i3))/(2.*dsa) 
   ! c22s = (C22(i1+js1,i2+js2,i3)-C22(i1-js1,i2-js2,i3))/(2.*dsa) 
   ! c1s =   (C1Order2(i1+js1,i2+js2,i3)- C1Order2(i1-js1,i2-js2,i3))/(2.*dsa) 
   ! c2s =   (C2Order2(i1+js1,i2+js2,i3)- C2Order2(i1-js1,i2-js2,i3))/(2.*dsa) 

   ! fourth-order:
!$$$   c11s = (8.*(C11(i1+  js1,i2+  js2,i3)-C11(i1-  js1,i2-  js2,i3))   \
!$$$             -(C11(i1+2*js1,i2+2*js2,i3)-C11(i1-2*js1,i2-2*js2,i3))   )/(12.*dsa)
!$$$   c22s = (8.*(C22(i1+  js1,i2+  js2,i3)-C22(i1-  js1,i2-  js2,i3))   \
!$$$             -(C22(i1+2*js1,i2+2*js2,i3)-C22(i1-2*js1,i2-2*js2,i3))   )/(12.*dsa)

   c11r = (8.*(C11(i1+  is1,i2+  is2,i3)-C11(i1-  is1,i2-  is2,i3))   \
             -(C11(i1+2*is1,i2+2*is2,i3)-C11(i1-2*is1,i2-2*is2,i3))   )/(12.*dra)
   c22r = (8.*(C22(i1+  is1,i2+  is2,i3)-C22(i1-  is1,i2-  is2,i3))   \
             -(C22(i1+2*is1,i2+2*is2,i3)-C22(i1-2*is1,i2-2*is2,i3))   )/(12.*dra)

   if( axis.eq.0 )then
     c1r = C1r4(i1,i2,i3)
     c2r = C2r4(i1,i2,i3)
   else
     c1r = C1s4(i1,i2,i3)
     c2r = C2s4(i1,i2,i3)
   end if

   us=US4(ex)
   uss=USS4(ex)
   usss=USSS2(ex)

   vs=US4(ey)
   vss=USS4(ey)
   vsss=USSS2(ey)

!   ws=US2(hz)
!   wss=USS2(hz)
   ws=US4(hz)
   wss=USS4(hz)

   tau1=rsxy(i1,i2,i3,axisp1,0)
   tau2=rsxy(i1,i2,i3,axisp1,1)

   uex=u(i1,i2,i3,ex)
   uey=u(i1,i2,i3,ey)



  ! Dr( a1.Delta\uv ) = (b3u,b3v).uvrrr + (b2u,b2v).uvrr + (b1u,b1v).uv + bf = 0 
  ! For TZ choose bf = bf - Dr( a1.Delta uvExact )
  ! see bcdiv.maple for:

#Include "bcdivMaxwell.h"


! ************ Answer *******************


 ctlrr=1.
 ctlr=1.

 ! forcing terms for TZ are stored in 
 cgI=1.
 gIf=0.
 gIVf=0.
 tau1DotUtt=0.
 Da1DotU=0.

 ! for Hz (w)
 fw1=0.
 fw2=0.

 if( forcingOption.eq.planeWaveBoundaryForcing )then
   ! In the plane wave forcing case we subtract out a plane wave incident field
   ! This causes the BC to be 
   !           tau.u = - tau.uI
   !   and     tau.utt = -tau.uI.tt

   ! *** set RHS for (a1.u).r =  - Ds( a2.uv )
   Da1DotU = -(  a21s*uex+a22s*uey + a21*us+a22*vs )

   x0=xy(i1,i2,i3,0)
   y0=xy(i1,i2,i3,1)
   ! Note minus sign since we are subtracting out the incident field
   if( fieldOption.eq.0 )then
     utt00=-planeWave2Dextt(x0,y0,t) 
     vtt00=-planeWave2Deytt(x0,y0,t)
     ut0  =-planeWave2Dext(x0,y0,t) 
     vt0  =-planeWave2Deyt(x0,y0,t) 
     uttt0=-planeWave2Dexttt(x0,y0,t) 
     vttt0=-planeWave2Deyttt(x0,y0,t)
   else
     ! we are assigning time derivatives (sosup)
     utt00=-planeWave2Dexttt(x0,y0,t) 
     vtt00=-planeWave2Deyttt(x0,y0,t)
     ut0  =-planeWave2Dextt(x0,y0,t) 
     vt0  =-planeWave2Deytt(x0,y0,t) 
     uttt0=-planeWave2Dextttt(x0,y0,t) 
     vttt0=-planeWave2Deytttt(x0,y0,t)
   end if

   tau1DotUtt = tau1*utt00+tau2*vtt00

   ! (a1.Delta u).r = - (a2.utt).s
   ! (a1.Delta u).r + bf = 0
   ! bf = bf + ( (a21zp1*uttzp1+a22zp1*vttzp1)-(a21zm1*uttzm1+a22zm1*vttzm1) )/(2.(dsa)

   x0=xy(i1+js1,i2+js2,i3,0)
   y0=xy(i1+js1,i2+js2,i3,1)
   if( fieldOption.eq.0 )then
     uttp1=-planeWave2Dextt(x0,y0,t) 
     vttp1=-planeWave2Deytt(x0,y0,t)
     utp1 =-planeWave2Dext(x0,y0,t) 
     vtp1 =-planeWave2Deyt(x0,y0,t) 
   else
     ! we are assigning time derivatives (sosup)
     uttp1=-planeWave2Dexttt(x0,y0,t) 
     vttp1=-planeWave2Deyttt(x0,y0,t)
     utp1 =-planeWave2Dextt(x0,y0,t) 
     vtp1 =-planeWave2Deytt(x0,y0,t) 
   end if

   x0=xy(i1-js1,i2-js2,i3,0)
   y0=xy(i1-js1,i2-js2,i3,1)
   if( fieldOption.eq.0 )then
     uttm1=-planeWave2Dextt(x0,y0,t) 
     vttm1=-planeWave2Deytt(x0,y0,t)
     utm1 =-planeWave2Dext(x0,y0,t) 
     vtm1 =-planeWave2Deyt(x0,y0,t) 
   else
     ! we are assigning time derivatives (sosup)
     uttm1=-planeWave2Dexttt(x0,y0,t) 
     vttm1=-planeWave2Deyttt(x0,y0,t)
     utm1 =-planeWave2Dextt(x0,y0,t) 
     vtm1 =-planeWave2Deytt(x0,y0,t) 
   end if

   x0=xy(i1+2*js1,i2+2*js2,i3,0)
   y0=xy(i1+2*js1,i2+2*js2,i3,1)
   if( fieldOption.eq.0 )then
     uttp2=-planeWave2Dextt(x0,y0,t) 
     vttp2=-planeWave2Deytt(x0,y0,t)
   else
     ! we are assigning time derivatives (sosup)
     uttp2=-planeWave2Dexttt(x0,y0,t) 
     vttp2=-planeWave2Deyttt(x0,y0,t)
   end if

   x0=xy(i1-2*js1,i2-2*js2,i3,0)
   y0=xy(i1-2*js1,i2-2*js2,i3,1)
   if( fieldOption.eq.0 )then
     uttm2=-planeWave2Dextt(x0,y0,t) 
     vttm2=-planeWave2Deytt(x0,y0,t)
   else
     ! we are assigning time derivatives (sosup)
     uttm2=-planeWave2Dexttt(x0,y0,t) 
     vttm2=-planeWave2Deyttt(x0,y0,t)
   end if

   utts = (8.*(uttp1-uttm1)-(uttp2-uttm2) )/(12.*dsa)
   vtts = (8.*(vttp1-vttm1)-(vttp2-vttm2) )/(12.*dsa)
   bf = bf + a21s*utt00+a22s*vtt00 + a21*utts + a22*vtts


   ! ***** Forcing for Hz ******
   ! (w).r = fw1                              (w.n = 0 )
   ! (c11*w.rr + c22*w.ss + ... ).r = fw2     ( (Delta w).n = 0 )

   ! *** for planeWaveBoundaryForcing we need to use: u.t=w.y and v.t=-w.x =>
   ! *****  (n1,n2).(w.x,w.y) = -n1*v.t + n2*u.t
   !  OR    (rx,ry).(w.x,w.y) = -rx*v.t + ry*u.t
   !   (rx**2+ry**2) w.r + (rx*sx+ry*sy)*ws = -rx*vt + ry*ut 
   
   ! Note: the first term here (rx*sx+ry*sy) will be zero on an orthogonal grid


    fw1=FW12D(i1,i2,i3,ws,ut0,vt0)
!$$$   fw1=( -(rsxy(i1,i2,i3,axis,0)*rsxy(i1,i2,i3,axisp1,0)+rsxy(i1,i2,i3,axis,1)*rsxy(i1,i2,i3,axisp1,1))*ws \
!$$$        - rsxy(i1,i2,i3,axis,0)*vt0 + rsxy(i1,i2,i3,axis,1)*ut0 \
!$$$       )/(rsxy(i1,i2,i3,axis,0)**2+rsxy(i1,i2,i3,axis,1)**2)

   ! fw2 = fw1.tt -[ c22*wrss + c2 wrs ]
   ! where
   !     w.r = fw1 = (-rx*vt + ry*ut - (rx*sx+ry*sy)*ws )/(rx**2+ry**2) 
   ! Compute wrs and wrss by differencing fw1

   wsm1 = (u(i1,i2,i3,hz)-u(i1-2*js1,i2-2*js2,i3,hz))/(2.*dsa)    ! ws(i1-js1,i2-js2,i3)
   wsp1 = (u(i1+2*js1,i2+2*js2,i3,hz)-u(i1,i2,i3,hz))/(2.*dsa)    ! ws(i1+js1,i2+js2,i3)

   fw1m1=FW12D(i1-js1,i2-js2,i3,wsm1,utm1,vtm1)
   fw1p1=FW12D(i1+js1,i2+js2,i3,wsp1,utp1,vtp1)

   ! NOTE: the term involving wtts is left off -- the coeff is zero for orthogonal grids
   fw2 = (-rsxy(i1,i2,i3,axis,0)*vttt0 + rsxy(i1,i2,i3,axis,1)*uttt0 )/\
                               (rsxy(i1,i2,i3,axis,0)**2+rsxy(i1,i2,i3,axis,1)**2)\
         - c22*( fw1p1-2.*fw1+fw1m1 )/(dsa**2) -c2*(fw1p1-fw1m1 )/(2.*dsa)

!   fw2 = (-rsxy(i1,i2,i3,axis,0)*vttt0 + rsxy(i1,i2,i3,axis,1)*uttt0 )/\
!                               (rsxy(i1,i2,i3,axis,0)**2+rsxy(i1,i2,i3,axis,1)**2)\
!         - c22*( (-rsxy(i1+js1,i2+js2,i3,axis,0)*vtp1 + rsxy(i1+js1,i2+js2,i3,axis,1)*utp1)\
!             -2.*(-rsxy(i1    ,i2    ,i3,axis,0)*vt0  + rsxy(i1    ,i2    ,i3,axis,1)*ut0 ) \
!                +(-rsxy(i1-js1,i2-js2,i3,axis,0)*vtm1 + rsxy(i1-js1,i2-js2,i3,axis,1)*utm1) )/(dsa**2) \
!         -  c2*( (-rsxy(i1+js1,i2+js2,i3,axis,0)*vtp1 + rsxy(i1+js1,i2+js2,i3,axis,1)*utp1)\
!                -(-rsxy(i1-js1,i2-js2,i3,axis,0)*vtm1 + rsxy(i1-js1,i2-js2,i3,axis,1)*utm1) )/(2.*dsa)
 end if

 #If #FORCING == "twilightZone"

  ! ********** For now do this: should work for quadratics *******************
  cgI=0.  
  gIf=0.
  ! ***********************************************

   ! For TZ: utt0 = utt - ett + Lap(e)
   call ogDeriv(ep, 0, 2,0,0, xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,ex, uxx)
   call ogDeriv(ep, 0, 0,2,0, xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,ex, uyy)
   utt00=uxx+uyy
  
   call ogDeriv(ep, 0, 2,0,0, xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,ey, vxx)
   call ogDeriv(ep, 0, 0,2,0, xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,ey, vyy)
   vtt00=vxx+vyy

  tau1DotUtt = tau1*utt00+tau2*vtt00

  call ogDeriv(ep, 0, 2,0,0, xy(i1-is1,i2-is2,i3,0),xy(i1-is1,i2-is2,i3,1),0.,t,ex, uxxm1)
  call ogDeriv(ep, 0, 0,2,0, xy(i1-is1,i2-is2,i3,0),xy(i1-is1,i2-is2,i3,1),0.,t,ex, uyym1)
  call ogDeriv(ep, 0, 2,0,0, xy(i1+is1,i2+is2,i3,0),xy(i1+is1,i2+is2,i3,1),0.,t,ex, uxxp1)
  call ogDeriv(ep, 0, 0,2,0, xy(i1+is1,i2+is2,i3,0),xy(i1+is1,i2+is2,i3,1),0.,t,ex, uyyp1)

  call ogDeriv(ep, 0, 2,0,0, xy(i1-is1,i2-is2,i3,0),xy(i1-is1,i2-is2,i3,1),0.,t,ey, vxxm1)
  call ogDeriv(ep, 0, 0,2,0, xy(i1-is1,i2-is2,i3,0),xy(i1-is1,i2-is2,i3,1),0.,t,ey, vyym1)
  call ogDeriv(ep, 0, 2,0,0, xy(i1+is1,i2+is2,i3,0),xy(i1+is1,i2+is2,i3,1),0.,t,ey, vxxp1)
  call ogDeriv(ep, 0, 0,2,0, xy(i1+is1,i2+is2,i3,0),xy(i1+is1,i2+is2,i3,1),0.,t,ey, vyyp1)

  call ogDeriv(ep, 0, 2,0,0, xy(i1-2*is1,i2-2*is2,i3,0),xy(i1-2*is1,i2-2*is2,i3,1),0.,t,ex, uxxm2)
  call ogDeriv(ep, 0, 0,2,0, xy(i1-2*is1,i2-2*is2,i3,0),xy(i1-2*is1,i2-2*is2,i3,1),0.,t,ex, uyym2)
  call ogDeriv(ep, 0, 2,0,0, xy(i1+2*is1,i2+2*is2,i3,0),xy(i1+2*is1,i2+2*is2,i3,1),0.,t,ex, uxxp2)
  call ogDeriv(ep, 0, 0,2,0, xy(i1+2*is1,i2+2*is2,i3,0),xy(i1+2*is1,i2+2*is2,i3,1),0.,t,ex, uyyp2)

  call ogDeriv(ep, 0, 2,0,0, xy(i1-2*is1,i2-2*is2,i3,0),xy(i1-2*is1,i2-2*is2,i3,1),0.,t,ey, vxxm2)
  call ogDeriv(ep, 0, 0,2,0, xy(i1-2*is1,i2-2*is2,i3,0),xy(i1-2*is1,i2-2*is2,i3,1),0.,t,ey, vyym2)
  call ogDeriv(ep, 0, 2,0,0, xy(i1+2*is1,i2+2*is2,i3,0),xy(i1+2*is1,i2+2*is2,i3,1),0.,t,ey, vxxp2)
  call ogDeriv(ep, 0, 0,2,0, xy(i1+2*is1,i2+2*is2,i3,0),xy(i1+2*is1,i2+2*is2,i3,1),0.,t,ey, vyyp2)

  ! For TZ choose bf = bf - Dr( a1.Delta uvExact )
  bf = bf - a11r*utt00 - a12r*vtt00 -a11*( 8.*((uxxp1+uyyp1)-(uxxm1+uyym1))-((uxxp2+uyyp2)-(uxxm2+uyym2)) )/(12.*dra) \
                                    -a12*( 8.*((vxxp1+vyyp1)-(vxxm1+vyym1))-((vxxp2+vyyp2)-(vxxm2+vyym2)) )/(12.*dra)

  ! write(*,'("  bc4:i1,i2=",2i3,"  b3u,b3v,b2u,b2v,b1u,b1v,bf,a11r,a12r=",9e12.4)') i1,i2,b3u,b3v,b2u,b2v,b1u,b1v,bf,a11r,a12r

  OGF2D(i1,i2,i3,t, uv0(0),uv0(1),uv0(2))
  OGF2D(i1-is1,i2-is2,i3,t, uvm(0),uvm(1),uvm(2))
  OGF2D(i1+is1,i2+is2,i3,t, uvp(0),uvp(1),uvp(2))
  OGF2D(i1-2*is1,i2-2*is2,i3,t, uvm2(0),uvm2(1),uvm2(2))
  OGF2D(i1+2*is1,i2+2*is2,i3,t, uvp2(0),uvp2(1),uvp2(2))

  ! Da1DotU = (a1.uv).r to 4th order
  Da1DotU = (8.*( (a11p1*uvp(0)+a12p1*uvp(1)) - (a11m1*uvm(0)+a12m1*uvm(1)) )\
              - ( (a11p2*uvp2(0)+a12p2*uvp2(1)) - (a11m2*uvm2(0)+a12m2*uvm2(1)) ) )/(12.*dra)


  ! For testing: *************************************************************
!$$$  urrr=(uvp2(0)-2.*(uvp(0)-uvm(0))-uvm2(0))/(2.*dra**3)
!$$$  vrrr=(uvp2(1)-2.*(uvp(1)-uvm(1))-uvm2(1))/(2.*dra**3)
!$$$
!$$$  urr=(-30.*uv0(0)+16.*(uvp(0)+uvm(0))-(uvp2(0)+uvm2(0)) )/(12.*dra**2)
!$$$  vrr=(-30.*uv0(1)+16.*(uvp(1)+uvm(1))-(uvp2(1)+uvm2(1)) )/(12.*dra**2)
!$$$
!$$$  ur=(8.*(uvp(0)-uvm(0))-(uvp2(0)-uvm2(0)))/(12.*dra)
!$$$  vr=(8.*(uvp(1)-uvm(1))-(uvp2(1)-uvm2(1)))/(12.*dra)
!$$$
!$$$  bf = -( b3u*urrr+b3v*vrrr+b2u*urr+b2v*vrr+b1u*ur+b1v*vr )
  ! *************************************************************************

 ! for now remove the error in the extrapolation ************
 ! gIVf = tau1*( uvm2(0)-4.*uvm(0)+6.*uv0(0)-4.*uvp(0)+uvp2(0)) +\
 !        tau2*( uvm2(1)-4.*uvm(1)+6.*uv0(1)-4.*uvp(1)+uvp2(1))
 gIVf=0.


  a21zp1= A21(i1+js1,i2+js2,i3) 
  a21zm1= A21(i1-js1,i2-js2,i3) 
  a21zp2= A21(i1+2*js1,i2+2*js2,i3) 
  a21zm2= A21(i1-2*js1,i2-2*js2,i3) 

  a22zp1= A22(i1+js1,i2+js2,i3) 
  a22zm1= A22(i1-js1,i2-js2,i3) 
  a22zp2= A22(i1+2*js1,i2+2*js2,i3) 
  a22zm2= A22(i1-2*js1,i2-2*js2,i3) 

  ! *** set to - Ds( a2.uv )
  Da1DotU = -(  \
       ( 8.*(a21zp1*u(i1+  js1,i2+  js2,i3,ex)-a21zm1*u(i1-  js1,i2-  js2,i3,ex)) \
           -(a21zp2*u(i1+2*js1,i2+2*js2,i3,ex)-a21zm2*u(i1-2*js1,i2-2*js2,i3,ex)) )/(12.*dsa) \
      +( 8.*(a22zp1*u(i1+  js1,i2+  js2,i3,ey)-a22zm1*u(i1-  js1,i2-  js2,i3,ey)) \
           -(a22zp2*u(i1+2*js1,i2+2*js2,i3,ey)-a22zm2*u(i1-2*js1,i2-2*js2,i3,ey)) )/(12.*dsa)  )


 ! ***** Forcing for Hz ******
 ! (w).r = fw1                              (w.n = 0 )
 ! (c11*w.rr + c22*w.ss + ... ).r = fw2     ( (Delta w).n = 0 )

 ! u(i1-is1,i2-is2,i3,hz) = u(i1-is1,i2-is2,i3,hz) + uvm(2)-uvp(2)
 ! u(i1-2*is1,i2-2*is2,i3,hz) = u(i1+2*is1,i2+2*is2,i3,hz)+ uvm2(2)-uvp2(2)

 fw1= (8.*(uvp(2)-uvm(2))-(uvp2(2)-uvm2(2)))/(12.*dra) 

 wrrr=(uvp2(2)-2.*(uvp(2)-uvm(2))-uvm2(2))/(2.*dra**3)

 ! wrr=(uvp(2)-2.*uv0(2)+uvm(2))/(dra**2)
 wrr=(-30.*uv0(2)+16.*(uvp(2)+uvm(2))-(uvp2(2)+uvm2(2)) )/(12.*dra**2)
 ! wr=(uvp(2)-uvm(2))/(2.*dra)
 wr=(8.*(uvp(2)-uvm(2))-(uvp2(2)-uvm2(2)))/(12.*dra) 
 fw2= c11*wrrr + (c1+c11r)*wrr + c1r*wr

 ! for tangential derivatives:
 OGF2D(i1-js1,i2-js2,i3,t, uvm(0),uvm(1),uvm(2))
 OGF2D(i1+js1,i2+js2,i3,t, uvp(0),uvp(1),uvp(2))
 OGF2D(i1-2*js1,i2-2*js2,i3,t, uvm2(0),uvm2(1),uvm2(2))
 OGF2D(i1+2*js1,i2+2*js2,i3,t, uvp2(0),uvp2(1),uvp2(2))

 ! These approximations should be consistent with the approximations for ws and wss above
 ! fw2=fw2 + c22r*(uvp(2)-2.*uv0(2)+uvm(2))/(dsa**2)+c2r*(uvp(2)-uvm(2))/(2.*dsa)
 fw2=fw2 + c22r*(-30.*uv0(2)+16.*(uvp(2)+uvm(2))-(uvp2(2)+uvm2(2)) )/(12.*dsa**2)\
         + c2r*(8.*(uvp(2)-uvm(2))-(uvp2(2)-uvm2(2)))/(12.*dsa) 



 #End


! Now assign ex and ey at the ghost points:
! #Include "bc4Maxwell.h"
! Use 5th-order extrap: 8wdh* 2015/07/03
#Include "bc4MaxwellExtrap5.h"

! extrapolate normal component:
! #Include "bc4eMaxwell.h"


! Now assign Hz at the ghost points

! u(i1-  is1,i2-  is2,i3-  is3,hz) = u(i1+  is1,i2+  is2,i3+  is3,hz)
! u(i1-2*is1,i2-2*is2,i3-2*is3,hz) = u(i1+2*is1,i2+2*is2,i3+2*is3,hz)

#Include "bc4HzMaxwell.h"

!  **********************************************************************************************

else if( mask(i1,i2,i3).lt.0 )then

 ! we need to assign ghost points that lie outside of interpolation points
 ! This case is similar to above except that we extrapolate the 2nd-ghost line values for a1.u


 jac=1./RXDET2D(i1,i2,i3)
 a11 =rsxy(i1,i2,i3,axis  ,0)*jac
 a12 =rsxy(i1,i2,i3,axis  ,1)*jac

 a21 =rsxy(i1,i2,i3,axisp1,0)*jac
 a22 =rsxy(i1,i2,i3,axisp1,1)*jac


 jacm1=1./RXDET2D(i1-is1,i2-is2,i3)
 a11m1 =rsxy(i1-is1,i2-is2,i3,axis  ,0)*jacm1
 a12m1 =rsxy(i1-is1,i2-is2,i3,axis  ,1)*jacm1

 jacp1=1./RXDET2D(i1+is1,i2+is2,i3)
 a11p1=rsxy(i1+is1,i2+is2,i3,axis,0)*jacp1
 a12p1=rsxy(i1+is1,i2+is2,i3,axis,1)*jacp1

 jacm2=1./RXDET2D(i1-2*is1,i2-2*is2,i3)
 a11m2=rsxy(i1-2*is1,i2-2*is2,i3,axis,0)*jacm2
 a12m2=rsxy(i1-2*is1,i2-2*is2,i3,axis,1)*jacm2

 jacp2=1./RXDET2D(i1+2*is1,i2+2*is2,i3)
 a11p2=rsxy(i1+2*is1,i2+2*is2,i3,axis,0)*jacp2
 a12p2=rsxy(i1+2*is1,i2+2*is2,i3,axis,1)*jacp2


 a11r = DR4($A11)
 a12r = DR4($A12)
 a21r = DR4($A21)
 a22r = DR4($A22)

! a11s = DS4($A11)
! a12s = DS4($A12)
! a21s = DS4($A21)
! a22s = DS4($A22)

 c11 = C11(i1,i2,i3)
 c22 = C22(i1,i2,i3)

! *  c1 = C1Order4(i1,i2,i3)
! *  c2 = C2Order4(i1,i2,i3)

 ! These next r derivatives are needed for Hz
 c11r = (8.*(C11(i1+  is1,i2+  is2,i3)-C11(i1-  is1,i2-  is2,i3))   \
           -(C11(i1+2*is1,i2+2*is2,i3)-C11(i1-2*is1,i2-2*is2,i3))   )/(12.*dra)
 c22r = (8.*(C22(i1+  is1,i2+  is2,i3)-C22(i1-  is1,i2-  is2,i3))   \
           -(C22(i1+2*is1,i2+2*is2,i3)-C22(i1-2*is1,i2-2*is2,i3))   )/(12.*dra)
! *  if( axis.eq.0 )then
! *    c1r = C1r4(i1,i2,i3)
! *    c2r = C2r4(i1,i2,i3)
! *  else
! *    c1r = C1s4(i1,i2,i3)
! *    c2r = C2s4(i1,i2,i3)
! *  end if

! ************** OLD **************
! *  ! Use one sided approximations as needed 
! *  js1a=abs(js1)
! *  js2a=abs(js2)
! *  if( (i1-2*js1a).ge.md1a .and. (i2-2*js2a).ge.md2a .and. (i1+2*js1a).le.md1b .and. (i2+2*js2a).le.md2b )then
! *    a11s = DS4($A11)
! *    a12s = DS4($A12)
! *    a21s = DS4($A21)
! *    a22s = DS4($A22)
! *  else if( (i1-js1a).ge.md1a .and. (i2-js2a).ge.md2a .and. (i1+js1a).le.md1b .and. (i2+js2a).le.md2b )then
! *    a11s = DS($A11)
! *    a12s = DS($A12)
! *    a21s = DS($A21)
! *    a22s = DS($A22)
! *  else if( (i1-js1).ge.md1a .and. (i1-js1).le.md1b .and. (i2-js2).ge.md2a .and. (i2-js2).le.md2b )then
! *   ! 2nd-order:
! *   a11s =-(-3.*A11(i1,i2,i3)+4.*A11(i1-js1,i2-js2,i3)-A11(i1-2*js1,i2-2*js2,i3))/(2.*dsa)
! *   a12s =-(-3.*A12(i1,i2,i3)+4.*A12(i1-js1,i2-js2,i3)-A12(i1-2*js1,i2-2*js2,i3))/(2.*dsa)
! *   a21s =-(-3.*A21(i1,i2,i3)+4.*A21(i1-js1,i2-js2,i3)-A21(i1-2*js1,i2-2*js2,i3))/(2.*dsa)
! *   a22s =-(-3.*A22(i1,i2,i3)+4.*A22(i1-js1,i2-js2,i3)-A22(i1-2*js1,i2-2*js2,i3))/(2.*dsa)
! *  else
! *   a11s = (-3.*A11(i1,i2,i3)+4.*A11(i1+js1,i2+js2,i3)-A11(i1+2*js1,i2+2*js2,i3))/(2.*dsa)
! *   a12s = (-3.*A12(i1,i2,i3)+4.*A12(i1+js1,i2+js2,i3)-A12(i1+2*js1,i2+2*js2,i3))/(2.*dsa)
! *   a21s = (-3.*A21(i1,i2,i3)+4.*A21(i1+js1,i2+js2,i3)-A21(i1+2*js1,i2+2*js2,i3))/(2.*dsa)
! *   a22s = (-3.*A22(i1,i2,i3)+4.*A22(i1+js1,i2+js2,i3)-A22(i1+2*js1,i2+2*js2,i3))/(2.*dsa)
! *  end if
! * 
! * 
! *  ! warning -- the compiler could still try to evaluate the mask at an invalid point
! *  if( (i1-js1a).ge.md1a .and. (i2-js2a).ge.md2a .and. mask(i1-js1,i2-js2,i3).ne.0 .and. \
! *      (i1+js1a).le.md1b .and. (i2+js2a).le.md2b .and. mask(i1+js1,i2+js2,i3).ne.0 )then
! *    us=US2(ex)
! *    vs=US2(ey)
! *    ws=US2(hz)
! * 
! *    uss=USS2(ex)
! *    vss=USS2(ey)
! *    wss=USS2(hz)
! *   !  write(*,'(" **ghost-interp: use central difference: us,uss=",2e10.2)') us,uss
! * 
! *  else if( (i1-2*js1).ge.md1a .and. (i1-2*js1).le.md1b .and. \
! *           (i2-2*js2).ge.md2a .and. (i2-2*js2).le.md2b .and. \
! *            mask(i1-js1,i2-js2,i3).ne.0 .and. mask(i1-2*js1,i2-2*js2,i3).ne.0 )then
! *    
! *   ! these are just first order but this is probably good enough since these values
! *   ! may not even appear in any other equations
! * !  us = (u(i1,i2,i3,ex)-u(i1-js1,i2-js2,i3,ex))/dsa
! * !  vs = (u(i1,i2,i3,ey)-u(i1-js1,i2-js2,i3,ey))/dsa
! * !  ws = (u(i1,i2,i3,hz)-u(i1-js1,i2-js2,i3,hz))/dsa
! * !
! * !  uss = (u(i1,i2,i3,ex)-2.*u(i1-js1,i2-js2,i3,ex)+u(i1-2*js1,i2-2*js2,i3,ex))/(dsa**2)
! * !  vss = (u(i1,i2,i3,ey)-2.*u(i1-js1,i2-js2,i3,ey)+u(i1-2*js1,i2-2*js2,i3,ey))/(dsa**2)
! * !  wss = (u(i1,i2,i3,hz)-2.*u(i1-js1,i2-js2,i3,hz)+u(i1-2*js1,i2-2*js2,i3,hz))/(dsa**2)
! * 
! *   ! 2nd-order:
! * 
! *   us = -(-3.*u(i1,i2,i3,ex)+4.*u(i1-js1,i2-js2,i3,ex)-u(i1-2*js1,i2-2*js2,i3,ex))/(2.*dsa)
! *   vs = -(-3.*u(i1,i2,i3,ey)+4.*u(i1-js1,i2-js2,i3,ey)-u(i1-2*js1,i2-2*js2,i3,ey))/(2.*dsa)
! *   ws = -(-3.*u(i1,i2,i3,hz)+4.*u(i1-js1,i2-js2,i3,hz)-u(i1-2*js1,i2-2*js2,i3,hz))/(2.*dsa)
! * 
! *   uss = (2.*u(i1,i2,i3,ex)-5.*u(i1-js1,i2-js2,i3,ex)+4.*u(i1-2*js1,i2-2*js2,i3,ex)-u(i1-3*js1,i2-3*js2,i3,ex))/(dsa**2)
! *   vss = (2.*u(i1,i2,i3,ey)-5.*u(i1-js1,i2-js2,i3,ey)+4.*u(i1-2*js1,i2-2*js2,i3,ey)-u(i1-3*js1,i2-3*js2,i3,ey))/(dsa**2)
! *   wss = (2.*u(i1,i2,i3,hz)-5.*u(i1-js1,i2-js2,i3,hz)+4.*u(i1-2*js1,i2-2*js2,i3,hz)-u(i1-3*js1,i2-3*js2,i3,hz))/(dsa**2)
! * 
! * !  write(*,'(" **ghost-interp: use left-difference: us,uss=",2e10.2," us1=",e10.2," js1,js2=",2i2)') us,uss,\
! * !            (u(i1,i2,i3,ex)-u(i1-js1,i2-js2,i3,ex))/dsa,js1,js2
! * 
! *  else if( (i1+2*js1).ge.md1a .and. (i1+2*js1).le.md1b .and. \
! *           (i2+2*js2).ge.md2a .and. (i2+2*js2).le.md2b .and.  \
! *           mask(i1+js1,i2+js2,i3).ne.0 .and. mask(i1+2*js1,i2+2*js2,i3).ne.0 )then
! * 
! * !  us = (u(i1+js1,i2+js2,i3,ex)-u(i1,i2,i3,ex))/dsa
! * !  vs = (u(i1+js1,i2+js2,i3,ey)-u(i1,i2,i3,ey))/dsa
! * !  ws = (u(i1+js1,i2+js2,i3,hz)-u(i1,i2,i3,hz))/dsa
! * !
! * !  uss = (u(i1,i2,i3,ex)-2.*u(i1+js1,i2+js2,i3,ex)+u(i1+2*js1,i2+2*js2,i3,ex))/(dsa**2)
! * !  vss = (u(i1,i2,i3,ey)-2.*u(i1+js1,i2+js2,i3,ey)+u(i1+2*js1,i2+2*js2,i3,ey))/(dsa**2)
! * !  wss = (u(i1,i2,i3,hz)-2.*u(i1+js1,i2+js2,i3,hz)+u(i1+2*js1,i2+2*js2,i3,hz))/(dsa**2)
! * 
! *   ! 2nd-order:
! *  us = (-3.*u(i1,i2,i3,ex)+4.*u(i1+js1,i2+js2,i3,ex)-u(i1+2*js1,i2+2*js2,i3,ex))/(2.*dsa)
! *  vs = (-3.*u(i1,i2,i3,ey)+4.*u(i1+js1,i2+js2,i3,ey)-u(i1+2*js1,i2+2*js2,i3,ey))/(2.*dsa)
! *  ws = (-3.*u(i1,i2,i3,hz)+4.*u(i1+js1,i2+js2,i3,hz)-u(i1+2*js1,i2+2*js2,i3,hz))/(2.*dsa)
! *  uss = (2.*u(i1,i2,i3,ex)-5.*u(i1+js1,i2+js2,i3,ex)+4.*u(i1+2*js1,i2+2*js2,i3,ex)-u(i1+3*js1,i2+3*js2,i3,ex))/(dsa**2)
! *  vss = (2.*u(i1,i2,i3,ey)-5.*u(i1+js1,i2+js2,i3,ey)+4.*u(i1+2*js1,i2+2*js2,i3,ey)-u(i1+3*js1,i2+3*js2,i3,ey))/(dsa**2)
! *  wss = (2.*u(i1,i2,i3,hz)-5.*u(i1+js1,i2+js2,i3,hz)+4.*u(i1+2*js1,i2+2*js2,i3,hz)-u(i1+3*js1,i2+3*js2,i3,hz))/(dsa**2)
! * 
! *  ! write(*,'(" **ghost-interp: use right-difference: us,uss=",2e10.2)') us,uss
! * 
! *  else 
! *    ! this case shouldn't matter
! *    us=0.
! *    vs=0.
! *    ws=0.
! *    uss=0.
! *    vss=0.
! *    wss=0.
! *  end if


! *********************** NEW ************************
 ! ***************************************************************************************
 ! Use one sided approximations as needed for expressions needing tangential derivatives
 ! ***************************************************************************************


 js1a=abs(js1)
 js2a=abs(js2)

 ! *** first do metric derivatives -- no need to worry about the mask value ****
 if( (i1-2*js1a).ge.md1a .and. (i1+2*js1a).le.md1b .and. \
     (i2-2*js2a).ge.md2a .and. (i2+2*js2a).le.md2b )then
  ! centered approximation is ok

  c1 = C1Order4(i1,i2,i3)
  c2 = C2Order4(i1,i2,i3)
  if( axis.eq.0 )then
    c1r = C1r4(i1,i2,i3)
    c2r = C2r4(i1,i2,i3)
  else
    c1r = C1s4(i1,i2,i3)
    c2r = C2s4(i1,i2,i3)
  end if
  a11s = DS4($A11)
  a12s = DS4($A12)
  a21s = DS4($A21)
  a22s = DS4($A22)

 else if( (i1-js1a).ge.md1a .and. (i1+js1a).le.md1b .and. \
          (i2-js2a).ge.md2a .and. (i2+js2a).le.md2b )then
  ! use 2nd-order centered approximation
  c1 = C1Order2(i1,i2,i3)
  c2 = C2Order2(i1,i2,i3)
  if( axis.eq.0 )then
    c1r = C1r2(i1,i2,i3)
    c2r = C2r2(i1,i2,i3)
  else
    c1r = C1s2(i1,i2,i3)
    c2r = C2s2(i1,i2,i3)
  end if
  a11s = DS($A11)
  a12s = DS($A12)
  a21s = DS($A21)
  a22s = DS($A22)

 else if( (i1-3*js1a).ge.md1a .and. \
          (i2-3*js2a).ge.md2a )then
  ! one sided  2nd-order:
  c1 = 2.*C1Order2(i1-js1a,i2-js2a,i3)-C1Order2(i1-2*js1a,i2-2*js2a,i3)
  c2 = 2.*C2Order2(i1-js1a,i2-js2a,i3)-C2Order2(i1-2*js1a,i2-2*js2a,i3)
  if( axis.eq.0 )then
    c1r = 2.*C1r2(i1-js1a,i2-js2a,i3)-C1r2(i1-2*js1a,i2-2*js2a,i3)
    c2r = 2.*C2r2(i1-js1a,i2-js2a,i3)-C2r2(i1-2*js1a,i2-2*js2a,i3)
  else
    c1r = 2.*C1s2(i1-js1a,i2-js2a,i3)-C1s2(i1-2*js1a,i2-2*js2a,i3)
    c2r = 2.*C2s2(i1-js1a,i2-js2a,i3)-C2s2(i1-2*js1a,i2-2*js2a,i3)
  end if
  a11s =-(-3.*A11(i1,i2,i3)+4.*A11(i1-js1a,i2-js2a,i3)-A11(i1-2*js1a,i2-2*js2a,i3))/(2.*dsb) ! NOTE: use ds not dsa
  a12s =-(-3.*A12(i1,i2,i3)+4.*A12(i1-js1a,i2-js2a,i3)-A12(i1-2*js1a,i2-2*js2a,i3))/(2.*dsb)
  a21s =-(-3.*A21(i1,i2,i3)+4.*A21(i1-js1a,i2-js2a,i3)-A21(i1-2*js1a,i2-2*js2a,i3))/(2.*dsb)
  a22s =-(-3.*A22(i1,i2,i3)+4.*A22(i1-js1a,i2-js2a,i3)-A22(i1-2*js1a,i2-2*js2a,i3))/(2.*dsb)
! if( debug.gt.0 )then
!   write(*,'(" ghost-interp:left-shift i=",3i3," js1a,js2a=",2i3," c2r,c2s2(-1),c2s2(-2)=",10e10.2)')\
!      i1,i2,i3,js1a,js2a,c2r,C2s2(i1-js1a,i2-js2a,i3),C2s2(i1-2*js1a,i2-2*js2a,i3)
! end if
 else if( (i1+3*js1a).le.md1b .and. \
          (i2+3*js2a).le.md2b )then
  ! one sided  2nd-order:
  c1 = 2.*C1Order2(i1+js1a,i2+js2a,i3)-C1Order2(i1+2*js1a,i2+2*js2a,i3)
  c2 = 2.*C2Order2(i1+js1a,i2+js2a,i3)-C2Order2(i1+2*js1a,i2+2*js2a,i3)
  if( axis.eq.0 )then
    c1r = 2.*C1r2(i1+js1a,i2+js2a,i3)-C1r2(i1+2*js1a,i2+2*js2a,i3)
    c2r = 2.*C2r2(i1+js1a,i2+js2a,i3)-C2r2(i1+2*js1a,i2+2*js2a,i3)
  else
    c1r = 2.*C1s2(i1+js1a,i2+js2a,i3)-C1s2(i1+2*js1a,i2+2*js2a,i3)
    c2r = 2.*C2s2(i1+js1a,i2+js2a,i3)-C2s2(i1+2*js1a,i2+2*js2a,i3)
  end if
! if( debug.gt.0 )then
!   write(*,'(" ghost-interp:right-shift i=",3i3," js1a,js2a=",2i3," c2r,c2s2(+1),c2s2(+2)=",10e10.2)')\
!      i1,i2,i3,js1a,js2a,c2r,C2s2(i1+js1a,i2+js2a,i3),C2s2(i1+2*js1a,i2+2*js2a,i3)
! end if
  a11s = (-3.*A11(i1,i2,i3)+4.*A11(i1+js1a,i2+js2a,i3)-A11(i1+2*js1a,i2+2*js2a,i3))/(2.*dsb)
  a12s = (-3.*A12(i1,i2,i3)+4.*A12(i1+js1a,i2+js2a,i3)-A12(i1+2*js1a,i2+2*js2a,i3))/(2.*dsb)
  a21s = (-3.*A21(i1,i2,i3)+4.*A21(i1+js1a,i2+js2a,i3)-A21(i1+2*js1a,i2+2*js2a,i3))/(2.*dsb)
  a22s = (-3.*A22(i1,i2,i3)+4.*A22(i1+js1a,i2+js2a,i3)-A22(i1+2*js1a,i2+2*js2a,i3))/(2.*dsb)
 else
  ! this case should not happen
  stop 44066
 end if


 ! ***** Now do "s"-derivatives *****
 ! warning -- the compiler could still try to evaluate the mask at an invalid point
 if( (i1-js1a).ge.md1a .and. (i2-js2a).ge.md2a .and. mask(i1-js1a,i2-js2a,i3).ne.0 .and. \
     (i1+js1a).le.md1b .and. (i2+js2a).le.md2b .and. mask(i1+js1a,i2+js2a,i3).ne.0 )then
   us=US2(ex)
   vs=US2(ey)
   ws=US2(hz)

   uss=USS2(ex)
   vss=USS2(ey)
   wss=USS2(hz)

 else if( (i1-2*js1a).ge.md1a .and. \
          (i2-2*js2a).ge.md2a .and. \
           mask(i1-js1a,i2-js2a,i3).ne.0 .and. mask(i1-2*js1a,i2-2*js2a,i3).ne.0 )then

  ! 2nd-order one-sided: ** note ** use ds not dsa
  us = USM(i1,i2,i3,js1a,js2a,0,dsb,ex)
  vs = USM(i1,i2,i3,js1a,js2a,0,dsb,ey)
  ws = USM(i1,i2,i3,js1a,js2a,0,dsb,hz)

  uss = USSM(i1,i2,i3,js1a,js2a,0,dsb,ex)
  vss = USSM(i1,i2,i3,js1a,js2a,0,dsb,ey)
  wss = USSM(i1,i2,i3,js1a,js2a,0,dsb,hz)

 else if( (i1+2*js1a).le.md1b .and. \
          (i2+2*js2a).le.md2b .and.  \
          mask(i1+js1a,i2+js2a,i3).ne.0 .and. mask(i1+2*js1a,i2+2*js2a,i3).ne.0 )then

  ! 2nd-order one-sided:
  us = USP(i1,i2,i3,js1a,js2a,0,dsb,ex)
  vs = USP(i1,i2,i3,js1a,js2a,0,dsb,ey)
  ws = USP(i1,i2,i3,js1a,js2a,0,dsb,hz)

  uss = USSP(i1,i2,i3,js1a,js2a,0,dsb,ex)
  vss = USSP(i1,i2,i3,js1a,js2a,0,dsb,ey)
  wss = USSP(i1,i2,i3,js1a,js2a,0,dsb,hz)

 else 
   ! this case shouldn't matter
   us=0.
   vs=0.
   ws=0.
   uss=0.
   vss=0.
   wss=0.
 end if


! ******************************* end NEW ************************

 tau1=rsxy(i1,i2,i3,axisp1,0)
 tau2=rsxy(i1,i2,i3,axisp1,1)

 uex=u(i1,i2,i3,ex)
 uey=u(i1,i2,i3,ey)

 ! forcing terms for TZ are stored in 
 gIVf=0.            ! forcing for extrap tau.u
 tau1DotUtt=0.      ! forcing for tau.Lu=0
 Da1DotU=0.         ! forcing for div(u)=0

 ! for Hz (w)
 fw1=0.
 fw2=0.

 #If #FORCING == "twilightZone"

   ! For TZ: utt0 = utt - ett + Lap(e)
   call ogDeriv(ep, 0, 2,0,0, xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,ex, uxx)
   call ogDeriv(ep, 0, 0,2,0, xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,ex, uyy)
   utt00=uxx+uyy
  
   call ogDeriv(ep, 0, 2,0,0, xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,ey, vxx)
   call ogDeriv(ep, 0, 0,2,0, xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,ey, vyy)
   vtt00=vxx+vyy

  tau1DotUtt = tau1*utt00+tau2*vtt00

  OGF2D(i1,i2,i3,t, uv0(0),uv0(1),uv0(2))
  OGF2D(i1-is1,i2-is2,i3,t, uvm(0),uvm(1),uvm(2))
  OGF2D(i1+is1,i2+is2,i3,t, uvp(0),uvp(1),uvp(2))
  OGF2D(i1-2*is1,i2-2*is2,i3,t, uvm2(0),uvm2(1),uvm2(2))
  OGF2D(i1+2*is1,i2+2*is2,i3,t, uvp2(0),uvp2(1),uvp2(2))

  ! Da1DotU = (a1.uv).r to 4th order
  Da1DotU = (8.*( (a11p1*uvp(0)+a12p1*uvp(1)) - (a11m1*uvm(0)+a12m1*uvm(1)) )\
              - ( (a11p2*uvp2(0)+a12p2*uvp2(1)) - (a11m2*uvm2(0)+a12m2*uvm2(1)) ) )/(12.*dra)


 ! for now remove the error in the extrapolation ************
 ! gIVf = tau1*( uvm2(0)-4.*uvm(0)+6.*uv0(0)-4.*uvp(0)+uvp2(0)) +\
 !        tau2*( uvm2(1)-4.*uvm(1)+6.*uv0(1)-4.*uvp(1)+uvp2(1))
 gIVf=0.


!  a21zp1= A21(i1+js1,i2+js2,i3) 
!  a21zm1= A21(i1-js1,i2-js2,i3) 
!  a21zp2= A21(i1+2*js1,i2+2*js2,i3) 
!  a21zm2= A21(i1-2*js1,i2-2*js2,i3) 
!
!  a22zp1= A22(i1+js1,i2+js2,i3) 
!  a22zm1= A22(i1-js1,i2-js2,i3) 
!  a22zp2= A22(i1+2*js1,i2+2*js2,i3) 
!  a22zm2= A22(i1-2*js1,i2-2*js2,i3) 
!
!  ! *** set to - Ds( a2.uv )
!  Da1DotU = -(  \
!       ( 8.*(a21zp1*u(i1+  js1,i2+  js2,i3,ex)-a21zm1*u(i1-  js1,i2-  js2,i3,ex)) \
!           -(a21zp2*u(i1+2*js1,i2+2*js2,i3,ex)-a21zm2*u(i1-2*js1,i2-2*js2,i3,ex)) )/(12.*dsa) \
!      +( 8.*(a22zp1*u(i1+  js1,i2+  js2,i3,ey)-a22zm1*u(i1-  js1,i2-  js2,i3,ey)) \
!           -(a22zp2*u(i1+2*js1,i2+2*js2,i3,ey)-a22zm2*u(i1-2*js1,i2-2*js2,i3,ey)) )/(12.*dsa)  )


 ! ***** Forcing for Hz ******
 ! (w).r = fw1                              (w.n = 0 )
 ! (c11*w.rr + c22*w.ss + ... ).r = fw2     ( (Delta w).n = 0 )

 fw1= (8.*(uvp(2)-uvm(2))-(uvp2(2)-uvm2(2)))/(12.*dra) 

 wrrr=(uvp2(2)-2.*(uvp(2)-uvm(2))-uvm2(2))/(2.*dra**3)

 ! wrr=(uvp(2)-2.*uv0(2)+uvm(2))/(dra**2)
 wrr=(-30.*uv0(2)+16.*(uvp(2)+uvm(2))-(uvp2(2)+uvm2(2)) )/(12.*dra**2)
 ! wr=(uvp(2)-uvm(2))/(2.*dra)
 wr=(8.*(uvp(2)-uvm(2))-(uvp2(2)-uvm2(2)))/(12.*dra) 
 fw2= c11*wrrr + (c1+c11r)*wrr + c1r*wr

 ! for tangential derivatives:
 OGF2D(i1-js1,i2-js2,i3,t, uvm(0),uvm(1),uvm(2))
 OGF2D(i1+js1,i2+js2,i3,t, uvp(0),uvp(1),uvp(2))
 OGF2D(i1-2*js1,i2-2*js2,i3,t, uvm2(0),uvm2(1),uvm2(2))
 OGF2D(i1+2*js1,i2+2*js2,i3,t, uvp2(0),uvp2(1),uvp2(2))

 ! These approximations should be consistent with the approximations for ws and wss above
 ! fw2=fw2 + c22r*(uvp(2)-2.*uv0(2)+uvm(2))/(dsa**2)+c2r*(uvp(2)-uvm(2))/(2.*dsa)
 fw2=fw2 + c22r*(-30.*uv0(2)+16.*(uvp(2)+uvm(2))-(uvp2(2)+uvm2(2)) )/(12.*dsa**2)\
         + c2r*(8.*(uvp(2)-uvm(2))-(uvp2(2)-uvm2(2)))/(12.*dsa) 



 #End


! assign values using extrapolation of the normal component:
#Include "bc4eMaxwell.h"

! Now assign Hz at the ghost points

#Include "bc4HzMaxwell.h"


 if( debug.gt.0 )then
  write(*,'(" ghost-interp: i=",3i3," ex=",e10.2," assign i=",3i3," ex=",e10.2," i=",3i3," ex=",e10.2)')\
     i1,i2,i3,u(i1,i2,i3,ex),i1-is1,i2-is2,i3-is3,u(i1-is1,i2-is2,i3-is3,ex),i1-2*is1,i2-2*is2,i3-2*is3,u(i1-2*is1,i2-2*is2,i3-2*is3,ex)

  det = (5.*tau2*a11*a12m2*tau1-5.*tau2**2*a11*a11m2-5.*a12*tau1**2*a12m2+5.*a12*tau1*tau2*a11m2-8.*tau2*a11*a12m1*tau1-8.*a12*tau1*tau2*a11m1+8.*tau2**2*a11*a11m1+8.*a12*tau1**2*a12m1)

  write(*,'(" ghost-interp: det=",e10.2," tau1,tau2,a11,a11m1,a11m2,a12,a12m1,a12m2=",10f8.4)') det,tau1,tau2,a11,a11m1,a11m2,a12,a12m1,a12m2

  write(*,'(" ghost-interp: gIII,tauU,tauUp1,tauUp2,gIV,ttu1,ttu2,c11,c1,dra=",10f8.3)') gIII,tauU,tauUp1,tauUp2,gIV,ttu1,ttu2,c11,c1,dra

  write(*,'(" ghost-interp: c1r,c2r,c22,uss,c2,us,vss,vs=",10e10.2)') c1r,c2r,c22,uss,c2,us,vss,vs

  OGF2D(i1-is1,i2-is2,i3,t, uvm(0),uvm(1),uvm(2))
  OGF2D(i1-2*is1,i2-2*is2,i3,t, uvm2(0),uvm2(1),uvm2(2))

  write(*,'(" .............tau1DotUtt,Da1DotU,us,uss=",4e11.3)') tau1DotUtt,Da1DotU,us,uss
  write(*,'(" .............err: ex(-1,-2)=",2e10.3,", ey(-1,-2)=",2e10.2,", hz(-1,-2)=",2e10.2)') \
           u(i1-is1,i2-is2,i3-is3,ex)-uvm(0),u(i1-2*is1,i2-2*is2,i3-2*is3,ex)-uvm2(0),\
           u(i1-is1,i2-is2,i3-is3,ey)-uvm(1),u(i1-2*is1,i2-2*is2,i3-2*is3,ey)-uvm2(1),\
           u(i1-is1,i2-is2,i3-is3,hz)-uvm(2),u(i1-2*is1,i2-2*is2,i3-2*is3,hz)-uvm2(2)

 end if

 ! ** NO NEED TO DO ALL THE ABOVE IF WE DO THIS:
 extrapInterpGhost=.true. 
 if( extrapInterpGhost )then
   ! extrapolate ghost points next to boundary interpolation points  *wdh* 2015/05/30 
   write(*,'(" extrap ghost next to interp")')
   u(i1-is1,i2-is2,i3-is3,ex) = extrap5(ex,i1,i2,i3,is1,is2,is3)
   u(i1-is1,i2-is2,i3-is3,ey) = extrap5(ey,i1,i2,i3,is1,is2,is3)
   u(i1-is1,i2-is2,i3-is3,hz) = extrap5(hz,i1,i2,i3,is1,is2,is3)

   u(i1-2*is1,i2-2*is2,i3-2*is3,ex) = extrap5(ex,i1-is1,i2-is2,i3-is3,is1,is2,is3)
   u(i1-2*is1,i2-2*is2,i3-2*is3,ey) = extrap5(ey,i1-is1,i2-is2,i3-is3,is1,is2,is3)
   u(i1-2*is1,i2-2*is2,i3-2*is3,hz) = extrap5(hz,i1-is1,i2-is2,i3-is3,is1,is2,is3)

 end if


end if ! mask>0
endLoops()

if( debug.gt.0 )then

! ============================DEBUG=======================================================
#If #FORCING == "twilightZone"

! **** check that we satisfy all the equations ****
maxDivc=0.
maxTauDotLapu=0.
maxExtrap=0.
maxDr3aDotU=0.

beginLoops()
if( mask(i1,i2,i3).gt.0 )then

 tau1=rsxy(i1,i2,i3,axisp1,0)
 tau2=rsxy(i1,i2,i3,axisp1,1)

 tauU=tau1*u(i1,i2,i3,ex)+tau2*u(i1,i2,i3,ey)

 div = ux42(i1,i2,i3,ex)+uy42(i1,i2,i3,ey)

 a11= A11(i1,i2,i3) 
 a12= A12(i1,i2,i3) 
 jac=1./RXDET2D(i1,i2,i3)
 a21 =rsxy(i1,i2,i3,axisp1,0)*jac
 a22 =rsxy(i1,i2,i3,axisp1,1)*jac

 a11p1= A11(i1+  is1,i2+  is2,i3) 
 a11m1= A11(i1-  is1,i2-  is2,i3) 
 a11p2= A11(i1+2*is1,i2+2*is2,i3) 
 a11m2= A11(i1-2*is1,i2-2*is2,i3) 

 a12p1= A12(i1+  is1,i2+  is2,i3) 
 a12m1= A12(i1-  is1,i2-  is2,i3) 
 a12p2= A12(i1+2*is1,i2+2*is2,i3) 
 a12m2= A12(i1-2*is1,i2-2*is2,i3) 



 a21zp1= A21(i1+  js1,i2+  js2,i3) 
 a21zm1= A21(i1-  js1,i2-  js2,i3) 
 a21zp2= A21(i1+2*js1,i2+2*js2,i3) 
 a21zm2= A21(i1-2*js1,i2-2*js2,i3) 

 a22zp1= A22(i1+  js1,i2+  js2,i3) 
 a22zm1= A22(i1-  js1,i2-  js2,i3) 
 a22zp2= A22(i1+2*js1,i2+2*js2,i3) 
 a22zm2= A22(i1-2*js1,i2-2*js2,i3) 

 divc= ( 8.*(a11p1*u(i1+  is1,i2+  is2,i3,ex)-a11m1*u(i1-  is1,i2-  is2,i3,ex)) \
           -(a11p2*u(i1+2*is1,i2+2*is2,i3,ex)-a11m2*u(i1-2*is1,i2-2*is2,i3,ex)) )/(12.*dra) \
      +( 8.*(a12p1*u(i1+  is1,i2+  is2,i3,ey)-a12m1*u(i1-  is1,i2-  is2,i3,ey)) \
           -(a12p2*u(i1+2*is1,i2+2*is2,i3,ey)-a12m2*u(i1-2*is1,i2-2*is2,i3,ey)) )/(12.*dra) \
      +( 8.*(a21zp1*u(i1+  js1,i2+  js2,i3,ex)-a21zm1*u(i1-  js1,i2-  js2,i3,ex)) \
           -(a21zp2*u(i1+2*js1,i2+2*js2,i3,ex)-a21zm2*u(i1-2*js1,i2-2*js2,i3,ex)) )/(12.*dsa) \
      +( 8.*(a22zp1*u(i1+  js1,i2+  js2,i3,ey)-a22zm1*u(i1-  js1,i2-  js2,i3,ey)) \
           -(a22zp2*u(i1+2*js1,i2+2*js2,i3,ey)-a22zm2*u(i1-2*js1,i2-2*js2,i3,ey)) )/(12.*dsa) 

 divc=divc*RXDET2D(i1,i2,i3)

 divc2= (a11p1*u(i1+  is1,i2+  is2,i3,ex)-a11m1*u(i1-  is1,i2-  is2,i3,ex))/(2.*dra) \
       +(a12p1*u(i1+  is1,i2+  is2,i3,ey)-a12m1*u(i1-  is1,i2-  is2,i3,ey))/(2.*dra) \
       +(a21zp1*u(i1+  js1,i2+  js2,i3,ex)-a21zm1*u(i1-  js1,i2-  js2,i3,ex))/(2.*dsa) \
       +(a22zp1*u(i1+  js1,i2+  js2,i3,ey)-a22zm1*u(i1-  js1,i2-  js2,i3,ey))/(2.*dsa)

 divc2=divc2*RXDET2D(i1,i2,i3)

 tauUp1=tau1*(u(i1-2*is1,i2-2*is2,i3,ex)-4.*u(i1-  is1,i2-  is2,i3,ex)+6.*u(i1,i2,i3,ex)\
                                        -4.*u(i1+  is1,i2+  is2,i3,ex)+u(i1+2*is1,i2+2*is2,i3,ex))\
       +tau2*(u(i1-2*is1,i2-2*is2,i3,ey)-4.*u(i1-  is1,i2-  is2,i3,ey)+6.*u(i1,i2,i3,ey)\
                                        -4.*u(i1+  is1,i2+  is2,i3,ey)+u(i1+2*is1,i2+2*is2,i3,ey))

 tauDotLap= tau1*ulaplacian42(i1,i2,i3,ex)+tau2*ulaplacian42(i1,i2,i3,ey)

 c11=C11(i1,i2,i3)
 c22=C22(i1,i2,i3)
 c1=C1Order4(i1,i2,i3)
 c2=C2Order4(i1,i2,i3)

 errLapex=(c11*URR4(ex)+c22*USS4(ex)+c1*UR4(ex)+c2*US4(ex))-ulaplacian42(i1,i2,i3,ex)
 errLapey=(c11*URR4(ey)+c22*USS4(ey)+c1*UR4(ey)+c2*US4(ey))-ulaplacian42(i1,i2,i3,ey)

 
 ! f1 := Dzr(Dpr(Dmr( a11*u + a12*v )))(i1,i2,i3)/dra^3 - cur*Dzr(u)(i1,i2,i3)/dra - cvr*Dzr(v)(i1,i2,i3)/dra - gI:


 ! 
 uex=u(i1,i2,i3,ex)
 uey=u(i1,i2,i3,ey)

 ur=UR4(ex)
 vr=UR4(ey)

 us=US4(ex)
 vs=US4(ey)

 urr=URR4(ex)
 vrr=URR4(ey)

 urs=URS4(ex)
 vrs=URS4(ey)

 uss=USS4(ex)
 vss=USS4(ey)

 urrs=URRS4(ex)
 vrrs=URRS4(ey)

 usss=USSS2(ex)
 vsss=USSS2(ey)

 urrr=URRR2(ex)
 vrrr=URRR2(ey)

 urss=URSS4(ex)
 vrss=URSS4(ey)

!       ursm=-(a12**2*c2*vs+a12**2*c1*vr+a12**2*c22*vss+2*a22*a21s*us*c11-a21r*us*a12*c11-a22r*vs*a12*c11-a22s*vr*a12*c11+a22*a12s*vr*c11+a22*a12r*vs*c11+a22*a12rs*uey*c11+a22*a11s*ur*c11+a22*a11r*us*c11+a22*a11rs*uex*c11-a12rr*uey*a12*c11+a22*a21ss*uex*c11-2*a12r*vr*a12*c11-a21s*ur*a12*c11-a21rs*uex*a12*c11+a22**2*vss*c11+a11*a12*c1*ur+a11*a12*c2*us+a11*a12*c22*uss-2*a11r*ur*a12*c11-a11rr*uex*a12*c11+a22*a21*uss*c11-a22rs*uey*a12*c11+a22*a22ss*uey*c11+2*a22*a22s*vs*c11)/c11/(-a21*a12+a11*a22)

! vrsm =(a21*a11s*ur+a21*a22*vss-a11*a21r*us-a11*a21s*ur-a11*a21rs*uex-a11*a12*vrr-2*a11*a12r*vr-a11*a12rr*uey-a11*a22r*vs-a11*a22s*vr-a11*a22rs*uey+a21*a11r*us+2*a21*a21s*us+a21*a12rs*uey+a21*a12r*vs-a11*a11rr*uex+a21*a12s*vr+a21*a11rs*uex-a11**2*urr+a21*a21ss*uex-2*a11*a11r*ur+a21**2*uss+a21*a22ss*uey+2*a21*a22s*vs)/(a11*a22-a21*a12)

 a11r = DR4($A11)
 a12r = DR4($A12)
 a21r = DR4($A21)
 a22r = DR4($A22)

 a11s = DS4($A11)
 a12s = DS4($A12)
 a21s = DS4($A21)
 a22s = DS4($A22)

 a11rs = Drs4($A11)
 a12rs = Drs4($A12)
 a21rs = Drs4($A21)
 a22rs = Drs4($A22)

 a11rr = DRR4($A11)
 a12rr = DRR4($A12)
 a21rr = DRR4($A21)
 a22rr = DRR4($A22)

 a11ss = DSS4($A11)
 a12ss = DSS4($A12)
 a21ss = DSS4($A21)
 a22ss = DSS4($A22)

 if( .true. )then
   a11sss = DSSS2($A11)
   a12sss = DSSS2($A12)
   a21sss = DSSS2($A21)
   a22sss = DSSS2($A22)
 else ! there are not enough ghost points in general to use:
   a11sss = DSSS4($A11)
   a12sss = DSSS4($A12)
   a21sss = DSSS4($A21)
   a22sss = DSSS4($A22)
 end if



 if( axis.eq.0 )then
   a11rss = Drss4($A11)
   a12rss = Drss4($A12)
   a21rss = Drss4($A21)
   a22rss = Drss4($A22)
 else
   a11rss = Drrs4($A11)
   a12rss = Drrs4($A12)
   a21rss = Drrs4($A21)
   a22rss = Drrs4($A22)
 end if
   
 ! fourth-order:
 c11s = (8.*(C11(i1+  js1,i2+  js2,i3)-C11(i1-  js1,i2-  js2,i3))   \
           -(C11(i1+2*js1,i2+2*js2,i3)-C11(i1-2*js1,i2-2*js2,i3))   )/(12.*dsa)
 c22s = (8.*(C22(i1+  js1,i2+  js2,i3)-C22(i1-  js1,i2-  js2,i3))   \
           -(C22(i1+2*js1,i2+2*js2,i3)-C22(i1-2*js1,i2-2*js2,i3))   )/(12.*dsa)

 if( axis.eq.0 )then
   c1s = C1s4(i1,i2,i3)
   c2s = C2s4(i1,i2,i3)
 else
   c1s = C1r4(i1,i2,i3)
   c2s = C2r4(i1,i2,i3)
 end if

 c11r = (8.*(C11(i1+  is1,i2+  is2,i3)-C11(i1-  is1,i2-  is2,i3))   \
           -(C11(i1+2*is1,i2+2*is2,i3)-C11(i1-2*is1,i2-2*is2,i3))   )/(12.*dra)
 c22r = (8.*(C22(i1+  is1,i2+  is2,i3)-C22(i1-  is1,i2-  is2,i3))   \
           -(C22(i1+2*is1,i2+2*is2,i3)-C22(i1-2*is1,i2-2*is2,i3))   )/(12.*dra)
 
 if( axis.eq.0 )then
   c1r = C1r4(i1,i2,i3)
   c2r = C2r4(i1,i2,i3)
 else
   c1r = C1s4(i1,i2,i3)
   c2r = C2s4(i1,i2,i3)
 end if


 g1a = a21rrs*uex+a22rrs*uey + a21rr*us+a22rr*vs +2.*( a21rs*ur+a22rs*vr +a21r*urs+a22r*vrs ) \
       +a21s*urr+a22s*vrr + a21*urrs+a22*vrrs

  
 g2a=dr3aDotU+g1a

#Include "bcdivMaxwell.h"


 ! forcing terms for TZ are stored in 
 cgI=1.
 gIf=0.
 tau1DotUtt=0.
 Da1DotU=0.

 #If #FORCING == "twilightZone"

  ! ********** For now do this: should work for quadratics *******************
  cgI=0.  
  gIf=0.
  ! ***********************************************

   ! For TZ: utt0 = utt - ett + Lap(e)
   call ogDeriv(ep, 0, 2,0,0, xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,ex, uxx)
   call ogDeriv(ep, 0, 0,2,0, xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,ex, uyy)
   utt00=uxx+uyy
  
   call ogDeriv(ep, 0, 2,0,0, xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,ey, vxx)
   call ogDeriv(ep, 0, 0,2,0, xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,ey, vyy)
   vtt00=vxx+vyy

  tau1DotUtt = tau1*utt00+tau2*vtt00

  ! ***
  tauDotLap = tauDotLap - tau1DotUtt


  OGF2D(i1,i2,i3,t, uv0(0),uv0(1),uv0(2))
  OGF2D(i1-is1,i2-is2,i3,t, uvm(0),uvm(1),uvm(2))
  OGF2D(i1+is1,i2+is2,i3,t, uvp(0),uvp(1),uvp(2))
  OGF2D(i1-2*is1,i2-2*is2,i3,t, uvm2(0),uvm2(1),uvm2(2))
  OGF2D(i1+2*is1,i2+2*is2,i3,t, uvp2(0),uvp2(1),uvp2(2))

  ! Da1DotU = (a1.uv).r to 4th order
  Da1DotU = (8.*( (a11p1*uvp(0)+a12p1*uvp(1)) - (a11m1*uvm(0)+a12m1*uvm(1)) )\
              - ( (a11p2*uvp2(0)+a12p2*uvp2(1)) - (a11m2*uvm2(0)+a12m2*uvm2(1)) ) )/(12.*dra)

 ! for now remove the error in the extrapolation ************
 !  gIVf = tau1*( uvm2(0)-4.*uvm(0)+6.*uv0(0)-4.*uvp(0)+uvp2(0)) +\
 !         tau2*( uvm2(1)-4.*uvm(1)+6.*uv0(1)-4.*uvp(1)+uvp2(1))
 gIVf=0.

  ! *** set to - Ds( a2.uv )
  Da1DotU = -(  \
       ( 8.*(a21zp1*u(i1+  js1,i2+  js2,i3,ex)-a21zm1*u(i1-  js1,i2-  js2,i3,ex)) \
           -(a21zp2*u(i1+2*js1,i2+2*js2,i3,ex)-a21zm2*u(i1-2*js1,i2-2*js2,i3,ex)) )/(12.*dsa) \
      +( 8.*(a22zp1*u(i1+  js1,i2+  js2,i3,ey)-a22zm1*u(i1-  js1,i2-  js2,i3,ey)) \
           -(a22zp2*u(i1+2*js1,i2+2*js2,i3,ey)-a22zm2*u(i1-2*js1,i2-2*js2,i3,ey)) )/(12.*dsa)  )


  tauU= tauU -( tau1*uv0(0)+tau2*uv0(1) )
 #End



 OGF2D(i1,i2,i3,t, uv0(0),uv0(1),uv0(2))
 OGF2D(i1-is1,i2-is2,i3,t, uvm(0),uvm(1),uvm(2))
 OGF2D(i1+is1,i2+is2,i3,t, uvp(0),uvp(1),uvp(2))
 OGF2D(i1-2*is1,i2-2*is2,i3,t, uvm2(0),uvm2(1),uvm2(2))

 write(*,'(/,"  bc4: (i1,i2,i3)=(",i6,",",i6,",",i6,") (side,axis)=(",i2,",",i2,")")') i1,i2,i3,side,axis

 write(*,'("  bc4: u(-1),err, v(-1),err",4e12.3)') u(i1-is1,i2-is2,i3,ex),u(i1-is1,i2-is2,i3,ex)-uvm(0),\
                                                   u(i1-is1,i2-is2,i3,ey),u(i1-is1,i2-is2,i3,ey)-uvm(1)

 write(*,'("  bc4: u(-2),err, v(-2),err",4e12.3)') u(i1-2*is1,i2-2*is2,i3,ex),u(i1-2*is1,i2-2*is2,i3,ex)-uvm2(0),\
                                                      u(i1-2*is1,i2-2*is2,i3,ey),u(i1-2*is1,i2-2*is2,i3,ey)-uvm2(1)

 write(*,'("  bc4: err(tau.u)=",e9.2," div4(u)=",e9.2," divc(u)=",e9.2,", divc2=",e9.2,", tauD+4u=",e9.2)') \
          tauU,div,divc,divc2,tauUp1

 ! write(*,'("  bc4: a11m2,a11m1,a11,a11p1,a11p2=",5e14.6)') a11m2,a11m1,a11,a11p1,a11p2
 ! write(*,'("  bc4: a12m2,a12m1,a12,a12p1,a12p2=",5e14.6)') a12m2,a12m1,a12,a12p1,a12p2
 ! write(*,'("  bc4: a21zm2,a21zm1,a21,a21zp1,a21zp2=",5e14.6)') a21zm2,a21zm1,a21,a21zp1,a21zp2
 ! write(*,'("  bc4: a22zm2,a22zm1,a22,a22zp1,a22zp2=",5e14.6)') a22zm2,a22zm1,a22,a22zp1,a22zp2

 g1a= ( 8.*(a11p1*u(i1+  is1,i2+  is2,i3,ex)-a11m1*u(i1-  is1,i2-  is2,i3,ex)) \
          -(a11p2*u(i1+2*is1,i2+2*is2,i3,ex)-a11m2*u(i1-2*is1,i2-2*is2,i3,ex)) )/(12.*dra) \
     +( 8.*(a12p1*u(i1+  is1,i2+  is2,i3,ey)-a12m1*u(i1-  is1,i2-  is2,i3,ey)) \
          -(a12p2*u(i1+2*is1,i2+2*is2,i3,ey)-a12m2*u(i1-2*is1,i2-2*is2,i3,ey)) )/(12.*dra)


 write(*,'("  bc4: uex,err,uey,err=",4e10.2)') uex,uex-uv0(0),uey,uey-uv0(1)
 write(*,'("  bc4: d(a1.uv),Da1DotU,err=",5e10.2)') g1a,Da1DotU,g1a-Da1DotU


 call ogDeriv(ep, 0, 2,0,0, xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,ex, uxx)
 call ogDeriv(ep, 0, 0,2,0, xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,ex, uyy)
 utt00=uxx+uyy
  
 call ogDeriv(ep, 0, 2,0,0, xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,ey, vxx)
 call ogDeriv(ep, 0, 0,2,0, xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,ey, vyy)
 vtt00=vxx+vyy

 uLap=uLaplacian42(i1,i2,i3,ex)
 vLap=uLaplacian42(i1,i2,i3,ey)
 write(*,'("  bc4: Lu-utt=",e10.2," Lv-vtt=",e10.2," tau.(L\uv-\uvtt)=",e10.2)') \
            uLap-utt00,vLap-vtt00,tau1*(uLap-utt00)+tau2*(vLap-vtt00)

  call ogDeriv(ep, 0, 2,0,0, xy(i1-is1,i2-is2,i3,0),xy(i1-is1,i2-is2,i3,1),0.,t,ex, uxxm1)
  call ogDeriv(ep, 0, 0,2,0, xy(i1-is1,i2-is2,i3,0),xy(i1-is1,i2-is2,i3,1),0.,t,ex, uyym1)
  call ogDeriv(ep, 0, 2,0,0, xy(i1+is1,i2+is2,i3,0),xy(i1+is1,i2+is2,i3,1),0.,t,ex, uxxp1)
  call ogDeriv(ep, 0, 0,2,0, xy(i1+is1,i2+is2,i3,0),xy(i1+is1,i2+is2,i3,1),0.,t,ex, uyyp1)

  call ogDeriv(ep, 0, 2,0,0, xy(i1-is1,i2-is2,i3,0),xy(i1-is1,i2-is2,i3,1),0.,t,ey, vxxm1)
  call ogDeriv(ep, 0, 0,2,0, xy(i1-is1,i2-is2,i3,0),xy(i1-is1,i2-is2,i3,1),0.,t,ey, vyym1)
  call ogDeriv(ep, 0, 2,0,0, xy(i1+is1,i2+is2,i3,0),xy(i1+is1,i2+is2,i3,1),0.,t,ey, vxxp1)
  call ogDeriv(ep, 0, 0,2,0, xy(i1+is1,i2+is2,i3,0),xy(i1+is1,i2+is2,i3,1),0.,t,ey, vyyp1)

  call ogDeriv(ep, 0, 2,0,0, xy(i1-2*is1,i2-2*is2,i3,0),xy(i1-2*is1,i2-2*is2,i3,1),0.,t,ex, uxxm2)
  call ogDeriv(ep, 0, 0,2,0, xy(i1-2*is1,i2-2*is2,i3,0),xy(i1-2*is1,i2-2*is2,i3,1),0.,t,ex, uyym2)
  call ogDeriv(ep, 0, 2,0,0, xy(i1+2*is1,i2+2*is2,i3,0),xy(i1+2*is1,i2+2*is2,i3,1),0.,t,ex, uxxp2)
  call ogDeriv(ep, 0, 0,2,0, xy(i1+2*is1,i2+2*is2,i3,0),xy(i1+2*is1,i2+2*is2,i3,1),0.,t,ex, uyyp2)

  call ogDeriv(ep, 0, 2,0,0, xy(i1-2*is1,i2-2*is2,i3,0),xy(i1-2*is1,i2-2*is2,i3,1),0.,t,ey, vxxm2)
  call ogDeriv(ep, 0, 0,2,0, xy(i1-2*is1,i2-2*is2,i3,0),xy(i1-2*is1,i2-2*is2,i3,1),0.,t,ey, vyym2)
  call ogDeriv(ep, 0, 2,0,0, xy(i1+2*is1,i2+2*is2,i3,0),xy(i1+2*is1,i2+2*is2,i3,1),0.,t,ey, vxxp2)
  call ogDeriv(ep, 0, 0,2,0, xy(i1+2*is1,i2+2*is2,i3,0),xy(i1+2*is1,i2+2*is2,i3,1),0.,t,ey, vyyp2)

  ! For TZ choose bf = bf - Dr( a1.Delta uvExact )
  bf = bf - a11r*utt00 - a12r*vtt00 -a11*( 8.*((uxxp1+uyyp1)-(uxxm1+uyym1))-((uxxp2+uyyp2)-(uxxm2+uyym2)) )/(12.*dra) \
                                    -a12*( 8.*((vxxp1+vyyp1)-(vxxm1+vyym1))-((vxxp2+vyyp2)-(vxxm2+vyym2)) )/(12.*dra)


  divtt=b3u*URRR2(ex)+b3v*URRR2(ey)+b2u*URR4(ex)+b2v*URR4(ey)+b1u*UR4(ex)+b1v*UR4(ey)+bf

  write(*,'("  bc4: divtt=b3u*urrr2(ex)+b3v*urrr2(ey)+...+bf",e10.2)') divtt
  ! write(*,'("  bc4:b3u,b3v,b2u,b2v,b1u,b1v,bf,a11r,a12r=",9e10.2)') b3u,b3v,b2u,b2v,b1u,b1v,bf,a11r,a12r

  divtt=a11 *( c11*urrr+c22*urss+c1*urr+c2*urs + c11r*urr+c22r*uss+c1r*ur+c2r*us ) \
       +a11r*( c11*urr+c22*uss+c1*ur+c2*us ) \
       +a12 *( c11*vrrr+c22*vrss+c1*vrr+c2*vrs + c11r*vrr+c22r*vss+c1r*vr+c2r*vs ) \
       +a12r*( c11*vrr+c22*vss+c1*vr+c2*vs )
  bf =  - a11r*utt00 - a12r*vtt00 -a11*( 8.*((uxxp1+uyyp1)-(uxxm1+uyym1))-((uxxp2+uyyp2)-(uxxm2+uyym2)) )/(12.*dra) \
                                  -a12*( 8.*((vxxp1+vyyp1)-(vxxm1+vyym1))-((vxxp2+vyyp2)-(vxxm2+vyym2)) )/(12.*dra)
  write(*,'("  bc4: (a.Lu).r - rhs=",e10.2," (a.Lu).r=",e10.2," rhs=",e10.2)') divtt+bf,divtt,bf

  ! write(*,'("  bc4: urrr,vrrr,urss,vrss,urr,vrr,urs,vrs,uss,vss,ur,vr,us,vs=",15e10.2)') urrr,vrrr,urss,vrss,urr,vrr,urs,vrs,uss,vss,ur,vr,us,vs
  
 ! gIa+cur*ur+cvr*vr = ( a2.uv).rrs

 ! write(*,'("  bc4: cur,cvr=",2e10.2)') cur,cvr
 ! write(*,'("  bc4: dr3aDotU=",e10.2," gIf=",e10.2," ,err=",e12.4)') dr3aDotU,gIf,dr3aDotU-gIf

 tauDotExtrap=tau1*( u(i1-2*is1,i2-2*is2,i3,ex)-4.*u(i1-is1,i2-is2,i3,ex)+6.*u(i1,i2,i3,ex) \
                                               -4.*u(i1+is1,i2+is2,i3,ex)+u(i1+2*is1,i2+2*is2,i3,ex)) \
             +tau2*( u(i1-2*is1,i2-2*is2,i3,ey)-4.*u(i1-is1,i2-is2,i3,ey)+6.*u(i1,i2,i3,ey) \
                                               -4.*u(i1+is1,i2+is2,i3,ey)+u(i1+2*is1,i2+2*is2,i3,ey))
 write(*,'("  bc4: tauDotD+4(uv)-gIVf=",2e10.2)') tauDotExtrap-gIVf
 tauDotExtrap=tau1*( u(i1-2*is1,i2-2*is2,i3,ex)-5.*u(i1-is1,i2-is2,i3,ex)+10.*u(i1,i2,i3,ex) \
                                              -10.*u(i1+is1,i2+is2,i3,ex)+5.*u(i1+2*is1,i2+2*is2,i3,ex)-u(i1+3*is1,i2+3*is2,i3,ex)) \
             +tau2*( u(i1-2*is1,i2-2*is2,i3,ey)-5.*u(i1-is1,i2-is2,i3,ey)+10.*u(i1,i2,i3,ey) \
                                              -10.*u(i1+is1,i2+is2,i3,ey)+5.*u(i1+2*is1,i2+2*is2,i3,ey)-u(i1+3*is1,i2+3*is2,i3,ey))
 write(*,'("  bc4: tauDotD+5(uv)-gIVf=",2e10.2)') tauDotExtrap-gIVf

 write(*,'("  bc4: tau.Lap=",e9.2,", err(lap)=",2e9.1," dr3aDotU-cur*Du-cvr*Dv,g1a,sum=",3e10.2)') tauDotLap,errLapex,errLapey,dr3aDotU,g1a,g2a
!write(*,'("  bc4: gIa=",e10.2') gIa

 write(*,'("  bc4: err(tau1.u(-1,-2))=",2e10.2)')\
    tau1*(u(i1-is1,i2-is2,i3-is3,ex)-uvm(0))+tau2*(u(i1-is1,i2-is2,i3-is3,ey)-uvm(1)),\
    tau1*(u(i1-2*is1,i2-2*is2,i3-2*is3,ex)-uvm2(0))+tau2*(u(i1-2*is1,i2-2*is2,i3-2*is3,ey)-uvm2(1))

  ! ****** compute the error in Dr( a1.Delta u ) + bf = 0
  if( axis.eq.0 )then
    urrr=urrr2(i1,i2,i3,ex)
    urr = urr4(i1,i2,i3,ex)
    ur  =  ur4(i1,i2,i3,ex)
    urs = urs2(i1,i2,i3,ex)  ! don't use a wide stencil since pts missing near boundaries
    urss=urss2(i1,i2,i3,ex)  ! we should maybe use the expression computed from bcdiv3d

    vrrr=urrr2(i1,i2,i3,ey)
    vrr = urr4(i1,i2,i3,ey)
    vr  =  ur4(i1,i2,i3,ey)
    vrs = urs2(i1,i2,i3,ey)
    vrss=urss2(i1,i2,i3,ey)

  else if( axis.eq.1 )then
    urrr=usss2(i1,i2,i3,ex)
    urr = uss4(i1,i2,i3,ex)
    ur  =  us4(i1,i2,i3,ex)
    urs = urs2(i1,i2,i3,ex)
    urss=urrs2(i1,i2,i3,ex)  ! we should maybe use the expression computed from bcdiv3d

    vrrr=usss2(i1,i2,i3,ey)
    vrr = uss4(i1,i2,i3,ey)
    vr  =  us4(i1,i2,i3,ey)
    vrs = urs2(i1,i2,i3,ey)
    vrss=urrs2(i1,i2,i3,ey)

  end if
  drA1DotDeltaU = a11*( c11*urrr+ c22*urss + c1*urr + c2*urs \
                       +c11r*urr+c22r*uss+c1r*ur+c2r*us) + a11r*uLap \
                 +a12*( c11*vrrr+ c22*vrss + c1*vrr + c2*vrs \
                       +c11r*vrr+c22r*vss+c1r*vr+c2r*vs) + a12r*vLap +bf

  write(*,'(" error in (a1.Delta u).r + bf = ",e11.3)') drA1DotDeltaU


 OGF2D(i1,i2,i3,t, uv0(0),uv0(1),uv0(2))
 OGF2D(i1-is1,i2-is2,i3,t, uvm(0),uvm(1),uvm(2))
 OGF2D(i1+is1,i2+is2,i3,t, uvp(0),uvp(1),uvp(2))
 OGF2D(i1-2*is1,i2-2*is2,i3,t, uvm2(0),uvm2(1),uvm2(2))
 OGF2D(i1+2*is1,i2+2*is2,i3,t, uvp2(0),uvp2(1),uvp2(2))

 ! compute exact ur,urr for testing ursm formula
 uex=uv0(0)
 uey=uv0(1)
 
 urr=(-30.*uv0(0)+16.*(uvp(0)+uvm(0))-(uvp2(0)+uvm2(0)) )/(12.*dra**2)
 vrr=(-30.*uv0(1)+16.*(uvp(1)+uvm(1))-(uvp2(1)+uvm2(1)) )/(12.*dra**2)

 ur=(8.*(uvp(0)-uvm(0))-(uvp2(0)-uvm2(0)))/(12.*dra)
 vr=(8.*(uvp(1)-uvm(1))-(uvp2(1)-uvm2(1)))/(12.*dra)

 OGF2D(i1,i2,i3,t, uv0(0),uv0(1),uv0(2))
 OGF2D(i1-js1,i2-js2,i3,t, uvm(0),uvm(1),uvm(2))
 OGF2D(i1+js1,i2+js2,i3,t, uvp(0),uvp(1),uvp(2))
 OGF2D(i1-2*js1,i2-2*js2,i3,t, uvm2(0),uvm2(1),uvm2(2))
 OGF2D(i1+2*js1,i2+2*js2,i3,t, uvp2(0),uvp2(1),uvp2(2))

 uss=(-30.*uv0(0)+16.*(uvp(0)+uvm(0))-(uvp2(0)+uvm2(0)) )/(12.*dsa**2)
 vss=(-30.*uv0(1)+16.*(uvp(1)+uvm(1))-(uvp2(1)+uvm2(1)) )/(12.*dsa**2)

 us=(8.*(uvp(0)-uvm(0))-(uvp2(0)-uvm2(0)))/(12.*dsa)
 vs=(8.*(uvp(1)-uvm(1))-(uvp2(1)-uvm2(1)))/(12.*dsa)

! These are from bcdiv.maple (includes urr,vrr)
 ursm =-(-a22rs*uey*a12-a22s*vr*a12-a22r*vs*a12+a22*a22ss*uey+2*a22*a22s*vs+2*a22*a21s*us+a22*a12rs*uey+a22*a12r*vs+a22*a12s*vr+a22*a11rs*uex+a22*a21ss*uex+a22*a11r*us-a21r*us*a12+a22*a11s*ur-a21s*ur*a12-a12**2*vrr-2*a12r*vr*a12-a21rs*uex*a12+a22*a21*uss+a22**2*vss-a11rr*uex*a12-2*a11r*ur*a12-a11*urr*a12-a12rr*uey*a12)/(a11*a22-a21*a12)
 vrsm =(a21*a11s*ur+a21*a22*vss-a11*a21r*us-a11*a21s*ur-a11*a21rs*uex-a11*a12*vrr-2*a11*a12r*vr-a11*a12rr*uey-a11*a22r*vs-a11*a22s*vr-a11*a22rs*uey+a21*a11r*us+2*a21*a21s*us+a21*a12rs*uey+a21*a12r*vs-a11*a11rr*uex+a21*a12s*vr+a21*a11rs*uex-a11**2*urr+a21*a21ss*uex-2*a11*a11r*ur+a21**2*uss+a21*a22ss*uey+2*a21*a22s*vs)/(a11*a22-a21*a12)


!      urrm=-(c22*uss+c1*ur+c2*us)/c11
!
!      vrrm=-(c22*vss+c1*vr+c2*vs)/c11
!
 write(*,'("  bc4: dra,dsa=",2e10.2)') dra,dsa
 write(*,'("  bc4: ursm,urs=",2e10.2," error=",e9.2)') ursm,urs,ursm-urs
 write(*,'("  bc4: vrsm,vrs=",2e10.2," error=",e9.2," vrs2=",e10.2)') vrsm,vrs,vrsm-vrs,URS2(ey)
 a1Dotur=-( a11r*uex +a12r*uey + a21*us + a22*vs + a21s*uex + a22s*uey )
 write(*,'("  bc4: a1.ur= -( a1r.u + a2.us + a2s.u) =",e10.2," a1.ur=",e10.2," error=",e10.2)') \
       a1Dotur,(a11*ur+a12*vr),a1Dotur-(a11*ur+a12*vr)
 a1Dotur=-( a11r*us +a12r*vs + a21*uss + a22*vss + a21s*us + a22s*vs  \
           +a11rs*uex+a12rs*uey + a21s*us + a22s*vs + a21ss*uex + a22ss*uey )
 write(*,'("  bc4: a1.urs= -( a1r.u...)_s =",e10.2," a1.urs=",e10.2," error=",e10.2)') \
        a1Dotur,(a11*urs+a12*vrs),a1Dotur-(a11*urs+a12*vrs)
 write(*,'("  bc4: a1.ursm=",e10.2," error=",e10.2)') a11*ursm+a12*vrsm,a11*ursm+a12*vrsm-(a11*urs+a12*vrs)
 write(*,'("  bc4: (a1.1).r + (a2.1).s = ",e10.2)') a11r+a12r+a21s+a22s
 write(*,'("  bc4: (a1.1).rr + (a2.1).rs = ",e10.2)') a11rr+a12rr+a21rs+a22rs
 write(*,'("  bc4: (a1.1).rs + (a2.1).ss = ",e10.2)') a11rs+a12rs+a21ss+a22ss
 write(*,'("  bc4: (a1.1).rss + (a2.1).sss = ",e10.2)') a11rss+a12rss+a21sss+a22sss

 write(*,'("  bc4: a11,a12,a21,a22,a11r,a12r,a21r,a22r=",8e11.3)') a11,a12,a21,a22
 write(*,'("  bc4: a11r,a12r,a21r,a22r=",8e11.3)') a11r,a12r,a21r,a22r
 write(*,'("  bc4: a11s,a12s,a21s,a22s=",8e11.3)') a11s,a12s,a21s,a22s
 write(*,'("  bc4: a11rs,a11ss,a12rs,a12ss=",4e11.3)') a11rs,a11ss,a12rs,a12ss
 write(*,'("  bc4: a21rs,a21ss,a22rs,a22ss=",4e11.3)') a21rs,a21ss,a22rs,a22ss

 write(*,'("  bc4:a11r,a11r2=",2e11.3," a12r,a12r2=",2e11.3)') a11r,DR($A11),a12r,DR($A12)
 write(*,'("  bc4:a21r,a21r2=",2e11.3," a22r,a22r2=",2e11.3)') a21r,DR($A21),a22r,DR($A22)

 write(*,'("  bc4:a11s,a11s2=",2e11.3," a12s,a12s2=",2e11.3)') a11s,DS($A11),a12s,DS($A12)
 write(*,'("  bc4:a21s,a21s2=",2e11.3," a22s,a22s2=",2e11.3)') a21s,DS($A21),a22s,DS($A22)

 write(*,'("  bc4:a11rs,a11rs2=",2e11.3," a12rs,a12rs2=",2e11.3)') a11rs,DRS($A11),a12rs,DRS($A12)
 write(*,'("  bc4:a21rs,a21rs2=",2e11.3," a22rs,a22rs2=",2e11.3)') a21rs,DRS($A21),a22rs,DRS($A22)

 write(*,'("  bc4:a11rr,a11rr2=",2e11.3," a12rr,a12rr2=",2e11.3)') a11rr,DRR($A11),a12rr,DRR($A12)
 write(*,'("  bc4:a21rr,a21rr2=",2e11.3," a22rr,a22rr2=",2e11.3)') a21rr,DRR($A21),a22rr,DRR($A22)

 write(*,'("  bc4:a11ss,a11ss2=",2e11.3," a12ss,a12ss2=",2e11.3)') a11ss,DSS($A11),a12ss,DSS($A12)
 write(*,'("  bc4:a21ss,a21ss2=",2e11.3," a22ss,a22ss2=",2e11.3)') a21ss,DSS($A21),a22ss,DSS($A22)

 if( axis.eq.1 )then
   write(*,'("  bc4: a11rss,a12rss=",2e11.3," 2nd-order=",2e11.3)') a11rss,a12rss,Drrs($A11),Drrs($A12)
 end if
!
 a1Doturss =-a11r*uss-a11rss*uex-2*a11rs*us-a11ss*ur-2*a11s*urs-a22*vsss-a12rss*uex-2*a12rs*vs-a12r*vss-2*a12s*vrs-a12ss*vr-a21sss*uex-3*a21ss*us-3*a21s*uss-a21*usss-3*a22ss*vs-a22sss*uex-3*a22s*vss
!
  write(*,'("  bc4:  a1.urss=-a11r*uss-...",e10.2,", a1.urs=",e10.2," error=",e10.2)') \
             a1Doturss,(a11*urss+a12*vrss),a1Doturss-(a11*urss+a12*vrss)
!
!
! write(*,'("  bc4: urrm,urr=",2e10.2," error=",e9.2)') urrm,URR4(ex),urrm-URR4(ex)
! write(*,'("  bc4: vrrm,vrr=",2e10.2," error=",e9.2)') vrrm,URR4(ey),vrrm-URR4(ey)
!
 ! write(*,'("  bc4: a21rrs,a22rrs,a21rr,a22rr,a21rs=",12f7.2)') \
 !                    a21rrs,a22rrs,a21rr,a22rr,a21rs
 ! write(*,'("  bc4: a22rs,a21r,a22r,a21s,a22s,a21,a22=",12f7.2)') \
 !                    a22rs,a21r,a22r,a21s,a22s,a21,a22
 ! write(*,'("  bc4: uex,uey,us,vs,ur,vr,urs,vrs=",12f7.2)')\
 !                    uex,uey,us,vs,ur,vr,urs,vrs 
 ! write(*,'("  bc4: urr,vrr,urrs,vrrs, vrrs2=",12f7.2)')\
 !                    urr,vrr,urrs,vrrs,(urr2(i1,i2+1,i3,ey)-urr2(i1,i2-1,i3,ey))/(2.*dsa)

 ! ***** Check Hz *******
 OGF2D(i1,i2,i3,t, uv0(0),uv0(1),uv0(2))
 OGF2D(i1-is1,i2-is2,i3,t, uvm(0),uvm(1),uvm(2))
 OGF2D(i1+is1,i2+is2,i3,t, uvp(0),uvp(1),uvp(2))
 OGF2D(i1-2*is1,i2-2*is2,i3,t, uvm2(0),uvm2(1),uvm2(2))
 OGF2D(i1+2*is1,i2+2*is2,i3,t, uvp2(0),uvp2(1),uvp2(2))

 fw1= (8.*(uvp(2)-uvm(2))-(uvp2(2)-uvm2(2)))/(12.*dra) 

 wrrr=(uvp2(2)-2.*(uvp(2)-uvm(2))-uvm2(2))/(2.*dra**3)

 ! wrr=(uvp(2)-2.*uv0(2)+uvm(2))/(dra**2)
 wrr=(-30.*uv0(2)+16.*(uvp(2)+uvm(2))-(uvp2(2)+uvm2(2)) )/(12.*dra**2)
 ! wr=(uvp(2)-uvm(2))/(2.*dra)
 wr=(8.*(uvp(2)-uvm(2))-(uvp2(2)-uvm2(2)))/(12.*dra) 
 fw2= c11*wrrr + (c1+c11r)*wrr + c1r*wr

 ! for tangential derivatives:
 OGF2D(i1-js1,i2-js2,i3,t, uvm(0),uvm(1),uvm(2))
 OGF2D(i1+js1,i2+js2,i3,t, uvp(0),uvp(1),uvp(2))
 OGF2D(i1-2*js1,i2-2*js2,i3,t, uvm2(0),uvm2(1),uvm2(2))
 OGF2D(i1+2*js1,i2+2*js2,i3,t, uvp2(0),uvp2(1),uvp2(2))

 ! These approximations should be consistent with the approximations for ws and wss above
 ! fw2=fw2 + c22r*(uvp(2)-2.*uv0(2)+uvm(2))/(dsa**2)+c2r*(uvp(2)-uvm(2))/(2.*dsa)
 fw2=fw2 + c22r*(-30.*uv0(2)+16.*(uvp(2)+uvm(2))-(uvp2(2)+uvm2(2)) )/(12.*dsa**2)\
         + c2r*(8.*(uvp(2)-uvm(2))-(uvp2(2)-uvm2(2)))/(12.*dsa) 


 write(*,'("  bc4: Hz: error in wr-fw1=",e10.2)') UR4(hz)-fw1
! write(*,'("  bc4: Hz: error in (Lw).r-fw2=",e10.2)') \
!                c11*URRR2(hz)+(c1+c11r)*URR2(hz)+c1r*UR2(hz)+c22r*USS2(hz)+c2r*US2(hz) - fw2
 write(*,'("  bc4: Hz: error in (Lw).r-fw2=",e10.2)') \
                c11*URRR2(hz)+(c1+c11r)*URR4(hz)+c1r*UR4(hz)+c22r*USS4(hz)+c2r*US4(hz) - fw2


 maxDivc=max(maxDivc,divc)
 maxTauDotLapu=max(maxTauDotLapu,tauDotLap)
 maxExtrap=max(maxExtrap,tauUp1)
 maxDr3aDotU=max(maxDr3aDotU,g2a)

end if ! mask>0
endLoops()

 write(*,'(" *** side,axis=",2i3," maxDivc=",e8.1,", maxTauDotLapu=",e8.1,", maxExtrap=",e8.1,", maxDr3aDotU=",e8.1," ***** ")') \
          side,axis,maxDivc,maxTauDotLapu,maxExtrap,maxDr3aDotU

#End 


! ============================END DEBUG=======================================================
end if


#endMacro



! ==========================================================================
!  Define some metric (and equation coefficients) terms and their derivatives
!
!  DAr4, DArr4, ... normal derivative
! ==========================================================================
#beginMacro defineMetricDerivatives(DAr4,DAs4,DAt4,DArr4,DAss4,DAtt4,DArs4,DArt4,DAst4,DAsss2,DAttt2,DArss4,DArtt4,DAsst4,DAstt4)

 ! precompute the inverse of the jacobian, used in macros AmnD3J

 i10=i1  ! used by jac3di in macros
 i20=i2
 i30=i3

 do m3=-2,2
 do m2=-2,2
 do m1=-2,2
  jac3di(m1,m2,m3)=1./RXDET3D(i1+m1,i2+m2,i3+m3)
 end do
 end do
 end do

 a11 =A11D3J(i1,i2,i3)
 a12 =A12D3J(i1,i2,i3)
 a13 =A13D3J(i1,i2,i3)

 a21 =A21D3J(i1,i2,i3)
 a22 =A22D3J(i1,i2,i3)
 a23 =A23D3J(i1,i2,i3)

 a31 =A31D3J(i1,i2,i3)
 a32 =A32D3J(i1,i2,i3)
 a33 =A33D3J(i1,i2,i3)

 a11m1 =A11D3J(i1-is1,i2-is2,i3-is3)
 a12m1 =A12D3J(i1-is1,i2-is2,i3-is3)
 a13m1 =A13D3J(i1-is1,i2-is2,i3-is3)

 a11p1 =A11D3J(i1+is1,i2+is2,i3+is3)
 a12p1 =A12D3J(i1+is1,i2+is2,i3+is3)
 a13p1 =A13D3J(i1+is1,i2+is2,i3+is3)

 a11m2 =A11D3J(i1-2*is1,i2-2*is2,i3-2*is3)
 a12m2 =A12D3J(i1-2*is1,i2-2*is2,i3-2*is3)
 a13m2 =A13D3J(i1-2*is1,i2-2*is2,i3-2*is3)

 a11p2 =A11D3J(i1+2*is1,i2+2*is2,i3+2*is3)
 a12p2 =A12D3J(i1+2*is1,i2+2*is2,i3+2*is3)
 a13p2 =A13D3J(i1+2*is1,i2+2*is2,i3+2*is3)

 a11r = D ## DAr4($A11D3J)
 a12r = D ## DAr4($A12D3J)
 a13r = D ## DAr4($A13D3J)

 a21r = D ## DAr4($A21D3J)
 a22r = D ## DAr4($A22D3J)
 a23r = D ## DAr4($A23D3J)

 a31r = D ## DAr4($A31D3J)
 a32r = D ## DAr4($A32D3J)
 a33r = D ## DAr4($A33D3J)

 a11rr = D ## DArr4($A11D3J)
 a12rr = D ## DArr4($A12D3J)
 a13rr = D ## DArr4($A13D3J)

 a21rr = D ## DArr4($A21D3J)
 a22rr = D ## DArr4($A22D3J)
 a23rr = D ## DArr4($A23D3J)

 a31rr = D ## DArr4($A31D3J)
 a32rr = D ## DArr4($A32D3J)
 a33rr = D ## DArr4($A33D3J)

 a11s = D ## DAs4($A11D3J)
 a12s = D ## DAs4($A12D3J)
 a13s = D ## DAs4($A13D3J)

 a21s = D ## DAs4($A21D3J)
 a22s = D ## DAs4($A22D3J)
 a23s = D ## DAs4($A23D3J)

 a31s = D ## DAs4($A31D3J)
 a32s = D ## DAs4($A32D3J)
 a33s = D ## DAs4($A33D3J)

 a11rs = D ## DArs4($A11D3J)
 a12rs = D ## DArs4($A12D3J)
 a13rs = D ## DArs4($A13D3J)

 a21rs = D ## DArs4($A21D3J)
 a22rs = D ## DArs4($A22D3J)
 a23rs = D ## DArs4($A23D3J)

 a31rs = D ## DArs4($A31D3J)
 a32rs = D ## DArs4($A32D3J)
 a33rs = D ## DArs4($A33D3J)

 a11ss = D ## DAss4($A11D3J)
 a12ss = D ## DAss4($A12D3J)
 a13ss = D ## DAss4($A13D3J)

 a21ss = D ## DAss4($A21D3J)
 a22ss = D ## DAss4($A22D3J)
 a23ss = D ## DAss4($A23D3J)

 a31ss = D ## DAss4($A31D3J)
 a32ss = D ## DAss4($A32D3J)
 a33ss = D ## DAss4($A33D3J)

 a11sss = D ## DAsss2($A11D3J)
 a12sss = D ## DAsss2($A12D3J)
 a13sss = D ## DAsss2($A13D3J)

 a21sss = D ## DAsss2($A21D3J)
 a22sss = D ## DAsss2($A22D3J)
 a23sss = D ## DAsss2($A23D3J)

 a31sss = D ## DAsss2($A31D3J)
 a32sss = D ## DAsss2($A32D3J)
 a33sss = D ## DAsss2($A33D3J)


 a11rss = D ## DArss4($A11D3J)
 a12rss = D ## DArss4($A12D3J)
 a13rss = D ## DArss4($A13D3J)

 a21rss = D ## DArss4($A21D3J)
 a22rss = D ## DArss4($A22D3J)
 a23rss = D ## DArss4($A23D3J)

 a31rss = D ## DArss4($A31D3J)
 a32rss = D ## DArss4($A32D3J)
 a33rss = D ## DArss4($A33D3J)

 a11t = D ## DAt4($A11D3J)
 a12t = D ## DAt4($A12D3J)
 a13t = D ## DAt4($A13D3J)

 a21t = D ## DAt4($A21D3J)
 a22t = D ## DAt4($A22D3J)
 a23t = D ## DAt4($A23D3J)

 a31t = D ## DAt4($A31D3J)
 a32t = D ## DAt4($A32D3J)
 a33t = D ## DAt4($A33D3J)

 a11rt = D ## DArt4($A11D3J)
 a12rt = D ## DArt4($A12D3J)
 a13rt = D ## DArt4($A13D3J)

 a21rt = D ## DArt4($A21D3J)
 a22rt = D ## DArt4($A22D3J)
 a23rt = D ## DArt4($A23D3J)

 a31rt = D ## DArt4($A31D3J)
 a32rt = D ## DArt4($A32D3J)
 a33rt = D ## DArt4($A33D3J)

 a11st = D ## DAst4($A11D3J)
 a12st = D ## DAst4($A12D3J)
 a13st = D ## DAst4($A13D3J)

 a21st = D ## DAst4($A21D3J)
 a22st = D ## DAst4($A22D3J)
 a23st = D ## DAst4($A23D3J)

 a31st = D ## DAst4($A31D3J)
 a32st = D ## DAst4($A32D3J)
 a33st = D ## DAst4($A33D3J)

 a11tt = D ## DAtt4($A11D3J)
 a12tt = D ## DAtt4($A12D3J)
 a13tt = D ## DAtt4($A13D3J)

 a21tt = D ## DAtt4($A21D3J)
 a22tt = D ## DAtt4($A22D3J)
 a23tt = D ## DAtt4($A23D3J)

 a31tt = D ## DAtt4($A31D3J)
 a32tt = D ## DAtt4($A32D3J)
 a33tt = D ## DAtt4($A33D3J)

 a11ttt = D ## DAttt2($A11D3J)
 a12ttt = D ## DAttt2($A12D3J)
 a13ttt = D ## DAttt2($A13D3J)

 a21ttt = D ## DAttt2($A21D3J)
 a22ttt = D ## DAttt2($A22D3J)
 a23ttt = D ## DAttt2($A23D3J)

 a31ttt = D ## DAttt2($A31D3J)
 a32ttt = D ## DAttt2($A32D3J)
 a33ttt = D ## DAttt2($A33D3J)


 a11rtt = D ## DArtt4($A11D3J)
 a12rtt = D ## DArtt4($A12D3J)
 a13rtt = D ## DArtt4($A13D3J)

 a21rtt = D ## DArtt4($A21D3J)
 a22rtt = D ## DArtt4($A22D3J)
 a23rtt = D ## DArtt4($A23D3J)

 a31rtt = D ## DArtt4($A31D3J)
 a32rtt = D ## DArtt4($A32D3J)
 a33rtt = D ## DArtt4($A33D3J)

 a11stt = D ## DAstt4($A11D3J)
 a12stt = D ## DAstt4($A12D3J)
 a13stt = D ## DAstt4($A13D3J)

 a21stt = D ## DAstt4($A21D3J)
 a22stt = D ## DAstt4($A22D3J)
 a23stt = D ## DAstt4($A23D3J)

 a31stt = D ## DAstt4($A31D3J)
 a32stt = D ## DAstt4($A32D3J)
 a33stt = D ## DAstt4($A33D3J)

 a11sst = D ## DAsst4($A11D3J)
 a12sst = D ## DAsst4($A12D3J)
 a13sst = D ## DAsst4($A13D3J)

 a21sst = D ## DAsst4($A21D3J)
 a22sst = D ## DAsst4($A22D3J)
 a23sst = D ## DAsst4($A23D3J)

 a31sst = D ## DAsst4($A31D3J)
 a32sst = D ## DAsst4($A32D3J)
 a33sst = D ## DAsst4($A33D3J)


 c11 = C11D3(i1,i2,i3)
 c22 = C22D3(i1,i2,i3)
 c33 = C33D3(i1,i2,i3)

 c1 = C1D3Order4(i1,i2,i3)
 c2 = C2D3Order4(i1,i2,i3)
 c3 = C3D3Order4(i1,i2,i3)

 c11r = (8.*(C11D3(i1+  is1,i2+  is2,i3+  is3)-C11D3(i1-  is1,i2-  is2,i3-  is3))   \
           -(C11D3(i1+2*is1,i2+2*is2,i3+2*is3)-C11D3(i1-2*is1,i2-2*is2,i3-2*is3))   )/(12.*dra)
 c22r = (8.*(C22D3(i1+  is1,i2+  is2,i3+  is3)-C22D3(i1-  is1,i2-  is2,i3-  is3))   \
           -(C22D3(i1+2*is1,i2+2*is2,i3+2*is3)-C22D3(i1-2*is1,i2-2*is2,i3-2*is3))   )/(12.*dra)
 c33r = (8.*(C33D3(i1+  is1,i2+  is2,i3+  is3)-C33D3(i1-  is1,i2-  is2,i3-  is3))   \
           -(C33D3(i1+2*is1,i2+2*is2,i3+2*is3)-C33D3(i1-2*is1,i2-2*is2,i3-2*is3))   )/(12.*dra)

 if( axis.eq.0 )then
   c1r = C1D3r4(i1,i2,i3)
   c2r = C2D3r4(i1,i2,i3)
   c3r = C3D3r4(i1,i2,i3)
 else if( axis.eq.1 )then
   c1r = C1D3s4(i1,i2,i3)
   c2r = C2D3s4(i1,i2,i3)
   c3r = C3D3s4(i1,i2,i3)
 else 
   c1r = C1D3t4(i1,i2,i3)
   c2r = C2D3t4(i1,i2,i3)
   c3r = C3D3t4(i1,i2,i3)
 end if

#endMacro

! ==========================================================================
!  Define some metric (and equation coefficients) terms and their derivatives
!
! Here are the derivatives we can compute directly based on (is1,is2,i3) (js1,js2,js3) (ks1,ks2,k3)
! ==========================================================================
#beginMacro defineMetricDerivatives1()

 ! precompute the inverse of the jacobian, used in macros AmnD3J

 i10=i1  ! used by jac3di in macros
 i20=i2
 i30=i3

 do m3=-2,2
 do m2=-2,2
 do m1=-2,2
  jac3di(m1,m2,m3)=1./RXDET3D(i1+m1,i2+m2,i3+m3)
 end do
 end do
 end do

 a11 =A11D3J(i1,i2,i3)
 a12 =A12D3J(i1,i2,i3)
 a13 =A13D3J(i1,i2,i3)

 a21 =A21D3J(i1,i2,i3)
 a22 =A22D3J(i1,i2,i3)
 a23 =A23D3J(i1,i2,i3)

 a31 =A31D3J(i1,i2,i3)
 a32 =A32D3J(i1,i2,i3)
 a33 =A33D3J(i1,i2,i3)

 a11m1 =A11D3J(i1-is1,i2-is2,i3-is3)
 a12m1 =A12D3J(i1-is1,i2-is2,i3-is3)
 a13m1 =A13D3J(i1-is1,i2-is2,i3-is3)

 a11p1 =A11D3J(i1+is1,i2+is2,i3+is3)
 a12p1 =A12D3J(i1+is1,i2+is2,i3+is3)
 a13p1 =A13D3J(i1+is1,i2+is2,i3+is3)

 a11m2 =A11D3J(i1-2*is1,i2-2*is2,i3-2*is3)
 a12m2 =A12D3J(i1-2*is1,i2-2*is2,i3-2*is3)
 a13m2 =A13D3J(i1-2*is1,i2-2*is2,i3-2*is3)

 a11p2 =A11D3J(i1+2*is1,i2+2*is2,i3+2*is3)
 a12p2 =A12D3J(i1+2*is1,i2+2*is2,i3+2*is3)
 a13p2 =A13D3J(i1+2*is1,i2+2*is2,i3+2*is3)

 a11r = DR4($A11D3J)
 a12r = DR4($A12D3J)
 a13r = DR4($A13D3J)

 a21r = DR4($A21D3J)
 a22r = DR4($A22D3J)
 a23r = DR4($A23D3J)

 a31r = DR4($A31D3J)
 a32r = DR4($A32D3J)
 a33r = DR4($A33D3J)

 a11rr = DRR4($A11D3J)
 a12rr = DRR4($A12D3J)
 a13rr = DRR4($A13D3J)

 a21rr = DRR4($A21D3J)
 a22rr = DRR4($A22D3J)
 a23rr = DRR4($A23D3J)

 a31rr = DRR4($A31D3J)
 a32rr = DRR4($A32D3J)
 a33rr = DRR4($A33D3J)

 a11s = DS4($A11D3J)
 a12s = DS4($A12D3J)
 a13s = DS4($A13D3J)

 a21s = DS4($A21D3J)
 a22s = DS4($A22D3J)
 a23s = DS4($A23D3J)

 a31s = DS4($A31D3J)
 a32s = DS4($A32D3J)
 a33s = DS4($A33D3J)

 a11ss = DSS4($A11D3J)
 a12ss = DSS4($A12D3J)
 a13ss = DSS4($A13D3J)

 a21ss = DSS4($A21D3J)
 a22ss = DSS4($A22D3J)
 a23ss = DSS4($A23D3J)

 a31ss = DSS4($A31D3J)
 a32ss = DSS4($A32D3J)
 a33ss = DSS4($A33D3J)

 a11sss = DSSS2($A11D3J)
 a12sss = DSSS2($A12D3J)
 a13sss = DSSS2($A13D3J)

 a21sss = DSSS2($A21D3J)
 a22sss = DSSS2($A22D3J)
 a23sss = DSSS2($A23D3J)

 a31sss = DSSS2($A31D3J)
 a32sss = DSSS2($A32D3J)
 a33sss = DSSS2($A33D3J)

 a11t = DT4($A11D3J)
 a12t = DT4($A12D3J)
 a13t = DT4($A13D3J)

 a21t = DT4($A21D3J)
 a22t = DT4($A22D3J)
 a23t = DT4($A23D3J)

 a31t = DT4($A31D3J)
 a32t = DT4($A32D3J)
 a33t = DT4($A33D3J)

 a11tt = DTT4($A11D3J)
 a12tt = DTT4($A12D3J)
 a13tt = DTT4($A13D3J)

 a21tt = DTT4($A21D3J)
 a22tt = DTT4($A22D3J)
 a23tt = DTT4($A23D3J)

 a31tt = DTT4($A31D3J)
 a32tt = DTT4($A32D3J)
 a33tt = DTT4($A33D3J)

 a11ttt = DTTT2($A11D3J)
 a12ttt = DTTT2($A12D3J)
 a13ttt = DTTT2($A13D3J)

 a21ttt = DTTT2($A21D3J)
 a22ttt = DTTT2($A22D3J)
 a23ttt = DTTT2($A23D3J)

 a31ttt = DTTT2($A31D3J)
 a32ttt = DTTT2($A32D3J)
 a33ttt = DTTT2($A33D3J)


 c11 = C11D3(i1,i2,i3)
 c22 = C22D3(i1,i2,i3)
 c33 = C33D3(i1,i2,i3)

 c1 = C1D3Order4(i1,i2,i3)
 c2 = C2D3Order4(i1,i2,i3)
 c3 = C3D3Order4(i1,i2,i3)

 c11r = (8.*(C11D3(i1+  is1,i2+  is2,i3+  is3)-C11D3(i1-  is1,i2-  is2,i3-  is3))   \
           -(C11D3(i1+2*is1,i2+2*is2,i3+2*is3)-C11D3(i1-2*is1,i2-2*is2,i3-2*is3))   )/(12.*dra)
 c22r = (8.*(C22D3(i1+  is1,i2+  is2,i3+  is3)-C22D3(i1-  is1,i2-  is2,i3-  is3))   \
           -(C22D3(i1+2*is1,i2+2*is2,i3+2*is3)-C22D3(i1-2*is1,i2-2*is2,i3-2*is3))   )/(12.*dra)
 c33r = (8.*(C33D3(i1+  is1,i2+  is2,i3+  is3)-C33D3(i1-  is1,i2-  is2,i3-  is3))   \
           -(C33D3(i1+2*is1,i2+2*is2,i3+2*is3)-C33D3(i1-2*is1,i2-2*is2,i3-2*is3))   )/(12.*dra)

 if( axis.eq.0 )then
   c1r = C1D3r4(i1,i2,i3)
   c2r = C2D3r4(i1,i2,i3)
   c3r = C3D3r4(i1,i2,i3)
 else if( axis.eq.1 )then
   c1r = C1D3s4(i1,i2,i3)
   c2r = C2D3s4(i1,i2,i3)
   c3r = C3D3s4(i1,i2,i3)
 else 
   c1r = C1D3t4(i1,i2,i3)
   c2r = C2D3t4(i1,i2,i3)
   c3r = C3D3t4(i1,i2,i3)
 end if

#endMacro

! ==========================================================================
!  Define some metric (and equation coefficients) terms and their derivatives
!
! Here are the derivatives that we need to use difference code for each values of axis
! ==========================================================================
#beginMacro defineMetricDerivatives2(DArs4,DArt4,DAst4, DArss4,DArtt4,DAsst4,DAstt4) 

 a11rs = D ## DArs4($A11D3J)
 a12rs = D ## DArs4($A12D3J)
 a13rs = D ## DArs4($A13D3J)

 a21rs = D ## DArs4($A21D3J)
 a22rs = D ## DArs4($A22D3J)
 a23rs = D ## DArs4($A23D3J)

 a31rs = D ## DArs4($A31D3J)
 a32rs = D ## DArs4($A32D3J)
 a33rs = D ## DArs4($A33D3J)

 a11rs = D ## DArs4($A11D3J)
 a12rs = D ## DArs4($A12D3J)
 a13rs = D ## DArs4($A13D3J)

 a21rt = D ## DArt4($A21D3J)
 a22rt = D ## DArt4($A22D3J)
 a23rt = D ## DArt4($A23D3J)

 a31rt = D ## DArt4($A31D3J)
 a32rt = D ## DArt4($A32D3J)
 a33rt = D ## DArt4($A33D3J)

 a11st = D ## DAst4($A11D3J)
 a12st = D ## DAst4($A12D3J)
 a13st = D ## DAst4($A13D3J)

 a21st = D ## DAst4($A21D3J)
 a22st = D ## DAst4($A22D3J)
 a23st = D ## DAst4($A23D3J)

 a31st = D ## DAst4($A31D3J)
 a32st = D ## DAst4($A32D3J)
 a33st = D ## DAst4($A33D3J)

 a11rss = D ## DArss4($A11D3J)
 a12rss = D ## DArss4($A12D3J)
 a13rss = D ## DArss4($A13D3J)

 a21rss = D ## DArss4($A21D3J)
 a22rss = D ## DArss4($A22D3J)
 a23rss = D ## DArss4($A23D3J)

 a31rss = D ## DArss4($A31D3J)
 a32rss = D ## DArss4($A32D3J)
 a33rss = D ## DArss4($A33D3J)

 a11rtt = D ## DArtt4($A11D3J)
 a12rtt = D ## DArtt4($A12D3J)
 a13rtt = D ## DArtt4($A13D3J)

 a21rtt = D ## DArtt4($A21D3J)
 a22rtt = D ## DArtt4($A22D3J)
 a23rtt = D ## DArtt4($A23D3J)

 a31rtt = D ## DArtt4($A31D3J)
 a32rtt = D ## DArtt4($A32D3J)
 a33rtt = D ## DArtt4($A33D3J)

 a11stt = D ## DAstt4($A11D3J)
 a12stt = D ## DAstt4($A12D3J)
 a13stt = D ## DAstt4($A13D3J)

 a21stt = D ## DAstt4($A21D3J)
 a22stt = D ## DAstt4($A22D3J)
 a23stt = D ## DAstt4($A23D3J)

 a31stt = D ## DAstt4($A31D3J)
 a32stt = D ## DAstt4($A32D3J)
 a33stt = D ## DAstt4($A33D3J)

 a11sst = D ## DAsst4($A11D3J)
 a12sst = D ## DAsst4($A12D3J)
 a13sst = D ## DAsst4($A13D3J)

 a21sst = D ## DAsst4($A21D3J)
 a22sst = D ## DAsst4($A22D3J)
 a23sst = D ## DAsst4($A23D3J)

 a31sst = D ## DAsst4($A31D3J)
 a32sst = D ## DAsst4($A32D3J)
 a33sst = D ## DAsst4($A33D3J)

#endMacro



!================================================================================
! Compute tangential derivatives
!================================================================================
#beginMacro getTangentialDerivatives(VS,VSS,VSSS,VT,VTT,VTTT, VST,VSST,VSTT)
 us=VS(i1,i2,i3,ex)
 uss=VSS(i1,i2,i3,ex)
 usss=VSSS(i1,i2,i3,ex)

 vs=VS(i1,i2,i3,ey)
 vss=VSS(i1,i2,i3,ey)
 vsss=VSSS(i1,i2,i3,ey)

 ws=VS(i1,i2,i3,ez)
 wss=VSS(i1,i2,i3,ez)
 wsss=VSSS(i1,i2,i3,ez)

 ut=VT(i1,i2,i3,ex)
 utt=VTT(i1,i2,i3,ex)
 uttt=VTTT(i1,i2,i3,ex)

 vt=VT(i1,i2,i3,ey)
 vtt=VTT(i1,i2,i3,ey)
 vttt=VTTT(i1,i2,i3,ey)

 wt=VT(i1,i2,i3,ez)
 wtt=VTT(i1,i2,i3,ez)
 wttt=VTTT(i1,i2,i3,ez)

 ust=VST(i1,i2,i3,ex)
 usst=VSST(i1,i2,i3,ex)
 ustt=VSTT(i1,i2,i3,ex)

 vst=VST(i1,i2,i3,ey)
 vsst=VSST(i1,i2,i3,ey)
 vstt=VSTT(i1,i2,i3,ey)

 wst=VST(i1,i2,i3,ez)
 wsst=VSST(i1,i2,i3,ez)
 wstt=VSTT(i1,i2,i3,ez)

#endMacro

!================================================================================
! Compute tangential derivatives
!
! Here are the derivatives we can compute directly based on (is1,is2,i3) (js1,js2,js3) (ks1,ks2,k3)
!================================================================================
#beginMacro getTangentialDerivatives1()

 us=US4(ex)
 uss=USS4(ex)
 usss=USSS2(ex)

 vs=US4(ey)
 vss=USS4(ey)
 vsss=USSS2(ey)

 ws=US4(ez)
 wss=USS4(ez)
 wsss=USSS2(ez)

 ut=UT4(ex)
 utt=UTT4(ex)
 uttt=UTTT2(ex)

 vt=UT4(ey)
 vtt=UTT4(ey)
 vttt=UTTT2(ey)

 wt=UT4(ez)
 wtt=UTT4(ez)
 wttt=UTTT2(ez)

#endMacro

! ======================================================================================
! Here are the derivatives that we need to use difference code for each values of axis
! ======================================================================================
#beginMacro getTangentialDerivatives2(VST,VSST,VSTT)

 ust=VST(i1,i2,i3,ex)
 usst=VSST(i1,i2,i3,ex)
 ustt=VSTT(i1,i2,i3,ex)

 vst  =VST(i1,i2,i3,ey)
 vsst=VSST(i1,i2,i3,ey)
 vstt=VSTT(i1,i2,i3,ey)

 wst  =VST(i1,i2,i3,ez)
 wsst=VSST(i1,i2,i3,ez)
 wstt=VSTT(i1,i2,i3,ez)

#endMacro


! ==========================================================================
!  Define some metric (and equation coefficients) terms and their derivatives
!  **** for the extrapolation case ***
! Here are the derivatives we can compute directly based on (is1,is2,i3) (js1,js2,js3) (ks1,ks2,k3)
! ==========================================================================
#beginMacro defineMetricDerivativesExtrap()

 ! precompute the inverse of the jacobian, used in macros AmnD3J

 i10=i1  ! used by jac3di in macros
 i20=i2
 i30=i3

 do m3=-2,2
 do m2=-2,2
 do m1=-2,2
  jac3di(m1,m2,m3)=1./RXDET3D(i1+m1,i2+m2,i3+m3)
 end do
 end do
 end do

 a11 =A11D3J(i1,i2,i3)
 a12 =A12D3J(i1,i2,i3)
 a13 =A13D3J(i1,i2,i3)

 a21 =A21D3J(i1,i2,i3)
 a22 =A22D3J(i1,i2,i3)
 a23 =A23D3J(i1,i2,i3)

 a31 =A31D3J(i1,i2,i3)
 a32 =A32D3J(i1,i2,i3)
 a33 =A33D3J(i1,i2,i3)

 a11m1 =A11D3J(i1-is1,i2-is2,i3-is3)
 a12m1 =A12D3J(i1-is1,i2-is2,i3-is3)
 a13m1 =A13D3J(i1-is1,i2-is2,i3-is3)

 a11p1 =A11D3J(i1+is1,i2+is2,i3+is3)
 a12p1 =A12D3J(i1+is1,i2+is2,i3+is3)
 a13p1 =A13D3J(i1+is1,i2+is2,i3+is3)

 a11m2 =A11D3J(i1-2*is1,i2-2*is2,i3-2*is3)
 a12m2 =A12D3J(i1-2*is1,i2-2*is2,i3-2*is3)
 a13m2 =A13D3J(i1-2*is1,i2-2*is2,i3-2*is3)

 a11p2 =A11D3J(i1+2*is1,i2+2*is2,i3+2*is3)
 a12p2 =A12D3J(i1+2*is1,i2+2*is2,i3+2*is3)
 a13p2 =A13D3J(i1+2*is1,i2+2*is2,i3+2*is3)

 c11 = C11D3(i1,i2,i3)
 c22 = C22D3(i1,i2,i3)
 c33 = C33D3(i1,i2,i3)

 c1 = C1D3Order4(i1,i2,i3)
 c2 = C2D3Order4(i1,i2,i3)
 c3 = C3D3Order4(i1,i2,i3)

#endMacro

!================================================================================
! Compute tangential derivatives for extrapolation case
!
! Here are the derivatives we can compute directly based on (is1,is2,i3) (js1,js2,js3) (ks1,ks2,k3)
!================================================================================
#beginMacro getTangentialDerivativesExtrap()

 us=US4(ex)
 uss=USS4(ex)

 vs=US4(ey)
 vss=USS4(ey)

 ws=US4(ez)
 wss=USS4(ez)

 ut=UT4(ex)
 utt=UTT4(ex)

 vt=UT4(ey)
 vtt=UTT4(ey)

 wt=UT4(ez)
 wtt=UTT4(ez)

#endMacro

! 2nd-order One-sided approximations:
#defineMacro USM(i1,i2,i3,n1,n2,n3,drr,cc) \
         (-(-3.*u(i1,i2,i3,cc)+4.*u(i1-n1,i2-n2,i3-n3,cc)-u(i1-2*n1,i2-2*n2,i3-2*n3,cc))/(2.*drr))
#defineMacro USSM(i1,i2,i3,n1,n2,n3,drr,cc) \
         ((2.*u(i1,i2,i3,cc)-5.*u(i1-n1,i2-n2,i3-n3,cc)+4.*u(i1-2*n1,i2-2*n2,i3-2*n3,cc)\
             -u(i1-3*n1,i2-3*n2,i3-3*n3,cc))/(drr**2) )

#defineMacro USP(i1,i2,i3,n1,n2,n3,drr,cc) \
         ( (-3.*u(i1,i2,i3,cc)+4.*u(i1+n1,i2+n2,i3+n3,cc)-u(i1+2*n1,i2+2*n2,i3+2*n3,cc))/(2.*drr))
#defineMacro USSP(i1,i2,i3,n1,n2,n3,drr,cc) \
         ((2.*u(i1,i2,i3,cc)-5.*u(i1+n1,i2+n2,i3+n3,cc)+4.*u(i1+2*n1,i2+2*n2,i3+2*n3,cc)\
             -u(i1+3*n1,i2+3*n2,i3+3*n3,cc))/(drr**2) )

!=============================================================================================
!  BCs for curvilinear grids in 3D
!
! Note:
!   The equations are generated assuming that r is the normal direction.
!   We need to permute the (r,s,t) derivatives according to the value of axis.
!      axis=0: (r,s,t)
!      axis=1: (s,t,r)
!      axis=2: (t,r,s)
!
!  FORCING: none, twilightZone
!=============================================================================================

! ***** Step 1 : assign values using extrapolation of the normal component ***
#beginMacro bcCurvilinear3dOrder4Step1(FORCING)
 ! Since is1 is +1 or -1 we need to flip the sign of dr in the derivative approximations
 dra = dr(axis  )*(1-2*side)
 dsa = dr(axisp1)*(1-2*side)
 dta = dr(axisp2)*(1-2*side)

 drb = dr(axis  )
 dsb = dr(axisp1)
 dtb = dr(axisp2)

 ! ** Fourth-order for tau.Delta\uv=0, setting  ctlrr=ctlr=0 in the code will revert to 2nd-order
 ctlrr=1.
 ctlr=1.

 if( debug.gt.0 )then
   write(*,'(" **bcCurvilinear3dOrder4Step1: START: grid,side,axis=",3i2," is1,is2,is3=",3i3," ks1,ks2,ks3=",3i3)')\
        grid,side,axis,is1,is2,is3,ks1,ks2,ks3
 end if

 beginLoops()
 if( mask(i1,i2,i3).gt.0 )then

 defineMetricDerivativesExtrap()
 getTangentialDerivativesExtrap()

 tau11=rsxy(i1,i2,i3,axisp1,0)
 tau12=rsxy(i1,i2,i3,axisp1,1)
 tau13=rsxy(i1,i2,i3,axisp1,2)

 tau21=rsxy(i1,i2,i3,axisp2,0)
 tau22=rsxy(i1,i2,i3,axisp2,1)
 tau23=rsxy(i1,i2,i3,axisp2,2)

 uex=u(i1,i2,i3,ex)
 uey=u(i1,i2,i3,ey)
 uez=u(i1,i2,i3,ez)


! ************ Answer *******************


 Da1DotU=0.
 tau1DotUtt=0.
 tau2DotUtt=0.

 gIVf1=0.
 gIVf2=0.

 if( forcingOption.eq.planeWaveBoundaryForcing )then
   ! In the plane wave forcing case we subtract out a plane wave incident field
   ! This causes the BC to be 
   !           tau.u = - tau.uI
   !   and     tau.utt = -tau.uI.tt

   a21s = DS4($A21D3J)
   a22s = DS4($A22D3J)
   a23s = DS4($A23D3J)
  
   a31t = DT4($A31D3J)
   a32t = DT4($A32D3J)
   a33t = DT4($A33D3J)

   ! *** set RHS for (a1.u).r =  - Ds( a2.uv ) -Dt( a3.uv )
   Da1DotU = -(  a21s*uex+a22s*uey+a23s*uez + a21*us+a22*vs+a23*ws \
               + a31t*uex+a32t*uey+a33t*uez + a31*ut+a32*vt+a33*wt )

   getMinusPlaneWave3Dtt(i1,i2,i3,t,udd,vdd,wdd)

   tau1DotUtt = tau11*udd+tau12*vdd+tau13*wdd
   tau2DotUtt = tau21*udd+tau22*vdd+tau23*wdd

 end if

 #If #FORCING == "twilightZone"
 if( useForcing.ne.0 )then

   ! For TZ: utt0 = utt - ett + Lap(e)
  OGDERIV3D(0, 2,0,0, i1,i2,i3, t,uxx,vxx,wxx)
  OGDERIV3D(0, 0,2,0, i1,i2,i3, t,uyy,vyy,wyy)
  OGDERIV3D(0, 0,0,2, i1,i2,i3, t,uzz,vzz,wzz)

  utt00=uxx+uyy+uzz
  vtt00=vxx+vyy+vzz
  wtt00=wxx+wyy+wzz

  tau1DotUtt = tau11*utt00+tau12*vtt00+tau13*wtt00
  tau2DotUtt = tau21*utt00+tau22*vtt00+tau23*wtt00


  ! OGF3D(i1,i2,i3,t, uv0(0),uv0(1),uv0(2))
  ! OGF3D(i1-is1,i2-is2,i3-is3,t, uvm(0),uvm(1),uvm(2))
  ! OGF3D(i1+is1,i2+is2,i3+is3,t, uvp(0),uvp(1),uvp(2))
  ! OGF3D(i1-2*is1,i2-2*is2,i3-2*is3,t, uvm2(0),uvm2(1),uvm2(2))
  ! OGF3D(i1+2*is1,i2+2*is2,i3+2*is3,t, uvp2(0),uvp2(1),uvp2(2))

  ! Da1DotU = (a1.uv).r to 4th order
  ! Da1DotU = (8.*( (a11p1*uvp(0) +a12p1*uvp(1))  - (a11m1*uvm(0) +a12m1*uvm(1)) )\
  !             - ( (a11p2*uvp2(0)+a12p2*uvp2(1)) - (a11m2*uvm2(0)+a12m2*uvm2(1)) ) )/(12.*dra)

 ! for now remove the error in the extrapolation ************
 ! gIVf1 = tau11*( uvm2(0)-4.*uvm(0)+6.*uv0(0)-4.*uvp(0)+uvp2(0)) +\
 !         tau12*( uvm2(1)-4.*uvm(1)+6.*uv0(1)-4.*uvp(1)+uvp2(1)) +\
 !         tau13*( uvm2(2)-4.*uvm(2)+6.*uv0(2)-4.*uvp(2)+uvp2(2))
 ! gIVf2 = tau21*( uvm2(0)-4.*uvm(0)+6.*uv0(0)-4.*uvp(0)+uvp2(0)) +\
 !         tau22*( uvm2(1)-4.*uvm(1)+6.*uv0(1)-4.*uvp(1)+uvp2(1)) +\
 !         tau23*( uvm2(2)-4.*uvm(2)+6.*uv0(2)-4.*uvp(2)+uvp2(2))

 ! gIVf1=0.  ! RHS for tau.D+^p(u)=0
 ! gIVf2=0.


  ! **** compute RHS for div(u) equation ****
  a21zp1= A21D3J(i1+js1,i2+js2,i3+js3) 
  a21zm1= A21D3J(i1-js1,i2-js2,i3-js3) 
  a21zp2= A21D3J(i1+2*js1,i2+2*js2,i3+2*js3) 
  a21zm2= A21D3J(i1-2*js1,i2-2*js2,i3-2*js3) 
 
  a22zp1= A22D3J(i1+js1,i2+js2,i3+js3) 
  a22zm1= A22D3J(i1-js1,i2-js2,i3-js3) 
  a22zp2= A22D3J(i1+2*js1,i2+2*js2,i3+2*js3) 
  a22zm2= A22D3J(i1-2*js1,i2-2*js2,i3-2*js3) 

  a23zp1= A23D3J(i1+js1,i2+js2,i3+js3) 
  a23zm1= A23D3J(i1-js1,i2-js2,i3-js3) 
  a23zp2= A23D3J(i1+2*js1,i2+2*js2,i3+2*js3) 
  a23zm2= A23D3J(i1-2*js1,i2-2*js2,i3-2*js3) 

  a31zp1= A31D3J(i1+ks1,i2+ks2,i3+ks3) 
  a31zm1= A31D3J(i1-ks1,i2-ks2,i3-ks3) 
  a31zp2= A31D3J(i1+2*ks1,i2+2*ks2,i3+2*ks3) 
  a31zm2= A31D3J(i1-2*ks1,i2-2*ks2,i3-2*ks3) 
 
  a32zp1= A32D3J(i1+ks1,i2+ks2,i3+ks3) 
  a32zm1= A32D3J(i1-ks1,i2-ks2,i3-ks3) 
  a32zp2= A32D3J(i1+2*ks1,i2+2*ks2,i3+2*ks3) 
  a32zm2= A32D3J(i1-2*ks1,i2-2*ks2,i3-2*ks3) 

  a33zp1= A33D3J(i1+ks1,i2+ks2,i3+ks3) 
  a33zm1= A33D3J(i1-ks1,i2-ks2,i3-ks3) 
  a33zp2= A33D3J(i1+2*ks1,i2+2*ks2,i3+2*ks3) 
  a33zm2= A33D3J(i1-2*ks1,i2-2*ks2,i3-2*ks3) 

  ! *** set to - Ds( a2.uv ) -Dt( a3.uv )
  Da1DotU = -(  \
       ( 8.*(a21zp1*u(i1+  js1,i2+  js2,i3+  js3,ex)-a21zm1*u(i1-  js1,i2-  js2,i3-  js3,ex)) \
           -(a21zp2*u(i1+2*js1,i2+2*js2,i3+2*js3,ex)-a21zm2*u(i1-2*js1,i2-2*js2,i3-2*js3,ex)) )/(12.*dsa) \
      +( 8.*(a22zp1*u(i1+  js1,i2+  js2,i3+  js3,ey)-a22zm1*u(i1-  js1,i2-  js2,i3-  js3,ey)) \
           -(a22zp2*u(i1+2*js1,i2+2*js2,i3+2*js3,ey)-a22zm2*u(i1-2*js1,i2-2*js2,i3-2*js3,ey)) )/(12.*dsa) \
      +( 8.*(a23zp1*u(i1+  js1,i2+  js2,i3+  js3,ez)-a23zm1*u(i1-  js1,i2-  js2,i3-  js3,ez)) \
           -(a23zp2*u(i1+2*js1,i2+2*js2,i3+2*js3,ez)-a23zm2*u(i1-2*js1,i2-2*js2,i3-2*js3,ez)) )/(12.*dsa)  ) \
             -(  \
       ( 8.*(a31zp1*u(i1+  ks1,i2+  ks2,i3+  ks3,ex)-a31zm1*u(i1-  ks1,i2-  ks2,i3-  ks3,ex)) \
           -(a31zp2*u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ex)-a31zm2*u(i1-2*ks1,i2-2*ks2,i3-2*ks3,ex)) )/(12.*dta) \
      +( 8.*(a32zp1*u(i1+  ks1,i2+  ks2,i3+  ks3,ey)-a32zm1*u(i1-  ks1,i2-  ks2,i3-  ks3,ey)) \
           -(a32zp2*u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ey)-a32zm2*u(i1-2*ks1,i2-2*ks2,i3-2*ks3,ey)) )/(12.*dta) \
      +( 8.*(a33zp1*u(i1+  ks1,i2+  ks2,i3+  ks3,ez)-a33zm1*u(i1-  ks1,i2-  ks2,i3-  ks3,ez)) \
           -(a33zp2*u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ez)-a33zm2*u(i1-2*ks1,i2-2*ks2,i3-2*ks3,ez)) )/(12.*dta)  )

 end if
 #End


! Now assign E at the ghost points:
#Include "bc4Maxwell3dExtrap.h"

!  if( debug.gt.0 )then
!
!   write(*,'(" bc4:extrap: i1,i2,i3=",3i3," u(-1)=",3f8.2," u(-2)=",3f8.2)') i1,i2,i3,\
!          u(i1-is1,i2-is2,i3-is3,ex),u(i1-is1,i2-is2,i3-is3,ey),u(i1-is1,i2-is2,i3-is3,ez),\
!          u(i1-2*is1,i2-2*is2,i3-2*is3,ex),u(i1-2*is1,i2-2*is2,i3-2*is3,ey),u(i1-2*is1,i2-2*is2,i3-2*is3,ez)
!  end if

  ! set to exact for testing
  ! OGF3D(i1-is1,i2-is2,i3-is3,t, uvm(0),uvm(1),uvm(2))
  ! OGF3D(i1-2*is1,i2-2*is2,i3-2*is3,t, uvm2(0),uvm2(1),uvm2(2))
  ! u(i1-is1,i2-is2,i3-is3,ex)=uvm(0)
  ! u(i1-is1,i2-is2,i3-is3,ey)=uvm(1)
  ! u(i1-is1,i2-is2,i3-is3,ez)=uvm(2)

  ! u(i1-2*is1,i2-2*is2,i3-2*is3,ex)=uvm2(0)
  ! u(i1-2*is1,i2-2*is2,i3-2*is3,ey)=uvm2(1)
  ! u(i1-2*is1,i2-2*is2,i3-2*is3,ez)=uvm2(2)


else if( mask(i1,i2,i3).lt.0 )then

 ! ** NEW WAY **  *wdh
 ! extrapolate ghost points next to boundary interpolation points  *wdh* 2015/08/11
 if( t.le.dt )then
   write(*,'("--MX-- BC4 extrap ghost next to interp")')
 end if

  u(i1-is1,i2-is2,i3-is3,ex) = extrap5(ex,i1,i2,i3,is1,is2,is3)
  u(i1-is1,i2-is2,i3-is3,ey) = extrap5(ey,i1,i2,i3,is1,is2,is3)
  u(i1-is1,i2-is2,i3-is3,ez) = extrap5(ez,i1,i2,i3,is1,is2,is3)
  u(i1-2*is1,i2-2*is2,i3-2*is3,ex) = extrap5(ex,i1-is1,i2-is2,i3-is3,is1,is2,is3)
  u(i1-2*is1,i2-2*is2,i3-2*is3,ey) = extrap5(ey,i1-is1,i2-is2,i3-is3,is1,is2,is3)
  u(i1-2*is1,i2-2*is2,i3-2*is3,ez) = extrap5(ez,i1-is1,i2-is2,i3-is3,is1,is2,is3)


else if( .FALSE. .and. mask(i1,i2,i3).lt.0 )then

  ! **OLD WAY**

 ! QUESTION: August 8, 2015 -- is this accurate enough ??
 

 ! we need to assign ghost points that lie outside of interpolation points


 if( debug.gt.0 )then
   write(*,'(" **ghost-interp3d: grid,side,axis=",3i2,", i1,i2,i3=",3i4)') grid,side,axis,i1,i2,i3
 end if

 tau11=rsxy(i1,i2,i3,axisp1,0)
 tau12=rsxy(i1,i2,i3,axisp1,1)
 tau13=rsxy(i1,i2,i3,axisp1,2)

 tau21=rsxy(i1,i2,i3,axisp2,0)
 tau22=rsxy(i1,i2,i3,axisp2,1)
 tau23=rsxy(i1,i2,i3,axisp2,2)

 uex=u(i1,i2,i3,ex)
 uey=u(i1,i2,i3,ey)
 uez=u(i1,i2,i3,ez)


 a11 =A11D3(i1,i2,i3)
 a12 =A12D3(i1,i2,i3)
 a13 =A13D3(i1,i2,i3)

 a21 =A21D3(i1,i2,i3)
 a22 =A22D3(i1,i2,i3)
 a23 =A23D3(i1,i2,i3)

 a31 =A31D3(i1,i2,i3)
 a32 =A32D3(i1,i2,i3)
 a33 =A33D3(i1,i2,i3)

 a11m1 =A11D3(i1-is1,i2-is2,i3-is3)
 a12m1 =A12D3(i1-is1,i2-is2,i3-is3)
 a13m1 =A13D3(i1-is1,i2-is2,i3-is3)

 a11p1 =A11D3(i1+is1,i2+is2,i3+is3)
 a12p1 =A12D3(i1+is1,i2+is2,i3+is3)
 a13p1 =A13D3(i1+is1,i2+is2,i3+is3)

 a11m2 =A11D3(i1-2*is1,i2-2*is2,i3-2*is3)
 a12m2 =A12D3(i1-2*is1,i2-2*is2,i3-2*is3)
 a13m2 =A13D3(i1-2*is1,i2-2*is2,i3-2*is3)

 a11p2 =A11D3(i1+2*is1,i2+2*is2,i3+2*is3)
 a12p2 =A12D3(i1+2*is1,i2+2*is2,i3+2*is3)
 a13p2 =A13D3(i1+2*is1,i2+2*is2,i3+2*is3)

 c11 = C11D3(i1,i2,i3)
 c22 = C22D3(i1,i2,i3)
 c33 = C33D3(i1,i2,i3)


 ! ***************************************************************************************
 ! Use one sided approximations as needed for expressions needing tangential derivatives
 ! ***************************************************************************************

 js1a=abs(js1)
 js2a=abs(js2)
 js3a=abs(js3)

 ks1a=abs(ks1)
 ks2a=abs(ks2)
 ks3a=abs(ks3)

 ! *** first do metric derivatives -- no need to worry about the mask value ****
 if( (i1-2*js1a).ge.md1a .and. (i1+2*js1a).le.md1b .and. \
     (i2-2*js2a).ge.md2a .and. (i2+2*js2a).le.md2b .and. \
     (i3-2*js3a).ge.md3a .and. (i3+2*js3a).le.md3b .and. \
     (i1-2*ks1a).ge.md1a .and. (i1+2*ks1a).le.md1b .and. \
     (i2-2*ks2a).ge.md2a .and. (i2+2*ks2a).le.md2b .and. \
     (i3-2*ks3a).ge.md3a .and. (i3+2*ks3a).le.md3b )then
   ! centered approximation is ok
   c1 = C1D3Order4(i1,i2,i3)
   c2 = C2D3Order4(i1,i2,i3)
   c3 = C3D3Order4(i1,i2,i3)

 else if( (i1-js1a).ge.md1a .and. (i1+js1a).le.md1b .and. \
          (i2-js2a).ge.md2a .and. (i2+js2a).le.md2b .and. \
          (i3-js3a).ge.md3a .and. (i3+js3a).le.md3b .and. \
          (i1-ks1a).ge.md1a .and. (i1+ks1a).le.md1b .and. \
          (i2-ks2a).ge.md2a .and. (i2+ks2a).le.md2b .and. \
          (i3-ks3a).ge.md3a .and. (i3+ks3a).le.md3b )then
   ! use 2nd-order centered approximation
   c1 = C1D3Order2(i1,i2,i3)
   c2 = C2D3Order2(i1,i2,i3)
   c3 = C3D3Order2(i1,i2,i3)

 else if( (i1-3*js1a).ge.md1a .and. \
          (i2-3*js2a).ge.md2a .and. \
          (i3-3*js3a).ge.md3a )then
  ! one sided  2nd-order:
  c1 = 2.*C1D3Order2(i1-js1a,i2-js2a,i3-js3a)-C1D3Order2(i1-2*js1a,i2-2*js2a,i3-2*js3a)
  c2 = 2.*C2D3Order2(i1-js1a,i2-js2a,i3-js3a)-C2D3Order2(i1-2*js1a,i2-2*js2a,i3-2*js3a)
  c3 = 2.*C3D3Order2(i1-js1a,i2-js2a,i3-js3a)-C3D3Order2(i1-2*js1a,i2-2*js2a,i3-2*js3a)
 else if( (i1+3*js1a).le.md1b .and. \
          (i2+3*js2a).le.md2b .and. \
          (i3+3*js3a).le.md3b )then
  ! one sided  2nd-order:
  c1 = 2.*C1D3Order2(i1+js1a,i2+js2a,i3+js3a)-C1D3Order2(i1+2*js1a,i2+2*js2a,i3+2*js3a)
  c2 = 2.*C2D3Order2(i1+js1a,i2+js2a,i3+js3a)-C2D3Order2(i1+2*js1a,i2+2*js2a,i3+2*js3a)
  c3 = 2.*C3D3Order2(i1+js1a,i2+js2a,i3+js3a)-C3D3Order2(i1+2*js1a,i2+2*js2a,i3+2*js3a)
 else if( (i1-3*ks1a).ge.md1a .and. \
          (i2-3*ks2a).ge.md2a .and. \
          (i3-3*ks3a).ge.md3a )then
  ! one sided  2nd-order:  -- this case should not be needed?
  c1 = 2.*C1D3Order2(i1-ks1a,i2-ks2a,i3-ks3a)-C1D3Order2(i1-2*ks1a,i2-2*ks2a,i3-2*ks3a)
  c2 = 2.*C2D3Order2(i1-ks1a,i2-ks2a,i3-ks3a)-C2D3Order2(i1-2*ks1a,i2-2*ks2a,i3-2*ks3a)
  c3 = 2.*C3D3Order2(i1-ks1a,i2-ks2a,i3-ks3a)-C3D3Order2(i1-2*ks1a,i2-2*ks2a,i3-2*ks3a)
 else if( (i1+3*ks1a).le.md1b .and. \
          (i2+3*ks2a).le.md2b .and. \
          (i3+3*ks3a).le.md3b )then
  ! one sided  2nd-order: -- this case should not be needed?
  c1 = 2.*C1D3Order2(i1+ks1a,i2+ks2a,i3+ks3a)-C1D3Order2(i1+2*ks1a,i2+2*ks2a,i3+2*ks3a)
  c2 = 2.*C2D3Order2(i1+ks1a,i2+ks2a,i3+ks3a)-C2D3Order2(i1+2*ks1a,i2+2*ks2a,i3+2*ks3a)
  c3 = 2.*C3D3Order2(i1+ks1a,i2+ks2a,i3+ks3a)-C3D3Order2(i1+2*ks1a,i2+2*ks2a,i3+2*ks3a)
 else
  ! this case should not happen
  stop 40066
 end if


 ! ***** Now do "s"-derivatives *****
 ! warning -- the compiler could still try to evaluate the mask at an invalid point
 if( (i1-js1a).ge.md1a .and. (i2-js2a).ge.md2a .and. (i3-js3a).ge.md3a .and. mask(i1-js1a,i2-js2a,i3-js3a).ne.0 .and. \
     (i1+js1a).le.md1b .and. (i2+js2a).le.md2b .and. (i3+js3a).le.md3b .and. mask(i1+js1a,i2+js2a,i3+js3a).ne.0 )then
   us=US2(ex)
   vs=US2(ey)
   ws=US2(ez)

   uss=USS2(ex)
   vss=USS2(ey)
   wss=USS2(ez)
  if( debug.gt.0 )then
   OGF3D(i1-js1a,i2-js2a,i3-js3a,t, uvm(0),uvm(1),uvm(2))
   OGF3D(i1+js1a,i2+js2a,i3+js3a,t, uvp(0),uvp(1),uvp(2))
   write(*,'(" **ghost-interp3d: use central-diff: us,uss=",2f8.3," us2,usm,usp=",3f8.3)') us,uss,\
             (uvp(0)-uvm(0))/(2.*dsb),USM(i1,i2,i3,js1a,js2a,js3a,dsb,ex),USP(i1,i2,i3,js1a,js2a,js3a,dsb,ex)
  end if

 else if( (i1-2*js1a).ge.md1a .and. \
          (i2-2*js2a).ge.md2a .and. \
          (i3-2*js3a).ge.md3a .and. \
           mask(i1-js1a,i2-js2a,i3-js3a).ne.0 .and. mask(i1-2*js1a,i2-2*js2a,i3-js3a).ne.0 )then
   

  ! 2nd-order one-sided: ** note ** use ds not dsa
  us = USM(i1,i2,i3,js1a,js2a,js3a,dsb,ex)
  vs = USM(i1,i2,i3,js1a,js2a,js3a,dsb,ey)
  ws = USM(i1,i2,i3,js1a,js2a,js3a,dsb,ez)

  uss = USSM(i1,i2,i3,js1a,js2a,js3a,dsb,ex)
  vss = USSM(i1,i2,i3,js1a,js2a,js3a,dsb,ey)
  wss = USSM(i1,i2,i3,js1a,js2a,js3a,dsb,ez)

  if( debug.gt.0 )then
   write(*,'(" **ghost-interp3d: use left-difference: us,uss=",2e10.2," us1=",e10.2," js1,js2=",2i2)') us,uss,\
             (u(i1,i2,i3,ex)-u(i1-js1a,i2-js2a,i3-js3a,ex))/dsb,js1,js2
  end if
 else if( (i1+2*js1a).le.md1b .and. \
          (i2+2*js2a).le.md2b .and.  \
          (i3+2*js3a).le.md3b .and.  \
          mask(i1+js1a,i2+js2a,i3+js3a).ne.0 .and. mask(i1+2*js1a,i2+2*js2a,i3+2*js3a).ne.0 )then

  ! 2nd-order one-sided:
  us = USP(i1,i2,i3,js1a,js2a,js3a,dsb,ex)
  vs = USP(i1,i2,i3,js1a,js2a,js3a,dsb,ey)
  ws = USP(i1,i2,i3,js1a,js2a,js3a,dsb,ez)

  uss = USSP(i1,i2,i3,js1a,js2a,js3a,dsb,ex)
  vss = USSP(i1,i2,i3,js1a,js2a,js3a,dsb,ey)
  wss = USSP(i1,i2,i3,js1a,js2a,js3a,dsb,ez)

  if( debug.gt.0 )then
   write(*,'(" **ghost-interp3d: use right-difference: us,uss=",2e10.2," us1=",e10.2," js1,js2=",2i2)') us,uss,\
             (u(i1+js1a,i2+js2a,i3+js3a,ex)-u(i1,i2,i3,ex))/dsb,js1,js2
  end if

 else 
   ! this case shouldn't matter
   us=0.
   vs=0.
   ws=0.
   uss=0.
   vss=0.
   wss=0.
 end if

 ! **** t - derivatives ****
 if( (i1-ks1a).ge.md1a .and. (i2-ks2a).ge.md2a .and. (i3-ks3a).ge.md3a .and. mask(i1-ks1a,i2-ks2a,i3-ks3a).ne.0 .and. \
     (i1+ks1a).le.md1b .and. (i2+ks2a).le.md2b .and. (i3+ks3a).le.md3b .and. mask(i1+ks1a,i2+ks2a,i3+ks3a).ne.0 )then
   ut=UT2(ex)
   vt=UT2(ey)
   wt=UT2(ez)

   utt=UTT2(ex)
   vtt=UTT2(ey)
   wtt=UTT2(ez)

  if( debug.gt.0 )then
   OGF3D(i1-ks1a,i2-ks2a,i3-ks3a,t, uvm(0),uvm(1),uvm(2))
   OGF3D(i1+ks1a,i2+ks2a,i3+ks3a,t, uvp(0),uvp(1),uvp(2))
   write(*,'(" **ghost-interp3d: use central-diff: ut,utt=",2f8.3," ut2=",f8.3)') ut,utt,\
             (uvp(0)-uvm(0))/(2.*dtb)
  end if

 else if( (i1-2*ks1a).ge.md1a .and. \
          (i2-2*ks2a).ge.md2a .and. \
          (i3-2*ks3a).ge.md3a .and. \
           mask(i1-ks1a,i2-ks2a,i3-ks3a).ne.0 .and. mask(i1-2*ks1a,i2-2*ks2a,i3-ks3a).ne.0 )then
   

  ! 2nd-order one-sided:
  ut = USM(i1,i2,i3,ks1a,ks2a,ks3a,dtb,ex)
  vt = USM(i1,i2,i3,ks1a,ks2a,ks3a,dtb,ey)
  wt = USM(i1,i2,i3,ks1a,ks2a,ks3a,dtb,ez)

  utt = USSM(i1,i2,i3,ks1a,ks2a,ks3a,dtb,ex)
  vtt = USSM(i1,i2,i3,ks1a,ks2a,ks3a,dtb,ey)
  wtt = USSM(i1,i2,i3,ks1a,ks2a,ks3a,dtb,ez)

  if( debug.gt.0 )then
   write(*,'(" **ghost-interp3d: use left-difference: ut,utt=",2e10.2," ut1=",e10.2," kt1,kt2=",2i2)') ut,utt,\
             (u(i1,i2,i3,ex)-u(i1-ks1a,i2-ks2a,i3-ks3a,ex))/dtb,ks1,ks2
  end if

 else if( (i1+2*ks1a).le.md1b .and. \
          (i2+2*ks2a).le.md2b .and.  \
          (i3+2*ks3a).le.md3b .and.  \
          mask(i1+ks1a,i2+ks2a,i3+ks3a).ne.0 .and. mask(i1+2*ks1a,i2+2*ks2a,i3+2*ks3a).ne.0 )then

  ! 2nd-order one-sided:
  ut = USP(i1,i2,i3,ks1a,ks2a,ks3a,dtb,ex)
  vt = USP(i1,i2,i3,ks1a,ks2a,ks3a,dtb,ey)
  wt = USP(i1,i2,i3,ks1a,ks2a,ks3a,dtb,ez)

  utt = USSP(i1,i2,i3,ks1a,ks2a,ks3a,dtb,ex)
  vtt = USSP(i1,i2,i3,ks1a,ks2a,ks3a,dtb,ey)
  wtt = USSP(i1,i2,i3,ks1a,ks2a,ks3a,dtb,ez)

  if( debug.gt.0 )then
   OGF3D(i1,i2,i3,t, uv0(0),uv0(1),uv0(2))
   OGF3D(i1+ks1a,i2+ks2a,i3+ks3a,t, uvp(0),uvp(1),uvp(2))
   OGF3D(i1+2*ks1a,i2+2*ks2a,i3+2*ks3a,t, uvp2(0),uvp2(1),uvp2(2))
   write(*,'(" **ghost-interp3d: use right-diff: ut,utt=",2f8.3," ut1,ut2=",2f8.3," dta,dtb=",2f7.4)') ut,utt,\
             (u(i1+ks1a,i2+ks2a,i3+ks3a,ex)-u(i1,i2,i3,ex))/dtb,\
             (4.*uvp(0)-3.*uv0(0)-uvp2(0))/(2.*dtb),dta,dtb
  end if
 ! write(*,'(" **ghost-interp: use right-difference: ut,utt=",2e10.2)') ut,utt

 else 
   ! this case shouldn't matter
   ut=0.
   vt=0.
   wt=0.
   utt=0.
   vtt=0.
   wtt=0.
 end if



 Da1DotU=0.
 tau1DotUtt=0.
 tau2DotUtt=0.

 gIVf1=0.
 gIVf2=0.

 ! Compute a21s, a31t, ... for RHS to div equation
 if( forcingOption.ne.0 )then
  if( (i1-js1a).ge.md1a .and. (i1+js1a).le.md1b .and. \
      (i2-js2a).ge.md2a .and. (i2+js2a).le.md2b .and. \
      (i3-js3a).ge.md3a .and. (i3+js3a).le.md3b )then
   a21s = (A21D3(i1+js1,i2+js2,i3+js3)-A21D3(i1-js1,i2-js2,i3-js3))/(2.*dsa)
   a22s = (A22D3(i1+js1,i2+js2,i3+js3)-A22D3(i1-js1,i2-js2,i3-js3))/(2.*dsa)
   a23s = (A23D3(i1+js1,i2+js2,i3+js3)-A23D3(i1-js1,i2-js2,i3-js3))/(2.*dsa)

  else if( (i1+js1).ge.md1a .and. (i1+js1).le.md1b .and. \
           (i2+js2).ge.md2a .and. (i2+js2).le.md2b .and. \
           (i3+js3).ge.md3a .and. (i3+js3).le.md3b )then
   a21s = (A21D3(i1+js1,i2+js2,i3+js3)-A21D3(i1,i2,i3))/(dsa)
   a22s = (A22D3(i1+js1,i2+js2,i3+js3)-A22D3(i1,i2,i3))/(dsa)
   a23s = (A23D3(i1+js1,i2+js2,i3+js3)-A23D3(i1,i2,i3))/(dsa)

  else if( (i1-js1).ge.md1a .and. (i1-js1).le.md1b .and. \
           (i2-js2).ge.md2a .and. (i2-js2).le.md2b .and. \
           (i3-js3).ge.md3a .and. (i3-js3).le.md3b )then
   a21s = (A21D3(i1,i2,i3)-A21D3(i1-js1,i2-js2,i3-js3))/(dsa)
   a22s = (A22D3(i1,i2,i3)-A22D3(i1-js1,i2-js2,i3-js3))/(dsa)
   a23s = (A23D3(i1,i2,i3)-A23D3(i1-js1,i2-js2,i3-js3))/(dsa)

  else
    stop 82750
  end if

  if( (i1-ks1a).ge.md1a .and. (i1+ks1a).le.md1b .and. \
      (i2-ks2a).ge.md2a .and. (i2+ks2a).le.md2b .and. \
      (i3-ks3a).ge.md3a .and. (i3+ks3a).le.md3b )then
   a31t = (A31D3(i1+ks1,i2+ks2,i3+ks3)-A31D3(i1-ks1,i2-ks2,i3-ks3))/(2.*dta)
   a32t = (A32D3(i1+ks1,i2+ks2,i3+ks3)-A32D3(i1-ks1,i2-ks2,i3-ks3))/(2.*dta)
   a33t = (A33D3(i1+ks1,i2+ks2,i3+ks3)-A33D3(i1-ks1,i2-ks2,i3-ks3))/(2.*dta)

  else if( (i1+ks1).ge.md1a .and. (i1+ks1).le.md1b .and. \
           (i2+ks2).ge.md2a .and. (i2+ks2).le.md2b .and. \
           (i3+ks3).ge.md3a .and. (i3+ks3).le.md3b )then
   a31t = (A31D3(i1+ks1,i2+ks2,i3+ks3)-A31D3(i1,i2,i3))/(dta)
   a32t = (A32D3(i1+ks1,i2+ks2,i3+ks3)-A32D3(i1,i2,i3))/(dta)
   a33t = (A33D3(i1+ks1,i2+ks2,i3+ks3)-A33D3(i1,i2,i3))/(dta)

  else if( (i1-ks1).ge.md1a .and. (i1-ks1).le.md1b .and. \
           (i2-ks2).ge.md2a .and. (i2-ks2).le.md2b .and. \
           (i3-ks3).ge.md3a .and. (i3-ks3).le.md3b )then
   a31t = (A31D3(i1,i2,i3)-A31D3(i1-ks1,i2-ks2,i3-ks3))/(dta)
   a32t = (A32D3(i1,i2,i3)-A32D3(i1-ks1,i2-ks2,i3-ks3))/(dta)
   a33t = (A33D3(i1,i2,i3)-A33D3(i1-ks1,i2-ks2,i3-ks3))/(dta)

  else
    stop 8250
  end if
 end if

 if( forcingOption.eq.planeWaveBoundaryForcing )then
   ! In the plane wave forcing case we subtract out a plane wave incident field
   !   --->    tau.utt = -tau.uI.tt

   ! *** set RHS for (a1.u).r =  - Ds( a2.uv ) -Dt( a3.uv )
   Da1DotU = -(  a21s*uex+a22s*uey+a23s*uez + a21*us+a22*vs+a23*ws \
               + a31t*uex+a32t*uey+a33t*uez + a31*ut+a32*vt+a33*wt )

   getMinusPlaneWave3Dtt(i1,i2,i3,t,udd,vdd,wdd)

   tau1DotUtt = tau11*udd+tau12*vdd+tau13*wdd
   tau2DotUtt = tau21*udd+tau22*vdd+tau23*wdd

 end if


 #If #FORCING == "twilightZone"
 if( useForcing.ne.0 )then

   ! For TZ: utt0 = utt - ett + Lap(e)
  OGDERIV3D(0, 2,0,0, i1,i2,i3, t,uxx,vxx,wxx)
  OGDERIV3D(0, 0,2,0, i1,i2,i3, t,uyy,vyy,wyy)
  OGDERIV3D(0, 0,0,2, i1,i2,i3, t,uzz,vzz,wzz)

  utt00=uxx+uyy+uzz
  vtt00=vxx+vyy+vzz
  wtt00=wxx+wyy+wzz

  tau1DotUtt = tau11*utt00+tau12*vtt00+tau13*wtt00
  tau2DotUtt = tau21*utt00+tau22*vtt00+tau23*wtt00


  ! OGF3D(i1,i2,i3,t, uv0(0),uv0(1),uv0(2))
  ! OGF3D(i1-is1,i2-is2,i3-is3,t, uvm(0),uvm(1),uvm(2))
  ! OGF3D(i1+is1,i2+is2,i3+is3,t, uvp(0),uvp(1),uvp(2))
  ! OGF3D(i1-2*is1,i2-2*is2,i3-2*is3,t, uvm2(0),uvm2(1),uvm2(2))
  ! OGF3D(i1+2*is1,i2+2*is2,i3+2*is3,t, uvp2(0),uvp2(1),uvp2(2))

  ! Da1DotU = (a1.uv).r to 4th order
  ! Da1DotU = (8.*( (a11p1*uvp(0) +a12p1*uvp(1))  - (a11m1*uvm(0) +a12m1*uvm(1)) )\
  !             - ( (a11p2*uvp2(0)+a12p2*uvp2(1)) - (a11m2*uvm2(0)+a12m2*uvm2(1)) ) )/(12.*dra)

 ! for now remove the error in the extrapolation ************
 ! gIVf1 = tau11*( uvm2(0)-4.*uvm(0)+6.*uv0(0)-4.*uvp(0)+uvp2(0)) +\
 !         tau12*( uvm2(1)-4.*uvm(1)+6.*uv0(1)-4.*uvp(1)+uvp2(1)) +\
 !         tau13*( uvm2(2)-4.*uvm(2)+6.*uv0(2)-4.*uvp(2)+uvp2(2))
 ! gIVf2 = tau21*( uvm2(0)-4.*uvm(0)+6.*uv0(0)-4.*uvp(0)+uvp2(0)) +\
 !         tau22*( uvm2(1)-4.*uvm(1)+6.*uv0(1)-4.*uvp(1)+uvp2(1)) +\
 !         tau23*( uvm2(2)-4.*uvm(2)+6.*uv0(2)-4.*uvp(2)+uvp2(2))

 ! gIVf1=0.  ! RHS for tau.D+^p(u)=0
 ! gIVf2=0.


  ! **** compute RHS for div(u) equation ****

  ! *** set to - Ds( a2.uv ) -Dt( a3.uv )
  ! do this: Da1DotU = -( a2.us + a2s.u + a3.ut + a3t.u )
  Da1DotU = -( a21*us+a22*vs+a23*ws + a21s*uex + a22s*uey + a23s*uez \
              +a31*ut+a32*vt+a33*wt + a31t*uex + a32t*uey + a33t*uez )
!  Da1DotU = -(  \
!       (a21zp1*u(i1+js1,i2+js2,i3+js3,ex)-a21zm1*u(i1-js1,i2-js2,i3-js3,ex))/(2.*dsa)\
!      +(a22zp1*u(i1+js1,i2+js2,i3+js3,ey)-a22zm1*u(i1-js1,i2-js2,i3-js3,ey))/(2.*dsa) \
!      +(a23zp1*u(i1+js1,i2+js2,i3+js3,ez)-a23zm1*u(i1-js1,i2-js2,i3-js3,ez))/(2.*dsa) ) \
!             -(  \
!       (a31zp1*u(i1+ks1,i2+ks2,i3+ks3,ex)-a31zm1*u(i1-ks1,i2-ks2,i3-ks3,ex))/(2.*dta) \
!      +(a32zp1*u(i1+ks1,i2+ks2,i3+ks3,ey)-a32zm1*u(i1-ks1,i2-ks2,i3-ks3,ey))/(2.*dta) \
!      +(a33zp1*u(i1+ks1,i2+ks2,i3+ks3,ez)-a33zm1*u(i1-ks1,i2-ks2,i3-ks3,ez))/(2.*dta) )


 end if
 #End


! Now assign E at the ghost points:
#Include "bc4Maxwell3dExtrap.h"

  if( debug.gt.0 )then
   OGF3D(i1-is1,i2-is2,i3-is3,t, uvm(0),uvm(1),uvm(2))
   OGF3D(i1-2*is1,i2-2*is2,i3-2*is3,t, uvm2(0),uvm2(1),uvm2(2))
   write(*,'(" **ghost-interp3d: errors u(-1)=",3e10.2)') u(i1-is1,i2-is2,i3-is3,ex)-uvm(0),\
               u(i1-is1,i2-is2,i3-is3,ey)-uvm(1),u(i1-is1,i2-is2,i3-is3,ez)-uvm(2)
   write(*,'(" **ghost-interp3d: errors u(-2)=",3e10.2)') u(i1-2*is1,i2-2*is2,i3-2*is3,ex)-uvm2(0),\
               u(i1-2*is1,i2-2*is2,i3-2*is3,ey)-uvm2(1),u(i1-2*is1,i2-2*is2,i3-2*is3,ez)-uvm2(2)
  end if
  ! set to exact for testing
  ! OGF3D(i1-is1,i2-is2,i3-is3,t, uvm(0),uvm(1),uvm(2))
  ! OGF3D(i1-2*is1,i2-2*is2,i3-2*is3,t, uvm2(0),uvm2(1),uvm2(2))
  ! u(i1-is1,i2-is2,i3-is3,ex)=uvm(0)
  ! u(i1-is1,i2-is2,i3-is3,ey)=uvm(1)
  ! u(i1-is1,i2-is2,i3-is3,ez)=uvm(2)

  ! u(i1-2*is1,i2-2*is2,i3-2*is3,ex)=uvm2(0)
  ! u(i1-2*is1,i2-2*is2,i3-2*is3,ey)=uvm2(1)
  ! u(i1-2*is1,i2-2*is2,i3-2*is3,ez)=uvm2(2)


! &&&&&&&&&&&&&&&&&&&&&&&


! *   detnt=tau23*a11*tau12-tau23*a12*tau11-a13*tau21*tau12+tau21*tau13*a12+a13*tau22*tau11-tau22*tau13*a11
! *   do m=1,2
! *     m1=i1-m*is1
! *     m2=i2-m*is2
! *     m3=i3-m*is3
! *     ! use u.r=0 for now:
! *     !    tau.urr=0
! *     a1DotU= a11*u(i1+m*is1,i2+m*is2,i3+m*is3,ex)\
! *            +a12*u(i1+m*is1,i2+m*is2,i3+m*is3,ey)\
! *            +a13*u(i1+m*is1,i2+m*is2,i3+m*is3,ez)  
! *     tau1DotU=-( tau11*u(i1+m*is1,i2+m*is2,i3+m*is3,ex)\
! *                +tau12*u(i1+m*is1,i2+m*is2,i3+m*is3,ey)\
! *                +tau13*u(i1+m*is1,i2+m*is2,i3+m*is3,ez) )
! *     tau2DotU=-( tau21*u(i1+m*is1,i2+m*is2,i3+m*is3,ex)\
! *                +tau22*u(i1+m*is1,i2+m*is2,i3+m*is3,ey)\
! *                +tau23*u(i1+m*is1,i2+m*is2,i3+m*is3,ez) )
! *   
! *     u(m1,m2,m3,ex)=(tau23*a1DotU*tau12-a13*tau2DotU*tau12+a13*tau22*tau1DotU+tau2DotU*tau13*a12-tau22*tau13*a1DotU-tau23*a12*tau1DotU)/detnt
! *     u(m1,m2,m3,ey)=(-tau13*a11*tau2DotU+tau13*a1DotU*tau21+a11*tau23*tau1DotU+a13*tau11*tau2DotU-a1DotU*tau23*tau11-a13*tau1DotU*tau21)/detnt
! *     u(m1,m2,m3,ez)=(a11*tau2DotU*tau12-a11*tau22*tau1DotU-a12*tau11*tau2DotU+a12*tau1DotU*tau21-a1DotU*tau21*tau12+a1DotU*tau22*tau11)/detnt
! *   end do 

end if
endLoops()

#endMacro






! **************************************************************
! *****************   Correction Step **************************
! **************************************************************

#beginMacro bcCurvilinear3dOrder4(FORCING)
 ! Since is1 is +1 or -1 we need to flip the sign of dr in the derivative approximations
 dra = dr(axis  )*(1-2*side)
 dsa = dr(axisp1)*(1-2*side)
 dta = dr(axisp2)*(1-2*side)

 drb = dr(axis  )
 dsb = dr(axisp1)
 dtb = dr(axisp2)

 ! ** Fourth-order for tau.Delta\uv=0, setting  ctlrr=ctlr=0 in the code will revert to 2nd-order
 ctlrr=1.
 ctlr=1.

 if( debug.gt.0 )then
   write(*,'(" **bcCurvilinear3dOrder4: START: grid,side,axis=",3i2," is1,is2,is3=",3i3," ks1,ks2,ks3=",3i3)')\
        grid,side,axis,is1,is2,is3,ks1,ks2,ks3
 end if

! ******************************************
! ************Correction loop***************
! ******************************************

! Given an initial answer at all points we now go back and resolve for the normal component
! from   div(u)=0 and (a1.Delta u).r = 0 
! We use the initial guess in order to compute the mixed derivatives urs, urss, urtt

if( .true. )then

! ** Periodic update is now done in a previous step -- this doesn't work in parallel
! first do a periodic update
! if( .false. .and.(boundaryCondition(0,axisp1).lt.0 .or. boundaryCondition(0,axisp2).lt.0) )then
!   indexRange(0,0)=gridIndexRange(0,0)
!   indexRange(1,0)=gridIndexRange(1,0)
!   indexRange(0,1)=gridIndexRange(0,1)
!   indexRange(1,1)=gridIndexRange(1,1)
!   indexRange(0,2)=gridIndexRange(0,2)
!   indexRange(1,2)=gridIndexRange(1,2)
!
!   isPeriodic(0)=0
!   isPeriodic(1)=0
!   isPeriodic(2)=0
!   if( boundaryCondition(0,axisp1).lt.0 )then
!     indexRange(1,axisp1)=gridIndexRange(1,axisp1)-1
!     isPeriodic(axisp1)=1  
!   end if
!   if( boundaryCondition(0,axisp2).lt.0 )then
!     indexRange(1,axisp2)=gridIndexRange(1,axisp2)-1
!     isPeriodic(axisp2)=1  
!   end if
!
!  write(*,'(" *********** call periodic update grid,side,axis=",3i4)') grid,side,axis
!
!  call periodicUpdateMaxwell(nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
!     u,ex,ez, indexRange, gridIndexRange, dimension, isPeriodic )
! end if


 beginLoops()
 if( mask(i1,i2,i3).gt.0 )then

 defineMetricDerivativesExtrap()
 getTangentialDerivativesExtrap()

 tau11=rsxy(i1,i2,i3,axisp1,0)
 tau12=rsxy(i1,i2,i3,axisp1,1)
 tau13=rsxy(i1,i2,i3,axisp1,2)

 tau21=rsxy(i1,i2,i3,axisp2,0)
 tau22=rsxy(i1,i2,i3,axisp2,1)
 tau23=rsxy(i1,i2,i3,axisp2,2)

 uex=u(i1,i2,i3,ex)
 uey=u(i1,i2,i3,ey)
 uez=u(i1,i2,i3,ez)

 a11r = DR4($A11D3J)
 a12r = DR4($A12D3J)
 a13r = DR4($A13D3J)

 if( axis.eq.0 )then
   urs  =  urs4(i1,i2,i3,ex)
   urt  =  urt4(i1,i2,i3,ex)
   urss = urss4(i1,i2,i3,ex)
   urtt = urtt4(i1,i2,i3,ex)

   vrs  =  urs4(i1,i2,i3,ey)
   vrt  =  urt4(i1,i2,i3,ey)
   vrss = urss4(i1,i2,i3,ey)
   vrtt = urtt4(i1,i2,i3,ey)

   wrs  =  urs4(i1,i2,i3,ez)
   wrt  =  urt4(i1,i2,i3,ez)
   wrss = urss4(i1,i2,i3,ez)
   wrtt = urtt4(i1,i2,i3,ez)

   c11r = C11D3r4(i1,i2,i3)
   c22r = C22D3r4(i1,i2,i3)
   c33r = C33D3r4(i1,i2,i3)

   c1r = C1D3r4(i1,i2,i3)
   c2r = C2D3r4(i1,i2,i3)
   c3r = C3D3r4(i1,i2,i3)
 else if( axis.eq.1 )then
   urs  =  ust4(i1,i2,i3,ex)
   urt  =  urs4(i1,i2,i3,ex)
   urss = ustt4(i1,i2,i3,ex)
   urtt = urrs4(i1,i2,i3,ex)

   vrs  =  ust4(i1,i2,i3,ey)
   vrt  =  urs4(i1,i2,i3,ey)
   vrss = ustt4(i1,i2,i3,ey)
   vrtt = urrs4(i1,i2,i3,ey)

   wrs  =  ust4(i1,i2,i3,ez)
   wrt  =  urs4(i1,i2,i3,ez)
   wrss = ustt4(i1,i2,i3,ez)
   wrtt = urrs4(i1,i2,i3,ez)

   c11r = C11D3s4(i1,i2,i3)
   c22r = C22D3s4(i1,i2,i3)
   c33r = C33D3s4(i1,i2,i3)

   c1r = C1D3s4(i1,i2,i3)
   c2r = C2D3s4(i1,i2,i3)
   c3r = C3D3s4(i1,i2,i3)
 else 
   urs  =  urt4(i1,i2,i3,ex)
   urt  =  ust4(i1,i2,i3,ex)
   urss = urrt4(i1,i2,i3,ex)
   urtt = usst4(i1,i2,i3,ex)

   vrs  =  urt4(i1,i2,i3,ey)
   vrt  =  ust4(i1,i2,i3,ey)
   vrss = urrt4(i1,i2,i3,ey)
   vrtt = usst4(i1,i2,i3,ey)

   wrs  =  urt4(i1,i2,i3,ez)
   wrt  =  ust4(i1,i2,i3,ez)
   wrss = urrt4(i1,i2,i3,ez)
   wrtt = usst4(i1,i2,i3,ez)

   c11r = C11D3t4(i1,i2,i3)
   c22r = C22D3t4(i1,i2,i3)
   c33r = C33D3t4(i1,i2,i3)

   c1r = C1D3t4(i1,i2,i3)
   c2r = C2D3t4(i1,i2,i3)
   c3r = C3D3t4(i1,i2,i3)
 end if

 Da1DotU=0.

 ! bf = RHS to (a1.Delta u).r =0 
 ! Here are the terms that remain after we eliminate the urrr, urr and ur terms
 bf = a11*( c22*urss + c22r*uss + c2*urs + c2r*us + c33*urtt + c33r*utt + c3*urt + c3r*ut ) \
     +a12*( c22*vrss + c22r*vss + c2*vrs + c2r*vs + c33*vrtt + c33r*vtt + c3*vrt + c3r*vt ) \
     +a13*( c22*wrss + c22r*wss + c2*wrs + c2r*ws + c33*wrtt + c33r*wtt + c3*wrt + c3r*wt ) \
     +a11r*( c22*uss + c2*us + c33*utt + c3*ut ) \
     +a12r*( c22*vss + c2*vs + c33*vtt + c3*vt ) \
     +a13r*( c22*wss + c2*ws + c33*wtt + c3*wt ) 

 if( forcingOption.eq.planeWaveBoundaryForcing )then
   ! In the plane wave forcing case we subtract out a plane wave incident field
   a21s = DS4($A21D3J)
   a22s = DS4($A22D3J)
   a23s = DS4($A23D3J)
  
   a31t = DT4($A31D3J)
   a32t = DT4($A32D3J)
   a33t = DT4($A33D3J)

   ! *** set RHS for (a1.u).r =  - Ds( a2.uv ) -Dt( a3.uv )
   Da1DotU = -(  a21s*uex+a22s*uey+a23s*uez + a21*us+a22*vs+a23*ws \
               + a31t*uex+a32t*uey+a33t*uez + a31*ut+a32*vt+a33*wt )

   ! *** NOTE: "d" denotes the time derivative as in udd = two time derivatives of u
   ! (a1.Delta u).r = - (a2.utt).s - (a3.utt).t
   ! (a1.Delta u).r + bf = 0
   ! bf = bf + (a2.utt).s + (a3.utt).t

   getMinusPlaneWave3Dtt(i1,i2,i3,t,udd,vdd,wdd)

   getMinusPlaneWave3Dtt(i1+js1,i2+js2,i3+js3,t,uddp1,vddp1,wddp1)
   getMinusPlaneWave3Dtt(i1-js1,i2-js2,i3-js3,t,uddm1,vddm1,wddm1)
   ! 2nd-order here should be good enough:
   udds = (uddp1-uddm1)/(2.*dsa)
   vdds = (vddp1-vddm1)/(2.*dsa)
   wdds = (wddp1-wddm1)/(2.*dsa)

   getMinusPlaneWave3Dtt(i1+ks1,i2+ks2,i3+ks3,t,uddp1,vddp1,wddp1)
   getMinusPlaneWave3Dtt(i1-ks1,i2-ks2,i3-ks3,t,uddm1,vddm1,wddm1)
   ! 2nd-order here should be good enough:
   uddt = (uddp1-uddm1)/(2.*dta)
   vddt = (vddp1-vddm1)/(2.*dta)
   wddt = (wddp1-wddm1)/(2.*dta)

   bf = bf + a21s*udd+a22s*vdd+a23s*wdd + a21*udds + a22*vdds+ a23*wdds \
           + a31t*udd+a32t*vdd+a33t*wdd + a31*uddt + a32*vddt+ a33*wddt


 end if

 #If #FORCING == "twilightZone"
 if( useForcing.ne.0 )then

  ! OGF3D(i1,i2,i3,t, uv0(0),uv0(1),uv0(2))
  ! OGF3D(i1-is1,i2-is2,i3-is3,t, uvm(0),uvm(1),uvm(2))
  ! OGF3D(i1+is1,i2+is2,i3+is3,t, uvp(0),uvp(1),uvp(2))
  ! OGF3D(i1-2*is1,i2-2*is2,i3-2*is3,t, uvm2(0),uvm2(1),uvm2(2))
  ! OGF3D(i1+2*is1,i2+2*is2,i3+2*is3,t, uvp2(0),uvp2(1),uvp2(2))


  OGDERIV3D(0, 2,0,0, i1,i2,i3, t,uxx,vxx,wxx)
  OGDERIV3D(0, 0,2,0, i1,i2,i3, t,uyy,vyy,wyy)
  OGDERIV3D(0, 0,0,2, i1,i2,i3, t,uzz,vzz,wzz)

  OGDERIV3D(0, 2,0,0, i1-is1,i2-is2,i3-is3, t,uxxm1,vxxm1,wxxm1)
  OGDERIV3D(0, 0,2,0, i1-is1,i2-is2,i3-is3, t,uyym1,vyym1,wyym1)
  OGDERIV3D(0, 0,0,2, i1-is1,i2-is2,i3-is3, t,uzzm1,vzzm1,wzzm1)

  OGDERIV3D(0, 2,0,0, i1+is1,i2+is2,i3+is3, t,uxxp1,vxxp1,wxxp1)
  OGDERIV3D(0, 0,2,0, i1+is1,i2+is2,i3+is3, t,uyyp1,vyyp1,wyyp1)
  OGDERIV3D(0, 0,0,2, i1+is1,i2+is2,i3+is3, t,uzzp1,vzzp1,wzzp1)

  OGDERIV3D(0, 2,0,0, i1-2*is1,i2-2*is2,i3-2*is3, t,uxxm2,vxxm2,wxxm2)
  OGDERIV3D(0, 0,2,0, i1-2*is1,i2-2*is2,i3-2*is3, t,uyym2,vyym2,wyym2)
  OGDERIV3D(0, 0,0,2, i1-2*is1,i2-2*is2,i3-2*is3, t,uzzm2,vzzm2,wzzm2)

  OGDERIV3D(0, 2,0,0, i1+2*is1,i2+2*is2,i3+2*is3, t,uxxp2,vxxp2,wxxp2)
  OGDERIV3D(0, 0,2,0, i1+2*is1,i2+2*is2,i3+2*is3, t,uyyp2,vyyp2,wyyp2)
  OGDERIV3D(0, 0,0,2, i1+2*is1,i2+2*is2,i3+2*is3, t,uzzp2,vzzp2,wzzp2)

  utt00=uxx+uyy+uzz
  vtt00=vxx+vyy+vzz
  wtt00=wxx+wyy+wzz

  ! For TZ choose bf = bf - Dr( a1.Delta uvExact )
  bf = bf - a11r*utt00 - a12r*vtt00 - a13r*wtt00 \
       -a11*( 8.*((uxxp1+uyyp1+uzzp1)-(uxxm1+uyym1+uzzm1))-((uxxp2+uyyp2+uzzp2)-(uxxm2+uyym2+uzzm2)) )/(12.*dra) \
       -a12*( 8.*((vxxp1+vyyp1+vzzp1)-(vxxm1+vyym1+vzzm1))-((vxxp2+vyyp2+vzzp2)-(vxxm2+vyym2+vzzm2)) )/(12.*dra) \
       -a13*( 8.*((wxxp1+wyyp1+wzzp1)-(wxxm1+wyym1+wzzm1))-((wxxp2+wyyp2+wzzp2)-(wxxm2+wyym2+wzzm2)) )/(12.*dra)

  ! For testing we could set
  !    bf = a1.( c11*urrr + c11r*urr + c1*urr + c1r*ur ) + a1r.( c11*urr + c1*ur )

  ! Da1DotU = (a1.uv).r to 4th order
  ! Da1DotU = (8.*( (a11p1*uvp(0) +a12p1*uvp(1))  - (a11m1*uvm(0) +a12m1*uvm(1)) )\
  !             - ( (a11p2*uvp2(0)+a12p2*uvp2(1)) - (a11m2*uvm2(0)+a12m2*uvm2(1)) ) )/(12.*dra)

  ! **** compute RHS for div(u) equation ****
  a21zp1= A21D3J(i1+js1,i2+js2,i3+js3) 
  a21zm1= A21D3J(i1-js1,i2-js2,i3-js3) 
  a21zp2= A21D3J(i1+2*js1,i2+2*js2,i3+2*js3) 
  a21zm2= A21D3J(i1-2*js1,i2-2*js2,i3-2*js3) 
 
  a22zp1= A22D3J(i1+js1,i2+js2,i3+js3) 
  a22zm1= A22D3J(i1-js1,i2-js2,i3-js3) 
  a22zp2= A22D3J(i1+2*js1,i2+2*js2,i3+2*js3) 
  a22zm2= A22D3J(i1-2*js1,i2-2*js2,i3-2*js3) 

  a23zp1= A23D3J(i1+js1,i2+js2,i3+js3) 
  a23zm1= A23D3J(i1-js1,i2-js2,i3-js3) 
  a23zp2= A23D3J(i1+2*js1,i2+2*js2,i3+2*js3) 
  a23zm2= A23D3J(i1-2*js1,i2-2*js2,i3-2*js3) 

  a31zp1= A31D3J(i1+ks1,i2+ks2,i3+ks3) 
  a31zm1= A31D3J(i1-ks1,i2-ks2,i3-ks3) 
  a31zp2= A31D3J(i1+2*ks1,i2+2*ks2,i3+2*ks3) 
  a31zm2= A31D3J(i1-2*ks1,i2-2*ks2,i3-2*ks3) 
 
  a32zp1= A32D3J(i1+ks1,i2+ks2,i3+ks3) 
  a32zm1= A32D3J(i1-ks1,i2-ks2,i3-ks3) 
  a32zp2= A32D3J(i1+2*ks1,i2+2*ks2,i3+2*ks3) 
  a32zm2= A32D3J(i1-2*ks1,i2-2*ks2,i3-2*ks3) 

  a33zp1= A33D3J(i1+ks1,i2+ks2,i3+ks3) 
  a33zm1= A33D3J(i1-ks1,i2-ks2,i3-ks3) 
  a33zp2= A33D3J(i1+2*ks1,i2+2*ks2,i3+2*ks3) 
  a33zm2= A33D3J(i1-2*ks1,i2-2*ks2,i3-2*ks3) 

  ! *** set to - Ds( a2.uv ) -Dt( a3.uv )
  Da1DotU = -(  \
       ( 8.*(a21zp1*u(i1+  js1,i2+  js2,i3+  js3,ex)-a21zm1*u(i1-  js1,i2-  js2,i3-  js3,ex)) \
           -(a21zp2*u(i1+2*js1,i2+2*js2,i3+2*js3,ex)-a21zm2*u(i1-2*js1,i2-2*js2,i3-2*js3,ex)) )/(12.*dsa) \
      +( 8.*(a22zp1*u(i1+  js1,i2+  js2,i3+  js3,ey)-a22zm1*u(i1-  js1,i2-  js2,i3-  js3,ey)) \
           -(a22zp2*u(i1+2*js1,i2+2*js2,i3+2*js3,ey)-a22zm2*u(i1-2*js1,i2-2*js2,i3-2*js3,ey)) )/(12.*dsa) \
      +( 8.*(a23zp1*u(i1+  js1,i2+  js2,i3+  js3,ez)-a23zm1*u(i1-  js1,i2-  js2,i3-  js3,ez)) \
           -(a23zp2*u(i1+2*js1,i2+2*js2,i3+2*js3,ez)-a23zm2*u(i1-2*js1,i2-2*js2,i3-2*js3,ez)) )/(12.*dsa)  ) \
             -(  \
       ( 8.*(a31zp1*u(i1+  ks1,i2+  ks2,i3+  ks3,ex)-a31zm1*u(i1-  ks1,i2-  ks2,i3-  ks3,ex)) \
           -(a31zp2*u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ex)-a31zm2*u(i1-2*ks1,i2-2*ks2,i3-2*ks3,ex)) )/(12.*dta) \
      +( 8.*(a32zp1*u(i1+  ks1,i2+  ks2,i3+  ks3,ey)-a32zm1*u(i1-  ks1,i2-  ks2,i3-  ks3,ey)) \
           -(a32zp2*u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ey)-a32zm2*u(i1-2*ks1,i2-2*ks2,i3-2*ks3,ey)) )/(12.*dta) \
      +( 8.*(a33zp1*u(i1+  ks1,i2+  ks2,i3+  ks3,ez)-a33zm1*u(i1-  ks1,i2-  ks2,i3-  ks3,ez)) \
           -(a33zp2*u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ez)-a33zm2*u(i1-2*ks1,i2-2*ks2,i3-2*ks3,ez)) )/(12.*dta)  )

 end if
 #End


! Now assign E at the ghost points:
#Include "bc4Maxwell3d.h"

!  if( .true. .or. debug.gt.0 )then
!   write(*,'(" bc4:corr:   i1,i2,i3=",3i3," u(-1)=",3f8.2," u(-2)=",3f8.2)') i1,i2,i3,\
!          u(i1-is1,i2-is2,i3-is3,ex),u(i1-is1,i2-is2,i3-is3,ey),u(i1-is1,i2-is2,i3-is3,ez),\
!          u(i1-2*is1,i2-2*is2,i3-2*is3,ex),u(i1-2*is1,i2-2*is2,i3-2*is3,ey),u(i1-2*is1,i2-2*is2,i3-2*is3,ez)
!  end if

  if( debug.gt.0 )then
   OGF3D(i1-is1,i2-is2,i3-is3,t, uvm(0),uvm(1),uvm(2))
   OGF3D(i1-2*is1,i2-2*is2,i3-2*is3,t, uvm2(0),uvm2(1),uvm2(2))
   write(*,'(" **bc4:correction: i=",3i4," errors u(-1)=",3e10.2)') i1,i2,i3,u(i1-is1,i2-is2,i3-is3,ex)-uvm(0),\
               u(i1-is1,i2-is2,i3-is3,ey)-uvm(1),u(i1-is1,i2-is2,i3-is3,ez)-uvm(2)
   write(*,'(" **bc4:correction: i=",3i4," errors u(-2)=",3e10.2)') i1,i2,i3,u(i1-2*is1,i2-2*is2,i3-2*is3,ex)-uvm2(0),\
               u(i1-2*is1,i2-2*is2,i3-2*is3,ey)-uvm2(1),u(i1-2*is1,i2-2*is2,i3-2*is3,ez)-uvm2(2)
  end if


  ! set to exact for testing
  ! OGF3D(i1-is1,i2-is2,i3-is3,t, uvm(0),uvm(1),uvm(2))
  ! OGF3D(i1-2*is1,i2-2*is2,i3-2*is3,t, uvm2(0),uvm2(1),uvm2(2))
  ! u(i1-is1,i2-is2,i3-is3,ex)=uvm(0)
  ! u(i1-is1,i2-is2,i3-is3,ey)=uvm(1)
  ! u(i1-is1,i2-is2,i3-is3,ez)=uvm(2)

  ! u(i1-2*is1,i2-2*is2,i3-2*is3,ex)=uvm2(0)
  ! u(i1-2*is1,i2-2*is2,i3-2*is3,ey)=uvm2(1)
  ! u(i1-2*is1,i2-2*is2,i3-2*is3,ez)=uvm2(2)


end if ! mask
endLoops()

end if ! if true




if( debug.gt.0 )then

! ============================DEBUG=======================================================
#If #FORCING == "twilightZone"
if( useForcing.ne.0 )then

! **** check that we satisfy all the equations ****
maxDivc=0.
maxTauDotLapu=0.
maxExtrap=0.
maxDr3aDotU=0.

write(*,'(" ***bc4:START grid=",i4,", side,axis=",2i3," **** ")') grid,side,axis

beginLoops()
if( mask(i1,i2,i3).gt.0 )then

 defineMetricDerivatives1()
 getTangentialDerivatives1()
 if( axis.eq.0 )then
   defineMetricDerivatives2(rs4,rt4,st4, rss4,rtt4,sst4,stt4)
   getTangentialDerivatives2(ust4,usst4,ustt4)
 else if( axis.eq.1 )then
   defineMetricDerivatives2(st4,rs4,rt4,stt4,rrs4,rtt4,rrt4)
   getTangentialDerivatives2(urt4,urtt4,urrt4)
 else ! axis.eq.2
   defineMetricDerivatives2(rt4,st4,rs4,rrt4,sst4,rrs4,rss4)
   getTangentialDerivatives2(urs4,urrs4,urss4)
 end if

 tau11=rsxy(i1,i2,i3,axisp1,0)
 tau12=rsxy(i1,i2,i3,axisp1,1)
 tau13=rsxy(i1,i2,i3,axisp1,2)

 tau21=rsxy(i1,i2,i3,axisp2,0)
 tau22=rsxy(i1,i2,i3,axisp2,1)
 tau23=rsxy(i1,i2,i3,axisp2,2)

 uex=u(i1,i2,i3,ex)
 uey=u(i1,i2,i3,ey)
 uez=u(i1,i2,i3,ez)

 ur=UR4(ex)
 vr=UR4(ey)

 urr=URR4(ex)
 vrr=URR4(ey)

 urs=URS4(ex)
 vrs=URS4(ey)

 urrs=URRS4(ex)
 vrrs=URRS4(ey)

 urrr=URRR2(ex)
 vrrr=URRR2(ey)

 urss=URSS4(ex)
 vrss=URSS4(ey)

 div = ux43(i1,i2,i3,ex)+uy43(i1,i2,i3,ey)+uz43(i1,i2,i3,ez)

 a11zp1= A11D3J(i1+is1,i2+is2,i3+is3) 
 a11zm1= A11D3J(i1-is1,i2-is2,i3-is3) 
 a11zp2= A11D3J(i1+2*is1,i2+2*is2,i3+2*is3) 
 a11zm2= A11D3J(i1-2*is1,i2-2*is2,i3-2*is3) 

 a12zp1= A12D3J(i1+is1,i2+is2,i3+is3) 
 a12zm1= A12D3J(i1-is1,i2-is2,i3-is3) 
 a12zp2= A12D3J(i1+2*is1,i2+2*is2,i3+2*is3) 
 a12zm2= A12D3J(i1-2*is1,i2-2*is2,i3-2*is3) 

 a13zp1= A13D3J(i1+is1,i2+is2,i3+is3) 
 a13zm1= A13D3J(i1-is1,i2-is2,i3-is3) 
 a13zp2= A13D3J(i1+2*is1,i2+2*is2,i3+2*is3) 
 a13zm2= A13D3J(i1-2*is1,i2-2*is2,i3-2*is3) 

 a21zp1= A21D3J(i1+js1,i2+js2,i3+js3) 
 a21zm1= A21D3J(i1-js1,i2-js2,i3-js3) 
 a21zp2= A21D3J(i1+2*js1,i2+2*js2,i3+2*js3) 
 a21zm2= A21D3J(i1-2*js1,i2-2*js2,i3-2*js3) 

 a22zp1= A22D3J(i1+js1,i2+js2,i3+js3) 
 a22zm1= A22D3J(i1-js1,i2-js2,i3-js3) 
 a22zp2= A22D3J(i1+2*js1,i2+2*js2,i3+2*js3) 
 a22zm2= A22D3J(i1-2*js1,i2-2*js2,i3-2*js3) 

 a23zp1= A23D3J(i1+js1,i2+js2,i3+js3) 
 a23zm1= A23D3J(i1-js1,i2-js2,i3-js3) 
 a23zp2= A23D3J(i1+2*js1,i2+2*js2,i3+2*js3) 
 a23zm2= A23D3J(i1-2*js1,i2-2*js2,i3-2*js3) 

 a31zp1= A31D3J(i1+ks1,i2+ks2,i3+ks3) 
 a31zm1= A31D3J(i1-ks1,i2-ks2,i3-ks3) 
 a31zp2= A31D3J(i1+2*ks1,i2+2*ks2,i3+2*ks3) 
 a31zm2= A31D3J(i1-2*ks1,i2-2*ks2,i3-2*ks3) 

 a32zp1= A32D3J(i1+ks1,i2+ks2,i3+ks3) 
 a32zm1= A32D3J(i1-ks1,i2-ks2,i3-ks3) 
 a32zp2= A32D3J(i1+2*ks1,i2+2*ks2,i3+2*ks3) 
 a32zm2= A32D3J(i1-2*ks1,i2-2*ks2,i3-2*ks3) 

 a33zp1= A33D3J(i1+ks1,i2+ks2,i3+ks3) 
 a33zm1= A33D3J(i1-ks1,i2-ks2,i3-ks3) 
 a33zp2= A33D3J(i1+2*ks1,i2+2*ks2,i3+2*ks3) 
 a33zm2= A33D3J(i1-2*ks1,i2-2*ks2,i3-2*ks3) 

 ! conservative form of the divergence
 divc=\
       ( 8.*(a11zp1*u(i1+  is1,i2+  is2,i3+  is3,ex)-a11zm1*u(i1-  is1,i2-  is2,i3-  is3,ex)) \
           -(a11zp2*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)-a11zm2*u(i1-2*is1,i2-2*is2,i3-2*is3,ex)) )/(12.*dra) \
      +( 8.*(a12zp1*u(i1+  is1,i2+  is2,i3+  is3,ey)-a12zm1*u(i1-  is1,i2-  is2,i3-  is3,ey)) \
           -(a12zp2*u(i1+2*is1,i2+2*is2,i3+2*is3,ey)-a12zm2*u(i1-2*is1,i2-2*is2,i3-2*is3,ey)) )/(12.*dra) \
      +( 8.*(a13zp1*u(i1+  is1,i2+  is2,i3+  is3,ez)-a13zm1*u(i1-  is1,i2-  is2,i3-  is3,ez)) \
           -(a13zp2*u(i1+2*is1,i2+2*is2,i3+2*is3,ez)-a13zm2*u(i1-2*is1,i2-2*is2,i3-2*is3,ez)) )/(12.*dra)  \
      +( 8.*(a21zp1*u(i1+  js1,i2+  js2,i3+  js3,ex)-a21zm1*u(i1-  js1,i2-  js2,i3-  js3,ex)) \
           -(a21zp2*u(i1+2*js1,i2+2*js2,i3+2*js3,ex)-a21zm2*u(i1-2*js1,i2-2*js2,i3-2*js3,ex)) )/(12.*dsa) \
      +( 8.*(a22zp1*u(i1+  js1,i2+  js2,i3+  js3,ey)-a22zm1*u(i1-  js1,i2-  js2,i3-  js3,ey)) \
           -(a22zp2*u(i1+2*js1,i2+2*js2,i3+2*js3,ey)-a22zm2*u(i1-2*js1,i2-2*js2,i3-2*js3,ey)) )/(12.*dsa) \
      +( 8.*(a23zp1*u(i1+  js1,i2+  js2,i3+  js3,ez)-a23zm1*u(i1-  js1,i2-  js2,i3-  js3,ez)) \
           -(a23zp2*u(i1+2*js1,i2+2*js2,i3+2*js3,ez)-a23zm2*u(i1-2*js1,i2-2*js2,i3-2*js3,ez)) )/(12.*dsa)  \
      +( 8.*(a31zp1*u(i1+  ks1,i2+  ks2,i3+  ks3,ex)-a31zm1*u(i1-  ks1,i2-  ks2,i3-  ks3,ex)) \
           -(a31zp2*u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ex)-a31zm2*u(i1-2*ks1,i2-2*ks2,i3-2*ks3,ex)) )/(12.*dta) \
      +( 8.*(a32zp1*u(i1+  ks1,i2+  ks2,i3+  ks3,ey)-a32zm1*u(i1-  ks1,i2-  ks2,i3-  ks3,ey)) \
           -(a32zp2*u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ey)-a32zm2*u(i1-2*ks1,i2-2*ks2,i3-2*ks3,ey)) )/(12.*dta) \
      +( 8.*(a33zp1*u(i1+  ks1,i2+  ks2,i3+  ks3,ez)-a33zm1*u(i1-  ks1,i2-  ks2,i3-  ks3,ez)) \
           -(a33zp2*u(i1+2*ks1,i2+2*ks2,i3+2*ks3,ez)-a33zm2*u(i1-2*ks1,i2-2*ks2,i3-2*ks3,ez)) )/(12.*dta) 

 divc=divc*RXDET3D(i1,i2,i3)

 tau1Up1=tau11*(u(i1-2*is1,i2-2*is2,i3-2*is3,ex)-4.*u(i1-is1,i2-is2,i3-is3,ex)+6.*u(i1,i2,i3,ex)\
                                                -4.*u(i1+is1,i2+is2,i3+is3,ex)+u(i1+2*is1,i2+2*is2,i3+2*is3,ex))\
        +tau12*(u(i1-2*is1,i2-2*is2,i3-2*is3,ey)-4.*u(i1-is1,i2-is2,i3-is3,ey)+6.*u(i1,i2,i3,ey)\
                                                -4.*u(i1+is1,i2+is2,i3+is3,ey)+u(i1+2*is1,i2+2*is2,i3+2*is3,ey))\
        +tau13*(u(i1-2*is1,i2-2*is2,i3-2*is3,ez)-4.*u(i1-is1,i2-is2,i3-is3,ez)+6.*u(i1,i2,i3,ez)\
                                                -4.*u(i1+is1,i2+is2,i3+is3,ez)+u(i1+2*is1,i2+2*is2,i3+2*is3,ez))

 tau2Up1=tau21*(u(i1-2*is1,i2-2*is2,i3-2*is3,ex)-4.*u(i1-is1,i2-is2,i3-is3,ex)+6.*u(i1,i2,i3,ex)\
                                                -4.*u(i1+is1,i2+is2,i3+is3,ex)+u(i1+2*is1,i2+2*is2,i3+2*is3,ex))\
        +tau22*(u(i1-2*is1,i2-2*is2,i3-2*is3,ey)-4.*u(i1-is1,i2-is2,i3-is3,ey)+6.*u(i1,i2,i3,ey)\
                                                -4.*u(i1+is1,i2+is2,i3+is3,ey)+u(i1+2*is1,i2+2*is2,i3+2*is3,ey))\
        +tau23*(u(i1-2*is1,i2-2*is2,i3-2*is3,ez)-4.*u(i1-is1,i2-is2,i3-is3,ez)+6.*u(i1,i2,i3,ez)\
                                                -4.*u(i1+is1,i2+is2,i3+is3,ez)+u(i1+2*is1,i2+2*is2,i3+2*is3,ez))

 uLap=ulaplacian43(i1,i2,i3,ex)
 vLap=ulaplacian43(i1,i2,i3,ey)
 wLap=ulaplacian43(i1,i2,i3,ez)
 tau1DotLap= tau11*uLap+tau12*vLap+tau13*wLap
 tau2DotLap= tau21*uLap+tau22*vLap+tau23*wLap


 errLapex=(c11*URR4(ex)+c22*USS4(ex)+c33*UTT4(ex)+c1*UR4(ex)+c2*US4(ex)+c3*UT4(ex))-uLap
 errLapey=(c11*URR4(ey)+c22*USS4(ey)+c33*UTT4(ey)+c1*UR4(ey)+c2*US4(ey)+c3*UT4(ey))-vLap
 errLapez=(c11*URR4(ez)+c22*USS4(ez)+c33*UTT4(ez)+c1*UR4(ez)+c2*US4(ez)+c3*UT4(ez))-wLap

 
 ! f1 := Dzr(Dpr(Dmr( a11*u + a12*v )))(i1,i2,i3)/dra^3 - cur*Dzr(u)(i1,i2,i3)/dra - cvr*Dzr(v)(i1,i2,i3)/dra - gI:

  OGF3D(i1,i2,i3,t, uv0(0),uv0(1),uv0(2))
  OGF3D(i1-is1,i2-is2,i3-is3,t, uvm(0),uvm(1),uvm(2))
  OGF3D(i1+is1,i2+is2,i3+is3,t, uvp(0),uvp(1),uvp(2))
  OGF3D(i1-2*is1,i2-2*is2,i3-2*is3,t, uvm2(0),uvm2(1),uvm2(2))
  OGF3D(i1+2*is1,i2+2*is2,i3+2*is3,t, uvp2(0),uvp2(1),uvp2(2))

 tau1DotU = tau11*(uex-uv0(0))+tau12*(uey-uv0(1))+tau13*(uez-uv0(2))
 tau2DotU = tau21*(uex-uv0(0))+tau22*(uey-uv0(1))+tau23*(uez-uv0(2))


 write(*,'("  bc4: (i1,i2,i3)=(",i6,",",i6,",",i6,") (side,axis)=(",i2,",",i2,")")') i1,i2,i3,side,axis

!  write(*,'("  bc4: a1=(",3e10.2,"), tau1=(",3e10.2,"), tau2=(",3e10.2,")")') a11,a12,a13,tau11,tau12,tau13,tau21,tau22,tau23
!  write(*,'("  bc4: a11r,a12r,a13r=",3e10.2)') a11r,a12r,a13r
!  write(*,'("  bc4: a11s,a12s,a13s=",3e10.2)') a11s,a12s,a13s
!  write(*,'("  bc4: a11t,a12t,a13t=",3e10.2)') a11t,a12t,a13t
!  write(*,'("  bc4: a11ss,a12ss,a13ss=",3e10.2)') a11ss,a12ss,a13ss
! 
!  write(*,'("  bc4: a21r,a22r,a23r=",3e10.2)') a21r,a22r,a23r
!  write(*,'("  bc4: a21s,a22s,a23s=",3e10.2)') a21s,a22s,a23s
!  write(*,'("  bc4: a21t,a22t,a23t=",3e10.2)') a21t,a22t,a23t
!  write(*,'("  bc4: a21ss,a22ss,a23ss=",3e10.2)') a21ss,a22ss,a23ss
! 
!  write(*,'("  bc4: a31r,a32r,a33r=",3e10.2)') a31r,a32r,a33r
!  write(*,'("  bc4: a31s,a32s,a33s=",3e10.2)') a31s,a32s,a33s
!  write(*,'("  bc4: a31t,a32t,a33t=",3e10.2)') a31t,a32t,a33t
!  write(*,'("  bc4: a31tt,a32tt,a33tt=",3e10.2)') a31tt,a32tt,a33tt
! 
!  write(*,'("  bc4: c11,c22,c33,c1,c2,c3=",6e10.2)') c11,c22,c33,c1,c2,c3
!  write(*,'("  bc4: c11r,c22r,c33r,c1r,c2r,c3r=",6e10.2)') c11r,c22r,c33r,c1r,c2r,c3r
!  write(*,'("  bc4: c11s,c22s,c33s,c1s,c2s,c3s=",6e10.2)') c11s,c22s,c33s,c1s,c2s,c3s
!  write(*,'("  bc4: c11t,c22t,c33t,c1t,c2t,c3t=",6e10.2)') c11t,c22t,c33t,c1t,c2t,c3t

! print neighbours
!  do m3=-2,2
!  do m2=-2,2
!  do m1=-2,2
!    OGF3D(i1+m1,i2+m2,i3+m3,t,uvm(0),uvm(1),uvm(2)
!    write(*,'("  err(E(",i2,",",i2,",",i2,") =",3e9.1)')\
!       i1+m1,i2+m2,i3+m3,\
!       u(i1+m1,i2+m2,i3+m3,ex)-uvm(0),\
!       u(i1+m1,i2+m2,i3+m3,ey)-uvm(1),\
!       u(i1+m1,i2+m2,i3+m3,ez)-uvm(2)
!  end do
!  end do
!  end do

 write(*,'("  bc4: E(-1)=",3e11.3,", E(-2)=",3e11.3)') u(i1-is1,i2-is2,i3-is3,ex),u(i1-is1,i2-is2,i3-is3,ey),\
   u(i1-is1,i2-is2,i3-is3,ez),u(i1-2*is1,i2-2*is2,i3-2*is3,ex),u(i1-2*is1,i2-2*is2,i3-2*is3,ey),\
   u(i1-2*is1,i2-2*is2,i3-2*is3,ez)

 write(*,'("  bc4: err(E)(-1) =",3e11.3," err(E)(-2)=",3e11.3)') \
   u(i1-is1,i2-is2,i3-is3,ex)-uvm(0),u(i1-is1,i2-is2,i3-is3,ey)-uvm(1),u(i1-is1,i2-is2,i3-is3,ez)-uvm(2),\
   u(i1-2*is1,i2-2*is2,i3-2*is3,ex)-uvm2(0),u(i1-2*is1,i2-2*is2,i3-2*is3,ey)-uvm2(1),\
   u(i1-2*is1,i2-2*is2,i3-2*is3,ez)-uvm2(2)
 write(*,'("  bc4: err(E)(0) =",3e11.3)') u(i1,i2,i3,ex)-uv0(0),u(i1,i2,i3,ey)-uv0(1),u(i1,i2,i3,ez)-uv0(2)
 
 write(*,'("  bc4: err(tau1.u)=",e9.1,", err(tau2.u)=",e9.1," div4(u)=",e9.1," divc(u)=",e9.1,", divc2=",e9.1)') \
          tau1DotU,tau2DotU,div,divc,divc2

! ttu11 = tau1.u(-1), ttu12 = tau1.u(-2)
! ttu21 = tau2.u(-1), ttu22 = tau2.u(-2)
 write(*,'("  bc4: err(tau1.u(-1,-2))=",2e9.1," err(tau2.u(-1,-2))=",2e9.1)')\
    tau11*(u(i1-is1,i2-is2,i3-is3,ex)-uvm(0))+tau12*(u(i1-is1,i2-is2,i3-is3,ey)-uvm(1))\
   +tau13*(u(i1-is1,i2-is2,i3-is3,ez)-uvm(2)),\
    tau11*(u(i1-2*is1,i2-2*is2,i3-2*is3,ex)-uvm2(0))+tau12*(u(i1-2*is1,i2-2*is2,i3-2*is3,ey)-uvm2(1))\
   +tau13*(u(i1-2*is1,i2-2*is2,i3-2*is3,ez)-uvm2(2)),\
    tau21*(u(i1-is1,i2-is2,i3-is3,ex)-uvm(0))+tau22*(u(i1-is1,i2-is2,i3-is3,ey)-uvm(1))\
   +tau23*(u(i1-is1,i2-is2,i3-is3,ez)-uvm(2)),\
    tau21*(u(i1-2*is1,i2-2*is2,i3-2*is3,ex)-uvm2(0))+tau22*(u(i1-2*is1,i2-2*is2,i3-2*is3,ey)-uvm2(1))\
   +tau23*(u(i1-2*is1,i2-2*is2,i3-2*is3,ez)-uvm2(2))


 write(*,'("  bc4: a1.extrap(u(-2))=",e10.2)') \
    a11*(  u(i1-2*is1,i2-2*is2,i3-2*is3,ex)-6.*u(i1-is1,i2-is2,i3-is3,ex)+15.*u(i1,i2,i3,ex)\
      -20.*u(i1+is1,i2+is2,i3+is3,ex)+15.*u(i1+2*is1,i2+2*is2,i3+2*is3,ex)\
       -6.*u(i1+3*is1,i2+3*is2,i3+3*is3,ex)+u(i1+4*is1,i2+4*is2,i3+4*is3,ex) )\
   +a12*(  u(i1-2*is1,i2-2*is2,i3-2*is3,ey)-6.*u(i1-is1,i2-is2,i3-is3,ey)+15.*u(i1,i2,i3,ey)\
      -20.*u(i1+is1,i2+is2,i3+is3,ey)+15.*u(i1+2*is1,i2+2*is2,i3+2*is3,ey)\
       -6.*u(i1+3*is1,i2+3*is2,i3+3*is3,ey)+u(i1+4*is1,i2+4*is2,i3+4*is3,ey) )\
   +a13*(  u(i1-2*is1,i2-2*is2,i3-2*is3,ez)-6.*u(i1-is1,i2-is2,i3-is3,ez)+15.*u(i1,i2,i3,ez)\
      -20.*u(i1+is1,i2+is2,i3+is3,ez)+15.*u(i1+2*is1,i2+2*is2,i3+2*is3,ez)\
       -6.*u(i1+3*is1,i2+3*is2,i3+3*is3,ez)+u(i1+4*is1,i2+4*is2,i3+4*is3,ez) )


! These need use to recompute ttu11,...
! write(*,'("  bc4: tau1.u(-1)-ttu11=",e9.1,", tau1.u(2)-ttu12=",e9.1)')\
!   tau11*u(i1-is1,i2-is2,i3-is3,ex)+tau12*u(i1-is1,i2-is2,i3-is3,ey)+tau13*u(i1-is1,i2-is2,i3-is3,ez) -ttu11,\
!   tau11*u(i1-2*is1,i2-2*is2,i3-2*is3,ex)+tau12*u(i1-2*is1,i2-2*is2,i3-2*is3,ey)\
!  +tau13*u(i1-2*is1,i2-2*is2,i3-2*is3,ez) -ttu12
! write(*,'("  bc4: tau2.u(-1)-ttu21=",e9.1,", tau2.u(2)-ttu22=",e9.1)')\
!   tau21*u(i1-is1,i2-is2,i3-is3,ex)+tau22*u(i1-is1,i2-is2,i3-is3,ey)+tau23*u(i1-is1,i2-is2,i3-is3,ez) -ttu21,\
!   tau21*u(i1-2*is1,i2-2*is2,i3-2*is3,ex)+tau22*u(i1-2*is1,i2-2*is2,i3-2*is3,ey)\
!  +tau23*u(i1-2*is1,i2-2*is2,i3-2*is3,ez) -ttu22
    

 ! for now remove the error in the extrapolation ************
 gIVf1 = tau11*( uvm2(0)-4.*uvm(0)+6.*uv0(0)-4.*uvp(0)+uvp2(0)) +\
         tau12*( uvm2(1)-4.*uvm(1)+6.*uv0(1)-4.*uvp(1)+uvp2(1)) +\
         tau13*( uvm2(2)-4.*uvm(2)+6.*uv0(2)-4.*uvp(2)+uvp2(2))
 gIVf2 = tau21*( uvm2(0)-4.*uvm(0)+6.*uv0(0)-4.*uvp(0)+uvp2(0)) +\
         tau22*( uvm2(1)-4.*uvm(1)+6.*uv0(1)-4.*uvp(1)+uvp2(1)) +\
         tau23*( uvm2(2)-4.*uvm(2)+6.*uv0(2)-4.*uvp(2)+uvp2(2))

 write(*,'("  bc4: tau1.D+4u-gIV1=",e9.1,", tau2.D+4u-gIV2=",e9.1)') tau1Up1-gIVf1,tau2Up1-gIVf2


  ! For TZ: utt0 = utt - ett + Lap(e)
 OGDERIV3D(0, 2,0,0, i1,i2,i3, t,uxx,vxx,wxx)
 OGDERIV3D(0, 0,2,0, i1,i2,i3, t,uyy,vyy,wyy)
 OGDERIV3D(0, 0,0,2, i1,i2,i3, t,uzz,vzz,wzz)

 utt00=uxx+uyy+uzz
 vtt00=vxx+vyy+vzz
 wtt00=wxx+wyy+wzz

 write(*,'("  bc4: Lu-utt=",e10.2," Lv-vtt=",e10.2," Lw-wtt=",e10.2)') uLap-utt00,vLap-vtt00,wLap-wtt00
 write(*,'("  bc4: tau1.(L\uv-\uvtt)=",e10.2," tau2.(L\uv-\uvtt)=",e10.2)') \
   tau11*(uLap-utt00)+tau12*(vLap-vtt00)+tau13*(wLap-wtt00), \
   tau21*(uLap-utt00)+tau22*(vLap-vtt00)+tau23*(wLap-wtt00)

 ! '
 ! write(*,'("  bc4: tau1.Lap=",e9.1,", tau2.Lap=",e9.1)')tau1DotLap,tau2DotLap

 write(*,'("  bc4: err(lap43-(c11*urr...))=",3e9.1)') errLapex,errLapey,errLapez
 write(*,'("  bc4: err(Delta u)=",3e9.1)') uLap-utt00,vLap-vtt00,wLap-wtt00


!  write(*,'(" error in a1r.Delta u =",e11.3)') a11r*uLap+a12r*vLap+ a13r*wLap-a11r*utt00-a12r*vtt00-a13r*wtt00 
!  write(*,'(" error in (Delta u).r=",e11.3," computed,true=",2e11.3)') \
!     ( c11*urrr+ c22*urss + c33*urtt + c1*urr + c2*urs + c3*urt \
!       +c11r*urr+c22r*uss+c33r*utt+c1r*ur+c2r*us+c3r*ut)-\
!       ( 8.*((uxxp1+uyyp1+uzzp1)-(uxxm1+uyym1+uzzm1))-((uxxp2+uyyp2+uzzp2)-(uxxm2+uyym2+uzzm2)) )/(12.*dra),\
!    ( c11*urrr+ c22*urss + c33*urtt + c1*urr + c2*urs + c3*urt \
!       +c11r*urr+c22r*uss+c33r*utt+c1r*ur+c2r*us+c3r*ut), \
!       ( 8.*((uxxp1+uyyp1+uzzp1)-(uxxm1+uyym1+uzzm1))-((uxxp2+uyyp2+uzzp2)-(uxxm2+uyym2+uzzm2)) )/(12.*dra)


 write(*,'(" ")')  ! done this (i1,i2,i3)


 maxDivc=max(maxDivc,divc)
 maxTauDotLapu=max(maxTauDotLapu,tau1DotLap)
 maxTauDotLapu=max(maxTauDotLapu,tau2DotLap)
 maxExtrap=max(maxExtrap,tau1Up1)
 maxExtrap=max(maxExtrap,tau2Up1)
 ! maxDr3aDotU=max(maxDr3aDotU,g2a)

end if
endLoops()

 write(*,'(" ***bc4: grid=",i4,", side,axis=",2i3," maxDivc=",e8.1,", maxTauDotLapu=",e8.1,", maxExtrap=",e8.1,", maxDr3aDotU=",e8.1," ***** ",/)') \
          grid,side,axis,maxDivc,maxTauDotLapu,maxExtrap,maxDr3aDotU
end if ! end if forcing
#End 


! ============================END DEBUG=======================================================
end if

#endMacro



#beginMacro buildFile(NAME,DIM,ORDER)
#beginFile NAME.f
 BC_MAXWELL(NAME,DIM,ORDER)
#endFile
#endMacro

      buildFile(bcOptMaxwell2dOrder4,2,4)
      buildFile(bcOptMaxwell3dOrder4,3,4)


