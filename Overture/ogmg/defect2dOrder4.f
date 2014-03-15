! This file automatically generated from defectOpt.bf with bpp.
! DEFECT_SUBROUTINE(defect2dOrder4,2,4)
        subroutine defect2dOrder4( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c, ndc, defect, f, c, u,mask,
     &  cc, s, ipar, rpar )
c ===================================================================================
c  Optimised defect
c
c defectOption: ipar(0)
c               =0 : compute defect only
c               =1 : compute defect and "l2-norm": sum( defect**2 ), returned as rpar(3), and sum( 1 ) returned at rpar(4)
c                    The l2-norm is then sqrt( rpar(3)/rpar(4) ) (appropriated adjusted in parallel)
c               =2 : compute defect and max norm, returned as rpar(5)
c               =3 : compute defect and both l2-norm and max-norm
c
c lineSmoothOption=ipar(1)
c lineSmoothOption: -1 : general
c                    0 : line smooth, direction 1
c                    1 : line smooth, direction 2
c                    2 : line smooth, direction 3
c                    3 : red-points
c                    4 : black-points
c
c Return values:
c  rpar(3) = sum(defect^2) = sum of the squares of the defects at valid points 
c  rpar(4) = sum(1) = count = number of points in the sum
c  rpar(5) = max-norm of the defect 
c
c  cc(m) : constant coefficients
c  sparseStencil : general=0, sparse=1, constantCoefficients=2, sparseConstantCoefficients=3
c ===================================================================================
        implicit none
        integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,n1a,n1b,n1c,n2a,n2b,
     & n2c,n3a,n3b,n3c, ndc,ipar(0:*)
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
        integer i1,i2,i3,n
        real count,defectSquared,defectMax
        integer m11,m12,m13,m14,m15,m21,m22,m23,m24,m25,m31,m32,m33,
     & m34,m35,m41,m42,m43,m44,m45,m51,m52,m53,m54,m55
        integer    m111,m211,m311,m411,m511,m121,m221,m321,m421,m521,
     & m131,m231,m331,m431,m531,m141,m241,m341,m441,m541,m151,m251,
     & m351,m451,m551,m112,m212,m312,m412,m512,m122,m222,m322,m422,
     & m522,m132,m232,m332,m432,m532,m142,m242,m342,m442,m542,m152,
     & m252,m352,m452,m552,m113,m213,m313,m413,m513,m123,m223,m323,
     & m423,m523,m133,m233,m333,m433,m533,m143,m243,m343,m443,m543,
     & m153,m253,m353,m453,m553,m114,m214,m314,m414,m514,m124,m224,
     & m324,m424,m524,m134,m234,m334,m434,m534,m144,m244,m344,m444,
     & m544,m154,m254,m354,m454,m554,m115,m215,m315,m415,m515,m125,
     & m225,m325,m425,m525,m135,m235,m335,m435,m535,m145,m245,m345,
     & m445,m545,m155,m255,m355,m455,m555
        integer    m11n,m21n,m31n,m41n,m51n,m12n,m22n,m32n,m42n,m52n,
     & m13n,m23n,m33n,m43n,m53n,m14n,m24n,m34n,m44n,m54n,m15n,m25n,
     & m35n,m45n,m55n
        real a1m,a1p,a2m,a2p,a3m,a3p,ad
        real dx2i,dy2i,dz2i
        integer general, sparse, constantCoefficients, 
     & sparseConstantCoefficients,variableCoefficients,
     & sparseVariableCoefficients
        parameter( general=0, sparse=1, constantCoefficients=2,
     & sparseConstantCoefficients=3,variableCoefficients=4,
     & sparseVariableCoefficients=5 )
c..........start statement functions
        real op2d,op2dSparse,op2dCC,op2dSparseCC
        real op2dl0,op2dSparsel0,op2dCCl0,op2dSparseCCl0
        real op2dl1,op2dSparsel1,op2dCCl1,op2dSparseCCl1
        real op3d,op3dSparse,op3dCC,op3dSparseCC
        real op3dl0,op3dSparsel0,op3dCCl0,op3dSparseCCl0
        real op3dl1,op3dSparsel1,op3dCCl1,op3dSparseCCl1
        real op3dl2,op3dSparsel2,op3dCCl2,op3dSparseCCl2
        real op2d4,op2dSparse4,op2dCC4,op2dSparseCC4
        real op2d4l0,op2dSparse4l0,op2dCC4l0,op2dSparseCC4l0
        real op2d4l1,op2dSparse4l1,op2dCC4l1,op2dSparseCC4l1
        real op3d4,op3dSparse4,op3dCC4,op3dSparseCC4
        real op3d4l0,op3dSparse4l0,op3dCC4l0,op3dSparseCC4l0
        real op3d4l1,op3dSparse4l1,op3dCC4l1,op3dSparseCC4l1
        real op3d4l2,op3dSparse4l2,op3dCC4l2,op3dSparseCC4l2
        real op3d4a,op3d4al0,op3d4al1,op3d4al2
        real op3dCC4a,op3dCC4al0,op3dCC4al1,op3dCC4al2
        real op2dSparseVC,op2dSparseVCl0,op2dSparseVCl1
        real op3dSparseVC,op3dSparseVCl0,op3dSparseVCl1,op3dSparseVCl2
! #If "2" == "2"
        op2d(i1,i2,i3)= c(m11,i1,i2,i3)*u(i1-1,i2-1,i3)+ c(m21,i1,i2,
     & i3)*u(i1  ,i2-1,i3)+ c(m31,i1,i2,i3)*u(i1+1,i2-1,i3)+ c(m12,i1,
     & i2,i3)*u(i1-1,i2  ,i3)+ c(m22,i1,i2,i3)*u(i1  ,i2  ,i3)+ c(m32,
     & i1,i2,i3)*u(i1+1,i2  ,i3)+ c(m13,i1,i2,i3)*u(i1-1,i2+1,i3)+ c(
     & m23,i1,i2,i3)*u(i1  ,i2+1,i3)+ c(m33,i1,i2,i3)*u(i1+1,i2+1,i3)
       ! line smooth direction 1 
        op2dl0(i1,i2,i3)=   c(m11,i1,i2,i3)*u(i1-1,i2-1,i3)+ c(m21,i1,
     & i2,i3)*u(i1  ,i2-1,i3)+ c(m31,i1,i2,i3)*u(i1+1,i2-1,i3)+ c(m13,
     & i1,i2,i3)*u(i1-1,i2+1,i3)+ c(m23,i1,i2,i3)*u(i1  ,i2+1,i3)+ c(
     & m33,i1,i2,i3)*u(i1+1,i2+1,i3)
        op2dl1(i1,i2,i3)= c(m11,i1,i2,i3)*u(i1-1,i2-1,i3)+ c(m31,i1,i2,
     & i3)*u(i1+1,i2-1,i3)+ c(m12,i1,i2,i3)*u(i1-1,i2  ,i3)+ c(m32,i1,
     & i2,i3)*u(i1+1,i2  ,i3)+ c(m13,i1,i2,i3)*u(i1-1,i2+1,i3)+ c(m33,
     & i1,i2,i3)*u(i1+1,i2+1,i3)
        op2dSparse(i1,i2,i3)= c(m22,i1,i2,i3)*u(i1  ,i2  ,i3)+ c(m32,
     & i1,i2,i3)*u(i1+1,i2  ,i3)+ c(m23,i1,i2,i3)*u(i1  ,i2+1,i3)+ c(
     & m12,i1,i2,i3)*u(i1-1,i2  ,i3)+ c(m21,i1,i2,i3)*u(i1  ,i2-1,i3)
        op2dSparsel0(i1,i2,i3)= c(m23,i1,i2,i3)*u(i1  ,i2+1,i3)+ c(m21,
     & i1,i2,i3)*u(i1  ,i2-1,i3)
        op2dSparsel1(i1,i2,i3)= c(m32,i1,i2,i3)*u(i1+1,i2  ,i3)+ c(m12,
     & i1,i2,i3)*u(i1-1,i2  ,i3)
        op2dCC(i1,i2,i3)= cc(m11)*u(i1-1,i2-1,i3)+ cc(m21)*u(i1  ,i2-1,
     & i3)+ cc(m31)*u(i1+1,i2-1,i3)+ cc(m12)*u(i1-1,i2  ,i3)+ cc(m22)*
     & u(i1  ,i2  ,i3)+ cc(m32)*u(i1+1,i2  ,i3)+ cc(m13)*u(i1-1,i2+1,
     & i3)+ cc(m23)*u(i1  ,i2+1,i3)+ cc(m33)*u(i1+1,i2+1,i3)
        op2dCCl0(i1,i2,i3)= cc(m11)*u(i1-1,i2-1,i3)+ cc(m21)*u(i1  ,i2-
     & 1,i3)+ cc(m31)*u(i1+1,i2-1,i3)+ cc(m13)*u(i1-1,i2+1,i3)+ cc(
     & m23)*u(i1  ,i2+1,i3)+ cc(m33)*u(i1+1,i2+1,i3)
        op2dCCl1(i1,i2,i3)= cc(m11)*u(i1-1,i2-1,i3)+ cc(m31)*u(i1+1,i2-
     & 1,i3)+ cc(m12)*u(i1-1,i2  ,i3)+ cc(m32)*u(i1+1,i2  ,i3)+ cc(
     & m13)*u(i1-1,i2+1,i3)+ cc(m33)*u(i1+1,i2+1,i3)
        op2dSparseCC(i1,i2,i3)= cc(m22)*u(i1  ,i2  ,i3)+ cc(m32)*u(i1+
     & 1,i2  ,i3)+ cc(m23)*u(i1  ,i2+1,i3)+ cc(m12)*u(i1-1,i2  ,i3)+ 
     & cc(m21)*u(i1  ,i2-1,i3)
        op2dSparseCCl0(i1,i2,i3)= cc(m23)*u(i1  ,i2+1,i3)+ cc(m21)*u(
     & i1  ,i2-1,i3)
        op2dSparseCCl1(i1,i2,i3)= cc(m32)*u(i1+1,i2  ,i3)+ cc(m12)*u(
     & i1-1,i2  ,i3)
! #If "2" == "3"
! #If "2" == "2"
        ! ===========  div( s grad ) ===========================
        op2dSparseVC(i1,i2,i3)= a2m*u(i1  ,i2-1,i3)+ a1m*u(i1-1,i2  ,
     & i3)+ ad *u(i1  ,i2  ,i3)+ a1p*u(i1+1,i2  ,i3)+ a2p*u(i1  ,i2+1,
     & i3)
        op2dSparseVCl0(i1,i2,i3)= a2m*u(i1  ,i2-1,i3)+ a2p*u(i1  ,i2+1,
     & i3)
        op2dSparseVCl1(i1,i2,i3)= a1m*u(i1-1,i2  ,i3)+ a1p*u(i1+1,i2  ,
     & i3)
! #If "2" == "3"
! #If "2" == "2"
        ! ==================================================
        ! ===========  4th order ===========================
        ! ==================================================
        op2dSparse4(i1,i2,i3)= c(m31,i1,i2,i3)*u(i1  ,i2-2,i3)+ c(m32,
     & i1,i2,i3)*u(i1  ,i2-1,i3)+ c(m13,i1,i2,i3)*u(i1-2,i2  ,i3)+ c(
     & m23,i1,i2,i3)*u(i1-1,i2  ,i3)+ c(m33,i1,i2,i3)*u(i1  ,i2  ,i3)+
     &  c(m43,i1,i2,i3)*u(i1+1,i2  ,i3)+ c(m53,i1,i2,i3)*u(i1+2,i2  ,
     & i3)+ c(m34,i1,i2,i3)*u(i1  ,i2+1,i3)+ c(m35,i1,i2,i3)*u(i1  ,
     & i2+2,i3)
        op2dSparse4l0(i1,i2,i3)= c(m31,i1,i2,i3)*u(i1  ,i2-2,i3)+ c(
     & m32,i1,i2,i3)*u(i1  ,i2-1,i3)+ c(m34,i1,i2,i3)*u(i1  ,i2+1,i3)+
     &  c(m35,i1,i2,i3)*u(i1  ,i2+2,i3)
        op2dSparse4l1(i1,i2,i3)= c(m13,i1,i2,i3)*u(i1-2,i2  ,i3)+ c(
     & m23,i1,i2,i3)*u(i1-1,i2  ,i3)+ c(m43,i1,i2,i3)*u(i1+1,i2  ,i3)+
     &  c(m53,i1,i2,i3)*u(i1+2,i2  ,i3)
        op2d4(i1,i2,i3)= c(m11,i1,i2,i3)*u(i1-2,i2-2,i3)+ c(m21,i1,i2,
     & i3)*u(i1-1,i2-2,i3)+ c(m31,i1,i2,i3)*u(i1  ,i2-2,i3)+ c(m41,i1,
     & i2,i3)*u(i1+1,i2-2,i3)+ c(m51,i1,i2,i3)*u(i1+2,i2-2,i3)+ c(m12,
     & i1,i2,i3)*u(i1-2,i2-1,i3)+ c(m22,i1,i2,i3)*u(i1-1,i2-1,i3)+ c(
     & m32,i1,i2,i3)*u(i1  ,i2-1,i3)+ c(m42,i1,i2,i3)*u(i1+1,i2-1,i3)+
     &  c(m52,i1,i2,i3)*u(i1+2,i2-1,i3)+ c(m13,i1,i2,i3)*u(i1-2,i2  ,
     & i3)+ c(m23,i1,i2,i3)*u(i1-1,i2  ,i3)+ c(m33,i1,i2,i3)*u(i1  ,
     & i2  ,i3)+ c(m43,i1,i2,i3)*u(i1+1,i2  ,i3)+ c(m53,i1,i2,i3)*u(
     & i1+2,i2  ,i3)+ c(m14,i1,i2,i3)*u(i1-2,i2+1,i3)+ c(m24,i1,i2,i3)
     & *u(i1-1,i2+1,i3)+ c(m34,i1,i2,i3)*u(i1  ,i2+1,i3)+ c(m44,i1,i2,
     & i3)*u(i1+1,i2+1,i3)+ c(m54,i1,i2,i3)*u(i1+2,i2+1,i3)+ c(m15,i1,
     & i2,i3)*u(i1-2,i2+2,i3)+ c(m25,i1,i2,i3)*u(i1-1,i2+2,i3)+ c(m35,
     & i1,i2,i3)*u(i1  ,i2+2,i3)+ c(m45,i1,i2,i3)*u(i1+1,i2+2,i3)+ c(
     & m55,i1,i2,i3)*u(i1+2,i2+2,i3)
        op2d4l0(i1,i2,i3)= c(m11,i1,i2,i3)*u(i1-2,i2-2,i3)+ c(m21,i1,
     & i2,i3)*u(i1-1,i2-2,i3)+ c(m31,i1,i2,i3)*u(i1  ,i2-2,i3)+ c(m41,
     & i1,i2,i3)*u(i1+1,i2-2,i3)+ c(m51,i1,i2,i3)*u(i1+2,i2-2,i3)+ c(
     & m12,i1,i2,i3)*u(i1-2,i2-1,i3)+ c(m22,i1,i2,i3)*u(i1-1,i2-1,i3)+
     &  c(m32,i1,i2,i3)*u(i1  ,i2-1,i3)+ c(m42,i1,i2,i3)*u(i1+1,i2-1,
     & i3)+ c(m52,i1,i2,i3)*u(i1+2,i2-1,i3)+ c(m14,i1,i2,i3)*u(i1-2,
     & i2+1,i3)+ c(m24,i1,i2,i3)*u(i1-1,i2+1,i3)+ c(m34,i1,i2,i3)*u(
     & i1  ,i2+1,i3)+ c(m44,i1,i2,i3)*u(i1+1,i2+1,i3)+ c(m54,i1,i2,i3)
     & *u(i1+2,i2+1,i3)+ c(m15,i1,i2,i3)*u(i1-2,i2+2,i3)+ c(m25,i1,i2,
     & i3)*u(i1-1,i2+2,i3)+ c(m35,i1,i2,i3)*u(i1  ,i2+2,i3)+ c(m45,i1,
     & i2,i3)*u(i1+1,i2+2,i3)+ c(m55,i1,i2,i3)*u(i1+2,i2+2,i3)
        op2d4l1(i1,i2,i3)= c(m11,i1,i2,i3)*u(i1-2,i2-2,i3)+ c(m21,i1,
     & i2,i3)*u(i1-1,i2-2,i3)+ c(m41,i1,i2,i3)*u(i1+1,i2-2,i3)+ c(m51,
     & i1,i2,i3)*u(i1+2,i2-2,i3)+ c(m12,i1,i2,i3)*u(i1-2,i2-1,i3)+ c(
     & m22,i1,i2,i3)*u(i1-1,i2-1,i3)+ c(m42,i1,i2,i3)*u(i1+1,i2-1,i3)+
     &  c(m52,i1,i2,i3)*u(i1+2,i2-1,i3)+ c(m13,i1,i2,i3)*u(i1-2,i2  ,
     & i3)+ c(m23,i1,i2,i3)*u(i1-1,i2  ,i3)+ c(m43,i1,i2,i3)*u(i1+1,
     & i2  ,i3)+ c(m53,i1,i2,i3)*u(i1+2,i2  ,i3)+ c(m14,i1,i2,i3)*u(
     & i1-2,i2+1,i3)+ c(m24,i1,i2,i3)*u(i1-1,i2+1,i3)+ c(m44,i1,i2,i3)
     & *u(i1+1,i2+1,i3)+ c(m54,i1,i2,i3)*u(i1+2,i2+1,i3)+ c(m15,i1,i2,
     & i3)*u(i1-2,i2+2,i3)+ c(m25,i1,i2,i3)*u(i1-1,i2+2,i3)+ c(m45,i1,
     & i2,i3)*u(i1+1,i2+2,i3)+ c(m55,i1,i2,i3)*u(i1+2,i2+2,i3)
! #If "2" == "3"
! #If "2" == "2"
        ! ------------------------------------------------------
        ! --------- const coefficients versions ----------------
        ! ------------------------------------------------------
        op2dSparseCC4(i1,i2,i3)= cc(m31)*u(i1  ,i2-2,i3)+ cc(m32)*u(i1 
     &  ,i2-1,i3)+ cc(m13)*u(i1-2,i2  ,i3)+ cc(m23)*u(i1-1,i2  ,i3)+ 
     & cc(m33)*u(i1  ,i2  ,i3)+ cc(m43)*u(i1+1,i2  ,i3)+ cc(m53)*u(i1+
     & 2,i2  ,i3)+ cc(m34)*u(i1  ,i2+1,i3)+ cc(m35)*u(i1  ,i2+2,i3)
        op2dSparseCC4l0(i1,i2,i3)= cc(m31)*u(i1  ,i2-2,i3)+ cc(m32)*u(
     & i1  ,i2-1,i3)+ cc(m34)*u(i1  ,i2+1,i3)+ cc(m35)*u(i1  ,i2+2,i3)
        op2dSparseCC4l1(i1,i2,i3)= cc(m13)*u(i1-2,i2  ,i3)+ cc(m23)*u(
     & i1-1,i2  ,i3)+ cc(m43)*u(i1+1,i2  ,i3)+ cc(m53)*u(i1+2,i2  ,i3)
        op2dCC4(i1,i2,i3)= cc(m11)*u(i1-2,i2-2,i3)+ cc(m21)*u(i1-1,i2-
     & 2,i3)+ cc(m31)*u(i1  ,i2-2,i3)+ cc(m41)*u(i1+1,i2-2,i3)+ cc(
     & m51)*u(i1+2,i2-2,i3)+ cc(m12)*u(i1-2,i2-1,i3)+ cc(m22)*u(i1-1,
     & i2-1,i3)+ cc(m32)*u(i1  ,i2-1,i3)+ cc(m42)*u(i1+1,i2-1,i3)+ cc(
     & m52)*u(i1+2,i2-1,i3)+ cc(m13)*u(i1-2,i2  ,i3)+ cc(m23)*u(i1-1,
     & i2  ,i3)+ cc(m33)*u(i1  ,i2  ,i3)+ cc(m43)*u(i1+1,i2  ,i3)+ cc(
     & m53)*u(i1+2,i2  ,i3)+ cc(m14)*u(i1-2,i2+1,i3)+ cc(m24)*u(i1-1,
     & i2+1,i3)+ cc(m34)*u(i1  ,i2+1,i3)+ cc(m44)*u(i1+1,i2+1,i3)+ cc(
     & m54)*u(i1+2,i2+1,i3)+ cc(m15)*u(i1-2,i2+2,i3)+ cc(m25)*u(i1-1,
     & i2+2,i3)+ cc(m35)*u(i1  ,i2+2,i3)+ cc(m45)*u(i1+1,i2+2,i3)+ cc(
     & m55)*u(i1+2,i2+2,i3)
        op2dCC4l0(i1,i2,i3)= cc(m11)*u(i1-2,i2-2,i3)+ cc(m21)*u(i1-1,
     & i2-2,i3)+ cc(m31)*u(i1  ,i2-2,i3)+ cc(m41)*u(i1+1,i2-2,i3)+ cc(
     & m51)*u(i1+2,i2-2,i3)+ cc(m12)*u(i1-2,i2-1,i3)+ cc(m22)*u(i1-1,
     & i2-1,i3)+ cc(m32)*u(i1  ,i2-1,i3)+ cc(m42)*u(i1+1,i2-1,i3)+ cc(
     & m52)*u(i1+2,i2-1,i3)+ cc(m14)*u(i1-2,i2+1,i3)+ cc(m24)*u(i1-1,
     & i2+1,i3)+ cc(m34)*u(i1  ,i2+1,i3)+ cc(m44)*u(i1+1,i2+1,i3)+ cc(
     & m54)*u(i1+2,i2+1,i3)+ cc(m15)*u(i1-2,i2+2,i3)+ cc(m25)*u(i1-1,
     & i2+2,i3)+ cc(m35)*u(i1  ,i2+2,i3)+ cc(m45)*u(i1+1,i2+2,i3)+ cc(
     & m55)*u(i1+2,i2+2,i3)
        op2dCC4l1(i1,i2,i3)= cc(m11)*u(i1-2,i2-2,i3)+ cc(m21)*u(i1-1,
     & i2-2,i3)+ cc(m41)*u(i1+1,i2-2,i3)+ cc(m51)*u(i1+2,i2-2,i3)+ cc(
     & m12)*u(i1-2,i2-1,i3)+ cc(m22)*u(i1-1,i2-1,i3)+ cc(m42)*u(i1+1,
     & i2-1,i3)+ cc(m52)*u(i1+2,i2-1,i3)+ cc(m13)*u(i1-2,i2  ,i3)+ cc(
     & m23)*u(i1-1,i2  ,i3)+ cc(m43)*u(i1+1,i2  ,i3)+ cc(m53)*u(i1+2,
     & i2  ,i3)+ cc(m14)*u(i1-2,i2+1,i3)+ cc(m24)*u(i1-1,i2+1,i3)+ cc(
     & m44)*u(i1+1,i2+1,i3)+ cc(m54)*u(i1+2,i2+1,i3)+ cc(m15)*u(i1-2,
     & i2+2,i3)+ cc(m25)*u(i1-1,i2+2,i3)+ cc(m45)*u(i1+1,i2+2,i3)+ cc(
     & m55)*u(i1+2,i2+2,i3)
! #If "2" == "3"
c............end statement functions
        defectOption    =ipar(0)
        lineSmoothOption=ipar(1)
        order           =ipar(2)
        sparseStencil   =ipar(3)
        if( order.ne.2 .and. order.ne.4 )then
          write(*,*) 'defectOpt:ERROR: invalid order=',order
          stop 1
        end if
        dx(1)=rpar(0)
        dx(2)=rpar(1)
        dx(3)=rpar(2)
        dx2i=.5/dx(1)**2
        dy2i=.5/dx(2)**2
        dz2i=.5/dx(3)**2
        defectSquared=0.
        defectMax=0.
!  #If "2" == "2"
          ! **************************
          ! ***** Two Dimensions *****
          ! **************************
!    #If "4" == "2"
!    #Elif "4" == "4"
            ! ****** 4th order accurate ****
            m11=1
            m21=2
            m31=3
            m41=4
            m51=5
            m12=6
            m22=7
            m32=8
            m42=9
            m52=10
            m13=11
            m23=12
            m33=13
            m43=14
            m53=15
            m14=16
            m24=17
            m34=18
            m44=19
            m54=20
            m15=21
            m25=22
            m35=23
            m45=24
            m55=25
            ! write(*,*) 'defect; cc=',(cc(i1),i1=1,25)
            if( sparseStencil.eq.sparse )then
! DEFECT(op2dSparse4,2)
              if( lineSmoothOption .eq. -1 )then
c  general defect
! loops(op2dSparse4)
                if( defectOption.eq.0 )then
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2dSparse4(i1,i2,
     & i3)
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else if( defectOption.eq.1 )then
                  ! In this case compute the l2-norm of the defect too
                  count=0
                  defectSquared=0.
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2dSparse4(i1,i2,
     & i3)
                      ! write(*,'(" i1,i2,i3=",3i3," defect=",e10.2," f=",e10.2)') i1,i2,i3,defect(i1,i2,i3),f(i1,i2,i3)
                      defectSquared=defectSquared+defect(i1,i2,i3)**2
                      count=count+1
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else if( defectOption.eq.2 )then
                  ! In this case compute the max-norm of the defect too
                  defectMax=0.
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2dSparse4(i1,i2,
     & i3)
                      defectMax=max(defectMax,abs(defect(i1,i2,i3)))
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else if( defectOption.eq.3 )then
                  ! In this case compute both the l2-norm and max-norm of the defect too
                  count=0
                  defectSquared=0.
                  defectMax=0.
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2dSparse4(i1,i2,
     & i3)
                      defectMax=max(defectMax,abs(defect(i1,i2,i3)))
                      defectSquared=defectSquared+defect(i1,i2,i3)**2
                      count=count+1
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else
                  write(*,'("ERROR:defectOpt: unknown defectOption")')
                  stop 89
                end if
              else if( lineSmoothOption .eq. 0 )then
c  defect for line smooth in direction 0
! loops(op2dSparse4l0)
                if( defectOption.eq.0 )then
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2dSparse4l0(i1,i2,
     & i3)
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else if( defectOption.eq.1 )then
                  ! In this case compute the l2-norm of the defect too
                  count=0
                  defectSquared=0.
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2dSparse4l0(i1,i2,
     & i3)
                      ! write(*,'(" i1,i2,i3=",3i3," defect=",e10.2," f=",e10.2)') i1,i2,i3,defect(i1,i2,i3),f(i1,i2,i3)
                      defectSquared=defectSquared+defect(i1,i2,i3)**2
                      count=count+1
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else if( defectOption.eq.2 )then
                  ! In this case compute the max-norm of the defect too
                  defectMax=0.
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2dSparse4l0(i1,i2,
     & i3)
                      defectMax=max(defectMax,abs(defect(i1,i2,i3)))
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else if( defectOption.eq.3 )then
                  ! In this case compute both the l2-norm and max-norm of the defect too
                  count=0
                  defectSquared=0.
                  defectMax=0.
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2dSparse4l0(i1,i2,
     & i3)
                      defectMax=max(defectMax,abs(defect(i1,i2,i3)))
                      defectSquared=defectSquared+defect(i1,i2,i3)**2
                      count=count+1
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else
                  write(*,'("ERROR:defectOpt: unknown defectOption")')
                  stop 89
                end if
! #If "2" ne "1"
              else if( lineSmoothOption .eq. 1 )then
c  defect for line smooth in direction 1
! loops(op2dSparse4l1)
                if( defectOption.eq.0 )then
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2dSparse4l1(i1,i2,
     & i3)
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else if( defectOption.eq.1 )then
                  ! In this case compute the l2-norm of the defect too
                  count=0
                  defectSquared=0.
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2dSparse4l1(i1,i2,
     & i3)
                      ! write(*,'(" i1,i2,i3=",3i3," defect=",e10.2," f=",e10.2)') i1,i2,i3,defect(i1,i2,i3),f(i1,i2,i3)
                      defectSquared=defectSquared+defect(i1,i2,i3)**2
                      count=count+1
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else if( defectOption.eq.2 )then
                  ! In this case compute the max-norm of the defect too
                  defectMax=0.
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2dSparse4l1(i1,i2,
     & i3)
                      defectMax=max(defectMax,abs(defect(i1,i2,i3)))
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else if( defectOption.eq.3 )then
                  ! In this case compute both the l2-norm and max-norm of the defect too
                  count=0
                  defectSquared=0.
                  defectMax=0.
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2dSparse4l1(i1,i2,
     & i3)
                      defectMax=max(defectMax,abs(defect(i1,i2,i3)))
                      defectSquared=defectSquared+defect(i1,i2,i3)**2
                      count=count+1
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else
                  write(*,'("ERROR:defectOpt: unknown defectOption")')
                  stop 89
                end if
! #If "2" eq "3"
              else
                write(*,*) 'defectOpt::ERROR invalid lineSmoothOption'
                write(*,*) 'defectOpt::nd="2"'
                write(*,*) 'defectOpt::lineSmoothOption=',
     & lineSmoothOption
                stop
              end if
            else if( sparseStencil.eq.general )then
              !    **** full stencil *****
! DEFECT(op2d4,2)
              if( lineSmoothOption .eq. -1 )then
c  general defect
! loops(op2d4)
                if( defectOption.eq.0 )then
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2d4(i1,i2,i3)
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else if( defectOption.eq.1 )then
                  ! In this case compute the l2-norm of the defect too
                  count=0
                  defectSquared=0.
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2d4(i1,i2,i3)
                      ! write(*,'(" i1,i2,i3=",3i3," defect=",e10.2," f=",e10.2)') i1,i2,i3,defect(i1,i2,i3),f(i1,i2,i3)
                      defectSquared=defectSquared+defect(i1,i2,i3)**2
                      count=count+1
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else if( defectOption.eq.2 )then
                  ! In this case compute the max-norm of the defect too
                  defectMax=0.
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2d4(i1,i2,i3)
                      defectMax=max(defectMax,abs(defect(i1,i2,i3)))
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else if( defectOption.eq.3 )then
                  ! In this case compute both the l2-norm and max-norm of the defect too
                  count=0
                  defectSquared=0.
                  defectMax=0.
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2d4(i1,i2,i3)
                      defectMax=max(defectMax,abs(defect(i1,i2,i3)))
                      defectSquared=defectSquared+defect(i1,i2,i3)**2
                      count=count+1
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else
                  write(*,'("ERROR:defectOpt: unknown defectOption")')
                  stop 89
                end if
              else if( lineSmoothOption .eq. 0 )then
c  defect for line smooth in direction 0
! loops(op2d4l0)
                if( defectOption.eq.0 )then
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2d4l0(i1,i2,i3)
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else if( defectOption.eq.1 )then
                  ! In this case compute the l2-norm of the defect too
                  count=0
                  defectSquared=0.
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2d4l0(i1,i2,i3)
                      ! write(*,'(" i1,i2,i3=",3i3," defect=",e10.2," f=",e10.2)') i1,i2,i3,defect(i1,i2,i3),f(i1,i2,i3)
                      defectSquared=defectSquared+defect(i1,i2,i3)**2
                      count=count+1
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else if( defectOption.eq.2 )then
                  ! In this case compute the max-norm of the defect too
                  defectMax=0.
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2d4l0(i1,i2,i3)
                      defectMax=max(defectMax,abs(defect(i1,i2,i3)))
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else if( defectOption.eq.3 )then
                  ! In this case compute both the l2-norm and max-norm of the defect too
                  count=0
                  defectSquared=0.
                  defectMax=0.
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2d4l0(i1,i2,i3)
                      defectMax=max(defectMax,abs(defect(i1,i2,i3)))
                      defectSquared=defectSquared+defect(i1,i2,i3)**2
                      count=count+1
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else
                  write(*,'("ERROR:defectOpt: unknown defectOption")')
                  stop 89
                end if
! #If "2" ne "1"
              else if( lineSmoothOption .eq. 1 )then
c  defect for line smooth in direction 1
! loops(op2d4l1)
                if( defectOption.eq.0 )then
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2d4l1(i1,i2,i3)
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else if( defectOption.eq.1 )then
                  ! In this case compute the l2-norm of the defect too
                  count=0
                  defectSquared=0.
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2d4l1(i1,i2,i3)
                      ! write(*,'(" i1,i2,i3=",3i3," defect=",e10.2," f=",e10.2)') i1,i2,i3,defect(i1,i2,i3),f(i1,i2,i3)
                      defectSquared=defectSquared+defect(i1,i2,i3)**2
                      count=count+1
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else if( defectOption.eq.2 )then
                  ! In this case compute the max-norm of the defect too
                  defectMax=0.
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2d4l1(i1,i2,i3)
                      defectMax=max(defectMax,abs(defect(i1,i2,i3)))
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else if( defectOption.eq.3 )then
                  ! In this case compute both the l2-norm and max-norm of the defect too
                  count=0
                  defectSquared=0.
                  defectMax=0.
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2d4l1(i1,i2,i3)
                      defectMax=max(defectMax,abs(defect(i1,i2,i3)))
                      defectSquared=defectSquared+defect(i1,i2,i3)**2
                      count=count+1
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else
                  write(*,'("ERROR:defectOpt: unknown defectOption")')
                  stop 89
                end if
! #If "2" eq "3"
              else
                write(*,*) 'defectOpt::ERROR invalid lineSmoothOption'
                write(*,*) 'defectOpt::nd="2"'
                write(*,*) 'defectOpt::lineSmoothOption=',
     & lineSmoothOption
                stop
              end if
            else if( sparseStencil.eq.constantCoefficients )then
! DEFECT(op2dCC4,2)
              if( lineSmoothOption .eq. -1 )then
c  general defect
! loops(op2dCC4)
                if( defectOption.eq.0 )then
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2dCC4(i1,i2,i3)
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else if( defectOption.eq.1 )then
                  ! In this case compute the l2-norm of the defect too
                  count=0
                  defectSquared=0.
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2dCC4(i1,i2,i3)
                      ! write(*,'(" i1,i2,i3=",3i3," defect=",e10.2," f=",e10.2)') i1,i2,i3,defect(i1,i2,i3),f(i1,i2,i3)
                      defectSquared=defectSquared+defect(i1,i2,i3)**2
                      count=count+1
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else if( defectOption.eq.2 )then
                  ! In this case compute the max-norm of the defect too
                  defectMax=0.
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2dCC4(i1,i2,i3)
                      defectMax=max(defectMax,abs(defect(i1,i2,i3)))
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else if( defectOption.eq.3 )then
                  ! In this case compute both the l2-norm and max-norm of the defect too
                  count=0
                  defectSquared=0.
                  defectMax=0.
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2dCC4(i1,i2,i3)
                      defectMax=max(defectMax,abs(defect(i1,i2,i3)))
                      defectSquared=defectSquared+defect(i1,i2,i3)**2
                      count=count+1
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else
                  write(*,'("ERROR:defectOpt: unknown defectOption")')
                  stop 89
                end if
              else if( lineSmoothOption .eq. 0 )then
c  defect for line smooth in direction 0
! loops(op2dCC4l0)
                if( defectOption.eq.0 )then
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2dCC4l0(i1,i2,i3)
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else if( defectOption.eq.1 )then
                  ! In this case compute the l2-norm of the defect too
                  count=0
                  defectSquared=0.
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2dCC4l0(i1,i2,i3)
                      ! write(*,'(" i1,i2,i3=",3i3," defect=",e10.2," f=",e10.2)') i1,i2,i3,defect(i1,i2,i3),f(i1,i2,i3)
                      defectSquared=defectSquared+defect(i1,i2,i3)**2
                      count=count+1
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else if( defectOption.eq.2 )then
                  ! In this case compute the max-norm of the defect too
                  defectMax=0.
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2dCC4l0(i1,i2,i3)
                      defectMax=max(defectMax,abs(defect(i1,i2,i3)))
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else if( defectOption.eq.3 )then
                  ! In this case compute both the l2-norm and max-norm of the defect too
                  count=0
                  defectSquared=0.
                  defectMax=0.
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2dCC4l0(i1,i2,i3)
                      defectMax=max(defectMax,abs(defect(i1,i2,i3)))
                      defectSquared=defectSquared+defect(i1,i2,i3)**2
                      count=count+1
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else
                  write(*,'("ERROR:defectOpt: unknown defectOption")')
                  stop 89
                end if
! #If "2" ne "1"
              else if( lineSmoothOption .eq. 1 )then
c  defect for line smooth in direction 1
! loops(op2dCC4l1)
                if( defectOption.eq.0 )then
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2dCC4l1(i1,i2,i3)
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else if( defectOption.eq.1 )then
                  ! In this case compute the l2-norm of the defect too
                  count=0
                  defectSquared=0.
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2dCC4l1(i1,i2,i3)
                      ! write(*,'(" i1,i2,i3=",3i3," defect=",e10.2," f=",e10.2)') i1,i2,i3,defect(i1,i2,i3),f(i1,i2,i3)
                      defectSquared=defectSquared+defect(i1,i2,i3)**2
                      count=count+1
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else if( defectOption.eq.2 )then
                  ! In this case compute the max-norm of the defect too
                  defectMax=0.
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2dCC4l1(i1,i2,i3)
                      defectMax=max(defectMax,abs(defect(i1,i2,i3)))
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else if( defectOption.eq.3 )then
                  ! In this case compute both the l2-norm and max-norm of the defect too
                  count=0
                  defectSquared=0.
                  defectMax=0.
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2dCC4l1(i1,i2,i3)
                      defectMax=max(defectMax,abs(defect(i1,i2,i3)))
                      defectSquared=defectSquared+defect(i1,i2,i3)**2
                      count=count+1
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else
                  write(*,'("ERROR:defectOpt: unknown defectOption")')
                  stop 89
                end if
! #If "2" eq "3"
              else
                write(*,*) 'defectOpt::ERROR invalid lineSmoothOption'
                write(*,*) 'defectOpt::nd="2"'
                write(*,*) 'defectOpt::lineSmoothOption=',
     & lineSmoothOption
                stop
              end if
            else if( sparseStencil.eq.sparseConstantCoefficients )then
! DEFECT(op2dSparseCC4,2)
              if( lineSmoothOption .eq. -1 )then
c  general defect
! loops(op2dSparseCC4)
                if( defectOption.eq.0 )then
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2dSparseCC4(i1,i2,
     & i3)
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else if( defectOption.eq.1 )then
                  ! In this case compute the l2-norm of the defect too
                  count=0
                  defectSquared=0.
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2dSparseCC4(i1,i2,
     & i3)
                      ! write(*,'(" i1,i2,i3=",3i3," defect=",e10.2," f=",e10.2)') i1,i2,i3,defect(i1,i2,i3),f(i1,i2,i3)
                      defectSquared=defectSquared+defect(i1,i2,i3)**2
                      count=count+1
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else if( defectOption.eq.2 )then
                  ! In this case compute the max-norm of the defect too
                  defectMax=0.
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2dSparseCC4(i1,i2,
     & i3)
                      defectMax=max(defectMax,abs(defect(i1,i2,i3)))
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else if( defectOption.eq.3 )then
                  ! In this case compute both the l2-norm and max-norm of the defect too
                  count=0
                  defectSquared=0.
                  defectMax=0.
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2dSparseCC4(i1,i2,
     & i3)
                      defectMax=max(defectMax,abs(defect(i1,i2,i3)))
                      defectSquared=defectSquared+defect(i1,i2,i3)**2
                      count=count+1
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else
                  write(*,'("ERROR:defectOpt: unknown defectOption")')
                  stop 89
                end if
              else if( lineSmoothOption .eq. 0 )then
c  defect for line smooth in direction 0
! loops(op2dSparseCC4l0)
                if( defectOption.eq.0 )then
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2dSparseCC4l0(i1,
     & i2,i3)
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else if( defectOption.eq.1 )then
                  ! In this case compute the l2-norm of the defect too
                  count=0
                  defectSquared=0.
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2dSparseCC4l0(i1,
     & i2,i3)
                      ! write(*,'(" i1,i2,i3=",3i3," defect=",e10.2," f=",e10.2)') i1,i2,i3,defect(i1,i2,i3),f(i1,i2,i3)
                      defectSquared=defectSquared+defect(i1,i2,i3)**2
                      count=count+1
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else if( defectOption.eq.2 )then
                  ! In this case compute the max-norm of the defect too
                  defectMax=0.
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2dSparseCC4l0(i1,
     & i2,i3)
                      defectMax=max(defectMax,abs(defect(i1,i2,i3)))
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else if( defectOption.eq.3 )then
                  ! In this case compute both the l2-norm and max-norm of the defect too
                  count=0
                  defectSquared=0.
                  defectMax=0.
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2dSparseCC4l0(i1,
     & i2,i3)
                      defectMax=max(defectMax,abs(defect(i1,i2,i3)))
                      defectSquared=defectSquared+defect(i1,i2,i3)**2
                      count=count+1
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else
                  write(*,'("ERROR:defectOpt: unknown defectOption")')
                  stop 89
                end if
! #If "2" ne "1"
              else if( lineSmoothOption .eq. 1 )then
c  defect for line smooth in direction 1
! loops(op2dSparseCC4l1)
                if( defectOption.eq.0 )then
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2dSparseCC4l1(i1,
     & i2,i3)
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else if( defectOption.eq.1 )then
                  ! In this case compute the l2-norm of the defect too
                  count=0
                  defectSquared=0.
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2dSparseCC4l1(i1,
     & i2,i3)
                      ! write(*,'(" i1,i2,i3=",3i3," defect=",e10.2," f=",e10.2)') i1,i2,i3,defect(i1,i2,i3),f(i1,i2,i3)
                      defectSquared=defectSquared+defect(i1,i2,i3)**2
                      count=count+1
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else if( defectOption.eq.2 )then
                  ! In this case compute the max-norm of the defect too
                  defectMax=0.
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2dSparseCC4l1(i1,
     & i2,i3)
                      defectMax=max(defectMax,abs(defect(i1,i2,i3)))
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else if( defectOption.eq.3 )then
                  ! In this case compute both the l2-norm and max-norm of the defect too
                  count=0
                  defectSquared=0.
                  defectMax=0.
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op2dSparseCC4l1(i1,
     & i2,i3)
                      defectMax=max(defectMax,abs(defect(i1,i2,i3)))
                      defectSquared=defectSquared+defect(i1,i2,i3)**2
                      count=count+1
                    else
                      defect(i1,i2,i3)=0.
                    end if
                  end do
                  end do
                  end do
                else
                  write(*,'("ERROR:defectOpt: unknown defectOption")')
                  stop 89
                end if
! #If "2" eq "3"
              else
                write(*,*) 'defectOpt::ERROR invalid lineSmoothOption'
                write(*,*) 'defectOpt::nd="2"'
                write(*,*) 'defectOpt::lineSmoothOption=',
     & lineSmoothOption
                stop
              end if
            else
              write(*,*) 'defectOpt:ERROR: invalid sparseStencil'
              stop 1
            end if
        rpar(3)=0.
        rpar(4)=0.
        rpar(5)=0.
        if( defectOption.eq.1 )then
          ! *wdh* 100928 rpar(3)=sqrt( defectSquared/max(count,1))
          rpar(3)=defectSquared
          rpar(4)=count
          ! write(*,'("defectOpt: count=",i6," l2-norm=",e13.3)') count,rpar(3)
        else if( defectOption.eq.2 )then
          ! *wdh* 100928 rpar(4)=defectMax
          rpar(5)=defectMax
          ! write(*,'("defectOpt: max-norm=",e13.3)') rpar(4)
        else if( defectOption.eq.3 )then
          ! *wdh* 100928 rpar(3)=sqrt( defectSquared/max(count,1))
          ! *wdh* 100928 rpar(4)=defectMax
          rpar(3)=defectSquared
          rpar(4)=count
          rpar(5)=defectMax
          ! write(*,'("defectOpt: max-norm=",e13.3," count=",i6," l2-norm=",e13.3)') rpar(4),count,rpar(3)
        else
          ! write(*,'("defectOpt: defectOption=",3i)') defectOption
        end if
       !  if( defectOption.ne.0 )then
       !  write(*,'("defectOpt: defectOption=",i2," lineSmoothOption=",i2," order=",i2," sparseStencil=",i2," max-norm=",e13.3," count=",i6," l2-norm=",e13.3)') defectOption,lineSmoothOption,order,sparseStencil,rpar(4),count,rpar(3)
       !  write(*,'("  n1a,n1b,n1c=",3i3," n2a,n2b,n2c=",3i3," n3a,n3b,n3c=",3i3)') n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c
       !  end if
        return
        end
