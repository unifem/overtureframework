! This file automatically generated from ellipticSmooth.bf with bpp.


      subroutine ellipticSmooth( md,nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &    n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c, omega, u, source,
     &    normal, ipar,rpar )
c ======================================================================================
c   Smooth the elliptic grid generation equations
c
c  md,nd : domain and range dimensions
c  normal : for surface grids this is the normal to the surface.
c
c ipar(0:*) : integer parameters
c             option = ipar(0)
c rpar(0:*) : real parameters
c
c  option: 0=smooth, 1=compute control functions to match current solution
c
c  Note: ghost point values are needed for u and normal
c ======================================================================================
      ! implicit none
      implicit real (a-h,o-z)
      integer md,nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &        n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c,option
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      real source(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      real normal(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      real omega(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)

      integer ipar(0:*)
      real rpar(0:*)

c --- local variables

      integer i1,i2,i3,j1,j2,j3,is(0:2)
      integer m1a,m1b,m2a,m2b,m3a,m3b
      integer m,kd,side,axis
      real coeff0,coeff1,coeff2,coeff3,coeff4,coeff5
      real diag

      real d12(3),d22(3)
      real dr(3)
      integer bc(0:1,0:2)

      real xr,xs,xt,yr,ys,yt,zr,zs,zt

      real xrNorm,xsNorm,tau1,tau2,an1,an2,eps,coeffn,coefft,duDotN,
     & bcEqn2d0,bcEqn2d1
      real tauDotU,nDotu,alpha

      integer side2,is1,is2,axisp1
      real f1,f2

      ! boundary condition values
      integer periodic,pointsFixed,pointsSlide,boundaryIsSmoothed
      parameter( periodic=-1, pointsFixed=0, pointsSlide=1, 
     & boundaryIsSmoothed=2 )

c ---------begin statement functions-------------
      ! define ur2,us2,ut2,urr2,...
      real h21(3), h22(3)

      real rx,ry,rz,sx,sy,sz,tx,ty,tz
      real urr2,uss2,utt2,urs2,urt2,ust2,ur2,us2,ut2
      real rxx2
      real rxx23,sxx23,txx23
      real rxr2,rxs2,rxt2,sxr2,sxs2,sxt2,txr2,txs2,txt2
      real ryr2,rys2,ryt2,syr2,sys2,syt2,tyr2,tys2,tyt2
      real rzr2,rzs2,rzt2,szr2,szs2,szt2,tzr2,tzs2,tzt2

      real UXX21R, UXX21
      real UXX22R, UXX22
      real UXX23R, UXX23
      real eqn2d,eqn3d,eqn23d,ceqn2d,ceqn3d,ceqn23d
      real a11,a12,a13,a21,a22,a23,a31,a32,a33,det,r1,r2,r3,s1,s2
      real b11,b12,b21,b22

      ! bogus definitions for rx,ry,...
      rx(i1,i2,i3)=u(i1,i2,i3,0)
      ry(i1,i2,i3)=u(i1,i2,i3,0)
      rz(i1,i2,i3)=u(i1,i2,i3,0)
      sx(i1,i2,i3)=u(i1,i2,i3,0)
      sy(i1,i2,i3)=u(i1,i2,i3,0)
      sz(i1,i2,i3)=u(i1,i2,i3,0)
      tx(i1,i2,i3)=u(i1,i2,i3,0)
      ty(i1,i2,i3)=u(i1,i2,i3,0)
      tz(i1,i2,i3)=u(i1,i2,i3,0)

      include 'cgux2af.h'


      eqn2d(i1,i2,i3,m)=
     & coeff0*( urr2(i1,i2,i3,m)+source(i1,i2,i3,0)*ur2(i1,i2,i3,m) )+
     & coeff1*( uss2(i1,i2,i3,m)+source(i1,i2,i3,1)*us2(i1,i2,i3,m) )+
     & coeff2*( urs2(i1,i2,i3,m) )

      eqn3d(i1,i2,i3,m)=
     & coeff0*( urr2(i1,i2,i3,m)+source(i1,i2,i3,0)*ur2(i1,i2,i3,m) )+
     & coeff1*( uss2(i1,i2,i3,m)+source(i1,i2,i3,1)*us2(i1,i2,i3,m) )+
     & coeff2*( utt2(i1,i2,i3,m)+source(i1,i2,i3,2)*ut2(i1,i2,i3,m) )+
     & coeff3*urs2(i1,i2,i3,m) +
     & coeff4*urt2(i1,i2,i3,m) +
     & coeff5*ust2(i1,i2,i3,m)

!    for surface grids
      eqn23d(i1,i2,i3,m)=
     & coeff0*( urr2(i1,i2,i3,m)+source(i1,i2,i3,0)*ur2(i1,i2,i3,m) )+
     & coeff1*( uss2(i1,i2,i3,m)+source(i1,i2,i3,1)*us2(i1,i2,i3,m) )+
     & coeff3*urs2(i1,i2,i3,m) +
     & coeff4*(normal(i1+1,i2,i3,m)-normal(i1-1,i2,i3,m))*d12(1)+    ! urt2(i1,i2,i3,m)
     & coeff5*(normal(i1,i2+1,i3,m)-normal(i1,i2-1,i3,m))*d12(2)     ! ust2(i1,i2,i3,m)

!  For the control function computation

      ceqn2d(i1,i2,i3,m)=
     & coeff0*( urr2(i1,i2,i3,m) )+
     & coeff1*( uss2(i1,i2,i3,m) )+
     & coeff2*( urs2(i1,i2,i3,m) )

      ceqn3d(i1,i2,i3,m)=
     & coeff0*( urr2(i1,i2,i3,m) )+
     & coeff1*( uss2(i1,i2,i3,m) )+
     & coeff2*( utt2(i1,i2,i3,m) )+
     & coeff3*urs2(i1,i2,i3,m) +
     & coeff4*urt2(i1,i2,i3,m) +
     & coeff5*ust2(i1,i2,i3,m)

!    for surface grids
      ceqn23d(i1,i2,i3,m)=
     & coeff0*( urr2(i1,i2,i3,m) )+
     & coeff1*( uss2(i1,i2,i3,m) )+
     & coeff3*urs2(i1,i2,i3,m) +
     & coeff4*(normal(i1+1,i2,i3,m)-normal(i1-1,i2,i3,m))*d12(1)+    ! urt2(i1,i2,i3,m)
     & coeff5*(normal(i1,i2+1,i3,m)-normal(i1,i2-1,i3,m))*d12(2)     ! ust2(i1,i2,i3,m)

! BC equations for the tangential component -- a combination of orthogonality and smoothing 
      ! orthogonality says tau.(u.r)=0 
      !     tau.[  (u(i1+1)-u(i1-1))/(2*dr) -alpha*( D0r(u(i2+1))-2.*D0r(u(i2))+D02(u(i2-1)) ) = 0
      bcEqn2d0(i1,i2,i3,m) = ur2(i1,i2,i3,m) -alpha*( uss2(i1+1,i2,i3,
     & m)-uss2(i1-1,i2,i3,m) )*dr(2)**2/(2.*dr(1))
      bcEqn2d1(i1,i2,i3,m) = us2(i1,i2,i3,m) -alpha*( urr2(i1,i2+1,i3,
     & m)-urr2(i1,i2-1,i3,m) )*dr(1)**2/(2.*dr(2))


c -----------end statement functions------------------

      option = ipar(0)
      bc(0,0)= ipar(1)
      bc(1,0)= ipar(2)
      bc(0,1)= ipar(3)
      bc(1,1)= ipar(4)
      bc(0,2)= ipar(5)
      bc(1,2)= ipar(6)

      ! use base 1 for dr for now -- 
      dr(1) = rpar(0)
      dr(2) = rpar(1)
      dr(3) = rpar(2)
      eps   = rpar(3)
      alpha = rpar(4) ! smoothing coefficient in BC for tangential component

      alpha=0.

      do axis=1,3
        d12(axis)=1./(2.*dr(axis))
        d22(axis)=1./dr(axis)**2
      end do

      if( option.eq.0 )then
        ! ******************************************
        ! ********* Elliptic Smooth ****************
        ! ******************************************
      if( nd.eq.2 )then

        m1a=n1a
        m1b=n1b
        m2a=n2a
        m2b=n2b
        m3a=n3a
        m3b=n3b
        ! do not smooth points on sliding boundaries here -- this is done below
        if( bc(0,0).eq.pointsSlide )then
          m1a=m1a+1
        end if
        if( bc(1,0).eq.pointsSlide )then
          m1b=m1b-1
        end if
        if( bc(0,1).eq.pointsSlide )then
          m2a=m2a+1
        end if
        if( bc(1,1).eq.pointsSlide )then
          m2b=m2b-1
        end if


        ! ------ interior points ----------
        do i3=m3a,m3b
        do i2=m2a,m2b
        do i1=m1a,m1b
          xr=ur2(i1,i2,i3,0)
          xs=us2(i1,i2,i3,0)
          yr=ur2(i1,i2,i3,1)
          ys=us2(i1,i2,i3,1)

          coeff0=xs*xs+ys*ys
          coeff1=xr*xr+yr*yr
          coeff2=-2.*(xr*xs+yr*ys)

          diag=(.5*omega(i1,i2,i3))/(coeff0*d22(1)+coeff1*d22(2))

          u(i1,i2,i3,0)=u(i1,i2,i3,0)+eqn2d(i1,i2,i3,0)*diag
          u(i1,i2,i3,1)=u(i1,i2,i3,1)+eqn2d(i1,i2,i3,1)*diag

        end do
        end do
        end do


        ! ---- BC's in 2D --------
        do side=0,1
        do axis=0,1
          axisp1 = mod(axis+1,2)
          if( bc(side,axis).eq.pointsSlide )then

            is1=1-2*side
            is(0)=0
            is(1)=0
            is(axis)=is1

! getBoundaryIndexBoundsMacro(side,axis)
             m1a=n1a
             m1b=n1b
             m2a=n2a
             m2b=n2b
             m3a=n3a
             m3b=n3b
             if( side.eq.0 .and. axis.eq.0 )then
               m1b=m1a
             else if( side.eq.1 .and. axis.eq.0 )then
               m1a=m1b
             else if( side.eq.0 .and. axis.eq.1 )then
               m2b=m2a
             else if( side.eq.1 .and. axis.eq.1 )then
               m2a=m2b
             else if( side.eq.0 .and. axis.eq.2 )then
               m3b=m3a
             else if( side.eq.1 .and. axis.eq.2 )then
               m3a=m3b
             end if

            ! Do not adjust extended boundary points -- unless periodic
            if( .false. .and. bc(0,axisp1).ne.periodic )then
              if( axis.eq.0 )then
                m2a=m2a+1
                m2b=m2b-1
              else
                m1a=m1a+1
                m1b=m1b-1
              end if
            end if

            do i3=m3a,m3b ! loop over boundary points
            do i2=m2a,m2b
            do i1=m1a,m1b

              xr=ur2(i1,i2,i3,0)
              xs=us2(i1,i2,i3,0)
              yr=ur2(i1,i2,i3,1)
              ys=us2(i1,i2,i3,1)

              coeff0=xs*xs+ys*ys
              coeff1=xr*xr+yr*yr
              coeff2=-2.*(xr*xs+yr*ys)

              diag=(.5*omega(i1,i2,i3))/(coeff0*d22(1)+coeff1*d22(2))

              ! compute the unit tangent vector
              if( axis.eq.0 )then
                xsNorm=max(eps,sqrt(xs**2+ys**2))
                tau1 = xs/xsNorm ! tangent
                tau2 = ys/xsNorm
                coeffn = coeff0/dr(1)**2  ! coefficient of ghost point in eqn for normal component
              else
                xrNorm=max(eps,sqrt(xr**2+yr**2))
                tau1 = xr/xrNorm ! tangent
                tau2 = yr/xrNorm
                coeffn = coeff1/dr(2)**2  ! coefficient of ghost point in eqn for normal component
              end if
              an1 = -tau2 ! unit normal vector
              an2 =  tau1

              ! adjust boundary points -- these should only move in the tangential direction
              ! therefore we subtract off the component in the normal direction 

              du0 = eqn2d(i1,i2,i3,0)*diag   ! change in boundary value before adjusting
              du1 = eqn2d(i1,i2,i3,1)*diag

              duDotN=du0*an1+du1*an2
              du0 = du0 - duDotN*an1     ! subtract off the normal component of the correction
              du1 = du1 - duDotN*an2

              u(i1,i2,i3,0)=u(i1,i2,i3,0)+du0   ! here are the new values on the boundary
              u(i1,i2,i3,1)=u(i1,i2,i3,1)+du1

              ! assign ghost points: 
              !    (1) normal component comes from interior equation, n.(Lu)=0
              !    (2) tangential component has a BC: 
              !            tau.[ u_r - alpha* D+s D-s( u_r ) ] = 0  .... for BC at r=const.

              j1=i1-is(0) ! ghost point is (j1,j2,j3)
              j2=i2-is(1)
              j3=i3

              du0 =  u(j1,j2,j3,0)-eqn2d(i1,i2,i3,0)/coeffn
              du1 =  u(j1,j2,j3,1)-eqn2d(i1,i2,i3,1)/coeffn

              nDotU = an1*du0+an2*du1  ! normal component of ghost point value

              ! **** check these again ***
              ! BC equations for the tangential component -- a combination of orthogonality and smoothing 
              ! We solve for the ghost point (but lag the neighbouring ghost pts that appear in the eqn)
              ! BC: (orthogonality says tau.(u.r)=0 
              !     tau.[  (u(i1+1)-u(i1-1))/(2*dr) -alpha*( D0r(u(i2+1))-2.*D0r(u(i2))+D02(u(i2-1)) ) = 0
              if( axis.eq.0 )then
                coefft = -is(0)*(1.+2.*alpha)/(2.*dr(1))  ! coefficient of ghost point in BC eqn
                du0 =  u(j1,j2,j3,0)-bcEqn2d0(i1,i2,i3,0)/coefft
                du1 =  u(j1,j2,j3,1)-bcEqn2d0(i1,i2,i3,1)/coefft
              else
                coefft = -is(1)*(1.+2.*alpha)/(2.*dr(2))   ! coefficient of ghost point in eqn
                du0 =  u(j1,j2,j3,0)-bcEqn2d1(i1,i2,i3,0)/coefft
                du1 =  u(j1,j2,j3,1)-bcEqn2d1(i1,i2,i3,1)/coefft
              end if

              tauDotU = tau1*du0+tau2*du1  ! tangential component of ghost point value

              ! Here are the new ghost point values:
              u(j1,j2,j3,0)= nDotU*an1 + tauDotU*tau1        ! should we under-relax these?
              u(j1,j2,j3,1)= nDotU*an2 + tauDotU*tau2

            end do
            end do
            end do



            ! ------------- corner ghost points --------------------

            ! At the corner ghost point we apply the two conditions:
            !     tau0.[  (u(i1+1)-u(i1-1))/(2*dr) -alpha*( D0r(u(i2+1))-2.*D0r(u(i2))+D0r(u(i2-1)) ) = 0
            !     tau1.[  (u(i2+1)-u(i2-1))/(2*ds) -alpha*( D0s(u(i1+1))-2.*D0s(u(i1))+D0s(u(i1-1)) ) = 0

            ! **** this assumes alpha!=0 *****

! getBoundaryIndexBoundsMacro(side,axis)
             m1a=n1a
             m1b=n1b
             m2a=n2a
             m2b=n2b
             m3a=n3a
             m3b=n3b
             if( side.eq.0 .and. axis.eq.0 )then
               m1b=m1a
             else if( side.eq.1 .and. axis.eq.0 )then
               m1a=m1b
             else if( side.eq.0 .and. axis.eq.1 )then
               m2b=m2a
             else if( side.eq.1 .and. axis.eq.1 )then
               m2a=m2b
             else if( side.eq.0 .and. axis.eq.2 )then
               m3b=m3a
             else if( side.eq.1 .and. axis.eq.2 )then
               m3a=m3b
             end if

            i3=m3a
            j3=i3
            do side2=0,1  ! adjacent sides
             if( bc(side2,axisp1).gt.periodic )then
              ! *** should we do all adjacent sides -- what if adjacent side is not sliding?

              is2=1-2*side2
              ! (i1,i2,i3) holds the corner point
              ! (j1,j2,j3) holds the ghost point
              i1=m1a
              i2=m2a
              if( axis.eq.0 )then
                if( side.eq.1 )then
                  i1=m1b
                end if
                if( side2.eq.1 )then
                  i2=m2b
                end if
                j1=i1-is1
                j2=i2-is2
              else if( axis.eq.1 ) then
                if( side.eq.1 )then
                  i2=m2b
                end if
                if( side2.eq.1 )then
                  i1=m1b
                end if
                j1=i1-is2
                j2=i2-is1
              end if

              xr=ur2(i1,i2,i3,0)
              xs=us2(i1,i2,i3,0)
              yr=ur2(i1,i2,i3,1)
              ys=us2(i1,i2,i3,1)

              tau1=xs
              tau2=ys
              a11=tau1
              a12=tau2
              coefft = is1*(alpha)/(2.*dr(1))  ! coefficient of corner ghost point in BC eqn
              du0 =  u(j1,j2,j3,0)-bcEqn2d0(i1,i2,i3,0)/coefft
              du1 =  u(j1,j2,j3,1)-bcEqn2d0(i1,i2,i3,1)/coefft
              f1 = tau1*du0+tau2*du1  ! tangential component of ghost point value

              tau1=xr
              tau2=yr
              a21=tau1
              a22=tau2
              coefft = is2*(alpha)/(2.*dr(2))   ! coefficient of corner ghost point in eqn
              du0 =  u(j1,j2,j3,0)-bcEqn2d1(i1,i2,i3,0)/coefft
              du1 =  u(j1,j2,j3,1)-bcEqn2d1(i1,i2,i3,1)/coefft
              f2 = tau1*du0+tau2*du1  ! tangential component of ghost point value

              ! Solve :    tau0.u(j1,j2) = tau0DotU
              !            tau1.u(j1,j2) = tau1DotU

              det = a11*a22-a12*a21
              if( .false. .and. abs(det).gt.eps ) then
                u(j1,j2,j3,0) = (f1*a22 - f2*a12)/det
                u(j1,j2,j3,1) = (f2*a11 - f1*a21)/det
              else
                ! backup values:
                ! write(*,'("ellipticSmooth:WARNING: |det|<eps for corner ghost! (j1,j2)=(",i3,",",i3,")")') j1,j2
                u(j1,j2,j3,0)=2.*u(i1,i2,i3,0)-u(2*i1-j1,2*i2-j2,i3,0)
                u(j1,j2,j3,1)=2.*u(i1,i2,i3,1)-u(2*i1-j1,2*i2-j2,i3,1)

                ! u.rs=0 : 
                ! u(j1,j2,j3,0)=u(2*i1-j1,j2,i3,0)+u(j1,2*i2-j2,i3,0)-u(2*i1-j1,2*i2-j2,i3,0)
                ! u(j1,j2,j3,1)=u(2*i1-j1,j2,i3,1)+u(j1,2*i2-j2,i3,1)-u(2*i1-j1,2*i2-j2,i3,1)
              end if
             end if
            end do ! end side2

          end if ! end bc

        end do ! end axis
        end do ! end side


      else if( md.eq.3 .and. nd.eq.3 )then

        do i3=n3a,n3b
        do i2=n2a,n2b
        do i1=n1a,n1b
          xr=ur2(i1,i2,i3,0)
          xs=us2(i1,i2,i3,0)
          xt=ut2(i1,i2,i3,0)
          yr=ur2(i1,i2,i3,1)
          ys=us2(i1,i2,i3,1)
          yt=ut2(i1,i2,i3,1)
          zr=ur2(i1,i2,i3,2)
          zs=us2(i1,i2,i3,2)
          zt=ut2(i1,i2,i3,2)

          a11 = xr*xr+yr*yr+zr*zr
          a22 = xs*xs+ys*ys+zs*zs
          a33 = xt*xt+yt*yt+zt*zt
          a12 = xr*xs+yr*ys+zr*zs
          a13 = xr*xt+yr*yt+zr*zt
          a23 = xs*xt+ys*yt+zs*zt

          coeff0=a22*a33-a23*a23
          coeff1=a33*a11-a13*a13
          coeff2=a11*a22-a12*a12
          coeff3=2.*(a13*a23-a12*a33)
          coeff4=2.*(a23*a12-a13*a22)
          coeff5=2.*(a13*a12-a23*a11)

          diag=(.5*omega(i1,i2,i3))/
     &       (coeff0*d22(1)+coeff1*d22(2)+coeff2*d22(3))

          u(i1,i2,i3,0)=u(i1,i2,i3,0)+eqn3d(i1,i2,i3,0)*diag
          u(i1,i2,i3,1)=u(i1,i2,i3,1)+eqn3d(i1,i2,i3,1)*diag
          u(i1,i2,i3,2)=u(i1,i2,i3,2)+eqn3d(i1,i2,i3,2)*diag

        end do
        end do
        end do

      else if( md.eq.2 .and. nd.eq.3 )then
        ! ***** surface grids *****
        !   x(r,s,t) = x(r,s) + normal(r,s)*t
        !   x_t(r,s,t) = normal(r,s)
        !   x_tt(r,s) = 0
        do i3=n3a,n3b
        do i2=n2a,n2b
        do i1=n1a,n1b
          xr=ur2(i1,i2,i3,0)
          xs=us2(i1,i2,i3,0)
          xt=normal(i1,i2,i3,0)
          yr=ur2(i1,i2,i3,1)
          ys=us2(i1,i2,i3,1)
          yt=normal(i1,i2,i3,1)
          zr=ur2(i1,i2,i3,2)
          zs=us2(i1,i2,i3,2)
          zt=normal(i1,i2,i3,2)

          a11 = xr*xr+yr*yr+zr*zr
          a22 = xs*xs+ys*ys+zs*zs
          a33 = xt*xt+yt*yt+zt*zt
          a12 = xr*xs+yr*ys+zr*zs
          a13 = xr*xt+yr*yt+zr*zt
          a23 = xs*xt+ys*yt+zs*zt

          coeff0=a22*a33-a23*a23
          coeff1=a33*a11-a13*a13
          coeff2=a11*a22-a12*a12
          coeff3=2.*(a13*a23-a12*a33)
          coeff4=2.*(a23*a12-a13*a22)
          coeff5=2.*(a13*a12-a23*a11)

          diag=(.5*omega(i1,i2,i3))/
     &            (coeff0*d22(1)+coeff1*d22(2)+coeff2*d22(3))

          u(i1,i2,i3,0)=u(i1,i2,i3,0)+eqn23d(i1,i2,i3,0)*diag
          u(i1,i2,i3,1)=u(i1,i2,i3,1)+eqn23d(i1,i2,i3,1)*diag
          u(i1,i2,i3,2)=u(i1,i2,i3,2)+eqn23d(i1,i2,i3,2)*diag

        end do
        end do
        end do

      else
        write(*,*) 'ERROR: invalid value for nd=',nd
        stop 3
      end if

      else if( option.eq.1 )then

        ! ******************************************
        ! ********* Compute the Control Function ***
        ! ******************************************
      if( nd.eq.2 )then

        do i3=n3a,n3b
        do i2=n2a,n2b
        do i1=n1a,n1b
          xr=ur2(i1,i2,i3,0)
          xs=us2(i1,i2,i3,0)
          yr=ur2(i1,i2,i3,1)
          ys=us2(i1,i2,i3,1)

          coeff0=xs*xs+ys*ys
          coeff1=xr*xr+yr*yr
          coeff2=-2.*(xr*xs+yr*ys)

          r1=-ceqn2d(i1,i2,i3,0)
          r2=-ceqn2d(i1,i2,i3,1)

          ! solve:    coeff0*xr*P+coeff1*xs*Q = r1
          !           coeff0*yr*P+coeff1*ys*Q = r2
          a11=coeff0*xr
          a12=coeff1*xs
          a21=coeff0*yr
          a22=coeff1*ys
          det=a11*a22-a12*a21   ! what if det==0 ??
          if( det.eq.0. )then
            write(*,*) 'ERROR: det=0. at i=',i1,i2
            det=1.
          end if
          source(i1,i2,i3,0)=(a22*r1-a12*r2)/det
          source(i1,i2,i3,1)=(a11*r2-a21*r1)/det

        end do
        end do
        end do

      else if( md.eq.3 .and. nd.eq.3 )then

        do i3=n3a,n3b
        do i2=n2a,n2b
        do i1=n1a,n1b
          xr=ur2(i1,i2,i3,0)
          xs=us2(i1,i2,i3,0)
          xt=ut2(i1,i2,i3,0)
          yr=ur2(i1,i2,i3,1)
          ys=us2(i1,i2,i3,1)
          yt=ut2(i1,i2,i3,1)
          zr=ur2(i1,i2,i3,2)
          zs=us2(i1,i2,i3,2)
          zt=ut2(i1,i2,i3,2)

          a11 = xr*xr+yr*yr+zr*zr
          a22 = xs*xs+ys*ys+zs*zs
          a33 = xt*xt+yt*yt+zt*zt
          a12 = xr*xs+yr*ys+zr*zs
          a13 = xr*xt+yr*yt+zr*zt
          a23 = xs*xt+ys*yt+zs*zt

          coeff0=a22*a33-a23*a23
          coeff1=a33*a11-a13*a13
          coeff2=a11*a22-a12*a12
          coeff3=2.*(a13*a23-a12*a33)
          coeff4=2.*(a23*a12-a13*a22)
          coeff5=2.*(a13*a12-a23*a11)

          r1=-ceqn3d(i1,i2,i3,0)
          r2=-ceqn3d(i1,i2,i3,1)
          r3=-ceqn3d(i1,i2,i3,2)

          ! solve:    coeff0*xr*P+coeff1*xs*Q+coeff2*xt*R = r1
          !           coeff0*yr*P+coeff1*ys*Q+coeff2*yt*R = r2
          !           coeff0*zr*P+coeff1*zs*Q+coeff2*zt*R = r3
          a11=coeff0*xr
          a12=coeff1*xs
          a13=coeff2*xt
          a21=coeff0*yr
          a22=coeff1*ys
          a23=coeff2*yt
          a31=coeff0*zr
          a32=coeff1*zs
          a33=coeff2*zt

          det = a11*(a22*a33-a23*a32)+
     &  a21*(a32*a13-a33*a12)+
     &  a31*(a12*a23-a13*a22)

          if( det.eq.0. )then
            write(*,*) 'ERROR: det=0. at i=',i1,i2,i3
            det=1.
          end if
          source(i1,i2,i3,0)=((a22*a33-a23*a32)*r1+
     &                        (a32*a13-a33*a12)*r2+
     &                        (a12*a23-a13*a22)*r3)/det
          source(i1,i2,i3,1)=((a23*a31-a21*a33)*r1+
     &                        (a33*a11-a31*a13)*r2+
     &                        (a13*a21-a11*a23)*r3)/det
          source(i1,i2,i3,2)=((a21*a32-a22*a31)*r1+
     &                        (a31*a12-a32*a11)*r2+
     &                        (a11*a22-a12*a21)*r3)/det

        end do
        end do
        end do

      else if( md.eq.2 .and. nd.eq.3 )then
        ! ***** surface grids *****
        !   x(r,s,t) = x(r,s) + normal(r,s)*t
        !   x_t(r,s,t) = normal(r,s)
        do i3=n3a,n3b
        do i2=n2a,n2b
        do i1=n1a,n1b
          xr=ur2(i1,i2,i3,0)
          xs=us2(i1,i2,i3,0)
          xt=normal(i1,i2,i3,0)
          yr=ur2(i1,i2,i3,1)
          ys=us2(i1,i2,i3,1)
          yt=normal(i1,i2,i3,1)
          zr=ur2(i1,i2,i3,2)
          zs=us2(i1,i2,i3,2)
          zt=normal(i1,i2,i3,2)

          a11 = xr*xr+yr*yr+zr*zr
          a22 = xs*xs+ys*ys+zs*zs
          a33 = xt*xt+yt*yt+zt*zt
          a12 = xr*xs+yr*ys+zr*zs
          a13 = xr*xt+yr*yt+zr*zt
          a23 = xs*xt+ys*yt+zs*zt

          coeff0=a22*a33-a23*a23
          coeff1=a33*a11-a13*a13
          coeff2=a11*a22-a12*a12
          coeff3=2.*(a13*a23-a12*a33)
          coeff4=2.*(a23*a12-a13*a22)
          coeff5=2.*(a13*a12-a23*a11)

          r1=-ceqn23d(i1,i2,i3,0)
          r2=-ceqn23d(i1,i2,i3,1)
          r3=-ceqn23d(i1,i2,i3,2)

          ! solve:    coeff0*xr*P+coeff1*xs*Q = r1
          !           coeff0*yr*P+coeff1*ys*Q = r2
          !           coeff0*zr*P+coeff1*zs*Q = r3
          a11=coeff0*xr
          a12=coeff1*xs
          a21=coeff0*yr
          a22=coeff1*ys
          a31=coeff0*zr
          a32=coeff1*zs

          ! solve by least squares  B = A*A
          b11=a11**2+a21**2+a31**2
          b12=a11*a12+a21*a22+a31*a32
          b21=b12
          b22=a12**2+a22**2+a32**2

          s1=a11*r1+a21*r2+a31*r3
          s2=a12*r1+a22*r2+a32*r3

          det=b11*b22-b12*b21   ! what if det==0 ??
          if( det.eq.0. )then
            write(*,*) 'ERROR: det=0. at i=',i1,i2
            det=1.
          end if
          source(i1,i2,i3,0)=(b22*s1-b12*s2)/det
          source(i1,i2,i3,1)=(b11*s2-b21*s1)/det

        end do
        end do
        end do

      else
        write(*,*) 'ERROR: invalid value for nd=',nd
        stop 3
      end if


      else
      write(*,*) 'ERROR: invalid value for option=',option
        stop 3
      end if


      return
      end



      subroutine fixedControlFunctions( md,nd,
     &    nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &    n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c, f, dr,
     & ndipar,ndrpar,npar, ipar, rpar )
c ======================================================================================
c   Evaulate the control functions that are fixed.
c
c  md,nd : domain and range dimensions
c ======================================================================================
      implicit none
      integer md,nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &        n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c,
     &        ndipar,ndrpar,npar
      real f(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      real dr(3),rpar(0:ndrpar-1,0:npar-1)
      integer ipar(0:ndipar-1,0:npar-1)
c --- local variables
      real r,a,b,c
      integer i1,i2,i3,m,n
      integer lineAttraction, pointAttraction
      parameter( lineAttraction=1,pointAttraction=2 )
c ---------begin statement functions-------------
c -----------end statement functions------------------

      do n=0,npar-1
        if( ipar(0,n).eq.lineAttraction .and. rpar(0,n).ne.0. )then
          m=ipar(1,n) ! axis
          a=rpar(0,n)
          b=rpar(1,n)
          c=rpar(2,n)
          write(*,*) 'line attract: m,a,b,c=',m,a,b,c
          if( m.eq.0 )then
            ! attraction of r1 lines to r1=c
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
              r=i1*dr(1)
              f(i1,i2,i3,m)=f(i1,i2,i3,m)-a*sign(exp(-b*abs(r-c)),r-c) ! sign(a,b)
            end do
            end do
            end do
          else if( m.eq.1 )then
            ! attraction of r2 lines to r2=c
            do i3=n3a,n3b
            do i2=n2a,n2b
              r=i2*dr(2)
            do i1=n1a,n1b
              f(i1,i2,i3,m)=f(i1,i2,i3,m)-a*sign(exp(-b*abs(r-c)),r-c)
            end do
            end do
            end do
          else if( m.eq.2 .and. m.lt.md )then
            ! attraction of r3 lines to r3=c
            do i3=n3a,n3b
              r=i3*dr(3)
            do i2=n2a,n2b
            do i1=n1a,n1b
              f(i1,i2,i3,m)=f(i1,i2,i3,m)-a*sign(exp(-b*abs(r-c)),r-c)
            end do
            end do
            end do
          end if
        end if
      end do

      return
      end


      subroutine smoothSurfaceNormals( nd,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &  n1a,n1b,n2a,n2b,n3a,n3b, nit, omega, normal, normal2  )
c ======================================================================================
c   Smooth the normals for a surface grid
c
c  nd : must be 3
c  nit : number of iterations (actually we do 2*nit iterations)
c  normal : input/output
c  normal2 work space
c ======================================================================================
      implicit none
      integer md,nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &        n1a,n1b,n2a,n2b,n3a,n3b,nit
      real normal(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      real normal2(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      real omega
c --- local variables

      integer i1,i2,i3,it,m
      real oneMinusOmega,omegaBy6,norm
      real eqn,eqn2
c ---------begin statement functions-------------
      eqn(i1,i2,i3,m)=oneMinusOmega*normal(i1,i2,i3,m)+
     & omegaBy6*(normal(i1-1,i2,i3,m)+normal(i1+1,i2,i3,m)+
     &           normal(i1,i2-1,i3,m)+normal(i1,i2+1,i3,m)+
     &           normal(i1,i2,i3-1,m)+normal(i1,i2,i3+1,m))
      eqn2(i1,i2,i3,m)=oneMinusOmega*normal2(i1,i2,i3,m)+
     & omegaBy6*(normal2(i1-1,i2,i3,m)+normal2(i1+1,i2,i3,m)+
     &           normal2(i1,i2-1,i3,m)+normal2(i1,i2+1,i3,m)+
     &           normal2(i1,i2,i3-1,m)+normal2(i1,i2,i3+1,m))

c -----------end statement functions------------------

      if( nd.ne.3 )then
        write(*,*) 'smoothNormals:ERROR nd.ne.3'
        stop 1
      end if

      omegaBy6=omega/6.
      oneMinusOmega=1.-omega
      i3=n3a
      do it=1,nit
        ! form symmetry we use a jacobi iteration instead of Gauss-Seidel
        do i2=n2a,n2b
        do i1=n1a,n1b
          normal2(i1,i2,i3,0)=eqn(i1,i2,i3,0)
          normal2(i1,i2,i3,1)=eqn(i1,i2,i3,1)
          normal2(i1,i2,i3,2)=eqn(i1,i2,i3,2)
          norm=1./sqrt(normal2(i1,i2,i3,0)**2+
     &                 normal2(i1,i2,i3,1)**2+
     &                 normal2(i1,i2,i3,2)**2)
          normal2(i1,i2,i3,0)=normal2(i1,i2,i3,0)*norm
          normal2(i1,i2,i3,1)=normal2(i1,i2,i3,1)*norm
          normal2(i1,i2,i3,2)=normal2(i1,i2,i3,2)*norm
        end do
        end do

        ! boundary conditions
        do i1=n1a,n1b
          normal2(i1,n2a-1,i3,0)=normal2(i1,n2a,i3,0)
          normal2(i1,n2a-1,i3,1)=normal2(i1,n2a,i3,1)
          normal2(i1,n2a-1,i3,2)=normal2(i1,n2a,i3,2)

          normal2(i1,n2b+1,i3,0)=normal2(i1,n2b,i3,0)
          normal2(i1,n2b+1,i3,1)=normal2(i1,n2b,i3,1)
          normal2(i1,n2b+1,i3,2)=normal2(i1,n2b,i3,2)
        end do
        do i2=n2a-1,n2b+1
          normal2(n1a-1,i2,i3,0)=normal2(n1a,i2,i3,0)
          normal2(n1a-1,i2,i3,1)=normal2(n1a,i2,i3,1)
          normal2(n1a-1,i2,i3,2)=normal2(n1a,i2,i3,2)

          normal2(n1b+1,i2,i3,0)=normal2(n1b,i2,i3,0)
          normal2(n1b+1,i2,i3,1)=normal2(n1b,i2,i3,1)
          normal2(n1b+1,i2,i3,2)=normal2(n1b,i2,i3,2)
        end do

        do i2=n2a,n2b
        do i1=n1a,n1b
          normal(i1,i2,i3,0)=eqn2(i1,i2,i3,0)
          normal(i1,i2,i3,1)=eqn2(i1,i2,i3,1)
          normal(i1,i2,i3,2)=eqn2(i1,i2,i3,2)
          norm=1./sqrt(normal(i1,i2,i3,0)**2+
     &                 normal(i1,i2,i3,1)**2+
     &                 normal(i1,i2,i3,2)**2)
          normal(i1,i2,i3,0)=normal(i1,i2,i3,0)*norm
          normal(i1,i2,i3,1)=normal(i1,i2,i3,1)*norm
          normal(i1,i2,i3,2)=normal(i1,i2,i3,2)*norm
        end do
        end do

        ! boundary conditions
        do i1=n1a,n1b
          normal(i1,n2a-1,i3,0)=normal(i1,n2a,i3,0)
          normal(i1,n2a-1,i3,1)=normal(i1,n2a,i3,1)
          normal(i1,n2a-1,i3,2)=normal(i1,n2a,i3,2)

          normal(i1,n2b+1,i3,0)=normal(i1,n2b,i3,0)
          normal(i1,n2b+1,i3,1)=normal(i1,n2b,i3,1)
          normal(i1,n2b+1,i3,2)=normal(i1,n2b,i3,2)
        end do
        do i2=n2a-1,n2b+1
          normal(n1a-1,i2,i3,0)=normal(n1a,i2,i3,0)
          normal(n1a-1,i2,i3,1)=normal(n1a,i2,i3,1)
          normal(n1a-1,i2,i3,2)=normal(n1a,i2,i3,2)

          normal(n1b+1,i2,i3,0)=normal(n1b,i2,i3,0)
          normal(n1b+1,i2,i3,1)=normal(n1b,i2,i3,1)
          normal(n1b+1,i2,i3,2)=normal(n1b,i2,i3,2)
        end do

      end do

      return
      end
