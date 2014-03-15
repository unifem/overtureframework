c These next include files will define the macros that will define the difference approximations
c The actual macro is called below
#Include "defineDiffOrder2f.h"
#Include "defineDiffOrder4f.h"


c**************************************************************************

c Include macros that are common to different orders of accuracy

#Include "bcOptMaxwellMacros.h"

c**************************************************************************

c Here are macros that define the planeWave solution
#Include "planeWave.h"

c ===============================================================================
c  Set the tangential component to zero on the boundary in 2D
c ===============================================================================
#beginMacro assignBoundary2d(FORCING)

 ! Set the tangential component to zero
 if( gridType.eq.curvilinear )then
   beginLoops()
     tau1=rsxy(i1,i2,i3,axisp1,0)
     tau2=rsxy(i1,i2,i3,axisp1,1)
     tau1DotU=(tau1*u(i1,i2,i3,ex)+tau2*u(i1,i2,i3,ey))/(tau1**2+tau2**2)

     #If #FORCING == "twilightZone"
       call ogf2d(ep,xy(i1    ,i2    ,i3,0),xy(i1    ,i2    ,i3,1),t, u0,v0,w0)
       tau1DotU = tau1DotU - ( tau1*u0 + tau2*v0 )/(tau1**2+tau2**2)
     #Elif #FORCING == "none"
     #Elif #FORCING == "planeWaveBoundaryForcing"
       x0=xy(i1,i2,i3,0)
       y0=xy(i1,i2,i3,1)
       u0=-planeWave2Dex(x0,y0,t)
       v0=-planeWave2Dey(x0,y0,t)
       tau1DotU = tau1DotU - ( tau1*u0 + tau2*v0 )/(tau1**2+tau2**2)
     #Else
       stop 52785
     #End

     u(i1,i2,i3,ex)=u(i1,i2,i3,ex)-tau1DotU*tau1
     u(i1,i2,i3,ey)=u(i1,i2,i3,ey)-tau1DotU*tau2

    ! if( .true. )then
    !   write(*,'(" assignBndry: i=",3i3," u=",2f12.8," u0,v0=",2f12.8," x0,y0,t=",3f8.5," ,ssf,sfft=",5f8.5)')\
    !            i1,i2,i3,u(i1,i2,i3,ex),u(i1,i2,i3,ey),u0,v0,x0,y0,t,ssf,ssft,ssftt
    !   write(*,'(" assignBndry: tau1,tau2=",2e10.2," err tau.u=",e10.2)') tau1,tau2,tau1*u(i1,i2,i3,ex)+tau2*u(i1,i2,i3,ey) - (tau1*u0 + tau2*v0)
    !   ! write(*,'(" assignBndry: tau*uv - tau*uv0 = ",e10.2)') tau1*u(i1,i2,i3,ex)+tau2*u(i1,i2,i3,ey) - (tau1*u0 + tau2*v0)
    ! end if
   endLoops()
 else
   if( axis.eq.0 )then
     et1=ey
     et2=ez
   else if( axis.eq.1 )then
     et1=ex
     et2=ez
   else
     et1=ex
     et2=ey
   end if

   beginLoops()
     #If #FORCING == "twilightZone"
       call ogf2d(ep,xy(i1    ,i2    ,i3,0),xy(i1    ,i2    ,i3,1),t, u0,v0,w0)
       uv(0)=u0
       uv(1)=v0
       u(i1,i2,i3,et1)=uv(et1)
     #Elif #FORCING == "none"
       u(i1,i2,i3,et1)=0.
     #Elif #FORCING == "planeWaveBoundaryForcing"
       x0=xy(i1,i2,i3,0)
       y0=xy(i1,i2,i3,1)
       uv(0)=planeWave2Dex(x0,y0,t)
       uv(1)=planeWave2Dey(x0,y0,t)
       u(i1,i2,i3,et1)=uv(et1)
     #Else
       stop 52785
     #End
   endLoops()
 end if

#endMacro

c ===============================================================================
c  Set the tangential component to zero on the boundary in 3D
c ===============================================================================
#beginMacro assignBoundary3d(FORCING)

 ! Set the tangential components to zero
 if( gridType.eq.curvilinear )then
   beginLoops()
     tau11=rsxy(i1,i2,i3,axisp1,0)
     tau12=rsxy(i1,i2,i3,axisp1,1)
     tau13=rsxy(i1,i2,i3,axisp1,2)

     tau1DotU=(tau11*u(i1,i2,i3,ex)+tau12*u(i1,i2,i3,ey)+tau13*u(i1,i2,i3,ez))/(tau11**2+tau12**2+tau13**2)

     tau21=rsxy(i1,i2,i3,axisp2,0)
     tau22=rsxy(i1,i2,i3,axisp2,1)
     tau23=rsxy(i1,i2,i3,axisp2,2)

     tau2DotU=(tau21*u(i1,i2,i3,ex)+tau22*u(i1,i2,i3,ey)+tau23*u(i1,i2,i3,ez))/(tau21**2+tau22**2+tau23**2)

     #If #FORCING == "twilightZone"
       call ogf3d(ep,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t, u0,v0,w0)
       tau1DotU = tau1DotU - ( tau11*u0 + tau12*v0 + tau13*w0 )/(tau11**2+tau12**2+tau13**2)
       tau2DotU = tau2DotU - ( tau21*u0 + tau22*v0 + tau23*w0 )/(tau21**2+tau22**2+tau23**2)
     #Elif #FORCING == "none"
     #Elif #FORCING == "planeWaveBoundaryForcing"
       x0=xy(i1,i2,i3,0)
       y0=xy(i1,i2,i3,1)
       z0=xy(i1,i2,i3,2)
       u0=-planeWave3Dex(x0,y0,z0,t)
       v0=-planeWave3Dey(x0,y0,z0,t)
       w0=-planeWave3Dez(x0,y0,z0,t)
       tau1DotU = tau1DotU - ( tau11*u0 + tau12*v0 + tau13*w0 )/(tau11**2+tau12**2+tau13**2)
       tau2DotU = tau2DotU - ( tau21*u0 + tau22*v0 + tau23*w0 )/(tau21**2+tau22**2+tau23**2)
     #Else
       stop 52785
     #End

     ! ** this assumes tau1 and tau2 are orthogonal **
     u(i1,i2,i3,ex)=u(i1,i2,i3,ex)-tau1DotU*tau11-tau2DotU*tau21
     u(i1,i2,i3,ey)=u(i1,i2,i3,ey)-tau1DotU*tau12-tau2DotU*tau22
     u(i1,i2,i3,ez)=u(i1,i2,i3,ez)-tau1DotU*tau13-tau2DotU*tau23

 ! write(*,'("assignBoundary3d: i1,i2,i3=",3i3," x=",3f5.2," u0=",3f5.2," u=",3f5.2," tau1=",3f5.2," tau2=",3f5.2)')\
 !   i1,i2,i3, xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2), u0,v0,w0,u(i1,i2,i3,ex),u(i1,i2,i3,ey),u(i1,i2,i3,ez),\
 !   tau11,tau12,tau13,tau21,tau22,tau23
   

   endLoops()
 else
   if( axis.eq.0 )then
     et1=ey
     et2=ez
   else if( axis.eq.1 )then
     et1=ex
     et2=ez
   else
     et1=ex
     et2=ey
   end if

   beginLoops()
     #If #FORCING == "twilightZone"
       call ogf3d(ep,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t, u0,v0,w0)
       uv(0)=u0
       uv(1)=v0
       uv(2)=w0
       u(i1,i2,i3,et1)=uv(et1)
       u(i1,i2,i3,et2)=uv(et2)
     #Elif #FORCING == "none"
       u(i1,i2,i3,et1)=0.
       u(i1,i2,i3,et2)=0.
     #Elif #FORCING == "planeWaveBoundaryForcing"
       x0=xy(i1,i2,i3,0)
       y0=xy(i1,i2,i3,1)
       z0=xy(i1,i2,i3,2)
       uv(0)=-planeWave3Dex(x0,y0,z0,t)
       uv(1)=-planeWave3Dey(x0,y0,z0,t)
       uv(2)=-planeWave3Dez(x0,y0,z0,t)
       u(i1,i2,i3,et1)=uv(et1)
       u(i1,i2,i3,et2)=uv(et2)
     #Else
       stop 52785
     #End
   endLoops()
 end if

#endMacro


c This formula is just Taylor series: (for odd functions)
#beginMacro extrapCorner(ks1,ks2,dr1,dr2)
  u(i1-ks1,i2-ks2,i3,ex)= 2.*u(i1,i2,i3,ex)-u(i1+ks1,i2+ks2,i3,ex) + ( (dr1)**2*urr+2.*(dr1)*(dr2)*urs+(dr2)**2*uss )
  u(i1-ks1,i2-ks2,i3,ey)= 2.*u(i1,i2,i3,ey)-u(i1+ks1,i2+ks2,i3,ey) + ( (dr1)**2*vrr+2.*(dr1)*(dr2)*vrs+(dr2)**2*vss )
#endMacro

c This formula is just Taylor series: (for odd functions)
#beginMacro extrapCornerOrder4(ks1,ks2,dr1,dr2)
  u(i1-ks1,i2-ks2,i3,ex)= 2.*u(i1,i2,i3,ex)-u(i1+ks1,i2+ks2,i3,ex) + ( (dr1)**2*urr+2.*(dr1)*(dr2)*urs+(dr2)**2*uss )\
        + (1./12.)*( (dr1)**4*urrrr + 4.*(dr1)**3*(dr2)*urrrs + 6.*(dr1)**2*(dr2)**2*urrss \
                                    + 4.*(dr1)*(dr2)**3*ursss + (dr2)**4*ussss )  
  u(i1-ks1,i2-ks2,i3,ey)= 2.*u(i1,i2,i3,ey)-u(i1+ks1,i2+ks2,i3,ey) + ( (dr1)**2*vrr+2.*(dr1)*(dr2)*vrs+(dr2)**2*vss )\
        + (1./12.)*( (dr1)**4*vrrrr + 4.*(dr1)**3*(dr2)*vrrrs + 6.*(dr1)**2*(dr2)**2*vrrss \
                                    + 4.*(dr1)*(dr2)**3*vrsss + (dr2)**4*vssss )  
#endMacro


c This formula is also Taylor series (for even functions)
#beginMacro extrapCornerHzOrder4(ks1,ks2,dr1,dr2)
    u(i1-ks1,i2-ks2,i3,hz)=u(i1+ks1,i2+ks2,i3,hz) - 2.*((dr1)*ur+(dr2)*us) \
    - (1./3.)*((dr1)**3*urrr+3.*(dr1)**2*(dr2)*urrs+3.*(dr1)*(dr2)**2*urss+(dr2)**3*usss)
#endMacro

c ===================================================================================
c Determine the values at ghost points outside corners
c  
c
c GRIDTYPE: curvilinear, rectangular
c The formula for urs,vrs are from bc4v.maple
c ===================================================================================
#beginMacro ghostValuesOutsideCorners2d(ORDER,GRIDTYPE,FORCING)

 axis=0
 axisp1=1

 #If #GRIDTYPE == "curvilinear"
  ! evaluate non-mixed derivatives at the corner
  
  #If #ORDER == "2" 
    ur=ur2(i1,i2,i3,ex)
    vr=ur2(i1,i2,i3,ey)
  
    us=us2(i1,i2,i3,ex)
    vs=us2(i1,i2,i3,ey)
  
    urr=urr2(i1,i2,i3,ex)
    vrr=urr2(i1,i2,i3,ey)
  
    uss=uss2(i1,i2,i3,ex)
    vss=uss2(i1,i2,i3,ey)
  
    jac=1./RXDET2D(i1,i2,i3)
    a11 =rsxy(i1,i2,i3,0,0)*jac
    a12 =rsxy(i1,i2,i3,0,1)*jac
   
    a21 =rsxy(i1,i2,i3,1,0)*jac
    a22 =rsxy(i1,i2,i3,1,1)*jac
   
    a11r = Dr($A11)
    a12r = Dr($A12)
    a21r = Dr($A21)
    a22r = Dr($A22)
   
    a11s = Ds($A11)
    a12s = Ds($A12)
    a21s = Ds($A21)
    a22s = Ds($A22)
   
   
    a11rs = Drs($A11)
    a12rs = Drs($A12)
    a21rs = Drs($A21)
    a22rs = Drs($A22)
   
    a11rr = Drr($A11)
    a12rr = Drr($A12)
    a21rr = Drr($A21)
    a22rr = Drr($A22)
   
    a21ss = Dss($A21)
    a22ss = Dss($A22)
   
    urs=-(a12**2*vrr-2*a21s*us*a22-a21ss*u(i1,i2,i3,ex)*a22-a12s*vr*a22-a12r*vs*a22-a22**2*vss-2*a22s*vs*a22-a21*uss*a22-a11rs*u(i1,i2,i3,ex)*a22-a11s*ur*a22-a11r*us*a22+2*a12*a12r*vr+a12*a21rs*u(i1,i2,i3,ex)+a12*a12rr*u(i1,i2,i3,ey)+a12*a11*urr+2*a12*a11r*ur-a12rs*u(i1,i2,i3,ey)*a22+a12*a22r*vs+a12*a11rr*u(i1,i2,i3,ex)+a12*a22s*vr+a12*a22rs*u(i1,i2,i3,ey)+a12*a21s*ur+a12*a21r*us-a22ss*u(i1,i2,i3,ey)*a22)/(-a11*a22+a21*a12)
   
    vrs=(a11*a21rs*u(i1,i2,i3,ex)+a11*a12*vrr+2*a11*a12r*vr+a11*a12rr*u(i1,i2,i3,ey)+2*a11*a11r*ur+a11*a11rr*u(i1,i2,i3,ex)-a21*a22*vss+a11*a22r*vs+a11*a22s*vr+a11*a22rs*u(i1,i2,i3,ey)+a11*a21r*us+a11*a21s*ur-a21*a12s*vr-a21*a12r*vs-a21*a12rs*u(i1,i2,i3,ey)-a21*a11s*ur-a21*a11r*us-a21*a11rs*u(i1,i2,i3,ex)-a21*a21ss*u(i1,i2,i3,ex)-a21*a22ss*u(i1,i2,i3,ey)-2*a21*a22s*vs-2*a21*a21s*us+a11**2*urr-a21**2*uss)/(-a11*a22+a21*a12)
   
    extrapCorner(is1,is2,dra,dsa)
  
    ur = ur2(i1,i2,i3,hz)
    us = us2(i1,i2,i3,hz)
  
    u(i1-is1,i2-is2,i3,hz)= u(i1+is1,i2+is2,i3,hz) - 2.*(dra*ur+dsa*us)


  #Elif #ORDER == "4" 

    ! ***** finish this *****

    ur=ur4(i1,i2,i3,ex)
    vr=ur4(i1,i2,i3,ey)
  
    us=us4(i1,i2,i3,ex)
    vs=us4(i1,i2,i3,ey)
  
    urr=urr4(i1,i2,i3,ex)
    vrr=urr4(i1,i2,i3,ey)
  
    uss=uss4(i1,i2,i3,ex)
    vss=uss4(i1,i2,i3,ey)
  
    jac=1./RXDET2D(i1,i2,i3)
    a11 =rsxy(i1,i2,i3,0,0)*jac
    a12 =rsxy(i1,i2,i3,0,1)*jac
   
    a21 =rsxy(i1,i2,i3,1,0)*jac
    a22 =rsxy(i1,i2,i3,1,1)*jac
   
    a11r = Dr4($A11)
    a12r = Dr4($A12)
    a21r = Dr4($A21)
    a22r = Dr4($A22)
   
    a11s = Ds4($A11)
    a12s = Ds4($A12)
    a21s = Ds4($A21)
    a22s = Ds4($A22)
   
   
    a11rs = Drs4($A11)
    a12rs = Drs4($A12)
    a21rs = Drs4($A21)
    a22rs = Drs4($A22)
   
    a11rr = Drr4($A11)
    a12rr = Drr4($A12)
    a21rr = Drr4($A21)
    a22rr = Drr4($A22)
   
    a21ss = Dss4($A21)
    a22ss = Dss4($A22)
   
    urs=-(a12**2*vrr-2*a21s*us*a22-a21ss*u(i1,i2,i3,ex)*a22-a12s*vr*a22-a12r*vs*a22-a22**2*vss-2*a22s*vs*a22-a21*uss*a22-a11rs*u(i1,i2,i3,ex)*a22-a11s*ur*a22-a11r*us*a22+2*a12*a12r*vr+a12*a21rs*u(i1,i2,i3,ex)+a12*a12rr*u(i1,i2,i3,ey)+a12*a11*urr+2*a12*a11r*ur-a12rs*u(i1,i2,i3,ey)*a22+a12*a22r*vs+a12*a11rr*u(i1,i2,i3,ex)+a12*a22s*vr+a12*a22rs*u(i1,i2,i3,ey)+a12*a21s*ur+a12*a21r*us-a22ss*u(i1,i2,i3,ey)*a22)/(-a11*a22+a21*a12)
   
    vrs=(a11*a21rs*u(i1,i2,i3,ex)+a11*a12*vrr+2*a11*a12r*vr+a11*a12rr*u(i1,i2,i3,ey)+2*a11*a11r*ur+a11*a11rr*u(i1,i2,i3,ex)-a21*a22*vss+a11*a22r*vs+a11*a22s*vr+a11*a22rs*u(i1,i2,i3,ey)+a11*a21r*us+a11*a21s*ur-a21*a12s*vr-a21*a12r*vs-a21*a12rs*u(i1,i2,i3,ey)-a21*a11s*ur-a21*a11r*us-a21*a11rs*u(i1,i2,i3,ex)-a21*a21ss*u(i1,i2,i3,ex)-a21*a22ss*u(i1,i2,i3,ey)-2*a21*a22s*vs-2*a21*a21s*us+a11**2*urr-a21**2*uss)/(-a11*a22+a21*a12)
   


    urrrr=urrrr2(i1,i2,i3,ex)
    ussss=ussss2(i1,i2,i3,ex)

    vrrrr=urrrr2(i1,i2,i3,ey)
    vssss=ussss2(i1,i2,i3,ey)

    #If #FORCING == "twilightZone"
     if( debug.gt.0 )then
     write(*,'("ghostValuesOutsideCorners2d: i1,i2,i3=",3i3," urs,-vss=",2f9.3," vrs,-urr=",2f9.3," dra,dsa=",2e10.2)') \
                 i1,i2,i3,urs,-vss,vrs,-urr,dra,dsa
     write(*,'("ghostValuesOutsideCorners2d:  urrr,usss=",2e10.2,", urrrr,ussss=",4e10.2)') \
                 urrr2(i1,i2,i3,ex),usss2(i1,i2,i3,ex),urrrr,ussss,vrrrr,vssss
     end if
    #End

    ! **** finish these ****
    urrss=0.  ! from equation   uxxxx + uxxyy = uttxx  [ u(x,0)=0 => uxxxx(x,0)=0 uxxtt(x,0)=0 ]
    vrrss=0.  ! from equation 

    urrrs=-vrrss  ! from div
    ursss=-vssss  ! from div

    vrrrs=-urrrr  ! from div
    vrsss=-urrss  ! from div


    extrapCornerOrder4(  is1,  is2,   dra,   dsa) ! ****************** CHECK ********************
    extrapCornerOrder4(2*is1,  is2,2.*dra,   dsa) 
    extrapCornerOrder4(  is1,2*is2,   dra,2.*dsa)
    extrapCornerOrder4(2*is1,2*is2,2.*dra,2.*dsa)

    setCornersToExact=.false.

    ! check errors
    #If #FORCING == "twilightZone"
      OGF2D(i1-is1,i2-is2,i3,t, uv0(0),uv0(1),uv0(2))
      if( debug.gt.0 ) write(*,'(" ghostValuesOutsideCorners2d: i1-is1,i2-is2=",2i4," ex,err,ey,err=",4e10.2)') i1-is1,i2-is2,\
                 u(i1-is1,i2-is2,i3,ex),u(i1-is1,i2-is2,i3,ex)-uv0(0),u(i1-is1,i2-is2,i3,ey),u(i1-is1,i2-is2,i3,ey)-uv0(1)    
      if( setCornersToExact )then
        u(i1-is1,i2-is2,i3,ex)=uv0(0)
        u(i1-is1,i2-is2,i3,ey)=uv0(1)
      end if

      OGF2D(i1-2*is1,i2-is2,i3,t, uv0(0),uv0(1),uv0(2))
      if( debug.gt.0 ) write(*,'(" ghostValuesOutsideCorners2d: i1-2*is1,i2-is2=",2i4," ex,err,ey,err=",4e10.2)') i1-2*is1,i2-is2,\
                 u(i1-2*is1,i2-is2,i3,ex),u(i1-2*is1,i2-is2,i3,ex)-uv0(0),u(i1-2*is1,i2-is2,i3,ey),u(i1-2*is1,i2-is2,i3,ey)-uv0(1)    
      if( setCornersToExact )then
        u(i1-2*is1,i2-is2,i3,ex)=uv0(0)
        u(i1-2*is1,i2-is2,i3,ey)=uv0(1)
      end if

      OGF2D(i1-is1,i2-2*is2,i3,t, uv0(0),uv0(1),uv0(2))
      if( debug.gt.0 ) write(*,'(" ghostValuesOutsideCorners2d: i1-is1,i2-2*is2=",2i4," ex,err,ey,err=",4e10.2)') i1-is1,i2-2*is2,\
                 u(i1-is1,i2-2*is2,i3,ex),u(i1-is1,i2-2*is2,i3,ex)-uv0(0),u(i1-is1,i2-2*is2,i3,ey),u(i1-is1,i2-2*is2,i3,ey)-uv0(1)    
      if( setCornersToExact )then
        u(i1-is1,i2-2*is2,i3,ex)=uv0(0)
        u(i1-is1,i2-2*is2,i3,ey)=uv0(1)
      end if

      OGF2D(i1-2*is1,i2-2*is2,i3,t, uv0(0),uv0(1),uv0(2))
      if( debug.gt.0 ) write(*,'(" ghostValuesOutsideCorners2d: i1-2*is1,i2-2*is2=",2i4," ex,err,ey,err=",4e10.2)') i1-2*is1,i2-2*is2,\
          u(i1-2*is1,i2-2*is2,i3,ex),u(i1-2*is1,i2-2*is2,i3,ex)-uv0(0),u(i1-2*is1,i2-2*is2,i3,ey),u(i1-2*is1,i2-2*is2,i3,ey)-uv0(1)    
      if( setCornersToExact )then
        u(i1-2*is1,i2-2*is2,i3,ex)=uv0(0)
        u(i1-2*is1,i2-2*is2,i3,ey)=uv0(1)
      end if


    #End

    ! --- Now do Hz ---

    ur = ur4(i1,i2,i3,hz)
    us = us4(i1,i2,i3,hz)

    urrr=urrr2(i1,i2,i3,hz)
    usss=usss2(i1,i2,i3,hz)

    urrs=0. !  (from ur(0,s)=0 and us(r,0)=0)  ! ****************** fix for TZ
    urss=0. !  (from ur(0,s)=0 and us(r,0)=0)

    #If #FORCING == "twilightZone"
      OGF2D(i1-1,i2-1,i3,t, uvmm(0),uvmm(1),uvmm(2))
      OGF2D(i1  ,i2-1,i3,t, uvzm(0),uvzm(1),uvzm(2))
      OGF2D(i1+1,i2-1,i3,t, uvpm(0),uvpm(1),uvpm(2))

      OGF2D(i1-1,i2    ,i3,t, uvmz(0),uvmz(1),uvmz(2))
      OGF2D(i1  ,i2    ,i3,t, uvzz(0),uvzz(1),uvzz(2))
      OGF2D(i1+1,i2    ,i3,t, uvpz(0),uvpz(1),uvpz(2))

      OGF2D(i1-1,i2+1,i3,t, uvmp(0),uvmp(1),uvmp(2))
      OGF2D(i1  ,i2+1,i3,t, uvzp(0),uvzp(1),uvzp(2))
      OGF2D(i1+1,i2+1,i3,t, uvpp(0),uvpp(1),uvpp(2))

      urrs=( (uvpp(2)-2.*uvzp(2)+uvmp(2))-(uvpm(2)-2.*uvzm(2)+uvmm(2)) )/(2.*dr(1)*dra**2)
      urss=( (uvpp(2)-2.*uvpz(2)+uvpm(2))-(uvmp(2)-2.*uvmz(2)+uvmm(2)) )/(2.*dr(0)*dsa**2)
      ! stop 6666
    #End
!   write(*,'(" ghostValuesOutsideCorners2d: i1,i2,is1,is2=",4i4," dra,dsa=",2e10.2," urrr,usss,urrs,urss=",4e10.2)')\
 i1,i2,is1,is2,dra,dsa,urrr,usss,urrs,urss
!    urrr=0.
!    usss=0.
!    urrs=0.
!   urss=0.

    extrapCornerHzOrder4(  is1,  is2,   dra,   dsa)
    extrapCornerHzOrder4(2*is1,  is2,2.*dra,   dsa)
    extrapCornerHzOrder4(  is1,2*is2,   dra,2.*dsa)
    extrapCornerHzOrder4(2*is1,2*is2,2.*dra,2.*dsa)

    #If #FORCING == "twilightZone"
!      setCornersToExact=.true.
!      if( setCornersToExact )then
!        OGF2D(i1-is1,i2-is2,i3,t, uv0(0),uv0(1),uv0(2))
!        write(*,'(" ghostValuesOutsideCorners2d: i1-is1,i2-is2=",2i4," hz,err=",4e10.2)') i1-is1,i2-is2,\
!                 u(i1-is1,i2-is2,i3,hz),u(i1-is1,i2-is2,i3,hz)-uv0(2)
!        u(i1-is1,i2-is2,i3,hz)=uv0(2)
!        OGF2D(i1-2*is1,i2-is2,i3,t, uv0(0),uv0(1),uv0(2))
!        u(i1-2*is1,i2-is2,i3,hz)=uv0(2)
!        write(*,'(" ghostValuesOutsideCorners2d: i1-2*is1,i2-is2=",2i4," hz,err=",4e10.2)') i1-2*is1,i2-is2,\
!                 u(i1-2*is1,i2-is2,i3,hz),u(i1-2*is1,i2-is2,i3,hz)-uv0(2)
!        OGF2D(i1-is1,i2-2*is2,i3,t, uv0(0),uv0(1),uv0(2))
!        write(*,'(" ghostValuesOutsideCorners2d: i1-is1,i2-2*is2=",2i4," hz,err=",4e10.2)') i1-is1,i2-2*is2,\
!                 u(i1-is1,i2-2*is2,i3,hz),u(i1-is1,i2-2*is2,i3,hz)-uv0(2)
!        u(i1-is1,i2-2*is2,i3,hz)=uv0(2)
!        OGF2D(i1-2*is1,i2-2*is2,i3,t, uv0(0),uv0(1),uv0(2))
!        write(*,'(" ghostValuesOutsideCorners2d: i1-2*is1,i2-2*is2=",2i4," hz,err=",4e10.2)') i1-2*is1,i2-2*is2,\
!                 u(i1-2*is1,i2-2*is2,i3,hz),u(i1-2*is1,i2-2*is2,i3,hz)-uv0(2)
!        u(i1-2*is1,i2-2*is2,i3,hz)=uv0(2)
!     end if
    #End
  #Else 
    stop 332255
  #End 

 #Elif #GRIDTYPE == "rectangular" 

  #If #ORDER == "2" 
    urr=uxx22r(i1,i2,i3,ex)  ! note: this is uxx
    vrr=uxx22r(i1,i2,i3,ey)

    uss=uyy22r(i1,i2,i3,ex)
    vss=uyy22r(i1,i2,i3,ey)

    urs=-vss  ! uxy=-vyy
    vrs=-urr
    extrapCorner(is1,is2,is1*dxa,is2*dya)

    ur = ux22r(i1,i2,i3,hz)
    us = uy22r(i1,i2,i3,hz)


    u(i1-  is1,i2-  is2,i3,hz)= u(i1+  is1,i2+  is2,i3,hz) - 2.*(is1*dxa*ur+is2*dya*us)

  #Elif #ORDER == "4" 

    urr=uxx42r(i1,i2,i3,ex)  ! note: this is uxx
    vrr=uxx42r(i1,i2,i3,ey)

    uss=uyy42r(i1,i2,i3,ex)
    vss=uyy42r(i1,i2,i3,ey)

    urs=-vss  ! uxy=-vyy
    vrs=-urr

    urrrr=uxxxx22r(i1,i2,i3,ex)
    ussss=uyyyy22r(i1,i2,i3,ex)

    vrrrr=uxxxx22r(i1,i2,i3,ey)
    vssss=uyyyy22r(i1,i2,i3,ey)

    urrss=0.  ! from equation   uxxxx + uxxyy = uttxx  [ u(x,0)=0 => uxxxx(x,0)=0 uxxtt(x,0)=0 ]
    vrrss=0.  ! from equation 

    urrrs=-vrrss  ! from div
    ursss=-vssss  ! from div

    vrrrs=-urrrr  ! from div
    vrsss=-urrss  ! from div

    extrapCornerOrder4(  is1,  is2,   is1*dxa,   is2*dya)
    extrapCornerOrder4(2*is1,  is2,2.*is1*dxa,   is2*dya)   
    extrapCornerOrder4(  is1,2*is2,   is1*dxa,2.*is2*dya)
    extrapCornerOrder4(2*is1,2*is2,2.*is1*dxa,2.*is2*dya)

    ! Now do Hz

    ur = ux42r(i1,i2,i3,hz)
    us = uy42r(i1,i2,i3,hz)

    urrr=uxxx22r(i1,i2,i3,hz)   ! 2nd order should be good enough
    usss=uyyy22r(i1,i2,i3,hz)

    urrs=0. !  (from ux(0,s)=0 and uy(r,0)=0)   ! ****************** fix for TZ
    urss=0. !  (from ux(0,s)=0 and uy(r,0)=0)
    #If #FORCING == "twilightZone"
      ! just set uxxy and uxyy to the exact values.
      call ogDeriv(ep, 0, 2,1,0, xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,hz, urrs)
      call ogDeriv(ep, 0, 1,2,0, xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,hz, urss)

    #End

    extrapCornerHzOrder4(  is1,  is2,   is1*dxa,   is2*dya)
    extrapCornerHzOrder4(2*is1,  is2,2.*is1*dxa,   is2*dya)
    extrapCornerHzOrder4(  is1,2*is2,   is1*dxa,2.*is2*dya)
    extrapCornerHzOrder4(2*is1,2*is2,2.*is1*dxa,2.*is2*dya)


  #Else 
    stop 3399
  #End 

 #Else
   stop 8383
 #End

#endMacro



c =================================================================================
c  Assign extended boundary points (from bc4c.maple)
c
c  Solve for the 8 unknowns
c         u(i1-1,i2,i3,ex), u(i1-2,i2,i3,ex), u(i1,i2-1,i3,ex), u(i1,i2-2,ex)
c         u(i1-1,i2,i3,ey), u(i1-2,i2,i3,ey), u(i1,i2-1,i3,ey), u(i1,i2-2,ey)
c   
c Use:
c   1) tangential components are zero (4 equations)
c   2) Use the equations on the corner  (2 equations)
c   3) Extrapolate normal components at (i1-2) and (i2-2)  (2 equations)
c
c NOTE: Call this macro with axis=0 and axisp1=1 for all sides and dra=dr(0)*is1, dsa=dr(1)*is2
c =================================================================================
#beginMacro assignExtendedBoundaries2dOrder4(FORCING,is1,is2)

 a11m2 =rsxy(i1-2*is1,i2    ,i3,0,0)
 a12m2 =rsxy(i1-2*is1,i2    ,i3,0,1)  

 a11m1 =rsxy(i1-is1,i2    ,i3,0,0)
 a12m1 =rsxy(i1-is1,i2    ,i3,0,1)  
                              
 a11   =rsxy(i1    ,i2    ,i3,0,0)
 a12   =rsxy(i1    ,i2    ,i3,0,1)  

 a21   =rsxy(i1    ,i2    ,i3,1,0)
 a22   =rsxy(i1    ,i2    ,i3,1,1)  
                              
 a21zm1 =rsxy(i1   ,i2-is2,i3,1,0)
 a22zm1 =rsxy(i1   ,i2-is2,i3,1,1)  

 a21zm2 =rsxy(i1   ,i2-2*is2,i3,1,0)
 a22zm2 =rsxy(i1   ,i2-2*is2,i3,1,1)  

 c11 = C11(i1,i2,i3)
 c22 = C22(i1,i2,i3)
 c1  = C1Order4(i1,i2,i3)
 c2  = C2Order4(i1,i2,i3)

 c11r = (8.*(C11(i1+  is1,i2,i3)-C11(i1-  is1,i2,i3))   \
           -(C11(i1+2*is1,i2,i3)-C11(i1-2*is1,i2,i3))   )/(12.*dra)
 c22r = (8.*(C22(i1+  is1,i2,i3)-C22(i1-  is1,i2,i3))   \
           -(C22(i1+2*is1,i2,i3)-C22(i1-2*is1,i2,i3))   )/(12.*dra)
 
 c11s = (8.*(C11(i1,i2+  is2,i3)-C11(i1,i2-  is2,i3))   \
           -(C11(i1,i2+2*is2,i3)-C11(i1,i2-2*is2,i3))   )/(12.*dsa)
 c22s = (8.*(C22(i1,i2+  is2,i3)-C22(i1,i2-  is2,i3))   \
           -(C22(i1,i2+2*is2,i3)-C22(i1,i2-2*is2,i3))   )/(12.*dsa)
 

 !  Solve for Hz on extended boundaries from:  
 !          wr=fw1  
 !          ws=fw2  
 !          c11*wrrr+(c1+c11r)*wrr + c22r*wss=fw3, (i.e. (Lw).r=0 )
 !          c22*wsss+(c2+c22s)*wss + c11s*wrr=fw4, (i.e. (Lw).s=0 )
 fw1=0.
 fw2=0.
 fw3=0.
 fw4=0.

 #If #FORCING == "twilightZone"

   OGF2D(i1-is1,i2    ,i3,t, uv0(0),uv0(1),uv0(2))
   tdu10=a11m1*uv0(0)+a12m1*uv0(1)
   OGF2D(i1    ,i2-is2,i3,t, uv0(0),uv0(1),uv0(2))
   tdu01=a21zm1*uv0(0)+a22zm1*uv0(1)

   OGF2D(i1-2*is1,i2    ,i3,t, uv0(0),uv0(1),uv0(2))
   tdu20=a11m2*uv0(0)+a12m2*uv0(1)
   OGF2D(i1    ,i2-2*is2,i3,t, uv0(0),uv0(1),uv0(2))
   tdu02=a21zm2*uv0(0)+a22zm2*uv0(1)

   ! For TZ: utt0 = utt - ett + Lap(e)
   call ogDeriv(ep, 0, 2,0,0, xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,ex, urr)
   call ogDeriv(ep, 0, 0,2,0, xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,ex, uss)
   utt00=urr+uss
  
   call ogDeriv(ep, 0, 2,0,0, xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,ey, vrr)
   call ogDeriv(ep, 0, 0,2,0, xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,ey, vss)
   vtt00=vrr+vss
  
   ! Now compute forcing for Hz
   OGF2D(i1      ,i2,i3,t, uv0(0),uv0(1),uv0(2))
   OGF2D(i1-  is1,i2,i3,t, uvm(0),uvm(1),uvm(2))
   OGF2D(i1+  is1,i2,i3,t, uvp(0),uvp(1),uvp(2))
   OGF2D(i1-2*is1,i2,i3,t, uvm2(0),uvm2(1),uvm2(2))
   OGF2D(i1+2*is1,i2,i3,t, uvp2(0),uvp2(1),uvp2(2))

   wr = (8.*(uvp(2)-uvm(2))-(uvp2(2)-uvm2(2)))/(12.*dra) 
   wrr=(uvp(2)-2.*uv0(2)+uvm(2))/(dra**2) 
   wrrr=(uvp2(2)-2.*(uvp(2)-uvm(2))-uvm2(2))/(2.*dra**3)
 

   OGF2D(i1,i2      ,i3,t, uv0(0),uv0(1),uv0(2))
   OGF2D(i1,i2-  is2,i3,t, uvm(0),uvm(1),uvm(2))
   OGF2D(i1,i2+  is2,i3,t, uvp(0),uvp(1),uvp(2))
   OGF2D(i1,i2-2*is2,i3,t, uvm2(0),uvm2(1),uvm2(2))
   OGF2D(i1,i2+2*is2,i3,t, uvp2(0),uvp2(1),uvp2(2))

   ws = (8.*(uvp(2)-uvm(2))-(uvp2(2)-uvm2(2)))/(12.*dsa) 
   wss=(uvp(2)-2.*uv0(2)+uvm(2))/(dsa**2) 
   wsss=(uvp2(2)-2.*(uvp(2)-uvm(2))-uvm2(2))/(2.*dsa**3)

   fw1=wr
   fw2=ws
   fw3= c11*wrrr+(c1+c11r)*wrr +c22r*wss
   fw4= c22*wsss+(c2+c22s)*wss +c11s*wrr

 #Else
   tdu10=0.  ! e1 := (a11*u+a12*v)(i1-1,i2,i3) - tdu10:
   tdu01=0.  ! e2 := (a21*u+a22*v)(i1,i2-1,i3) - tdu01:
   tdu20=0.
   tdu02=0.
   ! Lua := cu20*u(i1-2,i2,i3) + cu10*u(i1-1,i2,i3) + cu02*u(i1,i2-2,i3) + cu01*u(i1,i2-1,i3) + gLu - utt00:
   utt00=0.  ! u.tt
   vtt00=0.  ! u.tt

   
 #End

  ! uLap = uLaplacian42(i1,i2,i3,ex)
  ! vLap = uLaplacian42(i1,i2,i3,ey)
  ! Drop the cross term for now -- this should be fixed for non-orthogonal grids ---
  uLap = c11*urr4(i1,i2,i3,ex)+c22*uss4(i1,i2,i3,ex)+c1*ur4(i1,i2,i3,ex)+c2*us4(i1,i2,i3,ex)
  vLap = c11*urr4(i1,i2,i3,ey)+c22*uss4(i1,i2,i3,ey)+c1*ur4(i1,i2,i3,ey)+c2*us4(i1,i2,i3,ey)

! The next file is from bc4c.maple
#Include "bcExtended4Maxwell.h"

! The next file is from bc4c.maple
#Include "bcHzExtended4Maxwell.h"

 #If #FORCING == "twilightZone"
 if( debug.gt.0 )then
  write(*,'(\,"-------------")') 
  write(*,'(" bcOpt: extended4 i1,i2=",2i4," is1,is2=",2i3," x,y=",2f8.4,"dra,dsa=",2e8.2)') i1,i2,is1,is2,\
          xy(i1,i2,i3,0),xy(i1,i2,i3,1),dra,dsa
  write(*,'(" bcOpt: extended4 det,c11,c22,c1,c2=",5e10.2, ", c1Order2=",e10.2)') det,c11,c22,c1,c2,C1Order2(i1,i2,i3)

  write(*,'("      : Lu-utt=",e10.2," Lv-vtt=",e10.2)') uLaplacian42(i1,i2,i3,ex)-utt00,uLaplacian42(i1,i2,i3,ey)-vtt00


  ! write(*,'("   g1a,g2a,cu20,cu02,cu10,cu01=",6e16.8)') g1a,g2a,cu20,cu02,cu10,cu01
  ! write(*,'("   cv20,cv02,cv10,cv01=",6e18.10)') cv20,cv02,cv10,cv01
  ! write(*,'("   gLu,gLv,uLaplacian42(ex,ey)=",6e16.8)') gLu,gLv,uLaplacian42(i1,i2,i3,ex),uLaplacian42(i1,i2,i3,ey)

  OGF2D(i1-is1,i2    ,i3,t, uv0(0),uv0(1),uv0(2))
  write(*,'(" bcOpt: extended4 i1-is1,i2=",2i4," ex,err,ey,err=",4e10.2)') i1-is1,i2,\
               u(i1-is1,i2,i3,ex),u(i1-is1,i2,i3,ex)-uv0(0),u(i1-is1,i2,i3,ey),u(i1-is1,i2,i3,ey)-uv0(1)

  OGF2D(i1,i2-is2    ,i3,t, uv0(0),uv0(1),uv0(2))
  write(*,'(" bcOpt: extended4 i1,i2-is2=",2i4," ex,err,ey,err=",4e10.2)') i1,i2-is2,\
               u(i1,i2-is2,i3,ex),u(i1,i2-is2,i3,ex)-uv0(0),u(i1,i2-is2,i3,ey),u(i1,i2-is2,i3,ey)-uv0(1)

  OGF2D(i1-2*is1,i2    ,i3,t, uv0(0),uv0(1),uv0(2))
  write(*,'(" bcOpt: extended4 i1-2*is1,i2=",2i4," ex,err,ey,err=",4e10.2)') i1-2*is1,i2,\
               u(i1-2*is1,i2,i3,ex),u(i1-2*is1,i2,i3,ex)-uv0(0),u(i1-2*is1,i2,i3,ey),u(i1-2*is1,i2,i3,ey)-uv0(1)

  OGF2D(i1,i2-2*is2    ,i3,t, uv0(0),uv0(1),uv0(2))
  write(*,'(" bcOpt: extended4 i1,i2-2*is2=",2i4," ex,err,ey,err=",4e10.2)') i1,i2-2*is2,\
               u(i1,i2-2*is2,i3,ex),u(i1,i2-2*is2,i3,ex)-uv0(0),u(i1,i2-2*is2,i3,ey),u(i1,i2-2*is2,i3,ey)-uv0(1)
  write(*,'("-------------",/)') 
 end if
 #End

#endMacro


c ==========================================================================
c  Define some metric (and equation coefficients) terms and their derivatives
c
c Here are the derivatives we can compute directly based on (is1,is2,i3) (js1,js2,js3) (ks1,ks2,k3)
c ==========================================================================
#beginMacro defineCornerEdgeMetricDerivatives1(ORDER)

 ! precompute the inverse of the jacobian, used in macros AmnD3J

 i10=i1  ! used by jac3di in macros
 i20=i2
 i30=i3

 do m3=-numberOfGhostPoints,numberOfGhostPoints
 do m2=-numberOfGhostPoints,numberOfGhostPoints
 do m1=-numberOfGhostPoints,numberOfGhostPoints
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

#If #ORDER == "2"
 ! ************ Order 2 ******************

 a11r = DR($A11D3J)
 a12r = DR($A12D3J)
 a13r = DR($A13D3J)
         
 a21r = DR($A21D3J)
 a22r = DR($A22D3J)
 a23r = DR($A23D3J)
         
 a31r = DR($A31D3J)
 a32r = DR($A32D3J)
 a33r = DR($A33D3J)


 a11rr = DRR($A11D3J)
 a12rr = DRR($A12D3J)
 a13rr = DRR($A13D3J)
          
 a21rr = DRR($A21D3J)
 a22rr = DRR($A22D3J)
 a23rr = DRR($A23D3J)
          
 a31rr = DRR($A31D3J)
 a32rr = DRR($A32D3J)
 a33rr = DRR($A33D3J)

 a11s = DS($A11D3J)
 a12s = DS($A12D3J)
 a13s = DS($A13D3J)
         
 a21s = DS($A21D3J)
 a22s = DS($A22D3J)
 a23s = DS($A23D3J)
         
 a31s = DS($A31D3J)
 a32s = DS($A32D3J)
 a33s = DS($A33D3J)

 a11ss = DSS($A11D3J)
 a12ss = DSS($A12D3J)
 a13ss = DSS($A13D3J)
          
 a21ss = DSS($A21D3J)
 a22ss = DSS($A22D3J)
 a23ss = DSS($A23D3J)
          
 a31ss = DSS($A31D3J)
 a32ss = DSS($A32D3J)
 a33ss = DSS($A33D3J)

 a11t = DT($A11D3J)
 a12t = DT($A12D3J)
 a13t = DT($A13D3J)
         
 a21t = DT($A21D3J)
 a22t = DT($A22D3J)
 a23t = DT($A23D3J)
         
 a31t = DT($A31D3J)
 a32t = DT($A32D3J)
 a33t = DT($A33D3J)


 c11 = C11D3(i1,i2,i3)
 c22 = C22D3(i1,i2,i3)
 c33 = C33D3(i1,i2,i3)

 c1 = C1D3Order2(i1,i2,i3)
 c2 = C2D3Order2(i1,i2,i3)
 c3 = C3D3Order2(i1,i2,i3)

 c11r = (8.*(C11D3(i1+  is1,i2+  is2,i3+  is3)-C11D3(i1-  is1,i2-  is2,i3-  is3))   \
           -(C11D3(i1+2*is1,i2+2*is2,i3+2*is3)-C11D3(i1-2*is1,i2-2*is2,i3-2*is3))   )/(12.*dra)
 c22r = (8.*(C22D3(i1+  is1,i2+  is2,i3+  is3)-C22D3(i1-  is1,i2-  is2,i3-  is3))   \
           -(C22D3(i1+2*is1,i2+2*is2,i3+2*is3)-C22D3(i1-2*is1,i2-2*is2,i3-2*is3))   )/(12.*dra)
 c33r = (8.*(C33D3(i1+  is1,i2+  is2,i3+  is3)-C33D3(i1-  is1,i2-  is2,i3-  is3))   \
           -(C33D3(i1+2*is1,i2+2*is2,i3+2*is3)-C33D3(i1-2*is1,i2-2*is2,i3-2*is3))   )/(12.*dra)

 c11s = (8.*(C11D3(i1+  js1,i2+  js2,i3+  js3)-C11D3(i1-  js1,i2-  js2,i3-  js3))   \
           -(C11D3(i1+2*js1,i2+2*js2,i3+2*js3)-C11D3(i1-2*js1,i2-2*js2,i3-2*js3))   )/(12.*dsa)
 c22s = (8.*(C22D3(i1+  js1,i2+  js2,i3+  js3)-C22D3(i1-  js1,i2-  js2,i3-  js3))   \
           -(C22D3(i1+2*js1,i2+2*js2,i3+2*js3)-C22D3(i1-2*js1,i2-2*js2,i3-2*js3))   )/(12.*dsa)
 c33s = (8.*(C33D3(i1+  js1,i2+  js2,i3+  js3)-C33D3(i1-  js1,i2-  js2,i3-  js3))   \
           -(C33D3(i1+2*js1,i2+2*js2,i3+2*js3)-C33D3(i1-2*js1,i2-2*js2,i3-2*js3))   )/(12.*dsa)

 if( axis.eq.0 )then
   c1r = C1D3r2(i1,i2,i3)
   c2r = C2D3r2(i1,i2,i3)
   c3r = C3D3r2(i1,i2,i3)
   c1s = C1D3s2(i1,i2,i3)
   c2s = C2D3s2(i1,i2,i3)
   c3s = C3D3s2(i1,i2,i3)
 else if( axis.eq.1 )then
   c1r = C1D3s2(i1,i2,i3)
   c2r = C2D3s2(i1,i2,i3)
   c3r = C3D3s2(i1,i2,i3)
   c1s = C1D3t2(i1,i2,i3)
   c2s = C2D3t2(i1,i2,i3)
   c3s = C3D3t2(i1,i2,i3)
 else 
   c1r = C1D3t2(i1,i2,i3)
   c2r = C2D3t2(i1,i2,i3)
   c3r = C3D3t2(i1,i2,i3)
   c1s = C1D3r2(i1,i2,i3)
   c2s = C2D3r2(i1,i2,i3)
   c3s = C3D3r2(i1,i2,i3)
 end if

#Elif #ORDER == "4"
 ! ************ Order 4 ******************

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

 a11t = DT4($A11D3J)
 a12t = DT4($A12D3J)
 a13t = DT4($A13D3J)
         
 a21t = DT4($A21D3J)
 a22t = DT4($A22D3J)
 a23t = DT4($A23D3J)
         
 a31t = DT4($A31D3J)
 a32t = DT4($A32D3J)
 a33t = DT4($A33D3J)


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

 c11s = (8.*(C11D3(i1+  js1,i2+  js2,i3+  js3)-C11D3(i1-  js1,i2-  js2,i3-  js3))   \
           -(C11D3(i1+2*js1,i2+2*js2,i3+2*js3)-C11D3(i1-2*js1,i2-2*js2,i3-2*js3))   )/(12.*dsa)
 c22s = (8.*(C22D3(i1+  js1,i2+  js2,i3+  js3)-C22D3(i1-  js1,i2-  js2,i3-  js3))   \
           -(C22D3(i1+2*js1,i2+2*js2,i3+2*js3)-C22D3(i1-2*js1,i2-2*js2,i3-2*js3))   )/(12.*dsa)
 c33s = (8.*(C33D3(i1+  js1,i2+  js2,i3+  js3)-C33D3(i1-  js1,i2-  js2,i3-  js3))   \
           -(C33D3(i1+2*js1,i2+2*js2,i3+2*js3)-C33D3(i1-2*js1,i2-2*js2,i3-2*js3))   )/(12.*dsa)

 if( axis.eq.0 )then
   c1r = C1D3r4(i1,i2,i3)
   c2r = C2D3r4(i1,i2,i3)
   c3r = C3D3r4(i1,i2,i3)
   c1s = C1D3s4(i1,i2,i3)
   c2s = C2D3s4(i1,i2,i3)
   c3s = C3D3s4(i1,i2,i3)
 else if( axis.eq.1 )then
   c1r = C1D3s4(i1,i2,i3)
   c2r = C2D3s4(i1,i2,i3)
   c3r = C3D3s4(i1,i2,i3)
   c1s = C1D3t4(i1,i2,i3)
   c2s = C2D3t4(i1,i2,i3)
   c3s = C3D3t4(i1,i2,i3)
 else 
   c1r = C1D3t4(i1,i2,i3)
   c2r = C2D3t4(i1,i2,i3)
   c3r = C3D3t4(i1,i2,i3)
   c1s = C1D3r4(i1,i2,i3)
   c2s = C2D3r4(i1,i2,i3)
   c3s = C3D3r4(i1,i2,i3)
 end if

#Else
   stop 2863
#End

#endMacro

c ==========================================================================
c  Define some metric (and equation coefficients) terms and their derivatives
c
c Here are the derivatives that we need to use difference code for each values of axis
c ==========================================================================
#beginMacro defineCornerEdgeMetricDerivatives2(DArs4,DArt4,DAst4)

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


#endMacro

c================================================================================
c Compute tangential derivatives
c
c Here are the derivatives we can compute directly based on (is1,is2,i3) (js1,js2,js3) (ks1,ks2,k3)
c================================================================================
#beginMacro getCornerEdgeDerivatives1(ORDER)
#If #ORDER == "2"
 ! ************ Order 2 ******************
 ur=UR2(ex)
 urr=URR2(ex)

 vr=UR2(ey)
 vrr=URR2(ey)

 wr=UR2(ez)
 wrr=URR2(ez)

 us=US2(ex)
 uss=USS2(ex)

 vs=US2(ey)
 vss=USS2(ey)

 ws=US2(ez)
 wss=USS2(ez)

 ut=UT2(ex)
 utt=UTT2(ex)

 vt=UT2(ey)
 vtt=UTT2(ey)

 wt=UT2(ez)
 wtt=UTT2(ez)

#Elif #ORDER == "4"
 ! ************ Order 4 ******************

 ur=UR4(ex)
 urr=URR4(ex)
 urrr=URRR2(ex)

 vr=UR4(ey)
 vrr=URR4(ey)
 vrrr=URRR2(ey)

 wr=UR4(ez)
 wrr=URR4(ez)
 wrrr=URRR2(ez)

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

#Else
   stop 2877
#End

#endMacro

c ======================================================================================
c Here are the derivatives that we need to use difference code for each values of axis
c ======================================================================================
#beginMacro getCornerEdgeDerivatives2(VRT,VST,VRTT,VSTT)

 urt=VRT(i1,i2,i3,ex)
 ust=VST(i1,i2,i3,ex)
 urtt=VRTT(i1,i2,i3,ex)
 ustt=VSTT(i1,i2,i3,ex)

 vrt  =VRT(i1,i2,i3,ey)
 vst  =VST(i1,i2,i3,ey)
 vrtt=VRTT(i1,i2,i3,ey)
 vstt=VSTT(i1,i2,i3,ey)

 wrt  =VRT(i1,i2,i3,ez)
 wst  =VST(i1,i2,i3,ez)
 wrtt=VRTT(i1,i2,i3,ez)
 wstt=VSTT(i1,i2,i3,ez)

#endMacro


c Taylor series (designed for odd functions)
#beginMacro extrapEdgeCorners(cc,ks1,ks2,ks3,dr1,dr2,dr3,urr,uss,utt,urs,urt,ust)
  u(i1-ks1,i2-ks2,i3-ks3,cc)= 2.*u(i1,i2,i3,cc)-u(i1+ks1,i2+ks2,i3+ks3,cc) \
      + ( (dr1)**2*urr+(dr2)**2*uss+(dr3)**2*utt+2.*(dr1)*(dr2)*urs+2.*(dr1)*(dr3)*urt+2.*(dr2)*(dr3)*ust )
#endMacro




! ***************************************************************************
! ****************Assign Points Outside of Edges*****************************
!
!   GRIDTYPE: rectangular, curvilinear
!   ORDER: 2, 4, 6, ..
! ***************************************************************************
#beginMacro assignEdgeCorners(ORDER,GRIDTYPE,FORCING)

 do edgeDirection=0,2 ! direction parallel to the edge
! do edgeDirection=2,2 ! direction parallel to the edge
 do sidea=0,1
 do sideb=0,1
  if( edgeDirection.eq.0 )then
    side1=0
    side2=sidea
    side3=sideb
  else if( edgeDirection.eq.1 )then
    side1=sideb 
    side2=0
    side3=sidea
  else
    side1=sidea
    side2=sideb
    side3=0
  end if

 is1=1-2*(side1)
 is2=1-2*(side2)
 is3=1-2*(side3)
 if( edgeDirection.eq.2 )then
  is3=0
  n1a=gridIndexRange(side1,0)
  n1b=gridIndexRange(side1,0)
  n2a=gridIndexRange(side2,1)
  n2b=gridIndexRange(side2,1)
  n3a=gridIndexRange(0,2)
  n3b=gridIndexRange(1,2)
  bc1=boundaryCondition(side1,0)
  bc2=boundaryCondition(side2,1)
 else if( edgeDirection.eq.1 )then
  is2=0
  n1a=gridIndexRange(side1,0)
  n1b=gridIndexRange(side1,0)
  n2a=gridIndexRange(    0,1)
  n2b=gridIndexRange(    1,1)
  n3a=gridIndexRange(side3,2)
  n3b=gridIndexRange(side3,2)
  bc1=boundaryCondition(side1,0)
  bc2=boundaryCondition(side3,2)
 else 
  is1=0  
  n1a=gridIndexRange(    0,0)
  n1b=gridIndexRange(    1,0)
  n2a=gridIndexRange(side2,1)
  n2b=gridIndexRange(side2,1)
  n3a=gridIndexRange(side3,2)
  n3b=gridIndexRange(side3,2)
  bc1=boundaryCondition(side2,1)
  bc2=boundaryCondition(side3,2)
 end if


 #If #GRIDTYPE == "rectangular"

 ! *********************************************************
 ! ************* rectangular *******************************
 ! *********************************************************

 do m1=1,numberOfGhostPoints
 do m2=1,numberOfGhostPoints

  ! shift to ghost point "(m1,m2)"
  if( edgeDirection.eq.2 )then 
    js1=is1*m1  
    js2=is2*m2
    js3=0
  else if( edgeDirection.eq.1 )then 
    js1=is1*m1  
    js2=0
    js3=is3*m2
  else 
    js1=0
    js2=is2*m1
    js3=is3*m2
  end if 

  if( bc1.eq.perfectElectricalConductor .and.\
      bc2.eq.perfectElectricalConductor )then

   ! *********************************************************
   ! ************* PEC EDGE BC********************************
   ! *********************************************************

    do i3=n3a,n3b
    do i2=n2a,n2b
    do i1=n1a,n1b

     #If #FORCING == "twilightZone"
       OGF3D(i1,i2,i3,t,u0,v0,w0)
       OGF3D(i1-js1,i2-js2,i3-js3,t, um,vm,wm)
       OGF3D(i1+js1,i2+js2,i3+js3,t, up,vp,wp)
       g1=um-2.*u0+up
       g2=vm-2.*v0+vp
       g3=wm-2.*w0+wp
     #End
     u(i1-js1,i2-js2,i3-js3,ex)=2.*u(i1,i2,i3,ex)-u(i1+js1,i2+js2,i3+js3,ex) +g1
     u(i1-js1,i2-js2,i3-js3,ey)=2.*u(i1,i2,i3,ey)-u(i1+js1,i2+js2,i3+js3,ey) +g2
     u(i1-js1,i2-js2,i3-js3,ez)=2.*u(i1,i2,i3,ez)-u(i1+js1,i2+js2,i3+js3,ez) +g3

    end do ! end do i1
    end do ! end do i2
    end do ! end do i3

  else if( bc1.eq.dirichlet .or. bc2.eq.dirichlet )then

    ! This is a dirichlet BC 

    do i3=n3a,n3b
    do i2=n2a,n2b
    do i1=n1a,n1b

     #If #FORCING == "twilightZone"
       OGF3D(i1-js1,i2-js2,i3-js3,t, g1,g2,g3)
     #End
     u(i1-js1,i2-js2,i3-js3,ex)=g1
     u(i1-js1,i2-js2,i3-js3,ey)=g2
     u(i1-js1,i2-js2,i3-js3,ez)=g3

    end do ! end do i1
    end do ! end do i2
    end do ! end do i3


  else if( bc1.le.0 .or. bc2.le.0 )then
    ! periodic or interpolation -- nothing to do

  else if( bc1.eq.planeWaveBoundaryCondition .or.\
           bc2.eq.planeWaveBoundaryCondition .or. \
           bc1.eq.symmetryBoundaryCondition .or. \
           bc2.eq.symmetryBoundaryCondition .or. \
           (bc1.ge.abcEM2 .and. bc1.le.lastBC) .or. \
           (bc2.ge.abcEM2 .and. bc2.le.lastBC) )then
     ! do nothing
  else
    write(*,'("ERROR: unknown boundary conditions bc1,bc2=",2i3)') bc1,bc2
    ! unknown boundary conditions
    stop 8866
  end if

 end do ! end do m1
 end do ! end do m2

 #Elif #GRIDTYPE == "curvilinear"
 ! ********************************************************
 ! ********** curvilinear *********************************
 ! ********************************************************

  ls1=is1  ! save for extrapolation
  ls2=is2
  ls3=is3

  is1=0
  is2=0
  is3=0
  js1=0
  js2=0
  js3=0
  ks1=0
  ks2=0
  ks3=0
  if( edgeDirection.eq.0 )then
    axis=1
    axisp1=2
    axisp2=0

    side1=0
    side2=sidea
    side3=sideb

    is2=1-2*side2  ! normal direction 1
    js3=1-2*side3  ! normal direction 2
    ks1=1          ! tangential direction

  else if( edgeDirection.eq.1 )then
    axis=2
    axisp1=0
    axisp2=1

    side1=sideb 
    side2=0
    side3=sidea

    is3=1-2*side3  ! normal direction 1
    js1=1-2*side1  ! normal direction 2
    ks2=1          ! tangential direction

  else
    axis=0
    axisp1=1
    axisp2=2

    side1=sidea
    side2=sideb
    side3=0

    is1=1-2*side1  ! normal direction 1
    js2=1-2*side2  ! normal direction 2
    ks3=1          ! tangential direction
  end if

  dra=dr(axis  )*(1-2*sidea)
  dsa=dr(axisp1)*(1-2*sideb) 
  dta=dr(axisp2)

  if( bc1.eq.perfectElectricalConductor .and.\
      bc2.eq.perfectElectricalConductor )then

   ! *********************************************************
   ! ************* PEC EDGE BC********************************
   ! *********************************************************

    if( debug.gt.0 )then
      write(*,'(/," corner-edge-ORDER:Start edge=",i1," side1,side2,side3=",3i2," is=",3i3," js=",3i3," ks=",3i3)') \
            edgeDirection,side1,side2,side3,is1,is2,is3,js1,js2,js3,ks1,ks2,ks3
      write(*,'("   dra,dsa,dta=",3f8.5)') dra,dsa,dta
    end if
    do i3=n3a,n3b
    do i2=n2a,n2b
    do i1=n1a,n1b

     defineCornerEdgeMetricDerivatives1(ORDER)
     getCornerEdgeDerivatives1(ORDER)

     #If #ORDER == "2" 
      if( edgeDirection.eq.0 )then
        defineCornerEdgeMetricDerivatives2(st,rs,rt)
        getCornerEdgeDerivatives2(urs2,urt2,urrs2,urrt2)
      else if( edgeDirection.eq.1 )then
        defineCornerEdgeMetricDerivatives2(rt,st,rs)
        getCornerEdgeDerivatives2(ust2,urs2,usst2,urss2)
      else ! edgeDirection.eq.2
        defineCornerEdgeMetricDerivatives2(rs,rt,st)
        getCornerEdgeDerivatives2(urt2,ust2,urtt2,ustt2)
      end if
     #Elif #ORDER == "4" 
      if( edgeDirection.eq.0 )then
        defineCornerEdgeMetricDerivatives2(st4,rs4,rt4)
        getCornerEdgeDerivatives2(urs4,urt4,urrs2,urrt2)
      else if( edgeDirection.eq.1 )then
        defineCornerEdgeMetricDerivatives2(rt4,st4,rs4)
        getCornerEdgeDerivatives2(ust4,urs4,usst2,urss2)
      else ! edgeDirection.eq.2
        defineCornerEdgeMetricDerivatives2(rs4,rt4,st4)
        getCornerEdgeDerivatives2(urt4,ust4,urtt2,ustt2)
      end if

     #Else
       stop 8823
     #End
    
     uex=u(i1,i2,i3,ex)
     uey=u(i1,i2,i3,ey)
     uez=u(i1,i2,i3,ez)

     ! We get a1.urs, a2.urs from the divergence:
     ! a1.ur = -( a1r.u + a2.us + a2s.u + a3.ut + a3t.u )
     ! a1.urs = -( a1s.ur + a1r.us + a1rs.u + a2.uss + 2*a2s.us +a2ss*u + a3s.ut + a3.ust +  a3t.us + a3st.u)
     a1Doturs = -( (a11s*ur  +a12s*vr  +a13s*wr  ) \
                  +(a11r*us  +a12r*vs  +a13r*ws  ) \
                  +(a11rs*uex+a12rs*uey+a13rs*uez) \
                  +(a21*uss  +a22*vss  +a23*wss  ) \
               +2.*(a21s*us  +a22s*vs  +a23s*ws  ) \
                  +(a21ss*uex+a22ss*uey+a23ss*uez) \
                  +(a31s*ut  +a32s*vt  +a33s*wt  ) \
                  +(a31*ust  +a32*vst  +a33*wst  ) \
                  +(a31t*us  +a32t*vs  +a33t*ws  ) \
                  +(a31st*uex+a32st*uey+a33st*uez) )
     ! a2.us = -( a1.ur + a1r.u + a2s.u + a3.ut + a3t.u )
     ! a2.urs = -(  a1.urr+2*a1r*ur + a1rr*u + a2r.us +a2s.ur + a2rs.u+   a3r.ut + a3.urt +  a3t.ur + a3rt.u
     a2Doturs = -( (a21s*ur  +a22s*vr  +a23s*wr  ) \
                  +(a21r*us  +a22r*vs  +a23r*ws  ) \
                  +(a21rs*uex+a22rs*uey+a23rs*uez) \
                  +(a11*urr  +a12*vrr  +a13*wrr  ) \
               +2.*(a11r*ur  +a12r*vr  +a13r*wr  ) \
                  +(a11rr*uex+a12rr*uey+a13rr*uez) \
                  +(a31r*ut  +a32r*vt  +a33r*wt  ) \
                  +(a31*urt  +a32*vrt  +a33*wrt  ) \
                  +(a31t*ur  +a32t*vr  +a33t*wr  ) \
                  +(a31rt*uex+a32rt*uey+a33rt*uez) )

     ! here is a first order approximation to urs, used in the formula for urss and urrs below
     ! urs = ( (u(i1+is1+js1,i2+is2+js2,i3+is3+js3,ex)-u(i1+js1,i2+js2,i3+js3,ex)) \
     !       - (u(i1+is1    ,i2+is2    ,i3+is3    ,ex)-u(i1    ,i2    ,i3    ,ex)) )/(dra*dsa)
     ! vrs = ( (u(i1+is1+js1,i2+is2+js2,i3+is3+js3,ey)-u(i1+js1,i2+js2,i3+js3,ey)) \
     !       - (u(i1+is1    ,i2+is2    ,i3+is3    ,ey)-u(i1    ,i2    ,i3    ,ey)) )/(dra*dsa)
     ! wrs = ( (u(i1+is1+js1,i2+is2+js2,i3+is3+js3,ez)-u(i1+js1,i2+js2,i3+js3,ez)) \
     !       - (u(i1+is1    ,i2+is2    ,i3+is3    ,ez)-u(i1    ,i2    ,i3    ,ez)) )/(dra*dsa)

     ! here is a second order approximation to urs from :
     !  u(r,s)   =u0 + (r*ur+s*us) + (1/2)*( r^2*urr + 2*r*s*urs + s^2*uss ) + (1/6)*( r^3*urrr + ... )
     !  u(2r,2s) =u0 +2(         ) + (4/2)*(                               ) + (8/6)*(                ) 

     #If #ORDER == "2"
       ! We may need a more accurate approx for ur,us for urs
c       um2=4.*u(i1-  is1,i2-  is2,i3-  is3,ex)-6.*u(i1,i2,i3,ex)+4.*u(i1+  is1,i2+  is2,i3+  is3,ex)\
c             -u(i1+2*is1,i2+2*is2,i3+2*is3,ex)
c       ur = (8.*(u(i1+  is1,i2+  is2,i3+  is3,ex)-u(i1-  is1,i2-  is2,i3-  is3,ex))   \
c                           -(u(i1+2*is1,i2+2*is2,i3+2*is3,ex)-um2))   )/(12.*dra)


     #End
     urs = ( 8.*u(i1+is1+js1,i2+is2+js2,i3+is3+js3,ex)-u(i1+2*is1+2*js1,i2+2*is2+2*js2,i3+2*is3+2*js3,ex)-7.*uex \
           -6.*(dra*ur+dsa*us)-2.*(dra**2*urr+dsa**2*uss) )/(4.*dra*dsa)
     vrs = ( 8.*u(i1+is1+js1,i2+is2+js2,i3+is3+js3,ey)-u(i1+2*is1+2*js1,i2+2*is2+2*js2,i3+2*is3+2*js3,ey)-7.*uey \
           -6.*(dra*vr+dsa*vs)-2.*(dra**2*vrr+dsa**2*vss) )/(4.*dra*dsa)
     wrs = ( 8.*u(i1+is1+js1,i2+is2+js2,i3+is3+js3,ez)-u(i1+2*is1+2*js1,i2+2*is2+2*js2,i3+2*is3+2*js3,ez)-7.*uez \
           -6.*(dra*wr+dsa*ws)-2.*(dra**2*wrr+dsa**2*wss) )/(4.*dra*dsa)

     uLapr=0.
     vLapr=0.
     wLapr=0.

     uLaps=0.
     vLaps=0.
     wLaps=0.

     #If #FORCING == "twilightZone"
        ! we need to define uLap, uLaps
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
     
       uLapr=(8.*((uxxp1+uyyp1+uzzp1)-(uxxm1+uyym1+uzzm1))-((uxxp2+uyyp2+uzzp2)-(uxxm2+uyym2+uzzm2)) )/(12.*dra)
       vLapr=(8.*((vxxp1+vyyp1+vzzp1)-(vxxm1+vyym1+vzzm1))-((vxxp2+vyyp2+vzzp2)-(vxxm2+vyym2+vzzm2)) )/(12.*dra)
       wLapr=(8.*((wxxp1+wyyp1+wzzp1)-(wxxm1+wyym1+wzzm1))-((wxxp2+wyyp2+wzzp2)-(wxxm2+wyym2+wzzm2)) )/(12.*dra)
        
       OGDERIV3D(0, 2,0,0, i1-js1,i2-js2,i3-js3, t,uxxm1,vxxm1,wxxm1)
       OGDERIV3D(0, 0,2,0, i1-js1,i2-js2,i3-js3, t,uyym1,vyym1,wyym1)
       OGDERIV3D(0, 0,0,2, i1-js1,i2-js2,i3-js3, t,uzzm1,vzzm1,wzzm1)
     
       OGDERIV3D(0, 2,0,0, i1+js1,i2+js2,i3+js3, t,uxxp1,vxxp1,wxxp1)
       OGDERIV3D(0, 0,2,0, i1+js1,i2+js2,i3+js3, t,uyyp1,vyyp1,wyyp1)
       OGDERIV3D(0, 0,0,2, i1+js1,i2+js2,i3+js3, t,uzzp1,vzzp1,wzzp1)
     
       OGDERIV3D(0, 2,0,0, i1-2*js1,i2-2*js2,i3-2*js3, t,uxxm2,vxxm2,wxxm2)
       OGDERIV3D(0, 0,2,0, i1-2*js1,i2-2*js2,i3-2*js3, t,uyym2,vyym2,wyym2)
       OGDERIV3D(0, 0,0,2, i1-2*js1,i2-2*js2,i3-2*js3, t,uzzm2,vzzm2,wzzm2)
     
       OGDERIV3D(0, 2,0,0, i1+2*js1,i2+2*js2,i3+2*js3, t,uxxp2,vxxp2,wxxp2)
       OGDERIV3D(0, 0,2,0, i1+2*js1,i2+2*js2,i3+2*js3, t,uyyp2,vyyp2,wyyp2)
       OGDERIV3D(0, 0,0,2, i1+2*js1,i2+2*js2,i3+2*js3, t,uzzp2,vzzp2,wzzp2)
     
       uLaps=(8.*((uxxp1+uyyp1+uzzp1)-(uxxm1+uyym1+uzzm1))-((uxxp2+uyyp2+uzzp2)-(uxxm2+uyym2+uzzm2)) )/(12.*dsa)
       vLaps=(8.*((vxxp1+vyyp1+vzzp1)-(vxxm1+vyym1+vzzm1))-((vxxp2+vyyp2+vzzp2)-(vxxm2+vyym2+vzzm2)) )/(12.*dsa)
       wLaps=(8.*((wxxp1+wyyp1+wzzp1)-(wxxm1+wyym1+wzzm1))-((wxxp2+wyyp2+wzzp2)-(wxxm2+wyym2+wzzm2)) )/(12.*dsa)
        

     #End


     a1Dotu0=a11*u(i1,i2,i3,ex)+a12*u(i1,i2,i3,ey)+a13*u(i1,i2,i3,ez)
     a2Dotu0=a21*u(i1,i2,i3,ex)+a22*u(i1,i2,i3,ey)+a23*u(i1,i2,i3,ez)

     a1Doturr=a11*urr+a12*vrr+a13*wrr
     a1Dotuss=a11*uss+a12*vss+a13*wss
     a2Doturr=a21*urr+a22*vrr+a23*wrr
     a2Dotuss=a21*uss+a22*vss+a23*wss

     a3Dotur=a31*ur+a32*vr+a33*wr
     a3Dotus=a31*us+a32*vs+a33*ws

     #If #ORDER == "4"
       ! we get a3.urss and a3.urrs from the equation
       ! c22*uss = -( c11*urr + c33*utt + c1*ur + c2*us + c3*ut )
       ! c11*urr = -( c22*uss + c33*utt + c1*ur + c2*us + c3*ut )
       ! c22*urss = -( c22r*uss + c11*urrr + c11r*urr + c33*urtt + c33r*utt + ... )
       urss = -(  c22r*uss + c11*urrr + c11r*urr + c33*urtt + c33r*utt \
                + c1*urr + c1r*ur + c2*urs + c2r*us + c3*urt + c3r*ut - uLapr )/c22
       urrs = -(  c11s*urr + c22*usss + c22s*uss + c33*ustt + c33s*utt \
                + c1*urs + c1s*ur + c2*uss + c2s*us + c3*ust + c3s*ut - uLaps )/c11
  
       vrss = -(  c22r*vss + c11*vrrr + c11r*vrr + c33*vrtt + c33r*vtt \
                + c1*vrr + c1r*vr + c2*vrs + c2r*vs + c3*vrt + c3r*vt - vLapr )/c22
       vrrs = -(  c11s*vrr + c22*vsss + c22s*vss + c33*vstt + c33s*vtt \
                + c1*vrs + c1s*vr + c2*vss + c2s*vs + c3*vst + c3s*vt - vLaps )/c11
  
       wrss = -(  c22r*wss + c11*wrrr + c11r*wrr + c33*wrtt + c33r*wtt \
                + c1*wrr + c1r*wr + c2*wrs + c2r*ws + c3*wrt + c3r*wt - wLapr )/c22
       wrrs = -(  c11s*wrr + c22*wsss + c22s*wss + c33*wstt + c33s*wtt \
                + c1*wrs + c1s*wr + c2*wss + c2s*ws + c3*wst + c3s*wt - wLaps )/c11

       a3Doturrr=a31*urrr+a32*vrrr+a33*wrrr
       a3Dotusss=a31*usss+a32*vsss+a33*wsss
       a3Doturss=a31*urss+a32*vrss+a33*wrss
       a3Doturrs=a31*urrs+a32*vrrs+a33*wrrs
     #End

     detnt=a33*a11*a22-a33*a12*a21-a13*a31*a22+a31*a23*a12+a13*a32*a21-a32*a23*a11

     ! loop over different ghost points here -- could make a single loop, 1...4 and use arrays of ms1(m) 
     do m1=1,numberOfGhostPoints
     do m2=1,numberOfGhostPoints

      if( edgeDirection.eq.0 )then 
        ms1=0
        ms2=(1-2*side2)*m1
        ms3=(1-2*side3)*m2
        drb=dr(1)*ms2
        dsb=dr(2)*ms3
      else if( edgeDirection.eq.1 )then 
        ms2=0
        ms3=(1-2*side3)*m1
        ms1=(1-2*side1)*m2
        drb=dr(2)*ms3
        dsb=dr(0)*ms1
      else 
        ms3=0
        ms1=(1-2*side1)*m1
        ms2=(1-2*side2)*m2
        drb=dr(0)*ms1
        dsb=dr(1)*ms2
      end if 

     ! **** this is really for order=4 -- no need to be so accurate for order 2 ******


     ! Here are a1.u(i1-ms1,i2-ms2,i3-ms3,.) a2.u(...), a3.u(...)
     ! a1Dotu and a2Dotu -- odd Taylor series
     ! a3Dotu : even Taylor series
     a1Dotu = 2.*a1Dotu0 \
                -(a11*u(i1+ms1,i2+ms2,i3+ms3,ex)+a12*u(i1+ms1,i2+ms2,i3+ms3,ey)+a13*u(i1+ms1,i2+ms2,i3+ms3,ez)) \
                           + drb**2*(a1Doturr) + 2.*drb*dsb*a1Doturs + dsb**2*(a1Dotuss)
     a2Dotu = 2.*a2Dotu0 \
                -(a21*u(i1+ms1,i2+ms2,i3+ms3,ex)+a22*u(i1+ms1,i2+ms2,i3+ms3,ey)+a23*u(i1+ms1,i2+ms2,i3+ms3,ez)) \
                           + drb**2*(a2Doturr) + 2.*drb*dsb*a2Doturs + dsb**2*(a2Dotuss)
     #If #ORDER == "4"
       a3Dotu = (a31*u(i1+ms1,i2+ms2,i3+ms3,ex)+a32*u(i1+ms1,i2+ms2,i3+ms3,ey)+a33*u(i1+ms1,i2+ms2,i3+ms3,ez))\
                -2.*( drb*(a3Dotur) + dsb*(a3Dotus) ) \
           -(1./3.)*( drb**3*(a3Doturrr) + dsb**3*(a3Dotusss) + 3.*drb**2*dsb*(a3Doturrs) + 3.*drb*dsb**2*(a3Doturss) )
     #Elif #ORDER == "2"
       a3Dotu = (a31*u(i1+ms1,i2+ms2,i3+ms3,ex)+a32*u(i1+ms1,i2+ms2,i3+ms3,ey)+a33*u(i1+ms1,i2+ms2,i3+ms3,ez))\
                -2.*( drb*(a3Dotur) + dsb*(a3Dotus) ) 
     #Else
       stop 882266
     #End
     ! Now given a1.u(-1), a2.u(-1) a3.u(-1) we solve for u(-1)

     u(i1-ms1,i2-ms2,i3-ms3,ex)=(a33*a1DotU*a22-a13*a3DotU*a22+a13*a32*a2DotU+a3DotU*a23*a12-a32*a23*a1DotU\
                                -a33*a12*a2DotU)/detnt
     u(i1-ms1,i2-ms2,i3-ms3,ey)=(-a23*a11*a3DotU+a23*a1DotU*a31+a11*a33*a2DotU+a13*a21*a3DotU-a1DotU*a33*a21\
                                 -a13*a2DotU*a31)/detnt
     u(i1-ms1,i2-ms2,i3-ms3,ez)=(a11*a3DotU*a22-a11*a32*a2DotU-a12*a21*a3DotU+a12*a2DotU*a31-a1DotU*a31*a22\
                                +a1DotU*a32*a21)/detnt
     #If #ORDER == "4"
       ! *** extrap for now ****
       ! j1=i1-ms1
       ! j2=i2-ms2
       ! j3=i3-ms3
       ! u(j1,j2,j3,ex)=5.*u(j1+ls1,j2+ls2,j3+ls3,ex)-10.*u(j1+2*ls1,j2+2*ls2,j3+2*ls3,ex)+10.*u(j1+3*ls1,j2+3*ls2,j3+3*ls3,ex)\
       !               -5.*u(j1+4*ls1,j2+4*ls2,j3+4*ls3,ex)+u(j1+5*ls1,j2+5*ls2,j3+5*ls3,ex)
       ! u(j1,j2,j3,ey)=5.*u(j1+ls1,j2+ls2,j3+ls3,ey)-10.*u(j1+2*ls1,j2+2*ls2,j3+2*ls3,ey)+10.*u(j1+3*ls1,j2+3*ls2,j3+3*ls3,ey)\
       !               -5.*u(j1+4*ls1,j2+4*ls2,j3+4*ls3,ey)+u(j1+5*ls1,j2+5*ls2,j3+5*ls3,ey)
       ! u(j1,j2,j3,ez)=5.*u(j1+ls1,j2+ls2,j3+ls3,ez)-10.*u(j1+2*ls1,j2+2*ls2,j3+2*ls3,ez)+10.*u(j1+3*ls1,j2+3*ls2,j3+3*ls3,ez)\
       !               -5.*u(j1+4*ls1,j2+4*ls2,j3+4*ls3,ez)+u(j1+5*ls1,j2+5*ls2,j3+5*ls3,ez)
     #End

     #If #FORCING == "twilightZone"
     if( .true. .or. debug.gt.0 )then
       OGF3D(i1-ms1,i2-ms2,i3-ms3,t, uvm(0),uvm(1),uvm(2))
       if( debug.gt.0 )then
         write(*,'(" corner-edge-ORDER: ghost-pt=",3i4," ls=",3i3," error=",3e9.1)') \
            i1-ms1,i2-ms2,i3-ms3,ls1,ls2,ls3,\
            u(i1-ms1,i2-ms2,i3-ms3,ex)-uvm(0),\
            u(i1-ms1,i2-ms2,i3-ms3,ey)-uvm(1),\
            u(i1-ms1,i2-ms2,i3-ms3,ez)-uvm(2)
       end if
       ! *** for now reset the solution to the exact ***
       ! u(i1-ms1,i2-ms2,i3-ms3,ex)=uvm(0)
       ! u(i1-ms1,i2-ms2,i3-ms3,ey)=uvm(1)
       ! u(i1-ms1,i2-ms2,i3-ms3,ez)=uvm(2)

     end if

     if( debug.gt.2 )then

       write(*,'(" a11,a12,a13=",3f6.2)') a11,a12,a13
       write(*,'(" a21,a22,a23=",3f6.2)') a21,a22,a23
       write(*,'(" a31,a32,a33=",3f6.2)') a31,a32,a33
       write(*,'("  a3Dotu,true=",2e11.3," err=",e10.2)') a3Dotu,(a31*uvm(0)+a32*uvm(1)+a33*uvm(2)),\
             a3Dotu-(a31*uvm(0)+a32*uvm(1)+a33*uvm(2))

      OGF3D(i1-is1-js1,i2-is2-js2,i3-is3-js3,t, uvmm(0),uvmm(1),uvmm(2))
      OGF3D(i1    -js1,i2    -js2,i3    -js3,t, uvzm(0),uvzm(1),uvzm(2))
      OGF3D(i1+is1-js1,i2+is2-js2,i3+is3-js3,t, uvpm(0),uvpm(1),uvpm(2))
                                            
      OGF3D(i1-is1    ,i2-is2    ,i3-is3    ,t, uvmz(0),uvmz(1),uvmz(2))
      OGF3D(i1        ,i2        ,i3        ,t, uvzz(0),uvzz(1),uvzz(2))
      OGF3D(i1+is1    ,i2+is2    ,i3+is3    ,t, uvpz(0),uvpz(1),uvpz(2))

      OGF3D(i1-is1+js1,i2-is2+js2,i3-is3+js3,t, uvmp(0),uvmp(1),uvmp(2))
      OGF3D(i1    +js1,i2    +js2,i3    +js3,t, uvzp(0),uvzp(1),uvzp(2))
      OGF3D(i1+is1+js1,i2+is2+js2,i3+is3+js3,t, uvpp(0),uvpp(1),uvpp(2))


      ur0= ( uvpz(0)-uvmz(0) )/(2.*dra)
      us0= ( uvzp(0)-uvzm(0) )/(2.*dsa)

      urr0= ( uvpz(0)-2.*uvzz(0)+uvmz(0) )/(dra**2)
      uss0= ( uvzp(0)-2.*uvzz(0)+uvzm(0) )/(dsa**2)

      urs0= ( uvpp(0)-uvmp(0)-uvpm(0)+uvmm(0) )/(4.*dra*dsa)
      vrs0= ( uvpp(1)-uvmp(1)-uvpm(1)+uvmm(1) )/(4.*dra*dsa)
      wrs0= ( uvpp(2)-uvmp(2)-uvpm(2)+uvmm(2) )/(4.*dra*dsa)

      urrs0=( (uvpp(0)-2.*uvzp(0)+uvmp(0))-(uvpm(0)-2.*uvzm(0)+uvmm(0)) )/(2.*dsa*dra**2)
      vrrs0=( (uvpp(1)-2.*uvzp(1)+uvmp(1))-(uvpm(1)-2.*uvzm(1)+uvmm(1)) )/(2.*dsa*dra**2)
      wrrs0=( (uvpp(2)-2.*uvzp(2)+uvmp(2))-(uvpm(2)-2.*uvzm(2)+uvmm(2)) )/(2.*dsa*dra**2)

      urss0=( (uvpp(0)-2.*uvpz(0)+uvpm(0))-(uvmp(0)-2.*uvmz(0)+uvmm(0)) )/(2.*dra*dsa**2)
      vrss0=( (uvpp(1)-2.*uvpz(1)+uvpm(1))-(uvmp(1)-2.*uvmz(1)+uvmm(1)) )/(2.*dra*dsa**2)
      wrss0=( (uvpp(2)-2.*uvpz(2)+uvpm(2))-(uvmp(2)-2.*uvmz(2)+uvmm(2)) )/(2.*dra*dsa**2)


       write(*,'(" u(i-is),u(i),u(i+is): err=",3e10.2)') u(i1-is1,i2-is2,i3-is3,ex)-uvmz(0),\
          u(i1,i2,i3,ex)-uvzz(0),u(i1+is1,i2+is2,i3+is3,ex)-uvpz(0)

       write(*,'(" u(i-js),u(i),u(i+js): err=",3e10.2)') u(i1-js1,i2-js2,i3-js3,ex)-uvzm(0),\
          u(i1,i2,i3,ex)-uvzz(0),u(i1+js1,i2+js2,i3+js3,ex)-uvzp(0)

       write(*,'(" ur, true2=",2e11.3," err=",e10.2)') ur,ur0,ur-ur0
       write(*,'(" us, true2=",2e11.3," err=",e10.2)') us,us0,us-us0

       write(*,'(" urr, true2=",2e11.3," err=",e10.2)') urr,urr0,urr-urr0
       write(*,'(" uss, true2=",2e11.3," err=",e10.2)') uss,uss0,uss-uss0

       write(*,'(" urs, true2=",2e11.3," err=",e10.2)') urs,urs0,urs-urs0
       write(*,'(" vrs, true2=",2e11.3," err=",e10.2)') vrs,vrs0,vrs-vrs0
       if( edgeDirection.eq.0 ) then
         write(*,'("  vrs:true=",e11.3)') ust4(i1,i2,i3,ey)
       else if( edgeDirection.eq.1 )then
         write(*,'("  vrs:true=",e11.3)') urt4(i1,i2,i3,ey)
       else
         write(*,'("  vrs:true=",e11.3)') urs4(i1,i2,i3,ey)
       end if
       write(*,'(" wrs, true2=",2e11.3," err=",e10.2)') wrs,wrs0,wrs-wrs0

       #If #ORDER == "4"
        write(*,'(" urrr,true=",2e11.3," err=",e10.2)') urrr,URRR2(ex),urrr-URRR2(ex)
        write(*,'(" usss,true=",2e11.3," err=",e10.2)') usss,USSS2(ex),usss-USSS2(ex)
        write(*,'(" uttt,true=",2e11.3," err=",e10.2)') uttt,UTTT2(ex),uttt-UTTT2(ex)

        write(*,'(" wrrr,true=",2e11.3," err=",e10.2)') wrrr,URRR2(ez),wrrr-URRR2(ez)
        write(*,'(" wsss,true=",2e11.3," err=",e10.2)') wsss,USSS2(ez),wsss-USSS2(ez)
        write(*,'(" wttt,true=",2e11.3," err=",e10.2)') wttt,UTTT2(ez),wttt-UTTT2(ez)

        write(*,'(" urrs,true2=",2e11.3," err=",e10.2)') urrs,urrs0,urrs-urrs0
        write(*,'(" vrrs,true2=",2e11.3," err=",e10.2)') vrrs,vrrs0,vrrs-vrrs0
        write(*,'(" wrrs,true2=",2e11.3," err=",e10.2)') wrrs,wrrs0,wrrs-wrrs0

        write(*,'(" urss,true2=",2e11.3," err=",e10.2)') urss,urss0,urss-urss0
        write(*,'(" vrss,true2=",2e11.3," err=",e10.2)') vrss,vrss0,vrss-vrss0
        write(*,'(" wrss,true2=",2e11.3," err=",e10.2)') wrss,wrss0,wrss-wrss0
       #End


     end if
     #End

     end do
     end do ! m1

    end do ! end do i1
    end do ! end do i2
    end do ! end do i3

  else if( bc1.eq.dirichlet .or. bc2.eq.dirichlet )then

    ! This is a dirichlet BC 

   do m1=1,numberOfGhostPoints
   do m2=1,numberOfGhostPoints

    ! shift to ghost point "(m1,m2)"
    if( edgeDirection.eq.2 )then 
      js1=is1*m1  
      js2=is2*m2
      js3=0
    else if( edgeDirection.eq.1 )then 
      js1=is1*m1  
      js2=0
      js3=is3*m2
    else 
      js1=0
      js2=is2*m1
      js3=is3*m2
    end if 

    do i3=n3a,n3b
    do i2=n2a,n2b
    do i1=n1a,n1b
  
      #If #FORCING == "twilightZone"
        OGF3D(i1-js1,i2-js2,i3-js3,t, g1,g2,g3)
      #End
      u(i1-js1,i2-js2,i3-js3,ex)=g1
      u(i1-js1,i2-js2,i3-js3,ey)=g2
      u(i1-js1,i2-js2,i3-js3,ez)=g3

    end do ! end do i1
    end do ! end do i2
    end do ! end do i3

   end do
   end do ! m1

  else if( bc1.le.0 .or. bc2.le.0 )then
    ! periodic or interpolation -- nothing to do
  else if( bc1.eq.planeWaveBoundaryCondition .or.\
           bc2.eq.planeWaveBoundaryCondition .or. \
           bc1.eq.symmetryBoundaryCondition .or. \
           bc2.eq.symmetryBoundaryCondition .or. \
           (bc1.ge.abcEM2 .and. bc1.le.lastBC) .or. \
           (bc2.ge.abcEM2 .and. bc2.le.lastBC))then
     ! do nothing
  else
    write(*,'("ERROR: unknown boundary conditions bc1,bc2=",2i3)') bc1,bc2
    ! unknown boundary conditions
    stop 8866
  end if


 #Else
   write(*,'("unknown gridType")')
   stop 4578
 #End


 end do
 end do
 end do  ! edge direction

#endMacro







c ======================================================================================
c   Assign edges and corner points next to edges in 3D
c
c  Set the normal component of the solution on the extended boundaries (points N in figure)
c  Set the corner points "C" and points outside vertices
c              |
c              X
c              |
c        N--N--X--X----
c              |
c        C  C  N
c              |
c        C  C  N
c
c =================================================================================
#beginMacro assignEdges3d(ORDER,GRIDTYPE,FORCING)

 do edgeDirection=0,2 ! direction parallel to the edge
! do edgeDirection=0,0 ! direction parallel to the edge
 do sidea=0,1
 do sideb=0,1
   if( edgeDirection.eq.0 )then
     side1=0
     side2=sidea
     side3=sideb
   else if( edgeDirection.eq.1 )then
     side1=sideb 
     side2=0
     side3=sidea
   else
     side1=sidea
     side2=sideb
     side3=0
   end if

 is1=1-2*(side1)
 is2=1-2*(side2)
 is3=1-2*(side3)
 if( edgeDirection.eq.2 )then
  is3=0
  n1a=gridIndexRange(side1,0)
  n1b=gridIndexRange(side1,0)
  n2a=gridIndexRange(side2,1)
  n2b=gridIndexRange(side2,1)
  n3a=gridIndexRange(0,2)
  n3b=gridIndexRange(1,2)
  bc1=boundaryCondition(side1,0)
  bc2=boundaryCondition(side2,1)
 else if( edgeDirection.eq.1 )then
  is2=0
  n1a=gridIndexRange(side1,0)
  n1b=gridIndexRange(side1,0)
  n2a=gridIndexRange(    0,1)
  n2b=gridIndexRange(    1,1)
  n3a=gridIndexRange(side3,2)
  n3b=gridIndexRange(side3,2)
  bc1=boundaryCondition(side1,0)
  bc2=boundaryCondition(side3,2)
 else 
  is1=0  
  n1a=gridIndexRange(    0,0)
  n1b=gridIndexRange(    1,0)
  n2a=gridIndexRange(side2,1)
  n2b=gridIndexRange(side2,1)
  n3a=gridIndexRange(side3,2)
  n3b=gridIndexRange(side3,2)
  bc1=boundaryCondition(side2,1)
  bc2=boundaryCondition(side3,2)
 end if

 g1=0.
 g2=0.
 g3=0.

 ! ********************************************************************
 ! ***************Assign Extended boundary points**********************
 ! ********************************************************************

 #If #GRIDTYPE == "rectangular" 
  do m=1,numberOfGhostPoints

   js1=is1*m  ! shift to ghost point "m"
   js2=is2*m
   js3=is3*m
   if( bc1.eq.perfectElectricalConductor .and.\
       bc2.eq.perfectElectricalConductor )then

     do i3=n3a,n3b
     do i2=n2a,n2b
     do i1=n1a,n1b

      #If #FORCING == "twilightZone"
        OGF3D(i1,i2,i3,t,u0,v0,w0)
      #End
      if( edgeDirection.ne.0 )then
        #If #FORCING == "twilightZone"
          OGF3D(i1-js1,i2,i3,t, um,vm,wm)
          OGF3D(i1+js1,i2,i3,t, up,vp,wp)
          g1=um-up
          g2=vm-2.*v0+vp
          g3=wm-2.*w0+wp
        #End
        u(i1-js1,i2,i3,ex)=                  u(i1+js1,i2,i3,ex) +g1
        u(i1-js1,i2,i3,ey)=2.*u(i1,i2,i3,ey)-u(i1+js1,i2,i3,ey) +g2
        u(i1-js1,i2,i3,ez)=2.*u(i1,i2,i3,ez)-u(i1+js1,i2,i3,ez) +g3
      end if

      if( edgeDirection.ne.1 )then
       #If #FORCING == "twilightZone" 
          OGF3D(i1,i2-js2,i3,t, um,vm,wm)
          OGF3D(i1,i2+js2,i3,t, up,vp,wp)
          g1=um-2.*u0+up
          g2=vm-vp
          g3=wm-2.*w0+wp
        #End
        u(i1,i2-js2,i3,ex)=2.*u(i1,i2,i3,ex)-u(i1,i2+js2,i3,ex) +g1
        u(i1,i2-js2,i3,ey)=                  u(i1,i2+js2,i3,ey)+g2
        u(i1,i2-js2,i3,ez)=2.*u(i1,i2,i3,ez)-u(i1,i2+js2,i3,ez) +g3
      end if

      if( edgeDirection.ne.2 )then
        #If #FORCING == "twilightZone" 
          OGF3D(i1,i2,i3-js3,t, um,vm,wm)
          OGF3D(i1,i2,i3+js3,t, up,vp,wp)
          g1=um-2.*u0+up
          g2=vm-2.*v0+vp
          g3=wm-wp
        #End
        u(i1,i2,i3-js3,ex)=2.*u(i1,i2,i3,ex)-u(i1,i2,i3+js3,ex) +g1
        u(i1,i2,i3-js3,ey)=2.*u(i1,i2,i3,ey)-u(i1,i2,i3+js3,ey) +g2
        u(i1,i2,i3-js3,ez)=                   u(i1,i2,i3+js3,ez)+g3
      end if 

     end do ! end do i1
     end do ! end do i2
     end do ! end do i3

   else if( bc1.eq.dirichlet .or. bc2.eq.dirichlet )then

     do i3=n3a,n3b
     do i2=n2a,n2b
     do i1=n1a,n1b

      if( edgeDirection.ne.0 )then
        #If #FORCING == "twilightZone"
          OGF3D(i1-js1,i2,i3,t,g1,g2,g3)
        #End
        u(i1-js1,i2,i3,ex)=g1
        u(i1-js1,i2,i3,ey)=g2
        u(i1-js1,i2,i3,ez)=g3
      end if

      if( edgeDirection.ne.1 )then
        #If #FORCING == "twilightZone"
          OGF3D(i1,i2-js2,i3,t,g1,g2,g3)
        #End
        u(i1,i2-js2,i3,ex)=g1
        u(i1,i2-js2,i3,ey)=g2
        u(i1,i2-js2,i3,ez)=g3
      end if

      if( edgeDirection.ne.2 )then
        #If #FORCING == "twilightZone"
          OGF3D(i1,i2,i3-js3,t,g1,g2,g3)
        #End
        u(i1,i2,i3-js3,ex)=g1
        u(i1,i2,i3-js3,ey)=g2
        u(i1,i2,i3-js3,ez)=g3
      end if

     end do ! end do i1
     end do ! end do i2
     end do ! end do i3

   else if( bc1.le.0 .or. bc2.le.0 )then
    ! periodic or interpolation -- nothing to do
   else if( bc1.eq.planeWaveBoundaryCondition .or.\
            bc2.eq.planeWaveBoundaryCondition .or. \
           bc1.eq.symmetryBoundaryCondition .or. \
           bc2.eq.symmetryBoundaryCondition .or. \
           (bc1.ge.abcEM2 .and. bc1.le.lastBC) .or. \
           (bc2.ge.abcEM2 .and. bc2.le.lastBC))then
     ! do nothing
   else
     write(*,'("ERROR: unknown boundary conditions bc1,bc2=",2i3)') bc1,bc2
     ! unknown boundary conditions
      stop 8866
   end if

  end do ! end do m
   
 #Elif #GRIDTYPE == "curvilinear"

  is1=0
  is2=0
  is3=0
  js1=0
  js2=0
  js3=0
  ks1=0
  ks2=0
  ks3=0
  if( edgeDirection.eq.0 )then
    axis=1
    axisp1=2
    axisp2=0
    is2=1-2*(side2)
    js3=1-2*(side3)
    ks1=1

    dra=dr(axis  )*(1-2*(side2))
    dsa=dr(axisp1)*(1-2*(side3))
    dta=dr(axisp2)*(1          )
  else if( edgeDirection.eq.1 )then
    axis=2
    axisp1=0
    axisp2=1
    is3=1-2*(side3)
    js1=1-2*(side1)
    ks2=1
    dra=dr(axis  )*(1-2*(side3))
    dsa=dr(axisp1)*(1-2*(side1))
    dta=dr(axisp2)*(1          )
  else
    axis=0
    axisp1=1
    axisp2=2
    is1=1-2*(side1)
    js2=1-2*(side2)
    ks3=1
    dra=dr(axis  )*(1-2*(side1))
    dsa=dr(axisp1)*(1-2*(side2))
    dta=dr(axisp2)*(1          )
  end if 

  if( debug.gt.0 )then
    write(*,'(" bce4: **** Start: edgeDirection=",i1," ,side1,side2,side3 = ",3i2," axis,axisp1,axisp2=",3i2,/,"      dra,dsa,dta=",3e10.2,"****")') \
      edgeDirection,side1,side2,side3,axis,axisp1,axisp2,dra,dsa,dta
  end if

! if( orderOfAccuracy.eq.4 )then
  #If #ORDER == "4"

   if( bc1.eq.perfectElectricalConductor .and.\
       bc2.eq.perfectElectricalConductor )then

     do i3=n3a,n3b
     do i2=n2a,n2b
     do i1=n1a,n1b

       c11 = C11D3(i1,i2,i3)
       c22 = C22D3(i1,i2,i3)
       c33 = C33D3(i1,i2,i3)

       c1 = C1D3Order4(i1,i2,i3)
       c2 = C2D3Order4(i1,i2,i3)
       c3 = C3D3Order4(i1,i2,i3)

       ! urr=URR ,uss,utt,ur,us,ut (also for v and w)
       urr=URR4(ex)
       uss=USS4(ex)
       utt=UTT4(ex)
       ur = UR4(ex)
       us = US4(ex)
       ut = UT4(ex)
                
       vrr=URR4(ey)
       vss=USS4(ey)
       vtt=UTT4(ey)
       vr = UR4(ey)
       vs = US4(ey)
       vt = UT4(ey)
                
       wrr=URR4(ez)
       wss=USS4(ez)
       wtt=UTT4(ez)
       wr = UR4(ez)
       ws = US4(ez)
       wt = UT4(ez)

       !    deltaFu,deltaFv,deltaFw = RHS for Delta(u,v,w)
       deltaFu=0.
       deltaFv=0.
       deltaFw=0.
       !    g1f,g2f = RHS for extrapolation, a1.D+2^4u(i1,i2-2)=g1f, a2.D+2^4u(i1-2,i2)=g2f,    
       g1f=0.
       g2f=0.


!        if( debug.gt.1 )then
!         write(*,'(" bce4: before: u(-1,0),(-2,0)=",6f7.2)') \
!           u(i1-  is1,i2-  is2,i3-  is3,ex),\
!           u(i1-  is1,i2-  is2,i3-  is3,ey),\
!           u(i1-  is1,i2-  is2,i3-  is3,ez),\
!           u(i1-2*is1,i2-2*is2,i3-2*is3,ex),\
!           u(i1-2*is1,i2-2*is2,i3-2*is3,ey),\
!           u(i1-2*is1,i2-2*is2,i3-2*is3,ez)
!         write(*,'(" bce4: before u(0,2),u(0,1),u(0,0),u(0,-1),(0,-2)=",/,(4x,3f7.2))') \
!           u(i1+2*js1,i2+2*js2,i3+2*js3,ex),\
!           u(i1+2*js1,i2+2*js2,i3+2*js3,ey),\
!           u(i1+2*js1,i2+2*js2,i3+2*js3,ez),\
!           u(i1+  js1,i2+  js2,i3+  js3,ex),\
!           u(i1+  js1,i2+  js2,i3+  js3,ey),\
!           u(i1+  js1,i2+  js2,i3+  js3,ez),\
!           u(i1,i2,i3,ex),\
!           u(i1,i2,i3,ey),\
!           u(i1,i2,i3,ez),\
!           u(i1-  js1,i2-  js2,i3-  js3,ex),\
!           u(i1-  js1,i2-  js2,i3-  js3,ey),\
!           u(i1-  js1,i2-  js2,i3-  js3,ez),\
!           u(i1-2*js1,i2-2*js2,i3-2*js3,ex),\
!           u(i1-2*js1,i2-2*js2,i3-2*js3,ey),\
!           u(i1-2*js1,i2-2*js2,i3-2*js3,ez)
!          write(*,'(" bce4: urr,uss,utt=",9f7.2)') urr,vrr,wrr,uss,vss,wss,utt,vtt,wtt
!        end if

       #If #FORCING == "twilightZone"

        OGDERIV3D(0, 2,0,0, i1,i2,i3, t,uxx,vxx,wxx)
        OGDERIV3D(0, 0,2,0, i1,i2,i3, t,uyy,vyy,wyy)
        OGDERIV3D(0, 0,0,2, i1,i2,i3, t,uzz,vzz,wzz)

        deltaFu=uxx+uyy+uzz
        deltaFv=vxx+vyy+vzz
        deltaFw=wxx+wyy+wzz

        ! for now remove the error in the extrapolation ************
        OGF3D(i1,i2,i3,t, uv0(0),uv0(1),uv0(2))

        OGF3D(i1-js1,i2-js2,i3-js3,t, uvm(0),uvm(1),uvm(2))
        OGF3D(i1+js1,i2+js2,i3+js3,t, uvp(0),uvp(1),uvp(2))
        OGF3D(i1-2*js1,i2-2*js2,i3-2*js3,t, uvm2(0),uvm2(1),uvm2(2))
        OGF3D(i1+2*js1,i2+2*js2,i3+2*js3,t, uvp2(0),uvp2(1),uvp2(2))

        m1=i1-2*js1
        m2=i2-2*js2
        m3=i3-2*js3
        g1f   = A11D3(m1,m2,m3)*( uvm2(0)-4.*uvm(0)+6.*uv0(0)-4.*uvp(0)+uvp2(0)) +\
                A12D3(m1,m2,m3)*( uvm2(1)-4.*uvm(1)+6.*uv0(1)-4.*uvp(1)+uvp2(1)) +\
                A13D3(m1,m2,m3)*( uvm2(2)-4.*uvm(2)+6.*uv0(2)-4.*uvp(2)+uvp2(2))

        OGF3D(i1-is1,i2-is2,i3-is3,t, uvm(0),uvm(1),uvm(2))
        OGF3D(i1+is1,i2+is2,i3+is3,t, uvp(0),uvp(1),uvp(2))
        OGF3D(i1-2*is1,i2-2*is2,i3-2*is3,t, uvm2(0),uvm2(1),uvm2(2))
        OGF3D(i1+2*is1,i2+2*is2,i3+2*is3,t, uvp2(0),uvp2(1),uvp2(2))

        m1=i1-2*is1
        m2=i2-2*is2
        m3=i3-2*is3
        g2f = A21D3(m1,m2,m3)*( uvm2(0)-4.*uvm(0)+6.*uv0(0)-4.*uvp(0)+uvp2(0)) +\
              A22D3(m1,m2,m3)*( uvm2(1)-4.*uvm(1)+6.*uv0(1)-4.*uvp(1)+uvp2(1)) +\
              A23D3(m1,m2,m3)*( uvm2(2)-4.*uvm(2)+6.*uv0(2)-4.*uvp(2)+uvp2(2))

       #End

! this next file is generated by bce.maple
#Include "bcExtended3d4.h"

       #If #FORCING == "twilightZone"
        if( debug.gt.1 )then
         write(*,'(/," bce4: extended:(i1,i2,i3)=",3i5," is=",3i2," js=",3i2," ks=",3i2)') i1,i2,i3,is1,is2,is3,\
               js1,js2,js3,ks1,ks2,ks3
         write(*,'(" bce4: c11,c22,c33,c1,c2,c3, DeltaU,DeltaV,DeltaW=",9f6.2)') c11,c22,c33,c1,c2,c3, DeltaU,DeltaV,DeltaW
        end if
        if( debug.gt.0 )then
         OGF3D(i1-is1,i2-is2,i3-is3,t, uvm(0),uvm(1),uvm(2))
         OGF3D(i1-2*is1,i2-2*is2,i3-2*is3,t, uvm2(0),uvm2(1),uvm2(2))
         write(*,'(" bce4: extended: (i1,i2,i3)=",3i4," err(-1,0),(-2,0)=",6e9.1)') i1,i2,i3,\
           u(i1-  is1,i2-  is2,i3-  is3,ex)-uvm(0),\
           u(i1-  is1,i2-  is2,i3-  is3,ey)-uvm(1),\
           u(i1-  is1,i2-  is2,i3-  is3,ez)-uvm(2),\
           u(i1-2*is1,i2-2*is2,i3-2*is3,ex)-uvm2(0),\
           u(i1-2*is1,i2-2*is2,i3-2*is3,ey)-uvm2(1),\
           u(i1-2*is1,i2-2*is2,i3-2*is3,ez)-uvm2(2)
        end if
        if( debug.gt.1 )then
         write(*,'(" bce4: true(-1,0),(-2,0)    =",6f7.2)') uvm(0),uvm(1),uvm(2),uvm2(0),uvm2(1), uvm2(2)
         write(*,'(" bce4: computed(-1,0),(-2,0)=",6f7.2)') \
           u(i1-  is1,i2-  is2,i3-  is3,ex),\
           u(i1-  is1,i2-  is2,i3-  is3,ey),\
           u(i1-  is1,i2-  is2,i3-  is3,ez),\
           u(i1-2*is1,i2-2*is2,i3-2*is3,ex),\
           u(i1-2*is1,i2-2*is2,i3-2*is3,ey),\
           u(i1-2*is1,i2-2*is2,i3-2*is3,ez)
        end if
        if( debug.gt.0 )then
         OGF3D(i1-js1,i2-js2,i3-js3,t, uvm(0),uvm(1),uvm(2))
         OGF3D(i1-2*js1,i2-2*js2,i3-2*js3,t, uvm2(0),uvm2(1),uvm2(2))
         write(*,'(" bce4: extended: (i1,i2,i3)=",3i4," err(0,-1),(0,-2)=",6e9.1)') i1,i2,i3,\
           u(i1-  js1,i2-  js2,i3-  js3,ex)-uvm(0),\
           u(i1-  js1,i2-  js2,i3-  js3,ey)-uvm(1),\
           u(i1-  js1,i2-  js2,i3-  js3,ez)-uvm(2),\
           u(i1-2*js1,i2-2*js2,i3-2*js3,ex)-uvm2(0),\
           u(i1-2*js1,i2-2*js2,i3-2*js3,ey)-uvm2(1),\
           u(i1-2*js1,i2-2*js2,i3-2*js3,ez)-uvm2(2)
        end if
        if( debug.gt.1 )then
         write(*,'(" bce4: true(0,-1),(0,-2)    =",6f7.2)') uvm(0),uvm(1),uvm(2),uvm2(0),uvm2(1), uvm2(2)
         write(*,'(" bce4: computed(0,-1),(0,-2)=",6f7.2)') \
           u(i1-  js1,i2-  js2,i3-  js3,ex),\
           u(i1-  js1,i2-  js2,i3-  js3,ey),\
           u(i1-  js1,i2-  js2,i3-  js3,ez),\
           u(i1-2*js1,i2-2*js2,i3-2*js3,ex),\
           u(i1-2*js1,i2-2*js2,i3-2*js3,ey),\
           u(i1-2*js1,i2-2*js2,i3-2*js3,ez)
        end if

        if( debug.gt.1 )then
         m1=i1-is1
         m2=i2-is2
         m3=i3-is3
         OGF3D(m1,m2,m3,t, uvm(0),uvm(1),uvm(2))
         write(*,'(" bce4:tan-comp: err(a1.u1,a3.u1)=",2e10.2)') \
              A11D3(m1,m2,m3)*(u(m1,m2,m3,ex)-uvm(0))+\
              A12D3(m1,m2,m3)*(u(m1,m2,m3,ey)-uvm(1))+\
              A13D3(m1,m2,m3)*(u(m1,m2,m3,ez)-uvm(2)), \
              A31D3(m1,m2,m3)*(u(m1,m2,m3,ex)-uvm(0))+\
              A32D3(m1,m2,m3)*(u(m1,m2,m3,ey)-uvm(1))+\
              A33D3(m1,m2,m3)*(u(m1,m2,m3,ez)-uvm(2))

         
         write(*,'(" bce4:tan: u1[k] = b1[k] + g1[k]")')
         a11c=A11D3(m1,m2,m3)
         a12c=A12D3(m1,m2,m3)
         a13c=A13D3(m1,m2,m3)
         a21c=A21D3(m1,m2,m3)
         a22c=A22D3(m1,m2,m3)
         a23c=A23D3(m1,m2,m3)
         a31c=A31D3(m1,m2,m3)
         a32c=A32D3(m1,m2,m3)
         a33c=A33D3(m1,m2,m3)
         write(*,'(" bce4:tan: (a11,a12,a13)=(",3e10.2,")")') a11c,a12c,a13c
         write(*,'(" bce4:tan: (a21,a22,a23)=(",3e10.2,")")') a21c,a22c,a23c
         write(*,'(" bce4:tan: (a31,a32,a33)=(",3e10.2,")")') a31c,a32c,a33c
         write(*,'(" bce4:tan: (b11,b12,b13)=(",3e10.2,") (g11,g12,g12)=(",3e10.2,")")') b11,b12,b13,g11,g12,g13
         write(*,'(" bce4:tan: a1Dotu1-a1.g1 =",e10.2,", a3Dotu1-a3.g1 =",e10.2)') \
                      a1Dotu1-(a11c*g11+a12c*g12+a13c*g13),\
                      a3Dotu1-(a31c*g11+a32c*g12+a33c*g13)

         m1=i1-2*is1
         m2=i2-2*is2
         m3=i3-2*is3
         OGF3D(m1,m2,m3,t, uvm(0),uvm(1),uvm(2))
         write(*,'(" bce4:tan-comp: err(a1.u2,a3.u2)=",2e10.2)') \
              A11D3(m1,m2,m3)*(u(m1,m2,m3,ex)-uvm(0))+\
              A12D3(m1,m2,m3)*(u(m1,m2,m3,ey)-uvm(1))+\
              A13D3(m1,m2,m3)*(u(m1,m2,m3,ez)-uvm(2)), \
              A31D3(m1,m2,m3)*(u(m1,m2,m3,ex)-uvm(0))+\
              A32D3(m1,m2,m3)*(u(m1,m2,m3,ey)-uvm(1))+\
              A33D3(m1,m2,m3)*(u(m1,m2,m3,ez)-uvm(2))

         write(*,'(" bce4:tan: u2[k] = b2[k] + g2[k]")')
         a11c=A11D3(m1,m2,m3)
         a12c=A12D3(m1,m2,m3)
         a13c=A13D3(m1,m2,m3)
         a21c=A21D3(m1,m2,m3)
         a22c=A22D3(m1,m2,m3)
         a23c=A23D3(m1,m2,m3)
         a31c=A31D3(m1,m2,m3)
         a32c=A32D3(m1,m2,m3)
         a33c=A33D3(m1,m2,m3)
         write(*,'(" bce4:tan: (a11,a12,a13)=(",3e10.2,")")') a11c,a12c,a13c
         write(*,'(" bce4:tan: (b21,b22,b23)=(",3e10.2,") (g21,g22,g22)=(",3e10.2,")")') b21,b22,b23,g21,g22,g23
         write(*,'(" bce4:tan: a1Dotu2-a1.g2 =",e10.2,", a3Dotu2-a3.g2 =",e10.2)') \
                      a1Dotu2-(a11c*g21+a12c*g22+a13c*g23),\
                      a3Dotu2-(a31c*g21+a32c*g22+a33c*g23)


         ! error in extrap : a2.D+ u(i1-2) - g2f
         write(*,'(" bce4:extrap: err(a2.D+ u(i1-2)-g2f)=",e10.2," g2f=",e10.2)')\
              a21c*(u(i1-2*is1,i2-2*is2,i3-2*is3,ex)-4.*u(i1-is1,i2-is2,i3-is3,ex)\
                +6.*u(i1,i2,i3,ex)-4.*u(i1+is1,i2+is2,i3+is3,ex)+u(i1+2*is1,i2+2*is2,i3+2*is3,ex)) \
            + a22c*(u(i1-2*is1,i2-2*is2,i3-2*is3,ey)-4.*u(i1-is1,i2-is2,i3-is3,ey)\
                +6.*u(i1,i2,i3,ey)-4.*u(i1+is1,i2+is2,i3+is3,ey)+u(i1+2*is1,i2+2*is2,i3+2*is3,ey)) \
            + a23c*(u(i1-2*is1,i2-2*is2,i3-2*is3,ez)-4.*u(i1-is1,i2-is2,i3-is3,ez)\
                +6.*u(i1,i2,i3,ez)-4.*u(i1+is1,i2+is2,i3+is3,ez)+u(i1+2*is1,i2+2*is2,i3+2*is3,ez)) -g2f,g2f



         m1=i1-js1
         m2=i2-js2
         m3=i3-js3
         write(*,'(" bce4:tan: u3[k] = b3[k] + g3[k]")')
         a11c=A11D3(m1,m2,m3)
         a12c=A12D3(m1,m2,m3)
         a13c=A13D3(m1,m2,m3)
         a21c=A21D3(m1,m2,m3)
         a22c=A22D3(m1,m2,m3)
         a23c=A23D3(m1,m2,m3)
         a31c=A31D3(m1,m2,m3)
         a32c=A32D3(m1,m2,m3)
         a33c=A33D3(m1,m2,m3)
         write(*,'(" bce4:tan: (a11,a12,a13)=(",3e10.2,")")') a11c,a12c,a13c
         write(*,'(" bce4:tan: (b31,b32,b33)=(",3e10.2,") (g31,g32,g32)=(",3e10.2,")")') b31,b32,b33,g31,g32,g33
         write(*,'(" bce4:tan: a2Dotu3-a2.g3 =",e10.2,", a3Dotu3-a3.g3 =",e10.2)') \
                      a2Dotu3-(a21c*g31+a22c*g32+a23c*g33),\
                      a3Dotu3-(a31c*g31+a32c*g32+a33c*g33)

         OGF3D(m1,m2,m3,t, uvm(0),uvm(1),uvm(2))
         write(*,'(" bce4:tan-comp: err(a2.u3,a3.u3)=",2e10.2)') \
              A21D3(m1,m2,m3)*(u(m1,m2,m3,ex)-uvm(0))+\
              A22D3(m1,m2,m3)*(u(m1,m2,m3,ey)-uvm(1))+\
              A23D3(m1,m2,m3)*(u(m1,m2,m3,ez)-uvm(2)), \
              A31D3(m1,m2,m3)*(u(m1,m2,m3,ex)-uvm(0))+\
              A32D3(m1,m2,m3)*(u(m1,m2,m3,ey)-uvm(1))+\
              A33D3(m1,m2,m3)*(u(m1,m2,m3,ez)-uvm(2))

         m1=i1-2*js1
         m2=i2-2*js2
         m3=i3-2*js3
         a11c=A11D3(m1,m2,m3)
         a12c=A12D3(m1,m2,m3)
         a13c=A13D3(m1,m2,m3)
         a21c=A21D3(m1,m2,m3)
         a22c=A22D3(m1,m2,m3)
         a23c=A23D3(m1,m2,m3)
         a31c=A31D3(m1,m2,m3)
         a32c=A32D3(m1,m2,m3)
         a33c=A33D3(m1,m2,m3)
         write(*,'(" bce4:tan: (a11,a12,a13)=(",3e10.2,")")') a11c,a12c,a13c
         write(*,'(" bce4:tan: (b41,b42,b43)=(",3e10.2,") (g41,g42,g42)=(",3e10.2,")")') b41,b42,b43,g41,g42,g43
         write(*,'(" bce4:tan: a2Dotu4-a2.g4 =",e10.2,", a3Dotu4-a3.g4 =",e10.2)') \
                      a2Dotu4-(a21c*g41+a22c*g42+a23c*g43),\
                      a3Dotu4-(a31c*g41+a32c*g42+a33c*g43)


         OGF3D(m1,m2,m3,t, uvm(0),uvm(1),uvm(2))
         write(*,'(" bce4:tan-comp: err(a2.u4,a3.u4)=",2e10.2)') \
              A21D3(m1,m2,m3)*(u(m1,m2,m3,ex)-uvm(0))+\
              A22D3(m1,m2,m3)*(u(m1,m2,m3,ey)-uvm(1))+\
              A23D3(m1,m2,m3)*(u(m1,m2,m3,ez)-uvm(2)), \
              A31D3(m1,m2,m3)*(u(m1,m2,m3,ex)-uvm(0))+\
              A32D3(m1,m2,m3)*(u(m1,m2,m3,ey)-uvm(1))+\
              A33D3(m1,m2,m3)*(u(m1,m2,m3,ez)-uvm(2))

         a11c=A11D3(m1,m2,m3)
         a12c=A12D3(m1,m2,m3)
         a13c=A13D3(m1,m2,m3)
         a21c=A21D3(m1,m2,m3)
         a22c=A22D3(m1,m2,m3)
         a23c=A23D3(m1,m2,m3)
         a31c=A31D3(m1,m2,m3)
         a32c=A32D3(m1,m2,m3)
         a33c=A33D3(m1,m2,m3)

         ! error in extrap : a1.D+ u(i2-2) - g1f
         write(*,'(" bce4:extrap: err(a1.D+ u(i2-2)-g1f)=",e10.2," g1f=",e10.2)')\
              a11c*(u(i1-2*js1,i2-2*js2,i3-2*js3,ex)-4.*u(i1-js1,i2-js2,i3-js3,ex)\
                +6.*u(i1,i2,i3,ex)-4.*u(i1+js1,i2+js2,i3+js3,ex)+u(i1+2*js1,i2+2*js2,i3+2*js3,ex)) \
            + a12c*(u(i1-2*js1,i2-2*js2,i3-2*js3,ey)-4.*u(i1-js1,i2-js2,i3-js3,ey)\
                +6.*u(i1,i2,i3,ey)-4.*u(i1+js1,i2+js2,i3+js3,ey)+u(i1+2*js1,i2+2*js2,i3+2*js3,ey)) \
            + a13c*(u(i1-2*js1,i2-2*js2,i3-2*js3,ez)-4.*u(i1-js1,i2-js2,i3-js3,ez)\
                +6.*u(i1,i2,i3,ez)-4.*u(i1+js1,i2+js2,i3+js3,ez)+u(i1+2*js1,i2+2*js2,i3+2*js3,ez)) -g1f,g1f


         uLap=ulaplacian43(i1,i2,i3,ex)
         vLap=ulaplacian43(i1,i2,i3,ey)
         wLap=ulaplacian43(i1,i2,i3,ez)

         write(*,'(" bce4: err(a1.Delta u)=",e10.2," err(a2.Delta u)=",e10.2)')\
           A11D3(i1,i2,i3)*(uLap-deltaFu)+A12D3(i1,i2,i3)*(vLap-deltaFv)+A13D3(i1,i2,i3)*(wLap-deltaFw),\
           A21D3(i1,i2,i3)*(uLap-deltaFu)+A22D3(i1,i2,i3)*(vLap-deltaFv)+A23D3(i1,i2,i3)*(wLap-deltaFw)
        end if ! end debug
        if( debug.gt.2 )then
         write(*,'(" bce4: a1DotLu,a2DotLu=",2e10.2,", deltaFu,deltaFv,deltaFw="3e10.2)') \
                a1DotLu,a2DotLu,deltaFu,deltaFv,deltaFw
         
         write(*,'(" bce4: cc1ka : uv(0,-2) uv(0,-1) cc1kb: uv(2,0) uv(1,0)")')
         write(*,'(" bce4: 12.*(cc11a,cc12a,cc13a,cc14a,cc15a,cc16a)*dr^2=",6f6.2)')\
               12.*cc11a*dra**2,12.*cc12a*dra**2,12.*cc13a*dsa**2,12.*cc14a*dsa**2,12.*cc15a*dsa**2,12.*cc16a*dsa**2
         write(*,'(" bce4: 12.*(cc11b,cc12b,cc13b,cc14b,cc15b,cc16b)*dr^2=",6f6.2)')\
               12.*cc11b*dra**2,12.*cc12b*dra**2,12.*cc13b*dsa**2,12.*cc14b*dsa**2,12.*cc15b*dsa**2,12.*cc16b*dsa**2
         write(*,'(" bce4: 12.*(cc21a,cc22a,cc23a,cc24a,cc25a,cc26a)*dr^2=",6f6.2)')\
               12.*cc21a*dra**2,12.*cc22a*dra**2,12.*cc23a*dsa**2,12.*cc24a*dsa**2,12.*cc25a*dsa**2,12.*cc26a*dsa**2
         write(*,'(" bce4: 12.*(cc21b,cc22b,cc23b,cc24b,cc25b,cc26b)*dr^2=",6f6.2)')\
               12.*cc21b*dra**2,12.*cc22b*dra**2,12.*cc23b*dsa**2,12.*cc24b*dsa**2,12.*cc25b*dsa**2,12.*cc26b*dsa**2
         write(*,'(" bce4: 12.*(d11,d12,d13,d14)*dr^2=",4f6.2,", 12.*f1*dr^2,12.*f1x*dr^2=",2f7.2)')\
                12.*dd11*dra**2,12.*dd12*dra**2,12.*dd13*dsa**2,12.*dd14*dsa**2,12.*f1*dra**2,12.*f1x*dra**2
         write(*,'(" bce4: 12.*(d21,d22,d23,d24)*dr^2=",4f6.2,", 12.*f2,12.*f2x*dr^2=",2f7.2)')\
                12.*dd21*dra**2,12.*dd22*dra**2,12.*dd23*dsa**2,12.*dd24*dsa**2,12.*f2*dra**2,12.*f2x*dra**2
        end if ! end debug

         ! *** for now -- set solution to be exact ---
         
         ! OGF3D(i1-is1,i2-is2,i3-is3,t, uvm(0),uvm(1),uvm(2))
         ! OGF3D(i1-2*is1,i2-2*is2,i3-2*is3,t, uvm2(0),uvm2(1),uvm2(2))
         ! u(i1-  is1,i2-  is2,i3-  is3,ex)=uvm(0)
         ! u(i1-  is1,i2-  is2,i3-  is3,ey)=uvm(1)
         ! u(i1-  is1,i2-  is2,i3-  is3,ez)=uvm(2)
         ! u(i1-2*is1,i2-2*is2,i3-2*is3,ex)=uvm2(0)
         ! u(i1-2*is1,i2-2*is2,i3-2*is3,ey)=uvm2(1)
         ! u(i1-2*is1,i2-2*is2,i3-2*is3,ez)=uvm2(2)
         
         ! OGF3D(i1-js1,i2-js2,i3-js3,t, uvm(0),uvm(1),uvm(2))
         ! OGF3D(i1-2*js1,i2-2*js2,i3-2*js3,t, uvm2(0),uvm2(1),uvm2(2))
         ! u(i1-  js1,i2-  js2,i3-  js3,ex)=uvm(0)
         ! u(i1-  js1,i2-  js2,i3-  js3,ey)=uvm(1)
         ! u(i1-  js1,i2-  js2,i3-  js3,ez)=uvm(2)
         ! u(i1-2*js1,i2-2*js2,i3-2*js3,ex)=uvm2(0)
         ! u(i1-2*js1,i2-2*js2,i3-2*js3,ey)=uvm2(1)
         ! u(i1-2*js1,i2-2*js2,i3-2*js3,ez)=uvm2(2)



       #End

     end do ! end do i1
     end do ! end do i2
     end do ! end do i3

   else if( bc1.eq.dirichlet .or. bc2.eq.dirichlet )then

     do i3=n3a,n3b
     do i2=n2a,n2b
     do i1=n1a,n1b

      if( edgeDirection.ne.0 )then
        #If #FORCING == "twilightZone"
          OGF3D(i1-js1,i2,i3,t,g1,g2,g3)
        #End
        u(i1-js1,i2,i3,ex)=g1
        u(i1-js1,i2,i3,ey)=g2
        u(i1-js1,i2,i3,ez)=g3
      end if

      if( edgeDirection.ne.1 )then
        #If #FORCING == "twilightZone"
          OGF3D(i1,i2-js2,i3,t,g1,g2,g3)
        #End
        u(i1,i2-js2,i3,ex)=g1
        u(i1,i2-js2,i3,ey)=g2
        u(i1,i2-js2,i3,ez)=g3
      end if

      if( edgeDirection.ne.2 )then
        #If #FORCING == "twilightZone"
          OGF3D(i1,i2,i3-js3,t,g1,g2,g3)
        #End
        u(i1,i2,i3-js3,ex)=g1
        u(i1,i2,i3-js3,ey)=g2
        u(i1,i2,i3-js3,ez)=g3
      end if

     end do ! end do i1
     end do ! end do i2
     end do ! end do i3

    else if( bc1.le.0 .or. bc2.le.0 )then
      ! periodic or interpolation -- nothing to do
    else if( bc1.eq.planeWaveBoundaryCondition .or.\
             bc2.eq.planeWaveBoundaryCondition .or. \
           bc1.eq.symmetryBoundaryCondition .or. \
           bc2.eq.symmetryBoundaryCondition .or. \
           (bc1.ge.abcEM2 .and. bc1.le.lastBC) .or. \
           (bc2.ge.abcEM2 .and. bc2.le.lastBC))then
      ! do nothing
    else
      write(*,'("ERROR: unknown boundary conditions bc1,bc2=",2i3)') bc1,bc2
      ! unknown boundary conditions
      stop 8866
   end if


 ! end orderOfAccuracy==4 
 #Elif #ORDER == "2"

   ! write(*,'(" assignEdges3d: unimplemented orderOfAccuracy =",i4)') orderOfAccuracy
   ! stop 123456

   if( bc1.eq.perfectElectricalConductor .and.\
       bc2.eq.perfectElectricalConductor )then

     do i3=n3a,n3b
     do i2=n2a,n2b
     do i1=n1a,n1b

       !           |
       ! extrap(a2.u)
       !   a3.u=0  |
       !   a1.u=0  |
       !     X-----+-----------
       !   i-is    |
       !           |
       !           X a2.u=0, a3.u=0, Extrap( a1.u ) 
       !           i-js
       a11 =A11D3(i1,i2,i3)
       a12 =A12D3(i1,i2,i3)
       a13 =A13D3(i1,i2,i3)
                 
       a21 =A21D3(i1,i2,i3)
       a22 =A22D3(i1,i2,i3)
       a23 =A23D3(i1,i2,i3)
                 
       a31 =A31D3(i1,i2,i3)
       a32 =A32D3(i1,i2,i3)
       a33 =A33D3(i1,i2,i3)

       detnt=a33*a11*a22-a33*a12*a21-a13*a31*a22+a31*a23*a12+a13*a32*a21-a32*a23*a11

       ! **** assign point (i1-js1,i2-js2,i3-js3) ****

       a1DotU=0.
       a2DotU=2.*( a21*u(i1,i2,i3,ex)+a22*u(i1,i2,i3,ey)+a23*u(i1,i2,i3,ez) )\
                -( a21*u(i1+is1,i2+is2,i3+is3,ex)+a22*u(i1+is1,i2+is2,i3+is3,ey)+a23*u(i1+is1,i2+is2,i3+is3,ez))
       a3DotU=0.
       #If #FORCING == "twilightZone"
         OGF3D(i1,i2,i3,t, uv0(0),uv0(1),uv0(2))
         OGF3D(i1-is1,i2-is2,i3-is3,t, uvm(0),uvm(1),uvm(2))
         OGF3D(i1+is1,i2+is2,i3+is3,t, uvp(0),uvp(1),uvp(2))

         a1DotU=a11*uvm(0)+a12*uvm(1)+a13*uvm(2)
         a3DotU=a31*uvm(0)+a32*uvm(1)+a33*uvm(2)

         a2DotU=a2DotU + 2.*( a21*(uvm(0)-uv0(0))+a22*(uvm(1)-uv0(1))+a23*(uvm(2)-uv0(2)) )\
                           -( a21*(uvm(0)-uvp(0))+a22*(uvm(1)-uvp(1))+a23*(uvm(2)-uvp(2)) )
       #End

       u(i1-is1,i2-is2,i3-is3,ex)=(a33*a1DotU*a22-a13*a3DotU*a22+a13*a32*a2DotU+a3DotU*a23*a12-a32*a23*a1DotU\
                                  -a33*a12*a2DotU)/detnt
       u(i1-is1,i2-is2,i3-is3,ey)=(-a23*a11*a3DotU+a23*a1DotU*a31+a11*a33*a2DotU+a13*a21*a3DotU-a1DotU*a33*a21\
                                   -a13*a2DotU*a31)/detnt
       u(i1-is1,i2-is2,i3-is3,ez)=(a11*a3DotU*a22-a11*a32*a2DotU-a12*a21*a3DotU+a12*a2DotU*a31-a1DotU*a31*a22\
                                  +a1DotU*a32*a21)/detnt

       ! **** assign point (i1-js1,i2-js2,i3-is3) ****

       a1DotU=2.*( a11*u(i1,i2,i3,ex)+a12*u(i1,i2,i3,ey)+a13*u(i1,i2,i3,ez) )\
                -( a11*u(i1+js1,i2+js2,i3+js3,ex)+a12*u(i1+js1,i2+js2,i3+js3,ey)+a13*u(i1+js1,i2+js2,i3+js3,ez))
       a2DotU=0.
       a3DotU=0.
       #If #FORCING == "twilightZone"
         OGF3D(i1-js1,i2-js2,i3-js3,t, uvm(0),uvm(1),uvm(2))
         OGF3D(i1+js1,i2+js2,i3+js3,t, uvp(0),uvp(1),uvp(2))

         a2DotU=a21*uvm(0)+a22*uvm(1)+a23*uvm(2)
         a3DotU=a31*uvm(0)+a32*uvm(1)+a33*uvm(2)

         a1DotU=a1DotU + 2.*( a11*(uvm(0)-uv0(0))+a12*(uvm(1)-uv0(1))+a13*(uvm(2)-uv0(2)) )\
                           -( a11*(uvm(0)-uvp(0))+a12*(uvm(1)-uvp(1))+a13*(uvm(2)-uvp(2)) )
       #End

       u(i1-js1,i2-js2,i3-js3,ex)=(a33*a1DotU*a22-a13*a3DotU*a22+a13*a32*a2DotU+a3DotU*a23*a12-a32*a23*a1DotU\
                                  -a33*a12*a2DotU)/detnt
       u(i1-js1,i2-js2,i3-js3,ey)=(-a23*a11*a3DotU+a23*a1DotU*a31+a11*a33*a2DotU+a13*a21*a3DotU-a1DotU*a33*a21\
                                   -a13*a2DotU*a31)/detnt
       u(i1-js1,i2-js2,i3-js3,ez)=(a11*a3DotU*a22-a11*a32*a2DotU-a12*a21*a3DotU+a12*a2DotU*a31-a1DotU*a31*a22\
                                  +a1DotU*a32*a21)/detnt


       #If #FORCING == "twilightZone"
        if( debug.gt.0 )then
         write(*,'(/," bce2: extended:(i1,i2,i3)=",3i5," is=",3i2," js=",3i2)') i1,i2,i3,is1,is2,is3,\
               js1,js2,js3

         OGF3D(i1,i2,i3,t, uv0(0),uv0(1),uv0(2))
         OGF3D(i1-is1,i2-is2,i3-is3,t, uvm(0),uvm(1),uvm(2))
         OGF3D(i1+is1,i2+is2,i3+is3,t, uvp(0),uvp(1),uvp(2))

         write(*,'(" bce2: extended: (i1,i2,i3)=",3i4," err(-1,0)",6e9.1)') i1,i2,i3,\
           u(i1-  is1,i2-  is2,i3-  is3,ex)-uvm(0),\
           u(i1-  is1,i2-  is2,i3-  is3,ey)-uvm(1),\
           u(i1-  is1,i2-  is2,i3-  is3,ez)-uvm(2)
         write(*,'(" bce2: true(-1,0),computed(-1,0) =",6f7.2)') uvm(0),uvm(1),uvm(2),\
           u(i1-  is1,i2-  is2,i3-  is3,ex),\
           u(i1-  is1,i2-  is2,i3-  is3,ey),\
           u(i1-  is1,i2-  is2,i3-  is3,ez)

         OGF3D(i1-js1,i2-js2,i3-js3,t, uvm(0),uvm(1),uvm(2))
         OGF3D(i1+js1,i2+js2,i3+js3,t, uvp(0),uvp(1),uvp(2))

         write(*,'(" bce2: extended: (i1,i2,i3)=",3i4," err(0,-1)",6e9.1)') i1,i2,i3,\
           u(i1-  js1,i2-  js2,i3-  js3,ex)-uvm(0),\
           u(i1-  js1,i2-  js2,i3-  js3,ey)-uvm(1),\
           u(i1-  js1,i2-  js2,i3-  js3,ez)-uvm(2)
         write(*,'(" bce2: true(0,-1),computed(0,-1) =",6f7.2)') uvm(0),uvm(1),uvm(2),\
           u(i1-  js1,i2-  js2,i3-  js3,ex),\
           u(i1-  js1,i2-  js2,i3-  js3,ey),\
           u(i1-  js1,i2-  js2,i3-  js3,ez)
        end if

         ! *** for now -- set solution to be exact ---
         
         !  OGF3D(i1-is1,i2-is2,i3-is3,t, uvm(0),uvm(1),uvm(2))
         !  OGF3D(i1-2*is1,i2-2*is2,i3-2*is3,t, uvm2(0),uvm2(1),uvm2(2))
         !  u(i1-  is1,i2-  is2,i3-  is3,ex)=uvm(0)
         !  u(i1-  is1,i2-  is2,i3-  is3,ey)=uvm(1)
         !  u(i1-  is1,i2-  is2,i3-  is3,ez)=uvm(2)
         !  u(i1-2*is1,i2-2*is2,i3-2*is3,ex)=uvm2(0)
         !  u(i1-2*is1,i2-2*is2,i3-2*is3,ey)=uvm2(1)
         !  u(i1-2*is1,i2-2*is2,i3-2*is3,ez)=uvm2(2)
 
         !  OGF3D(i1-js1,i2-js2,i3-js3,t, uvm(0),uvm(1),uvm(2))
         !  OGF3D(i1-2*js1,i2-2*js2,i3-2*js3,t, uvm2(0),uvm2(1),uvm2(2))
         !  u(i1-  js1,i2-  js2,i3-  js3,ex)=uvm(0)
         !  u(i1-  js1,i2-  js2,i3-  js3,ey)=uvm(1)
         !  u(i1-  js1,i2-  js2,i3-  js3,ez)=uvm(2)
         !  u(i1-2*js1,i2-2*js2,i3-2*js3,ex)=uvm2(0)
         !  u(i1-2*js1,i2-2*js2,i3-2*js3,ey)=uvm2(1)
         !  u(i1-2*js1,i2-2*js2,i3-2*js3,ez)=uvm2(2)



       #End

     end do ! end do i1
     end do ! end do i2
     end do ! end do i3

   else if( bc1.eq.dirichlet .or. bc2.eq.dirichlet )then

     do i3=n3a,n3b
     do i2=n2a,n2b
     do i1=n1a,n1b

      if( edgeDirection.ne.0 )then
        #If #FORCING == "twilightZone"
          OGF3D(i1-js1,i2,i3,t,g1,g2,g3)
        #End
        u(i1-js1,i2,i3,ex)=g1
        u(i1-js1,i2,i3,ey)=g2
        u(i1-js1,i2,i3,ez)=g3
      end if

      if( edgeDirection.ne.1 )then
        #If #FORCING == "twilightZone"
          OGF3D(i1,i2-js2,i3,t,g1,g2,g3)
        #End
        u(i1,i2-js2,i3,ex)=g1
        u(i1,i2-js2,i3,ey)=g2
        u(i1,i2-js2,i3,ez)=g3
      end if

      if( edgeDirection.ne.2 )then
        #If #FORCING == "twilightZone"
          OGF3D(i1,i2,i3-js3,t,g1,g2,g3)
        #End
        u(i1,i2,i3-js3,ex)=g1
        u(i1,i2,i3-js3,ey)=g2
        u(i1,i2,i3-js3,ez)=g3
      end if

     end do ! end do i1
     end do ! end do i2
     end do ! end do i3

    else if( bc1.le.0 .or. bc2.le.0 )then
      ! periodic or interpolation -- nothing to do
    else if( bc1.eq.planeWaveBoundaryCondition .or.\
             bc2.eq.planeWaveBoundaryCondition .or. \
           bc1.eq.symmetryBoundaryCondition .or. \
           bc2.eq.symmetryBoundaryCondition .or. \
           (bc1.ge.abcEM2 .and. bc1.le.lastBC) .or. \
           (bc2.ge.abcEM2 .and. bc2.le.lastBC))then
      ! do nothing
    else
      write(*,'("ERROR: unknown boundary conditions bc1,bc2=",2i3)') bc1,bc2
      ! unknown boundary conditions
      stop 8866
   end if



 #Else

  if( bc1.le.0 .or. bc2.le.0 )then
    ! periodic or interpolation -- nothing to do *wdh* 050820
  else 

   write(*,'(" assignEdges3d: unimplemented orderOfAccuracy =",i4)') orderOfAccuracy
   stop 123456
  end if

 #End 

 #Else
   write(*,'("unknown gridType")')
   stop 4578
 #End

 end do
 end do
 end do ! edge direction


 ! ************ assign corner points outside edges ***********************
 assignEdgeCorners(ORDER,GRIDTYPE,FORCING)

    
#endMacro

c =================================================================================================
c 4th-order Taylor approximation for 3D corners -- the truncation looks like dr^4*u_rrrr
c =================================================================================================
#defineMacro taylorOdd3dOrder4(cc,ks1,ks2,ks3,dr1,dr2,dr3,urr,uss,utt,urs,urt,ust) \
   2.*u(i1,i2,i3,cc)-u(i1+ks1,i2+ks2,i3+ks3,cc) \
      + ( (dr1)**2*urr+(dr2)**2*uss+(dr3)**2*utt+2.*(dr1)*(dr2)*urs+2.*(dr1)*(dr3)*urt+2.*(dr2)*(dr3)*ust )

c finish this 
c #defineMacro taylorOdd3dOrder6(cc,ks1,ks2,ks3,dr1,dr2,dr3,urr,uss,utt,urs,urt,ust) \
c    2.*u(i1,i2,i3,cc)-u(i1+ks1,i2+ks2,i3+ks3,cc) \
c       + ( (dr1)**2*urr+(dr2)**2*uss+(dr3)**2*utt+2.*(dr1)*(dr2)*urs+2.*(dr1)*(dr3)*urt+2.*(dr2)*(dr3)*ust )
c        + (1./12.)*( (dr1)**4*urrrr + (dr2)**4*ussss + (dr3)**4*utttt \
c                     + 4.*(dr1)**3*(dr2)*urrrs + 4.*(dr1)**3*(dr3)*urrrt + 4.*(dr1)*(dr2)**3*ursss
c                     + 4.*(dr1)*(dr3)**3*urttt + 4.*(dr2)*(dr3)**3*usttt + 4.*(dr2)**3*(dr3)*ussst \
c                     + 6.*(dr1)**2*(dr2)**2*urrss + 6.*(dr1)**2*(dr3)**2*urrtt + 6.*(drs)**2*(dr3)**2*usstt \
c    *check this*   + 12.*(dr1)**2*(dr2)*(dr3)*urrst + 12.*(dr1)*(dr2)**2*(dr3)*ursst + 12.*(dr1)*(dr2)*(dr3)**2*urstt
                                      ) 

c*****************************************************************************************************
c   Assign corners and edges in 3D
c 
c  ORDER: 2,4,6,8
c  GRIDTYPE: 
c  FORCING:
c 
c NOTE: tangential components have already been assigned on the extended boundary by assignBoundary3d
c*****************************************************************************************************
#beginMacro assignCorners3d(ORDER,GRIDTYPE,FORCING)

  numberOfGhostPoints=orderOfAccuracy/2


  ! Assign the edges
  assignEdges3d(ORDER,GRIDTYPE,FORCING)



  ! Finally assign points outside the vertices of the unit cube
  g1=0.
  g2=0.
  g3=0.

  do side3=0,1
  do side2=0,1
  do side1=0,1

   ! assign ghost values outside the corner (vertex)
   i1=gridIndexRange(side1,0)
   i2=gridIndexRange(side2,1)
   i3=gridIndexRange(side3,2)
   is1=1-2*side1
   is2=1-2*side2
   is3=1-2*side3

   if( boundaryCondition(side1,0).eq.perfectElectricalConductor .and.\
       boundaryCondition(side2,1).eq.perfectElectricalConductor .and.\
       boundaryCondition(side3,2).eq.perfectElectricalConductor )then


    #If #GRIDTYPE == "curvilinear" && #ORDER == "4"
      urr = urr2(i1,i2,i3,ex)
      uss = uss2(i1,i2,i3,ex)
      utt = utt2(i1,i2,i3,ex)
      urs = urs2(i1,i2,i3,ex)
      urt = urt2(i1,i2,i3,ex)
      ust = ust2(i1,i2,i3,ex)

      vrr = urr2(i1,i2,i3,ey)
      vss = uss2(i1,i2,i3,ey)
      vtt = utt2(i1,i2,i3,ey)
      vrs = urs2(i1,i2,i3,ey)
      vrt = urt2(i1,i2,i3,ey)
      vst = ust2(i1,i2,i3,ey)

      wrr = urr2(i1,i2,i3,ez)
      wss = uss2(i1,i2,i3,ez)
      wtt = utt2(i1,i2,i3,ez)
      wrs = urs2(i1,i2,i3,ez)
      wrt = urt2(i1,i2,i3,ez)
      wst = ust2(i1,i2,i3,ez)
    #End
    do m3=1,numberOfGhostPoints
    do m2=1,numberOfGhostPoints
    do m1=1,numberOfGhostPoints

      js1=is1*m1  ! shift to ghost point "m"
      js2=is2*m2
      js3=is3*m3

      dra=dr(0)*js1
      dsa=dr(1)*js2
      dta=dr(2)*js3

      #If #FORCING == "twilightZone" 
        OGF3D(i1    ,i2    ,i3    ,t, u0,v0,w0)
        OGF3D(i1-js1,i2-js2,i3-js3,t, um,vm,wm)
        OGF3D(i1+js1,i2+js2,i3+js3,t, up,vp,wp)
        g1=um-2.*u0+up
        g2=vm-2.*v0+vp
        g3=wm-2.*w0+wp
      #End

      #If #GRIDTYPE == "rectangular" || #ORDER == "2"
       u(i1-js1,i2-js2,i3-js3,ex)=2.*u(i1,i2,i3,ex)-u(i1+js1,i2+js2,i3+js3,ex)+g1
       u(i1-js1,i2-js2,i3-js3,ey)=2.*u(i1,i2,i3,ey)-u(i1+js1,i2+js2,i3+js3,ey)+g2
       u(i1-js1,i2-js2,i3-js3,ez)=2.*u(i1,i2,i3,ez)-u(i1+js1,i2+js2,i3+js3,ez)+g3
      #Else
       
       ! Use a taylor series -- only exact for polynomials up to degree=3 -- is this good enough?
       u(i1-js1,i2-js2,i3-js3,ex)=taylorOdd3dOrder4(ex,js1,js2,js3,dra,dsa,dta,urr,uss,utt,urs,urt,ust)
       u(i1-js1,i2-js2,i3-js3,ey)=taylorOdd3dOrder4(ey,js1,js2,js3,dra,dsa,dta,vrr,vss,vtt,vrs,vrt,vst)
       u(i1-js1,i2-js2,i3-js3,ez)=taylorOdd3dOrder4(ez,js1,js2,js3,dra,dsa,dta,wrr,wss,wtt,wrs,wrt,wst)
        
     #If #ORDER == "4"
      ! *** extrap for now ****
      ! j1=i1-js1
      ! j2=i2-js2
      ! j3=i3-js3
      ! u(j1,j2,j3,ex)=5.*u(j1+is1,j2+is2,j3+is3,ex)-10.*u(j1+2*is1,j2+2*is2,j3+2*is3,ex)+10.*u(j1+3*is1,j2+3*is2,j3+3*is3,ex)\
      !               -5.*u(j1+4*is1,j2+4*is2,j3+4*is3,ex)+u(j1+5*is1,j2+5*is2,j3+5*is3,ex)
      ! u(j1,j2,j3,ey)=5.*u(j1+is1,j2+is2,j3+is3,ey)-10.*u(j1+2*is1,j2+2*is2,j3+2*is3,ey)+10.*u(j1+3*is1,j2+3*is2,j3+3*is3,ey)\
      !               -5.*u(j1+4*is1,j2+4*is2,j3+4*is3,ey)+u(j1+5*is1,j2+5*is2,j3+5*is3,ey)
      ! u(j1,j2,j3,ez)=5.*u(j1+is1,j2+is2,j3+is3,ez)-10.*u(j1+2*is1,j2+2*is2,j3+2*is3,ez)+10.*u(j1+3*is1,j2+3*is2,j3+3*is3,ez)\
      !               -5.*u(j1+4*is1,j2+4*is2,j3+4*is3,ez)+u(j1+5*is1,j2+5*is2,j3+5*is3,ez)
     #End

       if( debug.gt.2 )then
         write(*,'("Corner point from taylor: ghost-pt=",3i4," errors=",3e10.2)') i1-js1,i2-js2,i3-js3,\
             u(i1-js1,i2-js2,i3-js3,ex)-um,u(i1-js1,i2-js2,i3-js3,ey)-vm,u(i1-js1,i2-js2,i3-js3,ez)-wm
         ! write(*,'(" corner: dra,dsa,dta=",3f6.3," urr,uss,utt,urs,urt,ust=",6f8.3)') dra,dsa,dta,\
         !    urr,uss,utt,urs,urt,ust
       end if
       #If #FORCING == "twilightZone"
         ! Set the solution to exact for now
         ! OGF3D(i1-js1,i2-js2,i3-js3,t, um,vm,wm)
         ! u(i1-js1,i2-js2,i3-js3,ex)=um
         ! u(i1-js1,i2-js2,i3-js3,ey)=vm
         ! u(i1-js1,i2-js2,i3-js3,ez)=wm
       #End
      #End

    end do
    end do
    end do

   else if( boundaryCondition(side1,0).eq.dirichlet .or.\
            boundaryCondition(side2,1).eq.dirichlet .or.\
            boundaryCondition(side3,2).eq.dirichlet )then

    do m3=1,numberOfGhostPoints
    do m2=1,numberOfGhostPoints
    do m1=1,numberOfGhostPoints

      js1=is1*m1  ! shift to ghost point "m"
      js2=is2*m2
      js3=is3*m3

      #If #FORCING == "twilightZone" 
        OGF3D(i1-js1,i2-js2,i3-js3,t, g1,g2,g3)
      #End
      u(i1-js1,i2-js2,i3-js3,ex)=g1
      u(i1-js1,i2-js2,i3-js3,ey)=g2
      u(i1-js1,i2-js2,i3-js3,ez)=g3

    end do
    end do
    end do

   else if( boundaryCondition(side1,0).le.0 .or.\
            boundaryCondition(side2,1).le.0 .or.\
            boundaryCondition(side3,2).le.0 )then
      ! one or more boundaries are periodic or interpolation -- nothing to do

   else if( boundaryCondition(side1,0).eq.planeWaveBoundaryCondition .or.\
            boundaryCondition(side2,1).eq.planeWaveBoundaryCondition .or.\
            boundaryCondition(side3,2).eq.planeWaveBoundaryCondition  .or. \
            boundaryCondition(side1,0).eq.symmetryBoundaryCondition .or. \
            boundaryCondition(side2,1).eq.symmetryBoundaryCondition .or. \
            boundaryCondition(side3,2).eq.symmetryBoundaryCondition .or. \
           (boundaryCondition(side1,0).ge.abcEM2 .and. boundaryCondition(side1,0).le.lastBC) .or. \
           (boundaryCondition(side2,1).ge.abcEM2 .and. boundaryCondition(side2,1).le.lastBC) .or. \
           (boundaryCondition(side3,2).ge.abcEM2 .and. boundaryCondition(side3,2).le.lastBC)  \
                      )then
     ! do nothing
   else
     write(*,'("ERROR: unknown boundary conditions at a 3D corner bc1,bc2,bc3=",2i3)') \
        boundaryCondition(side1,0),boundaryCondition(side2,1),boundaryCondition(side3,2)

     ! unknown boundary conditions
     stop 3399

   end if

  end do
  end do
  end do

#endMacro


c =================================================================================
c   Assign values in the corners in 2D
c
c  Set the normal component of the solution on the extended boundaries (points N in figure)
c  Set the corner points "C" -- odd symmetry about the corner
c              |
c              X
c              |
c        N--N--X--X----
c              |
c        C  C  N
c              |
c        C  C  N
c
c =================================================================================
#beginMacro assignCorners2d(ORDER,GRIDTYPE,FORCING)

  axis=0
  axisp1=1

  i3=gridIndexRange(0,2)
  numberOfGhostPoints=orderOfAccuracy/2


  do side1=0,1
  do side2=0,1
  if( boundaryCondition(side1,0).eq.perfectElectricalConductor .and.\
      boundaryCondition(side2,1).eq.perfectElectricalConductor )then

    i1=gridIndexRange(side1,0) ! (i1,i2,i3)=corner point
    i2=gridIndexRange(side2,1)

    ! write(*,'("bcOpt: assign corner side1,side2,i1,i2,i3=",2i2,3i5)') side1,side2,i1,i2,i3

    is1=1-2*side1
    is2=1-2*side2

    dra=dr(0)*is1
    dsa=dr(1)*is2

    g2a=0.
    ! For now assign second ghost line by symmetry 
    do m=1,numberOfGhostPoints

      js1=is1*m  ! shift to ghost point "m"
      js2=is2*m

      #If #GRIDTYPE == "curvilinear"
       ! *** there is no need to do this for orderOfAccuracy.eq.4 -- these are done below
      #If #ORDER == "2" 
       a11 =rsxy(i1,i2,i3,0,0)
       a12 =rsxy(i1,i2,i3,0,1)  

       aNorm=a11**2+a12**2  

       aDotUm=(a11*u(i1,i2-js2,i3,ex)+a12*u(i1,i2-js2,i3,ey))
       aDotUp=(a11*u(i1,i2+js2,i3,ex)+a12*u(i1,i2+js2,i3,ey))

       #If #FORCING == "twilightZone"
         call ogf2d(ep,xy(i1,i2-js2,i3,0),xy(i1,i2-js2,i3,1),t, um,vm,wm)
         call ogf2d(ep,xy(i1,i2+js2,i3,0),xy(i1,i2+js2,i3,1),t, up,vp,wp)
         aDotUp=aDotUp - ( a11*up + a12*vp ) 
         aDotUm=aDotUm - ( a11*um + a12*vm ) 
         g2a=wm-wp
       #Elif #FORCING == "none"
       #Else
         stop 6767
       #End

       u(i1,i2-js2,i3,ex)=u(i1,i2-js2,i3,ex)-(aDotUp+aDotUm)*a11/aNorm
       u(i1,i2-js2,i3,ey)=u(i1,i2-js2,i3,ey)-(aDotUp+aDotUm)*a12/aNorm
       u(i1,i2-js2,i3,hz)=u(i1,i2+js2,i3,hz)+g2a  ! Hz is even symmetry ***** fix this ****

       #If #FORCING == "twilightZone"
        if( debug.gt.0 )then
         write(*,'(" bcOpt: extended-boundary i1,i2-js2=",2i4," ex,err,ey,err=",4e10.2)') i1,i2-js2,\
                u(i1,i2-js2,i3,ex),u(i1,i2-js2,i3,ex)-um,u(i1,i2-js2,i3,ey)-vm
        end if
       #End

       a11 =rsxy(i1,i2,i3,1,0)
       a12 =rsxy(i1,i2,i3,1,1)  

       aNorm=a11**2+a12**2

       aDotUm=(a11*u(i1-js1,i2,i3,ex)+a12*u(i1-js1,i2,i3,ey))
       aDotUp=(a11*u(i1+js1,i2,i3,ex)+a12*u(i1+js1,i2,i3,ey))

       #If #FORCING == "twilightZone"
         call ogf2d(ep,xy(i1-js1,i2,i3,0),xy(i1-js1,i2,i3,1),t, um,vm,wm)
         call ogf2d(ep,xy(i1+js1,i2,i3,0),xy(i1+js1,i2,i3,1),t, up,vp,wp)
         aDotUp=aDotUp - ( a11*up + a12*vp ) 
         aDotUm=aDotUm - ( a11*um + a12*vm ) 
         g2a=wm-wp
       #Elif #FORCING == "none"
       #Else
         stop 6767
       #End

       u(i1-js1,i2,i3,ex)=u(i1-js1,i2,i3,ex)-(aDotUp+aDotUm)*a11/aNorm
       u(i1-js1,i2,i3,ey)=u(i1-js1,i2,i3,ey)-(aDotUp+aDotUm)*a12/aNorm
       u(i1-js1,i2,i3,hz)=u(i1+js1,i2,i3,hz)+g2a  ! Hz is even symmetry ***** fix this ****

      #End ! end orderOfAccuracy.eq.2

      #Elif #GRIDTYPE == "rectangular"

       #If #FORCING == "twilightZone"
         call ogf2d(ep,xy(i1,i2,i3,0),xy(i1,i2,i3,1),t, u0,v0,w0)

         call ogf2d(ep,xy(i1,i2-js2,i3,0),xy(i1,i2-js2,i3,1),t, um,vm,wm)
         call ogf2d(ep,xy(i1,i2+js2,i3,0),xy(i1,i2+js2,i3,1),t, up,vp,wp)
         g1=um-2.*u0+up
         g2=vm-vp
         g3=wm-wp

         u(i1,i2-js2,i3,ex)=2.*u(i1,i2,i3,ex)-u(i1,i2+js2,i3,ex) +g1
         u(i1,i2-js2,i3,ey)=u(i1,i2+js2,i3,ey)+g2
         u(i1,i2-js2,i3,hz)=u(i1,i2+js2,i3,hz)+g3


         call ogf2d(ep,xy(i1-js1,i2,i3,0),xy(i1-js1,i2,i3,1),t, um,vm,wm)
         call ogf2d(ep,xy(i1+js1,i2,i3,0),xy(i1+js1,i2,i3,1),t, up,vp,wp)
         g1=um-up
         g2=vm-2.*v0+vp
         g3=wm-wp

         u(i1-js1,i2,i3,ex)=u(i1+js1,i2,i3,ex) +g1
         u(i1-js1,i2,i3,ey)=2.*u(i1,i2,i3,ey)-u(i1+js1,i2,i3,ey) +g2
         u(i1-js1,i2,i3,hz)=u(i1+js1,i2,i3,hz)+g3

       #Elif #FORCING == "none"
         u(i1,i2-js2,i3,ex)=2.*u(i1,i2,i3,ex)-u(i1,i2+js2,i3,ex)
         u(i1,i2-js2,i3,ey)=u(i1,i2+js2,i3,ey)

         u(i1-js1,i2,i3,ex)=u(i1+js1,i2,i3,ex)
         u(i1-js1,i2,i3,ey)=2.*u(i1,i2,i3,ey)-u(i1+js1,i2,i3,ey)

         u(i1,i2-js2,i3,hz)=u(i1,i2+js2,i3,hz)  ! Hz is even symmetry
         u(i1-js1,i2,i3,hz)=u(i1+js1,i2,i3,hz)  ! Hz is even symmetry
       #Else
         stop 6767
       #End

      #Else
        stop 4578
      #End


    end do
    
    ! assign u(i1-is1,i2,i3,ev) and u(i1,i2-is2,i3,ev)
    #If #GRIDTYPE == "curvilinear"
      #If #ORDER == "4" 
        ! write(*,'("assign extended-curvilinear-order4 grid,side,axis=",i5,2i3)') grid,side,axis
        axis=0   ! for c11, c22, ...
        axisp1=1 
        assignExtendedBoundaries2dOrder4(FORCING,is1,is2)
      #End 
    #End  


    #If #GRIDTYPE == "curvilinear" || #FORCING == "twilightZone"

      ! dra=dr(0)  ! ** reset *** is this correct?
      ! dsa=dr(1)
      ghostValuesOutsideCorners2d(ORDER,GRIDTYPE,FORCING)

    #Else
      ! Now do corner (C) points
      u(i1-  is1,i2-  is2,i3,ex)=-u(i1+  is1,i2+  is2,i3,ex)
      u(i1-  is1,i2-  is2,i3,ey)=-u(i1+  is1,i2+  is2,i3,ey)
      u(i1-  is1,i2-  is2,i3,hz)= u(i1+  is1,i2+  is2,i3,hz)  ! Hz is even symmetry
  
      #If #ORDER == "4" 
        u(i1-2*is1,i2-  is2,i3,ex)=-u(i1+2*is1,i2+  is2,i3,ex)
        u(i1-  is1,i2-2*is2,i3,ex)=-u(i1+  is1,i2+2*is2,i3,ex)
        u(i1-2*is1,i2-2*is2,i3,ex)=-u(i1+2*is1,i2+2*is2,i3,ex)
  
        u(i1-2*is1,i2-  is2,i3,ey)=-u(i1+2*is1,i2+  is2,i3,ey)
        u(i1-  is1,i2-2*is2,i3,ey)=-u(i1+  is1,i2+2*is2,i3,ey)
        u(i1-2*is1,i2-2*is2,i3,ey)=-u(i1+2*is1,i2+2*is2,i3,ey)
  

        u(i1-2*is1,i2-  is2,i3,hz)= u(i1+2*is1,i2+  is2,i3,hz)
        u(i1-  is1,i2-2*is2,i3,hz)= u(i1+  is1,i2+2*is2,i3,hz)
        u(i1-2*is1,i2-2*is2,i3,hz)= u(i1+2*is1,i2+2*is2,i3,hz)
      #End
    #End

  else if( boundaryCondition(side1,0).ge.abcEM2 .and. boundaryCondition(side1,0).le.lastBC .and. \
           boundaryCondition(side2,1).ge.abcEM2 .and. boundaryCondition(side2,1).le.lastBC )then

    ! **** do nothing *** this is done in abcMaxwell

    ! i1=gridIndexRange(side1,0) ! (i1,i2,i3)=corner point
    ! i2=gridIndexRange(side2,1)

    ! write(*,'("bcOpt: assign ABC corner side1,side2,i1,i2,i3=",2i2,3i5)') side1,side2,i1,i2,i3

    ! is1=1-2*side1
    ! is2=1-2*side2

    ! u(i1-is1,i2,i3,ex)=                  u(i1+is1,i2,i3,ex)
    ! u(i1-is1,i2,i3,ey)=2.*u(i1,i2,i3,ey)-u(i1+is1,i2,i3,ey)
    ! u(i1-is1,i2,i3,hz)=                  u(i1+is1,i2,i3,hz)
    
    ! u(i1,i2-is2,i3,ex)=2.*u(i1,i2,i3,ex)-u(i1,i2+is2,i3,ex)
    ! u(i1,i2-is2,i3,ey)=                  u(i1,i2+is2,i3,ey)
    ! u(i1,i2-is2,i3,hz)=                  u(i1,i2+is2,i3,hz)
    
    ! u(i1-is1,i2-is2,i3,ex)=2.*u(i1,i2,i3,ex)-u(i1+is1,i2+is2,i3,ex)
    ! u(i1-is1,i2-is2,i3,ey)=2.*u(i1,i2,i3,ey)-u(i1+is1,i2+is2,i3,ey)
    ! u(i1-is1,i2-is2,i3,hz)=                  u(i1+is1,i2+is2,i3,hz)

    ! u(i1-is1,i2,i3,ex)=3.*u(i1,i2,i3,ex)-3.*u(i1+is1,i2,i3,ex)+u(i1+2*is1,i2,i3,ex)
    ! u(i1-is1,i2,i3,ey)=2.*u(i1,i2,i3,ey)-u(i1+is1,i2,i3,ey)
    ! u(i1-is1,i2,i3,hz)=3.*u(i1,i2,i3,hz)-3.*u(i1+is1,i2,i3,hz)+u(i1+2*is1,i2,i3,hz)

    ! u(i1,i2-is2,i3,ex)=2.*u(i1,i2,i3,ex)-u(i1,i2+is2,i3,ex)
    ! u(i1,i2-is2,i3,ey)=3.*u(i1,i2,i3,ey)-3.*u(i1,i2+is2,i3,ey)+u(i1,i2+2*is2,i3,ey)
    ! u(i1,i2-is2,i3,hz)=3.*u(i1,i2,i3,hz)-3.*u(i1,i2+is2,i3,hz)+u(i1,i2+2*is2,i3,hz)

    ! u(i1-is1,i2-is2,i3,ex)=2.*u(i1,i2,i3,ex)-u(i1+is1,i2+is2,i3,ex)
    ! u(i1-is1,i2-is2,i3,ey)=2.*u(i1,i2,i3,ey)-u(i1+is1,i2+is2,i3,ey)
    ! u(i1-is1,i2-is2,i3,hz)=3.*u(i1,i2,i3,hz)-3.*u(i1+is1,i2+is2,i3,hz)+u(i1+2*is1,i2+2*is2,i3,hz)

    #If #GRIDTYPE == "curvilinear"
      stop 773399
    #End

  end if
  end do
  end do

#endMacro





c ************************************************************************************
c  NAME : name of the subroutine
c  ORDER : order of accuracy
c ************************************************************************************
#beginMacro CORNERS_MAXWELL(NAME,ORDER)
 subroutine NAME( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
                  ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,\
                  gridIndexRange,dimension,u,f,mask,rsxy, xy,\
                  bc, boundaryCondition, ipar, rpar, ierr )
c ===================================================================================
c  Optimised Boundary conditions for Maxwell's Equations.
c
c  gridType : 0=rectangular, 1=curvilinear
c  useForcing : 1=use f for RHS to BC
c  side,axis : 0:1 and 0:2
c ===================================================================================

 implicit none

 integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,\
         n1a,n1b,n2a,n2b,n3a,n3b, ndc, bc,ierr

 real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
 real f(ndf1a:ndf1b,ndf2a:ndf2b,ndf3a:ndf3b,0:*)
 integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
 real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
 real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
 integer gridIndexRange(0:1,0:2),dimension(0:1,0:2)

 integer ipar(0:*),boundaryCondition(0:1,0:2)
 real rpar(0:*)

c     --- local variables ----
      
 real ep ! holds the pointer to the TZ function

 integer is1,is2,is3,js1,js2,js3,ks1,ks2,ks3,ls1,ls2,ls3,orderOfAccuracy,gridType,debug,grid,\
        side,axis,useForcing,ex,ey,ez,hx,hy,hz,useWhereMask,side1,side2,side3,m1,m2,m3,bc1,bc2,forcingOption

 real dt,kx,ky,kz,eps,mu,c,cc,twoPi,slowStartInterval,ssf,ssft,ssftt,ssfttt,tt

 real dr(0:2), dx(0:2), t, uv(0:5), uvm(0:5), uv0(0:5), uvp(0:5), uvm2(0:5), uvp2(0:5) 
 real uvmm(0:2),uvzm(0:2),uvpm(0:2)
 real uvmz(0:2),uvzz(0:2),uvpz(0:2)
 real uvmp(0:2),uvzp(0:2),uvpp(0:2)

 integer i10,i20,i30
 real jac3di(-2:2,-2:2,-2:2)

 integer orderOfExtrapolation
 logical setCornersToExact

 ! boundary conditions parameters
 #Include "bcDefineFortranInclude.h"

 integer rectangular,curvilinear
 parameter(\
     rectangular=0,\
     curvilinear=1)

  ! forcingOption takes on these values:
 integer noForcing,magneticSinusoidalPointSource,gaussianSource,twilightZoneForcing,planeWaveBoundaryForcing
 parameter(noForcing=0,\
           magneticSinusoidalPointSource=1,\
           gaussianSource=2,\
           twilightZoneForcing=3,\
           planeWaveBoundaryForcing=4)

 integer i1,i2,i3,j1,j2,j3,axisp1,axisp2,en1,et1,et2,hn1,ht1,ht2,numberOfGhostPoints
 integer extra,extra1a,extra1b,extra2a,extra2b,extra3a,extra3b

 real det,dra,dsa,dta,dxa,dya,dza

 real tau1,tau2,tau11,tau12,tau13, tau21,tau22,tau23 
 real tau11s,tau12s,tau13s, tau21s,tau22s,tau23s
 real tau11t,tau12t,tau13t, tau21t,tau22t,tau23t
 real tau1u,tau2u,tau1Up1,tau1Up2,tau1Up3,tau2Up1,tau2Up2,tau2Up3

 real tau1Dotu,tau2Dotu,tauU,tauUp1,tauUp2,tauUp3,ttu1,ttu2
 real ttu11,ttu12,ttu13, ttu21,ttu22,ttu23

 real DtTau1DotUvr,DtTau2DotUvr,DsTau1DotUvr,DsTau2DotUvr,tau1DotUtt,tau2DotUtt,Da1DotU,a1DotU
 real drA1DotDeltaU
! real tau1DotUvrs, tau2DotUvrs, tau1DotUvrt, tau2DotUvrt

 real gx1,gx2,g1a,g2a
 real g1,g2,g3
 real tauDotExtrap

 real jac,jacm1,jacp1,jacp2,jacm2,detnt

 real a11,a12,a13,a21,a22,a23,a31,a32,a33
 real a11r,a12r,a13r,a21r,a22r,a23r,a31r,a32r,a33r
 real a11s,a12s,a13s,a21s,a22s,a23s,a31s,a32s,a33s
 real a11t,a12t,a13t,a21t,a22t,a23t,a31t,a32t,a33t

 real a11rr,a12rr,a13rr,a21rr,a22rr,a23rr,a31rr,a32rr,a33rr
 real a11ss,a12ss,a13ss,a21ss,a22ss,a23ss,a31ss,a32ss,a33ss
 real a11tt,a12tt,a13tt,a21tt,a22tt,a23tt,a31tt,a32tt,a33tt
 real a11rs,a12rs,a13rs,a21rs,a22rs,a23rs,a31rs,a32rs,a33rs
 real a11rt,a12rt,a13rt,a21rt,a22rt,a23rt,a31rt,a32rt,a33rt
 real a11st,a12st,a13st,a21st,a22st,a23st,a31st,a32st,a33st

 real a11rrs,a12rrs,a13rrs,a21rrs,a22rrs,a23rrs,a31rrs,a32rrs,a33rrs
 real a11sss,a12sss,a13sss,a21sss,a22sss,a23sss,a31sss,a32sss,a33sss
 real a11rss,a12rss,a13rss,a21rss,a22rss,a23rss,a31rss,a32rss,a33rss
 real a11ttt,a12ttt,a13ttt,a21ttt,a22ttt,a23ttt,a31ttt,a32ttt,a33ttt
 real a11rtt,a12rtt,a13rtt,a21rtt,a22rtt,a23rtt,a31rtt,a32rtt,a33rtt
 real a11sst,a12sst,a13sst,a21sst,a22sst,a23sst,a31sst,a32sst,a33sst
 real a11stt,a12stt,a13stt,a21stt,a22stt,a23stt,a31stt,a32stt,a33stt

 real a11zm1,a12zm1,a13zm1,a21zm1,a22zm1,a23zm1,a31zm1,a32zm1,a33zm1
 real a11zp1,a12zp1,a13zp1,a21zp1,a22zp1,a23zp1,a31zp1,a32zp1,a33zp1
 real a11zm2,a12zm2,a13zm2,a21zm2,a22zm2,a23zm2,a31zm2,a32zm2,a33zm2
 real a11zp2,a12zp2,a13zp2,a21zp2,a22zp2,a23zp2,a31zp2,a32zp2,a33zp2

 real a11m,a12m,a13m,a21m,a22m,a23m,a31m,a32m,a33m
 real a11p,a12p,a13p,a21p,a22p,a23p,a31p,a32p,a33p

 real a11m1,a12m1,a13m1,a21m1,a22m1,a23m1,a31m1,a32m1,a33m1
 real a11p1,a12p1,a13p1,a21p1,a22p1,a23p1,a31p1,a32p1,a33p1
 real a11m2,a12m2,a13m2,a21m2,a22m2,a23m2,a31m2,a32m2,a33m2
 real a11p2,a12p2,a13p2,a21p2,a22p2,a23p2,a31p2,a32p2,a33p2

 real c11,c22,c33,c1,c2,c3
 real c11r,c22r,c33r,c1r,c2r,c3r
 real c11s,c22s,c33s,c1s,c2s,c3s
 real c11t,c22t,c33t,c1t,c2t,c3t

 real uex,uey,uez
 real ur,us,ut,urr, uss,utt,urs,urt,ust, urrr,usss,uttt,urrs,urss,urtt,usst,ustt, urrrr,ussss,urrss,urrrs,ursss
 real vr,vs,vt,vrr, vss,vtt,vrs,vrt,vst, vrrr,vsss,vttt,vrrs,vrss,vrtt,vsst,vstt, vrrrr,vssss,vrrss,vrrrs,vrsss
 real wr,ws,wt,wrr, wss,wtt,wrs,wrt,wst, wrrr,wsss,wttt,wrrs,wrss,wrtt,wsst,wstt, wrrrr,wssss,wrrss,wrrrs,wrsss

 real ursm,urrsm,vrsm,vrrsm, urrm,vrrm

 real uxx,uyy,uzz, vxx,vyy,vzz, wxx,wyy,wzz
 real uxxm2,uyym2,uzzm2, vxxm2,vyym2,vzzm2, wxxm2,wyym2,wzzm2
 real uxxm1,uyym1,uzzm1, vxxm1,vyym1,vzzm1, wxxm1,wyym1,wzzm1
 real uxxp1,uyyp1,uzzp1, vxxp1,vyyp1,vzzp1, wxxp1,wyyp1,wzzp1
 real uxxp2,uyyp2,uzzp2, vxxp2,vyyp2,vzzp2, wxxp2,wyyp2,wzzp2

 real cur,cvr,gI,gIa,gIII,gIV,gIVf

 real uTmTm,vTmTm,wTmTm
 real uTmTmr,vTmTmr,wTmTmr

 real b3u,b3v,b3w, b2u,b2v,b2w, b1u,b1v,b1w, bf,divtt
 real cw1,cw2,bfw2,fw1,fw2,fw3,fw4

 real f1um1,f1um2,f1vm1,f1vm2,f1wm1,f1wm2,f1f
 real f2um1,f2um2,f2vm1,f2vm2,f2wm1,f2wm2,f2f

 real cursu,cursv,cursw, cvrsu,cvrsv,cvrsw,  cwrsu,cwrsv,cwrsw
 real curtu,curtv,curtw, cvrtu,cvrtv,cvrtw,  cwrtu,cwrtv,cwrtw
 real furs,fvrs,fwrs, furt,fvrt,fwrt 
 real a1DotUvrsRHS,a1DotUvrtRHS, a1DotUvrssRHS,a1DotUvrttRHS
 real gIII1,gIII2,gIVf1,gIVf2,gIV1,gIV2

 real uLap,vLap,wLap,tau1DotLap,tau2DotLap
 real cgI,gIf

 real aNorm,aDotUp,aDotUm,ctlrr,ctlr,div,divc,divc2,tauDotLap,errLapex,errLapey,errLapez

 real aDot1,aDot2,aDotUm2,aDotUm1,aDotU,aDotUp1,aDotUp2,aDotUp3

 real xm,ym,x0,y0,z0,xp,yp,um,vm,wm,u0,v0,w0,up,vp,wp

 real tdu10,tdu01,tdu20,tdu02,gLu,gLv,utt00,vtt00,wtt00
 real cu10,cu01,cu20,cu02,cv10,cv01,cv20,cv02

 real maxDivc,maxTauDotLapu,maxExtrap,maxDr3aDotU,dr3aDotU,a1Doturss

#Include "declareJacobianDerivatives.h"

c real uxxx22r,uyyy22r,uxxx42r,uyyy42r,uxxxx22r,uyyyy22r, urrrr2,ussss2
 real urrrr2,ussss2
 real urrs4,urrt4,usst4,urss4,ustt4,urtt4
 real urrs2,urrt2,usst2,urss2,ustt2,urtt2

 real deltaFu,deltaFv,deltaFw,g1f,g2f
 real a1Dotu1,a3Dotu1, a1Dotu2,a3Dotu2, a2Dotu3,a3Dotu3, a2Dotu4,a3Dotu4 
 real a11c,a12c,a13c,a21c,a22c,a23c,a31c,a32c,a33c
 real a1a1,a1a2,a1a3,a2a2,a2a3,a3a3
 real b11,b12,b13, g11,g12,g13 
 real b21,b22,b23, g21,g22,g23 
 real b31,b32,b33, g31,g32,g33 
 real b41,b42,b43, g41,g42,g43 
 real cc11a,cc12a,cc13a,cc14a,cc15a,cc16a,cc11b,cc12b,cc13b,cc14b,cc15b,cc16b
 real cc21a,cc22a,cc23a,cc24a,cc25a,cc26a,cc21b,cc22b,cc23b,cc24b,cc25b,cc26b
 real dd11,dd12,dd13,dd14,dd21,dd22,dd23,dd24,dd31,dd32,dd33,dd34,dd41,dd42,dd43,dd44
 real f1x,f2x,f3x,f4x
 real deltaU,deltaV,deltaW
 real a1DotLu,a2DotLu
 real f1,f2,f3,f4, x1,x2,x3,x4

 integer edgeDirection,sidea,sideb,ms1,ms2,ms3
 real a1Dotu0,a2Dotu0,a1Doturr,a1Dotuss,a2Doturr,a2Dotuss,a3Doturrr,a3Dotusss,a3Doturss,a3Doturrs
 real a1Doturs,a2Doturs,a3Doturs, a2Dotu, a3Dotu, a3Dotur, a3Dotus
 real uLapr,vLapr,wLapr,uLaps,vLaps,wLaps
 real drb,dsb,dtb
 real ur0,us0,urr0,uss0,  urs0,vrs0,wrs0,urrs0,vrrs0,wrrs0,urss0,vrss0,wrss0

c     --- start statement function ----
 integer kd,m,n
 real rx,ry,rz,sx,sy,sz,tx,ty,tz
c include 'declareDiffOrder2f.h'
c include 'declareDiffOrder4f.h'
 declareDifferenceOrder2(u,RX)
 declareDifferenceOrder4(u,RX)

c.......statement functions for jacobian
 rx(i1,i2,i3)=rsxy(i1,i2,i3,0,0)
 ry(i1,i2,i3)=rsxy(i1,i2,i3,0,1)
 rz(i1,i2,i3)=rsxy(i1,i2,i3,0,2)
 sx(i1,i2,i3)=rsxy(i1,i2,i3,1,0)
 sy(i1,i2,i3)=rsxy(i1,i2,i3,1,1)
 sz(i1,i2,i3)=rsxy(i1,i2,i3,1,2)
 tx(i1,i2,i3)=rsxy(i1,i2,i3,2,0)
 ty(i1,i2,i3)=rsxy(i1,i2,i3,2,1)
 tz(i1,i2,i3)=rsxy(i1,i2,i3,2,2)


c     The next macro call will define the difference approximation statement functions
 defineDifferenceOrder2Components1(u,RX)
 defineDifferenceOrder4Components1(u,RX)

c define derivatives of rsxy
#Include "jacobianDerivatives.h"

c rsxyr2(i1,i2,i3,m,n)=(rsxy(i1+1,i2,i3,m,n)-rsxy(i1-1,i2,i3,m,n))*d12(0)
c rsxys2(i1,i2,i3,m,n)=(rsxy(i1,i2+1,i3,m,n)-rsxy(i1,i2-1,i3,m,n))*d12(1)
c
c rsxyx22(i1,i2,i3,m,n)= rx(i1,i2,i3)*rsxyr2(i1,i2,i3,m,n)+sx(i1,i2,i3)*rsxys2(i1,i2,i3,m,n)
c rsxyy22(i1,i2,i3,m,n)= ry(i1,i2,i3)*rsxyr2(i1,i2,i3,m,n)+sy(i1,i2,i3)*rsxys2(i1,i2,i3,m,n)
c
c rsxyr4(i1,i2,i3,m,n)=(8.*(rsxy(i1+1,i2,i3,m,n)-rsxy(i1-1,i2,i3,m,n))\
c                         -(rsxy(i1+2,i2,i3,m,n)-rsxy(i1-2,i2,i3,m,n)))*d14(0)
c rsxys4(i1,i2,i3,m,n)=(8.*(rsxy(i1,i2+1,i3,m,n)-rsxy(i1,i2-1,i3,m,n))\
c                         -(rsxy(i1,i2+2,i3,m,n)-rsxy(i1,i2-2,i3,m,n)))*d14(1)
c rsxyt4(i1,i2,i3,m,n)=(8.*(rsxy(i1,i2,i3+1,m,n)-rsxy(i1,i2,i3-1,m,n))\
c                         -(rsxy(i1,i2,i3+2,m,n)-rsxy(i1,i2,i3-2,m,n)))*d14(2)
c
c rsxyrr4(i1,i2,i3,m,n)=(-30.*rsxy(i1,i2,i3,m,n)+16.*(rsxy(i1+1,i2,i3,m,n)+rsxy(i1-1,i2,i3,m,n))\
c                           -(rsxy(i1+2,i2,i3,m,n)+rsxy(i1-2,i2,i3,m,n)) )*d24(0)
c
c rsxyss4(i1,i2,i3,m,n)=(-30.*rsxy(i1,i2,i3,m,n)+16.*(rsxy(i1,i2+1,i3,m,n)+rsxy(i1,i2-1,i3,m,n))\
c                           -(rsxy(i1,i2+2,i3,m,n)+rsxy(i1,i2-2,i3,m,n)) )*d24(1)
c
c rsxytt4(i1,i2,i3,m,n)=(-30.*rsxy(i1,i2,i3,m,n)+16.*(rsxy(i1,i2,i3+1,m,n)+rsxy(i1,i2,i3-1,m,n))\
c                           -(rsxy(i1,i2,i3+2,m,n)+rsxy(i1,i2,i3-2,m,n)) )*d24(2)
c
c rsxyrs4(i1,i2,i3,m,n)=(8.*(rsxyr4(i1,i2+1,i3,m,n)-rsxyr4(i1,i2-1,i3,m,n))\
c                          -(rsxyr4(i1,i2+2,i3,m,n)-rsxyr4(i1,i2-2,i3,m,n)))*d14(1)
c
c rsxyrt4(i1,i2,i3,m,n)=(8.*(rsxyr4(i1,i2,i3+1,m,n)-rsxyr4(i1,i2,i3-1,m,n))\
c                          -(rsxyr4(i1,i2,i3+2,m,n)-rsxyr4(i1,i2,i3-2,m,n)))*d14(2)
c
c rsxyst4(i1,i2,i3,m,n)=(8.*(rsxys4(i1,i2,i3+1,m,n)-rsxys4(i1,i2,i3-1,m,n))\
c                          -(rsxys4(i1,i2,i3+2,m,n)-rsxys4(i1,i2,i3-2,m,n)))*d14(2)
c
c rsxyx42(i1,i2,i3,m,n)= rx(i1,i2,i3)*rsxyr4(i1,i2,i3,m,n)+sx(i1,i2,i3)*rsxys4(i1,i2,i3,m,n)
c rsxyy42(i1,i2,i3,m,n)= ry(i1,i2,i3)*rsxyr4(i1,i2,i3,m,n)+sy(i1,i2,i3)*rsxys4(i1,i2,i3,m,n)
c
c
c ! check these again:
c rsxyxr42(i1,i2,i3,m,n)= rsxyr4(i1,i2,i3,0,0)*rsxyr4(i1,i2,i3,m,n) + rx(i1,i2,i3)*rsxyrr4(i1,i2,i3,m,n)\
c                        +rsxyr4(i1,i2,i3,1,0)*rsxys4(i1,i2,i3,m,n) + sx(i1,i2,i3)*rsxyrs4(i1,i2,i3,m,n)
c rsxyxs42(i1,i2,i3,m,n)= rsxys4(i1,i2,i3,0,0)*rsxyr4(i1,i2,i3,m,n) + rx(i1,i2,i3)*rsxyrs4(i1,i2,i3,m,n)\
c                        +rsxys4(i1,i2,i3,1,0)*rsxys4(i1,i2,i3,m,n) + sx(i1,i2,i3)*rsxyss4(i1,i2,i3,m,n)
c
c rsxyyr42(i1,i2,i3,m,n)= rsxyr4(i1,i2,i3,0,1)*rsxyr4(i1,i2,i3,m,n) + ry(i1,i2,i3)*rsxyrr4(i1,i2,i3,m,n)\
c                        +rsxyr4(i1,i2,i3,1,1)*rsxys4(i1,i2,i3,m,n) + sy(i1,i2,i3)*rsxyrs4(i1,i2,i3,m,n)
c rsxyys42(i1,i2,i3,m,n)= rsxys4(i1,i2,i3,0,1)*rsxyr4(i1,i2,i3,m,n) + ry(i1,i2,i3)*rsxyrs4(i1,i2,i3,m,n)\
c                        +rsxys4(i1,i2,i3,1,1)*rsxys4(i1,i2,i3,m,n) + sy(i1,i2,i3)*rsxyss4(i1,i2,i3,m,n)
c
c ! 3d versions -- check these again
c rsxyx43(i1,i2,i3,m,n)= rx(i1,i2,i3)*rsxyr4(i1,i2,i3,m,n)+sx(i1,i2,i3)*rsxys4(i1,i2,i3,m,n)\
c                       +tx(i1,i2,i3)*rsxyt4(i1,i2,i3,m,n)
c rsxyy43(i1,i2,i3,m,n)= ry(i1,i2,i3)*rsxyr4(i1,i2,i3,m,n)+sy(i1,i2,i3)*rsxys4(i1,i2,i3,m,n)\
c                       +ty(i1,i2,i3)*rsxyt4(i1,i2,i3,m,n)
c rsxyz43(i1,i2,i3,m,n)= rz(i1,i2,i3)*rsxyr4(i1,i2,i3,m,n)+sz(i1,i2,i3)*rsxys4(i1,i2,i3,m,n)\
c                       +tz(i1,i2,i3)*rsxyt4(i1,i2,i3,m,n)
c
c rsxyxr43(i1,i2,i3,m,n)= rsxyr4(i1,i2,i3,0,0)*rsxyr4(i1,i2,i3,m,n) + rx(i1,i2,i3)*rsxyrr4(i1,i2,i3,m,n)\
c                        +rsxyr4(i1,i2,i3,1,0)*rsxys4(i1,i2,i3,m,n) + sx(i1,i2,i3)*rsxyrs4(i1,i2,i3,m,n)\
c                        +rsxyr4(i1,i2,i3,2,0)*rsxyt4(i1,i2,i3,m,n) + tx(i1,i2,i3)*rsxyrt4(i1,i2,i3,m,n)
c
c rsxyxs43(i1,i2,i3,m,n)= rsxys4(i1,i2,i3,0,0)*rsxyr4(i1,i2,i3,m,n) + rx(i1,i2,i3)*rsxyrs4(i1,i2,i3,m,n)\
c                        +rsxys4(i1,i2,i3,1,0)*rsxys4(i1,i2,i3,m,n) + sx(i1,i2,i3)*rsxyss4(i1,i2,i3,m,n)\
c                        +rsxys4(i1,i2,i3,2,0)*rsxyt4(i1,i2,i3,m,n) + tx(i1,i2,i3)*rsxyst4(i1,i2,i3,m,n)
c
c rsxyxt43(i1,i2,i3,m,n)= rsxyt4(i1,i2,i3,0,0)*rsxyr4(i1,i2,i3,m,n) + rx(i1,i2,i3)*rsxyrt4(i1,i2,i3,m,n)\
c                        +rsxyt4(i1,i2,i3,1,0)*rsxys4(i1,i2,i3,m,n) + sx(i1,i2,i3)*rsxyst4(i1,i2,i3,m,n)\
c                        +rsxyt4(i1,i2,i3,2,0)*rsxyt4(i1,i2,i3,m,n) + tx(i1,i2,i3)*rsxytt4(i1,i2,i3,m,n)
c
c rsxyyr43(i1,i2,i3,m,n)= rsxyr4(i1,i2,i3,0,1)*rsxyr4(i1,i2,i3,m,n) + ry(i1,i2,i3)*rsxyrr4(i1,i2,i3,m,n)\
c                        +rsxyr4(i1,i2,i3,1,1)*rsxys4(i1,i2,i3,m,n) + sy(i1,i2,i3)*rsxyrs4(i1,i2,i3,m,n)\
c                        +rsxyr4(i1,i2,i3,2,1)*rsxyt4(i1,i2,i3,m,n) + ty(i1,i2,i3)*rsxyrt4(i1,i2,i3,m,n)
c
c rsxyys43(i1,i2,i3,m,n)= rsxys4(i1,i2,i3,0,1)*rsxyr4(i1,i2,i3,m,n) + ry(i1,i2,i3)*rsxyrs4(i1,i2,i3,m,n)\
c                        +rsxys4(i1,i2,i3,1,1)*rsxys4(i1,i2,i3,m,n) + sy(i1,i2,i3)*rsxyss4(i1,i2,i3,m,n)\
c                        +rsxys4(i1,i2,i3,2,1)*rsxyt4(i1,i2,i3,m,n) + ty(i1,i2,i3)*rsxyst4(i1,i2,i3,m,n)
c
c rsxyyt43(i1,i2,i3,m,n)= rsxyt4(i1,i2,i3,0,1)*rsxyr4(i1,i2,i3,m,n) + ry(i1,i2,i3)*rsxyrt4(i1,i2,i3,m,n)\
c                        +rsxyt4(i1,i2,i3,1,1)*rsxys4(i1,i2,i3,m,n) + sy(i1,i2,i3)*rsxyst4(i1,i2,i3,m,n)\
c                        +rsxyt4(i1,i2,i3,2,1)*rsxyt4(i1,i2,i3,m,n) + ty(i1,i2,i3)*rsxytt4(i1,i2,i3,m,n)
c
c rsxyzr43(i1,i2,i3,m,n)= rsxyr4(i1,i2,i3,0,2)*rsxyr4(i1,i2,i3,m,n) + rz(i1,i2,i3)*rsxyrr4(i1,i2,i3,m,n)\
c                        +rsxyr4(i1,i2,i3,1,2)*rsxys4(i1,i2,i3,m,n) + sz(i1,i2,i3)*rsxyrs4(i1,i2,i3,m,n)\
c                        +rsxyr4(i1,i2,i3,2,2)*rsxyt4(i1,i2,i3,m,n) + tz(i1,i2,i3)*rsxyrt4(i1,i2,i3,m,n)
c
c rsxyzs43(i1,i2,i3,m,n)= rsxys4(i1,i2,i3,0,2)*rsxyr4(i1,i2,i3,m,n) + rz(i1,i2,i3)*rsxyrs4(i1,i2,i3,m,n)\
c                        +rsxys4(i1,i2,i3,1,2)*rsxys4(i1,i2,i3,m,n) + sz(i1,i2,i3)*rsxyss4(i1,i2,i3,m,n)\
c                        +rsxys4(i1,i2,i3,2,2)*rsxyt4(i1,i2,i3,m,n) + tz(i1,i2,i3)*rsxyst4(i1,i2,i3,m,n)
c
c rsxyzt43(i1,i2,i3,m,n)= rsxyt4(i1,i2,i3,0,2)*rsxyr4(i1,i2,i3,m,n) + rz(i1,i2,i3)*rsxyrt4(i1,i2,i3,m,n)\
c                        +rsxyt4(i1,i2,i3,1,2)*rsxys4(i1,i2,i3,m,n) + sz(i1,i2,i3)*rsxyst4(i1,i2,i3,m,n)\
c                        +rsxyt4(i1,i2,i3,2,2)*rsxyt4(i1,i2,i3,m,n) + tz(i1,i2,i3)*rsxytt4(i1,i2,i3,m,n)
c

c$$$ uxxx22r(i1,i2,i3,kd)=(-2.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))+(u(i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
c$$$ uyyy22r(i1,i2,i3,kd)=(-2.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))+(u(i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
c$$$
c$$$ uxxxx22r(i1,i2,i3,kd)=(6.*u(i1,i2,i3,kd)-4.*(u(i1+1,i2,i3,kd)+u(i1-1,i2,i3,kd))\
c$$$                         +(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)) )/(dx(0)**4)
c$$$
c$$$ uyyyy22r(i1,i2,i3,kd)=(6.*u(i1,i2,i3,kd)-4.*(u(i1,i2+1,i3,kd)+u(i1,i2-1,i3,kd))\
c$$$                         +(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )/(dx(1)**4)

 urrrr2(i1,i2,i3,kd)=(6.*u(i1,i2,i3,kd)-4.*(u(i1+1,i2,i3,kd)+u(i1-1,i2,i3,kd))\
                         +(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)) )/(dr(0)**4)

 ussss2(i1,i2,i3,kd)=(6.*u(i1,i2,i3,kd)-4.*(u(i1,i2+1,i3,kd)+u(i1,i2-1,i3,kd))\
                         +(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )/(dr(1)**4)

! add these to the derivatives include file

 urrs2(i1,i2,i3,kd)=(urr2(i1,i2+1,i3,kd)-urr2(i1,i2-1,i3,kd))/(2.*dr(1))
 urrt2(i1,i2,i3,kd)=(urr2(i1,i2,i3+1,kd)-urr2(i1,i2,i3-1,kd))/(2.*dr(2))

 urss2(i1,i2,i3,kd)=(uss2(i1+1,i2,i3,kd)-uss2(i1-1,i2,i3,kd))/(2.*dr(0))
 usst2(i1,i2,i3,kd)=(uss2(i1,i2,i3+1,kd)-uss2(i1,i2,i3-1,kd))/(2.*dr(2))

 urtt2(i1,i2,i3,kd)=(utt2(i1+1,i2,i3,kd)-utt2(i1-1,i2,i3,kd))/(2.*dr(0))
 ustt2(i1,i2,i3,kd)=(utt2(i1,i2+1,i3,kd)-utt2(i1,i2-1,i3,kd))/(2.*dr(1))

! these are from diff.maple
 urrs4(i1,i2,i3,kd) = (u(i1-2,i2+2,i3,kd)+16*u(i1+1,i2-2,i3,kd)-30*u(i1,i2-2,i3,kd)+16*u(i1-1,i2-2,i3,kd)-u(i1+2,i2-2,i3,kd)-u(i1-2,i2-2,i3,kd)-16*u(i1+1,i2+2,i3,kd)+30*u(i1,i2+2,i3,kd)-16*u(i1-1,i2+2,i3,kd)+u(i1+2,i2+2,i3,kd)-240*u(i1,i2+1,i3,kd)-8*u(i1+2,i2+1,i3,kd)-8*u(i1-2,i2+1,i3,kd)-128*u(i1+1,i2-1,i3,kd)+240*u(i1,i2-1,i3,kd)-128*u(i1-1,i2-1,i3,kd)+8*u(i1+2,i2-1,i3,kd)+8*u(i1-2,i2-1,i3,kd)+128*u(i1-1,i2+1,i3,kd)+128*u(i1+1,i2+1,i3,kd))/(144.*dr(0)**2*dr(1))

 urrt4(i1,i2,i3,kd) = (30*u(i1,i2,i3+2,kd)-16*u(i1-1,i2,i3+2,kd)+u(i1+2,i2,i3+2,kd)-16*u(i1+1,i2,i3+2,kd)-30*u(i1,i2,i3-2,kd)+16*u(i1+1,i2,i3-2,kd)+u(i1-2,i2,i3+2,kd)-u(i1+2,i2,i3-2,kd)-u(i1-2,i2,i3-2,kd)+16*u(i1-1,i2,i3-2,kd)+128*u(i1+1,i2,i3+1,kd)-240*u(i1,i2,i3+1,kd)+128*u(i1-1,i2,i3+1,kd)-8*u(i1+2,i2,i3+1,kd)-8*u(i1-2,i2,i3+1,kd)-128*u(i1+1,i2,i3-1,kd)+240*u(i1,i2,i3-1,kd)-128*u(i1-1,i2,i3-1,kd)+8*u(i1+2,i2,i3-1,kd)+8*u(i1-2,i2,i3-1,kd))/(144.*dr(0)**2*dr(2))

 usst4(i1,i2,i3,kd) = (30*u(i1,i2,i3+2,kd)-30*u(i1,i2,i3-2,kd)+128*u(i1,i2+1,i3+1,kd)+128*u(i1,i2-1,i3+1,kd)-8*u(i1,i2+2,i3+1,kd)-8*u(i1,i2-2,i3+1,kd)-128*u(i1,i2+1,i3-1,kd)-128*u(i1,i2-1,i3-1,kd)+8*u(i1,i2+2,i3-1,kd)+8*u(i1,i2-2,i3-1,kd)-240*u(i1,i2,i3+1,kd)+240*u(i1,i2,i3-1,kd)+16*u(i1,i2+1,i3-2,kd)-16*u(i1,i2+1,i3+2,kd)-16*u(i1,i2-1,i3+2,kd)+u(i1,i2+2,i3+2,kd)+u(i1,i2-2,i3+2,kd)+16*u(i1,i2-1,i3-2,kd)-u(i1,i2+2,i3-2,kd)-u(i1,i2-2,i3-2,kd))/(144.*dr(1)**2*dr(2))

 urss4(i1,i2,i3,kd) = (-240*u(i1+1,i2,i3,kd)+240*u(i1-1,i2,i3,kd)-u(i1-2,i2+2,i3,kd)-8*u(i1+1,i2-2,i3,kd)+8*u(i1-1,i2-2,i3,kd)+u(i1+2,i2-2,i3,kd)-u(i1-2,i2-2,i3,kd)-8*u(i1+1,i2+2,i3,kd)+8*u(i1-1,i2+2,i3,kd)+u(i1+2,i2+2,i3,kd)-16*u(i1+2,i2+1,i3,kd)+16*u(i1-2,i2+1,i3,kd)+128*u(i1+1,i2-1,i3,kd)-128*u(i1-1,i2-1,i3,kd)-16*u(i1+2,i2-1,i3,kd)+16*u(i1-2,i2-1,i3,kd)-128*u(i1-1,i2+1,i3,kd)+128*u(i1+1,i2+1,i3,kd)-30*u(i1-2,i2,i3,kd)+30*u(i1+2,i2,i3,kd))/(144.*dr(1)**2*dr(0))

 ustt4(i1,i2,i3,kd) = (-30*u(i1,i2-2,i3,kd)+30*u(i1,i2+2,i3,kd)-240*u(i1,i2+1,i3,kd)+240*u(i1,i2-1,i3,kd)+128*u(i1,i2+1,i3+1,kd)-128*u(i1,i2-1,i3+1,kd)-16*u(i1,i2+2,i3+1,kd)+16*u(i1,i2-2,i3+1,kd)+128*u(i1,i2+1,i3-1,kd)-128*u(i1,i2-1,i3-1,kd)-16*u(i1,i2+2,i3-1,kd)+16*u(i1,i2-2,i3-1,kd)-8*u(i1,i2+1,i3-2,kd)-8*u(i1,i2+1,i3+2,kd)+8*u(i1,i2-1,i3+2,kd)+u(i1,i2+2,i3+2,kd)-u(i1,i2-2,i3+2,kd)+8*u(i1,i2-1,i3-2,kd)+u(i1,i2+2,i3-2,kd)-u(i1,i2-2,i3-2,kd))/(144.*dr(2)**2*dr(1))

 urtt4(i1,i2,i3,kd) = (-240*u(i1+1,i2,i3,kd)+240*u(i1-1,i2,i3,kd)+8*u(i1-1,i2,i3+2,kd)+u(i1+2,i2,i3+2,kd)-8*u(i1+1,i2,i3+2,kd)-8*u(i1+1,i2,i3-2,kd)-u(i1-2,i2,i3+2,kd)+u(i1+2,i2,i3-2,kd)-u(i1-2,i2,i3-2,kd)+8*u(i1-1,i2,i3-2,kd)+128*u(i1+1,i2,i3+1,kd)-128*u(i1-1,i2,i3+1,kd)-16*u(i1+2,i2,i3+1,kd)+16*u(i1-2,i2,i3+1,kd)+128*u(i1+1,i2,i3-1,kd)-128*u(i1-1,i2,i3-1,kd)-16*u(i1+2,i2,i3-1,kd)+16*u(i1-2,i2,i3-1,kd)-30*u(i1-2,i2,i3,kd)+30*u(i1+2,i2,i3,kd))/(144.*dr(2)**2*dr(0))

c     --- end statement functions ----

 ierr=0

 side                 =ipar(0)
 axis                 =ipar(1)
 n1a                  =ipar(2)
 n1b                  =ipar(3)
 n2a                  =ipar(4)
 n2b                  =ipar(5)
 n3a                  =ipar(6)
 n3b                  =ipar(7)
 gridType             =ipar(8)
 orderOfAccuracy      =ipar(9)
 orderOfExtrapolation =ipar(10)
 useForcing           =ipar(11)
 ex                   =ipar(12)
 ey                   =ipar(13)
 ez                   =ipar(14)
 hx                   =ipar(15)
 hy                   =ipar(16)
 hz                   =ipar(17)
 useWhereMask         =ipar(18)
 grid                 =ipar(19)
 debug                =ipar(20)
 forcingOption        =ipar(21)

 dx(0)                =rpar(0)
 dx(1)                =rpar(1)
 dx(2)                =rpar(2)
 dr(0)                =rpar(3)
 dr(1)                =rpar(4)
 dr(2)                =rpar(5)
 t                    =rpar(6)
 ep                   =rpar(7)
 dt                   =rpar(8)
 c                    =rpar(9)
 eps                  =rpar(10)
 mu                   =rpar(11)
 kx                   =rpar(12)  ! for plane wave forcing
 ky                   =rpar(13)
 kz                   =rpar(14)
 slowStartInterval    =rpar(15)
 
 dxa=dx(0)
 dya=dx(1)
 dza=dx(2)
    
c       We first assign the boundary values for the tangential
c       components and then assign the corner values      

 twoPi=8.*atan2(1.,1.)
 cc= c*sqrt( kx*kx+ky*ky )
 ! write(*,'(" ***assign corners: forcingOption=",i4," twoPi=",f18.14," cc=",f10.7)') forcingOption,twoPi,cc

 initializeBoundaryForcing(t,slowStartInterval)

 numberOfGhostPoints=orderOfAccuracy/2
 extra=orderOfAccuracy/2  ! assign the extended boundary
 beginLoopOverSides(extra,numberOfGhostPoints)
   if( nd.eq.2 )then   
     if( forcingOption.eq.planeWaveBoundaryForcing )then
       ! write(*,'(" ***assign corners:planeWaveBoundaryForcing: twoPi=",f18.14," cc=",f10.7)') twoPi,cc
       assignBoundary2d(planeWaveBoundaryForcing)
     else if( useForcing.eq.0 )then
       assignBoundary2d(none)
     else
       assignBoundary2d(twilightZone)
     end if

   else  
     if( forcingOption.eq.planeWaveBoundaryForcing )then
       ! write(*,'(" ***assign corners:planeWaveBoundaryForcing: twoPi=",f18.14," cc=",f10.7)') twoPi,cc
       assignBoundary3d(planeWaveBoundaryForcing)
     else if( useForcing.eq.0 )then
       assignBoundary3d(none)
     else
       assignBoundary3d(twilightZone)
     end if
   end if 
 endLoopOverSides()
 
 if( nd.eq.2 )then  
   if( gridType.eq.rectangular )then
     if( useForcing.eq.0 )then
       assignCorners2d(ORDER,rectangular,none)
     else
       assignCorners2d(ORDER,rectangular,twilightZone)
     end if
   else
     if( useForcing.eq.0 )then
       assignCorners2d(ORDER,curvilinear,none)
     else
       assignCorners2d(ORDER,curvilinear,twilightZone)
     end if
   end if
 else  
   if( gridType.eq.rectangular )then
     if( useForcing.eq.0 )then
       assignCorners3d(ORDER,rectangular,none)
     else
       assignCorners3d(ORDER,rectangular,twilightZone)
     end if
   else
     if( useForcing.eq.0 )then
       assignCorners3d(ORDER,curvilinear,none)
     else
       assignCorners3d(ORDER,curvilinear,twilightZone)
     end if
   end if
 end if

 return
 end
#endMacro


#beginMacro buildFile(NAME,ORDER)
#beginFile NAME.f
 CORNERS_MAXWELL(NAME,ORDER)
#endFile
#endMacro

      buildFile(cornersMxOrder2,2)
      buildFile(cornersMxOrder4,4)
      buildFile(cornersMxOrder6,6)
      buildFile(cornersMxOrder8,8)
