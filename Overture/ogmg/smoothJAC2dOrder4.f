! This file automatically generated from smoothOpt.bf with bpp.
! SMOOTH_JACOBI_SUBROUTINE(smoothJAC2dOrder4,2,4)
        subroutine smoothJAC2dOrder4( nd, nd1a,nd1b,nd2a,nd2b,nd3a,
     & nd3b, n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c, ndc, f, c, u, v, 
     & mask, option, order, sparseStencil, cc, s, dx, omega0, bc,  np,
     &  ndip,ip, ipar )
c ===================================================================================
c  Optimised Jacobi and Gauss-Seidel
c
c  option: 0 : jacobi (solution returned in u, v is a temporary space)
c          1 : Gauss-Seidel
c          2 : jacobi on boundaries where bc(side,axis)>0 (solution returned in u, v is a temporary space)
c          3 : Gauss-Seidel on boundaries where bc(side,axis)>0 (solution returned in u, v is a temporary space)
c          4 : Gauss-Seidel on a list of points (ip)
c
c  cc(m) : constant coefficients
c  sparseStencil : general=0, sparse=1, constantCoefficients=2, sparseConstantCoefficients=3
c
c  ip(ndip,0:nd) : (ip(i,0:nd-1), i=1,...,np) : list of points for option=4
c ===================================================================================
        implicit none
        integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, n1a,n1b,n1c,n2a,n2b,
     & n2c,n3a,n3b,n3c, ndc, option, sparseStencil,order
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
        integer m11,m12,m13,m14,m15, m21,m22,m23,m24,m25, m31,m32,m33,
     & m34,m35, m41,m42,m43,m44,m45, m51,m52,m53,m54,m55
        integer    m111,m211,m311,m411,m511, m121,m221,m321,m421,m521, 
     & m131,m231,m331,m431,m531, m141,m241,m341,m441,m541, m151,m251,
     & m351,m451,m551, m112,m212,m312,m412,m512, m122,m222,m322,m422,
     & m522, m132,m232,m332,m432,m532, m142,m242,m342,m442,m542, m152,
     & m252,m352,m452,m552, m113,m213,m313,m413,m513, m123,m223,m323,
     & m423,m523, m133,m233,m333,m433,m533, m143,m243,m343,m443,m543, 
     & m153,m253,m353,m453,m553, m114,m214,m314,m414,m514, m124,m224,
     & m324,m424,m524, m134,m234,m334,m434,m534, m144,m244,m344,m444,
     & m544, m154,m254,m354,m454,m554, m115,m215,m315,m415,m515, m125,
     & m225,m325,m425,m525, m135,m235,m335,m435,m535, m145,m245,m345,
     & m445,m545, m155,m255,m355,m455,m555
        integer    m11n,m21n,m31n,m41n,m51n, m12n,m22n,m32n,m42n,m52n, 
     & m13n,m23n,m33n,m43n,m53n, m14n,m24n,m34n,m44n,m54n, m15n,m25n,
     & m35n,m45n,m55n
        real eps
        integer general, sparse, constantCoefficients,  
     & sparseConstantCoefficients,sparseVariableCoefficients, 
     & variableCoefficients
        parameter( general=0,  sparse=1,  constantCoefficients=2, 
     & sparseConstantCoefficients=3, sparseVariableCoefficients=4, 
     & variableCoefficients=5 )
        !   *** statement functions ***
        real update2dSparse,update2d,update3dSparse,update3d
        real update2dSparseCC,update2dCC,update3dSparseCC,update3dCC
        real update2dSparseVC,update2dVC,update3dSparseVC, update3dVC
        real update2dSparse4,update2d4,update3dSparse4,update3d4, 
     & update3d4a
        real update2dSparseCC4,update2dCC4,update3dSparseCC4,
     & update3dCC4, update3dCC4a
        real a1,a2,a3,a22,a222,a12
        real a1m,a1p,a2m,a2p,a3m,a3p,ad
        real dx2i,dy2i,dz2i
        ! ===========  2nd order ===========================
!  #If "2" == "2"
        update2dSparse(i1,i2,i3)=u(i1,i2,i3) + omega*(f(i1,i2,i3)-(    
     &     c(m22,i1,i2,i3)*u(i1  ,i2  ,i3)+ c(m32,i1,i2,i3)*u(i1+1,i2 
     &  ,i3)+ c(m23,i1,i2,i3)*u(i1  ,i2+1,i3)+ c(m12,i1,i2,i3)*u(i1-1,
     & i2  ,i3)+ c(m21,i1,i2,i3)*u(i1  ,i2-1,i3) ))/(c(m22,i1,i2,i3)+
     & eps)
        update2d(i1,i2,i3)=u(i1,i2,i3) +  omega*(f(i1,i2,i3)-(        
     & c(m11,i1,i2,i3)*u(i1-1,i2-1,i3)+ c(m21,i1,i2,i3)*u(i1  ,i2-1,
     & i3)+ c(m31,i1,i2,i3)*u(i1+1,i2-1,i3)+ c(m12,i1,i2,i3)*u(i1-1,
     & i2  ,i3)+ c(m22,i1,i2,i3)*u(i1  ,i2  ,i3)+ c(m32,i1,i2,i3)*u(
     & i1+1,i2  ,i3)+ c(m13,i1,i2,i3)*u(i1-1,i2+1,i3)+ c(m23,i1,i2,i3)
     & *u(i1  ,i2+1,i3)+ c(m33,i1,i2,i3)*u(i1+1,i2+1,i3) ))/(c(m22,i1,
     & i2,i3)+eps)
! #If "2" == "3"
! #If "2" == "2"
        !  --------- const coefficients versions ----------------
        update2dSparseCC(i1,i2,i3)=u(i1,i2,i3) + omega*(f(i1,i2,i3)-(  
     &       cc(m22)*u(i1  ,i2  ,i3)+ cc(m32)*u(i1+1,i2  ,i3)+ cc(m23)
     & *u(i1  ,i2+1,i3)+ cc(m12)*u(i1-1,i2  ,i3)+ cc(m21)*u(i1  ,i2-1,
     & i3) ))/(cc(m22))
        update2dCC(i1,i2,i3)=u(i1,i2,i3) +  omega*(f(i1,i2,i3)-(       
     &  cc(m11)*u(i1-1,i2-1,i3)+ cc(m21)*u(i1  ,i2-1,i3)+ cc(m31)*u(
     & i1+1,i2-1,i3)+ cc(m12)*u(i1-1,i2  ,i3)+ cc(m22)*u(i1  ,i2  ,i3)
     & + cc(m32)*u(i1+1,i2  ,i3)+ cc(m13)*u(i1-1,i2+1,i3)+ cc(m23)*u(
     & i1  ,i2+1,i3)+ cc(m33)*u(i1+1,i2+1,i3) ))/(cc(m22))
! #If "2" == "3"
! #If "2" == "2"
        !  ===========  4th order ===========================
        update2dSparse4(i1,i2,i3)=u(i1,i2,i3) + omega*(f(i1,i2,i3)-(   
     &      c(m31,i1,i2,i3)*u(i1  ,i2-2,i3)+ c(m32,i1,i2,i3)*u(i1  ,
     & i2-1,i3)+ c(m13,i1,i2,i3)*u(i1-2,i2  ,i3)+ c(m23,i1,i2,i3)*u(
     & i1-1,i2  ,i3)+ c(m33,i1,i2,i3)*u(i1  ,i2  ,i3)+ c(m43,i1,i2,i3)
     & *u(i1+1,i2  ,i3)+ c(m53,i1,i2,i3)*u(i1+2,i2  ,i3)+ c(m34,i1,i2,
     & i3)*u(i1  ,i2+1,i3)+ c(m35,i1,i2,i3)*u(i1  ,i2+2,i3) ))/(c(m33,
     & i1,i2,i3)+eps)
        update2d4(i1,i2,i3)=u(i1,i2,i3) +  omega*(f(i1,i2,i3)-(        
     & c(m11,i1,i2,i3)*u(i1-2,i2-2,i3)+ c(m21,i1,i2,i3)*u(i1-1,i2-2,
     & i3)+ c(m31,i1,i2,i3)*u(i1  ,i2-2,i3)+ c(m41,i1,i2,i3)*u(i1+1,
     & i2-2,i3)+ c(m51,i1,i2,i3)*u(i1+2,i2-2,i3)+ c(m12,i1,i2,i3)*u(
     & i1-2,i2-1,i3)+ c(m22,i1,i2,i3)*u(i1-1,i2-1,i3)+ c(m32,i1,i2,i3)
     & *u(i1  ,i2-1,i3)+ c(m42,i1,i2,i3)*u(i1+1,i2-1,i3)+ c(m52,i1,i2,
     & i3)*u(i1+2,i2-1,i3)+ c(m13,i1,i2,i3)*u(i1-2,i2  ,i3)+ c(m23,i1,
     & i2,i3)*u(i1-1,i2  ,i3)+ c(m33,i1,i2,i3)*u(i1  ,i2  ,i3)+ c(m43,
     & i1,i2,i3)*u(i1+1,i2  ,i3)+ c(m53,i1,i2,i3)*u(i1+2,i2  ,i3)+ c(
     & m14,i1,i2,i3)*u(i1-2,i2+1,i3)+ c(m24,i1,i2,i3)*u(i1-1,i2+1,i3)+
     &  c(m34,i1,i2,i3)*u(i1  ,i2+1,i3)+ c(m44,i1,i2,i3)*u(i1+1,i2+1,
     & i3)+ c(m54,i1,i2,i3)*u(i1+2,i2+1,i3)+ c(m15,i1,i2,i3)*u(i1-2,
     & i2+2,i3)+ c(m25,i1,i2,i3)*u(i1-1,i2+2,i3)+ c(m35,i1,i2,i3)*u(
     & i1  ,i2+2,i3)+ c(m45,i1,i2,i3)*u(i1+1,i2+2,i3)+ c(m55,i1,i2,i3)
     & *u(i1+2,i2+2,i3) ))/(c(m33,i1,i2,i3)+eps)
! #If "2" == "3"
! #If "2" == "2"
        !  --------- const coefficients versions ----------------
        update2dSparseCC4(i1,i2,i3)=u(i1,i2,i3) + omega*(f(i1,i2,i3)-( 
     &        cc(m31)*u(i1  ,i2-2,i3)+ cc(m32)*u(i1  ,i2-1,i3)+ cc(
     & m13)*u(i1-2,i2  ,i3)+ cc(m23)*u(i1-1,i2  ,i3)+ cc(m33)*u(i1  ,
     & i2  ,i3)+ cc(m43)*u(i1+1,i2  ,i3)+ cc(m53)*u(i1+2,i2  ,i3)+ cc(
     & m34)*u(i1  ,i2+1,i3)+ cc(m35)*u(i1  ,i2+2,i3) ))/(cc(m33)+eps)
        update2dCC4(i1,i2,i3)=u(i1,i2,i3) +  omega*(f(i1,i2,i3)-(      
     &   cc(m11)*u(i1-2,i2-2,i3)+ cc(m21)*u(i1-1,i2-2,i3)+ cc(m31)*u(
     & i1  ,i2-2,i3)+ cc(m41)*u(i1+1,i2-2,i3)+ cc(m51)*u(i1+2,i2-2,i3)
     & + cc(m12)*u(i1-2,i2-1,i3)+ cc(m22)*u(i1-1,i2-1,i3)+ cc(m32)*u(
     & i1  ,i2-1,i3)+ cc(m42)*u(i1+1,i2-1,i3)+ cc(m52)*u(i1+2,i2-1,i3)
     & + cc(m13)*u(i1-2,i2  ,i3)+ cc(m23)*u(i1-1,i2  ,i3)+ cc(m33)*u(
     & i1  ,i2  ,i3)+ cc(m43)*u(i1+1,i2  ,i3)+ cc(m53)*u(i1+2,i2  ,i3)
     & + cc(m14)*u(i1-2,i2+1,i3)+ cc(m24)*u(i1-1,i2+1,i3)+ cc(m34)*u(
     & i1  ,i2+1,i3)+ cc(m44)*u(i1+1,i2+1,i3)+ cc(m54)*u(i1+2,i2+1,i3)
     & + cc(m15)*u(i1-2,i2+2,i3)+ cc(m25)*u(i1-1,i2+2,i3)+ cc(m35)*u(
     & i1  ,i2+2,i3)+ cc(m45)*u(i1+1,i2+2,i3)+ cc(m55)*u(i1+2,i2+2,i3)
     &  ))/(cc(m33)+eps)
! #If "2" == "3"
! #If "2" == "2"
        ! ===========  div( s grad ) ===========================
        update2dSparseVC(i1,i2,i3)=u(i1,i2,i3) + omega*(f(i1,i2,i3)-(  
     &       a2m*u(i1  ,i2-1,i3)+ a1m*u(i1-1,i2  ,i3)+ ad *u(i1  ,i2  
     & ,i3)+ a1p*u(i1+1,i2  ,i3)+ a2p*u(i1  ,i2+1,i3) ))/(ad)
        update3dSparseVC(i1,i2,i3) = u(i1,i2,i3) + omega*(f(i1,i2,i3)-(
     &         a3m*u(i1  ,i2  ,i3-1)+ a2m*u(i1  ,i2-1,i3  )+ a1m*u(i1-
     & 1,i2  ,i3  )+ ad*u(i1  ,i2  ,i3  )+ a1p*u(i1+1,i2  ,i3  )+ a2p*
     & u(i1  ,i2+1,i3  )+ a3p*u(i1  ,i2  ,i3+1) ))/(ad)
c   *** end statement functions
        boundaryLayers=ipar(0) ! number of boundary layers to smooth
        nb=boundaryLayers-1  ! number of extra boundary lines to smooth
        eps=1.e-30 ! *****
        if( order.ne.2 .and. order.ne.4 )then
          write(*,*) 'smoothOpt:ERROR: invalid order=',order
          stop 1
        end if
        dx2i=.5/dx(1)**2
        dy2i=.5/dx(2)**2
        dz2i=.5/dx(3)**2
        if( omega0.gt.0. )then
          omega=omega0
        else if( nd.eq.2 )then
          omega=4./5.
        else
          omega=6./7.
        end if
!  #If "2" == "2"
!    #If "4" == "2"
!    #Elif "4" == "4"
           ! 4th order
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
            if( sparseStencil.eq.sparse )then
        !             Here we can assume that the operator is a 9-point 4th-order operator 
! updateLoops(update2dSparse4)
c write(*,*) 'n1a..',n1a,n1b,n1c,n1d,n2a,n2b,n2c
             if( option.eq.0 )then
               ! Jacobi
               do i3=n3a,n3b,n3c
               do i2=n2a,n2b,n2c
               do i1=n1a,n1b,n1c
                 if( mask(i1,i2,i3).gt.0 )then
                   v(i1,i2,i3)=update2dSparse4(i1,i2,i3) ! update2dSparse4 points R1 or B1
                 end if
               end do
               end do
               end do
             else if( option.eq.1 )then
               ! Gauss-Seidel
               do i3=n3a,n3b,n3c
               do i2=n2a,n2b,n2c
               do i1=n1a,n1b,n1c
                 if( mask(i1,i2,i3).gt.0 )then
                   u(i1,i2,i3)=update2dSparse4(i1,i2,i3) ! update2dSparse4 points R1 or B1
                 end if
               end do
               end do
               end do
             else if( option.eq.2 .or. option.eq.3 )then
! updateBoundaryLoops(v,u,update2dSparse4)
c write(*,*) 'n1a..',n1a,n1b,n1c,n1d,n2a,n2b,n2c
                 if( bc(0,0).gt.0 )then
! boundaryLoop(v,u,update2dSparse4,n1a,n1a+nb,n2a,n2b,n3a,n3b)
                   if( option.eq.2 )then
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1a+nb
                       if( mask(i1,i2,i3).gt.0 )then
                         v(i1,i2,i3)=update2dSparse4(i1,i2,i3)
                       end if
                     end do
                     end do
                     end do
                   else
                     ! Gauss-Seidel
                     ! write(*,*) 'bl:',n1a,n1a+nb,n2a,n2b,n3a,n3b
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1a+nb
                       if( mask(i1,i2,i3).gt.0 )then
                         u(i1,i2,i3)=update2dSparse4(i1,i2,i3)
                         ! write(*,*) 'u,f=',u(i1,i2,i3),f(i1,i2,i3)
                       end if
                     end do
                     end do
                     end do
                   end if
                 end if
                 if( bc(1,0).gt.0 )then
! boundaryLoop(v,u,update2dSparse4,n1b-nb,n1b,n2a,n2b,n3a,n3b)
                   if( option.eq.2 )then
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1b-nb,n1b
                       if( mask(i1,i2,i3).gt.0 )then
                         v(i1,i2,i3)=update2dSparse4(i1,i2,i3)
                       end if
                     end do
                     end do
                     end do
                   else
                     ! Gauss-Seidel
                     ! write(*,*) 'bl:',n1b-nb,n1b,n2a,n2b,n3a,n3b
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1b-nb,n1b
                       if( mask(i1,i2,i3).gt.0 )then
                         u(i1,i2,i3)=update2dSparse4(i1,i2,i3)
                         ! write(*,*) 'u,f=',u(i1,i2,i3),f(i1,i2,i3)
                       end if
                     end do
                     end do
                     end do
                   end if
                 end if
                 if( bc(0,1).gt.0 )then
! boundaryLoop(v,u,update2dSparse4,n1a,n1b,n2a,n2a+nb,n3a,n3b)
                   if( option.eq.2 )then
                     do i3=n3a,n3b
                     do i2=n2a,n2a+nb
                     do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
                         v(i1,i2,i3)=update2dSparse4(i1,i2,i3)
                       end if
                     end do
                     end do
                     end do
                   else
                     ! Gauss-Seidel
                     ! write(*,*) 'bl:',n1a,n1b,n2a,n2a+nb,n3a,n3b
                     do i3=n3a,n3b
                     do i2=n2a,n2a+nb
                     do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
                         u(i1,i2,i3)=update2dSparse4(i1,i2,i3)
                         ! write(*,*) 'u,f=',u(i1,i2,i3),f(i1,i2,i3)
                       end if
                     end do
                     end do
                     end do
                   end if
                 end if
                 if( bc(1,1).gt.0 )then
! boundaryLoop(v,u,update2dSparse4,n1a,n1b,n2b-nb,n2b,n3a,n3b)
                   if( option.eq.2 )then
                     do i3=n3a,n3b
                     do i2=n2b-nb,n2b
                     do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
                         v(i1,i2,i3)=update2dSparse4(i1,i2,i3)
                       end if
                     end do
                     end do
                     end do
                   else
                     ! Gauss-Seidel
                     ! write(*,*) 'bl:',n1a,n1b,n2b-nb,n2b,n3a,n3b
                     do i3=n3a,n3b
                     do i2=n2b-nb,n2b
                     do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
                         u(i1,i2,i3)=update2dSparse4(i1,i2,i3)
                         ! write(*,*) 'u,f=',u(i1,i2,i3),f(i1,i2,i3)
                       end if
                     end do
                     end do
                     end do
                   end if
                 end if
                 if( nd.eq.3 .and. bc(0,2).gt.0 )then
! boundaryLoop(v,u,update2dSparse4,n1a,n1b,n2a,n2b,n3a,n3a+nb)
                   if( option.eq.2 )then
                     do i3=n3a,n3a+nb
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
                         v(i1,i2,i3)=update2dSparse4(i1,i2,i3)
                       end if
                     end do
                     end do
                     end do
                   else
                     ! Gauss-Seidel
                     ! write(*,*) 'bl:',n1a,n1b,n2a,n2b,n3a,n3a+nb
                     do i3=n3a,n3a+nb
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
                         u(i1,i2,i3)=update2dSparse4(i1,i2,i3)
                         ! write(*,*) 'u,f=',u(i1,i2,i3),f(i1,i2,i3)
                       end if
                     end do
                     end do
                     end do
                   end if
                 end if
                 if( nd.eq.3 .and. bc(1,2).gt.0 )then
! boundaryLoop(v,u,update2dSparse4,n1a,n1b,n2a,n2b,n3b-nb,n3b)
                   if( option.eq.2 )then
                     do i3=n3b-nb,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
                         v(i1,i2,i3)=update2dSparse4(i1,i2,i3)
                       end if
                     end do
                     end do
                     end do
                   else
                     ! Gauss-Seidel
                     ! write(*,*) 'bl:',n1a,n1b,n2a,n2b,n3b-nb,n3b
                     do i3=n3b-nb,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
                         u(i1,i2,i3)=update2dSparse4(i1,i2,i3)
                         ! write(*,*) 'u,f=',u(i1,i2,i3),f(i1,i2,i3)
                       end if
                     end do
                     end do
                     end do
                   end if
                 end if
             else if( option.eq.4 )then
! updateListOfPoints(update2dSparse4)
                ! write(*,*) 'updateListOfPoints...'
                if( nd.eq.2 )then
                 i3=0
                 do i=1,np
                   i1=ip(i,0)
                   i2=ip(i,1)
                   if( mask(i1,i2,i3).gt.0 )then
                     u(i1,i2,i3)=update2dSparse4(i1,i2,i3)
                   end if
                 end do
                else
                 do i=1,np
                   i1=ip(i,0)
                   i2=ip(i,1)
                   i3=ip(i,2)
                   if( mask(i1,i2,i3).gt.0 )then
                     u(i1,i2,i3)=update2dSparse4(i1,i2,i3)
                   end if
                 end do
                end if
             else if( option.eq.5 )then
! updateListOfPointsJacobi(update2dSparse4)
                ! write(*,*) 'updateListOfPointsJacobi...'
                if( nd.eq.2 )then
                 i3=0
                 do i=1,np
                   i1=ip(i,0)
                   i2=ip(i,1)
                   if( mask(i1,i2,i3).gt.0 )then
                     v(i1,i2,i3)=update2dSparse4(i1,i2,i3)
                   end if
                 end do
                 do i=1,np
                   i1=ip(i,0)
                   i2=ip(i,1)
                   if( mask(i1,i2,i3).gt.0 )then
                     u(i1,i2,i3)=v(i1,i2,i3)
                   end if
                 end do
                else
                 do i=1,np
                   i1=ip(i,0)
                   i2=ip(i,1)
                   i3=ip(i,2)
                   if( mask(i1,i2,i3).gt.0 )then
                     v(i1,i2,i3)=update2dSparse4(i1,i2,i3)
                   end if
                 end do
                 do i=1,np
                   i1=ip(i,0)
                   i2=ip(i,1)
                   i3=ip(i,2)
                   if( mask(i1,i2,i3).gt.0 )then
                     u(i1,i2,i3)=v(i1,i2,i3)
                   end if
                 end do
                end if
             else
               write(*,*) 'ERROR: invalid option=',option
               stop 23
             end if
            else if( sparseStencil.eq.sparseConstantCoefficients )then
! updateLoops(update2dSparseCC4)
c write(*,*) 'n1a..',n1a,n1b,n1c,n1d,n2a,n2b,n2c
              if( option.eq.0 )then
                ! Jacobi
                do i3=n3a,n3b,n3c
                do i2=n2a,n2b,n2c
                do i1=n1a,n1b,n1c
                  if( mask(i1,i2,i3).gt.0 )then
                    v(i1,i2,i3)=update2dSparseCC4(i1,i2,i3) ! update2dSparseCC4 points R1 or B1
                  end if
                end do
                end do
                end do
              else if( option.eq.1 )then
                ! Gauss-Seidel
                do i3=n3a,n3b,n3c
                do i2=n2a,n2b,n2c
                do i1=n1a,n1b,n1c
                  if( mask(i1,i2,i3).gt.0 )then
                    u(i1,i2,i3)=update2dSparseCC4(i1,i2,i3) ! update2dSparseCC4 points R1 or B1
                  end if
                end do
                end do
                end do
              else if( option.eq.2 .or. option.eq.3 )then
! updateBoundaryLoops(v,u,update2dSparseCC4)
c write(*,*) 'n1a..',n1a,n1b,n1c,n1d,n2a,n2b,n2c
                  if( bc(0,0).gt.0 )then
! boundaryLoop(v,u,update2dSparseCC4,n1a,n1a+nb,n2a,n2b,n3a,n3b)
                    if( option.eq.2 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1a+nb
                        if( mask(i1,i2,i3).gt.0 )then
                          v(i1,i2,i3)=update2dSparseCC4(i1,i2,i3)
                        end if
                      end do
                      end do
                      end do
                    else
                      ! Gauss-Seidel
                      ! write(*,*) 'bl:',n1a,n1a+nb,n2a,n2b,n3a,n3b
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1a+nb
                        if( mask(i1,i2,i3).gt.0 )then
                          u(i1,i2,i3)=update2dSparseCC4(i1,i2,i3)
                          ! write(*,*) 'u,f=',u(i1,i2,i3),f(i1,i2,i3)
                        end if
                      end do
                      end do
                      end do
                    end if
                  end if
                  if( bc(1,0).gt.0 )then
! boundaryLoop(v,u,update2dSparseCC4,n1b-nb,n1b,n2a,n2b,n3a,n3b)
                    if( option.eq.2 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1b-nb,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          v(i1,i2,i3)=update2dSparseCC4(i1,i2,i3)
                        end if
                      end do
                      end do
                      end do
                    else
                      ! Gauss-Seidel
                      ! write(*,*) 'bl:',n1b-nb,n1b,n2a,n2b,n3a,n3b
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1b-nb,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          u(i1,i2,i3)=update2dSparseCC4(i1,i2,i3)
                          ! write(*,*) 'u,f=',u(i1,i2,i3),f(i1,i2,i3)
                        end if
                      end do
                      end do
                      end do
                    end if
                  end if
                  if( bc(0,1).gt.0 )then
! boundaryLoop(v,u,update2dSparseCC4,n1a,n1b,n2a,n2a+nb,n3a,n3b)
                    if( option.eq.2 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2a+nb
                      do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          v(i1,i2,i3)=update2dSparseCC4(i1,i2,i3)
                        end if
                      end do
                      end do
                      end do
                    else
                      ! Gauss-Seidel
                      ! write(*,*) 'bl:',n1a,n1b,n2a,n2a+nb,n3a,n3b
                      do i3=n3a,n3b
                      do i2=n2a,n2a+nb
                      do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          u(i1,i2,i3)=update2dSparseCC4(i1,i2,i3)
                          ! write(*,*) 'u,f=',u(i1,i2,i3),f(i1,i2,i3)
                        end if
                      end do
                      end do
                      end do
                    end if
                  end if
                  if( bc(1,1).gt.0 )then
! boundaryLoop(v,u,update2dSparseCC4,n1a,n1b,n2b-nb,n2b,n3a,n3b)
                    if( option.eq.2 )then
                      do i3=n3a,n3b
                      do i2=n2b-nb,n2b
                      do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          v(i1,i2,i3)=update2dSparseCC4(i1,i2,i3)
                        end if
                      end do
                      end do
                      end do
                    else
                      ! Gauss-Seidel
                      ! write(*,*) 'bl:',n1a,n1b,n2b-nb,n2b,n3a,n3b
                      do i3=n3a,n3b
                      do i2=n2b-nb,n2b
                      do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          u(i1,i2,i3)=update2dSparseCC4(i1,i2,i3)
                          ! write(*,*) 'u,f=',u(i1,i2,i3),f(i1,i2,i3)
                        end if
                      end do
                      end do
                      end do
                    end if
                  end if
                  if( nd.eq.3 .and. bc(0,2).gt.0 )then
! boundaryLoop(v,u,update2dSparseCC4,n1a,n1b,n2a,n2b,n3a,n3a+nb)
                    if( option.eq.2 )then
                      do i3=n3a,n3a+nb
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          v(i1,i2,i3)=update2dSparseCC4(i1,i2,i3)
                        end if
                      end do
                      end do
                      end do
                    else
                      ! Gauss-Seidel
                      ! write(*,*) 'bl:',n1a,n1b,n2a,n2b,n3a,n3a+nb
                      do i3=n3a,n3a+nb
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          u(i1,i2,i3)=update2dSparseCC4(i1,i2,i3)
                          ! write(*,*) 'u,f=',u(i1,i2,i3),f(i1,i2,i3)
                        end if
                      end do
                      end do
                      end do
                    end if
                  end if
                  if( nd.eq.3 .and. bc(1,2).gt.0 )then
! boundaryLoop(v,u,update2dSparseCC4,n1a,n1b,n2a,n2b,n3b-nb,n3b)
                    if( option.eq.2 )then
                      do i3=n3b-nb,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          v(i1,i2,i3)=update2dSparseCC4(i1,i2,i3)
                        end if
                      end do
                      end do
                      end do
                    else
                      ! Gauss-Seidel
                      ! write(*,*) 'bl:',n1a,n1b,n2a,n2b,n3b-nb,n3b
                      do i3=n3b-nb,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          u(i1,i2,i3)=update2dSparseCC4(i1,i2,i3)
                          ! write(*,*) 'u,f=',u(i1,i2,i3),f(i1,i2,i3)
                        end if
                      end do
                      end do
                      end do
                    end if
                  end if
              else if( option.eq.4 )then
! updateListOfPoints(update2dSparseCC4)
                 ! write(*,*) 'updateListOfPoints...'
                 if( nd.eq.2 )then
                  i3=0
                  do i=1,np
                    i1=ip(i,0)
                    i2=ip(i,1)
                    if( mask(i1,i2,i3).gt.0 )then
                      u(i1,i2,i3)=update2dSparseCC4(i1,i2,i3)
                    end if
                  end do
                 else
                  do i=1,np
                    i1=ip(i,0)
                    i2=ip(i,1)
                    i3=ip(i,2)
                    if( mask(i1,i2,i3).gt.0 )then
                      u(i1,i2,i3)=update2dSparseCC4(i1,i2,i3)
                    end if
                  end do
                 end if
              else if( option.eq.5 )then
! updateListOfPointsJacobi(update2dSparseCC4)
                 ! write(*,*) 'updateListOfPointsJacobi...'
                 if( nd.eq.2 )then
                  i3=0
                  do i=1,np
                    i1=ip(i,0)
                    i2=ip(i,1)
                    if( mask(i1,i2,i3).gt.0 )then
                      v(i1,i2,i3)=update2dSparseCC4(i1,i2,i3)
                    end if
                  end do
                  do i=1,np
                    i1=ip(i,0)
                    i2=ip(i,1)
                    if( mask(i1,i2,i3).gt.0 )then
                      u(i1,i2,i3)=v(i1,i2,i3)
                    end if
                  end do
                 else
                  do i=1,np
                    i1=ip(i,0)
                    i2=ip(i,1)
                    i3=ip(i,2)
                    if( mask(i1,i2,i3).gt.0 )then
                      v(i1,i2,i3)=update2dSparseCC4(i1,i2,i3)
                    end if
                  end do
                  do i=1,np
                    i1=ip(i,0)
                    i2=ip(i,1)
                    i3=ip(i,2)
                    if( mask(i1,i2,i3).gt.0 )then
                      u(i1,i2,i3)=v(i1,i2,i3)
                    end if
                  end do
                 end if
              else
                write(*,*) 'ERROR: invalid option=',option
                stop 23
              end if
            else if( sparseStencil.eq.general )then
        !      **** full stencil *****
! updateLoops(update2d4)
c write(*,*) 'n1a..',n1a,n1b,n1c,n1d,n2a,n2b,n2c
              if( option.eq.0 )then
                ! Jacobi
                do i3=n3a,n3b,n3c
                do i2=n2a,n2b,n2c
                do i1=n1a,n1b,n1c
                  if( mask(i1,i2,i3).gt.0 )then
                    v(i1,i2,i3)=update2d4(i1,i2,i3) ! update2d4 points R1 or B1
                  end if
                end do
                end do
                end do
              else if( option.eq.1 )then
                ! Gauss-Seidel
                do i3=n3a,n3b,n3c
                do i2=n2a,n2b,n2c
                do i1=n1a,n1b,n1c
                  if( mask(i1,i2,i3).gt.0 )then
                    u(i1,i2,i3)=update2d4(i1,i2,i3) ! update2d4 points R1 or B1
                  end if
                end do
                end do
                end do
              else if( option.eq.2 .or. option.eq.3 )then
! updateBoundaryLoops(v,u,update2d4)
c write(*,*) 'n1a..',n1a,n1b,n1c,n1d,n2a,n2b,n2c
                  if( bc(0,0).gt.0 )then
! boundaryLoop(v,u,update2d4,n1a,n1a+nb,n2a,n2b,n3a,n3b)
                    if( option.eq.2 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1a+nb
                        if( mask(i1,i2,i3).gt.0 )then
                          v(i1,i2,i3)=update2d4(i1,i2,i3)
                        end if
                      end do
                      end do
                      end do
                    else
                      ! Gauss-Seidel
                      ! write(*,*) 'bl:',n1a,n1a+nb,n2a,n2b,n3a,n3b
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1a+nb
                        if( mask(i1,i2,i3).gt.0 )then
                          u(i1,i2,i3)=update2d4(i1,i2,i3)
                          ! write(*,*) 'u,f=',u(i1,i2,i3),f(i1,i2,i3)
                        end if
                      end do
                      end do
                      end do
                    end if
                  end if
                  if( bc(1,0).gt.0 )then
! boundaryLoop(v,u,update2d4,n1b-nb,n1b,n2a,n2b,n3a,n3b)
                    if( option.eq.2 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1b-nb,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          v(i1,i2,i3)=update2d4(i1,i2,i3)
                        end if
                      end do
                      end do
                      end do
                    else
                      ! Gauss-Seidel
                      ! write(*,*) 'bl:',n1b-nb,n1b,n2a,n2b,n3a,n3b
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1b-nb,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          u(i1,i2,i3)=update2d4(i1,i2,i3)
                          ! write(*,*) 'u,f=',u(i1,i2,i3),f(i1,i2,i3)
                        end if
                      end do
                      end do
                      end do
                    end if
                  end if
                  if( bc(0,1).gt.0 )then
! boundaryLoop(v,u,update2d4,n1a,n1b,n2a,n2a+nb,n3a,n3b)
                    if( option.eq.2 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2a+nb
                      do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          v(i1,i2,i3)=update2d4(i1,i2,i3)
                        end if
                      end do
                      end do
                      end do
                    else
                      ! Gauss-Seidel
                      ! write(*,*) 'bl:',n1a,n1b,n2a,n2a+nb,n3a,n3b
                      do i3=n3a,n3b
                      do i2=n2a,n2a+nb
                      do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          u(i1,i2,i3)=update2d4(i1,i2,i3)
                          ! write(*,*) 'u,f=',u(i1,i2,i3),f(i1,i2,i3)
                        end if
                      end do
                      end do
                      end do
                    end if
                  end if
                  if( bc(1,1).gt.0 )then
! boundaryLoop(v,u,update2d4,n1a,n1b,n2b-nb,n2b,n3a,n3b)
                    if( option.eq.2 )then
                      do i3=n3a,n3b
                      do i2=n2b-nb,n2b
                      do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          v(i1,i2,i3)=update2d4(i1,i2,i3)
                        end if
                      end do
                      end do
                      end do
                    else
                      ! Gauss-Seidel
                      ! write(*,*) 'bl:',n1a,n1b,n2b-nb,n2b,n3a,n3b
                      do i3=n3a,n3b
                      do i2=n2b-nb,n2b
                      do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          u(i1,i2,i3)=update2d4(i1,i2,i3)
                          ! write(*,*) 'u,f=',u(i1,i2,i3),f(i1,i2,i3)
                        end if
                      end do
                      end do
                      end do
                    end if
                  end if
                  if( nd.eq.3 .and. bc(0,2).gt.0 )then
! boundaryLoop(v,u,update2d4,n1a,n1b,n2a,n2b,n3a,n3a+nb)
                    if( option.eq.2 )then
                      do i3=n3a,n3a+nb
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          v(i1,i2,i3)=update2d4(i1,i2,i3)
                        end if
                      end do
                      end do
                      end do
                    else
                      ! Gauss-Seidel
                      ! write(*,*) 'bl:',n1a,n1b,n2a,n2b,n3a,n3a+nb
                      do i3=n3a,n3a+nb
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          u(i1,i2,i3)=update2d4(i1,i2,i3)
                          ! write(*,*) 'u,f=',u(i1,i2,i3),f(i1,i2,i3)
                        end if
                      end do
                      end do
                      end do
                    end if
                  end if
                  if( nd.eq.3 .and. bc(1,2).gt.0 )then
! boundaryLoop(v,u,update2d4,n1a,n1b,n2a,n2b,n3b-nb,n3b)
                    if( option.eq.2 )then
                      do i3=n3b-nb,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          v(i1,i2,i3)=update2d4(i1,i2,i3)
                        end if
                      end do
                      end do
                      end do
                    else
                      ! Gauss-Seidel
                      ! write(*,*) 'bl:',n1a,n1b,n2a,n2b,n3b-nb,n3b
                      do i3=n3b-nb,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          u(i1,i2,i3)=update2d4(i1,i2,i3)
                          ! write(*,*) 'u,f=',u(i1,i2,i3),f(i1,i2,i3)
                        end if
                      end do
                      end do
                      end do
                    end if
                  end if
              else if( option.eq.4 )then
! updateListOfPoints(update2d4)
                 ! write(*,*) 'updateListOfPoints...'
                 if( nd.eq.2 )then
                  i3=0
                  do i=1,np
                    i1=ip(i,0)
                    i2=ip(i,1)
                    if( mask(i1,i2,i3).gt.0 )then
                      u(i1,i2,i3)=update2d4(i1,i2,i3)
                    end if
                  end do
                 else
                  do i=1,np
                    i1=ip(i,0)
                    i2=ip(i,1)
                    i3=ip(i,2)
                    if( mask(i1,i2,i3).gt.0 )then
                      u(i1,i2,i3)=update2d4(i1,i2,i3)
                    end if
                  end do
                 end if
              else if( option.eq.5 )then
! updateListOfPointsJacobi(update2d4)
                 ! write(*,*) 'updateListOfPointsJacobi...'
                 if( nd.eq.2 )then
                  i3=0
                  do i=1,np
                    i1=ip(i,0)
                    i2=ip(i,1)
                    if( mask(i1,i2,i3).gt.0 )then
                      v(i1,i2,i3)=update2d4(i1,i2,i3)
                    end if
                  end do
                  do i=1,np
                    i1=ip(i,0)
                    i2=ip(i,1)
                    if( mask(i1,i2,i3).gt.0 )then
                      u(i1,i2,i3)=v(i1,i2,i3)
                    end if
                  end do
                 else
                  do i=1,np
                    i1=ip(i,0)
                    i2=ip(i,1)
                    i3=ip(i,2)
                    if( mask(i1,i2,i3).gt.0 )then
                      v(i1,i2,i3)=update2d4(i1,i2,i3)
                    end if
                  end do
                  do i=1,np
                    i1=ip(i,0)
                    i2=ip(i,1)
                    i3=ip(i,2)
                    if( mask(i1,i2,i3).gt.0 )then
                      u(i1,i2,i3)=v(i1,i2,i3)
                    end if
                  end do
                 end if
              else
                write(*,*) 'ERROR: invalid option=',option
                stop 23
              end if
            else if( sparseStencil.eq.constantCoefficients )then
        !      **** constant coefficients *****
! updateLoops(update2dCC4)
c write(*,*) 'n1a..',n1a,n1b,n1c,n1d,n2a,n2b,n2c
              if( option.eq.0 )then
                ! Jacobi
                do i3=n3a,n3b,n3c
                do i2=n2a,n2b,n2c
                do i1=n1a,n1b,n1c
                  if( mask(i1,i2,i3).gt.0 )then
                    v(i1,i2,i3)=update2dCC4(i1,i2,i3) ! update2dCC4 points R1 or B1
                  end if
                end do
                end do
                end do
              else if( option.eq.1 )then
                ! Gauss-Seidel
                do i3=n3a,n3b,n3c
                do i2=n2a,n2b,n2c
                do i1=n1a,n1b,n1c
                  if( mask(i1,i2,i3).gt.0 )then
                    u(i1,i2,i3)=update2dCC4(i1,i2,i3) ! update2dCC4 points R1 or B1
                  end if
                end do
                end do
                end do
              else if( option.eq.2 .or. option.eq.3 )then
! updateBoundaryLoops(v,u,update2dCC4)
c write(*,*) 'n1a..',n1a,n1b,n1c,n1d,n2a,n2b,n2c
                  if( bc(0,0).gt.0 )then
! boundaryLoop(v,u,update2dCC4,n1a,n1a+nb,n2a,n2b,n3a,n3b)
                    if( option.eq.2 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1a+nb
                        if( mask(i1,i2,i3).gt.0 )then
                          v(i1,i2,i3)=update2dCC4(i1,i2,i3)
                        end if
                      end do
                      end do
                      end do
                    else
                      ! Gauss-Seidel
                      ! write(*,*) 'bl:',n1a,n1a+nb,n2a,n2b,n3a,n3b
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1a+nb
                        if( mask(i1,i2,i3).gt.0 )then
                          u(i1,i2,i3)=update2dCC4(i1,i2,i3)
                          ! write(*,*) 'u,f=',u(i1,i2,i3),f(i1,i2,i3)
                        end if
                      end do
                      end do
                      end do
                    end if
                  end if
                  if( bc(1,0).gt.0 )then
! boundaryLoop(v,u,update2dCC4,n1b-nb,n1b,n2a,n2b,n3a,n3b)
                    if( option.eq.2 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1b-nb,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          v(i1,i2,i3)=update2dCC4(i1,i2,i3)
                        end if
                      end do
                      end do
                      end do
                    else
                      ! Gauss-Seidel
                      ! write(*,*) 'bl:',n1b-nb,n1b,n2a,n2b,n3a,n3b
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1b-nb,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          u(i1,i2,i3)=update2dCC4(i1,i2,i3)
                          ! write(*,*) 'u,f=',u(i1,i2,i3),f(i1,i2,i3)
                        end if
                      end do
                      end do
                      end do
                    end if
                  end if
                  if( bc(0,1).gt.0 )then
! boundaryLoop(v,u,update2dCC4,n1a,n1b,n2a,n2a+nb,n3a,n3b)
                    if( option.eq.2 )then
                      do i3=n3a,n3b
                      do i2=n2a,n2a+nb
                      do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          v(i1,i2,i3)=update2dCC4(i1,i2,i3)
                        end if
                      end do
                      end do
                      end do
                    else
                      ! Gauss-Seidel
                      ! write(*,*) 'bl:',n1a,n1b,n2a,n2a+nb,n3a,n3b
                      do i3=n3a,n3b
                      do i2=n2a,n2a+nb
                      do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          u(i1,i2,i3)=update2dCC4(i1,i2,i3)
                          ! write(*,*) 'u,f=',u(i1,i2,i3),f(i1,i2,i3)
                        end if
                      end do
                      end do
                      end do
                    end if
                  end if
                  if( bc(1,1).gt.0 )then
! boundaryLoop(v,u,update2dCC4,n1a,n1b,n2b-nb,n2b,n3a,n3b)
                    if( option.eq.2 )then
                      do i3=n3a,n3b
                      do i2=n2b-nb,n2b
                      do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          v(i1,i2,i3)=update2dCC4(i1,i2,i3)
                        end if
                      end do
                      end do
                      end do
                    else
                      ! Gauss-Seidel
                      ! write(*,*) 'bl:',n1a,n1b,n2b-nb,n2b,n3a,n3b
                      do i3=n3a,n3b
                      do i2=n2b-nb,n2b
                      do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          u(i1,i2,i3)=update2dCC4(i1,i2,i3)
                          ! write(*,*) 'u,f=',u(i1,i2,i3),f(i1,i2,i3)
                        end if
                      end do
                      end do
                      end do
                    end if
                  end if
                  if( nd.eq.3 .and. bc(0,2).gt.0 )then
! boundaryLoop(v,u,update2dCC4,n1a,n1b,n2a,n2b,n3a,n3a+nb)
                    if( option.eq.2 )then
                      do i3=n3a,n3a+nb
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          v(i1,i2,i3)=update2dCC4(i1,i2,i3)
                        end if
                      end do
                      end do
                      end do
                    else
                      ! Gauss-Seidel
                      ! write(*,*) 'bl:',n1a,n1b,n2a,n2b,n3a,n3a+nb
                      do i3=n3a,n3a+nb
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          u(i1,i2,i3)=update2dCC4(i1,i2,i3)
                          ! write(*,*) 'u,f=',u(i1,i2,i3),f(i1,i2,i3)
                        end if
                      end do
                      end do
                      end do
                    end if
                  end if
                  if( nd.eq.3 .and. bc(1,2).gt.0 )then
! boundaryLoop(v,u,update2dCC4,n1a,n1b,n2a,n2b,n3b-nb,n3b)
                    if( option.eq.2 )then
                      do i3=n3b-nb,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          v(i1,i2,i3)=update2dCC4(i1,i2,i3)
                        end if
                      end do
                      end do
                      end do
                    else
                      ! Gauss-Seidel
                      ! write(*,*) 'bl:',n1a,n1b,n2a,n2b,n3b-nb,n3b
                      do i3=n3b-nb,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                          u(i1,i2,i3)=update2dCC4(i1,i2,i3)
                          ! write(*,*) 'u,f=',u(i1,i2,i3),f(i1,i2,i3)
                        end if
                      end do
                      end do
                      end do
                    end if
                  end if
              else if( option.eq.4 )then
! updateListOfPoints(update2dCC4)
                 ! write(*,*) 'updateListOfPoints...'
                 if( nd.eq.2 )then
                  i3=0
                  do i=1,np
                    i1=ip(i,0)
                    i2=ip(i,1)
                    if( mask(i1,i2,i3).gt.0 )then
                      u(i1,i2,i3)=update2dCC4(i1,i2,i3)
                    end if
                  end do
                 else
                  do i=1,np
                    i1=ip(i,0)
                    i2=ip(i,1)
                    i3=ip(i,2)
                    if( mask(i1,i2,i3).gt.0 )then
                      u(i1,i2,i3)=update2dCC4(i1,i2,i3)
                    end if
                  end do
                 end if
              else if( option.eq.5 )then
! updateListOfPointsJacobi(update2dCC4)
                 ! write(*,*) 'updateListOfPointsJacobi...'
                 if( nd.eq.2 )then
                  i3=0
                  do i=1,np
                    i1=ip(i,0)
                    i2=ip(i,1)
                    if( mask(i1,i2,i3).gt.0 )then
                      v(i1,i2,i3)=update2dCC4(i1,i2,i3)
                    end if
                  end do
                  do i=1,np
                    i1=ip(i,0)
                    i2=ip(i,1)
                    if( mask(i1,i2,i3).gt.0 )then
                      u(i1,i2,i3)=v(i1,i2,i3)
                    end if
                  end do
                 else
                  do i=1,np
                    i1=ip(i,0)
                    i2=ip(i,1)
                    i3=ip(i,2)
                    if( mask(i1,i2,i3).gt.0 )then
                      v(i1,i2,i3)=update2dCC4(i1,i2,i3)
                    end if
                  end do
                  do i=1,np
                    i1=ip(i,0)
                    i2=ip(i,1)
                    i3=ip(i,2)
                    if( mask(i1,i2,i3).gt.0 )then
                      u(i1,i2,i3)=v(i1,i2,i3)
                    end if
                  end do
                 end if
              else
                write(*,*) 'ERROR: invalid option=',option
                stop 23
              end if
            else
              write(*,*) 'smoothJacobiOpt: ERROR invalid sparseStencil'
              stop 1
            end if
        if( option.eq.0 )then
          ! Jacobi
          do i3=n3a,n3b
            do i2=n2a,n2b
              do i1=n1a,n1b
                if( mask(i1,i2,i3).gt.0 )then
                  u(i1,i2,i3)=v(i1,i2,i3)
                end if
              end do
            end do
          end do
        else if( option.eq.2 )then
          ! set u=v on the boundary
! updateBoundaryLoops(u,u,v)
c write(*,*) 'n1a..',n1a,n1b,n1c,n1d,n2a,n2b,n2c
            if( bc(0,0).gt.0 )then
! boundaryLoop(u,u,v,n1a,n1a+nb,n2a,n2b,n3a,n3b)
              if( option.eq.2 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1a+nb
                  if( mask(i1,i2,i3).gt.0 )then
                    u(i1,i2,i3)=v(i1,i2,i3)
                  end if
                end do
                end do
                end do
              else
                ! Gauss-Seidel
                ! write(*,*) 'bl:',n1a,n1a+nb,n2a,n2b,n3a,n3b
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1a+nb
                  if( mask(i1,i2,i3).gt.0 )then
                    u(i1,i2,i3)=v(i1,i2,i3)
                    ! write(*,*) 'u,f=',u(i1,i2,i3),f(i1,i2,i3)
                  end if
                end do
                end do
                end do
              end if
            end if
            if( bc(1,0).gt.0 )then
! boundaryLoop(u,u,v,n1b-nb,n1b,n2a,n2b,n3a,n3b)
              if( option.eq.2 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1b-nb,n1b
                  if( mask(i1,i2,i3).gt.0 )then
                    u(i1,i2,i3)=v(i1,i2,i3)
                  end if
                end do
                end do
                end do
              else
                ! Gauss-Seidel
                ! write(*,*) 'bl:',n1b-nb,n1b,n2a,n2b,n3a,n3b
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1b-nb,n1b
                  if( mask(i1,i2,i3).gt.0 )then
                    u(i1,i2,i3)=v(i1,i2,i3)
                    ! write(*,*) 'u,f=',u(i1,i2,i3),f(i1,i2,i3)
                  end if
                end do
                end do
                end do
              end if
            end if
            if( bc(0,1).gt.0 )then
! boundaryLoop(u,u,v,n1a,n1b,n2a,n2a+nb,n3a,n3b)
              if( option.eq.2 )then
                do i3=n3a,n3b
                do i2=n2a,n2a+nb
                do i1=n1a,n1b
                  if( mask(i1,i2,i3).gt.0 )then
                    u(i1,i2,i3)=v(i1,i2,i3)
                  end if
                end do
                end do
                end do
              else
                ! Gauss-Seidel
                ! write(*,*) 'bl:',n1a,n1b,n2a,n2a+nb,n3a,n3b
                do i3=n3a,n3b
                do i2=n2a,n2a+nb
                do i1=n1a,n1b
                  if( mask(i1,i2,i3).gt.0 )then
                    u(i1,i2,i3)=v(i1,i2,i3)
                    ! write(*,*) 'u,f=',u(i1,i2,i3),f(i1,i2,i3)
                  end if
                end do
                end do
                end do
              end if
            end if
            if( bc(1,1).gt.0 )then
! boundaryLoop(u,u,v,n1a,n1b,n2b-nb,n2b,n3a,n3b)
              if( option.eq.2 )then
                do i3=n3a,n3b
                do i2=n2b-nb,n2b
                do i1=n1a,n1b
                  if( mask(i1,i2,i3).gt.0 )then
                    u(i1,i2,i3)=v(i1,i2,i3)
                  end if
                end do
                end do
                end do
              else
                ! Gauss-Seidel
                ! write(*,*) 'bl:',n1a,n1b,n2b-nb,n2b,n3a,n3b
                do i3=n3a,n3b
                do i2=n2b-nb,n2b
                do i1=n1a,n1b
                  if( mask(i1,i2,i3).gt.0 )then
                    u(i1,i2,i3)=v(i1,i2,i3)
                    ! write(*,*) 'u,f=',u(i1,i2,i3),f(i1,i2,i3)
                  end if
                end do
                end do
                end do
              end if
            end if
            if( nd.eq.3 .and. bc(0,2).gt.0 )then
! boundaryLoop(u,u,v,n1a,n1b,n2a,n2b,n3a,n3a+nb)
              if( option.eq.2 )then
                do i3=n3a,n3a+nb
                do i2=n2a,n2b
                do i1=n1a,n1b
                  if( mask(i1,i2,i3).gt.0 )then
                    u(i1,i2,i3)=v(i1,i2,i3)
                  end if
                end do
                end do
                end do
              else
                ! Gauss-Seidel
                ! write(*,*) 'bl:',n1a,n1b,n2a,n2b,n3a,n3a+nb
                do i3=n3a,n3a+nb
                do i2=n2a,n2b
                do i1=n1a,n1b
                  if( mask(i1,i2,i3).gt.0 )then
                    u(i1,i2,i3)=v(i1,i2,i3)
                    ! write(*,*) 'u,f=',u(i1,i2,i3),f(i1,i2,i3)
                  end if
                end do
                end do
                end do
              end if
            end if
            if( nd.eq.3 .and. bc(1,2).gt.0 )then
! boundaryLoop(u,u,v,n1a,n1b,n2a,n2b,n3b-nb,n3b)
              if( option.eq.2 )then
                do i3=n3b-nb,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                  if( mask(i1,i2,i3).gt.0 )then
                    u(i1,i2,i3)=v(i1,i2,i3)
                  end if
                end do
                end do
                end do
              else
                ! Gauss-Seidel
                ! write(*,*) 'bl:',n1a,n1b,n2a,n2b,n3b-nb,n3b
                do i3=n3b-nb,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                  if( mask(i1,i2,i3).gt.0 )then
                    u(i1,i2,i3)=v(i1,i2,i3)
                    ! write(*,*) 'u,f=',u(i1,i2,i3),f(i1,i2,i3)
                  end if
                end do
                end do
                end do
              end if
            end if
        end if
        return
        end
