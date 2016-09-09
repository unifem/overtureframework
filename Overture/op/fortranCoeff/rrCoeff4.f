! This file automatically generated from opcoeff.bf with bpp.
! coeffOperator4thOrder(rr)
       subroutine rrCoeff4thOrder( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,nds1a,nds1b,nds2a,nds2b,
     & nds3a,nds3b, n1a,n1b,n2a,n2b,n3a,n3b, ndc,nc,ns,ea,eb,ca,cb,dx,
     & dr, rsxy,coeff, derivOption, derivType, gridType, order, s, 
     & jac, averagingType, dir1, dir2,a11,a22,a12,a21,a33,a13,a23,a31,
     & a32 )
       ! ===============================================================
       !  Derivative Coefficients - 4th order version
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
       ! real rx,ry,rz,sx,sy,sz,tx,ty,tz,d,d8,d64
       ! real rxSq,rxxyy,sxSq,sxxyy,txxyy,txSq
       ! real rxx,ryy,sxx,syy,rxx3,ryy3,rzz3,sxx3,syy3,szz3,txx3,tyy3,tzz3
       ! real rsx,rtx,stx
       ! real rxt,ryt,sxt,syt,txr,txs
       ! real txt,tyr,tys,tyt,rzr,rzs,rzt
       ! real szr,szs,szt,tzr,tzs,tzt
       ! real rxr,rxs,ryr,rys,sxr,sxs,syr,sys
       ! real rsx8,rsx64,rtx8,rtx64,stx8,stx64
       !..... added by kkc 1/2/02 for g77 unsatisfied reference
       real u(1,1,1,1)
       real d24(3),d14(3),h42(3),h41(3)
       integer i1,i2,i3,kd3,kd,kdd,e,c,ec
       integer m12,m22,m32,m42,m52
       integer m(-2:2,-2:2),m3(-2:2,-2:2,-2:2)
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
       if( order.ne.4 )then
         write(*,*) 'laplacianCoeff4:ERROR: order!=4 '
         stop
       end if
! #If "rr" == "divScalarGrad"
       do n=1,3
         d14(n)=1./(12.*dr(n))
         d24(n)=1./(12.*dr(n)**2)
         h41(n)=1./(12.*dx(n))
         h42(n)=1./(12.*dx(n)**2)
       end do
       kd3=nd
       if( nd .eq. 2 )then
       !       ************************
       !       ******* 2D *************      
       !       ************************
!   #If "rr" == "identity"
!   #Elif "rr" == "r"
!   #Elif "rr" == "s"
!   #Elif "rr" == "rr"
! beginLoops4()
           ! ***** loop over equations and components *****
           do e=ea,eb
           do c=ca,cb
           ec=ns*(c+nc*e)
           ! ** it did not affect performance to use an array to index coeff ***
           if( nd.eq.2 )then
           do i2=-2,2
             do i1=-2,2
              m(i1,i2)=i1+2+5*(i2+2) +1 + ec
             end do
           end do
           else if( nd.eq.3 )then
           do i3=-2,2
             do i2=-2,2
               do i1=-2,2
                 m3(i1,i2,i3)=i1+2+5*(i2+2+5*(i3+2)) +1 + ec
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
! rr4thOrder2dRectangular(x,1)
! loopBody4thOrder2dSwitchxx(0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,-d24(1),16.* d24(1),-30.*d24(1),16.* d24(1),-d24(1),0.,0.,0.,0.,0., 0.,0.,0.,0.,0.)
! loopBody4thOrder2d(0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,-d24(1),16.*d24(1),-30.*d24(1),16.*d24(1),-d24(1),0.,0.,0.,0.,0.,0.,0.,0.,0.,0.)
             coeff(m(-2,-2),i1,i2,i3)=0.
             coeff(m(-1,-2),i1,i2,i3)=0.
             coeff(m( 0,-2),i1,i2,i3)=0.
             coeff(m( 1,-2),i1,i2,i3)=0.
             coeff(m( 2,-2),i1,i2,i3)=0.
             coeff(m(-2,-1),i1,i2,i3)=0.
             coeff(m(-1,-1),i1,i2,i3)=0.
             coeff(m( 0,-1),i1,i2,i3)=0.
             coeff(m( 1,-1),i1,i2,i3)=0.
             coeff(m( 2,-1),i1,i2,i3)=0.
             coeff(m(-2, 0),i1,i2,i3)=-d24(1)
             coeff(m(-1, 0),i1,i2,i3)=16.*d24(1)
             coeff(m( 0, 0),i1,i2,i3)=-30.*d24(1)
             coeff(m(+1, 0),i1,i2,i3)=16.*d24(1)
             coeff(m(+2, 0),i1,i2,i3)=-d24(1)
             coeff(m(-2, 1),i1,i2,i3)=0.
             coeff(m(-1, 1),i1,i2,i3)=0.
             coeff(m( 0, 1),i1,i2,i3)=0.
             coeff(m( 1, 1),i1,i2,i3)=0.
             coeff(m( 2, 1),i1,i2,i3)=0.
             coeff(m(-2, 2),i1,i2,i3)=0.
             coeff(m(-1, 2),i1,i2,i3)=0.
             coeff(m( 0, 2),i1,i2,i3)=0.
             coeff(m( 1, 2),i1,i2,i3)=0.
             coeff(m( 2, 2),i1,i2,i3)=0.
! endLoops()
           end do
           end do
           end do
           end do
           end do
           return
         if( gridType .eq. 0 )then
       !   rectangular
! beginLoops4()
           ! ***** loop over equations and components *****
           do e=ea,eb
           do c=ca,cb
           ec=ns*(c+nc*e)
           ! ** it did not affect performance to use an array to index coeff ***
           if( nd.eq.2 )then
           do i2=-2,2
             do i1=-2,2
              m(i1,i2)=i1+2+5*(i2+2) +1 + ec
             end do
           end do
           else if( nd.eq.3 )then
           do i3=-2,2
             do i2=-2,2
               do i1=-2,2
                 m3(i1,i2,i3)=i1+2+5*(i2+2+5*(i3+2)) +1 + ec
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
!       #If "rr" == "laplacian"
!       #Elif "rr" == "x"
!       #Elif "rr" == "y"
!       #Elif "rr" == "xx"
!       #Elif "rr" == "yy"
!       #Elif "rr" == "xy"
! endLoops()
           end do
           end do
           end do
           end do
           end do
         else
       !  ***** not rectangular *****
! beginLoops4()
           ! ***** loop over equations and components *****
           do e=ea,eb
           do c=ca,cb
           ec=ns*(c+nc*e)
           ! ** it did not affect performance to use an array to index coeff ***
           if( nd.eq.2 )then
           do i2=-2,2
             do i1=-2,2
              m(i1,i2)=i1+2+5*(i2+2) +1 + ec
             end do
           end do
           else if( nd.eq.3 )then
           do i3=-2,2
             do i2=-2,2
               do i1=-2,2
                 m3(i1,i2,i3)=i1+2+5*(i2+2+5*(i3+2)) +1 + ec
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
!       #If "rr" == "laplacian"
!       #Elif "rr" == "x"
!       #Elif "rr" == "y"
!       #Elif "rr" == "xx"
!       #Elif "rr" == "yy"
!      #Elif "rr" == "xy"
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
!   #If "rr" == "identity"
!   #Elif "rr" == "r"
!   #Elif "rr" == "s"
!   #Elif "rr" == "t"
!   #Elif "rr" == "rr"
! beginLoops4()
           ! ***** loop over equations and components *****
           do e=ea,eb
           do c=ca,cb
           ec=ns*(c+nc*e)
           ! ** it did not affect performance to use an array to index coeff ***
           if( nd.eq.2 )then
           do i2=-2,2
             do i1=-2,2
              m(i1,i2)=i1+2+5*(i2+2) +1 + ec
             end do
           end do
           else if( nd.eq.3 )then
           do i3=-2,2
             do i2=-2,2
               do i1=-2,2
                 m3(i1,i2,i3)=i1+2+5*(i2+2+5*(i3+2)) +1 + ec
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
! rr4thOrder3dRectangular(x,1)
! loopBody4thOrder3dSwitchxx(0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,0.,0.,0.,0.,0., 0.,0.,0.,0.,0., -d24(1),16.* d24(1),-30.*d24(1),16.* d24(1),-d24(1),0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.)
! loopBody4thOrder3d(0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,-d24(1),16.*d24(1),-30.*d24(1),16.*d24(1),-d24(1), 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0., 0.,0.,0.,0.,0.,0.,0.,0.,0.,0.)
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
             coeff(m3(-2,-2, 0),i1,i2,i3)=0.
             coeff(m3(-1,-2, 0),i1,i2,i3)=0.
             coeff(m3( 0,-2, 0),i1,i2,i3)=0.
             coeff(m3(+1,-2, 0),i1,i2,i3)=0.
             coeff(m3(+2,-2, 0),i1,i2,i3)=0.
             coeff(m3(-2,-1, 0),i1,i2,i3)=0.
             coeff(m3(-1,-1, 0),i1,i2,i3)=0.
             coeff(m3( 0,-1, 0),i1,i2,i3)=0.
             coeff(m3(+1,-1, 0),i1,i2,i3)=0.
             coeff(m3(+2,-1, 0),i1,i2,i3)=0.
             coeff(m3(-2, 0, 0),i1,i2,i3)=-d24(1)
             coeff(m3(-1, 0, 0),i1,i2,i3)=16.*d24(1)
             coeff(m3( 0, 0, 0),i1,i2,i3)=-30.*d24(1)
             coeff(m3(+1, 0, 0),i1,i2,i3)=16.*d24(1)
             coeff(m3(+2, 0, 0),i1,i2,i3)=-d24(1)
             coeff(m3(-2, 1, 0),i1,i2,i3)=0.
             coeff(m3(-1, 1, 0),i1,i2,i3)=0.
             coeff(m3( 0, 1, 0),i1,i2,i3)=0.
             coeff(m3(+1, 1, 0),i1,i2,i3)=0.
             coeff(m3(+2, 1, 0),i1,i2,i3)=0.
             coeff(m3(-2, 2, 0),i1,i2,i3)=0.
             coeff(m3(-1, 2, 0),i1,i2,i3)=0.
             coeff(m3( 0, 2, 0),i1,i2,i3)=0.
             coeff(m3(+1, 2, 0),i1,i2,i3)=0.
             coeff(m3(+2, 2, 0),i1,i2,i3)=0.
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
           return
         if( gridType .eq. 0 )then
       !   rectangular
! beginLoops4()
           ! ***** loop over equations and components *****
           do e=ea,eb
           do c=ca,cb
           ec=ns*(c+nc*e)
           ! ** it did not affect performance to use an array to index coeff ***
           if( nd.eq.2 )then
           do i2=-2,2
             do i1=-2,2
              m(i1,i2)=i1+2+5*(i2+2) +1 + ec
             end do
           end do
           else if( nd.eq.3 )then
           do i3=-2,2
             do i2=-2,2
               do i1=-2,2
                 m3(i1,i2,i3)=i1+2+5*(i2+2+5*(i3+2)) +1 + ec
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
!      #If "rr" == "laplacian"
!      #Elif "rr" == "x"
!      #Elif "rr" == "y"
!      #Elif "rr" == "z"
!      #Elif "rr" == "xx"
!      #Elif "rr" == "yy"
!      #Elif "rr" == "zz"
!      #Elif "rr" == "xy"
!      #Elif "rr" == "xz"
!      #Elif "rr" == "yz"
! endLoops()
           end do
           end do
           end do
           end do
           end do
         else
       !  ***** not rectangular *****
! beginLoops4()
           ! ***** loop over equations and components *****
           do e=ea,eb
           do c=ca,cb
           ec=ns*(c+nc*e)
           ! ** it did not affect performance to use an array to index coeff ***
           if( nd.eq.2 )then
           do i2=-2,2
             do i1=-2,2
              m(i1,i2)=i1+2+5*(i2+2) +1 + ec
             end do
           end do
           else if( nd.eq.3 )then
           do i3=-2,2
             do i2=-2,2
               do i1=-2,2
                 m3(i1,i2,i3)=i1+2+5*(i2+2+5*(i3+2)) +1 + ec
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
!      #If "rr" == "laplacian"
!      #Elif "rr" == "x"
!      #Elif "rr" == "y"
!      #Elif "rr" == "z"
!      #Elif "rr" == "xx"
!      #Elif "rr" == "yy"
!      #Elif "rr" == "zz"
!      #Elif "rr" == "xy"
!      #Elif "rr" == "xz"
!      #Elif "rr" == "yz"
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
!   #If "rr" == "identity"
!   #Elif "rr" == "rr"
! beginLoops4()
           ! ***** loop over equations and components *****
           do e=ea,eb
           do c=ca,cb
           ec=ns*(c+nc*e)
           ! ** it did not affect performance to use an array to index coeff ***
           if( nd.eq.2 )then
           do i2=-2,2
             do i1=-2,2
              m(i1,i2)=i1+2+5*(i2+2) +1 + ec
             end do
           end do
           else if( nd.eq.3 )then
           do i3=-2,2
             do i2=-2,2
               do i1=-2,2
                 m3(i1,i2,i3)=i1+2+5*(i2+2+5*(i3+2)) +1 + ec
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
! loopBody4thOrder1d(-d24(1),16.* d24(1),-30.*d24(1),16.* d24(1),-d24(1))
              coeff(m12,i1,i2,i3)=-d24(1)
              coeff(m22,i1,i2,i3)=16.*d24(1)
              coeff(m32,i1,i2,i3)=-30.*d24(1)
              coeff(m42,i1,i2,i3)=16.*d24(1)
              coeff(m52,i1,i2,i3)=-d24(1)
! endLoops()
           end do
           end do
           end do
           end do
           end do
           return
         if( gridType .eq. 0 )then
       !   rectangular
! beginLoops4()
           ! ***** loop over equations and components *****
           do e=ea,eb
           do c=ca,cb
           ec=ns*(c+nc*e)
           ! ** it did not affect performance to use an array to index coeff ***
           if( nd.eq.2 )then
           do i2=-2,2
             do i1=-2,2
              m(i1,i2)=i1+2+5*(i2+2) +1 + ec
             end do
           end do
           else if( nd.eq.3 )then
           do i3=-2,2
             do i2=-2,2
               do i1=-2,2
                 m3(i1,i2,i3)=i1+2+5*(i2+2+5*(i3+2)) +1 + ec
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
!      #If "rr" == "laplacian" || "rr" == "xx"
!      #Elif "rr" == "x"
! endLoops()
           end do
           end do
           end do
           end do
           end do
         else
       !  ***** not rectangular *****
! beginLoops4()
           ! ***** loop over equations and components *****
           do e=ea,eb
           do c=ca,cb
           ec=ns*(c+nc*e)
           ! ** it did not affect performance to use an array to index coeff ***
           if( nd.eq.2 )then
           do i2=-2,2
             do i1=-2,2
              m(i1,i2)=i1+2+5*(i2+2) +1 + ec
             end do
           end do
           else if( nd.eq.3 )then
           do i3=-2,2
             do i2=-2,2
               do i1=-2,2
                 m3(i1,i2,i3)=i1+2+5*(i2+2+5*(i3+2)) +1 + ec
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
!      #If "rr" == "laplacian" || "rr" == "xx"
!      #Elif "rr" == "x"
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
