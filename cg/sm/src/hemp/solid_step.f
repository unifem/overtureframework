      subroutine solid_step( nxl,nxu,nyl,nyu,nzl,nzu,dt,R,Y0,p0,c0,cl,
     *                       hgVisc,lemu,lelambda,
     *                       hgFlag,m,x,y,
     *                       vx,vy,sx,sy,txy,p,q,
     *                       rho0,e0,
     *                       phi,xold,yold,vx_temp,vy_temp,
     *                       vxold,vyold,
     *                       sigma_x,sigma_y,Area,
     *                       apr,bpr,cpr,dpr,lamMax,vismax,ilinear,
     *                       boundaryCondition,dim,bcf0,bcOffset )
ccccccccc
c This is the routine to compute the actual Hemp update
ccccccccc
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
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c     nxl  -- x points lower bound
c     nxu  -- x points upper bound
c     nyl  -- y points lower bound
c     nyu  -- y points upper bound
c     dt   -- time step (for now a single time step broken into two equal halves)
c     R    -- gas constant
c     Y0   -- yield strength (constant)
c     hgFlag -- hourglass control flag
c     m    -- zonal masses
c     x    -- nodal x locations ... including ghost nodes (at t_n)
c     y    -- nodal y locations ... including ghost nodes (at t_n)
c     vx   -- nodal x-velocities (at t_n+1/2)
c     vy   -- nodal y-velocities (at t_n+1/2)
c     sx   -- zonal x-deviatoric stress (at t_n)
c     sy   -- zonal y-deviatoric stress (at t_n)
c     txy  -- zonal off diagonal deviatoric stress (at t_n)
c     p    -- zonal pressure (at t_n)
c     q    -- zonal artificial viscosity (at t_n)
c     rho0 -- reference density
c     e0   -- reference internal energy
c     phi  -- temporary nodal mass
c     xold -- temporary old x positions
c     yold -- temporary old y positions
c     vx_temp -- temporary storage for x-velocities (Margolin's filter)
c     vy_temp -- temporary storage for y-velocities (Margolin's filter)
c     sigma_x -- temporary storage for diagonal component of stress tensor
c     sigma_y -- temporary storage for diagonal component of stress tensor
c     Area    -- temporary storage for old Area
c     lamMax -- maximum value for a1/dx+a2/dy where a1,a2 is the wave speed and dx,dy are characteristic cell spacings
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c..declarations of local variables
      real eps_dot_x,eps_dot_y,eps_dot_xy,press_total,
     *     xI,xII,xIII,xIV,
     *     yI,yII,yIII,yIV,
     *     x2mx3,x3mx4,x4mx1,x1mx2,
     *     y2my3,y3my4,y4my1,y1my2,
     *     x1,x2,x3,x4,
     *     y1,y2,y3,y4,
     *     Aa,Ab,Area_new,Area_half,
     *     rho,rho_new,V,V_new,V_half,grad_V,
     *     vx2mvx4,vx3mvx1,vy2mvy4,vy3mvy1,
     *     y2my4,y3my1,x2mx4,x3mx1,
     *     dvxdx,dvydy,dvxdy,dvydx,
     *     co,so,sx_temp,sy_temp,txy_temp,
     *     sx_new,sy_new,txy_new,
     *     sx_half,sy_half,txy_half,
     *     temperature,mu,mult,
     *     a,d1,d2,rad,dvel,q1,q2,q3,q4,q_new,
     *     q_bar,dz,e_tilde,p_tilde,
     *     eng,radmin,
     *     dvx,dvy,dmx,dmy,delta_e,delta_p,
     *     e_tot_o,e_tot_n,p_cent,junk,
     *     fx,fy,fx0,fy0,yieldTest,tmp
c
      integer i,j,k
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c     c0          -- constant for quadratic q
c     cl          -- constant for linear q
c     eps_dot_x   -- strain rate
c     eps_dot_y   -- strain rate
c     eps_dot_xy  -- strain rate
c     press_total -- total pressure (including artificial viscosity)
c     xI          -- position used for velocity updates
c     xII         -- position used for velocity updates
c     xIII        -- position used for velocity updates
c     xIV         -- position used for velocity updates
c     yI          -- position used for velocity updates
c     yII         -- position used for velocity updates
c     yIII        -- position used for velocity updates
c     yIV         -- position used for velocity updates
c     x2mx3       -- difference used for velocity updates
c     x3mx4       -- difference used for velocity updates
c     x4mx1       -- difference used for velocity updates
c     x1mx2       -- difference used for velocity updates
c     y2my3       -- difference used for velocity updates
c     y3my4       -- difference used for velocity updates
c     y4my1       -- difference used for velocity updates
c     y1my2       -- difference used for velocity updates
c     x1          -- velocity used for stress updates
c     x2          -- velocity used for stress updates
c     x3          -- velocity used for stress updates
c     x4          -- velocity used for stress updates
c     x1          -- velocity used for stress updates
c     x2          -- velocity used for stress updates
c     x3          -- velocity used for stress updates
c     x4          -- velocity used for stress updates
c     Aa          -- trinagle area
c     Ab          -- triangle area
c     Area_new    -- updated area
c     Area_half   -- half time area
c     rho         -- density
c     rho_new     -- new density
c     V           -- specific volume
c     V_new       -- new specific volume
c     V_half      -- half time specific volume
c     grad_V      -- specific volume gradient
c     vx2mvx4     -- difference used for stress updates
c     vx3mvx1     -- difference used for stress updates
c     vy2mvy4     -- difference used for stress updates
c     vy3mvy1     -- difference used for stress updates
c     y2my4       -- difference used for stress updates
c     y3my1       -- difference used for stress updates
c     x2mx4       -- difference used for stress updates
c     x3mx1       -- difference used for stress updates
c     dvxdx       -- velocity derivative for stress updates
c     dvydy       -- velocity derivative for stress updates
c     dvxdy       -- velocity derivative for stress updates
c     dvydx       -- velocity derivative for stress updates
c     co          -- cosine of rotation angle
c     so          -- sine of rotation angle
c     sx_temp     -- temp stress for rotation correction
c     sy_temp     -- temp stress for rotation correction
c     txy_temp    -- temp stress for rotation correction
c     sx_new      -- new stress
c     sy_new      -- new stress
c     txy_new     -- new stress
c     sx_half     -- half time stress
c     sy_half     -- half time stress
c     txy_half    -- half time stres
c     temperature -- you should know ...
c     mu          -- material quantity (perhaps from Steinberg-Guinan model)
c     mult        -- yeild multiplier
c     a           -- sound speed
c     d1          -- direction component
c     d2          -- direction component
c     rad         -- direction size
c     dvel        -- velocity difference (for q calculation)
c     q1          -- q along edge
c     q2          -- q along edge
c     q3          -- q along edge
c     q4          -- q along edge
c     q_new       -- updated q
c     q_bar       -- average q
c     dz          -- used for energy update
c     e_tilde     -- used for energy update
c     p_tilde     -- used for energy update
c     eng         -- used to store internal energy
c     radmin      -- smalles grid spacing (used for time stepping)
c
c     i           -- loop variable
c     j           -- loop variable
c     k           -- loop variable
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
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
      i3 = nzu
c      write(6,*)nxl,nxu,nyl,nyu
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
cc..set artificial viscosity
c      call setQ( nxl,nxu,nyl,nyu,x,y,vx,vy,Area,
c     *           mass,p,q,c0,cl )
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
c
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
          x(i,j)=xold(i,j)+vx(i,j)*dt
          y(i,j)=yold(i,j)+vy(i,j)*dt
        end do
      end do
c
c..Fixup the positions for displacement BC
      ! this is problematic if the bcf vectors aren't properly filled.
      ! default values tend to be 0 and so all positions would set to (0,0)
      ! until that bug gets fixed this portion of the code can't be run!
      ! we might also want to consider doing this outside this code
      if( .false. ) then
c      if( .true. ) then
      if( boundaryCondition(1,1).eq.displacementBC ) then
        i = nxl
        do j = nyl,nyu
          x(i,j) = bcf(1,1,i,j,i3,1)
          y(i,j) = bcf(1,1,i,j,i3,2)
        end do
      end if
      if( boundaryCondition(2,1).eq.displacementBC ) then
        i = nxu
        do j = nyl,nyu
          x(i,j) = bcf(2,1,i,j,i3,1)
          y(i,j) = bcf(2,1,i,j,i3,2)
        end do
      end if
      if( boundaryCondition(1,2).eq.displacementBC ) then
        j = nyl
        do i = nxl,nxu
          x(i,j) = bcf(1,2,i,j,i3,1)
          y(i,j) = bcf(1,2,i,j,i3,2)
        end do
      end if
      if( boundaryCondition(2,2).eq.displacementBC ) then
        j = nyu
        do j = nxl,nxu
          x(i,j) = bcf(2,2,i,j,i3,1)
          y(i,j) = bcf(2,2,i,j,i3,2)
        end do
      end if
      end if
c
c..Update all the stresses, pressure, and internal energy
c      dxbalmin = 1.e10 ! an initial huge value
      lamMax = 0.0
      vismax = 0.0 ! need to fix this
      do j = nyl,nyu-1
        do  i = nxl,nxu-1
          ! compute new zone volumes ... 
          x1 = x(i,j)
          y1 = y(i,j)
          x2 = x(i+1,j)
          y2 = y(i+1,j)
          x3 = x(i+1,j+1)
          y3 = y(i+1,j+1)
          x4 = x(i,j+1)
          y4 = y(i,j+1)

          call getArea( x1,x2,x3,x4,y1,y2,y3,y4,Area_new )

          Area_half = 0.5e0*(Area_new+Area(i,j))
          ! might want to check for zone inversions

          ! compute specific volumes
          rho = m(i,j)/Area(i,j)
          rho_new = m(i,j)/Area_new
          V = rho0(i,j)/rho
          V_new = rho0(i,j)/rho_new
          V_half = 0.5*(V_new+V)

          ! compute strain rates
          vx2mvx4 = vx(i+1,j)  -vx(i,j+1)
          vx3mvx1 = vx(i+1,j+1)-vx(i,j)
          y2my4   = 0.5e0*(yold(i+1,j)  +y(i+1,j))-
     *              0.5e0*(yold(i,j+1)  +y(i,j+1))
          y3my1   = 0.5e0*(yold(i+1,j+1)+y(i+1,j+1))-
     *              0.5e0*(yold(i,j)    +y(i,j))
          vy2mvy4 = vy(i+1,j)  -vy(i,j+1)
          vy3mvy1 = vy(i+1,j+1)-vy(i,j)
          x2mx4   = 0.5e0*(xold(i+1,j)  +x(i+1,j))-
     *              0.5e0*(xold(i,j+1)  +x(i,j+1))
          x3mx1   = 0.5e0*(xold(i+1,j+1)+x(i+1,j+1))-
     *              0.5e0*(xold(i,j)    +x(i,j))

          tmp = 1.e0/(2.e0*Area_half)
          dvxdx = (vx2mvx4*y3my1-y2my4*vx3mvx1)*tmp
          dvydy = -(vy2mvy4*x3mx1-x2mx4*vy3mvy1)*tmp
          dvxdy = -(vx2mvx4*x3mx1-x2mx4*vx3mvx1)*tmp
          dvydx = (vy2mvy4*y3my1-y2my4*vy3mvy1)*tmp

          eps_dot_x = dvxdx
          eps_dot_y = dvydy
          eps_dot_xy = dvydx+dvxdy

          ! correction of old stress to account for rotations
          so = 0.5e0*(dvydx-dvxdy)*dt
          co = cos(asin(so))
          sx_temp = sx(i,j)*co**2+sy(i,j)*so**2-2.e0*txy(i,j)*co*so
          sy_temp = sx(i,j)*so**2+sy(i,j)*co**2+2.e0*txy(i,j)*so*co
          txy_temp = txy(i,j)*(co**2-so**2)+(sx(i,j)-sy(i,j))*co*so
          
          sx(i,j) = sx_temp
          sy(i,j) = sy_temp
          txy(i,j) = txy_temp

          ! compute stresses
          call getEng( 1.e0/V, p(i,j),eng,apr,bpr,cpr,dpr )
          temperature = ((eng-e0(i,j))/rho0(i,j))/(3.e0*R)
          call getMu( p(i,j),V,temperature,mu )
          grad_V = (V_new-V)/(dt*V_half)
          sx_new = sx(i,j)+2.e0*mu*dt*(eps_dot_x-grad_V/3.e0)
          sy_new = sy(i,j)+2.e0*mu*dt*(eps_dot_y-grad_V/3.e0)
          txy_new = txy(i,j)+mu*dt*eps_dot_xy

          ! check Von Misis yield condition
          yieldTest = (sx_new**2+sy_new**2+2.e0*txy_new**2)-
     *       2.e0/3.e0*Y0**2
          if( yieldTest.gt.0.e0 ) then
            mult = Y0*sqrt(2.e0/(3.e0*
     *         (sx_new**2+sy_new**2+2.e0*txy_new**2)))
            sx_new = sx_new*mult
            sy_new = sy_new*mult
            txy_new = txy_new*mult
          end if
            
          sx_half = 0.5*(sx_new+sx(i,j))
          sy_half = 0.5*(sy_new+sy(i,j))
          txy_half = 0.5*(txy_new+txy(i,j))

          ! calculate artificial viscosity "q"
          !  look at each edge and then combine
c          a = sqrt(p(i,j)/rho) ! maybe check for negative p or density ...
          a = sqrt(max(1.e-16,p(i,j))/rho) ! maybe check for negative p or density ...

          d1 = x(i+1,j)-x(i,j)
          d2 = y(i+1,j)-y(i,j)
          rad = sqrt(d1**2+d2**2)
          d1 = d1/rad
          d2 = d2/rad
          dvel = d1*(vx(i+1,j)-vx(i,j))+
     *           d2*(vy(i+1,j)-vy(i,j))
          if( d1*(x(i+1,j)-x(i,j))+
     *            d2*(y(i+1,j)-y(i,j)).gt.
     *        d1*(xold(i+1,j)-xold(i,j))+
     *            d2*(yold(i+1,j)-yold(i,j)) ) then
            q1 = 0.e0
          else
            q1 = c0**2*rho*dvel**2+cl*a*rho*abs(dvel)
          end if
          radmin = rad
      
          d1 = x(i+1,j+1)-x(i+1,j)
          d2 = y(i+1,j+1)-y(i+1,j)
          rad = sqrt(d1**2+d2**2)
          d1 = d1/rad
          d2 = d2/rad
          dvel = d1*(vx(i+1,j+1)-vx(i+1,j))+
     *           d2*(vy(i+1,j+1)-vy(i+1,j))
          if( d1*(x(i+1,j+1)-x(i+1,j))+
     *            d2*(y(i+1,j+1)-y(i+1,j)).gt.
     *        d1*(xold(i+1,j+1)-xold(i+1,j))+
     *            d2*(yold(i+1,j+1)-yold(i+1,j)) ) then
            q2 = 0.e0
          else
            q2 = c0**2*rho*dvel**2+cl*a*rho*abs(dvel)
          end if
          radmin = min( radmin,rad )
    
          d1 = x(i,j+1)-x(i+1,j+1)
          d2 = y(i,j+1)-y(i+1,j+1)
          rad = sqrt(d1**2+d2**2)
          d1 = d1/rad
          d2 = d2/rad
          dvel = d1*(vx(i,j+1)-vx(i+1,j+1))+
     *           d2*(vy(i,j+1)-vy(i+1,j+1))
          if( d1*(x(i,j+1)-x(i+1,j+1))+
     *            d2*(y(i,j+1)-y(i+1,j+1)).gt.
     *        d1*(xold(i,j+1)-xold(i+1,j+1))+
     *            d2*(yold(i,j+1)-yold(i+1,j+1)) ) then
            q3 = 0.e0
          else
            q3 = c0**2*rho*dvel**2+cl*a*rho*abs(dvel)
          end if
          radmin = min( radmin,rad )
    
          d1 = x(i,j)-x(i,j+1)
          d2 = y(i,j)-y(i,j+1)
          rad = sqrt(d1**2+d2**2)
          d1 = d1/rad
          d2 = d2/rad
          dvel = d1*(vx(i,j)-vx(i,j+1))+
     *           d2*(vy(i,j)-vy(i,j+1))
          if( d1*(x(i,j)-x(i,j+1))+
     *            d2*(y(i,j)-y(i,j+1)).gt.
     *        d1*(xold(i,j)-xold(i,j+1))+
     *            d2*(yold(i,j)-yold(i,j+1)) ) then
            q4 = 0.e0
          else
            q4 = c0**2*rho*dvel**2+cl*a*rho*abs(dvel)
          end if
          radmin = min( radmin,rad )
      
          !q_new = 0.5e0*(q1+q2+q3+q4)
          q_new = max(q1,max(q2,max(q3,q4)))
          if( V_new.gt.V ) then
            q_new = 0.e0
          end if
c          q_new = 0.e0

          q_bar = 0.5e0*(q(i,j)+q_new)
          
          if( ilinear.eq.1 ) then ! linear elasticity
            p(i,j) = p(i,j)-dt*mu*grad_V

c            dxbalmin = min( dxbalmin, radmin/(sqrt( 3.0*mu/rho )) )
            lamMax = max( lamMax,sqrt( 3.0*mu/rho )/radmin )
          else
            a = sqrt( max(1.e-16,(p(i,j)+q(i,j)))/rho )
c            dxbalmin = min( dxbalmin,radmin/a )
            lamMax = max( lamMax,a/radmin )
            ! find energy increment
            dz = V_half*(sx_half*eps_dot_x+
     *         sy_half*eps_dot_y+
     *         txy_half*eps_dot_xy)*dt
            e_tilde = eng-(p(i,j)+q_bar)*(V_new-V)+dz
            call getPress( 1.e0/V_new,e_tilde,p_tilde,apr,bpr,cpr,dpr )
            eng = e_tilde-0.5e0*(p_tilde-p(i,j))*(V_new-V)
            call getPress( 1.e0/V_new,eng,p(i,j),apr,bpr,cpr,dpr )
          end if

          ! update quantities
          sx(i,j) = sx_new
          sy(i,j) = sy_new
          txy(i,j) = txy_new
          q(i,j) = q_new
        end do
      end do

c      write(6,*)'***** ',dxbalmin
c      dxbalmin = 0.001;

      return
      end
