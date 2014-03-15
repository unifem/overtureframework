! This file automatically generated from smoothOpt.bf with bpp.
! SMOOTH_SUBROUTINE(smoothRB2dOrder2,2,2)
        subroutine smoothRB2dOrder2( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     & n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c, ndc, f, c, u, v, mask, 
     & option, order, sparseStencil, cc, s, dx, omega, 
     & useLocallyOptimalOmega,variableOmegaScaleFactor, ipar, rpar )
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
        integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, n1a,n1b,n1c,n2a,n2b,
     & n2c,n3a,n3b,n3c, ndc, option, sparseStencil,order,
     & useLocallyOptimalOmega
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
        !     *** statement functions ***
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
c ===========  2nd order ===========================
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
        ! --------- const coefficients versions ----------------
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
        ! ===========  4th order ===========================
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
        ! --------- const coefficients versions ----------------
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
! #If "2" == "3"
c   *** end statement functions
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
        dx2i=.5/dx(1)**2
        dy2i=.5/dx(2)**2
        dz2i=.5/dx(3)**2
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
              omega=1.15 ! NOTE change other value too
            end if
          end if
        end if
        ! write(*,*) 'smoothRB: omega=',omega
!  #If "2" == "2"
c     Red and black points:
c     B2 R2 B2 R2 B2 R2 B2 R2 
c     R1 B1 R1 B1 R1 B1 R1 B1
c     B2 R2 B2 R2 B2 R2 B2 R2 
c     R1 B1 R1 B1 R1 B1 R1 B1
          if( option .eq. 0 )then
            !     **** red points *****
            i1s=1
            i2s=1
            n1d=n1b
          else
            !    **** black points *****
            i1s=-1
            i2s=1
       !     if( mod(n1b,2).eq.0 )then  ! added 040409 for P++ case *** no -- cannaot assume n1b mod 2==0 for black!! (neumann)
            if( .true. )then
              n1d=n1b-1
            else
              n1d=n1b  ! this case for red points that we make look like black
            end if
          end if
!    #If "2" == "2"
            m11=1                ! MCE(-1,-1, 0)
            m21=2                ! MCE( 0,-1, 0)
            m31=3                ! MCE(+1,-1, 0)
            m12=4                ! MCE(-1, 0, 0)
            m22=5                ! MCE( 0, 0, 0)
            m32=6                ! MCE(+1, 0, 0)
            m13=7                ! MCE(-1,+1, 0)
            m23=8                ! MCE( 0,+1, 0)
            m33=9                ! MCE(+1,+1, 0)
            if( sparseStencil.eq.sparse )then
             !    Here we can assume that the operator is a 5-point  operator 
! updateLoops2d(update2dSparse)
c write(*,*) '***RB: n1a..',n1a,n1b,n1c,n1d,n2a,n2b,n2c
             do i3=n3a,n3b,n3c
               j3=i3
               do i2=n2a,n2b,n2c
                 do i1=n1a,n1b,n1c
                   if( mask(i1,i2,i3).gt.0 .and. i1.le.n1d )then
                     v(i1,i2,i3)=update2dSparse(i1,i2,i3) ! update2dSparse points R1 or B1
c        write(*,*) 'RB: i1',i1,i2,v(i1,i2,i3)
c        if( sparseStencil.eq.general )then
c        write(*,*) f(i1,i2,i3),c(m11,i1,i2,i3),c(m21,i1,i2,i3),c(m31,i1,i2,i3),c          c(m12,i1,i2,i3),c(m22,i1,i2,i3),c(m32,i1,i2,i3),c(m13,i1,i2,i3),c(m23,i1,i2,i3),c(m33,i1,i2,i3),c(m22,i1,i2,i3)+eps
c      end if
                   end if
                   j1=i1+i1s
                   j2=i2+i2s
                   if( mask(j1,j2,i3).gt.0 .and. j1.le.n1d .and. 
     & j2.le.n2b )then
                     v(j1,j2,i3)=update2dSparse(j1,j2,i3) ! update2dSparse points R2 or B2
c        write(*,*) 'RB: j1',j1,j2,v(j1,j2,i3)
                   end if
                 end do
               end do
             end do
            else if( sparseStencil.eq.sparseConstantCoefficients )then
! updateLoops2d(update2dSparseCC)
c write(*,*) '***RB: n1a..',n1a,n1b,n1c,n1d,n2a,n2b,n2c
              do i3=n3a,n3b,n3c
                j3=i3
                do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 .and. i1.le.n1d )then
                      v(i1,i2,i3)=update2dSparseCC(i1,i2,i3) ! update2dSparseCC points R1 or B1
c        write(*,*) 'RB: i1',i1,i2,v(i1,i2,i3)
c        if( sparseStencil.eq.general )then
c        write(*,*) f(i1,i2,i3),c(m11,i1,i2,i3),c(m21,i1,i2,i3),c(m31,i1,i2,i3),c          c(m12,i1,i2,i3),c(m22,i1,i2,i3),c(m32,i1,i2,i3),c(m13,i1,i2,i3),c(m23,i1,i2,i3),c(m33,i1,i2,i3),c(m22,i1,i2,i3)+eps
c      end if
                    end if
                    j1=i1+i1s
                    j2=i2+i2s
                    if( mask(j1,j2,i3).gt.0 .and. j1.le.n1d .and. 
     & j2.le.n2b )then
                      v(j1,j2,i3)=update2dSparseCC(j1,j2,i3) ! update2dSparseCC points R2 or B2
c        write(*,*) 'RB: j1',j1,j2,v(j1,j2,i3)
                    end if
                  end do
                end do
              end do
            else if( sparseStencil.eq.general )then
              !   **** full stencil *****
              if( useLocallyOptimalOmega.ne.0 )then
! updateLoops2dVariableOmega(update2d)
c write(*,*) 'n1a..',n1a,n1b,n1c,n1d,n2a,n2b,n2c
                do i3=n3a,n3b,n3c
                  j3=i3
                  do i2=n2a,n2b,n2c
                    do i1=n1a,n1b,n1c
                      if( mask(i1,i2,i3).gt.0 .and. i1.le.n1d )then
! computeOmega2d(i1,i2,i3)
                         c1=abs(c(m21,i1,i2,i3)+c(m23,i1,i2,i3))
                         c2=abs(c(m12,i1,i2,i3)+c(m32,i1,i2,i3))
                         cmax=1.-min(c1,c2)/(c1+c2)
                         omega=variableOmegaFactor/(1.+sqrt(1.-cmax**2)
     & )
                         ! write(*,'(''i1,i2='',2i3,'' cmax,omega='',2(f7.4,1x))') i1,i2,cmax,omega
                        v(i1,i2,i3)=update2d(i1,i2,i3) ! update2d points R1 or B1
                      end if
                      j1=i1+i1s
                      j2=i2+i2s
                      if( mask(j1,j2,i3).gt.0 .and. j1.le.n1d .and. 
     & j2.le.n2b )then
! computeOmega2d(j1,j2,j3)
                         c1=abs(c(m21,j1,j2,j3)+c(m23,j1,j2,j3))
                         c2=abs(c(m12,j1,j2,j3)+c(m32,j1,j2,j3))
                         cmax=1.-min(c1,c2)/(c1+c2)
                         omega=variableOmegaFactor/(1.+sqrt(1.-cmax**2)
     & )
                         ! write(*,'(''j1,j2='',2i3,'' cmax,omega='',2(f7.4,1x))') j1,j2,cmax,omega
                        v(j1,j2,i3)=update2d(j1,j2,i3) ! update2d points R2 or B2
c        write(*,*) 'j1',i1,i2,u(i1,i2,i3)
                      end if
                    end do
                  end do
                end do
              else
! updateLoops2d(update2d)
c write(*,*) '***RB: n1a..',n1a,n1b,n1c,n1d,n2a,n2b,n2c
                do i3=n3a,n3b,n3c
                  j3=i3
                  do i2=n2a,n2b,n2c
                    do i1=n1a,n1b,n1c
                      if( mask(i1,i2,i3).gt.0 .and. i1.le.n1d )then
                        v(i1,i2,i3)=update2d(i1,i2,i3) ! update2d points R1 or B1
c        write(*,*) 'RB: i1',i1,i2,v(i1,i2,i3)
c        if( sparseStencil.eq.general )then
c        write(*,*) f(i1,i2,i3),c(m11,i1,i2,i3),c(m21,i1,i2,i3),c(m31,i1,i2,i3),c          c(m12,i1,i2,i3),c(m22,i1,i2,i3),c(m32,i1,i2,i3),c(m13,i1,i2,i3),c(m23,i1,i2,i3),c(m33,i1,i2,i3),c(m22,i1,i2,i3)+eps
c      end if
                      end if
                      j1=i1+i1s
                      j2=i2+i2s
                      if( mask(j1,j2,i3).gt.0 .and. j1.le.n1d .and. 
     & j2.le.n2b )then
                        v(j1,j2,i3)=update2d(j1,j2,i3) ! update2d points R2 or B2
c        write(*,*) 'RB: j1',j1,j2,v(j1,j2,i3)
                      end if
                    end do
                  end do
                end do
              end if
            else if( sparseStencil.eq.constantCoefficients )then
              !   **** constant coefficients *****
! updateLoops2d(update2dCC)
c write(*,*) '***RB: n1a..',n1a,n1b,n1c,n1d,n2a,n2b,n2c
              do i3=n3a,n3b,n3c
                j3=i3
                do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 .and. i1.le.n1d )then
                      v(i1,i2,i3)=update2dCC(i1,i2,i3) ! update2dCC points R1 or B1
c        write(*,*) 'RB: i1',i1,i2,v(i1,i2,i3)
c        if( sparseStencil.eq.general )then
c        write(*,*) f(i1,i2,i3),c(m11,i1,i2,i3),c(m21,i1,i2,i3),c(m31,i1,i2,i3),c          c(m12,i1,i2,i3),c(m22,i1,i2,i3),c(m32,i1,i2,i3),c(m13,i1,i2,i3),c(m23,i1,i2,i3),c(m33,i1,i2,i3),c(m22,i1,i2,i3)+eps
c      end if
                    end if
                    j1=i1+i1s
                    j2=i2+i2s
                    if( mask(j1,j2,i3).gt.0 .and. j1.le.n1d .and. 
     & j2.le.n2b )then
                      v(j1,j2,i3)=update2dCC(j1,j2,i3) ! update2dCC points R2 or B2
c        write(*,*) 'RB: j1',j1,j2,v(j1,j2,i3)
                    end if
                  end do
                end do
              end do
            else if( sparseStencil.eq.sparseVariableCoefficients )then
! updateLoops2dSparseVC()
              do i3=n3a,n3b,n3c
                j3=i3
                do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 .and. i1.le.n1d )then
                      a1p=(s(i1,i2,i3)+s(i1+1,i2,i3))*dx2i
                      a1m=(s(i1,i2,i3)+s(i1-1,i2,i3))*dx2i
                      a2p=(s(i1,i2,i3)+s(i1,i2+1,i3))*dy2i
                      a2m=(s(i1,i2,i3)+s(i1,i2-1,i3))*dy2i
                      ad=-(a1p+a1m+a2p+a2m)
                      v(i1,i2,i3)=update2dSparseVC(i1,i2,i3) ! update points R1 or B1
                    end if
                    j1=i1+i1s
                    j2=i2+i2s
                    if( mask(j1,j2,i3).gt.0 .and. j1.le.n1d .and. 
     & j2.le.n2b )then
                      a1p=(s(j1,j2,j3)+s(j1+1,j2,j3))*dx2i
                      a1m=(s(j1,j2,j3)+s(j1-1,j2,j3))*dx2i
                      a2p=(s(j1,j2,j3)+s(j1,j2+1,j3))*dy2i
                      a2m=(s(j1,j2,j3)+s(j1,j2-1,j3))*dy2i
                      ad=-(a1p+a1m+a2p+a2m)
                      v(j1,j2,i3)=update2dSparseVC(j1,j2,i3) ! update points R2 or B2
                    end if
                  end do
                end do
              end do
            else if( sparseStencil.eq.variableCoefficients )then
              ! use sparse version for now:
! updateLoops2dSparseVC()
              do i3=n3a,n3b,n3c
                j3=i3
                do i2=n2a,n2b,n2c
                  do i1=n1a,n1b,n1c
                    if( mask(i1,i2,i3).gt.0 .and. i1.le.n1d )then
                      a1p=(s(i1,i2,i3)+s(i1+1,i2,i3))*dx2i
                      a1m=(s(i1,i2,i3)+s(i1-1,i2,i3))*dx2i
                      a2p=(s(i1,i2,i3)+s(i1,i2+1,i3))*dy2i
                      a2m=(s(i1,i2,i3)+s(i1,i2-1,i3))*dy2i
                      ad=-(a1p+a1m+a2p+a2m)
                      v(i1,i2,i3)=update2dSparseVC(i1,i2,i3) ! update points R1 or B1
                    end if
                    j1=i1+i1s
                    j2=i2+i2s
                    if( mask(j1,j2,i3).gt.0 .and. j1.le.n1d .and. 
     & j2.le.n2b )then
                      a1p=(s(j1,j2,j3)+s(j1+1,j2,j3))*dx2i
                      a1m=(s(j1,j2,j3)+s(j1-1,j2,j3))*dx2i
                      a2p=(s(j1,j2,j3)+s(j1,j2+1,j3))*dy2i
                      a2m=(s(j1,j2,j3)+s(j1,j2-1,j3))*dy2i
                      ad=-(a1p+a1m+a2p+a2m)
                      v(j1,j2,i3)=update2dSparseVC(j1,j2,i3) ! update points R2 or B2
                    end if
                  end do
                end do
              end do
            else
              write(*,*) 'smoothRedBlackOpt: ERROR invalid 
     & sparseStencil'
              stop 1
            end if
        return
        end
