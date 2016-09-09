! This file automatically generated from opcoeff.bf with bpp.
! coeffOperator6thOrder(xy)
       subroutine xyCoeff6thOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,nds2b,
     & nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc,nc,ns,ea,eb,ca,cb,dx,
     & dr, rsxy,coeff, derivOption, derivType, gridType, order, s, 
     & jac, averagingType, dir1, dir2,a11,a22,a12,a21,a33,a13,a23,a31,
     & a32 )
       ! ===============================================================
       !  Derivative Coefficients - 6th order version
       !  
       ! gridType: 0=rectangular, 1=non-rectangular
       ! rsxy : not used if rectangular
       ! h42 : 1/h**2 : for rectangular  
       ! ===============================================================
       !      implicit none
       integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,n1a,n1b,n2a,n2b,n3a,
     & n3b, ndc, nc, ns, ea,eb, ca,cb,gridType,order
       integer ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,
     & nds2b,nds3a,nds3b
       integer derivOption, derivType, averagingType, dir1, dir2
       real dx(3),dr(3)
       real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,1:nd,1:nd)
       real coeff(1:ndc,ndc1a:ndc1b,ndc2a:ndc2b,ndc3a:ndc3b)
       ! *wdh* 2016/08/27 real s(nds1a:nds1b,nds2a:nds2b,nds3a:nds3b)
       real s(nds1a:nds1b,nds2a:nds2b,nds3a:nds3b,0:*)
       real jac(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
       real a11(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
       real a12(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
       real a13(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
       real a21(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
       real a22(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
       real a23(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
       real a31(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
       real a32(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
       real a33(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
       !..... added by kkc 1/2/02 for g77 unsatisfied reference
       real u(1,1,1,1)
       real d24(3),d14(3),h42(3),h41(3)
       real d26(3),d16(3),h62(3),h61(3)
       integer i1,i2,i3,kd3,kd,kdd,e,c,ec,j1,j2,j3
       integer m12,m22,m32,m42,m52
       integer width,halfWidth
       integer m(-3:3,-3:3),m3(-3:3,-3:3,-3:3)
       integer laplace,divScalarGrad,derivativeScalarDerivative
       parameter(laplace=0,divScalarGrad=1,
     & derivativeScalarDerivative=2)
       integer arithmeticAverage,harmonicAverage
       parameter( arithmeticAverage=0,harmonicAverage=1 )
       integer symmetric
       parameter( symmetric=2 )
       !....statement functions for jacobian
       rx(i1,i2,i3)=rsxy(i1,i2,i3,  1,  1)
       ry(i1,i2,i3)=rsxy(i1,i2,i3,  1,  2)
       rz(i1,i2,i3)=rsxy(i1,i2,i3,  1,kd3)
       sx(i1,i2,i3)=rsxy(i1,i2,i3,  2,  1)
       sy(i1,i2,i3)=rsxy(i1,i2,i3,  2,  2)
       sz(i1,i2,i3)=rsxy(i1,i2,i3,  2,kd3)
       tx(i1,i2,i3)=rsxy(i1,i2,i3,kd3,  1)
       ty(i1,i2,i3)=rsxy(i1,i2,i3,kd3,  2)
       tz(i1,i2,i3)=rsxy(i1,i2,i3,kd3,kd3)
       include 'cgux4af.h'
       rxx1(i1,i2,i3)=rx(i1,i2,i3)*rxr(i1,i2,i3)
       !.....end statement functions
! #If "xy" == "laplacian"
! #Else
         if( .true. )then
           write(*,*) 'opcoeff: order=6  finish me!'
           stop 1190
         end if
       if( order.ne.6 )then
         write(*,*) 'opcoeff: ERROR: order!=6 '
         stop 1191
       end if
       ! stencil width and "half-width"
       width=7
       halfWidth=3
! #If "xy" == "divScalarGrad"
       ! keep d14, d24, etc. for now ... while converting to order=6
       do n=1,3
         d14(n)=1./(12.*dr(n))
         d24(n)=1./(12.*dr(n)**2)
         h41(n)=1./(12.*dx(n))
         h42(n)=1./(12.*dx(n)**2)
         h62(n)=1./(180.*dx(n)**2)
       end do
       kd3=nd
       if( nd .eq. 2 )then
       !       ************************
       !       ******* 2D *************      
       !       ************************
!   #If "xy" == "identity"
!   #Elif "xy" == "r"
!   #Elif "xy" == "s"
!   #Elif "xy" == "rr"
!   #Elif "xy" == "ss"
!   #Elif "xy" == "rs"
         if( gridType .eq. 0 )then
       !   rectangular
! beginLoops6()
           ! ***** loop over equations and components *****
           do e=ea,eb
           do c=ca,cb
           ec=ns*(c+nc*e)
           ! ** it did not affect performance to use an array to index coeff ***
           if( nd.eq.2 )then
           do i2=-halfWidth,halfWidth
             do i1=-halfWidth,halfWidth
              m(i1,i2)=i1+halfWidth+width*(i2+halfWidth) +1 + ec
             end do
           end do
           else if( nd.eq.3 )then
           do i3=-halfWidth,halfWidth
             do i2=-halfWidth,halfWidth
               do i1=-halfWidth,halfWidth
                 m3(i1,i2,i3)=i1+halfWidth+width*(i2+halfWidth+width*(
     & i3+halfWidth)) +1 + ec
               end do
             end do
           end do
           else
           m12=1+ec
           m22=2+ec
           m32=3+ec
           m42=4+ec
           m52=5+ec
           endif
           do i3=n3a,n3b
           do i2=n2a,n2b
           do i1=n1a,n1b
!       #If "xy" == "laplacian"
!       #Elif "xy" == "x"
!       #Elif "xy" == "y"
!       #Elif "xy" == "xx"
!       #Elif "xy" == "yy"
!       #Elif "xy" == "xy"
               d=h41(1)*h41(2)
               d8=d*8.
               d64=d*64.
! loopBody6thOrder2d(d,-d8,0.,d8,-d, -d8,d64,0.,-d64,d8, 0.,0.,0.,0.,0., d8,-d64,0.,d64,-d8, -d,d8,0.,-d8,d)
                coeff(m(-2,-2),i1,i2,i3)=d
                coeff(m(-1,-2),i1,i2,i3)=-d8
                coeff(m( 0,-2),i1,i2,i3)=0.
                coeff(m( 1,-2),i1,i2,i3)=d8
                coeff(m( 2,-2),i1,i2,i3)=-d
                coeff(m(-2,-1),i1,i2,i3)=-d8
                coeff(m(-1,-1),i1,i2,i3)=d64
                coeff(m( 0,-1),i1,i2,i3)=0.
                coeff(m( 1,-1),i1,i2,i3)=-d64
                coeff(m( 2,-1),i1,i2,i3)=d8
                coeff(m(-2, 0),i1,i2,i3)=0.
                coeff(m(-1, 0),i1,i2,i3)=0.
                coeff(m( 0, 0),i1,i2,i3)=0.
                coeff(m(+1, 0),i1,i2,i3)=0.
                coeff(m(+2, 0),i1,i2,i3)=0.
                coeff(m(-2, 1),i1,i2,i3)=d8
                coeff(m(-1, 1),i1,i2,i3)=-d64
                coeff(m( 0, 1),i1,i2,i3)=0.
                coeff(m( 1, 1),i1,i2,i3)=d64
                coeff(m( 2, 1),i1,i2,i3)=-d8
                coeff(m(-2, 2),i1,i2,i3)=-d
                coeff(m(-1, 2),i1,i2,i3)=d8
                coeff(m( 0, 2),i1,i2,i3)=0.
                coeff(m( 1, 2),i1,i2,i3)=-d8
                coeff(m( 2, 2),i1,i2,i3)=d
! endLoops()
           end do
           end do
           end do
           end do
           end do
         else
       !  ***** not rectangular *****
! beginLoops6()
           ! ***** loop over equations and components *****
           do e=ea,eb
           do c=ca,cb
           ec=ns*(c+nc*e)
           ! ** it did not affect performance to use an array to index coeff ***
           if( nd.eq.2 )then
           do i2=-halfWidth,halfWidth
             do i1=-halfWidth,halfWidth
              m(i1,i2)=i1+halfWidth+width*(i2+halfWidth) +1 + ec
             end do
           end do
           else if( nd.eq.3 )then
           do i3=-halfWidth,halfWidth
             do i2=-halfWidth,halfWidth
               do i1=-halfWidth,halfWidth
                 m3(i1,i2,i3)=i1+halfWidth+width*(i2+halfWidth+width*(
     & i3+halfWidth)) +1 + ec
               end do
             end do
           end do
           else
           m12=1+ec
           m22=2+ec
           m32=3+ec
           m42=4+ec
           m52=5+ec
           endif
           do i3=n3a,n3b
           do i2=n2a,n2b
           do i1=n1a,n1b
!       #If "xy" == "laplacian"
!       #Elif "xy" == "x"
!       #Elif "xy" == "y"
!       #Elif "xy" == "xx"
!       #Elif "xy" == "yy"
!      #Elif "xy" == "xy"
! xy6thOrder2d(x,y)
              rxsq = d24(1)*(r x(i1,i2,i3)*r y(i1,i2,i3))
              rxxyy= d14(1)*(r x y (i1,i2,i3))
              sxsq = d24(2)*(s x(i1,i2,i3)*s y(i1,i2,i3))
              sxxyy=d14(2)*(s x y (i1,i2,i3))
              rsx = (d14(1)*d14(2))*(r x(i1,i2,i3)*s y(i1,i2,i3)+r y(
     & i1,i2,i3)*s x(i1,i2,i3))
              ! check this	
! loopBody6thOrder2d(rsx,-8.*rsx,-sxSq+sxxyy,8.*rsx,-rsx, -8.*rsx,64.*rsx,16.*sxSq-8.*sxxyy,-64.*rsx,8.*rsx, -rxSq+rxxyy,16.*rxSq-8.*rxxyy,-30.*(rxSq+sxSq),16.*rxSq+8.*rxxyy,-rxSq-rxxyy,8.*rsx,-64.*rsx,16.*sxSq+8.*sxxyy,64.*rsx,-8.*rsx, -rsx,8.*rsx,-sxSq-sxxyy,-8.*rsx,rsx)
               coeff(m(-2,-2),i1,i2,i3)=rsx
               coeff(m(-1,-2),i1,i2,i3)=-8.*rsx
               coeff(m( 0,-2),i1,i2,i3)=-sxSq+sxxyy
               coeff(m( 1,-2),i1,i2,i3)=8.*rsx
               coeff(m( 2,-2),i1,i2,i3)=-rsx
               coeff(m(-2,-1),i1,i2,i3)=-8.*rsx
               coeff(m(-1,-1),i1,i2,i3)=64.*rsx
               coeff(m( 0,-1),i1,i2,i3)=16.*sxSq-8.*sxxyy
               coeff(m( 1,-1),i1,i2,i3)=-64.*rsx
               coeff(m( 2,-1),i1,i2,i3)=8.*rsx
               coeff(m(-2, 0),i1,i2,i3)=-rxSq+rxxyy
               coeff(m(-1, 0),i1,i2,i3)=16.*rxSq-8.*rxxyy
               coeff(m( 0, 0),i1,i2,i3)=-30.*(rxSq+sxSq)
               coeff(m(+1, 0),i1,i2,i3)=16.*rxSq+8.*rxxyy
               coeff(m(+2, 0),i1,i2,i3)=-rxSq-rxxyy
               coeff(m(-2, 1),i1,i2,i3)=8.*rsx
               coeff(m(-1, 1),i1,i2,i3)=-64.*rsx
               coeff(m( 0, 1),i1,i2,i3)=16.*sxSq+8.*sxxyy
               coeff(m( 1, 1),i1,i2,i3)=64.*rsx
               coeff(m( 2, 1),i1,i2,i3)=-8.*rsx
               coeff(m(-2, 2),i1,i2,i3)=-rsx
               coeff(m(-1, 2),i1,i2,i3)=8.*rsx
               coeff(m( 0, 2),i1,i2,i3)=-sxSq-sxxyy
               coeff(m( 1, 2),i1,i2,i3)=-8.*rsx
               coeff(m( 2, 2),i1,i2,i3)=rsx
! endLoops()
           end do
           end do
           end do
           end do
           end do
         endif
       elseif( nd.eq.3 )then
       !       ************************
       !       ******* 3D *************      
       !       ************************
!   #If "xy" == "identity"
!   #Elif "xy" == "r"
!   #Elif "xy" == "s"
!   #Elif "xy" == "t"
!   #Elif "xy" == "rr"
!   #Elif "xy" == "ss"
!   #Elif "xy" == "tt"
!   #Elif "xy" == "rs"
!   #Elif "xy" == "rt"
!   #Elif "xy" == "st"
         if( gridType .eq. 0 )then
       !   rectangular
! beginLoops6()
           ! ***** loop over equations and components *****
           do e=ea,eb
           do c=ca,cb
           ec=ns*(c+nc*e)
           ! ** it did not affect performance to use an array to index coeff ***
           if( nd.eq.2 )then
           do i2=-halfWidth,halfWidth
             do i1=-halfWidth,halfWidth
              m(i1,i2)=i1+halfWidth+width*(i2+halfWidth) +1 + ec
             end do
           end do
           else if( nd.eq.3 )then
           do i3=-halfWidth,halfWidth
             do i2=-halfWidth,halfWidth
               do i1=-halfWidth,halfWidth
                 m3(i1,i2,i3)=i1+halfWidth+width*(i2+halfWidth+width*(
     & i3+halfWidth)) +1 + ec
               end do
             end do
           end do
           else
           m12=1+ec
           m22=2+ec
           m32=3+ec
           m42=4+ec
           m52=5+ec
           endif
           do i3=n3a,n3b
           do i2=n2a,n2b
           do i1=n1a,n1b
!      #If "xy" == "laplacian"
!      #Elif "xy" == "x"
!      #Elif "xy" == "y"
!      #Elif "xy" == "z"
!      #Elif "xy" == "xx"
!      #Elif "xy" == "yy"
!      #Elif "xy" == "zz"
!      #Elif "xy" == "xy"
! xy6thOrder3dRectangular(x,y,1,2)
              d=h41(1)*h41(2)
              d8=d*8.
              d64=d*64.
! loopBody6thOrder3dSwitchxy(0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,d,-d8,0.,d8,-d, -d8,d64,0.,-d64,d8, 0.,0.,0.,0.,0., d8,-d64,0.,d64,-d8, -d,d8,0.,-d8,d,0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.)
! loopBody6thOrder3d(0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,0.,0.,0.,0.,0., 0.,0.,0.,0.,0., d,-d8,0.,d8,-d, -d8,d64,0.,-d64,d8,0.,0.,0.,0.,0., d8,-d64,0.,d64,-d8, -d,d8,0.,-d8,d, 0.,0.,0.,0.,0.,0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,0.,0.,0.,0.,0.)
               coeff(m3(-2,-2,-2),i1,i2,i3)=0.
               coeff(m3(-1,-2,-2),i1,i2,i3)=0.
               coeff(m3( 0,-2,-2),i1,i2,i3)=0.
               coeff(m3( 1,-2,-2),i1,i2,i3)=0.
               coeff(m3( 2,-2,-2),i1,i2,i3)=0.
               coeff(m3(-2,-1,-2),i1,i2,i3)=0.
               coeff(m3(-1,-1,-2),i1,i2,i3)=0.
               coeff(m3( 0,-1,-2),i1,i2,i3)=0.
               coeff(m3( 1,-1,-2),i1,i2,i3)=0.
               coeff(m3( 2,-1,-2),i1,i2,i3)=0.
               coeff(m3(-2, 0,-2),i1,i2,i3)=0.
               coeff(m3(-1, 0,-2),i1,i2,i3)=0.
               coeff(m3( 0, 0,-2),i1,i2,i3)=0.
               coeff(m3(+1, 0,-2),i1,i2,i3)=0.
               coeff(m3(+2, 0,-2),i1,i2,i3)=0.
               coeff(m3(-2, 1,-2),i1,i2,i3)=0.
               coeff(m3(-1, 1,-2),i1,i2,i3)=0.
               coeff(m3( 0, 1,-2),i1,i2,i3)=0.
               coeff(m3( 1, 1,-2),i1,i2,i3)=0.
               coeff(m3( 2, 1,-2),i1,i2,i3)=0.
               coeff(m3(-2, 2,-2),i1,i2,i3)=0.
               coeff(m3(-1, 2,-2),i1,i2,i3)=0.
               coeff(m3( 0, 2,-2),i1,i2,i3)=0.
               coeff(m3( 1, 2,-2),i1,i2,i3)=0.
               coeff(m3( 2, 2,-2),i1,i2,i3)=0.
               coeff(m3(-2,-2,-1),i1,i2,i3)=0.
               coeff(m3(-1,-2,-1),i1,i2,i3)=0.
               coeff(m3( 0,-2,-1),i1,i2,i3)=0.
               coeff(m3(+1,-2,-1),i1,i2,i3)=0.
               coeff(m3(+2,-2,-1),i1,i2,i3)=0.
               coeff(m3(-2,-1,-1),i1,i2,i3)=0.
               coeff(m3(-1,-1,-1),i1,i2,i3)=0.
               coeff(m3( 0,-1,-1),i1,i2,i3)=0.
               coeff(m3(+1,-1,-1),i1,i2,i3)=0.
               coeff(m3(+2,-1,-1),i1,i2,i3)=0.
               coeff(m3(-2, 0,-1),i1,i2,i3)=0.
               coeff(m3(-1, 0,-1),i1,i2,i3)=0.
               coeff(m3( 0, 0,-1),i1,i2,i3)=0.
               coeff(m3(+1, 0,-1),i1,i2,i3)=0.
               coeff(m3(+2, 0,-1),i1,i2,i3)=0.
               coeff(m3(-2,+1,-1),i1,i2,i3)=0.
               coeff(m3(-1,+1,-1),i1,i2,i3)=0.
               coeff(m3( 0,+1,-1),i1,i2,i3)=0.
               coeff(m3(+1,+1,-1),i1,i2,i3)=0.
               coeff(m3(+2,+1,-1),i1,i2,i3)=0.
               coeff(m3(-2,+2,-1),i1,i2,i3)=0.
               coeff(m3(-1,+2,-1),i1,i2,i3)=0.
               coeff(m3( 0,+2,-1),i1,i2,i3)=0.
               coeff(m3(+1,+2,-1),i1,i2,i3)=0.
               coeff(m3(+2,+2,-1),i1,i2,i3)=0.
               coeff(m3(-2,-2, 0),i1,i2,i3)=d
               coeff(m3(-1,-2, 0),i1,i2,i3)=-d8
               coeff(m3( 0,-2, 0),i1,i2,i3)=0.
               coeff(m3(+1,-2, 0),i1,i2,i3)=d8
               coeff(m3(+2,-2, 0),i1,i2,i3)=-d
               coeff(m3(-2,-1, 0),i1,i2,i3)=-d8
               coeff(m3(-1,-1, 0),i1,i2,i3)=d64
               coeff(m3( 0,-1, 0),i1,i2,i3)=0.
               coeff(m3(+1,-1, 0),i1,i2,i3)=-d64
               coeff(m3(+2,-1, 0),i1,i2,i3)=d8
               coeff(m3(-2, 0, 0),i1,i2,i3)=0.
               coeff(m3(-1, 0, 0),i1,i2,i3)=0.
               coeff(m3( 0, 0, 0),i1,i2,i3)=0.
               coeff(m3(+1, 0, 0),i1,i2,i3)=0.
               coeff(m3(+2, 0, 0),i1,i2,i3)=0.
               coeff(m3(-2, 1, 0),i1,i2,i3)=d8
               coeff(m3(-1, 1, 0),i1,i2,i3)=-d64
               coeff(m3( 0, 1, 0),i1,i2,i3)=0.
               coeff(m3(+1, 1, 0),i1,i2,i3)=d64
               coeff(m3(+2, 1, 0),i1,i2,i3)=-d8
               coeff(m3(-2, 2, 0),i1,i2,i3)=-d
               coeff(m3(-1, 2, 0),i1,i2,i3)=d8
               coeff(m3( 0, 2, 0),i1,i2,i3)=0.
               coeff(m3(+1, 2, 0),i1,i2,i3)=-d8
               coeff(m3(+2, 2, 0),i1,i2,i3)=d
               coeff(m3(-2,-2, 1),i1,i2,i3)=0.
               coeff(m3(-1,-2, 1),i1,i2,i3)=0.
               coeff(m3( 0,-2, 1),i1,i2,i3)=0.
               coeff(m3(+1,-2, 1),i1,i2,i3)=0.
               coeff(m3(+2,-2, 1),i1,i2,i3)=0.
               coeff(m3(-2,-1, 1),i1,i2,i3)=0.
               coeff(m3(-1,-1, 1),i1,i2,i3)=0.
               coeff(m3( 0,-1, 1),i1,i2,i3)=0.
               coeff(m3(+1,-1, 1),i1,i2,i3)=0.
               coeff(m3(+2,-1, 1),i1,i2,i3)=0.
               coeff(m3(-2, 0, 1),i1,i2,i3)=0.
               coeff(m3(-1, 0, 1),i1,i2,i3)=0.
               coeff(m3( 0, 0, 1),i1,i2,i3)=0.
               coeff(m3(+1, 0, 1),i1,i2,i3)=0.
               coeff(m3(+2, 0, 1),i1,i2,i3)=0.
               coeff(m3(-2, 1, 1),i1,i2,i3)=0.
               coeff(m3(-1, 1, 1),i1,i2,i3)=0.
               coeff(m3( 0, 1, 1),i1,i2,i3)=0.
               coeff(m3(+1, 1, 1),i1,i2,i3)=0.
               coeff(m3(+2, 1, 1),i1,i2,i3)=0.
               coeff(m3(-2, 2, 1),i1,i2,i3)=0.
               coeff(m3(-1, 2, 1),i1,i2,i3)=0.
               coeff(m3( 0, 2, 1),i1,i2,i3)=0.
               coeff(m3(+1, 2, 1),i1,i2,i3)=0.
               coeff(m3(+2, 2, 1),i1,i2,i3)=0.
               coeff(m3(-2,-2, 2),i1,i2,i3)=0.
               coeff(m3(-1,-2, 2),i1,i2,i3)=0.
               coeff(m3( 0,-2, 2),i1,i2,i3)=0.
               coeff(m3(+1,-2, 2),i1,i2,i3)=0.
               coeff(m3(+2,-2, 2),i1,i2,i3)=0.
               coeff(m3(-2,-1, 2),i1,i2,i3)=0.
               coeff(m3(-1,-1, 2),i1,i2,i3)=0.
               coeff(m3( 0,-1, 2),i1,i2,i3)=0.
               coeff(m3(+1,-1, 2),i1,i2,i3)=0.
               coeff(m3(+2,-1, 2),i1,i2,i3)=0.
               coeff(m3(-2, 0, 2),i1,i2,i3)=0.
               coeff(m3(-1, 0, 2),i1,i2,i3)=0.
               coeff(m3( 0, 0, 2),i1,i2,i3)=0.
               coeff(m3(+1, 0, 2),i1,i2,i3)=0.
               coeff(m3(+2, 0, 2),i1,i2,i3)=0.
               coeff(m3(-2,+1, 2),i1,i2,i3)=0.
               coeff(m3(-1,+1, 2),i1,i2,i3)=0.
               coeff(m3( 0,+1, 2),i1,i2,i3)=0.
               coeff(m3(+1,+1, 2),i1,i2,i3)=0.
               coeff(m3(+2,+1, 2),i1,i2,i3)=0.
               coeff(m3(-2,+2, 2),i1,i2,i3)=0.
               coeff(m3(-1,+2, 2),i1,i2,i3)=0.
               coeff(m3( 0,+2, 2),i1,i2,i3)=0.
               coeff(m3(+1,+2, 2),i1,i2,i3)=0.
               coeff(m3(+2,+2, 2),i1,i2,i3)=0.
! endLoops()
           end do
           end do
           end do
           end do
           end do
         else
       !  ***** not rectangular *****
! beginLoops6()
           ! ***** loop over equations and components *****
           do e=ea,eb
           do c=ca,cb
           ec=ns*(c+nc*e)
           ! ** it did not affect performance to use an array to index coeff ***
           if( nd.eq.2 )then
           do i2=-halfWidth,halfWidth
             do i1=-halfWidth,halfWidth
              m(i1,i2)=i1+halfWidth+width*(i2+halfWidth) +1 + ec
             end do
           end do
           else if( nd.eq.3 )then
           do i3=-halfWidth,halfWidth
             do i2=-halfWidth,halfWidth
               do i1=-halfWidth,halfWidth
                 m3(i1,i2,i3)=i1+halfWidth+width*(i2+halfWidth+width*(
     & i3+halfWidth)) +1 + ec
               end do
             end do
           end do
           else
           m12=1+ec
           m22=2+ec
           m32=3+ec
           m42=4+ec
           m52=5+ec
           endif
           do i3=n3a,n3b
           do i2=n2a,n2b
           do i1=n1a,n1b
!      #If "xy" == "laplacian"
!      #Elif "xy" == "x"
!      #Elif "xy" == "y"
!      #Elif "xy" == "z"
!      #Elif "xy" == "xx"
!      #Elif "xy" == "yy"
!      #Elif "xy" == "zz"
!      #Elif "xy" == "xy"
! xy6thOrder3d(x,y)
              rxsq = d24(1)*(r x(i1,i2,i3)*r y(i1,i2,i3))
              rxxyy=d14(1)*(r x y 3(i1,i2,i3))
              sxsq = d24(2)*(s x(i1,i2,i3)*s y(i1,i2,i3))
              sxxyy=d14(2)*(s x y 3(i1,i2,i3))
              txsq = d24(3)*(t x(i1,i2,i3)*t y(i1,i2,i3))
              txxyy=d14(3)*(t x y 3(i1,i2,i3))
              rsx = (d14(1)*d14(2))*(r x(i1,i2,i3)*s y(i1,i2,i3)+r y(
     & i1,i2,i3)*s x(i1,i2,i3))
              rtx = (d14(1)*d14(3))*(r x(i1,i2,i3)*t y(i1,i2,i3)+r y(
     & i1,i2,i3)*t x(i1,i2,i3))
              stx = (d14(2)*d14(3))*(s x(i1,i2,i3)*t y(i1,i2,i3)+s y(
     & i1,i2,i3)*t x(i1,i2,i3))
              rsx8  = rsx*8.
              rsx64 = rsx*64.
              rtx8  = rtx*8.
              rtx64 = rtx*64.
              stx8  = stx*8.
              stx64 = stx*64.
! loopBody6thOrder3d(0.,0.,stx,0.,0., 0.,0.,-stx8,0.,0., rtx,-rtx8,-txSq+txxyy,rtx8,-rtx, 0.,0.,stx8,0.,0., 0.,0.,-stx,0.,0.,0.,0.,-stx8,0.,0., 0.,0.,stx64,0.,0., -rtx8,rtx64,16.*txSq-8.*txxyy,-rtx64,rtx8, 0.,0.,-stx64,0.,0., 0.,0.,stx8,0.,0.,rsx,-rsx8,-sxSq+sxxyy,rsx8,-rsx,-rsx8,rsx64,16.*sxSq-8.*sxxyy,-rsx64,rsx8, -rxSq+rxxyy,16.*rxSq-8.*rxxyy,-30.*(rxSq+sxSq+txSq),16.*rxSq+8.*rxxyy,-rxSq-rxxyy, rsx8,-rsx64,16.*sxSq+8.*sxxyy,rsx64,-rsx8, -rsx,rsx8,-sxSq-sxxyy,-rsx8,rsx,0.,0.,stx8,0.,0., 0.,0.,-stx64,0.,0., rtx8,-rtx64,16.*txSq+8.*txxyy,rtx64,-rtx8, 0.,0.,stx64,0.,0., 0.,0.,-stx8,0.,0.,0.,0.,-stx,0.,0., 0.,0.,stx8,0.,0., -rtx,rtx8,-txSq-txxyy,-rtx8,rtx, 0.,0.,-stx8,0.,0., 0.,0.,stx,0.,0.)
               coeff(m3(-2,-2,-2),i1,i2,i3)=0.
               coeff(m3(-1,-2,-2),i1,i2,i3)=0.
               coeff(m3( 0,-2,-2),i1,i2,i3)=stx
               coeff(m3( 1,-2,-2),i1,i2,i3)=0.
               coeff(m3( 2,-2,-2),i1,i2,i3)=0.
               coeff(m3(-2,-1,-2),i1,i2,i3)=0.
               coeff(m3(-1,-1,-2),i1,i2,i3)=0.
               coeff(m3( 0,-1,-2),i1,i2,i3)=-stx8
               coeff(m3( 1,-1,-2),i1,i2,i3)=0.
               coeff(m3( 2,-1,-2),i1,i2,i3)=0.
               coeff(m3(-2, 0,-2),i1,i2,i3)=rtx
               coeff(m3(-1, 0,-2),i1,i2,i3)=-rtx8
               coeff(m3( 0, 0,-2),i1,i2,i3)=-txSq+txxyy
               coeff(m3(+1, 0,-2),i1,i2,i3)=rtx8
               coeff(m3(+2, 0,-2),i1,i2,i3)=-rtx
               coeff(m3(-2, 1,-2),i1,i2,i3)=0.
               coeff(m3(-1, 1,-2),i1,i2,i3)=0.
               coeff(m3( 0, 1,-2),i1,i2,i3)=stx8
               coeff(m3( 1, 1,-2),i1,i2,i3)=0.
               coeff(m3( 2, 1,-2),i1,i2,i3)=0.
               coeff(m3(-2, 2,-2),i1,i2,i3)=0.
               coeff(m3(-1, 2,-2),i1,i2,i3)=0.
               coeff(m3( 0, 2,-2),i1,i2,i3)=-stx
               coeff(m3( 1, 2,-2),i1,i2,i3)=0.
               coeff(m3( 2, 2,-2),i1,i2,i3)=0.
               coeff(m3(-2,-2,-1),i1,i2,i3)=0.
               coeff(m3(-1,-2,-1),i1,i2,i3)=0.
               coeff(m3( 0,-2,-1),i1,i2,i3)=-stx8
               coeff(m3(+1,-2,-1),i1,i2,i3)=0.
               coeff(m3(+2,-2,-1),i1,i2,i3)=0.
               coeff(m3(-2,-1,-1),i1,i2,i3)=0.
               coeff(m3(-1,-1,-1),i1,i2,i3)=0.
               coeff(m3( 0,-1,-1),i1,i2,i3)=stx64
               coeff(m3(+1,-1,-1),i1,i2,i3)=0.
               coeff(m3(+2,-1,-1),i1,i2,i3)=0.
               coeff(m3(-2, 0,-1),i1,i2,i3)=-rtx8
               coeff(m3(-1, 0,-1),i1,i2,i3)=rtx64
               coeff(m3( 0, 0,-1),i1,i2,i3)=16.*txSq-8.*txxyy
               coeff(m3(+1, 0,-1),i1,i2,i3)=-rtx64
               coeff(m3(+2, 0,-1),i1,i2,i3)=rtx8
               coeff(m3(-2,+1,-1),i1,i2,i3)=0.
               coeff(m3(-1,+1,-1),i1,i2,i3)=0.
               coeff(m3( 0,+1,-1),i1,i2,i3)=-stx64
               coeff(m3(+1,+1,-1),i1,i2,i3)=0.
               coeff(m3(+2,+1,-1),i1,i2,i3)=0.
               coeff(m3(-2,+2,-1),i1,i2,i3)=0.
               coeff(m3(-1,+2,-1),i1,i2,i3)=0.
               coeff(m3( 0,+2,-1),i1,i2,i3)=stx8
               coeff(m3(+1,+2,-1),i1,i2,i3)=0.
               coeff(m3(+2,+2,-1),i1,i2,i3)=0.
               coeff(m3(-2,-2, 0),i1,i2,i3)=rsx
               coeff(m3(-1,-2, 0),i1,i2,i3)=-rsx8
               coeff(m3( 0,-2, 0),i1,i2,i3)=-sxSq+sxxyy
               coeff(m3(+1,-2, 0),i1,i2,i3)=rsx8
               coeff(m3(+2,-2, 0),i1,i2,i3)=-rsx
               coeff(m3(-2,-1, 0),i1,i2,i3)=-rsx8
               coeff(m3(-1,-1, 0),i1,i2,i3)=rsx64
               coeff(m3( 0,-1, 0),i1,i2,i3)=16.*sxSq-8.*sxxyy
               coeff(m3(+1,-1, 0),i1,i2,i3)=-rsx64
               coeff(m3(+2,-1, 0),i1,i2,i3)=rsx8
               coeff(m3(-2, 0, 0),i1,i2,i3)=-rxSq+rxxyy
               coeff(m3(-1, 0, 0),i1,i2,i3)=16.*rxSq-8.*rxxyy
               coeff(m3( 0, 0, 0),i1,i2,i3)=-30.*(rxSq+sxSq+txSq)
               coeff(m3(+1, 0, 0),i1,i2,i3)=16.*rxSq+8.*rxxyy
               coeff(m3(+2, 0, 0),i1,i2,i3)=-rxSq-rxxyy
               coeff(m3(-2, 1, 0),i1,i2,i3)=rsx8
               coeff(m3(-1, 1, 0),i1,i2,i3)=-rsx64
               coeff(m3( 0, 1, 0),i1,i2,i3)=16.*sxSq+8.*sxxyy
               coeff(m3(+1, 1, 0),i1,i2,i3)=rsx64
               coeff(m3(+2, 1, 0),i1,i2,i3)=-rsx8
               coeff(m3(-2, 2, 0),i1,i2,i3)=-rsx
               coeff(m3(-1, 2, 0),i1,i2,i3)=rsx8
               coeff(m3( 0, 2, 0),i1,i2,i3)=-sxSq-sxxyy
               coeff(m3(+1, 2, 0),i1,i2,i3)=-rsx8
               coeff(m3(+2, 2, 0),i1,i2,i3)=rsx
               coeff(m3(-2,-2, 1),i1,i2,i3)=0.
               coeff(m3(-1,-2, 1),i1,i2,i3)=0.
               coeff(m3( 0,-2, 1),i1,i2,i3)=stx8
               coeff(m3(+1,-2, 1),i1,i2,i3)=0.
               coeff(m3(+2,-2, 1),i1,i2,i3)=0.
               coeff(m3(-2,-1, 1),i1,i2,i3)=0.
               coeff(m3(-1,-1, 1),i1,i2,i3)=0.
               coeff(m3( 0,-1, 1),i1,i2,i3)=-stx64
               coeff(m3(+1,-1, 1),i1,i2,i3)=0.
               coeff(m3(+2,-1, 1),i1,i2,i3)=0.
               coeff(m3(-2, 0, 1),i1,i2,i3)=rtx8
               coeff(m3(-1, 0, 1),i1,i2,i3)=-rtx64
               coeff(m3( 0, 0, 1),i1,i2,i3)=16.*txSq+8.*txxyy
               coeff(m3(+1, 0, 1),i1,i2,i3)=rtx64
               coeff(m3(+2, 0, 1),i1,i2,i3)=-rtx8
               coeff(m3(-2, 1, 1),i1,i2,i3)=0.
               coeff(m3(-1, 1, 1),i1,i2,i3)=0.
               coeff(m3( 0, 1, 1),i1,i2,i3)=stx64
               coeff(m3(+1, 1, 1),i1,i2,i3)=0.
               coeff(m3(+2, 1, 1),i1,i2,i3)=0.
               coeff(m3(-2, 2, 1),i1,i2,i3)=0.
               coeff(m3(-1, 2, 1),i1,i2,i3)=0.
               coeff(m3( 0, 2, 1),i1,i2,i3)=-stx8
               coeff(m3(+1, 2, 1),i1,i2,i3)=0.
               coeff(m3(+2, 2, 1),i1,i2,i3)=0.
               coeff(m3(-2,-2, 2),i1,i2,i3)=0.
               coeff(m3(-1,-2, 2),i1,i2,i3)=0.
               coeff(m3( 0,-2, 2),i1,i2,i3)=-stx
               coeff(m3(+1,-2, 2),i1,i2,i3)=0.
               coeff(m3(+2,-2, 2),i1,i2,i3)=0.
               coeff(m3(-2,-1, 2),i1,i2,i3)=0.
               coeff(m3(-1,-1, 2),i1,i2,i3)=0.
               coeff(m3( 0,-1, 2),i1,i2,i3)=stx8
               coeff(m3(+1,-1, 2),i1,i2,i3)=0.
               coeff(m3(+2,-1, 2),i1,i2,i3)=0.
               coeff(m3(-2, 0, 2),i1,i2,i3)=-rtx
               coeff(m3(-1, 0, 2),i1,i2,i3)=rtx8
               coeff(m3( 0, 0, 2),i1,i2,i3)=-txSq-txxyy
               coeff(m3(+1, 0, 2),i1,i2,i3)=-rtx8
               coeff(m3(+2, 0, 2),i1,i2,i3)=rtx
               coeff(m3(-2,+1, 2),i1,i2,i3)=0.
               coeff(m3(-1,+1, 2),i1,i2,i3)=0.
               coeff(m3( 0,+1, 2),i1,i2,i3)=-stx8
               coeff(m3(+1,+1, 2),i1,i2,i3)=0.
               coeff(m3(+2,+1, 2),i1,i2,i3)=0.
               coeff(m3(-2,+2, 2),i1,i2,i3)=0.
               coeff(m3(-1,+2, 2),i1,i2,i3)=0.
               coeff(m3( 0,+2, 2),i1,i2,i3)=stx
               coeff(m3(+1,+2, 2),i1,i2,i3)=0.
               coeff(m3(+2,+2, 2),i1,i2,i3)=0.
! endLoops()
           end do
           end do
           end do
           end do
           end do
         end if
       elseif( nd.eq.1 )then
       !       ************************
       !       ******* 1D *************      
       !       ************************
!   #If "xy" == "identity"
!   #Elif "xy" == "rr"
!   #Elif "xy" == "r"
         if( gridType .eq. 0 )then
       !   rectangular
! beginLoops6()
           ! ***** loop over equations and components *****
           do e=ea,eb
           do c=ca,cb
           ec=ns*(c+nc*e)
           ! ** it did not affect performance to use an array to index coeff ***
           if( nd.eq.2 )then
           do i2=-halfWidth,halfWidth
             do i1=-halfWidth,halfWidth
              m(i1,i2)=i1+halfWidth+width*(i2+halfWidth) +1 + ec
             end do
           end do
           else if( nd.eq.3 )then
           do i3=-halfWidth,halfWidth
             do i2=-halfWidth,halfWidth
               do i1=-halfWidth,halfWidth
                 m3(i1,i2,i3)=i1+halfWidth+width*(i2+halfWidth+width*(
     & i3+halfWidth)) +1 + ec
               end do
             end do
           end do
           else
           m12=1+ec
           m22=2+ec
           m32=3+ec
           m42=4+ec
           m52=5+ec
           endif
           do i3=n3a,n3b
           do i2=n2a,n2b
           do i1=n1a,n1b
!      #If "xy" == "laplacian" || "xy" == "xx"
!      #Elif "xy" == "x"
! endLoops()
           end do
           end do
           end do
           end do
           end do
         else
       !  ***** not rectangular *****
! beginLoops6()
           ! ***** loop over equations and components *****
           do e=ea,eb
           do c=ca,cb
           ec=ns*(c+nc*e)
           ! ** it did not affect performance to use an array to index coeff ***
           if( nd.eq.2 )then
           do i2=-halfWidth,halfWidth
             do i1=-halfWidth,halfWidth
              m(i1,i2)=i1+halfWidth+width*(i2+halfWidth) +1 + ec
             end do
           end do
           else if( nd.eq.3 )then
           do i3=-halfWidth,halfWidth
             do i2=-halfWidth,halfWidth
               do i1=-halfWidth,halfWidth
                 m3(i1,i2,i3)=i1+halfWidth+width*(i2+halfWidth+width*(
     & i3+halfWidth)) +1 + ec
               end do
             end do
           end do
           else
           m12=1+ec
           m22=2+ec
           m32=3+ec
           m42=4+ec
           m52=5+ec
           endif
           do i3=n3a,n3b
           do i2=n2a,n2b
           do i1=n1a,n1b
!      #If "xy" == "laplacian" || "xy" == "xx"
!      #Elif "xy" == "x"
! endLoops()
           end do
           end do
           end do
           end do
           end do
         end if
         else if( nd.eq.0 )then
       !       *** add these lines to avoid warnings about unused statement functions
           include "cgux4afNoWarnings.h"
           temp=rxx1(i1,i2,i3)
         end if
       return
       end
