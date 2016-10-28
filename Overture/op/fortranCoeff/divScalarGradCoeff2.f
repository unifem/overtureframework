! This file automatically generated from opcoeff.bf with bpp.
! coeffOperator2ndOrder(divScalarGrad)
       subroutine divScalarGradCoeff2ndOrder( nd, nd1a,nd1b,nd2a,nd2b,
     & nd3a,nd3b,ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b, nds1a,nds1b,
     & nds2a,nds2b,nds3a,nds3b,n1a,n1b,n2a,n2b,n3a,n3b, ndc, nc, ns, 
     & ea,eb, ca,cb,dx,dr, rsxy,coeff, derivOption, derivType, 
     & gridType, order, s, jac, averagingType, dir1, dir2,a11,a22,a12,
     & a21,a33,a13,a23,a31,a32 )
       ! ===============================================================
       !  Derivative Coefficients
       !  
       !  nd : number of range spatial dimensions 
       !  nd1a,nd1b : mesh dimensions axis 1
       !  nd2a,nd2b : mesh dimensions axis 2
       !  nd3a,nd3b : mesh dimensions axis 3
       !
       !  ndc : number of coefficients/mesh point
       !  nc1a,nd1b : coefficient array dimensions axis 1
       !  nc2a,nd2b : coefficient array dimensions axis 2
       !  nc3a,nd3b : coefficient array dimensions axis 3
       !
       !  nc1a,nd1b : subset for evaluating divScalarGrad, axis 1
       !  nc2a,nd2b : subset for evaluating divScalarGrad, axis 2
       !  nc3a,nd3b : subset for evaluating divScalarGrad, axis 3
       !
       !  nc : number of components
       !  ns : stencil size
       !  ca,cb : assign components c=ca,..,cb (base 0)
       !  ea,eb : assign equations e=ea,..eb   (base 0)
       !
       !  d11 : 1/dr
       !
       !  h11 : 1/h    :  for rectangular   
       !
       !  rsxy : jacobian information, not used if rectangular
       !  coeff : coefficient matrix
       !  gridType: 0=rectangular, 1=non-rectangular
       !  order : 2 or 4
       ! nc : number of components
       ! ns : stencil size
       ! ca,cb : assign components c=ca,..,cb (base 0)
       ! ea,eb : assign equations e=ea,..eb   (base 0)
       ! gridType: 0=rectangular, 1=non-rectangular
       ! order : 2 or 4
       ! rsxy : not used if rectangular
       ! ===============================================================
       !      implicit none
       integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,n1a,n1b,n2a,n2b,n3a,
     & n3b, ndc, nc,ns, ca,cb,ea,eb, gridType, order
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
       ! real rx,ry,rz,sx,sy,sz,tx,ty,tz,d
       ! real rxSq,rxx,sxSq,sxx,rsx,rxx2,ryy2,sxx2,syy2
       ! real rxt2,ryt2,rzz23,sxt2,syt2,szz23,txr2,txs2
       ! real txt2,tyr2,tys2,tyt2,tzz23,rzr2,rzs2,rzt2
       ! real szr2,szs2,szt2,tzr2,tzs2,tzt2
       ! real rxr2,rxs2,ryr2,rys2,sxr2,sxs2,syr2,sys2
       ! real txx,txSq,rtx,stx,rxx23,ryy23,sxx23,syy23,txx23,tyy23
       !..... added by kkc 1/2/02 for g77 unsatisfied reference
       real u(1,1,1,1)
       real h21(3),d22(3),d12(3),h22(3)
       integer i1,i2,i3,kd3,kd,c,e,ec
       integer m12,m22,m32
       integer m(-1:1,-1:1),m3(-1:1,-1:1,-1:1)
       integer laplace,divScalarGrad,derivativeScalarDerivative
       parameter(laplace=0,divScalarGrad=1,
     & derivativeScalarDerivative=2)
       integer arithmeticAverage,harmonicAverage
       parameter( arithmeticAverage=0,harmonicAverage=1 )
       integer symmetric
       parameter( symmetric=2 )
       !.......statement functions for jacobian
       rx(i1,i2,i3)=rsxy(i1,i2,i3,  1,  1)
       ry(i1,i2,i3)=rsxy(i1,i2,i3,  1,  2)
       rz(i1,i2,i3)=rsxy(i1,i2,i3,  1,kd3)
       sx(i1,i2,i3)=rsxy(i1,i2,i3,  2,  1)
       sy(i1,i2,i3)=rsxy(i1,i2,i3,  2,  2)
       sz(i1,i2,i3)=rsxy(i1,i2,i3,  2,kd3)
       tx(i1,i2,i3)=rsxy(i1,i2,i3,kd3,  1)
       ty(i1,i2,i3)=rsxy(i1,i2,i3,kd3,  2)
       tz(i1,i2,i3)=rsxy(i1,i2,i3,kd3,kd3)
       include 'cgux2af.h'
       rxx1(i1,i2,i3)=rx(i1,i2,i3)*rxr2(i1,i2,i3)
       !.....end statement functions
       if( order.ne.2 )then
         write(*,*) 'laplacianCoeff:ERROR: order!=2 '
         stop
       end if
       do n=1,3
         d12(n)=1./(2.*dr(n))
         d22(n)=1./(dr(n)**2)
         h21(n)=1./(2.*dx(n))
         h22(n)=1./(dx(n)**2)
       end do
       kd3=nd
       if( nd .eq. 2 )then
       !       ************************
       !       ******* 2D *************      
       !       ************************
!   #If "divScalarGrad" == "identity"
!   #Elif "divScalarGrad" == "r"
!   #Elif "divScalarGrad" == "s"
!   #Elif "divScalarGrad" == "rr"
!   #Elif "divScalarGrad" == "ss"
!   #Elif "divScalarGrad" == "rs"
         if( gridType .eq. 0 )then
       !   rectangular
!     #If "divScalarGrad" == "divScalarGrad"
! defineA22R()
             m1a=n1a-1
             m1b=n1b+1
             m2a=n2a-1
             m2b=n2b+1
             m3a=n3a
             m3b=n3b
             if( averagingType .eq. arithmeticAverage )then
               factor=.5
               if( derivOption.eq.divScalarGrad  )then
! loopsDSG1(a11(j1,j2,j3) = factor*h22(1)*(s(j1,j2,j3,0)+s(j1-1,j2,j3,0)))
                 m1a=n1a
                 do j3=m3a,m3b
                   do j2=m2a,m2b
                     do j1=m1a,m1b
                       a11(j1,j2,j3)=factor*h22(1)*(s(j1,j2,j3,0)+s(j1-
     & 1,j2,j3,0))
                     end do
                   end do
                 end do
                 m1a=n1a-1
! loopsDSG2(a22(j1,j2,j3) = factor*h22(2)*(s(j1,j2,j3,0)+s(j1,j2-1,j3,0)))
                 m2a=n2a
                 do j3=m3a,m3b
                   do j2=m2a,m2b
                     do j1=m1a,m1b
                       a22(j1,j2,j3)=factor*h22(2)*(s(j1,j2,j3,0)+s(j1,
     & j2-1,j3,0))
                     end do
                   end do
                 end do
                 m2a=n2a-1
               else if( derivOption.eq.divTensorGrad )then
                 ! form the coefficients and average
                 !   we need to worry about the end points m1a=n1a-1 *wdh* 060210
                 do j3=m3a,m3b
                 do j2=m2a,m2b
                   j2m1=max(j2-1,m2a)
                 do j1=m1a,m1b
                   j1m1=max(j1-1,m1a)
                   a11(j1,j2,j3) = .5*(s(j1,j2,j3,0)+s(j1m1,j2,j3,0))
     & /dx(1)**2
                   a21(j1,j2,j3) = s(j1,j2,j3,1)/(4.*dx(1)*dx(2))
                   a12(j1,j2,j3) = s(j1,j2,j3,2)/(4.*dx(1)*dx(2))
                   a22(j1,j2,j3) = .5*(s(j1,j2,j3,3)+s(j1,j2m1,j3,3))
     & /dx(2)**2
                 end do
                 end do
                 end do
               else if( derivOption.eq.derivativeScalarDerivative )then
                 if( dir1.eq.dir2 )then
                   hh=h22(dir1+1)
                 else
                   hh=h21(dir1+1)*h21(dir2+1)
                 end if
                 if( dir1.eq.0 )then
! loopsDSG1(a11(j1,j2,j3) = factor*hh*(s(j1,j2,j3,0)+s(j1-1,j2,j3,0)))
                   m1a=n1a
                   do j3=m3a,m3b
                     do j2=m2a,m2b
                       do j1=m1a,m1b
                         a11(j1,j2,j3)=factor*hh*(s(j1,j2,j3,0)+s(j1-1,
     & j2,j3,0))
                       end do
                     end do
                   end do
                   m1a=n1a-1
                 else
! loopsDSG2(a11(j1,j2,j3) = factor*hh*(s(j1,j2,j3,0)+s(j1,j2-1,j3,0)))
                   m2a=n2a
                   do j3=m3a,m3b
                     do j2=m2a,m2b
                       do j1=m1a,m1b
                         a11(j1,j2,j3)=factor*hh*(s(j1,j2,j3,0)+s(j1,
     & j2-1,j3,0))
                       end do
                     end do
                   end do
                   m2a=n2a-1
                 end if
              else
                stop 1129
              end if
             else
c  Harmonic average
               factor=2.
               if( derivOption.eq.divScalarGrad  )then
                 ! should be worry about division by zero?
! loopsDSG1(a11(j1,j2,j3) =s(j1,j2,j3,0)*s(j1-1,j2,j3,0)*h22(1)*factor/(s(j1,j2,j3,0)+s(j1-1,j2,j3,0)))
                 m1a=n1a
                 do j3=m3a,m3b
                   do j2=m2a,m2b
                     do j1=m1a,m1b
                       a11(j1,j2,j3)=s(j1,j2,j3,0)*s(j1-1,j2,j3,0)*h22(
     & 1)*factor/(s(j1,j2,j3,0)+s(j1-1,j2,j3,0))
                     end do
                   end do
                 end do
                 m1a=n1a-1
! loopsDSG2(a22(j1,j2,j3) =s(j1,j2,j3,0)*s(j1,j2-1,j3,0)*h22(2)*factor/(s(j1,j2,j3,0)+s(j1,j2-1,j3,0)))
                 m2a=n2a
                 do j3=m3a,m3b
                   do j2=m2a,m2b
                     do j1=m1a,m1b
                       a22(j1,j2,j3)=s(j1,j2,j3,0)*s(j1,j2-1,j3,0)*h22(
     & 2)*factor/(s(j1,j2,j3,0)+s(j1,j2-1,j3,0))
                     end do
                   end do
                 end do
                 m2a=n2a-1
               else if( derivOption.eq.divTensorGrad )then
                 stop 2219
               else
                 if( dir1.eq.dir2 )then
                   hh=h22(dir1+1)
                 else
                   hh=h21(dir1+1)*h21(dir2+1)
                 end if
                 if( dir1.eq.0 )then
! loopsDSG1(a11(j1,j2,j3)=s(j1,j2,j3,0)*s(j1-1,j2,j3,0)*hh*factor/(s(j1,j2,j3,0)+s(j1-1,j2,j3,0)))
                   m1a=n1a
                   do j3=m3a,m3b
                     do j2=m2a,m2b
                       do j1=m1a,m1b
                         a11(j1,j2,j3)=s(j1,j2,j3,0)*s(j1-1,j2,j3,0)*
     & hh*factor/(s(j1,j2,j3,0)+s(j1-1,j2,j3,0))
                       end do
                     end do
                   end do
                   m1a=n1a-1
                 else
! loopsDSG2(a11(j1,j2,j3)=s(j1,j2,j3,0)*s(j1,j2-1,j3,0)*hh*factor/(s(j1,j2,j3,0)+s(j1,j2-1,j3,0)))
                   m2a=n2a
                   do j3=m3a,m3b
                     do j2=m2a,m2b
                       do j1=m1a,m1b
                         a11(j1,j2,j3)=s(j1,j2,j3,0)*s(j1,j2-1,j3,0)*
     & hh*factor/(s(j1,j2,j3,0)+s(j1,j2-1,j3,0))
                       end do
                     end do
                   end do
                   m2a=n2a-1
                 end if
               end if
             end if
             if( derivOption.eq.divScalarGrad )then
! beginLoops()
              ! ***** loop over equations and components *****
              do e=ea,eb
              do c=ca,cb
              ec=ns*(c+nc*e)
              ! ** it did not affect performance to use an array to index coeff ***
              if( nd.eq.2 )then
              do i2=-1,1
                do i1=-1,1
                 m(i1,i2)=i1+1+3*(i2+1) +1 + ec
                end do
              end do
              else if( nd.eq.3 )then
              do i3=-1,1
                do i2=-1,1
                  do i1=-1,1
                    m3(i1,i2,i3)=i1+1+3*(i2+1+3*(i3+1)) +1 + ec
                  end do
                end do
              end do
              else
              m12=1 + ec
              m22=2 + ec
              m32=3 + ec
              endif
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
! loopBody2ndOrder2d(0.,a22(i1,i2,i3),0., a11(i1,i2,i3),-(a11(i1+1,i2,i3)+a11(i1,i2,i3)+a22(i1,i2,i3)+a22(i1,i2+1,i3)), a11(i1+1,i2,i3),  0.,a22(i1,i2+1,i3),0.)
                coeff(m(-1,-1),i1,i2,i3)=0.
                coeff(m( 0,-1),i1,i2,i3)=a22(i1,i2,i3)
                coeff(m(+1,-1),i1,i2,i3)=0.
                coeff(m(-1, 0),i1,i2,i3)=a11(i1,i2,i3)
                coeff(m( 0, 0),i1,i2,i3)=-(a11(i1+1,i2,i3)+a11(i1,i2,
     & i3)+a22(i1,i2,i3)+a22(i1,i2+1,i3))
                coeff(m(+1, 0),i1,i2,i3)=a11(i1+1,i2,i3)
                coeff(m(-1,+1),i1,i2,i3)=0.
                coeff(m( 0,+1),i1,i2,i3)=a22(i1,i2+1,i3)
                coeff(m(+1,+1),i1,i2,i3)=0.
! endLoops()
              end do
              end do
              end do
              end do
              end do
             else if( dir1.eq.0 .and. dir2.eq.0 )then
! beginLoops()
               ! ***** loop over equations and components *****
               do e=ea,eb
               do c=ca,cb
               ec=ns*(c+nc*e)
               ! ** it did not affect performance to use an array to index coeff ***
               if( nd.eq.2 )then
               do i2=-1,1
                 do i1=-1,1
                  m(i1,i2)=i1+1+3*(i2+1) +1 + ec
                 end do
               end do
               else if( nd.eq.3 )then
               do i3=-1,1
                 do i2=-1,1
                   do i1=-1,1
                     m3(i1,i2,i3)=i1+1+3*(i2+1+3*(i3+1)) +1 + ec
                   end do
                 end do
               end do
               else
               m12=1 + ec
               m22=2 + ec
               m32=3 + ec
               endif
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
! loopBody2ndOrder2d(0.,0.,0., a11(i1,i2,i3), -(a11(i1+1,i2,i3)+a11(i1,i2,i3)), a11(i1+1,i2,i3), 0.,0.,0.)
                  coeff(m(-1,-1),i1,i2,i3)=0.
                  coeff(m( 0,-1),i1,i2,i3)=0.
                  coeff(m(+1,-1),i1,i2,i3)=0.
                  coeff(m(-1, 0),i1,i2,i3)=a11(i1,i2,i3)
                  coeff(m( 0, 0),i1,i2,i3)=-(a11(i1+1,i2,i3)+a11(i1,i2,
     & i3))
                  coeff(m(+1, 0),i1,i2,i3)=a11(i1+1,i2,i3)
                  coeff(m(-1,+1),i1,i2,i3)=0.
                  coeff(m( 0,+1),i1,i2,i3)=0.
                  coeff(m(+1,+1),i1,i2,i3)=0.
! endLoops()
               end do
               end do
               end do
               end do
               end do
             else if( dir1.eq.0 .and. dir2.eq.1 )then
! beginLoops()
               ! ***** loop over equations and components *****
               do e=ea,eb
               do c=ca,cb
               ec=ns*(c+nc*e)
               ! ** it did not affect performance to use an array to index coeff ***
               if( nd.eq.2 )then
               do i2=-1,1
                 do i1=-1,1
                  m(i1,i2)=i1+1+3*(i2+1) +1 + ec
                 end do
               end do
               else if( nd.eq.3 )then
               do i3=-1,1
                 do i2=-1,1
                   do i1=-1,1
                     m3(i1,i2,i3)=i1+1+3*(i2+1+3*(i3+1)) +1 + ec
                   end do
                 end do
               end do
               else
               m12=1 + ec
               m22=2 + ec
               m32=3 + ec
               endif
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
! loopBody2ndOrder2d(a11(i1,i2,i3),-a11(i1+1,i2,i3)+a11(i1,i2,i3),-a11(i1+1,i2,i3),0,0,0,-a11(i1,i2,i3), a11(i1+1,i2,i3)-a11(i1,i2,i3),a11(i1+1,i2,i3))
                  coeff(m(-1,-1),i1,i2,i3)=a11(i1,i2,i3)
                  coeff(m( 0,-1),i1,i2,i3)=-a11(i1+1,i2,i3)+a11(i1,i2,
     & i3)
                  coeff(m(+1,-1),i1,i2,i3)=-a11(i1+1,i2,i3)
                  coeff(m(-1, 0),i1,i2,i3)=0
                  coeff(m( 0, 0),i1,i2,i3)=0
                  coeff(m(+1, 0),i1,i2,i3)=0
                  coeff(m(-1,+1),i1,i2,i3)=-a11(i1,i2,i3)
                  coeff(m( 0,+1),i1,i2,i3)=a11(i1+1,i2,i3)-a11(i1,i2,
     & i3)
                  coeff(m(+1,+1),i1,i2,i3)=a11(i1+1,i2,i3)
! endLoops()
               end do
               end do
               end do
               end do
               end do
             else if( dir1.eq.1 .and. dir2.eq.0 )then
! beginLoops()
               ! ***** loop over equations and components *****
               do e=ea,eb
               do c=ca,cb
               ec=ns*(c+nc*e)
               ! ** it did not affect performance to use an array to index coeff ***
               if( nd.eq.2 )then
               do i2=-1,1
                 do i1=-1,1
                  m(i1,i2)=i1+1+3*(i2+1) +1 + ec
                 end do
               end do
               else if( nd.eq.3 )then
               do i3=-1,1
                 do i2=-1,1
                   do i1=-1,1
                     m3(i1,i2,i3)=i1+1+3*(i2+1+3*(i3+1)) +1 + ec
                   end do
                 end do
               end do
               else
               m12=1 + ec
               m22=2 + ec
               m32=3 + ec
               endif
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
! loopBody2ndOrder2d(a11(i1,i2,i3),0,-a11(i1,i2,i3),-a11(i1,i2+1,i3)+a11(i1,i2,i3),0, a11(i1,i2+1,i3)-a11(i1,i2,i3),-a11(i1,i2+1,i3),0,a11(i1,i2+1,i3))
                  coeff(m(-1,-1),i1,i2,i3)=a11(i1,i2,i3)
                  coeff(m( 0,-1),i1,i2,i3)=0
                  coeff(m(+1,-1),i1,i2,i3)=-a11(i1,i2,i3)
                  coeff(m(-1, 0),i1,i2,i3)=-a11(i1,i2+1,i3)+a11(i1,i2,
     & i3)
                  coeff(m( 0, 0),i1,i2,i3)=0
                  coeff(m(+1, 0),i1,i2,i3)=a11(i1,i2+1,i3)-a11(i1,i2,
     & i3)
                  coeff(m(-1,+1),i1,i2,i3)=-a11(i1,i2+1,i3)
                  coeff(m( 0,+1),i1,i2,i3)=0
                  coeff(m(+1,+1),i1,i2,i3)=a11(i1,i2+1,i3)
! endLoops()
               end do
               end do
               end do
               end do
               end do
             else if( dir1.eq.1 .and. dir2.eq.1 )then
! beginLoops()
               ! ***** loop over equations and components *****
               do e=ea,eb
               do c=ca,cb
               ec=ns*(c+nc*e)
               ! ** it did not affect performance to use an array to index coeff ***
               if( nd.eq.2 )then
               do i2=-1,1
                 do i1=-1,1
                  m(i1,i2)=i1+1+3*(i2+1) +1 + ec
                 end do
               end do
               else if( nd.eq.3 )then
               do i3=-1,1
                 do i2=-1,1
                   do i1=-1,1
                     m3(i1,i2,i3)=i1+1+3*(i2+1+3*(i3+1)) +1 + ec
                   end do
                 end do
               end do
               else
               m12=1 + ec
               m22=2 + ec
               m32=3 + ec
               endif
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
! loopBody2ndOrder2d(0,a11(i1,i2,i3),0,0,-a11(i1,i2+1,i3)-a11(i1,i2,i3),0,0,a11(i1,i2+1,i3),0)
                  coeff(m(-1,-1),i1,i2,i3)=0
                  coeff(m( 0,-1),i1,i2,i3)=a11(i1,i2,i3)
                  coeff(m(+1,-1),i1,i2,i3)=0
                  coeff(m(-1, 0),i1,i2,i3)=0
                  coeff(m( 0, 0),i1,i2,i3)=-a11(i1,i2+1,i3)-a11(i1,i2,
     & i3)
                  coeff(m(+1, 0),i1,i2,i3)=0
                  coeff(m(-1,+1),i1,i2,i3)=0
                  coeff(m( 0,+1),i1,i2,i3)=a11(i1,i2+1,i3)
                  coeff(m(+1,+1),i1,i2,i3)=0
! endLoops()
               end do
               end do
               end do
               end do
               end do
             end if
         else
       !  ***** not rectangular *****
!     #If "divScalarGrad" == "divScalarGrad"
       !       Here we define divScalarGrad as well as laplacian and DxSDy etc
! defineA22()
             m1a=n1a-1
             m1b=n1b+1
             m2a=n2a-1
             m2b=n2b+1
             m3a=n3a
             m3b=n3b
             if( averagingType .eq. arithmeticAverage .or. 
     & derivOption.eq.laplace )then
               factor=.5
! GETA22(s(j1,j2,j3,0)*jac(j1,j2,j3),c,factor)
               if( derivOption.eq.laplace )then
                 do j3=m3a,m3b
                   do j2=m2a,m2b
                     do j1=m1a,m1b
                       sj = jac(j1,j2,j3)
                       a11(j1,j2,j3) = (rx(j1,j2,j3)**2+ry(j1,j2,j3)**
     & 2)*sj
                       a12(j1,j2,j3) = (rx(j1,j2,j3)*sx(j1,j2,j3)+ry(
     & j1,j2,j3)*sy(j1,j2,j3))*sj
                       a22(j1,j2,j3) = (sx(j1,j2,j3)**2+sy(j1,j2,j3)**
     & 2)*sj
                       a21(j1,j2,j3) = a12(j1,j2,j3)
                     end do
                   end do
                 end do
               else if( derivOption.eq.divScalarGrad )then
                 do j3=m3a,m3b
                   do j2=m2a,m2b
                     do j1=m1a,m1b
                       sj = s(j1,j2,j3,0)*jac(j1,j2,j3)
                       a11(j1,j2,j3) = (rx(j1,j2,j3)**2+ry(j1,j2,j3)**
     & 2)*sj
                       a12(j1,j2,j3) = (rx(j1,j2,j3)*sx(j1,j2,j3)+ry(
     & j1,j2,j3)*sy(j1,j2,j3))*sj
                       a22(j1,j2,j3) = (sx(j1,j2,j3)**2+sy(j1,j2,j3)**
     & 2)*sj
                       a21(j1,j2,j3) = a12(j1,j2,j3)
                     end do
                   end do
                 end do
               else if( derivOption.eq.divTensorGrad )then
                 do j3=m3a,m3b
                   do j2=m2a,m2b
                     do j1=m1a,m1b
                       sj =jac(j1,j2,j3)
                       s11=s(j1,j2,j3,0)
                       s21=s(j1,j2,j3,1)
                       s12=s(j1,j2,j3,2)
                       s22=s(j1,j2,j3,3)
                       a11(j1,j2,j3) = (s11*rx(j1,j2,j3)**2+(s12+s21)*
     & rx(j1,j2,j3)*ry(j1,j2,j3)+s22*ry(j1,j2,j3)**2)*sj
                       a12(j1,j2,j3) = (s11*rx(j1,j2,j3)*sx(j1,j2,j3)+
     & s12*rx(j1,j2,j3)*sy(j1,j2,j3)+s21*ry(j1,j2,j3)*sx(j1,j2,j3)+
     & s22*ry(j1,j2,j3)*sy(j1,j2,j3))*sj
                       a22(j1,j2,j3) = (s11*sx(j1,j2,j3)**2+(s12+s21)*
     & sx(j1,j2,j3)*sy(j1,j2,j3)+s22*sy(j1,j2,j3)**2)*sj
                       a21(j1,j2,j3) = (s11*sx(j1,j2,j3)*rx(j1,j2,j3)+
     & s12*sx(j1,j2,j3)*ry(j1,j2,j3)+s21*sy(j1,j2,j3)*rx(j1,j2,j3)+
     & s22*sy(j1,j2,j3)*ry(j1,j2,j3))*sj
                     end do
                   end do
                 end do
               else if( derivOption.eq.derivativeScalarDerivative )then
                 if(      dir1.eq.0 .and. dir2.eq.0 )then
! DXSDY22(x,x,s(j1,j2,j3,0)*jac(j1,j2,j3))
                   do j3=m3a,m3b
                     do j2=m2a,m2b
                       do j1=m1a,m1b
                         sj = s(j1,j2,j3,0)*jac(j1,j2,j3)
                         a11(j1,j2,j3) = (r x (j1,j2,j3)*r x (j1,j2,j3)
     & )*sj
                         a12(j1,j2,j3) = (r x (j1,j2,j3)*s x (j1,j2,j3)
     & )*sj
                         a22(j1,j2,j3) = (s x (j1,j2,j3)*s x (j1,j2,j3)
     & )*sj
! #If "x" == "x"
                         a21(j1,j2,j3) = a12(j1,j2,j3)
                       end do
                     end do
                   end do
                 else if( dir1.eq.0 .and. dir2.eq.1 )then
! DXSDY22(x,y,s(j1,j2,j3,0)*jac(j1,j2,j3))
                   do j3=m3a,m3b
                     do j2=m2a,m2b
                       do j1=m1a,m1b
                         sj = s(j1,j2,j3,0)*jac(j1,j2,j3)
                         a11(j1,j2,j3) = (r x (j1,j2,j3)*r y (j1,j2,j3)
     & )*sj
                         a12(j1,j2,j3) = (r x (j1,j2,j3)*s y (j1,j2,j3)
     & )*sj
                         a22(j1,j2,j3) = (s x (j1,j2,j3)*s y (j1,j2,j3)
     & )*sj
! #If "x" == "y"
! #Else
                         a21(j1,j2,j3) = (s x (j1,j2,j3)*r y (j1,j2,j3)
     & )*sj
                       end do
                     end do
                   end do
                 else if( dir1.eq.1 .and. dir2.eq.0 )then
! DXSDY22(y,x,s(j1,j2,j3,0)*jac(j1,j2,j3))
                   do j3=m3a,m3b
                     do j2=m2a,m2b
                       do j1=m1a,m1b
                         sj = s(j1,j2,j3,0)*jac(j1,j2,j3)
                         a11(j1,j2,j3) = (r y (j1,j2,j3)*r x (j1,j2,j3)
     & )*sj
                         a12(j1,j2,j3) = (r y (j1,j2,j3)*s x (j1,j2,j3)
     & )*sj
                         a22(j1,j2,j3) = (s y (j1,j2,j3)*s x (j1,j2,j3)
     & )*sj
! #If "y" == "x"
! #Else
                         a21(j1,j2,j3) = (s y (j1,j2,j3)*r x (j1,j2,j3)
     & )*sj
                       end do
                     end do
                   end do
                 else if( dir1.eq.1 .and. dir2.eq.1 )then
! DXSDY22(y,y,s(j1,j2,j3,0)*jac(j1,j2,j3))
                   do j3=m3a,m3b
                     do j2=m2a,m2b
                       do j1=m1a,m1b
                         sj = s(j1,j2,j3,0)*jac(j1,j2,j3)
                         a11(j1,j2,j3) = (r y (j1,j2,j3)*r y (j1,j2,j3)
     & )*sj
                         a12(j1,j2,j3) = (r y (j1,j2,j3)*s y (j1,j2,j3)
     & )*sj
                         a22(j1,j2,j3) = (s y (j1,j2,j3)*s y (j1,j2,j3)
     & )*sj
! #If "y" == "y"
                         a21(j1,j2,j3) = a12(j1,j2,j3)
                       end do
                     end do
                   end do
                 else
                   write(*,*) 'ERROR invalid values: dir1=',dir1,' 
     & dir2=',dir2
                 end if
               else
               write(*,*) 'ERROR: unknown value for derivOption=',
     & derivOption
               end if
               if( derivType.eq.symmetric )then
               ! symmetric case -- do not average a12 and a21 -- could do better
                m1a=n1a
                do j3=m3a,m3b
                 do j2=m2a,m2b
                   do j1=m1b,m1a,-1 ! go backwards   worry about division by zero
c     sh=s(j1,j2,j3,0)*s(j1-1,j2,j3,0)/(s(j1,j2,j3,0)+s(j1-1,j2,j3,0))
                     a11(j1,j2,j3) = factor *d22(1)*(a11(j1,j2,j3)+a11(
     & j1-1,j2,j3))
                   end do
                 end do
                end do
                m1a=n1a-1
               ! ***** for now do not average in this case
                do j3=m3a,m3b
                 do j2=m2a,m2b
                   do j1=m1b,m1a,-1
! #If "c" eq "c"
                     a12(j1,j2,j3) =         (d12(1)*d12(2))*a12(j1,j2,
     & j3)
                   end do
                 end do
                end do
                m2a=n2a
                do j3=m3a,m3b
                 do j2=m2b,m2a,-1
                   do j1=m1a,m1b
c     sh=s(j1,j2,j3,0)*s(j1,j2-1,j3,0)/(s(j1,j2,j3,0)+s(j1,j2-1,j3,0)) 
                     a22(j1,j2,j3) = factor *d22(2)*(a22(j1,j2,j3)+a22(
     & j1,j2-1,j3))
                   end do
                 end do
                end do
                m2a=n2a-1
                do j3=m3a,m3b
                 do j2=m2b,m2a,-1
                   do j1=m1a,m1b
! #If "c" eq "c"
                     a21(j1,j2,j3) =         (d12(1)*d12(2))*a21(j1,j2,
     & j3)
                   end do
                 end do
                end do
               else
                m1a=n1a
                do j3=m3a,m3b
                 do j2=m2a,m2b
                   do j1=m1b,m1a,-1 ! go backwards   worry about division by zero
c     sh=s(j1,j2,j3,0)*s(j1-1,j2,j3,0)/(s(j1,j2,j3,0)+s(j1-1,j2,j3,0))
                     a11(j1,j2,j3) = factor *d22(1)*(a11(j1,j2,j3)+a11(
     & j1-1,j2,j3))
                     a12(j1,j2,j3) = factor *(d12(1)*d12(2))*(a12(j1,
     & j2,j3)+a12(j1-1,j2,j3))
                   end do
                 end do
                end do
                m1a=n1a-1
                m2a=n2a
                do j3=m3a,m3b
                 do j2=m2b,m2a,-1
                   do j1=m1a,m1b
c     sh=s(j1,j2,j3,0)*s(j1,j2-1,j3,0)/(s(j1,j2,j3,0)+s(j1,j2-1,j3,0)) 
                     a21(j1,j2,j3) = factor *(d12(1)*d12(2))*(a21(j1,
     & j2,j3)+a21(j1,j2-1,j3))
                     a22(j1,j2,j3) = factor *d22(2)*(a22(j1,j2,j3)+a22(
     & j1,j2-1,j3))
                   end do
                 end do
                end do
                m2a=n2a-1
               end if
             else
c Harmonic average
c  factor=2.
c do not average in s:  
! GETA22(jac(j1,j2,j3), ,sh)
                if( derivOption.eq.laplace )then
                  do j3=m3a,m3b
                    do j2=m2a,m2b
                      do j1=m1a,m1b
                        sj = jac(j1,j2,j3)
                        a11(j1,j2,j3) = (rx(j1,j2,j3)**2+ry(j1,j2,j3)**
     & 2)*sj
                        a12(j1,j2,j3) = (rx(j1,j2,j3)*sx(j1,j2,j3)+ry(
     & j1,j2,j3)*sy(j1,j2,j3))*sj
                        a22(j1,j2,j3) = (sx(j1,j2,j3)**2+sy(j1,j2,j3)**
     & 2)*sj
                        a21(j1,j2,j3) = a12(j1,j2,j3)
                      end do
                    end do
                  end do
                else if( derivOption.eq.divScalarGrad )then
                  do j3=m3a,m3b
                    do j2=m2a,m2b
                      do j1=m1a,m1b
                        sj = jac(j1,j2,j3)
                        a11(j1,j2,j3) = (rx(j1,j2,j3)**2+ry(j1,j2,j3)**
     & 2)*sj
                        a12(j1,j2,j3) = (rx(j1,j2,j3)*sx(j1,j2,j3)+ry(
     & j1,j2,j3)*sy(j1,j2,j3))*sj
                        a22(j1,j2,j3) = (sx(j1,j2,j3)**2+sy(j1,j2,j3)**
     & 2)*sj
                        a21(j1,j2,j3) = a12(j1,j2,j3)
                      end do
                    end do
                  end do
                else if( derivOption.eq.divTensorGrad )then
                  do j3=m3a,m3b
                    do j2=m2a,m2b
                      do j1=m1a,m1b
                        sj =jac(j1,j2,j3)
                        s11=s(j1,j2,j3,0)
                        s21=s(j1,j2,j3,1)
                        s12=s(j1,j2,j3,2)
                        s22=s(j1,j2,j3,3)
                        a11(j1,j2,j3) = (s11*rx(j1,j2,j3)**2+(s12+s21)*
     & rx(j1,j2,j3)*ry(j1,j2,j3)+s22*ry(j1,j2,j3)**2)*sj
                        a12(j1,j2,j3) = (s11*rx(j1,j2,j3)*sx(j1,j2,j3)+
     & s12*rx(j1,j2,j3)*sy(j1,j2,j3)+s21*ry(j1,j2,j3)*sx(j1,j2,j3)+
     & s22*ry(j1,j2,j3)*sy(j1,j2,j3))*sj
                        a22(j1,j2,j3) = (s11*sx(j1,j2,j3)**2+(s12+s21)*
     & sx(j1,j2,j3)*sy(j1,j2,j3)+s22*sy(j1,j2,j3)**2)*sj
                        a21(j1,j2,j3) = (s11*sx(j1,j2,j3)*rx(j1,j2,j3)+
     & s12*sx(j1,j2,j3)*ry(j1,j2,j3)+s21*sy(j1,j2,j3)*rx(j1,j2,j3)+
     & s22*sy(j1,j2,j3)*ry(j1,j2,j3))*sj
                      end do
                    end do
                  end do
                else if( derivOption.eq.derivativeScalarDerivative )
     & then
                  if(      dir1.eq.0 .and. dir2.eq.0 )then
! DXSDY22(x,x,jac(j1,j2,j3))
                    do j3=m3a,m3b
                      do j2=m2a,m2b
                        do j1=m1a,m1b
                          sj = jac(j1,j2,j3)
                          a11(j1,j2,j3) = (r x (j1,j2,j3)*r x (j1,j2,
     & j3))*sj
                          a12(j1,j2,j3) = (r x (j1,j2,j3)*s x (j1,j2,
     & j3))*sj
                          a22(j1,j2,j3) = (s x (j1,j2,j3)*s x (j1,j2,
     & j3))*sj
! #If "x" == "x"
                          a21(j1,j2,j3) = a12(j1,j2,j3)
                        end do
                      end do
                    end do
                  else if( dir1.eq.0 .and. dir2.eq.1 )then
! DXSDY22(x,y,jac(j1,j2,j3))
                    do j3=m3a,m3b
                      do j2=m2a,m2b
                        do j1=m1a,m1b
                          sj = jac(j1,j2,j3)
                          a11(j1,j2,j3) = (r x (j1,j2,j3)*r y (j1,j2,
     & j3))*sj
                          a12(j1,j2,j3) = (r x (j1,j2,j3)*s y (j1,j2,
     & j3))*sj
                          a22(j1,j2,j3) = (s x (j1,j2,j3)*s y (j1,j2,
     & j3))*sj
! #If "x" == "y"
! #Else
                          a21(j1,j2,j3) = (s x (j1,j2,j3)*r y (j1,j2,
     & j3))*sj
                        end do
                      end do
                    end do
                  else if( dir1.eq.1 .and. dir2.eq.0 )then
! DXSDY22(y,x,jac(j1,j2,j3))
                    do j3=m3a,m3b
                      do j2=m2a,m2b
                        do j1=m1a,m1b
                          sj = jac(j1,j2,j3)
                          a11(j1,j2,j3) = (r y (j1,j2,j3)*r x (j1,j2,
     & j3))*sj
                          a12(j1,j2,j3) = (r y (j1,j2,j3)*s x (j1,j2,
     & j3))*sj
                          a22(j1,j2,j3) = (s y (j1,j2,j3)*s x (j1,j2,
     & j3))*sj
! #If "y" == "x"
! #Else
                          a21(j1,j2,j3) = (s y (j1,j2,j3)*r x (j1,j2,
     & j3))*sj
                        end do
                      end do
                    end do
                  else if( dir1.eq.1 .and. dir2.eq.1 )then
! DXSDY22(y,y,jac(j1,j2,j3))
                    do j3=m3a,m3b
                      do j2=m2a,m2b
                        do j1=m1a,m1b
                          sj = jac(j1,j2,j3)
                          a11(j1,j2,j3) = (r y (j1,j2,j3)*r y (j1,j2,
     & j3))*sj
                          a12(j1,j2,j3) = (r y (j1,j2,j3)*s y (j1,j2,
     & j3))*sj
                          a22(j1,j2,j3) = (s y (j1,j2,j3)*s y (j1,j2,
     & j3))*sj
! #If "y" == "y"
                          a21(j1,j2,j3) = a12(j1,j2,j3)
                        end do
                      end do
                    end do
                  else
                    write(*,*) 'ERROR invalid values: dir1=',dir1,' 
     & dir2=',dir2
                  end if
                else
                write(*,*) 'ERROR: unknown value for derivOption=',
     & derivOption
                end if
                if( derivType.eq.symmetric )then
                ! symmetric case -- do not average a12 and a21 -- could do better
                 m1a=n1a
                 do j3=m3a,m3b
                  do j2=m2a,m2b
                    do j1=m1b,m1a,-1 ! go backwards   worry about division by zero
                     sh=s(j1,j2,j3,0)*s(j1-1,j2,j3,0)/(s(j1,j2,j3,0)+s(
     & j1-1,j2,j3,0))
                      a11(j1,j2,j3) = sh *d22(1)*(a11(j1,j2,j3)+a11(j1-
     & 1,j2,j3))
                    end do
                  end do
                 end do
                 m1a=n1a-1
                ! ***** for now do not average in this case
                 do j3=m3a,m3b
                  do j2=m2a,m2b
                    do j1=m1b,m1a,-1
! #If "" eq "c"
! #Else
                      a12(j1,j2,j3) = s(j1,j2,j3,0)*(d12(1)*d12(2))*
     & a12(j1,j2,j3)
                    end do
                  end do
                 end do
                 m2a=n2a
                 do j3=m3a,m3b
                  do j2=m2b,m2a,-1
                    do j1=m1a,m1b
                     sh=s(j1,j2,j3,0)*s(j1,j2-1,j3,0)/(s(j1,j2,j3,0)+s(
     & j1,j2-1,j3,0))
                      a22(j1,j2,j3) = sh *d22(2)*(a22(j1,j2,j3)+a22(j1,
     & j2-1,j3))
                    end do
                  end do
                 end do
                 m2a=n2a-1
                 do j3=m3a,m3b
                  do j2=m2b,m2a,-1
                    do j1=m1a,m1b
! #If "" eq "c"
! #Else
                      a21(j1,j2,j3) = s(j1,j2,j3,0)*(d12(1)*d12(2))*
     & a21(j1,j2,j3)
                    end do
                  end do
                 end do
                else
                 m1a=n1a
                 do j3=m3a,m3b
                  do j2=m2a,m2b
                    do j1=m1b,m1a,-1 ! go backwards   worry about division by zero
                     sh=s(j1,j2,j3,0)*s(j1-1,j2,j3,0)/(s(j1,j2,j3,0)+s(
     & j1-1,j2,j3,0))
                      a11(j1,j2,j3) = sh *d22(1)*(a11(j1,j2,j3)+a11(j1-
     & 1,j2,j3))
                      a12(j1,j2,j3) = sh *(d12(1)*d12(2))*(a12(j1,j2,
     & j3)+a12(j1-1,j2,j3))
                    end do
                  end do
                 end do
                 m1a=n1a-1
                 m2a=n2a
                 do j3=m3a,m3b
                  do j2=m2b,m2a,-1
                    do j1=m1a,m1b
                     sh=s(j1,j2,j3,0)*s(j1,j2-1,j3,0)/(s(j1,j2,j3,0)+s(
     & j1,j2-1,j3,0))
                      a21(j1,j2,j3) = sh *(d12(1)*d12(2))*(a21(j1,j2,
     & j3)+a21(j1,j2-1,j3))
                      a22(j1,j2,j3) = sh *d22(2)*(a22(j1,j2,j3)+a22(j1,
     & j2-1,j3))
                    end do
                  end do
                 end do
                 m2a=n2a-1
                end if
             end if
       !       This was generated by dd.m
! beginLoops()
             ! ***** loop over equations and components *****
             do e=ea,eb
             do c=ca,cb
             ec=ns*(c+nc*e)
             ! ** it did not affect performance to use an array to index coeff ***
             if( nd.eq.2 )then
             do i2=-1,1
               do i1=-1,1
                m(i1,i2)=i1+1+3*(i2+1) +1 + ec
               end do
             end do
             else if( nd.eq.3 )then
             do i3=-1,1
               do i2=-1,1
                 do i1=-1,1
                   m3(i1,i2,i3)=i1+1+3*(i2+1+3*(i3+1)) +1 + ec
                 end do
               end do
             end do
             else
             m12=1 + ec
             m22=2 + ec
             m32=3 + ec
             endif
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
! loopBody2ndOrder2d( (a12(i1,i2,i3)+a21(i1,i2,i3))/jac(i1,i2,i3), (a12(i1,i2,i3)+a22(i1,i2,i3)-a12(i1+1,i2,i3))/jac(i1,i2,i3), -(a21(i1,i2,i3)+a12(i1+1,i2,i3))/jac(i1,i2,i3), (a11(i1,i2,i3)+a21(i1,i2,i3)-a21(i1,i2+1,i3))/jac(i1,i2,i3), -(a11(i1+1,i2,i3)+a11(i1,i2,i3)+a22(i1,i2+1,i3)+a22(i1,i2,i3))/jac(i1,i2,i3), -(-a11(i1+1,i2,i3)-a21(i1,i2+1,i3)+a21(i1,i2,i3))/jac(i1,i2,i3), -(a21(i1,i2+1,i3)+a12(i1,i2,i3))/jac(i1,i2,i3), -(a12(i1,i2,i3)-a22(i1,i2+1,i3)-a12(i1+1,i2,i3))/jac(i1,i2,i3), (a21(i1,i2+1,i3)+a12(i1+1,i2,i3))/jac(i1,i2,i3) )
                 coeff(m(-1,-1),i1,i2,i3)=(a12(i1,i2,i3)+a21(i1,i2,i3))
     & /jac(i1,i2,i3)
                 coeff(m( 0,-1),i1,i2,i3)=(a12(i1,i2,i3)+a22(i1,i2,i3)-
     & a12(i1+1,i2,i3))/jac(i1,i2,i3)
                 coeff(m(+1,-1),i1,i2,i3)=-(a21(i1,i2,i3)+a12(i1+1,i2,
     & i3))/jac(i1,i2,i3)
                 coeff(m(-1, 0),i1,i2,i3)=(a11(i1,i2,i3)+a21(i1,i2,i3)-
     & a21(i1,i2+1,i3))/jac(i1,i2,i3)
                 coeff(m( 0, 0),i1,i2,i3)=-(a11(i1+1,i2,i3)+a11(i1,i2,
     & i3)+a22(i1,i2+1,i3)+a22(i1,i2,i3))/jac(i1,i2,i3)
                 coeff(m(+1, 0),i1,i2,i3)=-(-a11(i1+1,i2,i3)-a21(i1,i2+
     & 1,i3)+a21(i1,i2,i3))/jac(i1,i2,i3)
                 coeff(m(-1,+1),i1,i2,i3)=-(a21(i1,i2+1,i3)+a12(i1,i2,
     & i3))/jac(i1,i2,i3)
                 coeff(m( 0,+1),i1,i2,i3)=-(a12(i1,i2,i3)-a22(i1,i2+1,
     & i3)-a12(i1+1,i2,i3))/jac(i1,i2,i3)
                 coeff(m(+1,+1),i1,i2,i3)=(a21(i1,i2+1,i3)+a12(i1+1,i2,
     & i3))/jac(i1,i2,i3)
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
!   #If "divScalarGrad" == "identity"
!    #Elif "divScalarGrad" == "r"
!    #Elif "divScalarGrad" == "s"
!    #Elif "divScalarGrad" == "t"
!    #Elif "divScalarGrad" == "rr"
!    #Elif "divScalarGrad" == "ss"
!    #Elif "divScalarGrad" == "tt"
!    #Elif "divScalarGrad" == "rs"
!    #Elif "divScalarGrad" == "rt"
!    #Elif "divScalarGrad" == "st"
         if( gridType .eq. 0 )then
       !   rectangular
!     #If "divScalarGrad" == "divScalarGrad"
! defineA23R()
             m1a=n1a-1
             m1b=n1b+1
             m2a=n2a-1
             m2b=n2b+1
             m3a=n3a-1
             m3b=n3b+1
             if( averagingType .eq. arithmeticAverage )then
              factor=.5
              if( derivOption.eq.divScalarGrad  )then
! loopsDSG1(a11(j1,j2,j3) = factor*h22(1)*(s(j1,j2,j3,0)+s(j1-1,j2,j3,0)))
                m1a=n1a
                do j3=m3a,m3b
                  do j2=m2a,m2b
                    do j1=m1a,m1b
                      a11(j1,j2,j3)=factor*h22(1)*(s(j1,j2,j3,0)+s(j1-
     & 1,j2,j3,0))
                    end do
                  end do
                end do
                m1a=n1a-1
! loopsDSG2(a22(j1,j2,j3) = factor*h22(2)*(s(j1,j2,j3,0)+s(j1,j2-1,j3,0)))
                m2a=n2a
                do j3=m3a,m3b
                  do j2=m2a,m2b
                    do j1=m1a,m1b
                      a22(j1,j2,j3)=factor*h22(2)*(s(j1,j2,j3,0)+s(j1,
     & j2-1,j3,0))
                    end do
                  end do
                end do
                m2a=n2a-1
! loopsDSG3(a33(j1,j2,j3) = factor*h22(3)*(s(j1,j2,j3,0)+s(j1,j2,j3-1,0)))
                m3a=n3a
                do j3=m3a,m3b
                  do j2=m2a,m2b
                    do j1=m1a,m1b
                      a33(j1,j2,j3)=factor*h22(3)*(s(j1,j2,j3,0)+s(j1,
     & j2,j3-1,0))
                    end do
                  end do
                end do
                m3a=n3a-1
              else if( derivOption.eq.divTensorGrad )then
                ! form the coefficients and average
                !   we need to worry about the end points m1a=n1a-1 *wdh* 060210
                do j3=m3a,m3b
                  j3m1=max(j3-1,m3a)
                do j2=m2a,m2b
                  j2m1=max(j2-1,m2a)
                do j1=m1a,m1b
                  j1m1=max(j1-1,m1a)
                  a11(j1,j2,j3) = .5*(s(j1,j2,j3,0)+s(j1m1,j2,j3,0))
     & /dx(1)**2
                  a21(j1,j2,j3) =     s(j1,j2,j3,1)/(4.*dx(2)*dx(1))
                  a31(j1,j2,j3) =     s(j1,j2,j3,2)/(4.*dx(3)*dx(1))
                  a12(j1,j2,j3) =     s(j1,j2,j3,3)/(4.*dx(1)*dx(2))
                  a22(j1,j2,j3) = .5*(s(j1,j2,j3,4)+s(j1,j2m1,j3,4))
     & /dx(2)**2
                  a32(j1,j2,j3) =     s(j1,j2,j3,5)/(4.*dx(3)*dx(2))
                  a13(j1,j2,j3) =     s(j1,j2,j3,6)/(4.*dx(1)*dx(3))
                  a23(j1,j2,j3) =     s(j1,j2,j3,7)/(4.*dx(2)*dx(3))
                  a33(j1,j2,j3) = .5*(s(j1,j2,j3,8)+s(j1,j2,j3m1,8))
     & /dx(3)**2
                end do
                end do
                end do
              else if( derivOption.eq.derivativeScalarDerivative )then
                if( dir1.eq.dir2 )then
                  hh=h22(dir1+1)
                else
                  hh=h21(dir1+1)*h21(dir2+1)
                end if
                if( dir1.eq.0 )then
! loopsDSG1(a11(j1,j2,j3) = factor*hh*(s(j1,j2,j3,0)+s(j1-1,j2,j3,0)))
                  m1a=n1a
                  do j3=m3a,m3b
                    do j2=m2a,m2b
                      do j1=m1a,m1b
                        a11(j1,j2,j3)=factor*hh*(s(j1,j2,j3,0)+s(j1-1,
     & j2,j3,0))
                      end do
                    end do
                  end do
                  m1a=n1a-1
                else if( dir1.eq.1 )then
! loopsDSG2(a11(j1,j2,j3) = factor*hh*(s(j1,j2,j3,0)+s(j1,j2-1,j3,0)))
                  m2a=n2a
                  do j3=m3a,m3b
                    do j2=m2a,m2b
                      do j1=m1a,m1b
                        a11(j1,j2,j3)=factor*hh*(s(j1,j2,j3,0)+s(j1,j2-
     & 1,j3,0))
                      end do
                    end do
                  end do
                  m2a=n2a-1
                else
! loopsDSG3(a11(j1,j2,j3) = factor*hh*(s(j1,j2,j3,0)+s(j1,j2,j3-1,0)))
                  m3a=n3a
                  do j3=m3a,m3b
                    do j2=m2a,m2b
                      do j1=m1a,m1b
                        a11(j1,j2,j3)=factor*hh*(s(j1,j2,j3,0)+s(j1,j2,
     & j3-1,0))
                      end do
                    end do
                  end do
                  m3a=n3a-1
                end if
              else
                stop 3329
              end if
             else
c  Harmonic average
               factor=2.
               if( derivOption.eq.divScalarGrad  )then
                 ! should be worry about division by zero?
! loopsDSG1(a11(j1,j2,j3) =s(j1,j2,j3,0)*s(j1-1,j2,j3,0)*h22(1)*factor/(s(j1,j2,j3,0)+s(j1-1,j2,j3,0)))
                 m1a=n1a
                 do j3=m3a,m3b
                   do j2=m2a,m2b
                     do j1=m1a,m1b
                       a11(j1,j2,j3)=s(j1,j2,j3,0)*s(j1-1,j2,j3,0)*h22(
     & 1)*factor/(s(j1,j2,j3,0)+s(j1-1,j2,j3,0))
                     end do
                   end do
                 end do
                 m1a=n1a-1
! loopsDSG2(a22(j1,j2,j3) =s(j1,j2,j3,0)*s(j1,j2-1,j3,0)*h22(2)*factor/(s(j1,j2,j3,0)+s(j1,j2-1,j3,0)))
                 m2a=n2a
                 do j3=m3a,m3b
                   do j2=m2a,m2b
                     do j1=m1a,m1b
                       a22(j1,j2,j3)=s(j1,j2,j3,0)*s(j1,j2-1,j3,0)*h22(
     & 2)*factor/(s(j1,j2,j3,0)+s(j1,j2-1,j3,0))
                     end do
                   end do
                 end do
                 m2a=n2a-1
! loopsDSG3(a33(j1,j2,j3) =s(j1,j2,j3,0)*s(j1,j2,j3-1,0)*h22(3)*factor/(s(j1,j2,j3,0)+s(j1,j2,j3-1,0)))
                 m3a=n3a
                 do j3=m3a,m3b
                   do j2=m2a,m2b
                     do j1=m1a,m1b
                       a33(j1,j2,j3)=s(j1,j2,j3,0)*s(j1,j2,j3-1,0)*h22(
     & 3)*factor/(s(j1,j2,j3,0)+s(j1,j2,j3-1,0))
                     end do
                   end do
                 end do
                 m3a=n3a-1
               else if( derivOption.eq.divTensorGrad )then
                 stop 3388
               else if( derivOption.eq.derivativeScalarDerivative )then
                 if( dir1.eq.dir2 )then
                   hh=h22(dir1+1)
                 else
                   hh=h21(dir1+1)*h21(dir2+1)
                 end if
                 if( dir1.eq.0 )then
! loopsDSG1(a11(j1,j2,j3)=s(j1,j2,j3,0)*s(j1-1,j2,j3,0)*hh*factor/(s(j1,j2,j3,0)+s(j1-1,j2,j3,0)))
                   m1a=n1a
                   do j3=m3a,m3b
                     do j2=m2a,m2b
                       do j1=m1a,m1b
                         a11(j1,j2,j3)=s(j1,j2,j3,0)*s(j1-1,j2,j3,0)*
     & hh*factor/(s(j1,j2,j3,0)+s(j1-1,j2,j3,0))
                       end do
                     end do
                   end do
                   m1a=n1a-1
                 else if( dir1.eq.1 )then
! loopsDSG2(a11(j1,j2,j3)=s(j1,j2,j3,0)*s(j1,j2-1,j3,0)*hh*factor/(s(j1,j2,j3,0)+s(j1,j2-1,j3,0)))
                   m2a=n2a
                   do j3=m3a,m3b
                     do j2=m2a,m2b
                       do j1=m1a,m1b
                         a11(j1,j2,j3)=s(j1,j2,j3,0)*s(j1,j2-1,j3,0)*
     & hh*factor/(s(j1,j2,j3,0)+s(j1,j2-1,j3,0))
                       end do
                     end do
                   end do
                   m2a=n2a-1
                 else
! loopsDSG3(a11(j1,j2,j3)=s(j1,j2,j3,0)*s(j1,j2,j3-1,0)*hh*factor/(s(j1,j2,j3,0)+s(j1,j2,j3-1,0)))
                   m3a=n3a
                   do j3=m3a,m3b
                     do j2=m2a,m2b
                       do j1=m1a,m1b
                         a11(j1,j2,j3)=s(j1,j2,j3,0)*s(j1,j2,j3-1,0)*
     & hh*factor/(s(j1,j2,j3,0)+s(j1,j2,j3-1,0))
                       end do
                     end do
                   end do
                   m3a=n3a-1
                 end if
               else
                 stop 3429
               end if
             end if
             if( derivOption.eq.divScalarGrad )then
! beginLoops()
              ! ***** loop over equations and components *****
              do e=ea,eb
              do c=ca,cb
              ec=ns*(c+nc*e)
              ! ** it did not affect performance to use an array to index coeff ***
              if( nd.eq.2 )then
              do i2=-1,1
                do i1=-1,1
                 m(i1,i2)=i1+1+3*(i2+1) +1 + ec
                end do
              end do
              else if( nd.eq.3 )then
              do i3=-1,1
                do i2=-1,1
                  do i1=-1,1
                    m3(i1,i2,i3)=i1+1+3*(i2+1+3*(i3+1)) +1 + ec
                  end do
                end do
              end do
              else
              m12=1 + ec
              m22=2 + ec
              m32=3 + ec
              endif
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
       !       This was generated by dd.m
! loopBody2ndOrder3d(0,0,0,0,a33(i1,i2,i3),0,0,0,0,0,a22(i1,i2,i3),0,a11(i1,i2,i3), -a11(i1+1,i2,i3)-a11(i1,i2,i3)-a22(i1,i2+1,i3)-a22(i1,i2,i3)-a33(i1,i2,i3+1)-a33(i1,i2,i3), a11(i1+1,i2,i3),0,a22(i1,i2+1,i3),0,0,0,0,0,a33(i1,i2,i3+1),0,0,0,0 )
                coeff(m3(-1,-1,-1),i1,i2,i3)=0
                coeff(m3( 0,-1,-1),i1,i2,i3)=0
                coeff(m3(+1,-1,-1),i1,i2,i3)=0
                coeff(m3(-1, 0,-1),i1,i2,i3)=0
                coeff(m3( 0, 0,-1),i1,i2,i3)=a33(i1,i2,i3)
                coeff(m3(+1, 0,-1),i1,i2,i3)=0
                coeff(m3(-1,+1,-1),i1,i2,i3)=0
                coeff(m3( 0,+1,-1),i1,i2,i3)=0
                coeff(m3(+1,+1,-1),i1,i2,i3)=0
                coeff(m3(-1,-1, 0),i1,i2,i3)=0
                coeff(m3( 0,-1, 0),i1,i2,i3)=a22(i1,i2,i3)
                coeff(m3(+1,-1, 0),i1,i2,i3)=0
                coeff(m3(-1, 0, 0),i1,i2,i3)=a11(i1,i2,i3)
                coeff(m3( 0, 0, 0),i1,i2,i3)=-a11(i1+1,i2,i3)-a11(i1,
     & i2,i3)-a22(i1,i2+1,i3)-a22(i1,i2,i3)-a33(i1,i2,i3+1)-a33(i1,i2,
     & i3)
                coeff(m3(+1, 0, 0),i1,i2,i3)=a11(i1+1,i2,i3)
                coeff(m3(-1,+1, 0),i1,i2,i3)=0
                coeff(m3( 0,+1, 0),i1,i2,i3)=a22(i1,i2+1,i3)
                coeff(m3(+1,+1, 0),i1,i2,i3)=0
                coeff(m3(-1,-1,+1),i1,i2,i3)=0
                coeff(m3( 0,-1,+1),i1,i2,i3)=0
                coeff(m3(+1,-1,+1),i1,i2,i3)=0
                coeff(m3(-1, 0,+1),i1,i2,i3)=0
                coeff(m3( 0, 0,+1),i1,i2,i3)=a33(i1,i2,i3+1)
                coeff(m3(+1, 0,+1),i1,i2,i3)=0
                coeff(m3(-1,+1,+1),i1,i2,i3)=0
                coeff(m3( 0,+1,+1),i1,i2,i3)=0
                coeff(m3(+1,+1,+1),i1,i2,i3)=0
! endLoops()
              end do
              end do
              end do
              end do
              end do
             else if( dir1.eq.0 .and. dir2.eq.0 )then
! beginLoops()
               ! ***** loop over equations and components *****
               do e=ea,eb
               do c=ca,cb
               ec=ns*(c+nc*e)
               ! ** it did not affect performance to use an array to index coeff ***
               if( nd.eq.2 )then
               do i2=-1,1
                 do i1=-1,1
                  m(i1,i2)=i1+1+3*(i2+1) +1 + ec
                 end do
               end do
               else if( nd.eq.3 )then
               do i3=-1,1
                 do i2=-1,1
                   do i1=-1,1
                     m3(i1,i2,i3)=i1+1+3*(i2+1+3*(i3+1)) +1 + ec
                   end do
                 end do
               end do
               else
               m12=1 + ec
               m22=2 + ec
               m32=3 + ec
               endif
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
! loopBody2ndOrder3d(0,0,0,0,0,0,0,0,0,0,0,0,a11(i1,i2,i3),-a11(i1+1,i2,i3)-a11(i1,i2,i3),a11(i1+1,i2,i3),0,0,0,0,0,0,0,0,0,0,0,0 )
                 coeff(m3(-1,-1,-1),i1,i2,i3)=0
                 coeff(m3( 0,-1,-1),i1,i2,i3)=0
                 coeff(m3(+1,-1,-1),i1,i2,i3)=0
                 coeff(m3(-1, 0,-1),i1,i2,i3)=0
                 coeff(m3( 0, 0,-1),i1,i2,i3)=0
                 coeff(m3(+1, 0,-1),i1,i2,i3)=0
                 coeff(m3(-1,+1,-1),i1,i2,i3)=0
                 coeff(m3( 0,+1,-1),i1,i2,i3)=0
                 coeff(m3(+1,+1,-1),i1,i2,i3)=0
                 coeff(m3(-1,-1, 0),i1,i2,i3)=0
                 coeff(m3( 0,-1, 0),i1,i2,i3)=0
                 coeff(m3(+1,-1, 0),i1,i2,i3)=0
                 coeff(m3(-1, 0, 0),i1,i2,i3)=a11(i1,i2,i3)
                 coeff(m3( 0, 0, 0),i1,i2,i3)=-a11(i1+1,i2,i3)-a11(i1,
     & i2,i3)
                 coeff(m3(+1, 0, 0),i1,i2,i3)=a11(i1+1,i2,i3)
                 coeff(m3(-1,+1, 0),i1,i2,i3)=0
                 coeff(m3( 0,+1, 0),i1,i2,i3)=0
                 coeff(m3(+1,+1, 0),i1,i2,i3)=0
                 coeff(m3(-1,-1,+1),i1,i2,i3)=0
                 coeff(m3( 0,-1,+1),i1,i2,i3)=0
                 coeff(m3(+1,-1,+1),i1,i2,i3)=0
                 coeff(m3(-1, 0,+1),i1,i2,i3)=0
                 coeff(m3( 0, 0,+1),i1,i2,i3)=0
                 coeff(m3(+1, 0,+1),i1,i2,i3)=0
                 coeff(m3(-1,+1,+1),i1,i2,i3)=0
                 coeff(m3( 0,+1,+1),i1,i2,i3)=0
                 coeff(m3(+1,+1,+1),i1,i2,i3)=0
! endLoops()
               end do
               end do
               end do
               end do
               end do
             else if( dir1.eq.0 .and. dir2.eq.1 )then
! beginLoops()
               ! ***** loop over equations and components *****
               do e=ea,eb
               do c=ca,cb
               ec=ns*(c+nc*e)
               ! ** it did not affect performance to use an array to index coeff ***
               if( nd.eq.2 )then
               do i2=-1,1
                 do i1=-1,1
                  m(i1,i2)=i1+1+3*(i2+1) +1 + ec
                 end do
               end do
               else if( nd.eq.3 )then
               do i3=-1,1
                 do i2=-1,1
                   do i1=-1,1
                     m3(i1,i2,i3)=i1+1+3*(i2+1+3*(i3+1)) +1 + ec
                   end do
                 end do
               end do
               else
               m12=1 + ec
               m22=2 + ec
               m32=3 + ec
               endif
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
! loopBody2ndOrder3d(0,0,0,0,0,0,0,0,0,a11(i1,i2,i3),-a11(i1+1,i2,i3)+a11(i1,i2,i3),-a11(i1+1,i2,i3),0,0,0,-a11(i1,i2,i3), a11(i1+1,i2,i3)-a11(i1,i2,i3),a11(i1+1,i2,i3),0,0,0,0,0,0,0,0,0 )
                 coeff(m3(-1,-1,-1),i1,i2,i3)=0
                 coeff(m3( 0,-1,-1),i1,i2,i3)=0
                 coeff(m3(+1,-1,-1),i1,i2,i3)=0
                 coeff(m3(-1, 0,-1),i1,i2,i3)=0
                 coeff(m3( 0, 0,-1),i1,i2,i3)=0
                 coeff(m3(+1, 0,-1),i1,i2,i3)=0
                 coeff(m3(-1,+1,-1),i1,i2,i3)=0
                 coeff(m3( 0,+1,-1),i1,i2,i3)=0
                 coeff(m3(+1,+1,-1),i1,i2,i3)=0
                 coeff(m3(-1,-1, 0),i1,i2,i3)=a11(i1,i2,i3)
                 coeff(m3( 0,-1, 0),i1,i2,i3)=-a11(i1+1,i2,i3)+a11(i1,
     & i2,i3)
                 coeff(m3(+1,-1, 0),i1,i2,i3)=-a11(i1+1,i2,i3)
                 coeff(m3(-1, 0, 0),i1,i2,i3)=0
                 coeff(m3( 0, 0, 0),i1,i2,i3)=0
                 coeff(m3(+1, 0, 0),i1,i2,i3)=0
                 coeff(m3(-1,+1, 0),i1,i2,i3)=-a11(i1,i2,i3)
                 coeff(m3( 0,+1, 0),i1,i2,i3)=a11(i1+1,i2,i3)-a11(i1,
     & i2,i3)
                 coeff(m3(+1,+1, 0),i1,i2,i3)=a11(i1+1,i2,i3)
                 coeff(m3(-1,-1,+1),i1,i2,i3)=0
                 coeff(m3( 0,-1,+1),i1,i2,i3)=0
                 coeff(m3(+1,-1,+1),i1,i2,i3)=0
                 coeff(m3(-1, 0,+1),i1,i2,i3)=0
                 coeff(m3( 0, 0,+1),i1,i2,i3)=0
                 coeff(m3(+1, 0,+1),i1,i2,i3)=0
                 coeff(m3(-1,+1,+1),i1,i2,i3)=0
                 coeff(m3( 0,+1,+1),i1,i2,i3)=0
                 coeff(m3(+1,+1,+1),i1,i2,i3)=0
! endLoops()
               end do
               end do
               end do
               end do
               end do
             else if( dir1.eq.0 .and. dir2.eq.2 )then
! beginLoops()
               ! ***** loop over equations and components *****
               do e=ea,eb
               do c=ca,cb
               ec=ns*(c+nc*e)
               ! ** it did not affect performance to use an array to index coeff ***
               if( nd.eq.2 )then
               do i2=-1,1
                 do i1=-1,1
                  m(i1,i2)=i1+1+3*(i2+1) +1 + ec
                 end do
               end do
               else if( nd.eq.3 )then
               do i3=-1,1
                 do i2=-1,1
                   do i1=-1,1
                     m3(i1,i2,i3)=i1+1+3*(i2+1+3*(i3+1)) +1 + ec
                   end do
                 end do
               end do
               else
               m12=1 + ec
               m22=2 + ec
               m32=3 + ec
               endif
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
! loopBody2ndOrder3d(0,0,0,a11(i1,i2,i3),-a11(i1+1,i2,i3)+a11(i1,i2,i3),-a11(i1+1,i2,i3),0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, -a11(i1,i2,i3),a11(i1+1,i2,i3)-a11(i1,i2,i3),a11(i1+1,i2,i3),0,0,0 )
                 coeff(m3(-1,-1,-1),i1,i2,i3)=0
                 coeff(m3( 0,-1,-1),i1,i2,i3)=0
                 coeff(m3(+1,-1,-1),i1,i2,i3)=0
                 coeff(m3(-1, 0,-1),i1,i2,i3)=a11(i1,i2,i3)
                 coeff(m3( 0, 0,-1),i1,i2,i3)=-a11(i1+1,i2,i3)+a11(i1,
     & i2,i3)
                 coeff(m3(+1, 0,-1),i1,i2,i3)=-a11(i1+1,i2,i3)
                 coeff(m3(-1,+1,-1),i1,i2,i3)=0
                 coeff(m3( 0,+1,-1),i1,i2,i3)=0
                 coeff(m3(+1,+1,-1),i1,i2,i3)=0
                 coeff(m3(-1,-1, 0),i1,i2,i3)=0
                 coeff(m3( 0,-1, 0),i1,i2,i3)=0
                 coeff(m3(+1,-1, 0),i1,i2,i3)=0
                 coeff(m3(-1, 0, 0),i1,i2,i3)=0
                 coeff(m3( 0, 0, 0),i1,i2,i3)=0
                 coeff(m3(+1, 0, 0),i1,i2,i3)=0
                 coeff(m3(-1,+1, 0),i1,i2,i3)=0
                 coeff(m3( 0,+1, 0),i1,i2,i3)=0
                 coeff(m3(+1,+1, 0),i1,i2,i3)=0
                 coeff(m3(-1,-1,+1),i1,i2,i3)=0
                 coeff(m3( 0,-1,+1),i1,i2,i3)=0
                 coeff(m3(+1,-1,+1),i1,i2,i3)=0
                 coeff(m3(-1, 0,+1),i1,i2,i3)=-a11(i1,i2,i3)
                 coeff(m3( 0, 0,+1),i1,i2,i3)=a11(i1+1,i2,i3)-a11(i1,
     & i2,i3)
                 coeff(m3(+1, 0,+1),i1,i2,i3)=a11(i1+1,i2,i3)
                 coeff(m3(-1,+1,+1),i1,i2,i3)=0
                 coeff(m3( 0,+1,+1),i1,i2,i3)=0
                 coeff(m3(+1,+1,+1),i1,i2,i3)=0
! endLoops()
               end do
               end do
               end do
               end do
               end do
             else if( dir1.eq.1 .and. dir2.eq.0 )then
! beginLoops()
               ! ***** loop over equations and components *****
               do e=ea,eb
               do c=ca,cb
               ec=ns*(c+nc*e)
               ! ** it did not affect performance to use an array to index coeff ***
               if( nd.eq.2 )then
               do i2=-1,1
                 do i1=-1,1
                  m(i1,i2)=i1+1+3*(i2+1) +1 + ec
                 end do
               end do
               else if( nd.eq.3 )then
               do i3=-1,1
                 do i2=-1,1
                   do i1=-1,1
                     m3(i1,i2,i3)=i1+1+3*(i2+1+3*(i3+1)) +1 + ec
                   end do
                 end do
               end do
               else
               m12=1 + ec
               m22=2 + ec
               m32=3 + ec
               endif
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
! loopBody2ndOrder3d(0,0,0,0,0,0,0,0,0,a11(i1,i2,i3),0,-a11(i1,i2,i3),-a11(i1,i2+1,i3)+a11(i1,i2,i3),0, a11(i1,i2+1,i3)-a11(i1,i2,i3),-a11(i1,i2+1,i3),0,a11(i1,i2+1,i3),0,0,0,0,0,0,0,0,0 )
                 coeff(m3(-1,-1,-1),i1,i2,i3)=0
                 coeff(m3( 0,-1,-1),i1,i2,i3)=0
                 coeff(m3(+1,-1,-1),i1,i2,i3)=0
                 coeff(m3(-1, 0,-1),i1,i2,i3)=0
                 coeff(m3( 0, 0,-1),i1,i2,i3)=0
                 coeff(m3(+1, 0,-1),i1,i2,i3)=0
                 coeff(m3(-1,+1,-1),i1,i2,i3)=0
                 coeff(m3( 0,+1,-1),i1,i2,i3)=0
                 coeff(m3(+1,+1,-1),i1,i2,i3)=0
                 coeff(m3(-1,-1, 0),i1,i2,i3)=a11(i1,i2,i3)
                 coeff(m3( 0,-1, 0),i1,i2,i3)=0
                 coeff(m3(+1,-1, 0),i1,i2,i3)=-a11(i1,i2,i3)
                 coeff(m3(-1, 0, 0),i1,i2,i3)=-a11(i1,i2+1,i3)+a11(i1,
     & i2,i3)
                 coeff(m3( 0, 0, 0),i1,i2,i3)=0
                 coeff(m3(+1, 0, 0),i1,i2,i3)=a11(i1,i2+1,i3)-a11(i1,
     & i2,i3)
                 coeff(m3(-1,+1, 0),i1,i2,i3)=-a11(i1,i2+1,i3)
                 coeff(m3( 0,+1, 0),i1,i2,i3)=0
                 coeff(m3(+1,+1, 0),i1,i2,i3)=a11(i1,i2+1,i3)
                 coeff(m3(-1,-1,+1),i1,i2,i3)=0
                 coeff(m3( 0,-1,+1),i1,i2,i3)=0
                 coeff(m3(+1,-1,+1),i1,i2,i3)=0
                 coeff(m3(-1, 0,+1),i1,i2,i3)=0
                 coeff(m3( 0, 0,+1),i1,i2,i3)=0
                 coeff(m3(+1, 0,+1),i1,i2,i3)=0
                 coeff(m3(-1,+1,+1),i1,i2,i3)=0
                 coeff(m3( 0,+1,+1),i1,i2,i3)=0
                 coeff(m3(+1,+1,+1),i1,i2,i3)=0
! endLoops()
               end do
               end do
               end do
               end do
               end do
             else if( dir1.eq.1 .and. dir2.eq.1 )then
! beginLoops()
               ! ***** loop over equations and components *****
               do e=ea,eb
               do c=ca,cb
               ec=ns*(c+nc*e)
               ! ** it did not affect performance to use an array to index coeff ***
               if( nd.eq.2 )then
               do i2=-1,1
                 do i1=-1,1
                  m(i1,i2)=i1+1+3*(i2+1) +1 + ec
                 end do
               end do
               else if( nd.eq.3 )then
               do i3=-1,1
                 do i2=-1,1
                   do i1=-1,1
                     m3(i1,i2,i3)=i1+1+3*(i2+1+3*(i3+1)) +1 + ec
                   end do
                 end do
               end do
               else
               m12=1 + ec
               m22=2 + ec
               m32=3 + ec
               endif
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
! loopBody2ndOrder3d(0,0,0,0,0,0,0,0,0,0,a11(i1,i2,i3),0,0,-a11(i1,i2+1,i3)-a11(i1,i2,i3),0,0,a11(i1,i2+1,i3),0,0,0,0,0,0,0,0,0,0 )
                 coeff(m3(-1,-1,-1),i1,i2,i3)=0
                 coeff(m3( 0,-1,-1),i1,i2,i3)=0
                 coeff(m3(+1,-1,-1),i1,i2,i3)=0
                 coeff(m3(-1, 0,-1),i1,i2,i3)=0
                 coeff(m3( 0, 0,-1),i1,i2,i3)=0
                 coeff(m3(+1, 0,-1),i1,i2,i3)=0
                 coeff(m3(-1,+1,-1),i1,i2,i3)=0
                 coeff(m3( 0,+1,-1),i1,i2,i3)=0
                 coeff(m3(+1,+1,-1),i1,i2,i3)=0
                 coeff(m3(-1,-1, 0),i1,i2,i3)=0
                 coeff(m3( 0,-1, 0),i1,i2,i3)=a11(i1,i2,i3)
                 coeff(m3(+1,-1, 0),i1,i2,i3)=0
                 coeff(m3(-1, 0, 0),i1,i2,i3)=0
                 coeff(m3( 0, 0, 0),i1,i2,i3)=-a11(i1,i2+1,i3)-a11(i1,
     & i2,i3)
                 coeff(m3(+1, 0, 0),i1,i2,i3)=0
                 coeff(m3(-1,+1, 0),i1,i2,i3)=0
                 coeff(m3( 0,+1, 0),i1,i2,i3)=a11(i1,i2+1,i3)
                 coeff(m3(+1,+1, 0),i1,i2,i3)=0
                 coeff(m3(-1,-1,+1),i1,i2,i3)=0
                 coeff(m3( 0,-1,+1),i1,i2,i3)=0
                 coeff(m3(+1,-1,+1),i1,i2,i3)=0
                 coeff(m3(-1, 0,+1),i1,i2,i3)=0
                 coeff(m3( 0, 0,+1),i1,i2,i3)=0
                 coeff(m3(+1, 0,+1),i1,i2,i3)=0
                 coeff(m3(-1,+1,+1),i1,i2,i3)=0
                 coeff(m3( 0,+1,+1),i1,i2,i3)=0
                 coeff(m3(+1,+1,+1),i1,i2,i3)=0
! endLoops()
               end do
               end do
               end do
               end do
               end do
             else if( dir1.eq.1 .and. dir2.eq.2 )then
! beginLoops()
               ! ***** loop over equations and components *****
               do e=ea,eb
               do c=ca,cb
               ec=ns*(c+nc*e)
               ! ** it did not affect performance to use an array to index coeff ***
               if( nd.eq.2 )then
               do i2=-1,1
                 do i1=-1,1
                  m(i1,i2)=i1+1+3*(i2+1) +1 + ec
                 end do
               end do
               else if( nd.eq.3 )then
               do i3=-1,1
                 do i2=-1,1
                   do i1=-1,1
                     m3(i1,i2,i3)=i1+1+3*(i2+1+3*(i3+1)) +1 + ec
                   end do
                 end do
               end do
               else
               m12=1 + ec
               m22=2 + ec
               m32=3 + ec
               endif
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
! loopBody2ndOrder3d(0,a11(i1,i2,i3),0,0,-a11(i1,i2+1,i3)+a11(i1,i2,i3),0,0,-a11(i1,i2+1,i3),0,0,0,0,0,0,0,0,0,0,0,-a11(i1,i2,i3),0, 0,a11(i1,i2+1,i3)-a11(i1,i2,i3),0,0,a11(i1,i2+1,i3),0 )
                coeff(m3(-1,-1,-1),i1,i2,i3)=0
                coeff(m3( 0,-1,-1),i1,i2,i3)=a11(i1,i2,i3)
                coeff(m3(+1,-1,-1),i1,i2,i3)=0
                coeff(m3(-1, 0,-1),i1,i2,i3)=0
                coeff(m3( 0, 0,-1),i1,i2,i3)=-a11(i1,i2+1,i3)+a11(i1,
     & i2,i3)
                coeff(m3(+1, 0,-1),i1,i2,i3)=0
                coeff(m3(-1,+1,-1),i1,i2,i3)=0
                coeff(m3( 0,+1,-1),i1,i2,i3)=-a11(i1,i2+1,i3)
                coeff(m3(+1,+1,-1),i1,i2,i3)=0
                coeff(m3(-1,-1, 0),i1,i2,i3)=0
                coeff(m3( 0,-1, 0),i1,i2,i3)=0
                coeff(m3(+1,-1, 0),i1,i2,i3)=0
                coeff(m3(-1, 0, 0),i1,i2,i3)=0
                coeff(m3( 0, 0, 0),i1,i2,i3)=0
                coeff(m3(+1, 0, 0),i1,i2,i3)=0
                coeff(m3(-1,+1, 0),i1,i2,i3)=0
                coeff(m3( 0,+1, 0),i1,i2,i3)=0
                coeff(m3(+1,+1, 0),i1,i2,i3)=0
                coeff(m3(-1,-1,+1),i1,i2,i3)=0
                coeff(m3( 0,-1,+1),i1,i2,i3)=-a11(i1,i2,i3)
                coeff(m3(+1,-1,+1),i1,i2,i3)=0
                coeff(m3(-1, 0,+1),i1,i2,i3)=0
                coeff(m3( 0, 0,+1),i1,i2,i3)=a11(i1,i2+1,i3)-a11(i1,i2,
     & i3)
                coeff(m3(+1, 0,+1),i1,i2,i3)=0
                coeff(m3(-1,+1,+1),i1,i2,i3)=0
                coeff(m3( 0,+1,+1),i1,i2,i3)=a11(i1,i2+1,i3)
                coeff(m3(+1,+1,+1),i1,i2,i3)=0
! endLoops()
               end do
               end do
               end do
               end do
               end do
             else if( dir1.eq.2 .and. dir2.eq.0 )then
! beginLoops()
               ! ***** loop over equations and components *****
               do e=ea,eb
               do c=ca,cb
               ec=ns*(c+nc*e)
               ! ** it did not affect performance to use an array to index coeff ***
               if( nd.eq.2 )then
               do i2=-1,1
                 do i1=-1,1
                  m(i1,i2)=i1+1+3*(i2+1) +1 + ec
                 end do
               end do
               else if( nd.eq.3 )then
               do i3=-1,1
                 do i2=-1,1
                   do i1=-1,1
                     m3(i1,i2,i3)=i1+1+3*(i2+1+3*(i3+1)) +1 + ec
                   end do
                 end do
               end do
               else
               m12=1 + ec
               m22=2 + ec
               m32=3 + ec
               endif
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
! loopBody2ndOrder3d(0,0,0,a11(i1,i2,i3),0,-a11(i1,i2,i3),0,0,0,0,0,0,-a11(i1,i2,i3+1)+a11(i1,i2,i3),0, a11(i1,i2,i3+1)-a11(i1,i2,i3),0,0,0,0,0,0,-a11(i1,i2,i3+1),0,a11(i1,i2,i3+1),0,0,0 )
                 coeff(m3(-1,-1,-1),i1,i2,i3)=0
                 coeff(m3( 0,-1,-1),i1,i2,i3)=0
                 coeff(m3(+1,-1,-1),i1,i2,i3)=0
                 coeff(m3(-1, 0,-1),i1,i2,i3)=a11(i1,i2,i3)
                 coeff(m3( 0, 0,-1),i1,i2,i3)=0
                 coeff(m3(+1, 0,-1),i1,i2,i3)=-a11(i1,i2,i3)
                 coeff(m3(-1,+1,-1),i1,i2,i3)=0
                 coeff(m3( 0,+1,-1),i1,i2,i3)=0
                 coeff(m3(+1,+1,-1),i1,i2,i3)=0
                 coeff(m3(-1,-1, 0),i1,i2,i3)=0
                 coeff(m3( 0,-1, 0),i1,i2,i3)=0
                 coeff(m3(+1,-1, 0),i1,i2,i3)=0
                 coeff(m3(-1, 0, 0),i1,i2,i3)=-a11(i1,i2,i3+1)+a11(i1,
     & i2,i3)
                 coeff(m3( 0, 0, 0),i1,i2,i3)=0
                 coeff(m3(+1, 0, 0),i1,i2,i3)=a11(i1,i2,i3+1)-a11(i1,
     & i2,i3)
                 coeff(m3(-1,+1, 0),i1,i2,i3)=0
                 coeff(m3( 0,+1, 0),i1,i2,i3)=0
                 coeff(m3(+1,+1, 0),i1,i2,i3)=0
                 coeff(m3(-1,-1,+1),i1,i2,i3)=0
                 coeff(m3( 0,-1,+1),i1,i2,i3)=0
                 coeff(m3(+1,-1,+1),i1,i2,i3)=0
                 coeff(m3(-1, 0,+1),i1,i2,i3)=-a11(i1,i2,i3+1)
                 coeff(m3( 0, 0,+1),i1,i2,i3)=0
                 coeff(m3(+1, 0,+1),i1,i2,i3)=a11(i1,i2,i3+1)
                 coeff(m3(-1,+1,+1),i1,i2,i3)=0
                 coeff(m3( 0,+1,+1),i1,i2,i3)=0
                 coeff(m3(+1,+1,+1),i1,i2,i3)=0
! endLoops()
               end do
               end do
               end do
               end do
               end do
             else if( dir1.eq.2 .and. dir2.eq.1 )then
! beginLoops()
               ! ***** loop over equations and components *****
               do e=ea,eb
               do c=ca,cb
               ec=ns*(c+nc*e)
               ! ** it did not affect performance to use an array to index coeff ***
               if( nd.eq.2 )then
               do i2=-1,1
                 do i1=-1,1
                  m(i1,i2)=i1+1+3*(i2+1) +1 + ec
                 end do
               end do
               else if( nd.eq.3 )then
               do i3=-1,1
                 do i2=-1,1
                   do i1=-1,1
                     m3(i1,i2,i3)=i1+1+3*(i2+1+3*(i3+1)) +1 + ec
                   end do
                 end do
               end do
               else
               m12=1 + ec
               m22=2 + ec
               m32=3 + ec
               endif
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
! loopBody2ndOrder3d(0,a11(i1,i2,i3),0,0,0,0,0,-a11(i1,i2,i3),0,0,-a11(i1,i2,i3+1)+a11(i1,i2,i3),0,0,0,0,0, a11(i1,i2,i3+1)-a11(i1,i2,i3),0,0,-a11(i1,i2,i3+1),0,0,0,0,0,a11(i1,i2,i3+1),0 )
                 coeff(m3(-1,-1,-1),i1,i2,i3)=0
                 coeff(m3( 0,-1,-1),i1,i2,i3)=a11(i1,i2,i3)
                 coeff(m3(+1,-1,-1),i1,i2,i3)=0
                 coeff(m3(-1, 0,-1),i1,i2,i3)=0
                 coeff(m3( 0, 0,-1),i1,i2,i3)=0
                 coeff(m3(+1, 0,-1),i1,i2,i3)=0
                 coeff(m3(-1,+1,-1),i1,i2,i3)=0
                 coeff(m3( 0,+1,-1),i1,i2,i3)=-a11(i1,i2,i3)
                 coeff(m3(+1,+1,-1),i1,i2,i3)=0
                 coeff(m3(-1,-1, 0),i1,i2,i3)=0
                 coeff(m3( 0,-1, 0),i1,i2,i3)=-a11(i1,i2,i3+1)+a11(i1,
     & i2,i3)
                 coeff(m3(+1,-1, 0),i1,i2,i3)=0
                 coeff(m3(-1, 0, 0),i1,i2,i3)=0
                 coeff(m3( 0, 0, 0),i1,i2,i3)=0
                 coeff(m3(+1, 0, 0),i1,i2,i3)=0
                 coeff(m3(-1,+1, 0),i1,i2,i3)=0
                 coeff(m3( 0,+1, 0),i1,i2,i3)=a11(i1,i2,i3+1)-a11(i1,
     & i2,i3)
                 coeff(m3(+1,+1, 0),i1,i2,i3)=0
                 coeff(m3(-1,-1,+1),i1,i2,i3)=0
                 coeff(m3( 0,-1,+1),i1,i2,i3)=-a11(i1,i2,i3+1)
                 coeff(m3(+1,-1,+1),i1,i2,i3)=0
                 coeff(m3(-1, 0,+1),i1,i2,i3)=0
                 coeff(m3( 0, 0,+1),i1,i2,i3)=0
                 coeff(m3(+1, 0,+1),i1,i2,i3)=0
                 coeff(m3(-1,+1,+1),i1,i2,i3)=0
                 coeff(m3( 0,+1,+1),i1,i2,i3)=a11(i1,i2,i3+1)
                 coeff(m3(+1,+1,+1),i1,i2,i3)=0
! endLoops()
               end do
               end do
               end do
               end do
               end do
             else if( dir1.eq.2 .and. dir2.eq.2 )then
! beginLoops()
               ! ***** loop over equations and components *****
               do e=ea,eb
               do c=ca,cb
               ec=ns*(c+nc*e)
               ! ** it did not affect performance to use an array to index coeff ***
               if( nd.eq.2 )then
               do i2=-1,1
                 do i1=-1,1
                  m(i1,i2)=i1+1+3*(i2+1) +1 + ec
                 end do
               end do
               else if( nd.eq.3 )then
               do i3=-1,1
                 do i2=-1,1
                   do i1=-1,1
                     m3(i1,i2,i3)=i1+1+3*(i2+1+3*(i3+1)) +1 + ec
                   end do
                 end do
               end do
               else
               m12=1 + ec
               m22=2 + ec
               m32=3 + ec
               endif
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
! loopBody2ndOrder3d(0,0,0,0,a11(i1,i2,i3),0,0,0,0,0,0,0,0,-a11(i1,i2,i3+1)-a11(i1,i2,i3),0,0,0,0,0,0,0,0,a11(i1,i2,i3+1),0,0,0,0 )
                 coeff(m3(-1,-1,-1),i1,i2,i3)=0
                 coeff(m3( 0,-1,-1),i1,i2,i3)=0
                 coeff(m3(+1,-1,-1),i1,i2,i3)=0
                 coeff(m3(-1, 0,-1),i1,i2,i3)=0
                 coeff(m3( 0, 0,-1),i1,i2,i3)=a11(i1,i2,i3)
                 coeff(m3(+1, 0,-1),i1,i2,i3)=0
                 coeff(m3(-1,+1,-1),i1,i2,i3)=0
                 coeff(m3( 0,+1,-1),i1,i2,i3)=0
                 coeff(m3(+1,+1,-1),i1,i2,i3)=0
                 coeff(m3(-1,-1, 0),i1,i2,i3)=0
                 coeff(m3( 0,-1, 0),i1,i2,i3)=0
                 coeff(m3(+1,-1, 0),i1,i2,i3)=0
                 coeff(m3(-1, 0, 0),i1,i2,i3)=0
                 coeff(m3( 0, 0, 0),i1,i2,i3)=-a11(i1,i2,i3+1)-a11(i1,
     & i2,i3)
                 coeff(m3(+1, 0, 0),i1,i2,i3)=0
                 coeff(m3(-1,+1, 0),i1,i2,i3)=0
                 coeff(m3( 0,+1, 0),i1,i2,i3)=0
                 coeff(m3(+1,+1, 0),i1,i2,i3)=0
                 coeff(m3(-1,-1,+1),i1,i2,i3)=0
                 coeff(m3( 0,-1,+1),i1,i2,i3)=0
                 coeff(m3(+1,-1,+1),i1,i2,i3)=0
                 coeff(m3(-1, 0,+1),i1,i2,i3)=0
                 coeff(m3( 0, 0,+1),i1,i2,i3)=a11(i1,i2,i3+1)
                 coeff(m3(+1, 0,+1),i1,i2,i3)=0
                 coeff(m3(-1,+1,+1),i1,i2,i3)=0
                 coeff(m3( 0,+1,+1),i1,i2,i3)=0
                 coeff(m3(+1,+1,+1),i1,i2,i3)=0
! endLoops()
               end do
               end do
               end do
               end do
               end do
             end if
         else
       !  ***** not rectangular *****
!     #If "divScalarGrad" == "divScalarGrad"
! defineA23()
             m1a=n1a-1
             m1b=n1b+1
             m2a=n2a-1
             m2b=n2b+1
             m3a=n3a-1
             m3b=n3b+1
             if( averagingType .eq. arithmeticAverage )then
               factor=.5
! GETA23(s(j1,j2,j3,0)*jac(j1,j2,j3),c,factor)
               if( derivOption.eq.laplace )then
                 do j3=m3a,m3b
                   do j2=m2a,m2b
                     do j1=m1a,m1b
                       sj = jac(j1,j2,j3)
                       a11(j1,j2,j3) = (rx(j1,j2,j3)**2+ry(j1,j2,j3)**
     & 2+rz(j1,j2,j3)**2)*sj
                       a12(j1,j2,j3) = (rx(j1,j2,j3)*sx(j1,j2,j3)+ry(
     & j1,j2,j3)*sy(j1,j2,j3)+rz(j1,j2,j3)*sz(j1,j2,j3))*sj
                       a13(j1,j2,j3) = (rx(j1,j2,j3)*tx(j1,j2,j3)+ry(
     & j1,j2,j3)*ty(j1,j2,j3)+rz(j1,j2,j3)*tz(j1,j2,j3))*sj
                       a22(j1,j2,j3) = (sx(j1,j2,j3)**2+sy(j1,j2,j3)**
     & 2+sz(j1,j2,j3)**2)*sj
                       a23(j1,j2,j3) = (sx(j1,j2,j3)*tx(j1,j2,j3)+sy(
     & j1,j2,j3)*ty(j1,j2,j3)+sz(j1,j2,j3)*tz(j1,j2,j3))*sj
                       a33(j1,j2,j3) = (tx(j1,j2,j3)**2+ty(j1,j2,j3)**
     & 2+tz(j1,j2,j3)**2)*sj
                       a21(j1,j2,j3) = a12(j1,j2,j3)
                       a31(j1,j2,j3) = a13(j1,j2,j3)
                       a32(j1,j2,j3) = a23(j1,j2,j3)
                     end do
                   end do
                 end do
               else if( derivOption.eq.divScalarGrad )then
                 do j3=m3a,m3b
                   do j2=m2a,m2b
                     do j1=m1a,m1b
                       sj = s(j1,j2,j3,0)*jac(j1,j2,j3)
                       a11(j1,j2,j3) = (rx(j1,j2,j3)**2+ry(j1,j2,j3)**
     & 2+rz(j1,j2,j3)**2)*sj
                       a12(j1,j2,j3) = (rx(j1,j2,j3)*sx(j1,j2,j3)+ry(
     & j1,j2,j3)*sy(j1,j2,j3)+rz(j1,j2,j3)*sz(j1,j2,j3))*sj
                       a13(j1,j2,j3) = (rx(j1,j2,j3)*tx(j1,j2,j3)+ry(
     & j1,j2,j3)*ty(j1,j2,j3)+rz(j1,j2,j3)*tz(j1,j2,j3))*sj
                       a22(j1,j2,j3) = (sx(j1,j2,j3)**2+sy(j1,j2,j3)**
     & 2+sz(j1,j2,j3)**2)*sj
                       a23(j1,j2,j3) = (sx(j1,j2,j3)*tx(j1,j2,j3)+sy(
     & j1,j2,j3)*ty(j1,j2,j3)+sz(j1,j2,j3)*tz(j1,j2,j3))*sj
                       a33(j1,j2,j3) = (tx(j1,j2,j3)**2+ty(j1,j2,j3)**
     & 2+tz(j1,j2,j3)**2)*sj
                       a21(j1,j2,j3) = a12(j1,j2,j3)
                       a31(j1,j2,j3) = a13(j1,j2,j3)
                       a32(j1,j2,j3) = a23(j1,j2,j3)
                     end do
                   end do
                 end do
               else if( derivOption.eq.divTensorGrad )then
                 do j3=m3a,m3b
                   do j2=m2a,m2b
                     do j1=m1a,m1b
                       sj =jac(j1,j2,j3)
                       s11=s(j1,j2,j3,0)
                       s21=s(j1,j2,j3,1)
                       s31=s(j1,j2,j3,2)
                       s12=s(j1,j2,j3,3)
                       s22=s(j1,j2,j3,4)
                       s32=s(j1,j2,j3,5)
                       s13=s(j1,j2,j3,6)
                       s23=s(j1,j2,j3,7)
                       s33=s(j1,j2,j3,8)
                       rxj=rx(j1,j2,j3)
                       ryj=ry(j1,j2,j3)
                       rzj=rz(j1,j2,j3)
                       sxj=sx(j1,j2,j3)
                       syj=sy(j1,j2,j3)
                       szj=sz(j1,j2,j3)
                       txj=tx(j1,j2,j3)
                       tyj=ty(j1,j2,j3)
                       tzj=tz(j1,j2,j3)
                       a11(j1,j2,j3) = (s11*rxj**2+s22*ryj**2+s33*rzj**
     & 2+(s12+s21)*rxj*ryj+(s13+s31)*rxj*rzj+(s23+s32)*ryj*rzj)*sj
                       a22(j1,j2,j3) = (s11*sxj**2+s22*syj**2+s33*szj**
     & 2+(s12+s21)*sxj*syj+(s13+s31)*sxj*szj+(s23+s32)*syj*szj)*sj
                       a33(j1,j2,j3) = (s11*txj**2+s22*tyj**2+s33*tzj**
     & 2+(s12+s21)*txj*tyj+(s13+s31)*txj*tzj+(s23+s32)*tyj*tzj)*sj
                       a12(j1,j2,j3) = (s11*rxj*sxj+s22*ryj*syj+s33*
     & rzj*szj+s12*rxj*syj+s13*rxj*szj+s21*ryj*sxj+s23*ryj*szj+s31*
     & rzj*sxj+s32*rzj*syj)*sj
                       a13(j1,j2,j3) = (s11*rxj*txj+s22*ryj*tyj+s33*
     & rzj*tzj+s12*rxj*tyj+s13*rxj*tzj+s21*ryj*txj+s23*ryj*tzj+s31*
     & rzj*txj+s32*rzj*tyj)*sj
                       a23(j1,j2,j3) = (s11*sxj*txj+s22*syj*tyj+s33*
     & szj*tzj+s12*sxj*tyj+s13*sxj*tzj+s21*syj*txj+s23*syj*tzj+s31*
     & szj*txj+s32*szj*tyj)*sj
                       a21(j1,j2,j3) = (s11*sxj*rxj+s22*syj*ryj+s33*
     & szj*rzj+s12*sxj*ryj+s13*sxj*rzj+s21*syj*rxj+s23*syj*rzj+s31*
     & szj*rxj+s32*szj*ryj)*sj
                       a31(j1,j2,j3) = (s11*txj*rxj+s22*tyj*ryj+s33*
     & tzj*rzj+s12*txj*ryj+s13*txj*rzj+s21*tyj*rxj+s23*tyj*rzj+s31*
     & tzj*rxj+s32*tzj*ryj)*sj
                       a32(j1,j2,j3) = (s11*txj*sxj+s22*tyj*syj+s33*
     & tzj*szj+s12*txj*syj+s13*txj*szj+s21*tyj*sxj+s23*tyj*szj+s31*
     & tzj*sxj+s32*tzj*syj)*sj
                     end do
                   end do
                 end do
               else if( derivOption.eq.derivativeScalarDerivative )then
                 if(      dir1.eq.0 .and. dir2.eq.0 )then
! DXSDY23(x,x,s(j1,j2,j3,0)*jac(j1,j2,j3))
                   do j3=m3a,m3b
                     do j2=m2a,m2b
                       do j1=m1a,m1b
                         sj = s(j1,j2,j3,0)*jac(j1,j2,j3)
                         a11(j1,j2,j3) = (r x (j1,j2,j3)*r x (j1,j2,j3)
     & )*sj
                         a12(j1,j2,j3) = (r x (j1,j2,j3)*s x (j1,j2,j3)
     & )*sj
                         a13(j1,j2,j3) = (r x (j1,j2,j3)*t x (j1,j2,j3)
     & )*sj
                         a22(j1,j2,j3) = (s x (j1,j2,j3)*s x (j1,j2,j3)
     & )*sj
                         a23(j1,j2,j3) = (s x (j1,j2,j3)*t x (j1,j2,j3)
     & )*sj
                         a33(j1,j2,j3) = (t x (j1,j2,j3)*t x (j1,j2,j3)
     & )*sj
! #If "x" == "x"
                         a21(j1,j2,j3) = a12(j1,j2,j3)
                         a31(j1,j2,j3) = a13(j1,j2,j3)
                         a32(j1,j2,j3) = a23(j1,j2,j3)
                       end do
                     end do
                   end do
                 else if( dir1.eq.0 .and. dir2.eq.1 )then
! DXSDY23(x,y,s(j1,j2,j3,0)*jac(j1,j2,j3))
                   do j3=m3a,m3b
                     do j2=m2a,m2b
                       do j1=m1a,m1b
                         sj = s(j1,j2,j3,0)*jac(j1,j2,j3)
                         a11(j1,j2,j3) = (r x (j1,j2,j3)*r y (j1,j2,j3)
     & )*sj
                         a12(j1,j2,j3) = (r x (j1,j2,j3)*s y (j1,j2,j3)
     & )*sj
                         a13(j1,j2,j3) = (r x (j1,j2,j3)*t y (j1,j2,j3)
     & )*sj
                         a22(j1,j2,j3) = (s x (j1,j2,j3)*s y (j1,j2,j3)
     & )*sj
                         a23(j1,j2,j3) = (s x (j1,j2,j3)*t y (j1,j2,j3)
     & )*sj
                         a33(j1,j2,j3) = (t x (j1,j2,j3)*t y (j1,j2,j3)
     & )*sj
! #If "x" == "y"
! #Else
                         a21(j1,j2,j3) = (s x (j1,j2,j3)*r y (j1,j2,j3)
     & )*sj
                         a31(j1,j2,j3) = (t x (j1,j2,j3)*r y (j1,j2,j3)
     & )*sj
                         a32(j1,j2,j3) = (t x (j1,j2,j3)*s y (j1,j2,j3)
     & )*sj
                       end do
                     end do
                   end do
                 else if( dir1.eq.0 .and. dir2.eq.2 )then
! DXSDY23(x,z,s(j1,j2,j3,0)*jac(j1,j2,j3))
                   do j3=m3a,m3b
                     do j2=m2a,m2b
                       do j1=m1a,m1b
                         sj = s(j1,j2,j3,0)*jac(j1,j2,j3)
                         a11(j1,j2,j3) = (r x (j1,j2,j3)*r z (j1,j2,j3)
     & )*sj
                         a12(j1,j2,j3) = (r x (j1,j2,j3)*s z (j1,j2,j3)
     & )*sj
                         a13(j1,j2,j3) = (r x (j1,j2,j3)*t z (j1,j2,j3)
     & )*sj
                         a22(j1,j2,j3) = (s x (j1,j2,j3)*s z (j1,j2,j3)
     & )*sj
                         a23(j1,j2,j3) = (s x (j1,j2,j3)*t z (j1,j2,j3)
     & )*sj
                         a33(j1,j2,j3) = (t x (j1,j2,j3)*t z (j1,j2,j3)
     & )*sj
! #If "x" == "z"
! #Else
                         a21(j1,j2,j3) = (s x (j1,j2,j3)*r z (j1,j2,j3)
     & )*sj
                         a31(j1,j2,j3) = (t x (j1,j2,j3)*r z (j1,j2,j3)
     & )*sj
                         a32(j1,j2,j3) = (t x (j1,j2,j3)*s z (j1,j2,j3)
     & )*sj
                       end do
                     end do
                   end do
                 else if( dir1.eq.1 .and. dir2.eq.0 )then
! DXSDY23(y,x,s(j1,j2,j3,0)*jac(j1,j2,j3))
                   do j3=m3a,m3b
                     do j2=m2a,m2b
                       do j1=m1a,m1b
                         sj = s(j1,j2,j3,0)*jac(j1,j2,j3)
                         a11(j1,j2,j3) = (r y (j1,j2,j3)*r x (j1,j2,j3)
     & )*sj
                         a12(j1,j2,j3) = (r y (j1,j2,j3)*s x (j1,j2,j3)
     & )*sj
                         a13(j1,j2,j3) = (r y (j1,j2,j3)*t x (j1,j2,j3)
     & )*sj
                         a22(j1,j2,j3) = (s y (j1,j2,j3)*s x (j1,j2,j3)
     & )*sj
                         a23(j1,j2,j3) = (s y (j1,j2,j3)*t x (j1,j2,j3)
     & )*sj
                         a33(j1,j2,j3) = (t y (j1,j2,j3)*t x (j1,j2,j3)
     & )*sj
! #If "y" == "x"
! #Else
                         a21(j1,j2,j3) = (s y (j1,j2,j3)*r x (j1,j2,j3)
     & )*sj
                         a31(j1,j2,j3) = (t y (j1,j2,j3)*r x (j1,j2,j3)
     & )*sj
                         a32(j1,j2,j3) = (t y (j1,j2,j3)*s x (j1,j2,j3)
     & )*sj
                       end do
                     end do
                   end do
                 else if( dir1.eq.1 .and. dir2.eq.1 )then
! DXSDY23(y,y,s(j1,j2,j3,0)*jac(j1,j2,j3))
                   do j3=m3a,m3b
                     do j2=m2a,m2b
                       do j1=m1a,m1b
                         sj = s(j1,j2,j3,0)*jac(j1,j2,j3)
                         a11(j1,j2,j3) = (r y (j1,j2,j3)*r y (j1,j2,j3)
     & )*sj
                         a12(j1,j2,j3) = (r y (j1,j2,j3)*s y (j1,j2,j3)
     & )*sj
                         a13(j1,j2,j3) = (r y (j1,j2,j3)*t y (j1,j2,j3)
     & )*sj
                         a22(j1,j2,j3) = (s y (j1,j2,j3)*s y (j1,j2,j3)
     & )*sj
                         a23(j1,j2,j3) = (s y (j1,j2,j3)*t y (j1,j2,j3)
     & )*sj
                         a33(j1,j2,j3) = (t y (j1,j2,j3)*t y (j1,j2,j3)
     & )*sj
! #If "y" == "y"
                         a21(j1,j2,j3) = a12(j1,j2,j3)
                         a31(j1,j2,j3) = a13(j1,j2,j3)
                         a32(j1,j2,j3) = a23(j1,j2,j3)
                       end do
                     end do
                   end do
                 else if( dir1.eq.1 .and. dir2.eq.2 )then
! DXSDY23(y,z,s(j1,j2,j3,0)*jac(j1,j2,j3))
                   do j3=m3a,m3b
                     do j2=m2a,m2b
                       do j1=m1a,m1b
                         sj = s(j1,j2,j3,0)*jac(j1,j2,j3)
                         a11(j1,j2,j3) = (r y (j1,j2,j3)*r z (j1,j2,j3)
     & )*sj
                         a12(j1,j2,j3) = (r y (j1,j2,j3)*s z (j1,j2,j3)
     & )*sj
                         a13(j1,j2,j3) = (r y (j1,j2,j3)*t z (j1,j2,j3)
     & )*sj
                         a22(j1,j2,j3) = (s y (j1,j2,j3)*s z (j1,j2,j3)
     & )*sj
                         a23(j1,j2,j3) = (s y (j1,j2,j3)*t z (j1,j2,j3)
     & )*sj
                         a33(j1,j2,j3) = (t y (j1,j2,j3)*t z (j1,j2,j3)
     & )*sj
! #If "y" == "z"
! #Else
                         a21(j1,j2,j3) = (s y (j1,j2,j3)*r z (j1,j2,j3)
     & )*sj
                         a31(j1,j2,j3) = (t y (j1,j2,j3)*r z (j1,j2,j3)
     & )*sj
                         a32(j1,j2,j3) = (t y (j1,j2,j3)*s z (j1,j2,j3)
     & )*sj
                       end do
                     end do
                   end do
                 else if( dir1.eq.2 .and. dir2.eq.0 )then
! DXSDY23(z,x,s(j1,j2,j3,0)*jac(j1,j2,j3))
                   do j3=m3a,m3b
                     do j2=m2a,m2b
                       do j1=m1a,m1b
                         sj = s(j1,j2,j3,0)*jac(j1,j2,j3)
                         a11(j1,j2,j3) = (r z (j1,j2,j3)*r x (j1,j2,j3)
     & )*sj
                         a12(j1,j2,j3) = (r z (j1,j2,j3)*s x (j1,j2,j3)
     & )*sj
                         a13(j1,j2,j3) = (r z (j1,j2,j3)*t x (j1,j2,j3)
     & )*sj
                         a22(j1,j2,j3) = (s z (j1,j2,j3)*s x (j1,j2,j3)
     & )*sj
                         a23(j1,j2,j3) = (s z (j1,j2,j3)*t x (j1,j2,j3)
     & )*sj
                         a33(j1,j2,j3) = (t z (j1,j2,j3)*t x (j1,j2,j3)
     & )*sj
! #If "z" == "x"
! #Else
                         a21(j1,j2,j3) = (s z (j1,j2,j3)*r x (j1,j2,j3)
     & )*sj
                         a31(j1,j2,j3) = (t z (j1,j2,j3)*r x (j1,j2,j3)
     & )*sj
                         a32(j1,j2,j3) = (t z (j1,j2,j3)*s x (j1,j2,j3)
     & )*sj
                       end do
                     end do
                   end do
                 else if( dir1.eq.2 .and. dir2.eq.1 )then
! DXSDY23(z,y,s(j1,j2,j3,0)*jac(j1,j2,j3))
                   do j3=m3a,m3b
                     do j2=m2a,m2b
                       do j1=m1a,m1b
                         sj = s(j1,j2,j3,0)*jac(j1,j2,j3)
                         a11(j1,j2,j3) = (r z (j1,j2,j3)*r y (j1,j2,j3)
     & )*sj
                         a12(j1,j2,j3) = (r z (j1,j2,j3)*s y (j1,j2,j3)
     & )*sj
                         a13(j1,j2,j3) = (r z (j1,j2,j3)*t y (j1,j2,j3)
     & )*sj
                         a22(j1,j2,j3) = (s z (j1,j2,j3)*s y (j1,j2,j3)
     & )*sj
                         a23(j1,j2,j3) = (s z (j1,j2,j3)*t y (j1,j2,j3)
     & )*sj
                         a33(j1,j2,j3) = (t z (j1,j2,j3)*t y (j1,j2,j3)
     & )*sj
! #If "z" == "y"
! #Else
                         a21(j1,j2,j3) = (s z (j1,j2,j3)*r y (j1,j2,j3)
     & )*sj
                         a31(j1,j2,j3) = (t z (j1,j2,j3)*r y (j1,j2,j3)
     & )*sj
                         a32(j1,j2,j3) = (t z (j1,j2,j3)*s y (j1,j2,j3)
     & )*sj
                       end do
                     end do
                   end do
                 else if( dir1.eq.2 .and. dir2.eq.2 )then
! DXSDY23(z,z,s(j1,j2,j3,0)*jac(j1,j2,j3))
                   do j3=m3a,m3b
                     do j2=m2a,m2b
                       do j1=m1a,m1b
                         sj = s(j1,j2,j3,0)*jac(j1,j2,j3)
                         a11(j1,j2,j3) = (r z (j1,j2,j3)*r z (j1,j2,j3)
     & )*sj
                         a12(j1,j2,j3) = (r z (j1,j2,j3)*s z (j1,j2,j3)
     & )*sj
                         a13(j1,j2,j3) = (r z (j1,j2,j3)*t z (j1,j2,j3)
     & )*sj
                         a22(j1,j2,j3) = (s z (j1,j2,j3)*s z (j1,j2,j3)
     & )*sj
                         a23(j1,j2,j3) = (s z (j1,j2,j3)*t z (j1,j2,j3)
     & )*sj
                         a33(j1,j2,j3) = (t z (j1,j2,j3)*t z (j1,j2,j3)
     & )*sj
! #If "z" == "z"
                         a21(j1,j2,j3) = a12(j1,j2,j3)
                         a31(j1,j2,j3) = a13(j1,j2,j3)
                         a32(j1,j2,j3) = a23(j1,j2,j3)
                       end do
                     end do
                   end do
                 else
                   write(*,*) 'ERROR invalid values: dir1=',dir1,' 
     & dir2=',dir2
                 end if
               end if
               if( derivType.eq.symmetric )then
               ! symmetric case -- do not average a12, a21, a13, z31, etc.  -- could do better
                m1a=n1a
                do j3=m3a,m3b
                 do j2=m2a,m2b
                   do j1=m1b,m1a,-1 ! go backwards   worry about division by zero
c     sh=s(j1,j2,j3,0)*s(j1-1,j2,j3,0)/(s(j1,j2,j3,0)+s(j1-1,j2,j3,0))
                     a11(j1,j2,j3) = factor *d22(1)*(a11(j1,j2,j3)+a11(
     & j1-1,j2,j3))
                   end do
                 end do
                end do
                m1a=n1a-1
                do j3=m3a,m3b
                 do j2=m2a,m2b
                   do j1=m1b,m1a,-1
! #If "c" eq "c"
                     a12(j1,j2,j3) =         (d12(1)*d12(2))*a12(j1,j2,
     & j3)
                     a13(j1,j2,j3) =         (d12(1)*d12(3))*a13(j1,j2,
     & j3)
                   end do
                 end do
                end do
                m2a=n2a
                do j3=m3a,m3b
                 do j2=m2b,m2a,-1
                   do j1=m1a,m1b
c     sh=s(j1,j2,j3,0)*s(j1,j2-1,j3,0)/(s(j1,j2,j3,0)+s(j1,j2-1,j3,0)) 
                     a22(j1,j2,j3) = factor *d22(2)*(a22(j1,j2,j3)+a22(
     & j1,j2-1,j3))
                   end do
                 end do
                end do
                m2a=n2a-1
                do j3=m3a,m3b
                 do j2=m2b,m2a,-1
                   do j1=m1a,m1b
! #If "c" eq "c"
                     a21(j1,j2,j3) =         (d12(1)*d12(2))*a21(j1,j2,
     & j3)
                     a23(j1,j2,j3) =         (d12(2)*d12(3))*a23(j1,j2,
     & j3)
                   end do
                 end do
                end do
                m3a=n3a
                do j3=m3b,m3a,-1
                 do j2=m2a,m2b
                   do j1=m1a,m1b
c     sh=s(j1,j2,j3,0)*s(j1,j2,j3-1,0)/(s(j1,j2,j3,0)+s(j1,j2,j3-1,0)) 
                     a33(j1,j2,j3) = factor *d22(3)*(a33(j1,j2,j3)+a33(
     & j1,j2,j3-1))
                   end do
                 end do
                end do
                m3a=n3a-1
                do j3=m3b,m3a,-1
                 do j2=m2a,m2b
                   do j1=m1a,m1b
! #If "c" eq "c"
                     a31(j1,j2,j3) =         (d12(1)*d12(3))*a31(j1,j2,
     & j3)
                     a32(j1,j2,j3) =         (d12(2)*d12(3))*a32(j1,j2,
     & j3)
                   end do
                 end do
                end do
                  else
                ! ** old way -- average all coefficients
               m1a=n1a
               do j3=m3a,m3b
                 do j2=m2a,m2b
                   do j1=m1b,m1a,-1 ! go backwards  worry about division by zero
c     sh=s(j1,j2,j3,0)*s(j1-1,j2,j3,0)/(s(j1,j2,j3,0)+s(j1-1,j2,j3,0)) 
                     a11(j1,j2,j3) = factor *d22(1)*(a11(j1,j2,j3)+a11(
     & j1-1,j2,j3))
                     a12(j1,j2,j3) = factor *(d12(1)*d12(2))*(a12(j1,
     & j2,j3)+a12(j1-1,j2,j3))
                     a13(j1,j2,j3) = factor *(d12(1)*d12(3))*(a13(j1,
     & j2,j3)+a13(j1-1,j2,j3))
                   end do
                 end do
               end do
               m1a=n1a-1
               m2a=n2a
               do j3=m3a,m3b
                 do j2=m2b,m2a,-1
                   do j1=m1a,m1b
c     sh=s(j1,j2,j3,0)*s(j1,j2-1,j3,0)/(s(j1,j2,j3,0)+s(j1,j2-1,j3,0)) 
                     a21(j1,j2,j3) = factor *(d12(1)*d12(2))*(a21(j1,
     & j2,j3)+a21(j1,j2-1,j3))
                     a22(j1,j2,j3) = factor *d22(2)*(a22(j1,j2,j3)+a22(
     & j1,j2-1,j3))
                     a23(j1,j2,j3) = factor *(d12(2)*d12(3))*(a23(j1,
     & j2,j3)+a23(j1,j2-1,j3))
                   end do
                 end do
               end do
               m2a=n2a-1
               m3a=n3a
               do j3=m3b,m3a,-1
                 do j2=m2a,m2b
                   do j1=m1a,m1b
c     sh=s(j1,j2,j3,0)*s(j1,j2,j3-1,0)/(s(j1,j2,j3,0)+s(j1,j2,j3-1,0)) 
                     a31(j1,j2,j3) = factor *(d12(1)*d12(3))*(a31(j1,
     & j2,j3)+a31(j1,j2,j3-1))
                     a32(j1,j2,j3) = factor *(d12(2)*d12(3))*(a32(j1,
     & j2,j3)+a32(j1,j2,j3-1))
                     a33(j1,j2,j3) = factor *d22(3)*(a33(j1,j2,j3)+a33(
     & j1,j2,j3-1))
                   end do
                 end do
               end do
               m3a=n3a-1
               end if
             else
c  Harmonic average
               factor=2.
c  do not average in s:  
! GETA23(jac(j1,j2,j3), ,sh)
               if( derivOption.eq.laplace )then
                 do j3=m3a,m3b
                   do j2=m2a,m2b
                     do j1=m1a,m1b
                       sj = jac(j1,j2,j3)
                       a11(j1,j2,j3) = (rx(j1,j2,j3)**2+ry(j1,j2,j3)**
     & 2+rz(j1,j2,j3)**2)*sj
                       a12(j1,j2,j3) = (rx(j1,j2,j3)*sx(j1,j2,j3)+ry(
     & j1,j2,j3)*sy(j1,j2,j3)+rz(j1,j2,j3)*sz(j1,j2,j3))*sj
                       a13(j1,j2,j3) = (rx(j1,j2,j3)*tx(j1,j2,j3)+ry(
     & j1,j2,j3)*ty(j1,j2,j3)+rz(j1,j2,j3)*tz(j1,j2,j3))*sj
                       a22(j1,j2,j3) = (sx(j1,j2,j3)**2+sy(j1,j2,j3)**
     & 2+sz(j1,j2,j3)**2)*sj
                       a23(j1,j2,j3) = (sx(j1,j2,j3)*tx(j1,j2,j3)+sy(
     & j1,j2,j3)*ty(j1,j2,j3)+sz(j1,j2,j3)*tz(j1,j2,j3))*sj
                       a33(j1,j2,j3) = (tx(j1,j2,j3)**2+ty(j1,j2,j3)**
     & 2+tz(j1,j2,j3)**2)*sj
                       a21(j1,j2,j3) = a12(j1,j2,j3)
                       a31(j1,j2,j3) = a13(j1,j2,j3)
                       a32(j1,j2,j3) = a23(j1,j2,j3)
                     end do
                   end do
                 end do
               else if( derivOption.eq.divScalarGrad )then
                 do j3=m3a,m3b
                   do j2=m2a,m2b
                     do j1=m1a,m1b
                       sj = jac(j1,j2,j3)
                       a11(j1,j2,j3) = (rx(j1,j2,j3)**2+ry(j1,j2,j3)**
     & 2+rz(j1,j2,j3)**2)*sj
                       a12(j1,j2,j3) = (rx(j1,j2,j3)*sx(j1,j2,j3)+ry(
     & j1,j2,j3)*sy(j1,j2,j3)+rz(j1,j2,j3)*sz(j1,j2,j3))*sj
                       a13(j1,j2,j3) = (rx(j1,j2,j3)*tx(j1,j2,j3)+ry(
     & j1,j2,j3)*ty(j1,j2,j3)+rz(j1,j2,j3)*tz(j1,j2,j3))*sj
                       a22(j1,j2,j3) = (sx(j1,j2,j3)**2+sy(j1,j2,j3)**
     & 2+sz(j1,j2,j3)**2)*sj
                       a23(j1,j2,j3) = (sx(j1,j2,j3)*tx(j1,j2,j3)+sy(
     & j1,j2,j3)*ty(j1,j2,j3)+sz(j1,j2,j3)*tz(j1,j2,j3))*sj
                       a33(j1,j2,j3) = (tx(j1,j2,j3)**2+ty(j1,j2,j3)**
     & 2+tz(j1,j2,j3)**2)*sj
                       a21(j1,j2,j3) = a12(j1,j2,j3)
                       a31(j1,j2,j3) = a13(j1,j2,j3)
                       a32(j1,j2,j3) = a23(j1,j2,j3)
                     end do
                   end do
                 end do
               else if( derivOption.eq.divTensorGrad )then
                 do j3=m3a,m3b
                   do j2=m2a,m2b
                     do j1=m1a,m1b
                       sj =jac(j1,j2,j3)
                       s11=s(j1,j2,j3,0)
                       s21=s(j1,j2,j3,1)
                       s31=s(j1,j2,j3,2)
                       s12=s(j1,j2,j3,3)
                       s22=s(j1,j2,j3,4)
                       s32=s(j1,j2,j3,5)
                       s13=s(j1,j2,j3,6)
                       s23=s(j1,j2,j3,7)
                       s33=s(j1,j2,j3,8)
                       rxj=rx(j1,j2,j3)
                       ryj=ry(j1,j2,j3)
                       rzj=rz(j1,j2,j3)
                       sxj=sx(j1,j2,j3)
                       syj=sy(j1,j2,j3)
                       szj=sz(j1,j2,j3)
                       txj=tx(j1,j2,j3)
                       tyj=ty(j1,j2,j3)
                       tzj=tz(j1,j2,j3)
                       a11(j1,j2,j3) = (s11*rxj**2+s22*ryj**2+s33*rzj**
     & 2+(s12+s21)*rxj*ryj+(s13+s31)*rxj*rzj+(s23+s32)*ryj*rzj)*sj
                       a22(j1,j2,j3) = (s11*sxj**2+s22*syj**2+s33*szj**
     & 2+(s12+s21)*sxj*syj+(s13+s31)*sxj*szj+(s23+s32)*syj*szj)*sj
                       a33(j1,j2,j3) = (s11*txj**2+s22*tyj**2+s33*tzj**
     & 2+(s12+s21)*txj*tyj+(s13+s31)*txj*tzj+(s23+s32)*tyj*tzj)*sj
                       a12(j1,j2,j3) = (s11*rxj*sxj+s22*ryj*syj+s33*
     & rzj*szj+s12*rxj*syj+s13*rxj*szj+s21*ryj*sxj+s23*ryj*szj+s31*
     & rzj*sxj+s32*rzj*syj)*sj
                       a13(j1,j2,j3) = (s11*rxj*txj+s22*ryj*tyj+s33*
     & rzj*tzj+s12*rxj*tyj+s13*rxj*tzj+s21*ryj*txj+s23*ryj*tzj+s31*
     & rzj*txj+s32*rzj*tyj)*sj
                       a23(j1,j2,j3) = (s11*sxj*txj+s22*syj*tyj+s33*
     & szj*tzj+s12*sxj*tyj+s13*sxj*tzj+s21*syj*txj+s23*syj*tzj+s31*
     & szj*txj+s32*szj*tyj)*sj
                       a21(j1,j2,j3) = (s11*sxj*rxj+s22*syj*ryj+s33*
     & szj*rzj+s12*sxj*ryj+s13*sxj*rzj+s21*syj*rxj+s23*syj*rzj+s31*
     & szj*rxj+s32*szj*ryj)*sj
                       a31(j1,j2,j3) = (s11*txj*rxj+s22*tyj*ryj+s33*
     & tzj*rzj+s12*txj*ryj+s13*txj*rzj+s21*tyj*rxj+s23*tyj*rzj+s31*
     & tzj*rxj+s32*tzj*ryj)*sj
                       a32(j1,j2,j3) = (s11*txj*sxj+s22*tyj*syj+s33*
     & tzj*szj+s12*txj*syj+s13*txj*szj+s21*tyj*sxj+s23*tyj*szj+s31*
     & tzj*sxj+s32*tzj*syj)*sj
                     end do
                   end do
                 end do
               else if( derivOption.eq.derivativeScalarDerivative )then
                 if(      dir1.eq.0 .and. dir2.eq.0 )then
! DXSDY23(x,x,jac(j1,j2,j3))
                   do j3=m3a,m3b
                     do j2=m2a,m2b
                       do j1=m1a,m1b
                         sj = jac(j1,j2,j3)
                         a11(j1,j2,j3) = (r x (j1,j2,j3)*r x (j1,j2,j3)
     & )*sj
                         a12(j1,j2,j3) = (r x (j1,j2,j3)*s x (j1,j2,j3)
     & )*sj
                         a13(j1,j2,j3) = (r x (j1,j2,j3)*t x (j1,j2,j3)
     & )*sj
                         a22(j1,j2,j3) = (s x (j1,j2,j3)*s x (j1,j2,j3)
     & )*sj
                         a23(j1,j2,j3) = (s x (j1,j2,j3)*t x (j1,j2,j3)
     & )*sj
                         a33(j1,j2,j3) = (t x (j1,j2,j3)*t x (j1,j2,j3)
     & )*sj
! #If "x" == "x"
                         a21(j1,j2,j3) = a12(j1,j2,j3)
                         a31(j1,j2,j3) = a13(j1,j2,j3)
                         a32(j1,j2,j3) = a23(j1,j2,j3)
                       end do
                     end do
                   end do
                 else if( dir1.eq.0 .and. dir2.eq.1 )then
! DXSDY23(x,y,jac(j1,j2,j3))
                   do j3=m3a,m3b
                     do j2=m2a,m2b
                       do j1=m1a,m1b
                         sj = jac(j1,j2,j3)
                         a11(j1,j2,j3) = (r x (j1,j2,j3)*r y (j1,j2,j3)
     & )*sj
                         a12(j1,j2,j3) = (r x (j1,j2,j3)*s y (j1,j2,j3)
     & )*sj
                         a13(j1,j2,j3) = (r x (j1,j2,j3)*t y (j1,j2,j3)
     & )*sj
                         a22(j1,j2,j3) = (s x (j1,j2,j3)*s y (j1,j2,j3)
     & )*sj
                         a23(j1,j2,j3) = (s x (j1,j2,j3)*t y (j1,j2,j3)
     & )*sj
                         a33(j1,j2,j3) = (t x (j1,j2,j3)*t y (j1,j2,j3)
     & )*sj
! #If "x" == "y"
! #Else
                         a21(j1,j2,j3) = (s x (j1,j2,j3)*r y (j1,j2,j3)
     & )*sj
                         a31(j1,j2,j3) = (t x (j1,j2,j3)*r y (j1,j2,j3)
     & )*sj
                         a32(j1,j2,j3) = (t x (j1,j2,j3)*s y (j1,j2,j3)
     & )*sj
                       end do
                     end do
                   end do
                 else if( dir1.eq.0 .and. dir2.eq.2 )then
! DXSDY23(x,z,jac(j1,j2,j3))
                   do j3=m3a,m3b
                     do j2=m2a,m2b
                       do j1=m1a,m1b
                         sj = jac(j1,j2,j3)
                         a11(j1,j2,j3) = (r x (j1,j2,j3)*r z (j1,j2,j3)
     & )*sj
                         a12(j1,j2,j3) = (r x (j1,j2,j3)*s z (j1,j2,j3)
     & )*sj
                         a13(j1,j2,j3) = (r x (j1,j2,j3)*t z (j1,j2,j3)
     & )*sj
                         a22(j1,j2,j3) = (s x (j1,j2,j3)*s z (j1,j2,j3)
     & )*sj
                         a23(j1,j2,j3) = (s x (j1,j2,j3)*t z (j1,j2,j3)
     & )*sj
                         a33(j1,j2,j3) = (t x (j1,j2,j3)*t z (j1,j2,j3)
     & )*sj
! #If "x" == "z"
! #Else
                         a21(j1,j2,j3) = (s x (j1,j2,j3)*r z (j1,j2,j3)
     & )*sj
                         a31(j1,j2,j3) = (t x (j1,j2,j3)*r z (j1,j2,j3)
     & )*sj
                         a32(j1,j2,j3) = (t x (j1,j2,j3)*s z (j1,j2,j3)
     & )*sj
                       end do
                     end do
                   end do
                 else if( dir1.eq.1 .and. dir2.eq.0 )then
! DXSDY23(y,x,jac(j1,j2,j3))
                   do j3=m3a,m3b
                     do j2=m2a,m2b
                       do j1=m1a,m1b
                         sj = jac(j1,j2,j3)
                         a11(j1,j2,j3) = (r y (j1,j2,j3)*r x (j1,j2,j3)
     & )*sj
                         a12(j1,j2,j3) = (r y (j1,j2,j3)*s x (j1,j2,j3)
     & )*sj
                         a13(j1,j2,j3) = (r y (j1,j2,j3)*t x (j1,j2,j3)
     & )*sj
                         a22(j1,j2,j3) = (s y (j1,j2,j3)*s x (j1,j2,j3)
     & )*sj
                         a23(j1,j2,j3) = (s y (j1,j2,j3)*t x (j1,j2,j3)
     & )*sj
                         a33(j1,j2,j3) = (t y (j1,j2,j3)*t x (j1,j2,j3)
     & )*sj
! #If "y" == "x"
! #Else
                         a21(j1,j2,j3) = (s y (j1,j2,j3)*r x (j1,j2,j3)
     & )*sj
                         a31(j1,j2,j3) = (t y (j1,j2,j3)*r x (j1,j2,j3)
     & )*sj
                         a32(j1,j2,j3) = (t y (j1,j2,j3)*s x (j1,j2,j3)
     & )*sj
                       end do
                     end do
                   end do
                 else if( dir1.eq.1 .and. dir2.eq.1 )then
! DXSDY23(y,y,jac(j1,j2,j3))
                   do j3=m3a,m3b
                     do j2=m2a,m2b
                       do j1=m1a,m1b
                         sj = jac(j1,j2,j3)
                         a11(j1,j2,j3) = (r y (j1,j2,j3)*r y (j1,j2,j3)
     & )*sj
                         a12(j1,j2,j3) = (r y (j1,j2,j3)*s y (j1,j2,j3)
     & )*sj
                         a13(j1,j2,j3) = (r y (j1,j2,j3)*t y (j1,j2,j3)
     & )*sj
                         a22(j1,j2,j3) = (s y (j1,j2,j3)*s y (j1,j2,j3)
     & )*sj
                         a23(j1,j2,j3) = (s y (j1,j2,j3)*t y (j1,j2,j3)
     & )*sj
                         a33(j1,j2,j3) = (t y (j1,j2,j3)*t y (j1,j2,j3)
     & )*sj
! #If "y" == "y"
                         a21(j1,j2,j3) = a12(j1,j2,j3)
                         a31(j1,j2,j3) = a13(j1,j2,j3)
                         a32(j1,j2,j3) = a23(j1,j2,j3)
                       end do
                     end do
                   end do
                 else if( dir1.eq.1 .and. dir2.eq.2 )then
! DXSDY23(y,z,jac(j1,j2,j3))
                   do j3=m3a,m3b
                     do j2=m2a,m2b
                       do j1=m1a,m1b
                         sj = jac(j1,j2,j3)
                         a11(j1,j2,j3) = (r y (j1,j2,j3)*r z (j1,j2,j3)
     & )*sj
                         a12(j1,j2,j3) = (r y (j1,j2,j3)*s z (j1,j2,j3)
     & )*sj
                         a13(j1,j2,j3) = (r y (j1,j2,j3)*t z (j1,j2,j3)
     & )*sj
                         a22(j1,j2,j3) = (s y (j1,j2,j3)*s z (j1,j2,j3)
     & )*sj
                         a23(j1,j2,j3) = (s y (j1,j2,j3)*t z (j1,j2,j3)
     & )*sj
                         a33(j1,j2,j3) = (t y (j1,j2,j3)*t z (j1,j2,j3)
     & )*sj
! #If "y" == "z"
! #Else
                         a21(j1,j2,j3) = (s y (j1,j2,j3)*r z (j1,j2,j3)
     & )*sj
                         a31(j1,j2,j3) = (t y (j1,j2,j3)*r z (j1,j2,j3)
     & )*sj
                         a32(j1,j2,j3) = (t y (j1,j2,j3)*s z (j1,j2,j3)
     & )*sj
                       end do
                     end do
                   end do
                 else if( dir1.eq.2 .and. dir2.eq.0 )then
! DXSDY23(z,x,jac(j1,j2,j3))
                   do j3=m3a,m3b
                     do j2=m2a,m2b
                       do j1=m1a,m1b
                         sj = jac(j1,j2,j3)
                         a11(j1,j2,j3) = (r z (j1,j2,j3)*r x (j1,j2,j3)
     & )*sj
                         a12(j1,j2,j3) = (r z (j1,j2,j3)*s x (j1,j2,j3)
     & )*sj
                         a13(j1,j2,j3) = (r z (j1,j2,j3)*t x (j1,j2,j3)
     & )*sj
                         a22(j1,j2,j3) = (s z (j1,j2,j3)*s x (j1,j2,j3)
     & )*sj
                         a23(j1,j2,j3) = (s z (j1,j2,j3)*t x (j1,j2,j3)
     & )*sj
                         a33(j1,j2,j3) = (t z (j1,j2,j3)*t x (j1,j2,j3)
     & )*sj
! #If "z" == "x"
! #Else
                         a21(j1,j2,j3) = (s z (j1,j2,j3)*r x (j1,j2,j3)
     & )*sj
                         a31(j1,j2,j3) = (t z (j1,j2,j3)*r x (j1,j2,j3)
     & )*sj
                         a32(j1,j2,j3) = (t z (j1,j2,j3)*s x (j1,j2,j3)
     & )*sj
                       end do
                     end do
                   end do
                 else if( dir1.eq.2 .and. dir2.eq.1 )then
! DXSDY23(z,y,jac(j1,j2,j3))
                   do j3=m3a,m3b
                     do j2=m2a,m2b
                       do j1=m1a,m1b
                         sj = jac(j1,j2,j3)
                         a11(j1,j2,j3) = (r z (j1,j2,j3)*r y (j1,j2,j3)
     & )*sj
                         a12(j1,j2,j3) = (r z (j1,j2,j3)*s y (j1,j2,j3)
     & )*sj
                         a13(j1,j2,j3) = (r z (j1,j2,j3)*t y (j1,j2,j3)
     & )*sj
                         a22(j1,j2,j3) = (s z (j1,j2,j3)*s y (j1,j2,j3)
     & )*sj
                         a23(j1,j2,j3) = (s z (j1,j2,j3)*t y (j1,j2,j3)
     & )*sj
                         a33(j1,j2,j3) = (t z (j1,j2,j3)*t y (j1,j2,j3)
     & )*sj
! #If "z" == "y"
! #Else
                         a21(j1,j2,j3) = (s z (j1,j2,j3)*r y (j1,j2,j3)
     & )*sj
                         a31(j1,j2,j3) = (t z (j1,j2,j3)*r y (j1,j2,j3)
     & )*sj
                         a32(j1,j2,j3) = (t z (j1,j2,j3)*s y (j1,j2,j3)
     & )*sj
                       end do
                     end do
                   end do
                 else if( dir1.eq.2 .and. dir2.eq.2 )then
! DXSDY23(z,z,jac(j1,j2,j3))
                   do j3=m3a,m3b
                     do j2=m2a,m2b
                       do j1=m1a,m1b
                         sj = jac(j1,j2,j3)
                         a11(j1,j2,j3) = (r z (j1,j2,j3)*r z (j1,j2,j3)
     & )*sj
                         a12(j1,j2,j3) = (r z (j1,j2,j3)*s z (j1,j2,j3)
     & )*sj
                         a13(j1,j2,j3) = (r z (j1,j2,j3)*t z (j1,j2,j3)
     & )*sj
                         a22(j1,j2,j3) = (s z (j1,j2,j3)*s z (j1,j2,j3)
     & )*sj
                         a23(j1,j2,j3) = (s z (j1,j2,j3)*t z (j1,j2,j3)
     & )*sj
                         a33(j1,j2,j3) = (t z (j1,j2,j3)*t z (j1,j2,j3)
     & )*sj
! #If "z" == "z"
                         a21(j1,j2,j3) = a12(j1,j2,j3)
                         a31(j1,j2,j3) = a13(j1,j2,j3)
                         a32(j1,j2,j3) = a23(j1,j2,j3)
                       end do
                     end do
                   end do
                 else
                   write(*,*) 'ERROR invalid values: dir1=',dir1,' 
     & dir2=',dir2
                 end if
               end if
               if( derivType.eq.symmetric )then
               ! symmetric case -- do not average a12, a21, a13, z31, etc.  -- could do better
                m1a=n1a
                do j3=m3a,m3b
                 do j2=m2a,m2b
                   do j1=m1b,m1a,-1 ! go backwards   worry about division by zero
                    sh=s(j1,j2,j3,0)*s(j1-1,j2,j3,0)/(s(j1,j2,j3,0)+s(
     & j1-1,j2,j3,0))
                     a11(j1,j2,j3) = sh *d22(1)*(a11(j1,j2,j3)+a11(j1-
     & 1,j2,j3))
                   end do
                 end do
                end do
                m1a=n1a-1
                do j3=m3a,m3b
                 do j2=m2a,m2b
                   do j1=m1b,m1a,-1
! #If "" eq "c"
! #Else
                     a12(j1,j2,j3) = s(j1,j2,j3,0)*(d12(1)*d12(2))*a12(
     & j1,j2,j3)
                     a13(j1,j2,j3) = s(j1,j2,j3,0)*(d12(1)*d12(3))*a13(
     & j1,j2,j3)
                   end do
                 end do
                end do
                m2a=n2a
                do j3=m3a,m3b
                 do j2=m2b,m2a,-1
                   do j1=m1a,m1b
                    sh=s(j1,j2,j3,0)*s(j1,j2-1,j3,0)/(s(j1,j2,j3,0)+s(
     & j1,j2-1,j3,0))
                     a22(j1,j2,j3) = sh *d22(2)*(a22(j1,j2,j3)+a22(j1,
     & j2-1,j3))
                   end do
                 end do
                end do
                m2a=n2a-1
                do j3=m3a,m3b
                 do j2=m2b,m2a,-1
                   do j1=m1a,m1b
! #If "" eq "c"
! #Else
                     a21(j1,j2,j3) = s(j1,j2,j3,0)*(d12(1)*d12(2))*a21(
     & j1,j2,j3)
                     a23(j1,j2,j3) = s(j1,j2,j3,0)*(d12(2)*d12(3))*a23(
     & j1,j2,j3)
                   end do
                 end do
                end do
                m3a=n3a
                do j3=m3b,m3a,-1
                 do j2=m2a,m2b
                   do j1=m1a,m1b
                    sh=s(j1,j2,j3,0)*s(j1,j2,j3-1,0)/(s(j1,j2,j3,0)+s(
     & j1,j2,j3-1,0))
                     a33(j1,j2,j3) = sh *d22(3)*(a33(j1,j2,j3)+a33(j1,
     & j2,j3-1))
                   end do
                 end do
                end do
                m3a=n3a-1
                do j3=m3b,m3a,-1
                 do j2=m2a,m2b
                   do j1=m1a,m1b
! #If "" eq "c"
! #Else
                     a31(j1,j2,j3) = s(j1,j2,j3,0)*(d12(1)*d12(3))*a31(
     & j1,j2,j3)
                     a32(j1,j2,j3) = s(j1,j2,j3,0)*(d12(2)*d12(3))*a32(
     & j1,j2,j3)
                   end do
                 end do
                end do
                  else
                ! ** old way -- average all coefficients
               m1a=n1a
               do j3=m3a,m3b
                 do j2=m2a,m2b
                   do j1=m1b,m1a,-1 ! go backwards  worry about division by zero
                    sh=s(j1,j2,j3,0)*s(j1-1,j2,j3,0)/(s(j1,j2,j3,0)+s(
     & j1-1,j2,j3,0))
                     a11(j1,j2,j3) = sh *d22(1)*(a11(j1,j2,j3)+a11(j1-
     & 1,j2,j3))
                     a12(j1,j2,j3) = sh *(d12(1)*d12(2))*(a12(j1,j2,j3)
     & +a12(j1-1,j2,j3))
                     a13(j1,j2,j3) = sh *(d12(1)*d12(3))*(a13(j1,j2,j3)
     & +a13(j1-1,j2,j3))
                   end do
                 end do
               end do
               m1a=n1a-1
               m2a=n2a
               do j3=m3a,m3b
                 do j2=m2b,m2a,-1
                   do j1=m1a,m1b
                    sh=s(j1,j2,j3,0)*s(j1,j2-1,j3,0)/(s(j1,j2,j3,0)+s(
     & j1,j2-1,j3,0))
                     a21(j1,j2,j3) = sh *(d12(1)*d12(2))*(a21(j1,j2,j3)
     & +a21(j1,j2-1,j3))
                     a22(j1,j2,j3) = sh *d22(2)*(a22(j1,j2,j3)+a22(j1,
     & j2-1,j3))
                     a23(j1,j2,j3) = sh *(d12(2)*d12(3))*(a23(j1,j2,j3)
     & +a23(j1,j2-1,j3))
                   end do
                 end do
               end do
               m2a=n2a-1
               m3a=n3a
               do j3=m3b,m3a,-1
                 do j2=m2a,m2b
                   do j1=m1a,m1b
                    sh=s(j1,j2,j3,0)*s(j1,j2,j3-1,0)/(s(j1,j2,j3,0)+s(
     & j1,j2,j3-1,0))
                     a31(j1,j2,j3) = sh *(d12(1)*d12(3))*(a31(j1,j2,j3)
     & +a31(j1,j2,j3-1))
                     a32(j1,j2,j3) = sh *(d12(2)*d12(3))*(a32(j1,j2,j3)
     & +a32(j1,j2,j3-1))
                     a33(j1,j2,j3) = sh *d22(3)*(a33(j1,j2,j3)+a33(j1,
     & j2,j3-1))
                   end do
                 end do
               end do
               m3a=n3a-1
               end if
             end if
       !       This was generated by dd.m
! beginLoops()
             ! ***** loop over equations and components *****
             do e=ea,eb
             do c=ca,cb
             ec=ns*(c+nc*e)
             ! ** it did not affect performance to use an array to index coeff ***
             if( nd.eq.2 )then
             do i2=-1,1
               do i1=-1,1
                m(i1,i2)=i1+1+3*(i2+1) +1 + ec
               end do
             end do
             else if( nd.eq.3 )then
             do i3=-1,1
               do i2=-1,1
                 do i1=-1,1
                   m3(i1,i2,i3)=i1+1+3*(i2+1+3*(i3+1)) +1 + ec
                 end do
               end do
             end do
             else
             m12=1 + ec
             m22=2 + ec
             m32=3 + ec
             endif
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
! loopBody2ndOrder3d(0,(a23(i1,i2,i3)+a32(i1,i2,i3))/jac(i1,i2,i3),0,(a13(i1,i2,i3)+a31(i1,i2,i3))/jac(i1,i2,i3), -(a13(i1+1,i2,i3)-a33(i1,i2,i3)-a13(i1,i2,i3)-a23(i1,i2,i3)+a23(i1,i2+1,i3))/jac(i1,i2,i3), -(a13(i1+1,i2,i3)+a31(i1,i2,i3))/jac(i1,i2,i3),0, -(a23(i1,i2+1,i3)+a32(i1,i2,i3))/jac(i1,i2,i3),0,(a12(i1,i2,i3)+a21(i1,i2,i3))/jac(i1,i2,i3), (-a32(i1,i2,i3+1)-a12(i1+1,i2,i3)+a32(i1,i2,i3)+a22(i1,i2,i3)+a12(i1,i2,i3))/jac(i1,i2,i3), -(a21(i1,i2,i3)+a12(i1+1,i2,i3))/jac(i1,i2,i3), (-a31(i1,i2,i3+1)+a11(i1,i2,i3)+a31(i1,i2,i3)-a21(i1,i2+1,i3)+a21(i1,i2,i3))/jac(i1,i2,i3), -(a11(i1+1,i2,i3)+a11(i1,i2,i3)+a22(i1,i2+1,i3)+a22(i1,i2,i3)+a33(i1,i2,i3+1)+a33(i1,i2,i3))/jac(i1,i2,i3), -(-a11(i1+1,i2,i3)+a21(i1,i2,i3)+a31(i1,i2,i3)-a21(i1,i2+1,i3)-a31(i1,i2,i3+1))/jac(i1,i2,i3), -(a21(i1,i2+1,i3)+a12(i1,i2,i3))/jac(i1,i2,i3), -(-a32(i1,i2,i3+1)-a12(i1+1,i2,i3)-a22(i1,i2+1,i3)+a12(i1,i2,i3)+a32(i1,i2,i3))/jac(i1,i2,i3), (a21(i1,i2+1,i3)+a12(i1+1,i2,i3))/jac(i1,i2,i3),0, -(a32(i1,i2,i3+1)+a23(i1,i2,i3))/jac(i1,i2,i3),0, -(a13(i1,i2,i3)+a31(i1,i2,i3+1))/jac(i1,i2,i3), -(-a13(i1+1,i2,i3)+a13(i1,i2,i3)-a33(i1,i2,i3+1)-a23(i1,i2+1,i3)+a23(i1,i2,i3))/jac(i1,i2,i3), (a13(i1+1,i2,i3)+a31(i1,i2,i3+1))/jac(i1,i2,i3),0, (a23(i1,i2+1,i3)+a32(i1,i2,i3+1))/jac(i1,i2,i3),0 )
                coeff(m3(-1,-1,-1),i1,i2,i3)=0
                coeff(m3( 0,-1,-1),i1,i2,i3)=(a23(i1,i2,i3)+a32(i1,i2,
     & i3))/jac(i1,i2,i3)
                coeff(m3(+1,-1,-1),i1,i2,i3)=0
                coeff(m3(-1, 0,-1),i1,i2,i3)=(a13(i1,i2,i3)+a31(i1,i2,
     & i3))/jac(i1,i2,i3)
                coeff(m3( 0, 0,-1),i1,i2,i3)=-(a13(i1+1,i2,i3)-a33(i1,
     & i2,i3)-a13(i1,i2,i3)-a23(i1,i2,i3)+a23(i1,i2+1,i3))/jac(i1,i2,
     & i3)
                coeff(m3(+1, 0,-1),i1,i2,i3)=-(a13(i1+1,i2,i3)+a31(i1,
     & i2,i3))/jac(i1,i2,i3)
                coeff(m3(-1,+1,-1),i1,i2,i3)=0
                coeff(m3( 0,+1,-1),i1,i2,i3)=-(a23(i1,i2+1,i3)+a32(i1,
     & i2,i3))/jac(i1,i2,i3)
                coeff(m3(+1,+1,-1),i1,i2,i3)=0
                coeff(m3(-1,-1, 0),i1,i2,i3)=(a12(i1,i2,i3)+a21(i1,i2,
     & i3))/jac(i1,i2,i3)
                coeff(m3( 0,-1, 0),i1,i2,i3)=(-a32(i1,i2,i3+1)-a12(i1+
     & 1,i2,i3)+a32(i1,i2,i3)+a22(i1,i2,i3)+a12(i1,i2,i3))/jac(i1,i2,
     & i3)
                coeff(m3(+1,-1, 0),i1,i2,i3)=-(a21(i1,i2,i3)+a12(i1+1,
     & i2,i3))/jac(i1,i2,i3)
                coeff(m3(-1, 0, 0),i1,i2,i3)=(-a31(i1,i2,i3+1)+a11(i1,
     & i2,i3)+a31(i1,i2,i3)-a21(i1,i2+1,i3)+a21(i1,i2,i3))/jac(i1,i2,
     & i3)
                coeff(m3( 0, 0, 0),i1,i2,i3)=-(a11(i1+1,i2,i3)+a11(i1,
     & i2,i3)+a22(i1,i2+1,i3)+a22(i1,i2,i3)+a33(i1,i2,i3+1)+a33(i1,i2,
     & i3))/jac(i1,i2,i3)
                coeff(m3(+1, 0, 0),i1,i2,i3)=-(-a11(i1+1,i2,i3)+a21(i1,
     & i2,i3)+a31(i1,i2,i3)-a21(i1,i2+1,i3)-a31(i1,i2,i3+1))/jac(i1,
     & i2,i3)
                coeff(m3(-1,+1, 0),i1,i2,i3)=-(a21(i1,i2+1,i3)+a12(i1,
     & i2,i3))/jac(i1,i2,i3)
                coeff(m3( 0,+1, 0),i1,i2,i3)=-(-a32(i1,i2,i3+1)-a12(i1+
     & 1,i2,i3)-a22(i1,i2+1,i3)+a12(i1,i2,i3)+a32(i1,i2,i3))/jac(i1,
     & i2,i3)
                coeff(m3(+1,+1, 0),i1,i2,i3)=(a21(i1,i2+1,i3)+a12(i1+1,
     & i2,i3))/jac(i1,i2,i3)
                coeff(m3(-1,-1,+1),i1,i2,i3)=0
                coeff(m3( 0,-1,+1),i1,i2,i3)=-(a32(i1,i2,i3+1)+a23(i1,
     & i2,i3))/jac(i1,i2,i3)
                coeff(m3(+1,-1,+1),i1,i2,i3)=0
                coeff(m3(-1, 0,+1),i1,i2,i3)=-(a13(i1,i2,i3)+a31(i1,i2,
     & i3+1))/jac(i1,i2,i3)
                coeff(m3( 0, 0,+1),i1,i2,i3)=-(-a13(i1+1,i2,i3)+a13(i1,
     & i2,i3)-a33(i1,i2,i3+1)-a23(i1,i2+1,i3)+a23(i1,i2,i3))/jac(i1,
     & i2,i3)
                coeff(m3(+1, 0,+1),i1,i2,i3)=(a13(i1+1,i2,i3)+a31(i1,
     & i2,i3+1))/jac(i1,i2,i3)
                coeff(m3(-1,+1,+1),i1,i2,i3)=0
                coeff(m3( 0,+1,+1),i1,i2,i3)=(a23(i1,i2+1,i3)+a32(i1,
     & i2,i3+1))/jac(i1,i2,i3)
                coeff(m3(+1,+1,+1),i1,i2,i3)=0
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
!   #If "divScalarGrad" == "identity"
!   #Elif "divScalarGrad" == "rr"
!   #Elif "divScalarGrad" == "r"
         if( gridType .eq. 0 )then
       !   rectangular
!     #If "divScalarGrad" == "divScalarGrad"
! defineA21R()
             m1a=n1a-1
             m1b=n1b+1
             m2a=n2a
             m2b=n2b
             m3a=n3a
             m3b=n3b
c **** both divScalarGrad and derivativeScalarDerivative are the same in 1D *****
             if( averagingType .eq. arithmeticAverage )then
               factor=.5
! loopsDSG1(a11(j1,j2,j3) = factor*h22(1)*(s(j1,j2,j3,0)+s(j1-1,j2,j3,0)))
               m1a=n1a
               do j3=m3a,m3b
                 do j2=m2a,m2b
                   do j1=m1a,m1b
                     a11(j1,j2,j3)=factor*h22(1)*(s(j1,j2,j3,0)+s(j1-1,
     & j2,j3,0))
                   end do
                 end do
               end do
               m1a=n1a-1
             else
c  Harmonic average
               factor=2.
               ! should be worry about division by zero?
! loopsDSG1(a11(j1,j2,j3)=s(j1,j2,j3,0)*h22(1)*s(j1-1,j2,j3,0)/(s(j1,j2,j3,0)+s(j1-1,j2,j3,0)))
               m1a=n1a
               do j3=m3a,m3b
                 do j2=m2a,m2b
                   do j1=m1a,m1b
                     a11(j1,j2,j3)=s(j1,j2,j3,0)*h22(1)*s(j1-1,j2,j3,0)
     & /(s(j1,j2,j3,0)+s(j1-1,j2,j3,0))
                   end do
                 end do
               end do
               m1a=n1a-1
             end if
! beginLoops()
               ! ***** loop over equations and components *****
               do e=ea,eb
               do c=ca,cb
               ec=ns*(c+nc*e)
               ! ** it did not affect performance to use an array to index coeff ***
               if( nd.eq.2 )then
               do i2=-1,1
                 do i1=-1,1
                  m(i1,i2)=i1+1+3*(i2+1) +1 + ec
                 end do
               end do
               else if( nd.eq.3 )then
               do i3=-1,1
                 do i2=-1,1
                   do i1=-1,1
                     m3(i1,i2,i3)=i1+1+3*(i2+1+3*(i3+1)) +1 + ec
                   end do
                 end do
               end do
               else
               m12=1 + ec
               m22=2 + ec
               m32=3 + ec
               endif
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
! loopBody2ndOrder1d(a11(i1,i2,i3),-(a11(i1+1,i2,i3)+a11(i1,i2,i3)),a11(i1+1,i2,i3))
                coeff(m12,i1,i2,i3)=a11(i1,i2,i3)
                coeff(m22,i1,i2,i3)=-(a11(i1+1,i2,i3)+a11(i1,i2,i3))
                coeff(m32,i1,i2,i3)=a11(i1+1,i2,i3)
! endLoops()
               end do
               end do
               end do
               end do
               end do
         else
       !  ***** not rectangular *****
!     #If "divScalarGrad" == "divScalarGrad"
! defineA21()
             m1a=n1a-1
             m1b=n1b+1
             m2a=n2a
             m2b=n2b
             m3a=n3a
             m3b=n3b
             if( averagingType .eq. arithmeticAverage )then
               factor=.5
! GETA21(s(j1,j2,j3,0)*jac(j1,j2,j3),c,factor)
               if( derivOption.eq.laplace )then
                 do j3=m3a,m3b
                   do j2=m2a,m2b
                     do j1=m1a,m1b
                       sj = jac(j1,j2,j3)
                       a11(j1,j2,j3) = (rx(j1,j2,j3)**2)*sj
                     end do
                   end do
                 end do
               else if( derivOption.eq.divScalarGrad .or. derivOption 
     & .eq. divTensorGrad )then
                 do j3=m3a,m3b
                   do j2=m2a,m2b
                     do j1=m1a,m1b
                       sj = s(j1,j2,j3,0)*jac(j1,j2,j3)
                       a11(j1,j2,j3) = (rx(j1,j2,j3)**2)*sj
                     end do
                   end do
                 end do
               else if( derivOption.eq.derivativeScalarDerivative )then
                 if(      dir1.eq.0 .and. dir2.eq.0 )then
! DXSDY21(x,x,s(j1,j2,j3,0)*jac(j1,j2,j3))
                   do j3=m3a,m3b
                     do j2=m2a,m2b
                       do j1=m1a,m1b
                         sj = s(j1,j2,j3,0)*jac(j1,j2,j3)
                         a11(j1,j2,j3) = (r x (j1,j2,j3)*r x (j1,j2,j3)
     & )*sj
                       end do
                     end do
                   end do
                 else
                   write(*,*) 'ERROR invalid values: dir1=',dir1,' 
     & dir2=',dir2
                 end if
               end if
               m1a=n1a
               do j3=m3a,m3b
                 do j2=m2a,m2b
                   do j1=m1b,m1a,-1 ! go backwards ** worry about division by zero
c     sh=s(j1,j2,j3,0)*s(j1-1,j2,j3,0)/(s(j1,j2,j3,0)+s(j1-1,j2,j3,0))
                     a11(j1,j2,j3) = factor *d22(1)*(a11(j1,j2,j3)+a11(
     & j1-1,j2,j3))
                   end do
                 end do
               end do
               m1a=n1a-1
             else
c       Harmonic average
             factor=2.
c       do not average in s:  
! GETA21(jac(j1,j2,j3), ,sh)
             if( derivOption.eq.laplace )then
               do j3=m3a,m3b
                 do j2=m2a,m2b
                   do j1=m1a,m1b
                     sj = jac(j1,j2,j3)
                     a11(j1,j2,j3) = (rx(j1,j2,j3)**2)*sj
                   end do
                 end do
               end do
             else if( derivOption.eq.divScalarGrad .or. derivOption 
     & .eq. divTensorGrad )then
               do j3=m3a,m3b
                 do j2=m2a,m2b
                   do j1=m1a,m1b
                     sj = jac(j1,j2,j3)
                     a11(j1,j2,j3) = (rx(j1,j2,j3)**2)*sj
                   end do
                 end do
               end do
             else if( derivOption.eq.derivativeScalarDerivative )then
               if(      dir1.eq.0 .and. dir2.eq.0 )then
! DXSDY21(x,x,jac(j1,j2,j3))
                 do j3=m3a,m3b
                   do j2=m2a,m2b
                     do j1=m1a,m1b
                       sj = jac(j1,j2,j3)
                       a11(j1,j2,j3) = (r x (j1,j2,j3)*r x (j1,j2,j3))*
     & sj
                     end do
                   end do
                 end do
               else
                 write(*,*) 'ERROR invalid values: dir1=',dir1,' 
     & dir2=',dir2
               end if
             end if
             m1a=n1a
             do j3=m3a,m3b
               do j2=m2a,m2b
                 do j1=m1b,m1a,-1 ! go backwards ** worry about division by zero
                  sh=s(j1,j2,j3,0)*s(j1-1,j2,j3,0)/(s(j1,j2,j3,0)+s(j1-
     & 1,j2,j3,0))
                   a11(j1,j2,j3) = sh *d22(1)*(a11(j1,j2,j3)+a11(j1-1,
     & j2,j3))
                 end do
               end do
             end do
             m1a=n1a-1
             end if
       !      This was generated by dd.m
! beginLoops()
             ! ***** loop over equations and components *****
             do e=ea,eb
             do c=ca,cb
             ec=ns*(c+nc*e)
             ! ** it did not affect performance to use an array to index coeff ***
             if( nd.eq.2 )then
             do i2=-1,1
               do i1=-1,1
                m(i1,i2)=i1+1+3*(i2+1) +1 + ec
               end do
             end do
             else if( nd.eq.3 )then
             do i3=-1,1
               do i2=-1,1
                 do i1=-1,1
                   m3(i1,i2,i3)=i1+1+3*(i2+1+3*(i3+1)) +1 + ec
                 end do
               end do
             end do
             else
             m12=1 + ec
             m22=2 + ec
             m32=3 + ec
             endif
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
! loopBody2ndOrder1d((a11(i1,i2,i3))/jac(i1,i2,i3), -(a11(i1+1,i2,i3)+a11(i1,i2,i3))/jac(i1,i2,i3), (a11(i1+1,i2,i3))/jac(i1,i2,i3) )
               coeff(m12,i1,i2,i3)=(a11(i1,i2,i3))/jac(i1,i2,i3)
               coeff(m22,i1,i2,i3)=-(a11(i1+1,i2,i3)+a11(i1,i2,i3))
     & /jac(i1,i2,i3)
               coeff(m32,i1,i2,i3)=(a11(i1+1,i2,i3))/jac(i1,i2,i3)
! endLoops()
              end do
              end do
              end do
              end do
              end do
         end if
         else if( nd.eq.0 )then
       !       *** add these lines to avoid warnings about unused statement functions
           include "cgux2afNoWarnings.h"
           temp=rxx1(i1,i2,i3)
         end if
       return
       end
