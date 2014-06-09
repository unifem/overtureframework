! This file automatically generated from defectOpt.bf with bpp.



c =============================================================


c =============================================================









! buildFile(defect2dOrder2,2,2)
! buildFile(defect2dOrder4,2,4)
! buildFile(defect3dOrder2,3,2)
! buildFile(defect3dOrder4,3,4)



      subroutine defectOpt( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &    n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c, ndc, defect, f, c, u,
     &    mask, cc, s, ipar, rpar )
c ===================================================================================
c  Optimised defect
c
c defectOption: ipar(0)
c               =0 : compute defect only
c               =1 : compute defect and l2-norm: sqrt{ sum( defect**2 )/sum( 1 ) }, returned as rpar(3)
c               =2 : compute defect and max norm, returned as rpar(4)
c               =3 : compute defect and both l2-norm and max-norm, l2-norm=rpar(3), max-norm=rpar(4)  
c
c lineSmoothOption=ipar(1)
c lineSmoothOption: -1 : general
c                    0 : line smooth, direction 1
c                    1 : line smooth, direction 2
c                    2 : line smooth, direction 3
c                    3 : red-points
c                    4 : black-points
c
c
c  cc(m) : constant coefficients
c  sparseStencil : general=0, sparse=1, constantCoefficients=2, sparseConstantCoefficients=3
c ===================================================================================

      implicit none
      integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &        n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c, ndc,
     &        ipar(0:*)
      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)

      real defect(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real f(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real c(1:ndc,nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real cc(1:*),rpar(0:*)
      real s(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)

c......local variables
      integer lineSmoothOption,order,sparseStencil,defectOption
      real dx(3)
      integer i1,i2,i3,n,count
      real defectSquared,defectMax

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

      real a1m,a1p,a2m,a2p,a3m,a3p,ad
      real dx2i,dy2i,dz2i

      integer general, sparse, constantCoefficients,
     &   sparseConstantCoefficients,variableCoefficients,
     &   sparseVariableCoefficients
      parameter( general=0, sparse=1, constantCoefficients=2,
     &           sparseConstantCoefficients=3,
     &           variableCoefficients=4,
     &           sparseVariableCoefficients=5 )



      defectOption    =ipar(0)
      lineSmoothOption=ipar(1)
      order           =ipar(2)
      sparseStencil   =ipar(3)

      if( order.ne.2 .and. order.ne.4 )then
        write(*,*) 'defectOpt:ERROR: invalid order=',order
        stop 1
      end if

      if( nd.eq.2 )then
        ! **************************
        ! ***** Two Dimensions *****
        ! **************************

        if( order.eq.2 )then
          ! ****** 2nd order accurate ****

         call defect2dOrder2( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &    n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c, ndc, defect, f, c, u,
     &    mask, cc, s, ipar, rpar )

        else if( order.eq.4 ) then
          ! ****** 4th order accurate ****

         call defect2dOrder4( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &    n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c, ndc, defect, f, c, u,
     &    mask, cc, s, ipar, rpar )

        else
          write(*,*) 'defectOpt:ERROR unknown order=',order
          stop 1
        end if

      else
c       ****************       
c       ***** 3D *******       
c       ****************       
        if( order.eq.2 )then
          ! ****** 2nd order accurate ****

         call defect3dOrder2( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &    n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c, ndc, defect, f, c, u,
     &    mask, cc, s, ipar, rpar )

        else if( order.eq.4 ) then
          ! ****** 4th order accurate ****

         call defect3dOrder4( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &    n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c, ndc, defect, f, c, u,
     &    mask, cc, s, ipar, rpar )

        else
          write(*,*) 'defectOpt:ERROR unknown order=',order
          stop 1
        end if

      end if
      return
      end

