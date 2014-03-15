! ******** This file generated from pmlUpdate.h using pml.maple ***** 

c ** NOTE: run pml.maple to take this file and generate the pml.h file
c     restart; read "pml.maple";
c ====================================================================================================
c  Fourth-order update on a side
c   OPTION : fullUpdate or partialUpdate
c   2 : 2 or 3 space dimensions
c ====================================================================================================
#beginMacro update4x2d(va,wa, m,OPTION)

 
 ! (u(n+1) - 2*u + u(n-1))/dt^2 = u_tt + (dt^4/12)*u_tttt + ...
 !
 ! [u(n)-u(n-1)]/dt = u_t + (dt/2)*u_tt + O(dt^2)
 !
 ! u_tt = Delta u - v_x - w
 ! u_tttt = Delta u_tt - v_xtt - wtt
 ! 

 v=va (i1,i2,i3,m)
 vx  = va x42r(i1,i2,i3,m)
 vxx = va xx42r(i1,i2,i3,m)
 vxxx= va xxx22r(i1,i2,i3,m)
 vxyy= va xyy22r(i1,i2,i3,m)

 w=wa(i1,i2,i3,m)
 wx  = wa x42r(i1,i2,i3,m)
 wxx = wa xx42r(i1,i2,i3,m)

 ux= ux42r(i1,i2,i3,m)
 uxx= uxx42r(i1,i2,i3,m)
 uxxx=uxxx22r(i1,i2,i3,m)
 uxyy=uxyy22r(i1,i2,i3,m)

 uxxxx=uxxxx22r(i1,i2,i3,m)
 uxxyy=uxxyy22r(i1,i2,i3,m)

 uLap = uLaplacian42r(i1,i2,i3,m)

 ! --- these change in 3D ---
 #If "2" == "2"
   uyyyy=uyyyy22r(i1,i2,i3,m)
   uLapSq=uxxxx +2.*uxxyy +uyyyy
   uLapx = uxxx+uxyy
   uLapxx= uxxxx+uxxyy
   vLapx=vxxx+vxyy
 #Elif "2" == "3"
   uLapSq=uLapSq23r(i1,i2,i3,m)
   uxxx=uxxx23r(i1,i2,i3,m)
   uxxxx=uxxx23r(i1,i2,i3,m)
   vxxx= va xxx23r(i1,i2,i3,m)

   uLapx = uxxx+uxyy+uxxx
   uLapxx= uxxxx+uxxyy+uxxxx
   vLapx=vxxx+vxyy+vxxx
 #Else
   stop 111999
 #End

 ut = (u(i1,i2,i3,m)-um(i1,i2,i3,m))/dt - (.5*dt*csq)*( uLap - vx - w )
 uxt = ( ux-umx42r(i1,i2,i3,m))/dt  - (.5*dt*csq)*( uLapx - vxx - wx )
 uxxt= (uxx-umxx42r(i1,i2,i3,m))/dt - (.5*dt*csq)*( uLapxx - vxxx - wxx )
 ! *** uxxxt= (uxxx-umxxx42r(i1,i2,i3,m))/dt   ! only need to first order in dt
 ! *** uxyyt= (uxyy-umxyy42r(i1,i2,i3,m))/dt   ! only need to first order in dt
 ! *** uxxxxt= (uxxxx-umxxxx42r(i1,i2,i3,m))/dt   ! only need to first order in dt
 ! *** uxxyyt= (uxxyy-umxxyy42r(i1,i2,i3,m))/dt   ! only need to first order in dt
 
 vt = sigma1*( -v + ux )
 vxt = sigma1*( -vx + uxx ) + sigma1x*( -v + ux )
 vxtt = sigma1*( -vxt + uxxt ) + sigma1x*( -vt + uxt )

 wt =  sigma1*( -w -vx + uxx )
 wtt = sigma1*( -wt -vxt + uxxt )

 #If #OPTION == "fullUpdate"
   un(i1,i2,i3,m)=2.*u(i1,i2,i3,m)-um(i1,i2,i3,m) \
                   + cdtsq*( uLap - vx -w ) \
                   + cdt4Over12*( uLapSq - vLapx - wa Laplacian42r(i1,i2,i3,m)  - vxtt - wtt ) 
 #Elif #OPTION == "partialUpdate"
   ! on an edge just add the other terms
   un(i1,i2,i3,m)=un(i1,i2,i3,m)\
                   + cdtsq*( - vx -w ) \
                   + cdt4Over12*( - vLapx - wa Laplacian42r(i1,i2,i3,m)  - vxtt - wtt ) 
 #Else
   stop 88437
 #End

 ! auxilliary variables       
 !  v_t = sigma1*( -v + u_x )
 !  (v(n+1)-v(n-1))/(2*dt) = v_t + (dt^2/3)*vttt
 !  vttt = sigma1*( -v_tt + u_xtt )

 uxtt = csq*( uLapx - vxx -wx )
 uxxtt = csq*( uLapxx - vxxx -wxx )

 ! new:
 ! *** uxttt = csq*( uxxxt +uxyyt - vxxt -wxt )
 ! *** uxxttt = csq*( uxxxxt+uxxyyt - vxxxt -wxxt )

 vtt = sigma1*( -vt + uxt )
 vttt = sigma1*( -vtt + uxtt )
 vtttt = 0. ! ***  sigma1*( -vttt + uxttt)

 ! (v(n+1)-v(n-1))/(2dt) = vt + (dt^2/6)*vttt
 ! va n(i1,i2,i3,m)=va m(i1,i2,i3,m)+(2.*dt)*( vt + (dt**2/6.)*vttt )
 va n(i1,i2,i3,m)=va(i1,i2,i3,m)+(dt)*( vt + dt*( .5*vtt + dt*( (1./6.)*vttt + dt*( (1./24.)*vtttt ) ) ) )
 ! write(*,'(" i1,i2=",2i3," vt,vtt,vttt=",3e10.2," v,ux,uxt,uxtt=",4e10.2)') i1,i2,vt,vtt,vttt,v,ux,uxt,uxtt

 ! w_t = sigma1*( -w -vx + uxx )

 wttt = sigma1*( -wtt -vxtt + uxxtt )
 wtttt = 0. ! **** sigma1*( -wttt -vxttt + uxxttt )
! wan(i1,i2,i3,m)=wam(i1,i2,i3,m)+(2.*dt)*( wt + (dt**2/6.)*wttt )
 wa n(i1,i2,i3,m)=wa(i1,i2,i3,m)+(dt)*(  wt + dt*( .5*wtt + dt*( (1./6.)*wttt + dt*( (1./24.)*wtttt ) ) ) )


#endMacro

! ******** This file generated from pmlUpdate.h using pml.maple ***** 

c ** NOTE: run pml.maple to take this file and generate the pml.h file
c     restart; read "pml.maple";
c ====================================================================================================
c  Fourth-order update on a side
c   OPTION : fullUpdate or partialUpdate
c   2 : 2 or 3 space dimensions
c ====================================================================================================
#beginMacro update4y2d(va,wa, m,OPTION)

 
 ! (u(n+1) - 2*u + u(n-1))/dt^2 = u_tt + (dt^4/12)*u_tttt + ...
 !
 ! [u(n)-u(n-1)]/dt = u_t + (dt/2)*u_tt + O(dt^2)
 !
 ! u_tt = Delta u - v_y - w
 ! u_tttt = Delta u_tt - v_ytt - wtt
 ! 

 v=va (i1,i2,i3,m)
 vy  = va y42r(i1,i2,i3,m)
 vyy = va yy42r(i1,i2,i3,m)
 vyyy= va yyy22r(i1,i2,i3,m)
 vxxy= va xxy22r(i1,i2,i3,m)

 w=wa(i1,i2,i3,m)
 wy  = wa y42r(i1,i2,i3,m)
 wyy = wa yy42r(i1,i2,i3,m)

 uy= uy42r(i1,i2,i3,m)
 uyy= uyy42r(i1,i2,i3,m)
 uyyy=uyyy22r(i1,i2,i3,m)
 uxxy=uxxy22r(i1,i2,i3,m)

 uyyyy=uyyyy22r(i1,i2,i3,m)
 uxxyy=uxxyy22r(i1,i2,i3,m)

 uLap = uLaplacian42r(i1,i2,i3,m)

 ! --- these change in 3D ---
 #If "2" == "2"
   uxxxx=uxxxx22r(i1,i2,i3,m)
   uLapSq=uyyyy +2.*uxxyy +uxxxx
   uLapy = uyyy+uxxy
   uLapyy= uyyyy+uxxyy
   vLapy=vyyy+vxxy
 #Elif "2" == "3"
   uLapSq=uLapSq23r(i1,i2,i3,m)
   uyyy=uyyy23r(i1,i2,i3,m)
   uyyyy=uyyy23r(i1,i2,i3,m)
   vyyy= va yyy23r(i1,i2,i3,m)

   uLapy = uyyy+uxxy+uyyy
   uLapyy= uyyyy+uxxyy+uyyyy
   vLapy=vyyy+vxxy+vyyy
 #Else
   stop 111999
 #End

 ut = (u(i1,i2,i3,m)-um(i1,i2,i3,m))/dt - (.5*dt*csq)*( uLap - vy - w )
 uyt = ( uy-umy42r(i1,i2,i3,m))/dt  - (.5*dt*csq)*( uLapy - vyy - wy )
 uyyt= (uyy-umyy42r(i1,i2,i3,m))/dt - (.5*dt*csq)*( uLapyy - vyyy - wyy )
 ! *** uyyyt= (uyyy-umyyy42r(i1,i2,i3,m))/dt   ! onlx need to first order in dt
 ! *** uxxyt= (uxxy-umxxy42r(i1,i2,i3,m))/dt   ! onlx need to first order in dt
 ! *** uyyyyt= (uyyyy-umyyyy42r(i1,i2,i3,m))/dt   ! onlx need to first order in dt
 ! *** uxxyyt= (uxxyy-umxxyy42r(i1,i2,i3,m))/dt   ! onlx need to first order in dt
 
 vt = sigma2*( -v + uy )
 vyt = sigma2*( -vy + uyy ) + sigma2y*( -v + uy )
 vytt = sigma2*( -vyt + uyyt ) + sigma2y*( -vt + uyt )

 wt =  sigma2*( -w -vy + uyy )
 wtt = sigma2*( -wt -vyt + uyyt )

 #If #OPTION == "fullUpdate"
   un(i1,i2,i3,m)=2.*u(i1,i2,i3,m)-um(i1,i2,i3,m) \
                   + cdtsq*( uLap - vy -w ) \
                   + cdt4Over12*( uLapSq - vLapy - wa Laplacian42r(i1,i2,i3,m)  - vytt - wtt ) 
 #Elif #OPTION == "partialUpdate"
   ! on an edge just add the other terms
   un(i1,i2,i3,m)=un(i1,i2,i3,m)\
                   + cdtsq*( - vy -w ) \
                   + cdt4Over12*( - vLapy - wa Laplacian42r(i1,i2,i3,m)  - vytt - wtt ) 
 #Else
   stop 88437
 #End

 ! auyilliarx variables       
 !  v_t = sigma2*( -v + u_y )
 !  (v(n+1)-v(n-1))/(2*dt) = v_t + (dt^2/3)*vttt
 !  vttt = sigma2*( -v_tt + u_ytt )

 uytt = csq*( uLapy - vyy -wy )
 uyytt = csq*( uLapyy - vyyy -wyy )

 ! new:
 ! *** uyttt = csq*( uyyyt +uxxyt - vyyt -wyt )
 ! *** uyyttt = csq*( uyyyyt+uxxyyt - vyyyt -wyyt )

 vtt = sigma2*( -vt + uyt )
 vttt = sigma2*( -vtt + uytt )
 vtttt = 0. ! ***  sigma2*( -vttt + uyttt)

 ! (v(n+1)-v(n-1))/(2dt) = vt + (dt^2/6)*vttt
 ! va n(i1,i2,i3,m)=va m(i1,i2,i3,m)+(2.*dt)*( vt + (dt**2/6.)*vttt )
 va n(i1,i2,i3,m)=va(i1,i2,i3,m)+(dt)*( vt + dt*( .5*vtt + dt*( (1./6.)*vttt + dt*( (1./24.)*vtttt ) ) ) )
 ! write(*,'(" i1,i2=",2i3," vt,vtt,vttt=",3e10.2," v,uy,uyt,uytt=",4e10.2)') i1,i2,vt,vtt,vttt,v,uy,uyt,uytt

 ! w_t = sigma2*( -w -vy + uyy )

 wttt = sigma2*( -wtt -vytt + uyytt )
 wtttt = 0. ! **** sigma2*( -wttt -vyttt + uyyttt )
! wan(i1,i2,i3,m)=wam(i1,i2,i3,m)+(2.*dt)*( wt + (dt**2/6.)*wttt )
 wa n(i1,i2,i3,m)=wa(i1,i2,i3,m)+(dt)*(  wt + dt*( .5*wtt + dt*( (1./6.)*wttt + dt*( (1./24.)*wtttt ) ) ) )


#endMacro

! ******** This file generated from pmlUpdate.h using pml.maple ***** 

c ** NOTE: run pml.maple to take this file and generate the pml.h file
c     restart; read "pml.maple";
c ====================================================================================================
c  Fourth-order update on a side
c   OPTION : fullUpdate or partialUpdate
c   3 : 2 or 3 space dimensions
c ====================================================================================================
#beginMacro update4x3d(va,wa, m,OPTION)

 
 ! (u(n+1) - 2*u + u(n-1))/dt^2 = u_tt + (dt^4/12)*u_tttt + ...
 !
 ! [u(n)-u(n-1)]/dt = u_t + (dt/2)*u_tt + O(dt^2)
 !
 ! u_tt = Delta u - v_x - w
 ! u_tttt = Delta u_tt - v_xtt - wtt
 ! 

 v=va (i1,i2,i3,m)
 vx  = va x43r(i1,i2,i3,m)
 vxx = va xx43r(i1,i2,i3,m)
 vxxx= va xxx23r(i1,i2,i3,m)
 vxyy= va xyy23r(i1,i2,i3,m)

 w=wa(i1,i2,i3,m)
 wx  = wa x43r(i1,i2,i3,m)
 wxx = wa xx43r(i1,i2,i3,m)

 ux= ux43r(i1,i2,i3,m)
 uxx= uxx43r(i1,i2,i3,m)
 uxxx=uxxx23r(i1,i2,i3,m)
 uxyy=uxyy23r(i1,i2,i3,m)

 uxxxx=uxxxx23r(i1,i2,i3,m)
 uxxyy=uxxyy23r(i1,i2,i3,m)

 uLap = uLaplacian43r(i1,i2,i3,m)

 ! --- these change in 3D ---
 #If "3" == "2"
   uyyyy=uyyyy23r(i1,i2,i3,m)
   uLapSq=uxxxx +2.*uxxyy +uyyyy
   uLapx = uxxx+uxyy
   uLapxx= uxxxx+uxxyy
   vLapx=vxxx+vxyy
 #Elif "3" == "3"
   uLapSq=uLapSq23r(i1,i2,i3,m)
   uxzz=uxzz23r(i1,i2,i3,m)
   uxxzz=uxzz23r(i1,i2,i3,m)
   vxzz= va xzz23r(i1,i2,i3,m)

   uLapx = uxxx+uxyy+uxzz
   uLapxx= uxxxx+uxxyy+uxxzz
   vLapx=vxxx+vxyy+vxzz
 #Else
   stop 111999
 #End

 ut = (u(i1,i2,i3,m)-um(i1,i2,i3,m))/dt - (.5*dt*csq)*( uLap - vx - w )
 uxt = ( ux-umx43r(i1,i2,i3,m))/dt  - (.5*dt*csq)*( uLapx - vxx - wx )
 uxxt= (uxx-umxx43r(i1,i2,i3,m))/dt - (.5*dt*csq)*( uLapxx - vxxx - wxx )
 ! *** uxxxt= (uxxx-umxxx43r(i1,i2,i3,m))/dt   ! only need to first order in dt
 ! *** uxyyt= (uxyy-umxyy43r(i1,i2,i3,m))/dt   ! only need to first order in dt
 ! *** uxxxxt= (uxxxx-umxxxx43r(i1,i2,i3,m))/dt   ! only need to first order in dt
 ! *** uxxyyt= (uxxyy-umxxyy43r(i1,i2,i3,m))/dt   ! only need to first order in dt
 
 vt = sigma1*( -v + ux )
 vxt = sigma1*( -vx + uxx ) + sigma1x*( -v + ux )
 vxtt = sigma1*( -vxt + uxxt ) + sigma1x*( -vt + uxt )

 wt =  sigma1*( -w -vx + uxx )
 wtt = sigma1*( -wt -vxt + uxxt )

 #If #OPTION == "fullUpdate"
   un(i1,i2,i3,m)=2.*u(i1,i2,i3,m)-um(i1,i2,i3,m) \
                   + cdtsq*( uLap - vx -w ) \
                   + cdt4Over12*( uLapSq - vLapx - wa Laplacian43r(i1,i2,i3,m)  - vxtt - wtt ) 
 #Elif #OPTION == "partialUpdate"
   ! on an edge just add the other terms
   un(i1,i2,i3,m)=un(i1,i2,i3,m)\
                   + cdtsq*( - vx -w ) \
                   + cdt4Over12*( - vLapx - wa Laplacian43r(i1,i2,i3,m)  - vxtt - wtt ) 
 #Else
   stop 88437
 #End

 ! auxilliary variables       
 !  v_t = sigma1*( -v + u_x )
 !  (v(n+1)-v(n-1))/(2*dt) = v_t + (dt^2/3)*vttt
 !  vttt = sigma1*( -v_tt + u_xtt )

 uxtt = csq*( uLapx - vxx -wx )
 uxxtt = csq*( uLapxx - vxxx -wxx )

 ! new:
 ! *** uxttt = csq*( uxxxt +uxyyt - vxxt -wxt )
 ! *** uxxttt = csq*( uxxxxt+uxxyyt - vxxxt -wxxt )

 vtt = sigma1*( -vt + uxt )
 vttt = sigma1*( -vtt + uxtt )
 vtttt = 0. ! ***  sigma1*( -vttt + uxttt)

 ! (v(n+1)-v(n-1))/(2dt) = vt + (dt^2/6)*vttt
 ! va n(i1,i2,i3,m)=va m(i1,i2,i3,m)+(2.*dt)*( vt + (dt**2/6.)*vttt )
 va n(i1,i2,i3,m)=va(i1,i2,i3,m)+(dt)*( vt + dt*( .5*vtt + dt*( (1./6.)*vttt + dt*( (1./24.)*vtttt ) ) ) )
 ! write(*,'(" i1,i2=",2i3," vt,vtt,vttt=",3e10.2," v,ux,uxt,uxtt=",4e10.2)') i1,i2,vt,vtt,vttt,v,ux,uxt,uxtt

 ! w_t = sigma1*( -w -vx + uxx )

 wttt = sigma1*( -wtt -vxtt + uxxtt )
 wtttt = 0. ! **** sigma1*( -wttt -vxttt + uxxttt )
! wan(i1,i2,i3,m)=wam(i1,i2,i3,m)+(2.*dt)*( wt + (dt**2/6.)*wttt )
 wa n(i1,i2,i3,m)=wa(i1,i2,i3,m)+(dt)*(  wt + dt*( .5*wtt + dt*( (1./6.)*wttt + dt*( (1./24.)*wtttt ) ) ) )


#endMacro

! ******** This file generated from pmlUpdate.h using pml.maple ***** 

c ** NOTE: run pml.maple to take this file and generate the pml.h file
c     restart; read "pml.maple";
c ====================================================================================================
c  Fourth-order update on a side
c   OPTION : fullUpdate or partialUpdate
c   3 : 2 or 3 space dimensions
c ====================================================================================================
#beginMacro update4y3d(va,wa, m,OPTION)

 
 ! (u(n+1) - 2*u + u(n-1))/dt^2 = u_tt + (dt^4/12)*u_tttt + ...
 !
 ! [u(n)-u(n-1)]/dt = u_t + (dt/2)*u_tt + O(dt^2)
 !
 ! u_tt = Delta u - v_y - w
 ! u_tttt = Delta u_tt - v_ytt - wtt
 ! 

 v=va (i1,i2,i3,m)
 vy  = va y43r(i1,i2,i3,m)
 vyy = va yy43r(i1,i2,i3,m)
 vyyy= va yyy23r(i1,i2,i3,m)
 vyzz= va yzz23r(i1,i2,i3,m)

 w=wa(i1,i2,i3,m)
 wy  = wa y43r(i1,i2,i3,m)
 wyy = wa yy43r(i1,i2,i3,m)

 uy= uy43r(i1,i2,i3,m)
 uyy= uyy43r(i1,i2,i3,m)
 uyyy=uyyy23r(i1,i2,i3,m)
 uyzz=uyzz23r(i1,i2,i3,m)

 uyyyy=uyyyy23r(i1,i2,i3,m)
 uyyzz=uyyzz23r(i1,i2,i3,m)

 uLap = uLaplacian43r(i1,i2,i3,m)

 ! --- these change in 3D ---
 #If "3" == "2"
   uzzzz=uzzzz23r(i1,i2,i3,m)
   uLapSq=uyyyy +2.*uyyzz +uzzzz
   uLapy = uyyy+uyzz
   uLapyy= uyyyy+uyyzz
   vLapy=vyyy+vyzz
 #Elif "3" == "3"
   uLapSq=uLapSq23r(i1,i2,i3,m)
   uxxy=uxxy23r(i1,i2,i3,m)
   uxxyy=uxxy23r(i1,i2,i3,m)
   vxxy= va xxy23r(i1,i2,i3,m)

   uLapy = uyyy+uyzz+uxxy
   uLapyy= uyyyy+uyyzz+uxxyy
   vLapy=vyyy+vyzz+vxxy
 #Else
   stop 111999
 #End

 ut = (u(i1,i2,i3,m)-um(i1,i2,i3,m))/dt - (.5*dt*csq)*( uLap - vy - w )
 uyt = ( uy-umy43r(i1,i2,i3,m))/dt  - (.5*dt*csq)*( uLapy - vyy - wy )
 uyyt= (uyy-umyy43r(i1,i2,i3,m))/dt - (.5*dt*csq)*( uLapyy - vyyy - wyy )
 ! *** uyyyt= (uyyy-umyyy43r(i1,i2,i3,m))/dt   ! onlz need to first order in dt
 ! *** uyzzt= (uyzz-umyzz43r(i1,i2,i3,m))/dt   ! onlz need to first order in dt
 ! *** uyyyyt= (uyyyy-umyyyy43r(i1,i2,i3,m))/dt   ! onlz need to first order in dt
 ! *** uyyzzt= (uyyzz-umyyzz43r(i1,i2,i3,m))/dt   ! onlz need to first order in dt
 
 vt = sigma2*( -v + uy )
 vyt = sigma2*( -vy + uyy ) + sigma2y*( -v + uy )
 vytt = sigma2*( -vyt + uyyt ) + sigma2y*( -vt + uyt )

 wt =  sigma2*( -w -vy + uyy )
 wtt = sigma2*( -wt -vyt + uyyt )

 #If #OPTION == "fullUpdate"
   un(i1,i2,i3,m)=2.*u(i1,i2,i3,m)-um(i1,i2,i3,m) \
                   + cdtsq*( uLap - vy -w ) \
                   + cdt4Over12*( uLapSq - vLapy - wa Laplacian43r(i1,i2,i3,m)  - vytt - wtt ) 
 #Elif #OPTION == "partialUpdate"
   ! on an edge just add the other terms
   un(i1,i2,i3,m)=un(i1,i2,i3,m)\
                   + cdtsq*( - vy -w ) \
                   + cdt4Over12*( - vLapy - wa Laplacian43r(i1,i2,i3,m)  - vytt - wtt ) 
 #Else
   stop 88437
 #End

 ! auyilliarz variables       
 !  v_t = sigma2*( -v + u_y )
 !  (v(n+1)-v(n-1))/(2*dt) = v_t + (dt^2/3)*vttt
 !  vttt = sigma2*( -v_tt + u_ytt )

 uytt = csq*( uLapy - vyy -wy )
 uyytt = csq*( uLapyy - vyyy -wyy )

 ! new:
 ! *** uyttt = csq*( uyyyt +uyzzt - vyyt -wyt )
 ! *** uyyttt = csq*( uyyyyt+uyyzzt - vyyyt -wyyt )

 vtt = sigma2*( -vt + uyt )
 vttt = sigma2*( -vtt + uytt )
 vtttt = 0. ! ***  sigma2*( -vttt + uyttt)

 ! (v(n+1)-v(n-1))/(2dt) = vt + (dt^2/6)*vttt
 ! va n(i1,i2,i3,m)=va m(i1,i2,i3,m)+(2.*dt)*( vt + (dt**2/6.)*vttt )
 va n(i1,i2,i3,m)=va(i1,i2,i3,m)+(dt)*( vt + dt*( .5*vtt + dt*( (1./6.)*vttt + dt*( (1./24.)*vtttt ) ) ) )
 ! write(*,'(" i1,i2=",2i3," vt,vtt,vttt=",3e10.2," v,uy,uyt,uytt=",4e10.2)') i1,i2,vt,vtt,vttt,v,uy,uyt,uytt

 ! w_t = sigma2*( -w -vy + uyy )

 wttt = sigma2*( -wtt -vytt + uyytt )
 wtttt = 0. ! **** sigma2*( -wttt -vyttt + uyyttt )
! wan(i1,i2,i3,m)=wam(i1,i2,i3,m)+(2.*dt)*( wt + (dt**2/6.)*wttt )
 wa n(i1,i2,i3,m)=wa(i1,i2,i3,m)+(dt)*(  wt + dt*( .5*wtt + dt*( (1./6.)*wttt + dt*( (1./24.)*wtttt ) ) ) )


#endMacro

! ******** This file generated from pmlUpdate.h using pml.maple ***** 

c ** NOTE: run pml.maple to take this file and generate the pml.h file
c     restart; read "pml.maple";
c ====================================================================================================
c  Fourth-order update on a side
c   OPTION : fullUpdate or partialUpdate
c   3 : 2 or 3 space dimensions
c ====================================================================================================
#beginMacro update4z3d(va,wa, m,OPTION)

 
 ! (u(n+1) - 2*u + u(n-1))/dt^2 = u_tt + (dt^4/12)*u_tttt + ...
 !
 ! [u(n)-u(n-1)]/dt = u_t + (dt/2)*u_tt + O(dt^2)
 !
 ! u_tt = Delta u - v_z - w
 ! u_tttt = Delta u_tt - v_ztt - wtt
 ! 

 v=va (i1,i2,i3,m)
 vz  = va z43r(i1,i2,i3,m)
 vzz = va zz43r(i1,i2,i3,m)
 vzzz= va zzz23r(i1,i2,i3,m)
 vxxz= va xxz23r(i1,i2,i3,m)

 w=wa(i1,i2,i3,m)
 wz  = wa z43r(i1,i2,i3,m)
 wzz = wa zz43r(i1,i2,i3,m)

 uz= uz43r(i1,i2,i3,m)
 uzz= uzz43r(i1,i2,i3,m)
 uzzz=uzzz23r(i1,i2,i3,m)
 uxxz=uxxz23r(i1,i2,i3,m)

 uzzzz=uzzzz23r(i1,i2,i3,m)
 uxxzz=uxxzz23r(i1,i2,i3,m)

 uLap = uLaplacian43r(i1,i2,i3,m)

 ! --- these change in 3D ---
 #If "3" == "2"
   uxxxx=uxxxx23r(i1,i2,i3,m)
   uLapSq=uzzzz +2.*uxxzz +uxxxx
   uLapz = uzzz+uxxz
   uLapzz= uzzzz+uxxzz
   vLapz=vzzz+vxxz
 #Elif "3" == "3"
   uLapSq=uLapSq23r(i1,i2,i3,m)
   uyyz=uyyz23r(i1,i2,i3,m)
   uyyzz=uyyz23r(i1,i2,i3,m)
   vyyz= va yyz23r(i1,i2,i3,m)

   uLapz = uzzz+uxxz+uyyz
   uLapzz= uzzzz+uxxzz+uyyzz
   vLapz=vzzz+vxxz+vyyz
 #Else
   stop 111999
 #End

 ut = (u(i1,i2,i3,m)-um(i1,i2,i3,m))/dt - (.5*dt*csq)*( uLap - vz - w )
 uzt = ( uz-umz43r(i1,i2,i3,m))/dt  - (.5*dt*csq)*( uLapz - vzz - wz )
 uzzt= (uzz-umzz43r(i1,i2,i3,m))/dt - (.5*dt*csq)*( uLapzz - vzzz - wzz )
 ! *** uzzzt= (uzzz-umzzz43r(i1,i2,i3,m))/dt   ! onlx need to first order in dt
 ! *** uxxzt= (uxxz-umxxz43r(i1,i2,i3,m))/dt   ! onlx need to first order in dt
 ! *** uzzzzt= (uzzzz-umzzzz43r(i1,i2,i3,m))/dt   ! onlx need to first order in dt
 ! *** uxxzzt= (uxxzz-umxxzz43r(i1,i2,i3,m))/dt   ! onlx need to first order in dt
 
 vt = sigma3*( -v + uz )
 vzt = sigma3*( -vz + uzz ) + sigma3z*( -v + uz )
 vztt = sigma3*( -vzt + uzzt ) + sigma3z*( -vt + uzt )

 wt =  sigma3*( -w -vz + uzz )
 wtt = sigma3*( -wt -vzt + uzzt )

 #If #OPTION == "fullUpdate"
   un(i1,i2,i3,m)=2.*u(i1,i2,i3,m)-um(i1,i2,i3,m) \
                   + cdtsq*( uLap - vz -w ) \
                   + cdt4Over12*( uLapSq - vLapz - wa Laplacian43r(i1,i2,i3,m)  - vztt - wtt ) 
 #Elif #OPTION == "partialUpdate"
   ! on an edge just add the other terms
   un(i1,i2,i3,m)=un(i1,i2,i3,m)\
                   + cdtsq*( - vz -w ) \
                   + cdt4Over12*( - vLapz - wa Laplacian43r(i1,i2,i3,m)  - vztt - wtt ) 
 #Else
   stop 88437
 #End

 ! auzilliarx variables       
 !  v_t = sigma3*( -v + u_z )
 !  (v(n+1)-v(n-1))/(2*dt) = v_t + (dt^2/3)*vttt
 !  vttt = sigma3*( -v_tt + u_ztt )

 uztt = csq*( uLapz - vzz -wz )
 uzztt = csq*( uLapzz - vzzz -wzz )

 ! new:
 ! *** uzttt = csq*( uzzzt +uxxzt - vzzt -wzt )
 ! *** uzzttt = csq*( uzzzzt+uxxzzt - vzzzt -wzzt )

 vtt = sigma3*( -vt + uzt )
 vttt = sigma3*( -vtt + uztt )
 vtttt = 0. ! ***  sigma3*( -vttt + uzttt)

 ! (v(n+1)-v(n-1))/(2dt) = vt + (dt^2/6)*vttt
 ! va n(i1,i2,i3,m)=va m(i1,i2,i3,m)+(2.*dt)*( vt + (dt**2/6.)*vttt )
 va n(i1,i2,i3,m)=va(i1,i2,i3,m)+(dt)*( vt + dt*( .5*vtt + dt*( (1./6.)*vttt + dt*( (1./24.)*vtttt ) ) ) )
 ! write(*,'(" i1,i2=",2i3," vt,vtt,vttt=",3e10.2," v,uz,uzt,uztt=",4e10.2)') i1,i2,vt,vtt,vttt,v,uz,uzt,uztt

 ! w_t = sigma3*( -w -vz + uzz )

 wttt = sigma3*( -wtt -vztt + uzztt )
 wtttt = 0. ! **** sigma3*( -wttt -vzttt + uzzttt )
! wan(i1,i2,i3,m)=wam(i1,i2,i3,m)+(2.*dt)*( wt + (dt**2/6.)*wttt )
 wa n(i1,i2,i3,m)=wa(i1,i2,i3,m)+(dt)*(  wt + dt*( .5*wtt + dt*( (1./6.)*wttt + dt*( (1./24.)*wtttt ) ) ) )


#endMacro

