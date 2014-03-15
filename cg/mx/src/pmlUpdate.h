c ** NOTE: run pml.maple to take this file and generate the pml.h file
c     restart; read "pml.maple";
c ====================================================================================================
c  Fourth-order update on a side
c   OPTION : fullUpdate or partialUpdate
c   DIM : 2 or 3 space dimensions
c ====================================================================================================
#beginMacro update4xDIMd(va,wa, m,OPTION)

 
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
 #If "DIM" == "2"
   uyyyy=uyyyy22r(i1,i2,i3,m)
   uLapSq=uxxxx +2.*uxxyy +uyyyy
   uLapx = uxxx+uxyy
   uLapxx= uxxxx+uxxyy
   vLapx=vxxx+vxyy
 #Elif "DIM" == "3"
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

