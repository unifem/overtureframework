      subroutine hemp_bcs( 
     *   nxl,nxu,nyl,nyu,x,y,vx,vy,
     *   sigma_x,sigma_y,txy,phi,dt,p0,
     *   boundaryCondition,dim,bcf0,bcOffset )
c
      implicit none
      integer nxl,nxu,nyl,nyu
      real x(nxl:nxu,nyl:nyu),y(nxl:nxu,nyl:nyu)
      real vx(nxl:nxu,nyl:nyu),vy(nxl:nxu,nyl:nyu)
      real sigma_x(nxl:nxu-1,nyl:nyu-1),sigma_y(nxl:nxu-1,nyl:nyu-1)
      real txy(nxl:nxu-1,nyl:nyu-1),phi(nxl:nxu,nyl:nyu)
      real dt,p0
c
      integer boundaryCondition(1:2,1:3)
      integer dim(1:2,1:3,1:2,1:3)
      real bcf0(1:*)
      integer*8 bcOffset(1:2,1:3)
c
cc
      integer i,j
      real xI,xII,xIII,xIV
      real yI,yII,yIII,yIV
      real x2mx3,x3mx4,x4mx1,x1mx2
      real y2my3,y3my4,y4my1,y1my2
      real fx,fx0,fy,fy0,len
c
      include 'bcDefineFortranInclude.h'
      real bcf
      integer kd,ks,k
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
      i3 = 0
c
      ! loop over left boundary
      i = nxl
      if( boundaryCondition(1,1).eq.displacementBC ) then ! prescribed velocity
        do j = nyl+1,nyu-1
          vx(i,j) = bcf(1,1,i,j,i3,3)
          vy(i,j) = bcf(1,1,i,j,i3,4)
        end do
      elseif( boundaryCondition(1,1).eq.tractionBC ) then ! prescribed force
        do j = nyl+1,nyu-1
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
          ! compute required differences for integration path
          x2mx3 = xII-xIII
          x1mx2 = xI-xII
          y2my3 = yII-yIII
          y1my2 = yI-yII
          len = 0.5*(sqrt((xI-xIII)**2+(yI-yIII)**2))

          fx = -dt/(2.e0*phi(i,j))*(
     *       (sigma_x(i,j))*y2my3+
     *       (sigma_x(i,j-1))*y1my2-
     *       (txy(i,j))*x2mx3-
     *       (txy(i,j-1))*x1mx2)+
     *       dt/(abs(phi(i,j)))*bcf(1,1,i,j,i3,1)*len

          fy = +dt/(2.e0*phi(i,j))*(
     *       (sigma_y(i,j))*x2mx3+
     *       (sigma_y(i,j-1))*x1mx2-
     *       (txy(i,j))*y2my3-
     *       (txy(i,j-1))*y1my2)+
     *       dt/(abs(phi(i,j)))*bcf(1,1,i,j,i3,2)*len

          fx0 = -dt/(2.e0*phi(i,j))*(
     *       (-p0)*y2my3+
     *       (-p0)*y1my2)

          fy0 = +dt/(2.e0*phi(i,j))*(
     *       (-p0)*x2mx3+
     *       (-p0)*x1mx2)
          
          ! finally update the velocities
          vx(i,j) = vx(i,j)+(fx-fx0)
          vy(i,j) = vy(i,j)+(fy-fy0)
        end do
      end if
c
      ! now loop over right boundary
      i = nxu
      if( boundaryCondition(2,1).eq.displacementBC ) then ! prescribed velocity
        do j = nyl+1,nyu-1
          vx(i,j) = bcf(2,1,i,j,i3,3)
          vy(i,j) = bcf(2,1,i,j,i3,4)
        end do
      elseif( boundaryCondition(2,1).eq.tractionBC ) then ! prescribed force
        do j = nyl+1,nyu-1
          ! first set positions I,II,III,IV
          xI = x(i,j-1)
          yI = y(i,j-1)
c          
          xIII = x(i,j+1)
          yIII = y(i,j+1)
c
          xIV = x(i-1,j)
          yIV = y(i-1,j)
c
          ! compute required differences for integration path
          x3mx4 = xIII-xIV
          x4mx1 = xIV-xI
          y3my4 = yIII-yIV
          y4my1 = yIV-yI
          len = 0.5*(sqrt((xI-xIII)**2+(yI-yIII)**2))

          fx = -dt/(2.e0*phi(i,j))*(
     *       (sigma_x(i-1,j))*y3my4+
     *       (sigma_x(i-1,j-1))*y4my1-
     *       (txy(i-1,j))*x3mx4-
     *       (txy(i-1,j-1))*x4mx1)+
     *       dt/(abs(phi(i,j)))*bcf(2,1,i,j,i3,1)*len

          fy = +dt/(2.e0*phi(i,j))*(
     *       (sigma_y(i-1,j))*x3mx4+
     *       (sigma_y(i-1,j-1))*x4mx1-
     *       (txy(i-1,j))*y3my4-
     *       (txy(i-1,j-1))*y4my1)+
     *       dt/(abs(phi(i,j)))*bcf(2,1,i,j,i3,2)*len

          fx0 = -dt/(2.e0*phi(i,j))*(
     *       (-p0)*y3my4+
     *       (-p0)*y4my1)

          fy0 = +dt/(2.e0*phi(i,j))*(
     *       (-p0)*x3mx4+
     *       (-p0)*x4mx1)

          ! finally update the velocities
          vx(i,j) = vx(i,j)+(fx-fx0)
          vy(i,j) = vy(i,j)+(fy-fy0)
        end do
      end if

      ! now loop over bottom nodes
      j = nyl
      if( boundaryCondition(1,2).eq.displacementBC ) then ! prescribed velocity
        do i = nxl+1,nxu-1
          vx(i,j) = bcf(1,2,i,j,i3,3)
          vy(i,j) = bcf(1,2,i,j,i3,4)
        end do
      elseif( boundaryCondition(1,2).eq.tractionBC ) then ! prescribed force
        do i = nxl+1,nxu-1
          ! first set positions I,II,III,IV
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
          y2my3 = yII-yIII
          y3my4 = yIII-yIV
          len = 0.5*(sqrt((xII-xIV)**2+(yII-yIV)**2))

          fx = -dt/(2.e0*phi(i,j))*(
     *       (sigma_x(i,j))*y2my3+
     *       (sigma_x(i-1,j))*y3my4-
     *       (txy(i,j))*x2mx3-
     *       (txy(i-1,j))*x3mx4)+
     *       dt/(abs(phi(i,j)))*bcf(1,2,i,j,i3,1)*len

          fy = +dt/(2.e0*phi(i,j))*(
     *       (sigma_y(i,j))*x2mx3+
     *       (sigma_y(i-1,j))*x3mx4-
     *       (txy(i,j))*y2my3-
     *       (txy(i-1,j))*y3my4)+
     *       dt/(abs(phi(i,j)))*bcf(1,2,i,j,i3,2)*len

c          fx = -dt/(2.e0*phi(i,j))*(
c     *       (sigma_x(i,j))*y2my3+
c     *       (sigma_x(i-1,j))*y3my4-
c     *       (txy(i,j))*x2mx3-
c     *       (txy(i-1,j))*x3mx4)
c
c          fy = +dt/(2.e0*phi(i,j))*(
c     *       (sigma_y(i,j))*x2mx3+
c     *       (sigma_y(i-1,j))*x3mx4-
c     *       (txy(i,j))*y2my3-
c     *       (txy(i-1,j))*y3my4)
c          p0 = 1.0

          fx0 = -dt/(2.e0*phi(i,j))*(
     *       (-p0)*y2my3+
     *       (-p0)*y3my4)

          fy0 = dt/(2.e0*phi(i,j))*(
     *       (-p0)*x2mx3+
     *       (-p0)*x3mx4)

          ! finally update the velocities

          vx(i,j) = vx(i,j)+(fx-fx0)
          vy(i,j) = vy(i,j)+(fy-fy0)
        end do
      end if

      ! now loop over top nodes
      j = nyu
      if( boundaryCondition(2,2).eq.displacementBC ) then ! prescribed velocity
        do i = nxl+1,nxu-1
          vx(i,j) = bcf(2,2,i,j,i3,3)
          vy(i,j) = bcf(2,2,i,j,i3,4)
        end do
      elseif( boundaryCondition(2,2).eq.tractionBC ) then ! prescribed force
        do i = nxl+1,nxu-1
          ! first set positions I,II,III,IV
          xI = x(i,j-1)
          yI = y(i,j-1)
c          
          xII = x(i+1,j)
          yII = y(i+1,j)
c
          xIV = x(i-1,j)
          yIV = y(i-1,j)
c
          ! compute required differences for integration path
          x4mx1 = xIV-xI
          x1mx2 = xI-xII
          y4my1 = yIV-yI
          y1my2 = yI-yII
          len = 0.5*(sqrt((xII-xIV)**2+(yII-yIV)**2))

          fx = -dt/(2.e0*phi(i,j))*(
     *       (sigma_x(i-1,j-1))*y4my1+
     *       (sigma_x(i,j-1))*y1my2-
     *       (txy(i-1,j-1))*x4mx1-
     *       (txy(i,j-1))*x1mx2)+
     *       dt/(abs(phi(i,j)))*bcf(2,2,i,j,i3,1)*len

          fy = +dt/(2.e0*phi(i,j))*(
     *       (sigma_y(i-1,j-1))*x4mx1+
     *       (sigma_y(i,j-1))*x1mx2-
     *       (txy(i-1,j-1))*y4my1-
     *       (txy(i,j-1))*y1my2)+
     *       dt/(abs(phi(i,j)))*bcf(2,2,i,j,i3,2)*len

c          fx = -dt/(2.e0*phi(i,j))*(
c     *       (sigma_x(i-1,j-1))*y4my1+
c     *       (sigma_x(i,j-1))*y1my2-
c     *       (txy(i-1,j-1))*x4mx1-
c     *       (txy(i,j-1))*x1mx2)
c
c          fy = +dt/(2.e0*phi(i,j))*(
c     *       (sigma_y(i-1,j-1))*x4mx1+
c     *       (sigma_y(i,j-1))*x1mx2-
c     *       (txy(i-1,j-1))*y4my1-
c     *       (txy(i,j-1))*y1my2)
c          p0 = 2.0

          fx0 = -dt/(2.e0*phi(i,j))*(
     *       (-p0)*y4my1+
     *       (-p0)*y1my2)
          
          fy0 = +dt/(2.e0*phi(i,j))*(
     *       (-p0)*x4mx1+
     *       (-p0)*x1mx2)

          ! finally update the velocities
          
          vx(i,j) = vx(i,j)+(fx-fx0)
          vy(i,j) = vy(i,j)+(fy-fy0)
        end do
      end if

      ! now do lower left corner
      if( boundaryCondition(1,1).eq.displacementBC ) then
        vx(nxl,nyl) = bcf(1,1,nxl,nyl,i3,3)
        vy(nxl,nyl) = bcf(1,1,nxl,nyl,i3,4)
      elseif( boundaryCondition(1,2).eq.displacementBC ) then
        vx(nxl,nyl) = bcf(1,2,nxl,nyl,i3,3)
        vy(nxl,nyl) = bcf(1,2,nxl,nyl,i3,4)
      elseif( boundaryCondition(1,1).eq.tractionBC ) then 
        x2mx3 = x(nxl+1,nyl)-x(nxl,nyl+1)
        y2my3 = y(nxl+1,nyl)-y(nxl,nyl+1)
        len = 0.5*(sqrt((x(nxl,nyl+1)-x(nxl,nyl))**2+
     *                  (y(nxl,nyl+1)-y(nxl,nyl))**2)+
     *             sqrt((x(nxl+1,nyl)-x(nxl,nyl))**2+
     *                  (y(nxl+1,nyl)-y(nxl,nyl))**2))
        fx = -dt/(2.e0*phi(nxl,nyl))*(
     *     (sigma_x(nxl,nyl))*y2my3-
     *     (txy(nxl,nyl))*x2mx3)+
     *     dt/(abs(phi(nxl,nyl)))*bcf(1,1,nxl,nyl,i3,1)*len
        fy = +dt/(2.e0*phi(nxl,nyl))*(
     *     (sigma_y(nxl,nyl))*x2mx3-
     *     (txy(nxl,nyl))*y2my3)+
     *     dt/(abs(phi(nxl,nyl)))*bcf(1,1,nxl,nyl,i3,2)*len

        fx0 = -dt/(2.e0*phi(nxl,nyl))*(
     *     (-p0)*y2my3)
        fy0 = +dt/(2.e0*phi(nxl,nyl))*(
     *     (-p0)*x2mx3)

        vx(nxl,nyl) = vx(nxl,nyl)+(fx-fx0)
        vy(nxl,nyl) = vy(nxl,nyl)+(fy-fy0)
      elseif( boundaryCondition(1,2).eq.tractionBC ) then 
        x2mx3 = x(nxl+1,nyl)-x(nxl,nyl+1)
        y2my3 = y(nxl+1,nyl)-y(nxl,nyl+1)
        len = 0.5*(sqrt((x(nxl,nyl+1)-x(nxl,nyl))**2+
     *                  (y(nxl,nyl+1)-y(nxl,nyl))**2)+
     *             sqrt((x(nxl+1,nyl)-x(nxl,nyl))**2+
     *                  (y(nxl+1,nyl)-y(nxl,nyl))**2))
        fx = -dt/(2.e0*phi(nxl,nyl))*(
     *     (sigma_x(nxl,nyl))*y2my3-
     *     (txy(nxl,nyl))*x2mx3)+
     *     dt/(abs(phi(nxl,nyl)))*bcf(1,2,nxl,nyl,i3,1)*len
        fy = +dt/(2.e0*phi(nxl,nyl))*(
     *     (sigma_y(nxl,nyl))*x2mx3-
     *     (txy(nxl,nyl))*y2my3)+
     *     dt/(abs(phi(nxl,nyl)))*bcf(1,2,nxl,nyl,i3,2)*len

        fx0 = -dt/(2.e0*phi(nxl,nyl))*(
     *     (-p0)*y2my3)
        fy0 = +dt/(2.e0*phi(nxl,nyl))*(
     *     (-p0)*x2mx3)

        vx(nxl,nyl) = vx(nxl,nyl)+(fx-fx0)
        vy(nxl,nyl) = vy(nxl,nyl)+(fy-fy0)
      end if

      ! now do lower right corner
      if( boundaryCondition(2,1).eq.displacementBC ) then
        vx(nxu,nyu) = bcf(2,1,nxu,nyl,i3,3)
        vy(nxu,nyl) = bcf(2,1,nxu,nyl,i3,4)
      elseif( boundaryCondition(1,2).eq.displacementBC ) then
        vx(nxu,nyl) = bcf(1,2,nxu,nyl,i3,3)
        vy(nxu,nyl) = bcf(1,2,nxu,nyl,i3,4)
      elseif( boundaryCondition(2,1).eq.tractionBC ) then 
        x3mx4 = x(nxu,nyl+1)-x(nxu-1,nyl)
        y3my4 = y(nxu,nyl+1)-y(nxu-1,nyl)
        len = 0.5*(sqrt((x(nxu,nyl+1)-x(nxu,nyl))**2+
     *                  (y(nxu,nyl+1)-y(nxu,nyl))**2)+
     *             sqrt((x(nxu,nyl)-x(nxu-1,nyl))**2+
     *                  (y(nxu,nyl)-y(nxu-1,nyl))**2))
        fx = -dt/(2.e0*phi(nxu,nyl))*(
     *     (sigma_x(nxu-1,nyl))*y3my4-
     *     (txy(nxu-1,nyl))*x3mx4)+
     *     dt/(abs(phi(nxu,nyl)))*bcf(2,1,nxu,nyl,i3,1)*len
        fy = +dt/(2.e0*phi(nxu,nyl))*(
     *     (sigma_y(nxu-1,nyl))*x3mx4-
     *     (txy(nxu-1,nyl))*y3my4)+
     *     dt/(abs(phi(nxu,nyl)))*bcf(2,1,nxu,nyl,i3,2)*len

        fx0 = -dt/(2.e0*phi(nxu,nyl))*(
     *     (-p0)*y3my4)
        fy0 = +dt/(2.e0*phi(nxu,nyl))*(
     *     (-p0)*x3mx4)

        vx(nxu,nyl) = vx(nxu,nyl)+(fx-fx0)
        vy(nxu,nyl) = vy(nxu,nyl)+(fy-fy0)
      elseif( boundaryCondition(1,2).eq.tractionBC ) then 
        x3mx4 = x(nxu,nyl+1)-x(nxu-1,nyl)
        y3my4 = y(nxu,nyl+1)-y(nxu-1,nyl)
        len = 0.5*(sqrt((x(nxu,nyl+1)-x(nxu,nyl))**2+
     *                  (y(nxu,nyl+1)-y(nxu,nyl))**2)+
     *             sqrt((x(nxu,nyl)-x(nxu-1,nyl))**2+
     *                  (y(nxu,nyl)-y(nxu-1,nyl))**2))
        fx = -dt/(2.e0*phi(nxu,nyl))*(
     *     (sigma_x(nxu-1,nyl))*y3my4-
     *     (txy(nxu-1,nyl))*x3mx4)+
     *     dt/(abs(phi(nxu,nyl)))*bcf(1,2,nxu,nyl,i3,1)*len
        fy = +dt/(2.e0*phi(nxu,nyl))*(
     *     (sigma_y(nxu-1,nyl))*x3mx4-
     *     (txy(nxu-1,nyl))*y3my4)+
     *     dt/(abs(phi(nxu,nyl)))*bcf(1,2,nxu,nyl,i3,2)*len

        fx0 = -dt/(2.e0*phi(nxu,nyl))*(
     *     (-p0)*y3my4)
        fy0 = +dt/(2.e0*phi(nxu,nyl))*(
     *     (-p0)*x3mx4)

        vx(nxu,nyl) = vx(nxu,nyl)+(fx-fx0)
        vy(nxu,nyl) = vy(nxu,nyl)+(fy-fy0)
      end if

      ! now do upper left corner
      if( boundaryCondition(1,1).eq.displacementBC ) then
        vx(nxl,nyu) = bcf(1,1,nxl,nyu,i3,3)
        vy(nxl,nyu) = bcf(1,1,nxl,nyu,i3,4)
      elseif( boundaryCondition(2,2).eq.displacementBC ) then
        vx(nxl,nyu) = bcf(2,2,nxl,nyu,i3,3)
        vy(nxl,nyu) = bcf(2,2,nxl,nyu,i3,4)
      elseif( boundaryCondition(1,1).eq.tractionBC ) then 
        x1mx2 = x(nxl,nyu-1)-x(nxl+1,nyu)
        y1my2 = y(nxl,nyu-1)-y(nxl+1,nyu)
        len = 0.5*(sqrt((x(nxl,nyu)-x(nxl,nyu-1))**2+
     *                  (y(nxl,nyu)-y(nxl,nyu-1))**2)+
     *             sqrt((x(nxl+1,nyu)-x(nxl,nyu))**2+
     *                  (y(nxl+1,nyu)-y(nxl,nyu))**2))
        fx = -dt/(2.e0*phi(nxl,nyu))*(
     *     (sigma_x(nxl,nyu-1))*y1my2-
     *     (txy(nxl,nyu-1))*x1mx2)+
     *     dt/(abs(phi(nxl,nyu)))*bcf(1,1,nxl,nyu,i3,1)*len
        fy = +dt/(2.e0*phi(nxl,nyu))*(
     *     (sigma_y(nxl,nyu-1))*x1mx2-
     *     (txy(nxl,nyu-1))*y1my2)+
     *     dt/(abs(phi(nxl,nyu)))*bcf(1,1,nxl,nyu,i3,2)*len

        fx0 = -dt/(2.e0*phi(nxl,nyu))*(
     *     (-p0)*y1my2)
        fy0 = +dt/(2.e0*phi(nxl,nyu))*(
     *     (-p0)*x1mx2)
        
        vx(nxl,nyu) = vx(nxl,nyu)+(fx-fx0)
        vy(nxl,nyu) = vy(nxl,nyu)+(fy-fy0)
      elseif( boundaryCondition(2,2).eq.tractionBC ) then 
        x1mx2 = x(nxl,nyu-1)-x(nxl+1,nyu)
        y1my2 = y(nxl,nyu-1)-y(nxl+1,nyu)
        len = 0.5*(sqrt((x(nxl,nyu)-x(nxl,nyu-1))**2+
     *                  (y(nxl,nyu)-y(nxl,nyu-1))**2)+
     *             sqrt((x(nxl+1,nyu)-x(nxl,nyu))**2+
     *                  (y(nxl+1,nyu)-y(nxl,nyu))**2))
        fx = -dt/(2.e0*phi(nxl,nyu))*(
     *     (sigma_x(nxl,nyu-1))*y1my2-
     *     (txy(nxl,nyu-1))*x1mx2)+
     *     dt/(abs(phi(nxl,nyu)))*bcf(2,2,nxl,nyu,i3,1)*len
        fy = +dt/(2.e0*phi(nxl,nyu))*(
     *     (sigma_y(nxl,nyu-1))*x1mx2-
     *     (txy(nxl,nyu-1))*y1my2)+
     *     dt/(abs(phi(nxl,nyu)))*bcf(2,2,nxl,nyu,i3,2)*len

        fx0 = -dt/(2.e0*phi(nxl,nyu))*(
     *     (-p0)*y1my2)
        fy0 = +dt/(2.e0*phi(nxl,nyu))*(
     *     (-p0)*x1mx2)
        
        vx(nxl,nyu) = vx(nxl,nyu)+(fx-fx0)
        vy(nxl,nyu) = vy(nxl,nyu)+(fy-fy0)
      end if

      ! now do upper right corner
      if( boundaryCondition(2,1).eq.displacementBC ) then
        vx(nxu,nyu) = bcf(2,1,nxu,nyu,i3,3)
        vy(nxu,nyu) = bcf(2,1,nxu,nyu,i3,4)
      elseif( boundaryCondition(2,2).eq.displacementBC ) then
        vx(nxu,nyu) = bcf(2,2,nxu,nyu,i3,3)
        vy(nxu,nyu) = bcf(2,2,nxu,nyu,i3,4)
      elseif( boundaryCondition(2,1).eq.tractionBC ) then 
        x4mx1 = x(nxu-1,nyu)-x(nxu,nyu-1)
        y4my1 = y(nxu-1,nyu)-y(nxu,nyu-1)
        len = 0.5*(sqrt((x(nxu,nyu)-x(nxu,nyu-1))**2+
     *                  (y(nxu,nyu)-y(nxu,nyu-1))**2)+
     *             sqrt((x(nxu,nyu)-x(nxu-1,nyu))**2+
     *                  (y(nxu,nyu)-y(nxu-1,nyu))**2))
        fx = -dt/(2.e0*phi(nxu,nyu))*(
     *     (sigma_x(nxu-1,nyu-1))*y4my1-
     *     (txy(nxu-1,nyu-1))*x4mx1)+
     *     dt/(abs(phi(nxu,nyu)))*bcf(2,1,nxu,nyu,i3,1)*len
        fy = +dt/(2.e0*phi(nxu,nyu))*(
     *     (sigma_y(nxu-1,nyu-1))*x4mx1-
     *     (txy(nxu-1,nyu-1))*y4my1)+
     *     dt/(abs(phi(nxu,nyu)))*bcf(2,1,nxu,nyu,i3,2)*len

        fx0 = -dt/(2.e0*phi(nxu,nyu))*(
     *     (-p0)*y4my1)
        fy0 = +dt/(2.e0*phi(nxu,nyu))*(
     *     (-p0)*x4mx1)

        vx(nxu,nyu) = vx(nxu,nyu)+(fx-fx0)
        vy(nxu,nyu) = vy(nxu,nyu)+(fy-fy0)
      elseif( boundaryCondition(2,2).eq.tractionBC ) then 
        x4mx1 = x(nxu-1,nyu)-x(nxu,nyu-1)
        y4my1 = y(nxu-1,nyu)-y(nxu,nyu-1)
        len = 0.5*(sqrt((x(nxu,nyu)-x(nxu,nyu-1))**2+
     *                  (y(nxu,nyu)-y(nxu,nyu-1))**2)+
     *             sqrt((x(nxu,nyu)-x(nxu-1,nyu))**2+
     *                  (y(nxu,nyu)-y(nxu-1,nyu))**2))
        fx = -dt/(2.e0*phi(nxu,nyu))*(
     *     (sigma_x(nxu-1,nyu-1))*y4my1-
     *     (txy(nxu-1,nyu-1))*x4mx1)+
     *     dt/(abs(phi(nxu,nyu)))*bcf(2,2,nxu,nyu,i3,1)*len
        fy = +dt/(2.e0*phi(nxu,nyu))*(
     *     (sigma_y(nxu-1,nyu-1))*x4mx1-
     *     (txy(nxu-1,nyu-1))*y4my1)+
     *     dt/(abs(phi(nxu,nyu)))*bcf(2,2,nxu,nyu,i3,2)*len

        fx0 = -dt/(2.e0*phi(nxu,nyu))*(
     *     (-p0)*y4my1)
        fy0 = +dt/(2.e0*phi(nxu,nyu))*(
     *     (-p0)*x4mx1)

        vx(nxu,nyu) = vx(nxu,nyu)+(fx-fx0)
        vy(nxu,nyu) = vy(nxu,nyu)+(fy-fy0)
      end if
c
      return
      end
