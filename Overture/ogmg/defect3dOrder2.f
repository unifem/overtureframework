! This file automatically generated from defectOpt.bf with bpp.
! DEFECT_SUBROUTINE(defect3dOrder2,3,2)
        subroutine defect3dOrder2( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
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
! #If "3" == "2"
! #If "3" == "3"
        op3d(i1,i2,i3)= c(m111,i1,i2,i3)*u(i1-1,i2-1,i3-1)+ c(m211,i1,
     & i2,i3)*u(i1  ,i2-1,i3-1)+ c(m311,i1,i2,i3)*u(i1+1,i2-1,i3-1)+ 
     & c(m121,i1,i2,i3)*u(i1-1,i2  ,i3-1)+ c(m221,i1,i2,i3)*u(i1  ,i2 
     &  ,i3-1)+ c(m321,i1,i2,i3)*u(i1+1,i2  ,i3-1)+ c(m131,i1,i2,i3)*
     & u(i1-1,i2+1,i3-1)+ c(m231,i1,i2,i3)*u(i1  ,i2+1,i3-1)+ c(m331,
     & i1,i2,i3)*u(i1+1,i2+1,i3-1)+ c(m112,i1,i2,i3)*u(i1-1,i2-1,i3  )
     & + c(m212,i1,i2,i3)*u(i1  ,i2-1,i3  )+ c(m312,i1,i2,i3)*u(i1+1,
     & i2-1,i3  )+ c(m122,i1,i2,i3)*u(i1-1,i2  ,i3  )+ c(m222,i1,i2,
     & i3)*u(i1  ,i2  ,i3  )+ c(m322,i1,i2,i3)*u(i1+1,i2  ,i3  )+ c(
     & m132,i1,i2,i3)*u(i1-1,i2+1,i3  )+ c(m232,i1,i2,i3)*u(i1  ,i2+1,
     & i3  )+ c(m332,i1,i2,i3)*u(i1+1,i2+1,i3  )+ c(m113,i1,i2,i3)*u(
     & i1-1,i2-1,i3+1)+ c(m213,i1,i2,i3)*u(i1  ,i2-1,i3+1)+ c(m313,i1,
     & i2,i3)*u(i1+1,i2-1,i3+1)+ c(m123,i1,i2,i3)*u(i1-1,i2  ,i3+1)+ 
     & c(m223,i1,i2,i3)*u(i1  ,i2  ,i3+1)+ c(m323,i1,i2,i3)*u(i1+1,i2 
     &  ,i3+1)+ c(m133,i1,i2,i3)*u(i1-1,i2+1,i3+1)+ c(m233,i1,i2,i3)*
     & u(i1  ,i2+1,i3+1)+ c(m333,i1,i2,i3)*u(i1+1,i2+1,i3+1)
        op3dl0(i1,i2,i3)= c(m111,i1,i2,i3)*u(i1-1,i2-1,i3-1)+ c(m211,
     & i1,i2,i3)*u(i1  ,i2-1,i3-1)+ c(m311,i1,i2,i3)*u(i1+1,i2-1,i3-1)
     & + c(m121,i1,i2,i3)*u(i1-1,i2  ,i3-1)+ c(m221,i1,i2,i3)*u(i1  ,
     & i2  ,i3-1)+ c(m321,i1,i2,i3)*u(i1+1,i2  ,i3-1)+ c(m131,i1,i2,
     & i3)*u(i1-1,i2+1,i3-1)+ c(m231,i1,i2,i3)*u(i1  ,i2+1,i3-1)+ c(
     & m331,i1,i2,i3)*u(i1+1,i2+1,i3-1)+ c(m112,i1,i2,i3)*u(i1-1,i2-1,
     & i3  )+ c(m212,i1,i2,i3)*u(i1  ,i2-1,i3  )+ c(m312,i1,i2,i3)*u(
     & i1+1,i2-1,i3  )+ c(m132,i1,i2,i3)*u(i1-1,i2+1,i3  )+ c(m232,i1,
     & i2,i3)*u(i1  ,i2+1,i3  )+ c(m332,i1,i2,i3)*u(i1+1,i2+1,i3  )+ 
     & c(m113,i1,i2,i3)*u(i1-1,i2-1,i3+1)+ c(m213,i1,i2,i3)*u(i1  ,i2-
     & 1,i3+1)+ c(m313,i1,i2,i3)*u(i1+1,i2-1,i3+1)+ c(m123,i1,i2,i3)*
     & u(i1-1,i2  ,i3+1)+ c(m223,i1,i2,i3)*u(i1  ,i2  ,i3+1)+ c(m323,
     & i1,i2,i3)*u(i1+1,i2  ,i3+1)+ c(m133,i1,i2,i3)*u(i1-1,i2+1,i3+1)
     & + c(m233,i1,i2,i3)*u(i1  ,i2+1,i3+1)+ c(m333,i1,i2,i3)*u(i1+1,
     & i2+1,i3+1)
        op3dl1(i1,i2,i3)= c(m111,i1,i2,i3)*u(i1-1,i2-1,i3-1)+ c(m211,
     & i1,i2,i3)*u(i1  ,i2-1,i3-1)+ c(m311,i1,i2,i3)*u(i1+1,i2-1,i3-1)
     & + c(m121,i1,i2,i3)*u(i1-1,i2  ,i3-1)+ c(m221,i1,i2,i3)*u(i1  ,
     & i2  ,i3-1)+ c(m321,i1,i2,i3)*u(i1+1,i2  ,i3-1)+ c(m131,i1,i2,
     & i3)*u(i1-1,i2+1,i3-1)+ c(m231,i1,i2,i3)*u(i1  ,i2+1,i3-1)+ c(
     & m331,i1,i2,i3)*u(i1+1,i2+1,i3-1)+ c(m112,i1,i2,i3)*u(i1-1,i2-1,
     & i3  )+ c(m312,i1,i2,i3)*u(i1+1,i2-1,i3  )+ c(m122,i1,i2,i3)*u(
     & i1-1,i2  ,i3  )+ c(m322,i1,i2,i3)*u(i1+1,i2  ,i3  )+ c(m132,i1,
     & i2,i3)*u(i1-1,i2+1,i3  )+ c(m332,i1,i2,i3)*u(i1+1,i2+1,i3  )+ 
     & c(m113,i1,i2,i3)*u(i1-1,i2-1,i3+1)+ c(m213,i1,i2,i3)*u(i1  ,i2-
     & 1,i3+1)+ c(m313,i1,i2,i3)*u(i1+1,i2-1,i3+1)+ c(m123,i1,i2,i3)*
     & u(i1-1,i2  ,i3+1)+ c(m223,i1,i2,i3)*u(i1  ,i2  ,i3+1)+ c(m323,
     & i1,i2,i3)*u(i1+1,i2  ,i3+1)+ c(m133,i1,i2,i3)*u(i1-1,i2+1,i3+1)
     & + c(m233,i1,i2,i3)*u(i1  ,i2+1,i3+1)+ c(m333,i1,i2,i3)*u(i1+1,
     & i2+1,i3+1)
        op3dl2(i1,i2,i3)= c(m111,i1,i2,i3)*u(i1-1,i2-1,i3-1)+ c(m211,
     & i1,i2,i3)*u(i1  ,i2-1,i3-1)+ c(m311,i1,i2,i3)*u(i1+1,i2-1,i3-1)
     & + c(m121,i1,i2,i3)*u(i1-1,i2  ,i3-1)+ c(m321,i1,i2,i3)*u(i1+1,
     & i2  ,i3-1)+ c(m131,i1,i2,i3)*u(i1-1,i2+1,i3-1)+ c(m231,i1,i2,
     & i3)*u(i1  ,i2+1,i3-1)+ c(m331,i1,i2,i3)*u(i1+1,i2+1,i3-1)+ c(
     & m112,i1,i2,i3)*u(i1-1,i2-1,i3  )+ c(m212,i1,i2,i3)*u(i1  ,i2-1,
     & i3  )+ c(m312,i1,i2,i3)*u(i1+1,i2-1,i3  )+ c(m122,i1,i2,i3)*u(
     & i1-1,i2  ,i3  )+ c(m322,i1,i2,i3)*u(i1+1,i2  ,i3  )+ c(m132,i1,
     & i2,i3)*u(i1-1,i2+1,i3  )+ c(m232,i1,i2,i3)*u(i1  ,i2+1,i3  )+ 
     & c(m332,i1,i2,i3)*u(i1+1,i2+1,i3  )+ c(m113,i1,i2,i3)*u(i1-1,i2-
     & 1,i3+1)+ c(m213,i1,i2,i3)*u(i1  ,i2-1,i3+1)+ c(m313,i1,i2,i3)*
     & u(i1+1,i2-1,i3+1)+ c(m123,i1,i2,i3)*u(i1-1,i2  ,i3+1)+ c(m323,
     & i1,i2,i3)*u(i1+1,i2  ,i3+1)+ c(m133,i1,i2,i3)*u(i1-1,i2+1,i3+1)
     & + c(m233,i1,i2,i3)*u(i1  ,i2+1,i3+1)+ c(m333,i1,i2,i3)*u(i1+1,
     & i2+1,i3+1)
        op3dSparse(i1,i2,i3)= c(m221,i1,i2,i3)*u(i1  ,i2  ,i3-1)+ c(
     & m212,i1,i2,i3)*u(i1  ,i2-1,i3  )+ c(m122,i1,i2,i3)*u(i1-1,i2  ,
     & i3  )+ c(m222,i1,i2,i3)*u(i1  ,i2  ,i3  )+ c(m322,i1,i2,i3)*u(
     & i1+1,i2  ,i3  )+ c(m232,i1,i2,i3)*u(i1  ,i2+1,i3  )+ c(m223,i1,
     & i2,i3)*u(i1  ,i2  ,i3+1)
        op3dSparsel0(i1,i2,i3)= c(m221,i1,i2,i3)*u(i1  ,i2  ,i3-1)+ c(
     & m212,i1,i2,i3)*u(i1  ,i2-1,i3  )+ c(m232,i1,i2,i3)*u(i1  ,i2+1,
     & i3  )+ c(m223,i1,i2,i3)*u(i1  ,i2  ,i3+1)
        op3dSparsel1(i1,i2,i3)= c(m221,i1,i2,i3)*u(i1  ,i2  ,i3-1)+ c(
     & m122,i1,i2,i3)*u(i1-1,i2  ,i3  )+ c(m322,i1,i2,i3)*u(i1+1,i2  ,
     & i3  )+ c(m223,i1,i2,i3)*u(i1  ,i2  ,i3+1)
        op3dSparsel2(i1,i2,i3)= c(m212,i1,i2,i3)*u(i1  ,i2-1,i3  )+ c(
     & m122,i1,i2,i3)*u(i1-1,i2  ,i3  )+ c(m322,i1,i2,i3)*u(i1+1,i2  ,
     & i3  )+ c(m232,i1,i2,i3)*u(i1  ,i2+1,i3  )
        op3dSparseCC(i1,i2,i3) = cc(m221)*u(i1  ,i2  ,i3-1)+ cc(m212)*
     & u(i1  ,i2-1,i3  )+ cc(m122)*u(i1-1,i2  ,i3  )+ cc(m222)*u(i1  ,
     & i2  ,i3  )+ cc(m322)*u(i1+1,i2  ,i3  )+ cc(m232)*u(i1  ,i2+1,
     & i3  )+ cc(m223)*u(i1  ,i2  ,i3+1)
        op3dSparseCCl0(i1,i2,i3) = cc(m221)*u(i1  ,i2  ,i3-1)+ cc(m212)
     & *u(i1  ,i2-1,i3  )+ cc(m232)*u(i1  ,i2+1,i3  )+ cc(m223)*u(i1  
     & ,i2  ,i3+1)
        op3dSparseCCl1(i1,i2,i3) = cc(m221)*u(i1  ,i2  ,i3-1)+ cc(m122)
     & *u(i1-1,i2  ,i3  )+ cc(m322)*u(i1+1,i2  ,i3  )+ cc(m223)*u(i1  
     & ,i2  ,i3+1)
        op3dSparseCCl2(i1,i2,i3) = cc(m212)*u(i1  ,i2-1,i3  )+ cc(m122)
     & *u(i1-1,i2  ,i3  )+ cc(m322)*u(i1+1,i2  ,i3  )+ cc(m232)*u(i1  
     & ,i2+1,i3  )
        op3dCC(i1,i2,i3)= cc(m111)*u(i1-1,i2-1,i3-1)+ cc(m211)*u(i1  ,
     & i2-1,i3-1)+ cc(m311)*u(i1+1,i2-1,i3-1)+ cc(m121)*u(i1-1,i2  ,
     & i3-1)+ cc(m221)*u(i1  ,i2  ,i3-1)+ cc(m321)*u(i1+1,i2  ,i3-1)+ 
     & cc(m131)*u(i1-1,i2+1,i3-1)+ cc(m231)*u(i1  ,i2+1,i3-1)+ cc(
     & m331)*u(i1+1,i2+1,i3-1)+ cc(m112)*u(i1-1,i2-1,i3  )+ cc(m212)*
     & u(i1  ,i2-1,i3  )+ cc(m312)*u(i1+1,i2-1,i3  )+ cc(m122)*u(i1-1,
     & i2  ,i3  )+ cc(m222)*u(i1  ,i2  ,i3  )+ cc(m322)*u(i1+1,i2  ,
     & i3  )+ cc(m132)*u(i1-1,i2+1,i3  )+ cc(m232)*u(i1  ,i2+1,i3  )+ 
     & cc(m332)*u(i1+1,i2+1,i3  )+ cc(m113)*u(i1-1,i2-1,i3+1)+ cc(
     & m213)*u(i1  ,i2-1,i3+1)+ cc(m313)*u(i1+1,i2-1,i3+1)+ cc(m123)*
     & u(i1-1,i2  ,i3+1)+ cc(m223)*u(i1  ,i2  ,i3+1)+ cc(m323)*u(i1+1,
     & i2  ,i3+1)+ cc(m133)*u(i1-1,i2+1,i3+1)+ cc(m233)*u(i1  ,i2+1,
     & i3+1)+ cc(m333)*u(i1+1,i2+1,i3+1)
        op3dCCl0(i1,i2,i3)= cc(m111)*u(i1-1,i2-1,i3-1)+ cc(m211)*u(i1  
     & ,i2-1,i3-1)+ cc(m311)*u(i1+1,i2-1,i3-1)+ cc(m121)*u(i1-1,i2  ,
     & i3-1)+ cc(m221)*u(i1  ,i2  ,i3-1)+ cc(m321)*u(i1+1,i2  ,i3-1)+ 
     & cc(m131)*u(i1-1,i2+1,i3-1)+ cc(m231)*u(i1  ,i2+1,i3-1)+ cc(
     & m331)*u(i1+1,i2+1,i3-1)+ cc(m112)*u(i1-1,i2-1,i3  )+ cc(m212)*
     & u(i1  ,i2-1,i3  )+ cc(m312)*u(i1+1,i2-1,i3  )+ cc(m132)*u(i1-1,
     & i2+1,i3  )+ cc(m232)*u(i1  ,i2+1,i3  )+ cc(m332)*u(i1+1,i2+1,
     & i3  )+ cc(m113)*u(i1-1,i2-1,i3+1)+ cc(m213)*u(i1  ,i2-1,i3+1)+ 
     & cc(m313)*u(i1+1,i2-1,i3+1)+ cc(m123)*u(i1-1,i2  ,i3+1)+ cc(
     & m223)*u(i1  ,i2  ,i3+1)+ cc(m323)*u(i1+1,i2  ,i3+1)+ cc(m133)*
     & u(i1-1,i2+1,i3+1)+ cc(m233)*u(i1  ,i2+1,i3+1)+ cc(m333)*u(i1+1,
     & i2+1,i3+1)
        op3dCCl1(i1,i2,i3)= cc(m111)*u(i1-1,i2-1,i3-1)+ cc(m211)*u(i1  
     & ,i2-1,i3-1)+ cc(m311)*u(i1+1,i2-1,i3-1)+ cc(m121)*u(i1-1,i2  ,
     & i3-1)+ cc(m221)*u(i1  ,i2  ,i3-1)+ cc(m321)*u(i1+1,i2  ,i3-1)+ 
     & cc(m131)*u(i1-1,i2+1,i3-1)+ cc(m231)*u(i1  ,i2+1,i3-1)+ cc(
     & m331)*u(i1+1,i2+1,i3-1)+ cc(m112)*u(i1-1,i2-1,i3  )+ cc(m312)*
     & u(i1+1,i2-1,i3  )+ cc(m122)*u(i1-1,i2  ,i3  )+ cc(m322)*u(i1+1,
     & i2  ,i3  )+ cc(m132)*u(i1-1,i2+1,i3  )+ cc(m332)*u(i1+1,i2+1,
     & i3  )+ cc(m113)*u(i1-1,i2-1,i3+1)+ cc(m213)*u(i1  ,i2-1,i3+1)+ 
     & cc(m313)*u(i1+1,i2-1,i3+1)+ cc(m123)*u(i1-1,i2  ,i3+1)+ cc(
     & m223)*u(i1  ,i2  ,i3+1)+ cc(m323)*u(i1+1,i2  ,i3+1)+ cc(m133)*
     & u(i1-1,i2+1,i3+1)+ cc(m233)*u(i1  ,i2+1,i3+1)+ cc(m333)*u(i1+1,
     & i2+1,i3+1)
        op3dCCl2(i1,i2,i3)= cc(m111)*u(i1-1,i2-1,i3-1)+ cc(m211)*u(i1  
     & ,i2-1,i3-1)+ cc(m311)*u(i1+1,i2-1,i3-1)+ cc(m121)*u(i1-1,i2  ,
     & i3-1)+ cc(m321)*u(i1+1,i2  ,i3-1)+ cc(m131)*u(i1-1,i2+1,i3-1)+ 
     & cc(m231)*u(i1  ,i2+1,i3-1)+ cc(m331)*u(i1+1,i2+1,i3-1)+ cc(
     & m112)*u(i1-1,i2-1,i3  )+ cc(m212)*u(i1  ,i2-1,i3  )+ cc(m312)*
     & u(i1+1,i2-1,i3  )+ cc(m122)*u(i1-1,i2  ,i3  )+ cc(m322)*u(i1+1,
     & i2  ,i3  )+ cc(m132)*u(i1-1,i2+1,i3  )+ cc(m232)*u(i1  ,i2+1,
     & i3  )+ cc(m332)*u(i1+1,i2+1,i3  )+ cc(m113)*u(i1-1,i2-1,i3+1)+ 
     & cc(m213)*u(i1  ,i2-1,i3+1)+ cc(m313)*u(i1+1,i2-1,i3+1)+ cc(
     & m123)*u(i1-1,i2  ,i3+1)+ cc(m323)*u(i1+1,i2  ,i3+1)+ cc(m133)*
     & u(i1-1,i2+1,i3+1)+ cc(m233)*u(i1  ,i2+1,i3+1)+ cc(m333)*u(i1+1,
     & i2+1,i3+1)
! #If "3" == "2"
! #If "3" == "3"
        op3dSparseVC(i1,i2,i3) = a3m*u(i1  ,i2  ,i3-1)+ a2m*u(i1  ,i2-
     & 1,i3  )+ a1m*u(i1-1,i2  ,i3  )+ ad*u(i1  ,i2  ,i3  )+ a1p*u(i1+
     & 1,i2  ,i3  )+ a2p*u(i1  ,i2+1,i3  )+ a3p*u(i1  ,i2  ,i3+1)
        op3dSparseVCl0(i1,i2,i3) = a3m*u(i1  ,i2  ,i3-1)+ a2m*u(i1  ,
     & i2-1,i3  )+ a2p*u(i1  ,i2+1,i3  )+ a3p*u(i1  ,i2  ,i3+1)
        op3dSparseVCl1(i1,i2,i3) = a3m*u(i1  ,i2  ,i3-1)+ a1m*u(i1-1,
     & i2  ,i3  )+ a1p*u(i1+1,i2  ,i3  )+ a3p*u(i1  ,i2  ,i3+1)
        op3dSparseVCl2(i1,i2,i3) = a2m*u(i1  ,i2-1,i3  )+ a1m*u(i1-1,
     & i2  ,i3  )+ a1p*u(i1+1,i2  ,i3  )+ a2p*u(i1  ,i2+1,i3  )
! #If "3" == "2"
! #If "3" == "3"
        op3dSparse4(i1,i2,i3) =  c(m331,i1,i2,i3)*u(i1  ,i2  ,i3-2)+ c(
     & m332,i1,i2,i3)*u(i1  ,i2  ,i3-1)+ c(m313,i1,i2,i3)*u(i1  ,i2-2,
     & i3  )+ c(m323,i1,i2,i3)*u(i1  ,i2-1,i3  )+ c(m133,i1,i2,i3)*u(
     & i1-2,i2  ,i3  )+ c(m233,i1,i2,i3)*u(i1-1,i2  ,i3  )+ c(m333,i1,
     & i2,i3)*u(i1  ,i2  ,i3  )+ c(m433,i1,i2,i3)*u(i1+1,i2  ,i3  )+ 
     & c(m533,i1,i2,i3)*u(i1+2,i2  ,i3  )+ c(m343,i1,i2,i3)*u(i1  ,i2+
     & 1,i3  )+ c(m353,i1,i2,i3)*u(i1  ,i2+2,i3  )+ c(m334,i1,i2,i3)*
     & u(i1  ,i2  ,i3+1)+ c(m335,i1,i2,i3)*u(i1  ,i2  ,i3+2)
        op3dSparse4l0(i1,i2,i3) =  c(m331,i1,i2,i3)*u(i1  ,i2  ,i3-2)+ 
     & c(m332,i1,i2,i3)*u(i1  ,i2  ,i3-1)+ c(m313,i1,i2,i3)*u(i1  ,i2-
     & 2,i3  )+ c(m323,i1,i2,i3)*u(i1  ,i2-1,i3  )+ c(m343,i1,i2,i3)*
     & u(i1  ,i2+1,i3  )+ c(m353,i1,i2,i3)*u(i1  ,i2+2,i3  )+ c(m334,
     & i1,i2,i3)*u(i1  ,i2  ,i3+1)+ c(m335,i1,i2,i3)*u(i1  ,i2  ,i3+2)
        op3dSparse4l1(i1,i2,i3) =  c(m331,i1,i2,i3)*u(i1  ,i2  ,i3-2)+ 
     & c(m332,i1,i2,i3)*u(i1  ,i2  ,i3-1)+ c(m133,i1,i2,i3)*u(i1-2,i2 
     &  ,i3  )+ c(m233,i1,i2,i3)*u(i1-1,i2  ,i3  )+ c(m433,i1,i2,i3)*
     & u(i1+1,i2  ,i3  )+ c(m533,i1,i2,i3)*u(i1+2,i2  ,i3  )+ c(m334,
     & i1,i2,i3)*u(i1  ,i2  ,i3+1)+ c(m335,i1,i2,i3)*u(i1  ,i2  ,i3+2)
        op3dSparse4l2(i1,i2,i3) =  c(m313,i1,i2,i3)*u(i1  ,i2-2,i3  )+ 
     & c(m323,i1,i2,i3)*u(i1  ,i2-1,i3  )+ c(m133,i1,i2,i3)*u(i1-2,i2 
     &  ,i3  )+ c(m233,i1,i2,i3)*u(i1-1,i2  ,i3  )+ c(m433,i1,i2,i3)*
     & u(i1+1,i2  ,i3  )+ c(m533,i1,i2,i3)*u(i1+2,i2  ,i3  )+ c(m343,
     & i1,i2,i3)*u(i1  ,i2+1,i3  )+ c(m353,i1,i2,i3)*u(i1  ,i2+2,i3  )
        op3d4a(i1,i2,i3,n, m11n,m21n,m31n,m41n,m51n, m12n,m22n,m32n,
     & m42n,m52n, m13n,m23n,m33n,m43n,m53n, m14n,m24n,m34n,m44n,m54n, 
     & m15n,m25n,m35n,m45n,m55n)= c(m11n,i1,i2,i3)*u(i1-2,i2-2,i3+n)+ 
     & c(m21n,i1,i2,i3)*u(i1-1,i2-2,i3+n)+ c(m31n,i1,i2,i3)*u(i1  ,i2-
     & 2,i3+n)+ c(m41n,i1,i2,i3)*u(i1+1,i2-2,i3+n)+ c(m51n,i1,i2,i3)*
     & u(i1+2,i2-2,i3+n)+ c(m12n,i1,i2,i3)*u(i1-2,i2-1,i3+n)+ c(m22n,
     & i1,i2,i3)*u(i1-1,i2-1,i3+n)+ c(m32n,i1,i2,i3)*u(i1  ,i2-1,i3+n)
     & + c(m42n,i1,i2,i3)*u(i1+1,i2-1,i3+n)+ c(m52n,i1,i2,i3)*u(i1+2,
     & i2-1,i3+n)+ c(m13n,i1,i2,i3)*u(i1-2,i2  ,i3+n)+ c(m23n,i1,i2,
     & i3)*u(i1-1,i2  ,i3+n)+ c(m33n,i1,i2,i3)*u(i1  ,i2  ,i3+n)+ c(
     & m43n,i1,i2,i3)*u(i1+1,i2  ,i3+n)+ c(m53n,i1,i2,i3)*u(i1+2,i2  ,
     & i3+n)+ c(m14n,i1,i2,i3)*u(i1-2,i2+1,i3+n)+ c(m24n,i1,i2,i3)*u(
     & i1-1,i2+1,i3+n)+ c(m34n,i1,i2,i3)*u(i1  ,i2+1,i3+n)+ c(m44n,i1,
     & i2,i3)*u(i1+1,i2+1,i3+n)+ c(m54n,i1,i2,i3)*u(i1+2,i2+1,i3+n)+ 
     & c(m15n,i1,i2,i3)*u(i1-2,i2+2,i3+n)+ c(m25n,i1,i2,i3)*u(i1-1,i2+
     & 2,i3+n)+ c(m35n,i1,i2,i3)*u(i1  ,i2+2,i3+n)+ c(m45n,i1,i2,i3)*
     & u(i1+1,i2+2,i3+n)+ c(m55n,i1,i2,i3)*u(i1+2,i2+2,i3+n)
        op3d4al0(i1,i2,i3,n, m11n,m21n,m31n,m41n,m51n, m12n,m22n,m32n,
     & m42n,m52n, m13n,m23n,m33n,m43n,m53n, m14n,m24n,m34n,m44n,m54n, 
     & m15n,m25n,m35n,m45n,m55n)= c(m11n,i1,i2,i3)*u(i1-2,i2-2,i3+n)+ 
     & c(m21n,i1,i2,i3)*u(i1-1,i2-2,i3+n)+ c(m31n,i1,i2,i3)*u(i1  ,i2-
     & 2,i3+n)+ c(m41n,i1,i2,i3)*u(i1+1,i2-2,i3+n)+ c(m51n,i1,i2,i3)*
     & u(i1+2,i2-2,i3+n)+ c(m12n,i1,i2,i3)*u(i1-2,i2-1,i3+n)+ c(m22n,
     & i1,i2,i3)*u(i1-1,i2-1,i3+n)+ c(m32n,i1,i2,i3)*u(i1  ,i2-1,i3+n)
     & + c(m42n,i1,i2,i3)*u(i1+1,i2-1,i3+n)+ c(m52n,i1,i2,i3)*u(i1+2,
     & i2-1,i3+n)+ c(m14n,i1,i2,i3)*u(i1-2,i2+1,i3+n)+ c(m24n,i1,i2,
     & i3)*u(i1-1,i2+1,i3+n)+ c(m34n,i1,i2,i3)*u(i1  ,i2+1,i3+n)+ c(
     & m44n,i1,i2,i3)*u(i1+1,i2+1,i3+n)+ c(m54n,i1,i2,i3)*u(i1+2,i2+1,
     & i3+n)+ c(m15n,i1,i2,i3)*u(i1-2,i2+2,i3+n)+ c(m25n,i1,i2,i3)*u(
     & i1-1,i2+2,i3+n)+ c(m35n,i1,i2,i3)*u(i1  ,i2+2,i3+n)+ c(m45n,i1,
     & i2,i3)*u(i1+1,i2+2,i3+n)+ c(m55n,i1,i2,i3)*u(i1+2,i2+2,i3+n)
        op3d4al1(i1,i2,i3,n, m11n,m21n,m31n,m41n,m51n, m12n,m22n,m32n,
     & m42n,m52n, m13n,m23n,m33n,m43n,m53n, m14n,m24n,m34n,m44n,m54n, 
     & m15n,m25n,m35n,m45n,m55n)= c(m11n,i1,i2,i3)*u(i1-2,i2-2,i3+n)+ 
     & c(m21n,i1,i2,i3)*u(i1-1,i2-2,i3+n)+ c(m41n,i1,i2,i3)*u(i1+1,i2-
     & 2,i3+n)+ c(m51n,i1,i2,i3)*u(i1+2,i2-2,i3+n)+ c(m12n,i1,i2,i3)*
     & u(i1-2,i2-1,i3+n)+ c(m22n,i1,i2,i3)*u(i1-1,i2-1,i3+n)+ c(m42n,
     & i1,i2,i3)*u(i1+1,i2-1,i3+n)+ c(m52n,i1,i2,i3)*u(i1+2,i2-1,i3+n)
     & + c(m13n,i1,i2,i3)*u(i1-2,i2  ,i3+n)+ c(m23n,i1,i2,i3)*u(i1-1,
     & i2  ,i3+n)+ c(m43n,i1,i2,i3)*u(i1+1,i2  ,i3+n)+ c(m53n,i1,i2,
     & i3)*u(i1+2,i2  ,i3+n)+ c(m14n,i1,i2,i3)*u(i1-2,i2+1,i3+n)+ c(
     & m24n,i1,i2,i3)*u(i1-1,i2+1,i3+n)+ c(m44n,i1,i2,i3)*u(i1+1,i2+1,
     & i3+n)+ c(m54n,i1,i2,i3)*u(i1+2,i2+1,i3+n)+ c(m15n,i1,i2,i3)*u(
     & i1-2,i2+2,i3+n)+ c(m25n,i1,i2,i3)*u(i1-1,i2+2,i3+n)+ c(m45n,i1,
     & i2,i3)*u(i1+1,i2+2,i3+n)+ c(m55n,i1,i2,i3)*u(i1+2,i2+2,i3+n)
        op3d4al2(i1,i2,i3,n, m11n,m21n,m31n,m41n,m51n, m12n,m22n,m32n,
     & m42n,m52n, m13n,m23n,m33n,m43n,m53n, m14n,m24n,m34n,m44n,m54n, 
     & m15n,m25n,m35n,m45n,m55n)= c(m11n,i1,i2,i3)*u(i1-2,i2-2,i3+n)+ 
     & c(m21n,i1,i2,i3)*u(i1-1,i2-2,i3+n)+ c(m31n,i1,i2,i3)*u(i1  ,i2-
     & 2,i3+n)+ c(m41n,i1,i2,i3)*u(i1+1,i2-2,i3+n)+ c(m51n,i1,i2,i3)*
     & u(i1+2,i2-2,i3+n)+ c(m12n,i1,i2,i3)*u(i1-2,i2-1,i3+n)+ c(m22n,
     & i1,i2,i3)*u(i1-1,i2-1,i3+n)+ c(m32n,i1,i2,i3)*u(i1  ,i2-1,i3+n)
     & + c(m42n,i1,i2,i3)*u(i1+1,i2-1,i3+n)+ c(m52n,i1,i2,i3)*u(i1+2,
     & i2-1,i3+n)+ c(m13n,i1,i2,i3)*u(i1-2,i2  ,i3+n)+ c(m23n,i1,i2,
     & i3)*u(i1-1,i2  ,i3+n)+ c(m43n,i1,i2,i3)*u(i1+1,i2  ,i3+n)+ c(
     & m53n,i1,i2,i3)*u(i1+2,i2  ,i3+n)+ c(m14n,i1,i2,i3)*u(i1-2,i2+1,
     & i3+n)+ c(m24n,i1,i2,i3)*u(i1-1,i2+1,i3+n)+ c(m34n,i1,i2,i3)*u(
     & i1  ,i2+1,i3+n)+ c(m44n,i1,i2,i3)*u(i1+1,i2+1,i3+n)+ c(m54n,i1,
     & i2,i3)*u(i1+2,i2+1,i3+n)+ c(m15n,i1,i2,i3)*u(i1-2,i2+2,i3+n)+ 
     & c(m25n,i1,i2,i3)*u(i1-1,i2+2,i3+n)+ c(m35n,i1,i2,i3)*u(i1  ,i2+
     & 2,i3+n)+ c(m45n,i1,i2,i3)*u(i1+1,i2+2,i3+n)+ c(m55n,i1,i2,i3)*
     & u(i1+2,i2+2,i3+n)
        op3d4(i1,i2,i3)= op3d4a(i1,i2,i3,-2, m111,m211,m311,m411,m511, 
     & m121,m221,m321,m421,m521, m131,m231,m331,m431,m531, m141,m241,
     & m341,m441,m541, m151,m251,m351,m451,m551) +op3d4a(i1,i2,i3,-1, 
     & m112,m212,m312,m412,m512, m122,m222,m322,m422,m522, m132,m232,
     & m332,m432,m532, m142,m242,m342,m442,m542, m152,m252,m352,m452,
     & m552) +op3d4a(i1,i2,i3,0, m113,m213,m313,m413,m513, m123,m223,
     & m323,m423,m523, m133,m233,m333,m433,m533, m143,m243,m343,m443,
     & m543, m153,m253,m353,m453,m553) +op3d4a(i1,i2,i3,1, m114,m214,
     & m314,m414,m514, m124,m224,m324,m424,m524, m134,m234,m334,m434,
     & m534, m144,m244,m344,m444,m544, m154,m254,m354,m454,m554) +
     & op3d4a(i1,i2,i3,2, m115,m215,m315,m415,m515, m125,m225,m325,
     & m425,m525, m135,m235,m335,m435,m535, m145,m245,m345,m445,m545, 
     & m155,m255,m355,m455,m555)
        op3d4l0(i1,i2,i3)= op3d4a(i1,i2,i3,-2, m111,m211,m311,m411,
     & m511, m121,m221,m321,m421,m521, m131,m231,m331,m431,m531, m141,
     & m241,m341,m441,m541, m151,m251,m351,m451,m551) +op3d4a(i1,i2,
     & i3,-1, m112,m212,m312,m412,m512, m122,m222,m322,m422,m522, 
     & m132,m232,m332,m432,m532, m142,m242,m342,m442,m542, m152,m252,
     & m352,m452,m552) +op3d4al0(i1,i2,i3,0, m113,m213,m313,m413,m513,
     &  m123,m223,m323,m423,m523, m133,m233,m333,m433,m533, m143,m243,
     & m343,m443,m543, m153,m253,m353,m453,m553) +op3d4a(i1,i2,i3,1, 
     & m114,m214,m314,m414,m514, m124,m224,m324,m424,m524, m134,m234,
     & m334,m434,m534, m144,m244,m344,m444,m544, m154,m254,m354,m454,
     & m554) +op3d4a(i1,i2,i3,2, m115,m215,m315,m415,m515, m125,m225,
     & m325,m425,m525, m135,m235,m335,m435,m535, m145,m245,m345,m445,
     & m545, m155,m255,m355,m455,m555)
        op3d4l1(i1,i2,i3)= op3d4a(i1,i2,i3,-2, m111,m211,m311,m411,
     & m511, m121,m221,m321,m421,m521, m131,m231,m331,m431,m531, m141,
     & m241,m341,m441,m541, m151,m251,m351,m451,m551) +op3d4a(i1,i2,
     & i3,-1, m112,m212,m312,m412,m512, m122,m222,m322,m422,m522, 
     & m132,m232,m332,m432,m532, m142,m242,m342,m442,m542, m152,m252,
     & m352,m452,m552) +op3d4al1(i1,i2,i3,0, m113,m213,m313,m413,m513,
     &  m123,m223,m323,m423,m523, m133,m233,m333,m433,m533, m143,m243,
     & m343,m443,m543, m153,m253,m353,m453,m553) +op3d4a(i1,i2,i3,1, 
     & m114,m214,m314,m414,m514, m124,m224,m324,m424,m524, m134,m234,
     & m334,m434,m534, m144,m244,m344,m444,m544, m154,m254,m354,m454,
     & m554) +op3d4a(i1,i2,i3,2, m115,m215,m315,m415,m515, m125,m225,
     & m325,m425,m525, m135,m235,m335,m435,m535, m145,m245,m345,m445,
     & m545, m155,m255,m355,m455,m555)
        op3d4l2(i1,i2,i3)= op3d4al2(i1,i2,i3,-2, m111,m211,m311,m411,
     & m511, m121,m221,m321,m421,m521, m131,m231,m331,m431,m531, m141,
     & m241,m341,m441,m541, m151,m251,m351,m451,m551) +op3d4al2(i1,i2,
     & i3,-1, m112,m212,m312,m412,m512, m122,m222,m322,m422,m522, 
     & m132,m232,m332,m432,m532, m142,m242,m342,m442,m542, m152,m252,
     & m352,m452,m552) +op3d4al2(i1,i2,i3,0, m113,m213,m313,m413,m513,
     &  m123,m223,m323,m423,m523, m133,m233,m333,m433,m533, m143,m243,
     & m343,m443,m543, m153,m253,m353,m453,m553) +op3d4al2(i1,i2,i3,1,
     &  m114,m214,m314,m414,m514, m124,m224,m324,m424,m524, m134,m234,
     & m334,m434,m534, m144,m244,m344,m444,m544, m154,m254,m354,m454,
     & m554) +op3d4al2(i1,i2,i3,2, m115,m215,m315,m415,m515, m125,
     & m225,m325,m425,m525, m135,m235,m335,m435,m535, m145,m245,m345,
     & m445,m545, m155,m255,m355,m455,m555)
! #If "3" == "2"
! #If "3" == "3"
        op3dSparseCC4(i1,i2,i3) =  cc(m331)*u(i1  ,i2  ,i3-2)+ cc(m332)
     & *u(i1  ,i2  ,i3-1)+ cc(m313)*u(i1  ,i2-2,i3  )+ cc(m323)*u(i1  
     & ,i2-1,i3  )+ cc(m133)*u(i1-2,i2  ,i3  )+ cc(m233)*u(i1-1,i2  ,
     & i3  )+ cc(m333)*u(i1  ,i2  ,i3  )+ cc(m433)*u(i1+1,i2  ,i3  )+ 
     & cc(m533)*u(i1+2,i2  ,i3  )+ cc(m343)*u(i1  ,i2+1,i3  )+ cc(
     & m353)*u(i1  ,i2+2,i3  )+ cc(m334)*u(i1  ,i2  ,i3+1)+ cc(m335)*
     & u(i1  ,i2  ,i3+2)
        op3dSparseCC4l0(i1,i2,i3) =  cc(m331)*u(i1  ,i2  ,i3-2)+ cc(
     & m332)*u(i1  ,i2  ,i3-1)+ cc(m313)*u(i1  ,i2-2,i3  )+ cc(m323)*
     & u(i1  ,i2-1,i3  )+ cc(m343)*u(i1  ,i2+1,i3  )+ cc(m353)*u(i1  ,
     & i2+2,i3  )+ cc(m334)*u(i1  ,i2  ,i3+1)+ cc(m335)*u(i1  ,i2  ,
     & i3+2)
        op3dSparseCC4l1(i1,i2,i3) =  cc(m331)*u(i1  ,i2  ,i3-2)+ cc(
     & m332)*u(i1  ,i2  ,i3-1)+ cc(m133)*u(i1-2,i2  ,i3  )+ cc(m233)*
     & u(i1-1,i2  ,i3  )+ cc(m433)*u(i1+1,i2  ,i3  )+ cc(m533)*u(i1+2,
     & i2  ,i3  )+ cc(m334)*u(i1  ,i2  ,i3+1)+ cc(m335)*u(i1  ,i2  ,
     & i3+2)
        op3dSparseCC4l2(i1,i2,i3) =  cc(m313)*u(i1  ,i2-2,i3  )+ cc(
     & m323)*u(i1  ,i2-1,i3  )+ cc(m133)*u(i1-2,i2  ,i3  )+ cc(m233)*
     & u(i1-1,i2  ,i3  )+ cc(m433)*u(i1+1,i2  ,i3  )+ cc(m533)*u(i1+2,
     & i2  ,i3  )+ cc(m343)*u(i1  ,i2+1,i3  )+ cc(m353)*u(i1  ,i2+2,
     & i3  )
        op3dCC4a(i1,i2,i3,n, m11n,m21n,m31n,m41n,m51n, m12n,m22n,m32n,
     & m42n,m52n, m13n,m23n,m33n,m43n,m53n, m14n,m24n,m34n,m44n,m54n, 
     & m15n,m25n,m35n,m45n,m55n)= cc(m11n)*u(i1-2,i2-2,i3+n)+ cc(m21n)
     & *u(i1-1,i2-2,i3+n)+ cc(m31n)*u(i1  ,i2-2,i3+n)+ cc(m41n)*u(i1+
     & 1,i2-2,i3+n)+ cc(m51n)*u(i1+2,i2-2,i3+n)+ cc(m12n)*u(i1-2,i2-1,
     & i3+n)+ cc(m22n)*u(i1-1,i2-1,i3+n)+ cc(m32n)*u(i1  ,i2-1,i3+n)+ 
     & cc(m42n)*u(i1+1,i2-1,i3+n)+ cc(m52n)*u(i1+2,i2-1,i3+n)+ cc(
     & m13n)*u(i1-2,i2  ,i3+n)+ cc(m23n)*u(i1-1,i2  ,i3+n)+ cc(m33n)*
     & u(i1  ,i2  ,i3+n)+ cc(m43n)*u(i1+1,i2  ,i3+n)+ cc(m53n)*u(i1+2,
     & i2  ,i3+n)+ cc(m14n)*u(i1-2,i2+1,i3+n)+ cc(m24n)*u(i1-1,i2+1,
     & i3+n)+ cc(m34n)*u(i1  ,i2+1,i3+n)+ cc(m44n)*u(i1+1,i2+1,i3+n)+ 
     & cc(m54n)*u(i1+2,i2+1,i3+n)+ cc(m15n)*u(i1-2,i2+2,i3+n)+ cc(
     & m25n)*u(i1-1,i2+2,i3+n)+ cc(m35n)*u(i1  ,i2+2,i3+n)+ cc(m45n)*
     & u(i1+1,i2+2,i3+n)+ cc(m55n)*u(i1+2,i2+2,i3+n)
        op3dCC4al0(i1,i2,i3,n, m11n,m21n,m31n,m41n,m51n, m12n,m22n,
     & m32n,m42n,m52n, m13n,m23n,m33n,m43n,m53n, m14n,m24n,m34n,m44n,
     & m54n, m15n,m25n,m35n,m45n,m55n)= cc(m11n)*u(i1-2,i2-2,i3+n)+ 
     & cc(m21n)*u(i1-1,i2-2,i3+n)+ cc(m31n)*u(i1  ,i2-2,i3+n)+ cc(
     & m41n)*u(i1+1,i2-2,i3+n)+ cc(m51n)*u(i1+2,i2-2,i3+n)+ cc(m12n)*
     & u(i1-2,i2-1,i3+n)+ cc(m22n)*u(i1-1,i2-1,i3+n)+ cc(m32n)*u(i1  ,
     & i2-1,i3+n)+ cc(m42n)*u(i1+1,i2-1,i3+n)+ cc(m52n)*u(i1+2,i2-1,
     & i3+n)+ cc(m14n)*u(i1-2,i2+1,i3+n)+ cc(m24n)*u(i1-1,i2+1,i3+n)+ 
     & cc(m34n)*u(i1  ,i2+1,i3+n)+ cc(m44n)*u(i1+1,i2+1,i3+n)+ cc(
     & m54n)*u(i1+2,i2+1,i3+n)+ cc(m15n)*u(i1-2,i2+2,i3+n)+ cc(m25n)*
     & u(i1-1,i2+2,i3+n)+ cc(m35n)*u(i1  ,i2+2,i3+n)+ cc(m45n)*u(i1+1,
     & i2+2,i3+n)+ cc(m55n)*u(i1+2,i2+2,i3+n)
        op3dCC4al1(i1,i2,i3,n, m11n,m21n,m31n,m41n,m51n, m12n,m22n,
     & m32n,m42n,m52n, m13n,m23n,m33n,m43n,m53n, m14n,m24n,m34n,m44n,
     & m54n, m15n,m25n,m35n,m45n,m55n)= cc(m11n)*u(i1-2,i2-2,i3+n)+ 
     & cc(m21n)*u(i1-1,i2-2,i3+n)+ cc(m41n)*u(i1+1,i2-2,i3+n)+ cc(
     & m51n)*u(i1+2,i2-2,i3+n)+ cc(m12n)*u(i1-2,i2-1,i3+n)+ cc(m22n)*
     & u(i1-1,i2-1,i3+n)+ cc(m42n)*u(i1+1,i2-1,i3+n)+ cc(m52n)*u(i1+2,
     & i2-1,i3+n)+ cc(m13n)*u(i1-2,i2  ,i3+n)+ cc(m23n)*u(i1-1,i2  ,
     & i3+n)+ cc(m43n)*u(i1+1,i2  ,i3+n)+ cc(m53n)*u(i1+2,i2  ,i3+n)+ 
     & cc(m14n)*u(i1-2,i2+1,i3+n)+ cc(m24n)*u(i1-1,i2+1,i3+n)+ cc(
     & m44n)*u(i1+1,i2+1,i3+n)+ cc(m54n)*u(i1+2,i2+1,i3+n)+ cc(m15n)*
     & u(i1-2,i2+2,i3+n)+ cc(m25n)*u(i1-1,i2+2,i3+n)+ cc(m45n)*u(i1+1,
     & i2+2,i3+n)+ cc(m55n)*u(i1+2,i2+2,i3+n)
        op3dCC4al2(i1,i2,i3,n, m11n,m21n,m31n,m41n,m51n, m12n,m22n,
     & m32n,m42n,m52n, m13n,m23n,m33n,m43n,m53n, m14n,m24n,m34n,m44n,
     & m54n, m15n,m25n,m35n,m45n,m55n)= cc(m11n)*u(i1-2,i2-2,i3+n)+ 
     & cc(m21n)*u(i1-1,i2-2,i3+n)+ cc(m31n)*u(i1  ,i2-2,i3+n)+ cc(
     & m41n)*u(i1+1,i2-2,i3+n)+ cc(m51n)*u(i1+2,i2-2,i3+n)+ cc(m12n)*
     & u(i1-2,i2-1,i3+n)+ cc(m22n)*u(i1-1,i2-1,i3+n)+ cc(m32n)*u(i1  ,
     & i2-1,i3+n)+ cc(m42n)*u(i1+1,i2-1,i3+n)+ cc(m52n)*u(i1+2,i2-1,
     & i3+n)+ cc(m13n)*u(i1-2,i2  ,i3+n)+ cc(m23n)*u(i1-1,i2  ,i3+n)+ 
     & cc(m43n)*u(i1+1,i2  ,i3+n)+ cc(m53n)*u(i1+2,i2  ,i3+n)+ cc(
     & m14n)*u(i1-2,i2+1,i3+n)+ cc(m24n)*u(i1-1,i2+1,i3+n)+ cc(m34n)*
     & u(i1  ,i2+1,i3+n)+ cc(m44n)*u(i1+1,i2+1,i3+n)+ cc(m54n)*u(i1+2,
     & i2+1,i3+n)+ cc(m15n)*u(i1-2,i2+2,i3+n)+ cc(m25n)*u(i1-1,i2+2,
     & i3+n)+ cc(m35n)*u(i1  ,i2+2,i3+n)+ cc(m45n)*u(i1+1,i2+2,i3+n)+ 
     & cc(m55n)*u(i1+2,i2+2,i3+n)
        op3dCC4(i1,i2,i3)= op3dCC4a(i1,i2,i3,-2, m111,m211,m311,m411,
     & m511, m121,m221,m321,m421,m521, m131,m231,m331,m431,m531, m141,
     & m241,m341,m441,m541, m151,m251,m351,m451,m551) +op3dCC4a(i1,i2,
     & i3,-1, m112,m212,m312,m412,m512, m122,m222,m322,m422,m522, 
     & m132,m232,m332,m432,m532, m142,m242,m342,m442,m542, m152,m252,
     & m352,m452,m552) +op3dCC4a(i1,i2,i3,0, m113,m213,m313,m413,m513,
     &  m123,m223,m323,m423,m523, m133,m233,m333,m433,m533, m143,m243,
     & m343,m443,m543, m153,m253,m353,m453,m553) +op3dCC4a(i1,i2,i3,1,
     &  m114,m214,m314,m414,m514, m124,m224,m324,m424,m524, m134,m234,
     & m334,m434,m534, m144,m244,m344,m444,m544, m154,m254,m354,m454,
     & m554) +op3dCC4a(i1,i2,i3,2, m115,m215,m315,m415,m515, m125,
     & m225,m325,m425,m525, m135,m235,m335,m435,m535, m145,m245,m345,
     & m445,m545, m155,m255,m355,m455,m555)
        op3dCC4l0(i1,i2,i3)= op3dCC4a(i1,i2,i3,-2, m111,m211,m311,m411,
     & m511, m121,m221,m321,m421,m521, m131,m231,m331,m431,m531, m141,
     & m241,m341,m441,m541, m151,m251,m351,m451,m551) +op3dCC4a(i1,i2,
     & i3,-1, m112,m212,m312,m412,m512, m122,m222,m322,m422,m522, 
     & m132,m232,m332,m432,m532, m142,m242,m342,m442,m542, m152,m252,
     & m352,m452,m552) +op3dCC4al0(i1,i2,i3,0, m113,m213,m313,m413,
     & m513, m123,m223,m323,m423,m523, m133,m233,m333,m433,m533, m143,
     & m243,m343,m443,m543, m153,m253,m353,m453,m553) +op3dCC4a(i1,i2,
     & i3,1, m114,m214,m314,m414,m514, m124,m224,m324,m424,m524, m134,
     & m234,m334,m434,m534, m144,m244,m344,m444,m544, m154,m254,m354,
     & m454,m554) +op3dCC4a(i1,i2,i3,2, m115,m215,m315,m415,m515, 
     & m125,m225,m325,m425,m525, m135,m235,m335,m435,m535, m145,m245,
     & m345,m445,m545, m155,m255,m355,m455,m555)
        op3dCC4l1(i1,i2,i3)= op3dCC4a(i1,i2,i3,-2, m111,m211,m311,m411,
     & m511, m121,m221,m321,m421,m521, m131,m231,m331,m431,m531, m141,
     & m241,m341,m441,m541, m151,m251,m351,m451,m551) +op3dCC4a(i1,i2,
     & i3,-1, m112,m212,m312,m412,m512, m122,m222,m322,m422,m522, 
     & m132,m232,m332,m432,m532, m142,m242,m342,m442,m542, m152,m252,
     & m352,m452,m552) +op3dCC4al1(i1,i2,i3,0, m113,m213,m313,m413,
     & m513, m123,m223,m323,m423,m523, m133,m233,m333,m433,m533, m143,
     & m243,m343,m443,m543, m153,m253,m353,m453,m553) +op3dCC4a(i1,i2,
     & i3,1, m114,m214,m314,m414,m514, m124,m224,m324,m424,m524, m134,
     & m234,m334,m434,m534, m144,m244,m344,m444,m544, m154,m254,m354,
     & m454,m554) +op3dCC4a(i1,i2,i3,2, m115,m215,m315,m415,m515, 
     & m125,m225,m325,m425,m525, m135,m235,m335,m435,m535, m145,m245,
     & m345,m445,m545, m155,m255,m355,m455,m555)
        op3dCC4l2(i1,i2,i3)= op3dCC4al2(i1,i2,i3,-2, m111,m211,m311,
     & m411,m511, m121,m221,m321,m421,m521, m131,m231,m331,m431,m531, 
     & m141,m241,m341,m441,m541, m151,m251,m351,m451,m551) +
     & op3dCC4al2(i1,i2,i3,-1, m112,m212,m312,m412,m512, m122,m222,
     & m322,m422,m522, m132,m232,m332,m432,m532, m142,m242,m342,m442,
     & m542, m152,m252,m352,m452,m552) +op3dCC4al2(i1,i2,i3,0, m113,
     & m213,m313,m413,m513, m123,m223,m323,m423,m523, m133,m233,m333,
     & m433,m533, m143,m243,m343,m443,m543, m153,m253,m353,m453,m553) 
     & +op3dCC4al2(i1,i2,i3,1, m114,m214,m314,m414,m514, m124,m224,
     & m324,m424,m524, m134,m234,m334,m434,m534, m144,m244,m344,m444,
     & m544, m154,m254,m354,m454,m554) +op3dCC4al2(i1,i2,i3,2, m115,
     & m215,m315,m415,m515, m125,m225,m325,m425,m525, m135,m235,m335,
     & m435,m535, m145,m245,m345,m445,m545, m155,m255,m355,m455,m555)
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
!  #If "3" == "2"
!  #Else
          ! ****************       
          ! ***** 3D *******       
          ! ****************       
!    #If "2" == "2"
            ! ****** 2nd order accurate ****
            m111=1
            m211=2
            m311=3
            m121=4
            m221=5
            m321=6
            m131=7
            m231=8
            m331=9
            m112=10
            m212=11
            m312=12
            m122=13
            m222=14
            m322=15
            m132=16
            m232=17
            m332=18
            m113=19
            m213=20
            m313=21
            m123=22
            m223=23
            m323=24
            m133=25
            m233=26
            m333=27
            if( sparseStencil.eq.sparse )then
! DEFECT(op3dSparse,3)
              if( lineSmoothOption .eq. -1 )then
c  general defect
! loops(op3dSparse)
                if( defectOption.eq.0 )then
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dSparse(i1,i2,i3)
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dSparse(i1,i2,i3)
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dSparse(i1,i2,i3)
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dSparse(i1,i2,i3)
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
! loops(op3dSparsel0)
                if( defectOption.eq.0 )then
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dSparsel0(i1,i2,
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dSparsel0(i1,i2,
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dSparsel0(i1,i2,
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dSparsel0(i1,i2,
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
! #If "3" ne "1"
              else if( lineSmoothOption .eq. 1 )then
c  defect for line smooth in direction 1
! loops(op3dSparsel1)
                if( defectOption.eq.0 )then
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dSparsel1(i1,i2,
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dSparsel1(i1,i2,
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dSparsel1(i1,i2,
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dSparsel1(i1,i2,
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
! #If "3" eq "3"
              else if( lineSmoothOption .eq. 2 )then
c  defect for line smooth in direction 2
! loops(op3dSparsel2)
                if( defectOption.eq.0 )then
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dSparsel2(i1,i2,
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dSparsel2(i1,i2,
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dSparsel2(i1,i2,
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dSparsel2(i1,i2,
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
              else
                write(*,*) 'defectOpt::ERROR invalid lineSmoothOption'
                write(*,*) 'defectOpt::nd="3"'
                write(*,*) 'defectOpt::lineSmoothOption=',
     & lineSmoothOption
                stop
              end if
            else if( sparseStencil.eq.general )then
              ! *** full stencil *****
! DEFECT(op3d,3)
              if( lineSmoothOption .eq. -1 )then
c  general defect
! loops(op3d)
                if( defectOption.eq.0 )then
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3d(i1,i2,i3)
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3d(i1,i2,i3)
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3d(i1,i2,i3)
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3d(i1,i2,i3)
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
! loops(op3dl0)
                if( defectOption.eq.0 )then
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dl0(i1,i2,i3)
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dl0(i1,i2,i3)
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dl0(i1,i2,i3)
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dl0(i1,i2,i3)
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
! #If "3" ne "1"
              else if( lineSmoothOption .eq. 1 )then
c  defect for line smooth in direction 1
! loops(op3dl1)
                if( defectOption.eq.0 )then
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dl1(i1,i2,i3)
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dl1(i1,i2,i3)
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dl1(i1,i2,i3)
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dl1(i1,i2,i3)
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
! #If "3" eq "3"
              else if( lineSmoothOption .eq. 2 )then
c  defect for line smooth in direction 2
! loops(op3dl2)
                if( defectOption.eq.0 )then
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dl2(i1,i2,i3)
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dl2(i1,i2,i3)
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dl2(i1,i2,i3)
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dl2(i1,i2,i3)
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
              else
                write(*,*) 'defectOpt::ERROR invalid lineSmoothOption'
                write(*,*) 'defectOpt::nd="3"'
                write(*,*) 'defectOpt::lineSmoothOption=',
     & lineSmoothOption
                stop
              end if
            else if( sparseStencil.eq.constantCoefficients )then
! DEFECT(op3dCC,3)
              if( lineSmoothOption .eq. -1 )then
c  general defect
! loops(op3dCC)
                if( defectOption.eq.0 )then
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dCC(i1,i2,i3)
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dCC(i1,i2,i3)
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dCC(i1,i2,i3)
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dCC(i1,i2,i3)
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
! loops(op3dCCl0)
                if( defectOption.eq.0 )then
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dCCl0(i1,i2,i3)
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dCCl0(i1,i2,i3)
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dCCl0(i1,i2,i3)
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dCCl0(i1,i2,i3)
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
! #If "3" ne "1"
              else if( lineSmoothOption .eq. 1 )then
c  defect for line smooth in direction 1
! loops(op3dCCl1)
                if( defectOption.eq.0 )then
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dCCl1(i1,i2,i3)
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dCCl1(i1,i2,i3)
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dCCl1(i1,i2,i3)
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dCCl1(i1,i2,i3)
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
! #If "3" eq "3"
              else if( lineSmoothOption .eq. 2 )then
c  defect for line smooth in direction 2
! loops(op3dCCl2)
                if( defectOption.eq.0 )then
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dCCl2(i1,i2,i3)
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dCCl2(i1,i2,i3)
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dCCl2(i1,i2,i3)
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dCCl2(i1,i2,i3)
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
              else
                write(*,*) 'defectOpt::ERROR invalid lineSmoothOption'
                write(*,*) 'defectOpt::nd="3"'
                write(*,*) 'defectOpt::lineSmoothOption=',
     & lineSmoothOption
                stop
              end if
            else if( sparseStencil.eq.sparseConstantCoefficients )then
! DEFECT(op3dSparseCC,3)
              if( lineSmoothOption .eq. -1 )then
c  general defect
! loops(op3dSparseCC)
                if( defectOption.eq.0 )then
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dSparseCC(i1,i2,
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dSparseCC(i1,i2,
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dSparseCC(i1,i2,
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dSparseCC(i1,i2,
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
! loops(op3dSparseCCl0)
                if( defectOption.eq.0 )then
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dSparseCCl0(i1,
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dSparseCCl0(i1,
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dSparseCCl0(i1,
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dSparseCCl0(i1,
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
! #If "3" ne "1"
              else if( lineSmoothOption .eq. 1 )then
c  defect for line smooth in direction 1
! loops(op3dSparseCCl1)
                if( defectOption.eq.0 )then
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dSparseCCl1(i1,
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dSparseCCl1(i1,
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dSparseCCl1(i1,
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dSparseCCl1(i1,
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
! #If "3" eq "3"
              else if( lineSmoothOption .eq. 2 )then
c  defect for line smooth in direction 2
! loops(op3dSparseCCl2)
                if( defectOption.eq.0 )then
                  do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 )then
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dSparseCCl2(i1,
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dSparseCCl2(i1,
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dSparseCCl2(i1,
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
                      defect(i1,i2,i3)=f(i1,i2,i3)-op3dSparseCCl2(i1,
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
              else
                write(*,*) 'defectOpt::ERROR invalid lineSmoothOption'
                write(*,*) 'defectOpt::nd="3"'
                write(*,*) 'defectOpt::lineSmoothOption=',
     & lineSmoothOption
                stop
              end if
            else if( sparseStencil.eq.sparseVariableCoefficients )then
! DEFECT3dVC(op3dSparseVC,3)
              if( lineSmoothOption .eq. -1 )then
c  general defect
! loops3dVC(op3dSparseVC)
                do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                    do i1=n1a,n1b,n1c
                      if( mask(i1,i2,i3).gt.0 )then
                        a1p=(s(i1,i2,i3)+s(i1+1,i2,i3))*dx2i
                        a1m=(s(i1,i2,i3)+s(i1-1,i2,i3))*dx2i
                        a2p=(s(i1,i2,i3)+s(i1,i2+1,i3))*dy2i
                        a2m=(s(i1,i2,i3)+s(i1,i2-1,i3))*dy2i
                        a3p=(s(i1,i2,i3)+s(i1,i2,i3+1))*dz2i
                        a3m=(s(i1,i2,i3)+s(i1,i2,i3-1))*dz2i
                        ad=-(a1p+a1m+a2p+a2m+a3p+a3m)
                        defect(i1,i2,i3)=f(i1,i2,i3)-op3dSparseVC(i1,
     & i2,i3)
                      else
                        defect(i1,i2,i3)=0.
                      end if
                    end do
                  end do
                end do
              else if( lineSmoothOption .eq. 0 )then
c  defect for line smooth in direction 0
! loops3dVC(op3dSparseVCl0)
                do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                    do i1=n1a,n1b,n1c
                      if( mask(i1,i2,i3).gt.0 )then
                        a1p=(s(i1,i2,i3)+s(i1+1,i2,i3))*dx2i
                        a1m=(s(i1,i2,i3)+s(i1-1,i2,i3))*dx2i
                        a2p=(s(i1,i2,i3)+s(i1,i2+1,i3))*dy2i
                        a2m=(s(i1,i2,i3)+s(i1,i2-1,i3))*dy2i
                        a3p=(s(i1,i2,i3)+s(i1,i2,i3+1))*dz2i
                        a3m=(s(i1,i2,i3)+s(i1,i2,i3-1))*dz2i
                        ad=-(a1p+a1m+a2p+a2m+a3p+a3m)
                        defect(i1,i2,i3)=f(i1,i2,i3)-op3dSparseVCl0(i1,
     & i2,i3)
                      else
                        defect(i1,i2,i3)=0.
                      end if
                    end do
                  end do
                end do
! #If "3" ne "1"
              else if( lineSmoothOption .eq. 1 )then
c  defect for line smooth in direction 1
! loops3dVC(op3dSparseVCl1)
                do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                    do i1=n1a,n1b,n1c
                      if( mask(i1,i2,i3).gt.0 )then
                        a1p=(s(i1,i2,i3)+s(i1+1,i2,i3))*dx2i
                        a1m=(s(i1,i2,i3)+s(i1-1,i2,i3))*dx2i
                        a2p=(s(i1,i2,i3)+s(i1,i2+1,i3))*dy2i
                        a2m=(s(i1,i2,i3)+s(i1,i2-1,i3))*dy2i
                        a3p=(s(i1,i2,i3)+s(i1,i2,i3+1))*dz2i
                        a3m=(s(i1,i2,i3)+s(i1,i2,i3-1))*dz2i
                        ad=-(a1p+a1m+a2p+a2m+a3p+a3m)
                        defect(i1,i2,i3)=f(i1,i2,i3)-op3dSparseVCl1(i1,
     & i2,i3)
                      else
                        defect(i1,i2,i3)=0.
                      end if
                    end do
                  end do
                end do
! #If "3" eq "3"
              else if( lineSmoothOption .eq. 2 )then
c  defect for line smooth in direction 2
! loops3dVC(op3dSparseVCl2)
                do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                    do i1=n1a,n1b,n1c
                      if( mask(i1,i2,i3).gt.0 )then
                        a1p=(s(i1,i2,i3)+s(i1+1,i2,i3))*dx2i
                        a1m=(s(i1,i2,i3)+s(i1-1,i2,i3))*dx2i
                        a2p=(s(i1,i2,i3)+s(i1,i2+1,i3))*dy2i
                        a2m=(s(i1,i2,i3)+s(i1,i2-1,i3))*dy2i
                        a3p=(s(i1,i2,i3)+s(i1,i2,i3+1))*dz2i
                        a3m=(s(i1,i2,i3)+s(i1,i2,i3-1))*dz2i
                        ad=-(a1p+a1m+a2p+a2m+a3p+a3m)
                        defect(i1,i2,i3)=f(i1,i2,i3)-op3dSparseVCl2(i1,
     & i2,i3)
                      else
                        defect(i1,i2,i3)=0.
                      end if
                    end do
                  end do
                end do
              else
                write(*,*) 'defectOpt::ERROR invalid lineSmoothOption'
                stop
              end if
            else if( sparseStencil.eq.variableCoefficients )then
              ! use sparse version for now:
! DEFECT3dVC(op3dSparseVC,3)
              if( lineSmoothOption .eq. -1 )then
c  general defect
! loops3dVC(op3dSparseVC)
                do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                    do i1=n1a,n1b,n1c
                      if( mask(i1,i2,i3).gt.0 )then
                        a1p=(s(i1,i2,i3)+s(i1+1,i2,i3))*dx2i
                        a1m=(s(i1,i2,i3)+s(i1-1,i2,i3))*dx2i
                        a2p=(s(i1,i2,i3)+s(i1,i2+1,i3))*dy2i
                        a2m=(s(i1,i2,i3)+s(i1,i2-1,i3))*dy2i
                        a3p=(s(i1,i2,i3)+s(i1,i2,i3+1))*dz2i
                        a3m=(s(i1,i2,i3)+s(i1,i2,i3-1))*dz2i
                        ad=-(a1p+a1m+a2p+a2m+a3p+a3m)
                        defect(i1,i2,i3)=f(i1,i2,i3)-op3dSparseVC(i1,
     & i2,i3)
                      else
                        defect(i1,i2,i3)=0.
                      end if
                    end do
                  end do
                end do
              else if( lineSmoothOption .eq. 0 )then
c  defect for line smooth in direction 0
! loops3dVC(op3dSparseVCl0)
                do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                    do i1=n1a,n1b,n1c
                      if( mask(i1,i2,i3).gt.0 )then
                        a1p=(s(i1,i2,i3)+s(i1+1,i2,i3))*dx2i
                        a1m=(s(i1,i2,i3)+s(i1-1,i2,i3))*dx2i
                        a2p=(s(i1,i2,i3)+s(i1,i2+1,i3))*dy2i
                        a2m=(s(i1,i2,i3)+s(i1,i2-1,i3))*dy2i
                        a3p=(s(i1,i2,i3)+s(i1,i2,i3+1))*dz2i
                        a3m=(s(i1,i2,i3)+s(i1,i2,i3-1))*dz2i
                        ad=-(a1p+a1m+a2p+a2m+a3p+a3m)
                        defect(i1,i2,i3)=f(i1,i2,i3)-op3dSparseVCl0(i1,
     & i2,i3)
                      else
                        defect(i1,i2,i3)=0.
                      end if
                    end do
                  end do
                end do
! #If "3" ne "1"
              else if( lineSmoothOption .eq. 1 )then
c  defect for line smooth in direction 1
! loops3dVC(op3dSparseVCl1)
                do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                    do i1=n1a,n1b,n1c
                      if( mask(i1,i2,i3).gt.0 )then
                        a1p=(s(i1,i2,i3)+s(i1+1,i2,i3))*dx2i
                        a1m=(s(i1,i2,i3)+s(i1-1,i2,i3))*dx2i
                        a2p=(s(i1,i2,i3)+s(i1,i2+1,i3))*dy2i
                        a2m=(s(i1,i2,i3)+s(i1,i2-1,i3))*dy2i
                        a3p=(s(i1,i2,i3)+s(i1,i2,i3+1))*dz2i
                        a3m=(s(i1,i2,i3)+s(i1,i2,i3-1))*dz2i
                        ad=-(a1p+a1m+a2p+a2m+a3p+a3m)
                        defect(i1,i2,i3)=f(i1,i2,i3)-op3dSparseVCl1(i1,
     & i2,i3)
                      else
                        defect(i1,i2,i3)=0.
                      end if
                    end do
                  end do
                end do
! #If "3" eq "3"
              else if( lineSmoothOption .eq. 2 )then
c  defect for line smooth in direction 2
! loops3dVC(op3dSparseVCl2)
                do i3=n3a,n3b,n3c
                  do i2=n2a,n2b,n2c
                    do i1=n1a,n1b,n1c
                      if( mask(i1,i2,i3).gt.0 )then
                        a1p=(s(i1,i2,i3)+s(i1+1,i2,i3))*dx2i
                        a1m=(s(i1,i2,i3)+s(i1-1,i2,i3))*dx2i
                        a2p=(s(i1,i2,i3)+s(i1,i2+1,i3))*dy2i
                        a2m=(s(i1,i2,i3)+s(i1,i2-1,i3))*dy2i
                        a3p=(s(i1,i2,i3)+s(i1,i2,i3+1))*dz2i
                        a3m=(s(i1,i2,i3)+s(i1,i2,i3-1))*dz2i
                        ad=-(a1p+a1m+a2p+a2m+a3p+a3m)
                        defect(i1,i2,i3)=f(i1,i2,i3)-op3dSparseVCl2(i1,
     & i2,i3)
                      else
                        defect(i1,i2,i3)=0.
                      end if
                    end do
                  end do
                end do
              else
                write(*,*) 'defectOpt::ERROR invalid lineSmoothOption'
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
