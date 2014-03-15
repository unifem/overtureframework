! This file automatically generated from smOpt.bf with bpp.
c *******************************************
c   New optimized version 
c *******************************************






c update variable coefficient case













! buildFile(smRB2dOrder2,2,2)
! buildFile(smRB2dOrder4,2,4)
! buildFile(smRB3dOrder2,3,2)
! buildFile(smRB3dOrder4,3,4)


      subroutine smRedBlack( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
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
            omega=1.15   ! NOTE: change value above too
          end if
        end if
      end if

      ! write(*,*) 'smoothRB: omega=',omega

      if( nd.eq.2 )then

        if( order.eq.2 )then

          call smRB2dOrder2( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &    n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c, ndc, f, c, u, v,
     & mask, option, order, sparseStencil, cc, s, dx, omega, 
     & useLocallyOptimalOmega,
     &    variableOmegaScaleFactor, ipar, rpar )


        else
         ! 4th order

          call smRB2dOrder4( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
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

          call smRB3dOrder2( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &    n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c, ndc, f, c, u, v,
     & mask, option, order, sparseStencil, cc, s, dx, omega, 
     & useLocallyOptimalOmega,
     &    variableOmegaScaleFactor, ipar, rpar )


        else
         ! 4th order

          call smRB3dOrder4( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &    n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c, ndc, f, c, u, v,
     & mask, option, order, sparseStencil, cc, s, dx, omega, 
     & useLocallyOptimalOmega,
     &    variableOmegaScaleFactor, ipar, rpar )


        end if

      end if

      return
      end
