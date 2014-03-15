      subroutine edgefix3d( nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *       dx,dr,mu,lambda,kappa,uc,vc,wc,s11c,s12c,s13c,
     *       s21c,s22c,s23c,s31c,s32c,s33c,gridType,
     *       boundaryCondition,gridIndexRange,rx,u,mask )
      implicit none
      integer nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b
      integer uc,vc,wc,s11c,s12c,s13c
      integer s21c,s22c,s23c,s31c,s32c,s33c
      integer gridType
      integer boundaryCondition(0:1,0:2),gridIndexRange(0:1,0:nd-1)
      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
c
      real dx(0:2),dr(0:2),mu,lambda,kappa
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      real rx(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
c
      integer i1,i2,i3,j,k
      integer axis1,axis2,axis3
      real rxLoc(3,3)
      real drLoc(3)

      ! boundary conditions parameters
      #Include "bcDefineFortranInclude.h"
 
      integer rectangular,curvilinear
      parameter(rectangular=0,curvilinear=1)
c
      if( gridType.eq.rectangular ) then
        do j = 0,2
        do k = 0,2
          rxLoc(j,k) = 0.0
        end do
        end do
        do j = 0,2
          rxLoc(j,j) = 1.0
          drLoc(j) = dx(j)
        end do
      end if

      axis1 = 0
      axis2 = 1
      axis3 = 2
      do i2 = n2a,n2b
        do side1 = 0,1
        do side3 = 0,1
          i1 = gridIndexRange(side1,axis1)
          i3 = gridIndexRange(side3,axis3)
c.. set grid metrics if curvilinear
          if( gridType.eq.curvilinear ) then
            do j = 0,2
            do k = 0,2
              rxLoc(j,k) = rx(i1,i2,i3,j,k)
            end do
            end do
          end if
          r1deriv = 0
          r2deriv = 0
          r3deriv = 0
          if( boundaryCondition(side1,axis1).eq.tractionBC.and.
     *        boundaryCondition(side3,axis3).eq.tractionBC ) then
c.. traction/traction fix ... no derivatives set
          else
            ur2 = (u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(2.0*drLoc(1))
            vr2 = (u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.0*drLoc(1))
            wr2 = (u(i1,i2+1,i3,wc)-u(i1,i2-1,i3,wc))/(2.0*drLoc(1))
            r2deriv = 1
            if( boundaryCondition(side1,axis1).eq.displacementBC ) then
              ur3 = (u(i1,i2,i3+1,uc)-u(i1,i2,i3-1,uc))/(2.0*drLoc(2))
              vr3 = (u(i1,i2,i3+1,vc)-u(i1,i2,i3-1,vc))/(2.0*drLoc(2))
              wr3 = (u(i1,i2,i3+1,wc)-u(i1,i2,i3-1,wc))/(2.0*drLoc(2))
              r3deriv = 1
            end if
            if( boundaryCondition(side3,axis3).eq.displacementBC ) then
              ur1 = (u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.0*drLoc(0))
              vr1 = (u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(2.0*drLoc(0))
              wr1 = (u(i1+1,i2,i3,wc)-u(i1-1,i2,i3,wc))/(2.0*drLoc(0))
              r1deriv = 1
            end if
          end if

        end do
        end do
      end do
