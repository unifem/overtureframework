      subroutine solid_step_ol( nxl,nxu,nyl,nyu,nzl,nzu,dt,R,Y0,p0,
     *                       c0,cl,hgVisc,lemu,lelambda,hgFlag,m,x,y,
     *                       vx,vy,sx,sy,txy,p,q,
     *                       rho0,e0,
     *                       phi,xold,yold,vx_temp,vy_temp,
     *                       vxold,vyold,
     *                       sigma_x,sigma_y,Area,
     *                       apr,bpr,cpr,dpr,lamMax,vismax,ilinear,
     *                       boundaryCondition,dim,bcf0,bcOffset )
c
c..declarations of incomming variables 
      implicit none
      integer nxl,nxu,nyl,nyu,nzl,nzu,hgFlag,ilinear
      real dt,R,Y0,x,y,vx,vy,sx,sy,txy,p,q,m,rho0,e0,p0,c0,cl,hgVisc,
     *     phi,xold,yold,vx_temp,vy_temp,sigma_x,
     *     sigma_y,Area,
     *     lamMax,apr,bpr,cpr,dpr,vxold,vyold,
     *     lemu,lelambda,vismax
      dimension x(nxl:nxu,nyl:nyu),y(nxl:nxu,nyl:nyu),
     *          vx(nxl:nxu,nyl:nyu),vy(nxl:nxu,nyl:nyu),
     *          sx(nxl:nxu-1,nyl:nyu-1),sy(nxl:nxu-1,nyl:nyu-1),
     *          txy(nxl:nxu-1,nyl:nyu-1),p(nxl:nxu-1,nyl:nyu-1),
     *          q(nxl:nxu-1,nyl:nyu-1),m(nxl:nxu-1,nyl:nyu-1),
     *          rho0(nxl:nxu-1,nyl:nyu-1),e0(nxl:nxu-1,nyl:nyu-1),
     *          phi(nxl:nxu,nyl:nyu),
     *          xold(nxl:nxu,nyl:nyu),yold(nxl:nxu,nyl:nyu),
     *          vx_temp(nxl:nxu,nyl:nyu),vy_temp(nxl:nxu,nyl:nyu),
     *          sigma_x(nxl:nxu-1,nyl:nyu-1),
     *          sigma_y(nxl:nxu-1,nyl:nyu-1),
     *          Area(nxl:nxu-1,nyl:nyu-1)
      dimension vxold(nxl:nxu,nyl:nyu),vyold(nxl:nxu,nyl:nyu)
c
      integer boundaryCondition(1:2,1:3)
      integer dim(1:2,1:3,1:2,1:3)
      real bcf0(1:*)
      integer*8 bcOffset(1:2,1:3)
c
c..declarations of local variables
      real eps_dot_x,eps_dot_y,eps_dot_xy,press_total,
     *     xI,xII,xIII,xIV,
     *     yI,yII,yIII,yIV,
     *     x2mx3,x3mx4,x4mx1,x1mx2,
     *     y2my3,y3my4,y4my1,y1my2,
     *     x1,x2,x3,x4,
     *     y1,y2,y3,y4,
     *     rho,V,
     *     vx2mvx4,vx3mvx1,vy2mvy4,vy3mvy1,
     *     y2my4,y3my1,x2mx4,x3mx1,
     *     dvxdx,dvydy,dvxdy,dvydx,
     *     sx_new,sy_new,txy_new,
     *     temperature,mu,mult,a2,eng,
     *     e_tot_o,e_tot_n,p_cent,junk,pmin,tmp,
     *     div,vort,al1,al2,kap,dx,dy,alx,aly,
     *     e_dot,rho_dot,p_dot,
     *     e_r,e_p,p_r,p_re
c
      integer i,j,k
      data pmin / 1.e-12 /
c
      include 'bcDefineFortranInclude.h'
      real bcf
      integer kd,ks
      integer i1,i2,i3
c
c     --- start statement functions ----

      ! Here is the the generic boundary condition forcing array. It uses the bcOffset(side,axis) values as an
      ! an offset from the bcf0 array to access the bcf10, bcf01, bcf11, ... arrays
      bcf(ks,kd,i1,i2,i3,k) = bcf0(bcOffset(ks,kd)+1+
     & (i1-dim(1,1,ks,kd)+(dim(2,1,ks,kd)-dim(1,1,ks,kd)+1)* 
     & (i2-dim(1,2,ks,kd)+(dim(2,2,ks,kd)-dim(1,2,ks,kd)+1)* 
     & (i3-dim(1,3,ks,kd)+(dim(2,3,ks,kd)-dim(1,3,ks,kd)+1)*(k-1)))))
c    --- end statement functions ----
c
c
c..compute phi (this is a kind of nodal mass)
c   Probably we could compute this once and for all but for now
c   I just wanted to get it done so ...
      do j = nyl+1,nyu-1
        do i = nxl+1,nxu-1
          phi(i,j) = 0.25e0*(m(i,j)+m(i-1,j)+m(i-1,j-1)+m(i,j-1))
        end do
      end do
      do i = nxl+1,nxu-1
        phi(i,nyl) = 0.25e0*(m(i,nyl)+m(i-1,nyl))
        phi(i,nyu) = 0.25e0*(m(i-1,nyu-1)+m(i,nyu-1))
c        phi(i,nyl) = 0.5e0*(m(i,nyl)+m(i-1,nyl))
c        phi(i,nyu) = 0.5e0*(m(i-1,nyu-1)+m(i,nyu-1))
      end do
      do j = nyl+1,nyu-1
        phi(nxl,j) = 0.25e0*(m(nxl,j)+m(nxl,j-1))
        phi(nxu,j) = 0.25e0*(m(nxu-1,j)+m(nxu-1,j-1))
c        phi(nxl,j) = 0.5e0*(m(nxl,j)+m(nxl,j-1))
c        phi(nxu,j) = 0.5e0*(m(nxu-1,j)+m(nxu-1,j-1))
      end do
      phi(nxl,nyl) = 0.25e0*m(nxl+1,nyl+1)
      phi(nxl,nyu) = 0.25e0*m(nxl+1,nyu-1)
      phi(nxu,nyl) = 0.25e0*m(nxu-1,nyl+1)
      phi(nxu,nyu) = 0.25e0*m(nxu-1,nyu-1)
c      phi(nxl,nyl) = 1.0e0*m(nxl+1,nyl+1)
c      phi(nxl,nyu) = 1.0e0*m(nxl+1,nyu-1)
c      phi(nxu,nyl) = 1.0e0*m(nxu-1,nyl+1)
c      phi(nxu,nyu) = 1.0e0*m(nxu-1,nyu-1)
c
c..Compute areas of the current mesh
      do j = nyl,nyu-1
        do  i = nxl,nxu-1
          x1 = x(i,j)
          y1 = y(i,j)
          x2 = x(i+1,j)
          y2 = y(i+1,j)
          x3 = x(i+1,j+1)
          y3 = y(i+1,j+1)
          x4 = x(i,j+1)
          y4 = y(i,j+1)

          call getArea( x1,x2,x3,x4,y1,y2,y3,y4,Area(i,j) )
        end do
      end do
c
c..set artificial viscosity
      call setQ( nxl,nxu,nyl,nyu,x,y,vx,vy,Area,
     *           m,rho0,e0,p,q,sx,sy,c0,cl,R,
     *           apr,bpr,cpr,dpr,lemu,lelambda,ilinear,vismax )
c
c..set diagonal stress components at nodes 
      do j = nyl,nyu-1
        do i = nxl,nxu-1
          press_total = p(i,j)+q(i,j)
          sigma_x(i,j) = -press_total+sx(i,j)
          sigma_y(i,j) = -press_total+sy(i,j)
        end do
      end do
c
c..copy off velocities
      do j = nyl,nyu
        do i = nxl,nxu
          vxold(i,j) = vx(i,j)
          vyold(i,j) = vy(i,j)
        end do
      end do

c..compute new velocities
      ! first loop over interior nodes
      do j = nyl+1,nyu-1
        do i = nxl+1,nxu-1
          ! first set positions I,II,III,IV
          xI = x(i,j-1)
          yI = y(i,j-1)
c          
          xII = x(i+1,j)
          yII = y(i+1,j)
c
          xIII = x(i,j+1)
          yIII = y(i,j+1)
c
          xIV = x(i-1,j)
          yIV = y(i-1,j)
c
          ! compute required differences for integration path
          x2mx3 = xII-xIII
          x3mx4 = xIII-xIV
          x4mx1 = xIV-xI
          x1mx2 = xI-xII
          y2my3 = yII-yIII
          y3my4 = yIII-yIV
          y4my1 = yIV-yI
          y1my2 = yI-yII

          ! finally update the velocities
          vx(i,j) = vx(i,j)-dt/(2.e0*phi(i,j))*(
     *                         (sigma_x(i,j))*y2my3+
     *                         (sigma_x(i-1,j))*y3my4+
     *                         (sigma_x(i-1,j-1))*y4my1+
     *                         (sigma_x(i,j-1))*y1my2-
     *                         (txy(i,j))*x2mx3-
     *                         (txy(i-1,j))*x3mx4-
     *                         (txy(i-1,j-1))*x4mx1-
     *                         (txy(i,j-1))*x1mx2)

          vy(i,j) = vy(i,j)+dt/(2.e0*phi(i,j))*(
     *                         (sigma_y(i,j))*x2mx3+
     *                         (sigma_y(i-1,j))*x3mx4+
     *                         (sigma_y(i-1,j-1))*x4mx1+
     *                         (sigma_y(i,j-1))*x1mx2-
     *                         (txy(i,j))*y2my3-
     *                         (txy(i-1,j))*y3my4-
     *                         (txy(i-1,j-1))*y4my1-
     *                         (txy(i,j-1))*y1my2)
        end do
      end do
c
c.. deal with BCs
      call hemp_bcs( nxl,nxu,nyl,nyu,x,y,vx,vy,
     *   sigma_x,sigma_y,txy,phi,dt,p0,
     *   boundaryCondition,dim,bcf0,bcOffset )
c
c.. Do hourglass control
      call hemp_hg( nxl,nxu,nyl,nyu,vx,vy,vxold,vyold,
     *   vx_temp,vy_temp,phi,dt,hgFlag,hgVisc )
c
c..Compute new node positions (at new time)
      do j = nyl,nyu
        do i = nxl,nxu
          ! while we are at it we might as well store off the
          ! old positions and velocities and make the most of the loops
          xold(i,j) = x(i,j)
          yold(i,j) = y(i,j)
c
          x(i,j)=xold(i,j)+vxold(i,j)*dt
          y(i,j)=yold(i,j)+vyold(i,j)*dt
        end do
      end do
c
c..Update all the stresses, pressure, and internal energy
      lamMax = 0.0
      do j = nyl,nyu-1
        do  i = nxl,nxu-1
          ! compute specific volumes
          rho = m(i,j)/Area(i,j)
          V = rho0(i,j)/rho

          ! compute strain rates
          vx2mvx4 = vxold(i+1,j)  -vxold(i,j+1)
          vx3mvx1 = vxold(i+1,j+1)-vxold(i,j)
          y2my4   = yold(i+1,j)   -yold(i,j+1)
          y3my1   = yold(i+1,j+1) -yold(i,j)
          vy2mvy4 = vyold(i+1,j)  -vyold(i,j+1)
          vy3mvy1 = vyold(i+1,j+1)-vyold(i,j)
          x2mx4   = xold(i+1,j)   -xold(i,j+1)
          x3mx1   = xold(i+1,j+1) -xold(i,j)

          tmp = 1.e0/(2.e0*Area(i,j))
          dvxdx =  (vx2mvx4*y3my1-y2my4*vx3mvx1)*tmp
          dvydy = -(vy2mvy4*x3mx1-x2mx4*vy3mvy1)*tmp
          dvxdy = -(vx2mvx4*x3mx1-x2mx4*vx3mvx1)*tmp
          dvydx =  (vy2mvy4*y3my1-y2my4*vy3mvy1)*tmp

          ! compute specific volumes
          rho = m(i,j)/Area(i,j)
          V = rho0(i,j)/rho

          ! compute stresses
          div  = dvxdx+dvydy
          vort = dvxdy-dvydx

          if( ilinear.eq.1 ) then
            mu = lemu
            vort = 0.0
          else
            call getEng( 1.e0/V, p(i,j),eng,apr,bpr,cpr,dpr )
            temperature = ((eng-e0(i,j))/rho0(i,j))/(3.e0*R)
            call getMu( p(i,j),V,temperature,mu )
          end if

          sx_new = sx(i,j)+2.e0*mu*dt*(dvxdx-div/3.e0)-
     *       dt*vort*txy(i,j)
          sy_new = sy(i,j)+2.e0*mu*dt*(dvydy-div/3.e0)+
     *       dt*vort*txy(i,j)
          txy_new = txy(i,j)+mu*dt*(dvxdy+dvydx)-
     *       0.5*dt*vort*(sy(i,j)-sx(i,j))

          ! check Von Misis yield condition
          K = (sx_new**2+sy_new**2+2.e0*txy_new**2)-2.e0/3.e0*Y0**2
          if( K.gt.0.e0 ) then
            mult = Y0*sqrt(2.e0/(3.e0*
     *         (sx_new**2+sy_new**2+2.e0*txy_new**2)))
            sx_new = sx_new*mult
            sy_new = sy_new*mult
            txy_new = txy_new*mult
          end if

          if( ilinear.eq.1 ) then 
            ! linear elasticity
            kap = lelambda+mu*2.0/3.0
            p(i,j) = p(i,j)-dt*kap*div

            dx = abs(Area(i,j)/(max(abs(y3my1),abs(y2my4))))
            dy = abs(Area(i,j)/(max(abs(x3mx1),abs(x2mx4))))

            al1 = sqrt((3.0*kap+4.0*mu)/(3.0*rho))
c            al2 = sqrt((2.0*mu+sy(i,j)-sx(i,j))/(2.0*rho))
            al2 = sqrt((2.0*mu)/(2.0*rho))
            alx = max( al1,al2 )

            al1 = sqrt((3.0*kap+4.0*mu)/(3.0*rho))
c            al2 = sqrt((2.0*mu+sx(i,j)-sy(i,j))/(2.0*rho))
            al2 = sqrt((2.0*mu)/(2.0*rho))
            aly = max( al1,al2 )

            lamMax = max( lamMax,alx/dx+aly/dy )
          else
            ! evolve energy
            call getEOSDerivs( rho,rho0(i,j),p(i,j),eng,
     *                         apr,bpr,cpr,dpr,
     *                         p_r,p_re,e_r,e_p )
            press_total = p(i,j)+q(i,j)
            e_dot = V*(-press_total*div+
     *         sx(i,j)*dvxdx+
     *         sy(i,j)*dvydy+
     *         txy(i,j)*(dvydx+dvxdy))
            rho_dot = -rho*div
            p_dot = (e_dot-e_r*rho_dot)/(e_p)
            p(i,j) = p(i,j)+dt*p_dot

            dx = abs(Area(i,j)/(max(abs(y3my1),abs(y2my4))))
            dy = abs(Area(i,j)/(max(abs(x3mx1),abs(x2mx4))))
            a2 = p_r+p_re*(eng+p(i,j)/rho)
            ! notice we are being brave and not checking for negative arguments ... be careful
            al1 = sqrt( (2.0*mu-sx(i,j)+sy(i,j))/(2.0*rho) )
            al2 = sqrt( (4.0*mu/3.0-p_re*sx(i,j))/rho+a2 )
            alx = max( al1,al2 )

            al1 = sqrt( (2.0*mu-sy(i,j)+sx(i,j))/(2.0*rho) )
            al2 = sqrt( (4.0*mu/3.0-p_re*sy(i,j))/rho+a2 )
            aly = max( al1,al2 )
            lamMax = max( lamMax,alx/dx+aly/dy )

c            e_tilde = eng+V*dt*(-press_total*div+
c     *         sx(i,j)*dvxdx+
c     *         sy(i,j)*dvydy+
c     *         txy(i,j)*(dvydx+dvxdy))
c            ! compute new zone specific volume
c            x1 = x(i,j)
c            y1 = y(i,j)
c            x2 = x(i+1,j)
c            y2 = y(i+1,j)
c            x3 = x(i+1,j+1)
c            y3 = y(i+1,j+1)
c            x4 = x(i,j+1)
c            y4 = y(i,j+1)
c            call getArea( x1,x2,x3,x4,y1,y2,y3,y4,Area_new )
c            rho_new = m(i,j)/Area_new
c            V_new = rho0(i,j)/rho_new
c
c            ! compute updated pressure
c            call getPress( 1.e0/V_new,e_tilde,p(i,j),apr,bpr,cpr,dpr )
          end if

          ! update quantities
          sx(i,j) = sx_new
          sy(i,j) = sy_new
          txy(i,j) = txy_new
        end do
      end do
      
      return
      end
