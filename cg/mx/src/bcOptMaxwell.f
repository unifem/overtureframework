! This file automatically generated from bcOptMaxwell.bf with bpp.
! *******************************************************************************
! ************** Boundary Conditions for Maxwell ********************************
! *******************************************************************************



! These next include files will define the macros that will define the difference approximations
! The actual macro is called below
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


!**************************************************************************

! Include macros that are common to different orders of accuracy

c Macros that are common to different orders of accuracy






c use the mask 





c Tangent vectors (un-normalized)
        









! ======== WARNING: These next derivatives are really R and S derivatives ===============






! ***************************************************************************************************
! *************************  here are versions for 3d **********************************************
! ***************************************************************************************************




c Here are versions that use a precomputed jacobian
                                              
           


















! ***************************************************************************************************













! These next approximations are from diff.maple







! ========================================================================================


! **note** this next one only works in 2D


! **note** this next one only works in 2D



! These next two came from ov/bpp/test11.bf
! **note** THESE ARE WRONG ****


























c=====================================================================================
c Boundary conditions for a rectangular grid:
c   Normal component of E is even symmetry
c   Tangential components of E are odd symmetry
c In 2d: normal component of Hz is even symmetry (Neumann BC)
c
c DIM: 2,3
c ORDER: 2,4,6,8
c FORCING: none,twilightZone
c=====================================================================================


c ************************************************************************************************
c  This macro is used for looping over the faces of a grid to assign booundary conditions
c
c extra: extra points to assign
c          Case 1: extra=numberOfGhostPoints -- for assigning extended boundaries
c          Case 2: extra=-1 -- for assigning ghost points but not including extended boundaries
c numberOfGhostPoints : number of ghost points (1 for 2nd order, 2 for fourth-order ...)
c ***********************************************************************************************







!**************************************************************************

! Here are macros that define the planeWave solution
c **************************************************
c Here are macros that define the:
c      planeWave solution 
c **************************************************

c ======================================================================
c  Slow start function 
c    tba = length of slow start interval (<0 mean no slow start)
c ======================================================================

c cubic ramp
c tba=max(REAL_EPSILON,tb-ta);
c dta=t-ta;
      
c This (cubic) ramp has 1-derivative zero at t=0 and t=tba

c This ramp has 3-derivatives zero at t=0 and t=1
c This is from ramp.maple
c r=-84*t**5+35*t**4-20*t**7+70*t**6
c rt=-420*t**4+140*t**3-140*t**6+420*t**5
c rtt=-1680*t**3+420*t**2-840*t**5+2100*t**4
c rttt=-5040*t**2+840*t-4200*t**4+8400*t**3


c This ramp has 4-derivatives zero at t=0 and t=1
c This is from ramp.maple
c r=126*(t)**5-315*(t)**8+70*(t)**9-420*(t)**6+540*(t)**7
c rt=630*(t)**4-2520*(t)**7+630*(t)**8-2520*(t)**5+3780*(t)**6
c rtt=2520*(t)**3-17640*(t)**6+5040*(t)**7-12600*(t)**4+22680*(t)**5
c rttt=7560*(t)**2-105840*(t)**5+35280*(t)**6-50400*(t)**3+113400*(t)**4


c ============================================================
c  Initialize parameters for the boundary forcing
c   tba: slow start time interval -- no slow start if this is negative
c ===========================================================

c **************** Here is the new generic plane wave solution *******************

! component n=ex,ey,ez, hx,hy,hz (assumes ex=0)
! one time derivative:
! two time derivatives:
! three time derivatives:

c *************** Here is the 2D planeWave solution ******************************


c one time derivative:

c two time derivatives:

c three time derivatives:

c four time derivatives:

c Here are the slow start versions

c one time derivative:

c two time derivatives:

c three time derivatives:

c four time derivatives:


c **************** Here is the 3D planeWave solution ***************************************



c one time derivative:


c two time derivatives:


c three time derivatives:


c four time derivatives:


c Here are the slow start versions


c one time derivative:


c two time derivatives:

c three time derivatives:

c four time derivatives:


c Helper function: Return minus the second time derivative



!========================================================================================
!  Boundary conditions for a curvilinear grid
!
!      D0r( a1.uv ) + D0s( a2.uv) = 0
!      D+^2( tau.uv ) = 0
!      
!  FORCING: none, twilightZone
!========================================================================================


! =============================================================================================
! These formulae are from maxwell/bc.maple
! =============================================================================================





!     order=4: generated by bcOptMaxwell4.bf




! *** build null versions that are faster to compile






      subroutine bcOptMaxwell( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,ndf1a,
     & ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,gridIndexRange,dimension,u,f,
     & mask,rsxy, xy,bc, boundaryCondition, ipar, rpar, ierr )
! ===================================================================================
!  Optimised Boundary conditions for Maxwell's Equations.
!
!  gridType : 0=rectangular, 1=curvilinear
!  useForcing : 1=use f for RHS to BC
!  side,axis : 0:1 and 0:2
! ===================================================================================

      implicit none

      integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, ndf1a,ndf1b,ndf2a,
     & ndf2b,ndf3a,ndf3b,n1a,n1b,n2a,n2b,n3a,n3b, ndc, bc,ierr

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      real f(ndf1a:ndf1b,ndf2a:ndf2b,ndf3a:ndf3b,0:*)
      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      integer gridIndexRange(0:1,0:2),dimension(0:1,0:2)

      integer ipar(0:*),boundaryCondition(0:1,0:2)
      real rpar(0:*)

!     --- local variables ----

      integer orderOfAccuracy

      ierr=0

      orderOfAccuracy      =ipar(9)

      ! assign corners and edges (3d)
      if( orderOfAccuracy.eq.2 )then
        call cornersMxOrder2(nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,ndf1a,
     & ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,gridIndexRange,dimension,u,f,
     & mask,rsxy, xy,bc, boundaryCondition, ipar, rpar, ierr )
      else if( orderOfAccuracy.eq.4 )then
        call cornersMxOrder4(nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,ndf1a,
     & ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,gridIndexRange,dimension,u,f,
     & mask,rsxy, xy,bc, boundaryCondition, ipar, rpar, ierr )
      else if( orderOfAccuracy.eq.6 )then
        call cornersMxOrder6(nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,ndf1a,
     & ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,gridIndexRange,dimension,u,f,
     & mask,rsxy, xy,bc, boundaryCondition, ipar, rpar, ierr )
      else if( orderOfAccuracy.eq.8 )then
        call cornersMxOrder8(nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,ndf1a,
     & ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,gridIndexRange,dimension,u,f,
     & mask,rsxy, xy,bc, boundaryCondition, ipar, rpar, ierr )
      else
         stop 5533
      end if

      ! ok if( .true. ) return ! **********************************************************

      if( nd.eq.2 )then
        if( orderOfAccuracy.eq.2 )then
          call bcOptMaxwell2dOrder2(nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     & ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,gridIndexRange,dimension,u,
     & f,mask,rsxy, xy,bc, boundaryCondition, ipar, rpar, ierr )
        else if( orderOfAccuracy.eq.4 )then
          call bcOptMaxwell2dOrder4(nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     & ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,gridIndexRange,dimension,u,
     & f,mask,rsxy, xy,bc, boundaryCondition, ipar, rpar, ierr )
        else if( orderOfAccuracy.eq.6 )then
          call bcOptMaxwell2dOrder6(nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     & ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,gridIndexRange,dimension,u,
     & f,mask,rsxy, xy,bc, boundaryCondition, ipar, rpar, ierr )
        else if( orderOfAccuracy.eq.8 )then
          call bcOptMaxwell2dOrder8(nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     & ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,gridIndexRange,dimension,u,
     & f,mask,rsxy, xy,bc, boundaryCondition, ipar, rpar, ierr )
        else
          stop 5533
        end if
      else if( nd.eq.3 )then
        if( orderOfAccuracy.eq.2 )then
          call bcOptMaxwell3dOrder2(nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     & ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,gridIndexRange,dimension,u,
     & f,mask,rsxy, xy,bc, boundaryCondition, ipar, rpar, ierr )
        else if( orderOfAccuracy.eq.4 )then
          call bcOptMaxwell3dOrder4(nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     & ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,gridIndexRange,dimension,u,
     & f,mask,rsxy, xy,bc, boundaryCondition, ipar, rpar, ierr )
        else if( orderOfAccuracy.eq.6 )then
          call bcOptMaxwell3dOrder6(nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     & ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,gridIndexRange,dimension,u,
     & f,mask,rsxy, xy,bc, boundaryCondition, ipar, rpar, ierr )
        else if( orderOfAccuracy.eq.8 )then
          call bcOptMaxwell3dOrder8(nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     & ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,gridIndexRange,dimension,u,
     & f,mask,rsxy, xy,bc, boundaryCondition, ipar, rpar, ierr )
        else
          stop 5533
        end if
      else
        stop 8822
      end if

      return
      end






      subroutine periodicUpdateMaxwell(nd,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,
     & u,ca,cb, indexRange, gridIndexRange, dimension,
     & isPeriodic )
!======================================================================
!  Optimised Boundary Conditions
!         
! nd : number of space dimensions
! ca,cb : assign components c=uC(ca),..,uC(cb)
! useWhereMask : if not equal to zero, only apply the BC where mask(i1,i2,i3).ne.0
!======================================================================
      implicit none
      integer nd,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b

      integer isPeriodic(0:2),indexRange(0:1,0:2)
      integer gridIndexRange(0:1,0:2),dimension(0:1,0:2)

      real u(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,0:*)
      integer ca,cb

!     --- local variables 
      integer c,i1,i2,i3,axis,diff
      integer n1a,n1b, n2a,n2b, n3a,n3b


      n1a=dimension(0,0)
      n1b=dimension(1,0)
      n2a=dimension(0,1)
      n2b=dimension(1,1)
      n3a=dimension(0,2)
      n3b=dimension(1,2)

      do axis=0,nd-1
        if( isPeriodic(axis).ne.0 )then
!         length of the period:
          diff=gridIndexRange(1,axis)-gridIndexRange(0,axis)
!         assign all ghost points on "left"
!         I[i]=Range(dimension(Start,axis),indexRange(Start,axis)-1);
!         u(I[0],I[1],I[2],I[3])=u(I[0]+diff[0],I[1]+diff[1],I[2]+diff[2],I[3]+diff[3]);
!         // assign all ghost points on "right"
!         I[i]=Range(indexRange(End,axis)+1,dimension(End,axis));
!         u(I[0],I[1],I[2],I[3])=u(I[0]-diff[0],I[1]-diff[1],I[2]-diff[2],I[3]-diff[3]);

          if( axis.eq.0 )then
            n1a=dimension(0,0)
            n1b=indexRange(0,0)-1
            do c=ca,cb
            do i1=n1a,n1b
            do i3=n3a,n3b
            do i2=n2a,n2b
              u(i1,i2,i3,c)=u(i1+diff,i2,i3,c)
            end do
            end do
            end do
            end do
            n1a=indexRange(1,0)+1
            n1b=dimension(1,0)
            do c=ca,cb
            do i1=n1a,n1b
            do i3=n3a,n3b
            do i2=n2a,n2b
              u(i1,i2,i3,c)=u(i1-diff,i2,i3,c)
            end do
            end do
            end do
            end do
            n1a=dimension(0,0)
            n1b=dimension(1,0)
          else if( axis.eq.1 )then
            n2a=dimension(0,1)
            n2b=indexRange(0,1)-1
            do c=ca,cb
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
              u(i1,i2,i3,c)=u(i1,i2+diff,i3,c)
            end do
            end do
            end do
            end do
            n2a=indexRange(1,1)+1
            n2b=dimension(1,1)
            do c=ca,cb
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
              u(i1,i2,i3,c)=u(i1,i2-diff,i3,c)
            end do
            end do
            end do
            end do
            n2a=dimension(0,1)
            n2b=dimension(1,1)
          else
            n3a=dimension(0,2)
            n3b=indexRange(0,2)-1
            do c=ca,cb
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
              u(i1,i2,i3,c)=u(i1,i2,i3+diff,c)
            end do
            end do
            end do
            end do
            n3a=indexRange(1,2)+1
            n3b=dimension(1,2)
            do c=ca,cb
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
              u(i1,i2,i3,c)=u(i1,i2,i3-diff,c)
            end do
            end do
            end do
            end do
            n3a=dimension(0,2)
            n3b=dimension(1,2)
          end if

        end if
      end do
      return
      end
