! This file automatically generated from smoothOpt.bf with bpp.




c update variable coefficient case













! buildFile(smoothRB2dOrder2,2,2)
! buildFile(smoothRB2dOrder4,2,4)
! buildFile(smoothRB3dOrder2,3,2)
! buildFile(smoothRB3dOrder4,3,4)


      subroutine smoothRedBlackOpt( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &    n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c, ndc, f, c, u, v,
     & mask, option, order, sparseStencil, cc, s, dx, omega, 
     & useLocallyOptimalOmega,
     &    variableOmegaScaleFactor, ipar, rpar )
c ===================================================================================
c  Optimised Red-black smooth
c
c  option:  0 : red-points
c           1 : black-points
c
c
c  cc(m) : constant coefficients
c  sparseStencil : general=0, sparse=1, constantCoefficients=2, sparseConstantCoefficients=3
c ===================================================================================

      implicit none
      integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &        n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c, ndc,
     &  option, sparseStencil,order,useLocallyOptimalOmega

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real v(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real f(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real c(1:ndc,nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real s(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real cc(1:*),dx(*),omega,variableOmegaScaleFactor
      integer ipar(0:*)
      real rpar(0:*)

c..........local
      real c1,c2,c3,cmin,cmax,variableOmegaFactor
      integer numberOfSmooths,cycleType

      integer i1,i2,i3,i1s,i2s,i3s,j1,j2,j3,n1d,n
      integer m11,m12,m13,m14,m15,
     &        m21,m22,m23,m24,m25,
     &        m31,m32,m33,m34,m35,
     &        m41,m42,m43,m44,m45,
     &        m51,m52,m53,m54,m55
      integer    m111,m211,m311,m411,m511,
     &           m121,m221,m321,m421,m521,
     &           m131,m231,m331,m431,m531,
     &           m141,m241,m341,m441,m541,
     &           m151,m251,m351,m451,m551,
     &           m112,m212,m312,m412,m512,
     &           m122,m222,m322,m422,m522,
     &           m132,m232,m332,m432,m532,
     &           m142,m242,m342,m442,m542,
     &           m152,m252,m352,m452,m552,
     &           m113,m213,m313,m413,m513,
     &           m123,m223,m323,m423,m523,
     &           m133,m233,m333,m433,m533,
     &           m143,m243,m343,m443,m543,
     &           m153,m253,m353,m453,m553,
     &           m114,m214,m314,m414,m514,
     &           m124,m224,m324,m424,m524,
     &           m134,m234,m334,m434,m534,
     &           m144,m244,m344,m444,m544,
     &           m154,m254,m354,m454,m554,
     &           m115,m215,m315,m415,m515,
     &           m125,m225,m325,m425,m525,
     &           m135,m235,m335,m435,m535,
     &           m145,m245,m345,m445,m545,
     &           m155,m255,m355,m455,m555

      integer    m11n,m21n,m31n,m41n,m51n,
     &           m12n,m22n,m32n,m42n,m52n,
     &           m13n,m23n,m33n,m43n,m53n,
     &           m14n,m24n,m34n,m44n,m54n,
     &           m15n,m25n,m35n,m45n,m55n

      real eps

      integer general, sparse, constantCoefficients,
     &   sparseConstantCoefficients,sparseVariableCoefficients,
     &   variableCoefficients
      parameter( general=0,
     &           sparse=1,
     &           constantCoefficients=2,
     &           sparseConstantCoefficients=3,
     &           sparseVariableCoefficients=4,
     &           variableCoefficients=5 )


      eps=1.e-30 ! *****


c$$$      ipar(0)=order
c$$$      ipar(1)=sparseStencil
c$$$      ipar(2)=useLocallyOptimalOmega
c$$$      ipar(3)=boundaryLayers ! number of layers of boundary points to smooth
c$$$      rpar(0)=omega
c$$$      rpar(1)=variableOmegaScaleFactor

      numberOfSmooths=ipar(4) ! total number of smooths per cycle -- used to determine omega
      cycleType=ipar(5)       ! cycleType: 0=F, 1=V, 2+W, ...

      if( order.ne.2 .and. order.ne.4 )then
        write(*,*) 'smoothOpt:ERROR: invalid order=',order
        stop 1
      end if

c     dx2i=.5/dx(1)**2
c     dy2i=.5/dx(2)**2
c     dz2i=.5/dx(3)**2

      ! scale factor for the locally optimal omega
      if( order.eq.2 )then
        variableOmegaFactor=2.*variableOmegaScaleFactor*.98 ! NOTE
      else
        variableOmegaFactor=variableOmegaScaleFactor
      end if

      if( omega.lt.0. )then
        ! choose defaults
        if( order.eq.2 )then
          if( nd.eq.2 )then
            ! 030721 omega=1.1   ! 1.07
            omega=1.09 ! 1.085  ! W[2,1]
          else
            cmax=1.-1./3.
            omega=variableOmegaFactor/(1.+sqrt(1.-cmax**2))
            ! write(*,'("redBlack: 3D: omega=",f6.4)') omega
            ! omega=1.15 ! for 3d
          end if
        else ! fourth-order accurate
          if( nd.eq.2 )then
            omega=1.15   !
          else
            omega=1.20 ! what should this be ?
            ! experimentally determined for V(1,1) rbj  *wdh* 100722
            omega=1.15  ! NOTE change other value too
          end if
        end if
      end if

      ! write(*,*) 'smoothRB: omega=',omega

      if( nd.eq.2 )then

        if( order.eq.2 )then

          call smoothRB2dOrder2( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &    n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c, ndc, f, c, u, v,
     & mask, option, order, sparseStencil, cc, s, dx, omega, 
     & useLocallyOptimalOmega,
     &    variableOmegaScaleFactor, ipar, rpar )


        else
         ! 4th order

          call smoothRB2dOrder4( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &    n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c, ndc, f, c, u, v,
     & mask, option, order, sparseStencil, cc, s, dx, omega, 
     & useLocallyOptimalOmega,
     &    variableOmegaScaleFactor, ipar, rpar )

        end if


      else

c     ****************       
c     ***** 3D *******       
c     ****************       

        if( order.eq.2 )then

          call smoothRB3dOrder2( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &    n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c, ndc, f, c, u, v,
     & mask, option, order, sparseStencil, cc, s, dx, omega, 
     & useLocallyOptimalOmega,
     &    variableOmegaScaleFactor, ipar, rpar )


        else
         ! 4th order

          call smoothRB3dOrder4( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &    n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c, ndc, f, c, u, v,
     & mask, option, order, sparseStencil, cc, s, dx, omega, 
     & useLocallyOptimalOmega,
     &    variableOmegaScaleFactor, ipar, rpar )


        end if

      end if

      return
      end





c smooth points on the boundary





c update variable coefficient case








! buildFile(smoothJAC2dOrder2,2,2)
! buildFile(smoothJAC2dOrder4,2,4)
! buildFile(smoothJAC3dOrder2,3,2)
! buildFile(smoothJAC3dOrder4,3,4)


      subroutine smoothJacobiOpt( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &    n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c, ndc, f, c, u, v,
     &    mask, option, order, sparseStencil, cc, s, dx, omega0, bc,
     &    np, ndip,ip, ipar )
c ===================================================================================
c  Optimised Jacobi and Gauss-Seidel
c
c  option: 0 : jacobi (solution returned in u, v is a temporary space)
c          1 : Gauss-Seidel
c          2 : jacobi on boundaries where bc(side,axis)>0 (solution returned in u, v is a temporary space)
c          3 : Gauss-Seidel on boundaries where bc(side,axis)>0 (solution returned in u, v is a temporary space)
c          4 : Gauss-Seidel on a list of points (ip)
c          5 : Jacobi on a list of points (ip) (*wdh* added 100114)
c
c  cc(m) : constant coefficients
c  sparseStencil : general=0, sparse=1, constantCoefficients=2, sparseConstantCoefficients=3
c
c  ip(ndip,0:nd) : (ip(i,0:nd-1), i=1,...,np) : list of points for option=4
c ===================================================================================

      implicit none
      integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &        n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c, ndc,
     &  option, sparseStencil,order
      integer ipar(0:*)

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b),bc(0:1,0:2)
      integer np,ndip,ip(1:ndip,0:*)
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real v(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real f(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real c(1:ndc,nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real s(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real cc(1:*),dx(*),omega0,omega

      integer i1,i2,i3,n,nb,boundaryLayers,i
      integer m11,m12,m13,m14,m15,
     &        m21,m22,m23,m24,m25,
     &        m31,m32,m33,m34,m35,
     &        m41,m42,m43,m44,m45,
     &        m51,m52,m53,m54,m55
      integer    m111,m211,m311,m411,m511,
     &           m121,m221,m321,m421,m521,
     &           m131,m231,m331,m431,m531,
     &           m141,m241,m341,m441,m541,
     &           m151,m251,m351,m451,m551,
     &           m112,m212,m312,m412,m512,
     &           m122,m222,m322,m422,m522,
     &           m132,m232,m332,m432,m532,
     &           m142,m242,m342,m442,m542,
     &           m152,m252,m352,m452,m552,
     &           m113,m213,m313,m413,m513,
     &           m123,m223,m323,m423,m523,
     &           m133,m233,m333,m433,m533,
     &           m143,m243,m343,m443,m543,
     &           m153,m253,m353,m453,m553,
     &           m114,m214,m314,m414,m514,
     &           m124,m224,m324,m424,m524,
     &           m134,m234,m334,m434,m534,
     &           m144,m244,m344,m444,m544,
     &           m154,m254,m354,m454,m554,
     &           m115,m215,m315,m415,m515,
     &           m125,m225,m325,m425,m525,
     &           m135,m235,m335,m435,m535,
     &           m145,m245,m345,m445,m545,
     &           m155,m255,m355,m455,m555

      integer    m11n,m21n,m31n,m41n,m51n,
     &           m12n,m22n,m32n,m42n,m52n,
     &           m13n,m23n,m33n,m43n,m53n,
     &           m14n,m24n,m34n,m44n,m54n,
     &           m15n,m25n,m35n,m45n,m55n

      real eps

      integer general, sparse, constantCoefficients,
     &   sparseConstantCoefficients,sparseVariableCoefficients,
     &   variableCoefficients
      parameter( general=0,
     &           sparse=1,
     &           constantCoefficients=2,
     &           sparseConstantCoefficients=3,
     &           sparseVariableCoefficients=4,
     &           variableCoefficients=5 )


      boundaryLayers=ipar(0) ! number of boundary layers to smooth
      nb=boundaryLayers-1  ! number of extra boundary lines to smooth


      if( order.ne.2 .and. order.ne.4 )then
        write(*,*) 'smoothOpt:ERROR: invalid order=',order
        stop 1
      end if

      if( nd.eq.2 )then

        if( order.eq.2 )then

          call smoothJAC2dOrder2( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &    n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c, ndc, f, c, u, v,
     &    mask, option, order, sparseStencil, cc, s, dx, omega0, bc,
     &    np, ndip,ip, ipar )


        else

         ! 4th order
          call smoothJAC2dOrder4( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &    n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c, ndc, f, c, u, v,
     &    mask, option, order, sparseStencil, cc, s, dx, omega0, bc,
     &    np, ndip,ip, ipar )

        end if

      else


c     ****************       
c     ***** 3D *******       
c     ****************       

        if( order.eq.2 )then

          call smoothJAC3dOrder2( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &    n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c, ndc, f, c, u, v,
     &    mask, option, order, sparseStencil, cc, s, dx, omega0, bc,
     &    np, ndip,ip, ipar )


        else

         ! 4th order
          call smoothJAC3dOrder4( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &    n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c, ndc, f, c, u, v,
     &    mask, option, order, sparseStencil, cc, s, dx, omega0, bc,
     &    np, ndip,ip, ipar )

        end if

      end if


      return
      end



! ===========================================================================================
! Macro to make a list of interp neighbours using the interpolationPoint array
! DIM is 2 or 3
! ===========================================================================================

      subroutine getInterpNeighbours( nd, nd1a,nd1b,nd2a,nd2b,nd3a,
     & nd3b,
     &    n1a,n1b,n2a,n2b,n3a,n3b,
     &    mask,mask2, nip,ndip,ip, nipn,ndipn,ipn, width, ierr )
c ===================================================================================
c IBS (interpolation boundary smoothing) routine: 
c  Make a list of discretization points that are within "width" points of an interpolation point
c
c   --- This version uses the interpolationPoint array and only works in serial ---
c
c Input
c  ip(ndip,0:nd) : (ip(i,0:nd-1), i=1,...,nip) : list of interpolation points
c  mask : mask array
c  mask2 : workspace
c Outout 
c  ipn(ndipn,0:nd) : (ipn(i,0:nd-1), i=1,...,nipn) : list of interpolation neighbours
c ===================================================================================

      implicit none
      integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,width,n1a,n1b,n2a,n2b,
     & n3a,n3b

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer mask2(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)

      integer nip,ndip,ip(1:ndip,0:*),ierr
      integer nipn,ndipn,ipn(1:ndipn,0:*)

c.............local
      integer i1,i2,i3,j1,j2,j3,i
      integer j1a,j1b,j2a,j2b,j3a,j3b


      ierr=0
      nipn=0

      if( nd.eq.2 )then

! interpNeighboursMacro(2)
         i3=0
         j3=0
         do i=1,nip
           i1=ip(i,0)
           i2=ip(i,1)
!  #If "2" == "3"
           j1a=max(n1a,i1-width)
           j1b=min(n1b,i1+width)
           j2a=max(n2a,i2-width)
           j2b=min(n2b,i2+width)
!  #If "2" == "3"
           do j2=j2a,j2b
           do j1=j1a,j1b
             mask2(j1,j2,j3)=0
           end do
           end do
!  #If "2" == "3"
         end do
         do i=1,nip
           i1=ip(i,0)
           i2=ip(i,1)
!  #If "2" == "3"
           mask2(i1,i2,i3)=1     ! mark that this point is not a valid neighbour
           j1a=max(n1a,i1-width)
           j1b=min(n1b,i1+width)
           j2a=max(n2a,i2-width)
           j2b=min(n2b,i2+width)
!  #If "2" == "3"
           do j2=j2a,j2b
           do j1=j1a,j1b
             if( mask(j1,j2,j3).gt.0 .and. mask2(j1,j2,j3).eq.0 )then
               nipn=nipn+1
               ! write(*,'('' nipn='',i4)') nipn
               if( nipn.gt.ndipn )then ! ******************* remove this for efficiency ?
                 ierr=1
                 write(*,*) 'getInterpNeighbours:WARNING nipn,ndipn=',
     & nipn,ndipn
                 return
               end if
               ipn(nipn,0)=j1
               ipn(nipn,1)=j2
!  #If "2" == "3"
               mask2(j1,j2,j3)=1 ! mark that this point has already been found
             end if
           end do
           end do
!  #If "2" == "3"
         end do

      else

! interpNeighboursMacro(3)
         i3=0
         j3=0
         do i=1,nip
           i1=ip(i,0)
           i2=ip(i,1)
!  #If "3" == "3"
           i3=ip(i,2)
           j1a=max(n1a,i1-width)
           j1b=min(n1b,i1+width)
           j2a=max(n2a,i2-width)
           j2b=min(n2b,i2+width)
!  #If "3" == "3"
           j3a=max(n3a,i3-width)
           j3b=min(n3b,i3+width)
           do j3=j3a,j3b
           do j2=j2a,j2b
           do j1=j1a,j1b
             mask2(j1,j2,j3)=0
           end do
           end do
!  #If "3" == "3"
           end do
         end do
         do i=1,nip
           i1=ip(i,0)
           i2=ip(i,1)
!  #If "3" == "3"
           i3=ip(i,2)
           mask2(i1,i2,i3)=1     ! mark that this point is not a valid neighbour
           j1a=max(n1a,i1-width)
           j1b=min(n1b,i1+width)
           j2a=max(n2a,i2-width)
           j2b=min(n2b,i2+width)
!  #If "3" == "3"
           j3a=max(n3a,i3-width)
           j3b=min(n3b,i3+width)
           do j3=j3a,j3b
           do j2=j2a,j2b
           do j1=j1a,j1b
             if( mask(j1,j2,j3).gt.0 .and. mask2(j1,j2,j3).eq.0 )then
               nipn=nipn+1
               ! write(*,'('' nipn='',i4)') nipn
               if( nipn.gt.ndipn )then ! ******************* remove this for efficiency ?
                 ierr=1
                 write(*,*) 'getInterpNeighbours:WARNING nipn,ndipn=',
     & nipn,ndipn
                 return
               end if
               ipn(nipn,0)=j1
               ipn(nipn,1)=j2
!  #If "3" == "3"
               ipn(nipn,2)=j3
               mask2(j1,j2,j3)=1 ! mark that this point has already been found
             end if
           end do
           end do
!  #If "3" == "3"
           end do
         end do

      end if


      return
      end


! ===========================================================================================
! Macro to make a list of interp neighbours using the mask array (parallel version)
! 
! DIM is 2 or 3
! ===========================================================================================

! ======================================================================================
! Macro to make a list of interp neighbours using the mask array (parallel version)
! 
! ** In this version we make the list of neighbours on a second pass through the
!   mask2 array so that the points are ordered in a different way
!
! DIM is 2 or 3
! ======================================================================================


      subroutine markInterpNeighbours( nd, nd1a,nd1b,nd2a,nd2b,nd3a,
     & nd3b,
     & n1a,n1b,n2a,n2b,n3a,n3b, eir, mask,mask2, nipn,ndipn,ipn, 
     & width, ierr )
! ===================================================================================
! IBS (interpolation boundary smoothing) routine: 
!   Make a list of discretization points that are within "width" points of an interpolation point
!
!   --- This is the new parallel version which only uses the mask ---
!
! Input
!  n1a,n1b, n2a,n2b,n3a,n3b : mark pts lying in these index ranges
!  mask : mask array
!  mask2 : workspace
!  ndipn : dimension of array ipn and max. number of interp. neighbours that can be set,
!          on return ierr=1 if ndipn was too small. 
!  width : make a list disc. pts within "width" pts of any interp. pt. 
! Output 
!  ipn(ndipn,0:nd) : (ipn(i,0:nd-1), i=1,...,nipn) : list of interpolation neighbours
!  ierr : 0=success, 1=ndipn was too small, you need to increase the space allocated for
!          interp neighbours
! ===================================================================================

      implicit none
      integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,width,n1a,n1b,n2a,n2b,
     & n3a,n3b
      integer eir(0:1,0:2)
      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer mask2(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)

      integer ierr
      integer nipn,ndipn,ipn(1:ndipn,0:*)

!.............local
      integer i1,i2,i3,j1,j2,j3,i
      integer j1a,j1b,j2a,j2b,j3a,j3b
      integer m1a,m1b,m2a,m2b,m3a,m3b

      ierr=0
      nipn=0

      m1a=eir(0,0)
      m1b=eir(1,0)
      m2a=eir(0,1)
      m2b=eir(1,1)
      m3a=eir(0,2)
      m3b=eir(1,2)

      ! initialize the work space mask2 to zero.
      ! mask2(i1,i2,i3) will later be set to 1 at interp neighbours
      do i3=n3a,n3b
      do i2=n2a,n2b
      do i1=n1a,n1b
        mask2(i1,i2,i3)=0
      end do
      end do
      end do

      if( nd.eq.2 )then
! markInterpNeighboursMacro(2)
         i3=0
         j3=0
         ! Note: we check for interp points on the larger extended index range [m1a,m1b][m2a,m2b]
         !       so we include interp pts in the ghost pts
         do i3=m3a,m3b
         do i2=m2a,m2b
         do i1=m1a,m1b
          if( mask(i1,i2,i3).lt.0 )then
            ! This is an interpolation point -- mark any discr. pts within "width" pts of (i1,i2,i3)
            j1a=max(n1a,i1-width)
            j1b=min(n1b,i1+width)
            j2a=max(n2a,i2-width)
            j2b=min(n2b,i2+width)
!     #If "2" == "3"
            do j2=j2a,j2b
            do j1=j1a,j1b
              if( mask(j1,j2,j3).gt.0 .and. mask2(j1,j2,j3).eq.0 )then
                mask2(j1,j2,j3)=1  ! mark this interp neighbour as found
              end if
            end do
            end do
!     #If "2" == "3"
          end if
         end do
         end do
         end do
         do i3=n3a,n3b
         do i2=n2a,n2b
         do i1=n1a,n1b
          if( mask2(i1,i2,i3).eq.1 )then
            nipn=nipn+1
            ! write(*,'('' nipn='',i4)') nipn
            if( nipn.gt.ndipn )then ! ******************* remove this for efficiency ?
              ierr=1
              write(*,*) 'markInterpNeighbours:WARNING nipn,ndipn=',
     & nipn,ndipn
              return
            end if
            ipn(nipn,0)=i1
            ipn(nipn,1)=i2
!     #If "2" == "3"
          end if
         end do
         end do
         end do
      else
! markInterpNeighboursMacro(3)
         i3=0
         j3=0
         ! Note: we check for interp points on the larger extended index range [m1a,m1b][m2a,m2b]
         !       so we include interp pts in the ghost pts
         do i3=m3a,m3b
         do i2=m2a,m2b
         do i1=m1a,m1b
          if( mask(i1,i2,i3).lt.0 )then
            ! This is an interpolation point -- mark any discr. pts within "width" pts of (i1,i2,i3)
            j1a=max(n1a,i1-width)
            j1b=min(n1b,i1+width)
            j2a=max(n2a,i2-width)
            j2b=min(n2b,i2+width)
!     #If "3" == "3"
             j3a=max(n3a,i3-width)
             j3b=min(n3b,i3+width)
             do j3=j3a,j3b
            do j2=j2a,j2b
            do j1=j1a,j1b
              if( mask(j1,j2,j3).gt.0 .and. mask2(j1,j2,j3).eq.0 )then
                mask2(j1,j2,j3)=1  ! mark this interp neighbour as found
              end if
            end do
            end do
!     #If "3" == "3"
            end do
          end if
         end do
         end do
         end do
         do i3=n3a,n3b
         do i2=n2a,n2b
         do i1=n1a,n1b
          if( mask2(i1,i2,i3).eq.1 )then
            nipn=nipn+1
            ! write(*,'('' nipn='',i4)') nipn
            if( nipn.gt.ndipn )then ! ******************* remove this for efficiency ?
              ierr=1
              write(*,*) 'markInterpNeighbours:WARNING nipn,ndipn=',
     & nipn,ndipn
              return
            end if
            ipn(nipn,0)=i1
            ipn(nipn,1)=i2
!     #If "3" == "3"
             ipn(nipn,2)=i3
          end if
         end do
         end do
         end do
      end if

      return
      end


