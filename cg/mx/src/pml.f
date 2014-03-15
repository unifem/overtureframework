! This file automatically generated from pml.bf with bpp.
! *******************************************************************************
!   Integrate the PML Absorbing boundary condition equations
!
!  NOTE: Run "pml.maple" to generate "pml.h" from "pmlUpdate.h"; pml.h is included in this file.
! *******************************************************************************

c These next include files will define the macros that will define the difference approximations
c The actual macro is called below
c Use this next macro to declare the statement functions that are defined below
c To include derivatives of rx use OPTION=RX


c Define statement functions for difference approximations of order 2 
c To include derivatives of rx use OPTION=RX
c To include derivatives of rx use OPTION=RX



c Use this next macro to declare the statement functions that are defined below
c To include derivatives of rx use OPTION=RX


c Define statement functions for difference approximations of order 4 
c To include derivatives of rx use OPTION=RX
c To include derivatives of rx use OPTION=RX




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

c ====================================================================================================
c  Update a variable in a 2D corner or along an edge in 3d
c ====================================================================================================



c ================================================================================================
c ================================================================================================

c ====================================================================================================
c  Fourth-order update on a side
c ====================================================================================================

! ******** This file generated from pmlUpdate.h using pml.maple ***** 

c ** NOTE: run pml.maple to take this file and generate the pml.h file
c     restart; read "pml.maple";
c ====================================================================================================
c  Fourth-order update on a side
c   OPTION : fullUpdate or partialUpdate
c   2 : 2 or 3 space dimensions
c ====================================================================================================

! ******** This file generated from pmlUpdate.h using pml.maple ***** 

c ** NOTE: run pml.maple to take this file and generate the pml.h file
c     restart; read "pml.maple";
c ====================================================================================================
c  Fourth-order update on a side
c   OPTION : fullUpdate or partialUpdate
c   2 : 2 or 3 space dimensions
c ====================================================================================================

! ******** This file generated from pmlUpdate.h using pml.maple ***** 

c ** NOTE: run pml.maple to take this file and generate the pml.h file
c     restart; read "pml.maple";
c ====================================================================================================
c  Fourth-order update on a side
c   OPTION : fullUpdate or partialUpdate
c   3 : 2 or 3 space dimensions
c ====================================================================================================

! ******** This file generated from pmlUpdate.h using pml.maple ***** 

c ** NOTE: run pml.maple to take this file and generate the pml.h file
c     restart; read "pml.maple";
c ====================================================================================================
c  Fourth-order update on a side
c   OPTION : fullUpdate or partialUpdate
c   3 : 2 or 3 space dimensions
c ====================================================================================================

! ******** This file generated from pmlUpdate.h using pml.maple ***** 

c ** NOTE: run pml.maple to take this file and generate the pml.h file
c     restart; read "pml.maple";
c ====================================================================================================
c  Fourth-order update on a side
c   OPTION : fullUpdate or partialUpdate
c   3 : 2 or 3 space dimensions
c ====================================================================================================


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
c$$$ un(i1,i2,i3,m)=2.*u(i1,i2,i3,m)-um(i1,i2,i3,m) c$$$                 + cdtsq*( uLap - vx -w ) c$$$                 + cdt4Over12*( uLapSq - vxxx - vxyy - waLaplacian42r(i1,i2,i3,ex)  - vxtt - wtt ) 
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
c$$$ un(i1,i2,i3,m)=2.*u(i1,i2,i3,m)-um(i1,i2,i3,m) c$$$                 + cdtsq*( uLap - vx -w ) c$$$                 + cdt4Over12*( uLapSq - vxxx - vxyy - waLaplacian42r(i1,i2,i3,ex)  - vxtt - wtt ) 
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













! ====================================================================================
! Macro: Advance the PML equations in corners in 2d, order of accuracy 2
! ===================================================================================


! ====================================================================================
! Macro: Advance the PML equations on edges and corners in 3d, order of accuracy 2
! ===================================================================================

! ====================================================================================
! Macro: Advance the PML equations in corners in 2d, order of accuracy 4
!   **** This version has trouble -- far corners go unstable after long time ****
!   **** This version is not really correct anyway -- we should advance the corner
!        regions with an unsplit approximation -- see notes in CgmxReferenceGuide ****
! ===================================================================================

! Macro used below to advance edges and corners in 3d to order 4
! Macro used below to advance edges and corners in 3d to order 4
! Macro used below to advance edges and corners in 3d to order 4

! Macro used below to advance edges and corners in 3d to order 4

! ====================================================================================
! Macro: Advance the PML equations in edges and corners in 3d, order of accuracy 4
!   **** This version has trouble -- far corners go unstable after long time ****
!   **** This version is not really correct anyway -- we should advance the corner
!        regions with an unsplit approximation -- see notes in CgmxReferenceGuide ****
! ===================================================================================




      subroutine pmlMaxwell( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,ndf1a,
     & ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,gridIndexRange, um, u, un, 
     & ndra1a,ndra1b,ndra2a,ndra2b,ndra3a,ndra3b,vram, vra, vran, 
     & wram, wra, wran, ndrb1a,ndrb1b,ndrb2a,ndrb2b,ndrb3a,ndrb3b,
     & vrbm, vrb, vrbn, wrbm, wrb, wrbn, ndsa1a,ndsa1b,ndsa2a,ndsa2b,
     & ndsa3a,ndsa3b,vsam, vsa, vsan, wsam, wsa, wsan, ndsb1a,ndsb1b,
     & ndsb2a,ndsb2b,ndsb3a,ndsb3b,vsbm, vsb, vsbn, wsbm, wsb, wsbn, 
     & ndta1a,ndta1b,ndta2a,ndta2b,ndta3a,ndta3b,vtam, vta, vtan, 
     & wtam, wta, wtan, ndtb1a,ndtb1b,ndtb2a,ndtb2b,ndtb3a,ndtb3b,
     & vtbm, vtb, vtbn, wtbm, wtb, wtbn, f,mask,rsxy, xy,bc, 
     & boundaryCondition, ipar, rpar, ierr )
! ===================================================================================
!  Absorbing boundary conditions for Maxwell's Equations.
!
!  gridType : 0=rectangular, 1=curvilinear
!  useForcing : 1=use f for RHS to BC
!  side,axis : 0:1 and 0:2
!
!  u : solution at time t
!  um : time t-dt
!  un : on output the solution at time t+dt
!
!  The PML variables are stored on the ghost points of the six faces of the cube
!
!   vra, vrab : ??
!
!   v1a, v1b : left and right side (r=0,1)
!     dimensions:  (ng=numberOfGhostPoints, n1a=gridIndexRange(0,0), etc)
!       v1a(nd1a:n1a+ng-1,nd2a:nd2b,nd3a:nd3b,0:*)
!       v1b(n1b-ng+1:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
!   v2a, v2b : bottom and top (s=0,1)
!       v2a(nd1a:nd1b,nd2a:n2a+ng-1,nd3a:nd3b,0:*)
!   v3a, v3b : front and back (t=0,1)
!
!   v1a,v1am,v1an w1a,w1am,w1an : v and w at times t,t-dt,t+dt for the left side (r=0)
! ===================================================================================

      implicit none

      integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, ndf1a,ndf1b,ndf2a,
     & ndf2b,ndf3a,ndf3b,n1a,n1b,n2a,n2b,n3a,n3b, ndc, bc,ierr

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

! ..............

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

!     --- local variables ----

      integer side,axis,gridType,orderOfAccuracy,orderOfExtrapolation,
     & useForcing,ex,ey,ez,hx,hy,hz,useWhereMask,grid,debug,side1,
     & side2,side3
      real dx(0:2),dr(0:2),t,ep,dt,c
      real dxa,dya,dza
      integer axisp1,axisp2,i1,i2,i3,is1,is2,is3,js1,js2,js3,ks1,ks2,
     & ks3
      integer numberOfGhostPoints
      integer bc1,bc2

      real expsdt
      real ux,uy,uz,uxx,uyy,uzz,uxxx,uyyy,uzzz,uxxxx,uyyyy,uzzzz,uxxyy,
     & uxxzz,uyyzz,uxxy,uxxz,uxyy,uxzz,uyzz,uyyz

      real uLap,uLapsq,uLapx,uLapy,uLapz,uLapxx,uLapyy,uLapzz
      real ut,uxt,uyt,uzt,uxtt,uytt,uztt,uxxt,uyyt,uzzt,uxxtt,uyytt,
     & uzztt

      real v,vx,vy,vz,vxx,vyy,vzz,vxxx,vyyy,vzzz,vxyy,vxxy,vxxz,vyyz,
     & vxzz,vyzz,vt,vtt,vttt,vxt,vyt,vzt,vxtt,vytt,vztt,vtttt
      real vLapx,vLapy,vLapz
      real w,wx,wy,wz,wxx,wyy,wzz,wxxx,wyyy,wzzz,wxyy,wxxy,wt,wtt,wttt,
     & wxt,wxtt,wtttt


      ! Box types:
      integer xSide,ySide,zSide,xyEdge,xzEdge,yzEdge,xyzCorner
      parameter( xSide=0,ySide=1,zSide=2,xyEdge=3,xzEdge=4,yzEdge=5,
     & xyzCorner=6 )

      integer md1a,md1b,md2a,md2b,md3a,md3b
      integer m1a,m1b,m2a,m2b,m3a,m3b,i1a,i1b,i2a,i2b,i3a,i3b
      integer nb,power,numberOfBoxes,boxType,assignInterior
      real layerStrength,xScale,yScale,zScale,xx,yy,zz,csq,cdtsq,cxy,
     & cdt4Over12
      real sigma,sigma1,sigma2,sigma3,sigma1x,sigma2y,sigma3z
      integer box(0:9,26)

      ! boundary conditions parameters
! define BC parameters for fortran routines
! boundary conditions
      integer dirichlet,perfectElectricalConductor,
     & perfectMagneticConductor,planeWaveBoundaryCondition,
     & interfaceBC,symmetryBoundaryCondition,abcEM2,abcPML,abc3,abc4,
     & abc5,rbcNonLocal,rbcLocal,lastBC
      parameter( dirichlet=1,perfectElectricalConductor=2,
     & perfectMagneticConductor=3,planeWaveBoundaryCondition=4,
     & symmetryBoundaryCondition=5,interfaceBC=6,abcEM2=7,abcPML=8,
     & abc3=9,abc4=10,abc5=11,rbcNonLocal=12,rbcLocal=13,lastBC=13 )

      integer rectangular,curvilinear
      parameter(rectangular=0,curvilinear=1)


!     --- start statement function ----
      integer kd,m,n
      real rx,ry,rz,sx,sy,sz,tx,ty,tz


       real d12
       real d22
       real h12
       real h22
       real rxr2
       real rxs2
       real rxt2
       real rxrr2
       real rxss2
       real rxrs2
       real ryr2
       real rys2
       real ryt2
       real ryrr2
       real ryss2
       real ryrs2
       real rzr2
       real rzs2
       real rzt2
       real rzrr2
       real rzss2
       real rzrs2
       real sxr2
       real sxs2
       real sxt2
       real sxrr2
       real sxss2
       real sxrs2
       real syr2
       real sys2
       real syt2
       real syrr2
       real syss2
       real syrs2
       real szr2
       real szs2
       real szt2
       real szrr2
       real szss2
       real szrs2
       real txr2
       real txs2
       real txt2
       real txrr2
       real txss2
       real txrs2
       real tyr2
       real tys2
       real tyt2
       real tyrr2
       real tyss2
       real tyrs2
       real tzr2
       real tzs2
       real tzt2
       real tzrr2
       real tzss2
       real tzrs2
       real rxx21
       real rxx22
       real rxy22
       real rxx23
       real rxy23
       real rxz23
       real ryx22
       real ryy22
       real ryx23
       real ryy23
       real ryz23
       real rzx22
       real rzy22
       real rzx23
       real rzy23
       real rzz23
       real sxx22
       real sxy22
       real sxx23
       real sxy23
       real sxz23
       real syx22
       real syy22
       real syx23
       real syy23
       real syz23
       real szx22
       real szy22
       real szx23
       real szy23
       real szz23
       real txx22
       real txy22
       real txx23
       real txy23
       real txz23
       real tyx22
       real tyy22
       real tyx23
       real tyy23
       real tyz23
       real tzx22
       real tzy22
       real tzx23
       real tzy23
       real tzz23
       real ur2
       real us2
       real ut2
       real urr2
       real uss2
       real urs2
       real utt2
       real urt2
       real ust2
       real urrr2
       real usss2
       real uttt2
       real ux21
       real uy21
       real uz21
       real ux22
       real uy22
       real uz22
       real ux23
       real uy23
       real uz23
       real uxx21
       real uyy21
       real uxy21
       real uxz21
       real uyz21
       real uzz21
       real ulaplacian21
       real uxx22
       real uyy22
       real uxy22
       real uxz22
       real uyz22
       real uzz22
       real ulaplacian22
       real uxx23
       real uyy23
       real uzz23
       real uxy23
       real uxz23
       real uyz23
       real ulaplacian23
       real ux23r
       real uy23r
       real uz23r
       real uxx23r
       real uyy23r
       real uxy23r
       real uzz23r
       real uxz23r
       real uyz23r
       real ux21r
       real uy21r
       real uz21r
       real uxx21r
       real uyy21r
       real uzz21r
       real uxy21r
       real uxz21r
       real uyz21r
       real ulaplacian21r
       real ux22r
       real uy22r
       real uz22r
       real uxx22r
       real uyy22r
       real uzz22r
       real uxy22r
       real uxz22r
       real uyz22r
       real ulaplacian22r
       real ulaplacian23r
       real uxxx22r
       real uyyy22r
       real uxxy22r
       real uxyy22r
       real uxxxx22r
       real uyyyy22r
       real uxxyy22r
       real uxxx23r
       real uyyy23r
       real uzzz23r
       real uxxy23r
       real uxxz23r
       real uxyy23r
       real uyyz23r
       real uxzz23r
       real uyzz23r
       real uxxxx23r
       real uyyyy23r
       real uzzzz23r
       real uxxyy23r
       real uxxzz23r
       real uyyzz23r
       real uLapSq22r
       real uLapSq23r
       real umr2
       real ums2
       real umt2
       real umrr2
       real umss2
       real umrs2
       real umtt2
       real umrt2
       real umst2
       real umrrr2
       real umsss2
       real umttt2
       real umx21
       real umy21
       real umz21
       real umx22
       real umy22
       real umz22
       real umx23
       real umy23
       real umz23
       real umxx21
       real umyy21
       real umxy21
       real umxz21
       real umyz21
       real umzz21
       real umlaplacian21
       real umxx22
       real umyy22
       real umxy22
       real umxz22
       real umyz22
       real umzz22
       real umlaplacian22
       real umxx23
       real umyy23
       real umzz23
       real umxy23
       real umxz23
       real umyz23
       real umlaplacian23
       real umx23r
       real umy23r
       real umz23r
       real umxx23r
       real umyy23r
       real umxy23r
       real umzz23r
       real umxz23r
       real umyz23r
       real umx21r
       real umy21r
       real umz21r
       real umxx21r
       real umyy21r
       real umzz21r
       real umxy21r
       real umxz21r
       real umyz21r
       real umlaplacian21r
       real umx22r
       real umy22r
       real umz22r
       real umxx22r
       real umyy22r
       real umzz22r
       real umxy22r
       real umxz22r
       real umyz22r
       real umlaplacian22r
       real umlaplacian23r
       real umxxx22r
       real umyyy22r
       real umxxy22r
       real umxyy22r
       real umxxxx22r
       real umyyyy22r
       real umxxyy22r
       real umxxx23r
       real umyyy23r
       real umzzz23r
       real umxxy23r
       real umxxz23r
       real umxyy23r
       real umyyz23r
       real umxzz23r
       real umyzz23r
       real umxxxx23r
       real umyyyy23r
       real umzzzz23r
       real umxxyy23r
       real umxxzz23r
       real umyyzz23r
       real umLapSq22r
       real umLapSq23r

       real vrar2
       real vras2
       real vrat2
       real vrarr2
       real vrass2
       real vrars2
       real vratt2
       real vrart2
       real vrast2
       real vrarrr2
       real vrasss2
       real vrattt2
       real vrax21
       real vray21
       real vraz21
       real vrax22
       real vray22
       real vraz22
       real vrax23
       real vray23
       real vraz23
       real vraxx21
       real vrayy21
       real vraxy21
       real vraxz21
       real vrayz21
       real vrazz21
       real vralaplacian21
       real vraxx22
       real vrayy22
       real vraxy22
       real vraxz22
       real vrayz22
       real vrazz22
       real vralaplacian22
       real vraxx23
       real vrayy23
       real vrazz23
       real vraxy23
       real vraxz23
       real vrayz23
       real vralaplacian23
       real vrax23r
       real vray23r
       real vraz23r
       real vraxx23r
       real vrayy23r
       real vraxy23r
       real vrazz23r
       real vraxz23r
       real vrayz23r
       real vrax21r
       real vray21r
       real vraz21r
       real vraxx21r
       real vrayy21r
       real vrazz21r
       real vraxy21r
       real vraxz21r
       real vrayz21r
       real vralaplacian21r
       real vrax22r
       real vray22r
       real vraz22r
       real vraxx22r
       real vrayy22r
       real vrazz22r
       real vraxy22r
       real vraxz22r
       real vrayz22r
       real vralaplacian22r
       real vralaplacian23r
       real vraxxx22r
       real vrayyy22r
       real vraxxy22r
       real vraxyy22r
       real vraxxxx22r
       real vrayyyy22r
       real vraxxyy22r
       real vraxxx23r
       real vrayyy23r
       real vrazzz23r
       real vraxxy23r
       real vraxxz23r
       real vraxyy23r
       real vrayyz23r
       real vraxzz23r
       real vrayzz23r
       real vraxxxx23r
       real vrayyyy23r
       real vrazzzz23r
       real vraxxyy23r
       real vraxxzz23r
       real vrayyzz23r
       real vraLapSq22r
       real vraLapSq23r
       real vramr2
       real vrams2
       real vramt2
       real vramrr2
       real vramss2
       real vramrs2
       real vramtt2
       real vramrt2
       real vramst2
       real vramrrr2
       real vramsss2
       real vramttt2
       real vramx21
       real vramy21
       real vramz21
       real vramx22
       real vramy22
       real vramz22
       real vramx23
       real vramy23
       real vramz23
       real vramxx21
       real vramyy21
       real vramxy21
       real vramxz21
       real vramyz21
       real vramzz21
       real vramlaplacian21
       real vramxx22
       real vramyy22
       real vramxy22
       real vramxz22
       real vramyz22
       real vramzz22
       real vramlaplacian22
       real vramxx23
       real vramyy23
       real vramzz23
       real vramxy23
       real vramxz23
       real vramyz23
       real vramlaplacian23
       real vramx23r
       real vramy23r
       real vramz23r
       real vramxx23r
       real vramyy23r
       real vramxy23r
       real vramzz23r
       real vramxz23r
       real vramyz23r
       real vramx21r
       real vramy21r
       real vramz21r
       real vramxx21r
       real vramyy21r
       real vramzz21r
       real vramxy21r
       real vramxz21r
       real vramyz21r
       real vramlaplacian21r
       real vramx22r
       real vramy22r
       real vramz22r
       real vramxx22r
       real vramyy22r
       real vramzz22r
       real vramxy22r
       real vramxz22r
       real vramyz22r
       real vramlaplacian22r
       real vramlaplacian23r
       real vramxxx22r
       real vramyyy22r
       real vramxxy22r
       real vramxyy22r
       real vramxxxx22r
       real vramyyyy22r
       real vramxxyy22r
       real vramxxx23r
       real vramyyy23r
       real vramzzz23r
       real vramxxy23r
       real vramxxz23r
       real vramxyy23r
       real vramyyz23r
       real vramxzz23r
       real vramyzz23r
       real vramxxxx23r
       real vramyyyy23r
       real vramzzzz23r
       real vramxxyy23r
       real vramxxzz23r
       real vramyyzz23r
       real vramLapSq22r
       real vramLapSq23r
       real wrar2
       real wras2
       real wrat2
       real wrarr2
       real wrass2
       real wrars2
       real wratt2
       real wrart2
       real wrast2
       real wrarrr2
       real wrasss2
       real wrattt2
       real wrax21
       real wray21
       real wraz21
       real wrax22
       real wray22
       real wraz22
       real wrax23
       real wray23
       real wraz23
       real wraxx21
       real wrayy21
       real wraxy21
       real wraxz21
       real wrayz21
       real wrazz21
       real wralaplacian21
       real wraxx22
       real wrayy22
       real wraxy22
       real wraxz22
       real wrayz22
       real wrazz22
       real wralaplacian22
       real wraxx23
       real wrayy23
       real wrazz23
       real wraxy23
       real wraxz23
       real wrayz23
       real wralaplacian23
       real wrax23r
       real wray23r
       real wraz23r
       real wraxx23r
       real wrayy23r
       real wraxy23r
       real wrazz23r
       real wraxz23r
       real wrayz23r
       real wrax21r
       real wray21r
       real wraz21r
       real wraxx21r
       real wrayy21r
       real wrazz21r
       real wraxy21r
       real wraxz21r
       real wrayz21r
       real wralaplacian21r
       real wrax22r
       real wray22r
       real wraz22r
       real wraxx22r
       real wrayy22r
       real wrazz22r
       real wraxy22r
       real wraxz22r
       real wrayz22r
       real wralaplacian22r
       real wralaplacian23r
       real wraxxx22r
       real wrayyy22r
       real wraxxy22r
       real wraxyy22r
       real wraxxxx22r
       real wrayyyy22r
       real wraxxyy22r
       real wraxxx23r
       real wrayyy23r
       real wrazzz23r
       real wraxxy23r
       real wraxxz23r
       real wraxyy23r
       real wrayyz23r
       real wraxzz23r
       real wrayzz23r
       real wraxxxx23r
       real wrayyyy23r
       real wrazzzz23r
       real wraxxyy23r
       real wraxxzz23r
       real wrayyzz23r
       real wraLapSq22r
       real wraLapSq23r
       real wramr2
       real wrams2
       real wramt2
       real wramrr2
       real wramss2
       real wramrs2
       real wramtt2
       real wramrt2
       real wramst2
       real wramrrr2
       real wramsss2
       real wramttt2
       real wramx21
       real wramy21
       real wramz21
       real wramx22
       real wramy22
       real wramz22
       real wramx23
       real wramy23
       real wramz23
       real wramxx21
       real wramyy21
       real wramxy21
       real wramxz21
       real wramyz21
       real wramzz21
       real wramlaplacian21
       real wramxx22
       real wramyy22
       real wramxy22
       real wramxz22
       real wramyz22
       real wramzz22
       real wramlaplacian22
       real wramxx23
       real wramyy23
       real wramzz23
       real wramxy23
       real wramxz23
       real wramyz23
       real wramlaplacian23
       real wramx23r
       real wramy23r
       real wramz23r
       real wramxx23r
       real wramyy23r
       real wramxy23r
       real wramzz23r
       real wramxz23r
       real wramyz23r
       real wramx21r
       real wramy21r
       real wramz21r
       real wramxx21r
       real wramyy21r
       real wramzz21r
       real wramxy21r
       real wramxz21r
       real wramyz21r
       real wramlaplacian21r
       real wramx22r
       real wramy22r
       real wramz22r
       real wramxx22r
       real wramyy22r
       real wramzz22r
       real wramxy22r
       real wramxz22r
       real wramyz22r
       real wramlaplacian22r
       real wramlaplacian23r
       real wramxxx22r
       real wramyyy22r
       real wramxxy22r
       real wramxyy22r
       real wramxxxx22r
       real wramyyyy22r
       real wramxxyy22r
       real wramxxx23r
       real wramyyy23r
       real wramzzz23r
       real wramxxy23r
       real wramxxz23r
       real wramxyy23r
       real wramyyz23r
       real wramxzz23r
       real wramyzz23r
       real wramxxxx23r
       real wramyyyy23r
       real wramzzzz23r
       real wramxxyy23r
       real wramxxzz23r
       real wramyyzz23r
       real wramLapSq22r
       real wramLapSq23r

       real vrbr2
       real vrbs2
       real vrbt2
       real vrbrr2
       real vrbss2
       real vrbrs2
       real vrbtt2
       real vrbrt2
       real vrbst2
       real vrbrrr2
       real vrbsss2
       real vrbttt2
       real vrbx21
       real vrby21
       real vrbz21
       real vrbx22
       real vrby22
       real vrbz22
       real vrbx23
       real vrby23
       real vrbz23
       real vrbxx21
       real vrbyy21
       real vrbxy21
       real vrbxz21
       real vrbyz21
       real vrbzz21
       real vrblaplacian21
       real vrbxx22
       real vrbyy22
       real vrbxy22
       real vrbxz22
       real vrbyz22
       real vrbzz22
       real vrblaplacian22
       real vrbxx23
       real vrbyy23
       real vrbzz23
       real vrbxy23
       real vrbxz23
       real vrbyz23
       real vrblaplacian23
       real vrbx23r
       real vrby23r
       real vrbz23r
       real vrbxx23r
       real vrbyy23r
       real vrbxy23r
       real vrbzz23r
       real vrbxz23r
       real vrbyz23r
       real vrbx21r
       real vrby21r
       real vrbz21r
       real vrbxx21r
       real vrbyy21r
       real vrbzz21r
       real vrbxy21r
       real vrbxz21r
       real vrbyz21r
       real vrblaplacian21r
       real vrbx22r
       real vrby22r
       real vrbz22r
       real vrbxx22r
       real vrbyy22r
       real vrbzz22r
       real vrbxy22r
       real vrbxz22r
       real vrbyz22r
       real vrblaplacian22r
       real vrblaplacian23r
       real vrbxxx22r
       real vrbyyy22r
       real vrbxxy22r
       real vrbxyy22r
       real vrbxxxx22r
       real vrbyyyy22r
       real vrbxxyy22r
       real vrbxxx23r
       real vrbyyy23r
       real vrbzzz23r
       real vrbxxy23r
       real vrbxxz23r
       real vrbxyy23r
       real vrbyyz23r
       real vrbxzz23r
       real vrbyzz23r
       real vrbxxxx23r
       real vrbyyyy23r
       real vrbzzzz23r
       real vrbxxyy23r
       real vrbxxzz23r
       real vrbyyzz23r
       real vrbLapSq22r
       real vrbLapSq23r
       real vrbmr2
       real vrbms2
       real vrbmt2
       real vrbmrr2
       real vrbmss2
       real vrbmrs2
       real vrbmtt2
       real vrbmrt2
       real vrbmst2
       real vrbmrrr2
       real vrbmsss2
       real vrbmttt2
       real vrbmx21
       real vrbmy21
       real vrbmz21
       real vrbmx22
       real vrbmy22
       real vrbmz22
       real vrbmx23
       real vrbmy23
       real vrbmz23
       real vrbmxx21
       real vrbmyy21
       real vrbmxy21
       real vrbmxz21
       real vrbmyz21
       real vrbmzz21
       real vrbmlaplacian21
       real vrbmxx22
       real vrbmyy22
       real vrbmxy22
       real vrbmxz22
       real vrbmyz22
       real vrbmzz22
       real vrbmlaplacian22
       real vrbmxx23
       real vrbmyy23
       real vrbmzz23
       real vrbmxy23
       real vrbmxz23
       real vrbmyz23
       real vrbmlaplacian23
       real vrbmx23r
       real vrbmy23r
       real vrbmz23r
       real vrbmxx23r
       real vrbmyy23r
       real vrbmxy23r
       real vrbmzz23r
       real vrbmxz23r
       real vrbmyz23r
       real vrbmx21r
       real vrbmy21r
       real vrbmz21r
       real vrbmxx21r
       real vrbmyy21r
       real vrbmzz21r
       real vrbmxy21r
       real vrbmxz21r
       real vrbmyz21r
       real vrbmlaplacian21r
       real vrbmx22r
       real vrbmy22r
       real vrbmz22r
       real vrbmxx22r
       real vrbmyy22r
       real vrbmzz22r
       real vrbmxy22r
       real vrbmxz22r
       real vrbmyz22r
       real vrbmlaplacian22r
       real vrbmlaplacian23r
       real vrbmxxx22r
       real vrbmyyy22r
       real vrbmxxy22r
       real vrbmxyy22r
       real vrbmxxxx22r
       real vrbmyyyy22r
       real vrbmxxyy22r
       real vrbmxxx23r
       real vrbmyyy23r
       real vrbmzzz23r
       real vrbmxxy23r
       real vrbmxxz23r
       real vrbmxyy23r
       real vrbmyyz23r
       real vrbmxzz23r
       real vrbmyzz23r
       real vrbmxxxx23r
       real vrbmyyyy23r
       real vrbmzzzz23r
       real vrbmxxyy23r
       real vrbmxxzz23r
       real vrbmyyzz23r
       real vrbmLapSq22r
       real vrbmLapSq23r
       real wrbr2
       real wrbs2
       real wrbt2
       real wrbrr2
       real wrbss2
       real wrbrs2
       real wrbtt2
       real wrbrt2
       real wrbst2
       real wrbrrr2
       real wrbsss2
       real wrbttt2
       real wrbx21
       real wrby21
       real wrbz21
       real wrbx22
       real wrby22
       real wrbz22
       real wrbx23
       real wrby23
       real wrbz23
       real wrbxx21
       real wrbyy21
       real wrbxy21
       real wrbxz21
       real wrbyz21
       real wrbzz21
       real wrblaplacian21
       real wrbxx22
       real wrbyy22
       real wrbxy22
       real wrbxz22
       real wrbyz22
       real wrbzz22
       real wrblaplacian22
       real wrbxx23
       real wrbyy23
       real wrbzz23
       real wrbxy23
       real wrbxz23
       real wrbyz23
       real wrblaplacian23
       real wrbx23r
       real wrby23r
       real wrbz23r
       real wrbxx23r
       real wrbyy23r
       real wrbxy23r
       real wrbzz23r
       real wrbxz23r
       real wrbyz23r
       real wrbx21r
       real wrby21r
       real wrbz21r
       real wrbxx21r
       real wrbyy21r
       real wrbzz21r
       real wrbxy21r
       real wrbxz21r
       real wrbyz21r
       real wrblaplacian21r
       real wrbx22r
       real wrby22r
       real wrbz22r
       real wrbxx22r
       real wrbyy22r
       real wrbzz22r
       real wrbxy22r
       real wrbxz22r
       real wrbyz22r
       real wrblaplacian22r
       real wrblaplacian23r
       real wrbxxx22r
       real wrbyyy22r
       real wrbxxy22r
       real wrbxyy22r
       real wrbxxxx22r
       real wrbyyyy22r
       real wrbxxyy22r
       real wrbxxx23r
       real wrbyyy23r
       real wrbzzz23r
       real wrbxxy23r
       real wrbxxz23r
       real wrbxyy23r
       real wrbyyz23r
       real wrbxzz23r
       real wrbyzz23r
       real wrbxxxx23r
       real wrbyyyy23r
       real wrbzzzz23r
       real wrbxxyy23r
       real wrbxxzz23r
       real wrbyyzz23r
       real wrbLapSq22r
       real wrbLapSq23r
       real wrbmr2
       real wrbms2
       real wrbmt2
       real wrbmrr2
       real wrbmss2
       real wrbmrs2
       real wrbmtt2
       real wrbmrt2
       real wrbmst2
       real wrbmrrr2
       real wrbmsss2
       real wrbmttt2
       real wrbmx21
       real wrbmy21
       real wrbmz21
       real wrbmx22
       real wrbmy22
       real wrbmz22
       real wrbmx23
       real wrbmy23
       real wrbmz23
       real wrbmxx21
       real wrbmyy21
       real wrbmxy21
       real wrbmxz21
       real wrbmyz21
       real wrbmzz21
       real wrbmlaplacian21
       real wrbmxx22
       real wrbmyy22
       real wrbmxy22
       real wrbmxz22
       real wrbmyz22
       real wrbmzz22
       real wrbmlaplacian22
       real wrbmxx23
       real wrbmyy23
       real wrbmzz23
       real wrbmxy23
       real wrbmxz23
       real wrbmyz23
       real wrbmlaplacian23
       real wrbmx23r
       real wrbmy23r
       real wrbmz23r
       real wrbmxx23r
       real wrbmyy23r
       real wrbmxy23r
       real wrbmzz23r
       real wrbmxz23r
       real wrbmyz23r
       real wrbmx21r
       real wrbmy21r
       real wrbmz21r
       real wrbmxx21r
       real wrbmyy21r
       real wrbmzz21r
       real wrbmxy21r
       real wrbmxz21r
       real wrbmyz21r
       real wrbmlaplacian21r
       real wrbmx22r
       real wrbmy22r
       real wrbmz22r
       real wrbmxx22r
       real wrbmyy22r
       real wrbmzz22r
       real wrbmxy22r
       real wrbmxz22r
       real wrbmyz22r
       real wrbmlaplacian22r
       real wrbmlaplacian23r
       real wrbmxxx22r
       real wrbmyyy22r
       real wrbmxxy22r
       real wrbmxyy22r
       real wrbmxxxx22r
       real wrbmyyyy22r
       real wrbmxxyy22r
       real wrbmxxx23r
       real wrbmyyy23r
       real wrbmzzz23r
       real wrbmxxy23r
       real wrbmxxz23r
       real wrbmxyy23r
       real wrbmyyz23r
       real wrbmxzz23r
       real wrbmyzz23r
       real wrbmxxxx23r
       real wrbmyyyy23r
       real wrbmzzzz23r
       real wrbmxxyy23r
       real wrbmxxzz23r
       real wrbmyyzz23r
       real wrbmLapSq22r
       real wrbmLapSq23r

       real vsar2
       real vsas2
       real vsat2
       real vsarr2
       real vsass2
       real vsars2
       real vsatt2
       real vsart2
       real vsast2
       real vsarrr2
       real vsasss2
       real vsattt2
       real vsax21
       real vsay21
       real vsaz21
       real vsax22
       real vsay22
       real vsaz22
       real vsax23
       real vsay23
       real vsaz23
       real vsaxx21
       real vsayy21
       real vsaxy21
       real vsaxz21
       real vsayz21
       real vsazz21
       real vsalaplacian21
       real vsaxx22
       real vsayy22
       real vsaxy22
       real vsaxz22
       real vsayz22
       real vsazz22
       real vsalaplacian22
       real vsaxx23
       real vsayy23
       real vsazz23
       real vsaxy23
       real vsaxz23
       real vsayz23
       real vsalaplacian23
       real vsax23r
       real vsay23r
       real vsaz23r
       real vsaxx23r
       real vsayy23r
       real vsaxy23r
       real vsazz23r
       real vsaxz23r
       real vsayz23r
       real vsax21r
       real vsay21r
       real vsaz21r
       real vsaxx21r
       real vsayy21r
       real vsazz21r
       real vsaxy21r
       real vsaxz21r
       real vsayz21r
       real vsalaplacian21r
       real vsax22r
       real vsay22r
       real vsaz22r
       real vsaxx22r
       real vsayy22r
       real vsazz22r
       real vsaxy22r
       real vsaxz22r
       real vsayz22r
       real vsalaplacian22r
       real vsalaplacian23r
       real vsaxxx22r
       real vsayyy22r
       real vsaxxy22r
       real vsaxyy22r
       real vsaxxxx22r
       real vsayyyy22r
       real vsaxxyy22r
       real vsaxxx23r
       real vsayyy23r
       real vsazzz23r
       real vsaxxy23r
       real vsaxxz23r
       real vsaxyy23r
       real vsayyz23r
       real vsaxzz23r
       real vsayzz23r
       real vsaxxxx23r
       real vsayyyy23r
       real vsazzzz23r
       real vsaxxyy23r
       real vsaxxzz23r
       real vsayyzz23r
       real vsaLapSq22r
       real vsaLapSq23r
       real vsamr2
       real vsams2
       real vsamt2
       real vsamrr2
       real vsamss2
       real vsamrs2
       real vsamtt2
       real vsamrt2
       real vsamst2
       real vsamrrr2
       real vsamsss2
       real vsamttt2
       real vsamx21
       real vsamy21
       real vsamz21
       real vsamx22
       real vsamy22
       real vsamz22
       real vsamx23
       real vsamy23
       real vsamz23
       real vsamxx21
       real vsamyy21
       real vsamxy21
       real vsamxz21
       real vsamyz21
       real vsamzz21
       real vsamlaplacian21
       real vsamxx22
       real vsamyy22
       real vsamxy22
       real vsamxz22
       real vsamyz22
       real vsamzz22
       real vsamlaplacian22
       real vsamxx23
       real vsamyy23
       real vsamzz23
       real vsamxy23
       real vsamxz23
       real vsamyz23
       real vsamlaplacian23
       real vsamx23r
       real vsamy23r
       real vsamz23r
       real vsamxx23r
       real vsamyy23r
       real vsamxy23r
       real vsamzz23r
       real vsamxz23r
       real vsamyz23r
       real vsamx21r
       real vsamy21r
       real vsamz21r
       real vsamxx21r
       real vsamyy21r
       real vsamzz21r
       real vsamxy21r
       real vsamxz21r
       real vsamyz21r
       real vsamlaplacian21r
       real vsamx22r
       real vsamy22r
       real vsamz22r
       real vsamxx22r
       real vsamyy22r
       real vsamzz22r
       real vsamxy22r
       real vsamxz22r
       real vsamyz22r
       real vsamlaplacian22r
       real vsamlaplacian23r
       real vsamxxx22r
       real vsamyyy22r
       real vsamxxy22r
       real vsamxyy22r
       real vsamxxxx22r
       real vsamyyyy22r
       real vsamxxyy22r
       real vsamxxx23r
       real vsamyyy23r
       real vsamzzz23r
       real vsamxxy23r
       real vsamxxz23r
       real vsamxyy23r
       real vsamyyz23r
       real vsamxzz23r
       real vsamyzz23r
       real vsamxxxx23r
       real vsamyyyy23r
       real vsamzzzz23r
       real vsamxxyy23r
       real vsamxxzz23r
       real vsamyyzz23r
       real vsamLapSq22r
       real vsamLapSq23r
       real wsar2
       real wsas2
       real wsat2
       real wsarr2
       real wsass2
       real wsars2
       real wsatt2
       real wsart2
       real wsast2
       real wsarrr2
       real wsasss2
       real wsattt2
       real wsax21
       real wsay21
       real wsaz21
       real wsax22
       real wsay22
       real wsaz22
       real wsax23
       real wsay23
       real wsaz23
       real wsaxx21
       real wsayy21
       real wsaxy21
       real wsaxz21
       real wsayz21
       real wsazz21
       real wsalaplacian21
       real wsaxx22
       real wsayy22
       real wsaxy22
       real wsaxz22
       real wsayz22
       real wsazz22
       real wsalaplacian22
       real wsaxx23
       real wsayy23
       real wsazz23
       real wsaxy23
       real wsaxz23
       real wsayz23
       real wsalaplacian23
       real wsax23r
       real wsay23r
       real wsaz23r
       real wsaxx23r
       real wsayy23r
       real wsaxy23r
       real wsazz23r
       real wsaxz23r
       real wsayz23r
       real wsax21r
       real wsay21r
       real wsaz21r
       real wsaxx21r
       real wsayy21r
       real wsazz21r
       real wsaxy21r
       real wsaxz21r
       real wsayz21r
       real wsalaplacian21r
       real wsax22r
       real wsay22r
       real wsaz22r
       real wsaxx22r
       real wsayy22r
       real wsazz22r
       real wsaxy22r
       real wsaxz22r
       real wsayz22r
       real wsalaplacian22r
       real wsalaplacian23r
       real wsaxxx22r
       real wsayyy22r
       real wsaxxy22r
       real wsaxyy22r
       real wsaxxxx22r
       real wsayyyy22r
       real wsaxxyy22r
       real wsaxxx23r
       real wsayyy23r
       real wsazzz23r
       real wsaxxy23r
       real wsaxxz23r
       real wsaxyy23r
       real wsayyz23r
       real wsaxzz23r
       real wsayzz23r
       real wsaxxxx23r
       real wsayyyy23r
       real wsazzzz23r
       real wsaxxyy23r
       real wsaxxzz23r
       real wsayyzz23r
       real wsaLapSq22r
       real wsaLapSq23r
       real wsamr2
       real wsams2
       real wsamt2
       real wsamrr2
       real wsamss2
       real wsamrs2
       real wsamtt2
       real wsamrt2
       real wsamst2
       real wsamrrr2
       real wsamsss2
       real wsamttt2
       real wsamx21
       real wsamy21
       real wsamz21
       real wsamx22
       real wsamy22
       real wsamz22
       real wsamx23
       real wsamy23
       real wsamz23
       real wsamxx21
       real wsamyy21
       real wsamxy21
       real wsamxz21
       real wsamyz21
       real wsamzz21
       real wsamlaplacian21
       real wsamxx22
       real wsamyy22
       real wsamxy22
       real wsamxz22
       real wsamyz22
       real wsamzz22
       real wsamlaplacian22
       real wsamxx23
       real wsamyy23
       real wsamzz23
       real wsamxy23
       real wsamxz23
       real wsamyz23
       real wsamlaplacian23
       real wsamx23r
       real wsamy23r
       real wsamz23r
       real wsamxx23r
       real wsamyy23r
       real wsamxy23r
       real wsamzz23r
       real wsamxz23r
       real wsamyz23r
       real wsamx21r
       real wsamy21r
       real wsamz21r
       real wsamxx21r
       real wsamyy21r
       real wsamzz21r
       real wsamxy21r
       real wsamxz21r
       real wsamyz21r
       real wsamlaplacian21r
       real wsamx22r
       real wsamy22r
       real wsamz22r
       real wsamxx22r
       real wsamyy22r
       real wsamzz22r
       real wsamxy22r
       real wsamxz22r
       real wsamyz22r
       real wsamlaplacian22r
       real wsamlaplacian23r
       real wsamxxx22r
       real wsamyyy22r
       real wsamxxy22r
       real wsamxyy22r
       real wsamxxxx22r
       real wsamyyyy22r
       real wsamxxyy22r
       real wsamxxx23r
       real wsamyyy23r
       real wsamzzz23r
       real wsamxxy23r
       real wsamxxz23r
       real wsamxyy23r
       real wsamyyz23r
       real wsamxzz23r
       real wsamyzz23r
       real wsamxxxx23r
       real wsamyyyy23r
       real wsamzzzz23r
       real wsamxxyy23r
       real wsamxxzz23r
       real wsamyyzz23r
       real wsamLapSq22r
       real wsamLapSq23r

       real vsbr2
       real vsbs2
       real vsbt2
       real vsbrr2
       real vsbss2
       real vsbrs2
       real vsbtt2
       real vsbrt2
       real vsbst2
       real vsbrrr2
       real vsbsss2
       real vsbttt2
       real vsbx21
       real vsby21
       real vsbz21
       real vsbx22
       real vsby22
       real vsbz22
       real vsbx23
       real vsby23
       real vsbz23
       real vsbxx21
       real vsbyy21
       real vsbxy21
       real vsbxz21
       real vsbyz21
       real vsbzz21
       real vsblaplacian21
       real vsbxx22
       real vsbyy22
       real vsbxy22
       real vsbxz22
       real vsbyz22
       real vsbzz22
       real vsblaplacian22
       real vsbxx23
       real vsbyy23
       real vsbzz23
       real vsbxy23
       real vsbxz23
       real vsbyz23
       real vsblaplacian23
       real vsbx23r
       real vsby23r
       real vsbz23r
       real vsbxx23r
       real vsbyy23r
       real vsbxy23r
       real vsbzz23r
       real vsbxz23r
       real vsbyz23r
       real vsbx21r
       real vsby21r
       real vsbz21r
       real vsbxx21r
       real vsbyy21r
       real vsbzz21r
       real vsbxy21r
       real vsbxz21r
       real vsbyz21r
       real vsblaplacian21r
       real vsbx22r
       real vsby22r
       real vsbz22r
       real vsbxx22r
       real vsbyy22r
       real vsbzz22r
       real vsbxy22r
       real vsbxz22r
       real vsbyz22r
       real vsblaplacian22r
       real vsblaplacian23r
       real vsbxxx22r
       real vsbyyy22r
       real vsbxxy22r
       real vsbxyy22r
       real vsbxxxx22r
       real vsbyyyy22r
       real vsbxxyy22r
       real vsbxxx23r
       real vsbyyy23r
       real vsbzzz23r
       real vsbxxy23r
       real vsbxxz23r
       real vsbxyy23r
       real vsbyyz23r
       real vsbxzz23r
       real vsbyzz23r
       real vsbxxxx23r
       real vsbyyyy23r
       real vsbzzzz23r
       real vsbxxyy23r
       real vsbxxzz23r
       real vsbyyzz23r
       real vsbLapSq22r
       real vsbLapSq23r
       real vsbmr2
       real vsbms2
       real vsbmt2
       real vsbmrr2
       real vsbmss2
       real vsbmrs2
       real vsbmtt2
       real vsbmrt2
       real vsbmst2
       real vsbmrrr2
       real vsbmsss2
       real vsbmttt2
       real vsbmx21
       real vsbmy21
       real vsbmz21
       real vsbmx22
       real vsbmy22
       real vsbmz22
       real vsbmx23
       real vsbmy23
       real vsbmz23
       real vsbmxx21
       real vsbmyy21
       real vsbmxy21
       real vsbmxz21
       real vsbmyz21
       real vsbmzz21
       real vsbmlaplacian21
       real vsbmxx22
       real vsbmyy22
       real vsbmxy22
       real vsbmxz22
       real vsbmyz22
       real vsbmzz22
       real vsbmlaplacian22
       real vsbmxx23
       real vsbmyy23
       real vsbmzz23
       real vsbmxy23
       real vsbmxz23
       real vsbmyz23
       real vsbmlaplacian23
       real vsbmx23r
       real vsbmy23r
       real vsbmz23r
       real vsbmxx23r
       real vsbmyy23r
       real vsbmxy23r
       real vsbmzz23r
       real vsbmxz23r
       real vsbmyz23r
       real vsbmx21r
       real vsbmy21r
       real vsbmz21r
       real vsbmxx21r
       real vsbmyy21r
       real vsbmzz21r
       real vsbmxy21r
       real vsbmxz21r
       real vsbmyz21r
       real vsbmlaplacian21r
       real vsbmx22r
       real vsbmy22r
       real vsbmz22r
       real vsbmxx22r
       real vsbmyy22r
       real vsbmzz22r
       real vsbmxy22r
       real vsbmxz22r
       real vsbmyz22r
       real vsbmlaplacian22r
       real vsbmlaplacian23r
       real vsbmxxx22r
       real vsbmyyy22r
       real vsbmxxy22r
       real vsbmxyy22r
       real vsbmxxxx22r
       real vsbmyyyy22r
       real vsbmxxyy22r
       real vsbmxxx23r
       real vsbmyyy23r
       real vsbmzzz23r
       real vsbmxxy23r
       real vsbmxxz23r
       real vsbmxyy23r
       real vsbmyyz23r
       real vsbmxzz23r
       real vsbmyzz23r
       real vsbmxxxx23r
       real vsbmyyyy23r
       real vsbmzzzz23r
       real vsbmxxyy23r
       real vsbmxxzz23r
       real vsbmyyzz23r
       real vsbmLapSq22r
       real vsbmLapSq23r
       real wsbr2
       real wsbs2
       real wsbt2
       real wsbrr2
       real wsbss2
       real wsbrs2
       real wsbtt2
       real wsbrt2
       real wsbst2
       real wsbrrr2
       real wsbsss2
       real wsbttt2
       real wsbx21
       real wsby21
       real wsbz21
       real wsbx22
       real wsby22
       real wsbz22
       real wsbx23
       real wsby23
       real wsbz23
       real wsbxx21
       real wsbyy21
       real wsbxy21
       real wsbxz21
       real wsbyz21
       real wsbzz21
       real wsblaplacian21
       real wsbxx22
       real wsbyy22
       real wsbxy22
       real wsbxz22
       real wsbyz22
       real wsbzz22
       real wsblaplacian22
       real wsbxx23
       real wsbyy23
       real wsbzz23
       real wsbxy23
       real wsbxz23
       real wsbyz23
       real wsblaplacian23
       real wsbx23r
       real wsby23r
       real wsbz23r
       real wsbxx23r
       real wsbyy23r
       real wsbxy23r
       real wsbzz23r
       real wsbxz23r
       real wsbyz23r
       real wsbx21r
       real wsby21r
       real wsbz21r
       real wsbxx21r
       real wsbyy21r
       real wsbzz21r
       real wsbxy21r
       real wsbxz21r
       real wsbyz21r
       real wsblaplacian21r
       real wsbx22r
       real wsby22r
       real wsbz22r
       real wsbxx22r
       real wsbyy22r
       real wsbzz22r
       real wsbxy22r
       real wsbxz22r
       real wsbyz22r
       real wsblaplacian22r
       real wsblaplacian23r
       real wsbxxx22r
       real wsbyyy22r
       real wsbxxy22r
       real wsbxyy22r
       real wsbxxxx22r
       real wsbyyyy22r
       real wsbxxyy22r
       real wsbxxx23r
       real wsbyyy23r
       real wsbzzz23r
       real wsbxxy23r
       real wsbxxz23r
       real wsbxyy23r
       real wsbyyz23r
       real wsbxzz23r
       real wsbyzz23r
       real wsbxxxx23r
       real wsbyyyy23r
       real wsbzzzz23r
       real wsbxxyy23r
       real wsbxxzz23r
       real wsbyyzz23r
       real wsbLapSq22r
       real wsbLapSq23r
       real wsbmr2
       real wsbms2
       real wsbmt2
       real wsbmrr2
       real wsbmss2
       real wsbmrs2
       real wsbmtt2
       real wsbmrt2
       real wsbmst2
       real wsbmrrr2
       real wsbmsss2
       real wsbmttt2
       real wsbmx21
       real wsbmy21
       real wsbmz21
       real wsbmx22
       real wsbmy22
       real wsbmz22
       real wsbmx23
       real wsbmy23
       real wsbmz23
       real wsbmxx21
       real wsbmyy21
       real wsbmxy21
       real wsbmxz21
       real wsbmyz21
       real wsbmzz21
       real wsbmlaplacian21
       real wsbmxx22
       real wsbmyy22
       real wsbmxy22
       real wsbmxz22
       real wsbmyz22
       real wsbmzz22
       real wsbmlaplacian22
       real wsbmxx23
       real wsbmyy23
       real wsbmzz23
       real wsbmxy23
       real wsbmxz23
       real wsbmyz23
       real wsbmlaplacian23
       real wsbmx23r
       real wsbmy23r
       real wsbmz23r
       real wsbmxx23r
       real wsbmyy23r
       real wsbmxy23r
       real wsbmzz23r
       real wsbmxz23r
       real wsbmyz23r
       real wsbmx21r
       real wsbmy21r
       real wsbmz21r
       real wsbmxx21r
       real wsbmyy21r
       real wsbmzz21r
       real wsbmxy21r
       real wsbmxz21r
       real wsbmyz21r
       real wsbmlaplacian21r
       real wsbmx22r
       real wsbmy22r
       real wsbmz22r
       real wsbmxx22r
       real wsbmyy22r
       real wsbmzz22r
       real wsbmxy22r
       real wsbmxz22r
       real wsbmyz22r
       real wsbmlaplacian22r
       real wsbmlaplacian23r
       real wsbmxxx22r
       real wsbmyyy22r
       real wsbmxxy22r
       real wsbmxyy22r
       real wsbmxxxx22r
       real wsbmyyyy22r
       real wsbmxxyy22r
       real wsbmxxx23r
       real wsbmyyy23r
       real wsbmzzz23r
       real wsbmxxy23r
       real wsbmxxz23r
       real wsbmxyy23r
       real wsbmyyz23r
       real wsbmxzz23r
       real wsbmyzz23r
       real wsbmxxxx23r
       real wsbmyyyy23r
       real wsbmzzzz23r
       real wsbmxxyy23r
       real wsbmxxzz23r
       real wsbmyyzz23r
       real wsbmLapSq22r
       real wsbmLapSq23r

       real vtar2
       real vtas2
       real vtat2
       real vtarr2
       real vtass2
       real vtars2
       real vtatt2
       real vtart2
       real vtast2
       real vtarrr2
       real vtasss2
       real vtattt2
       real vtax21
       real vtay21
       real vtaz21
       real vtax22
       real vtay22
       real vtaz22
       real vtax23
       real vtay23
       real vtaz23
       real vtaxx21
       real vtayy21
       real vtaxy21
       real vtaxz21
       real vtayz21
       real vtazz21
       real vtalaplacian21
       real vtaxx22
       real vtayy22
       real vtaxy22
       real vtaxz22
       real vtayz22
       real vtazz22
       real vtalaplacian22
       real vtaxx23
       real vtayy23
       real vtazz23
       real vtaxy23
       real vtaxz23
       real vtayz23
       real vtalaplacian23
       real vtax23r
       real vtay23r
       real vtaz23r
       real vtaxx23r
       real vtayy23r
       real vtaxy23r
       real vtazz23r
       real vtaxz23r
       real vtayz23r
       real vtax21r
       real vtay21r
       real vtaz21r
       real vtaxx21r
       real vtayy21r
       real vtazz21r
       real vtaxy21r
       real vtaxz21r
       real vtayz21r
       real vtalaplacian21r
       real vtax22r
       real vtay22r
       real vtaz22r
       real vtaxx22r
       real vtayy22r
       real vtazz22r
       real vtaxy22r
       real vtaxz22r
       real vtayz22r
       real vtalaplacian22r
       real vtalaplacian23r
       real vtaxxx22r
       real vtayyy22r
       real vtaxxy22r
       real vtaxyy22r
       real vtaxxxx22r
       real vtayyyy22r
       real vtaxxyy22r
       real vtaxxx23r
       real vtayyy23r
       real vtazzz23r
       real vtaxxy23r
       real vtaxxz23r
       real vtaxyy23r
       real vtayyz23r
       real vtaxzz23r
       real vtayzz23r
       real vtaxxxx23r
       real vtayyyy23r
       real vtazzzz23r
       real vtaxxyy23r
       real vtaxxzz23r
       real vtayyzz23r
       real vtaLapSq22r
       real vtaLapSq23r
       real vtamr2
       real vtams2
       real vtamt2
       real vtamrr2
       real vtamss2
       real vtamrs2
       real vtamtt2
       real vtamrt2
       real vtamst2
       real vtamrrr2
       real vtamsss2
       real vtamttt2
       real vtamx21
       real vtamy21
       real vtamz21
       real vtamx22
       real vtamy22
       real vtamz22
       real vtamx23
       real vtamy23
       real vtamz23
       real vtamxx21
       real vtamyy21
       real vtamxy21
       real vtamxz21
       real vtamyz21
       real vtamzz21
       real vtamlaplacian21
       real vtamxx22
       real vtamyy22
       real vtamxy22
       real vtamxz22
       real vtamyz22
       real vtamzz22
       real vtamlaplacian22
       real vtamxx23
       real vtamyy23
       real vtamzz23
       real vtamxy23
       real vtamxz23
       real vtamyz23
       real vtamlaplacian23
       real vtamx23r
       real vtamy23r
       real vtamz23r
       real vtamxx23r
       real vtamyy23r
       real vtamxy23r
       real vtamzz23r
       real vtamxz23r
       real vtamyz23r
       real vtamx21r
       real vtamy21r
       real vtamz21r
       real vtamxx21r
       real vtamyy21r
       real vtamzz21r
       real vtamxy21r
       real vtamxz21r
       real vtamyz21r
       real vtamlaplacian21r
       real vtamx22r
       real vtamy22r
       real vtamz22r
       real vtamxx22r
       real vtamyy22r
       real vtamzz22r
       real vtamxy22r
       real vtamxz22r
       real vtamyz22r
       real vtamlaplacian22r
       real vtamlaplacian23r
       real vtamxxx22r
       real vtamyyy22r
       real vtamxxy22r
       real vtamxyy22r
       real vtamxxxx22r
       real vtamyyyy22r
       real vtamxxyy22r
       real vtamxxx23r
       real vtamyyy23r
       real vtamzzz23r
       real vtamxxy23r
       real vtamxxz23r
       real vtamxyy23r
       real vtamyyz23r
       real vtamxzz23r
       real vtamyzz23r
       real vtamxxxx23r
       real vtamyyyy23r
       real vtamzzzz23r
       real vtamxxyy23r
       real vtamxxzz23r
       real vtamyyzz23r
       real vtamLapSq22r
       real vtamLapSq23r
       real wtar2
       real wtas2
       real wtat2
       real wtarr2
       real wtass2
       real wtars2
       real wtatt2
       real wtart2
       real wtast2
       real wtarrr2
       real wtasss2
       real wtattt2
       real wtax21
       real wtay21
       real wtaz21
       real wtax22
       real wtay22
       real wtaz22
       real wtax23
       real wtay23
       real wtaz23
       real wtaxx21
       real wtayy21
       real wtaxy21
       real wtaxz21
       real wtayz21
       real wtazz21
       real wtalaplacian21
       real wtaxx22
       real wtayy22
       real wtaxy22
       real wtaxz22
       real wtayz22
       real wtazz22
       real wtalaplacian22
       real wtaxx23
       real wtayy23
       real wtazz23
       real wtaxy23
       real wtaxz23
       real wtayz23
       real wtalaplacian23
       real wtax23r
       real wtay23r
       real wtaz23r
       real wtaxx23r
       real wtayy23r
       real wtaxy23r
       real wtazz23r
       real wtaxz23r
       real wtayz23r
       real wtax21r
       real wtay21r
       real wtaz21r
       real wtaxx21r
       real wtayy21r
       real wtazz21r
       real wtaxy21r
       real wtaxz21r
       real wtayz21r
       real wtalaplacian21r
       real wtax22r
       real wtay22r
       real wtaz22r
       real wtaxx22r
       real wtayy22r
       real wtazz22r
       real wtaxy22r
       real wtaxz22r
       real wtayz22r
       real wtalaplacian22r
       real wtalaplacian23r
       real wtaxxx22r
       real wtayyy22r
       real wtaxxy22r
       real wtaxyy22r
       real wtaxxxx22r
       real wtayyyy22r
       real wtaxxyy22r
       real wtaxxx23r
       real wtayyy23r
       real wtazzz23r
       real wtaxxy23r
       real wtaxxz23r
       real wtaxyy23r
       real wtayyz23r
       real wtaxzz23r
       real wtayzz23r
       real wtaxxxx23r
       real wtayyyy23r
       real wtazzzz23r
       real wtaxxyy23r
       real wtaxxzz23r
       real wtayyzz23r
       real wtaLapSq22r
       real wtaLapSq23r
       real wtamr2
       real wtams2
       real wtamt2
       real wtamrr2
       real wtamss2
       real wtamrs2
       real wtamtt2
       real wtamrt2
       real wtamst2
       real wtamrrr2
       real wtamsss2
       real wtamttt2
       real wtamx21
       real wtamy21
       real wtamz21
       real wtamx22
       real wtamy22
       real wtamz22
       real wtamx23
       real wtamy23
       real wtamz23
       real wtamxx21
       real wtamyy21
       real wtamxy21
       real wtamxz21
       real wtamyz21
       real wtamzz21
       real wtamlaplacian21
       real wtamxx22
       real wtamyy22
       real wtamxy22
       real wtamxz22
       real wtamyz22
       real wtamzz22
       real wtamlaplacian22
       real wtamxx23
       real wtamyy23
       real wtamzz23
       real wtamxy23
       real wtamxz23
       real wtamyz23
       real wtamlaplacian23
       real wtamx23r
       real wtamy23r
       real wtamz23r
       real wtamxx23r
       real wtamyy23r
       real wtamxy23r
       real wtamzz23r
       real wtamxz23r
       real wtamyz23r
       real wtamx21r
       real wtamy21r
       real wtamz21r
       real wtamxx21r
       real wtamyy21r
       real wtamzz21r
       real wtamxy21r
       real wtamxz21r
       real wtamyz21r
       real wtamlaplacian21r
       real wtamx22r
       real wtamy22r
       real wtamz22r
       real wtamxx22r
       real wtamyy22r
       real wtamzz22r
       real wtamxy22r
       real wtamxz22r
       real wtamyz22r
       real wtamlaplacian22r
       real wtamlaplacian23r
       real wtamxxx22r
       real wtamyyy22r
       real wtamxxy22r
       real wtamxyy22r
       real wtamxxxx22r
       real wtamyyyy22r
       real wtamxxyy22r
       real wtamxxx23r
       real wtamyyy23r
       real wtamzzz23r
       real wtamxxy23r
       real wtamxxz23r
       real wtamxyy23r
       real wtamyyz23r
       real wtamxzz23r
       real wtamyzz23r
       real wtamxxxx23r
       real wtamyyyy23r
       real wtamzzzz23r
       real wtamxxyy23r
       real wtamxxzz23r
       real wtamyyzz23r
       real wtamLapSq22r
       real wtamLapSq23r

       real vtbr2
       real vtbs2
       real vtbt2
       real vtbrr2
       real vtbss2
       real vtbrs2
       real vtbtt2
       real vtbrt2
       real vtbst2
       real vtbrrr2
       real vtbsss2
       real vtbttt2
       real vtbx21
       real vtby21
       real vtbz21
       real vtbx22
       real vtby22
       real vtbz22
       real vtbx23
       real vtby23
       real vtbz23
       real vtbxx21
       real vtbyy21
       real vtbxy21
       real vtbxz21
       real vtbyz21
       real vtbzz21
       real vtblaplacian21
       real vtbxx22
       real vtbyy22
       real vtbxy22
       real vtbxz22
       real vtbyz22
       real vtbzz22
       real vtblaplacian22
       real vtbxx23
       real vtbyy23
       real vtbzz23
       real vtbxy23
       real vtbxz23
       real vtbyz23
       real vtblaplacian23
       real vtbx23r
       real vtby23r
       real vtbz23r
       real vtbxx23r
       real vtbyy23r
       real vtbxy23r
       real vtbzz23r
       real vtbxz23r
       real vtbyz23r
       real vtbx21r
       real vtby21r
       real vtbz21r
       real vtbxx21r
       real vtbyy21r
       real vtbzz21r
       real vtbxy21r
       real vtbxz21r
       real vtbyz21r
       real vtblaplacian21r
       real vtbx22r
       real vtby22r
       real vtbz22r
       real vtbxx22r
       real vtbyy22r
       real vtbzz22r
       real vtbxy22r
       real vtbxz22r
       real vtbyz22r
       real vtblaplacian22r
       real vtblaplacian23r
       real vtbxxx22r
       real vtbyyy22r
       real vtbxxy22r
       real vtbxyy22r
       real vtbxxxx22r
       real vtbyyyy22r
       real vtbxxyy22r
       real vtbxxx23r
       real vtbyyy23r
       real vtbzzz23r
       real vtbxxy23r
       real vtbxxz23r
       real vtbxyy23r
       real vtbyyz23r
       real vtbxzz23r
       real vtbyzz23r
       real vtbxxxx23r
       real vtbyyyy23r
       real vtbzzzz23r
       real vtbxxyy23r
       real vtbxxzz23r
       real vtbyyzz23r
       real vtbLapSq22r
       real vtbLapSq23r
       real vtbmr2
       real vtbms2
       real vtbmt2
       real vtbmrr2
       real vtbmss2
       real vtbmrs2
       real vtbmtt2
       real vtbmrt2
       real vtbmst2
       real vtbmrrr2
       real vtbmsss2
       real vtbmttt2
       real vtbmx21
       real vtbmy21
       real vtbmz21
       real vtbmx22
       real vtbmy22
       real vtbmz22
       real vtbmx23
       real vtbmy23
       real vtbmz23
       real vtbmxx21
       real vtbmyy21
       real vtbmxy21
       real vtbmxz21
       real vtbmyz21
       real vtbmzz21
       real vtbmlaplacian21
       real vtbmxx22
       real vtbmyy22
       real vtbmxy22
       real vtbmxz22
       real vtbmyz22
       real vtbmzz22
       real vtbmlaplacian22
       real vtbmxx23
       real vtbmyy23
       real vtbmzz23
       real vtbmxy23
       real vtbmxz23
       real vtbmyz23
       real vtbmlaplacian23
       real vtbmx23r
       real vtbmy23r
       real vtbmz23r
       real vtbmxx23r
       real vtbmyy23r
       real vtbmxy23r
       real vtbmzz23r
       real vtbmxz23r
       real vtbmyz23r
       real vtbmx21r
       real vtbmy21r
       real vtbmz21r
       real vtbmxx21r
       real vtbmyy21r
       real vtbmzz21r
       real vtbmxy21r
       real vtbmxz21r
       real vtbmyz21r
       real vtbmlaplacian21r
       real vtbmx22r
       real vtbmy22r
       real vtbmz22r
       real vtbmxx22r
       real vtbmyy22r
       real vtbmzz22r
       real vtbmxy22r
       real vtbmxz22r
       real vtbmyz22r
       real vtbmlaplacian22r
       real vtbmlaplacian23r
       real vtbmxxx22r
       real vtbmyyy22r
       real vtbmxxy22r
       real vtbmxyy22r
       real vtbmxxxx22r
       real vtbmyyyy22r
       real vtbmxxyy22r
       real vtbmxxx23r
       real vtbmyyy23r
       real vtbmzzz23r
       real vtbmxxy23r
       real vtbmxxz23r
       real vtbmxyy23r
       real vtbmyyz23r
       real vtbmxzz23r
       real vtbmyzz23r
       real vtbmxxxx23r
       real vtbmyyyy23r
       real vtbmzzzz23r
       real vtbmxxyy23r
       real vtbmxxzz23r
       real vtbmyyzz23r
       real vtbmLapSq22r
       real vtbmLapSq23r
       real wtbr2
       real wtbs2
       real wtbt2
       real wtbrr2
       real wtbss2
       real wtbrs2
       real wtbtt2
       real wtbrt2
       real wtbst2
       real wtbrrr2
       real wtbsss2
       real wtbttt2
       real wtbx21
       real wtby21
       real wtbz21
       real wtbx22
       real wtby22
       real wtbz22
       real wtbx23
       real wtby23
       real wtbz23
       real wtbxx21
       real wtbyy21
       real wtbxy21
       real wtbxz21
       real wtbyz21
       real wtbzz21
       real wtblaplacian21
       real wtbxx22
       real wtbyy22
       real wtbxy22
       real wtbxz22
       real wtbyz22
       real wtbzz22
       real wtblaplacian22
       real wtbxx23
       real wtbyy23
       real wtbzz23
       real wtbxy23
       real wtbxz23
       real wtbyz23
       real wtblaplacian23
       real wtbx23r
       real wtby23r
       real wtbz23r
       real wtbxx23r
       real wtbyy23r
       real wtbxy23r
       real wtbzz23r
       real wtbxz23r
       real wtbyz23r
       real wtbx21r
       real wtby21r
       real wtbz21r
       real wtbxx21r
       real wtbyy21r
       real wtbzz21r
       real wtbxy21r
       real wtbxz21r
       real wtbyz21r
       real wtblaplacian21r
       real wtbx22r
       real wtby22r
       real wtbz22r
       real wtbxx22r
       real wtbyy22r
       real wtbzz22r
       real wtbxy22r
       real wtbxz22r
       real wtbyz22r
       real wtblaplacian22r
       real wtblaplacian23r
       real wtbxxx22r
       real wtbyyy22r
       real wtbxxy22r
       real wtbxyy22r
       real wtbxxxx22r
       real wtbyyyy22r
       real wtbxxyy22r
       real wtbxxx23r
       real wtbyyy23r
       real wtbzzz23r
       real wtbxxy23r
       real wtbxxz23r
       real wtbxyy23r
       real wtbyyz23r
       real wtbxzz23r
       real wtbyzz23r
       real wtbxxxx23r
       real wtbyyyy23r
       real wtbzzzz23r
       real wtbxxyy23r
       real wtbxxzz23r
       real wtbyyzz23r
       real wtbLapSq22r
       real wtbLapSq23r
       real wtbmr2
       real wtbms2
       real wtbmt2
       real wtbmrr2
       real wtbmss2
       real wtbmrs2
       real wtbmtt2
       real wtbmrt2
       real wtbmst2
       real wtbmrrr2
       real wtbmsss2
       real wtbmttt2
       real wtbmx21
       real wtbmy21
       real wtbmz21
       real wtbmx22
       real wtbmy22
       real wtbmz22
       real wtbmx23
       real wtbmy23
       real wtbmz23
       real wtbmxx21
       real wtbmyy21
       real wtbmxy21
       real wtbmxz21
       real wtbmyz21
       real wtbmzz21
       real wtbmlaplacian21
       real wtbmxx22
       real wtbmyy22
       real wtbmxy22
       real wtbmxz22
       real wtbmyz22
       real wtbmzz22
       real wtbmlaplacian22
       real wtbmxx23
       real wtbmyy23
       real wtbmzz23
       real wtbmxy23
       real wtbmxz23
       real wtbmyz23
       real wtbmlaplacian23
       real wtbmx23r
       real wtbmy23r
       real wtbmz23r
       real wtbmxx23r
       real wtbmyy23r
       real wtbmxy23r
       real wtbmzz23r
       real wtbmxz23r
       real wtbmyz23r
       real wtbmx21r
       real wtbmy21r
       real wtbmz21r
       real wtbmxx21r
       real wtbmyy21r
       real wtbmzz21r
       real wtbmxy21r
       real wtbmxz21r
       real wtbmyz21r
       real wtbmlaplacian21r
       real wtbmx22r
       real wtbmy22r
       real wtbmz22r
       real wtbmxx22r
       real wtbmyy22r
       real wtbmzz22r
       real wtbmxy22r
       real wtbmxz22r
       real wtbmyz22r
       real wtbmlaplacian22r
       real wtbmlaplacian23r
       real wtbmxxx22r
       real wtbmyyy22r
       real wtbmxxy22r
       real wtbmxyy22r
       real wtbmxxxx22r
       real wtbmyyyy22r
       real wtbmxxyy22r
       real wtbmxxx23r
       real wtbmyyy23r
       real wtbmzzz23r
       real wtbmxxy23r
       real wtbmxxz23r
       real wtbmxyy23r
       real wtbmyyz23r
       real wtbmxzz23r
       real wtbmyzz23r
       real wtbmxxxx23r
       real wtbmyyyy23r
       real wtbmzzzz23r
       real wtbmxxyy23r
       real wtbmxxzz23r
       real wtbmyyzz23r
       real wtbmLapSq22r
       real wtbmLapSq23r

       real d14
       real d24
       real h41
       real h42
       real rxr4
       real rxs4
       real rxt4
       real ryr4
       real rys4
       real ryt4
       real rzr4
       real rzs4
       real rzt4
       real sxr4
       real sxs4
       real sxt4
       real syr4
       real sys4
       real syt4
       real szr4
       real szs4
       real szt4
       real txr4
       real txs4
       real txt4
       real tyr4
       real tys4
       real tyt4
       real tzr4
       real tzs4
       real tzt4
       real rxx41
       real rxx42
       real rxy42
       real rxx43
       real rxy43
       real rxz43
       real ryx42
       real ryy42
       real ryx43
       real ryy43
       real ryz43
       real rzx42
       real rzy42
       real rzx43
       real rzy43
       real rzz43
       real sxx42
       real sxy42
       real sxx43
       real sxy43
       real sxz43
       real syx42
       real syy42
       real syx43
       real syy43
       real syz43
       real szx42
       real szy42
       real szx43
       real szy43
       real szz43
       real txx42
       real txy42
       real txx43
       real txy43
       real txz43
       real tyx42
       real tyy42
       real tyx43
       real tyy43
       real tyz43
       real tzx42
       real tzy42
       real tzx43
       real tzy43
       real tzz43
       real ur4
       real us4
       real ut4
       real urr4
       real uss4
       real utt4
       real urs4
       real urt4
       real ust4
       real ux41
       real uy41
       real uz41
       real ux42
       real uy42
       real uz42
       real ux43
       real uy43
       real uz43
       real uxx41
       real uyy41
       real uxy41
       real uxz41
       real uyz41
       real uzz41
       real ulaplacian41
       real uxx42
       real uyy42
       real uxy42
       real uxz42
       real uyz42
       real uzz42
       real ulaplacian42
       real uxx43
       real uyy43
       real uzz43
       real uxy43
       real uxz43
       real uyz43
       real ulaplacian43
       real ux43r
       real uy43r
       real uz43r
       real uxx43r
       real uyy43r
       real uzz43r
       real uxy43r
       real uxz43r
       real uyz43r
       real ux41r
       real uy41r
       real uz41r
       real uxx41r
       real uyy41r
       real uzz41r
       real uxy41r
       real uxz41r
       real uyz41r
       real ulaplacian41r
       real ux42r
       real uy42r
       real uz42r
       real uxx42r
       real uyy42r
       real uzz42r
       real uxy42r
       real uxz42r
       real uyz42r
       real ulaplacian42r
       real ulaplacian43r
       real umr4
       real ums4
       real umt4
       real umrr4
       real umss4
       real umtt4
       real umrs4
       real umrt4
       real umst4
       real umx41
       real umy41
       real umz41
       real umx42
       real umy42
       real umz42
       real umx43
       real umy43
       real umz43
       real umxx41
       real umyy41
       real umxy41
       real umxz41
       real umyz41
       real umzz41
       real umlaplacian41
       real umxx42
       real umyy42
       real umxy42
       real umxz42
       real umyz42
       real umzz42
       real umlaplacian42
       real umxx43
       real umyy43
       real umzz43
       real umxy43
       real umxz43
       real umyz43
       real umlaplacian43
       real umx43r
       real umy43r
       real umz43r
       real umxx43r
       real umyy43r
       real umzz43r
       real umxy43r
       real umxz43r
       real umyz43r
       real umx41r
       real umy41r
       real umz41r
       real umxx41r
       real umyy41r
       real umzz41r
       real umxy41r
       real umxz41r
       real umyz41r
       real umlaplacian41r
       real umx42r
       real umy42r
       real umz42r
       real umxx42r
       real umyy42r
       real umzz42r
       real umxy42r
       real umxz42r
       real umyz42r
       real umlaplacian42r
       real umlaplacian43r

       real vrar4
       real vras4
       real vrat4
       real vrarr4
       real vrass4
       real vratt4
       real vrars4
       real vrart4
       real vrast4
       real vrax41
       real vray41
       real vraz41
       real vrax42
       real vray42
       real vraz42
       real vrax43
       real vray43
       real vraz43
       real vraxx41
       real vrayy41
       real vraxy41
       real vraxz41
       real vrayz41
       real vrazz41
       real vralaplacian41
       real vraxx42
       real vrayy42
       real vraxy42
       real vraxz42
       real vrayz42
       real vrazz42
       real vralaplacian42
       real vraxx43
       real vrayy43
       real vrazz43
       real vraxy43
       real vraxz43
       real vrayz43
       real vralaplacian43
       real vrax43r
       real vray43r
       real vraz43r
       real vraxx43r
       real vrayy43r
       real vrazz43r
       real vraxy43r
       real vraxz43r
       real vrayz43r
       real vrax41r
       real vray41r
       real vraz41r
       real vraxx41r
       real vrayy41r
       real vrazz41r
       real vraxy41r
       real vraxz41r
       real vrayz41r
       real vralaplacian41r
       real vrax42r
       real vray42r
       real vraz42r
       real vraxx42r
       real vrayy42r
       real vrazz42r
       real vraxy42r
       real vraxz42r
       real vrayz42r
       real vralaplacian42r
       real vralaplacian43r
       real vramr4
       real vrams4
       real vramt4
       real vramrr4
       real vramss4
       real vramtt4
       real vramrs4
       real vramrt4
       real vramst4
       real vramx41
       real vramy41
       real vramz41
       real vramx42
       real vramy42
       real vramz42
       real vramx43
       real vramy43
       real vramz43
       real vramxx41
       real vramyy41
       real vramxy41
       real vramxz41
       real vramyz41
       real vramzz41
       real vramlaplacian41
       real vramxx42
       real vramyy42
       real vramxy42
       real vramxz42
       real vramyz42
       real vramzz42
       real vramlaplacian42
       real vramxx43
       real vramyy43
       real vramzz43
       real vramxy43
       real vramxz43
       real vramyz43
       real vramlaplacian43
       real vramx43r
       real vramy43r
       real vramz43r
       real vramxx43r
       real vramyy43r
       real vramzz43r
       real vramxy43r
       real vramxz43r
       real vramyz43r
       real vramx41r
       real vramy41r
       real vramz41r
       real vramxx41r
       real vramyy41r
       real vramzz41r
       real vramxy41r
       real vramxz41r
       real vramyz41r
       real vramlaplacian41r
       real vramx42r
       real vramy42r
       real vramz42r
       real vramxx42r
       real vramyy42r
       real vramzz42r
       real vramxy42r
       real vramxz42r
       real vramyz42r
       real vramlaplacian42r
       real vramlaplacian43r
       real wrar4
       real wras4
       real wrat4
       real wrarr4
       real wrass4
       real wratt4
       real wrars4
       real wrart4
       real wrast4
       real wrax41
       real wray41
       real wraz41
       real wrax42
       real wray42
       real wraz42
       real wrax43
       real wray43
       real wraz43
       real wraxx41
       real wrayy41
       real wraxy41
       real wraxz41
       real wrayz41
       real wrazz41
       real wralaplacian41
       real wraxx42
       real wrayy42
       real wraxy42
       real wraxz42
       real wrayz42
       real wrazz42
       real wralaplacian42
       real wraxx43
       real wrayy43
       real wrazz43
       real wraxy43
       real wraxz43
       real wrayz43
       real wralaplacian43
       real wrax43r
       real wray43r
       real wraz43r
       real wraxx43r
       real wrayy43r
       real wrazz43r
       real wraxy43r
       real wraxz43r
       real wrayz43r
       real wrax41r
       real wray41r
       real wraz41r
       real wraxx41r
       real wrayy41r
       real wrazz41r
       real wraxy41r
       real wraxz41r
       real wrayz41r
       real wralaplacian41r
       real wrax42r
       real wray42r
       real wraz42r
       real wraxx42r
       real wrayy42r
       real wrazz42r
       real wraxy42r
       real wraxz42r
       real wrayz42r
       real wralaplacian42r
       real wralaplacian43r
       real wramr4
       real wrams4
       real wramt4
       real wramrr4
       real wramss4
       real wramtt4
       real wramrs4
       real wramrt4
       real wramst4
       real wramx41
       real wramy41
       real wramz41
       real wramx42
       real wramy42
       real wramz42
       real wramx43
       real wramy43
       real wramz43
       real wramxx41
       real wramyy41
       real wramxy41
       real wramxz41
       real wramyz41
       real wramzz41
       real wramlaplacian41
       real wramxx42
       real wramyy42
       real wramxy42
       real wramxz42
       real wramyz42
       real wramzz42
       real wramlaplacian42
       real wramxx43
       real wramyy43
       real wramzz43
       real wramxy43
       real wramxz43
       real wramyz43
       real wramlaplacian43
       real wramx43r
       real wramy43r
       real wramz43r
       real wramxx43r
       real wramyy43r
       real wramzz43r
       real wramxy43r
       real wramxz43r
       real wramyz43r
       real wramx41r
       real wramy41r
       real wramz41r
       real wramxx41r
       real wramyy41r
       real wramzz41r
       real wramxy41r
       real wramxz41r
       real wramyz41r
       real wramlaplacian41r
       real wramx42r
       real wramy42r
       real wramz42r
       real wramxx42r
       real wramyy42r
       real wramzz42r
       real wramxy42r
       real wramxz42r
       real wramyz42r
       real wramlaplacian42r
       real wramlaplacian43r

       real vrbr4
       real vrbs4
       real vrbt4
       real vrbrr4
       real vrbss4
       real vrbtt4
       real vrbrs4
       real vrbrt4
       real vrbst4
       real vrbx41
       real vrby41
       real vrbz41
       real vrbx42
       real vrby42
       real vrbz42
       real vrbx43
       real vrby43
       real vrbz43
       real vrbxx41
       real vrbyy41
       real vrbxy41
       real vrbxz41
       real vrbyz41
       real vrbzz41
       real vrblaplacian41
       real vrbxx42
       real vrbyy42
       real vrbxy42
       real vrbxz42
       real vrbyz42
       real vrbzz42
       real vrblaplacian42
       real vrbxx43
       real vrbyy43
       real vrbzz43
       real vrbxy43
       real vrbxz43
       real vrbyz43
       real vrblaplacian43
       real vrbx43r
       real vrby43r
       real vrbz43r
       real vrbxx43r
       real vrbyy43r
       real vrbzz43r
       real vrbxy43r
       real vrbxz43r
       real vrbyz43r
       real vrbx41r
       real vrby41r
       real vrbz41r
       real vrbxx41r
       real vrbyy41r
       real vrbzz41r
       real vrbxy41r
       real vrbxz41r
       real vrbyz41r
       real vrblaplacian41r
       real vrbx42r
       real vrby42r
       real vrbz42r
       real vrbxx42r
       real vrbyy42r
       real vrbzz42r
       real vrbxy42r
       real vrbxz42r
       real vrbyz42r
       real vrblaplacian42r
       real vrblaplacian43r
       real vrbmr4
       real vrbms4
       real vrbmt4
       real vrbmrr4
       real vrbmss4
       real vrbmtt4
       real vrbmrs4
       real vrbmrt4
       real vrbmst4
       real vrbmx41
       real vrbmy41
       real vrbmz41
       real vrbmx42
       real vrbmy42
       real vrbmz42
       real vrbmx43
       real vrbmy43
       real vrbmz43
       real vrbmxx41
       real vrbmyy41
       real vrbmxy41
       real vrbmxz41
       real vrbmyz41
       real vrbmzz41
       real vrbmlaplacian41
       real vrbmxx42
       real vrbmyy42
       real vrbmxy42
       real vrbmxz42
       real vrbmyz42
       real vrbmzz42
       real vrbmlaplacian42
       real vrbmxx43
       real vrbmyy43
       real vrbmzz43
       real vrbmxy43
       real vrbmxz43
       real vrbmyz43
       real vrbmlaplacian43
       real vrbmx43r
       real vrbmy43r
       real vrbmz43r
       real vrbmxx43r
       real vrbmyy43r
       real vrbmzz43r
       real vrbmxy43r
       real vrbmxz43r
       real vrbmyz43r
       real vrbmx41r
       real vrbmy41r
       real vrbmz41r
       real vrbmxx41r
       real vrbmyy41r
       real vrbmzz41r
       real vrbmxy41r
       real vrbmxz41r
       real vrbmyz41r
       real vrbmlaplacian41r
       real vrbmx42r
       real vrbmy42r
       real vrbmz42r
       real vrbmxx42r
       real vrbmyy42r
       real vrbmzz42r
       real vrbmxy42r
       real vrbmxz42r
       real vrbmyz42r
       real vrbmlaplacian42r
       real vrbmlaplacian43r
       real wrbr4
       real wrbs4
       real wrbt4
       real wrbrr4
       real wrbss4
       real wrbtt4
       real wrbrs4
       real wrbrt4
       real wrbst4
       real wrbx41
       real wrby41
       real wrbz41
       real wrbx42
       real wrby42
       real wrbz42
       real wrbx43
       real wrby43
       real wrbz43
       real wrbxx41
       real wrbyy41
       real wrbxy41
       real wrbxz41
       real wrbyz41
       real wrbzz41
       real wrblaplacian41
       real wrbxx42
       real wrbyy42
       real wrbxy42
       real wrbxz42
       real wrbyz42
       real wrbzz42
       real wrblaplacian42
       real wrbxx43
       real wrbyy43
       real wrbzz43
       real wrbxy43
       real wrbxz43
       real wrbyz43
       real wrblaplacian43
       real wrbx43r
       real wrby43r
       real wrbz43r
       real wrbxx43r
       real wrbyy43r
       real wrbzz43r
       real wrbxy43r
       real wrbxz43r
       real wrbyz43r
       real wrbx41r
       real wrby41r
       real wrbz41r
       real wrbxx41r
       real wrbyy41r
       real wrbzz41r
       real wrbxy41r
       real wrbxz41r
       real wrbyz41r
       real wrblaplacian41r
       real wrbx42r
       real wrby42r
       real wrbz42r
       real wrbxx42r
       real wrbyy42r
       real wrbzz42r
       real wrbxy42r
       real wrbxz42r
       real wrbyz42r
       real wrblaplacian42r
       real wrblaplacian43r
       real wrbmr4
       real wrbms4
       real wrbmt4
       real wrbmrr4
       real wrbmss4
       real wrbmtt4
       real wrbmrs4
       real wrbmrt4
       real wrbmst4
       real wrbmx41
       real wrbmy41
       real wrbmz41
       real wrbmx42
       real wrbmy42
       real wrbmz42
       real wrbmx43
       real wrbmy43
       real wrbmz43
       real wrbmxx41
       real wrbmyy41
       real wrbmxy41
       real wrbmxz41
       real wrbmyz41
       real wrbmzz41
       real wrbmlaplacian41
       real wrbmxx42
       real wrbmyy42
       real wrbmxy42
       real wrbmxz42
       real wrbmyz42
       real wrbmzz42
       real wrbmlaplacian42
       real wrbmxx43
       real wrbmyy43
       real wrbmzz43
       real wrbmxy43
       real wrbmxz43
       real wrbmyz43
       real wrbmlaplacian43
       real wrbmx43r
       real wrbmy43r
       real wrbmz43r
       real wrbmxx43r
       real wrbmyy43r
       real wrbmzz43r
       real wrbmxy43r
       real wrbmxz43r
       real wrbmyz43r
       real wrbmx41r
       real wrbmy41r
       real wrbmz41r
       real wrbmxx41r
       real wrbmyy41r
       real wrbmzz41r
       real wrbmxy41r
       real wrbmxz41r
       real wrbmyz41r
       real wrbmlaplacian41r
       real wrbmx42r
       real wrbmy42r
       real wrbmz42r
       real wrbmxx42r
       real wrbmyy42r
       real wrbmzz42r
       real wrbmxy42r
       real wrbmxz42r
       real wrbmyz42r
       real wrbmlaplacian42r
       real wrbmlaplacian43r

       real vsar4
       real vsas4
       real vsat4
       real vsarr4
       real vsass4
       real vsatt4
       real vsars4
       real vsart4
       real vsast4
       real vsax41
       real vsay41
       real vsaz41
       real vsax42
       real vsay42
       real vsaz42
       real vsax43
       real vsay43
       real vsaz43
       real vsaxx41
       real vsayy41
       real vsaxy41
       real vsaxz41
       real vsayz41
       real vsazz41
       real vsalaplacian41
       real vsaxx42
       real vsayy42
       real vsaxy42
       real vsaxz42
       real vsayz42
       real vsazz42
       real vsalaplacian42
       real vsaxx43
       real vsayy43
       real vsazz43
       real vsaxy43
       real vsaxz43
       real vsayz43
       real vsalaplacian43
       real vsax43r
       real vsay43r
       real vsaz43r
       real vsaxx43r
       real vsayy43r
       real vsazz43r
       real vsaxy43r
       real vsaxz43r
       real vsayz43r
       real vsax41r
       real vsay41r
       real vsaz41r
       real vsaxx41r
       real vsayy41r
       real vsazz41r
       real vsaxy41r
       real vsaxz41r
       real vsayz41r
       real vsalaplacian41r
       real vsax42r
       real vsay42r
       real vsaz42r
       real vsaxx42r
       real vsayy42r
       real vsazz42r
       real vsaxy42r
       real vsaxz42r
       real vsayz42r
       real vsalaplacian42r
       real vsalaplacian43r
       real vsamr4
       real vsams4
       real vsamt4
       real vsamrr4
       real vsamss4
       real vsamtt4
       real vsamrs4
       real vsamrt4
       real vsamst4
       real vsamx41
       real vsamy41
       real vsamz41
       real vsamx42
       real vsamy42
       real vsamz42
       real vsamx43
       real vsamy43
       real vsamz43
       real vsamxx41
       real vsamyy41
       real vsamxy41
       real vsamxz41
       real vsamyz41
       real vsamzz41
       real vsamlaplacian41
       real vsamxx42
       real vsamyy42
       real vsamxy42
       real vsamxz42
       real vsamyz42
       real vsamzz42
       real vsamlaplacian42
       real vsamxx43
       real vsamyy43
       real vsamzz43
       real vsamxy43
       real vsamxz43
       real vsamyz43
       real vsamlaplacian43
       real vsamx43r
       real vsamy43r
       real vsamz43r
       real vsamxx43r
       real vsamyy43r
       real vsamzz43r
       real vsamxy43r
       real vsamxz43r
       real vsamyz43r
       real vsamx41r
       real vsamy41r
       real vsamz41r
       real vsamxx41r
       real vsamyy41r
       real vsamzz41r
       real vsamxy41r
       real vsamxz41r
       real vsamyz41r
       real vsamlaplacian41r
       real vsamx42r
       real vsamy42r
       real vsamz42r
       real vsamxx42r
       real vsamyy42r
       real vsamzz42r
       real vsamxy42r
       real vsamxz42r
       real vsamyz42r
       real vsamlaplacian42r
       real vsamlaplacian43r
       real wsar4
       real wsas4
       real wsat4
       real wsarr4
       real wsass4
       real wsatt4
       real wsars4
       real wsart4
       real wsast4
       real wsax41
       real wsay41
       real wsaz41
       real wsax42
       real wsay42
       real wsaz42
       real wsax43
       real wsay43
       real wsaz43
       real wsaxx41
       real wsayy41
       real wsaxy41
       real wsaxz41
       real wsayz41
       real wsazz41
       real wsalaplacian41
       real wsaxx42
       real wsayy42
       real wsaxy42
       real wsaxz42
       real wsayz42
       real wsazz42
       real wsalaplacian42
       real wsaxx43
       real wsayy43
       real wsazz43
       real wsaxy43
       real wsaxz43
       real wsayz43
       real wsalaplacian43
       real wsax43r
       real wsay43r
       real wsaz43r
       real wsaxx43r
       real wsayy43r
       real wsazz43r
       real wsaxy43r
       real wsaxz43r
       real wsayz43r
       real wsax41r
       real wsay41r
       real wsaz41r
       real wsaxx41r
       real wsayy41r
       real wsazz41r
       real wsaxy41r
       real wsaxz41r
       real wsayz41r
       real wsalaplacian41r
       real wsax42r
       real wsay42r
       real wsaz42r
       real wsaxx42r
       real wsayy42r
       real wsazz42r
       real wsaxy42r
       real wsaxz42r
       real wsayz42r
       real wsalaplacian42r
       real wsalaplacian43r
       real wsamr4
       real wsams4
       real wsamt4
       real wsamrr4
       real wsamss4
       real wsamtt4
       real wsamrs4
       real wsamrt4
       real wsamst4
       real wsamx41
       real wsamy41
       real wsamz41
       real wsamx42
       real wsamy42
       real wsamz42
       real wsamx43
       real wsamy43
       real wsamz43
       real wsamxx41
       real wsamyy41
       real wsamxy41
       real wsamxz41
       real wsamyz41
       real wsamzz41
       real wsamlaplacian41
       real wsamxx42
       real wsamyy42
       real wsamxy42
       real wsamxz42
       real wsamyz42
       real wsamzz42
       real wsamlaplacian42
       real wsamxx43
       real wsamyy43
       real wsamzz43
       real wsamxy43
       real wsamxz43
       real wsamyz43
       real wsamlaplacian43
       real wsamx43r
       real wsamy43r
       real wsamz43r
       real wsamxx43r
       real wsamyy43r
       real wsamzz43r
       real wsamxy43r
       real wsamxz43r
       real wsamyz43r
       real wsamx41r
       real wsamy41r
       real wsamz41r
       real wsamxx41r
       real wsamyy41r
       real wsamzz41r
       real wsamxy41r
       real wsamxz41r
       real wsamyz41r
       real wsamlaplacian41r
       real wsamx42r
       real wsamy42r
       real wsamz42r
       real wsamxx42r
       real wsamyy42r
       real wsamzz42r
       real wsamxy42r
       real wsamxz42r
       real wsamyz42r
       real wsamlaplacian42r
       real wsamlaplacian43r

       real vsbr4
       real vsbs4
       real vsbt4
       real vsbrr4
       real vsbss4
       real vsbtt4
       real vsbrs4
       real vsbrt4
       real vsbst4
       real vsbx41
       real vsby41
       real vsbz41
       real vsbx42
       real vsby42
       real vsbz42
       real vsbx43
       real vsby43
       real vsbz43
       real vsbxx41
       real vsbyy41
       real vsbxy41
       real vsbxz41
       real vsbyz41
       real vsbzz41
       real vsblaplacian41
       real vsbxx42
       real vsbyy42
       real vsbxy42
       real vsbxz42
       real vsbyz42
       real vsbzz42
       real vsblaplacian42
       real vsbxx43
       real vsbyy43
       real vsbzz43
       real vsbxy43
       real vsbxz43
       real vsbyz43
       real vsblaplacian43
       real vsbx43r
       real vsby43r
       real vsbz43r
       real vsbxx43r
       real vsbyy43r
       real vsbzz43r
       real vsbxy43r
       real vsbxz43r
       real vsbyz43r
       real vsbx41r
       real vsby41r
       real vsbz41r
       real vsbxx41r
       real vsbyy41r
       real vsbzz41r
       real vsbxy41r
       real vsbxz41r
       real vsbyz41r
       real vsblaplacian41r
       real vsbx42r
       real vsby42r
       real vsbz42r
       real vsbxx42r
       real vsbyy42r
       real vsbzz42r
       real vsbxy42r
       real vsbxz42r
       real vsbyz42r
       real vsblaplacian42r
       real vsblaplacian43r
       real vsbmr4
       real vsbms4
       real vsbmt4
       real vsbmrr4
       real vsbmss4
       real vsbmtt4
       real vsbmrs4
       real vsbmrt4
       real vsbmst4
       real vsbmx41
       real vsbmy41
       real vsbmz41
       real vsbmx42
       real vsbmy42
       real vsbmz42
       real vsbmx43
       real vsbmy43
       real vsbmz43
       real vsbmxx41
       real vsbmyy41
       real vsbmxy41
       real vsbmxz41
       real vsbmyz41
       real vsbmzz41
       real vsbmlaplacian41
       real vsbmxx42
       real vsbmyy42
       real vsbmxy42
       real vsbmxz42
       real vsbmyz42
       real vsbmzz42
       real vsbmlaplacian42
       real vsbmxx43
       real vsbmyy43
       real vsbmzz43
       real vsbmxy43
       real vsbmxz43
       real vsbmyz43
       real vsbmlaplacian43
       real vsbmx43r
       real vsbmy43r
       real vsbmz43r
       real vsbmxx43r
       real vsbmyy43r
       real vsbmzz43r
       real vsbmxy43r
       real vsbmxz43r
       real vsbmyz43r
       real vsbmx41r
       real vsbmy41r
       real vsbmz41r
       real vsbmxx41r
       real vsbmyy41r
       real vsbmzz41r
       real vsbmxy41r
       real vsbmxz41r
       real vsbmyz41r
       real vsbmlaplacian41r
       real vsbmx42r
       real vsbmy42r
       real vsbmz42r
       real vsbmxx42r
       real vsbmyy42r
       real vsbmzz42r
       real vsbmxy42r
       real vsbmxz42r
       real vsbmyz42r
       real vsbmlaplacian42r
       real vsbmlaplacian43r
       real wsbr4
       real wsbs4
       real wsbt4
       real wsbrr4
       real wsbss4
       real wsbtt4
       real wsbrs4
       real wsbrt4
       real wsbst4
       real wsbx41
       real wsby41
       real wsbz41
       real wsbx42
       real wsby42
       real wsbz42
       real wsbx43
       real wsby43
       real wsbz43
       real wsbxx41
       real wsbyy41
       real wsbxy41
       real wsbxz41
       real wsbyz41
       real wsbzz41
       real wsblaplacian41
       real wsbxx42
       real wsbyy42
       real wsbxy42
       real wsbxz42
       real wsbyz42
       real wsbzz42
       real wsblaplacian42
       real wsbxx43
       real wsbyy43
       real wsbzz43
       real wsbxy43
       real wsbxz43
       real wsbyz43
       real wsblaplacian43
       real wsbx43r
       real wsby43r
       real wsbz43r
       real wsbxx43r
       real wsbyy43r
       real wsbzz43r
       real wsbxy43r
       real wsbxz43r
       real wsbyz43r
       real wsbx41r
       real wsby41r
       real wsbz41r
       real wsbxx41r
       real wsbyy41r
       real wsbzz41r
       real wsbxy41r
       real wsbxz41r
       real wsbyz41r
       real wsblaplacian41r
       real wsbx42r
       real wsby42r
       real wsbz42r
       real wsbxx42r
       real wsbyy42r
       real wsbzz42r
       real wsbxy42r
       real wsbxz42r
       real wsbyz42r
       real wsblaplacian42r
       real wsblaplacian43r
       real wsbmr4
       real wsbms4
       real wsbmt4
       real wsbmrr4
       real wsbmss4
       real wsbmtt4
       real wsbmrs4
       real wsbmrt4
       real wsbmst4
       real wsbmx41
       real wsbmy41
       real wsbmz41
       real wsbmx42
       real wsbmy42
       real wsbmz42
       real wsbmx43
       real wsbmy43
       real wsbmz43
       real wsbmxx41
       real wsbmyy41
       real wsbmxy41
       real wsbmxz41
       real wsbmyz41
       real wsbmzz41
       real wsbmlaplacian41
       real wsbmxx42
       real wsbmyy42
       real wsbmxy42
       real wsbmxz42
       real wsbmyz42
       real wsbmzz42
       real wsbmlaplacian42
       real wsbmxx43
       real wsbmyy43
       real wsbmzz43
       real wsbmxy43
       real wsbmxz43
       real wsbmyz43
       real wsbmlaplacian43
       real wsbmx43r
       real wsbmy43r
       real wsbmz43r
       real wsbmxx43r
       real wsbmyy43r
       real wsbmzz43r
       real wsbmxy43r
       real wsbmxz43r
       real wsbmyz43r
       real wsbmx41r
       real wsbmy41r
       real wsbmz41r
       real wsbmxx41r
       real wsbmyy41r
       real wsbmzz41r
       real wsbmxy41r
       real wsbmxz41r
       real wsbmyz41r
       real wsbmlaplacian41r
       real wsbmx42r
       real wsbmy42r
       real wsbmz42r
       real wsbmxx42r
       real wsbmyy42r
       real wsbmzz42r
       real wsbmxy42r
       real wsbmxz42r
       real wsbmyz42r
       real wsbmlaplacian42r
       real wsbmlaplacian43r

       real vtar4
       real vtas4
       real vtat4
       real vtarr4
       real vtass4
       real vtatt4
       real vtars4
       real vtart4
       real vtast4
       real vtax41
       real vtay41
       real vtaz41
       real vtax42
       real vtay42
       real vtaz42
       real vtax43
       real vtay43
       real vtaz43
       real vtaxx41
       real vtayy41
       real vtaxy41
       real vtaxz41
       real vtayz41
       real vtazz41
       real vtalaplacian41
       real vtaxx42
       real vtayy42
       real vtaxy42
       real vtaxz42
       real vtayz42
       real vtazz42
       real vtalaplacian42
       real vtaxx43
       real vtayy43
       real vtazz43
       real vtaxy43
       real vtaxz43
       real vtayz43
       real vtalaplacian43
       real vtax43r
       real vtay43r
       real vtaz43r
       real vtaxx43r
       real vtayy43r
       real vtazz43r
       real vtaxy43r
       real vtaxz43r
       real vtayz43r
       real vtax41r
       real vtay41r
       real vtaz41r
       real vtaxx41r
       real vtayy41r
       real vtazz41r
       real vtaxy41r
       real vtaxz41r
       real vtayz41r
       real vtalaplacian41r
       real vtax42r
       real vtay42r
       real vtaz42r
       real vtaxx42r
       real vtayy42r
       real vtazz42r
       real vtaxy42r
       real vtaxz42r
       real vtayz42r
       real vtalaplacian42r
       real vtalaplacian43r
       real vtamr4
       real vtams4
       real vtamt4
       real vtamrr4
       real vtamss4
       real vtamtt4
       real vtamrs4
       real vtamrt4
       real vtamst4
       real vtamx41
       real vtamy41
       real vtamz41
       real vtamx42
       real vtamy42
       real vtamz42
       real vtamx43
       real vtamy43
       real vtamz43
       real vtamxx41
       real vtamyy41
       real vtamxy41
       real vtamxz41
       real vtamyz41
       real vtamzz41
       real vtamlaplacian41
       real vtamxx42
       real vtamyy42
       real vtamxy42
       real vtamxz42
       real vtamyz42
       real vtamzz42
       real vtamlaplacian42
       real vtamxx43
       real vtamyy43
       real vtamzz43
       real vtamxy43
       real vtamxz43
       real vtamyz43
       real vtamlaplacian43
       real vtamx43r
       real vtamy43r
       real vtamz43r
       real vtamxx43r
       real vtamyy43r
       real vtamzz43r
       real vtamxy43r
       real vtamxz43r
       real vtamyz43r
       real vtamx41r
       real vtamy41r
       real vtamz41r
       real vtamxx41r
       real vtamyy41r
       real vtamzz41r
       real vtamxy41r
       real vtamxz41r
       real vtamyz41r
       real vtamlaplacian41r
       real vtamx42r
       real vtamy42r
       real vtamz42r
       real vtamxx42r
       real vtamyy42r
       real vtamzz42r
       real vtamxy42r
       real vtamxz42r
       real vtamyz42r
       real vtamlaplacian42r
       real vtamlaplacian43r
       real wtar4
       real wtas4
       real wtat4
       real wtarr4
       real wtass4
       real wtatt4
       real wtars4
       real wtart4
       real wtast4
       real wtax41
       real wtay41
       real wtaz41
       real wtax42
       real wtay42
       real wtaz42
       real wtax43
       real wtay43
       real wtaz43
       real wtaxx41
       real wtayy41
       real wtaxy41
       real wtaxz41
       real wtayz41
       real wtazz41
       real wtalaplacian41
       real wtaxx42
       real wtayy42
       real wtaxy42
       real wtaxz42
       real wtayz42
       real wtazz42
       real wtalaplacian42
       real wtaxx43
       real wtayy43
       real wtazz43
       real wtaxy43
       real wtaxz43
       real wtayz43
       real wtalaplacian43
       real wtax43r
       real wtay43r
       real wtaz43r
       real wtaxx43r
       real wtayy43r
       real wtazz43r
       real wtaxy43r
       real wtaxz43r
       real wtayz43r
       real wtax41r
       real wtay41r
       real wtaz41r
       real wtaxx41r
       real wtayy41r
       real wtazz41r
       real wtaxy41r
       real wtaxz41r
       real wtayz41r
       real wtalaplacian41r
       real wtax42r
       real wtay42r
       real wtaz42r
       real wtaxx42r
       real wtayy42r
       real wtazz42r
       real wtaxy42r
       real wtaxz42r
       real wtayz42r
       real wtalaplacian42r
       real wtalaplacian43r
       real wtamr4
       real wtams4
       real wtamt4
       real wtamrr4
       real wtamss4
       real wtamtt4
       real wtamrs4
       real wtamrt4
       real wtamst4
       real wtamx41
       real wtamy41
       real wtamz41
       real wtamx42
       real wtamy42
       real wtamz42
       real wtamx43
       real wtamy43
       real wtamz43
       real wtamxx41
       real wtamyy41
       real wtamxy41
       real wtamxz41
       real wtamyz41
       real wtamzz41
       real wtamlaplacian41
       real wtamxx42
       real wtamyy42
       real wtamxy42
       real wtamxz42
       real wtamyz42
       real wtamzz42
       real wtamlaplacian42
       real wtamxx43
       real wtamyy43
       real wtamzz43
       real wtamxy43
       real wtamxz43
       real wtamyz43
       real wtamlaplacian43
       real wtamx43r
       real wtamy43r
       real wtamz43r
       real wtamxx43r
       real wtamyy43r
       real wtamzz43r
       real wtamxy43r
       real wtamxz43r
       real wtamyz43r
       real wtamx41r
       real wtamy41r
       real wtamz41r
       real wtamxx41r
       real wtamyy41r
       real wtamzz41r
       real wtamxy41r
       real wtamxz41r
       real wtamyz41r
       real wtamlaplacian41r
       real wtamx42r
       real wtamy42r
       real wtamz42r
       real wtamxx42r
       real wtamyy42r
       real wtamzz42r
       real wtamxy42r
       real wtamxz42r
       real wtamyz42r
       real wtamlaplacian42r
       real wtamlaplacian43r

       real vtbr4
       real vtbs4
       real vtbt4
       real vtbrr4
       real vtbss4
       real vtbtt4
       real vtbrs4
       real vtbrt4
       real vtbst4
       real vtbx41
       real vtby41
       real vtbz41
       real vtbx42
       real vtby42
       real vtbz42
       real vtbx43
       real vtby43
       real vtbz43
       real vtbxx41
       real vtbyy41
       real vtbxy41
       real vtbxz41
       real vtbyz41
       real vtbzz41
       real vtblaplacian41
       real vtbxx42
       real vtbyy42
       real vtbxy42
       real vtbxz42
       real vtbyz42
       real vtbzz42
       real vtblaplacian42
       real vtbxx43
       real vtbyy43
       real vtbzz43
       real vtbxy43
       real vtbxz43
       real vtbyz43
       real vtblaplacian43
       real vtbx43r
       real vtby43r
       real vtbz43r
       real vtbxx43r
       real vtbyy43r
       real vtbzz43r
       real vtbxy43r
       real vtbxz43r
       real vtbyz43r
       real vtbx41r
       real vtby41r
       real vtbz41r
       real vtbxx41r
       real vtbyy41r
       real vtbzz41r
       real vtbxy41r
       real vtbxz41r
       real vtbyz41r
       real vtblaplacian41r
       real vtbx42r
       real vtby42r
       real vtbz42r
       real vtbxx42r
       real vtbyy42r
       real vtbzz42r
       real vtbxy42r
       real vtbxz42r
       real vtbyz42r
       real vtblaplacian42r
       real vtblaplacian43r
       real vtbmr4
       real vtbms4
       real vtbmt4
       real vtbmrr4
       real vtbmss4
       real vtbmtt4
       real vtbmrs4
       real vtbmrt4
       real vtbmst4
       real vtbmx41
       real vtbmy41
       real vtbmz41
       real vtbmx42
       real vtbmy42
       real vtbmz42
       real vtbmx43
       real vtbmy43
       real vtbmz43
       real vtbmxx41
       real vtbmyy41
       real vtbmxy41
       real vtbmxz41
       real vtbmyz41
       real vtbmzz41
       real vtbmlaplacian41
       real vtbmxx42
       real vtbmyy42
       real vtbmxy42
       real vtbmxz42
       real vtbmyz42
       real vtbmzz42
       real vtbmlaplacian42
       real vtbmxx43
       real vtbmyy43
       real vtbmzz43
       real vtbmxy43
       real vtbmxz43
       real vtbmyz43
       real vtbmlaplacian43
       real vtbmx43r
       real vtbmy43r
       real vtbmz43r
       real vtbmxx43r
       real vtbmyy43r
       real vtbmzz43r
       real vtbmxy43r
       real vtbmxz43r
       real vtbmyz43r
       real vtbmx41r
       real vtbmy41r
       real vtbmz41r
       real vtbmxx41r
       real vtbmyy41r
       real vtbmzz41r
       real vtbmxy41r
       real vtbmxz41r
       real vtbmyz41r
       real vtbmlaplacian41r
       real vtbmx42r
       real vtbmy42r
       real vtbmz42r
       real vtbmxx42r
       real vtbmyy42r
       real vtbmzz42r
       real vtbmxy42r
       real vtbmxz42r
       real vtbmyz42r
       real vtbmlaplacian42r
       real vtbmlaplacian43r
       real wtbr4
       real wtbs4
       real wtbt4
       real wtbrr4
       real wtbss4
       real wtbtt4
       real wtbrs4
       real wtbrt4
       real wtbst4
       real wtbx41
       real wtby41
       real wtbz41
       real wtbx42
       real wtby42
       real wtbz42
       real wtbx43
       real wtby43
       real wtbz43
       real wtbxx41
       real wtbyy41
       real wtbxy41
       real wtbxz41
       real wtbyz41
       real wtbzz41
       real wtblaplacian41
       real wtbxx42
       real wtbyy42
       real wtbxy42
       real wtbxz42
       real wtbyz42
       real wtbzz42
       real wtblaplacian42
       real wtbxx43
       real wtbyy43
       real wtbzz43
       real wtbxy43
       real wtbxz43
       real wtbyz43
       real wtblaplacian43
       real wtbx43r
       real wtby43r
       real wtbz43r
       real wtbxx43r
       real wtbyy43r
       real wtbzz43r
       real wtbxy43r
       real wtbxz43r
       real wtbyz43r
       real wtbx41r
       real wtby41r
       real wtbz41r
       real wtbxx41r
       real wtbyy41r
       real wtbzz41r
       real wtbxy41r
       real wtbxz41r
       real wtbyz41r
       real wtblaplacian41r
       real wtbx42r
       real wtby42r
       real wtbz42r
       real wtbxx42r
       real wtbyy42r
       real wtbzz42r
       real wtbxy42r
       real wtbxz42r
       real wtbyz42r
       real wtblaplacian42r
       real wtblaplacian43r
       real wtbmr4
       real wtbms4
       real wtbmt4
       real wtbmrr4
       real wtbmss4
       real wtbmtt4
       real wtbmrs4
       real wtbmrt4
       real wtbmst4
       real wtbmx41
       real wtbmy41
       real wtbmz41
       real wtbmx42
       real wtbmy42
       real wtbmz42
       real wtbmx43
       real wtbmy43
       real wtbmz43
       real wtbmxx41
       real wtbmyy41
       real wtbmxy41
       real wtbmxz41
       real wtbmyz41
       real wtbmzz41
       real wtbmlaplacian41
       real wtbmxx42
       real wtbmyy42
       real wtbmxy42
       real wtbmxz42
       real wtbmyz42
       real wtbmzz42
       real wtbmlaplacian42
       real wtbmxx43
       real wtbmyy43
       real wtbmzz43
       real wtbmxy43
       real wtbmxz43
       real wtbmyz43
       real wtbmlaplacian43
       real wtbmx43r
       real wtbmy43r
       real wtbmz43r
       real wtbmxx43r
       real wtbmyy43r
       real wtbmzz43r
       real wtbmxy43r
       real wtbmxz43r
       real wtbmyz43r
       real wtbmx41r
       real wtbmy41r
       real wtbmz41r
       real wtbmxx41r
       real wtbmyy41r
       real wtbmzz41r
       real wtbmxy41r
       real wtbmxz41r
       real wtbmyz41r
       real wtbmlaplacian41r
       real wtbmx42r
       real wtbmy42r
       real wtbmz42r
       real wtbmxx42r
       real wtbmyy42r
       real wtbmzz42r
       real wtbmxy42r
       real wtbmxz42r
       real wtbmyz42r
       real wtbmlaplacian42r
       real wtbmlaplacian43r

!.......statement functions for jacobian
      rx(i1,i2,i3)=rsxy(i1,i2,i3,0,0)
      ry(i1,i2,i3)=rsxy(i1,i2,i3,0,1)
      rz(i1,i2,i3)=rsxy(i1,i2,i3,0,2)
      sx(i1,i2,i3)=rsxy(i1,i2,i3,1,0)
      sy(i1,i2,i3)=rsxy(i1,i2,i3,1,1)
      sz(i1,i2,i3)=rsxy(i1,i2,i3,1,2)
      tx(i1,i2,i3)=rsxy(i1,i2,i3,2,0)
      ty(i1,i2,i3)=rsxy(i1,i2,i3,2,1)
      tz(i1,i2,i3)=rsxy(i1,i2,i3,2,2)


!     The next macro call will define the difference approximation statement functions
      d12(kd) = 1./(2.*dr(kd))
      d22(kd) = 1./(dr(kd)**2)
      ur2(i1,i2,i3,kd)=(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))*d12(0)
      us2(i1,i2,i3,kd)=(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))*d12(1)
      ut2(i1,i2,i3,kd)=(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))*d12(2)
      urr2(i1,i2,i3,kd)=(-2.*u(i1,i2,i3,kd)+(u(i1+1,i2,i3,kd)+u(i1-1,
     & i2,i3,kd)) )*d22(0)
      uss2(i1,i2,i3,kd)=(-2.*u(i1,i2,i3,kd)+(u(i1,i2+1,i3,kd)+u(i1,i2-
     & 1,i3,kd)) )*d22(1)
      urs2(i1,i2,i3,kd)=(ur2(i1,i2+1,i3,kd)-ur2(i1,i2-1,i3,kd))*d12(1)
      utt2(i1,i2,i3,kd)=(-2.*u(i1,i2,i3,kd)+(u(i1,i2,i3+1,kd)+u(i1,i2,
     & i3-1,kd)) )*d22(2)
      urt2(i1,i2,i3,kd)=(ur2(i1,i2,i3+1,kd)-ur2(i1,i2,i3-1,kd))*d12(2)
      ust2(i1,i2,i3,kd)=(us2(i1,i2,i3+1,kd)-us2(i1,i2,i3-1,kd))*d12(2)
      urrr2(i1,i2,i3,kd)=(-2.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))+(u(
     & i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd)) )*d22(0)*d12(0)
      usss2(i1,i2,i3,kd)=(-2.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))+(u(
     & i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd)) )*d22(1)*d12(1)
      uttt2(i1,i2,i3,kd)=(-2.*(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))+(u(
     & i1,i2,i3+2,kd)-u(i1,i2,i3-2,kd)) )*d22(2)*d12(2)
      rxr2(i1,i2,i3)=(rx(i1+1,i2,i3)-rx(i1-1,i2,i3))*d12(0)
      rxs2(i1,i2,i3)=(rx(i1,i2+1,i3)-rx(i1,i2-1,i3))*d12(1)
      rxt2(i1,i2,i3)=(rx(i1,i2,i3+1)-rx(i1,i2,i3-1))*d12(2)
      rxrr2(i1,i2,i3)=(-2.*rx(i1,i2,i3)+(rx(i1+1,i2,i3)+rx(i1-1,i2,i3))
     &  )*d22(0)
      rxss2(i1,i2,i3)=(-2.*rx(i1,i2,i3)+(rx(i1,i2+1,i3)+rx(i1,i2-1,i3))
     &  )*d22(1)
      rxrs2(i1,i2,i3)=(rxr2(i1,i2+1,i3)-rxr2(i1,i2-1,i3))*d12(1)
      ryr2(i1,i2,i3)=(ry(i1+1,i2,i3)-ry(i1-1,i2,i3))*d12(0)
      rys2(i1,i2,i3)=(ry(i1,i2+1,i3)-ry(i1,i2-1,i3))*d12(1)
      ryt2(i1,i2,i3)=(ry(i1,i2,i3+1)-ry(i1,i2,i3-1))*d12(2)
      ryrr2(i1,i2,i3)=(-2.*ry(i1,i2,i3)+(ry(i1+1,i2,i3)+ry(i1-1,i2,i3))
     &  )*d22(0)
      ryss2(i1,i2,i3)=(-2.*ry(i1,i2,i3)+(ry(i1,i2+1,i3)+ry(i1,i2-1,i3))
     &  )*d22(1)
      ryrs2(i1,i2,i3)=(ryr2(i1,i2+1,i3)-ryr2(i1,i2-1,i3))*d12(1)
      rzr2(i1,i2,i3)=(rz(i1+1,i2,i3)-rz(i1-1,i2,i3))*d12(0)
      rzs2(i1,i2,i3)=(rz(i1,i2+1,i3)-rz(i1,i2-1,i3))*d12(1)
      rzt2(i1,i2,i3)=(rz(i1,i2,i3+1)-rz(i1,i2,i3-1))*d12(2)
      rzrr2(i1,i2,i3)=(-2.*rz(i1,i2,i3)+(rz(i1+1,i2,i3)+rz(i1-1,i2,i3))
     &  )*d22(0)
      rzss2(i1,i2,i3)=(-2.*rz(i1,i2,i3)+(rz(i1,i2+1,i3)+rz(i1,i2-1,i3))
     &  )*d22(1)
      rzrs2(i1,i2,i3)=(rzr2(i1,i2+1,i3)-rzr2(i1,i2-1,i3))*d12(1)
      sxr2(i1,i2,i3)=(sx(i1+1,i2,i3)-sx(i1-1,i2,i3))*d12(0)
      sxs2(i1,i2,i3)=(sx(i1,i2+1,i3)-sx(i1,i2-1,i3))*d12(1)
      sxt2(i1,i2,i3)=(sx(i1,i2,i3+1)-sx(i1,i2,i3-1))*d12(2)
      sxrr2(i1,i2,i3)=(-2.*sx(i1,i2,i3)+(sx(i1+1,i2,i3)+sx(i1-1,i2,i3))
     &  )*d22(0)
      sxss2(i1,i2,i3)=(-2.*sx(i1,i2,i3)+(sx(i1,i2+1,i3)+sx(i1,i2-1,i3))
     &  )*d22(1)
      sxrs2(i1,i2,i3)=(sxr2(i1,i2+1,i3)-sxr2(i1,i2-1,i3))*d12(1)
      syr2(i1,i2,i3)=(sy(i1+1,i2,i3)-sy(i1-1,i2,i3))*d12(0)
      sys2(i1,i2,i3)=(sy(i1,i2+1,i3)-sy(i1,i2-1,i3))*d12(1)
      syt2(i1,i2,i3)=(sy(i1,i2,i3+1)-sy(i1,i2,i3-1))*d12(2)
      syrr2(i1,i2,i3)=(-2.*sy(i1,i2,i3)+(sy(i1+1,i2,i3)+sy(i1-1,i2,i3))
     &  )*d22(0)
      syss2(i1,i2,i3)=(-2.*sy(i1,i2,i3)+(sy(i1,i2+1,i3)+sy(i1,i2-1,i3))
     &  )*d22(1)
      syrs2(i1,i2,i3)=(syr2(i1,i2+1,i3)-syr2(i1,i2-1,i3))*d12(1)
      szr2(i1,i2,i3)=(sz(i1+1,i2,i3)-sz(i1-1,i2,i3))*d12(0)
      szs2(i1,i2,i3)=(sz(i1,i2+1,i3)-sz(i1,i2-1,i3))*d12(1)
      szt2(i1,i2,i3)=(sz(i1,i2,i3+1)-sz(i1,i2,i3-1))*d12(2)
      szrr2(i1,i2,i3)=(-2.*sz(i1,i2,i3)+(sz(i1+1,i2,i3)+sz(i1-1,i2,i3))
     &  )*d22(0)
      szss2(i1,i2,i3)=(-2.*sz(i1,i2,i3)+(sz(i1,i2+1,i3)+sz(i1,i2-1,i3))
     &  )*d22(1)
      szrs2(i1,i2,i3)=(szr2(i1,i2+1,i3)-szr2(i1,i2-1,i3))*d12(1)
      txr2(i1,i2,i3)=(tx(i1+1,i2,i3)-tx(i1-1,i2,i3))*d12(0)
      txs2(i1,i2,i3)=(tx(i1,i2+1,i3)-tx(i1,i2-1,i3))*d12(1)
      txt2(i1,i2,i3)=(tx(i1,i2,i3+1)-tx(i1,i2,i3-1))*d12(2)
      txrr2(i1,i2,i3)=(-2.*tx(i1,i2,i3)+(tx(i1+1,i2,i3)+tx(i1-1,i2,i3))
     &  )*d22(0)
      txss2(i1,i2,i3)=(-2.*tx(i1,i2,i3)+(tx(i1,i2+1,i3)+tx(i1,i2-1,i3))
     &  )*d22(1)
      txrs2(i1,i2,i3)=(txr2(i1,i2+1,i3)-txr2(i1,i2-1,i3))*d12(1)
      tyr2(i1,i2,i3)=(ty(i1+1,i2,i3)-ty(i1-1,i2,i3))*d12(0)
      tys2(i1,i2,i3)=(ty(i1,i2+1,i3)-ty(i1,i2-1,i3))*d12(1)
      tyt2(i1,i2,i3)=(ty(i1,i2,i3+1)-ty(i1,i2,i3-1))*d12(2)
      tyrr2(i1,i2,i3)=(-2.*ty(i1,i2,i3)+(ty(i1+1,i2,i3)+ty(i1-1,i2,i3))
     &  )*d22(0)
      tyss2(i1,i2,i3)=(-2.*ty(i1,i2,i3)+(ty(i1,i2+1,i3)+ty(i1,i2-1,i3))
     &  )*d22(1)
      tyrs2(i1,i2,i3)=(tyr2(i1,i2+1,i3)-tyr2(i1,i2-1,i3))*d12(1)
      tzr2(i1,i2,i3)=(tz(i1+1,i2,i3)-tz(i1-1,i2,i3))*d12(0)
      tzs2(i1,i2,i3)=(tz(i1,i2+1,i3)-tz(i1,i2-1,i3))*d12(1)
      tzt2(i1,i2,i3)=(tz(i1,i2,i3+1)-tz(i1,i2,i3-1))*d12(2)
      tzrr2(i1,i2,i3)=(-2.*tz(i1,i2,i3)+(tz(i1+1,i2,i3)+tz(i1-1,i2,i3))
     &  )*d22(0)
      tzss2(i1,i2,i3)=(-2.*tz(i1,i2,i3)+(tz(i1,i2+1,i3)+tz(i1,i2-1,i3))
     &  )*d22(1)
      tzrs2(i1,i2,i3)=(tzr2(i1,i2+1,i3)-tzr2(i1,i2-1,i3))*d12(1)
      ux21(i1,i2,i3,kd)= rx(i1,i2,i3)*ur2(i1,i2,i3,kd)
      uy21(i1,i2,i3,kd)=0
      uz21(i1,i2,i3,kd)=0
      ux22(i1,i2,i3,kd)= rx(i1,i2,i3)*ur2(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & us2(i1,i2,i3,kd)
      uy22(i1,i2,i3,kd)= ry(i1,i2,i3)*ur2(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & us2(i1,i2,i3,kd)
      uz22(i1,i2,i3,kd)=0
      ux23(i1,i2,i3,kd)=rx(i1,i2,i3)*ur2(i1,i2,i3,kd)+sx(i1,i2,i3)*us2(
     & i1,i2,i3,kd)+tx(i1,i2,i3)*ut2(i1,i2,i3,kd)
      uy23(i1,i2,i3,kd)=ry(i1,i2,i3)*ur2(i1,i2,i3,kd)+sy(i1,i2,i3)*us2(
     & i1,i2,i3,kd)+ty(i1,i2,i3)*ut2(i1,i2,i3,kd)
      uz23(i1,i2,i3,kd)=rz(i1,i2,i3)*ur2(i1,i2,i3,kd)+sz(i1,i2,i3)*us2(
     & i1,i2,i3,kd)+tz(i1,i2,i3)*ut2(i1,i2,i3,kd)
      rxx21(i1,i2,i3)= rx(i1,i2,i3)*rxr2(i1,i2,i3)
      rxx22(i1,i2,i3)= rx(i1,i2,i3)*rxr2(i1,i2,i3)+sx(i1,i2,i3)*rxs2(
     & i1,i2,i3)
      rxy22(i1,i2,i3)= ry(i1,i2,i3)*rxr2(i1,i2,i3)+sy(i1,i2,i3)*rxs2(
     & i1,i2,i3)
      rxx23(i1,i2,i3)=rx(i1,i2,i3)*rxr2(i1,i2,i3)+sx(i1,i2,i3)*rxs2(i1,
     & i2,i3)+tx(i1,i2,i3)*rxt2(i1,i2,i3)
      rxy23(i1,i2,i3)=ry(i1,i2,i3)*rxr2(i1,i2,i3)+sy(i1,i2,i3)*rxs2(i1,
     & i2,i3)+ty(i1,i2,i3)*rxt2(i1,i2,i3)
      rxz23(i1,i2,i3)=rz(i1,i2,i3)*rxr2(i1,i2,i3)+sz(i1,i2,i3)*rxs2(i1,
     & i2,i3)+tz(i1,i2,i3)*rxt2(i1,i2,i3)
      ryx22(i1,i2,i3)= rx(i1,i2,i3)*ryr2(i1,i2,i3)+sx(i1,i2,i3)*rys2(
     & i1,i2,i3)
      ryy22(i1,i2,i3)= ry(i1,i2,i3)*ryr2(i1,i2,i3)+sy(i1,i2,i3)*rys2(
     & i1,i2,i3)
      ryx23(i1,i2,i3)=rx(i1,i2,i3)*ryr2(i1,i2,i3)+sx(i1,i2,i3)*rys2(i1,
     & i2,i3)+tx(i1,i2,i3)*ryt2(i1,i2,i3)
      ryy23(i1,i2,i3)=ry(i1,i2,i3)*ryr2(i1,i2,i3)+sy(i1,i2,i3)*rys2(i1,
     & i2,i3)+ty(i1,i2,i3)*ryt2(i1,i2,i3)
      ryz23(i1,i2,i3)=rz(i1,i2,i3)*ryr2(i1,i2,i3)+sz(i1,i2,i3)*rys2(i1,
     & i2,i3)+tz(i1,i2,i3)*ryt2(i1,i2,i3)
      rzx22(i1,i2,i3)= rx(i1,i2,i3)*rzr2(i1,i2,i3)+sx(i1,i2,i3)*rzs2(
     & i1,i2,i3)
      rzy22(i1,i2,i3)= ry(i1,i2,i3)*rzr2(i1,i2,i3)+sy(i1,i2,i3)*rzs2(
     & i1,i2,i3)
      rzx23(i1,i2,i3)=rx(i1,i2,i3)*rzr2(i1,i2,i3)+sx(i1,i2,i3)*rzs2(i1,
     & i2,i3)+tx(i1,i2,i3)*rzt2(i1,i2,i3)
      rzy23(i1,i2,i3)=ry(i1,i2,i3)*rzr2(i1,i2,i3)+sy(i1,i2,i3)*rzs2(i1,
     & i2,i3)+ty(i1,i2,i3)*rzt2(i1,i2,i3)
      rzz23(i1,i2,i3)=rz(i1,i2,i3)*rzr2(i1,i2,i3)+sz(i1,i2,i3)*rzs2(i1,
     & i2,i3)+tz(i1,i2,i3)*rzt2(i1,i2,i3)
      sxx22(i1,i2,i3)= rx(i1,i2,i3)*sxr2(i1,i2,i3)+sx(i1,i2,i3)*sxs2(
     & i1,i2,i3)
      sxy22(i1,i2,i3)= ry(i1,i2,i3)*sxr2(i1,i2,i3)+sy(i1,i2,i3)*sxs2(
     & i1,i2,i3)
      sxx23(i1,i2,i3)=rx(i1,i2,i3)*sxr2(i1,i2,i3)+sx(i1,i2,i3)*sxs2(i1,
     & i2,i3)+tx(i1,i2,i3)*sxt2(i1,i2,i3)
      sxy23(i1,i2,i3)=ry(i1,i2,i3)*sxr2(i1,i2,i3)+sy(i1,i2,i3)*sxs2(i1,
     & i2,i3)+ty(i1,i2,i3)*sxt2(i1,i2,i3)
      sxz23(i1,i2,i3)=rz(i1,i2,i3)*sxr2(i1,i2,i3)+sz(i1,i2,i3)*sxs2(i1,
     & i2,i3)+tz(i1,i2,i3)*sxt2(i1,i2,i3)
      syx22(i1,i2,i3)= rx(i1,i2,i3)*syr2(i1,i2,i3)+sx(i1,i2,i3)*sys2(
     & i1,i2,i3)
      syy22(i1,i2,i3)= ry(i1,i2,i3)*syr2(i1,i2,i3)+sy(i1,i2,i3)*sys2(
     & i1,i2,i3)
      syx23(i1,i2,i3)=rx(i1,i2,i3)*syr2(i1,i2,i3)+sx(i1,i2,i3)*sys2(i1,
     & i2,i3)+tx(i1,i2,i3)*syt2(i1,i2,i3)
      syy23(i1,i2,i3)=ry(i1,i2,i3)*syr2(i1,i2,i3)+sy(i1,i2,i3)*sys2(i1,
     & i2,i3)+ty(i1,i2,i3)*syt2(i1,i2,i3)
      syz23(i1,i2,i3)=rz(i1,i2,i3)*syr2(i1,i2,i3)+sz(i1,i2,i3)*sys2(i1,
     & i2,i3)+tz(i1,i2,i3)*syt2(i1,i2,i3)
      szx22(i1,i2,i3)= rx(i1,i2,i3)*szr2(i1,i2,i3)+sx(i1,i2,i3)*szs2(
     & i1,i2,i3)
      szy22(i1,i2,i3)= ry(i1,i2,i3)*szr2(i1,i2,i3)+sy(i1,i2,i3)*szs2(
     & i1,i2,i3)
      szx23(i1,i2,i3)=rx(i1,i2,i3)*szr2(i1,i2,i3)+sx(i1,i2,i3)*szs2(i1,
     & i2,i3)+tx(i1,i2,i3)*szt2(i1,i2,i3)
      szy23(i1,i2,i3)=ry(i1,i2,i3)*szr2(i1,i2,i3)+sy(i1,i2,i3)*szs2(i1,
     & i2,i3)+ty(i1,i2,i3)*szt2(i1,i2,i3)
      szz23(i1,i2,i3)=rz(i1,i2,i3)*szr2(i1,i2,i3)+sz(i1,i2,i3)*szs2(i1,
     & i2,i3)+tz(i1,i2,i3)*szt2(i1,i2,i3)
      txx22(i1,i2,i3)= rx(i1,i2,i3)*txr2(i1,i2,i3)+sx(i1,i2,i3)*txs2(
     & i1,i2,i3)
      txy22(i1,i2,i3)= ry(i1,i2,i3)*txr2(i1,i2,i3)+sy(i1,i2,i3)*txs2(
     & i1,i2,i3)
      txx23(i1,i2,i3)=rx(i1,i2,i3)*txr2(i1,i2,i3)+sx(i1,i2,i3)*txs2(i1,
     & i2,i3)+tx(i1,i2,i3)*txt2(i1,i2,i3)
      txy23(i1,i2,i3)=ry(i1,i2,i3)*txr2(i1,i2,i3)+sy(i1,i2,i3)*txs2(i1,
     & i2,i3)+ty(i1,i2,i3)*txt2(i1,i2,i3)
      txz23(i1,i2,i3)=rz(i1,i2,i3)*txr2(i1,i2,i3)+sz(i1,i2,i3)*txs2(i1,
     & i2,i3)+tz(i1,i2,i3)*txt2(i1,i2,i3)
      tyx22(i1,i2,i3)= rx(i1,i2,i3)*tyr2(i1,i2,i3)+sx(i1,i2,i3)*tys2(
     & i1,i2,i3)
      tyy22(i1,i2,i3)= ry(i1,i2,i3)*tyr2(i1,i2,i3)+sy(i1,i2,i3)*tys2(
     & i1,i2,i3)
      tyx23(i1,i2,i3)=rx(i1,i2,i3)*tyr2(i1,i2,i3)+sx(i1,i2,i3)*tys2(i1,
     & i2,i3)+tx(i1,i2,i3)*tyt2(i1,i2,i3)
      tyy23(i1,i2,i3)=ry(i1,i2,i3)*tyr2(i1,i2,i3)+sy(i1,i2,i3)*tys2(i1,
     & i2,i3)+ty(i1,i2,i3)*tyt2(i1,i2,i3)
      tyz23(i1,i2,i3)=rz(i1,i2,i3)*tyr2(i1,i2,i3)+sz(i1,i2,i3)*tys2(i1,
     & i2,i3)+tz(i1,i2,i3)*tyt2(i1,i2,i3)
      tzx22(i1,i2,i3)= rx(i1,i2,i3)*tzr2(i1,i2,i3)+sx(i1,i2,i3)*tzs2(
     & i1,i2,i3)
      tzy22(i1,i2,i3)= ry(i1,i2,i3)*tzr2(i1,i2,i3)+sy(i1,i2,i3)*tzs2(
     & i1,i2,i3)
      tzx23(i1,i2,i3)=rx(i1,i2,i3)*tzr2(i1,i2,i3)+sx(i1,i2,i3)*tzs2(i1,
     & i2,i3)+tx(i1,i2,i3)*tzt2(i1,i2,i3)
      tzy23(i1,i2,i3)=ry(i1,i2,i3)*tzr2(i1,i2,i3)+sy(i1,i2,i3)*tzs2(i1,
     & i2,i3)+ty(i1,i2,i3)*tzt2(i1,i2,i3)
      tzz23(i1,i2,i3)=rz(i1,i2,i3)*tzr2(i1,i2,i3)+sz(i1,i2,i3)*tzs2(i1,
     & i2,i3)+tz(i1,i2,i3)*tzt2(i1,i2,i3)
      uxx21(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*urr2(i1,i2,i3,kd)+(rxx22(i1,
     & i2,i3))*ur2(i1,i2,i3,kd)
      uyy21(i1,i2,i3,kd)=0
      uxy21(i1,i2,i3,kd)=0
      uxz21(i1,i2,i3,kd)=0
      uyz21(i1,i2,i3,kd)=0
      uzz21(i1,i2,i3,kd)=0
      ulaplacian21(i1,i2,i3,kd)=uxx21(i1,i2,i3,kd)
      uxx22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*urr2(i1,i2,i3,kd)+2.*(rx(i1,
     & i2,i3)*sx(i1,i2,i3))*urs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2)*uss2(
     & i1,i2,i3,kd)+(rxx22(i1,i2,i3))*ur2(i1,i2,i3,kd)+(sxx22(i1,i2,
     & i3))*us2(i1,i2,i3,kd)
      uyy22(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*urr2(i1,i2,i3,kd)+2.*(ry(i1,
     & i2,i3)*sy(i1,i2,i3))*urs2(i1,i2,i3,kd)+(sy(i1,i2,i3)**2)*uss2(
     & i1,i2,i3,kd)+(ryy22(i1,i2,i3))*ur2(i1,i2,i3,kd)+(syy22(i1,i2,
     & i3))*us2(i1,i2,i3,kd)
      uxy22(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*urr2(i1,i2,i3,kd)+(
     & rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*urs2(i1,
     & i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*uss2(i1,i2,i3,kd)+rxy22(i1,
     & i2,i3)*ur2(i1,i2,i3,kd)+sxy22(i1,i2,i3)*us2(i1,i2,i3,kd)
      uxz22(i1,i2,i3,kd)=0
      uyz22(i1,i2,i3,kd)=0
      uzz22(i1,i2,i3,kd)=0
      ulaplacian22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*urr2(
     & i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,
     & i2,i3))*urs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2)*
     & uss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3)+ryy22(i1,i2,i3))*ur2(i1,i2,
     & i3,kd)+(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*us2(i1,i2,i3,kd)
      uxx23(i1,i2,i3,kd)=rx(i1,i2,i3)**2*urr2(i1,i2,i3,kd)+sx(i1,i2,i3)
     & **2*uss2(i1,i2,i3,kd)+tx(i1,i2,i3)**2*utt2(i1,i2,i3,kd)+2.*rx(
     & i1,i2,i3)*sx(i1,i2,i3)*urs2(i1,i2,i3,kd)+2.*rx(i1,i2,i3)*tx(i1,
     & i2,i3)*urt2(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,i2,i3)*ust2(i1,
     & i2,i3,kd)+rxx23(i1,i2,i3)*ur2(i1,i2,i3,kd)+sxx23(i1,i2,i3)*us2(
     & i1,i2,i3,kd)+txx23(i1,i2,i3)*ut2(i1,i2,i3,kd)
      uyy23(i1,i2,i3,kd)=ry(i1,i2,i3)**2*urr2(i1,i2,i3,kd)+sy(i1,i2,i3)
     & **2*uss2(i1,i2,i3,kd)+ty(i1,i2,i3)**2*utt2(i1,i2,i3,kd)+2.*ry(
     & i1,i2,i3)*sy(i1,i2,i3)*urs2(i1,i2,i3,kd)+2.*ry(i1,i2,i3)*ty(i1,
     & i2,i3)*urt2(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,i2,i3)*ust2(i1,
     & i2,i3,kd)+ryy23(i1,i2,i3)*ur2(i1,i2,i3,kd)+syy23(i1,i2,i3)*us2(
     & i1,i2,i3,kd)+tyy23(i1,i2,i3)*ut2(i1,i2,i3,kd)
      uzz23(i1,i2,i3,kd)=rz(i1,i2,i3)**2*urr2(i1,i2,i3,kd)+sz(i1,i2,i3)
     & **2*uss2(i1,i2,i3,kd)+tz(i1,i2,i3)**2*utt2(i1,i2,i3,kd)+2.*rz(
     & i1,i2,i3)*sz(i1,i2,i3)*urs2(i1,i2,i3,kd)+2.*rz(i1,i2,i3)*tz(i1,
     & i2,i3)*urt2(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,i2,i3)*ust2(i1,
     & i2,i3,kd)+rzz23(i1,i2,i3)*ur2(i1,i2,i3,kd)+szz23(i1,i2,i3)*us2(
     & i1,i2,i3,kd)+tzz23(i1,i2,i3)*ut2(i1,i2,i3,kd)
      uxy23(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*urr2(i1,i2,i3,kd)+
     & sx(i1,i2,i3)*sy(i1,i2,i3)*uss2(i1,i2,i3,kd)+tx(i1,i2,i3)*ty(i1,
     & i2,i3)*utt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,
     & i3)*sx(i1,i2,i3))*urs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(i1,i2,i3)+
     & ry(i1,i2,i3)*tx(i1,i2,i3))*urt2(i1,i2,i3,kd)+(sx(i1,i2,i3)*ty(
     & i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*ust2(i1,i2,i3,kd)+rxy23(
     & i1,i2,i3)*ur2(i1,i2,i3,kd)+sxy23(i1,i2,i3)*us2(i1,i2,i3,kd)+
     & txy23(i1,i2,i3)*ut2(i1,i2,i3,kd)
      uxz23(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*urr2(i1,i2,i3,kd)+
     & sx(i1,i2,i3)*sz(i1,i2,i3)*uss2(i1,i2,i3,kd)+tx(i1,i2,i3)*tz(i1,
     & i2,i3)*utt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,i2,
     & i3)*sx(i1,i2,i3))*urs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(i1,i2,i3)+
     & rz(i1,i2,i3)*tx(i1,i2,i3))*urt2(i1,i2,i3,kd)+(sx(i1,i2,i3)*tz(
     & i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*ust2(i1,i2,i3,kd)+rxz23(
     & i1,i2,i3)*ur2(i1,i2,i3,kd)+sxz23(i1,i2,i3)*us2(i1,i2,i3,kd)+
     & txz23(i1,i2,i3)*ut2(i1,i2,i3,kd)
      uyz23(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*urr2(i1,i2,i3,kd)+
     & sy(i1,i2,i3)*sz(i1,i2,i3)*uss2(i1,i2,i3,kd)+ty(i1,i2,i3)*tz(i1,
     & i2,i3)*utt2(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,i2,
     & i3)*sy(i1,i2,i3))*urs2(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(i1,i2,i3)+
     & rz(i1,i2,i3)*ty(i1,i2,i3))*urt2(i1,i2,i3,kd)+(sy(i1,i2,i3)*tz(
     & i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*ust2(i1,i2,i3,kd)+ryz23(
     & i1,i2,i3)*ur2(i1,i2,i3,kd)+syz23(i1,i2,i3)*us2(i1,i2,i3,kd)+
     & tyz23(i1,i2,i3)*ut2(i1,i2,i3,kd)
      ulaplacian23(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(i1,
     & i2,i3)**2)*urr2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2+
     & sz(i1,i2,i3)**2)*uss2(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(i1,i2,
     & i3)**2+tz(i1,i2,i3)**2)*utt2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(
     & i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,i3))
     & *urs2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(i1,i2,i3)*
     & ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*urt2(i1,i2,i3,kd)+2.*(
     & sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)+sz(i1,i2,
     & i3)*tz(i1,i2,i3))*ust2(i1,i2,i3,kd)+(rxx23(i1,i2,i3)+ryy23(i1,
     & i2,i3)+rzz23(i1,i2,i3))*ur2(i1,i2,i3,kd)+(sxx23(i1,i2,i3)+
     & syy23(i1,i2,i3)+szz23(i1,i2,i3))*us2(i1,i2,i3,kd)+(txx23(i1,i2,
     & i3)+tyy23(i1,i2,i3)+tzz23(i1,i2,i3))*ut2(i1,i2,i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      h12(kd) = 1./(2.*dx(kd))
      h22(kd) = 1./(dx(kd)**2)
      ux23r(i1,i2,i3,kd)=(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))*h12(0)
      uy23r(i1,i2,i3,kd)=(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))*h12(1)
      uz23r(i1,i2,i3,kd)=(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))*h12(2)
      uxx23r(i1,i2,i3,kd)=(-2.*u(i1,i2,i3,kd)+(u(i1+1,i2,i3,kd)+u(i1-1,
     & i2,i3,kd)) )*h22(0)
      uyy23r(i1,i2,i3,kd)=(-2.*u(i1,i2,i3,kd)+(u(i1,i2+1,i3,kd)+u(i1,
     & i2-1,i3,kd)) )*h22(1)
      uxy23r(i1,i2,i3,kd)=(ux23r(i1,i2+1,i3,kd)-ux23r(i1,i2-1,i3,kd))*
     & h12(1)
      uzz23r(i1,i2,i3,kd)=(-2.*u(i1,i2,i3,kd)+(u(i1,i2,i3+1,kd)+u(i1,
     & i2,i3-1,kd)) )*h22(2)
      uxz23r(i1,i2,i3,kd)=(ux23r(i1,i2,i3+1,kd)-ux23r(i1,i2,i3-1,kd))*
     & h12(2)
      uyz23r(i1,i2,i3,kd)=(uy23r(i1,i2,i3+1,kd)-uy23r(i1,i2,i3-1,kd))*
     & h12(2)
      ux21r(i1,i2,i3,kd)= ux23r(i1,i2,i3,kd)
      uy21r(i1,i2,i3,kd)= uy23r(i1,i2,i3,kd)
      uz21r(i1,i2,i3,kd)= uz23r(i1,i2,i3,kd)
      uxx21r(i1,i2,i3,kd)= uxx23r(i1,i2,i3,kd)
      uyy21r(i1,i2,i3,kd)= uyy23r(i1,i2,i3,kd)
      uzz21r(i1,i2,i3,kd)= uzz23r(i1,i2,i3,kd)
      uxy21r(i1,i2,i3,kd)= uxy23r(i1,i2,i3,kd)
      uxz21r(i1,i2,i3,kd)= uxz23r(i1,i2,i3,kd)
      uyz21r(i1,i2,i3,kd)= uyz23r(i1,i2,i3,kd)
      ulaplacian21r(i1,i2,i3,kd)=uxx23r(i1,i2,i3,kd)
      ux22r(i1,i2,i3,kd)= ux23r(i1,i2,i3,kd)
      uy22r(i1,i2,i3,kd)= uy23r(i1,i2,i3,kd)
      uz22r(i1,i2,i3,kd)= uz23r(i1,i2,i3,kd)
      uxx22r(i1,i2,i3,kd)= uxx23r(i1,i2,i3,kd)
      uyy22r(i1,i2,i3,kd)= uyy23r(i1,i2,i3,kd)
      uzz22r(i1,i2,i3,kd)= uzz23r(i1,i2,i3,kd)
      uxy22r(i1,i2,i3,kd)= uxy23r(i1,i2,i3,kd)
      uxz22r(i1,i2,i3,kd)= uxz23r(i1,i2,i3,kd)
      uyz22r(i1,i2,i3,kd)= uyz23r(i1,i2,i3,kd)
      ulaplacian22r(i1,i2,i3,kd)=uxx23r(i1,i2,i3,kd)+uyy23r(i1,i2,i3,
     & kd)
      ulaplacian23r(i1,i2,i3,kd)=uxx23r(i1,i2,i3,kd)+uyy23r(i1,i2,i3,
     & kd)+uzz23r(i1,i2,i3,kd)
      uxxx22r(i1,i2,i3,kd)=(-2.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))+(u(
     & i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
      uyyy22r(i1,i2,i3,kd)=(-2.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))+(u(
     & i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
      uxxy22r(i1,i2,i3,kd)=( uxx22r(i1,i2+1,i3,kd)-uxx22r(i1,i2-1,i3,
     & kd))/(2.*dx(1))
      uxyy22r(i1,i2,i3,kd)=( uyy22r(i1+1,i2,i3,kd)-uyy22r(i1-1,i2,i3,
     & kd))/(2.*dx(0))
      uxxxx22r(i1,i2,i3,kd)=(6.*u(i1,i2,i3,kd)-4.*(u(i1+1,i2,i3,kd)+u(
     & i1-1,i2,i3,kd))+(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)) )/(dx(0)**
     & 4)
      uyyyy22r(i1,i2,i3,kd)=(6.*u(i1,i2,i3,kd)-4.*(u(i1,i2+1,i3,kd)+u(
     & i1,i2-1,i3,kd))+(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )/(dx(1)**
     & 4)
      uxxyy22r(i1,i2,i3,kd)=( 4.*u(i1,i2,i3,kd)     -2.*(u(i1+1,i2,i3,
     & kd)+u(i1-1,i2,i3,kd)+u(i1,i2+1,i3,kd)+u(i1,i2-1,i3,kd))   +   (
     & u(i1+1,i2+1,i3,kd)+u(i1-1,i2+1,i3,kd)+u(i1+1,i2-1,i3,kd)+u(i1-
     & 1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      ! 2D laplacian squared = u.xxxx + 2 u.xxyy + u.yyyy
      uLapSq22r(i1,i2,i3,kd)= ( 6.*u(i1,i2,i3,kd)   - 4.*(u(i1+1,i2,i3,
     & kd)+u(i1-1,i2,i3,kd))    +(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)) )
     & /(dx(0)**4) +( 6.*u(i1,i2,i3,kd)    -4.*(u(i1,i2+1,i3,kd)+u(i1,
     & i2-1,i3,kd))    +(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )/(dx(1)**
     & 4)  +( 8.*u(i1,i2,i3,kd)     -4.*(u(i1+1,i2,i3,kd)+u(i1-1,i2,
     & i3,kd)+u(i1,i2+1,i3,kd)+u(i1,i2-1,i3,kd))   +2.*(u(i1+1,i2+1,
     & i3,kd)+u(i1-1,i2+1,i3,kd)+u(i1+1,i2-1,i3,kd)+u(i1-1,i2-1,i3,kd)
     & ) )/(dx(0)**2*dx(1)**2)
      uxxx23r(i1,i2,i3,kd)=(-2.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))+(u(
     & i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
      uyyy23r(i1,i2,i3,kd)=(-2.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))+(u(
     & i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
      uzzz23r(i1,i2,i3,kd)=(-2.*(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))+(u(
     & i1,i2,i3+2,kd)-u(i1,i2,i3-2,kd)) )*h22(1)*h12(2)
      uxxy23r(i1,i2,i3,kd)=( uxx22r(i1,i2+1,i3,kd)-uxx22r(i1,i2-1,i3,
     & kd))/(2.*dx(1))
      uxyy23r(i1,i2,i3,kd)=( uyy22r(i1+1,i2,i3,kd)-uyy22r(i1-1,i2,i3,
     & kd))/(2.*dx(0))
      uxxz23r(i1,i2,i3,kd)=( uxx22r(i1,i2,i3+1,kd)-uxx22r(i1,i2,i3-1,
     & kd))/(2.*dx(2))
      uyyz23r(i1,i2,i3,kd)=( uyy22r(i1,i2,i3+1,kd)-uyy22r(i1,i2,i3-1,
     & kd))/(2.*dx(2))
      uxzz23r(i1,i2,i3,kd)=( uzz22r(i1+1,i2,i3,kd)-uzz22r(i1-1,i2,i3,
     & kd))/(2.*dx(0))
      uyzz23r(i1,i2,i3,kd)=( uzz22r(i1,i2+1,i3,kd)-uzz22r(i1,i2-1,i3,
     & kd))/(2.*dx(1))
      uxxxx23r(i1,i2,i3,kd)=(6.*u(i1,i2,i3,kd)-4.*(u(i1+1,i2,i3,kd)+u(
     & i1-1,i2,i3,kd))+(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)) )/(dx(0)**
     & 4)
      uyyyy23r(i1,i2,i3,kd)=(6.*u(i1,i2,i3,kd)-4.*(u(i1,i2+1,i3,kd)+u(
     & i1,i2-1,i3,kd))+(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )/(dx(1)**
     & 4)
      uzzzz23r(i1,i2,i3,kd)=(6.*u(i1,i2,i3,kd)-4.*(u(i1,i2,i3+1,kd)+u(
     & i1,i2,i3-1,kd))+(u(i1,i2,i3+2,kd)+u(i1,i2,i3-2,kd)) )/(dx(2)**
     & 4)
      uxxyy23r(i1,i2,i3,kd)=( 4.*u(i1,i2,i3,kd)     -2.*(u(i1+1,i2,i3,
     & kd)+u(i1-1,i2,i3,kd)+u(i1,i2+1,i3,kd)+u(i1,i2-1,i3,kd))   +   (
     & u(i1+1,i2+1,i3,kd)+u(i1-1,i2+1,i3,kd)+u(i1+1,i2-1,i3,kd)+u(i1-
     & 1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      uxxzz23r(i1,i2,i3,kd)=( 4.*u(i1,i2,i3,kd)     -2.*(u(i1+1,i2,i3,
     & kd)+u(i1-1,i2,i3,kd)+u(i1,i2,i3+1,kd)+u(i1,i2,i3-1,kd))   +   (
     & u(i1+1,i2,i3+1,kd)+u(i1-1,i2,i3+1,kd)+u(i1+1,i2,i3-1,kd)+u(i1-
     & 1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)
      uyyzz23r(i1,i2,i3,kd)=( 4.*u(i1,i2,i3,kd)     -2.*(u(i1,i2+1,i3,
     & kd)  +u(i1,i2-1,i3,kd)+  u(i1,i2  ,i3+1,kd)+u(i1,i2  ,i3-1,kd))
     &    +   (u(i1,i2+1,i3+1,kd)+u(i1,i2-1,i3+1,kd)+u(i1,i2+1,i3-1,
     & kd)+u(i1,i2-1,i3-1,kd)) )/(dx(1)**2*dx(2)**2)
      ! 3D laplacian squared = u.xxxx + u.yyyy + u.zzzz + 2 (u.xxyy + u.xxzz + u.yyzz )
      uLapSq23r(i1,i2,i3,kd)= ( 6.*u(i1,i2,i3,kd)   - 4.*(u(i1+1,i2,i3,
     & kd)+u(i1-1,i2,i3,kd))    +(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)) )
     & /(dx(0)**4) +( 6.*u(i1,i2,i3,kd)    -4.*(u(i1,i2+1,i3,kd)+u(i1,
     & i2-1,i3,kd))    +(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )/(dx(1)**
     & 4)  +( 6.*u(i1,i2,i3,kd)    -4.*(u(i1,i2,i3+1,kd)+u(i1,i2,i3-1,
     & kd))    +(u(i1,i2,i3+2,kd)+u(i1,i2,i3-2,kd)) )/(dx(2)**4)  +( 
     & 8.*u(i1,i2,i3,kd)     -4.*(u(i1+1,i2,i3,kd)  +u(i1-1,i2,i3,kd) 
     &  +u(i1  ,i2+1,i3,kd)+u(i1  ,i2-1,i3,kd))   +2.*(u(i1+1,i2+1,i3,
     & kd)+u(i1-1,i2+1,i3,kd)+u(i1+1,i2-1,i3,kd)+u(i1-1,i2-1,i3,kd)) )
     & /(dx(0)**2*dx(1)**2)+( 8.*u(i1,i2,i3,kd)     -4.*(u(i1+1,i2,i3,
     & kd)  +u(i1-1,i2,i3,kd)  +u(i1  ,i2,i3+1,kd)+u(i1  ,i2,i3-1,kd))
     &    +2.*(u(i1+1,i2,i3+1,kd)+u(i1-1,i2,i3+1,kd)+u(i1+1,i2,i3-1,
     & kd)+u(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)+( 8.*u(i1,i2,i3,
     & kd)     -4.*(u(i1,i2+1,i3,kd)  +u(i1,i2-1,i3,kd)  +u(i1,i2  ,
     & i3+1,kd)+u(i1,i2  ,i3-1,kd))   +2.*(u(i1,i2+1,i3+1,kd)+u(i1,i2-
     & 1,i3+1,kd)+u(i1,i2+1,i3-1,kd)+u(i1,i2-1,i3-1,kd)) )/(dx(1)**2*
     & dx(2)**2)
      umr2(i1,i2,i3,kd)=(um(i1+1,i2,i3,kd)-um(i1-1,i2,i3,kd))*d12(0)
      ums2(i1,i2,i3,kd)=(um(i1,i2+1,i3,kd)-um(i1,i2-1,i3,kd))*d12(1)
      umt2(i1,i2,i3,kd)=(um(i1,i2,i3+1,kd)-um(i1,i2,i3-1,kd))*d12(2)
      umrr2(i1,i2,i3,kd)=(-2.*um(i1,i2,i3,kd)+(um(i1+1,i2,i3,kd)+um(i1-
     & 1,i2,i3,kd)) )*d22(0)
      umss2(i1,i2,i3,kd)=(-2.*um(i1,i2,i3,kd)+(um(i1,i2+1,i3,kd)+um(i1,
     & i2-1,i3,kd)) )*d22(1)
      umrs2(i1,i2,i3,kd)=(umr2(i1,i2+1,i3,kd)-umr2(i1,i2-1,i3,kd))*d12(
     & 1)
      umtt2(i1,i2,i3,kd)=(-2.*um(i1,i2,i3,kd)+(um(i1,i2,i3+1,kd)+um(i1,
     & i2,i3-1,kd)) )*d22(2)
      umrt2(i1,i2,i3,kd)=(umr2(i1,i2,i3+1,kd)-umr2(i1,i2,i3-1,kd))*d12(
     & 2)
      umst2(i1,i2,i3,kd)=(ums2(i1,i2,i3+1,kd)-ums2(i1,i2,i3-1,kd))*d12(
     & 2)
      umrrr2(i1,i2,i3,kd)=(-2.*(um(i1+1,i2,i3,kd)-um(i1-1,i2,i3,kd))+(
     & um(i1+2,i2,i3,kd)-um(i1-2,i2,i3,kd)) )*d22(0)*d12(0)
      umsss2(i1,i2,i3,kd)=(-2.*(um(i1,i2+1,i3,kd)-um(i1,i2-1,i3,kd))+(
     & um(i1,i2+2,i3,kd)-um(i1,i2-2,i3,kd)) )*d22(1)*d12(1)
      umttt2(i1,i2,i3,kd)=(-2.*(um(i1,i2,i3+1,kd)-um(i1,i2,i3-1,kd))+(
     & um(i1,i2,i3+2,kd)-um(i1,i2,i3-2,kd)) )*d22(2)*d12(2)
      umx21(i1,i2,i3,kd)= rx(i1,i2,i3)*umr2(i1,i2,i3,kd)
      umy21(i1,i2,i3,kd)=0
      umz21(i1,i2,i3,kd)=0
      umx22(i1,i2,i3,kd)= rx(i1,i2,i3)*umr2(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & ums2(i1,i2,i3,kd)
      umy22(i1,i2,i3,kd)= ry(i1,i2,i3)*umr2(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & ums2(i1,i2,i3,kd)
      umz22(i1,i2,i3,kd)=0
      umx23(i1,i2,i3,kd)=rx(i1,i2,i3)*umr2(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & ums2(i1,i2,i3,kd)+tx(i1,i2,i3)*umt2(i1,i2,i3,kd)
      umy23(i1,i2,i3,kd)=ry(i1,i2,i3)*umr2(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & ums2(i1,i2,i3,kd)+ty(i1,i2,i3)*umt2(i1,i2,i3,kd)
      umz23(i1,i2,i3,kd)=rz(i1,i2,i3)*umr2(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & ums2(i1,i2,i3,kd)+tz(i1,i2,i3)*umt2(i1,i2,i3,kd)
      umxx21(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*umrr2(i1,i2,i3,kd)+(rxx22(
     & i1,i2,i3))*umr2(i1,i2,i3,kd)
      umyy21(i1,i2,i3,kd)=0
      umxy21(i1,i2,i3,kd)=0
      umxz21(i1,i2,i3,kd)=0
      umyz21(i1,i2,i3,kd)=0
      umzz21(i1,i2,i3,kd)=0
      umlaplacian21(i1,i2,i3,kd)=umxx21(i1,i2,i3,kd)
      umxx22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*umrr2(i1,i2,i3,kd)+2.*(rx(
     & i1,i2,i3)*sx(i1,i2,i3))*umrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2)*
     & umss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3))*umr2(i1,i2,i3,kd)+(sxx22(
     & i1,i2,i3))*ums2(i1,i2,i3,kd)
      umyy22(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*umrr2(i1,i2,i3,kd)+2.*(ry(
     & i1,i2,i3)*sy(i1,i2,i3))*umrs2(i1,i2,i3,kd)+(sy(i1,i2,i3)**2)*
     & umss2(i1,i2,i3,kd)+(ryy22(i1,i2,i3))*umr2(i1,i2,i3,kd)+(syy22(
     & i1,i2,i3))*ums2(i1,i2,i3,kd)
      umxy22(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*umrr2(i1,i2,i3,kd)+
     & (rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*umrs2(i1,
     & i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*umss2(i1,i2,i3,kd)+rxy22(
     & i1,i2,i3)*umr2(i1,i2,i3,kd)+sxy22(i1,i2,i3)*ums2(i1,i2,i3,kd)
      umxz22(i1,i2,i3,kd)=0
      umyz22(i1,i2,i3,kd)=0
      umzz22(i1,i2,i3,kd)=0
      umlaplacian22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & umrr2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*
     & sy(i1,i2,i3))*umrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)*
     & *2)*umss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3)+ryy22(i1,i2,i3))*umr2(
     & i1,i2,i3,kd)+(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*ums2(i1,i2,i3,
     & kd)
      umxx23(i1,i2,i3,kd)=rx(i1,i2,i3)**2*umrr2(i1,i2,i3,kd)+sx(i1,i2,
     & i3)**2*umss2(i1,i2,i3,kd)+tx(i1,i2,i3)**2*umtt2(i1,i2,i3,kd)+
     & 2.*rx(i1,i2,i3)*sx(i1,i2,i3)*umrs2(i1,i2,i3,kd)+2.*rx(i1,i2,i3)
     & *tx(i1,i2,i3)*umrt2(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,i2,i3)*
     & umst2(i1,i2,i3,kd)+rxx23(i1,i2,i3)*umr2(i1,i2,i3,kd)+sxx23(i1,
     & i2,i3)*ums2(i1,i2,i3,kd)+txx23(i1,i2,i3)*umt2(i1,i2,i3,kd)
      umyy23(i1,i2,i3,kd)=ry(i1,i2,i3)**2*umrr2(i1,i2,i3,kd)+sy(i1,i2,
     & i3)**2*umss2(i1,i2,i3,kd)+ty(i1,i2,i3)**2*umtt2(i1,i2,i3,kd)+
     & 2.*ry(i1,i2,i3)*sy(i1,i2,i3)*umrs2(i1,i2,i3,kd)+2.*ry(i1,i2,i3)
     & *ty(i1,i2,i3)*umrt2(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,i2,i3)*
     & umst2(i1,i2,i3,kd)+ryy23(i1,i2,i3)*umr2(i1,i2,i3,kd)+syy23(i1,
     & i2,i3)*ums2(i1,i2,i3,kd)+tyy23(i1,i2,i3)*umt2(i1,i2,i3,kd)
      umzz23(i1,i2,i3,kd)=rz(i1,i2,i3)**2*umrr2(i1,i2,i3,kd)+sz(i1,i2,
     & i3)**2*umss2(i1,i2,i3,kd)+tz(i1,i2,i3)**2*umtt2(i1,i2,i3,kd)+
     & 2.*rz(i1,i2,i3)*sz(i1,i2,i3)*umrs2(i1,i2,i3,kd)+2.*rz(i1,i2,i3)
     & *tz(i1,i2,i3)*umrt2(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,i2,i3)*
     & umst2(i1,i2,i3,kd)+rzz23(i1,i2,i3)*umr2(i1,i2,i3,kd)+szz23(i1,
     & i2,i3)*ums2(i1,i2,i3,kd)+tzz23(i1,i2,i3)*umt2(i1,i2,i3,kd)
      umxy23(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*umrr2(i1,i2,i3,kd)+
     & sx(i1,i2,i3)*sy(i1,i2,i3)*umss2(i1,i2,i3,kd)+tx(i1,i2,i3)*ty(
     & i1,i2,i3)*umtt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,
     & i2,i3)*sx(i1,i2,i3))*umrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(i1,i2,
     & i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*umrt2(i1,i2,i3,kd)+(sx(i1,i2,i3)
     & *ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*umst2(i1,i2,i3,kd)+
     & rxy23(i1,i2,i3)*umr2(i1,i2,i3,kd)+sxy23(i1,i2,i3)*ums2(i1,i2,
     & i3,kd)+txy23(i1,i2,i3)*umt2(i1,i2,i3,kd)
      umxz23(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*umrr2(i1,i2,i3,kd)+
     & sx(i1,i2,i3)*sz(i1,i2,i3)*umss2(i1,i2,i3,kd)+tx(i1,i2,i3)*tz(
     & i1,i2,i3)*umtt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,
     & i2,i3)*sx(i1,i2,i3))*umrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(i1,i2,
     & i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*umrt2(i1,i2,i3,kd)+(sx(i1,i2,i3)
     & *tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*umst2(i1,i2,i3,kd)+
     & rxz23(i1,i2,i3)*umr2(i1,i2,i3,kd)+sxz23(i1,i2,i3)*ums2(i1,i2,
     & i3,kd)+txz23(i1,i2,i3)*umt2(i1,i2,i3,kd)
      umyz23(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*umrr2(i1,i2,i3,kd)+
     & sy(i1,i2,i3)*sz(i1,i2,i3)*umss2(i1,i2,i3,kd)+ty(i1,i2,i3)*tz(
     & i1,i2,i3)*umtt2(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,
     & i2,i3)*sy(i1,i2,i3))*umrs2(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(i1,i2,
     & i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*umrt2(i1,i2,i3,kd)+(sy(i1,i2,i3)
     & *tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*umst2(i1,i2,i3,kd)+
     & ryz23(i1,i2,i3)*umr2(i1,i2,i3,kd)+syz23(i1,i2,i3)*ums2(i1,i2,
     & i3,kd)+tyz23(i1,i2,i3)*umt2(i1,i2,i3,kd)
      umlaplacian23(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*umrr2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)*
     & *2+sz(i1,i2,i3)**2)*umss2(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(i1,
     & i2,i3)**2+tz(i1,i2,i3)**2)*umtt2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*
     & sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,
     & i3))*umrs2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(i1,
     & i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*umrt2(i1,i2,i3,
     & kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)+
     & sz(i1,i2,i3)*tz(i1,i2,i3))*umst2(i1,i2,i3,kd)+(rxx23(i1,i2,i3)+
     & ryy23(i1,i2,i3)+rzz23(i1,i2,i3))*umr2(i1,i2,i3,kd)+(sxx23(i1,
     & i2,i3)+syy23(i1,i2,i3)+szz23(i1,i2,i3))*ums2(i1,i2,i3,kd)+(
     & txx23(i1,i2,i3)+tyy23(i1,i2,i3)+tzz23(i1,i2,i3))*umt2(i1,i2,i3,
     & kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      umx23r(i1,i2,i3,kd)=(um(i1+1,i2,i3,kd)-um(i1-1,i2,i3,kd))*h12(0)
      umy23r(i1,i2,i3,kd)=(um(i1,i2+1,i3,kd)-um(i1,i2-1,i3,kd))*h12(1)
      umz23r(i1,i2,i3,kd)=(um(i1,i2,i3+1,kd)-um(i1,i2,i3-1,kd))*h12(2)
      umxx23r(i1,i2,i3,kd)=(-2.*um(i1,i2,i3,kd)+(um(i1+1,i2,i3,kd)+um(
     & i1-1,i2,i3,kd)) )*h22(0)
      umyy23r(i1,i2,i3,kd)=(-2.*um(i1,i2,i3,kd)+(um(i1,i2+1,i3,kd)+um(
     & i1,i2-1,i3,kd)) )*h22(1)
      umxy23r(i1,i2,i3,kd)=(umx23r(i1,i2+1,i3,kd)-umx23r(i1,i2-1,i3,kd)
     & )*h12(1)
      umzz23r(i1,i2,i3,kd)=(-2.*um(i1,i2,i3,kd)+(um(i1,i2,i3+1,kd)+um(
     & i1,i2,i3-1,kd)) )*h22(2)
      umxz23r(i1,i2,i3,kd)=(umx23r(i1,i2,i3+1,kd)-umx23r(i1,i2,i3-1,kd)
     & )*h12(2)
      umyz23r(i1,i2,i3,kd)=(umy23r(i1,i2,i3+1,kd)-umy23r(i1,i2,i3-1,kd)
     & )*h12(2)
      umx21r(i1,i2,i3,kd)= umx23r(i1,i2,i3,kd)
      umy21r(i1,i2,i3,kd)= umy23r(i1,i2,i3,kd)
      umz21r(i1,i2,i3,kd)= umz23r(i1,i2,i3,kd)
      umxx21r(i1,i2,i3,kd)= umxx23r(i1,i2,i3,kd)
      umyy21r(i1,i2,i3,kd)= umyy23r(i1,i2,i3,kd)
      umzz21r(i1,i2,i3,kd)= umzz23r(i1,i2,i3,kd)
      umxy21r(i1,i2,i3,kd)= umxy23r(i1,i2,i3,kd)
      umxz21r(i1,i2,i3,kd)= umxz23r(i1,i2,i3,kd)
      umyz21r(i1,i2,i3,kd)= umyz23r(i1,i2,i3,kd)
      umlaplacian21r(i1,i2,i3,kd)=umxx23r(i1,i2,i3,kd)
      umx22r(i1,i2,i3,kd)= umx23r(i1,i2,i3,kd)
      umy22r(i1,i2,i3,kd)= umy23r(i1,i2,i3,kd)
      umz22r(i1,i2,i3,kd)= umz23r(i1,i2,i3,kd)
      umxx22r(i1,i2,i3,kd)= umxx23r(i1,i2,i3,kd)
      umyy22r(i1,i2,i3,kd)= umyy23r(i1,i2,i3,kd)
      umzz22r(i1,i2,i3,kd)= umzz23r(i1,i2,i3,kd)
      umxy22r(i1,i2,i3,kd)= umxy23r(i1,i2,i3,kd)
      umxz22r(i1,i2,i3,kd)= umxz23r(i1,i2,i3,kd)
      umyz22r(i1,i2,i3,kd)= umyz23r(i1,i2,i3,kd)
      umlaplacian22r(i1,i2,i3,kd)=umxx23r(i1,i2,i3,kd)+umyy23r(i1,i2,
     & i3,kd)
      umlaplacian23r(i1,i2,i3,kd)=umxx23r(i1,i2,i3,kd)+umyy23r(i1,i2,
     & i3,kd)+umzz23r(i1,i2,i3,kd)
      umxxx22r(i1,i2,i3,kd)=(-2.*(um(i1+1,i2,i3,kd)-um(i1-1,i2,i3,kd))+
     & (um(i1+2,i2,i3,kd)-um(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
      umyyy22r(i1,i2,i3,kd)=(-2.*(um(i1,i2+1,i3,kd)-um(i1,i2-1,i3,kd))+
     & (um(i1,i2+2,i3,kd)-um(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
      umxxy22r(i1,i2,i3,kd)=( umxx22r(i1,i2+1,i3,kd)-umxx22r(i1,i2-1,
     & i3,kd))/(2.*dx(1))
      umxyy22r(i1,i2,i3,kd)=( umyy22r(i1+1,i2,i3,kd)-umyy22r(i1-1,i2,
     & i3,kd))/(2.*dx(0))
      umxxxx22r(i1,i2,i3,kd)=(6.*um(i1,i2,i3,kd)-4.*(um(i1+1,i2,i3,kd)+
     & um(i1-1,i2,i3,kd))+(um(i1+2,i2,i3,kd)+um(i1-2,i2,i3,kd)) )/(dx(
     & 0)**4)
      umyyyy22r(i1,i2,i3,kd)=(6.*um(i1,i2,i3,kd)-4.*(um(i1,i2+1,i3,kd)+
     & um(i1,i2-1,i3,kd))+(um(i1,i2+2,i3,kd)+um(i1,i2-2,i3,kd)) )/(dx(
     & 1)**4)
      umxxyy22r(i1,i2,i3,kd)=( 4.*um(i1,i2,i3,kd)     -2.*(um(i1+1,i2,
     & i3,kd)+um(i1-1,i2,i3,kd)+um(i1,i2+1,i3,kd)+um(i1,i2-1,i3,kd))  
     &  +   (um(i1+1,i2+1,i3,kd)+um(i1-1,i2+1,i3,kd)+um(i1+1,i2-1,i3,
     & kd)+um(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      ! 2D laplacian squared = um.xxxx + 2 um.xxyy + um.yyyy
      umLapSq22r(i1,i2,i3,kd)= ( 6.*um(i1,i2,i3,kd)   - 4.*(um(i1+1,i2,
     & i3,kd)+um(i1-1,i2,i3,kd))    +(um(i1+2,i2,i3,kd)+um(i1-2,i2,i3,
     & kd)) )/(dx(0)**4) +( 6.*um(i1,i2,i3,kd)    -4.*(um(i1,i2+1,i3,
     & kd)+um(i1,i2-1,i3,kd))    +(um(i1,i2+2,i3,kd)+um(i1,i2-2,i3,kd)
     & ) )/(dx(1)**4)  +( 8.*um(i1,i2,i3,kd)     -4.*(um(i1+1,i2,i3,
     & kd)+um(i1-1,i2,i3,kd)+um(i1,i2+1,i3,kd)+um(i1,i2-1,i3,kd))   +
     & 2.*(um(i1+1,i2+1,i3,kd)+um(i1-1,i2+1,i3,kd)+um(i1+1,i2-1,i3,kd)
     & +um(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      umxxx23r(i1,i2,i3,kd)=(-2.*(um(i1+1,i2,i3,kd)-um(i1-1,i2,i3,kd))+
     & (um(i1+2,i2,i3,kd)-um(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
      umyyy23r(i1,i2,i3,kd)=(-2.*(um(i1,i2+1,i3,kd)-um(i1,i2-1,i3,kd))+
     & (um(i1,i2+2,i3,kd)-um(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
      umzzz23r(i1,i2,i3,kd)=(-2.*(um(i1,i2,i3+1,kd)-um(i1,i2,i3-1,kd))+
     & (um(i1,i2,i3+2,kd)-um(i1,i2,i3-2,kd)) )*h22(1)*h12(2)
      umxxy23r(i1,i2,i3,kd)=( umxx22r(i1,i2+1,i3,kd)-umxx22r(i1,i2-1,
     & i3,kd))/(2.*dx(1))
      umxyy23r(i1,i2,i3,kd)=( umyy22r(i1+1,i2,i3,kd)-umyy22r(i1-1,i2,
     & i3,kd))/(2.*dx(0))
      umxxz23r(i1,i2,i3,kd)=( umxx22r(i1,i2,i3+1,kd)-umxx22r(i1,i2,i3-
     & 1,kd))/(2.*dx(2))
      umyyz23r(i1,i2,i3,kd)=( umyy22r(i1,i2,i3+1,kd)-umyy22r(i1,i2,i3-
     & 1,kd))/(2.*dx(2))
      umxzz23r(i1,i2,i3,kd)=( umzz22r(i1+1,i2,i3,kd)-umzz22r(i1-1,i2,
     & i3,kd))/(2.*dx(0))
      umyzz23r(i1,i2,i3,kd)=( umzz22r(i1,i2+1,i3,kd)-umzz22r(i1,i2-1,
     & i3,kd))/(2.*dx(1))
      umxxxx23r(i1,i2,i3,kd)=(6.*um(i1,i2,i3,kd)-4.*(um(i1+1,i2,i3,kd)+
     & um(i1-1,i2,i3,kd))+(um(i1+2,i2,i3,kd)+um(i1-2,i2,i3,kd)) )/(dx(
     & 0)**4)
      umyyyy23r(i1,i2,i3,kd)=(6.*um(i1,i2,i3,kd)-4.*(um(i1,i2+1,i3,kd)+
     & um(i1,i2-1,i3,kd))+(um(i1,i2+2,i3,kd)+um(i1,i2-2,i3,kd)) )/(dx(
     & 1)**4)
      umzzzz23r(i1,i2,i3,kd)=(6.*um(i1,i2,i3,kd)-4.*(um(i1,i2,i3+1,kd)+
     & um(i1,i2,i3-1,kd))+(um(i1,i2,i3+2,kd)+um(i1,i2,i3-2,kd)) )/(dx(
     & 2)**4)
      umxxyy23r(i1,i2,i3,kd)=( 4.*um(i1,i2,i3,kd)     -2.*(um(i1+1,i2,
     & i3,kd)+um(i1-1,i2,i3,kd)+um(i1,i2+1,i3,kd)+um(i1,i2-1,i3,kd))  
     &  +   (um(i1+1,i2+1,i3,kd)+um(i1-1,i2+1,i3,kd)+um(i1+1,i2-1,i3,
     & kd)+um(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      umxxzz23r(i1,i2,i3,kd)=( 4.*um(i1,i2,i3,kd)     -2.*(um(i1+1,i2,
     & i3,kd)+um(i1-1,i2,i3,kd)+um(i1,i2,i3+1,kd)+um(i1,i2,i3-1,kd))  
     &  +   (um(i1+1,i2,i3+1,kd)+um(i1-1,i2,i3+1,kd)+um(i1+1,i2,i3-1,
     & kd)+um(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)
      umyyzz23r(i1,i2,i3,kd)=( 4.*um(i1,i2,i3,kd)     -2.*(um(i1,i2+1,
     & i3,kd)  +um(i1,i2-1,i3,kd)+  um(i1,i2  ,i3+1,kd)+um(i1,i2  ,i3-
     & 1,kd))   +   (um(i1,i2+1,i3+1,kd)+um(i1,i2-1,i3+1,kd)+um(i1,i2+
     & 1,i3-1,kd)+um(i1,i2-1,i3-1,kd)) )/(dx(1)**2*dx(2)**2)
      ! 3D laplacian squared = um.xxxx + um.yyyy + um.zzzz + 2 (um.xxyy + um.xxzz + um.yyzz )
      umLapSq23r(i1,i2,i3,kd)= ( 6.*um(i1,i2,i3,kd)   - 4.*(um(i1+1,i2,
     & i3,kd)+um(i1-1,i2,i3,kd))    +(um(i1+2,i2,i3,kd)+um(i1-2,i2,i3,
     & kd)) )/(dx(0)**4) +( 6.*um(i1,i2,i3,kd)    -4.*(um(i1,i2+1,i3,
     & kd)+um(i1,i2-1,i3,kd))    +(um(i1,i2+2,i3,kd)+um(i1,i2-2,i3,kd)
     & ) )/(dx(1)**4)  +( 6.*um(i1,i2,i3,kd)    -4.*(um(i1,i2,i3+1,kd)
     & +um(i1,i2,i3-1,kd))    +(um(i1,i2,i3+2,kd)+um(i1,i2,i3-2,kd)) )
     & /(dx(2)**4)  +( 8.*um(i1,i2,i3,kd)     -4.*(um(i1+1,i2,i3,kd)  
     & +um(i1-1,i2,i3,kd)  +um(i1  ,i2+1,i3,kd)+um(i1  ,i2-1,i3,kd))  
     &  +2.*(um(i1+1,i2+1,i3,kd)+um(i1-1,i2+1,i3,kd)+um(i1+1,i2-1,i3,
     & kd)+um(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)+( 8.*um(i1,i2,
     & i3,kd)     -4.*(um(i1+1,i2,i3,kd)  +um(i1-1,i2,i3,kd)  +um(i1  
     & ,i2,i3+1,kd)+um(i1  ,i2,i3-1,kd))   +2.*(um(i1+1,i2,i3+1,kd)+
     & um(i1-1,i2,i3+1,kd)+um(i1+1,i2,i3-1,kd)+um(i1-1,i2,i3-1,kd)) )
     & /(dx(0)**2*dx(2)**2)+( 8.*um(i1,i2,i3,kd)     -4.*(um(i1,i2+1,
     & i3,kd)  +um(i1,i2-1,i3,kd)  +um(i1,i2  ,i3+1,kd)+um(i1,i2  ,i3-
     & 1,kd))   +2.*(um(i1,i2+1,i3+1,kd)+um(i1,i2-1,i3+1,kd)+um(i1,i2+
     & 1,i3-1,kd)+um(i1,i2-1,i3-1,kd)) )/(dx(1)**2*dx(2)**2)

      vrar2(i1,i2,i3,kd)=(vra(i1+1,i2,i3,kd)-vra(i1-1,i2,i3,kd))*d12(0)
      vras2(i1,i2,i3,kd)=(vra(i1,i2+1,i3,kd)-vra(i1,i2-1,i3,kd))*d12(1)
      vrat2(i1,i2,i3,kd)=(vra(i1,i2,i3+1,kd)-vra(i1,i2,i3-1,kd))*d12(2)
      vrarr2(i1,i2,i3,kd)=(-2.*vra(i1,i2,i3,kd)+(vra(i1+1,i2,i3,kd)+
     & vra(i1-1,i2,i3,kd)) )*d22(0)
      vrass2(i1,i2,i3,kd)=(-2.*vra(i1,i2,i3,kd)+(vra(i1,i2+1,i3,kd)+
     & vra(i1,i2-1,i3,kd)) )*d22(1)
      vrars2(i1,i2,i3,kd)=(vrar2(i1,i2+1,i3,kd)-vrar2(i1,i2-1,i3,kd))*
     & d12(1)
      vratt2(i1,i2,i3,kd)=(-2.*vra(i1,i2,i3,kd)+(vra(i1,i2,i3+1,kd)+
     & vra(i1,i2,i3-1,kd)) )*d22(2)
      vrart2(i1,i2,i3,kd)=(vrar2(i1,i2,i3+1,kd)-vrar2(i1,i2,i3-1,kd))*
     & d12(2)
      vrast2(i1,i2,i3,kd)=(vras2(i1,i2,i3+1,kd)-vras2(i1,i2,i3-1,kd))*
     & d12(2)
      vrarrr2(i1,i2,i3,kd)=(-2.*(vra(i1+1,i2,i3,kd)-vra(i1-1,i2,i3,kd))
     & +(vra(i1+2,i2,i3,kd)-vra(i1-2,i2,i3,kd)) )*d22(0)*d12(0)
      vrasss2(i1,i2,i3,kd)=(-2.*(vra(i1,i2+1,i3,kd)-vra(i1,i2-1,i3,kd))
     & +(vra(i1,i2+2,i3,kd)-vra(i1,i2-2,i3,kd)) )*d22(1)*d12(1)
      vrattt2(i1,i2,i3,kd)=(-2.*(vra(i1,i2,i3+1,kd)-vra(i1,i2,i3-1,kd))
     & +(vra(i1,i2,i3+2,kd)-vra(i1,i2,i3-2,kd)) )*d22(2)*d12(2)
      vrax21(i1,i2,i3,kd)= rx(i1,i2,i3)*vrar2(i1,i2,i3,kd)
      vray21(i1,i2,i3,kd)=0
      vraz21(i1,i2,i3,kd)=0
      vrax22(i1,i2,i3,kd)= rx(i1,i2,i3)*vrar2(i1,i2,i3,kd)+sx(i1,i2,i3)
     & *vras2(i1,i2,i3,kd)
      vray22(i1,i2,i3,kd)= ry(i1,i2,i3)*vrar2(i1,i2,i3,kd)+sy(i1,i2,i3)
     & *vras2(i1,i2,i3,kd)
      vraz22(i1,i2,i3,kd)=0
      vrax23(i1,i2,i3,kd)=rx(i1,i2,i3)*vrar2(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & vras2(i1,i2,i3,kd)+tx(i1,i2,i3)*vrat2(i1,i2,i3,kd)
      vray23(i1,i2,i3,kd)=ry(i1,i2,i3)*vrar2(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & vras2(i1,i2,i3,kd)+ty(i1,i2,i3)*vrat2(i1,i2,i3,kd)
      vraz23(i1,i2,i3,kd)=rz(i1,i2,i3)*vrar2(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & vras2(i1,i2,i3,kd)+tz(i1,i2,i3)*vrat2(i1,i2,i3,kd)
      vraxx21(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vrarr2(i1,i2,i3,kd)+(
     & rxx22(i1,i2,i3))*vrar2(i1,i2,i3,kd)
      vrayy21(i1,i2,i3,kd)=0
      vraxy21(i1,i2,i3,kd)=0
      vraxz21(i1,i2,i3,kd)=0
      vrayz21(i1,i2,i3,kd)=0
      vrazz21(i1,i2,i3,kd)=0
      vralaplacian21(i1,i2,i3,kd)=vraxx21(i1,i2,i3,kd)
      vraxx22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vrarr2(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*vrars2(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*vrass2(i1,i2,i3,kd)+(rxx22(i1,i2,i3))*vrar2(i1,i2,i3,kd)+(
     & sxx22(i1,i2,i3))*vras2(i1,i2,i3,kd)
      vrayy22(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*vrarr2(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*vrars2(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*vrass2(i1,i2,i3,kd)+(ryy22(i1,i2,i3))*vrar2(i1,i2,i3,kd)+(
     & syy22(i1,i2,i3))*vras2(i1,i2,i3,kd)
      vraxy22(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vrarr2(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & vrars2(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vrass2(i1,i2,i3,
     & kd)+rxy22(i1,i2,i3)*vrar2(i1,i2,i3,kd)+sxy22(i1,i2,i3)*vras2(
     & i1,i2,i3,kd)
      vraxz22(i1,i2,i3,kd)=0
      vrayz22(i1,i2,i3,kd)=0
      vrazz22(i1,i2,i3,kd)=0
      vralaplacian22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & vrarr2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)
     & *sy(i1,i2,i3))*vrars2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2)*vrass2(i1,i2,i3,kd)+(rxx22(i1,i2,i3)+ryy22(i1,i2,i3))*
     & vrar2(i1,i2,i3,kd)+(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*vras2(i1,
     & i2,i3,kd)
      vraxx23(i1,i2,i3,kd)=rx(i1,i2,i3)**2*vrarr2(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*vrass2(i1,i2,i3,kd)+tx(i1,i2,i3)**2*vratt2(i1,i2,i3,
     & kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*vrars2(i1,i2,i3,kd)+2.*rx(i1,
     & i2,i3)*tx(i1,i2,i3)*vrart2(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,
     & i2,i3)*vrast2(i1,i2,i3,kd)+rxx23(i1,i2,i3)*vrar2(i1,i2,i3,kd)+
     & sxx23(i1,i2,i3)*vras2(i1,i2,i3,kd)+txx23(i1,i2,i3)*vrat2(i1,i2,
     & i3,kd)
      vrayy23(i1,i2,i3,kd)=ry(i1,i2,i3)**2*vrarr2(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*vrass2(i1,i2,i3,kd)+ty(i1,i2,i3)**2*vratt2(i1,i2,i3,
     & kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*vrars2(i1,i2,i3,kd)+2.*ry(i1,
     & i2,i3)*ty(i1,i2,i3)*vrart2(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,
     & i2,i3)*vrast2(i1,i2,i3,kd)+ryy23(i1,i2,i3)*vrar2(i1,i2,i3,kd)+
     & syy23(i1,i2,i3)*vras2(i1,i2,i3,kd)+tyy23(i1,i2,i3)*vrat2(i1,i2,
     & i3,kd)
      vrazz23(i1,i2,i3,kd)=rz(i1,i2,i3)**2*vrarr2(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*vrass2(i1,i2,i3,kd)+tz(i1,i2,i3)**2*vratt2(i1,i2,i3,
     & kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*vrars2(i1,i2,i3,kd)+2.*rz(i1,
     & i2,i3)*tz(i1,i2,i3)*vrart2(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,
     & i2,i3)*vrast2(i1,i2,i3,kd)+rzz23(i1,i2,i3)*vrar2(i1,i2,i3,kd)+
     & szz23(i1,i2,i3)*vras2(i1,i2,i3,kd)+tzz23(i1,i2,i3)*vrat2(i1,i2,
     & i3,kd)
      vraxy23(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vrarr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vrass2(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & ty(i1,i2,i3)*vratt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(
     & i1,i2,i3)*sx(i1,i2,i3))*vrars2(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(
     & i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*vrart2(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*vrast2(i1,i2,
     & i3,kd)+rxy23(i1,i2,i3)*vrar2(i1,i2,i3,kd)+sxy23(i1,i2,i3)*
     & vras2(i1,i2,i3,kd)+txy23(i1,i2,i3)*vrat2(i1,i2,i3,kd)
      vraxz23(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*vrarr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*vrass2(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & tz(i1,i2,i3)*vratt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sx(i1,i2,i3))*vrars2(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*vrart2(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*vrast2(i1,i2,
     & i3,kd)+rxz23(i1,i2,i3)*vrar2(i1,i2,i3,kd)+sxz23(i1,i2,i3)*
     & vras2(i1,i2,i3,kd)+txz23(i1,i2,i3)*vrat2(i1,i2,i3,kd)
      vrayz23(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*vrarr2(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*vrass2(i1,i2,i3,kd)+ty(i1,i2,i3)*
     & tz(i1,i2,i3)*vratt2(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sy(i1,i2,i3))*vrars2(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*vrart2(i1,i2,i3,kd)+(sy(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*vrast2(i1,i2,
     & i3,kd)+ryz23(i1,i2,i3)*vrar2(i1,i2,i3,kd)+syz23(i1,i2,i3)*
     & vras2(i1,i2,i3,kd)+tyz23(i1,i2,i3)*vrat2(i1,i2,i3,kd)
      vralaplacian23(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*vrarr2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)
     & **2+sz(i1,i2,i3)**2)*vrass2(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(
     & i1,i2,i3)**2+tz(i1,i2,i3)**2)*vratt2(i1,i2,i3,kd)+2.*(rx(i1,i2,
     & i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,
     & i2,i3))*vrars2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(
     & i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*vrart2(i1,i2,
     & i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)
     & +sz(i1,i2,i3)*tz(i1,i2,i3))*vrast2(i1,i2,i3,kd)+(rxx23(i1,i2,
     & i3)+ryy23(i1,i2,i3)+rzz23(i1,i2,i3))*vrar2(i1,i2,i3,kd)+(sxx23(
     & i1,i2,i3)+syy23(i1,i2,i3)+szz23(i1,i2,i3))*vras2(i1,i2,i3,kd)+(
     & txx23(i1,i2,i3)+tyy23(i1,i2,i3)+tzz23(i1,i2,i3))*vrat2(i1,i2,
     & i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      vrax23r(i1,i2,i3,kd)=(vra(i1+1,i2,i3,kd)-vra(i1-1,i2,i3,kd))*h12(
     & 0)
      vray23r(i1,i2,i3,kd)=(vra(i1,i2+1,i3,kd)-vra(i1,i2-1,i3,kd))*h12(
     & 1)
      vraz23r(i1,i2,i3,kd)=(vra(i1,i2,i3+1,kd)-vra(i1,i2,i3-1,kd))*h12(
     & 2)
      vraxx23r(i1,i2,i3,kd)=(-2.*vra(i1,i2,i3,kd)+(vra(i1+1,i2,i3,kd)+
     & vra(i1-1,i2,i3,kd)) )*h22(0)
      vrayy23r(i1,i2,i3,kd)=(-2.*vra(i1,i2,i3,kd)+(vra(i1,i2+1,i3,kd)+
     & vra(i1,i2-1,i3,kd)) )*h22(1)
      vraxy23r(i1,i2,i3,kd)=(vrax23r(i1,i2+1,i3,kd)-vrax23r(i1,i2-1,i3,
     & kd))*h12(1)
      vrazz23r(i1,i2,i3,kd)=(-2.*vra(i1,i2,i3,kd)+(vra(i1,i2,i3+1,kd)+
     & vra(i1,i2,i3-1,kd)) )*h22(2)
      vraxz23r(i1,i2,i3,kd)=(vrax23r(i1,i2,i3+1,kd)-vrax23r(i1,i2,i3-1,
     & kd))*h12(2)
      vrayz23r(i1,i2,i3,kd)=(vray23r(i1,i2,i3+1,kd)-vray23r(i1,i2,i3-1,
     & kd))*h12(2)
      vrax21r(i1,i2,i3,kd)= vrax23r(i1,i2,i3,kd)
      vray21r(i1,i2,i3,kd)= vray23r(i1,i2,i3,kd)
      vraz21r(i1,i2,i3,kd)= vraz23r(i1,i2,i3,kd)
      vraxx21r(i1,i2,i3,kd)= vraxx23r(i1,i2,i3,kd)
      vrayy21r(i1,i2,i3,kd)= vrayy23r(i1,i2,i3,kd)
      vrazz21r(i1,i2,i3,kd)= vrazz23r(i1,i2,i3,kd)
      vraxy21r(i1,i2,i3,kd)= vraxy23r(i1,i2,i3,kd)
      vraxz21r(i1,i2,i3,kd)= vraxz23r(i1,i2,i3,kd)
      vrayz21r(i1,i2,i3,kd)= vrayz23r(i1,i2,i3,kd)
      vralaplacian21r(i1,i2,i3,kd)=vraxx23r(i1,i2,i3,kd)
      vrax22r(i1,i2,i3,kd)= vrax23r(i1,i2,i3,kd)
      vray22r(i1,i2,i3,kd)= vray23r(i1,i2,i3,kd)
      vraz22r(i1,i2,i3,kd)= vraz23r(i1,i2,i3,kd)
      vraxx22r(i1,i2,i3,kd)= vraxx23r(i1,i2,i3,kd)
      vrayy22r(i1,i2,i3,kd)= vrayy23r(i1,i2,i3,kd)
      vrazz22r(i1,i2,i3,kd)= vrazz23r(i1,i2,i3,kd)
      vraxy22r(i1,i2,i3,kd)= vraxy23r(i1,i2,i3,kd)
      vraxz22r(i1,i2,i3,kd)= vraxz23r(i1,i2,i3,kd)
      vrayz22r(i1,i2,i3,kd)= vrayz23r(i1,i2,i3,kd)
      vralaplacian22r(i1,i2,i3,kd)=vraxx23r(i1,i2,i3,kd)+vrayy23r(i1,
     & i2,i3,kd)
      vralaplacian23r(i1,i2,i3,kd)=vraxx23r(i1,i2,i3,kd)+vrayy23r(i1,
     & i2,i3,kd)+vrazz23r(i1,i2,i3,kd)
      vraxxx22r(i1,i2,i3,kd)=(-2.*(vra(i1+1,i2,i3,kd)-vra(i1-1,i2,i3,
     & kd))+(vra(i1+2,i2,i3,kd)-vra(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
      vrayyy22r(i1,i2,i3,kd)=(-2.*(vra(i1,i2+1,i3,kd)-vra(i1,i2-1,i3,
     & kd))+(vra(i1,i2+2,i3,kd)-vra(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
      vraxxy22r(i1,i2,i3,kd)=( vraxx22r(i1,i2+1,i3,kd)-vraxx22r(i1,i2-
     & 1,i3,kd))/(2.*dx(1))
      vraxyy22r(i1,i2,i3,kd)=( vrayy22r(i1+1,i2,i3,kd)-vrayy22r(i1-1,
     & i2,i3,kd))/(2.*dx(0))
      vraxxxx22r(i1,i2,i3,kd)=(6.*vra(i1,i2,i3,kd)-4.*(vra(i1+1,i2,i3,
     & kd)+vra(i1-1,i2,i3,kd))+(vra(i1+2,i2,i3,kd)+vra(i1-2,i2,i3,kd))
     &  )/(dx(0)**4)
      vrayyyy22r(i1,i2,i3,kd)=(6.*vra(i1,i2,i3,kd)-4.*(vra(i1,i2+1,i3,
     & kd)+vra(i1,i2-1,i3,kd))+(vra(i1,i2+2,i3,kd)+vra(i1,i2-2,i3,kd))
     &  )/(dx(1)**4)
      vraxxyy22r(i1,i2,i3,kd)=( 4.*vra(i1,i2,i3,kd)     -2.*(vra(i1+1,
     & i2,i3,kd)+vra(i1-1,i2,i3,kd)+vra(i1,i2+1,i3,kd)+vra(i1,i2-1,i3,
     & kd))   +   (vra(i1+1,i2+1,i3,kd)+vra(i1-1,i2+1,i3,kd)+vra(i1+1,
     & i2-1,i3,kd)+vra(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      ! 2D laplacian squared = vra.xxxx + 2 vra.xxyy + vra.yyyy
      vraLapSq22r(i1,i2,i3,kd)= ( 6.*vra(i1,i2,i3,kd)   - 4.*(vra(i1+1,
     & i2,i3,kd)+vra(i1-1,i2,i3,kd))    +(vra(i1+2,i2,i3,kd)+vra(i1-2,
     & i2,i3,kd)) )/(dx(0)**4) +( 6.*vra(i1,i2,i3,kd)    -4.*(vra(i1,
     & i2+1,i3,kd)+vra(i1,i2-1,i3,kd))    +(vra(i1,i2+2,i3,kd)+vra(i1,
     & i2-2,i3,kd)) )/(dx(1)**4)  +( 8.*vra(i1,i2,i3,kd)     -4.*(vra(
     & i1+1,i2,i3,kd)+vra(i1-1,i2,i3,kd)+vra(i1,i2+1,i3,kd)+vra(i1,i2-
     & 1,i3,kd))   +2.*(vra(i1+1,i2+1,i3,kd)+vra(i1-1,i2+1,i3,kd)+vra(
     & i1+1,i2-1,i3,kd)+vra(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      vraxxx23r(i1,i2,i3,kd)=(-2.*(vra(i1+1,i2,i3,kd)-vra(i1-1,i2,i3,
     & kd))+(vra(i1+2,i2,i3,kd)-vra(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
      vrayyy23r(i1,i2,i3,kd)=(-2.*(vra(i1,i2+1,i3,kd)-vra(i1,i2-1,i3,
     & kd))+(vra(i1,i2+2,i3,kd)-vra(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
      vrazzz23r(i1,i2,i3,kd)=(-2.*(vra(i1,i2,i3+1,kd)-vra(i1,i2,i3-1,
     & kd))+(vra(i1,i2,i3+2,kd)-vra(i1,i2,i3-2,kd)) )*h22(1)*h12(2)
      vraxxy23r(i1,i2,i3,kd)=( vraxx22r(i1,i2+1,i3,kd)-vraxx22r(i1,i2-
     & 1,i3,kd))/(2.*dx(1))
      vraxyy23r(i1,i2,i3,kd)=( vrayy22r(i1+1,i2,i3,kd)-vrayy22r(i1-1,
     & i2,i3,kd))/(2.*dx(0))
      vraxxz23r(i1,i2,i3,kd)=( vraxx22r(i1,i2,i3+1,kd)-vraxx22r(i1,i2,
     & i3-1,kd))/(2.*dx(2))
      vrayyz23r(i1,i2,i3,kd)=( vrayy22r(i1,i2,i3+1,kd)-vrayy22r(i1,i2,
     & i3-1,kd))/(2.*dx(2))
      vraxzz23r(i1,i2,i3,kd)=( vrazz22r(i1+1,i2,i3,kd)-vrazz22r(i1-1,
     & i2,i3,kd))/(2.*dx(0))
      vrayzz23r(i1,i2,i3,kd)=( vrazz22r(i1,i2+1,i3,kd)-vrazz22r(i1,i2-
     & 1,i3,kd))/(2.*dx(1))
      vraxxxx23r(i1,i2,i3,kd)=(6.*vra(i1,i2,i3,kd)-4.*(vra(i1+1,i2,i3,
     & kd)+vra(i1-1,i2,i3,kd))+(vra(i1+2,i2,i3,kd)+vra(i1-2,i2,i3,kd))
     &  )/(dx(0)**4)
      vrayyyy23r(i1,i2,i3,kd)=(6.*vra(i1,i2,i3,kd)-4.*(vra(i1,i2+1,i3,
     & kd)+vra(i1,i2-1,i3,kd))+(vra(i1,i2+2,i3,kd)+vra(i1,i2-2,i3,kd))
     &  )/(dx(1)**4)
      vrazzzz23r(i1,i2,i3,kd)=(6.*vra(i1,i2,i3,kd)-4.*(vra(i1,i2,i3+1,
     & kd)+vra(i1,i2,i3-1,kd))+(vra(i1,i2,i3+2,kd)+vra(i1,i2,i3-2,kd))
     &  )/(dx(2)**4)
      vraxxyy23r(i1,i2,i3,kd)=( 4.*vra(i1,i2,i3,kd)     -2.*(vra(i1+1,
     & i2,i3,kd)+vra(i1-1,i2,i3,kd)+vra(i1,i2+1,i3,kd)+vra(i1,i2-1,i3,
     & kd))   +   (vra(i1+1,i2+1,i3,kd)+vra(i1-1,i2+1,i3,kd)+vra(i1+1,
     & i2-1,i3,kd)+vra(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      vraxxzz23r(i1,i2,i3,kd)=( 4.*vra(i1,i2,i3,kd)     -2.*(vra(i1+1,
     & i2,i3,kd)+vra(i1-1,i2,i3,kd)+vra(i1,i2,i3+1,kd)+vra(i1,i2,i3-1,
     & kd))   +   (vra(i1+1,i2,i3+1,kd)+vra(i1-1,i2,i3+1,kd)+vra(i1+1,
     & i2,i3-1,kd)+vra(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)
      vrayyzz23r(i1,i2,i3,kd)=( 4.*vra(i1,i2,i3,kd)     -2.*(vra(i1,i2+
     & 1,i3,kd)  +vra(i1,i2-1,i3,kd)+  vra(i1,i2  ,i3+1,kd)+vra(i1,i2 
     &  ,i3-1,kd))   +   (vra(i1,i2+1,i3+1,kd)+vra(i1,i2-1,i3+1,kd)+
     & vra(i1,i2+1,i3-1,kd)+vra(i1,i2-1,i3-1,kd)) )/(dx(1)**2*dx(2)**
     & 2)
      ! 3D laplacian squared = vra.xxxx + vra.yyyy + vra.zzzz + 2 (vra.xxyy + vra.xxzz + vra.yyzz )
      vraLapSq23r(i1,i2,i3,kd)= ( 6.*vra(i1,i2,i3,kd)   - 4.*(vra(i1+1,
     & i2,i3,kd)+vra(i1-1,i2,i3,kd))    +(vra(i1+2,i2,i3,kd)+vra(i1-2,
     & i2,i3,kd)) )/(dx(0)**4) +( 6.*vra(i1,i2,i3,kd)    -4.*(vra(i1,
     & i2+1,i3,kd)+vra(i1,i2-1,i3,kd))    +(vra(i1,i2+2,i3,kd)+vra(i1,
     & i2-2,i3,kd)) )/(dx(1)**4)  +( 6.*vra(i1,i2,i3,kd)    -4.*(vra(
     & i1,i2,i3+1,kd)+vra(i1,i2,i3-1,kd))    +(vra(i1,i2,i3+2,kd)+vra(
     & i1,i2,i3-2,kd)) )/(dx(2)**4)  +( 8.*vra(i1,i2,i3,kd)     -4.*(
     & vra(i1+1,i2,i3,kd)  +vra(i1-1,i2,i3,kd)  +vra(i1  ,i2+1,i3,kd)+
     & vra(i1  ,i2-1,i3,kd))   +2.*(vra(i1+1,i2+1,i3,kd)+vra(i1-1,i2+
     & 1,i3,kd)+vra(i1+1,i2-1,i3,kd)+vra(i1-1,i2-1,i3,kd)) )/(dx(0)**
     & 2*dx(1)**2)+( 8.*vra(i1,i2,i3,kd)     -4.*(vra(i1+1,i2,i3,kd)  
     & +vra(i1-1,i2,i3,kd)  +vra(i1  ,i2,i3+1,kd)+vra(i1  ,i2,i3-1,kd)
     & )   +2.*(vra(i1+1,i2,i3+1,kd)+vra(i1-1,i2,i3+1,kd)+vra(i1+1,i2,
     & i3-1,kd)+vra(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)+( 8.*vra(
     & i1,i2,i3,kd)     -4.*(vra(i1,i2+1,i3,kd)  +vra(i1,i2-1,i3,kd)  
     & +vra(i1,i2  ,i3+1,kd)+vra(i1,i2  ,i3-1,kd))   +2.*(vra(i1,i2+1,
     & i3+1,kd)+vra(i1,i2-1,i3+1,kd)+vra(i1,i2+1,i3-1,kd)+vra(i1,i2-1,
     & i3-1,kd)) )/(dx(1)**2*dx(2)**2)
      vramr2(i1,i2,i3,kd)=(vram(i1+1,i2,i3,kd)-vram(i1-1,i2,i3,kd))*
     & d12(0)
      vrams2(i1,i2,i3,kd)=(vram(i1,i2+1,i3,kd)-vram(i1,i2-1,i3,kd))*
     & d12(1)
      vramt2(i1,i2,i3,kd)=(vram(i1,i2,i3+1,kd)-vram(i1,i2,i3-1,kd))*
     & d12(2)
      vramrr2(i1,i2,i3,kd)=(-2.*vram(i1,i2,i3,kd)+(vram(i1+1,i2,i3,kd)+
     & vram(i1-1,i2,i3,kd)) )*d22(0)
      vramss2(i1,i2,i3,kd)=(-2.*vram(i1,i2,i3,kd)+(vram(i1,i2+1,i3,kd)+
     & vram(i1,i2-1,i3,kd)) )*d22(1)
      vramrs2(i1,i2,i3,kd)=(vramr2(i1,i2+1,i3,kd)-vramr2(i1,i2-1,i3,kd)
     & )*d12(1)
      vramtt2(i1,i2,i3,kd)=(-2.*vram(i1,i2,i3,kd)+(vram(i1,i2,i3+1,kd)+
     & vram(i1,i2,i3-1,kd)) )*d22(2)
      vramrt2(i1,i2,i3,kd)=(vramr2(i1,i2,i3+1,kd)-vramr2(i1,i2,i3-1,kd)
     & )*d12(2)
      vramst2(i1,i2,i3,kd)=(vrams2(i1,i2,i3+1,kd)-vrams2(i1,i2,i3-1,kd)
     & )*d12(2)
      vramrrr2(i1,i2,i3,kd)=(-2.*(vram(i1+1,i2,i3,kd)-vram(i1-1,i2,i3,
     & kd))+(vram(i1+2,i2,i3,kd)-vram(i1-2,i2,i3,kd)) )*d22(0)*d12(0)
      vramsss2(i1,i2,i3,kd)=(-2.*(vram(i1,i2+1,i3,kd)-vram(i1,i2-1,i3,
     & kd))+(vram(i1,i2+2,i3,kd)-vram(i1,i2-2,i3,kd)) )*d22(1)*d12(1)
      vramttt2(i1,i2,i3,kd)=(-2.*(vram(i1,i2,i3+1,kd)-vram(i1,i2,i3-1,
     & kd))+(vram(i1,i2,i3+2,kd)-vram(i1,i2,i3-2,kd)) )*d22(2)*d12(2)
      vramx21(i1,i2,i3,kd)= rx(i1,i2,i3)*vramr2(i1,i2,i3,kd)
      vramy21(i1,i2,i3,kd)=0
      vramz21(i1,i2,i3,kd)=0
      vramx22(i1,i2,i3,kd)= rx(i1,i2,i3)*vramr2(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*vrams2(i1,i2,i3,kd)
      vramy22(i1,i2,i3,kd)= ry(i1,i2,i3)*vramr2(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*vrams2(i1,i2,i3,kd)
      vramz22(i1,i2,i3,kd)=0
      vramx23(i1,i2,i3,kd)=rx(i1,i2,i3)*vramr2(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*vrams2(i1,i2,i3,kd)+tx(i1,i2,i3)*vramt2(i1,i2,i3,kd)
      vramy23(i1,i2,i3,kd)=ry(i1,i2,i3)*vramr2(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*vrams2(i1,i2,i3,kd)+ty(i1,i2,i3)*vramt2(i1,i2,i3,kd)
      vramz23(i1,i2,i3,kd)=rz(i1,i2,i3)*vramr2(i1,i2,i3,kd)+sz(i1,i2,
     & i3)*vrams2(i1,i2,i3,kd)+tz(i1,i2,i3)*vramt2(i1,i2,i3,kd)
      vramxx21(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vramrr2(i1,i2,i3,kd)+(
     & rxx22(i1,i2,i3))*vramr2(i1,i2,i3,kd)
      vramyy21(i1,i2,i3,kd)=0
      vramxy21(i1,i2,i3,kd)=0
      vramxz21(i1,i2,i3,kd)=0
      vramyz21(i1,i2,i3,kd)=0
      vramzz21(i1,i2,i3,kd)=0
      vramlaplacian21(i1,i2,i3,kd)=vramxx21(i1,i2,i3,kd)
      vramxx22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vramrr2(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*vramrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*vramss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3))*vramr2(i1,i2,i3,kd)+(
     & sxx22(i1,i2,i3))*vrams2(i1,i2,i3,kd)
      vramyy22(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*vramrr2(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*vramrs2(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*vramss2(i1,i2,i3,kd)+(ryy22(i1,i2,i3))*vramr2(i1,i2,i3,kd)+(
     & syy22(i1,i2,i3))*vrams2(i1,i2,i3,kd)
      vramxy22(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vramrr2(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & vramrs2(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vramss2(i1,i2,
     & i3,kd)+rxy22(i1,i2,i3)*vramr2(i1,i2,i3,kd)+sxy22(i1,i2,i3)*
     & vrams2(i1,i2,i3,kd)
      vramxz22(i1,i2,i3,kd)=0
      vramyz22(i1,i2,i3,kd)=0
      vramzz22(i1,i2,i3,kd)=0
      vramlaplacian22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & vramrr2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,
     & i3)*sy(i1,i2,i3))*vramrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,
     & i2,i3)**2)*vramss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3)+ryy22(i1,i2,
     & i3))*vramr2(i1,i2,i3,kd)+(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*
     & vrams2(i1,i2,i3,kd)
      vramxx23(i1,i2,i3,kd)=rx(i1,i2,i3)**2*vramrr2(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*vramss2(i1,i2,i3,kd)+tx(i1,i2,i3)**2*vramtt2(i1,i2,
     & i3,kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*vramrs2(i1,i2,i3,kd)+2.*rx(
     & i1,i2,i3)*tx(i1,i2,i3)*vramrt2(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(
     & i1,i2,i3)*vramst2(i1,i2,i3,kd)+rxx23(i1,i2,i3)*vramr2(i1,i2,i3,
     & kd)+sxx23(i1,i2,i3)*vrams2(i1,i2,i3,kd)+txx23(i1,i2,i3)*vramt2(
     & i1,i2,i3,kd)
      vramyy23(i1,i2,i3,kd)=ry(i1,i2,i3)**2*vramrr2(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*vramss2(i1,i2,i3,kd)+ty(i1,i2,i3)**2*vramtt2(i1,i2,
     & i3,kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*vramrs2(i1,i2,i3,kd)+2.*ry(
     & i1,i2,i3)*ty(i1,i2,i3)*vramrt2(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(
     & i1,i2,i3)*vramst2(i1,i2,i3,kd)+ryy23(i1,i2,i3)*vramr2(i1,i2,i3,
     & kd)+syy23(i1,i2,i3)*vrams2(i1,i2,i3,kd)+tyy23(i1,i2,i3)*vramt2(
     & i1,i2,i3,kd)
      vramzz23(i1,i2,i3,kd)=rz(i1,i2,i3)**2*vramrr2(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*vramss2(i1,i2,i3,kd)+tz(i1,i2,i3)**2*vramtt2(i1,i2,
     & i3,kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*vramrs2(i1,i2,i3,kd)+2.*rz(
     & i1,i2,i3)*tz(i1,i2,i3)*vramrt2(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(
     & i1,i2,i3)*vramst2(i1,i2,i3,kd)+rzz23(i1,i2,i3)*vramr2(i1,i2,i3,
     & kd)+szz23(i1,i2,i3)*vrams2(i1,i2,i3,kd)+tzz23(i1,i2,i3)*vramt2(
     & i1,i2,i3,kd)
      vramxy23(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vramrr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vramss2(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *ty(i1,i2,i3)*vramtt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+
     & ry(i1,i2,i3)*sx(i1,i2,i3))*vramrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & ty(i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*vramrt2(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*vramst2(
     & i1,i2,i3,kd)+rxy23(i1,i2,i3)*vramr2(i1,i2,i3,kd)+sxy23(i1,i2,
     & i3)*vrams2(i1,i2,i3,kd)+txy23(i1,i2,i3)*vramt2(i1,i2,i3,kd)
      vramxz23(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*vramrr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*vramss2(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *tz(i1,i2,i3)*vramtt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sx(i1,i2,i3))*vramrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*vramrt2(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*vramst2(
     & i1,i2,i3,kd)+rxz23(i1,i2,i3)*vramr2(i1,i2,i3,kd)+sxz23(i1,i2,
     & i3)*vrams2(i1,i2,i3,kd)+txz23(i1,i2,i3)*vramt2(i1,i2,i3,kd)
      vramyz23(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*vramrr2(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*vramss2(i1,i2,i3,kd)+ty(i1,i2,i3)
     & *tz(i1,i2,i3)*vramtt2(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sy(i1,i2,i3))*vramrs2(i1,i2,i3,kd)+(ry(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*vramrt2(i1,i2,i3,kd)+(
     & sy(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*vramst2(
     & i1,i2,i3,kd)+ryz23(i1,i2,i3)*vramr2(i1,i2,i3,kd)+syz23(i1,i2,
     & i3)*vrams2(i1,i2,i3,kd)+tyz23(i1,i2,i3)*vramt2(i1,i2,i3,kd)
      vramlaplacian23(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*vramrr2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2+sz(i1,i2,i3)**2)*vramss2(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+
     & ty(i1,i2,i3)**2+tz(i1,i2,i3)**2)*vramtt2(i1,i2,i3,kd)+2.*(rx(
     & i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*
     & sz(i1,i2,i3))*vramrs2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,
     & i3)+ ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*
     & vramrt2(i1,i2,i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,
     & i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3))*vramst2(i1,i2,i3,
     & kd)+(rxx23(i1,i2,i3)+ryy23(i1,i2,i3)+rzz23(i1,i2,i3))*vramr2(
     & i1,i2,i3,kd)+(sxx23(i1,i2,i3)+syy23(i1,i2,i3)+szz23(i1,i2,i3))*
     & vrams2(i1,i2,i3,kd)+(txx23(i1,i2,i3)+tyy23(i1,i2,i3)+tzz23(i1,
     & i2,i3))*vramt2(i1,i2,i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      vramx23r(i1,i2,i3,kd)=(vram(i1+1,i2,i3,kd)-vram(i1-1,i2,i3,kd))*
     & h12(0)
      vramy23r(i1,i2,i3,kd)=(vram(i1,i2+1,i3,kd)-vram(i1,i2-1,i3,kd))*
     & h12(1)
      vramz23r(i1,i2,i3,kd)=(vram(i1,i2,i3+1,kd)-vram(i1,i2,i3-1,kd))*
     & h12(2)
      vramxx23r(i1,i2,i3,kd)=(-2.*vram(i1,i2,i3,kd)+(vram(i1+1,i2,i3,
     & kd)+vram(i1-1,i2,i3,kd)) )*h22(0)
      vramyy23r(i1,i2,i3,kd)=(-2.*vram(i1,i2,i3,kd)+(vram(i1,i2+1,i3,
     & kd)+vram(i1,i2-1,i3,kd)) )*h22(1)
      vramxy23r(i1,i2,i3,kd)=(vramx23r(i1,i2+1,i3,kd)-vramx23r(i1,i2-1,
     & i3,kd))*h12(1)
      vramzz23r(i1,i2,i3,kd)=(-2.*vram(i1,i2,i3,kd)+(vram(i1,i2,i3+1,
     & kd)+vram(i1,i2,i3-1,kd)) )*h22(2)
      vramxz23r(i1,i2,i3,kd)=(vramx23r(i1,i2,i3+1,kd)-vramx23r(i1,i2,
     & i3-1,kd))*h12(2)
      vramyz23r(i1,i2,i3,kd)=(vramy23r(i1,i2,i3+1,kd)-vramy23r(i1,i2,
     & i3-1,kd))*h12(2)
      vramx21r(i1,i2,i3,kd)= vramx23r(i1,i2,i3,kd)
      vramy21r(i1,i2,i3,kd)= vramy23r(i1,i2,i3,kd)
      vramz21r(i1,i2,i3,kd)= vramz23r(i1,i2,i3,kd)
      vramxx21r(i1,i2,i3,kd)= vramxx23r(i1,i2,i3,kd)
      vramyy21r(i1,i2,i3,kd)= vramyy23r(i1,i2,i3,kd)
      vramzz21r(i1,i2,i3,kd)= vramzz23r(i1,i2,i3,kd)
      vramxy21r(i1,i2,i3,kd)= vramxy23r(i1,i2,i3,kd)
      vramxz21r(i1,i2,i3,kd)= vramxz23r(i1,i2,i3,kd)
      vramyz21r(i1,i2,i3,kd)= vramyz23r(i1,i2,i3,kd)
      vramlaplacian21r(i1,i2,i3,kd)=vramxx23r(i1,i2,i3,kd)
      vramx22r(i1,i2,i3,kd)= vramx23r(i1,i2,i3,kd)
      vramy22r(i1,i2,i3,kd)= vramy23r(i1,i2,i3,kd)
      vramz22r(i1,i2,i3,kd)= vramz23r(i1,i2,i3,kd)
      vramxx22r(i1,i2,i3,kd)= vramxx23r(i1,i2,i3,kd)
      vramyy22r(i1,i2,i3,kd)= vramyy23r(i1,i2,i3,kd)
      vramzz22r(i1,i2,i3,kd)= vramzz23r(i1,i2,i3,kd)
      vramxy22r(i1,i2,i3,kd)= vramxy23r(i1,i2,i3,kd)
      vramxz22r(i1,i2,i3,kd)= vramxz23r(i1,i2,i3,kd)
      vramyz22r(i1,i2,i3,kd)= vramyz23r(i1,i2,i3,kd)
      vramlaplacian22r(i1,i2,i3,kd)=vramxx23r(i1,i2,i3,kd)+vramyy23r(
     & i1,i2,i3,kd)
      vramlaplacian23r(i1,i2,i3,kd)=vramxx23r(i1,i2,i3,kd)+vramyy23r(
     & i1,i2,i3,kd)+vramzz23r(i1,i2,i3,kd)
      vramxxx22r(i1,i2,i3,kd)=(-2.*(vram(i1+1,i2,i3,kd)-vram(i1-1,i2,
     & i3,kd))+(vram(i1+2,i2,i3,kd)-vram(i1-2,i2,i3,kd)) )*h22(0)*h12(
     & 0)
      vramyyy22r(i1,i2,i3,kd)=(-2.*(vram(i1,i2+1,i3,kd)-vram(i1,i2-1,
     & i3,kd))+(vram(i1,i2+2,i3,kd)-vram(i1,i2-2,i3,kd)) )*h22(1)*h12(
     & 1)
      vramxxy22r(i1,i2,i3,kd)=( vramxx22r(i1,i2+1,i3,kd)-vramxx22r(i1,
     & i2-1,i3,kd))/(2.*dx(1))
      vramxyy22r(i1,i2,i3,kd)=( vramyy22r(i1+1,i2,i3,kd)-vramyy22r(i1-
     & 1,i2,i3,kd))/(2.*dx(0))
      vramxxxx22r(i1,i2,i3,kd)=(6.*vram(i1,i2,i3,kd)-4.*(vram(i1+1,i2,
     & i3,kd)+vram(i1-1,i2,i3,kd))+(vram(i1+2,i2,i3,kd)+vram(i1-2,i2,
     & i3,kd)) )/(dx(0)**4)
      vramyyyy22r(i1,i2,i3,kd)=(6.*vram(i1,i2,i3,kd)-4.*(vram(i1,i2+1,
     & i3,kd)+vram(i1,i2-1,i3,kd))+(vram(i1,i2+2,i3,kd)+vram(i1,i2-2,
     & i3,kd)) )/(dx(1)**4)
      vramxxyy22r(i1,i2,i3,kd)=( 4.*vram(i1,i2,i3,kd)     -2.*(vram(i1+
     & 1,i2,i3,kd)+vram(i1-1,i2,i3,kd)+vram(i1,i2+1,i3,kd)+vram(i1,i2-
     & 1,i3,kd))   +   (vram(i1+1,i2+1,i3,kd)+vram(i1-1,i2+1,i3,kd)+
     & vram(i1+1,i2-1,i3,kd)+vram(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)*
     & *2)
      ! 2D laplacian squared = vram.xxxx + 2 vram.xxyy + vram.yyyy
      vramLapSq22r(i1,i2,i3,kd)= ( 6.*vram(i1,i2,i3,kd)   - 4.*(vram(
     & i1+1,i2,i3,kd)+vram(i1-1,i2,i3,kd))    +(vram(i1+2,i2,i3,kd)+
     & vram(i1-2,i2,i3,kd)) )/(dx(0)**4) +( 6.*vram(i1,i2,i3,kd)    -
     & 4.*(vram(i1,i2+1,i3,kd)+vram(i1,i2-1,i3,kd))    +(vram(i1,i2+2,
     & i3,kd)+vram(i1,i2-2,i3,kd)) )/(dx(1)**4)  +( 8.*vram(i1,i2,i3,
     & kd)     -4.*(vram(i1+1,i2,i3,kd)+vram(i1-1,i2,i3,kd)+vram(i1,
     & i2+1,i3,kd)+vram(i1,i2-1,i3,kd))   +2.*(vram(i1+1,i2+1,i3,kd)+
     & vram(i1-1,i2+1,i3,kd)+vram(i1+1,i2-1,i3,kd)+vram(i1-1,i2-1,i3,
     & kd)) )/(dx(0)**2*dx(1)**2)
      vramxxx23r(i1,i2,i3,kd)=(-2.*(vram(i1+1,i2,i3,kd)-vram(i1-1,i2,
     & i3,kd))+(vram(i1+2,i2,i3,kd)-vram(i1-2,i2,i3,kd)) )*h22(0)*h12(
     & 0)
      vramyyy23r(i1,i2,i3,kd)=(-2.*(vram(i1,i2+1,i3,kd)-vram(i1,i2-1,
     & i3,kd))+(vram(i1,i2+2,i3,kd)-vram(i1,i2-2,i3,kd)) )*h22(1)*h12(
     & 1)
      vramzzz23r(i1,i2,i3,kd)=(-2.*(vram(i1,i2,i3+1,kd)-vram(i1,i2,i3-
     & 1,kd))+(vram(i1,i2,i3+2,kd)-vram(i1,i2,i3-2,kd)) )*h22(1)*h12(
     & 2)
      vramxxy23r(i1,i2,i3,kd)=( vramxx22r(i1,i2+1,i3,kd)-vramxx22r(i1,
     & i2-1,i3,kd))/(2.*dx(1))
      vramxyy23r(i1,i2,i3,kd)=( vramyy22r(i1+1,i2,i3,kd)-vramyy22r(i1-
     & 1,i2,i3,kd))/(2.*dx(0))
      vramxxz23r(i1,i2,i3,kd)=( vramxx22r(i1,i2,i3+1,kd)-vramxx22r(i1,
     & i2,i3-1,kd))/(2.*dx(2))
      vramyyz23r(i1,i2,i3,kd)=( vramyy22r(i1,i2,i3+1,kd)-vramyy22r(i1,
     & i2,i3-1,kd))/(2.*dx(2))
      vramxzz23r(i1,i2,i3,kd)=( vramzz22r(i1+1,i2,i3,kd)-vramzz22r(i1-
     & 1,i2,i3,kd))/(2.*dx(0))
      vramyzz23r(i1,i2,i3,kd)=( vramzz22r(i1,i2+1,i3,kd)-vramzz22r(i1,
     & i2-1,i3,kd))/(2.*dx(1))
      vramxxxx23r(i1,i2,i3,kd)=(6.*vram(i1,i2,i3,kd)-4.*(vram(i1+1,i2,
     & i3,kd)+vram(i1-1,i2,i3,kd))+(vram(i1+2,i2,i3,kd)+vram(i1-2,i2,
     & i3,kd)) )/(dx(0)**4)
      vramyyyy23r(i1,i2,i3,kd)=(6.*vram(i1,i2,i3,kd)-4.*(vram(i1,i2+1,
     & i3,kd)+vram(i1,i2-1,i3,kd))+(vram(i1,i2+2,i3,kd)+vram(i1,i2-2,
     & i3,kd)) )/(dx(1)**4)
      vramzzzz23r(i1,i2,i3,kd)=(6.*vram(i1,i2,i3,kd)-4.*(vram(i1,i2,i3+
     & 1,kd)+vram(i1,i2,i3-1,kd))+(vram(i1,i2,i3+2,kd)+vram(i1,i2,i3-
     & 2,kd)) )/(dx(2)**4)
      vramxxyy23r(i1,i2,i3,kd)=( 4.*vram(i1,i2,i3,kd)     -2.*(vram(i1+
     & 1,i2,i3,kd)+vram(i1-1,i2,i3,kd)+vram(i1,i2+1,i3,kd)+vram(i1,i2-
     & 1,i3,kd))   +   (vram(i1+1,i2+1,i3,kd)+vram(i1-1,i2+1,i3,kd)+
     & vram(i1+1,i2-1,i3,kd)+vram(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)*
     & *2)
      vramxxzz23r(i1,i2,i3,kd)=( 4.*vram(i1,i2,i3,kd)     -2.*(vram(i1+
     & 1,i2,i3,kd)+vram(i1-1,i2,i3,kd)+vram(i1,i2,i3+1,kd)+vram(i1,i2,
     & i3-1,kd))   +   (vram(i1+1,i2,i3+1,kd)+vram(i1-1,i2,i3+1,kd)+
     & vram(i1+1,i2,i3-1,kd)+vram(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)*
     & *2)
      vramyyzz23r(i1,i2,i3,kd)=( 4.*vram(i1,i2,i3,kd)     -2.*(vram(i1,
     & i2+1,i3,kd)  +vram(i1,i2-1,i3,kd)+  vram(i1,i2  ,i3+1,kd)+vram(
     & i1,i2  ,i3-1,kd))   +   (vram(i1,i2+1,i3+1,kd)+vram(i1,i2-1,i3+
     & 1,kd)+vram(i1,i2+1,i3-1,kd)+vram(i1,i2-1,i3-1,kd)) )/(dx(1)**2*
     & dx(2)**2)
      ! 3D laplacian squared = vram.xxxx + vram.yyyy + vram.zzzz + 2 (vram.xxyy + vram.xxzz + vram.yyzz )
      vramLapSq23r(i1,i2,i3,kd)= ( 6.*vram(i1,i2,i3,kd)   - 4.*(vram(
     & i1+1,i2,i3,kd)+vram(i1-1,i2,i3,kd))    +(vram(i1+2,i2,i3,kd)+
     & vram(i1-2,i2,i3,kd)) )/(dx(0)**4) +( 6.*vram(i1,i2,i3,kd)    -
     & 4.*(vram(i1,i2+1,i3,kd)+vram(i1,i2-1,i3,kd))    +(vram(i1,i2+2,
     & i3,kd)+vram(i1,i2-2,i3,kd)) )/(dx(1)**4)  +( 6.*vram(i1,i2,i3,
     & kd)    -4.*(vram(i1,i2,i3+1,kd)+vram(i1,i2,i3-1,kd))    +(vram(
     & i1,i2,i3+2,kd)+vram(i1,i2,i3-2,kd)) )/(dx(2)**4)  +( 8.*vram(
     & i1,i2,i3,kd)     -4.*(vram(i1+1,i2,i3,kd)  +vram(i1-1,i2,i3,kd)
     &   +vram(i1  ,i2+1,i3,kd)+vram(i1  ,i2-1,i3,kd))   +2.*(vram(i1+
     & 1,i2+1,i3,kd)+vram(i1-1,i2+1,i3,kd)+vram(i1+1,i2-1,i3,kd)+vram(
     & i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)+( 8.*vram(i1,i2,i3,kd) 
     &     -4.*(vram(i1+1,i2,i3,kd)  +vram(i1-1,i2,i3,kd)  +vram(i1  ,
     & i2,i3+1,kd)+vram(i1  ,i2,i3-1,kd))   +2.*(vram(i1+1,i2,i3+1,kd)
     & +vram(i1-1,i2,i3+1,kd)+vram(i1+1,i2,i3-1,kd)+vram(i1-1,i2,i3-1,
     & kd)) )/(dx(0)**2*dx(2)**2)+( 8.*vram(i1,i2,i3,kd)     -4.*(
     & vram(i1,i2+1,i3,kd)  +vram(i1,i2-1,i3,kd)  +vram(i1,i2  ,i3+1,
     & kd)+vram(i1,i2  ,i3-1,kd))   +2.*(vram(i1,i2+1,i3+1,kd)+vram(
     & i1,i2-1,i3+1,kd)+vram(i1,i2+1,i3-1,kd)+vram(i1,i2-1,i3-1,kd)) )
     & /(dx(1)**2*dx(2)**2)
      wrar2(i1,i2,i3,kd)=(wra(i1+1,i2,i3,kd)-wra(i1-1,i2,i3,kd))*d12(0)
      wras2(i1,i2,i3,kd)=(wra(i1,i2+1,i3,kd)-wra(i1,i2-1,i3,kd))*d12(1)
      wrat2(i1,i2,i3,kd)=(wra(i1,i2,i3+1,kd)-wra(i1,i2,i3-1,kd))*d12(2)
      wrarr2(i1,i2,i3,kd)=(-2.*wra(i1,i2,i3,kd)+(wra(i1+1,i2,i3,kd)+
     & wra(i1-1,i2,i3,kd)) )*d22(0)
      wrass2(i1,i2,i3,kd)=(-2.*wra(i1,i2,i3,kd)+(wra(i1,i2+1,i3,kd)+
     & wra(i1,i2-1,i3,kd)) )*d22(1)
      wrars2(i1,i2,i3,kd)=(wrar2(i1,i2+1,i3,kd)-wrar2(i1,i2-1,i3,kd))*
     & d12(1)
      wratt2(i1,i2,i3,kd)=(-2.*wra(i1,i2,i3,kd)+(wra(i1,i2,i3+1,kd)+
     & wra(i1,i2,i3-1,kd)) )*d22(2)
      wrart2(i1,i2,i3,kd)=(wrar2(i1,i2,i3+1,kd)-wrar2(i1,i2,i3-1,kd))*
     & d12(2)
      wrast2(i1,i2,i3,kd)=(wras2(i1,i2,i3+1,kd)-wras2(i1,i2,i3-1,kd))*
     & d12(2)
      wrarrr2(i1,i2,i3,kd)=(-2.*(wra(i1+1,i2,i3,kd)-wra(i1-1,i2,i3,kd))
     & +(wra(i1+2,i2,i3,kd)-wra(i1-2,i2,i3,kd)) )*d22(0)*d12(0)
      wrasss2(i1,i2,i3,kd)=(-2.*(wra(i1,i2+1,i3,kd)-wra(i1,i2-1,i3,kd))
     & +(wra(i1,i2+2,i3,kd)-wra(i1,i2-2,i3,kd)) )*d22(1)*d12(1)
      wrattt2(i1,i2,i3,kd)=(-2.*(wra(i1,i2,i3+1,kd)-wra(i1,i2,i3-1,kd))
     & +(wra(i1,i2,i3+2,kd)-wra(i1,i2,i3-2,kd)) )*d22(2)*d12(2)
      wrax21(i1,i2,i3,kd)= rx(i1,i2,i3)*wrar2(i1,i2,i3,kd)
      wray21(i1,i2,i3,kd)=0
      wraz21(i1,i2,i3,kd)=0
      wrax22(i1,i2,i3,kd)= rx(i1,i2,i3)*wrar2(i1,i2,i3,kd)+sx(i1,i2,i3)
     & *wras2(i1,i2,i3,kd)
      wray22(i1,i2,i3,kd)= ry(i1,i2,i3)*wrar2(i1,i2,i3,kd)+sy(i1,i2,i3)
     & *wras2(i1,i2,i3,kd)
      wraz22(i1,i2,i3,kd)=0
      wrax23(i1,i2,i3,kd)=rx(i1,i2,i3)*wrar2(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & wras2(i1,i2,i3,kd)+tx(i1,i2,i3)*wrat2(i1,i2,i3,kd)
      wray23(i1,i2,i3,kd)=ry(i1,i2,i3)*wrar2(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & wras2(i1,i2,i3,kd)+ty(i1,i2,i3)*wrat2(i1,i2,i3,kd)
      wraz23(i1,i2,i3,kd)=rz(i1,i2,i3)*wrar2(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & wras2(i1,i2,i3,kd)+tz(i1,i2,i3)*wrat2(i1,i2,i3,kd)
      wraxx21(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wrarr2(i1,i2,i3,kd)+(
     & rxx22(i1,i2,i3))*wrar2(i1,i2,i3,kd)
      wrayy21(i1,i2,i3,kd)=0
      wraxy21(i1,i2,i3,kd)=0
      wraxz21(i1,i2,i3,kd)=0
      wrayz21(i1,i2,i3,kd)=0
      wrazz21(i1,i2,i3,kd)=0
      wralaplacian21(i1,i2,i3,kd)=wraxx21(i1,i2,i3,kd)
      wraxx22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wrarr2(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*wrars2(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*wrass2(i1,i2,i3,kd)+(rxx22(i1,i2,i3))*wrar2(i1,i2,i3,kd)+(
     & sxx22(i1,i2,i3))*wras2(i1,i2,i3,kd)
      wrayy22(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*wrarr2(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*wrars2(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*wrass2(i1,i2,i3,kd)+(ryy22(i1,i2,i3))*wrar2(i1,i2,i3,kd)+(
     & syy22(i1,i2,i3))*wras2(i1,i2,i3,kd)
      wraxy22(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wrarr2(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & wrars2(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wrass2(i1,i2,i3,
     & kd)+rxy22(i1,i2,i3)*wrar2(i1,i2,i3,kd)+sxy22(i1,i2,i3)*wras2(
     & i1,i2,i3,kd)
      wraxz22(i1,i2,i3,kd)=0
      wrayz22(i1,i2,i3,kd)=0
      wrazz22(i1,i2,i3,kd)=0
      wralaplacian22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & wrarr2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)
     & *sy(i1,i2,i3))*wrars2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2)*wrass2(i1,i2,i3,kd)+(rxx22(i1,i2,i3)+ryy22(i1,i2,i3))*
     & wrar2(i1,i2,i3,kd)+(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*wras2(i1,
     & i2,i3,kd)
      wraxx23(i1,i2,i3,kd)=rx(i1,i2,i3)**2*wrarr2(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*wrass2(i1,i2,i3,kd)+tx(i1,i2,i3)**2*wratt2(i1,i2,i3,
     & kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*wrars2(i1,i2,i3,kd)+2.*rx(i1,
     & i2,i3)*tx(i1,i2,i3)*wrart2(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,
     & i2,i3)*wrast2(i1,i2,i3,kd)+rxx23(i1,i2,i3)*wrar2(i1,i2,i3,kd)+
     & sxx23(i1,i2,i3)*wras2(i1,i2,i3,kd)+txx23(i1,i2,i3)*wrat2(i1,i2,
     & i3,kd)
      wrayy23(i1,i2,i3,kd)=ry(i1,i2,i3)**2*wrarr2(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*wrass2(i1,i2,i3,kd)+ty(i1,i2,i3)**2*wratt2(i1,i2,i3,
     & kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*wrars2(i1,i2,i3,kd)+2.*ry(i1,
     & i2,i3)*ty(i1,i2,i3)*wrart2(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,
     & i2,i3)*wrast2(i1,i2,i3,kd)+ryy23(i1,i2,i3)*wrar2(i1,i2,i3,kd)+
     & syy23(i1,i2,i3)*wras2(i1,i2,i3,kd)+tyy23(i1,i2,i3)*wrat2(i1,i2,
     & i3,kd)
      wrazz23(i1,i2,i3,kd)=rz(i1,i2,i3)**2*wrarr2(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*wrass2(i1,i2,i3,kd)+tz(i1,i2,i3)**2*wratt2(i1,i2,i3,
     & kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*wrars2(i1,i2,i3,kd)+2.*rz(i1,
     & i2,i3)*tz(i1,i2,i3)*wrart2(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,
     & i2,i3)*wrast2(i1,i2,i3,kd)+rzz23(i1,i2,i3)*wrar2(i1,i2,i3,kd)+
     & szz23(i1,i2,i3)*wras2(i1,i2,i3,kd)+tzz23(i1,i2,i3)*wrat2(i1,i2,
     & i3,kd)
      wraxy23(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wrarr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wrass2(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & ty(i1,i2,i3)*wratt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(
     & i1,i2,i3)*sx(i1,i2,i3))*wrars2(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(
     & i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*wrart2(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*wrast2(i1,i2,
     & i3,kd)+rxy23(i1,i2,i3)*wrar2(i1,i2,i3,kd)+sxy23(i1,i2,i3)*
     & wras2(i1,i2,i3,kd)+txy23(i1,i2,i3)*wrat2(i1,i2,i3,kd)
      wraxz23(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*wrarr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*wrass2(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & tz(i1,i2,i3)*wratt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sx(i1,i2,i3))*wrars2(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*wrart2(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*wrast2(i1,i2,
     & i3,kd)+rxz23(i1,i2,i3)*wrar2(i1,i2,i3,kd)+sxz23(i1,i2,i3)*
     & wras2(i1,i2,i3,kd)+txz23(i1,i2,i3)*wrat2(i1,i2,i3,kd)
      wrayz23(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*wrarr2(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*wrass2(i1,i2,i3,kd)+ty(i1,i2,i3)*
     & tz(i1,i2,i3)*wratt2(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sy(i1,i2,i3))*wrars2(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*wrart2(i1,i2,i3,kd)+(sy(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*wrast2(i1,i2,
     & i3,kd)+ryz23(i1,i2,i3)*wrar2(i1,i2,i3,kd)+syz23(i1,i2,i3)*
     & wras2(i1,i2,i3,kd)+tyz23(i1,i2,i3)*wrat2(i1,i2,i3,kd)
      wralaplacian23(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*wrarr2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)
     & **2+sz(i1,i2,i3)**2)*wrass2(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(
     & i1,i2,i3)**2+tz(i1,i2,i3)**2)*wratt2(i1,i2,i3,kd)+2.*(rx(i1,i2,
     & i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,
     & i2,i3))*wrars2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(
     & i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*wrart2(i1,i2,
     & i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)
     & +sz(i1,i2,i3)*tz(i1,i2,i3))*wrast2(i1,i2,i3,kd)+(rxx23(i1,i2,
     & i3)+ryy23(i1,i2,i3)+rzz23(i1,i2,i3))*wrar2(i1,i2,i3,kd)+(sxx23(
     & i1,i2,i3)+syy23(i1,i2,i3)+szz23(i1,i2,i3))*wras2(i1,i2,i3,kd)+(
     & txx23(i1,i2,i3)+tyy23(i1,i2,i3)+tzz23(i1,i2,i3))*wrat2(i1,i2,
     & i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      wrax23r(i1,i2,i3,kd)=(wra(i1+1,i2,i3,kd)-wra(i1-1,i2,i3,kd))*h12(
     & 0)
      wray23r(i1,i2,i3,kd)=(wra(i1,i2+1,i3,kd)-wra(i1,i2-1,i3,kd))*h12(
     & 1)
      wraz23r(i1,i2,i3,kd)=(wra(i1,i2,i3+1,kd)-wra(i1,i2,i3-1,kd))*h12(
     & 2)
      wraxx23r(i1,i2,i3,kd)=(-2.*wra(i1,i2,i3,kd)+(wra(i1+1,i2,i3,kd)+
     & wra(i1-1,i2,i3,kd)) )*h22(0)
      wrayy23r(i1,i2,i3,kd)=(-2.*wra(i1,i2,i3,kd)+(wra(i1,i2+1,i3,kd)+
     & wra(i1,i2-1,i3,kd)) )*h22(1)
      wraxy23r(i1,i2,i3,kd)=(wrax23r(i1,i2+1,i3,kd)-wrax23r(i1,i2-1,i3,
     & kd))*h12(1)
      wrazz23r(i1,i2,i3,kd)=(-2.*wra(i1,i2,i3,kd)+(wra(i1,i2,i3+1,kd)+
     & wra(i1,i2,i3-1,kd)) )*h22(2)
      wraxz23r(i1,i2,i3,kd)=(wrax23r(i1,i2,i3+1,kd)-wrax23r(i1,i2,i3-1,
     & kd))*h12(2)
      wrayz23r(i1,i2,i3,kd)=(wray23r(i1,i2,i3+1,kd)-wray23r(i1,i2,i3-1,
     & kd))*h12(2)
      wrax21r(i1,i2,i3,kd)= wrax23r(i1,i2,i3,kd)
      wray21r(i1,i2,i3,kd)= wray23r(i1,i2,i3,kd)
      wraz21r(i1,i2,i3,kd)= wraz23r(i1,i2,i3,kd)
      wraxx21r(i1,i2,i3,kd)= wraxx23r(i1,i2,i3,kd)
      wrayy21r(i1,i2,i3,kd)= wrayy23r(i1,i2,i3,kd)
      wrazz21r(i1,i2,i3,kd)= wrazz23r(i1,i2,i3,kd)
      wraxy21r(i1,i2,i3,kd)= wraxy23r(i1,i2,i3,kd)
      wraxz21r(i1,i2,i3,kd)= wraxz23r(i1,i2,i3,kd)
      wrayz21r(i1,i2,i3,kd)= wrayz23r(i1,i2,i3,kd)
      wralaplacian21r(i1,i2,i3,kd)=wraxx23r(i1,i2,i3,kd)
      wrax22r(i1,i2,i3,kd)= wrax23r(i1,i2,i3,kd)
      wray22r(i1,i2,i3,kd)= wray23r(i1,i2,i3,kd)
      wraz22r(i1,i2,i3,kd)= wraz23r(i1,i2,i3,kd)
      wraxx22r(i1,i2,i3,kd)= wraxx23r(i1,i2,i3,kd)
      wrayy22r(i1,i2,i3,kd)= wrayy23r(i1,i2,i3,kd)
      wrazz22r(i1,i2,i3,kd)= wrazz23r(i1,i2,i3,kd)
      wraxy22r(i1,i2,i3,kd)= wraxy23r(i1,i2,i3,kd)
      wraxz22r(i1,i2,i3,kd)= wraxz23r(i1,i2,i3,kd)
      wrayz22r(i1,i2,i3,kd)= wrayz23r(i1,i2,i3,kd)
      wralaplacian22r(i1,i2,i3,kd)=wraxx23r(i1,i2,i3,kd)+wrayy23r(i1,
     & i2,i3,kd)
      wralaplacian23r(i1,i2,i3,kd)=wraxx23r(i1,i2,i3,kd)+wrayy23r(i1,
     & i2,i3,kd)+wrazz23r(i1,i2,i3,kd)
      wraxxx22r(i1,i2,i3,kd)=(-2.*(wra(i1+1,i2,i3,kd)-wra(i1-1,i2,i3,
     & kd))+(wra(i1+2,i2,i3,kd)-wra(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
      wrayyy22r(i1,i2,i3,kd)=(-2.*(wra(i1,i2+1,i3,kd)-wra(i1,i2-1,i3,
     & kd))+(wra(i1,i2+2,i3,kd)-wra(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
      wraxxy22r(i1,i2,i3,kd)=( wraxx22r(i1,i2+1,i3,kd)-wraxx22r(i1,i2-
     & 1,i3,kd))/(2.*dx(1))
      wraxyy22r(i1,i2,i3,kd)=( wrayy22r(i1+1,i2,i3,kd)-wrayy22r(i1-1,
     & i2,i3,kd))/(2.*dx(0))
      wraxxxx22r(i1,i2,i3,kd)=(6.*wra(i1,i2,i3,kd)-4.*(wra(i1+1,i2,i3,
     & kd)+wra(i1-1,i2,i3,kd))+(wra(i1+2,i2,i3,kd)+wra(i1-2,i2,i3,kd))
     &  )/(dx(0)**4)
      wrayyyy22r(i1,i2,i3,kd)=(6.*wra(i1,i2,i3,kd)-4.*(wra(i1,i2+1,i3,
     & kd)+wra(i1,i2-1,i3,kd))+(wra(i1,i2+2,i3,kd)+wra(i1,i2-2,i3,kd))
     &  )/(dx(1)**4)
      wraxxyy22r(i1,i2,i3,kd)=( 4.*wra(i1,i2,i3,kd)     -2.*(wra(i1+1,
     & i2,i3,kd)+wra(i1-1,i2,i3,kd)+wra(i1,i2+1,i3,kd)+wra(i1,i2-1,i3,
     & kd))   +   (wra(i1+1,i2+1,i3,kd)+wra(i1-1,i2+1,i3,kd)+wra(i1+1,
     & i2-1,i3,kd)+wra(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      ! 2D laplacian squared = wra.xxxx + 2 wra.xxyy + wra.yyyy
      wraLapSq22r(i1,i2,i3,kd)= ( 6.*wra(i1,i2,i3,kd)   - 4.*(wra(i1+1,
     & i2,i3,kd)+wra(i1-1,i2,i3,kd))    +(wra(i1+2,i2,i3,kd)+wra(i1-2,
     & i2,i3,kd)) )/(dx(0)**4) +( 6.*wra(i1,i2,i3,kd)    -4.*(wra(i1,
     & i2+1,i3,kd)+wra(i1,i2-1,i3,kd))    +(wra(i1,i2+2,i3,kd)+wra(i1,
     & i2-2,i3,kd)) )/(dx(1)**4)  +( 8.*wra(i1,i2,i3,kd)     -4.*(wra(
     & i1+1,i2,i3,kd)+wra(i1-1,i2,i3,kd)+wra(i1,i2+1,i3,kd)+wra(i1,i2-
     & 1,i3,kd))   +2.*(wra(i1+1,i2+1,i3,kd)+wra(i1-1,i2+1,i3,kd)+wra(
     & i1+1,i2-1,i3,kd)+wra(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      wraxxx23r(i1,i2,i3,kd)=(-2.*(wra(i1+1,i2,i3,kd)-wra(i1-1,i2,i3,
     & kd))+(wra(i1+2,i2,i3,kd)-wra(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
      wrayyy23r(i1,i2,i3,kd)=(-2.*(wra(i1,i2+1,i3,kd)-wra(i1,i2-1,i3,
     & kd))+(wra(i1,i2+2,i3,kd)-wra(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
      wrazzz23r(i1,i2,i3,kd)=(-2.*(wra(i1,i2,i3+1,kd)-wra(i1,i2,i3-1,
     & kd))+(wra(i1,i2,i3+2,kd)-wra(i1,i2,i3-2,kd)) )*h22(1)*h12(2)
      wraxxy23r(i1,i2,i3,kd)=( wraxx22r(i1,i2+1,i3,kd)-wraxx22r(i1,i2-
     & 1,i3,kd))/(2.*dx(1))
      wraxyy23r(i1,i2,i3,kd)=( wrayy22r(i1+1,i2,i3,kd)-wrayy22r(i1-1,
     & i2,i3,kd))/(2.*dx(0))
      wraxxz23r(i1,i2,i3,kd)=( wraxx22r(i1,i2,i3+1,kd)-wraxx22r(i1,i2,
     & i3-1,kd))/(2.*dx(2))
      wrayyz23r(i1,i2,i3,kd)=( wrayy22r(i1,i2,i3+1,kd)-wrayy22r(i1,i2,
     & i3-1,kd))/(2.*dx(2))
      wraxzz23r(i1,i2,i3,kd)=( wrazz22r(i1+1,i2,i3,kd)-wrazz22r(i1-1,
     & i2,i3,kd))/(2.*dx(0))
      wrayzz23r(i1,i2,i3,kd)=( wrazz22r(i1,i2+1,i3,kd)-wrazz22r(i1,i2-
     & 1,i3,kd))/(2.*dx(1))
      wraxxxx23r(i1,i2,i3,kd)=(6.*wra(i1,i2,i3,kd)-4.*(wra(i1+1,i2,i3,
     & kd)+wra(i1-1,i2,i3,kd))+(wra(i1+2,i2,i3,kd)+wra(i1-2,i2,i3,kd))
     &  )/(dx(0)**4)
      wrayyyy23r(i1,i2,i3,kd)=(6.*wra(i1,i2,i3,kd)-4.*(wra(i1,i2+1,i3,
     & kd)+wra(i1,i2-1,i3,kd))+(wra(i1,i2+2,i3,kd)+wra(i1,i2-2,i3,kd))
     &  )/(dx(1)**4)
      wrazzzz23r(i1,i2,i3,kd)=(6.*wra(i1,i2,i3,kd)-4.*(wra(i1,i2,i3+1,
     & kd)+wra(i1,i2,i3-1,kd))+(wra(i1,i2,i3+2,kd)+wra(i1,i2,i3-2,kd))
     &  )/(dx(2)**4)
      wraxxyy23r(i1,i2,i3,kd)=( 4.*wra(i1,i2,i3,kd)     -2.*(wra(i1+1,
     & i2,i3,kd)+wra(i1-1,i2,i3,kd)+wra(i1,i2+1,i3,kd)+wra(i1,i2-1,i3,
     & kd))   +   (wra(i1+1,i2+1,i3,kd)+wra(i1-1,i2+1,i3,kd)+wra(i1+1,
     & i2-1,i3,kd)+wra(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      wraxxzz23r(i1,i2,i3,kd)=( 4.*wra(i1,i2,i3,kd)     -2.*(wra(i1+1,
     & i2,i3,kd)+wra(i1-1,i2,i3,kd)+wra(i1,i2,i3+1,kd)+wra(i1,i2,i3-1,
     & kd))   +   (wra(i1+1,i2,i3+1,kd)+wra(i1-1,i2,i3+1,kd)+wra(i1+1,
     & i2,i3-1,kd)+wra(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)
      wrayyzz23r(i1,i2,i3,kd)=( 4.*wra(i1,i2,i3,kd)     -2.*(wra(i1,i2+
     & 1,i3,kd)  +wra(i1,i2-1,i3,kd)+  wra(i1,i2  ,i3+1,kd)+wra(i1,i2 
     &  ,i3-1,kd))   +   (wra(i1,i2+1,i3+1,kd)+wra(i1,i2-1,i3+1,kd)+
     & wra(i1,i2+1,i3-1,kd)+wra(i1,i2-1,i3-1,kd)) )/(dx(1)**2*dx(2)**
     & 2)
      ! 3D laplacian squared = wra.xxxx + wra.yyyy + wra.zzzz + 2 (wra.xxyy + wra.xxzz + wra.yyzz )
      wraLapSq23r(i1,i2,i3,kd)= ( 6.*wra(i1,i2,i3,kd)   - 4.*(wra(i1+1,
     & i2,i3,kd)+wra(i1-1,i2,i3,kd))    +(wra(i1+2,i2,i3,kd)+wra(i1-2,
     & i2,i3,kd)) )/(dx(0)**4) +( 6.*wra(i1,i2,i3,kd)    -4.*(wra(i1,
     & i2+1,i3,kd)+wra(i1,i2-1,i3,kd))    +(wra(i1,i2+2,i3,kd)+wra(i1,
     & i2-2,i3,kd)) )/(dx(1)**4)  +( 6.*wra(i1,i2,i3,kd)    -4.*(wra(
     & i1,i2,i3+1,kd)+wra(i1,i2,i3-1,kd))    +(wra(i1,i2,i3+2,kd)+wra(
     & i1,i2,i3-2,kd)) )/(dx(2)**4)  +( 8.*wra(i1,i2,i3,kd)     -4.*(
     & wra(i1+1,i2,i3,kd)  +wra(i1-1,i2,i3,kd)  +wra(i1  ,i2+1,i3,kd)+
     & wra(i1  ,i2-1,i3,kd))   +2.*(wra(i1+1,i2+1,i3,kd)+wra(i1-1,i2+
     & 1,i3,kd)+wra(i1+1,i2-1,i3,kd)+wra(i1-1,i2-1,i3,kd)) )/(dx(0)**
     & 2*dx(1)**2)+( 8.*wra(i1,i2,i3,kd)     -4.*(wra(i1+1,i2,i3,kd)  
     & +wra(i1-1,i2,i3,kd)  +wra(i1  ,i2,i3+1,kd)+wra(i1  ,i2,i3-1,kd)
     & )   +2.*(wra(i1+1,i2,i3+1,kd)+wra(i1-1,i2,i3+1,kd)+wra(i1+1,i2,
     & i3-1,kd)+wra(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)+( 8.*wra(
     & i1,i2,i3,kd)     -4.*(wra(i1,i2+1,i3,kd)  +wra(i1,i2-1,i3,kd)  
     & +wra(i1,i2  ,i3+1,kd)+wra(i1,i2  ,i3-1,kd))   +2.*(wra(i1,i2+1,
     & i3+1,kd)+wra(i1,i2-1,i3+1,kd)+wra(i1,i2+1,i3-1,kd)+wra(i1,i2-1,
     & i3-1,kd)) )/(dx(1)**2*dx(2)**2)
      wramr2(i1,i2,i3,kd)=(wram(i1+1,i2,i3,kd)-wram(i1-1,i2,i3,kd))*
     & d12(0)
      wrams2(i1,i2,i3,kd)=(wram(i1,i2+1,i3,kd)-wram(i1,i2-1,i3,kd))*
     & d12(1)
      wramt2(i1,i2,i3,kd)=(wram(i1,i2,i3+1,kd)-wram(i1,i2,i3-1,kd))*
     & d12(2)
      wramrr2(i1,i2,i3,kd)=(-2.*wram(i1,i2,i3,kd)+(wram(i1+1,i2,i3,kd)+
     & wram(i1-1,i2,i3,kd)) )*d22(0)
      wramss2(i1,i2,i3,kd)=(-2.*wram(i1,i2,i3,kd)+(wram(i1,i2+1,i3,kd)+
     & wram(i1,i2-1,i3,kd)) )*d22(1)
      wramrs2(i1,i2,i3,kd)=(wramr2(i1,i2+1,i3,kd)-wramr2(i1,i2-1,i3,kd)
     & )*d12(1)
      wramtt2(i1,i2,i3,kd)=(-2.*wram(i1,i2,i3,kd)+(wram(i1,i2,i3+1,kd)+
     & wram(i1,i2,i3-1,kd)) )*d22(2)
      wramrt2(i1,i2,i3,kd)=(wramr2(i1,i2,i3+1,kd)-wramr2(i1,i2,i3-1,kd)
     & )*d12(2)
      wramst2(i1,i2,i3,kd)=(wrams2(i1,i2,i3+1,kd)-wrams2(i1,i2,i3-1,kd)
     & )*d12(2)
      wramrrr2(i1,i2,i3,kd)=(-2.*(wram(i1+1,i2,i3,kd)-wram(i1-1,i2,i3,
     & kd))+(wram(i1+2,i2,i3,kd)-wram(i1-2,i2,i3,kd)) )*d22(0)*d12(0)
      wramsss2(i1,i2,i3,kd)=(-2.*(wram(i1,i2+1,i3,kd)-wram(i1,i2-1,i3,
     & kd))+(wram(i1,i2+2,i3,kd)-wram(i1,i2-2,i3,kd)) )*d22(1)*d12(1)
      wramttt2(i1,i2,i3,kd)=(-2.*(wram(i1,i2,i3+1,kd)-wram(i1,i2,i3-1,
     & kd))+(wram(i1,i2,i3+2,kd)-wram(i1,i2,i3-2,kd)) )*d22(2)*d12(2)
      wramx21(i1,i2,i3,kd)= rx(i1,i2,i3)*wramr2(i1,i2,i3,kd)
      wramy21(i1,i2,i3,kd)=0
      wramz21(i1,i2,i3,kd)=0
      wramx22(i1,i2,i3,kd)= rx(i1,i2,i3)*wramr2(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*wrams2(i1,i2,i3,kd)
      wramy22(i1,i2,i3,kd)= ry(i1,i2,i3)*wramr2(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*wrams2(i1,i2,i3,kd)
      wramz22(i1,i2,i3,kd)=0
      wramx23(i1,i2,i3,kd)=rx(i1,i2,i3)*wramr2(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*wrams2(i1,i2,i3,kd)+tx(i1,i2,i3)*wramt2(i1,i2,i3,kd)
      wramy23(i1,i2,i3,kd)=ry(i1,i2,i3)*wramr2(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*wrams2(i1,i2,i3,kd)+ty(i1,i2,i3)*wramt2(i1,i2,i3,kd)
      wramz23(i1,i2,i3,kd)=rz(i1,i2,i3)*wramr2(i1,i2,i3,kd)+sz(i1,i2,
     & i3)*wrams2(i1,i2,i3,kd)+tz(i1,i2,i3)*wramt2(i1,i2,i3,kd)
      wramxx21(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wramrr2(i1,i2,i3,kd)+(
     & rxx22(i1,i2,i3))*wramr2(i1,i2,i3,kd)
      wramyy21(i1,i2,i3,kd)=0
      wramxy21(i1,i2,i3,kd)=0
      wramxz21(i1,i2,i3,kd)=0
      wramyz21(i1,i2,i3,kd)=0
      wramzz21(i1,i2,i3,kd)=0
      wramlaplacian21(i1,i2,i3,kd)=wramxx21(i1,i2,i3,kd)
      wramxx22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wramrr2(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*wramrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*wramss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3))*wramr2(i1,i2,i3,kd)+(
     & sxx22(i1,i2,i3))*wrams2(i1,i2,i3,kd)
      wramyy22(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*wramrr2(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*wramrs2(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*wramss2(i1,i2,i3,kd)+(ryy22(i1,i2,i3))*wramr2(i1,i2,i3,kd)+(
     & syy22(i1,i2,i3))*wrams2(i1,i2,i3,kd)
      wramxy22(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wramrr2(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & wramrs2(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wramss2(i1,i2,
     & i3,kd)+rxy22(i1,i2,i3)*wramr2(i1,i2,i3,kd)+sxy22(i1,i2,i3)*
     & wrams2(i1,i2,i3,kd)
      wramxz22(i1,i2,i3,kd)=0
      wramyz22(i1,i2,i3,kd)=0
      wramzz22(i1,i2,i3,kd)=0
      wramlaplacian22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & wramrr2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,
     & i3)*sy(i1,i2,i3))*wramrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,
     & i2,i3)**2)*wramss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3)+ryy22(i1,i2,
     & i3))*wramr2(i1,i2,i3,kd)+(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*
     & wrams2(i1,i2,i3,kd)
      wramxx23(i1,i2,i3,kd)=rx(i1,i2,i3)**2*wramrr2(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*wramss2(i1,i2,i3,kd)+tx(i1,i2,i3)**2*wramtt2(i1,i2,
     & i3,kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*wramrs2(i1,i2,i3,kd)+2.*rx(
     & i1,i2,i3)*tx(i1,i2,i3)*wramrt2(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(
     & i1,i2,i3)*wramst2(i1,i2,i3,kd)+rxx23(i1,i2,i3)*wramr2(i1,i2,i3,
     & kd)+sxx23(i1,i2,i3)*wrams2(i1,i2,i3,kd)+txx23(i1,i2,i3)*wramt2(
     & i1,i2,i3,kd)
      wramyy23(i1,i2,i3,kd)=ry(i1,i2,i3)**2*wramrr2(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*wramss2(i1,i2,i3,kd)+ty(i1,i2,i3)**2*wramtt2(i1,i2,
     & i3,kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*wramrs2(i1,i2,i3,kd)+2.*ry(
     & i1,i2,i3)*ty(i1,i2,i3)*wramrt2(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(
     & i1,i2,i3)*wramst2(i1,i2,i3,kd)+ryy23(i1,i2,i3)*wramr2(i1,i2,i3,
     & kd)+syy23(i1,i2,i3)*wrams2(i1,i2,i3,kd)+tyy23(i1,i2,i3)*wramt2(
     & i1,i2,i3,kd)
      wramzz23(i1,i2,i3,kd)=rz(i1,i2,i3)**2*wramrr2(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*wramss2(i1,i2,i3,kd)+tz(i1,i2,i3)**2*wramtt2(i1,i2,
     & i3,kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*wramrs2(i1,i2,i3,kd)+2.*rz(
     & i1,i2,i3)*tz(i1,i2,i3)*wramrt2(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(
     & i1,i2,i3)*wramst2(i1,i2,i3,kd)+rzz23(i1,i2,i3)*wramr2(i1,i2,i3,
     & kd)+szz23(i1,i2,i3)*wrams2(i1,i2,i3,kd)+tzz23(i1,i2,i3)*wramt2(
     & i1,i2,i3,kd)
      wramxy23(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wramrr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wramss2(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *ty(i1,i2,i3)*wramtt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+
     & ry(i1,i2,i3)*sx(i1,i2,i3))*wramrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & ty(i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*wramrt2(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*wramst2(
     & i1,i2,i3,kd)+rxy23(i1,i2,i3)*wramr2(i1,i2,i3,kd)+sxy23(i1,i2,
     & i3)*wrams2(i1,i2,i3,kd)+txy23(i1,i2,i3)*wramt2(i1,i2,i3,kd)
      wramxz23(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*wramrr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*wramss2(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *tz(i1,i2,i3)*wramtt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sx(i1,i2,i3))*wramrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*wramrt2(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*wramst2(
     & i1,i2,i3,kd)+rxz23(i1,i2,i3)*wramr2(i1,i2,i3,kd)+sxz23(i1,i2,
     & i3)*wrams2(i1,i2,i3,kd)+txz23(i1,i2,i3)*wramt2(i1,i2,i3,kd)
      wramyz23(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*wramrr2(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*wramss2(i1,i2,i3,kd)+ty(i1,i2,i3)
     & *tz(i1,i2,i3)*wramtt2(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sy(i1,i2,i3))*wramrs2(i1,i2,i3,kd)+(ry(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*wramrt2(i1,i2,i3,kd)+(
     & sy(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*wramst2(
     & i1,i2,i3,kd)+ryz23(i1,i2,i3)*wramr2(i1,i2,i3,kd)+syz23(i1,i2,
     & i3)*wrams2(i1,i2,i3,kd)+tyz23(i1,i2,i3)*wramt2(i1,i2,i3,kd)
      wramlaplacian23(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*wramrr2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2+sz(i1,i2,i3)**2)*wramss2(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+
     & ty(i1,i2,i3)**2+tz(i1,i2,i3)**2)*wramtt2(i1,i2,i3,kd)+2.*(rx(
     & i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*
     & sz(i1,i2,i3))*wramrs2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,
     & i3)+ ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*
     & wramrt2(i1,i2,i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,
     & i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3))*wramst2(i1,i2,i3,
     & kd)+(rxx23(i1,i2,i3)+ryy23(i1,i2,i3)+rzz23(i1,i2,i3))*wramr2(
     & i1,i2,i3,kd)+(sxx23(i1,i2,i3)+syy23(i1,i2,i3)+szz23(i1,i2,i3))*
     & wrams2(i1,i2,i3,kd)+(txx23(i1,i2,i3)+tyy23(i1,i2,i3)+tzz23(i1,
     & i2,i3))*wramt2(i1,i2,i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      wramx23r(i1,i2,i3,kd)=(wram(i1+1,i2,i3,kd)-wram(i1-1,i2,i3,kd))*
     & h12(0)
      wramy23r(i1,i2,i3,kd)=(wram(i1,i2+1,i3,kd)-wram(i1,i2-1,i3,kd))*
     & h12(1)
      wramz23r(i1,i2,i3,kd)=(wram(i1,i2,i3+1,kd)-wram(i1,i2,i3-1,kd))*
     & h12(2)
      wramxx23r(i1,i2,i3,kd)=(-2.*wram(i1,i2,i3,kd)+(wram(i1+1,i2,i3,
     & kd)+wram(i1-1,i2,i3,kd)) )*h22(0)
      wramyy23r(i1,i2,i3,kd)=(-2.*wram(i1,i2,i3,kd)+(wram(i1,i2+1,i3,
     & kd)+wram(i1,i2-1,i3,kd)) )*h22(1)
      wramxy23r(i1,i2,i3,kd)=(wramx23r(i1,i2+1,i3,kd)-wramx23r(i1,i2-1,
     & i3,kd))*h12(1)
      wramzz23r(i1,i2,i3,kd)=(-2.*wram(i1,i2,i3,kd)+(wram(i1,i2,i3+1,
     & kd)+wram(i1,i2,i3-1,kd)) )*h22(2)
      wramxz23r(i1,i2,i3,kd)=(wramx23r(i1,i2,i3+1,kd)-wramx23r(i1,i2,
     & i3-1,kd))*h12(2)
      wramyz23r(i1,i2,i3,kd)=(wramy23r(i1,i2,i3+1,kd)-wramy23r(i1,i2,
     & i3-1,kd))*h12(2)
      wramx21r(i1,i2,i3,kd)= wramx23r(i1,i2,i3,kd)
      wramy21r(i1,i2,i3,kd)= wramy23r(i1,i2,i3,kd)
      wramz21r(i1,i2,i3,kd)= wramz23r(i1,i2,i3,kd)
      wramxx21r(i1,i2,i3,kd)= wramxx23r(i1,i2,i3,kd)
      wramyy21r(i1,i2,i3,kd)= wramyy23r(i1,i2,i3,kd)
      wramzz21r(i1,i2,i3,kd)= wramzz23r(i1,i2,i3,kd)
      wramxy21r(i1,i2,i3,kd)= wramxy23r(i1,i2,i3,kd)
      wramxz21r(i1,i2,i3,kd)= wramxz23r(i1,i2,i3,kd)
      wramyz21r(i1,i2,i3,kd)= wramyz23r(i1,i2,i3,kd)
      wramlaplacian21r(i1,i2,i3,kd)=wramxx23r(i1,i2,i3,kd)
      wramx22r(i1,i2,i3,kd)= wramx23r(i1,i2,i3,kd)
      wramy22r(i1,i2,i3,kd)= wramy23r(i1,i2,i3,kd)
      wramz22r(i1,i2,i3,kd)= wramz23r(i1,i2,i3,kd)
      wramxx22r(i1,i2,i3,kd)= wramxx23r(i1,i2,i3,kd)
      wramyy22r(i1,i2,i3,kd)= wramyy23r(i1,i2,i3,kd)
      wramzz22r(i1,i2,i3,kd)= wramzz23r(i1,i2,i3,kd)
      wramxy22r(i1,i2,i3,kd)= wramxy23r(i1,i2,i3,kd)
      wramxz22r(i1,i2,i3,kd)= wramxz23r(i1,i2,i3,kd)
      wramyz22r(i1,i2,i3,kd)= wramyz23r(i1,i2,i3,kd)
      wramlaplacian22r(i1,i2,i3,kd)=wramxx23r(i1,i2,i3,kd)+wramyy23r(
     & i1,i2,i3,kd)
      wramlaplacian23r(i1,i2,i3,kd)=wramxx23r(i1,i2,i3,kd)+wramyy23r(
     & i1,i2,i3,kd)+wramzz23r(i1,i2,i3,kd)
      wramxxx22r(i1,i2,i3,kd)=(-2.*(wram(i1+1,i2,i3,kd)-wram(i1-1,i2,
     & i3,kd))+(wram(i1+2,i2,i3,kd)-wram(i1-2,i2,i3,kd)) )*h22(0)*h12(
     & 0)
      wramyyy22r(i1,i2,i3,kd)=(-2.*(wram(i1,i2+1,i3,kd)-wram(i1,i2-1,
     & i3,kd))+(wram(i1,i2+2,i3,kd)-wram(i1,i2-2,i3,kd)) )*h22(1)*h12(
     & 1)
      wramxxy22r(i1,i2,i3,kd)=( wramxx22r(i1,i2+1,i3,kd)-wramxx22r(i1,
     & i2-1,i3,kd))/(2.*dx(1))
      wramxyy22r(i1,i2,i3,kd)=( wramyy22r(i1+1,i2,i3,kd)-wramyy22r(i1-
     & 1,i2,i3,kd))/(2.*dx(0))
      wramxxxx22r(i1,i2,i3,kd)=(6.*wram(i1,i2,i3,kd)-4.*(wram(i1+1,i2,
     & i3,kd)+wram(i1-1,i2,i3,kd))+(wram(i1+2,i2,i3,kd)+wram(i1-2,i2,
     & i3,kd)) )/(dx(0)**4)
      wramyyyy22r(i1,i2,i3,kd)=(6.*wram(i1,i2,i3,kd)-4.*(wram(i1,i2+1,
     & i3,kd)+wram(i1,i2-1,i3,kd))+(wram(i1,i2+2,i3,kd)+wram(i1,i2-2,
     & i3,kd)) )/(dx(1)**4)
      wramxxyy22r(i1,i2,i3,kd)=( 4.*wram(i1,i2,i3,kd)     -2.*(wram(i1+
     & 1,i2,i3,kd)+wram(i1-1,i2,i3,kd)+wram(i1,i2+1,i3,kd)+wram(i1,i2-
     & 1,i3,kd))   +   (wram(i1+1,i2+1,i3,kd)+wram(i1-1,i2+1,i3,kd)+
     & wram(i1+1,i2-1,i3,kd)+wram(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)*
     & *2)
      ! 2D laplacian squared = wram.xxxx + 2 wram.xxyy + wram.yyyy
      wramLapSq22r(i1,i2,i3,kd)= ( 6.*wram(i1,i2,i3,kd)   - 4.*(wram(
     & i1+1,i2,i3,kd)+wram(i1-1,i2,i3,kd))    +(wram(i1+2,i2,i3,kd)+
     & wram(i1-2,i2,i3,kd)) )/(dx(0)**4) +( 6.*wram(i1,i2,i3,kd)    -
     & 4.*(wram(i1,i2+1,i3,kd)+wram(i1,i2-1,i3,kd))    +(wram(i1,i2+2,
     & i3,kd)+wram(i1,i2-2,i3,kd)) )/(dx(1)**4)  +( 8.*wram(i1,i2,i3,
     & kd)     -4.*(wram(i1+1,i2,i3,kd)+wram(i1-1,i2,i3,kd)+wram(i1,
     & i2+1,i3,kd)+wram(i1,i2-1,i3,kd))   +2.*(wram(i1+1,i2+1,i3,kd)+
     & wram(i1-1,i2+1,i3,kd)+wram(i1+1,i2-1,i3,kd)+wram(i1-1,i2-1,i3,
     & kd)) )/(dx(0)**2*dx(1)**2)
      wramxxx23r(i1,i2,i3,kd)=(-2.*(wram(i1+1,i2,i3,kd)-wram(i1-1,i2,
     & i3,kd))+(wram(i1+2,i2,i3,kd)-wram(i1-2,i2,i3,kd)) )*h22(0)*h12(
     & 0)
      wramyyy23r(i1,i2,i3,kd)=(-2.*(wram(i1,i2+1,i3,kd)-wram(i1,i2-1,
     & i3,kd))+(wram(i1,i2+2,i3,kd)-wram(i1,i2-2,i3,kd)) )*h22(1)*h12(
     & 1)
      wramzzz23r(i1,i2,i3,kd)=(-2.*(wram(i1,i2,i3+1,kd)-wram(i1,i2,i3-
     & 1,kd))+(wram(i1,i2,i3+2,kd)-wram(i1,i2,i3-2,kd)) )*h22(1)*h12(
     & 2)
      wramxxy23r(i1,i2,i3,kd)=( wramxx22r(i1,i2+1,i3,kd)-wramxx22r(i1,
     & i2-1,i3,kd))/(2.*dx(1))
      wramxyy23r(i1,i2,i3,kd)=( wramyy22r(i1+1,i2,i3,kd)-wramyy22r(i1-
     & 1,i2,i3,kd))/(2.*dx(0))
      wramxxz23r(i1,i2,i3,kd)=( wramxx22r(i1,i2,i3+1,kd)-wramxx22r(i1,
     & i2,i3-1,kd))/(2.*dx(2))
      wramyyz23r(i1,i2,i3,kd)=( wramyy22r(i1,i2,i3+1,kd)-wramyy22r(i1,
     & i2,i3-1,kd))/(2.*dx(2))
      wramxzz23r(i1,i2,i3,kd)=( wramzz22r(i1+1,i2,i3,kd)-wramzz22r(i1-
     & 1,i2,i3,kd))/(2.*dx(0))
      wramyzz23r(i1,i2,i3,kd)=( wramzz22r(i1,i2+1,i3,kd)-wramzz22r(i1,
     & i2-1,i3,kd))/(2.*dx(1))
      wramxxxx23r(i1,i2,i3,kd)=(6.*wram(i1,i2,i3,kd)-4.*(wram(i1+1,i2,
     & i3,kd)+wram(i1-1,i2,i3,kd))+(wram(i1+2,i2,i3,kd)+wram(i1-2,i2,
     & i3,kd)) )/(dx(0)**4)
      wramyyyy23r(i1,i2,i3,kd)=(6.*wram(i1,i2,i3,kd)-4.*(wram(i1,i2+1,
     & i3,kd)+wram(i1,i2-1,i3,kd))+(wram(i1,i2+2,i3,kd)+wram(i1,i2-2,
     & i3,kd)) )/(dx(1)**4)
      wramzzzz23r(i1,i2,i3,kd)=(6.*wram(i1,i2,i3,kd)-4.*(wram(i1,i2,i3+
     & 1,kd)+wram(i1,i2,i3-1,kd))+(wram(i1,i2,i3+2,kd)+wram(i1,i2,i3-
     & 2,kd)) )/(dx(2)**4)
      wramxxyy23r(i1,i2,i3,kd)=( 4.*wram(i1,i2,i3,kd)     -2.*(wram(i1+
     & 1,i2,i3,kd)+wram(i1-1,i2,i3,kd)+wram(i1,i2+1,i3,kd)+wram(i1,i2-
     & 1,i3,kd))   +   (wram(i1+1,i2+1,i3,kd)+wram(i1-1,i2+1,i3,kd)+
     & wram(i1+1,i2-1,i3,kd)+wram(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)*
     & *2)
      wramxxzz23r(i1,i2,i3,kd)=( 4.*wram(i1,i2,i3,kd)     -2.*(wram(i1+
     & 1,i2,i3,kd)+wram(i1-1,i2,i3,kd)+wram(i1,i2,i3+1,kd)+wram(i1,i2,
     & i3-1,kd))   +   (wram(i1+1,i2,i3+1,kd)+wram(i1-1,i2,i3+1,kd)+
     & wram(i1+1,i2,i3-1,kd)+wram(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)*
     & *2)
      wramyyzz23r(i1,i2,i3,kd)=( 4.*wram(i1,i2,i3,kd)     -2.*(wram(i1,
     & i2+1,i3,kd)  +wram(i1,i2-1,i3,kd)+  wram(i1,i2  ,i3+1,kd)+wram(
     & i1,i2  ,i3-1,kd))   +   (wram(i1,i2+1,i3+1,kd)+wram(i1,i2-1,i3+
     & 1,kd)+wram(i1,i2+1,i3-1,kd)+wram(i1,i2-1,i3-1,kd)) )/(dx(1)**2*
     & dx(2)**2)
      ! 3D laplacian squared = wram.xxxx + wram.yyyy + wram.zzzz + 2 (wram.xxyy + wram.xxzz + wram.yyzz )
      wramLapSq23r(i1,i2,i3,kd)= ( 6.*wram(i1,i2,i3,kd)   - 4.*(wram(
     & i1+1,i2,i3,kd)+wram(i1-1,i2,i3,kd))    +(wram(i1+2,i2,i3,kd)+
     & wram(i1-2,i2,i3,kd)) )/(dx(0)**4) +( 6.*wram(i1,i2,i3,kd)    -
     & 4.*(wram(i1,i2+1,i3,kd)+wram(i1,i2-1,i3,kd))    +(wram(i1,i2+2,
     & i3,kd)+wram(i1,i2-2,i3,kd)) )/(dx(1)**4)  +( 6.*wram(i1,i2,i3,
     & kd)    -4.*(wram(i1,i2,i3+1,kd)+wram(i1,i2,i3-1,kd))    +(wram(
     & i1,i2,i3+2,kd)+wram(i1,i2,i3-2,kd)) )/(dx(2)**4)  +( 8.*wram(
     & i1,i2,i3,kd)     -4.*(wram(i1+1,i2,i3,kd)  +wram(i1-1,i2,i3,kd)
     &   +wram(i1  ,i2+1,i3,kd)+wram(i1  ,i2-1,i3,kd))   +2.*(wram(i1+
     & 1,i2+1,i3,kd)+wram(i1-1,i2+1,i3,kd)+wram(i1+1,i2-1,i3,kd)+wram(
     & i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)+( 8.*wram(i1,i2,i3,kd) 
     &     -4.*(wram(i1+1,i2,i3,kd)  +wram(i1-1,i2,i3,kd)  +wram(i1  ,
     & i2,i3+1,kd)+wram(i1  ,i2,i3-1,kd))   +2.*(wram(i1+1,i2,i3+1,kd)
     & +wram(i1-1,i2,i3+1,kd)+wram(i1+1,i2,i3-1,kd)+wram(i1-1,i2,i3-1,
     & kd)) )/(dx(0)**2*dx(2)**2)+( 8.*wram(i1,i2,i3,kd)     -4.*(
     & wram(i1,i2+1,i3,kd)  +wram(i1,i2-1,i3,kd)  +wram(i1,i2  ,i3+1,
     & kd)+wram(i1,i2  ,i3-1,kd))   +2.*(wram(i1,i2+1,i3+1,kd)+wram(
     & i1,i2-1,i3+1,kd)+wram(i1,i2+1,i3-1,kd)+wram(i1,i2-1,i3-1,kd)) )
     & /(dx(1)**2*dx(2)**2)

      vrbr2(i1,i2,i3,kd)=(vrb(i1+1,i2,i3,kd)-vrb(i1-1,i2,i3,kd))*d12(0)
      vrbs2(i1,i2,i3,kd)=(vrb(i1,i2+1,i3,kd)-vrb(i1,i2-1,i3,kd))*d12(1)
      vrbt2(i1,i2,i3,kd)=(vrb(i1,i2,i3+1,kd)-vrb(i1,i2,i3-1,kd))*d12(2)
      vrbrr2(i1,i2,i3,kd)=(-2.*vrb(i1,i2,i3,kd)+(vrb(i1+1,i2,i3,kd)+
     & vrb(i1-1,i2,i3,kd)) )*d22(0)
      vrbss2(i1,i2,i3,kd)=(-2.*vrb(i1,i2,i3,kd)+(vrb(i1,i2+1,i3,kd)+
     & vrb(i1,i2-1,i3,kd)) )*d22(1)
      vrbrs2(i1,i2,i3,kd)=(vrbr2(i1,i2+1,i3,kd)-vrbr2(i1,i2-1,i3,kd))*
     & d12(1)
      vrbtt2(i1,i2,i3,kd)=(-2.*vrb(i1,i2,i3,kd)+(vrb(i1,i2,i3+1,kd)+
     & vrb(i1,i2,i3-1,kd)) )*d22(2)
      vrbrt2(i1,i2,i3,kd)=(vrbr2(i1,i2,i3+1,kd)-vrbr2(i1,i2,i3-1,kd))*
     & d12(2)
      vrbst2(i1,i2,i3,kd)=(vrbs2(i1,i2,i3+1,kd)-vrbs2(i1,i2,i3-1,kd))*
     & d12(2)
      vrbrrr2(i1,i2,i3,kd)=(-2.*(vrb(i1+1,i2,i3,kd)-vrb(i1-1,i2,i3,kd))
     & +(vrb(i1+2,i2,i3,kd)-vrb(i1-2,i2,i3,kd)) )*d22(0)*d12(0)
      vrbsss2(i1,i2,i3,kd)=(-2.*(vrb(i1,i2+1,i3,kd)-vrb(i1,i2-1,i3,kd))
     & +(vrb(i1,i2+2,i3,kd)-vrb(i1,i2-2,i3,kd)) )*d22(1)*d12(1)
      vrbttt2(i1,i2,i3,kd)=(-2.*(vrb(i1,i2,i3+1,kd)-vrb(i1,i2,i3-1,kd))
     & +(vrb(i1,i2,i3+2,kd)-vrb(i1,i2,i3-2,kd)) )*d22(2)*d12(2)
      vrbx21(i1,i2,i3,kd)= rx(i1,i2,i3)*vrbr2(i1,i2,i3,kd)
      vrby21(i1,i2,i3,kd)=0
      vrbz21(i1,i2,i3,kd)=0
      vrbx22(i1,i2,i3,kd)= rx(i1,i2,i3)*vrbr2(i1,i2,i3,kd)+sx(i1,i2,i3)
     & *vrbs2(i1,i2,i3,kd)
      vrby22(i1,i2,i3,kd)= ry(i1,i2,i3)*vrbr2(i1,i2,i3,kd)+sy(i1,i2,i3)
     & *vrbs2(i1,i2,i3,kd)
      vrbz22(i1,i2,i3,kd)=0
      vrbx23(i1,i2,i3,kd)=rx(i1,i2,i3)*vrbr2(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & vrbs2(i1,i2,i3,kd)+tx(i1,i2,i3)*vrbt2(i1,i2,i3,kd)
      vrby23(i1,i2,i3,kd)=ry(i1,i2,i3)*vrbr2(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & vrbs2(i1,i2,i3,kd)+ty(i1,i2,i3)*vrbt2(i1,i2,i3,kd)
      vrbz23(i1,i2,i3,kd)=rz(i1,i2,i3)*vrbr2(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & vrbs2(i1,i2,i3,kd)+tz(i1,i2,i3)*vrbt2(i1,i2,i3,kd)
      vrbxx21(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vrbrr2(i1,i2,i3,kd)+(
     & rxx22(i1,i2,i3))*vrbr2(i1,i2,i3,kd)
      vrbyy21(i1,i2,i3,kd)=0
      vrbxy21(i1,i2,i3,kd)=0
      vrbxz21(i1,i2,i3,kd)=0
      vrbyz21(i1,i2,i3,kd)=0
      vrbzz21(i1,i2,i3,kd)=0
      vrblaplacian21(i1,i2,i3,kd)=vrbxx21(i1,i2,i3,kd)
      vrbxx22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vrbrr2(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*vrbrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*vrbss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3))*vrbr2(i1,i2,i3,kd)+(
     & sxx22(i1,i2,i3))*vrbs2(i1,i2,i3,kd)
      vrbyy22(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*vrbrr2(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*vrbrs2(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*vrbss2(i1,i2,i3,kd)+(ryy22(i1,i2,i3))*vrbr2(i1,i2,i3,kd)+(
     & syy22(i1,i2,i3))*vrbs2(i1,i2,i3,kd)
      vrbxy22(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vrbrr2(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & vrbrs2(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vrbss2(i1,i2,i3,
     & kd)+rxy22(i1,i2,i3)*vrbr2(i1,i2,i3,kd)+sxy22(i1,i2,i3)*vrbs2(
     & i1,i2,i3,kd)
      vrbxz22(i1,i2,i3,kd)=0
      vrbyz22(i1,i2,i3,kd)=0
      vrbzz22(i1,i2,i3,kd)=0
      vrblaplacian22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & vrbrr2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)
     & *sy(i1,i2,i3))*vrbrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2)*vrbss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3)+ryy22(i1,i2,i3))*
     & vrbr2(i1,i2,i3,kd)+(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*vrbs2(i1,
     & i2,i3,kd)
      vrbxx23(i1,i2,i3,kd)=rx(i1,i2,i3)**2*vrbrr2(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*vrbss2(i1,i2,i3,kd)+tx(i1,i2,i3)**2*vrbtt2(i1,i2,i3,
     & kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*vrbrs2(i1,i2,i3,kd)+2.*rx(i1,
     & i2,i3)*tx(i1,i2,i3)*vrbrt2(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,
     & i2,i3)*vrbst2(i1,i2,i3,kd)+rxx23(i1,i2,i3)*vrbr2(i1,i2,i3,kd)+
     & sxx23(i1,i2,i3)*vrbs2(i1,i2,i3,kd)+txx23(i1,i2,i3)*vrbt2(i1,i2,
     & i3,kd)
      vrbyy23(i1,i2,i3,kd)=ry(i1,i2,i3)**2*vrbrr2(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*vrbss2(i1,i2,i3,kd)+ty(i1,i2,i3)**2*vrbtt2(i1,i2,i3,
     & kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*vrbrs2(i1,i2,i3,kd)+2.*ry(i1,
     & i2,i3)*ty(i1,i2,i3)*vrbrt2(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,
     & i2,i3)*vrbst2(i1,i2,i3,kd)+ryy23(i1,i2,i3)*vrbr2(i1,i2,i3,kd)+
     & syy23(i1,i2,i3)*vrbs2(i1,i2,i3,kd)+tyy23(i1,i2,i3)*vrbt2(i1,i2,
     & i3,kd)
      vrbzz23(i1,i2,i3,kd)=rz(i1,i2,i3)**2*vrbrr2(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*vrbss2(i1,i2,i3,kd)+tz(i1,i2,i3)**2*vrbtt2(i1,i2,i3,
     & kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*vrbrs2(i1,i2,i3,kd)+2.*rz(i1,
     & i2,i3)*tz(i1,i2,i3)*vrbrt2(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,
     & i2,i3)*vrbst2(i1,i2,i3,kd)+rzz23(i1,i2,i3)*vrbr2(i1,i2,i3,kd)+
     & szz23(i1,i2,i3)*vrbs2(i1,i2,i3,kd)+tzz23(i1,i2,i3)*vrbt2(i1,i2,
     & i3,kd)
      vrbxy23(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vrbrr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vrbss2(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & ty(i1,i2,i3)*vrbtt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(
     & i1,i2,i3)*sx(i1,i2,i3))*vrbrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(
     & i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*vrbrt2(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*vrbst2(i1,i2,
     & i3,kd)+rxy23(i1,i2,i3)*vrbr2(i1,i2,i3,kd)+sxy23(i1,i2,i3)*
     & vrbs2(i1,i2,i3,kd)+txy23(i1,i2,i3)*vrbt2(i1,i2,i3,kd)
      vrbxz23(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*vrbrr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*vrbss2(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & tz(i1,i2,i3)*vrbtt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sx(i1,i2,i3))*vrbrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*vrbrt2(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*vrbst2(i1,i2,
     & i3,kd)+rxz23(i1,i2,i3)*vrbr2(i1,i2,i3,kd)+sxz23(i1,i2,i3)*
     & vrbs2(i1,i2,i3,kd)+txz23(i1,i2,i3)*vrbt2(i1,i2,i3,kd)
      vrbyz23(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*vrbrr2(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*vrbss2(i1,i2,i3,kd)+ty(i1,i2,i3)*
     & tz(i1,i2,i3)*vrbtt2(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sy(i1,i2,i3))*vrbrs2(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*vrbrt2(i1,i2,i3,kd)+(sy(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*vrbst2(i1,i2,
     & i3,kd)+ryz23(i1,i2,i3)*vrbr2(i1,i2,i3,kd)+syz23(i1,i2,i3)*
     & vrbs2(i1,i2,i3,kd)+tyz23(i1,i2,i3)*vrbt2(i1,i2,i3,kd)
      vrblaplacian23(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*vrbrr2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)
     & **2+sz(i1,i2,i3)**2)*vrbss2(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(
     & i1,i2,i3)**2+tz(i1,i2,i3)**2)*vrbtt2(i1,i2,i3,kd)+2.*(rx(i1,i2,
     & i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,
     & i2,i3))*vrbrs2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(
     & i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*vrbrt2(i1,i2,
     & i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)
     & +sz(i1,i2,i3)*tz(i1,i2,i3))*vrbst2(i1,i2,i3,kd)+(rxx23(i1,i2,
     & i3)+ryy23(i1,i2,i3)+rzz23(i1,i2,i3))*vrbr2(i1,i2,i3,kd)+(sxx23(
     & i1,i2,i3)+syy23(i1,i2,i3)+szz23(i1,i2,i3))*vrbs2(i1,i2,i3,kd)+(
     & txx23(i1,i2,i3)+tyy23(i1,i2,i3)+tzz23(i1,i2,i3))*vrbt2(i1,i2,
     & i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      vrbx23r(i1,i2,i3,kd)=(vrb(i1+1,i2,i3,kd)-vrb(i1-1,i2,i3,kd))*h12(
     & 0)
      vrby23r(i1,i2,i3,kd)=(vrb(i1,i2+1,i3,kd)-vrb(i1,i2-1,i3,kd))*h12(
     & 1)
      vrbz23r(i1,i2,i3,kd)=(vrb(i1,i2,i3+1,kd)-vrb(i1,i2,i3-1,kd))*h12(
     & 2)
      vrbxx23r(i1,i2,i3,kd)=(-2.*vrb(i1,i2,i3,kd)+(vrb(i1+1,i2,i3,kd)+
     & vrb(i1-1,i2,i3,kd)) )*h22(0)
      vrbyy23r(i1,i2,i3,kd)=(-2.*vrb(i1,i2,i3,kd)+(vrb(i1,i2+1,i3,kd)+
     & vrb(i1,i2-1,i3,kd)) )*h22(1)
      vrbxy23r(i1,i2,i3,kd)=(vrbx23r(i1,i2+1,i3,kd)-vrbx23r(i1,i2-1,i3,
     & kd))*h12(1)
      vrbzz23r(i1,i2,i3,kd)=(-2.*vrb(i1,i2,i3,kd)+(vrb(i1,i2,i3+1,kd)+
     & vrb(i1,i2,i3-1,kd)) )*h22(2)
      vrbxz23r(i1,i2,i3,kd)=(vrbx23r(i1,i2,i3+1,kd)-vrbx23r(i1,i2,i3-1,
     & kd))*h12(2)
      vrbyz23r(i1,i2,i3,kd)=(vrby23r(i1,i2,i3+1,kd)-vrby23r(i1,i2,i3-1,
     & kd))*h12(2)
      vrbx21r(i1,i2,i3,kd)= vrbx23r(i1,i2,i3,kd)
      vrby21r(i1,i2,i3,kd)= vrby23r(i1,i2,i3,kd)
      vrbz21r(i1,i2,i3,kd)= vrbz23r(i1,i2,i3,kd)
      vrbxx21r(i1,i2,i3,kd)= vrbxx23r(i1,i2,i3,kd)
      vrbyy21r(i1,i2,i3,kd)= vrbyy23r(i1,i2,i3,kd)
      vrbzz21r(i1,i2,i3,kd)= vrbzz23r(i1,i2,i3,kd)
      vrbxy21r(i1,i2,i3,kd)= vrbxy23r(i1,i2,i3,kd)
      vrbxz21r(i1,i2,i3,kd)= vrbxz23r(i1,i2,i3,kd)
      vrbyz21r(i1,i2,i3,kd)= vrbyz23r(i1,i2,i3,kd)
      vrblaplacian21r(i1,i2,i3,kd)=vrbxx23r(i1,i2,i3,kd)
      vrbx22r(i1,i2,i3,kd)= vrbx23r(i1,i2,i3,kd)
      vrby22r(i1,i2,i3,kd)= vrby23r(i1,i2,i3,kd)
      vrbz22r(i1,i2,i3,kd)= vrbz23r(i1,i2,i3,kd)
      vrbxx22r(i1,i2,i3,kd)= vrbxx23r(i1,i2,i3,kd)
      vrbyy22r(i1,i2,i3,kd)= vrbyy23r(i1,i2,i3,kd)
      vrbzz22r(i1,i2,i3,kd)= vrbzz23r(i1,i2,i3,kd)
      vrbxy22r(i1,i2,i3,kd)= vrbxy23r(i1,i2,i3,kd)
      vrbxz22r(i1,i2,i3,kd)= vrbxz23r(i1,i2,i3,kd)
      vrbyz22r(i1,i2,i3,kd)= vrbyz23r(i1,i2,i3,kd)
      vrblaplacian22r(i1,i2,i3,kd)=vrbxx23r(i1,i2,i3,kd)+vrbyy23r(i1,
     & i2,i3,kd)
      vrblaplacian23r(i1,i2,i3,kd)=vrbxx23r(i1,i2,i3,kd)+vrbyy23r(i1,
     & i2,i3,kd)+vrbzz23r(i1,i2,i3,kd)
      vrbxxx22r(i1,i2,i3,kd)=(-2.*(vrb(i1+1,i2,i3,kd)-vrb(i1-1,i2,i3,
     & kd))+(vrb(i1+2,i2,i3,kd)-vrb(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
      vrbyyy22r(i1,i2,i3,kd)=(-2.*(vrb(i1,i2+1,i3,kd)-vrb(i1,i2-1,i3,
     & kd))+(vrb(i1,i2+2,i3,kd)-vrb(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
      vrbxxy22r(i1,i2,i3,kd)=( vrbxx22r(i1,i2+1,i3,kd)-vrbxx22r(i1,i2-
     & 1,i3,kd))/(2.*dx(1))
      vrbxyy22r(i1,i2,i3,kd)=( vrbyy22r(i1+1,i2,i3,kd)-vrbyy22r(i1-1,
     & i2,i3,kd))/(2.*dx(0))
      vrbxxxx22r(i1,i2,i3,kd)=(6.*vrb(i1,i2,i3,kd)-4.*(vrb(i1+1,i2,i3,
     & kd)+vrb(i1-1,i2,i3,kd))+(vrb(i1+2,i2,i3,kd)+vrb(i1-2,i2,i3,kd))
     &  )/(dx(0)**4)
      vrbyyyy22r(i1,i2,i3,kd)=(6.*vrb(i1,i2,i3,kd)-4.*(vrb(i1,i2+1,i3,
     & kd)+vrb(i1,i2-1,i3,kd))+(vrb(i1,i2+2,i3,kd)+vrb(i1,i2-2,i3,kd))
     &  )/(dx(1)**4)
      vrbxxyy22r(i1,i2,i3,kd)=( 4.*vrb(i1,i2,i3,kd)     -2.*(vrb(i1+1,
     & i2,i3,kd)+vrb(i1-1,i2,i3,kd)+vrb(i1,i2+1,i3,kd)+vrb(i1,i2-1,i3,
     & kd))   +   (vrb(i1+1,i2+1,i3,kd)+vrb(i1-1,i2+1,i3,kd)+vrb(i1+1,
     & i2-1,i3,kd)+vrb(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      ! 2D laplacian squared = vrb.xxxx + 2 vrb.xxyy + vrb.yyyy
      vrbLapSq22r(i1,i2,i3,kd)= ( 6.*vrb(i1,i2,i3,kd)   - 4.*(vrb(i1+1,
     & i2,i3,kd)+vrb(i1-1,i2,i3,kd))    +(vrb(i1+2,i2,i3,kd)+vrb(i1-2,
     & i2,i3,kd)) )/(dx(0)**4) +( 6.*vrb(i1,i2,i3,kd)    -4.*(vrb(i1,
     & i2+1,i3,kd)+vrb(i1,i2-1,i3,kd))    +(vrb(i1,i2+2,i3,kd)+vrb(i1,
     & i2-2,i3,kd)) )/(dx(1)**4)  +( 8.*vrb(i1,i2,i3,kd)     -4.*(vrb(
     & i1+1,i2,i3,kd)+vrb(i1-1,i2,i3,kd)+vrb(i1,i2+1,i3,kd)+vrb(i1,i2-
     & 1,i3,kd))   +2.*(vrb(i1+1,i2+1,i3,kd)+vrb(i1-1,i2+1,i3,kd)+vrb(
     & i1+1,i2-1,i3,kd)+vrb(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      vrbxxx23r(i1,i2,i3,kd)=(-2.*(vrb(i1+1,i2,i3,kd)-vrb(i1-1,i2,i3,
     & kd))+(vrb(i1+2,i2,i3,kd)-vrb(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
      vrbyyy23r(i1,i2,i3,kd)=(-2.*(vrb(i1,i2+1,i3,kd)-vrb(i1,i2-1,i3,
     & kd))+(vrb(i1,i2+2,i3,kd)-vrb(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
      vrbzzz23r(i1,i2,i3,kd)=(-2.*(vrb(i1,i2,i3+1,kd)-vrb(i1,i2,i3-1,
     & kd))+(vrb(i1,i2,i3+2,kd)-vrb(i1,i2,i3-2,kd)) )*h22(1)*h12(2)
      vrbxxy23r(i1,i2,i3,kd)=( vrbxx22r(i1,i2+1,i3,kd)-vrbxx22r(i1,i2-
     & 1,i3,kd))/(2.*dx(1))
      vrbxyy23r(i1,i2,i3,kd)=( vrbyy22r(i1+1,i2,i3,kd)-vrbyy22r(i1-1,
     & i2,i3,kd))/(2.*dx(0))
      vrbxxz23r(i1,i2,i3,kd)=( vrbxx22r(i1,i2,i3+1,kd)-vrbxx22r(i1,i2,
     & i3-1,kd))/(2.*dx(2))
      vrbyyz23r(i1,i2,i3,kd)=( vrbyy22r(i1,i2,i3+1,kd)-vrbyy22r(i1,i2,
     & i3-1,kd))/(2.*dx(2))
      vrbxzz23r(i1,i2,i3,kd)=( vrbzz22r(i1+1,i2,i3,kd)-vrbzz22r(i1-1,
     & i2,i3,kd))/(2.*dx(0))
      vrbyzz23r(i1,i2,i3,kd)=( vrbzz22r(i1,i2+1,i3,kd)-vrbzz22r(i1,i2-
     & 1,i3,kd))/(2.*dx(1))
      vrbxxxx23r(i1,i2,i3,kd)=(6.*vrb(i1,i2,i3,kd)-4.*(vrb(i1+1,i2,i3,
     & kd)+vrb(i1-1,i2,i3,kd))+(vrb(i1+2,i2,i3,kd)+vrb(i1-2,i2,i3,kd))
     &  )/(dx(0)**4)
      vrbyyyy23r(i1,i2,i3,kd)=(6.*vrb(i1,i2,i3,kd)-4.*(vrb(i1,i2+1,i3,
     & kd)+vrb(i1,i2-1,i3,kd))+(vrb(i1,i2+2,i3,kd)+vrb(i1,i2-2,i3,kd))
     &  )/(dx(1)**4)
      vrbzzzz23r(i1,i2,i3,kd)=(6.*vrb(i1,i2,i3,kd)-4.*(vrb(i1,i2,i3+1,
     & kd)+vrb(i1,i2,i3-1,kd))+(vrb(i1,i2,i3+2,kd)+vrb(i1,i2,i3-2,kd))
     &  )/(dx(2)**4)
      vrbxxyy23r(i1,i2,i3,kd)=( 4.*vrb(i1,i2,i3,kd)     -2.*(vrb(i1+1,
     & i2,i3,kd)+vrb(i1-1,i2,i3,kd)+vrb(i1,i2+1,i3,kd)+vrb(i1,i2-1,i3,
     & kd))   +   (vrb(i1+1,i2+1,i3,kd)+vrb(i1-1,i2+1,i3,kd)+vrb(i1+1,
     & i2-1,i3,kd)+vrb(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      vrbxxzz23r(i1,i2,i3,kd)=( 4.*vrb(i1,i2,i3,kd)     -2.*(vrb(i1+1,
     & i2,i3,kd)+vrb(i1-1,i2,i3,kd)+vrb(i1,i2,i3+1,kd)+vrb(i1,i2,i3-1,
     & kd))   +   (vrb(i1+1,i2,i3+1,kd)+vrb(i1-1,i2,i3+1,kd)+vrb(i1+1,
     & i2,i3-1,kd)+vrb(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)
      vrbyyzz23r(i1,i2,i3,kd)=( 4.*vrb(i1,i2,i3,kd)     -2.*(vrb(i1,i2+
     & 1,i3,kd)  +vrb(i1,i2-1,i3,kd)+  vrb(i1,i2  ,i3+1,kd)+vrb(i1,i2 
     &  ,i3-1,kd))   +   (vrb(i1,i2+1,i3+1,kd)+vrb(i1,i2-1,i3+1,kd)+
     & vrb(i1,i2+1,i3-1,kd)+vrb(i1,i2-1,i3-1,kd)) )/(dx(1)**2*dx(2)**
     & 2)
      ! 3D laplacian squared = vrb.xxxx + vrb.yyyy + vrb.zzzz + 2 (vrb.xxyy + vrb.xxzz + vrb.yyzz )
      vrbLapSq23r(i1,i2,i3,kd)= ( 6.*vrb(i1,i2,i3,kd)   - 4.*(vrb(i1+1,
     & i2,i3,kd)+vrb(i1-1,i2,i3,kd))    +(vrb(i1+2,i2,i3,kd)+vrb(i1-2,
     & i2,i3,kd)) )/(dx(0)**4) +( 6.*vrb(i1,i2,i3,kd)    -4.*(vrb(i1,
     & i2+1,i3,kd)+vrb(i1,i2-1,i3,kd))    +(vrb(i1,i2+2,i3,kd)+vrb(i1,
     & i2-2,i3,kd)) )/(dx(1)**4)  +( 6.*vrb(i1,i2,i3,kd)    -4.*(vrb(
     & i1,i2,i3+1,kd)+vrb(i1,i2,i3-1,kd))    +(vrb(i1,i2,i3+2,kd)+vrb(
     & i1,i2,i3-2,kd)) )/(dx(2)**4)  +( 8.*vrb(i1,i2,i3,kd)     -4.*(
     & vrb(i1+1,i2,i3,kd)  +vrb(i1-1,i2,i3,kd)  +vrb(i1  ,i2+1,i3,kd)+
     & vrb(i1  ,i2-1,i3,kd))   +2.*(vrb(i1+1,i2+1,i3,kd)+vrb(i1-1,i2+
     & 1,i3,kd)+vrb(i1+1,i2-1,i3,kd)+vrb(i1-1,i2-1,i3,kd)) )/(dx(0)**
     & 2*dx(1)**2)+( 8.*vrb(i1,i2,i3,kd)     -4.*(vrb(i1+1,i2,i3,kd)  
     & +vrb(i1-1,i2,i3,kd)  +vrb(i1  ,i2,i3+1,kd)+vrb(i1  ,i2,i3-1,kd)
     & )   +2.*(vrb(i1+1,i2,i3+1,kd)+vrb(i1-1,i2,i3+1,kd)+vrb(i1+1,i2,
     & i3-1,kd)+vrb(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)+( 8.*vrb(
     & i1,i2,i3,kd)     -4.*(vrb(i1,i2+1,i3,kd)  +vrb(i1,i2-1,i3,kd)  
     & +vrb(i1,i2  ,i3+1,kd)+vrb(i1,i2  ,i3-1,kd))   +2.*(vrb(i1,i2+1,
     & i3+1,kd)+vrb(i1,i2-1,i3+1,kd)+vrb(i1,i2+1,i3-1,kd)+vrb(i1,i2-1,
     & i3-1,kd)) )/(dx(1)**2*dx(2)**2)
      vrbmr2(i1,i2,i3,kd)=(vrbm(i1+1,i2,i3,kd)-vrbm(i1-1,i2,i3,kd))*
     & d12(0)
      vrbms2(i1,i2,i3,kd)=(vrbm(i1,i2+1,i3,kd)-vrbm(i1,i2-1,i3,kd))*
     & d12(1)
      vrbmt2(i1,i2,i3,kd)=(vrbm(i1,i2,i3+1,kd)-vrbm(i1,i2,i3-1,kd))*
     & d12(2)
      vrbmrr2(i1,i2,i3,kd)=(-2.*vrbm(i1,i2,i3,kd)+(vrbm(i1+1,i2,i3,kd)+
     & vrbm(i1-1,i2,i3,kd)) )*d22(0)
      vrbmss2(i1,i2,i3,kd)=(-2.*vrbm(i1,i2,i3,kd)+(vrbm(i1,i2+1,i3,kd)+
     & vrbm(i1,i2-1,i3,kd)) )*d22(1)
      vrbmrs2(i1,i2,i3,kd)=(vrbmr2(i1,i2+1,i3,kd)-vrbmr2(i1,i2-1,i3,kd)
     & )*d12(1)
      vrbmtt2(i1,i2,i3,kd)=(-2.*vrbm(i1,i2,i3,kd)+(vrbm(i1,i2,i3+1,kd)+
     & vrbm(i1,i2,i3-1,kd)) )*d22(2)
      vrbmrt2(i1,i2,i3,kd)=(vrbmr2(i1,i2,i3+1,kd)-vrbmr2(i1,i2,i3-1,kd)
     & )*d12(2)
      vrbmst2(i1,i2,i3,kd)=(vrbms2(i1,i2,i3+1,kd)-vrbms2(i1,i2,i3-1,kd)
     & )*d12(2)
      vrbmrrr2(i1,i2,i3,kd)=(-2.*(vrbm(i1+1,i2,i3,kd)-vrbm(i1-1,i2,i3,
     & kd))+(vrbm(i1+2,i2,i3,kd)-vrbm(i1-2,i2,i3,kd)) )*d22(0)*d12(0)
      vrbmsss2(i1,i2,i3,kd)=(-2.*(vrbm(i1,i2+1,i3,kd)-vrbm(i1,i2-1,i3,
     & kd))+(vrbm(i1,i2+2,i3,kd)-vrbm(i1,i2-2,i3,kd)) )*d22(1)*d12(1)
      vrbmttt2(i1,i2,i3,kd)=(-2.*(vrbm(i1,i2,i3+1,kd)-vrbm(i1,i2,i3-1,
     & kd))+(vrbm(i1,i2,i3+2,kd)-vrbm(i1,i2,i3-2,kd)) )*d22(2)*d12(2)
      vrbmx21(i1,i2,i3,kd)= rx(i1,i2,i3)*vrbmr2(i1,i2,i3,kd)
      vrbmy21(i1,i2,i3,kd)=0
      vrbmz21(i1,i2,i3,kd)=0
      vrbmx22(i1,i2,i3,kd)= rx(i1,i2,i3)*vrbmr2(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*vrbms2(i1,i2,i3,kd)
      vrbmy22(i1,i2,i3,kd)= ry(i1,i2,i3)*vrbmr2(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*vrbms2(i1,i2,i3,kd)
      vrbmz22(i1,i2,i3,kd)=0
      vrbmx23(i1,i2,i3,kd)=rx(i1,i2,i3)*vrbmr2(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*vrbms2(i1,i2,i3,kd)+tx(i1,i2,i3)*vrbmt2(i1,i2,i3,kd)
      vrbmy23(i1,i2,i3,kd)=ry(i1,i2,i3)*vrbmr2(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*vrbms2(i1,i2,i3,kd)+ty(i1,i2,i3)*vrbmt2(i1,i2,i3,kd)
      vrbmz23(i1,i2,i3,kd)=rz(i1,i2,i3)*vrbmr2(i1,i2,i3,kd)+sz(i1,i2,
     & i3)*vrbms2(i1,i2,i3,kd)+tz(i1,i2,i3)*vrbmt2(i1,i2,i3,kd)
      vrbmxx21(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vrbmrr2(i1,i2,i3,kd)+(
     & rxx22(i1,i2,i3))*vrbmr2(i1,i2,i3,kd)
      vrbmyy21(i1,i2,i3,kd)=0
      vrbmxy21(i1,i2,i3,kd)=0
      vrbmxz21(i1,i2,i3,kd)=0
      vrbmyz21(i1,i2,i3,kd)=0
      vrbmzz21(i1,i2,i3,kd)=0
      vrbmlaplacian21(i1,i2,i3,kd)=vrbmxx21(i1,i2,i3,kd)
      vrbmxx22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vrbmrr2(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*vrbmrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*vrbmss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3))*vrbmr2(i1,i2,i3,kd)+(
     & sxx22(i1,i2,i3))*vrbms2(i1,i2,i3,kd)
      vrbmyy22(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*vrbmrr2(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*vrbmrs2(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*vrbmss2(i1,i2,i3,kd)+(ryy22(i1,i2,i3))*vrbmr2(i1,i2,i3,kd)+(
     & syy22(i1,i2,i3))*vrbms2(i1,i2,i3,kd)
      vrbmxy22(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vrbmrr2(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & vrbmrs2(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vrbmss2(i1,i2,
     & i3,kd)+rxy22(i1,i2,i3)*vrbmr2(i1,i2,i3,kd)+sxy22(i1,i2,i3)*
     & vrbms2(i1,i2,i3,kd)
      vrbmxz22(i1,i2,i3,kd)=0
      vrbmyz22(i1,i2,i3,kd)=0
      vrbmzz22(i1,i2,i3,kd)=0
      vrbmlaplacian22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & vrbmrr2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,
     & i3)*sy(i1,i2,i3))*vrbmrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,
     & i2,i3)**2)*vrbmss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3)+ryy22(i1,i2,
     & i3))*vrbmr2(i1,i2,i3,kd)+(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*
     & vrbms2(i1,i2,i3,kd)
      vrbmxx23(i1,i2,i3,kd)=rx(i1,i2,i3)**2*vrbmrr2(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*vrbmss2(i1,i2,i3,kd)+tx(i1,i2,i3)**2*vrbmtt2(i1,i2,
     & i3,kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*vrbmrs2(i1,i2,i3,kd)+2.*rx(
     & i1,i2,i3)*tx(i1,i2,i3)*vrbmrt2(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(
     & i1,i2,i3)*vrbmst2(i1,i2,i3,kd)+rxx23(i1,i2,i3)*vrbmr2(i1,i2,i3,
     & kd)+sxx23(i1,i2,i3)*vrbms2(i1,i2,i3,kd)+txx23(i1,i2,i3)*vrbmt2(
     & i1,i2,i3,kd)
      vrbmyy23(i1,i2,i3,kd)=ry(i1,i2,i3)**2*vrbmrr2(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*vrbmss2(i1,i2,i3,kd)+ty(i1,i2,i3)**2*vrbmtt2(i1,i2,
     & i3,kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*vrbmrs2(i1,i2,i3,kd)+2.*ry(
     & i1,i2,i3)*ty(i1,i2,i3)*vrbmrt2(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(
     & i1,i2,i3)*vrbmst2(i1,i2,i3,kd)+ryy23(i1,i2,i3)*vrbmr2(i1,i2,i3,
     & kd)+syy23(i1,i2,i3)*vrbms2(i1,i2,i3,kd)+tyy23(i1,i2,i3)*vrbmt2(
     & i1,i2,i3,kd)
      vrbmzz23(i1,i2,i3,kd)=rz(i1,i2,i3)**2*vrbmrr2(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*vrbmss2(i1,i2,i3,kd)+tz(i1,i2,i3)**2*vrbmtt2(i1,i2,
     & i3,kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*vrbmrs2(i1,i2,i3,kd)+2.*rz(
     & i1,i2,i3)*tz(i1,i2,i3)*vrbmrt2(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(
     & i1,i2,i3)*vrbmst2(i1,i2,i3,kd)+rzz23(i1,i2,i3)*vrbmr2(i1,i2,i3,
     & kd)+szz23(i1,i2,i3)*vrbms2(i1,i2,i3,kd)+tzz23(i1,i2,i3)*vrbmt2(
     & i1,i2,i3,kd)
      vrbmxy23(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vrbmrr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vrbmss2(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *ty(i1,i2,i3)*vrbmtt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+
     & ry(i1,i2,i3)*sx(i1,i2,i3))*vrbmrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & ty(i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*vrbmrt2(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*vrbmst2(
     & i1,i2,i3,kd)+rxy23(i1,i2,i3)*vrbmr2(i1,i2,i3,kd)+sxy23(i1,i2,
     & i3)*vrbms2(i1,i2,i3,kd)+txy23(i1,i2,i3)*vrbmt2(i1,i2,i3,kd)
      vrbmxz23(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*vrbmrr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*vrbmss2(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *tz(i1,i2,i3)*vrbmtt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sx(i1,i2,i3))*vrbmrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*vrbmrt2(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*vrbmst2(
     & i1,i2,i3,kd)+rxz23(i1,i2,i3)*vrbmr2(i1,i2,i3,kd)+sxz23(i1,i2,
     & i3)*vrbms2(i1,i2,i3,kd)+txz23(i1,i2,i3)*vrbmt2(i1,i2,i3,kd)
      vrbmyz23(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*vrbmrr2(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*vrbmss2(i1,i2,i3,kd)+ty(i1,i2,i3)
     & *tz(i1,i2,i3)*vrbmtt2(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sy(i1,i2,i3))*vrbmrs2(i1,i2,i3,kd)+(ry(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*vrbmrt2(i1,i2,i3,kd)+(
     & sy(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*vrbmst2(
     & i1,i2,i3,kd)+ryz23(i1,i2,i3)*vrbmr2(i1,i2,i3,kd)+syz23(i1,i2,
     & i3)*vrbms2(i1,i2,i3,kd)+tyz23(i1,i2,i3)*vrbmt2(i1,i2,i3,kd)
      vrbmlaplacian23(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*vrbmrr2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2+sz(i1,i2,i3)**2)*vrbmss2(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+
     & ty(i1,i2,i3)**2+tz(i1,i2,i3)**2)*vrbmtt2(i1,i2,i3,kd)+2.*(rx(
     & i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*
     & sz(i1,i2,i3))*vrbmrs2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,
     & i3)+ ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*
     & vrbmrt2(i1,i2,i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,
     & i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3))*vrbmst2(i1,i2,i3,
     & kd)+(rxx23(i1,i2,i3)+ryy23(i1,i2,i3)+rzz23(i1,i2,i3))*vrbmr2(
     & i1,i2,i3,kd)+(sxx23(i1,i2,i3)+syy23(i1,i2,i3)+szz23(i1,i2,i3))*
     & vrbms2(i1,i2,i3,kd)+(txx23(i1,i2,i3)+tyy23(i1,i2,i3)+tzz23(i1,
     & i2,i3))*vrbmt2(i1,i2,i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      vrbmx23r(i1,i2,i3,kd)=(vrbm(i1+1,i2,i3,kd)-vrbm(i1-1,i2,i3,kd))*
     & h12(0)
      vrbmy23r(i1,i2,i3,kd)=(vrbm(i1,i2+1,i3,kd)-vrbm(i1,i2-1,i3,kd))*
     & h12(1)
      vrbmz23r(i1,i2,i3,kd)=(vrbm(i1,i2,i3+1,kd)-vrbm(i1,i2,i3-1,kd))*
     & h12(2)
      vrbmxx23r(i1,i2,i3,kd)=(-2.*vrbm(i1,i2,i3,kd)+(vrbm(i1+1,i2,i3,
     & kd)+vrbm(i1-1,i2,i3,kd)) )*h22(0)
      vrbmyy23r(i1,i2,i3,kd)=(-2.*vrbm(i1,i2,i3,kd)+(vrbm(i1,i2+1,i3,
     & kd)+vrbm(i1,i2-1,i3,kd)) )*h22(1)
      vrbmxy23r(i1,i2,i3,kd)=(vrbmx23r(i1,i2+1,i3,kd)-vrbmx23r(i1,i2-1,
     & i3,kd))*h12(1)
      vrbmzz23r(i1,i2,i3,kd)=(-2.*vrbm(i1,i2,i3,kd)+(vrbm(i1,i2,i3+1,
     & kd)+vrbm(i1,i2,i3-1,kd)) )*h22(2)
      vrbmxz23r(i1,i2,i3,kd)=(vrbmx23r(i1,i2,i3+1,kd)-vrbmx23r(i1,i2,
     & i3-1,kd))*h12(2)
      vrbmyz23r(i1,i2,i3,kd)=(vrbmy23r(i1,i2,i3+1,kd)-vrbmy23r(i1,i2,
     & i3-1,kd))*h12(2)
      vrbmx21r(i1,i2,i3,kd)= vrbmx23r(i1,i2,i3,kd)
      vrbmy21r(i1,i2,i3,kd)= vrbmy23r(i1,i2,i3,kd)
      vrbmz21r(i1,i2,i3,kd)= vrbmz23r(i1,i2,i3,kd)
      vrbmxx21r(i1,i2,i3,kd)= vrbmxx23r(i1,i2,i3,kd)
      vrbmyy21r(i1,i2,i3,kd)= vrbmyy23r(i1,i2,i3,kd)
      vrbmzz21r(i1,i2,i3,kd)= vrbmzz23r(i1,i2,i3,kd)
      vrbmxy21r(i1,i2,i3,kd)= vrbmxy23r(i1,i2,i3,kd)
      vrbmxz21r(i1,i2,i3,kd)= vrbmxz23r(i1,i2,i3,kd)
      vrbmyz21r(i1,i2,i3,kd)= vrbmyz23r(i1,i2,i3,kd)
      vrbmlaplacian21r(i1,i2,i3,kd)=vrbmxx23r(i1,i2,i3,kd)
      vrbmx22r(i1,i2,i3,kd)= vrbmx23r(i1,i2,i3,kd)
      vrbmy22r(i1,i2,i3,kd)= vrbmy23r(i1,i2,i3,kd)
      vrbmz22r(i1,i2,i3,kd)= vrbmz23r(i1,i2,i3,kd)
      vrbmxx22r(i1,i2,i3,kd)= vrbmxx23r(i1,i2,i3,kd)
      vrbmyy22r(i1,i2,i3,kd)= vrbmyy23r(i1,i2,i3,kd)
      vrbmzz22r(i1,i2,i3,kd)= vrbmzz23r(i1,i2,i3,kd)
      vrbmxy22r(i1,i2,i3,kd)= vrbmxy23r(i1,i2,i3,kd)
      vrbmxz22r(i1,i2,i3,kd)= vrbmxz23r(i1,i2,i3,kd)
      vrbmyz22r(i1,i2,i3,kd)= vrbmyz23r(i1,i2,i3,kd)
      vrbmlaplacian22r(i1,i2,i3,kd)=vrbmxx23r(i1,i2,i3,kd)+vrbmyy23r(
     & i1,i2,i3,kd)
      vrbmlaplacian23r(i1,i2,i3,kd)=vrbmxx23r(i1,i2,i3,kd)+vrbmyy23r(
     & i1,i2,i3,kd)+vrbmzz23r(i1,i2,i3,kd)
      vrbmxxx22r(i1,i2,i3,kd)=(-2.*(vrbm(i1+1,i2,i3,kd)-vrbm(i1-1,i2,
     & i3,kd))+(vrbm(i1+2,i2,i3,kd)-vrbm(i1-2,i2,i3,kd)) )*h22(0)*h12(
     & 0)
      vrbmyyy22r(i1,i2,i3,kd)=(-2.*(vrbm(i1,i2+1,i3,kd)-vrbm(i1,i2-1,
     & i3,kd))+(vrbm(i1,i2+2,i3,kd)-vrbm(i1,i2-2,i3,kd)) )*h22(1)*h12(
     & 1)
      vrbmxxy22r(i1,i2,i3,kd)=( vrbmxx22r(i1,i2+1,i3,kd)-vrbmxx22r(i1,
     & i2-1,i3,kd))/(2.*dx(1))
      vrbmxyy22r(i1,i2,i3,kd)=( vrbmyy22r(i1+1,i2,i3,kd)-vrbmyy22r(i1-
     & 1,i2,i3,kd))/(2.*dx(0))
      vrbmxxxx22r(i1,i2,i3,kd)=(6.*vrbm(i1,i2,i3,kd)-4.*(vrbm(i1+1,i2,
     & i3,kd)+vrbm(i1-1,i2,i3,kd))+(vrbm(i1+2,i2,i3,kd)+vrbm(i1-2,i2,
     & i3,kd)) )/(dx(0)**4)
      vrbmyyyy22r(i1,i2,i3,kd)=(6.*vrbm(i1,i2,i3,kd)-4.*(vrbm(i1,i2+1,
     & i3,kd)+vrbm(i1,i2-1,i3,kd))+(vrbm(i1,i2+2,i3,kd)+vrbm(i1,i2-2,
     & i3,kd)) )/(dx(1)**4)
      vrbmxxyy22r(i1,i2,i3,kd)=( 4.*vrbm(i1,i2,i3,kd)     -2.*(vrbm(i1+
     & 1,i2,i3,kd)+vrbm(i1-1,i2,i3,kd)+vrbm(i1,i2+1,i3,kd)+vrbm(i1,i2-
     & 1,i3,kd))   +   (vrbm(i1+1,i2+1,i3,kd)+vrbm(i1-1,i2+1,i3,kd)+
     & vrbm(i1+1,i2-1,i3,kd)+vrbm(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)*
     & *2)
      ! 2D laplacian squared = vrbm.xxxx + 2 vrbm.xxyy + vrbm.yyyy
      vrbmLapSq22r(i1,i2,i3,kd)= ( 6.*vrbm(i1,i2,i3,kd)   - 4.*(vrbm(
     & i1+1,i2,i3,kd)+vrbm(i1-1,i2,i3,kd))    +(vrbm(i1+2,i2,i3,kd)+
     & vrbm(i1-2,i2,i3,kd)) )/(dx(0)**4) +( 6.*vrbm(i1,i2,i3,kd)    -
     & 4.*(vrbm(i1,i2+1,i3,kd)+vrbm(i1,i2-1,i3,kd))    +(vrbm(i1,i2+2,
     & i3,kd)+vrbm(i1,i2-2,i3,kd)) )/(dx(1)**4)  +( 8.*vrbm(i1,i2,i3,
     & kd)     -4.*(vrbm(i1+1,i2,i3,kd)+vrbm(i1-1,i2,i3,kd)+vrbm(i1,
     & i2+1,i3,kd)+vrbm(i1,i2-1,i3,kd))   +2.*(vrbm(i1+1,i2+1,i3,kd)+
     & vrbm(i1-1,i2+1,i3,kd)+vrbm(i1+1,i2-1,i3,kd)+vrbm(i1-1,i2-1,i3,
     & kd)) )/(dx(0)**2*dx(1)**2)
      vrbmxxx23r(i1,i2,i3,kd)=(-2.*(vrbm(i1+1,i2,i3,kd)-vrbm(i1-1,i2,
     & i3,kd))+(vrbm(i1+2,i2,i3,kd)-vrbm(i1-2,i2,i3,kd)) )*h22(0)*h12(
     & 0)
      vrbmyyy23r(i1,i2,i3,kd)=(-2.*(vrbm(i1,i2+1,i3,kd)-vrbm(i1,i2-1,
     & i3,kd))+(vrbm(i1,i2+2,i3,kd)-vrbm(i1,i2-2,i3,kd)) )*h22(1)*h12(
     & 1)
      vrbmzzz23r(i1,i2,i3,kd)=(-2.*(vrbm(i1,i2,i3+1,kd)-vrbm(i1,i2,i3-
     & 1,kd))+(vrbm(i1,i2,i3+2,kd)-vrbm(i1,i2,i3-2,kd)) )*h22(1)*h12(
     & 2)
      vrbmxxy23r(i1,i2,i3,kd)=( vrbmxx22r(i1,i2+1,i3,kd)-vrbmxx22r(i1,
     & i2-1,i3,kd))/(2.*dx(1))
      vrbmxyy23r(i1,i2,i3,kd)=( vrbmyy22r(i1+1,i2,i3,kd)-vrbmyy22r(i1-
     & 1,i2,i3,kd))/(2.*dx(0))
      vrbmxxz23r(i1,i2,i3,kd)=( vrbmxx22r(i1,i2,i3+1,kd)-vrbmxx22r(i1,
     & i2,i3-1,kd))/(2.*dx(2))
      vrbmyyz23r(i1,i2,i3,kd)=( vrbmyy22r(i1,i2,i3+1,kd)-vrbmyy22r(i1,
     & i2,i3-1,kd))/(2.*dx(2))
      vrbmxzz23r(i1,i2,i3,kd)=( vrbmzz22r(i1+1,i2,i3,kd)-vrbmzz22r(i1-
     & 1,i2,i3,kd))/(2.*dx(0))
      vrbmyzz23r(i1,i2,i3,kd)=( vrbmzz22r(i1,i2+1,i3,kd)-vrbmzz22r(i1,
     & i2-1,i3,kd))/(2.*dx(1))
      vrbmxxxx23r(i1,i2,i3,kd)=(6.*vrbm(i1,i2,i3,kd)-4.*(vrbm(i1+1,i2,
     & i3,kd)+vrbm(i1-1,i2,i3,kd))+(vrbm(i1+2,i2,i3,kd)+vrbm(i1-2,i2,
     & i3,kd)) )/(dx(0)**4)
      vrbmyyyy23r(i1,i2,i3,kd)=(6.*vrbm(i1,i2,i3,kd)-4.*(vrbm(i1,i2+1,
     & i3,kd)+vrbm(i1,i2-1,i3,kd))+(vrbm(i1,i2+2,i3,kd)+vrbm(i1,i2-2,
     & i3,kd)) )/(dx(1)**4)
      vrbmzzzz23r(i1,i2,i3,kd)=(6.*vrbm(i1,i2,i3,kd)-4.*(vrbm(i1,i2,i3+
     & 1,kd)+vrbm(i1,i2,i3-1,kd))+(vrbm(i1,i2,i3+2,kd)+vrbm(i1,i2,i3-
     & 2,kd)) )/(dx(2)**4)
      vrbmxxyy23r(i1,i2,i3,kd)=( 4.*vrbm(i1,i2,i3,kd)     -2.*(vrbm(i1+
     & 1,i2,i3,kd)+vrbm(i1-1,i2,i3,kd)+vrbm(i1,i2+1,i3,kd)+vrbm(i1,i2-
     & 1,i3,kd))   +   (vrbm(i1+1,i2+1,i3,kd)+vrbm(i1-1,i2+1,i3,kd)+
     & vrbm(i1+1,i2-1,i3,kd)+vrbm(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)*
     & *2)
      vrbmxxzz23r(i1,i2,i3,kd)=( 4.*vrbm(i1,i2,i3,kd)     -2.*(vrbm(i1+
     & 1,i2,i3,kd)+vrbm(i1-1,i2,i3,kd)+vrbm(i1,i2,i3+1,kd)+vrbm(i1,i2,
     & i3-1,kd))   +   (vrbm(i1+1,i2,i3+1,kd)+vrbm(i1-1,i2,i3+1,kd)+
     & vrbm(i1+1,i2,i3-1,kd)+vrbm(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)*
     & *2)
      vrbmyyzz23r(i1,i2,i3,kd)=( 4.*vrbm(i1,i2,i3,kd)     -2.*(vrbm(i1,
     & i2+1,i3,kd)  +vrbm(i1,i2-1,i3,kd)+  vrbm(i1,i2  ,i3+1,kd)+vrbm(
     & i1,i2  ,i3-1,kd))   +   (vrbm(i1,i2+1,i3+1,kd)+vrbm(i1,i2-1,i3+
     & 1,kd)+vrbm(i1,i2+1,i3-1,kd)+vrbm(i1,i2-1,i3-1,kd)) )/(dx(1)**2*
     & dx(2)**2)
      ! 3D laplacian squared = vrbm.xxxx + vrbm.yyyy + vrbm.zzzz + 2 (vrbm.xxyy + vrbm.xxzz + vrbm.yyzz )
      vrbmLapSq23r(i1,i2,i3,kd)= ( 6.*vrbm(i1,i2,i3,kd)   - 4.*(vrbm(
     & i1+1,i2,i3,kd)+vrbm(i1-1,i2,i3,kd))    +(vrbm(i1+2,i2,i3,kd)+
     & vrbm(i1-2,i2,i3,kd)) )/(dx(0)**4) +( 6.*vrbm(i1,i2,i3,kd)    -
     & 4.*(vrbm(i1,i2+1,i3,kd)+vrbm(i1,i2-1,i3,kd))    +(vrbm(i1,i2+2,
     & i3,kd)+vrbm(i1,i2-2,i3,kd)) )/(dx(1)**4)  +( 6.*vrbm(i1,i2,i3,
     & kd)    -4.*(vrbm(i1,i2,i3+1,kd)+vrbm(i1,i2,i3-1,kd))    +(vrbm(
     & i1,i2,i3+2,kd)+vrbm(i1,i2,i3-2,kd)) )/(dx(2)**4)  +( 8.*vrbm(
     & i1,i2,i3,kd)     -4.*(vrbm(i1+1,i2,i3,kd)  +vrbm(i1-1,i2,i3,kd)
     &   +vrbm(i1  ,i2+1,i3,kd)+vrbm(i1  ,i2-1,i3,kd))   +2.*(vrbm(i1+
     & 1,i2+1,i3,kd)+vrbm(i1-1,i2+1,i3,kd)+vrbm(i1+1,i2-1,i3,kd)+vrbm(
     & i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)+( 8.*vrbm(i1,i2,i3,kd) 
     &     -4.*(vrbm(i1+1,i2,i3,kd)  +vrbm(i1-1,i2,i3,kd)  +vrbm(i1  ,
     & i2,i3+1,kd)+vrbm(i1  ,i2,i3-1,kd))   +2.*(vrbm(i1+1,i2,i3+1,kd)
     & +vrbm(i1-1,i2,i3+1,kd)+vrbm(i1+1,i2,i3-1,kd)+vrbm(i1-1,i2,i3-1,
     & kd)) )/(dx(0)**2*dx(2)**2)+( 8.*vrbm(i1,i2,i3,kd)     -4.*(
     & vrbm(i1,i2+1,i3,kd)  +vrbm(i1,i2-1,i3,kd)  +vrbm(i1,i2  ,i3+1,
     & kd)+vrbm(i1,i2  ,i3-1,kd))   +2.*(vrbm(i1,i2+1,i3+1,kd)+vrbm(
     & i1,i2-1,i3+1,kd)+vrbm(i1,i2+1,i3-1,kd)+vrbm(i1,i2-1,i3-1,kd)) )
     & /(dx(1)**2*dx(2)**2)
      wrbr2(i1,i2,i3,kd)=(wrb(i1+1,i2,i3,kd)-wrb(i1-1,i2,i3,kd))*d12(0)
      wrbs2(i1,i2,i3,kd)=(wrb(i1,i2+1,i3,kd)-wrb(i1,i2-1,i3,kd))*d12(1)
      wrbt2(i1,i2,i3,kd)=(wrb(i1,i2,i3+1,kd)-wrb(i1,i2,i3-1,kd))*d12(2)
      wrbrr2(i1,i2,i3,kd)=(-2.*wrb(i1,i2,i3,kd)+(wrb(i1+1,i2,i3,kd)+
     & wrb(i1-1,i2,i3,kd)) )*d22(0)
      wrbss2(i1,i2,i3,kd)=(-2.*wrb(i1,i2,i3,kd)+(wrb(i1,i2+1,i3,kd)+
     & wrb(i1,i2-1,i3,kd)) )*d22(1)
      wrbrs2(i1,i2,i3,kd)=(wrbr2(i1,i2+1,i3,kd)-wrbr2(i1,i2-1,i3,kd))*
     & d12(1)
      wrbtt2(i1,i2,i3,kd)=(-2.*wrb(i1,i2,i3,kd)+(wrb(i1,i2,i3+1,kd)+
     & wrb(i1,i2,i3-1,kd)) )*d22(2)
      wrbrt2(i1,i2,i3,kd)=(wrbr2(i1,i2,i3+1,kd)-wrbr2(i1,i2,i3-1,kd))*
     & d12(2)
      wrbst2(i1,i2,i3,kd)=(wrbs2(i1,i2,i3+1,kd)-wrbs2(i1,i2,i3-1,kd))*
     & d12(2)
      wrbrrr2(i1,i2,i3,kd)=(-2.*(wrb(i1+1,i2,i3,kd)-wrb(i1-1,i2,i3,kd))
     & +(wrb(i1+2,i2,i3,kd)-wrb(i1-2,i2,i3,kd)) )*d22(0)*d12(0)
      wrbsss2(i1,i2,i3,kd)=(-2.*(wrb(i1,i2+1,i3,kd)-wrb(i1,i2-1,i3,kd))
     & +(wrb(i1,i2+2,i3,kd)-wrb(i1,i2-2,i3,kd)) )*d22(1)*d12(1)
      wrbttt2(i1,i2,i3,kd)=(-2.*(wrb(i1,i2,i3+1,kd)-wrb(i1,i2,i3-1,kd))
     & +(wrb(i1,i2,i3+2,kd)-wrb(i1,i2,i3-2,kd)) )*d22(2)*d12(2)
      wrbx21(i1,i2,i3,kd)= rx(i1,i2,i3)*wrbr2(i1,i2,i3,kd)
      wrby21(i1,i2,i3,kd)=0
      wrbz21(i1,i2,i3,kd)=0
      wrbx22(i1,i2,i3,kd)= rx(i1,i2,i3)*wrbr2(i1,i2,i3,kd)+sx(i1,i2,i3)
     & *wrbs2(i1,i2,i3,kd)
      wrby22(i1,i2,i3,kd)= ry(i1,i2,i3)*wrbr2(i1,i2,i3,kd)+sy(i1,i2,i3)
     & *wrbs2(i1,i2,i3,kd)
      wrbz22(i1,i2,i3,kd)=0
      wrbx23(i1,i2,i3,kd)=rx(i1,i2,i3)*wrbr2(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & wrbs2(i1,i2,i3,kd)+tx(i1,i2,i3)*wrbt2(i1,i2,i3,kd)
      wrby23(i1,i2,i3,kd)=ry(i1,i2,i3)*wrbr2(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & wrbs2(i1,i2,i3,kd)+ty(i1,i2,i3)*wrbt2(i1,i2,i3,kd)
      wrbz23(i1,i2,i3,kd)=rz(i1,i2,i3)*wrbr2(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & wrbs2(i1,i2,i3,kd)+tz(i1,i2,i3)*wrbt2(i1,i2,i3,kd)
      wrbxx21(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wrbrr2(i1,i2,i3,kd)+(
     & rxx22(i1,i2,i3))*wrbr2(i1,i2,i3,kd)
      wrbyy21(i1,i2,i3,kd)=0
      wrbxy21(i1,i2,i3,kd)=0
      wrbxz21(i1,i2,i3,kd)=0
      wrbyz21(i1,i2,i3,kd)=0
      wrbzz21(i1,i2,i3,kd)=0
      wrblaplacian21(i1,i2,i3,kd)=wrbxx21(i1,i2,i3,kd)
      wrbxx22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wrbrr2(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*wrbrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*wrbss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3))*wrbr2(i1,i2,i3,kd)+(
     & sxx22(i1,i2,i3))*wrbs2(i1,i2,i3,kd)
      wrbyy22(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*wrbrr2(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*wrbrs2(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*wrbss2(i1,i2,i3,kd)+(ryy22(i1,i2,i3))*wrbr2(i1,i2,i3,kd)+(
     & syy22(i1,i2,i3))*wrbs2(i1,i2,i3,kd)
      wrbxy22(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wrbrr2(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & wrbrs2(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wrbss2(i1,i2,i3,
     & kd)+rxy22(i1,i2,i3)*wrbr2(i1,i2,i3,kd)+sxy22(i1,i2,i3)*wrbs2(
     & i1,i2,i3,kd)
      wrbxz22(i1,i2,i3,kd)=0
      wrbyz22(i1,i2,i3,kd)=0
      wrbzz22(i1,i2,i3,kd)=0
      wrblaplacian22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & wrbrr2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)
     & *sy(i1,i2,i3))*wrbrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2)*wrbss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3)+ryy22(i1,i2,i3))*
     & wrbr2(i1,i2,i3,kd)+(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*wrbs2(i1,
     & i2,i3,kd)
      wrbxx23(i1,i2,i3,kd)=rx(i1,i2,i3)**2*wrbrr2(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*wrbss2(i1,i2,i3,kd)+tx(i1,i2,i3)**2*wrbtt2(i1,i2,i3,
     & kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*wrbrs2(i1,i2,i3,kd)+2.*rx(i1,
     & i2,i3)*tx(i1,i2,i3)*wrbrt2(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,
     & i2,i3)*wrbst2(i1,i2,i3,kd)+rxx23(i1,i2,i3)*wrbr2(i1,i2,i3,kd)+
     & sxx23(i1,i2,i3)*wrbs2(i1,i2,i3,kd)+txx23(i1,i2,i3)*wrbt2(i1,i2,
     & i3,kd)
      wrbyy23(i1,i2,i3,kd)=ry(i1,i2,i3)**2*wrbrr2(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*wrbss2(i1,i2,i3,kd)+ty(i1,i2,i3)**2*wrbtt2(i1,i2,i3,
     & kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*wrbrs2(i1,i2,i3,kd)+2.*ry(i1,
     & i2,i3)*ty(i1,i2,i3)*wrbrt2(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,
     & i2,i3)*wrbst2(i1,i2,i3,kd)+ryy23(i1,i2,i3)*wrbr2(i1,i2,i3,kd)+
     & syy23(i1,i2,i3)*wrbs2(i1,i2,i3,kd)+tyy23(i1,i2,i3)*wrbt2(i1,i2,
     & i3,kd)
      wrbzz23(i1,i2,i3,kd)=rz(i1,i2,i3)**2*wrbrr2(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*wrbss2(i1,i2,i3,kd)+tz(i1,i2,i3)**2*wrbtt2(i1,i2,i3,
     & kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*wrbrs2(i1,i2,i3,kd)+2.*rz(i1,
     & i2,i3)*tz(i1,i2,i3)*wrbrt2(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,
     & i2,i3)*wrbst2(i1,i2,i3,kd)+rzz23(i1,i2,i3)*wrbr2(i1,i2,i3,kd)+
     & szz23(i1,i2,i3)*wrbs2(i1,i2,i3,kd)+tzz23(i1,i2,i3)*wrbt2(i1,i2,
     & i3,kd)
      wrbxy23(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wrbrr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wrbss2(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & ty(i1,i2,i3)*wrbtt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(
     & i1,i2,i3)*sx(i1,i2,i3))*wrbrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(
     & i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*wrbrt2(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*wrbst2(i1,i2,
     & i3,kd)+rxy23(i1,i2,i3)*wrbr2(i1,i2,i3,kd)+sxy23(i1,i2,i3)*
     & wrbs2(i1,i2,i3,kd)+txy23(i1,i2,i3)*wrbt2(i1,i2,i3,kd)
      wrbxz23(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*wrbrr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*wrbss2(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & tz(i1,i2,i3)*wrbtt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sx(i1,i2,i3))*wrbrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*wrbrt2(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*wrbst2(i1,i2,
     & i3,kd)+rxz23(i1,i2,i3)*wrbr2(i1,i2,i3,kd)+sxz23(i1,i2,i3)*
     & wrbs2(i1,i2,i3,kd)+txz23(i1,i2,i3)*wrbt2(i1,i2,i3,kd)
      wrbyz23(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*wrbrr2(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*wrbss2(i1,i2,i3,kd)+ty(i1,i2,i3)*
     & tz(i1,i2,i3)*wrbtt2(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sy(i1,i2,i3))*wrbrs2(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*wrbrt2(i1,i2,i3,kd)+(sy(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*wrbst2(i1,i2,
     & i3,kd)+ryz23(i1,i2,i3)*wrbr2(i1,i2,i3,kd)+syz23(i1,i2,i3)*
     & wrbs2(i1,i2,i3,kd)+tyz23(i1,i2,i3)*wrbt2(i1,i2,i3,kd)
      wrblaplacian23(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*wrbrr2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)
     & **2+sz(i1,i2,i3)**2)*wrbss2(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(
     & i1,i2,i3)**2+tz(i1,i2,i3)**2)*wrbtt2(i1,i2,i3,kd)+2.*(rx(i1,i2,
     & i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,
     & i2,i3))*wrbrs2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(
     & i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*wrbrt2(i1,i2,
     & i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)
     & +sz(i1,i2,i3)*tz(i1,i2,i3))*wrbst2(i1,i2,i3,kd)+(rxx23(i1,i2,
     & i3)+ryy23(i1,i2,i3)+rzz23(i1,i2,i3))*wrbr2(i1,i2,i3,kd)+(sxx23(
     & i1,i2,i3)+syy23(i1,i2,i3)+szz23(i1,i2,i3))*wrbs2(i1,i2,i3,kd)+(
     & txx23(i1,i2,i3)+tyy23(i1,i2,i3)+tzz23(i1,i2,i3))*wrbt2(i1,i2,
     & i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      wrbx23r(i1,i2,i3,kd)=(wrb(i1+1,i2,i3,kd)-wrb(i1-1,i2,i3,kd))*h12(
     & 0)
      wrby23r(i1,i2,i3,kd)=(wrb(i1,i2+1,i3,kd)-wrb(i1,i2-1,i3,kd))*h12(
     & 1)
      wrbz23r(i1,i2,i3,kd)=(wrb(i1,i2,i3+1,kd)-wrb(i1,i2,i3-1,kd))*h12(
     & 2)
      wrbxx23r(i1,i2,i3,kd)=(-2.*wrb(i1,i2,i3,kd)+(wrb(i1+1,i2,i3,kd)+
     & wrb(i1-1,i2,i3,kd)) )*h22(0)
      wrbyy23r(i1,i2,i3,kd)=(-2.*wrb(i1,i2,i3,kd)+(wrb(i1,i2+1,i3,kd)+
     & wrb(i1,i2-1,i3,kd)) )*h22(1)
      wrbxy23r(i1,i2,i3,kd)=(wrbx23r(i1,i2+1,i3,kd)-wrbx23r(i1,i2-1,i3,
     & kd))*h12(1)
      wrbzz23r(i1,i2,i3,kd)=(-2.*wrb(i1,i2,i3,kd)+(wrb(i1,i2,i3+1,kd)+
     & wrb(i1,i2,i3-1,kd)) )*h22(2)
      wrbxz23r(i1,i2,i3,kd)=(wrbx23r(i1,i2,i3+1,kd)-wrbx23r(i1,i2,i3-1,
     & kd))*h12(2)
      wrbyz23r(i1,i2,i3,kd)=(wrby23r(i1,i2,i3+1,kd)-wrby23r(i1,i2,i3-1,
     & kd))*h12(2)
      wrbx21r(i1,i2,i3,kd)= wrbx23r(i1,i2,i3,kd)
      wrby21r(i1,i2,i3,kd)= wrby23r(i1,i2,i3,kd)
      wrbz21r(i1,i2,i3,kd)= wrbz23r(i1,i2,i3,kd)
      wrbxx21r(i1,i2,i3,kd)= wrbxx23r(i1,i2,i3,kd)
      wrbyy21r(i1,i2,i3,kd)= wrbyy23r(i1,i2,i3,kd)
      wrbzz21r(i1,i2,i3,kd)= wrbzz23r(i1,i2,i3,kd)
      wrbxy21r(i1,i2,i3,kd)= wrbxy23r(i1,i2,i3,kd)
      wrbxz21r(i1,i2,i3,kd)= wrbxz23r(i1,i2,i3,kd)
      wrbyz21r(i1,i2,i3,kd)= wrbyz23r(i1,i2,i3,kd)
      wrblaplacian21r(i1,i2,i3,kd)=wrbxx23r(i1,i2,i3,kd)
      wrbx22r(i1,i2,i3,kd)= wrbx23r(i1,i2,i3,kd)
      wrby22r(i1,i2,i3,kd)= wrby23r(i1,i2,i3,kd)
      wrbz22r(i1,i2,i3,kd)= wrbz23r(i1,i2,i3,kd)
      wrbxx22r(i1,i2,i3,kd)= wrbxx23r(i1,i2,i3,kd)
      wrbyy22r(i1,i2,i3,kd)= wrbyy23r(i1,i2,i3,kd)
      wrbzz22r(i1,i2,i3,kd)= wrbzz23r(i1,i2,i3,kd)
      wrbxy22r(i1,i2,i3,kd)= wrbxy23r(i1,i2,i3,kd)
      wrbxz22r(i1,i2,i3,kd)= wrbxz23r(i1,i2,i3,kd)
      wrbyz22r(i1,i2,i3,kd)= wrbyz23r(i1,i2,i3,kd)
      wrblaplacian22r(i1,i2,i3,kd)=wrbxx23r(i1,i2,i3,kd)+wrbyy23r(i1,
     & i2,i3,kd)
      wrblaplacian23r(i1,i2,i3,kd)=wrbxx23r(i1,i2,i3,kd)+wrbyy23r(i1,
     & i2,i3,kd)+wrbzz23r(i1,i2,i3,kd)
      wrbxxx22r(i1,i2,i3,kd)=(-2.*(wrb(i1+1,i2,i3,kd)-wrb(i1-1,i2,i3,
     & kd))+(wrb(i1+2,i2,i3,kd)-wrb(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
      wrbyyy22r(i1,i2,i3,kd)=(-2.*(wrb(i1,i2+1,i3,kd)-wrb(i1,i2-1,i3,
     & kd))+(wrb(i1,i2+2,i3,kd)-wrb(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
      wrbxxy22r(i1,i2,i3,kd)=( wrbxx22r(i1,i2+1,i3,kd)-wrbxx22r(i1,i2-
     & 1,i3,kd))/(2.*dx(1))
      wrbxyy22r(i1,i2,i3,kd)=( wrbyy22r(i1+1,i2,i3,kd)-wrbyy22r(i1-1,
     & i2,i3,kd))/(2.*dx(0))
      wrbxxxx22r(i1,i2,i3,kd)=(6.*wrb(i1,i2,i3,kd)-4.*(wrb(i1+1,i2,i3,
     & kd)+wrb(i1-1,i2,i3,kd))+(wrb(i1+2,i2,i3,kd)+wrb(i1-2,i2,i3,kd))
     &  )/(dx(0)**4)
      wrbyyyy22r(i1,i2,i3,kd)=(6.*wrb(i1,i2,i3,kd)-4.*(wrb(i1,i2+1,i3,
     & kd)+wrb(i1,i2-1,i3,kd))+(wrb(i1,i2+2,i3,kd)+wrb(i1,i2-2,i3,kd))
     &  )/(dx(1)**4)
      wrbxxyy22r(i1,i2,i3,kd)=( 4.*wrb(i1,i2,i3,kd)     -2.*(wrb(i1+1,
     & i2,i3,kd)+wrb(i1-1,i2,i3,kd)+wrb(i1,i2+1,i3,kd)+wrb(i1,i2-1,i3,
     & kd))   +   (wrb(i1+1,i2+1,i3,kd)+wrb(i1-1,i2+1,i3,kd)+wrb(i1+1,
     & i2-1,i3,kd)+wrb(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      ! 2D laplacian squared = wrb.xxxx + 2 wrb.xxyy + wrb.yyyy
      wrbLapSq22r(i1,i2,i3,kd)= ( 6.*wrb(i1,i2,i3,kd)   - 4.*(wrb(i1+1,
     & i2,i3,kd)+wrb(i1-1,i2,i3,kd))    +(wrb(i1+2,i2,i3,kd)+wrb(i1-2,
     & i2,i3,kd)) )/(dx(0)**4) +( 6.*wrb(i1,i2,i3,kd)    -4.*(wrb(i1,
     & i2+1,i3,kd)+wrb(i1,i2-1,i3,kd))    +(wrb(i1,i2+2,i3,kd)+wrb(i1,
     & i2-2,i3,kd)) )/(dx(1)**4)  +( 8.*wrb(i1,i2,i3,kd)     -4.*(wrb(
     & i1+1,i2,i3,kd)+wrb(i1-1,i2,i3,kd)+wrb(i1,i2+1,i3,kd)+wrb(i1,i2-
     & 1,i3,kd))   +2.*(wrb(i1+1,i2+1,i3,kd)+wrb(i1-1,i2+1,i3,kd)+wrb(
     & i1+1,i2-1,i3,kd)+wrb(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      wrbxxx23r(i1,i2,i3,kd)=(-2.*(wrb(i1+1,i2,i3,kd)-wrb(i1-1,i2,i3,
     & kd))+(wrb(i1+2,i2,i3,kd)-wrb(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
      wrbyyy23r(i1,i2,i3,kd)=(-2.*(wrb(i1,i2+1,i3,kd)-wrb(i1,i2-1,i3,
     & kd))+(wrb(i1,i2+2,i3,kd)-wrb(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
      wrbzzz23r(i1,i2,i3,kd)=(-2.*(wrb(i1,i2,i3+1,kd)-wrb(i1,i2,i3-1,
     & kd))+(wrb(i1,i2,i3+2,kd)-wrb(i1,i2,i3-2,kd)) )*h22(1)*h12(2)
      wrbxxy23r(i1,i2,i3,kd)=( wrbxx22r(i1,i2+1,i3,kd)-wrbxx22r(i1,i2-
     & 1,i3,kd))/(2.*dx(1))
      wrbxyy23r(i1,i2,i3,kd)=( wrbyy22r(i1+1,i2,i3,kd)-wrbyy22r(i1-1,
     & i2,i3,kd))/(2.*dx(0))
      wrbxxz23r(i1,i2,i3,kd)=( wrbxx22r(i1,i2,i3+1,kd)-wrbxx22r(i1,i2,
     & i3-1,kd))/(2.*dx(2))
      wrbyyz23r(i1,i2,i3,kd)=( wrbyy22r(i1,i2,i3+1,kd)-wrbyy22r(i1,i2,
     & i3-1,kd))/(2.*dx(2))
      wrbxzz23r(i1,i2,i3,kd)=( wrbzz22r(i1+1,i2,i3,kd)-wrbzz22r(i1-1,
     & i2,i3,kd))/(2.*dx(0))
      wrbyzz23r(i1,i2,i3,kd)=( wrbzz22r(i1,i2+1,i3,kd)-wrbzz22r(i1,i2-
     & 1,i3,kd))/(2.*dx(1))
      wrbxxxx23r(i1,i2,i3,kd)=(6.*wrb(i1,i2,i3,kd)-4.*(wrb(i1+1,i2,i3,
     & kd)+wrb(i1-1,i2,i3,kd))+(wrb(i1+2,i2,i3,kd)+wrb(i1-2,i2,i3,kd))
     &  )/(dx(0)**4)
      wrbyyyy23r(i1,i2,i3,kd)=(6.*wrb(i1,i2,i3,kd)-4.*(wrb(i1,i2+1,i3,
     & kd)+wrb(i1,i2-1,i3,kd))+(wrb(i1,i2+2,i3,kd)+wrb(i1,i2-2,i3,kd))
     &  )/(dx(1)**4)
      wrbzzzz23r(i1,i2,i3,kd)=(6.*wrb(i1,i2,i3,kd)-4.*(wrb(i1,i2,i3+1,
     & kd)+wrb(i1,i2,i3-1,kd))+(wrb(i1,i2,i3+2,kd)+wrb(i1,i2,i3-2,kd))
     &  )/(dx(2)**4)
      wrbxxyy23r(i1,i2,i3,kd)=( 4.*wrb(i1,i2,i3,kd)     -2.*(wrb(i1+1,
     & i2,i3,kd)+wrb(i1-1,i2,i3,kd)+wrb(i1,i2+1,i3,kd)+wrb(i1,i2-1,i3,
     & kd))   +   (wrb(i1+1,i2+1,i3,kd)+wrb(i1-1,i2+1,i3,kd)+wrb(i1+1,
     & i2-1,i3,kd)+wrb(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      wrbxxzz23r(i1,i2,i3,kd)=( 4.*wrb(i1,i2,i3,kd)     -2.*(wrb(i1+1,
     & i2,i3,kd)+wrb(i1-1,i2,i3,kd)+wrb(i1,i2,i3+1,kd)+wrb(i1,i2,i3-1,
     & kd))   +   (wrb(i1+1,i2,i3+1,kd)+wrb(i1-1,i2,i3+1,kd)+wrb(i1+1,
     & i2,i3-1,kd)+wrb(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)
      wrbyyzz23r(i1,i2,i3,kd)=( 4.*wrb(i1,i2,i3,kd)     -2.*(wrb(i1,i2+
     & 1,i3,kd)  +wrb(i1,i2-1,i3,kd)+  wrb(i1,i2  ,i3+1,kd)+wrb(i1,i2 
     &  ,i3-1,kd))   +   (wrb(i1,i2+1,i3+1,kd)+wrb(i1,i2-1,i3+1,kd)+
     & wrb(i1,i2+1,i3-1,kd)+wrb(i1,i2-1,i3-1,kd)) )/(dx(1)**2*dx(2)**
     & 2)
      ! 3D laplacian squared = wrb.xxxx + wrb.yyyy + wrb.zzzz + 2 (wrb.xxyy + wrb.xxzz + wrb.yyzz )
      wrbLapSq23r(i1,i2,i3,kd)= ( 6.*wrb(i1,i2,i3,kd)   - 4.*(wrb(i1+1,
     & i2,i3,kd)+wrb(i1-1,i2,i3,kd))    +(wrb(i1+2,i2,i3,kd)+wrb(i1-2,
     & i2,i3,kd)) )/(dx(0)**4) +( 6.*wrb(i1,i2,i3,kd)    -4.*(wrb(i1,
     & i2+1,i3,kd)+wrb(i1,i2-1,i3,kd))    +(wrb(i1,i2+2,i3,kd)+wrb(i1,
     & i2-2,i3,kd)) )/(dx(1)**4)  +( 6.*wrb(i1,i2,i3,kd)    -4.*(wrb(
     & i1,i2,i3+1,kd)+wrb(i1,i2,i3-1,kd))    +(wrb(i1,i2,i3+2,kd)+wrb(
     & i1,i2,i3-2,kd)) )/(dx(2)**4)  +( 8.*wrb(i1,i2,i3,kd)     -4.*(
     & wrb(i1+1,i2,i3,kd)  +wrb(i1-1,i2,i3,kd)  +wrb(i1  ,i2+1,i3,kd)+
     & wrb(i1  ,i2-1,i3,kd))   +2.*(wrb(i1+1,i2+1,i3,kd)+wrb(i1-1,i2+
     & 1,i3,kd)+wrb(i1+1,i2-1,i3,kd)+wrb(i1-1,i2-1,i3,kd)) )/(dx(0)**
     & 2*dx(1)**2)+( 8.*wrb(i1,i2,i3,kd)     -4.*(wrb(i1+1,i2,i3,kd)  
     & +wrb(i1-1,i2,i3,kd)  +wrb(i1  ,i2,i3+1,kd)+wrb(i1  ,i2,i3-1,kd)
     & )   +2.*(wrb(i1+1,i2,i3+1,kd)+wrb(i1-1,i2,i3+1,kd)+wrb(i1+1,i2,
     & i3-1,kd)+wrb(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)+( 8.*wrb(
     & i1,i2,i3,kd)     -4.*(wrb(i1,i2+1,i3,kd)  +wrb(i1,i2-1,i3,kd)  
     & +wrb(i1,i2  ,i3+1,kd)+wrb(i1,i2  ,i3-1,kd))   +2.*(wrb(i1,i2+1,
     & i3+1,kd)+wrb(i1,i2-1,i3+1,kd)+wrb(i1,i2+1,i3-1,kd)+wrb(i1,i2-1,
     & i3-1,kd)) )/(dx(1)**2*dx(2)**2)
      wrbmr2(i1,i2,i3,kd)=(wrbm(i1+1,i2,i3,kd)-wrbm(i1-1,i2,i3,kd))*
     & d12(0)
      wrbms2(i1,i2,i3,kd)=(wrbm(i1,i2+1,i3,kd)-wrbm(i1,i2-1,i3,kd))*
     & d12(1)
      wrbmt2(i1,i2,i3,kd)=(wrbm(i1,i2,i3+1,kd)-wrbm(i1,i2,i3-1,kd))*
     & d12(2)
      wrbmrr2(i1,i2,i3,kd)=(-2.*wrbm(i1,i2,i3,kd)+(wrbm(i1+1,i2,i3,kd)+
     & wrbm(i1-1,i2,i3,kd)) )*d22(0)
      wrbmss2(i1,i2,i3,kd)=(-2.*wrbm(i1,i2,i3,kd)+(wrbm(i1,i2+1,i3,kd)+
     & wrbm(i1,i2-1,i3,kd)) )*d22(1)
      wrbmrs2(i1,i2,i3,kd)=(wrbmr2(i1,i2+1,i3,kd)-wrbmr2(i1,i2-1,i3,kd)
     & )*d12(1)
      wrbmtt2(i1,i2,i3,kd)=(-2.*wrbm(i1,i2,i3,kd)+(wrbm(i1,i2,i3+1,kd)+
     & wrbm(i1,i2,i3-1,kd)) )*d22(2)
      wrbmrt2(i1,i2,i3,kd)=(wrbmr2(i1,i2,i3+1,kd)-wrbmr2(i1,i2,i3-1,kd)
     & )*d12(2)
      wrbmst2(i1,i2,i3,kd)=(wrbms2(i1,i2,i3+1,kd)-wrbms2(i1,i2,i3-1,kd)
     & )*d12(2)
      wrbmrrr2(i1,i2,i3,kd)=(-2.*(wrbm(i1+1,i2,i3,kd)-wrbm(i1-1,i2,i3,
     & kd))+(wrbm(i1+2,i2,i3,kd)-wrbm(i1-2,i2,i3,kd)) )*d22(0)*d12(0)
      wrbmsss2(i1,i2,i3,kd)=(-2.*(wrbm(i1,i2+1,i3,kd)-wrbm(i1,i2-1,i3,
     & kd))+(wrbm(i1,i2+2,i3,kd)-wrbm(i1,i2-2,i3,kd)) )*d22(1)*d12(1)
      wrbmttt2(i1,i2,i3,kd)=(-2.*(wrbm(i1,i2,i3+1,kd)-wrbm(i1,i2,i3-1,
     & kd))+(wrbm(i1,i2,i3+2,kd)-wrbm(i1,i2,i3-2,kd)) )*d22(2)*d12(2)
      wrbmx21(i1,i2,i3,kd)= rx(i1,i2,i3)*wrbmr2(i1,i2,i3,kd)
      wrbmy21(i1,i2,i3,kd)=0
      wrbmz21(i1,i2,i3,kd)=0
      wrbmx22(i1,i2,i3,kd)= rx(i1,i2,i3)*wrbmr2(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*wrbms2(i1,i2,i3,kd)
      wrbmy22(i1,i2,i3,kd)= ry(i1,i2,i3)*wrbmr2(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*wrbms2(i1,i2,i3,kd)
      wrbmz22(i1,i2,i3,kd)=0
      wrbmx23(i1,i2,i3,kd)=rx(i1,i2,i3)*wrbmr2(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*wrbms2(i1,i2,i3,kd)+tx(i1,i2,i3)*wrbmt2(i1,i2,i3,kd)
      wrbmy23(i1,i2,i3,kd)=ry(i1,i2,i3)*wrbmr2(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*wrbms2(i1,i2,i3,kd)+ty(i1,i2,i3)*wrbmt2(i1,i2,i3,kd)
      wrbmz23(i1,i2,i3,kd)=rz(i1,i2,i3)*wrbmr2(i1,i2,i3,kd)+sz(i1,i2,
     & i3)*wrbms2(i1,i2,i3,kd)+tz(i1,i2,i3)*wrbmt2(i1,i2,i3,kd)
      wrbmxx21(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wrbmrr2(i1,i2,i3,kd)+(
     & rxx22(i1,i2,i3))*wrbmr2(i1,i2,i3,kd)
      wrbmyy21(i1,i2,i3,kd)=0
      wrbmxy21(i1,i2,i3,kd)=0
      wrbmxz21(i1,i2,i3,kd)=0
      wrbmyz21(i1,i2,i3,kd)=0
      wrbmzz21(i1,i2,i3,kd)=0
      wrbmlaplacian21(i1,i2,i3,kd)=wrbmxx21(i1,i2,i3,kd)
      wrbmxx22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wrbmrr2(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*wrbmrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*wrbmss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3))*wrbmr2(i1,i2,i3,kd)+(
     & sxx22(i1,i2,i3))*wrbms2(i1,i2,i3,kd)
      wrbmyy22(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*wrbmrr2(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*wrbmrs2(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*wrbmss2(i1,i2,i3,kd)+(ryy22(i1,i2,i3))*wrbmr2(i1,i2,i3,kd)+(
     & syy22(i1,i2,i3))*wrbms2(i1,i2,i3,kd)
      wrbmxy22(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wrbmrr2(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & wrbmrs2(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wrbmss2(i1,i2,
     & i3,kd)+rxy22(i1,i2,i3)*wrbmr2(i1,i2,i3,kd)+sxy22(i1,i2,i3)*
     & wrbms2(i1,i2,i3,kd)
      wrbmxz22(i1,i2,i3,kd)=0
      wrbmyz22(i1,i2,i3,kd)=0
      wrbmzz22(i1,i2,i3,kd)=0
      wrbmlaplacian22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & wrbmrr2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,
     & i3)*sy(i1,i2,i3))*wrbmrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,
     & i2,i3)**2)*wrbmss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3)+ryy22(i1,i2,
     & i3))*wrbmr2(i1,i2,i3,kd)+(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*
     & wrbms2(i1,i2,i3,kd)
      wrbmxx23(i1,i2,i3,kd)=rx(i1,i2,i3)**2*wrbmrr2(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*wrbmss2(i1,i2,i3,kd)+tx(i1,i2,i3)**2*wrbmtt2(i1,i2,
     & i3,kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*wrbmrs2(i1,i2,i3,kd)+2.*rx(
     & i1,i2,i3)*tx(i1,i2,i3)*wrbmrt2(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(
     & i1,i2,i3)*wrbmst2(i1,i2,i3,kd)+rxx23(i1,i2,i3)*wrbmr2(i1,i2,i3,
     & kd)+sxx23(i1,i2,i3)*wrbms2(i1,i2,i3,kd)+txx23(i1,i2,i3)*wrbmt2(
     & i1,i2,i3,kd)
      wrbmyy23(i1,i2,i3,kd)=ry(i1,i2,i3)**2*wrbmrr2(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*wrbmss2(i1,i2,i3,kd)+ty(i1,i2,i3)**2*wrbmtt2(i1,i2,
     & i3,kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*wrbmrs2(i1,i2,i3,kd)+2.*ry(
     & i1,i2,i3)*ty(i1,i2,i3)*wrbmrt2(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(
     & i1,i2,i3)*wrbmst2(i1,i2,i3,kd)+ryy23(i1,i2,i3)*wrbmr2(i1,i2,i3,
     & kd)+syy23(i1,i2,i3)*wrbms2(i1,i2,i3,kd)+tyy23(i1,i2,i3)*wrbmt2(
     & i1,i2,i3,kd)
      wrbmzz23(i1,i2,i3,kd)=rz(i1,i2,i3)**2*wrbmrr2(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*wrbmss2(i1,i2,i3,kd)+tz(i1,i2,i3)**2*wrbmtt2(i1,i2,
     & i3,kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*wrbmrs2(i1,i2,i3,kd)+2.*rz(
     & i1,i2,i3)*tz(i1,i2,i3)*wrbmrt2(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(
     & i1,i2,i3)*wrbmst2(i1,i2,i3,kd)+rzz23(i1,i2,i3)*wrbmr2(i1,i2,i3,
     & kd)+szz23(i1,i2,i3)*wrbms2(i1,i2,i3,kd)+tzz23(i1,i2,i3)*wrbmt2(
     & i1,i2,i3,kd)
      wrbmxy23(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wrbmrr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wrbmss2(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *ty(i1,i2,i3)*wrbmtt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+
     & ry(i1,i2,i3)*sx(i1,i2,i3))*wrbmrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & ty(i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*wrbmrt2(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*wrbmst2(
     & i1,i2,i3,kd)+rxy23(i1,i2,i3)*wrbmr2(i1,i2,i3,kd)+sxy23(i1,i2,
     & i3)*wrbms2(i1,i2,i3,kd)+txy23(i1,i2,i3)*wrbmt2(i1,i2,i3,kd)
      wrbmxz23(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*wrbmrr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*wrbmss2(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *tz(i1,i2,i3)*wrbmtt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sx(i1,i2,i3))*wrbmrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*wrbmrt2(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*wrbmst2(
     & i1,i2,i3,kd)+rxz23(i1,i2,i3)*wrbmr2(i1,i2,i3,kd)+sxz23(i1,i2,
     & i3)*wrbms2(i1,i2,i3,kd)+txz23(i1,i2,i3)*wrbmt2(i1,i2,i3,kd)
      wrbmyz23(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*wrbmrr2(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*wrbmss2(i1,i2,i3,kd)+ty(i1,i2,i3)
     & *tz(i1,i2,i3)*wrbmtt2(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sy(i1,i2,i3))*wrbmrs2(i1,i2,i3,kd)+(ry(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*wrbmrt2(i1,i2,i3,kd)+(
     & sy(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*wrbmst2(
     & i1,i2,i3,kd)+ryz23(i1,i2,i3)*wrbmr2(i1,i2,i3,kd)+syz23(i1,i2,
     & i3)*wrbms2(i1,i2,i3,kd)+tyz23(i1,i2,i3)*wrbmt2(i1,i2,i3,kd)
      wrbmlaplacian23(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*wrbmrr2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2+sz(i1,i2,i3)**2)*wrbmss2(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+
     & ty(i1,i2,i3)**2+tz(i1,i2,i3)**2)*wrbmtt2(i1,i2,i3,kd)+2.*(rx(
     & i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*
     & sz(i1,i2,i3))*wrbmrs2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,
     & i3)+ ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*
     & wrbmrt2(i1,i2,i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,
     & i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3))*wrbmst2(i1,i2,i3,
     & kd)+(rxx23(i1,i2,i3)+ryy23(i1,i2,i3)+rzz23(i1,i2,i3))*wrbmr2(
     & i1,i2,i3,kd)+(sxx23(i1,i2,i3)+syy23(i1,i2,i3)+szz23(i1,i2,i3))*
     & wrbms2(i1,i2,i3,kd)+(txx23(i1,i2,i3)+tyy23(i1,i2,i3)+tzz23(i1,
     & i2,i3))*wrbmt2(i1,i2,i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      wrbmx23r(i1,i2,i3,kd)=(wrbm(i1+1,i2,i3,kd)-wrbm(i1-1,i2,i3,kd))*
     & h12(0)
      wrbmy23r(i1,i2,i3,kd)=(wrbm(i1,i2+1,i3,kd)-wrbm(i1,i2-1,i3,kd))*
     & h12(1)
      wrbmz23r(i1,i2,i3,kd)=(wrbm(i1,i2,i3+1,kd)-wrbm(i1,i2,i3-1,kd))*
     & h12(2)
      wrbmxx23r(i1,i2,i3,kd)=(-2.*wrbm(i1,i2,i3,kd)+(wrbm(i1+1,i2,i3,
     & kd)+wrbm(i1-1,i2,i3,kd)) )*h22(0)
      wrbmyy23r(i1,i2,i3,kd)=(-2.*wrbm(i1,i2,i3,kd)+(wrbm(i1,i2+1,i3,
     & kd)+wrbm(i1,i2-1,i3,kd)) )*h22(1)
      wrbmxy23r(i1,i2,i3,kd)=(wrbmx23r(i1,i2+1,i3,kd)-wrbmx23r(i1,i2-1,
     & i3,kd))*h12(1)
      wrbmzz23r(i1,i2,i3,kd)=(-2.*wrbm(i1,i2,i3,kd)+(wrbm(i1,i2,i3+1,
     & kd)+wrbm(i1,i2,i3-1,kd)) )*h22(2)
      wrbmxz23r(i1,i2,i3,kd)=(wrbmx23r(i1,i2,i3+1,kd)-wrbmx23r(i1,i2,
     & i3-1,kd))*h12(2)
      wrbmyz23r(i1,i2,i3,kd)=(wrbmy23r(i1,i2,i3+1,kd)-wrbmy23r(i1,i2,
     & i3-1,kd))*h12(2)
      wrbmx21r(i1,i2,i3,kd)= wrbmx23r(i1,i2,i3,kd)
      wrbmy21r(i1,i2,i3,kd)= wrbmy23r(i1,i2,i3,kd)
      wrbmz21r(i1,i2,i3,kd)= wrbmz23r(i1,i2,i3,kd)
      wrbmxx21r(i1,i2,i3,kd)= wrbmxx23r(i1,i2,i3,kd)
      wrbmyy21r(i1,i2,i3,kd)= wrbmyy23r(i1,i2,i3,kd)
      wrbmzz21r(i1,i2,i3,kd)= wrbmzz23r(i1,i2,i3,kd)
      wrbmxy21r(i1,i2,i3,kd)= wrbmxy23r(i1,i2,i3,kd)
      wrbmxz21r(i1,i2,i3,kd)= wrbmxz23r(i1,i2,i3,kd)
      wrbmyz21r(i1,i2,i3,kd)= wrbmyz23r(i1,i2,i3,kd)
      wrbmlaplacian21r(i1,i2,i3,kd)=wrbmxx23r(i1,i2,i3,kd)
      wrbmx22r(i1,i2,i3,kd)= wrbmx23r(i1,i2,i3,kd)
      wrbmy22r(i1,i2,i3,kd)= wrbmy23r(i1,i2,i3,kd)
      wrbmz22r(i1,i2,i3,kd)= wrbmz23r(i1,i2,i3,kd)
      wrbmxx22r(i1,i2,i3,kd)= wrbmxx23r(i1,i2,i3,kd)
      wrbmyy22r(i1,i2,i3,kd)= wrbmyy23r(i1,i2,i3,kd)
      wrbmzz22r(i1,i2,i3,kd)= wrbmzz23r(i1,i2,i3,kd)
      wrbmxy22r(i1,i2,i3,kd)= wrbmxy23r(i1,i2,i3,kd)
      wrbmxz22r(i1,i2,i3,kd)= wrbmxz23r(i1,i2,i3,kd)
      wrbmyz22r(i1,i2,i3,kd)= wrbmyz23r(i1,i2,i3,kd)
      wrbmlaplacian22r(i1,i2,i3,kd)=wrbmxx23r(i1,i2,i3,kd)+wrbmyy23r(
     & i1,i2,i3,kd)
      wrbmlaplacian23r(i1,i2,i3,kd)=wrbmxx23r(i1,i2,i3,kd)+wrbmyy23r(
     & i1,i2,i3,kd)+wrbmzz23r(i1,i2,i3,kd)
      wrbmxxx22r(i1,i2,i3,kd)=(-2.*(wrbm(i1+1,i2,i3,kd)-wrbm(i1-1,i2,
     & i3,kd))+(wrbm(i1+2,i2,i3,kd)-wrbm(i1-2,i2,i3,kd)) )*h22(0)*h12(
     & 0)
      wrbmyyy22r(i1,i2,i3,kd)=(-2.*(wrbm(i1,i2+1,i3,kd)-wrbm(i1,i2-1,
     & i3,kd))+(wrbm(i1,i2+2,i3,kd)-wrbm(i1,i2-2,i3,kd)) )*h22(1)*h12(
     & 1)
      wrbmxxy22r(i1,i2,i3,kd)=( wrbmxx22r(i1,i2+1,i3,kd)-wrbmxx22r(i1,
     & i2-1,i3,kd))/(2.*dx(1))
      wrbmxyy22r(i1,i2,i3,kd)=( wrbmyy22r(i1+1,i2,i3,kd)-wrbmyy22r(i1-
     & 1,i2,i3,kd))/(2.*dx(0))
      wrbmxxxx22r(i1,i2,i3,kd)=(6.*wrbm(i1,i2,i3,kd)-4.*(wrbm(i1+1,i2,
     & i3,kd)+wrbm(i1-1,i2,i3,kd))+(wrbm(i1+2,i2,i3,kd)+wrbm(i1-2,i2,
     & i3,kd)) )/(dx(0)**4)
      wrbmyyyy22r(i1,i2,i3,kd)=(6.*wrbm(i1,i2,i3,kd)-4.*(wrbm(i1,i2+1,
     & i3,kd)+wrbm(i1,i2-1,i3,kd))+(wrbm(i1,i2+2,i3,kd)+wrbm(i1,i2-2,
     & i3,kd)) )/(dx(1)**4)
      wrbmxxyy22r(i1,i2,i3,kd)=( 4.*wrbm(i1,i2,i3,kd)     -2.*(wrbm(i1+
     & 1,i2,i3,kd)+wrbm(i1-1,i2,i3,kd)+wrbm(i1,i2+1,i3,kd)+wrbm(i1,i2-
     & 1,i3,kd))   +   (wrbm(i1+1,i2+1,i3,kd)+wrbm(i1-1,i2+1,i3,kd)+
     & wrbm(i1+1,i2-1,i3,kd)+wrbm(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)*
     & *2)
      ! 2D laplacian squared = wrbm.xxxx + 2 wrbm.xxyy + wrbm.yyyy
      wrbmLapSq22r(i1,i2,i3,kd)= ( 6.*wrbm(i1,i2,i3,kd)   - 4.*(wrbm(
     & i1+1,i2,i3,kd)+wrbm(i1-1,i2,i3,kd))    +(wrbm(i1+2,i2,i3,kd)+
     & wrbm(i1-2,i2,i3,kd)) )/(dx(0)**4) +( 6.*wrbm(i1,i2,i3,kd)    -
     & 4.*(wrbm(i1,i2+1,i3,kd)+wrbm(i1,i2-1,i3,kd))    +(wrbm(i1,i2+2,
     & i3,kd)+wrbm(i1,i2-2,i3,kd)) )/(dx(1)**4)  +( 8.*wrbm(i1,i2,i3,
     & kd)     -4.*(wrbm(i1+1,i2,i3,kd)+wrbm(i1-1,i2,i3,kd)+wrbm(i1,
     & i2+1,i3,kd)+wrbm(i1,i2-1,i3,kd))   +2.*(wrbm(i1+1,i2+1,i3,kd)+
     & wrbm(i1-1,i2+1,i3,kd)+wrbm(i1+1,i2-1,i3,kd)+wrbm(i1-1,i2-1,i3,
     & kd)) )/(dx(0)**2*dx(1)**2)
      wrbmxxx23r(i1,i2,i3,kd)=(-2.*(wrbm(i1+1,i2,i3,kd)-wrbm(i1-1,i2,
     & i3,kd))+(wrbm(i1+2,i2,i3,kd)-wrbm(i1-2,i2,i3,kd)) )*h22(0)*h12(
     & 0)
      wrbmyyy23r(i1,i2,i3,kd)=(-2.*(wrbm(i1,i2+1,i3,kd)-wrbm(i1,i2-1,
     & i3,kd))+(wrbm(i1,i2+2,i3,kd)-wrbm(i1,i2-2,i3,kd)) )*h22(1)*h12(
     & 1)
      wrbmzzz23r(i1,i2,i3,kd)=(-2.*(wrbm(i1,i2,i3+1,kd)-wrbm(i1,i2,i3-
     & 1,kd))+(wrbm(i1,i2,i3+2,kd)-wrbm(i1,i2,i3-2,kd)) )*h22(1)*h12(
     & 2)
      wrbmxxy23r(i1,i2,i3,kd)=( wrbmxx22r(i1,i2+1,i3,kd)-wrbmxx22r(i1,
     & i2-1,i3,kd))/(2.*dx(1))
      wrbmxyy23r(i1,i2,i3,kd)=( wrbmyy22r(i1+1,i2,i3,kd)-wrbmyy22r(i1-
     & 1,i2,i3,kd))/(2.*dx(0))
      wrbmxxz23r(i1,i2,i3,kd)=( wrbmxx22r(i1,i2,i3+1,kd)-wrbmxx22r(i1,
     & i2,i3-1,kd))/(2.*dx(2))
      wrbmyyz23r(i1,i2,i3,kd)=( wrbmyy22r(i1,i2,i3+1,kd)-wrbmyy22r(i1,
     & i2,i3-1,kd))/(2.*dx(2))
      wrbmxzz23r(i1,i2,i3,kd)=( wrbmzz22r(i1+1,i2,i3,kd)-wrbmzz22r(i1-
     & 1,i2,i3,kd))/(2.*dx(0))
      wrbmyzz23r(i1,i2,i3,kd)=( wrbmzz22r(i1,i2+1,i3,kd)-wrbmzz22r(i1,
     & i2-1,i3,kd))/(2.*dx(1))
      wrbmxxxx23r(i1,i2,i3,kd)=(6.*wrbm(i1,i2,i3,kd)-4.*(wrbm(i1+1,i2,
     & i3,kd)+wrbm(i1-1,i2,i3,kd))+(wrbm(i1+2,i2,i3,kd)+wrbm(i1-2,i2,
     & i3,kd)) )/(dx(0)**4)
      wrbmyyyy23r(i1,i2,i3,kd)=(6.*wrbm(i1,i2,i3,kd)-4.*(wrbm(i1,i2+1,
     & i3,kd)+wrbm(i1,i2-1,i3,kd))+(wrbm(i1,i2+2,i3,kd)+wrbm(i1,i2-2,
     & i3,kd)) )/(dx(1)**4)
      wrbmzzzz23r(i1,i2,i3,kd)=(6.*wrbm(i1,i2,i3,kd)-4.*(wrbm(i1,i2,i3+
     & 1,kd)+wrbm(i1,i2,i3-1,kd))+(wrbm(i1,i2,i3+2,kd)+wrbm(i1,i2,i3-
     & 2,kd)) )/(dx(2)**4)
      wrbmxxyy23r(i1,i2,i3,kd)=( 4.*wrbm(i1,i2,i3,kd)     -2.*(wrbm(i1+
     & 1,i2,i3,kd)+wrbm(i1-1,i2,i3,kd)+wrbm(i1,i2+1,i3,kd)+wrbm(i1,i2-
     & 1,i3,kd))   +   (wrbm(i1+1,i2+1,i3,kd)+wrbm(i1-1,i2+1,i3,kd)+
     & wrbm(i1+1,i2-1,i3,kd)+wrbm(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)*
     & *2)
      wrbmxxzz23r(i1,i2,i3,kd)=( 4.*wrbm(i1,i2,i3,kd)     -2.*(wrbm(i1+
     & 1,i2,i3,kd)+wrbm(i1-1,i2,i3,kd)+wrbm(i1,i2,i3+1,kd)+wrbm(i1,i2,
     & i3-1,kd))   +   (wrbm(i1+1,i2,i3+1,kd)+wrbm(i1-1,i2,i3+1,kd)+
     & wrbm(i1+1,i2,i3-1,kd)+wrbm(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)*
     & *2)
      wrbmyyzz23r(i1,i2,i3,kd)=( 4.*wrbm(i1,i2,i3,kd)     -2.*(wrbm(i1,
     & i2+1,i3,kd)  +wrbm(i1,i2-1,i3,kd)+  wrbm(i1,i2  ,i3+1,kd)+wrbm(
     & i1,i2  ,i3-1,kd))   +   (wrbm(i1,i2+1,i3+1,kd)+wrbm(i1,i2-1,i3+
     & 1,kd)+wrbm(i1,i2+1,i3-1,kd)+wrbm(i1,i2-1,i3-1,kd)) )/(dx(1)**2*
     & dx(2)**2)
      ! 3D laplacian squared = wrbm.xxxx + wrbm.yyyy + wrbm.zzzz + 2 (wrbm.xxyy + wrbm.xxzz + wrbm.yyzz )
      wrbmLapSq23r(i1,i2,i3,kd)= ( 6.*wrbm(i1,i2,i3,kd)   - 4.*(wrbm(
     & i1+1,i2,i3,kd)+wrbm(i1-1,i2,i3,kd))    +(wrbm(i1+2,i2,i3,kd)+
     & wrbm(i1-2,i2,i3,kd)) )/(dx(0)**4) +( 6.*wrbm(i1,i2,i3,kd)    -
     & 4.*(wrbm(i1,i2+1,i3,kd)+wrbm(i1,i2-1,i3,kd))    +(wrbm(i1,i2+2,
     & i3,kd)+wrbm(i1,i2-2,i3,kd)) )/(dx(1)**4)  +( 6.*wrbm(i1,i2,i3,
     & kd)    -4.*(wrbm(i1,i2,i3+1,kd)+wrbm(i1,i2,i3-1,kd))    +(wrbm(
     & i1,i2,i3+2,kd)+wrbm(i1,i2,i3-2,kd)) )/(dx(2)**4)  +( 8.*wrbm(
     & i1,i2,i3,kd)     -4.*(wrbm(i1+1,i2,i3,kd)  +wrbm(i1-1,i2,i3,kd)
     &   +wrbm(i1  ,i2+1,i3,kd)+wrbm(i1  ,i2-1,i3,kd))   +2.*(wrbm(i1+
     & 1,i2+1,i3,kd)+wrbm(i1-1,i2+1,i3,kd)+wrbm(i1+1,i2-1,i3,kd)+wrbm(
     & i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)+( 8.*wrbm(i1,i2,i3,kd) 
     &     -4.*(wrbm(i1+1,i2,i3,kd)  +wrbm(i1-1,i2,i3,kd)  +wrbm(i1  ,
     & i2,i3+1,kd)+wrbm(i1  ,i2,i3-1,kd))   +2.*(wrbm(i1+1,i2,i3+1,kd)
     & +wrbm(i1-1,i2,i3+1,kd)+wrbm(i1+1,i2,i3-1,kd)+wrbm(i1-1,i2,i3-1,
     & kd)) )/(dx(0)**2*dx(2)**2)+( 8.*wrbm(i1,i2,i3,kd)     -4.*(
     & wrbm(i1,i2+1,i3,kd)  +wrbm(i1,i2-1,i3,kd)  +wrbm(i1,i2  ,i3+1,
     & kd)+wrbm(i1,i2  ,i3-1,kd))   +2.*(wrbm(i1,i2+1,i3+1,kd)+wrbm(
     & i1,i2-1,i3+1,kd)+wrbm(i1,i2+1,i3-1,kd)+wrbm(i1,i2-1,i3-1,kd)) )
     & /(dx(1)**2*dx(2)**2)

      vsar2(i1,i2,i3,kd)=(vsa(i1+1,i2,i3,kd)-vsa(i1-1,i2,i3,kd))*d12(0)
      vsas2(i1,i2,i3,kd)=(vsa(i1,i2+1,i3,kd)-vsa(i1,i2-1,i3,kd))*d12(1)
      vsat2(i1,i2,i3,kd)=(vsa(i1,i2,i3+1,kd)-vsa(i1,i2,i3-1,kd))*d12(2)
      vsarr2(i1,i2,i3,kd)=(-2.*vsa(i1,i2,i3,kd)+(vsa(i1+1,i2,i3,kd)+
     & vsa(i1-1,i2,i3,kd)) )*d22(0)
      vsass2(i1,i2,i3,kd)=(-2.*vsa(i1,i2,i3,kd)+(vsa(i1,i2+1,i3,kd)+
     & vsa(i1,i2-1,i3,kd)) )*d22(1)
      vsars2(i1,i2,i3,kd)=(vsar2(i1,i2+1,i3,kd)-vsar2(i1,i2-1,i3,kd))*
     & d12(1)
      vsatt2(i1,i2,i3,kd)=(-2.*vsa(i1,i2,i3,kd)+(vsa(i1,i2,i3+1,kd)+
     & vsa(i1,i2,i3-1,kd)) )*d22(2)
      vsart2(i1,i2,i3,kd)=(vsar2(i1,i2,i3+1,kd)-vsar2(i1,i2,i3-1,kd))*
     & d12(2)
      vsast2(i1,i2,i3,kd)=(vsas2(i1,i2,i3+1,kd)-vsas2(i1,i2,i3-1,kd))*
     & d12(2)
      vsarrr2(i1,i2,i3,kd)=(-2.*(vsa(i1+1,i2,i3,kd)-vsa(i1-1,i2,i3,kd))
     & +(vsa(i1+2,i2,i3,kd)-vsa(i1-2,i2,i3,kd)) )*d22(0)*d12(0)
      vsasss2(i1,i2,i3,kd)=(-2.*(vsa(i1,i2+1,i3,kd)-vsa(i1,i2-1,i3,kd))
     & +(vsa(i1,i2+2,i3,kd)-vsa(i1,i2-2,i3,kd)) )*d22(1)*d12(1)
      vsattt2(i1,i2,i3,kd)=(-2.*(vsa(i1,i2,i3+1,kd)-vsa(i1,i2,i3-1,kd))
     & +(vsa(i1,i2,i3+2,kd)-vsa(i1,i2,i3-2,kd)) )*d22(2)*d12(2)
      vsax21(i1,i2,i3,kd)= rx(i1,i2,i3)*vsar2(i1,i2,i3,kd)
      vsay21(i1,i2,i3,kd)=0
      vsaz21(i1,i2,i3,kd)=0
      vsax22(i1,i2,i3,kd)= rx(i1,i2,i3)*vsar2(i1,i2,i3,kd)+sx(i1,i2,i3)
     & *vsas2(i1,i2,i3,kd)
      vsay22(i1,i2,i3,kd)= ry(i1,i2,i3)*vsar2(i1,i2,i3,kd)+sy(i1,i2,i3)
     & *vsas2(i1,i2,i3,kd)
      vsaz22(i1,i2,i3,kd)=0
      vsax23(i1,i2,i3,kd)=rx(i1,i2,i3)*vsar2(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & vsas2(i1,i2,i3,kd)+tx(i1,i2,i3)*vsat2(i1,i2,i3,kd)
      vsay23(i1,i2,i3,kd)=ry(i1,i2,i3)*vsar2(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & vsas2(i1,i2,i3,kd)+ty(i1,i2,i3)*vsat2(i1,i2,i3,kd)
      vsaz23(i1,i2,i3,kd)=rz(i1,i2,i3)*vsar2(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & vsas2(i1,i2,i3,kd)+tz(i1,i2,i3)*vsat2(i1,i2,i3,kd)
      vsaxx21(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vsarr2(i1,i2,i3,kd)+(
     & rxx22(i1,i2,i3))*vsar2(i1,i2,i3,kd)
      vsayy21(i1,i2,i3,kd)=0
      vsaxy21(i1,i2,i3,kd)=0
      vsaxz21(i1,i2,i3,kd)=0
      vsayz21(i1,i2,i3,kd)=0
      vsazz21(i1,i2,i3,kd)=0
      vsalaplacian21(i1,i2,i3,kd)=vsaxx21(i1,i2,i3,kd)
      vsaxx22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vsarr2(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*vsars2(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*vsass2(i1,i2,i3,kd)+(rxx22(i1,i2,i3))*vsar2(i1,i2,i3,kd)+(
     & sxx22(i1,i2,i3))*vsas2(i1,i2,i3,kd)
      vsayy22(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*vsarr2(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*vsars2(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*vsass2(i1,i2,i3,kd)+(ryy22(i1,i2,i3))*vsar2(i1,i2,i3,kd)+(
     & syy22(i1,i2,i3))*vsas2(i1,i2,i3,kd)
      vsaxy22(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vsarr2(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & vsars2(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vsass2(i1,i2,i3,
     & kd)+rxy22(i1,i2,i3)*vsar2(i1,i2,i3,kd)+sxy22(i1,i2,i3)*vsas2(
     & i1,i2,i3,kd)
      vsaxz22(i1,i2,i3,kd)=0
      vsayz22(i1,i2,i3,kd)=0
      vsazz22(i1,i2,i3,kd)=0
      vsalaplacian22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & vsarr2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)
     & *sy(i1,i2,i3))*vsars2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2)*vsass2(i1,i2,i3,kd)+(rxx22(i1,i2,i3)+ryy22(i1,i2,i3))*
     & vsar2(i1,i2,i3,kd)+(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*vsas2(i1,
     & i2,i3,kd)
      vsaxx23(i1,i2,i3,kd)=rx(i1,i2,i3)**2*vsarr2(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*vsass2(i1,i2,i3,kd)+tx(i1,i2,i3)**2*vsatt2(i1,i2,i3,
     & kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*vsars2(i1,i2,i3,kd)+2.*rx(i1,
     & i2,i3)*tx(i1,i2,i3)*vsart2(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,
     & i2,i3)*vsast2(i1,i2,i3,kd)+rxx23(i1,i2,i3)*vsar2(i1,i2,i3,kd)+
     & sxx23(i1,i2,i3)*vsas2(i1,i2,i3,kd)+txx23(i1,i2,i3)*vsat2(i1,i2,
     & i3,kd)
      vsayy23(i1,i2,i3,kd)=ry(i1,i2,i3)**2*vsarr2(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*vsass2(i1,i2,i3,kd)+ty(i1,i2,i3)**2*vsatt2(i1,i2,i3,
     & kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*vsars2(i1,i2,i3,kd)+2.*ry(i1,
     & i2,i3)*ty(i1,i2,i3)*vsart2(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,
     & i2,i3)*vsast2(i1,i2,i3,kd)+ryy23(i1,i2,i3)*vsar2(i1,i2,i3,kd)+
     & syy23(i1,i2,i3)*vsas2(i1,i2,i3,kd)+tyy23(i1,i2,i3)*vsat2(i1,i2,
     & i3,kd)
      vsazz23(i1,i2,i3,kd)=rz(i1,i2,i3)**2*vsarr2(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*vsass2(i1,i2,i3,kd)+tz(i1,i2,i3)**2*vsatt2(i1,i2,i3,
     & kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*vsars2(i1,i2,i3,kd)+2.*rz(i1,
     & i2,i3)*tz(i1,i2,i3)*vsart2(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,
     & i2,i3)*vsast2(i1,i2,i3,kd)+rzz23(i1,i2,i3)*vsar2(i1,i2,i3,kd)+
     & szz23(i1,i2,i3)*vsas2(i1,i2,i3,kd)+tzz23(i1,i2,i3)*vsat2(i1,i2,
     & i3,kd)
      vsaxy23(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vsarr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vsass2(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & ty(i1,i2,i3)*vsatt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(
     & i1,i2,i3)*sx(i1,i2,i3))*vsars2(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(
     & i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*vsart2(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*vsast2(i1,i2,
     & i3,kd)+rxy23(i1,i2,i3)*vsar2(i1,i2,i3,kd)+sxy23(i1,i2,i3)*
     & vsas2(i1,i2,i3,kd)+txy23(i1,i2,i3)*vsat2(i1,i2,i3,kd)
      vsaxz23(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*vsarr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*vsass2(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & tz(i1,i2,i3)*vsatt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sx(i1,i2,i3))*vsars2(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*vsart2(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*vsast2(i1,i2,
     & i3,kd)+rxz23(i1,i2,i3)*vsar2(i1,i2,i3,kd)+sxz23(i1,i2,i3)*
     & vsas2(i1,i2,i3,kd)+txz23(i1,i2,i3)*vsat2(i1,i2,i3,kd)
      vsayz23(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*vsarr2(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*vsass2(i1,i2,i3,kd)+ty(i1,i2,i3)*
     & tz(i1,i2,i3)*vsatt2(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sy(i1,i2,i3))*vsars2(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*vsart2(i1,i2,i3,kd)+(sy(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*vsast2(i1,i2,
     & i3,kd)+ryz23(i1,i2,i3)*vsar2(i1,i2,i3,kd)+syz23(i1,i2,i3)*
     & vsas2(i1,i2,i3,kd)+tyz23(i1,i2,i3)*vsat2(i1,i2,i3,kd)
      vsalaplacian23(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*vsarr2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)
     & **2+sz(i1,i2,i3)**2)*vsass2(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(
     & i1,i2,i3)**2+tz(i1,i2,i3)**2)*vsatt2(i1,i2,i3,kd)+2.*(rx(i1,i2,
     & i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,
     & i2,i3))*vsars2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(
     & i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*vsart2(i1,i2,
     & i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)
     & +sz(i1,i2,i3)*tz(i1,i2,i3))*vsast2(i1,i2,i3,kd)+(rxx23(i1,i2,
     & i3)+ryy23(i1,i2,i3)+rzz23(i1,i2,i3))*vsar2(i1,i2,i3,kd)+(sxx23(
     & i1,i2,i3)+syy23(i1,i2,i3)+szz23(i1,i2,i3))*vsas2(i1,i2,i3,kd)+(
     & txx23(i1,i2,i3)+tyy23(i1,i2,i3)+tzz23(i1,i2,i3))*vsat2(i1,i2,
     & i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      vsax23r(i1,i2,i3,kd)=(vsa(i1+1,i2,i3,kd)-vsa(i1-1,i2,i3,kd))*h12(
     & 0)
      vsay23r(i1,i2,i3,kd)=(vsa(i1,i2+1,i3,kd)-vsa(i1,i2-1,i3,kd))*h12(
     & 1)
      vsaz23r(i1,i2,i3,kd)=(vsa(i1,i2,i3+1,kd)-vsa(i1,i2,i3-1,kd))*h12(
     & 2)
      vsaxx23r(i1,i2,i3,kd)=(-2.*vsa(i1,i2,i3,kd)+(vsa(i1+1,i2,i3,kd)+
     & vsa(i1-1,i2,i3,kd)) )*h22(0)
      vsayy23r(i1,i2,i3,kd)=(-2.*vsa(i1,i2,i3,kd)+(vsa(i1,i2+1,i3,kd)+
     & vsa(i1,i2-1,i3,kd)) )*h22(1)
      vsaxy23r(i1,i2,i3,kd)=(vsax23r(i1,i2+1,i3,kd)-vsax23r(i1,i2-1,i3,
     & kd))*h12(1)
      vsazz23r(i1,i2,i3,kd)=(-2.*vsa(i1,i2,i3,kd)+(vsa(i1,i2,i3+1,kd)+
     & vsa(i1,i2,i3-1,kd)) )*h22(2)
      vsaxz23r(i1,i2,i3,kd)=(vsax23r(i1,i2,i3+1,kd)-vsax23r(i1,i2,i3-1,
     & kd))*h12(2)
      vsayz23r(i1,i2,i3,kd)=(vsay23r(i1,i2,i3+1,kd)-vsay23r(i1,i2,i3-1,
     & kd))*h12(2)
      vsax21r(i1,i2,i3,kd)= vsax23r(i1,i2,i3,kd)
      vsay21r(i1,i2,i3,kd)= vsay23r(i1,i2,i3,kd)
      vsaz21r(i1,i2,i3,kd)= vsaz23r(i1,i2,i3,kd)
      vsaxx21r(i1,i2,i3,kd)= vsaxx23r(i1,i2,i3,kd)
      vsayy21r(i1,i2,i3,kd)= vsayy23r(i1,i2,i3,kd)
      vsazz21r(i1,i2,i3,kd)= vsazz23r(i1,i2,i3,kd)
      vsaxy21r(i1,i2,i3,kd)= vsaxy23r(i1,i2,i3,kd)
      vsaxz21r(i1,i2,i3,kd)= vsaxz23r(i1,i2,i3,kd)
      vsayz21r(i1,i2,i3,kd)= vsayz23r(i1,i2,i3,kd)
      vsalaplacian21r(i1,i2,i3,kd)=vsaxx23r(i1,i2,i3,kd)
      vsax22r(i1,i2,i3,kd)= vsax23r(i1,i2,i3,kd)
      vsay22r(i1,i2,i3,kd)= vsay23r(i1,i2,i3,kd)
      vsaz22r(i1,i2,i3,kd)= vsaz23r(i1,i2,i3,kd)
      vsaxx22r(i1,i2,i3,kd)= vsaxx23r(i1,i2,i3,kd)
      vsayy22r(i1,i2,i3,kd)= vsayy23r(i1,i2,i3,kd)
      vsazz22r(i1,i2,i3,kd)= vsazz23r(i1,i2,i3,kd)
      vsaxy22r(i1,i2,i3,kd)= vsaxy23r(i1,i2,i3,kd)
      vsaxz22r(i1,i2,i3,kd)= vsaxz23r(i1,i2,i3,kd)
      vsayz22r(i1,i2,i3,kd)= vsayz23r(i1,i2,i3,kd)
      vsalaplacian22r(i1,i2,i3,kd)=vsaxx23r(i1,i2,i3,kd)+vsayy23r(i1,
     & i2,i3,kd)
      vsalaplacian23r(i1,i2,i3,kd)=vsaxx23r(i1,i2,i3,kd)+vsayy23r(i1,
     & i2,i3,kd)+vsazz23r(i1,i2,i3,kd)
      vsaxxx22r(i1,i2,i3,kd)=(-2.*(vsa(i1+1,i2,i3,kd)-vsa(i1-1,i2,i3,
     & kd))+(vsa(i1+2,i2,i3,kd)-vsa(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
      vsayyy22r(i1,i2,i3,kd)=(-2.*(vsa(i1,i2+1,i3,kd)-vsa(i1,i2-1,i3,
     & kd))+(vsa(i1,i2+2,i3,kd)-vsa(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
      vsaxxy22r(i1,i2,i3,kd)=( vsaxx22r(i1,i2+1,i3,kd)-vsaxx22r(i1,i2-
     & 1,i3,kd))/(2.*dx(1))
      vsaxyy22r(i1,i2,i3,kd)=( vsayy22r(i1+1,i2,i3,kd)-vsayy22r(i1-1,
     & i2,i3,kd))/(2.*dx(0))
      vsaxxxx22r(i1,i2,i3,kd)=(6.*vsa(i1,i2,i3,kd)-4.*(vsa(i1+1,i2,i3,
     & kd)+vsa(i1-1,i2,i3,kd))+(vsa(i1+2,i2,i3,kd)+vsa(i1-2,i2,i3,kd))
     &  )/(dx(0)**4)
      vsayyyy22r(i1,i2,i3,kd)=(6.*vsa(i1,i2,i3,kd)-4.*(vsa(i1,i2+1,i3,
     & kd)+vsa(i1,i2-1,i3,kd))+(vsa(i1,i2+2,i3,kd)+vsa(i1,i2-2,i3,kd))
     &  )/(dx(1)**4)
      vsaxxyy22r(i1,i2,i3,kd)=( 4.*vsa(i1,i2,i3,kd)     -2.*(vsa(i1+1,
     & i2,i3,kd)+vsa(i1-1,i2,i3,kd)+vsa(i1,i2+1,i3,kd)+vsa(i1,i2-1,i3,
     & kd))   +   (vsa(i1+1,i2+1,i3,kd)+vsa(i1-1,i2+1,i3,kd)+vsa(i1+1,
     & i2-1,i3,kd)+vsa(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      ! 2D laplacian squared = vsa.xxxx + 2 vsa.xxyy + vsa.yyyy
      vsaLapSq22r(i1,i2,i3,kd)= ( 6.*vsa(i1,i2,i3,kd)   - 4.*(vsa(i1+1,
     & i2,i3,kd)+vsa(i1-1,i2,i3,kd))    +(vsa(i1+2,i2,i3,kd)+vsa(i1-2,
     & i2,i3,kd)) )/(dx(0)**4) +( 6.*vsa(i1,i2,i3,kd)    -4.*(vsa(i1,
     & i2+1,i3,kd)+vsa(i1,i2-1,i3,kd))    +(vsa(i1,i2+2,i3,kd)+vsa(i1,
     & i2-2,i3,kd)) )/(dx(1)**4)  +( 8.*vsa(i1,i2,i3,kd)     -4.*(vsa(
     & i1+1,i2,i3,kd)+vsa(i1-1,i2,i3,kd)+vsa(i1,i2+1,i3,kd)+vsa(i1,i2-
     & 1,i3,kd))   +2.*(vsa(i1+1,i2+1,i3,kd)+vsa(i1-1,i2+1,i3,kd)+vsa(
     & i1+1,i2-1,i3,kd)+vsa(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      vsaxxx23r(i1,i2,i3,kd)=(-2.*(vsa(i1+1,i2,i3,kd)-vsa(i1-1,i2,i3,
     & kd))+(vsa(i1+2,i2,i3,kd)-vsa(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
      vsayyy23r(i1,i2,i3,kd)=(-2.*(vsa(i1,i2+1,i3,kd)-vsa(i1,i2-1,i3,
     & kd))+(vsa(i1,i2+2,i3,kd)-vsa(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
      vsazzz23r(i1,i2,i3,kd)=(-2.*(vsa(i1,i2,i3+1,kd)-vsa(i1,i2,i3-1,
     & kd))+(vsa(i1,i2,i3+2,kd)-vsa(i1,i2,i3-2,kd)) )*h22(1)*h12(2)
      vsaxxy23r(i1,i2,i3,kd)=( vsaxx22r(i1,i2+1,i3,kd)-vsaxx22r(i1,i2-
     & 1,i3,kd))/(2.*dx(1))
      vsaxyy23r(i1,i2,i3,kd)=( vsayy22r(i1+1,i2,i3,kd)-vsayy22r(i1-1,
     & i2,i3,kd))/(2.*dx(0))
      vsaxxz23r(i1,i2,i3,kd)=( vsaxx22r(i1,i2,i3+1,kd)-vsaxx22r(i1,i2,
     & i3-1,kd))/(2.*dx(2))
      vsayyz23r(i1,i2,i3,kd)=( vsayy22r(i1,i2,i3+1,kd)-vsayy22r(i1,i2,
     & i3-1,kd))/(2.*dx(2))
      vsaxzz23r(i1,i2,i3,kd)=( vsazz22r(i1+1,i2,i3,kd)-vsazz22r(i1-1,
     & i2,i3,kd))/(2.*dx(0))
      vsayzz23r(i1,i2,i3,kd)=( vsazz22r(i1,i2+1,i3,kd)-vsazz22r(i1,i2-
     & 1,i3,kd))/(2.*dx(1))
      vsaxxxx23r(i1,i2,i3,kd)=(6.*vsa(i1,i2,i3,kd)-4.*(vsa(i1+1,i2,i3,
     & kd)+vsa(i1-1,i2,i3,kd))+(vsa(i1+2,i2,i3,kd)+vsa(i1-2,i2,i3,kd))
     &  )/(dx(0)**4)
      vsayyyy23r(i1,i2,i3,kd)=(6.*vsa(i1,i2,i3,kd)-4.*(vsa(i1,i2+1,i3,
     & kd)+vsa(i1,i2-1,i3,kd))+(vsa(i1,i2+2,i3,kd)+vsa(i1,i2-2,i3,kd))
     &  )/(dx(1)**4)
      vsazzzz23r(i1,i2,i3,kd)=(6.*vsa(i1,i2,i3,kd)-4.*(vsa(i1,i2,i3+1,
     & kd)+vsa(i1,i2,i3-1,kd))+(vsa(i1,i2,i3+2,kd)+vsa(i1,i2,i3-2,kd))
     &  )/(dx(2)**4)
      vsaxxyy23r(i1,i2,i3,kd)=( 4.*vsa(i1,i2,i3,kd)     -2.*(vsa(i1+1,
     & i2,i3,kd)+vsa(i1-1,i2,i3,kd)+vsa(i1,i2+1,i3,kd)+vsa(i1,i2-1,i3,
     & kd))   +   (vsa(i1+1,i2+1,i3,kd)+vsa(i1-1,i2+1,i3,kd)+vsa(i1+1,
     & i2-1,i3,kd)+vsa(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      vsaxxzz23r(i1,i2,i3,kd)=( 4.*vsa(i1,i2,i3,kd)     -2.*(vsa(i1+1,
     & i2,i3,kd)+vsa(i1-1,i2,i3,kd)+vsa(i1,i2,i3+1,kd)+vsa(i1,i2,i3-1,
     & kd))   +   (vsa(i1+1,i2,i3+1,kd)+vsa(i1-1,i2,i3+1,kd)+vsa(i1+1,
     & i2,i3-1,kd)+vsa(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)
      vsayyzz23r(i1,i2,i3,kd)=( 4.*vsa(i1,i2,i3,kd)     -2.*(vsa(i1,i2+
     & 1,i3,kd)  +vsa(i1,i2-1,i3,kd)+  vsa(i1,i2  ,i3+1,kd)+vsa(i1,i2 
     &  ,i3-1,kd))   +   (vsa(i1,i2+1,i3+1,kd)+vsa(i1,i2-1,i3+1,kd)+
     & vsa(i1,i2+1,i3-1,kd)+vsa(i1,i2-1,i3-1,kd)) )/(dx(1)**2*dx(2)**
     & 2)
      ! 3D laplacian squared = vsa.xxxx + vsa.yyyy + vsa.zzzz + 2 (vsa.xxyy + vsa.xxzz + vsa.yyzz )
      vsaLapSq23r(i1,i2,i3,kd)= ( 6.*vsa(i1,i2,i3,kd)   - 4.*(vsa(i1+1,
     & i2,i3,kd)+vsa(i1-1,i2,i3,kd))    +(vsa(i1+2,i2,i3,kd)+vsa(i1-2,
     & i2,i3,kd)) )/(dx(0)**4) +( 6.*vsa(i1,i2,i3,kd)    -4.*(vsa(i1,
     & i2+1,i3,kd)+vsa(i1,i2-1,i3,kd))    +(vsa(i1,i2+2,i3,kd)+vsa(i1,
     & i2-2,i3,kd)) )/(dx(1)**4)  +( 6.*vsa(i1,i2,i3,kd)    -4.*(vsa(
     & i1,i2,i3+1,kd)+vsa(i1,i2,i3-1,kd))    +(vsa(i1,i2,i3+2,kd)+vsa(
     & i1,i2,i3-2,kd)) )/(dx(2)**4)  +( 8.*vsa(i1,i2,i3,kd)     -4.*(
     & vsa(i1+1,i2,i3,kd)  +vsa(i1-1,i2,i3,kd)  +vsa(i1  ,i2+1,i3,kd)+
     & vsa(i1  ,i2-1,i3,kd))   +2.*(vsa(i1+1,i2+1,i3,kd)+vsa(i1-1,i2+
     & 1,i3,kd)+vsa(i1+1,i2-1,i3,kd)+vsa(i1-1,i2-1,i3,kd)) )/(dx(0)**
     & 2*dx(1)**2)+( 8.*vsa(i1,i2,i3,kd)     -4.*(vsa(i1+1,i2,i3,kd)  
     & +vsa(i1-1,i2,i3,kd)  +vsa(i1  ,i2,i3+1,kd)+vsa(i1  ,i2,i3-1,kd)
     & )   +2.*(vsa(i1+1,i2,i3+1,kd)+vsa(i1-1,i2,i3+1,kd)+vsa(i1+1,i2,
     & i3-1,kd)+vsa(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)+( 8.*vsa(
     & i1,i2,i3,kd)     -4.*(vsa(i1,i2+1,i3,kd)  +vsa(i1,i2-1,i3,kd)  
     & +vsa(i1,i2  ,i3+1,kd)+vsa(i1,i2  ,i3-1,kd))   +2.*(vsa(i1,i2+1,
     & i3+1,kd)+vsa(i1,i2-1,i3+1,kd)+vsa(i1,i2+1,i3-1,kd)+vsa(i1,i2-1,
     & i3-1,kd)) )/(dx(1)**2*dx(2)**2)
      vsamr2(i1,i2,i3,kd)=(vsam(i1+1,i2,i3,kd)-vsam(i1-1,i2,i3,kd))*
     & d12(0)
      vsams2(i1,i2,i3,kd)=(vsam(i1,i2+1,i3,kd)-vsam(i1,i2-1,i3,kd))*
     & d12(1)
      vsamt2(i1,i2,i3,kd)=(vsam(i1,i2,i3+1,kd)-vsam(i1,i2,i3-1,kd))*
     & d12(2)
      vsamrr2(i1,i2,i3,kd)=(-2.*vsam(i1,i2,i3,kd)+(vsam(i1+1,i2,i3,kd)+
     & vsam(i1-1,i2,i3,kd)) )*d22(0)
      vsamss2(i1,i2,i3,kd)=(-2.*vsam(i1,i2,i3,kd)+(vsam(i1,i2+1,i3,kd)+
     & vsam(i1,i2-1,i3,kd)) )*d22(1)
      vsamrs2(i1,i2,i3,kd)=(vsamr2(i1,i2+1,i3,kd)-vsamr2(i1,i2-1,i3,kd)
     & )*d12(1)
      vsamtt2(i1,i2,i3,kd)=(-2.*vsam(i1,i2,i3,kd)+(vsam(i1,i2,i3+1,kd)+
     & vsam(i1,i2,i3-1,kd)) )*d22(2)
      vsamrt2(i1,i2,i3,kd)=(vsamr2(i1,i2,i3+1,kd)-vsamr2(i1,i2,i3-1,kd)
     & )*d12(2)
      vsamst2(i1,i2,i3,kd)=(vsams2(i1,i2,i3+1,kd)-vsams2(i1,i2,i3-1,kd)
     & )*d12(2)
      vsamrrr2(i1,i2,i3,kd)=(-2.*(vsam(i1+1,i2,i3,kd)-vsam(i1-1,i2,i3,
     & kd))+(vsam(i1+2,i2,i3,kd)-vsam(i1-2,i2,i3,kd)) )*d22(0)*d12(0)
      vsamsss2(i1,i2,i3,kd)=(-2.*(vsam(i1,i2+1,i3,kd)-vsam(i1,i2-1,i3,
     & kd))+(vsam(i1,i2+2,i3,kd)-vsam(i1,i2-2,i3,kd)) )*d22(1)*d12(1)
      vsamttt2(i1,i2,i3,kd)=(-2.*(vsam(i1,i2,i3+1,kd)-vsam(i1,i2,i3-1,
     & kd))+(vsam(i1,i2,i3+2,kd)-vsam(i1,i2,i3-2,kd)) )*d22(2)*d12(2)
      vsamx21(i1,i2,i3,kd)= rx(i1,i2,i3)*vsamr2(i1,i2,i3,kd)
      vsamy21(i1,i2,i3,kd)=0
      vsamz21(i1,i2,i3,kd)=0
      vsamx22(i1,i2,i3,kd)= rx(i1,i2,i3)*vsamr2(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*vsams2(i1,i2,i3,kd)
      vsamy22(i1,i2,i3,kd)= ry(i1,i2,i3)*vsamr2(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*vsams2(i1,i2,i3,kd)
      vsamz22(i1,i2,i3,kd)=0
      vsamx23(i1,i2,i3,kd)=rx(i1,i2,i3)*vsamr2(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*vsams2(i1,i2,i3,kd)+tx(i1,i2,i3)*vsamt2(i1,i2,i3,kd)
      vsamy23(i1,i2,i3,kd)=ry(i1,i2,i3)*vsamr2(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*vsams2(i1,i2,i3,kd)+ty(i1,i2,i3)*vsamt2(i1,i2,i3,kd)
      vsamz23(i1,i2,i3,kd)=rz(i1,i2,i3)*vsamr2(i1,i2,i3,kd)+sz(i1,i2,
     & i3)*vsams2(i1,i2,i3,kd)+tz(i1,i2,i3)*vsamt2(i1,i2,i3,kd)
      vsamxx21(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vsamrr2(i1,i2,i3,kd)+(
     & rxx22(i1,i2,i3))*vsamr2(i1,i2,i3,kd)
      vsamyy21(i1,i2,i3,kd)=0
      vsamxy21(i1,i2,i3,kd)=0
      vsamxz21(i1,i2,i3,kd)=0
      vsamyz21(i1,i2,i3,kd)=0
      vsamzz21(i1,i2,i3,kd)=0
      vsamlaplacian21(i1,i2,i3,kd)=vsamxx21(i1,i2,i3,kd)
      vsamxx22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vsamrr2(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*vsamrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*vsamss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3))*vsamr2(i1,i2,i3,kd)+(
     & sxx22(i1,i2,i3))*vsams2(i1,i2,i3,kd)
      vsamyy22(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*vsamrr2(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*vsamrs2(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*vsamss2(i1,i2,i3,kd)+(ryy22(i1,i2,i3))*vsamr2(i1,i2,i3,kd)+(
     & syy22(i1,i2,i3))*vsams2(i1,i2,i3,kd)
      vsamxy22(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vsamrr2(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & vsamrs2(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vsamss2(i1,i2,
     & i3,kd)+rxy22(i1,i2,i3)*vsamr2(i1,i2,i3,kd)+sxy22(i1,i2,i3)*
     & vsams2(i1,i2,i3,kd)
      vsamxz22(i1,i2,i3,kd)=0
      vsamyz22(i1,i2,i3,kd)=0
      vsamzz22(i1,i2,i3,kd)=0
      vsamlaplacian22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & vsamrr2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,
     & i3)*sy(i1,i2,i3))*vsamrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,
     & i2,i3)**2)*vsamss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3)+ryy22(i1,i2,
     & i3))*vsamr2(i1,i2,i3,kd)+(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*
     & vsams2(i1,i2,i3,kd)
      vsamxx23(i1,i2,i3,kd)=rx(i1,i2,i3)**2*vsamrr2(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*vsamss2(i1,i2,i3,kd)+tx(i1,i2,i3)**2*vsamtt2(i1,i2,
     & i3,kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*vsamrs2(i1,i2,i3,kd)+2.*rx(
     & i1,i2,i3)*tx(i1,i2,i3)*vsamrt2(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(
     & i1,i2,i3)*vsamst2(i1,i2,i3,kd)+rxx23(i1,i2,i3)*vsamr2(i1,i2,i3,
     & kd)+sxx23(i1,i2,i3)*vsams2(i1,i2,i3,kd)+txx23(i1,i2,i3)*vsamt2(
     & i1,i2,i3,kd)
      vsamyy23(i1,i2,i3,kd)=ry(i1,i2,i3)**2*vsamrr2(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*vsamss2(i1,i2,i3,kd)+ty(i1,i2,i3)**2*vsamtt2(i1,i2,
     & i3,kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*vsamrs2(i1,i2,i3,kd)+2.*ry(
     & i1,i2,i3)*ty(i1,i2,i3)*vsamrt2(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(
     & i1,i2,i3)*vsamst2(i1,i2,i3,kd)+ryy23(i1,i2,i3)*vsamr2(i1,i2,i3,
     & kd)+syy23(i1,i2,i3)*vsams2(i1,i2,i3,kd)+tyy23(i1,i2,i3)*vsamt2(
     & i1,i2,i3,kd)
      vsamzz23(i1,i2,i3,kd)=rz(i1,i2,i3)**2*vsamrr2(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*vsamss2(i1,i2,i3,kd)+tz(i1,i2,i3)**2*vsamtt2(i1,i2,
     & i3,kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*vsamrs2(i1,i2,i3,kd)+2.*rz(
     & i1,i2,i3)*tz(i1,i2,i3)*vsamrt2(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(
     & i1,i2,i3)*vsamst2(i1,i2,i3,kd)+rzz23(i1,i2,i3)*vsamr2(i1,i2,i3,
     & kd)+szz23(i1,i2,i3)*vsams2(i1,i2,i3,kd)+tzz23(i1,i2,i3)*vsamt2(
     & i1,i2,i3,kd)
      vsamxy23(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vsamrr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vsamss2(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *ty(i1,i2,i3)*vsamtt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+
     & ry(i1,i2,i3)*sx(i1,i2,i3))*vsamrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & ty(i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*vsamrt2(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*vsamst2(
     & i1,i2,i3,kd)+rxy23(i1,i2,i3)*vsamr2(i1,i2,i3,kd)+sxy23(i1,i2,
     & i3)*vsams2(i1,i2,i3,kd)+txy23(i1,i2,i3)*vsamt2(i1,i2,i3,kd)
      vsamxz23(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*vsamrr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*vsamss2(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *tz(i1,i2,i3)*vsamtt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sx(i1,i2,i3))*vsamrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*vsamrt2(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*vsamst2(
     & i1,i2,i3,kd)+rxz23(i1,i2,i3)*vsamr2(i1,i2,i3,kd)+sxz23(i1,i2,
     & i3)*vsams2(i1,i2,i3,kd)+txz23(i1,i2,i3)*vsamt2(i1,i2,i3,kd)
      vsamyz23(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*vsamrr2(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*vsamss2(i1,i2,i3,kd)+ty(i1,i2,i3)
     & *tz(i1,i2,i3)*vsamtt2(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sy(i1,i2,i3))*vsamrs2(i1,i2,i3,kd)+(ry(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*vsamrt2(i1,i2,i3,kd)+(
     & sy(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*vsamst2(
     & i1,i2,i3,kd)+ryz23(i1,i2,i3)*vsamr2(i1,i2,i3,kd)+syz23(i1,i2,
     & i3)*vsams2(i1,i2,i3,kd)+tyz23(i1,i2,i3)*vsamt2(i1,i2,i3,kd)
      vsamlaplacian23(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*vsamrr2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2+sz(i1,i2,i3)**2)*vsamss2(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+
     & ty(i1,i2,i3)**2+tz(i1,i2,i3)**2)*vsamtt2(i1,i2,i3,kd)+2.*(rx(
     & i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*
     & sz(i1,i2,i3))*vsamrs2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,
     & i3)+ ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*
     & vsamrt2(i1,i2,i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,
     & i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3))*vsamst2(i1,i2,i3,
     & kd)+(rxx23(i1,i2,i3)+ryy23(i1,i2,i3)+rzz23(i1,i2,i3))*vsamr2(
     & i1,i2,i3,kd)+(sxx23(i1,i2,i3)+syy23(i1,i2,i3)+szz23(i1,i2,i3))*
     & vsams2(i1,i2,i3,kd)+(txx23(i1,i2,i3)+tyy23(i1,i2,i3)+tzz23(i1,
     & i2,i3))*vsamt2(i1,i2,i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      vsamx23r(i1,i2,i3,kd)=(vsam(i1+1,i2,i3,kd)-vsam(i1-1,i2,i3,kd))*
     & h12(0)
      vsamy23r(i1,i2,i3,kd)=(vsam(i1,i2+1,i3,kd)-vsam(i1,i2-1,i3,kd))*
     & h12(1)
      vsamz23r(i1,i2,i3,kd)=(vsam(i1,i2,i3+1,kd)-vsam(i1,i2,i3-1,kd))*
     & h12(2)
      vsamxx23r(i1,i2,i3,kd)=(-2.*vsam(i1,i2,i3,kd)+(vsam(i1+1,i2,i3,
     & kd)+vsam(i1-1,i2,i3,kd)) )*h22(0)
      vsamyy23r(i1,i2,i3,kd)=(-2.*vsam(i1,i2,i3,kd)+(vsam(i1,i2+1,i3,
     & kd)+vsam(i1,i2-1,i3,kd)) )*h22(1)
      vsamxy23r(i1,i2,i3,kd)=(vsamx23r(i1,i2+1,i3,kd)-vsamx23r(i1,i2-1,
     & i3,kd))*h12(1)
      vsamzz23r(i1,i2,i3,kd)=(-2.*vsam(i1,i2,i3,kd)+(vsam(i1,i2,i3+1,
     & kd)+vsam(i1,i2,i3-1,kd)) )*h22(2)
      vsamxz23r(i1,i2,i3,kd)=(vsamx23r(i1,i2,i3+1,kd)-vsamx23r(i1,i2,
     & i3-1,kd))*h12(2)
      vsamyz23r(i1,i2,i3,kd)=(vsamy23r(i1,i2,i3+1,kd)-vsamy23r(i1,i2,
     & i3-1,kd))*h12(2)
      vsamx21r(i1,i2,i3,kd)= vsamx23r(i1,i2,i3,kd)
      vsamy21r(i1,i2,i3,kd)= vsamy23r(i1,i2,i3,kd)
      vsamz21r(i1,i2,i3,kd)= vsamz23r(i1,i2,i3,kd)
      vsamxx21r(i1,i2,i3,kd)= vsamxx23r(i1,i2,i3,kd)
      vsamyy21r(i1,i2,i3,kd)= vsamyy23r(i1,i2,i3,kd)
      vsamzz21r(i1,i2,i3,kd)= vsamzz23r(i1,i2,i3,kd)
      vsamxy21r(i1,i2,i3,kd)= vsamxy23r(i1,i2,i3,kd)
      vsamxz21r(i1,i2,i3,kd)= vsamxz23r(i1,i2,i3,kd)
      vsamyz21r(i1,i2,i3,kd)= vsamyz23r(i1,i2,i3,kd)
      vsamlaplacian21r(i1,i2,i3,kd)=vsamxx23r(i1,i2,i3,kd)
      vsamx22r(i1,i2,i3,kd)= vsamx23r(i1,i2,i3,kd)
      vsamy22r(i1,i2,i3,kd)= vsamy23r(i1,i2,i3,kd)
      vsamz22r(i1,i2,i3,kd)= vsamz23r(i1,i2,i3,kd)
      vsamxx22r(i1,i2,i3,kd)= vsamxx23r(i1,i2,i3,kd)
      vsamyy22r(i1,i2,i3,kd)= vsamyy23r(i1,i2,i3,kd)
      vsamzz22r(i1,i2,i3,kd)= vsamzz23r(i1,i2,i3,kd)
      vsamxy22r(i1,i2,i3,kd)= vsamxy23r(i1,i2,i3,kd)
      vsamxz22r(i1,i2,i3,kd)= vsamxz23r(i1,i2,i3,kd)
      vsamyz22r(i1,i2,i3,kd)= vsamyz23r(i1,i2,i3,kd)
      vsamlaplacian22r(i1,i2,i3,kd)=vsamxx23r(i1,i2,i3,kd)+vsamyy23r(
     & i1,i2,i3,kd)
      vsamlaplacian23r(i1,i2,i3,kd)=vsamxx23r(i1,i2,i3,kd)+vsamyy23r(
     & i1,i2,i3,kd)+vsamzz23r(i1,i2,i3,kd)
      vsamxxx22r(i1,i2,i3,kd)=(-2.*(vsam(i1+1,i2,i3,kd)-vsam(i1-1,i2,
     & i3,kd))+(vsam(i1+2,i2,i3,kd)-vsam(i1-2,i2,i3,kd)) )*h22(0)*h12(
     & 0)
      vsamyyy22r(i1,i2,i3,kd)=(-2.*(vsam(i1,i2+1,i3,kd)-vsam(i1,i2-1,
     & i3,kd))+(vsam(i1,i2+2,i3,kd)-vsam(i1,i2-2,i3,kd)) )*h22(1)*h12(
     & 1)
      vsamxxy22r(i1,i2,i3,kd)=( vsamxx22r(i1,i2+1,i3,kd)-vsamxx22r(i1,
     & i2-1,i3,kd))/(2.*dx(1))
      vsamxyy22r(i1,i2,i3,kd)=( vsamyy22r(i1+1,i2,i3,kd)-vsamyy22r(i1-
     & 1,i2,i3,kd))/(2.*dx(0))
      vsamxxxx22r(i1,i2,i3,kd)=(6.*vsam(i1,i2,i3,kd)-4.*(vsam(i1+1,i2,
     & i3,kd)+vsam(i1-1,i2,i3,kd))+(vsam(i1+2,i2,i3,kd)+vsam(i1-2,i2,
     & i3,kd)) )/(dx(0)**4)
      vsamyyyy22r(i1,i2,i3,kd)=(6.*vsam(i1,i2,i3,kd)-4.*(vsam(i1,i2+1,
     & i3,kd)+vsam(i1,i2-1,i3,kd))+(vsam(i1,i2+2,i3,kd)+vsam(i1,i2-2,
     & i3,kd)) )/(dx(1)**4)
      vsamxxyy22r(i1,i2,i3,kd)=( 4.*vsam(i1,i2,i3,kd)     -2.*(vsam(i1+
     & 1,i2,i3,kd)+vsam(i1-1,i2,i3,kd)+vsam(i1,i2+1,i3,kd)+vsam(i1,i2-
     & 1,i3,kd))   +   (vsam(i1+1,i2+1,i3,kd)+vsam(i1-1,i2+1,i3,kd)+
     & vsam(i1+1,i2-1,i3,kd)+vsam(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)*
     & *2)
      ! 2D laplacian squared = vsam.xxxx + 2 vsam.xxyy + vsam.yyyy
      vsamLapSq22r(i1,i2,i3,kd)= ( 6.*vsam(i1,i2,i3,kd)   - 4.*(vsam(
     & i1+1,i2,i3,kd)+vsam(i1-1,i2,i3,kd))    +(vsam(i1+2,i2,i3,kd)+
     & vsam(i1-2,i2,i3,kd)) )/(dx(0)**4) +( 6.*vsam(i1,i2,i3,kd)    -
     & 4.*(vsam(i1,i2+1,i3,kd)+vsam(i1,i2-1,i3,kd))    +(vsam(i1,i2+2,
     & i3,kd)+vsam(i1,i2-2,i3,kd)) )/(dx(1)**4)  +( 8.*vsam(i1,i2,i3,
     & kd)     -4.*(vsam(i1+1,i2,i3,kd)+vsam(i1-1,i2,i3,kd)+vsam(i1,
     & i2+1,i3,kd)+vsam(i1,i2-1,i3,kd))   +2.*(vsam(i1+1,i2+1,i3,kd)+
     & vsam(i1-1,i2+1,i3,kd)+vsam(i1+1,i2-1,i3,kd)+vsam(i1-1,i2-1,i3,
     & kd)) )/(dx(0)**2*dx(1)**2)
      vsamxxx23r(i1,i2,i3,kd)=(-2.*(vsam(i1+1,i2,i3,kd)-vsam(i1-1,i2,
     & i3,kd))+(vsam(i1+2,i2,i3,kd)-vsam(i1-2,i2,i3,kd)) )*h22(0)*h12(
     & 0)
      vsamyyy23r(i1,i2,i3,kd)=(-2.*(vsam(i1,i2+1,i3,kd)-vsam(i1,i2-1,
     & i3,kd))+(vsam(i1,i2+2,i3,kd)-vsam(i1,i2-2,i3,kd)) )*h22(1)*h12(
     & 1)
      vsamzzz23r(i1,i2,i3,kd)=(-2.*(vsam(i1,i2,i3+1,kd)-vsam(i1,i2,i3-
     & 1,kd))+(vsam(i1,i2,i3+2,kd)-vsam(i1,i2,i3-2,kd)) )*h22(1)*h12(
     & 2)
      vsamxxy23r(i1,i2,i3,kd)=( vsamxx22r(i1,i2+1,i3,kd)-vsamxx22r(i1,
     & i2-1,i3,kd))/(2.*dx(1))
      vsamxyy23r(i1,i2,i3,kd)=( vsamyy22r(i1+1,i2,i3,kd)-vsamyy22r(i1-
     & 1,i2,i3,kd))/(2.*dx(0))
      vsamxxz23r(i1,i2,i3,kd)=( vsamxx22r(i1,i2,i3+1,kd)-vsamxx22r(i1,
     & i2,i3-1,kd))/(2.*dx(2))
      vsamyyz23r(i1,i2,i3,kd)=( vsamyy22r(i1,i2,i3+1,kd)-vsamyy22r(i1,
     & i2,i3-1,kd))/(2.*dx(2))
      vsamxzz23r(i1,i2,i3,kd)=( vsamzz22r(i1+1,i2,i3,kd)-vsamzz22r(i1-
     & 1,i2,i3,kd))/(2.*dx(0))
      vsamyzz23r(i1,i2,i3,kd)=( vsamzz22r(i1,i2+1,i3,kd)-vsamzz22r(i1,
     & i2-1,i3,kd))/(2.*dx(1))
      vsamxxxx23r(i1,i2,i3,kd)=(6.*vsam(i1,i2,i3,kd)-4.*(vsam(i1+1,i2,
     & i3,kd)+vsam(i1-1,i2,i3,kd))+(vsam(i1+2,i2,i3,kd)+vsam(i1-2,i2,
     & i3,kd)) )/(dx(0)**4)
      vsamyyyy23r(i1,i2,i3,kd)=(6.*vsam(i1,i2,i3,kd)-4.*(vsam(i1,i2+1,
     & i3,kd)+vsam(i1,i2-1,i3,kd))+(vsam(i1,i2+2,i3,kd)+vsam(i1,i2-2,
     & i3,kd)) )/(dx(1)**4)
      vsamzzzz23r(i1,i2,i3,kd)=(6.*vsam(i1,i2,i3,kd)-4.*(vsam(i1,i2,i3+
     & 1,kd)+vsam(i1,i2,i3-1,kd))+(vsam(i1,i2,i3+2,kd)+vsam(i1,i2,i3-
     & 2,kd)) )/(dx(2)**4)
      vsamxxyy23r(i1,i2,i3,kd)=( 4.*vsam(i1,i2,i3,kd)     -2.*(vsam(i1+
     & 1,i2,i3,kd)+vsam(i1-1,i2,i3,kd)+vsam(i1,i2+1,i3,kd)+vsam(i1,i2-
     & 1,i3,kd))   +   (vsam(i1+1,i2+1,i3,kd)+vsam(i1-1,i2+1,i3,kd)+
     & vsam(i1+1,i2-1,i3,kd)+vsam(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)*
     & *2)
      vsamxxzz23r(i1,i2,i3,kd)=( 4.*vsam(i1,i2,i3,kd)     -2.*(vsam(i1+
     & 1,i2,i3,kd)+vsam(i1-1,i2,i3,kd)+vsam(i1,i2,i3+1,kd)+vsam(i1,i2,
     & i3-1,kd))   +   (vsam(i1+1,i2,i3+1,kd)+vsam(i1-1,i2,i3+1,kd)+
     & vsam(i1+1,i2,i3-1,kd)+vsam(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)*
     & *2)
      vsamyyzz23r(i1,i2,i3,kd)=( 4.*vsam(i1,i2,i3,kd)     -2.*(vsam(i1,
     & i2+1,i3,kd)  +vsam(i1,i2-1,i3,kd)+  vsam(i1,i2  ,i3+1,kd)+vsam(
     & i1,i2  ,i3-1,kd))   +   (vsam(i1,i2+1,i3+1,kd)+vsam(i1,i2-1,i3+
     & 1,kd)+vsam(i1,i2+1,i3-1,kd)+vsam(i1,i2-1,i3-1,kd)) )/(dx(1)**2*
     & dx(2)**2)
      ! 3D laplacian squared = vsam.xxxx + vsam.yyyy + vsam.zzzz + 2 (vsam.xxyy + vsam.xxzz + vsam.yyzz )
      vsamLapSq23r(i1,i2,i3,kd)= ( 6.*vsam(i1,i2,i3,kd)   - 4.*(vsam(
     & i1+1,i2,i3,kd)+vsam(i1-1,i2,i3,kd))    +(vsam(i1+2,i2,i3,kd)+
     & vsam(i1-2,i2,i3,kd)) )/(dx(0)**4) +( 6.*vsam(i1,i2,i3,kd)    -
     & 4.*(vsam(i1,i2+1,i3,kd)+vsam(i1,i2-1,i3,kd))    +(vsam(i1,i2+2,
     & i3,kd)+vsam(i1,i2-2,i3,kd)) )/(dx(1)**4)  +( 6.*vsam(i1,i2,i3,
     & kd)    -4.*(vsam(i1,i2,i3+1,kd)+vsam(i1,i2,i3-1,kd))    +(vsam(
     & i1,i2,i3+2,kd)+vsam(i1,i2,i3-2,kd)) )/(dx(2)**4)  +( 8.*vsam(
     & i1,i2,i3,kd)     -4.*(vsam(i1+1,i2,i3,kd)  +vsam(i1-1,i2,i3,kd)
     &   +vsam(i1  ,i2+1,i3,kd)+vsam(i1  ,i2-1,i3,kd))   +2.*(vsam(i1+
     & 1,i2+1,i3,kd)+vsam(i1-1,i2+1,i3,kd)+vsam(i1+1,i2-1,i3,kd)+vsam(
     & i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)+( 8.*vsam(i1,i2,i3,kd) 
     &     -4.*(vsam(i1+1,i2,i3,kd)  +vsam(i1-1,i2,i3,kd)  +vsam(i1  ,
     & i2,i3+1,kd)+vsam(i1  ,i2,i3-1,kd))   +2.*(vsam(i1+1,i2,i3+1,kd)
     & +vsam(i1-1,i2,i3+1,kd)+vsam(i1+1,i2,i3-1,kd)+vsam(i1-1,i2,i3-1,
     & kd)) )/(dx(0)**2*dx(2)**2)+( 8.*vsam(i1,i2,i3,kd)     -4.*(
     & vsam(i1,i2+1,i3,kd)  +vsam(i1,i2-1,i3,kd)  +vsam(i1,i2  ,i3+1,
     & kd)+vsam(i1,i2  ,i3-1,kd))   +2.*(vsam(i1,i2+1,i3+1,kd)+vsam(
     & i1,i2-1,i3+1,kd)+vsam(i1,i2+1,i3-1,kd)+vsam(i1,i2-1,i3-1,kd)) )
     & /(dx(1)**2*dx(2)**2)
      wsar2(i1,i2,i3,kd)=(wsa(i1+1,i2,i3,kd)-wsa(i1-1,i2,i3,kd))*d12(0)
      wsas2(i1,i2,i3,kd)=(wsa(i1,i2+1,i3,kd)-wsa(i1,i2-1,i3,kd))*d12(1)
      wsat2(i1,i2,i3,kd)=(wsa(i1,i2,i3+1,kd)-wsa(i1,i2,i3-1,kd))*d12(2)
      wsarr2(i1,i2,i3,kd)=(-2.*wsa(i1,i2,i3,kd)+(wsa(i1+1,i2,i3,kd)+
     & wsa(i1-1,i2,i3,kd)) )*d22(0)
      wsass2(i1,i2,i3,kd)=(-2.*wsa(i1,i2,i3,kd)+(wsa(i1,i2+1,i3,kd)+
     & wsa(i1,i2-1,i3,kd)) )*d22(1)
      wsars2(i1,i2,i3,kd)=(wsar2(i1,i2+1,i3,kd)-wsar2(i1,i2-1,i3,kd))*
     & d12(1)
      wsatt2(i1,i2,i3,kd)=(-2.*wsa(i1,i2,i3,kd)+(wsa(i1,i2,i3+1,kd)+
     & wsa(i1,i2,i3-1,kd)) )*d22(2)
      wsart2(i1,i2,i3,kd)=(wsar2(i1,i2,i3+1,kd)-wsar2(i1,i2,i3-1,kd))*
     & d12(2)
      wsast2(i1,i2,i3,kd)=(wsas2(i1,i2,i3+1,kd)-wsas2(i1,i2,i3-1,kd))*
     & d12(2)
      wsarrr2(i1,i2,i3,kd)=(-2.*(wsa(i1+1,i2,i3,kd)-wsa(i1-1,i2,i3,kd))
     & +(wsa(i1+2,i2,i3,kd)-wsa(i1-2,i2,i3,kd)) )*d22(0)*d12(0)
      wsasss2(i1,i2,i3,kd)=(-2.*(wsa(i1,i2+1,i3,kd)-wsa(i1,i2-1,i3,kd))
     & +(wsa(i1,i2+2,i3,kd)-wsa(i1,i2-2,i3,kd)) )*d22(1)*d12(1)
      wsattt2(i1,i2,i3,kd)=(-2.*(wsa(i1,i2,i3+1,kd)-wsa(i1,i2,i3-1,kd))
     & +(wsa(i1,i2,i3+2,kd)-wsa(i1,i2,i3-2,kd)) )*d22(2)*d12(2)
      wsax21(i1,i2,i3,kd)= rx(i1,i2,i3)*wsar2(i1,i2,i3,kd)
      wsay21(i1,i2,i3,kd)=0
      wsaz21(i1,i2,i3,kd)=0
      wsax22(i1,i2,i3,kd)= rx(i1,i2,i3)*wsar2(i1,i2,i3,kd)+sx(i1,i2,i3)
     & *wsas2(i1,i2,i3,kd)
      wsay22(i1,i2,i3,kd)= ry(i1,i2,i3)*wsar2(i1,i2,i3,kd)+sy(i1,i2,i3)
     & *wsas2(i1,i2,i3,kd)
      wsaz22(i1,i2,i3,kd)=0
      wsax23(i1,i2,i3,kd)=rx(i1,i2,i3)*wsar2(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & wsas2(i1,i2,i3,kd)+tx(i1,i2,i3)*wsat2(i1,i2,i3,kd)
      wsay23(i1,i2,i3,kd)=ry(i1,i2,i3)*wsar2(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & wsas2(i1,i2,i3,kd)+ty(i1,i2,i3)*wsat2(i1,i2,i3,kd)
      wsaz23(i1,i2,i3,kd)=rz(i1,i2,i3)*wsar2(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & wsas2(i1,i2,i3,kd)+tz(i1,i2,i3)*wsat2(i1,i2,i3,kd)
      wsaxx21(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wsarr2(i1,i2,i3,kd)+(
     & rxx22(i1,i2,i3))*wsar2(i1,i2,i3,kd)
      wsayy21(i1,i2,i3,kd)=0
      wsaxy21(i1,i2,i3,kd)=0
      wsaxz21(i1,i2,i3,kd)=0
      wsayz21(i1,i2,i3,kd)=0
      wsazz21(i1,i2,i3,kd)=0
      wsalaplacian21(i1,i2,i3,kd)=wsaxx21(i1,i2,i3,kd)
      wsaxx22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wsarr2(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*wsars2(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*wsass2(i1,i2,i3,kd)+(rxx22(i1,i2,i3))*wsar2(i1,i2,i3,kd)+(
     & sxx22(i1,i2,i3))*wsas2(i1,i2,i3,kd)
      wsayy22(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*wsarr2(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*wsars2(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*wsass2(i1,i2,i3,kd)+(ryy22(i1,i2,i3))*wsar2(i1,i2,i3,kd)+(
     & syy22(i1,i2,i3))*wsas2(i1,i2,i3,kd)
      wsaxy22(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wsarr2(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & wsars2(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wsass2(i1,i2,i3,
     & kd)+rxy22(i1,i2,i3)*wsar2(i1,i2,i3,kd)+sxy22(i1,i2,i3)*wsas2(
     & i1,i2,i3,kd)
      wsaxz22(i1,i2,i3,kd)=0
      wsayz22(i1,i2,i3,kd)=0
      wsazz22(i1,i2,i3,kd)=0
      wsalaplacian22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & wsarr2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)
     & *sy(i1,i2,i3))*wsars2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2)*wsass2(i1,i2,i3,kd)+(rxx22(i1,i2,i3)+ryy22(i1,i2,i3))*
     & wsar2(i1,i2,i3,kd)+(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*wsas2(i1,
     & i2,i3,kd)
      wsaxx23(i1,i2,i3,kd)=rx(i1,i2,i3)**2*wsarr2(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*wsass2(i1,i2,i3,kd)+tx(i1,i2,i3)**2*wsatt2(i1,i2,i3,
     & kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*wsars2(i1,i2,i3,kd)+2.*rx(i1,
     & i2,i3)*tx(i1,i2,i3)*wsart2(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,
     & i2,i3)*wsast2(i1,i2,i3,kd)+rxx23(i1,i2,i3)*wsar2(i1,i2,i3,kd)+
     & sxx23(i1,i2,i3)*wsas2(i1,i2,i3,kd)+txx23(i1,i2,i3)*wsat2(i1,i2,
     & i3,kd)
      wsayy23(i1,i2,i3,kd)=ry(i1,i2,i3)**2*wsarr2(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*wsass2(i1,i2,i3,kd)+ty(i1,i2,i3)**2*wsatt2(i1,i2,i3,
     & kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*wsars2(i1,i2,i3,kd)+2.*ry(i1,
     & i2,i3)*ty(i1,i2,i3)*wsart2(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,
     & i2,i3)*wsast2(i1,i2,i3,kd)+ryy23(i1,i2,i3)*wsar2(i1,i2,i3,kd)+
     & syy23(i1,i2,i3)*wsas2(i1,i2,i3,kd)+tyy23(i1,i2,i3)*wsat2(i1,i2,
     & i3,kd)
      wsazz23(i1,i2,i3,kd)=rz(i1,i2,i3)**2*wsarr2(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*wsass2(i1,i2,i3,kd)+tz(i1,i2,i3)**2*wsatt2(i1,i2,i3,
     & kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*wsars2(i1,i2,i3,kd)+2.*rz(i1,
     & i2,i3)*tz(i1,i2,i3)*wsart2(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,
     & i2,i3)*wsast2(i1,i2,i3,kd)+rzz23(i1,i2,i3)*wsar2(i1,i2,i3,kd)+
     & szz23(i1,i2,i3)*wsas2(i1,i2,i3,kd)+tzz23(i1,i2,i3)*wsat2(i1,i2,
     & i3,kd)
      wsaxy23(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wsarr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wsass2(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & ty(i1,i2,i3)*wsatt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(
     & i1,i2,i3)*sx(i1,i2,i3))*wsars2(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(
     & i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*wsart2(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*wsast2(i1,i2,
     & i3,kd)+rxy23(i1,i2,i3)*wsar2(i1,i2,i3,kd)+sxy23(i1,i2,i3)*
     & wsas2(i1,i2,i3,kd)+txy23(i1,i2,i3)*wsat2(i1,i2,i3,kd)
      wsaxz23(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*wsarr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*wsass2(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & tz(i1,i2,i3)*wsatt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sx(i1,i2,i3))*wsars2(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*wsart2(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*wsast2(i1,i2,
     & i3,kd)+rxz23(i1,i2,i3)*wsar2(i1,i2,i3,kd)+sxz23(i1,i2,i3)*
     & wsas2(i1,i2,i3,kd)+txz23(i1,i2,i3)*wsat2(i1,i2,i3,kd)
      wsayz23(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*wsarr2(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*wsass2(i1,i2,i3,kd)+ty(i1,i2,i3)*
     & tz(i1,i2,i3)*wsatt2(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sy(i1,i2,i3))*wsars2(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*wsart2(i1,i2,i3,kd)+(sy(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*wsast2(i1,i2,
     & i3,kd)+ryz23(i1,i2,i3)*wsar2(i1,i2,i3,kd)+syz23(i1,i2,i3)*
     & wsas2(i1,i2,i3,kd)+tyz23(i1,i2,i3)*wsat2(i1,i2,i3,kd)
      wsalaplacian23(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*wsarr2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)
     & **2+sz(i1,i2,i3)**2)*wsass2(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(
     & i1,i2,i3)**2+tz(i1,i2,i3)**2)*wsatt2(i1,i2,i3,kd)+2.*(rx(i1,i2,
     & i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,
     & i2,i3))*wsars2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(
     & i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*wsart2(i1,i2,
     & i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)
     & +sz(i1,i2,i3)*tz(i1,i2,i3))*wsast2(i1,i2,i3,kd)+(rxx23(i1,i2,
     & i3)+ryy23(i1,i2,i3)+rzz23(i1,i2,i3))*wsar2(i1,i2,i3,kd)+(sxx23(
     & i1,i2,i3)+syy23(i1,i2,i3)+szz23(i1,i2,i3))*wsas2(i1,i2,i3,kd)+(
     & txx23(i1,i2,i3)+tyy23(i1,i2,i3)+tzz23(i1,i2,i3))*wsat2(i1,i2,
     & i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      wsax23r(i1,i2,i3,kd)=(wsa(i1+1,i2,i3,kd)-wsa(i1-1,i2,i3,kd))*h12(
     & 0)
      wsay23r(i1,i2,i3,kd)=(wsa(i1,i2+1,i3,kd)-wsa(i1,i2-1,i3,kd))*h12(
     & 1)
      wsaz23r(i1,i2,i3,kd)=(wsa(i1,i2,i3+1,kd)-wsa(i1,i2,i3-1,kd))*h12(
     & 2)
      wsaxx23r(i1,i2,i3,kd)=(-2.*wsa(i1,i2,i3,kd)+(wsa(i1+1,i2,i3,kd)+
     & wsa(i1-1,i2,i3,kd)) )*h22(0)
      wsayy23r(i1,i2,i3,kd)=(-2.*wsa(i1,i2,i3,kd)+(wsa(i1,i2+1,i3,kd)+
     & wsa(i1,i2-1,i3,kd)) )*h22(1)
      wsaxy23r(i1,i2,i3,kd)=(wsax23r(i1,i2+1,i3,kd)-wsax23r(i1,i2-1,i3,
     & kd))*h12(1)
      wsazz23r(i1,i2,i3,kd)=(-2.*wsa(i1,i2,i3,kd)+(wsa(i1,i2,i3+1,kd)+
     & wsa(i1,i2,i3-1,kd)) )*h22(2)
      wsaxz23r(i1,i2,i3,kd)=(wsax23r(i1,i2,i3+1,kd)-wsax23r(i1,i2,i3-1,
     & kd))*h12(2)
      wsayz23r(i1,i2,i3,kd)=(wsay23r(i1,i2,i3+1,kd)-wsay23r(i1,i2,i3-1,
     & kd))*h12(2)
      wsax21r(i1,i2,i3,kd)= wsax23r(i1,i2,i3,kd)
      wsay21r(i1,i2,i3,kd)= wsay23r(i1,i2,i3,kd)
      wsaz21r(i1,i2,i3,kd)= wsaz23r(i1,i2,i3,kd)
      wsaxx21r(i1,i2,i3,kd)= wsaxx23r(i1,i2,i3,kd)
      wsayy21r(i1,i2,i3,kd)= wsayy23r(i1,i2,i3,kd)
      wsazz21r(i1,i2,i3,kd)= wsazz23r(i1,i2,i3,kd)
      wsaxy21r(i1,i2,i3,kd)= wsaxy23r(i1,i2,i3,kd)
      wsaxz21r(i1,i2,i3,kd)= wsaxz23r(i1,i2,i3,kd)
      wsayz21r(i1,i2,i3,kd)= wsayz23r(i1,i2,i3,kd)
      wsalaplacian21r(i1,i2,i3,kd)=wsaxx23r(i1,i2,i3,kd)
      wsax22r(i1,i2,i3,kd)= wsax23r(i1,i2,i3,kd)
      wsay22r(i1,i2,i3,kd)= wsay23r(i1,i2,i3,kd)
      wsaz22r(i1,i2,i3,kd)= wsaz23r(i1,i2,i3,kd)
      wsaxx22r(i1,i2,i3,kd)= wsaxx23r(i1,i2,i3,kd)
      wsayy22r(i1,i2,i3,kd)= wsayy23r(i1,i2,i3,kd)
      wsazz22r(i1,i2,i3,kd)= wsazz23r(i1,i2,i3,kd)
      wsaxy22r(i1,i2,i3,kd)= wsaxy23r(i1,i2,i3,kd)
      wsaxz22r(i1,i2,i3,kd)= wsaxz23r(i1,i2,i3,kd)
      wsayz22r(i1,i2,i3,kd)= wsayz23r(i1,i2,i3,kd)
      wsalaplacian22r(i1,i2,i3,kd)=wsaxx23r(i1,i2,i3,kd)+wsayy23r(i1,
     & i2,i3,kd)
      wsalaplacian23r(i1,i2,i3,kd)=wsaxx23r(i1,i2,i3,kd)+wsayy23r(i1,
     & i2,i3,kd)+wsazz23r(i1,i2,i3,kd)
      wsaxxx22r(i1,i2,i3,kd)=(-2.*(wsa(i1+1,i2,i3,kd)-wsa(i1-1,i2,i3,
     & kd))+(wsa(i1+2,i2,i3,kd)-wsa(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
      wsayyy22r(i1,i2,i3,kd)=(-2.*(wsa(i1,i2+1,i3,kd)-wsa(i1,i2-1,i3,
     & kd))+(wsa(i1,i2+2,i3,kd)-wsa(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
      wsaxxy22r(i1,i2,i3,kd)=( wsaxx22r(i1,i2+1,i3,kd)-wsaxx22r(i1,i2-
     & 1,i3,kd))/(2.*dx(1))
      wsaxyy22r(i1,i2,i3,kd)=( wsayy22r(i1+1,i2,i3,kd)-wsayy22r(i1-1,
     & i2,i3,kd))/(2.*dx(0))
      wsaxxxx22r(i1,i2,i3,kd)=(6.*wsa(i1,i2,i3,kd)-4.*(wsa(i1+1,i2,i3,
     & kd)+wsa(i1-1,i2,i3,kd))+(wsa(i1+2,i2,i3,kd)+wsa(i1-2,i2,i3,kd))
     &  )/(dx(0)**4)
      wsayyyy22r(i1,i2,i3,kd)=(6.*wsa(i1,i2,i3,kd)-4.*(wsa(i1,i2+1,i3,
     & kd)+wsa(i1,i2-1,i3,kd))+(wsa(i1,i2+2,i3,kd)+wsa(i1,i2-2,i3,kd))
     &  )/(dx(1)**4)
      wsaxxyy22r(i1,i2,i3,kd)=( 4.*wsa(i1,i2,i3,kd)     -2.*(wsa(i1+1,
     & i2,i3,kd)+wsa(i1-1,i2,i3,kd)+wsa(i1,i2+1,i3,kd)+wsa(i1,i2-1,i3,
     & kd))   +   (wsa(i1+1,i2+1,i3,kd)+wsa(i1-1,i2+1,i3,kd)+wsa(i1+1,
     & i2-1,i3,kd)+wsa(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      ! 2D laplacian squared = wsa.xxxx + 2 wsa.xxyy + wsa.yyyy
      wsaLapSq22r(i1,i2,i3,kd)= ( 6.*wsa(i1,i2,i3,kd)   - 4.*(wsa(i1+1,
     & i2,i3,kd)+wsa(i1-1,i2,i3,kd))    +(wsa(i1+2,i2,i3,kd)+wsa(i1-2,
     & i2,i3,kd)) )/(dx(0)**4) +( 6.*wsa(i1,i2,i3,kd)    -4.*(wsa(i1,
     & i2+1,i3,kd)+wsa(i1,i2-1,i3,kd))    +(wsa(i1,i2+2,i3,kd)+wsa(i1,
     & i2-2,i3,kd)) )/(dx(1)**4)  +( 8.*wsa(i1,i2,i3,kd)     -4.*(wsa(
     & i1+1,i2,i3,kd)+wsa(i1-1,i2,i3,kd)+wsa(i1,i2+1,i3,kd)+wsa(i1,i2-
     & 1,i3,kd))   +2.*(wsa(i1+1,i2+1,i3,kd)+wsa(i1-1,i2+1,i3,kd)+wsa(
     & i1+1,i2-1,i3,kd)+wsa(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      wsaxxx23r(i1,i2,i3,kd)=(-2.*(wsa(i1+1,i2,i3,kd)-wsa(i1-1,i2,i3,
     & kd))+(wsa(i1+2,i2,i3,kd)-wsa(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
      wsayyy23r(i1,i2,i3,kd)=(-2.*(wsa(i1,i2+1,i3,kd)-wsa(i1,i2-1,i3,
     & kd))+(wsa(i1,i2+2,i3,kd)-wsa(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
      wsazzz23r(i1,i2,i3,kd)=(-2.*(wsa(i1,i2,i3+1,kd)-wsa(i1,i2,i3-1,
     & kd))+(wsa(i1,i2,i3+2,kd)-wsa(i1,i2,i3-2,kd)) )*h22(1)*h12(2)
      wsaxxy23r(i1,i2,i3,kd)=( wsaxx22r(i1,i2+1,i3,kd)-wsaxx22r(i1,i2-
     & 1,i3,kd))/(2.*dx(1))
      wsaxyy23r(i1,i2,i3,kd)=( wsayy22r(i1+1,i2,i3,kd)-wsayy22r(i1-1,
     & i2,i3,kd))/(2.*dx(0))
      wsaxxz23r(i1,i2,i3,kd)=( wsaxx22r(i1,i2,i3+1,kd)-wsaxx22r(i1,i2,
     & i3-1,kd))/(2.*dx(2))
      wsayyz23r(i1,i2,i3,kd)=( wsayy22r(i1,i2,i3+1,kd)-wsayy22r(i1,i2,
     & i3-1,kd))/(2.*dx(2))
      wsaxzz23r(i1,i2,i3,kd)=( wsazz22r(i1+1,i2,i3,kd)-wsazz22r(i1-1,
     & i2,i3,kd))/(2.*dx(0))
      wsayzz23r(i1,i2,i3,kd)=( wsazz22r(i1,i2+1,i3,kd)-wsazz22r(i1,i2-
     & 1,i3,kd))/(2.*dx(1))
      wsaxxxx23r(i1,i2,i3,kd)=(6.*wsa(i1,i2,i3,kd)-4.*(wsa(i1+1,i2,i3,
     & kd)+wsa(i1-1,i2,i3,kd))+(wsa(i1+2,i2,i3,kd)+wsa(i1-2,i2,i3,kd))
     &  )/(dx(0)**4)
      wsayyyy23r(i1,i2,i3,kd)=(6.*wsa(i1,i2,i3,kd)-4.*(wsa(i1,i2+1,i3,
     & kd)+wsa(i1,i2-1,i3,kd))+(wsa(i1,i2+2,i3,kd)+wsa(i1,i2-2,i3,kd))
     &  )/(dx(1)**4)
      wsazzzz23r(i1,i2,i3,kd)=(6.*wsa(i1,i2,i3,kd)-4.*(wsa(i1,i2,i3+1,
     & kd)+wsa(i1,i2,i3-1,kd))+(wsa(i1,i2,i3+2,kd)+wsa(i1,i2,i3-2,kd))
     &  )/(dx(2)**4)
      wsaxxyy23r(i1,i2,i3,kd)=( 4.*wsa(i1,i2,i3,kd)     -2.*(wsa(i1+1,
     & i2,i3,kd)+wsa(i1-1,i2,i3,kd)+wsa(i1,i2+1,i3,kd)+wsa(i1,i2-1,i3,
     & kd))   +   (wsa(i1+1,i2+1,i3,kd)+wsa(i1-1,i2+1,i3,kd)+wsa(i1+1,
     & i2-1,i3,kd)+wsa(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      wsaxxzz23r(i1,i2,i3,kd)=( 4.*wsa(i1,i2,i3,kd)     -2.*(wsa(i1+1,
     & i2,i3,kd)+wsa(i1-1,i2,i3,kd)+wsa(i1,i2,i3+1,kd)+wsa(i1,i2,i3-1,
     & kd))   +   (wsa(i1+1,i2,i3+1,kd)+wsa(i1-1,i2,i3+1,kd)+wsa(i1+1,
     & i2,i3-1,kd)+wsa(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)
      wsayyzz23r(i1,i2,i3,kd)=( 4.*wsa(i1,i2,i3,kd)     -2.*(wsa(i1,i2+
     & 1,i3,kd)  +wsa(i1,i2-1,i3,kd)+  wsa(i1,i2  ,i3+1,kd)+wsa(i1,i2 
     &  ,i3-1,kd))   +   (wsa(i1,i2+1,i3+1,kd)+wsa(i1,i2-1,i3+1,kd)+
     & wsa(i1,i2+1,i3-1,kd)+wsa(i1,i2-1,i3-1,kd)) )/(dx(1)**2*dx(2)**
     & 2)
      ! 3D laplacian squared = wsa.xxxx + wsa.yyyy + wsa.zzzz + 2 (wsa.xxyy + wsa.xxzz + wsa.yyzz )
      wsaLapSq23r(i1,i2,i3,kd)= ( 6.*wsa(i1,i2,i3,kd)   - 4.*(wsa(i1+1,
     & i2,i3,kd)+wsa(i1-1,i2,i3,kd))    +(wsa(i1+2,i2,i3,kd)+wsa(i1-2,
     & i2,i3,kd)) )/(dx(0)**4) +( 6.*wsa(i1,i2,i3,kd)    -4.*(wsa(i1,
     & i2+1,i3,kd)+wsa(i1,i2-1,i3,kd))    +(wsa(i1,i2+2,i3,kd)+wsa(i1,
     & i2-2,i3,kd)) )/(dx(1)**4)  +( 6.*wsa(i1,i2,i3,kd)    -4.*(wsa(
     & i1,i2,i3+1,kd)+wsa(i1,i2,i3-1,kd))    +(wsa(i1,i2,i3+2,kd)+wsa(
     & i1,i2,i3-2,kd)) )/(dx(2)**4)  +( 8.*wsa(i1,i2,i3,kd)     -4.*(
     & wsa(i1+1,i2,i3,kd)  +wsa(i1-1,i2,i3,kd)  +wsa(i1  ,i2+1,i3,kd)+
     & wsa(i1  ,i2-1,i3,kd))   +2.*(wsa(i1+1,i2+1,i3,kd)+wsa(i1-1,i2+
     & 1,i3,kd)+wsa(i1+1,i2-1,i3,kd)+wsa(i1-1,i2-1,i3,kd)) )/(dx(0)**
     & 2*dx(1)**2)+( 8.*wsa(i1,i2,i3,kd)     -4.*(wsa(i1+1,i2,i3,kd)  
     & +wsa(i1-1,i2,i3,kd)  +wsa(i1  ,i2,i3+1,kd)+wsa(i1  ,i2,i3-1,kd)
     & )   +2.*(wsa(i1+1,i2,i3+1,kd)+wsa(i1-1,i2,i3+1,kd)+wsa(i1+1,i2,
     & i3-1,kd)+wsa(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)+( 8.*wsa(
     & i1,i2,i3,kd)     -4.*(wsa(i1,i2+1,i3,kd)  +wsa(i1,i2-1,i3,kd)  
     & +wsa(i1,i2  ,i3+1,kd)+wsa(i1,i2  ,i3-1,kd))   +2.*(wsa(i1,i2+1,
     & i3+1,kd)+wsa(i1,i2-1,i3+1,kd)+wsa(i1,i2+1,i3-1,kd)+wsa(i1,i2-1,
     & i3-1,kd)) )/(dx(1)**2*dx(2)**2)
      wsamr2(i1,i2,i3,kd)=(wsam(i1+1,i2,i3,kd)-wsam(i1-1,i2,i3,kd))*
     & d12(0)
      wsams2(i1,i2,i3,kd)=(wsam(i1,i2+1,i3,kd)-wsam(i1,i2-1,i3,kd))*
     & d12(1)
      wsamt2(i1,i2,i3,kd)=(wsam(i1,i2,i3+1,kd)-wsam(i1,i2,i3-1,kd))*
     & d12(2)
      wsamrr2(i1,i2,i3,kd)=(-2.*wsam(i1,i2,i3,kd)+(wsam(i1+1,i2,i3,kd)+
     & wsam(i1-1,i2,i3,kd)) )*d22(0)
      wsamss2(i1,i2,i3,kd)=(-2.*wsam(i1,i2,i3,kd)+(wsam(i1,i2+1,i3,kd)+
     & wsam(i1,i2-1,i3,kd)) )*d22(1)
      wsamrs2(i1,i2,i3,kd)=(wsamr2(i1,i2+1,i3,kd)-wsamr2(i1,i2-1,i3,kd)
     & )*d12(1)
      wsamtt2(i1,i2,i3,kd)=(-2.*wsam(i1,i2,i3,kd)+(wsam(i1,i2,i3+1,kd)+
     & wsam(i1,i2,i3-1,kd)) )*d22(2)
      wsamrt2(i1,i2,i3,kd)=(wsamr2(i1,i2,i3+1,kd)-wsamr2(i1,i2,i3-1,kd)
     & )*d12(2)
      wsamst2(i1,i2,i3,kd)=(wsams2(i1,i2,i3+1,kd)-wsams2(i1,i2,i3-1,kd)
     & )*d12(2)
      wsamrrr2(i1,i2,i3,kd)=(-2.*(wsam(i1+1,i2,i3,kd)-wsam(i1-1,i2,i3,
     & kd))+(wsam(i1+2,i2,i3,kd)-wsam(i1-2,i2,i3,kd)) )*d22(0)*d12(0)
      wsamsss2(i1,i2,i3,kd)=(-2.*(wsam(i1,i2+1,i3,kd)-wsam(i1,i2-1,i3,
     & kd))+(wsam(i1,i2+2,i3,kd)-wsam(i1,i2-2,i3,kd)) )*d22(1)*d12(1)
      wsamttt2(i1,i2,i3,kd)=(-2.*(wsam(i1,i2,i3+1,kd)-wsam(i1,i2,i3-1,
     & kd))+(wsam(i1,i2,i3+2,kd)-wsam(i1,i2,i3-2,kd)) )*d22(2)*d12(2)
      wsamx21(i1,i2,i3,kd)= rx(i1,i2,i3)*wsamr2(i1,i2,i3,kd)
      wsamy21(i1,i2,i3,kd)=0
      wsamz21(i1,i2,i3,kd)=0
      wsamx22(i1,i2,i3,kd)= rx(i1,i2,i3)*wsamr2(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*wsams2(i1,i2,i3,kd)
      wsamy22(i1,i2,i3,kd)= ry(i1,i2,i3)*wsamr2(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*wsams2(i1,i2,i3,kd)
      wsamz22(i1,i2,i3,kd)=0
      wsamx23(i1,i2,i3,kd)=rx(i1,i2,i3)*wsamr2(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*wsams2(i1,i2,i3,kd)+tx(i1,i2,i3)*wsamt2(i1,i2,i3,kd)
      wsamy23(i1,i2,i3,kd)=ry(i1,i2,i3)*wsamr2(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*wsams2(i1,i2,i3,kd)+ty(i1,i2,i3)*wsamt2(i1,i2,i3,kd)
      wsamz23(i1,i2,i3,kd)=rz(i1,i2,i3)*wsamr2(i1,i2,i3,kd)+sz(i1,i2,
     & i3)*wsams2(i1,i2,i3,kd)+tz(i1,i2,i3)*wsamt2(i1,i2,i3,kd)
      wsamxx21(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wsamrr2(i1,i2,i3,kd)+(
     & rxx22(i1,i2,i3))*wsamr2(i1,i2,i3,kd)
      wsamyy21(i1,i2,i3,kd)=0
      wsamxy21(i1,i2,i3,kd)=0
      wsamxz21(i1,i2,i3,kd)=0
      wsamyz21(i1,i2,i3,kd)=0
      wsamzz21(i1,i2,i3,kd)=0
      wsamlaplacian21(i1,i2,i3,kd)=wsamxx21(i1,i2,i3,kd)
      wsamxx22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wsamrr2(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*wsamrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*wsamss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3))*wsamr2(i1,i2,i3,kd)+(
     & sxx22(i1,i2,i3))*wsams2(i1,i2,i3,kd)
      wsamyy22(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*wsamrr2(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*wsamrs2(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*wsamss2(i1,i2,i3,kd)+(ryy22(i1,i2,i3))*wsamr2(i1,i2,i3,kd)+(
     & syy22(i1,i2,i3))*wsams2(i1,i2,i3,kd)
      wsamxy22(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wsamrr2(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & wsamrs2(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wsamss2(i1,i2,
     & i3,kd)+rxy22(i1,i2,i3)*wsamr2(i1,i2,i3,kd)+sxy22(i1,i2,i3)*
     & wsams2(i1,i2,i3,kd)
      wsamxz22(i1,i2,i3,kd)=0
      wsamyz22(i1,i2,i3,kd)=0
      wsamzz22(i1,i2,i3,kd)=0
      wsamlaplacian22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & wsamrr2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,
     & i3)*sy(i1,i2,i3))*wsamrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,
     & i2,i3)**2)*wsamss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3)+ryy22(i1,i2,
     & i3))*wsamr2(i1,i2,i3,kd)+(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*
     & wsams2(i1,i2,i3,kd)
      wsamxx23(i1,i2,i3,kd)=rx(i1,i2,i3)**2*wsamrr2(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*wsamss2(i1,i2,i3,kd)+tx(i1,i2,i3)**2*wsamtt2(i1,i2,
     & i3,kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*wsamrs2(i1,i2,i3,kd)+2.*rx(
     & i1,i2,i3)*tx(i1,i2,i3)*wsamrt2(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(
     & i1,i2,i3)*wsamst2(i1,i2,i3,kd)+rxx23(i1,i2,i3)*wsamr2(i1,i2,i3,
     & kd)+sxx23(i1,i2,i3)*wsams2(i1,i2,i3,kd)+txx23(i1,i2,i3)*wsamt2(
     & i1,i2,i3,kd)
      wsamyy23(i1,i2,i3,kd)=ry(i1,i2,i3)**2*wsamrr2(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*wsamss2(i1,i2,i3,kd)+ty(i1,i2,i3)**2*wsamtt2(i1,i2,
     & i3,kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*wsamrs2(i1,i2,i3,kd)+2.*ry(
     & i1,i2,i3)*ty(i1,i2,i3)*wsamrt2(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(
     & i1,i2,i3)*wsamst2(i1,i2,i3,kd)+ryy23(i1,i2,i3)*wsamr2(i1,i2,i3,
     & kd)+syy23(i1,i2,i3)*wsams2(i1,i2,i3,kd)+tyy23(i1,i2,i3)*wsamt2(
     & i1,i2,i3,kd)
      wsamzz23(i1,i2,i3,kd)=rz(i1,i2,i3)**2*wsamrr2(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*wsamss2(i1,i2,i3,kd)+tz(i1,i2,i3)**2*wsamtt2(i1,i2,
     & i3,kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*wsamrs2(i1,i2,i3,kd)+2.*rz(
     & i1,i2,i3)*tz(i1,i2,i3)*wsamrt2(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(
     & i1,i2,i3)*wsamst2(i1,i2,i3,kd)+rzz23(i1,i2,i3)*wsamr2(i1,i2,i3,
     & kd)+szz23(i1,i2,i3)*wsams2(i1,i2,i3,kd)+tzz23(i1,i2,i3)*wsamt2(
     & i1,i2,i3,kd)
      wsamxy23(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wsamrr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wsamss2(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *ty(i1,i2,i3)*wsamtt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+
     & ry(i1,i2,i3)*sx(i1,i2,i3))*wsamrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & ty(i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*wsamrt2(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*wsamst2(
     & i1,i2,i3,kd)+rxy23(i1,i2,i3)*wsamr2(i1,i2,i3,kd)+sxy23(i1,i2,
     & i3)*wsams2(i1,i2,i3,kd)+txy23(i1,i2,i3)*wsamt2(i1,i2,i3,kd)
      wsamxz23(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*wsamrr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*wsamss2(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *tz(i1,i2,i3)*wsamtt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sx(i1,i2,i3))*wsamrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*wsamrt2(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*wsamst2(
     & i1,i2,i3,kd)+rxz23(i1,i2,i3)*wsamr2(i1,i2,i3,kd)+sxz23(i1,i2,
     & i3)*wsams2(i1,i2,i3,kd)+txz23(i1,i2,i3)*wsamt2(i1,i2,i3,kd)
      wsamyz23(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*wsamrr2(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*wsamss2(i1,i2,i3,kd)+ty(i1,i2,i3)
     & *tz(i1,i2,i3)*wsamtt2(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sy(i1,i2,i3))*wsamrs2(i1,i2,i3,kd)+(ry(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*wsamrt2(i1,i2,i3,kd)+(
     & sy(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*wsamst2(
     & i1,i2,i3,kd)+ryz23(i1,i2,i3)*wsamr2(i1,i2,i3,kd)+syz23(i1,i2,
     & i3)*wsams2(i1,i2,i3,kd)+tyz23(i1,i2,i3)*wsamt2(i1,i2,i3,kd)
      wsamlaplacian23(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*wsamrr2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2+sz(i1,i2,i3)**2)*wsamss2(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+
     & ty(i1,i2,i3)**2+tz(i1,i2,i3)**2)*wsamtt2(i1,i2,i3,kd)+2.*(rx(
     & i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*
     & sz(i1,i2,i3))*wsamrs2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,
     & i3)+ ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*
     & wsamrt2(i1,i2,i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,
     & i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3))*wsamst2(i1,i2,i3,
     & kd)+(rxx23(i1,i2,i3)+ryy23(i1,i2,i3)+rzz23(i1,i2,i3))*wsamr2(
     & i1,i2,i3,kd)+(sxx23(i1,i2,i3)+syy23(i1,i2,i3)+szz23(i1,i2,i3))*
     & wsams2(i1,i2,i3,kd)+(txx23(i1,i2,i3)+tyy23(i1,i2,i3)+tzz23(i1,
     & i2,i3))*wsamt2(i1,i2,i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      wsamx23r(i1,i2,i3,kd)=(wsam(i1+1,i2,i3,kd)-wsam(i1-1,i2,i3,kd))*
     & h12(0)
      wsamy23r(i1,i2,i3,kd)=(wsam(i1,i2+1,i3,kd)-wsam(i1,i2-1,i3,kd))*
     & h12(1)
      wsamz23r(i1,i2,i3,kd)=(wsam(i1,i2,i3+1,kd)-wsam(i1,i2,i3-1,kd))*
     & h12(2)
      wsamxx23r(i1,i2,i3,kd)=(-2.*wsam(i1,i2,i3,kd)+(wsam(i1+1,i2,i3,
     & kd)+wsam(i1-1,i2,i3,kd)) )*h22(0)
      wsamyy23r(i1,i2,i3,kd)=(-2.*wsam(i1,i2,i3,kd)+(wsam(i1,i2+1,i3,
     & kd)+wsam(i1,i2-1,i3,kd)) )*h22(1)
      wsamxy23r(i1,i2,i3,kd)=(wsamx23r(i1,i2+1,i3,kd)-wsamx23r(i1,i2-1,
     & i3,kd))*h12(1)
      wsamzz23r(i1,i2,i3,kd)=(-2.*wsam(i1,i2,i3,kd)+(wsam(i1,i2,i3+1,
     & kd)+wsam(i1,i2,i3-1,kd)) )*h22(2)
      wsamxz23r(i1,i2,i3,kd)=(wsamx23r(i1,i2,i3+1,kd)-wsamx23r(i1,i2,
     & i3-1,kd))*h12(2)
      wsamyz23r(i1,i2,i3,kd)=(wsamy23r(i1,i2,i3+1,kd)-wsamy23r(i1,i2,
     & i3-1,kd))*h12(2)
      wsamx21r(i1,i2,i3,kd)= wsamx23r(i1,i2,i3,kd)
      wsamy21r(i1,i2,i3,kd)= wsamy23r(i1,i2,i3,kd)
      wsamz21r(i1,i2,i3,kd)= wsamz23r(i1,i2,i3,kd)
      wsamxx21r(i1,i2,i3,kd)= wsamxx23r(i1,i2,i3,kd)
      wsamyy21r(i1,i2,i3,kd)= wsamyy23r(i1,i2,i3,kd)
      wsamzz21r(i1,i2,i3,kd)= wsamzz23r(i1,i2,i3,kd)
      wsamxy21r(i1,i2,i3,kd)= wsamxy23r(i1,i2,i3,kd)
      wsamxz21r(i1,i2,i3,kd)= wsamxz23r(i1,i2,i3,kd)
      wsamyz21r(i1,i2,i3,kd)= wsamyz23r(i1,i2,i3,kd)
      wsamlaplacian21r(i1,i2,i3,kd)=wsamxx23r(i1,i2,i3,kd)
      wsamx22r(i1,i2,i3,kd)= wsamx23r(i1,i2,i3,kd)
      wsamy22r(i1,i2,i3,kd)= wsamy23r(i1,i2,i3,kd)
      wsamz22r(i1,i2,i3,kd)= wsamz23r(i1,i2,i3,kd)
      wsamxx22r(i1,i2,i3,kd)= wsamxx23r(i1,i2,i3,kd)
      wsamyy22r(i1,i2,i3,kd)= wsamyy23r(i1,i2,i3,kd)
      wsamzz22r(i1,i2,i3,kd)= wsamzz23r(i1,i2,i3,kd)
      wsamxy22r(i1,i2,i3,kd)= wsamxy23r(i1,i2,i3,kd)
      wsamxz22r(i1,i2,i3,kd)= wsamxz23r(i1,i2,i3,kd)
      wsamyz22r(i1,i2,i3,kd)= wsamyz23r(i1,i2,i3,kd)
      wsamlaplacian22r(i1,i2,i3,kd)=wsamxx23r(i1,i2,i3,kd)+wsamyy23r(
     & i1,i2,i3,kd)
      wsamlaplacian23r(i1,i2,i3,kd)=wsamxx23r(i1,i2,i3,kd)+wsamyy23r(
     & i1,i2,i3,kd)+wsamzz23r(i1,i2,i3,kd)
      wsamxxx22r(i1,i2,i3,kd)=(-2.*(wsam(i1+1,i2,i3,kd)-wsam(i1-1,i2,
     & i3,kd))+(wsam(i1+2,i2,i3,kd)-wsam(i1-2,i2,i3,kd)) )*h22(0)*h12(
     & 0)
      wsamyyy22r(i1,i2,i3,kd)=(-2.*(wsam(i1,i2+1,i3,kd)-wsam(i1,i2-1,
     & i3,kd))+(wsam(i1,i2+2,i3,kd)-wsam(i1,i2-2,i3,kd)) )*h22(1)*h12(
     & 1)
      wsamxxy22r(i1,i2,i3,kd)=( wsamxx22r(i1,i2+1,i3,kd)-wsamxx22r(i1,
     & i2-1,i3,kd))/(2.*dx(1))
      wsamxyy22r(i1,i2,i3,kd)=( wsamyy22r(i1+1,i2,i3,kd)-wsamyy22r(i1-
     & 1,i2,i3,kd))/(2.*dx(0))
      wsamxxxx22r(i1,i2,i3,kd)=(6.*wsam(i1,i2,i3,kd)-4.*(wsam(i1+1,i2,
     & i3,kd)+wsam(i1-1,i2,i3,kd))+(wsam(i1+2,i2,i3,kd)+wsam(i1-2,i2,
     & i3,kd)) )/(dx(0)**4)
      wsamyyyy22r(i1,i2,i3,kd)=(6.*wsam(i1,i2,i3,kd)-4.*(wsam(i1,i2+1,
     & i3,kd)+wsam(i1,i2-1,i3,kd))+(wsam(i1,i2+2,i3,kd)+wsam(i1,i2-2,
     & i3,kd)) )/(dx(1)**4)
      wsamxxyy22r(i1,i2,i3,kd)=( 4.*wsam(i1,i2,i3,kd)     -2.*(wsam(i1+
     & 1,i2,i3,kd)+wsam(i1-1,i2,i3,kd)+wsam(i1,i2+1,i3,kd)+wsam(i1,i2-
     & 1,i3,kd))   +   (wsam(i1+1,i2+1,i3,kd)+wsam(i1-1,i2+1,i3,kd)+
     & wsam(i1+1,i2-1,i3,kd)+wsam(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)*
     & *2)
      ! 2D laplacian squared = wsam.xxxx + 2 wsam.xxyy + wsam.yyyy
      wsamLapSq22r(i1,i2,i3,kd)= ( 6.*wsam(i1,i2,i3,kd)   - 4.*(wsam(
     & i1+1,i2,i3,kd)+wsam(i1-1,i2,i3,kd))    +(wsam(i1+2,i2,i3,kd)+
     & wsam(i1-2,i2,i3,kd)) )/(dx(0)**4) +( 6.*wsam(i1,i2,i3,kd)    -
     & 4.*(wsam(i1,i2+1,i3,kd)+wsam(i1,i2-1,i3,kd))    +(wsam(i1,i2+2,
     & i3,kd)+wsam(i1,i2-2,i3,kd)) )/(dx(1)**4)  +( 8.*wsam(i1,i2,i3,
     & kd)     -4.*(wsam(i1+1,i2,i3,kd)+wsam(i1-1,i2,i3,kd)+wsam(i1,
     & i2+1,i3,kd)+wsam(i1,i2-1,i3,kd))   +2.*(wsam(i1+1,i2+1,i3,kd)+
     & wsam(i1-1,i2+1,i3,kd)+wsam(i1+1,i2-1,i3,kd)+wsam(i1-1,i2-1,i3,
     & kd)) )/(dx(0)**2*dx(1)**2)
      wsamxxx23r(i1,i2,i3,kd)=(-2.*(wsam(i1+1,i2,i3,kd)-wsam(i1-1,i2,
     & i3,kd))+(wsam(i1+2,i2,i3,kd)-wsam(i1-2,i2,i3,kd)) )*h22(0)*h12(
     & 0)
      wsamyyy23r(i1,i2,i3,kd)=(-2.*(wsam(i1,i2+1,i3,kd)-wsam(i1,i2-1,
     & i3,kd))+(wsam(i1,i2+2,i3,kd)-wsam(i1,i2-2,i3,kd)) )*h22(1)*h12(
     & 1)
      wsamzzz23r(i1,i2,i3,kd)=(-2.*(wsam(i1,i2,i3+1,kd)-wsam(i1,i2,i3-
     & 1,kd))+(wsam(i1,i2,i3+2,kd)-wsam(i1,i2,i3-2,kd)) )*h22(1)*h12(
     & 2)
      wsamxxy23r(i1,i2,i3,kd)=( wsamxx22r(i1,i2+1,i3,kd)-wsamxx22r(i1,
     & i2-1,i3,kd))/(2.*dx(1))
      wsamxyy23r(i1,i2,i3,kd)=( wsamyy22r(i1+1,i2,i3,kd)-wsamyy22r(i1-
     & 1,i2,i3,kd))/(2.*dx(0))
      wsamxxz23r(i1,i2,i3,kd)=( wsamxx22r(i1,i2,i3+1,kd)-wsamxx22r(i1,
     & i2,i3-1,kd))/(2.*dx(2))
      wsamyyz23r(i1,i2,i3,kd)=( wsamyy22r(i1,i2,i3+1,kd)-wsamyy22r(i1,
     & i2,i3-1,kd))/(2.*dx(2))
      wsamxzz23r(i1,i2,i3,kd)=( wsamzz22r(i1+1,i2,i3,kd)-wsamzz22r(i1-
     & 1,i2,i3,kd))/(2.*dx(0))
      wsamyzz23r(i1,i2,i3,kd)=( wsamzz22r(i1,i2+1,i3,kd)-wsamzz22r(i1,
     & i2-1,i3,kd))/(2.*dx(1))
      wsamxxxx23r(i1,i2,i3,kd)=(6.*wsam(i1,i2,i3,kd)-4.*(wsam(i1+1,i2,
     & i3,kd)+wsam(i1-1,i2,i3,kd))+(wsam(i1+2,i2,i3,kd)+wsam(i1-2,i2,
     & i3,kd)) )/(dx(0)**4)
      wsamyyyy23r(i1,i2,i3,kd)=(6.*wsam(i1,i2,i3,kd)-4.*(wsam(i1,i2+1,
     & i3,kd)+wsam(i1,i2-1,i3,kd))+(wsam(i1,i2+2,i3,kd)+wsam(i1,i2-2,
     & i3,kd)) )/(dx(1)**4)
      wsamzzzz23r(i1,i2,i3,kd)=(6.*wsam(i1,i2,i3,kd)-4.*(wsam(i1,i2,i3+
     & 1,kd)+wsam(i1,i2,i3-1,kd))+(wsam(i1,i2,i3+2,kd)+wsam(i1,i2,i3-
     & 2,kd)) )/(dx(2)**4)
      wsamxxyy23r(i1,i2,i3,kd)=( 4.*wsam(i1,i2,i3,kd)     -2.*(wsam(i1+
     & 1,i2,i3,kd)+wsam(i1-1,i2,i3,kd)+wsam(i1,i2+1,i3,kd)+wsam(i1,i2-
     & 1,i3,kd))   +   (wsam(i1+1,i2+1,i3,kd)+wsam(i1-1,i2+1,i3,kd)+
     & wsam(i1+1,i2-1,i3,kd)+wsam(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)*
     & *2)
      wsamxxzz23r(i1,i2,i3,kd)=( 4.*wsam(i1,i2,i3,kd)     -2.*(wsam(i1+
     & 1,i2,i3,kd)+wsam(i1-1,i2,i3,kd)+wsam(i1,i2,i3+1,kd)+wsam(i1,i2,
     & i3-1,kd))   +   (wsam(i1+1,i2,i3+1,kd)+wsam(i1-1,i2,i3+1,kd)+
     & wsam(i1+1,i2,i3-1,kd)+wsam(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)*
     & *2)
      wsamyyzz23r(i1,i2,i3,kd)=( 4.*wsam(i1,i2,i3,kd)     -2.*(wsam(i1,
     & i2+1,i3,kd)  +wsam(i1,i2-1,i3,kd)+  wsam(i1,i2  ,i3+1,kd)+wsam(
     & i1,i2  ,i3-1,kd))   +   (wsam(i1,i2+1,i3+1,kd)+wsam(i1,i2-1,i3+
     & 1,kd)+wsam(i1,i2+1,i3-1,kd)+wsam(i1,i2-1,i3-1,kd)) )/(dx(1)**2*
     & dx(2)**2)
      ! 3D laplacian squared = wsam.xxxx + wsam.yyyy + wsam.zzzz + 2 (wsam.xxyy + wsam.xxzz + wsam.yyzz )
      wsamLapSq23r(i1,i2,i3,kd)= ( 6.*wsam(i1,i2,i3,kd)   - 4.*(wsam(
     & i1+1,i2,i3,kd)+wsam(i1-1,i2,i3,kd))    +(wsam(i1+2,i2,i3,kd)+
     & wsam(i1-2,i2,i3,kd)) )/(dx(0)**4) +( 6.*wsam(i1,i2,i3,kd)    -
     & 4.*(wsam(i1,i2+1,i3,kd)+wsam(i1,i2-1,i3,kd))    +(wsam(i1,i2+2,
     & i3,kd)+wsam(i1,i2-2,i3,kd)) )/(dx(1)**4)  +( 6.*wsam(i1,i2,i3,
     & kd)    -4.*(wsam(i1,i2,i3+1,kd)+wsam(i1,i2,i3-1,kd))    +(wsam(
     & i1,i2,i3+2,kd)+wsam(i1,i2,i3-2,kd)) )/(dx(2)**4)  +( 8.*wsam(
     & i1,i2,i3,kd)     -4.*(wsam(i1+1,i2,i3,kd)  +wsam(i1-1,i2,i3,kd)
     &   +wsam(i1  ,i2+1,i3,kd)+wsam(i1  ,i2-1,i3,kd))   +2.*(wsam(i1+
     & 1,i2+1,i3,kd)+wsam(i1-1,i2+1,i3,kd)+wsam(i1+1,i2-1,i3,kd)+wsam(
     & i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)+( 8.*wsam(i1,i2,i3,kd) 
     &     -4.*(wsam(i1+1,i2,i3,kd)  +wsam(i1-1,i2,i3,kd)  +wsam(i1  ,
     & i2,i3+1,kd)+wsam(i1  ,i2,i3-1,kd))   +2.*(wsam(i1+1,i2,i3+1,kd)
     & +wsam(i1-1,i2,i3+1,kd)+wsam(i1+1,i2,i3-1,kd)+wsam(i1-1,i2,i3-1,
     & kd)) )/(dx(0)**2*dx(2)**2)+( 8.*wsam(i1,i2,i3,kd)     -4.*(
     & wsam(i1,i2+1,i3,kd)  +wsam(i1,i2-1,i3,kd)  +wsam(i1,i2  ,i3+1,
     & kd)+wsam(i1,i2  ,i3-1,kd))   +2.*(wsam(i1,i2+1,i3+1,kd)+wsam(
     & i1,i2-1,i3+1,kd)+wsam(i1,i2+1,i3-1,kd)+wsam(i1,i2-1,i3-1,kd)) )
     & /(dx(1)**2*dx(2)**2)

      vsbr2(i1,i2,i3,kd)=(vsb(i1+1,i2,i3,kd)-vsb(i1-1,i2,i3,kd))*d12(0)
      vsbs2(i1,i2,i3,kd)=(vsb(i1,i2+1,i3,kd)-vsb(i1,i2-1,i3,kd))*d12(1)
      vsbt2(i1,i2,i3,kd)=(vsb(i1,i2,i3+1,kd)-vsb(i1,i2,i3-1,kd))*d12(2)
      vsbrr2(i1,i2,i3,kd)=(-2.*vsb(i1,i2,i3,kd)+(vsb(i1+1,i2,i3,kd)+
     & vsb(i1-1,i2,i3,kd)) )*d22(0)
      vsbss2(i1,i2,i3,kd)=(-2.*vsb(i1,i2,i3,kd)+(vsb(i1,i2+1,i3,kd)+
     & vsb(i1,i2-1,i3,kd)) )*d22(1)
      vsbrs2(i1,i2,i3,kd)=(vsbr2(i1,i2+1,i3,kd)-vsbr2(i1,i2-1,i3,kd))*
     & d12(1)
      vsbtt2(i1,i2,i3,kd)=(-2.*vsb(i1,i2,i3,kd)+(vsb(i1,i2,i3+1,kd)+
     & vsb(i1,i2,i3-1,kd)) )*d22(2)
      vsbrt2(i1,i2,i3,kd)=(vsbr2(i1,i2,i3+1,kd)-vsbr2(i1,i2,i3-1,kd))*
     & d12(2)
      vsbst2(i1,i2,i3,kd)=(vsbs2(i1,i2,i3+1,kd)-vsbs2(i1,i2,i3-1,kd))*
     & d12(2)
      vsbrrr2(i1,i2,i3,kd)=(-2.*(vsb(i1+1,i2,i3,kd)-vsb(i1-1,i2,i3,kd))
     & +(vsb(i1+2,i2,i3,kd)-vsb(i1-2,i2,i3,kd)) )*d22(0)*d12(0)
      vsbsss2(i1,i2,i3,kd)=(-2.*(vsb(i1,i2+1,i3,kd)-vsb(i1,i2-1,i3,kd))
     & +(vsb(i1,i2+2,i3,kd)-vsb(i1,i2-2,i3,kd)) )*d22(1)*d12(1)
      vsbttt2(i1,i2,i3,kd)=(-2.*(vsb(i1,i2,i3+1,kd)-vsb(i1,i2,i3-1,kd))
     & +(vsb(i1,i2,i3+2,kd)-vsb(i1,i2,i3-2,kd)) )*d22(2)*d12(2)
      vsbx21(i1,i2,i3,kd)= rx(i1,i2,i3)*vsbr2(i1,i2,i3,kd)
      vsby21(i1,i2,i3,kd)=0
      vsbz21(i1,i2,i3,kd)=0
      vsbx22(i1,i2,i3,kd)= rx(i1,i2,i3)*vsbr2(i1,i2,i3,kd)+sx(i1,i2,i3)
     & *vsbs2(i1,i2,i3,kd)
      vsby22(i1,i2,i3,kd)= ry(i1,i2,i3)*vsbr2(i1,i2,i3,kd)+sy(i1,i2,i3)
     & *vsbs2(i1,i2,i3,kd)
      vsbz22(i1,i2,i3,kd)=0
      vsbx23(i1,i2,i3,kd)=rx(i1,i2,i3)*vsbr2(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & vsbs2(i1,i2,i3,kd)+tx(i1,i2,i3)*vsbt2(i1,i2,i3,kd)
      vsby23(i1,i2,i3,kd)=ry(i1,i2,i3)*vsbr2(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & vsbs2(i1,i2,i3,kd)+ty(i1,i2,i3)*vsbt2(i1,i2,i3,kd)
      vsbz23(i1,i2,i3,kd)=rz(i1,i2,i3)*vsbr2(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & vsbs2(i1,i2,i3,kd)+tz(i1,i2,i3)*vsbt2(i1,i2,i3,kd)
      vsbxx21(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vsbrr2(i1,i2,i3,kd)+(
     & rxx22(i1,i2,i3))*vsbr2(i1,i2,i3,kd)
      vsbyy21(i1,i2,i3,kd)=0
      vsbxy21(i1,i2,i3,kd)=0
      vsbxz21(i1,i2,i3,kd)=0
      vsbyz21(i1,i2,i3,kd)=0
      vsbzz21(i1,i2,i3,kd)=0
      vsblaplacian21(i1,i2,i3,kd)=vsbxx21(i1,i2,i3,kd)
      vsbxx22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vsbrr2(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*vsbrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*vsbss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3))*vsbr2(i1,i2,i3,kd)+(
     & sxx22(i1,i2,i3))*vsbs2(i1,i2,i3,kd)
      vsbyy22(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*vsbrr2(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*vsbrs2(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*vsbss2(i1,i2,i3,kd)+(ryy22(i1,i2,i3))*vsbr2(i1,i2,i3,kd)+(
     & syy22(i1,i2,i3))*vsbs2(i1,i2,i3,kd)
      vsbxy22(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vsbrr2(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & vsbrs2(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vsbss2(i1,i2,i3,
     & kd)+rxy22(i1,i2,i3)*vsbr2(i1,i2,i3,kd)+sxy22(i1,i2,i3)*vsbs2(
     & i1,i2,i3,kd)
      vsbxz22(i1,i2,i3,kd)=0
      vsbyz22(i1,i2,i3,kd)=0
      vsbzz22(i1,i2,i3,kd)=0
      vsblaplacian22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & vsbrr2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)
     & *sy(i1,i2,i3))*vsbrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2)*vsbss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3)+ryy22(i1,i2,i3))*
     & vsbr2(i1,i2,i3,kd)+(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*vsbs2(i1,
     & i2,i3,kd)
      vsbxx23(i1,i2,i3,kd)=rx(i1,i2,i3)**2*vsbrr2(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*vsbss2(i1,i2,i3,kd)+tx(i1,i2,i3)**2*vsbtt2(i1,i2,i3,
     & kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*vsbrs2(i1,i2,i3,kd)+2.*rx(i1,
     & i2,i3)*tx(i1,i2,i3)*vsbrt2(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,
     & i2,i3)*vsbst2(i1,i2,i3,kd)+rxx23(i1,i2,i3)*vsbr2(i1,i2,i3,kd)+
     & sxx23(i1,i2,i3)*vsbs2(i1,i2,i3,kd)+txx23(i1,i2,i3)*vsbt2(i1,i2,
     & i3,kd)
      vsbyy23(i1,i2,i3,kd)=ry(i1,i2,i3)**2*vsbrr2(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*vsbss2(i1,i2,i3,kd)+ty(i1,i2,i3)**2*vsbtt2(i1,i2,i3,
     & kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*vsbrs2(i1,i2,i3,kd)+2.*ry(i1,
     & i2,i3)*ty(i1,i2,i3)*vsbrt2(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,
     & i2,i3)*vsbst2(i1,i2,i3,kd)+ryy23(i1,i2,i3)*vsbr2(i1,i2,i3,kd)+
     & syy23(i1,i2,i3)*vsbs2(i1,i2,i3,kd)+tyy23(i1,i2,i3)*vsbt2(i1,i2,
     & i3,kd)
      vsbzz23(i1,i2,i3,kd)=rz(i1,i2,i3)**2*vsbrr2(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*vsbss2(i1,i2,i3,kd)+tz(i1,i2,i3)**2*vsbtt2(i1,i2,i3,
     & kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*vsbrs2(i1,i2,i3,kd)+2.*rz(i1,
     & i2,i3)*tz(i1,i2,i3)*vsbrt2(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,
     & i2,i3)*vsbst2(i1,i2,i3,kd)+rzz23(i1,i2,i3)*vsbr2(i1,i2,i3,kd)+
     & szz23(i1,i2,i3)*vsbs2(i1,i2,i3,kd)+tzz23(i1,i2,i3)*vsbt2(i1,i2,
     & i3,kd)
      vsbxy23(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vsbrr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vsbss2(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & ty(i1,i2,i3)*vsbtt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(
     & i1,i2,i3)*sx(i1,i2,i3))*vsbrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(
     & i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*vsbrt2(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*vsbst2(i1,i2,
     & i3,kd)+rxy23(i1,i2,i3)*vsbr2(i1,i2,i3,kd)+sxy23(i1,i2,i3)*
     & vsbs2(i1,i2,i3,kd)+txy23(i1,i2,i3)*vsbt2(i1,i2,i3,kd)
      vsbxz23(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*vsbrr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*vsbss2(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & tz(i1,i2,i3)*vsbtt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sx(i1,i2,i3))*vsbrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*vsbrt2(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*vsbst2(i1,i2,
     & i3,kd)+rxz23(i1,i2,i3)*vsbr2(i1,i2,i3,kd)+sxz23(i1,i2,i3)*
     & vsbs2(i1,i2,i3,kd)+txz23(i1,i2,i3)*vsbt2(i1,i2,i3,kd)
      vsbyz23(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*vsbrr2(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*vsbss2(i1,i2,i3,kd)+ty(i1,i2,i3)*
     & tz(i1,i2,i3)*vsbtt2(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sy(i1,i2,i3))*vsbrs2(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*vsbrt2(i1,i2,i3,kd)+(sy(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*vsbst2(i1,i2,
     & i3,kd)+ryz23(i1,i2,i3)*vsbr2(i1,i2,i3,kd)+syz23(i1,i2,i3)*
     & vsbs2(i1,i2,i3,kd)+tyz23(i1,i2,i3)*vsbt2(i1,i2,i3,kd)
      vsblaplacian23(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*vsbrr2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)
     & **2+sz(i1,i2,i3)**2)*vsbss2(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(
     & i1,i2,i3)**2+tz(i1,i2,i3)**2)*vsbtt2(i1,i2,i3,kd)+2.*(rx(i1,i2,
     & i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,
     & i2,i3))*vsbrs2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(
     & i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*vsbrt2(i1,i2,
     & i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)
     & +sz(i1,i2,i3)*tz(i1,i2,i3))*vsbst2(i1,i2,i3,kd)+(rxx23(i1,i2,
     & i3)+ryy23(i1,i2,i3)+rzz23(i1,i2,i3))*vsbr2(i1,i2,i3,kd)+(sxx23(
     & i1,i2,i3)+syy23(i1,i2,i3)+szz23(i1,i2,i3))*vsbs2(i1,i2,i3,kd)+(
     & txx23(i1,i2,i3)+tyy23(i1,i2,i3)+tzz23(i1,i2,i3))*vsbt2(i1,i2,
     & i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      vsbx23r(i1,i2,i3,kd)=(vsb(i1+1,i2,i3,kd)-vsb(i1-1,i2,i3,kd))*h12(
     & 0)
      vsby23r(i1,i2,i3,kd)=(vsb(i1,i2+1,i3,kd)-vsb(i1,i2-1,i3,kd))*h12(
     & 1)
      vsbz23r(i1,i2,i3,kd)=(vsb(i1,i2,i3+1,kd)-vsb(i1,i2,i3-1,kd))*h12(
     & 2)
      vsbxx23r(i1,i2,i3,kd)=(-2.*vsb(i1,i2,i3,kd)+(vsb(i1+1,i2,i3,kd)+
     & vsb(i1-1,i2,i3,kd)) )*h22(0)
      vsbyy23r(i1,i2,i3,kd)=(-2.*vsb(i1,i2,i3,kd)+(vsb(i1,i2+1,i3,kd)+
     & vsb(i1,i2-1,i3,kd)) )*h22(1)
      vsbxy23r(i1,i2,i3,kd)=(vsbx23r(i1,i2+1,i3,kd)-vsbx23r(i1,i2-1,i3,
     & kd))*h12(1)
      vsbzz23r(i1,i2,i3,kd)=(-2.*vsb(i1,i2,i3,kd)+(vsb(i1,i2,i3+1,kd)+
     & vsb(i1,i2,i3-1,kd)) )*h22(2)
      vsbxz23r(i1,i2,i3,kd)=(vsbx23r(i1,i2,i3+1,kd)-vsbx23r(i1,i2,i3-1,
     & kd))*h12(2)
      vsbyz23r(i1,i2,i3,kd)=(vsby23r(i1,i2,i3+1,kd)-vsby23r(i1,i2,i3-1,
     & kd))*h12(2)
      vsbx21r(i1,i2,i3,kd)= vsbx23r(i1,i2,i3,kd)
      vsby21r(i1,i2,i3,kd)= vsby23r(i1,i2,i3,kd)
      vsbz21r(i1,i2,i3,kd)= vsbz23r(i1,i2,i3,kd)
      vsbxx21r(i1,i2,i3,kd)= vsbxx23r(i1,i2,i3,kd)
      vsbyy21r(i1,i2,i3,kd)= vsbyy23r(i1,i2,i3,kd)
      vsbzz21r(i1,i2,i3,kd)= vsbzz23r(i1,i2,i3,kd)
      vsbxy21r(i1,i2,i3,kd)= vsbxy23r(i1,i2,i3,kd)
      vsbxz21r(i1,i2,i3,kd)= vsbxz23r(i1,i2,i3,kd)
      vsbyz21r(i1,i2,i3,kd)= vsbyz23r(i1,i2,i3,kd)
      vsblaplacian21r(i1,i2,i3,kd)=vsbxx23r(i1,i2,i3,kd)
      vsbx22r(i1,i2,i3,kd)= vsbx23r(i1,i2,i3,kd)
      vsby22r(i1,i2,i3,kd)= vsby23r(i1,i2,i3,kd)
      vsbz22r(i1,i2,i3,kd)= vsbz23r(i1,i2,i3,kd)
      vsbxx22r(i1,i2,i3,kd)= vsbxx23r(i1,i2,i3,kd)
      vsbyy22r(i1,i2,i3,kd)= vsbyy23r(i1,i2,i3,kd)
      vsbzz22r(i1,i2,i3,kd)= vsbzz23r(i1,i2,i3,kd)
      vsbxy22r(i1,i2,i3,kd)= vsbxy23r(i1,i2,i3,kd)
      vsbxz22r(i1,i2,i3,kd)= vsbxz23r(i1,i2,i3,kd)
      vsbyz22r(i1,i2,i3,kd)= vsbyz23r(i1,i2,i3,kd)
      vsblaplacian22r(i1,i2,i3,kd)=vsbxx23r(i1,i2,i3,kd)+vsbyy23r(i1,
     & i2,i3,kd)
      vsblaplacian23r(i1,i2,i3,kd)=vsbxx23r(i1,i2,i3,kd)+vsbyy23r(i1,
     & i2,i3,kd)+vsbzz23r(i1,i2,i3,kd)
      vsbxxx22r(i1,i2,i3,kd)=(-2.*(vsb(i1+1,i2,i3,kd)-vsb(i1-1,i2,i3,
     & kd))+(vsb(i1+2,i2,i3,kd)-vsb(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
      vsbyyy22r(i1,i2,i3,kd)=(-2.*(vsb(i1,i2+1,i3,kd)-vsb(i1,i2-1,i3,
     & kd))+(vsb(i1,i2+2,i3,kd)-vsb(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
      vsbxxy22r(i1,i2,i3,kd)=( vsbxx22r(i1,i2+1,i3,kd)-vsbxx22r(i1,i2-
     & 1,i3,kd))/(2.*dx(1))
      vsbxyy22r(i1,i2,i3,kd)=( vsbyy22r(i1+1,i2,i3,kd)-vsbyy22r(i1-1,
     & i2,i3,kd))/(2.*dx(0))
      vsbxxxx22r(i1,i2,i3,kd)=(6.*vsb(i1,i2,i3,kd)-4.*(vsb(i1+1,i2,i3,
     & kd)+vsb(i1-1,i2,i3,kd))+(vsb(i1+2,i2,i3,kd)+vsb(i1-2,i2,i3,kd))
     &  )/(dx(0)**4)
      vsbyyyy22r(i1,i2,i3,kd)=(6.*vsb(i1,i2,i3,kd)-4.*(vsb(i1,i2+1,i3,
     & kd)+vsb(i1,i2-1,i3,kd))+(vsb(i1,i2+2,i3,kd)+vsb(i1,i2-2,i3,kd))
     &  )/(dx(1)**4)
      vsbxxyy22r(i1,i2,i3,kd)=( 4.*vsb(i1,i2,i3,kd)     -2.*(vsb(i1+1,
     & i2,i3,kd)+vsb(i1-1,i2,i3,kd)+vsb(i1,i2+1,i3,kd)+vsb(i1,i2-1,i3,
     & kd))   +   (vsb(i1+1,i2+1,i3,kd)+vsb(i1-1,i2+1,i3,kd)+vsb(i1+1,
     & i2-1,i3,kd)+vsb(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      ! 2D laplacian squared = vsb.xxxx + 2 vsb.xxyy + vsb.yyyy
      vsbLapSq22r(i1,i2,i3,kd)= ( 6.*vsb(i1,i2,i3,kd)   - 4.*(vsb(i1+1,
     & i2,i3,kd)+vsb(i1-1,i2,i3,kd))    +(vsb(i1+2,i2,i3,kd)+vsb(i1-2,
     & i2,i3,kd)) )/(dx(0)**4) +( 6.*vsb(i1,i2,i3,kd)    -4.*(vsb(i1,
     & i2+1,i3,kd)+vsb(i1,i2-1,i3,kd))    +(vsb(i1,i2+2,i3,kd)+vsb(i1,
     & i2-2,i3,kd)) )/(dx(1)**4)  +( 8.*vsb(i1,i2,i3,kd)     -4.*(vsb(
     & i1+1,i2,i3,kd)+vsb(i1-1,i2,i3,kd)+vsb(i1,i2+1,i3,kd)+vsb(i1,i2-
     & 1,i3,kd))   +2.*(vsb(i1+1,i2+1,i3,kd)+vsb(i1-1,i2+1,i3,kd)+vsb(
     & i1+1,i2-1,i3,kd)+vsb(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      vsbxxx23r(i1,i2,i3,kd)=(-2.*(vsb(i1+1,i2,i3,kd)-vsb(i1-1,i2,i3,
     & kd))+(vsb(i1+2,i2,i3,kd)-vsb(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
      vsbyyy23r(i1,i2,i3,kd)=(-2.*(vsb(i1,i2+1,i3,kd)-vsb(i1,i2-1,i3,
     & kd))+(vsb(i1,i2+2,i3,kd)-vsb(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
      vsbzzz23r(i1,i2,i3,kd)=(-2.*(vsb(i1,i2,i3+1,kd)-vsb(i1,i2,i3-1,
     & kd))+(vsb(i1,i2,i3+2,kd)-vsb(i1,i2,i3-2,kd)) )*h22(1)*h12(2)
      vsbxxy23r(i1,i2,i3,kd)=( vsbxx22r(i1,i2+1,i3,kd)-vsbxx22r(i1,i2-
     & 1,i3,kd))/(2.*dx(1))
      vsbxyy23r(i1,i2,i3,kd)=( vsbyy22r(i1+1,i2,i3,kd)-vsbyy22r(i1-1,
     & i2,i3,kd))/(2.*dx(0))
      vsbxxz23r(i1,i2,i3,kd)=( vsbxx22r(i1,i2,i3+1,kd)-vsbxx22r(i1,i2,
     & i3-1,kd))/(2.*dx(2))
      vsbyyz23r(i1,i2,i3,kd)=( vsbyy22r(i1,i2,i3+1,kd)-vsbyy22r(i1,i2,
     & i3-1,kd))/(2.*dx(2))
      vsbxzz23r(i1,i2,i3,kd)=( vsbzz22r(i1+1,i2,i3,kd)-vsbzz22r(i1-1,
     & i2,i3,kd))/(2.*dx(0))
      vsbyzz23r(i1,i2,i3,kd)=( vsbzz22r(i1,i2+1,i3,kd)-vsbzz22r(i1,i2-
     & 1,i3,kd))/(2.*dx(1))
      vsbxxxx23r(i1,i2,i3,kd)=(6.*vsb(i1,i2,i3,kd)-4.*(vsb(i1+1,i2,i3,
     & kd)+vsb(i1-1,i2,i3,kd))+(vsb(i1+2,i2,i3,kd)+vsb(i1-2,i2,i3,kd))
     &  )/(dx(0)**4)
      vsbyyyy23r(i1,i2,i3,kd)=(6.*vsb(i1,i2,i3,kd)-4.*(vsb(i1,i2+1,i3,
     & kd)+vsb(i1,i2-1,i3,kd))+(vsb(i1,i2+2,i3,kd)+vsb(i1,i2-2,i3,kd))
     &  )/(dx(1)**4)
      vsbzzzz23r(i1,i2,i3,kd)=(6.*vsb(i1,i2,i3,kd)-4.*(vsb(i1,i2,i3+1,
     & kd)+vsb(i1,i2,i3-1,kd))+(vsb(i1,i2,i3+2,kd)+vsb(i1,i2,i3-2,kd))
     &  )/(dx(2)**4)
      vsbxxyy23r(i1,i2,i3,kd)=( 4.*vsb(i1,i2,i3,kd)     -2.*(vsb(i1+1,
     & i2,i3,kd)+vsb(i1-1,i2,i3,kd)+vsb(i1,i2+1,i3,kd)+vsb(i1,i2-1,i3,
     & kd))   +   (vsb(i1+1,i2+1,i3,kd)+vsb(i1-1,i2+1,i3,kd)+vsb(i1+1,
     & i2-1,i3,kd)+vsb(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      vsbxxzz23r(i1,i2,i3,kd)=( 4.*vsb(i1,i2,i3,kd)     -2.*(vsb(i1+1,
     & i2,i3,kd)+vsb(i1-1,i2,i3,kd)+vsb(i1,i2,i3+1,kd)+vsb(i1,i2,i3-1,
     & kd))   +   (vsb(i1+1,i2,i3+1,kd)+vsb(i1-1,i2,i3+1,kd)+vsb(i1+1,
     & i2,i3-1,kd)+vsb(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)
      vsbyyzz23r(i1,i2,i3,kd)=( 4.*vsb(i1,i2,i3,kd)     -2.*(vsb(i1,i2+
     & 1,i3,kd)  +vsb(i1,i2-1,i3,kd)+  vsb(i1,i2  ,i3+1,kd)+vsb(i1,i2 
     &  ,i3-1,kd))   +   (vsb(i1,i2+1,i3+1,kd)+vsb(i1,i2-1,i3+1,kd)+
     & vsb(i1,i2+1,i3-1,kd)+vsb(i1,i2-1,i3-1,kd)) )/(dx(1)**2*dx(2)**
     & 2)
      ! 3D laplacian squared = vsb.xxxx + vsb.yyyy + vsb.zzzz + 2 (vsb.xxyy + vsb.xxzz + vsb.yyzz )
      vsbLapSq23r(i1,i2,i3,kd)= ( 6.*vsb(i1,i2,i3,kd)   - 4.*(vsb(i1+1,
     & i2,i3,kd)+vsb(i1-1,i2,i3,kd))    +(vsb(i1+2,i2,i3,kd)+vsb(i1-2,
     & i2,i3,kd)) )/(dx(0)**4) +( 6.*vsb(i1,i2,i3,kd)    -4.*(vsb(i1,
     & i2+1,i3,kd)+vsb(i1,i2-1,i3,kd))    +(vsb(i1,i2+2,i3,kd)+vsb(i1,
     & i2-2,i3,kd)) )/(dx(1)**4)  +( 6.*vsb(i1,i2,i3,kd)    -4.*(vsb(
     & i1,i2,i3+1,kd)+vsb(i1,i2,i3-1,kd))    +(vsb(i1,i2,i3+2,kd)+vsb(
     & i1,i2,i3-2,kd)) )/(dx(2)**4)  +( 8.*vsb(i1,i2,i3,kd)     -4.*(
     & vsb(i1+1,i2,i3,kd)  +vsb(i1-1,i2,i3,kd)  +vsb(i1  ,i2+1,i3,kd)+
     & vsb(i1  ,i2-1,i3,kd))   +2.*(vsb(i1+1,i2+1,i3,kd)+vsb(i1-1,i2+
     & 1,i3,kd)+vsb(i1+1,i2-1,i3,kd)+vsb(i1-1,i2-1,i3,kd)) )/(dx(0)**
     & 2*dx(1)**2)+( 8.*vsb(i1,i2,i3,kd)     -4.*(vsb(i1+1,i2,i3,kd)  
     & +vsb(i1-1,i2,i3,kd)  +vsb(i1  ,i2,i3+1,kd)+vsb(i1  ,i2,i3-1,kd)
     & )   +2.*(vsb(i1+1,i2,i3+1,kd)+vsb(i1-1,i2,i3+1,kd)+vsb(i1+1,i2,
     & i3-1,kd)+vsb(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)+( 8.*vsb(
     & i1,i2,i3,kd)     -4.*(vsb(i1,i2+1,i3,kd)  +vsb(i1,i2-1,i3,kd)  
     & +vsb(i1,i2  ,i3+1,kd)+vsb(i1,i2  ,i3-1,kd))   +2.*(vsb(i1,i2+1,
     & i3+1,kd)+vsb(i1,i2-1,i3+1,kd)+vsb(i1,i2+1,i3-1,kd)+vsb(i1,i2-1,
     & i3-1,kd)) )/(dx(1)**2*dx(2)**2)
      vsbmr2(i1,i2,i3,kd)=(vsbm(i1+1,i2,i3,kd)-vsbm(i1-1,i2,i3,kd))*
     & d12(0)
      vsbms2(i1,i2,i3,kd)=(vsbm(i1,i2+1,i3,kd)-vsbm(i1,i2-1,i3,kd))*
     & d12(1)
      vsbmt2(i1,i2,i3,kd)=(vsbm(i1,i2,i3+1,kd)-vsbm(i1,i2,i3-1,kd))*
     & d12(2)
      vsbmrr2(i1,i2,i3,kd)=(-2.*vsbm(i1,i2,i3,kd)+(vsbm(i1+1,i2,i3,kd)+
     & vsbm(i1-1,i2,i3,kd)) )*d22(0)
      vsbmss2(i1,i2,i3,kd)=(-2.*vsbm(i1,i2,i3,kd)+(vsbm(i1,i2+1,i3,kd)+
     & vsbm(i1,i2-1,i3,kd)) )*d22(1)
      vsbmrs2(i1,i2,i3,kd)=(vsbmr2(i1,i2+1,i3,kd)-vsbmr2(i1,i2-1,i3,kd)
     & )*d12(1)
      vsbmtt2(i1,i2,i3,kd)=(-2.*vsbm(i1,i2,i3,kd)+(vsbm(i1,i2,i3+1,kd)+
     & vsbm(i1,i2,i3-1,kd)) )*d22(2)
      vsbmrt2(i1,i2,i3,kd)=(vsbmr2(i1,i2,i3+1,kd)-vsbmr2(i1,i2,i3-1,kd)
     & )*d12(2)
      vsbmst2(i1,i2,i3,kd)=(vsbms2(i1,i2,i3+1,kd)-vsbms2(i1,i2,i3-1,kd)
     & )*d12(2)
      vsbmrrr2(i1,i2,i3,kd)=(-2.*(vsbm(i1+1,i2,i3,kd)-vsbm(i1-1,i2,i3,
     & kd))+(vsbm(i1+2,i2,i3,kd)-vsbm(i1-2,i2,i3,kd)) )*d22(0)*d12(0)
      vsbmsss2(i1,i2,i3,kd)=(-2.*(vsbm(i1,i2+1,i3,kd)-vsbm(i1,i2-1,i3,
     & kd))+(vsbm(i1,i2+2,i3,kd)-vsbm(i1,i2-2,i3,kd)) )*d22(1)*d12(1)
      vsbmttt2(i1,i2,i3,kd)=(-2.*(vsbm(i1,i2,i3+1,kd)-vsbm(i1,i2,i3-1,
     & kd))+(vsbm(i1,i2,i3+2,kd)-vsbm(i1,i2,i3-2,kd)) )*d22(2)*d12(2)
      vsbmx21(i1,i2,i3,kd)= rx(i1,i2,i3)*vsbmr2(i1,i2,i3,kd)
      vsbmy21(i1,i2,i3,kd)=0
      vsbmz21(i1,i2,i3,kd)=0
      vsbmx22(i1,i2,i3,kd)= rx(i1,i2,i3)*vsbmr2(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*vsbms2(i1,i2,i3,kd)
      vsbmy22(i1,i2,i3,kd)= ry(i1,i2,i3)*vsbmr2(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*vsbms2(i1,i2,i3,kd)
      vsbmz22(i1,i2,i3,kd)=0
      vsbmx23(i1,i2,i3,kd)=rx(i1,i2,i3)*vsbmr2(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*vsbms2(i1,i2,i3,kd)+tx(i1,i2,i3)*vsbmt2(i1,i2,i3,kd)
      vsbmy23(i1,i2,i3,kd)=ry(i1,i2,i3)*vsbmr2(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*vsbms2(i1,i2,i3,kd)+ty(i1,i2,i3)*vsbmt2(i1,i2,i3,kd)
      vsbmz23(i1,i2,i3,kd)=rz(i1,i2,i3)*vsbmr2(i1,i2,i3,kd)+sz(i1,i2,
     & i3)*vsbms2(i1,i2,i3,kd)+tz(i1,i2,i3)*vsbmt2(i1,i2,i3,kd)
      vsbmxx21(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vsbmrr2(i1,i2,i3,kd)+(
     & rxx22(i1,i2,i3))*vsbmr2(i1,i2,i3,kd)
      vsbmyy21(i1,i2,i3,kd)=0
      vsbmxy21(i1,i2,i3,kd)=0
      vsbmxz21(i1,i2,i3,kd)=0
      vsbmyz21(i1,i2,i3,kd)=0
      vsbmzz21(i1,i2,i3,kd)=0
      vsbmlaplacian21(i1,i2,i3,kd)=vsbmxx21(i1,i2,i3,kd)
      vsbmxx22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vsbmrr2(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*vsbmrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*vsbmss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3))*vsbmr2(i1,i2,i3,kd)+(
     & sxx22(i1,i2,i3))*vsbms2(i1,i2,i3,kd)
      vsbmyy22(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*vsbmrr2(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*vsbmrs2(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*vsbmss2(i1,i2,i3,kd)+(ryy22(i1,i2,i3))*vsbmr2(i1,i2,i3,kd)+(
     & syy22(i1,i2,i3))*vsbms2(i1,i2,i3,kd)
      vsbmxy22(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vsbmrr2(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & vsbmrs2(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vsbmss2(i1,i2,
     & i3,kd)+rxy22(i1,i2,i3)*vsbmr2(i1,i2,i3,kd)+sxy22(i1,i2,i3)*
     & vsbms2(i1,i2,i3,kd)
      vsbmxz22(i1,i2,i3,kd)=0
      vsbmyz22(i1,i2,i3,kd)=0
      vsbmzz22(i1,i2,i3,kd)=0
      vsbmlaplacian22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & vsbmrr2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,
     & i3)*sy(i1,i2,i3))*vsbmrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,
     & i2,i3)**2)*vsbmss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3)+ryy22(i1,i2,
     & i3))*vsbmr2(i1,i2,i3,kd)+(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*
     & vsbms2(i1,i2,i3,kd)
      vsbmxx23(i1,i2,i3,kd)=rx(i1,i2,i3)**2*vsbmrr2(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*vsbmss2(i1,i2,i3,kd)+tx(i1,i2,i3)**2*vsbmtt2(i1,i2,
     & i3,kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*vsbmrs2(i1,i2,i3,kd)+2.*rx(
     & i1,i2,i3)*tx(i1,i2,i3)*vsbmrt2(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(
     & i1,i2,i3)*vsbmst2(i1,i2,i3,kd)+rxx23(i1,i2,i3)*vsbmr2(i1,i2,i3,
     & kd)+sxx23(i1,i2,i3)*vsbms2(i1,i2,i3,kd)+txx23(i1,i2,i3)*vsbmt2(
     & i1,i2,i3,kd)
      vsbmyy23(i1,i2,i3,kd)=ry(i1,i2,i3)**2*vsbmrr2(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*vsbmss2(i1,i2,i3,kd)+ty(i1,i2,i3)**2*vsbmtt2(i1,i2,
     & i3,kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*vsbmrs2(i1,i2,i3,kd)+2.*ry(
     & i1,i2,i3)*ty(i1,i2,i3)*vsbmrt2(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(
     & i1,i2,i3)*vsbmst2(i1,i2,i3,kd)+ryy23(i1,i2,i3)*vsbmr2(i1,i2,i3,
     & kd)+syy23(i1,i2,i3)*vsbms2(i1,i2,i3,kd)+tyy23(i1,i2,i3)*vsbmt2(
     & i1,i2,i3,kd)
      vsbmzz23(i1,i2,i3,kd)=rz(i1,i2,i3)**2*vsbmrr2(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*vsbmss2(i1,i2,i3,kd)+tz(i1,i2,i3)**2*vsbmtt2(i1,i2,
     & i3,kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*vsbmrs2(i1,i2,i3,kd)+2.*rz(
     & i1,i2,i3)*tz(i1,i2,i3)*vsbmrt2(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(
     & i1,i2,i3)*vsbmst2(i1,i2,i3,kd)+rzz23(i1,i2,i3)*vsbmr2(i1,i2,i3,
     & kd)+szz23(i1,i2,i3)*vsbms2(i1,i2,i3,kd)+tzz23(i1,i2,i3)*vsbmt2(
     & i1,i2,i3,kd)
      vsbmxy23(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vsbmrr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vsbmss2(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *ty(i1,i2,i3)*vsbmtt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+
     & ry(i1,i2,i3)*sx(i1,i2,i3))*vsbmrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & ty(i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*vsbmrt2(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*vsbmst2(
     & i1,i2,i3,kd)+rxy23(i1,i2,i3)*vsbmr2(i1,i2,i3,kd)+sxy23(i1,i2,
     & i3)*vsbms2(i1,i2,i3,kd)+txy23(i1,i2,i3)*vsbmt2(i1,i2,i3,kd)
      vsbmxz23(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*vsbmrr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*vsbmss2(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *tz(i1,i2,i3)*vsbmtt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sx(i1,i2,i3))*vsbmrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*vsbmrt2(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*vsbmst2(
     & i1,i2,i3,kd)+rxz23(i1,i2,i3)*vsbmr2(i1,i2,i3,kd)+sxz23(i1,i2,
     & i3)*vsbms2(i1,i2,i3,kd)+txz23(i1,i2,i3)*vsbmt2(i1,i2,i3,kd)
      vsbmyz23(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*vsbmrr2(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*vsbmss2(i1,i2,i3,kd)+ty(i1,i2,i3)
     & *tz(i1,i2,i3)*vsbmtt2(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sy(i1,i2,i3))*vsbmrs2(i1,i2,i3,kd)+(ry(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*vsbmrt2(i1,i2,i3,kd)+(
     & sy(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*vsbmst2(
     & i1,i2,i3,kd)+ryz23(i1,i2,i3)*vsbmr2(i1,i2,i3,kd)+syz23(i1,i2,
     & i3)*vsbms2(i1,i2,i3,kd)+tyz23(i1,i2,i3)*vsbmt2(i1,i2,i3,kd)
      vsbmlaplacian23(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*vsbmrr2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2+sz(i1,i2,i3)**2)*vsbmss2(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+
     & ty(i1,i2,i3)**2+tz(i1,i2,i3)**2)*vsbmtt2(i1,i2,i3,kd)+2.*(rx(
     & i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*
     & sz(i1,i2,i3))*vsbmrs2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,
     & i3)+ ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*
     & vsbmrt2(i1,i2,i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,
     & i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3))*vsbmst2(i1,i2,i3,
     & kd)+(rxx23(i1,i2,i3)+ryy23(i1,i2,i3)+rzz23(i1,i2,i3))*vsbmr2(
     & i1,i2,i3,kd)+(sxx23(i1,i2,i3)+syy23(i1,i2,i3)+szz23(i1,i2,i3))*
     & vsbms2(i1,i2,i3,kd)+(txx23(i1,i2,i3)+tyy23(i1,i2,i3)+tzz23(i1,
     & i2,i3))*vsbmt2(i1,i2,i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      vsbmx23r(i1,i2,i3,kd)=(vsbm(i1+1,i2,i3,kd)-vsbm(i1-1,i2,i3,kd))*
     & h12(0)
      vsbmy23r(i1,i2,i3,kd)=(vsbm(i1,i2+1,i3,kd)-vsbm(i1,i2-1,i3,kd))*
     & h12(1)
      vsbmz23r(i1,i2,i3,kd)=(vsbm(i1,i2,i3+1,kd)-vsbm(i1,i2,i3-1,kd))*
     & h12(2)
      vsbmxx23r(i1,i2,i3,kd)=(-2.*vsbm(i1,i2,i3,kd)+(vsbm(i1+1,i2,i3,
     & kd)+vsbm(i1-1,i2,i3,kd)) )*h22(0)
      vsbmyy23r(i1,i2,i3,kd)=(-2.*vsbm(i1,i2,i3,kd)+(vsbm(i1,i2+1,i3,
     & kd)+vsbm(i1,i2-1,i3,kd)) )*h22(1)
      vsbmxy23r(i1,i2,i3,kd)=(vsbmx23r(i1,i2+1,i3,kd)-vsbmx23r(i1,i2-1,
     & i3,kd))*h12(1)
      vsbmzz23r(i1,i2,i3,kd)=(-2.*vsbm(i1,i2,i3,kd)+(vsbm(i1,i2,i3+1,
     & kd)+vsbm(i1,i2,i3-1,kd)) )*h22(2)
      vsbmxz23r(i1,i2,i3,kd)=(vsbmx23r(i1,i2,i3+1,kd)-vsbmx23r(i1,i2,
     & i3-1,kd))*h12(2)
      vsbmyz23r(i1,i2,i3,kd)=(vsbmy23r(i1,i2,i3+1,kd)-vsbmy23r(i1,i2,
     & i3-1,kd))*h12(2)
      vsbmx21r(i1,i2,i3,kd)= vsbmx23r(i1,i2,i3,kd)
      vsbmy21r(i1,i2,i3,kd)= vsbmy23r(i1,i2,i3,kd)
      vsbmz21r(i1,i2,i3,kd)= vsbmz23r(i1,i2,i3,kd)
      vsbmxx21r(i1,i2,i3,kd)= vsbmxx23r(i1,i2,i3,kd)
      vsbmyy21r(i1,i2,i3,kd)= vsbmyy23r(i1,i2,i3,kd)
      vsbmzz21r(i1,i2,i3,kd)= vsbmzz23r(i1,i2,i3,kd)
      vsbmxy21r(i1,i2,i3,kd)= vsbmxy23r(i1,i2,i3,kd)
      vsbmxz21r(i1,i2,i3,kd)= vsbmxz23r(i1,i2,i3,kd)
      vsbmyz21r(i1,i2,i3,kd)= vsbmyz23r(i1,i2,i3,kd)
      vsbmlaplacian21r(i1,i2,i3,kd)=vsbmxx23r(i1,i2,i3,kd)
      vsbmx22r(i1,i2,i3,kd)= vsbmx23r(i1,i2,i3,kd)
      vsbmy22r(i1,i2,i3,kd)= vsbmy23r(i1,i2,i3,kd)
      vsbmz22r(i1,i2,i3,kd)= vsbmz23r(i1,i2,i3,kd)
      vsbmxx22r(i1,i2,i3,kd)= vsbmxx23r(i1,i2,i3,kd)
      vsbmyy22r(i1,i2,i3,kd)= vsbmyy23r(i1,i2,i3,kd)
      vsbmzz22r(i1,i2,i3,kd)= vsbmzz23r(i1,i2,i3,kd)
      vsbmxy22r(i1,i2,i3,kd)= vsbmxy23r(i1,i2,i3,kd)
      vsbmxz22r(i1,i2,i3,kd)= vsbmxz23r(i1,i2,i3,kd)
      vsbmyz22r(i1,i2,i3,kd)= vsbmyz23r(i1,i2,i3,kd)
      vsbmlaplacian22r(i1,i2,i3,kd)=vsbmxx23r(i1,i2,i3,kd)+vsbmyy23r(
     & i1,i2,i3,kd)
      vsbmlaplacian23r(i1,i2,i3,kd)=vsbmxx23r(i1,i2,i3,kd)+vsbmyy23r(
     & i1,i2,i3,kd)+vsbmzz23r(i1,i2,i3,kd)
      vsbmxxx22r(i1,i2,i3,kd)=(-2.*(vsbm(i1+1,i2,i3,kd)-vsbm(i1-1,i2,
     & i3,kd))+(vsbm(i1+2,i2,i3,kd)-vsbm(i1-2,i2,i3,kd)) )*h22(0)*h12(
     & 0)
      vsbmyyy22r(i1,i2,i3,kd)=(-2.*(vsbm(i1,i2+1,i3,kd)-vsbm(i1,i2-1,
     & i3,kd))+(vsbm(i1,i2+2,i3,kd)-vsbm(i1,i2-2,i3,kd)) )*h22(1)*h12(
     & 1)
      vsbmxxy22r(i1,i2,i3,kd)=( vsbmxx22r(i1,i2+1,i3,kd)-vsbmxx22r(i1,
     & i2-1,i3,kd))/(2.*dx(1))
      vsbmxyy22r(i1,i2,i3,kd)=( vsbmyy22r(i1+1,i2,i3,kd)-vsbmyy22r(i1-
     & 1,i2,i3,kd))/(2.*dx(0))
      vsbmxxxx22r(i1,i2,i3,kd)=(6.*vsbm(i1,i2,i3,kd)-4.*(vsbm(i1+1,i2,
     & i3,kd)+vsbm(i1-1,i2,i3,kd))+(vsbm(i1+2,i2,i3,kd)+vsbm(i1-2,i2,
     & i3,kd)) )/(dx(0)**4)
      vsbmyyyy22r(i1,i2,i3,kd)=(6.*vsbm(i1,i2,i3,kd)-4.*(vsbm(i1,i2+1,
     & i3,kd)+vsbm(i1,i2-1,i3,kd))+(vsbm(i1,i2+2,i3,kd)+vsbm(i1,i2-2,
     & i3,kd)) )/(dx(1)**4)
      vsbmxxyy22r(i1,i2,i3,kd)=( 4.*vsbm(i1,i2,i3,kd)     -2.*(vsbm(i1+
     & 1,i2,i3,kd)+vsbm(i1-1,i2,i3,kd)+vsbm(i1,i2+1,i3,kd)+vsbm(i1,i2-
     & 1,i3,kd))   +   (vsbm(i1+1,i2+1,i3,kd)+vsbm(i1-1,i2+1,i3,kd)+
     & vsbm(i1+1,i2-1,i3,kd)+vsbm(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)*
     & *2)
      ! 2D laplacian squared = vsbm.xxxx + 2 vsbm.xxyy + vsbm.yyyy
      vsbmLapSq22r(i1,i2,i3,kd)= ( 6.*vsbm(i1,i2,i3,kd)   - 4.*(vsbm(
     & i1+1,i2,i3,kd)+vsbm(i1-1,i2,i3,kd))    +(vsbm(i1+2,i2,i3,kd)+
     & vsbm(i1-2,i2,i3,kd)) )/(dx(0)**4) +( 6.*vsbm(i1,i2,i3,kd)    -
     & 4.*(vsbm(i1,i2+1,i3,kd)+vsbm(i1,i2-1,i3,kd))    +(vsbm(i1,i2+2,
     & i3,kd)+vsbm(i1,i2-2,i3,kd)) )/(dx(1)**4)  +( 8.*vsbm(i1,i2,i3,
     & kd)     -4.*(vsbm(i1+1,i2,i3,kd)+vsbm(i1-1,i2,i3,kd)+vsbm(i1,
     & i2+1,i3,kd)+vsbm(i1,i2-1,i3,kd))   +2.*(vsbm(i1+1,i2+1,i3,kd)+
     & vsbm(i1-1,i2+1,i3,kd)+vsbm(i1+1,i2-1,i3,kd)+vsbm(i1-1,i2-1,i3,
     & kd)) )/(dx(0)**2*dx(1)**2)
      vsbmxxx23r(i1,i2,i3,kd)=(-2.*(vsbm(i1+1,i2,i3,kd)-vsbm(i1-1,i2,
     & i3,kd))+(vsbm(i1+2,i2,i3,kd)-vsbm(i1-2,i2,i3,kd)) )*h22(0)*h12(
     & 0)
      vsbmyyy23r(i1,i2,i3,kd)=(-2.*(vsbm(i1,i2+1,i3,kd)-vsbm(i1,i2-1,
     & i3,kd))+(vsbm(i1,i2+2,i3,kd)-vsbm(i1,i2-2,i3,kd)) )*h22(1)*h12(
     & 1)
      vsbmzzz23r(i1,i2,i3,kd)=(-2.*(vsbm(i1,i2,i3+1,kd)-vsbm(i1,i2,i3-
     & 1,kd))+(vsbm(i1,i2,i3+2,kd)-vsbm(i1,i2,i3-2,kd)) )*h22(1)*h12(
     & 2)
      vsbmxxy23r(i1,i2,i3,kd)=( vsbmxx22r(i1,i2+1,i3,kd)-vsbmxx22r(i1,
     & i2-1,i3,kd))/(2.*dx(1))
      vsbmxyy23r(i1,i2,i3,kd)=( vsbmyy22r(i1+1,i2,i3,kd)-vsbmyy22r(i1-
     & 1,i2,i3,kd))/(2.*dx(0))
      vsbmxxz23r(i1,i2,i3,kd)=( vsbmxx22r(i1,i2,i3+1,kd)-vsbmxx22r(i1,
     & i2,i3-1,kd))/(2.*dx(2))
      vsbmyyz23r(i1,i2,i3,kd)=( vsbmyy22r(i1,i2,i3+1,kd)-vsbmyy22r(i1,
     & i2,i3-1,kd))/(2.*dx(2))
      vsbmxzz23r(i1,i2,i3,kd)=( vsbmzz22r(i1+1,i2,i3,kd)-vsbmzz22r(i1-
     & 1,i2,i3,kd))/(2.*dx(0))
      vsbmyzz23r(i1,i2,i3,kd)=( vsbmzz22r(i1,i2+1,i3,kd)-vsbmzz22r(i1,
     & i2-1,i3,kd))/(2.*dx(1))
      vsbmxxxx23r(i1,i2,i3,kd)=(6.*vsbm(i1,i2,i3,kd)-4.*(vsbm(i1+1,i2,
     & i3,kd)+vsbm(i1-1,i2,i3,kd))+(vsbm(i1+2,i2,i3,kd)+vsbm(i1-2,i2,
     & i3,kd)) )/(dx(0)**4)
      vsbmyyyy23r(i1,i2,i3,kd)=(6.*vsbm(i1,i2,i3,kd)-4.*(vsbm(i1,i2+1,
     & i3,kd)+vsbm(i1,i2-1,i3,kd))+(vsbm(i1,i2+2,i3,kd)+vsbm(i1,i2-2,
     & i3,kd)) )/(dx(1)**4)
      vsbmzzzz23r(i1,i2,i3,kd)=(6.*vsbm(i1,i2,i3,kd)-4.*(vsbm(i1,i2,i3+
     & 1,kd)+vsbm(i1,i2,i3-1,kd))+(vsbm(i1,i2,i3+2,kd)+vsbm(i1,i2,i3-
     & 2,kd)) )/(dx(2)**4)
      vsbmxxyy23r(i1,i2,i3,kd)=( 4.*vsbm(i1,i2,i3,kd)     -2.*(vsbm(i1+
     & 1,i2,i3,kd)+vsbm(i1-1,i2,i3,kd)+vsbm(i1,i2+1,i3,kd)+vsbm(i1,i2-
     & 1,i3,kd))   +   (vsbm(i1+1,i2+1,i3,kd)+vsbm(i1-1,i2+1,i3,kd)+
     & vsbm(i1+1,i2-1,i3,kd)+vsbm(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)*
     & *2)
      vsbmxxzz23r(i1,i2,i3,kd)=( 4.*vsbm(i1,i2,i3,kd)     -2.*(vsbm(i1+
     & 1,i2,i3,kd)+vsbm(i1-1,i2,i3,kd)+vsbm(i1,i2,i3+1,kd)+vsbm(i1,i2,
     & i3-1,kd))   +   (vsbm(i1+1,i2,i3+1,kd)+vsbm(i1-1,i2,i3+1,kd)+
     & vsbm(i1+1,i2,i3-1,kd)+vsbm(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)*
     & *2)
      vsbmyyzz23r(i1,i2,i3,kd)=( 4.*vsbm(i1,i2,i3,kd)     -2.*(vsbm(i1,
     & i2+1,i3,kd)  +vsbm(i1,i2-1,i3,kd)+  vsbm(i1,i2  ,i3+1,kd)+vsbm(
     & i1,i2  ,i3-1,kd))   +   (vsbm(i1,i2+1,i3+1,kd)+vsbm(i1,i2-1,i3+
     & 1,kd)+vsbm(i1,i2+1,i3-1,kd)+vsbm(i1,i2-1,i3-1,kd)) )/(dx(1)**2*
     & dx(2)**2)
      ! 3D laplacian squared = vsbm.xxxx + vsbm.yyyy + vsbm.zzzz + 2 (vsbm.xxyy + vsbm.xxzz + vsbm.yyzz )
      vsbmLapSq23r(i1,i2,i3,kd)= ( 6.*vsbm(i1,i2,i3,kd)   - 4.*(vsbm(
     & i1+1,i2,i3,kd)+vsbm(i1-1,i2,i3,kd))    +(vsbm(i1+2,i2,i3,kd)+
     & vsbm(i1-2,i2,i3,kd)) )/(dx(0)**4) +( 6.*vsbm(i1,i2,i3,kd)    -
     & 4.*(vsbm(i1,i2+1,i3,kd)+vsbm(i1,i2-1,i3,kd))    +(vsbm(i1,i2+2,
     & i3,kd)+vsbm(i1,i2-2,i3,kd)) )/(dx(1)**4)  +( 6.*vsbm(i1,i2,i3,
     & kd)    -4.*(vsbm(i1,i2,i3+1,kd)+vsbm(i1,i2,i3-1,kd))    +(vsbm(
     & i1,i2,i3+2,kd)+vsbm(i1,i2,i3-2,kd)) )/(dx(2)**4)  +( 8.*vsbm(
     & i1,i2,i3,kd)     -4.*(vsbm(i1+1,i2,i3,kd)  +vsbm(i1-1,i2,i3,kd)
     &   +vsbm(i1  ,i2+1,i3,kd)+vsbm(i1  ,i2-1,i3,kd))   +2.*(vsbm(i1+
     & 1,i2+1,i3,kd)+vsbm(i1-1,i2+1,i3,kd)+vsbm(i1+1,i2-1,i3,kd)+vsbm(
     & i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)+( 8.*vsbm(i1,i2,i3,kd) 
     &     -4.*(vsbm(i1+1,i2,i3,kd)  +vsbm(i1-1,i2,i3,kd)  +vsbm(i1  ,
     & i2,i3+1,kd)+vsbm(i1  ,i2,i3-1,kd))   +2.*(vsbm(i1+1,i2,i3+1,kd)
     & +vsbm(i1-1,i2,i3+1,kd)+vsbm(i1+1,i2,i3-1,kd)+vsbm(i1-1,i2,i3-1,
     & kd)) )/(dx(0)**2*dx(2)**2)+( 8.*vsbm(i1,i2,i3,kd)     -4.*(
     & vsbm(i1,i2+1,i3,kd)  +vsbm(i1,i2-1,i3,kd)  +vsbm(i1,i2  ,i3+1,
     & kd)+vsbm(i1,i2  ,i3-1,kd))   +2.*(vsbm(i1,i2+1,i3+1,kd)+vsbm(
     & i1,i2-1,i3+1,kd)+vsbm(i1,i2+1,i3-1,kd)+vsbm(i1,i2-1,i3-1,kd)) )
     & /(dx(1)**2*dx(2)**2)
      wsbr2(i1,i2,i3,kd)=(wsb(i1+1,i2,i3,kd)-wsb(i1-1,i2,i3,kd))*d12(0)
      wsbs2(i1,i2,i3,kd)=(wsb(i1,i2+1,i3,kd)-wsb(i1,i2-1,i3,kd))*d12(1)
      wsbt2(i1,i2,i3,kd)=(wsb(i1,i2,i3+1,kd)-wsb(i1,i2,i3-1,kd))*d12(2)
      wsbrr2(i1,i2,i3,kd)=(-2.*wsb(i1,i2,i3,kd)+(wsb(i1+1,i2,i3,kd)+
     & wsb(i1-1,i2,i3,kd)) )*d22(0)
      wsbss2(i1,i2,i3,kd)=(-2.*wsb(i1,i2,i3,kd)+(wsb(i1,i2+1,i3,kd)+
     & wsb(i1,i2-1,i3,kd)) )*d22(1)
      wsbrs2(i1,i2,i3,kd)=(wsbr2(i1,i2+1,i3,kd)-wsbr2(i1,i2-1,i3,kd))*
     & d12(1)
      wsbtt2(i1,i2,i3,kd)=(-2.*wsb(i1,i2,i3,kd)+(wsb(i1,i2,i3+1,kd)+
     & wsb(i1,i2,i3-1,kd)) )*d22(2)
      wsbrt2(i1,i2,i3,kd)=(wsbr2(i1,i2,i3+1,kd)-wsbr2(i1,i2,i3-1,kd))*
     & d12(2)
      wsbst2(i1,i2,i3,kd)=(wsbs2(i1,i2,i3+1,kd)-wsbs2(i1,i2,i3-1,kd))*
     & d12(2)
      wsbrrr2(i1,i2,i3,kd)=(-2.*(wsb(i1+1,i2,i3,kd)-wsb(i1-1,i2,i3,kd))
     & +(wsb(i1+2,i2,i3,kd)-wsb(i1-2,i2,i3,kd)) )*d22(0)*d12(0)
      wsbsss2(i1,i2,i3,kd)=(-2.*(wsb(i1,i2+1,i3,kd)-wsb(i1,i2-1,i3,kd))
     & +(wsb(i1,i2+2,i3,kd)-wsb(i1,i2-2,i3,kd)) )*d22(1)*d12(1)
      wsbttt2(i1,i2,i3,kd)=(-2.*(wsb(i1,i2,i3+1,kd)-wsb(i1,i2,i3-1,kd))
     & +(wsb(i1,i2,i3+2,kd)-wsb(i1,i2,i3-2,kd)) )*d22(2)*d12(2)
      wsbx21(i1,i2,i3,kd)= rx(i1,i2,i3)*wsbr2(i1,i2,i3,kd)
      wsby21(i1,i2,i3,kd)=0
      wsbz21(i1,i2,i3,kd)=0
      wsbx22(i1,i2,i3,kd)= rx(i1,i2,i3)*wsbr2(i1,i2,i3,kd)+sx(i1,i2,i3)
     & *wsbs2(i1,i2,i3,kd)
      wsby22(i1,i2,i3,kd)= ry(i1,i2,i3)*wsbr2(i1,i2,i3,kd)+sy(i1,i2,i3)
     & *wsbs2(i1,i2,i3,kd)
      wsbz22(i1,i2,i3,kd)=0
      wsbx23(i1,i2,i3,kd)=rx(i1,i2,i3)*wsbr2(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & wsbs2(i1,i2,i3,kd)+tx(i1,i2,i3)*wsbt2(i1,i2,i3,kd)
      wsby23(i1,i2,i3,kd)=ry(i1,i2,i3)*wsbr2(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & wsbs2(i1,i2,i3,kd)+ty(i1,i2,i3)*wsbt2(i1,i2,i3,kd)
      wsbz23(i1,i2,i3,kd)=rz(i1,i2,i3)*wsbr2(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & wsbs2(i1,i2,i3,kd)+tz(i1,i2,i3)*wsbt2(i1,i2,i3,kd)
      wsbxx21(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wsbrr2(i1,i2,i3,kd)+(
     & rxx22(i1,i2,i3))*wsbr2(i1,i2,i3,kd)
      wsbyy21(i1,i2,i3,kd)=0
      wsbxy21(i1,i2,i3,kd)=0
      wsbxz21(i1,i2,i3,kd)=0
      wsbyz21(i1,i2,i3,kd)=0
      wsbzz21(i1,i2,i3,kd)=0
      wsblaplacian21(i1,i2,i3,kd)=wsbxx21(i1,i2,i3,kd)
      wsbxx22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wsbrr2(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*wsbrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*wsbss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3))*wsbr2(i1,i2,i3,kd)+(
     & sxx22(i1,i2,i3))*wsbs2(i1,i2,i3,kd)
      wsbyy22(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*wsbrr2(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*wsbrs2(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*wsbss2(i1,i2,i3,kd)+(ryy22(i1,i2,i3))*wsbr2(i1,i2,i3,kd)+(
     & syy22(i1,i2,i3))*wsbs2(i1,i2,i3,kd)
      wsbxy22(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wsbrr2(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & wsbrs2(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wsbss2(i1,i2,i3,
     & kd)+rxy22(i1,i2,i3)*wsbr2(i1,i2,i3,kd)+sxy22(i1,i2,i3)*wsbs2(
     & i1,i2,i3,kd)
      wsbxz22(i1,i2,i3,kd)=0
      wsbyz22(i1,i2,i3,kd)=0
      wsbzz22(i1,i2,i3,kd)=0
      wsblaplacian22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & wsbrr2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)
     & *sy(i1,i2,i3))*wsbrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2)*wsbss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3)+ryy22(i1,i2,i3))*
     & wsbr2(i1,i2,i3,kd)+(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*wsbs2(i1,
     & i2,i3,kd)
      wsbxx23(i1,i2,i3,kd)=rx(i1,i2,i3)**2*wsbrr2(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*wsbss2(i1,i2,i3,kd)+tx(i1,i2,i3)**2*wsbtt2(i1,i2,i3,
     & kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*wsbrs2(i1,i2,i3,kd)+2.*rx(i1,
     & i2,i3)*tx(i1,i2,i3)*wsbrt2(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,
     & i2,i3)*wsbst2(i1,i2,i3,kd)+rxx23(i1,i2,i3)*wsbr2(i1,i2,i3,kd)+
     & sxx23(i1,i2,i3)*wsbs2(i1,i2,i3,kd)+txx23(i1,i2,i3)*wsbt2(i1,i2,
     & i3,kd)
      wsbyy23(i1,i2,i3,kd)=ry(i1,i2,i3)**2*wsbrr2(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*wsbss2(i1,i2,i3,kd)+ty(i1,i2,i3)**2*wsbtt2(i1,i2,i3,
     & kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*wsbrs2(i1,i2,i3,kd)+2.*ry(i1,
     & i2,i3)*ty(i1,i2,i3)*wsbrt2(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,
     & i2,i3)*wsbst2(i1,i2,i3,kd)+ryy23(i1,i2,i3)*wsbr2(i1,i2,i3,kd)+
     & syy23(i1,i2,i3)*wsbs2(i1,i2,i3,kd)+tyy23(i1,i2,i3)*wsbt2(i1,i2,
     & i3,kd)
      wsbzz23(i1,i2,i3,kd)=rz(i1,i2,i3)**2*wsbrr2(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*wsbss2(i1,i2,i3,kd)+tz(i1,i2,i3)**2*wsbtt2(i1,i2,i3,
     & kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*wsbrs2(i1,i2,i3,kd)+2.*rz(i1,
     & i2,i3)*tz(i1,i2,i3)*wsbrt2(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,
     & i2,i3)*wsbst2(i1,i2,i3,kd)+rzz23(i1,i2,i3)*wsbr2(i1,i2,i3,kd)+
     & szz23(i1,i2,i3)*wsbs2(i1,i2,i3,kd)+tzz23(i1,i2,i3)*wsbt2(i1,i2,
     & i3,kd)
      wsbxy23(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wsbrr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wsbss2(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & ty(i1,i2,i3)*wsbtt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(
     & i1,i2,i3)*sx(i1,i2,i3))*wsbrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(
     & i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*wsbrt2(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*wsbst2(i1,i2,
     & i3,kd)+rxy23(i1,i2,i3)*wsbr2(i1,i2,i3,kd)+sxy23(i1,i2,i3)*
     & wsbs2(i1,i2,i3,kd)+txy23(i1,i2,i3)*wsbt2(i1,i2,i3,kd)
      wsbxz23(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*wsbrr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*wsbss2(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & tz(i1,i2,i3)*wsbtt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sx(i1,i2,i3))*wsbrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*wsbrt2(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*wsbst2(i1,i2,
     & i3,kd)+rxz23(i1,i2,i3)*wsbr2(i1,i2,i3,kd)+sxz23(i1,i2,i3)*
     & wsbs2(i1,i2,i3,kd)+txz23(i1,i2,i3)*wsbt2(i1,i2,i3,kd)
      wsbyz23(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*wsbrr2(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*wsbss2(i1,i2,i3,kd)+ty(i1,i2,i3)*
     & tz(i1,i2,i3)*wsbtt2(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sy(i1,i2,i3))*wsbrs2(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*wsbrt2(i1,i2,i3,kd)+(sy(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*wsbst2(i1,i2,
     & i3,kd)+ryz23(i1,i2,i3)*wsbr2(i1,i2,i3,kd)+syz23(i1,i2,i3)*
     & wsbs2(i1,i2,i3,kd)+tyz23(i1,i2,i3)*wsbt2(i1,i2,i3,kd)
      wsblaplacian23(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*wsbrr2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)
     & **2+sz(i1,i2,i3)**2)*wsbss2(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(
     & i1,i2,i3)**2+tz(i1,i2,i3)**2)*wsbtt2(i1,i2,i3,kd)+2.*(rx(i1,i2,
     & i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,
     & i2,i3))*wsbrs2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(
     & i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*wsbrt2(i1,i2,
     & i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)
     & +sz(i1,i2,i3)*tz(i1,i2,i3))*wsbst2(i1,i2,i3,kd)+(rxx23(i1,i2,
     & i3)+ryy23(i1,i2,i3)+rzz23(i1,i2,i3))*wsbr2(i1,i2,i3,kd)+(sxx23(
     & i1,i2,i3)+syy23(i1,i2,i3)+szz23(i1,i2,i3))*wsbs2(i1,i2,i3,kd)+(
     & txx23(i1,i2,i3)+tyy23(i1,i2,i3)+tzz23(i1,i2,i3))*wsbt2(i1,i2,
     & i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      wsbx23r(i1,i2,i3,kd)=(wsb(i1+1,i2,i3,kd)-wsb(i1-1,i2,i3,kd))*h12(
     & 0)
      wsby23r(i1,i2,i3,kd)=(wsb(i1,i2+1,i3,kd)-wsb(i1,i2-1,i3,kd))*h12(
     & 1)
      wsbz23r(i1,i2,i3,kd)=(wsb(i1,i2,i3+1,kd)-wsb(i1,i2,i3-1,kd))*h12(
     & 2)
      wsbxx23r(i1,i2,i3,kd)=(-2.*wsb(i1,i2,i3,kd)+(wsb(i1+1,i2,i3,kd)+
     & wsb(i1-1,i2,i3,kd)) )*h22(0)
      wsbyy23r(i1,i2,i3,kd)=(-2.*wsb(i1,i2,i3,kd)+(wsb(i1,i2+1,i3,kd)+
     & wsb(i1,i2-1,i3,kd)) )*h22(1)
      wsbxy23r(i1,i2,i3,kd)=(wsbx23r(i1,i2+1,i3,kd)-wsbx23r(i1,i2-1,i3,
     & kd))*h12(1)
      wsbzz23r(i1,i2,i3,kd)=(-2.*wsb(i1,i2,i3,kd)+(wsb(i1,i2,i3+1,kd)+
     & wsb(i1,i2,i3-1,kd)) )*h22(2)
      wsbxz23r(i1,i2,i3,kd)=(wsbx23r(i1,i2,i3+1,kd)-wsbx23r(i1,i2,i3-1,
     & kd))*h12(2)
      wsbyz23r(i1,i2,i3,kd)=(wsby23r(i1,i2,i3+1,kd)-wsby23r(i1,i2,i3-1,
     & kd))*h12(2)
      wsbx21r(i1,i2,i3,kd)= wsbx23r(i1,i2,i3,kd)
      wsby21r(i1,i2,i3,kd)= wsby23r(i1,i2,i3,kd)
      wsbz21r(i1,i2,i3,kd)= wsbz23r(i1,i2,i3,kd)
      wsbxx21r(i1,i2,i3,kd)= wsbxx23r(i1,i2,i3,kd)
      wsbyy21r(i1,i2,i3,kd)= wsbyy23r(i1,i2,i3,kd)
      wsbzz21r(i1,i2,i3,kd)= wsbzz23r(i1,i2,i3,kd)
      wsbxy21r(i1,i2,i3,kd)= wsbxy23r(i1,i2,i3,kd)
      wsbxz21r(i1,i2,i3,kd)= wsbxz23r(i1,i2,i3,kd)
      wsbyz21r(i1,i2,i3,kd)= wsbyz23r(i1,i2,i3,kd)
      wsblaplacian21r(i1,i2,i3,kd)=wsbxx23r(i1,i2,i3,kd)
      wsbx22r(i1,i2,i3,kd)= wsbx23r(i1,i2,i3,kd)
      wsby22r(i1,i2,i3,kd)= wsby23r(i1,i2,i3,kd)
      wsbz22r(i1,i2,i3,kd)= wsbz23r(i1,i2,i3,kd)
      wsbxx22r(i1,i2,i3,kd)= wsbxx23r(i1,i2,i3,kd)
      wsbyy22r(i1,i2,i3,kd)= wsbyy23r(i1,i2,i3,kd)
      wsbzz22r(i1,i2,i3,kd)= wsbzz23r(i1,i2,i3,kd)
      wsbxy22r(i1,i2,i3,kd)= wsbxy23r(i1,i2,i3,kd)
      wsbxz22r(i1,i2,i3,kd)= wsbxz23r(i1,i2,i3,kd)
      wsbyz22r(i1,i2,i3,kd)= wsbyz23r(i1,i2,i3,kd)
      wsblaplacian22r(i1,i2,i3,kd)=wsbxx23r(i1,i2,i3,kd)+wsbyy23r(i1,
     & i2,i3,kd)
      wsblaplacian23r(i1,i2,i3,kd)=wsbxx23r(i1,i2,i3,kd)+wsbyy23r(i1,
     & i2,i3,kd)+wsbzz23r(i1,i2,i3,kd)
      wsbxxx22r(i1,i2,i3,kd)=(-2.*(wsb(i1+1,i2,i3,kd)-wsb(i1-1,i2,i3,
     & kd))+(wsb(i1+2,i2,i3,kd)-wsb(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
      wsbyyy22r(i1,i2,i3,kd)=(-2.*(wsb(i1,i2+1,i3,kd)-wsb(i1,i2-1,i3,
     & kd))+(wsb(i1,i2+2,i3,kd)-wsb(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
      wsbxxy22r(i1,i2,i3,kd)=( wsbxx22r(i1,i2+1,i3,kd)-wsbxx22r(i1,i2-
     & 1,i3,kd))/(2.*dx(1))
      wsbxyy22r(i1,i2,i3,kd)=( wsbyy22r(i1+1,i2,i3,kd)-wsbyy22r(i1-1,
     & i2,i3,kd))/(2.*dx(0))
      wsbxxxx22r(i1,i2,i3,kd)=(6.*wsb(i1,i2,i3,kd)-4.*(wsb(i1+1,i2,i3,
     & kd)+wsb(i1-1,i2,i3,kd))+(wsb(i1+2,i2,i3,kd)+wsb(i1-2,i2,i3,kd))
     &  )/(dx(0)**4)
      wsbyyyy22r(i1,i2,i3,kd)=(6.*wsb(i1,i2,i3,kd)-4.*(wsb(i1,i2+1,i3,
     & kd)+wsb(i1,i2-1,i3,kd))+(wsb(i1,i2+2,i3,kd)+wsb(i1,i2-2,i3,kd))
     &  )/(dx(1)**4)
      wsbxxyy22r(i1,i2,i3,kd)=( 4.*wsb(i1,i2,i3,kd)     -2.*(wsb(i1+1,
     & i2,i3,kd)+wsb(i1-1,i2,i3,kd)+wsb(i1,i2+1,i3,kd)+wsb(i1,i2-1,i3,
     & kd))   +   (wsb(i1+1,i2+1,i3,kd)+wsb(i1-1,i2+1,i3,kd)+wsb(i1+1,
     & i2-1,i3,kd)+wsb(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      ! 2D laplacian squared = wsb.xxxx + 2 wsb.xxyy + wsb.yyyy
      wsbLapSq22r(i1,i2,i3,kd)= ( 6.*wsb(i1,i2,i3,kd)   - 4.*(wsb(i1+1,
     & i2,i3,kd)+wsb(i1-1,i2,i3,kd))    +(wsb(i1+2,i2,i3,kd)+wsb(i1-2,
     & i2,i3,kd)) )/(dx(0)**4) +( 6.*wsb(i1,i2,i3,kd)    -4.*(wsb(i1,
     & i2+1,i3,kd)+wsb(i1,i2-1,i3,kd))    +(wsb(i1,i2+2,i3,kd)+wsb(i1,
     & i2-2,i3,kd)) )/(dx(1)**4)  +( 8.*wsb(i1,i2,i3,kd)     -4.*(wsb(
     & i1+1,i2,i3,kd)+wsb(i1-1,i2,i3,kd)+wsb(i1,i2+1,i3,kd)+wsb(i1,i2-
     & 1,i3,kd))   +2.*(wsb(i1+1,i2+1,i3,kd)+wsb(i1-1,i2+1,i3,kd)+wsb(
     & i1+1,i2-1,i3,kd)+wsb(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      wsbxxx23r(i1,i2,i3,kd)=(-2.*(wsb(i1+1,i2,i3,kd)-wsb(i1-1,i2,i3,
     & kd))+(wsb(i1+2,i2,i3,kd)-wsb(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
      wsbyyy23r(i1,i2,i3,kd)=(-2.*(wsb(i1,i2+1,i3,kd)-wsb(i1,i2-1,i3,
     & kd))+(wsb(i1,i2+2,i3,kd)-wsb(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
      wsbzzz23r(i1,i2,i3,kd)=(-2.*(wsb(i1,i2,i3+1,kd)-wsb(i1,i2,i3-1,
     & kd))+(wsb(i1,i2,i3+2,kd)-wsb(i1,i2,i3-2,kd)) )*h22(1)*h12(2)
      wsbxxy23r(i1,i2,i3,kd)=( wsbxx22r(i1,i2+1,i3,kd)-wsbxx22r(i1,i2-
     & 1,i3,kd))/(2.*dx(1))
      wsbxyy23r(i1,i2,i3,kd)=( wsbyy22r(i1+1,i2,i3,kd)-wsbyy22r(i1-1,
     & i2,i3,kd))/(2.*dx(0))
      wsbxxz23r(i1,i2,i3,kd)=( wsbxx22r(i1,i2,i3+1,kd)-wsbxx22r(i1,i2,
     & i3-1,kd))/(2.*dx(2))
      wsbyyz23r(i1,i2,i3,kd)=( wsbyy22r(i1,i2,i3+1,kd)-wsbyy22r(i1,i2,
     & i3-1,kd))/(2.*dx(2))
      wsbxzz23r(i1,i2,i3,kd)=( wsbzz22r(i1+1,i2,i3,kd)-wsbzz22r(i1-1,
     & i2,i3,kd))/(2.*dx(0))
      wsbyzz23r(i1,i2,i3,kd)=( wsbzz22r(i1,i2+1,i3,kd)-wsbzz22r(i1,i2-
     & 1,i3,kd))/(2.*dx(1))
      wsbxxxx23r(i1,i2,i3,kd)=(6.*wsb(i1,i2,i3,kd)-4.*(wsb(i1+1,i2,i3,
     & kd)+wsb(i1-1,i2,i3,kd))+(wsb(i1+2,i2,i3,kd)+wsb(i1-2,i2,i3,kd))
     &  )/(dx(0)**4)
      wsbyyyy23r(i1,i2,i3,kd)=(6.*wsb(i1,i2,i3,kd)-4.*(wsb(i1,i2+1,i3,
     & kd)+wsb(i1,i2-1,i3,kd))+(wsb(i1,i2+2,i3,kd)+wsb(i1,i2-2,i3,kd))
     &  )/(dx(1)**4)
      wsbzzzz23r(i1,i2,i3,kd)=(6.*wsb(i1,i2,i3,kd)-4.*(wsb(i1,i2,i3+1,
     & kd)+wsb(i1,i2,i3-1,kd))+(wsb(i1,i2,i3+2,kd)+wsb(i1,i2,i3-2,kd))
     &  )/(dx(2)**4)
      wsbxxyy23r(i1,i2,i3,kd)=( 4.*wsb(i1,i2,i3,kd)     -2.*(wsb(i1+1,
     & i2,i3,kd)+wsb(i1-1,i2,i3,kd)+wsb(i1,i2+1,i3,kd)+wsb(i1,i2-1,i3,
     & kd))   +   (wsb(i1+1,i2+1,i3,kd)+wsb(i1-1,i2+1,i3,kd)+wsb(i1+1,
     & i2-1,i3,kd)+wsb(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      wsbxxzz23r(i1,i2,i3,kd)=( 4.*wsb(i1,i2,i3,kd)     -2.*(wsb(i1+1,
     & i2,i3,kd)+wsb(i1-1,i2,i3,kd)+wsb(i1,i2,i3+1,kd)+wsb(i1,i2,i3-1,
     & kd))   +   (wsb(i1+1,i2,i3+1,kd)+wsb(i1-1,i2,i3+1,kd)+wsb(i1+1,
     & i2,i3-1,kd)+wsb(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)
      wsbyyzz23r(i1,i2,i3,kd)=( 4.*wsb(i1,i2,i3,kd)     -2.*(wsb(i1,i2+
     & 1,i3,kd)  +wsb(i1,i2-1,i3,kd)+  wsb(i1,i2  ,i3+1,kd)+wsb(i1,i2 
     &  ,i3-1,kd))   +   (wsb(i1,i2+1,i3+1,kd)+wsb(i1,i2-1,i3+1,kd)+
     & wsb(i1,i2+1,i3-1,kd)+wsb(i1,i2-1,i3-1,kd)) )/(dx(1)**2*dx(2)**
     & 2)
      ! 3D laplacian squared = wsb.xxxx + wsb.yyyy + wsb.zzzz + 2 (wsb.xxyy + wsb.xxzz + wsb.yyzz )
      wsbLapSq23r(i1,i2,i3,kd)= ( 6.*wsb(i1,i2,i3,kd)   - 4.*(wsb(i1+1,
     & i2,i3,kd)+wsb(i1-1,i2,i3,kd))    +(wsb(i1+2,i2,i3,kd)+wsb(i1-2,
     & i2,i3,kd)) )/(dx(0)**4) +( 6.*wsb(i1,i2,i3,kd)    -4.*(wsb(i1,
     & i2+1,i3,kd)+wsb(i1,i2-1,i3,kd))    +(wsb(i1,i2+2,i3,kd)+wsb(i1,
     & i2-2,i3,kd)) )/(dx(1)**4)  +( 6.*wsb(i1,i2,i3,kd)    -4.*(wsb(
     & i1,i2,i3+1,kd)+wsb(i1,i2,i3-1,kd))    +(wsb(i1,i2,i3+2,kd)+wsb(
     & i1,i2,i3-2,kd)) )/(dx(2)**4)  +( 8.*wsb(i1,i2,i3,kd)     -4.*(
     & wsb(i1+1,i2,i3,kd)  +wsb(i1-1,i2,i3,kd)  +wsb(i1  ,i2+1,i3,kd)+
     & wsb(i1  ,i2-1,i3,kd))   +2.*(wsb(i1+1,i2+1,i3,kd)+wsb(i1-1,i2+
     & 1,i3,kd)+wsb(i1+1,i2-1,i3,kd)+wsb(i1-1,i2-1,i3,kd)) )/(dx(0)**
     & 2*dx(1)**2)+( 8.*wsb(i1,i2,i3,kd)     -4.*(wsb(i1+1,i2,i3,kd)  
     & +wsb(i1-1,i2,i3,kd)  +wsb(i1  ,i2,i3+1,kd)+wsb(i1  ,i2,i3-1,kd)
     & )   +2.*(wsb(i1+1,i2,i3+1,kd)+wsb(i1-1,i2,i3+1,kd)+wsb(i1+1,i2,
     & i3-1,kd)+wsb(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)+( 8.*wsb(
     & i1,i2,i3,kd)     -4.*(wsb(i1,i2+1,i3,kd)  +wsb(i1,i2-1,i3,kd)  
     & +wsb(i1,i2  ,i3+1,kd)+wsb(i1,i2  ,i3-1,kd))   +2.*(wsb(i1,i2+1,
     & i3+1,kd)+wsb(i1,i2-1,i3+1,kd)+wsb(i1,i2+1,i3-1,kd)+wsb(i1,i2-1,
     & i3-1,kd)) )/(dx(1)**2*dx(2)**2)
      wsbmr2(i1,i2,i3,kd)=(wsbm(i1+1,i2,i3,kd)-wsbm(i1-1,i2,i3,kd))*
     & d12(0)
      wsbms2(i1,i2,i3,kd)=(wsbm(i1,i2+1,i3,kd)-wsbm(i1,i2-1,i3,kd))*
     & d12(1)
      wsbmt2(i1,i2,i3,kd)=(wsbm(i1,i2,i3+1,kd)-wsbm(i1,i2,i3-1,kd))*
     & d12(2)
      wsbmrr2(i1,i2,i3,kd)=(-2.*wsbm(i1,i2,i3,kd)+(wsbm(i1+1,i2,i3,kd)+
     & wsbm(i1-1,i2,i3,kd)) )*d22(0)
      wsbmss2(i1,i2,i3,kd)=(-2.*wsbm(i1,i2,i3,kd)+(wsbm(i1,i2+1,i3,kd)+
     & wsbm(i1,i2-1,i3,kd)) )*d22(1)
      wsbmrs2(i1,i2,i3,kd)=(wsbmr2(i1,i2+1,i3,kd)-wsbmr2(i1,i2-1,i3,kd)
     & )*d12(1)
      wsbmtt2(i1,i2,i3,kd)=(-2.*wsbm(i1,i2,i3,kd)+(wsbm(i1,i2,i3+1,kd)+
     & wsbm(i1,i2,i3-1,kd)) )*d22(2)
      wsbmrt2(i1,i2,i3,kd)=(wsbmr2(i1,i2,i3+1,kd)-wsbmr2(i1,i2,i3-1,kd)
     & )*d12(2)
      wsbmst2(i1,i2,i3,kd)=(wsbms2(i1,i2,i3+1,kd)-wsbms2(i1,i2,i3-1,kd)
     & )*d12(2)
      wsbmrrr2(i1,i2,i3,kd)=(-2.*(wsbm(i1+1,i2,i3,kd)-wsbm(i1-1,i2,i3,
     & kd))+(wsbm(i1+2,i2,i3,kd)-wsbm(i1-2,i2,i3,kd)) )*d22(0)*d12(0)
      wsbmsss2(i1,i2,i3,kd)=(-2.*(wsbm(i1,i2+1,i3,kd)-wsbm(i1,i2-1,i3,
     & kd))+(wsbm(i1,i2+2,i3,kd)-wsbm(i1,i2-2,i3,kd)) )*d22(1)*d12(1)
      wsbmttt2(i1,i2,i3,kd)=(-2.*(wsbm(i1,i2,i3+1,kd)-wsbm(i1,i2,i3-1,
     & kd))+(wsbm(i1,i2,i3+2,kd)-wsbm(i1,i2,i3-2,kd)) )*d22(2)*d12(2)
      wsbmx21(i1,i2,i3,kd)= rx(i1,i2,i3)*wsbmr2(i1,i2,i3,kd)
      wsbmy21(i1,i2,i3,kd)=0
      wsbmz21(i1,i2,i3,kd)=0
      wsbmx22(i1,i2,i3,kd)= rx(i1,i2,i3)*wsbmr2(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*wsbms2(i1,i2,i3,kd)
      wsbmy22(i1,i2,i3,kd)= ry(i1,i2,i3)*wsbmr2(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*wsbms2(i1,i2,i3,kd)
      wsbmz22(i1,i2,i3,kd)=0
      wsbmx23(i1,i2,i3,kd)=rx(i1,i2,i3)*wsbmr2(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*wsbms2(i1,i2,i3,kd)+tx(i1,i2,i3)*wsbmt2(i1,i2,i3,kd)
      wsbmy23(i1,i2,i3,kd)=ry(i1,i2,i3)*wsbmr2(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*wsbms2(i1,i2,i3,kd)+ty(i1,i2,i3)*wsbmt2(i1,i2,i3,kd)
      wsbmz23(i1,i2,i3,kd)=rz(i1,i2,i3)*wsbmr2(i1,i2,i3,kd)+sz(i1,i2,
     & i3)*wsbms2(i1,i2,i3,kd)+tz(i1,i2,i3)*wsbmt2(i1,i2,i3,kd)
      wsbmxx21(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wsbmrr2(i1,i2,i3,kd)+(
     & rxx22(i1,i2,i3))*wsbmr2(i1,i2,i3,kd)
      wsbmyy21(i1,i2,i3,kd)=0
      wsbmxy21(i1,i2,i3,kd)=0
      wsbmxz21(i1,i2,i3,kd)=0
      wsbmyz21(i1,i2,i3,kd)=0
      wsbmzz21(i1,i2,i3,kd)=0
      wsbmlaplacian21(i1,i2,i3,kd)=wsbmxx21(i1,i2,i3,kd)
      wsbmxx22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wsbmrr2(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*wsbmrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*wsbmss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3))*wsbmr2(i1,i2,i3,kd)+(
     & sxx22(i1,i2,i3))*wsbms2(i1,i2,i3,kd)
      wsbmyy22(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*wsbmrr2(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*wsbmrs2(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*wsbmss2(i1,i2,i3,kd)+(ryy22(i1,i2,i3))*wsbmr2(i1,i2,i3,kd)+(
     & syy22(i1,i2,i3))*wsbms2(i1,i2,i3,kd)
      wsbmxy22(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wsbmrr2(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & wsbmrs2(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wsbmss2(i1,i2,
     & i3,kd)+rxy22(i1,i2,i3)*wsbmr2(i1,i2,i3,kd)+sxy22(i1,i2,i3)*
     & wsbms2(i1,i2,i3,kd)
      wsbmxz22(i1,i2,i3,kd)=0
      wsbmyz22(i1,i2,i3,kd)=0
      wsbmzz22(i1,i2,i3,kd)=0
      wsbmlaplacian22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & wsbmrr2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,
     & i3)*sy(i1,i2,i3))*wsbmrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,
     & i2,i3)**2)*wsbmss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3)+ryy22(i1,i2,
     & i3))*wsbmr2(i1,i2,i3,kd)+(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*
     & wsbms2(i1,i2,i3,kd)
      wsbmxx23(i1,i2,i3,kd)=rx(i1,i2,i3)**2*wsbmrr2(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*wsbmss2(i1,i2,i3,kd)+tx(i1,i2,i3)**2*wsbmtt2(i1,i2,
     & i3,kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*wsbmrs2(i1,i2,i3,kd)+2.*rx(
     & i1,i2,i3)*tx(i1,i2,i3)*wsbmrt2(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(
     & i1,i2,i3)*wsbmst2(i1,i2,i3,kd)+rxx23(i1,i2,i3)*wsbmr2(i1,i2,i3,
     & kd)+sxx23(i1,i2,i3)*wsbms2(i1,i2,i3,kd)+txx23(i1,i2,i3)*wsbmt2(
     & i1,i2,i3,kd)
      wsbmyy23(i1,i2,i3,kd)=ry(i1,i2,i3)**2*wsbmrr2(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*wsbmss2(i1,i2,i3,kd)+ty(i1,i2,i3)**2*wsbmtt2(i1,i2,
     & i3,kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*wsbmrs2(i1,i2,i3,kd)+2.*ry(
     & i1,i2,i3)*ty(i1,i2,i3)*wsbmrt2(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(
     & i1,i2,i3)*wsbmst2(i1,i2,i3,kd)+ryy23(i1,i2,i3)*wsbmr2(i1,i2,i3,
     & kd)+syy23(i1,i2,i3)*wsbms2(i1,i2,i3,kd)+tyy23(i1,i2,i3)*wsbmt2(
     & i1,i2,i3,kd)
      wsbmzz23(i1,i2,i3,kd)=rz(i1,i2,i3)**2*wsbmrr2(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*wsbmss2(i1,i2,i3,kd)+tz(i1,i2,i3)**2*wsbmtt2(i1,i2,
     & i3,kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*wsbmrs2(i1,i2,i3,kd)+2.*rz(
     & i1,i2,i3)*tz(i1,i2,i3)*wsbmrt2(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(
     & i1,i2,i3)*wsbmst2(i1,i2,i3,kd)+rzz23(i1,i2,i3)*wsbmr2(i1,i2,i3,
     & kd)+szz23(i1,i2,i3)*wsbms2(i1,i2,i3,kd)+tzz23(i1,i2,i3)*wsbmt2(
     & i1,i2,i3,kd)
      wsbmxy23(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wsbmrr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wsbmss2(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *ty(i1,i2,i3)*wsbmtt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+
     & ry(i1,i2,i3)*sx(i1,i2,i3))*wsbmrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & ty(i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*wsbmrt2(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*wsbmst2(
     & i1,i2,i3,kd)+rxy23(i1,i2,i3)*wsbmr2(i1,i2,i3,kd)+sxy23(i1,i2,
     & i3)*wsbms2(i1,i2,i3,kd)+txy23(i1,i2,i3)*wsbmt2(i1,i2,i3,kd)
      wsbmxz23(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*wsbmrr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*wsbmss2(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *tz(i1,i2,i3)*wsbmtt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sx(i1,i2,i3))*wsbmrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*wsbmrt2(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*wsbmst2(
     & i1,i2,i3,kd)+rxz23(i1,i2,i3)*wsbmr2(i1,i2,i3,kd)+sxz23(i1,i2,
     & i3)*wsbms2(i1,i2,i3,kd)+txz23(i1,i2,i3)*wsbmt2(i1,i2,i3,kd)
      wsbmyz23(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*wsbmrr2(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*wsbmss2(i1,i2,i3,kd)+ty(i1,i2,i3)
     & *tz(i1,i2,i3)*wsbmtt2(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sy(i1,i2,i3))*wsbmrs2(i1,i2,i3,kd)+(ry(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*wsbmrt2(i1,i2,i3,kd)+(
     & sy(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*wsbmst2(
     & i1,i2,i3,kd)+ryz23(i1,i2,i3)*wsbmr2(i1,i2,i3,kd)+syz23(i1,i2,
     & i3)*wsbms2(i1,i2,i3,kd)+tyz23(i1,i2,i3)*wsbmt2(i1,i2,i3,kd)
      wsbmlaplacian23(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*wsbmrr2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2+sz(i1,i2,i3)**2)*wsbmss2(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+
     & ty(i1,i2,i3)**2+tz(i1,i2,i3)**2)*wsbmtt2(i1,i2,i3,kd)+2.*(rx(
     & i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*
     & sz(i1,i2,i3))*wsbmrs2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,
     & i3)+ ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*
     & wsbmrt2(i1,i2,i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,
     & i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3))*wsbmst2(i1,i2,i3,
     & kd)+(rxx23(i1,i2,i3)+ryy23(i1,i2,i3)+rzz23(i1,i2,i3))*wsbmr2(
     & i1,i2,i3,kd)+(sxx23(i1,i2,i3)+syy23(i1,i2,i3)+szz23(i1,i2,i3))*
     & wsbms2(i1,i2,i3,kd)+(txx23(i1,i2,i3)+tyy23(i1,i2,i3)+tzz23(i1,
     & i2,i3))*wsbmt2(i1,i2,i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      wsbmx23r(i1,i2,i3,kd)=(wsbm(i1+1,i2,i3,kd)-wsbm(i1-1,i2,i3,kd))*
     & h12(0)
      wsbmy23r(i1,i2,i3,kd)=(wsbm(i1,i2+1,i3,kd)-wsbm(i1,i2-1,i3,kd))*
     & h12(1)
      wsbmz23r(i1,i2,i3,kd)=(wsbm(i1,i2,i3+1,kd)-wsbm(i1,i2,i3-1,kd))*
     & h12(2)
      wsbmxx23r(i1,i2,i3,kd)=(-2.*wsbm(i1,i2,i3,kd)+(wsbm(i1+1,i2,i3,
     & kd)+wsbm(i1-1,i2,i3,kd)) )*h22(0)
      wsbmyy23r(i1,i2,i3,kd)=(-2.*wsbm(i1,i2,i3,kd)+(wsbm(i1,i2+1,i3,
     & kd)+wsbm(i1,i2-1,i3,kd)) )*h22(1)
      wsbmxy23r(i1,i2,i3,kd)=(wsbmx23r(i1,i2+1,i3,kd)-wsbmx23r(i1,i2-1,
     & i3,kd))*h12(1)
      wsbmzz23r(i1,i2,i3,kd)=(-2.*wsbm(i1,i2,i3,kd)+(wsbm(i1,i2,i3+1,
     & kd)+wsbm(i1,i2,i3-1,kd)) )*h22(2)
      wsbmxz23r(i1,i2,i3,kd)=(wsbmx23r(i1,i2,i3+1,kd)-wsbmx23r(i1,i2,
     & i3-1,kd))*h12(2)
      wsbmyz23r(i1,i2,i3,kd)=(wsbmy23r(i1,i2,i3+1,kd)-wsbmy23r(i1,i2,
     & i3-1,kd))*h12(2)
      wsbmx21r(i1,i2,i3,kd)= wsbmx23r(i1,i2,i3,kd)
      wsbmy21r(i1,i2,i3,kd)= wsbmy23r(i1,i2,i3,kd)
      wsbmz21r(i1,i2,i3,kd)= wsbmz23r(i1,i2,i3,kd)
      wsbmxx21r(i1,i2,i3,kd)= wsbmxx23r(i1,i2,i3,kd)
      wsbmyy21r(i1,i2,i3,kd)= wsbmyy23r(i1,i2,i3,kd)
      wsbmzz21r(i1,i2,i3,kd)= wsbmzz23r(i1,i2,i3,kd)
      wsbmxy21r(i1,i2,i3,kd)= wsbmxy23r(i1,i2,i3,kd)
      wsbmxz21r(i1,i2,i3,kd)= wsbmxz23r(i1,i2,i3,kd)
      wsbmyz21r(i1,i2,i3,kd)= wsbmyz23r(i1,i2,i3,kd)
      wsbmlaplacian21r(i1,i2,i3,kd)=wsbmxx23r(i1,i2,i3,kd)
      wsbmx22r(i1,i2,i3,kd)= wsbmx23r(i1,i2,i3,kd)
      wsbmy22r(i1,i2,i3,kd)= wsbmy23r(i1,i2,i3,kd)
      wsbmz22r(i1,i2,i3,kd)= wsbmz23r(i1,i2,i3,kd)
      wsbmxx22r(i1,i2,i3,kd)= wsbmxx23r(i1,i2,i3,kd)
      wsbmyy22r(i1,i2,i3,kd)= wsbmyy23r(i1,i2,i3,kd)
      wsbmzz22r(i1,i2,i3,kd)= wsbmzz23r(i1,i2,i3,kd)
      wsbmxy22r(i1,i2,i3,kd)= wsbmxy23r(i1,i2,i3,kd)
      wsbmxz22r(i1,i2,i3,kd)= wsbmxz23r(i1,i2,i3,kd)
      wsbmyz22r(i1,i2,i3,kd)= wsbmyz23r(i1,i2,i3,kd)
      wsbmlaplacian22r(i1,i2,i3,kd)=wsbmxx23r(i1,i2,i3,kd)+wsbmyy23r(
     & i1,i2,i3,kd)
      wsbmlaplacian23r(i1,i2,i3,kd)=wsbmxx23r(i1,i2,i3,kd)+wsbmyy23r(
     & i1,i2,i3,kd)+wsbmzz23r(i1,i2,i3,kd)
      wsbmxxx22r(i1,i2,i3,kd)=(-2.*(wsbm(i1+1,i2,i3,kd)-wsbm(i1-1,i2,
     & i3,kd))+(wsbm(i1+2,i2,i3,kd)-wsbm(i1-2,i2,i3,kd)) )*h22(0)*h12(
     & 0)
      wsbmyyy22r(i1,i2,i3,kd)=(-2.*(wsbm(i1,i2+1,i3,kd)-wsbm(i1,i2-1,
     & i3,kd))+(wsbm(i1,i2+2,i3,kd)-wsbm(i1,i2-2,i3,kd)) )*h22(1)*h12(
     & 1)
      wsbmxxy22r(i1,i2,i3,kd)=( wsbmxx22r(i1,i2+1,i3,kd)-wsbmxx22r(i1,
     & i2-1,i3,kd))/(2.*dx(1))
      wsbmxyy22r(i1,i2,i3,kd)=( wsbmyy22r(i1+1,i2,i3,kd)-wsbmyy22r(i1-
     & 1,i2,i3,kd))/(2.*dx(0))
      wsbmxxxx22r(i1,i2,i3,kd)=(6.*wsbm(i1,i2,i3,kd)-4.*(wsbm(i1+1,i2,
     & i3,kd)+wsbm(i1-1,i2,i3,kd))+(wsbm(i1+2,i2,i3,kd)+wsbm(i1-2,i2,
     & i3,kd)) )/(dx(0)**4)
      wsbmyyyy22r(i1,i2,i3,kd)=(6.*wsbm(i1,i2,i3,kd)-4.*(wsbm(i1,i2+1,
     & i3,kd)+wsbm(i1,i2-1,i3,kd))+(wsbm(i1,i2+2,i3,kd)+wsbm(i1,i2-2,
     & i3,kd)) )/(dx(1)**4)
      wsbmxxyy22r(i1,i2,i3,kd)=( 4.*wsbm(i1,i2,i3,kd)     -2.*(wsbm(i1+
     & 1,i2,i3,kd)+wsbm(i1-1,i2,i3,kd)+wsbm(i1,i2+1,i3,kd)+wsbm(i1,i2-
     & 1,i3,kd))   +   (wsbm(i1+1,i2+1,i3,kd)+wsbm(i1-1,i2+1,i3,kd)+
     & wsbm(i1+1,i2-1,i3,kd)+wsbm(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)*
     & *2)
      ! 2D laplacian squared = wsbm.xxxx + 2 wsbm.xxyy + wsbm.yyyy
      wsbmLapSq22r(i1,i2,i3,kd)= ( 6.*wsbm(i1,i2,i3,kd)   - 4.*(wsbm(
     & i1+1,i2,i3,kd)+wsbm(i1-1,i2,i3,kd))    +(wsbm(i1+2,i2,i3,kd)+
     & wsbm(i1-2,i2,i3,kd)) )/(dx(0)**4) +( 6.*wsbm(i1,i2,i3,kd)    -
     & 4.*(wsbm(i1,i2+1,i3,kd)+wsbm(i1,i2-1,i3,kd))    +(wsbm(i1,i2+2,
     & i3,kd)+wsbm(i1,i2-2,i3,kd)) )/(dx(1)**4)  +( 8.*wsbm(i1,i2,i3,
     & kd)     -4.*(wsbm(i1+1,i2,i3,kd)+wsbm(i1-1,i2,i3,kd)+wsbm(i1,
     & i2+1,i3,kd)+wsbm(i1,i2-1,i3,kd))   +2.*(wsbm(i1+1,i2+1,i3,kd)+
     & wsbm(i1-1,i2+1,i3,kd)+wsbm(i1+1,i2-1,i3,kd)+wsbm(i1-1,i2-1,i3,
     & kd)) )/(dx(0)**2*dx(1)**2)
      wsbmxxx23r(i1,i2,i3,kd)=(-2.*(wsbm(i1+1,i2,i3,kd)-wsbm(i1-1,i2,
     & i3,kd))+(wsbm(i1+2,i2,i3,kd)-wsbm(i1-2,i2,i3,kd)) )*h22(0)*h12(
     & 0)
      wsbmyyy23r(i1,i2,i3,kd)=(-2.*(wsbm(i1,i2+1,i3,kd)-wsbm(i1,i2-1,
     & i3,kd))+(wsbm(i1,i2+2,i3,kd)-wsbm(i1,i2-2,i3,kd)) )*h22(1)*h12(
     & 1)
      wsbmzzz23r(i1,i2,i3,kd)=(-2.*(wsbm(i1,i2,i3+1,kd)-wsbm(i1,i2,i3-
     & 1,kd))+(wsbm(i1,i2,i3+2,kd)-wsbm(i1,i2,i3-2,kd)) )*h22(1)*h12(
     & 2)
      wsbmxxy23r(i1,i2,i3,kd)=( wsbmxx22r(i1,i2+1,i3,kd)-wsbmxx22r(i1,
     & i2-1,i3,kd))/(2.*dx(1))
      wsbmxyy23r(i1,i2,i3,kd)=( wsbmyy22r(i1+1,i2,i3,kd)-wsbmyy22r(i1-
     & 1,i2,i3,kd))/(2.*dx(0))
      wsbmxxz23r(i1,i2,i3,kd)=( wsbmxx22r(i1,i2,i3+1,kd)-wsbmxx22r(i1,
     & i2,i3-1,kd))/(2.*dx(2))
      wsbmyyz23r(i1,i2,i3,kd)=( wsbmyy22r(i1,i2,i3+1,kd)-wsbmyy22r(i1,
     & i2,i3-1,kd))/(2.*dx(2))
      wsbmxzz23r(i1,i2,i3,kd)=( wsbmzz22r(i1+1,i2,i3,kd)-wsbmzz22r(i1-
     & 1,i2,i3,kd))/(2.*dx(0))
      wsbmyzz23r(i1,i2,i3,kd)=( wsbmzz22r(i1,i2+1,i3,kd)-wsbmzz22r(i1,
     & i2-1,i3,kd))/(2.*dx(1))
      wsbmxxxx23r(i1,i2,i3,kd)=(6.*wsbm(i1,i2,i3,kd)-4.*(wsbm(i1+1,i2,
     & i3,kd)+wsbm(i1-1,i2,i3,kd))+(wsbm(i1+2,i2,i3,kd)+wsbm(i1-2,i2,
     & i3,kd)) )/(dx(0)**4)
      wsbmyyyy23r(i1,i2,i3,kd)=(6.*wsbm(i1,i2,i3,kd)-4.*(wsbm(i1,i2+1,
     & i3,kd)+wsbm(i1,i2-1,i3,kd))+(wsbm(i1,i2+2,i3,kd)+wsbm(i1,i2-2,
     & i3,kd)) )/(dx(1)**4)
      wsbmzzzz23r(i1,i2,i3,kd)=(6.*wsbm(i1,i2,i3,kd)-4.*(wsbm(i1,i2,i3+
     & 1,kd)+wsbm(i1,i2,i3-1,kd))+(wsbm(i1,i2,i3+2,kd)+wsbm(i1,i2,i3-
     & 2,kd)) )/(dx(2)**4)
      wsbmxxyy23r(i1,i2,i3,kd)=( 4.*wsbm(i1,i2,i3,kd)     -2.*(wsbm(i1+
     & 1,i2,i3,kd)+wsbm(i1-1,i2,i3,kd)+wsbm(i1,i2+1,i3,kd)+wsbm(i1,i2-
     & 1,i3,kd))   +   (wsbm(i1+1,i2+1,i3,kd)+wsbm(i1-1,i2+1,i3,kd)+
     & wsbm(i1+1,i2-1,i3,kd)+wsbm(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)*
     & *2)
      wsbmxxzz23r(i1,i2,i3,kd)=( 4.*wsbm(i1,i2,i3,kd)     -2.*(wsbm(i1+
     & 1,i2,i3,kd)+wsbm(i1-1,i2,i3,kd)+wsbm(i1,i2,i3+1,kd)+wsbm(i1,i2,
     & i3-1,kd))   +   (wsbm(i1+1,i2,i3+1,kd)+wsbm(i1-1,i2,i3+1,kd)+
     & wsbm(i1+1,i2,i3-1,kd)+wsbm(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)*
     & *2)
      wsbmyyzz23r(i1,i2,i3,kd)=( 4.*wsbm(i1,i2,i3,kd)     -2.*(wsbm(i1,
     & i2+1,i3,kd)  +wsbm(i1,i2-1,i3,kd)+  wsbm(i1,i2  ,i3+1,kd)+wsbm(
     & i1,i2  ,i3-1,kd))   +   (wsbm(i1,i2+1,i3+1,kd)+wsbm(i1,i2-1,i3+
     & 1,kd)+wsbm(i1,i2+1,i3-1,kd)+wsbm(i1,i2-1,i3-1,kd)) )/(dx(1)**2*
     & dx(2)**2)
      ! 3D laplacian squared = wsbm.xxxx + wsbm.yyyy + wsbm.zzzz + 2 (wsbm.xxyy + wsbm.xxzz + wsbm.yyzz )
      wsbmLapSq23r(i1,i2,i3,kd)= ( 6.*wsbm(i1,i2,i3,kd)   - 4.*(wsbm(
     & i1+1,i2,i3,kd)+wsbm(i1-1,i2,i3,kd))    +(wsbm(i1+2,i2,i3,kd)+
     & wsbm(i1-2,i2,i3,kd)) )/(dx(0)**4) +( 6.*wsbm(i1,i2,i3,kd)    -
     & 4.*(wsbm(i1,i2+1,i3,kd)+wsbm(i1,i2-1,i3,kd))    +(wsbm(i1,i2+2,
     & i3,kd)+wsbm(i1,i2-2,i3,kd)) )/(dx(1)**4)  +( 6.*wsbm(i1,i2,i3,
     & kd)    -4.*(wsbm(i1,i2,i3+1,kd)+wsbm(i1,i2,i3-1,kd))    +(wsbm(
     & i1,i2,i3+2,kd)+wsbm(i1,i2,i3-2,kd)) )/(dx(2)**4)  +( 8.*wsbm(
     & i1,i2,i3,kd)     -4.*(wsbm(i1+1,i2,i3,kd)  +wsbm(i1-1,i2,i3,kd)
     &   +wsbm(i1  ,i2+1,i3,kd)+wsbm(i1  ,i2-1,i3,kd))   +2.*(wsbm(i1+
     & 1,i2+1,i3,kd)+wsbm(i1-1,i2+1,i3,kd)+wsbm(i1+1,i2-1,i3,kd)+wsbm(
     & i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)+( 8.*wsbm(i1,i2,i3,kd) 
     &     -4.*(wsbm(i1+1,i2,i3,kd)  +wsbm(i1-1,i2,i3,kd)  +wsbm(i1  ,
     & i2,i3+1,kd)+wsbm(i1  ,i2,i3-1,kd))   +2.*(wsbm(i1+1,i2,i3+1,kd)
     & +wsbm(i1-1,i2,i3+1,kd)+wsbm(i1+1,i2,i3-1,kd)+wsbm(i1-1,i2,i3-1,
     & kd)) )/(dx(0)**2*dx(2)**2)+( 8.*wsbm(i1,i2,i3,kd)     -4.*(
     & wsbm(i1,i2+1,i3,kd)  +wsbm(i1,i2-1,i3,kd)  +wsbm(i1,i2  ,i3+1,
     & kd)+wsbm(i1,i2  ,i3-1,kd))   +2.*(wsbm(i1,i2+1,i3+1,kd)+wsbm(
     & i1,i2-1,i3+1,kd)+wsbm(i1,i2+1,i3-1,kd)+wsbm(i1,i2-1,i3-1,kd)) )
     & /(dx(1)**2*dx(2)**2)

      vtar2(i1,i2,i3,kd)=(vta(i1+1,i2,i3,kd)-vta(i1-1,i2,i3,kd))*d12(0)
      vtas2(i1,i2,i3,kd)=(vta(i1,i2+1,i3,kd)-vta(i1,i2-1,i3,kd))*d12(1)
      vtat2(i1,i2,i3,kd)=(vta(i1,i2,i3+1,kd)-vta(i1,i2,i3-1,kd))*d12(2)
      vtarr2(i1,i2,i3,kd)=(-2.*vta(i1,i2,i3,kd)+(vta(i1+1,i2,i3,kd)+
     & vta(i1-1,i2,i3,kd)) )*d22(0)
      vtass2(i1,i2,i3,kd)=(-2.*vta(i1,i2,i3,kd)+(vta(i1,i2+1,i3,kd)+
     & vta(i1,i2-1,i3,kd)) )*d22(1)
      vtars2(i1,i2,i3,kd)=(vtar2(i1,i2+1,i3,kd)-vtar2(i1,i2-1,i3,kd))*
     & d12(1)
      vtatt2(i1,i2,i3,kd)=(-2.*vta(i1,i2,i3,kd)+(vta(i1,i2,i3+1,kd)+
     & vta(i1,i2,i3-1,kd)) )*d22(2)
      vtart2(i1,i2,i3,kd)=(vtar2(i1,i2,i3+1,kd)-vtar2(i1,i2,i3-1,kd))*
     & d12(2)
      vtast2(i1,i2,i3,kd)=(vtas2(i1,i2,i3+1,kd)-vtas2(i1,i2,i3-1,kd))*
     & d12(2)
      vtarrr2(i1,i2,i3,kd)=(-2.*(vta(i1+1,i2,i3,kd)-vta(i1-1,i2,i3,kd))
     & +(vta(i1+2,i2,i3,kd)-vta(i1-2,i2,i3,kd)) )*d22(0)*d12(0)
      vtasss2(i1,i2,i3,kd)=(-2.*(vta(i1,i2+1,i3,kd)-vta(i1,i2-1,i3,kd))
     & +(vta(i1,i2+2,i3,kd)-vta(i1,i2-2,i3,kd)) )*d22(1)*d12(1)
      vtattt2(i1,i2,i3,kd)=(-2.*(vta(i1,i2,i3+1,kd)-vta(i1,i2,i3-1,kd))
     & +(vta(i1,i2,i3+2,kd)-vta(i1,i2,i3-2,kd)) )*d22(2)*d12(2)
      vtax21(i1,i2,i3,kd)= rx(i1,i2,i3)*vtar2(i1,i2,i3,kd)
      vtay21(i1,i2,i3,kd)=0
      vtaz21(i1,i2,i3,kd)=0
      vtax22(i1,i2,i3,kd)= rx(i1,i2,i3)*vtar2(i1,i2,i3,kd)+sx(i1,i2,i3)
     & *vtas2(i1,i2,i3,kd)
      vtay22(i1,i2,i3,kd)= ry(i1,i2,i3)*vtar2(i1,i2,i3,kd)+sy(i1,i2,i3)
     & *vtas2(i1,i2,i3,kd)
      vtaz22(i1,i2,i3,kd)=0
      vtax23(i1,i2,i3,kd)=rx(i1,i2,i3)*vtar2(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & vtas2(i1,i2,i3,kd)+tx(i1,i2,i3)*vtat2(i1,i2,i3,kd)
      vtay23(i1,i2,i3,kd)=ry(i1,i2,i3)*vtar2(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & vtas2(i1,i2,i3,kd)+ty(i1,i2,i3)*vtat2(i1,i2,i3,kd)
      vtaz23(i1,i2,i3,kd)=rz(i1,i2,i3)*vtar2(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & vtas2(i1,i2,i3,kd)+tz(i1,i2,i3)*vtat2(i1,i2,i3,kd)
      vtaxx21(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vtarr2(i1,i2,i3,kd)+(
     & rxx22(i1,i2,i3))*vtar2(i1,i2,i3,kd)
      vtayy21(i1,i2,i3,kd)=0
      vtaxy21(i1,i2,i3,kd)=0
      vtaxz21(i1,i2,i3,kd)=0
      vtayz21(i1,i2,i3,kd)=0
      vtazz21(i1,i2,i3,kd)=0
      vtalaplacian21(i1,i2,i3,kd)=vtaxx21(i1,i2,i3,kd)
      vtaxx22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vtarr2(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*vtars2(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*vtass2(i1,i2,i3,kd)+(rxx22(i1,i2,i3))*vtar2(i1,i2,i3,kd)+(
     & sxx22(i1,i2,i3))*vtas2(i1,i2,i3,kd)
      vtayy22(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*vtarr2(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*vtars2(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*vtass2(i1,i2,i3,kd)+(ryy22(i1,i2,i3))*vtar2(i1,i2,i3,kd)+(
     & syy22(i1,i2,i3))*vtas2(i1,i2,i3,kd)
      vtaxy22(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vtarr2(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & vtars2(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vtass2(i1,i2,i3,
     & kd)+rxy22(i1,i2,i3)*vtar2(i1,i2,i3,kd)+sxy22(i1,i2,i3)*vtas2(
     & i1,i2,i3,kd)
      vtaxz22(i1,i2,i3,kd)=0
      vtayz22(i1,i2,i3,kd)=0
      vtazz22(i1,i2,i3,kd)=0
      vtalaplacian22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & vtarr2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)
     & *sy(i1,i2,i3))*vtars2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2)*vtass2(i1,i2,i3,kd)+(rxx22(i1,i2,i3)+ryy22(i1,i2,i3))*
     & vtar2(i1,i2,i3,kd)+(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*vtas2(i1,
     & i2,i3,kd)
      vtaxx23(i1,i2,i3,kd)=rx(i1,i2,i3)**2*vtarr2(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*vtass2(i1,i2,i3,kd)+tx(i1,i2,i3)**2*vtatt2(i1,i2,i3,
     & kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*vtars2(i1,i2,i3,kd)+2.*rx(i1,
     & i2,i3)*tx(i1,i2,i3)*vtart2(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,
     & i2,i3)*vtast2(i1,i2,i3,kd)+rxx23(i1,i2,i3)*vtar2(i1,i2,i3,kd)+
     & sxx23(i1,i2,i3)*vtas2(i1,i2,i3,kd)+txx23(i1,i2,i3)*vtat2(i1,i2,
     & i3,kd)
      vtayy23(i1,i2,i3,kd)=ry(i1,i2,i3)**2*vtarr2(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*vtass2(i1,i2,i3,kd)+ty(i1,i2,i3)**2*vtatt2(i1,i2,i3,
     & kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*vtars2(i1,i2,i3,kd)+2.*ry(i1,
     & i2,i3)*ty(i1,i2,i3)*vtart2(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,
     & i2,i3)*vtast2(i1,i2,i3,kd)+ryy23(i1,i2,i3)*vtar2(i1,i2,i3,kd)+
     & syy23(i1,i2,i3)*vtas2(i1,i2,i3,kd)+tyy23(i1,i2,i3)*vtat2(i1,i2,
     & i3,kd)
      vtazz23(i1,i2,i3,kd)=rz(i1,i2,i3)**2*vtarr2(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*vtass2(i1,i2,i3,kd)+tz(i1,i2,i3)**2*vtatt2(i1,i2,i3,
     & kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*vtars2(i1,i2,i3,kd)+2.*rz(i1,
     & i2,i3)*tz(i1,i2,i3)*vtart2(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,
     & i2,i3)*vtast2(i1,i2,i3,kd)+rzz23(i1,i2,i3)*vtar2(i1,i2,i3,kd)+
     & szz23(i1,i2,i3)*vtas2(i1,i2,i3,kd)+tzz23(i1,i2,i3)*vtat2(i1,i2,
     & i3,kd)
      vtaxy23(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vtarr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vtass2(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & ty(i1,i2,i3)*vtatt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(
     & i1,i2,i3)*sx(i1,i2,i3))*vtars2(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(
     & i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*vtart2(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*vtast2(i1,i2,
     & i3,kd)+rxy23(i1,i2,i3)*vtar2(i1,i2,i3,kd)+sxy23(i1,i2,i3)*
     & vtas2(i1,i2,i3,kd)+txy23(i1,i2,i3)*vtat2(i1,i2,i3,kd)
      vtaxz23(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*vtarr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*vtass2(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & tz(i1,i2,i3)*vtatt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sx(i1,i2,i3))*vtars2(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*vtart2(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*vtast2(i1,i2,
     & i3,kd)+rxz23(i1,i2,i3)*vtar2(i1,i2,i3,kd)+sxz23(i1,i2,i3)*
     & vtas2(i1,i2,i3,kd)+txz23(i1,i2,i3)*vtat2(i1,i2,i3,kd)
      vtayz23(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*vtarr2(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*vtass2(i1,i2,i3,kd)+ty(i1,i2,i3)*
     & tz(i1,i2,i3)*vtatt2(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sy(i1,i2,i3))*vtars2(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*vtart2(i1,i2,i3,kd)+(sy(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*vtast2(i1,i2,
     & i3,kd)+ryz23(i1,i2,i3)*vtar2(i1,i2,i3,kd)+syz23(i1,i2,i3)*
     & vtas2(i1,i2,i3,kd)+tyz23(i1,i2,i3)*vtat2(i1,i2,i3,kd)
      vtalaplacian23(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*vtarr2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)
     & **2+sz(i1,i2,i3)**2)*vtass2(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(
     & i1,i2,i3)**2+tz(i1,i2,i3)**2)*vtatt2(i1,i2,i3,kd)+2.*(rx(i1,i2,
     & i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,
     & i2,i3))*vtars2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(
     & i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*vtart2(i1,i2,
     & i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)
     & +sz(i1,i2,i3)*tz(i1,i2,i3))*vtast2(i1,i2,i3,kd)+(rxx23(i1,i2,
     & i3)+ryy23(i1,i2,i3)+rzz23(i1,i2,i3))*vtar2(i1,i2,i3,kd)+(sxx23(
     & i1,i2,i3)+syy23(i1,i2,i3)+szz23(i1,i2,i3))*vtas2(i1,i2,i3,kd)+(
     & txx23(i1,i2,i3)+tyy23(i1,i2,i3)+tzz23(i1,i2,i3))*vtat2(i1,i2,
     & i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      vtax23r(i1,i2,i3,kd)=(vta(i1+1,i2,i3,kd)-vta(i1-1,i2,i3,kd))*h12(
     & 0)
      vtay23r(i1,i2,i3,kd)=(vta(i1,i2+1,i3,kd)-vta(i1,i2-1,i3,kd))*h12(
     & 1)
      vtaz23r(i1,i2,i3,kd)=(vta(i1,i2,i3+1,kd)-vta(i1,i2,i3-1,kd))*h12(
     & 2)
      vtaxx23r(i1,i2,i3,kd)=(-2.*vta(i1,i2,i3,kd)+(vta(i1+1,i2,i3,kd)+
     & vta(i1-1,i2,i3,kd)) )*h22(0)
      vtayy23r(i1,i2,i3,kd)=(-2.*vta(i1,i2,i3,kd)+(vta(i1,i2+1,i3,kd)+
     & vta(i1,i2-1,i3,kd)) )*h22(1)
      vtaxy23r(i1,i2,i3,kd)=(vtax23r(i1,i2+1,i3,kd)-vtax23r(i1,i2-1,i3,
     & kd))*h12(1)
      vtazz23r(i1,i2,i3,kd)=(-2.*vta(i1,i2,i3,kd)+(vta(i1,i2,i3+1,kd)+
     & vta(i1,i2,i3-1,kd)) )*h22(2)
      vtaxz23r(i1,i2,i3,kd)=(vtax23r(i1,i2,i3+1,kd)-vtax23r(i1,i2,i3-1,
     & kd))*h12(2)
      vtayz23r(i1,i2,i3,kd)=(vtay23r(i1,i2,i3+1,kd)-vtay23r(i1,i2,i3-1,
     & kd))*h12(2)
      vtax21r(i1,i2,i3,kd)= vtax23r(i1,i2,i3,kd)
      vtay21r(i1,i2,i3,kd)= vtay23r(i1,i2,i3,kd)
      vtaz21r(i1,i2,i3,kd)= vtaz23r(i1,i2,i3,kd)
      vtaxx21r(i1,i2,i3,kd)= vtaxx23r(i1,i2,i3,kd)
      vtayy21r(i1,i2,i3,kd)= vtayy23r(i1,i2,i3,kd)
      vtazz21r(i1,i2,i3,kd)= vtazz23r(i1,i2,i3,kd)
      vtaxy21r(i1,i2,i3,kd)= vtaxy23r(i1,i2,i3,kd)
      vtaxz21r(i1,i2,i3,kd)= vtaxz23r(i1,i2,i3,kd)
      vtayz21r(i1,i2,i3,kd)= vtayz23r(i1,i2,i3,kd)
      vtalaplacian21r(i1,i2,i3,kd)=vtaxx23r(i1,i2,i3,kd)
      vtax22r(i1,i2,i3,kd)= vtax23r(i1,i2,i3,kd)
      vtay22r(i1,i2,i3,kd)= vtay23r(i1,i2,i3,kd)
      vtaz22r(i1,i2,i3,kd)= vtaz23r(i1,i2,i3,kd)
      vtaxx22r(i1,i2,i3,kd)= vtaxx23r(i1,i2,i3,kd)
      vtayy22r(i1,i2,i3,kd)= vtayy23r(i1,i2,i3,kd)
      vtazz22r(i1,i2,i3,kd)= vtazz23r(i1,i2,i3,kd)
      vtaxy22r(i1,i2,i3,kd)= vtaxy23r(i1,i2,i3,kd)
      vtaxz22r(i1,i2,i3,kd)= vtaxz23r(i1,i2,i3,kd)
      vtayz22r(i1,i2,i3,kd)= vtayz23r(i1,i2,i3,kd)
      vtalaplacian22r(i1,i2,i3,kd)=vtaxx23r(i1,i2,i3,kd)+vtayy23r(i1,
     & i2,i3,kd)
      vtalaplacian23r(i1,i2,i3,kd)=vtaxx23r(i1,i2,i3,kd)+vtayy23r(i1,
     & i2,i3,kd)+vtazz23r(i1,i2,i3,kd)
      vtaxxx22r(i1,i2,i3,kd)=(-2.*(vta(i1+1,i2,i3,kd)-vta(i1-1,i2,i3,
     & kd))+(vta(i1+2,i2,i3,kd)-vta(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
      vtayyy22r(i1,i2,i3,kd)=(-2.*(vta(i1,i2+1,i3,kd)-vta(i1,i2-1,i3,
     & kd))+(vta(i1,i2+2,i3,kd)-vta(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
      vtaxxy22r(i1,i2,i3,kd)=( vtaxx22r(i1,i2+1,i3,kd)-vtaxx22r(i1,i2-
     & 1,i3,kd))/(2.*dx(1))
      vtaxyy22r(i1,i2,i3,kd)=( vtayy22r(i1+1,i2,i3,kd)-vtayy22r(i1-1,
     & i2,i3,kd))/(2.*dx(0))
      vtaxxxx22r(i1,i2,i3,kd)=(6.*vta(i1,i2,i3,kd)-4.*(vta(i1+1,i2,i3,
     & kd)+vta(i1-1,i2,i3,kd))+(vta(i1+2,i2,i3,kd)+vta(i1-2,i2,i3,kd))
     &  )/(dx(0)**4)
      vtayyyy22r(i1,i2,i3,kd)=(6.*vta(i1,i2,i3,kd)-4.*(vta(i1,i2+1,i3,
     & kd)+vta(i1,i2-1,i3,kd))+(vta(i1,i2+2,i3,kd)+vta(i1,i2-2,i3,kd))
     &  )/(dx(1)**4)
      vtaxxyy22r(i1,i2,i3,kd)=( 4.*vta(i1,i2,i3,kd)     -2.*(vta(i1+1,
     & i2,i3,kd)+vta(i1-1,i2,i3,kd)+vta(i1,i2+1,i3,kd)+vta(i1,i2-1,i3,
     & kd))   +   (vta(i1+1,i2+1,i3,kd)+vta(i1-1,i2+1,i3,kd)+vta(i1+1,
     & i2-1,i3,kd)+vta(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      ! 2D laplacian squared = vta.xxxx + 2 vta.xxyy + vta.yyyy
      vtaLapSq22r(i1,i2,i3,kd)= ( 6.*vta(i1,i2,i3,kd)   - 4.*(vta(i1+1,
     & i2,i3,kd)+vta(i1-1,i2,i3,kd))    +(vta(i1+2,i2,i3,kd)+vta(i1-2,
     & i2,i3,kd)) )/(dx(0)**4) +( 6.*vta(i1,i2,i3,kd)    -4.*(vta(i1,
     & i2+1,i3,kd)+vta(i1,i2-1,i3,kd))    +(vta(i1,i2+2,i3,kd)+vta(i1,
     & i2-2,i3,kd)) )/(dx(1)**4)  +( 8.*vta(i1,i2,i3,kd)     -4.*(vta(
     & i1+1,i2,i3,kd)+vta(i1-1,i2,i3,kd)+vta(i1,i2+1,i3,kd)+vta(i1,i2-
     & 1,i3,kd))   +2.*(vta(i1+1,i2+1,i3,kd)+vta(i1-1,i2+1,i3,kd)+vta(
     & i1+1,i2-1,i3,kd)+vta(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      vtaxxx23r(i1,i2,i3,kd)=(-2.*(vta(i1+1,i2,i3,kd)-vta(i1-1,i2,i3,
     & kd))+(vta(i1+2,i2,i3,kd)-vta(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
      vtayyy23r(i1,i2,i3,kd)=(-2.*(vta(i1,i2+1,i3,kd)-vta(i1,i2-1,i3,
     & kd))+(vta(i1,i2+2,i3,kd)-vta(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
      vtazzz23r(i1,i2,i3,kd)=(-2.*(vta(i1,i2,i3+1,kd)-vta(i1,i2,i3-1,
     & kd))+(vta(i1,i2,i3+2,kd)-vta(i1,i2,i3-2,kd)) )*h22(1)*h12(2)
      vtaxxy23r(i1,i2,i3,kd)=( vtaxx22r(i1,i2+1,i3,kd)-vtaxx22r(i1,i2-
     & 1,i3,kd))/(2.*dx(1))
      vtaxyy23r(i1,i2,i3,kd)=( vtayy22r(i1+1,i2,i3,kd)-vtayy22r(i1-1,
     & i2,i3,kd))/(2.*dx(0))
      vtaxxz23r(i1,i2,i3,kd)=( vtaxx22r(i1,i2,i3+1,kd)-vtaxx22r(i1,i2,
     & i3-1,kd))/(2.*dx(2))
      vtayyz23r(i1,i2,i3,kd)=( vtayy22r(i1,i2,i3+1,kd)-vtayy22r(i1,i2,
     & i3-1,kd))/(2.*dx(2))
      vtaxzz23r(i1,i2,i3,kd)=( vtazz22r(i1+1,i2,i3,kd)-vtazz22r(i1-1,
     & i2,i3,kd))/(2.*dx(0))
      vtayzz23r(i1,i2,i3,kd)=( vtazz22r(i1,i2+1,i3,kd)-vtazz22r(i1,i2-
     & 1,i3,kd))/(2.*dx(1))
      vtaxxxx23r(i1,i2,i3,kd)=(6.*vta(i1,i2,i3,kd)-4.*(vta(i1+1,i2,i3,
     & kd)+vta(i1-1,i2,i3,kd))+(vta(i1+2,i2,i3,kd)+vta(i1-2,i2,i3,kd))
     &  )/(dx(0)**4)
      vtayyyy23r(i1,i2,i3,kd)=(6.*vta(i1,i2,i3,kd)-4.*(vta(i1,i2+1,i3,
     & kd)+vta(i1,i2-1,i3,kd))+(vta(i1,i2+2,i3,kd)+vta(i1,i2-2,i3,kd))
     &  )/(dx(1)**4)
      vtazzzz23r(i1,i2,i3,kd)=(6.*vta(i1,i2,i3,kd)-4.*(vta(i1,i2,i3+1,
     & kd)+vta(i1,i2,i3-1,kd))+(vta(i1,i2,i3+2,kd)+vta(i1,i2,i3-2,kd))
     &  )/(dx(2)**4)
      vtaxxyy23r(i1,i2,i3,kd)=( 4.*vta(i1,i2,i3,kd)     -2.*(vta(i1+1,
     & i2,i3,kd)+vta(i1-1,i2,i3,kd)+vta(i1,i2+1,i3,kd)+vta(i1,i2-1,i3,
     & kd))   +   (vta(i1+1,i2+1,i3,kd)+vta(i1-1,i2+1,i3,kd)+vta(i1+1,
     & i2-1,i3,kd)+vta(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      vtaxxzz23r(i1,i2,i3,kd)=( 4.*vta(i1,i2,i3,kd)     -2.*(vta(i1+1,
     & i2,i3,kd)+vta(i1-1,i2,i3,kd)+vta(i1,i2,i3+1,kd)+vta(i1,i2,i3-1,
     & kd))   +   (vta(i1+1,i2,i3+1,kd)+vta(i1-1,i2,i3+1,kd)+vta(i1+1,
     & i2,i3-1,kd)+vta(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)
      vtayyzz23r(i1,i2,i3,kd)=( 4.*vta(i1,i2,i3,kd)     -2.*(vta(i1,i2+
     & 1,i3,kd)  +vta(i1,i2-1,i3,kd)+  vta(i1,i2  ,i3+1,kd)+vta(i1,i2 
     &  ,i3-1,kd))   +   (vta(i1,i2+1,i3+1,kd)+vta(i1,i2-1,i3+1,kd)+
     & vta(i1,i2+1,i3-1,kd)+vta(i1,i2-1,i3-1,kd)) )/(dx(1)**2*dx(2)**
     & 2)
      ! 3D laplacian squared = vta.xxxx + vta.yyyy + vta.zzzz + 2 (vta.xxyy + vta.xxzz + vta.yyzz )
      vtaLapSq23r(i1,i2,i3,kd)= ( 6.*vta(i1,i2,i3,kd)   - 4.*(vta(i1+1,
     & i2,i3,kd)+vta(i1-1,i2,i3,kd))    +(vta(i1+2,i2,i3,kd)+vta(i1-2,
     & i2,i3,kd)) )/(dx(0)**4) +( 6.*vta(i1,i2,i3,kd)    -4.*(vta(i1,
     & i2+1,i3,kd)+vta(i1,i2-1,i3,kd))    +(vta(i1,i2+2,i3,kd)+vta(i1,
     & i2-2,i3,kd)) )/(dx(1)**4)  +( 6.*vta(i1,i2,i3,kd)    -4.*(vta(
     & i1,i2,i3+1,kd)+vta(i1,i2,i3-1,kd))    +(vta(i1,i2,i3+2,kd)+vta(
     & i1,i2,i3-2,kd)) )/(dx(2)**4)  +( 8.*vta(i1,i2,i3,kd)     -4.*(
     & vta(i1+1,i2,i3,kd)  +vta(i1-1,i2,i3,kd)  +vta(i1  ,i2+1,i3,kd)+
     & vta(i1  ,i2-1,i3,kd))   +2.*(vta(i1+1,i2+1,i3,kd)+vta(i1-1,i2+
     & 1,i3,kd)+vta(i1+1,i2-1,i3,kd)+vta(i1-1,i2-1,i3,kd)) )/(dx(0)**
     & 2*dx(1)**2)+( 8.*vta(i1,i2,i3,kd)     -4.*(vta(i1+1,i2,i3,kd)  
     & +vta(i1-1,i2,i3,kd)  +vta(i1  ,i2,i3+1,kd)+vta(i1  ,i2,i3-1,kd)
     & )   +2.*(vta(i1+1,i2,i3+1,kd)+vta(i1-1,i2,i3+1,kd)+vta(i1+1,i2,
     & i3-1,kd)+vta(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)+( 8.*vta(
     & i1,i2,i3,kd)     -4.*(vta(i1,i2+1,i3,kd)  +vta(i1,i2-1,i3,kd)  
     & +vta(i1,i2  ,i3+1,kd)+vta(i1,i2  ,i3-1,kd))   +2.*(vta(i1,i2+1,
     & i3+1,kd)+vta(i1,i2-1,i3+1,kd)+vta(i1,i2+1,i3-1,kd)+vta(i1,i2-1,
     & i3-1,kd)) )/(dx(1)**2*dx(2)**2)
      vtamr2(i1,i2,i3,kd)=(vtam(i1+1,i2,i3,kd)-vtam(i1-1,i2,i3,kd))*
     & d12(0)
      vtams2(i1,i2,i3,kd)=(vtam(i1,i2+1,i3,kd)-vtam(i1,i2-1,i3,kd))*
     & d12(1)
      vtamt2(i1,i2,i3,kd)=(vtam(i1,i2,i3+1,kd)-vtam(i1,i2,i3-1,kd))*
     & d12(2)
      vtamrr2(i1,i2,i3,kd)=(-2.*vtam(i1,i2,i3,kd)+(vtam(i1+1,i2,i3,kd)+
     & vtam(i1-1,i2,i3,kd)) )*d22(0)
      vtamss2(i1,i2,i3,kd)=(-2.*vtam(i1,i2,i3,kd)+(vtam(i1,i2+1,i3,kd)+
     & vtam(i1,i2-1,i3,kd)) )*d22(1)
      vtamrs2(i1,i2,i3,kd)=(vtamr2(i1,i2+1,i3,kd)-vtamr2(i1,i2-1,i3,kd)
     & )*d12(1)
      vtamtt2(i1,i2,i3,kd)=(-2.*vtam(i1,i2,i3,kd)+(vtam(i1,i2,i3+1,kd)+
     & vtam(i1,i2,i3-1,kd)) )*d22(2)
      vtamrt2(i1,i2,i3,kd)=(vtamr2(i1,i2,i3+1,kd)-vtamr2(i1,i2,i3-1,kd)
     & )*d12(2)
      vtamst2(i1,i2,i3,kd)=(vtams2(i1,i2,i3+1,kd)-vtams2(i1,i2,i3-1,kd)
     & )*d12(2)
      vtamrrr2(i1,i2,i3,kd)=(-2.*(vtam(i1+1,i2,i3,kd)-vtam(i1-1,i2,i3,
     & kd))+(vtam(i1+2,i2,i3,kd)-vtam(i1-2,i2,i3,kd)) )*d22(0)*d12(0)
      vtamsss2(i1,i2,i3,kd)=(-2.*(vtam(i1,i2+1,i3,kd)-vtam(i1,i2-1,i3,
     & kd))+(vtam(i1,i2+2,i3,kd)-vtam(i1,i2-2,i3,kd)) )*d22(1)*d12(1)
      vtamttt2(i1,i2,i3,kd)=(-2.*(vtam(i1,i2,i3+1,kd)-vtam(i1,i2,i3-1,
     & kd))+(vtam(i1,i2,i3+2,kd)-vtam(i1,i2,i3-2,kd)) )*d22(2)*d12(2)
      vtamx21(i1,i2,i3,kd)= rx(i1,i2,i3)*vtamr2(i1,i2,i3,kd)
      vtamy21(i1,i2,i3,kd)=0
      vtamz21(i1,i2,i3,kd)=0
      vtamx22(i1,i2,i3,kd)= rx(i1,i2,i3)*vtamr2(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*vtams2(i1,i2,i3,kd)
      vtamy22(i1,i2,i3,kd)= ry(i1,i2,i3)*vtamr2(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*vtams2(i1,i2,i3,kd)
      vtamz22(i1,i2,i3,kd)=0
      vtamx23(i1,i2,i3,kd)=rx(i1,i2,i3)*vtamr2(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*vtams2(i1,i2,i3,kd)+tx(i1,i2,i3)*vtamt2(i1,i2,i3,kd)
      vtamy23(i1,i2,i3,kd)=ry(i1,i2,i3)*vtamr2(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*vtams2(i1,i2,i3,kd)+ty(i1,i2,i3)*vtamt2(i1,i2,i3,kd)
      vtamz23(i1,i2,i3,kd)=rz(i1,i2,i3)*vtamr2(i1,i2,i3,kd)+sz(i1,i2,
     & i3)*vtams2(i1,i2,i3,kd)+tz(i1,i2,i3)*vtamt2(i1,i2,i3,kd)
      vtamxx21(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vtamrr2(i1,i2,i3,kd)+(
     & rxx22(i1,i2,i3))*vtamr2(i1,i2,i3,kd)
      vtamyy21(i1,i2,i3,kd)=0
      vtamxy21(i1,i2,i3,kd)=0
      vtamxz21(i1,i2,i3,kd)=0
      vtamyz21(i1,i2,i3,kd)=0
      vtamzz21(i1,i2,i3,kd)=0
      vtamlaplacian21(i1,i2,i3,kd)=vtamxx21(i1,i2,i3,kd)
      vtamxx22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vtamrr2(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*vtamrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*vtamss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3))*vtamr2(i1,i2,i3,kd)+(
     & sxx22(i1,i2,i3))*vtams2(i1,i2,i3,kd)
      vtamyy22(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*vtamrr2(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*vtamrs2(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*vtamss2(i1,i2,i3,kd)+(ryy22(i1,i2,i3))*vtamr2(i1,i2,i3,kd)+(
     & syy22(i1,i2,i3))*vtams2(i1,i2,i3,kd)
      vtamxy22(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vtamrr2(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & vtamrs2(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vtamss2(i1,i2,
     & i3,kd)+rxy22(i1,i2,i3)*vtamr2(i1,i2,i3,kd)+sxy22(i1,i2,i3)*
     & vtams2(i1,i2,i3,kd)
      vtamxz22(i1,i2,i3,kd)=0
      vtamyz22(i1,i2,i3,kd)=0
      vtamzz22(i1,i2,i3,kd)=0
      vtamlaplacian22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & vtamrr2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,
     & i3)*sy(i1,i2,i3))*vtamrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,
     & i2,i3)**2)*vtamss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3)+ryy22(i1,i2,
     & i3))*vtamr2(i1,i2,i3,kd)+(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*
     & vtams2(i1,i2,i3,kd)
      vtamxx23(i1,i2,i3,kd)=rx(i1,i2,i3)**2*vtamrr2(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*vtamss2(i1,i2,i3,kd)+tx(i1,i2,i3)**2*vtamtt2(i1,i2,
     & i3,kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*vtamrs2(i1,i2,i3,kd)+2.*rx(
     & i1,i2,i3)*tx(i1,i2,i3)*vtamrt2(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(
     & i1,i2,i3)*vtamst2(i1,i2,i3,kd)+rxx23(i1,i2,i3)*vtamr2(i1,i2,i3,
     & kd)+sxx23(i1,i2,i3)*vtams2(i1,i2,i3,kd)+txx23(i1,i2,i3)*vtamt2(
     & i1,i2,i3,kd)
      vtamyy23(i1,i2,i3,kd)=ry(i1,i2,i3)**2*vtamrr2(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*vtamss2(i1,i2,i3,kd)+ty(i1,i2,i3)**2*vtamtt2(i1,i2,
     & i3,kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*vtamrs2(i1,i2,i3,kd)+2.*ry(
     & i1,i2,i3)*ty(i1,i2,i3)*vtamrt2(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(
     & i1,i2,i3)*vtamst2(i1,i2,i3,kd)+ryy23(i1,i2,i3)*vtamr2(i1,i2,i3,
     & kd)+syy23(i1,i2,i3)*vtams2(i1,i2,i3,kd)+tyy23(i1,i2,i3)*vtamt2(
     & i1,i2,i3,kd)
      vtamzz23(i1,i2,i3,kd)=rz(i1,i2,i3)**2*vtamrr2(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*vtamss2(i1,i2,i3,kd)+tz(i1,i2,i3)**2*vtamtt2(i1,i2,
     & i3,kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*vtamrs2(i1,i2,i3,kd)+2.*rz(
     & i1,i2,i3)*tz(i1,i2,i3)*vtamrt2(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(
     & i1,i2,i3)*vtamst2(i1,i2,i3,kd)+rzz23(i1,i2,i3)*vtamr2(i1,i2,i3,
     & kd)+szz23(i1,i2,i3)*vtams2(i1,i2,i3,kd)+tzz23(i1,i2,i3)*vtamt2(
     & i1,i2,i3,kd)
      vtamxy23(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vtamrr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vtamss2(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *ty(i1,i2,i3)*vtamtt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+
     & ry(i1,i2,i3)*sx(i1,i2,i3))*vtamrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & ty(i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*vtamrt2(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*vtamst2(
     & i1,i2,i3,kd)+rxy23(i1,i2,i3)*vtamr2(i1,i2,i3,kd)+sxy23(i1,i2,
     & i3)*vtams2(i1,i2,i3,kd)+txy23(i1,i2,i3)*vtamt2(i1,i2,i3,kd)
      vtamxz23(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*vtamrr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*vtamss2(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *tz(i1,i2,i3)*vtamtt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sx(i1,i2,i3))*vtamrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*vtamrt2(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*vtamst2(
     & i1,i2,i3,kd)+rxz23(i1,i2,i3)*vtamr2(i1,i2,i3,kd)+sxz23(i1,i2,
     & i3)*vtams2(i1,i2,i3,kd)+txz23(i1,i2,i3)*vtamt2(i1,i2,i3,kd)
      vtamyz23(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*vtamrr2(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*vtamss2(i1,i2,i3,kd)+ty(i1,i2,i3)
     & *tz(i1,i2,i3)*vtamtt2(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sy(i1,i2,i3))*vtamrs2(i1,i2,i3,kd)+(ry(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*vtamrt2(i1,i2,i3,kd)+(
     & sy(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*vtamst2(
     & i1,i2,i3,kd)+ryz23(i1,i2,i3)*vtamr2(i1,i2,i3,kd)+syz23(i1,i2,
     & i3)*vtams2(i1,i2,i3,kd)+tyz23(i1,i2,i3)*vtamt2(i1,i2,i3,kd)
      vtamlaplacian23(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*vtamrr2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2+sz(i1,i2,i3)**2)*vtamss2(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+
     & ty(i1,i2,i3)**2+tz(i1,i2,i3)**2)*vtamtt2(i1,i2,i3,kd)+2.*(rx(
     & i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*
     & sz(i1,i2,i3))*vtamrs2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,
     & i3)+ ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*
     & vtamrt2(i1,i2,i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,
     & i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3))*vtamst2(i1,i2,i3,
     & kd)+(rxx23(i1,i2,i3)+ryy23(i1,i2,i3)+rzz23(i1,i2,i3))*vtamr2(
     & i1,i2,i3,kd)+(sxx23(i1,i2,i3)+syy23(i1,i2,i3)+szz23(i1,i2,i3))*
     & vtams2(i1,i2,i3,kd)+(txx23(i1,i2,i3)+tyy23(i1,i2,i3)+tzz23(i1,
     & i2,i3))*vtamt2(i1,i2,i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      vtamx23r(i1,i2,i3,kd)=(vtam(i1+1,i2,i3,kd)-vtam(i1-1,i2,i3,kd))*
     & h12(0)
      vtamy23r(i1,i2,i3,kd)=(vtam(i1,i2+1,i3,kd)-vtam(i1,i2-1,i3,kd))*
     & h12(1)
      vtamz23r(i1,i2,i3,kd)=(vtam(i1,i2,i3+1,kd)-vtam(i1,i2,i3-1,kd))*
     & h12(2)
      vtamxx23r(i1,i2,i3,kd)=(-2.*vtam(i1,i2,i3,kd)+(vtam(i1+1,i2,i3,
     & kd)+vtam(i1-1,i2,i3,kd)) )*h22(0)
      vtamyy23r(i1,i2,i3,kd)=(-2.*vtam(i1,i2,i3,kd)+(vtam(i1,i2+1,i3,
     & kd)+vtam(i1,i2-1,i3,kd)) )*h22(1)
      vtamxy23r(i1,i2,i3,kd)=(vtamx23r(i1,i2+1,i3,kd)-vtamx23r(i1,i2-1,
     & i3,kd))*h12(1)
      vtamzz23r(i1,i2,i3,kd)=(-2.*vtam(i1,i2,i3,kd)+(vtam(i1,i2,i3+1,
     & kd)+vtam(i1,i2,i3-1,kd)) )*h22(2)
      vtamxz23r(i1,i2,i3,kd)=(vtamx23r(i1,i2,i3+1,kd)-vtamx23r(i1,i2,
     & i3-1,kd))*h12(2)
      vtamyz23r(i1,i2,i3,kd)=(vtamy23r(i1,i2,i3+1,kd)-vtamy23r(i1,i2,
     & i3-1,kd))*h12(2)
      vtamx21r(i1,i2,i3,kd)= vtamx23r(i1,i2,i3,kd)
      vtamy21r(i1,i2,i3,kd)= vtamy23r(i1,i2,i3,kd)
      vtamz21r(i1,i2,i3,kd)= vtamz23r(i1,i2,i3,kd)
      vtamxx21r(i1,i2,i3,kd)= vtamxx23r(i1,i2,i3,kd)
      vtamyy21r(i1,i2,i3,kd)= vtamyy23r(i1,i2,i3,kd)
      vtamzz21r(i1,i2,i3,kd)= vtamzz23r(i1,i2,i3,kd)
      vtamxy21r(i1,i2,i3,kd)= vtamxy23r(i1,i2,i3,kd)
      vtamxz21r(i1,i2,i3,kd)= vtamxz23r(i1,i2,i3,kd)
      vtamyz21r(i1,i2,i3,kd)= vtamyz23r(i1,i2,i3,kd)
      vtamlaplacian21r(i1,i2,i3,kd)=vtamxx23r(i1,i2,i3,kd)
      vtamx22r(i1,i2,i3,kd)= vtamx23r(i1,i2,i3,kd)
      vtamy22r(i1,i2,i3,kd)= vtamy23r(i1,i2,i3,kd)
      vtamz22r(i1,i2,i3,kd)= vtamz23r(i1,i2,i3,kd)
      vtamxx22r(i1,i2,i3,kd)= vtamxx23r(i1,i2,i3,kd)
      vtamyy22r(i1,i2,i3,kd)= vtamyy23r(i1,i2,i3,kd)
      vtamzz22r(i1,i2,i3,kd)= vtamzz23r(i1,i2,i3,kd)
      vtamxy22r(i1,i2,i3,kd)= vtamxy23r(i1,i2,i3,kd)
      vtamxz22r(i1,i2,i3,kd)= vtamxz23r(i1,i2,i3,kd)
      vtamyz22r(i1,i2,i3,kd)= vtamyz23r(i1,i2,i3,kd)
      vtamlaplacian22r(i1,i2,i3,kd)=vtamxx23r(i1,i2,i3,kd)+vtamyy23r(
     & i1,i2,i3,kd)
      vtamlaplacian23r(i1,i2,i3,kd)=vtamxx23r(i1,i2,i3,kd)+vtamyy23r(
     & i1,i2,i3,kd)+vtamzz23r(i1,i2,i3,kd)
      vtamxxx22r(i1,i2,i3,kd)=(-2.*(vtam(i1+1,i2,i3,kd)-vtam(i1-1,i2,
     & i3,kd))+(vtam(i1+2,i2,i3,kd)-vtam(i1-2,i2,i3,kd)) )*h22(0)*h12(
     & 0)
      vtamyyy22r(i1,i2,i3,kd)=(-2.*(vtam(i1,i2+1,i3,kd)-vtam(i1,i2-1,
     & i3,kd))+(vtam(i1,i2+2,i3,kd)-vtam(i1,i2-2,i3,kd)) )*h22(1)*h12(
     & 1)
      vtamxxy22r(i1,i2,i3,kd)=( vtamxx22r(i1,i2+1,i3,kd)-vtamxx22r(i1,
     & i2-1,i3,kd))/(2.*dx(1))
      vtamxyy22r(i1,i2,i3,kd)=( vtamyy22r(i1+1,i2,i3,kd)-vtamyy22r(i1-
     & 1,i2,i3,kd))/(2.*dx(0))
      vtamxxxx22r(i1,i2,i3,kd)=(6.*vtam(i1,i2,i3,kd)-4.*(vtam(i1+1,i2,
     & i3,kd)+vtam(i1-1,i2,i3,kd))+(vtam(i1+2,i2,i3,kd)+vtam(i1-2,i2,
     & i3,kd)) )/(dx(0)**4)
      vtamyyyy22r(i1,i2,i3,kd)=(6.*vtam(i1,i2,i3,kd)-4.*(vtam(i1,i2+1,
     & i3,kd)+vtam(i1,i2-1,i3,kd))+(vtam(i1,i2+2,i3,kd)+vtam(i1,i2-2,
     & i3,kd)) )/(dx(1)**4)
      vtamxxyy22r(i1,i2,i3,kd)=( 4.*vtam(i1,i2,i3,kd)     -2.*(vtam(i1+
     & 1,i2,i3,kd)+vtam(i1-1,i2,i3,kd)+vtam(i1,i2+1,i3,kd)+vtam(i1,i2-
     & 1,i3,kd))   +   (vtam(i1+1,i2+1,i3,kd)+vtam(i1-1,i2+1,i3,kd)+
     & vtam(i1+1,i2-1,i3,kd)+vtam(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)*
     & *2)
      ! 2D laplacian squared = vtam.xxxx + 2 vtam.xxyy + vtam.yyyy
      vtamLapSq22r(i1,i2,i3,kd)= ( 6.*vtam(i1,i2,i3,kd)   - 4.*(vtam(
     & i1+1,i2,i3,kd)+vtam(i1-1,i2,i3,kd))    +(vtam(i1+2,i2,i3,kd)+
     & vtam(i1-2,i2,i3,kd)) )/(dx(0)**4) +( 6.*vtam(i1,i2,i3,kd)    -
     & 4.*(vtam(i1,i2+1,i3,kd)+vtam(i1,i2-1,i3,kd))    +(vtam(i1,i2+2,
     & i3,kd)+vtam(i1,i2-2,i3,kd)) )/(dx(1)**4)  +( 8.*vtam(i1,i2,i3,
     & kd)     -4.*(vtam(i1+1,i2,i3,kd)+vtam(i1-1,i2,i3,kd)+vtam(i1,
     & i2+1,i3,kd)+vtam(i1,i2-1,i3,kd))   +2.*(vtam(i1+1,i2+1,i3,kd)+
     & vtam(i1-1,i2+1,i3,kd)+vtam(i1+1,i2-1,i3,kd)+vtam(i1-1,i2-1,i3,
     & kd)) )/(dx(0)**2*dx(1)**2)
      vtamxxx23r(i1,i2,i3,kd)=(-2.*(vtam(i1+1,i2,i3,kd)-vtam(i1-1,i2,
     & i3,kd))+(vtam(i1+2,i2,i3,kd)-vtam(i1-2,i2,i3,kd)) )*h22(0)*h12(
     & 0)
      vtamyyy23r(i1,i2,i3,kd)=(-2.*(vtam(i1,i2+1,i3,kd)-vtam(i1,i2-1,
     & i3,kd))+(vtam(i1,i2+2,i3,kd)-vtam(i1,i2-2,i3,kd)) )*h22(1)*h12(
     & 1)
      vtamzzz23r(i1,i2,i3,kd)=(-2.*(vtam(i1,i2,i3+1,kd)-vtam(i1,i2,i3-
     & 1,kd))+(vtam(i1,i2,i3+2,kd)-vtam(i1,i2,i3-2,kd)) )*h22(1)*h12(
     & 2)
      vtamxxy23r(i1,i2,i3,kd)=( vtamxx22r(i1,i2+1,i3,kd)-vtamxx22r(i1,
     & i2-1,i3,kd))/(2.*dx(1))
      vtamxyy23r(i1,i2,i3,kd)=( vtamyy22r(i1+1,i2,i3,kd)-vtamyy22r(i1-
     & 1,i2,i3,kd))/(2.*dx(0))
      vtamxxz23r(i1,i2,i3,kd)=( vtamxx22r(i1,i2,i3+1,kd)-vtamxx22r(i1,
     & i2,i3-1,kd))/(2.*dx(2))
      vtamyyz23r(i1,i2,i3,kd)=( vtamyy22r(i1,i2,i3+1,kd)-vtamyy22r(i1,
     & i2,i3-1,kd))/(2.*dx(2))
      vtamxzz23r(i1,i2,i3,kd)=( vtamzz22r(i1+1,i2,i3,kd)-vtamzz22r(i1-
     & 1,i2,i3,kd))/(2.*dx(0))
      vtamyzz23r(i1,i2,i3,kd)=( vtamzz22r(i1,i2+1,i3,kd)-vtamzz22r(i1,
     & i2-1,i3,kd))/(2.*dx(1))
      vtamxxxx23r(i1,i2,i3,kd)=(6.*vtam(i1,i2,i3,kd)-4.*(vtam(i1+1,i2,
     & i3,kd)+vtam(i1-1,i2,i3,kd))+(vtam(i1+2,i2,i3,kd)+vtam(i1-2,i2,
     & i3,kd)) )/(dx(0)**4)
      vtamyyyy23r(i1,i2,i3,kd)=(6.*vtam(i1,i2,i3,kd)-4.*(vtam(i1,i2+1,
     & i3,kd)+vtam(i1,i2-1,i3,kd))+(vtam(i1,i2+2,i3,kd)+vtam(i1,i2-2,
     & i3,kd)) )/(dx(1)**4)
      vtamzzzz23r(i1,i2,i3,kd)=(6.*vtam(i1,i2,i3,kd)-4.*(vtam(i1,i2,i3+
     & 1,kd)+vtam(i1,i2,i3-1,kd))+(vtam(i1,i2,i3+2,kd)+vtam(i1,i2,i3-
     & 2,kd)) )/(dx(2)**4)
      vtamxxyy23r(i1,i2,i3,kd)=( 4.*vtam(i1,i2,i3,kd)     -2.*(vtam(i1+
     & 1,i2,i3,kd)+vtam(i1-1,i2,i3,kd)+vtam(i1,i2+1,i3,kd)+vtam(i1,i2-
     & 1,i3,kd))   +   (vtam(i1+1,i2+1,i3,kd)+vtam(i1-1,i2+1,i3,kd)+
     & vtam(i1+1,i2-1,i3,kd)+vtam(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)*
     & *2)
      vtamxxzz23r(i1,i2,i3,kd)=( 4.*vtam(i1,i2,i3,kd)     -2.*(vtam(i1+
     & 1,i2,i3,kd)+vtam(i1-1,i2,i3,kd)+vtam(i1,i2,i3+1,kd)+vtam(i1,i2,
     & i3-1,kd))   +   (vtam(i1+1,i2,i3+1,kd)+vtam(i1-1,i2,i3+1,kd)+
     & vtam(i1+1,i2,i3-1,kd)+vtam(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)*
     & *2)
      vtamyyzz23r(i1,i2,i3,kd)=( 4.*vtam(i1,i2,i3,kd)     -2.*(vtam(i1,
     & i2+1,i3,kd)  +vtam(i1,i2-1,i3,kd)+  vtam(i1,i2  ,i3+1,kd)+vtam(
     & i1,i2  ,i3-1,kd))   +   (vtam(i1,i2+1,i3+1,kd)+vtam(i1,i2-1,i3+
     & 1,kd)+vtam(i1,i2+1,i3-1,kd)+vtam(i1,i2-1,i3-1,kd)) )/(dx(1)**2*
     & dx(2)**2)
      ! 3D laplacian squared = vtam.xxxx + vtam.yyyy + vtam.zzzz + 2 (vtam.xxyy + vtam.xxzz + vtam.yyzz )
      vtamLapSq23r(i1,i2,i3,kd)= ( 6.*vtam(i1,i2,i3,kd)   - 4.*(vtam(
     & i1+1,i2,i3,kd)+vtam(i1-1,i2,i3,kd))    +(vtam(i1+2,i2,i3,kd)+
     & vtam(i1-2,i2,i3,kd)) )/(dx(0)**4) +( 6.*vtam(i1,i2,i3,kd)    -
     & 4.*(vtam(i1,i2+1,i3,kd)+vtam(i1,i2-1,i3,kd))    +(vtam(i1,i2+2,
     & i3,kd)+vtam(i1,i2-2,i3,kd)) )/(dx(1)**4)  +( 6.*vtam(i1,i2,i3,
     & kd)    -4.*(vtam(i1,i2,i3+1,kd)+vtam(i1,i2,i3-1,kd))    +(vtam(
     & i1,i2,i3+2,kd)+vtam(i1,i2,i3-2,kd)) )/(dx(2)**4)  +( 8.*vtam(
     & i1,i2,i3,kd)     -4.*(vtam(i1+1,i2,i3,kd)  +vtam(i1-1,i2,i3,kd)
     &   +vtam(i1  ,i2+1,i3,kd)+vtam(i1  ,i2-1,i3,kd))   +2.*(vtam(i1+
     & 1,i2+1,i3,kd)+vtam(i1-1,i2+1,i3,kd)+vtam(i1+1,i2-1,i3,kd)+vtam(
     & i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)+( 8.*vtam(i1,i2,i3,kd) 
     &     -4.*(vtam(i1+1,i2,i3,kd)  +vtam(i1-1,i2,i3,kd)  +vtam(i1  ,
     & i2,i3+1,kd)+vtam(i1  ,i2,i3-1,kd))   +2.*(vtam(i1+1,i2,i3+1,kd)
     & +vtam(i1-1,i2,i3+1,kd)+vtam(i1+1,i2,i3-1,kd)+vtam(i1-1,i2,i3-1,
     & kd)) )/(dx(0)**2*dx(2)**2)+( 8.*vtam(i1,i2,i3,kd)     -4.*(
     & vtam(i1,i2+1,i3,kd)  +vtam(i1,i2-1,i3,kd)  +vtam(i1,i2  ,i3+1,
     & kd)+vtam(i1,i2  ,i3-1,kd))   +2.*(vtam(i1,i2+1,i3+1,kd)+vtam(
     & i1,i2-1,i3+1,kd)+vtam(i1,i2+1,i3-1,kd)+vtam(i1,i2-1,i3-1,kd)) )
     & /(dx(1)**2*dx(2)**2)
      wtar2(i1,i2,i3,kd)=(wta(i1+1,i2,i3,kd)-wta(i1-1,i2,i3,kd))*d12(0)
      wtas2(i1,i2,i3,kd)=(wta(i1,i2+1,i3,kd)-wta(i1,i2-1,i3,kd))*d12(1)
      wtat2(i1,i2,i3,kd)=(wta(i1,i2,i3+1,kd)-wta(i1,i2,i3-1,kd))*d12(2)
      wtarr2(i1,i2,i3,kd)=(-2.*wta(i1,i2,i3,kd)+(wta(i1+1,i2,i3,kd)+
     & wta(i1-1,i2,i3,kd)) )*d22(0)
      wtass2(i1,i2,i3,kd)=(-2.*wta(i1,i2,i3,kd)+(wta(i1,i2+1,i3,kd)+
     & wta(i1,i2-1,i3,kd)) )*d22(1)
      wtars2(i1,i2,i3,kd)=(wtar2(i1,i2+1,i3,kd)-wtar2(i1,i2-1,i3,kd))*
     & d12(1)
      wtatt2(i1,i2,i3,kd)=(-2.*wta(i1,i2,i3,kd)+(wta(i1,i2,i3+1,kd)+
     & wta(i1,i2,i3-1,kd)) )*d22(2)
      wtart2(i1,i2,i3,kd)=(wtar2(i1,i2,i3+1,kd)-wtar2(i1,i2,i3-1,kd))*
     & d12(2)
      wtast2(i1,i2,i3,kd)=(wtas2(i1,i2,i3+1,kd)-wtas2(i1,i2,i3-1,kd))*
     & d12(2)
      wtarrr2(i1,i2,i3,kd)=(-2.*(wta(i1+1,i2,i3,kd)-wta(i1-1,i2,i3,kd))
     & +(wta(i1+2,i2,i3,kd)-wta(i1-2,i2,i3,kd)) )*d22(0)*d12(0)
      wtasss2(i1,i2,i3,kd)=(-2.*(wta(i1,i2+1,i3,kd)-wta(i1,i2-1,i3,kd))
     & +(wta(i1,i2+2,i3,kd)-wta(i1,i2-2,i3,kd)) )*d22(1)*d12(1)
      wtattt2(i1,i2,i3,kd)=(-2.*(wta(i1,i2,i3+1,kd)-wta(i1,i2,i3-1,kd))
     & +(wta(i1,i2,i3+2,kd)-wta(i1,i2,i3-2,kd)) )*d22(2)*d12(2)
      wtax21(i1,i2,i3,kd)= rx(i1,i2,i3)*wtar2(i1,i2,i3,kd)
      wtay21(i1,i2,i3,kd)=0
      wtaz21(i1,i2,i3,kd)=0
      wtax22(i1,i2,i3,kd)= rx(i1,i2,i3)*wtar2(i1,i2,i3,kd)+sx(i1,i2,i3)
     & *wtas2(i1,i2,i3,kd)
      wtay22(i1,i2,i3,kd)= ry(i1,i2,i3)*wtar2(i1,i2,i3,kd)+sy(i1,i2,i3)
     & *wtas2(i1,i2,i3,kd)
      wtaz22(i1,i2,i3,kd)=0
      wtax23(i1,i2,i3,kd)=rx(i1,i2,i3)*wtar2(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & wtas2(i1,i2,i3,kd)+tx(i1,i2,i3)*wtat2(i1,i2,i3,kd)
      wtay23(i1,i2,i3,kd)=ry(i1,i2,i3)*wtar2(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & wtas2(i1,i2,i3,kd)+ty(i1,i2,i3)*wtat2(i1,i2,i3,kd)
      wtaz23(i1,i2,i3,kd)=rz(i1,i2,i3)*wtar2(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & wtas2(i1,i2,i3,kd)+tz(i1,i2,i3)*wtat2(i1,i2,i3,kd)
      wtaxx21(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wtarr2(i1,i2,i3,kd)+(
     & rxx22(i1,i2,i3))*wtar2(i1,i2,i3,kd)
      wtayy21(i1,i2,i3,kd)=0
      wtaxy21(i1,i2,i3,kd)=0
      wtaxz21(i1,i2,i3,kd)=0
      wtayz21(i1,i2,i3,kd)=0
      wtazz21(i1,i2,i3,kd)=0
      wtalaplacian21(i1,i2,i3,kd)=wtaxx21(i1,i2,i3,kd)
      wtaxx22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wtarr2(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*wtars2(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*wtass2(i1,i2,i3,kd)+(rxx22(i1,i2,i3))*wtar2(i1,i2,i3,kd)+(
     & sxx22(i1,i2,i3))*wtas2(i1,i2,i3,kd)
      wtayy22(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*wtarr2(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*wtars2(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*wtass2(i1,i2,i3,kd)+(ryy22(i1,i2,i3))*wtar2(i1,i2,i3,kd)+(
     & syy22(i1,i2,i3))*wtas2(i1,i2,i3,kd)
      wtaxy22(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wtarr2(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & wtars2(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wtass2(i1,i2,i3,
     & kd)+rxy22(i1,i2,i3)*wtar2(i1,i2,i3,kd)+sxy22(i1,i2,i3)*wtas2(
     & i1,i2,i3,kd)
      wtaxz22(i1,i2,i3,kd)=0
      wtayz22(i1,i2,i3,kd)=0
      wtazz22(i1,i2,i3,kd)=0
      wtalaplacian22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & wtarr2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)
     & *sy(i1,i2,i3))*wtars2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2)*wtass2(i1,i2,i3,kd)+(rxx22(i1,i2,i3)+ryy22(i1,i2,i3))*
     & wtar2(i1,i2,i3,kd)+(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*wtas2(i1,
     & i2,i3,kd)
      wtaxx23(i1,i2,i3,kd)=rx(i1,i2,i3)**2*wtarr2(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*wtass2(i1,i2,i3,kd)+tx(i1,i2,i3)**2*wtatt2(i1,i2,i3,
     & kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*wtars2(i1,i2,i3,kd)+2.*rx(i1,
     & i2,i3)*tx(i1,i2,i3)*wtart2(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,
     & i2,i3)*wtast2(i1,i2,i3,kd)+rxx23(i1,i2,i3)*wtar2(i1,i2,i3,kd)+
     & sxx23(i1,i2,i3)*wtas2(i1,i2,i3,kd)+txx23(i1,i2,i3)*wtat2(i1,i2,
     & i3,kd)
      wtayy23(i1,i2,i3,kd)=ry(i1,i2,i3)**2*wtarr2(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*wtass2(i1,i2,i3,kd)+ty(i1,i2,i3)**2*wtatt2(i1,i2,i3,
     & kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*wtars2(i1,i2,i3,kd)+2.*ry(i1,
     & i2,i3)*ty(i1,i2,i3)*wtart2(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,
     & i2,i3)*wtast2(i1,i2,i3,kd)+ryy23(i1,i2,i3)*wtar2(i1,i2,i3,kd)+
     & syy23(i1,i2,i3)*wtas2(i1,i2,i3,kd)+tyy23(i1,i2,i3)*wtat2(i1,i2,
     & i3,kd)
      wtazz23(i1,i2,i3,kd)=rz(i1,i2,i3)**2*wtarr2(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*wtass2(i1,i2,i3,kd)+tz(i1,i2,i3)**2*wtatt2(i1,i2,i3,
     & kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*wtars2(i1,i2,i3,kd)+2.*rz(i1,
     & i2,i3)*tz(i1,i2,i3)*wtart2(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,
     & i2,i3)*wtast2(i1,i2,i3,kd)+rzz23(i1,i2,i3)*wtar2(i1,i2,i3,kd)+
     & szz23(i1,i2,i3)*wtas2(i1,i2,i3,kd)+tzz23(i1,i2,i3)*wtat2(i1,i2,
     & i3,kd)
      wtaxy23(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wtarr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wtass2(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & ty(i1,i2,i3)*wtatt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(
     & i1,i2,i3)*sx(i1,i2,i3))*wtars2(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(
     & i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*wtart2(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*wtast2(i1,i2,
     & i3,kd)+rxy23(i1,i2,i3)*wtar2(i1,i2,i3,kd)+sxy23(i1,i2,i3)*
     & wtas2(i1,i2,i3,kd)+txy23(i1,i2,i3)*wtat2(i1,i2,i3,kd)
      wtaxz23(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*wtarr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*wtass2(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & tz(i1,i2,i3)*wtatt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sx(i1,i2,i3))*wtars2(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*wtart2(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*wtast2(i1,i2,
     & i3,kd)+rxz23(i1,i2,i3)*wtar2(i1,i2,i3,kd)+sxz23(i1,i2,i3)*
     & wtas2(i1,i2,i3,kd)+txz23(i1,i2,i3)*wtat2(i1,i2,i3,kd)
      wtayz23(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*wtarr2(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*wtass2(i1,i2,i3,kd)+ty(i1,i2,i3)*
     & tz(i1,i2,i3)*wtatt2(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sy(i1,i2,i3))*wtars2(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*wtart2(i1,i2,i3,kd)+(sy(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*wtast2(i1,i2,
     & i3,kd)+ryz23(i1,i2,i3)*wtar2(i1,i2,i3,kd)+syz23(i1,i2,i3)*
     & wtas2(i1,i2,i3,kd)+tyz23(i1,i2,i3)*wtat2(i1,i2,i3,kd)
      wtalaplacian23(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*wtarr2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)
     & **2+sz(i1,i2,i3)**2)*wtass2(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(
     & i1,i2,i3)**2+tz(i1,i2,i3)**2)*wtatt2(i1,i2,i3,kd)+2.*(rx(i1,i2,
     & i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,
     & i2,i3))*wtars2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(
     & i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*wtart2(i1,i2,
     & i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)
     & +sz(i1,i2,i3)*tz(i1,i2,i3))*wtast2(i1,i2,i3,kd)+(rxx23(i1,i2,
     & i3)+ryy23(i1,i2,i3)+rzz23(i1,i2,i3))*wtar2(i1,i2,i3,kd)+(sxx23(
     & i1,i2,i3)+syy23(i1,i2,i3)+szz23(i1,i2,i3))*wtas2(i1,i2,i3,kd)+(
     & txx23(i1,i2,i3)+tyy23(i1,i2,i3)+tzz23(i1,i2,i3))*wtat2(i1,i2,
     & i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      wtax23r(i1,i2,i3,kd)=(wta(i1+1,i2,i3,kd)-wta(i1-1,i2,i3,kd))*h12(
     & 0)
      wtay23r(i1,i2,i3,kd)=(wta(i1,i2+1,i3,kd)-wta(i1,i2-1,i3,kd))*h12(
     & 1)
      wtaz23r(i1,i2,i3,kd)=(wta(i1,i2,i3+1,kd)-wta(i1,i2,i3-1,kd))*h12(
     & 2)
      wtaxx23r(i1,i2,i3,kd)=(-2.*wta(i1,i2,i3,kd)+(wta(i1+1,i2,i3,kd)+
     & wta(i1-1,i2,i3,kd)) )*h22(0)
      wtayy23r(i1,i2,i3,kd)=(-2.*wta(i1,i2,i3,kd)+(wta(i1,i2+1,i3,kd)+
     & wta(i1,i2-1,i3,kd)) )*h22(1)
      wtaxy23r(i1,i2,i3,kd)=(wtax23r(i1,i2+1,i3,kd)-wtax23r(i1,i2-1,i3,
     & kd))*h12(1)
      wtazz23r(i1,i2,i3,kd)=(-2.*wta(i1,i2,i3,kd)+(wta(i1,i2,i3+1,kd)+
     & wta(i1,i2,i3-1,kd)) )*h22(2)
      wtaxz23r(i1,i2,i3,kd)=(wtax23r(i1,i2,i3+1,kd)-wtax23r(i1,i2,i3-1,
     & kd))*h12(2)
      wtayz23r(i1,i2,i3,kd)=(wtay23r(i1,i2,i3+1,kd)-wtay23r(i1,i2,i3-1,
     & kd))*h12(2)
      wtax21r(i1,i2,i3,kd)= wtax23r(i1,i2,i3,kd)
      wtay21r(i1,i2,i3,kd)= wtay23r(i1,i2,i3,kd)
      wtaz21r(i1,i2,i3,kd)= wtaz23r(i1,i2,i3,kd)
      wtaxx21r(i1,i2,i3,kd)= wtaxx23r(i1,i2,i3,kd)
      wtayy21r(i1,i2,i3,kd)= wtayy23r(i1,i2,i3,kd)
      wtazz21r(i1,i2,i3,kd)= wtazz23r(i1,i2,i3,kd)
      wtaxy21r(i1,i2,i3,kd)= wtaxy23r(i1,i2,i3,kd)
      wtaxz21r(i1,i2,i3,kd)= wtaxz23r(i1,i2,i3,kd)
      wtayz21r(i1,i2,i3,kd)= wtayz23r(i1,i2,i3,kd)
      wtalaplacian21r(i1,i2,i3,kd)=wtaxx23r(i1,i2,i3,kd)
      wtax22r(i1,i2,i3,kd)= wtax23r(i1,i2,i3,kd)
      wtay22r(i1,i2,i3,kd)= wtay23r(i1,i2,i3,kd)
      wtaz22r(i1,i2,i3,kd)= wtaz23r(i1,i2,i3,kd)
      wtaxx22r(i1,i2,i3,kd)= wtaxx23r(i1,i2,i3,kd)
      wtayy22r(i1,i2,i3,kd)= wtayy23r(i1,i2,i3,kd)
      wtazz22r(i1,i2,i3,kd)= wtazz23r(i1,i2,i3,kd)
      wtaxy22r(i1,i2,i3,kd)= wtaxy23r(i1,i2,i3,kd)
      wtaxz22r(i1,i2,i3,kd)= wtaxz23r(i1,i2,i3,kd)
      wtayz22r(i1,i2,i3,kd)= wtayz23r(i1,i2,i3,kd)
      wtalaplacian22r(i1,i2,i3,kd)=wtaxx23r(i1,i2,i3,kd)+wtayy23r(i1,
     & i2,i3,kd)
      wtalaplacian23r(i1,i2,i3,kd)=wtaxx23r(i1,i2,i3,kd)+wtayy23r(i1,
     & i2,i3,kd)+wtazz23r(i1,i2,i3,kd)
      wtaxxx22r(i1,i2,i3,kd)=(-2.*(wta(i1+1,i2,i3,kd)-wta(i1-1,i2,i3,
     & kd))+(wta(i1+2,i2,i3,kd)-wta(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
      wtayyy22r(i1,i2,i3,kd)=(-2.*(wta(i1,i2+1,i3,kd)-wta(i1,i2-1,i3,
     & kd))+(wta(i1,i2+2,i3,kd)-wta(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
      wtaxxy22r(i1,i2,i3,kd)=( wtaxx22r(i1,i2+1,i3,kd)-wtaxx22r(i1,i2-
     & 1,i3,kd))/(2.*dx(1))
      wtaxyy22r(i1,i2,i3,kd)=( wtayy22r(i1+1,i2,i3,kd)-wtayy22r(i1-1,
     & i2,i3,kd))/(2.*dx(0))
      wtaxxxx22r(i1,i2,i3,kd)=(6.*wta(i1,i2,i3,kd)-4.*(wta(i1+1,i2,i3,
     & kd)+wta(i1-1,i2,i3,kd))+(wta(i1+2,i2,i3,kd)+wta(i1-2,i2,i3,kd))
     &  )/(dx(0)**4)
      wtayyyy22r(i1,i2,i3,kd)=(6.*wta(i1,i2,i3,kd)-4.*(wta(i1,i2+1,i3,
     & kd)+wta(i1,i2-1,i3,kd))+(wta(i1,i2+2,i3,kd)+wta(i1,i2-2,i3,kd))
     &  )/(dx(1)**4)
      wtaxxyy22r(i1,i2,i3,kd)=( 4.*wta(i1,i2,i3,kd)     -2.*(wta(i1+1,
     & i2,i3,kd)+wta(i1-1,i2,i3,kd)+wta(i1,i2+1,i3,kd)+wta(i1,i2-1,i3,
     & kd))   +   (wta(i1+1,i2+1,i3,kd)+wta(i1-1,i2+1,i3,kd)+wta(i1+1,
     & i2-1,i3,kd)+wta(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      ! 2D laplacian squared = wta.xxxx + 2 wta.xxyy + wta.yyyy
      wtaLapSq22r(i1,i2,i3,kd)= ( 6.*wta(i1,i2,i3,kd)   - 4.*(wta(i1+1,
     & i2,i3,kd)+wta(i1-1,i2,i3,kd))    +(wta(i1+2,i2,i3,kd)+wta(i1-2,
     & i2,i3,kd)) )/(dx(0)**4) +( 6.*wta(i1,i2,i3,kd)    -4.*(wta(i1,
     & i2+1,i3,kd)+wta(i1,i2-1,i3,kd))    +(wta(i1,i2+2,i3,kd)+wta(i1,
     & i2-2,i3,kd)) )/(dx(1)**4)  +( 8.*wta(i1,i2,i3,kd)     -4.*(wta(
     & i1+1,i2,i3,kd)+wta(i1-1,i2,i3,kd)+wta(i1,i2+1,i3,kd)+wta(i1,i2-
     & 1,i3,kd))   +2.*(wta(i1+1,i2+1,i3,kd)+wta(i1-1,i2+1,i3,kd)+wta(
     & i1+1,i2-1,i3,kd)+wta(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      wtaxxx23r(i1,i2,i3,kd)=(-2.*(wta(i1+1,i2,i3,kd)-wta(i1-1,i2,i3,
     & kd))+(wta(i1+2,i2,i3,kd)-wta(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
      wtayyy23r(i1,i2,i3,kd)=(-2.*(wta(i1,i2+1,i3,kd)-wta(i1,i2-1,i3,
     & kd))+(wta(i1,i2+2,i3,kd)-wta(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
      wtazzz23r(i1,i2,i3,kd)=(-2.*(wta(i1,i2,i3+1,kd)-wta(i1,i2,i3-1,
     & kd))+(wta(i1,i2,i3+2,kd)-wta(i1,i2,i3-2,kd)) )*h22(1)*h12(2)
      wtaxxy23r(i1,i2,i3,kd)=( wtaxx22r(i1,i2+1,i3,kd)-wtaxx22r(i1,i2-
     & 1,i3,kd))/(2.*dx(1))
      wtaxyy23r(i1,i2,i3,kd)=( wtayy22r(i1+1,i2,i3,kd)-wtayy22r(i1-1,
     & i2,i3,kd))/(2.*dx(0))
      wtaxxz23r(i1,i2,i3,kd)=( wtaxx22r(i1,i2,i3+1,kd)-wtaxx22r(i1,i2,
     & i3-1,kd))/(2.*dx(2))
      wtayyz23r(i1,i2,i3,kd)=( wtayy22r(i1,i2,i3+1,kd)-wtayy22r(i1,i2,
     & i3-1,kd))/(2.*dx(2))
      wtaxzz23r(i1,i2,i3,kd)=( wtazz22r(i1+1,i2,i3,kd)-wtazz22r(i1-1,
     & i2,i3,kd))/(2.*dx(0))
      wtayzz23r(i1,i2,i3,kd)=( wtazz22r(i1,i2+1,i3,kd)-wtazz22r(i1,i2-
     & 1,i3,kd))/(2.*dx(1))
      wtaxxxx23r(i1,i2,i3,kd)=(6.*wta(i1,i2,i3,kd)-4.*(wta(i1+1,i2,i3,
     & kd)+wta(i1-1,i2,i3,kd))+(wta(i1+2,i2,i3,kd)+wta(i1-2,i2,i3,kd))
     &  )/(dx(0)**4)
      wtayyyy23r(i1,i2,i3,kd)=(6.*wta(i1,i2,i3,kd)-4.*(wta(i1,i2+1,i3,
     & kd)+wta(i1,i2-1,i3,kd))+(wta(i1,i2+2,i3,kd)+wta(i1,i2-2,i3,kd))
     &  )/(dx(1)**4)
      wtazzzz23r(i1,i2,i3,kd)=(6.*wta(i1,i2,i3,kd)-4.*(wta(i1,i2,i3+1,
     & kd)+wta(i1,i2,i3-1,kd))+(wta(i1,i2,i3+2,kd)+wta(i1,i2,i3-2,kd))
     &  )/(dx(2)**4)
      wtaxxyy23r(i1,i2,i3,kd)=( 4.*wta(i1,i2,i3,kd)     -2.*(wta(i1+1,
     & i2,i3,kd)+wta(i1-1,i2,i3,kd)+wta(i1,i2+1,i3,kd)+wta(i1,i2-1,i3,
     & kd))   +   (wta(i1+1,i2+1,i3,kd)+wta(i1-1,i2+1,i3,kd)+wta(i1+1,
     & i2-1,i3,kd)+wta(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      wtaxxzz23r(i1,i2,i3,kd)=( 4.*wta(i1,i2,i3,kd)     -2.*(wta(i1+1,
     & i2,i3,kd)+wta(i1-1,i2,i3,kd)+wta(i1,i2,i3+1,kd)+wta(i1,i2,i3-1,
     & kd))   +   (wta(i1+1,i2,i3+1,kd)+wta(i1-1,i2,i3+1,kd)+wta(i1+1,
     & i2,i3-1,kd)+wta(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)
      wtayyzz23r(i1,i2,i3,kd)=( 4.*wta(i1,i2,i3,kd)     -2.*(wta(i1,i2+
     & 1,i3,kd)  +wta(i1,i2-1,i3,kd)+  wta(i1,i2  ,i3+1,kd)+wta(i1,i2 
     &  ,i3-1,kd))   +   (wta(i1,i2+1,i3+1,kd)+wta(i1,i2-1,i3+1,kd)+
     & wta(i1,i2+1,i3-1,kd)+wta(i1,i2-1,i3-1,kd)) )/(dx(1)**2*dx(2)**
     & 2)
      ! 3D laplacian squared = wta.xxxx + wta.yyyy + wta.zzzz + 2 (wta.xxyy + wta.xxzz + wta.yyzz )
      wtaLapSq23r(i1,i2,i3,kd)= ( 6.*wta(i1,i2,i3,kd)   - 4.*(wta(i1+1,
     & i2,i3,kd)+wta(i1-1,i2,i3,kd))    +(wta(i1+2,i2,i3,kd)+wta(i1-2,
     & i2,i3,kd)) )/(dx(0)**4) +( 6.*wta(i1,i2,i3,kd)    -4.*(wta(i1,
     & i2+1,i3,kd)+wta(i1,i2-1,i3,kd))    +(wta(i1,i2+2,i3,kd)+wta(i1,
     & i2-2,i3,kd)) )/(dx(1)**4)  +( 6.*wta(i1,i2,i3,kd)    -4.*(wta(
     & i1,i2,i3+1,kd)+wta(i1,i2,i3-1,kd))    +(wta(i1,i2,i3+2,kd)+wta(
     & i1,i2,i3-2,kd)) )/(dx(2)**4)  +( 8.*wta(i1,i2,i3,kd)     -4.*(
     & wta(i1+1,i2,i3,kd)  +wta(i1-1,i2,i3,kd)  +wta(i1  ,i2+1,i3,kd)+
     & wta(i1  ,i2-1,i3,kd))   +2.*(wta(i1+1,i2+1,i3,kd)+wta(i1-1,i2+
     & 1,i3,kd)+wta(i1+1,i2-1,i3,kd)+wta(i1-1,i2-1,i3,kd)) )/(dx(0)**
     & 2*dx(1)**2)+( 8.*wta(i1,i2,i3,kd)     -4.*(wta(i1+1,i2,i3,kd)  
     & +wta(i1-1,i2,i3,kd)  +wta(i1  ,i2,i3+1,kd)+wta(i1  ,i2,i3-1,kd)
     & )   +2.*(wta(i1+1,i2,i3+1,kd)+wta(i1-1,i2,i3+1,kd)+wta(i1+1,i2,
     & i3-1,kd)+wta(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)+( 8.*wta(
     & i1,i2,i3,kd)     -4.*(wta(i1,i2+1,i3,kd)  +wta(i1,i2-1,i3,kd)  
     & +wta(i1,i2  ,i3+1,kd)+wta(i1,i2  ,i3-1,kd))   +2.*(wta(i1,i2+1,
     & i3+1,kd)+wta(i1,i2-1,i3+1,kd)+wta(i1,i2+1,i3-1,kd)+wta(i1,i2-1,
     & i3-1,kd)) )/(dx(1)**2*dx(2)**2)
      wtamr2(i1,i2,i3,kd)=(wtam(i1+1,i2,i3,kd)-wtam(i1-1,i2,i3,kd))*
     & d12(0)
      wtams2(i1,i2,i3,kd)=(wtam(i1,i2+1,i3,kd)-wtam(i1,i2-1,i3,kd))*
     & d12(1)
      wtamt2(i1,i2,i3,kd)=(wtam(i1,i2,i3+1,kd)-wtam(i1,i2,i3-1,kd))*
     & d12(2)
      wtamrr2(i1,i2,i3,kd)=(-2.*wtam(i1,i2,i3,kd)+(wtam(i1+1,i2,i3,kd)+
     & wtam(i1-1,i2,i3,kd)) )*d22(0)
      wtamss2(i1,i2,i3,kd)=(-2.*wtam(i1,i2,i3,kd)+(wtam(i1,i2+1,i3,kd)+
     & wtam(i1,i2-1,i3,kd)) )*d22(1)
      wtamrs2(i1,i2,i3,kd)=(wtamr2(i1,i2+1,i3,kd)-wtamr2(i1,i2-1,i3,kd)
     & )*d12(1)
      wtamtt2(i1,i2,i3,kd)=(-2.*wtam(i1,i2,i3,kd)+(wtam(i1,i2,i3+1,kd)+
     & wtam(i1,i2,i3-1,kd)) )*d22(2)
      wtamrt2(i1,i2,i3,kd)=(wtamr2(i1,i2,i3+1,kd)-wtamr2(i1,i2,i3-1,kd)
     & )*d12(2)
      wtamst2(i1,i2,i3,kd)=(wtams2(i1,i2,i3+1,kd)-wtams2(i1,i2,i3-1,kd)
     & )*d12(2)
      wtamrrr2(i1,i2,i3,kd)=(-2.*(wtam(i1+1,i2,i3,kd)-wtam(i1-1,i2,i3,
     & kd))+(wtam(i1+2,i2,i3,kd)-wtam(i1-2,i2,i3,kd)) )*d22(0)*d12(0)
      wtamsss2(i1,i2,i3,kd)=(-2.*(wtam(i1,i2+1,i3,kd)-wtam(i1,i2-1,i3,
     & kd))+(wtam(i1,i2+2,i3,kd)-wtam(i1,i2-2,i3,kd)) )*d22(1)*d12(1)
      wtamttt2(i1,i2,i3,kd)=(-2.*(wtam(i1,i2,i3+1,kd)-wtam(i1,i2,i3-1,
     & kd))+(wtam(i1,i2,i3+2,kd)-wtam(i1,i2,i3-2,kd)) )*d22(2)*d12(2)
      wtamx21(i1,i2,i3,kd)= rx(i1,i2,i3)*wtamr2(i1,i2,i3,kd)
      wtamy21(i1,i2,i3,kd)=0
      wtamz21(i1,i2,i3,kd)=0
      wtamx22(i1,i2,i3,kd)= rx(i1,i2,i3)*wtamr2(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*wtams2(i1,i2,i3,kd)
      wtamy22(i1,i2,i3,kd)= ry(i1,i2,i3)*wtamr2(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*wtams2(i1,i2,i3,kd)
      wtamz22(i1,i2,i3,kd)=0
      wtamx23(i1,i2,i3,kd)=rx(i1,i2,i3)*wtamr2(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*wtams2(i1,i2,i3,kd)+tx(i1,i2,i3)*wtamt2(i1,i2,i3,kd)
      wtamy23(i1,i2,i3,kd)=ry(i1,i2,i3)*wtamr2(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*wtams2(i1,i2,i3,kd)+ty(i1,i2,i3)*wtamt2(i1,i2,i3,kd)
      wtamz23(i1,i2,i3,kd)=rz(i1,i2,i3)*wtamr2(i1,i2,i3,kd)+sz(i1,i2,
     & i3)*wtams2(i1,i2,i3,kd)+tz(i1,i2,i3)*wtamt2(i1,i2,i3,kd)
      wtamxx21(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wtamrr2(i1,i2,i3,kd)+(
     & rxx22(i1,i2,i3))*wtamr2(i1,i2,i3,kd)
      wtamyy21(i1,i2,i3,kd)=0
      wtamxy21(i1,i2,i3,kd)=0
      wtamxz21(i1,i2,i3,kd)=0
      wtamyz21(i1,i2,i3,kd)=0
      wtamzz21(i1,i2,i3,kd)=0
      wtamlaplacian21(i1,i2,i3,kd)=wtamxx21(i1,i2,i3,kd)
      wtamxx22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wtamrr2(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*wtamrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*wtamss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3))*wtamr2(i1,i2,i3,kd)+(
     & sxx22(i1,i2,i3))*wtams2(i1,i2,i3,kd)
      wtamyy22(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*wtamrr2(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*wtamrs2(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*wtamss2(i1,i2,i3,kd)+(ryy22(i1,i2,i3))*wtamr2(i1,i2,i3,kd)+(
     & syy22(i1,i2,i3))*wtams2(i1,i2,i3,kd)
      wtamxy22(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wtamrr2(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & wtamrs2(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wtamss2(i1,i2,
     & i3,kd)+rxy22(i1,i2,i3)*wtamr2(i1,i2,i3,kd)+sxy22(i1,i2,i3)*
     & wtams2(i1,i2,i3,kd)
      wtamxz22(i1,i2,i3,kd)=0
      wtamyz22(i1,i2,i3,kd)=0
      wtamzz22(i1,i2,i3,kd)=0
      wtamlaplacian22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & wtamrr2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,
     & i3)*sy(i1,i2,i3))*wtamrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,
     & i2,i3)**2)*wtamss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3)+ryy22(i1,i2,
     & i3))*wtamr2(i1,i2,i3,kd)+(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*
     & wtams2(i1,i2,i3,kd)
      wtamxx23(i1,i2,i3,kd)=rx(i1,i2,i3)**2*wtamrr2(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*wtamss2(i1,i2,i3,kd)+tx(i1,i2,i3)**2*wtamtt2(i1,i2,
     & i3,kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*wtamrs2(i1,i2,i3,kd)+2.*rx(
     & i1,i2,i3)*tx(i1,i2,i3)*wtamrt2(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(
     & i1,i2,i3)*wtamst2(i1,i2,i3,kd)+rxx23(i1,i2,i3)*wtamr2(i1,i2,i3,
     & kd)+sxx23(i1,i2,i3)*wtams2(i1,i2,i3,kd)+txx23(i1,i2,i3)*wtamt2(
     & i1,i2,i3,kd)
      wtamyy23(i1,i2,i3,kd)=ry(i1,i2,i3)**2*wtamrr2(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*wtamss2(i1,i2,i3,kd)+ty(i1,i2,i3)**2*wtamtt2(i1,i2,
     & i3,kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*wtamrs2(i1,i2,i3,kd)+2.*ry(
     & i1,i2,i3)*ty(i1,i2,i3)*wtamrt2(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(
     & i1,i2,i3)*wtamst2(i1,i2,i3,kd)+ryy23(i1,i2,i3)*wtamr2(i1,i2,i3,
     & kd)+syy23(i1,i2,i3)*wtams2(i1,i2,i3,kd)+tyy23(i1,i2,i3)*wtamt2(
     & i1,i2,i3,kd)
      wtamzz23(i1,i2,i3,kd)=rz(i1,i2,i3)**2*wtamrr2(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*wtamss2(i1,i2,i3,kd)+tz(i1,i2,i3)**2*wtamtt2(i1,i2,
     & i3,kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*wtamrs2(i1,i2,i3,kd)+2.*rz(
     & i1,i2,i3)*tz(i1,i2,i3)*wtamrt2(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(
     & i1,i2,i3)*wtamst2(i1,i2,i3,kd)+rzz23(i1,i2,i3)*wtamr2(i1,i2,i3,
     & kd)+szz23(i1,i2,i3)*wtams2(i1,i2,i3,kd)+tzz23(i1,i2,i3)*wtamt2(
     & i1,i2,i3,kd)
      wtamxy23(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wtamrr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wtamss2(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *ty(i1,i2,i3)*wtamtt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+
     & ry(i1,i2,i3)*sx(i1,i2,i3))*wtamrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & ty(i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*wtamrt2(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*wtamst2(
     & i1,i2,i3,kd)+rxy23(i1,i2,i3)*wtamr2(i1,i2,i3,kd)+sxy23(i1,i2,
     & i3)*wtams2(i1,i2,i3,kd)+txy23(i1,i2,i3)*wtamt2(i1,i2,i3,kd)
      wtamxz23(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*wtamrr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*wtamss2(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *tz(i1,i2,i3)*wtamtt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sx(i1,i2,i3))*wtamrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*wtamrt2(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*wtamst2(
     & i1,i2,i3,kd)+rxz23(i1,i2,i3)*wtamr2(i1,i2,i3,kd)+sxz23(i1,i2,
     & i3)*wtams2(i1,i2,i3,kd)+txz23(i1,i2,i3)*wtamt2(i1,i2,i3,kd)
      wtamyz23(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*wtamrr2(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*wtamss2(i1,i2,i3,kd)+ty(i1,i2,i3)
     & *tz(i1,i2,i3)*wtamtt2(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sy(i1,i2,i3))*wtamrs2(i1,i2,i3,kd)+(ry(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*wtamrt2(i1,i2,i3,kd)+(
     & sy(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*wtamst2(
     & i1,i2,i3,kd)+ryz23(i1,i2,i3)*wtamr2(i1,i2,i3,kd)+syz23(i1,i2,
     & i3)*wtams2(i1,i2,i3,kd)+tyz23(i1,i2,i3)*wtamt2(i1,i2,i3,kd)
      wtamlaplacian23(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*wtamrr2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2+sz(i1,i2,i3)**2)*wtamss2(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+
     & ty(i1,i2,i3)**2+tz(i1,i2,i3)**2)*wtamtt2(i1,i2,i3,kd)+2.*(rx(
     & i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*
     & sz(i1,i2,i3))*wtamrs2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,
     & i3)+ ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*
     & wtamrt2(i1,i2,i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,
     & i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3))*wtamst2(i1,i2,i3,
     & kd)+(rxx23(i1,i2,i3)+ryy23(i1,i2,i3)+rzz23(i1,i2,i3))*wtamr2(
     & i1,i2,i3,kd)+(sxx23(i1,i2,i3)+syy23(i1,i2,i3)+szz23(i1,i2,i3))*
     & wtams2(i1,i2,i3,kd)+(txx23(i1,i2,i3)+tyy23(i1,i2,i3)+tzz23(i1,
     & i2,i3))*wtamt2(i1,i2,i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      wtamx23r(i1,i2,i3,kd)=(wtam(i1+1,i2,i3,kd)-wtam(i1-1,i2,i3,kd))*
     & h12(0)
      wtamy23r(i1,i2,i3,kd)=(wtam(i1,i2+1,i3,kd)-wtam(i1,i2-1,i3,kd))*
     & h12(1)
      wtamz23r(i1,i2,i3,kd)=(wtam(i1,i2,i3+1,kd)-wtam(i1,i2,i3-1,kd))*
     & h12(2)
      wtamxx23r(i1,i2,i3,kd)=(-2.*wtam(i1,i2,i3,kd)+(wtam(i1+1,i2,i3,
     & kd)+wtam(i1-1,i2,i3,kd)) )*h22(0)
      wtamyy23r(i1,i2,i3,kd)=(-2.*wtam(i1,i2,i3,kd)+(wtam(i1,i2+1,i3,
     & kd)+wtam(i1,i2-1,i3,kd)) )*h22(1)
      wtamxy23r(i1,i2,i3,kd)=(wtamx23r(i1,i2+1,i3,kd)-wtamx23r(i1,i2-1,
     & i3,kd))*h12(1)
      wtamzz23r(i1,i2,i3,kd)=(-2.*wtam(i1,i2,i3,kd)+(wtam(i1,i2,i3+1,
     & kd)+wtam(i1,i2,i3-1,kd)) )*h22(2)
      wtamxz23r(i1,i2,i3,kd)=(wtamx23r(i1,i2,i3+1,kd)-wtamx23r(i1,i2,
     & i3-1,kd))*h12(2)
      wtamyz23r(i1,i2,i3,kd)=(wtamy23r(i1,i2,i3+1,kd)-wtamy23r(i1,i2,
     & i3-1,kd))*h12(2)
      wtamx21r(i1,i2,i3,kd)= wtamx23r(i1,i2,i3,kd)
      wtamy21r(i1,i2,i3,kd)= wtamy23r(i1,i2,i3,kd)
      wtamz21r(i1,i2,i3,kd)= wtamz23r(i1,i2,i3,kd)
      wtamxx21r(i1,i2,i3,kd)= wtamxx23r(i1,i2,i3,kd)
      wtamyy21r(i1,i2,i3,kd)= wtamyy23r(i1,i2,i3,kd)
      wtamzz21r(i1,i2,i3,kd)= wtamzz23r(i1,i2,i3,kd)
      wtamxy21r(i1,i2,i3,kd)= wtamxy23r(i1,i2,i3,kd)
      wtamxz21r(i1,i2,i3,kd)= wtamxz23r(i1,i2,i3,kd)
      wtamyz21r(i1,i2,i3,kd)= wtamyz23r(i1,i2,i3,kd)
      wtamlaplacian21r(i1,i2,i3,kd)=wtamxx23r(i1,i2,i3,kd)
      wtamx22r(i1,i2,i3,kd)= wtamx23r(i1,i2,i3,kd)
      wtamy22r(i1,i2,i3,kd)= wtamy23r(i1,i2,i3,kd)
      wtamz22r(i1,i2,i3,kd)= wtamz23r(i1,i2,i3,kd)
      wtamxx22r(i1,i2,i3,kd)= wtamxx23r(i1,i2,i3,kd)
      wtamyy22r(i1,i2,i3,kd)= wtamyy23r(i1,i2,i3,kd)
      wtamzz22r(i1,i2,i3,kd)= wtamzz23r(i1,i2,i3,kd)
      wtamxy22r(i1,i2,i3,kd)= wtamxy23r(i1,i2,i3,kd)
      wtamxz22r(i1,i2,i3,kd)= wtamxz23r(i1,i2,i3,kd)
      wtamyz22r(i1,i2,i3,kd)= wtamyz23r(i1,i2,i3,kd)
      wtamlaplacian22r(i1,i2,i3,kd)=wtamxx23r(i1,i2,i3,kd)+wtamyy23r(
     & i1,i2,i3,kd)
      wtamlaplacian23r(i1,i2,i3,kd)=wtamxx23r(i1,i2,i3,kd)+wtamyy23r(
     & i1,i2,i3,kd)+wtamzz23r(i1,i2,i3,kd)
      wtamxxx22r(i1,i2,i3,kd)=(-2.*(wtam(i1+1,i2,i3,kd)-wtam(i1-1,i2,
     & i3,kd))+(wtam(i1+2,i2,i3,kd)-wtam(i1-2,i2,i3,kd)) )*h22(0)*h12(
     & 0)
      wtamyyy22r(i1,i2,i3,kd)=(-2.*(wtam(i1,i2+1,i3,kd)-wtam(i1,i2-1,
     & i3,kd))+(wtam(i1,i2+2,i3,kd)-wtam(i1,i2-2,i3,kd)) )*h22(1)*h12(
     & 1)
      wtamxxy22r(i1,i2,i3,kd)=( wtamxx22r(i1,i2+1,i3,kd)-wtamxx22r(i1,
     & i2-1,i3,kd))/(2.*dx(1))
      wtamxyy22r(i1,i2,i3,kd)=( wtamyy22r(i1+1,i2,i3,kd)-wtamyy22r(i1-
     & 1,i2,i3,kd))/(2.*dx(0))
      wtamxxxx22r(i1,i2,i3,kd)=(6.*wtam(i1,i2,i3,kd)-4.*(wtam(i1+1,i2,
     & i3,kd)+wtam(i1-1,i2,i3,kd))+(wtam(i1+2,i2,i3,kd)+wtam(i1-2,i2,
     & i3,kd)) )/(dx(0)**4)
      wtamyyyy22r(i1,i2,i3,kd)=(6.*wtam(i1,i2,i3,kd)-4.*(wtam(i1,i2+1,
     & i3,kd)+wtam(i1,i2-1,i3,kd))+(wtam(i1,i2+2,i3,kd)+wtam(i1,i2-2,
     & i3,kd)) )/(dx(1)**4)
      wtamxxyy22r(i1,i2,i3,kd)=( 4.*wtam(i1,i2,i3,kd)     -2.*(wtam(i1+
     & 1,i2,i3,kd)+wtam(i1-1,i2,i3,kd)+wtam(i1,i2+1,i3,kd)+wtam(i1,i2-
     & 1,i3,kd))   +   (wtam(i1+1,i2+1,i3,kd)+wtam(i1-1,i2+1,i3,kd)+
     & wtam(i1+1,i2-1,i3,kd)+wtam(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)*
     & *2)
      ! 2D laplacian squared = wtam.xxxx + 2 wtam.xxyy + wtam.yyyy
      wtamLapSq22r(i1,i2,i3,kd)= ( 6.*wtam(i1,i2,i3,kd)   - 4.*(wtam(
     & i1+1,i2,i3,kd)+wtam(i1-1,i2,i3,kd))    +(wtam(i1+2,i2,i3,kd)+
     & wtam(i1-2,i2,i3,kd)) )/(dx(0)**4) +( 6.*wtam(i1,i2,i3,kd)    -
     & 4.*(wtam(i1,i2+1,i3,kd)+wtam(i1,i2-1,i3,kd))    +(wtam(i1,i2+2,
     & i3,kd)+wtam(i1,i2-2,i3,kd)) )/(dx(1)**4)  +( 8.*wtam(i1,i2,i3,
     & kd)     -4.*(wtam(i1+1,i2,i3,kd)+wtam(i1-1,i2,i3,kd)+wtam(i1,
     & i2+1,i3,kd)+wtam(i1,i2-1,i3,kd))   +2.*(wtam(i1+1,i2+1,i3,kd)+
     & wtam(i1-1,i2+1,i3,kd)+wtam(i1+1,i2-1,i3,kd)+wtam(i1-1,i2-1,i3,
     & kd)) )/(dx(0)**2*dx(1)**2)
      wtamxxx23r(i1,i2,i3,kd)=(-2.*(wtam(i1+1,i2,i3,kd)-wtam(i1-1,i2,
     & i3,kd))+(wtam(i1+2,i2,i3,kd)-wtam(i1-2,i2,i3,kd)) )*h22(0)*h12(
     & 0)
      wtamyyy23r(i1,i2,i3,kd)=(-2.*(wtam(i1,i2+1,i3,kd)-wtam(i1,i2-1,
     & i3,kd))+(wtam(i1,i2+2,i3,kd)-wtam(i1,i2-2,i3,kd)) )*h22(1)*h12(
     & 1)
      wtamzzz23r(i1,i2,i3,kd)=(-2.*(wtam(i1,i2,i3+1,kd)-wtam(i1,i2,i3-
     & 1,kd))+(wtam(i1,i2,i3+2,kd)-wtam(i1,i2,i3-2,kd)) )*h22(1)*h12(
     & 2)
      wtamxxy23r(i1,i2,i3,kd)=( wtamxx22r(i1,i2+1,i3,kd)-wtamxx22r(i1,
     & i2-1,i3,kd))/(2.*dx(1))
      wtamxyy23r(i1,i2,i3,kd)=( wtamyy22r(i1+1,i2,i3,kd)-wtamyy22r(i1-
     & 1,i2,i3,kd))/(2.*dx(0))
      wtamxxz23r(i1,i2,i3,kd)=( wtamxx22r(i1,i2,i3+1,kd)-wtamxx22r(i1,
     & i2,i3-1,kd))/(2.*dx(2))
      wtamyyz23r(i1,i2,i3,kd)=( wtamyy22r(i1,i2,i3+1,kd)-wtamyy22r(i1,
     & i2,i3-1,kd))/(2.*dx(2))
      wtamxzz23r(i1,i2,i3,kd)=( wtamzz22r(i1+1,i2,i3,kd)-wtamzz22r(i1-
     & 1,i2,i3,kd))/(2.*dx(0))
      wtamyzz23r(i1,i2,i3,kd)=( wtamzz22r(i1,i2+1,i3,kd)-wtamzz22r(i1,
     & i2-1,i3,kd))/(2.*dx(1))
      wtamxxxx23r(i1,i2,i3,kd)=(6.*wtam(i1,i2,i3,kd)-4.*(wtam(i1+1,i2,
     & i3,kd)+wtam(i1-1,i2,i3,kd))+(wtam(i1+2,i2,i3,kd)+wtam(i1-2,i2,
     & i3,kd)) )/(dx(0)**4)
      wtamyyyy23r(i1,i2,i3,kd)=(6.*wtam(i1,i2,i3,kd)-4.*(wtam(i1,i2+1,
     & i3,kd)+wtam(i1,i2-1,i3,kd))+(wtam(i1,i2+2,i3,kd)+wtam(i1,i2-2,
     & i3,kd)) )/(dx(1)**4)
      wtamzzzz23r(i1,i2,i3,kd)=(6.*wtam(i1,i2,i3,kd)-4.*(wtam(i1,i2,i3+
     & 1,kd)+wtam(i1,i2,i3-1,kd))+(wtam(i1,i2,i3+2,kd)+wtam(i1,i2,i3-
     & 2,kd)) )/(dx(2)**4)
      wtamxxyy23r(i1,i2,i3,kd)=( 4.*wtam(i1,i2,i3,kd)     -2.*(wtam(i1+
     & 1,i2,i3,kd)+wtam(i1-1,i2,i3,kd)+wtam(i1,i2+1,i3,kd)+wtam(i1,i2-
     & 1,i3,kd))   +   (wtam(i1+1,i2+1,i3,kd)+wtam(i1-1,i2+1,i3,kd)+
     & wtam(i1+1,i2-1,i3,kd)+wtam(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)*
     & *2)
      wtamxxzz23r(i1,i2,i3,kd)=( 4.*wtam(i1,i2,i3,kd)     -2.*(wtam(i1+
     & 1,i2,i3,kd)+wtam(i1-1,i2,i3,kd)+wtam(i1,i2,i3+1,kd)+wtam(i1,i2,
     & i3-1,kd))   +   (wtam(i1+1,i2,i3+1,kd)+wtam(i1-1,i2,i3+1,kd)+
     & wtam(i1+1,i2,i3-1,kd)+wtam(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)*
     & *2)
      wtamyyzz23r(i1,i2,i3,kd)=( 4.*wtam(i1,i2,i3,kd)     -2.*(wtam(i1,
     & i2+1,i3,kd)  +wtam(i1,i2-1,i3,kd)+  wtam(i1,i2  ,i3+1,kd)+wtam(
     & i1,i2  ,i3-1,kd))   +   (wtam(i1,i2+1,i3+1,kd)+wtam(i1,i2-1,i3+
     & 1,kd)+wtam(i1,i2+1,i3-1,kd)+wtam(i1,i2-1,i3-1,kd)) )/(dx(1)**2*
     & dx(2)**2)
      ! 3D laplacian squared = wtam.xxxx + wtam.yyyy + wtam.zzzz + 2 (wtam.xxyy + wtam.xxzz + wtam.yyzz )
      wtamLapSq23r(i1,i2,i3,kd)= ( 6.*wtam(i1,i2,i3,kd)   - 4.*(wtam(
     & i1+1,i2,i3,kd)+wtam(i1-1,i2,i3,kd))    +(wtam(i1+2,i2,i3,kd)+
     & wtam(i1-2,i2,i3,kd)) )/(dx(0)**4) +( 6.*wtam(i1,i2,i3,kd)    -
     & 4.*(wtam(i1,i2+1,i3,kd)+wtam(i1,i2-1,i3,kd))    +(wtam(i1,i2+2,
     & i3,kd)+wtam(i1,i2-2,i3,kd)) )/(dx(1)**4)  +( 6.*wtam(i1,i2,i3,
     & kd)    -4.*(wtam(i1,i2,i3+1,kd)+wtam(i1,i2,i3-1,kd))    +(wtam(
     & i1,i2,i3+2,kd)+wtam(i1,i2,i3-2,kd)) )/(dx(2)**4)  +( 8.*wtam(
     & i1,i2,i3,kd)     -4.*(wtam(i1+1,i2,i3,kd)  +wtam(i1-1,i2,i3,kd)
     &   +wtam(i1  ,i2+1,i3,kd)+wtam(i1  ,i2-1,i3,kd))   +2.*(wtam(i1+
     & 1,i2+1,i3,kd)+wtam(i1-1,i2+1,i3,kd)+wtam(i1+1,i2-1,i3,kd)+wtam(
     & i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)+( 8.*wtam(i1,i2,i3,kd) 
     &     -4.*(wtam(i1+1,i2,i3,kd)  +wtam(i1-1,i2,i3,kd)  +wtam(i1  ,
     & i2,i3+1,kd)+wtam(i1  ,i2,i3-1,kd))   +2.*(wtam(i1+1,i2,i3+1,kd)
     & +wtam(i1-1,i2,i3+1,kd)+wtam(i1+1,i2,i3-1,kd)+wtam(i1-1,i2,i3-1,
     & kd)) )/(dx(0)**2*dx(2)**2)+( 8.*wtam(i1,i2,i3,kd)     -4.*(
     & wtam(i1,i2+1,i3,kd)  +wtam(i1,i2-1,i3,kd)  +wtam(i1,i2  ,i3+1,
     & kd)+wtam(i1,i2  ,i3-1,kd))   +2.*(wtam(i1,i2+1,i3+1,kd)+wtam(
     & i1,i2-1,i3+1,kd)+wtam(i1,i2+1,i3-1,kd)+wtam(i1,i2-1,i3-1,kd)) )
     & /(dx(1)**2*dx(2)**2)

      vtbr2(i1,i2,i3,kd)=(vtb(i1+1,i2,i3,kd)-vtb(i1-1,i2,i3,kd))*d12(0)
      vtbs2(i1,i2,i3,kd)=(vtb(i1,i2+1,i3,kd)-vtb(i1,i2-1,i3,kd))*d12(1)
      vtbt2(i1,i2,i3,kd)=(vtb(i1,i2,i3+1,kd)-vtb(i1,i2,i3-1,kd))*d12(2)
      vtbrr2(i1,i2,i3,kd)=(-2.*vtb(i1,i2,i3,kd)+(vtb(i1+1,i2,i3,kd)+
     & vtb(i1-1,i2,i3,kd)) )*d22(0)
      vtbss2(i1,i2,i3,kd)=(-2.*vtb(i1,i2,i3,kd)+(vtb(i1,i2+1,i3,kd)+
     & vtb(i1,i2-1,i3,kd)) )*d22(1)
      vtbrs2(i1,i2,i3,kd)=(vtbr2(i1,i2+1,i3,kd)-vtbr2(i1,i2-1,i3,kd))*
     & d12(1)
      vtbtt2(i1,i2,i3,kd)=(-2.*vtb(i1,i2,i3,kd)+(vtb(i1,i2,i3+1,kd)+
     & vtb(i1,i2,i3-1,kd)) )*d22(2)
      vtbrt2(i1,i2,i3,kd)=(vtbr2(i1,i2,i3+1,kd)-vtbr2(i1,i2,i3-1,kd))*
     & d12(2)
      vtbst2(i1,i2,i3,kd)=(vtbs2(i1,i2,i3+1,kd)-vtbs2(i1,i2,i3-1,kd))*
     & d12(2)
      vtbrrr2(i1,i2,i3,kd)=(-2.*(vtb(i1+1,i2,i3,kd)-vtb(i1-1,i2,i3,kd))
     & +(vtb(i1+2,i2,i3,kd)-vtb(i1-2,i2,i3,kd)) )*d22(0)*d12(0)
      vtbsss2(i1,i2,i3,kd)=(-2.*(vtb(i1,i2+1,i3,kd)-vtb(i1,i2-1,i3,kd))
     & +(vtb(i1,i2+2,i3,kd)-vtb(i1,i2-2,i3,kd)) )*d22(1)*d12(1)
      vtbttt2(i1,i2,i3,kd)=(-2.*(vtb(i1,i2,i3+1,kd)-vtb(i1,i2,i3-1,kd))
     & +(vtb(i1,i2,i3+2,kd)-vtb(i1,i2,i3-2,kd)) )*d22(2)*d12(2)
      vtbx21(i1,i2,i3,kd)= rx(i1,i2,i3)*vtbr2(i1,i2,i3,kd)
      vtby21(i1,i2,i3,kd)=0
      vtbz21(i1,i2,i3,kd)=0
      vtbx22(i1,i2,i3,kd)= rx(i1,i2,i3)*vtbr2(i1,i2,i3,kd)+sx(i1,i2,i3)
     & *vtbs2(i1,i2,i3,kd)
      vtby22(i1,i2,i3,kd)= ry(i1,i2,i3)*vtbr2(i1,i2,i3,kd)+sy(i1,i2,i3)
     & *vtbs2(i1,i2,i3,kd)
      vtbz22(i1,i2,i3,kd)=0
      vtbx23(i1,i2,i3,kd)=rx(i1,i2,i3)*vtbr2(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & vtbs2(i1,i2,i3,kd)+tx(i1,i2,i3)*vtbt2(i1,i2,i3,kd)
      vtby23(i1,i2,i3,kd)=ry(i1,i2,i3)*vtbr2(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & vtbs2(i1,i2,i3,kd)+ty(i1,i2,i3)*vtbt2(i1,i2,i3,kd)
      vtbz23(i1,i2,i3,kd)=rz(i1,i2,i3)*vtbr2(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & vtbs2(i1,i2,i3,kd)+tz(i1,i2,i3)*vtbt2(i1,i2,i3,kd)
      vtbxx21(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vtbrr2(i1,i2,i3,kd)+(
     & rxx22(i1,i2,i3))*vtbr2(i1,i2,i3,kd)
      vtbyy21(i1,i2,i3,kd)=0
      vtbxy21(i1,i2,i3,kd)=0
      vtbxz21(i1,i2,i3,kd)=0
      vtbyz21(i1,i2,i3,kd)=0
      vtbzz21(i1,i2,i3,kd)=0
      vtblaplacian21(i1,i2,i3,kd)=vtbxx21(i1,i2,i3,kd)
      vtbxx22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vtbrr2(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*vtbrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*vtbss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3))*vtbr2(i1,i2,i3,kd)+(
     & sxx22(i1,i2,i3))*vtbs2(i1,i2,i3,kd)
      vtbyy22(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*vtbrr2(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*vtbrs2(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*vtbss2(i1,i2,i3,kd)+(ryy22(i1,i2,i3))*vtbr2(i1,i2,i3,kd)+(
     & syy22(i1,i2,i3))*vtbs2(i1,i2,i3,kd)
      vtbxy22(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vtbrr2(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & vtbrs2(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vtbss2(i1,i2,i3,
     & kd)+rxy22(i1,i2,i3)*vtbr2(i1,i2,i3,kd)+sxy22(i1,i2,i3)*vtbs2(
     & i1,i2,i3,kd)
      vtbxz22(i1,i2,i3,kd)=0
      vtbyz22(i1,i2,i3,kd)=0
      vtbzz22(i1,i2,i3,kd)=0
      vtblaplacian22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & vtbrr2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)
     & *sy(i1,i2,i3))*vtbrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2)*vtbss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3)+ryy22(i1,i2,i3))*
     & vtbr2(i1,i2,i3,kd)+(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*vtbs2(i1,
     & i2,i3,kd)
      vtbxx23(i1,i2,i3,kd)=rx(i1,i2,i3)**2*vtbrr2(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*vtbss2(i1,i2,i3,kd)+tx(i1,i2,i3)**2*vtbtt2(i1,i2,i3,
     & kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*vtbrs2(i1,i2,i3,kd)+2.*rx(i1,
     & i2,i3)*tx(i1,i2,i3)*vtbrt2(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,
     & i2,i3)*vtbst2(i1,i2,i3,kd)+rxx23(i1,i2,i3)*vtbr2(i1,i2,i3,kd)+
     & sxx23(i1,i2,i3)*vtbs2(i1,i2,i3,kd)+txx23(i1,i2,i3)*vtbt2(i1,i2,
     & i3,kd)
      vtbyy23(i1,i2,i3,kd)=ry(i1,i2,i3)**2*vtbrr2(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*vtbss2(i1,i2,i3,kd)+ty(i1,i2,i3)**2*vtbtt2(i1,i2,i3,
     & kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*vtbrs2(i1,i2,i3,kd)+2.*ry(i1,
     & i2,i3)*ty(i1,i2,i3)*vtbrt2(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,
     & i2,i3)*vtbst2(i1,i2,i3,kd)+ryy23(i1,i2,i3)*vtbr2(i1,i2,i3,kd)+
     & syy23(i1,i2,i3)*vtbs2(i1,i2,i3,kd)+tyy23(i1,i2,i3)*vtbt2(i1,i2,
     & i3,kd)
      vtbzz23(i1,i2,i3,kd)=rz(i1,i2,i3)**2*vtbrr2(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*vtbss2(i1,i2,i3,kd)+tz(i1,i2,i3)**2*vtbtt2(i1,i2,i3,
     & kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*vtbrs2(i1,i2,i3,kd)+2.*rz(i1,
     & i2,i3)*tz(i1,i2,i3)*vtbrt2(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,
     & i2,i3)*vtbst2(i1,i2,i3,kd)+rzz23(i1,i2,i3)*vtbr2(i1,i2,i3,kd)+
     & szz23(i1,i2,i3)*vtbs2(i1,i2,i3,kd)+tzz23(i1,i2,i3)*vtbt2(i1,i2,
     & i3,kd)
      vtbxy23(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vtbrr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vtbss2(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & ty(i1,i2,i3)*vtbtt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(
     & i1,i2,i3)*sx(i1,i2,i3))*vtbrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(
     & i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*vtbrt2(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*vtbst2(i1,i2,
     & i3,kd)+rxy23(i1,i2,i3)*vtbr2(i1,i2,i3,kd)+sxy23(i1,i2,i3)*
     & vtbs2(i1,i2,i3,kd)+txy23(i1,i2,i3)*vtbt2(i1,i2,i3,kd)
      vtbxz23(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*vtbrr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*vtbss2(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & tz(i1,i2,i3)*vtbtt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sx(i1,i2,i3))*vtbrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*vtbrt2(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*vtbst2(i1,i2,
     & i3,kd)+rxz23(i1,i2,i3)*vtbr2(i1,i2,i3,kd)+sxz23(i1,i2,i3)*
     & vtbs2(i1,i2,i3,kd)+txz23(i1,i2,i3)*vtbt2(i1,i2,i3,kd)
      vtbyz23(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*vtbrr2(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*vtbss2(i1,i2,i3,kd)+ty(i1,i2,i3)*
     & tz(i1,i2,i3)*vtbtt2(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sy(i1,i2,i3))*vtbrs2(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*vtbrt2(i1,i2,i3,kd)+(sy(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*vtbst2(i1,i2,
     & i3,kd)+ryz23(i1,i2,i3)*vtbr2(i1,i2,i3,kd)+syz23(i1,i2,i3)*
     & vtbs2(i1,i2,i3,kd)+tyz23(i1,i2,i3)*vtbt2(i1,i2,i3,kd)
      vtblaplacian23(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*vtbrr2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)
     & **2+sz(i1,i2,i3)**2)*vtbss2(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(
     & i1,i2,i3)**2+tz(i1,i2,i3)**2)*vtbtt2(i1,i2,i3,kd)+2.*(rx(i1,i2,
     & i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,
     & i2,i3))*vtbrs2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(
     & i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*vtbrt2(i1,i2,
     & i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)
     & +sz(i1,i2,i3)*tz(i1,i2,i3))*vtbst2(i1,i2,i3,kd)+(rxx23(i1,i2,
     & i3)+ryy23(i1,i2,i3)+rzz23(i1,i2,i3))*vtbr2(i1,i2,i3,kd)+(sxx23(
     & i1,i2,i3)+syy23(i1,i2,i3)+szz23(i1,i2,i3))*vtbs2(i1,i2,i3,kd)+(
     & txx23(i1,i2,i3)+tyy23(i1,i2,i3)+tzz23(i1,i2,i3))*vtbt2(i1,i2,
     & i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      vtbx23r(i1,i2,i3,kd)=(vtb(i1+1,i2,i3,kd)-vtb(i1-1,i2,i3,kd))*h12(
     & 0)
      vtby23r(i1,i2,i3,kd)=(vtb(i1,i2+1,i3,kd)-vtb(i1,i2-1,i3,kd))*h12(
     & 1)
      vtbz23r(i1,i2,i3,kd)=(vtb(i1,i2,i3+1,kd)-vtb(i1,i2,i3-1,kd))*h12(
     & 2)
      vtbxx23r(i1,i2,i3,kd)=(-2.*vtb(i1,i2,i3,kd)+(vtb(i1+1,i2,i3,kd)+
     & vtb(i1-1,i2,i3,kd)) )*h22(0)
      vtbyy23r(i1,i2,i3,kd)=(-2.*vtb(i1,i2,i3,kd)+(vtb(i1,i2+1,i3,kd)+
     & vtb(i1,i2-1,i3,kd)) )*h22(1)
      vtbxy23r(i1,i2,i3,kd)=(vtbx23r(i1,i2+1,i3,kd)-vtbx23r(i1,i2-1,i3,
     & kd))*h12(1)
      vtbzz23r(i1,i2,i3,kd)=(-2.*vtb(i1,i2,i3,kd)+(vtb(i1,i2,i3+1,kd)+
     & vtb(i1,i2,i3-1,kd)) )*h22(2)
      vtbxz23r(i1,i2,i3,kd)=(vtbx23r(i1,i2,i3+1,kd)-vtbx23r(i1,i2,i3-1,
     & kd))*h12(2)
      vtbyz23r(i1,i2,i3,kd)=(vtby23r(i1,i2,i3+1,kd)-vtby23r(i1,i2,i3-1,
     & kd))*h12(2)
      vtbx21r(i1,i2,i3,kd)= vtbx23r(i1,i2,i3,kd)
      vtby21r(i1,i2,i3,kd)= vtby23r(i1,i2,i3,kd)
      vtbz21r(i1,i2,i3,kd)= vtbz23r(i1,i2,i3,kd)
      vtbxx21r(i1,i2,i3,kd)= vtbxx23r(i1,i2,i3,kd)
      vtbyy21r(i1,i2,i3,kd)= vtbyy23r(i1,i2,i3,kd)
      vtbzz21r(i1,i2,i3,kd)= vtbzz23r(i1,i2,i3,kd)
      vtbxy21r(i1,i2,i3,kd)= vtbxy23r(i1,i2,i3,kd)
      vtbxz21r(i1,i2,i3,kd)= vtbxz23r(i1,i2,i3,kd)
      vtbyz21r(i1,i2,i3,kd)= vtbyz23r(i1,i2,i3,kd)
      vtblaplacian21r(i1,i2,i3,kd)=vtbxx23r(i1,i2,i3,kd)
      vtbx22r(i1,i2,i3,kd)= vtbx23r(i1,i2,i3,kd)
      vtby22r(i1,i2,i3,kd)= vtby23r(i1,i2,i3,kd)
      vtbz22r(i1,i2,i3,kd)= vtbz23r(i1,i2,i3,kd)
      vtbxx22r(i1,i2,i3,kd)= vtbxx23r(i1,i2,i3,kd)
      vtbyy22r(i1,i2,i3,kd)= vtbyy23r(i1,i2,i3,kd)
      vtbzz22r(i1,i2,i3,kd)= vtbzz23r(i1,i2,i3,kd)
      vtbxy22r(i1,i2,i3,kd)= vtbxy23r(i1,i2,i3,kd)
      vtbxz22r(i1,i2,i3,kd)= vtbxz23r(i1,i2,i3,kd)
      vtbyz22r(i1,i2,i3,kd)= vtbyz23r(i1,i2,i3,kd)
      vtblaplacian22r(i1,i2,i3,kd)=vtbxx23r(i1,i2,i3,kd)+vtbyy23r(i1,
     & i2,i3,kd)
      vtblaplacian23r(i1,i2,i3,kd)=vtbxx23r(i1,i2,i3,kd)+vtbyy23r(i1,
     & i2,i3,kd)+vtbzz23r(i1,i2,i3,kd)
      vtbxxx22r(i1,i2,i3,kd)=(-2.*(vtb(i1+1,i2,i3,kd)-vtb(i1-1,i2,i3,
     & kd))+(vtb(i1+2,i2,i3,kd)-vtb(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
      vtbyyy22r(i1,i2,i3,kd)=(-2.*(vtb(i1,i2+1,i3,kd)-vtb(i1,i2-1,i3,
     & kd))+(vtb(i1,i2+2,i3,kd)-vtb(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
      vtbxxy22r(i1,i2,i3,kd)=( vtbxx22r(i1,i2+1,i3,kd)-vtbxx22r(i1,i2-
     & 1,i3,kd))/(2.*dx(1))
      vtbxyy22r(i1,i2,i3,kd)=( vtbyy22r(i1+1,i2,i3,kd)-vtbyy22r(i1-1,
     & i2,i3,kd))/(2.*dx(0))
      vtbxxxx22r(i1,i2,i3,kd)=(6.*vtb(i1,i2,i3,kd)-4.*(vtb(i1+1,i2,i3,
     & kd)+vtb(i1-1,i2,i3,kd))+(vtb(i1+2,i2,i3,kd)+vtb(i1-2,i2,i3,kd))
     &  )/(dx(0)**4)
      vtbyyyy22r(i1,i2,i3,kd)=(6.*vtb(i1,i2,i3,kd)-4.*(vtb(i1,i2+1,i3,
     & kd)+vtb(i1,i2-1,i3,kd))+(vtb(i1,i2+2,i3,kd)+vtb(i1,i2-2,i3,kd))
     &  )/(dx(1)**4)
      vtbxxyy22r(i1,i2,i3,kd)=( 4.*vtb(i1,i2,i3,kd)     -2.*(vtb(i1+1,
     & i2,i3,kd)+vtb(i1-1,i2,i3,kd)+vtb(i1,i2+1,i3,kd)+vtb(i1,i2-1,i3,
     & kd))   +   (vtb(i1+1,i2+1,i3,kd)+vtb(i1-1,i2+1,i3,kd)+vtb(i1+1,
     & i2-1,i3,kd)+vtb(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      ! 2D laplacian squared = vtb.xxxx + 2 vtb.xxyy + vtb.yyyy
      vtbLapSq22r(i1,i2,i3,kd)= ( 6.*vtb(i1,i2,i3,kd)   - 4.*(vtb(i1+1,
     & i2,i3,kd)+vtb(i1-1,i2,i3,kd))    +(vtb(i1+2,i2,i3,kd)+vtb(i1-2,
     & i2,i3,kd)) )/(dx(0)**4) +( 6.*vtb(i1,i2,i3,kd)    -4.*(vtb(i1,
     & i2+1,i3,kd)+vtb(i1,i2-1,i3,kd))    +(vtb(i1,i2+2,i3,kd)+vtb(i1,
     & i2-2,i3,kd)) )/(dx(1)**4)  +( 8.*vtb(i1,i2,i3,kd)     -4.*(vtb(
     & i1+1,i2,i3,kd)+vtb(i1-1,i2,i3,kd)+vtb(i1,i2+1,i3,kd)+vtb(i1,i2-
     & 1,i3,kd))   +2.*(vtb(i1+1,i2+1,i3,kd)+vtb(i1-1,i2+1,i3,kd)+vtb(
     & i1+1,i2-1,i3,kd)+vtb(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      vtbxxx23r(i1,i2,i3,kd)=(-2.*(vtb(i1+1,i2,i3,kd)-vtb(i1-1,i2,i3,
     & kd))+(vtb(i1+2,i2,i3,kd)-vtb(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
      vtbyyy23r(i1,i2,i3,kd)=(-2.*(vtb(i1,i2+1,i3,kd)-vtb(i1,i2-1,i3,
     & kd))+(vtb(i1,i2+2,i3,kd)-vtb(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
      vtbzzz23r(i1,i2,i3,kd)=(-2.*(vtb(i1,i2,i3+1,kd)-vtb(i1,i2,i3-1,
     & kd))+(vtb(i1,i2,i3+2,kd)-vtb(i1,i2,i3-2,kd)) )*h22(1)*h12(2)
      vtbxxy23r(i1,i2,i3,kd)=( vtbxx22r(i1,i2+1,i3,kd)-vtbxx22r(i1,i2-
     & 1,i3,kd))/(2.*dx(1))
      vtbxyy23r(i1,i2,i3,kd)=( vtbyy22r(i1+1,i2,i3,kd)-vtbyy22r(i1-1,
     & i2,i3,kd))/(2.*dx(0))
      vtbxxz23r(i1,i2,i3,kd)=( vtbxx22r(i1,i2,i3+1,kd)-vtbxx22r(i1,i2,
     & i3-1,kd))/(2.*dx(2))
      vtbyyz23r(i1,i2,i3,kd)=( vtbyy22r(i1,i2,i3+1,kd)-vtbyy22r(i1,i2,
     & i3-1,kd))/(2.*dx(2))
      vtbxzz23r(i1,i2,i3,kd)=( vtbzz22r(i1+1,i2,i3,kd)-vtbzz22r(i1-1,
     & i2,i3,kd))/(2.*dx(0))
      vtbyzz23r(i1,i2,i3,kd)=( vtbzz22r(i1,i2+1,i3,kd)-vtbzz22r(i1,i2-
     & 1,i3,kd))/(2.*dx(1))
      vtbxxxx23r(i1,i2,i3,kd)=(6.*vtb(i1,i2,i3,kd)-4.*(vtb(i1+1,i2,i3,
     & kd)+vtb(i1-1,i2,i3,kd))+(vtb(i1+2,i2,i3,kd)+vtb(i1-2,i2,i3,kd))
     &  )/(dx(0)**4)
      vtbyyyy23r(i1,i2,i3,kd)=(6.*vtb(i1,i2,i3,kd)-4.*(vtb(i1,i2+1,i3,
     & kd)+vtb(i1,i2-1,i3,kd))+(vtb(i1,i2+2,i3,kd)+vtb(i1,i2-2,i3,kd))
     &  )/(dx(1)**4)
      vtbzzzz23r(i1,i2,i3,kd)=(6.*vtb(i1,i2,i3,kd)-4.*(vtb(i1,i2,i3+1,
     & kd)+vtb(i1,i2,i3-1,kd))+(vtb(i1,i2,i3+2,kd)+vtb(i1,i2,i3-2,kd))
     &  )/(dx(2)**4)
      vtbxxyy23r(i1,i2,i3,kd)=( 4.*vtb(i1,i2,i3,kd)     -2.*(vtb(i1+1,
     & i2,i3,kd)+vtb(i1-1,i2,i3,kd)+vtb(i1,i2+1,i3,kd)+vtb(i1,i2-1,i3,
     & kd))   +   (vtb(i1+1,i2+1,i3,kd)+vtb(i1-1,i2+1,i3,kd)+vtb(i1+1,
     & i2-1,i3,kd)+vtb(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      vtbxxzz23r(i1,i2,i3,kd)=( 4.*vtb(i1,i2,i3,kd)     -2.*(vtb(i1+1,
     & i2,i3,kd)+vtb(i1-1,i2,i3,kd)+vtb(i1,i2,i3+1,kd)+vtb(i1,i2,i3-1,
     & kd))   +   (vtb(i1+1,i2,i3+1,kd)+vtb(i1-1,i2,i3+1,kd)+vtb(i1+1,
     & i2,i3-1,kd)+vtb(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)
      vtbyyzz23r(i1,i2,i3,kd)=( 4.*vtb(i1,i2,i3,kd)     -2.*(vtb(i1,i2+
     & 1,i3,kd)  +vtb(i1,i2-1,i3,kd)+  vtb(i1,i2  ,i3+1,kd)+vtb(i1,i2 
     &  ,i3-1,kd))   +   (vtb(i1,i2+1,i3+1,kd)+vtb(i1,i2-1,i3+1,kd)+
     & vtb(i1,i2+1,i3-1,kd)+vtb(i1,i2-1,i3-1,kd)) )/(dx(1)**2*dx(2)**
     & 2)
      ! 3D laplacian squared = vtb.xxxx + vtb.yyyy + vtb.zzzz + 2 (vtb.xxyy + vtb.xxzz + vtb.yyzz )
      vtbLapSq23r(i1,i2,i3,kd)= ( 6.*vtb(i1,i2,i3,kd)   - 4.*(vtb(i1+1,
     & i2,i3,kd)+vtb(i1-1,i2,i3,kd))    +(vtb(i1+2,i2,i3,kd)+vtb(i1-2,
     & i2,i3,kd)) )/(dx(0)**4) +( 6.*vtb(i1,i2,i3,kd)    -4.*(vtb(i1,
     & i2+1,i3,kd)+vtb(i1,i2-1,i3,kd))    +(vtb(i1,i2+2,i3,kd)+vtb(i1,
     & i2-2,i3,kd)) )/(dx(1)**4)  +( 6.*vtb(i1,i2,i3,kd)    -4.*(vtb(
     & i1,i2,i3+1,kd)+vtb(i1,i2,i3-1,kd))    +(vtb(i1,i2,i3+2,kd)+vtb(
     & i1,i2,i3-2,kd)) )/(dx(2)**4)  +( 8.*vtb(i1,i2,i3,kd)     -4.*(
     & vtb(i1+1,i2,i3,kd)  +vtb(i1-1,i2,i3,kd)  +vtb(i1  ,i2+1,i3,kd)+
     & vtb(i1  ,i2-1,i3,kd))   +2.*(vtb(i1+1,i2+1,i3,kd)+vtb(i1-1,i2+
     & 1,i3,kd)+vtb(i1+1,i2-1,i3,kd)+vtb(i1-1,i2-1,i3,kd)) )/(dx(0)**
     & 2*dx(1)**2)+( 8.*vtb(i1,i2,i3,kd)     -4.*(vtb(i1+1,i2,i3,kd)  
     & +vtb(i1-1,i2,i3,kd)  +vtb(i1  ,i2,i3+1,kd)+vtb(i1  ,i2,i3-1,kd)
     & )   +2.*(vtb(i1+1,i2,i3+1,kd)+vtb(i1-1,i2,i3+1,kd)+vtb(i1+1,i2,
     & i3-1,kd)+vtb(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)+( 8.*vtb(
     & i1,i2,i3,kd)     -4.*(vtb(i1,i2+1,i3,kd)  +vtb(i1,i2-1,i3,kd)  
     & +vtb(i1,i2  ,i3+1,kd)+vtb(i1,i2  ,i3-1,kd))   +2.*(vtb(i1,i2+1,
     & i3+1,kd)+vtb(i1,i2-1,i3+1,kd)+vtb(i1,i2+1,i3-1,kd)+vtb(i1,i2-1,
     & i3-1,kd)) )/(dx(1)**2*dx(2)**2)
      vtbmr2(i1,i2,i3,kd)=(vtbm(i1+1,i2,i3,kd)-vtbm(i1-1,i2,i3,kd))*
     & d12(0)
      vtbms2(i1,i2,i3,kd)=(vtbm(i1,i2+1,i3,kd)-vtbm(i1,i2-1,i3,kd))*
     & d12(1)
      vtbmt2(i1,i2,i3,kd)=(vtbm(i1,i2,i3+1,kd)-vtbm(i1,i2,i3-1,kd))*
     & d12(2)
      vtbmrr2(i1,i2,i3,kd)=(-2.*vtbm(i1,i2,i3,kd)+(vtbm(i1+1,i2,i3,kd)+
     & vtbm(i1-1,i2,i3,kd)) )*d22(0)
      vtbmss2(i1,i2,i3,kd)=(-2.*vtbm(i1,i2,i3,kd)+(vtbm(i1,i2+1,i3,kd)+
     & vtbm(i1,i2-1,i3,kd)) )*d22(1)
      vtbmrs2(i1,i2,i3,kd)=(vtbmr2(i1,i2+1,i3,kd)-vtbmr2(i1,i2-1,i3,kd)
     & )*d12(1)
      vtbmtt2(i1,i2,i3,kd)=(-2.*vtbm(i1,i2,i3,kd)+(vtbm(i1,i2,i3+1,kd)+
     & vtbm(i1,i2,i3-1,kd)) )*d22(2)
      vtbmrt2(i1,i2,i3,kd)=(vtbmr2(i1,i2,i3+1,kd)-vtbmr2(i1,i2,i3-1,kd)
     & )*d12(2)
      vtbmst2(i1,i2,i3,kd)=(vtbms2(i1,i2,i3+1,kd)-vtbms2(i1,i2,i3-1,kd)
     & )*d12(2)
      vtbmrrr2(i1,i2,i3,kd)=(-2.*(vtbm(i1+1,i2,i3,kd)-vtbm(i1-1,i2,i3,
     & kd))+(vtbm(i1+2,i2,i3,kd)-vtbm(i1-2,i2,i3,kd)) )*d22(0)*d12(0)
      vtbmsss2(i1,i2,i3,kd)=(-2.*(vtbm(i1,i2+1,i3,kd)-vtbm(i1,i2-1,i3,
     & kd))+(vtbm(i1,i2+2,i3,kd)-vtbm(i1,i2-2,i3,kd)) )*d22(1)*d12(1)
      vtbmttt2(i1,i2,i3,kd)=(-2.*(vtbm(i1,i2,i3+1,kd)-vtbm(i1,i2,i3-1,
     & kd))+(vtbm(i1,i2,i3+2,kd)-vtbm(i1,i2,i3-2,kd)) )*d22(2)*d12(2)
      vtbmx21(i1,i2,i3,kd)= rx(i1,i2,i3)*vtbmr2(i1,i2,i3,kd)
      vtbmy21(i1,i2,i3,kd)=0
      vtbmz21(i1,i2,i3,kd)=0
      vtbmx22(i1,i2,i3,kd)= rx(i1,i2,i3)*vtbmr2(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*vtbms2(i1,i2,i3,kd)
      vtbmy22(i1,i2,i3,kd)= ry(i1,i2,i3)*vtbmr2(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*vtbms2(i1,i2,i3,kd)
      vtbmz22(i1,i2,i3,kd)=0
      vtbmx23(i1,i2,i3,kd)=rx(i1,i2,i3)*vtbmr2(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*vtbms2(i1,i2,i3,kd)+tx(i1,i2,i3)*vtbmt2(i1,i2,i3,kd)
      vtbmy23(i1,i2,i3,kd)=ry(i1,i2,i3)*vtbmr2(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*vtbms2(i1,i2,i3,kd)+ty(i1,i2,i3)*vtbmt2(i1,i2,i3,kd)
      vtbmz23(i1,i2,i3,kd)=rz(i1,i2,i3)*vtbmr2(i1,i2,i3,kd)+sz(i1,i2,
     & i3)*vtbms2(i1,i2,i3,kd)+tz(i1,i2,i3)*vtbmt2(i1,i2,i3,kd)
      vtbmxx21(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vtbmrr2(i1,i2,i3,kd)+(
     & rxx22(i1,i2,i3))*vtbmr2(i1,i2,i3,kd)
      vtbmyy21(i1,i2,i3,kd)=0
      vtbmxy21(i1,i2,i3,kd)=0
      vtbmxz21(i1,i2,i3,kd)=0
      vtbmyz21(i1,i2,i3,kd)=0
      vtbmzz21(i1,i2,i3,kd)=0
      vtbmlaplacian21(i1,i2,i3,kd)=vtbmxx21(i1,i2,i3,kd)
      vtbmxx22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vtbmrr2(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*vtbmrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*vtbmss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3))*vtbmr2(i1,i2,i3,kd)+(
     & sxx22(i1,i2,i3))*vtbms2(i1,i2,i3,kd)
      vtbmyy22(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*vtbmrr2(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*vtbmrs2(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*vtbmss2(i1,i2,i3,kd)+(ryy22(i1,i2,i3))*vtbmr2(i1,i2,i3,kd)+(
     & syy22(i1,i2,i3))*vtbms2(i1,i2,i3,kd)
      vtbmxy22(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vtbmrr2(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & vtbmrs2(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vtbmss2(i1,i2,
     & i3,kd)+rxy22(i1,i2,i3)*vtbmr2(i1,i2,i3,kd)+sxy22(i1,i2,i3)*
     & vtbms2(i1,i2,i3,kd)
      vtbmxz22(i1,i2,i3,kd)=0
      vtbmyz22(i1,i2,i3,kd)=0
      vtbmzz22(i1,i2,i3,kd)=0
      vtbmlaplacian22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & vtbmrr2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,
     & i3)*sy(i1,i2,i3))*vtbmrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,
     & i2,i3)**2)*vtbmss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3)+ryy22(i1,i2,
     & i3))*vtbmr2(i1,i2,i3,kd)+(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*
     & vtbms2(i1,i2,i3,kd)
      vtbmxx23(i1,i2,i3,kd)=rx(i1,i2,i3)**2*vtbmrr2(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*vtbmss2(i1,i2,i3,kd)+tx(i1,i2,i3)**2*vtbmtt2(i1,i2,
     & i3,kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*vtbmrs2(i1,i2,i3,kd)+2.*rx(
     & i1,i2,i3)*tx(i1,i2,i3)*vtbmrt2(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(
     & i1,i2,i3)*vtbmst2(i1,i2,i3,kd)+rxx23(i1,i2,i3)*vtbmr2(i1,i2,i3,
     & kd)+sxx23(i1,i2,i3)*vtbms2(i1,i2,i3,kd)+txx23(i1,i2,i3)*vtbmt2(
     & i1,i2,i3,kd)
      vtbmyy23(i1,i2,i3,kd)=ry(i1,i2,i3)**2*vtbmrr2(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*vtbmss2(i1,i2,i3,kd)+ty(i1,i2,i3)**2*vtbmtt2(i1,i2,
     & i3,kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*vtbmrs2(i1,i2,i3,kd)+2.*ry(
     & i1,i2,i3)*ty(i1,i2,i3)*vtbmrt2(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(
     & i1,i2,i3)*vtbmst2(i1,i2,i3,kd)+ryy23(i1,i2,i3)*vtbmr2(i1,i2,i3,
     & kd)+syy23(i1,i2,i3)*vtbms2(i1,i2,i3,kd)+tyy23(i1,i2,i3)*vtbmt2(
     & i1,i2,i3,kd)
      vtbmzz23(i1,i2,i3,kd)=rz(i1,i2,i3)**2*vtbmrr2(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*vtbmss2(i1,i2,i3,kd)+tz(i1,i2,i3)**2*vtbmtt2(i1,i2,
     & i3,kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*vtbmrs2(i1,i2,i3,kd)+2.*rz(
     & i1,i2,i3)*tz(i1,i2,i3)*vtbmrt2(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(
     & i1,i2,i3)*vtbmst2(i1,i2,i3,kd)+rzz23(i1,i2,i3)*vtbmr2(i1,i2,i3,
     & kd)+szz23(i1,i2,i3)*vtbms2(i1,i2,i3,kd)+tzz23(i1,i2,i3)*vtbmt2(
     & i1,i2,i3,kd)
      vtbmxy23(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vtbmrr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vtbmss2(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *ty(i1,i2,i3)*vtbmtt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+
     & ry(i1,i2,i3)*sx(i1,i2,i3))*vtbmrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & ty(i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*vtbmrt2(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*vtbmst2(
     & i1,i2,i3,kd)+rxy23(i1,i2,i3)*vtbmr2(i1,i2,i3,kd)+sxy23(i1,i2,
     & i3)*vtbms2(i1,i2,i3,kd)+txy23(i1,i2,i3)*vtbmt2(i1,i2,i3,kd)
      vtbmxz23(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*vtbmrr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*vtbmss2(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *tz(i1,i2,i3)*vtbmtt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sx(i1,i2,i3))*vtbmrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*vtbmrt2(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*vtbmst2(
     & i1,i2,i3,kd)+rxz23(i1,i2,i3)*vtbmr2(i1,i2,i3,kd)+sxz23(i1,i2,
     & i3)*vtbms2(i1,i2,i3,kd)+txz23(i1,i2,i3)*vtbmt2(i1,i2,i3,kd)
      vtbmyz23(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*vtbmrr2(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*vtbmss2(i1,i2,i3,kd)+ty(i1,i2,i3)
     & *tz(i1,i2,i3)*vtbmtt2(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sy(i1,i2,i3))*vtbmrs2(i1,i2,i3,kd)+(ry(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*vtbmrt2(i1,i2,i3,kd)+(
     & sy(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*vtbmst2(
     & i1,i2,i3,kd)+ryz23(i1,i2,i3)*vtbmr2(i1,i2,i3,kd)+syz23(i1,i2,
     & i3)*vtbms2(i1,i2,i3,kd)+tyz23(i1,i2,i3)*vtbmt2(i1,i2,i3,kd)
      vtbmlaplacian23(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*vtbmrr2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2+sz(i1,i2,i3)**2)*vtbmss2(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+
     & ty(i1,i2,i3)**2+tz(i1,i2,i3)**2)*vtbmtt2(i1,i2,i3,kd)+2.*(rx(
     & i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*
     & sz(i1,i2,i3))*vtbmrs2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,
     & i3)+ ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*
     & vtbmrt2(i1,i2,i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,
     & i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3))*vtbmst2(i1,i2,i3,
     & kd)+(rxx23(i1,i2,i3)+ryy23(i1,i2,i3)+rzz23(i1,i2,i3))*vtbmr2(
     & i1,i2,i3,kd)+(sxx23(i1,i2,i3)+syy23(i1,i2,i3)+szz23(i1,i2,i3))*
     & vtbms2(i1,i2,i3,kd)+(txx23(i1,i2,i3)+tyy23(i1,i2,i3)+tzz23(i1,
     & i2,i3))*vtbmt2(i1,i2,i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      vtbmx23r(i1,i2,i3,kd)=(vtbm(i1+1,i2,i3,kd)-vtbm(i1-1,i2,i3,kd))*
     & h12(0)
      vtbmy23r(i1,i2,i3,kd)=(vtbm(i1,i2+1,i3,kd)-vtbm(i1,i2-1,i3,kd))*
     & h12(1)
      vtbmz23r(i1,i2,i3,kd)=(vtbm(i1,i2,i3+1,kd)-vtbm(i1,i2,i3-1,kd))*
     & h12(2)
      vtbmxx23r(i1,i2,i3,kd)=(-2.*vtbm(i1,i2,i3,kd)+(vtbm(i1+1,i2,i3,
     & kd)+vtbm(i1-1,i2,i3,kd)) )*h22(0)
      vtbmyy23r(i1,i2,i3,kd)=(-2.*vtbm(i1,i2,i3,kd)+(vtbm(i1,i2+1,i3,
     & kd)+vtbm(i1,i2-1,i3,kd)) )*h22(1)
      vtbmxy23r(i1,i2,i3,kd)=(vtbmx23r(i1,i2+1,i3,kd)-vtbmx23r(i1,i2-1,
     & i3,kd))*h12(1)
      vtbmzz23r(i1,i2,i3,kd)=(-2.*vtbm(i1,i2,i3,kd)+(vtbm(i1,i2,i3+1,
     & kd)+vtbm(i1,i2,i3-1,kd)) )*h22(2)
      vtbmxz23r(i1,i2,i3,kd)=(vtbmx23r(i1,i2,i3+1,kd)-vtbmx23r(i1,i2,
     & i3-1,kd))*h12(2)
      vtbmyz23r(i1,i2,i3,kd)=(vtbmy23r(i1,i2,i3+1,kd)-vtbmy23r(i1,i2,
     & i3-1,kd))*h12(2)
      vtbmx21r(i1,i2,i3,kd)= vtbmx23r(i1,i2,i3,kd)
      vtbmy21r(i1,i2,i3,kd)= vtbmy23r(i1,i2,i3,kd)
      vtbmz21r(i1,i2,i3,kd)= vtbmz23r(i1,i2,i3,kd)
      vtbmxx21r(i1,i2,i3,kd)= vtbmxx23r(i1,i2,i3,kd)
      vtbmyy21r(i1,i2,i3,kd)= vtbmyy23r(i1,i2,i3,kd)
      vtbmzz21r(i1,i2,i3,kd)= vtbmzz23r(i1,i2,i3,kd)
      vtbmxy21r(i1,i2,i3,kd)= vtbmxy23r(i1,i2,i3,kd)
      vtbmxz21r(i1,i2,i3,kd)= vtbmxz23r(i1,i2,i3,kd)
      vtbmyz21r(i1,i2,i3,kd)= vtbmyz23r(i1,i2,i3,kd)
      vtbmlaplacian21r(i1,i2,i3,kd)=vtbmxx23r(i1,i2,i3,kd)
      vtbmx22r(i1,i2,i3,kd)= vtbmx23r(i1,i2,i3,kd)
      vtbmy22r(i1,i2,i3,kd)= vtbmy23r(i1,i2,i3,kd)
      vtbmz22r(i1,i2,i3,kd)= vtbmz23r(i1,i2,i3,kd)
      vtbmxx22r(i1,i2,i3,kd)= vtbmxx23r(i1,i2,i3,kd)
      vtbmyy22r(i1,i2,i3,kd)= vtbmyy23r(i1,i2,i3,kd)
      vtbmzz22r(i1,i2,i3,kd)= vtbmzz23r(i1,i2,i3,kd)
      vtbmxy22r(i1,i2,i3,kd)= vtbmxy23r(i1,i2,i3,kd)
      vtbmxz22r(i1,i2,i3,kd)= vtbmxz23r(i1,i2,i3,kd)
      vtbmyz22r(i1,i2,i3,kd)= vtbmyz23r(i1,i2,i3,kd)
      vtbmlaplacian22r(i1,i2,i3,kd)=vtbmxx23r(i1,i2,i3,kd)+vtbmyy23r(
     & i1,i2,i3,kd)
      vtbmlaplacian23r(i1,i2,i3,kd)=vtbmxx23r(i1,i2,i3,kd)+vtbmyy23r(
     & i1,i2,i3,kd)+vtbmzz23r(i1,i2,i3,kd)
      vtbmxxx22r(i1,i2,i3,kd)=(-2.*(vtbm(i1+1,i2,i3,kd)-vtbm(i1-1,i2,
     & i3,kd))+(vtbm(i1+2,i2,i3,kd)-vtbm(i1-2,i2,i3,kd)) )*h22(0)*h12(
     & 0)
      vtbmyyy22r(i1,i2,i3,kd)=(-2.*(vtbm(i1,i2+1,i3,kd)-vtbm(i1,i2-1,
     & i3,kd))+(vtbm(i1,i2+2,i3,kd)-vtbm(i1,i2-2,i3,kd)) )*h22(1)*h12(
     & 1)
      vtbmxxy22r(i1,i2,i3,kd)=( vtbmxx22r(i1,i2+1,i3,kd)-vtbmxx22r(i1,
     & i2-1,i3,kd))/(2.*dx(1))
      vtbmxyy22r(i1,i2,i3,kd)=( vtbmyy22r(i1+1,i2,i3,kd)-vtbmyy22r(i1-
     & 1,i2,i3,kd))/(2.*dx(0))
      vtbmxxxx22r(i1,i2,i3,kd)=(6.*vtbm(i1,i2,i3,kd)-4.*(vtbm(i1+1,i2,
     & i3,kd)+vtbm(i1-1,i2,i3,kd))+(vtbm(i1+2,i2,i3,kd)+vtbm(i1-2,i2,
     & i3,kd)) )/(dx(0)**4)
      vtbmyyyy22r(i1,i2,i3,kd)=(6.*vtbm(i1,i2,i3,kd)-4.*(vtbm(i1,i2+1,
     & i3,kd)+vtbm(i1,i2-1,i3,kd))+(vtbm(i1,i2+2,i3,kd)+vtbm(i1,i2-2,
     & i3,kd)) )/(dx(1)**4)
      vtbmxxyy22r(i1,i2,i3,kd)=( 4.*vtbm(i1,i2,i3,kd)     -2.*(vtbm(i1+
     & 1,i2,i3,kd)+vtbm(i1-1,i2,i3,kd)+vtbm(i1,i2+1,i3,kd)+vtbm(i1,i2-
     & 1,i3,kd))   +   (vtbm(i1+1,i2+1,i3,kd)+vtbm(i1-1,i2+1,i3,kd)+
     & vtbm(i1+1,i2-1,i3,kd)+vtbm(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)*
     & *2)
      ! 2D laplacian squared = vtbm.xxxx + 2 vtbm.xxyy + vtbm.yyyy
      vtbmLapSq22r(i1,i2,i3,kd)= ( 6.*vtbm(i1,i2,i3,kd)   - 4.*(vtbm(
     & i1+1,i2,i3,kd)+vtbm(i1-1,i2,i3,kd))    +(vtbm(i1+2,i2,i3,kd)+
     & vtbm(i1-2,i2,i3,kd)) )/(dx(0)**4) +( 6.*vtbm(i1,i2,i3,kd)    -
     & 4.*(vtbm(i1,i2+1,i3,kd)+vtbm(i1,i2-1,i3,kd))    +(vtbm(i1,i2+2,
     & i3,kd)+vtbm(i1,i2-2,i3,kd)) )/(dx(1)**4)  +( 8.*vtbm(i1,i2,i3,
     & kd)     -4.*(vtbm(i1+1,i2,i3,kd)+vtbm(i1-1,i2,i3,kd)+vtbm(i1,
     & i2+1,i3,kd)+vtbm(i1,i2-1,i3,kd))   +2.*(vtbm(i1+1,i2+1,i3,kd)+
     & vtbm(i1-1,i2+1,i3,kd)+vtbm(i1+1,i2-1,i3,kd)+vtbm(i1-1,i2-1,i3,
     & kd)) )/(dx(0)**2*dx(1)**2)
      vtbmxxx23r(i1,i2,i3,kd)=(-2.*(vtbm(i1+1,i2,i3,kd)-vtbm(i1-1,i2,
     & i3,kd))+(vtbm(i1+2,i2,i3,kd)-vtbm(i1-2,i2,i3,kd)) )*h22(0)*h12(
     & 0)
      vtbmyyy23r(i1,i2,i3,kd)=(-2.*(vtbm(i1,i2+1,i3,kd)-vtbm(i1,i2-1,
     & i3,kd))+(vtbm(i1,i2+2,i3,kd)-vtbm(i1,i2-2,i3,kd)) )*h22(1)*h12(
     & 1)
      vtbmzzz23r(i1,i2,i3,kd)=(-2.*(vtbm(i1,i2,i3+1,kd)-vtbm(i1,i2,i3-
     & 1,kd))+(vtbm(i1,i2,i3+2,kd)-vtbm(i1,i2,i3-2,kd)) )*h22(1)*h12(
     & 2)
      vtbmxxy23r(i1,i2,i3,kd)=( vtbmxx22r(i1,i2+1,i3,kd)-vtbmxx22r(i1,
     & i2-1,i3,kd))/(2.*dx(1))
      vtbmxyy23r(i1,i2,i3,kd)=( vtbmyy22r(i1+1,i2,i3,kd)-vtbmyy22r(i1-
     & 1,i2,i3,kd))/(2.*dx(0))
      vtbmxxz23r(i1,i2,i3,kd)=( vtbmxx22r(i1,i2,i3+1,kd)-vtbmxx22r(i1,
     & i2,i3-1,kd))/(2.*dx(2))
      vtbmyyz23r(i1,i2,i3,kd)=( vtbmyy22r(i1,i2,i3+1,kd)-vtbmyy22r(i1,
     & i2,i3-1,kd))/(2.*dx(2))
      vtbmxzz23r(i1,i2,i3,kd)=( vtbmzz22r(i1+1,i2,i3,kd)-vtbmzz22r(i1-
     & 1,i2,i3,kd))/(2.*dx(0))
      vtbmyzz23r(i1,i2,i3,kd)=( vtbmzz22r(i1,i2+1,i3,kd)-vtbmzz22r(i1,
     & i2-1,i3,kd))/(2.*dx(1))
      vtbmxxxx23r(i1,i2,i3,kd)=(6.*vtbm(i1,i2,i3,kd)-4.*(vtbm(i1+1,i2,
     & i3,kd)+vtbm(i1-1,i2,i3,kd))+(vtbm(i1+2,i2,i3,kd)+vtbm(i1-2,i2,
     & i3,kd)) )/(dx(0)**4)
      vtbmyyyy23r(i1,i2,i3,kd)=(6.*vtbm(i1,i2,i3,kd)-4.*(vtbm(i1,i2+1,
     & i3,kd)+vtbm(i1,i2-1,i3,kd))+(vtbm(i1,i2+2,i3,kd)+vtbm(i1,i2-2,
     & i3,kd)) )/(dx(1)**4)
      vtbmzzzz23r(i1,i2,i3,kd)=(6.*vtbm(i1,i2,i3,kd)-4.*(vtbm(i1,i2,i3+
     & 1,kd)+vtbm(i1,i2,i3-1,kd))+(vtbm(i1,i2,i3+2,kd)+vtbm(i1,i2,i3-
     & 2,kd)) )/(dx(2)**4)
      vtbmxxyy23r(i1,i2,i3,kd)=( 4.*vtbm(i1,i2,i3,kd)     -2.*(vtbm(i1+
     & 1,i2,i3,kd)+vtbm(i1-1,i2,i3,kd)+vtbm(i1,i2+1,i3,kd)+vtbm(i1,i2-
     & 1,i3,kd))   +   (vtbm(i1+1,i2+1,i3,kd)+vtbm(i1-1,i2+1,i3,kd)+
     & vtbm(i1+1,i2-1,i3,kd)+vtbm(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)*
     & *2)
      vtbmxxzz23r(i1,i2,i3,kd)=( 4.*vtbm(i1,i2,i3,kd)     -2.*(vtbm(i1+
     & 1,i2,i3,kd)+vtbm(i1-1,i2,i3,kd)+vtbm(i1,i2,i3+1,kd)+vtbm(i1,i2,
     & i3-1,kd))   +   (vtbm(i1+1,i2,i3+1,kd)+vtbm(i1-1,i2,i3+1,kd)+
     & vtbm(i1+1,i2,i3-1,kd)+vtbm(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)*
     & *2)
      vtbmyyzz23r(i1,i2,i3,kd)=( 4.*vtbm(i1,i2,i3,kd)     -2.*(vtbm(i1,
     & i2+1,i3,kd)  +vtbm(i1,i2-1,i3,kd)+  vtbm(i1,i2  ,i3+1,kd)+vtbm(
     & i1,i2  ,i3-1,kd))   +   (vtbm(i1,i2+1,i3+1,kd)+vtbm(i1,i2-1,i3+
     & 1,kd)+vtbm(i1,i2+1,i3-1,kd)+vtbm(i1,i2-1,i3-1,kd)) )/(dx(1)**2*
     & dx(2)**2)
      ! 3D laplacian squared = vtbm.xxxx + vtbm.yyyy + vtbm.zzzz + 2 (vtbm.xxyy + vtbm.xxzz + vtbm.yyzz )
      vtbmLapSq23r(i1,i2,i3,kd)= ( 6.*vtbm(i1,i2,i3,kd)   - 4.*(vtbm(
     & i1+1,i2,i3,kd)+vtbm(i1-1,i2,i3,kd))    +(vtbm(i1+2,i2,i3,kd)+
     & vtbm(i1-2,i2,i3,kd)) )/(dx(0)**4) +( 6.*vtbm(i1,i2,i3,kd)    -
     & 4.*(vtbm(i1,i2+1,i3,kd)+vtbm(i1,i2-1,i3,kd))    +(vtbm(i1,i2+2,
     & i3,kd)+vtbm(i1,i2-2,i3,kd)) )/(dx(1)**4)  +( 6.*vtbm(i1,i2,i3,
     & kd)    -4.*(vtbm(i1,i2,i3+1,kd)+vtbm(i1,i2,i3-1,kd))    +(vtbm(
     & i1,i2,i3+2,kd)+vtbm(i1,i2,i3-2,kd)) )/(dx(2)**4)  +( 8.*vtbm(
     & i1,i2,i3,kd)     -4.*(vtbm(i1+1,i2,i3,kd)  +vtbm(i1-1,i2,i3,kd)
     &   +vtbm(i1  ,i2+1,i3,kd)+vtbm(i1  ,i2-1,i3,kd))   +2.*(vtbm(i1+
     & 1,i2+1,i3,kd)+vtbm(i1-1,i2+1,i3,kd)+vtbm(i1+1,i2-1,i3,kd)+vtbm(
     & i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)+( 8.*vtbm(i1,i2,i3,kd) 
     &     -4.*(vtbm(i1+1,i2,i3,kd)  +vtbm(i1-1,i2,i3,kd)  +vtbm(i1  ,
     & i2,i3+1,kd)+vtbm(i1  ,i2,i3-1,kd))   +2.*(vtbm(i1+1,i2,i3+1,kd)
     & +vtbm(i1-1,i2,i3+1,kd)+vtbm(i1+1,i2,i3-1,kd)+vtbm(i1-1,i2,i3-1,
     & kd)) )/(dx(0)**2*dx(2)**2)+( 8.*vtbm(i1,i2,i3,kd)     -4.*(
     & vtbm(i1,i2+1,i3,kd)  +vtbm(i1,i2-1,i3,kd)  +vtbm(i1,i2  ,i3+1,
     & kd)+vtbm(i1,i2  ,i3-1,kd))   +2.*(vtbm(i1,i2+1,i3+1,kd)+vtbm(
     & i1,i2-1,i3+1,kd)+vtbm(i1,i2+1,i3-1,kd)+vtbm(i1,i2-1,i3-1,kd)) )
     & /(dx(1)**2*dx(2)**2)
      wtbr2(i1,i2,i3,kd)=(wtb(i1+1,i2,i3,kd)-wtb(i1-1,i2,i3,kd))*d12(0)
      wtbs2(i1,i2,i3,kd)=(wtb(i1,i2+1,i3,kd)-wtb(i1,i2-1,i3,kd))*d12(1)
      wtbt2(i1,i2,i3,kd)=(wtb(i1,i2,i3+1,kd)-wtb(i1,i2,i3-1,kd))*d12(2)
      wtbrr2(i1,i2,i3,kd)=(-2.*wtb(i1,i2,i3,kd)+(wtb(i1+1,i2,i3,kd)+
     & wtb(i1-1,i2,i3,kd)) )*d22(0)
      wtbss2(i1,i2,i3,kd)=(-2.*wtb(i1,i2,i3,kd)+(wtb(i1,i2+1,i3,kd)+
     & wtb(i1,i2-1,i3,kd)) )*d22(1)
      wtbrs2(i1,i2,i3,kd)=(wtbr2(i1,i2+1,i3,kd)-wtbr2(i1,i2-1,i3,kd))*
     & d12(1)
      wtbtt2(i1,i2,i3,kd)=(-2.*wtb(i1,i2,i3,kd)+(wtb(i1,i2,i3+1,kd)+
     & wtb(i1,i2,i3-1,kd)) )*d22(2)
      wtbrt2(i1,i2,i3,kd)=(wtbr2(i1,i2,i3+1,kd)-wtbr2(i1,i2,i3-1,kd))*
     & d12(2)
      wtbst2(i1,i2,i3,kd)=(wtbs2(i1,i2,i3+1,kd)-wtbs2(i1,i2,i3-1,kd))*
     & d12(2)
      wtbrrr2(i1,i2,i3,kd)=(-2.*(wtb(i1+1,i2,i3,kd)-wtb(i1-1,i2,i3,kd))
     & +(wtb(i1+2,i2,i3,kd)-wtb(i1-2,i2,i3,kd)) )*d22(0)*d12(0)
      wtbsss2(i1,i2,i3,kd)=(-2.*(wtb(i1,i2+1,i3,kd)-wtb(i1,i2-1,i3,kd))
     & +(wtb(i1,i2+2,i3,kd)-wtb(i1,i2-2,i3,kd)) )*d22(1)*d12(1)
      wtbttt2(i1,i2,i3,kd)=(-2.*(wtb(i1,i2,i3+1,kd)-wtb(i1,i2,i3-1,kd))
     & +(wtb(i1,i2,i3+2,kd)-wtb(i1,i2,i3-2,kd)) )*d22(2)*d12(2)
      wtbx21(i1,i2,i3,kd)= rx(i1,i2,i3)*wtbr2(i1,i2,i3,kd)
      wtby21(i1,i2,i3,kd)=0
      wtbz21(i1,i2,i3,kd)=0
      wtbx22(i1,i2,i3,kd)= rx(i1,i2,i3)*wtbr2(i1,i2,i3,kd)+sx(i1,i2,i3)
     & *wtbs2(i1,i2,i3,kd)
      wtby22(i1,i2,i3,kd)= ry(i1,i2,i3)*wtbr2(i1,i2,i3,kd)+sy(i1,i2,i3)
     & *wtbs2(i1,i2,i3,kd)
      wtbz22(i1,i2,i3,kd)=0
      wtbx23(i1,i2,i3,kd)=rx(i1,i2,i3)*wtbr2(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & wtbs2(i1,i2,i3,kd)+tx(i1,i2,i3)*wtbt2(i1,i2,i3,kd)
      wtby23(i1,i2,i3,kd)=ry(i1,i2,i3)*wtbr2(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & wtbs2(i1,i2,i3,kd)+ty(i1,i2,i3)*wtbt2(i1,i2,i3,kd)
      wtbz23(i1,i2,i3,kd)=rz(i1,i2,i3)*wtbr2(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & wtbs2(i1,i2,i3,kd)+tz(i1,i2,i3)*wtbt2(i1,i2,i3,kd)
      wtbxx21(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wtbrr2(i1,i2,i3,kd)+(
     & rxx22(i1,i2,i3))*wtbr2(i1,i2,i3,kd)
      wtbyy21(i1,i2,i3,kd)=0
      wtbxy21(i1,i2,i3,kd)=0
      wtbxz21(i1,i2,i3,kd)=0
      wtbyz21(i1,i2,i3,kd)=0
      wtbzz21(i1,i2,i3,kd)=0
      wtblaplacian21(i1,i2,i3,kd)=wtbxx21(i1,i2,i3,kd)
      wtbxx22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wtbrr2(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*wtbrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*wtbss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3))*wtbr2(i1,i2,i3,kd)+(
     & sxx22(i1,i2,i3))*wtbs2(i1,i2,i3,kd)
      wtbyy22(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*wtbrr2(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*wtbrs2(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*wtbss2(i1,i2,i3,kd)+(ryy22(i1,i2,i3))*wtbr2(i1,i2,i3,kd)+(
     & syy22(i1,i2,i3))*wtbs2(i1,i2,i3,kd)
      wtbxy22(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wtbrr2(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & wtbrs2(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wtbss2(i1,i2,i3,
     & kd)+rxy22(i1,i2,i3)*wtbr2(i1,i2,i3,kd)+sxy22(i1,i2,i3)*wtbs2(
     & i1,i2,i3,kd)
      wtbxz22(i1,i2,i3,kd)=0
      wtbyz22(i1,i2,i3,kd)=0
      wtbzz22(i1,i2,i3,kd)=0
      wtblaplacian22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & wtbrr2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)
     & *sy(i1,i2,i3))*wtbrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2)*wtbss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3)+ryy22(i1,i2,i3))*
     & wtbr2(i1,i2,i3,kd)+(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*wtbs2(i1,
     & i2,i3,kd)
      wtbxx23(i1,i2,i3,kd)=rx(i1,i2,i3)**2*wtbrr2(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*wtbss2(i1,i2,i3,kd)+tx(i1,i2,i3)**2*wtbtt2(i1,i2,i3,
     & kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*wtbrs2(i1,i2,i3,kd)+2.*rx(i1,
     & i2,i3)*tx(i1,i2,i3)*wtbrt2(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,
     & i2,i3)*wtbst2(i1,i2,i3,kd)+rxx23(i1,i2,i3)*wtbr2(i1,i2,i3,kd)+
     & sxx23(i1,i2,i3)*wtbs2(i1,i2,i3,kd)+txx23(i1,i2,i3)*wtbt2(i1,i2,
     & i3,kd)
      wtbyy23(i1,i2,i3,kd)=ry(i1,i2,i3)**2*wtbrr2(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*wtbss2(i1,i2,i3,kd)+ty(i1,i2,i3)**2*wtbtt2(i1,i2,i3,
     & kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*wtbrs2(i1,i2,i3,kd)+2.*ry(i1,
     & i2,i3)*ty(i1,i2,i3)*wtbrt2(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,
     & i2,i3)*wtbst2(i1,i2,i3,kd)+ryy23(i1,i2,i3)*wtbr2(i1,i2,i3,kd)+
     & syy23(i1,i2,i3)*wtbs2(i1,i2,i3,kd)+tyy23(i1,i2,i3)*wtbt2(i1,i2,
     & i3,kd)
      wtbzz23(i1,i2,i3,kd)=rz(i1,i2,i3)**2*wtbrr2(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*wtbss2(i1,i2,i3,kd)+tz(i1,i2,i3)**2*wtbtt2(i1,i2,i3,
     & kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*wtbrs2(i1,i2,i3,kd)+2.*rz(i1,
     & i2,i3)*tz(i1,i2,i3)*wtbrt2(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,
     & i2,i3)*wtbst2(i1,i2,i3,kd)+rzz23(i1,i2,i3)*wtbr2(i1,i2,i3,kd)+
     & szz23(i1,i2,i3)*wtbs2(i1,i2,i3,kd)+tzz23(i1,i2,i3)*wtbt2(i1,i2,
     & i3,kd)
      wtbxy23(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wtbrr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wtbss2(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & ty(i1,i2,i3)*wtbtt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(
     & i1,i2,i3)*sx(i1,i2,i3))*wtbrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(
     & i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*wtbrt2(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*wtbst2(i1,i2,
     & i3,kd)+rxy23(i1,i2,i3)*wtbr2(i1,i2,i3,kd)+sxy23(i1,i2,i3)*
     & wtbs2(i1,i2,i3,kd)+txy23(i1,i2,i3)*wtbt2(i1,i2,i3,kd)
      wtbxz23(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*wtbrr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*wtbss2(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & tz(i1,i2,i3)*wtbtt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sx(i1,i2,i3))*wtbrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*wtbrt2(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*wtbst2(i1,i2,
     & i3,kd)+rxz23(i1,i2,i3)*wtbr2(i1,i2,i3,kd)+sxz23(i1,i2,i3)*
     & wtbs2(i1,i2,i3,kd)+txz23(i1,i2,i3)*wtbt2(i1,i2,i3,kd)
      wtbyz23(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*wtbrr2(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*wtbss2(i1,i2,i3,kd)+ty(i1,i2,i3)*
     & tz(i1,i2,i3)*wtbtt2(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sy(i1,i2,i3))*wtbrs2(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*wtbrt2(i1,i2,i3,kd)+(sy(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*wtbst2(i1,i2,
     & i3,kd)+ryz23(i1,i2,i3)*wtbr2(i1,i2,i3,kd)+syz23(i1,i2,i3)*
     & wtbs2(i1,i2,i3,kd)+tyz23(i1,i2,i3)*wtbt2(i1,i2,i3,kd)
      wtblaplacian23(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*wtbrr2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)
     & **2+sz(i1,i2,i3)**2)*wtbss2(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(
     & i1,i2,i3)**2+tz(i1,i2,i3)**2)*wtbtt2(i1,i2,i3,kd)+2.*(rx(i1,i2,
     & i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,
     & i2,i3))*wtbrs2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(
     & i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*wtbrt2(i1,i2,
     & i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)
     & +sz(i1,i2,i3)*tz(i1,i2,i3))*wtbst2(i1,i2,i3,kd)+(rxx23(i1,i2,
     & i3)+ryy23(i1,i2,i3)+rzz23(i1,i2,i3))*wtbr2(i1,i2,i3,kd)+(sxx23(
     & i1,i2,i3)+syy23(i1,i2,i3)+szz23(i1,i2,i3))*wtbs2(i1,i2,i3,kd)+(
     & txx23(i1,i2,i3)+tyy23(i1,i2,i3)+tzz23(i1,i2,i3))*wtbt2(i1,i2,
     & i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      wtbx23r(i1,i2,i3,kd)=(wtb(i1+1,i2,i3,kd)-wtb(i1-1,i2,i3,kd))*h12(
     & 0)
      wtby23r(i1,i2,i3,kd)=(wtb(i1,i2+1,i3,kd)-wtb(i1,i2-1,i3,kd))*h12(
     & 1)
      wtbz23r(i1,i2,i3,kd)=(wtb(i1,i2,i3+1,kd)-wtb(i1,i2,i3-1,kd))*h12(
     & 2)
      wtbxx23r(i1,i2,i3,kd)=(-2.*wtb(i1,i2,i3,kd)+(wtb(i1+1,i2,i3,kd)+
     & wtb(i1-1,i2,i3,kd)) )*h22(0)
      wtbyy23r(i1,i2,i3,kd)=(-2.*wtb(i1,i2,i3,kd)+(wtb(i1,i2+1,i3,kd)+
     & wtb(i1,i2-1,i3,kd)) )*h22(1)
      wtbxy23r(i1,i2,i3,kd)=(wtbx23r(i1,i2+1,i3,kd)-wtbx23r(i1,i2-1,i3,
     & kd))*h12(1)
      wtbzz23r(i1,i2,i3,kd)=(-2.*wtb(i1,i2,i3,kd)+(wtb(i1,i2,i3+1,kd)+
     & wtb(i1,i2,i3-1,kd)) )*h22(2)
      wtbxz23r(i1,i2,i3,kd)=(wtbx23r(i1,i2,i3+1,kd)-wtbx23r(i1,i2,i3-1,
     & kd))*h12(2)
      wtbyz23r(i1,i2,i3,kd)=(wtby23r(i1,i2,i3+1,kd)-wtby23r(i1,i2,i3-1,
     & kd))*h12(2)
      wtbx21r(i1,i2,i3,kd)= wtbx23r(i1,i2,i3,kd)
      wtby21r(i1,i2,i3,kd)= wtby23r(i1,i2,i3,kd)
      wtbz21r(i1,i2,i3,kd)= wtbz23r(i1,i2,i3,kd)
      wtbxx21r(i1,i2,i3,kd)= wtbxx23r(i1,i2,i3,kd)
      wtbyy21r(i1,i2,i3,kd)= wtbyy23r(i1,i2,i3,kd)
      wtbzz21r(i1,i2,i3,kd)= wtbzz23r(i1,i2,i3,kd)
      wtbxy21r(i1,i2,i3,kd)= wtbxy23r(i1,i2,i3,kd)
      wtbxz21r(i1,i2,i3,kd)= wtbxz23r(i1,i2,i3,kd)
      wtbyz21r(i1,i2,i3,kd)= wtbyz23r(i1,i2,i3,kd)
      wtblaplacian21r(i1,i2,i3,kd)=wtbxx23r(i1,i2,i3,kd)
      wtbx22r(i1,i2,i3,kd)= wtbx23r(i1,i2,i3,kd)
      wtby22r(i1,i2,i3,kd)= wtby23r(i1,i2,i3,kd)
      wtbz22r(i1,i2,i3,kd)= wtbz23r(i1,i2,i3,kd)
      wtbxx22r(i1,i2,i3,kd)= wtbxx23r(i1,i2,i3,kd)
      wtbyy22r(i1,i2,i3,kd)= wtbyy23r(i1,i2,i3,kd)
      wtbzz22r(i1,i2,i3,kd)= wtbzz23r(i1,i2,i3,kd)
      wtbxy22r(i1,i2,i3,kd)= wtbxy23r(i1,i2,i3,kd)
      wtbxz22r(i1,i2,i3,kd)= wtbxz23r(i1,i2,i3,kd)
      wtbyz22r(i1,i2,i3,kd)= wtbyz23r(i1,i2,i3,kd)
      wtblaplacian22r(i1,i2,i3,kd)=wtbxx23r(i1,i2,i3,kd)+wtbyy23r(i1,
     & i2,i3,kd)
      wtblaplacian23r(i1,i2,i3,kd)=wtbxx23r(i1,i2,i3,kd)+wtbyy23r(i1,
     & i2,i3,kd)+wtbzz23r(i1,i2,i3,kd)
      wtbxxx22r(i1,i2,i3,kd)=(-2.*(wtb(i1+1,i2,i3,kd)-wtb(i1-1,i2,i3,
     & kd))+(wtb(i1+2,i2,i3,kd)-wtb(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
      wtbyyy22r(i1,i2,i3,kd)=(-2.*(wtb(i1,i2+1,i3,kd)-wtb(i1,i2-1,i3,
     & kd))+(wtb(i1,i2+2,i3,kd)-wtb(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
      wtbxxy22r(i1,i2,i3,kd)=( wtbxx22r(i1,i2+1,i3,kd)-wtbxx22r(i1,i2-
     & 1,i3,kd))/(2.*dx(1))
      wtbxyy22r(i1,i2,i3,kd)=( wtbyy22r(i1+1,i2,i3,kd)-wtbyy22r(i1-1,
     & i2,i3,kd))/(2.*dx(0))
      wtbxxxx22r(i1,i2,i3,kd)=(6.*wtb(i1,i2,i3,kd)-4.*(wtb(i1+1,i2,i3,
     & kd)+wtb(i1-1,i2,i3,kd))+(wtb(i1+2,i2,i3,kd)+wtb(i1-2,i2,i3,kd))
     &  )/(dx(0)**4)
      wtbyyyy22r(i1,i2,i3,kd)=(6.*wtb(i1,i2,i3,kd)-4.*(wtb(i1,i2+1,i3,
     & kd)+wtb(i1,i2-1,i3,kd))+(wtb(i1,i2+2,i3,kd)+wtb(i1,i2-2,i3,kd))
     &  )/(dx(1)**4)
      wtbxxyy22r(i1,i2,i3,kd)=( 4.*wtb(i1,i2,i3,kd)     -2.*(wtb(i1+1,
     & i2,i3,kd)+wtb(i1-1,i2,i3,kd)+wtb(i1,i2+1,i3,kd)+wtb(i1,i2-1,i3,
     & kd))   +   (wtb(i1+1,i2+1,i3,kd)+wtb(i1-1,i2+1,i3,kd)+wtb(i1+1,
     & i2-1,i3,kd)+wtb(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      ! 2D laplacian squared = wtb.xxxx + 2 wtb.xxyy + wtb.yyyy
      wtbLapSq22r(i1,i2,i3,kd)= ( 6.*wtb(i1,i2,i3,kd)   - 4.*(wtb(i1+1,
     & i2,i3,kd)+wtb(i1-1,i2,i3,kd))    +(wtb(i1+2,i2,i3,kd)+wtb(i1-2,
     & i2,i3,kd)) )/(dx(0)**4) +( 6.*wtb(i1,i2,i3,kd)    -4.*(wtb(i1,
     & i2+1,i3,kd)+wtb(i1,i2-1,i3,kd))    +(wtb(i1,i2+2,i3,kd)+wtb(i1,
     & i2-2,i3,kd)) )/(dx(1)**4)  +( 8.*wtb(i1,i2,i3,kd)     -4.*(wtb(
     & i1+1,i2,i3,kd)+wtb(i1-1,i2,i3,kd)+wtb(i1,i2+1,i3,kd)+wtb(i1,i2-
     & 1,i3,kd))   +2.*(wtb(i1+1,i2+1,i3,kd)+wtb(i1-1,i2+1,i3,kd)+wtb(
     & i1+1,i2-1,i3,kd)+wtb(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      wtbxxx23r(i1,i2,i3,kd)=(-2.*(wtb(i1+1,i2,i3,kd)-wtb(i1-1,i2,i3,
     & kd))+(wtb(i1+2,i2,i3,kd)-wtb(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
      wtbyyy23r(i1,i2,i3,kd)=(-2.*(wtb(i1,i2+1,i3,kd)-wtb(i1,i2-1,i3,
     & kd))+(wtb(i1,i2+2,i3,kd)-wtb(i1,i2-2,i3,kd)) )*h22(1)*h12(1)
      wtbzzz23r(i1,i2,i3,kd)=(-2.*(wtb(i1,i2,i3+1,kd)-wtb(i1,i2,i3-1,
     & kd))+(wtb(i1,i2,i3+2,kd)-wtb(i1,i2,i3-2,kd)) )*h22(1)*h12(2)
      wtbxxy23r(i1,i2,i3,kd)=( wtbxx22r(i1,i2+1,i3,kd)-wtbxx22r(i1,i2-
     & 1,i3,kd))/(2.*dx(1))
      wtbxyy23r(i1,i2,i3,kd)=( wtbyy22r(i1+1,i2,i3,kd)-wtbyy22r(i1-1,
     & i2,i3,kd))/(2.*dx(0))
      wtbxxz23r(i1,i2,i3,kd)=( wtbxx22r(i1,i2,i3+1,kd)-wtbxx22r(i1,i2,
     & i3-1,kd))/(2.*dx(2))
      wtbyyz23r(i1,i2,i3,kd)=( wtbyy22r(i1,i2,i3+1,kd)-wtbyy22r(i1,i2,
     & i3-1,kd))/(2.*dx(2))
      wtbxzz23r(i1,i2,i3,kd)=( wtbzz22r(i1+1,i2,i3,kd)-wtbzz22r(i1-1,
     & i2,i3,kd))/(2.*dx(0))
      wtbyzz23r(i1,i2,i3,kd)=( wtbzz22r(i1,i2+1,i3,kd)-wtbzz22r(i1,i2-
     & 1,i3,kd))/(2.*dx(1))
      wtbxxxx23r(i1,i2,i3,kd)=(6.*wtb(i1,i2,i3,kd)-4.*(wtb(i1+1,i2,i3,
     & kd)+wtb(i1-1,i2,i3,kd))+(wtb(i1+2,i2,i3,kd)+wtb(i1-2,i2,i3,kd))
     &  )/(dx(0)**4)
      wtbyyyy23r(i1,i2,i3,kd)=(6.*wtb(i1,i2,i3,kd)-4.*(wtb(i1,i2+1,i3,
     & kd)+wtb(i1,i2-1,i3,kd))+(wtb(i1,i2+2,i3,kd)+wtb(i1,i2-2,i3,kd))
     &  )/(dx(1)**4)
      wtbzzzz23r(i1,i2,i3,kd)=(6.*wtb(i1,i2,i3,kd)-4.*(wtb(i1,i2,i3+1,
     & kd)+wtb(i1,i2,i3-1,kd))+(wtb(i1,i2,i3+2,kd)+wtb(i1,i2,i3-2,kd))
     &  )/(dx(2)**4)
      wtbxxyy23r(i1,i2,i3,kd)=( 4.*wtb(i1,i2,i3,kd)     -2.*(wtb(i1+1,
     & i2,i3,kd)+wtb(i1-1,i2,i3,kd)+wtb(i1,i2+1,i3,kd)+wtb(i1,i2-1,i3,
     & kd))   +   (wtb(i1+1,i2+1,i3,kd)+wtb(i1-1,i2+1,i3,kd)+wtb(i1+1,
     & i2-1,i3,kd)+wtb(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)
      wtbxxzz23r(i1,i2,i3,kd)=( 4.*wtb(i1,i2,i3,kd)     -2.*(wtb(i1+1,
     & i2,i3,kd)+wtb(i1-1,i2,i3,kd)+wtb(i1,i2,i3+1,kd)+wtb(i1,i2,i3-1,
     & kd))   +   (wtb(i1+1,i2,i3+1,kd)+wtb(i1-1,i2,i3+1,kd)+wtb(i1+1,
     & i2,i3-1,kd)+wtb(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)
      wtbyyzz23r(i1,i2,i3,kd)=( 4.*wtb(i1,i2,i3,kd)     -2.*(wtb(i1,i2+
     & 1,i3,kd)  +wtb(i1,i2-1,i3,kd)+  wtb(i1,i2  ,i3+1,kd)+wtb(i1,i2 
     &  ,i3-1,kd))   +   (wtb(i1,i2+1,i3+1,kd)+wtb(i1,i2-1,i3+1,kd)+
     & wtb(i1,i2+1,i3-1,kd)+wtb(i1,i2-1,i3-1,kd)) )/(dx(1)**2*dx(2)**
     & 2)
      ! 3D laplacian squared = wtb.xxxx + wtb.yyyy + wtb.zzzz + 2 (wtb.xxyy + wtb.xxzz + wtb.yyzz )
      wtbLapSq23r(i1,i2,i3,kd)= ( 6.*wtb(i1,i2,i3,kd)   - 4.*(wtb(i1+1,
     & i2,i3,kd)+wtb(i1-1,i2,i3,kd))    +(wtb(i1+2,i2,i3,kd)+wtb(i1-2,
     & i2,i3,kd)) )/(dx(0)**4) +( 6.*wtb(i1,i2,i3,kd)    -4.*(wtb(i1,
     & i2+1,i3,kd)+wtb(i1,i2-1,i3,kd))    +(wtb(i1,i2+2,i3,kd)+wtb(i1,
     & i2-2,i3,kd)) )/(dx(1)**4)  +( 6.*wtb(i1,i2,i3,kd)    -4.*(wtb(
     & i1,i2,i3+1,kd)+wtb(i1,i2,i3-1,kd))    +(wtb(i1,i2,i3+2,kd)+wtb(
     & i1,i2,i3-2,kd)) )/(dx(2)**4)  +( 8.*wtb(i1,i2,i3,kd)     -4.*(
     & wtb(i1+1,i2,i3,kd)  +wtb(i1-1,i2,i3,kd)  +wtb(i1  ,i2+1,i3,kd)+
     & wtb(i1  ,i2-1,i3,kd))   +2.*(wtb(i1+1,i2+1,i3,kd)+wtb(i1-1,i2+
     & 1,i3,kd)+wtb(i1+1,i2-1,i3,kd)+wtb(i1-1,i2-1,i3,kd)) )/(dx(0)**
     & 2*dx(1)**2)+( 8.*wtb(i1,i2,i3,kd)     -4.*(wtb(i1+1,i2,i3,kd)  
     & +wtb(i1-1,i2,i3,kd)  +wtb(i1  ,i2,i3+1,kd)+wtb(i1  ,i2,i3-1,kd)
     & )   +2.*(wtb(i1+1,i2,i3+1,kd)+wtb(i1-1,i2,i3+1,kd)+wtb(i1+1,i2,
     & i3-1,kd)+wtb(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)**2)+( 8.*wtb(
     & i1,i2,i3,kd)     -4.*(wtb(i1,i2+1,i3,kd)  +wtb(i1,i2-1,i3,kd)  
     & +wtb(i1,i2  ,i3+1,kd)+wtb(i1,i2  ,i3-1,kd))   +2.*(wtb(i1,i2+1,
     & i3+1,kd)+wtb(i1,i2-1,i3+1,kd)+wtb(i1,i2+1,i3-1,kd)+wtb(i1,i2-1,
     & i3-1,kd)) )/(dx(1)**2*dx(2)**2)
      wtbmr2(i1,i2,i3,kd)=(wtbm(i1+1,i2,i3,kd)-wtbm(i1-1,i2,i3,kd))*
     & d12(0)
      wtbms2(i1,i2,i3,kd)=(wtbm(i1,i2+1,i3,kd)-wtbm(i1,i2-1,i3,kd))*
     & d12(1)
      wtbmt2(i1,i2,i3,kd)=(wtbm(i1,i2,i3+1,kd)-wtbm(i1,i2,i3-1,kd))*
     & d12(2)
      wtbmrr2(i1,i2,i3,kd)=(-2.*wtbm(i1,i2,i3,kd)+(wtbm(i1+1,i2,i3,kd)+
     & wtbm(i1-1,i2,i3,kd)) )*d22(0)
      wtbmss2(i1,i2,i3,kd)=(-2.*wtbm(i1,i2,i3,kd)+(wtbm(i1,i2+1,i3,kd)+
     & wtbm(i1,i2-1,i3,kd)) )*d22(1)
      wtbmrs2(i1,i2,i3,kd)=(wtbmr2(i1,i2+1,i3,kd)-wtbmr2(i1,i2-1,i3,kd)
     & )*d12(1)
      wtbmtt2(i1,i2,i3,kd)=(-2.*wtbm(i1,i2,i3,kd)+(wtbm(i1,i2,i3+1,kd)+
     & wtbm(i1,i2,i3-1,kd)) )*d22(2)
      wtbmrt2(i1,i2,i3,kd)=(wtbmr2(i1,i2,i3+1,kd)-wtbmr2(i1,i2,i3-1,kd)
     & )*d12(2)
      wtbmst2(i1,i2,i3,kd)=(wtbms2(i1,i2,i3+1,kd)-wtbms2(i1,i2,i3-1,kd)
     & )*d12(2)
      wtbmrrr2(i1,i2,i3,kd)=(-2.*(wtbm(i1+1,i2,i3,kd)-wtbm(i1-1,i2,i3,
     & kd))+(wtbm(i1+2,i2,i3,kd)-wtbm(i1-2,i2,i3,kd)) )*d22(0)*d12(0)
      wtbmsss2(i1,i2,i3,kd)=(-2.*(wtbm(i1,i2+1,i3,kd)-wtbm(i1,i2-1,i3,
     & kd))+(wtbm(i1,i2+2,i3,kd)-wtbm(i1,i2-2,i3,kd)) )*d22(1)*d12(1)
      wtbmttt2(i1,i2,i3,kd)=(-2.*(wtbm(i1,i2,i3+1,kd)-wtbm(i1,i2,i3-1,
     & kd))+(wtbm(i1,i2,i3+2,kd)-wtbm(i1,i2,i3-2,kd)) )*d22(2)*d12(2)
      wtbmx21(i1,i2,i3,kd)= rx(i1,i2,i3)*wtbmr2(i1,i2,i3,kd)
      wtbmy21(i1,i2,i3,kd)=0
      wtbmz21(i1,i2,i3,kd)=0
      wtbmx22(i1,i2,i3,kd)= rx(i1,i2,i3)*wtbmr2(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*wtbms2(i1,i2,i3,kd)
      wtbmy22(i1,i2,i3,kd)= ry(i1,i2,i3)*wtbmr2(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*wtbms2(i1,i2,i3,kd)
      wtbmz22(i1,i2,i3,kd)=0
      wtbmx23(i1,i2,i3,kd)=rx(i1,i2,i3)*wtbmr2(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*wtbms2(i1,i2,i3,kd)+tx(i1,i2,i3)*wtbmt2(i1,i2,i3,kd)
      wtbmy23(i1,i2,i3,kd)=ry(i1,i2,i3)*wtbmr2(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*wtbms2(i1,i2,i3,kd)+ty(i1,i2,i3)*wtbmt2(i1,i2,i3,kd)
      wtbmz23(i1,i2,i3,kd)=rz(i1,i2,i3)*wtbmr2(i1,i2,i3,kd)+sz(i1,i2,
     & i3)*wtbms2(i1,i2,i3,kd)+tz(i1,i2,i3)*wtbmt2(i1,i2,i3,kd)
      wtbmxx21(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wtbmrr2(i1,i2,i3,kd)+(
     & rxx22(i1,i2,i3))*wtbmr2(i1,i2,i3,kd)
      wtbmyy21(i1,i2,i3,kd)=0
      wtbmxy21(i1,i2,i3,kd)=0
      wtbmxz21(i1,i2,i3,kd)=0
      wtbmyz21(i1,i2,i3,kd)=0
      wtbmzz21(i1,i2,i3,kd)=0
      wtbmlaplacian21(i1,i2,i3,kd)=wtbmxx21(i1,i2,i3,kd)
      wtbmxx22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wtbmrr2(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*wtbmrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*wtbmss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3))*wtbmr2(i1,i2,i3,kd)+(
     & sxx22(i1,i2,i3))*wtbms2(i1,i2,i3,kd)
      wtbmyy22(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*wtbmrr2(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*wtbmrs2(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*wtbmss2(i1,i2,i3,kd)+(ryy22(i1,i2,i3))*wtbmr2(i1,i2,i3,kd)+(
     & syy22(i1,i2,i3))*wtbms2(i1,i2,i3,kd)
      wtbmxy22(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wtbmrr2(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & wtbmrs2(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wtbmss2(i1,i2,
     & i3,kd)+rxy22(i1,i2,i3)*wtbmr2(i1,i2,i3,kd)+sxy22(i1,i2,i3)*
     & wtbms2(i1,i2,i3,kd)
      wtbmxz22(i1,i2,i3,kd)=0
      wtbmyz22(i1,i2,i3,kd)=0
      wtbmzz22(i1,i2,i3,kd)=0
      wtbmlaplacian22(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & wtbmrr2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,
     & i3)*sy(i1,i2,i3))*wtbmrs2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,
     & i2,i3)**2)*wtbmss2(i1,i2,i3,kd)+(rxx22(i1,i2,i3)+ryy22(i1,i2,
     & i3))*wtbmr2(i1,i2,i3,kd)+(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*
     & wtbms2(i1,i2,i3,kd)
      wtbmxx23(i1,i2,i3,kd)=rx(i1,i2,i3)**2*wtbmrr2(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*wtbmss2(i1,i2,i3,kd)+tx(i1,i2,i3)**2*wtbmtt2(i1,i2,
     & i3,kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*wtbmrs2(i1,i2,i3,kd)+2.*rx(
     & i1,i2,i3)*tx(i1,i2,i3)*wtbmrt2(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(
     & i1,i2,i3)*wtbmst2(i1,i2,i3,kd)+rxx23(i1,i2,i3)*wtbmr2(i1,i2,i3,
     & kd)+sxx23(i1,i2,i3)*wtbms2(i1,i2,i3,kd)+txx23(i1,i2,i3)*wtbmt2(
     & i1,i2,i3,kd)
      wtbmyy23(i1,i2,i3,kd)=ry(i1,i2,i3)**2*wtbmrr2(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*wtbmss2(i1,i2,i3,kd)+ty(i1,i2,i3)**2*wtbmtt2(i1,i2,
     & i3,kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*wtbmrs2(i1,i2,i3,kd)+2.*ry(
     & i1,i2,i3)*ty(i1,i2,i3)*wtbmrt2(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(
     & i1,i2,i3)*wtbmst2(i1,i2,i3,kd)+ryy23(i1,i2,i3)*wtbmr2(i1,i2,i3,
     & kd)+syy23(i1,i2,i3)*wtbms2(i1,i2,i3,kd)+tyy23(i1,i2,i3)*wtbmt2(
     & i1,i2,i3,kd)
      wtbmzz23(i1,i2,i3,kd)=rz(i1,i2,i3)**2*wtbmrr2(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*wtbmss2(i1,i2,i3,kd)+tz(i1,i2,i3)**2*wtbmtt2(i1,i2,
     & i3,kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*wtbmrs2(i1,i2,i3,kd)+2.*rz(
     & i1,i2,i3)*tz(i1,i2,i3)*wtbmrt2(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(
     & i1,i2,i3)*wtbmst2(i1,i2,i3,kd)+rzz23(i1,i2,i3)*wtbmr2(i1,i2,i3,
     & kd)+szz23(i1,i2,i3)*wtbms2(i1,i2,i3,kd)+tzz23(i1,i2,i3)*wtbmt2(
     & i1,i2,i3,kd)
      wtbmxy23(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wtbmrr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wtbmss2(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *ty(i1,i2,i3)*wtbmtt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+
     & ry(i1,i2,i3)*sx(i1,i2,i3))*wtbmrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & ty(i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*wtbmrt2(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*wtbmst2(
     & i1,i2,i3,kd)+rxy23(i1,i2,i3)*wtbmr2(i1,i2,i3,kd)+sxy23(i1,i2,
     & i3)*wtbms2(i1,i2,i3,kd)+txy23(i1,i2,i3)*wtbmt2(i1,i2,i3,kd)
      wtbmxz23(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*wtbmrr2(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*wtbmss2(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *tz(i1,i2,i3)*wtbmtt2(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sx(i1,i2,i3))*wtbmrs2(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*wtbmrt2(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*wtbmst2(
     & i1,i2,i3,kd)+rxz23(i1,i2,i3)*wtbmr2(i1,i2,i3,kd)+sxz23(i1,i2,
     & i3)*wtbms2(i1,i2,i3,kd)+txz23(i1,i2,i3)*wtbmt2(i1,i2,i3,kd)
      wtbmyz23(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*wtbmrr2(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*wtbmss2(i1,i2,i3,kd)+ty(i1,i2,i3)
     & *tz(i1,i2,i3)*wtbmtt2(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sy(i1,i2,i3))*wtbmrs2(i1,i2,i3,kd)+(ry(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*wtbmrt2(i1,i2,i3,kd)+(
     & sy(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*wtbmst2(
     & i1,i2,i3,kd)+ryz23(i1,i2,i3)*wtbmr2(i1,i2,i3,kd)+syz23(i1,i2,
     & i3)*wtbms2(i1,i2,i3,kd)+tyz23(i1,i2,i3)*wtbmt2(i1,i2,i3,kd)
      wtbmlaplacian23(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*wtbmrr2(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2+sz(i1,i2,i3)**2)*wtbmss2(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+
     & ty(i1,i2,i3)**2+tz(i1,i2,i3)**2)*wtbmtt2(i1,i2,i3,kd)+2.*(rx(
     & i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*
     & sz(i1,i2,i3))*wtbmrs2(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,
     & i3)+ ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*
     & wtbmrt2(i1,i2,i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,
     & i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3))*wtbmst2(i1,i2,i3,
     & kd)+(rxx23(i1,i2,i3)+ryy23(i1,i2,i3)+rzz23(i1,i2,i3))*wtbmr2(
     & i1,i2,i3,kd)+(sxx23(i1,i2,i3)+syy23(i1,i2,i3)+szz23(i1,i2,i3))*
     & wtbms2(i1,i2,i3,kd)+(txx23(i1,i2,i3)+tyy23(i1,i2,i3)+tzz23(i1,
     & i2,i3))*wtbmt2(i1,i2,i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      wtbmx23r(i1,i2,i3,kd)=(wtbm(i1+1,i2,i3,kd)-wtbm(i1-1,i2,i3,kd))*
     & h12(0)
      wtbmy23r(i1,i2,i3,kd)=(wtbm(i1,i2+1,i3,kd)-wtbm(i1,i2-1,i3,kd))*
     & h12(1)
      wtbmz23r(i1,i2,i3,kd)=(wtbm(i1,i2,i3+1,kd)-wtbm(i1,i2,i3-1,kd))*
     & h12(2)
      wtbmxx23r(i1,i2,i3,kd)=(-2.*wtbm(i1,i2,i3,kd)+(wtbm(i1+1,i2,i3,
     & kd)+wtbm(i1-1,i2,i3,kd)) )*h22(0)
      wtbmyy23r(i1,i2,i3,kd)=(-2.*wtbm(i1,i2,i3,kd)+(wtbm(i1,i2+1,i3,
     & kd)+wtbm(i1,i2-1,i3,kd)) )*h22(1)
      wtbmxy23r(i1,i2,i3,kd)=(wtbmx23r(i1,i2+1,i3,kd)-wtbmx23r(i1,i2-1,
     & i3,kd))*h12(1)
      wtbmzz23r(i1,i2,i3,kd)=(-2.*wtbm(i1,i2,i3,kd)+(wtbm(i1,i2,i3+1,
     & kd)+wtbm(i1,i2,i3-1,kd)) )*h22(2)
      wtbmxz23r(i1,i2,i3,kd)=(wtbmx23r(i1,i2,i3+1,kd)-wtbmx23r(i1,i2,
     & i3-1,kd))*h12(2)
      wtbmyz23r(i1,i2,i3,kd)=(wtbmy23r(i1,i2,i3+1,kd)-wtbmy23r(i1,i2,
     & i3-1,kd))*h12(2)
      wtbmx21r(i1,i2,i3,kd)= wtbmx23r(i1,i2,i3,kd)
      wtbmy21r(i1,i2,i3,kd)= wtbmy23r(i1,i2,i3,kd)
      wtbmz21r(i1,i2,i3,kd)= wtbmz23r(i1,i2,i3,kd)
      wtbmxx21r(i1,i2,i3,kd)= wtbmxx23r(i1,i2,i3,kd)
      wtbmyy21r(i1,i2,i3,kd)= wtbmyy23r(i1,i2,i3,kd)
      wtbmzz21r(i1,i2,i3,kd)= wtbmzz23r(i1,i2,i3,kd)
      wtbmxy21r(i1,i2,i3,kd)= wtbmxy23r(i1,i2,i3,kd)
      wtbmxz21r(i1,i2,i3,kd)= wtbmxz23r(i1,i2,i3,kd)
      wtbmyz21r(i1,i2,i3,kd)= wtbmyz23r(i1,i2,i3,kd)
      wtbmlaplacian21r(i1,i2,i3,kd)=wtbmxx23r(i1,i2,i3,kd)
      wtbmx22r(i1,i2,i3,kd)= wtbmx23r(i1,i2,i3,kd)
      wtbmy22r(i1,i2,i3,kd)= wtbmy23r(i1,i2,i3,kd)
      wtbmz22r(i1,i2,i3,kd)= wtbmz23r(i1,i2,i3,kd)
      wtbmxx22r(i1,i2,i3,kd)= wtbmxx23r(i1,i2,i3,kd)
      wtbmyy22r(i1,i2,i3,kd)= wtbmyy23r(i1,i2,i3,kd)
      wtbmzz22r(i1,i2,i3,kd)= wtbmzz23r(i1,i2,i3,kd)
      wtbmxy22r(i1,i2,i3,kd)= wtbmxy23r(i1,i2,i3,kd)
      wtbmxz22r(i1,i2,i3,kd)= wtbmxz23r(i1,i2,i3,kd)
      wtbmyz22r(i1,i2,i3,kd)= wtbmyz23r(i1,i2,i3,kd)
      wtbmlaplacian22r(i1,i2,i3,kd)=wtbmxx23r(i1,i2,i3,kd)+wtbmyy23r(
     & i1,i2,i3,kd)
      wtbmlaplacian23r(i1,i2,i3,kd)=wtbmxx23r(i1,i2,i3,kd)+wtbmyy23r(
     & i1,i2,i3,kd)+wtbmzz23r(i1,i2,i3,kd)
      wtbmxxx22r(i1,i2,i3,kd)=(-2.*(wtbm(i1+1,i2,i3,kd)-wtbm(i1-1,i2,
     & i3,kd))+(wtbm(i1+2,i2,i3,kd)-wtbm(i1-2,i2,i3,kd)) )*h22(0)*h12(
     & 0)
      wtbmyyy22r(i1,i2,i3,kd)=(-2.*(wtbm(i1,i2+1,i3,kd)-wtbm(i1,i2-1,
     & i3,kd))+(wtbm(i1,i2+2,i3,kd)-wtbm(i1,i2-2,i3,kd)) )*h22(1)*h12(
     & 1)
      wtbmxxy22r(i1,i2,i3,kd)=( wtbmxx22r(i1,i2+1,i3,kd)-wtbmxx22r(i1,
     & i2-1,i3,kd))/(2.*dx(1))
      wtbmxyy22r(i1,i2,i3,kd)=( wtbmyy22r(i1+1,i2,i3,kd)-wtbmyy22r(i1-
     & 1,i2,i3,kd))/(2.*dx(0))
      wtbmxxxx22r(i1,i2,i3,kd)=(6.*wtbm(i1,i2,i3,kd)-4.*(wtbm(i1+1,i2,
     & i3,kd)+wtbm(i1-1,i2,i3,kd))+(wtbm(i1+2,i2,i3,kd)+wtbm(i1-2,i2,
     & i3,kd)) )/(dx(0)**4)
      wtbmyyyy22r(i1,i2,i3,kd)=(6.*wtbm(i1,i2,i3,kd)-4.*(wtbm(i1,i2+1,
     & i3,kd)+wtbm(i1,i2-1,i3,kd))+(wtbm(i1,i2+2,i3,kd)+wtbm(i1,i2-2,
     & i3,kd)) )/(dx(1)**4)
      wtbmxxyy22r(i1,i2,i3,kd)=( 4.*wtbm(i1,i2,i3,kd)     -2.*(wtbm(i1+
     & 1,i2,i3,kd)+wtbm(i1-1,i2,i3,kd)+wtbm(i1,i2+1,i3,kd)+wtbm(i1,i2-
     & 1,i3,kd))   +   (wtbm(i1+1,i2+1,i3,kd)+wtbm(i1-1,i2+1,i3,kd)+
     & wtbm(i1+1,i2-1,i3,kd)+wtbm(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)*
     & *2)
      ! 2D laplacian squared = wtbm.xxxx + 2 wtbm.xxyy + wtbm.yyyy
      wtbmLapSq22r(i1,i2,i3,kd)= ( 6.*wtbm(i1,i2,i3,kd)   - 4.*(wtbm(
     & i1+1,i2,i3,kd)+wtbm(i1-1,i2,i3,kd))    +(wtbm(i1+2,i2,i3,kd)+
     & wtbm(i1-2,i2,i3,kd)) )/(dx(0)**4) +( 6.*wtbm(i1,i2,i3,kd)    -
     & 4.*(wtbm(i1,i2+1,i3,kd)+wtbm(i1,i2-1,i3,kd))    +(wtbm(i1,i2+2,
     & i3,kd)+wtbm(i1,i2-2,i3,kd)) )/(dx(1)**4)  +( 8.*wtbm(i1,i2,i3,
     & kd)     -4.*(wtbm(i1+1,i2,i3,kd)+wtbm(i1-1,i2,i3,kd)+wtbm(i1,
     & i2+1,i3,kd)+wtbm(i1,i2-1,i3,kd))   +2.*(wtbm(i1+1,i2+1,i3,kd)+
     & wtbm(i1-1,i2+1,i3,kd)+wtbm(i1+1,i2-1,i3,kd)+wtbm(i1-1,i2-1,i3,
     & kd)) )/(dx(0)**2*dx(1)**2)
      wtbmxxx23r(i1,i2,i3,kd)=(-2.*(wtbm(i1+1,i2,i3,kd)-wtbm(i1-1,i2,
     & i3,kd))+(wtbm(i1+2,i2,i3,kd)-wtbm(i1-2,i2,i3,kd)) )*h22(0)*h12(
     & 0)
      wtbmyyy23r(i1,i2,i3,kd)=(-2.*(wtbm(i1,i2+1,i3,kd)-wtbm(i1,i2-1,
     & i3,kd))+(wtbm(i1,i2+2,i3,kd)-wtbm(i1,i2-2,i3,kd)) )*h22(1)*h12(
     & 1)
      wtbmzzz23r(i1,i2,i3,kd)=(-2.*(wtbm(i1,i2,i3+1,kd)-wtbm(i1,i2,i3-
     & 1,kd))+(wtbm(i1,i2,i3+2,kd)-wtbm(i1,i2,i3-2,kd)) )*h22(1)*h12(
     & 2)
      wtbmxxy23r(i1,i2,i3,kd)=( wtbmxx22r(i1,i2+1,i3,kd)-wtbmxx22r(i1,
     & i2-1,i3,kd))/(2.*dx(1))
      wtbmxyy23r(i1,i2,i3,kd)=( wtbmyy22r(i1+1,i2,i3,kd)-wtbmyy22r(i1-
     & 1,i2,i3,kd))/(2.*dx(0))
      wtbmxxz23r(i1,i2,i3,kd)=( wtbmxx22r(i1,i2,i3+1,kd)-wtbmxx22r(i1,
     & i2,i3-1,kd))/(2.*dx(2))
      wtbmyyz23r(i1,i2,i3,kd)=( wtbmyy22r(i1,i2,i3+1,kd)-wtbmyy22r(i1,
     & i2,i3-1,kd))/(2.*dx(2))
      wtbmxzz23r(i1,i2,i3,kd)=( wtbmzz22r(i1+1,i2,i3,kd)-wtbmzz22r(i1-
     & 1,i2,i3,kd))/(2.*dx(0))
      wtbmyzz23r(i1,i2,i3,kd)=( wtbmzz22r(i1,i2+1,i3,kd)-wtbmzz22r(i1,
     & i2-1,i3,kd))/(2.*dx(1))
      wtbmxxxx23r(i1,i2,i3,kd)=(6.*wtbm(i1,i2,i3,kd)-4.*(wtbm(i1+1,i2,
     & i3,kd)+wtbm(i1-1,i2,i3,kd))+(wtbm(i1+2,i2,i3,kd)+wtbm(i1-2,i2,
     & i3,kd)) )/(dx(0)**4)
      wtbmyyyy23r(i1,i2,i3,kd)=(6.*wtbm(i1,i2,i3,kd)-4.*(wtbm(i1,i2+1,
     & i3,kd)+wtbm(i1,i2-1,i3,kd))+(wtbm(i1,i2+2,i3,kd)+wtbm(i1,i2-2,
     & i3,kd)) )/(dx(1)**4)
      wtbmzzzz23r(i1,i2,i3,kd)=(6.*wtbm(i1,i2,i3,kd)-4.*(wtbm(i1,i2,i3+
     & 1,kd)+wtbm(i1,i2,i3-1,kd))+(wtbm(i1,i2,i3+2,kd)+wtbm(i1,i2,i3-
     & 2,kd)) )/(dx(2)**4)
      wtbmxxyy23r(i1,i2,i3,kd)=( 4.*wtbm(i1,i2,i3,kd)     -2.*(wtbm(i1+
     & 1,i2,i3,kd)+wtbm(i1-1,i2,i3,kd)+wtbm(i1,i2+1,i3,kd)+wtbm(i1,i2-
     & 1,i3,kd))   +   (wtbm(i1+1,i2+1,i3,kd)+wtbm(i1-1,i2+1,i3,kd)+
     & wtbm(i1+1,i2-1,i3,kd)+wtbm(i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)*
     & *2)
      wtbmxxzz23r(i1,i2,i3,kd)=( 4.*wtbm(i1,i2,i3,kd)     -2.*(wtbm(i1+
     & 1,i2,i3,kd)+wtbm(i1-1,i2,i3,kd)+wtbm(i1,i2,i3+1,kd)+wtbm(i1,i2,
     & i3-1,kd))   +   (wtbm(i1+1,i2,i3+1,kd)+wtbm(i1-1,i2,i3+1,kd)+
     & wtbm(i1+1,i2,i3-1,kd)+wtbm(i1-1,i2,i3-1,kd)) )/(dx(0)**2*dx(2)*
     & *2)
      wtbmyyzz23r(i1,i2,i3,kd)=( 4.*wtbm(i1,i2,i3,kd)     -2.*(wtbm(i1,
     & i2+1,i3,kd)  +wtbm(i1,i2-1,i3,kd)+  wtbm(i1,i2  ,i3+1,kd)+wtbm(
     & i1,i2  ,i3-1,kd))   +   (wtbm(i1,i2+1,i3+1,kd)+wtbm(i1,i2-1,i3+
     & 1,kd)+wtbm(i1,i2+1,i3-1,kd)+wtbm(i1,i2-1,i3-1,kd)) )/(dx(1)**2*
     & dx(2)**2)
      ! 3D laplacian squared = wtbm.xxxx + wtbm.yyyy + wtbm.zzzz + 2 (wtbm.xxyy + wtbm.xxzz + wtbm.yyzz )
      wtbmLapSq23r(i1,i2,i3,kd)= ( 6.*wtbm(i1,i2,i3,kd)   - 4.*(wtbm(
     & i1+1,i2,i3,kd)+wtbm(i1-1,i2,i3,kd))    +(wtbm(i1+2,i2,i3,kd)+
     & wtbm(i1-2,i2,i3,kd)) )/(dx(0)**4) +( 6.*wtbm(i1,i2,i3,kd)    -
     & 4.*(wtbm(i1,i2+1,i3,kd)+wtbm(i1,i2-1,i3,kd))    +(wtbm(i1,i2+2,
     & i3,kd)+wtbm(i1,i2-2,i3,kd)) )/(dx(1)**4)  +( 6.*wtbm(i1,i2,i3,
     & kd)    -4.*(wtbm(i1,i2,i3+1,kd)+wtbm(i1,i2,i3-1,kd))    +(wtbm(
     & i1,i2,i3+2,kd)+wtbm(i1,i2,i3-2,kd)) )/(dx(2)**4)  +( 8.*wtbm(
     & i1,i2,i3,kd)     -4.*(wtbm(i1+1,i2,i3,kd)  +wtbm(i1-1,i2,i3,kd)
     &   +wtbm(i1  ,i2+1,i3,kd)+wtbm(i1  ,i2-1,i3,kd))   +2.*(wtbm(i1+
     & 1,i2+1,i3,kd)+wtbm(i1-1,i2+1,i3,kd)+wtbm(i1+1,i2-1,i3,kd)+wtbm(
     & i1-1,i2-1,i3,kd)) )/(dx(0)**2*dx(1)**2)+( 8.*wtbm(i1,i2,i3,kd) 
     &     -4.*(wtbm(i1+1,i2,i3,kd)  +wtbm(i1-1,i2,i3,kd)  +wtbm(i1  ,
     & i2,i3+1,kd)+wtbm(i1  ,i2,i3-1,kd))   +2.*(wtbm(i1+1,i2,i3+1,kd)
     & +wtbm(i1-1,i2,i3+1,kd)+wtbm(i1+1,i2,i3-1,kd)+wtbm(i1-1,i2,i3-1,
     & kd)) )/(dx(0)**2*dx(2)**2)+( 8.*wtbm(i1,i2,i3,kd)     -4.*(
     & wtbm(i1,i2+1,i3,kd)  +wtbm(i1,i2-1,i3,kd)  +wtbm(i1,i2  ,i3+1,
     & kd)+wtbm(i1,i2  ,i3-1,kd))   +2.*(wtbm(i1,i2+1,i3+1,kd)+wtbm(
     & i1,i2-1,i3+1,kd)+wtbm(i1,i2+1,i3-1,kd)+wtbm(i1,i2-1,i3-1,kd)) )
     & /(dx(1)**2*dx(2)**2)


      d14(kd) = 1./(12.*dr(kd))
      d24(kd) = 1./(12.*dr(kd)**2)
      ur4(i1,i2,i3,kd)=(8.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))-(u(i1+2,
     & i2,i3,kd)-u(i1-2,i2,i3,kd)))*d14(0)
      us4(i1,i2,i3,kd)=(8.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))-(u(i1,
     & i2+2,i3,kd)-u(i1,i2-2,i3,kd)))*d14(1)
      ut4(i1,i2,i3,kd)=(8.*(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))-(u(i1,
     & i2,i3+2,kd)-u(i1,i2,i3-2,kd)))*d14(2)
      urr4(i1,i2,i3,kd)=(-30.*u(i1,i2,i3,kd)+16.*(u(i1+1,i2,i3,kd)+u(
     & i1-1,i2,i3,kd))-(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)) )*d24(0)
      uss4(i1,i2,i3,kd)=(-30.*u(i1,i2,i3,kd)+16.*(u(i1,i2+1,i3,kd)+u(
     & i1,i2-1,i3,kd))-(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )*d24(1)
      utt4(i1,i2,i3,kd)=(-30.*u(i1,i2,i3,kd)+16.*(u(i1,i2,i3+1,kd)+u(
     & i1,i2,i3-1,kd))-(u(i1,i2,i3+2,kd)+u(i1,i2,i3-2,kd)) )*d24(2)
      urs4(i1,i2,i3,kd)=(8.*(ur4(i1,i2+1,i3,kd)-ur4(i1,i2-1,i3,kd))-(
     & ur4(i1,i2+2,i3,kd)-ur4(i1,i2-2,i3,kd)))*d14(1)
      urt4(i1,i2,i3,kd)=(8.*(ur4(i1,i2,i3+1,kd)-ur4(i1,i2,i3-1,kd))-(
     & ur4(i1,i2,i3+2,kd)-ur4(i1,i2,i3-2,kd)))*d14(2)
      ust4(i1,i2,i3,kd)=(8.*(us4(i1,i2,i3+1,kd)-us4(i1,i2,i3-1,kd))-(
     & us4(i1,i2,i3+2,kd)-us4(i1,i2,i3-2,kd)))*d14(2)
      rxr4(i1,i2,i3)=(8.*(rx(i1+1,i2,i3)-rx(i1-1,i2,i3))-(rx(i1+2,i2,
     & i3)-rx(i1-2,i2,i3)))*d14(0)
      rxs4(i1,i2,i3)=(8.*(rx(i1,i2+1,i3)-rx(i1,i2-1,i3))-(rx(i1,i2+2,
     & i3)-rx(i1,i2-2,i3)))*d14(1)
      rxt4(i1,i2,i3)=(8.*(rx(i1,i2,i3+1)-rx(i1,i2,i3-1))-(rx(i1,i2,i3+
     & 2)-rx(i1,i2,i3-2)))*d14(2)
      ryr4(i1,i2,i3)=(8.*(ry(i1+1,i2,i3)-ry(i1-1,i2,i3))-(ry(i1+2,i2,
     & i3)-ry(i1-2,i2,i3)))*d14(0)
      rys4(i1,i2,i3)=(8.*(ry(i1,i2+1,i3)-ry(i1,i2-1,i3))-(ry(i1,i2+2,
     & i3)-ry(i1,i2-2,i3)))*d14(1)
      ryt4(i1,i2,i3)=(8.*(ry(i1,i2,i3+1)-ry(i1,i2,i3-1))-(ry(i1,i2,i3+
     & 2)-ry(i1,i2,i3-2)))*d14(2)
      rzr4(i1,i2,i3)=(8.*(rz(i1+1,i2,i3)-rz(i1-1,i2,i3))-(rz(i1+2,i2,
     & i3)-rz(i1-2,i2,i3)))*d14(0)
      rzs4(i1,i2,i3)=(8.*(rz(i1,i2+1,i3)-rz(i1,i2-1,i3))-(rz(i1,i2+2,
     & i3)-rz(i1,i2-2,i3)))*d14(1)
      rzt4(i1,i2,i3)=(8.*(rz(i1,i2,i3+1)-rz(i1,i2,i3-1))-(rz(i1,i2,i3+
     & 2)-rz(i1,i2,i3-2)))*d14(2)
      sxr4(i1,i2,i3)=(8.*(sx(i1+1,i2,i3)-sx(i1-1,i2,i3))-(sx(i1+2,i2,
     & i3)-sx(i1-2,i2,i3)))*d14(0)
      sxs4(i1,i2,i3)=(8.*(sx(i1,i2+1,i3)-sx(i1,i2-1,i3))-(sx(i1,i2+2,
     & i3)-sx(i1,i2-2,i3)))*d14(1)
      sxt4(i1,i2,i3)=(8.*(sx(i1,i2,i3+1)-sx(i1,i2,i3-1))-(sx(i1,i2,i3+
     & 2)-sx(i1,i2,i3-2)))*d14(2)
      syr4(i1,i2,i3)=(8.*(sy(i1+1,i2,i3)-sy(i1-1,i2,i3))-(sy(i1+2,i2,
     & i3)-sy(i1-2,i2,i3)))*d14(0)
      sys4(i1,i2,i3)=(8.*(sy(i1,i2+1,i3)-sy(i1,i2-1,i3))-(sy(i1,i2+2,
     & i3)-sy(i1,i2-2,i3)))*d14(1)
      syt4(i1,i2,i3)=(8.*(sy(i1,i2,i3+1)-sy(i1,i2,i3-1))-(sy(i1,i2,i3+
     & 2)-sy(i1,i2,i3-2)))*d14(2)
      szr4(i1,i2,i3)=(8.*(sz(i1+1,i2,i3)-sz(i1-1,i2,i3))-(sz(i1+2,i2,
     & i3)-sz(i1-2,i2,i3)))*d14(0)
      szs4(i1,i2,i3)=(8.*(sz(i1,i2+1,i3)-sz(i1,i2-1,i3))-(sz(i1,i2+2,
     & i3)-sz(i1,i2-2,i3)))*d14(1)
      szt4(i1,i2,i3)=(8.*(sz(i1,i2,i3+1)-sz(i1,i2,i3-1))-(sz(i1,i2,i3+
     & 2)-sz(i1,i2,i3-2)))*d14(2)
      txr4(i1,i2,i3)=(8.*(tx(i1+1,i2,i3)-tx(i1-1,i2,i3))-(tx(i1+2,i2,
     & i3)-tx(i1-2,i2,i3)))*d14(0)
      txs4(i1,i2,i3)=(8.*(tx(i1,i2+1,i3)-tx(i1,i2-1,i3))-(tx(i1,i2+2,
     & i3)-tx(i1,i2-2,i3)))*d14(1)
      txt4(i1,i2,i3)=(8.*(tx(i1,i2,i3+1)-tx(i1,i2,i3-1))-(tx(i1,i2,i3+
     & 2)-tx(i1,i2,i3-2)))*d14(2)
      tyr4(i1,i2,i3)=(8.*(ty(i1+1,i2,i3)-ty(i1-1,i2,i3))-(ty(i1+2,i2,
     & i3)-ty(i1-2,i2,i3)))*d14(0)
      tys4(i1,i2,i3)=(8.*(ty(i1,i2+1,i3)-ty(i1,i2-1,i3))-(ty(i1,i2+2,
     & i3)-ty(i1,i2-2,i3)))*d14(1)
      tyt4(i1,i2,i3)=(8.*(ty(i1,i2,i3+1)-ty(i1,i2,i3-1))-(ty(i1,i2,i3+
     & 2)-ty(i1,i2,i3-2)))*d14(2)
      tzr4(i1,i2,i3)=(8.*(tz(i1+1,i2,i3)-tz(i1-1,i2,i3))-(tz(i1+2,i2,
     & i3)-tz(i1-2,i2,i3)))*d14(0)
      tzs4(i1,i2,i3)=(8.*(tz(i1,i2+1,i3)-tz(i1,i2-1,i3))-(tz(i1,i2+2,
     & i3)-tz(i1,i2-2,i3)))*d14(1)
      tzt4(i1,i2,i3)=(8.*(tz(i1,i2,i3+1)-tz(i1,i2,i3-1))-(tz(i1,i2,i3+
     & 2)-tz(i1,i2,i3-2)))*d14(2)
      ux41(i1,i2,i3,kd)= rx(i1,i2,i3)*ur4(i1,i2,i3,kd)
      uy41(i1,i2,i3,kd)=0
      uz41(i1,i2,i3,kd)=0
      ux42(i1,i2,i3,kd)= rx(i1,i2,i3)*ur4(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & us4(i1,i2,i3,kd)
      uy42(i1,i2,i3,kd)= ry(i1,i2,i3)*ur4(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & us4(i1,i2,i3,kd)
      uz42(i1,i2,i3,kd)=0
      ux43(i1,i2,i3,kd)=rx(i1,i2,i3)*ur4(i1,i2,i3,kd)+sx(i1,i2,i3)*us4(
     & i1,i2,i3,kd)+tx(i1,i2,i3)*ut4(i1,i2,i3,kd)
      uy43(i1,i2,i3,kd)=ry(i1,i2,i3)*ur4(i1,i2,i3,kd)+sy(i1,i2,i3)*us4(
     & i1,i2,i3,kd)+ty(i1,i2,i3)*ut4(i1,i2,i3,kd)
      uz43(i1,i2,i3,kd)=rz(i1,i2,i3)*ur4(i1,i2,i3,kd)+sz(i1,i2,i3)*us4(
     & i1,i2,i3,kd)+tz(i1,i2,i3)*ut4(i1,i2,i3,kd)
      rxx41(i1,i2,i3)= rx(i1,i2,i3)*rxr4(i1,i2,i3)
      rxx42(i1,i2,i3)= rx(i1,i2,i3)*rxr4(i1,i2,i3)+sx(i1,i2,i3)*rxs4(
     & i1,i2,i3)
      rxy42(i1,i2,i3)= ry(i1,i2,i3)*rxr4(i1,i2,i3)+sy(i1,i2,i3)*rxs4(
     & i1,i2,i3)
      rxx43(i1,i2,i3)=rx(i1,i2,i3)*rxr4(i1,i2,i3)+sx(i1,i2,i3)*rxs4(i1,
     & i2,i3)+tx(i1,i2,i3)*rxt4(i1,i2,i3)
      rxy43(i1,i2,i3)=ry(i1,i2,i3)*rxr4(i1,i2,i3)+sy(i1,i2,i3)*rxs4(i1,
     & i2,i3)+ty(i1,i2,i3)*rxt4(i1,i2,i3)
      rxz43(i1,i2,i3)=rz(i1,i2,i3)*rxr4(i1,i2,i3)+sz(i1,i2,i3)*rxs4(i1,
     & i2,i3)+tz(i1,i2,i3)*rxt4(i1,i2,i3)
      ryx42(i1,i2,i3)= rx(i1,i2,i3)*ryr4(i1,i2,i3)+sx(i1,i2,i3)*rys4(
     & i1,i2,i3)
      ryy42(i1,i2,i3)= ry(i1,i2,i3)*ryr4(i1,i2,i3)+sy(i1,i2,i3)*rys4(
     & i1,i2,i3)
      ryx43(i1,i2,i3)=rx(i1,i2,i3)*ryr4(i1,i2,i3)+sx(i1,i2,i3)*rys4(i1,
     & i2,i3)+tx(i1,i2,i3)*ryt4(i1,i2,i3)
      ryy43(i1,i2,i3)=ry(i1,i2,i3)*ryr4(i1,i2,i3)+sy(i1,i2,i3)*rys4(i1,
     & i2,i3)+ty(i1,i2,i3)*ryt4(i1,i2,i3)
      ryz43(i1,i2,i3)=rz(i1,i2,i3)*ryr4(i1,i2,i3)+sz(i1,i2,i3)*rys4(i1,
     & i2,i3)+tz(i1,i2,i3)*ryt4(i1,i2,i3)
      rzx42(i1,i2,i3)= rx(i1,i2,i3)*rzr4(i1,i2,i3)+sx(i1,i2,i3)*rzs4(
     & i1,i2,i3)
      rzy42(i1,i2,i3)= ry(i1,i2,i3)*rzr4(i1,i2,i3)+sy(i1,i2,i3)*rzs4(
     & i1,i2,i3)
      rzx43(i1,i2,i3)=rx(i1,i2,i3)*rzr4(i1,i2,i3)+sx(i1,i2,i3)*rzs4(i1,
     & i2,i3)+tx(i1,i2,i3)*rzt4(i1,i2,i3)
      rzy43(i1,i2,i3)=ry(i1,i2,i3)*rzr4(i1,i2,i3)+sy(i1,i2,i3)*rzs4(i1,
     & i2,i3)+ty(i1,i2,i3)*rzt4(i1,i2,i3)
      rzz43(i1,i2,i3)=rz(i1,i2,i3)*rzr4(i1,i2,i3)+sz(i1,i2,i3)*rzs4(i1,
     & i2,i3)+tz(i1,i2,i3)*rzt4(i1,i2,i3)
      sxx42(i1,i2,i3)= rx(i1,i2,i3)*sxr4(i1,i2,i3)+sx(i1,i2,i3)*sxs4(
     & i1,i2,i3)
      sxy42(i1,i2,i3)= ry(i1,i2,i3)*sxr4(i1,i2,i3)+sy(i1,i2,i3)*sxs4(
     & i1,i2,i3)
      sxx43(i1,i2,i3)=rx(i1,i2,i3)*sxr4(i1,i2,i3)+sx(i1,i2,i3)*sxs4(i1,
     & i2,i3)+tx(i1,i2,i3)*sxt4(i1,i2,i3)
      sxy43(i1,i2,i3)=ry(i1,i2,i3)*sxr4(i1,i2,i3)+sy(i1,i2,i3)*sxs4(i1,
     & i2,i3)+ty(i1,i2,i3)*sxt4(i1,i2,i3)
      sxz43(i1,i2,i3)=rz(i1,i2,i3)*sxr4(i1,i2,i3)+sz(i1,i2,i3)*sxs4(i1,
     & i2,i3)+tz(i1,i2,i3)*sxt4(i1,i2,i3)
      syx42(i1,i2,i3)= rx(i1,i2,i3)*syr4(i1,i2,i3)+sx(i1,i2,i3)*sys4(
     & i1,i2,i3)
      syy42(i1,i2,i3)= ry(i1,i2,i3)*syr4(i1,i2,i3)+sy(i1,i2,i3)*sys4(
     & i1,i2,i3)
      syx43(i1,i2,i3)=rx(i1,i2,i3)*syr4(i1,i2,i3)+sx(i1,i2,i3)*sys4(i1,
     & i2,i3)+tx(i1,i2,i3)*syt4(i1,i2,i3)
      syy43(i1,i2,i3)=ry(i1,i2,i3)*syr4(i1,i2,i3)+sy(i1,i2,i3)*sys4(i1,
     & i2,i3)+ty(i1,i2,i3)*syt4(i1,i2,i3)
      syz43(i1,i2,i3)=rz(i1,i2,i3)*syr4(i1,i2,i3)+sz(i1,i2,i3)*sys4(i1,
     & i2,i3)+tz(i1,i2,i3)*syt4(i1,i2,i3)
      szx42(i1,i2,i3)= rx(i1,i2,i3)*szr4(i1,i2,i3)+sx(i1,i2,i3)*szs4(
     & i1,i2,i3)
      szy42(i1,i2,i3)= ry(i1,i2,i3)*szr4(i1,i2,i3)+sy(i1,i2,i3)*szs4(
     & i1,i2,i3)
      szx43(i1,i2,i3)=rx(i1,i2,i3)*szr4(i1,i2,i3)+sx(i1,i2,i3)*szs4(i1,
     & i2,i3)+tx(i1,i2,i3)*szt4(i1,i2,i3)
      szy43(i1,i2,i3)=ry(i1,i2,i3)*szr4(i1,i2,i3)+sy(i1,i2,i3)*szs4(i1,
     & i2,i3)+ty(i1,i2,i3)*szt4(i1,i2,i3)
      szz43(i1,i2,i3)=rz(i1,i2,i3)*szr4(i1,i2,i3)+sz(i1,i2,i3)*szs4(i1,
     & i2,i3)+tz(i1,i2,i3)*szt4(i1,i2,i3)
      txx42(i1,i2,i3)= rx(i1,i2,i3)*txr4(i1,i2,i3)+sx(i1,i2,i3)*txs4(
     & i1,i2,i3)
      txy42(i1,i2,i3)= ry(i1,i2,i3)*txr4(i1,i2,i3)+sy(i1,i2,i3)*txs4(
     & i1,i2,i3)
      txx43(i1,i2,i3)=rx(i1,i2,i3)*txr4(i1,i2,i3)+sx(i1,i2,i3)*txs4(i1,
     & i2,i3)+tx(i1,i2,i3)*txt4(i1,i2,i3)
      txy43(i1,i2,i3)=ry(i1,i2,i3)*txr4(i1,i2,i3)+sy(i1,i2,i3)*txs4(i1,
     & i2,i3)+ty(i1,i2,i3)*txt4(i1,i2,i3)
      txz43(i1,i2,i3)=rz(i1,i2,i3)*txr4(i1,i2,i3)+sz(i1,i2,i3)*txs4(i1,
     & i2,i3)+tz(i1,i2,i3)*txt4(i1,i2,i3)
      tyx42(i1,i2,i3)= rx(i1,i2,i3)*tyr4(i1,i2,i3)+sx(i1,i2,i3)*tys4(
     & i1,i2,i3)
      tyy42(i1,i2,i3)= ry(i1,i2,i3)*tyr4(i1,i2,i3)+sy(i1,i2,i3)*tys4(
     & i1,i2,i3)
      tyx43(i1,i2,i3)=rx(i1,i2,i3)*tyr4(i1,i2,i3)+sx(i1,i2,i3)*tys4(i1,
     & i2,i3)+tx(i1,i2,i3)*tyt4(i1,i2,i3)
      tyy43(i1,i2,i3)=ry(i1,i2,i3)*tyr4(i1,i2,i3)+sy(i1,i2,i3)*tys4(i1,
     & i2,i3)+ty(i1,i2,i3)*tyt4(i1,i2,i3)
      tyz43(i1,i2,i3)=rz(i1,i2,i3)*tyr4(i1,i2,i3)+sz(i1,i2,i3)*tys4(i1,
     & i2,i3)+tz(i1,i2,i3)*tyt4(i1,i2,i3)
      tzx42(i1,i2,i3)= rx(i1,i2,i3)*tzr4(i1,i2,i3)+sx(i1,i2,i3)*tzs4(
     & i1,i2,i3)
      tzy42(i1,i2,i3)= ry(i1,i2,i3)*tzr4(i1,i2,i3)+sy(i1,i2,i3)*tzs4(
     & i1,i2,i3)
      tzx43(i1,i2,i3)=rx(i1,i2,i3)*tzr4(i1,i2,i3)+sx(i1,i2,i3)*tzs4(i1,
     & i2,i3)+tx(i1,i2,i3)*tzt4(i1,i2,i3)
      tzy43(i1,i2,i3)=ry(i1,i2,i3)*tzr4(i1,i2,i3)+sy(i1,i2,i3)*tzs4(i1,
     & i2,i3)+ty(i1,i2,i3)*tzt4(i1,i2,i3)
      tzz43(i1,i2,i3)=rz(i1,i2,i3)*tzr4(i1,i2,i3)+sz(i1,i2,i3)*tzs4(i1,
     & i2,i3)+tz(i1,i2,i3)*tzt4(i1,i2,i3)
      uxx41(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*urr4(i1,i2,i3,kd)+(rxx42(i1,
     & i2,i3))*ur4(i1,i2,i3,kd)
      uyy41(i1,i2,i3,kd)=0
      uxy41(i1,i2,i3,kd)=0
      uxz41(i1,i2,i3,kd)=0
      uyz41(i1,i2,i3,kd)=0
      uzz41(i1,i2,i3,kd)=0
      ulaplacian41(i1,i2,i3,kd)=uxx41(i1,i2,i3,kd)
      uxx42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*urr4(i1,i2,i3,kd)+2.*(rx(i1,
     & i2,i3)*sx(i1,i2,i3))*urs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2)*uss4(
     & i1,i2,i3,kd)+(rxx42(i1,i2,i3))*ur4(i1,i2,i3,kd)+(sxx42(i1,i2,
     & i3))*us4(i1,i2,i3,kd)
      uyy42(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*urr4(i1,i2,i3,kd)+2.*(ry(i1,
     & i2,i3)*sy(i1,i2,i3))*urs4(i1,i2,i3,kd)+(sy(i1,i2,i3)**2)*uss4(
     & i1,i2,i3,kd)+(ryy42(i1,i2,i3))*ur4(i1,i2,i3,kd)+(syy42(i1,i2,
     & i3))*us4(i1,i2,i3,kd)
      uxy42(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*urr4(i1,i2,i3,kd)+(
     & rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*urs4(i1,
     & i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*uss4(i1,i2,i3,kd)+rxy42(i1,
     & i2,i3)*ur4(i1,i2,i3,kd)+sxy42(i1,i2,i3)*us4(i1,i2,i3,kd)
      uxz42(i1,i2,i3,kd)=0
      uyz42(i1,i2,i3,kd)=0
      uzz42(i1,i2,i3,kd)=0
      ulaplacian42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*urr4(
     & i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,
     & i2,i3))*urs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2)*
     & uss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3)+ryy42(i1,i2,i3))*ur4(i1,i2,
     & i3,kd)+(sxx42(i1,i2,i3)+syy42(i1,i2,i3))*us4(i1,i2,i3,kd)
      uxx43(i1,i2,i3,kd)=rx(i1,i2,i3)**2*urr4(i1,i2,i3,kd)+sx(i1,i2,i3)
     & **2*uss4(i1,i2,i3,kd)+tx(i1,i2,i3)**2*utt4(i1,i2,i3,kd)+2.*rx(
     & i1,i2,i3)*sx(i1,i2,i3)*urs4(i1,i2,i3,kd)+2.*rx(i1,i2,i3)*tx(i1,
     & i2,i3)*urt4(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,i2,i3)*ust4(i1,
     & i2,i3,kd)+rxx43(i1,i2,i3)*ur4(i1,i2,i3,kd)+sxx43(i1,i2,i3)*us4(
     & i1,i2,i3,kd)+txx43(i1,i2,i3)*ut4(i1,i2,i3,kd)
      uyy43(i1,i2,i3,kd)=ry(i1,i2,i3)**2*urr4(i1,i2,i3,kd)+sy(i1,i2,i3)
     & **2*uss4(i1,i2,i3,kd)+ty(i1,i2,i3)**2*utt4(i1,i2,i3,kd)+2.*ry(
     & i1,i2,i3)*sy(i1,i2,i3)*urs4(i1,i2,i3,kd)+2.*ry(i1,i2,i3)*ty(i1,
     & i2,i3)*urt4(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,i2,i3)*ust4(i1,
     & i2,i3,kd)+ryy43(i1,i2,i3)*ur4(i1,i2,i3,kd)+syy43(i1,i2,i3)*us4(
     & i1,i2,i3,kd)+tyy43(i1,i2,i3)*ut4(i1,i2,i3,kd)
      uzz43(i1,i2,i3,kd)=rz(i1,i2,i3)**2*urr4(i1,i2,i3,kd)+sz(i1,i2,i3)
     & **2*uss4(i1,i2,i3,kd)+tz(i1,i2,i3)**2*utt4(i1,i2,i3,kd)+2.*rz(
     & i1,i2,i3)*sz(i1,i2,i3)*urs4(i1,i2,i3,kd)+2.*rz(i1,i2,i3)*tz(i1,
     & i2,i3)*urt4(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,i2,i3)*ust4(i1,
     & i2,i3,kd)+rzz43(i1,i2,i3)*ur4(i1,i2,i3,kd)+szz43(i1,i2,i3)*us4(
     & i1,i2,i3,kd)+tzz43(i1,i2,i3)*ut4(i1,i2,i3,kd)
      uxy43(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*urr4(i1,i2,i3,kd)+
     & sx(i1,i2,i3)*sy(i1,i2,i3)*uss4(i1,i2,i3,kd)+tx(i1,i2,i3)*ty(i1,
     & i2,i3)*utt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,
     & i3)*sx(i1,i2,i3))*urs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(i1,i2,i3)+
     & ry(i1,i2,i3)*tx(i1,i2,i3))*urt4(i1,i2,i3,kd)+(sx(i1,i2,i3)*ty(
     & i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*ust4(i1,i2,i3,kd)+rxy43(
     & i1,i2,i3)*ur4(i1,i2,i3,kd)+sxy43(i1,i2,i3)*us4(i1,i2,i3,kd)+
     & txy43(i1,i2,i3)*ut4(i1,i2,i3,kd)
      uxz43(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*urr4(i1,i2,i3,kd)+
     & sx(i1,i2,i3)*sz(i1,i2,i3)*uss4(i1,i2,i3,kd)+tx(i1,i2,i3)*tz(i1,
     & i2,i3)*utt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,i2,
     & i3)*sx(i1,i2,i3))*urs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(i1,i2,i3)+
     & rz(i1,i2,i3)*tx(i1,i2,i3))*urt4(i1,i2,i3,kd)+(sx(i1,i2,i3)*tz(
     & i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*ust4(i1,i2,i3,kd)+rxz43(
     & i1,i2,i3)*ur4(i1,i2,i3,kd)+sxz43(i1,i2,i3)*us4(i1,i2,i3,kd)+
     & txz43(i1,i2,i3)*ut4(i1,i2,i3,kd)
      uyz43(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*urr4(i1,i2,i3,kd)+
     & sy(i1,i2,i3)*sz(i1,i2,i3)*uss4(i1,i2,i3,kd)+ty(i1,i2,i3)*tz(i1,
     & i2,i3)*utt4(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,i2,
     & i3)*sy(i1,i2,i3))*urs4(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(i1,i2,i3)+
     & rz(i1,i2,i3)*ty(i1,i2,i3))*urt4(i1,i2,i3,kd)+(sy(i1,i2,i3)*tz(
     & i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*ust4(i1,i2,i3,kd)+ryz43(
     & i1,i2,i3)*ur4(i1,i2,i3,kd)+syz43(i1,i2,i3)*us4(i1,i2,i3,kd)+
     & tyz43(i1,i2,i3)*ut4(i1,i2,i3,kd)
      ulaplacian43(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(i1,
     & i2,i3)**2)*urr4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2+
     & sz(i1,i2,i3)**2)*uss4(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(i1,i2,
     & i3)**2+tz(i1,i2,i3)**2)*utt4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(
     & i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,i3))
     & *urs4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(i1,i2,i3)*
     & ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*urt4(i1,i2,i3,kd)+2.*(
     & sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)+sz(i1,i2,
     & i3)*tz(i1,i2,i3))*ust4(i1,i2,i3,kd)+(rxx43(i1,i2,i3)+ryy43(i1,
     & i2,i3)+rzz43(i1,i2,i3))*ur4(i1,i2,i3,kd)+(sxx43(i1,i2,i3)+
     & syy43(i1,i2,i3)+szz43(i1,i2,i3))*us4(i1,i2,i3,kd)+(txx43(i1,i2,
     & i3)+tyy43(i1,i2,i3)+tzz43(i1,i2,i3))*ut4(i1,i2,i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      h41(kd) = 1./(12.*dx(kd))
      h42(kd) = 1./(12.*dx(kd)**2)
      ux43r(i1,i2,i3,kd)=(8.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))-(u(i1+
     & 2,i2,i3,kd)-u(i1-2,i2,i3,kd)))*h41(0)
      uy43r(i1,i2,i3,kd)=(8.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))-(u(i1,
     & i2+2,i3,kd)-u(i1,i2-2,i3,kd)))*h41(1)
      uz43r(i1,i2,i3,kd)=(8.*(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))-(u(i1,
     & i2,i3+2,kd)-u(i1,i2,i3-2,kd)))*h41(2)
      uxx43r(i1,i2,i3,kd)=( -30.*u(i1,i2,i3,kd)+16.*(u(i1+1,i2,i3,kd)+
     & u(i1-1,i2,i3,kd))-(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)) )*h42(0)
      uyy43r(i1,i2,i3,kd)=( -30.*u(i1,i2,i3,kd)+16.*(u(i1,i2+1,i3,kd)+
     & u(i1,i2-1,i3,kd))-(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )*h42(1)
      uzz43r(i1,i2,i3,kd)=( -30.*u(i1,i2,i3,kd)+16.*(u(i1,i2,i3+1,kd)+
     & u(i1,i2,i3-1,kd))-(u(i1,i2,i3+2,kd)+u(i1,i2,i3-2,kd)) )*h42(2)
      uxy43r(i1,i2,i3,kd)=( (u(i1+2,i2+2,i3,kd)-u(i1-2,i2+2,i3,kd)- u(
     & i1+2,i2-2,i3,kd)+u(i1-2,i2-2,i3,kd)) +8.*(u(i1-1,i2+2,i3,kd)-u(
     & i1-1,i2-2,i3,kd)-u(i1+1,i2+2,i3,kd)+u(i1+1,i2-2,i3,kd) +u(i1+2,
     & i2-1,i3,kd)-u(i1-2,i2-1,i3,kd)-u(i1+2,i2+1,i3,kd)+u(i1-2,i2+1,
     & i3,kd))+64.*(u(i1+1,i2+1,i3,kd)-u(i1-1,i2+1,i3,kd)- u(i1+1,i2-
     & 1,i3,kd)+u(i1-1,i2-1,i3,kd)))*(h41(0)*h41(1))
      uxz43r(i1,i2,i3,kd)=( (u(i1+2,i2,i3+2,kd)-u(i1-2,i2,i3+2,kd)-u(
     & i1+2,i2,i3-2,kd)+u(i1-2,i2,i3-2,kd)) +8.*(u(i1-1,i2,i3+2,kd)-u(
     & i1-1,i2,i3-2,kd)-u(i1+1,i2,i3+2,kd)+u(i1+1,i2,i3-2,kd) +u(i1+2,
     & i2,i3-1,kd)-u(i1-2,i2,i3-1,kd)- u(i1+2,i2,i3+1,kd)+u(i1-2,i2,
     & i3+1,kd)) +64.*(u(i1+1,i2,i3+1,kd)-u(i1-1,i2,i3+1,kd)-u(i1+1,
     & i2,i3-1,kd)+u(i1-1,i2,i3-1,kd)) )*(h41(0)*h41(2))
      uyz43r(i1,i2,i3,kd)=( (u(i1,i2+2,i3+2,kd)-u(i1,i2-2,i3+2,kd)-u(
     & i1,i2+2,i3-2,kd)+u(i1,i2-2,i3-2,kd)) +8.*(u(i1,i2-1,i3+2,kd)-u(
     & i1,i2-1,i3-2,kd)-u(i1,i2+1,i3+2,kd)+u(i1,i2+1,i3-2,kd) +u(i1,
     & i2+2,i3-1,kd)-u(i1,i2-2,i3-1,kd)-u(i1,i2+2,i3+1,kd)+u(i1,i2-2,
     & i3+1,kd)) +64.*(u(i1,i2+1,i3+1,kd)-u(i1,i2-1,i3+1,kd)-u(i1,i2+
     & 1,i3-1,kd)+u(i1,i2-1,i3-1,kd)) )*(h41(1)*h41(2))
      ux41r(i1,i2,i3,kd)= ux43r(i1,i2,i3,kd)
      uy41r(i1,i2,i3,kd)= uy43r(i1,i2,i3,kd)
      uz41r(i1,i2,i3,kd)= uz43r(i1,i2,i3,kd)
      uxx41r(i1,i2,i3,kd)= uxx43r(i1,i2,i3,kd)
      uyy41r(i1,i2,i3,kd)= uyy43r(i1,i2,i3,kd)
      uzz41r(i1,i2,i3,kd)= uzz43r(i1,i2,i3,kd)
      uxy41r(i1,i2,i3,kd)= uxy43r(i1,i2,i3,kd)
      uxz41r(i1,i2,i3,kd)= uxz43r(i1,i2,i3,kd)
      uyz41r(i1,i2,i3,kd)= uyz43r(i1,i2,i3,kd)
      ulaplacian41r(i1,i2,i3,kd)=uxx43r(i1,i2,i3,kd)
      ux42r(i1,i2,i3,kd)= ux43r(i1,i2,i3,kd)
      uy42r(i1,i2,i3,kd)= uy43r(i1,i2,i3,kd)
      uz42r(i1,i2,i3,kd)= uz43r(i1,i2,i3,kd)
      uxx42r(i1,i2,i3,kd)= uxx43r(i1,i2,i3,kd)
      uyy42r(i1,i2,i3,kd)= uyy43r(i1,i2,i3,kd)
      uzz42r(i1,i2,i3,kd)= uzz43r(i1,i2,i3,kd)
      uxy42r(i1,i2,i3,kd)= uxy43r(i1,i2,i3,kd)
      uxz42r(i1,i2,i3,kd)= uxz43r(i1,i2,i3,kd)
      uyz42r(i1,i2,i3,kd)= uyz43r(i1,i2,i3,kd)
      ulaplacian42r(i1,i2,i3,kd)=uxx43r(i1,i2,i3,kd)+uyy43r(i1,i2,i3,
     & kd)
      ulaplacian43r(i1,i2,i3,kd)=uxx43r(i1,i2,i3,kd)+uyy43r(i1,i2,i3,
     & kd)+uzz43r(i1,i2,i3,kd)
      umr4(i1,i2,i3,kd)=(8.*(um(i1+1,i2,i3,kd)-um(i1-1,i2,i3,kd))-(um(
     & i1+2,i2,i3,kd)-um(i1-2,i2,i3,kd)))*d14(0)
      ums4(i1,i2,i3,kd)=(8.*(um(i1,i2+1,i3,kd)-um(i1,i2-1,i3,kd))-(um(
     & i1,i2+2,i3,kd)-um(i1,i2-2,i3,kd)))*d14(1)
      umt4(i1,i2,i3,kd)=(8.*(um(i1,i2,i3+1,kd)-um(i1,i2,i3-1,kd))-(um(
     & i1,i2,i3+2,kd)-um(i1,i2,i3-2,kd)))*d14(2)
      umrr4(i1,i2,i3,kd)=(-30.*um(i1,i2,i3,kd)+16.*(um(i1+1,i2,i3,kd)+
     & um(i1-1,i2,i3,kd))-(um(i1+2,i2,i3,kd)+um(i1-2,i2,i3,kd)) )*d24(
     & 0)
      umss4(i1,i2,i3,kd)=(-30.*um(i1,i2,i3,kd)+16.*(um(i1,i2+1,i3,kd)+
     & um(i1,i2-1,i3,kd))-(um(i1,i2+2,i3,kd)+um(i1,i2-2,i3,kd)) )*d24(
     & 1)
      umtt4(i1,i2,i3,kd)=(-30.*um(i1,i2,i3,kd)+16.*(um(i1,i2,i3+1,kd)+
     & um(i1,i2,i3-1,kd))-(um(i1,i2,i3+2,kd)+um(i1,i2,i3-2,kd)) )*d24(
     & 2)
      umrs4(i1,i2,i3,kd)=(8.*(umr4(i1,i2+1,i3,kd)-umr4(i1,i2-1,i3,kd))-
     & (umr4(i1,i2+2,i3,kd)-umr4(i1,i2-2,i3,kd)))*d14(1)
      umrt4(i1,i2,i3,kd)=(8.*(umr4(i1,i2,i3+1,kd)-umr4(i1,i2,i3-1,kd))-
     & (umr4(i1,i2,i3+2,kd)-umr4(i1,i2,i3-2,kd)))*d14(2)
      umst4(i1,i2,i3,kd)=(8.*(ums4(i1,i2,i3+1,kd)-ums4(i1,i2,i3-1,kd))-
     & (ums4(i1,i2,i3+2,kd)-ums4(i1,i2,i3-2,kd)))*d14(2)
      umx41(i1,i2,i3,kd)= rx(i1,i2,i3)*umr4(i1,i2,i3,kd)
      umy41(i1,i2,i3,kd)=0
      umz41(i1,i2,i3,kd)=0
      umx42(i1,i2,i3,kd)= rx(i1,i2,i3)*umr4(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & ums4(i1,i2,i3,kd)
      umy42(i1,i2,i3,kd)= ry(i1,i2,i3)*umr4(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & ums4(i1,i2,i3,kd)
      umz42(i1,i2,i3,kd)=0
      umx43(i1,i2,i3,kd)=rx(i1,i2,i3)*umr4(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & ums4(i1,i2,i3,kd)+tx(i1,i2,i3)*umt4(i1,i2,i3,kd)
      umy43(i1,i2,i3,kd)=ry(i1,i2,i3)*umr4(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & ums4(i1,i2,i3,kd)+ty(i1,i2,i3)*umt4(i1,i2,i3,kd)
      umz43(i1,i2,i3,kd)=rz(i1,i2,i3)*umr4(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & ums4(i1,i2,i3,kd)+tz(i1,i2,i3)*umt4(i1,i2,i3,kd)
      umxx41(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*umrr4(i1,i2,i3,kd)+(rxx42(
     & i1,i2,i3))*umr4(i1,i2,i3,kd)
      umyy41(i1,i2,i3,kd)=0
      umxy41(i1,i2,i3,kd)=0
      umxz41(i1,i2,i3,kd)=0
      umyz41(i1,i2,i3,kd)=0
      umzz41(i1,i2,i3,kd)=0
      umlaplacian41(i1,i2,i3,kd)=umxx41(i1,i2,i3,kd)
      umxx42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*umrr4(i1,i2,i3,kd)+2.*(rx(
     & i1,i2,i3)*sx(i1,i2,i3))*umrs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2)*
     & umss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3))*umr4(i1,i2,i3,kd)+(sxx42(
     & i1,i2,i3))*ums4(i1,i2,i3,kd)
      umyy42(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*umrr4(i1,i2,i3,kd)+2.*(ry(
     & i1,i2,i3)*sy(i1,i2,i3))*umrs4(i1,i2,i3,kd)+(sy(i1,i2,i3)**2)*
     & umss4(i1,i2,i3,kd)+(ryy42(i1,i2,i3))*umr4(i1,i2,i3,kd)+(syy42(
     & i1,i2,i3))*ums4(i1,i2,i3,kd)
      umxy42(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*umrr4(i1,i2,i3,kd)+
     & (rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*umrs4(i1,
     & i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*umss4(i1,i2,i3,kd)+rxy42(
     & i1,i2,i3)*umr4(i1,i2,i3,kd)+sxy42(i1,i2,i3)*ums4(i1,i2,i3,kd)
      umxz42(i1,i2,i3,kd)=0
      umyz42(i1,i2,i3,kd)=0
      umzz42(i1,i2,i3,kd)=0
      umlaplacian42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & umrr4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*
     & sy(i1,i2,i3))*umrs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)*
     & *2)*umss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3)+ryy42(i1,i2,i3))*umr4(
     & i1,i2,i3,kd)+(sxx42(i1,i2,i3)+syy42(i1,i2,i3))*ums4(i1,i2,i3,
     & kd)
      umxx43(i1,i2,i3,kd)=rx(i1,i2,i3)**2*umrr4(i1,i2,i3,kd)+sx(i1,i2,
     & i3)**2*umss4(i1,i2,i3,kd)+tx(i1,i2,i3)**2*umtt4(i1,i2,i3,kd)+
     & 2.*rx(i1,i2,i3)*sx(i1,i2,i3)*umrs4(i1,i2,i3,kd)+2.*rx(i1,i2,i3)
     & *tx(i1,i2,i3)*umrt4(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,i2,i3)*
     & umst4(i1,i2,i3,kd)+rxx43(i1,i2,i3)*umr4(i1,i2,i3,kd)+sxx43(i1,
     & i2,i3)*ums4(i1,i2,i3,kd)+txx43(i1,i2,i3)*umt4(i1,i2,i3,kd)
      umyy43(i1,i2,i3,kd)=ry(i1,i2,i3)**2*umrr4(i1,i2,i3,kd)+sy(i1,i2,
     & i3)**2*umss4(i1,i2,i3,kd)+ty(i1,i2,i3)**2*umtt4(i1,i2,i3,kd)+
     & 2.*ry(i1,i2,i3)*sy(i1,i2,i3)*umrs4(i1,i2,i3,kd)+2.*ry(i1,i2,i3)
     & *ty(i1,i2,i3)*umrt4(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,i2,i3)*
     & umst4(i1,i2,i3,kd)+ryy43(i1,i2,i3)*umr4(i1,i2,i3,kd)+syy43(i1,
     & i2,i3)*ums4(i1,i2,i3,kd)+tyy43(i1,i2,i3)*umt4(i1,i2,i3,kd)
      umzz43(i1,i2,i3,kd)=rz(i1,i2,i3)**2*umrr4(i1,i2,i3,kd)+sz(i1,i2,
     & i3)**2*umss4(i1,i2,i3,kd)+tz(i1,i2,i3)**2*umtt4(i1,i2,i3,kd)+
     & 2.*rz(i1,i2,i3)*sz(i1,i2,i3)*umrs4(i1,i2,i3,kd)+2.*rz(i1,i2,i3)
     & *tz(i1,i2,i3)*umrt4(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,i2,i3)*
     & umst4(i1,i2,i3,kd)+rzz43(i1,i2,i3)*umr4(i1,i2,i3,kd)+szz43(i1,
     & i2,i3)*ums4(i1,i2,i3,kd)+tzz43(i1,i2,i3)*umt4(i1,i2,i3,kd)
      umxy43(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*umrr4(i1,i2,i3,kd)+
     & sx(i1,i2,i3)*sy(i1,i2,i3)*umss4(i1,i2,i3,kd)+tx(i1,i2,i3)*ty(
     & i1,i2,i3)*umtt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,
     & i2,i3)*sx(i1,i2,i3))*umrs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(i1,i2,
     & i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*umrt4(i1,i2,i3,kd)+(sx(i1,i2,i3)
     & *ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*umst4(i1,i2,i3,kd)+
     & rxy43(i1,i2,i3)*umr4(i1,i2,i3,kd)+sxy43(i1,i2,i3)*ums4(i1,i2,
     & i3,kd)+txy43(i1,i2,i3)*umt4(i1,i2,i3,kd)
      umxz43(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*umrr4(i1,i2,i3,kd)+
     & sx(i1,i2,i3)*sz(i1,i2,i3)*umss4(i1,i2,i3,kd)+tx(i1,i2,i3)*tz(
     & i1,i2,i3)*umtt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,
     & i2,i3)*sx(i1,i2,i3))*umrs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(i1,i2,
     & i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*umrt4(i1,i2,i3,kd)+(sx(i1,i2,i3)
     & *tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*umst4(i1,i2,i3,kd)+
     & rxz43(i1,i2,i3)*umr4(i1,i2,i3,kd)+sxz43(i1,i2,i3)*ums4(i1,i2,
     & i3,kd)+txz43(i1,i2,i3)*umt4(i1,i2,i3,kd)
      umyz43(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*umrr4(i1,i2,i3,kd)+
     & sy(i1,i2,i3)*sz(i1,i2,i3)*umss4(i1,i2,i3,kd)+ty(i1,i2,i3)*tz(
     & i1,i2,i3)*umtt4(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(i1,
     & i2,i3)*sy(i1,i2,i3))*umrs4(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(i1,i2,
     & i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*umrt4(i1,i2,i3,kd)+(sy(i1,i2,i3)
     & *tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*umst4(i1,i2,i3,kd)+
     & ryz43(i1,i2,i3)*umr4(i1,i2,i3,kd)+syz43(i1,i2,i3)*ums4(i1,i2,
     & i3,kd)+tyz43(i1,i2,i3)*umt4(i1,i2,i3,kd)
      umlaplacian43(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*umrr4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)*
     & *2+sz(i1,i2,i3)**2)*umss4(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(i1,
     & i2,i3)**2+tz(i1,i2,i3)**2)*umtt4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*
     & sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,
     & i3))*umrs4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(i1,
     & i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*umrt4(i1,i2,i3,
     & kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)+
     & sz(i1,i2,i3)*tz(i1,i2,i3))*umst4(i1,i2,i3,kd)+(rxx43(i1,i2,i3)+
     & ryy43(i1,i2,i3)+rzz43(i1,i2,i3))*umr4(i1,i2,i3,kd)+(sxx43(i1,
     & i2,i3)+syy43(i1,i2,i3)+szz43(i1,i2,i3))*ums4(i1,i2,i3,kd)+(
     & txx43(i1,i2,i3)+tyy43(i1,i2,i3)+tzz43(i1,i2,i3))*umt4(i1,i2,i3,
     & kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      umx43r(i1,i2,i3,kd)=(8.*(um(i1+1,i2,i3,kd)-um(i1-1,i2,i3,kd))-(
     & um(i1+2,i2,i3,kd)-um(i1-2,i2,i3,kd)))*h41(0)
      umy43r(i1,i2,i3,kd)=(8.*(um(i1,i2+1,i3,kd)-um(i1,i2-1,i3,kd))-(
     & um(i1,i2+2,i3,kd)-um(i1,i2-2,i3,kd)))*h41(1)
      umz43r(i1,i2,i3,kd)=(8.*(um(i1,i2,i3+1,kd)-um(i1,i2,i3-1,kd))-(
     & um(i1,i2,i3+2,kd)-um(i1,i2,i3-2,kd)))*h41(2)
      umxx43r(i1,i2,i3,kd)=( -30.*um(i1,i2,i3,kd)+16.*(um(i1+1,i2,i3,
     & kd)+um(i1-1,i2,i3,kd))-(um(i1+2,i2,i3,kd)+um(i1-2,i2,i3,kd)) )*
     & h42(0)
      umyy43r(i1,i2,i3,kd)=( -30.*um(i1,i2,i3,kd)+16.*(um(i1,i2+1,i3,
     & kd)+um(i1,i2-1,i3,kd))-(um(i1,i2+2,i3,kd)+um(i1,i2-2,i3,kd)) )*
     & h42(1)
      umzz43r(i1,i2,i3,kd)=( -30.*um(i1,i2,i3,kd)+16.*(um(i1,i2,i3+1,
     & kd)+um(i1,i2,i3-1,kd))-(um(i1,i2,i3+2,kd)+um(i1,i2,i3-2,kd)) )*
     & h42(2)
      umxy43r(i1,i2,i3,kd)=( (um(i1+2,i2+2,i3,kd)-um(i1-2,i2+2,i3,kd)- 
     & um(i1+2,i2-2,i3,kd)+um(i1-2,i2-2,i3,kd)) +8.*(um(i1-1,i2+2,i3,
     & kd)-um(i1-1,i2-2,i3,kd)-um(i1+1,i2+2,i3,kd)+um(i1+1,i2-2,i3,kd)
     &  +um(i1+2,i2-1,i3,kd)-um(i1-2,i2-1,i3,kd)-um(i1+2,i2+1,i3,kd)+
     & um(i1-2,i2+1,i3,kd))+64.*(um(i1+1,i2+1,i3,kd)-um(i1-1,i2+1,i3,
     & kd)- um(i1+1,i2-1,i3,kd)+um(i1-1,i2-1,i3,kd)))*(h41(0)*h41(1))
      umxz43r(i1,i2,i3,kd)=( (um(i1+2,i2,i3+2,kd)-um(i1-2,i2,i3+2,kd)-
     & um(i1+2,i2,i3-2,kd)+um(i1-2,i2,i3-2,kd)) +8.*(um(i1-1,i2,i3+2,
     & kd)-um(i1-1,i2,i3-2,kd)-um(i1+1,i2,i3+2,kd)+um(i1+1,i2,i3-2,kd)
     &  +um(i1+2,i2,i3-1,kd)-um(i1-2,i2,i3-1,kd)- um(i1+2,i2,i3+1,kd)+
     & um(i1-2,i2,i3+1,kd)) +64.*(um(i1+1,i2,i3+1,kd)-um(i1-1,i2,i3+1,
     & kd)-um(i1+1,i2,i3-1,kd)+um(i1-1,i2,i3-1,kd)) )*(h41(0)*h41(2))
      umyz43r(i1,i2,i3,kd)=( (um(i1,i2+2,i3+2,kd)-um(i1,i2-2,i3+2,kd)-
     & um(i1,i2+2,i3-2,kd)+um(i1,i2-2,i3-2,kd)) +8.*(um(i1,i2-1,i3+2,
     & kd)-um(i1,i2-1,i3-2,kd)-um(i1,i2+1,i3+2,kd)+um(i1,i2+1,i3-2,kd)
     &  +um(i1,i2+2,i3-1,kd)-um(i1,i2-2,i3-1,kd)-um(i1,i2+2,i3+1,kd)+
     & um(i1,i2-2,i3+1,kd)) +64.*(um(i1,i2+1,i3+1,kd)-um(i1,i2-1,i3+1,
     & kd)-um(i1,i2+1,i3-1,kd)+um(i1,i2-1,i3-1,kd)) )*(h41(1)*h41(2))
      umx41r(i1,i2,i3,kd)= umx43r(i1,i2,i3,kd)
      umy41r(i1,i2,i3,kd)= umy43r(i1,i2,i3,kd)
      umz41r(i1,i2,i3,kd)= umz43r(i1,i2,i3,kd)
      umxx41r(i1,i2,i3,kd)= umxx43r(i1,i2,i3,kd)
      umyy41r(i1,i2,i3,kd)= umyy43r(i1,i2,i3,kd)
      umzz41r(i1,i2,i3,kd)= umzz43r(i1,i2,i3,kd)
      umxy41r(i1,i2,i3,kd)= umxy43r(i1,i2,i3,kd)
      umxz41r(i1,i2,i3,kd)= umxz43r(i1,i2,i3,kd)
      umyz41r(i1,i2,i3,kd)= umyz43r(i1,i2,i3,kd)
      umlaplacian41r(i1,i2,i3,kd)=umxx43r(i1,i2,i3,kd)
      umx42r(i1,i2,i3,kd)= umx43r(i1,i2,i3,kd)
      umy42r(i1,i2,i3,kd)= umy43r(i1,i2,i3,kd)
      umz42r(i1,i2,i3,kd)= umz43r(i1,i2,i3,kd)
      umxx42r(i1,i2,i3,kd)= umxx43r(i1,i2,i3,kd)
      umyy42r(i1,i2,i3,kd)= umyy43r(i1,i2,i3,kd)
      umzz42r(i1,i2,i3,kd)= umzz43r(i1,i2,i3,kd)
      umxy42r(i1,i2,i3,kd)= umxy43r(i1,i2,i3,kd)
      umxz42r(i1,i2,i3,kd)= umxz43r(i1,i2,i3,kd)
      umyz42r(i1,i2,i3,kd)= umyz43r(i1,i2,i3,kd)
      umlaplacian42r(i1,i2,i3,kd)=umxx43r(i1,i2,i3,kd)+umyy43r(i1,i2,
     & i3,kd)
      umlaplacian43r(i1,i2,i3,kd)=umxx43r(i1,i2,i3,kd)+umyy43r(i1,i2,
     & i3,kd)+umzz43r(i1,i2,i3,kd)

      vrar4(i1,i2,i3,kd)=(8.*(vra(i1+1,i2,i3,kd)-vra(i1-1,i2,i3,kd))-(
     & vra(i1+2,i2,i3,kd)-vra(i1-2,i2,i3,kd)))*d14(0)
      vras4(i1,i2,i3,kd)=(8.*(vra(i1,i2+1,i3,kd)-vra(i1,i2-1,i3,kd))-(
     & vra(i1,i2+2,i3,kd)-vra(i1,i2-2,i3,kd)))*d14(1)
      vrat4(i1,i2,i3,kd)=(8.*(vra(i1,i2,i3+1,kd)-vra(i1,i2,i3-1,kd))-(
     & vra(i1,i2,i3+2,kd)-vra(i1,i2,i3-2,kd)))*d14(2)
      vrarr4(i1,i2,i3,kd)=(-30.*vra(i1,i2,i3,kd)+16.*(vra(i1+1,i2,i3,
     & kd)+vra(i1-1,i2,i3,kd))-(vra(i1+2,i2,i3,kd)+vra(i1-2,i2,i3,kd))
     &  )*d24(0)
      vrass4(i1,i2,i3,kd)=(-30.*vra(i1,i2,i3,kd)+16.*(vra(i1,i2+1,i3,
     & kd)+vra(i1,i2-1,i3,kd))-(vra(i1,i2+2,i3,kd)+vra(i1,i2-2,i3,kd))
     &  )*d24(1)
      vratt4(i1,i2,i3,kd)=(-30.*vra(i1,i2,i3,kd)+16.*(vra(i1,i2,i3+1,
     & kd)+vra(i1,i2,i3-1,kd))-(vra(i1,i2,i3+2,kd)+vra(i1,i2,i3-2,kd))
     &  )*d24(2)
      vrars4(i1,i2,i3,kd)=(8.*(vrar4(i1,i2+1,i3,kd)-vrar4(i1,i2-1,i3,
     & kd))-(vrar4(i1,i2+2,i3,kd)-vrar4(i1,i2-2,i3,kd)))*d14(1)
      vrart4(i1,i2,i3,kd)=(8.*(vrar4(i1,i2,i3+1,kd)-vrar4(i1,i2,i3-1,
     & kd))-(vrar4(i1,i2,i3+2,kd)-vrar4(i1,i2,i3-2,kd)))*d14(2)
      vrast4(i1,i2,i3,kd)=(8.*(vras4(i1,i2,i3+1,kd)-vras4(i1,i2,i3-1,
     & kd))-(vras4(i1,i2,i3+2,kd)-vras4(i1,i2,i3-2,kd)))*d14(2)
      vrax41(i1,i2,i3,kd)= rx(i1,i2,i3)*vrar4(i1,i2,i3,kd)
      vray41(i1,i2,i3,kd)=0
      vraz41(i1,i2,i3,kd)=0
      vrax42(i1,i2,i3,kd)= rx(i1,i2,i3)*vrar4(i1,i2,i3,kd)+sx(i1,i2,i3)
     & *vras4(i1,i2,i3,kd)
      vray42(i1,i2,i3,kd)= ry(i1,i2,i3)*vrar4(i1,i2,i3,kd)+sy(i1,i2,i3)
     & *vras4(i1,i2,i3,kd)
      vraz42(i1,i2,i3,kd)=0
      vrax43(i1,i2,i3,kd)=rx(i1,i2,i3)*vrar4(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & vras4(i1,i2,i3,kd)+tx(i1,i2,i3)*vrat4(i1,i2,i3,kd)
      vray43(i1,i2,i3,kd)=ry(i1,i2,i3)*vrar4(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & vras4(i1,i2,i3,kd)+ty(i1,i2,i3)*vrat4(i1,i2,i3,kd)
      vraz43(i1,i2,i3,kd)=rz(i1,i2,i3)*vrar4(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & vras4(i1,i2,i3,kd)+tz(i1,i2,i3)*vrat4(i1,i2,i3,kd)
      vraxx41(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vrarr4(i1,i2,i3,kd)+(
     & rxx42(i1,i2,i3))*vrar4(i1,i2,i3,kd)
      vrayy41(i1,i2,i3,kd)=0
      vraxy41(i1,i2,i3,kd)=0
      vraxz41(i1,i2,i3,kd)=0
      vrayz41(i1,i2,i3,kd)=0
      vrazz41(i1,i2,i3,kd)=0
      vralaplacian41(i1,i2,i3,kd)=vraxx41(i1,i2,i3,kd)
      vraxx42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vrarr4(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*vrars4(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*vrass4(i1,i2,i3,kd)+(rxx42(i1,i2,i3))*vrar4(i1,i2,i3,kd)+(
     & sxx42(i1,i2,i3))*vras4(i1,i2,i3,kd)
      vrayy42(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*vrarr4(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*vrars4(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*vrass4(i1,i2,i3,kd)+(ryy42(i1,i2,i3))*vrar4(i1,i2,i3,kd)+(
     & syy42(i1,i2,i3))*vras4(i1,i2,i3,kd)
      vraxy42(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vrarr4(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & vrars4(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vrass4(i1,i2,i3,
     & kd)+rxy42(i1,i2,i3)*vrar4(i1,i2,i3,kd)+sxy42(i1,i2,i3)*vras4(
     & i1,i2,i3,kd)
      vraxz42(i1,i2,i3,kd)=0
      vrayz42(i1,i2,i3,kd)=0
      vrazz42(i1,i2,i3,kd)=0
      vralaplacian42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & vrarr4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)
     & *sy(i1,i2,i3))*vrars4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2)*vrass4(i1,i2,i3,kd)+(rxx42(i1,i2,i3)+ryy42(i1,i2,i3))*
     & vrar4(i1,i2,i3,kd)+(sxx42(i1,i2,i3)+syy42(i1,i2,i3))*vras4(i1,
     & i2,i3,kd)
      vraxx43(i1,i2,i3,kd)=rx(i1,i2,i3)**2*vrarr4(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*vrass4(i1,i2,i3,kd)+tx(i1,i2,i3)**2*vratt4(i1,i2,i3,
     & kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*vrars4(i1,i2,i3,kd)+2.*rx(i1,
     & i2,i3)*tx(i1,i2,i3)*vrart4(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,
     & i2,i3)*vrast4(i1,i2,i3,kd)+rxx43(i1,i2,i3)*vrar4(i1,i2,i3,kd)+
     & sxx43(i1,i2,i3)*vras4(i1,i2,i3,kd)+txx43(i1,i2,i3)*vrat4(i1,i2,
     & i3,kd)
      vrayy43(i1,i2,i3,kd)=ry(i1,i2,i3)**2*vrarr4(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*vrass4(i1,i2,i3,kd)+ty(i1,i2,i3)**2*vratt4(i1,i2,i3,
     & kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*vrars4(i1,i2,i3,kd)+2.*ry(i1,
     & i2,i3)*ty(i1,i2,i3)*vrart4(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,
     & i2,i3)*vrast4(i1,i2,i3,kd)+ryy43(i1,i2,i3)*vrar4(i1,i2,i3,kd)+
     & syy43(i1,i2,i3)*vras4(i1,i2,i3,kd)+tyy43(i1,i2,i3)*vrat4(i1,i2,
     & i3,kd)
      vrazz43(i1,i2,i3,kd)=rz(i1,i2,i3)**2*vrarr4(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*vrass4(i1,i2,i3,kd)+tz(i1,i2,i3)**2*vratt4(i1,i2,i3,
     & kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*vrars4(i1,i2,i3,kd)+2.*rz(i1,
     & i2,i3)*tz(i1,i2,i3)*vrart4(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,
     & i2,i3)*vrast4(i1,i2,i3,kd)+rzz43(i1,i2,i3)*vrar4(i1,i2,i3,kd)+
     & szz43(i1,i2,i3)*vras4(i1,i2,i3,kd)+tzz43(i1,i2,i3)*vrat4(i1,i2,
     & i3,kd)
      vraxy43(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vrarr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vrass4(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & ty(i1,i2,i3)*vratt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(
     & i1,i2,i3)*sx(i1,i2,i3))*vrars4(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(
     & i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*vrart4(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*vrast4(i1,i2,
     & i3,kd)+rxy43(i1,i2,i3)*vrar4(i1,i2,i3,kd)+sxy43(i1,i2,i3)*
     & vras4(i1,i2,i3,kd)+txy43(i1,i2,i3)*vrat4(i1,i2,i3,kd)
      vraxz43(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*vrarr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*vrass4(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & tz(i1,i2,i3)*vratt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sx(i1,i2,i3))*vrars4(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*vrart4(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*vrast4(i1,i2,
     & i3,kd)+rxz43(i1,i2,i3)*vrar4(i1,i2,i3,kd)+sxz43(i1,i2,i3)*
     & vras4(i1,i2,i3,kd)+txz43(i1,i2,i3)*vrat4(i1,i2,i3,kd)
      vrayz43(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*vrarr4(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*vrass4(i1,i2,i3,kd)+ty(i1,i2,i3)*
     & tz(i1,i2,i3)*vratt4(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sy(i1,i2,i3))*vrars4(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*vrart4(i1,i2,i3,kd)+(sy(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*vrast4(i1,i2,
     & i3,kd)+ryz43(i1,i2,i3)*vrar4(i1,i2,i3,kd)+syz43(i1,i2,i3)*
     & vras4(i1,i2,i3,kd)+tyz43(i1,i2,i3)*vrat4(i1,i2,i3,kd)
      vralaplacian43(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*vrarr4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)
     & **2+sz(i1,i2,i3)**2)*vrass4(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(
     & i1,i2,i3)**2+tz(i1,i2,i3)**2)*vratt4(i1,i2,i3,kd)+2.*(rx(i1,i2,
     & i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,
     & i2,i3))*vrars4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(
     & i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*vrart4(i1,i2,
     & i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)
     & +sz(i1,i2,i3)*tz(i1,i2,i3))*vrast4(i1,i2,i3,kd)+(rxx43(i1,i2,
     & i3)+ryy43(i1,i2,i3)+rzz43(i1,i2,i3))*vrar4(i1,i2,i3,kd)+(sxx43(
     & i1,i2,i3)+syy43(i1,i2,i3)+szz43(i1,i2,i3))*vras4(i1,i2,i3,kd)+(
     & txx43(i1,i2,i3)+tyy43(i1,i2,i3)+tzz43(i1,i2,i3))*vrat4(i1,i2,
     & i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      vrax43r(i1,i2,i3,kd)=(8.*(vra(i1+1,i2,i3,kd)-vra(i1-1,i2,i3,kd))-
     & (vra(i1+2,i2,i3,kd)-vra(i1-2,i2,i3,kd)))*h41(0)
      vray43r(i1,i2,i3,kd)=(8.*(vra(i1,i2+1,i3,kd)-vra(i1,i2-1,i3,kd))-
     & (vra(i1,i2+2,i3,kd)-vra(i1,i2-2,i3,kd)))*h41(1)
      vraz43r(i1,i2,i3,kd)=(8.*(vra(i1,i2,i3+1,kd)-vra(i1,i2,i3-1,kd))-
     & (vra(i1,i2,i3+2,kd)-vra(i1,i2,i3-2,kd)))*h41(2)
      vraxx43r(i1,i2,i3,kd)=( -30.*vra(i1,i2,i3,kd)+16.*(vra(i1+1,i2,
     & i3,kd)+vra(i1-1,i2,i3,kd))-(vra(i1+2,i2,i3,kd)+vra(i1-2,i2,i3,
     & kd)) )*h42(0)
      vrayy43r(i1,i2,i3,kd)=( -30.*vra(i1,i2,i3,kd)+16.*(vra(i1,i2+1,
     & i3,kd)+vra(i1,i2-1,i3,kd))-(vra(i1,i2+2,i3,kd)+vra(i1,i2-2,i3,
     & kd)) )*h42(1)
      vrazz43r(i1,i2,i3,kd)=( -30.*vra(i1,i2,i3,kd)+16.*(vra(i1,i2,i3+
     & 1,kd)+vra(i1,i2,i3-1,kd))-(vra(i1,i2,i3+2,kd)+vra(i1,i2,i3-2,
     & kd)) )*h42(2)
      vraxy43r(i1,i2,i3,kd)=( (vra(i1+2,i2+2,i3,kd)-vra(i1-2,i2+2,i3,
     & kd)- vra(i1+2,i2-2,i3,kd)+vra(i1-2,i2-2,i3,kd)) +8.*(vra(i1-1,
     & i2+2,i3,kd)-vra(i1-1,i2-2,i3,kd)-vra(i1+1,i2+2,i3,kd)+vra(i1+1,
     & i2-2,i3,kd) +vra(i1+2,i2-1,i3,kd)-vra(i1-2,i2-1,i3,kd)-vra(i1+
     & 2,i2+1,i3,kd)+vra(i1-2,i2+1,i3,kd))+64.*(vra(i1+1,i2+1,i3,kd)-
     & vra(i1-1,i2+1,i3,kd)- vra(i1+1,i2-1,i3,kd)+vra(i1-1,i2-1,i3,kd)
     & ))*(h41(0)*h41(1))
      vraxz43r(i1,i2,i3,kd)=( (vra(i1+2,i2,i3+2,kd)-vra(i1-2,i2,i3+2,
     & kd)-vra(i1+2,i2,i3-2,kd)+vra(i1-2,i2,i3-2,kd)) +8.*(vra(i1-1,
     & i2,i3+2,kd)-vra(i1-1,i2,i3-2,kd)-vra(i1+1,i2,i3+2,kd)+vra(i1+1,
     & i2,i3-2,kd) +vra(i1+2,i2,i3-1,kd)-vra(i1-2,i2,i3-1,kd)- vra(i1+
     & 2,i2,i3+1,kd)+vra(i1-2,i2,i3+1,kd)) +64.*(vra(i1+1,i2,i3+1,kd)-
     & vra(i1-1,i2,i3+1,kd)-vra(i1+1,i2,i3-1,kd)+vra(i1-1,i2,i3-1,kd))
     &  )*(h41(0)*h41(2))
      vrayz43r(i1,i2,i3,kd)=( (vra(i1,i2+2,i3+2,kd)-vra(i1,i2-2,i3+2,
     & kd)-vra(i1,i2+2,i3-2,kd)+vra(i1,i2-2,i3-2,kd)) +8.*(vra(i1,i2-
     & 1,i3+2,kd)-vra(i1,i2-1,i3-2,kd)-vra(i1,i2+1,i3+2,kd)+vra(i1,i2+
     & 1,i3-2,kd) +vra(i1,i2+2,i3-1,kd)-vra(i1,i2-2,i3-1,kd)-vra(i1,
     & i2+2,i3+1,kd)+vra(i1,i2-2,i3+1,kd)) +64.*(vra(i1,i2+1,i3+1,kd)-
     & vra(i1,i2-1,i3+1,kd)-vra(i1,i2+1,i3-1,kd)+vra(i1,i2-1,i3-1,kd))
     &  )*(h41(1)*h41(2))
      vrax41r(i1,i2,i3,kd)= vrax43r(i1,i2,i3,kd)
      vray41r(i1,i2,i3,kd)= vray43r(i1,i2,i3,kd)
      vraz41r(i1,i2,i3,kd)= vraz43r(i1,i2,i3,kd)
      vraxx41r(i1,i2,i3,kd)= vraxx43r(i1,i2,i3,kd)
      vrayy41r(i1,i2,i3,kd)= vrayy43r(i1,i2,i3,kd)
      vrazz41r(i1,i2,i3,kd)= vrazz43r(i1,i2,i3,kd)
      vraxy41r(i1,i2,i3,kd)= vraxy43r(i1,i2,i3,kd)
      vraxz41r(i1,i2,i3,kd)= vraxz43r(i1,i2,i3,kd)
      vrayz41r(i1,i2,i3,kd)= vrayz43r(i1,i2,i3,kd)
      vralaplacian41r(i1,i2,i3,kd)=vraxx43r(i1,i2,i3,kd)
      vrax42r(i1,i2,i3,kd)= vrax43r(i1,i2,i3,kd)
      vray42r(i1,i2,i3,kd)= vray43r(i1,i2,i3,kd)
      vraz42r(i1,i2,i3,kd)= vraz43r(i1,i2,i3,kd)
      vraxx42r(i1,i2,i3,kd)= vraxx43r(i1,i2,i3,kd)
      vrayy42r(i1,i2,i3,kd)= vrayy43r(i1,i2,i3,kd)
      vrazz42r(i1,i2,i3,kd)= vrazz43r(i1,i2,i3,kd)
      vraxy42r(i1,i2,i3,kd)= vraxy43r(i1,i2,i3,kd)
      vraxz42r(i1,i2,i3,kd)= vraxz43r(i1,i2,i3,kd)
      vrayz42r(i1,i2,i3,kd)= vrayz43r(i1,i2,i3,kd)
      vralaplacian42r(i1,i2,i3,kd)=vraxx43r(i1,i2,i3,kd)+vrayy43r(i1,
     & i2,i3,kd)
      vralaplacian43r(i1,i2,i3,kd)=vraxx43r(i1,i2,i3,kd)+vrayy43r(i1,
     & i2,i3,kd)+vrazz43r(i1,i2,i3,kd)
      vramr4(i1,i2,i3,kd)=(8.*(vram(i1+1,i2,i3,kd)-vram(i1-1,i2,i3,kd))
     & -(vram(i1+2,i2,i3,kd)-vram(i1-2,i2,i3,kd)))*d14(0)
      vrams4(i1,i2,i3,kd)=(8.*(vram(i1,i2+1,i3,kd)-vram(i1,i2-1,i3,kd))
     & -(vram(i1,i2+2,i3,kd)-vram(i1,i2-2,i3,kd)))*d14(1)
      vramt4(i1,i2,i3,kd)=(8.*(vram(i1,i2,i3+1,kd)-vram(i1,i2,i3-1,kd))
     & -(vram(i1,i2,i3+2,kd)-vram(i1,i2,i3-2,kd)))*d14(2)
      vramrr4(i1,i2,i3,kd)=(-30.*vram(i1,i2,i3,kd)+16.*(vram(i1+1,i2,
     & i3,kd)+vram(i1-1,i2,i3,kd))-(vram(i1+2,i2,i3,kd)+vram(i1-2,i2,
     & i3,kd)) )*d24(0)
      vramss4(i1,i2,i3,kd)=(-30.*vram(i1,i2,i3,kd)+16.*(vram(i1,i2+1,
     & i3,kd)+vram(i1,i2-1,i3,kd))-(vram(i1,i2+2,i3,kd)+vram(i1,i2-2,
     & i3,kd)) )*d24(1)
      vramtt4(i1,i2,i3,kd)=(-30.*vram(i1,i2,i3,kd)+16.*(vram(i1,i2,i3+
     & 1,kd)+vram(i1,i2,i3-1,kd))-(vram(i1,i2,i3+2,kd)+vram(i1,i2,i3-
     & 2,kd)) )*d24(2)
      vramrs4(i1,i2,i3,kd)=(8.*(vramr4(i1,i2+1,i3,kd)-vramr4(i1,i2-1,
     & i3,kd))-(vramr4(i1,i2+2,i3,kd)-vramr4(i1,i2-2,i3,kd)))*d14(1)
      vramrt4(i1,i2,i3,kd)=(8.*(vramr4(i1,i2,i3+1,kd)-vramr4(i1,i2,i3-
     & 1,kd))-(vramr4(i1,i2,i3+2,kd)-vramr4(i1,i2,i3-2,kd)))*d14(2)
      vramst4(i1,i2,i3,kd)=(8.*(vrams4(i1,i2,i3+1,kd)-vrams4(i1,i2,i3-
     & 1,kd))-(vrams4(i1,i2,i3+2,kd)-vrams4(i1,i2,i3-2,kd)))*d14(2)
      vramx41(i1,i2,i3,kd)= rx(i1,i2,i3)*vramr4(i1,i2,i3,kd)
      vramy41(i1,i2,i3,kd)=0
      vramz41(i1,i2,i3,kd)=0
      vramx42(i1,i2,i3,kd)= rx(i1,i2,i3)*vramr4(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*vrams4(i1,i2,i3,kd)
      vramy42(i1,i2,i3,kd)= ry(i1,i2,i3)*vramr4(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*vrams4(i1,i2,i3,kd)
      vramz42(i1,i2,i3,kd)=0
      vramx43(i1,i2,i3,kd)=rx(i1,i2,i3)*vramr4(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*vrams4(i1,i2,i3,kd)+tx(i1,i2,i3)*vramt4(i1,i2,i3,kd)
      vramy43(i1,i2,i3,kd)=ry(i1,i2,i3)*vramr4(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*vrams4(i1,i2,i3,kd)+ty(i1,i2,i3)*vramt4(i1,i2,i3,kd)
      vramz43(i1,i2,i3,kd)=rz(i1,i2,i3)*vramr4(i1,i2,i3,kd)+sz(i1,i2,
     & i3)*vrams4(i1,i2,i3,kd)+tz(i1,i2,i3)*vramt4(i1,i2,i3,kd)
      vramxx41(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vramrr4(i1,i2,i3,kd)+(
     & rxx42(i1,i2,i3))*vramr4(i1,i2,i3,kd)
      vramyy41(i1,i2,i3,kd)=0
      vramxy41(i1,i2,i3,kd)=0
      vramxz41(i1,i2,i3,kd)=0
      vramyz41(i1,i2,i3,kd)=0
      vramzz41(i1,i2,i3,kd)=0
      vramlaplacian41(i1,i2,i3,kd)=vramxx41(i1,i2,i3,kd)
      vramxx42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vramrr4(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*vramrs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*vramss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3))*vramr4(i1,i2,i3,kd)+(
     & sxx42(i1,i2,i3))*vrams4(i1,i2,i3,kd)
      vramyy42(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*vramrr4(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*vramrs4(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*vramss4(i1,i2,i3,kd)+(ryy42(i1,i2,i3))*vramr4(i1,i2,i3,kd)+(
     & syy42(i1,i2,i3))*vrams4(i1,i2,i3,kd)
      vramxy42(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vramrr4(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & vramrs4(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vramss4(i1,i2,
     & i3,kd)+rxy42(i1,i2,i3)*vramr4(i1,i2,i3,kd)+sxy42(i1,i2,i3)*
     & vrams4(i1,i2,i3,kd)
      vramxz42(i1,i2,i3,kd)=0
      vramyz42(i1,i2,i3,kd)=0
      vramzz42(i1,i2,i3,kd)=0
      vramlaplacian42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & vramrr4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,
     & i3)*sy(i1,i2,i3))*vramrs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,
     & i2,i3)**2)*vramss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3)+ryy42(i1,i2,
     & i3))*vramr4(i1,i2,i3,kd)+(sxx42(i1,i2,i3)+syy42(i1,i2,i3))*
     & vrams4(i1,i2,i3,kd)
      vramxx43(i1,i2,i3,kd)=rx(i1,i2,i3)**2*vramrr4(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*vramss4(i1,i2,i3,kd)+tx(i1,i2,i3)**2*vramtt4(i1,i2,
     & i3,kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*vramrs4(i1,i2,i3,kd)+2.*rx(
     & i1,i2,i3)*tx(i1,i2,i3)*vramrt4(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(
     & i1,i2,i3)*vramst4(i1,i2,i3,kd)+rxx43(i1,i2,i3)*vramr4(i1,i2,i3,
     & kd)+sxx43(i1,i2,i3)*vrams4(i1,i2,i3,kd)+txx43(i1,i2,i3)*vramt4(
     & i1,i2,i3,kd)
      vramyy43(i1,i2,i3,kd)=ry(i1,i2,i3)**2*vramrr4(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*vramss4(i1,i2,i3,kd)+ty(i1,i2,i3)**2*vramtt4(i1,i2,
     & i3,kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*vramrs4(i1,i2,i3,kd)+2.*ry(
     & i1,i2,i3)*ty(i1,i2,i3)*vramrt4(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(
     & i1,i2,i3)*vramst4(i1,i2,i3,kd)+ryy43(i1,i2,i3)*vramr4(i1,i2,i3,
     & kd)+syy43(i1,i2,i3)*vrams4(i1,i2,i3,kd)+tyy43(i1,i2,i3)*vramt4(
     & i1,i2,i3,kd)
      vramzz43(i1,i2,i3,kd)=rz(i1,i2,i3)**2*vramrr4(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*vramss4(i1,i2,i3,kd)+tz(i1,i2,i3)**2*vramtt4(i1,i2,
     & i3,kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*vramrs4(i1,i2,i3,kd)+2.*rz(
     & i1,i2,i3)*tz(i1,i2,i3)*vramrt4(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(
     & i1,i2,i3)*vramst4(i1,i2,i3,kd)+rzz43(i1,i2,i3)*vramr4(i1,i2,i3,
     & kd)+szz43(i1,i2,i3)*vrams4(i1,i2,i3,kd)+tzz43(i1,i2,i3)*vramt4(
     & i1,i2,i3,kd)
      vramxy43(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vramrr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vramss4(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *ty(i1,i2,i3)*vramtt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+
     & ry(i1,i2,i3)*sx(i1,i2,i3))*vramrs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & ty(i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*vramrt4(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*vramst4(
     & i1,i2,i3,kd)+rxy43(i1,i2,i3)*vramr4(i1,i2,i3,kd)+sxy43(i1,i2,
     & i3)*vrams4(i1,i2,i3,kd)+txy43(i1,i2,i3)*vramt4(i1,i2,i3,kd)
      vramxz43(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*vramrr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*vramss4(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *tz(i1,i2,i3)*vramtt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sx(i1,i2,i3))*vramrs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*vramrt4(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*vramst4(
     & i1,i2,i3,kd)+rxz43(i1,i2,i3)*vramr4(i1,i2,i3,kd)+sxz43(i1,i2,
     & i3)*vrams4(i1,i2,i3,kd)+txz43(i1,i2,i3)*vramt4(i1,i2,i3,kd)
      vramyz43(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*vramrr4(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*vramss4(i1,i2,i3,kd)+ty(i1,i2,i3)
     & *tz(i1,i2,i3)*vramtt4(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sy(i1,i2,i3))*vramrs4(i1,i2,i3,kd)+(ry(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*vramrt4(i1,i2,i3,kd)+(
     & sy(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*vramst4(
     & i1,i2,i3,kd)+ryz43(i1,i2,i3)*vramr4(i1,i2,i3,kd)+syz43(i1,i2,
     & i3)*vrams4(i1,i2,i3,kd)+tyz43(i1,i2,i3)*vramt4(i1,i2,i3,kd)
      vramlaplacian43(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*vramrr4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2+sz(i1,i2,i3)**2)*vramss4(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+
     & ty(i1,i2,i3)**2+tz(i1,i2,i3)**2)*vramtt4(i1,i2,i3,kd)+2.*(rx(
     & i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*
     & sz(i1,i2,i3))*vramrs4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,
     & i3)+ ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*
     & vramrt4(i1,i2,i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,
     & i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3))*vramst4(i1,i2,i3,
     & kd)+(rxx43(i1,i2,i3)+ryy43(i1,i2,i3)+rzz43(i1,i2,i3))*vramr4(
     & i1,i2,i3,kd)+(sxx43(i1,i2,i3)+syy43(i1,i2,i3)+szz43(i1,i2,i3))*
     & vrams4(i1,i2,i3,kd)+(txx43(i1,i2,i3)+tyy43(i1,i2,i3)+tzz43(i1,
     & i2,i3))*vramt4(i1,i2,i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      vramx43r(i1,i2,i3,kd)=(8.*(vram(i1+1,i2,i3,kd)-vram(i1-1,i2,i3,
     & kd))-(vram(i1+2,i2,i3,kd)-vram(i1-2,i2,i3,kd)))*h41(0)
      vramy43r(i1,i2,i3,kd)=(8.*(vram(i1,i2+1,i3,kd)-vram(i1,i2-1,i3,
     & kd))-(vram(i1,i2+2,i3,kd)-vram(i1,i2-2,i3,kd)))*h41(1)
      vramz43r(i1,i2,i3,kd)=(8.*(vram(i1,i2,i3+1,kd)-vram(i1,i2,i3-1,
     & kd))-(vram(i1,i2,i3+2,kd)-vram(i1,i2,i3-2,kd)))*h41(2)
      vramxx43r(i1,i2,i3,kd)=( -30.*vram(i1,i2,i3,kd)+16.*(vram(i1+1,
     & i2,i3,kd)+vram(i1-1,i2,i3,kd))-(vram(i1+2,i2,i3,kd)+vram(i1-2,
     & i2,i3,kd)) )*h42(0)
      vramyy43r(i1,i2,i3,kd)=( -30.*vram(i1,i2,i3,kd)+16.*(vram(i1,i2+
     & 1,i3,kd)+vram(i1,i2-1,i3,kd))-(vram(i1,i2+2,i3,kd)+vram(i1,i2-
     & 2,i3,kd)) )*h42(1)
      vramzz43r(i1,i2,i3,kd)=( -30.*vram(i1,i2,i3,kd)+16.*(vram(i1,i2,
     & i3+1,kd)+vram(i1,i2,i3-1,kd))-(vram(i1,i2,i3+2,kd)+vram(i1,i2,
     & i3-2,kd)) )*h42(2)
      vramxy43r(i1,i2,i3,kd)=( (vram(i1+2,i2+2,i3,kd)-vram(i1-2,i2+2,
     & i3,kd)- vram(i1+2,i2-2,i3,kd)+vram(i1-2,i2-2,i3,kd)) +8.*(vram(
     & i1-1,i2+2,i3,kd)-vram(i1-1,i2-2,i3,kd)-vram(i1+1,i2+2,i3,kd)+
     & vram(i1+1,i2-2,i3,kd) +vram(i1+2,i2-1,i3,kd)-vram(i1-2,i2-1,i3,
     & kd)-vram(i1+2,i2+1,i3,kd)+vram(i1-2,i2+1,i3,kd))+64.*(vram(i1+
     & 1,i2+1,i3,kd)-vram(i1-1,i2+1,i3,kd)- vram(i1+1,i2-1,i3,kd)+
     & vram(i1-1,i2-1,i3,kd)))*(h41(0)*h41(1))
      vramxz43r(i1,i2,i3,kd)=( (vram(i1+2,i2,i3+2,kd)-vram(i1-2,i2,i3+
     & 2,kd)-vram(i1+2,i2,i3-2,kd)+vram(i1-2,i2,i3-2,kd)) +8.*(vram(
     & i1-1,i2,i3+2,kd)-vram(i1-1,i2,i3-2,kd)-vram(i1+1,i2,i3+2,kd)+
     & vram(i1+1,i2,i3-2,kd) +vram(i1+2,i2,i3-1,kd)-vram(i1-2,i2,i3-1,
     & kd)- vram(i1+2,i2,i3+1,kd)+vram(i1-2,i2,i3+1,kd)) +64.*(vram(
     & i1+1,i2,i3+1,kd)-vram(i1-1,i2,i3+1,kd)-vram(i1+1,i2,i3-1,kd)+
     & vram(i1-1,i2,i3-1,kd)) )*(h41(0)*h41(2))
      vramyz43r(i1,i2,i3,kd)=( (vram(i1,i2+2,i3+2,kd)-vram(i1,i2-2,i3+
     & 2,kd)-vram(i1,i2+2,i3-2,kd)+vram(i1,i2-2,i3-2,kd)) +8.*(vram(
     & i1,i2-1,i3+2,kd)-vram(i1,i2-1,i3-2,kd)-vram(i1,i2+1,i3+2,kd)+
     & vram(i1,i2+1,i3-2,kd) +vram(i1,i2+2,i3-1,kd)-vram(i1,i2-2,i3-1,
     & kd)-vram(i1,i2+2,i3+1,kd)+vram(i1,i2-2,i3+1,kd)) +64.*(vram(i1,
     & i2+1,i3+1,kd)-vram(i1,i2-1,i3+1,kd)-vram(i1,i2+1,i3-1,kd)+vram(
     & i1,i2-1,i3-1,kd)) )*(h41(1)*h41(2))
      vramx41r(i1,i2,i3,kd)= vramx43r(i1,i2,i3,kd)
      vramy41r(i1,i2,i3,kd)= vramy43r(i1,i2,i3,kd)
      vramz41r(i1,i2,i3,kd)= vramz43r(i1,i2,i3,kd)
      vramxx41r(i1,i2,i3,kd)= vramxx43r(i1,i2,i3,kd)
      vramyy41r(i1,i2,i3,kd)= vramyy43r(i1,i2,i3,kd)
      vramzz41r(i1,i2,i3,kd)= vramzz43r(i1,i2,i3,kd)
      vramxy41r(i1,i2,i3,kd)= vramxy43r(i1,i2,i3,kd)
      vramxz41r(i1,i2,i3,kd)= vramxz43r(i1,i2,i3,kd)
      vramyz41r(i1,i2,i3,kd)= vramyz43r(i1,i2,i3,kd)
      vramlaplacian41r(i1,i2,i3,kd)=vramxx43r(i1,i2,i3,kd)
      vramx42r(i1,i2,i3,kd)= vramx43r(i1,i2,i3,kd)
      vramy42r(i1,i2,i3,kd)= vramy43r(i1,i2,i3,kd)
      vramz42r(i1,i2,i3,kd)= vramz43r(i1,i2,i3,kd)
      vramxx42r(i1,i2,i3,kd)= vramxx43r(i1,i2,i3,kd)
      vramyy42r(i1,i2,i3,kd)= vramyy43r(i1,i2,i3,kd)
      vramzz42r(i1,i2,i3,kd)= vramzz43r(i1,i2,i3,kd)
      vramxy42r(i1,i2,i3,kd)= vramxy43r(i1,i2,i3,kd)
      vramxz42r(i1,i2,i3,kd)= vramxz43r(i1,i2,i3,kd)
      vramyz42r(i1,i2,i3,kd)= vramyz43r(i1,i2,i3,kd)
      vramlaplacian42r(i1,i2,i3,kd)=vramxx43r(i1,i2,i3,kd)+vramyy43r(
     & i1,i2,i3,kd)
      vramlaplacian43r(i1,i2,i3,kd)=vramxx43r(i1,i2,i3,kd)+vramyy43r(
     & i1,i2,i3,kd)+vramzz43r(i1,i2,i3,kd)
      wrar4(i1,i2,i3,kd)=(8.*(wra(i1+1,i2,i3,kd)-wra(i1-1,i2,i3,kd))-(
     & wra(i1+2,i2,i3,kd)-wra(i1-2,i2,i3,kd)))*d14(0)
      wras4(i1,i2,i3,kd)=(8.*(wra(i1,i2+1,i3,kd)-wra(i1,i2-1,i3,kd))-(
     & wra(i1,i2+2,i3,kd)-wra(i1,i2-2,i3,kd)))*d14(1)
      wrat4(i1,i2,i3,kd)=(8.*(wra(i1,i2,i3+1,kd)-wra(i1,i2,i3-1,kd))-(
     & wra(i1,i2,i3+2,kd)-wra(i1,i2,i3-2,kd)))*d14(2)
      wrarr4(i1,i2,i3,kd)=(-30.*wra(i1,i2,i3,kd)+16.*(wra(i1+1,i2,i3,
     & kd)+wra(i1-1,i2,i3,kd))-(wra(i1+2,i2,i3,kd)+wra(i1-2,i2,i3,kd))
     &  )*d24(0)
      wrass4(i1,i2,i3,kd)=(-30.*wra(i1,i2,i3,kd)+16.*(wra(i1,i2+1,i3,
     & kd)+wra(i1,i2-1,i3,kd))-(wra(i1,i2+2,i3,kd)+wra(i1,i2-2,i3,kd))
     &  )*d24(1)
      wratt4(i1,i2,i3,kd)=(-30.*wra(i1,i2,i3,kd)+16.*(wra(i1,i2,i3+1,
     & kd)+wra(i1,i2,i3-1,kd))-(wra(i1,i2,i3+2,kd)+wra(i1,i2,i3-2,kd))
     &  )*d24(2)
      wrars4(i1,i2,i3,kd)=(8.*(wrar4(i1,i2+1,i3,kd)-wrar4(i1,i2-1,i3,
     & kd))-(wrar4(i1,i2+2,i3,kd)-wrar4(i1,i2-2,i3,kd)))*d14(1)
      wrart4(i1,i2,i3,kd)=(8.*(wrar4(i1,i2,i3+1,kd)-wrar4(i1,i2,i3-1,
     & kd))-(wrar4(i1,i2,i3+2,kd)-wrar4(i1,i2,i3-2,kd)))*d14(2)
      wrast4(i1,i2,i3,kd)=(8.*(wras4(i1,i2,i3+1,kd)-wras4(i1,i2,i3-1,
     & kd))-(wras4(i1,i2,i3+2,kd)-wras4(i1,i2,i3-2,kd)))*d14(2)
      wrax41(i1,i2,i3,kd)= rx(i1,i2,i3)*wrar4(i1,i2,i3,kd)
      wray41(i1,i2,i3,kd)=0
      wraz41(i1,i2,i3,kd)=0
      wrax42(i1,i2,i3,kd)= rx(i1,i2,i3)*wrar4(i1,i2,i3,kd)+sx(i1,i2,i3)
     & *wras4(i1,i2,i3,kd)
      wray42(i1,i2,i3,kd)= ry(i1,i2,i3)*wrar4(i1,i2,i3,kd)+sy(i1,i2,i3)
     & *wras4(i1,i2,i3,kd)
      wraz42(i1,i2,i3,kd)=0
      wrax43(i1,i2,i3,kd)=rx(i1,i2,i3)*wrar4(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & wras4(i1,i2,i3,kd)+tx(i1,i2,i3)*wrat4(i1,i2,i3,kd)
      wray43(i1,i2,i3,kd)=ry(i1,i2,i3)*wrar4(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & wras4(i1,i2,i3,kd)+ty(i1,i2,i3)*wrat4(i1,i2,i3,kd)
      wraz43(i1,i2,i3,kd)=rz(i1,i2,i3)*wrar4(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & wras4(i1,i2,i3,kd)+tz(i1,i2,i3)*wrat4(i1,i2,i3,kd)
      wraxx41(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wrarr4(i1,i2,i3,kd)+(
     & rxx42(i1,i2,i3))*wrar4(i1,i2,i3,kd)
      wrayy41(i1,i2,i3,kd)=0
      wraxy41(i1,i2,i3,kd)=0
      wraxz41(i1,i2,i3,kd)=0
      wrayz41(i1,i2,i3,kd)=0
      wrazz41(i1,i2,i3,kd)=0
      wralaplacian41(i1,i2,i3,kd)=wraxx41(i1,i2,i3,kd)
      wraxx42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wrarr4(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*wrars4(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*wrass4(i1,i2,i3,kd)+(rxx42(i1,i2,i3))*wrar4(i1,i2,i3,kd)+(
     & sxx42(i1,i2,i3))*wras4(i1,i2,i3,kd)
      wrayy42(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*wrarr4(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*wrars4(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*wrass4(i1,i2,i3,kd)+(ryy42(i1,i2,i3))*wrar4(i1,i2,i3,kd)+(
     & syy42(i1,i2,i3))*wras4(i1,i2,i3,kd)
      wraxy42(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wrarr4(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & wrars4(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wrass4(i1,i2,i3,
     & kd)+rxy42(i1,i2,i3)*wrar4(i1,i2,i3,kd)+sxy42(i1,i2,i3)*wras4(
     & i1,i2,i3,kd)
      wraxz42(i1,i2,i3,kd)=0
      wrayz42(i1,i2,i3,kd)=0
      wrazz42(i1,i2,i3,kd)=0
      wralaplacian42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & wrarr4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)
     & *sy(i1,i2,i3))*wrars4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2)*wrass4(i1,i2,i3,kd)+(rxx42(i1,i2,i3)+ryy42(i1,i2,i3))*
     & wrar4(i1,i2,i3,kd)+(sxx42(i1,i2,i3)+syy42(i1,i2,i3))*wras4(i1,
     & i2,i3,kd)
      wraxx43(i1,i2,i3,kd)=rx(i1,i2,i3)**2*wrarr4(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*wrass4(i1,i2,i3,kd)+tx(i1,i2,i3)**2*wratt4(i1,i2,i3,
     & kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*wrars4(i1,i2,i3,kd)+2.*rx(i1,
     & i2,i3)*tx(i1,i2,i3)*wrart4(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,
     & i2,i3)*wrast4(i1,i2,i3,kd)+rxx43(i1,i2,i3)*wrar4(i1,i2,i3,kd)+
     & sxx43(i1,i2,i3)*wras4(i1,i2,i3,kd)+txx43(i1,i2,i3)*wrat4(i1,i2,
     & i3,kd)
      wrayy43(i1,i2,i3,kd)=ry(i1,i2,i3)**2*wrarr4(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*wrass4(i1,i2,i3,kd)+ty(i1,i2,i3)**2*wratt4(i1,i2,i3,
     & kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*wrars4(i1,i2,i3,kd)+2.*ry(i1,
     & i2,i3)*ty(i1,i2,i3)*wrart4(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,
     & i2,i3)*wrast4(i1,i2,i3,kd)+ryy43(i1,i2,i3)*wrar4(i1,i2,i3,kd)+
     & syy43(i1,i2,i3)*wras4(i1,i2,i3,kd)+tyy43(i1,i2,i3)*wrat4(i1,i2,
     & i3,kd)
      wrazz43(i1,i2,i3,kd)=rz(i1,i2,i3)**2*wrarr4(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*wrass4(i1,i2,i3,kd)+tz(i1,i2,i3)**2*wratt4(i1,i2,i3,
     & kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*wrars4(i1,i2,i3,kd)+2.*rz(i1,
     & i2,i3)*tz(i1,i2,i3)*wrart4(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,
     & i2,i3)*wrast4(i1,i2,i3,kd)+rzz43(i1,i2,i3)*wrar4(i1,i2,i3,kd)+
     & szz43(i1,i2,i3)*wras4(i1,i2,i3,kd)+tzz43(i1,i2,i3)*wrat4(i1,i2,
     & i3,kd)
      wraxy43(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wrarr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wrass4(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & ty(i1,i2,i3)*wratt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(
     & i1,i2,i3)*sx(i1,i2,i3))*wrars4(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(
     & i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*wrart4(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*wrast4(i1,i2,
     & i3,kd)+rxy43(i1,i2,i3)*wrar4(i1,i2,i3,kd)+sxy43(i1,i2,i3)*
     & wras4(i1,i2,i3,kd)+txy43(i1,i2,i3)*wrat4(i1,i2,i3,kd)
      wraxz43(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*wrarr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*wrass4(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & tz(i1,i2,i3)*wratt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sx(i1,i2,i3))*wrars4(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*wrart4(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*wrast4(i1,i2,
     & i3,kd)+rxz43(i1,i2,i3)*wrar4(i1,i2,i3,kd)+sxz43(i1,i2,i3)*
     & wras4(i1,i2,i3,kd)+txz43(i1,i2,i3)*wrat4(i1,i2,i3,kd)
      wrayz43(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*wrarr4(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*wrass4(i1,i2,i3,kd)+ty(i1,i2,i3)*
     & tz(i1,i2,i3)*wratt4(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sy(i1,i2,i3))*wrars4(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*wrart4(i1,i2,i3,kd)+(sy(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*wrast4(i1,i2,
     & i3,kd)+ryz43(i1,i2,i3)*wrar4(i1,i2,i3,kd)+syz43(i1,i2,i3)*
     & wras4(i1,i2,i3,kd)+tyz43(i1,i2,i3)*wrat4(i1,i2,i3,kd)
      wralaplacian43(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*wrarr4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)
     & **2+sz(i1,i2,i3)**2)*wrass4(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(
     & i1,i2,i3)**2+tz(i1,i2,i3)**2)*wratt4(i1,i2,i3,kd)+2.*(rx(i1,i2,
     & i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,
     & i2,i3))*wrars4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(
     & i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*wrart4(i1,i2,
     & i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)
     & +sz(i1,i2,i3)*tz(i1,i2,i3))*wrast4(i1,i2,i3,kd)+(rxx43(i1,i2,
     & i3)+ryy43(i1,i2,i3)+rzz43(i1,i2,i3))*wrar4(i1,i2,i3,kd)+(sxx43(
     & i1,i2,i3)+syy43(i1,i2,i3)+szz43(i1,i2,i3))*wras4(i1,i2,i3,kd)+(
     & txx43(i1,i2,i3)+tyy43(i1,i2,i3)+tzz43(i1,i2,i3))*wrat4(i1,i2,
     & i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      wrax43r(i1,i2,i3,kd)=(8.*(wra(i1+1,i2,i3,kd)-wra(i1-1,i2,i3,kd))-
     & (wra(i1+2,i2,i3,kd)-wra(i1-2,i2,i3,kd)))*h41(0)
      wray43r(i1,i2,i3,kd)=(8.*(wra(i1,i2+1,i3,kd)-wra(i1,i2-1,i3,kd))-
     & (wra(i1,i2+2,i3,kd)-wra(i1,i2-2,i3,kd)))*h41(1)
      wraz43r(i1,i2,i3,kd)=(8.*(wra(i1,i2,i3+1,kd)-wra(i1,i2,i3-1,kd))-
     & (wra(i1,i2,i3+2,kd)-wra(i1,i2,i3-2,kd)))*h41(2)
      wraxx43r(i1,i2,i3,kd)=( -30.*wra(i1,i2,i3,kd)+16.*(wra(i1+1,i2,
     & i3,kd)+wra(i1-1,i2,i3,kd))-(wra(i1+2,i2,i3,kd)+wra(i1-2,i2,i3,
     & kd)) )*h42(0)
      wrayy43r(i1,i2,i3,kd)=( -30.*wra(i1,i2,i3,kd)+16.*(wra(i1,i2+1,
     & i3,kd)+wra(i1,i2-1,i3,kd))-(wra(i1,i2+2,i3,kd)+wra(i1,i2-2,i3,
     & kd)) )*h42(1)
      wrazz43r(i1,i2,i3,kd)=( -30.*wra(i1,i2,i3,kd)+16.*(wra(i1,i2,i3+
     & 1,kd)+wra(i1,i2,i3-1,kd))-(wra(i1,i2,i3+2,kd)+wra(i1,i2,i3-2,
     & kd)) )*h42(2)
      wraxy43r(i1,i2,i3,kd)=( (wra(i1+2,i2+2,i3,kd)-wra(i1-2,i2+2,i3,
     & kd)- wra(i1+2,i2-2,i3,kd)+wra(i1-2,i2-2,i3,kd)) +8.*(wra(i1-1,
     & i2+2,i3,kd)-wra(i1-1,i2-2,i3,kd)-wra(i1+1,i2+2,i3,kd)+wra(i1+1,
     & i2-2,i3,kd) +wra(i1+2,i2-1,i3,kd)-wra(i1-2,i2-1,i3,kd)-wra(i1+
     & 2,i2+1,i3,kd)+wra(i1-2,i2+1,i3,kd))+64.*(wra(i1+1,i2+1,i3,kd)-
     & wra(i1-1,i2+1,i3,kd)- wra(i1+1,i2-1,i3,kd)+wra(i1-1,i2-1,i3,kd)
     & ))*(h41(0)*h41(1))
      wraxz43r(i1,i2,i3,kd)=( (wra(i1+2,i2,i3+2,kd)-wra(i1-2,i2,i3+2,
     & kd)-wra(i1+2,i2,i3-2,kd)+wra(i1-2,i2,i3-2,kd)) +8.*(wra(i1-1,
     & i2,i3+2,kd)-wra(i1-1,i2,i3-2,kd)-wra(i1+1,i2,i3+2,kd)+wra(i1+1,
     & i2,i3-2,kd) +wra(i1+2,i2,i3-1,kd)-wra(i1-2,i2,i3-1,kd)- wra(i1+
     & 2,i2,i3+1,kd)+wra(i1-2,i2,i3+1,kd)) +64.*(wra(i1+1,i2,i3+1,kd)-
     & wra(i1-1,i2,i3+1,kd)-wra(i1+1,i2,i3-1,kd)+wra(i1-1,i2,i3-1,kd))
     &  )*(h41(0)*h41(2))
      wrayz43r(i1,i2,i3,kd)=( (wra(i1,i2+2,i3+2,kd)-wra(i1,i2-2,i3+2,
     & kd)-wra(i1,i2+2,i3-2,kd)+wra(i1,i2-2,i3-2,kd)) +8.*(wra(i1,i2-
     & 1,i3+2,kd)-wra(i1,i2-1,i3-2,kd)-wra(i1,i2+1,i3+2,kd)+wra(i1,i2+
     & 1,i3-2,kd) +wra(i1,i2+2,i3-1,kd)-wra(i1,i2-2,i3-1,kd)-wra(i1,
     & i2+2,i3+1,kd)+wra(i1,i2-2,i3+1,kd)) +64.*(wra(i1,i2+1,i3+1,kd)-
     & wra(i1,i2-1,i3+1,kd)-wra(i1,i2+1,i3-1,kd)+wra(i1,i2-1,i3-1,kd))
     &  )*(h41(1)*h41(2))
      wrax41r(i1,i2,i3,kd)= wrax43r(i1,i2,i3,kd)
      wray41r(i1,i2,i3,kd)= wray43r(i1,i2,i3,kd)
      wraz41r(i1,i2,i3,kd)= wraz43r(i1,i2,i3,kd)
      wraxx41r(i1,i2,i3,kd)= wraxx43r(i1,i2,i3,kd)
      wrayy41r(i1,i2,i3,kd)= wrayy43r(i1,i2,i3,kd)
      wrazz41r(i1,i2,i3,kd)= wrazz43r(i1,i2,i3,kd)
      wraxy41r(i1,i2,i3,kd)= wraxy43r(i1,i2,i3,kd)
      wraxz41r(i1,i2,i3,kd)= wraxz43r(i1,i2,i3,kd)
      wrayz41r(i1,i2,i3,kd)= wrayz43r(i1,i2,i3,kd)
      wralaplacian41r(i1,i2,i3,kd)=wraxx43r(i1,i2,i3,kd)
      wrax42r(i1,i2,i3,kd)= wrax43r(i1,i2,i3,kd)
      wray42r(i1,i2,i3,kd)= wray43r(i1,i2,i3,kd)
      wraz42r(i1,i2,i3,kd)= wraz43r(i1,i2,i3,kd)
      wraxx42r(i1,i2,i3,kd)= wraxx43r(i1,i2,i3,kd)
      wrayy42r(i1,i2,i3,kd)= wrayy43r(i1,i2,i3,kd)
      wrazz42r(i1,i2,i3,kd)= wrazz43r(i1,i2,i3,kd)
      wraxy42r(i1,i2,i3,kd)= wraxy43r(i1,i2,i3,kd)
      wraxz42r(i1,i2,i3,kd)= wraxz43r(i1,i2,i3,kd)
      wrayz42r(i1,i2,i3,kd)= wrayz43r(i1,i2,i3,kd)
      wralaplacian42r(i1,i2,i3,kd)=wraxx43r(i1,i2,i3,kd)+wrayy43r(i1,
     & i2,i3,kd)
      wralaplacian43r(i1,i2,i3,kd)=wraxx43r(i1,i2,i3,kd)+wrayy43r(i1,
     & i2,i3,kd)+wrazz43r(i1,i2,i3,kd)
      wramr4(i1,i2,i3,kd)=(8.*(wram(i1+1,i2,i3,kd)-wram(i1-1,i2,i3,kd))
     & -(wram(i1+2,i2,i3,kd)-wram(i1-2,i2,i3,kd)))*d14(0)
      wrams4(i1,i2,i3,kd)=(8.*(wram(i1,i2+1,i3,kd)-wram(i1,i2-1,i3,kd))
     & -(wram(i1,i2+2,i3,kd)-wram(i1,i2-2,i3,kd)))*d14(1)
      wramt4(i1,i2,i3,kd)=(8.*(wram(i1,i2,i3+1,kd)-wram(i1,i2,i3-1,kd))
     & -(wram(i1,i2,i3+2,kd)-wram(i1,i2,i3-2,kd)))*d14(2)
      wramrr4(i1,i2,i3,kd)=(-30.*wram(i1,i2,i3,kd)+16.*(wram(i1+1,i2,
     & i3,kd)+wram(i1-1,i2,i3,kd))-(wram(i1+2,i2,i3,kd)+wram(i1-2,i2,
     & i3,kd)) )*d24(0)
      wramss4(i1,i2,i3,kd)=(-30.*wram(i1,i2,i3,kd)+16.*(wram(i1,i2+1,
     & i3,kd)+wram(i1,i2-1,i3,kd))-(wram(i1,i2+2,i3,kd)+wram(i1,i2-2,
     & i3,kd)) )*d24(1)
      wramtt4(i1,i2,i3,kd)=(-30.*wram(i1,i2,i3,kd)+16.*(wram(i1,i2,i3+
     & 1,kd)+wram(i1,i2,i3-1,kd))-(wram(i1,i2,i3+2,kd)+wram(i1,i2,i3-
     & 2,kd)) )*d24(2)
      wramrs4(i1,i2,i3,kd)=(8.*(wramr4(i1,i2+1,i3,kd)-wramr4(i1,i2-1,
     & i3,kd))-(wramr4(i1,i2+2,i3,kd)-wramr4(i1,i2-2,i3,kd)))*d14(1)
      wramrt4(i1,i2,i3,kd)=(8.*(wramr4(i1,i2,i3+1,kd)-wramr4(i1,i2,i3-
     & 1,kd))-(wramr4(i1,i2,i3+2,kd)-wramr4(i1,i2,i3-2,kd)))*d14(2)
      wramst4(i1,i2,i3,kd)=(8.*(wrams4(i1,i2,i3+1,kd)-wrams4(i1,i2,i3-
     & 1,kd))-(wrams4(i1,i2,i3+2,kd)-wrams4(i1,i2,i3-2,kd)))*d14(2)
      wramx41(i1,i2,i3,kd)= rx(i1,i2,i3)*wramr4(i1,i2,i3,kd)
      wramy41(i1,i2,i3,kd)=0
      wramz41(i1,i2,i3,kd)=0
      wramx42(i1,i2,i3,kd)= rx(i1,i2,i3)*wramr4(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*wrams4(i1,i2,i3,kd)
      wramy42(i1,i2,i3,kd)= ry(i1,i2,i3)*wramr4(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*wrams4(i1,i2,i3,kd)
      wramz42(i1,i2,i3,kd)=0
      wramx43(i1,i2,i3,kd)=rx(i1,i2,i3)*wramr4(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*wrams4(i1,i2,i3,kd)+tx(i1,i2,i3)*wramt4(i1,i2,i3,kd)
      wramy43(i1,i2,i3,kd)=ry(i1,i2,i3)*wramr4(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*wrams4(i1,i2,i3,kd)+ty(i1,i2,i3)*wramt4(i1,i2,i3,kd)
      wramz43(i1,i2,i3,kd)=rz(i1,i2,i3)*wramr4(i1,i2,i3,kd)+sz(i1,i2,
     & i3)*wrams4(i1,i2,i3,kd)+tz(i1,i2,i3)*wramt4(i1,i2,i3,kd)
      wramxx41(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wramrr4(i1,i2,i3,kd)+(
     & rxx42(i1,i2,i3))*wramr4(i1,i2,i3,kd)
      wramyy41(i1,i2,i3,kd)=0
      wramxy41(i1,i2,i3,kd)=0
      wramxz41(i1,i2,i3,kd)=0
      wramyz41(i1,i2,i3,kd)=0
      wramzz41(i1,i2,i3,kd)=0
      wramlaplacian41(i1,i2,i3,kd)=wramxx41(i1,i2,i3,kd)
      wramxx42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wramrr4(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*wramrs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*wramss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3))*wramr4(i1,i2,i3,kd)+(
     & sxx42(i1,i2,i3))*wrams4(i1,i2,i3,kd)
      wramyy42(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*wramrr4(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*wramrs4(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*wramss4(i1,i2,i3,kd)+(ryy42(i1,i2,i3))*wramr4(i1,i2,i3,kd)+(
     & syy42(i1,i2,i3))*wrams4(i1,i2,i3,kd)
      wramxy42(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wramrr4(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & wramrs4(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wramss4(i1,i2,
     & i3,kd)+rxy42(i1,i2,i3)*wramr4(i1,i2,i3,kd)+sxy42(i1,i2,i3)*
     & wrams4(i1,i2,i3,kd)
      wramxz42(i1,i2,i3,kd)=0
      wramyz42(i1,i2,i3,kd)=0
      wramzz42(i1,i2,i3,kd)=0
      wramlaplacian42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & wramrr4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,
     & i3)*sy(i1,i2,i3))*wramrs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,
     & i2,i3)**2)*wramss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3)+ryy42(i1,i2,
     & i3))*wramr4(i1,i2,i3,kd)+(sxx42(i1,i2,i3)+syy42(i1,i2,i3))*
     & wrams4(i1,i2,i3,kd)
      wramxx43(i1,i2,i3,kd)=rx(i1,i2,i3)**2*wramrr4(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*wramss4(i1,i2,i3,kd)+tx(i1,i2,i3)**2*wramtt4(i1,i2,
     & i3,kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*wramrs4(i1,i2,i3,kd)+2.*rx(
     & i1,i2,i3)*tx(i1,i2,i3)*wramrt4(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(
     & i1,i2,i3)*wramst4(i1,i2,i3,kd)+rxx43(i1,i2,i3)*wramr4(i1,i2,i3,
     & kd)+sxx43(i1,i2,i3)*wrams4(i1,i2,i3,kd)+txx43(i1,i2,i3)*wramt4(
     & i1,i2,i3,kd)
      wramyy43(i1,i2,i3,kd)=ry(i1,i2,i3)**2*wramrr4(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*wramss4(i1,i2,i3,kd)+ty(i1,i2,i3)**2*wramtt4(i1,i2,
     & i3,kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*wramrs4(i1,i2,i3,kd)+2.*ry(
     & i1,i2,i3)*ty(i1,i2,i3)*wramrt4(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(
     & i1,i2,i3)*wramst4(i1,i2,i3,kd)+ryy43(i1,i2,i3)*wramr4(i1,i2,i3,
     & kd)+syy43(i1,i2,i3)*wrams4(i1,i2,i3,kd)+tyy43(i1,i2,i3)*wramt4(
     & i1,i2,i3,kd)
      wramzz43(i1,i2,i3,kd)=rz(i1,i2,i3)**2*wramrr4(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*wramss4(i1,i2,i3,kd)+tz(i1,i2,i3)**2*wramtt4(i1,i2,
     & i3,kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*wramrs4(i1,i2,i3,kd)+2.*rz(
     & i1,i2,i3)*tz(i1,i2,i3)*wramrt4(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(
     & i1,i2,i3)*wramst4(i1,i2,i3,kd)+rzz43(i1,i2,i3)*wramr4(i1,i2,i3,
     & kd)+szz43(i1,i2,i3)*wrams4(i1,i2,i3,kd)+tzz43(i1,i2,i3)*wramt4(
     & i1,i2,i3,kd)
      wramxy43(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wramrr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wramss4(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *ty(i1,i2,i3)*wramtt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+
     & ry(i1,i2,i3)*sx(i1,i2,i3))*wramrs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & ty(i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*wramrt4(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*wramst4(
     & i1,i2,i3,kd)+rxy43(i1,i2,i3)*wramr4(i1,i2,i3,kd)+sxy43(i1,i2,
     & i3)*wrams4(i1,i2,i3,kd)+txy43(i1,i2,i3)*wramt4(i1,i2,i3,kd)
      wramxz43(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*wramrr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*wramss4(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *tz(i1,i2,i3)*wramtt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sx(i1,i2,i3))*wramrs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*wramrt4(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*wramst4(
     & i1,i2,i3,kd)+rxz43(i1,i2,i3)*wramr4(i1,i2,i3,kd)+sxz43(i1,i2,
     & i3)*wrams4(i1,i2,i3,kd)+txz43(i1,i2,i3)*wramt4(i1,i2,i3,kd)
      wramyz43(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*wramrr4(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*wramss4(i1,i2,i3,kd)+ty(i1,i2,i3)
     & *tz(i1,i2,i3)*wramtt4(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sy(i1,i2,i3))*wramrs4(i1,i2,i3,kd)+(ry(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*wramrt4(i1,i2,i3,kd)+(
     & sy(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*wramst4(
     & i1,i2,i3,kd)+ryz43(i1,i2,i3)*wramr4(i1,i2,i3,kd)+syz43(i1,i2,
     & i3)*wrams4(i1,i2,i3,kd)+tyz43(i1,i2,i3)*wramt4(i1,i2,i3,kd)
      wramlaplacian43(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*wramrr4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2+sz(i1,i2,i3)**2)*wramss4(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+
     & ty(i1,i2,i3)**2+tz(i1,i2,i3)**2)*wramtt4(i1,i2,i3,kd)+2.*(rx(
     & i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*
     & sz(i1,i2,i3))*wramrs4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,
     & i3)+ ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*
     & wramrt4(i1,i2,i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,
     & i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3))*wramst4(i1,i2,i3,
     & kd)+(rxx43(i1,i2,i3)+ryy43(i1,i2,i3)+rzz43(i1,i2,i3))*wramr4(
     & i1,i2,i3,kd)+(sxx43(i1,i2,i3)+syy43(i1,i2,i3)+szz43(i1,i2,i3))*
     & wrams4(i1,i2,i3,kd)+(txx43(i1,i2,i3)+tyy43(i1,i2,i3)+tzz43(i1,
     & i2,i3))*wramt4(i1,i2,i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      wramx43r(i1,i2,i3,kd)=(8.*(wram(i1+1,i2,i3,kd)-wram(i1-1,i2,i3,
     & kd))-(wram(i1+2,i2,i3,kd)-wram(i1-2,i2,i3,kd)))*h41(0)
      wramy43r(i1,i2,i3,kd)=(8.*(wram(i1,i2+1,i3,kd)-wram(i1,i2-1,i3,
     & kd))-(wram(i1,i2+2,i3,kd)-wram(i1,i2-2,i3,kd)))*h41(1)
      wramz43r(i1,i2,i3,kd)=(8.*(wram(i1,i2,i3+1,kd)-wram(i1,i2,i3-1,
     & kd))-(wram(i1,i2,i3+2,kd)-wram(i1,i2,i3-2,kd)))*h41(2)
      wramxx43r(i1,i2,i3,kd)=( -30.*wram(i1,i2,i3,kd)+16.*(wram(i1+1,
     & i2,i3,kd)+wram(i1-1,i2,i3,kd))-(wram(i1+2,i2,i3,kd)+wram(i1-2,
     & i2,i3,kd)) )*h42(0)
      wramyy43r(i1,i2,i3,kd)=( -30.*wram(i1,i2,i3,kd)+16.*(wram(i1,i2+
     & 1,i3,kd)+wram(i1,i2-1,i3,kd))-(wram(i1,i2+2,i3,kd)+wram(i1,i2-
     & 2,i3,kd)) )*h42(1)
      wramzz43r(i1,i2,i3,kd)=( -30.*wram(i1,i2,i3,kd)+16.*(wram(i1,i2,
     & i3+1,kd)+wram(i1,i2,i3-1,kd))-(wram(i1,i2,i3+2,kd)+wram(i1,i2,
     & i3-2,kd)) )*h42(2)
      wramxy43r(i1,i2,i3,kd)=( (wram(i1+2,i2+2,i3,kd)-wram(i1-2,i2+2,
     & i3,kd)- wram(i1+2,i2-2,i3,kd)+wram(i1-2,i2-2,i3,kd)) +8.*(wram(
     & i1-1,i2+2,i3,kd)-wram(i1-1,i2-2,i3,kd)-wram(i1+1,i2+2,i3,kd)+
     & wram(i1+1,i2-2,i3,kd) +wram(i1+2,i2-1,i3,kd)-wram(i1-2,i2-1,i3,
     & kd)-wram(i1+2,i2+1,i3,kd)+wram(i1-2,i2+1,i3,kd))+64.*(wram(i1+
     & 1,i2+1,i3,kd)-wram(i1-1,i2+1,i3,kd)- wram(i1+1,i2-1,i3,kd)+
     & wram(i1-1,i2-1,i3,kd)))*(h41(0)*h41(1))
      wramxz43r(i1,i2,i3,kd)=( (wram(i1+2,i2,i3+2,kd)-wram(i1-2,i2,i3+
     & 2,kd)-wram(i1+2,i2,i3-2,kd)+wram(i1-2,i2,i3-2,kd)) +8.*(wram(
     & i1-1,i2,i3+2,kd)-wram(i1-1,i2,i3-2,kd)-wram(i1+1,i2,i3+2,kd)+
     & wram(i1+1,i2,i3-2,kd) +wram(i1+2,i2,i3-1,kd)-wram(i1-2,i2,i3-1,
     & kd)- wram(i1+2,i2,i3+1,kd)+wram(i1-2,i2,i3+1,kd)) +64.*(wram(
     & i1+1,i2,i3+1,kd)-wram(i1-1,i2,i3+1,kd)-wram(i1+1,i2,i3-1,kd)+
     & wram(i1-1,i2,i3-1,kd)) )*(h41(0)*h41(2))
      wramyz43r(i1,i2,i3,kd)=( (wram(i1,i2+2,i3+2,kd)-wram(i1,i2-2,i3+
     & 2,kd)-wram(i1,i2+2,i3-2,kd)+wram(i1,i2-2,i3-2,kd)) +8.*(wram(
     & i1,i2-1,i3+2,kd)-wram(i1,i2-1,i3-2,kd)-wram(i1,i2+1,i3+2,kd)+
     & wram(i1,i2+1,i3-2,kd) +wram(i1,i2+2,i3-1,kd)-wram(i1,i2-2,i3-1,
     & kd)-wram(i1,i2+2,i3+1,kd)+wram(i1,i2-2,i3+1,kd)) +64.*(wram(i1,
     & i2+1,i3+1,kd)-wram(i1,i2-1,i3+1,kd)-wram(i1,i2+1,i3-1,kd)+wram(
     & i1,i2-1,i3-1,kd)) )*(h41(1)*h41(2))
      wramx41r(i1,i2,i3,kd)= wramx43r(i1,i2,i3,kd)
      wramy41r(i1,i2,i3,kd)= wramy43r(i1,i2,i3,kd)
      wramz41r(i1,i2,i3,kd)= wramz43r(i1,i2,i3,kd)
      wramxx41r(i1,i2,i3,kd)= wramxx43r(i1,i2,i3,kd)
      wramyy41r(i1,i2,i3,kd)= wramyy43r(i1,i2,i3,kd)
      wramzz41r(i1,i2,i3,kd)= wramzz43r(i1,i2,i3,kd)
      wramxy41r(i1,i2,i3,kd)= wramxy43r(i1,i2,i3,kd)
      wramxz41r(i1,i2,i3,kd)= wramxz43r(i1,i2,i3,kd)
      wramyz41r(i1,i2,i3,kd)= wramyz43r(i1,i2,i3,kd)
      wramlaplacian41r(i1,i2,i3,kd)=wramxx43r(i1,i2,i3,kd)
      wramx42r(i1,i2,i3,kd)= wramx43r(i1,i2,i3,kd)
      wramy42r(i1,i2,i3,kd)= wramy43r(i1,i2,i3,kd)
      wramz42r(i1,i2,i3,kd)= wramz43r(i1,i2,i3,kd)
      wramxx42r(i1,i2,i3,kd)= wramxx43r(i1,i2,i3,kd)
      wramyy42r(i1,i2,i3,kd)= wramyy43r(i1,i2,i3,kd)
      wramzz42r(i1,i2,i3,kd)= wramzz43r(i1,i2,i3,kd)
      wramxy42r(i1,i2,i3,kd)= wramxy43r(i1,i2,i3,kd)
      wramxz42r(i1,i2,i3,kd)= wramxz43r(i1,i2,i3,kd)
      wramyz42r(i1,i2,i3,kd)= wramyz43r(i1,i2,i3,kd)
      wramlaplacian42r(i1,i2,i3,kd)=wramxx43r(i1,i2,i3,kd)+wramyy43r(
     & i1,i2,i3,kd)
      wramlaplacian43r(i1,i2,i3,kd)=wramxx43r(i1,i2,i3,kd)+wramyy43r(
     & i1,i2,i3,kd)+wramzz43r(i1,i2,i3,kd)

      vrbr4(i1,i2,i3,kd)=(8.*(vrb(i1+1,i2,i3,kd)-vrb(i1-1,i2,i3,kd))-(
     & vrb(i1+2,i2,i3,kd)-vrb(i1-2,i2,i3,kd)))*d14(0)
      vrbs4(i1,i2,i3,kd)=(8.*(vrb(i1,i2+1,i3,kd)-vrb(i1,i2-1,i3,kd))-(
     & vrb(i1,i2+2,i3,kd)-vrb(i1,i2-2,i3,kd)))*d14(1)
      vrbt4(i1,i2,i3,kd)=(8.*(vrb(i1,i2,i3+1,kd)-vrb(i1,i2,i3-1,kd))-(
     & vrb(i1,i2,i3+2,kd)-vrb(i1,i2,i3-2,kd)))*d14(2)
      vrbrr4(i1,i2,i3,kd)=(-30.*vrb(i1,i2,i3,kd)+16.*(vrb(i1+1,i2,i3,
     & kd)+vrb(i1-1,i2,i3,kd))-(vrb(i1+2,i2,i3,kd)+vrb(i1-2,i2,i3,kd))
     &  )*d24(0)
      vrbss4(i1,i2,i3,kd)=(-30.*vrb(i1,i2,i3,kd)+16.*(vrb(i1,i2+1,i3,
     & kd)+vrb(i1,i2-1,i3,kd))-(vrb(i1,i2+2,i3,kd)+vrb(i1,i2-2,i3,kd))
     &  )*d24(1)
      vrbtt4(i1,i2,i3,kd)=(-30.*vrb(i1,i2,i3,kd)+16.*(vrb(i1,i2,i3+1,
     & kd)+vrb(i1,i2,i3-1,kd))-(vrb(i1,i2,i3+2,kd)+vrb(i1,i2,i3-2,kd))
     &  )*d24(2)
      vrbrs4(i1,i2,i3,kd)=(8.*(vrbr4(i1,i2+1,i3,kd)-vrbr4(i1,i2-1,i3,
     & kd))-(vrbr4(i1,i2+2,i3,kd)-vrbr4(i1,i2-2,i3,kd)))*d14(1)
      vrbrt4(i1,i2,i3,kd)=(8.*(vrbr4(i1,i2,i3+1,kd)-vrbr4(i1,i2,i3-1,
     & kd))-(vrbr4(i1,i2,i3+2,kd)-vrbr4(i1,i2,i3-2,kd)))*d14(2)
      vrbst4(i1,i2,i3,kd)=(8.*(vrbs4(i1,i2,i3+1,kd)-vrbs4(i1,i2,i3-1,
     & kd))-(vrbs4(i1,i2,i3+2,kd)-vrbs4(i1,i2,i3-2,kd)))*d14(2)
      vrbx41(i1,i2,i3,kd)= rx(i1,i2,i3)*vrbr4(i1,i2,i3,kd)
      vrby41(i1,i2,i3,kd)=0
      vrbz41(i1,i2,i3,kd)=0
      vrbx42(i1,i2,i3,kd)= rx(i1,i2,i3)*vrbr4(i1,i2,i3,kd)+sx(i1,i2,i3)
     & *vrbs4(i1,i2,i3,kd)
      vrby42(i1,i2,i3,kd)= ry(i1,i2,i3)*vrbr4(i1,i2,i3,kd)+sy(i1,i2,i3)
     & *vrbs4(i1,i2,i3,kd)
      vrbz42(i1,i2,i3,kd)=0
      vrbx43(i1,i2,i3,kd)=rx(i1,i2,i3)*vrbr4(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & vrbs4(i1,i2,i3,kd)+tx(i1,i2,i3)*vrbt4(i1,i2,i3,kd)
      vrby43(i1,i2,i3,kd)=ry(i1,i2,i3)*vrbr4(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & vrbs4(i1,i2,i3,kd)+ty(i1,i2,i3)*vrbt4(i1,i2,i3,kd)
      vrbz43(i1,i2,i3,kd)=rz(i1,i2,i3)*vrbr4(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & vrbs4(i1,i2,i3,kd)+tz(i1,i2,i3)*vrbt4(i1,i2,i3,kd)
      vrbxx41(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vrbrr4(i1,i2,i3,kd)+(
     & rxx42(i1,i2,i3))*vrbr4(i1,i2,i3,kd)
      vrbyy41(i1,i2,i3,kd)=0
      vrbxy41(i1,i2,i3,kd)=0
      vrbxz41(i1,i2,i3,kd)=0
      vrbyz41(i1,i2,i3,kd)=0
      vrbzz41(i1,i2,i3,kd)=0
      vrblaplacian41(i1,i2,i3,kd)=vrbxx41(i1,i2,i3,kd)
      vrbxx42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vrbrr4(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*vrbrs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*vrbss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3))*vrbr4(i1,i2,i3,kd)+(
     & sxx42(i1,i2,i3))*vrbs4(i1,i2,i3,kd)
      vrbyy42(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*vrbrr4(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*vrbrs4(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*vrbss4(i1,i2,i3,kd)+(ryy42(i1,i2,i3))*vrbr4(i1,i2,i3,kd)+(
     & syy42(i1,i2,i3))*vrbs4(i1,i2,i3,kd)
      vrbxy42(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vrbrr4(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & vrbrs4(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vrbss4(i1,i2,i3,
     & kd)+rxy42(i1,i2,i3)*vrbr4(i1,i2,i3,kd)+sxy42(i1,i2,i3)*vrbs4(
     & i1,i2,i3,kd)
      vrbxz42(i1,i2,i3,kd)=0
      vrbyz42(i1,i2,i3,kd)=0
      vrbzz42(i1,i2,i3,kd)=0
      vrblaplacian42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & vrbrr4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)
     & *sy(i1,i2,i3))*vrbrs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2)*vrbss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3)+ryy42(i1,i2,i3))*
     & vrbr4(i1,i2,i3,kd)+(sxx42(i1,i2,i3)+syy42(i1,i2,i3))*vrbs4(i1,
     & i2,i3,kd)
      vrbxx43(i1,i2,i3,kd)=rx(i1,i2,i3)**2*vrbrr4(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*vrbss4(i1,i2,i3,kd)+tx(i1,i2,i3)**2*vrbtt4(i1,i2,i3,
     & kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*vrbrs4(i1,i2,i3,kd)+2.*rx(i1,
     & i2,i3)*tx(i1,i2,i3)*vrbrt4(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,
     & i2,i3)*vrbst4(i1,i2,i3,kd)+rxx43(i1,i2,i3)*vrbr4(i1,i2,i3,kd)+
     & sxx43(i1,i2,i3)*vrbs4(i1,i2,i3,kd)+txx43(i1,i2,i3)*vrbt4(i1,i2,
     & i3,kd)
      vrbyy43(i1,i2,i3,kd)=ry(i1,i2,i3)**2*vrbrr4(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*vrbss4(i1,i2,i3,kd)+ty(i1,i2,i3)**2*vrbtt4(i1,i2,i3,
     & kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*vrbrs4(i1,i2,i3,kd)+2.*ry(i1,
     & i2,i3)*ty(i1,i2,i3)*vrbrt4(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,
     & i2,i3)*vrbst4(i1,i2,i3,kd)+ryy43(i1,i2,i3)*vrbr4(i1,i2,i3,kd)+
     & syy43(i1,i2,i3)*vrbs4(i1,i2,i3,kd)+tyy43(i1,i2,i3)*vrbt4(i1,i2,
     & i3,kd)
      vrbzz43(i1,i2,i3,kd)=rz(i1,i2,i3)**2*vrbrr4(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*vrbss4(i1,i2,i3,kd)+tz(i1,i2,i3)**2*vrbtt4(i1,i2,i3,
     & kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*vrbrs4(i1,i2,i3,kd)+2.*rz(i1,
     & i2,i3)*tz(i1,i2,i3)*vrbrt4(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,
     & i2,i3)*vrbst4(i1,i2,i3,kd)+rzz43(i1,i2,i3)*vrbr4(i1,i2,i3,kd)+
     & szz43(i1,i2,i3)*vrbs4(i1,i2,i3,kd)+tzz43(i1,i2,i3)*vrbt4(i1,i2,
     & i3,kd)
      vrbxy43(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vrbrr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vrbss4(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & ty(i1,i2,i3)*vrbtt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(
     & i1,i2,i3)*sx(i1,i2,i3))*vrbrs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(
     & i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*vrbrt4(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*vrbst4(i1,i2,
     & i3,kd)+rxy43(i1,i2,i3)*vrbr4(i1,i2,i3,kd)+sxy43(i1,i2,i3)*
     & vrbs4(i1,i2,i3,kd)+txy43(i1,i2,i3)*vrbt4(i1,i2,i3,kd)
      vrbxz43(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*vrbrr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*vrbss4(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & tz(i1,i2,i3)*vrbtt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sx(i1,i2,i3))*vrbrs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*vrbrt4(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*vrbst4(i1,i2,
     & i3,kd)+rxz43(i1,i2,i3)*vrbr4(i1,i2,i3,kd)+sxz43(i1,i2,i3)*
     & vrbs4(i1,i2,i3,kd)+txz43(i1,i2,i3)*vrbt4(i1,i2,i3,kd)
      vrbyz43(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*vrbrr4(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*vrbss4(i1,i2,i3,kd)+ty(i1,i2,i3)*
     & tz(i1,i2,i3)*vrbtt4(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sy(i1,i2,i3))*vrbrs4(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*vrbrt4(i1,i2,i3,kd)+(sy(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*vrbst4(i1,i2,
     & i3,kd)+ryz43(i1,i2,i3)*vrbr4(i1,i2,i3,kd)+syz43(i1,i2,i3)*
     & vrbs4(i1,i2,i3,kd)+tyz43(i1,i2,i3)*vrbt4(i1,i2,i3,kd)
      vrblaplacian43(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*vrbrr4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)
     & **2+sz(i1,i2,i3)**2)*vrbss4(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(
     & i1,i2,i3)**2+tz(i1,i2,i3)**2)*vrbtt4(i1,i2,i3,kd)+2.*(rx(i1,i2,
     & i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,
     & i2,i3))*vrbrs4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(
     & i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*vrbrt4(i1,i2,
     & i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)
     & +sz(i1,i2,i3)*tz(i1,i2,i3))*vrbst4(i1,i2,i3,kd)+(rxx43(i1,i2,
     & i3)+ryy43(i1,i2,i3)+rzz43(i1,i2,i3))*vrbr4(i1,i2,i3,kd)+(sxx43(
     & i1,i2,i3)+syy43(i1,i2,i3)+szz43(i1,i2,i3))*vrbs4(i1,i2,i3,kd)+(
     & txx43(i1,i2,i3)+tyy43(i1,i2,i3)+tzz43(i1,i2,i3))*vrbt4(i1,i2,
     & i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      vrbx43r(i1,i2,i3,kd)=(8.*(vrb(i1+1,i2,i3,kd)-vrb(i1-1,i2,i3,kd))-
     & (vrb(i1+2,i2,i3,kd)-vrb(i1-2,i2,i3,kd)))*h41(0)
      vrby43r(i1,i2,i3,kd)=(8.*(vrb(i1,i2+1,i3,kd)-vrb(i1,i2-1,i3,kd))-
     & (vrb(i1,i2+2,i3,kd)-vrb(i1,i2-2,i3,kd)))*h41(1)
      vrbz43r(i1,i2,i3,kd)=(8.*(vrb(i1,i2,i3+1,kd)-vrb(i1,i2,i3-1,kd))-
     & (vrb(i1,i2,i3+2,kd)-vrb(i1,i2,i3-2,kd)))*h41(2)
      vrbxx43r(i1,i2,i3,kd)=( -30.*vrb(i1,i2,i3,kd)+16.*(vrb(i1+1,i2,
     & i3,kd)+vrb(i1-1,i2,i3,kd))-(vrb(i1+2,i2,i3,kd)+vrb(i1-2,i2,i3,
     & kd)) )*h42(0)
      vrbyy43r(i1,i2,i3,kd)=( -30.*vrb(i1,i2,i3,kd)+16.*(vrb(i1,i2+1,
     & i3,kd)+vrb(i1,i2-1,i3,kd))-(vrb(i1,i2+2,i3,kd)+vrb(i1,i2-2,i3,
     & kd)) )*h42(1)
      vrbzz43r(i1,i2,i3,kd)=( -30.*vrb(i1,i2,i3,kd)+16.*(vrb(i1,i2,i3+
     & 1,kd)+vrb(i1,i2,i3-1,kd))-(vrb(i1,i2,i3+2,kd)+vrb(i1,i2,i3-2,
     & kd)) )*h42(2)
      vrbxy43r(i1,i2,i3,kd)=( (vrb(i1+2,i2+2,i3,kd)-vrb(i1-2,i2+2,i3,
     & kd)- vrb(i1+2,i2-2,i3,kd)+vrb(i1-2,i2-2,i3,kd)) +8.*(vrb(i1-1,
     & i2+2,i3,kd)-vrb(i1-1,i2-2,i3,kd)-vrb(i1+1,i2+2,i3,kd)+vrb(i1+1,
     & i2-2,i3,kd) +vrb(i1+2,i2-1,i3,kd)-vrb(i1-2,i2-1,i3,kd)-vrb(i1+
     & 2,i2+1,i3,kd)+vrb(i1-2,i2+1,i3,kd))+64.*(vrb(i1+1,i2+1,i3,kd)-
     & vrb(i1-1,i2+1,i3,kd)- vrb(i1+1,i2-1,i3,kd)+vrb(i1-1,i2-1,i3,kd)
     & ))*(h41(0)*h41(1))
      vrbxz43r(i1,i2,i3,kd)=( (vrb(i1+2,i2,i3+2,kd)-vrb(i1-2,i2,i3+2,
     & kd)-vrb(i1+2,i2,i3-2,kd)+vrb(i1-2,i2,i3-2,kd)) +8.*(vrb(i1-1,
     & i2,i3+2,kd)-vrb(i1-1,i2,i3-2,kd)-vrb(i1+1,i2,i3+2,kd)+vrb(i1+1,
     & i2,i3-2,kd) +vrb(i1+2,i2,i3-1,kd)-vrb(i1-2,i2,i3-1,kd)- vrb(i1+
     & 2,i2,i3+1,kd)+vrb(i1-2,i2,i3+1,kd)) +64.*(vrb(i1+1,i2,i3+1,kd)-
     & vrb(i1-1,i2,i3+1,kd)-vrb(i1+1,i2,i3-1,kd)+vrb(i1-1,i2,i3-1,kd))
     &  )*(h41(0)*h41(2))
      vrbyz43r(i1,i2,i3,kd)=( (vrb(i1,i2+2,i3+2,kd)-vrb(i1,i2-2,i3+2,
     & kd)-vrb(i1,i2+2,i3-2,kd)+vrb(i1,i2-2,i3-2,kd)) +8.*(vrb(i1,i2-
     & 1,i3+2,kd)-vrb(i1,i2-1,i3-2,kd)-vrb(i1,i2+1,i3+2,kd)+vrb(i1,i2+
     & 1,i3-2,kd) +vrb(i1,i2+2,i3-1,kd)-vrb(i1,i2-2,i3-1,kd)-vrb(i1,
     & i2+2,i3+1,kd)+vrb(i1,i2-2,i3+1,kd)) +64.*(vrb(i1,i2+1,i3+1,kd)-
     & vrb(i1,i2-1,i3+1,kd)-vrb(i1,i2+1,i3-1,kd)+vrb(i1,i2-1,i3-1,kd))
     &  )*(h41(1)*h41(2))
      vrbx41r(i1,i2,i3,kd)= vrbx43r(i1,i2,i3,kd)
      vrby41r(i1,i2,i3,kd)= vrby43r(i1,i2,i3,kd)
      vrbz41r(i1,i2,i3,kd)= vrbz43r(i1,i2,i3,kd)
      vrbxx41r(i1,i2,i3,kd)= vrbxx43r(i1,i2,i3,kd)
      vrbyy41r(i1,i2,i3,kd)= vrbyy43r(i1,i2,i3,kd)
      vrbzz41r(i1,i2,i3,kd)= vrbzz43r(i1,i2,i3,kd)
      vrbxy41r(i1,i2,i3,kd)= vrbxy43r(i1,i2,i3,kd)
      vrbxz41r(i1,i2,i3,kd)= vrbxz43r(i1,i2,i3,kd)
      vrbyz41r(i1,i2,i3,kd)= vrbyz43r(i1,i2,i3,kd)
      vrblaplacian41r(i1,i2,i3,kd)=vrbxx43r(i1,i2,i3,kd)
      vrbx42r(i1,i2,i3,kd)= vrbx43r(i1,i2,i3,kd)
      vrby42r(i1,i2,i3,kd)= vrby43r(i1,i2,i3,kd)
      vrbz42r(i1,i2,i3,kd)= vrbz43r(i1,i2,i3,kd)
      vrbxx42r(i1,i2,i3,kd)= vrbxx43r(i1,i2,i3,kd)
      vrbyy42r(i1,i2,i3,kd)= vrbyy43r(i1,i2,i3,kd)
      vrbzz42r(i1,i2,i3,kd)= vrbzz43r(i1,i2,i3,kd)
      vrbxy42r(i1,i2,i3,kd)= vrbxy43r(i1,i2,i3,kd)
      vrbxz42r(i1,i2,i3,kd)= vrbxz43r(i1,i2,i3,kd)
      vrbyz42r(i1,i2,i3,kd)= vrbyz43r(i1,i2,i3,kd)
      vrblaplacian42r(i1,i2,i3,kd)=vrbxx43r(i1,i2,i3,kd)+vrbyy43r(i1,
     & i2,i3,kd)
      vrblaplacian43r(i1,i2,i3,kd)=vrbxx43r(i1,i2,i3,kd)+vrbyy43r(i1,
     & i2,i3,kd)+vrbzz43r(i1,i2,i3,kd)
      vrbmr4(i1,i2,i3,kd)=(8.*(vrbm(i1+1,i2,i3,kd)-vrbm(i1-1,i2,i3,kd))
     & -(vrbm(i1+2,i2,i3,kd)-vrbm(i1-2,i2,i3,kd)))*d14(0)
      vrbms4(i1,i2,i3,kd)=(8.*(vrbm(i1,i2+1,i3,kd)-vrbm(i1,i2-1,i3,kd))
     & -(vrbm(i1,i2+2,i3,kd)-vrbm(i1,i2-2,i3,kd)))*d14(1)
      vrbmt4(i1,i2,i3,kd)=(8.*(vrbm(i1,i2,i3+1,kd)-vrbm(i1,i2,i3-1,kd))
     & -(vrbm(i1,i2,i3+2,kd)-vrbm(i1,i2,i3-2,kd)))*d14(2)
      vrbmrr4(i1,i2,i3,kd)=(-30.*vrbm(i1,i2,i3,kd)+16.*(vrbm(i1+1,i2,
     & i3,kd)+vrbm(i1-1,i2,i3,kd))-(vrbm(i1+2,i2,i3,kd)+vrbm(i1-2,i2,
     & i3,kd)) )*d24(0)
      vrbmss4(i1,i2,i3,kd)=(-30.*vrbm(i1,i2,i3,kd)+16.*(vrbm(i1,i2+1,
     & i3,kd)+vrbm(i1,i2-1,i3,kd))-(vrbm(i1,i2+2,i3,kd)+vrbm(i1,i2-2,
     & i3,kd)) )*d24(1)
      vrbmtt4(i1,i2,i3,kd)=(-30.*vrbm(i1,i2,i3,kd)+16.*(vrbm(i1,i2,i3+
     & 1,kd)+vrbm(i1,i2,i3-1,kd))-(vrbm(i1,i2,i3+2,kd)+vrbm(i1,i2,i3-
     & 2,kd)) )*d24(2)
      vrbmrs4(i1,i2,i3,kd)=(8.*(vrbmr4(i1,i2+1,i3,kd)-vrbmr4(i1,i2-1,
     & i3,kd))-(vrbmr4(i1,i2+2,i3,kd)-vrbmr4(i1,i2-2,i3,kd)))*d14(1)
      vrbmrt4(i1,i2,i3,kd)=(8.*(vrbmr4(i1,i2,i3+1,kd)-vrbmr4(i1,i2,i3-
     & 1,kd))-(vrbmr4(i1,i2,i3+2,kd)-vrbmr4(i1,i2,i3-2,kd)))*d14(2)
      vrbmst4(i1,i2,i3,kd)=(8.*(vrbms4(i1,i2,i3+1,kd)-vrbms4(i1,i2,i3-
     & 1,kd))-(vrbms4(i1,i2,i3+2,kd)-vrbms4(i1,i2,i3-2,kd)))*d14(2)
      vrbmx41(i1,i2,i3,kd)= rx(i1,i2,i3)*vrbmr4(i1,i2,i3,kd)
      vrbmy41(i1,i2,i3,kd)=0
      vrbmz41(i1,i2,i3,kd)=0
      vrbmx42(i1,i2,i3,kd)= rx(i1,i2,i3)*vrbmr4(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*vrbms4(i1,i2,i3,kd)
      vrbmy42(i1,i2,i3,kd)= ry(i1,i2,i3)*vrbmr4(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*vrbms4(i1,i2,i3,kd)
      vrbmz42(i1,i2,i3,kd)=0
      vrbmx43(i1,i2,i3,kd)=rx(i1,i2,i3)*vrbmr4(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*vrbms4(i1,i2,i3,kd)+tx(i1,i2,i3)*vrbmt4(i1,i2,i3,kd)
      vrbmy43(i1,i2,i3,kd)=ry(i1,i2,i3)*vrbmr4(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*vrbms4(i1,i2,i3,kd)+ty(i1,i2,i3)*vrbmt4(i1,i2,i3,kd)
      vrbmz43(i1,i2,i3,kd)=rz(i1,i2,i3)*vrbmr4(i1,i2,i3,kd)+sz(i1,i2,
     & i3)*vrbms4(i1,i2,i3,kd)+tz(i1,i2,i3)*vrbmt4(i1,i2,i3,kd)
      vrbmxx41(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vrbmrr4(i1,i2,i3,kd)+(
     & rxx42(i1,i2,i3))*vrbmr4(i1,i2,i3,kd)
      vrbmyy41(i1,i2,i3,kd)=0
      vrbmxy41(i1,i2,i3,kd)=0
      vrbmxz41(i1,i2,i3,kd)=0
      vrbmyz41(i1,i2,i3,kd)=0
      vrbmzz41(i1,i2,i3,kd)=0
      vrbmlaplacian41(i1,i2,i3,kd)=vrbmxx41(i1,i2,i3,kd)
      vrbmxx42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vrbmrr4(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*vrbmrs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*vrbmss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3))*vrbmr4(i1,i2,i3,kd)+(
     & sxx42(i1,i2,i3))*vrbms4(i1,i2,i3,kd)
      vrbmyy42(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*vrbmrr4(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*vrbmrs4(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*vrbmss4(i1,i2,i3,kd)+(ryy42(i1,i2,i3))*vrbmr4(i1,i2,i3,kd)+(
     & syy42(i1,i2,i3))*vrbms4(i1,i2,i3,kd)
      vrbmxy42(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vrbmrr4(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & vrbmrs4(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vrbmss4(i1,i2,
     & i3,kd)+rxy42(i1,i2,i3)*vrbmr4(i1,i2,i3,kd)+sxy42(i1,i2,i3)*
     & vrbms4(i1,i2,i3,kd)
      vrbmxz42(i1,i2,i3,kd)=0
      vrbmyz42(i1,i2,i3,kd)=0
      vrbmzz42(i1,i2,i3,kd)=0
      vrbmlaplacian42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & vrbmrr4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,
     & i3)*sy(i1,i2,i3))*vrbmrs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,
     & i2,i3)**2)*vrbmss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3)+ryy42(i1,i2,
     & i3))*vrbmr4(i1,i2,i3,kd)+(sxx42(i1,i2,i3)+syy42(i1,i2,i3))*
     & vrbms4(i1,i2,i3,kd)
      vrbmxx43(i1,i2,i3,kd)=rx(i1,i2,i3)**2*vrbmrr4(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*vrbmss4(i1,i2,i3,kd)+tx(i1,i2,i3)**2*vrbmtt4(i1,i2,
     & i3,kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*vrbmrs4(i1,i2,i3,kd)+2.*rx(
     & i1,i2,i3)*tx(i1,i2,i3)*vrbmrt4(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(
     & i1,i2,i3)*vrbmst4(i1,i2,i3,kd)+rxx43(i1,i2,i3)*vrbmr4(i1,i2,i3,
     & kd)+sxx43(i1,i2,i3)*vrbms4(i1,i2,i3,kd)+txx43(i1,i2,i3)*vrbmt4(
     & i1,i2,i3,kd)
      vrbmyy43(i1,i2,i3,kd)=ry(i1,i2,i3)**2*vrbmrr4(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*vrbmss4(i1,i2,i3,kd)+ty(i1,i2,i3)**2*vrbmtt4(i1,i2,
     & i3,kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*vrbmrs4(i1,i2,i3,kd)+2.*ry(
     & i1,i2,i3)*ty(i1,i2,i3)*vrbmrt4(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(
     & i1,i2,i3)*vrbmst4(i1,i2,i3,kd)+ryy43(i1,i2,i3)*vrbmr4(i1,i2,i3,
     & kd)+syy43(i1,i2,i3)*vrbms4(i1,i2,i3,kd)+tyy43(i1,i2,i3)*vrbmt4(
     & i1,i2,i3,kd)
      vrbmzz43(i1,i2,i3,kd)=rz(i1,i2,i3)**2*vrbmrr4(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*vrbmss4(i1,i2,i3,kd)+tz(i1,i2,i3)**2*vrbmtt4(i1,i2,
     & i3,kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*vrbmrs4(i1,i2,i3,kd)+2.*rz(
     & i1,i2,i3)*tz(i1,i2,i3)*vrbmrt4(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(
     & i1,i2,i3)*vrbmst4(i1,i2,i3,kd)+rzz43(i1,i2,i3)*vrbmr4(i1,i2,i3,
     & kd)+szz43(i1,i2,i3)*vrbms4(i1,i2,i3,kd)+tzz43(i1,i2,i3)*vrbmt4(
     & i1,i2,i3,kd)
      vrbmxy43(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vrbmrr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vrbmss4(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *ty(i1,i2,i3)*vrbmtt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+
     & ry(i1,i2,i3)*sx(i1,i2,i3))*vrbmrs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & ty(i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*vrbmrt4(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*vrbmst4(
     & i1,i2,i3,kd)+rxy43(i1,i2,i3)*vrbmr4(i1,i2,i3,kd)+sxy43(i1,i2,
     & i3)*vrbms4(i1,i2,i3,kd)+txy43(i1,i2,i3)*vrbmt4(i1,i2,i3,kd)
      vrbmxz43(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*vrbmrr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*vrbmss4(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *tz(i1,i2,i3)*vrbmtt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sx(i1,i2,i3))*vrbmrs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*vrbmrt4(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*vrbmst4(
     & i1,i2,i3,kd)+rxz43(i1,i2,i3)*vrbmr4(i1,i2,i3,kd)+sxz43(i1,i2,
     & i3)*vrbms4(i1,i2,i3,kd)+txz43(i1,i2,i3)*vrbmt4(i1,i2,i3,kd)
      vrbmyz43(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*vrbmrr4(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*vrbmss4(i1,i2,i3,kd)+ty(i1,i2,i3)
     & *tz(i1,i2,i3)*vrbmtt4(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sy(i1,i2,i3))*vrbmrs4(i1,i2,i3,kd)+(ry(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*vrbmrt4(i1,i2,i3,kd)+(
     & sy(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*vrbmst4(
     & i1,i2,i3,kd)+ryz43(i1,i2,i3)*vrbmr4(i1,i2,i3,kd)+syz43(i1,i2,
     & i3)*vrbms4(i1,i2,i3,kd)+tyz43(i1,i2,i3)*vrbmt4(i1,i2,i3,kd)
      vrbmlaplacian43(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*vrbmrr4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2+sz(i1,i2,i3)**2)*vrbmss4(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+
     & ty(i1,i2,i3)**2+tz(i1,i2,i3)**2)*vrbmtt4(i1,i2,i3,kd)+2.*(rx(
     & i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*
     & sz(i1,i2,i3))*vrbmrs4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,
     & i3)+ ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*
     & vrbmrt4(i1,i2,i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,
     & i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3))*vrbmst4(i1,i2,i3,
     & kd)+(rxx43(i1,i2,i3)+ryy43(i1,i2,i3)+rzz43(i1,i2,i3))*vrbmr4(
     & i1,i2,i3,kd)+(sxx43(i1,i2,i3)+syy43(i1,i2,i3)+szz43(i1,i2,i3))*
     & vrbms4(i1,i2,i3,kd)+(txx43(i1,i2,i3)+tyy43(i1,i2,i3)+tzz43(i1,
     & i2,i3))*vrbmt4(i1,i2,i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      vrbmx43r(i1,i2,i3,kd)=(8.*(vrbm(i1+1,i2,i3,kd)-vrbm(i1-1,i2,i3,
     & kd))-(vrbm(i1+2,i2,i3,kd)-vrbm(i1-2,i2,i3,kd)))*h41(0)
      vrbmy43r(i1,i2,i3,kd)=(8.*(vrbm(i1,i2+1,i3,kd)-vrbm(i1,i2-1,i3,
     & kd))-(vrbm(i1,i2+2,i3,kd)-vrbm(i1,i2-2,i3,kd)))*h41(1)
      vrbmz43r(i1,i2,i3,kd)=(8.*(vrbm(i1,i2,i3+1,kd)-vrbm(i1,i2,i3-1,
     & kd))-(vrbm(i1,i2,i3+2,kd)-vrbm(i1,i2,i3-2,kd)))*h41(2)
      vrbmxx43r(i1,i2,i3,kd)=( -30.*vrbm(i1,i2,i3,kd)+16.*(vrbm(i1+1,
     & i2,i3,kd)+vrbm(i1-1,i2,i3,kd))-(vrbm(i1+2,i2,i3,kd)+vrbm(i1-2,
     & i2,i3,kd)) )*h42(0)
      vrbmyy43r(i1,i2,i3,kd)=( -30.*vrbm(i1,i2,i3,kd)+16.*(vrbm(i1,i2+
     & 1,i3,kd)+vrbm(i1,i2-1,i3,kd))-(vrbm(i1,i2+2,i3,kd)+vrbm(i1,i2-
     & 2,i3,kd)) )*h42(1)
      vrbmzz43r(i1,i2,i3,kd)=( -30.*vrbm(i1,i2,i3,kd)+16.*(vrbm(i1,i2,
     & i3+1,kd)+vrbm(i1,i2,i3-1,kd))-(vrbm(i1,i2,i3+2,kd)+vrbm(i1,i2,
     & i3-2,kd)) )*h42(2)
      vrbmxy43r(i1,i2,i3,kd)=( (vrbm(i1+2,i2+2,i3,kd)-vrbm(i1-2,i2+2,
     & i3,kd)- vrbm(i1+2,i2-2,i3,kd)+vrbm(i1-2,i2-2,i3,kd)) +8.*(vrbm(
     & i1-1,i2+2,i3,kd)-vrbm(i1-1,i2-2,i3,kd)-vrbm(i1+1,i2+2,i3,kd)+
     & vrbm(i1+1,i2-2,i3,kd) +vrbm(i1+2,i2-1,i3,kd)-vrbm(i1-2,i2-1,i3,
     & kd)-vrbm(i1+2,i2+1,i3,kd)+vrbm(i1-2,i2+1,i3,kd))+64.*(vrbm(i1+
     & 1,i2+1,i3,kd)-vrbm(i1-1,i2+1,i3,kd)- vrbm(i1+1,i2-1,i3,kd)+
     & vrbm(i1-1,i2-1,i3,kd)))*(h41(0)*h41(1))
      vrbmxz43r(i1,i2,i3,kd)=( (vrbm(i1+2,i2,i3+2,kd)-vrbm(i1-2,i2,i3+
     & 2,kd)-vrbm(i1+2,i2,i3-2,kd)+vrbm(i1-2,i2,i3-2,kd)) +8.*(vrbm(
     & i1-1,i2,i3+2,kd)-vrbm(i1-1,i2,i3-2,kd)-vrbm(i1+1,i2,i3+2,kd)+
     & vrbm(i1+1,i2,i3-2,kd) +vrbm(i1+2,i2,i3-1,kd)-vrbm(i1-2,i2,i3-1,
     & kd)- vrbm(i1+2,i2,i3+1,kd)+vrbm(i1-2,i2,i3+1,kd)) +64.*(vrbm(
     & i1+1,i2,i3+1,kd)-vrbm(i1-1,i2,i3+1,kd)-vrbm(i1+1,i2,i3-1,kd)+
     & vrbm(i1-1,i2,i3-1,kd)) )*(h41(0)*h41(2))
      vrbmyz43r(i1,i2,i3,kd)=( (vrbm(i1,i2+2,i3+2,kd)-vrbm(i1,i2-2,i3+
     & 2,kd)-vrbm(i1,i2+2,i3-2,kd)+vrbm(i1,i2-2,i3-2,kd)) +8.*(vrbm(
     & i1,i2-1,i3+2,kd)-vrbm(i1,i2-1,i3-2,kd)-vrbm(i1,i2+1,i3+2,kd)+
     & vrbm(i1,i2+1,i3-2,kd) +vrbm(i1,i2+2,i3-1,kd)-vrbm(i1,i2-2,i3-1,
     & kd)-vrbm(i1,i2+2,i3+1,kd)+vrbm(i1,i2-2,i3+1,kd)) +64.*(vrbm(i1,
     & i2+1,i3+1,kd)-vrbm(i1,i2-1,i3+1,kd)-vrbm(i1,i2+1,i3-1,kd)+vrbm(
     & i1,i2-1,i3-1,kd)) )*(h41(1)*h41(2))
      vrbmx41r(i1,i2,i3,kd)= vrbmx43r(i1,i2,i3,kd)
      vrbmy41r(i1,i2,i3,kd)= vrbmy43r(i1,i2,i3,kd)
      vrbmz41r(i1,i2,i3,kd)= vrbmz43r(i1,i2,i3,kd)
      vrbmxx41r(i1,i2,i3,kd)= vrbmxx43r(i1,i2,i3,kd)
      vrbmyy41r(i1,i2,i3,kd)= vrbmyy43r(i1,i2,i3,kd)
      vrbmzz41r(i1,i2,i3,kd)= vrbmzz43r(i1,i2,i3,kd)
      vrbmxy41r(i1,i2,i3,kd)= vrbmxy43r(i1,i2,i3,kd)
      vrbmxz41r(i1,i2,i3,kd)= vrbmxz43r(i1,i2,i3,kd)
      vrbmyz41r(i1,i2,i3,kd)= vrbmyz43r(i1,i2,i3,kd)
      vrbmlaplacian41r(i1,i2,i3,kd)=vrbmxx43r(i1,i2,i3,kd)
      vrbmx42r(i1,i2,i3,kd)= vrbmx43r(i1,i2,i3,kd)
      vrbmy42r(i1,i2,i3,kd)= vrbmy43r(i1,i2,i3,kd)
      vrbmz42r(i1,i2,i3,kd)= vrbmz43r(i1,i2,i3,kd)
      vrbmxx42r(i1,i2,i3,kd)= vrbmxx43r(i1,i2,i3,kd)
      vrbmyy42r(i1,i2,i3,kd)= vrbmyy43r(i1,i2,i3,kd)
      vrbmzz42r(i1,i2,i3,kd)= vrbmzz43r(i1,i2,i3,kd)
      vrbmxy42r(i1,i2,i3,kd)= vrbmxy43r(i1,i2,i3,kd)
      vrbmxz42r(i1,i2,i3,kd)= vrbmxz43r(i1,i2,i3,kd)
      vrbmyz42r(i1,i2,i3,kd)= vrbmyz43r(i1,i2,i3,kd)
      vrbmlaplacian42r(i1,i2,i3,kd)=vrbmxx43r(i1,i2,i3,kd)+vrbmyy43r(
     & i1,i2,i3,kd)
      vrbmlaplacian43r(i1,i2,i3,kd)=vrbmxx43r(i1,i2,i3,kd)+vrbmyy43r(
     & i1,i2,i3,kd)+vrbmzz43r(i1,i2,i3,kd)
      wrbr4(i1,i2,i3,kd)=(8.*(wrb(i1+1,i2,i3,kd)-wrb(i1-1,i2,i3,kd))-(
     & wrb(i1+2,i2,i3,kd)-wrb(i1-2,i2,i3,kd)))*d14(0)
      wrbs4(i1,i2,i3,kd)=(8.*(wrb(i1,i2+1,i3,kd)-wrb(i1,i2-1,i3,kd))-(
     & wrb(i1,i2+2,i3,kd)-wrb(i1,i2-2,i3,kd)))*d14(1)
      wrbt4(i1,i2,i3,kd)=(8.*(wrb(i1,i2,i3+1,kd)-wrb(i1,i2,i3-1,kd))-(
     & wrb(i1,i2,i3+2,kd)-wrb(i1,i2,i3-2,kd)))*d14(2)
      wrbrr4(i1,i2,i3,kd)=(-30.*wrb(i1,i2,i3,kd)+16.*(wrb(i1+1,i2,i3,
     & kd)+wrb(i1-1,i2,i3,kd))-(wrb(i1+2,i2,i3,kd)+wrb(i1-2,i2,i3,kd))
     &  )*d24(0)
      wrbss4(i1,i2,i3,kd)=(-30.*wrb(i1,i2,i3,kd)+16.*(wrb(i1,i2+1,i3,
     & kd)+wrb(i1,i2-1,i3,kd))-(wrb(i1,i2+2,i3,kd)+wrb(i1,i2-2,i3,kd))
     &  )*d24(1)
      wrbtt4(i1,i2,i3,kd)=(-30.*wrb(i1,i2,i3,kd)+16.*(wrb(i1,i2,i3+1,
     & kd)+wrb(i1,i2,i3-1,kd))-(wrb(i1,i2,i3+2,kd)+wrb(i1,i2,i3-2,kd))
     &  )*d24(2)
      wrbrs4(i1,i2,i3,kd)=(8.*(wrbr4(i1,i2+1,i3,kd)-wrbr4(i1,i2-1,i3,
     & kd))-(wrbr4(i1,i2+2,i3,kd)-wrbr4(i1,i2-2,i3,kd)))*d14(1)
      wrbrt4(i1,i2,i3,kd)=(8.*(wrbr4(i1,i2,i3+1,kd)-wrbr4(i1,i2,i3-1,
     & kd))-(wrbr4(i1,i2,i3+2,kd)-wrbr4(i1,i2,i3-2,kd)))*d14(2)
      wrbst4(i1,i2,i3,kd)=(8.*(wrbs4(i1,i2,i3+1,kd)-wrbs4(i1,i2,i3-1,
     & kd))-(wrbs4(i1,i2,i3+2,kd)-wrbs4(i1,i2,i3-2,kd)))*d14(2)
      wrbx41(i1,i2,i3,kd)= rx(i1,i2,i3)*wrbr4(i1,i2,i3,kd)
      wrby41(i1,i2,i3,kd)=0
      wrbz41(i1,i2,i3,kd)=0
      wrbx42(i1,i2,i3,kd)= rx(i1,i2,i3)*wrbr4(i1,i2,i3,kd)+sx(i1,i2,i3)
     & *wrbs4(i1,i2,i3,kd)
      wrby42(i1,i2,i3,kd)= ry(i1,i2,i3)*wrbr4(i1,i2,i3,kd)+sy(i1,i2,i3)
     & *wrbs4(i1,i2,i3,kd)
      wrbz42(i1,i2,i3,kd)=0
      wrbx43(i1,i2,i3,kd)=rx(i1,i2,i3)*wrbr4(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & wrbs4(i1,i2,i3,kd)+tx(i1,i2,i3)*wrbt4(i1,i2,i3,kd)
      wrby43(i1,i2,i3,kd)=ry(i1,i2,i3)*wrbr4(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & wrbs4(i1,i2,i3,kd)+ty(i1,i2,i3)*wrbt4(i1,i2,i3,kd)
      wrbz43(i1,i2,i3,kd)=rz(i1,i2,i3)*wrbr4(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & wrbs4(i1,i2,i3,kd)+tz(i1,i2,i3)*wrbt4(i1,i2,i3,kd)
      wrbxx41(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wrbrr4(i1,i2,i3,kd)+(
     & rxx42(i1,i2,i3))*wrbr4(i1,i2,i3,kd)
      wrbyy41(i1,i2,i3,kd)=0
      wrbxy41(i1,i2,i3,kd)=0
      wrbxz41(i1,i2,i3,kd)=0
      wrbyz41(i1,i2,i3,kd)=0
      wrbzz41(i1,i2,i3,kd)=0
      wrblaplacian41(i1,i2,i3,kd)=wrbxx41(i1,i2,i3,kd)
      wrbxx42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wrbrr4(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*wrbrs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*wrbss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3))*wrbr4(i1,i2,i3,kd)+(
     & sxx42(i1,i2,i3))*wrbs4(i1,i2,i3,kd)
      wrbyy42(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*wrbrr4(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*wrbrs4(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*wrbss4(i1,i2,i3,kd)+(ryy42(i1,i2,i3))*wrbr4(i1,i2,i3,kd)+(
     & syy42(i1,i2,i3))*wrbs4(i1,i2,i3,kd)
      wrbxy42(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wrbrr4(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & wrbrs4(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wrbss4(i1,i2,i3,
     & kd)+rxy42(i1,i2,i3)*wrbr4(i1,i2,i3,kd)+sxy42(i1,i2,i3)*wrbs4(
     & i1,i2,i3,kd)
      wrbxz42(i1,i2,i3,kd)=0
      wrbyz42(i1,i2,i3,kd)=0
      wrbzz42(i1,i2,i3,kd)=0
      wrblaplacian42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & wrbrr4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)
     & *sy(i1,i2,i3))*wrbrs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2)*wrbss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3)+ryy42(i1,i2,i3))*
     & wrbr4(i1,i2,i3,kd)+(sxx42(i1,i2,i3)+syy42(i1,i2,i3))*wrbs4(i1,
     & i2,i3,kd)
      wrbxx43(i1,i2,i3,kd)=rx(i1,i2,i3)**2*wrbrr4(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*wrbss4(i1,i2,i3,kd)+tx(i1,i2,i3)**2*wrbtt4(i1,i2,i3,
     & kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*wrbrs4(i1,i2,i3,kd)+2.*rx(i1,
     & i2,i3)*tx(i1,i2,i3)*wrbrt4(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,
     & i2,i3)*wrbst4(i1,i2,i3,kd)+rxx43(i1,i2,i3)*wrbr4(i1,i2,i3,kd)+
     & sxx43(i1,i2,i3)*wrbs4(i1,i2,i3,kd)+txx43(i1,i2,i3)*wrbt4(i1,i2,
     & i3,kd)
      wrbyy43(i1,i2,i3,kd)=ry(i1,i2,i3)**2*wrbrr4(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*wrbss4(i1,i2,i3,kd)+ty(i1,i2,i3)**2*wrbtt4(i1,i2,i3,
     & kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*wrbrs4(i1,i2,i3,kd)+2.*ry(i1,
     & i2,i3)*ty(i1,i2,i3)*wrbrt4(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,
     & i2,i3)*wrbst4(i1,i2,i3,kd)+ryy43(i1,i2,i3)*wrbr4(i1,i2,i3,kd)+
     & syy43(i1,i2,i3)*wrbs4(i1,i2,i3,kd)+tyy43(i1,i2,i3)*wrbt4(i1,i2,
     & i3,kd)
      wrbzz43(i1,i2,i3,kd)=rz(i1,i2,i3)**2*wrbrr4(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*wrbss4(i1,i2,i3,kd)+tz(i1,i2,i3)**2*wrbtt4(i1,i2,i3,
     & kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*wrbrs4(i1,i2,i3,kd)+2.*rz(i1,
     & i2,i3)*tz(i1,i2,i3)*wrbrt4(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,
     & i2,i3)*wrbst4(i1,i2,i3,kd)+rzz43(i1,i2,i3)*wrbr4(i1,i2,i3,kd)+
     & szz43(i1,i2,i3)*wrbs4(i1,i2,i3,kd)+tzz43(i1,i2,i3)*wrbt4(i1,i2,
     & i3,kd)
      wrbxy43(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wrbrr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wrbss4(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & ty(i1,i2,i3)*wrbtt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(
     & i1,i2,i3)*sx(i1,i2,i3))*wrbrs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(
     & i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*wrbrt4(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*wrbst4(i1,i2,
     & i3,kd)+rxy43(i1,i2,i3)*wrbr4(i1,i2,i3,kd)+sxy43(i1,i2,i3)*
     & wrbs4(i1,i2,i3,kd)+txy43(i1,i2,i3)*wrbt4(i1,i2,i3,kd)
      wrbxz43(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*wrbrr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*wrbss4(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & tz(i1,i2,i3)*wrbtt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sx(i1,i2,i3))*wrbrs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*wrbrt4(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*wrbst4(i1,i2,
     & i3,kd)+rxz43(i1,i2,i3)*wrbr4(i1,i2,i3,kd)+sxz43(i1,i2,i3)*
     & wrbs4(i1,i2,i3,kd)+txz43(i1,i2,i3)*wrbt4(i1,i2,i3,kd)
      wrbyz43(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*wrbrr4(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*wrbss4(i1,i2,i3,kd)+ty(i1,i2,i3)*
     & tz(i1,i2,i3)*wrbtt4(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sy(i1,i2,i3))*wrbrs4(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*wrbrt4(i1,i2,i3,kd)+(sy(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*wrbst4(i1,i2,
     & i3,kd)+ryz43(i1,i2,i3)*wrbr4(i1,i2,i3,kd)+syz43(i1,i2,i3)*
     & wrbs4(i1,i2,i3,kd)+tyz43(i1,i2,i3)*wrbt4(i1,i2,i3,kd)
      wrblaplacian43(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*wrbrr4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)
     & **2+sz(i1,i2,i3)**2)*wrbss4(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(
     & i1,i2,i3)**2+tz(i1,i2,i3)**2)*wrbtt4(i1,i2,i3,kd)+2.*(rx(i1,i2,
     & i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,
     & i2,i3))*wrbrs4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(
     & i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*wrbrt4(i1,i2,
     & i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)
     & +sz(i1,i2,i3)*tz(i1,i2,i3))*wrbst4(i1,i2,i3,kd)+(rxx43(i1,i2,
     & i3)+ryy43(i1,i2,i3)+rzz43(i1,i2,i3))*wrbr4(i1,i2,i3,kd)+(sxx43(
     & i1,i2,i3)+syy43(i1,i2,i3)+szz43(i1,i2,i3))*wrbs4(i1,i2,i3,kd)+(
     & txx43(i1,i2,i3)+tyy43(i1,i2,i3)+tzz43(i1,i2,i3))*wrbt4(i1,i2,
     & i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      wrbx43r(i1,i2,i3,kd)=(8.*(wrb(i1+1,i2,i3,kd)-wrb(i1-1,i2,i3,kd))-
     & (wrb(i1+2,i2,i3,kd)-wrb(i1-2,i2,i3,kd)))*h41(0)
      wrby43r(i1,i2,i3,kd)=(8.*(wrb(i1,i2+1,i3,kd)-wrb(i1,i2-1,i3,kd))-
     & (wrb(i1,i2+2,i3,kd)-wrb(i1,i2-2,i3,kd)))*h41(1)
      wrbz43r(i1,i2,i3,kd)=(8.*(wrb(i1,i2,i3+1,kd)-wrb(i1,i2,i3-1,kd))-
     & (wrb(i1,i2,i3+2,kd)-wrb(i1,i2,i3-2,kd)))*h41(2)
      wrbxx43r(i1,i2,i3,kd)=( -30.*wrb(i1,i2,i3,kd)+16.*(wrb(i1+1,i2,
     & i3,kd)+wrb(i1-1,i2,i3,kd))-(wrb(i1+2,i2,i3,kd)+wrb(i1-2,i2,i3,
     & kd)) )*h42(0)
      wrbyy43r(i1,i2,i3,kd)=( -30.*wrb(i1,i2,i3,kd)+16.*(wrb(i1,i2+1,
     & i3,kd)+wrb(i1,i2-1,i3,kd))-(wrb(i1,i2+2,i3,kd)+wrb(i1,i2-2,i3,
     & kd)) )*h42(1)
      wrbzz43r(i1,i2,i3,kd)=( -30.*wrb(i1,i2,i3,kd)+16.*(wrb(i1,i2,i3+
     & 1,kd)+wrb(i1,i2,i3-1,kd))-(wrb(i1,i2,i3+2,kd)+wrb(i1,i2,i3-2,
     & kd)) )*h42(2)
      wrbxy43r(i1,i2,i3,kd)=( (wrb(i1+2,i2+2,i3,kd)-wrb(i1-2,i2+2,i3,
     & kd)- wrb(i1+2,i2-2,i3,kd)+wrb(i1-2,i2-2,i3,kd)) +8.*(wrb(i1-1,
     & i2+2,i3,kd)-wrb(i1-1,i2-2,i3,kd)-wrb(i1+1,i2+2,i3,kd)+wrb(i1+1,
     & i2-2,i3,kd) +wrb(i1+2,i2-1,i3,kd)-wrb(i1-2,i2-1,i3,kd)-wrb(i1+
     & 2,i2+1,i3,kd)+wrb(i1-2,i2+1,i3,kd))+64.*(wrb(i1+1,i2+1,i3,kd)-
     & wrb(i1-1,i2+1,i3,kd)- wrb(i1+1,i2-1,i3,kd)+wrb(i1-1,i2-1,i3,kd)
     & ))*(h41(0)*h41(1))
      wrbxz43r(i1,i2,i3,kd)=( (wrb(i1+2,i2,i3+2,kd)-wrb(i1-2,i2,i3+2,
     & kd)-wrb(i1+2,i2,i3-2,kd)+wrb(i1-2,i2,i3-2,kd)) +8.*(wrb(i1-1,
     & i2,i3+2,kd)-wrb(i1-1,i2,i3-2,kd)-wrb(i1+1,i2,i3+2,kd)+wrb(i1+1,
     & i2,i3-2,kd) +wrb(i1+2,i2,i3-1,kd)-wrb(i1-2,i2,i3-1,kd)- wrb(i1+
     & 2,i2,i3+1,kd)+wrb(i1-2,i2,i3+1,kd)) +64.*(wrb(i1+1,i2,i3+1,kd)-
     & wrb(i1-1,i2,i3+1,kd)-wrb(i1+1,i2,i3-1,kd)+wrb(i1-1,i2,i3-1,kd))
     &  )*(h41(0)*h41(2))
      wrbyz43r(i1,i2,i3,kd)=( (wrb(i1,i2+2,i3+2,kd)-wrb(i1,i2-2,i3+2,
     & kd)-wrb(i1,i2+2,i3-2,kd)+wrb(i1,i2-2,i3-2,kd)) +8.*(wrb(i1,i2-
     & 1,i3+2,kd)-wrb(i1,i2-1,i3-2,kd)-wrb(i1,i2+1,i3+2,kd)+wrb(i1,i2+
     & 1,i3-2,kd) +wrb(i1,i2+2,i3-1,kd)-wrb(i1,i2-2,i3-1,kd)-wrb(i1,
     & i2+2,i3+1,kd)+wrb(i1,i2-2,i3+1,kd)) +64.*(wrb(i1,i2+1,i3+1,kd)-
     & wrb(i1,i2-1,i3+1,kd)-wrb(i1,i2+1,i3-1,kd)+wrb(i1,i2-1,i3-1,kd))
     &  )*(h41(1)*h41(2))
      wrbx41r(i1,i2,i3,kd)= wrbx43r(i1,i2,i3,kd)
      wrby41r(i1,i2,i3,kd)= wrby43r(i1,i2,i3,kd)
      wrbz41r(i1,i2,i3,kd)= wrbz43r(i1,i2,i3,kd)
      wrbxx41r(i1,i2,i3,kd)= wrbxx43r(i1,i2,i3,kd)
      wrbyy41r(i1,i2,i3,kd)= wrbyy43r(i1,i2,i3,kd)
      wrbzz41r(i1,i2,i3,kd)= wrbzz43r(i1,i2,i3,kd)
      wrbxy41r(i1,i2,i3,kd)= wrbxy43r(i1,i2,i3,kd)
      wrbxz41r(i1,i2,i3,kd)= wrbxz43r(i1,i2,i3,kd)
      wrbyz41r(i1,i2,i3,kd)= wrbyz43r(i1,i2,i3,kd)
      wrblaplacian41r(i1,i2,i3,kd)=wrbxx43r(i1,i2,i3,kd)
      wrbx42r(i1,i2,i3,kd)= wrbx43r(i1,i2,i3,kd)
      wrby42r(i1,i2,i3,kd)= wrby43r(i1,i2,i3,kd)
      wrbz42r(i1,i2,i3,kd)= wrbz43r(i1,i2,i3,kd)
      wrbxx42r(i1,i2,i3,kd)= wrbxx43r(i1,i2,i3,kd)
      wrbyy42r(i1,i2,i3,kd)= wrbyy43r(i1,i2,i3,kd)
      wrbzz42r(i1,i2,i3,kd)= wrbzz43r(i1,i2,i3,kd)
      wrbxy42r(i1,i2,i3,kd)= wrbxy43r(i1,i2,i3,kd)
      wrbxz42r(i1,i2,i3,kd)= wrbxz43r(i1,i2,i3,kd)
      wrbyz42r(i1,i2,i3,kd)= wrbyz43r(i1,i2,i3,kd)
      wrblaplacian42r(i1,i2,i3,kd)=wrbxx43r(i1,i2,i3,kd)+wrbyy43r(i1,
     & i2,i3,kd)
      wrblaplacian43r(i1,i2,i3,kd)=wrbxx43r(i1,i2,i3,kd)+wrbyy43r(i1,
     & i2,i3,kd)+wrbzz43r(i1,i2,i3,kd)
      wrbmr4(i1,i2,i3,kd)=(8.*(wrbm(i1+1,i2,i3,kd)-wrbm(i1-1,i2,i3,kd))
     & -(wrbm(i1+2,i2,i3,kd)-wrbm(i1-2,i2,i3,kd)))*d14(0)
      wrbms4(i1,i2,i3,kd)=(8.*(wrbm(i1,i2+1,i3,kd)-wrbm(i1,i2-1,i3,kd))
     & -(wrbm(i1,i2+2,i3,kd)-wrbm(i1,i2-2,i3,kd)))*d14(1)
      wrbmt4(i1,i2,i3,kd)=(8.*(wrbm(i1,i2,i3+1,kd)-wrbm(i1,i2,i3-1,kd))
     & -(wrbm(i1,i2,i3+2,kd)-wrbm(i1,i2,i3-2,kd)))*d14(2)
      wrbmrr4(i1,i2,i3,kd)=(-30.*wrbm(i1,i2,i3,kd)+16.*(wrbm(i1+1,i2,
     & i3,kd)+wrbm(i1-1,i2,i3,kd))-(wrbm(i1+2,i2,i3,kd)+wrbm(i1-2,i2,
     & i3,kd)) )*d24(0)
      wrbmss4(i1,i2,i3,kd)=(-30.*wrbm(i1,i2,i3,kd)+16.*(wrbm(i1,i2+1,
     & i3,kd)+wrbm(i1,i2-1,i3,kd))-(wrbm(i1,i2+2,i3,kd)+wrbm(i1,i2-2,
     & i3,kd)) )*d24(1)
      wrbmtt4(i1,i2,i3,kd)=(-30.*wrbm(i1,i2,i3,kd)+16.*(wrbm(i1,i2,i3+
     & 1,kd)+wrbm(i1,i2,i3-1,kd))-(wrbm(i1,i2,i3+2,kd)+wrbm(i1,i2,i3-
     & 2,kd)) )*d24(2)
      wrbmrs4(i1,i2,i3,kd)=(8.*(wrbmr4(i1,i2+1,i3,kd)-wrbmr4(i1,i2-1,
     & i3,kd))-(wrbmr4(i1,i2+2,i3,kd)-wrbmr4(i1,i2-2,i3,kd)))*d14(1)
      wrbmrt4(i1,i2,i3,kd)=(8.*(wrbmr4(i1,i2,i3+1,kd)-wrbmr4(i1,i2,i3-
     & 1,kd))-(wrbmr4(i1,i2,i3+2,kd)-wrbmr4(i1,i2,i3-2,kd)))*d14(2)
      wrbmst4(i1,i2,i3,kd)=(8.*(wrbms4(i1,i2,i3+1,kd)-wrbms4(i1,i2,i3-
     & 1,kd))-(wrbms4(i1,i2,i3+2,kd)-wrbms4(i1,i2,i3-2,kd)))*d14(2)
      wrbmx41(i1,i2,i3,kd)= rx(i1,i2,i3)*wrbmr4(i1,i2,i3,kd)
      wrbmy41(i1,i2,i3,kd)=0
      wrbmz41(i1,i2,i3,kd)=0
      wrbmx42(i1,i2,i3,kd)= rx(i1,i2,i3)*wrbmr4(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*wrbms4(i1,i2,i3,kd)
      wrbmy42(i1,i2,i3,kd)= ry(i1,i2,i3)*wrbmr4(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*wrbms4(i1,i2,i3,kd)
      wrbmz42(i1,i2,i3,kd)=0
      wrbmx43(i1,i2,i3,kd)=rx(i1,i2,i3)*wrbmr4(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*wrbms4(i1,i2,i3,kd)+tx(i1,i2,i3)*wrbmt4(i1,i2,i3,kd)
      wrbmy43(i1,i2,i3,kd)=ry(i1,i2,i3)*wrbmr4(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*wrbms4(i1,i2,i3,kd)+ty(i1,i2,i3)*wrbmt4(i1,i2,i3,kd)
      wrbmz43(i1,i2,i3,kd)=rz(i1,i2,i3)*wrbmr4(i1,i2,i3,kd)+sz(i1,i2,
     & i3)*wrbms4(i1,i2,i3,kd)+tz(i1,i2,i3)*wrbmt4(i1,i2,i3,kd)
      wrbmxx41(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wrbmrr4(i1,i2,i3,kd)+(
     & rxx42(i1,i2,i3))*wrbmr4(i1,i2,i3,kd)
      wrbmyy41(i1,i2,i3,kd)=0
      wrbmxy41(i1,i2,i3,kd)=0
      wrbmxz41(i1,i2,i3,kd)=0
      wrbmyz41(i1,i2,i3,kd)=0
      wrbmzz41(i1,i2,i3,kd)=0
      wrbmlaplacian41(i1,i2,i3,kd)=wrbmxx41(i1,i2,i3,kd)
      wrbmxx42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wrbmrr4(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*wrbmrs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*wrbmss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3))*wrbmr4(i1,i2,i3,kd)+(
     & sxx42(i1,i2,i3))*wrbms4(i1,i2,i3,kd)
      wrbmyy42(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*wrbmrr4(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*wrbmrs4(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*wrbmss4(i1,i2,i3,kd)+(ryy42(i1,i2,i3))*wrbmr4(i1,i2,i3,kd)+(
     & syy42(i1,i2,i3))*wrbms4(i1,i2,i3,kd)
      wrbmxy42(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wrbmrr4(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & wrbmrs4(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wrbmss4(i1,i2,
     & i3,kd)+rxy42(i1,i2,i3)*wrbmr4(i1,i2,i3,kd)+sxy42(i1,i2,i3)*
     & wrbms4(i1,i2,i3,kd)
      wrbmxz42(i1,i2,i3,kd)=0
      wrbmyz42(i1,i2,i3,kd)=0
      wrbmzz42(i1,i2,i3,kd)=0
      wrbmlaplacian42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & wrbmrr4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,
     & i3)*sy(i1,i2,i3))*wrbmrs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,
     & i2,i3)**2)*wrbmss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3)+ryy42(i1,i2,
     & i3))*wrbmr4(i1,i2,i3,kd)+(sxx42(i1,i2,i3)+syy42(i1,i2,i3))*
     & wrbms4(i1,i2,i3,kd)
      wrbmxx43(i1,i2,i3,kd)=rx(i1,i2,i3)**2*wrbmrr4(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*wrbmss4(i1,i2,i3,kd)+tx(i1,i2,i3)**2*wrbmtt4(i1,i2,
     & i3,kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*wrbmrs4(i1,i2,i3,kd)+2.*rx(
     & i1,i2,i3)*tx(i1,i2,i3)*wrbmrt4(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(
     & i1,i2,i3)*wrbmst4(i1,i2,i3,kd)+rxx43(i1,i2,i3)*wrbmr4(i1,i2,i3,
     & kd)+sxx43(i1,i2,i3)*wrbms4(i1,i2,i3,kd)+txx43(i1,i2,i3)*wrbmt4(
     & i1,i2,i3,kd)
      wrbmyy43(i1,i2,i3,kd)=ry(i1,i2,i3)**2*wrbmrr4(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*wrbmss4(i1,i2,i3,kd)+ty(i1,i2,i3)**2*wrbmtt4(i1,i2,
     & i3,kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*wrbmrs4(i1,i2,i3,kd)+2.*ry(
     & i1,i2,i3)*ty(i1,i2,i3)*wrbmrt4(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(
     & i1,i2,i3)*wrbmst4(i1,i2,i3,kd)+ryy43(i1,i2,i3)*wrbmr4(i1,i2,i3,
     & kd)+syy43(i1,i2,i3)*wrbms4(i1,i2,i3,kd)+tyy43(i1,i2,i3)*wrbmt4(
     & i1,i2,i3,kd)
      wrbmzz43(i1,i2,i3,kd)=rz(i1,i2,i3)**2*wrbmrr4(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*wrbmss4(i1,i2,i3,kd)+tz(i1,i2,i3)**2*wrbmtt4(i1,i2,
     & i3,kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*wrbmrs4(i1,i2,i3,kd)+2.*rz(
     & i1,i2,i3)*tz(i1,i2,i3)*wrbmrt4(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(
     & i1,i2,i3)*wrbmst4(i1,i2,i3,kd)+rzz43(i1,i2,i3)*wrbmr4(i1,i2,i3,
     & kd)+szz43(i1,i2,i3)*wrbms4(i1,i2,i3,kd)+tzz43(i1,i2,i3)*wrbmt4(
     & i1,i2,i3,kd)
      wrbmxy43(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wrbmrr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wrbmss4(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *ty(i1,i2,i3)*wrbmtt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+
     & ry(i1,i2,i3)*sx(i1,i2,i3))*wrbmrs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & ty(i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*wrbmrt4(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*wrbmst4(
     & i1,i2,i3,kd)+rxy43(i1,i2,i3)*wrbmr4(i1,i2,i3,kd)+sxy43(i1,i2,
     & i3)*wrbms4(i1,i2,i3,kd)+txy43(i1,i2,i3)*wrbmt4(i1,i2,i3,kd)
      wrbmxz43(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*wrbmrr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*wrbmss4(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *tz(i1,i2,i3)*wrbmtt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sx(i1,i2,i3))*wrbmrs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*wrbmrt4(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*wrbmst4(
     & i1,i2,i3,kd)+rxz43(i1,i2,i3)*wrbmr4(i1,i2,i3,kd)+sxz43(i1,i2,
     & i3)*wrbms4(i1,i2,i3,kd)+txz43(i1,i2,i3)*wrbmt4(i1,i2,i3,kd)
      wrbmyz43(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*wrbmrr4(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*wrbmss4(i1,i2,i3,kd)+ty(i1,i2,i3)
     & *tz(i1,i2,i3)*wrbmtt4(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sy(i1,i2,i3))*wrbmrs4(i1,i2,i3,kd)+(ry(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*wrbmrt4(i1,i2,i3,kd)+(
     & sy(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*wrbmst4(
     & i1,i2,i3,kd)+ryz43(i1,i2,i3)*wrbmr4(i1,i2,i3,kd)+syz43(i1,i2,
     & i3)*wrbms4(i1,i2,i3,kd)+tyz43(i1,i2,i3)*wrbmt4(i1,i2,i3,kd)
      wrbmlaplacian43(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*wrbmrr4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2+sz(i1,i2,i3)**2)*wrbmss4(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+
     & ty(i1,i2,i3)**2+tz(i1,i2,i3)**2)*wrbmtt4(i1,i2,i3,kd)+2.*(rx(
     & i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*
     & sz(i1,i2,i3))*wrbmrs4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,
     & i3)+ ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*
     & wrbmrt4(i1,i2,i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,
     & i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3))*wrbmst4(i1,i2,i3,
     & kd)+(rxx43(i1,i2,i3)+ryy43(i1,i2,i3)+rzz43(i1,i2,i3))*wrbmr4(
     & i1,i2,i3,kd)+(sxx43(i1,i2,i3)+syy43(i1,i2,i3)+szz43(i1,i2,i3))*
     & wrbms4(i1,i2,i3,kd)+(txx43(i1,i2,i3)+tyy43(i1,i2,i3)+tzz43(i1,
     & i2,i3))*wrbmt4(i1,i2,i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      wrbmx43r(i1,i2,i3,kd)=(8.*(wrbm(i1+1,i2,i3,kd)-wrbm(i1-1,i2,i3,
     & kd))-(wrbm(i1+2,i2,i3,kd)-wrbm(i1-2,i2,i3,kd)))*h41(0)
      wrbmy43r(i1,i2,i3,kd)=(8.*(wrbm(i1,i2+1,i3,kd)-wrbm(i1,i2-1,i3,
     & kd))-(wrbm(i1,i2+2,i3,kd)-wrbm(i1,i2-2,i3,kd)))*h41(1)
      wrbmz43r(i1,i2,i3,kd)=(8.*(wrbm(i1,i2,i3+1,kd)-wrbm(i1,i2,i3-1,
     & kd))-(wrbm(i1,i2,i3+2,kd)-wrbm(i1,i2,i3-2,kd)))*h41(2)
      wrbmxx43r(i1,i2,i3,kd)=( -30.*wrbm(i1,i2,i3,kd)+16.*(wrbm(i1+1,
     & i2,i3,kd)+wrbm(i1-1,i2,i3,kd))-(wrbm(i1+2,i2,i3,kd)+wrbm(i1-2,
     & i2,i3,kd)) )*h42(0)
      wrbmyy43r(i1,i2,i3,kd)=( -30.*wrbm(i1,i2,i3,kd)+16.*(wrbm(i1,i2+
     & 1,i3,kd)+wrbm(i1,i2-1,i3,kd))-(wrbm(i1,i2+2,i3,kd)+wrbm(i1,i2-
     & 2,i3,kd)) )*h42(1)
      wrbmzz43r(i1,i2,i3,kd)=( -30.*wrbm(i1,i2,i3,kd)+16.*(wrbm(i1,i2,
     & i3+1,kd)+wrbm(i1,i2,i3-1,kd))-(wrbm(i1,i2,i3+2,kd)+wrbm(i1,i2,
     & i3-2,kd)) )*h42(2)
      wrbmxy43r(i1,i2,i3,kd)=( (wrbm(i1+2,i2+2,i3,kd)-wrbm(i1-2,i2+2,
     & i3,kd)- wrbm(i1+2,i2-2,i3,kd)+wrbm(i1-2,i2-2,i3,kd)) +8.*(wrbm(
     & i1-1,i2+2,i3,kd)-wrbm(i1-1,i2-2,i3,kd)-wrbm(i1+1,i2+2,i3,kd)+
     & wrbm(i1+1,i2-2,i3,kd) +wrbm(i1+2,i2-1,i3,kd)-wrbm(i1-2,i2-1,i3,
     & kd)-wrbm(i1+2,i2+1,i3,kd)+wrbm(i1-2,i2+1,i3,kd))+64.*(wrbm(i1+
     & 1,i2+1,i3,kd)-wrbm(i1-1,i2+1,i3,kd)- wrbm(i1+1,i2-1,i3,kd)+
     & wrbm(i1-1,i2-1,i3,kd)))*(h41(0)*h41(1))
      wrbmxz43r(i1,i2,i3,kd)=( (wrbm(i1+2,i2,i3+2,kd)-wrbm(i1-2,i2,i3+
     & 2,kd)-wrbm(i1+2,i2,i3-2,kd)+wrbm(i1-2,i2,i3-2,kd)) +8.*(wrbm(
     & i1-1,i2,i3+2,kd)-wrbm(i1-1,i2,i3-2,kd)-wrbm(i1+1,i2,i3+2,kd)+
     & wrbm(i1+1,i2,i3-2,kd) +wrbm(i1+2,i2,i3-1,kd)-wrbm(i1-2,i2,i3-1,
     & kd)- wrbm(i1+2,i2,i3+1,kd)+wrbm(i1-2,i2,i3+1,kd)) +64.*(wrbm(
     & i1+1,i2,i3+1,kd)-wrbm(i1-1,i2,i3+1,kd)-wrbm(i1+1,i2,i3-1,kd)+
     & wrbm(i1-1,i2,i3-1,kd)) )*(h41(0)*h41(2))
      wrbmyz43r(i1,i2,i3,kd)=( (wrbm(i1,i2+2,i3+2,kd)-wrbm(i1,i2-2,i3+
     & 2,kd)-wrbm(i1,i2+2,i3-2,kd)+wrbm(i1,i2-2,i3-2,kd)) +8.*(wrbm(
     & i1,i2-1,i3+2,kd)-wrbm(i1,i2-1,i3-2,kd)-wrbm(i1,i2+1,i3+2,kd)+
     & wrbm(i1,i2+1,i3-2,kd) +wrbm(i1,i2+2,i3-1,kd)-wrbm(i1,i2-2,i3-1,
     & kd)-wrbm(i1,i2+2,i3+1,kd)+wrbm(i1,i2-2,i3+1,kd)) +64.*(wrbm(i1,
     & i2+1,i3+1,kd)-wrbm(i1,i2-1,i3+1,kd)-wrbm(i1,i2+1,i3-1,kd)+wrbm(
     & i1,i2-1,i3-1,kd)) )*(h41(1)*h41(2))
      wrbmx41r(i1,i2,i3,kd)= wrbmx43r(i1,i2,i3,kd)
      wrbmy41r(i1,i2,i3,kd)= wrbmy43r(i1,i2,i3,kd)
      wrbmz41r(i1,i2,i3,kd)= wrbmz43r(i1,i2,i3,kd)
      wrbmxx41r(i1,i2,i3,kd)= wrbmxx43r(i1,i2,i3,kd)
      wrbmyy41r(i1,i2,i3,kd)= wrbmyy43r(i1,i2,i3,kd)
      wrbmzz41r(i1,i2,i3,kd)= wrbmzz43r(i1,i2,i3,kd)
      wrbmxy41r(i1,i2,i3,kd)= wrbmxy43r(i1,i2,i3,kd)
      wrbmxz41r(i1,i2,i3,kd)= wrbmxz43r(i1,i2,i3,kd)
      wrbmyz41r(i1,i2,i3,kd)= wrbmyz43r(i1,i2,i3,kd)
      wrbmlaplacian41r(i1,i2,i3,kd)=wrbmxx43r(i1,i2,i3,kd)
      wrbmx42r(i1,i2,i3,kd)= wrbmx43r(i1,i2,i3,kd)
      wrbmy42r(i1,i2,i3,kd)= wrbmy43r(i1,i2,i3,kd)
      wrbmz42r(i1,i2,i3,kd)= wrbmz43r(i1,i2,i3,kd)
      wrbmxx42r(i1,i2,i3,kd)= wrbmxx43r(i1,i2,i3,kd)
      wrbmyy42r(i1,i2,i3,kd)= wrbmyy43r(i1,i2,i3,kd)
      wrbmzz42r(i1,i2,i3,kd)= wrbmzz43r(i1,i2,i3,kd)
      wrbmxy42r(i1,i2,i3,kd)= wrbmxy43r(i1,i2,i3,kd)
      wrbmxz42r(i1,i2,i3,kd)= wrbmxz43r(i1,i2,i3,kd)
      wrbmyz42r(i1,i2,i3,kd)= wrbmyz43r(i1,i2,i3,kd)
      wrbmlaplacian42r(i1,i2,i3,kd)=wrbmxx43r(i1,i2,i3,kd)+wrbmyy43r(
     & i1,i2,i3,kd)
      wrbmlaplacian43r(i1,i2,i3,kd)=wrbmxx43r(i1,i2,i3,kd)+wrbmyy43r(
     & i1,i2,i3,kd)+wrbmzz43r(i1,i2,i3,kd)

      vsar4(i1,i2,i3,kd)=(8.*(vsa(i1+1,i2,i3,kd)-vsa(i1-1,i2,i3,kd))-(
     & vsa(i1+2,i2,i3,kd)-vsa(i1-2,i2,i3,kd)))*d14(0)
      vsas4(i1,i2,i3,kd)=(8.*(vsa(i1,i2+1,i3,kd)-vsa(i1,i2-1,i3,kd))-(
     & vsa(i1,i2+2,i3,kd)-vsa(i1,i2-2,i3,kd)))*d14(1)
      vsat4(i1,i2,i3,kd)=(8.*(vsa(i1,i2,i3+1,kd)-vsa(i1,i2,i3-1,kd))-(
     & vsa(i1,i2,i3+2,kd)-vsa(i1,i2,i3-2,kd)))*d14(2)
      vsarr4(i1,i2,i3,kd)=(-30.*vsa(i1,i2,i3,kd)+16.*(vsa(i1+1,i2,i3,
     & kd)+vsa(i1-1,i2,i3,kd))-(vsa(i1+2,i2,i3,kd)+vsa(i1-2,i2,i3,kd))
     &  )*d24(0)
      vsass4(i1,i2,i3,kd)=(-30.*vsa(i1,i2,i3,kd)+16.*(vsa(i1,i2+1,i3,
     & kd)+vsa(i1,i2-1,i3,kd))-(vsa(i1,i2+2,i3,kd)+vsa(i1,i2-2,i3,kd))
     &  )*d24(1)
      vsatt4(i1,i2,i3,kd)=(-30.*vsa(i1,i2,i3,kd)+16.*(vsa(i1,i2,i3+1,
     & kd)+vsa(i1,i2,i3-1,kd))-(vsa(i1,i2,i3+2,kd)+vsa(i1,i2,i3-2,kd))
     &  )*d24(2)
      vsars4(i1,i2,i3,kd)=(8.*(vsar4(i1,i2+1,i3,kd)-vsar4(i1,i2-1,i3,
     & kd))-(vsar4(i1,i2+2,i3,kd)-vsar4(i1,i2-2,i3,kd)))*d14(1)
      vsart4(i1,i2,i3,kd)=(8.*(vsar4(i1,i2,i3+1,kd)-vsar4(i1,i2,i3-1,
     & kd))-(vsar4(i1,i2,i3+2,kd)-vsar4(i1,i2,i3-2,kd)))*d14(2)
      vsast4(i1,i2,i3,kd)=(8.*(vsas4(i1,i2,i3+1,kd)-vsas4(i1,i2,i3-1,
     & kd))-(vsas4(i1,i2,i3+2,kd)-vsas4(i1,i2,i3-2,kd)))*d14(2)
      vsax41(i1,i2,i3,kd)= rx(i1,i2,i3)*vsar4(i1,i2,i3,kd)
      vsay41(i1,i2,i3,kd)=0
      vsaz41(i1,i2,i3,kd)=0
      vsax42(i1,i2,i3,kd)= rx(i1,i2,i3)*vsar4(i1,i2,i3,kd)+sx(i1,i2,i3)
     & *vsas4(i1,i2,i3,kd)
      vsay42(i1,i2,i3,kd)= ry(i1,i2,i3)*vsar4(i1,i2,i3,kd)+sy(i1,i2,i3)
     & *vsas4(i1,i2,i3,kd)
      vsaz42(i1,i2,i3,kd)=0
      vsax43(i1,i2,i3,kd)=rx(i1,i2,i3)*vsar4(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & vsas4(i1,i2,i3,kd)+tx(i1,i2,i3)*vsat4(i1,i2,i3,kd)
      vsay43(i1,i2,i3,kd)=ry(i1,i2,i3)*vsar4(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & vsas4(i1,i2,i3,kd)+ty(i1,i2,i3)*vsat4(i1,i2,i3,kd)
      vsaz43(i1,i2,i3,kd)=rz(i1,i2,i3)*vsar4(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & vsas4(i1,i2,i3,kd)+tz(i1,i2,i3)*vsat4(i1,i2,i3,kd)
      vsaxx41(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vsarr4(i1,i2,i3,kd)+(
     & rxx42(i1,i2,i3))*vsar4(i1,i2,i3,kd)
      vsayy41(i1,i2,i3,kd)=0
      vsaxy41(i1,i2,i3,kd)=0
      vsaxz41(i1,i2,i3,kd)=0
      vsayz41(i1,i2,i3,kd)=0
      vsazz41(i1,i2,i3,kd)=0
      vsalaplacian41(i1,i2,i3,kd)=vsaxx41(i1,i2,i3,kd)
      vsaxx42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vsarr4(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*vsars4(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*vsass4(i1,i2,i3,kd)+(rxx42(i1,i2,i3))*vsar4(i1,i2,i3,kd)+(
     & sxx42(i1,i2,i3))*vsas4(i1,i2,i3,kd)
      vsayy42(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*vsarr4(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*vsars4(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*vsass4(i1,i2,i3,kd)+(ryy42(i1,i2,i3))*vsar4(i1,i2,i3,kd)+(
     & syy42(i1,i2,i3))*vsas4(i1,i2,i3,kd)
      vsaxy42(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vsarr4(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & vsars4(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vsass4(i1,i2,i3,
     & kd)+rxy42(i1,i2,i3)*vsar4(i1,i2,i3,kd)+sxy42(i1,i2,i3)*vsas4(
     & i1,i2,i3,kd)
      vsaxz42(i1,i2,i3,kd)=0
      vsayz42(i1,i2,i3,kd)=0
      vsazz42(i1,i2,i3,kd)=0
      vsalaplacian42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & vsarr4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)
     & *sy(i1,i2,i3))*vsars4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2)*vsass4(i1,i2,i3,kd)+(rxx42(i1,i2,i3)+ryy42(i1,i2,i3))*
     & vsar4(i1,i2,i3,kd)+(sxx42(i1,i2,i3)+syy42(i1,i2,i3))*vsas4(i1,
     & i2,i3,kd)
      vsaxx43(i1,i2,i3,kd)=rx(i1,i2,i3)**2*vsarr4(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*vsass4(i1,i2,i3,kd)+tx(i1,i2,i3)**2*vsatt4(i1,i2,i3,
     & kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*vsars4(i1,i2,i3,kd)+2.*rx(i1,
     & i2,i3)*tx(i1,i2,i3)*vsart4(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,
     & i2,i3)*vsast4(i1,i2,i3,kd)+rxx43(i1,i2,i3)*vsar4(i1,i2,i3,kd)+
     & sxx43(i1,i2,i3)*vsas4(i1,i2,i3,kd)+txx43(i1,i2,i3)*vsat4(i1,i2,
     & i3,kd)
      vsayy43(i1,i2,i3,kd)=ry(i1,i2,i3)**2*vsarr4(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*vsass4(i1,i2,i3,kd)+ty(i1,i2,i3)**2*vsatt4(i1,i2,i3,
     & kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*vsars4(i1,i2,i3,kd)+2.*ry(i1,
     & i2,i3)*ty(i1,i2,i3)*vsart4(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,
     & i2,i3)*vsast4(i1,i2,i3,kd)+ryy43(i1,i2,i3)*vsar4(i1,i2,i3,kd)+
     & syy43(i1,i2,i3)*vsas4(i1,i2,i3,kd)+tyy43(i1,i2,i3)*vsat4(i1,i2,
     & i3,kd)
      vsazz43(i1,i2,i3,kd)=rz(i1,i2,i3)**2*vsarr4(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*vsass4(i1,i2,i3,kd)+tz(i1,i2,i3)**2*vsatt4(i1,i2,i3,
     & kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*vsars4(i1,i2,i3,kd)+2.*rz(i1,
     & i2,i3)*tz(i1,i2,i3)*vsart4(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,
     & i2,i3)*vsast4(i1,i2,i3,kd)+rzz43(i1,i2,i3)*vsar4(i1,i2,i3,kd)+
     & szz43(i1,i2,i3)*vsas4(i1,i2,i3,kd)+tzz43(i1,i2,i3)*vsat4(i1,i2,
     & i3,kd)
      vsaxy43(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vsarr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vsass4(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & ty(i1,i2,i3)*vsatt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(
     & i1,i2,i3)*sx(i1,i2,i3))*vsars4(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(
     & i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*vsart4(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*vsast4(i1,i2,
     & i3,kd)+rxy43(i1,i2,i3)*vsar4(i1,i2,i3,kd)+sxy43(i1,i2,i3)*
     & vsas4(i1,i2,i3,kd)+txy43(i1,i2,i3)*vsat4(i1,i2,i3,kd)
      vsaxz43(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*vsarr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*vsass4(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & tz(i1,i2,i3)*vsatt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sx(i1,i2,i3))*vsars4(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*vsart4(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*vsast4(i1,i2,
     & i3,kd)+rxz43(i1,i2,i3)*vsar4(i1,i2,i3,kd)+sxz43(i1,i2,i3)*
     & vsas4(i1,i2,i3,kd)+txz43(i1,i2,i3)*vsat4(i1,i2,i3,kd)
      vsayz43(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*vsarr4(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*vsass4(i1,i2,i3,kd)+ty(i1,i2,i3)*
     & tz(i1,i2,i3)*vsatt4(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sy(i1,i2,i3))*vsars4(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*vsart4(i1,i2,i3,kd)+(sy(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*vsast4(i1,i2,
     & i3,kd)+ryz43(i1,i2,i3)*vsar4(i1,i2,i3,kd)+syz43(i1,i2,i3)*
     & vsas4(i1,i2,i3,kd)+tyz43(i1,i2,i3)*vsat4(i1,i2,i3,kd)
      vsalaplacian43(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*vsarr4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)
     & **2+sz(i1,i2,i3)**2)*vsass4(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(
     & i1,i2,i3)**2+tz(i1,i2,i3)**2)*vsatt4(i1,i2,i3,kd)+2.*(rx(i1,i2,
     & i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,
     & i2,i3))*vsars4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(
     & i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*vsart4(i1,i2,
     & i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)
     & +sz(i1,i2,i3)*tz(i1,i2,i3))*vsast4(i1,i2,i3,kd)+(rxx43(i1,i2,
     & i3)+ryy43(i1,i2,i3)+rzz43(i1,i2,i3))*vsar4(i1,i2,i3,kd)+(sxx43(
     & i1,i2,i3)+syy43(i1,i2,i3)+szz43(i1,i2,i3))*vsas4(i1,i2,i3,kd)+(
     & txx43(i1,i2,i3)+tyy43(i1,i2,i3)+tzz43(i1,i2,i3))*vsat4(i1,i2,
     & i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      vsax43r(i1,i2,i3,kd)=(8.*(vsa(i1+1,i2,i3,kd)-vsa(i1-1,i2,i3,kd))-
     & (vsa(i1+2,i2,i3,kd)-vsa(i1-2,i2,i3,kd)))*h41(0)
      vsay43r(i1,i2,i3,kd)=(8.*(vsa(i1,i2+1,i3,kd)-vsa(i1,i2-1,i3,kd))-
     & (vsa(i1,i2+2,i3,kd)-vsa(i1,i2-2,i3,kd)))*h41(1)
      vsaz43r(i1,i2,i3,kd)=(8.*(vsa(i1,i2,i3+1,kd)-vsa(i1,i2,i3-1,kd))-
     & (vsa(i1,i2,i3+2,kd)-vsa(i1,i2,i3-2,kd)))*h41(2)
      vsaxx43r(i1,i2,i3,kd)=( -30.*vsa(i1,i2,i3,kd)+16.*(vsa(i1+1,i2,
     & i3,kd)+vsa(i1-1,i2,i3,kd))-(vsa(i1+2,i2,i3,kd)+vsa(i1-2,i2,i3,
     & kd)) )*h42(0)
      vsayy43r(i1,i2,i3,kd)=( -30.*vsa(i1,i2,i3,kd)+16.*(vsa(i1,i2+1,
     & i3,kd)+vsa(i1,i2-1,i3,kd))-(vsa(i1,i2+2,i3,kd)+vsa(i1,i2-2,i3,
     & kd)) )*h42(1)
      vsazz43r(i1,i2,i3,kd)=( -30.*vsa(i1,i2,i3,kd)+16.*(vsa(i1,i2,i3+
     & 1,kd)+vsa(i1,i2,i3-1,kd))-(vsa(i1,i2,i3+2,kd)+vsa(i1,i2,i3-2,
     & kd)) )*h42(2)
      vsaxy43r(i1,i2,i3,kd)=( (vsa(i1+2,i2+2,i3,kd)-vsa(i1-2,i2+2,i3,
     & kd)- vsa(i1+2,i2-2,i3,kd)+vsa(i1-2,i2-2,i3,kd)) +8.*(vsa(i1-1,
     & i2+2,i3,kd)-vsa(i1-1,i2-2,i3,kd)-vsa(i1+1,i2+2,i3,kd)+vsa(i1+1,
     & i2-2,i3,kd) +vsa(i1+2,i2-1,i3,kd)-vsa(i1-2,i2-1,i3,kd)-vsa(i1+
     & 2,i2+1,i3,kd)+vsa(i1-2,i2+1,i3,kd))+64.*(vsa(i1+1,i2+1,i3,kd)-
     & vsa(i1-1,i2+1,i3,kd)- vsa(i1+1,i2-1,i3,kd)+vsa(i1-1,i2-1,i3,kd)
     & ))*(h41(0)*h41(1))
      vsaxz43r(i1,i2,i3,kd)=( (vsa(i1+2,i2,i3+2,kd)-vsa(i1-2,i2,i3+2,
     & kd)-vsa(i1+2,i2,i3-2,kd)+vsa(i1-2,i2,i3-2,kd)) +8.*(vsa(i1-1,
     & i2,i3+2,kd)-vsa(i1-1,i2,i3-2,kd)-vsa(i1+1,i2,i3+2,kd)+vsa(i1+1,
     & i2,i3-2,kd) +vsa(i1+2,i2,i3-1,kd)-vsa(i1-2,i2,i3-1,kd)- vsa(i1+
     & 2,i2,i3+1,kd)+vsa(i1-2,i2,i3+1,kd)) +64.*(vsa(i1+1,i2,i3+1,kd)-
     & vsa(i1-1,i2,i3+1,kd)-vsa(i1+1,i2,i3-1,kd)+vsa(i1-1,i2,i3-1,kd))
     &  )*(h41(0)*h41(2))
      vsayz43r(i1,i2,i3,kd)=( (vsa(i1,i2+2,i3+2,kd)-vsa(i1,i2-2,i3+2,
     & kd)-vsa(i1,i2+2,i3-2,kd)+vsa(i1,i2-2,i3-2,kd)) +8.*(vsa(i1,i2-
     & 1,i3+2,kd)-vsa(i1,i2-1,i3-2,kd)-vsa(i1,i2+1,i3+2,kd)+vsa(i1,i2+
     & 1,i3-2,kd) +vsa(i1,i2+2,i3-1,kd)-vsa(i1,i2-2,i3-1,kd)-vsa(i1,
     & i2+2,i3+1,kd)+vsa(i1,i2-2,i3+1,kd)) +64.*(vsa(i1,i2+1,i3+1,kd)-
     & vsa(i1,i2-1,i3+1,kd)-vsa(i1,i2+1,i3-1,kd)+vsa(i1,i2-1,i3-1,kd))
     &  )*(h41(1)*h41(2))
      vsax41r(i1,i2,i3,kd)= vsax43r(i1,i2,i3,kd)
      vsay41r(i1,i2,i3,kd)= vsay43r(i1,i2,i3,kd)
      vsaz41r(i1,i2,i3,kd)= vsaz43r(i1,i2,i3,kd)
      vsaxx41r(i1,i2,i3,kd)= vsaxx43r(i1,i2,i3,kd)
      vsayy41r(i1,i2,i3,kd)= vsayy43r(i1,i2,i3,kd)
      vsazz41r(i1,i2,i3,kd)= vsazz43r(i1,i2,i3,kd)
      vsaxy41r(i1,i2,i3,kd)= vsaxy43r(i1,i2,i3,kd)
      vsaxz41r(i1,i2,i3,kd)= vsaxz43r(i1,i2,i3,kd)
      vsayz41r(i1,i2,i3,kd)= vsayz43r(i1,i2,i3,kd)
      vsalaplacian41r(i1,i2,i3,kd)=vsaxx43r(i1,i2,i3,kd)
      vsax42r(i1,i2,i3,kd)= vsax43r(i1,i2,i3,kd)
      vsay42r(i1,i2,i3,kd)= vsay43r(i1,i2,i3,kd)
      vsaz42r(i1,i2,i3,kd)= vsaz43r(i1,i2,i3,kd)
      vsaxx42r(i1,i2,i3,kd)= vsaxx43r(i1,i2,i3,kd)
      vsayy42r(i1,i2,i3,kd)= vsayy43r(i1,i2,i3,kd)
      vsazz42r(i1,i2,i3,kd)= vsazz43r(i1,i2,i3,kd)
      vsaxy42r(i1,i2,i3,kd)= vsaxy43r(i1,i2,i3,kd)
      vsaxz42r(i1,i2,i3,kd)= vsaxz43r(i1,i2,i3,kd)
      vsayz42r(i1,i2,i3,kd)= vsayz43r(i1,i2,i3,kd)
      vsalaplacian42r(i1,i2,i3,kd)=vsaxx43r(i1,i2,i3,kd)+vsayy43r(i1,
     & i2,i3,kd)
      vsalaplacian43r(i1,i2,i3,kd)=vsaxx43r(i1,i2,i3,kd)+vsayy43r(i1,
     & i2,i3,kd)+vsazz43r(i1,i2,i3,kd)
      vsamr4(i1,i2,i3,kd)=(8.*(vsam(i1+1,i2,i3,kd)-vsam(i1-1,i2,i3,kd))
     & -(vsam(i1+2,i2,i3,kd)-vsam(i1-2,i2,i3,kd)))*d14(0)
      vsams4(i1,i2,i3,kd)=(8.*(vsam(i1,i2+1,i3,kd)-vsam(i1,i2-1,i3,kd))
     & -(vsam(i1,i2+2,i3,kd)-vsam(i1,i2-2,i3,kd)))*d14(1)
      vsamt4(i1,i2,i3,kd)=(8.*(vsam(i1,i2,i3+1,kd)-vsam(i1,i2,i3-1,kd))
     & -(vsam(i1,i2,i3+2,kd)-vsam(i1,i2,i3-2,kd)))*d14(2)
      vsamrr4(i1,i2,i3,kd)=(-30.*vsam(i1,i2,i3,kd)+16.*(vsam(i1+1,i2,
     & i3,kd)+vsam(i1-1,i2,i3,kd))-(vsam(i1+2,i2,i3,kd)+vsam(i1-2,i2,
     & i3,kd)) )*d24(0)
      vsamss4(i1,i2,i3,kd)=(-30.*vsam(i1,i2,i3,kd)+16.*(vsam(i1,i2+1,
     & i3,kd)+vsam(i1,i2-1,i3,kd))-(vsam(i1,i2+2,i3,kd)+vsam(i1,i2-2,
     & i3,kd)) )*d24(1)
      vsamtt4(i1,i2,i3,kd)=(-30.*vsam(i1,i2,i3,kd)+16.*(vsam(i1,i2,i3+
     & 1,kd)+vsam(i1,i2,i3-1,kd))-(vsam(i1,i2,i3+2,kd)+vsam(i1,i2,i3-
     & 2,kd)) )*d24(2)
      vsamrs4(i1,i2,i3,kd)=(8.*(vsamr4(i1,i2+1,i3,kd)-vsamr4(i1,i2-1,
     & i3,kd))-(vsamr4(i1,i2+2,i3,kd)-vsamr4(i1,i2-2,i3,kd)))*d14(1)
      vsamrt4(i1,i2,i3,kd)=(8.*(vsamr4(i1,i2,i3+1,kd)-vsamr4(i1,i2,i3-
     & 1,kd))-(vsamr4(i1,i2,i3+2,kd)-vsamr4(i1,i2,i3-2,kd)))*d14(2)
      vsamst4(i1,i2,i3,kd)=(8.*(vsams4(i1,i2,i3+1,kd)-vsams4(i1,i2,i3-
     & 1,kd))-(vsams4(i1,i2,i3+2,kd)-vsams4(i1,i2,i3-2,kd)))*d14(2)
      vsamx41(i1,i2,i3,kd)= rx(i1,i2,i3)*vsamr4(i1,i2,i3,kd)
      vsamy41(i1,i2,i3,kd)=0
      vsamz41(i1,i2,i3,kd)=0
      vsamx42(i1,i2,i3,kd)= rx(i1,i2,i3)*vsamr4(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*vsams4(i1,i2,i3,kd)
      vsamy42(i1,i2,i3,kd)= ry(i1,i2,i3)*vsamr4(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*vsams4(i1,i2,i3,kd)
      vsamz42(i1,i2,i3,kd)=0
      vsamx43(i1,i2,i3,kd)=rx(i1,i2,i3)*vsamr4(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*vsams4(i1,i2,i3,kd)+tx(i1,i2,i3)*vsamt4(i1,i2,i3,kd)
      vsamy43(i1,i2,i3,kd)=ry(i1,i2,i3)*vsamr4(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*vsams4(i1,i2,i3,kd)+ty(i1,i2,i3)*vsamt4(i1,i2,i3,kd)
      vsamz43(i1,i2,i3,kd)=rz(i1,i2,i3)*vsamr4(i1,i2,i3,kd)+sz(i1,i2,
     & i3)*vsams4(i1,i2,i3,kd)+tz(i1,i2,i3)*vsamt4(i1,i2,i3,kd)
      vsamxx41(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vsamrr4(i1,i2,i3,kd)+(
     & rxx42(i1,i2,i3))*vsamr4(i1,i2,i3,kd)
      vsamyy41(i1,i2,i3,kd)=0
      vsamxy41(i1,i2,i3,kd)=0
      vsamxz41(i1,i2,i3,kd)=0
      vsamyz41(i1,i2,i3,kd)=0
      vsamzz41(i1,i2,i3,kd)=0
      vsamlaplacian41(i1,i2,i3,kd)=vsamxx41(i1,i2,i3,kd)
      vsamxx42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vsamrr4(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*vsamrs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*vsamss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3))*vsamr4(i1,i2,i3,kd)+(
     & sxx42(i1,i2,i3))*vsams4(i1,i2,i3,kd)
      vsamyy42(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*vsamrr4(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*vsamrs4(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*vsamss4(i1,i2,i3,kd)+(ryy42(i1,i2,i3))*vsamr4(i1,i2,i3,kd)+(
     & syy42(i1,i2,i3))*vsams4(i1,i2,i3,kd)
      vsamxy42(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vsamrr4(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & vsamrs4(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vsamss4(i1,i2,
     & i3,kd)+rxy42(i1,i2,i3)*vsamr4(i1,i2,i3,kd)+sxy42(i1,i2,i3)*
     & vsams4(i1,i2,i3,kd)
      vsamxz42(i1,i2,i3,kd)=0
      vsamyz42(i1,i2,i3,kd)=0
      vsamzz42(i1,i2,i3,kd)=0
      vsamlaplacian42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & vsamrr4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,
     & i3)*sy(i1,i2,i3))*vsamrs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,
     & i2,i3)**2)*vsamss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3)+ryy42(i1,i2,
     & i3))*vsamr4(i1,i2,i3,kd)+(sxx42(i1,i2,i3)+syy42(i1,i2,i3))*
     & vsams4(i1,i2,i3,kd)
      vsamxx43(i1,i2,i3,kd)=rx(i1,i2,i3)**2*vsamrr4(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*vsamss4(i1,i2,i3,kd)+tx(i1,i2,i3)**2*vsamtt4(i1,i2,
     & i3,kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*vsamrs4(i1,i2,i3,kd)+2.*rx(
     & i1,i2,i3)*tx(i1,i2,i3)*vsamrt4(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(
     & i1,i2,i3)*vsamst4(i1,i2,i3,kd)+rxx43(i1,i2,i3)*vsamr4(i1,i2,i3,
     & kd)+sxx43(i1,i2,i3)*vsams4(i1,i2,i3,kd)+txx43(i1,i2,i3)*vsamt4(
     & i1,i2,i3,kd)
      vsamyy43(i1,i2,i3,kd)=ry(i1,i2,i3)**2*vsamrr4(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*vsamss4(i1,i2,i3,kd)+ty(i1,i2,i3)**2*vsamtt4(i1,i2,
     & i3,kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*vsamrs4(i1,i2,i3,kd)+2.*ry(
     & i1,i2,i3)*ty(i1,i2,i3)*vsamrt4(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(
     & i1,i2,i3)*vsamst4(i1,i2,i3,kd)+ryy43(i1,i2,i3)*vsamr4(i1,i2,i3,
     & kd)+syy43(i1,i2,i3)*vsams4(i1,i2,i3,kd)+tyy43(i1,i2,i3)*vsamt4(
     & i1,i2,i3,kd)
      vsamzz43(i1,i2,i3,kd)=rz(i1,i2,i3)**2*vsamrr4(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*vsamss4(i1,i2,i3,kd)+tz(i1,i2,i3)**2*vsamtt4(i1,i2,
     & i3,kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*vsamrs4(i1,i2,i3,kd)+2.*rz(
     & i1,i2,i3)*tz(i1,i2,i3)*vsamrt4(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(
     & i1,i2,i3)*vsamst4(i1,i2,i3,kd)+rzz43(i1,i2,i3)*vsamr4(i1,i2,i3,
     & kd)+szz43(i1,i2,i3)*vsams4(i1,i2,i3,kd)+tzz43(i1,i2,i3)*vsamt4(
     & i1,i2,i3,kd)
      vsamxy43(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vsamrr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vsamss4(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *ty(i1,i2,i3)*vsamtt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+
     & ry(i1,i2,i3)*sx(i1,i2,i3))*vsamrs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & ty(i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*vsamrt4(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*vsamst4(
     & i1,i2,i3,kd)+rxy43(i1,i2,i3)*vsamr4(i1,i2,i3,kd)+sxy43(i1,i2,
     & i3)*vsams4(i1,i2,i3,kd)+txy43(i1,i2,i3)*vsamt4(i1,i2,i3,kd)
      vsamxz43(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*vsamrr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*vsamss4(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *tz(i1,i2,i3)*vsamtt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sx(i1,i2,i3))*vsamrs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*vsamrt4(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*vsamst4(
     & i1,i2,i3,kd)+rxz43(i1,i2,i3)*vsamr4(i1,i2,i3,kd)+sxz43(i1,i2,
     & i3)*vsams4(i1,i2,i3,kd)+txz43(i1,i2,i3)*vsamt4(i1,i2,i3,kd)
      vsamyz43(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*vsamrr4(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*vsamss4(i1,i2,i3,kd)+ty(i1,i2,i3)
     & *tz(i1,i2,i3)*vsamtt4(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sy(i1,i2,i3))*vsamrs4(i1,i2,i3,kd)+(ry(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*vsamrt4(i1,i2,i3,kd)+(
     & sy(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*vsamst4(
     & i1,i2,i3,kd)+ryz43(i1,i2,i3)*vsamr4(i1,i2,i3,kd)+syz43(i1,i2,
     & i3)*vsams4(i1,i2,i3,kd)+tyz43(i1,i2,i3)*vsamt4(i1,i2,i3,kd)
      vsamlaplacian43(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*vsamrr4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2+sz(i1,i2,i3)**2)*vsamss4(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+
     & ty(i1,i2,i3)**2+tz(i1,i2,i3)**2)*vsamtt4(i1,i2,i3,kd)+2.*(rx(
     & i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*
     & sz(i1,i2,i3))*vsamrs4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,
     & i3)+ ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*
     & vsamrt4(i1,i2,i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,
     & i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3))*vsamst4(i1,i2,i3,
     & kd)+(rxx43(i1,i2,i3)+ryy43(i1,i2,i3)+rzz43(i1,i2,i3))*vsamr4(
     & i1,i2,i3,kd)+(sxx43(i1,i2,i3)+syy43(i1,i2,i3)+szz43(i1,i2,i3))*
     & vsams4(i1,i2,i3,kd)+(txx43(i1,i2,i3)+tyy43(i1,i2,i3)+tzz43(i1,
     & i2,i3))*vsamt4(i1,i2,i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      vsamx43r(i1,i2,i3,kd)=(8.*(vsam(i1+1,i2,i3,kd)-vsam(i1-1,i2,i3,
     & kd))-(vsam(i1+2,i2,i3,kd)-vsam(i1-2,i2,i3,kd)))*h41(0)
      vsamy43r(i1,i2,i3,kd)=(8.*(vsam(i1,i2+1,i3,kd)-vsam(i1,i2-1,i3,
     & kd))-(vsam(i1,i2+2,i3,kd)-vsam(i1,i2-2,i3,kd)))*h41(1)
      vsamz43r(i1,i2,i3,kd)=(8.*(vsam(i1,i2,i3+1,kd)-vsam(i1,i2,i3-1,
     & kd))-(vsam(i1,i2,i3+2,kd)-vsam(i1,i2,i3-2,kd)))*h41(2)
      vsamxx43r(i1,i2,i3,kd)=( -30.*vsam(i1,i2,i3,kd)+16.*(vsam(i1+1,
     & i2,i3,kd)+vsam(i1-1,i2,i3,kd))-(vsam(i1+2,i2,i3,kd)+vsam(i1-2,
     & i2,i3,kd)) )*h42(0)
      vsamyy43r(i1,i2,i3,kd)=( -30.*vsam(i1,i2,i3,kd)+16.*(vsam(i1,i2+
     & 1,i3,kd)+vsam(i1,i2-1,i3,kd))-(vsam(i1,i2+2,i3,kd)+vsam(i1,i2-
     & 2,i3,kd)) )*h42(1)
      vsamzz43r(i1,i2,i3,kd)=( -30.*vsam(i1,i2,i3,kd)+16.*(vsam(i1,i2,
     & i3+1,kd)+vsam(i1,i2,i3-1,kd))-(vsam(i1,i2,i3+2,kd)+vsam(i1,i2,
     & i3-2,kd)) )*h42(2)
      vsamxy43r(i1,i2,i3,kd)=( (vsam(i1+2,i2+2,i3,kd)-vsam(i1-2,i2+2,
     & i3,kd)- vsam(i1+2,i2-2,i3,kd)+vsam(i1-2,i2-2,i3,kd)) +8.*(vsam(
     & i1-1,i2+2,i3,kd)-vsam(i1-1,i2-2,i3,kd)-vsam(i1+1,i2+2,i3,kd)+
     & vsam(i1+1,i2-2,i3,kd) +vsam(i1+2,i2-1,i3,kd)-vsam(i1-2,i2-1,i3,
     & kd)-vsam(i1+2,i2+1,i3,kd)+vsam(i1-2,i2+1,i3,kd))+64.*(vsam(i1+
     & 1,i2+1,i3,kd)-vsam(i1-1,i2+1,i3,kd)- vsam(i1+1,i2-1,i3,kd)+
     & vsam(i1-1,i2-1,i3,kd)))*(h41(0)*h41(1))
      vsamxz43r(i1,i2,i3,kd)=( (vsam(i1+2,i2,i3+2,kd)-vsam(i1-2,i2,i3+
     & 2,kd)-vsam(i1+2,i2,i3-2,kd)+vsam(i1-2,i2,i3-2,kd)) +8.*(vsam(
     & i1-1,i2,i3+2,kd)-vsam(i1-1,i2,i3-2,kd)-vsam(i1+1,i2,i3+2,kd)+
     & vsam(i1+1,i2,i3-2,kd) +vsam(i1+2,i2,i3-1,kd)-vsam(i1-2,i2,i3-1,
     & kd)- vsam(i1+2,i2,i3+1,kd)+vsam(i1-2,i2,i3+1,kd)) +64.*(vsam(
     & i1+1,i2,i3+1,kd)-vsam(i1-1,i2,i3+1,kd)-vsam(i1+1,i2,i3-1,kd)+
     & vsam(i1-1,i2,i3-1,kd)) )*(h41(0)*h41(2))
      vsamyz43r(i1,i2,i3,kd)=( (vsam(i1,i2+2,i3+2,kd)-vsam(i1,i2-2,i3+
     & 2,kd)-vsam(i1,i2+2,i3-2,kd)+vsam(i1,i2-2,i3-2,kd)) +8.*(vsam(
     & i1,i2-1,i3+2,kd)-vsam(i1,i2-1,i3-2,kd)-vsam(i1,i2+1,i3+2,kd)+
     & vsam(i1,i2+1,i3-2,kd) +vsam(i1,i2+2,i3-1,kd)-vsam(i1,i2-2,i3-1,
     & kd)-vsam(i1,i2+2,i3+1,kd)+vsam(i1,i2-2,i3+1,kd)) +64.*(vsam(i1,
     & i2+1,i3+1,kd)-vsam(i1,i2-1,i3+1,kd)-vsam(i1,i2+1,i3-1,kd)+vsam(
     & i1,i2-1,i3-1,kd)) )*(h41(1)*h41(2))
      vsamx41r(i1,i2,i3,kd)= vsamx43r(i1,i2,i3,kd)
      vsamy41r(i1,i2,i3,kd)= vsamy43r(i1,i2,i3,kd)
      vsamz41r(i1,i2,i3,kd)= vsamz43r(i1,i2,i3,kd)
      vsamxx41r(i1,i2,i3,kd)= vsamxx43r(i1,i2,i3,kd)
      vsamyy41r(i1,i2,i3,kd)= vsamyy43r(i1,i2,i3,kd)
      vsamzz41r(i1,i2,i3,kd)= vsamzz43r(i1,i2,i3,kd)
      vsamxy41r(i1,i2,i3,kd)= vsamxy43r(i1,i2,i3,kd)
      vsamxz41r(i1,i2,i3,kd)= vsamxz43r(i1,i2,i3,kd)
      vsamyz41r(i1,i2,i3,kd)= vsamyz43r(i1,i2,i3,kd)
      vsamlaplacian41r(i1,i2,i3,kd)=vsamxx43r(i1,i2,i3,kd)
      vsamx42r(i1,i2,i3,kd)= vsamx43r(i1,i2,i3,kd)
      vsamy42r(i1,i2,i3,kd)= vsamy43r(i1,i2,i3,kd)
      vsamz42r(i1,i2,i3,kd)= vsamz43r(i1,i2,i3,kd)
      vsamxx42r(i1,i2,i3,kd)= vsamxx43r(i1,i2,i3,kd)
      vsamyy42r(i1,i2,i3,kd)= vsamyy43r(i1,i2,i3,kd)
      vsamzz42r(i1,i2,i3,kd)= vsamzz43r(i1,i2,i3,kd)
      vsamxy42r(i1,i2,i3,kd)= vsamxy43r(i1,i2,i3,kd)
      vsamxz42r(i1,i2,i3,kd)= vsamxz43r(i1,i2,i3,kd)
      vsamyz42r(i1,i2,i3,kd)= vsamyz43r(i1,i2,i3,kd)
      vsamlaplacian42r(i1,i2,i3,kd)=vsamxx43r(i1,i2,i3,kd)+vsamyy43r(
     & i1,i2,i3,kd)
      vsamlaplacian43r(i1,i2,i3,kd)=vsamxx43r(i1,i2,i3,kd)+vsamyy43r(
     & i1,i2,i3,kd)+vsamzz43r(i1,i2,i3,kd)
      wsar4(i1,i2,i3,kd)=(8.*(wsa(i1+1,i2,i3,kd)-wsa(i1-1,i2,i3,kd))-(
     & wsa(i1+2,i2,i3,kd)-wsa(i1-2,i2,i3,kd)))*d14(0)
      wsas4(i1,i2,i3,kd)=(8.*(wsa(i1,i2+1,i3,kd)-wsa(i1,i2-1,i3,kd))-(
     & wsa(i1,i2+2,i3,kd)-wsa(i1,i2-2,i3,kd)))*d14(1)
      wsat4(i1,i2,i3,kd)=(8.*(wsa(i1,i2,i3+1,kd)-wsa(i1,i2,i3-1,kd))-(
     & wsa(i1,i2,i3+2,kd)-wsa(i1,i2,i3-2,kd)))*d14(2)
      wsarr4(i1,i2,i3,kd)=(-30.*wsa(i1,i2,i3,kd)+16.*(wsa(i1+1,i2,i3,
     & kd)+wsa(i1-1,i2,i3,kd))-(wsa(i1+2,i2,i3,kd)+wsa(i1-2,i2,i3,kd))
     &  )*d24(0)
      wsass4(i1,i2,i3,kd)=(-30.*wsa(i1,i2,i3,kd)+16.*(wsa(i1,i2+1,i3,
     & kd)+wsa(i1,i2-1,i3,kd))-(wsa(i1,i2+2,i3,kd)+wsa(i1,i2-2,i3,kd))
     &  )*d24(1)
      wsatt4(i1,i2,i3,kd)=(-30.*wsa(i1,i2,i3,kd)+16.*(wsa(i1,i2,i3+1,
     & kd)+wsa(i1,i2,i3-1,kd))-(wsa(i1,i2,i3+2,kd)+wsa(i1,i2,i3-2,kd))
     &  )*d24(2)
      wsars4(i1,i2,i3,kd)=(8.*(wsar4(i1,i2+1,i3,kd)-wsar4(i1,i2-1,i3,
     & kd))-(wsar4(i1,i2+2,i3,kd)-wsar4(i1,i2-2,i3,kd)))*d14(1)
      wsart4(i1,i2,i3,kd)=(8.*(wsar4(i1,i2,i3+1,kd)-wsar4(i1,i2,i3-1,
     & kd))-(wsar4(i1,i2,i3+2,kd)-wsar4(i1,i2,i3-2,kd)))*d14(2)
      wsast4(i1,i2,i3,kd)=(8.*(wsas4(i1,i2,i3+1,kd)-wsas4(i1,i2,i3-1,
     & kd))-(wsas4(i1,i2,i3+2,kd)-wsas4(i1,i2,i3-2,kd)))*d14(2)
      wsax41(i1,i2,i3,kd)= rx(i1,i2,i3)*wsar4(i1,i2,i3,kd)
      wsay41(i1,i2,i3,kd)=0
      wsaz41(i1,i2,i3,kd)=0
      wsax42(i1,i2,i3,kd)= rx(i1,i2,i3)*wsar4(i1,i2,i3,kd)+sx(i1,i2,i3)
     & *wsas4(i1,i2,i3,kd)
      wsay42(i1,i2,i3,kd)= ry(i1,i2,i3)*wsar4(i1,i2,i3,kd)+sy(i1,i2,i3)
     & *wsas4(i1,i2,i3,kd)
      wsaz42(i1,i2,i3,kd)=0
      wsax43(i1,i2,i3,kd)=rx(i1,i2,i3)*wsar4(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & wsas4(i1,i2,i3,kd)+tx(i1,i2,i3)*wsat4(i1,i2,i3,kd)
      wsay43(i1,i2,i3,kd)=ry(i1,i2,i3)*wsar4(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & wsas4(i1,i2,i3,kd)+ty(i1,i2,i3)*wsat4(i1,i2,i3,kd)
      wsaz43(i1,i2,i3,kd)=rz(i1,i2,i3)*wsar4(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & wsas4(i1,i2,i3,kd)+tz(i1,i2,i3)*wsat4(i1,i2,i3,kd)
      wsaxx41(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wsarr4(i1,i2,i3,kd)+(
     & rxx42(i1,i2,i3))*wsar4(i1,i2,i3,kd)
      wsayy41(i1,i2,i3,kd)=0
      wsaxy41(i1,i2,i3,kd)=0
      wsaxz41(i1,i2,i3,kd)=0
      wsayz41(i1,i2,i3,kd)=0
      wsazz41(i1,i2,i3,kd)=0
      wsalaplacian41(i1,i2,i3,kd)=wsaxx41(i1,i2,i3,kd)
      wsaxx42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wsarr4(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*wsars4(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*wsass4(i1,i2,i3,kd)+(rxx42(i1,i2,i3))*wsar4(i1,i2,i3,kd)+(
     & sxx42(i1,i2,i3))*wsas4(i1,i2,i3,kd)
      wsayy42(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*wsarr4(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*wsars4(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*wsass4(i1,i2,i3,kd)+(ryy42(i1,i2,i3))*wsar4(i1,i2,i3,kd)+(
     & syy42(i1,i2,i3))*wsas4(i1,i2,i3,kd)
      wsaxy42(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wsarr4(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & wsars4(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wsass4(i1,i2,i3,
     & kd)+rxy42(i1,i2,i3)*wsar4(i1,i2,i3,kd)+sxy42(i1,i2,i3)*wsas4(
     & i1,i2,i3,kd)
      wsaxz42(i1,i2,i3,kd)=0
      wsayz42(i1,i2,i3,kd)=0
      wsazz42(i1,i2,i3,kd)=0
      wsalaplacian42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & wsarr4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)
     & *sy(i1,i2,i3))*wsars4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2)*wsass4(i1,i2,i3,kd)+(rxx42(i1,i2,i3)+ryy42(i1,i2,i3))*
     & wsar4(i1,i2,i3,kd)+(sxx42(i1,i2,i3)+syy42(i1,i2,i3))*wsas4(i1,
     & i2,i3,kd)
      wsaxx43(i1,i2,i3,kd)=rx(i1,i2,i3)**2*wsarr4(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*wsass4(i1,i2,i3,kd)+tx(i1,i2,i3)**2*wsatt4(i1,i2,i3,
     & kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*wsars4(i1,i2,i3,kd)+2.*rx(i1,
     & i2,i3)*tx(i1,i2,i3)*wsart4(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,
     & i2,i3)*wsast4(i1,i2,i3,kd)+rxx43(i1,i2,i3)*wsar4(i1,i2,i3,kd)+
     & sxx43(i1,i2,i3)*wsas4(i1,i2,i3,kd)+txx43(i1,i2,i3)*wsat4(i1,i2,
     & i3,kd)
      wsayy43(i1,i2,i3,kd)=ry(i1,i2,i3)**2*wsarr4(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*wsass4(i1,i2,i3,kd)+ty(i1,i2,i3)**2*wsatt4(i1,i2,i3,
     & kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*wsars4(i1,i2,i3,kd)+2.*ry(i1,
     & i2,i3)*ty(i1,i2,i3)*wsart4(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,
     & i2,i3)*wsast4(i1,i2,i3,kd)+ryy43(i1,i2,i3)*wsar4(i1,i2,i3,kd)+
     & syy43(i1,i2,i3)*wsas4(i1,i2,i3,kd)+tyy43(i1,i2,i3)*wsat4(i1,i2,
     & i3,kd)
      wsazz43(i1,i2,i3,kd)=rz(i1,i2,i3)**2*wsarr4(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*wsass4(i1,i2,i3,kd)+tz(i1,i2,i3)**2*wsatt4(i1,i2,i3,
     & kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*wsars4(i1,i2,i3,kd)+2.*rz(i1,
     & i2,i3)*tz(i1,i2,i3)*wsart4(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,
     & i2,i3)*wsast4(i1,i2,i3,kd)+rzz43(i1,i2,i3)*wsar4(i1,i2,i3,kd)+
     & szz43(i1,i2,i3)*wsas4(i1,i2,i3,kd)+tzz43(i1,i2,i3)*wsat4(i1,i2,
     & i3,kd)
      wsaxy43(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wsarr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wsass4(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & ty(i1,i2,i3)*wsatt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(
     & i1,i2,i3)*sx(i1,i2,i3))*wsars4(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(
     & i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*wsart4(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*wsast4(i1,i2,
     & i3,kd)+rxy43(i1,i2,i3)*wsar4(i1,i2,i3,kd)+sxy43(i1,i2,i3)*
     & wsas4(i1,i2,i3,kd)+txy43(i1,i2,i3)*wsat4(i1,i2,i3,kd)
      wsaxz43(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*wsarr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*wsass4(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & tz(i1,i2,i3)*wsatt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sx(i1,i2,i3))*wsars4(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*wsart4(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*wsast4(i1,i2,
     & i3,kd)+rxz43(i1,i2,i3)*wsar4(i1,i2,i3,kd)+sxz43(i1,i2,i3)*
     & wsas4(i1,i2,i3,kd)+txz43(i1,i2,i3)*wsat4(i1,i2,i3,kd)
      wsayz43(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*wsarr4(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*wsass4(i1,i2,i3,kd)+ty(i1,i2,i3)*
     & tz(i1,i2,i3)*wsatt4(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sy(i1,i2,i3))*wsars4(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*wsart4(i1,i2,i3,kd)+(sy(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*wsast4(i1,i2,
     & i3,kd)+ryz43(i1,i2,i3)*wsar4(i1,i2,i3,kd)+syz43(i1,i2,i3)*
     & wsas4(i1,i2,i3,kd)+tyz43(i1,i2,i3)*wsat4(i1,i2,i3,kd)
      wsalaplacian43(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*wsarr4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)
     & **2+sz(i1,i2,i3)**2)*wsass4(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(
     & i1,i2,i3)**2+tz(i1,i2,i3)**2)*wsatt4(i1,i2,i3,kd)+2.*(rx(i1,i2,
     & i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,
     & i2,i3))*wsars4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(
     & i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*wsart4(i1,i2,
     & i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)
     & +sz(i1,i2,i3)*tz(i1,i2,i3))*wsast4(i1,i2,i3,kd)+(rxx43(i1,i2,
     & i3)+ryy43(i1,i2,i3)+rzz43(i1,i2,i3))*wsar4(i1,i2,i3,kd)+(sxx43(
     & i1,i2,i3)+syy43(i1,i2,i3)+szz43(i1,i2,i3))*wsas4(i1,i2,i3,kd)+(
     & txx43(i1,i2,i3)+tyy43(i1,i2,i3)+tzz43(i1,i2,i3))*wsat4(i1,i2,
     & i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      wsax43r(i1,i2,i3,kd)=(8.*(wsa(i1+1,i2,i3,kd)-wsa(i1-1,i2,i3,kd))-
     & (wsa(i1+2,i2,i3,kd)-wsa(i1-2,i2,i3,kd)))*h41(0)
      wsay43r(i1,i2,i3,kd)=(8.*(wsa(i1,i2+1,i3,kd)-wsa(i1,i2-1,i3,kd))-
     & (wsa(i1,i2+2,i3,kd)-wsa(i1,i2-2,i3,kd)))*h41(1)
      wsaz43r(i1,i2,i3,kd)=(8.*(wsa(i1,i2,i3+1,kd)-wsa(i1,i2,i3-1,kd))-
     & (wsa(i1,i2,i3+2,kd)-wsa(i1,i2,i3-2,kd)))*h41(2)
      wsaxx43r(i1,i2,i3,kd)=( -30.*wsa(i1,i2,i3,kd)+16.*(wsa(i1+1,i2,
     & i3,kd)+wsa(i1-1,i2,i3,kd))-(wsa(i1+2,i2,i3,kd)+wsa(i1-2,i2,i3,
     & kd)) )*h42(0)
      wsayy43r(i1,i2,i3,kd)=( -30.*wsa(i1,i2,i3,kd)+16.*(wsa(i1,i2+1,
     & i3,kd)+wsa(i1,i2-1,i3,kd))-(wsa(i1,i2+2,i3,kd)+wsa(i1,i2-2,i3,
     & kd)) )*h42(1)
      wsazz43r(i1,i2,i3,kd)=( -30.*wsa(i1,i2,i3,kd)+16.*(wsa(i1,i2,i3+
     & 1,kd)+wsa(i1,i2,i3-1,kd))-(wsa(i1,i2,i3+2,kd)+wsa(i1,i2,i3-2,
     & kd)) )*h42(2)
      wsaxy43r(i1,i2,i3,kd)=( (wsa(i1+2,i2+2,i3,kd)-wsa(i1-2,i2+2,i3,
     & kd)- wsa(i1+2,i2-2,i3,kd)+wsa(i1-2,i2-2,i3,kd)) +8.*(wsa(i1-1,
     & i2+2,i3,kd)-wsa(i1-1,i2-2,i3,kd)-wsa(i1+1,i2+2,i3,kd)+wsa(i1+1,
     & i2-2,i3,kd) +wsa(i1+2,i2-1,i3,kd)-wsa(i1-2,i2-1,i3,kd)-wsa(i1+
     & 2,i2+1,i3,kd)+wsa(i1-2,i2+1,i3,kd))+64.*(wsa(i1+1,i2+1,i3,kd)-
     & wsa(i1-1,i2+1,i3,kd)- wsa(i1+1,i2-1,i3,kd)+wsa(i1-1,i2-1,i3,kd)
     & ))*(h41(0)*h41(1))
      wsaxz43r(i1,i2,i3,kd)=( (wsa(i1+2,i2,i3+2,kd)-wsa(i1-2,i2,i3+2,
     & kd)-wsa(i1+2,i2,i3-2,kd)+wsa(i1-2,i2,i3-2,kd)) +8.*(wsa(i1-1,
     & i2,i3+2,kd)-wsa(i1-1,i2,i3-2,kd)-wsa(i1+1,i2,i3+2,kd)+wsa(i1+1,
     & i2,i3-2,kd) +wsa(i1+2,i2,i3-1,kd)-wsa(i1-2,i2,i3-1,kd)- wsa(i1+
     & 2,i2,i3+1,kd)+wsa(i1-2,i2,i3+1,kd)) +64.*(wsa(i1+1,i2,i3+1,kd)-
     & wsa(i1-1,i2,i3+1,kd)-wsa(i1+1,i2,i3-1,kd)+wsa(i1-1,i2,i3-1,kd))
     &  )*(h41(0)*h41(2))
      wsayz43r(i1,i2,i3,kd)=( (wsa(i1,i2+2,i3+2,kd)-wsa(i1,i2-2,i3+2,
     & kd)-wsa(i1,i2+2,i3-2,kd)+wsa(i1,i2-2,i3-2,kd)) +8.*(wsa(i1,i2-
     & 1,i3+2,kd)-wsa(i1,i2-1,i3-2,kd)-wsa(i1,i2+1,i3+2,kd)+wsa(i1,i2+
     & 1,i3-2,kd) +wsa(i1,i2+2,i3-1,kd)-wsa(i1,i2-2,i3-1,kd)-wsa(i1,
     & i2+2,i3+1,kd)+wsa(i1,i2-2,i3+1,kd)) +64.*(wsa(i1,i2+1,i3+1,kd)-
     & wsa(i1,i2-1,i3+1,kd)-wsa(i1,i2+1,i3-1,kd)+wsa(i1,i2-1,i3-1,kd))
     &  )*(h41(1)*h41(2))
      wsax41r(i1,i2,i3,kd)= wsax43r(i1,i2,i3,kd)
      wsay41r(i1,i2,i3,kd)= wsay43r(i1,i2,i3,kd)
      wsaz41r(i1,i2,i3,kd)= wsaz43r(i1,i2,i3,kd)
      wsaxx41r(i1,i2,i3,kd)= wsaxx43r(i1,i2,i3,kd)
      wsayy41r(i1,i2,i3,kd)= wsayy43r(i1,i2,i3,kd)
      wsazz41r(i1,i2,i3,kd)= wsazz43r(i1,i2,i3,kd)
      wsaxy41r(i1,i2,i3,kd)= wsaxy43r(i1,i2,i3,kd)
      wsaxz41r(i1,i2,i3,kd)= wsaxz43r(i1,i2,i3,kd)
      wsayz41r(i1,i2,i3,kd)= wsayz43r(i1,i2,i3,kd)
      wsalaplacian41r(i1,i2,i3,kd)=wsaxx43r(i1,i2,i3,kd)
      wsax42r(i1,i2,i3,kd)= wsax43r(i1,i2,i3,kd)
      wsay42r(i1,i2,i3,kd)= wsay43r(i1,i2,i3,kd)
      wsaz42r(i1,i2,i3,kd)= wsaz43r(i1,i2,i3,kd)
      wsaxx42r(i1,i2,i3,kd)= wsaxx43r(i1,i2,i3,kd)
      wsayy42r(i1,i2,i3,kd)= wsayy43r(i1,i2,i3,kd)
      wsazz42r(i1,i2,i3,kd)= wsazz43r(i1,i2,i3,kd)
      wsaxy42r(i1,i2,i3,kd)= wsaxy43r(i1,i2,i3,kd)
      wsaxz42r(i1,i2,i3,kd)= wsaxz43r(i1,i2,i3,kd)
      wsayz42r(i1,i2,i3,kd)= wsayz43r(i1,i2,i3,kd)
      wsalaplacian42r(i1,i2,i3,kd)=wsaxx43r(i1,i2,i3,kd)+wsayy43r(i1,
     & i2,i3,kd)
      wsalaplacian43r(i1,i2,i3,kd)=wsaxx43r(i1,i2,i3,kd)+wsayy43r(i1,
     & i2,i3,kd)+wsazz43r(i1,i2,i3,kd)
      wsamr4(i1,i2,i3,kd)=(8.*(wsam(i1+1,i2,i3,kd)-wsam(i1-1,i2,i3,kd))
     & -(wsam(i1+2,i2,i3,kd)-wsam(i1-2,i2,i3,kd)))*d14(0)
      wsams4(i1,i2,i3,kd)=(8.*(wsam(i1,i2+1,i3,kd)-wsam(i1,i2-1,i3,kd))
     & -(wsam(i1,i2+2,i3,kd)-wsam(i1,i2-2,i3,kd)))*d14(1)
      wsamt4(i1,i2,i3,kd)=(8.*(wsam(i1,i2,i3+1,kd)-wsam(i1,i2,i3-1,kd))
     & -(wsam(i1,i2,i3+2,kd)-wsam(i1,i2,i3-2,kd)))*d14(2)
      wsamrr4(i1,i2,i3,kd)=(-30.*wsam(i1,i2,i3,kd)+16.*(wsam(i1+1,i2,
     & i3,kd)+wsam(i1-1,i2,i3,kd))-(wsam(i1+2,i2,i3,kd)+wsam(i1-2,i2,
     & i3,kd)) )*d24(0)
      wsamss4(i1,i2,i3,kd)=(-30.*wsam(i1,i2,i3,kd)+16.*(wsam(i1,i2+1,
     & i3,kd)+wsam(i1,i2-1,i3,kd))-(wsam(i1,i2+2,i3,kd)+wsam(i1,i2-2,
     & i3,kd)) )*d24(1)
      wsamtt4(i1,i2,i3,kd)=(-30.*wsam(i1,i2,i3,kd)+16.*(wsam(i1,i2,i3+
     & 1,kd)+wsam(i1,i2,i3-1,kd))-(wsam(i1,i2,i3+2,kd)+wsam(i1,i2,i3-
     & 2,kd)) )*d24(2)
      wsamrs4(i1,i2,i3,kd)=(8.*(wsamr4(i1,i2+1,i3,kd)-wsamr4(i1,i2-1,
     & i3,kd))-(wsamr4(i1,i2+2,i3,kd)-wsamr4(i1,i2-2,i3,kd)))*d14(1)
      wsamrt4(i1,i2,i3,kd)=(8.*(wsamr4(i1,i2,i3+1,kd)-wsamr4(i1,i2,i3-
     & 1,kd))-(wsamr4(i1,i2,i3+2,kd)-wsamr4(i1,i2,i3-2,kd)))*d14(2)
      wsamst4(i1,i2,i3,kd)=(8.*(wsams4(i1,i2,i3+1,kd)-wsams4(i1,i2,i3-
     & 1,kd))-(wsams4(i1,i2,i3+2,kd)-wsams4(i1,i2,i3-2,kd)))*d14(2)
      wsamx41(i1,i2,i3,kd)= rx(i1,i2,i3)*wsamr4(i1,i2,i3,kd)
      wsamy41(i1,i2,i3,kd)=0
      wsamz41(i1,i2,i3,kd)=0
      wsamx42(i1,i2,i3,kd)= rx(i1,i2,i3)*wsamr4(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*wsams4(i1,i2,i3,kd)
      wsamy42(i1,i2,i3,kd)= ry(i1,i2,i3)*wsamr4(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*wsams4(i1,i2,i3,kd)
      wsamz42(i1,i2,i3,kd)=0
      wsamx43(i1,i2,i3,kd)=rx(i1,i2,i3)*wsamr4(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*wsams4(i1,i2,i3,kd)+tx(i1,i2,i3)*wsamt4(i1,i2,i3,kd)
      wsamy43(i1,i2,i3,kd)=ry(i1,i2,i3)*wsamr4(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*wsams4(i1,i2,i3,kd)+ty(i1,i2,i3)*wsamt4(i1,i2,i3,kd)
      wsamz43(i1,i2,i3,kd)=rz(i1,i2,i3)*wsamr4(i1,i2,i3,kd)+sz(i1,i2,
     & i3)*wsams4(i1,i2,i3,kd)+tz(i1,i2,i3)*wsamt4(i1,i2,i3,kd)
      wsamxx41(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wsamrr4(i1,i2,i3,kd)+(
     & rxx42(i1,i2,i3))*wsamr4(i1,i2,i3,kd)
      wsamyy41(i1,i2,i3,kd)=0
      wsamxy41(i1,i2,i3,kd)=0
      wsamxz41(i1,i2,i3,kd)=0
      wsamyz41(i1,i2,i3,kd)=0
      wsamzz41(i1,i2,i3,kd)=0
      wsamlaplacian41(i1,i2,i3,kd)=wsamxx41(i1,i2,i3,kd)
      wsamxx42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wsamrr4(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*wsamrs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*wsamss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3))*wsamr4(i1,i2,i3,kd)+(
     & sxx42(i1,i2,i3))*wsams4(i1,i2,i3,kd)
      wsamyy42(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*wsamrr4(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*wsamrs4(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*wsamss4(i1,i2,i3,kd)+(ryy42(i1,i2,i3))*wsamr4(i1,i2,i3,kd)+(
     & syy42(i1,i2,i3))*wsams4(i1,i2,i3,kd)
      wsamxy42(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wsamrr4(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & wsamrs4(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wsamss4(i1,i2,
     & i3,kd)+rxy42(i1,i2,i3)*wsamr4(i1,i2,i3,kd)+sxy42(i1,i2,i3)*
     & wsams4(i1,i2,i3,kd)
      wsamxz42(i1,i2,i3,kd)=0
      wsamyz42(i1,i2,i3,kd)=0
      wsamzz42(i1,i2,i3,kd)=0
      wsamlaplacian42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & wsamrr4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,
     & i3)*sy(i1,i2,i3))*wsamrs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,
     & i2,i3)**2)*wsamss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3)+ryy42(i1,i2,
     & i3))*wsamr4(i1,i2,i3,kd)+(sxx42(i1,i2,i3)+syy42(i1,i2,i3))*
     & wsams4(i1,i2,i3,kd)
      wsamxx43(i1,i2,i3,kd)=rx(i1,i2,i3)**2*wsamrr4(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*wsamss4(i1,i2,i3,kd)+tx(i1,i2,i3)**2*wsamtt4(i1,i2,
     & i3,kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*wsamrs4(i1,i2,i3,kd)+2.*rx(
     & i1,i2,i3)*tx(i1,i2,i3)*wsamrt4(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(
     & i1,i2,i3)*wsamst4(i1,i2,i3,kd)+rxx43(i1,i2,i3)*wsamr4(i1,i2,i3,
     & kd)+sxx43(i1,i2,i3)*wsams4(i1,i2,i3,kd)+txx43(i1,i2,i3)*wsamt4(
     & i1,i2,i3,kd)
      wsamyy43(i1,i2,i3,kd)=ry(i1,i2,i3)**2*wsamrr4(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*wsamss4(i1,i2,i3,kd)+ty(i1,i2,i3)**2*wsamtt4(i1,i2,
     & i3,kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*wsamrs4(i1,i2,i3,kd)+2.*ry(
     & i1,i2,i3)*ty(i1,i2,i3)*wsamrt4(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(
     & i1,i2,i3)*wsamst4(i1,i2,i3,kd)+ryy43(i1,i2,i3)*wsamr4(i1,i2,i3,
     & kd)+syy43(i1,i2,i3)*wsams4(i1,i2,i3,kd)+tyy43(i1,i2,i3)*wsamt4(
     & i1,i2,i3,kd)
      wsamzz43(i1,i2,i3,kd)=rz(i1,i2,i3)**2*wsamrr4(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*wsamss4(i1,i2,i3,kd)+tz(i1,i2,i3)**2*wsamtt4(i1,i2,
     & i3,kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*wsamrs4(i1,i2,i3,kd)+2.*rz(
     & i1,i2,i3)*tz(i1,i2,i3)*wsamrt4(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(
     & i1,i2,i3)*wsamst4(i1,i2,i3,kd)+rzz43(i1,i2,i3)*wsamr4(i1,i2,i3,
     & kd)+szz43(i1,i2,i3)*wsams4(i1,i2,i3,kd)+tzz43(i1,i2,i3)*wsamt4(
     & i1,i2,i3,kd)
      wsamxy43(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wsamrr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wsamss4(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *ty(i1,i2,i3)*wsamtt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+
     & ry(i1,i2,i3)*sx(i1,i2,i3))*wsamrs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & ty(i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*wsamrt4(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*wsamst4(
     & i1,i2,i3,kd)+rxy43(i1,i2,i3)*wsamr4(i1,i2,i3,kd)+sxy43(i1,i2,
     & i3)*wsams4(i1,i2,i3,kd)+txy43(i1,i2,i3)*wsamt4(i1,i2,i3,kd)
      wsamxz43(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*wsamrr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*wsamss4(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *tz(i1,i2,i3)*wsamtt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sx(i1,i2,i3))*wsamrs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*wsamrt4(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*wsamst4(
     & i1,i2,i3,kd)+rxz43(i1,i2,i3)*wsamr4(i1,i2,i3,kd)+sxz43(i1,i2,
     & i3)*wsams4(i1,i2,i3,kd)+txz43(i1,i2,i3)*wsamt4(i1,i2,i3,kd)
      wsamyz43(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*wsamrr4(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*wsamss4(i1,i2,i3,kd)+ty(i1,i2,i3)
     & *tz(i1,i2,i3)*wsamtt4(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sy(i1,i2,i3))*wsamrs4(i1,i2,i3,kd)+(ry(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*wsamrt4(i1,i2,i3,kd)+(
     & sy(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*wsamst4(
     & i1,i2,i3,kd)+ryz43(i1,i2,i3)*wsamr4(i1,i2,i3,kd)+syz43(i1,i2,
     & i3)*wsams4(i1,i2,i3,kd)+tyz43(i1,i2,i3)*wsamt4(i1,i2,i3,kd)
      wsamlaplacian43(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*wsamrr4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2+sz(i1,i2,i3)**2)*wsamss4(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+
     & ty(i1,i2,i3)**2+tz(i1,i2,i3)**2)*wsamtt4(i1,i2,i3,kd)+2.*(rx(
     & i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*
     & sz(i1,i2,i3))*wsamrs4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,
     & i3)+ ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*
     & wsamrt4(i1,i2,i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,
     & i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3))*wsamst4(i1,i2,i3,
     & kd)+(rxx43(i1,i2,i3)+ryy43(i1,i2,i3)+rzz43(i1,i2,i3))*wsamr4(
     & i1,i2,i3,kd)+(sxx43(i1,i2,i3)+syy43(i1,i2,i3)+szz43(i1,i2,i3))*
     & wsams4(i1,i2,i3,kd)+(txx43(i1,i2,i3)+tyy43(i1,i2,i3)+tzz43(i1,
     & i2,i3))*wsamt4(i1,i2,i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      wsamx43r(i1,i2,i3,kd)=(8.*(wsam(i1+1,i2,i3,kd)-wsam(i1-1,i2,i3,
     & kd))-(wsam(i1+2,i2,i3,kd)-wsam(i1-2,i2,i3,kd)))*h41(0)
      wsamy43r(i1,i2,i3,kd)=(8.*(wsam(i1,i2+1,i3,kd)-wsam(i1,i2-1,i3,
     & kd))-(wsam(i1,i2+2,i3,kd)-wsam(i1,i2-2,i3,kd)))*h41(1)
      wsamz43r(i1,i2,i3,kd)=(8.*(wsam(i1,i2,i3+1,kd)-wsam(i1,i2,i3-1,
     & kd))-(wsam(i1,i2,i3+2,kd)-wsam(i1,i2,i3-2,kd)))*h41(2)
      wsamxx43r(i1,i2,i3,kd)=( -30.*wsam(i1,i2,i3,kd)+16.*(wsam(i1+1,
     & i2,i3,kd)+wsam(i1-1,i2,i3,kd))-(wsam(i1+2,i2,i3,kd)+wsam(i1-2,
     & i2,i3,kd)) )*h42(0)
      wsamyy43r(i1,i2,i3,kd)=( -30.*wsam(i1,i2,i3,kd)+16.*(wsam(i1,i2+
     & 1,i3,kd)+wsam(i1,i2-1,i3,kd))-(wsam(i1,i2+2,i3,kd)+wsam(i1,i2-
     & 2,i3,kd)) )*h42(1)
      wsamzz43r(i1,i2,i3,kd)=( -30.*wsam(i1,i2,i3,kd)+16.*(wsam(i1,i2,
     & i3+1,kd)+wsam(i1,i2,i3-1,kd))-(wsam(i1,i2,i3+2,kd)+wsam(i1,i2,
     & i3-2,kd)) )*h42(2)
      wsamxy43r(i1,i2,i3,kd)=( (wsam(i1+2,i2+2,i3,kd)-wsam(i1-2,i2+2,
     & i3,kd)- wsam(i1+2,i2-2,i3,kd)+wsam(i1-2,i2-2,i3,kd)) +8.*(wsam(
     & i1-1,i2+2,i3,kd)-wsam(i1-1,i2-2,i3,kd)-wsam(i1+1,i2+2,i3,kd)+
     & wsam(i1+1,i2-2,i3,kd) +wsam(i1+2,i2-1,i3,kd)-wsam(i1-2,i2-1,i3,
     & kd)-wsam(i1+2,i2+1,i3,kd)+wsam(i1-2,i2+1,i3,kd))+64.*(wsam(i1+
     & 1,i2+1,i3,kd)-wsam(i1-1,i2+1,i3,kd)- wsam(i1+1,i2-1,i3,kd)+
     & wsam(i1-1,i2-1,i3,kd)))*(h41(0)*h41(1))
      wsamxz43r(i1,i2,i3,kd)=( (wsam(i1+2,i2,i3+2,kd)-wsam(i1-2,i2,i3+
     & 2,kd)-wsam(i1+2,i2,i3-2,kd)+wsam(i1-2,i2,i3-2,kd)) +8.*(wsam(
     & i1-1,i2,i3+2,kd)-wsam(i1-1,i2,i3-2,kd)-wsam(i1+1,i2,i3+2,kd)+
     & wsam(i1+1,i2,i3-2,kd) +wsam(i1+2,i2,i3-1,kd)-wsam(i1-2,i2,i3-1,
     & kd)- wsam(i1+2,i2,i3+1,kd)+wsam(i1-2,i2,i3+1,kd)) +64.*(wsam(
     & i1+1,i2,i3+1,kd)-wsam(i1-1,i2,i3+1,kd)-wsam(i1+1,i2,i3-1,kd)+
     & wsam(i1-1,i2,i3-1,kd)) )*(h41(0)*h41(2))
      wsamyz43r(i1,i2,i3,kd)=( (wsam(i1,i2+2,i3+2,kd)-wsam(i1,i2-2,i3+
     & 2,kd)-wsam(i1,i2+2,i3-2,kd)+wsam(i1,i2-2,i3-2,kd)) +8.*(wsam(
     & i1,i2-1,i3+2,kd)-wsam(i1,i2-1,i3-2,kd)-wsam(i1,i2+1,i3+2,kd)+
     & wsam(i1,i2+1,i3-2,kd) +wsam(i1,i2+2,i3-1,kd)-wsam(i1,i2-2,i3-1,
     & kd)-wsam(i1,i2+2,i3+1,kd)+wsam(i1,i2-2,i3+1,kd)) +64.*(wsam(i1,
     & i2+1,i3+1,kd)-wsam(i1,i2-1,i3+1,kd)-wsam(i1,i2+1,i3-1,kd)+wsam(
     & i1,i2-1,i3-1,kd)) )*(h41(1)*h41(2))
      wsamx41r(i1,i2,i3,kd)= wsamx43r(i1,i2,i3,kd)
      wsamy41r(i1,i2,i3,kd)= wsamy43r(i1,i2,i3,kd)
      wsamz41r(i1,i2,i3,kd)= wsamz43r(i1,i2,i3,kd)
      wsamxx41r(i1,i2,i3,kd)= wsamxx43r(i1,i2,i3,kd)
      wsamyy41r(i1,i2,i3,kd)= wsamyy43r(i1,i2,i3,kd)
      wsamzz41r(i1,i2,i3,kd)= wsamzz43r(i1,i2,i3,kd)
      wsamxy41r(i1,i2,i3,kd)= wsamxy43r(i1,i2,i3,kd)
      wsamxz41r(i1,i2,i3,kd)= wsamxz43r(i1,i2,i3,kd)
      wsamyz41r(i1,i2,i3,kd)= wsamyz43r(i1,i2,i3,kd)
      wsamlaplacian41r(i1,i2,i3,kd)=wsamxx43r(i1,i2,i3,kd)
      wsamx42r(i1,i2,i3,kd)= wsamx43r(i1,i2,i3,kd)
      wsamy42r(i1,i2,i3,kd)= wsamy43r(i1,i2,i3,kd)
      wsamz42r(i1,i2,i3,kd)= wsamz43r(i1,i2,i3,kd)
      wsamxx42r(i1,i2,i3,kd)= wsamxx43r(i1,i2,i3,kd)
      wsamyy42r(i1,i2,i3,kd)= wsamyy43r(i1,i2,i3,kd)
      wsamzz42r(i1,i2,i3,kd)= wsamzz43r(i1,i2,i3,kd)
      wsamxy42r(i1,i2,i3,kd)= wsamxy43r(i1,i2,i3,kd)
      wsamxz42r(i1,i2,i3,kd)= wsamxz43r(i1,i2,i3,kd)
      wsamyz42r(i1,i2,i3,kd)= wsamyz43r(i1,i2,i3,kd)
      wsamlaplacian42r(i1,i2,i3,kd)=wsamxx43r(i1,i2,i3,kd)+wsamyy43r(
     & i1,i2,i3,kd)
      wsamlaplacian43r(i1,i2,i3,kd)=wsamxx43r(i1,i2,i3,kd)+wsamyy43r(
     & i1,i2,i3,kd)+wsamzz43r(i1,i2,i3,kd)

      vsbr4(i1,i2,i3,kd)=(8.*(vsb(i1+1,i2,i3,kd)-vsb(i1-1,i2,i3,kd))-(
     & vsb(i1+2,i2,i3,kd)-vsb(i1-2,i2,i3,kd)))*d14(0)
      vsbs4(i1,i2,i3,kd)=(8.*(vsb(i1,i2+1,i3,kd)-vsb(i1,i2-1,i3,kd))-(
     & vsb(i1,i2+2,i3,kd)-vsb(i1,i2-2,i3,kd)))*d14(1)
      vsbt4(i1,i2,i3,kd)=(8.*(vsb(i1,i2,i3+1,kd)-vsb(i1,i2,i3-1,kd))-(
     & vsb(i1,i2,i3+2,kd)-vsb(i1,i2,i3-2,kd)))*d14(2)
      vsbrr4(i1,i2,i3,kd)=(-30.*vsb(i1,i2,i3,kd)+16.*(vsb(i1+1,i2,i3,
     & kd)+vsb(i1-1,i2,i3,kd))-(vsb(i1+2,i2,i3,kd)+vsb(i1-2,i2,i3,kd))
     &  )*d24(0)
      vsbss4(i1,i2,i3,kd)=(-30.*vsb(i1,i2,i3,kd)+16.*(vsb(i1,i2+1,i3,
     & kd)+vsb(i1,i2-1,i3,kd))-(vsb(i1,i2+2,i3,kd)+vsb(i1,i2-2,i3,kd))
     &  )*d24(1)
      vsbtt4(i1,i2,i3,kd)=(-30.*vsb(i1,i2,i3,kd)+16.*(vsb(i1,i2,i3+1,
     & kd)+vsb(i1,i2,i3-1,kd))-(vsb(i1,i2,i3+2,kd)+vsb(i1,i2,i3-2,kd))
     &  )*d24(2)
      vsbrs4(i1,i2,i3,kd)=(8.*(vsbr4(i1,i2+1,i3,kd)-vsbr4(i1,i2-1,i3,
     & kd))-(vsbr4(i1,i2+2,i3,kd)-vsbr4(i1,i2-2,i3,kd)))*d14(1)
      vsbrt4(i1,i2,i3,kd)=(8.*(vsbr4(i1,i2,i3+1,kd)-vsbr4(i1,i2,i3-1,
     & kd))-(vsbr4(i1,i2,i3+2,kd)-vsbr4(i1,i2,i3-2,kd)))*d14(2)
      vsbst4(i1,i2,i3,kd)=(8.*(vsbs4(i1,i2,i3+1,kd)-vsbs4(i1,i2,i3-1,
     & kd))-(vsbs4(i1,i2,i3+2,kd)-vsbs4(i1,i2,i3-2,kd)))*d14(2)
      vsbx41(i1,i2,i3,kd)= rx(i1,i2,i3)*vsbr4(i1,i2,i3,kd)
      vsby41(i1,i2,i3,kd)=0
      vsbz41(i1,i2,i3,kd)=0
      vsbx42(i1,i2,i3,kd)= rx(i1,i2,i3)*vsbr4(i1,i2,i3,kd)+sx(i1,i2,i3)
     & *vsbs4(i1,i2,i3,kd)
      vsby42(i1,i2,i3,kd)= ry(i1,i2,i3)*vsbr4(i1,i2,i3,kd)+sy(i1,i2,i3)
     & *vsbs4(i1,i2,i3,kd)
      vsbz42(i1,i2,i3,kd)=0
      vsbx43(i1,i2,i3,kd)=rx(i1,i2,i3)*vsbr4(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & vsbs4(i1,i2,i3,kd)+tx(i1,i2,i3)*vsbt4(i1,i2,i3,kd)
      vsby43(i1,i2,i3,kd)=ry(i1,i2,i3)*vsbr4(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & vsbs4(i1,i2,i3,kd)+ty(i1,i2,i3)*vsbt4(i1,i2,i3,kd)
      vsbz43(i1,i2,i3,kd)=rz(i1,i2,i3)*vsbr4(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & vsbs4(i1,i2,i3,kd)+tz(i1,i2,i3)*vsbt4(i1,i2,i3,kd)
      vsbxx41(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vsbrr4(i1,i2,i3,kd)+(
     & rxx42(i1,i2,i3))*vsbr4(i1,i2,i3,kd)
      vsbyy41(i1,i2,i3,kd)=0
      vsbxy41(i1,i2,i3,kd)=0
      vsbxz41(i1,i2,i3,kd)=0
      vsbyz41(i1,i2,i3,kd)=0
      vsbzz41(i1,i2,i3,kd)=0
      vsblaplacian41(i1,i2,i3,kd)=vsbxx41(i1,i2,i3,kd)
      vsbxx42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vsbrr4(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*vsbrs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*vsbss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3))*vsbr4(i1,i2,i3,kd)+(
     & sxx42(i1,i2,i3))*vsbs4(i1,i2,i3,kd)
      vsbyy42(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*vsbrr4(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*vsbrs4(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*vsbss4(i1,i2,i3,kd)+(ryy42(i1,i2,i3))*vsbr4(i1,i2,i3,kd)+(
     & syy42(i1,i2,i3))*vsbs4(i1,i2,i3,kd)
      vsbxy42(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vsbrr4(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & vsbrs4(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vsbss4(i1,i2,i3,
     & kd)+rxy42(i1,i2,i3)*vsbr4(i1,i2,i3,kd)+sxy42(i1,i2,i3)*vsbs4(
     & i1,i2,i3,kd)
      vsbxz42(i1,i2,i3,kd)=0
      vsbyz42(i1,i2,i3,kd)=0
      vsbzz42(i1,i2,i3,kd)=0
      vsblaplacian42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & vsbrr4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)
     & *sy(i1,i2,i3))*vsbrs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2)*vsbss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3)+ryy42(i1,i2,i3))*
     & vsbr4(i1,i2,i3,kd)+(sxx42(i1,i2,i3)+syy42(i1,i2,i3))*vsbs4(i1,
     & i2,i3,kd)
      vsbxx43(i1,i2,i3,kd)=rx(i1,i2,i3)**2*vsbrr4(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*vsbss4(i1,i2,i3,kd)+tx(i1,i2,i3)**2*vsbtt4(i1,i2,i3,
     & kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*vsbrs4(i1,i2,i3,kd)+2.*rx(i1,
     & i2,i3)*tx(i1,i2,i3)*vsbrt4(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,
     & i2,i3)*vsbst4(i1,i2,i3,kd)+rxx43(i1,i2,i3)*vsbr4(i1,i2,i3,kd)+
     & sxx43(i1,i2,i3)*vsbs4(i1,i2,i3,kd)+txx43(i1,i2,i3)*vsbt4(i1,i2,
     & i3,kd)
      vsbyy43(i1,i2,i3,kd)=ry(i1,i2,i3)**2*vsbrr4(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*vsbss4(i1,i2,i3,kd)+ty(i1,i2,i3)**2*vsbtt4(i1,i2,i3,
     & kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*vsbrs4(i1,i2,i3,kd)+2.*ry(i1,
     & i2,i3)*ty(i1,i2,i3)*vsbrt4(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,
     & i2,i3)*vsbst4(i1,i2,i3,kd)+ryy43(i1,i2,i3)*vsbr4(i1,i2,i3,kd)+
     & syy43(i1,i2,i3)*vsbs4(i1,i2,i3,kd)+tyy43(i1,i2,i3)*vsbt4(i1,i2,
     & i3,kd)
      vsbzz43(i1,i2,i3,kd)=rz(i1,i2,i3)**2*vsbrr4(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*vsbss4(i1,i2,i3,kd)+tz(i1,i2,i3)**2*vsbtt4(i1,i2,i3,
     & kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*vsbrs4(i1,i2,i3,kd)+2.*rz(i1,
     & i2,i3)*tz(i1,i2,i3)*vsbrt4(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,
     & i2,i3)*vsbst4(i1,i2,i3,kd)+rzz43(i1,i2,i3)*vsbr4(i1,i2,i3,kd)+
     & szz43(i1,i2,i3)*vsbs4(i1,i2,i3,kd)+tzz43(i1,i2,i3)*vsbt4(i1,i2,
     & i3,kd)
      vsbxy43(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vsbrr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vsbss4(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & ty(i1,i2,i3)*vsbtt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(
     & i1,i2,i3)*sx(i1,i2,i3))*vsbrs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(
     & i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*vsbrt4(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*vsbst4(i1,i2,
     & i3,kd)+rxy43(i1,i2,i3)*vsbr4(i1,i2,i3,kd)+sxy43(i1,i2,i3)*
     & vsbs4(i1,i2,i3,kd)+txy43(i1,i2,i3)*vsbt4(i1,i2,i3,kd)
      vsbxz43(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*vsbrr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*vsbss4(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & tz(i1,i2,i3)*vsbtt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sx(i1,i2,i3))*vsbrs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*vsbrt4(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*vsbst4(i1,i2,
     & i3,kd)+rxz43(i1,i2,i3)*vsbr4(i1,i2,i3,kd)+sxz43(i1,i2,i3)*
     & vsbs4(i1,i2,i3,kd)+txz43(i1,i2,i3)*vsbt4(i1,i2,i3,kd)
      vsbyz43(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*vsbrr4(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*vsbss4(i1,i2,i3,kd)+ty(i1,i2,i3)*
     & tz(i1,i2,i3)*vsbtt4(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sy(i1,i2,i3))*vsbrs4(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*vsbrt4(i1,i2,i3,kd)+(sy(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*vsbst4(i1,i2,
     & i3,kd)+ryz43(i1,i2,i3)*vsbr4(i1,i2,i3,kd)+syz43(i1,i2,i3)*
     & vsbs4(i1,i2,i3,kd)+tyz43(i1,i2,i3)*vsbt4(i1,i2,i3,kd)
      vsblaplacian43(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*vsbrr4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)
     & **2+sz(i1,i2,i3)**2)*vsbss4(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(
     & i1,i2,i3)**2+tz(i1,i2,i3)**2)*vsbtt4(i1,i2,i3,kd)+2.*(rx(i1,i2,
     & i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,
     & i2,i3))*vsbrs4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(
     & i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*vsbrt4(i1,i2,
     & i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)
     & +sz(i1,i2,i3)*tz(i1,i2,i3))*vsbst4(i1,i2,i3,kd)+(rxx43(i1,i2,
     & i3)+ryy43(i1,i2,i3)+rzz43(i1,i2,i3))*vsbr4(i1,i2,i3,kd)+(sxx43(
     & i1,i2,i3)+syy43(i1,i2,i3)+szz43(i1,i2,i3))*vsbs4(i1,i2,i3,kd)+(
     & txx43(i1,i2,i3)+tyy43(i1,i2,i3)+tzz43(i1,i2,i3))*vsbt4(i1,i2,
     & i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      vsbx43r(i1,i2,i3,kd)=(8.*(vsb(i1+1,i2,i3,kd)-vsb(i1-1,i2,i3,kd))-
     & (vsb(i1+2,i2,i3,kd)-vsb(i1-2,i2,i3,kd)))*h41(0)
      vsby43r(i1,i2,i3,kd)=(8.*(vsb(i1,i2+1,i3,kd)-vsb(i1,i2-1,i3,kd))-
     & (vsb(i1,i2+2,i3,kd)-vsb(i1,i2-2,i3,kd)))*h41(1)
      vsbz43r(i1,i2,i3,kd)=(8.*(vsb(i1,i2,i3+1,kd)-vsb(i1,i2,i3-1,kd))-
     & (vsb(i1,i2,i3+2,kd)-vsb(i1,i2,i3-2,kd)))*h41(2)
      vsbxx43r(i1,i2,i3,kd)=( -30.*vsb(i1,i2,i3,kd)+16.*(vsb(i1+1,i2,
     & i3,kd)+vsb(i1-1,i2,i3,kd))-(vsb(i1+2,i2,i3,kd)+vsb(i1-2,i2,i3,
     & kd)) )*h42(0)
      vsbyy43r(i1,i2,i3,kd)=( -30.*vsb(i1,i2,i3,kd)+16.*(vsb(i1,i2+1,
     & i3,kd)+vsb(i1,i2-1,i3,kd))-(vsb(i1,i2+2,i3,kd)+vsb(i1,i2-2,i3,
     & kd)) )*h42(1)
      vsbzz43r(i1,i2,i3,kd)=( -30.*vsb(i1,i2,i3,kd)+16.*(vsb(i1,i2,i3+
     & 1,kd)+vsb(i1,i2,i3-1,kd))-(vsb(i1,i2,i3+2,kd)+vsb(i1,i2,i3-2,
     & kd)) )*h42(2)
      vsbxy43r(i1,i2,i3,kd)=( (vsb(i1+2,i2+2,i3,kd)-vsb(i1-2,i2+2,i3,
     & kd)- vsb(i1+2,i2-2,i3,kd)+vsb(i1-2,i2-2,i3,kd)) +8.*(vsb(i1-1,
     & i2+2,i3,kd)-vsb(i1-1,i2-2,i3,kd)-vsb(i1+1,i2+2,i3,kd)+vsb(i1+1,
     & i2-2,i3,kd) +vsb(i1+2,i2-1,i3,kd)-vsb(i1-2,i2-1,i3,kd)-vsb(i1+
     & 2,i2+1,i3,kd)+vsb(i1-2,i2+1,i3,kd))+64.*(vsb(i1+1,i2+1,i3,kd)-
     & vsb(i1-1,i2+1,i3,kd)- vsb(i1+1,i2-1,i3,kd)+vsb(i1-1,i2-1,i3,kd)
     & ))*(h41(0)*h41(1))
      vsbxz43r(i1,i2,i3,kd)=( (vsb(i1+2,i2,i3+2,kd)-vsb(i1-2,i2,i3+2,
     & kd)-vsb(i1+2,i2,i3-2,kd)+vsb(i1-2,i2,i3-2,kd)) +8.*(vsb(i1-1,
     & i2,i3+2,kd)-vsb(i1-1,i2,i3-2,kd)-vsb(i1+1,i2,i3+2,kd)+vsb(i1+1,
     & i2,i3-2,kd) +vsb(i1+2,i2,i3-1,kd)-vsb(i1-2,i2,i3-1,kd)- vsb(i1+
     & 2,i2,i3+1,kd)+vsb(i1-2,i2,i3+1,kd)) +64.*(vsb(i1+1,i2,i3+1,kd)-
     & vsb(i1-1,i2,i3+1,kd)-vsb(i1+1,i2,i3-1,kd)+vsb(i1-1,i2,i3-1,kd))
     &  )*(h41(0)*h41(2))
      vsbyz43r(i1,i2,i3,kd)=( (vsb(i1,i2+2,i3+2,kd)-vsb(i1,i2-2,i3+2,
     & kd)-vsb(i1,i2+2,i3-2,kd)+vsb(i1,i2-2,i3-2,kd)) +8.*(vsb(i1,i2-
     & 1,i3+2,kd)-vsb(i1,i2-1,i3-2,kd)-vsb(i1,i2+1,i3+2,kd)+vsb(i1,i2+
     & 1,i3-2,kd) +vsb(i1,i2+2,i3-1,kd)-vsb(i1,i2-2,i3-1,kd)-vsb(i1,
     & i2+2,i3+1,kd)+vsb(i1,i2-2,i3+1,kd)) +64.*(vsb(i1,i2+1,i3+1,kd)-
     & vsb(i1,i2-1,i3+1,kd)-vsb(i1,i2+1,i3-1,kd)+vsb(i1,i2-1,i3-1,kd))
     &  )*(h41(1)*h41(2))
      vsbx41r(i1,i2,i3,kd)= vsbx43r(i1,i2,i3,kd)
      vsby41r(i1,i2,i3,kd)= vsby43r(i1,i2,i3,kd)
      vsbz41r(i1,i2,i3,kd)= vsbz43r(i1,i2,i3,kd)
      vsbxx41r(i1,i2,i3,kd)= vsbxx43r(i1,i2,i3,kd)
      vsbyy41r(i1,i2,i3,kd)= vsbyy43r(i1,i2,i3,kd)
      vsbzz41r(i1,i2,i3,kd)= vsbzz43r(i1,i2,i3,kd)
      vsbxy41r(i1,i2,i3,kd)= vsbxy43r(i1,i2,i3,kd)
      vsbxz41r(i1,i2,i3,kd)= vsbxz43r(i1,i2,i3,kd)
      vsbyz41r(i1,i2,i3,kd)= vsbyz43r(i1,i2,i3,kd)
      vsblaplacian41r(i1,i2,i3,kd)=vsbxx43r(i1,i2,i3,kd)
      vsbx42r(i1,i2,i3,kd)= vsbx43r(i1,i2,i3,kd)
      vsby42r(i1,i2,i3,kd)= vsby43r(i1,i2,i3,kd)
      vsbz42r(i1,i2,i3,kd)= vsbz43r(i1,i2,i3,kd)
      vsbxx42r(i1,i2,i3,kd)= vsbxx43r(i1,i2,i3,kd)
      vsbyy42r(i1,i2,i3,kd)= vsbyy43r(i1,i2,i3,kd)
      vsbzz42r(i1,i2,i3,kd)= vsbzz43r(i1,i2,i3,kd)
      vsbxy42r(i1,i2,i3,kd)= vsbxy43r(i1,i2,i3,kd)
      vsbxz42r(i1,i2,i3,kd)= vsbxz43r(i1,i2,i3,kd)
      vsbyz42r(i1,i2,i3,kd)= vsbyz43r(i1,i2,i3,kd)
      vsblaplacian42r(i1,i2,i3,kd)=vsbxx43r(i1,i2,i3,kd)+vsbyy43r(i1,
     & i2,i3,kd)
      vsblaplacian43r(i1,i2,i3,kd)=vsbxx43r(i1,i2,i3,kd)+vsbyy43r(i1,
     & i2,i3,kd)+vsbzz43r(i1,i2,i3,kd)
      vsbmr4(i1,i2,i3,kd)=(8.*(vsbm(i1+1,i2,i3,kd)-vsbm(i1-1,i2,i3,kd))
     & -(vsbm(i1+2,i2,i3,kd)-vsbm(i1-2,i2,i3,kd)))*d14(0)
      vsbms4(i1,i2,i3,kd)=(8.*(vsbm(i1,i2+1,i3,kd)-vsbm(i1,i2-1,i3,kd))
     & -(vsbm(i1,i2+2,i3,kd)-vsbm(i1,i2-2,i3,kd)))*d14(1)
      vsbmt4(i1,i2,i3,kd)=(8.*(vsbm(i1,i2,i3+1,kd)-vsbm(i1,i2,i3-1,kd))
     & -(vsbm(i1,i2,i3+2,kd)-vsbm(i1,i2,i3-2,kd)))*d14(2)
      vsbmrr4(i1,i2,i3,kd)=(-30.*vsbm(i1,i2,i3,kd)+16.*(vsbm(i1+1,i2,
     & i3,kd)+vsbm(i1-1,i2,i3,kd))-(vsbm(i1+2,i2,i3,kd)+vsbm(i1-2,i2,
     & i3,kd)) )*d24(0)
      vsbmss4(i1,i2,i3,kd)=(-30.*vsbm(i1,i2,i3,kd)+16.*(vsbm(i1,i2+1,
     & i3,kd)+vsbm(i1,i2-1,i3,kd))-(vsbm(i1,i2+2,i3,kd)+vsbm(i1,i2-2,
     & i3,kd)) )*d24(1)
      vsbmtt4(i1,i2,i3,kd)=(-30.*vsbm(i1,i2,i3,kd)+16.*(vsbm(i1,i2,i3+
     & 1,kd)+vsbm(i1,i2,i3-1,kd))-(vsbm(i1,i2,i3+2,kd)+vsbm(i1,i2,i3-
     & 2,kd)) )*d24(2)
      vsbmrs4(i1,i2,i3,kd)=(8.*(vsbmr4(i1,i2+1,i3,kd)-vsbmr4(i1,i2-1,
     & i3,kd))-(vsbmr4(i1,i2+2,i3,kd)-vsbmr4(i1,i2-2,i3,kd)))*d14(1)
      vsbmrt4(i1,i2,i3,kd)=(8.*(vsbmr4(i1,i2,i3+1,kd)-vsbmr4(i1,i2,i3-
     & 1,kd))-(vsbmr4(i1,i2,i3+2,kd)-vsbmr4(i1,i2,i3-2,kd)))*d14(2)
      vsbmst4(i1,i2,i3,kd)=(8.*(vsbms4(i1,i2,i3+1,kd)-vsbms4(i1,i2,i3-
     & 1,kd))-(vsbms4(i1,i2,i3+2,kd)-vsbms4(i1,i2,i3-2,kd)))*d14(2)
      vsbmx41(i1,i2,i3,kd)= rx(i1,i2,i3)*vsbmr4(i1,i2,i3,kd)
      vsbmy41(i1,i2,i3,kd)=0
      vsbmz41(i1,i2,i3,kd)=0
      vsbmx42(i1,i2,i3,kd)= rx(i1,i2,i3)*vsbmr4(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*vsbms4(i1,i2,i3,kd)
      vsbmy42(i1,i2,i3,kd)= ry(i1,i2,i3)*vsbmr4(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*vsbms4(i1,i2,i3,kd)
      vsbmz42(i1,i2,i3,kd)=0
      vsbmx43(i1,i2,i3,kd)=rx(i1,i2,i3)*vsbmr4(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*vsbms4(i1,i2,i3,kd)+tx(i1,i2,i3)*vsbmt4(i1,i2,i3,kd)
      vsbmy43(i1,i2,i3,kd)=ry(i1,i2,i3)*vsbmr4(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*vsbms4(i1,i2,i3,kd)+ty(i1,i2,i3)*vsbmt4(i1,i2,i3,kd)
      vsbmz43(i1,i2,i3,kd)=rz(i1,i2,i3)*vsbmr4(i1,i2,i3,kd)+sz(i1,i2,
     & i3)*vsbms4(i1,i2,i3,kd)+tz(i1,i2,i3)*vsbmt4(i1,i2,i3,kd)
      vsbmxx41(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vsbmrr4(i1,i2,i3,kd)+(
     & rxx42(i1,i2,i3))*vsbmr4(i1,i2,i3,kd)
      vsbmyy41(i1,i2,i3,kd)=0
      vsbmxy41(i1,i2,i3,kd)=0
      vsbmxz41(i1,i2,i3,kd)=0
      vsbmyz41(i1,i2,i3,kd)=0
      vsbmzz41(i1,i2,i3,kd)=0
      vsbmlaplacian41(i1,i2,i3,kd)=vsbmxx41(i1,i2,i3,kd)
      vsbmxx42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vsbmrr4(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*vsbmrs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*vsbmss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3))*vsbmr4(i1,i2,i3,kd)+(
     & sxx42(i1,i2,i3))*vsbms4(i1,i2,i3,kd)
      vsbmyy42(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*vsbmrr4(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*vsbmrs4(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*vsbmss4(i1,i2,i3,kd)+(ryy42(i1,i2,i3))*vsbmr4(i1,i2,i3,kd)+(
     & syy42(i1,i2,i3))*vsbms4(i1,i2,i3,kd)
      vsbmxy42(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vsbmrr4(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & vsbmrs4(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vsbmss4(i1,i2,
     & i3,kd)+rxy42(i1,i2,i3)*vsbmr4(i1,i2,i3,kd)+sxy42(i1,i2,i3)*
     & vsbms4(i1,i2,i3,kd)
      vsbmxz42(i1,i2,i3,kd)=0
      vsbmyz42(i1,i2,i3,kd)=0
      vsbmzz42(i1,i2,i3,kd)=0
      vsbmlaplacian42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & vsbmrr4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,
     & i3)*sy(i1,i2,i3))*vsbmrs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,
     & i2,i3)**2)*vsbmss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3)+ryy42(i1,i2,
     & i3))*vsbmr4(i1,i2,i3,kd)+(sxx42(i1,i2,i3)+syy42(i1,i2,i3))*
     & vsbms4(i1,i2,i3,kd)
      vsbmxx43(i1,i2,i3,kd)=rx(i1,i2,i3)**2*vsbmrr4(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*vsbmss4(i1,i2,i3,kd)+tx(i1,i2,i3)**2*vsbmtt4(i1,i2,
     & i3,kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*vsbmrs4(i1,i2,i3,kd)+2.*rx(
     & i1,i2,i3)*tx(i1,i2,i3)*vsbmrt4(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(
     & i1,i2,i3)*vsbmst4(i1,i2,i3,kd)+rxx43(i1,i2,i3)*vsbmr4(i1,i2,i3,
     & kd)+sxx43(i1,i2,i3)*vsbms4(i1,i2,i3,kd)+txx43(i1,i2,i3)*vsbmt4(
     & i1,i2,i3,kd)
      vsbmyy43(i1,i2,i3,kd)=ry(i1,i2,i3)**2*vsbmrr4(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*vsbmss4(i1,i2,i3,kd)+ty(i1,i2,i3)**2*vsbmtt4(i1,i2,
     & i3,kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*vsbmrs4(i1,i2,i3,kd)+2.*ry(
     & i1,i2,i3)*ty(i1,i2,i3)*vsbmrt4(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(
     & i1,i2,i3)*vsbmst4(i1,i2,i3,kd)+ryy43(i1,i2,i3)*vsbmr4(i1,i2,i3,
     & kd)+syy43(i1,i2,i3)*vsbms4(i1,i2,i3,kd)+tyy43(i1,i2,i3)*vsbmt4(
     & i1,i2,i3,kd)
      vsbmzz43(i1,i2,i3,kd)=rz(i1,i2,i3)**2*vsbmrr4(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*vsbmss4(i1,i2,i3,kd)+tz(i1,i2,i3)**2*vsbmtt4(i1,i2,
     & i3,kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*vsbmrs4(i1,i2,i3,kd)+2.*rz(
     & i1,i2,i3)*tz(i1,i2,i3)*vsbmrt4(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(
     & i1,i2,i3)*vsbmst4(i1,i2,i3,kd)+rzz43(i1,i2,i3)*vsbmr4(i1,i2,i3,
     & kd)+szz43(i1,i2,i3)*vsbms4(i1,i2,i3,kd)+tzz43(i1,i2,i3)*vsbmt4(
     & i1,i2,i3,kd)
      vsbmxy43(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vsbmrr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vsbmss4(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *ty(i1,i2,i3)*vsbmtt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+
     & ry(i1,i2,i3)*sx(i1,i2,i3))*vsbmrs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & ty(i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*vsbmrt4(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*vsbmst4(
     & i1,i2,i3,kd)+rxy43(i1,i2,i3)*vsbmr4(i1,i2,i3,kd)+sxy43(i1,i2,
     & i3)*vsbms4(i1,i2,i3,kd)+txy43(i1,i2,i3)*vsbmt4(i1,i2,i3,kd)
      vsbmxz43(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*vsbmrr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*vsbmss4(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *tz(i1,i2,i3)*vsbmtt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sx(i1,i2,i3))*vsbmrs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*vsbmrt4(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*vsbmst4(
     & i1,i2,i3,kd)+rxz43(i1,i2,i3)*vsbmr4(i1,i2,i3,kd)+sxz43(i1,i2,
     & i3)*vsbms4(i1,i2,i3,kd)+txz43(i1,i2,i3)*vsbmt4(i1,i2,i3,kd)
      vsbmyz43(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*vsbmrr4(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*vsbmss4(i1,i2,i3,kd)+ty(i1,i2,i3)
     & *tz(i1,i2,i3)*vsbmtt4(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sy(i1,i2,i3))*vsbmrs4(i1,i2,i3,kd)+(ry(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*vsbmrt4(i1,i2,i3,kd)+(
     & sy(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*vsbmst4(
     & i1,i2,i3,kd)+ryz43(i1,i2,i3)*vsbmr4(i1,i2,i3,kd)+syz43(i1,i2,
     & i3)*vsbms4(i1,i2,i3,kd)+tyz43(i1,i2,i3)*vsbmt4(i1,i2,i3,kd)
      vsbmlaplacian43(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*vsbmrr4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2+sz(i1,i2,i3)**2)*vsbmss4(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+
     & ty(i1,i2,i3)**2+tz(i1,i2,i3)**2)*vsbmtt4(i1,i2,i3,kd)+2.*(rx(
     & i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*
     & sz(i1,i2,i3))*vsbmrs4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,
     & i3)+ ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*
     & vsbmrt4(i1,i2,i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,
     & i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3))*vsbmst4(i1,i2,i3,
     & kd)+(rxx43(i1,i2,i3)+ryy43(i1,i2,i3)+rzz43(i1,i2,i3))*vsbmr4(
     & i1,i2,i3,kd)+(sxx43(i1,i2,i3)+syy43(i1,i2,i3)+szz43(i1,i2,i3))*
     & vsbms4(i1,i2,i3,kd)+(txx43(i1,i2,i3)+tyy43(i1,i2,i3)+tzz43(i1,
     & i2,i3))*vsbmt4(i1,i2,i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      vsbmx43r(i1,i2,i3,kd)=(8.*(vsbm(i1+1,i2,i3,kd)-vsbm(i1-1,i2,i3,
     & kd))-(vsbm(i1+2,i2,i3,kd)-vsbm(i1-2,i2,i3,kd)))*h41(0)
      vsbmy43r(i1,i2,i3,kd)=(8.*(vsbm(i1,i2+1,i3,kd)-vsbm(i1,i2-1,i3,
     & kd))-(vsbm(i1,i2+2,i3,kd)-vsbm(i1,i2-2,i3,kd)))*h41(1)
      vsbmz43r(i1,i2,i3,kd)=(8.*(vsbm(i1,i2,i3+1,kd)-vsbm(i1,i2,i3-1,
     & kd))-(vsbm(i1,i2,i3+2,kd)-vsbm(i1,i2,i3-2,kd)))*h41(2)
      vsbmxx43r(i1,i2,i3,kd)=( -30.*vsbm(i1,i2,i3,kd)+16.*(vsbm(i1+1,
     & i2,i3,kd)+vsbm(i1-1,i2,i3,kd))-(vsbm(i1+2,i2,i3,kd)+vsbm(i1-2,
     & i2,i3,kd)) )*h42(0)
      vsbmyy43r(i1,i2,i3,kd)=( -30.*vsbm(i1,i2,i3,kd)+16.*(vsbm(i1,i2+
     & 1,i3,kd)+vsbm(i1,i2-1,i3,kd))-(vsbm(i1,i2+2,i3,kd)+vsbm(i1,i2-
     & 2,i3,kd)) )*h42(1)
      vsbmzz43r(i1,i2,i3,kd)=( -30.*vsbm(i1,i2,i3,kd)+16.*(vsbm(i1,i2,
     & i3+1,kd)+vsbm(i1,i2,i3-1,kd))-(vsbm(i1,i2,i3+2,kd)+vsbm(i1,i2,
     & i3-2,kd)) )*h42(2)
      vsbmxy43r(i1,i2,i3,kd)=( (vsbm(i1+2,i2+2,i3,kd)-vsbm(i1-2,i2+2,
     & i3,kd)- vsbm(i1+2,i2-2,i3,kd)+vsbm(i1-2,i2-2,i3,kd)) +8.*(vsbm(
     & i1-1,i2+2,i3,kd)-vsbm(i1-1,i2-2,i3,kd)-vsbm(i1+1,i2+2,i3,kd)+
     & vsbm(i1+1,i2-2,i3,kd) +vsbm(i1+2,i2-1,i3,kd)-vsbm(i1-2,i2-1,i3,
     & kd)-vsbm(i1+2,i2+1,i3,kd)+vsbm(i1-2,i2+1,i3,kd))+64.*(vsbm(i1+
     & 1,i2+1,i3,kd)-vsbm(i1-1,i2+1,i3,kd)- vsbm(i1+1,i2-1,i3,kd)+
     & vsbm(i1-1,i2-1,i3,kd)))*(h41(0)*h41(1))
      vsbmxz43r(i1,i2,i3,kd)=( (vsbm(i1+2,i2,i3+2,kd)-vsbm(i1-2,i2,i3+
     & 2,kd)-vsbm(i1+2,i2,i3-2,kd)+vsbm(i1-2,i2,i3-2,kd)) +8.*(vsbm(
     & i1-1,i2,i3+2,kd)-vsbm(i1-1,i2,i3-2,kd)-vsbm(i1+1,i2,i3+2,kd)+
     & vsbm(i1+1,i2,i3-2,kd) +vsbm(i1+2,i2,i3-1,kd)-vsbm(i1-2,i2,i3-1,
     & kd)- vsbm(i1+2,i2,i3+1,kd)+vsbm(i1-2,i2,i3+1,kd)) +64.*(vsbm(
     & i1+1,i2,i3+1,kd)-vsbm(i1-1,i2,i3+1,kd)-vsbm(i1+1,i2,i3-1,kd)+
     & vsbm(i1-1,i2,i3-1,kd)) )*(h41(0)*h41(2))
      vsbmyz43r(i1,i2,i3,kd)=( (vsbm(i1,i2+2,i3+2,kd)-vsbm(i1,i2-2,i3+
     & 2,kd)-vsbm(i1,i2+2,i3-2,kd)+vsbm(i1,i2-2,i3-2,kd)) +8.*(vsbm(
     & i1,i2-1,i3+2,kd)-vsbm(i1,i2-1,i3-2,kd)-vsbm(i1,i2+1,i3+2,kd)+
     & vsbm(i1,i2+1,i3-2,kd) +vsbm(i1,i2+2,i3-1,kd)-vsbm(i1,i2-2,i3-1,
     & kd)-vsbm(i1,i2+2,i3+1,kd)+vsbm(i1,i2-2,i3+1,kd)) +64.*(vsbm(i1,
     & i2+1,i3+1,kd)-vsbm(i1,i2-1,i3+1,kd)-vsbm(i1,i2+1,i3-1,kd)+vsbm(
     & i1,i2-1,i3-1,kd)) )*(h41(1)*h41(2))
      vsbmx41r(i1,i2,i3,kd)= vsbmx43r(i1,i2,i3,kd)
      vsbmy41r(i1,i2,i3,kd)= vsbmy43r(i1,i2,i3,kd)
      vsbmz41r(i1,i2,i3,kd)= vsbmz43r(i1,i2,i3,kd)
      vsbmxx41r(i1,i2,i3,kd)= vsbmxx43r(i1,i2,i3,kd)
      vsbmyy41r(i1,i2,i3,kd)= vsbmyy43r(i1,i2,i3,kd)
      vsbmzz41r(i1,i2,i3,kd)= vsbmzz43r(i1,i2,i3,kd)
      vsbmxy41r(i1,i2,i3,kd)= vsbmxy43r(i1,i2,i3,kd)
      vsbmxz41r(i1,i2,i3,kd)= vsbmxz43r(i1,i2,i3,kd)
      vsbmyz41r(i1,i2,i3,kd)= vsbmyz43r(i1,i2,i3,kd)
      vsbmlaplacian41r(i1,i2,i3,kd)=vsbmxx43r(i1,i2,i3,kd)
      vsbmx42r(i1,i2,i3,kd)= vsbmx43r(i1,i2,i3,kd)
      vsbmy42r(i1,i2,i3,kd)= vsbmy43r(i1,i2,i3,kd)
      vsbmz42r(i1,i2,i3,kd)= vsbmz43r(i1,i2,i3,kd)
      vsbmxx42r(i1,i2,i3,kd)= vsbmxx43r(i1,i2,i3,kd)
      vsbmyy42r(i1,i2,i3,kd)= vsbmyy43r(i1,i2,i3,kd)
      vsbmzz42r(i1,i2,i3,kd)= vsbmzz43r(i1,i2,i3,kd)
      vsbmxy42r(i1,i2,i3,kd)= vsbmxy43r(i1,i2,i3,kd)
      vsbmxz42r(i1,i2,i3,kd)= vsbmxz43r(i1,i2,i3,kd)
      vsbmyz42r(i1,i2,i3,kd)= vsbmyz43r(i1,i2,i3,kd)
      vsbmlaplacian42r(i1,i2,i3,kd)=vsbmxx43r(i1,i2,i3,kd)+vsbmyy43r(
     & i1,i2,i3,kd)
      vsbmlaplacian43r(i1,i2,i3,kd)=vsbmxx43r(i1,i2,i3,kd)+vsbmyy43r(
     & i1,i2,i3,kd)+vsbmzz43r(i1,i2,i3,kd)
      wsbr4(i1,i2,i3,kd)=(8.*(wsb(i1+1,i2,i3,kd)-wsb(i1-1,i2,i3,kd))-(
     & wsb(i1+2,i2,i3,kd)-wsb(i1-2,i2,i3,kd)))*d14(0)
      wsbs4(i1,i2,i3,kd)=(8.*(wsb(i1,i2+1,i3,kd)-wsb(i1,i2-1,i3,kd))-(
     & wsb(i1,i2+2,i3,kd)-wsb(i1,i2-2,i3,kd)))*d14(1)
      wsbt4(i1,i2,i3,kd)=(8.*(wsb(i1,i2,i3+1,kd)-wsb(i1,i2,i3-1,kd))-(
     & wsb(i1,i2,i3+2,kd)-wsb(i1,i2,i3-2,kd)))*d14(2)
      wsbrr4(i1,i2,i3,kd)=(-30.*wsb(i1,i2,i3,kd)+16.*(wsb(i1+1,i2,i3,
     & kd)+wsb(i1-1,i2,i3,kd))-(wsb(i1+2,i2,i3,kd)+wsb(i1-2,i2,i3,kd))
     &  )*d24(0)
      wsbss4(i1,i2,i3,kd)=(-30.*wsb(i1,i2,i3,kd)+16.*(wsb(i1,i2+1,i3,
     & kd)+wsb(i1,i2-1,i3,kd))-(wsb(i1,i2+2,i3,kd)+wsb(i1,i2-2,i3,kd))
     &  )*d24(1)
      wsbtt4(i1,i2,i3,kd)=(-30.*wsb(i1,i2,i3,kd)+16.*(wsb(i1,i2,i3+1,
     & kd)+wsb(i1,i2,i3-1,kd))-(wsb(i1,i2,i3+2,kd)+wsb(i1,i2,i3-2,kd))
     &  )*d24(2)
      wsbrs4(i1,i2,i3,kd)=(8.*(wsbr4(i1,i2+1,i3,kd)-wsbr4(i1,i2-1,i3,
     & kd))-(wsbr4(i1,i2+2,i3,kd)-wsbr4(i1,i2-2,i3,kd)))*d14(1)
      wsbrt4(i1,i2,i3,kd)=(8.*(wsbr4(i1,i2,i3+1,kd)-wsbr4(i1,i2,i3-1,
     & kd))-(wsbr4(i1,i2,i3+2,kd)-wsbr4(i1,i2,i3-2,kd)))*d14(2)
      wsbst4(i1,i2,i3,kd)=(8.*(wsbs4(i1,i2,i3+1,kd)-wsbs4(i1,i2,i3-1,
     & kd))-(wsbs4(i1,i2,i3+2,kd)-wsbs4(i1,i2,i3-2,kd)))*d14(2)
      wsbx41(i1,i2,i3,kd)= rx(i1,i2,i3)*wsbr4(i1,i2,i3,kd)
      wsby41(i1,i2,i3,kd)=0
      wsbz41(i1,i2,i3,kd)=0
      wsbx42(i1,i2,i3,kd)= rx(i1,i2,i3)*wsbr4(i1,i2,i3,kd)+sx(i1,i2,i3)
     & *wsbs4(i1,i2,i3,kd)
      wsby42(i1,i2,i3,kd)= ry(i1,i2,i3)*wsbr4(i1,i2,i3,kd)+sy(i1,i2,i3)
     & *wsbs4(i1,i2,i3,kd)
      wsbz42(i1,i2,i3,kd)=0
      wsbx43(i1,i2,i3,kd)=rx(i1,i2,i3)*wsbr4(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & wsbs4(i1,i2,i3,kd)+tx(i1,i2,i3)*wsbt4(i1,i2,i3,kd)
      wsby43(i1,i2,i3,kd)=ry(i1,i2,i3)*wsbr4(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & wsbs4(i1,i2,i3,kd)+ty(i1,i2,i3)*wsbt4(i1,i2,i3,kd)
      wsbz43(i1,i2,i3,kd)=rz(i1,i2,i3)*wsbr4(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & wsbs4(i1,i2,i3,kd)+tz(i1,i2,i3)*wsbt4(i1,i2,i3,kd)
      wsbxx41(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wsbrr4(i1,i2,i3,kd)+(
     & rxx42(i1,i2,i3))*wsbr4(i1,i2,i3,kd)
      wsbyy41(i1,i2,i3,kd)=0
      wsbxy41(i1,i2,i3,kd)=0
      wsbxz41(i1,i2,i3,kd)=0
      wsbyz41(i1,i2,i3,kd)=0
      wsbzz41(i1,i2,i3,kd)=0
      wsblaplacian41(i1,i2,i3,kd)=wsbxx41(i1,i2,i3,kd)
      wsbxx42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wsbrr4(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*wsbrs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*wsbss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3))*wsbr4(i1,i2,i3,kd)+(
     & sxx42(i1,i2,i3))*wsbs4(i1,i2,i3,kd)
      wsbyy42(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*wsbrr4(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*wsbrs4(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*wsbss4(i1,i2,i3,kd)+(ryy42(i1,i2,i3))*wsbr4(i1,i2,i3,kd)+(
     & syy42(i1,i2,i3))*wsbs4(i1,i2,i3,kd)
      wsbxy42(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wsbrr4(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & wsbrs4(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wsbss4(i1,i2,i3,
     & kd)+rxy42(i1,i2,i3)*wsbr4(i1,i2,i3,kd)+sxy42(i1,i2,i3)*wsbs4(
     & i1,i2,i3,kd)
      wsbxz42(i1,i2,i3,kd)=0
      wsbyz42(i1,i2,i3,kd)=0
      wsbzz42(i1,i2,i3,kd)=0
      wsblaplacian42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & wsbrr4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)
     & *sy(i1,i2,i3))*wsbrs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2)*wsbss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3)+ryy42(i1,i2,i3))*
     & wsbr4(i1,i2,i3,kd)+(sxx42(i1,i2,i3)+syy42(i1,i2,i3))*wsbs4(i1,
     & i2,i3,kd)
      wsbxx43(i1,i2,i3,kd)=rx(i1,i2,i3)**2*wsbrr4(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*wsbss4(i1,i2,i3,kd)+tx(i1,i2,i3)**2*wsbtt4(i1,i2,i3,
     & kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*wsbrs4(i1,i2,i3,kd)+2.*rx(i1,
     & i2,i3)*tx(i1,i2,i3)*wsbrt4(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,
     & i2,i3)*wsbst4(i1,i2,i3,kd)+rxx43(i1,i2,i3)*wsbr4(i1,i2,i3,kd)+
     & sxx43(i1,i2,i3)*wsbs4(i1,i2,i3,kd)+txx43(i1,i2,i3)*wsbt4(i1,i2,
     & i3,kd)
      wsbyy43(i1,i2,i3,kd)=ry(i1,i2,i3)**2*wsbrr4(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*wsbss4(i1,i2,i3,kd)+ty(i1,i2,i3)**2*wsbtt4(i1,i2,i3,
     & kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*wsbrs4(i1,i2,i3,kd)+2.*ry(i1,
     & i2,i3)*ty(i1,i2,i3)*wsbrt4(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,
     & i2,i3)*wsbst4(i1,i2,i3,kd)+ryy43(i1,i2,i3)*wsbr4(i1,i2,i3,kd)+
     & syy43(i1,i2,i3)*wsbs4(i1,i2,i3,kd)+tyy43(i1,i2,i3)*wsbt4(i1,i2,
     & i3,kd)
      wsbzz43(i1,i2,i3,kd)=rz(i1,i2,i3)**2*wsbrr4(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*wsbss4(i1,i2,i3,kd)+tz(i1,i2,i3)**2*wsbtt4(i1,i2,i3,
     & kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*wsbrs4(i1,i2,i3,kd)+2.*rz(i1,
     & i2,i3)*tz(i1,i2,i3)*wsbrt4(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,
     & i2,i3)*wsbst4(i1,i2,i3,kd)+rzz43(i1,i2,i3)*wsbr4(i1,i2,i3,kd)+
     & szz43(i1,i2,i3)*wsbs4(i1,i2,i3,kd)+tzz43(i1,i2,i3)*wsbt4(i1,i2,
     & i3,kd)
      wsbxy43(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wsbrr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wsbss4(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & ty(i1,i2,i3)*wsbtt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(
     & i1,i2,i3)*sx(i1,i2,i3))*wsbrs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(
     & i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*wsbrt4(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*wsbst4(i1,i2,
     & i3,kd)+rxy43(i1,i2,i3)*wsbr4(i1,i2,i3,kd)+sxy43(i1,i2,i3)*
     & wsbs4(i1,i2,i3,kd)+txy43(i1,i2,i3)*wsbt4(i1,i2,i3,kd)
      wsbxz43(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*wsbrr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*wsbss4(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & tz(i1,i2,i3)*wsbtt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sx(i1,i2,i3))*wsbrs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*wsbrt4(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*wsbst4(i1,i2,
     & i3,kd)+rxz43(i1,i2,i3)*wsbr4(i1,i2,i3,kd)+sxz43(i1,i2,i3)*
     & wsbs4(i1,i2,i3,kd)+txz43(i1,i2,i3)*wsbt4(i1,i2,i3,kd)
      wsbyz43(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*wsbrr4(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*wsbss4(i1,i2,i3,kd)+ty(i1,i2,i3)*
     & tz(i1,i2,i3)*wsbtt4(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sy(i1,i2,i3))*wsbrs4(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*wsbrt4(i1,i2,i3,kd)+(sy(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*wsbst4(i1,i2,
     & i3,kd)+ryz43(i1,i2,i3)*wsbr4(i1,i2,i3,kd)+syz43(i1,i2,i3)*
     & wsbs4(i1,i2,i3,kd)+tyz43(i1,i2,i3)*wsbt4(i1,i2,i3,kd)
      wsblaplacian43(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*wsbrr4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)
     & **2+sz(i1,i2,i3)**2)*wsbss4(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(
     & i1,i2,i3)**2+tz(i1,i2,i3)**2)*wsbtt4(i1,i2,i3,kd)+2.*(rx(i1,i2,
     & i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,
     & i2,i3))*wsbrs4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(
     & i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*wsbrt4(i1,i2,
     & i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)
     & +sz(i1,i2,i3)*tz(i1,i2,i3))*wsbst4(i1,i2,i3,kd)+(rxx43(i1,i2,
     & i3)+ryy43(i1,i2,i3)+rzz43(i1,i2,i3))*wsbr4(i1,i2,i3,kd)+(sxx43(
     & i1,i2,i3)+syy43(i1,i2,i3)+szz43(i1,i2,i3))*wsbs4(i1,i2,i3,kd)+(
     & txx43(i1,i2,i3)+tyy43(i1,i2,i3)+tzz43(i1,i2,i3))*wsbt4(i1,i2,
     & i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      wsbx43r(i1,i2,i3,kd)=(8.*(wsb(i1+1,i2,i3,kd)-wsb(i1-1,i2,i3,kd))-
     & (wsb(i1+2,i2,i3,kd)-wsb(i1-2,i2,i3,kd)))*h41(0)
      wsby43r(i1,i2,i3,kd)=(8.*(wsb(i1,i2+1,i3,kd)-wsb(i1,i2-1,i3,kd))-
     & (wsb(i1,i2+2,i3,kd)-wsb(i1,i2-2,i3,kd)))*h41(1)
      wsbz43r(i1,i2,i3,kd)=(8.*(wsb(i1,i2,i3+1,kd)-wsb(i1,i2,i3-1,kd))-
     & (wsb(i1,i2,i3+2,kd)-wsb(i1,i2,i3-2,kd)))*h41(2)
      wsbxx43r(i1,i2,i3,kd)=( -30.*wsb(i1,i2,i3,kd)+16.*(wsb(i1+1,i2,
     & i3,kd)+wsb(i1-1,i2,i3,kd))-(wsb(i1+2,i2,i3,kd)+wsb(i1-2,i2,i3,
     & kd)) )*h42(0)
      wsbyy43r(i1,i2,i3,kd)=( -30.*wsb(i1,i2,i3,kd)+16.*(wsb(i1,i2+1,
     & i3,kd)+wsb(i1,i2-1,i3,kd))-(wsb(i1,i2+2,i3,kd)+wsb(i1,i2-2,i3,
     & kd)) )*h42(1)
      wsbzz43r(i1,i2,i3,kd)=( -30.*wsb(i1,i2,i3,kd)+16.*(wsb(i1,i2,i3+
     & 1,kd)+wsb(i1,i2,i3-1,kd))-(wsb(i1,i2,i3+2,kd)+wsb(i1,i2,i3-2,
     & kd)) )*h42(2)
      wsbxy43r(i1,i2,i3,kd)=( (wsb(i1+2,i2+2,i3,kd)-wsb(i1-2,i2+2,i3,
     & kd)- wsb(i1+2,i2-2,i3,kd)+wsb(i1-2,i2-2,i3,kd)) +8.*(wsb(i1-1,
     & i2+2,i3,kd)-wsb(i1-1,i2-2,i3,kd)-wsb(i1+1,i2+2,i3,kd)+wsb(i1+1,
     & i2-2,i3,kd) +wsb(i1+2,i2-1,i3,kd)-wsb(i1-2,i2-1,i3,kd)-wsb(i1+
     & 2,i2+1,i3,kd)+wsb(i1-2,i2+1,i3,kd))+64.*(wsb(i1+1,i2+1,i3,kd)-
     & wsb(i1-1,i2+1,i3,kd)- wsb(i1+1,i2-1,i3,kd)+wsb(i1-1,i2-1,i3,kd)
     & ))*(h41(0)*h41(1))
      wsbxz43r(i1,i2,i3,kd)=( (wsb(i1+2,i2,i3+2,kd)-wsb(i1-2,i2,i3+2,
     & kd)-wsb(i1+2,i2,i3-2,kd)+wsb(i1-2,i2,i3-2,kd)) +8.*(wsb(i1-1,
     & i2,i3+2,kd)-wsb(i1-1,i2,i3-2,kd)-wsb(i1+1,i2,i3+2,kd)+wsb(i1+1,
     & i2,i3-2,kd) +wsb(i1+2,i2,i3-1,kd)-wsb(i1-2,i2,i3-1,kd)- wsb(i1+
     & 2,i2,i3+1,kd)+wsb(i1-2,i2,i3+1,kd)) +64.*(wsb(i1+1,i2,i3+1,kd)-
     & wsb(i1-1,i2,i3+1,kd)-wsb(i1+1,i2,i3-1,kd)+wsb(i1-1,i2,i3-1,kd))
     &  )*(h41(0)*h41(2))
      wsbyz43r(i1,i2,i3,kd)=( (wsb(i1,i2+2,i3+2,kd)-wsb(i1,i2-2,i3+2,
     & kd)-wsb(i1,i2+2,i3-2,kd)+wsb(i1,i2-2,i3-2,kd)) +8.*(wsb(i1,i2-
     & 1,i3+2,kd)-wsb(i1,i2-1,i3-2,kd)-wsb(i1,i2+1,i3+2,kd)+wsb(i1,i2+
     & 1,i3-2,kd) +wsb(i1,i2+2,i3-1,kd)-wsb(i1,i2-2,i3-1,kd)-wsb(i1,
     & i2+2,i3+1,kd)+wsb(i1,i2-2,i3+1,kd)) +64.*(wsb(i1,i2+1,i3+1,kd)-
     & wsb(i1,i2-1,i3+1,kd)-wsb(i1,i2+1,i3-1,kd)+wsb(i1,i2-1,i3-1,kd))
     &  )*(h41(1)*h41(2))
      wsbx41r(i1,i2,i3,kd)= wsbx43r(i1,i2,i3,kd)
      wsby41r(i1,i2,i3,kd)= wsby43r(i1,i2,i3,kd)
      wsbz41r(i1,i2,i3,kd)= wsbz43r(i1,i2,i3,kd)
      wsbxx41r(i1,i2,i3,kd)= wsbxx43r(i1,i2,i3,kd)
      wsbyy41r(i1,i2,i3,kd)= wsbyy43r(i1,i2,i3,kd)
      wsbzz41r(i1,i2,i3,kd)= wsbzz43r(i1,i2,i3,kd)
      wsbxy41r(i1,i2,i3,kd)= wsbxy43r(i1,i2,i3,kd)
      wsbxz41r(i1,i2,i3,kd)= wsbxz43r(i1,i2,i3,kd)
      wsbyz41r(i1,i2,i3,kd)= wsbyz43r(i1,i2,i3,kd)
      wsblaplacian41r(i1,i2,i3,kd)=wsbxx43r(i1,i2,i3,kd)
      wsbx42r(i1,i2,i3,kd)= wsbx43r(i1,i2,i3,kd)
      wsby42r(i1,i2,i3,kd)= wsby43r(i1,i2,i3,kd)
      wsbz42r(i1,i2,i3,kd)= wsbz43r(i1,i2,i3,kd)
      wsbxx42r(i1,i2,i3,kd)= wsbxx43r(i1,i2,i3,kd)
      wsbyy42r(i1,i2,i3,kd)= wsbyy43r(i1,i2,i3,kd)
      wsbzz42r(i1,i2,i3,kd)= wsbzz43r(i1,i2,i3,kd)
      wsbxy42r(i1,i2,i3,kd)= wsbxy43r(i1,i2,i3,kd)
      wsbxz42r(i1,i2,i3,kd)= wsbxz43r(i1,i2,i3,kd)
      wsbyz42r(i1,i2,i3,kd)= wsbyz43r(i1,i2,i3,kd)
      wsblaplacian42r(i1,i2,i3,kd)=wsbxx43r(i1,i2,i3,kd)+wsbyy43r(i1,
     & i2,i3,kd)
      wsblaplacian43r(i1,i2,i3,kd)=wsbxx43r(i1,i2,i3,kd)+wsbyy43r(i1,
     & i2,i3,kd)+wsbzz43r(i1,i2,i3,kd)
      wsbmr4(i1,i2,i3,kd)=(8.*(wsbm(i1+1,i2,i3,kd)-wsbm(i1-1,i2,i3,kd))
     & -(wsbm(i1+2,i2,i3,kd)-wsbm(i1-2,i2,i3,kd)))*d14(0)
      wsbms4(i1,i2,i3,kd)=(8.*(wsbm(i1,i2+1,i3,kd)-wsbm(i1,i2-1,i3,kd))
     & -(wsbm(i1,i2+2,i3,kd)-wsbm(i1,i2-2,i3,kd)))*d14(1)
      wsbmt4(i1,i2,i3,kd)=(8.*(wsbm(i1,i2,i3+1,kd)-wsbm(i1,i2,i3-1,kd))
     & -(wsbm(i1,i2,i3+2,kd)-wsbm(i1,i2,i3-2,kd)))*d14(2)
      wsbmrr4(i1,i2,i3,kd)=(-30.*wsbm(i1,i2,i3,kd)+16.*(wsbm(i1+1,i2,
     & i3,kd)+wsbm(i1-1,i2,i3,kd))-(wsbm(i1+2,i2,i3,kd)+wsbm(i1-2,i2,
     & i3,kd)) )*d24(0)
      wsbmss4(i1,i2,i3,kd)=(-30.*wsbm(i1,i2,i3,kd)+16.*(wsbm(i1,i2+1,
     & i3,kd)+wsbm(i1,i2-1,i3,kd))-(wsbm(i1,i2+2,i3,kd)+wsbm(i1,i2-2,
     & i3,kd)) )*d24(1)
      wsbmtt4(i1,i2,i3,kd)=(-30.*wsbm(i1,i2,i3,kd)+16.*(wsbm(i1,i2,i3+
     & 1,kd)+wsbm(i1,i2,i3-1,kd))-(wsbm(i1,i2,i3+2,kd)+wsbm(i1,i2,i3-
     & 2,kd)) )*d24(2)
      wsbmrs4(i1,i2,i3,kd)=(8.*(wsbmr4(i1,i2+1,i3,kd)-wsbmr4(i1,i2-1,
     & i3,kd))-(wsbmr4(i1,i2+2,i3,kd)-wsbmr4(i1,i2-2,i3,kd)))*d14(1)
      wsbmrt4(i1,i2,i3,kd)=(8.*(wsbmr4(i1,i2,i3+1,kd)-wsbmr4(i1,i2,i3-
     & 1,kd))-(wsbmr4(i1,i2,i3+2,kd)-wsbmr4(i1,i2,i3-2,kd)))*d14(2)
      wsbmst4(i1,i2,i3,kd)=(8.*(wsbms4(i1,i2,i3+1,kd)-wsbms4(i1,i2,i3-
     & 1,kd))-(wsbms4(i1,i2,i3+2,kd)-wsbms4(i1,i2,i3-2,kd)))*d14(2)
      wsbmx41(i1,i2,i3,kd)= rx(i1,i2,i3)*wsbmr4(i1,i2,i3,kd)
      wsbmy41(i1,i2,i3,kd)=0
      wsbmz41(i1,i2,i3,kd)=0
      wsbmx42(i1,i2,i3,kd)= rx(i1,i2,i3)*wsbmr4(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*wsbms4(i1,i2,i3,kd)
      wsbmy42(i1,i2,i3,kd)= ry(i1,i2,i3)*wsbmr4(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*wsbms4(i1,i2,i3,kd)
      wsbmz42(i1,i2,i3,kd)=0
      wsbmx43(i1,i2,i3,kd)=rx(i1,i2,i3)*wsbmr4(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*wsbms4(i1,i2,i3,kd)+tx(i1,i2,i3)*wsbmt4(i1,i2,i3,kd)
      wsbmy43(i1,i2,i3,kd)=ry(i1,i2,i3)*wsbmr4(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*wsbms4(i1,i2,i3,kd)+ty(i1,i2,i3)*wsbmt4(i1,i2,i3,kd)
      wsbmz43(i1,i2,i3,kd)=rz(i1,i2,i3)*wsbmr4(i1,i2,i3,kd)+sz(i1,i2,
     & i3)*wsbms4(i1,i2,i3,kd)+tz(i1,i2,i3)*wsbmt4(i1,i2,i3,kd)
      wsbmxx41(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wsbmrr4(i1,i2,i3,kd)+(
     & rxx42(i1,i2,i3))*wsbmr4(i1,i2,i3,kd)
      wsbmyy41(i1,i2,i3,kd)=0
      wsbmxy41(i1,i2,i3,kd)=0
      wsbmxz41(i1,i2,i3,kd)=0
      wsbmyz41(i1,i2,i3,kd)=0
      wsbmzz41(i1,i2,i3,kd)=0
      wsbmlaplacian41(i1,i2,i3,kd)=wsbmxx41(i1,i2,i3,kd)
      wsbmxx42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wsbmrr4(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*wsbmrs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*wsbmss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3))*wsbmr4(i1,i2,i3,kd)+(
     & sxx42(i1,i2,i3))*wsbms4(i1,i2,i3,kd)
      wsbmyy42(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*wsbmrr4(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*wsbmrs4(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*wsbmss4(i1,i2,i3,kd)+(ryy42(i1,i2,i3))*wsbmr4(i1,i2,i3,kd)+(
     & syy42(i1,i2,i3))*wsbms4(i1,i2,i3,kd)
      wsbmxy42(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wsbmrr4(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & wsbmrs4(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wsbmss4(i1,i2,
     & i3,kd)+rxy42(i1,i2,i3)*wsbmr4(i1,i2,i3,kd)+sxy42(i1,i2,i3)*
     & wsbms4(i1,i2,i3,kd)
      wsbmxz42(i1,i2,i3,kd)=0
      wsbmyz42(i1,i2,i3,kd)=0
      wsbmzz42(i1,i2,i3,kd)=0
      wsbmlaplacian42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & wsbmrr4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,
     & i3)*sy(i1,i2,i3))*wsbmrs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,
     & i2,i3)**2)*wsbmss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3)+ryy42(i1,i2,
     & i3))*wsbmr4(i1,i2,i3,kd)+(sxx42(i1,i2,i3)+syy42(i1,i2,i3))*
     & wsbms4(i1,i2,i3,kd)
      wsbmxx43(i1,i2,i3,kd)=rx(i1,i2,i3)**2*wsbmrr4(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*wsbmss4(i1,i2,i3,kd)+tx(i1,i2,i3)**2*wsbmtt4(i1,i2,
     & i3,kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*wsbmrs4(i1,i2,i3,kd)+2.*rx(
     & i1,i2,i3)*tx(i1,i2,i3)*wsbmrt4(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(
     & i1,i2,i3)*wsbmst4(i1,i2,i3,kd)+rxx43(i1,i2,i3)*wsbmr4(i1,i2,i3,
     & kd)+sxx43(i1,i2,i3)*wsbms4(i1,i2,i3,kd)+txx43(i1,i2,i3)*wsbmt4(
     & i1,i2,i3,kd)
      wsbmyy43(i1,i2,i3,kd)=ry(i1,i2,i3)**2*wsbmrr4(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*wsbmss4(i1,i2,i3,kd)+ty(i1,i2,i3)**2*wsbmtt4(i1,i2,
     & i3,kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*wsbmrs4(i1,i2,i3,kd)+2.*ry(
     & i1,i2,i3)*ty(i1,i2,i3)*wsbmrt4(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(
     & i1,i2,i3)*wsbmst4(i1,i2,i3,kd)+ryy43(i1,i2,i3)*wsbmr4(i1,i2,i3,
     & kd)+syy43(i1,i2,i3)*wsbms4(i1,i2,i3,kd)+tyy43(i1,i2,i3)*wsbmt4(
     & i1,i2,i3,kd)
      wsbmzz43(i1,i2,i3,kd)=rz(i1,i2,i3)**2*wsbmrr4(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*wsbmss4(i1,i2,i3,kd)+tz(i1,i2,i3)**2*wsbmtt4(i1,i2,
     & i3,kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*wsbmrs4(i1,i2,i3,kd)+2.*rz(
     & i1,i2,i3)*tz(i1,i2,i3)*wsbmrt4(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(
     & i1,i2,i3)*wsbmst4(i1,i2,i3,kd)+rzz43(i1,i2,i3)*wsbmr4(i1,i2,i3,
     & kd)+szz43(i1,i2,i3)*wsbms4(i1,i2,i3,kd)+tzz43(i1,i2,i3)*wsbmt4(
     & i1,i2,i3,kd)
      wsbmxy43(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wsbmrr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wsbmss4(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *ty(i1,i2,i3)*wsbmtt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+
     & ry(i1,i2,i3)*sx(i1,i2,i3))*wsbmrs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & ty(i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*wsbmrt4(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*wsbmst4(
     & i1,i2,i3,kd)+rxy43(i1,i2,i3)*wsbmr4(i1,i2,i3,kd)+sxy43(i1,i2,
     & i3)*wsbms4(i1,i2,i3,kd)+txy43(i1,i2,i3)*wsbmt4(i1,i2,i3,kd)
      wsbmxz43(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*wsbmrr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*wsbmss4(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *tz(i1,i2,i3)*wsbmtt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sx(i1,i2,i3))*wsbmrs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*wsbmrt4(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*wsbmst4(
     & i1,i2,i3,kd)+rxz43(i1,i2,i3)*wsbmr4(i1,i2,i3,kd)+sxz43(i1,i2,
     & i3)*wsbms4(i1,i2,i3,kd)+txz43(i1,i2,i3)*wsbmt4(i1,i2,i3,kd)
      wsbmyz43(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*wsbmrr4(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*wsbmss4(i1,i2,i3,kd)+ty(i1,i2,i3)
     & *tz(i1,i2,i3)*wsbmtt4(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sy(i1,i2,i3))*wsbmrs4(i1,i2,i3,kd)+(ry(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*wsbmrt4(i1,i2,i3,kd)+(
     & sy(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*wsbmst4(
     & i1,i2,i3,kd)+ryz43(i1,i2,i3)*wsbmr4(i1,i2,i3,kd)+syz43(i1,i2,
     & i3)*wsbms4(i1,i2,i3,kd)+tyz43(i1,i2,i3)*wsbmt4(i1,i2,i3,kd)
      wsbmlaplacian43(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*wsbmrr4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2+sz(i1,i2,i3)**2)*wsbmss4(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+
     & ty(i1,i2,i3)**2+tz(i1,i2,i3)**2)*wsbmtt4(i1,i2,i3,kd)+2.*(rx(
     & i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*
     & sz(i1,i2,i3))*wsbmrs4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,
     & i3)+ ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*
     & wsbmrt4(i1,i2,i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,
     & i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3))*wsbmst4(i1,i2,i3,
     & kd)+(rxx43(i1,i2,i3)+ryy43(i1,i2,i3)+rzz43(i1,i2,i3))*wsbmr4(
     & i1,i2,i3,kd)+(sxx43(i1,i2,i3)+syy43(i1,i2,i3)+szz43(i1,i2,i3))*
     & wsbms4(i1,i2,i3,kd)+(txx43(i1,i2,i3)+tyy43(i1,i2,i3)+tzz43(i1,
     & i2,i3))*wsbmt4(i1,i2,i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      wsbmx43r(i1,i2,i3,kd)=(8.*(wsbm(i1+1,i2,i3,kd)-wsbm(i1-1,i2,i3,
     & kd))-(wsbm(i1+2,i2,i3,kd)-wsbm(i1-2,i2,i3,kd)))*h41(0)
      wsbmy43r(i1,i2,i3,kd)=(8.*(wsbm(i1,i2+1,i3,kd)-wsbm(i1,i2-1,i3,
     & kd))-(wsbm(i1,i2+2,i3,kd)-wsbm(i1,i2-2,i3,kd)))*h41(1)
      wsbmz43r(i1,i2,i3,kd)=(8.*(wsbm(i1,i2,i3+1,kd)-wsbm(i1,i2,i3-1,
     & kd))-(wsbm(i1,i2,i3+2,kd)-wsbm(i1,i2,i3-2,kd)))*h41(2)
      wsbmxx43r(i1,i2,i3,kd)=( -30.*wsbm(i1,i2,i3,kd)+16.*(wsbm(i1+1,
     & i2,i3,kd)+wsbm(i1-1,i2,i3,kd))-(wsbm(i1+2,i2,i3,kd)+wsbm(i1-2,
     & i2,i3,kd)) )*h42(0)
      wsbmyy43r(i1,i2,i3,kd)=( -30.*wsbm(i1,i2,i3,kd)+16.*(wsbm(i1,i2+
     & 1,i3,kd)+wsbm(i1,i2-1,i3,kd))-(wsbm(i1,i2+2,i3,kd)+wsbm(i1,i2-
     & 2,i3,kd)) )*h42(1)
      wsbmzz43r(i1,i2,i3,kd)=( -30.*wsbm(i1,i2,i3,kd)+16.*(wsbm(i1,i2,
     & i3+1,kd)+wsbm(i1,i2,i3-1,kd))-(wsbm(i1,i2,i3+2,kd)+wsbm(i1,i2,
     & i3-2,kd)) )*h42(2)
      wsbmxy43r(i1,i2,i3,kd)=( (wsbm(i1+2,i2+2,i3,kd)-wsbm(i1-2,i2+2,
     & i3,kd)- wsbm(i1+2,i2-2,i3,kd)+wsbm(i1-2,i2-2,i3,kd)) +8.*(wsbm(
     & i1-1,i2+2,i3,kd)-wsbm(i1-1,i2-2,i3,kd)-wsbm(i1+1,i2+2,i3,kd)+
     & wsbm(i1+1,i2-2,i3,kd) +wsbm(i1+2,i2-1,i3,kd)-wsbm(i1-2,i2-1,i3,
     & kd)-wsbm(i1+2,i2+1,i3,kd)+wsbm(i1-2,i2+1,i3,kd))+64.*(wsbm(i1+
     & 1,i2+1,i3,kd)-wsbm(i1-1,i2+1,i3,kd)- wsbm(i1+1,i2-1,i3,kd)+
     & wsbm(i1-1,i2-1,i3,kd)))*(h41(0)*h41(1))
      wsbmxz43r(i1,i2,i3,kd)=( (wsbm(i1+2,i2,i3+2,kd)-wsbm(i1-2,i2,i3+
     & 2,kd)-wsbm(i1+2,i2,i3-2,kd)+wsbm(i1-2,i2,i3-2,kd)) +8.*(wsbm(
     & i1-1,i2,i3+2,kd)-wsbm(i1-1,i2,i3-2,kd)-wsbm(i1+1,i2,i3+2,kd)+
     & wsbm(i1+1,i2,i3-2,kd) +wsbm(i1+2,i2,i3-1,kd)-wsbm(i1-2,i2,i3-1,
     & kd)- wsbm(i1+2,i2,i3+1,kd)+wsbm(i1-2,i2,i3+1,kd)) +64.*(wsbm(
     & i1+1,i2,i3+1,kd)-wsbm(i1-1,i2,i3+1,kd)-wsbm(i1+1,i2,i3-1,kd)+
     & wsbm(i1-1,i2,i3-1,kd)) )*(h41(0)*h41(2))
      wsbmyz43r(i1,i2,i3,kd)=( (wsbm(i1,i2+2,i3+2,kd)-wsbm(i1,i2-2,i3+
     & 2,kd)-wsbm(i1,i2+2,i3-2,kd)+wsbm(i1,i2-2,i3-2,kd)) +8.*(wsbm(
     & i1,i2-1,i3+2,kd)-wsbm(i1,i2-1,i3-2,kd)-wsbm(i1,i2+1,i3+2,kd)+
     & wsbm(i1,i2+1,i3-2,kd) +wsbm(i1,i2+2,i3-1,kd)-wsbm(i1,i2-2,i3-1,
     & kd)-wsbm(i1,i2+2,i3+1,kd)+wsbm(i1,i2-2,i3+1,kd)) +64.*(wsbm(i1,
     & i2+1,i3+1,kd)-wsbm(i1,i2-1,i3+1,kd)-wsbm(i1,i2+1,i3-1,kd)+wsbm(
     & i1,i2-1,i3-1,kd)) )*(h41(1)*h41(2))
      wsbmx41r(i1,i2,i3,kd)= wsbmx43r(i1,i2,i3,kd)
      wsbmy41r(i1,i2,i3,kd)= wsbmy43r(i1,i2,i3,kd)
      wsbmz41r(i1,i2,i3,kd)= wsbmz43r(i1,i2,i3,kd)
      wsbmxx41r(i1,i2,i3,kd)= wsbmxx43r(i1,i2,i3,kd)
      wsbmyy41r(i1,i2,i3,kd)= wsbmyy43r(i1,i2,i3,kd)
      wsbmzz41r(i1,i2,i3,kd)= wsbmzz43r(i1,i2,i3,kd)
      wsbmxy41r(i1,i2,i3,kd)= wsbmxy43r(i1,i2,i3,kd)
      wsbmxz41r(i1,i2,i3,kd)= wsbmxz43r(i1,i2,i3,kd)
      wsbmyz41r(i1,i2,i3,kd)= wsbmyz43r(i1,i2,i3,kd)
      wsbmlaplacian41r(i1,i2,i3,kd)=wsbmxx43r(i1,i2,i3,kd)
      wsbmx42r(i1,i2,i3,kd)= wsbmx43r(i1,i2,i3,kd)
      wsbmy42r(i1,i2,i3,kd)= wsbmy43r(i1,i2,i3,kd)
      wsbmz42r(i1,i2,i3,kd)= wsbmz43r(i1,i2,i3,kd)
      wsbmxx42r(i1,i2,i3,kd)= wsbmxx43r(i1,i2,i3,kd)
      wsbmyy42r(i1,i2,i3,kd)= wsbmyy43r(i1,i2,i3,kd)
      wsbmzz42r(i1,i2,i3,kd)= wsbmzz43r(i1,i2,i3,kd)
      wsbmxy42r(i1,i2,i3,kd)= wsbmxy43r(i1,i2,i3,kd)
      wsbmxz42r(i1,i2,i3,kd)= wsbmxz43r(i1,i2,i3,kd)
      wsbmyz42r(i1,i2,i3,kd)= wsbmyz43r(i1,i2,i3,kd)
      wsbmlaplacian42r(i1,i2,i3,kd)=wsbmxx43r(i1,i2,i3,kd)+wsbmyy43r(
     & i1,i2,i3,kd)
      wsbmlaplacian43r(i1,i2,i3,kd)=wsbmxx43r(i1,i2,i3,kd)+wsbmyy43r(
     & i1,i2,i3,kd)+wsbmzz43r(i1,i2,i3,kd)

      vtar4(i1,i2,i3,kd)=(8.*(vta(i1+1,i2,i3,kd)-vta(i1-1,i2,i3,kd))-(
     & vta(i1+2,i2,i3,kd)-vta(i1-2,i2,i3,kd)))*d14(0)
      vtas4(i1,i2,i3,kd)=(8.*(vta(i1,i2+1,i3,kd)-vta(i1,i2-1,i3,kd))-(
     & vta(i1,i2+2,i3,kd)-vta(i1,i2-2,i3,kd)))*d14(1)
      vtat4(i1,i2,i3,kd)=(8.*(vta(i1,i2,i3+1,kd)-vta(i1,i2,i3-1,kd))-(
     & vta(i1,i2,i3+2,kd)-vta(i1,i2,i3-2,kd)))*d14(2)
      vtarr4(i1,i2,i3,kd)=(-30.*vta(i1,i2,i3,kd)+16.*(vta(i1+1,i2,i3,
     & kd)+vta(i1-1,i2,i3,kd))-(vta(i1+2,i2,i3,kd)+vta(i1-2,i2,i3,kd))
     &  )*d24(0)
      vtass4(i1,i2,i3,kd)=(-30.*vta(i1,i2,i3,kd)+16.*(vta(i1,i2+1,i3,
     & kd)+vta(i1,i2-1,i3,kd))-(vta(i1,i2+2,i3,kd)+vta(i1,i2-2,i3,kd))
     &  )*d24(1)
      vtatt4(i1,i2,i3,kd)=(-30.*vta(i1,i2,i3,kd)+16.*(vta(i1,i2,i3+1,
     & kd)+vta(i1,i2,i3-1,kd))-(vta(i1,i2,i3+2,kd)+vta(i1,i2,i3-2,kd))
     &  )*d24(2)
      vtars4(i1,i2,i3,kd)=(8.*(vtar4(i1,i2+1,i3,kd)-vtar4(i1,i2-1,i3,
     & kd))-(vtar4(i1,i2+2,i3,kd)-vtar4(i1,i2-2,i3,kd)))*d14(1)
      vtart4(i1,i2,i3,kd)=(8.*(vtar4(i1,i2,i3+1,kd)-vtar4(i1,i2,i3-1,
     & kd))-(vtar4(i1,i2,i3+2,kd)-vtar4(i1,i2,i3-2,kd)))*d14(2)
      vtast4(i1,i2,i3,kd)=(8.*(vtas4(i1,i2,i3+1,kd)-vtas4(i1,i2,i3-1,
     & kd))-(vtas4(i1,i2,i3+2,kd)-vtas4(i1,i2,i3-2,kd)))*d14(2)
      vtax41(i1,i2,i3,kd)= rx(i1,i2,i3)*vtar4(i1,i2,i3,kd)
      vtay41(i1,i2,i3,kd)=0
      vtaz41(i1,i2,i3,kd)=0
      vtax42(i1,i2,i3,kd)= rx(i1,i2,i3)*vtar4(i1,i2,i3,kd)+sx(i1,i2,i3)
     & *vtas4(i1,i2,i3,kd)
      vtay42(i1,i2,i3,kd)= ry(i1,i2,i3)*vtar4(i1,i2,i3,kd)+sy(i1,i2,i3)
     & *vtas4(i1,i2,i3,kd)
      vtaz42(i1,i2,i3,kd)=0
      vtax43(i1,i2,i3,kd)=rx(i1,i2,i3)*vtar4(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & vtas4(i1,i2,i3,kd)+tx(i1,i2,i3)*vtat4(i1,i2,i3,kd)
      vtay43(i1,i2,i3,kd)=ry(i1,i2,i3)*vtar4(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & vtas4(i1,i2,i3,kd)+ty(i1,i2,i3)*vtat4(i1,i2,i3,kd)
      vtaz43(i1,i2,i3,kd)=rz(i1,i2,i3)*vtar4(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & vtas4(i1,i2,i3,kd)+tz(i1,i2,i3)*vtat4(i1,i2,i3,kd)
      vtaxx41(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vtarr4(i1,i2,i3,kd)+(
     & rxx42(i1,i2,i3))*vtar4(i1,i2,i3,kd)
      vtayy41(i1,i2,i3,kd)=0
      vtaxy41(i1,i2,i3,kd)=0
      vtaxz41(i1,i2,i3,kd)=0
      vtayz41(i1,i2,i3,kd)=0
      vtazz41(i1,i2,i3,kd)=0
      vtalaplacian41(i1,i2,i3,kd)=vtaxx41(i1,i2,i3,kd)
      vtaxx42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vtarr4(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*vtars4(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*vtass4(i1,i2,i3,kd)+(rxx42(i1,i2,i3))*vtar4(i1,i2,i3,kd)+(
     & sxx42(i1,i2,i3))*vtas4(i1,i2,i3,kd)
      vtayy42(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*vtarr4(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*vtars4(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*vtass4(i1,i2,i3,kd)+(ryy42(i1,i2,i3))*vtar4(i1,i2,i3,kd)+(
     & syy42(i1,i2,i3))*vtas4(i1,i2,i3,kd)
      vtaxy42(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vtarr4(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & vtars4(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vtass4(i1,i2,i3,
     & kd)+rxy42(i1,i2,i3)*vtar4(i1,i2,i3,kd)+sxy42(i1,i2,i3)*vtas4(
     & i1,i2,i3,kd)
      vtaxz42(i1,i2,i3,kd)=0
      vtayz42(i1,i2,i3,kd)=0
      vtazz42(i1,i2,i3,kd)=0
      vtalaplacian42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & vtarr4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)
     & *sy(i1,i2,i3))*vtars4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2)*vtass4(i1,i2,i3,kd)+(rxx42(i1,i2,i3)+ryy42(i1,i2,i3))*
     & vtar4(i1,i2,i3,kd)+(sxx42(i1,i2,i3)+syy42(i1,i2,i3))*vtas4(i1,
     & i2,i3,kd)
      vtaxx43(i1,i2,i3,kd)=rx(i1,i2,i3)**2*vtarr4(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*vtass4(i1,i2,i3,kd)+tx(i1,i2,i3)**2*vtatt4(i1,i2,i3,
     & kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*vtars4(i1,i2,i3,kd)+2.*rx(i1,
     & i2,i3)*tx(i1,i2,i3)*vtart4(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,
     & i2,i3)*vtast4(i1,i2,i3,kd)+rxx43(i1,i2,i3)*vtar4(i1,i2,i3,kd)+
     & sxx43(i1,i2,i3)*vtas4(i1,i2,i3,kd)+txx43(i1,i2,i3)*vtat4(i1,i2,
     & i3,kd)
      vtayy43(i1,i2,i3,kd)=ry(i1,i2,i3)**2*vtarr4(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*vtass4(i1,i2,i3,kd)+ty(i1,i2,i3)**2*vtatt4(i1,i2,i3,
     & kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*vtars4(i1,i2,i3,kd)+2.*ry(i1,
     & i2,i3)*ty(i1,i2,i3)*vtart4(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,
     & i2,i3)*vtast4(i1,i2,i3,kd)+ryy43(i1,i2,i3)*vtar4(i1,i2,i3,kd)+
     & syy43(i1,i2,i3)*vtas4(i1,i2,i3,kd)+tyy43(i1,i2,i3)*vtat4(i1,i2,
     & i3,kd)
      vtazz43(i1,i2,i3,kd)=rz(i1,i2,i3)**2*vtarr4(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*vtass4(i1,i2,i3,kd)+tz(i1,i2,i3)**2*vtatt4(i1,i2,i3,
     & kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*vtars4(i1,i2,i3,kd)+2.*rz(i1,
     & i2,i3)*tz(i1,i2,i3)*vtart4(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,
     & i2,i3)*vtast4(i1,i2,i3,kd)+rzz43(i1,i2,i3)*vtar4(i1,i2,i3,kd)+
     & szz43(i1,i2,i3)*vtas4(i1,i2,i3,kd)+tzz43(i1,i2,i3)*vtat4(i1,i2,
     & i3,kd)
      vtaxy43(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vtarr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vtass4(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & ty(i1,i2,i3)*vtatt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(
     & i1,i2,i3)*sx(i1,i2,i3))*vtars4(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(
     & i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*vtart4(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*vtast4(i1,i2,
     & i3,kd)+rxy43(i1,i2,i3)*vtar4(i1,i2,i3,kd)+sxy43(i1,i2,i3)*
     & vtas4(i1,i2,i3,kd)+txy43(i1,i2,i3)*vtat4(i1,i2,i3,kd)
      vtaxz43(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*vtarr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*vtass4(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & tz(i1,i2,i3)*vtatt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sx(i1,i2,i3))*vtars4(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*vtart4(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*vtast4(i1,i2,
     & i3,kd)+rxz43(i1,i2,i3)*vtar4(i1,i2,i3,kd)+sxz43(i1,i2,i3)*
     & vtas4(i1,i2,i3,kd)+txz43(i1,i2,i3)*vtat4(i1,i2,i3,kd)
      vtayz43(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*vtarr4(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*vtass4(i1,i2,i3,kd)+ty(i1,i2,i3)*
     & tz(i1,i2,i3)*vtatt4(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sy(i1,i2,i3))*vtars4(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*vtart4(i1,i2,i3,kd)+(sy(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*vtast4(i1,i2,
     & i3,kd)+ryz43(i1,i2,i3)*vtar4(i1,i2,i3,kd)+syz43(i1,i2,i3)*
     & vtas4(i1,i2,i3,kd)+tyz43(i1,i2,i3)*vtat4(i1,i2,i3,kd)
      vtalaplacian43(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*vtarr4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)
     & **2+sz(i1,i2,i3)**2)*vtass4(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(
     & i1,i2,i3)**2+tz(i1,i2,i3)**2)*vtatt4(i1,i2,i3,kd)+2.*(rx(i1,i2,
     & i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,
     & i2,i3))*vtars4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(
     & i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*vtart4(i1,i2,
     & i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)
     & +sz(i1,i2,i3)*tz(i1,i2,i3))*vtast4(i1,i2,i3,kd)+(rxx43(i1,i2,
     & i3)+ryy43(i1,i2,i3)+rzz43(i1,i2,i3))*vtar4(i1,i2,i3,kd)+(sxx43(
     & i1,i2,i3)+syy43(i1,i2,i3)+szz43(i1,i2,i3))*vtas4(i1,i2,i3,kd)+(
     & txx43(i1,i2,i3)+tyy43(i1,i2,i3)+tzz43(i1,i2,i3))*vtat4(i1,i2,
     & i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      vtax43r(i1,i2,i3,kd)=(8.*(vta(i1+1,i2,i3,kd)-vta(i1-1,i2,i3,kd))-
     & (vta(i1+2,i2,i3,kd)-vta(i1-2,i2,i3,kd)))*h41(0)
      vtay43r(i1,i2,i3,kd)=(8.*(vta(i1,i2+1,i3,kd)-vta(i1,i2-1,i3,kd))-
     & (vta(i1,i2+2,i3,kd)-vta(i1,i2-2,i3,kd)))*h41(1)
      vtaz43r(i1,i2,i3,kd)=(8.*(vta(i1,i2,i3+1,kd)-vta(i1,i2,i3-1,kd))-
     & (vta(i1,i2,i3+2,kd)-vta(i1,i2,i3-2,kd)))*h41(2)
      vtaxx43r(i1,i2,i3,kd)=( -30.*vta(i1,i2,i3,kd)+16.*(vta(i1+1,i2,
     & i3,kd)+vta(i1-1,i2,i3,kd))-(vta(i1+2,i2,i3,kd)+vta(i1-2,i2,i3,
     & kd)) )*h42(0)
      vtayy43r(i1,i2,i3,kd)=( -30.*vta(i1,i2,i3,kd)+16.*(vta(i1,i2+1,
     & i3,kd)+vta(i1,i2-1,i3,kd))-(vta(i1,i2+2,i3,kd)+vta(i1,i2-2,i3,
     & kd)) )*h42(1)
      vtazz43r(i1,i2,i3,kd)=( -30.*vta(i1,i2,i3,kd)+16.*(vta(i1,i2,i3+
     & 1,kd)+vta(i1,i2,i3-1,kd))-(vta(i1,i2,i3+2,kd)+vta(i1,i2,i3-2,
     & kd)) )*h42(2)
      vtaxy43r(i1,i2,i3,kd)=( (vta(i1+2,i2+2,i3,kd)-vta(i1-2,i2+2,i3,
     & kd)- vta(i1+2,i2-2,i3,kd)+vta(i1-2,i2-2,i3,kd)) +8.*(vta(i1-1,
     & i2+2,i3,kd)-vta(i1-1,i2-2,i3,kd)-vta(i1+1,i2+2,i3,kd)+vta(i1+1,
     & i2-2,i3,kd) +vta(i1+2,i2-1,i3,kd)-vta(i1-2,i2-1,i3,kd)-vta(i1+
     & 2,i2+1,i3,kd)+vta(i1-2,i2+1,i3,kd))+64.*(vta(i1+1,i2+1,i3,kd)-
     & vta(i1-1,i2+1,i3,kd)- vta(i1+1,i2-1,i3,kd)+vta(i1-1,i2-1,i3,kd)
     & ))*(h41(0)*h41(1))
      vtaxz43r(i1,i2,i3,kd)=( (vta(i1+2,i2,i3+2,kd)-vta(i1-2,i2,i3+2,
     & kd)-vta(i1+2,i2,i3-2,kd)+vta(i1-2,i2,i3-2,kd)) +8.*(vta(i1-1,
     & i2,i3+2,kd)-vta(i1-1,i2,i3-2,kd)-vta(i1+1,i2,i3+2,kd)+vta(i1+1,
     & i2,i3-2,kd) +vta(i1+2,i2,i3-1,kd)-vta(i1-2,i2,i3-1,kd)- vta(i1+
     & 2,i2,i3+1,kd)+vta(i1-2,i2,i3+1,kd)) +64.*(vta(i1+1,i2,i3+1,kd)-
     & vta(i1-1,i2,i3+1,kd)-vta(i1+1,i2,i3-1,kd)+vta(i1-1,i2,i3-1,kd))
     &  )*(h41(0)*h41(2))
      vtayz43r(i1,i2,i3,kd)=( (vta(i1,i2+2,i3+2,kd)-vta(i1,i2-2,i3+2,
     & kd)-vta(i1,i2+2,i3-2,kd)+vta(i1,i2-2,i3-2,kd)) +8.*(vta(i1,i2-
     & 1,i3+2,kd)-vta(i1,i2-1,i3-2,kd)-vta(i1,i2+1,i3+2,kd)+vta(i1,i2+
     & 1,i3-2,kd) +vta(i1,i2+2,i3-1,kd)-vta(i1,i2-2,i3-1,kd)-vta(i1,
     & i2+2,i3+1,kd)+vta(i1,i2-2,i3+1,kd)) +64.*(vta(i1,i2+1,i3+1,kd)-
     & vta(i1,i2-1,i3+1,kd)-vta(i1,i2+1,i3-1,kd)+vta(i1,i2-1,i3-1,kd))
     &  )*(h41(1)*h41(2))
      vtax41r(i1,i2,i3,kd)= vtax43r(i1,i2,i3,kd)
      vtay41r(i1,i2,i3,kd)= vtay43r(i1,i2,i3,kd)
      vtaz41r(i1,i2,i3,kd)= vtaz43r(i1,i2,i3,kd)
      vtaxx41r(i1,i2,i3,kd)= vtaxx43r(i1,i2,i3,kd)
      vtayy41r(i1,i2,i3,kd)= vtayy43r(i1,i2,i3,kd)
      vtazz41r(i1,i2,i3,kd)= vtazz43r(i1,i2,i3,kd)
      vtaxy41r(i1,i2,i3,kd)= vtaxy43r(i1,i2,i3,kd)
      vtaxz41r(i1,i2,i3,kd)= vtaxz43r(i1,i2,i3,kd)
      vtayz41r(i1,i2,i3,kd)= vtayz43r(i1,i2,i3,kd)
      vtalaplacian41r(i1,i2,i3,kd)=vtaxx43r(i1,i2,i3,kd)
      vtax42r(i1,i2,i3,kd)= vtax43r(i1,i2,i3,kd)
      vtay42r(i1,i2,i3,kd)= vtay43r(i1,i2,i3,kd)
      vtaz42r(i1,i2,i3,kd)= vtaz43r(i1,i2,i3,kd)
      vtaxx42r(i1,i2,i3,kd)= vtaxx43r(i1,i2,i3,kd)
      vtayy42r(i1,i2,i3,kd)= vtayy43r(i1,i2,i3,kd)
      vtazz42r(i1,i2,i3,kd)= vtazz43r(i1,i2,i3,kd)
      vtaxy42r(i1,i2,i3,kd)= vtaxy43r(i1,i2,i3,kd)
      vtaxz42r(i1,i2,i3,kd)= vtaxz43r(i1,i2,i3,kd)
      vtayz42r(i1,i2,i3,kd)= vtayz43r(i1,i2,i3,kd)
      vtalaplacian42r(i1,i2,i3,kd)=vtaxx43r(i1,i2,i3,kd)+vtayy43r(i1,
     & i2,i3,kd)
      vtalaplacian43r(i1,i2,i3,kd)=vtaxx43r(i1,i2,i3,kd)+vtayy43r(i1,
     & i2,i3,kd)+vtazz43r(i1,i2,i3,kd)
      vtamr4(i1,i2,i3,kd)=(8.*(vtam(i1+1,i2,i3,kd)-vtam(i1-1,i2,i3,kd))
     & -(vtam(i1+2,i2,i3,kd)-vtam(i1-2,i2,i3,kd)))*d14(0)
      vtams4(i1,i2,i3,kd)=(8.*(vtam(i1,i2+1,i3,kd)-vtam(i1,i2-1,i3,kd))
     & -(vtam(i1,i2+2,i3,kd)-vtam(i1,i2-2,i3,kd)))*d14(1)
      vtamt4(i1,i2,i3,kd)=(8.*(vtam(i1,i2,i3+1,kd)-vtam(i1,i2,i3-1,kd))
     & -(vtam(i1,i2,i3+2,kd)-vtam(i1,i2,i3-2,kd)))*d14(2)
      vtamrr4(i1,i2,i3,kd)=(-30.*vtam(i1,i2,i3,kd)+16.*(vtam(i1+1,i2,
     & i3,kd)+vtam(i1-1,i2,i3,kd))-(vtam(i1+2,i2,i3,kd)+vtam(i1-2,i2,
     & i3,kd)) )*d24(0)
      vtamss4(i1,i2,i3,kd)=(-30.*vtam(i1,i2,i3,kd)+16.*(vtam(i1,i2+1,
     & i3,kd)+vtam(i1,i2-1,i3,kd))-(vtam(i1,i2+2,i3,kd)+vtam(i1,i2-2,
     & i3,kd)) )*d24(1)
      vtamtt4(i1,i2,i3,kd)=(-30.*vtam(i1,i2,i3,kd)+16.*(vtam(i1,i2,i3+
     & 1,kd)+vtam(i1,i2,i3-1,kd))-(vtam(i1,i2,i3+2,kd)+vtam(i1,i2,i3-
     & 2,kd)) )*d24(2)
      vtamrs4(i1,i2,i3,kd)=(8.*(vtamr4(i1,i2+1,i3,kd)-vtamr4(i1,i2-1,
     & i3,kd))-(vtamr4(i1,i2+2,i3,kd)-vtamr4(i1,i2-2,i3,kd)))*d14(1)
      vtamrt4(i1,i2,i3,kd)=(8.*(vtamr4(i1,i2,i3+1,kd)-vtamr4(i1,i2,i3-
     & 1,kd))-(vtamr4(i1,i2,i3+2,kd)-vtamr4(i1,i2,i3-2,kd)))*d14(2)
      vtamst4(i1,i2,i3,kd)=(8.*(vtams4(i1,i2,i3+1,kd)-vtams4(i1,i2,i3-
     & 1,kd))-(vtams4(i1,i2,i3+2,kd)-vtams4(i1,i2,i3-2,kd)))*d14(2)
      vtamx41(i1,i2,i3,kd)= rx(i1,i2,i3)*vtamr4(i1,i2,i3,kd)
      vtamy41(i1,i2,i3,kd)=0
      vtamz41(i1,i2,i3,kd)=0
      vtamx42(i1,i2,i3,kd)= rx(i1,i2,i3)*vtamr4(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*vtams4(i1,i2,i3,kd)
      vtamy42(i1,i2,i3,kd)= ry(i1,i2,i3)*vtamr4(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*vtams4(i1,i2,i3,kd)
      vtamz42(i1,i2,i3,kd)=0
      vtamx43(i1,i2,i3,kd)=rx(i1,i2,i3)*vtamr4(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*vtams4(i1,i2,i3,kd)+tx(i1,i2,i3)*vtamt4(i1,i2,i3,kd)
      vtamy43(i1,i2,i3,kd)=ry(i1,i2,i3)*vtamr4(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*vtams4(i1,i2,i3,kd)+ty(i1,i2,i3)*vtamt4(i1,i2,i3,kd)
      vtamz43(i1,i2,i3,kd)=rz(i1,i2,i3)*vtamr4(i1,i2,i3,kd)+sz(i1,i2,
     & i3)*vtams4(i1,i2,i3,kd)+tz(i1,i2,i3)*vtamt4(i1,i2,i3,kd)
      vtamxx41(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vtamrr4(i1,i2,i3,kd)+(
     & rxx42(i1,i2,i3))*vtamr4(i1,i2,i3,kd)
      vtamyy41(i1,i2,i3,kd)=0
      vtamxy41(i1,i2,i3,kd)=0
      vtamxz41(i1,i2,i3,kd)=0
      vtamyz41(i1,i2,i3,kd)=0
      vtamzz41(i1,i2,i3,kd)=0
      vtamlaplacian41(i1,i2,i3,kd)=vtamxx41(i1,i2,i3,kd)
      vtamxx42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vtamrr4(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*vtamrs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*vtamss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3))*vtamr4(i1,i2,i3,kd)+(
     & sxx42(i1,i2,i3))*vtams4(i1,i2,i3,kd)
      vtamyy42(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*vtamrr4(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*vtamrs4(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*vtamss4(i1,i2,i3,kd)+(ryy42(i1,i2,i3))*vtamr4(i1,i2,i3,kd)+(
     & syy42(i1,i2,i3))*vtams4(i1,i2,i3,kd)
      vtamxy42(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vtamrr4(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & vtamrs4(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vtamss4(i1,i2,
     & i3,kd)+rxy42(i1,i2,i3)*vtamr4(i1,i2,i3,kd)+sxy42(i1,i2,i3)*
     & vtams4(i1,i2,i3,kd)
      vtamxz42(i1,i2,i3,kd)=0
      vtamyz42(i1,i2,i3,kd)=0
      vtamzz42(i1,i2,i3,kd)=0
      vtamlaplacian42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & vtamrr4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,
     & i3)*sy(i1,i2,i3))*vtamrs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,
     & i2,i3)**2)*vtamss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3)+ryy42(i1,i2,
     & i3))*vtamr4(i1,i2,i3,kd)+(sxx42(i1,i2,i3)+syy42(i1,i2,i3))*
     & vtams4(i1,i2,i3,kd)
      vtamxx43(i1,i2,i3,kd)=rx(i1,i2,i3)**2*vtamrr4(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*vtamss4(i1,i2,i3,kd)+tx(i1,i2,i3)**2*vtamtt4(i1,i2,
     & i3,kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*vtamrs4(i1,i2,i3,kd)+2.*rx(
     & i1,i2,i3)*tx(i1,i2,i3)*vtamrt4(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(
     & i1,i2,i3)*vtamst4(i1,i2,i3,kd)+rxx43(i1,i2,i3)*vtamr4(i1,i2,i3,
     & kd)+sxx43(i1,i2,i3)*vtams4(i1,i2,i3,kd)+txx43(i1,i2,i3)*vtamt4(
     & i1,i2,i3,kd)
      vtamyy43(i1,i2,i3,kd)=ry(i1,i2,i3)**2*vtamrr4(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*vtamss4(i1,i2,i3,kd)+ty(i1,i2,i3)**2*vtamtt4(i1,i2,
     & i3,kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*vtamrs4(i1,i2,i3,kd)+2.*ry(
     & i1,i2,i3)*ty(i1,i2,i3)*vtamrt4(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(
     & i1,i2,i3)*vtamst4(i1,i2,i3,kd)+ryy43(i1,i2,i3)*vtamr4(i1,i2,i3,
     & kd)+syy43(i1,i2,i3)*vtams4(i1,i2,i3,kd)+tyy43(i1,i2,i3)*vtamt4(
     & i1,i2,i3,kd)
      vtamzz43(i1,i2,i3,kd)=rz(i1,i2,i3)**2*vtamrr4(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*vtamss4(i1,i2,i3,kd)+tz(i1,i2,i3)**2*vtamtt4(i1,i2,
     & i3,kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*vtamrs4(i1,i2,i3,kd)+2.*rz(
     & i1,i2,i3)*tz(i1,i2,i3)*vtamrt4(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(
     & i1,i2,i3)*vtamst4(i1,i2,i3,kd)+rzz43(i1,i2,i3)*vtamr4(i1,i2,i3,
     & kd)+szz43(i1,i2,i3)*vtams4(i1,i2,i3,kd)+tzz43(i1,i2,i3)*vtamt4(
     & i1,i2,i3,kd)
      vtamxy43(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vtamrr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vtamss4(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *ty(i1,i2,i3)*vtamtt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+
     & ry(i1,i2,i3)*sx(i1,i2,i3))*vtamrs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & ty(i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*vtamrt4(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*vtamst4(
     & i1,i2,i3,kd)+rxy43(i1,i2,i3)*vtamr4(i1,i2,i3,kd)+sxy43(i1,i2,
     & i3)*vtams4(i1,i2,i3,kd)+txy43(i1,i2,i3)*vtamt4(i1,i2,i3,kd)
      vtamxz43(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*vtamrr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*vtamss4(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *tz(i1,i2,i3)*vtamtt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sx(i1,i2,i3))*vtamrs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*vtamrt4(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*vtamst4(
     & i1,i2,i3,kd)+rxz43(i1,i2,i3)*vtamr4(i1,i2,i3,kd)+sxz43(i1,i2,
     & i3)*vtams4(i1,i2,i3,kd)+txz43(i1,i2,i3)*vtamt4(i1,i2,i3,kd)
      vtamyz43(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*vtamrr4(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*vtamss4(i1,i2,i3,kd)+ty(i1,i2,i3)
     & *tz(i1,i2,i3)*vtamtt4(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sy(i1,i2,i3))*vtamrs4(i1,i2,i3,kd)+(ry(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*vtamrt4(i1,i2,i3,kd)+(
     & sy(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*vtamst4(
     & i1,i2,i3,kd)+ryz43(i1,i2,i3)*vtamr4(i1,i2,i3,kd)+syz43(i1,i2,
     & i3)*vtams4(i1,i2,i3,kd)+tyz43(i1,i2,i3)*vtamt4(i1,i2,i3,kd)
      vtamlaplacian43(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*vtamrr4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2+sz(i1,i2,i3)**2)*vtamss4(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+
     & ty(i1,i2,i3)**2+tz(i1,i2,i3)**2)*vtamtt4(i1,i2,i3,kd)+2.*(rx(
     & i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*
     & sz(i1,i2,i3))*vtamrs4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,
     & i3)+ ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*
     & vtamrt4(i1,i2,i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,
     & i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3))*vtamst4(i1,i2,i3,
     & kd)+(rxx43(i1,i2,i3)+ryy43(i1,i2,i3)+rzz43(i1,i2,i3))*vtamr4(
     & i1,i2,i3,kd)+(sxx43(i1,i2,i3)+syy43(i1,i2,i3)+szz43(i1,i2,i3))*
     & vtams4(i1,i2,i3,kd)+(txx43(i1,i2,i3)+tyy43(i1,i2,i3)+tzz43(i1,
     & i2,i3))*vtamt4(i1,i2,i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      vtamx43r(i1,i2,i3,kd)=(8.*(vtam(i1+1,i2,i3,kd)-vtam(i1-1,i2,i3,
     & kd))-(vtam(i1+2,i2,i3,kd)-vtam(i1-2,i2,i3,kd)))*h41(0)
      vtamy43r(i1,i2,i3,kd)=(8.*(vtam(i1,i2+1,i3,kd)-vtam(i1,i2-1,i3,
     & kd))-(vtam(i1,i2+2,i3,kd)-vtam(i1,i2-2,i3,kd)))*h41(1)
      vtamz43r(i1,i2,i3,kd)=(8.*(vtam(i1,i2,i3+1,kd)-vtam(i1,i2,i3-1,
     & kd))-(vtam(i1,i2,i3+2,kd)-vtam(i1,i2,i3-2,kd)))*h41(2)
      vtamxx43r(i1,i2,i3,kd)=( -30.*vtam(i1,i2,i3,kd)+16.*(vtam(i1+1,
     & i2,i3,kd)+vtam(i1-1,i2,i3,kd))-(vtam(i1+2,i2,i3,kd)+vtam(i1-2,
     & i2,i3,kd)) )*h42(0)
      vtamyy43r(i1,i2,i3,kd)=( -30.*vtam(i1,i2,i3,kd)+16.*(vtam(i1,i2+
     & 1,i3,kd)+vtam(i1,i2-1,i3,kd))-(vtam(i1,i2+2,i3,kd)+vtam(i1,i2-
     & 2,i3,kd)) )*h42(1)
      vtamzz43r(i1,i2,i3,kd)=( -30.*vtam(i1,i2,i3,kd)+16.*(vtam(i1,i2,
     & i3+1,kd)+vtam(i1,i2,i3-1,kd))-(vtam(i1,i2,i3+2,kd)+vtam(i1,i2,
     & i3-2,kd)) )*h42(2)
      vtamxy43r(i1,i2,i3,kd)=( (vtam(i1+2,i2+2,i3,kd)-vtam(i1-2,i2+2,
     & i3,kd)- vtam(i1+2,i2-2,i3,kd)+vtam(i1-2,i2-2,i3,kd)) +8.*(vtam(
     & i1-1,i2+2,i3,kd)-vtam(i1-1,i2-2,i3,kd)-vtam(i1+1,i2+2,i3,kd)+
     & vtam(i1+1,i2-2,i3,kd) +vtam(i1+2,i2-1,i3,kd)-vtam(i1-2,i2-1,i3,
     & kd)-vtam(i1+2,i2+1,i3,kd)+vtam(i1-2,i2+1,i3,kd))+64.*(vtam(i1+
     & 1,i2+1,i3,kd)-vtam(i1-1,i2+1,i3,kd)- vtam(i1+1,i2-1,i3,kd)+
     & vtam(i1-1,i2-1,i3,kd)))*(h41(0)*h41(1))
      vtamxz43r(i1,i2,i3,kd)=( (vtam(i1+2,i2,i3+2,kd)-vtam(i1-2,i2,i3+
     & 2,kd)-vtam(i1+2,i2,i3-2,kd)+vtam(i1-2,i2,i3-2,kd)) +8.*(vtam(
     & i1-1,i2,i3+2,kd)-vtam(i1-1,i2,i3-2,kd)-vtam(i1+1,i2,i3+2,kd)+
     & vtam(i1+1,i2,i3-2,kd) +vtam(i1+2,i2,i3-1,kd)-vtam(i1-2,i2,i3-1,
     & kd)- vtam(i1+2,i2,i3+1,kd)+vtam(i1-2,i2,i3+1,kd)) +64.*(vtam(
     & i1+1,i2,i3+1,kd)-vtam(i1-1,i2,i3+1,kd)-vtam(i1+1,i2,i3-1,kd)+
     & vtam(i1-1,i2,i3-1,kd)) )*(h41(0)*h41(2))
      vtamyz43r(i1,i2,i3,kd)=( (vtam(i1,i2+2,i3+2,kd)-vtam(i1,i2-2,i3+
     & 2,kd)-vtam(i1,i2+2,i3-2,kd)+vtam(i1,i2-2,i3-2,kd)) +8.*(vtam(
     & i1,i2-1,i3+2,kd)-vtam(i1,i2-1,i3-2,kd)-vtam(i1,i2+1,i3+2,kd)+
     & vtam(i1,i2+1,i3-2,kd) +vtam(i1,i2+2,i3-1,kd)-vtam(i1,i2-2,i3-1,
     & kd)-vtam(i1,i2+2,i3+1,kd)+vtam(i1,i2-2,i3+1,kd)) +64.*(vtam(i1,
     & i2+1,i3+1,kd)-vtam(i1,i2-1,i3+1,kd)-vtam(i1,i2+1,i3-1,kd)+vtam(
     & i1,i2-1,i3-1,kd)) )*(h41(1)*h41(2))
      vtamx41r(i1,i2,i3,kd)= vtamx43r(i1,i2,i3,kd)
      vtamy41r(i1,i2,i3,kd)= vtamy43r(i1,i2,i3,kd)
      vtamz41r(i1,i2,i3,kd)= vtamz43r(i1,i2,i3,kd)
      vtamxx41r(i1,i2,i3,kd)= vtamxx43r(i1,i2,i3,kd)
      vtamyy41r(i1,i2,i3,kd)= vtamyy43r(i1,i2,i3,kd)
      vtamzz41r(i1,i2,i3,kd)= vtamzz43r(i1,i2,i3,kd)
      vtamxy41r(i1,i2,i3,kd)= vtamxy43r(i1,i2,i3,kd)
      vtamxz41r(i1,i2,i3,kd)= vtamxz43r(i1,i2,i3,kd)
      vtamyz41r(i1,i2,i3,kd)= vtamyz43r(i1,i2,i3,kd)
      vtamlaplacian41r(i1,i2,i3,kd)=vtamxx43r(i1,i2,i3,kd)
      vtamx42r(i1,i2,i3,kd)= vtamx43r(i1,i2,i3,kd)
      vtamy42r(i1,i2,i3,kd)= vtamy43r(i1,i2,i3,kd)
      vtamz42r(i1,i2,i3,kd)= vtamz43r(i1,i2,i3,kd)
      vtamxx42r(i1,i2,i3,kd)= vtamxx43r(i1,i2,i3,kd)
      vtamyy42r(i1,i2,i3,kd)= vtamyy43r(i1,i2,i3,kd)
      vtamzz42r(i1,i2,i3,kd)= vtamzz43r(i1,i2,i3,kd)
      vtamxy42r(i1,i2,i3,kd)= vtamxy43r(i1,i2,i3,kd)
      vtamxz42r(i1,i2,i3,kd)= vtamxz43r(i1,i2,i3,kd)
      vtamyz42r(i1,i2,i3,kd)= vtamyz43r(i1,i2,i3,kd)
      vtamlaplacian42r(i1,i2,i3,kd)=vtamxx43r(i1,i2,i3,kd)+vtamyy43r(
     & i1,i2,i3,kd)
      vtamlaplacian43r(i1,i2,i3,kd)=vtamxx43r(i1,i2,i3,kd)+vtamyy43r(
     & i1,i2,i3,kd)+vtamzz43r(i1,i2,i3,kd)
      wtar4(i1,i2,i3,kd)=(8.*(wta(i1+1,i2,i3,kd)-wta(i1-1,i2,i3,kd))-(
     & wta(i1+2,i2,i3,kd)-wta(i1-2,i2,i3,kd)))*d14(0)
      wtas4(i1,i2,i3,kd)=(8.*(wta(i1,i2+1,i3,kd)-wta(i1,i2-1,i3,kd))-(
     & wta(i1,i2+2,i3,kd)-wta(i1,i2-2,i3,kd)))*d14(1)
      wtat4(i1,i2,i3,kd)=(8.*(wta(i1,i2,i3+1,kd)-wta(i1,i2,i3-1,kd))-(
     & wta(i1,i2,i3+2,kd)-wta(i1,i2,i3-2,kd)))*d14(2)
      wtarr4(i1,i2,i3,kd)=(-30.*wta(i1,i2,i3,kd)+16.*(wta(i1+1,i2,i3,
     & kd)+wta(i1-1,i2,i3,kd))-(wta(i1+2,i2,i3,kd)+wta(i1-2,i2,i3,kd))
     &  )*d24(0)
      wtass4(i1,i2,i3,kd)=(-30.*wta(i1,i2,i3,kd)+16.*(wta(i1,i2+1,i3,
     & kd)+wta(i1,i2-1,i3,kd))-(wta(i1,i2+2,i3,kd)+wta(i1,i2-2,i3,kd))
     &  )*d24(1)
      wtatt4(i1,i2,i3,kd)=(-30.*wta(i1,i2,i3,kd)+16.*(wta(i1,i2,i3+1,
     & kd)+wta(i1,i2,i3-1,kd))-(wta(i1,i2,i3+2,kd)+wta(i1,i2,i3-2,kd))
     &  )*d24(2)
      wtars4(i1,i2,i3,kd)=(8.*(wtar4(i1,i2+1,i3,kd)-wtar4(i1,i2-1,i3,
     & kd))-(wtar4(i1,i2+2,i3,kd)-wtar4(i1,i2-2,i3,kd)))*d14(1)
      wtart4(i1,i2,i3,kd)=(8.*(wtar4(i1,i2,i3+1,kd)-wtar4(i1,i2,i3-1,
     & kd))-(wtar4(i1,i2,i3+2,kd)-wtar4(i1,i2,i3-2,kd)))*d14(2)
      wtast4(i1,i2,i3,kd)=(8.*(wtas4(i1,i2,i3+1,kd)-wtas4(i1,i2,i3-1,
     & kd))-(wtas4(i1,i2,i3+2,kd)-wtas4(i1,i2,i3-2,kd)))*d14(2)
      wtax41(i1,i2,i3,kd)= rx(i1,i2,i3)*wtar4(i1,i2,i3,kd)
      wtay41(i1,i2,i3,kd)=0
      wtaz41(i1,i2,i3,kd)=0
      wtax42(i1,i2,i3,kd)= rx(i1,i2,i3)*wtar4(i1,i2,i3,kd)+sx(i1,i2,i3)
     & *wtas4(i1,i2,i3,kd)
      wtay42(i1,i2,i3,kd)= ry(i1,i2,i3)*wtar4(i1,i2,i3,kd)+sy(i1,i2,i3)
     & *wtas4(i1,i2,i3,kd)
      wtaz42(i1,i2,i3,kd)=0
      wtax43(i1,i2,i3,kd)=rx(i1,i2,i3)*wtar4(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & wtas4(i1,i2,i3,kd)+tx(i1,i2,i3)*wtat4(i1,i2,i3,kd)
      wtay43(i1,i2,i3,kd)=ry(i1,i2,i3)*wtar4(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & wtas4(i1,i2,i3,kd)+ty(i1,i2,i3)*wtat4(i1,i2,i3,kd)
      wtaz43(i1,i2,i3,kd)=rz(i1,i2,i3)*wtar4(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & wtas4(i1,i2,i3,kd)+tz(i1,i2,i3)*wtat4(i1,i2,i3,kd)
      wtaxx41(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wtarr4(i1,i2,i3,kd)+(
     & rxx42(i1,i2,i3))*wtar4(i1,i2,i3,kd)
      wtayy41(i1,i2,i3,kd)=0
      wtaxy41(i1,i2,i3,kd)=0
      wtaxz41(i1,i2,i3,kd)=0
      wtayz41(i1,i2,i3,kd)=0
      wtazz41(i1,i2,i3,kd)=0
      wtalaplacian41(i1,i2,i3,kd)=wtaxx41(i1,i2,i3,kd)
      wtaxx42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wtarr4(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*wtars4(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*wtass4(i1,i2,i3,kd)+(rxx42(i1,i2,i3))*wtar4(i1,i2,i3,kd)+(
     & sxx42(i1,i2,i3))*wtas4(i1,i2,i3,kd)
      wtayy42(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*wtarr4(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*wtars4(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*wtass4(i1,i2,i3,kd)+(ryy42(i1,i2,i3))*wtar4(i1,i2,i3,kd)+(
     & syy42(i1,i2,i3))*wtas4(i1,i2,i3,kd)
      wtaxy42(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wtarr4(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & wtars4(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wtass4(i1,i2,i3,
     & kd)+rxy42(i1,i2,i3)*wtar4(i1,i2,i3,kd)+sxy42(i1,i2,i3)*wtas4(
     & i1,i2,i3,kd)
      wtaxz42(i1,i2,i3,kd)=0
      wtayz42(i1,i2,i3,kd)=0
      wtazz42(i1,i2,i3,kd)=0
      wtalaplacian42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & wtarr4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)
     & *sy(i1,i2,i3))*wtars4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2)*wtass4(i1,i2,i3,kd)+(rxx42(i1,i2,i3)+ryy42(i1,i2,i3))*
     & wtar4(i1,i2,i3,kd)+(sxx42(i1,i2,i3)+syy42(i1,i2,i3))*wtas4(i1,
     & i2,i3,kd)
      wtaxx43(i1,i2,i3,kd)=rx(i1,i2,i3)**2*wtarr4(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*wtass4(i1,i2,i3,kd)+tx(i1,i2,i3)**2*wtatt4(i1,i2,i3,
     & kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*wtars4(i1,i2,i3,kd)+2.*rx(i1,
     & i2,i3)*tx(i1,i2,i3)*wtart4(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,
     & i2,i3)*wtast4(i1,i2,i3,kd)+rxx43(i1,i2,i3)*wtar4(i1,i2,i3,kd)+
     & sxx43(i1,i2,i3)*wtas4(i1,i2,i3,kd)+txx43(i1,i2,i3)*wtat4(i1,i2,
     & i3,kd)
      wtayy43(i1,i2,i3,kd)=ry(i1,i2,i3)**2*wtarr4(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*wtass4(i1,i2,i3,kd)+ty(i1,i2,i3)**2*wtatt4(i1,i2,i3,
     & kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*wtars4(i1,i2,i3,kd)+2.*ry(i1,
     & i2,i3)*ty(i1,i2,i3)*wtart4(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,
     & i2,i3)*wtast4(i1,i2,i3,kd)+ryy43(i1,i2,i3)*wtar4(i1,i2,i3,kd)+
     & syy43(i1,i2,i3)*wtas4(i1,i2,i3,kd)+tyy43(i1,i2,i3)*wtat4(i1,i2,
     & i3,kd)
      wtazz43(i1,i2,i3,kd)=rz(i1,i2,i3)**2*wtarr4(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*wtass4(i1,i2,i3,kd)+tz(i1,i2,i3)**2*wtatt4(i1,i2,i3,
     & kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*wtars4(i1,i2,i3,kd)+2.*rz(i1,
     & i2,i3)*tz(i1,i2,i3)*wtart4(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,
     & i2,i3)*wtast4(i1,i2,i3,kd)+rzz43(i1,i2,i3)*wtar4(i1,i2,i3,kd)+
     & szz43(i1,i2,i3)*wtas4(i1,i2,i3,kd)+tzz43(i1,i2,i3)*wtat4(i1,i2,
     & i3,kd)
      wtaxy43(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wtarr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wtass4(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & ty(i1,i2,i3)*wtatt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(
     & i1,i2,i3)*sx(i1,i2,i3))*wtars4(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(
     & i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*wtart4(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*wtast4(i1,i2,
     & i3,kd)+rxy43(i1,i2,i3)*wtar4(i1,i2,i3,kd)+sxy43(i1,i2,i3)*
     & wtas4(i1,i2,i3,kd)+txy43(i1,i2,i3)*wtat4(i1,i2,i3,kd)
      wtaxz43(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*wtarr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*wtass4(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & tz(i1,i2,i3)*wtatt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sx(i1,i2,i3))*wtars4(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*wtart4(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*wtast4(i1,i2,
     & i3,kd)+rxz43(i1,i2,i3)*wtar4(i1,i2,i3,kd)+sxz43(i1,i2,i3)*
     & wtas4(i1,i2,i3,kd)+txz43(i1,i2,i3)*wtat4(i1,i2,i3,kd)
      wtayz43(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*wtarr4(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*wtass4(i1,i2,i3,kd)+ty(i1,i2,i3)*
     & tz(i1,i2,i3)*wtatt4(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sy(i1,i2,i3))*wtars4(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*wtart4(i1,i2,i3,kd)+(sy(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*wtast4(i1,i2,
     & i3,kd)+ryz43(i1,i2,i3)*wtar4(i1,i2,i3,kd)+syz43(i1,i2,i3)*
     & wtas4(i1,i2,i3,kd)+tyz43(i1,i2,i3)*wtat4(i1,i2,i3,kd)
      wtalaplacian43(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*wtarr4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)
     & **2+sz(i1,i2,i3)**2)*wtass4(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(
     & i1,i2,i3)**2+tz(i1,i2,i3)**2)*wtatt4(i1,i2,i3,kd)+2.*(rx(i1,i2,
     & i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,
     & i2,i3))*wtars4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(
     & i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*wtart4(i1,i2,
     & i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)
     & +sz(i1,i2,i3)*tz(i1,i2,i3))*wtast4(i1,i2,i3,kd)+(rxx43(i1,i2,
     & i3)+ryy43(i1,i2,i3)+rzz43(i1,i2,i3))*wtar4(i1,i2,i3,kd)+(sxx43(
     & i1,i2,i3)+syy43(i1,i2,i3)+szz43(i1,i2,i3))*wtas4(i1,i2,i3,kd)+(
     & txx43(i1,i2,i3)+tyy43(i1,i2,i3)+tzz43(i1,i2,i3))*wtat4(i1,i2,
     & i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      wtax43r(i1,i2,i3,kd)=(8.*(wta(i1+1,i2,i3,kd)-wta(i1-1,i2,i3,kd))-
     & (wta(i1+2,i2,i3,kd)-wta(i1-2,i2,i3,kd)))*h41(0)
      wtay43r(i1,i2,i3,kd)=(8.*(wta(i1,i2+1,i3,kd)-wta(i1,i2-1,i3,kd))-
     & (wta(i1,i2+2,i3,kd)-wta(i1,i2-2,i3,kd)))*h41(1)
      wtaz43r(i1,i2,i3,kd)=(8.*(wta(i1,i2,i3+1,kd)-wta(i1,i2,i3-1,kd))-
     & (wta(i1,i2,i3+2,kd)-wta(i1,i2,i3-2,kd)))*h41(2)
      wtaxx43r(i1,i2,i3,kd)=( -30.*wta(i1,i2,i3,kd)+16.*(wta(i1+1,i2,
     & i3,kd)+wta(i1-1,i2,i3,kd))-(wta(i1+2,i2,i3,kd)+wta(i1-2,i2,i3,
     & kd)) )*h42(0)
      wtayy43r(i1,i2,i3,kd)=( -30.*wta(i1,i2,i3,kd)+16.*(wta(i1,i2+1,
     & i3,kd)+wta(i1,i2-1,i3,kd))-(wta(i1,i2+2,i3,kd)+wta(i1,i2-2,i3,
     & kd)) )*h42(1)
      wtazz43r(i1,i2,i3,kd)=( -30.*wta(i1,i2,i3,kd)+16.*(wta(i1,i2,i3+
     & 1,kd)+wta(i1,i2,i3-1,kd))-(wta(i1,i2,i3+2,kd)+wta(i1,i2,i3-2,
     & kd)) )*h42(2)
      wtaxy43r(i1,i2,i3,kd)=( (wta(i1+2,i2+2,i3,kd)-wta(i1-2,i2+2,i3,
     & kd)- wta(i1+2,i2-2,i3,kd)+wta(i1-2,i2-2,i3,kd)) +8.*(wta(i1-1,
     & i2+2,i3,kd)-wta(i1-1,i2-2,i3,kd)-wta(i1+1,i2+2,i3,kd)+wta(i1+1,
     & i2-2,i3,kd) +wta(i1+2,i2-1,i3,kd)-wta(i1-2,i2-1,i3,kd)-wta(i1+
     & 2,i2+1,i3,kd)+wta(i1-2,i2+1,i3,kd))+64.*(wta(i1+1,i2+1,i3,kd)-
     & wta(i1-1,i2+1,i3,kd)- wta(i1+1,i2-1,i3,kd)+wta(i1-1,i2-1,i3,kd)
     & ))*(h41(0)*h41(1))
      wtaxz43r(i1,i2,i3,kd)=( (wta(i1+2,i2,i3+2,kd)-wta(i1-2,i2,i3+2,
     & kd)-wta(i1+2,i2,i3-2,kd)+wta(i1-2,i2,i3-2,kd)) +8.*(wta(i1-1,
     & i2,i3+2,kd)-wta(i1-1,i2,i3-2,kd)-wta(i1+1,i2,i3+2,kd)+wta(i1+1,
     & i2,i3-2,kd) +wta(i1+2,i2,i3-1,kd)-wta(i1-2,i2,i3-1,kd)- wta(i1+
     & 2,i2,i3+1,kd)+wta(i1-2,i2,i3+1,kd)) +64.*(wta(i1+1,i2,i3+1,kd)-
     & wta(i1-1,i2,i3+1,kd)-wta(i1+1,i2,i3-1,kd)+wta(i1-1,i2,i3-1,kd))
     &  )*(h41(0)*h41(2))
      wtayz43r(i1,i2,i3,kd)=( (wta(i1,i2+2,i3+2,kd)-wta(i1,i2-2,i3+2,
     & kd)-wta(i1,i2+2,i3-2,kd)+wta(i1,i2-2,i3-2,kd)) +8.*(wta(i1,i2-
     & 1,i3+2,kd)-wta(i1,i2-1,i3-2,kd)-wta(i1,i2+1,i3+2,kd)+wta(i1,i2+
     & 1,i3-2,kd) +wta(i1,i2+2,i3-1,kd)-wta(i1,i2-2,i3-1,kd)-wta(i1,
     & i2+2,i3+1,kd)+wta(i1,i2-2,i3+1,kd)) +64.*(wta(i1,i2+1,i3+1,kd)-
     & wta(i1,i2-1,i3+1,kd)-wta(i1,i2+1,i3-1,kd)+wta(i1,i2-1,i3-1,kd))
     &  )*(h41(1)*h41(2))
      wtax41r(i1,i2,i3,kd)= wtax43r(i1,i2,i3,kd)
      wtay41r(i1,i2,i3,kd)= wtay43r(i1,i2,i3,kd)
      wtaz41r(i1,i2,i3,kd)= wtaz43r(i1,i2,i3,kd)
      wtaxx41r(i1,i2,i3,kd)= wtaxx43r(i1,i2,i3,kd)
      wtayy41r(i1,i2,i3,kd)= wtayy43r(i1,i2,i3,kd)
      wtazz41r(i1,i2,i3,kd)= wtazz43r(i1,i2,i3,kd)
      wtaxy41r(i1,i2,i3,kd)= wtaxy43r(i1,i2,i3,kd)
      wtaxz41r(i1,i2,i3,kd)= wtaxz43r(i1,i2,i3,kd)
      wtayz41r(i1,i2,i3,kd)= wtayz43r(i1,i2,i3,kd)
      wtalaplacian41r(i1,i2,i3,kd)=wtaxx43r(i1,i2,i3,kd)
      wtax42r(i1,i2,i3,kd)= wtax43r(i1,i2,i3,kd)
      wtay42r(i1,i2,i3,kd)= wtay43r(i1,i2,i3,kd)
      wtaz42r(i1,i2,i3,kd)= wtaz43r(i1,i2,i3,kd)
      wtaxx42r(i1,i2,i3,kd)= wtaxx43r(i1,i2,i3,kd)
      wtayy42r(i1,i2,i3,kd)= wtayy43r(i1,i2,i3,kd)
      wtazz42r(i1,i2,i3,kd)= wtazz43r(i1,i2,i3,kd)
      wtaxy42r(i1,i2,i3,kd)= wtaxy43r(i1,i2,i3,kd)
      wtaxz42r(i1,i2,i3,kd)= wtaxz43r(i1,i2,i3,kd)
      wtayz42r(i1,i2,i3,kd)= wtayz43r(i1,i2,i3,kd)
      wtalaplacian42r(i1,i2,i3,kd)=wtaxx43r(i1,i2,i3,kd)+wtayy43r(i1,
     & i2,i3,kd)
      wtalaplacian43r(i1,i2,i3,kd)=wtaxx43r(i1,i2,i3,kd)+wtayy43r(i1,
     & i2,i3,kd)+wtazz43r(i1,i2,i3,kd)
      wtamr4(i1,i2,i3,kd)=(8.*(wtam(i1+1,i2,i3,kd)-wtam(i1-1,i2,i3,kd))
     & -(wtam(i1+2,i2,i3,kd)-wtam(i1-2,i2,i3,kd)))*d14(0)
      wtams4(i1,i2,i3,kd)=(8.*(wtam(i1,i2+1,i3,kd)-wtam(i1,i2-1,i3,kd))
     & -(wtam(i1,i2+2,i3,kd)-wtam(i1,i2-2,i3,kd)))*d14(1)
      wtamt4(i1,i2,i3,kd)=(8.*(wtam(i1,i2,i3+1,kd)-wtam(i1,i2,i3-1,kd))
     & -(wtam(i1,i2,i3+2,kd)-wtam(i1,i2,i3-2,kd)))*d14(2)
      wtamrr4(i1,i2,i3,kd)=(-30.*wtam(i1,i2,i3,kd)+16.*(wtam(i1+1,i2,
     & i3,kd)+wtam(i1-1,i2,i3,kd))-(wtam(i1+2,i2,i3,kd)+wtam(i1-2,i2,
     & i3,kd)) )*d24(0)
      wtamss4(i1,i2,i3,kd)=(-30.*wtam(i1,i2,i3,kd)+16.*(wtam(i1,i2+1,
     & i3,kd)+wtam(i1,i2-1,i3,kd))-(wtam(i1,i2+2,i3,kd)+wtam(i1,i2-2,
     & i3,kd)) )*d24(1)
      wtamtt4(i1,i2,i3,kd)=(-30.*wtam(i1,i2,i3,kd)+16.*(wtam(i1,i2,i3+
     & 1,kd)+wtam(i1,i2,i3-1,kd))-(wtam(i1,i2,i3+2,kd)+wtam(i1,i2,i3-
     & 2,kd)) )*d24(2)
      wtamrs4(i1,i2,i3,kd)=(8.*(wtamr4(i1,i2+1,i3,kd)-wtamr4(i1,i2-1,
     & i3,kd))-(wtamr4(i1,i2+2,i3,kd)-wtamr4(i1,i2-2,i3,kd)))*d14(1)
      wtamrt4(i1,i2,i3,kd)=(8.*(wtamr4(i1,i2,i3+1,kd)-wtamr4(i1,i2,i3-
     & 1,kd))-(wtamr4(i1,i2,i3+2,kd)-wtamr4(i1,i2,i3-2,kd)))*d14(2)
      wtamst4(i1,i2,i3,kd)=(8.*(wtams4(i1,i2,i3+1,kd)-wtams4(i1,i2,i3-
     & 1,kd))-(wtams4(i1,i2,i3+2,kd)-wtams4(i1,i2,i3-2,kd)))*d14(2)
      wtamx41(i1,i2,i3,kd)= rx(i1,i2,i3)*wtamr4(i1,i2,i3,kd)
      wtamy41(i1,i2,i3,kd)=0
      wtamz41(i1,i2,i3,kd)=0
      wtamx42(i1,i2,i3,kd)= rx(i1,i2,i3)*wtamr4(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*wtams4(i1,i2,i3,kd)
      wtamy42(i1,i2,i3,kd)= ry(i1,i2,i3)*wtamr4(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*wtams4(i1,i2,i3,kd)
      wtamz42(i1,i2,i3,kd)=0
      wtamx43(i1,i2,i3,kd)=rx(i1,i2,i3)*wtamr4(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*wtams4(i1,i2,i3,kd)+tx(i1,i2,i3)*wtamt4(i1,i2,i3,kd)
      wtamy43(i1,i2,i3,kd)=ry(i1,i2,i3)*wtamr4(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*wtams4(i1,i2,i3,kd)+ty(i1,i2,i3)*wtamt4(i1,i2,i3,kd)
      wtamz43(i1,i2,i3,kd)=rz(i1,i2,i3)*wtamr4(i1,i2,i3,kd)+sz(i1,i2,
     & i3)*wtams4(i1,i2,i3,kd)+tz(i1,i2,i3)*wtamt4(i1,i2,i3,kd)
      wtamxx41(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wtamrr4(i1,i2,i3,kd)+(
     & rxx42(i1,i2,i3))*wtamr4(i1,i2,i3,kd)
      wtamyy41(i1,i2,i3,kd)=0
      wtamxy41(i1,i2,i3,kd)=0
      wtamxz41(i1,i2,i3,kd)=0
      wtamyz41(i1,i2,i3,kd)=0
      wtamzz41(i1,i2,i3,kd)=0
      wtamlaplacian41(i1,i2,i3,kd)=wtamxx41(i1,i2,i3,kd)
      wtamxx42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wtamrr4(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*wtamrs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*wtamss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3))*wtamr4(i1,i2,i3,kd)+(
     & sxx42(i1,i2,i3))*wtams4(i1,i2,i3,kd)
      wtamyy42(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*wtamrr4(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*wtamrs4(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*wtamss4(i1,i2,i3,kd)+(ryy42(i1,i2,i3))*wtamr4(i1,i2,i3,kd)+(
     & syy42(i1,i2,i3))*wtams4(i1,i2,i3,kd)
      wtamxy42(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wtamrr4(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & wtamrs4(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wtamss4(i1,i2,
     & i3,kd)+rxy42(i1,i2,i3)*wtamr4(i1,i2,i3,kd)+sxy42(i1,i2,i3)*
     & wtams4(i1,i2,i3,kd)
      wtamxz42(i1,i2,i3,kd)=0
      wtamyz42(i1,i2,i3,kd)=0
      wtamzz42(i1,i2,i3,kd)=0
      wtamlaplacian42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & wtamrr4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,
     & i3)*sy(i1,i2,i3))*wtamrs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,
     & i2,i3)**2)*wtamss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3)+ryy42(i1,i2,
     & i3))*wtamr4(i1,i2,i3,kd)+(sxx42(i1,i2,i3)+syy42(i1,i2,i3))*
     & wtams4(i1,i2,i3,kd)
      wtamxx43(i1,i2,i3,kd)=rx(i1,i2,i3)**2*wtamrr4(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*wtamss4(i1,i2,i3,kd)+tx(i1,i2,i3)**2*wtamtt4(i1,i2,
     & i3,kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*wtamrs4(i1,i2,i3,kd)+2.*rx(
     & i1,i2,i3)*tx(i1,i2,i3)*wtamrt4(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(
     & i1,i2,i3)*wtamst4(i1,i2,i3,kd)+rxx43(i1,i2,i3)*wtamr4(i1,i2,i3,
     & kd)+sxx43(i1,i2,i3)*wtams4(i1,i2,i3,kd)+txx43(i1,i2,i3)*wtamt4(
     & i1,i2,i3,kd)
      wtamyy43(i1,i2,i3,kd)=ry(i1,i2,i3)**2*wtamrr4(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*wtamss4(i1,i2,i3,kd)+ty(i1,i2,i3)**2*wtamtt4(i1,i2,
     & i3,kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*wtamrs4(i1,i2,i3,kd)+2.*ry(
     & i1,i2,i3)*ty(i1,i2,i3)*wtamrt4(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(
     & i1,i2,i3)*wtamst4(i1,i2,i3,kd)+ryy43(i1,i2,i3)*wtamr4(i1,i2,i3,
     & kd)+syy43(i1,i2,i3)*wtams4(i1,i2,i3,kd)+tyy43(i1,i2,i3)*wtamt4(
     & i1,i2,i3,kd)
      wtamzz43(i1,i2,i3,kd)=rz(i1,i2,i3)**2*wtamrr4(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*wtamss4(i1,i2,i3,kd)+tz(i1,i2,i3)**2*wtamtt4(i1,i2,
     & i3,kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*wtamrs4(i1,i2,i3,kd)+2.*rz(
     & i1,i2,i3)*tz(i1,i2,i3)*wtamrt4(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(
     & i1,i2,i3)*wtamst4(i1,i2,i3,kd)+rzz43(i1,i2,i3)*wtamr4(i1,i2,i3,
     & kd)+szz43(i1,i2,i3)*wtams4(i1,i2,i3,kd)+tzz43(i1,i2,i3)*wtamt4(
     & i1,i2,i3,kd)
      wtamxy43(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wtamrr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wtamss4(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *ty(i1,i2,i3)*wtamtt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+
     & ry(i1,i2,i3)*sx(i1,i2,i3))*wtamrs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & ty(i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*wtamrt4(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*wtamst4(
     & i1,i2,i3,kd)+rxy43(i1,i2,i3)*wtamr4(i1,i2,i3,kd)+sxy43(i1,i2,
     & i3)*wtams4(i1,i2,i3,kd)+txy43(i1,i2,i3)*wtamt4(i1,i2,i3,kd)
      wtamxz43(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*wtamrr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*wtamss4(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *tz(i1,i2,i3)*wtamtt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sx(i1,i2,i3))*wtamrs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*wtamrt4(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*wtamst4(
     & i1,i2,i3,kd)+rxz43(i1,i2,i3)*wtamr4(i1,i2,i3,kd)+sxz43(i1,i2,
     & i3)*wtams4(i1,i2,i3,kd)+txz43(i1,i2,i3)*wtamt4(i1,i2,i3,kd)
      wtamyz43(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*wtamrr4(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*wtamss4(i1,i2,i3,kd)+ty(i1,i2,i3)
     & *tz(i1,i2,i3)*wtamtt4(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sy(i1,i2,i3))*wtamrs4(i1,i2,i3,kd)+(ry(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*wtamrt4(i1,i2,i3,kd)+(
     & sy(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*wtamst4(
     & i1,i2,i3,kd)+ryz43(i1,i2,i3)*wtamr4(i1,i2,i3,kd)+syz43(i1,i2,
     & i3)*wtams4(i1,i2,i3,kd)+tyz43(i1,i2,i3)*wtamt4(i1,i2,i3,kd)
      wtamlaplacian43(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*wtamrr4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2+sz(i1,i2,i3)**2)*wtamss4(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+
     & ty(i1,i2,i3)**2+tz(i1,i2,i3)**2)*wtamtt4(i1,i2,i3,kd)+2.*(rx(
     & i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*
     & sz(i1,i2,i3))*wtamrs4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,
     & i3)+ ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*
     & wtamrt4(i1,i2,i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,
     & i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3))*wtamst4(i1,i2,i3,
     & kd)+(rxx43(i1,i2,i3)+ryy43(i1,i2,i3)+rzz43(i1,i2,i3))*wtamr4(
     & i1,i2,i3,kd)+(sxx43(i1,i2,i3)+syy43(i1,i2,i3)+szz43(i1,i2,i3))*
     & wtams4(i1,i2,i3,kd)+(txx43(i1,i2,i3)+tyy43(i1,i2,i3)+tzz43(i1,
     & i2,i3))*wtamt4(i1,i2,i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      wtamx43r(i1,i2,i3,kd)=(8.*(wtam(i1+1,i2,i3,kd)-wtam(i1-1,i2,i3,
     & kd))-(wtam(i1+2,i2,i3,kd)-wtam(i1-2,i2,i3,kd)))*h41(0)
      wtamy43r(i1,i2,i3,kd)=(8.*(wtam(i1,i2+1,i3,kd)-wtam(i1,i2-1,i3,
     & kd))-(wtam(i1,i2+2,i3,kd)-wtam(i1,i2-2,i3,kd)))*h41(1)
      wtamz43r(i1,i2,i3,kd)=(8.*(wtam(i1,i2,i3+1,kd)-wtam(i1,i2,i3-1,
     & kd))-(wtam(i1,i2,i3+2,kd)-wtam(i1,i2,i3-2,kd)))*h41(2)
      wtamxx43r(i1,i2,i3,kd)=( -30.*wtam(i1,i2,i3,kd)+16.*(wtam(i1+1,
     & i2,i3,kd)+wtam(i1-1,i2,i3,kd))-(wtam(i1+2,i2,i3,kd)+wtam(i1-2,
     & i2,i3,kd)) )*h42(0)
      wtamyy43r(i1,i2,i3,kd)=( -30.*wtam(i1,i2,i3,kd)+16.*(wtam(i1,i2+
     & 1,i3,kd)+wtam(i1,i2-1,i3,kd))-(wtam(i1,i2+2,i3,kd)+wtam(i1,i2-
     & 2,i3,kd)) )*h42(1)
      wtamzz43r(i1,i2,i3,kd)=( -30.*wtam(i1,i2,i3,kd)+16.*(wtam(i1,i2,
     & i3+1,kd)+wtam(i1,i2,i3-1,kd))-(wtam(i1,i2,i3+2,kd)+wtam(i1,i2,
     & i3-2,kd)) )*h42(2)
      wtamxy43r(i1,i2,i3,kd)=( (wtam(i1+2,i2+2,i3,kd)-wtam(i1-2,i2+2,
     & i3,kd)- wtam(i1+2,i2-2,i3,kd)+wtam(i1-2,i2-2,i3,kd)) +8.*(wtam(
     & i1-1,i2+2,i3,kd)-wtam(i1-1,i2-2,i3,kd)-wtam(i1+1,i2+2,i3,kd)+
     & wtam(i1+1,i2-2,i3,kd) +wtam(i1+2,i2-1,i3,kd)-wtam(i1-2,i2-1,i3,
     & kd)-wtam(i1+2,i2+1,i3,kd)+wtam(i1-2,i2+1,i3,kd))+64.*(wtam(i1+
     & 1,i2+1,i3,kd)-wtam(i1-1,i2+1,i3,kd)- wtam(i1+1,i2-1,i3,kd)+
     & wtam(i1-1,i2-1,i3,kd)))*(h41(0)*h41(1))
      wtamxz43r(i1,i2,i3,kd)=( (wtam(i1+2,i2,i3+2,kd)-wtam(i1-2,i2,i3+
     & 2,kd)-wtam(i1+2,i2,i3-2,kd)+wtam(i1-2,i2,i3-2,kd)) +8.*(wtam(
     & i1-1,i2,i3+2,kd)-wtam(i1-1,i2,i3-2,kd)-wtam(i1+1,i2,i3+2,kd)+
     & wtam(i1+1,i2,i3-2,kd) +wtam(i1+2,i2,i3-1,kd)-wtam(i1-2,i2,i3-1,
     & kd)- wtam(i1+2,i2,i3+1,kd)+wtam(i1-2,i2,i3+1,kd)) +64.*(wtam(
     & i1+1,i2,i3+1,kd)-wtam(i1-1,i2,i3+1,kd)-wtam(i1+1,i2,i3-1,kd)+
     & wtam(i1-1,i2,i3-1,kd)) )*(h41(0)*h41(2))
      wtamyz43r(i1,i2,i3,kd)=( (wtam(i1,i2+2,i3+2,kd)-wtam(i1,i2-2,i3+
     & 2,kd)-wtam(i1,i2+2,i3-2,kd)+wtam(i1,i2-2,i3-2,kd)) +8.*(wtam(
     & i1,i2-1,i3+2,kd)-wtam(i1,i2-1,i3-2,kd)-wtam(i1,i2+1,i3+2,kd)+
     & wtam(i1,i2+1,i3-2,kd) +wtam(i1,i2+2,i3-1,kd)-wtam(i1,i2-2,i3-1,
     & kd)-wtam(i1,i2+2,i3+1,kd)+wtam(i1,i2-2,i3+1,kd)) +64.*(wtam(i1,
     & i2+1,i3+1,kd)-wtam(i1,i2-1,i3+1,kd)-wtam(i1,i2+1,i3-1,kd)+wtam(
     & i1,i2-1,i3-1,kd)) )*(h41(1)*h41(2))
      wtamx41r(i1,i2,i3,kd)= wtamx43r(i1,i2,i3,kd)
      wtamy41r(i1,i2,i3,kd)= wtamy43r(i1,i2,i3,kd)
      wtamz41r(i1,i2,i3,kd)= wtamz43r(i1,i2,i3,kd)
      wtamxx41r(i1,i2,i3,kd)= wtamxx43r(i1,i2,i3,kd)
      wtamyy41r(i1,i2,i3,kd)= wtamyy43r(i1,i2,i3,kd)
      wtamzz41r(i1,i2,i3,kd)= wtamzz43r(i1,i2,i3,kd)
      wtamxy41r(i1,i2,i3,kd)= wtamxy43r(i1,i2,i3,kd)
      wtamxz41r(i1,i2,i3,kd)= wtamxz43r(i1,i2,i3,kd)
      wtamyz41r(i1,i2,i3,kd)= wtamyz43r(i1,i2,i3,kd)
      wtamlaplacian41r(i1,i2,i3,kd)=wtamxx43r(i1,i2,i3,kd)
      wtamx42r(i1,i2,i3,kd)= wtamx43r(i1,i2,i3,kd)
      wtamy42r(i1,i2,i3,kd)= wtamy43r(i1,i2,i3,kd)
      wtamz42r(i1,i2,i3,kd)= wtamz43r(i1,i2,i3,kd)
      wtamxx42r(i1,i2,i3,kd)= wtamxx43r(i1,i2,i3,kd)
      wtamyy42r(i1,i2,i3,kd)= wtamyy43r(i1,i2,i3,kd)
      wtamzz42r(i1,i2,i3,kd)= wtamzz43r(i1,i2,i3,kd)
      wtamxy42r(i1,i2,i3,kd)= wtamxy43r(i1,i2,i3,kd)
      wtamxz42r(i1,i2,i3,kd)= wtamxz43r(i1,i2,i3,kd)
      wtamyz42r(i1,i2,i3,kd)= wtamyz43r(i1,i2,i3,kd)
      wtamlaplacian42r(i1,i2,i3,kd)=wtamxx43r(i1,i2,i3,kd)+wtamyy43r(
     & i1,i2,i3,kd)
      wtamlaplacian43r(i1,i2,i3,kd)=wtamxx43r(i1,i2,i3,kd)+wtamyy43r(
     & i1,i2,i3,kd)+wtamzz43r(i1,i2,i3,kd)

      vtbr4(i1,i2,i3,kd)=(8.*(vtb(i1+1,i2,i3,kd)-vtb(i1-1,i2,i3,kd))-(
     & vtb(i1+2,i2,i3,kd)-vtb(i1-2,i2,i3,kd)))*d14(0)
      vtbs4(i1,i2,i3,kd)=(8.*(vtb(i1,i2+1,i3,kd)-vtb(i1,i2-1,i3,kd))-(
     & vtb(i1,i2+2,i3,kd)-vtb(i1,i2-2,i3,kd)))*d14(1)
      vtbt4(i1,i2,i3,kd)=(8.*(vtb(i1,i2,i3+1,kd)-vtb(i1,i2,i3-1,kd))-(
     & vtb(i1,i2,i3+2,kd)-vtb(i1,i2,i3-2,kd)))*d14(2)
      vtbrr4(i1,i2,i3,kd)=(-30.*vtb(i1,i2,i3,kd)+16.*(vtb(i1+1,i2,i3,
     & kd)+vtb(i1-1,i2,i3,kd))-(vtb(i1+2,i2,i3,kd)+vtb(i1-2,i2,i3,kd))
     &  )*d24(0)
      vtbss4(i1,i2,i3,kd)=(-30.*vtb(i1,i2,i3,kd)+16.*(vtb(i1,i2+1,i3,
     & kd)+vtb(i1,i2-1,i3,kd))-(vtb(i1,i2+2,i3,kd)+vtb(i1,i2-2,i3,kd))
     &  )*d24(1)
      vtbtt4(i1,i2,i3,kd)=(-30.*vtb(i1,i2,i3,kd)+16.*(vtb(i1,i2,i3+1,
     & kd)+vtb(i1,i2,i3-1,kd))-(vtb(i1,i2,i3+2,kd)+vtb(i1,i2,i3-2,kd))
     &  )*d24(2)
      vtbrs4(i1,i2,i3,kd)=(8.*(vtbr4(i1,i2+1,i3,kd)-vtbr4(i1,i2-1,i3,
     & kd))-(vtbr4(i1,i2+2,i3,kd)-vtbr4(i1,i2-2,i3,kd)))*d14(1)
      vtbrt4(i1,i2,i3,kd)=(8.*(vtbr4(i1,i2,i3+1,kd)-vtbr4(i1,i2,i3-1,
     & kd))-(vtbr4(i1,i2,i3+2,kd)-vtbr4(i1,i2,i3-2,kd)))*d14(2)
      vtbst4(i1,i2,i3,kd)=(8.*(vtbs4(i1,i2,i3+1,kd)-vtbs4(i1,i2,i3-1,
     & kd))-(vtbs4(i1,i2,i3+2,kd)-vtbs4(i1,i2,i3-2,kd)))*d14(2)
      vtbx41(i1,i2,i3,kd)= rx(i1,i2,i3)*vtbr4(i1,i2,i3,kd)
      vtby41(i1,i2,i3,kd)=0
      vtbz41(i1,i2,i3,kd)=0
      vtbx42(i1,i2,i3,kd)= rx(i1,i2,i3)*vtbr4(i1,i2,i3,kd)+sx(i1,i2,i3)
     & *vtbs4(i1,i2,i3,kd)
      vtby42(i1,i2,i3,kd)= ry(i1,i2,i3)*vtbr4(i1,i2,i3,kd)+sy(i1,i2,i3)
     & *vtbs4(i1,i2,i3,kd)
      vtbz42(i1,i2,i3,kd)=0
      vtbx43(i1,i2,i3,kd)=rx(i1,i2,i3)*vtbr4(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & vtbs4(i1,i2,i3,kd)+tx(i1,i2,i3)*vtbt4(i1,i2,i3,kd)
      vtby43(i1,i2,i3,kd)=ry(i1,i2,i3)*vtbr4(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & vtbs4(i1,i2,i3,kd)+ty(i1,i2,i3)*vtbt4(i1,i2,i3,kd)
      vtbz43(i1,i2,i3,kd)=rz(i1,i2,i3)*vtbr4(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & vtbs4(i1,i2,i3,kd)+tz(i1,i2,i3)*vtbt4(i1,i2,i3,kd)
      vtbxx41(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vtbrr4(i1,i2,i3,kd)+(
     & rxx42(i1,i2,i3))*vtbr4(i1,i2,i3,kd)
      vtbyy41(i1,i2,i3,kd)=0
      vtbxy41(i1,i2,i3,kd)=0
      vtbxz41(i1,i2,i3,kd)=0
      vtbyz41(i1,i2,i3,kd)=0
      vtbzz41(i1,i2,i3,kd)=0
      vtblaplacian41(i1,i2,i3,kd)=vtbxx41(i1,i2,i3,kd)
      vtbxx42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vtbrr4(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*vtbrs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*vtbss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3))*vtbr4(i1,i2,i3,kd)+(
     & sxx42(i1,i2,i3))*vtbs4(i1,i2,i3,kd)
      vtbyy42(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*vtbrr4(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*vtbrs4(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*vtbss4(i1,i2,i3,kd)+(ryy42(i1,i2,i3))*vtbr4(i1,i2,i3,kd)+(
     & syy42(i1,i2,i3))*vtbs4(i1,i2,i3,kd)
      vtbxy42(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vtbrr4(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & vtbrs4(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vtbss4(i1,i2,i3,
     & kd)+rxy42(i1,i2,i3)*vtbr4(i1,i2,i3,kd)+sxy42(i1,i2,i3)*vtbs4(
     & i1,i2,i3,kd)
      vtbxz42(i1,i2,i3,kd)=0
      vtbyz42(i1,i2,i3,kd)=0
      vtbzz42(i1,i2,i3,kd)=0
      vtblaplacian42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & vtbrr4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)
     & *sy(i1,i2,i3))*vtbrs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2)*vtbss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3)+ryy42(i1,i2,i3))*
     & vtbr4(i1,i2,i3,kd)+(sxx42(i1,i2,i3)+syy42(i1,i2,i3))*vtbs4(i1,
     & i2,i3,kd)
      vtbxx43(i1,i2,i3,kd)=rx(i1,i2,i3)**2*vtbrr4(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*vtbss4(i1,i2,i3,kd)+tx(i1,i2,i3)**2*vtbtt4(i1,i2,i3,
     & kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*vtbrs4(i1,i2,i3,kd)+2.*rx(i1,
     & i2,i3)*tx(i1,i2,i3)*vtbrt4(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,
     & i2,i3)*vtbst4(i1,i2,i3,kd)+rxx43(i1,i2,i3)*vtbr4(i1,i2,i3,kd)+
     & sxx43(i1,i2,i3)*vtbs4(i1,i2,i3,kd)+txx43(i1,i2,i3)*vtbt4(i1,i2,
     & i3,kd)
      vtbyy43(i1,i2,i3,kd)=ry(i1,i2,i3)**2*vtbrr4(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*vtbss4(i1,i2,i3,kd)+ty(i1,i2,i3)**2*vtbtt4(i1,i2,i3,
     & kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*vtbrs4(i1,i2,i3,kd)+2.*ry(i1,
     & i2,i3)*ty(i1,i2,i3)*vtbrt4(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,
     & i2,i3)*vtbst4(i1,i2,i3,kd)+ryy43(i1,i2,i3)*vtbr4(i1,i2,i3,kd)+
     & syy43(i1,i2,i3)*vtbs4(i1,i2,i3,kd)+tyy43(i1,i2,i3)*vtbt4(i1,i2,
     & i3,kd)
      vtbzz43(i1,i2,i3,kd)=rz(i1,i2,i3)**2*vtbrr4(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*vtbss4(i1,i2,i3,kd)+tz(i1,i2,i3)**2*vtbtt4(i1,i2,i3,
     & kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*vtbrs4(i1,i2,i3,kd)+2.*rz(i1,
     & i2,i3)*tz(i1,i2,i3)*vtbrt4(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,
     & i2,i3)*vtbst4(i1,i2,i3,kd)+rzz43(i1,i2,i3)*vtbr4(i1,i2,i3,kd)+
     & szz43(i1,i2,i3)*vtbs4(i1,i2,i3,kd)+tzz43(i1,i2,i3)*vtbt4(i1,i2,
     & i3,kd)
      vtbxy43(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vtbrr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vtbss4(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & ty(i1,i2,i3)*vtbtt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(
     & i1,i2,i3)*sx(i1,i2,i3))*vtbrs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(
     & i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*vtbrt4(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*vtbst4(i1,i2,
     & i3,kd)+rxy43(i1,i2,i3)*vtbr4(i1,i2,i3,kd)+sxy43(i1,i2,i3)*
     & vtbs4(i1,i2,i3,kd)+txy43(i1,i2,i3)*vtbt4(i1,i2,i3,kd)
      vtbxz43(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*vtbrr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*vtbss4(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & tz(i1,i2,i3)*vtbtt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sx(i1,i2,i3))*vtbrs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*vtbrt4(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*vtbst4(i1,i2,
     & i3,kd)+rxz43(i1,i2,i3)*vtbr4(i1,i2,i3,kd)+sxz43(i1,i2,i3)*
     & vtbs4(i1,i2,i3,kd)+txz43(i1,i2,i3)*vtbt4(i1,i2,i3,kd)
      vtbyz43(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*vtbrr4(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*vtbss4(i1,i2,i3,kd)+ty(i1,i2,i3)*
     & tz(i1,i2,i3)*vtbtt4(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sy(i1,i2,i3))*vtbrs4(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*vtbrt4(i1,i2,i3,kd)+(sy(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*vtbst4(i1,i2,
     & i3,kd)+ryz43(i1,i2,i3)*vtbr4(i1,i2,i3,kd)+syz43(i1,i2,i3)*
     & vtbs4(i1,i2,i3,kd)+tyz43(i1,i2,i3)*vtbt4(i1,i2,i3,kd)
      vtblaplacian43(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*vtbrr4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)
     & **2+sz(i1,i2,i3)**2)*vtbss4(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(
     & i1,i2,i3)**2+tz(i1,i2,i3)**2)*vtbtt4(i1,i2,i3,kd)+2.*(rx(i1,i2,
     & i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,
     & i2,i3))*vtbrs4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(
     & i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*vtbrt4(i1,i2,
     & i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)
     & +sz(i1,i2,i3)*tz(i1,i2,i3))*vtbst4(i1,i2,i3,kd)+(rxx43(i1,i2,
     & i3)+ryy43(i1,i2,i3)+rzz43(i1,i2,i3))*vtbr4(i1,i2,i3,kd)+(sxx43(
     & i1,i2,i3)+syy43(i1,i2,i3)+szz43(i1,i2,i3))*vtbs4(i1,i2,i3,kd)+(
     & txx43(i1,i2,i3)+tyy43(i1,i2,i3)+tzz43(i1,i2,i3))*vtbt4(i1,i2,
     & i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      vtbx43r(i1,i2,i3,kd)=(8.*(vtb(i1+1,i2,i3,kd)-vtb(i1-1,i2,i3,kd))-
     & (vtb(i1+2,i2,i3,kd)-vtb(i1-2,i2,i3,kd)))*h41(0)
      vtby43r(i1,i2,i3,kd)=(8.*(vtb(i1,i2+1,i3,kd)-vtb(i1,i2-1,i3,kd))-
     & (vtb(i1,i2+2,i3,kd)-vtb(i1,i2-2,i3,kd)))*h41(1)
      vtbz43r(i1,i2,i3,kd)=(8.*(vtb(i1,i2,i3+1,kd)-vtb(i1,i2,i3-1,kd))-
     & (vtb(i1,i2,i3+2,kd)-vtb(i1,i2,i3-2,kd)))*h41(2)
      vtbxx43r(i1,i2,i3,kd)=( -30.*vtb(i1,i2,i3,kd)+16.*(vtb(i1+1,i2,
     & i3,kd)+vtb(i1-1,i2,i3,kd))-(vtb(i1+2,i2,i3,kd)+vtb(i1-2,i2,i3,
     & kd)) )*h42(0)
      vtbyy43r(i1,i2,i3,kd)=( -30.*vtb(i1,i2,i3,kd)+16.*(vtb(i1,i2+1,
     & i3,kd)+vtb(i1,i2-1,i3,kd))-(vtb(i1,i2+2,i3,kd)+vtb(i1,i2-2,i3,
     & kd)) )*h42(1)
      vtbzz43r(i1,i2,i3,kd)=( -30.*vtb(i1,i2,i3,kd)+16.*(vtb(i1,i2,i3+
     & 1,kd)+vtb(i1,i2,i3-1,kd))-(vtb(i1,i2,i3+2,kd)+vtb(i1,i2,i3-2,
     & kd)) )*h42(2)
      vtbxy43r(i1,i2,i3,kd)=( (vtb(i1+2,i2+2,i3,kd)-vtb(i1-2,i2+2,i3,
     & kd)- vtb(i1+2,i2-2,i3,kd)+vtb(i1-2,i2-2,i3,kd)) +8.*(vtb(i1-1,
     & i2+2,i3,kd)-vtb(i1-1,i2-2,i3,kd)-vtb(i1+1,i2+2,i3,kd)+vtb(i1+1,
     & i2-2,i3,kd) +vtb(i1+2,i2-1,i3,kd)-vtb(i1-2,i2-1,i3,kd)-vtb(i1+
     & 2,i2+1,i3,kd)+vtb(i1-2,i2+1,i3,kd))+64.*(vtb(i1+1,i2+1,i3,kd)-
     & vtb(i1-1,i2+1,i3,kd)- vtb(i1+1,i2-1,i3,kd)+vtb(i1-1,i2-1,i3,kd)
     & ))*(h41(0)*h41(1))
      vtbxz43r(i1,i2,i3,kd)=( (vtb(i1+2,i2,i3+2,kd)-vtb(i1-2,i2,i3+2,
     & kd)-vtb(i1+2,i2,i3-2,kd)+vtb(i1-2,i2,i3-2,kd)) +8.*(vtb(i1-1,
     & i2,i3+2,kd)-vtb(i1-1,i2,i3-2,kd)-vtb(i1+1,i2,i3+2,kd)+vtb(i1+1,
     & i2,i3-2,kd) +vtb(i1+2,i2,i3-1,kd)-vtb(i1-2,i2,i3-1,kd)- vtb(i1+
     & 2,i2,i3+1,kd)+vtb(i1-2,i2,i3+1,kd)) +64.*(vtb(i1+1,i2,i3+1,kd)-
     & vtb(i1-1,i2,i3+1,kd)-vtb(i1+1,i2,i3-1,kd)+vtb(i1-1,i2,i3-1,kd))
     &  )*(h41(0)*h41(2))
      vtbyz43r(i1,i2,i3,kd)=( (vtb(i1,i2+2,i3+2,kd)-vtb(i1,i2-2,i3+2,
     & kd)-vtb(i1,i2+2,i3-2,kd)+vtb(i1,i2-2,i3-2,kd)) +8.*(vtb(i1,i2-
     & 1,i3+2,kd)-vtb(i1,i2-1,i3-2,kd)-vtb(i1,i2+1,i3+2,kd)+vtb(i1,i2+
     & 1,i3-2,kd) +vtb(i1,i2+2,i3-1,kd)-vtb(i1,i2-2,i3-1,kd)-vtb(i1,
     & i2+2,i3+1,kd)+vtb(i1,i2-2,i3+1,kd)) +64.*(vtb(i1,i2+1,i3+1,kd)-
     & vtb(i1,i2-1,i3+1,kd)-vtb(i1,i2+1,i3-1,kd)+vtb(i1,i2-1,i3-1,kd))
     &  )*(h41(1)*h41(2))
      vtbx41r(i1,i2,i3,kd)= vtbx43r(i1,i2,i3,kd)
      vtby41r(i1,i2,i3,kd)= vtby43r(i1,i2,i3,kd)
      vtbz41r(i1,i2,i3,kd)= vtbz43r(i1,i2,i3,kd)
      vtbxx41r(i1,i2,i3,kd)= vtbxx43r(i1,i2,i3,kd)
      vtbyy41r(i1,i2,i3,kd)= vtbyy43r(i1,i2,i3,kd)
      vtbzz41r(i1,i2,i3,kd)= vtbzz43r(i1,i2,i3,kd)
      vtbxy41r(i1,i2,i3,kd)= vtbxy43r(i1,i2,i3,kd)
      vtbxz41r(i1,i2,i3,kd)= vtbxz43r(i1,i2,i3,kd)
      vtbyz41r(i1,i2,i3,kd)= vtbyz43r(i1,i2,i3,kd)
      vtblaplacian41r(i1,i2,i3,kd)=vtbxx43r(i1,i2,i3,kd)
      vtbx42r(i1,i2,i3,kd)= vtbx43r(i1,i2,i3,kd)
      vtby42r(i1,i2,i3,kd)= vtby43r(i1,i2,i3,kd)
      vtbz42r(i1,i2,i3,kd)= vtbz43r(i1,i2,i3,kd)
      vtbxx42r(i1,i2,i3,kd)= vtbxx43r(i1,i2,i3,kd)
      vtbyy42r(i1,i2,i3,kd)= vtbyy43r(i1,i2,i3,kd)
      vtbzz42r(i1,i2,i3,kd)= vtbzz43r(i1,i2,i3,kd)
      vtbxy42r(i1,i2,i3,kd)= vtbxy43r(i1,i2,i3,kd)
      vtbxz42r(i1,i2,i3,kd)= vtbxz43r(i1,i2,i3,kd)
      vtbyz42r(i1,i2,i3,kd)= vtbyz43r(i1,i2,i3,kd)
      vtblaplacian42r(i1,i2,i3,kd)=vtbxx43r(i1,i2,i3,kd)+vtbyy43r(i1,
     & i2,i3,kd)
      vtblaplacian43r(i1,i2,i3,kd)=vtbxx43r(i1,i2,i3,kd)+vtbyy43r(i1,
     & i2,i3,kd)+vtbzz43r(i1,i2,i3,kd)
      vtbmr4(i1,i2,i3,kd)=(8.*(vtbm(i1+1,i2,i3,kd)-vtbm(i1-1,i2,i3,kd))
     & -(vtbm(i1+2,i2,i3,kd)-vtbm(i1-2,i2,i3,kd)))*d14(0)
      vtbms4(i1,i2,i3,kd)=(8.*(vtbm(i1,i2+1,i3,kd)-vtbm(i1,i2-1,i3,kd))
     & -(vtbm(i1,i2+2,i3,kd)-vtbm(i1,i2-2,i3,kd)))*d14(1)
      vtbmt4(i1,i2,i3,kd)=(8.*(vtbm(i1,i2,i3+1,kd)-vtbm(i1,i2,i3-1,kd))
     & -(vtbm(i1,i2,i3+2,kd)-vtbm(i1,i2,i3-2,kd)))*d14(2)
      vtbmrr4(i1,i2,i3,kd)=(-30.*vtbm(i1,i2,i3,kd)+16.*(vtbm(i1+1,i2,
     & i3,kd)+vtbm(i1-1,i2,i3,kd))-(vtbm(i1+2,i2,i3,kd)+vtbm(i1-2,i2,
     & i3,kd)) )*d24(0)
      vtbmss4(i1,i2,i3,kd)=(-30.*vtbm(i1,i2,i3,kd)+16.*(vtbm(i1,i2+1,
     & i3,kd)+vtbm(i1,i2-1,i3,kd))-(vtbm(i1,i2+2,i3,kd)+vtbm(i1,i2-2,
     & i3,kd)) )*d24(1)
      vtbmtt4(i1,i2,i3,kd)=(-30.*vtbm(i1,i2,i3,kd)+16.*(vtbm(i1,i2,i3+
     & 1,kd)+vtbm(i1,i2,i3-1,kd))-(vtbm(i1,i2,i3+2,kd)+vtbm(i1,i2,i3-
     & 2,kd)) )*d24(2)
      vtbmrs4(i1,i2,i3,kd)=(8.*(vtbmr4(i1,i2+1,i3,kd)-vtbmr4(i1,i2-1,
     & i3,kd))-(vtbmr4(i1,i2+2,i3,kd)-vtbmr4(i1,i2-2,i3,kd)))*d14(1)
      vtbmrt4(i1,i2,i3,kd)=(8.*(vtbmr4(i1,i2,i3+1,kd)-vtbmr4(i1,i2,i3-
     & 1,kd))-(vtbmr4(i1,i2,i3+2,kd)-vtbmr4(i1,i2,i3-2,kd)))*d14(2)
      vtbmst4(i1,i2,i3,kd)=(8.*(vtbms4(i1,i2,i3+1,kd)-vtbms4(i1,i2,i3-
     & 1,kd))-(vtbms4(i1,i2,i3+2,kd)-vtbms4(i1,i2,i3-2,kd)))*d14(2)
      vtbmx41(i1,i2,i3,kd)= rx(i1,i2,i3)*vtbmr4(i1,i2,i3,kd)
      vtbmy41(i1,i2,i3,kd)=0
      vtbmz41(i1,i2,i3,kd)=0
      vtbmx42(i1,i2,i3,kd)= rx(i1,i2,i3)*vtbmr4(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*vtbms4(i1,i2,i3,kd)
      vtbmy42(i1,i2,i3,kd)= ry(i1,i2,i3)*vtbmr4(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*vtbms4(i1,i2,i3,kd)
      vtbmz42(i1,i2,i3,kd)=0
      vtbmx43(i1,i2,i3,kd)=rx(i1,i2,i3)*vtbmr4(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*vtbms4(i1,i2,i3,kd)+tx(i1,i2,i3)*vtbmt4(i1,i2,i3,kd)
      vtbmy43(i1,i2,i3,kd)=ry(i1,i2,i3)*vtbmr4(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*vtbms4(i1,i2,i3,kd)+ty(i1,i2,i3)*vtbmt4(i1,i2,i3,kd)
      vtbmz43(i1,i2,i3,kd)=rz(i1,i2,i3)*vtbmr4(i1,i2,i3,kd)+sz(i1,i2,
     & i3)*vtbms4(i1,i2,i3,kd)+tz(i1,i2,i3)*vtbmt4(i1,i2,i3,kd)
      vtbmxx41(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vtbmrr4(i1,i2,i3,kd)+(
     & rxx42(i1,i2,i3))*vtbmr4(i1,i2,i3,kd)
      vtbmyy41(i1,i2,i3,kd)=0
      vtbmxy41(i1,i2,i3,kd)=0
      vtbmxz41(i1,i2,i3,kd)=0
      vtbmyz41(i1,i2,i3,kd)=0
      vtbmzz41(i1,i2,i3,kd)=0
      vtbmlaplacian41(i1,i2,i3,kd)=vtbmxx41(i1,i2,i3,kd)
      vtbmxx42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*vtbmrr4(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*vtbmrs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*vtbmss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3))*vtbmr4(i1,i2,i3,kd)+(
     & sxx42(i1,i2,i3))*vtbms4(i1,i2,i3,kd)
      vtbmyy42(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*vtbmrr4(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*vtbmrs4(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*vtbmss4(i1,i2,i3,kd)+(ryy42(i1,i2,i3))*vtbmr4(i1,i2,i3,kd)+(
     & syy42(i1,i2,i3))*vtbms4(i1,i2,i3,kd)
      vtbmxy42(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vtbmrr4(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & vtbmrs4(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vtbmss4(i1,i2,
     & i3,kd)+rxy42(i1,i2,i3)*vtbmr4(i1,i2,i3,kd)+sxy42(i1,i2,i3)*
     & vtbms4(i1,i2,i3,kd)
      vtbmxz42(i1,i2,i3,kd)=0
      vtbmyz42(i1,i2,i3,kd)=0
      vtbmzz42(i1,i2,i3,kd)=0
      vtbmlaplacian42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & vtbmrr4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,
     & i3)*sy(i1,i2,i3))*vtbmrs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,
     & i2,i3)**2)*vtbmss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3)+ryy42(i1,i2,
     & i3))*vtbmr4(i1,i2,i3,kd)+(sxx42(i1,i2,i3)+syy42(i1,i2,i3))*
     & vtbms4(i1,i2,i3,kd)
      vtbmxx43(i1,i2,i3,kd)=rx(i1,i2,i3)**2*vtbmrr4(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*vtbmss4(i1,i2,i3,kd)+tx(i1,i2,i3)**2*vtbmtt4(i1,i2,
     & i3,kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*vtbmrs4(i1,i2,i3,kd)+2.*rx(
     & i1,i2,i3)*tx(i1,i2,i3)*vtbmrt4(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(
     & i1,i2,i3)*vtbmst4(i1,i2,i3,kd)+rxx43(i1,i2,i3)*vtbmr4(i1,i2,i3,
     & kd)+sxx43(i1,i2,i3)*vtbms4(i1,i2,i3,kd)+txx43(i1,i2,i3)*vtbmt4(
     & i1,i2,i3,kd)
      vtbmyy43(i1,i2,i3,kd)=ry(i1,i2,i3)**2*vtbmrr4(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*vtbmss4(i1,i2,i3,kd)+ty(i1,i2,i3)**2*vtbmtt4(i1,i2,
     & i3,kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*vtbmrs4(i1,i2,i3,kd)+2.*ry(
     & i1,i2,i3)*ty(i1,i2,i3)*vtbmrt4(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(
     & i1,i2,i3)*vtbmst4(i1,i2,i3,kd)+ryy43(i1,i2,i3)*vtbmr4(i1,i2,i3,
     & kd)+syy43(i1,i2,i3)*vtbms4(i1,i2,i3,kd)+tyy43(i1,i2,i3)*vtbmt4(
     & i1,i2,i3,kd)
      vtbmzz43(i1,i2,i3,kd)=rz(i1,i2,i3)**2*vtbmrr4(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*vtbmss4(i1,i2,i3,kd)+tz(i1,i2,i3)**2*vtbmtt4(i1,i2,
     & i3,kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*vtbmrs4(i1,i2,i3,kd)+2.*rz(
     & i1,i2,i3)*tz(i1,i2,i3)*vtbmrt4(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(
     & i1,i2,i3)*vtbmst4(i1,i2,i3,kd)+rzz43(i1,i2,i3)*vtbmr4(i1,i2,i3,
     & kd)+szz43(i1,i2,i3)*vtbms4(i1,i2,i3,kd)+tzz43(i1,i2,i3)*vtbmt4(
     & i1,i2,i3,kd)
      vtbmxy43(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*vtbmrr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*vtbmss4(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *ty(i1,i2,i3)*vtbmtt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+
     & ry(i1,i2,i3)*sx(i1,i2,i3))*vtbmrs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & ty(i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*vtbmrt4(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*vtbmst4(
     & i1,i2,i3,kd)+rxy43(i1,i2,i3)*vtbmr4(i1,i2,i3,kd)+sxy43(i1,i2,
     & i3)*vtbms4(i1,i2,i3,kd)+txy43(i1,i2,i3)*vtbmt4(i1,i2,i3,kd)
      vtbmxz43(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*vtbmrr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*vtbmss4(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *tz(i1,i2,i3)*vtbmtt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sx(i1,i2,i3))*vtbmrs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*vtbmrt4(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*vtbmst4(
     & i1,i2,i3,kd)+rxz43(i1,i2,i3)*vtbmr4(i1,i2,i3,kd)+sxz43(i1,i2,
     & i3)*vtbms4(i1,i2,i3,kd)+txz43(i1,i2,i3)*vtbmt4(i1,i2,i3,kd)
      vtbmyz43(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*vtbmrr4(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*vtbmss4(i1,i2,i3,kd)+ty(i1,i2,i3)
     & *tz(i1,i2,i3)*vtbmtt4(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sy(i1,i2,i3))*vtbmrs4(i1,i2,i3,kd)+(ry(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*vtbmrt4(i1,i2,i3,kd)+(
     & sy(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*vtbmst4(
     & i1,i2,i3,kd)+ryz43(i1,i2,i3)*vtbmr4(i1,i2,i3,kd)+syz43(i1,i2,
     & i3)*vtbms4(i1,i2,i3,kd)+tyz43(i1,i2,i3)*vtbmt4(i1,i2,i3,kd)
      vtbmlaplacian43(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*vtbmrr4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2+sz(i1,i2,i3)**2)*vtbmss4(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+
     & ty(i1,i2,i3)**2+tz(i1,i2,i3)**2)*vtbmtt4(i1,i2,i3,kd)+2.*(rx(
     & i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*
     & sz(i1,i2,i3))*vtbmrs4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,
     & i3)+ ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*
     & vtbmrt4(i1,i2,i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,
     & i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3))*vtbmst4(i1,i2,i3,
     & kd)+(rxx43(i1,i2,i3)+ryy43(i1,i2,i3)+rzz43(i1,i2,i3))*vtbmr4(
     & i1,i2,i3,kd)+(sxx43(i1,i2,i3)+syy43(i1,i2,i3)+szz43(i1,i2,i3))*
     & vtbms4(i1,i2,i3,kd)+(txx43(i1,i2,i3)+tyy43(i1,i2,i3)+tzz43(i1,
     & i2,i3))*vtbmt4(i1,i2,i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      vtbmx43r(i1,i2,i3,kd)=(8.*(vtbm(i1+1,i2,i3,kd)-vtbm(i1-1,i2,i3,
     & kd))-(vtbm(i1+2,i2,i3,kd)-vtbm(i1-2,i2,i3,kd)))*h41(0)
      vtbmy43r(i1,i2,i3,kd)=(8.*(vtbm(i1,i2+1,i3,kd)-vtbm(i1,i2-1,i3,
     & kd))-(vtbm(i1,i2+2,i3,kd)-vtbm(i1,i2-2,i3,kd)))*h41(1)
      vtbmz43r(i1,i2,i3,kd)=(8.*(vtbm(i1,i2,i3+1,kd)-vtbm(i1,i2,i3-1,
     & kd))-(vtbm(i1,i2,i3+2,kd)-vtbm(i1,i2,i3-2,kd)))*h41(2)
      vtbmxx43r(i1,i2,i3,kd)=( -30.*vtbm(i1,i2,i3,kd)+16.*(vtbm(i1+1,
     & i2,i3,kd)+vtbm(i1-1,i2,i3,kd))-(vtbm(i1+2,i2,i3,kd)+vtbm(i1-2,
     & i2,i3,kd)) )*h42(0)
      vtbmyy43r(i1,i2,i3,kd)=( -30.*vtbm(i1,i2,i3,kd)+16.*(vtbm(i1,i2+
     & 1,i3,kd)+vtbm(i1,i2-1,i3,kd))-(vtbm(i1,i2+2,i3,kd)+vtbm(i1,i2-
     & 2,i3,kd)) )*h42(1)
      vtbmzz43r(i1,i2,i3,kd)=( -30.*vtbm(i1,i2,i3,kd)+16.*(vtbm(i1,i2,
     & i3+1,kd)+vtbm(i1,i2,i3-1,kd))-(vtbm(i1,i2,i3+2,kd)+vtbm(i1,i2,
     & i3-2,kd)) )*h42(2)
      vtbmxy43r(i1,i2,i3,kd)=( (vtbm(i1+2,i2+2,i3,kd)-vtbm(i1-2,i2+2,
     & i3,kd)- vtbm(i1+2,i2-2,i3,kd)+vtbm(i1-2,i2-2,i3,kd)) +8.*(vtbm(
     & i1-1,i2+2,i3,kd)-vtbm(i1-1,i2-2,i3,kd)-vtbm(i1+1,i2+2,i3,kd)+
     & vtbm(i1+1,i2-2,i3,kd) +vtbm(i1+2,i2-1,i3,kd)-vtbm(i1-2,i2-1,i3,
     & kd)-vtbm(i1+2,i2+1,i3,kd)+vtbm(i1-2,i2+1,i3,kd))+64.*(vtbm(i1+
     & 1,i2+1,i3,kd)-vtbm(i1-1,i2+1,i3,kd)- vtbm(i1+1,i2-1,i3,kd)+
     & vtbm(i1-1,i2-1,i3,kd)))*(h41(0)*h41(1))
      vtbmxz43r(i1,i2,i3,kd)=( (vtbm(i1+2,i2,i3+2,kd)-vtbm(i1-2,i2,i3+
     & 2,kd)-vtbm(i1+2,i2,i3-2,kd)+vtbm(i1-2,i2,i3-2,kd)) +8.*(vtbm(
     & i1-1,i2,i3+2,kd)-vtbm(i1-1,i2,i3-2,kd)-vtbm(i1+1,i2,i3+2,kd)+
     & vtbm(i1+1,i2,i3-2,kd) +vtbm(i1+2,i2,i3-1,kd)-vtbm(i1-2,i2,i3-1,
     & kd)- vtbm(i1+2,i2,i3+1,kd)+vtbm(i1-2,i2,i3+1,kd)) +64.*(vtbm(
     & i1+1,i2,i3+1,kd)-vtbm(i1-1,i2,i3+1,kd)-vtbm(i1+1,i2,i3-1,kd)+
     & vtbm(i1-1,i2,i3-1,kd)) )*(h41(0)*h41(2))
      vtbmyz43r(i1,i2,i3,kd)=( (vtbm(i1,i2+2,i3+2,kd)-vtbm(i1,i2-2,i3+
     & 2,kd)-vtbm(i1,i2+2,i3-2,kd)+vtbm(i1,i2-2,i3-2,kd)) +8.*(vtbm(
     & i1,i2-1,i3+2,kd)-vtbm(i1,i2-1,i3-2,kd)-vtbm(i1,i2+1,i3+2,kd)+
     & vtbm(i1,i2+1,i3-2,kd) +vtbm(i1,i2+2,i3-1,kd)-vtbm(i1,i2-2,i3-1,
     & kd)-vtbm(i1,i2+2,i3+1,kd)+vtbm(i1,i2-2,i3+1,kd)) +64.*(vtbm(i1,
     & i2+1,i3+1,kd)-vtbm(i1,i2-1,i3+1,kd)-vtbm(i1,i2+1,i3-1,kd)+vtbm(
     & i1,i2-1,i3-1,kd)) )*(h41(1)*h41(2))
      vtbmx41r(i1,i2,i3,kd)= vtbmx43r(i1,i2,i3,kd)
      vtbmy41r(i1,i2,i3,kd)= vtbmy43r(i1,i2,i3,kd)
      vtbmz41r(i1,i2,i3,kd)= vtbmz43r(i1,i2,i3,kd)
      vtbmxx41r(i1,i2,i3,kd)= vtbmxx43r(i1,i2,i3,kd)
      vtbmyy41r(i1,i2,i3,kd)= vtbmyy43r(i1,i2,i3,kd)
      vtbmzz41r(i1,i2,i3,kd)= vtbmzz43r(i1,i2,i3,kd)
      vtbmxy41r(i1,i2,i3,kd)= vtbmxy43r(i1,i2,i3,kd)
      vtbmxz41r(i1,i2,i3,kd)= vtbmxz43r(i1,i2,i3,kd)
      vtbmyz41r(i1,i2,i3,kd)= vtbmyz43r(i1,i2,i3,kd)
      vtbmlaplacian41r(i1,i2,i3,kd)=vtbmxx43r(i1,i2,i3,kd)
      vtbmx42r(i1,i2,i3,kd)= vtbmx43r(i1,i2,i3,kd)
      vtbmy42r(i1,i2,i3,kd)= vtbmy43r(i1,i2,i3,kd)
      vtbmz42r(i1,i2,i3,kd)= vtbmz43r(i1,i2,i3,kd)
      vtbmxx42r(i1,i2,i3,kd)= vtbmxx43r(i1,i2,i3,kd)
      vtbmyy42r(i1,i2,i3,kd)= vtbmyy43r(i1,i2,i3,kd)
      vtbmzz42r(i1,i2,i3,kd)= vtbmzz43r(i1,i2,i3,kd)
      vtbmxy42r(i1,i2,i3,kd)= vtbmxy43r(i1,i2,i3,kd)
      vtbmxz42r(i1,i2,i3,kd)= vtbmxz43r(i1,i2,i3,kd)
      vtbmyz42r(i1,i2,i3,kd)= vtbmyz43r(i1,i2,i3,kd)
      vtbmlaplacian42r(i1,i2,i3,kd)=vtbmxx43r(i1,i2,i3,kd)+vtbmyy43r(
     & i1,i2,i3,kd)
      vtbmlaplacian43r(i1,i2,i3,kd)=vtbmxx43r(i1,i2,i3,kd)+vtbmyy43r(
     & i1,i2,i3,kd)+vtbmzz43r(i1,i2,i3,kd)
      wtbr4(i1,i2,i3,kd)=(8.*(wtb(i1+1,i2,i3,kd)-wtb(i1-1,i2,i3,kd))-(
     & wtb(i1+2,i2,i3,kd)-wtb(i1-2,i2,i3,kd)))*d14(0)
      wtbs4(i1,i2,i3,kd)=(8.*(wtb(i1,i2+1,i3,kd)-wtb(i1,i2-1,i3,kd))-(
     & wtb(i1,i2+2,i3,kd)-wtb(i1,i2-2,i3,kd)))*d14(1)
      wtbt4(i1,i2,i3,kd)=(8.*(wtb(i1,i2,i3+1,kd)-wtb(i1,i2,i3-1,kd))-(
     & wtb(i1,i2,i3+2,kd)-wtb(i1,i2,i3-2,kd)))*d14(2)
      wtbrr4(i1,i2,i3,kd)=(-30.*wtb(i1,i2,i3,kd)+16.*(wtb(i1+1,i2,i3,
     & kd)+wtb(i1-1,i2,i3,kd))-(wtb(i1+2,i2,i3,kd)+wtb(i1-2,i2,i3,kd))
     &  )*d24(0)
      wtbss4(i1,i2,i3,kd)=(-30.*wtb(i1,i2,i3,kd)+16.*(wtb(i1,i2+1,i3,
     & kd)+wtb(i1,i2-1,i3,kd))-(wtb(i1,i2+2,i3,kd)+wtb(i1,i2-2,i3,kd))
     &  )*d24(1)
      wtbtt4(i1,i2,i3,kd)=(-30.*wtb(i1,i2,i3,kd)+16.*(wtb(i1,i2,i3+1,
     & kd)+wtb(i1,i2,i3-1,kd))-(wtb(i1,i2,i3+2,kd)+wtb(i1,i2,i3-2,kd))
     &  )*d24(2)
      wtbrs4(i1,i2,i3,kd)=(8.*(wtbr4(i1,i2+1,i3,kd)-wtbr4(i1,i2-1,i3,
     & kd))-(wtbr4(i1,i2+2,i3,kd)-wtbr4(i1,i2-2,i3,kd)))*d14(1)
      wtbrt4(i1,i2,i3,kd)=(8.*(wtbr4(i1,i2,i3+1,kd)-wtbr4(i1,i2,i3-1,
     & kd))-(wtbr4(i1,i2,i3+2,kd)-wtbr4(i1,i2,i3-2,kd)))*d14(2)
      wtbst4(i1,i2,i3,kd)=(8.*(wtbs4(i1,i2,i3+1,kd)-wtbs4(i1,i2,i3-1,
     & kd))-(wtbs4(i1,i2,i3+2,kd)-wtbs4(i1,i2,i3-2,kd)))*d14(2)
      wtbx41(i1,i2,i3,kd)= rx(i1,i2,i3)*wtbr4(i1,i2,i3,kd)
      wtby41(i1,i2,i3,kd)=0
      wtbz41(i1,i2,i3,kd)=0
      wtbx42(i1,i2,i3,kd)= rx(i1,i2,i3)*wtbr4(i1,i2,i3,kd)+sx(i1,i2,i3)
     & *wtbs4(i1,i2,i3,kd)
      wtby42(i1,i2,i3,kd)= ry(i1,i2,i3)*wtbr4(i1,i2,i3,kd)+sy(i1,i2,i3)
     & *wtbs4(i1,i2,i3,kd)
      wtbz42(i1,i2,i3,kd)=0
      wtbx43(i1,i2,i3,kd)=rx(i1,i2,i3)*wtbr4(i1,i2,i3,kd)+sx(i1,i2,i3)*
     & wtbs4(i1,i2,i3,kd)+tx(i1,i2,i3)*wtbt4(i1,i2,i3,kd)
      wtby43(i1,i2,i3,kd)=ry(i1,i2,i3)*wtbr4(i1,i2,i3,kd)+sy(i1,i2,i3)*
     & wtbs4(i1,i2,i3,kd)+ty(i1,i2,i3)*wtbt4(i1,i2,i3,kd)
      wtbz43(i1,i2,i3,kd)=rz(i1,i2,i3)*wtbr4(i1,i2,i3,kd)+sz(i1,i2,i3)*
     & wtbs4(i1,i2,i3,kd)+tz(i1,i2,i3)*wtbt4(i1,i2,i3,kd)
      wtbxx41(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wtbrr4(i1,i2,i3,kd)+(
     & rxx42(i1,i2,i3))*wtbr4(i1,i2,i3,kd)
      wtbyy41(i1,i2,i3,kd)=0
      wtbxy41(i1,i2,i3,kd)=0
      wtbxz41(i1,i2,i3,kd)=0
      wtbyz41(i1,i2,i3,kd)=0
      wtbzz41(i1,i2,i3,kd)=0
      wtblaplacian41(i1,i2,i3,kd)=wtbxx41(i1,i2,i3,kd)
      wtbxx42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wtbrr4(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*wtbrs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*wtbss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3))*wtbr4(i1,i2,i3,kd)+(
     & sxx42(i1,i2,i3))*wtbs4(i1,i2,i3,kd)
      wtbyy42(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*wtbrr4(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*wtbrs4(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*wtbss4(i1,i2,i3,kd)+(ryy42(i1,i2,i3))*wtbr4(i1,i2,i3,kd)+(
     & syy42(i1,i2,i3))*wtbs4(i1,i2,i3,kd)
      wtbxy42(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wtbrr4(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & wtbrs4(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wtbss4(i1,i2,i3,
     & kd)+rxy42(i1,i2,i3)*wtbr4(i1,i2,i3,kd)+sxy42(i1,i2,i3)*wtbs4(
     & i1,i2,i3,kd)
      wtbxz42(i1,i2,i3,kd)=0
      wtbyz42(i1,i2,i3,kd)=0
      wtbzz42(i1,i2,i3,kd)=0
      wtblaplacian42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & wtbrr4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)
     & *sy(i1,i2,i3))*wtbrs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2)*wtbss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3)+ryy42(i1,i2,i3))*
     & wtbr4(i1,i2,i3,kd)+(sxx42(i1,i2,i3)+syy42(i1,i2,i3))*wtbs4(i1,
     & i2,i3,kd)
      wtbxx43(i1,i2,i3,kd)=rx(i1,i2,i3)**2*wtbrr4(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*wtbss4(i1,i2,i3,kd)+tx(i1,i2,i3)**2*wtbtt4(i1,i2,i3,
     & kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*wtbrs4(i1,i2,i3,kd)+2.*rx(i1,
     & i2,i3)*tx(i1,i2,i3)*wtbrt4(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(i1,
     & i2,i3)*wtbst4(i1,i2,i3,kd)+rxx43(i1,i2,i3)*wtbr4(i1,i2,i3,kd)+
     & sxx43(i1,i2,i3)*wtbs4(i1,i2,i3,kd)+txx43(i1,i2,i3)*wtbt4(i1,i2,
     & i3,kd)
      wtbyy43(i1,i2,i3,kd)=ry(i1,i2,i3)**2*wtbrr4(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*wtbss4(i1,i2,i3,kd)+ty(i1,i2,i3)**2*wtbtt4(i1,i2,i3,
     & kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*wtbrs4(i1,i2,i3,kd)+2.*ry(i1,
     & i2,i3)*ty(i1,i2,i3)*wtbrt4(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(i1,
     & i2,i3)*wtbst4(i1,i2,i3,kd)+ryy43(i1,i2,i3)*wtbr4(i1,i2,i3,kd)+
     & syy43(i1,i2,i3)*wtbs4(i1,i2,i3,kd)+tyy43(i1,i2,i3)*wtbt4(i1,i2,
     & i3,kd)
      wtbzz43(i1,i2,i3,kd)=rz(i1,i2,i3)**2*wtbrr4(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*wtbss4(i1,i2,i3,kd)+tz(i1,i2,i3)**2*wtbtt4(i1,i2,i3,
     & kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*wtbrs4(i1,i2,i3,kd)+2.*rz(i1,
     & i2,i3)*tz(i1,i2,i3)*wtbrt4(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(i1,
     & i2,i3)*wtbst4(i1,i2,i3,kd)+rzz43(i1,i2,i3)*wtbr4(i1,i2,i3,kd)+
     & szz43(i1,i2,i3)*wtbs4(i1,i2,i3,kd)+tzz43(i1,i2,i3)*wtbt4(i1,i2,
     & i3,kd)
      wtbxy43(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wtbrr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wtbss4(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & ty(i1,i2,i3)*wtbtt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(
     & i1,i2,i3)*sx(i1,i2,i3))*wtbrs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*ty(
     & i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*wtbrt4(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*wtbst4(i1,i2,
     & i3,kd)+rxy43(i1,i2,i3)*wtbr4(i1,i2,i3,kd)+sxy43(i1,i2,i3)*
     & wtbs4(i1,i2,i3,kd)+txy43(i1,i2,i3)*wtbt4(i1,i2,i3,kd)
      wtbxz43(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*wtbrr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*wtbss4(i1,i2,i3,kd)+tx(i1,i2,i3)*
     & tz(i1,i2,i3)*wtbtt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sx(i1,i2,i3))*wtbrs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*wtbrt4(i1,i2,i3,kd)+(sx(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*wtbst4(i1,i2,
     & i3,kd)+rxz43(i1,i2,i3)*wtbr4(i1,i2,i3,kd)+sxz43(i1,i2,i3)*
     & wtbs4(i1,i2,i3,kd)+txz43(i1,i2,i3)*wtbt4(i1,i2,i3,kd)
      wtbyz43(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*wtbrr4(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*wtbss4(i1,i2,i3,kd)+ty(i1,i2,i3)*
     & tz(i1,i2,i3)*wtbtt4(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+rz(
     & i1,i2,i3)*sy(i1,i2,i3))*wtbrs4(i1,i2,i3,kd)+(ry(i1,i2,i3)*tz(
     & i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*wtbrt4(i1,i2,i3,kd)+(sy(
     & i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*wtbst4(i1,i2,
     & i3,kd)+ryz43(i1,i2,i3)*wtbr4(i1,i2,i3,kd)+syz43(i1,i2,i3)*
     & wtbs4(i1,i2,i3,kd)+tyz43(i1,i2,i3)*wtbt4(i1,i2,i3,kd)
      wtblaplacian43(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*wtbrr4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)
     & **2+sz(i1,i2,i3)**2)*wtbss4(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+ty(
     & i1,i2,i3)**2+tz(i1,i2,i3)**2)*wtbtt4(i1,i2,i3,kd)+2.*(rx(i1,i2,
     & i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,
     & i2,i3))*wtbrs4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,i3)+ ry(
     & i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*wtbrt4(i1,i2,
     & i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,i3)*ty(i1,i2,i3)
     & +sz(i1,i2,i3)*tz(i1,i2,i3))*wtbst4(i1,i2,i3,kd)+(rxx43(i1,i2,
     & i3)+ryy43(i1,i2,i3)+rzz43(i1,i2,i3))*wtbr4(i1,i2,i3,kd)+(sxx43(
     & i1,i2,i3)+syy43(i1,i2,i3)+szz43(i1,i2,i3))*wtbs4(i1,i2,i3,kd)+(
     & txx43(i1,i2,i3)+tyy43(i1,i2,i3)+tzz43(i1,i2,i3))*wtbt4(i1,i2,
     & i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      wtbx43r(i1,i2,i3,kd)=(8.*(wtb(i1+1,i2,i3,kd)-wtb(i1-1,i2,i3,kd))-
     & (wtb(i1+2,i2,i3,kd)-wtb(i1-2,i2,i3,kd)))*h41(0)
      wtby43r(i1,i2,i3,kd)=(8.*(wtb(i1,i2+1,i3,kd)-wtb(i1,i2-1,i3,kd))-
     & (wtb(i1,i2+2,i3,kd)-wtb(i1,i2-2,i3,kd)))*h41(1)
      wtbz43r(i1,i2,i3,kd)=(8.*(wtb(i1,i2,i3+1,kd)-wtb(i1,i2,i3-1,kd))-
     & (wtb(i1,i2,i3+2,kd)-wtb(i1,i2,i3-2,kd)))*h41(2)
      wtbxx43r(i1,i2,i3,kd)=( -30.*wtb(i1,i2,i3,kd)+16.*(wtb(i1+1,i2,
     & i3,kd)+wtb(i1-1,i2,i3,kd))-(wtb(i1+2,i2,i3,kd)+wtb(i1-2,i2,i3,
     & kd)) )*h42(0)
      wtbyy43r(i1,i2,i3,kd)=( -30.*wtb(i1,i2,i3,kd)+16.*(wtb(i1,i2+1,
     & i3,kd)+wtb(i1,i2-1,i3,kd))-(wtb(i1,i2+2,i3,kd)+wtb(i1,i2-2,i3,
     & kd)) )*h42(1)
      wtbzz43r(i1,i2,i3,kd)=( -30.*wtb(i1,i2,i3,kd)+16.*(wtb(i1,i2,i3+
     & 1,kd)+wtb(i1,i2,i3-1,kd))-(wtb(i1,i2,i3+2,kd)+wtb(i1,i2,i3-2,
     & kd)) )*h42(2)
      wtbxy43r(i1,i2,i3,kd)=( (wtb(i1+2,i2+2,i3,kd)-wtb(i1-2,i2+2,i3,
     & kd)- wtb(i1+2,i2-2,i3,kd)+wtb(i1-2,i2-2,i3,kd)) +8.*(wtb(i1-1,
     & i2+2,i3,kd)-wtb(i1-1,i2-2,i3,kd)-wtb(i1+1,i2+2,i3,kd)+wtb(i1+1,
     & i2-2,i3,kd) +wtb(i1+2,i2-1,i3,kd)-wtb(i1-2,i2-1,i3,kd)-wtb(i1+
     & 2,i2+1,i3,kd)+wtb(i1-2,i2+1,i3,kd))+64.*(wtb(i1+1,i2+1,i3,kd)-
     & wtb(i1-1,i2+1,i3,kd)- wtb(i1+1,i2-1,i3,kd)+wtb(i1-1,i2-1,i3,kd)
     & ))*(h41(0)*h41(1))
      wtbxz43r(i1,i2,i3,kd)=( (wtb(i1+2,i2,i3+2,kd)-wtb(i1-2,i2,i3+2,
     & kd)-wtb(i1+2,i2,i3-2,kd)+wtb(i1-2,i2,i3-2,kd)) +8.*(wtb(i1-1,
     & i2,i3+2,kd)-wtb(i1-1,i2,i3-2,kd)-wtb(i1+1,i2,i3+2,kd)+wtb(i1+1,
     & i2,i3-2,kd) +wtb(i1+2,i2,i3-1,kd)-wtb(i1-2,i2,i3-1,kd)- wtb(i1+
     & 2,i2,i3+1,kd)+wtb(i1-2,i2,i3+1,kd)) +64.*(wtb(i1+1,i2,i3+1,kd)-
     & wtb(i1-1,i2,i3+1,kd)-wtb(i1+1,i2,i3-1,kd)+wtb(i1-1,i2,i3-1,kd))
     &  )*(h41(0)*h41(2))
      wtbyz43r(i1,i2,i3,kd)=( (wtb(i1,i2+2,i3+2,kd)-wtb(i1,i2-2,i3+2,
     & kd)-wtb(i1,i2+2,i3-2,kd)+wtb(i1,i2-2,i3-2,kd)) +8.*(wtb(i1,i2-
     & 1,i3+2,kd)-wtb(i1,i2-1,i3-2,kd)-wtb(i1,i2+1,i3+2,kd)+wtb(i1,i2+
     & 1,i3-2,kd) +wtb(i1,i2+2,i3-1,kd)-wtb(i1,i2-2,i3-1,kd)-wtb(i1,
     & i2+2,i3+1,kd)+wtb(i1,i2-2,i3+1,kd)) +64.*(wtb(i1,i2+1,i3+1,kd)-
     & wtb(i1,i2-1,i3+1,kd)-wtb(i1,i2+1,i3-1,kd)+wtb(i1,i2-1,i3-1,kd))
     &  )*(h41(1)*h41(2))
      wtbx41r(i1,i2,i3,kd)= wtbx43r(i1,i2,i3,kd)
      wtby41r(i1,i2,i3,kd)= wtby43r(i1,i2,i3,kd)
      wtbz41r(i1,i2,i3,kd)= wtbz43r(i1,i2,i3,kd)
      wtbxx41r(i1,i2,i3,kd)= wtbxx43r(i1,i2,i3,kd)
      wtbyy41r(i1,i2,i3,kd)= wtbyy43r(i1,i2,i3,kd)
      wtbzz41r(i1,i2,i3,kd)= wtbzz43r(i1,i2,i3,kd)
      wtbxy41r(i1,i2,i3,kd)= wtbxy43r(i1,i2,i3,kd)
      wtbxz41r(i1,i2,i3,kd)= wtbxz43r(i1,i2,i3,kd)
      wtbyz41r(i1,i2,i3,kd)= wtbyz43r(i1,i2,i3,kd)
      wtblaplacian41r(i1,i2,i3,kd)=wtbxx43r(i1,i2,i3,kd)
      wtbx42r(i1,i2,i3,kd)= wtbx43r(i1,i2,i3,kd)
      wtby42r(i1,i2,i3,kd)= wtby43r(i1,i2,i3,kd)
      wtbz42r(i1,i2,i3,kd)= wtbz43r(i1,i2,i3,kd)
      wtbxx42r(i1,i2,i3,kd)= wtbxx43r(i1,i2,i3,kd)
      wtbyy42r(i1,i2,i3,kd)= wtbyy43r(i1,i2,i3,kd)
      wtbzz42r(i1,i2,i3,kd)= wtbzz43r(i1,i2,i3,kd)
      wtbxy42r(i1,i2,i3,kd)= wtbxy43r(i1,i2,i3,kd)
      wtbxz42r(i1,i2,i3,kd)= wtbxz43r(i1,i2,i3,kd)
      wtbyz42r(i1,i2,i3,kd)= wtbyz43r(i1,i2,i3,kd)
      wtblaplacian42r(i1,i2,i3,kd)=wtbxx43r(i1,i2,i3,kd)+wtbyy43r(i1,
     & i2,i3,kd)
      wtblaplacian43r(i1,i2,i3,kd)=wtbxx43r(i1,i2,i3,kd)+wtbyy43r(i1,
     & i2,i3,kd)+wtbzz43r(i1,i2,i3,kd)
      wtbmr4(i1,i2,i3,kd)=(8.*(wtbm(i1+1,i2,i3,kd)-wtbm(i1-1,i2,i3,kd))
     & -(wtbm(i1+2,i2,i3,kd)-wtbm(i1-2,i2,i3,kd)))*d14(0)
      wtbms4(i1,i2,i3,kd)=(8.*(wtbm(i1,i2+1,i3,kd)-wtbm(i1,i2-1,i3,kd))
     & -(wtbm(i1,i2+2,i3,kd)-wtbm(i1,i2-2,i3,kd)))*d14(1)
      wtbmt4(i1,i2,i3,kd)=(8.*(wtbm(i1,i2,i3+1,kd)-wtbm(i1,i2,i3-1,kd))
     & -(wtbm(i1,i2,i3+2,kd)-wtbm(i1,i2,i3-2,kd)))*d14(2)
      wtbmrr4(i1,i2,i3,kd)=(-30.*wtbm(i1,i2,i3,kd)+16.*(wtbm(i1+1,i2,
     & i3,kd)+wtbm(i1-1,i2,i3,kd))-(wtbm(i1+2,i2,i3,kd)+wtbm(i1-2,i2,
     & i3,kd)) )*d24(0)
      wtbmss4(i1,i2,i3,kd)=(-30.*wtbm(i1,i2,i3,kd)+16.*(wtbm(i1,i2+1,
     & i3,kd)+wtbm(i1,i2-1,i3,kd))-(wtbm(i1,i2+2,i3,kd)+wtbm(i1,i2-2,
     & i3,kd)) )*d24(1)
      wtbmtt4(i1,i2,i3,kd)=(-30.*wtbm(i1,i2,i3,kd)+16.*(wtbm(i1,i2,i3+
     & 1,kd)+wtbm(i1,i2,i3-1,kd))-(wtbm(i1,i2,i3+2,kd)+wtbm(i1,i2,i3-
     & 2,kd)) )*d24(2)
      wtbmrs4(i1,i2,i3,kd)=(8.*(wtbmr4(i1,i2+1,i3,kd)-wtbmr4(i1,i2-1,
     & i3,kd))-(wtbmr4(i1,i2+2,i3,kd)-wtbmr4(i1,i2-2,i3,kd)))*d14(1)
      wtbmrt4(i1,i2,i3,kd)=(8.*(wtbmr4(i1,i2,i3+1,kd)-wtbmr4(i1,i2,i3-
     & 1,kd))-(wtbmr4(i1,i2,i3+2,kd)-wtbmr4(i1,i2,i3-2,kd)))*d14(2)
      wtbmst4(i1,i2,i3,kd)=(8.*(wtbms4(i1,i2,i3+1,kd)-wtbms4(i1,i2,i3-
     & 1,kd))-(wtbms4(i1,i2,i3+2,kd)-wtbms4(i1,i2,i3-2,kd)))*d14(2)
      wtbmx41(i1,i2,i3,kd)= rx(i1,i2,i3)*wtbmr4(i1,i2,i3,kd)
      wtbmy41(i1,i2,i3,kd)=0
      wtbmz41(i1,i2,i3,kd)=0
      wtbmx42(i1,i2,i3,kd)= rx(i1,i2,i3)*wtbmr4(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*wtbms4(i1,i2,i3,kd)
      wtbmy42(i1,i2,i3,kd)= ry(i1,i2,i3)*wtbmr4(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*wtbms4(i1,i2,i3,kd)
      wtbmz42(i1,i2,i3,kd)=0
      wtbmx43(i1,i2,i3,kd)=rx(i1,i2,i3)*wtbmr4(i1,i2,i3,kd)+sx(i1,i2,
     & i3)*wtbms4(i1,i2,i3,kd)+tx(i1,i2,i3)*wtbmt4(i1,i2,i3,kd)
      wtbmy43(i1,i2,i3,kd)=ry(i1,i2,i3)*wtbmr4(i1,i2,i3,kd)+sy(i1,i2,
     & i3)*wtbms4(i1,i2,i3,kd)+ty(i1,i2,i3)*wtbmt4(i1,i2,i3,kd)
      wtbmz43(i1,i2,i3,kd)=rz(i1,i2,i3)*wtbmr4(i1,i2,i3,kd)+sz(i1,i2,
     & i3)*wtbms4(i1,i2,i3,kd)+tz(i1,i2,i3)*wtbmt4(i1,i2,i3,kd)
      wtbmxx41(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wtbmrr4(i1,i2,i3,kd)+(
     & rxx42(i1,i2,i3))*wtbmr4(i1,i2,i3,kd)
      wtbmyy41(i1,i2,i3,kd)=0
      wtbmxy41(i1,i2,i3,kd)=0
      wtbmxz41(i1,i2,i3,kd)=0
      wtbmyz41(i1,i2,i3,kd)=0
      wtbmzz41(i1,i2,i3,kd)=0
      wtbmlaplacian41(i1,i2,i3,kd)=wtbmxx41(i1,i2,i3,kd)
      wtbmxx42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2)*wtbmrr4(i1,i2,i3,kd)+2.*(
     & rx(i1,i2,i3)*sx(i1,i2,i3))*wtbmrs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**
     & 2)*wtbmss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3))*wtbmr4(i1,i2,i3,kd)+(
     & sxx42(i1,i2,i3))*wtbms4(i1,i2,i3,kd)
      wtbmyy42(i1,i2,i3,kd)=(ry(i1,i2,i3)**2)*wtbmrr4(i1,i2,i3,kd)+2.*(
     & ry(i1,i2,i3)*sy(i1,i2,i3))*wtbmrs4(i1,i2,i3,kd)+(sy(i1,i2,i3)**
     & 2)*wtbmss4(i1,i2,i3,kd)+(ryy42(i1,i2,i3))*wtbmr4(i1,i2,i3,kd)+(
     & syy42(i1,i2,i3))*wtbms4(i1,i2,i3,kd)
      wtbmxy42(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wtbmrr4(i1,i2,i3,
     & kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+ry(i1,i2,i3)*sx(i1,i2,i3))*
     & wtbmrs4(i1,i2,i3,kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wtbmss4(i1,i2,
     & i3,kd)+rxy42(i1,i2,i3)*wtbmr4(i1,i2,i3,kd)+sxy42(i1,i2,i3)*
     & wtbms4(i1,i2,i3,kd)
      wtbmxz42(i1,i2,i3,kd)=0
      wtbmyz42(i1,i2,i3,kd)=0
      wtbmzz42(i1,i2,i3,kd)=0
      wtbmlaplacian42(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*
     & wtbmrr4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,
     & i3)*sy(i1,i2,i3))*wtbmrs4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,
     & i2,i3)**2)*wtbmss4(i1,i2,i3,kd)+(rxx42(i1,i2,i3)+ryy42(i1,i2,
     & i3))*wtbmr4(i1,i2,i3,kd)+(sxx42(i1,i2,i3)+syy42(i1,i2,i3))*
     & wtbms4(i1,i2,i3,kd)
      wtbmxx43(i1,i2,i3,kd)=rx(i1,i2,i3)**2*wtbmrr4(i1,i2,i3,kd)+sx(i1,
     & i2,i3)**2*wtbmss4(i1,i2,i3,kd)+tx(i1,i2,i3)**2*wtbmtt4(i1,i2,
     & i3,kd)+2.*rx(i1,i2,i3)*sx(i1,i2,i3)*wtbmrs4(i1,i2,i3,kd)+2.*rx(
     & i1,i2,i3)*tx(i1,i2,i3)*wtbmrt4(i1,i2,i3,kd)+2.*sx(i1,i2,i3)*tx(
     & i1,i2,i3)*wtbmst4(i1,i2,i3,kd)+rxx43(i1,i2,i3)*wtbmr4(i1,i2,i3,
     & kd)+sxx43(i1,i2,i3)*wtbms4(i1,i2,i3,kd)+txx43(i1,i2,i3)*wtbmt4(
     & i1,i2,i3,kd)
      wtbmyy43(i1,i2,i3,kd)=ry(i1,i2,i3)**2*wtbmrr4(i1,i2,i3,kd)+sy(i1,
     & i2,i3)**2*wtbmss4(i1,i2,i3,kd)+ty(i1,i2,i3)**2*wtbmtt4(i1,i2,
     & i3,kd)+2.*ry(i1,i2,i3)*sy(i1,i2,i3)*wtbmrs4(i1,i2,i3,kd)+2.*ry(
     & i1,i2,i3)*ty(i1,i2,i3)*wtbmrt4(i1,i2,i3,kd)+2.*sy(i1,i2,i3)*ty(
     & i1,i2,i3)*wtbmst4(i1,i2,i3,kd)+ryy43(i1,i2,i3)*wtbmr4(i1,i2,i3,
     & kd)+syy43(i1,i2,i3)*wtbms4(i1,i2,i3,kd)+tyy43(i1,i2,i3)*wtbmt4(
     & i1,i2,i3,kd)
      wtbmzz43(i1,i2,i3,kd)=rz(i1,i2,i3)**2*wtbmrr4(i1,i2,i3,kd)+sz(i1,
     & i2,i3)**2*wtbmss4(i1,i2,i3,kd)+tz(i1,i2,i3)**2*wtbmtt4(i1,i2,
     & i3,kd)+2.*rz(i1,i2,i3)*sz(i1,i2,i3)*wtbmrs4(i1,i2,i3,kd)+2.*rz(
     & i1,i2,i3)*tz(i1,i2,i3)*wtbmrt4(i1,i2,i3,kd)+2.*sz(i1,i2,i3)*tz(
     & i1,i2,i3)*wtbmst4(i1,i2,i3,kd)+rzz43(i1,i2,i3)*wtbmr4(i1,i2,i3,
     & kd)+szz43(i1,i2,i3)*wtbms4(i1,i2,i3,kd)+tzz43(i1,i2,i3)*wtbmt4(
     & i1,i2,i3,kd)
      wtbmxy43(i1,i2,i3,kd)=rx(i1,i2,i3)*ry(i1,i2,i3)*wtbmrr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sy(i1,i2,i3)*wtbmss4(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *ty(i1,i2,i3)*wtbmtt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sy(i1,i2,i3)+
     & ry(i1,i2,i3)*sx(i1,i2,i3))*wtbmrs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & ty(i1,i2,i3)+ry(i1,i2,i3)*tx(i1,i2,i3))*wtbmrt4(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*ty(i1,i2,i3)+sy(i1,i2,i3)*tx(i1,i2,i3))*wtbmst4(
     & i1,i2,i3,kd)+rxy43(i1,i2,i3)*wtbmr4(i1,i2,i3,kd)+sxy43(i1,i2,
     & i3)*wtbms4(i1,i2,i3,kd)+txy43(i1,i2,i3)*wtbmt4(i1,i2,i3,kd)
      wtbmxz43(i1,i2,i3,kd)=rx(i1,i2,i3)*rz(i1,i2,i3)*wtbmrr4(i1,i2,i3,
     & kd)+sx(i1,i2,i3)*sz(i1,i2,i3)*wtbmss4(i1,i2,i3,kd)+tx(i1,i2,i3)
     & *tz(i1,i2,i3)*wtbmtt4(i1,i2,i3,kd)+(rx(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sx(i1,i2,i3))*wtbmrs4(i1,i2,i3,kd)+(rx(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*tx(i1,i2,i3))*wtbmrt4(i1,i2,i3,kd)+(
     & sx(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*tx(i1,i2,i3))*wtbmst4(
     & i1,i2,i3,kd)+rxz43(i1,i2,i3)*wtbmr4(i1,i2,i3,kd)+sxz43(i1,i2,
     & i3)*wtbms4(i1,i2,i3,kd)+txz43(i1,i2,i3)*wtbmt4(i1,i2,i3,kd)
      wtbmyz43(i1,i2,i3,kd)=ry(i1,i2,i3)*rz(i1,i2,i3)*wtbmrr4(i1,i2,i3,
     & kd)+sy(i1,i2,i3)*sz(i1,i2,i3)*wtbmss4(i1,i2,i3,kd)+ty(i1,i2,i3)
     & *tz(i1,i2,i3)*wtbmtt4(i1,i2,i3,kd)+(ry(i1,i2,i3)*sz(i1,i2,i3)+
     & rz(i1,i2,i3)*sy(i1,i2,i3))*wtbmrs4(i1,i2,i3,kd)+(ry(i1,i2,i3)*
     & tz(i1,i2,i3)+rz(i1,i2,i3)*ty(i1,i2,i3))*wtbmrt4(i1,i2,i3,kd)+(
     & sy(i1,i2,i3)*tz(i1,i2,i3)+sz(i1,i2,i3)*ty(i1,i2,i3))*wtbmst4(
     & i1,i2,i3,kd)+ryz43(i1,i2,i3)*wtbmr4(i1,i2,i3,kd)+syz43(i1,i2,
     & i3)*wtbms4(i1,i2,i3,kd)+tyz43(i1,i2,i3)*wtbmt4(i1,i2,i3,kd)
      wtbmlaplacian43(i1,i2,i3,kd)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(
     & i1,i2,i3)**2)*wtbmrr4(i1,i2,i3,kd)+(sx(i1,i2,i3)**2+sy(i1,i2,
     & i3)**2+sz(i1,i2,i3)**2)*wtbmss4(i1,i2,i3,kd)+(tx(i1,i2,i3)**2+
     & ty(i1,i2,i3)**2+tz(i1,i2,i3)**2)*wtbmtt4(i1,i2,i3,kd)+2.*(rx(
     & i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*
     & sz(i1,i2,i3))*wtbmrs4(i1,i2,i3,kd)+2.*(rx(i1,i2,i3)*tx(i1,i2,
     & i3)+ ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))*
     & wtbmrt4(i1,i2,i3,kd)+2.*(sx(i1,i2,i3)*tx(i1,i2,i3)+ sy(i1,i2,
     & i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3))*wtbmst4(i1,i2,i3,
     & kd)+(rxx43(i1,i2,i3)+ryy43(i1,i2,i3)+rzz43(i1,i2,i3))*wtbmr4(
     & i1,i2,i3,kd)+(sxx43(i1,i2,i3)+syy43(i1,i2,i3)+szz43(i1,i2,i3))*
     & wtbms4(i1,i2,i3,kd)+(txx43(i1,i2,i3)+tyy43(i1,i2,i3)+tzz43(i1,
     & i2,i3))*wtbmt4(i1,i2,i3,kd)
c============================================================================================
c Define derivatives for a rectangular grid
c
c============================================================================================
      wtbmx43r(i1,i2,i3,kd)=(8.*(wtbm(i1+1,i2,i3,kd)-wtbm(i1-1,i2,i3,
     & kd))-(wtbm(i1+2,i2,i3,kd)-wtbm(i1-2,i2,i3,kd)))*h41(0)
      wtbmy43r(i1,i2,i3,kd)=(8.*(wtbm(i1,i2+1,i3,kd)-wtbm(i1,i2-1,i3,
     & kd))-(wtbm(i1,i2+2,i3,kd)-wtbm(i1,i2-2,i3,kd)))*h41(1)
      wtbmz43r(i1,i2,i3,kd)=(8.*(wtbm(i1,i2,i3+1,kd)-wtbm(i1,i2,i3-1,
     & kd))-(wtbm(i1,i2,i3+2,kd)-wtbm(i1,i2,i3-2,kd)))*h41(2)
      wtbmxx43r(i1,i2,i3,kd)=( -30.*wtbm(i1,i2,i3,kd)+16.*(wtbm(i1+1,
     & i2,i3,kd)+wtbm(i1-1,i2,i3,kd))-(wtbm(i1+2,i2,i3,kd)+wtbm(i1-2,
     & i2,i3,kd)) )*h42(0)
      wtbmyy43r(i1,i2,i3,kd)=( -30.*wtbm(i1,i2,i3,kd)+16.*(wtbm(i1,i2+
     & 1,i3,kd)+wtbm(i1,i2-1,i3,kd))-(wtbm(i1,i2+2,i3,kd)+wtbm(i1,i2-
     & 2,i3,kd)) )*h42(1)
      wtbmzz43r(i1,i2,i3,kd)=( -30.*wtbm(i1,i2,i3,kd)+16.*(wtbm(i1,i2,
     & i3+1,kd)+wtbm(i1,i2,i3-1,kd))-(wtbm(i1,i2,i3+2,kd)+wtbm(i1,i2,
     & i3-2,kd)) )*h42(2)
      wtbmxy43r(i1,i2,i3,kd)=( (wtbm(i1+2,i2+2,i3,kd)-wtbm(i1-2,i2+2,
     & i3,kd)- wtbm(i1+2,i2-2,i3,kd)+wtbm(i1-2,i2-2,i3,kd)) +8.*(wtbm(
     & i1-1,i2+2,i3,kd)-wtbm(i1-1,i2-2,i3,kd)-wtbm(i1+1,i2+2,i3,kd)+
     & wtbm(i1+1,i2-2,i3,kd) +wtbm(i1+2,i2-1,i3,kd)-wtbm(i1-2,i2-1,i3,
     & kd)-wtbm(i1+2,i2+1,i3,kd)+wtbm(i1-2,i2+1,i3,kd))+64.*(wtbm(i1+
     & 1,i2+1,i3,kd)-wtbm(i1-1,i2+1,i3,kd)- wtbm(i1+1,i2-1,i3,kd)+
     & wtbm(i1-1,i2-1,i3,kd)))*(h41(0)*h41(1))
      wtbmxz43r(i1,i2,i3,kd)=( (wtbm(i1+2,i2,i3+2,kd)-wtbm(i1-2,i2,i3+
     & 2,kd)-wtbm(i1+2,i2,i3-2,kd)+wtbm(i1-2,i2,i3-2,kd)) +8.*(wtbm(
     & i1-1,i2,i3+2,kd)-wtbm(i1-1,i2,i3-2,kd)-wtbm(i1+1,i2,i3+2,kd)+
     & wtbm(i1+1,i2,i3-2,kd) +wtbm(i1+2,i2,i3-1,kd)-wtbm(i1-2,i2,i3-1,
     & kd)- wtbm(i1+2,i2,i3+1,kd)+wtbm(i1-2,i2,i3+1,kd)) +64.*(wtbm(
     & i1+1,i2,i3+1,kd)-wtbm(i1-1,i2,i3+1,kd)-wtbm(i1+1,i2,i3-1,kd)+
     & wtbm(i1-1,i2,i3-1,kd)) )*(h41(0)*h41(2))
      wtbmyz43r(i1,i2,i3,kd)=( (wtbm(i1,i2+2,i3+2,kd)-wtbm(i1,i2-2,i3+
     & 2,kd)-wtbm(i1,i2+2,i3-2,kd)+wtbm(i1,i2-2,i3-2,kd)) +8.*(wtbm(
     & i1,i2-1,i3+2,kd)-wtbm(i1,i2-1,i3-2,kd)-wtbm(i1,i2+1,i3+2,kd)+
     & wtbm(i1,i2+1,i3-2,kd) +wtbm(i1,i2+2,i3-1,kd)-wtbm(i1,i2-2,i3-1,
     & kd)-wtbm(i1,i2+2,i3+1,kd)+wtbm(i1,i2-2,i3+1,kd)) +64.*(wtbm(i1,
     & i2+1,i3+1,kd)-wtbm(i1,i2-1,i3+1,kd)-wtbm(i1,i2+1,i3-1,kd)+wtbm(
     & i1,i2-1,i3-1,kd)) )*(h41(1)*h41(2))
      wtbmx41r(i1,i2,i3,kd)= wtbmx43r(i1,i2,i3,kd)
      wtbmy41r(i1,i2,i3,kd)= wtbmy43r(i1,i2,i3,kd)
      wtbmz41r(i1,i2,i3,kd)= wtbmz43r(i1,i2,i3,kd)
      wtbmxx41r(i1,i2,i3,kd)= wtbmxx43r(i1,i2,i3,kd)
      wtbmyy41r(i1,i2,i3,kd)= wtbmyy43r(i1,i2,i3,kd)
      wtbmzz41r(i1,i2,i3,kd)= wtbmzz43r(i1,i2,i3,kd)
      wtbmxy41r(i1,i2,i3,kd)= wtbmxy43r(i1,i2,i3,kd)
      wtbmxz41r(i1,i2,i3,kd)= wtbmxz43r(i1,i2,i3,kd)
      wtbmyz41r(i1,i2,i3,kd)= wtbmyz43r(i1,i2,i3,kd)
      wtbmlaplacian41r(i1,i2,i3,kd)=wtbmxx43r(i1,i2,i3,kd)
      wtbmx42r(i1,i2,i3,kd)= wtbmx43r(i1,i2,i3,kd)
      wtbmy42r(i1,i2,i3,kd)= wtbmy43r(i1,i2,i3,kd)
      wtbmz42r(i1,i2,i3,kd)= wtbmz43r(i1,i2,i3,kd)
      wtbmxx42r(i1,i2,i3,kd)= wtbmxx43r(i1,i2,i3,kd)
      wtbmyy42r(i1,i2,i3,kd)= wtbmyy43r(i1,i2,i3,kd)
      wtbmzz42r(i1,i2,i3,kd)= wtbmzz43r(i1,i2,i3,kd)
      wtbmxy42r(i1,i2,i3,kd)= wtbmxy43r(i1,i2,i3,kd)
      wtbmxz42r(i1,i2,i3,kd)= wtbmxz43r(i1,i2,i3,kd)
      wtbmyz42r(i1,i2,i3,kd)= wtbmyz43r(i1,i2,i3,kd)
      wtbmlaplacian42r(i1,i2,i3,kd)=wtbmxx43r(i1,i2,i3,kd)+wtbmyy43r(
     & i1,i2,i3,kd)
      wtbmlaplacian43r(i1,i2,i3,kd)=wtbmxx43r(i1,i2,i3,kd)+wtbmyy43r(
     & i1,i2,i3,kd)+wtbmzz43r(i1,i2,i3,kd)


!............... end statement functions

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
        write(*,'(" pmlMaxwell: **START** grid=",i4," side,axis=",2i2,
     & ", c,dt=",2f8.5," layerStrength,power=",f6.2,i2)') grid,side,
     & axis,c,dt,layerStrength,power
        write(*,'(" pmlMaxwell: nd,orderOfAccuracy,gridType=",i2,i2,i2)
     & ') nd,orderOfAccuracy,gridType
        write(*,'(" pmlMaxwell: dx=",3e10.2)') dx(0),dx(1),dx(2)
        ! ' 
      end if

      csq=c*c
      cdtsq=(c*dt)**2
      cdt4Over12=(c*dt)**4/12.

      ! ***** first fill in the parameters for the boxes we need to assign ****
      numberOfGhostPoints=orderOfAccuracy/2

      ! We apply the PML equations out to these bounds: (make use of all ghost points - stencilWidth/2)
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
        ! **** TESTING apply PML equations in the interior ******

        write(*,'(" ****pml: assign interior pts=[",i3,",",i3,"][",i3,
     & ",",i3,"][",i3,",",i3,"]")') m1a,m1b,m2a,m2b,m3a,m3b
        ! ' 
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
           un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + cdtsq*( 
     & uLaplacian22r(i1,i2,i3,ex) )
         end do
         end do
         end do
        else
         do i3=m3a,m3b
         do i2=m2a,m2b
         do i1=m1a,m1b
           un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + cdtsq*( 
     & uLaplacian23r(i1,i2,i3,ex) )
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
           un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + cdtsq*( 
     & uLaplacian42r(i1,i2,i3,ex) ) + cdt4Over12*( uLapSq22r(i1,i2,i3,
     & ex) )
         end do
         end do
         end do
        else
         do i3=m3a,m3b
         do i2=m2a,m2b
         do i1=m1a,m1b
           un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + cdtsq*( 
     & uLaplacian43r(i1,i2,i3,ex) ) + cdt4Over12*( uLapSq23r(i1,i2,i3,
     & ex) )
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
      if( boundaryCondition(0,0).eq.abcPML )then
          box(0,nb)=md1a
          box(1,nb)=m1a
          box(2,nb)=n2a
          box(3,nb)=n2b
          box(4,nb)=n3a
          box(5,nb)=n3b
          box(6,nb)=xSide
          box(7,nb)=0
          box(8,nb)=0
          box(9,nb)=0
          nb=nb+1
      end if
      ! right face
      if( boundaryCondition(1,0).eq.abcPML )then
          box(0,nb)=m1b
          box(1,nb)=md1b
          box(2,nb)=n2a
          box(3,nb)=n2b
          box(4,nb)=n3a
          box(5,nb)=n3b
          box(6,nb)=xSide
          box(7,nb)=1
          box(8,nb)=0
          box(9,nb)=0
          nb=nb+1
      end if
      ! bottom and top
      if( boundaryCondition(0,1).eq.abcPML )then
          box(0,nb)=n1a
          box(1,nb)=n1b
          box(2,nb)=md2a
          box(3,nb)=m2a
          box(4,nb)=n3a
          box(5,nb)=n3b
          box(6,nb)=ySide
          box(7,nb)=0
          box(8,nb)=0
          box(9,nb)=0
          nb=nb+1
      end if
      if( boundaryCondition(1,1).eq.abcPML )then
          box(0,nb)=n1a
          box(1,nb)=n1b
          box(2,nb)=m2b
          box(3,nb)=md2b
          box(4,nb)=n3a
          box(5,nb)=n3b
          box(6,nb)=ySide
          box(7,nb)=0
          box(8,nb)=1
          box(9,nb)=0
          nb=nb+1
      end if
      ! edges (corners in 2d)
      if( boundaryCondition(0,0).eq.abcPML .and. boundaryCondition(0,1)
     & .eq.abcPML )then
          box(0,nb)=md1a
          box(1,nb)=m1a
          box(2,nb)=md2a
          box(3,nb)=m2a
          box(4,nb)=n3a
          box(5,nb)=n3b
          box(6,nb)=xyEdge
          box(7,nb)=0
          box(8,nb)=0
          box(9,nb)=0
          nb=nb+1
      end if
      if( boundaryCondition(0,0).eq.abcPML .and. boundaryCondition(1,1)
     & .eq.abcPML )then
          box(0,nb)=md1a
          box(1,nb)=m1a
          box(2,nb)=m2b
          box(3,nb)=md2b
          box(4,nb)=n3a
          box(5,nb)=n3b
          box(6,nb)=xyEdge
          box(7,nb)=0
          box(8,nb)=1
          box(9,nb)=0
          nb=nb+1
      end if
      if( boundaryCondition(1,0).eq.abcPML .and. boundaryCondition(0,1)
     & .eq.abcPML )then
          box(0,nb)=m1b
          box(1,nb)=md1b
          box(2,nb)=md2a
          box(3,nb)=m2a
          box(4,nb)=n3a
          box(5,nb)=n3b
          box(6,nb)=xyEdge
          box(7,nb)=1
          box(8,nb)=0
          box(9,nb)=0
          nb=nb+1
      end if
      if( boundaryCondition(1,0).eq.abcPML .and. boundaryCondition(1,1)
     & .eq.abcPML )then
          box(0,nb)=m1b
          box(1,nb)=md1b
          box(2,nb)=m2b
          box(3,nb)=md2b
          box(4,nb)=n3a
          box(5,nb)=n3b
          box(6,nb)=xyEdge
          box(7,nb)=1
          box(8,nb)=1
          box(9,nb)=0
          nb=nb+1
      end if

      if( nd.eq.3 )then
        ! front and back
        if( boundaryCondition(0,2).eq.abcPML )then
            box(0,nb)=n1a
            box(1,nb)=n1b
            box(2,nb)=n2a
            box(3,nb)=n2b
            box(4,nb)=md3a
            box(5,nb)=m3a
            box(6,nb)=zSide
            box(7,nb)=0
            box(8,nb)=0
            box(9,nb)=0
            nb=nb+1
        end if
        if( boundaryCondition(1,2).eq.abcPML )then
            box(0,nb)=n1a
            box(1,nb)=n1b
            box(2,nb)=n2a
            box(3,nb)=n2b
            box(4,nb)=m3b
            box(5,nb)=md3b
            box(6,nb)=zSide
            box(7,nb)=0
            box(8,nb)=0
            box(9,nb)=1
            nb=nb+1
        end if
        ! more edges

        if( boundaryCondition(0,0).eq.abcPML .and. boundaryCondition(0,
     & 2).eq.abcPML )then
            box(0,nb)=md1a
            box(1,nb)=m1a
            box(2,nb)=n2a
            box(3,nb)=n2b
            box(4,nb)=md3a
            box(5,nb)=m3a
            box(6,nb)=xzEdge
            box(7,nb)=0
            box(8,nb)=0
            box(9,nb)=0
            nb=nb+1
        end if
        if( boundaryCondition(0,0).eq.abcPML .and. boundaryCondition(1,
     & 2).eq.abcPML )then
            box(0,nb)=md1a
            box(1,nb)=m1a
            box(2,nb)=n2a
            box(3,nb)=n2b
            box(4,nb)=m3b
            box(5,nb)=md3b
            box(6,nb)=xzEdge
            box(7,nb)=0
            box(8,nb)=0
            box(9,nb)=1
            nb=nb+1
        end if
        if( boundaryCondition(1,0).eq.abcPML .and. boundaryCondition(0,
     & 2).eq.abcPML )then
            box(0,nb)=m1b
            box(1,nb)=md1b
            box(2,nb)=n2a
            box(3,nb)=n2b
            box(4,nb)=md3a
            box(5,nb)=m3a
            box(6,nb)=xzEdge
            box(7,nb)=1
            box(8,nb)=0
            box(9,nb)=0
            nb=nb+1
        end if
        if( boundaryCondition(1,0).eq.abcPML .and. boundaryCondition(1,
     & 2).eq.abcPML )then
            box(0,nb)=m1b
            box(1,nb)=md1b
            box(2,nb)=n2a
            box(3,nb)=n2b
            box(4,nb)=m3b
            box(5,nb)=md3b
            box(6,nb)=xzEdge
            box(7,nb)=1
            box(8,nb)=0
            box(9,nb)=1
            nb=nb+1
        end if

        if( boundaryCondition(0,1).eq.abcPML .and. boundaryCondition(0,
     & 2).eq.abcPML )then
            box(0,nb)=n1a
            box(1,nb)=n1b
            box(2,nb)=md2a
            box(3,nb)=m2a
            box(4,nb)=md3a
            box(5,nb)=m3a
            box(6,nb)=yzEdge
            box(7,nb)=0
            box(8,nb)=0
            box(9,nb)=0
            nb=nb+1
        end if
        if( boundaryCondition(0,1).eq.abcPML .and. boundaryCondition(1,
     & 2).eq.abcPML )then
            box(0,nb)=n1a
            box(1,nb)=n1b
            box(2,nb)=md2a
            box(3,nb)=m2a
            box(4,nb)=m3b
            box(5,nb)=md3b
            box(6,nb)=yzEdge
            box(7,nb)=0
            box(8,nb)=0
            box(9,nb)=1
            nb=nb+1
        end if
        if( boundaryCondition(1,1).eq.abcPML .and. boundaryCondition(0,
     & 2).eq.abcPML )then
            box(0,nb)=n1a
            box(1,nb)=n1b
            box(2,nb)=m2b
            box(3,nb)=md2b
            box(4,nb)=md3a
            box(5,nb)=m3a
            box(6,nb)=yzEdge
            box(7,nb)=0
            box(8,nb)=1
            box(9,nb)=0
            nb=nb+1
        end if
        if( boundaryCondition(1,1).eq.abcPML .and. boundaryCondition(1,
     & 2).eq.abcPML )then
            box(0,nb)=n1a
            box(1,nb)=n1b
            box(2,nb)=m2b
            box(3,nb)=md2b
            box(4,nb)=m3b
            box(5,nb)=md3b
            box(6,nb)=yzEdge
            box(7,nb)=0
            box(8,nb)=1
            box(9,nb)=1
            nb=nb+1
        end if

        ! corners
        if( boundaryCondition(0,0).eq.abcPML .and. boundaryCondition(0,
     & 1).eq.abcPML .and. boundaryCondition(0,2).eq.abcPML)then
            box(0,nb)=md1a
            box(1,nb)=m1a
            box(2,nb)=md2a
            box(3,nb)=m2a
            box(4,nb)=md3a
            box(5,nb)=m3a
            box(6,nb)=xyzCorner
            box(7,nb)=0
            box(8,nb)=0
            box(9,nb)=0
            nb=nb+1
        end if
        if( boundaryCondition(1,0).eq.abcPML .and. boundaryCondition(0,
     & 1).eq.abcPML .and. boundaryCondition(0,2).eq.abcPML)then
            box(0,nb)=m1b
            box(1,nb)=md1b
            box(2,nb)=md2a
            box(3,nb)=m2a
            box(4,nb)=md3a
            box(5,nb)=m3a
            box(6,nb)=xyzCorner
            box(7,nb)=1
            box(8,nb)=0
            box(9,nb)=0
            nb=nb+1
        end if
        if( boundaryCondition(0,0).eq.abcPML .and. boundaryCondition(1,
     & 1).eq.abcPML .and. boundaryCondition(0,2).eq.abcPML)then
            box(0,nb)=md1a
            box(1,nb)=m1a
            box(2,nb)=m2b
            box(3,nb)=md2b
            box(4,nb)=md3a
            box(5,nb)=m3a
            box(6,nb)=xyzCorner
            box(7,nb)=0
            box(8,nb)=1
            box(9,nb)=0
            nb=nb+1
        end if
        if( boundaryCondition(1,0).eq.abcPML .and. boundaryCondition(1,
     & 1).eq.abcPML .and. boundaryCondition(0,2).eq.abcPML)then
            box(0,nb)=m1b
            box(1,nb)=md1b
            box(2,nb)=m2b
            box(3,nb)=md2b
            box(4,nb)=md3a
            box(5,nb)=m3a
            box(6,nb)=xyzCorner
            box(7,nb)=1
            box(8,nb)=1
            box(9,nb)=0
            nb=nb+1
        end if

        if( boundaryCondition(0,0).eq.abcPML .and. boundaryCondition(0,
     & 1).eq.abcPML .and. boundaryCondition(1,2).eq.abcPML)then
            box(0,nb)=md1a
            box(1,nb)=m1a
            box(2,nb)=md2a
            box(3,nb)=m2a
            box(4,nb)=m3b
            box(5,nb)=md3b
            box(6,nb)=xyzCorner
            box(7,nb)=0
            box(8,nb)=0
            box(9,nb)=1
            nb=nb+1
        end if
        if( boundaryCondition(1,0).eq.abcPML .and. boundaryCondition(0,
     & 1).eq.abcPML .and. boundaryCondition(1,2).eq.abcPML)then
            box(0,nb)=m1b
            box(1,nb)=md1b
            box(2,nb)=md2a
            box(3,nb)=m2a
            box(4,nb)=m3b
            box(5,nb)=md3b
            box(6,nb)=xyzCorner
            box(7,nb)=1
            box(8,nb)=0
            box(9,nb)=1
            nb=nb+1
        end if
        if( boundaryCondition(0,0).eq.abcPML .and. boundaryCondition(1,
     & 1).eq.abcPML .and. boundaryCondition(1,2).eq.abcPML)then
            box(0,nb)=md1a
            box(1,nb)=m1a
            box(2,nb)=m2b
            box(3,nb)=md2b
            box(4,nb)=m3b
            box(5,nb)=md3b
            box(6,nb)=xyzCorner
            box(7,nb)=0
            box(8,nb)=1
            box(9,nb)=1
            nb=nb+1
        end if
        if( boundaryCondition(1,0).eq.abcPML .and. boundaryCondition(1,
     & 1).eq.abcPML .and. boundaryCondition(1,2).eq.abcPML)then
            box(0,nb)=m1b
            box(1,nb)=md1b
            box(2,nb)=m2b
            box(3,nb)=md2b
            box(4,nb)=m3b
            box(5,nb)=md3b
            box(6,nb)=xyzCorner
            box(7,nb)=1
            box(8,nb)=1
            box(9,nb)=1
            nb=nb+1
        end if


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

       if( .false. )then
         ! write debuginfo:
         write(*,'("     pml: box(",i2,"=[",i3,",",i3,"][",i3,",",i3,
     & "][",i3,",",i3,"], boxType=",i2,", side=",3i2," ex=",i2)') nb,
     & m1a,m1b,m2a,m2b,m3a,m3b,boxType,side1,side2,side3,ex

         write(*,'(" Loop bounds [",i3,":",i3,",",i3,":",i3,",",i3,":",
     & i3,"]=[m1a:m1b,m2a:m2b,m3a:m3b]")') m1a,m1b,m2a,m2b,m3a,m3b
         if( boxType.eq.xSide )then
           write(*,'(" bounds: vra=[",i3,":",i3,",",i3,":",i3,",",i3,
     & ":",i3,"]")') ndra1a,ndra1b,ndra2a,ndra2b,ndra3a,ndra3b
           write(*,'(" bounds: vba=[",i3,":",i3,",",i3,":",i3,",",i3,
     & ":",i3,"]")') ndrb1a,ndrb1b,ndrb2a,ndrb2b,ndrb3a,ndrb3b
           write(*,'(" i1a,i1b=",i3,",",i3," (sigma(i1a)=0)")') i1a,i1b
         else if( boxType.eq.ySide )then
           write(*,'(" bounds: vsa=[",i3,":",i3,",",i3,":",i3,",",i3,
     & ":",i3,"]")') ndsa1a,ndsa1b,ndsa2a,ndsa2b,ndsa3a,ndsa3b
           write(*,'(" bounds: vsb=[",i3,":",i3,",",i3,":",i3,",",i3,
     & ":",i3,"]")') ndsb1a,ndsb1b,ndsb2a,ndsb2b,ndsb3a,ndsb3b
           write(*,'(" i2a,i2b=",i3,",",i3," (sigma(i2a)=0)")') i2a,i2b
         else if( boxType.eq.zSide )then
           write(*,'(" bounds: vta=[",i3,":",i3,",",i3,":",i3,",",i3,
     & ":",i3,"]")') ndta1a,ndta1b,ndta2a,ndta2b,ndta3a,ndta3b
           write(*,'(" bounds: vtb=[",i3,":",i3,",",i3,":",i3,",",i3,
     & ":",i3,"]")') ndtb1a,ndtb1b,ndtb2a,ndtb2b,ndtb3a,ndtb3b
           write(*,'(" i3a,i3b=",i3,",",i3," (sigma(i2a)=0)")') i3a,i3b
         else
         end if
       end if


       if( gridType.eq.rectangular .and. orderOfAccuracy.eq.2 )then
        ! *******************************************************
        ! ************Rectangular grid Order=2*******************
        ! *******************************************************

        if( nd.eq.2 )then
         ! *********************************************************************
         ! **************** Two Dimensions, Order 2 ****************************
         ! *********************************************************************


         ! write(*,'("     pml: assign 2d rectangular grid orderOfAccuracy==2")') 
         if( boxType.eq.xSide )then

          if( side1.eq.0 )then
            do i3=m3a,m3b
            do i2=m2a,m2b
            do i1=m1a,m1b
              ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
              ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
              xx=(i1-i1a)/real(i1b-i1a)
              sigma = layerStrength*xx**power
              un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( ulaplacian22r(i1,i2,i3,ex) - vra x22r(i1,i2,i3,ex) -
     & wra(i1,i2,i3,ex) )
             ! write(*,'(" i=",3i3," um,u,un,uLap,cdtsq=",5e10.2)') i1,i2,i3,um(i1,i2,i3,ex),u(i1,i2,i3,ex),un(i1,i2,i3,ex),!        ulaplacian22r(i1,i2,i3,ex), cdtsq
              ! auxilliary variables       
              vra n(i1,i2,i3,ex)=vra(i1,i2,i3,ex)+sigma*dt*( 1.5*( -
     & vra(i1,i2,i3,ex) +  u x22r(i1,i2,i3,ex))-0.5*(-vra m(i1,i2,i3,
     & ex) + um x22r(i1,i2,i3,ex)) )
              wra n(i1,i2,i3,ex)=wra(i1,i2,i3,ex)+sigma*dt*( 1.5*( -
     & wra(i1,i2,i3,ex)+  uxx22r(i1,i2,i3,ex)-vra   x22r(i1,i2,i3,ex))
     & -0.5*(-wra m(i1,i2,i3,ex)+ umxx22r(i1,i2,i3,ex)-vra m x22r(i1,
     & i2,i3,ex)) )
            end do
            end do
            end do
          else
            do i3=m3a,m3b
            do i2=m2a,m2b
            do i1=m1a,m1b
              ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
              ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
              xx=(i1-i1a)/real(i1b-i1a)
              sigma = layerStrength*xx**power
              un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( ulaplacian22r(i1,i2,i3,ex) - vrb x22r(i1,i2,i3,ex) -
     & wrb(i1,i2,i3,ex) )
             ! write(*,'(" i=",3i3," um,u,un,uLap,cdtsq=",5e10.2)') i1,i2,i3,um(i1,i2,i3,ex),u(i1,i2,i3,ex),un(i1,i2,i3,ex),!        ulaplacian22r(i1,i2,i3,ex), cdtsq
              ! auxilliary variables       
              vrb n(i1,i2,i3,ex)=vrb(i1,i2,i3,ex)+sigma*dt*( 1.5*( -
     & vrb(i1,i2,i3,ex) +  u x22r(i1,i2,i3,ex))-0.5*(-vrb m(i1,i2,i3,
     & ex) + um x22r(i1,i2,i3,ex)) )
              wrb n(i1,i2,i3,ex)=wrb(i1,i2,i3,ex)+sigma*dt*( 1.5*( -
     & wrb(i1,i2,i3,ex)+  uxx22r(i1,i2,i3,ex)-vrb   x22r(i1,i2,i3,ex))
     & -0.5*(-wrb m(i1,i2,i3,ex)+ umxx22r(i1,i2,i3,ex)-vrb m x22r(i1,
     & i2,i3,ex)) )
            end do
            end do
            end do
          end if

         else if( boxType.eq.ySide )then

          if( side2.eq.0 )then
           do i3=m3a,m3b
           do i2=m2a,m2b
           do i1=m1a,m1b
             ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
             ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
             yy=(i2-i2a)/real(i2b-i2a)
             sigma = layerStrength*yy**power
             un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + cdtsq*
     & ( ulaplacian22r(i1,i2,i3,ex) - vsa y22r(i1,i2,i3,ex) -wsa(i1,
     & i2,i3,ex) )
            ! write(*,'(" i=",3i3," um,u,un,uLap,cdtsq=",5e10.2)') i1,i2,i3,um(i1,i2,i3,ex),u(i1,i2,i3,ex),un(i1,i2,i3,ex),!        ulaplacian22r(i1,i2,i3,ex), cdtsq
             ! auxilliary variables       
             vsa n(i1,i2,i3,ex)=vsa(i1,i2,i3,ex)+sigma*dt*( 1.5*( -vsa(
     & i1,i2,i3,ex) +  u y22r(i1,i2,i3,ex))-0.5*(-vsa m(i1,i2,i3,ex) +
     &  um y22r(i1,i2,i3,ex)) )
             wsa n(i1,i2,i3,ex)=wsa(i1,i2,i3,ex)+sigma*dt*( 1.5*( -wsa(
     & i1,i2,i3,ex)+  uyy22r(i1,i2,i3,ex)-vsa   y22r(i1,i2,i3,ex))-
     & 0.5*(-wsa m(i1,i2,i3,ex)+ umyy22r(i1,i2,i3,ex)-vsa m y22r(i1,
     & i2,i3,ex)) )
           end do
           end do
           end do
          else
           do i3=m3a,m3b
           do i2=m2a,m2b
           do i1=m1a,m1b
             ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
             ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
             yy=(i2-i2a)/real(i2b-i2a)
             sigma = layerStrength*yy**power
             un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + cdtsq*
     & ( ulaplacian22r(i1,i2,i3,ex) - vsb y22r(i1,i2,i3,ex) -wsb(i1,
     & i2,i3,ex) )
            ! write(*,'(" i=",3i3," um,u,un,uLap,cdtsq=",5e10.2)') i1,i2,i3,um(i1,i2,i3,ex),u(i1,i2,i3,ex),un(i1,i2,i3,ex),!        ulaplacian22r(i1,i2,i3,ex), cdtsq
             ! auxilliary variables       
             vsb n(i1,i2,i3,ex)=vsb(i1,i2,i3,ex)+sigma*dt*( 1.5*( -vsb(
     & i1,i2,i3,ex) +  u y22r(i1,i2,i3,ex))-0.5*(-vsb m(i1,i2,i3,ex) +
     &  um y22r(i1,i2,i3,ex)) )
             wsb n(i1,i2,i3,ex)=wsb(i1,i2,i3,ex)+sigma*dt*( 1.5*( -wsb(
     & i1,i2,i3,ex)+  uyy22r(i1,i2,i3,ex)-vsb   y22r(i1,i2,i3,ex))-
     & 0.5*(-wsb m(i1,i2,i3,ex)+ umyy22r(i1,i2,i3,ex)-vsb m y22r(i1,
     & i2,i3,ex)) )
           end do
           end do
           end do
          end if

         else

           ! macro to advance corner regions:
             if( boxType.eq.xyEdge )then
              if( side1.eq.0 .and. side2.eq.0 )then
               do i3=m3a,m3b
               do i2=m2a,m2b
               do i1=m1a,m1b
                 ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
                 ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
                 xx=(i1-i1a)/real(i1b-i1a)
                 sigma1 = layerStrength*xx**power
                 ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
                 ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
                 yy=(i2-i2a)/real(i2b-i2a)
                 sigma2 = layerStrength*yy**power
                 un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( ulaplacian22r(i1,i2,i3,ex) - vra x22r(i1,i2,i3,ex) -
     & wra(i1,i2,i3,ex) - vsa y22r(i1,i2,i3,ex) -wsa(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vra n(i1,i2,i3,ex)=vra(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -vra(i1,i2,i3,ex) +  ux22r(i1,i2,i3,ex))-0.5*(-vra m(i1,i2,i3,
     & ex) + umx22r(i1,i2,i3,ex)) )
                 wra n(i1,i2,i3,ex)=wra(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -wra(i1,i2,i3,ex)+  uxx22r(i1,i2,i3,ex) - vra x22r(i1,i2,i3,
     & ex))-0.5*(-wra m(i1,i2,i3,ex)+ umxx22r(i1,i2,i3,ex)-vra m x22r(
     & i1,i2,i3,ex)) )
                 vsa n(i1,i2,i3,ex)=vsa(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     &  -vsa(i1,i2,i3,ex) +  uy22r(i1,i2,i3,ex))-0.5*(-vsa m(i1,i2,i3,
     & ex) + umy22r(i1,i2,i3,ex)) )
                 wsa n(i1,i2,i3,ex)=wsa(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     & -wsa(i1,i2,i3,ex)+  uyy22r(i1,i2,i3,ex)   -vsa y22r(i1,i2,i3,
     & ex))-0.5*(-wsa m(i1,i2,i3,ex)+ umyy22r(i1,i2,i3,ex)-vsa m y22r(
     & i1,i2,i3,ex)) )
               end do
               end do
               end do
              else if( side1.eq.1 .and. side2.eq.0 )then
               do i3=m3a,m3b
               do i2=m2a,m2b
               do i1=m1a,m1b
                 ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
                 ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
                 xx=(i1-i1a)/real(i1b-i1a)
                 sigma1 = layerStrength*xx**power
                 ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
                 ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
                 yy=(i2-i2a)/real(i2b-i2a)
                 sigma2 = layerStrength*yy**power
                 un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( ulaplacian22r(i1,i2,i3,ex) - vrb x22r(i1,i2,i3,ex) -
     & wrb(i1,i2,i3,ex) - vsa y22r(i1,i2,i3,ex) -wsa(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vrb n(i1,i2,i3,ex)=vrb(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -vrb(i1,i2,i3,ex) +  ux22r(i1,i2,i3,ex))-0.5*(-vrb m(i1,i2,i3,
     & ex) + umx22r(i1,i2,i3,ex)) )
                 wrb n(i1,i2,i3,ex)=wrb(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -wrb(i1,i2,i3,ex)+  uxx22r(i1,i2,i3,ex) - vrb x22r(i1,i2,i3,
     & ex))-0.5*(-wrb m(i1,i2,i3,ex)+ umxx22r(i1,i2,i3,ex)-vrb m x22r(
     & i1,i2,i3,ex)) )
                 vsa n(i1,i2,i3,ex)=vsa(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     &  -vsa(i1,i2,i3,ex) +  uy22r(i1,i2,i3,ex))-0.5*(-vsa m(i1,i2,i3,
     & ex) + umy22r(i1,i2,i3,ex)) )
                 wsa n(i1,i2,i3,ex)=wsa(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     & -wsa(i1,i2,i3,ex)+  uyy22r(i1,i2,i3,ex)   -vsa y22r(i1,i2,i3,
     & ex))-0.5*(-wsa m(i1,i2,i3,ex)+ umyy22r(i1,i2,i3,ex)-vsa m y22r(
     & i1,i2,i3,ex)) )
               end do
               end do
               end do
              else if( side1.eq.0 .and. side2.eq.1 )then
               do i3=m3a,m3b
               do i2=m2a,m2b
               do i1=m1a,m1b
                 ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
                 ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
                 xx=(i1-i1a)/real(i1b-i1a)
                 sigma1 = layerStrength*xx**power
                 ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
                 ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
                 yy=(i2-i2a)/real(i2b-i2a)
                 sigma2 = layerStrength*yy**power
                 un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( ulaplacian22r(i1,i2,i3,ex) - vra x22r(i1,i2,i3,ex) -
     & wra(i1,i2,i3,ex) - vsb y22r(i1,i2,i3,ex) -wsb(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vra n(i1,i2,i3,ex)=vra(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -vra(i1,i2,i3,ex) +  ux22r(i1,i2,i3,ex))-0.5*(-vra m(i1,i2,i3,
     & ex) + umx22r(i1,i2,i3,ex)) )
                 wra n(i1,i2,i3,ex)=wra(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -wra(i1,i2,i3,ex)+  uxx22r(i1,i2,i3,ex) - vra x22r(i1,i2,i3,
     & ex))-0.5*(-wra m(i1,i2,i3,ex)+ umxx22r(i1,i2,i3,ex)-vra m x22r(
     & i1,i2,i3,ex)) )
                 vsb n(i1,i2,i3,ex)=vsb(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     &  -vsb(i1,i2,i3,ex) +  uy22r(i1,i2,i3,ex))-0.5*(-vsb m(i1,i2,i3,
     & ex) + umy22r(i1,i2,i3,ex)) )
                 wsb n(i1,i2,i3,ex)=wsb(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     & -wsb(i1,i2,i3,ex)+  uyy22r(i1,i2,i3,ex)   -vsb y22r(i1,i2,i3,
     & ex))-0.5*(-wsb m(i1,i2,i3,ex)+ umyy22r(i1,i2,i3,ex)-vsb m y22r(
     & i1,i2,i3,ex)) )
               end do
               end do
               end do
              else if( side1.eq.1 .and. side2.eq.1 )then
               do i3=m3a,m3b
               do i2=m2a,m2b
               do i1=m1a,m1b
                 ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
                 ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
                 xx=(i1-i1a)/real(i1b-i1a)
                 sigma1 = layerStrength*xx**power
                 ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
                 ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
                 yy=(i2-i2a)/real(i2b-i2a)
                 sigma2 = layerStrength*yy**power
                 un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( ulaplacian22r(i1,i2,i3,ex) - vrb x22r(i1,i2,i3,ex) -
     & wrb(i1,i2,i3,ex) - vsb y22r(i1,i2,i3,ex) -wsb(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vrb n(i1,i2,i3,ex)=vrb(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -vrb(i1,i2,i3,ex) +  ux22r(i1,i2,i3,ex))-0.5*(-vrb m(i1,i2,i3,
     & ex) + umx22r(i1,i2,i3,ex)) )
                 wrb n(i1,i2,i3,ex)=wrb(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -wrb(i1,i2,i3,ex)+  uxx22r(i1,i2,i3,ex) - vrb x22r(i1,i2,i3,
     & ex))-0.5*(-wrb m(i1,i2,i3,ex)+ umxx22r(i1,i2,i3,ex)-vrb m x22r(
     & i1,i2,i3,ex)) )
                 vsb n(i1,i2,i3,ex)=vsb(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     &  -vsb(i1,i2,i3,ex) +  uy22r(i1,i2,i3,ex))-0.5*(-vsb m(i1,i2,i3,
     & ex) + umy22r(i1,i2,i3,ex)) )
                 wsb n(i1,i2,i3,ex)=wsb(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     & -wsb(i1,i2,i3,ex)+  uyy22r(i1,i2,i3,ex)   -vsb y22r(i1,i2,i3,
     & ex))-0.5*(-wsb m(i1,i2,i3,ex)+ umyy22r(i1,i2,i3,ex)-vsb m y22r(
     & i1,i2,i3,ex)) )
               end do
               end do
               end do
              else
                stop 66244
              end if
             else
               stop 23415
             end if

         end if

        else if( nd.eq.3 )then

         ! *********************************************************************
         ! ******************* Three Dimensions Order=2 ************************
         ! *********************************************************************

         if( boxType.eq.xSide )then

          if( side1.eq.0 )then
            do i3=m3a,m3b
            do i2=m2a,m2b
            do i1=m1a,m1b
              ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
              ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
              xx=(i1-i1a)/real(i1b-i1a)
              sigma = layerStrength*xx**power
              un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( ulaplacian23r(i1,i2,i3,ex) - vra x23r(i1,i2,i3,ex) -
     & wra(i1,i2,i3,ex) )
             ! write(*,'(" i=",3i3," um,u,un,uLap,cdtsq=",5e10.2)') i1,i2,i3,um(i1,i2,i3,ex),u(i1,i2,i3,ex),un(i1,i2,i3,ex),!        ulaplacian23r(i1,i2,i3,ex), cdtsq
              ! auxilliary variables       
              vra n(i1,i2,i3,ex)=vra(i1,i2,i3,ex)+sigma*dt*( 1.5*( -
     & vra(i1,i2,i3,ex) +  u x23r(i1,i2,i3,ex))-0.5*(-vra m(i1,i2,i3,
     & ex) + um x23r(i1,i2,i3,ex)) )
              wra n(i1,i2,i3,ex)=wra(i1,i2,i3,ex)+sigma*dt*( 1.5*( -
     & wra(i1,i2,i3,ex)+  uxx23r(i1,i2,i3,ex)-vra   x23r(i1,i2,i3,ex))
     & -0.5*(-wra m(i1,i2,i3,ex)+ umxx23r(i1,i2,i3,ex)-vra m x23r(i1,
     & i2,i3,ex)) )
            end do
            end do
            end do
          else
            do i3=m3a,m3b
            do i2=m2a,m2b
            do i1=m1a,m1b
              ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
              ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
              xx=(i1-i1a)/real(i1b-i1a)
              sigma = layerStrength*xx**power
              un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( ulaplacian23r(i1,i2,i3,ex) - vrb x23r(i1,i2,i3,ex) -
     & wrb(i1,i2,i3,ex) )
             ! write(*,'(" i=",3i3," um,u,un,uLap,cdtsq=",5e10.2)') i1,i2,i3,um(i1,i2,i3,ex),u(i1,i2,i3,ex),un(i1,i2,i3,ex),!        ulaplacian23r(i1,i2,i3,ex), cdtsq
              ! auxilliary variables       
              vrb n(i1,i2,i3,ex)=vrb(i1,i2,i3,ex)+sigma*dt*( 1.5*( -
     & vrb(i1,i2,i3,ex) +  u x23r(i1,i2,i3,ex))-0.5*(-vrb m(i1,i2,i3,
     & ex) + um x23r(i1,i2,i3,ex)) )
              wrb n(i1,i2,i3,ex)=wrb(i1,i2,i3,ex)+sigma*dt*( 1.5*( -
     & wrb(i1,i2,i3,ex)+  uxx23r(i1,i2,i3,ex)-vrb   x23r(i1,i2,i3,ex))
     & -0.5*(-wrb m(i1,i2,i3,ex)+ umxx23r(i1,i2,i3,ex)-vrb m x23r(i1,
     & i2,i3,ex)) )
            end do
            end do
            end do
          end if

         else if( boxType.eq.ySide )then

          if( side2.eq.0 )then
           do i3=m3a,m3b
           do i2=m2a,m2b
           do i1=m1a,m1b
             ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
             ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
             yy=(i2-i2a)/real(i2b-i2a)
             sigma = layerStrength*yy**power
             un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + cdtsq*
     & ( ulaplacian23r(i1,i2,i3,ex) - vsa y23r(i1,i2,i3,ex) -wsa(i1,
     & i2,i3,ex) )
            ! write(*,'(" i=",3i3," um,u,un,uLap,cdtsq=",5e10.2)') i1,i2,i3,um(i1,i2,i3,ex),u(i1,i2,i3,ex),un(i1,i2,i3,ex),!        ulaplacian23r(i1,i2,i3,ex), cdtsq
             ! auxilliary variables       
             vsa n(i1,i2,i3,ex)=vsa(i1,i2,i3,ex)+sigma*dt*( 1.5*( -vsa(
     & i1,i2,i3,ex) +  u y23r(i1,i2,i3,ex))-0.5*(-vsa m(i1,i2,i3,ex) +
     &  um y23r(i1,i2,i3,ex)) )
             wsa n(i1,i2,i3,ex)=wsa(i1,i2,i3,ex)+sigma*dt*( 1.5*( -wsa(
     & i1,i2,i3,ex)+  uyy23r(i1,i2,i3,ex)-vsa   y23r(i1,i2,i3,ex))-
     & 0.5*(-wsa m(i1,i2,i3,ex)+ umyy23r(i1,i2,i3,ex)-vsa m y23r(i1,
     & i2,i3,ex)) )
           end do
           end do
           end do
          else
           do i3=m3a,m3b
           do i2=m2a,m2b
           do i1=m1a,m1b
             ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
             ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
             yy=(i2-i2a)/real(i2b-i2a)
             sigma = layerStrength*yy**power
             un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + cdtsq*
     & ( ulaplacian23r(i1,i2,i3,ex) - vsb y23r(i1,i2,i3,ex) -wsb(i1,
     & i2,i3,ex) )
            ! write(*,'(" i=",3i3," um,u,un,uLap,cdtsq=",5e10.2)') i1,i2,i3,um(i1,i2,i3,ex),u(i1,i2,i3,ex),un(i1,i2,i3,ex),!        ulaplacian23r(i1,i2,i3,ex), cdtsq
             ! auxilliary variables       
             vsb n(i1,i2,i3,ex)=vsb(i1,i2,i3,ex)+sigma*dt*( 1.5*( -vsb(
     & i1,i2,i3,ex) +  u y23r(i1,i2,i3,ex))-0.5*(-vsb m(i1,i2,i3,ex) +
     &  um y23r(i1,i2,i3,ex)) )
             wsb n(i1,i2,i3,ex)=wsb(i1,i2,i3,ex)+sigma*dt*( 1.5*( -wsb(
     & i1,i2,i3,ex)+  uyy23r(i1,i2,i3,ex)-vsb   y23r(i1,i2,i3,ex))-
     & 0.5*(-wsb m(i1,i2,i3,ex)+ umyy23r(i1,i2,i3,ex)-vsb m y23r(i1,
     & i2,i3,ex)) )
           end do
           end do
           end do
          end if

         else if( boxType.eq.zSide )then

          if( side3.eq.0 )then
           do i3=m3a,m3b
           do i2=m2a,m2b
           do i1=m1a,m1b
             ! zScale=xy(i1,i2,i3b,2)-xy(i1,i2,i3a,2)
             ! zz=(xy(i1,i2,i3,2)-xy(i1,i2,i3a,2))/zScale
             zz=(i3-i3a)/real(i3b-i3a)
             sigma = layerStrength*zz**power
             un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + cdtsq*
     & ( ulaplacian23r(i1,i2,i3,ex) - vta z23r(i1,i2,i3,ex) -wta(i1,
     & i2,i3,ex) )
            ! write(*,'(" i=",3i3," um,u,un,uLap,cdtsq=",5e10.2)') i1,i2,i3,um(i1,i2,i3,ex),u(i1,i2,i3,ex),un(i1,i2,i3,ex),!        ulaplacian23r(i1,i2,i3,ex), cdtsq
             ! auxilliary variables       
             vta n(i1,i2,i3,ex)=vta(i1,i2,i3,ex)+sigma*dt*( 1.5*( -vta(
     & i1,i2,i3,ex) +  u z23r(i1,i2,i3,ex))-0.5*(-vta m(i1,i2,i3,ex) +
     &  um z23r(i1,i2,i3,ex)) )
             wta n(i1,i2,i3,ex)=wta(i1,i2,i3,ex)+sigma*dt*( 1.5*( -wta(
     & i1,i2,i3,ex)+  uzz23r(i1,i2,i3,ex)-vta   z23r(i1,i2,i3,ex))-
     & 0.5*(-wta m(i1,i2,i3,ex)+ umzz23r(i1,i2,i3,ex)-vta m z23r(i1,
     & i2,i3,ex)) )
           end do
           end do
           end do
          else
           do i3=m3a,m3b
           do i2=m2a,m2b
           do i1=m1a,m1b
             ! zScale=xy(i1,i2,i3b,2)-xy(i1,i2,i3a,2)
             ! zz=(xy(i1,i2,i3,2)-xy(i1,i2,i3a,2))/zScale
             zz=(i3-i3a)/real(i3b-i3a)
             sigma = layerStrength*zz**power
             un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + cdtsq*
     & ( ulaplacian23r(i1,i2,i3,ex) - vtb z23r(i1,i2,i3,ex) -wtb(i1,
     & i2,i3,ex) )
            ! write(*,'(" i=",3i3," um,u,un,uLap,cdtsq=",5e10.2)') i1,i2,i3,um(i1,i2,i3,ex),u(i1,i2,i3,ex),un(i1,i2,i3,ex),!        ulaplacian23r(i1,i2,i3,ex), cdtsq
             ! auxilliary variables       
             vtb n(i1,i2,i3,ex)=vtb(i1,i2,i3,ex)+sigma*dt*( 1.5*( -vtb(
     & i1,i2,i3,ex) +  u z23r(i1,i2,i3,ex))-0.5*(-vtb m(i1,i2,i3,ex) +
     &  um z23r(i1,i2,i3,ex)) )
             wtb n(i1,i2,i3,ex)=wtb(i1,i2,i3,ex)+sigma*dt*( 1.5*( -wtb(
     & i1,i2,i3,ex)+  uzz23r(i1,i2,i3,ex)-vtb   z23r(i1,i2,i3,ex))-
     & 0.5*(-wtb m(i1,i2,i3,ex)+ umzz23r(i1,i2,i3,ex)-vtb m z23r(i1,
     & i2,i3,ex)) )
           end do
           end do
           end do
          end if

         else

           ! macro to advance edge and corner regions:
             if( boxType.eq.xyEdge )then
              if( side1.eq.0 .and. side2.eq.0 )then
               do i3=m3a,m3b
               do i2=m2a,m2b
               do i1=m1a,m1b
                 ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
                 ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
                 xx=(i1-i1a)/real(i1b-i1a)
                 sigma1 = layerStrength*xx**power
                 ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
                 ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
                 yy=(i2-i2a)/real(i2b-i2a)
                 sigma2 = layerStrength*yy**power
                 un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( ulaplacian23r(i1,i2,i3,ex) - vra x23r(i1,i2,i3,ex) -
     & wra(i1,i2,i3,ex) - vsa y23r(i1,i2,i3,ex) -wsa(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vra n(i1,i2,i3,ex)=vra(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -vra(i1,i2,i3,ex) +  ux23r(i1,i2,i3,ex))-0.5*(-vra m(i1,i2,i3,
     & ex) + umx23r(i1,i2,i3,ex)) )
                 wra n(i1,i2,i3,ex)=wra(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -wra(i1,i2,i3,ex)+  uxx23r(i1,i2,i3,ex) - vra x23r(i1,i2,i3,
     & ex))-0.5*(-wra m(i1,i2,i3,ex)+ umxx23r(i1,i2,i3,ex)-vra m x23r(
     & i1,i2,i3,ex)) )
                 vsa n(i1,i2,i3,ex)=vsa(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     &  -vsa(i1,i2,i3,ex) +  uy23r(i1,i2,i3,ex))-0.5*(-vsa m(i1,i2,i3,
     & ex) + umy23r(i1,i2,i3,ex)) )
                 wsa n(i1,i2,i3,ex)=wsa(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     & -wsa(i1,i2,i3,ex)+  uyy23r(i1,i2,i3,ex)   -vsa y23r(i1,i2,i3,
     & ex))-0.5*(-wsa m(i1,i2,i3,ex)+ umyy23r(i1,i2,i3,ex)-vsa m y23r(
     & i1,i2,i3,ex)) )
               end do
               end do
               end do
              else if( side1.eq.1 .and. side2.eq.0 )then
               do i3=m3a,m3b
               do i2=m2a,m2b
               do i1=m1a,m1b
                 ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
                 ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
                 xx=(i1-i1a)/real(i1b-i1a)
                 sigma1 = layerStrength*xx**power
                 ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
                 ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
                 yy=(i2-i2a)/real(i2b-i2a)
                 sigma2 = layerStrength*yy**power
                 un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( ulaplacian23r(i1,i2,i3,ex) - vrb x23r(i1,i2,i3,ex) -
     & wrb(i1,i2,i3,ex) - vsa y23r(i1,i2,i3,ex) -wsa(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vrb n(i1,i2,i3,ex)=vrb(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -vrb(i1,i2,i3,ex) +  ux23r(i1,i2,i3,ex))-0.5*(-vrb m(i1,i2,i3,
     & ex) + umx23r(i1,i2,i3,ex)) )
                 wrb n(i1,i2,i3,ex)=wrb(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -wrb(i1,i2,i3,ex)+  uxx23r(i1,i2,i3,ex) - vrb x23r(i1,i2,i3,
     & ex))-0.5*(-wrb m(i1,i2,i3,ex)+ umxx23r(i1,i2,i3,ex)-vrb m x23r(
     & i1,i2,i3,ex)) )
                 vsa n(i1,i2,i3,ex)=vsa(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     &  -vsa(i1,i2,i3,ex) +  uy23r(i1,i2,i3,ex))-0.5*(-vsa m(i1,i2,i3,
     & ex) + umy23r(i1,i2,i3,ex)) )
                 wsa n(i1,i2,i3,ex)=wsa(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     & -wsa(i1,i2,i3,ex)+  uyy23r(i1,i2,i3,ex)   -vsa y23r(i1,i2,i3,
     & ex))-0.5*(-wsa m(i1,i2,i3,ex)+ umyy23r(i1,i2,i3,ex)-vsa m y23r(
     & i1,i2,i3,ex)) )
               end do
               end do
               end do
              else if( side1.eq.0 .and. side2.eq.1 )then
               do i3=m3a,m3b
               do i2=m2a,m2b
               do i1=m1a,m1b
                 ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
                 ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
                 xx=(i1-i1a)/real(i1b-i1a)
                 sigma1 = layerStrength*xx**power
                 ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
                 ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
                 yy=(i2-i2a)/real(i2b-i2a)
                 sigma2 = layerStrength*yy**power
                 un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( ulaplacian23r(i1,i2,i3,ex) - vra x23r(i1,i2,i3,ex) -
     & wra(i1,i2,i3,ex) - vsb y23r(i1,i2,i3,ex) -wsb(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vra n(i1,i2,i3,ex)=vra(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -vra(i1,i2,i3,ex) +  ux23r(i1,i2,i3,ex))-0.5*(-vra m(i1,i2,i3,
     & ex) + umx23r(i1,i2,i3,ex)) )
                 wra n(i1,i2,i3,ex)=wra(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -wra(i1,i2,i3,ex)+  uxx23r(i1,i2,i3,ex) - vra x23r(i1,i2,i3,
     & ex))-0.5*(-wra m(i1,i2,i3,ex)+ umxx23r(i1,i2,i3,ex)-vra m x23r(
     & i1,i2,i3,ex)) )
                 vsb n(i1,i2,i3,ex)=vsb(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     &  -vsb(i1,i2,i3,ex) +  uy23r(i1,i2,i3,ex))-0.5*(-vsb m(i1,i2,i3,
     & ex) + umy23r(i1,i2,i3,ex)) )
                 wsb n(i1,i2,i3,ex)=wsb(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     & -wsb(i1,i2,i3,ex)+  uyy23r(i1,i2,i3,ex)   -vsb y23r(i1,i2,i3,
     & ex))-0.5*(-wsb m(i1,i2,i3,ex)+ umyy23r(i1,i2,i3,ex)-vsb m y23r(
     & i1,i2,i3,ex)) )
               end do
               end do
               end do
              else if( side1.eq.1 .and. side2.eq.1 )then
               do i3=m3a,m3b
               do i2=m2a,m2b
               do i1=m1a,m1b
                 ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
                 ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
                 xx=(i1-i1a)/real(i1b-i1a)
                 sigma1 = layerStrength*xx**power
                 ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
                 ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
                 yy=(i2-i2a)/real(i2b-i2a)
                 sigma2 = layerStrength*yy**power
                 un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( ulaplacian23r(i1,i2,i3,ex) - vrb x23r(i1,i2,i3,ex) -
     & wrb(i1,i2,i3,ex) - vsb y23r(i1,i2,i3,ex) -wsb(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vrb n(i1,i2,i3,ex)=vrb(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -vrb(i1,i2,i3,ex) +  ux23r(i1,i2,i3,ex))-0.5*(-vrb m(i1,i2,i3,
     & ex) + umx23r(i1,i2,i3,ex)) )
                 wrb n(i1,i2,i3,ex)=wrb(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -wrb(i1,i2,i3,ex)+  uxx23r(i1,i2,i3,ex) - vrb x23r(i1,i2,i3,
     & ex))-0.5*(-wrb m(i1,i2,i3,ex)+ umxx23r(i1,i2,i3,ex)-vrb m x23r(
     & i1,i2,i3,ex)) )
                 vsb n(i1,i2,i3,ex)=vsb(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     &  -vsb(i1,i2,i3,ex) +  uy23r(i1,i2,i3,ex))-0.5*(-vsb m(i1,i2,i3,
     & ex) + umy23r(i1,i2,i3,ex)) )
                 wsb n(i1,i2,i3,ex)=wsb(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     & -wsb(i1,i2,i3,ex)+  uyy23r(i1,i2,i3,ex)   -vsb y23r(i1,i2,i3,
     & ex))-0.5*(-wsb m(i1,i2,i3,ex)+ umyy23r(i1,i2,i3,ex)-vsb m y23r(
     & i1,i2,i3,ex)) )
               end do
               end do
               end do
              else
                stop 66224
              end if
             else if( boxType.eq.xzEdge )then
              if( side1.eq.0 .and. side3.eq.0 )then
               do i3=m3a,m3b
               do i2=m2a,m2b
               do i1=m1a,m1b
                 ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
                 ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
                 xx=(i1-i1a)/real(i1b-i1a)
                 sigma1 = layerStrength*xx**power
                 ! zScale=xy(i1,i2,i3b,2)-xy(i1,i2,i3a,2)
                 ! zz=(xy(i1,i2,i3,2)-xy(i1,i2,i3a,2))/zScale
                 zz=(i3-i3a)/real(i3b-i3a)
                 sigma3 = layerStrength*zz**power
                 un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( ulaplacian23r(i1,i2,i3,ex) - vra x23r(i1,i2,i3,ex) -
     & wra(i1,i2,i3,ex) - vta z23r(i1,i2,i3,ex) -wta(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vra n(i1,i2,i3,ex)=vra(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -vra(i1,i2,i3,ex) +  ux23r(i1,i2,i3,ex))-0.5*(-vra m(i1,i2,i3,
     & ex) + umx23r(i1,i2,i3,ex)) )
                 wra n(i1,i2,i3,ex)=wra(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -wra(i1,i2,i3,ex)+  uxx23r(i1,i2,i3,ex) - vra x23r(i1,i2,i3,
     & ex))-0.5*(-wra m(i1,i2,i3,ex)+ umxx23r(i1,i2,i3,ex)-vra m x23r(
     & i1,i2,i3,ex)) )
                 vta n(i1,i2,i3,ex)=vta(i1,i2,i3,ex)+sigma3*dt*( 1.5*( 
     &  -vta(i1,i2,i3,ex) +  uz23r(i1,i2,i3,ex))-0.5*(-vta m(i1,i2,i3,
     & ex) + umz23r(i1,i2,i3,ex)) )
                 wta n(i1,i2,i3,ex)=wta(i1,i2,i3,ex)+sigma3*dt*( 1.5*( 
     & -wta(i1,i2,i3,ex)+  uzz23r(i1,i2,i3,ex)   -vta z23r(i1,i2,i3,
     & ex))-0.5*(-wta m(i1,i2,i3,ex)+ umzz23r(i1,i2,i3,ex)-vta m z23r(
     & i1,i2,i3,ex)) )
               end do
               end do
               end do
              else if( side1.eq.1 .and. side3.eq.0 )then
               do i3=m3a,m3b
               do i2=m2a,m2b
               do i1=m1a,m1b
                 ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
                 ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
                 xx=(i1-i1a)/real(i1b-i1a)
                 sigma1 = layerStrength*xx**power
                 ! zScale=xy(i1,i2,i3b,2)-xy(i1,i2,i3a,2)
                 ! zz=(xy(i1,i2,i3,2)-xy(i1,i2,i3a,2))/zScale
                 zz=(i3-i3a)/real(i3b-i3a)
                 sigma3 = layerStrength*zz**power
                 un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( ulaplacian23r(i1,i2,i3,ex) - vrb x23r(i1,i2,i3,ex) -
     & wrb(i1,i2,i3,ex) - vta z23r(i1,i2,i3,ex) -wta(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vrb n(i1,i2,i3,ex)=vrb(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -vrb(i1,i2,i3,ex) +  ux23r(i1,i2,i3,ex))-0.5*(-vrb m(i1,i2,i3,
     & ex) + umx23r(i1,i2,i3,ex)) )
                 wrb n(i1,i2,i3,ex)=wrb(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -wrb(i1,i2,i3,ex)+  uxx23r(i1,i2,i3,ex) - vrb x23r(i1,i2,i3,
     & ex))-0.5*(-wrb m(i1,i2,i3,ex)+ umxx23r(i1,i2,i3,ex)-vrb m x23r(
     & i1,i2,i3,ex)) )
                 vta n(i1,i2,i3,ex)=vta(i1,i2,i3,ex)+sigma3*dt*( 1.5*( 
     &  -vta(i1,i2,i3,ex) +  uz23r(i1,i2,i3,ex))-0.5*(-vta m(i1,i2,i3,
     & ex) + umz23r(i1,i2,i3,ex)) )
                 wta n(i1,i2,i3,ex)=wta(i1,i2,i3,ex)+sigma3*dt*( 1.5*( 
     & -wta(i1,i2,i3,ex)+  uzz23r(i1,i2,i3,ex)   -vta z23r(i1,i2,i3,
     & ex))-0.5*(-wta m(i1,i2,i3,ex)+ umzz23r(i1,i2,i3,ex)-vta m z23r(
     & i1,i2,i3,ex)) )
               end do
               end do
               end do
              else if( side1.eq.0 .and. side3.eq.1 )then
               do i3=m3a,m3b
               do i2=m2a,m2b
               do i1=m1a,m1b
                 ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
                 ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
                 xx=(i1-i1a)/real(i1b-i1a)
                 sigma1 = layerStrength*xx**power
                 ! zScale=xy(i1,i2,i3b,2)-xy(i1,i2,i3a,2)
                 ! zz=(xy(i1,i2,i3,2)-xy(i1,i2,i3a,2))/zScale
                 zz=(i3-i3a)/real(i3b-i3a)
                 sigma3 = layerStrength*zz**power
                 un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( ulaplacian23r(i1,i2,i3,ex) - vra x23r(i1,i2,i3,ex) -
     & wra(i1,i2,i3,ex) - vtb z23r(i1,i2,i3,ex) -wtb(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vra n(i1,i2,i3,ex)=vra(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -vra(i1,i2,i3,ex) +  ux23r(i1,i2,i3,ex))-0.5*(-vra m(i1,i2,i3,
     & ex) + umx23r(i1,i2,i3,ex)) )
                 wra n(i1,i2,i3,ex)=wra(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -wra(i1,i2,i3,ex)+  uxx23r(i1,i2,i3,ex) - vra x23r(i1,i2,i3,
     & ex))-0.5*(-wra m(i1,i2,i3,ex)+ umxx23r(i1,i2,i3,ex)-vra m x23r(
     & i1,i2,i3,ex)) )
                 vtb n(i1,i2,i3,ex)=vtb(i1,i2,i3,ex)+sigma3*dt*( 1.5*( 
     &  -vtb(i1,i2,i3,ex) +  uz23r(i1,i2,i3,ex))-0.5*(-vtb m(i1,i2,i3,
     & ex) + umz23r(i1,i2,i3,ex)) )
                 wtb n(i1,i2,i3,ex)=wtb(i1,i2,i3,ex)+sigma3*dt*( 1.5*( 
     & -wtb(i1,i2,i3,ex)+  uzz23r(i1,i2,i3,ex)   -vtb z23r(i1,i2,i3,
     & ex))-0.5*(-wtb m(i1,i2,i3,ex)+ umzz23r(i1,i2,i3,ex)-vtb m z23r(
     & i1,i2,i3,ex)) )
               end do
               end do
               end do
              else if( side1.eq.1 .and. side3.eq.1 )then
               do i3=m3a,m3b
               do i2=m2a,m2b
               do i1=m1a,m1b
                 ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
                 ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
                 xx=(i1-i1a)/real(i1b-i1a)
                 sigma1 = layerStrength*xx**power
                 ! zScale=xy(i1,i2,i3b,2)-xy(i1,i2,i3a,2)
                 ! zz=(xy(i1,i2,i3,2)-xy(i1,i2,i3a,2))/zScale
                 zz=(i3-i3a)/real(i3b-i3a)
                 sigma3 = layerStrength*zz**power
                 un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( ulaplacian23r(i1,i2,i3,ex) - vrb x23r(i1,i2,i3,ex) -
     & wrb(i1,i2,i3,ex) - vtb z23r(i1,i2,i3,ex) -wtb(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vrb n(i1,i2,i3,ex)=vrb(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -vrb(i1,i2,i3,ex) +  ux23r(i1,i2,i3,ex))-0.5*(-vrb m(i1,i2,i3,
     & ex) + umx23r(i1,i2,i3,ex)) )
                 wrb n(i1,i2,i3,ex)=wrb(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -wrb(i1,i2,i3,ex)+  uxx23r(i1,i2,i3,ex) - vrb x23r(i1,i2,i3,
     & ex))-0.5*(-wrb m(i1,i2,i3,ex)+ umxx23r(i1,i2,i3,ex)-vrb m x23r(
     & i1,i2,i3,ex)) )
                 vtb n(i1,i2,i3,ex)=vtb(i1,i2,i3,ex)+sigma3*dt*( 1.5*( 
     &  -vtb(i1,i2,i3,ex) +  uz23r(i1,i2,i3,ex))-0.5*(-vtb m(i1,i2,i3,
     & ex) + umz23r(i1,i2,i3,ex)) )
                 wtb n(i1,i2,i3,ex)=wtb(i1,i2,i3,ex)+sigma3*dt*( 1.5*( 
     & -wtb(i1,i2,i3,ex)+  uzz23r(i1,i2,i3,ex)   -vtb z23r(i1,i2,i3,
     & ex))-0.5*(-wtb m(i1,i2,i3,ex)+ umzz23r(i1,i2,i3,ex)-vtb m z23r(
     & i1,i2,i3,ex)) )
               end do
               end do
               end do
              else
                stop 62244
              end if
             else if( boxType.eq.yzEdge )then
              if( side2.eq.0 .and. side3.eq.0 )then
               do i3=m3a,m3b
               do i2=m2a,m2b
               do i1=m1a,m1b
                 ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
                 ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
                 yy=(i2-i2a)/real(i2b-i2a)
                 sigma2 = layerStrength*yy**power
                 ! zScale=xy(i1,i2,i3b,2)-xy(i1,i2,i3a,2)
                 ! zz=(xy(i1,i2,i3,2)-xy(i1,i2,i3a,2))/zScale
                 zz=(i3-i3a)/real(i3b-i3a)
                 sigma3 = layerStrength*zz**power
                 un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( ulaplacian23r(i1,i2,i3,ex) - vsa y23r(i1,i2,i3,ex) -
     & wsa(i1,i2,i3,ex) - vta z23r(i1,i2,i3,ex) -wta(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vsa n(i1,i2,i3,ex)=vsa(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     &  -vsa(i1,i2,i3,ex) +  uy23r(i1,i2,i3,ex))-0.5*(-vsa m(i1,i2,i3,
     & ex) + umy23r(i1,i2,i3,ex)) )
                 wsa n(i1,i2,i3,ex)=wsa(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     &  -wsa(i1,i2,i3,ex)+  uyy23r(i1,i2,i3,ex) - vsa y23r(i1,i2,i3,
     & ex))-0.5*(-wsa m(i1,i2,i3,ex)+ umyy23r(i1,i2,i3,ex)-vsa m y23r(
     & i1,i2,i3,ex)) )
                 vta n(i1,i2,i3,ex)=vta(i1,i2,i3,ex)+sigma3*dt*( 1.5*( 
     &  -vta(i1,i2,i3,ex) +  uz23r(i1,i2,i3,ex))-0.5*(-vta m(i1,i2,i3,
     & ex) + umz23r(i1,i2,i3,ex)) )
                 wta n(i1,i2,i3,ex)=wta(i1,i2,i3,ex)+sigma3*dt*( 1.5*( 
     & -wta(i1,i2,i3,ex)+  uzz23r(i1,i2,i3,ex)   -vta z23r(i1,i2,i3,
     & ex))-0.5*(-wta m(i1,i2,i3,ex)+ umzz23r(i1,i2,i3,ex)-vta m z23r(
     & i1,i2,i3,ex)) )
               end do
               end do
               end do
              else if( side2.eq.1 .and. side3.eq.0 )then
               do i3=m3a,m3b
               do i2=m2a,m2b
               do i1=m1a,m1b
                 ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
                 ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
                 yy=(i2-i2a)/real(i2b-i2a)
                 sigma2 = layerStrength*yy**power
                 ! zScale=xy(i1,i2,i3b,2)-xy(i1,i2,i3a,2)
                 ! zz=(xy(i1,i2,i3,2)-xy(i1,i2,i3a,2))/zScale
                 zz=(i3-i3a)/real(i3b-i3a)
                 sigma3 = layerStrength*zz**power
                 un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( ulaplacian23r(i1,i2,i3,ex) - vsb y23r(i1,i2,i3,ex) -
     & wsb(i1,i2,i3,ex) - vta z23r(i1,i2,i3,ex) -wta(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vsb n(i1,i2,i3,ex)=vsb(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     &  -vsb(i1,i2,i3,ex) +  uy23r(i1,i2,i3,ex))-0.5*(-vsb m(i1,i2,i3,
     & ex) + umy23r(i1,i2,i3,ex)) )
                 wsb n(i1,i2,i3,ex)=wsb(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     &  -wsb(i1,i2,i3,ex)+  uyy23r(i1,i2,i3,ex) - vsb y23r(i1,i2,i3,
     & ex))-0.5*(-wsb m(i1,i2,i3,ex)+ umyy23r(i1,i2,i3,ex)-vsb m y23r(
     & i1,i2,i3,ex)) )
                 vta n(i1,i2,i3,ex)=vta(i1,i2,i3,ex)+sigma3*dt*( 1.5*( 
     &  -vta(i1,i2,i3,ex) +  uz23r(i1,i2,i3,ex))-0.5*(-vta m(i1,i2,i3,
     & ex) + umz23r(i1,i2,i3,ex)) )
                 wta n(i1,i2,i3,ex)=wta(i1,i2,i3,ex)+sigma3*dt*( 1.5*( 
     & -wta(i1,i2,i3,ex)+  uzz23r(i1,i2,i3,ex)   -vta z23r(i1,i2,i3,
     & ex))-0.5*(-wta m(i1,i2,i3,ex)+ umzz23r(i1,i2,i3,ex)-vta m z23r(
     & i1,i2,i3,ex)) )
               end do
               end do
               end do
              else if( side2.eq.0 .and. side3.eq.1 )then
               do i3=m3a,m3b
               do i2=m2a,m2b
               do i1=m1a,m1b
                 ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
                 ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
                 yy=(i2-i2a)/real(i2b-i2a)
                 sigma2 = layerStrength*yy**power
                 ! zScale=xy(i1,i2,i3b,2)-xy(i1,i2,i3a,2)
                 ! zz=(xy(i1,i2,i3,2)-xy(i1,i2,i3a,2))/zScale
                 zz=(i3-i3a)/real(i3b-i3a)
                 sigma3 = layerStrength*zz**power
                 un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( ulaplacian23r(i1,i2,i3,ex) - vsa y23r(i1,i2,i3,ex) -
     & wsa(i1,i2,i3,ex) - vtb z23r(i1,i2,i3,ex) -wtb(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vsa n(i1,i2,i3,ex)=vsa(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     &  -vsa(i1,i2,i3,ex) +  uy23r(i1,i2,i3,ex))-0.5*(-vsa m(i1,i2,i3,
     & ex) + umy23r(i1,i2,i3,ex)) )
                 wsa n(i1,i2,i3,ex)=wsa(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     &  -wsa(i1,i2,i3,ex)+  uyy23r(i1,i2,i3,ex) - vsa y23r(i1,i2,i3,
     & ex))-0.5*(-wsa m(i1,i2,i3,ex)+ umyy23r(i1,i2,i3,ex)-vsa m y23r(
     & i1,i2,i3,ex)) )
                 vtb n(i1,i2,i3,ex)=vtb(i1,i2,i3,ex)+sigma3*dt*( 1.5*( 
     &  -vtb(i1,i2,i3,ex) +  uz23r(i1,i2,i3,ex))-0.5*(-vtb m(i1,i2,i3,
     & ex) + umz23r(i1,i2,i3,ex)) )
                 wtb n(i1,i2,i3,ex)=wtb(i1,i2,i3,ex)+sigma3*dt*( 1.5*( 
     & -wtb(i1,i2,i3,ex)+  uzz23r(i1,i2,i3,ex)   -vtb z23r(i1,i2,i3,
     & ex))-0.5*(-wtb m(i1,i2,i3,ex)+ umzz23r(i1,i2,i3,ex)-vtb m z23r(
     & i1,i2,i3,ex)) )
               end do
               end do
               end do
              else if( side2.eq.1 .and. side3.eq.1 )then
               do i3=m3a,m3b
               do i2=m2a,m2b
               do i1=m1a,m1b
                 ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
                 ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
                 yy=(i2-i2a)/real(i2b-i2a)
                 sigma2 = layerStrength*yy**power
                 ! zScale=xy(i1,i2,i3b,2)-xy(i1,i2,i3a,2)
                 ! zz=(xy(i1,i2,i3,2)-xy(i1,i2,i3a,2))/zScale
                 zz=(i3-i3a)/real(i3b-i3a)
                 sigma3 = layerStrength*zz**power
                 un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( ulaplacian23r(i1,i2,i3,ex) - vsb y23r(i1,i2,i3,ex) -
     & wsb(i1,i2,i3,ex) - vtb z23r(i1,i2,i3,ex) -wtb(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vsb n(i1,i2,i3,ex)=vsb(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     &  -vsb(i1,i2,i3,ex) +  uy23r(i1,i2,i3,ex))-0.5*(-vsb m(i1,i2,i3,
     & ex) + umy23r(i1,i2,i3,ex)) )
                 wsb n(i1,i2,i3,ex)=wsb(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     &  -wsb(i1,i2,i3,ex)+  uyy23r(i1,i2,i3,ex) - vsb y23r(i1,i2,i3,
     & ex))-0.5*(-wsb m(i1,i2,i3,ex)+ umyy23r(i1,i2,i3,ex)-vsb m y23r(
     & i1,i2,i3,ex)) )
                 vtb n(i1,i2,i3,ex)=vtb(i1,i2,i3,ex)+sigma3*dt*( 1.5*( 
     &  -vtb(i1,i2,i3,ex) +  uz23r(i1,i2,i3,ex))-0.5*(-vtb m(i1,i2,i3,
     & ex) + umz23r(i1,i2,i3,ex)) )
                 wtb n(i1,i2,i3,ex)=wtb(i1,i2,i3,ex)+sigma3*dt*( 1.5*( 
     & -wtb(i1,i2,i3,ex)+  uzz23r(i1,i2,i3,ex)   -vtb z23r(i1,i2,i3,
     & ex))-0.5*(-wtb m(i1,i2,i3,ex)+ umzz23r(i1,i2,i3,ex)-vtb m z23r(
     & i1,i2,i3,ex)) )
               end do
               end do
               end do
              else
                stop 6644
              end if
             else if( boxType.eq.xyzCorner )then
              if(      side1.eq.0 .and. side2.eq.0 .and. side3.eq.0 )
     & then
                 do i3=m3a,m3b
                 do i2=m2a,m2b
                 do i1=m1a,m1b
                    ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
                    ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
                    xx=(i1-i1a)/real(i1b-i1a)
                    sigma1 = layerStrength*xx**power
                    ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
                    ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
                    yy=(i2-i2a)/real(i2b-i2a)
                    sigma2 = layerStrength*yy**power
                    ! zScale=xy(i1,i2,i3b,2)-xy(i1,i2,i3a,2)
                    ! zz=(xy(i1,i2,i3,2)-xy(i1,i2,i3a,2))/zScale
                    zz=(i3-i3a)/real(i3b-i3a)
                    sigma3 = layerStrength*zz**power
                   un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( u laplacian 23r(i1,i2,i3,ex) - vra x 23r(i1,i2,i3,ex) -
     & wra(i1,i2,i3,ex) - vsa y 23r(i1,i2,i3,ex) -wsa(i1,i2,i3,ex) - 
     & vta z 23r(i1,i2,i3,ex) -wta(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vra n(i1,i2,i3,ex)=vra(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -vra(i1,i2,i3,ex) +  u x 23r(i1,i2,i3,ex))-0.5*(-vra m(i1,i2,
     & i3,ex) + um x 23r(i1,i2,i3,ex)) )
                 wra n(i1,i2,i3,ex)=wra(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -wra(i1,i2,i3,ex)+  u xx 23r(i1,i2,i3,ex) - vra   x 23r(i1,i2,
     & i3,ex))-0.5*(-wra m(i1,i2,i3,ex)+ um xx 23r(i1,i2,i3,ex) - vra 
     & m x 23r(i1,i2,i3,ex)) )
                 vsa n(i1,i2,i3,ex)=vsa(i1,i2,i3,ex)+sigma2*dt*( 1.5*(-
     & vsa  (i1,i2,i3,ex) +  u y 23r(i1,i2,i3,ex))-0.5*(-vsa m(i1,i2,
     & i3,ex) + um y 23r(i1,i2,i3,ex)) )
                 wsa n(i1,i2,i3,ex)=wsa(i1,i2,i3,ex)+sigma2*dt*( 1.5*(-
     & wsa  (i1,i2,i3,ex)+ u  yy 23r(i1,i2,i3,ex) -vsa   y 23r(i1,i2,
     & i3,ex))-0.5*(-wsa m(i1,i2,i3,ex)+ um yy 23r(i1,i2,i3,ex) -vsa 
     & m y 23r(i1,i2,i3,ex)) )
                 vta n(i1,i2,i3,ex)=vta(i1,i2,i3,ex)+sigma3*dt*( 1.5*(-
     & vta  (i1,i2,i3,ex) +  u z 23r(i1,i2,i3,ex))-0.5*(-vta m(i1,i2,
     & i3,ex) + um z 23r(i1,i2,i3,ex)) )
                 wta n(i1,i2,i3,ex)=wta(i1,i2,i3,ex)+sigma3*dt*( 1.5*(-
     & wta  (i1,i2,i3,ex)+ u  zz 23r(i1,i2,i3,ex) -vta   z 23r(i1,i2,
     & i3,ex))-0.5*(-wta m(i1,i2,i3,ex)+ um zz 23r(i1,i2,i3,ex) -vta 
     & m z 23r(i1,i2,i3,ex)) )
                 end do
                 end do
                 end do
              else if( side1.eq.1 .and. side2.eq.0 .and. side3.eq.0 )
     & then
                 do i3=m3a,m3b
                 do i2=m2a,m2b
                 do i1=m1a,m1b
                    ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
                    ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
                    xx=(i1-i1a)/real(i1b-i1a)
                    sigma1 = layerStrength*xx**power
                    ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
                    ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
                    yy=(i2-i2a)/real(i2b-i2a)
                    sigma2 = layerStrength*yy**power
                    ! zScale=xy(i1,i2,i3b,2)-xy(i1,i2,i3a,2)
                    ! zz=(xy(i1,i2,i3,2)-xy(i1,i2,i3a,2))/zScale
                    zz=(i3-i3a)/real(i3b-i3a)
                    sigma3 = layerStrength*zz**power
                   un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( u laplacian 23r(i1,i2,i3,ex) - vrb x 23r(i1,i2,i3,ex) -
     & wrb(i1,i2,i3,ex) - vsa y 23r(i1,i2,i3,ex) -wsa(i1,i2,i3,ex) - 
     & vta z 23r(i1,i2,i3,ex) -wta(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vrb n(i1,i2,i3,ex)=vrb(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -vrb(i1,i2,i3,ex) +  u x 23r(i1,i2,i3,ex))-0.5*(-vrb m(i1,i2,
     & i3,ex) + um x 23r(i1,i2,i3,ex)) )
                 wrb n(i1,i2,i3,ex)=wrb(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -wrb(i1,i2,i3,ex)+  u xx 23r(i1,i2,i3,ex) - vrb   x 23r(i1,i2,
     & i3,ex))-0.5*(-wrb m(i1,i2,i3,ex)+ um xx 23r(i1,i2,i3,ex) - vrb 
     & m x 23r(i1,i2,i3,ex)) )
                 vsa n(i1,i2,i3,ex)=vsa(i1,i2,i3,ex)+sigma2*dt*( 1.5*(-
     & vsa  (i1,i2,i3,ex) +  u y 23r(i1,i2,i3,ex))-0.5*(-vsa m(i1,i2,
     & i3,ex) + um y 23r(i1,i2,i3,ex)) )
                 wsa n(i1,i2,i3,ex)=wsa(i1,i2,i3,ex)+sigma2*dt*( 1.5*(-
     & wsa  (i1,i2,i3,ex)+ u  yy 23r(i1,i2,i3,ex) -vsa   y 23r(i1,i2,
     & i3,ex))-0.5*(-wsa m(i1,i2,i3,ex)+ um yy 23r(i1,i2,i3,ex) -vsa 
     & m y 23r(i1,i2,i3,ex)) )
                 vta n(i1,i2,i3,ex)=vta(i1,i2,i3,ex)+sigma3*dt*( 1.5*(-
     & vta  (i1,i2,i3,ex) +  u z 23r(i1,i2,i3,ex))-0.5*(-vta m(i1,i2,
     & i3,ex) + um z 23r(i1,i2,i3,ex)) )
                 wta n(i1,i2,i3,ex)=wta(i1,i2,i3,ex)+sigma3*dt*( 1.5*(-
     & wta  (i1,i2,i3,ex)+ u  zz 23r(i1,i2,i3,ex) -vta   z 23r(i1,i2,
     & i3,ex))-0.5*(-wta m(i1,i2,i3,ex)+ um zz 23r(i1,i2,i3,ex) -vta 
     & m z 23r(i1,i2,i3,ex)) )
                 end do
                 end do
                 end do
              else if( side1.eq.0 .and. side2.eq.1 .and. side3.eq.0 )
     & then
                 do i3=m3a,m3b
                 do i2=m2a,m2b
                 do i1=m1a,m1b
                    ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
                    ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
                    xx=(i1-i1a)/real(i1b-i1a)
                    sigma1 = layerStrength*xx**power
                    ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
                    ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
                    yy=(i2-i2a)/real(i2b-i2a)
                    sigma2 = layerStrength*yy**power
                    ! zScale=xy(i1,i2,i3b,2)-xy(i1,i2,i3a,2)
                    ! zz=(xy(i1,i2,i3,2)-xy(i1,i2,i3a,2))/zScale
                    zz=(i3-i3a)/real(i3b-i3a)
                    sigma3 = layerStrength*zz**power
                   un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( u laplacian 23r(i1,i2,i3,ex) - vra x 23r(i1,i2,i3,ex) -
     & wra(i1,i2,i3,ex) - vsb y 23r(i1,i2,i3,ex) -wsb(i1,i2,i3,ex) - 
     & vta z 23r(i1,i2,i3,ex) -wta(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vra n(i1,i2,i3,ex)=vra(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -vra(i1,i2,i3,ex) +  u x 23r(i1,i2,i3,ex))-0.5*(-vra m(i1,i2,
     & i3,ex) + um x 23r(i1,i2,i3,ex)) )
                 wra n(i1,i2,i3,ex)=wra(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -wra(i1,i2,i3,ex)+  u xx 23r(i1,i2,i3,ex) - vra   x 23r(i1,i2,
     & i3,ex))-0.5*(-wra m(i1,i2,i3,ex)+ um xx 23r(i1,i2,i3,ex) - vra 
     & m x 23r(i1,i2,i3,ex)) )
                 vsb n(i1,i2,i3,ex)=vsb(i1,i2,i3,ex)+sigma2*dt*( 1.5*(-
     & vsb  (i1,i2,i3,ex) +  u y 23r(i1,i2,i3,ex))-0.5*(-vsb m(i1,i2,
     & i3,ex) + um y 23r(i1,i2,i3,ex)) )
                 wsb n(i1,i2,i3,ex)=wsb(i1,i2,i3,ex)+sigma2*dt*( 1.5*(-
     & wsb  (i1,i2,i3,ex)+ u  yy 23r(i1,i2,i3,ex) -vsb   y 23r(i1,i2,
     & i3,ex))-0.5*(-wsb m(i1,i2,i3,ex)+ um yy 23r(i1,i2,i3,ex) -vsb 
     & m y 23r(i1,i2,i3,ex)) )
                 vta n(i1,i2,i3,ex)=vta(i1,i2,i3,ex)+sigma3*dt*( 1.5*(-
     & vta  (i1,i2,i3,ex) +  u z 23r(i1,i2,i3,ex))-0.5*(-vta m(i1,i2,
     & i3,ex) + um z 23r(i1,i2,i3,ex)) )
                 wta n(i1,i2,i3,ex)=wta(i1,i2,i3,ex)+sigma3*dt*( 1.5*(-
     & wta  (i1,i2,i3,ex)+ u  zz 23r(i1,i2,i3,ex) -vta   z 23r(i1,i2,
     & i3,ex))-0.5*(-wta m(i1,i2,i3,ex)+ um zz 23r(i1,i2,i3,ex) -vta 
     & m z 23r(i1,i2,i3,ex)) )
                 end do
                 end do
                 end do
              else if( side1.eq.1 .and. side2.eq.1 .and. side3.eq.0 )
     & then
                 do i3=m3a,m3b
                 do i2=m2a,m2b
                 do i1=m1a,m1b
                    ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
                    ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
                    xx=(i1-i1a)/real(i1b-i1a)
                    sigma1 = layerStrength*xx**power
                    ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
                    ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
                    yy=(i2-i2a)/real(i2b-i2a)
                    sigma2 = layerStrength*yy**power
                    ! zScale=xy(i1,i2,i3b,2)-xy(i1,i2,i3a,2)
                    ! zz=(xy(i1,i2,i3,2)-xy(i1,i2,i3a,2))/zScale
                    zz=(i3-i3a)/real(i3b-i3a)
                    sigma3 = layerStrength*zz**power
                   un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( u laplacian 23r(i1,i2,i3,ex) - vrb x 23r(i1,i2,i3,ex) -
     & wrb(i1,i2,i3,ex) - vsb y 23r(i1,i2,i3,ex) -wsb(i1,i2,i3,ex) - 
     & vta z 23r(i1,i2,i3,ex) -wta(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vrb n(i1,i2,i3,ex)=vrb(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -vrb(i1,i2,i3,ex) +  u x 23r(i1,i2,i3,ex))-0.5*(-vrb m(i1,i2,
     & i3,ex) + um x 23r(i1,i2,i3,ex)) )
                 wrb n(i1,i2,i3,ex)=wrb(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -wrb(i1,i2,i3,ex)+  u xx 23r(i1,i2,i3,ex) - vrb   x 23r(i1,i2,
     & i3,ex))-0.5*(-wrb m(i1,i2,i3,ex)+ um xx 23r(i1,i2,i3,ex) - vrb 
     & m x 23r(i1,i2,i3,ex)) )
                 vsb n(i1,i2,i3,ex)=vsb(i1,i2,i3,ex)+sigma2*dt*( 1.5*(-
     & vsb  (i1,i2,i3,ex) +  u y 23r(i1,i2,i3,ex))-0.5*(-vsb m(i1,i2,
     & i3,ex) + um y 23r(i1,i2,i3,ex)) )
                 wsb n(i1,i2,i3,ex)=wsb(i1,i2,i3,ex)+sigma2*dt*( 1.5*(-
     & wsb  (i1,i2,i3,ex)+ u  yy 23r(i1,i2,i3,ex) -vsb   y 23r(i1,i2,
     & i3,ex))-0.5*(-wsb m(i1,i2,i3,ex)+ um yy 23r(i1,i2,i3,ex) -vsb 
     & m y 23r(i1,i2,i3,ex)) )
                 vta n(i1,i2,i3,ex)=vta(i1,i2,i3,ex)+sigma3*dt*( 1.5*(-
     & vta  (i1,i2,i3,ex) +  u z 23r(i1,i2,i3,ex))-0.5*(-vta m(i1,i2,
     & i3,ex) + um z 23r(i1,i2,i3,ex)) )
                 wta n(i1,i2,i3,ex)=wta(i1,i2,i3,ex)+sigma3*dt*( 1.5*(-
     & wta  (i1,i2,i3,ex)+ u  zz 23r(i1,i2,i3,ex) -vta   z 23r(i1,i2,
     & i3,ex))-0.5*(-wta m(i1,i2,i3,ex)+ um zz 23r(i1,i2,i3,ex) -vta 
     & m z 23r(i1,i2,i3,ex)) )
                 end do
                 end do
                 end do
              else if( side1.eq.0 .and. side2.eq.0 .and. side3.eq.1 )
     & then
                 do i3=m3a,m3b
                 do i2=m2a,m2b
                 do i1=m1a,m1b
                    ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
                    ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
                    xx=(i1-i1a)/real(i1b-i1a)
                    sigma1 = layerStrength*xx**power
                    ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
                    ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
                    yy=(i2-i2a)/real(i2b-i2a)
                    sigma2 = layerStrength*yy**power
                    ! zScale=xy(i1,i2,i3b,2)-xy(i1,i2,i3a,2)
                    ! zz=(xy(i1,i2,i3,2)-xy(i1,i2,i3a,2))/zScale
                    zz=(i3-i3a)/real(i3b-i3a)
                    sigma3 = layerStrength*zz**power
                   un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( u laplacian 23r(i1,i2,i3,ex) - vra x 23r(i1,i2,i3,ex) -
     & wra(i1,i2,i3,ex) - vsa y 23r(i1,i2,i3,ex) -wsa(i1,i2,i3,ex) - 
     & vtb z 23r(i1,i2,i3,ex) -wtb(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vra n(i1,i2,i3,ex)=vra(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -vra(i1,i2,i3,ex) +  u x 23r(i1,i2,i3,ex))-0.5*(-vra m(i1,i2,
     & i3,ex) + um x 23r(i1,i2,i3,ex)) )
                 wra n(i1,i2,i3,ex)=wra(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -wra(i1,i2,i3,ex)+  u xx 23r(i1,i2,i3,ex) - vra   x 23r(i1,i2,
     & i3,ex))-0.5*(-wra m(i1,i2,i3,ex)+ um xx 23r(i1,i2,i3,ex) - vra 
     & m x 23r(i1,i2,i3,ex)) )
                 vsa n(i1,i2,i3,ex)=vsa(i1,i2,i3,ex)+sigma2*dt*( 1.5*(-
     & vsa  (i1,i2,i3,ex) +  u y 23r(i1,i2,i3,ex))-0.5*(-vsa m(i1,i2,
     & i3,ex) + um y 23r(i1,i2,i3,ex)) )
                 wsa n(i1,i2,i3,ex)=wsa(i1,i2,i3,ex)+sigma2*dt*( 1.5*(-
     & wsa  (i1,i2,i3,ex)+ u  yy 23r(i1,i2,i3,ex) -vsa   y 23r(i1,i2,
     & i3,ex))-0.5*(-wsa m(i1,i2,i3,ex)+ um yy 23r(i1,i2,i3,ex) -vsa 
     & m y 23r(i1,i2,i3,ex)) )
                 vtb n(i1,i2,i3,ex)=vtb(i1,i2,i3,ex)+sigma3*dt*( 1.5*(-
     & vtb  (i1,i2,i3,ex) +  u z 23r(i1,i2,i3,ex))-0.5*(-vtb m(i1,i2,
     & i3,ex) + um z 23r(i1,i2,i3,ex)) )
                 wtb n(i1,i2,i3,ex)=wtb(i1,i2,i3,ex)+sigma3*dt*( 1.5*(-
     & wtb  (i1,i2,i3,ex)+ u  zz 23r(i1,i2,i3,ex) -vtb   z 23r(i1,i2,
     & i3,ex))-0.5*(-wtb m(i1,i2,i3,ex)+ um zz 23r(i1,i2,i3,ex) -vtb 
     & m z 23r(i1,i2,i3,ex)) )
                 end do
                 end do
                 end do
              else if( side1.eq.1 .and. side2.eq.0 .and. side3.eq.1 )
     & then
                 do i3=m3a,m3b
                 do i2=m2a,m2b
                 do i1=m1a,m1b
                    ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
                    ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
                    xx=(i1-i1a)/real(i1b-i1a)
                    sigma1 = layerStrength*xx**power
                    ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
                    ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
                    yy=(i2-i2a)/real(i2b-i2a)
                    sigma2 = layerStrength*yy**power
                    ! zScale=xy(i1,i2,i3b,2)-xy(i1,i2,i3a,2)
                    ! zz=(xy(i1,i2,i3,2)-xy(i1,i2,i3a,2))/zScale
                    zz=(i3-i3a)/real(i3b-i3a)
                    sigma3 = layerStrength*zz**power
                   un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( u laplacian 23r(i1,i2,i3,ex) - vrb x 23r(i1,i2,i3,ex) -
     & wrb(i1,i2,i3,ex) - vsa y 23r(i1,i2,i3,ex) -wsa(i1,i2,i3,ex) - 
     & vtb z 23r(i1,i2,i3,ex) -wtb(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vrb n(i1,i2,i3,ex)=vrb(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -vrb(i1,i2,i3,ex) +  u x 23r(i1,i2,i3,ex))-0.5*(-vrb m(i1,i2,
     & i3,ex) + um x 23r(i1,i2,i3,ex)) )
                 wrb n(i1,i2,i3,ex)=wrb(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -wrb(i1,i2,i3,ex)+  u xx 23r(i1,i2,i3,ex) - vrb   x 23r(i1,i2,
     & i3,ex))-0.5*(-wrb m(i1,i2,i3,ex)+ um xx 23r(i1,i2,i3,ex) - vrb 
     & m x 23r(i1,i2,i3,ex)) )
                 vsa n(i1,i2,i3,ex)=vsa(i1,i2,i3,ex)+sigma2*dt*( 1.5*(-
     & vsa  (i1,i2,i3,ex) +  u y 23r(i1,i2,i3,ex))-0.5*(-vsa m(i1,i2,
     & i3,ex) + um y 23r(i1,i2,i3,ex)) )
                 wsa n(i1,i2,i3,ex)=wsa(i1,i2,i3,ex)+sigma2*dt*( 1.5*(-
     & wsa  (i1,i2,i3,ex)+ u  yy 23r(i1,i2,i3,ex) -vsa   y 23r(i1,i2,
     & i3,ex))-0.5*(-wsa m(i1,i2,i3,ex)+ um yy 23r(i1,i2,i3,ex) -vsa 
     & m y 23r(i1,i2,i3,ex)) )
                 vtb n(i1,i2,i3,ex)=vtb(i1,i2,i3,ex)+sigma3*dt*( 1.5*(-
     & vtb  (i1,i2,i3,ex) +  u z 23r(i1,i2,i3,ex))-0.5*(-vtb m(i1,i2,
     & i3,ex) + um z 23r(i1,i2,i3,ex)) )
                 wtb n(i1,i2,i3,ex)=wtb(i1,i2,i3,ex)+sigma3*dt*( 1.5*(-
     & wtb  (i1,i2,i3,ex)+ u  zz 23r(i1,i2,i3,ex) -vtb   z 23r(i1,i2,
     & i3,ex))-0.5*(-wtb m(i1,i2,i3,ex)+ um zz 23r(i1,i2,i3,ex) -vtb 
     & m z 23r(i1,i2,i3,ex)) )
                 end do
                 end do
                 end do
              else if( side1.eq.0 .and. side2.eq.1 .and. side3.eq.1 )
     & then
                 do i3=m3a,m3b
                 do i2=m2a,m2b
                 do i1=m1a,m1b
                    ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
                    ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
                    xx=(i1-i1a)/real(i1b-i1a)
                    sigma1 = layerStrength*xx**power
                    ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
                    ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
                    yy=(i2-i2a)/real(i2b-i2a)
                    sigma2 = layerStrength*yy**power
                    ! zScale=xy(i1,i2,i3b,2)-xy(i1,i2,i3a,2)
                    ! zz=(xy(i1,i2,i3,2)-xy(i1,i2,i3a,2))/zScale
                    zz=(i3-i3a)/real(i3b-i3a)
                    sigma3 = layerStrength*zz**power
                   un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( u laplacian 23r(i1,i2,i3,ex) - vra x 23r(i1,i2,i3,ex) -
     & wra(i1,i2,i3,ex) - vsb y 23r(i1,i2,i3,ex) -wsb(i1,i2,i3,ex) - 
     & vtb z 23r(i1,i2,i3,ex) -wtb(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vra n(i1,i2,i3,ex)=vra(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -vra(i1,i2,i3,ex) +  u x 23r(i1,i2,i3,ex))-0.5*(-vra m(i1,i2,
     & i3,ex) + um x 23r(i1,i2,i3,ex)) )
                 wra n(i1,i2,i3,ex)=wra(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -wra(i1,i2,i3,ex)+  u xx 23r(i1,i2,i3,ex) - vra   x 23r(i1,i2,
     & i3,ex))-0.5*(-wra m(i1,i2,i3,ex)+ um xx 23r(i1,i2,i3,ex) - vra 
     & m x 23r(i1,i2,i3,ex)) )
                 vsb n(i1,i2,i3,ex)=vsb(i1,i2,i3,ex)+sigma2*dt*( 1.5*(-
     & vsb  (i1,i2,i3,ex) +  u y 23r(i1,i2,i3,ex))-0.5*(-vsb m(i1,i2,
     & i3,ex) + um y 23r(i1,i2,i3,ex)) )
                 wsb n(i1,i2,i3,ex)=wsb(i1,i2,i3,ex)+sigma2*dt*( 1.5*(-
     & wsb  (i1,i2,i3,ex)+ u  yy 23r(i1,i2,i3,ex) -vsb   y 23r(i1,i2,
     & i3,ex))-0.5*(-wsb m(i1,i2,i3,ex)+ um yy 23r(i1,i2,i3,ex) -vsb 
     & m y 23r(i1,i2,i3,ex)) )
                 vtb n(i1,i2,i3,ex)=vtb(i1,i2,i3,ex)+sigma3*dt*( 1.5*(-
     & vtb  (i1,i2,i3,ex) +  u z 23r(i1,i2,i3,ex))-0.5*(-vtb m(i1,i2,
     & i3,ex) + um z 23r(i1,i2,i3,ex)) )
                 wtb n(i1,i2,i3,ex)=wtb(i1,i2,i3,ex)+sigma3*dt*( 1.5*(-
     & wtb  (i1,i2,i3,ex)+ u  zz 23r(i1,i2,i3,ex) -vtb   z 23r(i1,i2,
     & i3,ex))-0.5*(-wtb m(i1,i2,i3,ex)+ um zz 23r(i1,i2,i3,ex) -vtb 
     & m z 23r(i1,i2,i3,ex)) )
                 end do
                 end do
                 end do
              else if( side1.eq.1 .and. side2.eq.1 .and. side3.eq.1 )
     & then
                 do i3=m3a,m3b
                 do i2=m2a,m2b
                 do i1=m1a,m1b
                    ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
                    ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
                    xx=(i1-i1a)/real(i1b-i1a)
                    sigma1 = layerStrength*xx**power
                    ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
                    ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
                    yy=(i2-i2a)/real(i2b-i2a)
                    sigma2 = layerStrength*yy**power
                    ! zScale=xy(i1,i2,i3b,2)-xy(i1,i2,i3a,2)
                    ! zz=(xy(i1,i2,i3,2)-xy(i1,i2,i3a,2))/zScale
                    zz=(i3-i3a)/real(i3b-i3a)
                    sigma3 = layerStrength*zz**power
                   un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( u laplacian 23r(i1,i2,i3,ex) - vrb x 23r(i1,i2,i3,ex) -
     & wrb(i1,i2,i3,ex) - vsb y 23r(i1,i2,i3,ex) -wsb(i1,i2,i3,ex) - 
     & vtb z 23r(i1,i2,i3,ex) -wtb(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vrb n(i1,i2,i3,ex)=vrb(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -vrb(i1,i2,i3,ex) +  u x 23r(i1,i2,i3,ex))-0.5*(-vrb m(i1,i2,
     & i3,ex) + um x 23r(i1,i2,i3,ex)) )
                 wrb n(i1,i2,i3,ex)=wrb(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -wrb(i1,i2,i3,ex)+  u xx 23r(i1,i2,i3,ex) - vrb   x 23r(i1,i2,
     & i3,ex))-0.5*(-wrb m(i1,i2,i3,ex)+ um xx 23r(i1,i2,i3,ex) - vrb 
     & m x 23r(i1,i2,i3,ex)) )
                 vsb n(i1,i2,i3,ex)=vsb(i1,i2,i3,ex)+sigma2*dt*( 1.5*(-
     & vsb  (i1,i2,i3,ex) +  u y 23r(i1,i2,i3,ex))-0.5*(-vsb m(i1,i2,
     & i3,ex) + um y 23r(i1,i2,i3,ex)) )
                 wsb n(i1,i2,i3,ex)=wsb(i1,i2,i3,ex)+sigma2*dt*( 1.5*(-
     & wsb  (i1,i2,i3,ex)+ u  yy 23r(i1,i2,i3,ex) -vsb   y 23r(i1,i2,
     & i3,ex))-0.5*(-wsb m(i1,i2,i3,ex)+ um yy 23r(i1,i2,i3,ex) -vsb 
     & m y 23r(i1,i2,i3,ex)) )
                 vtb n(i1,i2,i3,ex)=vtb(i1,i2,i3,ex)+sigma3*dt*( 1.5*(-
     & vtb  (i1,i2,i3,ex) +  u z 23r(i1,i2,i3,ex))-0.5*(-vtb m(i1,i2,
     & i3,ex) + um z 23r(i1,i2,i3,ex)) )
                 wtb n(i1,i2,i3,ex)=wtb(i1,i2,i3,ex)+sigma3*dt*( 1.5*(-
     & wtb  (i1,i2,i3,ex)+ u  zz 23r(i1,i2,i3,ex) -vtb   z 23r(i1,i2,
     & i3,ex))-0.5*(-wtb m(i1,i2,i3,ex)+ um zz 23r(i1,i2,i3,ex) -vtb 
     & m z 23r(i1,i2,i3,ex)) )
                 end do
                 end do
                 end do
              else
                stop 6224
              end if
             else
               stop 23415
             end if

         end if

        end if

       else if( gridType.eq.rectangular .and. orderOfAccuracy.eq.4 )
     & then

        ! ***********************************************
        ! ************rectangular grid*******************
        ! ************ fourth-order   *******************
        ! ***********************************************

        if( nd.eq.2 )then
         ! *********************************************************************
         ! **************** Two Dimensions, Order 4 ****************************
         ! *********************************************************************

         if( boxType.eq.xSide )then

          if( side1.eq.0 )then
           do i3=m3a,m3b
           do i2=m2a,m2b
           do i1=m1a,m1b
             ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
             ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
             xx=(i1-i1a)/real(i1b-i1a)
             sigma1 = layerStrength*xx**power
             sigma1x = (2*side1-1)*power*layerStrength*xx**(power-1)
            ! update4xNew(ex,fullUpdate,2)
             ! (u(n+1) - 2*u + u(n-1))/dt^2 = u_tt + (dt^4/12)*u_tttt + ...
             !
             ! [u(n)-u(n-1)]/dt = u_t + (dt/2)*u_tt + O(dt^2)
             !
             ! u_tt = Delta u - v_x - w
             ! u_tttt = Delta u_tt - v_xtt - wtt
             ! 
             v=vra (i1,i2,i3,ex)
             vx  = vra x42r(i1,i2,i3,ex)
             vxx = vra xx42r(i1,i2,i3,ex)
             vxxx= vra xxx22r(i1,i2,i3,ex)
             vxyy= vra xyy22r(i1,i2,i3,ex)
             w=wra(i1,i2,i3,ex)
             wx  = wra x42r(i1,i2,i3,ex)
             wxx = wra xx42r(i1,i2,i3,ex)
             ux= ux42r(i1,i2,i3,ex)
             uxx= uxx42r(i1,i2,i3,ex)
             uxxx=uxxx22r(i1,i2,i3,ex)
             uxyy=uxyy22r(i1,i2,i3,ex)
             uxxxx=uxxxx22r(i1,i2,i3,ex)
             uxxyy=uxxyy22r(i1,i2,i3,ex)
             uLap = uLaplacian42r(i1,i2,i3,ex)
             ! --- these change in 3D ---
               uyyyy=uyyyy22r(i1,i2,i3,ex)
               uLapSq=uxxxx +2.*uxxyy +uyyyy
               uLapx = uxxx+uxyy
               uLapxx= uxxxx+uxxyy
               vLapx=vxxx+vxyy
             ut = (u(i1,i2,i3,ex)-um(i1,i2,i3,ex))/dt - (.5*dt*csq)*( 
     & uLap - vx - w )
             uxt = ( ux-umx42r(i1,i2,i3,ex))/dt  - (.5*dt*csq)*( uLapx 
     & - vxx - wx )
             uxxt= (uxx-umxx42r(i1,i2,i3,ex))/dt - (.5*dt*csq)*( 
     & uLapxx - vxxx - wxx )
             ! *** uxxxt= (uxxx-umxxx42r(i1,i2,i3,ex))/dt   ! only need to first order in dt
             ! *** uxyyt= (uxyy-umxyy42r(i1,i2,i3,ex))/dt   ! only need to first order in dt
             ! *** uxxxxt= (uxxxx-umxxxx42r(i1,i2,i3,ex))/dt   ! only need to first order in dt
             ! *** uxxyyt= (uxxyy-umxxyy42r(i1,i2,i3,ex))/dt   ! only need to first order in dt
             vt = sigma1*( -v + ux )
             vxt = sigma1*( -vx + uxx ) + sigma1x*( -v + ux )
             vxtt = sigma1*( -vxt + uxxt ) + sigma1x*( -vt + uxt )
             wt =  sigma1*( -w -vx + uxx )
             wtt = sigma1*( -wt -vxt + uxxt )
               un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( uLap - vx -w ) + cdt4Over12*( uLapSq - vLapx - wra 
     & Laplacian42r(i1,i2,i3,ex)  - vxtt - wtt )
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
             ! vra n(i1,i2,i3,ex)=vra ex(i1,i2,i3,ex)+(2.*dt)*( vt + (dt**2/6.)*vttt )
             vra n(i1,i2,i3,ex)=vra(i1,i2,i3,ex)+(dt)*( vt + dt*( .5*
     & vtt + dt*( (1./6.)*vttt + dt*( (1./24.)*vtttt ) ) ) )
             ! write(*,'(" i1,i2=",2i3," vt,vtt,vttt=",3e10.2," v,ux,uxt,uxtt=",4e10.2)') i1,i2,vt,vtt,vttt,v,ux,uxt,uxtt
             ! w_t = sigma1*( -w -vx + uxx )
             wttt = sigma1*( -wtt -vxtt + uxxtt )
             wtttt = 0. ! **** sigma1*( -wttt -vxttt + uxxttt )
            ! wan(i1,i2,i3,ex)=wam(i1,i2,i3,ex)+(2.*dt)*( wt + (dt**2/6.)*wttt )
             wra n(i1,i2,i3,ex)=wra(i1,i2,i3,ex)+(dt)*(  wt + dt*( .5*
     & wtt + dt*( (1./6.)*wttt + dt*( (1./24.)*wtttt ) ) ) )
           end do
           end do
           end do
          else
           do i3=m3a,m3b
           do i2=m2a,m2b
           do i1=m1a,m1b
             ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
             ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
             xx=(i1-i1a)/real(i1b-i1a)
             sigma1 = layerStrength*xx**power
             sigma1x = (2*side1-1)*power*layerStrength*xx**(power-1)
             ! (u(n+1) - 2*u + u(n-1))/dt^2 = u_tt + (dt^4/12)*u_tttt + ...
             !
             ! [u(n)-u(n-1)]/dt = u_t + (dt/2)*u_tt + O(dt^2)
             !
             ! u_tt = Delta u - v_x - w
             ! u_tttt = Delta u_tt - v_xtt - wtt
             ! 
             v=vrb (i1,i2,i3,ex)
             vx  = vrb x42r(i1,i2,i3,ex)
             vxx = vrb xx42r(i1,i2,i3,ex)
             vxxx= vrb xxx22r(i1,i2,i3,ex)
             vxyy= vrb xyy22r(i1,i2,i3,ex)
             w=wrb(i1,i2,i3,ex)
             wx  = wrb x42r(i1,i2,i3,ex)
             wxx = wrb xx42r(i1,i2,i3,ex)
             ux= ux42r(i1,i2,i3,ex)
             uxx= uxx42r(i1,i2,i3,ex)
             uxxx=uxxx22r(i1,i2,i3,ex)
             uxyy=uxyy22r(i1,i2,i3,ex)
             uxxxx=uxxxx22r(i1,i2,i3,ex)
             uxxyy=uxxyy22r(i1,i2,i3,ex)
             uLap = uLaplacian42r(i1,i2,i3,ex)
             ! --- these change in 3D ---
               uyyyy=uyyyy22r(i1,i2,i3,ex)
               uLapSq=uxxxx +2.*uxxyy +uyyyy
               uLapx = uxxx+uxyy
               uLapxx= uxxxx+uxxyy
               vLapx=vxxx+vxyy
             ut = (u(i1,i2,i3,ex)-um(i1,i2,i3,ex))/dt - (.5*dt*csq)*( 
     & uLap - vx - w )
             uxt = ( ux-umx42r(i1,i2,i3,ex))/dt  - (.5*dt*csq)*( uLapx 
     & - vxx - wx )
             uxxt= (uxx-umxx42r(i1,i2,i3,ex))/dt - (.5*dt*csq)*( 
     & uLapxx - vxxx - wxx )
             ! *** uxxxt= (uxxx-umxxx42r(i1,i2,i3,ex))/dt   ! only need to first order in dt
             ! *** uxyyt= (uxyy-umxyy42r(i1,i2,i3,ex))/dt   ! only need to first order in dt
             ! *** uxxxxt= (uxxxx-umxxxx42r(i1,i2,i3,ex))/dt   ! only need to first order in dt
             ! *** uxxyyt= (uxxyy-umxxyy42r(i1,i2,i3,ex))/dt   ! only need to first order in dt
             vt = sigma1*( -v + ux )
             vxt = sigma1*( -vx + uxx ) + sigma1x*( -v + ux )
             vxtt = sigma1*( -vxt + uxxt ) + sigma1x*( -vt + uxt )
             wt =  sigma1*( -w -vx + uxx )
             wtt = sigma1*( -wt -vxt + uxxt )
               un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( uLap - vx -w ) + cdt4Over12*( uLapSq - vLapx - wrb 
     & Laplacian42r(i1,i2,i3,ex)  - vxtt - wtt )
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
             ! vrb n(i1,i2,i3,ex)=vrb ex(i1,i2,i3,ex)+(2.*dt)*( vt + (dt**2/6.)*vttt )
             vrb n(i1,i2,i3,ex)=vrb(i1,i2,i3,ex)+(dt)*( vt + dt*( .5*
     & vtt + dt*( (1./6.)*vttt + dt*( (1./24.)*vtttt ) ) ) )
             ! write(*,'(" i1,i2=",2i3," vt,vtt,vttt=",3e10.2," v,ux,uxt,uxtt=",4e10.2)') i1,i2,vt,vtt,vttt,v,ux,uxt,uxtt
             ! w_t = sigma1*( -w -vx + uxx )
             wttt = sigma1*( -wtt -vxtt + uxxtt )
             wtttt = 0. ! **** sigma1*( -wttt -vxttt + uxxttt )
            ! wan(i1,i2,i3,ex)=wam(i1,i2,i3,ex)+(2.*dt)*( wt + (dt**2/6.)*wttt )
             wrb n(i1,i2,i3,ex)=wrb(i1,i2,i3,ex)+(dt)*(  wt + dt*( .5*
     & wtt + dt*( (1./6.)*wttt + dt*( (1./24.)*wtttt ) ) ) )
           end do
           end do
           end do
          end if

         else if( boxType.eq.ySide )then

          if( side2.eq.0 )then
           do i3=m3a,m3b
           do i2=m2a,m2b
           do i1=m1a,m1b
             ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
             ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
             yy=(i2-i2a)/real(i2b-i2a)
             sigma2 = layerStrength*yy**power
             sigma2y = (2*side2-1)*power*layerStrength*yy**(power-1)
             ! (u(n+1) - 2*u + u(n-1))/dt^2 = u_tt + (dt^4/12)*u_tttt + ...
             !
             ! [u(n)-u(n-1)]/dt = u_t + (dt/2)*u_tt + O(dt^2)
             !
             ! u_tt = Delta u - v_y - w
             ! u_tttt = Delta u_tt - v_ytt - wtt
             ! 
             v=vsa (i1,i2,i3,ex)
             vy  = vsa y42r(i1,i2,i3,ex)
             vyy = vsa yy42r(i1,i2,i3,ex)
             vyyy= vsa yyy22r(i1,i2,i3,ex)
             vxxy= vsa xxy22r(i1,i2,i3,ex)
             w=wsa(i1,i2,i3,ex)
             wy  = wsa y42r(i1,i2,i3,ex)
             wyy = wsa yy42r(i1,i2,i3,ex)
             uy= uy42r(i1,i2,i3,ex)
             uyy= uyy42r(i1,i2,i3,ex)
             uyyy=uyyy22r(i1,i2,i3,ex)
             uxxy=uxxy22r(i1,i2,i3,ex)
             uyyyy=uyyyy22r(i1,i2,i3,ex)
             uxxyy=uxxyy22r(i1,i2,i3,ex)
             uLap = uLaplacian42r(i1,i2,i3,ex)
             ! --- these change in 3D ---
               uxxxx=uxxxx22r(i1,i2,i3,ex)
               uLapSq=uyyyy +2.*uxxyy +uxxxx
               uLapy = uyyy+uxxy
               uLapyy= uyyyy+uxxyy
               vLapy=vyyy+vxxy
             ut = (u(i1,i2,i3,ex)-um(i1,i2,i3,ex))/dt - (.5*dt*csq)*( 
     & uLap - vy - w )
             uyt = ( uy-umy42r(i1,i2,i3,ex))/dt  - (.5*dt*csq)*( uLapy 
     & - vyy - wy )
             uyyt= (uyy-umyy42r(i1,i2,i3,ex))/dt - (.5*dt*csq)*( 
     & uLapyy - vyyy - wyy )
             ! *** uyyyt= (uyyy-umyyy42r(i1,i2,i3,ex))/dt   ! onlx need to first order in dt
             ! *** uxxyt= (uxxy-umxxy42r(i1,i2,i3,ex))/dt   ! onlx need to first order in dt
             ! *** uyyyyt= (uyyyy-umyyyy42r(i1,i2,i3,ex))/dt   ! onlx need to first order in dt
             ! *** uxxyyt= (uxxyy-umxxyy42r(i1,i2,i3,ex))/dt   ! onlx need to first order in dt
             vt = sigma2*( -v + uy )
             vyt = sigma2*( -vy + uyy ) + sigma2y*( -v + uy )
             vytt = sigma2*( -vyt + uyyt ) + sigma2y*( -vt + uyt )
             wt =  sigma2*( -w -vy + uyy )
             wtt = sigma2*( -wt -vyt + uyyt )
               un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( uLap - vy -w ) + cdt4Over12*( uLapSq - vLapy - wsa 
     & Laplacian42r(i1,i2,i3,ex)  - vytt - wtt )
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
             ! vsa n(i1,i2,i3,ex)=vsa ex(i1,i2,i3,ex)+(2.*dt)*( vt + (dt**2/6.)*vttt )
             vsa n(i1,i2,i3,ex)=vsa(i1,i2,i3,ex)+(dt)*( vt + dt*( .5*
     & vtt + dt*( (1./6.)*vttt + dt*( (1./24.)*vtttt ) ) ) )
             ! write(*,'(" i1,i2=",2i3," vt,vtt,vttt=",3e10.2," v,uy,uyt,uytt=",4e10.2)') i1,i2,vt,vtt,vttt,v,uy,uyt,uytt
             ! w_t = sigma2*( -w -vy + uyy )
             wttt = sigma2*( -wtt -vytt + uyytt )
             wtttt = 0. ! **** sigma2*( -wttt -vyttt + uyyttt )
            ! wan(i1,i2,i3,ex)=wam(i1,i2,i3,ex)+(2.*dt)*( wt + (dt**2/6.)*wttt )
             wsa n(i1,i2,i3,ex)=wsa(i1,i2,i3,ex)+(dt)*(  wt + dt*( .5*
     & wtt + dt*( (1./6.)*wttt + dt*( (1./24.)*wtttt ) ) ) )
           end do
           end do
           end do
          else
           do i3=m3a,m3b
           do i2=m2a,m2b
           do i1=m1a,m1b
             ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
             ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
             yy=(i2-i2a)/real(i2b-i2a)
             sigma2 = layerStrength*yy**power
             sigma2y = (2*side2-1)*power*layerStrength*yy**(power-1)
             ! (u(n+1) - 2*u + u(n-1))/dt^2 = u_tt + (dt^4/12)*u_tttt + ...
             !
             ! [u(n)-u(n-1)]/dt = u_t + (dt/2)*u_tt + O(dt^2)
             !
             ! u_tt = Delta u - v_y - w
             ! u_tttt = Delta u_tt - v_ytt - wtt
             ! 
             v=vsb (i1,i2,i3,ex)
             vy  = vsb y42r(i1,i2,i3,ex)
             vyy = vsb yy42r(i1,i2,i3,ex)
             vyyy= vsb yyy22r(i1,i2,i3,ex)
             vxxy= vsb xxy22r(i1,i2,i3,ex)
             w=wsb(i1,i2,i3,ex)
             wy  = wsb y42r(i1,i2,i3,ex)
             wyy = wsb yy42r(i1,i2,i3,ex)
             uy= uy42r(i1,i2,i3,ex)
             uyy= uyy42r(i1,i2,i3,ex)
             uyyy=uyyy22r(i1,i2,i3,ex)
             uxxy=uxxy22r(i1,i2,i3,ex)
             uyyyy=uyyyy22r(i1,i2,i3,ex)
             uxxyy=uxxyy22r(i1,i2,i3,ex)
             uLap = uLaplacian42r(i1,i2,i3,ex)
             ! --- these change in 3D ---
               uxxxx=uxxxx22r(i1,i2,i3,ex)
               uLapSq=uyyyy +2.*uxxyy +uxxxx
               uLapy = uyyy+uxxy
               uLapyy= uyyyy+uxxyy
               vLapy=vyyy+vxxy
             ut = (u(i1,i2,i3,ex)-um(i1,i2,i3,ex))/dt - (.5*dt*csq)*( 
     & uLap - vy - w )
             uyt = ( uy-umy42r(i1,i2,i3,ex))/dt  - (.5*dt*csq)*( uLapy 
     & - vyy - wy )
             uyyt= (uyy-umyy42r(i1,i2,i3,ex))/dt - (.5*dt*csq)*( 
     & uLapyy - vyyy - wyy )
             ! *** uyyyt= (uyyy-umyyy42r(i1,i2,i3,ex))/dt   ! onlx need to first order in dt
             ! *** uxxyt= (uxxy-umxxy42r(i1,i2,i3,ex))/dt   ! onlx need to first order in dt
             ! *** uyyyyt= (uyyyy-umyyyy42r(i1,i2,i3,ex))/dt   ! onlx need to first order in dt
             ! *** uxxyyt= (uxxyy-umxxyy42r(i1,i2,i3,ex))/dt   ! onlx need to first order in dt
             vt = sigma2*( -v + uy )
             vyt = sigma2*( -vy + uyy ) + sigma2y*( -v + uy )
             vytt = sigma2*( -vyt + uyyt ) + sigma2y*( -vt + uyt )
             wt =  sigma2*( -w -vy + uyy )
             wtt = sigma2*( -wt -vyt + uyyt )
               un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( uLap - vy -w ) + cdt4Over12*( uLapSq - vLapy - wsb 
     & Laplacian42r(i1,i2,i3,ex)  - vytt - wtt )
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
             ! vsb n(i1,i2,i3,ex)=vsb ex(i1,i2,i3,ex)+(2.*dt)*( vt + (dt**2/6.)*vttt )
             vsb n(i1,i2,i3,ex)=vsb(i1,i2,i3,ex)+(dt)*( vt + dt*( .5*
     & vtt + dt*( (1./6.)*vttt + dt*( (1./24.)*vtttt ) ) ) )
             ! write(*,'(" i1,i2=",2i3," vt,vtt,vttt=",3e10.2," v,uy,uyt,uytt=",4e10.2)') i1,i2,vt,vtt,vttt,v,uy,uyt,uytt
             ! w_t = sigma2*( -w -vy + uyy )
             wttt = sigma2*( -wtt -vytt + uyytt )
             wtttt = 0. ! **** sigma2*( -wttt -vyttt + uyyttt )
            ! wan(i1,i2,i3,ex)=wam(i1,i2,i3,ex)+(2.*dt)*( wt + (dt**2/6.)*wttt )
             wsb n(i1,i2,i3,ex)=wsb(i1,i2,i3,ex)+(dt)*(  wt + dt*( .5*
     & wtt + dt*( (1./6.)*wttt + dt*( (1./24.)*wtttt ) ) ) )
           end do
           end do
           end do
          end if

         else

           ! macro to advance corner regions:
           ! -- Use second-order version for now:
             if( boxType.eq.xyEdge )then
              if( side1.eq.0 .and. side2.eq.0 )then
               do i3=m3a,m3b
               do i2=m2a,m2b
               do i1=m1a,m1b
                 ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
                 ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
                 xx=(i1-i1a)/real(i1b-i1a)
                 sigma1 = layerStrength*xx**power
                 ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
                 ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
                 yy=(i2-i2a)/real(i2b-i2a)
                 sigma2 = layerStrength*yy**power
                 un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( ulaplacian22r(i1,i2,i3,ex) - vra x22r(i1,i2,i3,ex) -
     & wra(i1,i2,i3,ex) - vsa y22r(i1,i2,i3,ex) -wsa(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vra n(i1,i2,i3,ex)=vra(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -vra(i1,i2,i3,ex) +  ux22r(i1,i2,i3,ex))-0.5*(-vra m(i1,i2,i3,
     & ex) + umx22r(i1,i2,i3,ex)) )
                 wra n(i1,i2,i3,ex)=wra(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -wra(i1,i2,i3,ex)+  uxx22r(i1,i2,i3,ex) - vra x22r(i1,i2,i3,
     & ex))-0.5*(-wra m(i1,i2,i3,ex)+ umxx22r(i1,i2,i3,ex)-vra m x22r(
     & i1,i2,i3,ex)) )
                 vsa n(i1,i2,i3,ex)=vsa(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     &  -vsa(i1,i2,i3,ex) +  uy22r(i1,i2,i3,ex))-0.5*(-vsa m(i1,i2,i3,
     & ex) + umy22r(i1,i2,i3,ex)) )
                 wsa n(i1,i2,i3,ex)=wsa(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     & -wsa(i1,i2,i3,ex)+  uyy22r(i1,i2,i3,ex)   -vsa y22r(i1,i2,i3,
     & ex))-0.5*(-wsa m(i1,i2,i3,ex)+ umyy22r(i1,i2,i3,ex)-vsa m y22r(
     & i1,i2,i3,ex)) )
               end do
               end do
               end do
              else if( side1.eq.1 .and. side2.eq.0 )then
               do i3=m3a,m3b
               do i2=m2a,m2b
               do i1=m1a,m1b
                 ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
                 ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
                 xx=(i1-i1a)/real(i1b-i1a)
                 sigma1 = layerStrength*xx**power
                 ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
                 ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
                 yy=(i2-i2a)/real(i2b-i2a)
                 sigma2 = layerStrength*yy**power
                 un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( ulaplacian22r(i1,i2,i3,ex) - vrb x22r(i1,i2,i3,ex) -
     & wrb(i1,i2,i3,ex) - vsa y22r(i1,i2,i3,ex) -wsa(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vrb n(i1,i2,i3,ex)=vrb(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -vrb(i1,i2,i3,ex) +  ux22r(i1,i2,i3,ex))-0.5*(-vrb m(i1,i2,i3,
     & ex) + umx22r(i1,i2,i3,ex)) )
                 wrb n(i1,i2,i3,ex)=wrb(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -wrb(i1,i2,i3,ex)+  uxx22r(i1,i2,i3,ex) - vrb x22r(i1,i2,i3,
     & ex))-0.5*(-wrb m(i1,i2,i3,ex)+ umxx22r(i1,i2,i3,ex)-vrb m x22r(
     & i1,i2,i3,ex)) )
                 vsa n(i1,i2,i3,ex)=vsa(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     &  -vsa(i1,i2,i3,ex) +  uy22r(i1,i2,i3,ex))-0.5*(-vsa m(i1,i2,i3,
     & ex) + umy22r(i1,i2,i3,ex)) )
                 wsa n(i1,i2,i3,ex)=wsa(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     & -wsa(i1,i2,i3,ex)+  uyy22r(i1,i2,i3,ex)   -vsa y22r(i1,i2,i3,
     & ex))-0.5*(-wsa m(i1,i2,i3,ex)+ umyy22r(i1,i2,i3,ex)-vsa m y22r(
     & i1,i2,i3,ex)) )
               end do
               end do
               end do
              else if( side1.eq.0 .and. side2.eq.1 )then
               do i3=m3a,m3b
               do i2=m2a,m2b
               do i1=m1a,m1b
                 ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
                 ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
                 xx=(i1-i1a)/real(i1b-i1a)
                 sigma1 = layerStrength*xx**power
                 ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
                 ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
                 yy=(i2-i2a)/real(i2b-i2a)
                 sigma2 = layerStrength*yy**power
                 un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( ulaplacian22r(i1,i2,i3,ex) - vra x22r(i1,i2,i3,ex) -
     & wra(i1,i2,i3,ex) - vsb y22r(i1,i2,i3,ex) -wsb(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vra n(i1,i2,i3,ex)=vra(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -vra(i1,i2,i3,ex) +  ux22r(i1,i2,i3,ex))-0.5*(-vra m(i1,i2,i3,
     & ex) + umx22r(i1,i2,i3,ex)) )
                 wra n(i1,i2,i3,ex)=wra(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -wra(i1,i2,i3,ex)+  uxx22r(i1,i2,i3,ex) - vra x22r(i1,i2,i3,
     & ex))-0.5*(-wra m(i1,i2,i3,ex)+ umxx22r(i1,i2,i3,ex)-vra m x22r(
     & i1,i2,i3,ex)) )
                 vsb n(i1,i2,i3,ex)=vsb(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     &  -vsb(i1,i2,i3,ex) +  uy22r(i1,i2,i3,ex))-0.5*(-vsb m(i1,i2,i3,
     & ex) + umy22r(i1,i2,i3,ex)) )
                 wsb n(i1,i2,i3,ex)=wsb(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     & -wsb(i1,i2,i3,ex)+  uyy22r(i1,i2,i3,ex)   -vsb y22r(i1,i2,i3,
     & ex))-0.5*(-wsb m(i1,i2,i3,ex)+ umyy22r(i1,i2,i3,ex)-vsb m y22r(
     & i1,i2,i3,ex)) )
               end do
               end do
               end do
              else if( side1.eq.1 .and. side2.eq.1 )then
               do i3=m3a,m3b
               do i2=m2a,m2b
               do i1=m1a,m1b
                 ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
                 ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
                 xx=(i1-i1a)/real(i1b-i1a)
                 sigma1 = layerStrength*xx**power
                 ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
                 ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
                 yy=(i2-i2a)/real(i2b-i2a)
                 sigma2 = layerStrength*yy**power
                 un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( ulaplacian22r(i1,i2,i3,ex) - vrb x22r(i1,i2,i3,ex) -
     & wrb(i1,i2,i3,ex) - vsb y22r(i1,i2,i3,ex) -wsb(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vrb n(i1,i2,i3,ex)=vrb(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -vrb(i1,i2,i3,ex) +  ux22r(i1,i2,i3,ex))-0.5*(-vrb m(i1,i2,i3,
     & ex) + umx22r(i1,i2,i3,ex)) )
                 wrb n(i1,i2,i3,ex)=wrb(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -wrb(i1,i2,i3,ex)+  uxx22r(i1,i2,i3,ex) - vrb x22r(i1,i2,i3,
     & ex))-0.5*(-wrb m(i1,i2,i3,ex)+ umxx22r(i1,i2,i3,ex)-vrb m x22r(
     & i1,i2,i3,ex)) )
                 vsb n(i1,i2,i3,ex)=vsb(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     &  -vsb(i1,i2,i3,ex) +  uy22r(i1,i2,i3,ex))-0.5*(-vsb m(i1,i2,i3,
     & ex) + umy22r(i1,i2,i3,ex)) )
                 wsb n(i1,i2,i3,ex)=wsb(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     & -wsb(i1,i2,i3,ex)+  uyy22r(i1,i2,i3,ex)   -vsb y22r(i1,i2,i3,
     & ex))-0.5*(-wsb m(i1,i2,i3,ex)+ umyy22r(i1,i2,i3,ex)-vsb m y22r(
     & i1,i2,i3,ex)) )
               end do
               end do
               end do
              else
                stop 66244
              end if
             else
               stop 23415
             end if

           ! Trouble with the fourth-order version: far corners go unstable
           ! advanceCorners2dOrder4()

         end if

        else if( nd.eq.3 )then
         ! *********************************************************************
         ! **************** Three Dimensions, Order 4 ****************************
         ! *********************************************************************

         if( boxType.eq.xSide )then

          if( side1.eq.0 )then
           do i3=m3a,m3b
           do i2=m2a,m2b
           do i1=m1a,m1b
             ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
             ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
             xx=(i1-i1a)/real(i1b-i1a)
             sigma1 = layerStrength*xx**power
             sigma1x = (2*side1-1)*power*layerStrength*xx**(power-1)
             ! (u(n+1) - 2*u + u(n-1))/dt^2 = u_tt + (dt^4/12)*u_tttt + ...
             !
             ! [u(n)-u(n-1)]/dt = u_t + (dt/2)*u_tt + O(dt^2)
             !
             ! u_tt = Delta u - v_x - w
             ! u_tttt = Delta u_tt - v_xtt - wtt
             ! 
             v=vra (i1,i2,i3,ex)
             vx  = vra x43r(i1,i2,i3,ex)
             vxx = vra xx43r(i1,i2,i3,ex)
             vxxx= vra xxx23r(i1,i2,i3,ex)
             vxyy= vra xyy23r(i1,i2,i3,ex)
             w=wra(i1,i2,i3,ex)
             wx  = wra x43r(i1,i2,i3,ex)
             wxx = wra xx43r(i1,i2,i3,ex)
             ux= ux43r(i1,i2,i3,ex)
             uxx= uxx43r(i1,i2,i3,ex)
             uxxx=uxxx23r(i1,i2,i3,ex)
             uxyy=uxyy23r(i1,i2,i3,ex)
             uxxxx=uxxxx23r(i1,i2,i3,ex)
             uxxyy=uxxyy23r(i1,i2,i3,ex)
             uLap = uLaplacian43r(i1,i2,i3,ex)
             ! --- these change in 3D ---
               uLapSq=uLapSq23r(i1,i2,i3,ex)
               uxzz=uxzz23r(i1,i2,i3,ex)
               uxxzz=uxzz23r(i1,i2,i3,ex)
               vxzz= vra xzz23r(i1,i2,i3,ex)
               uLapx = uxxx+uxyy+uxzz
               uLapxx= uxxxx+uxxyy+uxxzz
               vLapx=vxxx+vxyy+vxzz
             ut = (u(i1,i2,i3,ex)-um(i1,i2,i3,ex))/dt - (.5*dt*csq)*( 
     & uLap - vx - w )
             uxt = ( ux-umx43r(i1,i2,i3,ex))/dt  - (.5*dt*csq)*( uLapx 
     & - vxx - wx )
             uxxt= (uxx-umxx43r(i1,i2,i3,ex))/dt - (.5*dt*csq)*( 
     & uLapxx - vxxx - wxx )
             ! *** uxxxt= (uxxx-umxxx43r(i1,i2,i3,ex))/dt   ! only need to first order in dt
             ! *** uxyyt= (uxyy-umxyy43r(i1,i2,i3,ex))/dt   ! only need to first order in dt
             ! *** uxxxxt= (uxxxx-umxxxx43r(i1,i2,i3,ex))/dt   ! only need to first order in dt
             ! *** uxxyyt= (uxxyy-umxxyy43r(i1,i2,i3,ex))/dt   ! only need to first order in dt
             vt = sigma1*( -v + ux )
             vxt = sigma1*( -vx + uxx ) + sigma1x*( -v + ux )
             vxtt = sigma1*( -vxt + uxxt ) + sigma1x*( -vt + uxt )
             wt =  sigma1*( -w -vx + uxx )
             wtt = sigma1*( -wt -vxt + uxxt )
               un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( uLap - vx -w ) + cdt4Over12*( uLapSq - vLapx - wra 
     & Laplacian43r(i1,i2,i3,ex)  - vxtt - wtt )
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
             ! vra n(i1,i2,i3,ex)=vra ex(i1,i2,i3,ex)+(2.*dt)*( vt + (dt**2/6.)*vttt )
             vra n(i1,i2,i3,ex)=vra(i1,i2,i3,ex)+(dt)*( vt + dt*( .5*
     & vtt + dt*( (1./6.)*vttt + dt*( (1./24.)*vtttt ) ) ) )
             ! write(*,'(" i1,i2=",2i3," vt,vtt,vttt=",3e10.2," v,ux,uxt,uxtt=",4e10.2)') i1,i2,vt,vtt,vttt,v,ux,uxt,uxtt
             ! w_t = sigma1*( -w -vx + uxx )
             wttt = sigma1*( -wtt -vxtt + uxxtt )
             wtttt = 0. ! **** sigma1*( -wttt -vxttt + uxxttt )
            ! wan(i1,i2,i3,ex)=wam(i1,i2,i3,ex)+(2.*dt)*( wt + (dt**2/6.)*wttt )
             wra n(i1,i2,i3,ex)=wra(i1,i2,i3,ex)+(dt)*(  wt + dt*( .5*
     & wtt + dt*( (1./6.)*wttt + dt*( (1./24.)*wtttt ) ) ) )
           end do
           end do
           end do
          else
           do i3=m3a,m3b
           do i2=m2a,m2b
           do i1=m1a,m1b
             ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
             ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
             xx=(i1-i1a)/real(i1b-i1a)
             sigma1 = layerStrength*xx**power
             sigma1x = (2*side1-1)*power*layerStrength*xx**(power-1)
             ! (u(n+1) - 2*u + u(n-1))/dt^2 = u_tt + (dt^4/12)*u_tttt + ...
             !
             ! [u(n)-u(n-1)]/dt = u_t + (dt/2)*u_tt + O(dt^2)
             !
             ! u_tt = Delta u - v_x - w
             ! u_tttt = Delta u_tt - v_xtt - wtt
             ! 
             v=vrb (i1,i2,i3,ex)
             vx  = vrb x43r(i1,i2,i3,ex)
             vxx = vrb xx43r(i1,i2,i3,ex)
             vxxx= vrb xxx23r(i1,i2,i3,ex)
             vxyy= vrb xyy23r(i1,i2,i3,ex)
             w=wrb(i1,i2,i3,ex)
             wx  = wrb x43r(i1,i2,i3,ex)
             wxx = wrb xx43r(i1,i2,i3,ex)
             ux= ux43r(i1,i2,i3,ex)
             uxx= uxx43r(i1,i2,i3,ex)
             uxxx=uxxx23r(i1,i2,i3,ex)
             uxyy=uxyy23r(i1,i2,i3,ex)
             uxxxx=uxxxx23r(i1,i2,i3,ex)
             uxxyy=uxxyy23r(i1,i2,i3,ex)
             uLap = uLaplacian43r(i1,i2,i3,ex)
             ! --- these change in 3D ---
               uLapSq=uLapSq23r(i1,i2,i3,ex)
               uxzz=uxzz23r(i1,i2,i3,ex)
               uxxzz=uxzz23r(i1,i2,i3,ex)
               vxzz= vrb xzz23r(i1,i2,i3,ex)
               uLapx = uxxx+uxyy+uxzz
               uLapxx= uxxxx+uxxyy+uxxzz
               vLapx=vxxx+vxyy+vxzz
             ut = (u(i1,i2,i3,ex)-um(i1,i2,i3,ex))/dt - (.5*dt*csq)*( 
     & uLap - vx - w )
             uxt = ( ux-umx43r(i1,i2,i3,ex))/dt  - (.5*dt*csq)*( uLapx 
     & - vxx - wx )
             uxxt= (uxx-umxx43r(i1,i2,i3,ex))/dt - (.5*dt*csq)*( 
     & uLapxx - vxxx - wxx )
             ! *** uxxxt= (uxxx-umxxx43r(i1,i2,i3,ex))/dt   ! only need to first order in dt
             ! *** uxyyt= (uxyy-umxyy43r(i1,i2,i3,ex))/dt   ! only need to first order in dt
             ! *** uxxxxt= (uxxxx-umxxxx43r(i1,i2,i3,ex))/dt   ! only need to first order in dt
             ! *** uxxyyt= (uxxyy-umxxyy43r(i1,i2,i3,ex))/dt   ! only need to first order in dt
             vt = sigma1*( -v + ux )
             vxt = sigma1*( -vx + uxx ) + sigma1x*( -v + ux )
             vxtt = sigma1*( -vxt + uxxt ) + sigma1x*( -vt + uxt )
             wt =  sigma1*( -w -vx + uxx )
             wtt = sigma1*( -wt -vxt + uxxt )
               un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( uLap - vx -w ) + cdt4Over12*( uLapSq - vLapx - wrb 
     & Laplacian43r(i1,i2,i3,ex)  - vxtt - wtt )
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
             ! vrb n(i1,i2,i3,ex)=vrb ex(i1,i2,i3,ex)+(2.*dt)*( vt + (dt**2/6.)*vttt )
             vrb n(i1,i2,i3,ex)=vrb(i1,i2,i3,ex)+(dt)*( vt + dt*( .5*
     & vtt + dt*( (1./6.)*vttt + dt*( (1./24.)*vtttt ) ) ) )
             ! write(*,'(" i1,i2=",2i3," vt,vtt,vttt=",3e10.2," v,ux,uxt,uxtt=",4e10.2)') i1,i2,vt,vtt,vttt,v,ux,uxt,uxtt
             ! w_t = sigma1*( -w -vx + uxx )
             wttt = sigma1*( -wtt -vxtt + uxxtt )
             wtttt = 0. ! **** sigma1*( -wttt -vxttt + uxxttt )
            ! wan(i1,i2,i3,ex)=wam(i1,i2,i3,ex)+(2.*dt)*( wt + (dt**2/6.)*wttt )
             wrb n(i1,i2,i3,ex)=wrb(i1,i2,i3,ex)+(dt)*(  wt + dt*( .5*
     & wtt + dt*( (1./6.)*wttt + dt*( (1./24.)*wtttt ) ) ) )
           end do
           end do
           end do
          end if

         else if( boxType.eq.ySide )then

          if( side2.eq.0 )then
           do i3=m3a,m3b
           do i2=m2a,m2b
           do i1=m1a,m1b
             ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
             ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
             yy=(i2-i2a)/real(i2b-i2a)
             sigma2 = layerStrength*yy**power
             sigma2y = (2*side2-1)*power*layerStrength*yy**(power-1)
             ! (u(n+1) - 2*u + u(n-1))/dt^2 = u_tt + (dt^4/12)*u_tttt + ...
             !
             ! [u(n)-u(n-1)]/dt = u_t + (dt/2)*u_tt + O(dt^2)
             !
             ! u_tt = Delta u - v_y - w
             ! u_tttt = Delta u_tt - v_ytt - wtt
             ! 
             v=vsa (i1,i2,i3,ex)
             vy  = vsa y43r(i1,i2,i3,ex)
             vyy = vsa yy43r(i1,i2,i3,ex)
             vyyy= vsa yyy23r(i1,i2,i3,ex)
             vyzz= vsa yzz23r(i1,i2,i3,ex)
             w=wsa(i1,i2,i3,ex)
             wy  = wsa y43r(i1,i2,i3,ex)
             wyy = wsa yy43r(i1,i2,i3,ex)
             uy= uy43r(i1,i2,i3,ex)
             uyy= uyy43r(i1,i2,i3,ex)
             uyyy=uyyy23r(i1,i2,i3,ex)
             uyzz=uyzz23r(i1,i2,i3,ex)
             uyyyy=uyyyy23r(i1,i2,i3,ex)
             uyyzz=uyyzz23r(i1,i2,i3,ex)
             uLap = uLaplacian43r(i1,i2,i3,ex)
             ! --- these change in 3D ---
               uLapSq=uLapSq23r(i1,i2,i3,ex)
               uxxy=uxxy23r(i1,i2,i3,ex)
               uxxyy=uxxy23r(i1,i2,i3,ex)
               vxxy= vsa xxy23r(i1,i2,i3,ex)
               uLapy = uyyy+uyzz+uxxy
               uLapyy= uyyyy+uyyzz+uxxyy
               vLapy=vyyy+vyzz+vxxy
             ut = (u(i1,i2,i3,ex)-um(i1,i2,i3,ex))/dt - (.5*dt*csq)*( 
     & uLap - vy - w )
             uyt = ( uy-umy43r(i1,i2,i3,ex))/dt  - (.5*dt*csq)*( uLapy 
     & - vyy - wy )
             uyyt= (uyy-umyy43r(i1,i2,i3,ex))/dt - (.5*dt*csq)*( 
     & uLapyy - vyyy - wyy )
             ! *** uyyyt= (uyyy-umyyy43r(i1,i2,i3,ex))/dt   ! onlz need to first order in dt
             ! *** uyzzt= (uyzz-umyzz43r(i1,i2,i3,ex))/dt   ! onlz need to first order in dt
             ! *** uyyyyt= (uyyyy-umyyyy43r(i1,i2,i3,ex))/dt   ! onlz need to first order in dt
             ! *** uyyzzt= (uyyzz-umyyzz43r(i1,i2,i3,ex))/dt   ! onlz need to first order in dt
             vt = sigma2*( -v + uy )
             vyt = sigma2*( -vy + uyy ) + sigma2y*( -v + uy )
             vytt = sigma2*( -vyt + uyyt ) + sigma2y*( -vt + uyt )
             wt =  sigma2*( -w -vy + uyy )
             wtt = sigma2*( -wt -vyt + uyyt )
               un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( uLap - vy -w ) + cdt4Over12*( uLapSq - vLapy - wsa 
     & Laplacian43r(i1,i2,i3,ex)  - vytt - wtt )
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
             ! vsa n(i1,i2,i3,ex)=vsa ex(i1,i2,i3,ex)+(2.*dt)*( vt + (dt**2/6.)*vttt )
             vsa n(i1,i2,i3,ex)=vsa(i1,i2,i3,ex)+(dt)*( vt + dt*( .5*
     & vtt + dt*( (1./6.)*vttt + dt*( (1./24.)*vtttt ) ) ) )
             ! write(*,'(" i1,i2=",2i3," vt,vtt,vttt=",3e10.2," v,uy,uyt,uytt=",4e10.2)') i1,i2,vt,vtt,vttt,v,uy,uyt,uytt
             ! w_t = sigma2*( -w -vy + uyy )
             wttt = sigma2*( -wtt -vytt + uyytt )
             wtttt = 0. ! **** sigma2*( -wttt -vyttt + uyyttt )
            ! wan(i1,i2,i3,ex)=wam(i1,i2,i3,ex)+(2.*dt)*( wt + (dt**2/6.)*wttt )
             wsa n(i1,i2,i3,ex)=wsa(i1,i2,i3,ex)+(dt)*(  wt + dt*( .5*
     & wtt + dt*( (1./6.)*wttt + dt*( (1./24.)*wtttt ) ) ) )
           end do
           end do
           end do
          else
           do i3=m3a,m3b
           do i2=m2a,m2b
           do i1=m1a,m1b
             ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
             ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
             yy=(i2-i2a)/real(i2b-i2a)
             sigma2 = layerStrength*yy**power
             sigma2y = (2*side2-1)*power*layerStrength*yy**(power-1)
             ! (u(n+1) - 2*u + u(n-1))/dt^2 = u_tt + (dt^4/12)*u_tttt + ...
             !
             ! [u(n)-u(n-1)]/dt = u_t + (dt/2)*u_tt + O(dt^2)
             !
             ! u_tt = Delta u - v_y - w
             ! u_tttt = Delta u_tt - v_ytt - wtt
             ! 
             v=vsb (i1,i2,i3,ex)
             vy  = vsb y43r(i1,i2,i3,ex)
             vyy = vsb yy43r(i1,i2,i3,ex)
             vyyy= vsb yyy23r(i1,i2,i3,ex)
             vyzz= vsb yzz23r(i1,i2,i3,ex)
             w=wsb(i1,i2,i3,ex)
             wy  = wsb y43r(i1,i2,i3,ex)
             wyy = wsb yy43r(i1,i2,i3,ex)
             uy= uy43r(i1,i2,i3,ex)
             uyy= uyy43r(i1,i2,i3,ex)
             uyyy=uyyy23r(i1,i2,i3,ex)
             uyzz=uyzz23r(i1,i2,i3,ex)
             uyyyy=uyyyy23r(i1,i2,i3,ex)
             uyyzz=uyyzz23r(i1,i2,i3,ex)
             uLap = uLaplacian43r(i1,i2,i3,ex)
             ! --- these change in 3D ---
               uLapSq=uLapSq23r(i1,i2,i3,ex)
               uxxy=uxxy23r(i1,i2,i3,ex)
               uxxyy=uxxy23r(i1,i2,i3,ex)
               vxxy= vsb xxy23r(i1,i2,i3,ex)
               uLapy = uyyy+uyzz+uxxy
               uLapyy= uyyyy+uyyzz+uxxyy
               vLapy=vyyy+vyzz+vxxy
             ut = (u(i1,i2,i3,ex)-um(i1,i2,i3,ex))/dt - (.5*dt*csq)*( 
     & uLap - vy - w )
             uyt = ( uy-umy43r(i1,i2,i3,ex))/dt  - (.5*dt*csq)*( uLapy 
     & - vyy - wy )
             uyyt= (uyy-umyy43r(i1,i2,i3,ex))/dt - (.5*dt*csq)*( 
     & uLapyy - vyyy - wyy )
             ! *** uyyyt= (uyyy-umyyy43r(i1,i2,i3,ex))/dt   ! onlz need to first order in dt
             ! *** uyzzt= (uyzz-umyzz43r(i1,i2,i3,ex))/dt   ! onlz need to first order in dt
             ! *** uyyyyt= (uyyyy-umyyyy43r(i1,i2,i3,ex))/dt   ! onlz need to first order in dt
             ! *** uyyzzt= (uyyzz-umyyzz43r(i1,i2,i3,ex))/dt   ! onlz need to first order in dt
             vt = sigma2*( -v + uy )
             vyt = sigma2*( -vy + uyy ) + sigma2y*( -v + uy )
             vytt = sigma2*( -vyt + uyyt ) + sigma2y*( -vt + uyt )
             wt =  sigma2*( -w -vy + uyy )
             wtt = sigma2*( -wt -vyt + uyyt )
               un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( uLap - vy -w ) + cdt4Over12*( uLapSq - vLapy - wsb 
     & Laplacian43r(i1,i2,i3,ex)  - vytt - wtt )
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
             ! vsb n(i1,i2,i3,ex)=vsb ex(i1,i2,i3,ex)+(2.*dt)*( vt + (dt**2/6.)*vttt )
             vsb n(i1,i2,i3,ex)=vsb(i1,i2,i3,ex)+(dt)*( vt + dt*( .5*
     & vtt + dt*( (1./6.)*vttt + dt*( (1./24.)*vtttt ) ) ) )
             ! write(*,'(" i1,i2=",2i3," vt,vtt,vttt=",3e10.2," v,uy,uyt,uytt=",4e10.2)') i1,i2,vt,vtt,vttt,v,uy,uyt,uytt
             ! w_t = sigma2*( -w -vy + uyy )
             wttt = sigma2*( -wtt -vytt + uyytt )
             wtttt = 0. ! **** sigma2*( -wttt -vyttt + uyyttt )
            ! wan(i1,i2,i3,ex)=wam(i1,i2,i3,ex)+(2.*dt)*( wt + (dt**2/6.)*wttt )
             wsb n(i1,i2,i3,ex)=wsb(i1,i2,i3,ex)+(dt)*(  wt + dt*( .5*
     & wtt + dt*( (1./6.)*wttt + dt*( (1./24.)*wtttt ) ) ) )
           end do
           end do
           end do
          end if

         else if( boxType.eq.zSide )then

          if( side3.eq.0 )then
           do i3=m3a,m3b
           do i2=m2a,m2b
           do i1=m1a,m1b
             ! zScale=xy(i1,i2,i3b,2)-xy(i1,i2,i3a,2)
             ! zz=(xy(i1,i2,i3,2)-xy(i1,i2,i3a,2))/zScale
             zz=(i3-i3a)/real(i3b-i3a)
             sigma3 = layerStrength*zz**power
             sigma3z = (2*side3-1)*power*layerStrength*zz**(power-1)
             ! (u(n+1) - 2*u + u(n-1))/dt^2 = u_tt + (dt^4/12)*u_tttt + ...
             !
             ! [u(n)-u(n-1)]/dt = u_t + (dt/2)*u_tt + O(dt^2)
             !
             ! u_tt = Delta u - v_z - w
             ! u_tttt = Delta u_tt - v_ztt - wtt
             ! 
             v=vta (i1,i2,i3,ex)
             vz  = vta z43r(i1,i2,i3,ex)
             vzz = vta zz43r(i1,i2,i3,ex)
             vzzz= vta zzz23r(i1,i2,i3,ex)
             vxxz= vta xxz23r(i1,i2,i3,ex)
             w=wta(i1,i2,i3,ex)
             wz  = wta z43r(i1,i2,i3,ex)
             wzz = wta zz43r(i1,i2,i3,ex)
             uz= uz43r(i1,i2,i3,ex)
             uzz= uzz43r(i1,i2,i3,ex)
             uzzz=uzzz23r(i1,i2,i3,ex)
             uxxz=uxxz23r(i1,i2,i3,ex)
             uzzzz=uzzzz23r(i1,i2,i3,ex)
             uxxzz=uxxzz23r(i1,i2,i3,ex)
             uLap = uLaplacian43r(i1,i2,i3,ex)
             ! --- these change in 3D ---
               uLapSq=uLapSq23r(i1,i2,i3,ex)
               uyyz=uyyz23r(i1,i2,i3,ex)
               uyyzz=uyyz23r(i1,i2,i3,ex)
               vyyz= vta yyz23r(i1,i2,i3,ex)
               uLapz = uzzz+uxxz+uyyz
               uLapzz= uzzzz+uxxzz+uyyzz
               vLapz=vzzz+vxxz+vyyz
             ut = (u(i1,i2,i3,ex)-um(i1,i2,i3,ex))/dt - (.5*dt*csq)*( 
     & uLap - vz - w )
             uzt = ( uz-umz43r(i1,i2,i3,ex))/dt  - (.5*dt*csq)*( uLapz 
     & - vzz - wz )
             uzzt= (uzz-umzz43r(i1,i2,i3,ex))/dt - (.5*dt*csq)*( 
     & uLapzz - vzzz - wzz )
             ! *** uzzzt= (uzzz-umzzz43r(i1,i2,i3,ex))/dt   ! onlx need to first order in dt
             ! *** uxxzt= (uxxz-umxxz43r(i1,i2,i3,ex))/dt   ! onlx need to first order in dt
             ! *** uzzzzt= (uzzzz-umzzzz43r(i1,i2,i3,ex))/dt   ! onlx need to first order in dt
             ! *** uxxzzt= (uxxzz-umxxzz43r(i1,i2,i3,ex))/dt   ! onlx need to first order in dt
             vt = sigma3*( -v + uz )
             vzt = sigma3*( -vz + uzz ) + sigma3z*( -v + uz )
             vztt = sigma3*( -vzt + uzzt ) + sigma3z*( -vt + uzt )
             wt =  sigma3*( -w -vz + uzz )
             wtt = sigma3*( -wt -vzt + uzzt )
               un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( uLap - vz -w ) + cdt4Over12*( uLapSq - vLapz - wta 
     & Laplacian43r(i1,i2,i3,ex)  - vztt - wtt )
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
             ! vta n(i1,i2,i3,ex)=vta ex(i1,i2,i3,ex)+(2.*dt)*( vt + (dt**2/6.)*vttt )
             vta n(i1,i2,i3,ex)=vta(i1,i2,i3,ex)+(dt)*( vt + dt*( .5*
     & vtt + dt*( (1./6.)*vttt + dt*( (1./24.)*vtttt ) ) ) )
             ! write(*,'(" i1,i2=",2i3," vt,vtt,vttt=",3e10.2," v,uz,uzt,uztt=",4e10.2)') i1,i2,vt,vtt,vttt,v,uz,uzt,uztt
             ! w_t = sigma3*( -w -vz + uzz )
             wttt = sigma3*( -wtt -vztt + uzztt )
             wtttt = 0. ! **** sigma3*( -wttt -vzttt + uzzttt )
            ! wan(i1,i2,i3,ex)=wam(i1,i2,i3,ex)+(2.*dt)*( wt + (dt**2/6.)*wttt )
             wta n(i1,i2,i3,ex)=wta(i1,i2,i3,ex)+(dt)*(  wt + dt*( .5*
     & wtt + dt*( (1./6.)*wttt + dt*( (1./24.)*wtttt ) ) ) )
           end do
           end do
           end do
          else
           do i3=m3a,m3b
           do i2=m2a,m2b
           do i1=m1a,m1b
             ! zScale=xy(i1,i2,i3b,2)-xy(i1,i2,i3a,2)
             ! zz=(xy(i1,i2,i3,2)-xy(i1,i2,i3a,2))/zScale
             zz=(i3-i3a)/real(i3b-i3a)
             sigma3 = layerStrength*zz**power
             sigma3z = (2*side3-1)*power*layerStrength*zz**(power-1)
             ! (u(n+1) - 2*u + u(n-1))/dt^2 = u_tt + (dt^4/12)*u_tttt + ...
             !
             ! [u(n)-u(n-1)]/dt = u_t + (dt/2)*u_tt + O(dt^2)
             !
             ! u_tt = Delta u - v_z - w
             ! u_tttt = Delta u_tt - v_ztt - wtt
             ! 
             v=vtb (i1,i2,i3,ex)
             vz  = vtb z43r(i1,i2,i3,ex)
             vzz = vtb zz43r(i1,i2,i3,ex)
             vzzz= vtb zzz23r(i1,i2,i3,ex)
             vxxz= vtb xxz23r(i1,i2,i3,ex)
             w=wtb(i1,i2,i3,ex)
             wz  = wtb z43r(i1,i2,i3,ex)
             wzz = wtb zz43r(i1,i2,i3,ex)
             uz= uz43r(i1,i2,i3,ex)
             uzz= uzz43r(i1,i2,i3,ex)
             uzzz=uzzz23r(i1,i2,i3,ex)
             uxxz=uxxz23r(i1,i2,i3,ex)
             uzzzz=uzzzz23r(i1,i2,i3,ex)
             uxxzz=uxxzz23r(i1,i2,i3,ex)
             uLap = uLaplacian43r(i1,i2,i3,ex)
             ! --- these change in 3D ---
               uLapSq=uLapSq23r(i1,i2,i3,ex)
               uyyz=uyyz23r(i1,i2,i3,ex)
               uyyzz=uyyz23r(i1,i2,i3,ex)
               vyyz= vtb yyz23r(i1,i2,i3,ex)
               uLapz = uzzz+uxxz+uyyz
               uLapzz= uzzzz+uxxzz+uyyzz
               vLapz=vzzz+vxxz+vyyz
             ut = (u(i1,i2,i3,ex)-um(i1,i2,i3,ex))/dt - (.5*dt*csq)*( 
     & uLap - vz - w )
             uzt = ( uz-umz43r(i1,i2,i3,ex))/dt  - (.5*dt*csq)*( uLapz 
     & - vzz - wz )
             uzzt= (uzz-umzz43r(i1,i2,i3,ex))/dt - (.5*dt*csq)*( 
     & uLapzz - vzzz - wzz )
             ! *** uzzzt= (uzzz-umzzz43r(i1,i2,i3,ex))/dt   ! onlx need to first order in dt
             ! *** uxxzt= (uxxz-umxxz43r(i1,i2,i3,ex))/dt   ! onlx need to first order in dt
             ! *** uzzzzt= (uzzzz-umzzzz43r(i1,i2,i3,ex))/dt   ! onlx need to first order in dt
             ! *** uxxzzt= (uxxzz-umxxzz43r(i1,i2,i3,ex))/dt   ! onlx need to first order in dt
             vt = sigma3*( -v + uz )
             vzt = sigma3*( -vz + uzz ) + sigma3z*( -v + uz )
             vztt = sigma3*( -vzt + uzzt ) + sigma3z*( -vt + uzt )
             wt =  sigma3*( -w -vz + uzz )
             wtt = sigma3*( -wt -vzt + uzzt )
               un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( uLap - vz -w ) + cdt4Over12*( uLapSq - vLapz - wtb 
     & Laplacian43r(i1,i2,i3,ex)  - vztt - wtt )
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
             ! vtb n(i1,i2,i3,ex)=vtb ex(i1,i2,i3,ex)+(2.*dt)*( vt + (dt**2/6.)*vttt )
             vtb n(i1,i2,i3,ex)=vtb(i1,i2,i3,ex)+(dt)*( vt + dt*( .5*
     & vtt + dt*( (1./6.)*vttt + dt*( (1./24.)*vtttt ) ) ) )
             ! write(*,'(" i1,i2=",2i3," vt,vtt,vttt=",3e10.2," v,uz,uzt,uztt=",4e10.2)') i1,i2,vt,vtt,vttt,v,uz,uzt,uztt
             ! w_t = sigma3*( -w -vz + uzz )
             wttt = sigma3*( -wtt -vztt + uzztt )
             wtttt = 0. ! **** sigma3*( -wttt -vzttt + uzzttt )
            ! wan(i1,i2,i3,ex)=wam(i1,i2,i3,ex)+(2.*dt)*( wt + (dt**2/6.)*wttt )
             wtb n(i1,i2,i3,ex)=wtb(i1,i2,i3,ex)+(dt)*(  wt + dt*( .5*
     & wtt + dt*( (1./6.)*wttt + dt*( (1./24.)*wtttt ) ) ) )
           end do
           end do
           end do
          end if

         else

           ! macro to advance edge and corner regions:
           ! **** use 2nd order version for now:
             if( boxType.eq.xyEdge )then
              if( side1.eq.0 .and. side2.eq.0 )then
               do i3=m3a,m3b
               do i2=m2a,m2b
               do i1=m1a,m1b
                 ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
                 ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
                 xx=(i1-i1a)/real(i1b-i1a)
                 sigma1 = layerStrength*xx**power
                 ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
                 ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
                 yy=(i2-i2a)/real(i2b-i2a)
                 sigma2 = layerStrength*yy**power
                 un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( ulaplacian23r(i1,i2,i3,ex) - vra x23r(i1,i2,i3,ex) -
     & wra(i1,i2,i3,ex) - vsa y23r(i1,i2,i3,ex) -wsa(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vra n(i1,i2,i3,ex)=vra(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -vra(i1,i2,i3,ex) +  ux23r(i1,i2,i3,ex))-0.5*(-vra m(i1,i2,i3,
     & ex) + umx23r(i1,i2,i3,ex)) )
                 wra n(i1,i2,i3,ex)=wra(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -wra(i1,i2,i3,ex)+  uxx23r(i1,i2,i3,ex) - vra x23r(i1,i2,i3,
     & ex))-0.5*(-wra m(i1,i2,i3,ex)+ umxx23r(i1,i2,i3,ex)-vra m x23r(
     & i1,i2,i3,ex)) )
                 vsa n(i1,i2,i3,ex)=vsa(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     &  -vsa(i1,i2,i3,ex) +  uy23r(i1,i2,i3,ex))-0.5*(-vsa m(i1,i2,i3,
     & ex) + umy23r(i1,i2,i3,ex)) )
                 wsa n(i1,i2,i3,ex)=wsa(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     & -wsa(i1,i2,i3,ex)+  uyy23r(i1,i2,i3,ex)   -vsa y23r(i1,i2,i3,
     & ex))-0.5*(-wsa m(i1,i2,i3,ex)+ umyy23r(i1,i2,i3,ex)-vsa m y23r(
     & i1,i2,i3,ex)) )
               end do
               end do
               end do
              else if( side1.eq.1 .and. side2.eq.0 )then
               do i3=m3a,m3b
               do i2=m2a,m2b
               do i1=m1a,m1b
                 ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
                 ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
                 xx=(i1-i1a)/real(i1b-i1a)
                 sigma1 = layerStrength*xx**power
                 ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
                 ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
                 yy=(i2-i2a)/real(i2b-i2a)
                 sigma2 = layerStrength*yy**power
                 un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( ulaplacian23r(i1,i2,i3,ex) - vrb x23r(i1,i2,i3,ex) -
     & wrb(i1,i2,i3,ex) - vsa y23r(i1,i2,i3,ex) -wsa(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vrb n(i1,i2,i3,ex)=vrb(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -vrb(i1,i2,i3,ex) +  ux23r(i1,i2,i3,ex))-0.5*(-vrb m(i1,i2,i3,
     & ex) + umx23r(i1,i2,i3,ex)) )
                 wrb n(i1,i2,i3,ex)=wrb(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -wrb(i1,i2,i3,ex)+  uxx23r(i1,i2,i3,ex) - vrb x23r(i1,i2,i3,
     & ex))-0.5*(-wrb m(i1,i2,i3,ex)+ umxx23r(i1,i2,i3,ex)-vrb m x23r(
     & i1,i2,i3,ex)) )
                 vsa n(i1,i2,i3,ex)=vsa(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     &  -vsa(i1,i2,i3,ex) +  uy23r(i1,i2,i3,ex))-0.5*(-vsa m(i1,i2,i3,
     & ex) + umy23r(i1,i2,i3,ex)) )
                 wsa n(i1,i2,i3,ex)=wsa(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     & -wsa(i1,i2,i3,ex)+  uyy23r(i1,i2,i3,ex)   -vsa y23r(i1,i2,i3,
     & ex))-0.5*(-wsa m(i1,i2,i3,ex)+ umyy23r(i1,i2,i3,ex)-vsa m y23r(
     & i1,i2,i3,ex)) )
               end do
               end do
               end do
              else if( side1.eq.0 .and. side2.eq.1 )then
               do i3=m3a,m3b
               do i2=m2a,m2b
               do i1=m1a,m1b
                 ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
                 ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
                 xx=(i1-i1a)/real(i1b-i1a)
                 sigma1 = layerStrength*xx**power
                 ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
                 ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
                 yy=(i2-i2a)/real(i2b-i2a)
                 sigma2 = layerStrength*yy**power
                 un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( ulaplacian23r(i1,i2,i3,ex) - vra x23r(i1,i2,i3,ex) -
     & wra(i1,i2,i3,ex) - vsb y23r(i1,i2,i3,ex) -wsb(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vra n(i1,i2,i3,ex)=vra(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -vra(i1,i2,i3,ex) +  ux23r(i1,i2,i3,ex))-0.5*(-vra m(i1,i2,i3,
     & ex) + umx23r(i1,i2,i3,ex)) )
                 wra n(i1,i2,i3,ex)=wra(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -wra(i1,i2,i3,ex)+  uxx23r(i1,i2,i3,ex) - vra x23r(i1,i2,i3,
     & ex))-0.5*(-wra m(i1,i2,i3,ex)+ umxx23r(i1,i2,i3,ex)-vra m x23r(
     & i1,i2,i3,ex)) )
                 vsb n(i1,i2,i3,ex)=vsb(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     &  -vsb(i1,i2,i3,ex) +  uy23r(i1,i2,i3,ex))-0.5*(-vsb m(i1,i2,i3,
     & ex) + umy23r(i1,i2,i3,ex)) )
                 wsb n(i1,i2,i3,ex)=wsb(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     & -wsb(i1,i2,i3,ex)+  uyy23r(i1,i2,i3,ex)   -vsb y23r(i1,i2,i3,
     & ex))-0.5*(-wsb m(i1,i2,i3,ex)+ umyy23r(i1,i2,i3,ex)-vsb m y23r(
     & i1,i2,i3,ex)) )
               end do
               end do
               end do
              else if( side1.eq.1 .and. side2.eq.1 )then
               do i3=m3a,m3b
               do i2=m2a,m2b
               do i1=m1a,m1b
                 ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
                 ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
                 xx=(i1-i1a)/real(i1b-i1a)
                 sigma1 = layerStrength*xx**power
                 ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
                 ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
                 yy=(i2-i2a)/real(i2b-i2a)
                 sigma2 = layerStrength*yy**power
                 un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( ulaplacian23r(i1,i2,i3,ex) - vrb x23r(i1,i2,i3,ex) -
     & wrb(i1,i2,i3,ex) - vsb y23r(i1,i2,i3,ex) -wsb(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vrb n(i1,i2,i3,ex)=vrb(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -vrb(i1,i2,i3,ex) +  ux23r(i1,i2,i3,ex))-0.5*(-vrb m(i1,i2,i3,
     & ex) + umx23r(i1,i2,i3,ex)) )
                 wrb n(i1,i2,i3,ex)=wrb(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -wrb(i1,i2,i3,ex)+  uxx23r(i1,i2,i3,ex) - vrb x23r(i1,i2,i3,
     & ex))-0.5*(-wrb m(i1,i2,i3,ex)+ umxx23r(i1,i2,i3,ex)-vrb m x23r(
     & i1,i2,i3,ex)) )
                 vsb n(i1,i2,i3,ex)=vsb(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     &  -vsb(i1,i2,i3,ex) +  uy23r(i1,i2,i3,ex))-0.5*(-vsb m(i1,i2,i3,
     & ex) + umy23r(i1,i2,i3,ex)) )
                 wsb n(i1,i2,i3,ex)=wsb(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     & -wsb(i1,i2,i3,ex)+  uyy23r(i1,i2,i3,ex)   -vsb y23r(i1,i2,i3,
     & ex))-0.5*(-wsb m(i1,i2,i3,ex)+ umyy23r(i1,i2,i3,ex)-vsb m y23r(
     & i1,i2,i3,ex)) )
               end do
               end do
               end do
              else
                stop 66224
              end if
             else if( boxType.eq.xzEdge )then
              if( side1.eq.0 .and. side3.eq.0 )then
               do i3=m3a,m3b
               do i2=m2a,m2b
               do i1=m1a,m1b
                 ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
                 ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
                 xx=(i1-i1a)/real(i1b-i1a)
                 sigma1 = layerStrength*xx**power
                 ! zScale=xy(i1,i2,i3b,2)-xy(i1,i2,i3a,2)
                 ! zz=(xy(i1,i2,i3,2)-xy(i1,i2,i3a,2))/zScale
                 zz=(i3-i3a)/real(i3b-i3a)
                 sigma3 = layerStrength*zz**power
                 un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( ulaplacian23r(i1,i2,i3,ex) - vra x23r(i1,i2,i3,ex) -
     & wra(i1,i2,i3,ex) - vta z23r(i1,i2,i3,ex) -wta(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vra n(i1,i2,i3,ex)=vra(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -vra(i1,i2,i3,ex) +  ux23r(i1,i2,i3,ex))-0.5*(-vra m(i1,i2,i3,
     & ex) + umx23r(i1,i2,i3,ex)) )
                 wra n(i1,i2,i3,ex)=wra(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -wra(i1,i2,i3,ex)+  uxx23r(i1,i2,i3,ex) - vra x23r(i1,i2,i3,
     & ex))-0.5*(-wra m(i1,i2,i3,ex)+ umxx23r(i1,i2,i3,ex)-vra m x23r(
     & i1,i2,i3,ex)) )
                 vta n(i1,i2,i3,ex)=vta(i1,i2,i3,ex)+sigma3*dt*( 1.5*( 
     &  -vta(i1,i2,i3,ex) +  uz23r(i1,i2,i3,ex))-0.5*(-vta m(i1,i2,i3,
     & ex) + umz23r(i1,i2,i3,ex)) )
                 wta n(i1,i2,i3,ex)=wta(i1,i2,i3,ex)+sigma3*dt*( 1.5*( 
     & -wta(i1,i2,i3,ex)+  uzz23r(i1,i2,i3,ex)   -vta z23r(i1,i2,i3,
     & ex))-0.5*(-wta m(i1,i2,i3,ex)+ umzz23r(i1,i2,i3,ex)-vta m z23r(
     & i1,i2,i3,ex)) )
               end do
               end do
               end do
              else if( side1.eq.1 .and. side3.eq.0 )then
               do i3=m3a,m3b
               do i2=m2a,m2b
               do i1=m1a,m1b
                 ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
                 ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
                 xx=(i1-i1a)/real(i1b-i1a)
                 sigma1 = layerStrength*xx**power
                 ! zScale=xy(i1,i2,i3b,2)-xy(i1,i2,i3a,2)
                 ! zz=(xy(i1,i2,i3,2)-xy(i1,i2,i3a,2))/zScale
                 zz=(i3-i3a)/real(i3b-i3a)
                 sigma3 = layerStrength*zz**power
                 un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( ulaplacian23r(i1,i2,i3,ex) - vrb x23r(i1,i2,i3,ex) -
     & wrb(i1,i2,i3,ex) - vta z23r(i1,i2,i3,ex) -wta(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vrb n(i1,i2,i3,ex)=vrb(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -vrb(i1,i2,i3,ex) +  ux23r(i1,i2,i3,ex))-0.5*(-vrb m(i1,i2,i3,
     & ex) + umx23r(i1,i2,i3,ex)) )
                 wrb n(i1,i2,i3,ex)=wrb(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -wrb(i1,i2,i3,ex)+  uxx23r(i1,i2,i3,ex) - vrb x23r(i1,i2,i3,
     & ex))-0.5*(-wrb m(i1,i2,i3,ex)+ umxx23r(i1,i2,i3,ex)-vrb m x23r(
     & i1,i2,i3,ex)) )
                 vta n(i1,i2,i3,ex)=vta(i1,i2,i3,ex)+sigma3*dt*( 1.5*( 
     &  -vta(i1,i2,i3,ex) +  uz23r(i1,i2,i3,ex))-0.5*(-vta m(i1,i2,i3,
     & ex) + umz23r(i1,i2,i3,ex)) )
                 wta n(i1,i2,i3,ex)=wta(i1,i2,i3,ex)+sigma3*dt*( 1.5*( 
     & -wta(i1,i2,i3,ex)+  uzz23r(i1,i2,i3,ex)   -vta z23r(i1,i2,i3,
     & ex))-0.5*(-wta m(i1,i2,i3,ex)+ umzz23r(i1,i2,i3,ex)-vta m z23r(
     & i1,i2,i3,ex)) )
               end do
               end do
               end do
              else if( side1.eq.0 .and. side3.eq.1 )then
               do i3=m3a,m3b
               do i2=m2a,m2b
               do i1=m1a,m1b
                 ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
                 ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
                 xx=(i1-i1a)/real(i1b-i1a)
                 sigma1 = layerStrength*xx**power
                 ! zScale=xy(i1,i2,i3b,2)-xy(i1,i2,i3a,2)
                 ! zz=(xy(i1,i2,i3,2)-xy(i1,i2,i3a,2))/zScale
                 zz=(i3-i3a)/real(i3b-i3a)
                 sigma3 = layerStrength*zz**power
                 un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( ulaplacian23r(i1,i2,i3,ex) - vra x23r(i1,i2,i3,ex) -
     & wra(i1,i2,i3,ex) - vtb z23r(i1,i2,i3,ex) -wtb(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vra n(i1,i2,i3,ex)=vra(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -vra(i1,i2,i3,ex) +  ux23r(i1,i2,i3,ex))-0.5*(-vra m(i1,i2,i3,
     & ex) + umx23r(i1,i2,i3,ex)) )
                 wra n(i1,i2,i3,ex)=wra(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -wra(i1,i2,i3,ex)+  uxx23r(i1,i2,i3,ex) - vra x23r(i1,i2,i3,
     & ex))-0.5*(-wra m(i1,i2,i3,ex)+ umxx23r(i1,i2,i3,ex)-vra m x23r(
     & i1,i2,i3,ex)) )
                 vtb n(i1,i2,i3,ex)=vtb(i1,i2,i3,ex)+sigma3*dt*( 1.5*( 
     &  -vtb(i1,i2,i3,ex) +  uz23r(i1,i2,i3,ex))-0.5*(-vtb m(i1,i2,i3,
     & ex) + umz23r(i1,i2,i3,ex)) )
                 wtb n(i1,i2,i3,ex)=wtb(i1,i2,i3,ex)+sigma3*dt*( 1.5*( 
     & -wtb(i1,i2,i3,ex)+  uzz23r(i1,i2,i3,ex)   -vtb z23r(i1,i2,i3,
     & ex))-0.5*(-wtb m(i1,i2,i3,ex)+ umzz23r(i1,i2,i3,ex)-vtb m z23r(
     & i1,i2,i3,ex)) )
               end do
               end do
               end do
              else if( side1.eq.1 .and. side3.eq.1 )then
               do i3=m3a,m3b
               do i2=m2a,m2b
               do i1=m1a,m1b
                 ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
                 ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
                 xx=(i1-i1a)/real(i1b-i1a)
                 sigma1 = layerStrength*xx**power
                 ! zScale=xy(i1,i2,i3b,2)-xy(i1,i2,i3a,2)
                 ! zz=(xy(i1,i2,i3,2)-xy(i1,i2,i3a,2))/zScale
                 zz=(i3-i3a)/real(i3b-i3a)
                 sigma3 = layerStrength*zz**power
                 un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( ulaplacian23r(i1,i2,i3,ex) - vrb x23r(i1,i2,i3,ex) -
     & wrb(i1,i2,i3,ex) - vtb z23r(i1,i2,i3,ex) -wtb(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vrb n(i1,i2,i3,ex)=vrb(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -vrb(i1,i2,i3,ex) +  ux23r(i1,i2,i3,ex))-0.5*(-vrb m(i1,i2,i3,
     & ex) + umx23r(i1,i2,i3,ex)) )
                 wrb n(i1,i2,i3,ex)=wrb(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -wrb(i1,i2,i3,ex)+  uxx23r(i1,i2,i3,ex) - vrb x23r(i1,i2,i3,
     & ex))-0.5*(-wrb m(i1,i2,i3,ex)+ umxx23r(i1,i2,i3,ex)-vrb m x23r(
     & i1,i2,i3,ex)) )
                 vtb n(i1,i2,i3,ex)=vtb(i1,i2,i3,ex)+sigma3*dt*( 1.5*( 
     &  -vtb(i1,i2,i3,ex) +  uz23r(i1,i2,i3,ex))-0.5*(-vtb m(i1,i2,i3,
     & ex) + umz23r(i1,i2,i3,ex)) )
                 wtb n(i1,i2,i3,ex)=wtb(i1,i2,i3,ex)+sigma3*dt*( 1.5*( 
     & -wtb(i1,i2,i3,ex)+  uzz23r(i1,i2,i3,ex)   -vtb z23r(i1,i2,i3,
     & ex))-0.5*(-wtb m(i1,i2,i3,ex)+ umzz23r(i1,i2,i3,ex)-vtb m z23r(
     & i1,i2,i3,ex)) )
               end do
               end do
               end do
              else
                stop 62244
              end if
             else if( boxType.eq.yzEdge )then
              if( side2.eq.0 .and. side3.eq.0 )then
               do i3=m3a,m3b
               do i2=m2a,m2b
               do i1=m1a,m1b
                 ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
                 ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
                 yy=(i2-i2a)/real(i2b-i2a)
                 sigma2 = layerStrength*yy**power
                 ! zScale=xy(i1,i2,i3b,2)-xy(i1,i2,i3a,2)
                 ! zz=(xy(i1,i2,i3,2)-xy(i1,i2,i3a,2))/zScale
                 zz=(i3-i3a)/real(i3b-i3a)
                 sigma3 = layerStrength*zz**power
                 un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( ulaplacian23r(i1,i2,i3,ex) - vsa y23r(i1,i2,i3,ex) -
     & wsa(i1,i2,i3,ex) - vta z23r(i1,i2,i3,ex) -wta(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vsa n(i1,i2,i3,ex)=vsa(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     &  -vsa(i1,i2,i3,ex) +  uy23r(i1,i2,i3,ex))-0.5*(-vsa m(i1,i2,i3,
     & ex) + umy23r(i1,i2,i3,ex)) )
                 wsa n(i1,i2,i3,ex)=wsa(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     &  -wsa(i1,i2,i3,ex)+  uyy23r(i1,i2,i3,ex) - vsa y23r(i1,i2,i3,
     & ex))-0.5*(-wsa m(i1,i2,i3,ex)+ umyy23r(i1,i2,i3,ex)-vsa m y23r(
     & i1,i2,i3,ex)) )
                 vta n(i1,i2,i3,ex)=vta(i1,i2,i3,ex)+sigma3*dt*( 1.5*( 
     &  -vta(i1,i2,i3,ex) +  uz23r(i1,i2,i3,ex))-0.5*(-vta m(i1,i2,i3,
     & ex) + umz23r(i1,i2,i3,ex)) )
                 wta n(i1,i2,i3,ex)=wta(i1,i2,i3,ex)+sigma3*dt*( 1.5*( 
     & -wta(i1,i2,i3,ex)+  uzz23r(i1,i2,i3,ex)   -vta z23r(i1,i2,i3,
     & ex))-0.5*(-wta m(i1,i2,i3,ex)+ umzz23r(i1,i2,i3,ex)-vta m z23r(
     & i1,i2,i3,ex)) )
               end do
               end do
               end do
              else if( side2.eq.1 .and. side3.eq.0 )then
               do i3=m3a,m3b
               do i2=m2a,m2b
               do i1=m1a,m1b
                 ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
                 ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
                 yy=(i2-i2a)/real(i2b-i2a)
                 sigma2 = layerStrength*yy**power
                 ! zScale=xy(i1,i2,i3b,2)-xy(i1,i2,i3a,2)
                 ! zz=(xy(i1,i2,i3,2)-xy(i1,i2,i3a,2))/zScale
                 zz=(i3-i3a)/real(i3b-i3a)
                 sigma3 = layerStrength*zz**power
                 un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( ulaplacian23r(i1,i2,i3,ex) - vsb y23r(i1,i2,i3,ex) -
     & wsb(i1,i2,i3,ex) - vta z23r(i1,i2,i3,ex) -wta(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vsb n(i1,i2,i3,ex)=vsb(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     &  -vsb(i1,i2,i3,ex) +  uy23r(i1,i2,i3,ex))-0.5*(-vsb m(i1,i2,i3,
     & ex) + umy23r(i1,i2,i3,ex)) )
                 wsb n(i1,i2,i3,ex)=wsb(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     &  -wsb(i1,i2,i3,ex)+  uyy23r(i1,i2,i3,ex) - vsb y23r(i1,i2,i3,
     & ex))-0.5*(-wsb m(i1,i2,i3,ex)+ umyy23r(i1,i2,i3,ex)-vsb m y23r(
     & i1,i2,i3,ex)) )
                 vta n(i1,i2,i3,ex)=vta(i1,i2,i3,ex)+sigma3*dt*( 1.5*( 
     &  -vta(i1,i2,i3,ex) +  uz23r(i1,i2,i3,ex))-0.5*(-vta m(i1,i2,i3,
     & ex) + umz23r(i1,i2,i3,ex)) )
                 wta n(i1,i2,i3,ex)=wta(i1,i2,i3,ex)+sigma3*dt*( 1.5*( 
     & -wta(i1,i2,i3,ex)+  uzz23r(i1,i2,i3,ex)   -vta z23r(i1,i2,i3,
     & ex))-0.5*(-wta m(i1,i2,i3,ex)+ umzz23r(i1,i2,i3,ex)-vta m z23r(
     & i1,i2,i3,ex)) )
               end do
               end do
               end do
              else if( side2.eq.0 .and. side3.eq.1 )then
               do i3=m3a,m3b
               do i2=m2a,m2b
               do i1=m1a,m1b
                 ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
                 ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
                 yy=(i2-i2a)/real(i2b-i2a)
                 sigma2 = layerStrength*yy**power
                 ! zScale=xy(i1,i2,i3b,2)-xy(i1,i2,i3a,2)
                 ! zz=(xy(i1,i2,i3,2)-xy(i1,i2,i3a,2))/zScale
                 zz=(i3-i3a)/real(i3b-i3a)
                 sigma3 = layerStrength*zz**power
                 un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( ulaplacian23r(i1,i2,i3,ex) - vsa y23r(i1,i2,i3,ex) -
     & wsa(i1,i2,i3,ex) - vtb z23r(i1,i2,i3,ex) -wtb(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vsa n(i1,i2,i3,ex)=vsa(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     &  -vsa(i1,i2,i3,ex) +  uy23r(i1,i2,i3,ex))-0.5*(-vsa m(i1,i2,i3,
     & ex) + umy23r(i1,i2,i3,ex)) )
                 wsa n(i1,i2,i3,ex)=wsa(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     &  -wsa(i1,i2,i3,ex)+  uyy23r(i1,i2,i3,ex) - vsa y23r(i1,i2,i3,
     & ex))-0.5*(-wsa m(i1,i2,i3,ex)+ umyy23r(i1,i2,i3,ex)-vsa m y23r(
     & i1,i2,i3,ex)) )
                 vtb n(i1,i2,i3,ex)=vtb(i1,i2,i3,ex)+sigma3*dt*( 1.5*( 
     &  -vtb(i1,i2,i3,ex) +  uz23r(i1,i2,i3,ex))-0.5*(-vtb m(i1,i2,i3,
     & ex) + umz23r(i1,i2,i3,ex)) )
                 wtb n(i1,i2,i3,ex)=wtb(i1,i2,i3,ex)+sigma3*dt*( 1.5*( 
     & -wtb(i1,i2,i3,ex)+  uzz23r(i1,i2,i3,ex)   -vtb z23r(i1,i2,i3,
     & ex))-0.5*(-wtb m(i1,i2,i3,ex)+ umzz23r(i1,i2,i3,ex)-vtb m z23r(
     & i1,i2,i3,ex)) )
               end do
               end do
               end do
              else if( side2.eq.1 .and. side3.eq.1 )then
               do i3=m3a,m3b
               do i2=m2a,m2b
               do i1=m1a,m1b
                 ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
                 ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
                 yy=(i2-i2a)/real(i2b-i2a)
                 sigma2 = layerStrength*yy**power
                 ! zScale=xy(i1,i2,i3b,2)-xy(i1,i2,i3a,2)
                 ! zz=(xy(i1,i2,i3,2)-xy(i1,i2,i3a,2))/zScale
                 zz=(i3-i3a)/real(i3b-i3a)
                 sigma3 = layerStrength*zz**power
                 un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( ulaplacian23r(i1,i2,i3,ex) - vsb y23r(i1,i2,i3,ex) -
     & wsb(i1,i2,i3,ex) - vtb z23r(i1,i2,i3,ex) -wtb(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vsb n(i1,i2,i3,ex)=vsb(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     &  -vsb(i1,i2,i3,ex) +  uy23r(i1,i2,i3,ex))-0.5*(-vsb m(i1,i2,i3,
     & ex) + umy23r(i1,i2,i3,ex)) )
                 wsb n(i1,i2,i3,ex)=wsb(i1,i2,i3,ex)+sigma2*dt*( 1.5*( 
     &  -wsb(i1,i2,i3,ex)+  uyy23r(i1,i2,i3,ex) - vsb y23r(i1,i2,i3,
     & ex))-0.5*(-wsb m(i1,i2,i3,ex)+ umyy23r(i1,i2,i3,ex)-vsb m y23r(
     & i1,i2,i3,ex)) )
                 vtb n(i1,i2,i3,ex)=vtb(i1,i2,i3,ex)+sigma3*dt*( 1.5*( 
     &  -vtb(i1,i2,i3,ex) +  uz23r(i1,i2,i3,ex))-0.5*(-vtb m(i1,i2,i3,
     & ex) + umz23r(i1,i2,i3,ex)) )
                 wtb n(i1,i2,i3,ex)=wtb(i1,i2,i3,ex)+sigma3*dt*( 1.5*( 
     & -wtb(i1,i2,i3,ex)+  uzz23r(i1,i2,i3,ex)   -vtb z23r(i1,i2,i3,
     & ex))-0.5*(-wtb m(i1,i2,i3,ex)+ umzz23r(i1,i2,i3,ex)-vtb m z23r(
     & i1,i2,i3,ex)) )
               end do
               end do
               end do
              else
                stop 6644
              end if
             else if( boxType.eq.xyzCorner )then
              if(      side1.eq.0 .and. side2.eq.0 .and. side3.eq.0 )
     & then
                 do i3=m3a,m3b
                 do i2=m2a,m2b
                 do i1=m1a,m1b
                    ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
                    ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
                    xx=(i1-i1a)/real(i1b-i1a)
                    sigma1 = layerStrength*xx**power
                    ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
                    ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
                    yy=(i2-i2a)/real(i2b-i2a)
                    sigma2 = layerStrength*yy**power
                    ! zScale=xy(i1,i2,i3b,2)-xy(i1,i2,i3a,2)
                    ! zz=(xy(i1,i2,i3,2)-xy(i1,i2,i3a,2))/zScale
                    zz=(i3-i3a)/real(i3b-i3a)
                    sigma3 = layerStrength*zz**power
                   un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( u laplacian 23r(i1,i2,i3,ex) - vra x 23r(i1,i2,i3,ex) -
     & wra(i1,i2,i3,ex) - vsa y 23r(i1,i2,i3,ex) -wsa(i1,i2,i3,ex) - 
     & vta z 23r(i1,i2,i3,ex) -wta(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vra n(i1,i2,i3,ex)=vra(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -vra(i1,i2,i3,ex) +  u x 23r(i1,i2,i3,ex))-0.5*(-vra m(i1,i2,
     & i3,ex) + um x 23r(i1,i2,i3,ex)) )
                 wra n(i1,i2,i3,ex)=wra(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -wra(i1,i2,i3,ex)+  u xx 23r(i1,i2,i3,ex) - vra   x 23r(i1,i2,
     & i3,ex))-0.5*(-wra m(i1,i2,i3,ex)+ um xx 23r(i1,i2,i3,ex) - vra 
     & m x 23r(i1,i2,i3,ex)) )
                 vsa n(i1,i2,i3,ex)=vsa(i1,i2,i3,ex)+sigma2*dt*( 1.5*(-
     & vsa  (i1,i2,i3,ex) +  u y 23r(i1,i2,i3,ex))-0.5*(-vsa m(i1,i2,
     & i3,ex) + um y 23r(i1,i2,i3,ex)) )
                 wsa n(i1,i2,i3,ex)=wsa(i1,i2,i3,ex)+sigma2*dt*( 1.5*(-
     & wsa  (i1,i2,i3,ex)+ u  yy 23r(i1,i2,i3,ex) -vsa   y 23r(i1,i2,
     & i3,ex))-0.5*(-wsa m(i1,i2,i3,ex)+ um yy 23r(i1,i2,i3,ex) -vsa 
     & m y 23r(i1,i2,i3,ex)) )
                 vta n(i1,i2,i3,ex)=vta(i1,i2,i3,ex)+sigma3*dt*( 1.5*(-
     & vta  (i1,i2,i3,ex) +  u z 23r(i1,i2,i3,ex))-0.5*(-vta m(i1,i2,
     & i3,ex) + um z 23r(i1,i2,i3,ex)) )
                 wta n(i1,i2,i3,ex)=wta(i1,i2,i3,ex)+sigma3*dt*( 1.5*(-
     & wta  (i1,i2,i3,ex)+ u  zz 23r(i1,i2,i3,ex) -vta   z 23r(i1,i2,
     & i3,ex))-0.5*(-wta m(i1,i2,i3,ex)+ um zz 23r(i1,i2,i3,ex) -vta 
     & m z 23r(i1,i2,i3,ex)) )
                 end do
                 end do
                 end do
              else if( side1.eq.1 .and. side2.eq.0 .and. side3.eq.0 )
     & then
                 do i3=m3a,m3b
                 do i2=m2a,m2b
                 do i1=m1a,m1b
                    ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
                    ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
                    xx=(i1-i1a)/real(i1b-i1a)
                    sigma1 = layerStrength*xx**power
                    ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
                    ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
                    yy=(i2-i2a)/real(i2b-i2a)
                    sigma2 = layerStrength*yy**power
                    ! zScale=xy(i1,i2,i3b,2)-xy(i1,i2,i3a,2)
                    ! zz=(xy(i1,i2,i3,2)-xy(i1,i2,i3a,2))/zScale
                    zz=(i3-i3a)/real(i3b-i3a)
                    sigma3 = layerStrength*zz**power
                   un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( u laplacian 23r(i1,i2,i3,ex) - vrb x 23r(i1,i2,i3,ex) -
     & wrb(i1,i2,i3,ex) - vsa y 23r(i1,i2,i3,ex) -wsa(i1,i2,i3,ex) - 
     & vta z 23r(i1,i2,i3,ex) -wta(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vrb n(i1,i2,i3,ex)=vrb(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -vrb(i1,i2,i3,ex) +  u x 23r(i1,i2,i3,ex))-0.5*(-vrb m(i1,i2,
     & i3,ex) + um x 23r(i1,i2,i3,ex)) )
                 wrb n(i1,i2,i3,ex)=wrb(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -wrb(i1,i2,i3,ex)+  u xx 23r(i1,i2,i3,ex) - vrb   x 23r(i1,i2,
     & i3,ex))-0.5*(-wrb m(i1,i2,i3,ex)+ um xx 23r(i1,i2,i3,ex) - vrb 
     & m x 23r(i1,i2,i3,ex)) )
                 vsa n(i1,i2,i3,ex)=vsa(i1,i2,i3,ex)+sigma2*dt*( 1.5*(-
     & vsa  (i1,i2,i3,ex) +  u y 23r(i1,i2,i3,ex))-0.5*(-vsa m(i1,i2,
     & i3,ex) + um y 23r(i1,i2,i3,ex)) )
                 wsa n(i1,i2,i3,ex)=wsa(i1,i2,i3,ex)+sigma2*dt*( 1.5*(-
     & wsa  (i1,i2,i3,ex)+ u  yy 23r(i1,i2,i3,ex) -vsa   y 23r(i1,i2,
     & i3,ex))-0.5*(-wsa m(i1,i2,i3,ex)+ um yy 23r(i1,i2,i3,ex) -vsa 
     & m y 23r(i1,i2,i3,ex)) )
                 vta n(i1,i2,i3,ex)=vta(i1,i2,i3,ex)+sigma3*dt*( 1.5*(-
     & vta  (i1,i2,i3,ex) +  u z 23r(i1,i2,i3,ex))-0.5*(-vta m(i1,i2,
     & i3,ex) + um z 23r(i1,i2,i3,ex)) )
                 wta n(i1,i2,i3,ex)=wta(i1,i2,i3,ex)+sigma3*dt*( 1.5*(-
     & wta  (i1,i2,i3,ex)+ u  zz 23r(i1,i2,i3,ex) -vta   z 23r(i1,i2,
     & i3,ex))-0.5*(-wta m(i1,i2,i3,ex)+ um zz 23r(i1,i2,i3,ex) -vta 
     & m z 23r(i1,i2,i3,ex)) )
                 end do
                 end do
                 end do
              else if( side1.eq.0 .and. side2.eq.1 .and. side3.eq.0 )
     & then
                 do i3=m3a,m3b
                 do i2=m2a,m2b
                 do i1=m1a,m1b
                    ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
                    ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
                    xx=(i1-i1a)/real(i1b-i1a)
                    sigma1 = layerStrength*xx**power
                    ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
                    ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
                    yy=(i2-i2a)/real(i2b-i2a)
                    sigma2 = layerStrength*yy**power
                    ! zScale=xy(i1,i2,i3b,2)-xy(i1,i2,i3a,2)
                    ! zz=(xy(i1,i2,i3,2)-xy(i1,i2,i3a,2))/zScale
                    zz=(i3-i3a)/real(i3b-i3a)
                    sigma3 = layerStrength*zz**power
                   un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( u laplacian 23r(i1,i2,i3,ex) - vra x 23r(i1,i2,i3,ex) -
     & wra(i1,i2,i3,ex) - vsb y 23r(i1,i2,i3,ex) -wsb(i1,i2,i3,ex) - 
     & vta z 23r(i1,i2,i3,ex) -wta(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vra n(i1,i2,i3,ex)=vra(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -vra(i1,i2,i3,ex) +  u x 23r(i1,i2,i3,ex))-0.5*(-vra m(i1,i2,
     & i3,ex) + um x 23r(i1,i2,i3,ex)) )
                 wra n(i1,i2,i3,ex)=wra(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -wra(i1,i2,i3,ex)+  u xx 23r(i1,i2,i3,ex) - vra   x 23r(i1,i2,
     & i3,ex))-0.5*(-wra m(i1,i2,i3,ex)+ um xx 23r(i1,i2,i3,ex) - vra 
     & m x 23r(i1,i2,i3,ex)) )
                 vsb n(i1,i2,i3,ex)=vsb(i1,i2,i3,ex)+sigma2*dt*( 1.5*(-
     & vsb  (i1,i2,i3,ex) +  u y 23r(i1,i2,i3,ex))-0.5*(-vsb m(i1,i2,
     & i3,ex) + um y 23r(i1,i2,i3,ex)) )
                 wsb n(i1,i2,i3,ex)=wsb(i1,i2,i3,ex)+sigma2*dt*( 1.5*(-
     & wsb  (i1,i2,i3,ex)+ u  yy 23r(i1,i2,i3,ex) -vsb   y 23r(i1,i2,
     & i3,ex))-0.5*(-wsb m(i1,i2,i3,ex)+ um yy 23r(i1,i2,i3,ex) -vsb 
     & m y 23r(i1,i2,i3,ex)) )
                 vta n(i1,i2,i3,ex)=vta(i1,i2,i3,ex)+sigma3*dt*( 1.5*(-
     & vta  (i1,i2,i3,ex) +  u z 23r(i1,i2,i3,ex))-0.5*(-vta m(i1,i2,
     & i3,ex) + um z 23r(i1,i2,i3,ex)) )
                 wta n(i1,i2,i3,ex)=wta(i1,i2,i3,ex)+sigma3*dt*( 1.5*(-
     & wta  (i1,i2,i3,ex)+ u  zz 23r(i1,i2,i3,ex) -vta   z 23r(i1,i2,
     & i3,ex))-0.5*(-wta m(i1,i2,i3,ex)+ um zz 23r(i1,i2,i3,ex) -vta 
     & m z 23r(i1,i2,i3,ex)) )
                 end do
                 end do
                 end do
              else if( side1.eq.1 .and. side2.eq.1 .and. side3.eq.0 )
     & then
                 do i3=m3a,m3b
                 do i2=m2a,m2b
                 do i1=m1a,m1b
                    ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
                    ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
                    xx=(i1-i1a)/real(i1b-i1a)
                    sigma1 = layerStrength*xx**power
                    ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
                    ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
                    yy=(i2-i2a)/real(i2b-i2a)
                    sigma2 = layerStrength*yy**power
                    ! zScale=xy(i1,i2,i3b,2)-xy(i1,i2,i3a,2)
                    ! zz=(xy(i1,i2,i3,2)-xy(i1,i2,i3a,2))/zScale
                    zz=(i3-i3a)/real(i3b-i3a)
                    sigma3 = layerStrength*zz**power
                   un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( u laplacian 23r(i1,i2,i3,ex) - vrb x 23r(i1,i2,i3,ex) -
     & wrb(i1,i2,i3,ex) - vsb y 23r(i1,i2,i3,ex) -wsb(i1,i2,i3,ex) - 
     & vta z 23r(i1,i2,i3,ex) -wta(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vrb n(i1,i2,i3,ex)=vrb(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -vrb(i1,i2,i3,ex) +  u x 23r(i1,i2,i3,ex))-0.5*(-vrb m(i1,i2,
     & i3,ex) + um x 23r(i1,i2,i3,ex)) )
                 wrb n(i1,i2,i3,ex)=wrb(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -wrb(i1,i2,i3,ex)+  u xx 23r(i1,i2,i3,ex) - vrb   x 23r(i1,i2,
     & i3,ex))-0.5*(-wrb m(i1,i2,i3,ex)+ um xx 23r(i1,i2,i3,ex) - vrb 
     & m x 23r(i1,i2,i3,ex)) )
                 vsb n(i1,i2,i3,ex)=vsb(i1,i2,i3,ex)+sigma2*dt*( 1.5*(-
     & vsb  (i1,i2,i3,ex) +  u y 23r(i1,i2,i3,ex))-0.5*(-vsb m(i1,i2,
     & i3,ex) + um y 23r(i1,i2,i3,ex)) )
                 wsb n(i1,i2,i3,ex)=wsb(i1,i2,i3,ex)+sigma2*dt*( 1.5*(-
     & wsb  (i1,i2,i3,ex)+ u  yy 23r(i1,i2,i3,ex) -vsb   y 23r(i1,i2,
     & i3,ex))-0.5*(-wsb m(i1,i2,i3,ex)+ um yy 23r(i1,i2,i3,ex) -vsb 
     & m y 23r(i1,i2,i3,ex)) )
                 vta n(i1,i2,i3,ex)=vta(i1,i2,i3,ex)+sigma3*dt*( 1.5*(-
     & vta  (i1,i2,i3,ex) +  u z 23r(i1,i2,i3,ex))-0.5*(-vta m(i1,i2,
     & i3,ex) + um z 23r(i1,i2,i3,ex)) )
                 wta n(i1,i2,i3,ex)=wta(i1,i2,i3,ex)+sigma3*dt*( 1.5*(-
     & wta  (i1,i2,i3,ex)+ u  zz 23r(i1,i2,i3,ex) -vta   z 23r(i1,i2,
     & i3,ex))-0.5*(-wta m(i1,i2,i3,ex)+ um zz 23r(i1,i2,i3,ex) -vta 
     & m z 23r(i1,i2,i3,ex)) )
                 end do
                 end do
                 end do
              else if( side1.eq.0 .and. side2.eq.0 .and. side3.eq.1 )
     & then
                 do i3=m3a,m3b
                 do i2=m2a,m2b
                 do i1=m1a,m1b
                    ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
                    ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
                    xx=(i1-i1a)/real(i1b-i1a)
                    sigma1 = layerStrength*xx**power
                    ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
                    ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
                    yy=(i2-i2a)/real(i2b-i2a)
                    sigma2 = layerStrength*yy**power
                    ! zScale=xy(i1,i2,i3b,2)-xy(i1,i2,i3a,2)
                    ! zz=(xy(i1,i2,i3,2)-xy(i1,i2,i3a,2))/zScale
                    zz=(i3-i3a)/real(i3b-i3a)
                    sigma3 = layerStrength*zz**power
                   un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( u laplacian 23r(i1,i2,i3,ex) - vra x 23r(i1,i2,i3,ex) -
     & wra(i1,i2,i3,ex) - vsa y 23r(i1,i2,i3,ex) -wsa(i1,i2,i3,ex) - 
     & vtb z 23r(i1,i2,i3,ex) -wtb(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vra n(i1,i2,i3,ex)=vra(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -vra(i1,i2,i3,ex) +  u x 23r(i1,i2,i3,ex))-0.5*(-vra m(i1,i2,
     & i3,ex) + um x 23r(i1,i2,i3,ex)) )
                 wra n(i1,i2,i3,ex)=wra(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -wra(i1,i2,i3,ex)+  u xx 23r(i1,i2,i3,ex) - vra   x 23r(i1,i2,
     & i3,ex))-0.5*(-wra m(i1,i2,i3,ex)+ um xx 23r(i1,i2,i3,ex) - vra 
     & m x 23r(i1,i2,i3,ex)) )
                 vsa n(i1,i2,i3,ex)=vsa(i1,i2,i3,ex)+sigma2*dt*( 1.5*(-
     & vsa  (i1,i2,i3,ex) +  u y 23r(i1,i2,i3,ex))-0.5*(-vsa m(i1,i2,
     & i3,ex) + um y 23r(i1,i2,i3,ex)) )
                 wsa n(i1,i2,i3,ex)=wsa(i1,i2,i3,ex)+sigma2*dt*( 1.5*(-
     & wsa  (i1,i2,i3,ex)+ u  yy 23r(i1,i2,i3,ex) -vsa   y 23r(i1,i2,
     & i3,ex))-0.5*(-wsa m(i1,i2,i3,ex)+ um yy 23r(i1,i2,i3,ex) -vsa 
     & m y 23r(i1,i2,i3,ex)) )
                 vtb n(i1,i2,i3,ex)=vtb(i1,i2,i3,ex)+sigma3*dt*( 1.5*(-
     & vtb  (i1,i2,i3,ex) +  u z 23r(i1,i2,i3,ex))-0.5*(-vtb m(i1,i2,
     & i3,ex) + um z 23r(i1,i2,i3,ex)) )
                 wtb n(i1,i2,i3,ex)=wtb(i1,i2,i3,ex)+sigma3*dt*( 1.5*(-
     & wtb  (i1,i2,i3,ex)+ u  zz 23r(i1,i2,i3,ex) -vtb   z 23r(i1,i2,
     & i3,ex))-0.5*(-wtb m(i1,i2,i3,ex)+ um zz 23r(i1,i2,i3,ex) -vtb 
     & m z 23r(i1,i2,i3,ex)) )
                 end do
                 end do
                 end do
              else if( side1.eq.1 .and. side2.eq.0 .and. side3.eq.1 )
     & then
                 do i3=m3a,m3b
                 do i2=m2a,m2b
                 do i1=m1a,m1b
                    ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
                    ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
                    xx=(i1-i1a)/real(i1b-i1a)
                    sigma1 = layerStrength*xx**power
                    ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
                    ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
                    yy=(i2-i2a)/real(i2b-i2a)
                    sigma2 = layerStrength*yy**power
                    ! zScale=xy(i1,i2,i3b,2)-xy(i1,i2,i3a,2)
                    ! zz=(xy(i1,i2,i3,2)-xy(i1,i2,i3a,2))/zScale
                    zz=(i3-i3a)/real(i3b-i3a)
                    sigma3 = layerStrength*zz**power
                   un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( u laplacian 23r(i1,i2,i3,ex) - vrb x 23r(i1,i2,i3,ex) -
     & wrb(i1,i2,i3,ex) - vsa y 23r(i1,i2,i3,ex) -wsa(i1,i2,i3,ex) - 
     & vtb z 23r(i1,i2,i3,ex) -wtb(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vrb n(i1,i2,i3,ex)=vrb(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -vrb(i1,i2,i3,ex) +  u x 23r(i1,i2,i3,ex))-0.5*(-vrb m(i1,i2,
     & i3,ex) + um x 23r(i1,i2,i3,ex)) )
                 wrb n(i1,i2,i3,ex)=wrb(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -wrb(i1,i2,i3,ex)+  u xx 23r(i1,i2,i3,ex) - vrb   x 23r(i1,i2,
     & i3,ex))-0.5*(-wrb m(i1,i2,i3,ex)+ um xx 23r(i1,i2,i3,ex) - vrb 
     & m x 23r(i1,i2,i3,ex)) )
                 vsa n(i1,i2,i3,ex)=vsa(i1,i2,i3,ex)+sigma2*dt*( 1.5*(-
     & vsa  (i1,i2,i3,ex) +  u y 23r(i1,i2,i3,ex))-0.5*(-vsa m(i1,i2,
     & i3,ex) + um y 23r(i1,i2,i3,ex)) )
                 wsa n(i1,i2,i3,ex)=wsa(i1,i2,i3,ex)+sigma2*dt*( 1.5*(-
     & wsa  (i1,i2,i3,ex)+ u  yy 23r(i1,i2,i3,ex) -vsa   y 23r(i1,i2,
     & i3,ex))-0.5*(-wsa m(i1,i2,i3,ex)+ um yy 23r(i1,i2,i3,ex) -vsa 
     & m y 23r(i1,i2,i3,ex)) )
                 vtb n(i1,i2,i3,ex)=vtb(i1,i2,i3,ex)+sigma3*dt*( 1.5*(-
     & vtb  (i1,i2,i3,ex) +  u z 23r(i1,i2,i3,ex))-0.5*(-vtb m(i1,i2,
     & i3,ex) + um z 23r(i1,i2,i3,ex)) )
                 wtb n(i1,i2,i3,ex)=wtb(i1,i2,i3,ex)+sigma3*dt*( 1.5*(-
     & wtb  (i1,i2,i3,ex)+ u  zz 23r(i1,i2,i3,ex) -vtb   z 23r(i1,i2,
     & i3,ex))-0.5*(-wtb m(i1,i2,i3,ex)+ um zz 23r(i1,i2,i3,ex) -vtb 
     & m z 23r(i1,i2,i3,ex)) )
                 end do
                 end do
                 end do
              else if( side1.eq.0 .and. side2.eq.1 .and. side3.eq.1 )
     & then
                 do i3=m3a,m3b
                 do i2=m2a,m2b
                 do i1=m1a,m1b
                    ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
                    ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
                    xx=(i1-i1a)/real(i1b-i1a)
                    sigma1 = layerStrength*xx**power
                    ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
                    ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
                    yy=(i2-i2a)/real(i2b-i2a)
                    sigma2 = layerStrength*yy**power
                    ! zScale=xy(i1,i2,i3b,2)-xy(i1,i2,i3a,2)
                    ! zz=(xy(i1,i2,i3,2)-xy(i1,i2,i3a,2))/zScale
                    zz=(i3-i3a)/real(i3b-i3a)
                    sigma3 = layerStrength*zz**power
                   un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( u laplacian 23r(i1,i2,i3,ex) - vra x 23r(i1,i2,i3,ex) -
     & wra(i1,i2,i3,ex) - vsb y 23r(i1,i2,i3,ex) -wsb(i1,i2,i3,ex) - 
     & vtb z 23r(i1,i2,i3,ex) -wtb(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vra n(i1,i2,i3,ex)=vra(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -vra(i1,i2,i3,ex) +  u x 23r(i1,i2,i3,ex))-0.5*(-vra m(i1,i2,
     & i3,ex) + um x 23r(i1,i2,i3,ex)) )
                 wra n(i1,i2,i3,ex)=wra(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -wra(i1,i2,i3,ex)+  u xx 23r(i1,i2,i3,ex) - vra   x 23r(i1,i2,
     & i3,ex))-0.5*(-wra m(i1,i2,i3,ex)+ um xx 23r(i1,i2,i3,ex) - vra 
     & m x 23r(i1,i2,i3,ex)) )
                 vsb n(i1,i2,i3,ex)=vsb(i1,i2,i3,ex)+sigma2*dt*( 1.5*(-
     & vsb  (i1,i2,i3,ex) +  u y 23r(i1,i2,i3,ex))-0.5*(-vsb m(i1,i2,
     & i3,ex) + um y 23r(i1,i2,i3,ex)) )
                 wsb n(i1,i2,i3,ex)=wsb(i1,i2,i3,ex)+sigma2*dt*( 1.5*(-
     & wsb  (i1,i2,i3,ex)+ u  yy 23r(i1,i2,i3,ex) -vsb   y 23r(i1,i2,
     & i3,ex))-0.5*(-wsb m(i1,i2,i3,ex)+ um yy 23r(i1,i2,i3,ex) -vsb 
     & m y 23r(i1,i2,i3,ex)) )
                 vtb n(i1,i2,i3,ex)=vtb(i1,i2,i3,ex)+sigma3*dt*( 1.5*(-
     & vtb  (i1,i2,i3,ex) +  u z 23r(i1,i2,i3,ex))-0.5*(-vtb m(i1,i2,
     & i3,ex) + um z 23r(i1,i2,i3,ex)) )
                 wtb n(i1,i2,i3,ex)=wtb(i1,i2,i3,ex)+sigma3*dt*( 1.5*(-
     & wtb  (i1,i2,i3,ex)+ u  zz 23r(i1,i2,i3,ex) -vtb   z 23r(i1,i2,
     & i3,ex))-0.5*(-wtb m(i1,i2,i3,ex)+ um zz 23r(i1,i2,i3,ex) -vtb 
     & m z 23r(i1,i2,i3,ex)) )
                 end do
                 end do
                 end do
              else if( side1.eq.1 .and. side2.eq.1 .and. side3.eq.1 )
     & then
                 do i3=m3a,m3b
                 do i2=m2a,m2b
                 do i1=m1a,m1b
                    ! xScale=xy(i1b,i2,i3,0)-xy(i1a,i2,i3,0)
                    ! xx=(xy(i1,i2,i3,0)-xy(i1a,i2,i3,0))/xScale
                    xx=(i1-i1a)/real(i1b-i1a)
                    sigma1 = layerStrength*xx**power
                    ! yScale=xy(i1,i2b,i3,1)-xy(i1,i2a,i3,1)
                    ! yy=(xy(i1,i2,i3,1)-xy(i1,i2a,i3,1))/yScale
                    yy=(i2-i2a)/real(i2b-i2a)
                    sigma2 = layerStrength*yy**power
                    ! zScale=xy(i1,i2,i3b,2)-xy(i1,i2,i3a,2)
                    ! zz=(xy(i1,i2,i3,2)-xy(i1,i2,i3a,2))/zScale
                    zz=(i3-i3a)/real(i3b-i3a)
                    sigma3 = layerStrength*zz**power
                   un(i1,i2,i3,ex)=2.*u(i1,i2,i3,ex)-um(i1,i2,i3,ex) + 
     & cdtsq*( u laplacian 23r(i1,i2,i3,ex) - vrb x 23r(i1,i2,i3,ex) -
     & wrb(i1,i2,i3,ex) - vsb y 23r(i1,i2,i3,ex) -wsb(i1,i2,i3,ex) - 
     & vtb z 23r(i1,i2,i3,ex) -wtb(i1,i2,i3,ex) )
                 ! auxilliary variables       
                 vrb n(i1,i2,i3,ex)=vrb(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -vrb(i1,i2,i3,ex) +  u x 23r(i1,i2,i3,ex))-0.5*(-vrb m(i1,i2,
     & i3,ex) + um x 23r(i1,i2,i3,ex)) )
                 wrb n(i1,i2,i3,ex)=wrb(i1,i2,i3,ex)+sigma1*dt*( 1.5*( 
     &  -wrb(i1,i2,i3,ex)+  u xx 23r(i1,i2,i3,ex) - vrb   x 23r(i1,i2,
     & i3,ex))-0.5*(-wrb m(i1,i2,i3,ex)+ um xx 23r(i1,i2,i3,ex) - vrb 
     & m x 23r(i1,i2,i3,ex)) )
                 vsb n(i1,i2,i3,ex)=vsb(i1,i2,i3,ex)+sigma2*dt*( 1.5*(-
     & vsb  (i1,i2,i3,ex) +  u y 23r(i1,i2,i3,ex))-0.5*(-vsb m(i1,i2,
     & i3,ex) + um y 23r(i1,i2,i3,ex)) )
                 wsb n(i1,i2,i3,ex)=wsb(i1,i2,i3,ex)+sigma2*dt*( 1.5*(-
     & wsb  (i1,i2,i3,ex)+ u  yy 23r(i1,i2,i3,ex) -vsb   y 23r(i1,i2,
     & i3,ex))-0.5*(-wsb m(i1,i2,i3,ex)+ um yy 23r(i1,i2,i3,ex) -vsb 
     & m y 23r(i1,i2,i3,ex)) )
                 vtb n(i1,i2,i3,ex)=vtb(i1,i2,i3,ex)+sigma3*dt*( 1.5*(-
     & vtb  (i1,i2,i3,ex) +  u z 23r(i1,i2,i3,ex))-0.5*(-vtb m(i1,i2,
     & i3,ex) + um z 23r(i1,i2,i3,ex)) )
                 wtb n(i1,i2,i3,ex)=wtb(i1,i2,i3,ex)+sigma3*dt*( 1.5*(-
     & wtb  (i1,i2,i3,ex)+ u  zz 23r(i1,i2,i3,ex) -vtb   z 23r(i1,i2,
     & i3,ex))-0.5*(-wtb m(i1,i2,i3,ex)+ um zz 23r(i1,i2,i3,ex) -vtb 
     & m z 23r(i1,i2,i3,ex)) )
                 end do
                 end do
                 end do
              else
                stop 6224
              end if
             else
               stop 23415
             end if

           ! Trouble with the fourth-order version: far corners go unstable
           ! advanceEdgesAndCorners3dOrder4()

         end if

        end if ! end nd==3

       else  ! end rectangular 4th order
         stop 22555
       end if
      end do  ! numberOfBoxes

      ! Apply zero BCs for pml 
      do axis=0,nd-1
      do side=0,1
        m1a=nd1a
        m1b=nd1b
        m2a=nd2a
        m2b=nd2b
        m3a=nd3a
        m3b=nd3b
        if( boundaryCondition(side,axis).eq.abcPML )then
          if(      side.eq.0 .and. axis.eq.0 )then
            m1a=nd1a
            m1b=md1a
          else if( side.eq.1 .and. axis.eq.0 )then
            m1a=md1b
            m1b=nd1b
          else if( side.eq.0 .and. axis.eq.1 )then
            m2a=nd2a
            m2b=md2a
          else if( side.eq.1 .and. axis.eq.1 )then
            m2a=md2b
            m2b=nd2b
          else if( side.eq.0 .and. axis.eq.2 )then
            m3a=nd3a
            m3b=md3a
          else if( side.eq.1 .and. axis.eq.2 )then
            m3a=md3b
            m3b=nd3b
          end if
          if( nd.eq.2 )then
            do i3=m3a,m3b
            do i2=m2a,m2b
            do i1=m1a,m1b
             un(i1,i2,i3,ex)=0.
           end do
           end do
           end do
          else
            do i3=m3a,m3b
            do i2=m2a,m2b
            do i1=m1a,m1b
             un(i1,i2,i3,ex)=0.
           end do
           end do
           end do
          end if
        end if
      end do
      end do


      return
      end




