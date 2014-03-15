c *******************************************************************************
c   Integrate the PML Absorbing boundary condition equations
c
c  NOTE: Run "pml.maple" to generate "pml.h" from "pmlUpdate.h"; pml.h is included in this file.
c *******************************************************************************

c These next include files will define the macros that will define the difference approximations
c The actual macro is called below
#Include "defineDiffOrder2f.h"
#Include "defineDiffOrder4f.h"


#beginMacro beginLoops()
do i3=m3a,m3b
do i2=m2a,m2b
do i1=m1a,m1b
#endMacro

#beginMacro endLoops()
end do
end do
end do
#endMacro

c$$$      wx2= wx1 + (dt)*sigma1*( 1.5*( -wx1 + u1.xx() -vx1.x() ) -.5*( -wx2 + u2.xx() -vx2.x() ) );
c$$$	  wy2= wy1 + (dt)*sigma2*( 1.5*( -wy1 + u1.yy() -vy1.y() ) -.5*( -wy2 + u2.yy() -vy2.y() ) );
c$$$
c$$$	  vx2= vx1 + (dt)*sigma1*( 1.5*( -vx1 + u1.x() ) -.5*( -vx2 + u2.x() ) );
c$$$	  vy2= vy1 + (dt)*sigma2*( 1.5*( -vy1 + u1.y() ) -.5*( -vy2 + u2.y() ) );
c$$$
c$$$	  u2=2.*u1-u2  + (dtSquared)*( u1.laplacian() - vx1.x() - wx1   - vy1.y() - wy1   );

c ====================================================================================================
c  Update a variable on a side
c ====================================================================================================
#beginMacro update(v,w,ex,LAPLACIAN,XD,XXD)

 un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) \
                 + cdtsq*( u ## LAPLACIAN(i1,i2,i3,ex) - v XD(i1,i2,i3,ex) -w(i1,i2,i3,ex) )
! write(*,'(" i=",3i3," um,u,un,uLap,cdtsq=",5e10.2)') i1,i2,i3,um(i1,i2,i3,ex),u(i1,i2,i3,ex),un(i1,i2,i3,ex),\
!        u ## LAPLACIAN(i1,i2,i3,ex), cdtsq
 ! auxilliary variables       
 v n(i1,i2,i3,ex)=v(i1,i2,i3,ex)+sigma*dt*( 1.5*( -v(i1,i2,i3,ex) +  u XD(i1,i2,i3,ex))\
                                          -0.5*(-v m(i1,i2,i3,ex) + um XD(i1,i2,i3,ex)) )

 w n(i1,i2,i3,ex)=w(i1,i2,i3,ex)+sigma*dt*( 1.5*( -w(i1,i2,i3,ex)+  u ## XXD(i1,i2,i3,ex)-v   XD(i1,i2,i3,ex))\
                                          -0.5*(-w m(i1,i2,i3,ex)+ um ## XXD(i1,i2,i3,ex)-v m XD(i1,i2,i3,ex)) )
#endMacro

c ====================================================================================================
c  Update a variable in a 2D corner or along an edge in 3d
c ====================================================================================================
#beginMacro updateCorner(vx,wx, vy,wy, sigmaA,sigmaB, ex, LAPLACIAN,XD,XXD,YD,YYD,XYD)
 un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) \
                 + cdtsq*( u ## LAPLACIAN(i1,i2,i3,ex) - vx XD(i1,i2,i3,ex) -wx(i1,i2,i3,ex) \
                                                       - vy YD(i1,i2,i3,ex) -wy(i1,i2,i3,ex) )
 ! auxilliary variables       
 vx n(i1,i2,i3,ex)=vx(i1,i2,i3,ex)\
                 +sigmaA*dt*( 1.5*(  -vx(i1,i2,i3,ex) +  u ## XD(i1,i2,i3,ex))\
                             -0.5*(-vx m(i1,i2,i3,ex) + um ## XD(i1,i2,i3,ex)) )

 wx n(i1,i2,i3,ex)=wx(i1,i2,i3,ex)\
    +sigmaA*dt*( 1.5*(  -wx(i1,i2,i3,ex)+  u ## XXD(i1,i2,i3,ex) - vx XD(i1,i2,i3,ex))\
                -0.5*(-wx m(i1,i2,i3,ex)+ um ## XXD(i1,i2,i3,ex)-vx m XD(i1,i2,i3,ex)) )

 vy n(i1,i2,i3,ex)=vy(i1,i2,i3,ex)\
                 +sigmaB*dt*( 1.5*(  -vy(i1,i2,i3,ex) +  u ## YD(i1,i2,i3,ex))\
                             -0.5*(-vy m(i1,i2,i3,ex) + um ## YD(i1,i2,i3,ex)) )

 wy n(i1,i2,i3,ex)=wy(i1,i2,i3,ex)\
    +sigmaB*dt*( 1.5*( -wy(i1,i2,i3,ex)+  u ## YYD(i1,i2,i3,ex)   -vy YD(i1,i2,i3,ex))\
                -0.5*(-wy m(i1,i2,i3,ex)+ um ## YYD(i1,i2,i3,ex)-vy m YD(i1,i2,i3,ex)) )

#endMacro


#beginMacro updateVertex(vx,wx, vy,wy, vz,wz, ex, DD )
 beginLoops()
   getSigma1(sigma1)
   getSigma2(sigma2)
   getSigma3(sigma3)

   un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) \
                 + cdtsq*( u laplacian DD(i1,i2,i3,ex) - vx x DD(i1,i2,i3,ex) -wx(i1,i2,i3,ex) \
                                                       - vy y DD(i1,i2,i3,ex) -wy(i1,i2,i3,ex) \
                                                       - vz z DD(i1,i2,i3,ex) -wz(i1,i2,i3,ex) )
 ! auxilliary variables       
 vx n(i1,i2,i3,ex)=vx(i1,i2,i3,ex)\
                 +sigma1*dt*( 1.5*(  -vx(i1,i2,i3,ex) +  u x DD(i1,i2,i3,ex))\
                             -0.5*(-vx m(i1,i2,i3,ex) + um x DD(i1,i2,i3,ex)) )

 wx n(i1,i2,i3,ex)=wx(i1,i2,i3,ex)\
    +sigma1*dt*( 1.5*(  -wx(i1,i2,i3,ex)+  u xx DD(i1,i2,i3,ex) - vx   x DD(i1,i2,i3,ex))\
                -0.5*(-wx m(i1,i2,i3,ex)+ um xx DD(i1,i2,i3,ex) - vx m x DD(i1,i2,i3,ex)) )

 vy n(i1,i2,i3,ex)=vy(i1,i2,i3,ex)\
                 +sigma2*dt*( 1.5*(-vy  (i1,i2,i3,ex) +  u y DD(i1,i2,i3,ex))\
                             -0.5*(-vy m(i1,i2,i3,ex) + um y DD(i1,i2,i3,ex)) )

 wy n(i1,i2,i3,ex)=wy(i1,i2,i3,ex)\
    +sigma2*dt*( 1.5*(-wy  (i1,i2,i3,ex)+ u  yy DD(i1,i2,i3,ex) -vy   y DD(i1,i2,i3,ex))\
                -0.5*(-wy m(i1,i2,i3,ex)+ um yy DD(i1,i2,i3,ex) -vy m y DD(i1,i2,i3,ex)) )

 vz n(i1,i2,i3,ex)=vz(i1,i2,i3,ex)\
                 +sigma3*dt*( 1.5*(-vz  (i1,i2,i3,ex) +  u z DD(i1,i2,i3,ex))\
                             -0.5*(-vz m(i1,i2,i3,ex) + um z DD(i1,i2,i3,ex)) )

 wz n(i1,i2,i3,ex)=wz(i1,i2,i3,ex)\
    +sigma3*dt*( 1.5*(-wz  (i1,i2,i3,ex)+ u  zz DD(i1,i2,i3,ex) -vz   z DD(i1,i2,i3,ex))\
                -0.5*(-wz m(i1,i2,i3,ex)+ um zz DD(i1,i2,i3,ex) -vz m z DD(i1,i2,i3,ex)) )

 endLoops()
#endMacro

c ================================================================================================
c ================================================================================================
#beginMacro updateMx(v,w,LAPLACIAN,XD,XXD,OPTION)
 #If #OPTION == "scalar" 
   update(v,w,ex,LAPLACIAN,XD,XXD)
 #Else
   update(v,w,ex,LAPLACIAN,XD,XXD)
   update(v,w,ey,LAPLACIAN,XD,XXD)
   update(v,w,ez,LAPLACIAN,XD,XXD)
 #End

#endMacro

c ====================================================================================================
c  Fourth-order update on a side
c ====================================================================================================

#Include "pml.h"

c$$$#beginMacro update4x(m)
c$$$
c$$$ 
c$$$ ! (u(n+1) - 2*u + u(n-1))/dt^2 = u_tt + (dt^4/12)*u_tttt + ...
c$$$ !
c$$$ ! [u(n)-u(n-1)]/dt = u_t + (dt/2)*u_tt + O(dt^2)
c$$$ !
c$$$ ! u_tt = Delta u - v_x - w
c$$$ ! u_tttt = Delta u_tt - v_xtt - wtt
c$$$ ! 
c$$$
c$$$ v=va(i1,i2,i3,m)
c$$$ vx  = vax42r(i1,i2,i3,m)
c$$$ vxx = vaxx42r(i1,i2,i3,m)
c$$$ vxxx= vaxxx22r(i1,i2,i3,m)
c$$$ vxyy= vaxyy22r(i1,i2,i3,m)
c$$$
c$$$ w=wa(i1,i2,i3,m)
c$$$ wx  = wax42r(i1,i2,i3,m)
c$$$ wxx = waxx42r(i1,i2,i3,m)
c$$$
c$$$ ux= ux42r(i1,i2,i3,m)
c$$$ uxx= uxx42r(i1,i2,i3,m)
c$$$ uxxx=uxxx22r(i1,i2,i3,m)
c$$$ uxyy=uxyy22r(i1,i2,i3,m)
c$$$
c$$$ uxxxx=uxxxx22r(i1,i2,i3,m)
c$$$ uxxyy=uxxyy22r(i1,i2,i3,m)
c$$$ uyyyy=uyyyy22r(i1,i2,i3,m)
c$$$
c$$$ uLap = uLaplacian42r(i1,i2,i3,m)
c$$$ uLapSq=uxxxx +2.*uxxyy +uyyyy
c$$$
c$$$ ut = (u(i1,i2,i3,m)-um(i1,i2,i3,m))/dt - (.5*dt*csq)*( uLap - vx - w )
c$$$ uxt = ( ux-umx42r(i1,i2,i3,m))/dt  - (.5*dt*csq)*( uxxx+uxyy - vxx - wx )
c$$$ uxxt= (uxx-umxx42r(i1,i2,i3,m))/dt - (.5*dt*csq)*( uxxxx+uxxyy - vxxx - wxx )
c$$$ 
c$$$ vxt = sigma1*( -vx + uxx ) + sigma1x*( -v + ux )
c$$$ vxtt = sigma1**2*( vx-uxx ) +sigma1*uxt + sigma1x*ut
c$$$ wt =  sigma1*( -w -vx + uxx )
c$$$ wtt = sigma1*( -wt -vxt + uxxt )
c$$$
c$$$ un(i1,i2,i3,m)=2.*u(i1,i2,i3,m)-um(i1,i2,i3,m) \
c$$$                 + cdtsq*( uLap - vx -w ) \
c$$$                 + cdt4Over12*( uLapSq - vxxx - vxyy - waLaplacian42r(i1,i2,i3,ex)  - vxtt - wtt ) 
c$$$
c$$$ ! auxilliary variables       
c$$$ !  v_t = sigma1*( -v + u_x )
c$$$ !  (v(n+1)-v(n-1))/(2*dt) = v_t + (dt^2/3)*vttt
c$$$ !  vttt = sigma1*( -v_tt + u_xtt )
c$$$
c$$$ uxtt = csq*( uxxx+uxyy - vxx -wx )
c$$$ uxxtt = csq*( uxxxx+uxxyy - vxxx -wxx )
c$$$
c$$$ vt = sigma1*( -v + ux )
c$$$ vtt = sigma1*( -vt + uxt )
c$$$ vttt = sigma1*( -vtt + uxtt )
c$$$ ! (v(n+1)-v(n-1))/(2dt) = vt + (dt^2/6)*vttt
c$$$ ! van(i1,i2,i3,m)=vam(i1,i2,i3,m)+(2.*dt)*( vt + (dt**2/6.)*vttt )
c$$$ van(i1,i2,i3,m)=va(i1,i2,i3,m)+(dt)*( vt + .5*dt*vtt + (dt**2/6.)*vttt )
c$$$ ! write(*,'(" i1,i2=",2i3," vt,vtt,vttt=",3e10.2," v,ux,uxt,uxtt=",4e10.2)') i1,i2,vt,vtt,vttt,v,ux,uxt,uxtt
c$$$
c$$$ ! w_t = sigma1*( -w -vx + uxx )
c$$$
c$$$ wt = sigma1*( -w -vx + uxx )
c$$$ wtt = sigma1*( -wt -vxt + uxxt  )
c$$$ wttt = sigma1*( -wtt -vxtt + uxxtt )
c$$$! wan(i1,i2,i3,m)=wam(i1,i2,i3,m)+(2.*dt)*( wt + (dt**2/6.)*wttt )
c$$$ wan(i1,i2,i3,m)=wa(i1,i2,i3,m)+(dt)*( wt +.5*dt*wtt + (dt**2/6.)*wttt )
c$$$
c$$$
c$$$#endMacro
c$$$
c$$$c ====================================================================================================
c$$$c  Fourth-order update on a side
c$$$c ====================================================================================================
c$$$#beginMacro update4xNew(m)
c$$$
c$$$ 
c$$$ ! (u(n+1) - 2*u + u(n-1))/dt^2 = u_tt + (dt^4/12)*u_tttt + ...
c$$$ !
c$$$ ! [u(n)-u(n-1)]/dt = u_t + (dt/2)*u_tt + O(dt^2)
c$$$ !
c$$$ ! u_tt = Delta u - v_x - w
c$$$ ! u_tttt = Delta u_tt - v_xtt - wtt
c$$$ ! 
c$$$
c$$$ v=va(i1,i2,i3,m)
c$$$ vx  = vax42r(i1,i2,i3,m)
c$$$ vxx = vaxx42r(i1,i2,i3,m)
c$$$ vxxx= vaxxx22r(i1,i2,i3,m)
c$$$ vxyy= vaxyy22r(i1,i2,i3,m)
c$$$
c$$$ w=wa(i1,i2,i3,m)
c$$$ wx  = wax42r(i1,i2,i3,m)
c$$$ wxx = waxx42r(i1,i2,i3,m)
c$$$
c$$$ ux= ux42r(i1,i2,i3,m)
c$$$ uxx= uxx42r(i1,i2,i3,m)
c$$$ uxxx=uxxx22r(i1,i2,i3,m)
c$$$ uxyy=uxyy22r(i1,i2,i3,m)
c$$$
c$$$ uxxxx=uxxxx22r(i1,i2,i3,m)
c$$$ uxxyy=uxxyy22r(i1,i2,i3,m)
c$$$ uyyyy=uyyyy22r(i1,i2,i3,m)
c$$$
c$$$ uLap = uLaplacian42r(i1,i2,i3,m)
c$$$ uLapSq=uxxxx +2.*uxxyy +uyyyy
c$$$
c$$$ ut = (u(i1,i2,i3,m)-um(i1,i2,i3,m))/dt - (.5*dt*csq)*( uLap - vx - w )
c$$$ uxt = ( ux-umx42r(i1,i2,i3,m))/dt  - (.5*dt*csq)*( uxxx+uxyy - vxx - wx )
c$$$ uxxt= (uxx-umxx42r(i1,i2,i3,m))/dt - (.5*dt*csq)*( uxxxx+uxxyy - vxxx - wxx )
c$$$ 
c$$$ vxt = sigma1*( -vx + uxx ) + sigma1x*( -v + ux )
c$$$ vxtt = sigma1**2*( vx-uxx ) +sigma1*uxt + sigma1x*ut
c$$$ wt =  sigma1*( -w -vx + uxx )
c$$$ wtt = sigma1*( -wt -vxt + uxxt )
c$$$
c$$$ un(i1,i2,i3,m)=2.*u(i1,i2,i3,m)-um(i1,i2,i3,m) \
c$$$                 + cdtsq*( uLap - vx -w ) \
c$$$                 + cdt4Over12*( uLapSq - vxxx - vxyy - waLaplacian42r(i1,i2,i3,ex)  - vxtt - wtt ) 
c$$$
c$$$ ! auxilliary variables       
c$$$ !  v_t = sigma1*( -v + u_x )
c$$$ !  (v(n+1)-v(n-1))/(2*dt) = v_t + (dt^2/3)*vttt
c$$$ !  vttt = sigma1*( -v_tt + u_xtt )
c$$$
c$$$ uxtt = csq*( uxxx+uxyy - vxx -wx )
c$$$ uxxtt = csq*( uxxxx+uxxyy - vxxx -wxx )
c$$$
c$$$ vt = sigma1*( ux )   ! = f 
c$$$ vtt = sigma1*( vt  + uxt )  ! = sigma*f + f' 
c$$$ vttt = sigma1*( sigma*(vt +2.*uxt) + uxtt )  ! = sigma^2*f + 2 sigma*f' + f''
c$$$ ! (v(n+1)-v(n-1))/(2dt) = vt + (dt^2/6)*vttt
c$$$ ! van(i1,i2,i3,m)=vam(i1,i2,i3,m)+sigma1*(2.*dt)*( vt + (dt**2/6.)*vttt )
c$$$
c$$$ expsdt=exp(-sigma1*dt)
c$$$ van(i1,i2,i3,m)=expsdt*( va(i1,i2,i3,m)+(dt)*( vt + .5*dt*vtt + (dt**2/6.)*vttt ) )
c$$$
c$$$ ! write(*,'(" i1,i2=",2i3," vt,vtt,vttt=",3e10.2," v,ux,uxt,uxtt=",4e10.2)') i1,i2,vt,vtt,vttt,v,ux,uxt,uxtt
c$$$
c$$$ ! w_t = sigma1*( -w -vx + uxx )
c$$$
c$$$ wt = sigma1*( -vx + uxx )
c$$$ wtt = sigma1*( wt -vxt + uxxt  )
c$$$ wttt = sigma1*( sigma*( wt +2.*(-vxt+uxxt)) -vxtt + uxxtt )
c$$$! wan(i1,i2,i3,m)=wam(i1,i2,i3,m)+sigma1*(2.*dt)*( wt + (dt**2/6.)*wttt )
c$$$ wan(i1,i2,i3,m)=expsdt*( wa(i1,i2,i3,m)+(dt)*( wt +.5*dt*wtt + (dt**2/6.)*wttt ) )
c$$$
c$$$
c$$$#endMacro



#beginMacro setBox( l1a,l1b,l2a,l2b,l3a,l3b,boxType,side1,side2,side3 )
  box(0,nb)=l1a
  box(1,nb)=l1b
  box(2,nb)=l2a
  box(3,nb)=l2b
  box(4,nb)=l3a
  box(5,nb)=l3b
  box(6,nb)=boxType
  box(7,nb)=side1
  box(8,nb)=side2
  box(9,nb)=side3
  nb=nb+1
#endMacro


#beginMacro getSigma1(sigma)
 ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
 ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
 xx=(i1-i1a)/real(i1b-i1a)
 sigma = layerStrength*xx**power
#endMacro

#beginMacro getSigma2(sigma)
 ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
 ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
 yy=(i2-i2a)/real(i2b-i2a)
 sigma = layerStrength*yy**power
#endMacro

#beginMacro getSigma3(sigma)
 ! zScale=xy(i1,i2,i3b,2)-xy(i1,i2,i3a,2)
 ! zz=(xy(i1,i2,i3,2)-xy(i1,i2,i3a,2))/zScale
 zz=(i3-i3a)/real(i3b-i3a)
 sigma = layerStrength*zz**power
#endMacro

#beginMacro getSigma1x(sigma,sigmax)
 ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
 ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
 xx=(i1-i1a)/real(i1b-i1a)
 sigma = layerStrength*xx**power
 sigmax = (2*side1-1)*power*layerStrength*xx**(power-1)
#endMacro

#beginMacro getSigma2y(sigma,sigmay)
 ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
 ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
 yy=(i2-i2a)/real(i2b-i2a)
 sigma = layerStrength*yy**power
 sigmay = (2*side2-1)*power*layerStrength*yy**(power-1)
#endMacro

#beginMacro getSigma3z(sigma,sigmaz)
 ! zScale=xy(i1,i2,i3b,2)-xy(i1,i2,i3a,2)
 ! zz=(xy(i1,i2,i3,2)-xy(i1,i2,i3a,2))/zScale
 zz=(i3-i3a)/real(i3b-i3a)
 sigma = layerStrength*zz**power
 sigmaz = (2*side3-1)*power*layerStrength*zz**(power-1)
#endMacro

      subroutine pmlSolidMechanics( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
                               ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,\
                               gridIndexRange, \
                               um, u, un, \
                               ndra1a,ndra1b,ndra2a,ndra2b,ndra3a,ndra3b,\
                               vram, vra, vran, wram, wra, wran, \
                               ndrb1a,ndrb1b,ndrb2a,ndrb2b,ndrb3a,ndrb3b,\
                               vrbm, vrb, vrbn, wrbm, wrb, wrbn, \
                               ndsa1a,ndsa1b,ndsa2a,ndsa2b,ndsa3a,ndsa3b,\
                               vsam, vsa, vsan, wsam, wsa, wsan, \
                               ndsb1a,ndsb1b,ndsb2a,ndsb2b,ndsb3a,ndsb3b,\
                               vsbm, vsb, vsbn, wsbm, wsb, wsbn, \
                               ndta1a,ndta1b,ndta2a,ndta2b,ndta3a,ndta3b,\
                               vtam, vta, vtan, wtam, wta, wtan, \
                               ndtb1a,ndtb1b,ndtb2a,ndtb2b,ndtb3a,ndtb3b,\
                               vtbm, vtb, vtbn, wtbm, wtb, wtbn, \
                               f,mask,rsxy, xy,\
                               bc, boundaryCondition, ipar, rpar, ierr )
c ===================================================================================
c  Absorbing boundary conditions for Solid Mechanics
c
c  gridType : 0=rectangular, 1=curvilinear
c  useForcing : 1=use f for RHS to BC
c  side,axis : 0:1 and 0:2
c
c  u : solution at time t
c  um : time t-dt
c  un : on output the solution at time t+dt
c
c  The PML variables are stored on the ghost points of the six faces of the cube
c
c   vra, vrab : ??
c
c   v1a, v1b : left and right side (r=0,1)
c     dimensions:  (ng=numberOfGhostPoints, n1a=gridIndexRange(0,0), etc)
c       v1a(nd1a:n1a+ng-1,nd2a:nd2b,nd3a:nd3b,0:*)
c       v1b(n1b-ng+1:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
c   v2a, v2b : bottom and top (s=0,1)
c       v2a(nd1a:nd1b,nd2a:n2a+ng-1,nd3a:nd3b,0:*)
c   v3a, v3b : front and back (t=0,1)
c
c   v1a,v1am,v1an w1a,w1am,w1an : v and w at times t,t-dt,t+dt for the left side (r=0)
c ===================================================================================

      implicit none

      integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,\
              n1a,n1b,n2a,n2b,n3a,n3b, ndc, bc,ierr

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      real un(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      real um(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)

      integer ndra1a,ndra1b,ndra2a,ndra2b,ndra3a,ndra3b
      real vra(ndra1a:ndra1b,ndra2a:ndra2b,ndra3a:ndra3b,0:*)
      real vran(ndra1a:ndra1b,ndra2a:ndra2b,ndra3a:ndra3b,0:*)
      real vram(ndra1a:ndra1b,ndra2a:ndra2b,ndra3a:ndra3b,0:*)

      integer ndrb1a,ndrb1b,ndrb2a,ndrb2b,ndrb3a,ndrb3b
      real vrb(ndrb1a:ndrb1b,ndrb2a:ndrb2b,ndrb3a:ndrb3b,0:*)
      real vrbn(ndrb1a:ndrb1b,ndrb2a:ndrb2b,ndrb3a:ndrb3b,0:*)
      real vrbm(ndrb1a:ndrb1b,ndrb2a:ndrb2b,ndrb3a:ndrb3b,0:*)

      integer ndsa1a,ndsa1b,ndsa2a,ndsa2b,ndsa3a,ndsa3b
      real vsa(ndsa1a:ndsa1b,ndsa2a:ndsa2b,ndsa3a:ndsa3b,0:*)
      real vsan(ndsa1a:ndsa1b,ndsa2a:ndsa2b,ndsa3a:ndsa3b,0:*)
      real vsam(ndsa1a:ndsa1b,ndsa2a:ndsa2b,ndsa3a:ndsa3b,0:*)

      integer ndsb1a,ndsb1b,ndsb2a,ndsb2b,ndsb3a,ndsb3b
      real vsb(ndsb1a:ndsb1b,ndsb2a:ndsb2b,ndsb3a:ndsb3b,0:*)
      real vsbn(ndsb1a:ndsb1b,ndsb2a:ndsb2b,ndsb3a:ndsb3b,0:*)
      real vsbm(ndsb1a:ndsb1b,ndsb2a:ndsb2b,ndsb3a:ndsb3b,0:*)


      integer ndta1a,ndta1b,ndta2a,ndta2b,ndta3a,ndta3b
      real vta(ndta1a:ndta1b,ndta2a:ndta2b,ndta3a:ndta3b,0:*)
      real vtan(ndta1a:ndta1b,ndta2a:ndta2b,ndta3a:ndta3b,0:*)
      real vtam(ndta1a:ndta1b,ndta2a:ndta2b,ndta3a:ndta3b,0:*)

      integer ndtb1a,ndtb1b,ndtb2a,ndtb2b,ndtb3a,ndtb3b
      real vtb(ndtb1a:ndtb1b,ndtb2a:ndtb2b,ndtb3a:ndtb3b,0:*)
      real vtbn(ndtb1a:ndtb1b,ndtb2a:ndtb2b,ndtb3a:ndtb3b,0:*)
      real vtbm(ndtb1a:ndtb1b,ndtb2a:ndtb2b,ndtb3a:ndtb3b,0:*)

c ..............

      real wra(ndra1a:ndra1b,ndra2a:ndra2b,ndra3a:ndra3b,0:*)
      real wran(ndra1a:ndra1b,ndra2a:ndra2b,ndra3a:ndra3b,0:*)
      real wram(ndra1a:ndra1b,ndra2a:ndra2b,ndra3a:ndra3b,0:*)

      real wrb(ndrb1a:ndrb1b,ndrb2a:ndrb2b,ndrb3a:ndrb3b,0:*)
      real wrbn(ndrb1a:ndrb1b,ndrb2a:ndrb2b,ndrb3a:ndrb3b,0:*)
      real wrbm(ndrb1a:ndrb1b,ndrb2a:ndrb2b,ndrb3a:ndrb3b,0:*)

      real wsa(ndsa1a:ndsa1b,ndsa2a:ndsa2b,ndsa3a:ndsa3b,0:*)
      real wsan(ndsa1a:ndsa1b,ndsa2a:ndsa2b,ndsa3a:ndsa3b,0:*)
      real wsam(ndsa1a:ndsa1b,ndsa2a:ndsa2b,ndsa3a:ndsa3b,0:*)

      real wsb(ndsb1a:ndsb1b,ndsb2a:ndsb2b,ndsb3a:ndsb3b,0:*)
      real wsbn(ndsb1a:ndsb1b,ndsb2a:ndsb2b,ndsb3a:ndsb3b,0:*)
      real wsbm(ndsb1a:ndsb1b,ndsb2a:ndsb2b,ndsb3a:ndsb3b,0:*)

      real wta(ndta1a:ndta1b,ndta2a:ndta2b,ndta3a:ndta3b,0:*)
      real wtan(ndta1a:ndta1b,ndta2a:ndta2b,ndta3a:ndta3b,0:*)
      real wtam(ndta1a:ndta1b,ndta2a:ndta2b,ndta3a:ndta3b,0:*)

      real wtb(ndtb1a:ndtb1b,ndtb2a:ndtb2b,ndtb3a:ndtb3b,0:*)
      real wtbn(ndtb1a:ndtb1b,ndtb2a:ndtb2b,ndtb3a:ndtb3b,0:*)
      real wtbm(ndtb1a:ndtb1b,ndtb2a:ndtb2b,ndtb3a:ndtb3b,0:*)




      real f(ndf1a:ndf1b,ndf2a:ndf2b,ndf3a:ndf3b,0:*)
      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      integer gridIndexRange(0:1,0:2)

      integer ipar(0:*),boundaryCondition(0:1,0:2)
      real rpar(0:*)

c     --- local variables ----
      
      integer side,axis,gridType,orderOfAccuracy,orderOfExtrapolation,useForcing,\
        ex,ey,ez,hx,hy,hz,useWhereMask,grid,debug,side1,side2,side3
      real dx(0:2),dr(0:2),t,ep,dt,c      
      real dxa,dya,dza
      integer axisp1,axisp2,i1,i2,i3,is1,is2,is3,js1,js2,js3,ks1,ks2,ks3
      integer numberOfGhostPoints
      integer bc1,bc2

      real expsdt
      real ux,uy,uz,uxx,uyy,uzz,uxxx,uyyy,uzzz,uxxxx,uyyyy,uzzzz,uxxyy,uxxzz,uyyzz,uxxy,uxxz,uxyy,uxzz,uyzz,uyyz
       
      real uLap,uLapsq,uLapx,uLapy,uLapz,uLapxx,uLapyy,uLapzz
      real ut,uxt,uyt,uzt,uxtt,uytt,uztt,uxxt,uyyt,uzzt,uxxtt,uyytt,uzztt

      real v,vx,vy,vz,vxx,vyy,vzz,vxxx,vyyy,vzzz,vxyy,vxxy,vxxz,vyyz,vxzz,vyzz,vt,vtt,vttt,vxt,vyt,vzt,vxtt,vytt,vztt,vtttt
      real vLapx,vLapy,vLapz
      real w,wx,wy,wz,wxx,wyy,wzz,wxxx,wyyy,wzzz,wxyy,wxxy,wt,wtt,wttt,wxt,wxtt,wtttt


      ! Box types:
      integer xSide,ySide,zSide,xyEdge,xzEdge,yzEdge,xyzCorner
      parameter( xSide=0,ySide=1,zSide=2,xyEdge=3,xzEdge=4,yzEdge=5,xyzCorner=6 )

      integer md1a,md1b,md2a,md2b,md3a,md3b
      integer m1a,m1b,m2a,m2b,m3a,m3b,i1a,i1b,i2a,i2b,i3a,i3b
      integer nb,power,numberOfBoxes,boxType,assignInterior
      real layerStrength,xScale,yScale,zScale,xx,yy,zz,csq,cdtsq,cxy,cdt4Over12
      real sigma,sigma1,sigma2,sigma3,sigma1x,sigma2y,sigma3z
      integer box(0:9,26)

      ! boundary conditions parameters
      #Include "bcDefineFortranInclude.h"
 
      integer rectangular,curvilinear
      parameter(\
        rectangular=0,\
        curvilinear=1)


c     --- start statement function ----
      integer kd,m,n
      real rx,ry,rz,sx,sy,sz,tx,ty,tz


      declareDifferenceOrder2(u,RX)
      declareDifferenceOrder2(um,none)

      declareDifferenceOrder2(vra,none)
      declareDifferenceOrder2(vram,none)
      declareDifferenceOrder2(wra,none)
      declareDifferenceOrder2(wram,none)
 
      declareDifferenceOrder2(vrb,none)
      declareDifferenceOrder2(vrbm,none)
      declareDifferenceOrder2(wrb,none)
      declareDifferenceOrder2(wrbm,none)
 
      declareDifferenceOrder2(vsa,none)
      declareDifferenceOrder2(vsam,none)
      declareDifferenceOrder2(wsa,none)
      declareDifferenceOrder2(wsam,none)
 
      declareDifferenceOrder2(vsb,none)
      declareDifferenceOrder2(vsbm,none)
      declareDifferenceOrder2(wsb,none)
      declareDifferenceOrder2(wsbm,none)
 
      declareDifferenceOrder2(vta,none)
      declareDifferenceOrder2(vtam,none)
      declareDifferenceOrder2(wta,none)
      declareDifferenceOrder2(wtam,none)
 
      declareDifferenceOrder2(vtb,none)
      declareDifferenceOrder2(vtbm,none)
      declareDifferenceOrder2(wtb,none)
      declareDifferenceOrder2(wtbm,none)
 
      declareDifferenceOrder4(u,RX)
      declareDifferenceOrder4(um,none)

      declareDifferenceOrder4(vra,none)
      declareDifferenceOrder4(vram,none)
      declareDifferenceOrder4(wra,none)
      declareDifferenceOrder4(wram,none)
 
      declareDifferenceOrder4(vrb,none)
      declareDifferenceOrder4(vrbm,none)
      declareDifferenceOrder4(wrb,none)
      declareDifferenceOrder4(wrbm,none)
 
      declareDifferenceOrder4(vsa,none)
      declareDifferenceOrder4(vsam,none)
      declareDifferenceOrder4(wsa,none)
      declareDifferenceOrder4(wsam,none)
 
      declareDifferenceOrder4(vsb,none)
      declareDifferenceOrder4(vsbm,none)
      declareDifferenceOrder4(wsb,none)
      declareDifferenceOrder4(wsbm,none)
 
      declareDifferenceOrder4(vta,none)
      declareDifferenceOrder4(vtam,none)
      declareDifferenceOrder4(wta,none)
      declareDifferenceOrder4(wtam,none)
 
      declareDifferenceOrder4(vtb,none)
      declareDifferenceOrder4(vtbm,none)
      declareDifferenceOrder4(wtb,none)
      declareDifferenceOrder4(wtbm,none)

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
      defineDifferenceOrder2Components1(um,none)

      defineDifferenceOrder2Components1(vra,none)
      defineDifferenceOrder2Components1(vram,none)
      defineDifferenceOrder2Components1(wra,none)
      defineDifferenceOrder2Components1(wram,none)

      defineDifferenceOrder2Components1(vrb,none)
      defineDifferenceOrder2Components1(vrbm,none)
      defineDifferenceOrder2Components1(wrb,none)
      defineDifferenceOrder2Components1(wrbm,none)

      defineDifferenceOrder2Components1(vsa,none)
      defineDifferenceOrder2Components1(vsam,none)
      defineDifferenceOrder2Components1(wsa,none)
      defineDifferenceOrder2Components1(wsam,none)

      defineDifferenceOrder2Components1(vsb,none)
      defineDifferenceOrder2Components1(vsbm,none)
      defineDifferenceOrder2Components1(wsb,none)
      defineDifferenceOrder2Components1(wsbm,none)

      defineDifferenceOrder2Components1(vta,none)
      defineDifferenceOrder2Components1(vtam,none)
      defineDifferenceOrder2Components1(wta,none)
      defineDifferenceOrder2Components1(wtam,none)

      defineDifferenceOrder2Components1(vtb,none)
      defineDifferenceOrder2Components1(vtbm,none)
      defineDifferenceOrder2Components1(wtb,none)
      defineDifferenceOrder2Components1(wtbm,none)


      defineDifferenceOrder4Components1(u,RX)
      defineDifferenceOrder4Components1(um,none)

      defineDifferenceOrder4Components1(vra,none)
      defineDifferenceOrder4Components1(vram,none)
      defineDifferenceOrder4Components1(wra,none)
      defineDifferenceOrder4Components1(wram,none)

      defineDifferenceOrder4Components1(vrb,none)
      defineDifferenceOrder4Components1(vrbm,none)
      defineDifferenceOrder4Components1(wrb,none)
      defineDifferenceOrder4Components1(wrbm,none)

      defineDifferenceOrder4Components1(vsa,none)
      defineDifferenceOrder4Components1(vsam,none)
      defineDifferenceOrder4Components1(wsa,none)
      defineDifferenceOrder4Components1(wsam,none)

      defineDifferenceOrder4Components1(vsb,none)
      defineDifferenceOrder4Components1(vsbm,none)
      defineDifferenceOrder4Components1(wsb,none)
      defineDifferenceOrder4Components1(wsbm,none)

      defineDifferenceOrder4Components1(vta,none)
      defineDifferenceOrder4Components1(vtam,none)
      defineDifferenceOrder4Components1(wta,none)
      defineDifferenceOrder4Components1(wtam,none)

      defineDifferenceOrder4Components1(vtb,none)
      defineDifferenceOrder4Components1(vtbm,none)
      defineDifferenceOrder4Components1(wtb,none)
      defineDifferenceOrder4Components1(wtbm,none)


c............... end statement functions

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

      power                =ipar(22)
      assignInterior       =ipar(23)
     
      dx(0)                =rpar(0)
      dx(1)                =rpar(1)
      dx(2)                =rpar(2)
      dr(0)                =rpar(3)
      dr(1)                =rpar(4)
      dr(2)                =rpar(5)
      t                    =rpar(6)
      ep                   =rpar(7) ! pointer for exact solution
      dt                   =rpar(8)
      c                    =rpar(9)

      layerStrength        =rpar(16)

      ! power=4
      ! layerStrength=30.

      if( debug.gt.1 )then
        write(*,'(" pmlMaxwell: **START** grid=",i4," side,axis=",2i2,", c,dt=",2f8.5," layerStrength,power=",f6.2,i2)') grid,side,axis,c,dt,layerStrength,power
        write(*,'(" pmlMaxwell: nd,orderOfAccuracy,gridType=",i2,i2,i2)') nd,orderOfAccuracy,gridType
        write(*,'(" pmlMaxwell: dx=",3e10.2)') dx(0),dx(1),dx(2)

      end if
     
      csq=c*c
      cdtsq=(c*dt)**2
      cdt4Over12=(c*dt)**4/12.

      ! ***** first fill in the parameters for the boxes we need to assign ****
      numberOfGhostPoints=orderOfAccuracy/2

      ! We apply the PML equations out to these bounds: (make use all ghost points - stencilWidth/2)
      md1a=nd1a+numberOfGhostPoints
      md2a=nd2a+numberOfGhostPoints
      md3a=nd3a+numberOfGhostPoints
      md1b=nd1b-numberOfGhostPoints
      md2b=nd2b-numberOfGhostPoints
      md3b=nd3b-numberOfGhostPoints

      if( nd.eq.2 )then
        md3a=nd3a
        md3b=nd3b
      end if

       m1a=n1a
       m1b=n1b
       m2a=n2a
       m2b=n2b
       m3a=n3a
       m3b=n3b
      if( assignInterior.eq.1 )then
        write(*,'(" ****pml: assign interior pts=[",i3,",",i3,"][",i3,",",i3,"][",i3,",",i3,"]")') m1a,m1b,m2a,m2b,m3a,m3b

       if( layerStrength.le.0. )then
         ! apply equation everywhere  in this case
         m1a=md1a
         m1b=md1b
         m2a=md2a
         m2b=md2b
         m3a=md3a
         m3b=md3b
       end if
      if( orderOfAccuracy.eq.2  )then
        ! advance the interior equations
        if( nd.eq.2 )then
         do i3=m3a,m3b
         do i2=m2a,m2b
         do i1=m1a,m1b
           un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) \
                    + cdtsq*( uLaplacian22r(i1,i2,i3,ex) ) 
         end do
         end do
         end do
        else
         do i3=m3a,m3b
         do i2=m2a,m2b
         do i1=m1a,m1b
           un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) \
                    + cdtsq*( uLaplacian23r(i1,i2,i3,ex) ) 
         end do
         end do
         end do
        end if
      
      else if( orderOfAccuracy.eq.4  )then
        ! advance the interior equations
        if( nd.eq.2 )then
         do i3=m3a,m3b
         do i2=m2a,m2b
         do i1=m1a,m1b
           un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) \
                    + cdtsq*( uLaplacian42r(i1,i2,i3,ex) ) + cdt4Over12*( uLapSq22r(i1,i2,i3,ex) )
         end do
         end do
         end do
        else
         do i3=m3a,m3b
         do i2=m2a,m2b
         do i1=m1a,m1b
           un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) \
                    + cdtsq*( uLaplacian43r(i1,i2,i3,ex) ) + cdt4Over12*( uLapSq23r(i1,i2,i3,ex) )
         end do
         end do
         end do
        end if

        if( layerStrength.le.0. )then
          write(*,'(" --- pml: apply equation everywhere ---")')
          return
        end if
      end if
      end if

      ! The PML equations are applied outside the box [n1a,n1b]x[n2a,n2b]x[n3a,n3b]
      m1a=n1a-1
      m2a=n2a-1
      m3a=n3a-1
           
      m1b=n1b+1
      m2b=n2b+1
      m3b=n3b+1
      if( nd.eq.2 )then
        m3a=n3a
        m3b=n3b
      end if

      nb=1 ! counts boxes
      ! left face
      setBox(md1a,m1a, n2a,n2b, n3a,n3b, xSide,0,0,0 )
      ! right face
      setBox(m1b,md1b, n2a,n2b, n3a,n3b, xSide,1,0,0 )
      ! bottom and top
      setBox(n1a,n1b, md2a,m2a, n3a,n3b, ySide,0,0,0 )
      setBox(n1a,n1b, m2b,md2b, n3a,n3b, ySide,0,1,0 )
      ! edges (corners in 2d)
      setBox(md1a,m1a , md2a,m2a , n3a,n3b, xyEdge,0,0,0 )
      setBox(md1a,m1a , m2b ,md2b, n3a,n3b, xyEdge,0,1,0 )
      setBox(m1b ,md1b, md2a,m2a , n3a,n3b, xyEdge,1,0,0 )
      setBox(m1b ,md1b, m2b ,md2b, n3a,n3b, xyEdge,1,1,0 )

      if( nd.eq.3 )then
        ! front and back
        setBox(n1a,n1b, n2a,n2b, md3a,m3a, zSide,0,0,0 )
        setBox(n1a,n1b, n2a,n2b, m3b,md3b, zSide,0,0,1 )
        ! more edges
        
        setBox(md1a,m1a, n2a,n2b, md3a,m3a, xzEdge,0,0,0 )
        setBox(md1a,m1a, n2a,n2b, m3b,md3b, xzEdge,0,0,1 )
        setBox(m1b,md1b, n2a,n2b, md3a,m3a, xzEdge,1,0,0 )
        setBox(m1b,md1b, n2a,n2b, m3b,md3b, xzEdge,1,0,1 )
                                                    
        setBox(n1a,n1b, md2a,m2a, md3a,m3a, yzEdge,0,0,0 )
        setBox(n1a,n1b, md2a,m2a, m3b,md3b, yzEdge,0,0,1 )
        setBox(n1a,n1b, m2b,md2b, md3a,m3a, yzEdge,0,1,0 )
        setBox(n1a,n1b, m2b,md2b, m3b,md3b, yzEdge,0,1,1 )

        ! corners
        setBox(md1a,m1a, md2a,m2a, md3a,m3a, xyzCorner,0,0,0 )
        setBox(m1b,md1b, md2a,m2a, md3a,m3a, xyzCorner,1,0,0 )
        setBox(md1a,m1a, m2b,md2b, md3a,m3a, xyzCorner,0,1,0 )
        setBox(m1b,md1b, m2b,md2b, md3a,m3a, xyzCorner,1,1,0 )

        setBox(md1a,m1a, md2a,m2a, m3b,md3b, xyzCorner,0,0,1 )
        setBox(m1b,md1b, md2a,m2a, m3b,md3b, xyzCorner,1,0,1 )
        setBox(md1a,m1a, m2b,md2b, m3b,md3b, xyzCorner,0,1,1 )
        setBox(m1b,md1b, m2b,md2b, m3b,md3b, xyzCorner,1,1,1 )

        
      end if

      numberOfBoxes=nb-1

      ! -------------------------------------------------------------------------
      ! ------------------Loop over Boxes----------------------------------------
      ! -------------------------------------------------------------------------


      ! write(*,'(" >>>>Apply abcPML: grid,",i3," dt,c=",2e12.3," numberOfGhostPoints=",i2)') grid,dt,c,numberOfGhostPoints

      do nb=1,numberOfBoxes

       m1a=box(0,nb)
       m1b=box(1,nb)
       m2a=box(2,nb)
       m2b=box(3,nb)
       m3a=box(4,nb)
       m3b=box(5,nb)
       boxType=box(6,nb)
       side1=box(7,nb)
       side2=box(8,nb)
       side3=box(9,nb)

       if( side1.eq.0 )then
         ! assign the layer on the left interva [m1a,m1b]
         i1a=m1b+1  ! layer goes to zero at this point
         i1b=m1a    ! layer ends at this point
       else
         i1a=m1a-1  ! layer goes to zero at this point
         i1b=m1b    ! layer ends at this point
       end if
       if( side2.eq.0 )then
         i2a=m2b+1  ! layer goes to zero at this point
         i2b=m2a    ! layer ends at this point
       else
         i2a=m2a-1  ! layer goes to zero at this point
         i2b=m2b    ! layer ends at this point
       end if
       if( side3.eq.0 )then
         i3a=m3b+1  ! layer goes to zero at this point
         i3b=m3a    ! layer ends at this point
       else
         i3a=m3a-1  ! layer goes to zero at this point
         i3b=m3b    ! layer ends at this point
       end if

       ! write(*,'("     pml: box(",i2,"=[",i3,",",i3,"][",i3,",",i3,"][",i3,",",i3,"], boxType=",i2,", side=",3i2," ex=",i2)') nb,m1a,m1b,m2a,m2b,m3a,m3b,boxType,side1,side2,side3,ex


       if( gridType.eq.rectangular .and. orderOfAccuracy.eq.2 )then
        ! ***********************************************
        ! ************rectangular grid*******************
        ! ***********************************************

        if( nd.eq.2 )then

         ! write(*,'("     pml: assign 2d rectangular grid orderOfAccuracy==2")') 
         if( boxType.eq.xSide )then

          if( side1.eq.0 )then
            beginLoops()
             getSigma1(sigma)
             update(vra,wra,ex,laplacian22r,x22r,xx22r)
            endLoops()
          else
            beginLoops()
             getSigma1(sigma)
             update(vrb,wrb,ex,laplacian22r,x22r,xx22r)
            endLoops()
          end if

         else if( boxType.eq.ySide )then

          if( side2.eq.0 )then
           beginLoops()
            getSigma2(sigma)
            update(vsa,wsa,ex,laplacian22r,y22r,yy22r)
           endLoops()
          else
           beginLoops()
            getSigma2(sigma)
            update(vsb,wsb,ex,laplacian22r,y22r,yy22r)
           endLoops()
          end if

         else if( boxType.eq.xyEdge )then

          if( side1.eq.0 .and. side2.eq.0 )then
           beginLoops()
            getSigma1(sigma1)
            getSigma2(sigma2)
            updateCorner(vra,wra, vsa,wsa, sigma1,sigma2,\
                         ex,laplacian22r,x22r,xx22r,y22r,yy22r,xy22r)
           endLoops()
          else if( side1.eq.1 .and. side2.eq.0 )then
           beginLoops()
            getSigma1(sigma1)
            getSigma2(sigma2)
            updateCorner(vrb,wrb, vsa,wsa, sigma1,sigma2,\
                         ex,laplacian22r,x22r,xx22r,y22r,yy22r,xy22r)
           endLoops()
          else if( side1.eq.0 .and. side2.eq.1 )then
           beginLoops()
            getSigma1(sigma1)
            getSigma2(sigma2)
            updateCorner(vra,wra, vsb,wsb, sigma1,sigma2,\
                         ex,laplacian22r,x22r,xx22r,y22r,yy22r,xy22r)
           endLoops()
          else if( side1.eq.1 .and. side2.eq.1 )then
           beginLoops()
            getSigma1(sigma1)
            getSigma2(sigma2)
            updateCorner(vrb,wrb, vsb,wsb, sigma1,sigma2,\
                         ex,laplacian22r,x22r,xx22r,y22r,yy22r,xy22r)
           endLoops()
          else
            stop 62244
          end if

         else
           stop 23415
         end if

        else if( nd.eq.3 )then
         ! *********************************************************************
         ! ********************Three Dimensions*********************************
         ! *********************************************************************

         if( boxType.eq.xSide )then

          if( side1.eq.0 )then
            beginLoops()
             getSigma1(sigma)
             update(vra,wra,ex,laplacian23r,x23r,xx23r)
            endLoops()
          else
            beginLoops()
             getSigma1(sigma)
             update(vrb,wrb,ex,laplacian23r,x23r,xx23r)
            endLoops()
          end if

         else if( boxType.eq.ySide )then

          if( side2.eq.0 )then
           beginLoops()
            getSigma2(sigma)
            update(vsa,wsa,ex,laplacian23r,y23r,yy23r)
           endLoops()
          else
           beginLoops()
            getSigma2(sigma)
            update(vsb,wsb,ex,laplacian23r,y23r,yy23r)
           endLoops()
          end if

         else if( boxType.eq.zSide )then

          if( side3.eq.0 )then
           beginLoops()
            getSigma3(sigma)
            update(vta,wta,ex,laplacian23r,z23r,zz23r)
           endLoops()
          else
           beginLoops()
            getSigma3(sigma)
            update(vtb,wtb,ex,laplacian23r,z23r,zz23r)
           endLoops()
          end if

         else if( boxType.eq.xyEdge )then

          if( side1.eq.0 .and. side2.eq.0 )then
           beginLoops()
            getSigma1(sigma1)
            getSigma2(sigma2)
            updateCorner(vra,wra, vsa,wsa,sigma1,sigma2,\
                         ex,laplacian23r,x23r,xx23r,y23r,yy23r,xy23r)
           endLoops()
          else if( side1.eq.1 .and. side2.eq.0 )then
           beginLoops()
            getSigma1(sigma1)
            getSigma2(sigma2)
            updateCorner(vrb,wrb, vsa,wsa,sigma1,sigma2,\
                         ex,laplacian23r,x23r,xx23r,y23r,yy23r,xy23r)
           endLoops()
          else if( side1.eq.0 .and. side2.eq.1 )then
           beginLoops()
            getSigma1(sigma1)
            getSigma2(sigma2)
            updateCorner(vra,wra, vsb,wsb,sigma1,sigma2,\
                         ex,laplacian23r,x23r,xx23r,y23r,yy23r,xy23r)
           endLoops()
          else if( side1.eq.1 .and. side2.eq.1 )then
           beginLoops()
            getSigma1(sigma1)
            getSigma2(sigma2)
            updateCorner(vrb,wrb, vsb,wsb,sigma1,sigma2,\
                         ex,laplacian23r,x23r,xx23r,y23r,yy23r,xy23r)
           endLoops()
          else
            stop 66244
          end if

         else if( boxType.eq.xzEdge )then

          if( side1.eq.0 .and. side3.eq.0 )then
           beginLoops()
            getSigma1(sigma1)
            getSigma3(sigma3)
            updateCorner(vra,wra, vta,wta, sigma1,sigma3,\
                         ex,laplacian23r,x23r,xx23r,z23r,zz23r,xz23r)
           endLoops()
          else if( side1.eq.1 .and. side3.eq.0 )then
           beginLoops()
            getSigma1(sigma1)
            getSigma3(sigma3)
            updateCorner(vrb,wrb, vta,wta, sigma1,sigma3,\
                         ex,laplacian23r,x23r,xx23r,z23r,zz23r,xz23r)
           endLoops()
          else if( side1.eq.0 .and. side3.eq.1 )then
           beginLoops()
            getSigma1(sigma1)
            getSigma3(sigma3)
            updateCorner(vra,wra, vtb,wtb, sigma1,sigma3,\
                         ex,laplacian23r,x23r,xx23r,z23r,zz23r,xz23r)
           endLoops()
          else if( side1.eq.1 .and. side3.eq.1 )then
           beginLoops()
            getSigma1(sigma1)
            getSigma3(sigma3)
            updateCorner(vrb,wrb, vtb,wtb, sigma1,sigma3,\
                         ex,laplacian23r,x23r,xx23r,z23r,zz23r,xz23r)
           endLoops()
          else
            stop 66224
          end if

         else if( boxType.eq.yzEdge )then

          if( side2.eq.0 .and. side3.eq.0 )then
           beginLoops()
            getSigma2(sigma2)
            getSigma3(sigma3)
            updateCorner(vsa,wsa, vta,wta, sigma2,sigma3,\
                         ex,laplacian23r,y23r,yy23r,z23r,zz23r,yz23r)
           endLoops()
          else if( side2.eq.1 .and. side3.eq.0 )then
           beginLoops()
            getSigma2(sigma2)
            getSigma3(sigma3)
            updateCorner(vsb,wsb, vta,wta, sigma2,sigma3,\
                         ex,laplacian23r,y23r,yy23r,z23r,zz23r,yz23r)
           endLoops()
          else if( side2.eq.0 .and. side3.eq.1 )then
           beginLoops()
            getSigma2(sigma2)
            getSigma3(sigma3)
            updateCorner(vsa,wsa, vtb,wtb, sigma2,sigma3,\
                         ex,laplacian23r,y23r,yy23r,z23r,zz23r,yz23r)
           endLoops()
          else if( side2.eq.1 .and. side3.eq.1 )then
           beginLoops()
            getSigma2(sigma2)
            getSigma3(sigma3)
            updateCorner(vsb,wsb, vtb,wtb, sigma2,sigma3,\
                         ex,laplacian23r,y23r,yy23r,z23r,zz23r,yz23r)
           endLoops()
          else
            stop 6244
          end if

         else if( boxType.eq.xyzCorner )then

          if(      side1.eq.0 .and. side2.eq.0 .and. side3.eq.0 )then
            updateVertex(vra,wra, vsa,wsa, vta,wta, ex,23r )
          else if( side1.eq.1 .and. side2.eq.0 .and. side3.eq.0 )then
            updateVertex(vrb,wrb, vsa,wsa, vta,wta, ex,23r )
          else if( side1.eq.0 .and. side2.eq.1 .and. side3.eq.0 )then
            updateVertex(vra,wra, vsb,wsb, vta,wta, ex,23r )
          else if( side1.eq.1 .and. side2.eq.1 .and. side3.eq.0 )then
            updateVertex(vrb,wrb, vsb,wsb, vta,wta, ex,23r )
          else if( side1.eq.0 .and. side2.eq.0 .and. side3.eq.1 )then
            updateVertex(vra,wra, vsa,wsa, vtb,wtb, ex,23r )
          else if( side1.eq.1 .and. side2.eq.0 .and. side3.eq.1 )then
            updateVertex(vrb,wrb, vsa,wsa, vtb,wtb, ex,23r )
          else if( side1.eq.0 .and. side2.eq.1 .and. side3.eq.1 )then
            updateVertex(vra,wra, vsb,wsb, vtb,wtb, ex,23r )
          else if( side1.eq.1 .and. side2.eq.1 .and. side3.eq.1 )then
            updateVertex(vrb,wrb, vsb,wsb, vtb,wta, ex,23r )
          else
            stop 6624
          end if

         else
           stop 23415
         end if

        end if
     
       else if( gridType.eq.rectangular .and. orderOfAccuracy.eq.4 )then

        ! ***********************************************
        ! ************rectangular grid*******************
        ! ************ fourth-order   *******************
        ! ***********************************************

        if( nd.eq.2 )then

         if( boxType.eq.xSide )then

          if( side1.eq.0 )then
           beginLoops()
            getSigma1x(sigma1,sigma1x)
            ! update4xNew(ex,fullUpdate,2)
            update4x2d(vra,wra, ex,fullUpdate)
           endLoops()
          else
           beginLoops()
            getSigma1x(sigma1,sigma1x)
            update4x2d(vrb,wrb, ex,fullUpdate)
           endLoops()
          end if

         else if( boxType.eq.ySide )then

          if( side2.eq.0 )then
           beginLoops()
            getSigma2y(sigma2,sigma2y)
            update4y2d(vsa,wsa, ex,fullUpdate)
           endLoops()
          else
           beginLoops()
            getSigma2y(sigma2,sigma2y)
            update4y2d(vsb,wsb, ex,fullUpdate)
           endLoops()
          end if

         else if( boxType.eq.xyEdge )then

          if( side1.eq.0 .and. side2.eq.0 )then
           beginLoops()
            getSigma1x(sigma1,sigma1x)
            update4x2d(vra,wra, ex,fullUpdate)
            getSigma2y(sigma2,sigma2y)
            update4y2d(vsa,wsa, ex,partialUpdate)
           endLoops()
          else if( side1.eq.1 .and. side2.eq.0 )then 
           beginLoops()
            getSigma1x(sigma1,sigma1x)
            update4x2d(vrb,wrb, ex,fullUpdate)
            getSigma2y(sigma2,sigma2y)
            update4y2d(vsa,wsa, ex,partialUpdate)
           endLoops()
          else if( side1.eq.0 .and. side2.eq.1 )then 
           beginLoops()
            getSigma1x(sigma1,sigma1x)
            update4x2d(vra,wra, ex,fullUpdate)
            getSigma2y(sigma2,sigma2y)
            update4y2d(vsb,wsb, ex,partialUpdate)
           endLoops()
          else if( side1.eq.1 .and. side2.eq.1 )then 
           beginLoops()
            getSigma1x(sigma1,sigma1x)
            update4x2d(vrb,wrb, ex,fullUpdate)
            getSigma2y(sigma2,sigma2y)
            update4y2d(vsb,wsb, ex,partialUpdate)
           endLoops()
          else
            stop 55555 ! invalid side1, side2
          end if
         else
           stop 23415
         end if

        else if( nd.eq.3 )then
 
         if( boxType.eq.xSide )then

          if( side1.eq.0 )then
           beginLoops()
            getSigma1x(sigma1,sigma1x)
            update4x3d(vra,wra, ex,fullUpdate)
           endLoops()
          else
           beginLoops()
            getSigma1x(sigma1,sigma1x)
            update4x3d(vrb,wrb, ex,fullUpdate)
           endLoops()
          end if

         else if( boxType.eq.ySide )then

          if( side2.eq.0 )then
           beginLoops()
            getSigma2y(sigma2,sigma2y)
            update4y3d(vsa,wsa, ex,fullUpdate)
           endLoops()
          else
           beginLoops()
            getSigma2y(sigma2,sigma2y)
            update4y3d(vsb,wsb, ex,fullUpdate)
           endLoops()
          end if

         else if( boxType.eq.zSide )then

          if( side2.eq.0 )then
           beginLoops()
            getSigma3z(sigma3,sigma3z)
            update4z3d(vta,wta, ex,fullUpdate)
           endLoops()
          else
           beginLoops()
            getSigma3z(sigma3,sigma3z)
            update4z3d(vtb,wtb, ex,fullUpdate)
           endLoops()
          end if

         else if( boxType.eq.xyEdge )then

#beginMacro update4xy(vr,wr,vs,ws)
 beginLoops()
  getSigma1x(sigma1,sigma1x)
  update4x3d(vr,wr, ex,fullUpdate)
  getSigma2y(sigma2,sigma2y)
  update4y3d(vs,ws, ex,partialUpdate)
 endLoops()
#endMacro
#beginMacro update4xz(vr,wr,vt,wt)
 beginLoops()
  getSigma1x(sigma1,sigma1x)
  update4x3d(vr,wr, ex,fullUpdate)
  getSigma3z(sigma3,sigma3z)
  update4z3d(vt,wt, ex,partialUpdate)
 endLoops()
#endMacro
#beginMacro update4yz(vs,ws,vt,wt)
 beginLoops()
  getSigma2y(sigma2,sigma2y)
  update4y3d(vs,ws, ex,fullUpdate)
  getSigma3z(sigma3,sigma3z)
  update4z3d(vt,wt, ex,partialUpdate)
 endLoops()
#endMacro

#beginMacro update4xyz(vr,wr,vs,ws,vt,wt)
 beginLoops()
  getSigma1x(sigma1,sigma1x)
  update4x3d(vr,wr, ex,fullUpdate)
  getSigma2y(sigma2,sigma2y)
  update4y3d(vs,ws, ex,partialUpdate)
  getSigma3z(sigma3,sigma3z)
  update4z3d(vt,wt, ex,partialUpdate)
 endLoops()
#endMacro

          if( side1.eq.0 .and. side2.eq.0 )then
            update4xy(vra,wra,vsa,wsa)
          else if( side1.eq.1 .and. side2.eq.0 )then
            update4xy(vrb,wrb,vsa,wsa)
          else if( side1.eq.0 .and. side2.eq.1 )then
            update4xy(vra,wra,vsb,wsb)
          else if( side1.eq.1 .and. side2.eq.1 )then
            update4xy(vrb,wrb,vsb,wsb)
          else
            stop 6244
          end if

         else if( boxType.eq.xzEdge )then

          if( side1.eq.0 .and. side3.eq.0 )then
            update4xz(vra,wra,vta,wta)
          else if( side1.eq.1 .and. side3.eq.0 )then
            update4xz(vrb,wrb,vta,wta)
          else if( side1.eq.0 .and. side3.eq.1 )then
            update4xz(vra,wra,vtb,wtb)
          else if( side1.eq.1 .and. side3.eq.1 )then
            update4xz(vrb,wrb,vtb,wtb)
          else
            stop 6644
          end if

         else if( boxType.eq.yzEdge )then

          if( side2.eq.0 .and. side3.eq.0 )then
            update4yz(vsa,wsa,vta,wta)
          else if( side2.eq.1 .and. side3.eq.0 )then
            update4yz(vsb,wsb,vta,wta)
          else if( side2.eq.0 .and. side3.eq.1 )then
            update4yz(vsa,wsa,vtb,wtb)
          else if( side2.eq.1 .and. side3.eq.1 )then
            update4yz(vsb,wsb,vtb,wtb)
          else
            stop 6244
          end if

         else if( boxType.eq.xyzCorner )then

          if(      side1.eq.0 .and. side2.eq.0 .and. side3.eq.0 )then
            update4xyz(vra,wra,vsa,wsa,vta,wta)
          else if( side1.eq.1 .and. side2.eq.0 .and. side3.eq.0 )then
            update4xyz(vrb,wrb,vsa,wsa,vta,wta)
          else if( side1.eq.0 .and. side2.eq.1 .and. side3.eq.0 )then
            update4xyz(vra,wra,vsb,wsb,vta,wta)
          else if( side1.eq.1 .and. side2.eq.1 .and. side3.eq.0 )then
            update4xyz(vrb,wrb,vsb,wsb,vta,wta)
          else if( side1.eq.0 .and. side2.eq.0 .and. side3.eq.1 )then
            update4xyz(vra,wra,vsa,wsa,vtb,wtb)
          else if( side1.eq.1 .and. side2.eq.0 .and. side3.eq.1 )then
            update4xyz(vrb,wrb,vsa,wsa,vtb,wtb)
          else if( side1.eq.0 .and. side2.eq.1 .and. side3.eq.1 )then
            update4xyz(vra,wra,vsb,wsb,vtb,wtb)
          else if( side1.eq.1 .and. side2.eq.1 .and. side3.eq.1 )then
            update4xyz(vrb,wrb,vsb,wsb,vtb,wtb)
          else
            stop 6224
          end if

         else
           stop 23415  ! unknown boxType
         end if

        end if ! end nd==3
  
     
       else  ! end rectangular 4th order
         stop 22555
       end if
      end do  ! numberOfBoxes

      return
      end




