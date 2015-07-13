! This file automatically generated from bcSymmetry.bf with bpp.
! *******************************************************************************
!       Symmetry Boundary Conditions for Maxwell
! *******************************************************************************



! ************************************************************************************************
!  This macro is used for looping over the faces of a grid to assign booundary conditions
!
! extra: extra points to assign
!          Case 1: extra=numberOfGhostPoints -- for assigning extended boundaries
!          Case 2: extra=-1 -- for assigning ghost points but not including extended boundaries
! numberOfGhostPoints : number of ghost points (1 for 2nd order, 2 for fourth-order ...)
! ***********************************************************************************************


! NOTE: This normal may be outward or inward




! ======================================================================================
!  Set EVEN-ODD Symmetry conditions
!   GRIDTYPE: rectangular or curvilinear
!   DIM : 2 or 3 
!   (i1,i2,i3) (i1m,i2m,i3m) and (i1p,i2p,i3p)
!   en,et1,et2 for rectangular
! ======================================================================================

! ======================================================================================
!  Set EVEN-ODD forcing on the symmetry conditions 
!   GRIDTYPE: rectangular or curvilinear
!   DIM : 2 or 3 
! ======================================================================================

! ======================================================================================
!  Set EVEN Symmetry conditions
!   GRIDTYPE: rectangular or curvilinear
!   DIM : 2 or 3 
!   (i1,i2,i3) (i1m,i2m,i3m) and (i1p,i2p,i3p)
!   en,et1,et2 for rectangular
! ======================================================================================

! ======================================================================================
!  Set EVEN forcing on the symmetry conditions 
!   GRIDTYPE: rectangular or curvilinear
!   DIM : 2 or 3 
! ======================================================================================


! ======================================================================================
!   Symmetry BC: Assign edges and corner points next to edges in 3D
!
!  Set the corner edge points "C" and points outside vertices next to symmetry faces
!              |
!              X
!              |
!        X--X--X--X---- symmetry
!              |
!        C  C  X
!              |
!        C  C  X
!
! =================================================================================


! =================================================================================
! Assign corners where at least one face is a symmetry BC
! =================================================================================

! ==================================================================================
! This macro can be used to create bcSymmetryEven or bcSymmetryEvenOdd by
! redefining some macros
! ==================================================================================

! -- here we create the even symmetry subroutine

      subroutine bcSymmetryEven( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     & gridIndexRange, u, mask,rsxy, xy,bc, boundaryCondition, ipar, 
     & rpar, ierr )
! ===================================================================================
!  Symmetry Boundary Conditions for Maxwell's Equations.
!
!  gridType : 0=rectangular, 1=curvilinear
!  bcOption=ipar(26) : 0=assign all faces, 1=assign corners and edges
!
!  u : solution at time t
!
! ===================================================================================
      implicit none
      integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, ndf1a,ndf1b,ndf2a,
     & ndf2b,ndf3a,ndf3b,n1a,n1b,n2a,n2b,n3a,n3b, ndc, bc,ierr
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      integer gridIndexRange(0:1,0:2)
      integer ipar(0:*),boundaryCondition(0:1,0:2)
      real rpar(0:*)
!     --- local variables ----
      integer side,axis,gridType,orderOfAccuracy,orderOfExtrapolation,
     & useForcing,ex,ey,ez,hx,hy,hz,useWhereMask,grid,debug,side1,
     & side2,side3,forcingOption
      ! real dx(0:2),dr(0:2),t,ep,dt,c      
      real t,dt,ep
      ! real dxa,dya,dza
      integer axisp1,axisp2,i1,i2,i3,is1,is2,is3,js1,js2,js3,ks1,ks2,
     & ks3,is
      integer ghost1,ghost2,ghost3
      integer extra,extra1a,extra1b,extra2a,extra2b,extra3a,extra3b,
     & numberOfGhostPoints
      integer edgeDirection,sidea,sideb,sidec,bc1,bc2,bc3
      integer bcOption,ghost,en,et1,et2,i1m,i2m,i3m,i1p,i2p,i3p,
     & symmetryOption,myid
      real uv0(0:2),uvm(0:2),uvp(0:2)
      real an1,an2,an3,aNorm,epsX,nDotU
      !real rx0,ry0,rz0 , rxx0,ryy0, rzz0 
      !real dr0,cxt,cxx,cyy,czz,cm1,g,bxx,byy,bzz
      !real rxNorm, rxNormSq, Dn2, Lu, ur0,urr0, unr0, unrr0
      !real ux0,uy0,uz0, uxx0,uyy0,uzz0
      !real unx0,uny0,unz0, unxx0,unyy0,unzz0
      !real t0,t1,t2
      ! real eps,mu,kx,ky,kz,slowStartInterval,twoPi,cc
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
      ! forcing options
      ! forcingOptions -- these should match ForcingEnum in Maxwell.h 
      integer noForcing,magneticSinusoidalPointSource,gaussianSource,
     & twilightZoneForcing,planeWaveBoundaryForcing, 
     & gaussianChargeSource, userDefinedForcingOption
      parameter(noForcing                =0,
     & magneticSinusoidalPointSource =1,gaussianSource                
     & =2,twilightZoneForcing           =3,planeWaveBoundaryForcing   
     &    =4,    gaussianChargeSource          =5,
     & userDefinedForcingOption      =6 )
      integer rectangular,curvilinear
      parameter(rectangular=0,curvilinear=1)
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
      bcOption             =ipar(26)
      symmetryOption       =ipar(27)
      myid                 =ipar(28)
      ! dx(0)                =rpar(0)
      ! dx(1)                =rpar(1)
      ! dx(2)                =rpar(2)
      ! dr(0)                =rpar(3)
      ! dr(1)                =rpar(4)
      ! dr(2)                =rpar(5)
      t                    =rpar(6)
      ep                   =rpar(7) ! pointer for exact solution
      dt                   =rpar(8)
      ! c                    =rpar(9)
      if( debug.gt.1 )then
        write(*,'("bcSymmetryEven: **START** grid=",i4," bcOption=",i3)
     & ') grid,bcOption
      end if
      if( bcOption.ne.0 .and. bcOption.ne.1 )then
        write(*,'("bcSymmetryEven:ERROR: invalid bcOpt=",i6)') bcOption
        stop 7732
      end if
      epsX=1.e-30 ! fix this ***
      numberOfGhostPoints=orderOfAccuracy/2
      if( debug.gt.1 .and. t.le.dt .and. myid.eq.0 )then
        write(*,'("bcSymmetryEven: orderOfAccuracy,
     & numberOfGhostPoints=",2i3," t="e9.2)') orderOfAccuracy,
     & numberOfGhostPoints,t
        ! '
      end if
      ! assign extra points in the tangential direction: (this is probably ok)
      extra=numberOfGhostPoints
      if( bcOption.eq.0 )then
      ! -------------------------------------------------------------------------
      ! ------------------Loop over Sides----------------------------------------
      ! -------------------------------------------------------------------------
       extra1a=extra
       extra1b=extra
       extra2a=extra
       extra2b=extra
       if( nd.eq.3 )then
         extra3a=extra
         extra3b=extra
       else
         extra3a=0
         extra3b=0
       end if
       do axis=0,nd-1
       do side=0,1
         if( boundaryCondition(side,axis).eq.symmetryBoundaryCondition 
     & )then
           ! write(*,'(" bcSymmetry: grid,side,axis,bc=",3i2)') grid,side,axis,boundaryCondition(side,axis)
           n1a=max(gridIndexRange(0,0)-extra1a,nd1a)
           n1b=min(gridIndexRange(1,0)+extra1b,nd1b)
           n2a=max(gridIndexRange(0,1)-extra2a,nd2a)
           n2b=min(gridIndexRange(1,1)+extra2b,nd2b)
           n3a=max(gridIndexRange(0,2)-extra3a,nd3a)
           n3b=min(gridIndexRange(1,2)+extra3b,nd3b)
           if( axis.eq.0 )then
             n1a=gridIndexRange(side,axis)
             n1b=gridIndexRange(side,axis)
           else if( axis.eq.1 )then
             n2a=gridIndexRange(side,axis)
             n2b=gridIndexRange(side,axis)
           else
             n3a=gridIndexRange(side,axis)
             n3b=gridIndexRange(side,axis)
           end if
           is1=0
           is2=0
           is3=0
           if( axis.eq.0 )then
             is1=1-2*side
           else if( axis.eq.1 )then
             is2=1-2*side
           else if( axis.eq.2 )then
             is3=1-2*side
           else
             stop 5
           end if
           axisp1=mod(axis+1,nd)
           axisp2=mod(axis+2,nd)
       if( gridType.eq.rectangular )then
        ! ***********************************************
        ! ************rectangular grid*******************
        ! ***********************************************
        ! write(*,'(" Apply bcSymmetry: grid,side,axis=",3i3," dt,c=",2e12.3)') grid,side,axis,dt,c
        en =ex + axis             ! normal component
        et1=ex + mod(axis+1,nd)   ! tangential component 1
        et2=ex + mod(axis+2,nd)   ! tangential component 2
        if( nd.eq.2 )then
          i3=gridIndexRange(0,2)
          i3m=i3
          i3p=i3
          do i3=n3a,n3b
          do i2=n2a,n2b
          do i1=n1a,n1b
           do ghost=1,numberOfGhostPoints
            i1m=i1-is1*ghost
            i2m=i2-is2*ghost
            i1p=i1+is1*ghost
            i2p=i2+is2*ghost
               ! Ghost: even symmetry
               u(i1m,i2m,i3m,ex) = u(i1p,i2p,i3p,ex)
               u(i1m,i2m,i3m,ey) = u(i1p,i2p,i3p,ey)
               u(i1m,i2m,i3m,hz) = u(i1p,i2p,i3p,hz)
           end do
          end do
          end do
          end do
        else if( nd.eq.3 )then
          do i3=n3a,n3b
          do i2=n2a,n2b
          do i1=n1a,n1b
           do ghost=1,numberOfGhostPoints
            i1m=i1-is1*ghost
            i2m=i2-is2*ghost
            i3m=i3-is3*ghost
            i1p=i1+is1*ghost
            i2p=i2+is2*ghost
            i3p=i3+is3*ghost
               u(i1m,i2m,i3m,ex) = u(i1p,i2p,i3p,ex)
               u(i1m,i2m,i3m,ey) = u(i1p,i2p,i3p,ey)
               u(i1m,i2m,i3m,ez) = u(i1p,i2p,i3p,ez)
           end do
          end do
          end do
          end do
        else
          stop 9467
        end if
        if( forcingOption.eq.twilightZoneForcing )then
         ! -----------------------------------------------------
         ! ----------------- Twilight Zone Forcing -------------
         ! -----------------------------------------------------
         if( ex.ne.0 .or. ey.ne.1 )then
           ! we assume ex==0 etc. below 
           stop 1133
         end if
         if( nd.eq.2 )then
          i3=gridIndexRange(0,2)
          i3m=i3
          i3p=i3
          do i3=n3a,n3b
          do i2=n2a,n2b
          do i1=n1a,n1b
           ! get the exact solution on the boundary: 
           do ghost=1,numberOfGhostPoints
            i1m=i1-is1*ghost
            i2m=i2-is2*ghost
            i1p=i1+is1*ghost
            i2p=i2+is2*ghost
               call ogf2d(ep,xy(i1m,i2m,i3m,0),xy(i1m,i2m,i3m,1),t,uvm(
     & 0),uvm(1),uvm(2))
               call ogf2d(ep,xy(i1p,i2p,i3p,0),xy(i1p,i2p,i3p,1),t,uvp(
     & 0),uvp(1),uvp(2))
              ! even
              u(i1m,i2m,i3m,ex)= u(i1m,i2m,i3m,ex) - uvp(0) + uvm(0)
              u(i1m,i2m,i3m,ey)= u(i1m,i2m,i3m,ey) - uvp(1) + uvm(1)
              u(i1m,i2m,i3m,hz)= u(i1m,i2m,i3m,hz) - uvp(2) + uvm(2)
           end do
          end do
          end do
          end do
         else if( nd.eq.3 )then
          do i3=n3a,n3b
          do i2=n2a,n2b
          do i1=n1a,n1b
           ! get the exact solution on the boundary: 
           do ghost=1,numberOfGhostPoints
            i1m=i1-is1*ghost
            i2m=i2-is2*ghost
            i3m=i3-is3*ghost
            i1p=i1+is1*ghost
            i2p=i2+is2*ghost
            i3p=i3+is3*ghost
               call ogf3d(ep,xy(i1m,i2m,i3m,0),xy(i1m,i2m,i3m,1),xy(
     & i1m,i2m,i3m,2),t,uvm(0),uvm(1),uvm(2))
               call ogf3d(ep,xy(i1p,i2p,i3p,0),xy(i1p,i2p,i3p,1),xy(
     & i1p,i2p,i3p,2),t,uvp(0),uvp(1),uvp(2))
              u(i1m,i2m,i3m,ex)= u(i1m,i2m,i3m,ex) - uvp(0) + uvm(0)
              u(i1m,i2m,i3m,ey)= u(i1m,i2m,i3m,ey) - uvp(1) + uvm(1)
              u(i1m,i2m,i3m,ez)= u(i1m,i2m,i3m,ez) - uvp(2) + uvm(2)
           end do
          end do
          end do
          end do
         else
           stop 9467
         end if
        end if ! forcingOption.eq.twilightZoneForcing
       else if( gridType.eq.curvilinear )then
        ! ***********************************************
        ! ************curvilinear grid*******************
        ! ***********************************************
        if( nd.eq.2 )then
          i3=gridIndexRange(0,2)
          i3m=i3
          i3p=i3
          do i3=n3a,n3b
          do i2=n2a,n2b
          do i1=n1a,n1b
           do ghost=1,numberOfGhostPoints
            i1m=i1-is1*ghost
            i2m=i2-is2*ghost
            i1p=i1+is1*ghost
            i2p=i2+is2*ghost
               ! Ghost: even symmetry
               u(i1m,i2m,i3m,ex) = u(i1p,i2p,i3p,ex)
               u(i1m,i2m,i3m,ey) = u(i1p,i2p,i3p,ey)
               u(i1m,i2m,i3m,hz) = u(i1p,i2p,i3p,hz)
           end do
          end do
          end do
          end do
        else if( nd.eq.3 )then
          do i3=n3a,n3b
          do i2=n2a,n2b
          do i1=n1a,n1b
           do ghost=1,numberOfGhostPoints
            i1m=i1-is1*ghost
            i2m=i2-is2*ghost
            i3m=i3-is3*ghost
            i1p=i1+is1*ghost
            i2p=i2+is2*ghost
            i3p=i3+is3*ghost
               u(i1m,i2m,i3m,ex) = u(i1p,i2p,i3p,ex)
               u(i1m,i2m,i3m,ey) = u(i1p,i2p,i3p,ey)
               u(i1m,i2m,i3m,ez) = u(i1p,i2p,i3p,ez)
           end do
          end do
          end do
          end do
        else
          ! unexpected nd
          stop 1116
        end if
        if( forcingOption.eq.twilightZoneForcing )then
         ! -----------------------------------------------------
         ! ----------------- Twilight Zone Forcing -------------
         ! -----------------------------------------------------
         if( ex.ne.0 .or. ey.ne.1 )then
           ! we assume ex==0 etc. below 
           stop 1133
         end if
         if( nd.eq.2 )then
          i3=gridIndexRange(0,2)
          i3m=i3
          i3p=i3
          do i3=n3a,n3b
          do i2=n2a,n2b
          do i1=n1a,n1b
           do ghost=1,numberOfGhostPoints
            i1m=i1-is1*ghost
            i2m=i2-is2*ghost
            i1p=i1+is1*ghost
            i2p=i2+is2*ghost
               call ogf2d(ep,xy(i1m,i2m,i3m,0),xy(i1m,i2m,i3m,1),t,uvm(
     & 0),uvm(1),uvm(2))
               call ogf2d(ep,xy(i1p,i2p,i3p,0),xy(i1p,i2p,i3p,1),t,uvp(
     & 0),uvp(1),uvp(2))
              ! even
              u(i1m,i2m,i3m,ex)= u(i1m,i2m,i3m,ex) - uvp(0) + uvm(0)
              u(i1m,i2m,i3m,ey)= u(i1m,i2m,i3m,ey) - uvp(1) + uvm(1)
              u(i1m,i2m,i3m,hz)= u(i1m,i2m,i3m,hz) - uvp(2) + uvm(2)
           end do
          end do
          end do
          end do
         else if( nd.eq.3 )then
          do i3=n3a,n3b
          do i2=n2a,n2b
          do i1=n1a,n1b
           ! get the exact solution on the boundary: 
           do ghost=1,numberOfGhostPoints
            i1m=i1-is1*ghost
            i2m=i2-is2*ghost
            i3m=i3-is3*ghost
            i1p=i1+is1*ghost
            i2p=i2+is2*ghost
            i3p=i3+is3*ghost
               call ogf3d(ep,xy(i1m,i2m,i3m,0),xy(i1m,i2m,i3m,1),xy(
     & i1m,i2m,i3m,2),t,uvm(0),uvm(1),uvm(2))
               call ogf3d(ep,xy(i1p,i2p,i3p,0),xy(i1p,i2p,i3p,1),xy(
     & i1p,i2p,i3p,2),t,uvp(0),uvp(1),uvp(2))
              u(i1m,i2m,i3m,ex)= u(i1m,i2m,i3m,ex) - uvp(0) + uvm(0)
              u(i1m,i2m,i3m,ey)= u(i1m,i2m,i3m,ey) - uvp(1) + uvm(1)
              u(i1m,i2m,i3m,ez)= u(i1m,i2m,i3m,ez) - uvp(2) + uvm(2)
           end do
          end do
          end do
          end do
         else
           stop 9467
         end if
        end if ! forcingOption.eq.twilightZoneForcing
        else
          stop 9467
        end if
         end if
       end do
       end do
      end if ! bcOption.eq.0
      if( bcOption.eq.1 )then
      ! ------------------------------------------------------------------------
      ! ------------------ Corners next to Symmetry Boundaries -----------
      ! ------------------------------------------------------------------------
      if( nd.eq.2 )then
       i3=gridIndexRange(0,2)
       i3m=i3
       i3p=i3
       do side1=0,1
       do side2=0,1
        bc1 = boundaryCondition(side1,0)
        bc2 = boundaryCondition(side2,1)
        ! ** note ** treat symmetry-interface corners too
        if( bc1.eq.symmetryBoundaryCondition 
     & .or.bc2.eq.symmetryBoundaryCondition )then
          ! --- We are next to a symmetry corner ---
          is1=1-2*side1
          is2=1-2*side2
          if( bc1.eq.symmetryBoundaryCondition )then
            axis=0  ! treat this axis as the one to apply the symmetry condition to
          else
            axis=1
          end if
          ! Cartesian: 
          en =ex + axis             ! normal component
          et1=ex + mod(axis+1,nd)   ! tangential component 1
          et2=ex + mod(axis+2,nd)   ! tangential component 2
          i1=gridIndexRange(side1,0) ! (i1,i2,i3)=corner point
          i2=gridIndexRange(side2,1)
          do ghost2=1,numberOfGhostPoints
          do ghost1=1,numberOfGhostPoints
           i1m=i1-is1*ghost1  ! assign this corner ghost point (i1m,i2m)
           i2m=i2-is2*ghost2
           i1p=i1m  ! symmetry point is held in (i1p,i2p)
           i2p=i2m
           if( bc1.eq.symmetryBoundaryCondition )then
             i1p=i1+is1*ghost1  ! symmetry pt
           else if( bc2.eq.symmetryBoundaryCondition )then
             i2p=i2+is2*ghost2
           end if
           ! write(*,'("abc :corner symmetry: u(",i6,",",i6,")=u(",i6",",i6")")') i1m,i2m,i1p,i2p
           ! ' 
           ! set the ghost point
           if( gridType.eq.rectangular )then
                ! Ghost: even symmetry
                u(i1m,i2m,i3m,ex) = u(i1p,i2p,i3p,ex)
                u(i1m,i2m,i3m,ey) = u(i1p,i2p,i3p,ey)
                u(i1m,i2m,i3m,hz) = u(i1p,i2p,i3p,hz)
             if( forcingOption.eq.twilightZoneForcing )then
                 call ogf2d(ep,xy(i1m,i2m,i3m,0),xy(i1m,i2m,i3m,1),t,
     & uvm(0),uvm(1),uvm(2))
                 call ogf2d(ep,xy(i1p,i2p,i3p,0),xy(i1p,i2p,i3p,1),t,
     & uvp(0),uvp(1),uvp(2))
                ! even
                u(i1m,i2m,i3m,ex)= u(i1m,i2m,i3m,ex) - uvp(0) + uvm(0)
                u(i1m,i2m,i3m,ey)= u(i1m,i2m,i3m,ey) - uvp(1) + uvm(1)
                u(i1m,i2m,i3m,hz)= u(i1m,i2m,i3m,hz) - uvp(2) + uvm(2)
             end if
           else
                ! Ghost: even symmetry
                u(i1m,i2m,i3m,ex) = u(i1p,i2p,i3p,ex)
                u(i1m,i2m,i3m,ey) = u(i1p,i2p,i3p,ey)
                u(i1m,i2m,i3m,hz) = u(i1p,i2p,i3p,hz)
             if( forcingOption.eq.twilightZoneForcing )then
                 call ogf2d(ep,xy(i1m,i2m,i3m,0),xy(i1m,i2m,i3m,1),t,
     & uvm(0),uvm(1),uvm(2))
                 call ogf2d(ep,xy(i1p,i2p,i3p,0),xy(i1p,i2p,i3p,1),t,
     & uvp(0),uvp(1),uvp(2))
                ! even
                u(i1m,i2m,i3m,ex)= u(i1m,i2m,i3m,ex) - uvp(0) + uvm(0)
                u(i1m,i2m,i3m,ey)= u(i1m,i2m,i3m,ey) - uvp(1) + uvm(1)
                u(i1m,i2m,i3m,hz)= u(i1m,i2m,i3m,hz) - uvp(2) + uvm(2)
             end if
           end if
          end do ! ghost1
          end do ! ghost2
        end if
       end do
       end do
      else ! ***** 3D *****
       if( .true. )then
          do edgeDirection=0,2 ! direction parallel to the edge
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
          do ghost1=1,numberOfGhostPoints
          do ghost2=1,numberOfGhostPoints
           ! shift to ghost point "(ghost1,ghost2)"
           ks1=0
           ks2=0
           ks3=0
           if( edgeDirection.eq.2 )then
             js1=is1*ghost1
             js2=is2*ghost2
             js3=0
             ! Note: no need to consider the case when both adjacent faces are symmetry since the ghost
             ! pts outside the face will have already been set to the interior values : thus instead of
             ! setting u(-1,-1)=u(1,1) we can just set u(-1,-1)=u(1,-1) (or u(-1,-1)=u(-1,1))
             if( bc1.eq.symmetryBoundaryCondition )then
               ! u(-js1,-js2,0)=u( ks1, ks2,0)
               !               =u( js1,-js2,0)
               ks1= js1
               ks2=-js2  ! *wdh* 090720  -- fixed this, and below as well --
               axis=0    ! symmetry axis
             else if( bc2.eq.symmetryBoundaryCondition )then
               ! u(-js1,-js2,0)=u(-js1, js2,0)
               ks1=-js1
               ks2= js2
               axis=1   ! symmetry axis
             end if
           else if( edgeDirection.eq.1 )then
             js1=is1*ghost1
             js2=0
             js3=is3*ghost2
             if( bc1.eq.symmetryBoundaryCondition )then
               ks1= js1
               ks3=-js3
               axis=0   ! symmetry axis
             else if( bc2.eq.symmetryBoundaryCondition )then
               ks1=-js1
               ks3= js3
               axis=2   ! symmetry axis
             end if
           else
             js1=0
             js2=is2*ghost1
             js3=is3*ghost2
             if( bc1.eq.symmetryBoundaryCondition )then
               ks2= js2
               ks3=-js3
               axis=1 ! symmetry axis
             else if( bc2.eq.symmetryBoundaryCondition )then
               ks2=-js2
               ks3= js3
               axis=2 ! symmetry axis
             end if
           end if
           if( bc1.eq.symmetryBoundaryCondition 
     & .or.bc2.eq.symmetryBoundaryCondition )then
         !   if( (bc1.eq.symmetryBoundaryCondition .and. bc2.gt.0) .or.!       (bc2.eq.symmetryBoundaryCondition .and. bc1.gt.0) )then
            ! *********************************************************
            ! ************* SYMMETRY EDGE BC **************************
            ! *********************************************************
            ! Cartesian: 
            en =ex + axis             ! normal component
            et1=ex + mod(axis+1,nd)   ! tangential component 1
            et2=ex + mod(axis+2,nd)   ! tangential component 2
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
               i1m=i1-js1  ! assign this corner ghost point (i1m,i2m,i3m)
               i2m=i2-js2
               i3m=i3-js3
               i1p=i1+ks1   ! symmetry point
               i2p=i2+ks2
               i3p=i3+ks3
               ! if( i1.eq.n1a .and. i2.eq.n2a .and. i3.eq.n3a )then
               ! write(*,'("abc :3d edge symmetry: u(",i6,",",i6,",",i6,")=u(",i6,",",i6,",",i6)') i1m,i2m,i3m,i1p,i2p,i3p
               ! ' 
               ! end if
               if( gridType.eq.rectangular )then
                    u(i1m,i2m,i3m,ex) = u(i1p,i2p,i3p,ex)
                    u(i1m,i2m,i3m,ey) = u(i1p,i2p,i3p,ey)
                    u(i1m,i2m,i3m,ez) = u(i1p,i2p,i3p,ez)
                 if( forcingOption.eq.twilightZoneForcing )then
                     call ogf3d(ep,xy(i1m,i2m,i3m,0),xy(i1m,i2m,i3m,1),
     & xy(i1m,i2m,i3m,2),t,uvm(0),uvm(1),uvm(2))
                     call ogf3d(ep,xy(i1p,i2p,i3p,0),xy(i1p,i2p,i3p,1),
     & xy(i1p,i2p,i3p,2),t,uvp(0),uvp(1),uvp(2))
                    u(i1m,i2m,i3m,ex)= u(i1m,i2m,i3m,ex) - uvp(0) + 
     & uvm(0)
                    u(i1m,i2m,i3m,ey)= u(i1m,i2m,i3m,ey) - uvp(1) + 
     & uvm(1)
                    u(i1m,i2m,i3m,ez)= u(i1m,i2m,i3m,ez) - uvp(2) + 
     & uvm(2)
                 end if
               else
                    u(i1m,i2m,i3m,ex) = u(i1p,i2p,i3p,ex)
                    u(i1m,i2m,i3m,ey) = u(i1p,i2p,i3p,ey)
                    u(i1m,i2m,i3m,ez) = u(i1p,i2p,i3p,ez)
                 if( forcingOption.eq.twilightZoneForcing )then
                     call ogf3d(ep,xy(i1m,i2m,i3m,0),xy(i1m,i2m,i3m,1),
     & xy(i1m,i2m,i3m,2),t,uvm(0),uvm(1),uvm(2))
                     call ogf3d(ep,xy(i1p,i2p,i3p,0),xy(i1p,i2p,i3p,1),
     & xy(i1p,i2p,i3p,2),t,uvp(0),uvp(1),uvp(2))
                    u(i1m,i2m,i3m,ex)= u(i1m,i2m,i3m,ex) - uvp(0) + 
     & uvm(0)
                    u(i1m,i2m,i3m,ey)= u(i1m,i2m,i3m,ey) - uvp(1) + 
     & uvm(1)
                    u(i1m,i2m,i3m,ez)= u(i1m,i2m,i3m,ez) - uvp(2) + 
     & uvm(2)
                 end if
               end if
             end do ! end do i1
             end do ! end do i2
             end do ! end do i3
           end if
          end do ! ghost1
          end do ! ghost2
         end do ! sideb
         end do ! sidea
         end do ! edgeDirection
          do side3=0,1
          do side2=0,1
          do side1=0,1
           bc1 = boundaryCondition(side1,0)
           bc2 = boundaryCondition(side2,1)
           bc3 = boundaryCondition(side3,2)
           if( bc1.eq.symmetryBoundaryCondition 
     & .or.bc2.eq.symmetryBoundaryCondition 
     & .or.bc3.eq.symmetryBoundaryCondition )then
             ! --- symmetry corner ---
             is1=1-2*side1
             is2=1-2*side2
             is3=1-2*side3
             i1=gridIndexRange(side1,0) ! (i1,i2,i3)=corner point
             i2=gridIndexRange(side2,1)
             i3=gridIndexRange(side3,2)
             do ghost3=1,numberOfGhostPoints
             do ghost2=1,numberOfGhostPoints
             do ghost1=1,numberOfGhostPoints
              i1m=i1-is1*ghost1  ! assign this corner ghost point (i1m,i2m,i3m)
              i2m=i2-is2*ghost2
              i3m=i3-is3*ghost3
              i1p=i1m  ! symmetry point is held in (i1p,i2p,i3p)
              i2p=i2m
              i3p=i3m
              if( bc1.eq.symmetryBoundaryCondition )then
                i1p=i1+is1*ghost1  ! symmetry pt
                axis=0 ! symmetry axis
              else if( bc2.eq.symmetryBoundaryCondition )then
                i2p=i2+is2*ghost2
                axis=1 ! symmetry axis
              else if( bc3.eq.symmetryBoundaryCondition )then
                i3p=i3+is3*ghost3
                axis=2 ! symmetry axis
              end if
              ! write(*,'("abc :3d corner symmetry: u(",i6,",",i6,",",i6,")=u(",i6,",",i6,",",i6)') i1m,i2m,i3m,i1p,i2p,i3p
              ! ' 
              ! Cartesian: 
              en =ex + axis             ! normal component
              et1=ex + mod(axis+1,nd)   ! tangential component 1
              et2=ex + mod(axis+2,nd)   ! tangential component 2
              if( gridType.eq.rectangular )then
                   u(i1m,i2m,i3m,ex) = u(i1p,i2p,i3p,ex)
                   u(i1m,i2m,i3m,ey) = u(i1p,i2p,i3p,ey)
                   u(i1m,i2m,i3m,ez) = u(i1p,i2p,i3p,ez)
                if( forcingOption.eq.twilightZoneForcing )then
                    call ogf3d(ep,xy(i1m,i2m,i3m,0),xy(i1m,i2m,i3m,1),
     & xy(i1m,i2m,i3m,2),t,uvm(0),uvm(1),uvm(2))
                    call ogf3d(ep,xy(i1p,i2p,i3p,0),xy(i1p,i2p,i3p,1),
     & xy(i1p,i2p,i3p,2),t,uvp(0),uvp(1),uvp(2))
                   u(i1m,i2m,i3m,ex)= u(i1m,i2m,i3m,ex) - uvp(0) + uvm(
     & 0)
                   u(i1m,i2m,i3m,ey)= u(i1m,i2m,i3m,ey) - uvp(1) + uvm(
     & 1)
                   u(i1m,i2m,i3m,ez)= u(i1m,i2m,i3m,ez) - uvp(2) + uvm(
     & 2)
                end if
              else
                   u(i1m,i2m,i3m,ex) = u(i1p,i2p,i3p,ex)
                   u(i1m,i2m,i3m,ey) = u(i1p,i2p,i3p,ey)
                   u(i1m,i2m,i3m,ez) = u(i1p,i2p,i3p,ez)
                if( forcingOption.eq.twilightZoneForcing )then
                    call ogf3d(ep,xy(i1m,i2m,i3m,0),xy(i1m,i2m,i3m,1),
     & xy(i1m,i2m,i3m,2),t,uvm(0),uvm(1),uvm(2))
                    call ogf3d(ep,xy(i1p,i2p,i3p,0),xy(i1p,i2p,i3p,1),
     & xy(i1p,i2p,i3p,2),t,uvp(0),uvp(1),uvp(2))
                   u(i1m,i2m,i3m,ex)= u(i1m,i2m,i3m,ex) - uvp(0) + uvm(
     & 0)
                   u(i1m,i2m,i3m,ey)= u(i1m,i2m,i3m,ey) - uvp(1) + uvm(
     & 1)
                   u(i1m,i2m,i3m,ez)= u(i1m,i2m,i3m,ez) - uvp(2) + uvm(
     & 2)
                end if
              end if
             end do ! ghost1
             end do ! ghost2
             end do ! ghost3
           end if
          end do   ! end side1
          end do   ! end side2
          end do   ! end side3
       end if
      end if
      end if ! bcOption.eq.1
      return
      end


! -- here we create the even-odd symmetry subroutine

      subroutine bcSymmetryEvenOdd( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     & gridIndexRange, u, mask,rsxy, xy,bc, boundaryCondition, ipar, 
     & rpar, ierr )
! ===================================================================================
!  Symmetry Boundary Conditions for Maxwell's Equations.
!
!  gridType : 0=rectangular, 1=curvilinear
!  bcOption=ipar(26) : 0=assign all faces, 1=assign corners and edges
!
!  u : solution at time t
!
! ===================================================================================
      implicit none
      integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, ndf1a,ndf1b,ndf2a,
     & ndf2b,ndf3a,ndf3b,n1a,n1b,n2a,n2b,n3a,n3b, ndc, bc,ierr
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      integer gridIndexRange(0:1,0:2)
      integer ipar(0:*),boundaryCondition(0:1,0:2)
      real rpar(0:*)
!     --- local variables ----
      integer side,axis,gridType,orderOfAccuracy,orderOfExtrapolation,
     & useForcing,ex,ey,ez,hx,hy,hz,useWhereMask,grid,debug,side1,
     & side2,side3,forcingOption
      ! real dx(0:2),dr(0:2),t,ep,dt,c      
      real t,dt,ep
      ! real dxa,dya,dza
      integer axisp1,axisp2,i1,i2,i3,is1,is2,is3,js1,js2,js3,ks1,ks2,
     & ks3,is
      integer ghost1,ghost2,ghost3
      integer extra,extra1a,extra1b,extra2a,extra2b,extra3a,extra3b,
     & numberOfGhostPoints
      integer edgeDirection,sidea,sideb,sidec,bc1,bc2,bc3
      integer bcOption,ghost,en,et1,et2,i1m,i2m,i3m,i1p,i2p,i3p,
     & symmetryOption,myid
      real uv0(0:2),uvm(0:2),uvp(0:2)
      real an1,an2,an3,aNorm,epsX,nDotU
      !real rx0,ry0,rz0 , rxx0,ryy0, rzz0 
      !real dr0,cxt,cxx,cyy,czz,cm1,g,bxx,byy,bzz
      !real rxNorm, rxNormSq, Dn2, Lu, ur0,urr0, unr0, unrr0
      !real ux0,uy0,uz0, uxx0,uyy0,uzz0
      !real unx0,uny0,unz0, unxx0,unyy0,unzz0
      !real t0,t1,t2
      ! real eps,mu,kx,ky,kz,slowStartInterval,twoPi,cc
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
      ! forcing options
      ! forcingOptions -- these should match ForcingEnum in Maxwell.h 
      integer noForcing,magneticSinusoidalPointSource,gaussianSource,
     & twilightZoneForcing,planeWaveBoundaryForcing, 
     & gaussianChargeSource, userDefinedForcingOption
      parameter(noForcing                =0,
     & magneticSinusoidalPointSource =1,gaussianSource                
     & =2,twilightZoneForcing           =3,planeWaveBoundaryForcing   
     &    =4,    gaussianChargeSource          =5,
     & userDefinedForcingOption      =6 )
      integer rectangular,curvilinear
      parameter(rectangular=0,curvilinear=1)
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
      bcOption             =ipar(26)
      symmetryOption       =ipar(27)
      myid                 =ipar(28)
      ! dx(0)                =rpar(0)
      ! dx(1)                =rpar(1)
      ! dx(2)                =rpar(2)
      ! dr(0)                =rpar(3)
      ! dr(1)                =rpar(4)
      ! dr(2)                =rpar(5)
      t                    =rpar(6)
      ep                   =rpar(7) ! pointer for exact solution
      dt                   =rpar(8)
      ! c                    =rpar(9)
      if( debug.gt.1 )then
        write(*,'("bcSymmetryEvenOdd: **START** grid=",i4," bcOption=",
     & i3)') grid,bcOption
      end if
      if( bcOption.ne.0 .and. bcOption.ne.1 )then
        write(*,'("bcSymmetryEvenOdd:ERROR: invalid bcOpt=",i6)') 
     & bcOption
        stop 7732
      end if
      epsX=1.e-30 ! fix this ***
      numberOfGhostPoints=orderOfAccuracy/2
      if( debug.gt.1 .and. t.le.dt .and. myid.eq.0 )then
        write(*,'("bcSymmetryEvenOdd: orderOfAccuracy,
     & numberOfGhostPoints=",2i3," t="e9.2)') orderOfAccuracy,
     & numberOfGhostPoints,t
        ! '
      end if
      ! assign extra points in the tangential direction: (this is probably ok)
      extra=numberOfGhostPoints
      if( bcOption.eq.0 )then
      ! -------------------------------------------------------------------------
      ! ------------------Loop over Sides----------------------------------------
      ! -------------------------------------------------------------------------
       extra1a=extra
       extra1b=extra
       extra2a=extra
       extra2b=extra
       if( nd.eq.3 )then
         extra3a=extra
         extra3b=extra
       else
         extra3a=0
         extra3b=0
       end if
       do axis=0,nd-1
       do side=0,1
         if( boundaryCondition(side,axis).eq.symmetryBoundaryCondition 
     & )then
           ! write(*,'(" bcSymmetry: grid,side,axis,bc=",3i2)') grid,side,axis,boundaryCondition(side,axis)
           n1a=max(gridIndexRange(0,0)-extra1a,nd1a)
           n1b=min(gridIndexRange(1,0)+extra1b,nd1b)
           n2a=max(gridIndexRange(0,1)-extra2a,nd2a)
           n2b=min(gridIndexRange(1,1)+extra2b,nd2b)
           n3a=max(gridIndexRange(0,2)-extra3a,nd3a)
           n3b=min(gridIndexRange(1,2)+extra3b,nd3b)
           if( axis.eq.0 )then
             n1a=gridIndexRange(side,axis)
             n1b=gridIndexRange(side,axis)
           else if( axis.eq.1 )then
             n2a=gridIndexRange(side,axis)
             n2b=gridIndexRange(side,axis)
           else
             n3a=gridIndexRange(side,axis)
             n3b=gridIndexRange(side,axis)
           end if
           is1=0
           is2=0
           is3=0
           if( axis.eq.0 )then
             is1=1-2*side
           else if( axis.eq.1 )then
             is2=1-2*side
           else if( axis.eq.2 )then
             is3=1-2*side
           else
             stop 5
           end if
           axisp1=mod(axis+1,nd)
           axisp2=mod(axis+2,nd)
       if( gridType.eq.rectangular )then
        ! ***********************************************
        ! ************rectangular grid*******************
        ! ***********************************************
        ! write(*,'(" Apply bcSymmetry: grid,side,axis=",3i3," dt,c=",2e12.3)') grid,side,axis,dt,c
        en =ex + axis             ! normal component
        et1=ex + mod(axis+1,nd)   ! tangential component 1
        et2=ex + mod(axis+2,nd)   ! tangential component 2
        if( nd.eq.2 )then
          i3=gridIndexRange(0,2)
          i3m=i3
          i3p=i3
          do i3=n3a,n3b
          do i2=n2a,n2b
          do i1=n1a,n1b
           do ghost=1,numberOfGhostPoints
            i1m=i1-is1*ghost
            i2m=i2-is2*ghost
            i1p=i1+is1*ghost
            i2p=i2+is2*ghost
               ! write(*,'(" bcSymmetry: i1m,i2m,i3m,i1p,i2p,i3p,hz=",7i6)')  i1m,i2m,i3m,i1p,i2p,i3p,hz
               u(i1m,i2m,i3m,en )= u(i1p,i2p,i3p,en )   ! normal component is even
               u(i1 ,i2 ,i3 ,et1)= 0.                   ! tangential component is odd
               u(i1m,i2m,i3m,et1)=-u(i1p,i2p,i3p,et1)
               u(i1m,i2m,i3m,hz )= u(i1p,i2p,i3p,hz )   ! Hz is even (Neumann BC)
           end do
          end do
          end do
          end do
        else if( nd.eq.3 )then
          do i3=n3a,n3b
          do i2=n2a,n2b
          do i1=n1a,n1b
           do ghost=1,numberOfGhostPoints
            i1m=i1-is1*ghost
            i2m=i2-is2*ghost
            i3m=i3-is3*ghost
            i1p=i1+is1*ghost
            i2p=i2+is2*ghost
            i3p=i3+is3*ghost
               ! In 3D the tangential components are odd symmetry
               u(i1m,i2m,i3m,en )= u(i1p,i2p,i3p,en )   ! normal component is even
               ! u(i1 ,i2 ,i3 ,et1)= 0.                   ! tangential component is odd
               u(i1m,i2m,i3m,et1)=2.*u(i1 ,i2 ,i3 ,et1) - u(i1p,i2p,
     & i3p,et1)
               ! u(i1 ,i2 ,i3 ,et2)= 0.                   ! tangential component is odd
               u(i1m,i2m,i3m,et2)=2.*u(i1 ,i2 ,i3 ,et2)-u(i1p,i2p,i3p,
     & et2)
           end do
          end do
          end do
          end do
        else
          stop 9467
        end if
        if( forcingOption.eq.twilightZoneForcing )then
         ! -----------------------------------------------------
         ! ----------------- Twilight Zone Forcing -------------
         ! -----------------------------------------------------
         if( ex.ne.0 .or. ey.ne.1 )then
           ! we assume ex==0 etc. below 
           stop 1133
         end if
         if( nd.eq.2 )then
          i3=gridIndexRange(0,2)
          i3m=i3
          i3p=i3
          do i3=n3a,n3b
          do i2=n2a,n2b
          do i1=n1a,n1b
           ! get the exact solution on the boundary: 
           do ghost=1,numberOfGhostPoints
            i1m=i1-is1*ghost
            i2m=i2-is2*ghost
            i1p=i1+is1*ghost
            i2p=i2+is2*ghost
               call ogf2d(ep,xy(i1,i2,i3,0),xy(i1,i2,i3,1),t,uv0(0),
     & uv0(1),uv0(2))
               call ogf2d(ep,xy(i1m,i2m,i3m,0),xy(i1m,i2m,i3m,1),t,uvm(
     & 0),uvm(1),uvm(2))
               call ogf2d(ep,xy(i1p,i2p,i3p,0),xy(i1p,i2p,i3p,1),t,uvp(
     & 0),uvp(1),uvp(2))
              ! normal component should equal uvp(en) from above, so subtract
              ! this off and then add on the TZ value:
              u(i1m,i2m,i3m,en )= u(i1m,i2m,i3m,en ) - uvp(en) + uvm(
     & en)    !  normal component is even
              u(i1 ,i2 ,i3 ,et1)= uv0(et1)                             
     &      ! set tangential component on bndry
              ! tan. component should equal -uvp(et1) from above, so add this on
              ! and then add on the TZ value:
              u(i1m,i2m,i3m,et1)= u(i1m,i2m,i3m,et1) + uvp(et1) + uvm(
     & et1)  ! tangential component is odd
              u(i1m,i2m,i3m,hz )= u(i1m,i2m,i3m,hz ) - uvp(2) + uvm(2) 
     &      ! Hz is even
           end do
          end do
          end do
          end do
         else if( nd.eq.3 )then
          do i3=n3a,n3b
          do i2=n2a,n2b
          do i1=n1a,n1b
           ! get the exact solution on the boundary: 
           do ghost=1,numberOfGhostPoints
            i1m=i1-is1*ghost
            i2m=i2-is2*ghost
            i3m=i3-is3*ghost
            i1p=i1+is1*ghost
            i2p=i2+is2*ghost
            i3p=i3+is3*ghost
               call ogf3d(ep,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,
     & 2),t,uv0(0),uv0(1),uv0(2))
               call ogf3d(ep,xy(i1m,i2m,i3m,0),xy(i1m,i2m,i3m,1),xy(
     & i1m,i2m,i3m,2),t,uvm(0),uvm(1),uvm(2))
               call ogf3d(ep,xy(i1p,i2p,i3p,0),xy(i1p,i2p,i3p,1),xy(
     & i1p,i2p,i3p,2),t,uvp(0),uvp(1),uvp(2))
              u(i1m,i2m,i3m,en )= u(i1m,i2m,i3m,en ) - uvp(en) + uvm(
     & en)
              u(i1 ,i2 ,i3 ,et1)= uv0(et1)                             
     &       ! set tangential component on bndry
              u(i1m,i2m,i3m,et1)= u(i1m,i2m,i3m,et1) + uvp(et1) + uvm(
     & et1)
              u(i1 ,i2 ,i3 ,et2)= uv0(et2)                             
     &       ! set tangential component on bndry
              u(i1m,i2m,i3m,et2)= u(i1m,i2m,i3m,et2) + uvp(et2) + uvm(
     & et2)
           end do
          end do
          end do
          end do
         else
           stop 9467
         end if
        end if ! forcingOption.eq.twilightZoneForcing
       else if( gridType.eq.curvilinear )then
        ! ***********************************************
        ! ************curvilinear grid*******************
        ! ***********************************************
        if( nd.eq.2 )then
          i3=gridIndexRange(0,2)
          i3m=i3
          i3p=i3
          do i3=n3a,n3b
          do i2=n2a,n2b
          do i1=n1a,n1b
           do ghost=1,numberOfGhostPoints
            i1m=i1-is1*ghost
            i2m=i2-is2*ghost
            i1p=i1+is1*ghost
            i2p=i2+is2*ghost
                an1=rsxy(i1,i2,i3,axis,0)
                an2=rsxy(i1,i2,i3,axis,1)
                aNorm = 1./max(epsX,sqrt(an1**2+an2**2))
                an1=an1*aNorm
                an2=an2*aNorm
               ! Set tangential components on boundary to zero: (i.e. keep normal component only)
               nDotU = an1*u(i1,i2,i3,ex) + an2*u(i1,i2,i3,ey)
               u(i1,i2,i3,ex) = nDotU*an1
               u(i1,i2,i3,ey) = nDotU*an2
               ! Ghost: first make all components odd symmetry
               u(i1m,i2m,i3m,ex)=-u(i1p,i2p,i3p,ex)
               u(i1m,i2m,i3m,ey)=-u(i1p,i2p,i3p,ey)
               ! now make normal component even: 
               nDotU = an1*(u(i1p,i2p,i3p,ex)-u(i1m,i2m,i3m,ex)) + an2*
     & (u(i1p,i2p,i3p,ey)-u(i1m,i2m,i3m,ey))
               u(i1m,i2m,i3m,ex)= u(i1m,i2m,i3m,ex) + nDotU*an1
               u(i1m,i2m,i3m,ey)= u(i1m,i2m,i3m,ey) + nDotU*an2
               u(i1m,i2m,i3m,hz)= u(i1p,i2p,i3p,hz)   ! Hz is even
           end do
          end do
          end do
          end do
        else if( nd.eq.3 )then
          do i3=n3a,n3b
          do i2=n2a,n2b
          do i1=n1a,n1b
           do ghost=1,numberOfGhostPoints
            i1m=i1-is1*ghost
            i2m=i2-is2*ghost
            i3m=i3-is3*ghost
            i1p=i1+is1*ghost
            i2p=i2+is2*ghost
            i3p=i3+is3*ghost
                an1=rsxy(i1,i2,i3,axis,0)
                an2=rsxy(i1,i2,i3,axis,1)
                an3=rsxy(i1,i2,i3,axis,2)
                aNorm = 1./max(epsX,sqrt(an1**2+an2**2+an3**2))
                an1=an1*aNorm
                an2=an2*aNorm
                an3=an3*aNorm
               ! In 3D the tangential components are odd symmetry 
               ! nDotU = an1*u(i1,i2,i3,ex) + an2*u(i1,i2,i3,ey)+ an3*u(i1,i2,i3,ez)
               ! u(i1 ,i2 ,i3 ,ex) = nDotU*an1
               ! u(i1 ,i2 ,i3 ,ey) = nDotU*an2
               ! u(i1 ,i2 ,i3 ,ez) = nDotU*an3
               ! Ghost: first make all components odd symmetry
               u(i1m,i2m,i3m,ex)=2.*u(i1 ,i2 ,i3 ,ex)-u(i1p,i2p,i3p,ex)
               u(i1m,i2m,i3m,ey)=2.*u(i1 ,i2 ,i3 ,ey)-u(i1p,i2p,i3p,ey)
               u(i1m,i2m,i3m,ez)=2.*u(i1 ,i2 ,i3 ,ez)-u(i1p,i2p,i3p,ez)
               ! now make normal component even: 
               nDotU = an1*(u(i1p,i2p,i3p,ex)-u(i1m,i2m,i3m,ex)) + an2*
     & (u(i1p,i2p,i3p,ey)-u(i1m,i2m,i3m,ey)) + an3*(u(i1p,i2p,i3p,ez)-
     & u(i1m,i2m,i3m,ez))
               u(i1m,i2m,i3m,ex)= u(i1m,i2m,i3m,ex) + nDotU*an1
               u(i1m,i2m,i3m,ey)= u(i1m,i2m,i3m,ey) + nDotU*an2
               u(i1m,i2m,i3m,ez)= u(i1m,i2m,i3m,ez) + nDotU*an3
           end do
          end do
          end do
          end do
        else
          ! unexpected nd
          stop 1116
        end if
        if( forcingOption.eq.twilightZoneForcing )then
         ! -----------------------------------------------------
         ! ----------------- Twilight Zone Forcing -------------
         ! -----------------------------------------------------
         if( ex.ne.0 .or. ey.ne.1 )then
           ! we assume ex==0 etc. below 
           stop 1133
         end if
         if( nd.eq.2 )then
          i3=gridIndexRange(0,2)
          i3m=i3
          i3p=i3
          do i3=n3a,n3b
          do i2=n2a,n2b
          do i1=n1a,n1b
           do ghost=1,numberOfGhostPoints
            i1m=i1-is1*ghost
            i2m=i2-is2*ghost
            i1p=i1+is1*ghost
            i2p=i2+is2*ghost
               call ogf2d(ep,xy(i1,i2,i3,0),xy(i1,i2,i3,1),t,uv0(0),
     & uv0(1),uv0(2))
               call ogf2d(ep,xy(i1m,i2m,i3m,0),xy(i1m,i2m,i3m,1),t,uvm(
     & 0),uvm(1),uvm(2))
               call ogf2d(ep,xy(i1p,i2p,i3p,0),xy(i1p,i2p,i3p,1),t,uvp(
     & 0),uvp(1),uvp(2))
               an1=rsxy(i1,i2,i3,axis,0)
               an2=rsxy(i1,i2,i3,axis,1)
               aNorm = 1./max(epsX,sqrt(an1**2+an2**2))
               an1=an1*aNorm
               an2=an2*aNorm
              ! Set tangential component (do not change the normal component)
              nDotU = an1*(u(i1,i2,i3,ex)-uv0(0)) + an2*(u(i1,i2,i3,ey)
     & -uv0(1))
              u(i1 ,i2 ,i3 ,ex) = uv0(0) + nDotU*an1
              u(i1 ,i2 ,i3 ,ey) = uv0(1) + nDotU*an2
              ! Ghost: If the ghost were odd symmetry they would equal -uvp, so 
              ! subtract this off and add on true value:
              u(i1m,i2m,i3m,ex )=u(i1m,i2m,i3m,ex ) +uvp(0) + uvm(0)   ! -u(i1p,i2p,i3p,ex )
              u(i1m,i2m,i3m,ey )=u(i1m,i2m,i3m,ey ) +uvp(1) + uvm(1)   ! -u(i1p,i2p,i3p,ey )
              ! now make normal component even: 
              ! The normal component should now be n.( 2*uvp + uvm ) so subtract off n.( 2*uvp )
              nDotU = an1*( uvp(0) ) + an2*( uvp(1) )
              u(i1m,i2m,i3m,ex)= u(i1m,i2m,i3m,ex) - 2.*nDotU*an1
              u(i1m,i2m,i3m,ey)= u(i1m,i2m,i3m,ey) - 2.*nDotU*an2
              ! write(*,'(" i1m,i2=",2i4," ex,ey=",2e10.2," true=",2e10.2)') i1m,i2m,u(i1m,i2m,i3m,ex),u(i1m,i2m,i3m,ey),uvm(0),uvm(1)
              u(i1m,i2m,i3m,hz )= u(i1m,i2m,i3m,hz ) -uvp(2) + uvm(2)   ! Hz is even
           end do
          end do
          end do
          end do
         else if( nd.eq.3 )then
          do i3=n3a,n3b
          do i2=n2a,n2b
          do i1=n1a,n1b
           ! get the exact solution on the boundary: 
           do ghost=1,numberOfGhostPoints
            i1m=i1-is1*ghost
            i2m=i2-is2*ghost
            i3m=i3-is3*ghost
            i1p=i1+is1*ghost
            i2p=i2+is2*ghost
            i3p=i3+is3*ghost
               call ogf3d(ep,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,
     & 2),t,uv0(0),uv0(1),uv0(2))
               call ogf3d(ep,xy(i1m,i2m,i3m,0),xy(i1m,i2m,i3m,1),xy(
     & i1m,i2m,i3m,2),t,uvm(0),uvm(1),uvm(2))
               call ogf3d(ep,xy(i1p,i2p,i3p,0),xy(i1p,i2p,i3p,1),xy(
     & i1p,i2p,i3p,2),t,uvp(0),uvp(1),uvp(2))
               an1=rsxy(i1,i2,i3,axis,0)
               an2=rsxy(i1,i2,i3,axis,1)
               an3=rsxy(i1,i2,i3,axis,2)
               aNorm = 1./max(epsX,sqrt(an1**2+an2**2+an3**2))
               an1=an1*aNorm
               an2=an2*aNorm
               an3=an3*aNorm
              ! Set tangential components (do not change the normal component)
              nDotU = an1*(u(i1,i2,i3,ex)-uv0(0)) + an2*(u(i1,i2,i3,ey)
     & -uv0(1))+ an3*(u(i1,i2,i3,ez)-uv0(2))
              u(i1 ,i2 ,i3 ,ex) = uv0(0) + nDotU*an1
              u(i1 ,i2 ,i3 ,ey) = uv0(1) + nDotU*an2
              u(i1 ,i2 ,i3 ,ez) = uv0(2) + nDotU*an3
              ! Ghost: first make all components odd symmetry
              u(i1m,i2m,i3m,ex)=u(i1m,i2m,i3m,ex) +uvp(0) + uvm(0)   ! -u(i1p,i2p,i3p,ex )
              u(i1m,i2m,i3m,ey)=u(i1m,i2m,i3m,ey) +uvp(1) + uvm(1)   ! -u(i1p,i2p,i3p,ey )
              u(i1m,i2m,i3m,ez)=u(i1m,i2m,i3m,ez) +uvp(2) + uvm(2)   ! -u(i1p,i2p,i3p,ez )
              ! now make normal component even: 
              nDotU = an1*( uvp(0) ) + an2*( uvp(1) ) + an3*( uvp(2) )
              u(i1m,i2m,i3m,ex)= u(i1m,i2m,i3m,ex) - 2.*nDotU*an1
              u(i1m,i2m,i3m,ey)= u(i1m,i2m,i3m,ey) - 2.*nDotU*an2
              u(i1m,i2m,i3m,ez)= u(i1m,i2m,i3m,ez) - 2.*nDotU*an3
           end do
          end do
          end do
          end do
         else
           stop 9467
         end if
        end if ! forcingOption.eq.twilightZoneForcing
        else
          stop 9467
        end if
         end if
       end do
       end do
      end if ! bcOption.eq.0
      if( bcOption.eq.1 )then
      ! ------------------------------------------------------------------------
      ! ------------------ Corners next to Symmetry Boundaries -----------
      ! ------------------------------------------------------------------------
      if( nd.eq.2 )then
       i3=gridIndexRange(0,2)
       i3m=i3
       i3p=i3
       do side1=0,1
       do side2=0,1
        bc1 = boundaryCondition(side1,0)
        bc2 = boundaryCondition(side2,1)
        ! ** note ** treat symmetry-interface corners too
        if( bc1.eq.symmetryBoundaryCondition 
     & .or.bc2.eq.symmetryBoundaryCondition )then
          ! --- We are next to a symmetry corner ---
          is1=1-2*side1
          is2=1-2*side2
          if( bc1.eq.symmetryBoundaryCondition )then
            axis=0  ! treat this axis as the one to apply the symmetry condition to
          else
            axis=1
          end if
          ! Cartesian: 
          en =ex + axis             ! normal component
          et1=ex + mod(axis+1,nd)   ! tangential component 1
          et2=ex + mod(axis+2,nd)   ! tangential component 2
          i1=gridIndexRange(side1,0) ! (i1,i2,i3)=corner point
          i2=gridIndexRange(side2,1)
          do ghost2=1,numberOfGhostPoints
          do ghost1=1,numberOfGhostPoints
           i1m=i1-is1*ghost1  ! assign this corner ghost point (i1m,i2m)
           i2m=i2-is2*ghost2
           i1p=i1m  ! symmetry point is held in (i1p,i2p)
           i2p=i2m
           if( bc1.eq.symmetryBoundaryCondition )then
             i1p=i1+is1*ghost1  ! symmetry pt
           else if( bc2.eq.symmetryBoundaryCondition )then
             i2p=i2+is2*ghost2
           end if
           ! write(*,'("abc :corner symmetry: u(",i6,",",i6,")=u(",i6",",i6")")') i1m,i2m,i1p,i2p
           ! ' 
           ! set the ghost point
           if( gridType.eq.rectangular )then
                ! write(*,'(" bcSymmetry: i1m,i2m,i3m,i1p,i2p,i3p,hz=",7i6)')  i1m,i2m,i3m,i1p,i2p,i3p,hz
                u(i1m,i2m,i3m,en )= u(i1p,i2p,i3p,en )   ! normal component is even
                u(i1 ,i2 ,i3 ,et1)= 0.                   ! tangential component is odd
                u(i1m,i2m,i3m,et1)=-u(i1p,i2p,i3p,et1)
                u(i1m,i2m,i3m,hz )= u(i1p,i2p,i3p,hz )   ! Hz is even (Neumann BC)
             if( forcingOption.eq.twilightZoneForcing )then
                 call ogf2d(ep,xy(i1,i2,i3,0),xy(i1,i2,i3,1),t,uv0(0),
     & uv0(1),uv0(2))
                 call ogf2d(ep,xy(i1m,i2m,i3m,0),xy(i1m,i2m,i3m,1),t,
     & uvm(0),uvm(1),uvm(2))
                 call ogf2d(ep,xy(i1p,i2p,i3p,0),xy(i1p,i2p,i3p,1),t,
     & uvp(0),uvp(1),uvp(2))
                ! normal component should equal uvp(en) from above, so subtract
                ! this off and then add on the TZ value:
                u(i1m,i2m,i3m,en )= u(i1m,i2m,i3m,en ) - uvp(en) + uvm(
     & en)    !  normal component is even
                u(i1 ,i2 ,i3 ,et1)= uv0(et1)                           
     &        ! set tangential component on bndry
                ! tan. component should equal -uvp(et1) from above, so add this on
                ! and then add on the TZ value:
                u(i1m,i2m,i3m,et1)= u(i1m,i2m,i3m,et1) + uvp(et1) + 
     & uvm(et1)  ! tangential component is odd
                u(i1m,i2m,i3m,hz )= u(i1m,i2m,i3m,hz ) - uvp(2) + uvm(
     & 2)      ! Hz is even
             end if
           else
                 an1=rsxy(i1,i2,i3,axis,0)
                 an2=rsxy(i1,i2,i3,axis,1)
                 aNorm = 1./max(epsX,sqrt(an1**2+an2**2))
                 an1=an1*aNorm
                 an2=an2*aNorm
                ! Set tangential components on boundary to zero: (i.e. keep normal component only)
                nDotU = an1*u(i1,i2,i3,ex) + an2*u(i1,i2,i3,ey)
                u(i1,i2,i3,ex) = nDotU*an1
                u(i1,i2,i3,ey) = nDotU*an2
                ! Ghost: first make all components odd symmetry
                u(i1m,i2m,i3m,ex)=-u(i1p,i2p,i3p,ex)
                u(i1m,i2m,i3m,ey)=-u(i1p,i2p,i3p,ey)
                ! now make normal component even: 
                nDotU = an1*(u(i1p,i2p,i3p,ex)-u(i1m,i2m,i3m,ex)) + 
     & an2*(u(i1p,i2p,i3p,ey)-u(i1m,i2m,i3m,ey))
                u(i1m,i2m,i3m,ex)= u(i1m,i2m,i3m,ex) + nDotU*an1
                u(i1m,i2m,i3m,ey)= u(i1m,i2m,i3m,ey) + nDotU*an2
                u(i1m,i2m,i3m,hz)= u(i1p,i2p,i3p,hz)   ! Hz is even
             if( forcingOption.eq.twilightZoneForcing )then
                 call ogf2d(ep,xy(i1,i2,i3,0),xy(i1,i2,i3,1),t,uv0(0),
     & uv0(1),uv0(2))
                 call ogf2d(ep,xy(i1m,i2m,i3m,0),xy(i1m,i2m,i3m,1),t,
     & uvm(0),uvm(1),uvm(2))
                 call ogf2d(ep,xy(i1p,i2p,i3p,0),xy(i1p,i2p,i3p,1),t,
     & uvp(0),uvp(1),uvp(2))
                 an1=rsxy(i1,i2,i3,axis,0)
                 an2=rsxy(i1,i2,i3,axis,1)
                 aNorm = 1./max(epsX,sqrt(an1**2+an2**2))
                 an1=an1*aNorm
                 an2=an2*aNorm
                ! Set tangential component (do not change the normal component)
                nDotU = an1*(u(i1,i2,i3,ex)-uv0(0)) + an2*(u(i1,i2,i3,
     & ey)-uv0(1))
                u(i1 ,i2 ,i3 ,ex) = uv0(0) + nDotU*an1
                u(i1 ,i2 ,i3 ,ey) = uv0(1) + nDotU*an2
                ! Ghost: If the ghost were odd symmetry they would equal -uvp, so 
                ! subtract this off and add on true value:
                u(i1m,i2m,i3m,ex )=u(i1m,i2m,i3m,ex ) +uvp(0) + uvm(0) 
     &   ! -u(i1p,i2p,i3p,ex )
                u(i1m,i2m,i3m,ey )=u(i1m,i2m,i3m,ey ) +uvp(1) + uvm(1) 
     &   ! -u(i1p,i2p,i3p,ey )
                ! now make normal component even: 
                ! The normal component should now be n.( 2*uvp + uvm ) so subtract off n.( 2*uvp )
                nDotU = an1*( uvp(0) ) + an2*( uvp(1) )
                u(i1m,i2m,i3m,ex)= u(i1m,i2m,i3m,ex) - 2.*nDotU*an1
                u(i1m,i2m,i3m,ey)= u(i1m,i2m,i3m,ey) - 2.*nDotU*an2
                ! write(*,'(" i1m,i2=",2i4," ex,ey=",2e10.2," true=",2e10.2)') i1m,i2m,u(i1m,i2m,i3m,ex),u(i1m,i2m,i3m,ey),uvm(0),uvm(1)
                u(i1m,i2m,i3m,hz )= u(i1m,i2m,i3m,hz ) -uvp(2) + uvm(2)
     &    ! Hz is even
             end if
           end if
          end do ! ghost1
          end do ! ghost2
        end if
       end do
       end do
      else ! ***** 3D *****
       if( .true. )then
          do edgeDirection=0,2 ! direction parallel to the edge
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
          do ghost1=1,numberOfGhostPoints
          do ghost2=1,numberOfGhostPoints
           ! shift to ghost point "(ghost1,ghost2)"
           ks1=0
           ks2=0
           ks3=0
           if( edgeDirection.eq.2 )then
             js1=is1*ghost1
             js2=is2*ghost2
             js3=0
             ! Note: no need to consider the case when both adjacent faces are symmetry since the ghost
             ! pts outside the face will have already been set to the interior values : thus instead of
             ! setting u(-1,-1)=u(1,1) we can just set u(-1,-1)=u(1,-1) (or u(-1,-1)=u(-1,1))
             if( bc1.eq.symmetryBoundaryCondition )then
               ! u(-js1,-js2,0)=u( ks1, ks2,0)
               !               =u( js1,-js2,0)
               ks1= js1
               ks2=-js2  ! *wdh* 090720  -- fixed this, and below as well --
               axis=0    ! symmetry axis
             else if( bc2.eq.symmetryBoundaryCondition )then
               ! u(-js1,-js2,0)=u(-js1, js2,0)
               ks1=-js1
               ks2= js2
               axis=1   ! symmetry axis
             end if
           else if( edgeDirection.eq.1 )then
             js1=is1*ghost1
             js2=0
             js3=is3*ghost2
             if( bc1.eq.symmetryBoundaryCondition )then
               ks1= js1
               ks3=-js3
               axis=0   ! symmetry axis
             else if( bc2.eq.symmetryBoundaryCondition )then
               ks1=-js1
               ks3= js3
               axis=2   ! symmetry axis
             end if
           else
             js1=0
             js2=is2*ghost1
             js3=is3*ghost2
             if( bc1.eq.symmetryBoundaryCondition )then
               ks2= js2
               ks3=-js3
               axis=1 ! symmetry axis
             else if( bc2.eq.symmetryBoundaryCondition )then
               ks2=-js2
               ks3= js3
               axis=2 ! symmetry axis
             end if
           end if
           if( bc1.eq.symmetryBoundaryCondition 
     & .or.bc2.eq.symmetryBoundaryCondition )then
         !   if( (bc1.eq.symmetryBoundaryCondition .and. bc2.gt.0) .or.!       (bc2.eq.symmetryBoundaryCondition .and. bc1.gt.0) )then
            ! *********************************************************
            ! ************* SYMMETRY EDGE BC **************************
            ! *********************************************************
            ! Cartesian: 
            en =ex + axis             ! normal component
            et1=ex + mod(axis+1,nd)   ! tangential component 1
            et2=ex + mod(axis+2,nd)   ! tangential component 2
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
               i1m=i1-js1  ! assign this corner ghost point (i1m,i2m,i3m)
               i2m=i2-js2
               i3m=i3-js3
               i1p=i1+ks1   ! symmetry point
               i2p=i2+ks2
               i3p=i3+ks3
               ! if( i1.eq.n1a .and. i2.eq.n2a .and. i3.eq.n3a )then
               ! write(*,'("abc :3d edge symmetry: u(",i6,",",i6,",",i6,")=u(",i6,",",i6,",",i6)') i1m,i2m,i3m,i1p,i2p,i3p
               ! ' 
               ! end if
               if( gridType.eq.rectangular )then
                    ! In 3D the tangential components are odd symmetry
                    u(i1m,i2m,i3m,en )= u(i1p,i2p,i3p,en )   ! normal component is even
                    ! u(i1 ,i2 ,i3 ,et1)= 0.                   ! tangential component is odd
                    u(i1m,i2m,i3m,et1)=2.*u(i1 ,i2 ,i3 ,et1) - u(i1p,
     & i2p,i3p,et1)
                    ! u(i1 ,i2 ,i3 ,et2)= 0.                   ! tangential component is odd
                    u(i1m,i2m,i3m,et2)=2.*u(i1 ,i2 ,i3 ,et2)-u(i1p,i2p,
     & i3p,et2)
                 if( forcingOption.eq.twilightZoneForcing )then
                     call ogf3d(ep,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,
     & i2,i3,2),t,uv0(0),uv0(1),uv0(2))
                     call ogf3d(ep,xy(i1m,i2m,i3m,0),xy(i1m,i2m,i3m,1),
     & xy(i1m,i2m,i3m,2),t,uvm(0),uvm(1),uvm(2))
                     call ogf3d(ep,xy(i1p,i2p,i3p,0),xy(i1p,i2p,i3p,1),
     & xy(i1p,i2p,i3p,2),t,uvp(0),uvp(1),uvp(2))
                    u(i1m,i2m,i3m,en )= u(i1m,i2m,i3m,en ) - uvp(en) + 
     & uvm(en)
                    u(i1 ,i2 ,i3 ,et1)= uv0(et1)                       
     &             ! set tangential component on bndry
                    u(i1m,i2m,i3m,et1)= u(i1m,i2m,i3m,et1) + uvp(et1) +
     &  uvm(et1)
                    u(i1 ,i2 ,i3 ,et2)= uv0(et2)                       
     &             ! set tangential component on bndry
                    u(i1m,i2m,i3m,et2)= u(i1m,i2m,i3m,et2) + uvp(et2) +
     &  uvm(et2)
                 end if
               else
                     an1=rsxy(i1,i2,i3,axis,0)
                     an2=rsxy(i1,i2,i3,axis,1)
                     an3=rsxy(i1,i2,i3,axis,2)
                     aNorm = 1./max(epsX,sqrt(an1**2+an2**2+an3**2))
                     an1=an1*aNorm
                     an2=an2*aNorm
                     an3=an3*aNorm
                    ! In 3D the tangential components are odd symmetry 
                    ! nDotU = an1*u(i1,i2,i3,ex) + an2*u(i1,i2,i3,ey)+ an3*u(i1,i2,i3,ez)
                    ! u(i1 ,i2 ,i3 ,ex) = nDotU*an1
                    ! u(i1 ,i2 ,i3 ,ey) = nDotU*an2
                    ! u(i1 ,i2 ,i3 ,ez) = nDotU*an3
                    ! Ghost: first make all components odd symmetry
                    u(i1m,i2m,i3m,ex)=2.*u(i1 ,i2 ,i3 ,ex)-u(i1p,i2p,
     & i3p,ex)
                    u(i1m,i2m,i3m,ey)=2.*u(i1 ,i2 ,i3 ,ey)-u(i1p,i2p,
     & i3p,ey)
                    u(i1m,i2m,i3m,ez)=2.*u(i1 ,i2 ,i3 ,ez)-u(i1p,i2p,
     & i3p,ez)
                    ! now make normal component even: 
                    nDotU = an1*(u(i1p,i2p,i3p,ex)-u(i1m,i2m,i3m,ex)) +
     &  an2*(u(i1p,i2p,i3p,ey)-u(i1m,i2m,i3m,ey)) + an3*(u(i1p,i2p,
     & i3p,ez)-u(i1m,i2m,i3m,ez))
                    u(i1m,i2m,i3m,ex)= u(i1m,i2m,i3m,ex) + nDotU*an1
                    u(i1m,i2m,i3m,ey)= u(i1m,i2m,i3m,ey) + nDotU*an2
                    u(i1m,i2m,i3m,ez)= u(i1m,i2m,i3m,ez) + nDotU*an3
                 if( forcingOption.eq.twilightZoneForcing )then
                     call ogf3d(ep,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,
     & i2,i3,2),t,uv0(0),uv0(1),uv0(2))
                     call ogf3d(ep,xy(i1m,i2m,i3m,0),xy(i1m,i2m,i3m,1),
     & xy(i1m,i2m,i3m,2),t,uvm(0),uvm(1),uvm(2))
                     call ogf3d(ep,xy(i1p,i2p,i3p,0),xy(i1p,i2p,i3p,1),
     & xy(i1p,i2p,i3p,2),t,uvp(0),uvp(1),uvp(2))
                     an1=rsxy(i1,i2,i3,axis,0)
                     an2=rsxy(i1,i2,i3,axis,1)
                     an3=rsxy(i1,i2,i3,axis,2)
                     aNorm = 1./max(epsX,sqrt(an1**2+an2**2+an3**2))
                     an1=an1*aNorm
                     an2=an2*aNorm
                     an3=an3*aNorm
                    ! Set tangential components (do not change the normal component)
                    nDotU = an1*(u(i1,i2,i3,ex)-uv0(0)) + an2*(u(i1,i2,
     & i3,ey)-uv0(1))+ an3*(u(i1,i2,i3,ez)-uv0(2))
                    u(i1 ,i2 ,i3 ,ex) = uv0(0) + nDotU*an1
                    u(i1 ,i2 ,i3 ,ey) = uv0(1) + nDotU*an2
                    u(i1 ,i2 ,i3 ,ez) = uv0(2) + nDotU*an3
                    ! Ghost: first make all components odd symmetry
                    u(i1m,i2m,i3m,ex)=u(i1m,i2m,i3m,ex) +uvp(0) + uvm(
     & 0)   ! -u(i1p,i2p,i3p,ex )
                    u(i1m,i2m,i3m,ey)=u(i1m,i2m,i3m,ey) +uvp(1) + uvm(
     & 1)   ! -u(i1p,i2p,i3p,ey )
                    u(i1m,i2m,i3m,ez)=u(i1m,i2m,i3m,ez) +uvp(2) + uvm(
     & 2)   ! -u(i1p,i2p,i3p,ez )
                    ! now make normal component even: 
                    nDotU = an1*( uvp(0) ) + an2*( uvp(1) ) + an3*( 
     & uvp(2) )
                    u(i1m,i2m,i3m,ex)= u(i1m,i2m,i3m,ex) - 2.*nDotU*an1
                    u(i1m,i2m,i3m,ey)= u(i1m,i2m,i3m,ey) - 2.*nDotU*an2
                    u(i1m,i2m,i3m,ez)= u(i1m,i2m,i3m,ez) - 2.*nDotU*an3
                 end if
               end if
             end do ! end do i1
             end do ! end do i2
             end do ! end do i3
           end if
          end do ! ghost1
          end do ! ghost2
         end do ! sideb
         end do ! sidea
         end do ! edgeDirection
          do side3=0,1
          do side2=0,1
          do side1=0,1
           bc1 = boundaryCondition(side1,0)
           bc2 = boundaryCondition(side2,1)
           bc3 = boundaryCondition(side3,2)
           if( bc1.eq.symmetryBoundaryCondition 
     & .or.bc2.eq.symmetryBoundaryCondition 
     & .or.bc3.eq.symmetryBoundaryCondition )then
             ! --- symmetry corner ---
             is1=1-2*side1
             is2=1-2*side2
             is3=1-2*side3
             i1=gridIndexRange(side1,0) ! (i1,i2,i3)=corner point
             i2=gridIndexRange(side2,1)
             i3=gridIndexRange(side3,2)
             do ghost3=1,numberOfGhostPoints
             do ghost2=1,numberOfGhostPoints
             do ghost1=1,numberOfGhostPoints
              i1m=i1-is1*ghost1  ! assign this corner ghost point (i1m,i2m,i3m)
              i2m=i2-is2*ghost2
              i3m=i3-is3*ghost3
              i1p=i1m  ! symmetry point is held in (i1p,i2p,i3p)
              i2p=i2m
              i3p=i3m
              if( bc1.eq.symmetryBoundaryCondition )then
                i1p=i1+is1*ghost1  ! symmetry pt
                axis=0 ! symmetry axis
              else if( bc2.eq.symmetryBoundaryCondition )then
                i2p=i2+is2*ghost2
                axis=1 ! symmetry axis
              else if( bc3.eq.symmetryBoundaryCondition )then
                i3p=i3+is3*ghost3
                axis=2 ! symmetry axis
              end if
              ! write(*,'("abc :3d corner symmetry: u(",i6,",",i6,",",i6,")=u(",i6,",",i6,",",i6)') i1m,i2m,i3m,i1p,i2p,i3p
              ! ' 
              ! Cartesian: 
              en =ex + axis             ! normal component
              et1=ex + mod(axis+1,nd)   ! tangential component 1
              et2=ex + mod(axis+2,nd)   ! tangential component 2
              if( gridType.eq.rectangular )then
                   ! In 3D the tangential components are odd symmetry
                   u(i1m,i2m,i3m,en )= u(i1p,i2p,i3p,en )   ! normal component is even
                   ! u(i1 ,i2 ,i3 ,et1)= 0.                   ! tangential component is odd
                   u(i1m,i2m,i3m,et1)=2.*u(i1 ,i2 ,i3 ,et1) - u(i1p,
     & i2p,i3p,et1)
                   ! u(i1 ,i2 ,i3 ,et2)= 0.                   ! tangential component is odd
                   u(i1m,i2m,i3m,et2)=2.*u(i1 ,i2 ,i3 ,et2)-u(i1p,i2p,
     & i3p,et2)
                if( forcingOption.eq.twilightZoneForcing )then
                    call ogf3d(ep,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,
     & i2,i3,2),t,uv0(0),uv0(1),uv0(2))
                    call ogf3d(ep,xy(i1m,i2m,i3m,0),xy(i1m,i2m,i3m,1),
     & xy(i1m,i2m,i3m,2),t,uvm(0),uvm(1),uvm(2))
                    call ogf3d(ep,xy(i1p,i2p,i3p,0),xy(i1p,i2p,i3p,1),
     & xy(i1p,i2p,i3p,2),t,uvp(0),uvp(1),uvp(2))
                   u(i1m,i2m,i3m,en )= u(i1m,i2m,i3m,en ) - uvp(en) + 
     & uvm(en)
                   u(i1 ,i2 ,i3 ,et1)= uv0(et1)                        
     &            ! set tangential component on bndry
                   u(i1m,i2m,i3m,et1)= u(i1m,i2m,i3m,et1) + uvp(et1) + 
     & uvm(et1)
                   u(i1 ,i2 ,i3 ,et2)= uv0(et2)                        
     &            ! set tangential component on bndry
                   u(i1m,i2m,i3m,et2)= u(i1m,i2m,i3m,et2) + uvp(et2) + 
     & uvm(et2)
                end if
              else
                    an1=rsxy(i1,i2,i3,axis,0)
                    an2=rsxy(i1,i2,i3,axis,1)
                    an3=rsxy(i1,i2,i3,axis,2)
                    aNorm = 1./max(epsX,sqrt(an1**2+an2**2+an3**2))
                    an1=an1*aNorm
                    an2=an2*aNorm
                    an3=an3*aNorm
                   ! In 3D the tangential components are odd symmetry 
                   ! nDotU = an1*u(i1,i2,i3,ex) + an2*u(i1,i2,i3,ey)+ an3*u(i1,i2,i3,ez)
                   ! u(i1 ,i2 ,i3 ,ex) = nDotU*an1
                   ! u(i1 ,i2 ,i3 ,ey) = nDotU*an2
                   ! u(i1 ,i2 ,i3 ,ez) = nDotU*an3
                   ! Ghost: first make all components odd symmetry
                   u(i1m,i2m,i3m,ex)=2.*u(i1 ,i2 ,i3 ,ex)-u(i1p,i2p,
     & i3p,ex)
                   u(i1m,i2m,i3m,ey)=2.*u(i1 ,i2 ,i3 ,ey)-u(i1p,i2p,
     & i3p,ey)
                   u(i1m,i2m,i3m,ez)=2.*u(i1 ,i2 ,i3 ,ez)-u(i1p,i2p,
     & i3p,ez)
                   ! now make normal component even: 
                   nDotU = an1*(u(i1p,i2p,i3p,ex)-u(i1m,i2m,i3m,ex)) + 
     & an2*(u(i1p,i2p,i3p,ey)-u(i1m,i2m,i3m,ey)) + an3*(u(i1p,i2p,i3p,
     & ez)-u(i1m,i2m,i3m,ez))
                   u(i1m,i2m,i3m,ex)= u(i1m,i2m,i3m,ex) + nDotU*an1
                   u(i1m,i2m,i3m,ey)= u(i1m,i2m,i3m,ey) + nDotU*an2
                   u(i1m,i2m,i3m,ez)= u(i1m,i2m,i3m,ez) + nDotU*an3
                if( forcingOption.eq.twilightZoneForcing )then
                    call ogf3d(ep,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,
     & i2,i3,2),t,uv0(0),uv0(1),uv0(2))
                    call ogf3d(ep,xy(i1m,i2m,i3m,0),xy(i1m,i2m,i3m,1),
     & xy(i1m,i2m,i3m,2),t,uvm(0),uvm(1),uvm(2))
                    call ogf3d(ep,xy(i1p,i2p,i3p,0),xy(i1p,i2p,i3p,1),
     & xy(i1p,i2p,i3p,2),t,uvp(0),uvp(1),uvp(2))
                    an1=rsxy(i1,i2,i3,axis,0)
                    an2=rsxy(i1,i2,i3,axis,1)
                    an3=rsxy(i1,i2,i3,axis,2)
                    aNorm = 1./max(epsX,sqrt(an1**2+an2**2+an3**2))
                    an1=an1*aNorm
                    an2=an2*aNorm
                    an3=an3*aNorm
                   ! Set tangential components (do not change the normal component)
                   nDotU = an1*(u(i1,i2,i3,ex)-uv0(0)) + an2*(u(i1,i2,
     & i3,ey)-uv0(1))+ an3*(u(i1,i2,i3,ez)-uv0(2))
                   u(i1 ,i2 ,i3 ,ex) = uv0(0) + nDotU*an1
                   u(i1 ,i2 ,i3 ,ey) = uv0(1) + nDotU*an2
                   u(i1 ,i2 ,i3 ,ez) = uv0(2) + nDotU*an3
                   ! Ghost: first make all components odd symmetry
                   u(i1m,i2m,i3m,ex)=u(i1m,i2m,i3m,ex) +uvp(0) + uvm(0)
     &    ! -u(i1p,i2p,i3p,ex )
                   u(i1m,i2m,i3m,ey)=u(i1m,i2m,i3m,ey) +uvp(1) + uvm(1)
     &    ! -u(i1p,i2p,i3p,ey )
                   u(i1m,i2m,i3m,ez)=u(i1m,i2m,i3m,ez) +uvp(2) + uvm(2)
     &    ! -u(i1p,i2p,i3p,ez )
                   ! now make normal component even: 
                   nDotU = an1*( uvp(0) ) + an2*( uvp(1) ) + an3*( uvp(
     & 2) )
                   u(i1m,i2m,i3m,ex)= u(i1m,i2m,i3m,ex) - 2.*nDotU*an1
                   u(i1m,i2m,i3m,ey)= u(i1m,i2m,i3m,ey) - 2.*nDotU*an2
                   u(i1m,i2m,i3m,ez)= u(i1m,i2m,i3m,ez) - 2.*nDotU*an3
                end if
              end if
             end do ! ghost1
             end do ! ghost2
             end do ! ghost3
           end if
          end do   ! end side1
          end do   ! end side2
          end do   ! end side3
       end if
      end if
      end if ! bcOption.eq.1
      return
      end




      subroutine bcSymmetry( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     & gridIndexRange, u, mask,rsxy, xy,bc, boundaryCondition, ipar, 
     & rpar, ierr )
! ===================================================================================
!  Symmetry Boundary Conditions for Maxwell's Equations.
!
!  gridType : 0=rectangular, 1=curvilinear
!  bcOption=ipar(26) : 0=assign all faces, 1=assign corners and edges
!
!  u : solution at time t
!
! ===================================================================================

      implicit none

      integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, ndf1a,ndf1b,ndf2a,
     & ndf2b,ndf3a,ndf3b,n1a,n1b,n2a,n2b,n3a,n3b, ndc, bc,ierr

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      integer gridIndexRange(0:1,0:2)

      integer ipar(0:*),boundaryCondition(0:1,0:2)
      real rpar(0:*)

      integer symmetryOption

      symmetryOption =ipar(27)


      if( symmetryOption.eq.0 )then
        ! even symmetry
        call bcSymmetryEven( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     & gridIndexRange, u, mask,rsxy, xy,bc, boundaryCondition, ipar, 
     & rpar, ierr )
      else if( symmetryOption.eq.1 )then
        ! even-odd symmetry
        call bcSymmetryEvenOdd( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     & gridIndexRange, u, mask,rsxy, xy,bc, boundaryCondition, ipar, 
     & rpar, ierr )
      else
        write(*,'("bcSymmetry:ERROR: unknown symmetryOption=",i6)') 
     & symmetryOption
      end if

      return
      end
